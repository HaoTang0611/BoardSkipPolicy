// LogManager.cpp: implementation of the CLogManager class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "LogManager.h"
//-------------------------------------------------------------------------------------//
#include <process.h>
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
CLogManager             LogManager;
//-------------------------------------------------------------------------------------//
unsigned int  LogManagerThreadID = 0; //訊息檔管理器執行緒編號
HANDLE LogManagerThreadHandle = NULL; //訊息檔管理器執行緒處理碼 
HANDLE LogManagerThreadEvent = NULL;  //訊息檔管理器執行緒事件
unsigned int __stdcall LogManagerThreadFn(void *pParam);
//-------------------------------------------------------------------------------------//
unsigned int __stdcall LogManagerThreadFn(void *pParam)
{
	bool IsOK = true;
	DWORD  SleepTime = 100;	
	THREAD_STATE_MODE   ThreadState = THREAD_STATE_NONE;
	THREAD_COMMAND_MODE ThreadCmd = THREAD_COMMAND_TO_NONE;		
	
	double Time = 0;
	LARGE_INTEGER          nStartTime;
	LARGE_INTEGER          nEndTime;
	TCHAR strBuffer[256]=_T("");
	while ( true )
	{	
		SleepTime = LogManager.GetThreadDwellTime();
		ThreadCmd = LogManager.GetLogManagerThreadCmd();
		if ( THREAD_COMMAND_TO_EXIT == ThreadCmd )
		{	break;	}
		if ( THREAD_COMMAND_TO_IDLE == ThreadCmd ) 
		{ 			
			LogManager.SetLogManagerThreadState(THREAD_STATE_IDLE);		
			::Sleep(SleepTime);	
			continue; 
		}

		if ( THREAD_COMMAND_TO_RUN == ThreadCmd ) 
		{
			if ( THREAD_STATE_FINISH==ThreadState || THREAD_STATE_EXCEPTION==ThreadState)
			{
				::Sleep(SleepTime);
				continue;
			}
			//::SetThreadPriority(LogManagerThreadHandle, THREAD_PRIORITY_ABOVE_NORMAL);
			LogManager.SetLogManagerThreadState(THREAD_STATE_RUNNING);		
			QueryPerformanceCounter(&nStartTime);
			IsOK = LogManager.FlushLogMsgToFile();
			QueryPerformanceCounter(&nEndTime);
			if ( false == IsOK )
			{	
				CString ErrorMSG = LogManager.GetErrorString();				
				LogManager.SetLogManagerThreadState(THREAD_STATE_EXCEPTION);
				AOIExceptionCodeCtrl.SetAOIExceptionCode_FileWrite(ErrorMSG);
				JetAPI::ShowMessageBox(ErrorMSG);
			}
			else
			{	LogManager.SetLogManagerThreadState(THREAD_STATE_FINISH);	}

			/*
			LogManager.SetLogManagerThreadCmd(THREAD_COMMAND_TO_IDLE);
			if ( NULL  != LogManagerThreadEvent )
			{	::SetEvent(LogManagerThreadEvent); }
			*/			
			//continue;
		}
		::Sleep(SleepTime);
	};
	LogManager.SetLogManagerThreadState(THREAD_STATE_NONE);
	if ( NULL != LogManagerThreadEvent )
	{	::SetEvent(LogManagerThreadEvent); }
//	::_endthreadex(0);

	return 0;
}
//-------------------------------------------------------------------------------------//

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
CLogManager::CLogManager()
{
	::InitializeCriticalSection(&m_csLogManager);
	CLogManager::PreInitLogManager();
	CLogManager::InitialLogManager();

	CLogManager::CreateLogManagerThread();
	CLogManager::StartLogManagerThread(true);
}
//-------------------------------------------------------------------------------------//
CLogManager::~CLogManager()
{
	CLogManager::DeleteLogManagerThread();
	::DeleteCriticalSection(&m_csLogManager);
}
//-------------------------------------------------------------------------------------//
void CLogManager::PreInitLogManager()
{
	m_ThreadDwellTime = 250;//每隔250ms寫入檔案
	m_LogManagerThreadCmd = THREAD_COMMAND_TO_NONE;
	m_LogManagerThreadState = THREAD_STATE_NONE;
}
//-------------------------------------------------------------------------------------//
void CLogManager::InitialLogManager()
{
}
//-------------------------------------------------------------------------------------//
void CLogManager::LockLogManager()
{
	::EnterCriticalSection(&m_csLogManager);	
}
//-------------------------------------------------------------------------------------//
void CLogManager::UnlockLogManager()
{
	::LeaveCriticalSection(&m_csLogManager);	
}
//-------------------------------------------------------------------------------------//
LPCTSTR CLogManager::GetErrorString() const
{
	return m_ErrorString;
}
//-------------------------------------------------------------------------------------//
bool CLogManager::CreateLogManagerThread()//建立相機圖填滿執行緒
{	
	CString str;	
	if ( this->DeleteLogManagerThread() == false ) { return false; }	

	m_LogManagerThreadCmd = THREAD_COMMAND_TO_IDLE;
	LogManagerThreadHandle = (HANDLE)::_beginthreadex(NULL, NULL, &LogManagerThreadFn, 0, NULL, &LogManagerThreadID);
	if ( NULL == LogManagerThreadHandle )
	{	
		m_LogManagerThreadState = THREAD_STATE_NONE;
		this->m_ErrorString = _T("Eorror, Create Log Manager Thread Fault (LogManagerThreadHandle == NULL)");		
		AOIExceptionCodeCtrl.SetAOIExceptionCode_Thread(AOI_EXCEPTION_THREAD_CREATE, m_ErrorString);
		return false;	
	}

	str = _T("Log Manager Event");
	LogManagerThreadEvent = JetAPI::CreateEvent(NULL, TRUE, TRUE, str);
	::SetThreadPriority(LogManagerThreadHandle, THREAD_PRIORITY_BELOW_NORMAL); 	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLogManager::DeleteLogManagerThread()//刪除相機圖填滿執行緒
{	
	DWORD WaitTime = 1000;//1 sec
	if ( NULL == LogManagerThreadHandle ) { return true; }
	SetLogManagerThreadCmd(THREAD_COMMAND_TO_EXIT);	
	DWORD Res = ::WaitForSingleObject(LogManagerThreadHandle, WaitTime);
	::CloseHandle(LogManagerThreadHandle); 
	LogManagerThreadHandle = NULL;
	if ( NULL != LogManagerThreadEvent )
	{	
		::CloseHandle(LogManagerThreadEvent); 
		LogManagerThreadEvent=NULL; 	
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLogManager::StartLogManagerThread(bool WaitOn)//開始相機圖填滿執行緒
{
	if ( NULL != LogManagerThreadEvent )
	{	::ResetEvent(LogManagerThreadEvent); }
	this->SetLogManagerThreadCmd(THREAD_COMMAND_TO_RUN);	

	if ( WaitOn == true ) 
	{	
		size_t i = 0;
		const size_t MaxCount = 1000;
		const size_t SleepTime = 10;
		for ( i=0; i<MaxCount; i++ )
		{
			if ( THREAD_COMMAND_TO_RUN != m_LogManagerThreadCmd )//切入下一個階段
			{	break; }
			if ( THREAD_STATE_IDLE != m_LogManagerThreadState )
			{	break; }
			::Sleep(SleepTime);
		}
		if ( i == MaxCount )
		{
			this->m_ErrorString = _T("Error, wait for StartLogManagerThread too long");
			AOIExceptionCodeCtrl.SetAOIExceptionCode_Thread(AOI_EXCEPTION_THREAD_WAIT_FOR_START, m_ErrorString);
			return false;		
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLogManager::StopLogManagerThread(bool WaitOn)
{
	//if ( NULL != LogManagerThreadEvent )
	//{	::ResetEvent(LogManagerThreadEvent); }
	this->SetLogManagerThreadCmd(THREAD_COMMAND_TO_IDLE);
	if ( WaitOn == true ) 
	{	
		size_t i = 0;
		const size_t MaxCount = 1000;
		const size_t SleepTime = 10;
		for ( i=0; i<MaxCount; i++ )
		{
			if ( THREAD_STATE_NONE == m_LogManagerThreadState )
			{	break; }
			if ( THREAD_STATE_IDLE == m_LogManagerThreadState )
			{	break; }
			::Sleep(SleepTime);
		}
		if ( i == MaxCount )
		{
			this->m_ErrorString = _T("Error, wait for StopLogManagerThread too long");
			AOIExceptionCodeCtrl.SetAOIExceptionCode_Thread(AOI_EXCEPTION_THREAD_WAIT_FOR_STOP, m_ErrorString);
			return false;		
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLogManager::WaitForLogManagerThreadFinish()//等待相機圖填滿執行緒結束
{
	size_t i=0;	
	DWORD Res = 0;		
	const DWORD SleepTime = 100;		
	const size_t MaxCounts = 100+100;
	if ( NULL == LogManagerThreadEvent ) { return true; }
	for ( i=0; i<MaxCounts; i++ )
	{		
		if ( THREAD_COMMAND_TO_EXIT == m_LogManagerThreadCmd ) { return true; }		
		Res = ::WaitForSingleObject(LogManagerThreadEvent, SleepTime);
		if ( Res != WAIT_TIMEOUT ) { break; }
	}
	if ( i == MaxCounts )
	{
		this->m_ErrorString = _T("Error, Wait for WaitForLogManagerThreadFinish too long");
		AOIExceptionCodeCtrl.SetAOIExceptionCode_Thread(AOI_EXCEPTION_THREAD_WAIT_FOR_FINISH, m_ErrorString);
		return false;
	}		

	return true;
}
//-------------------------------------------------------------------------------------//
THREAD_COMMAND_MODE CLogManager::GetLogManagerThreadCmd() const
{
	return m_LogManagerThreadCmd;
}
//-------------------------------------------------------------------------------------//
void CLogManager::SetLogManagerThreadCmd(THREAD_COMMAND_MODE Cmd)
{
	if ( m_LogManagerThreadCmd == Cmd ) { return; }
	CLogManager::LockLogManager();
	CLogManager::m_LogManagerThreadCmd = Cmd;
	CLogManager::UnlockLogManager();
}
//-------------------------------------------------------------------------------------//	
THREAD_STATE_MODE CLogManager::GetLogManagerThreadState() const
{
	return m_LogManagerThreadState;
}
//-------------------------------------------------------------------------------------//
void CLogManager::SetLogManagerThreadState(THREAD_STATE_MODE State)
{
	if ( m_LogManagerThreadState == State ) { return; }
	CLogManager::LockLogManager();
	CLogManager::m_LogManagerThreadState = State;
	CLogManager::UnlockLogManager();
}
//-------------------------------------------------------------------------------------//
bool CLogManager::CheckLogIndex(unsigned int idx)
{	
	const size_t LogCount = m_LogList.size();	
	if ( idx >= LogCount ) 
	{
		m_ErrorString.Format(_T("Error, log index is out of range %d/%d"), idx, LogCount);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
unsigned int CLogManager::AddLogFile(LPCTSTR Folder, LPCTSTR Filename, LPCTSTR ExtName, LPCTSTR Backup, CLogNode::LOG_FILENAME_MODE Mode, bool bSaveHeader)
{
	CLogNode     NewLogNode;
	unsigned int NewIdx = (unsigned int)(m_LogList.size());
	NewLogNode.SetLogFile(Folder, Filename, ExtName, Backup, Mode, bSaveHeader);
	CLogManager::LockLogManager();
	m_LogList.push_back(NewLogNode);
	CLogManager::UnlockLogManager();
	return NewIdx;
}
//-------------------------------------------------------------------------------------//
unsigned int CLogManager::AddLogFileOnRuning(LPCTSTR Folder, LPCTSTR Filename, LPCTSTR ExtName, LPCTSTR Backup, CLogNode::LOG_FILENAME_MODE Mode, bool bSaveHeader)
{
	bool bWaitOn=true;
	unsigned int Index=-1;
	if ( StopLogManagerThread(bWaitOn) == false ) { return Index; }
	Index = AddLogFile(Folder, Filename, ExtName, Backup, Mode, bSaveHeader);
	StartLogManagerThread(false);
	return Index;
}
//-------------------------------------------------------------------------------------//
bool CLogManager::ResetLogFilename(unsigned int idx, LPCTSTR Folder, LPCTSTR Filename, LPCTSTR ExtName)
{
	if ( CheckLogIndex(idx) == false ) 
	{	return false; }

	//清掉舊的
	CLogManager::LockLogManager();
	m_LogList[idx].PushBackLogMsg();
	m_LogList[idx].FlushLogFile();
	m_LogList[idx].SetLogFile(Folder, Filename, ExtName);
	CLogManager::UnlockLogManager();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLogManager::DeleteLogFile(unsigned int idx)
{
	if ( CheckLogIndex(idx) == false ) 
	{	return false; }
	bool IsOK = true;
	CLogManager::LockLogManager();
	IsOK = m_LogList[idx].DeleteLogFile();
	if ( false == IsOK )
	{	this->m_ErrorString = m_LogList[idx].GetErrorString();	}	
	CLogManager::UnlockLogManager();	
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CLogManager::RemoveLogNode(unsigned int idx)//移除訊息物件
{
	if ( CheckLogIndex(idx) == false ) 
	{	return false; }	
	const bool WaitOn=true;
	if ( StopLogManagerThread(WaitOn) == false )
	{	return false; }

	std::vector<CLogNode> LogList=m_LogList;
	const unsigned int Count=LogList.size();
	m_LogList.clear();
	for ( unsigned int i=0; i<Count; i++ )
	{
		if ( i == idx ) { continue; }		
		m_LogList.push_back(LogList[i]);
	}	
	if ( StartLogManagerThread(false) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLogManager::GetLogFilePathName(unsigned int idx, CString &Name)
{
	if ( CheckLogIndex(idx) == false ) 
	{	return false; }	
	CLogManager::LockLogManager();
	Name = m_LogList[idx].GetLogPathName();	
	CLogManager::UnlockLogManager();	
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CLogManager::AddLogMessage(unsigned int idx, const char *Msg)
{
	if ( CheckLogIndex(idx) == false ) 
	{	return false; }	
	CLogManager::LockLogManager();
	m_LogList[idx].AddLogString(Msg);
	CLogManager::UnlockLogManager();

#ifdef SAVE_LOG_MSG_SYNC_USE
	if ( FlushLogNodeMsgToFile(idx) == false )
	{	return false; }
#endif//SAVE_LOG_MSG_SYNC_USE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLogManager::AddLogMessage(unsigned int idx, const wchar_t* Msg)
{
	if ( CheckLogIndex(idx) == false ) 
	{	return false; }	
	CLogManager::LockLogManager();
	m_LogList[idx].AddLogString(Msg);
	CLogManager::UnlockLogManager();

#ifdef SAVE_LOG_MSG_SYNC_USE
	if ( FlushLogNodeMsgToFile(idx) == false )
	{	return false; }
#endif//SAVE_LOG_MSG_SYNC_USE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLogManager::GetLogLastMessageTime(unsigned int idx, CString &Time)
{
	if ( CheckLogIndex(idx) == false ) 
	{	return false; }		
	CLogManager::LockLogManager();
	SYSTEMTIME SysTime = m_LogList[idx].GetLogLastTime();
	Time.Format(_T("%.4d%.2d%.2d%.2d%.2d%.2d"), SysTime.wYear, SysTime.wMonth, SysTime.wDay, SysTime.wHour, SysTime.wMinute, SysTime.wSecond);
	CLogManager::UnlockLogManager();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLogManager::FlushLogMsgToFile()
{
	size_t i=0;	
	const size_t LogCount = m_LogList.size();
	if ( 0 == LogCount ) { return true; }

	//Switch Msg
	CLogManager::LockLogManager();
	for ( i=0; i<LogCount; i++ )
	{	m_LogList[i].PushBackLogMsg();	}
	CLogManager::UnlockLogManager();

	//Save To File
	for ( i=0; i<LogCount; i++ )
	{
		if ( m_LogList[i].FlushLogFile() == false )
		{
			this->m_ErrorString = m_LogList[i].GetErrorString();
			return false;
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLogManager::FlushLogNodeMsgToFile(unsigned int idx)
{
	if ( CheckLogIndex(idx) == false ) 
	{	return false; }	
	bool IsOK=true;
	DWORD Timeout=10000;
	CLogManager::LockLogManager();
	CLogNode &LogNode=m_LogList[idx];
	IsOK = LogNode.WaitLogMsgEmpty(Timeout);	
	if ( false == IsOK )
	{	m_ErrorString = LogNode.GetErrorString();	}
	else
	{
		LogNode.PushBackLogMsg();
		IsOK = LogNode.FlushLogFile();
		if ( false == IsOK )
		{	m_ErrorString = LogNode.GetErrorString();}
	}
	CLogManager::UnlockLogManager();
	return IsOK;
}
//-------------------------------------------------------------------------------------//
