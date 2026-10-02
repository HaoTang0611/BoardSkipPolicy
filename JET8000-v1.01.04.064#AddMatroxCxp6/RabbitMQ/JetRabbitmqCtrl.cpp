#include "stdafx.h"
#include "JetRabbitmqCtrl.h"
#include "SimplePocoHandler.h"
#include <unordered_map>
#include <mutex>
#include <tuple>
#include <algorithm>

namespace JetRabbitmqCtrlStuff
{
	const int g_knTopicIdOffset = 128;

	static std::shared_ptr<std::thread > g_ProcessThread;

	class CJetRabbitmqCtrlImpl
	{
		void SetConnectType(int CId, const CConnectObj &rType, 
			std::unordered_map<int, CConnectObj> &rpaConncetType)
		{
			//m_ConnectType.insert(std::pair<int, CConnectObj>(CId, rType));
			rpaConncetType.insert(std::pair<int, CConnectObj>(CId, rType));
		}		

		void FreeProcThread()
		{
			if (m_bInitialSucc)
			{
				try
				{
					m_logSucc = false;
					m_bInitialSucc = false;

					handler->quit();
					g_ProcessThread->join();

					channel->close();
					connection->close();				
				}
				catch (...)
				{

				}				
			}
		}

	public:
		CJetRabbitmqCtrlImpl()
		{
			m_logSucc = false;
			m_bInitialSucc = false;
		}


		~CJetRabbitmqCtrlImpl()
		{
			FreeProcThread();
		}

		bool								m_logSucc;
		bool                                m_bInitialSucc;

		std::string							m_LastError;

		std::shared_ptr<SimplePocoHandler>	handler;

		std::shared_ptr<AMQP::Connection>	connection;

		std::shared_ptr<AMQP::Channel>		channel;

		static std::unordered_map<int, CConnectObj> m_ConnectType;
		static std::unordered_map<int, CConnectObj>	m_ConnectTypeTopic;

		static std::mutex				m_ReceiveMutexQ;
		static std::mutex				m_ReceiveMutexQTopic;

		static std::unordered_map<int, std::queue<std::string> > m_ReceiveQ;
		
		static std::unordered_map<int, std::queue<std::string> > m_ReceiveQTopic;

		bool ConnectServer(const std::string &rIpAddr, int Port)
		{
			try
			{
				m_logSucc = false;
				handler.reset(new SimplePocoHandler(rIpAddr, Port));				
				m_bInitialSucc = true;
			}
			catch (const std::exception &e)
			{
				m_LastError = e.what();				
				m_bInitialSucc = false;
				return false;
			}

			return true;
		}

		void Disconnect()
		{	
			FreeProcThread();
			//channel->close();
			//connection->close();
			m_ConnectType.clear();
			m_ConnectTypeTopic.clear();

			m_ReceiveQ.clear();
			m_ReceiveQTopic.clear();
		}

		void Login(const std::string &rUser, const std::string &rPwd, const std::string &rVHost)
		{
			connection.reset(new AMQP::Connection(handler.get(), AMQP::Login(rUser, rPwd), rVHost));			
			channel.reset(new AMQP::Channel(connection.get()));
			m_logSucc = false;
		}

		static int GetProtocolType(const std::string &exchange, const std::string &routingkey)
		{
			auto ECn = exchange.size();
			auto RCn = routingkey.size();
			//
			if ( (0 == ECn) && RCn)
				return CT_QUEUE_DIR;
			else if (ECn && (0 == RCn) )
				return CT_FANOUT;
			else if (ECn && RCn)
				return CT_DIRECT;			

			return -1;
		}

		static int CalcDataQueueId(const std::string &exchange, const std::string &routingkey)
		{
			int Type = GetProtocolType(exchange, routingkey);			

			for (const auto &itr : m_ConnectType)
			{
				const auto &rObj = itr.second;				
				if (Type == rObj.GetProtocolType())
				{
					if ( (CT_QUEUE_DIR == Type) && (routingkey == rObj.GetQueueName() ) )
					{
						return itr.first;
					}

					if ((exchange == rObj.GetQueueName()) &&
						(routingkey == rObj.GetKeyName(0)) ) return itr.first;
				}				
			}

			return -1;
		}

		static void receiveMessageCallback(const AMQP::Message &message,
			uint64_t deliveryTag,
			bool redelivered)
		{
			std::lock_guard<std::mutex> lock(m_ReceiveMutexQ);
			int QId = CalcDataQueueId(message.exchange(), message.routingkey());
			if (0 <= QId)
			{
				m_ReceiveQ[QId].push(std::string(message.body(), message.bodySize()));
			}
		}

		static int CalcTopicQueueId(const std::string &rQueueName)
		{
			for (auto &itr : m_ConnectTypeTopic)
			{
				if (rQueueName == itr.second.GetQueueName() )
				{
					return itr.first;
				}
			}

			return -1;
		}

		static void receiveMessageCallbackTopic(const AMQP::Message &message,
			uint64_t deliveryTag,
			bool redelivered)
		{
			std::lock_guard<std::mutex> lock(m_ReceiveMutexQTopic);
			int QId = CalcTopicQueueId(message.exchange());
			if (0 <= QId)
			{
				m_ReceiveQTopic[QId].push(std::string(message.body(), message.bodySize()));
			}
		}

		int GetRegisterId(const CConnectObj &rParas)
		{
			if (CT_TOPIC == rParas.GetProtocolType() )
			{				
				if ( CalcTopicQueueId(rParas.GetQueueName()) < g_knTopicIdOffset)
					return m_ConnectTypeTopic.size() + g_knTopicIdOffset;
				else
					return -1;
			}

			return m_ConnectType.size();
		}

		int RegisterReceiveName(const CConnectObj &rParas)
		{
			//queue or exchange is aleardy exist
			if (0 <= CalcDataQueueId(rParas.GetQueueName(), rParas.GetKeyName(0)) )
				return -1;

			int RId = GetRegisterId(rParas);

			if (RId < 0) return RId;

			switch (rParas.GetProtocolType() )
			{
			case CT_QUEUE_DIR:
				RegisterQueueName(RId, rParas);
			break;
			case CT_FANOUT:
				RegisterFanout_Direct(RId, rParas, AMQP::fanout);
			break;
			case CT_DIRECT:
				RegisterFanout_Direct(RId, rParas, AMQP::direct);
			break;
			case CT_TOPIC:
				RegisterTopic(RId, rParas, AMQP::topic);
			break;
			}

			return RId;
		}		

		void RegisterQueueName(int CId, const CConnectObj &rParas)
		{			
			channel->declareQueue(rParas.GetQueueName());

			//register recevie queue
			channel->consume(rParas.GetQueueName(), AMQP::noack).onReceived(receiveMessageCallback);

			SetConnectType(CId, rParas, m_ConnectType);
		}

		std::queue<std::pair<std::string, std::string> > m_rqDirect;
		void RegisterFanout_Direct(int CId, const CConnectObj &rParas, AMQP::ExchangeType eType)
		{
			channel->declareExchange(rParas.GetQueueName(), eType);

			//register
			m_rqDirect.push({ rParas.GetQueueName(), rParas.GetKeyName(0) });

			AMQP::QueueCallback callback = [&](const std::string &name, int msgcount, int consumercount)
			{
				auto paRName = m_rqDirect.front();
				channel->bindQueue(paRName.first, name, paRName.second);
				channel->consume(name, AMQP::noack).onReceived(receiveMessageCallback);
				m_rqDirect.pop();
			};

			channel->declareQueue(AMQP::exclusive).onSuccess(callback);

			SetConnectType(CId, rParas, m_ConnectType);
		}

		std::queue<std::pair<std::string, std::vector<std::string> > > m_rqTopic;
		void RegisterTopic(int CId, const CConnectObj &rParas, AMQP::ExchangeType eType)
		{
			channel->declareExchange(rParas.GetQueueName(), eType);

			//register
			m_rqTopic.push({ rParas.GetQueueName(), rParas.GetKeyList() });

			AMQP::QueueCallback callback = [&](const std::string &name,
				int msgcount,
				int consumercount)
			{
				auto qName = m_rqTopic.front().first;
				auto vbindingKeyList = m_rqTopic.front().second;
				std::for_each(vbindingKeyList.begin(), vbindingKeyList.end(),
					[&](const std::string &bindingKeys)
				{	
					channel->bindQueue(qName, name, bindingKeys);
					channel->consume(name, AMQP::noack).onReceived(receiveMessageCallbackTopic);
				});

				m_rqTopic.pop();
			};

			channel->declareQueue(AMQP::exclusive).onSuccess(callback);			

			SetConnectType(CId, rParas, m_ConnectTypeTopic);
		}

		void Fire()
		{
			g_ProcessThread.reset(new std::thread(&CJetRabbitmqCtrlImpl::ProcessBuff, this) );
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
		}

		void ProcessBuff()
		{
			channel->onReady([&]()
			{
				m_logSucc = true;
			});

			handler->loop();
		}

		bool IsLoginSucc()
		{
			return m_logSucc;
		}		

		bool Send(const CConnectObj &rParas, const std::string &rData)
		{
			if (m_logSucc)
			{
				auto QName = rParas.GetQueueName();
				switch (rParas.GetProtocolType())
				{
				case CT_QUEUE_DIR:
					channel->declareQueue(QName);
					channel->publish("", QName, rData);
				break;
				case CT_FANOUT:
					channel->declareExchange(QName, AMQP::fanout);
					channel->publish(QName, "", rData);
				break;
				case CT_DIRECT:
					channel->declareExchange(QName, AMQP::direct);
					channel->publish(QName, rParas.GetKeyName(0), rData);
				break;
				case CT_TOPIC:
				{					
					channel->declareExchange(QName, AMQP::topic);
					for (auto &itr : rParas.GetKeyList())
					{
						channel->publish(QName, itr, rData);
					}
				}
				break;
				}

				return true;
			}

			return false;
		}

		bool HasReceiveData(int CId, std::unordered_map<int, std::queue<std::string>	> &rReceiveQ)
		{
			if ((rReceiveQ.end() != rReceiveQ.find(CId)) && rReceiveQ[CId].size())
				return true;
			//
			return false;
		}

		bool Receive(int CId, std::queue<std::string> &rData)
		{
			if (CId < g_knTopicIdOffset)
			{
				if (HasReceiveData(CId, m_ReceiveQ))				
				{
					std::lock_guard<std::mutex> lock(m_ReceiveMutexQ);
					rData = std::move(m_ReceiveQ[CId]);
					return true;
				}
			}
			else
			{
				if (HasReceiveData(CId, m_ReceiveQTopic))
				{
					std::lock_guard<std::mutex> lock(m_ReceiveMutexQTopic);
					rData = std::move(m_ReceiveQTopic[CId]);
					return true;
				}
			}

			return false;
		}
	};

	std::unordered_map<int, CConnectObj> CJetRabbitmqCtrlImpl::m_ConnectType;

	std::unordered_map<int, CConnectObj> CJetRabbitmqCtrlImpl::m_ConnectTypeTopic;

	std::mutex	CJetRabbitmqCtrlImpl::m_ReceiveMutexQ;

	std::mutex	CJetRabbitmqCtrlImpl::m_ReceiveMutexQTopic;

	std::unordered_map<int, std::queue<std::string> > CJetRabbitmqCtrlImpl::m_ReceiveQ;

	std::unordered_map<int, std::queue<std::string>	> CJetRabbitmqCtrlImpl::m_ReceiveQTopic;

	CJetRabbitmqCtrl::CJetRabbitmqCtrl() : m_Destory(false), m_impl(new CJetRabbitmqCtrlImpl())
	{
	}

	CJetRabbitmqCtrl::~CJetRabbitmqCtrl()
	{
		m_Destory = true;
	}

	std::string& CJetRabbitmqCtrl::GetLastError() const
	{	
		return m_impl->m_LastError;
	}

	bool CJetRabbitmqCtrl::ConnectServer(const std::string &rIpAddr, int Port)
	{
		if ( true == m_Destory ) { return false; }
		return m_impl->ConnectServer(rIpAddr, Port);
	}

	void CJetRabbitmqCtrl::Login(const std::string &rUser, const std::string &rPwd, const std::string &rVHost)
	{
		if ( true == m_Destory ) { return ; }
		m_impl->Login(rUser, rPwd, rVHost);
	}	

	int CJetRabbitmqCtrl::RegisterReceiveName(const CConnectObj &rParas)
	{
		if ( true == m_Destory ) { return -1; }
		return m_impl->RegisterReceiveName(rParas);
	}	

	void CJetRabbitmqCtrl::Fire()
	{
		if ( true == m_Destory ) { return ; }
		m_impl->Fire();
	}

	bool CJetRabbitmqCtrl::IsLoginSucc() const
	{
		if ( true == m_Destory ) { return false; }
		return m_impl->IsLoginSucc();
	}

	bool CJetRabbitmqCtrl::Send(const CConnectObj &rParas, const std::string &rData)
	{
		if ( true == m_Destory ) { return false; }
		return m_impl->Send(rParas, rData);
	}

	bool CJetRabbitmqCtrl::Receive(int Id, std::queue<std::string> &rData)
	{
		if ( true == m_Destory ) { return false; }
		return m_impl->Receive(Id, rData);
	}

	void CJetRabbitmqCtrl::Disconnect()
	{
		if ( true == m_Destory ) { return; }
		m_impl->Disconnect();		
	}

	CJetRabbitmqCtrl &theRabbitmqUnit()
	{
		static CJetRabbitmqCtrl theCtrl;
		return theCtrl;
	}
	
}