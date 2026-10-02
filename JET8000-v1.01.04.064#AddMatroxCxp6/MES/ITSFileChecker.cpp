// ITSFileCheck.cpp: implementation of the CITSFileCheck class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "MES_IPS_Define.h"
#include "ITSFileChecker.h"
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
const int nUTF32BE = 1;
const int nUTF32LE = 2;
const int nUTF16BE = 3;
const int nUTF16LE = 4;
const int nUTF8    = 5;	
//-------------------------------------------------------------------------------------//
#define  CHECK_FILE_EXT_NAME   _T("JSON")
#define  CHECK_FILE_SYNC_NAME  _T("SYNC")
//-------------------------------------------------------------------------------------//
unsigned int __stdcall thFileRecvFn(void *pParam);//接收訊息執行緒函式
//-------------------------------------------------------------------------------------//
unsigned int __stdcall thFileRecvFn(void *pParam)//接收訊息執行緒函式
{
	CITSFileChecker *FileChkerPtr = (CITSFileChecker*)(pParam);
	if ( NULL == FileChkerPtr ) { return -1; }
	
	DWORD  dwTime = 20;
	bool    bThreadStop = false;	
	bool    bThreadExit = false;
	while (TRUE)
	{			
		bThreadStop = FileChkerPtr->GetThreadStop();
		bThreadExit = FileChkerPtr->GetThreadExit();		
		dwTime = FileChkerPtr->GetThreadSleepTime();
		if ( true == bThreadExit )
		{	break; }

		if ( true == bThreadStop ) 
		{
			if ( dwTime > 0 ) //避免空跑滿載
			{	::Sleep(dwTime); }		
			continue;
		}		
		
		if ( FileChkerPtr->ExecThread_Recv() == false )
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
CITSFileChecker::CITSFileChecker()
{
	m_ITSLinkerPtr = NULL;
	m_Connected = false;	
	m_FreezeComm = false;
	m_UseSyncFile = false;
	m_TheSameFolder = false;
	m_CreateSyncFileDelayTime=0;
	m_BackupFile = false;
	m_hWnd = NULL;
	m_uMsg = 0;
	m_wRecv = 0;
	m_wSend = 0;

	m_FileCheckMode = MES_FILE_CHECKER_ITS;

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
CITSFileChecker::~CITSFileChecker()
{
	CloseThread_Recv();
	Disconnect();
	::DeleteCriticalSection(&m_csRecvMsgList);
	::DeleteCriticalSection(&m_csRecvThread);	
}
//-------------------------------------------------------------------------------------//
CITSLinker* CITSFileChecker::GetITSLinkerPtr()
{
	return m_ITSLinkerPtr;
}
//-------------------------------------------------------------------------------------//
void CITSFileChecker::SetITSLinkerPtr(CITSLinker *Ptr)
{
	m_ITSLinkerPtr = Ptr;
}
//-------------------------------------------------------------------------------------//	
LPCTSTR CITSFileChecker::GetErrorString() const
{
	return m_ErrorString;
}
//-------------------------------------------------------------------------------------//
void CITSFileChecker::SetHWnd(HWND h, UINT m, WPARAM wRecv, WPARAM wSend)
{
	m_hWnd = h;
	m_uMsg = m;
	m_wRecv = wRecv;
	m_wSend = wSend;
}
//-------------------------------------------------------------------------------------//
bool CITSFileChecker::GetThreadStop() const
{
	return m_ThreadStop;
}
//-------------------------------------------------------------------------------------//
void CITSFileChecker::SetThreadStop(bool val)
{
	m_ThreadStop = val;
}
//-------------------------------------------------------------------------------------//
bool CITSFileChecker::GetThreadExit() const
{
	return m_ThreadExit;
}
//-------------------------------------------------------------------------------------//
void CITSFileChecker::SetThreadExit(bool val)
{
	m_ThreadExit = val;
}
//-------------------------------------------------------------------------------------//
void CITSFileChecker::LockRecvThread()
{
	::EnterCriticalSection(&m_csRecvThread);
}
//-------------------------------------------------------------------------------------//
void CITSFileChecker::UnockRecvThread()
{
	::LeaveCriticalSection(&m_csRecvThread);
}
//-------------------------------------------------------------------------------------//
void CITSFileChecker::LockRecvMsgList()
{
	::EnterCriticalSection(&m_csRecvMsgList);
}
//-------------------------------------------------------------------------------------//
void CITSFileChecker::UnlockRecvMsgList()
{
	::LeaveCriticalSection(&m_csRecvMsgList);
}
//-------------------------------------------------------------------------------------//
DWORD CITSFileChecker::GetThreadSleepTime() const
{
	return m_ThreadSleepTime;
}
//-------------------------------------------------------------------------------------//
void  CITSFileChecker::SetThreadSleepTime(DWORD val)
{
	m_ThreadSleepTime = val;
}
//-------------------------------------------------------------------------------------//
void CITSFileChecker::ShowRecv(const char *Str)
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
void CITSFileChecker::ShowSend(const char *Str)
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
bool CITSFileChecker::CheckConnected()
{
	if ( GetConnected() == false )
	{
		m_ErrorString = _T("Error, ITSFileChecker Not Ready");
		return false;
	}
	if ( m_FolderRecv.GetLength()==0 || m_FolderSend.GetLength()==0 || m_FolderTemp.GetLength()==0 )
	{ 
		m_ErrorString = _T("Error, ITSFileChecker Not Ready");
		return false; 
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CITSFileChecker::GetConnected() const
{
	return m_Connected;
}
//-------------------------------------------------------------------------------------//
bool CITSFileChecker::GetFreezeComm() const
{
	return m_FreezeComm;
}
//-------------------------------------------------------------------------------------//
void CITSFileChecker::SetFreezeComm(bool val)
{
	m_FreezeComm = val;
}
//-------------------------------------------------------------------------------------//
bool CITSFileChecker::CheckAddFilename() const
{	
	if ( GetFileCheckMode() == MES_FILE_CHECKER_IPS ) { return true; }
	return false;
}
//-------------------------------------------------------------------------------------//
bool CITSFileChecker::ExecAddFilename(std::string &str, CString filename)
{
	if ( CheckAddFilename() == false ) { return true; }
	std::wstring wsFilename;
	const UINT ConvertCode = CP_UTF8;
	JetAPI::TCHAR2wstring(filename, wsFilename, ConvertCode);

	int bRet=false;
	std::wstring wsBuf;
	rapidjson::CGMItr itr;	
	rapidjson::WDocument Doc;
	rapidjson::CJsonCtrl JSonCtrl;

	if ( JetAPI::char2wstring(str.c_str(), wsBuf, ConvertCode) == false ) 
	{	return false; }

	//initial
	Doc.SetObject();
	JSonCtrl.Set(&Doc);			
	if ( JSonCtrl.SetBuffer(wsBuf, Doc) == false ) 
	{	return false; }

#ifdef IPS_VERSION_V2
	bRet=JSonCtrl.AddMember(L"Filename", wsFilename);//Filename
#else
	bRet=JSonCtrl.AddMember(L"Filename_s", wsFilename);//Filename
#endif//IPS_VERSION_V2
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	bRet = JSonCtrl.GetBuffer(wsBuf, Doc);
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	if ( JetAPI::wchar2string(wsBuf.c_str(), str, ConvertCode) == false ) 
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CITSFileChecker::GetUseSyncFile() const
{
	return m_UseSyncFile;
}
//-------------------------------------------------------------------------------------//
void CITSFileChecker::SetUseSyncFile(bool val)
{
	m_UseSyncFile = val;
}
//-------------------------------------------------------------------------------------//
bool CITSFileChecker::CheckUseSyncFile() const
{
	if ( GetUseSyncFile() == true ) { return true; }
	if ( GetFileCheckMode() == MES_FILE_CHECKER_IPS ) { return true; }
	return false; 
}
//-------------------------------------------------------------------------------------//
bool CITSFileChecker::ExecAddSyncFile(LPCTSTR Filename)
{
	if ( CheckUseSyncFile() == false ) { return true; }
	CString MainName;
	CString TempName;
	CString SyncName;
	CString FolderTmp=m_FolderTemp;
	DWORD   DelayTime=GetCreateSyncFileDelayTime();
	JetAPI::ExtractMainFileName(Filename, MainName);
	TempName.Format(_T("%s\\%s"), FolderTmp, _T("Sync.TEMP"));
	SyncName.Format(_T("%s.%s"), MainName, CHECK_FILE_SYNC_NAME);

	if ( DelayTime > 0 )
	{	::Sleep(DelayTime); }

	int   i=0;	
	FILE *pfile=NULL;
	const int MaxCount=50;
	for ( i=0; i<MaxCount; i++ )
	{
		pfile=::_tfopen(TempName, _T("w+"));
		if ( NULL != pfile )
		{	
			::fclose(pfile); pfile=NULL;
			::MoveFile(TempName, SyncName);
			return true;
		}			
		::Sleep(10);
	}	
	m_ErrorString.Format(_T("Error, CITSFileChecker Create Sync File Fault [%s]"), SyncName);
	return false;
	//return true;
}
//-------------------------------------------------------------------------------------//
DWORD CITSFileChecker::GetCreateSyncFileDelayTime() const
{
	return m_CreateSyncFileDelayTime;
}
//-------------------------------------------------------------------------------------//
void CITSFileChecker::SetCreateSyncFileDelayTime(DWORD val)
{
	m_CreateSyncFileDelayTime = val;
}
//-------------------------------------------------------------------------------------//
MES_FILE_CHECKER_MODE CITSFileChecker::GetFileCheckMode() const
{
	return m_FileCheckMode;
}
//-------------------------------------------------------------------------------------//
void CITSFileChecker::SetFileCheckMode(MES_FILE_CHECKER_MODE val)
{
	m_FileCheckMode = val;
}
//-------------------------------------------------------------------------------------//
bool CITSFileChecker::GetTheSameFolder() const
{
	return m_TheSameFolder;
}
//-------------------------------------------------------------------------------------//
void CITSFileChecker::SetTheSameFolder(bool val)
{
	m_TheSameFolder = val;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CITSFileChecker::GetFilenameSend()//傳送檔案名稱
{
	return m_FilenameSend;
}
//-------------------------------------------------------------------------------------//
bool CITSFileChecker::CloseThread_Recv()//關閉接受訊息的執行續
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
bool CITSFileChecker::CreateThread_Recv()
{
	CloseThread_Recv();
	SetThreadExit(false);
	SetThreadStop(false);
	m_thRecvHandle = (HANDLE)::_beginthreadex(NULL, NULL, &thFileRecvFn, (LPVOID)this, NULL, &m_thRecvID);		
	//AOIExceptionCodeCtrl.SetAOIExceptionCode_Thread(AOI_EXCEPTION_THREAD_CREATE, m_ErrorString);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CITSFileChecker::ExecThread_Recv()
{
	if ( GetThreadExit() == true )
	{	return true; }
	if ( GetThreadStop() == true )
	{	return true; }
	if ( GetConnected() == false )	
	{	return true; }

	//below code must implement into the thread
	while (1)
	{
		Sleep(20);
		if ( GetThreadExit() == true )
		{	return true; }
		if ( GetThreadStop() == true )
		{	return true; }			
		
		std::queue<std::string> DataList;
		if ( Receive(DataList) == true )		
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
	return true;
}
//-------------------------------------------------------------------------------------//
bool CITSFileChecker::ConnectServer(LPCTSTR fdSend, LPCTSTR fdRecv, LPCTSTR fdTemp)
{
	bool bOK = true;
	if ( JetAPI::CreateFolder(fdSend) == false )
	{
		m_ErrorString.Format(_T("Error, ITSFileChecker Create Folder Fault\n(%s)"), fdSend);
		return false;
	}
	if ( JetAPI::CreateFolder(fdRecv) == false )
	{
		m_ErrorString.Format(_T("Error, ITSFileChecker Create Folder Fault\n(%s)"), fdRecv);
		return false;
	}
	if ( JetAPI::CreateFolder(fdTemp) == false )
	{
		m_ErrorString.Format(_T("Error, ITSFileChecker Create Folder Fault\n(%s)"), fdTemp);
		return false;
	}
	JetAPI::ClearFolder(fdSend);

	m_FolderTemp = fdTemp;
	m_FolderSend = fdSend;
	m_FolderRecv = fdRecv;	
	m_Connected = true;
	m_FilenameSend = _T("");
	m_FilenameSendChkCnt = 0;
	::memset(&m_FilenameSendTime, 0x00, sizeof(m_FilenameSendTime));
	if ( m_FolderSend.CompareNoCase(m_FolderRecv) == 0 )
	{	SetTheSameFolder(true); }
	else
	{	SetTheSameFolder(false); }
	return true;	
}
//-------------------------------------------------------------------------------------//
void CITSFileChecker::Disconnect()
{	
	m_Connected = false;	
	//m_FolderTemp = _T("");
	//m_FolderSend = _T("");
	//m_FolderRecv = _T("");
	m_FilenameSend = _T("");
	m_FilenameSendChkCnt = 0;
	::memset(&m_FilenameSendTime, 0x00, sizeof(m_FilenameSendTime));
}
//-------------------------------------------------------------------------------------//
bool CITSFileChecker::Send(const std::string &rData, LPCTSTR filename)
{
	bool IsOK = true;
	MES_FILE_CHECKER_MODE CheckerMode=GetFileCheckMode();
	switch ( CheckerMode )
	{
	case MES_FILE_CHECKER_IPS:	IsOK = Send_IPS(rData, filename);	break;

	default:
	case MES_FILE_CHECKER_ITS:	IsOK = Send_ITS(rData, filename);	break;
	}
	if ( true == IsOK )
	{
		if ( ExecAddSyncFile(m_FilenameSend) == false )
		{	return false; }
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CITSFileChecker::Send_ITS(const std::string &rData, LPCTSTR filename)
{
	if ( CheckConnected() == false ) { return false; }
	//queue send
	ShowSend(rData.c_str());

	DWORD   ChkCnt=0;
	CString strTime;
	CString Filename;
	SYSTEMTIME SysTime;
	CString FolderTemp = m_FolderTemp;	
	CString FolderSend = m_FolderSend;	
	::GetLocalTime(&SysTime);		
	strTime.Format(_T("%04d%02d%02d%02d%02d%02d_%03d"), SysTime.wYear, SysTime.wMonth, SysTime.wDay, SysTime.wHour, SysTime.wMinute, SysTime.wSecond, SysTime.wMilliseconds);
	if ( NULL != filename )
	{	Filename = filename;	}
	else
	{
		const SYSTEMTIME &LastSysTime=m_FilenameSendTime;
		if ( SysTime.wSecond!=LastSysTime.wSecond || SysTime.wMinute!=LastSysTime.wMinute || SysTime.wHour!=LastSysTime.wHour )
		{	ChkCnt = 0;	}
		else
		{	ChkCnt = m_FilenameSendChkCnt;	}
		Filename.Format(_T("%s\\%s#%04d.%s"), FolderSend, strTime, ChkCnt+1, CHECK_FILE_EXT_NAME);		
	}

	CString FilenameTemp;
	FilenameTemp.Format(_T("%s\\%s#%04d.Tmp"), FolderTemp, strTime, ChkCnt+1);	
	FILE *pfile = ::_tfopen(FilenameTemp, _T("w+"));
	if ( NULL == pfile )
	{
		m_ErrorString.Format(_T("Error, ITSFileChecker Open File Fault\n(%s)"), Filename);
		return false;
	}

	//Add UTF-8 Header Code
	unsigned char Utf8[3]={0xEF, 0xBB, 0xBF};
	::fwrite(Utf8, sizeof(Utf8), 1, pfile);

	const size_t sz=rData.size();
	size_t Res = ::fwrite(rData.c_str(), sz, 1, pfile);
	::fclose(pfile);
	if ( 0 == Res )
	{
		m_ErrorString.Format(_T("Error, ITSFileChecker Write File Fault\n(%s)"), Filename);
		return false;
	}	
	ExecBackupFile(true, FilenameTemp);
	::MoveFile(FilenameTemp, Filename);
	m_FilenameSend=Filename;
	m_FilenameSendTime=SysTime;
	m_FilenameSendChkCnt=ChkCnt+1;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CITSFileChecker::Send_IPS(const std::string &rData, LPCTSTR filename)
{
	if ( CheckConnected() == false ) { return false; }
	//queue send
	ShowSend(rData.c_str());

	DWORD   ChkCnt=0;
	CString strTime;
	CString Filename;
	SYSTEMTIME SysTime;
	CString FolderTemp = m_FolderTemp;	
	CString FolderSend = m_FolderSend;	
	CString RecvName=_T("ToSFC");
#ifdef IPS_VERSION_V2
	RecvName=_T("ToPCS");
#endif//IPS_VERSION_V2

	::GetLocalTime(&SysTime);		
	strTime.Format(_T("%04d%02d%02d%02d%02d%02d"), SysTime.wYear, SysTime.wMonth, SysTime.wDay, SysTime.wHour, SysTime.wMinute, SysTime.wSecond);
	//strTime.Format(_T("%04d%02d%02d%02d%02d%02d_%03d"), SysTime.wYear, SysTime.wMonth, SysTime.wDay, SysTime.wHour, SysTime.wMinute, SysTime.wSecond, SysTime.wMilliseconds);
	if ( NULL != filename )
	{	
		Filename = filename;	
		Filename.Format(_T("%s\\%s"), FolderSend, filename);
		GetFilenameDateTime_IPS(filename, RecvName, strTime, ChkCnt);
		ChkCnt = ChkCnt-1;
	}
	else
	{
		const SYSTEMTIME &LastSysTime=m_FilenameSendTime;
		if ( SysTime.wSecond!=LastSysTime.wSecond || SysTime.wMinute!=LastSysTime.wMinute || SysTime.wHour!=LastSysTime.wHour )
		{	ChkCnt = 0;	}
		else
		{	ChkCnt = m_FilenameSendChkCnt;	}
		Filename.Format(_T("%s\\%s_%s_%04d.%s"), FolderSend, RecvName, strTime, ChkCnt+1, CHECK_FILE_EXT_NAME);		
	}

	CString FilenameTemp;
	FilenameTemp.Format(_T("%s\\%s_%s_%04d.Tmp"), FolderTemp, RecvName, strTime, ChkCnt+1);	
	FILE *pfile = ::_tfopen(FilenameTemp, _T("w+"));
	if ( NULL == pfile )
	{
		m_ErrorString.Format(_T("Error, ITSFileChecker Open File Fault\n(%s)"), Filename);
		return false;
	}

	//Add UTF-8 Header Code
	unsigned char Utf8[3]={0xEF, 0xBB, 0xBF};
	::fwrite(Utf8, sizeof(Utf8), 1, pfile);

	const size_t sz=rData.size();
	size_t Res = ::fwrite(rData.c_str(), sz, 1, pfile);
	::fclose(pfile);
	if ( 0 == Res )
	{
		m_ErrorString.Format(_T("Error, ITSFileChecker Write File Fault\n(%s)"), Filename);
		return false;
	}	
	ExecBackupFile(true, FilenameTemp);
	::MoveFile(FilenameTemp, Filename);
	m_FilenameSend=Filename;
	m_FilenameSendTime=SysTime;
	m_FilenameSendChkCnt=ChkCnt+1;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CITSFileChecker::ReceiveExtName(CString &ExtName)
{	
	if ( CheckUseSyncFile() == true )
	{	ExtName = CHECK_FILE_SYNC_NAME; }
	else
	{	ExtName = CHECK_FILE_EXT_NAME; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CITSFileChecker::ReceiveFilenameCheck_IPS(LPCTSTR Filename)
{
	if ( NULL == Filename ) { return false; }	
	const bool TheSameFolder=GetTheSameFolder();
	if ( false == TheSameFolder ) { return true; }
	
	CString strKey;
	CString strACK;	
	CString strName;
	JetAPI::ExtractMainFileName(Filename, strName);	
	strName.MakeUpper();

	strKey=strName.Left(5);
	strACK=strName.Right(3);
	if (strKey.CompareNoCase(_T("TOSFC")) == 0 )
	{
		if ( strACK.CompareNoCase(_T("ACK")) != 0  )
		{	return false; }
		return true;
	}

	strKey=strName.Left(9);
	if (strKey.CompareNoCase(_T("TOJET8000")) == 0 )
	{	
		if ( strACK.CompareNoCase(_T("ACK")) == 0  )
		{	return false; }
		return true;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CITSFileChecker::Receive(std::queue<std::string> &rData)
{
	bool IsOK = true;
	MES_FILE_CHECKER_MODE CheckerMode=GetFileCheckMode();
	switch ( CheckerMode )
	{
	case MES_FILE_CHECKER_IPS:	IsOK = Receive_IPS(rData);	break;

	default:
	case MES_FILE_CHECKER_ITS:	IsOK = Receive_ITS(rData);	break;
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CITSFileChecker::Receive_ITS(std::queue<std::string> &rData)
{
	if ( CheckConnected() == false ) { return false; }
	DWORD   ChkCnt=0;
	CString str;	
	CString ExtName;
	CString Folder = m_FolderRecv;	
	std::vector<CString> FilenameList;
	const bool bUseSyncFile=CheckUseSyncFile();
	ReceiveExtName(ExtName);	
	if ( JetAPI::ListFilesInFolder(Folder, ExtName, FilenameList) == false )
	{
		m_ErrorString.Format(_T("Error, ITSFileChecker List Receieve File Fault\n(%s)"), Folder);
		return false;
	}
	
	int ErrCnt=0;
	int ErrCode=0;	
	size_t i=0, j=0;
	struct _stat64 buf;
	FILE *pfile = NULL;	
	CString MainName;
	CString Filename;
	CString ShortName;
	CString FolderBad;
	CString FolderLog;		
	CString FilenameSyn;
	CString FilenameBad;	
	CString FilenameLog;	
	const int ErrCode_OK       = 0;
	const int ErrCode_Tstati64 = 1;
	const int ErrCode_FileOpen = 2;
	const size_t FilenameCnt=FilenameList.size();
	CString FolderTmp = AOIDataCollect.GetAOITempDirectory();	
	FolderLog.Format(_T("%s\\%s"), FolderTmp, _T("ITS2AOI_Log"));
	FolderBad.Format(_T("%s\\%s"), FolderTmp, _T("ITS2AOI_Bad"));
	::CreateDirectory(FolderLog, NULL);
	::CreateDirectory(FolderBad, NULL);
	for ( i=0; i<FilenameCnt; i++ )
	{			
		ErrCnt = 0;
		ShortName=FilenameList[i];
		if ( true == bUseSyncFile )
		{
			FilenameSyn.Format(_T("%s\\%s"), Folder, ShortName);
			JetAPI::ExtractMainFileName(ShortName, MainName);
			ShortName.Format(_T("%s.%s"), MainName, CHECK_FILE_EXT_NAME);
		}
		if ( GetFreezeComm() == true ) { continue; }
		Filename.Format(_T("%s\\%s"), Folder, ShortName);		
		FilenameLog.Format(_T("%s\\%s"), FolderLog, ShortName);
		while ( true )
		{
			if ( GetFreezeComm() == true ) { break; }
			if ( ::CopyFile(Filename, FilenameLog, FALSE) == TRUE )
			{	break; }
			ErrCnt ++;
			if ( ErrCnt > 10 )
			{
				::DeleteFile(Filename);
				if ( FilenameSyn.GetLength() != 0 )
				{	::DeleteFile(FilenameSyn); }
				return false;
			}
			::Sleep(10);
		};		
		if ( GetFreezeComm() == true ) { continue; }
		if ( 0 != _tstati64(FilenameLog, &buf) ) 
		{
			ErrCnt ++;
			ErrCode = ErrCode_Tstati64;
			FilenameBad.Format(_T("%s\\%s"), FolderBad, ShortName);
			::CopyFile(FilenameLog, FilenameBad, FALSE);
			::DeleteFile(Filename);			
			::DeleteFile(FilenameLog);
			if ( FilenameSyn.GetLength() != 0 )
			{	::DeleteFile(FilenameSyn); }
			return false;
		}
		if ( 0 == buf.st_size )//檔案容量為0時, 先行跳過
		{	
			::DeleteFile(FilenameLog);
			continue;	
		}
		pfile = ::_tfopen(FilenameLog, _T("r"));
		if ( NULL == pfile ) 
		{ 
			ErrCnt ++;
			ErrCode = ErrCode_FileOpen;
			FilenameBad.Format(_T("%s\\%s"), FolderBad, ShortName);
			::CopyFile(FilenameLog, FilenameBad, FALSE);
			::DeleteFile(Filename);
			::DeleteFile(FilenameLog);
			if ( FilenameSyn.GetLength() != 0 )
			{	::DeleteFile(FilenameSyn); }
			return false;
		}
		std::string tmpStr(buf.st_size, '\0');		
		::fread((void*)(tmpStr.data()) , buf.st_size, 1, pfile);
		::fclose(pfile); pfile = NULL;
		RemoveUnicodeHeader(tmpStr);//可能會有Unicode檔案		
		const size_t tmpLen=tmpStr.size();
		rData.push(tmpStr);
		ExecBackupFile(false, FilenameLog);
		/*
		size_t wfLen2=0;
		std::wstring wrBuf2;		
		UINT ConvertCode = CP_UTF8;
		JetAPI::char2wstring(tmpStr.c_str(), wrBuf2, ConvertCode);
		wfLen2 = wrBuf2.size();

		size_t wfLen=0;		
		std::wstring wrBuf;		
		rapidjson::WDocument Doc;
		rapidjson::CJsonCtrl JSonCtrl;
		//initial
		Doc.SetObject();
		JSonCtrl.Set(&Doc);
		if ( JSonCtrl.OpenFile(Filename, Doc) == true )
		{
			JSonCtrl.GetBuffer(wrBuf, Doc);
			wfLen = wrBuf.size();
		}
		*/

		//Send(tmpStr);//For Debug Read Buffer Result		
		::DeleteFile(Filename);			
		::DeleteFile(FilenameLog);
		if ( FilenameSyn.GetLength() != 0 )
		{	::DeleteFile(FilenameSyn); }
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CITSFileChecker::Receive_IPS(std::queue<std::string> &rData)
{
	if ( CheckConnected() == false ) { return false; }
	DWORD   ChkCnt=0;
	CString str;	
	CString ExtName;
	CString Folder = m_FolderRecv;		
	std::vector<CString> FilenameList;	
	ReceiveExtName(ExtName);
	if ( JetAPI::ListFilesInFolder(Folder, ExtName, FilenameList) == false )
	{
		m_ErrorString.Format(_T("Error, ITSFileChecker List Receieve File Fault\n(%s)"), Folder);
		return false;
	}
	
	int ErrCnt=0;
	int ErrCode=0;	
	size_t i=0, j=0;
	struct _stat64 buf;
	FILE *pfile = NULL;	
	CString MainName;
	CString Filename;
	CString ShortName;
	CString FolderBad;
	CString FolderLog;
	CString FilenameSyn;
	CString FilenameBad;
	CString FilenameLog;	
	const int ErrCode_OK       = 0;
	const int ErrCode_Tstati64 = 1;
	const int ErrCode_FileOpen = 2;
	const size_t FilenameCnt=FilenameList.size();
	CString FolderTmp = AOIDataCollect.GetAOITempDirectory();	
	FolderLog.Format(_T("%s\\%s"), FolderTmp, _T("ITS2AOI_Log"));
	FolderBad.Format(_T("%s\\%s"), FolderTmp, _T("ITS2AOI_Bad"));
	::CreateDirectory(FolderLog, NULL);
	::CreateDirectory(FolderBad, NULL);
	for ( i=0; i<FilenameCnt; i++ )
	{			
		ErrCnt = 0;	
		ShortName=FilenameList[i];
		if ( GetFreezeComm() == true ) { continue; }
		if ( ReceiveFilenameCheck_IPS(ShortName) == false ) { continue; }
		FilenameSyn.Format(_T("%s\\%s"), Folder, ShortName);		

		JetAPI::ExtractMainFileName(ShortName, MainName);
		ShortName.Format(_T("%s.%s"), MainName, CHECK_FILE_EXT_NAME);
		Filename.Format(_T("%s\\%s"), Folder, ShortName);		
		FilenameLog.Format(_T("%s\\%s"), FolderLog, ShortName);		
		while ( true )
		{
			if ( GetFreezeComm() == true ) { break; }
			if ( ::CopyFile(Filename, FilenameLog, FALSE) == TRUE )
			{	break; }
			ErrCnt ++;
			if ( ErrCnt > 10 )
			{
				::DeleteFile(Filename);
				::DeleteFile(FilenameSyn);
				return false;
			}
			::Sleep(10);
		};		
		if ( GetFreezeComm() == true ) { continue; }
		if ( 0 != _tstati64(FilenameLog, &buf) ) 
		{
			ErrCnt ++;
			ErrCode = ErrCode_Tstati64;
			FilenameBad.Format(_T("%s\\%s"), FolderBad, ShortName);
			::CopyFile(FilenameLog, FilenameBad, FALSE);
			::DeleteFile(Filename);			
			::DeleteFile(FilenameLog);
			::DeleteFile(FilenameSyn);
			return false;
		}
		if ( 0 == buf.st_size )//檔案容量為0時, 先行跳過
		{	
			::DeleteFile(FilenameLog);
			continue;	
		}
		pfile = ::_tfopen(FilenameLog, _T("r"));
		if ( NULL == pfile ) 
		{ 
			ErrCnt ++;
			ErrCode = ErrCode_FileOpen;
			FilenameBad.Format(_T("%s\\%s"), FolderBad, ShortName);
			::CopyFile(FilenameLog, FilenameBad, FALSE);
			::DeleteFile(Filename);
			::DeleteFile(FilenameLog);
			::DeleteFile(FilenameSyn);			
			return false;
		}
		std::string tmpStr(buf.st_size, '\0');		
		::fread((void*)(tmpStr.data()) , buf.st_size, 1, pfile);
		::fclose(pfile); pfile = NULL;
		RemoveUnicodeHeader(tmpStr);//可能會有Unicode檔案
		ExecAddFilename(tmpStr, ShortName);		
		const size_t tmpLen=tmpStr.size();
		rData.push(tmpStr);
		ExecBackupFile(false, FilenameLog);
		/*
		size_t wfLen2=0;
		std::wstring wrBuf2;		
		UINT ConvertCode = CP_UTF8;
		JetAPI::char2wstring(tmpStr.c_str(), wrBuf2, ConvertCode);
		wfLen2 = wrBuf2.size();

		size_t wfLen=0;		
		std::wstring wrBuf;		
		rapidjson::WDocument Doc;
		rapidjson::CJsonCtrl JSonCtrl;
		//initial
		Doc.SetObject();
		JSonCtrl.Set(&Doc);
		if ( JSonCtrl.OpenFile(Filename, Doc) == true )
		{
			JSonCtrl.GetBuffer(wrBuf, Doc);
			wfLen = wrBuf.size();
		}
		*/

		//Send(tmpStr);//For Debug Read Buffer Result		
		::DeleteFile(Filename);			
		::DeleteFile(FilenameLog);
		::DeleteFile(FilenameSyn);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CITSFileChecker::AddRecvMsg(const std::string &msg)
{
	LockRecvMsgList();	
	m_RecvMsgList.push_back(msg);
	UnlockRecvMsgList();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CITSFileChecker::CloneRecvMsgList(std::vector<std::string> &sList, bool bClear)//取得接收訊息列表		
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
bool CITSFileChecker::RemoveUnicodeHeader(std::string &str)//移除Unicode檔頭
{
	size_t nHead=0;	
	const int nType=CheckUnicodeFile(str);		
	switch ( nType )
	{
	case nUTF32BE:	nHead = 4;	break;
	case nUTF32LE:	nHead = 4;	break;
	case nUTF16BE:	nHead = 3;	break;
	case nUTF16LE:	nHead = 3;	break;
	case nUTF8:		nHead = 3;	break;	
	}
	if ( 0 == nHead ) { return true; }

	size_t i=0;
	const size_t Cnt=str.size();		
	for ( i=nHead; i<Cnt; i++ )
	{	str[i-nHead]=str[i];	}
	str.resize(Cnt-nHead);
	return true;
}
//-------------------------------------------------------------------------------------//
int CITSFileChecker::CheckUnicodeFile(const std::string &str)//確認是否為Unicode檔案
{
	const size_t Cnt=str.size();
	if ( Cnt < 4 ) { return false; }
	
	int type = 0;	
	bool hasBOM = false;		
	const unsigned char* c = reinterpret_cast<const unsigned char *>(str.c_str());
	const unsigned int bom = static_cast<unsigned>(c[0] | (c[1] << 8) | (c[2] << 16) | (c[3] << 24));
	
	if (bom == 0xFFFE0000)                  { type = nUTF32BE; hasBOM = true; }
    else if (bom == 0x0000FEFF)             { type = nUTF32LE; hasBOM = true; }
    else if ((bom & 0xFFFF) == 0xFFFE)      { type = nUTF16BE; hasBOM = true; }
    else if ((bom & 0xFFFF) == 0xFEFF)      { type = nUTF16LE; hasBOM = true; }
    else if ((bom & 0xFFFFFF) == 0xBFBBEF)  { type = nUTF8;    hasBOM = true; }

    // RFC 4627: Section 3
    // "Since the first two characters of a JSON text will always be ASCII
    // characters [RFC0020], it is possible to determine whether an octet
    // stream is UTF-8, UTF-16 (BE or LE), or UTF-32 (BE or LE) by looking
    // at the pattern of nulls in the first four octets."
    // 00 00 00 xx  UTF-32BE
    // 00 xx 00 xx  UTF-16BE
    // xx 00 00 00  UTF-32LE
    // xx 00 xx 00  UTF-16LE
    // xx xx xx xx  UTF-8
	if ( false == hasBOM ) { return 0; }
	return type;
}
//-------------------------------------------------------------------------------------//
void CITSFileChecker::SetBackupFile(bool val)//設定是否備份檔案	
{
	m_BackupFile = val;
}
//-------------------------------------------------------------------------------------//
bool CITSFileChecker::GetBackupFile() const
{
	return m_BackupFile;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CITSFileChecker::GetBackupFolderSend() const
{
	return m_BackupFolderSend;
}
//-------------------------------------------------------------------------------------//
void CITSFileChecker::SetBackupFolderSend(LPCTSTR Folder)//備份檔案資料夾-傳送
{
	m_BackupFolderSend = Folder;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CITSFileChecker::GetBackupFolderRecv() const
{
	return m_BackupFolderRecv;
}
//-------------------------------------------------------------------------------------//
void CITSFileChecker::SetBackupFolderRecv(LPCTSTR Folder)//備份檔案資料夾-接收
{
	m_BackupFolderRecv = Folder;
}
//-------------------------------------------------------------------------------------//
bool CITSFileChecker::ExecBackupFile(bool bSend, LPCTSTR filename)//執行備份
{
	if ( GetBackupFile() == false )  { return true; }

	CString str;
	CString Key;	
	CString MainName;
	CString Filename;
	CString FileFolder;	
	CString FileFolder2;		

	int     nValue=0;
	bool    bToITS=true;
	DWORD   ChkCnt=0;	
	CString strTime;	
	SYSTEMTIME SysTime;
	const bool bUseLocalTimeName = false;
	MES_FILE_CHECKER_MODE CheckerMode=GetFileCheckMode();
	if ( true == bUseLocalTimeName )
	{	
		::GetLocalTime(&SysTime);
		strTime.Format(_T("%04d%02d%02d%02d%02d%02d_%03d"), SysTime.wYear, SysTime.wMonth, SysTime.wDay, SysTime.wHour, SysTime.wMinute, SysTime.wSecond, SysTime.wMilliseconds); 
	}
	else
	{	
		JetAPI::ExtractMainFileNameNoPath(filename, MainName); 

		if ( MES_FILE_CHECKER_IPS == CheckerMode )
		{
			CString RecvName=_T("ToSFC");
		#ifdef IPS_VERSION_V2
			RecvName=_T("ToPCS");
		#endif//IPS_VERSION_V2
			const int RecvNameLen=RecvName.GetLength();
			if ( MainName.GetLength() > RecvNameLen )
			{	
				str = MainName.Left(RecvNameLen);							
				if ( str.CompareNoCase(RecvName) == 0 )
				{	bToITS = true; }
				else
				{	bToITS = false; }
			}
		}
	}

	if ( true == bToITS )
	{
		if ( bSend )	{	nValue = 1; }
		else			{	nValue = 2; }
	}
	else
	{
		if ( bSend )	{	nValue = 2; }
		else			{	nValue = 1; }
	}

	if ( true == bSend ) 
	{
		//Key = _T("1#Send");
		Key.Format(_T("%d#Send"), nValue);
		FileFolder2=GetBackupFolderSend(); 
	}
	else 
	{
		//Key = _T("2#Recv");
		Key.Format(_T("%d#Recv"), nValue);
		FileFolder2=GetBackupFolderRecv(); 
	}

	if ( true == bUseLocalTimeName )
	{	FileFolder.Format(_T("%s\\%04d%02d%02d%02d"), FileFolder2, SysTime.wYear, SysTime.wMonth, SysTime.wDay, SysTime.wHour); }
	else
	{
		DWORD ChkCnt=0;
		CString RecvName;
		CString strYYYYMMDDhh;
		if ( MES_FILE_CHECKER_IPS == CheckerMode )
		{	GetFilenameDateTime_IPS(MainName, RecvName, strTime, ChkCnt);	}
		else
		{	GetFilenameDateTime_ITS(MainName, strTime);	}
		strYYYYMMDDhh=strTime.Left(10);
		FileFolder.Format(_T("%s\\%s"), FileFolder2, strYYYYMMDDhh);
	}
	::CreateDirectory(FileFolder, NULL);
	
	if ( true == bUseLocalTimeName )
	{
		while ( true )
		{
			Filename.Format(_T("%s\\%s#%04d_%s.JSON"), FileFolder, strTime, ChkCnt+1, Key);
			if ( ChkCnt > 1000 ) 
			{
				m_ErrorString.Format(_T("Error, ITSFileChecker Exec File Backup Fault\n(%s)"), Filename);
				return false;
			}
			if ( JetAPI::IsFileExist(Filename) == true )
			{
				ChkCnt ++;
				::Sleep(20);
				continue; 
			}
			break;
		};
	}
	else
	{	Filename.Format(_T("%s\\%s_%s.%s"), FileFolder, MainName, Key, CHECK_FILE_EXT_NAME);	}
	::CopyFile(filename, Filename, FALSE);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CITSFileChecker::GetFilenameDateTime_ITS(LPCTSTR filename, CString &DateTime)
{
	if ( NULL == filename ) { return false; }
	const int DateTimeLen=14;	
	CString Filename(filename);
	DateTime = Filename.Left(DateTimeLen);
	if ( DateTime.GetLength() != DateTimeLen )
	{	return false; }	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CITSFileChecker::GetFilenameDateTime_IPS(LPCTSTR filename, CString &RecvName, CString &DateTime, DWORD &ChkCnt)
{
	if ( NULL == filename ) { return false; }	
	const int DateTimeLen=14;	
	CString Filename(filename);	

	//ToSFC_YYYYMMDDhhmmss_Ack	
	const int nLeft=Filename.Find('_');
	if ( nLeft < 0 ) 
	{	return false;	}
	RecvName = Filename.Mid(0, nLeft);
	DateTime = Filename.Mid(nLeft+1, DateTimeLen);	
	if ( DateTime.GetLength() != DateTimeLen )
	{	return false; }

	//Get SN
	const int SNLen=4;	
	const int nLeft2=Filename.Find('_', nLeft+1);
	if ( nLeft2 < 0 ) 
	{	return false;	}
	CString strSN=Filename.Mid(nLeft2+1, SNLen);	
	if ( strSN.GetLength() != SNLen )
	{	return false; }
	ChkCnt=::_ttoi(strSN);
	return true;
}
//-------------------------------------------------------------------------------------//