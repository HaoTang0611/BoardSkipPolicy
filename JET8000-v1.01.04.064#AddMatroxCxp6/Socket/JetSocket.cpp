/* 
   Socket.cpp

   Copyright (C) 2002-2004 René Nyffenegger

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
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "JetSocket.h"
#include <iostream>
#include <ws2tcpip.h>
//-------------------------------------------------------------------------------------//
#include "MES\\ITSLinker.h"
//-------------------------------------------------------------------------------------//
// link with ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")
//-------------------------------------------------------------------------------------//
#define MSGSIZE               1024  
#define MAX_SOCKET_CLIENT_COUNT 64
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
using namespace std;
int CJetSocket::nofSockets_= 0;
unsigned int CJetSocket::s_MsgPPID = 0;
//-------------------------------------------------------------------------------------//
unsigned int __stdcall thSocketRecvFn(void *pParam);//接收訊息執行緒函式
unsigned int __stdcall thSocketSendFn(void *pParam);//傳送訊息執行緒函式
unsigned int __stdcall thSocketConnectionFn(void *pParam);//伺服等待客戶連線執行緒
//-------------------------------------------------------------------------------------//
unsigned int __stdcall thSocketRecvFn(void *pParam)//接收訊息執行緒函式
{
	CJetSocket *SockerPtr = (CJetSocket*)(pParam);
	if ( NULL == SockerPtr ) { return -1; }
	
	DWORD  dwTime = 10;
	bool    bThreadStop = false;	
	bool    bThreadExit = false;
	while (TRUE)
	{			
		bThreadStop = SockerPtr->GetThreadStop_Recv();
		bThreadExit = SockerPtr->GetThreadExit_Recv();
		dwTime = SockerPtr->GetThreadSleepTime();
		if ( true == bThreadExit ) 
		{	break; }
		
		if ( true == bThreadStop ) 
		{
			if ( dwTime > 0 ) //避免空跑滿載
			{	::Sleep(dwTime); }		
			continue;
		}
		
		if ( SockerPtr->ExecThread_Recv() == false )
		{	break; }
		if ( dwTime > 0 ) //避免空跑滿載
		{	::Sleep(dwTime); }		
	}	
	return 0;	
}
//-------------------------------------------------------------------------------------//
unsigned int __stdcall thSocketSendFn(void *pParam)//傳送訊息執行緒函式
{
	CJetSocket *SockerPtr = (CJetSocket*)(pParam);
	if ( NULL == SockerPtr ) { return -1; }
	
	DWORD  dwTime = 10;
	bool    bThreadStop = false;	
	bool    bThreadExit = false;
	while (TRUE)
	{			
		bThreadStop = SockerPtr->GetThreadStop_Send();
		bThreadExit = SockerPtr->GetThreadExit_Send();
		dwTime = SockerPtr->GetThreadSleepTime();

		if ( true == bThreadExit ) 
		{	break; }

		if ( true == bThreadStop ) 
		{
			if ( dwTime > 0 ) //避免空跑滿載
			{	::Sleep(dwTime); }		
			continue;
		}

		if ( SockerPtr->ExecThread_Send() == false )
		{	break; }		

		if ( dwTime > 0 ) //避免空跑滿載
		{	::Sleep(dwTime); }		
	}	
	return 0;	
}
//-------------------------------------------------------------------------------------//
unsigned int __stdcall thSocketConnectionFn(void *pParam)//伺服等待客戶連線執行緒
{
	CJetSocketServer *ServerPtr = (CJetSocketServer*)(pParam);
	if ( NULL == ServerPtr ) { return -1; }

	DWORD   dwTime = 10;
	bool    bThreadStop = false;	
	bool    bThreadExit = false;
	while ( true )
	{
		bThreadStop = ServerPtr->GetThreadStop_Conn();
		bThreadExit = ServerPtr->GetThreadExit_Conn();
		dwTime = ServerPtr->GetThreadSleepTime();
		if ( true == bThreadExit ) 
		{	break; }
		
		if ( true == bThreadStop ) 
		{
			if ( dwTime > 0 ) //避免空跑滿載
			{	::Sleep(dwTime); }		
			continue;
		}

		if ( ServerPtr->ExecThread_Connection() == false )
		{	break; }

		if ( dwTime > 0 ) //避免空跑滿載
		{	::SleepEx(dwTime, NULL); }
	}
	return 0;
}
//-------------------------------------------------------------------------------------//
void CJetSocket::Start() 
{
	if (!nofSockets_) 
	{
		WSADATA info;
		if (WSAStartup(MAKEWORD(2,2), &info)) 
		{	throw "Could not start WSA";	}
	}
	++nofSockets_;
}
//-------------------------------------------------------------------------------------//
void CJetSocket::End() 
{
	WSACleanup();
}
//-------------------------------------------------------------------------------------//
CJetSocket::CJetSocket():m_Socket(INVALID_SOCKET)
{
	PreInit();
	Start();
	// UDP: use SOCK_DGRAM instead of SOCK_STREAM
	m_Socket = socket(AF_INET,SOCK_STREAM,0);

	if ( INVALID_SOCKET == m_Socket) 
	{	m_SrrorStr = "Error, INVALID_SOCKET";		}

	refCounter_ = new int(1);
}
//-------------------------------------------------------------------------------------//
CJetSocket::CJetSocket(SOCKET s):m_Socket(s)
{
	PreInit();
	Start();
	refCounter_ = new int(1);	
};
//-------------------------------------------------------------------------------------//
CJetSocket::~CJetSocket() 
{
	if (! --(*refCounter_)) 
	{
		CloseSocket();
		delete refCounter_;
	}
	--nofSockets_;
	if (!nofSockets_)
	{	End(); }

	::DeleteCriticalSection(&m_csRecvThread);
	::DeleteCriticalSection(&m_csRecvMsgList);	
	::DeleteCriticalSection(&m_csSendMsgList);		
}
//-------------------------------------------------------------------------------------//
CJetSocket::CJetSocket(const CJetSocket& o) 
{	
	refCounter_=o.refCounter_;
	(*refCounter_)++;

	Clone(o);

	nofSockets_++;
}
//-------------------------------------------------------------------------------------//
CJetSocket& CJetSocket::operator=(CJetSocket& o) 
{
  (*o.refCounter_)++;
  refCounter_=o.refCounter_;

  Clone(o);

  nofSockets_++;
  return *this;
}
//-------------------------------------------------------------------------------------//
void CJetSocket::PreInit()
{	
	m_ITSLinkerPtr = NULL;
	m_hWnd = NULL;
	m_uMsg = 0;
	m_wRecv = 0;
	m_wSend = 0;
	m_bclosed = true;
	m_bThreadStop_Recv = true;
	m_bThreadStop_Send = true;
	m_bThreadExit_Recv = true;
	m_bThreadExit_Send = true;		
	m_dwThreadSleep = 10;
	m_bUseRecvMsgList = false;

	m_thRecvID = -1;
	m_thRecvHandle = NULL;
	m_thSendID = -1;
	m_thSendHandle = NULL;
	::InitializeCriticalSection(&m_csRecvThread);//同步化		
	::InitializeCriticalSection(&m_csRecvMsgList);	
	::InitializeCriticalSection(&m_csSendMsgList);
}
//-------------------------------------------------------------------------------------//
void CJetSocket::Clone(const CJetSocket& o)
{
	m_SrrorStr = o.m_SrrorStr;  
	m_Socket   =o.m_Socket;
	m_hWnd     =o.m_hWnd;
	m_uMsg     =o.m_uMsg;
	m_wRecv    =o.m_wRecv;  
	m_wSend    =o.m_wSend;  
	m_bclosed  =o.m_bclosed;
	m_dwThreadSleep = o.m_dwThreadSleep;	
	m_bThreadExit_Recv = o.m_bThreadExit_Recv;
	m_bThreadExit_Send = o.m_bThreadExit_Send;	
	
	m_RecvBuf = o.m_RecvBuf;
	m_SendBuf = o.m_SendBuf;
	m_WSAErrorStr = o.m_WSAErrorStr;
	m_RecvMsgList = o.m_RecvMsgList;	
	m_SendMsgList = o.m_SendMsgList;	
	m_bUseRecvMsgList = o.m_bUseRecvMsgList;	
}
//-------------------------------------------------------------------------------------//
bool CJetSocket::CheckSocket()
{
	if ( INVALID_SOCKET == m_Socket )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
std::string& CJetSocket::GetWSAErrorString(int ErrorID)
{
	std::string str;
	m_WSAErrorStr = "";
	switch ( ErrorID )
	{
	case WSAEINTR: str = "WSAEINTR";	break;
	case WSAEBADF: str = "WSAEBADF";	break;
	case WSAEACCES: str = "WSAEACCES";	break;
	case WSAEFAULT: str = "WSAEFAULT";	break;
	case WSAEINVAL: str = "WSAEINVAL";	break;
	case WSAEMFILE: str = "WSAEMFILE";	break;

	case WSAEWOULDBLOCK: str = "WSAEWOULDBLOCK";	break;
	case WSAEINPROGRESS: str = "WSAEINPROGRESS";	break;
	case WSAEALREADY: str = "WSAEALREADY";	break;
	case WSAENOTSOCK: str = "WSAENOTSOCK";	break;
	case WSAEDESTADDRREQ: str = "WSAEDESTADDRREQ";	break;
	case WSAEMSGSIZE: str = "WSAEMSGSIZE";	break;
	case WSAEPROTOTYPE: str = "WSAEPROTOTYPE";	break;
	case WSAENOPROTOOPT: str = "WSAENOPROTOOPT";	break;
	case WSAEPROTONOSUPPORT: str = "WSAEPROTONOSUPPORT";	break;
	case WSAESOCKTNOSUPPORT: str = "WSAESOCKTNOSUPPORT";	break;
	case WSAEOPNOTSUPP: str = "WSAEOPNOTSUPP";	break;
	case WSAEPFNOSUPPORT: str = "WSAEPFNOSUPPORT";	break;
	case WSAEAFNOSUPPORT: str = "WSAEAFNOSUPPORT";	break;
	case WSAEADDRINUSE: str = "WSAEADDRINUSE";	break;
	case WSAEADDRNOTAVAIL: str = "WSAEADDRNOTAVAIL";	break;
	case WSAENETDOWN: str = "WSAENETDOWN";	break;
	case WSAENETUNREACH: str = "WSAENETUNREACH";	break;
	case WSAENETRESET: str = "WSAENETRESET";	break;
	case WSAECONNABORTED: str = "WSAECONNABORTED";	break;
	case WSAECONNRESET: str = "WSAECONNRESET";	break;
	case WSAENOBUFS: str = "WSAENOBUFS";	break;
	case WSAEISCONN: str = "WSAEISCONN";	break;
	case WSAENOTCONN: str = "WSAENOTCONN";	break;
	case WSAESHUTDOWN: str = "WSAESHUTDOWN";	break;
	case WSAETOOMANYREFS: str = "WSAETOOMANYREFS";	break;
	case WSAETIMEDOUT: str = "WSAETIMEDOUT";	break;
	case WSAECONNREFUSED: str = "WSAECONNREFUSED";	break;
	case WSAELOOP: str = "WSAELOOP";	break;
	case WSAENAMETOOLONG: str = "WSAENAMETOOLONG";	break;
	case WSAEHOSTDOWN: str = "WSAEHOSTDOWN";	break;
	case WSAEHOSTUNREACH: str = "WSAEHOSTUNREACH";	break;
	case WSAENOTEMPTY: str = "WSAENOTEMPTY";	break;
	case WSAEPROCLIM: str = "WSAEPROCLIM";	break;
	case WSAEUSERS: str = "WSAEUSERS";	break;	
	case WSAEDQUOT: str = "WSAEDQUOT";	break;
	case WSAESTALE: str = "WSAESTALE";	break;
	case WSAEREMOTE: str = "WSAEREMOTE";	break;

	case WSASYSNOTREADY: str = "WSASYSNOTREADY";	break;
	case WSAVERNOTSUPPORTED: str = "WSAVERNOTSUPPORTED";	break;
	case WSANOTINITIALISED: str = "WSANOTINITIALISED";	break;
	case WSAEDISCON: str = "WSAEDISCON";	break;
	case WSAENOMORE: str = "WSAENOMORE";	break;
	case WSAECANCELLED: str = "WSAECANCELLED";	break;
	case WSAEINVALIDPROCTABLE: str = "WSAEINVALIDPROCTABLE";	break;
	case WSAEINVALIDPROVIDER: str = "WSAEINVALIDPROVIDER";	break;
	case WSAEPROVIDERFAILEDINIT: str = "WSAEPROVIDERFAILEDINIT";	break;
	case WSASYSCALLFAILURE: str = "WSASYSCALLFAILURE";	break;
	case WSASERVICE_NOT_FOUND: str = "WSASERVICE_NOT_FOUND";	break;
	case WSATYPE_NOT_FOUND: str = "WSATYPE_NOT_FOUND";	break;
	case WSA_E_NO_MORE: str = "WSA_E_NO_MORE";	break;
	case WSA_E_CANCELLED: str = "WSA_E_CANCELLED";	break;
	case WSAEREFUSED: str = "WSAEREFUSED";	break;
	
	//case AAAAAAAAAAAAAAAAAA: str = L"AAAAAAAAAAAAAAAAAA";	break;
	default:
		{			
			char  buffer[32]="";
			int ID2 = ErrorID-WSABASEERR;
			::sprintf_s(buffer, 32, "Undefine[%d]", ErrorID);	
			str = buffer;
		}
		break;
	}	
	m_WSAErrorStr = std::string("Error,")+str;
	return m_WSAErrorStr;
}
//-------------------------------------------------------------------------------------//
bool CJetSocket::CloseThread_Recv()//關閉接受訊息的執行續
{	
	if ( NULL == m_thRecvHandle )
	{	return true; }
	SetThreadExit_Recv(true);
	DWORD ret = WaitForSingleObject(m_thRecvHandle, 1000);
	::CloseHandle(m_thRecvHandle);
	m_thRecvID = -1;
	m_thRecvHandle = NULL;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetSocket::CloseThread_Send()//關閉傳送訊息的執行續
{
	if ( NULL == m_thSendHandle )
	{	return true; }
	SetThreadExit_Send(true);
	DWORD ret = WaitForSingleObject(m_thSendHandle, 1000);
	::CloseHandle(m_thSendHandle);
	m_thSendID = -1;
	m_thSendHandle = NULL;	
	return true;
}
//-------------------------------------------------------------------------------------//
void CJetSocket::CloseSocket() 
{
	m_bclosed = true;
	if ( INVALID_SOCKET != m_Socket )
	{
		closesocket(m_Socket);		
		m_Socket = INVALID_SOCKET;
	}
	CloseThread_Recv();
	CloseThread_Send();
}
//-------------------------------------------------------------------------------------//
bool CJetSocket::GetClosed()
{
	return m_bclosed;
}
//-------------------------------------------------------------------------------------//
void CJetSocket::SetClosed(bool val)
{
	m_bclosed = val;
}
//-------------------------------------------------------------------------------------//
std::string& CJetSocket::GetErrorString()
{	
	return m_SrrorStr;
}
//-------------------------------------------------------------------------------------//
CITSLinker* CJetSocket::GetITSLinkerPtr()
{
	return m_ITSLinkerPtr;
}
//-------------------------------------------------------------------------------------//
void CJetSocket::SetITSLinkerPtr(CITSLinker *Ptr)
{
	m_ITSLinkerPtr = Ptr;
}
//-------------------------------------------------------------------------------------//
HWND CJetSocket::GetHWnd()
{
	return m_hWnd;
}
//-------------------------------------------------------------------------------------//
void CJetSocket::ShowRecv(const char *Str)
{
	if ( NULL == m_hWnd ) { return; }
	CITSLinker *LinkPtr=GetITSLinkerPtr();
	if ( NULL != LinkPtr )
	{
		LinkPtr->SetNewBufferRecv(Str);
		::PostMessage(m_hWnd, m_uMsg, m_wRecv, NULL);
	}
	else
	{	::SendMessage(m_hWnd, m_uMsg, m_wRecv, (LPARAM)(Str)); }
}
//-------------------------------------------------------------------------------------//
void CJetSocket::ShowSend(const char *Str)
{
	if ( NULL == m_hWnd ) { return; }
	CITSLinker *LinkPtr=GetITSLinkerPtr();
	if ( NULL != LinkPtr )
	{
		LinkPtr->SetNewBufferSend(Str);
		::PostMessage(m_hWnd, m_uMsg, m_wSend, NULL);
	}
	else
	{	::SendMessage(m_hWnd, m_uMsg, m_wSend, (LPARAM)(Str)); }
}
//-------------------------------------------------------------------------------------//
void CJetSocket::SetHWnd(HWND h, UINT m, WPARAM wRecv, WPARAM wSend)
{
	m_hWnd = h;
	m_uMsg = m;
	m_wRecv = wRecv;
	m_wSend = wSend;
}
//-------------------------------------------------------------------------------------//
int CJetSocket::ReceiveBytes(std::string &str)
{
	if ( CheckSocket() == false ) { return 0; }	
	
	int         nret=0;
	size_t      szTotalRecv=0;	
	std::string ret;
	const size_t len=1024;
	int          nNoReadCnt=0;	
	char buf[len+1]={NULL};
	
	szTotalRecv = 0;
	while (true) 
	{
		u_long arg = 0;
		nret = ioctlsocket(m_Socket, FIONREAD, &arg);		
		if ( SOCKET_ERROR == nret)
		{	return 0; }
		
		if (arg == 0)
		{	
			break;//added
			//if ( ret.length() > 0 )
			//{	break; }
			if ( szTotalRecv > 0 ) 
			{	break; }
		}
		
		if ( arg > len ) 
		{	arg = len; }
		
		int rv = recv (m_Socket, buf, arg, 0);		
		if (rv <= 0)
		{
			break;//added
			nNoReadCnt ++;
			if ( nNoReadCnt > 5 )
			{	return 0;	}
			continue;
		}

		std::string t;
		t.assign (buf, rv);
		ret += t;

		szTotalRecv += rv;		
		
		nNoReadCnt = 0;

	} 
	str = ret;
	return 1;
}
//-------------------------------------------------------------------------------------//
int CJetSocket::ReceiveLine(std::string &str)
{
	if ( CheckSocket() == false ) { return 0; }
	std::string ret;
	while (true) 
	{
		int  v;
		char r;
		v = recv(m_Socket, &r, 1, 0);		
		switch ( v ) 
		{
		case 0: // not connected anymore;
              // ... but last line sent
              // might not end in \n,
              // so return ret anyway.
			return 0;
		case -1:
			return 0;
//			if (errno == EAGAIN) {
//			return ret;
//			} else {
//			// not connected anymore
//			return "";
//			}
		}

		ret += r;
		str = ret;
		if (r == '\n')
		{	return 1; }
	}
}
//-------------------------------------------------------------------------------------//
bool CJetSocket::SendLine(std::string s) 
{	
	s += '\n';
	return SendBytes(s);
}
//-------------------------------------------------------------------------------------//
bool CJetSocket::SendBytes(const std::string& s) 
{
	if ( CheckSocket() == false ) { return false; }

	int ret = send(m_Socket,s.c_str(),s.length(),0);
	if ( SOCKET_ERROR  == ret )
	{
		int ErrorID = WSAGetLastError();
		m_SrrorStr = GetWSAErrorString(ErrorID);
		return false;
	}
	
	return true;
}
//-------------------------------------------------------------------------------------//
int CJetSocket::GetAddress(std::string &rAddr)
{
	sockaddr_in SockAddr;
	int addrlen = sizeof(SockAddr);

	if (getsockname(m_Socket, (LPSOCKADDR)&SockAddr, &addrlen) == SOCKET_ERROR)
	{	return WSAGetLastError();	}

	char myIP[16];
	inet_ntop(AF_INET, &SockAddr.sin_addr, myIP, sizeof(myIP));
	rAddr = myIP;
	return 0;
}
//-------------------------------------------------------------------------------------//
bool CJetSocket::GetThreadStop_Recv() const
{
	return m_bThreadStop_Recv;
}
//-------------------------------------------------------------------------------------//
void CJetSocket::SetThreadStop_Recv(bool val)
{
	m_bThreadStop_Recv = val;
}
//-------------------------------------------------------------------------------------//
bool CJetSocket::GetThreadStop_Send() const
{
	return m_bThreadStop_Send;
}
//-------------------------------------------------------------------------------------//
void CJetSocket::SetThreadStop_Send(bool val)
{
	m_bThreadStop_Send = val;
}
//-------------------------------------------------------------------------------------//
bool CJetSocket::GetThreadExit_Recv() const
{
	return m_bThreadExit_Recv;
}
//-------------------------------------------------------------------------------------//
void CJetSocket::SetThreadExit_Recv(bool val)
{
	m_bThreadExit_Recv = val;
}
//-------------------------------------------------------------------------------------//
bool CJetSocket::GetThreadExit_Send() const
{
	return m_bThreadExit_Send;
}
//-------------------------------------------------------------------------------------//
void CJetSocket::SetThreadExit_Send(bool val)
{
	m_bThreadExit_Send = val;
}
//-------------------------------------------------------------------------------------//
DWORD CJetSocket::GetThreadSleepTime() const
{
	return m_dwThreadSleep;
}
//-------------------------------------------------------------------------------------//
bool CJetSocket::GetUseRecvMsgList() const
{
	return m_bUseRecvMsgList;
}
//-------------------------------------------------------------------------------------//
void CJetSocket::SetUseRecvMsgList(bool val)
{
	m_bUseRecvMsgList = val;
}
//-------------------------------------------------------------------------------------//
inline void CJetSocket::LockRecvThread()
{
	::EnterCriticalSection(&m_csRecvThread);	
}
//-------------------------------------------------------------------------------------//
inline void CJetSocket::UnlockRecvThread()
{
	::LeaveCriticalSection(&m_csRecvThread);	
}
//-------------------------------------------------------------------------------------//
inline void CJetSocket::LockRecvMsgList()
{
	::EnterCriticalSection(&m_csRecvMsgList);	
}
//-------------------------------------------------------------------------------------//
inline void CJetSocket::UnlockRecvMsgList()
{
	::LeaveCriticalSection(&m_csRecvMsgList);	
}
//-------------------------------------------------------------------------------------//
inline void CJetSocket::LockSendMsgList()
{
	::EnterCriticalSection(&m_csSendMsgList);	
}
//-------------------------------------------------------------------------------------//
inline void CJetSocket::UnlockSendMsgList()
{
	::LeaveCriticalSection(&m_csSendMsgList);	
}
//-------------------------------------------------------------------------------------//
bool CJetSocket::AddRecvMsg(const std::string &msg)
{
	LockRecvMsgList();	
	m_RecvMsgList.push_back(msg);
	UnlockRecvMsgList();
	return true;
}
//-------------------------------------------------------------------------------------//
size_t CJetSocket::GetRecvMsgCount() const
{
	return m_RecvMsgList.size();	
}
//-------------------------------------------------------------------------------------//
bool CJetSocket::CloneRecvMsgList(std::vector<std::string> &sList, bool bClear)//取得接收訊息列表
{
	const size_t Count = m_RecvMsgList.size();
	if ( 0 == Count ) { return true; }

	LockRecvMsgList();
	sList = m_RecvMsgList;
	if ( true == bClear )
	{	m_RecvMsgList.clear(); }
	UnlockRecvMsgList();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetSocket::AddSendMsg(const std::string &msg)
{
	LockSendMsgList();	
	m_SendMsgList.push_back(msg);
	UnlockSendMsgList();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetSocket::RemoveSendMsg(size_t index)
{
	const size_t Count = GetSendMsgCount();
	if ( index >= Count ) { return false; }
	LockSendMsgList();	
	m_SendMsgList.erase(m_SendMsgList.begin()+index);
	UnlockSendMsgList();
	return true;
}
//-------------------------------------------------------------------------------------//
size_t CJetSocket::GetSendMsgCount() const
{
	return m_SendMsgList.size();
}
//-------------------------------------------------------------------------------------//
bool CJetSocket::ExecThread_Recv()
{
	if ( CheckSocket() == false ) { return false; }	

	std::string r;
	int ret = ReceiveBytes(r);
	if ( 0 == ret ) 
	{	return false; }
	const size_t len = r.length();
	if ( 0 == len ) 
	{	return true; }

	//SendBytes(r);	
	if ( GetUseRecvMsgList() == true )
	{	AddRecvMsg(r);	}

	m_RecvBuf = r;
	ShowRecv(m_RecvBuf.c_str());
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetSocket::CreateThread_Recv()
{
	CloseThread_Recv();
	SetThreadExit_Recv(false);
	m_thRecvHandle = (HANDLE)::_beginthreadex(NULL, NULL, &thSocketRecvFn, (LPVOID)this, NULL, &m_thRecvID);		
	//AOIExceptionCodeCtrl.SetAOIExceptionCode_Thread(AOI_EXCEPTION_THREAD_CREATE, m_ErrorString);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetSocket::ExecThread_Send()
{
	if ( CheckSocket() == false ) { return false; }	

	const size_t Count = GetSendMsgCount();
	if ( 0 == Count ) { return true; }

	const size_t index = 0;
	m_SendBuf = m_SendMsgList[index];	
	if ( SendBytes(m_SendBuf) == false ) 
	{	return false; }
	RemoveSendMsg(index);
	ShowSend(m_SendBuf.c_str());	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetSocket::CreateThread_Send()
{
	CloseThread_Send();
	SetThreadExit_Send(false);
	m_thSendHandle = (HANDLE)::_beginthreadex(NULL, NULL, &thSocketSendFn, (LPVOID)this, NULL, &m_thSendID);			
	//AOIExceptionCodeCtrl.SetAOIExceptionCode_Thread(AOI_EXCEPTION_THREAD_CREATE, m_ErrorString);
	return true;
}
//-------------------------------------------------------------------------------------//
CJetSocketClient::CJetSocketClient():CJetSocket() 
{
	m_bConnected = false;
}
//-------------------------------------------------------------------------------------//
CJetSocketClient::CJetSocketClient(const std::string& host, int port):CJetSocket() 
{
	m_bConnected = false;
	Connect(host, port); 
}
//-------------------------------------------------------------------------------------//
CJetSocketClient::~CJetSocketClient()
{
}
//-------------------------------------------------------------------------------------//
void CJetSocketClient::SetConnected(bool val)
{
	m_bConnected = val;
}
//-------------------------------------------------------------------------------------//
bool CJetSocketClient::GetConnected() const
{
	return m_bConnected;
}
//-------------------------------------------------------------------------------------//
bool CJetSocketClient::CloseClient()
{
	SetConnected(false);
	SetThreadStop_Recv(true);
	SetThreadStop_Send(true);
	CloseSocket();	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetSocketClient::Connect(const std::string& host, int port)
{
	const bool bThread=true;
	return Connect(host, port, bThread);
}
//-------------------------------------------------------------------------------------//
bool CJetSocketClient::Connect(const std::string& host, int port, bool bThread)
{
	CloseClient();
	m_Socket = socket(AF_INET,SOCK_STREAM,0);
	if ( INVALID_SOCKET == m_Socket) 
	{
		int ErrorID = WSAGetLastError();
		m_SrrorStr = GetWSAErrorString(ErrorID);
		return false;
	}
	
	std::string error;
  /*
	hostent *he;
	if ((he = gethostbyname(host.c_str())) == 0) 
	{
		error = strerror(errno);
		throw error;
		return false;
	}
  
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr = *((in_addr *)he->h_addr);
	memset(&(addr.sin_zero), 0, 8);
  */
	//gethostbyname by getaddrinfo replacement
	ADDRINFO hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_flags = AI_ALL;
	hints.ai_family = PF_INET;
	hints.ai_protocol = IPPROTO_IPV4;
	ADDRINFO* pResult = NULL;

	int errcode = getaddrinfo((LPCSTR)host.c_str(), NULL, &hints, &pResult);
	if (errcode != 0)
	{
		const size_t errmsglen = 256;
		char errmsg[errmsglen];
		error = strerror_s(errmsg, errmsglen, errno);	  
		//throw error;		
		m_SrrorStr = std::string("Error, getaddrinfo Fault ")+ std::string(errmsg);
		return false;
	}

	sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = *((ULONG*)&(((sockaddr_in*)pResult->ai_addr)->sin_addr));
	addr.sin_port = htons(port);
	//
	if (::connect(m_Socket, (sockaddr *) &addr, sizeof(sockaddr))) 
	{
		//error = strerror(WSAGetLastError());
		const size_t errmsglen = 256;
		char errmsg[errmsglen];
		error = strerror_s(errmsg, errmsglen, WSAGetLastError());
		//throw error;
		m_SrrorStr = std::string("Error, Connec Fault ")+ std::string(errmsg);
		return false;
	}

	SetConnected(true);
	if ( true == bThread )
	{
		SetThreadStop_Recv(false);
		SetThreadStop_Send(false);
		CreateThread_Recv();
		CreateThread_Send();
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetSocketClient::ExecThread_Recv()
{
	return CJetSocket::ExecThread_Recv();
	return true;
}
//-------------------------------------------------------------------------------------//
CJetSocketServer::CJetSocketServer()
{	
	m_bThreadStop_Conn = true;	
	m_bThreadExit_Conn = true;
	m_thConnectionID = -1;
	m_thConnectionHandle=NULL;	
	::InitializeCriticalSection(&m_csSocketList);
}
//-------------------------------------------------------------------------------------//
CJetSocketServer::CJetSocketServer(int port, int connections, TypeSocket type) 
{
	m_bThreadStop_Conn = true;	
	m_bThreadExit_Conn = true;
	m_thConnectionID = -1;
	m_thConnectionHandle=NULL;	
	::InitializeCriticalSection(&m_csSocketList);
	CreateServer(port, connections, type);
}
//-------------------------------------------------------------------------------------//
CJetSocketServer::~CJetSocketServer()
{		
	CloseThread_Connection();	
	ClearSocketPtrList();
	::DeleteCriticalSection(&m_csSocketList);
}
//-------------------------------------------------------------------------------------//
void CJetSocketServer::LockSocketList()
{
	::EnterCriticalSection(&m_csSocketList);
}
//-------------------------------------------------------------------------------------//
void CJetSocketServer::UnlockSocketList()
{
	::LeaveCriticalSection(&m_csSocketList);
}
//-------------------------------------------------------------------------------------//
bool CJetSocketServer::GetThreadStop_Conn() const
{
	return m_bThreadStop_Conn;
}
//-------------------------------------------------------------------------------------//
void CJetSocketServer::SetThreadStop_Conn(bool val)
{
	m_bThreadStop_Conn = val;
}
//-------------------------------------------------------------------------------------//
bool CJetSocketServer::GetThreadExit_Conn() const
{
	return m_bThreadExit_Conn;
}
//-------------------------------------------------------------------------------------//
void CJetSocketServer::SetThreadExit_Conn(bool val)
{
	m_bThreadExit_Conn = val;
}
//-------------------------------------------------------------------------------------//
bool CJetSocketServer::CloseThread_Connection()
{	
	if ( NULL == m_thConnectionHandle ) 
	{	return true; }	
	SetThreadExit_Conn(true);
	DWORD ret = ::WaitForSingleObject(m_thConnectionHandle, 1000);
	::CloseHandle(m_thConnectionHandle);
	m_thConnectionID = -1;
	m_thConnectionHandle=NULL;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetSocketServer::CloseServer()
{
	CloseSocket();
	CloseThread_Connection();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetSocketServer::CreateServer(int port, int connections, TypeSocket type)
{
	CloseServer();

	sockaddr_in sa;
	memset(&sa, 0, sizeof(sa));

	sa.sin_family = PF_INET;             
	sa.sin_port = htons(port);          
	m_Socket = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == m_Socket) 
	{	
		//throw "INVALID_SOCKET";	
		m_SrrorStr = "Error, INVALID_SOCKET";	
		return false;
	}

	if(type==NonBlockingSocket) 
	{
		u_long arg = 1;
		ioctlsocket(m_Socket, FIONBIO, &arg);
	}

	/* bind the socket to the internet address */
	if (::bind(m_Socket, (sockaddr *)&sa, sizeof(sockaddr_in)) == SOCKET_ERROR) 
	{
		closesocket(m_Socket);
		//throw "INVALID_SOCKET";
		m_SrrorStr = "Error, Bind Fault";
		return false;
	}  
	listen(m_Socket, connections);
	m_bclosed=false;
	SetThreadStop_Conn(false);
	CreateThread_Connection();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetSocketServer::ExecThread_Recv()
{
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetSocketServer::ExecThread_Send()
{
	const size_t Count = GetSendMsgCount();
	if ( 0 == Count ) { return true; }
	const size_t Index = 0;
	m_SendBuf = m_SendMsgList[Index];
	if ( BroadcastOut(m_SendBuf) == false ) 
	{	return false; }
	ShowSend(m_SendBuf.c_str());
	RemoveSendMsg(Index);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetSocketServer::CreateThread_Connection()
{
	CloseThread_Connection();
	SetThreadExit_Conn(false);
	m_thConnectionHandle = (HANDLE)::_beginthreadex(NULL, NULL, &thSocketConnectionFn, (LPVOID)this, NULL, &m_thConnectionID);
	//AOIExceptionCodeCtrl.SetAOIExceptionCode_Thread(AOI_EXCEPTION_THREAD_CREATE, m_ErrorString);
	return true;
}
//-------------------------------------------------------------------------------------//
CJetSocket* CJetSocketServer::AcceptSocket() 
{
	SOCKET new_sock = accept(m_Socket, 0, 0);

	if (INVALID_SOCKET == new_sock)
	{
		if ( true == m_bclosed ) 
		{	return NULL; }

		int rc = WSAGetLastError();
		if(WSAEWOULDBLOCK==rc) 
		{
			return NULL; // non-blocking call, no request pending
		}
		else 
		{
			//throw std::exception("Invalid Socket");    
			m_SrrorStr = "Error, Accept Fault";
			return NULL;
		}
	}	
	CJetSocket* r = new CJetSocket(new_sock);
	return r;
}
//-------------------------------------------------------------------------------------//
bool CJetSocketServer::ExecThread_Connection()
{	
	CJetSocket *SocketPtr = AcceptSocket();
	if ( NULL == SocketPtr ) 
	{	return false; }
	
	LockSocketList();
	//SocketPtr->SendBytes("Welcome to the Message Distributor");
	SocketPtr->SetThreadStop_Recv(false);
	SocketPtr->SetThreadStop_Send(false);	
	SocketPtr->CreateThread_Recv();	
	SocketPtr->CreateThread_Send();
	m_SocketPtrList.push_back(SocketPtr);
	UnlockSocketList();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetSocketServer::BroadcastOut(const std::string& s)
{
	size_t       i=0;
	CJetSocket  *SocketPtr=NULL;
	LockSocketList();
	const size_t Count = m_SocketPtrList.size();
	for ( i=0; i<Count; i++ )
	{
		SocketPtr = m_SocketPtrList[i];
		if ( NULL == SocketPtr ) { continue; }	
		SocketPtr->SendBytes(s);
	}
	m_SocketPtrList.clear();
	UnlockSocketList();	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetSocketServer::ClearSocketPtrList()
{
	size_t       i=0;
	CJetSocket  *SocketPtr=NULL;
	LockSocketList();
	const size_t Count = m_SocketPtrList.size();
	for ( i=0; i<Count; i++ )
	{
		SocketPtr = m_SocketPtrList[i];
		if ( NULL == SocketPtr ) { continue; }	
		SocketPtr->SetThreadExit_Recv(true);
		SocketPtr->SetThreadExit_Send(true);
		SocketPtr->CloseSocket();

		delete m_SocketPtrList[i]; 
		m_SocketPtrList[i] = NULL;
	}
	m_SocketPtrList.clear();
	UnlockSocketList();
	return true;
}
//-------------------------------------------------------------------------------------//
CJetSocketServerSelect::CJetSocketServerSelect()
{	
}
//-------------------------------------------------------------------------------------//
CJetSocketServerSelect::CJetSocketServerSelect(int port, int connections, TypeSocket type)
{	
	CreateServer(port, connections, type);
}
//-------------------------------------------------------------------------------------//
CJetSocketServerSelect::~CJetSocketServerSelect()
{	
	CloseServer();
	ClearClientSocketList();	
}
//-------------------------------------------------------------------------------------//
bool CJetSocketServerSelect::CloseServer()
{
	CloseSocket();
	CloseThread_Connection();
	ClearClientSocketList();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetSocketServerSelect::CreateServer(int port, int connections, TypeSocket type)
{
	sockaddr_in sa;
	memset(&sa, 0, sizeof(sa));

	sa.sin_family = PF_INET;             
	sa.sin_port = htons(port);          
	m_Socket = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == m_Socket) 
	{	
		//throw "INVALID_SOCKET";	
		m_SrrorStr = "Error, INVALID_SOCKET";	
		return false;
	}

	if(type==NonBlockingSocket) 
	{
		u_long arg = 1;
		ioctlsocket(m_Socket, FIONBIO, &arg);
	}

	/* bind the socket to the internet address */
	if (::bind(m_Socket, (sockaddr *)&sa, sizeof(sockaddr_in)) == SOCKET_ERROR) 
	{
		closesocket(m_Socket);
		//throw "INVALID_SOCKET";
		m_SrrorStr = "Error, Bind Fault";
		return false;
	}  
	listen(m_Socket, connections);
	m_bclosed=false;
	SetThreadStop_Recv(false);
	SetThreadStop_Send(false);
	SetThreadStop_Conn(false);
	CreateThread_Connection();
	CreateThread_Recv();
	CreateThread_Send();
	return true;
}
//-------------------------------------------------------------------------------------//
size_t  CJetSocketServerSelect::GetClientSocketCount()
{
	return m_ClientSocketList.size();
}
//-------------------------------------------------------------------------------------//
bool CJetSocketServerSelect::ClearClientSocketList()
{
	size_t i=0;	
	LockSocketList();
	const size_t Count = GetClientSocketCount();
	for ( i=0; i<Count; i++ )
	{
		::closesocket(m_ClientSocketList[i]);
		m_ClientSocketList[i] = INVALID_SOCKET;
	}
	m_ClientSocketList.clear();
	UnlockSocketList();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetSocketServerSelect::AddClientSocket(SOCKET s)
{
	const size_t Count = GetClientSocketCount();
	if ( Count >= MAX_SOCKET_CLIENT_COUNT ) 
	{	return false; }
	LockSocketList();
	m_ClientSocketList.push_back(s);
	UnlockSocketList();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetSocketServerSelect::RemoveClientSocket(size_t index)
{
	const size_t Count = GetClientSocketCount();
	if ( index>=Count ) 
	{	return false; }	
	
	closesocket(m_ClientSocketList[index]);	
	m_ClientSocketList.erase(m_ClientSocketList.begin()+index);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetSocketServerSelect::ExecThread_Recv()
{
	int    i=0;
	int    ret=0;
	fd_set fdread;
	TIMEVAL *ptval=NULL;
	struct timeval tv = {1, 0};	
	const int len = MSGSIZE;
	char szbuffer[len]="";
	const int Count = GetClientSocketCount();
	if ( 0 == Count ) { return true; }
	
	TypeSocket type=NonBlockingSocket;
	if(type==NonBlockingSocket) 
	{	ptval = &tv;	}
	else 
	{	ptval = 0;	}

	//FD_CLR(s, *set)//Removes the descriptor s from set. 
	//FD_ISSET(s, *set) //Nonzero if s is a member of the set. Otherwise, zero. 
	//FD_SET(s, *set) //Adds descriptor s to set. 
	//FD_ZERO(*set) //Initializes the set to the NULL set.

	FD_ZERO(&fdread);
	for ( i=0; i<Count; i++)
	{	FD_SET(m_ClientSocketList[i], &fdread);	}
		
	ret = select(0, &fdread, NULL, NULL, ptval);	
	if ( SOCKET_ERROR == ret )
	{	
		int ErrorID = WSAGetLastError();
		m_SrrorStr = GetWSAErrorString(ErrorID);
		return false; 
	}
	if ( 0 == ret )
	{	return true;	}

	std::vector<size_t> RemoveIndexList;
	for ( i=0; i<Count; i++)
	{
		SOCKET Socket=m_ClientSocketList[i];
		if (FD_ISSET(Socket, &fdread))
		{				
			ret = recv(Socket, szbuffer, len, 0);//0->disconnected
			if ( SOCKET_ERROR==ret || 0==ret ) 
			{
				RemoveIndexList.push_back(i);
				continue;
			}
			if ( GetUseRecvMsgList() == true )
			{	AddRecvMsg(szbuffer); }
			m_RecvBuf = szbuffer;
			//ret = send(Socket, szbuffer, strlen(szbuffer), 0);
			ShowRecv(m_RecvBuf.c_str());
		}
	}

	const size_t RemoveCount = RemoveIndexList.size();
	if ( RemoveCount > 0 ) 
	{
		LockSocketList();
		for ( i=0; i<RemoveCount; i++ )
		{	RemoveClientSocket(RemoveIndexList[RemoveCount-i-1]);	}
		UnlockSocketList();
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetSocketServerSelect::ExecThread_Connection()//執行連線執行緒
{
	if ( CheckSocket() == false )
	{	return false; }	

	SOCKADDR_IN AddIn;
	SOCKET      sClient;	
	SOCKET      sServer=m_Socket;
	int         iaddrSize = sizeof(SOCKADDR_IN);	
	sClient = accept(sServer, (struct sockaddr *)&AddIn, &iaddrSize);		
	if ( INVALID_SOCKET == sClient ) 
	{
		if ( true == m_bclosed ) 
		{	return false; }

		int ErrorID = WSAGetLastError();
		if ( WSAEWOULDBLOCK==ErrorID) // non-blocking call, no request pending
		{	return true;	}

		m_SrrorStr = GetWSAErrorString(ErrorID);
		return false;
	}

	// Associate socket with network event
	if ( AddClientSocket(sClient) == false )
	{	return false; }	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetSocketServerSelect::BroadcastOut(const std::string& s)
{
	size_t i=0;	
	LockSocketList();
	const size_t Count = GetClientSocketCount();
	for ( i=0; i<Count; i++ )
	{	send(m_ClientSocketList[i],s.c_str(),s.length(),0);	}	
	UnlockSocketList();	
	return true;
}
//-------------------------------------------------------------------------------------//
CJetSocketServerEvent::CJetSocketServerEvent()
{	
}
//-------------------------------------------------------------------------------------//
CJetSocketServerEvent::CJetSocketServerEvent(int port, int connections, TypeSocket type)
{	
	CreateServer(port, connections, type);
}
//-------------------------------------------------------------------------------------//
CJetSocketServerEvent::~CJetSocketServerEvent()
{
	CloseServer();
	ClearClientSocketList();	
}
//-------------------------------------------------------------------------------------//
bool CJetSocketServerEvent::CloseServer()
{
	CloseSocket();
	CloseThread_Connection();
	return true;
}
//-------------------------------------------------------------------------------------//	
bool CJetSocketServerEvent::CreateServer(int port, int connections, TypeSocket type)
{
	CloseServer();

	sockaddr_in sa;
	memset(&sa, 0, sizeof(sa));

	sa.sin_family = PF_INET;             
	sa.sin_port = htons(port);          
	m_Socket = socket(AF_INET, SOCK_STREAM, 0);//socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);	
	if (INVALID_SOCKET == m_Socket) 
	{	
		//throw "INVALID_SOCKET";	
		m_SrrorStr = "Error, INVALID_SOCKET";	
		return false;
	}

	if(type==NonBlockingSocket) 
	{
		u_long arg = 1;
		ioctlsocket(m_Socket, FIONBIO, &arg);
	}

	/* bind the socket to the internet address */
	if (::bind(m_Socket, (sockaddr *)&sa, sizeof(sockaddr_in)) == SOCKET_ERROR) 
	{
		closesocket(m_Socket);
		//throw "INVALID_SOCKET";
		m_SrrorStr = "Error, Bind Fault";
		return false;
	}  
	listen(m_Socket, connections);
	m_bclosed=false;
	SetThreadStop_Recv(false);
	SetThreadStop_Send(false);
	SetThreadStop_Conn(false);
	CreateThread_Connection();	
	CreateThread_Recv();
	CreateThread_Send();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetSocketServerEvent::ExecThread_Recv()
{
	int              ret, index;
	WSANETWORKEVENTS NetworkEvents;
	char             szbuffer[MSGSIZE]="";
	const size_t EventCnt = GetClientEventCount();
	if ( 0 == EventCnt )	{	return true; }

	ret = WSAWaitForMultipleEvents(EventCnt, &(m_ClientEventList[0]), FALSE, 1000, FALSE);
	if (ret == WSA_WAIT_FAILED || ret == WSA_WAIT_TIMEOUT)
	{	return true;	}
	
	bool bRemoveIndex=false;
	index = ret - WSA_WAIT_EVENT_0;	
	SOCKET   Socket=m_ClientSocketList[index];
	WSAEVENT SocketEvent=m_ClientEventList[index];
	WSAEnumNetworkEvents(Socket, SocketEvent, &NetworkEvents);
	if (NetworkEvents.lNetworkEvents & FD_READ)
	{		
		ret = recv(Socket, szbuffer, MSGSIZE, 0);
		if ( SOCKET_ERROR == ret ) 
		{	bRemoveIndex = true;	}
		else
		{	
			if ( GetUseRecvMsgList() == true )
			{	AddRecvMsg(szbuffer); }
			m_RecvBuf = szbuffer;
			ShowRecv(m_RecvBuf.c_str());
			//ret = send(Socket, szbuffer, strlen(szbuffer), 0);
			//if ( SOCKET_ERROR  == ret ) 
			//{	bRemoveIndex = true;	}
		}
	}
	if (NetworkEvents.lNetworkEvents & FD_CLOSE)
	{	bRemoveIndex = true;	}

	if ( true == bRemoveIndex )
	{
		LockSocketList();
		RemoveClientSocket(index);
		UnlockSocketList();		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetSocketServerEvent::ExecThread_Connection()
{
	if ( CheckSocket() == false )
	{	return false; }	

	SOCKADDR_IN AddIn;
	SOCKET      sClient;	
	SOCKET      sServer=m_Socket;
	int         iaddrSize = sizeof(SOCKADDR_IN);	
	sClient = accept(sServer, (struct sockaddr *)&AddIn, &iaddrSize);		
	if ( INVALID_SOCKET == sClient ) 
	{
		if ( true == m_bclosed ) 
		{	return false; }

		int ErrorID = WSAGetLastError();
		if ( WSAEWOULDBLOCK==ErrorID) // non-blocking call, no request pending
		{	return true;	}

		m_SrrorStr = GetWSAErrorString(ErrorID);
		return false;
	}

	// Associate socket with network event
	if ( AddClientSocket(sClient) == false )
	{	return false; }	
	return true;
}
//-------------------------------------------------------------------------------------//
size_t CJetSocketServerEvent::GetClientEventCount()
{
	return m_ClientEventList.size();
}
//-------------------------------------------------------------------------------------//
size_t  CJetSocketServerEvent::GetClientSocketCount()
{
	return m_ClientSocketList.size();
}
//-------------------------------------------------------------------------------------//
bool CJetSocketServerEvent::ClearClientSocketList()
{	
	size_t i=0;
	LockSocketList();
	const size_t SocketCount=GetClientSocketCount();
	for ( i=0; i<SocketCount; i++ )
	{	
		::closesocket(m_ClientSocketList[i]);	
		m_ClientSocketList[i] = INVALID_SOCKET;
	}
	m_ClientSocketList.clear();

	const size_t EventCount=GetClientEventCount();
	for ( i=0; i<SocketCount; i++ )
	{	
		::WSACloseEvent(m_ClientEventList[i]);	
		m_ClientEventList[i] = NULL;
	}	
	m_ClientEventList.clear();
	UnlockSocketList();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetSocketServerEvent::AddClientSocket(SOCKET s)
{
	const size_t Count=GetClientSocketCount();
	if ( Count >= MAX_SOCKET_CLIENT_COUNT ) 
	{	return false; }
	WSAEVENT wsEvent=WSACreateEvent();	
	if ( WSA_INVALID_EVENT==wsEvent)
	{	return false;	}	
	int ret = WSAEventSelect(s, wsEvent, FD_READ | FD_CLOSE);
	if ( SOCKET_ERROR  == ret )
	{
		WSACloseEvent(wsEvent);
		return false;
	}

	LockSocketList();
	m_ClientSocketList.push_back(s);
	m_ClientEventList.push_back(wsEvent);
	UnlockSocketList();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetSocketServerEvent::RemoveClientSocket(size_t index)
{	
	const size_t SocketCount = GetClientSocketCount();	
	if ( index < SocketCount )
	{
		closesocket(m_ClientSocketList[index]);	
		m_ClientSocketList.erase(m_ClientSocketList.begin()+index);
	}
	const size_t EventCount = GetClientEventCount();
	if ( index < EventCount )
	{
		WSACloseEvent(m_ClientEventList[index]);	
		m_ClientEventList.erase(m_ClientEventList.begin()+index);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetSocketServerEvent::BroadcastOut(const std::string& s)
{
	size_t i=0;	
	LockSocketList();
	const size_t Count = GetClientSocketCount();
	for ( i=0; i<Count; i++ )
	{	send(m_ClientSocketList[i],s.c_str(),s.length(),0);	}	
	UnlockSocketList();	
	return true;
}
//-------------------------------------------------------------------------------------//