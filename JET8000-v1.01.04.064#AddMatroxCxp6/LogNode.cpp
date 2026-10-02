// LogNode.cpp: implementation of the CLogNode class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "LogNode.h"
//-------------------------------------------------------------------------------------//
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
CLogNode::CLogNode()
{
	CLogNode::PreInitLogNode();
	CLogNode::InitialLogNode();
}
//-------------------------------------------------------------------------------------//
CLogNode::CLogNode(const CLogNode &Log)
{
	CLogNode::PreInitLogNode();
	CLogNode::CloneLogNode(Log);
}
//-------------------------------------------------------------------------------------//
CLogNode::~CLogNode()
{

}
//-------------------------------------------------------------------------------------//
CLogNode& CLogNode::operator=(const CLogNode &Log)
{
	if ( this == &Log ) { return *this; }
	CLogNode::CloneLogNode(Log);
	return *this;
}
//-------------------------------------------------------------------------------------//
void CLogNode::PreInitLogNode()
{
	::GetLocalTime(&m_LogNameDatTime);
}
//-------------------------------------------------------------------------------------//
void CLogNode::InitialLogNode()
{
	CLogNode::m_ErrorString = _T("");

	CLogNode::m_FilenameMode = LOG_FILENAME_FIX;
	CLogNode::m_LogFolder = _T("");
	CLogNode::m_LogFilename = _T("");
	CLogNode::m_LogExtName = _T("");	
	CLogNode::m_LogPathName = _T("");	
	CLogNode::m_LogBackup = _T("");	
	CLogNode::m_LogMsgListIn.clear();
	CLogNode::m_LogMsgListOut.clear();
	::GetLocalTime(&m_LogNameDatTime);
}
//-------------------------------------------------------------------------------------//
void CLogNode::CloneLogNode(const CLogNode &Log)
{
	CLogNode::m_ErrorString = Log.m_ErrorString;

	CLogNode::m_FilenameMode = Log.m_FilenameMode;
	CLogNode::m_LogFolder = Log.m_LogFolder;
	CLogNode::m_LogFilename = Log.m_LogFilename;
	CLogNode::m_LogExtName = Log.m_LogExtName;
	CLogNode::m_LogPathName = Log.m_LogPathName;
	CLogNode::m_LogBackup = Log.m_LogBackup;	
	CLogNode::m_LogNameDatTime = Log.m_LogNameDatTime;
	CLogNode::m_LogMsgListIn = Log.m_LogMsgListIn;
	CLogNode::m_LogMsgListOut = Log.m_LogMsgListOut;
	CLogNode::m_LogSaveHeader = Log.m_LogSaveHeader;	
}
//-------------------------------------------------------------------------------------//
LPCTSTR CLogNode::GetErrorString() const
{
	return CLogNode::m_ErrorString;
}
//-------------------------------------------------------------------------------------//
SYSTEMTIME CLogNode::GetLogLastTime() const
{
	return CLogNode::m_LogNameDatTime;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CLogNode::GetLogFolder() const
{
	return m_LogFolder;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CLogNode::GetLogFilename() const
{
	return m_LogFilename;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CLogNode::GetLogPathName() const
{
	return m_LogPathName;
}
//-------------------------------------------------------------------------------------//
void CLogNode::GetPathFilename(CString &filename)
{	
	switch ( m_FilenameMode )
	{
	case LOG_FILENAME_BY_DATE:
	{
		::GetLocalTime(&m_LogNameDatTime);
		filename.Format(_T("%s\\%s_%.4d_%.2d_%.2d.%s"), m_LogFolder, m_LogFilename, m_LogNameDatTime.wYear, m_LogNameDatTime.wMonth, m_LogNameDatTime.wDay, m_LogExtName);		
	}
		break;
	default:
		filename.Format(_T("%s\\%s.%s"), m_LogFolder, m_LogFilename, m_LogExtName);
		break;
	}	
}
//-------------------------------------------------------------------------------------//
void CLogNode::SetLogFile(LPCTSTR Folder, LPCTSTR Filename, LPCTSTR ExtName)
{
	CLogNode::m_LogFolder = Folder;
	CLogNode::m_LogFilename = Filename;
	CLogNode::m_LogExtName = ExtName;
	CLogNode::GetPathFilename(m_LogPathName);		
}
//-------------------------------------------------------------------------------------//
void CLogNode::SetLogFile(LPCTSTR Folder, LPCTSTR Filename, LPCTSTR ExtName, LPCTSTR Backup, CLogNode::LOG_FILENAME_MODE Mode, bool bSaveHeader)//設定訊息檔案路徑與主檔名與副檔名
{
	m_FilenameMode = Mode;
	CLogNode::m_LogFolder = Folder;
	CLogNode::m_LogFilename = Filename;
	CLogNode::m_LogExtName = ExtName;
	CLogNode::m_LogBackup = Backup;
	CLogNode::m_LogSaveHeader = bSaveHeader;
	CLogNode::GetPathFilename(m_LogPathName);	
}
//-------------------------------------------------------------------------------------//
bool CLogNode::AddLogString(const char* Txt)
{	
	TLOG_MSG_TXT LogMsg;	
	LogMsg.Msg = Txt;	
	::GetLocalTime(&LogMsg.SysTime);	
	CLogNode::m_LogMsgListIn.push_back(LogMsg);		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLogNode::AddLogString(const wchar_t* Txt)
{	
	TLOG_MSG_TXT LogMsg;		
	LogMsg.Msg = Txt;	
	::GetLocalTime(&LogMsg.SysTime);	
	CLogNode::m_LogMsgListIn.push_back(LogMsg);		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLogNode::FlushLogFile()
{
	size_t       i=0;
	CString      Result;
	LPCTSTR      TxtPtr=NULL;
	TLOG_MSG_TXT *LogMsgPtr = NULL;
	const size_t TxtCount = m_LogMsgListOut.size();
	const size_t szBackup = m_LogBackup.GetLength();	
	if ( 0 == TxtCount ) { return true; }
	const bool bSaveHeader = m_LogSaveHeader;

	//for write file ccs=UTF-16LE, ccs=UTF-8, for read file ccs=UNICODE
	TCHAR TMode[32]=_T("");
	FILE *pfile = NULL;

	_tcscpy(TMode, _T("a+"));
	JetAPI::ModifyOpenFileMode_Write(TMode);
	pfile = ::_tfopen(m_LogPathName, TMode);

	if ( pfile == NULL ) 
	{
	#ifdef _DEBUG
		m_ErrorString.Format(_T("Error, Can Not Open Log File (%s)"), m_LogPathName);		
		return false;
	#else
		return true;
	#endif//_DEBUG		
	}	

	for ( i=0; i<TxtCount; i++ )
	{
		//TxtPtr = m_LogMsgListOut[i];
		LogMsgPtr = &(m_LogMsgListOut[i]);

		if ( LOG_FILENAME_BY_DATE == m_FilenameMode )
		{	
			if ( LogMsgPtr->SysTime.wYear!=m_LogNameDatTime.wYear || 
				 LogMsgPtr->SysTime.wMonth!=m_LogNameDatTime.wMonth ||
				 LogMsgPtr->SysTime.wDay!=m_LogNameDatTime.wDay )
			{
				::fclose(pfile);
				pfile = NULL;

				m_LogNameDatTime = LogMsgPtr->SysTime;
				m_LogPathName.Format(_T("%s\\%s_%.4d_%.2d_%.2d.%s"), m_LogFolder, m_LogFilename,m_LogNameDatTime.wYear, m_LogNameDatTime.wMonth, m_LogNameDatTime.wDay, m_LogExtName);
				pfile = ::_tfopen(m_LogPathName, _T("a+"));
				if ( pfile == NULL ) 
				{
					#ifdef _DEBUG
						m_ErrorString.Format(_T("Error, Can Not Open Log File (%s)"), m_LogPathName);		
						return false;
					#else
						return true;
					#endif//_DEBUG
				}
			}
		}

	#ifdef OFFLINE_VERSION			
		if ( false == bSaveHeader )
		{
			if ( 0 == szBackup ) 
			{	Result = LogMsgPtr->Msg;	}
			else
			{	Result.Format(_T("%s %s"), m_LogBackup, LogMsgPtr->Msg);	}
		}
		else
		{
			if ( 0 == szBackup ) 
			{
				Result.Format(_T("%.4d/%.2d/%.2d,%.2d:%.2d:%.2d.%.3d_Offline %s"),
						LogMsgPtr->SysTime.wYear,LogMsgPtr->SysTime.wMonth,LogMsgPtr->SysTime.wDay,
						LogMsgPtr->SysTime.wHour,LogMsgPtr->SysTime.wMinute,LogMsgPtr->SysTime.wSecond,LogMsgPtr->SysTime.wMilliseconds,
						LogMsgPtr->Msg);
			}
			else
			{
				Result.Format(_T("%.4d/%.2d/%.2d,%.2d:%.2d:%.2d.%.3d_Offline%s %s"),
						LogMsgPtr->SysTime.wYear,LogMsgPtr->SysTime.wMonth,LogMsgPtr->SysTime.wDay,
						LogMsgPtr->SysTime.wHour,LogMsgPtr->SysTime.wMinute,LogMsgPtr->SysTime.wSecond,LogMsgPtr->SysTime.wMilliseconds,
						m_LogBackup, LogMsgPtr->Msg);
			}
		}
	#else
		if ( false == bSaveHeader )
		{
			if ( 0 == szBackup ) 
			{	Result = LogMsgPtr->Msg;	}
			else
			{	Result.Format(_T("%s %s"), m_LogBackup, LogMsgPtr->Msg);	}
		}
		else
		{
			if ( 0 == szBackup ) 
			{
				Result.Format(_T("%.4d/%.2d/%.2d,%.2d:%.2d:%.2d.%.3d_Online %s"),
						LogMsgPtr->SysTime.wYear,LogMsgPtr->SysTime.wMonth,LogMsgPtr->SysTime.wDay,
						LogMsgPtr->SysTime.wHour,LogMsgPtr->SysTime.wMinute,LogMsgPtr->SysTime.wSecond,LogMsgPtr->SysTime.wMilliseconds,
						(LPCTSTR)LogMsgPtr->Msg);
			}
			else
			{
				Result.Format(_T("%.4d/%.2d/%.2d,%.2d:%.2d:%.2d.%.3d_Online%s %s"),
						LogMsgPtr->SysTime.wYear,LogMsgPtr->SysTime.wMonth,LogMsgPtr->SysTime.wDay,
						LogMsgPtr->SysTime.wHour,LogMsgPtr->SysTime.wMinute,LogMsgPtr->SysTime.wSecond,LogMsgPtr->SysTime.wMilliseconds,
						m_LogBackup, LogMsgPtr->Msg);
			}
		}
	#endif
		_ftprintf(pfile,_T("%s\n"), (LPCTSTR)Result.GetString());		
	}
	::fclose(pfile);
	pfile = NULL;

	m_LogMsgListOut.clear();
	return true;
}
//-------------------------------------------------------------------------------------//
void CLogNode::PushBackLogMsg()
{
	size_t i=0;
	const size_t CountIn  = m_LogMsgListIn.size();
	if ( 0 == CountIn ) { return; }	
	
	for ( i=0; i<CountIn; i++ )
	{	m_LogMsgListOut.push_back(m_LogMsgListIn[i]);	}
	m_LogMsgListIn.clear();	
}
//-------------------------------------------------------------------------------------//
bool CLogNode::CheckLogMsgOutEmpty() const//確認訊息輸出完畢	
{
	return m_LogMsgListOut.empty();
}
//-------------------------------------------------------------------------------------//
bool CLogNode::WaitLogMsgEmpty(DWORD Timeout)//等待訊息輸出完畢	
{	
	DWORD SleepTime=10;
	DWORD Count=MAX(Timeout/SleepTime, 10);	
	for ( DWORD i=0; i<Count; i++ )
	{
		if ( true == CheckLogMsgOutEmpty() )
		{	return true;	}
		::Sleep(SleepTime);
	}
	this->m_ErrorString.Format(_T("Error, Wait Write Log Msg Timeout [%s]"), m_LogPathName);
	return false;
}
//-------------------------------------------------------------------------------------//
bool CLogNode::DeleteLogFile()
{
	if ( ::DeleteFile(m_LogPathName) == FALSE )
	{
		this->m_ErrorString.Format(_T("Error, Delete File Fault [%s]"), m_LogPathName);
		return false; 
	}
	return true;
}
//-------------------------------------------------------------------------------------//