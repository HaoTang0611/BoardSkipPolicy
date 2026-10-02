// ITSLinker.cpp: implementation of the CITSLinker class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "ITSLinker.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
CITSLinker::CITSLinker()
{
	m_ITSLinkMode = ITS_LINK_SOCKET;
	//m_ITSLinkMode = ITS_LINK_RABBIT_MQ;

	m_TickCountRecv = 0;	
	m_TickCountSend = 0;		
	::InitializeCriticalSection(&m_csRecvSend);

	m_ITSSocket.SetITSLinkerPtr(this);
	m_ITSRabbitMQ.SetITSLinkerPtr(this);
	m_ITSFileChecker.SetITSLinkerPtr(this);
}
//-------------------------------------------------------------------------------------//
CITSLinker::~CITSLinker()
{
	::DeleteCriticalSection(&m_csRecvSend);
}
//-------------------------------------------------------------------------------------//
LPCTSTR CITSLinker::GetErrorString()
{
	return m_ErrorString;
}
//-------------------------------------------------------------------------------------//
ITS_LINK_MODE CITSLinker::GetITSLinkMode() const
{
	return m_ITSLinkMode;
}
//-------------------------------------------------------------------------------------//
//bool CITSLinker::ConnectITS(const std::string& host, int port)
bool CITSLinker::ConnectITS_Socket(const std::string& host, int port)
{
	bool bOK = true;
	bOK = m_ITSSocket.Connect(host, port);
	if ( false == bOK ) 
	{	
		m_ErrorString = m_ITSSocket.GetErrorString().c_str();	
		return false;
	}
	m_ITSSocket.SetUseRecvMsgList(true);
	m_ITSLinkMode = ITS_LINK_SOCKET;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CITSLinker::ConnectITS_Rabbit(const std::string& host, int port, const std::string &rName, const std::string &rPwd, const std::string &rRecv, const std::string &rSend)
{
	bool bOK = false;
	//port = 5672;
	//std::string host2 = "localhost";
	bOK = m_ITSRabbitMQ.ConnectServer(host, port);
	if ( false == bOK ) 
	{	
		m_ErrorString = theRabbitmqUnit().GetLastError().c_str();
		return false;
	}

	std::string UserName=rName;//"aoiuser";
	std::string Password=rPwd;//"123456";
	std::string VHost = "/";	
	m_ITSRabbitMQ.Login(UserName, Password, VHost);//使用者登錄		
	m_ITSRabbitMQ.Fire();		

	::Sleep(300);
	m_ITSRabbitMQ.RegisterSendName(rSend);//"AOI2ITS"
	m_ITSRabbitMQ.RegisterReceiveName(rRecv);//"ITS2AOI"
	m_ITSLinkMode = ITS_LINK_RABBIT_MQ;
	return bOK;
}
//-------------------------------------------------------------------------------------//
bool CITSLinker::ConnectITS_File(LPCTSTR fdSend, LPCTSTR fdRecv, LPCTSTR fdTemp, bool bBackup, bool bUseSync, LPCTSTR bkSend, LPCTSTR bkRecv)
{
	if ( m_ITSFileChecker.ConnectServer(fdSend, fdRecv, fdTemp) == false )
	{
		m_ErrorString = m_ITSFileChecker.GetErrorString();
		return false;
	}	
	m_ITSLinkMode = ITS_LINK_FILE;
	m_ITSFileChecker.SetBackupFile(bBackup);
	m_ITSFileChecker.SetUseSyncFile(bUseSync);
	m_ITSFileChecker.SetBackupFolderSend(bkSend);
	m_ITSFileChecker.SetBackupFolderRecv(bkRecv);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CITSLinker::GetITSConnected()
{
	bool bConnected=false;
	if ( ITS_LINK_SOCKET == m_ITSLinkMode ) 
	{	bConnected = m_ITSSocket.GetConnected();	}	
	if ( ITS_LINK_RABBIT_MQ == m_ITSLinkMode ) 
	{	bConnected = m_ITSRabbitMQ.IsLoginSucc();	}	
	if ( ITS_LINK_FILE == m_ITSLinkMode ) 
	{	bConnected = m_ITSFileChecker.GetConnected();	}	
	return bConnected;
}
//-------------------------------------------------------------------------------------//
void CITSLinker::DisconnectITS()
{
	if ( ITS_LINK_SOCKET == m_ITSLinkMode ) 
	{	m_ITSSocket.CloseSocket();	}
	if ( ITS_LINK_RABBIT_MQ == m_ITSLinkMode ) 
	{	m_ITSRabbitMQ.Disconnect();	}
	if ( ITS_LINK_FILE == m_ITSLinkMode ) 
	{	m_ITSFileChecker.Disconnect();	}	
}
//-------------------------------------------------------------------------------------//
void CITSLinker::SetFreezeComm(bool val)//凍結通訊
{
	if ( ITS_LINK_SOCKET == m_ITSLinkMode ) 
	{	} 
	if ( ITS_LINK_RABBIT_MQ == m_ITSLinkMode ) 
	{	}
	if ( ITS_LINK_FILE == m_ITSLinkMode ) 
	{	m_ITSFileChecker.SetFreezeComm(val); }	
}
//-------------------------------------------------------------------------------------//
void CITSLinker::SetITSHWnd(HWND hWnd, UINT msg, WPARAM wRecv, WPARAM wSend)
{
	if ( ITS_LINK_SOCKET == m_ITSLinkMode ) 
	{	m_ITSSocket.SetHWnd(hWnd, msg, wRecv, wSend);	} 
	if ( ITS_LINK_RABBIT_MQ == m_ITSLinkMode ) 
	{	m_ITSRabbitMQ.SetHWnd(hWnd, msg, wRecv, wSend);	}
	if ( ITS_LINK_FILE == m_ITSLinkMode ) 
	{	m_ITSFileChecker.SetHWnd(hWnd, msg, wRecv, wSend);	}	
}
//-------------------------------------------------------------------------------------//
bool CITSLinker::AddITSSendMsg(const std::string &msg, LPCTSTR filename)
{
	bool bOK=true;
	if ( ITS_LINK_SOCKET == m_ITSLinkMode ) 
	{
		bOK = m_ITSSocket.AddSendMsg(msg);
		if ( false == bOK )
		{
			m_ErrorString = m_ITSSocket.GetErrorString().c_str();	
			return false;
		}
	}
	if ( ITS_LINK_RABBIT_MQ == m_ITSLinkMode ) 
	{	
		bOK = m_ITSRabbitMQ.Send(msg);
		if ( false == bOK ) 
		{
			m_ErrorString = m_ITSRabbitMQ.GetErrorString();
			return false;
		}
	}
	if ( ITS_LINK_FILE == m_ITSLinkMode ) 
	{	
		bOK = m_ITSFileChecker.Send(msg, filename);
		if ( false == bOK ) 
		{
			m_ErrorString = m_ITSFileChecker.GetErrorString();
			return false;
		}
	}
	return bOK;
}
//-------------------------------------------------------------------------------------//
bool CITSLinker::CloneITSRecvMsgList(std::vector<std::string> &sList, bool bClear)//取得接收訊息列表		
{
	bool bOK = false;
	if ( ITS_LINK_SOCKET == m_ITSLinkMode ) 
	{
		bOK = m_ITSSocket.CloneRecvMsgList(sList, bClear);
		if ( false == bOK ) 
		{
			m_ErrorString = m_ITSSocket.GetErrorString().c_str();	
			return false;
		}
	}
	if ( ITS_LINK_RABBIT_MQ == m_ITSLinkMode ) 
	{
		bOK = m_ITSRabbitMQ.CloneRecvMsgList(sList, bClear);
		if ( false == bOK ) 
		{
			m_ErrorString = m_ITSRabbitMQ.GetErrorString();	
			return false;
		}
	}
	if ( ITS_LINK_FILE == m_ITSLinkMode ) 
	{
		bOK = m_ITSFileChecker.CloneRecvMsgList(sList, bClear);
		if ( false == bOK ) 
		{
			m_ErrorString = m_ITSFileChecker.GetErrorString();	
			return false;
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CITSLinker::GetITSThreadStop() const
{
	bool bStop = true;
	if ( ITS_LINK_SOCKET == m_ITSLinkMode ) 
	{	bStop = m_ITSSocket.GetThreadStop_Recv();	}
	if ( ITS_LINK_RABBIT_MQ == m_ITSLinkMode ) 
	{	bStop = m_ITSRabbitMQ.GetThreadStop();	}
	if ( ITS_LINK_FILE == m_ITSLinkMode ) 
	{	bStop = m_ITSFileChecker.GetThreadStop();	}
	return bStop;
}
//-------------------------------------------------------------------------------------//
void CITSLinker::SetITSThreadStop(bool val)
{
	if ( ITS_LINK_SOCKET == m_ITSLinkMode ) 
	{	m_ITSSocket.SetThreadStop_Recv(val);	}
	if ( ITS_LINK_RABBIT_MQ == m_ITSLinkMode ) 
	{	m_ITSRabbitMQ.SetThreadStop(val);	}
	if ( ITS_LINK_FILE == m_ITSLinkMode ) 
	{	m_ITSFileChecker.SetThreadStop(val);	}
}
//-------------------------------------------------------------------------------------//
CJetSocketClient& CITSLinker::GetITSSocket()//連到ITS軟體的WinSocket	
{
	return m_ITSSocket;
}
//-------------------------------------------------------------------------------------//
CITSRabbitMQ& CITSLinker::GetITSRabbitMQ()//連到Rabbit Server的物件
{
	return m_ITSRabbitMQ;
}
//-------------------------------------------------------------------------------------//
CITSFileChecker& CITSLinker::GetITSFileChecker()//連到ITS的檔案檢查器
{
	return m_ITSFileChecker;
}
//-------------------------------------------------------------------------------------//
void CITSLinker::LockBufferRecv()
{
	::EnterCriticalSection(&m_csRecvSend);
}
//-------------------------------------------------------------------------------------//
void CITSLinker::UnlockBufferRecv()
{
	::LeaveCriticalSection(&m_csRecvSend);
}
//-------------------------------------------------------------------------------------//
void CITSLinker::LockBufferSend()
{
	::EnterCriticalSection(&m_csRecvSend);
}
//-------------------------------------------------------------------------------------//
void CITSLinker::UnlockBufferSend()
{
	::LeaveCriticalSection(&m_csRecvSend);
}
//-------------------------------------------------------------------------------------//
DWORD CITSLinker::GetTickCountRecv() const
{
	return m_TickCountRecv;
}
//-------------------------------------------------------------------------------------//
void CITSLinker::SetTickCountRecv(DWORD val)
{
	m_TickCountRecv = val;
}
//-------------------------------------------------------------------------------------//
const char* CITSLinker::GetNewBufferRecv() const
{
	return m_NewBufferRecv.c_str();
}
//-------------------------------------------------------------------------------------//
void CITSLinker::SetNewBufferRecv(const char *str)
{
	LockBufferRecv();
	SetTickCountRecv(GetTickCount());
	m_NewBufferRecv = str;
	UnlockBufferRecv();
}
//-------------------------------------------------------------------------------------//
DWORD CITSLinker::GetTickCountSend() const
{
	return m_TickCountSend;
}
//-------------------------------------------------------------------------------------//
void CITSLinker::SetTickCountSend(DWORD val)
{
	m_TickCountSend = val;
}
//-------------------------------------------------------------------------------------//
const char* CITSLinker::GetNewBufferSend() const
{
	return m_NewBufferSend.c_str();
}
//-------------------------------------------------------------------------------------//
void CITSLinker::SetNewBufferSend(const char *str)
{
	LockBufferSend();
	SetTickCountSend(GetTickCount());
	m_NewBufferSend = str;
	UnlockBufferSend();
}
//-------------------------------------------------------------------------------------//	