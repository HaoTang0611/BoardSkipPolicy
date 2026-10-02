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
#define IPS_REMOTE_CONTROL_ERROR_SYSTEM_NOT_READY           1
#define IPS_REMOTE_CONTROL_ERROR_NO_SUPPORT_FUNC            2
#define IPS_REMOTE_CONTROL_ERROR_PROJECT_CLOSED             3
#define IPS_REMOTE_CONTROL_ERROR_ON_RUNNING                 4
#define IPS_REMOTE_CONTROL_ERROR_ON_STOPPING                5
#define IPS_REMOTE_CONTROL_ERROR_PROJECT_CREATE             7
#define IPS_REMOTE_CONTROL_ERROR_PROJECT_OPEN               8
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
	CITSLinker &ITSLinker=GetIPSLinker();
	ITSLinker.SetITSThreadStop(true);
	ITSLinker.DisconnectITS();
	return true;
}
//-------------------------------------------------------------------------------------//
int CMES_IPS::GetIPSStatusID_Ack(int StatusID)//取得IPS的回應訊息
{
	int AckID=IPS_STATAUS_NONE;	
	return AckID;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::CheckIPSStatusID_Ack(const TIPSCommNode &Node)//確認IPS的回應訊息
{
	bool bAckID = false;
	std::wstring wsSender=Node.wsSenderStr;
	JetAPI::wstring2upper(wsSender);
	if ( wsSender==L"JET8000") 
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
	case IPS_STATAUS_REMOTE_CONTROL_START://開始檢測
	case IPS_STATAUS_REMOTE_CONTROL_STOP://停止檢測	
	case IPS_STATAUS_REMOTE_CONTROL_PAUSE://暫停檢測
	case IPS_STATAUS_REMOTE_CONTROL_RESUME://繼續檢測	
	case IPS_STATAUS_REMOTE_CONTROL_ABORT://中斷檢測
	case IPS_STATAUS_REMOTE_CONTROL_BYPASS://開始流片
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

	if ( JetAPI::char2wstring(sBuff.c_str(), wsBuf, CP_UTF8) == false ) 
	{	return false; }

	//initial
	Doc.SetObject();
	JSonCtrl.Set(&Doc);			
	if ( JSonCtrl.SetBuffer(wsBuf, Doc) == false ) 
	{	return false; }
	
	//"Header_cs": {
    //"Version_s": "1.0.0",
    //"Sender_s": "JET8000",
    //"StatusCode_i": 300,
    //"Status_s": "End",
    //"ResultCode_i": 0,
    //"Result_s": "OK"
	//},		
	
	sNode = TIPSCommNode();	
	if ( JSonCtrl.FindMember(L"Header_cs", Obj_itr) == false ) 
	{	return false; }
	if ( Obj_itr->value.IsObject() == false )
	{	return false; } 

	if ( JSonCtrl.FindMember(Obj_itr, L"Version_s", itr) == false ) 
	{	return false; }				
	if ( itr->value.IsString() == false ) 
	{	return false; }
	sNode.wsVersionStr = itr->value.GetString();	

	if ( JSonCtrl.FindMember(Obj_itr, L"Sender_s", itr) == false ) 
	{	return false; }				
	if ( itr->value.IsString() == false ) 
	{	return false; }
	sNode.wsSenderStr = itr->value.GetString();	

	if ( JSonCtrl.FindMember(Obj_itr, L"StatusCode_i", itr) == false ) 
	{	return false; }				
	if ( itr->value.IsInt() == false ) 
	{	return false; }
	sNode.nStatusCode = itr->value.GetInt();

	if ( JSonCtrl.FindMember(Obj_itr, L"ResultCode_i", itr) == false ) 
	{	return false; }				
	if ( itr->value.IsInt() == false ) 
	{	return false; }
	sNode.nErrorCode = itr->value.GetInt();

	if ( JSonCtrl.FindMember(Obj_itr, L"Result_s", itr) == false ) 
	{	return false; }				
	if ( itr->value.IsString() == false ) 
	{	return false; }
	sNode.wsErrorCode = itr->value.GetString();	

	if ( JSonCtrl.FindMember(L"Filename_s", itr) == true ) 
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
				if ( JSonCtrl.FindMember(Data_itr, L"Message_s", itr) == true )
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
	if ( IPS_STATAUS_NONE == StatusRes ) 
	{	return true; }	
	
	bool IsOK=true;
	SetFreezeMESFuncMode(true);	
	::Sleep(50);//切換執行緒
	if ( CheckIPSStatusCode_RemoteControl(sNode) == true )
	{	IsOK = ExecIPSComm_MesRemoteControl(bThread, sNode, bBreak, bRemove);	}
	if ( IPS_STATAUS_REMOTE_CONTROL_PROGRAM_LOAD==sNode.nStatus )
	{	IsOK = ExecIPSComm_MesOpenProject(bThread, sNode, bBreak, bRemove);	}	
	if ( IPS_STATAUS_REMOTE_CONTROL_PROGRAM_LIST==sNode.nStatus )
	{	IsOK = ExecIPSComm_MesListProject(bThread, sNode, bBreak, bRemove);	}
	if ( IPS_STATAUS_SETUP==sNode.nStatus )
	{	IsOK = ExecIPSComm_MesSetParam(bThread, sNode, bBreak, bRemove);	}	
	SetFreezeMESFuncMode(false);	
	return IsOK;
#endif//IPS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::SendToIPSNode(const char *strBuf, bool bAck, int nPPID, DWORD AckTime, bool bChkFrz, LPCTSTR filename)//送資料給IPS
{
#ifndef IPS_DISABLE
	CString str;
	CString Err;
	CITSLinker &ITSLinker=GetIPSLinker();
	if ( true == bChkFrz )
	{
		if ( GetFreezeMESFuncMode() == true ) { return true; }
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
		bool bIsOK = WaitForIPSResponse(nPPID, AckTime, Filename);
		if ( bIsOK == false ) 
		{	ErrorString = GetErrorString(); }
		StartIPSProcThread(false);
		if ( bIsOK == false ) 
		{	
			SetErrorString(ErrorString); 
			return false;
		}
	}
#endif//IPS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::WaitForIPSResponse(int nPPID, DWORD AckTime, LPCTSTR Filename)//等待IPS回傳資料
{
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
	const IPS_STATAUS_ID IPS_StatusID=(IPS_STATAUS_ID)(sNode.nStatusCode);
	
	bBreak = false;
	bRemove = true;	
	switch ( OnlineTaskState )
	{
	case TASK_STATE_RUNNING:
		nErrorCode=IPS_REMOTE_CONTROL_ERROR_ON_RUNNING;
		GetIPS_RemoteControlErrorText(nErrorCode, wsErrorCode);
		break;
	case TASK_STATE_TO_STOP:
		nErrorCode=IPS_REMOTE_CONTROL_ERROR_ON_STOPPING;
		GetIPS_RemoteControlErrorText(nErrorCode, wsErrorCode);		
		break;
	default:		
		nErrorCode=IPS_REMOTE_CONTROL_ERROR_SUCCESS;
		GetIPS_RemoteControlErrorText(nErrorCode, wsErrorCode);		
		break;
	}
	
	if ( IPS_REMOTE_CONTROL_ERROR_SUCCESS == nErrorCode )
	{
		std::wstring wsBuf;
		rapidjson::CGMItr itr;	
		rapidjson::WDocument Doc;
		rapidjson::CJsonCtrl JSonCtrl;
		if ( JetAPI::char2wstring(sBuff.c_str(), wsBuf, ConvertCode) == false ) 
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
		
		itrLv2 = itr->value.FindMember(L"Location_s");
		if ( itrLv2 != itr->value.MemberEnd() )
		{
			if ( itrLv2->value.IsString() ) 
			{	SysParam.m_MachineLocation=itrLv2->value.GetString();	}
		}
		itrLv2 = itr->value.FindMember(L"Building_s");
		if ( itrLv2 != itr->value.MemberEnd() )
		{
			if ( itrLv2->value.IsString() ) 
			{	SysParam.m_MachineBuilding=itrLv2->value.GetString();	}
		}
		itrLv2 = itr->value.FindMember(L"Floor_s");
		if ( itrLv2 != itr->value.MemberEnd() )
		{
			if ( itrLv2->value.IsString() ) 
			{	SysParam.m_MachineFloor=itrLv2->value.GetString();	}
		}
		itrLv2 = itr->value.FindMember(L"Station_s");
		if ( itrLv2 != itr->value.MemberEnd() )
		{
			if ( itrLv2->value.IsString() ) 
			{	SysParam.m_MachineStation=itrLv2->value.GetString();	}
		}
		itrLv2 = itr->value.FindMember(L"Room_s");
		if ( itrLv2 != itr->value.MemberEnd() )
		{
			if ( itrLv2->value.IsString() ) 
			{	SysParam.m_MachineRoom=itrLv2->value.GetString();	}
		}
		itrLv2 = itr->value.FindMember(L"Line_s");
		if ( itrLv2 != itr->value.MemberEnd() )
		{
			if ( itrLv2->value.IsString() ) 
			{	SysParam.m_MachineLine=itrLv2->value.GetString();	}
		}

		CAOIProject *ProjectPtr=AOIDataCollect.GetActiveProject();
		if ( NULL != ProjectPtr )
		{
			//"Project_s": "F:\\AOI3DProject\\MultiPanel_T_25M.PRG",        
			//"Product_s": "MultiPanel",
			//"ProjectVersion_s": "",
		}
		/*
		"Data": {
		"Lane_s": "Lane",
		"User_s": "Sign Out",    
		"Security_s": "未登入",
		"Factory_s": "Factory",
		"Machine_s": "JET8000",
		"Department_s": "Department",
		"Shift_s": "Shift",
		"FixtureID_s": "FixtureID",
		"BoardName_s": "BoardName",    
		"SoftwareVersion_s": "1.01.03.100"
		*/
	}	

	int  nPPID=0;
	const bool bAck=false;
	const DWORD AckTime=1000;
	rapidjson::WDocument Doc;
	rapidjson::CJsonCtrl JSonCtrl;	

	Doc.SetObject();	
	BuildIPSDoc_Ack(nStatus, bAck, AckTime, nErrorCode, wsErrorCode.c_str(), Doc);
		
	std::string  sBufSend;
	std::wstring wBufSend;
	
	JSonCtrl.Set(&Doc);	
	rapidjson::WValue Data(rapidjson::kObjectType);	
	bRet=JSonCtrl.AddObject(&Data, L"MachineStatus_s", L"");
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}
	bRet=JSonCtrl.AddObject(&Data, L"SoftwareStatus_s", L"");
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
	const IPS_STATAUS_ID IPS_StatusID=(IPS_STATAUS_ID)(sNode.nStatusCode);	
	
	CString ProjectName, PathName;	

	NameCount=0;
	ClearProjectNameList();	
	if ( true == FindFilename )
	{
		std::wstring wsBuf, wsVal;		
		rapidjson::CGMItr itr, itrLv2;	
		rapidjson::WDocument Doc;
		rapidjson::CJsonCtrl JSonCtrl;			
		const std::string sBuff = sNode.sRawData;	
		if ( JetAPI::char2wstring(sBuff.c_str(), wsBuf, ConvertCode) == false ) 
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
					itrLv2 = itr->value.FindMember(L"RemoteControlData");
					if ( itrLv2 != itr->value.MemberEnd() )
					{
						if ( itrLv2->value.IsObject() == true )
						{							
							auto itrLv3 = itrLv2->value.FindMember(L"ProgramLoad");
							if ( itrLv3 != itrLv2->value.MemberEnd() )
							{
								if ( itrLv3->value.IsArray() == true )
								{							
									for (auto itrLv4 = itrLv3->value.Begin();  itrLv4!= itrLv3->value.End(); ++itrLv4)
									{
										if ( itrLv4->IsString() == false ) 
										{	continue; }

										str = CString(itrLv4->GetString());
										NameCount ++;
										AddProjectName(str);
										if ( ProjectName.GetLength() == 0 )
										{	ProjectName = str;	}										
									}//itrLv4
								}						
							}//itrLv3
						}
					}//itrLv2
				}
			}			
		}		
	}

	if ( 0 == NameCount )
	{	
		nErrorCode=IPS_REMOTE_CONTROL_ERROR_PROJECT_OPEN;
		GetIPS_RemoteControlErrorText(nErrorCode, wsErrorCode);
	}
	else
	{
		bRemove = true;			
		nErrorCode = IPS_REMOTE_CONTROL_ERROR_SUCCESS;	
		GetIPS_RemoteControlErrorText(nErrorCode, wsErrorCode);
		PathName.Format(_T("%s\\%s"), AOIDataCollect.GetAOIProjectDirectory(), ProjectName);
		switch ( OnlineTaskState )
		{					
		case TASK_STATE_RUNNING:
			nErrorCode=IPS_REMOTE_CONTROL_ERROR_ON_RUNNING;
			GetIPS_RemoteControlErrorText(nErrorCode, wsErrorCode);
			break;
		case TASK_STATE_TO_STOP:
			nErrorCode=IPS_REMOTE_CONTROL_ERROR_ON_STOPPING;
			GetIPS_RemoteControlErrorText(nErrorCode, wsErrorCode);			
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
	BuildIPSDoc_Ack(nStatus, bAck, AckTime, nErrorCode, wsErrorCode.c_str(), Doc);
		
	std::string  sBufSend;
	std::wstring wBufSend;
	
	JSonCtrl.Set(&Doc);	
	rapidjson::WValue Data(rapidjson::kObjectType);	
	bRet=JSonCtrl.AddObject(&Data, L"ACK_i", 1);
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
	const IPS_STATAUS_ID IPS_StatusID=(IPS_STATAUS_ID)(sNode.nStatusCode);			
	
	int  nPPID=0;
	bool bRet=true;
	const bool bAck=false;
	const DWORD AckTime=1000;
	rapidjson::WDocument Doc;
	rapidjson::CJsonCtrl JSonCtrl;	
	const int nStatus=IPS_StatusID;	

	Doc.SetObject();	
	BuildIPSDoc_Ack(nStatus, bAck, AckTime, nErrorCode, wsErrorCode.c_str(), Doc);
		
	std::string  sBufSend;
	std::wstring wBufSend;
	
	JSonCtrl.Set(&Doc);	
	rapidjson::WValue Data(rapidjson::kObjectType);	
	bRet=JSonCtrl.AddObject(&Data, L"ACK_i", 1);
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

	bRemove = true;
	const bool bChkFrz = false;
	CString Filename=sNode.wsFilename.c_str();
	if ( GetAckFilename(Filename) == false ) { return false; }
	if ( SendToIPSNode(sBufSend.c_str(), false, sNode.nPPID, 0, bChkFrz, Filename) == false )
	{	return false; }
	PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_MES_CMD_LIST_PROJECT, NULL);	
#endif//IPS_DISABLE	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::ExecIPSComm_MesRemoteControl(bool bThread, TIPSCommNode &sNode, bool &bBreak, bool &bRemove)
{
#ifndef IPS_DISABLE	
	CString str;
	int   nErrorCode=0;		
	std::wstring wsErrorCode=L"";
	std::wstring wsRemoteCmd=L"";
	const bool bAutoReset = true;
	const bool bChkStartLight = true;
	const UINT ConvertCode = CP_UTF8;	
	const TASK_STATE_MODE  OnlineTaskState = GetOnlineTaskState();
	const IPS_STATAUS_ID IPS_StatusID=(IPS_STATAUS_ID)(sNode.nStatusCode);			

	bRemove = true;		
	nErrorCode=IPS_REMOTE_CONTROL_ERROR_SUCCESS;
	GetIPS_RemoteControlErrorText(nErrorCode, wsErrorCode);
	switch ( IPS_StatusID )
	{
	case IPS_STATAUS_REMOTE_CONTROL_START:
		wsRemoteCmd=L"Start";
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
				GetIPS_RemoteControlErrorText(nErrorCode, wsErrorCode);
				break;
			case TASK_STATE_TO_STOP:
				nErrorCode=IPS_REMOTE_CONTROL_ERROR_ON_STOPPING;
				GetIPS_RemoteControlErrorText(nErrorCode, wsErrorCode);
				break;
			case TASK_STATE_NONE:
			case TASK_STATE_IDLE:
				if ( CheckSystemReady(bChkStartLight, bAutoReset) == true ) 
				{
					CAOIProject *ProjectPtr = GetActiveProject();
					if ( NULL != ProjectPtr ) 
					{	PostMainFrameWndMessage(WM_COMMAND, ID_ONLINE_RUN, NULL);	}
					else
					{	
						nErrorCode=IPS_REMOTE_CONTROL_ERROR_PROJECT_CLOSED;
						GetIPS_RemoteControlErrorText(nErrorCode, wsErrorCode);
					}
				}
				else
				{	
					str = GetErrorString();
					JetAPI::TCHAR2wstring(str, wsErrorCode, ConvertCode);
					nErrorCode = IPS_REMOTE_CONTROL_ERROR_SYSTEM_NOT_READY;
				}
				break;
			}
		}		
		break;
	case IPS_STATAUS_REMOTE_CONTROL_STOP:		
		wsRemoteCmd=L"Stop";
		switch ( OnlineTaskState )
		{
		case TASK_STATE_NONE:						
			break;
		case TASK_STATE_RUNNING:
			if ( true == bThread ) 
			{	
				bBreak = true; 
				bRemove = false;				
			}
			else
			{	SetOnlineTaskState(TASK_STATE_TO_STOP); }
			break;
		case TASK_STATE_TO_STOP:
			break;
		case TASK_STATE_IDLE:
			break;
		}		
		break;
	case IPS_STATAUS_REMOTE_CONTROL_BYPASS:
		wsRemoteCmd=L"Bypass";
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
				GetIPS_RemoteControlErrorText(nErrorCode, wsErrorCode);
				break;
			case TASK_STATE_TO_STOP:
				nErrorCode=IPS_REMOTE_CONTROL_ERROR_ON_STOPPING;
				GetIPS_RemoteControlErrorText(nErrorCode, wsErrorCode);
				break;
			case TASK_STATE_NONE:
			case TASK_STATE_IDLE:
				if ( CheckSystemReady(bChkStartLight, bAutoReset) == true ) 
				{	PostMainFrameWndMessage(WM_COMMAND, ID_ONLINE_BYPASS, NULL);	}
				else
				{	
					str = GetErrorString();
					JetAPI::TCHAR2wstring(str, wsErrorCode, ConvertCode);
					nErrorCode = IPS_REMOTE_CONTROL_ERROR_SYSTEM_NOT_READY;
				}					
				break;
			}
		}
		break;
	case IPS_STATAUS_REMOTE_CONTROL_PAUSE:
		wsRemoteCmd=L"Pause";
		nErrorCode=IPS_REMOTE_CONTROL_ERROR_NO_SUPPORT_FUNC;
		GetIPS_RemoteControlErrorText(nErrorCode, wsErrorCode);
		break;
	case IPS_STATAUS_REMOTE_CONTROL_RESUME:
		wsRemoteCmd=L"Resume";
		nErrorCode=IPS_REMOTE_CONTROL_ERROR_NO_SUPPORT_FUNC;
		GetIPS_RemoteControlErrorText(nErrorCode, wsErrorCode);
		break;
	case IPS_STATAUS_REMOTE_CONTROL_ABORT:
		wsRemoteCmd=L"Abort";
		switch ( OnlineTaskState )
		{
		case TASK_STATE_NONE:						
			break;
		case TASK_STATE_RUNNING:
			if ( true == bThread ) 
			{	
				bBreak = true; 
				bRemove = true;
			}
			SetOnlineTaskState(TASK_STATE_TO_ABORT);
			break;
		case TASK_STATE_TO_STOP:
			break;
		case TASK_STATE_IDLE:
			break;
		}	
		break;

	}	

	int  nPPID=0;
	bool bRet=true;
	const bool bAck=false;
	const DWORD AckTime=1000;
	rapidjson::WDocument Doc;
	rapidjson::CJsonCtrl JSonCtrl;	
	const int nStatus=IPS_StatusID;

	Doc.SetObject();	
	BuildIPSDoc_Ack(nStatus, bAck, AckTime, nErrorCode, wsErrorCode.c_str(), Doc);
		
	std::string  sBufSend;
	std::wstring wBufSend;
	
	JSonCtrl.Set(&Doc);	
	rapidjson::WValue Data(rapidjson::kObjectType);	
	bRet=JSonCtrl.AddObject(&Data, L"ACK_i", 1);
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
	if ( IPS_STATAUS_NONE == ProcessID )
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
	return true;
#ifndef IPS_DISABLE
	CString str;
	CString Err;
	int  nPPID = 0;
	bool bAck = true;
	int  AckTime = 1000;	
	std::wstring wsBuf;
	if ( BuildIPSDoc_SetAOIExceptionCode(bAck, AckTime, nPPID, wsBuf) == false )
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
bool CMES_IPS::ExecIPSComm_SetRemoteControlMode(bool bOnline)//執行IPS溝通-遠端控制模式	
{
#ifndef IPS_DISABLE
	CString str;
	CString Err;
	int  nPPID = 0;
	bool bAck = true;
	int  AckTime = 1000;	
	std::wstring wsBuf;
	if ( BuildIPSDoc_SetRemotControlMode(bAck, AckTime, nPPID, bOnline, wsBuf) == false )
	{	
		Err = GetErrorString();
		str = _T("Error, BuildIPSDoc_SetRemotControlMode Fault");
		str = GetMESMultiLanguage(str);
		m_ErrorString.Format(_T("%s\n%s"), str, Err);
		return false; 
	}

	std::string sBuf;
	UINT ConvertCode = CP_UTF8;
	if ( JetAPI::wchar2string(wsBuf.c_str(), sBuf, ConvertCode) == false ) 
	{
		m_ErrorString.Format(_T("Error, Conver wstring to string fault [%s]"), _T("SetRemoteControlMode"));
		return false;
	}
	
	if ( SendToIPSNode(sBuf.c_str(), bAck, nPPID, AckTime) == false ) 
	{	return false; }
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
bool CMES_IPS::ExecIPSComm_SetProjectList()//執行IPS溝通-條列專案
{
#ifndef IPS_DISABLE
	CString str;
	CString Err;
	int  nPPID = 0;
	bool bAck = true;
	int  AckTime = 1000;	
	std::wstring wsBuf;	
	if ( BuildIPSDoc_SetProjectList(bAck, AckTime, nPPID, wsBuf) == false )
	{
		Err = GetErrorString();
		str = _T("Error, BuildIPSDoc_SetProjectList Fault");
		str = GetMESMultiLanguage(str);
		m_ErrorString.Format(_T("%s\n%s"), str, Err);
		return false; 
	}

	std::string sBuf;
	UINT ConvertCode = CP_UTF8;
	if ( JetAPI::wchar2string(wsBuf.c_str(), sBuf, ConvertCode) == false ) 
	{
		m_ErrorString.Format(_T("Error, Conver wstring to string fault [%s]"), _T("SetProjectList"));
		return false;
	}
	
	if ( SendToIPSNode(sBuf.c_str(), bAck, nPPID, AckTime) == false ) 
	{	return false; }
#endif//IPS_DISABLE	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::ExecIPSComm_SetProjectLoad(CAOIProject *ProjectPtr, bool Success)//執行IPS溝通-專案載入
{
#ifndef IPS_DISABLE
	CString str;
	CString Err;
	int  nPPID = 0;
	bool bAck = true;
	int  AckTime = 1000;	
	std::wstring wsBuf;
	if ( BuildIPSDoc_SetProjectLoad(bAck, AckTime, nPPID, ProjectPtr, Success, wsBuf) == false )
	{	
		Err = GetErrorString();
		str = _T("Error, ExecIPSComm_SetProjectLoad Fault");
		str = GetMESMultiLanguage(str);
		m_ErrorString.Format(_T("%s\n%s"), str, Err);
		return false; 
	}

	std::string sBuf;
	UINT ConvertCode = CP_UTF8;
	if ( JetAPI::wchar2string(wsBuf.c_str(), sBuf, ConvertCode) == false ) 
	{
		m_ErrorString.Format(_T("Error, Conver wstring to string fault [%s]"), _T("SetProjectLoad"));
		return false;
	}
	
	if ( SendToIPSNode(sBuf.c_str(), bAck, nPPID, AckTime) == false ) 
	{	return false; }
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
bool CMES_IPS::BuildIPSDoc_BarcodeList(CAOIProject *ProjectPtr, rapidjson::WDocument &Doc, rapidjson::WValue &object)//建立IPS文檔
{
	if ( NULL == ProjectPtr ) 
	{
		m_ErrorString = _T("Error, No Project");
		return false;
	}

	CString      str;
	size_t       i=0, j=0;
	size_t       PanelBoardCount=0;
	CAOIPanel   *PanelPtr = NULL;
	CAOIBoard   *BoardPtr = NULL;	
	const size_t PanelCount=ProjectPtr->GetProjectPanelCount();

	bool         bRet=false;	
	std::wstring wsBuffer;
	const UINT ConvertCode = CP_UTF8;
	rapidjson::CJsonCtrl JSonCtrl;	

	JSonCtrl.Set(&Doc);
	auto &alc = Doc.GetAllocator();

	rapidjson::WValue PanelArray(rapidjson::kArrayType);
	for ( i=0; i<PanelCount; i++ )
	{
		PanelPtr = ProjectPtr->GetProjectPanelPtr(i, false);
		if ( NULL == PanelPtr ) { continue; }

		rapidjson::WValue BoardArray(rapidjson::kArrayType);
		PanelBoardCount = PanelPtr->GetPanelBoardCount();
		for ( j=0; j<PanelBoardCount; j++ )
		{
			BoardPtr = PanelPtr->GetPanelBoardPtr(j, false);
			if ( NULL == BoardPtr ) { continue; }
			
			//"BoardID_i": 1,
            //"Barcode_s": "Barcode_1_1",
            //"BarcodeInternal_s": "BarcodeInternal_1_1",
            //"Status_fg": true
			
			rapidjson::WValue BoardObject(rapidjson::kObjectType);

			bRet=JSonCtrl.AddObject(&BoardObject, L"BoardID_i", j+1);//BoardID
			if ( false == bRet ) 
			{
				m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
				return false; 
			}

			wsBuffer = BoardPtr->GetBoardBarcode();
			bRet=JSonCtrl.AddObject(&BoardObject, L"Barcode_s", wsBuffer);//Barcode
			if ( false == bRet ) 
			{
				m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
				return false; 
			}

			str = JetAPI::GetSpcBarcodeInternalCode();
			JetAPI::TCHAR2wstring(str, wsBuffer, ConvertCode);
			bRet=JSonCtrl.AddObject(&BoardObject, L"BarcodeInternal_s", wsBuffer);//@JET
			if ( false == bRet ) 
			{
				m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
				return false; 
			}

			bRet=JSonCtrl.AddObject(&BoardObject, L"Status_fg", true);//Status
			if ( false == bRet ) 
			{
				m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
				return false; 
			}

			BoardArray.PushBack(BoardObject, alc);
		}
		
		rapidjson::WValue PanelObject(rapidjson::kObjectType);
		bRet=JSonCtrl.AddObject(&PanelObject, L"BoardList", BoardArray);//Board List
		if ( false == bRet ) 
		{
			m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
			return false; 
		}

		//"PanelID_i": 1,
        //"Barcode_s": "Barcode_1",
        //"BarcodeInternal_s": "BarcodeInternal_1",
        //"Status_fg": true
		bRet=JSonCtrl.AddObject(&PanelObject, L"PanelID_i", i+1);//PanelID
		if ( false == bRet ) 
		{
			m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
			return false; 
		}

		wsBuffer = PanelPtr->GetPanelBarcode();
		bRet=JSonCtrl.AddObject(&PanelObject, L"Barcode_s", wsBuffer);//Barcode
		if ( false == bRet ) 
		{
			m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
			return false; 
		}

		str = JetAPI::GetSpcBarcodeInternalCode();
		JetAPI::TCHAR2wstring(str, wsBuffer, ConvertCode);
		bRet=JSonCtrl.AddObject(&PanelObject, L"BarcodeInternal_s", wsBuffer);//@JET
		if ( false == bRet ) 
		{
			m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
			return false; 
		}

		bRet=JSonCtrl.AddObject(&PanelObject, L"Status_fg", true);//Status
		if ( false == bRet ) 
		{
			m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
			return false; 
		}

		PanelArray.PushBack(PanelObject, alc);
	}

	bRet=JSonCtrl.AddObject(&object, L"PanelList", PanelArray);//Panel List
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::BuildIPSDoc(int nStatus, bool bAck, int AckTime, int &nPPID, rapidjson::WDocument &Doc)//建立IPS檔案
{
	bool         bRet=false;	
	std::wstring wstrBuffer;
	const UINT ConvertCode = CP_UTF8;
	CString      strStatus;	
	rapidjson::CJsonCtrl JSonCtrl;
	rapidjson::WValue object(rapidjson::kObjectType);	

	nPPID = nStatus;
	strStatus = GetIPSStatusText(nStatus);
	JetAPI::TCHAR2wstring(strStatus, wstrBuffer, ConvertCode);

	//"Header_cs": {
    //"Version_s": "1.0.0",
    //"Sender_s": "JET8000",
    //"StatusCode_i": 100,
    //"Status_s": "Setup",
    //"ResultCode_i": 0,
    //"Result_s": "OK"
    //},
	
	//initial	
	JSonCtrl.Set(&Doc);
	
	bRet=JSonCtrl.AddObject(&object, L"Version_s", L"1.0.0");//Version	
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	bRet=JSonCtrl.AddObject(&object, L"Sender_s", L"JET8000");//Sender	
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	bRet=JSonCtrl.AddObject(&object, L"StatusCode_i", nStatus);//StatusCode
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	bRet=JSonCtrl.AddObject(&object, L"Status_s", wstrBuffer);//StatusStr
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	bRet=JSonCtrl.AddObject(&object, L"ResultCode_i", 0);//ResultCode
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	bRet=JSonCtrl.AddObject(&object, L"Result_s", L"OK");//ResultStr
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	bRet=JSonCtrl.AddMember(L"Header_cs", object);
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
	strStatus = GetIPSStatusText(nStatus);
	JetAPI::TCHAR2wstring(strStatus, wstrBuffer, ConvertCode);

	//"Header_cs": {
    //"Version_s": "1.0.0",
    //"Sender_s": "JET8000",
    //"StatusCode_i": 100,
    //"Status_s": "Setup",
    //"ResultCode_i": 0,
    //"Result_s": "OK"
    //},
	
	//initial	
	JSonCtrl.Set(&Doc);
	
	bRet=JSonCtrl.AddObject(&object, L"Version_s", L"1.0.0");//Version	
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	bRet=JSonCtrl.AddObject(&object, L"Sender_s", L"JET8000");//Sender	
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	bRet=JSonCtrl.AddObject(&object, L"StatusCode_i", nStatus);//StatusCode
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	bRet=JSonCtrl.AddObject(&object, L"Status_s", wstrBuffer);//StatusStr
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	bRet=JSonCtrl.AddObject(&object, L"ResultCode_i", ResultCode);//ResultCode
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	bRet=JSonCtrl.AddObject(&object, L"Result_s", ResultStr);//ResultStr
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	bRet=JSonCtrl.AddMember(L"Header_cs", object);
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}
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
	
	IPS_STATAUS_ID IPSStatusID;
	if ( ONLINE_STATE_APP_CLOSE == Status )
	{	IPSStatusID = IPS_STATAUS_JET8000_END;	}
	else if ( ONLINE_STATE_APP_OPEN == Status )
	{	IPSStatusID = IPS_STATAUS_JET8000_START;	}
	else
	{	IPSStatusID = IPS_STATAUS_STATUS_MODE;	}	
	if ( BuildIPSDoc(IPSStatusID, bAck, AckTime, nPPID, Doc) == false )
	{	return false; }
	
	/*
	//"Data": {
    //"MachineStatus_s": "",
    //"SoftwareStatus_s": "Show"
	*/

	//add object into doc
	rapidjson::WValue object(rapidjson::kObjectType);	
	//add value into the object
	ONLINE_STATE_MODE eOnlineState = Status;

	if ( IPS_STATAUS_JET8000_END == IPSStatusID )
	{	
		bRet=JSonCtrl.AddObject(&object, L"String_s", L"ProgramEnd");//String_s
		if ( false == bRet ) 
		{
			m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
			return false; 
		}
	}
	else if ( IPS_STATAUS_JET8000_START == IPSStatusID )
	{	
		bRet=JSonCtrl.AddObject(&object, L"String_s", L"ProgramStart");//String_s
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
		bRet=JSonCtrl.AddObject(&object, L"MachineStatus_s", wsBuffer);//MachineStatus_s
		if ( false == bRet ) 
		{
			m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
			return false; 
		}

		bRet=JSonCtrl.AddObject(&object, L"SoftwareStatus_s", L"");//SoftwareStatus_s
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
bool CMES_IPS::BuildIPSDoc_SetSystemParam(bool bAck, int AckTime, int &nPPID, std::wstring& wsBuf)//建立系統資訊文檔
{
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();	
	return BuildIPSDoc_SetProjectParam(bAck, AckTime, nPPID, ProjectPtr, wsBuf);
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::BuildIPSDoc_SetProjectParam(bool bAck, int AckTime, int &nPPID, CAOIProject *ProjectPtr, std::wstring& wsBuf)//建立基本資訊文檔
{
	bool bRet=true;	
	TSystemParameter &SysParam = AOIDataCollect.GetSystemParameter();	

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

	if ( BuildIPSDoc(IPS_STATAUS_SETUP, bAck, AckTime, nPPID, Doc) == false )
	{	return false; }
	
	//"Data": {
    //"Project_s": "Project",
    //"Location_s": "Location",
    //"Building_s": "Building",
    //"Floor_s": "Floor",
    //"Station_s": "Station",
    //"Room_s": "Room",
    //"Line_s": "Line",
    //"Lane_s": "Lane",
    //"User_s": "User",
    //"Product_s": "Product",
    //"Security_s": "Security",
    //"Factory_s": "Factory",
    //"Machine_s": "Machine",
    //"Department_s": "Department",
    //"Shift_s": "Shift",
    //"FixtureID_s": "FixtureID",
    //"BoardName_s": "BoardName",
    //"ProjectVersion_s": "ProjectVersion",
    //"SoftwareVersion_s": "SoftwareVersion"
	//}

	//add object into doc
	rapidjson::WValue object(rapidjson::kObjectType);	
	
	//Project_s;
	if ( NULL == ProjectPtr )
	{	str=_T("");	}
	else
	{	str=ProjectPtr->GetProjectShowName(); }
	JetAPI::TCHAR2wstring(str, wsBuffer, ConvertCode);	
	bRet=JSonCtrl.AddObject(&object, L"Project_s", wsBuffer);//Project
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	str = SysParam.m_MachineLocation;
	JetAPI::TCHAR2wstring(str, wsBuffer, ConvertCode);	
	bRet=JSonCtrl.AddObject(&object, L"Location_s", wsBuffer);//Location
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	str = SysParam.m_MachineBuilding;
	JetAPI::TCHAR2wstring(str, wsBuffer, ConvertCode);	
	bRet=JSonCtrl.AddObject(&object, L"Building_s", wsBuffer);//Building
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	str = SysParam.m_MachineFloor;
	JetAPI::TCHAR2wstring(str, wsBuffer, ConvertCode);	
	bRet=JSonCtrl.AddObject(&object, L"Floor_s", wsBuffer);//Floor
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	str = SysParam.m_MachineStation;
	JetAPI::TCHAR2wstring(str, wsBuffer, ConvertCode);	
	bRet=JSonCtrl.AddObject(&object, L"Station_s", wsBuffer);//Station
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	str = SysParam.m_MachineRoom;
	JetAPI::TCHAR2wstring(str, wsBuffer, ConvertCode);	
	bRet=JSonCtrl.AddObject(&object, L"Room_s", wsBuffer);//Room
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	str = SysParam.m_MachineLine;
	JetAPI::TCHAR2wstring(str, wsBuffer, ConvertCode);	
	bRet=JSonCtrl.AddObject(&object, L"Line_s", wsBuffer);//Line
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
	
	str = AOIDataCollect.GetCurrentUserName();
    JetAPI::TCHAR2wstring(str, wsBuffer, ConvertCode);	
	bRet=JSonCtrl.AddObject(&object, L"User_s", wsBuffer);//User
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
	bRet=JSonCtrl.AddObject(&object, L"Product_s", wsBuffer);//Product
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	bRet=JSonCtrl.AddObject(&object, L"Security_s", L"Security");
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	bRet=JSonCtrl.AddObject(&object, L"Factory_s", L"Factory");
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	str = SysParam.m_MachineName;
	JetAPI::TCHAR2wstring(str, wsBuffer, ConvertCode);	
	bRet=JSonCtrl.AddObject(&object, L"Machine_s", wsBuffer);//Machine
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	bRet=JSonCtrl.AddObject(&object, L"Department_s", L"Department");
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	bRet=JSonCtrl.AddObject(&object, L"Shift_s", L"Shift");
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	bRet=JSonCtrl.AddObject(&object, L"FixtureID_s", L"FixtureID");
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
	bRet=JSonCtrl.AddObject(&object, L"BoardName_s", wsBuffer);//??
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	bRet=JSonCtrl.AddObject(&object, L"ProjectVersion_s", L"ProjectVersion");
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	bRet=JSonCtrl.AddObject(&object, L"SoftwareVersion_s", L"SoftwareVersion");
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
	
	if ( true == bAckMode )
	{	bAck = true;	}

	if ( BuildIPSDoc(ProcessID, bAck, AckTime, nPPID, Doc) == false )
	{	return false; }

	if ( IPS_STATAUS_INSPECTION_START==ProcessID || IPS_STATAUS_INSPECTION_END==ProcessID )
	{
		//add object into doc
		rapidjson::WValue object(rapidjson::kObjectType);			
		if ( BuildIPSDoc_BarcodeList(ProjectPtr, Doc, object) == false )
		{	return false;	}		
		bRet=JSonCtrl.AddMember(L"Data", object);
		if ( false == bRet ) 
		{
			m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
			return false; 
		}
	}
	else
	{
		//"Data": {
		//"MachineStatus_s": "ReadyToUnload",
		//"SoftwareStatus_s": ""
		//}	
		
		rapidjson::WValue object(rapidjson::kObjectType);	
		
		str = GetIPSStatusText(ProcessID);
		JetAPI::TCHAR2wstring(str, wsBuffer, ConvertCode);
		bRet=JSonCtrl.AddObject(&object, L"MachineStatus_s", wsBuffer);//MachineStatus
		if ( false == bRet ) 
		{
			m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
			return false; 
		}

		bRet=JSonCtrl.AddObject(&object, L"SoftwareStatus_s", L"");//SoftwareStatus
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
	}
	
	bRet = JSonCtrl.GetBuffer(wsBuf, Doc);
	if ( false == bRet ) { return false; }

	DumpMESDoc(Doc, L"BuildJSONRaw_SetStatus");
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::BuildIPSDoc_SetAOIExceptionCode(bool bAck, int AckTime, int &nPPID, std::wstring& wsBuf)//執行MES溝通-系統異常碼
{
	bool bRet=true;	

	CString    str;		
	std::wstring wsBuffer;
	rapidjson::WDocument Doc;
	rapidjson::CJsonCtrl JSonCtrl;	
	const UINT ConvertCode = CP_UTF8;
	const int  nStatus = IPS_STATAUS_STATUS_ALARM;
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
    //"MachineStatus_s": "Project",    
	//}

	//add object into doc
	rapidjson::WValue object(rapidjson::kObjectType);	

	str = GetIPSStatusText(nStatus);
	JetAPI::TCHAR2wstring(str, wsBuffer, ConvertCode);	
	bRet=JSonCtrl.AddObject(&object, L"MachineStatus_s", wsBuffer);//MachineStatus
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

	DumpMESDoc(Doc, L"BuildJSONRaw_SetSystemExceptionCode");
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

	if ( BuildIPSDoc(IPS_STATAUS_AOI_LOGIN_OUT_CMD, bAck, AckTime, nPPID, Doc) == false )
	{	return false; }
	
	//add object into doc
	rapidjson::WValue object(rapidjson::kObjectType);	
	//add value into the object	
	str = UserNode.wUserName;//MES登入名稱
	JetAPI::TCHAR2wstring(str, wsBuffer, ConvertCode);	
	bRet=JSonCtrl.AddObject(&object, L"MES_User_Name", wsBuffer);
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}
	
	str = UserNode.wPassword;//MES登入密碼
	JetAPI::TCHAR2wstring(str, wsBuffer, ConvertCode);	
	bRet=JSonCtrl.AddObject(&object, L"MES_User_Password", wsBuffer);
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	if ( true==Login ) { nVal = 1; }
	else { nVal = 2; }
	bRet=JSonCtrl.AddObject(&object, L"MES_Login", nVal);
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
bool CMES_IPS::BuildIPSDoc_SetRemotControlMode(bool bAck, int AckTime, int &nPPID, bool bOnline, std::wstring& wsBuf)//建立遠端控制文檔
{
	nPPID = 0;
	int  nStatus = 0;
	if ( true == bOnline )
	{	nStatus = IPS_STATAUS_STATUS_ONLINE;	}
	else
	{	nStatus = IPS_STATAUS_STATUS_OFFLINE;	}
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
	
	if ( true == bAckMode )
	{	bAck = true;	}

	if ( BuildIPSDoc(IPS_STATAUS_AOI_CHECK_BARCODE, bAck, AckTime, nPPID, Doc) == false )
	{	return false; }
	
	//add object into doc	
	rapidjson::WValue BarcodeSection(rapidjson::kObjectType);
	
	size_t     i=0, j=0;
	int        PanelIdx=0;
	int        BoardIdx=0;		
	int        BarcodeCount=0;		
	CAOIPanel *PanelPtr = NULL;
	CAOIBoard *BoardPtr = NULL;
	PANEL_SIDE_MODE PanelSideMode;
	BOARD_SIDE_MODE BoardSideMode;
	const size_t PanelCount=ProjectPtr->GetProjectPanelCount();
	
	for ( i=0; i<PanelCount; i++ )
	{
		PanelPtr = ProjectPtr->GetProjectPanelPtr(i, false);
		if ( NULL == PanelPtr ) { continue; }
		PanelIdx = (int)(i);
		BoardIdx = -1;
		if ( PanelPtr->GetPanelIsGetBarcode() == true )
		{
			//add value into the object
			rapidjson::WValue object2(rapidjson::kObjectType);	
			bRet=JSonCtrl.AddObject(&object2, L"Panel", PanelIdx+1);
			if ( false == bRet ) 
			{
				m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
				return false; 
			}
			bRet=JSonCtrl.AddObject(&object2, L"Board", BoardIdx+1);
			if ( false == bRet ) 
			{
				m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
				return false; 
			}
			str = PanelPtr->GetPanelBarcode();
			JetAPI::TCHAR2wstring(str, wsBuffer, ConvertCode);	
			bRet=JSonCtrl.AddObject(&object2, L"Barcode", wsBuffer);
			if ( false == bRet ) 
			{
				m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
				return false; 
			}

			PanelSideMode = PanelPtr->CheckPanelSideMode();
			switch ( PanelSideMode )
			{
			case PANEL_SIDE_BOTTOM:	str = _T("Bot");	break;			
			case PANEL_SIDE_TOP:	str = _T("Top");	break;
			default:
			case PANEL_SIDE_HYBRID:	str = _T("Hybrid");	break;
			}
			JetAPI::TCHAR2wstring(str, wsBuffer, ConvertCode);	
			bRet=JSonCtrl.AddObject(&object2, L"Board_Side", wsBuffer);
			if ( false == bRet ) 
			{
				m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
				return false; 
			}

			str.Format(_T("Barcode_%06d"), BarcodeCount+1);
			JetAPI::TCHAR2wstring(str, wsKey, ConvertCode);	
			bRet=JSonCtrl.AddObject(&BarcodeSection, wsKey, object2);
			if ( false == bRet ) 
			{
				m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
				return false; 
			}			

			BarcodeCount ++;
		}
		const size_t PanelBoardCount=PanelPtr->GetPanelBoardCount();
		for ( j=0; j<PanelBoardCount; j++ )
		{
			BoardPtr = PanelPtr->GetPanelBoardPtr(j, false);
			if ( NULL == BoardPtr ) { continue; }
			BoardIdx = (int)(j);
			if ( BoardPtr->GetBoardIsGetBarcode() == true )
			{
				//add value into the object
				rapidjson::WValue object2(rapidjson::kObjectType);	
				bRet=JSonCtrl.AddObject(&object2, L"Panel", PanelIdx+1);
				if ( false == bRet ) 
				{
					m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
					return false; 
				}
				bRet=JSonCtrl.AddObject(&object2, L"Board", BoardIdx+1);
				if ( false == bRet ) 
				{
					m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
					return false; 
				}
				str = BoardPtr->GetBoardBarcode();
				JetAPI::TCHAR2wstring(str, wsBuffer, ConvertCode);	
				bRet=JSonCtrl.AddObject(&object2, L"Barcode", wsBuffer);
				if ( false == bRet ) 
				{
					m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
					return false; 
				}

				BoardSideMode = BoardPtr->GetBoardSideMode();
				switch ( BoardSideMode )
				{
				case BOARD_SIDE_BOT:	str = _T("Bot");	break;
				default:
				case BOARD_SIDE_TOP:	str = _T("Top");	break;					
				}
				JetAPI::TCHAR2wstring(str, wsBuffer, ConvertCode);	
				bRet=JSonCtrl.AddObject(&object2, L"Board_Side", wsBuffer);
				if ( false == bRet ) 
				{
					m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
					return false; 
				}

				str.Format(_T("Barcode_%06d"), BarcodeCount+1);
				JetAPI::TCHAR2wstring(str, wsKey, ConvertCode);	
				bRet=JSonCtrl.AddObject(&BarcodeSection, wsKey, object2);
				if ( false == bRet ) 
				{
					m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
					return false; 
				}

				BarcodeCount ++;
			}
		}
	}	
	
	bRet=JSonCtrl.AddObject(&BarcodeSection, L"Barcode_Count", BarcodeCount);
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}
	
	rapidjson::WValue object(rapidjson::kObjectType);		
	bRet=JSonCtrl.AddObject(&object, L"MachineStatusData", BarcodeSection);
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

	DumpMESDoc(Doc, L"BuildJSONRaw_CheckBarcode");
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::BuildIPSDoc_SetProjectList(bool bAck, int AckTime, int &nPPID, std::wstring& wsBuf)//建立條列專案
{
	bool bRet=true;
	size_t     i=0;
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
	
	if ( true == bAckMode )
	{	bAck = true;	}

	if ( BuildIPSDoc(IPS_STATAUS_STATUS_MODE, bAck, AckTime, nPPID, Doc) == false )
	{	return false; }

	std::vector<CString> FileList;	
	CString ProjectFolder=AOIDataCollect.GetAOIProjectDirectory();
	JetAPI::ListFilesInFolder(ProjectFolder, _T("PRG"), FileList);
	const size_t FileCount=FileList.size();	
		
	std::string  sBufSend;
	std::wstring wBufSend;
	rapidjson::WValue Data(rapidjson::kObjectType);
	bRet=JSonCtrl.AddObject(&Data, L"MachineStatus_s", L"");
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}
	bRet=JSonCtrl.AddObject(&Data, L"RemoteControl_s", L"");
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}
	bRet=JSonCtrl.AddObject(&Data, L"SoftwareStatus_s", L"ProgramList");
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	auto &alc = Doc.GetAllocator();
	rapidjson::WValue SoftwareData(rapidjson::kObjectType);	
	rapidjson::WValue objectarray(rapidjson::kArrayType);
	for ( i=0; i<FileCount; i++ )
	{
		if ( JetAPI::TCHAR2wstring(FileList[i], wsBuffer, ConvertCode) == false ) { continue; }
		rapidjson::WValue Object(wsBuffer.c_str(), wcslen(wsBuffer.c_str()), alc);
		objectarray.PushBack(Object, alc);	
	}		
	bRet=JSonCtrl.AddObject(&SoftwareData, L"ProgramList", objectarray);
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}
	
	bRet=JSonCtrl.AddObject(&Data, L"SoftwareStatusData", SoftwareData);
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
	
	bRet = JSonCtrl.GetBuffer(wsBuf, Doc);
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}	
	DumpMESDoc(Doc, L"BuildJSONRaw_SetProjectList");	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::BuildIPSDoc_SetProjectLoad(bool bAck, int AckTime, int &nPPID, CAOIProject *ProjectPtr, bool Success, std::wstring& wsBuf)//建立專案載入文檔
{	
	bool bRet=true;
	size_t     i=0;
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
	
	if ( true == bAckMode )
	{	bAck = true;	}

	if ( BuildIPSDoc(IPS_STATAUS_STATUS_MODE, bAck, AckTime, nPPID, Doc) == false )
	{	return false; }
	
	std::string  sBufSend;
	std::wstring wBufSend;
	rapidjson::WValue Data(rapidjson::kObjectType);
	bRet=JSonCtrl.AddObject(&Data, L"MachineStatus_s", L"");
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}
	bRet=JSonCtrl.AddObject(&Data, L"RemoteControl_s", L"");
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	if ( true==Success )
	{	bRet=JSonCtrl.AddObject(&Data, L"SoftwareStatus_s", L"ProgramLoad_OK");	}
	else
	{	bRet=JSonCtrl.AddObject(&Data, L"SoftwareStatus_s", L"ProgramLoad_NG");	}	
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
	
	bRet = JSonCtrl.GetBuffer(wsBuf, Doc);
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}	
	DumpMESDoc(Doc, L"BuildJSONRaw_SetProjectLoad");	
	return true;
}
//-------------------------------------------------------------------------------------//
CString CMES_IPS::GetIPSStatusText(int StatusID)
{
	CString str;
	switch ( StatusID )
	{
	case IPS_STATAUS_NONE:
		str = _T("None");
		break;
	case IPS_STATAUS_SETUP:
		str = _T("Setup");
		break;
	case IPS_STATAUS_INSPECTION_START:
		str = _T("Start");
		break;
	case IPS_STATAUS_INSPECTION_END:
		str = _T("End");
		break;

	case IPS_STATAUS_STATUS_MODE:
		str = _T("Status");
		break;
	case IPS_STATAUS_STATUS_READY_TO_LOAD:
		str = _T("Status_ReadyToLoad");
		break;
	case IPS_STATAUS_STATUS_LOAD_COMPLETE:
		str = _T("Status_LoadComplete");
		break;
	case IPS_STATAUS_STATUS_READY_TO_UNLOAD:
		str = _T("Status_ReadyToUnload");
		break;
	case IPS_STATAUS_STATUS_UNLOAD_COMPLETE:
		str = _T("Status_UnloadComplete");
		break;
	case IPS_STATAUS_STATUS_INSPECTION_STOP:
		str = _T("Status_InspectionStop");
		break;

	case IPS_STATAUS_STATUS_OFFLINE: str = _T("Status_Offline");	break;
	case IPS_STATAUS_STATUS_ONLINE:  str = _T("Status_Online");	break;	

	case IPS_STATAUS_AOI_CHECK_BARCODE: str = _T("Status_CheckBarcode"); break;

	case IPS_STATAUS_STATUS_ALARM:  str = _T("Status_Alarm");	break;	

	case IPS_STATAUS_REMOTE_CONTROL_START:	str = _T("Status_RemotControl_Start");	break;
	case IPS_STATAUS_REMOTE_CONTROL_STOP:	str = _T("Status_RemotControl_Stop");	break;	
	case IPS_STATAUS_REMOTE_CONTROL_PAUSE:	str = _T("Status_RemotControl_Pause");	break;	
	case IPS_STATAUS_REMOTE_CONTROL_RESUME:	str = _T("Status_RemotControl_Resume");	break;	
	case IPS_STATAUS_REMOTE_CONTROL_ABORT:	str = _T("Status_RemotControl_Abort");	break;	
	case IPS_STATAUS_REMOTE_CONTROL_BYPASS:	str = _T("Status_RemotControl_Bypass");	break;	

	case IPS_STATAUS_REMOTE_CONTROL_PROGRAM_LOAD:	str = _T("Status_Project_ProgramLoad");	break;
	case IPS_STATAUS_REMOTE_CONTROL_PROGRAM_LIST:	str = _T("Status_Project_ProgramList");	break;	
	
	case IPS_STATAUS_JET8000_END:
		str = _T("ProgramEnd");
		break;
	case IPS_STATAUS_JET8000_START:
		str = _T("ProgramStart");
		break;

	default:
		str.Format(_T("Undefined[%d]"), StatusID);
		break;
	}
	return str;
}
//-------------------------------------------------------------------------------------//
int CMES_IPS::MapIPSComm_ProcessID(int ProcessID)//映射ProcessID
{
	int NewProcessID=0;
	switch ( ProcessID )
	{
	case MES_STATAUS_AOI_READY_TO_LOAD_CMD: NewProcessID=IPS_STATAUS_STATUS_READY_TO_LOAD;	break;
	case MES_STATAUS_AOI_LOAd_COMPLETE_CMD: NewProcessID=IPS_STATAUS_STATUS_LOAD_COMPLETE;	break;
	case MES_STATAUS_AOI_START_INSPECTION_CMD: NewProcessID=IPS_STATAUS_INSPECTION_START;	break;
	case MES_STATAUS_AOI_INSPECTION_COMPLETE_CMD: NewProcessID=IPS_STATAUS_INSPECTION_END;	break;

	case MES_STATAUS_AOI_READY_TO_UNLOAD_CMD: NewProcessID=IPS_STATAUS_STATUS_READY_TO_UNLOAD;	break;
	case MES_STATAUS_AOI_UNLOAd_COMPLETE_CMD: NewProcessID=IPS_STATAUS_STATUS_UNLOAD_COMPLETE;	break;
	case MES_STATAUS_AOI_INSPECTION_STOP_CMD: NewProcessID=IPS_STATAUS_STATUS_INSPECTION_STOP;	break;	
	default:
		NewProcessID=IPS_STATAUS_NONE;
		break;
	}
	return NewProcessID;
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
	return DisconnectIPSLinker();
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::ExecMESComm_SetSystemParam()//執行MES溝通-設定系統參數
{
	return ExecIPSComm_SetSystemParam();
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::ExecMESComm_SetProjectParam(CAOIProject *ProjectPtr)//執行MES溝通-設定專案參數
{
	return ExecIPSComm_SetProjectParam(ProjectPtr);
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
bool CMES_IPS::ExecMESComm_SetRemoteControlMode(bool bOnline)//執行MES溝通-遠端控制模式	
{
	return ExecIPSComm_SetRemoteControlMode(bOnline);
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::ExecMESComm_CheckBarcode(CAOIProject *ProjectPtr)//執行MES溝通-確認條碼
{
	return ExecIPSComm_CheckBarcode(ProjectPtr);
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::ExecMESComm_SetProjectList()//執行MES溝通-條列條碼
{
	return ExecIPSComm_SetProjectList();
}
//-------------------------------------------------------------------------------------//
bool CMES_IPS::ExecMESComm_SetProjectLoad(CAOIProject *ProjectPtr, bool Success)//執行MES溝通-專案載入
{
	return ExecIPSComm_SetProjectLoad(ProjectPtr, Success);
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