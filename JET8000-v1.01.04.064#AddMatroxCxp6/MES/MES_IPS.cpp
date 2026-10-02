// MES_IPS.cpp: implementation of the CMES_IPS class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "MES_IPS.h"
#include "resource.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
#ifndef MES_DISABLE
#ifndef IPS_DISABLE
//IPS執行
//-------------------------------------------------------------------------------------//
#define IPS_REMOTE_CONTROL_ERROR_SUCCESS                    0
#define IPS_REMOTE_CONTROL_ERROR_FAIL                       1
#define IPS_REMOTE_CONTROL_ERROR_SYSTEM_NOT_READY           2
#define IPS_REMOTE_CONTROL_ERROR_NO_SUPPORT_FUNC            3
#define IPS_REMOTE_CONTROL_ERROR_PROJECT_CLOSED             4
#define IPS_REMOTE_CONTROL_ERROR_ON_RUNNING                 5
#define IPS_REMOTE_CONTROL_ERROR_ON_STOPPING                6
#define IPS_REMOTE_CONTROL_ERROR_PROJECT_CREATE             7
#define IPS_REMOTE_CONTROL_ERROR_PROJECT_OPEN               8
#define IPS_REMOTE_CONTROL_ERROR_ONLINE                     9
#define IPS_REMOTE_CONTROL_ERROR_OFFLINE                   10
#define IPS_REMOTE_CONTROL_ERROR_JSON_DATA_EXCEPTION       11
#define IPS_REMOTE_CONTROL_ERROR_PROJECT_NOT_ALLOW         12
#define IPS_REMOTE_CONTROL_ERROR_REMOTE_CTRL_DISABLE       13
//-------------------------------------------------------------------------------------//
#define IPS_BOARD_MAPPING_TEST                              0
#define IPS_BOARD_MAPPING_SKIP                              1
//-------------------------------------------------------------------------------------//
unsigned int IPSProcThreadID = 0;//IPS執行執行緒編號
HANDLE IPSProcThreadHandle = NULL;//IPS執行執行緒處理碼
HANDLE IPSProcThreadEvent  = NULL;//IPS執行執行緒事件
unsigned int __stdcall IPSProcThreadFn(void *pParam);
//-------------------------------------------------------------------------------------//
unsigned int __stdcall IPSProcThreadFn(void *pParam)
{
#ifndef IPS_DISABLE
	bool IsOK = true;	
	DWORD  SleepTime = 10;//60 sec exec
	//const size_t ThreadIdx = (size_t)pParam;
	const size_t ThreadIdx=0;
	CMES_IPS *Ptr=(CMES_IPS*)(pParam);
	THREAD_STATE_MODE   ThreadState = THREAD_STATE_NONE;
	THREAD_COMMAND_MODE ThreadCmd = THREAD_COMMAND_TO_NONE;		
	
	double Time = 0;
	LARGE_INTEGER          nStartTime;
	LARGE_INTEGER          nEndTime;
	TCHAR strBuffer[256]=_T("");

	while ( true )
	{	
		ThreadCmd = Ptr->GetIPSProcThreadCmd();
		ThreadState = Ptr->GetIPSProcThreadState();
		if ( THREAD_COMMAND_TO_EXIT == ThreadCmd )
		{	break;	}
		if ( THREAD_COMMAND_TO_IDLE == ThreadCmd ) 
		{ 			
			Ptr->SetIPSProcThreadState(THREAD_STATE_IDLE);		
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
			Ptr->SetIPSProcThreadState(THREAD_STATE_RUNNING);		
			QueryPerformanceCounter(&nStartTime);
			IsOK = Ptr->ExecIPSProcFn();
			QueryPerformanceCounter(&nEndTime);			
			Time = (nEndTime.QuadPart - nStartTime.QuadPart) * 1000.0/AOIDataCollect.m_SystemFreq.QuadPart;
			const TSystemParameter &SystemParam = AOIDataCollect.GetSystemParameter();
			if ( FN_ENABLE == SystemParam.m_SaveITSProcThreadLog ) 
			{
				_stprintf(strBuffer, _T("IPSProcThreadFn[%d]: %.2f ms"), ThreadIdx+1, Time);
				AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, strBuffer);			
			}
		}
		::Sleep(SleepTime);
	};
	Ptr->SetIPSProcThreadState(THREAD_STATE_NONE);
	if ( NULL != IPSProcThreadEvent )
	{	::SetEvent(IPSProcThreadEvent); }
#endif//IPS_DISABLE
	return 0;
}
//-------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMES_IPS::CMES_IPS():CMES_Imp()
{	
	PreInitIPS();
	InitialIPS();
}
//-------------------------------------------------------------------------------------//
CMES_IPS::~CMES_IPS()
{	
	DeleteIPSProcThread();
}
//-------------------------------------------------------------------------------------//
void CMES_IPS::PreInitIPS()
{	
	m_MsgPPID = 0;	
	m_RemoteCtrlOn = false;
	m_EqpOnlineMode = 0;
	m_IPSProcThreadState = THREAD_STATE_NONE;
	m_IPSProcThreadCmd = THREAD_COMMAND_TO_NONE;
	CITSFileChecker &FileChecker=GetIPSLinker().GetITSFileChecker();	
	FileChecker.SetFileCheckMode(MES_FILE_CHECKER_IPS);	
	FileChecker.SetCreateSyncFileDelayTime(10);
}
//-------------------------------------------------------------------------------------//
void CMES_IPS::InitialIPS()
{
}
//-------------------------------------------------------------------------------------//
void CMES_IPS::CloneIPS(const CMES_IPS &other)
{
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::CheckProjectPtr(CAOIProject *ProjectPtr)//確認專案指標
{
	if ( NULL == ProjectPtr )
	{
		m_ErrorString = _T("Error, No Project Ptr");
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::GetRemoteCtrlOn() const
{
	return m_RemoteCtrlOn;
}
//-------------------------------------------------------------------------------------//
void CMES_IPS::SetRemoteCtrlOn(bool val)
{
	m_RemoteCtrlOn = val;
	SetMesRemotCtrlOn(val);
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::CheckRemoteCtrlOn()
{
	if ( GetRemoteCtrlOn()==false )
	{
		m_ErrorString=_T("Error, Remote Ctrl Not Enabled");
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::CheckEqpCtrlState_Remote()
{
	if ( GetMesEqpCtrlStateMode() != MES_EQP_CTRL_STATE_REMOTE )
	{	
		m_ErrorString=_T("Error, EQP Ctrl State Not Remoted");
		return false; 
	}
	return true;
}
//-------------------------------------------------------------------------------------//
int CMES_IPS::GetEqpOnlineMode() const
{	
	return m_EqpOnlineMode;
}
//-------------------------------------------------------------------------------------//
void CMES_IPS::SetEqpOnlineMode(int Mode)
{	
	m_EqpOnlineMode = Mode;

	MES_EQP_CTRL_STATE_MODE StateMode;
	switch ( Mode )
	{
	case IPS_COMM_STATUS_ONLINE:		StateMode=MES_EQP_CTRL_STATE_REMOTE;	break;
	case IPS_COMM_STATUS_LOCAL_ONLINE:	StateMode=MES_EQP_CTRL_STATE_LOCAL;		break;
	default:
	case IPS_COMM_STATUS_OFFLINE:		StateMode=MES_EQP_CTRL_STATE_OFFLINE;	break;
	}
	SetMesEqpCtrlStateMode(StateMode);
}
//-------------------------------------------------------------------------------------//
void CMES_IPS::LockIPSProc()//鎖住IPS執行緒同步化
{
	CMES_Imp::LockImpProc();	
}
//-------------------------------------------------------------------------------------//
void CMES_IPS::UnlockIPSProc()//釋放IPS執行緒同步化
{
	CMES_Imp::UnlockImpProc();	
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::ExecIPSProcFn()//執行IPS執行執行緒
{		
#ifndef IPS_DISABLE
	const bool bThread = true;
	THREAD_COMMAND_MODE    ThreadCmd;	
	while ( true )
	{
		ThreadCmd = GetIPSProcThreadCmd();
		if ( AOIDataCollect.GetIsSystemReleased() == true ) { return true; }
		if ( THREAD_COMMAND_TO_IDLE == ThreadCmd ) { return true; }
		if ( THREAD_COMMAND_TO_EXIT == ThreadCmd ) { return true; }		
		if ( THREAD_COMMAND_TO_NONE == ThreadCmd ) { return true; }			

		if ( RecvNewIPSRecvNodeList() == false )
		{	
			::Sleep(10);
			continue; 
		}
		if ( ProcesIPSRecvNodeList(bThread) == false ) 
		{	
			::Sleep(10);
			continue; 
		}
		::Sleep(10);
	}
#endif//IPS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::CreateIPSProcThread()//建立IPS執行執行緒
{
#ifndef IPS_DISABLE
	if ( DeleteIPSProcThread() == false ) { return false; }	
	m_IPSProcThreadCmd = THREAD_COMMAND_TO_IDLE;	

	IPSProcThreadHandle = (HANDLE)::_beginthreadex(NULL, NULL, &IPSProcThreadFn, (void*)this, NULL, &IPSProcThreadID);	
	if ( NULL == IPSProcThreadHandle )
	{	
		m_IPSProcThreadState = THREAD_STATE_NONE;
		m_ErrorString = _T("Eorror, Create Thread Fault (IPSProcThreadHandle == NULL)");		
		AOIExceptionCodeCtrl.SetAOIExceptionCode_Thread(AOI_EXCEPTION_THREAD_CREATE, m_ErrorString);
		return false;
	}		
	IPSProcThreadEvent = JetAPI::CreateEvent(NULL, TRUE, TRUE, _T("IPS Proc Event"));		
	::SetThreadPriority(IPSProcThreadHandle, THREAD_PRIORITY_BELOW_NORMAL); 
	//::SetThreadAffinityMask(IPSProcThreadHandle, 0x03);//保留2個		
	StartIPSProcThread(false);
#else
	IPSProcThreadHandle = NULL;
	IPSProcThreadEvent = NULL;
#endif//IPS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::DeleteIPSProcThread()//刪除IPS執行執行緒
{
#ifndef IPS_DISABLE
	DWORD WaitTime = 10000;//1 sec	
	if ( NULL == IPSProcThreadHandle ) { return true; }		
	SetIPSProcThreadCmd(THREAD_COMMAND_TO_EXIT);	
	DWORD Res = ::WaitForSingleObject(IPSProcThreadHandle, WaitTime);
	::CloseHandle(IPSProcThreadHandle); 
	IPSProcThreadHandle = NULL;
	if ( NULL != IPSProcThreadEvent )
	{	
		::CloseHandle(IPSProcThreadEvent); 
		IPSProcThreadEvent=NULL; 
	}	
#endif//IPS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::StopIPSProcThread(bool WaitOn)//開始IPS執行執行緒
{
#ifndef IPS_DISABLE
	size_t i=0;
	if ( NULL == IPSProcThreadHandle ) { return true; }
	if ( NULL != IPSProcThreadEvent )
	{	::ResetEvent(IPSProcThreadEvent); }
	SetIPSProcThreadCmd(THREAD_COMMAND_TO_IDLE);	

	if ( WaitOn == true ) 
	{	
		const size_t MaxCount = 100;
		const size_t SleepTime = 10;
		for ( i=0; i<MaxCount; i++ )
		{	
			if ( THREAD_STATE_NONE == m_IPSProcThreadState )
			{	break; }
			if ( THREAD_STATE_IDLE == m_IPSProcThreadState )
			{	break; }
			//if ( THREAD_STATE_FINISH == m_IPSProcThreadState )//有可能後面才完成, 所以要加上完成確認
			//{	break; }
			::Sleep(SleepTime);
		}
		if ( i == MaxCount )
		{
			m_ErrorString.Format(_T("Error, wait for StopIPSProcThread too long"));			
			return false;
		}		
	}
#endif//IPS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::StartIPSProcThread(bool WaitOn)//開始IPS執行執行緒
{
#ifndef IPS_DISABLE
	size_t i=0;
	if ( NULL == IPSProcThreadHandle ) { return true; }
	if ( NULL != IPSProcThreadEvent )
	{	::ResetEvent(IPSProcThreadEvent); }
	if ( THREAD_STATE_FINISH == m_IPSProcThreadState )
	{  SetIPSProcThreadState(THREAD_STATE_IDLE); }
	SetIPSProcThreadCmd(THREAD_COMMAND_TO_RUN);	

	if ( false == WaitOn )
	{	return true; }
	if ( WaitForIPSProcThreadStart() == false )
	{	return false; }	
#endif//IPS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::WaitForIPSProcThreadIdle()//等待IPS執行執行緒停止
{
#ifndef IPS_DISABLE
	size_t i=0;	
	DWORD Res = 0;		
	const DWORD SleepTime = 100;	
	const size_t MaxCounts = 200000;
	for ( i=0; i<MaxCounts; i++ )
	{
		if ( AOIDataCollect.GetIsSystemReleased() == true ) { return true; }				
		//if ( AOIDataCollect.GetIsSystemException() == true ) { return true; }
		if ( THREAD_STATE_NONE == m_IPSProcThreadState ) { return true; }
		if ( THREAD_STATE_IDLE == m_IPSProcThreadState ) { return true; }
		if ( SleepTime > 0 )
		{	::Sleep(SleepTime); }
	}
	if ( i == MaxCounts )
	{
		m_ErrorString.Format(_T("Error, Wait for WaitForIPSProcThreadIdle too long"));		
		AOIExceptionCodeCtrl.SetAOIExceptionCode_Thread(AOI_EXCEPTION_THREAD_WAIT_FOR_IDLE, m_ErrorString);
		return false;
	}	
#endif//IPS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::WaitForIPSProcThreadStop()//等待IPS執行執行緒停止
{
#ifndef IPS_DISABLE
	size_t i=0;	
	DWORD Res = 0;		
	const DWORD SleepTime = 100;	
	const size_t MaxCounts = 200000;
	for ( i=0; i<MaxCounts; i++ )
	{
		if ( AOIDataCollect.GetIsSystemReleased() == true ) { return true; }				
		//if ( AOIDataCollect.GetIsSystemException() == true ) { return true; }
		if ( THREAD_STATE_RUNNING != m_IPSProcThreadState ) { return true; }		
		if ( SleepTime > 0 )
		{	::Sleep(SleepTime); }
	}
	if ( i == MaxCounts )
	{
		m_ErrorString.Format(_T("Error, Wait for WaitForIPSProcThreadStop too long"));
		AOIExceptionCodeCtrl.SetAOIExceptionCode_Thread(AOI_EXCEPTION_THREAD_WAIT_FOR_STOP, m_ErrorString);
		return false;
	}	
#endif//IPS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::WaitForIPSProcThreadStart()//等待IPS執行執行緒開始
{
#ifndef IPS_DISABLE		
	size_t i=0;
	const size_t MaxCount = 100;
	const size_t SleepTime = 10;
	for ( i=0; i<MaxCount; i++ )
	{
		if ( THREAD_COMMAND_TO_RUN != m_IPSProcThreadCmd )//切入下一個階段
		{	break; }
		if ( THREAD_STATE_IDLE != m_IPSProcThreadState )
		{	break; }
		::Sleep(SleepTime);
	}
	if ( i == MaxCount )
	{
		m_ErrorString.Format(_T("Error, wait for StartIPSProcThread too long"));			
		AOIExceptionCodeCtrl.SetAOIExceptionCode_Thread(AOI_EXCEPTION_THREAD_WAIT_FOR_START, m_ErrorString);
		return false;
	}
#endif//IPS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::WaitForIPSProcThreadFinish()//等待IPS執行執行緒結束
{
#ifndef IPS_DISABLE
	size_t i=0;	
	DWORD Res = 0;		
	const DWORD SleepTime = 100;	
	const size_t MaxCounts = 200000;

	if ( NULL == IPSProcThreadEvent ) { return true; }
	for ( i=0; i<MaxCounts; i++ )
	{
		if ( AOIDataCollect.GetIsSystemReleased() == true ) { return true; }				
		if ( AOIDataCollect.GetIsSystemException() == true ) { return true; }
		if ( THREAD_COMMAND_TO_EXIT == m_IPSProcThreadCmd ) { return true; }		
		if ( THREAD_COMMAND_TO_IDLE == m_IPSProcThreadCmd )
		{	break; }

		Res = ::WaitForSingleObject(IPSProcThreadEvent, SleepTime);
		if ( Res != WAIT_TIMEOUT ) 
		{
			if ( THREAD_STATE_RUNNING == m_IPSProcThreadState )
			{
				if ( SleepTime > 0 ) { ::Sleep(SleepTime); }
				continue; 
			}
			break; 
		}
	}
	if ( i == MaxCounts )
	{
		m_ErrorString.Format(_T("Error, Wait for WaitForIPSProcThreadFinish too long"));
		AOIExceptionCodeCtrl.SetAOIExceptionCode_Thread(AOI_EXCEPTION_THREAD_WAIT_FOR_FINISH, m_ErrorString);
		return false;
	}	
#endif//IPS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::CheckIPSProcThreadState(THREAD_STATE_MODE State)//確認IPS執行執行緒狀態
{
	if ( NULL == IPSProcThreadHandle ) { return true; }
	if ( m_IPSProcThreadState != State )
	{
		m_ErrorString.Format(_T("Error, m_IPSProcThreadState is Exception"));
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CMES_IPS::SetIPSProcThreadState(THREAD_STATE_MODE State)//設定IPS執行執行緒狀態
{
	if ( m_IPSProcThreadState == State ) { return; }
	LockIPSProc();
	m_IPSProcThreadState = State;
	UnlockIPSProc();
}
//-------------------------------------------------------------------------------------//
THREAD_STATE_MODE CMES_IPS::GetIPSProcThreadState()//取得IPS執行執行緒狀態
{
	return m_IPSProcThreadState;
}
//-------------------------------------------------------------------------------------//
void CMES_IPS::SetIPSProcThreadCmd(THREAD_COMMAND_MODE Cmd)//設定IPS執行執行緒命令
{
	if ( m_IPSProcThreadCmd == Cmd ) { return; }
	LockIPSProc();
	m_IPSProcThreadCmd = Cmd;
	UnlockIPSProc();
}
//-------------------------------------------------------------------------------------//
THREAD_COMMAND_MODE CMES_IPS::GetIPSProcThreadCmd()//取得IPS執行執行緒命令
{
	return m_IPSProcThreadCmd;
}
//-------------------------------------------------------------------------------------//
CITSLinker& CMES_IPS::GetIPSLinker()//取得IPS連線參考
{
	return m_MESLinker;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::ConnectIPSLinker()//連線到MES連結軟體
{
#ifndef IPS_DISABLE
	CString ITSFilename;
	CString Folder = AOIDataCollect.GetAOIDirectory();
	const TSystemParameter &SysParam = AOIDataCollect.GetSystemParameter();
	ITS_COMMUNICATION_MODE CommMode = SysParam.m_ITSCommunicationMode;
	ITSFilename = SysParam.m_ITSFilename;
	if ( ITSFilename.GetLength() > 0 )
	{
		HINSTANCE hinstITS=::ShellExecute(NULL, _T("open"), ITSFilename, NULL, NULL, SW_SHOW);			
		::Sleep(1000);
	}
	bool bCommRet=false;
	CITSLinker &ITSLinker=GetIPSLinker();
	SetFreezeMESFuncMode(false);	

	if ( ITS_COMMUNICATION_RABBIT_MQ == CommMode ) 
	{
		char strIP[64] = "localhost";
		char strUserName[64] = "aoiuser";
		char strPassword[64] = "123456";
		char strRecvName[64] = "ITS2AOI";
		char strSendName[64] = "AOI2ITS";
		const int nPort = SysParam.m_RabbitMQServerPort;//5672
		JetAPI::TCHAR2char(SysParam.m_RabbitMQServerAddress, strIP, 64);
		JetAPI::TCHAR2char(SysParam.m_RabbitMQUserName, strUserName, 64);
		JetAPI::TCHAR2char(SysParam.m_RabbitMQPassword, strPassword, 64);
		JetAPI::TCHAR2char(SysParam.m_ITSRabbitMQRecvQueueName, strRecvName, 64);
		JetAPI::TCHAR2char(SysParam.m_ITSRabbitMQSendQueueName, strSendName, 64);	
		bCommRet = ITSLinker.ConnectITS_Rabbit(strIP, nPort, strUserName, strPassword, strRecvName, strSendName);	
	}
	else if ( ITS_COMMUNICATION_FILE_CHECKER == CommMode )
	{	
		CString strFolderTemp;
		CString strBackupFolderSend;
		CString strBackupFolderRecv;
		bool    bBackup=SysParam.m_ITSFileBackupEnabled;	
		bool    bUseSync=SysParam.m_ITSFileUseSyncFileEnabled;
		CString strFolderSend=SysParam.m_ITSFileFolderSend;
		CString strFolderRecv=SysParam.m_ITSFileFolderRecv;
		CString FolderTemp=AOIDataCollect.GetAOITempDirectory();
		CString strLogFolder=AOIDataCollect.GetAOILogDirectory();		
		strFolderTemp.Format(_T("%s\\%s"), FolderTemp, _T("ITSFileTemp"));
		strBackupFolderSend.Format(_T("%s\\ITSFileBackup"), strLogFolder);
		strBackupFolderRecv.Format(_T("%s\\ITSFileBackup"), strLogFolder);		
		::CreateDirectory(strFolderTemp, NULL); ::Sleep(0);
		::CreateDirectory(strBackupFolderSend, NULL); ::Sleep(0);
		::CreateDirectory(strBackupFolderRecv, NULL); ::Sleep(0);
		bCommRet = ITSLinker.ConnectITS_File(strFolderSend, strFolderRecv, strFolderTemp, bBackup, bUseSync, strBackupFolderSend, strBackupFolderRecv);	
	}
	else
	{
		char strIP[64] = "127.0.0.1";
		const int nPort = SysParam.m_ITSSocketIPPort;
		JetAPI::TCHAR2char(SysParam.m_ITSSocketIPAddress, strIP, 64);
		bCommRet = ITSLinker.ConnectITS_Socket(strIP, nPort);	
	}
	if ( false == bCommRet )
	{	
		m_ErrorString = ITSLinker.GetErrorString();
		return false;
	}	
#endif//IPS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::DisconnectIPSLinker()//停止連線到IPS連結軟體
{
	SetRemoteCtrlOn(false);
	CITSLinker &ITSLinker=GetIPSLinker();
	ITSLinker.SetITSThreadStop(true);
	ITSLinker.DisconnectITS();	
	return true;
}
//-------------------------------------------------------------------------------------//
int CMES_IPS::GetIPSStatusID_Ack(int StatusID)//取得IPS的回應訊息
{
	int AckID=0;	
	return AckID;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::CheckIPSStatusID_Ack(const TIPSCommNode &Node)//確認IPS的回應訊息
{
	bool bAckID = false;
	std::wstring wsSender=Node.wsSenderStr;
	JetAPI::wstring2upper(wsSender);
	if ( wsSender==AOI3D_APP_NAME_W)//L"JET8000" 
	{	bAckID = false; }
	else
	{	bAckID = true; }
	return bAckID;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::CheckIPSStatusCode_RemoteControl(const TIPSCommNode &Node)//確認IPS的遠端控制
{	
	bool bRemoteControl=false;
	switch ( Node.nStatusCode )
	{	
	case IPS_COMM_REMOTE_ON:		
	case IPS_COMM_REMOTE_OFF:
	case IPS_COMM_REMOTE_START:
	case IPS_COMM_REMOTE_STOP:
	case IPS_COMM_REMOTE_PAUSE:
	case IPS_COMM_REMOTE_RESUME:
	case IPS_COMM_REMOTE_ABORT:
	case IPS_COMM_REMOTE_BYPASS:

	case IPS_COMM_REMOTE_PROJECT_UPLOAD:
	case IPS_COMM_REMOTE_UPLOAD_FINISHED:
	case IPS_COMM_REMOTE_PROJECT_DOWNLOAD:
	case IPS_COMM_REMOTE_DOWNLOAD_FINISHED:
	case IPS_COMM_REMOTE_PROJECT_DELETE:
	case IPS_COMM_REMOTE_DELETE_FINISHED:

	case IPS_COMM_REMOTE_PARAM_UPLOAD:
	case IPS_COMM_REMOTE_PARAM_UPLOAD_FINISHED:
	case IPS_COMM_REMOTE_PARAM_DOWNLOAD:
	case IPS_COMM_REMOTE_PARAM_DOWNLOAD_FINISHED:

	case IPS_COMM_REMOTE_SET_LOT_NUMBER:
		bRemoteControl = true;
		break;
	default:
		bRemoteControl = false;
		break;
	}
	return bRemoteControl;
}
//-------------------------------------------------------------------------------------//
size_t CMES_IPS::GetIPSRecvNodeCount()
{
	return m_IPSRecvNodeList.size();
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::RecvNewIPSRecvNodeList()//接收新的IPS訊息列表
{	
#ifndef IPS_DISABLE
	size_t i=0;
	std::vector<std::string> RecvNodeList;	

	LockIPSProc();
	CITSLinker &ITSLinker=GetIPSLinker();
	ITSLinker.CloneITSRecvMsgList(RecvNodeList, true);
	const size_t RecvNodeCount = RecvNodeList.size();	
	for ( i=0; i<RecvNodeCount; i++ )
	{	
		if ( AddIPSRecvNode(RecvNodeList[i]) == false ) 
		{
			UnlockIPSProc();
			m_ErrorString = _T("Error, Receive IPS Node Fault");
			return false;
		}				
	}			
	UnlockIPSProc();
#endif//IPS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::AddIPSRecvNode(const std::string &sBuff)
{
#ifndef IPS_DISABLE
	TIPSCommNode CommNode;
	if ( DecoderIPSPacket(sBuff, CommNode) == false )
	{	return false; }
	m_IPSRecvNodeList.push_back(CommNode);	
#endif//IPS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::DecoderIPSPacket(const std::string &sBuff, TIPSCommNode &sNode)
{
	int bRet=false;
	std::wstring wsBuf;
	rapidjson::CGMItr itr;
	rapidjson::CGMItr Obj_itr;
	rapidjson::WDocument Doc;
	rapidjson::CJsonCtrl JSonCtrl;

	if ( JetAPI::char2wstring(sBuff.c_str(), wsBuf, CP_UTF8) == 0 ) 
	{	return false; }

	//initial
	Doc.SetObject();
	JSonCtrl.Set(&Doc);			
	if ( JSonCtrl.SetBuffer(wsBuf, Doc) == false ) 
	{	return false; }
	
	//"Header": {
    //"Version": "2.0.0",
    //"Sender": "JET8000",
    //"StatusCode": 300,
    //"Status": "End",
    //"ResultCode": 0,
    //"Result": "OK"
	//},		
	
	sNode = TIPSCommNode();	
	if ( JSonCtrl.FindMember(L"Header", Obj_itr) == false ) 
	{	return false; }
	if ( Obj_itr->value.IsObject() == false )
	{	return false; } 

	if ( JSonCtrl.FindMember(Obj_itr, L"Version", itr) == false ) 
	{	return false; }				
	if ( itr->value.IsString() == false ) 
	{	return false; }
	sNode.wsVersionStr = itr->value.GetString();	

	if ( JSonCtrl.FindMember(Obj_itr, L"Sender", itr) == false ) 
	{	return false; }				
	if ( itr->value.IsString() == false ) 
	{	return false; }
	sNode.wsSenderStr = itr->value.GetString();	

	if ( JSonCtrl.FindMember(Obj_itr, L"StatusCode", itr) == false ) 
	{	return false; }				
	if ( itr->value.IsInt() == false ) 
	{	return false; }
	sNode.nStatusCode = itr->value.GetInt();

	if ( JSonCtrl.FindMember(Obj_itr, L"ResultCode", itr) == false ) 
	{	return false; }				
	if ( itr->value.IsInt() == false ) 
	{	return false; }
	sNode.nErrorCode = itr->value.GetInt();

	if ( JSonCtrl.FindMember(Obj_itr, L"Result", itr) == false ) 
	{	return false; }				
	if ( itr->value.IsString() == false ) 
	{	return false; }
	sNode.wsErrorCode = itr->value.GetString();	

	if ( JSonCtrl.FindMember(L"Filename", itr) == true ) 
	{
		if ( itr->value.IsString() == true ) 
		{	sNode.wsFilename = itr->value.GetString();		}
	}

	if ( 0 != sNode.nErrorCode )
	{
		rapidjson::CGMItr Data_itr;
		if ( JSonCtrl.FindMember(L"Data", Data_itr) == true )
		{
			if ( Data_itr->value.IsObject() == true )
			{
				if ( JSonCtrl.FindMember(Data_itr, L"Message", itr) == true )
				{
					if ( itr->value.IsString() == true )
					{	sNode.wsErrorCode += std::wstring(L"(")+itr->value.GetString()+std::wstring(L")");	}
				}
			}			
		}
	}

	sNode.sRawData = sBuff;	
	sNode.nPPID = sNode.nStatusCode;
	sNode.nStatus = sNode.nStatusCode;	
	sNode.nAck = CheckIPSStatusID_Ack(sNode);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::GetIPSRecvNode(size_t index, bool bCheck, TIPSCommNode &sNode)
{
	if ( true == bCheck )
	{
		const size_t Count = m_IPSRecvNodeList.size();
		if ( index >= Count ) 
		{	return false; }
	}
	sNode = m_IPSRecvNodeList[index];
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::ProcesIPSRecvNodeList(bool bThread)//處理收到IPS的訊息列表
{
#ifndef IPS_DISABLE
	size_t       i=0;
	bool         bBreak=false;
	bool         bRemove = false;
	std::vector<size_t> EraseIndexList;	

	bBreak=false;
	LockIPSProc();
	const size_t RecvCount = m_IPSRecvNodeList.size();
	for ( i=0; i<RecvCount; i++ )
	{
		if ( ProcesIPSRecvNode(bThread, m_IPSRecvNodeList[i], bBreak, bRemove) == false )
		{	continue; }
		if ( true == bRemove )
		{	EraseIndexList.push_back(i);	}
		if ( true == bBreak )
		{	break; }
	}	
	
	size_t EraseIndex=0;
	const size_t EraseCount = EraseIndexList.size();
	for ( i=0; i<EraseCount; i++ )//for ( int i=3; i--; )
	{
		EraseIndex = EraseIndexList[EraseCount-i-1];
		m_IPSRecvNodeList.erase(m_IPSRecvNodeList.begin()+EraseIndex);
	}
	UnlockIPSProc();
#endif//IPS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::ProcesIPSRecvNode(bool bThread, TIPSCommNode &sNode, bool &bBreak, bool &bRemove)//處理收到IPS的訊息
{	
#ifndef IPS_DISABLE	
	sNode.nChkCount ++;
	//確認是否需要回傳
	if ( 0 == sNode.nAck ) { return true; }
	
	const int StatusRes = sNode.nStatusCode;	
	if ( IPS_COMM_NONE == StatusRes ) 
	{	return true; }	
	
	bool IsOK=true;
	SetFreezeMESFuncMode(true);	
	::Sleep(50);//切換執行緒	
	if ( CheckIPSStatusCode_RemoteControl(sNode) == true )
	{	IsOK = ExecIPSComm_MesRemoteControl(bThread, sNode, bBreak, bRemove);	}	
	else
	{
		switch ( sNode.nStatus )
		{
		case IPS_COMM_GEN_GET_STATUS:
			IsOK = ExecIPSComm_MesGetStatus(bThread, sNode, bBreak, bRemove);
			break;
		case IPS_COMM_REMOTE_PROJECT_LOAD:
			IsOK = ExecIPSComm_MesOpenProject(bThread, sNode, bBreak, bRemove);
			break;
		case IPS_COMM_REMOTE_PROJECT_LIST:
			IsOK = ExecIPSComm_MesListProject(bThread, sNode, bBreak, bRemove);
			break;
		case IPS_COMM_PCS_CONTROL:
			break;
		case IPS_COMM_PCS_SHOW_MESSAGE:
			IsOK = ExecIPSComm_PcsShowMessage(bThread, sNode, bBreak, bRemove);
			break;
		case IPS_COMM_PCS_UPLOAD_FINISHED:			
		case IPS_COMM_PCS_DOWNLOAD_FINISHED:
			IsOK = ExecIPSComm_PcsFinishedMsg(bThread, sNode, bBreak, bRemove);
			break;
		}
			 
	}
	SetFreezeMESFuncMode(false);	
	return IsOK;
#endif//IPS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::SendToIPSNode(const char *strBuf, bool bAck, int nPPID, DWORD AckTime, bool bChkFrz, LPCTSTR filename)//送資料給IPS
{
	TIPSCommNode RecvNode;
	bool bSucc=SendToIPSNode(strBuf, bAck, nPPID, AckTime, RecvNode, NULL, bChkFrz, filename);
	return bSucc;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::SendToIPSNode(const char *strBuf, bool bAck, int nPPID, DWORD AckTime, TIPSCommNode &RecvNode, CAOIProject *ProjectPtr, bool bChkFrz, LPCTSTR filename)//送資料給IPS
{
#ifndef IPS_DISABLE
	CString str;
	CString Err;
	CITSLinker &ITSLinker=GetIPSLinker();
	if ( true == bChkFrz )
	{
		//if ( GetFreezeMESFuncMode() == true ) { return true; }
		if ( WaitForFreezeMESFuncModeDone() == false )
		{	return false; }
	}

	if ( ITSLinker.GetITSConnected() == false )
	{
		str = _T("Error, IPS Client Not connected");
		m_ErrorString = GetMESMultiLanguage(str);
		return false;
	}

	m_IPSSendBuffer = strBuf;
	if ( ITSLinker.AddITSSendMsg(m_IPSSendBuffer, filename) == false )
	{
		str = _T("Error, IPS Client Send Bytes Fault");
		m_ErrorString = GetMESMultiLanguage(str);
		return false;
	}

	if ( true == bAck )
	{
		if ( WaitForIPSAckFile(nPPID, AckTime, RecvNode) == false )
		{	return false; }
	}

	/*
	CString PathName;		
	CITSFileChecker &FileChecker=ITSLinker.GetITSFileChecker();
	PathName=FileChecker.GetFilenameSend();	
	if ( true == bAck  ) 
	{		
		
		if ( StopIPSProcThread(true) == false )
		{	return false; }

		CString Filename;
		CString ErrorString;		
		JetAPI::ExtractFileNameNoPath(PathName, Filename);
		bool bIsOK = WaitForIPSResponse(nPPID, AckTime, Filename, RecvNode);
		if ( bIsOK == false ) 
		{	ErrorString = GetErrorString(); }
		StartIPSProcThread(false);
		if ( bIsOK == false ) 
		{	
			SetErrorString(ErrorString); 
			return false;
		}		
	}
	*/
#endif//IPS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::WaitForIPSAckFile(int nPPID, DWORD AckTime, TIPSCommNode &RecvNode)//等待IPS回傳資料
{	//return true;
#ifndef IPS_DISABLE
	CString PathName;		
	CITSLinker &ITSLinker=GetIPSLinker();
	CITSFileChecker &FileChecker=ITSLinker.GetITSFileChecker();
	PathName=FileChecker.GetFilenameSend();				
	if ( StopIPSProcThread(true) == false )
	{	return false; }

	CString Filename;
	CString ErrorString;		
	JetAPI::ExtractFileNameNoPath(PathName, Filename);
	bool bIsOK = WaitForIPSResponse(nPPID, AckTime, Filename, RecvNode);
	if ( bIsOK == false ) 
	{	ErrorString = GetErrorString(); }
	StartIPSProcThread(false);
	if ( bIsOK == false ) 
	{	
		SetErrorString(ErrorString); 
		return false;
	}
#endif//IPS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::WaitForIPSResponse(int nPPID, DWORD AckTime, LPCTSTR Filename, TIPSCommNode &RecvNode)//等待IPS回傳資料
{	
#ifdef IPS_DEBUG
	return true;
#endif//IPS_DEBUG
#ifndef IPS_DISABLE
	CString str;
	CString Err;
	size_t i=0;
	bool   bRet=false;
	bool   bFound=false;			
	DWORD  ChkCount=0;
	DWORD  SleepTime = 10;	
	size_t LastRecvCount=0;
	size_t CurRecvNodeCount=0;			
	std::string  RecvString;
	std::wstring RecvWString;
	CString FileDateTime=ExtractFilenameDateTime(Filename);
	//std::vector<std::string> RecvNodeList;
#ifdef _DEBUG
	DWORD MaxChkCount = MAX(AckTime/SleepTime, 10)*500;
#else
	DWORD MaxChkCount = MAX(AckTime/SleepTime, 10)*50;
#endif//_DEBUG
	//MaxChkCount = 1000000;	
	DWORD TimeoutMS=GetMesCommTimeoutMS();
	if ( 0 != TimeoutMS )
	{
		MaxChkCount = MAX(TimeoutMS, AckTime+1000);
		MaxChkCount /= SleepTime;		
	}	
	LastRecvCount = m_IPSRecvNodeList.size();
	while ( true ) 
	{	
		if ( RecvNewIPSRecvNodeList() == false ) 
		{	
			Err = GetErrorString();
			str = _T("Error, RecvNewIPSRecvNodeList Fault");
			str = GetMESMultiLanguage(str);
			m_ErrorString.Format(_T("%s\n%s"), str, Err);
			return false; 
		}

		LockIPSProc();
		CurRecvNodeCount = m_IPSRecvNodeList.size();
		for ( i=LastRecvCount; i<CurRecvNodeCount; i++ )
		{
			TIPSCommNode &sNode = m_IPSRecvNodeList[i];
			CString RecvFilename(sNode.wsFilename.c_str());
			CString RecvFileDateTime=ExtractFilenameDateTime(RecvFilename);
			//if ( sNode.nPPID == nPPID )//需要改成檔案名稱
			//if ( RecvFilename.CompareNoCase(Filename) == 0 )//需要改成時間, 因為檔名前綴會變化
			if ( RecvFileDateTime.Compare(FileDateTime) == 0 )
			{
				bFound = true;
				RecvNode = sNode;
				int nErrorCode = sNode.nErrorCode;
				bool bAckID = CheckIPSStatusID_Ack(sNode);
				CString strErrorCode = sNode.wsErrorCode.c_str();				
				m_IPSRecvNodeList.erase(m_IPSRecvNodeList.begin()+i);
				if ( 0 != nErrorCode )
				{
					UnlockIPSProc();
					str = _T("Error, IPS");
					str = GetMESMultiLanguage(str);
					m_ErrorString.Format(_T("%s %s"), str, strErrorCode);
					return false;
				}
				break;
			}
			else
			{	i = i;	}
		}	
		UnlockIPSProc();

		if ( true == bFound )
		{	break; }

		ChkCount ++;
		if ( ChkCount >= MaxChkCount ) 
		{	break; }

		::Sleep(SleepTime);
	};

	if ( ChkCount == MaxChkCount )
	{
		str = _T("Error, wait for IPS response too long");
		str = GetMESMultiLanguage(str);
		m_ErrorString.Format(_T("%s [PPID:%04d]"), str, nPPID);
		return false;
	}
#endif//IPS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::ExecIPSComm_MesSetParam(bool bThread, TIPSCommNode &sNode, bool &bBreak, bool &bRemove)
{	
#ifndef IPS_DISABLE
	int bRet=false;	
	int   nErrorCode=false;
	std::wstring wsErrorCode;	
	const UINT ConvertCode = CP_UTF8;
	const int nStatus = sNode.nStatus;
	const std::string sBuff = sNode.sRawData;
	const TASK_STATE_MODE  OnlineTaskState = GetOnlineTaskState();
	const IPS_COMM_ID IPS_StatusID=(IPS_COMM_ID)(sNode.nStatusCode);
	
	bBreak = false;
	bRemove = true;	
	switch ( OnlineTaskState )
	{
	case TASK_STATE_RUNNING:
		nErrorCode=IPS_REMOTE_CONTROL_ERROR_ON_RUNNING;		
		break;
	case TASK_STATE_TO_STOP:
		nErrorCode=IPS_REMOTE_CONTROL_ERROR_ON_STOPPING;		
		break;
	default:		
		nErrorCode=IPS_REMOTE_CONTROL_ERROR_SUCCESS;		
		break;
	}
	
	if ( IPS_REMOTE_CONTROL_ERROR_SUCCESS == nErrorCode )
	{
		std::wstring wsBuf;
		rapidjson::CGMItr itr;	
		rapidjson::WDocument Doc;
		rapidjson::CJsonCtrl JSonCtrl;
		if ( JetAPI::char2wstring(sBuff.c_str(), wsBuf, ConvertCode) == 0 ) 
		{	return false; }
		
		//initial
		Doc.SetObject();
		JSonCtrl.Set(&Doc);			
		if ( JSonCtrl.SetBuffer(wsBuf, Doc) == false ) 
		{	return false; }

		if ( JSonCtrl.FindMember(L"Data", itr) == false ) 
		{	return false; }				
		if ( itr->value.IsObject() == false ) 
		{	return false; }

		//CString str;
		//std::wstring wsMemName;		
		rapidjson::CGMItr itrLv2;	
		TSystemParameter &SysParam=AOIDataCollect.GetSystemParameter();
		
		itrLv2 = itr->value.FindMember(L"Location");
		if ( itrLv2 != itr->value.MemberEnd() )
		{
			if ( itrLv2->value.IsString() ) 
			{	SysParam.m_MachineLocation=itrLv2->value.GetString();	}
		}
		itrLv2 = itr->value.FindMember(L"Building");
		if ( itrLv2 != itr->value.MemberEnd() )
		{
			if ( itrLv2->value.IsString() ) 
			{	SysParam.m_MachineBuilding=itrLv2->value.GetString();	}
		}
		itrLv2 = itr->value.FindMember(L"Floor");
		if ( itrLv2 != itr->value.MemberEnd() )
		{
			if ( itrLv2->value.IsString() ) 
			{	SysParam.m_MachineFloor=itrLv2->value.GetString();	}
		}
		itrLv2 = itr->value.FindMember(L"Station");
		if ( itrLv2 != itr->value.MemberEnd() )
		{
			if ( itrLv2->value.IsString() ) 
			{	
				SysParam.m_MachineStation=itrLv2->value.GetString();	
				SysParam.m_MachineStation_LB=itrLv2->value.GetString();	
			}
		}
		itrLv2 = itr->value.FindMember(L"Room");
		if ( itrLv2 != itr->value.MemberEnd() )
		{
			if ( itrLv2->value.IsString() ) 
			{	SysParam.m_MachineRoom=itrLv2->value.GetString();	}
		}
		itrLv2 = itr->value.FindMember(L"Line");
		if ( itrLv2 != itr->value.MemberEnd() )
		{
			if ( itrLv2->value.IsString() ) 
			{	
				SysParam.m_MachineLine=itrLv2->value.GetString();	
				SysParam.m_MachineLine_LB=itrLv2->value.GetString();	
			}
		}

		CAOIProject *ProjectPtr=AOIDataCollect.GetActiveProject();
		if ( NULL != ProjectPtr )
		{
			//"Project": "F:\\AOI3DProject\\MultiPanel_T_25M.PRG",        
			//"Product": "MultiPanel",
			//"ProjectVersion": "",
		}
		/*
		"Data": {
		"Lane": "Lane",
		"User": "Sign Out",    
		"Security": "未登入",
		"Factory": "Factory",
		"Machine": "JET8000",
		"Department": "Department",
		"Shift": "Shift",
		"FixtureID": "FixtureID",
		"BoardName": "BoardName",    
		"SoftwareVersion": "1.01.03.100"
		*/
	}	

	int  nPPID=0;
	const bool bAck=false;
	const DWORD AckTime=1000;
	rapidjson::WDocument Doc;
	rapidjson::CJsonCtrl JSonCtrl;	

	Doc.SetObject();
	GetIPS_RemoteControlErrorText(nErrorCode, wsErrorCode);		
	BuildIPSDoc_Ack(nStatus, bAck, AckTime, nErrorCode, wsErrorCode.c_str(), Doc);
		
	std::string  sBufSend;
	std::wstring wBufSend;
	
	JSonCtrl.Set(&Doc);	
	rapidjson::WValue Data(rapidjson::kObjectType);	
	bRet=JSonCtrl.AddObject(&Data, L"MachineStatus", L"");
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}
	bRet=JSonCtrl.AddObject(&Data, L"SoftwareStatus", L"");
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}
	bRet=JSonCtrl.AddMember(L"Data", Data);
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}	

	JSonCtrl.GetBuffer(wBufSend, Doc);
	if ( JetAPI::wchar2string(wBufSend.c_str(), sBufSend, ConvertCode) == false ) 
	{	return false; }

	const bool bChkFrz = false;
	CString Filename=sNode.wsFilename.c_str();
	if ( GetAckFilename(Filename) == false ) { return false; }
	if ( SendToIPSNode(sBufSend.c_str(), false, sNode.nPPID, 0, bChkFrz, Filename) == false )
	{	return false; }
#endif//IPS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::ExecIPSComm_MesGetStatus(bool bThread, TIPSCommNode &sNode, bool &bBreak, bool &bRemove)
{
#ifndef IPS_DISABLE
	int bRet=false;	
	int   nErrorCode=false;
	std::wstring wsErrorCode;	
	const UINT ConvertCode = CP_UTF8;
	const int nStatus = sNode.nStatus;
	const std::string sBuff = sNode.sRawData;		
	const IPS_COMM_ID IPS_StatusID=(IPS_COMM_ID)(sNode.nStatusCode);	
	
	bBreak = false;
	bRemove = true;		

	int  nPPID=0;
	const bool bAck=false;
	const DWORD AckTime=1000;
	rapidjson::WDocument Doc;
	rapidjson::CJsonCtrl JSonCtrl;	

	Doc.SetObject();
	nErrorCode = IPS_REMOTE_CONTROL_ERROR_SUCCESS;	
	GetIPS_RemoteControlErrorText(nErrorCode, wsErrorCode);		
	BuildIPSDoc_Ack(nStatus, bAck, AckTime, nErrorCode, wsErrorCode.c_str(), Doc);	
	
	std::string  sBufSend;
	std::wstring wBufSend;
	const int EqpOnlineMode = GetEqpOnlineMode();
	
	JSonCtrl.Set(&Doc);	
	rapidjson::WValue Data(rapidjson::kObjectType);	
	bRet=JSonCtrl.AddObject(&Data, L"MachineStatus", EqpOnlineMode);//Offline:406, OnLine:407, LocalOnline:413, Other:0
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}
	//bRet=JSonCtrl.AddObject(&Data, L"RCMDStatus", 0);//On:601, Off:602, Other:0
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}
	bRet=JSonCtrl.AddMember(L"Data", Data);
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}	

	JSonCtrl.GetBuffer(wBufSend, Doc);
	if ( JetAPI::wchar2string(wBufSend.c_str(), sBufSend, ConvertCode) == false ) 
	{	return false; }

	const bool bChkFrz = false;
	CString Filename=sNode.wsFilename.c_str();
	if ( GetAckFilename(Filename) == false ) { return false; }
	if ( SendToIPSNode(sBufSend.c_str(), false, sNode.nPPID, 0, bChkFrz, Filename) == false )
	{	return false; }
#endif//IPS_DISABLE	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::ExecIPSComm_MesOpenProject(bool bThread, TIPSCommNode &sNode, bool &bBreak, bool &bRemove)
{	
#ifndef IPS_DISABLE	
	CString str;
	int   nErrorCode=0;
	int   NameCount=0;
	std::wstring wsErrorCode=L"";
	CAOIProject *ProjectPtr=NULL;
	const bool bOnline = false;		
	const bool FindFilename=true;
	const UINT ConvertCode = CP_UTF8;
	const TASK_STATE_MODE  OnlineTaskState = GetOnlineTaskState();
	const IPS_COMM_ID IPS_StatusID=(IPS_COMM_ID)(sNode.nStatusCode);	
	
	TMES_ProjectOpen ProjectOpen;	
	CString ProjectName, PathName;	

	bBreak = false;
	bRemove = true;

	NameCount=0;
	ClearProjectOpenList();	
	if ( true == FindFilename )
	{
		std::wstring wsBuf, wsVal;		
		rapidjson::CGMItr itr, itrLv2;	
		rapidjson::WDocument Doc;
		rapidjson::CJsonCtrl JSonCtrl;			
		const std::string sBuff = sNode.sRawData;	
		if ( JetAPI::char2wstring(sBuff.c_str(), wsBuf, ConvertCode) == 0 ) 
		{	return false; }

		//initial
		Doc.SetObject();
		JSonCtrl.Set(&Doc);	
		if ( JSonCtrl.SetBuffer(wsBuf, Doc) == true ) 
		{
			if ( JSonCtrl.FindMember(L"Data", itr) == true ) 
			{
				if ( itr->value.IsObject() == true ) 
				{
					itrLv2 = itr->value.FindMember(L"Lane");
					if ( itrLv2 != itr->value.MemberEnd() )
					{
						if ( itrLv2->value.IsInt() == true )
						{	ProjectOpen.nLaneID = itrLv2->value.GetInt();	}
					}
					itrLv2 = itr->value.FindMember(L"Side");
					if ( itrLv2 != itr->value.MemberEnd() )
					{
						if ( itrLv2->value.IsInt() == true )
						{	ProjectOpen.nSide = itrLv2->value.GetInt();	}
					}
					itrLv2 = itr->value.FindMember(L"LaneClearType");
					if ( itrLv2 != itr->value.MemberEnd() )
					{
						if ( itrLv2->value.IsInt() == true )
						{	ProjectOpen.nLaneClearType = itrLv2->value.GetInt();	}
					}
					itrLv2 = itr->value.FindMember(L"Project");
					if ( itrLv2 != itr->value.MemberEnd() )
					{
						if ( itrLv2->value.IsString() == true )
						{	
							ProjectName = CString(itrLv2->value.GetString());	
							ProjectOpen.sFilename = ProjectName;	
							NameCount ++;
							AddProjectOpen(ProjectOpen);							
						}
					}
				}
			}			
		}		
	}

	if ( CheckEqpCtrlState_Remote() == false )
	{	nErrorCode=IPS_REMOTE_CONTROL_ERROR_OFFLINE;	}
	else if ( CheckRemoteCtrlOn() == false )
	{	nErrorCode=IPS_REMOTE_CONTROL_ERROR_REMOTE_CTRL_DISABLE;	}
	else if ( 0 == NameCount )
	{	nErrorCode=IPS_REMOTE_CONTROL_ERROR_PROJECT_OPEN;	}
	else
	{
		bRemove = true;			
		nErrorCode = IPS_REMOTE_CONTROL_ERROR_SUCCESS;			
		PathName.Format(_T("%s\\%s"), AOIDataCollect.GetAOIProjectDirectory(), ProjectName);
		switch ( OnlineTaskState )
		{					
		case TASK_STATE_RUNNING:
			nErrorCode=IPS_REMOTE_CONTROL_ERROR_ON_RUNNING;			
			break;
		case TASK_STATE_TO_STOP:
			nErrorCode=IPS_REMOTE_CONTROL_ERROR_ON_STOPPING;			
			break;
		case TASK_STATE_NONE:
		case TASK_STATE_IDLE:
			break;
		}
	}
	
	int  nPPID=0;
	bool bRet=true;
	const bool bAck=false;
	const DWORD AckTime=1000;
	rapidjson::WDocument Doc;
	rapidjson::CJsonCtrl JSonCtrl;	
	const int nStatus=IPS_StatusID;

	Doc.SetObject();
	GetIPS_RemoteControlErrorText(nErrorCode, wsErrorCode);
	BuildIPSDoc_Ack(nStatus, bAck, AckTime, nErrorCode, wsErrorCode.c_str(), Doc);
		
	std::string  sBufSend;
	std::wstring wBufSend;
	
	JSonCtrl.Set(&Doc);	
	rapidjson::WValue Data(rapidjson::kObjectType);	
	if ( IPS_REMOTE_CONTROL_ERROR_SUCCESS != nErrorCode )
	{
		bRet=JSonCtrl.AddObject(&Data, L"Message", wsErrorCode);
		if ( false == bRet ) 
		{
			m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
			return false; 
		}
	}	
	bRet=JSonCtrl.AddObject(&Data, L"ACK", 1);
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}	
	bRet=JSonCtrl.AddMember(L"Data", Data);
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}	

	JSonCtrl.GetBuffer(wBufSend, Doc);
	if ( JetAPI::wchar2string(wBufSend.c_str(), sBufSend, ConvertCode) == false ) 
	{	return false; }

	const bool bChkFrz = false;
	CString Filename=sNode.wsFilename.c_str();
	if ( GetAckFilename(Filename) == false ) { return false; }
	if ( SendToIPSNode(sBufSend.c_str(), false, sNode.nPPID, 0, bChkFrz, Filename) == false )
	{	return false; }

	if ( IPS_REMOTE_CONTROL_ERROR_SUCCESS == nErrorCode )
	{	PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_MES_CMD_OPEN_PROJECT, NULL);	 }
#endif//IPS_DISABLE	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::ExecIPSComm_MesListProject(bool bThread, TIPSCommNode &sNode, bool &bBreak, bool &bRemove)
{	
#ifndef IPS_DISABLE	
	size_t i=0;
	CString str;	
	int   nErrorCode=0;				
	std::wstring wsBuffer=L"";
	std::wstring wsErrorCode=L"";
	CAOIProject *ProjectPtr=NULL;
	const bool bOnline = false;			
	const UINT ConvertCode = CP_UTF8;
	const TASK_STATE_MODE  OnlineTaskState = GetOnlineTaskState();
	const IPS_COMM_ID IPS_StatusID=(IPS_COMM_ID)(sNode.nStatusCode);			
	
	int  nPPID=0;
	bool bRet=true;
	const bool bAck=false;
	const DWORD AckTime=1000;
	rapidjson::WDocument Doc;
	rapidjson::CJsonCtrl JSonCtrl;	
	const int nStatus=IPS_StatusID;	

	nErrorCode=IPS_REMOTE_CONTROL_ERROR_SUCCESS;	
	if ( CheckEqpCtrlState_Remote() == false )
	{	nErrorCode=IPS_REMOTE_CONTROL_ERROR_OFFLINE;	}
	else if ( CheckRemoteCtrlOn() == false )
	{	nErrorCode=IPS_REMOTE_CONTROL_ERROR_REMOTE_CTRL_DISABLE;	}

	Doc.SetObject();
	GetIPS_RemoteControlErrorText(nErrorCode, wsErrorCode);
	BuildIPSDoc_Ack(nStatus, bAck, AckTime, nErrorCode, wsErrorCode.c_str(), Doc);
		
	std::string  sBufSend;
	std::wstring wBufSend;

	JSonCtrl.Set(&Doc);	
	auto &alc = Doc.GetAllocator();
	rapidjson::WValue Data(rapidjson::kObjectType);		
	if ( IPS_REMOTE_CONTROL_ERROR_SUCCESS != nErrorCode )
	{
		bRet=JSonCtrl.AddObject(&Data, L"Message", wsErrorCode);
		if ( false == bRet ) 
		{
			m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
			return false; 
		}
	}
	else
	{
		const bool bFullName=false;	
		std::vector<CString> NameList;	
		//AOIDataCollect.GetProjectNameList(NameList, bFullName);		
		CString ProjectFolder=AOIDataCollect.GetAOIProjectDirectory();	
		JetAPI::ListFilesInFolder(ProjectFolder, _T("PRG"), NameList);
		const size_t NameCount=NameList.size();
		rapidjson::WValue NameArray(rapidjson::kArrayType);		
		for ( size_t i=0; i<NameCount; i++ )
		{
			str = NameList[i];				
			JetAPI::TCHAR2wstring(str, wsBuffer, ConvertCode);		
			rapidjson::WValue Name(wsBuffer.c_str(), wsBuffer.length(), alc);//使用此方法深層複製字串內容		
			NameArray.PushBack(Name, alc);		
		}
		bRet=JSonCtrl.AddObject(&Data, L"ProjectList", NameArray);//ProjectList
		if ( false == bRet ) 
		{
			m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
			return false; 
		}
	}
	
	bRet=JSonCtrl.AddMember(L"Data", Data);
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}	
	
	JSonCtrl.GetBuffer(wBufSend, Doc);
	if ( JetAPI::wchar2string(wBufSend.c_str(), sBufSend, ConvertCode) == false ) 
	{	return false; }

	bRemove = true;
	const bool bChkFrz = false;
	CString Filename=sNode.wsFilename.c_str();
	if ( GetAckFilename(Filename) == false ) { return false; }
	if ( SendToIPSNode(sBufSend.c_str(), false, sNode.nPPID, 0, bChkFrz, Filename) == false )
	{	return false; }		
#endif//IPS_DISABLE	
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::ExecIPSComm_MesRemoteControl(bool bThread, TIPSCommNode &sNode, bool &bBreak, bool &bRemove)
{	
#ifndef IPS_DISABLE	
	CString str;	
	int   nErrorCode=0;
	std::wstring wsMessage=L"";	
	std::wstring wsErrorCode=L"";
	CAOIProject *ProjectPtr=NULL;
	const bool bAutoReset = true;
	const bool bChkStartLight = true;
	const UINT ConvertCode = CP_UTF8;			
	const TASK_STATE_MODE  OnlineTaskState = GetOnlineTaskState();	
	const IPS_COMM_ID IPS_StatusID=(IPS_COMM_ID)(sNode.nStatusCode);				

	bRemove = true;		
	nErrorCode=IPS_REMOTE_CONTROL_ERROR_SUCCESS;	

	bool bReadSucc=true;
	std::wstring wsBuf, wsVal;		
	rapidjson::CGMItr Data_itr;	
	rapidjson::CGMItr itrLv2;	
	rapidjson::WDocument ReadDoc;
	rapidjson::CJsonCtrl JSonCtrl;				
	const std::string sBuff = sNode.sRawData;		
	//initial
	ReadDoc.SetObject();
	JSonCtrl.Set(&ReadDoc);			
	bReadSucc = false;
	if ( JetAPI::char2wstring(sBuff.c_str(), wsBuf, ConvertCode) != 0 ) 	
	{
		if ( JSonCtrl.SetBuffer(wsBuf, ReadDoc) == true ) 		
		{
			if ( JSonCtrl.FindMember(L"Data", Data_itr) == true ) 
			{
				if ( Data_itr->value.IsObject() == true ) 
				{	bReadSucc = true;	}
			}
		}
	}
	if ( false == bReadSucc )
	{	nErrorCode=IPS_REMOTE_CONTROL_ERROR_JSON_DATA_EXCEPTION;	}	

	if ( IPS_REMOTE_CONTROL_ERROR_SUCCESS == nErrorCode )
	{
		if ( CheckEqpCtrlState_Remote() == false )
		{	nErrorCode=IPS_REMOTE_CONTROL_ERROR_OFFLINE;	}		
	}
	if ( IPS_REMOTE_CONTROL_ERROR_SUCCESS == nErrorCode )
	{
		switch ( IPS_StatusID )
		{
		case IPS_COMM_REMOTE_ON:
		case IPS_COMM_REMOTE_OFF:
			break;
		default:
			if ( CheckRemoteCtrlOn() == false )
			{	nErrorCode=IPS_REMOTE_CONTROL_ERROR_REMOTE_CTRL_DISABLE;	}
			break;
		}	
	}
	if ( IPS_REMOTE_CONTROL_ERROR_SUCCESS == nErrorCode )
	{
		switch ( IPS_StatusID )
		{
		case IPS_COMM_REMOTE_ON:			
			SetRemoteCtrlOn(true);
			break;
		case IPS_COMM_REMOTE_OFF:			
			SetRemoteCtrlOn(false);
			break;
		case IPS_COMM_REMOTE_START:			
			if ( false==bThread ) 
			{	
				bBreak = true;	
				bRemove = false;
			}
			else
			{	
				switch ( OnlineTaskState )
				{					
				case TASK_STATE_RUNNING:
					nErrorCode=IPS_REMOTE_CONTROL_ERROR_ON_RUNNING;					
					break;
				case TASK_STATE_TO_STOP:
					nErrorCode=IPS_REMOTE_CONTROL_ERROR_ON_STOPPING;					
					break;
				case TASK_STATE_NONE:
				case TASK_STATE_IDLE:
					if ( CheckSystemReady(bChkStartLight, bAutoReset) == true ) 
					{
						ProjectPtr = GetActiveProject();
						if ( NULL != ProjectPtr ) 
						{	PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_MES_CMD_ONLINE_RUN, NULL);	}
						else
						{	nErrorCode=IPS_REMOTE_CONTROL_ERROR_PROJECT_CLOSED;	}
					}
					else
					{	
						str = GetErrorString();
						JetAPI::TCHAR2wstring(str, wsMessage, ConvertCode);
						nErrorCode = IPS_REMOTE_CONTROL_ERROR_SYSTEM_NOT_READY;
					}
					break;
				}
			}					
			break;
		case IPS_COMM_REMOTE_STOP:
			switch ( OnlineTaskState )
			{
			case TASK_STATE_NONE:						
				break;
			case TASK_STATE_RUNNING:
				SetOnlineTaskState(TASK_STATE_TO_STOP);
				break;
			case TASK_STATE_TO_STOP:
				break;
			case TASK_STATE_IDLE:
				break;
			}		
			break;
		case IPS_COMM_REMOTE_BYPASS:
			if ( false==bThread ) 
			{	
				bBreak = true;	
				bRemove = false;
			}
			else
			{			
				switch ( OnlineTaskState )
				{
				case TASK_STATE_RUNNING:
					nErrorCode=IPS_REMOTE_CONTROL_ERROR_ON_RUNNING;					
					break;
				case TASK_STATE_TO_STOP:
					nErrorCode=IPS_REMOTE_CONTROL_ERROR_ON_STOPPING;					
					break;
				case TASK_STATE_NONE:
				case TASK_STATE_IDLE:
					if ( CheckSystemReady(bChkStartLight, bAutoReset) == true ) 
					{	PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_MES_CMD_ONLINE_BYPASS, NULL);	}
					else
					{	
						str = GetErrorString();
						JetAPI::TCHAR2wstring(str, wsMessage, ConvertCode);
						nErrorCode = IPS_REMOTE_CONTROL_ERROR_SYSTEM_NOT_READY;
					}					
					break;
				}
			}
			break;
		case IPS_COMM_REMOTE_PAUSE:			
			nErrorCode=IPS_REMOTE_CONTROL_ERROR_NO_SUPPORT_FUNC;			
			break;
		case IPS_COMM_REMOTE_RESUME:
			nErrorCode=IPS_REMOTE_CONTROL_ERROR_NO_SUPPORT_FUNC;			
			break;
		case IPS_COMM_REMOTE_ABORT:
			switch ( OnlineTaskState )
			{
			case TASK_STATE_NONE:						
				break;
			case TASK_STATE_RUNNING:				
				SetOnlineTaskState(TASK_STATE_TO_ABORT);
				break;
			case TASK_STATE_TO_STOP:
				break;
			case TASK_STATE_IDLE:
				break;
			}	
			break;		
		case IPS_COMM_REMOTE_PARAM_UPLOAD:
		case IPS_COMM_REMOTE_PROJECT_UPLOAD:			
			itrLv2 = Data_itr->value.FindMember(L"ProjectName");
			if ( itrLv2 == Data_itr->value.MemberEnd() )
			{	nErrorCode=IPS_REMOTE_CONTROL_ERROR_JSON_DATA_EXCEPTION;	}
			else
			{					
				if ( AOIDataCollect.CheckMESComm_ProjectUploadCanRun(CString(itrLv2->value.GetString())) == false )
				{
					str = AOIDataCollect.GetErrorString();
					JetAPI::TCHAR2wstring(str, wsMessage, ConvertCode);
					nErrorCode=IPS_REMOTE_CONTROL_ERROR_PROJECT_NOT_ALLOW;	
				}
			}
			break;		
		case IPS_COMM_REMOTE_UPLOAD_FINISHED:
		case IPS_COMM_REMOTE_PARAM_UPLOAD_FINISHED:
			AOIDataCollect.ResetMESComm_RemoteCtrlProject();
			break;
		case IPS_COMM_REMOTE_PARAM_DOWNLOAD:
		case IPS_COMM_REMOTE_PROJECT_DOWNLOAD:			
			itrLv2 = Data_itr->value.FindMember(L"ProjectName");
			if ( itrLv2 == Data_itr->value.MemberEnd() )
			{	nErrorCode=IPS_REMOTE_CONTROL_ERROR_JSON_DATA_EXCEPTION;	}
			else
			{	
				if ( AOIDataCollect.CheckMESComm_ProjectDownloadCanRun(CString(itrLv2->value.GetString())) == false )
				{
					str = AOIDataCollect.GetErrorString();
					JetAPI::TCHAR2wstring(str, wsMessage, ConvertCode);
					nErrorCode=IPS_REMOTE_CONTROL_ERROR_PROJECT_NOT_ALLOW;	
				}
			}
			break;
		case IPS_COMM_REMOTE_DOWNLOAD_FINISHED:
		case IPS_COMM_REMOTE_PARAM_DOWNLOAD_FINISHED:
			AOIDataCollect.ResetMESComm_RemoteCtrlProject();
			break;
		case IPS_COMM_REMOTE_PROJECT_DELETE:			
			itrLv2 = Data_itr->value.FindMember(L"ProjectName");
			if ( itrLv2 == Data_itr->value.MemberEnd() )
			{	nErrorCode=IPS_REMOTE_CONTROL_ERROR_JSON_DATA_EXCEPTION;	}
			else
			{	
				if ( AOIDataCollect.CheckMESComm_ProjectDeleteCanRun(CString(itrLv2->value.GetString())) == false )
				{
					str = AOIDataCollect.GetErrorString();
					JetAPI::TCHAR2wstring(str, wsMessage, ConvertCode);
					nErrorCode=IPS_REMOTE_CONTROL_ERROR_PROJECT_NOT_ALLOW;	
				}
			}
			break;
		case IPS_COMM_REMOTE_DELETE_FINISHED:			
			AOIDataCollect.ResetMESComm_RemoteCtrlProject();
			break;
		case IPS_COMM_REMOTE_SET_LOT_NUMBER:
			itrLv2 = Data_itr->value.FindMember(L"LotID");
			if ( itrLv2 == Data_itr->value.MemberEnd() )
			{	nErrorCode=IPS_REMOTE_CONTROL_ERROR_JSON_DATA_EXCEPTION;	}
			else
			{
				ProjectPtr = GetActiveProject();
				if ( NULL == ProjectPtr ) 
				{	nErrorCode=IPS_REMOTE_CONTROL_ERROR_PROJECT_CLOSED;	}
				else
				{
					ProjectPtr->SetProjectWorkNumber(CString(itrLv2->value.GetString()));	
					PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_UPDATE, NULL);	
				}
			}
			break;
		}	
	}

	if ( IPS_REMOTE_CONTROL_ERROR_SUCCESS == nErrorCode )
	{
		//避免重複發送, 等最後完成後才移除
		if ( false == bRemove )
		{	return true; }
	}

	int  nPPID=0;
	bool bRet=true;
	const bool bAck=false;
	const DWORD AckTime=1000;
	rapidjson::WDocument Doc;
	//rapidjson::CJsonCtrl JSonCtrl;	
	const int nStatus=IPS_StatusID;

	Doc.SetObject();	
	GetIPS_RemoteControlErrorText(nErrorCode, wsErrorCode);
	BuildIPSDoc_Ack(nStatus, bAck, AckTime, nErrorCode, wsErrorCode.c_str(), Doc);
		
	std::string  sBufSend;
	std::wstring wBufSend;
	
	JSonCtrl.Set(&Doc);	
	rapidjson::WValue Data(rapidjson::kObjectType);	
	bRet=JSonCtrl.AddObject(&Data, L"Message", wsErrorCode);
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}	
	bRet=JSonCtrl.AddMember(L"Data", Data);
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}	

	JSonCtrl.GetBuffer(wBufSend, Doc);
	if ( JetAPI::wchar2string(wBufSend.c_str(), sBufSend, ConvertCode) == false ) 
	{	return false; }

	const bool bChkFrz = false;
	CString Filename=sNode.wsFilename.c_str();
	if ( GetAckFilename(Filename) == false ) { return false; }
	if ( SendToIPSNode(sBufSend.c_str(), false, sNode.nPPID, 0, bChkFrz, Filename) == false )
	{	return false; }
#endif//IPS_DISABLE	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::ExecIPSComm_PcsShowMessage(bool bThread, TIPSCommNode &sNode, bool &bBreak, bool &bRemove)
{
#ifndef IPS_DISABLE	
	CString str;
	int   nErrorCode=0;
	int   NameCount=0;
	std::wstring wsErrorCode=L"";
	CAOIProject *ProjectPtr=NULL;
	const bool bOnline = false;		
	const bool FindFilename=true;
	const UINT ConvertCode = CP_UTF8;
	const TASK_STATE_MODE  OnlineTaskState = GetOnlineTaskState();
	const IPS_COMM_ID IPS_StatusID=(IPS_COMM_ID)(sNode.nStatusCode);	
	
	bRemove = true;			

	int ShowType=0;
	CString ShowMessage;
	const int ShowTypeConfirmByClick=0;
	const int ShowTypeSecondsDisplay=1;
	std::wstring wsBuf, wsVal;		
	rapidjson::CGMItr itr, itrLv2;	
	rapidjson::WDocument RecvDoc;
	rapidjson::CJsonCtrl JSonCtrl;			
	const std::string sBuff = sNode.sRawData;	
	if ( JetAPI::char2wstring(sBuff.c_str(), wsBuf, ConvertCode) == 0 ) 
	{	return false; }

	//initial
	RecvDoc.SetObject();
	JSonCtrl.Set(&RecvDoc);			
	nErrorCode = IPS_REMOTE_CONTROL_ERROR_JSON_DATA_EXCEPTION;
	if ( JSonCtrl.SetBuffer(wsBuf, RecvDoc) == true ) 
	{
		if ( JSonCtrl.FindMember(L"Data", itr) == true ) 
		{
			if ( itr->value.IsObject() == true ) 
			{				
				itrLv2 = itr->value.FindMember(L"Type");
				if ( itrLv2 != itr->value.MemberEnd() )				
				{
					if ( itrLv2->value.IsInt() == true )					
					{	ShowType = itrLv2->value.GetInt();	}
				}//itrLv2

				itrLv2 = itr->value.FindMember(L"Message");
				if ( itrLv2 != itr->value.MemberEnd() )				
				{
					if ( itrLv2->value.IsString() == true )					
					{	ShowMessage = CString(itrLv2->value.GetString());	}
				}//itrLv2
			}
		}	
		if ( ShowMessage.GetLength() > 0 )
		{	nErrorCode = IPS_REMOTE_CONTROL_ERROR_SUCCESS; }
	}			

	int  nPPID=0;
	bool bRet=true;
	const bool bAck=false;
	const DWORD AckTime=1000;
	rapidjson::WDocument Doc;
//	rapidjson::CJsonCtrl JSonCtrl;	
	const int nStatus=IPS_StatusID;

	Doc.SetObject();	
	BuildIPSDoc_Ack(nStatus, bAck, AckTime, nErrorCode, wsErrorCode.c_str(), Doc);
		
	std::string  sBufSend;
	std::wstring wBufSend;
	
	JSonCtrl.Set(&Doc);	
	rapidjson::WValue Data(rapidjson::kObjectType);	
	bRet=JSonCtrl.AddObject(&Data, L"ACK", 1);
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}
	bRet=JSonCtrl.AddMember(L"Data", Data);
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}	

	JSonCtrl.GetBuffer(wBufSend, Doc);
	if ( JetAPI::wchar2string(wBufSend.c_str(), sBufSend, ConvertCode) == false ) 
	{	return false; }

	const bool bChkFrz = false;
	CString Filename=sNode.wsFilename.c_str();
	if ( GetAckFilename(Filename) == false ) { return false; }
	if ( SendToIPSNode(sBufSend.c_str(), false, sNode.nPPID, 0, bChkFrz, Filename) == false )
	{	return false; }

	if ( IPS_REMOTE_CONTROL_ERROR_SUCCESS == nErrorCode )
	{	
		AOIDataCollect.SetMES_RemoteCtrlMessage(ShowMessage);
		PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_MES_CMD_SHOW_MESSAGE, ShowType);	 
	}
#endif//IPS_DISABLE		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::ExecIPSComm_PcsFinishedMsg(bool bThread, TIPSCommNode &sNode, bool &bBreak, bool &bRemove)
{
#ifndef IPS_DISABLE	
	CString str;
	int   nErrorCode=0;
	int   NameCount=0;
	std::wstring wsErrorCode=L"";
	CAOIProject *ProjectPtr=NULL;
	const bool bOnline = false;		
	const bool FindFilename=true;
	const UINT ConvertCode = CP_UTF8;
	const TASK_STATE_MODE  OnlineTaskState = GetOnlineTaskState();
	const IPS_COMM_ID IPS_StatusID=(IPS_COMM_ID)(sNode.nStatusCode);	
	
	bRemove = true;
	CString ProjectName;
	CString LockProject=AOIDataCollect.GetMESComm_RemoteCtrlProject();
	const int ShowTypeConfirmByClick=0;
	const int ShowTypeSecondsDisplay=1;
	std::wstring wsBuf, wsVal;		
	rapidjson::CGMItr itr, itrLv2;	
	rapidjson::WDocument RecvDoc;
	rapidjson::CJsonCtrl JSonCtrl;			
	const std::string sBuff = sNode.sRawData;	
	if ( JetAPI::char2wstring(sBuff.c_str(), wsBuf, ConvertCode) == 0 ) 
	{	return false; }

	//initial
	RecvDoc.SetObject();
	JSonCtrl.Set(&RecvDoc);			
	nErrorCode = IPS_REMOTE_CONTROL_ERROR_JSON_DATA_EXCEPTION;
	if ( JSonCtrl.SetBuffer(wsBuf, RecvDoc) == true ) 
	{
		if ( JSonCtrl.FindMember(L"Data", itr) == true ) 
		{
			if ( itr->value.IsObject() == true ) 
			{
				itrLv2 = itr->value.FindMember(L"ProjectName");
				if ( itrLv2 != itr->value.MemberEnd() )				
				{
					if ( itrLv2->value.IsString() == true )					
					{	ProjectName = CString(itrLv2->value.GetString());	}
				}//itrLv2
			}
		}							
	}
	if ( LockProject.CompareNoCase(ProjectName) == 0 )
	{	nErrorCode = IPS_REMOTE_CONTROL_ERROR_SUCCESS;	}

	int  nPPID=0;
	bool bRet=true;
	const bool bAck=false;
	const DWORD AckTime=1000;
	rapidjson::WDocument Doc;
//	rapidjson::CJsonCtrl JSonCtrl;	
	const int nStatus=IPS_StatusID;

	Doc.SetObject();
	GetIPS_RemoteControlErrorText(nErrorCode, wsErrorCode);		
	BuildIPSDoc_Ack(nStatus, bAck, AckTime, nErrorCode, wsErrorCode.c_str(), Doc);
		
	std::string  sBufSend;
	std::wstring wBufSend;
	
	JSonCtrl.Set(&Doc);	
	rapidjson::WValue Data(rapidjson::kObjectType);	
	bRet=JSonCtrl.AddObject(&Data, L"ACK", 1);
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}
	bRet=JSonCtrl.AddMember(L"Data", Data);
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}	

	JSonCtrl.GetBuffer(wBufSend, Doc);
	if ( JetAPI::wchar2string(wBufSend.c_str(), sBufSend, ConvertCode) == false ) 
	{	return false; }

	const bool bChkFrz = false;
	CString Filename=sNode.wsFilename.c_str();
	if ( GetAckFilename(Filename) == false ) { return false; }
	if ( SendToIPSNode(sBufSend.c_str(), false, sNode.nPPID, 0, bChkFrz, Filename) == false )
	{	return false; }

	if ( IPS_REMOTE_CONTROL_ERROR_SUCCESS == nErrorCode )
	{	AOIDataCollect.ResetMESComm_RemoteCtrlProject();	}
#endif//IPS_DISABLE			
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::ExecIPSComm_CheckMESReady()//執行IPS溝通-確認MES就緒
{
#ifndef IPS_DISABLE
	CString str;
	CString Err;
	int  nPPID = 0;
	bool bAck = true;
	int  AckTime = 1000;	
	std::wstring wsBuf;
	if ( BuildIPSDoc_ProcessID(IPS_COMM_ASK_CHECK_CONTINUE, bAck, AckTime, nPPID, wsBuf) == false )
	{	
		Err = GetErrorString();
		str = _T("Error, BuildIPSDoc_ProcessID Fault");
		str = GetMESMultiLanguage(str);
		m_ErrorString.Format(_T("%s\n%s"), str, Err);
		return false; 
	}

	std::string sBuf;
	UINT ConvertCode = CP_UTF8;
	if ( JetAPI::wchar2string(wsBuf.c_str(), sBuf, ConvertCode) == false ) 
	{
		m_ErrorString.Format(_T("Error, Conver wstring to string fault [%s]"), _T("CheckMESReady"));
		return false;
	}
	
	TIPSCommNode RecvNode;	
	if ( SendToIPSNode(sBuf.c_str(), bAck, nPPID, AckTime, RecvNode) == false ) 
	{	
		const int Ack_OK   = 0;
		const int Ack_Err  = 1;
		const int Ack_Wait = 2;
		if ( true==bAck )
		{
			while ( true )
			{
				if ( Ack_Wait != RecvNode.nErrorCode )
				{	return false; }
				if ( WaitForIPSAckFile(nPPID, AckTime, RecvNode) == true )
				{	return true; }
			};
		}		
		return false; 
	}
#endif//IPS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::ExecIPSComm_SetSystemParam()//執行IPS溝通-設定系統參數
{	
#ifndef IPS_DISABLE
	CString str;
	CString Err;
	int  nPPID = 0;
	bool bAck = true;
	int  AckTime = 1000;	
	std::wstring wsBuf;
	if ( BuildIPSDoc_SetSystemParam(bAck, AckTime, nPPID, wsBuf) == false )
	{	
		Err = GetErrorString();
		str = _T("Error, BuildIPSDoc_SetSystemParam Fault");
		str = GetMESMultiLanguage(str);
		m_ErrorString.Format(_T("%s\n%s"), str, Err);
		return false; 
	}

	std::string sBuf;
	UINT ConvertCode = CP_UTF8;
	if ( JetAPI::wchar2string(wsBuf.c_str(), sBuf, ConvertCode) == false ) 
	{
		m_ErrorString.Format(_T("Error, Conver wstring to string fault [%s]"), _T("SetSystemParam"));
		return false;
	}
	
	if ( SendToIPSNode(sBuf.c_str(), bAck, nPPID, AckTime) == false ) 
	{	return false; }
#endif//IPS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::ExecIPSComm_SetProjectOpen(CAOIProject *ProjectPtr)//執行IPS溝通-開啟專案
{
#ifndef IPS_DISABLE
	CString str;
	CString Err;
	int  nPPID = 0;
	bool bAck = true;
	int  AckTime = 1000;	
	std::wstring wsBuf;
	if ( BuildIPSDoc_SetProjectOpen(bAck, AckTime, nPPID, ProjectPtr, wsBuf) == false )
	{
		Err = GetErrorString();
		str = _T("Error, ExecIPSComm_SetProjectOpen Fault");
		str = GetMESMultiLanguage(str);
		m_ErrorString.Format(_T("%s\n%s"), str, Err);
		return false; 
	}

	std::string sBuf;
	UINT ConvertCode = CP_UTF8;
	if ( JetAPI::wchar2string(wsBuf.c_str(), sBuf, ConvertCode) == false ) 
	{
		m_ErrorString.Format(_T("Error, Conver wstring to string fault [%s]"), _T("SetProjectOpen"));
		return false;
	}
	
	if ( SendToIPSNode(sBuf.c_str(), bAck, nPPID, AckTime) == false ) 
	{	return false; }
#endif//IPS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::ExecIPSComm_SetProjectParam(CAOIProject *ProjectPtr)//執行IPS溝通-設定專案參數
{	
#ifndef IPS_DISABLE
	CString str;
	CString Err;
	int  nPPID = 0;
	bool bAck = true;
	int  AckTime = 1000;	
	std::wstring wsBuf;
	if ( BuildIPSDoc_SetProjectParam(bAck, AckTime, nPPID, ProjectPtr, wsBuf) == false )
	{
		Err = GetErrorString();
		str = _T("Error, BuildIPSDoc_SetProjectParam Fault");
		str = GetMESMultiLanguage(str);
		m_ErrorString.Format(_T("%s\n%s"), str, Err);
		return false; 
	}

	std::string sBuf;
	UINT ConvertCode = CP_UTF8;
	if ( JetAPI::wchar2string(wsBuf.c_str(), sBuf, ConvertCode) == false ) 
	{
		m_ErrorString.Format(_T("Error, Conver wstring to string fault [%s]"), _T("SetProjectParam"));
		return false;
	}
	
	if ( SendToIPSNode(sBuf.c_str(), bAck, nPPID, AckTime) == false ) 
	{	return false; }
#endif//IPS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::ExecIPSComm_SetProjectLoadFinished(CAOIProject *ProjectPtr, bool bSucc, LPCTSTR Err)//執行IPS溝通-專案載入完畢
{	
#ifndef IPS_DISABLE
	CString str;	
	int  nPPID = 0;
	bool bAck = true;
	int  AckTime = 1000;	
	std::wstring wsBuf;
	if ( BuildIPSDoc_SetProjectLoadFinished(bAck, AckTime, nPPID, ProjectPtr, bSucc, Err, wsBuf) == false )
	{
		Err = GetErrorString();
		str = _T("Error, BuildIPSDoc_SetProjectLoadFinished Fault");
		str = GetMESMultiLanguage(str);
		m_ErrorString.Format(_T("%s\n%s"), str, Err);
		return false; 
	}

	std::string sBuf;
	UINT ConvertCode = CP_UTF8;
	if ( JetAPI::wchar2string(wsBuf.c_str(), sBuf, ConvertCode) == false ) 
	{
		m_ErrorString.Format(_T("Error, Conver wstring to string fault [%s]"), _T("SetProjectLoadFinished"));
		return false;
	}
	
	if ( SendToIPSNode(sBuf.c_str(), bAck, nPPID, AckTime) == false ) 
	{	return false; }
#endif//IPS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::ExecIPSComm_UploadProjectFile(LPCTSTR filename)//執行IPS溝通-上傳專案檔案
{
#ifndef IPS_DISABLE
	CString str;
	CString Err;
	int  nPPID = 0;
	bool bAck = true;
	int  AckTime = 1000;	
	std::wstring wsBuf;
	if ( BuildIPSDoc_UploadDownloadFile(IPS_COMM_GEN_PROJECT_UPLOAD, bAck, AckTime, nPPID, filename, wsBuf) == false )
	{
		Err = GetErrorString();
		str = _T("Error, BuildIPSDoc_UploadDownloadFile Fault");
		str = GetMESMultiLanguage(str);
		m_ErrorString.Format(_T("%s\n%s"), str, Err);
		return false; 
	}

	std::string sBuf;
	UINT ConvertCode = CP_UTF8;
	if ( JetAPI::wchar2string(wsBuf.c_str(), sBuf, ConvertCode) == false ) 
	{
		m_ErrorString.Format(_T("Error, Conver wstring to string fault [%s]"), _T("UploadProjectFile"));
		return false;
	}
	
	if ( SendToIPSNode(sBuf.c_str(), bAck, nPPID, AckTime) == false ) 
	{	return false; }
#endif//IPS_DISABLE
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::ExecIPSComm_DownloadProjectFile(LPCTSTR filename)//執行IPS溝通-下載專案檔案
{
#ifndef IPS_DISABLE
	CString str;
	CString Err;
	int  nPPID = 0;
	bool bAck = true;
	int  AckTime = 1000;	
	std::wstring wsBuf;
	if ( BuildIPSDoc_UploadDownloadFile(IPS_COMM_GEN_PROJECT_DOWNLOAD, bAck, AckTime, nPPID, filename, wsBuf) == false )
	{
		Err = GetErrorString();
		str = _T("Error, BuildIPSDoc_UploadDownloadFile Fault");
		str = GetMESMultiLanguage(str);
		m_ErrorString.Format(_T("%s\n%s"), str, Err);
		return false; 
	}

	std::string sBuf;
	UINT ConvertCode = CP_UTF8;
	if ( JetAPI::wchar2string(wsBuf.c_str(), sBuf, ConvertCode) == false ) 
	{
		m_ErrorString.Format(_T("Error, Conver wstring to string fault [%s]"), _T("DownloadProjectFile"));
		return false;
	}
	
	if ( SendToIPSNode(sBuf.c_str(), bAck, nPPID, AckTime) == false ) 
	{	return false; }
#endif//IPS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::ExecIPSComm_SetMachineStatus()//執行IPS溝通-設定機台狀態
{
	ONLINE_STATE_MODE eStatus = AOIDataCollect.GetOnlineStateMode();	
	return ExecIPSComm_SetMachineStatus(eStatus);
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::ExecIPSComm_SetMachineStatus(ONLINE_STATE_MODE Status)//執行IPS溝通-設定機台狀態
{	
#ifndef IPS_DISABLE
	CString str;
	CString Err;
	int  nPPID = 0;
	bool bAck = false;
	int  AckTime = 1000;	
	std::wstring wsBuf;	
	const bool bAckMode = CheckIPSAlwaysAckMode();
	if ( true == bAckMode )
	{	bAck = true;	}

	if ( BuildIPSDoc_SetMachineStatus(Status, bAck, AckTime, nPPID, wsBuf) == false )
	{
		Err = GetErrorString();
		str = _T("Error, BuildIPSDoc_SetMachineStatus Fault");
		str = GetMESMultiLanguage(str);
		m_ErrorString.Format(_T("%s[StatusID:%d]\n%s"), str, Status, Err);
		return false; 
	}

	std::string sBuf;
	UINT ConvertCode = CP_UTF8;
	if ( JetAPI::wchar2string(wsBuf.c_str(), sBuf, ConvertCode) == false ) 
	{
		m_ErrorString.Format(_T("Error, Conver wstring to string fault [%s]"), _T("SetMachineStatus"));
		return false;
	}
	
	if ( SendToIPSNode(sBuf.c_str(), bAck, nPPID, AckTime) == false ) 
	{	return false; }
#endif//IPS_DISABLE
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::ExecIPSComm_ProcessID(int ProcessID)//執行IPS溝通-程序運作
{	
#ifndef IPS_DISABLE
	if ( 0 == ProcessID )
	{	return true; }

	CString str;
	CString Err;
	int  nPPID = 0;
	bool bAck = true;
	int  AckTime = 1000;	
	std::wstring wsBuf;
	if ( BuildIPSDoc_ProcessID(ProcessID, bAck, AckTime, nPPID, wsBuf) == false )
	{
		Err = GetErrorString();
		str = _T("Error, BuildIPSDoc_ProcessID Fault");
		str = GetMESMultiLanguage(str);
		m_ErrorString.Format(_T("%s[ProcessID:%d]\n%s"), str, ProcessID, Err);
		return false; 
	}

	std::string sBuf;
	UINT ConvertCode = CP_UTF8;
	if ( JetAPI::wchar2string(wsBuf.c_str(), sBuf, ConvertCode) == false ) 
	{
		m_ErrorString.Format(_T("Error, Conver wstring to string fault [ProcID:%d]"), ProcessID);
		return false;
	}
	
	if ( SendToIPSNode(sBuf.c_str(), bAck, nPPID, AckTime) == false ) 
	{	return false; }
#endif//IPS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::ExecIPSComm_SetAOIExceptionCode(LPCTSTR ErrStr)//執行MES溝通-系統異常碼
{
#ifndef IPS_DISABLE
	if ( CheckBypassAOIExceptionCode() == true )
	{	return true;	}

	CString str;
	CString Err;
	int  nPPID = 0;
	bool bAck = true;
	int  AckTime = 1000;	
	std::wstring wsBuf;
	if ( BuildIPSDoc_SetAOIExceptionCode(bAck, AckTime, ErrStr, nPPID, wsBuf) == false )
	{	
		Err = GetErrorString();
		str = _T("Error, BuildIPSDoc_SetAOIExceptionCode Fault");
		str = GetMESMultiLanguage(str);
		m_ErrorString.Format(_T("%s\n%s"), str, Err);
		return false; 
	}

	std::string sBuf;
	UINT ConvertCode = CP_UTF8;
	if ( JetAPI::wchar2string(wsBuf.c_str(), sBuf, ConvertCode) == false ) 
	{
		m_ErrorString.Format(_T("Error, Conver wstring to string fault [%s]"), _T("SetAOIExceptionCode"));
		return false;
	}
	
	if ( SendToIPSNode(sBuf.c_str(), bAck, nPPID, AckTime) == false ) 
	{	return false; }
#endif//IPS_DISABLE	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::ExecIPSComm_SetUserLogin_out(bool bLogin)//執行IPS溝通-設定使用者登入
{
#ifndef IPS_DISABLE
	CString str;
	CString Err;
	int  nPPID = 0;
	bool bAck = true;
	int  AckTime = 1000;	
	std::wstring wsBuf;
	USER_LEVEL_MODE  UserLevelMode = AOIDataCollect.GetCurrentUserLevel();//使用者權限	
	if ( USER_LEVEL_SIGN_OUT==UserLevelMode ||UserLevelMode>=USER_LEVEL_JET_FAE )
	{	return true; }
	if ( BuildIPSDoc_SetLogin_out(bAck, AckTime, nPPID, bLogin, wsBuf) == false )
	{	
		Err = GetErrorString();
		str = _T("Error, BuildIPSDoc_SetLogin_out Fault");
		str = GetMESMultiLanguage(str);
		m_ErrorString.Format(_T("%s\n%s"), str, Err);
		return false; 
	}

	std::string sBuf;
	UINT ConvertCode = CP_UTF8;
	if ( JetAPI::wchar2string(wsBuf.c_str(), sBuf, ConvertCode) == false ) 
	{
		m_ErrorString.Format(_T("Error, Conver wstring to string fault [%s]"), _T("SetUserLogin_out"));
		return false;
	}
	
	if ( SendToIPSNode(sBuf.c_str(), bAck, nPPID, AckTime) == false ) 
	{	return false; }
#endif//IPS_DISABLE
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::ExecIPSComm_SetControlStateMode(MES_EQP_CTRL_STATE_MODE Mode)//執行IPS溝通-設定控制狀態模式	
{
#ifndef IPS_DISABLE
	CString str;
	CString Err;
	int  nPPID = 0;
	bool bAck = true;
	int  AckTime = 1000;	
	std::wstring wsBuf;
	if ( BuildIPSDoc_SetControlStateMode(bAck, AckTime, nPPID, Mode, wsBuf) == false )
	{	
		Err = GetErrorString();
		str = _T("Error, BuildIPSDoc_SetControlStateMode Fault");
		str = GetMESMultiLanguage(str);
		m_ErrorString.Format(_T("%s\n%s"), str, Err);
		return false; 
	}

	std::string sBuf;
	UINT ConvertCode = CP_UTF8;
	if ( JetAPI::wchar2string(wsBuf.c_str(), sBuf, ConvertCode) == false ) 
	{
		m_ErrorString.Format(_T("Error, Conver wstring to string fault [%s]"), _T("SetControlStateMode"));
		return false;
	}
	
	if ( SendToIPSNode(sBuf.c_str(), bAck, nPPID, AckTime) == false ) 
	{	return false;	}	
#endif//IPS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::ExecIPSComm_CheckBarcode(CAOIProject *ProjectPtr)//執行IPS溝通-確認條碼
{
#ifndef IPS_DISABLE
	CString str;
	CString Err;
	int  nPPID = 0;
	bool bAck = true;
	int  AckTime = 1000;	
	std::wstring wsBuf;	
	if ( BuildIPSDoc_CheckBarcode(bAck, AckTime, nPPID, ProjectPtr, wsBuf) == false )
	{
		Err = GetErrorString();
		str = _T("Error, BuildIPSDoc_CheckBarcode Fault");
		str = GetMESMultiLanguage(str);
		m_ErrorString.Format(_T("%s\n%s"), str, Err);
		return false; 
	}

	std::string sBuf;
	UINT ConvertCode = CP_UTF8;
	if ( JetAPI::wchar2string(wsBuf.c_str(), sBuf, ConvertCode) == false ) 
	{
		m_ErrorString.Format(_T("Error, Conver wstring to string fault [%s]"), _T("CheckBarcode"));
		return false;
	}
	
	if ( SendToIPSNode(sBuf.c_str(), bAck, nPPID, AckTime) == false ) 
	{	return false; }
#endif//IPS_DISABLE	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::ExecIPSComm_AskBarcode(CAOIProject *ProjectPtr)//執行IPS溝通-詢問條碼
{
#ifndef IPS_DISABLE
	if ( CheckProjectPtr(ProjectPtr) == false )
	{	return false; }

	CString str;
	CString Err;
	int  nPPID = 0;
	bool bAck = true;
	int  AckTime = 1000;	
	std::wstring wsBuf;	
	if ( BuildIPSDoc_AskBarcode(bAck, AckTime, nPPID, ProjectPtr, wsBuf) == false )
	{
		Err = GetErrorString();
		str = _T("Error, BuildIPSDoc_AskBarcode Fault");
		str = GetMESMultiLanguage(str);
		m_ErrorString.Format(_T("%s\n%s"), str, Err);
		return false; 
	}

	std::string sBuf;
	UINT ConvertCode = CP_UTF8;
	if ( JetAPI::wchar2string(wsBuf.c_str(), sBuf, ConvertCode) == false ) 
	{
		m_ErrorString.Format(_T("Error, Conver wstring to string fault [%s]"), _T("SetBarcode"));
		return false;
	}	

	TIPSCommNode RecvNode;
	if ( SendToIPSNode(sBuf.c_str(), bAck, nPPID, AckTime, RecvNode, ProjectPtr) == false ) 
	{	return false; }

	std::wstring wsStr;
	rapidjson::CGMItr Data_itr;	
	rapidjson::WDocument Doc;
	rapidjson::CJsonCtrl JSonCtrl;
	sBuf = RecvNode.sRawData;
	if ( JetAPI::char2wstring(sBuf.c_str(), wsBuf, ConvertCode) == 0 ) 
	{	return false; }
		
	//initial
	Doc.SetObject();
	JSonCtrl.Set(&Doc);			
	if ( JSonCtrl.SetBuffer(wsBuf, Doc) == false ) 
	{	return false; }

	if ( JSonCtrl.FindMember(L"Data", Data_itr) == false ) 
	{	return false; }				
	if ( Data_itr->value.IsObject() == false ) 
	{	return false; }

	auto ProjectBarcode_itr=Data_itr->value.FindMember(L"ProjectBarCode");
	if ( (ProjectBarcode_itr!=Data_itr->value.MemberEnd()) &&  (ProjectBarcode_itr->value.IsString()==true) )
	{
		std::wstring wsBarcode = ProjectBarcode_itr->value.GetString();
		if ( wsBarcode.length() > 0 )
		{	ProjectPtr->SetProjectBarcode(wsBarcode.c_str()); }
	}

	auto PanelList_itr=Data_itr->value.FindMember(L"PanelList");
	if ( (PanelList_itr!=Data_itr->value.MemberEnd()) &&  (PanelList_itr->value.IsArray()==true) )
	{		
		for (auto Panel_itr=PanelList_itr->value.Begin();  Panel_itr!=PanelList_itr->value.End(); ++Panel_itr)
		{
			if ( Panel_itr->IsObject() == false ) 
			{	continue; }	

			CAOIPanel *PanelPtr = NULL;
			auto PanelID_itr=Panel_itr->FindMember(L"ID");
			if ( (PanelID_itr!=Panel_itr->MemberEnd()) && (PanelID_itr->value.IsInt()==true) )
			{	
				int ID = PanelID_itr->value.GetInt();
				PanelPtr = ProjectPtr->GetProjectPanelPtr(ID-1, true);
			}			
			if ( NULL == PanelPtr ) { continue; }

			auto PanelBarcode_itr=Panel_itr->FindMember(L"Barcode");
			if ( (PanelBarcode_itr!=Panel_itr->MemberEnd()) && (PanelBarcode_itr->value.IsString()==true) )
			{	
				std::wstring wsBarcode = PanelBarcode_itr->value.GetString();	
				if ( wsBarcode.length() > 0 )
				{	PanelPtr->SetPanelBarcode(wsBarcode.c_str());	}
			}

			auto BoardList_itr=Panel_itr->FindMember(L"BoardList");
			if ( (BoardList_itr!=Panel_itr->MemberEnd()) && (BoardList_itr->value.IsArray()==true) )
			{
				for ( auto Board_itr=BoardList_itr->value.Begin(); Board_itr!=BoardList_itr->value.End(); ++Board_itr )
				{
					if ( Board_itr->IsObject() == false )
					{	continue; }

					CAOIBoard *BoardPtr = NULL;

					auto BoardID_itr=Board_itr->FindMember(L"ID");
					if ( (BoardID_itr!=Board_itr->MemberEnd()) && (BoardID_itr->value.IsInt()==true) )
					{	
						int ID = BoardID_itr->value.GetInt();	
						BoardPtr = PanelPtr->GetPanelBoardPtr(ID-1, true);
					}					
					if ( NULL == BoardPtr ) { continue; }

					auto BoardBarcode_itr=Board_itr->FindMember(L"Barcode");
					if ( (BoardBarcode_itr!=Board_itr->MemberEnd()) && (BoardBarcode_itr->value.IsString()==true) )
					{	
						std::wstring wsBarcode = BoardBarcode_itr->value.GetString();	
						if ( wsBarcode.length() > 0 )
						{	BoardPtr->SetBoardBarcode(wsBarcode.c_str());	}
					}
				}//BoardList_itr
			}
		}//PanelList_itr
	}
#endif//IPS_DISABLE	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::ExecIPSComm_BoardMapping(CAOIProject *ProjectPtr)//執行IPS溝通-單板映射
{	
#ifndef IPS_DISABLE
	if ( CheckProjectPtr(ProjectPtr) == false )
	{	return false; }

	CString str;
	CString Err;
	int  nPPID = 0;
	bool bAck = true;
	int  AckTime = 1000;	
	std::wstring wsBuf;	
	if ( BuildIPSDoc_BoardMapping(bAck, AckTime, nPPID, ProjectPtr, wsBuf) == false )
	{
		Err = GetErrorString();
		str = _T("Error, BuildIPSDoc_BoardMapping Fault");
		str = GetMESMultiLanguage(str);
		m_ErrorString.Format(_T("%s\n%s"), str, Err);
		return false; 
	}

	std::string sBuf;
	UINT ConvertCode = CP_UTF8;
	if ( JetAPI::wchar2string(wsBuf.c_str(), sBuf, ConvertCode) == false ) 
	{
		m_ErrorString.Format(_T("Error, Conver wstring to string fault [%s]"), _T("BoardMapping"));
		return false;
	}	

	TIPSCommNode RecvNode;
	if ( SendToIPSNode(sBuf.c_str(), bAck, nPPID, AckTime, RecvNode, ProjectPtr) == false ) 
	{	return false; }

	std::wstring wsStr;
	rapidjson::CGMItr Data_itr;	
	rapidjson::WDocument Doc;
	rapidjson::CJsonCtrl JSonCtrl;
	sBuf = RecvNode.sRawData;
	LANE_ID LaneID = ProjectPtr->GetProjectActLaneID();
	if ( JetAPI::char2wstring(sBuf.c_str(), wsBuf, ConvertCode) == 0 ) 
	{
		m_ErrorString.Format(_T("Error, Conver string to wstring fault [%s]"), _T("BoardMapping_Ack"));
		return false; 
	}
		
	//initial
	Doc.SetObject();
	JSonCtrl.Set(&Doc);			
	if ( JSonCtrl.SetBuffer(wsBuf, Doc) == false ) 
	{	
		m_ErrorString.Format(_T("Error, Set Json Buffer fault [%s]"), _T("BoardMapping_Ack"));
		return false; 
	}

	if ( JSonCtrl.FindMember(L"Data", Data_itr) == false ) 
	{
		m_ErrorString.Format(_T("Error, Find Json [Data] fault [%s]"), _T("BoardMapping_Ack"));
		return false; 
	}				
	if ( Data_itr->value.IsObject() == false ) 
	{
		m_ErrorString.Format(_T("Error, Find Json [Data] fault [%s]"), _T("BoardMapping_Ack"));
		return false; 
	}

	auto PanelList_itr=Data_itr->value.FindMember(L"PanelList");
	if ( PanelList_itr==Data_itr->value.MemberEnd() || (PanelList_itr->value.IsArray()==false) )
	{
		m_ErrorString.Format(_T("Error, Find Json [PanelList] fault [%s]"), _T("BoardMapping_Ack"));
		return false; 
	}

	for (auto Panel_itr=PanelList_itr->value.Begin();  Panel_itr!=PanelList_itr->value.End(); ++Panel_itr)
	{
		if ( Panel_itr->IsObject() == false ) 
		{	continue; }	

		CAOIPanel *PanelPtr = NULL;
		std::wstring wsPanelBarcode;
		auto PanelID_itr=Panel_itr->FindMember(L"ID");
		if ( (PanelID_itr!=Panel_itr->MemberEnd()) && (PanelID_itr->value.IsInt()==true) )
		{	
			int ID = PanelID_itr->value.GetInt();
			PanelPtr = ProjectPtr->GetProjectPanelPtr(ID-1, true);
		}			
		if ( NULL == PanelPtr ) { continue; }

		auto PanelBarcode_itr=Panel_itr->FindMember(L"Barcode");
		if ( (PanelBarcode_itr!=Panel_itr->MemberEnd()) && (PanelBarcode_itr->value.IsString()==true) )
		{	wsPanelBarcode = PanelBarcode_itr->value.GetString();	}

		auto BoardList_itr=Panel_itr->FindMember(L"BoardList");
		if ( (BoardList_itr!=Panel_itr->MemberEnd()) && (BoardList_itr->value.IsArray()==true) )
		{
			for ( auto Board_itr=BoardList_itr->value.Begin(); Board_itr!=BoardList_itr->value.End(); ++Board_itr )
			{
				if ( Board_itr->IsObject() == false )
				{	continue; }

				int BoardState=0;
				CAOIBoard *BoardPtr = NULL;					

				auto BoardID_itr=Board_itr->FindMember(L"ID");
				if ( (BoardID_itr!=Board_itr->MemberEnd()) && (BoardID_itr->value.IsInt()==true) )
				{	
					int ID = BoardID_itr->value.GetInt();	
					BoardPtr = PanelPtr->GetPanelBoardPtr(ID-1, true);
				}					
				if ( NULL == BoardPtr ) { continue; }

				auto BoardState_itr=Board_itr->FindMember(L"BoardState");
				if ( (BoardState_itr!=Board_itr->MemberEnd()) && (BoardState_itr->value.IsInt()==true) )
				{	
					BoardState = BoardState_itr->value.GetInt();	
					if ( IPS_BOARD_MAPPING_SKIP == BoardState )
					{	BoardPtr->ExecBoardBeXBoard(LaneID);	}
				}
			}//BoardList_itr
		}
	}//PanelList_itr
#endif//IPS_DISABLE	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::ExecIPSComm_UnCheckTestFile(LANE_ID LaneID, int &Count)//執行IPS溝通-未判定檢測檔案數
{
#ifndef IPS_DISABLE
	CString str;
	CString Err;
	int  nPPID = 0;
	bool bAck = true;
	int  AckTime = 1000;	
	std::wstring wsBuf;	
	Count = 0;
	if ( BuildIPSDoc_UnCheckTestFile(bAck, AckTime, nPPID, LaneID, wsBuf) == false )
	{
		Err = GetErrorString();
		str = _T("Error, BuildIPSDoc_UnCheckTestFile Fault");
		str = GetMESMultiLanguage(str);
		m_ErrorString.Format(_T("%s\n%s"), str, Err);
		return false; 
	}

	std::string sBuf;
	UINT ConvertCode = CP_UTF8;
	if ( JetAPI::wchar2string(wsBuf.c_str(), sBuf, ConvertCode) == false ) 
	{
		m_ErrorString.Format(_T("Error, Conver wstring to string fault [%s]"), _T("UnCheckTestFile"));
		return false;
	}	

	TIPSCommNode RecvNode;
	if ( SendToIPSNode(sBuf.c_str(), bAck, nPPID, AckTime, RecvNode) == false ) 
	{	return false; }

	std::wstring wsStr;
	rapidjson::CGMItr Data_itr;	
	rapidjson::WDocument Doc;
	rapidjson::CJsonCtrl JSonCtrl;
	sBuf = RecvNode.sRawData;	
	if ( JetAPI::char2wstring(sBuf.c_str(), wsBuf, ConvertCode) == 0 ) 
	{
		m_ErrorString.Format(_T("Error, Conver string to wstring fault [%s]"), _T("UnCheckTestFile_Ack"));
		return false; 
	}
		
	//initial
	Doc.SetObject();
	JSonCtrl.Set(&Doc);			
	if ( JSonCtrl.SetBuffer(wsBuf, Doc) == false ) 
	{	
		m_ErrorString.Format(_T("Error, Set Json Buffer fault [%s]"), _T("UnCheckTestFile_Ack"));
		return false; 
	}

	if ( JSonCtrl.FindMember(L"Data", Data_itr) == false ) 
	{
		m_ErrorString.Format(_T("Error, Find Json [Data] fault [%s]"), _T("UnCheckTestFile_Ack"));
		return false; 
	}				
	if ( Data_itr->value.IsObject() == false ) 
	{
		m_ErrorString.Format(_T("Error, Find Json [Data] fault [%s]"), _T("UnCheckTestFile_Ack"));
		return false; 
	}

	auto UnCheckCount_itr=Data_itr->value.FindMember(L"UnCheckCount");
	if ( UnCheckCount_itr->value.IsInt()  )
	{	Count = UnCheckCount_itr->value.GetInt();	}
	else if ( UnCheckCount_itr->value.IsInt64() )
	{	Count = UnCheckCount_itr->value.GetInt64();	}
#endif//IPS_DISABLE	
	return true;
}
//-------------------------------------------------------------------------------------//
int CMES_IPS::GetIPSFreePPID()//取得IPS可用的PPID
{
	int uPPID = m_MsgPPID;
	m_MsgPPID ++;
	if ( m_MsgPPID > 0x0000FFFF ) 
	{	m_MsgPPID = 1; }
	return uPPID;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::CheckIPSAlwaysAckMode()//確認總是回傳模式
{
	CITSLinker &Linker=GetIPSLinker();
	ITS_LINK_MODE ITS_LinkMode = Linker.GetITSLinkMode();	
	if ( ITS_LINK_FILE != ITS_LinkMode )
	{	return false;	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::BuildIPSDoc(int nStatus, bool bAck, int AckTime, int &nPPID, rapidjson::WDocument &Doc)//建立IPS檔案
{	
	const int ResultCode = 0;
	const wchar_t *ResultStr =L"OK";
	const bool bSucc=BuildIPSDoc(nStatus, bAck, AckTime, nPPID, ResultCode, ResultStr, Doc);
	return bSucc;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::BuildIPSDoc(int nStatus, bool bAck, int AckTime, int &nPPID, int ResultCode, const wchar_t *ResultStr, rapidjson::WDocument &Doc)//建立IPS文檔		
{
	bool         bRet=false;	
	std::wstring wstrBuffer;
	const UINT ConvertCode = CP_UTF8;
	CString      strStatus;	
	rapidjson::CJsonCtrl JSonCtrl;
	rapidjson::WValue object(rapidjson::kObjectType);	

	nPPID = nStatus;
	strStatus = GetIPSCommText(nStatus);
	JetAPI::TCHAR2wstring(strStatus, wstrBuffer, ConvertCode);

	//"Header": {
    //"Version": "2.0.0",
    //"Sender": "JET8000",
    //"StatusCode": 101,
    //"Status": "Setup",
    //"ResultCode": 0,
    //"Result": "OK"
    //},
	
	//initial	
	JSonCtrl.Set(&Doc);
	
	bRet=JSonCtrl.AddObject(&object, L"Version", L"2.0.0");//Version	
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	bRet=JSonCtrl.AddObject(&object, L"Sender", AOI3D_APP_NAME_W);//Sender//L"JET8000"	
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	bRet=JSonCtrl.AddObject(&object, L"StatusCode", nStatus);//StatusCode
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	bRet=JSonCtrl.AddObject(&object, L"Status", wstrBuffer);//StatusStr
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	bRet=JSonCtrl.AddObject(&object, L"ResultCode", ResultCode);//ResultCode
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	bRet=JSonCtrl.AddObject(&object, L"Result", ResultStr);//ResultStr
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	bRet=JSonCtrl.AddMember(L"Header", object);
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::BuildIPSDoc_Ack(int nStatus, bool bAck, int AckTime, int ResultCode, const wchar_t *ResultStr, rapidjson::WDocument &Doc)//建立IPS文檔	
{	
	bool         bRet=false;	
	std::wstring wstrBuffer;
	const UINT ConvertCode = CP_UTF8;
	CString      strStatus;	
	rapidjson::CJsonCtrl JSonCtrl;
	rapidjson::WValue object(rapidjson::kObjectType);	

	const int nPPID = nStatus;
	strStatus = GetIPSCommText(nStatus);
	JetAPI::TCHAR2wstring(strStatus, wstrBuffer, ConvertCode);

	//"Header": {
    //"Version": "2.0.0",
    //"Sender": "JET8000",
    //"StatusCode": 100,
    //"Status": "Setup",
    //"ResultCode": 0,
    //"Result": "OK"
    //},
	
	//initial	
	JSonCtrl.Set(&Doc);
	
	bRet=JSonCtrl.AddObject(&object, L"Version", L"2.0.0");//Version	
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	bRet=JSonCtrl.AddObject(&object, L"Sender", AOI3D_APP_NAME_W);//Sender//L"JET8000"
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	bRet=JSonCtrl.AddObject(&object, L"StatusCode", nStatus);//StatusCode
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	bRet=JSonCtrl.AddObject(&object, L"Status", wstrBuffer);//StatusStr
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	if ( IPS_REMOTE_CONTROL_ERROR_SUCCESS != ResultCode )
	{	ResultCode = IPS_REMOTE_CONTROL_ERROR_FAIL; }
	bRet=JSonCtrl.AddObject(&object, L"ResultCode", ResultCode);//ResultCode
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	if ( IPS_REMOTE_CONTROL_ERROR_FAIL == ResultCode )//ResultStr
	{	bRet=JSonCtrl.AddObject(&object, L"Result", L"Error");	}
	else
	{	bRet=JSonCtrl.AddObject(&object, L"Result", ResultStr);	}	
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	bRet=JSonCtrl.AddMember(L"Header", object);
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::BuildIPSDoc_SetSystemParam(bool bAck, int AckTime, int &nPPID, std::wstring& wsBuf)//建立系統資訊文檔
{
	bool bRet=true;	
	TSystemParameter &SysParam = AOIDataCollect.GetSystemParameter();	

	CString    str;	
	std::wstring wsBuffer;
	rapidjson::WDocument Doc;
	rapidjson::CJsonCtrl JSonCtrl;	
	const UINT ConvertCode = CP_UTF8;			
	const bool bAckMode = CheckIPSAlwaysAckMode();	
	
	nPPID = 0;	
	//initial
	Doc.SetObject();
	JSonCtrl.Set(&Doc);
	
	if ( true == bAckMode )
	{	bAck = true;	}

	if ( BuildIPSDoc(IPS_COMM_GEN_SYSTEM_SETUP, bAck, AckTime, nPPID, Doc) == false )
	{	return false; }
	
	//"Data": {    
    //"Location": "Location",//廠區
    //"Building": "Building",//棟名
    //"Floor": "Floor",//樓層
    //"Station": "Station",//站名
    //"Room": "Room",//車間
    //"Line": "Line",//線名                
    //"Factory": "Factory",//工廠
    //"Machine": "Machine",//機台
    //"Department": "Department",//部門    
	//}

	//add object into doc
	rapidjson::WValue object(rapidjson::kObjectType);		
	
	str = SysParam.m_MachineLocation;
	JetAPI::TCHAR2wstring(str, wsBuffer, ConvertCode);	
	bRet=JSonCtrl.AddObject(&object, L"Location", wsBuffer);//Location
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	str = SysParam.m_MachineBuilding;
	JetAPI::TCHAR2wstring(str, wsBuffer, ConvertCode);	
	bRet=JSonCtrl.AddObject(&object, L"Building", wsBuffer);//Building
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	str = SysParam.m_MachineFloor;
	JetAPI::TCHAR2wstring(str, wsBuffer, ConvertCode);	
	bRet=JSonCtrl.AddObject(&object, L"Floor", wsBuffer);//Floor
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	str = SysParam.m_MachineStation;
	//str=SysParam.m_MachineStation_LB;
	JetAPI::TCHAR2wstring(str, wsBuffer, ConvertCode);	
	bRet=JSonCtrl.AddObject(&object, L"Station", wsBuffer);//Station
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	str = SysParam.m_MachineRoom;
	JetAPI::TCHAR2wstring(str, wsBuffer, ConvertCode);	
	bRet=JSonCtrl.AddObject(&object, L"Room", wsBuffer);//Room
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	str = SysParam.m_MachineLine;
	//str = SysParam.m_MachineLine_LB;
	JetAPI::TCHAR2wstring(str, wsBuffer, ConvertCode);	
	bRet=JSonCtrl.AddObject(&object, L"Line", wsBuffer);//Line
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}
	
	
	bRet=JSonCtrl.AddObject(&object, L"Factory", L"Factory");
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	str = SysParam.m_MachineName;
	JetAPI::TCHAR2wstring(str, wsBuffer, ConvertCode);	
	bRet=JSonCtrl.AddObject(&object, L"Machine", wsBuffer);//Machine
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	bRet=JSonCtrl.AddObject(&object, L"Department", L"Department");
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}	

	bRet=JSonCtrl.AddMember(L"Data", object);
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

	DumpMESDoc(Doc, L"BuildJSONRaw_SetParam");
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::BuildIPSDoc_SetProjectOpen(bool bAck, int AckTime, int &nPPID, CAOIProject *ProjectPtr, std::wstring& wsBuf)//建立基本資訊文檔
{
	bool bRet=true;	
	CString    str;	
	std::wstring wsBuffer;
	rapidjson::WDocument Doc;
	rapidjson::CJsonCtrl JSonCtrl;	
	const UINT ConvertCode = CP_UTF8;	
	LANE_ID LaneID = GetActiveLaneID();
	CAMERA_ID CameraID =  PRIMARY_CAMERA_ID;	
	const bool bAckMode = CheckIPSAlwaysAckMode();	
	
	nPPID = 0;	
	//initial
	Doc.SetObject();
	JSonCtrl.Set(&Doc);
	
	if ( true == bAckMode )
	{	bAck = true;	}

	if ( BuildIPSDoc(IPS_COMM_STATUS_OPEN_PROJECT, bAck, AckTime, nPPID, Doc) == false )
	{	return false; }
	
	//"Data": {
    //"ProjectName": "ProjectName",    
	//}

	//add object into doc
	rapidjson::WValue object(rapidjson::kObjectType);	
	
	//Project_s;
	if ( NULL == ProjectPtr )
	{	str=_T("");	}
	else
	{	JetAPI::ExtractMainFileNameNoPath(ProjectPtr->GetProjectShowName(), str);	}
	JetAPI::TCHAR2wstring(str, wsBuffer, ConvertCode);	
	bRet=JSonCtrl.AddObject(&object, L"ProjectName", wsBuffer);//ProjectName
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	bRet=JSonCtrl.AddMember(L"Data", object);
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

	DumpMESDoc(Doc, L"BuildIPSDoc_SetProjectOpen");
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::BuildIPSDoc_SetProjectParam(bool bAck, int AckTime, int &nPPID, CAOIProject *ProjectPtr, std::wstring& wsBuf)//建立基本資訊文檔
{	
	bool bRet=true;	
	CString    str;	
	std::wstring wsBuffer;
	rapidjson::WDocument Doc;
	rapidjson::CJsonCtrl JSonCtrl;	
	const UINT ConvertCode = CP_UTF8;	
	LANE_ID LaneID = GetActiveLaneID();
	CAMERA_ID CameraID =  PRIMARY_CAMERA_ID;	
	const bool bAckMode = CheckIPSAlwaysAckMode();	
	
	nPPID = 0;	
	//initial
	Doc.SetObject();
	JSonCtrl.Set(&Doc);
	
	if ( true == bAckMode )
	{	bAck = true;	}

	if ( BuildIPSDoc(IPS_COMM_GEN_INSPECTION_START, bAck, AckTime, nPPID, Doc) == false )
	{	return false; }
	
	//"Data": {
    //"ProjectName": "ProjectName",
    //"Product": "Product",
	//"Lane": 1,
    //"Lot": "LotNumber",    
	//}

	//add object into doc
	rapidjson::WValue object(rapidjson::kObjectType);	
	
	//Project_s;
	if ( NULL == ProjectPtr )
	{	str=_T("");	}
	else
	{	JetAPI::ExtractMainFileNameNoPath(ProjectPtr->GetProjectShowName(), str);	}
	JetAPI::TCHAR2wstring(str, wsBuffer, ConvertCode);	
	bRet=JSonCtrl.AddObject(&object, L"ProjectName", wsBuffer);//ProjectName
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	if ( LANE_ID_B == LaneID ) //Lane
	{	bRet=JSonCtrl.AddObject(&object, L"Lane", 2);	}
	else
	{	bRet=JSonCtrl.AddObject(&object, L"Lane", 1);	}
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}	

	if ( NULL == ProjectPtr )
	{	str=_T("");	}
	else
	{	str=ProjectPtr->GetProjectModuleName(); }
	JetAPI::TCHAR2wstring(str, wsBuffer, ConvertCode);	
	bRet=JSonCtrl.AddObject(&object, L"Product", wsBuffer);//Product
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	if ( NULL == ProjectPtr )
	{	str=_T("");	}
	else
	{	str=ProjectPtr->GetProjectWorkNumber(); }
	JetAPI::TCHAR2wstring(str, wsBuffer, ConvertCode);	
	bRet=JSonCtrl.AddObject(&object, L"Lot", wsBuffer);//LotNumber
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}	

	double FPY = 0.0;//20240531
	if ( NULL != ProjectPtr )
	{	FPY = ProjectPtr->GetProjectResultStatistic().sTest.CalcYielding();	}
	bRet=JSonCtrl.AddObject(&object, L"FPY", FPY);//FPY
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	int TotalParts=0;
	if ( NULL != ProjectPtr )
	{	TotalParts = ProjectPtr->GetProjectComponentNotAgentCount();	}
	bRet=JSonCtrl.AddObject(&object, L"TotalParts", TotalParts);//TotalParts
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	bRet=JSonCtrl.AddMember(L"Data", object);
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

	DumpMESDoc(Doc, L"BuildIPSDoc_SetProjectParam");
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::BuildIPSDoc_SetProjectLoadFinished(bool bAck, int AckTime, int &nPPID, CAOIProject *ProjectPtr, bool bSucc, LPCTSTR Err, std::wstring& wsBuf)//建立基本資訊文檔	
{
	if ( CheckProjectPtr(ProjectPtr) == false )
	{	return false; }

	bool bRet=true;	
	CString    str;	
	int  ResultCode=0;
	std::wstring wsBuffer;
	std::wstring wsResultStr;
	rapidjson::WDocument Doc;
	rapidjson::CJsonCtrl JSonCtrl;	
	const UINT ConvertCode = CP_UTF8;	
	LANE_ID LaneID = GetActiveLaneID();
	CAMERA_ID CameraID =  PRIMARY_CAMERA_ID;	
	const bool bAckMode = CheckIPSAlwaysAckMode();	
	
	nPPID = 0;	
	//initial
	Doc.SetObject();
	JSonCtrl.Set(&Doc);
	
	if ( true == bAckMode )
	{	bAck = true;	}
	if ( true == bSucc )
	{	
		ResultCode = 0;	
		wsResultStr=L"OK";
	}
	else
	{	
		ResultCode = 1; 
		wsResultStr=L"Error";
	}

	if ( BuildIPSDoc(IPS_COMM_STATUS_PROJECT_LOAD_FINISH, bAck, AckTime, nPPID, ResultCode, wsResultStr.c_str(), Doc) == false )
	{	return false; }
	
	//"Data": {
    //"Project": "ProjectName",
    //"Message": "",	
	//}

	//add object into doc
	rapidjson::WValue object(rapidjson::kObjectType);	
	
	//Project;
	JetAPI::ExtractMainFileNameNoPath(ProjectPtr->GetProjectShowName(), str);
	JetAPI::TCHAR2wstring(str, wsBuffer, ConvertCode);	
	bRet=JSonCtrl.AddObject(&object, L"Project", wsBuffer);//Project
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	//Message	
	str=Err;
	JetAPI::TCHAR2wstring(str, wsBuffer, ConvertCode);	
	bRet=JSonCtrl.AddObject(&object, L"Message", wsBuffer);//Message
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}	

	bRet=JSonCtrl.AddMember(L"Data", object);
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

	DumpMESDoc(Doc, L"BuildIPSDoc_SetProjectLoadFinished");
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::BuildIPSDoc_UploadDownloadFile(int nStatus, bool bAck, int AckTime, int &nPPID, LPCTSTR filename, std::wstring& wsBuf)//建立基本資訊文檔
{
	bool bRet=true;	
	CString    str;	
	std::wstring wsBuffer;
	rapidjson::WDocument Doc;
	rapidjson::CJsonCtrl JSonCtrl;	
	const UINT ConvertCode = CP_UTF8;	
	LANE_ID LaneID = GetActiveLaneID();
	CAMERA_ID CameraID =  PRIMARY_CAMERA_ID;	
	const bool bAckMode = CheckIPSAlwaysAckMode();	
	
	nPPID = 0;	
	//initial
	Doc.SetObject();
	JSonCtrl.Set(&Doc);
	
	if ( true == bAckMode )
	{	bAck = true;	}

	if ( BuildIPSDoc(nStatus, bAck, AckTime, nPPID, Doc) == false )
	{	return false; }
	
	//"Data": {
    //"ProjectName": "ProjectName",    
	//}

	//add object into doc
	rapidjson::WValue object(rapidjson::kObjectType);	
	
	//Project_s;
	str = filename;
	JetAPI::TCHAR2wstring(str, wsBuffer, ConvertCode);	
	bRet=JSonCtrl.AddObject(&object, L"ProjectName", wsBuffer);//ProjectName
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	bRet=JSonCtrl.AddMember(L"Data", object);
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

	DumpMESDoc(Doc, L"BuildIPSDoc_UploadDownloadFile");
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::BuildIPSDoc_SetMachineStatus(ONLINE_STATE_MODE Status, bool bAck, int AckTime, int &nPPID, std::wstring& wsBuf)//建立機台運作文檔
{	
	bool bRet=true;	

	CString    str;
	std::wstring wsBuffer;
	const UINT ConvertCode = CP_UTF8;
	rapidjson::WDocument Doc;
	rapidjson::CJsonCtrl JSonCtrl;	
	
	nPPID = 0;	
	//initial
	Doc.SetObject();
	JSonCtrl.Set(&Doc);
	
	IPS_COMM_ID IPSStatusID;
	if ( ONLINE_STATE_APP_CLOSE == Status )
	{	IPSStatusID = IPS_COMM_JET8000_END;	}
	else if ( ONLINE_STATE_APP_OPEN == Status )
	{	IPSStatusID = IPS_COMM_JET8000_START;	}
	else//一律顯示狀態切換
	{	IPSStatusID = IPS_COMM_STATUS;	}	
	IPSStatusID = IPS_COMM_STATUS;
	if ( BuildIPSDoc(IPSStatusID, bAck, AckTime, nPPID, Doc) == false )
	{	return false; }
	
	/*
	//"Data": {
    //"Message": "",    
	*/

	//add object into doc
	rapidjson::WValue object(rapidjson::kObjectType);	
	//add value into the object
	ONLINE_STATE_MODE eOnlineState = Status;

	if ( IPS_COMM_JET8000_END == IPSStatusID )
	{	
		bRet=JSonCtrl.AddObject(&object, L"Message", L"ProgramEnd");//String
		if ( false == bRet ) 
		{
			m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
			return false; 
		}
	}
	else if ( IPS_COMM_JET8000_START == IPSStatusID )
	{	
		bRet=JSonCtrl.AddObject(&object, L"Message", L"ProgramStart");//String
		if ( false == bRet ) 
		{
			m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
			return false; 
		}
	}
	else
	{
		str = AOIDataDefine.GetOnlineStateText(eOnlineState);
		JetAPI::TCHAR2wstring(str, wsBuffer, ConvertCode);
		bRet=JSonCtrl.AddObject(&object, L"Message", wsBuffer);//Message
		if ( false == bRet ) 
		{
			m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
			return false; 
		}		
	}
	
	bRet=JSonCtrl.AddMember(L"Data", object);
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
	
	DumpMESDoc(Doc, L"BuildJSONRaw_SetMachineStatus");
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::BuildIPSDoc_ProcessID(int ProcessID, bool bAck, int AckTime, int &nPPID, std::wstring& wsBuf)//建立程序運作文檔
{	
	bool bRet=false;			
	CString    str;	
	std::wstring wsBuffer;
	rapidjson::WDocument Doc;
	rapidjson::CJsonCtrl JSonCtrl;
	const UINT ConvertCode = CP_UTF8;
	const bool bAckMode = CheckIPSAlwaysAckMode();
	CAOIProject *ProjectPtr = GetActiveProject();

	nPPID=0;	
	//initial
	Doc.SetObject();
	JSonCtrl.Set(&Doc);
	auto &alc = Doc.GetAllocator();

	if ( true == bAckMode )
	{	bAck = true;	}

	if ( BuildIPSDoc(ProcessID, bAck, AckTime, nPPID, Doc) == false )
	{	return false; }	
		
	rapidjson::WValue object(rapidjson::kObjectType);	
		
	str = GetIPSCommText(ProcessID);
	JetAPI::TCHAR2wstring(str, wsBuffer, ConvertCode);
	bRet=JSonCtrl.AddObject(&object, L"Message", wsBuffer);//Message
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	if ( IPS_COMM_STATUS_ALARM_CLEAR==ProcessID )
	{	ResetAOIExceptionCode();	}

	if ( IPS_COMM_STATUS_INSPECTION_END==ProcessID )
	{
		double FPY = 0.0;//20240531
		if ( NULL != ProjectPtr )
		{	FPY = ProjectPtr->GetProjectResultStatistic().sTest.CalcYielding();	}
		bRet=JSonCtrl.AddObject(&object, L"FPY", FPY);//FPY
		if ( false == bRet ) 
		{
			m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
			return false; 
		}
	}

	if ( IPS_COMM_STATUS_PROJECT_LOAD_FINISH == ProcessID )
	{		
		if ( NULL != ProjectPtr )
		{			
			CString ShowName=ProjectPtr->GetProjectShowName();
			JetAPI::ExtractFileNameNoPath(ShowName, str);
			JetAPI::TCHAR2wstring(str, wsBuffer, ConvertCode);
			bRet=JSonCtrl.AddObject(&object, L"Project", wsBuffer);//ProjectList
			if ( false == bRet ) 
			{
				m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
				return false; 
			}
			/*
			const bool bFullName=true;
			std::vector<CString> NameList;
			AOIDataCollect.GetProjectNameList(NameList, bFullName);
			const size_t NameCount=NameList.size();
			rapidjson::WValue NameArray(rapidjson::kArrayType);	
			for ( size_t i=0; i<NameCount; i++ )
			{				
				JetAPI::TCHAR2wstring(NameList[i], wsBuffer, ConvertCode);
				rapidjson::WValue Name(wsBuffer.c_str(), wsBuffer.length(), alc);//使用此方法深層複製字串內容		
				NameArray.PushBack(Name, alc);
			}
			bRet=JSonCtrl.AddObject(&object, L"ProjectList", NameArray);//ProjectList
			if ( false == bRet ) 
			{
				m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
				return false; 
			}		
			*/
		}
	}

	bRet=JSonCtrl.AddMember(L"Data", object);
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}	
	
	bRet = JSonCtrl.GetBuffer(wsBuf, Doc);
	if ( false == bRet ) { return false; }

	DumpMESDoc(Doc, L"BuildJSONRaw_SetStatus");
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::BuildIPSDoc_SetAOIExceptionCode(bool bAck, int AckTime, LPCTSTR ErrStr, int &nPPID, std::wstring& wsBuf)//執行MES溝通-系統異常碼
{	
	bool bRet=true;	

	CString    str;		
	std::wstring wsBuffer;
	rapidjson::WDocument Doc;
	rapidjson::CJsonCtrl JSonCtrl;		
	const UINT ConvertCode = CP_UTF8;
	const int  nStatus = IPS_COMM_ALARM;
	const bool bAckMode = CheckIPSAlwaysAckMode();		
	const int  nExceptionCode=GetAOIExceptionCode();		

	nPPID = 0;	
	//initial
	Doc.SetObject();
	JSonCtrl.Set(&Doc);
	ResetAOIExceptionCode();

	if ( true == bAckMode )
	{	bAck = true;	}

	if ( BuildIPSDoc(nStatus, bAck, AckTime, nPPID, Doc) == false )
	{	return false; }
	
	//"Data": {
	//"ID": 9999,
    //"Message": "UnDefine",    
	//"Description": "Error",
	//}

	//add object into doc
	rapidjson::WValue object(rapidjson::kObjectType);	

	bRet=JSonCtrl.AddObject(&object, L"ID", nExceptionCode);//ID
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	str = GetAOIExceptionText();
	JetAPI::TCHAR2wstring(str, wsBuffer, ConvertCode);	
	bRet=JSonCtrl.AddObject(&object, L"Message", wsBuffer);//Message
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	str = ErrStr;
	JetAPI::TCHAR2wstring(str, wsBuffer, ConvertCode);	
	bRet=JSonCtrl.AddObject(&object, L"Description", wsBuffer);//Description
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	bRet=JSonCtrl.AddMember(L"Data", object);
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

	m_AOIExceptionCodeLast = nExceptionCode;
	m_AOIExceptionCodeTickCount = GetTickCount();
	DumpMESDoc(Doc, L"BuildJSONRaw_SetAOIExceptionCode");
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::BuildIPSDoc_SetLogin_out(bool bAck, int AckTime, int &nPPID, bool Login, std::wstring& wsBuf)//建立登入參數文檔
{	
	bool bRet=true;		

	int        nVal=0;
	CString    str;
	std::wstring wsBuffer;
	rapidjson::WDocument Doc;
	rapidjson::CJsonCtrl JSonCtrl;
	const UINT ConvertCode = CP_UTF8;		
	const bool bAckMode = CheckIPSAlwaysAckMode();
	TUserNode  UserNode = AOIDataCollect.GetLoginUserNode();//使用者資料
	
	nPPID = 0;	
	//initial
	Doc.SetObject();
	JSonCtrl.Set(&Doc);
	
	if ( true == bAckMode )
	{	bAck = true;	}

	if ( BuildIPSDoc(IPS_COMM_GEN_USER_DATA, bAck, AckTime, nPPID, Doc) == false )
	{	return false; }
	
	//add object into doc
	rapidjson::WValue object(rapidjson::kObjectType);	
	//add value into the object	
	str = UserNode.wUserName;//MES登入名稱
	JetAPI::TCHAR2wstring(str, wsBuffer, ConvertCode);	
	bRet=JSonCtrl.AddObject(&object, L"UserName", wsBuffer);
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}
	
	str = UserNode.wPassword;//MES登入密碼
	JetAPI::TCHAR2wstring(str, wsBuffer, ConvertCode);	
	bRet=JSonCtrl.AddObject(&object, L"PassWord", wsBuffer);
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}
	
	const int Permission_Admin = 1;
	const int Permission_FAE   = 2;
	const int Permission_OP    = 3;
	switch ( UserNode.eUserLevel )
	{
	case USER_LEVEL_OPERATOR: nVal=Permission_OP;	break;
	case USER_LEVEL_ENGINEER: nVal=Permission_FAE;	break;
	case USER_LEVEL_SUPERVISOR: nVal=Permission_FAE;	break;
	case USER_LEVEL_JET_FAE: nVal=Permission_FAE;	break;
	case USER_LEVEL_JET_SENIOR: nVal=Permission_FAE;	break;
	case USER_LEVEL_JET_RD: nVal=Permission_Admin;	break;
	default:				nVal = 0;	break;
	}	
	bRet=JSonCtrl.AddObject(&object, L"Permissions", nVal);
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	if ( true==Login ) { nVal = 1; }
	else { nVal = 2; }
	bRet=JSonCtrl.AddObject(&object, L"Login", nVal);
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	/*				
	//"Insp_Result_File":"C:\\ABC\\1234.json" 
	*/

	bRet=JSonCtrl.AddMember(L"Data", object);
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

	DumpMESDoc(Doc, L"BuildJSONRaw_Login_out");
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::BuildIPSDoc_SetControlStateMode(bool bAck, int AckTime, int &nPPID, int Mode, std::wstring& wsBuf)//建立控制狀態文檔	
{
	nPPID = 0;
	int  nStatus = 0;	
	switch ( Mode )
	{
	case MES_EQP_CTRL_STATE_LOCAL:	nStatus = IPS_COMM_STATUS_LOCAL_ONLINE;	break;
	case MES_EQP_CTRL_STATE_REMOTE:	nStatus = IPS_COMM_STATUS_ONLINE;		break;
	default:
	case MES_EQP_CTRL_STATE_OFFLINE:nStatus = IPS_COMM_STATUS_OFFLINE;		break;
	}
	SetEqpOnlineMode(nStatus);
	return BuildIPSDoc_ProcessID(nStatus, bAck, AckTime, nPPID, wsBuf);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::BuildIPSDoc_CheckBarcode(bool bAck, int AckTime, int &nPPID, CAOIProject *ProjectPtr, std::wstring& wsBuf)//建立確認條碼文檔
{	
	if ( CheckProjectPtr(ProjectPtr) == false )
	{	return false; }	

	bool bRet=true;
	int        nVal=0;
	CString    str;
	std::wstring wsKey;
	std::wstring wsBuffer;	
	rapidjson::WDocument Doc;
	rapidjson::CJsonCtrl JSonCtrl;
	const UINT ConvertCode = CP_UTF8;		
	const bool bAckMode = CheckIPSAlwaysAckMode();			

	nPPID = 0;	
	//initial
	Doc.SetObject();
	JSonCtrl.Set(&Doc);
	auto &alc = Doc.GetAllocator();

	if ( true == bAckMode )
	{	bAck = true;	}

	if ( BuildIPSDoc(IPS_COMM_GEN_CHECK_BARCODE, bAck, AckTime, nPPID, Doc) == false )
	{	return false; }
	
	//add object into doc		

	size_t     i=0, j=0;
	int        PanelIdx=0;
	int        BoardIdx=0;		
	int        BarcodeCount=0;		
	CAOIPanel *PanelPtr = NULL;
	CAOIBoard *BoardPtr = NULL;	
	rapidjson::WValue ProjectObj(rapidjson::kObjectType);	
	
	if ( ProjectPtr->GetProjectIsGetBarcode() == false )
	{	wsBuffer = L"";	}
	else
	{	wsBuffer = ProjectPtr->GetProjectBarcode();	}		
	bRet=JSonCtrl.AddObject(&ProjectObj, L"ProjectBarCode", wsBuffer);
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}	

	wsBuffer = ProjectPtr->GetProjectTrayBarcode();
	bRet=JSonCtrl.AddObject(&ProjectObj, L"Tray", wsBuffer);
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	wsBuffer = ProjectPtr->GetProjectCoverBarcode();
	bRet=JSonCtrl.AddObject(&ProjectObj, L"Cover", wsBuffer);
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	wsBuffer = ProjectPtr->GetProjectWorkNumber();
	bRet=JSonCtrl.AddObject(&ProjectObj, L"Lot", wsBuffer);
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	rapidjson::WValue PanelArray(rapidjson::kArrayType);
	const size_t PanelCount=ProjectPtr->GetProjectPanelCount();
	for ( i=0; i<PanelCount; i++ )
	{
		PanelPtr = ProjectPtr->GetProjectPanelPtr(i, false);
		if ( NULL == PanelPtr ) { continue; }

		//"ID": 1,
        //"Barcode": "PanelBarcode",        
		//"BoardList": []
		rapidjson::WValue PanelObject(rapidjson::kObjectType);
		bRet=JSonCtrl.AddObject(&PanelObject, L"ID", i+1);//PanelID
		if ( false == bRet ) 
		{
			m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
			return false; 
		}

		if ( PanelPtr->GetPanelIsGetBarcode() == false )
		{	wsBuffer = L"";	}
		else
		{	wsBuffer = PanelPtr->GetPanelBarcode();	}		
		bRet=JSonCtrl.AddObject(&PanelObject, L"Barcode", wsBuffer);//PanelBarcode
		if ( false == bRet ) 
		{
			m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
			return false; 
		}

		rapidjson::WValue BoardArray(rapidjson::kArrayType);		
		const size_t PanelBoardCount = PanelPtr->GetPanelBoardCount();
		for ( j=0; j<PanelBoardCount; j++ )
		{
			BoardPtr = PanelPtr->GetPanelBoardPtr(j, false);
			if ( NULL == BoardPtr ) { continue; }
			
			//"ID": 1,
            //"Barcode": "BoardBarcode",
			rapidjson::WValue BoardObject(rapidjson::kObjectType);
			bRet=JSonCtrl.AddObject(&BoardObject, L"ID", j+1);//BoardID
			if ( false == bRet ) 
			{
				m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
				return false; 
			}

			if ( BoardPtr->GetBoardIsGetBarcode() == false )
			{	wsBuffer = L"";	}
			else
			{	wsBuffer = BoardPtr->GetBoardBarcode(); }
			bRet=JSonCtrl.AddObject(&BoardObject, L"Barcode", wsBuffer);//BoardBarcode
			if ( false == bRet ) 
			{
				m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
				return false; 
			}
			BoardArray.PushBack(BoardObject, alc);
		}		
		
		bRet=JSonCtrl.AddObject(&PanelObject, L"BoardList", BoardArray);//Board List
		if ( false == bRet ) 
		{
			m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
			return false; 
		}

		PanelArray.PushBack(PanelObject, alc);
	}
	bRet=JSonCtrl.AddObject(&ProjectObj, L"PanelList", PanelArray);//Panel List	

	bRet=JSonCtrl.AddMember(L"Data", ProjectObj);
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

	DumpMESDoc(Doc, L"BuildJSONRaw_CheckBarcode");
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::BuildIPSDoc_AskBarcode(bool bAck, int AckTime, int &nPPID, CAOIProject *ProjectPtr, std::wstring& wsBuf)//建立設定條碼文檔
{
	bool bRet=true;	
	int        nVal=0;
	CString    str;
	std::wstring wsKey;
	std::wstring wsBuffer;	
	rapidjson::WDocument Doc;
	rapidjson::CJsonCtrl JSonCtrl;
	const UINT ConvertCode = CP_UTF8;		
	const bool bAckMode = CheckIPSAlwaysAckMode();			

	nPPID = 0;	
	//initial
	Doc.SetObject();
	JSonCtrl.Set(&Doc);
	auto &alc = Doc.GetAllocator();

	if ( true == bAckMode )
	{	bAck = true;	}

	if ( BuildIPSDoc(IPS_COMM_ASK_BARCODE, bAck, AckTime, nPPID, Doc) == false )
	{	return false; }
	
	//add object into doc		

	size_t     i=0, j=0;
	int        PanelIdx=0;
	int        BoardIdx=0;		
	int        BarcodeCount=0;		
	CAOIPanel *PanelPtr = NULL;
	CAOIBoard *BoardPtr = NULL;		
	rapidjson::WValue ProjectObj(rapidjson::kObjectType);	
	const size_t PanelCount=ProjectPtr->GetProjectPanelCount();
	const size_t BoardCount=ProjectPtr->GetProjectBoardCount();
	
	if ( ProjectPtr->GetProjectIsGetBarcode() == false )
	{	wsBuffer = L"";	}
	else
	{	wsBuffer = ProjectPtr->GetProjectBarcode();	}		
	bRet=JSonCtrl.AddObject(&ProjectObj, L"ProjectBarCode", wsBuffer);
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}	

	wsBuffer = ProjectPtr->GetProjectTrayBarcode();
	bRet=JSonCtrl.AddObject(&ProjectObj, L"Tray", wsBuffer);
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}
	
	wsBuffer = ProjectPtr->GetProjectCoverBarcode();
	bRet=JSonCtrl.AddObject(&ProjectObj, L"Cover", wsBuffer);
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	wsBuffer = ProjectPtr->GetProjectWorkNumber();
	bRet=JSonCtrl.AddObject(&ProjectObj, L"Lot", wsBuffer);
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	rapidjson::WValue PanelArray(rapidjson::kArrayType);	
	for ( i=0; i<PanelCount; i++ )
	{
		PanelPtr = ProjectPtr->GetProjectPanelPtr(i, false);
		if ( NULL == PanelPtr ) { continue; }

		//"ID": 1,
        //"Barcode": "PanelBarcode",        
		//"BoardList": []
		rapidjson::WValue PanelObject(rapidjson::kObjectType);
		bRet=JSonCtrl.AddObject(&PanelObject, L"ID", i+1);//PanelID
		if ( false == bRet ) 
		{
			m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
			return false; 
		}

		if ( PanelPtr->GetPanelIsGetBarcode() == false )
		{	wsBuffer = L"";	}
		else
		{	wsBuffer = PanelPtr->GetPanelBarcode();	}		
		bRet=JSonCtrl.AddObject(&PanelObject, L"Barcode", wsBuffer);//PanelBarcode
		if ( false == bRet ) 
		{
			m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
			return false; 
		}

		rapidjson::WValue BoardArray(rapidjson::kArrayType);		
		const size_t PanelBoardCount = PanelPtr->GetPanelBoardCount();
		for ( j=0; j<PanelBoardCount; j++ )
		{
			BoardPtr = PanelPtr->GetPanelBoardPtr(j, false);
			if ( NULL == BoardPtr ) { continue; }
			
			//"ID": 1,
            //"Barcode": "BoardBarcode",
			rapidjson::WValue BoardObject(rapidjson::kObjectType);
			bRet=JSonCtrl.AddObject(&BoardObject, L"ID", j+1);//BoardID
			if ( false == bRet ) 
			{
				m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
				return false; 
			}

			if ( BoardPtr->GetBoardIsGetBarcode() == false )
			{	wsBuffer = L"";	}
			else
			{	wsBuffer = BoardPtr->GetBoardBarcode(); }
			bRet=JSonCtrl.AddObject(&BoardObject, L"Barcode", wsBuffer);//BoardBarcode
			if ( false == bRet ) 
			{
				m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
				return false; 
			}
			BoardArray.PushBack(BoardObject, alc);
		}		
		
		bRet=JSonCtrl.AddObject(&PanelObject, L"BoardList", BoardArray);//Board List
		if ( false == bRet ) 
		{
			m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
			return false; 
		}

		PanelArray.PushBack(PanelObject, alc);
	}
	bRet=JSonCtrl.AddObject(&ProjectObj, L"PanelList", PanelArray);//Panel List	

	bRet=JSonCtrl.AddMember(L"Data", ProjectObj);
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
	DumpMESDoc(Doc, L"BuildJSONRaw_AskBarcode");	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::BuildIPSDoc_BoardMapping(bool bAck, int AckTime, int &nPPID, CAOIProject *ProjectPtr, std::wstring& wsBuf)//建立單板映射文檔
{
	if ( CheckProjectPtr(ProjectPtr) == false )
	{	return false; }	

	bool bRet=true;
	int        nVal=0;
	CString    str;
	std::wstring wsKey;
	std::wstring wsBuffer;	
	rapidjson::WDocument Doc;
	rapidjson::CJsonCtrl JSonCtrl;
	const UINT ConvertCode = CP_UTF8;		
	const bool bAckMode = CheckIPSAlwaysAckMode();			

	nPPID = 0;	
	//initial
	Doc.SetObject();
	JSonCtrl.Set(&Doc);
	auto &alc = Doc.GetAllocator();

	if ( true == bAckMode )
	{	bAck = true;	}

	if ( BuildIPSDoc(IPS_COMM_ASK_XBOARD_MAPPING, bAck, AckTime, nPPID, Doc) == false )
	{	return false; }
	
	//add object into doc		

	size_t     i=0, j=0;
	int        PanelIdx=0;
	int        BoardIdx=0;		
	int        BarcodeCount=0;		
	CAOIPanel *PanelPtr = NULL;
	CAOIBoard *BoardPtr = NULL;	
	rapidjson::WValue ProjectObj(rapidjson::kObjectType);	
	
	rapidjson::WValue PanelArray(rapidjson::kArrayType);
	const size_t PanelCount=ProjectPtr->GetProjectPanelCount();
	for ( i=0; i<PanelCount; i++ )
	{
		PanelPtr = ProjectPtr->GetProjectPanelPtr(i, false);
		if ( NULL == PanelPtr ) { continue; }

		const int BoardColCount = PanelPtr->GetPanelBoardColCount();
		const int BoardRowCount = PanelPtr->GetPanelBoardRowCount();
		const int BoardColBlockCount=PanelPtr->GetPanelBoardColBlockCount();
		const int TotalRowCount=BoardRowCount;
		const int TotalColCount=BoardColCount*BoardColBlockCount;

		//"ID": 1,		
		//"X": nCols,
		//"Y": nRows,
        //"Barcode": "PanelBarcode",
		//"BoardList": []
		rapidjson::WValue PanelObject(rapidjson::kObjectType);
		bRet=JSonCtrl.AddObject(&PanelObject, L"ID", i+1);//PanelID
		if ( false == bRet ) 
		{
			m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
			return false; 
		}
		bRet=JSonCtrl.AddObject(&PanelObject, L"X", TotalColCount);//X
		if ( false == bRet ) 
		{
			m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
			return false; 
		}
		bRet=JSonCtrl.AddObject(&PanelObject, L"Y", TotalRowCount);//Y
		if ( false == bRet ) 
		{
			m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
			return false; 
		}		

		if ( PanelPtr->GetPanelIsGetBarcode() == false )
		{	wsBuffer = L"";	}
		else
		{	wsBuffer = PanelPtr->GetPanelBarcode();	}		
		bRet=JSonCtrl.AddObject(&PanelObject, L"Barcode", wsBuffer);//PanelBarcode
		if ( false == bRet ) 
		{
			m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
			return false; 
		}

		rapidjson::WValue BoardArray(rapidjson::kArrayType);		
		const size_t PanelBoardCount = PanelPtr->GetPanelBoardCount();
		for ( j=0; j<PanelBoardCount; j++ )		
		{			
			BoardPtr = PanelPtr->GetPanelBoardPtr(j, false);
			if ( NULL == BoardPtr ) { continue; }			
			
			int X = 1;
			int Y = 1;
			int BoardState=0;
			//"ID": 1,			
            //"BoardState": "0",//0-Test, 1-Skip
			rapidjson::WValue BoardObject(rapidjson::kObjectType);
			bRet=JSonCtrl.AddObject(&BoardObject, L"ID", j+1);//BoardID			
			if ( false == bRet ) 
			{
				m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
				return false; 
			}

			if ( BoardPtr->CheckBoardBypassedSkipped() == true )
			{	BoardState = IPS_BOARD_MAPPING_SKIP;	}
			else
			{	BoardState = IPS_BOARD_MAPPING_TEST;	}
			bRet=JSonCtrl.AddObject(&BoardObject, L"BoardState", BoardState);//BoardState
			if ( false == bRet ) 
			{
				m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
				return false; 
			}
			BoardArray.PushBack(BoardObject, alc);
		}		
		
		bRet=JSonCtrl.AddObject(&PanelObject, L"BoardList", BoardArray);//Board List
		if ( false == bRet ) 
		{
			m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
			return false; 
		}

		PanelArray.PushBack(PanelObject, alc);
	}
	bRet=JSonCtrl.AddObject(&ProjectObj, L"PanelList", PanelArray);//Panel List	

	bRet=JSonCtrl.AddMember(L"Data", ProjectObj);
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

	DumpMESDoc(Doc, L"BuildJSONRaw_BoardMapping");
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::BuildIPSDoc_UnCheckTestFile(bool bAck, int AckTime, int &nPPID, LANE_ID LaneID, std::wstring& wsBuf)//建立未判定檢測檔案數文檔	
{	
	bool bRet=true;
	int        nVal=0;
	CString    str;
	std::wstring wsKey;
	std::wstring wsBuffer;	
	rapidjson::WDocument Doc;
	rapidjson::CJsonCtrl JSonCtrl;
	const UINT ConvertCode = CP_UTF8;		
	const bool bAckMode = CheckIPSAlwaysAckMode();			

	nPPID = 0;	
	//initial
	Doc.SetObject();
	JSonCtrl.Set(&Doc);
	auto &alc = Doc.GetAllocator();

	if ( true == bAckMode )
	{	bAck = true;	}

	if ( BuildIPSDoc(IPS_COMM_ASK_UNCHECK_TEST_FILE, bAck, AckTime, nPPID, Doc) == false )
	{	return false; }
	
	//add object into doc		
	const TSystemParameter &SysParam = AOIDataCollect.GetSystemParameter();	
	rapidjson::WValue object(rapidjson::kObjectType);			

	str = SysParam.m_MachineStation;
	//str = SysParam.m_MachineStation_LB;
	JetAPI::TCHAR2wstring(str, wsBuffer, ConvertCode);	
	bRet=JSonCtrl.AddObject(&object, L"Station", wsBuffer);//Station
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	str = SysParam.m_MachineLine;
	//str = SysParam.m_MachineLine_LB;
	JetAPI::TCHAR2wstring(str, wsBuffer, ConvertCode);	
	bRet=JSonCtrl.AddObject(&object, L"Line", wsBuffer);//Line
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}		

	str.Format(_T("%d"), LaneID);
	JetAPI::TCHAR2wstring(str, wsBuffer, ConvertCode);	
	bRet=JSonCtrl.AddObject(&object, L"Lane", wsBuffer);//Lane
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	bRet=JSonCtrl.AddMember(L"Data", object);
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

	DumpMESDoc(Doc, L"BuildJSONRaw_UnCheckTestFile");
	return true;
}
//-------------------------------------------------------------------------------------//
CString CMES_IPS::GetIPSCommText(int CommID)
{
	CString str;
	switch ( CommID )
	{
	case IPS_COMM_NONE:							
		str = _T("None");	
		break;

	case IPS_COMM_GEN:							str = _T("Setting");	break;
	case IPS_COMM_GEN_SYSTEM_SETUP:				str = _T("Setup");	break;		
	case IPS_COMM_GEN_INSPECTION_START:         str = _T("Start");	break;		
	case IPS_COMM_GEN_CUSTOMER_SETTING:			str = _T("CustomerSetting");	break;
	case IPS_COMM_GEN_CHECK_BARCODE:			str = _T("CheckBarcode");	break;
	case IPS_COMM_GEN_PROJECT_UPLOAD:			str = _T("ProjectUpload");	break;
	case IPS_COMM_GEN_PROJECT_DOWNLOAD:			str = _T("ProjectDownload");	break;
	case IPS_COMM_GEN_USER_DATA:			    str = _T("User Data");	break;
	case IPS_COMM_GEN_GET_STATUS:			    str = _T("GetStatus");	break;

	case IPS_COMM_ASK:							str = _T("Ask");	break;
	case IPS_COMM_ASK_CUSTOMER_ASK:				str = _T("CustomerAsk");	break;
	case IPS_COMM_ASK_BARCODE:					str = _T("AskBarcode");	break;
	case IPS_COMM_ASK_CHECK_CONTINUE:			str = _T("CheckContinue");	break;
	case IPS_COMM_ASK_XBOARD_MAPPING:			str = _T("Mapping");	break;		
	case IPS_COMM_ASK_UNCHECK_TEST_FILE:		str = _T("UnCheckTestFile");	break;	
		                             
	case IPS_COMM_STATUS:						str = _T("Status");	break;
	case IPS_COMM_STATUS_READY_TO_LOAD:			str = _T("ReadyToLoad");	break;
	case IPS_COMM_STATUS_LOAD_COMPLETE:			str = _T("LoadComplete");	break;
	case IPS_COMM_STATUS_READY_TO_UNLOAD:		str = _T("ReadyToUnload");	break;
	case IPS_COMM_STATUS_UNLOAD_COMPLETE:		str = _T("UnloadComplete");	break;
	case IPS_COMM_STATUS_INSPECTION_STOP:		str = _T("InspectionStop");	break;
	case IPS_COMM_STATUS_OFFLINE:				str = _T("Offline");	break;
	case IPS_COMM_STATUS_ONLINE:				str = _T("Online");	break;
	case IPS_COMM_STATUS_PARAM_CHANGE:			str = _T("ParamChange");	break;
	case IPS_COMM_STATUS_PROJECT_LOAD_FINISH:	str = _T("ProjectLoadFinish");	break;
	case IPS_COMM_STATUS_ALARM_CLEAR:			str = _T("AlarmClear");	break;
	
	case IPS_COMM_STATUS_INSPECTION_END:		str = _T("End");	break;
	case IPS_COMM_STATUS_IDLE:					str = _T("Idle");	break;
	case IPS_COMM_STATUS_LOCAL_ONLINE:		    str = _T("LocalOnline");	break;
	case IPS_COMM_STATUS_STOP:					str = _T("Stop");	break;
	case IPS_COMM_STATUS_EDIT:					str = _T("Edit");	break;
	case IPS_COMM_STATUS_OPEN_PROJECT:			str = _T("OpenProject");	break;
	case IPS_COMM_STATUS_TOWER_LIGHT:			str = _T("TowerLights");	break;
	case IPS_COMM_STATUS_ONLINE_TEST:			str = _T("OnlineTest");	break;

	case IPS_COMM_ALARM:						str = _T("Alarm");	break;

	case IPS_COMM_REMOTE:						str = _T("RemoteControl");	break;
	case IPS_COMM_REMOTE_ON:					str = _T("RCMD_On");	break;
	case IPS_COMM_REMOTE_OFF:					str = _T("RCMD_Off");	break;
	case IPS_COMM_REMOTE_START:					str = _T("RCMD_Start");	break;
	case IPS_COMM_REMOTE_STOP:					str = _T("RCMD_Stop");	break;
	case IPS_COMM_REMOTE_PAUSE:					str = _T("RCMD_Pause");	break;
	case IPS_COMM_REMOTE_RESUME:				str = _T("RCMD_Resume");	break;
	case IPS_COMM_REMOTE_ABORT:					str = _T("RCMD_Abort");	break;

	case IPS_COMM_REMOTE_PROJECT_LOAD:			str = _T("RCMD_ProjectLoad");	break;
	case IPS_COMM_REMOTE_PROJECT_LIST:			str = _T("RCMD_ProjectList");	break;
	case IPS_COMM_REMOTE_PROJECT_UPLOAD:		str = _T("RCMD_ProjectUpload");	break;
	case IPS_COMM_REMOTE_UPLOAD_FINISHED:		str = _T("RCMD_UploadFinished");	break;
	case IPS_COMM_REMOTE_PROJECT_DOWNLOAD:		str = _T("RCMD_ProjectDownload");	break;
	case IPS_COMM_REMOTE_DOWNLOAD_FINISHED:		str = _T("RCMD_DownloadFinished");	break;
	case IPS_COMM_REMOTE_PROJECT_DELETE:		str = _T("RCMD_ProjectDelete");	break;
	case IPS_COMM_REMOTE_DELETE_FINISHED:		str = _T("RCMD_DeleteFinished");	break;
	
	case IPS_COMM_REMOTE_PARAM_UPLOAD:			str = _T("RCMD_ParamLoad");	break;
	case IPS_COMM_REMOTE_PARAM_UPLOAD_FINISHED:	str = _T("RCMD_ParamUploadFinished");	break;
	case IPS_COMM_REMOTE_PARAM_DOWNLOAD:		str = _T("RCMD_ParamDownload");	break;
	case IPS_COMM_REMOTE_PARAM_DOWNLOAD_FINISHED:	str = _T("RCMD_ParamDownloadFinished");	break;

	case IPS_COMM_REMOTE_SET_LOT_NUMBER:		str = _T("RCMD_LotSetting");	break;

	case IPS_COMM_PCS_CONTROL:					str = _T("PCS_Contrl");	break;		
	case IPS_COMM_PCS_SHOW_MESSAGE:				str = _T("PCS_ShowMessage");	break;		
	case IPS_COMM_PCS_UPLOAD_FINISHED:			str = _T("PCS_UploadFinished");	break;		
	case IPS_COMM_PCS_DOWNLOAD_FINISHED:		str = _T("PCS_DownloadFinished");	break;				                     
	
	case IPS_COMM_JET8000_END:					str = _T("JET8000End");	break;
	case IPS_COMM_JET8000_START:				str = _T("JET8000Start");	break;
	
	case IPS_COMM_RETURN:
		str = _T("Return");
		break;
	default:
		str.Format(_T("IPS Comm Undefined[%d]"), CommID);
		break;
	}
	return str;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::GetAckFilename(CString &Filename)//取回Ack回傳的檔名
{
	CString MainName, ExtName;	
	JetAPI::ExtractMainFileName(Filename, MainName);
	JetAPI::ExtractExtendFileName(Filename, ExtName);
	Filename.Format(_T("%s_ACK.%s"), MainName, ExtName);
	return true;
}
//-------------------------------------------------------------------------------------//
int CMES_IPS::MapIPSComm_ProcessID(int ProcessID)//映射ProcessID
{	
	int NewProcessID=0;
	switch ( ProcessID )
	{
	case MES_STATAUS_AOI_READY_TO_LOAD_CMD: NewProcessID=IPS_COMM_STATUS_READY_TO_LOAD;	break;
	case MES_STATAUS_AOI_LOAd_COMPLETE_CMD: NewProcessID=IPS_COMM_STATUS_LOAD_COMPLETE;	break;
	case MES_STATAUS_AOI_START_INSPECTION_CMD: NewProcessID=IPS_COMM_GEN_INSPECTION_START;	break;
	case MES_STATAUS_AOI_INSPECTION_COMPLETE_CMD: NewProcessID=IPS_COMM_STATUS_INSPECTION_END;	break;

	case MES_STATAUS_AOI_READY_TO_UNLOAD_CMD: NewProcessID=IPS_COMM_STATUS_READY_TO_UNLOAD;	break;
	case MES_STATAUS_AOI_UNLOAd_COMPLETE_CMD: NewProcessID=IPS_COMM_STATUS_UNLOAD_COMPLETE;	break;	
	case MES_STATAUS_AOI_INSPECTION_STOP_CMD: NewProcessID=IPS_COMM_STATUS_INSPECTION_STOP;	break;

	case MES_STATAUS_AOI_PARAM_CHANGE_CMD: NewProcessID=IPS_COMM_STATUS_PARAM_CHANGE;	break;		
	case MES_STATAUS_AOI_PROJECT_LOAD_FINISH: NewProcessID=IPS_COMM_STATUS_PROJECT_LOAD_FINISH;	break;		
	case MES_STATAUS_AOI_ALARM_CLEAR: NewProcessID=IPS_COMM_STATUS_ALARM_CLEAR;	break;
	case MES_STATAUS_AOI_PROJECT_OPEN: NewProcessID=IPS_COMM_STATUS_OPEN_PROJECT;	break;
	case MES_STATAUS_AOI_EDIT_MODE: NewProcessID=IPS_COMM_STATUS_EDIT;	break;
	case MES_STATAUS_AOI_ONLINE_TEST: NewProcessID=IPS_COMM_STATUS_ONLINE_TEST;	break;

	default:
		NewProcessID=IPS_COMM_NONE;
		break;
	}	
	return NewProcessID;
}
//-------------------------------------------------------------------------------------//
CString CMES_IPS::ExtractFilenameDateTime(LPCTSTR Filename)//萃取檔案名稱上的日期時間
{
	CString MainName;	
	if ( JetAPI::ExtractMainFileName(Filename, MainName) == false )
	{	return CString(Filename); }

	CString DateTime;
	const int DateTimeLen=19;//"YYYYMMDDhhmmss_0000"		
	const int nLeft=MainName.Find('_');
	if ( nLeft < 0 ) 
	{	return CString(Filename);	}
	DateTime = MainName.Mid(nLeft+1, DateTimeLen);	
	if ( DateTime.GetLength() != DateTimeLen )
	{	return CString(Filename); }		
	return DateTime;
}
//-------------------------------------------------------------------------------------//
CString CMES_IPS::GetIPS_RemoteControlErrorText(int ErrorCode)//取得遠端控制錯誤訊息
{
	CString str;
	switch ( ErrorCode )
	{
	case IPS_REMOTE_CONTROL_ERROR_SUCCESS: str=_T("OK");	break;
	case IPS_REMOTE_CONTROL_ERROR_SYSTEM_NOT_READY: str=_T("Not Ready");	break;
	case IPS_REMOTE_CONTROL_ERROR_NO_SUPPORT_FUNC: str=_T("No Support");	break;
	case IPS_REMOTE_CONTROL_ERROR_PROJECT_CLOSED: str=_T("Project No Opened");	break;
	case IPS_REMOTE_CONTROL_ERROR_ON_RUNNING: str=_T("Running");	break;
	case IPS_REMOTE_CONTROL_ERROR_ON_STOPPING: str=_T("Stopping");	break;	
	case IPS_REMOTE_CONTROL_ERROR_PROJECT_CREATE: str=_T("Project Create Fault");	break;
	case IPS_REMOTE_CONTROL_ERROR_PROJECT_OPEN: str=_T("Project Open Fault");	break;
	case IPS_REMOTE_CONTROL_ERROR_ONLINE: str=_T("Online Mode"); break;
	case IPS_REMOTE_CONTROL_ERROR_OFFLINE: str=_T("Offline Mode"); break;
	case IPS_REMOTE_CONTROL_ERROR_JSON_DATA_EXCEPTION: str=_T("Json Data Exception"); break;
	case IPS_REMOTE_CONTROL_ERROR_PROJECT_NOT_ALLOW: str=_T("Project Not Allow"); break;
	case IPS_REMOTE_CONTROL_ERROR_REMOTE_CTRL_DISABLE: str=_T("Remote Ctrl Disable"); break;
	default:
		str.Format(_T("Unexception Code(%d)"), ErrorCode);
		break;
	}
	return str;
}
//-------------------------------------------------------------------------------------//
void CMES_IPS::GetIPS_RemoteControlErrorText(int ErrorCode, std::wstring &ErrorStr)//取得遠端控制錯誤訊息
{
	CString str=GetIPS_RemoteControlErrorText(ErrorCode);	
	JetAPI::TCHAR2wstring(str, ErrorStr, CP_UTF8);	
	return;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::CreateMESProcThread()//建立MES執行執行緒
{
	return CreateIPSProcThread();
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::DeleteMESProcThread()//刪除MES執行執行緒
{
	return DeleteIPSProcThread();
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::GetMESConnected()//是否已經連線
{
	return GetIPSLinker().GetITSConnected();
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::ConnectMESLinker()//連線到MES連結軟體
{
	return ConnectIPSLinker();
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::DisconnectMESLinker()//停止連線到MES連結軟體
{
	SetMesRemotCtrlOn(false);
	ResetMesEqpCtrlStateMode();
	return DisconnectIPSLinker();
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::ExecMESComm_CheckMESReady()//執行MES溝通-確認MES就緒
{
	return ExecIPSComm_CheckMESReady();
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::ExecMESComm_SetSystemParam()//執行MES溝通-設定系統參數
{	
	return ExecIPSComm_SetSystemParam();
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::ExecMESComm_SetProjectParam(CAOIProject *ProjectPtr)//執行MES溝通-設定專案參數
{	
	return true;//改成開始檢測時才發送
	return ExecIPSComm_SetProjectParam(ProjectPtr);
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::ExecMESComm_SetProjectLoadFinished(CAOIProject *ProjectPtr, bool bSucc, LPCTSTR Err)//執行MES溝通-專案載入完畢
{
	return ExecIPSComm_SetProjectLoadFinished(ProjectPtr, bSucc, Err);
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::ExecMESComm_UploadProjectFile(LPCTSTR filename)//執行MES溝通-上傳專案檔案
{
	return ExecIPSComm_UploadProjectFile(filename);	
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::ExecMESComm_DownloadProjectFile(LPCTSTR filename)//執行MES溝通-下載專案檔案
{
	return ExecIPSComm_DownloadProjectFile(filename);	
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::ExecMESComm_SetMachineStatus()//執行MES溝通-設定機台狀態
{	
	return ExecIPSComm_SetMachineStatus();
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::ExecMESComm_SetMachineStatus(ONLINE_STATE_MODE Status)//執行MES溝通-設定機台狀態		
{	
	return ExecIPSComm_SetMachineStatus(Status);
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::ExecMESComm_ProcessID(int ProcessID)//執行MES溝通-程序運作
{	
	const int IPS_ProcessID=MapIPSComm_ProcessID(ProcessID);	
	if ( IPS_COMM_GEN_INSPECTION_START == IPS_ProcessID )
	{
		CAOIProject *ProjectPtr=GetActiveProject();
		return ExecIPSComm_SetProjectParam(ProjectPtr);
	}
	if ( IPS_COMM_STATUS_OPEN_PROJECT == IPS_ProcessID )
	{
		CAOIProject *ProjectPtr=GetActiveProject();
		return ExecIPSComm_SetProjectOpen(ProjectPtr);
	}
	return ExecIPSComm_ProcessID(IPS_ProcessID);	
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::ExecMESComm_SetAOIExceptionCode(LPCTSTR ErrStr)//執行MES溝通-系統異常碼
{	
	return ExecIPSComm_SetAOIExceptionCode(ErrStr);
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::ExecMESComm_SetUserLogin_out(bool bLogin)//執行MES溝通-設定使用者登入	
{
	return ExecIPSComm_SetUserLogin_out(bLogin);
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::ExecMESComm_SetControlStateMode(MES_EQP_CTRL_STATE_MODE Mode)//執行MES溝通-設定控制狀態模式
{
	const bool bSucc=ExecIPSComm_SetControlStateMode(Mode);
	if ( false == bSucc )
	{	SetEqpOnlineMode(IPS_COMM_STATUS_OFFLINE); }
	return bSucc;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::ExecMESComm_CheckBarcode(CAOIProject *ProjectPtr)//執行MES溝通-確認條碼
{
	return ExecIPSComm_CheckBarcode(ProjectPtr);
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::ExecMESComm_AskBarcode(CAOIProject *ProjectPtr)//執行MES溝通-詢問條碼
{
	return ExecIPSComm_AskBarcode(ProjectPtr);
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::ExecMESComm_BoardMapping(CAOIProject *ProjectPtr)//執行MES溝通-單板映射
{
	return ExecIPSComm_BoardMapping(ProjectPtr);
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::ExecMESComm_UnCheckTestFile(LANE_ID LaneID, int &Count)//執行MES溝通-未判定檢測檔案數
{
	return ExecIPSComm_UnCheckTestFile(LaneID, Count);
}
//-------------------------------------------------------------------------------------//
size_t CMES_IPS::GetMESRecvNodeCount()
{
	return GetIPSRecvNodeCount();
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::ProcesMESRecvNodeList(bool bThread)//處理收到MES的訊息列表
{
	return ProcesIPSRecvNodeList(bThread);;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::SendToMESNode(const char *strBuf, bool bAck, int nPPID, DWORD AckTime)//送資料給MES
{
	return SendToIPSNode(strBuf, bAck, nPPID, AckTime);;
}
//-------------------------------------------------------------------------------------//
#endif//IPS_DISABLE
#endif//MES_DISABLE