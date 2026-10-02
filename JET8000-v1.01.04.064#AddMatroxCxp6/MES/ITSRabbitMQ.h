// ITSRabbitMQ.h: interface for the CITSRabbitMQ class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITSRABBITMQ_H__828E2168_819E_4C61_930F_FA07DA73B927__INCLUDED_)
#define AFX_ITSRABBITMQ_H__828E2168_819E_4C61_930F_FA07DA73B927__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include "JetRabbitmqCtrl.h"
//-------------------------------------------------------------------------------------//
using namespace JetRabbitmqCtrlStuff;
//-------------------------------------------------------------------------------------//
class CITSLinker;
//-------------------------------------------------------------------------------------//
class CITSRabbitMQ  
{
private:
	//---------------------------------------------------------------------------------//
	CITSLinker                *m_ITSLinkerPtr;
	//---------------------------------------------------------------------------------//
	bool                       m_Connected;
	std::string                m_SendName;
	std::vector<int>           m_ReceIdList;
	std::vector<std::string>   m_RecvMsgList;//收到的訊息列表	
	//---------------------------------------------------------------------------------//	
	HWND                       m_hWnd;
	UINT                       m_uMsg;
	WPARAM                     m_wRecv;
	WPARAM                     m_wSend;
	//---------------------------------------------------------------------------------//
	bool                       m_ThreadStop;
	bool                       m_ThreadExit;
	DWORD                      m_ThreadSleepTime;
	//---------------------------------------------------------------------------------//	
	CString                    m_ErrorString;
	//---------------------------------------------------------------------------------//
	unsigned int               m_thRecvID;  //接收訊息執行緒編號
	HANDLE                     m_thRecvHandle; //接收訊息執行緒處理碼
	//---------------------------------------------------------------------------------//
	CRITICAL_SECTION           m_csRecvThread;
	CRITICAL_SECTION           m_csRecvMsgList;	
	//---------------------------------------------------------------------------------//
protected:	
	//---------------------------------------------------------------------------------//
	CITSRabbitMQ(const CITSRabbitMQ &Rabbit);
	CITSRabbitMQ& operator=(const CITSRabbitMQ &Rabbit);
	//---------------------------------------------------------------------------------//	
	bool                       CloseThread_Recv();
	bool                       CreateThread_Recv();
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	CITSRabbitMQ();
	virtual ~CITSRabbitMQ();
	//---------------------------------------------------------------------------------//
	CITSLinker*                GetITSLinkerPtr();	
	void                       SetITSLinkerPtr(CITSLinker *Ptr);	
	//---------------------------------------------------------------------------------//
	LPCTSTR                    GetErrorString() const;
	//---------------------------------------------------------------------------------//	
	void                       SetHWnd(HWND h, UINT m, WPARAM wRecv, WPARAM wSend);
	//---------------------------------------------------------------------------------//	
	void                       LockRecvThread();
	void                       UnockRecvThread();
	//---------------------------------------------------------------------------------//
	void                       LockRecvMsgList();
	void                       UnlockRecvMsgList();
	//---------------------------------------------------------------------------------//	
	bool                       GetThreadStop() const;
	void                       SetThreadStop(bool val);	
	//---------------------------------------------------------------------------------//
	bool                       GetThreadExit() const;
	void                       SetThreadExit(bool val);	
	//---------------------------------------------------------------------------------//	
	DWORD                      GetThreadSleepTime() const;
	void                       SetThreadSleepTime(DWORD val);	
	//---------------------------------------------------------------------------------//
	void                       ShowRecv(const char *Str);
	void                       ShowSend(const char *Str);
	//---------------------------------------------------------------------------------//
	bool                       ExecThread_Recv();
	//---------------------------------------------------------------------------------//
	bool                       ConnectServer(const std::string &rIpAddr, int Port = 5672);
	void                       Disconnect();
	//---------------------------------------------------------------------------------//	
	void                       Login(const std::string &rUser, const std::string &rPwd, const std::string &rVHost);
	//---------------------------------------------------------------------------------//
	void                       RegisterSendName(const std::string &Name);
	bool                       RegisterReceiveName(const std::string &Name);
	//---------------------------------------------------------------------------------//
	void                       Fire();
	//---------------------------------------------------------------------------------//
	bool                       IsLoginSucc() const;
	//---------------------------------------------------------------------------------//
	bool                       Send(const std::string &rData);
	//---------------------------------------------------------------------------------//
	bool                       Receive(int Id, std::queue<std::string> &rData);	
	//---------------------------------------------------------------------------------//
	bool                       AddRecvMsg(const std::string &msg);	
	bool                       CloneRecvMsgList(std::vector<std::string> &sList, bool bClear);//取得接收訊息列表		
	//---------------------------------------------------------------------------------//
};
//-------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_ITSRABBITMQ_H__828E2168_819E_4C61_930F_FA07DA73B927__INCLUDED_)
