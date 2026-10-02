#ifndef JET_RABBITMQ_CTRL_H_
#define JET_RABBITMQ_CTRL_H_

#include <memory>
#include <string>
#include <queue>
#include <vector>

namespace JetRabbitmqCtrlStuff
{
	enum ProtocolType
	{
		CT_QUEUE_DIR,
		CT_FANOUT,
		CT_DIRECT,
		CT_TOPIC
	};

	class CConnectObj
	{
		ProtocolType m_Type;
		std::string m_QueueName;
		std::vector<std::string> m_vSeverity;

		void Swap(const CConnectObj &other)
		{
			m_Type		= other.m_Type;
			m_QueueName	= other.m_QueueName;
			m_vSeverity	= other.m_vSeverity;
		}
	public:
		CConnectObj() : m_QueueName(""), m_vSeverity({ "" }), m_Type(CT_QUEUE_DIR)
		{}		

		CConnectObj(ProtocolType Ty, const std::string &KeyName)
			: m_Type(Ty), m_QueueName(KeyName), m_vSeverity({ "" })
		{
		}

		CConnectObj(ProtocolType Ty, const std::string &KeyName, const std::string &Severity)
			: m_Type(Ty), m_QueueName(KeyName), m_vSeverity({ Severity })
		{
		}

		CConnectObj(ProtocolType Ty, const std::string &KeyName, const std::vector<std::string> &SList)
			: m_Type(Ty), m_QueueName(KeyName), m_vSeverity(SList)
		{
		}

		CConnectObj(const CConnectObj &other)
		{
			Swap(other);
		}

		~CConnectObj()
		{}

		CConnectObj &operator=(const CConnectObj rhs)
		{
			Swap(CConnectObj(rhs));
			return *this;
		}

		ProtocolType GetProtocolType() const
		{
			return m_Type;
		}

		void SetProtocolType(ProtocolType Type)
		{
			m_Type = Type;
		}

		auto GetQueueName() const -> const std::string &
		{
			return m_QueueName;
		}

		void SetQueueName(const std::string &rQName)
		{
			m_QueueName = rQName;
		}

		auto GetKeyList() const -> const std::vector<std::string> &
		{
			return m_vSeverity;
		}

		void SetKeyList(const std::vector<std::string> &rList)
		{
			m_vSeverity = rList;
		}

		auto GetKeyName(int Id) const ->const std::string &
		{
			return m_vSeverity[Id];
		}

		void SetKeyName(int Id, const std::string &rName)
		{
			m_vSeverity[Id] = rName;
		}
	};

	class CJetRabbitmqCtrl
	{		
		bool           m_Destory;
		std::shared_ptr<class CJetRabbitmqCtrlImpl> m_impl;		

		CJetRabbitmqCtrl();

	public:		

		virtual ~CJetRabbitmqCtrl();

		friend CJetRabbitmqCtrl &theRabbitmqUnit();

		std::string& GetLastError() const;

		bool ConnectServer(const std::string &rIpAddr, int Port = 5672);

		void Login(const std::string &rUser, const std::string &rPwd, const std::string &rVHost);

		int RegisterReceiveName(const CConnectObj &rParas);		

		void Fire();

		bool IsLoginSucc() const;
		
		bool Send(const CConnectObj &rParas, const std::string &rData);

		bool Receive(int Id, std::queue<std::string> &rData);	

		void Disconnect();
	};

	CJetRabbitmqCtrl &theRabbitmqUnit();
}

#endif
