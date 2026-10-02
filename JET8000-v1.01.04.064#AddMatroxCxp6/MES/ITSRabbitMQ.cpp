// ITSRabbitMQ.cpp: implementation of the CITSRabbitMQ class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "ITSRabbitMQ.h"
//-------------------------------------------------------------------------------------//
#include <thread>
//-------------------------------------------------------------------------------------//
#include "ITSLinker.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
unsigned int __stdcall thRabbitRecvFn(void *pParam);//接收訊息執行緒函式
//-------------------------------------------------------------------------------------//
unsigned int __stdcall thRabbitRecvFn(void *pParam)//接收訊息執行緒函式
{
	CITSRabbitMQ *RabbitPtr = (CITSRabbitMQ*)(pParam);
	if ( NULL == RabbitPtr ) { return -1; }
	
	DWORD  dwTime = 20;
	bool    bThreadStop = false;	
	bool    bThreadExit = false;
	while (TRUE)
	{			
		bThreadStop = RabbitPtr->GetThreadStop();
		bThreadExit = RabbitPtr->GetThreadExit();		
		dwTime = RabbitPtr->GetThreadSleepTime();
		if ( true == bThreadExit )
		{	break; }

		if ( true == bThreadStop ) 
		{
			if ( dwTime > 0 ) //避免空跑滿載
			{	::Sleep(dwTime); }		
			continue;
		}		
		
		if ( RabbitPtr->ExecThread_Recv() == false )
		{	break; }		

		if ( dwTime > 0 ) //避免空跑滿載
		{	::Sleep(dwTime); }		
	}	
	return 0;	
}
//-------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
CITSRabbitMQ::CITSRabbitMQ()
{	
	m_ITSLinkerPtr = NULL;
	m_hWnd = NULL;
	m_uMsg = 0;
	m_wRecv = 0;
	m_wSend = 0;
	m_Connected = false;
	m_ThreadExit = true;
	m_ThreadStop = true;
	m_ThreadSleepTime = 50;
	
	m_thRecvID = 0;  //接收訊息執行緒編號
	m_thRecvHandle = NULL; //接收訊息執行緒處理碼
	::InitializeCriticalSection(&m_csRecvThread);
	::InitializeCriticalSection(&m_csRecvMsgList);
	CreateThread_Recv();
}
//-------------------------------------------------------------------------------------//
CITSRabbitMQ::~CITSRabbitMQ()
{	
	CloseThread_Recv();
	Disconnect();
	::DeleteCriticalSection(&m_csRecvMsgList);
	::DeleteCriticalSection(&m_csRecvThread);	
}
//-------------------------------------------------------------------------------------//
CITSLinker* CITSRabbitMQ::GetITSLinkerPtr()
{
	return m_ITSLinkerPtr;
}
//-------------------------------------------------------------------------------------//
void CITSRabbitMQ::SetITSLinkerPtr(CITSLinker *Ptr)
{
	m_ITSLinkerPtr = Ptr;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CITSRabbitMQ::GetErrorString() const
{
	return m_ErrorString;
}
//-------------------------------------------------------------------------------------//
void CITSRabbitMQ::SetHWnd(HWND h, UINT m, WPARAM wRecv, WPARAM wSend)
{
	m_hWnd = h;
	m_uMsg = m;
	m_wRecv = wRecv;
	m_wSend = wSend;
}
//-------------------------------------------------------------------------------------//
bool CITSRabbitMQ::GetThreadStop() const
{
	return m_ThreadStop;
}
//-------------------------------------------------------------------------------------//
void CITSRabbitMQ::SetThreadStop(bool val)
{
	m_ThreadStop = val;
}
//-------------------------------------------------------------------------------------//
bool CITSRabbitMQ::GetThreadExit() const
{
	return m_ThreadExit;
}
//-------------------------------------------------------------------------------------//
void CITSRabbitMQ::SetThreadExit(bool val)
{
	m_ThreadExit = val;
}
//-------------------------------------------------------------------------------------//
void CITSRabbitMQ::LockRecvThread()
{
	::EnterCriticalSection(&m_csRecvThread);
}
//-------------------------------------------------------------------------------------//
void CITSRabbitMQ::UnockRecvThread()
{
	::LeaveCriticalSection(&m_csRecvThread);
}
//-------------------------------------------------------------------------------------//
void CITSRabbitMQ::LockRecvMsgList()
{
	::EnterCriticalSection(&m_csRecvMsgList);
}
//-------------------------------------------------------------------------------------//
void CITSRabbitMQ::UnlockRecvMsgList()
{
	::LeaveCriticalSection(&m_csRecvMsgList);
}
//-------------------------------------------------------------------------------------//
DWORD CITSRabbitMQ::GetThreadSleepTime() const
{
	return m_ThreadSleepTime;
}
//-------------------------------------------------------------------------------------//
void  CITSRabbitMQ::SetThreadSleepTime(DWORD val)
{
	m_ThreadSleepTime = val;
}
//-------------------------------------------------------------------------------------//
void CITSRabbitMQ::ShowRecv(const char *Str)
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
void CITSRabbitMQ::ShowSend(const char *Str)
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
bool CITSRabbitMQ::CloseThread_Recv()//關閉接受訊息的執行續
{	
	if ( NULL == m_thRecvHandle )
	{	return true; }
	SetThreadExit(true);
	DWORD ret = WaitForSingleObject(m_thRecvHandle, 1000);
	::CloseHandle(m_thRecvHandle);
	m_thRecvID = -1;
	m_thRecvHandle = NULL;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CITSRabbitMQ::CreateThread_Recv()
{
	CloseThread_Recv();
	SetThreadExit(false);
	SetThreadStop(false);
	m_thRecvHandle = (HANDLE)::_beginthreadex(NULL, NULL, &thRabbitRecvFn, (LPVOID)this, NULL, &m_thRecvID);		
	//AOIExceptionCodeCtrl.SetAOIExceptionCode_Thread(AOI_EXCEPTION_THREAD_CREATE, m_ErrorString);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CITSRabbitMQ::ExecThread_Recv()
{
	if ( GetThreadExit() == true )
	{	return true; }
	if ( GetThreadStop() == true )
	{	return true; }
	if (theRabbitmqUnit().IsLoginSucc() == false )
	{	return true; }

	//below code must implement into the thread
	while (1)
	{
		Sleep(20);
		if ( GetThreadExit() == true )
		{	return true; }
		if ( GetThreadStop() == true )
		{	return true; }
		for (auto itr : m_ReceIdList)
		{
			if ( GetThreadExit() == true )
			{	return true; }
			if ( GetThreadStop() == true )
			{	return true; }
			std::queue<std::string> DataList;
			if (theRabbitmqUnit().Receive(itr, DataList))
			{
				while (DataList.size())
				{
					if ( GetThreadExit() == true )
					{	return true; }
					if ( GetThreadStop() == true )
					{	return true; }
					AddRecvMsg(DataList.front());		
					ShowRecv(DataList.front().c_str());
					DataList.pop();
				}
			}
		}		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CITSRabbitMQ::ConnectServer(const std::string &rIpAddr, int Port)
{
	bool bOK = true;
	Port = 5672;
	std::string host2 = "localhost";
	bOK = theRabbitmqUnit().ConnectServer(host2, Port);
	if ( false == bOK ) 
	{	
		m_ErrorString = theRabbitmqUnit().GetLastError().c_str();
		return false;
	}	
	m_Connected = true;
	return true;	
}
//-------------------------------------------------------------------------------------//
void CITSRabbitMQ::Disconnect()
{		
	if ( false == m_Connected ) { return ; }
	theRabbitmqUnit().Disconnect();	
	m_Connected = false;
}
//-------------------------------------------------------------------------------------//
void CITSRabbitMQ::Login(const std::string &rUser, const std::string &rPwd, const std::string &rVHost)
{	
	//void Login(const std::string &rUser, const std::string &rPwd, const std::string &rVHost);
	theRabbitmqUnit().Login(rUser, rPwd, rVHost);//使用者登錄
}
//-------------------------------------------------------------------------------------//
void CITSRabbitMQ::RegisterSendName(const std::string &Name)
{
	m_SendName = Name;
}
//-------------------------------------------------------------------------------------//
bool CITSRabbitMQ::RegisterReceiveName(const std::string &Name)
{
	CConnectObj Paras(CT_QUEUE_DIR, Name);		
	m_ReceIdList.clear();
	m_ReceIdList.push_back(theRabbitmqUnit().RegisterReceiveName(Paras));	
	return true;
}
//-------------------------------------------------------------------------------------//
void CITSRabbitMQ::Fire()
{
	theRabbitmqUnit().Fire();
}
//-------------------------------------------------------------------------------------//
bool CITSRabbitMQ::IsLoginSucc() const
{
	return theRabbitmqUnit().IsLoginSucc();	
}
//-------------------------------------------------------------------------------------//
bool CITSRabbitMQ::Send(const std::string &rData)
{
	//queue send
	ShowSend(rData.c_str());
	CConnectObj Paras(CT_QUEUE_DIR, m_SendName);
	return theRabbitmqUnit().Send(Paras, rData);	
}
//-------------------------------------------------------------------------------------//
bool CITSRabbitMQ::Receive(int Id, std::queue<std::string> &rData)
{
	return theRabbitmqUnit().Receive(Id, rData);
}
//-------------------------------------------------------------------------------------//
bool CITSRabbitMQ::AddRecvMsg(const std::string &msg)
{
	LockRecvMsgList();	
	m_RecvMsgList.push_back(msg);
	UnlockRecvMsgList();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CITSRabbitMQ::CloneRecvMsgList(std::vector<std::string> &sList, bool bClear)//取得接收訊息列表		
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