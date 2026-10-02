/* 
   Socket.h

   Copyright (C) 2002-2017 René Nyffenegger

   This source code is provided 'as-is', without any express or implied
   warranty. In no event will the author be held liable for any damages
   arising from the use of this software.

   Permission is granted to anyone to use this software for any purpose,
   including commercial applications, and to alter it and redistribute it
   freely, subject to the following restrictions:

   1. The origin of this source code must not be misrepresented; you must not
      claim that you wrote the original source code. If you use this source code
      in a product, an acknowledgment in the product documentation would be
      appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
      misrepresented as being the original source code.

   3. This notice may not be removed or altered from any source distribution.

   René Nyffenegger rene.nyffenegger@adp-gmbh.ch
*/

#ifndef JET_SOCKET_H
#define JET_SOCKET_H
#include <map>
#include <vector>
#include <string>
#include <WinSock2.h>
//-------------------------------------------------------------------------------------//
enum TypeSocket {BlockingSocket, NonBlockingSocket};
//-------------------------------------------------------------------------------------//
class CITSLinker;
//-------------------------------------------------------------------------------------//
class CJetSocket 
{
private:
	//---------------------------------------------------------------------------------//	
	static void Start();
	static void End();
	static int  nofSockets_;
	static unsigned int s_MsgPPID;
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//
	friend class CJetSocketServer;
	friend class CJetSocketSelect;	
	//---------------------------------------------------------------------------------//
	CJetSocket();
	CJetSocket(SOCKET s);
	//---------------------------------------------------------------------------------//
	void                       PreInit();
	void                       Clone(const CJetSocket&);	
	std::string&               GetWSAErrorString(int ErrorID);
	//---------------------------------------------------------------------------------//		
	CITSLinker                *m_ITSLinkerPtr;	
	//---------------------------------------------------------------------------------//
	CRITICAL_SECTION           m_csRecvThread;
	CRITICAL_SECTION           m_csRecvMsgList;		
	CRITICAL_SECTION           m_csSendMsgList;
	//---------------------------------------------------------------------------------//
	HWND                       m_hWnd;
	UINT                       m_uMsg;
	WPARAM                     m_wRecv;
	WPARAM                     m_wSend;
	SOCKET                     m_Socket;
	bool                       m_bclosed;	
	DWORD                      m_dwThreadSleep;	
	bool                       m_bUseRecvMsgList;//使用收到訊息列表	
	std::string                m_SrrorStr;
	std::string                m_RecvBuf;//收到的訊息
	std::string                m_SendBuf;//傳送的訊息
	std::string                m_WSAErrorStr;
	std::vector<std::string>   m_RecvMsgList;//收到的訊息列表	
	std::vector<std::string>   m_SendMsgList;//傳送的訊息列表	
	//---------------------------------------------------------------------------------//	
	int*                       refCounter_;
	//---------------------------------------------------------------------------------//	
	void                       LockRecvThread();
	void                       UnlockRecvThread();
	//---------------------------------------------------------------------------------//	
	void                       LockRecvMsgList();
	void                       UnlockRecvMsgList();
	//---------------------------------------------------------------------------------//
	void                       LockSendMsgList();
	void                       UnlockSendMsgList();
	//---------------------------------------------------------------------------------//
	bool                       m_bThreadStop_Recv;	
	bool                       m_bThreadExit_Recv;
	unsigned int               m_thRecvID = 0;  //接收訊息執行緒編號
	HANDLE                     m_thRecvHandle = NULL; //接收訊息執行緒處理碼
	//---------------------------------------------------------------------------------//	
	bool                       m_bThreadStop_Send;	
	bool                       m_bThreadExit_Send;
	unsigned int               m_thSendID = 0;  //傳送訊息執行緒編號
	HANDLE                     m_thSendHandle = NULL; //傳送訊息執行緒處理碼
	//---------------------------------------------------------------------------------//	
	virtual bool               CloseThread_Recv();//關閉接受訊息的執行續
	//---------------------------------------------------------------------------------//
	virtual bool               CloseThread_Send();//關閉傳送訊息的執行續
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	virtual ~CJetSocket();
	CJetSocket(const CJetSocket&);
	CJetSocket& operator=(CJetSocket&);
	//---------------------------------------------------------------------------------//
	CITSLinker*                GetITSLinkerPtr();	
	void                       SetITSLinkerPtr(CITSLinker *Ptr);	
	//---------------------------------------------------------------------------------//
	HWND                       GetHWnd();
	void                       ShowRecv(const char *Str);
	void                       ShowSend(const char *Str);
	void                       SetHWnd(HWND h, UINT msg, WPARAM wRecv, WPARAM wSend);
	//---------------------------------------------------------------------------------//
	bool                       CheckSocket();
	std::string&               GetErrorString();
	//---------------------------------------------------------------------------------//	
	void                       CloseSocket();	
	bool                       GetClosed();
	void                       SetClosed(bool val);
	//---------------------------------------------------------------------------------//
	int                        ReceiveLine(std::string &str);
	int                        ReceiveBytes(std::string &str);
	//---------------------------------------------------------------------------------//
	bool                       SendLine (std::string);
	bool                       SendBytes(const std::string&);
	//---------------------------------------------------------------------------------//
	int                        GetAddress(std::string &rAddr);
	//---------------------------------------------------------------------------------//	
	bool                       GetThreadStop_Recv() const;
	void                       SetThreadStop_Recv(bool val);
	//---------------------------------------------------------------------------------//	
	bool                       GetThreadStop_Send() const;
	void                       SetThreadStop_Send(bool val);
	//---------------------------------------------------------------------------------//	
	bool                       GetThreadExit_Recv() const;
	void                       SetThreadExit_Recv(bool val);
	//---------------------------------------------------------------------------------//	
	bool                       GetThreadExit_Send() const;
	void                       SetThreadExit_Send(bool val);
	//---------------------------------------------------------------------------------//			
	DWORD                      GetThreadSleepTime() const;
	//---------------------------------------------------------------------------------//	
	bool                       GetUseRecvMsgList() const;
	void                       SetUseRecvMsgList(bool val);
	//---------------------------------------------------------------------------------//	
	bool                       AddRecvMsg(const std::string &msg);	
	size_t                     GetRecvMsgCount() const;
	bool                       CloneRecvMsgList(std::vector<std::string> &sList, bool bClear);//取得接收訊息列表		
	//---------------------------------------------------------------------------------//	
	bool                       AddSendMsg(const std::string &msg);	
	bool                       RemoveSendMsg(size_t index);
	size_t                     GetSendMsgCount() const;
	//---------------------------------------------------------------------------------//	
	virtual bool               ExecThread_Recv();	
	virtual bool               CreateThread_Recv();
	//---------------------------------------------------------------------------------//
	virtual bool               ExecThread_Send();
	virtual bool               CreateThread_Send();
	//---------------------------------------------------------------------------------//
};
//-------------------------------------------------------------------------------------//
class CJetSocketClient : public CJetSocket 
{
protected:
	//---------------------------------------------------------------------------------//
	bool                       m_bConnected;
	//---------------------------------------------------------------------------------//
	void                       SetConnected(bool val);
	//---------------------------------------------------------------------------------//
public:  
	//---------------------------------------------------------------------------------//
	CJetSocketClient();
	~CJetSocketClient();
	CJetSocketClient(const std::string& host, int port);
	//---------------------------------------------------------------------------------//	
	bool                       GetConnected() const;
	//---------------------------------------------------------------------------------//	
	bool                       CloseClient();
	bool                       Connect(const std::string& host, int port);
	bool                       Connect(const std::string& host, int port, bool bThread);	
	//---------------------------------------------------------------------------------//
	virtual bool               ExecThread_Recv();
	//---------------------------------------------------------------------------------//	
};
//-------------------------------------------------------------------------------------//
//each client has own recv thread
class CJetSocketServer : public CJetSocket 
{		
protected:
	//---------------------------------------------------------------------------------//		
	CRITICAL_SECTION           m_csSocketList;
	//---------------------------------------------------------------------------------//		
	std::vector<CJetSocket*>   m_SocketPtrList;
	//---------------------------------------------------------------------------------//
	bool                       m_bThreadStop_Conn;	
	bool                       m_bThreadExit_Conn;
	unsigned int               m_thConnectionID;
	HANDLE                     m_thConnectionHandle;
	//---------------------------------------------------------------------------------//	
	void                       LockSocketList();
	void                       UnlockSocketList();
	//---------------------------------------------------------------------------------//
	bool                       ClearSocketPtrList();
	//---------------------------------------------------------------------------------//		
	virtual bool               CloseThread_Connection();
	//---------------------------------------------------------------------------------//
	CJetSocket*                AcceptSocket();  
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	CJetSocketServer();
	~CJetSocketServer();
	CJetSocketServer(int port, int connections, TypeSocket type=BlockingSocket);
	//---------------------------------------------------------------------------------//
	virtual bool               CloseServer();
	virtual bool               CreateServer(int port, int connections, TypeSocket type=BlockingSocket);	
	//---------------------------------------------------------------------------------//
	bool                       GetThreadStop_Conn() const;
	void                       SetThreadStop_Conn(bool val);		
	//---------------------------------------------------------------------------------//	
	bool                       GetThreadExit_Conn() const;
	void                       SetThreadExit_Conn(bool val);	
	//---------------------------------------------------------------------------------//	
	virtual bool               ExecThread_Recv();
	virtual bool               ExecThread_Send();
	//---------------------------------------------------------------------------------//	
	virtual bool               CreateThread_Connection();
	virtual bool               ExecThread_Connection();
	//---------------------------------------------------------------------------------//	
	virtual bool               BroadcastOut(const std::string& s);	
	//---------------------------------------------------------------------------------//
};
//-------------------------------------------------------------------------------------//
//use select func to recv client message
class CJetSocketServerSelect : public CJetSocketServer 
{	
private:
	//---------------------------------------------------------------------------------//
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//	
	std::vector<SOCKET>        m_ClientSocketList;
	//---------------------------------------------------------------------------------//	
	size_t                     GetClientSocketCount();
	bool                       ClearClientSocketList();
	bool                       AddClientSocket(SOCKET s);	
	bool                       RemoveClientSocket(size_t index);
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//	
	CJetSocketServerSelect();
	~CJetSocketServerSelect();
	CJetSocketServerSelect(int port, int connections, TypeSocket type=BlockingSocket);	
	//---------------------------------------------------------------------------------//	
	virtual bool               CloseServer();
	virtual bool               CreateServer(int port, int connections, TypeSocket type=BlockingSocket);	
	//---------------------------------------------------------------------------------//
	virtual bool               ExecThread_Recv();		
	virtual bool               ExecThread_Connection();//執行連線執行緒
	//---------------------------------------------------------------------------------//
	virtual bool               BroadcastOut(const std::string& s);	
	//---------------------------------------------------------------------------------//    
}; 
//-------------------------------------------------------------------------------------//
//use select-event fuct to recv client message event
class CJetSocketServerEvent : public CJetSocketServer 
{
private:
	//---------------------------------------------------------------------------------//
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//		
	std::vector<WSAEVENT>      m_ClientEventList;//客戶連線事件
	std::vector<SOCKET>        m_ClientSocketList;
	//---------------------------------------------------------------------------------//		
	size_t                     GetClientEventCount();
	size_t                     GetClientSocketCount();
	bool                       ClearClientSocketList();
	bool                       AddClientSocket(SOCKET s);
	bool                       RemoveClientSocket(size_t index);	
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	CJetSocketServerEvent();
	~CJetSocketServerEvent();
	CJetSocketServerEvent(int port, int connections, TypeSocket type=BlockingSocket);
	//---------------------------------------------------------------------------------//
	virtual bool               CloseServer();
	virtual bool               CreateServer(int port, int connections, TypeSocket type=BlockingSocket);	
	//---------------------------------------------------------------------------------//
	virtual bool               ExecThread_Recv();
	virtual bool               ExecThread_Connection();//執行連線執行緒
	//---------------------------------------------------------------------------------//
	virtual bool               BroadcastOut(const std::string& s);	
	//---------------------------------------------------------------------------------//
};
//-------------------------------------------------------------------------------------//
extern CJetSocketClient        JetSocketClientTest;//測試用的WinSocket
//-------------------------------------------------------------------------------------//
#endif//JET_SOCKET_H
