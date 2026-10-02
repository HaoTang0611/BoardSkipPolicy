// MES_ITS.cpp: implementation of the CMES_ITS class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "MES_ITS.h"
#include "resource.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
#ifndef MES_DISABLE
#ifndef ITS_DISABLE
//-------------------------------------------------------------------------------------//
//ITS執行
unsigned int ITSProcThreadID = 0;//ITS執行執行緒編號
HANDLE ITSProcThreadHandle = NULL;//ITS執行執行緒處理碼
HANDLE ITSProcThreadEvent  = NULL;//ITS執行執行緒事件
unsigned int __stdcall ITSProcThreadFn(void *pParam);
//-------------------------------------------------------------------------------------//
unsigned int __stdcall ITSProcThreadFn(void *pParam)
{
#ifndef ITS_DISABLE
	bool IsOK = true;	
	DWORD  SleepTime = 10;//60 sec exec
	//const size_t ThreadIdx = (size_t)pParam;
	const size_t ThreadIdx = 0;
	CMES_ITS *Ptr=(CMES_ITS*)(pParam);
	THREAD_STATE_MODE   ThreadState = THREAD_STATE_NONE;
	THREAD_COMMAND_MODE ThreadCmd = THREAD_COMMAND_TO_NONE;		
	
	double Time = 0;
	LARGE_INTEGER          nStartTime;
	LARGE_INTEGER          nEndTime;
	TCHAR strBuffer[256]=_T("");

	while ( true )
	{	
		ThreadCmd = Ptr->GetITSProcThreadCmd();
		ThreadState = Ptr->GetITSProcThreadState();
		if ( THREAD_COMMAND_TO_EXIT == ThreadCmd )
		{	break;	}
		if ( THREAD_COMMAND_TO_IDLE == ThreadCmd ) 
		{ 			
			Ptr->SetITSProcThreadState(THREAD_STATE_IDLE);		
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
			Ptr->SetITSProcThreadState(THREAD_STATE_RUNNING);		
			QueryPerformanceCounter(&nStartTime);
			IsOK = Ptr->ExecITSProcFn();
			QueryPerformanceCounter(&nEndTime);			
			Time = (nEndTime.QuadPart - nStartTime.QuadPart) * 1000.0/AOIDataCollect.m_SystemFreq.QuadPart;
			const TSystemParameter &SystemParam = AOIDataCollect.GetSystemParameter();
			if ( FN_ENABLE == SystemParam.m_SaveITSProcThreadLog ) 
			{
				_stprintf(strBuffer, _T("ITSProcThreadFn[%d]: %.2f ms"), ThreadIdx+1, Time);
				AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, strBuffer);			
			}
		}
		::Sleep(SleepTime);
	};
	Ptr->SetITSProcThreadState(THREAD_STATE_NONE);
	if ( NULL != ITSProcThreadEvent )
	{	::SetEvent(ITSProcThreadEvent); }
#endif//ITS_DISABLE
	return 0;
}
//-------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
CMES_ITS::CMES_ITS()
{	
	PreInitITS();
	InitialITS();
}
//-------------------------------------------------------------------------------------//
CMES_ITS::~CMES_ITS()
{
	DeleteITSProcThread();
}
//-------------------------------------------------------------------------------------//
void CMES_ITS::PreInitITS()
{		
	m_MsgPPID = 0;	
	m_ITSProcThreadState = THREAD_STATE_NONE;
	m_ITSProcThreadCmd = THREAD_COMMAND_TO_NONE;
}
//-------------------------------------------------------------------------------------//
void CMES_ITS::InitialITS()
{
}
//-------------------------------------------------------------------------------------//
void CMES_ITS::CloneITS(const CMES_ITS &other)
{
}
//-------------------------------------------------------------------------------------//
bool CMES_ITS::CheckProjectPtr(CAOIProject *ProjectPtr)//確認專案指標
{
	if ( NULL == ProjectPtr )
	{
		m_ErrorString = _T("Error, No Project Ptr");
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CMES_ITS::LockITSProc()//鎖住ITS執行緒同步化
{
	CMES_Imp::LockImpProc();	
}
//-------------------------------------------------------------------------------------//
void CMES_ITS::UnlockITSProc()//釋放ITS執行緒同步化
{
	CMES_Imp::UnlockImpProc();	
}
//-------------------------------------------------------------------------------------//
bool CMES_ITS::ExecITSProcFn()//執行ITS執行執行緒
{		
#ifndef ITS_DISABLE
	const bool bThread = true;
	THREAD_COMMAND_MODE    ThreadCmd;	
	while ( true )
	{
		ThreadCmd = GetITSProcThreadCmd();
		if ( AOIDataCollect.GetIsSystemReleased() == true ) { return true; }
		if ( THREAD_COMMAND_TO_IDLE == ThreadCmd ) { return true; }
		if ( THREAD_COMMAND_TO_EXIT == ThreadCmd ) { return true; }		
		if ( THREAD_COMMAND_TO_NONE == ThreadCmd ) { return true; }			

		if ( RecvNewITSRecvNodeList() == false )
		{	
			::Sleep(10);
			continue; 
		}
		if ( ProcesITSRecvNodeList(bThread) == false ) 
		{	
			::Sleep(10);
			continue; 
		}
		::Sleep(10);
	}
#endif//ITS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_ITS::CreateITSProcThread()//建立ITS執行執行緒
{
#ifndef ITS_DISABLE
	if ( DeleteITSProcThread() == false ) { return false; }	
	m_ITSProcThreadCmd = THREAD_COMMAND_TO_IDLE;	

	ITSProcThreadHandle = (HANDLE)::_beginthreadex(NULL, NULL, &ITSProcThreadFn, (void*)this, NULL, &ITSProcThreadID);	
	if ( NULL == ITSProcThreadHandle )
	{	
		m_ITSProcThreadState = THREAD_STATE_NONE;
		m_ErrorString = _T("Eorror, Create Thread Fault (ITSProcThreadHandle == NULL)");		
		AOIExceptionCodeCtrl.SetAOIExceptionCode_Thread(AOI_EXCEPTION_THREAD_CREATE, m_ErrorString);
		return false;
	}		
	ITSProcThreadEvent = JetAPI::CreateEvent(NULL, TRUE, TRUE, _T("ITS Proc Event"));		
	::SetThreadPriority(ITSProcThreadHandle, THREAD_PRIORITY_BELOW_NORMAL); 
	//::SetThreadAffinityMask(ITSProcThreadHandle, 0x03);//保留2個		
	StartITSProcThread(false);
#else
	ITSProcThreadHandle = NULL;
	ITSProcThreadEvent = NULL;
#endif//ITS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_ITS::DeleteITSProcThread()//刪除ITS執行執行緒
{
#ifndef ITS_DISABLE
	DWORD WaitTime = 10000;//1 sec	
	if ( NULL == ITSProcThreadHandle ) { return true; }		
	SetITSProcThreadCmd(THREAD_COMMAND_TO_EXIT);	
	DWORD Res = ::WaitForSingleObject(ITSProcThreadHandle, WaitTime);
	::CloseHandle(ITSProcThreadHandle); 
	ITSProcThreadHandle = NULL;
	if ( NULL != ITSProcThreadEvent )
	{	
		::CloseHandle(ITSProcThreadEvent); 
		ITSProcThreadEvent=NULL; 
	}	
#endif//ITS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_ITS::StopITSProcThread(bool WaitOn)//開始ITS執行執行緒
{
#ifndef ITS_DISABLE
	size_t i=0;
	if ( NULL == ITSProcThreadHandle ) { return true; }
	if ( NULL != ITSProcThreadEvent )
	{	::ResetEvent(ITSProcThreadEvent); }
	SetITSProcThreadCmd(THREAD_COMMAND_TO_IDLE);	

	if ( WaitOn == true ) 
	{	
		const size_t MaxCount = 100;
		const size_t SleepTime = 10;
		for ( i=0; i<MaxCount; i++ )
		{	
			if ( THREAD_STATE_NONE == m_ITSProcThreadState )
			{	break; }
			if ( THREAD_STATE_IDLE == m_ITSProcThreadState )
			{	break; }
			//if ( THREAD_STATE_FINISH == m_ITSProcThreadState )//有可能後面才完成, 所以要加上完成確認
			//{	break; }
			::Sleep(SleepTime);
		}
		if ( i == MaxCount )
		{
			m_ErrorString.Format(_T("Error, wait for StopITSProcThread too long"));			
			return false;
		}		
	}
#endif//ITS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_ITS::StartITSProcThread(bool WaitOn)//開始ITS執行執行緒
{
#ifndef ITS_DISABLE
	size_t i=0;
	if ( NULL == ITSProcThreadHandle ) { return true; }
	if ( NULL != ITSProcThreadEvent )
	{	::ResetEvent(ITSProcThreadEvent); }
	if ( THREAD_STATE_FINISH == m_ITSProcThreadState )
	{  SetITSProcThreadState(THREAD_STATE_IDLE); }
	SetITSProcThreadCmd(THREAD_COMMAND_TO_RUN);	

	if ( false == WaitOn )
	{	return true; }
	if ( WaitForITSProcThreadStart() == false )
	{	return false; }	
#endif//ITS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_ITS::WaitForITSProcThreadIdle()//等待ITS執行執行緒停止
{
#ifndef ITS_DISABLE
	size_t i=0;	
	DWORD Res = 0;		
	const DWORD SleepTime = 100;	
	const size_t MaxCounts = 200000;
	for ( i=0; i<MaxCounts; i++ )
	{
		if ( AOIDataCollect.GetIsSystemReleased() == true ) { return true; }				
		//if ( AOIDataCollect.GetIsSystemException() == true ) { return true; }
		if ( THREAD_STATE_NONE == m_ITSProcThreadState ) { return true; }
		if ( THREAD_STATE_IDLE == m_ITSProcThreadState ) { return true; }
		if ( SleepTime > 0 )
		{	::Sleep(SleepTime); }
	}
	if ( i == MaxCounts )
	{
		m_ErrorString.Format(_T("Error, Wait for WaitForITSProcThreadIdle too long"));
		AOIExceptionCodeCtrl.SetAOIExceptionCode_Thread(AOI_EXCEPTION_THREAD_WAIT_FOR_IDLE, m_ErrorString);
		return false;
	}	
#endif//ITS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_ITS::WaitForITSProcThreadStop()//等待ITS執行執行緒停止
{
#ifndef ITS_DISABLE
	size_t i=0;	
	DWORD Res = 0;		
	const DWORD SleepTime = 100;	
	const size_t MaxCounts = 200000;
	for ( i=0; i<MaxCounts; i++ )
	{
		if ( AOIDataCollect.GetIsSystemReleased() == true ) { return true; }				
		//if ( AOIDataCollect.GetIsSystemException() == true ) { return true; }
		if ( THREAD_STATE_RUNNING != m_ITSProcThreadState ) { return true; }		
		if ( SleepTime > 0 )
		{	::Sleep(SleepTime); }
	}
	if ( i == MaxCounts )
	{
		m_ErrorString.Format(_T("Error, Wait for WaitForITSProcThreadStop too long"));
		AOIExceptionCodeCtrl.SetAOIExceptionCode_Thread(AOI_EXCEPTION_THREAD_WAIT_FOR_STOP, m_ErrorString);		
		return false;
	}	
#endif//ITS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_ITS::WaitForITSProcThreadStart()//等待ITS執行執行緒開始
{
#ifndef ITS_DISABLE		
	size_t i=0;
	const size_t MaxCount = 100;
	const size_t SleepTime = 10;
	for ( i=0; i<MaxCount; i++ )
	{
		if ( THREAD_COMMAND_TO_RUN != m_ITSProcThreadCmd )//切入下一個階段
		{	break; }
		if ( THREAD_STATE_IDLE != m_ITSProcThreadState )
		{	break; }
		::Sleep(SleepTime);
	}
	if ( i == MaxCount )
	{
		m_ErrorString.Format(_T("Error, wait for StartITSProcThread too long"));	
		AOIExceptionCodeCtrl.SetAOIExceptionCode_Thread(AOI_EXCEPTION_THREAD_WAIT_FOR_START, m_ErrorString);
		return false;
	}
#endif//ITS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_ITS::WaitForITSProcThreadFinish()//等待ITS執行執行緒結束
{
#ifndef ITS_DISABLE
	size_t i=0;	
	DWORD Res = 0;		
	const DWORD SleepTime = 100;	
	const size_t MaxCounts = 200000;

	if ( NULL == ITSProcThreadEvent ) { return true; }
	for ( i=0; i<MaxCounts; i++ )
	{
		if ( AOIDataCollect.GetIsSystemReleased() == true ) { return true; }				
		if ( AOIDataCollect.GetIsSystemException() == true ) { return true; }
		if ( THREAD_COMMAND_TO_EXIT == m_ITSProcThreadCmd ) { return true; }		
		if ( THREAD_COMMAND_TO_IDLE == m_ITSProcThreadCmd )
		{	break; }

		Res = ::WaitForSingleObject(ITSProcThreadEvent, SleepTime);
		if ( Res != WAIT_TIMEOUT ) 
		{
			if ( THREAD_STATE_RUNNING == m_ITSProcThreadState )
			{
				if ( SleepTime > 0 ) { ::Sleep(SleepTime); }
				continue; 
			}
			break; 
		}
	}
	if ( i == MaxCounts )
	{
		m_ErrorString.Format(_T("Error, Wait for WaitForITSProcThreadFinish too long"));
		AOIExceptionCodeCtrl.SetAOIExceptionCode_Thread(AOI_EXCEPTION_THREAD_WAIT_FOR_FINISH, m_ErrorString);
		return false;
	}	
#endif//ITS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_ITS::CheckITSProcThreadState(THREAD_STATE_MODE State)//確認ITS執行執行緒狀態
{
	if ( NULL == ITSProcThreadHandle ) { return true; }
	if ( m_ITSProcThreadState != State )
	{
		m_ErrorString.Format(_T("Error, m_ITSProcThreadState is Exception"));
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CMES_ITS::SetITSProcThreadState(THREAD_STATE_MODE State)//設定ITS執行執行緒狀態
{
	if ( m_ITSProcThreadState == State ) { return; }
	LockITSProc();
	m_ITSProcThreadState = State;
	UnlockITSProc();
}
//-------------------------------------------------------------------------------------//
THREAD_STATE_MODE CMES_ITS::GetITSProcThreadState()//取得ITS執行執行緒狀態
{
	return m_ITSProcThreadState;
}
//-------------------------------------------------------------------------------------//
void CMES_ITS::SetITSProcThreadCmd(THREAD_COMMAND_MODE Cmd)//設定ITS執行執行緒命令
{
	if ( m_ITSProcThreadCmd == Cmd ) { return; }
	LockITSProc();
	m_ITSProcThreadCmd = Cmd;
	UnlockITSProc();
}
//-------------------------------------------------------------------------------------//
THREAD_COMMAND_MODE CMES_ITS::GetITSProcThreadCmd()//取得ITS執行執行緒命令
{
	return m_ITSProcThreadCmd;
}
//-------------------------------------------------------------------------------------//
CITSLinker& CMES_ITS::GetITSLinker()//取得ITS連線參考
{
	return m_MESLinker;
}
//-------------------------------------------------------------------------------------//
bool CMES_ITS::ConnectITSLinker()//連線到MES連結軟體
{
#ifndef ITS_DISABLE
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
	CITSLinker &ITSLinker=GetITSLinker();
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
#endif//ITS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_ITS::DisconnectITSLinker()//停止連線到ITS連結軟體
{
	CITSLinker &ITSLinker=GetITSLinker();
	ITSLinker.SetITSThreadStop(true);
	ITSLinker.DisconnectITS();
	return true;
}
//-------------------------------------------------------------------------------------//
int CMES_ITS::GetITSStatusID_Ack(int StatusID)//取得ITS的回應訊息
{
	int AckID=ITS_STATAUS_NONE;
	switch ( StatusID )
	{
	case ITS_STATAUS_MES_SET_CMD:	
		AckID = ITS_STATAUS_MES_SET_RES;
		break;

	case ITS_STATAUS_MES_GET_CMD:	
		AckID = ITS_STATAUS_MES_GET_RES;
		break;

	case ITS_STATAUS_AOI_SET_CMD:	
		AckID = ITS_STATAUS_AOI_SET_RES;
		break;

	case ITS_STATAUS_AOI_GET_CMD:	
		AckID = ITS_STATAUS_AOI_GET_RES;
		break;

	case ITS_STATAUS_VRS_SET_CMD:
		AckID = ITS_STATAUS_VRS_SET_RES;
		break;
	case ITS_STATAUS_VRS_GET_CMD:
		AckID = ITS_STATAUS_VRS_GET_RES;
		break;

	case ITS_STATAUS_AOI_READY_TO_LOAD_CMD:	
		AckID = ITS_STATAUS_AOI_READY_TO_LOAD_RES;
		break;

	case ITS_STATAUS_AOI_LOAd_COMPLETE_CMD:
		AckID = ITS_STATAUS_AOI_LOAd_COMPLETE_RES;
		break;

	case ITS_STATAUS_AOI_START_INSPECTION_CMD:	
		AckID = ITS_STATAUS_AOI_START_INSPECTION_RES;
		break;

	case ITS_STATAUS_AOI_INSPECTION_COMPLETE_CMD:	
		AckID = ITS_STATAUS_AOI_INSPECTION_COMPLETE_RES;
		break;

	case ITS_STATAUS_AOI_READY_TO_UNLOAD_CMD:
		AckID = ITS_STATAUS_AOI_READY_TO_UNLOAD_RES;
		break;

	case ITS_STATAUS_AOI_UNLOAd_COMPLETE_CMD:	
		AckID = ITS_STATAUS_AOI_UNLOAd_COMPLETE_RES;
		break;

	case ITS_STATAUS_AOI_LOGIN_OUT_CMD:	
		AckID = ITS_STATAUS_AOI_LOGIN_OUT_RES;
		break;
	case ITS_STATAUS_AOI_CHECK_BARCODE_CMD:
		AckID = ITS_STATAUS_AOI_CHECK_BARCODE_RES;
		break;
	case ITS_STATAUS_AOI_INSPECTION_STOP_CMD:
		AckID = ITS_STATAUS_AOI_INSPECTION_STOP_RES;
		break;

	case ITS_STATAUS_VRS_UPLOAD_SFC_CMD:	
		AckID = ITS_STATAUS_VRS_UPLOAD_SFC_RES;
		break;	
	}
	return AckID;
}
//-------------------------------------------------------------------------------------//
bool CMES_ITS::CheckITSStatusID_Ack(int StatusID)//確認ITS的回應訊息
{
	bool bAckID = false;
	switch ( StatusID )
	{
	case ITS_STATAUS_MES_SET_RES:
	case ITS_STATAUS_MES_GET_RES:
	case ITS_STATAUS_AOI_SET_RES:
	case ITS_STATAUS_AOI_GET_RES:
	case ITS_STATAUS_VRS_SET_RES:
	case ITS_STATAUS_VRS_GET_RES:
	case ITS_STATAUS_AOI_READY_TO_LOAD_RES:
	case ITS_STATAUS_AOI_LOAd_COMPLETE_RES:
	case ITS_STATAUS_AOI_START_INSPECTION_RES:
	case ITS_STATAUS_AOI_INSPECTION_COMPLETE_RES:
	case ITS_STATAUS_AOI_READY_TO_UNLOAD_RES:
	case ITS_STATAUS_AOI_UNLOAd_COMPLETE_RES:
	case ITS_STATAUS_AOI_LOGIN_OUT_RES:
	case ITS_STATAUS_AOI_CHECK_BARCODE_RES:
	case ITS_STATAUS_VRS_UPLOAD_SFC_RES:
		bAckID = true;
		break;
	default:
		bAckID = false;
		break;
	}
	return bAckID;
}
//-------------------------------------------------------------------------------------//
size_t CMES_ITS::GetITSRecvNodeCount()
{
	return m_ITSRecvNodeList.size();
}
//-------------------------------------------------------------------------------------//
bool CMES_ITS::RecvNewITSRecvNodeList()//接收新的ITS訊息列表
{	
#ifndef ITS_DISABLE
	size_t i=0;
	std::vector<std::string> RecvNodeList;	

	LockITSProc();
	CITSLinker &ITSLinker=GetITSLinker();
	ITSLinker.CloneITSRecvMsgList(RecvNodeList, true);
	const size_t RecvNodeCount = RecvNodeList.size();	
	for ( i=0; i<RecvNodeCount; i++ )
	{	
		if ( AddITSRecvNode(RecvNodeList[i]) == false ) 
		{
			UnlockITSProc();
			m_ErrorString = _T("Error, Receive ITS Node Fault");
			return false;
		}				
	}			
	UnlockITSProc();
#endif//ITS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_ITS::AddITSRecvNode(const std::string &sBuff)
{
#ifndef ITS_DISABLE
	TITSCommNode CommNode;
	if ( DecoderITSPacket(sBuff, CommNode) == false )
	{	return false; }
	m_ITSRecvNodeList.push_back(CommNode);	
#endif//ITS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_ITS::DecoderITSPacket(const std::string &sBuff, TITSCommNode &sNode)
{
	int bRet=false;
	std::wstring wsBuf;
	rapidjson::CGMItr itr;
	rapidjson::WDocument Doc;
	rapidjson::CJsonCtrl JSonCtrl;

	if ( JetAPI::char2wstring(sBuff.c_str(), wsBuf, CP_UTF8) == 0 ) 
	{	return false; }

	//initial
	Doc.SetObject();
	JSonCtrl.Set(&Doc);			
	if ( JSonCtrl.SetBuffer(wsBuf, Doc) == false ) 
	{	return false; }
	
	sNode = TITSCommNode();
	std::wstring wsIdentifier;
	//"Identifier":"JET", 
	if ( JSonCtrl.FindMember(L"Identifier", itr) == false ) 
	{	return false; }				
	if ( itr->value.IsString() == false ) 
	{	return false; }
	wsIdentifier = itr->value.GetString();
	JetAPI::wstring2upper(wsIdentifier);
	if ( wsIdentifier!=AOI3D_VENDOR_W )//L"JET"
	{	return false; }

	/*
	if ( JSonCtrl.FindMember(L"Ver", itr) == false ) 
	{	return false; }				
	if ( itr->value.IsInt() == false ) 
	{	return false; }
	sNode.nVer = itr->value.GetInt();
	*/

	if ( JSonCtrl.FindMember(L"Dir", itr) == false ) 
	{	return false; }				
	if ( itr->value.IsInt() == false ) 
	{	return false; }
	sNode.nDir = itr->value.GetInt();

	if ( JSonCtrl.FindMember(L"Status", itr) == false ) 
	{	return false; }				
	if ( itr->value.IsInt() == false ) 
	{	return false; }
	sNode.nStatus = itr->value.GetInt();

	if ( JSonCtrl.FindMember(L"PPID", itr) == false ) 
	{	return false; }				
	if ( itr->value.IsInt() == false ) 
	{	return false; }
	sNode.nPPID = itr->value.GetInt();

	if ( JSonCtrl.FindMember(L"ACK", itr) == false ) 
	{	return false; }				
	if ( itr->value.IsInt() == false ) 
	{	return false; }
	sNode.nAck = itr->value.GetInt();

	if ( JSonCtrl.FindMember(L"Timeout", itr) == false ) 
	{	return false; }				
	if ( itr->value.IsInt() == false ) 
	{	return false; }
	sNode.nTimeout = itr->value.GetInt();

	if ( JSonCtrl.FindMember(L"Error_Code", itr) == false ) 
	{	return false; }				
	if ( itr->value.IsInt() == false ) 
	{	return false; }
	sNode.nErrorCode = itr->value.GetInt();

	if ( JSonCtrl.FindMember(L"Error_Str", itr) == false ) 
	{	return false; }				
	if ( itr->value.IsString() == false ) 
	{	return false; }
	sNode.wsErrorCode = itr->value.GetString();

	sNode.sRawData = sBuff;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_ITS::GetITSRecvNode(size_t index, bool bCheck, TITSCommNode &sNode)
{
	if ( true == bCheck )
	{
		const size_t Count = m_ITSRecvNodeList.size();
		if ( index >= Count ) 
		{	return false; }
	}
	sNode = m_ITSRecvNodeList[index];
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CMES_ITS::ProcesITSRecvNodeList(bool bThread)//處理收到ITS的訊息列表
{
#ifndef ITS_DISABLE
	size_t       i=0;
	bool         bBreak=false;
	bool         bRemove = false;
	std::vector<size_t> EraseIndexList;	

	bBreak=false;
	LockITSProc();
	const size_t RecvCount = m_ITSRecvNodeList.size();
	for ( i=0; i<RecvCount; i++ )
	{
		if ( ProcesITSRecvNode(bThread, m_ITSRecvNodeList[i], bBreak, bRemove) == false )
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
		m_ITSRecvNodeList.erase(m_ITSRecvNodeList.begin()+EraseIndex);
	}
	UnlockITSProc();
#endif//ITS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_ITS::ProcesITSRecvNode(bool bThread, TITSCommNode &sNode, bool &bBreak, bool &bRemove)//處理收到ITS的訊息
{
#ifndef ITS_DISABLE
	sNode.nChkCount ++;	
	//確認是否需要回傳
	if ( 0 == sNode.nAck ) { return true; }
	
	const int StatusRes = GetITSStatusID_Ack(sNode.nStatus);	
	if ( ITS_STATAUS_NONE == StatusRes ) 
	{	return true; }	
	
	switch ( sNode.nStatus )
	{
	case ITS_STATAUS_MES_SET_CMD:
		if ( ExecITSComm_MesSetParam(bThread, sNode, bBreak, bRemove) == false ) 
		{	break; }
		break;
	case ITS_STATAUS_MES_GET_CMD:
		break;
	}
	if ( true == bBreak ) 
	{	return true; }

#endif//ITS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_ITS::SendToITSNode(const char *strBuf, bool bAck, int nPPID, DWORD AckTime)//送資料給ITS
{
#ifndef ITS_DISABLE
	if ( false == bAck )
	{	bAck = bAck;	}
	CString str;
	CString Err;
	CITSLinker &ITSLinker=GetITSLinker();
	//if ( GetFreezeMESFuncMode() == true ) { return true; }
	if ( WaitForFreezeMESFuncModeDone() == false )
	{	return false; }

	if ( ITSLinker.GetITSConnected() == false )
	{
		str = _T("Error, ITS Client Not connected");
		m_ErrorString = GetMESMultiLanguage(str);
		return false;
	}

	m_ITSSendBuffer = strBuf;
	if ( ITSLinker.AddITSSendMsg(m_ITSSendBuffer) == false )
	{
		str = _T("Error, ITS Client Send Bytes Fault");
		m_ErrorString = GetMESMultiLanguage(str);
		return false;
	}

	if ( true == bAck  ) 
	{		
		if ( StopITSProcThread(true) == false )
		{	return false; }

		CString ErrorString;
		bool bIsOK = WaitForITSResponse(nPPID, AckTime);
		if ( bIsOK == false ) 
		{	ErrorString = GetErrorString(); }
		StartITSProcThread(false);
		if ( bIsOK == false ) 
		{	
			SetErrorString(ErrorString); 
			return false;
		}
	}
#endif//ITS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_ITS::WaitForITSResponse(int nPPID, DWORD AckTime)//等待ITS回傳資料
{
#ifndef ITS_DISABLE
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
	LastRecvCount = m_ITSRecvNodeList.size();
	while ( true ) 
	{	
		if ( RecvNewITSRecvNodeList() == false ) 
		{
			Err = GetErrorString();
			str = _T("Error, RecvNewITSRecvNodeList Fault");
			str = GetMESMultiLanguage(str);
			m_ErrorString.Format(_T("%s\n%s"), str, Err);
			return false; 
		}

		LockITSProc();
		CurRecvNodeCount = m_ITSRecvNodeList.size();
		for ( i=LastRecvCount; i<CurRecvNodeCount; i++ )
		{
			TITSCommNode &sNode = m_ITSRecvNodeList[i];				

			if ( sNode.nPPID == nPPID )
			{
				bFound = true;
				int nErrorCode = sNode.nErrorCode;
				bool bAckID = CheckITSStatusID_Ack(sNode.nStatus);
				CString strErrorCode = sNode.wsErrorCode.c_str();
				m_ITSRecvNodeList.erase(m_ITSRecvNodeList.begin()+i);
				if ( 0 != nErrorCode )
				{
					UnlockITSProc();
					str = _T("Error, ITS");
					str = GetMESMultiLanguage(str);
					m_ErrorString.Format(_T("%s %s"), str, strErrorCode);
					return false;
				}
				break;
			}
			else
			{	bFound = false;	}
		}	
		UnlockITSProc();

		if ( true == bFound )
		{	break; }

		ChkCount ++;
		if ( ChkCount >= MaxChkCount ) 
		{	break; }

		::Sleep(SleepTime);
	};

	if ( ChkCount == MaxChkCount )
	{		
		str = _T("Error, wait for ITS response too long");
		str = GetMESMultiLanguage(str);
		m_ErrorString.Format(_T("%s [PPID:%04d]"), str, nPPID);
		return false;
	}
#endif//ITS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_ITS::ExecITSComm_MesSetParam(bool bThread, TITSCommNode &sNode, bool &bBreak, bool &bRemove)
{	
#ifndef ITS_DISABLE
	int bRet=false;
	std::wstring wsBuf;
	rapidjson::CGMItr itr;	
	rapidjson::WDocument Doc;
	rapidjson::CJsonCtrl JSonCtrl;
	const UINT ConvertCode = CP_UTF8;
	const int nStatus = sNode.nStatus;
	const std::string sBuff = sNode.sRawData;
	
	if ( JetAPI::char2wstring(sBuff.c_str(), wsBuf, ConvertCode) == 0 ) 
	{	return false; }

	//initial
	Doc.SetObject();
	JSonCtrl.Set(&Doc);			
	if ( JSonCtrl.SetBuffer(wsBuf, Doc) == false ) 
	{	return false; }

	if ( JSonCtrl.FindMember(L"Data_Info", itr) == false ) 
	{	return false; }				
	if ( itr->value.IsObject() == false ) 
	{	return false; }

	CString str;
	bool  bReturn=false;
	int   nMachineStatus=0;
	int   nErrorCode=false;
	std::wstring wsErrorCode;	
	std::wstring wsMemName;		
	rapidjson::CGMItr itrLv2;	

	bRemove = true;
	itrLv2 = itr->value.FindMember(L"Machine_Status");
	if ( itrLv2 != itr->value.MemberEnd() )
	{
		if ( itrLv2->value.IsInt() == false ) 
		{	return false; }
		
		nMachineStatus = itrLv2->value.GetInt();
		TASK_STATE_MODE  OnlineTaskState = GetOnlineTaskState();
		switch ( nMachineStatus )
		{			
		case MES_CMD_TO_STOP:								
			bReturn = true;				
			switch ( OnlineTaskState )
			{
			case TASK_STATE_NONE:						
				break;
			case TASK_STATE_RUNNING:
				if ( true == bThread ) 
				{	
					bBreak = true; 
					bRemove = false;
					bReturn = false;
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
		case MES_CMD_TO_RUN:
			if ( false==bThread ) 
			{	
				bBreak = true;	
				bRemove = false;
			}
			else
			{	
				bReturn = true;
				bool bAutoReset = false;
				bool bChkStartLight = true;
				switch ( OnlineTaskState )
				{					
				case TASK_STATE_RUNNING:
					nErrorCode = 3;
					wsErrorCode = L"Running";
					break;
				case TASK_STATE_TO_STOP:
					nErrorCode = 5;
					wsErrorCode = L"Stopping";
					break;
				case TASK_STATE_NONE:
				case TASK_STATE_IDLE:
					if ( CheckSystemReady(bChkStartLight, bAutoReset) == true ) 
					{
						CAOIProject *ProjectPtr = GetActiveProject();
						if ( NULL != ProjectPtr ) 
						{	PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_MES_CMD_ONLINE_RUN, NULL);	}
						else
						{
							nErrorCode = 2;
							wsErrorCode = L"No Project Opened";							
						}
					}
					else
					{							
						nErrorCode = 1;
						str = GetErrorString();
						JetAPI::TCHAR2wstring(str, wsErrorCode, ConvertCode);
					}
					break;
				}
			}				
			break;			
		case MES_CMD_TO_BYPASS:
			if ( false==bThread ) 
			{	
				bBreak = true;	
				bRemove = false;
			}
			else
			{
				
				bReturn = true;
				bool bAutoReset = false;
				bool bChkStartLight = true;
				switch ( OnlineTaskState )
				{
				case TASK_STATE_RUNNING:
					nErrorCode = 3;
					wsErrorCode = L"Running";
					break;
				case TASK_STATE_TO_STOP:
					nErrorCode = 5;
					wsErrorCode = L"Stopping";
					break;
				case TASK_STATE_NONE:
				case TASK_STATE_IDLE:
					if ( CheckSystemReady(bChkStartLight, bAutoReset) == true ) 
					{	PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_MES_CMD_ONLINE_BYPASS, NULL);	}
					else
					{							
						nErrorCode = 1;
						str = GetErrorString();
						JetAPI::TCHAR2wstring(str, wsErrorCode, ConvertCode);
					}					
					break;
				}
			}
			break;
		}
		if ( true == bReturn )
		{
			const int nStatusRet = GetITSStatusID_Ack(nStatus);
			if ( ITS_STATAUS_NONE != nStatusRet ) 
			{
				std::string  sBufSend;
				std::wstring wBufSend;
				std::wstring wstrStatus;
				CString strStatus = AOIDataDefine.GetMESStatusText(nStatusRet);
				JetAPI::TCHAR2wstring(strStatus, wstrStatus, ConvertCode);
		
				JSonCtrl.ModifyMember(L"Status", nStatusRet);
				JSonCtrl.ModifyMember(L"Status_Str", wstrStatus);
				JSonCtrl.ModifyMember(L"Error_Code", nErrorCode);
				JSonCtrl.ModifyMember(L"Error_Str", wsErrorCode);

				JSonCtrl.GetBuffer(wBufSend, Doc);
				if ( JetAPI::wchar2string(wBufSend.c_str(), sBufSend, ConvertCode) == false ) 
				{	return false; }
				SendToITSNode(sBufSend.c_str(), false, sNode.nPPID, 0);
			}
		}
		if ( true==bBreak )
		{	return true; }		
	}	
#endif//ITS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_ITS::ExecITSComm_SetSystemParam()//執行ITS溝通-設定系統參數
{	
#ifndef ITS_DISABLE
	CString str;
	CString Err;
	int  nPPID = 0;
	bool bAck = true;
	int  AckTime = 1000;	
	std::wstring wsBuf;
	if ( BuildITSDoc_SetSystemParam(bAck, AckTime, nPPID, wsBuf) == false )
	{
		Err = GetErrorString();
		str = _T("Error, BuildITSDoc_SetSystemParam Fault");
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
	
	if ( SendToITSNode(sBuf.c_str(), bAck, nPPID, AckTime) == false ) 
	{	return false; }
#endif//ITS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_ITS::ExecITSComm_SetProjectParam(CAOIProject *ProjectPtr)//執行ITS溝通-設定專案參數
{	
#ifndef ITS_DISABLE
	CString str;
	CString Err;
	int  nPPID = 0;
	bool bAck = true;
	int  AckTime = 1000;	
	std::wstring wsBuf;
	if ( BuildITSDoc_SetProjectParam(bAck, AckTime, nPPID, ProjectPtr, wsBuf) == false )
	{
		Err = GetErrorString();
		str = _T("Error, BuildITSDoc_SetProjectParam Fault");
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
	
	if ( SendToITSNode(sBuf.c_str(), bAck, nPPID, AckTime) == false ) 
	{	return false; }
#endif//ITS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_ITS::ExecITSComm_SetMachineStatus()//執行ITS溝通-設定機台狀態
{
	ONLINE_STATE_MODE eStatus = AOIDataCollect.GetOnlineStateMode();	
	return ExecITSComm_SetMachineStatus(eStatus);
}
//-------------------------------------------------------------------------------------//
bool CMES_ITS::ExecITSComm_SetMachineStatus(ONLINE_STATE_MODE Status)//執行ITS溝通-設定機台狀態
{	
#ifndef ITS_DISABLE
	CString str;
	CString Err;
	int  nPPID = 0;
	bool bAck = false;
	int  AckTime = 1000;	
	std::wstring wsBuf;	
	const bool bAckMode = CheckITSAlwaysAckMode();
	if ( true == bAckMode )
	{	bAck = true;	}

	if ( BuildITSDoc_SetMachineStatus(Status, bAck, AckTime, nPPID, wsBuf) == false )
	{
		Err = GetErrorString();
		str = _T("Error, BuildITSDoc_SetMachineStatus Fault");
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
	
	if ( SendToITSNode(sBuf.c_str(), bAck, nPPID, AckTime) == false ) 
	{	return false; }
#endif//ITS_DISABLE
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CMES_ITS::ExecITSComm_ProcessID(int ProcessID)//執行ITS溝通-程序運作
{	
#ifndef ITS_DISABLE
	if ( ITS_STATAUS_NONE == ProcessID )
	{	return true; }

	CString str;
	CString Err;
	int  nPPID = 0;
	bool bAck = true;
	int  AckTime = 1000;	
	std::wstring wsBuf;
	if ( BuildITSDoc_ProcessID(ProcessID, bAck, AckTime, nPPID, wsBuf) == false )
	{
		Err = GetErrorString();
		str = _T("Error, BuildITSDoc_ProcessID Fault");
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
	
	if ( SendToITSNode(sBuf.c_str(), bAck, nPPID, AckTime) == false ) 
	{	return false; }
#endif//ITS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_ITS::ExecITSComm_SetUserLogin_out(bool bLogin)//執行ITS溝通-設定使用者登入
{
#ifndef ITS_DISABLE
	CString str;
	CString Err;
	int  nPPID = 0;
	bool bAck = true;
	int  AckTime = 1000;	
	std::wstring wsBuf;
	USER_LEVEL_MODE  UserLevelMode = AOIDataCollect.GetCurrentUserLevel();//使用者權限	
	if ( USER_LEVEL_SIGN_OUT==UserLevelMode ||UserLevelMode>=USER_LEVEL_JET_FAE )
	{	return true; }
	if ( BuildITSDoc_SetLogin_out(bAck, AckTime, nPPID, bLogin, wsBuf) == false )
	{
		Err = GetErrorString();
		str = _T("Error, BuildITSDoc_SetLogin_out Fault");
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
	
	if ( SendToITSNode(sBuf.c_str(), bAck, nPPID, AckTime) == false ) 
	{	return false; }
#endif//ITS_DISABLE
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CMES_ITS::ExecITSComm_CheckBarcode(CAOIProject *ProjectPtr)//執行ITS溝通-確認條碼
{
#ifndef ITS_DISABLE
	CString str;
	CString Err;
	int  nPPID = 0;
	bool bAck = true;
	int  AckTime = 1000;	
	std::wstring wsBuf;	
	if ( BuildITSDoc_CheckBarcode(bAck, AckTime, nPPID, ProjectPtr, wsBuf) == false )
	{
		Err = GetErrorString();
		str = _T("Error, BuildITSDoc_CheckBarcode Fault");
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
	
	if ( SendToITSNode(sBuf.c_str(), bAck, nPPID, AckTime) == false ) 
	{	return false; }
#endif//ITS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
int CMES_ITS::GetITSFreePPID()//取得ITS可用的PPID
{
	int uPPID = m_MsgPPID;
	m_MsgPPID ++;
	if ( m_MsgPPID > 0x0000FFFF ) 
	{	m_MsgPPID = 1; }
	return uPPID;
}
//-------------------------------------------------------------------------------------//
bool CMES_ITS::CheckITSAlwaysAckMode()//確認總是回傳模式
{
	ITS_LINK_MODE ITS_LinkMode = GetITSLinker().GetITSLinkMode();	
	if ( ITS_LINK_FILE != ITS_LinkMode )
	{	return false;	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_ITS::BuildITSDoc(int nStatus, bool bAck, int AckTime, int &nPPID, rapidjson::WDocument &Doc)//建立ITS檔案
{
	bool         bRet=false;	
	std::wstring wstrBuffer;
	CString      strStatus;	
	rapidjson::CJsonCtrl JSonCtrl;

	strStatus = GetITSStatusText(nStatus);
	JetAPI::TCHAR2wstring(strStatus, wstrBuffer);

	nPPID = GetITSFreePPID();	

	//initial	
	JSonCtrl.Set(&Doc);

	//Add value into doc
	bRet=JSonCtrl.AddMember(L"Identifier", AOI3D_VENDOR_W);//"Identifier":"JET",
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}
	bRet=JSonCtrl.AddMember(L"Ver", 0);//"Ver":0,
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}
	bRet=JSonCtrl.AddMember(L"Dir", 0);//"Dir":0,
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}
	bRet=JSonCtrl.AddMember(L"Status", nStatus);//"Status":33,
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}
	bRet=JSonCtrl.AddMember(L"Status_Str", wstrBuffer);//"Status_Str": "Set_Parameters",
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}
	bRet=JSonCtrl.AddMember(L"PPID", nPPID);//"PPID":2001,	
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}
	if ( true == bAck )//"ACK":1,		
	{	bRet=JSonCtrl.AddMember(L"ACK", 1); }
	else
	{	bRet=JSonCtrl.AddMember(L"ACK", 0); }
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}
	bRet=JSonCtrl.AddMember(L"Timeout", AckTime);//"Timeout": 1000,
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}
	bRet=JSonCtrl.AddMember(L"Error_Code", 0);//"Error_Code":0,
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}
	bRet=JSonCtrl.AddMember(L"Error_Str", L"OK");//"Error_Str":"OK",
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_ITS::BuildITSDoc_SetMachineStatus(ONLINE_STATE_MODE Status, bool bAck, int AckTime, int &nPPID, std::wstring& wsBuf)//建立機台運作文檔
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

	if ( BuildITSDoc(ITS_STATAUS_AOI_SET_CMD, bAck, AckTime, nPPID, Doc) == false )
	{	return false; }
	
	//add object into doc
	rapidjson::WValue object(rapidjson::kObjectType);	
	//add value into the object
	ONLINE_STATE_MODE eOnlineState = Status;
	bRet=JSonCtrl.AddObject(&object, L"Machine_Status", eOnlineState);//Machine_Status
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	str = AOIDataDefine.GetOnlineStateText(eOnlineState);
	JetAPI::TCHAR2wstring(str, wsBuffer, ConvertCode);
	bRet=JSonCtrl.AddObject(&object, L"Machine_Status_Str", wsBuffer);//Machine_Status_Str
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}	
	
	bRet=JSonCtrl.AddMember(L"Data_Info", object);
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
bool CMES_ITS::BuildITSDoc_SetSystemParam(bool bAck, int AckTime, int &nPPID, std::wstring& wsBuf)//建立系統資訊文檔
{
	bool bRet=true;	
	TSystemParameter &SysParam = AOIDataCollect.GetSystemParameter();	

	CString    str;
	std::wstring wsBuffer;
	rapidjson::WDocument Doc;
	rapidjson::CJsonCtrl JSonCtrl;
	const UINT ConvertCode = CP_UTF8;	
	CAMERA_ID CameraID =  PRIMARY_CAMERA_ID;
	const bool bAckMode = CheckITSAlwaysAckMode();
	double ResolutionX = AOIDataCollect.GetCameraResolutionX(CameraID);
	double ResolutionY = AOIDataCollect.GetCameraResolutionY(CameraID);
	
	nPPID = 0;	
	//initial
	Doc.SetObject();
	JSonCtrl.Set(&Doc);
	
	if ( true == bAckMode )
	{	bAck = true;	}

	if ( BuildITSDoc(ITS_STATAUS_AOI_SET_CMD, bAck, AckTime, nPPID, Doc) == false )
	{	return false; }
	
	//add object into doc
	rapidjson::WValue object(rapidjson::kObjectType);	
	//add value into the object
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
	bRet=JSonCtrl.AddObject(&object, L"Building", wsBuffer);//Build
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

	str = SysParam.m_MachineLine;
	//str = SysParam.m_MachineLine_LB;
	JetAPI::TCHAR2wstring(str, wsBuffer, ConvertCode);	
	bRet=JSonCtrl.AddObject(&object, L"Line", wsBuffer);//Line
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
	bRet=JSonCtrl.AddObject(&object, L"RoomName", wsBuffer);//RoomName
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	str = SysParam.m_MachineSN;
	JetAPI::TCHAR2wstring(str, wsBuffer, ConvertCode);	
	bRet=JSonCtrl.AddObject(&object, L"SN", wsBuffer);//SN
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	str = SysParam.m_MachineName;
	JetAPI::TCHAR2wstring(str, wsBuffer, ConvertCode);	
	bRet=JSonCtrl.AddObject(&object, L"Machine_Name", wsBuffer);//Machine_Name
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}
	
	//CString                    m_MachineVendor;//機台廠商
	//CString                    m_MachineAlias;//機台別名

	bRet=JSonCtrl.AddObject(&object, L"Machine_Type", 30);//Machine_Type
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}
	
	bRet=JSonCtrl.AddObject(&object, L"Resolution X", ResolutionX);//Resolution X
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	bRet=JSonCtrl.AddObject(&object, L"Resolution Y", ResolutionY);//Resolution Y
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}
	
	str = SysParam.m_MachineMES_Name;//MES登入名稱
	JetAPI::TCHAR2wstring(str, wsBuffer, ConvertCode);	
	bRet=JSonCtrl.AddObject(&object, L"MES_Login_Name", wsBuffer);
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}
	
	str = SysParam.m_MachineMES_Password;//MES登入密碼
	JetAPI::TCHAR2wstring(str, wsBuffer, ConvertCode);	
	bRet=JSonCtrl.AddObject(&object, L"MES_Login_Password", wsBuffer);
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	str = SysParam.m_MachineMES_Device;//MES登入裝置
	JetAPI::TCHAR2wstring(str, wsBuffer, ConvertCode);	
	bRet=JSonCtrl.AddObject(&object, L"MES_Device_Name", wsBuffer);
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	str = SysParam.m_MachineMES_Device2;//MES登入裝置-2
	JetAPI::TCHAR2wstring(str, wsBuffer, ConvertCode);	
	bRet=JSonCtrl.AddObject(&object, L"MES_Device_Name2", wsBuffer);
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}
	
	str = SysParam.m_MachineMES_CodeName;//MES-裝置代號
	JetAPI::TCHAR2wstring(str, wsBuffer, ConvertCode);	
	bRet=JSonCtrl.AddObject(&object, L"MES_Code_Name", wsBuffer);
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	/*				
	//"Insp_Result_File":"C:\\ABC\\1234.json" 
	*/

	bRet=JSonCtrl.AddMember(L"Data_Info", object);
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

	DumpMESDoc(Doc, L"BuildJSONRaw_SetSystemParam");
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_ITS::BuildITSDoc_SetProjectParam(bool bAck, int AckTime, int &nPPID, CAOIProject *ProjectPtr, std::wstring& wsBuf)//建立基本資訊文檔
{
	if ( CheckProjectPtr(ProjectPtr) == false )
	{	return false; }

	bool bRet=true;	
	TProjectParameter &ProgParam = ProjectPtr->GetProjectParameter();

	CString    str;
	std::wstring wsBuffer;
	rapidjson::WDocument Doc;
	rapidjson::CJsonCtrl JSonCtrl;
	const UINT ConvertCode = CP_UTF8;
	LANE_ID LaneID = GetActiveLaneID();
	CAMERA_ID CameraID =  PRIMARY_CAMERA_ID;
	const bool bAckMode = CheckITSAlwaysAckMode();
	double ResolutionX = AOIDataCollect.GetCameraResolutionX(CameraID);
	double ResolutionY = AOIDataCollect.GetCameraResolutionY(CameraID);
	
	nPPID = 0;	
	//initial
	Doc.SetObject();
	JSonCtrl.Set(&Doc);
	
	if ( true == bAckMode )
	{	bAck = true;	}

	if ( BuildITSDoc(ITS_STATAUS_AOI_SET_CMD, bAck, AckTime, nPPID, Doc) == false )
	{	return false; }
	
	//add object into doc
	rapidjson::WValue object(rapidjson::kObjectType);	
	//add value into the object
	str = ProjectPtr->GetProjectShowName();
	JetAPI::TCHAR2wstring(str, wsBuffer, ConvertCode);
	bRet=JSonCtrl.AddObject(&object, L"Project", wsBuffer);
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

	wsBuffer = ProgParam.m_ProjectModuleName;	
	bRet=JSonCtrl.AddObject(&object, L"Module", wsBuffer);//Module
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	wsBuffer = ProgParam.m_ProjectWorkNumber;
	bRet=JSonCtrl.AddObject(&object, L"Lot", wsBuffer);//Lot
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	str = AOIDataCollect.GetCurrentUserName();
	JetAPI::TCHAR2wstring(str, wsBuffer, ConvertCode);	
	bRet=JSonCtrl.AddObject(&object, L"Test_User", wsBuffer);//Test_User
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}
	
	bRet=JSonCtrl.AddObject(&object, L"Resolution X", ResolutionX);//Resolution X
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	bRet=JSonCtrl.AddObject(&object, L"Resolution Y", ResolutionY);//Resolution Y
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}
	
	/*				
	//"Insp_Result_File":"C:\\ABC\\1234.json" 
	*/

	bRet=JSonCtrl.AddMember(L"Data_Info", object);
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

	DumpMESDoc(Doc, L"BuildJSONRaw_SetProjectParam");
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_ITS::BuildITSDoc_ProcessID(int ProcessID, bool bAck, int AckTime, int &nPPID, std::wstring& wsBuf)//建立程序運作文檔
{
	bool bRet=false;			
	CString    str;	
	std::wstring wsBuffer;
	rapidjson::WDocument Doc;
	rapidjson::CJsonCtrl JSonCtrl;
	const UINT ConvertCode = CP_UTF8;
	const bool bAckMode = CheckITSAlwaysAckMode();
	CAOIProject *ProjectPtr = GetActiveProject();

	nPPID=0;	
	//initial
	Doc.SetObject();
	JSonCtrl.Set(&Doc);
	
	if ( true == bAckMode )
	{	bAck = true;	}

	if ( BuildITSDoc(ProcessID, bAck, AckTime, nPPID, Doc) == false )
	{	return false; }

	if ( ITS_STATAUS_AOI_INSPECTION_COMPLETE_CMD == ProcessID )
	{
		//add object into doc
		rapidjson::WValue object(rapidjson::kObjectType);	
		//add value into the object

		if ( NULL == ProjectPtr )
		{	str = _T("NoFile"); }
		else
		{			
			CString     strText;
			CString     ResultFolder=ProjectPtr->GetProjectSpcFileFolder();
			TTestResult ResultLatest = ProjectPtr->GetProjectResultCurrent();
			JetAPI::GetTime(strText, ResultLatest.sDateTimeS);
			str.Format(_T("%s\\%s.JSON"), ResultFolder, strText);
		}
		JetAPI::TCHAR2wstring(str, wsBuffer, ConvertCode);
		bRet=JSonCtrl.AddObject(&object, L"Insp_Result_File", wsBuffer);//Insp_Result_File
		if ( false == bRet ) 
		{
			m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
			return false; 
		}	
		bRet=JSonCtrl.AddMember(L"Data_Info", object);
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
bool CMES_ITS::BuildITSDoc_SetLogin_out(bool bAck, int AckTime, int &nPPID, bool Login, std::wstring& wsBuf)//建立登入參數文檔
{
	bool bRet=true;		

	int        nVal=0;
	CString    str;
	std::wstring wsBuffer;
	rapidjson::WDocument Doc;
	rapidjson::CJsonCtrl JSonCtrl;
	const UINT ConvertCode = CP_UTF8;		
	const bool bAckMode = CheckITSAlwaysAckMode();
	TUserNode  UserNode = AOIDataCollect.GetLoginUserNode();//使用者資料
	
	nPPID = 0;	
	//initial
	Doc.SetObject();
	JSonCtrl.Set(&Doc);
	
	if ( true == bAckMode )
	{	bAck = true;	}

	if ( BuildITSDoc(ITS_STATAUS_AOI_LOGIN_OUT_CMD, bAck, AckTime, nPPID, Doc) == false )
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

	bRet=JSonCtrl.AddMember(L"Data_Info", object);
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
bool CMES_ITS::BuildITSDoc_CheckBarcode(bool bAck, int AckTime, int &nPPID, CAOIProject *ProjectPtr, std::wstring& wsBuf)//建立確認條碼文檔
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
	const bool bAckMode = CheckITSAlwaysAckMode();			

	nPPID = 0;	
	//initial
	Doc.SetObject();
	JSonCtrl.Set(&Doc);
	
	if ( true == bAckMode )
	{	bAck = true;	}

	if ( BuildITSDoc(ITS_STATAUS_AOI_CHECK_BARCODE_CMD, bAck, AckTime, nPPID, Doc) == false )
	{	return false; }
	
	//add object into doc
	rapidjson::WValue object(rapidjson::kObjectType);		
	
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
			bRet=JSonCtrl.AddObject(&object, wsKey, object2);
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
				bRet=JSonCtrl.AddObject(&object, wsKey, object2);
				if ( false == bRet ) 
				{
					m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
					return false; 
				}

				BarcodeCount ++;
			}
		}
	}	
	
	bRet=JSonCtrl.AddObject(&object, L"Barcode_Count", BarcodeCount);
	if ( false == bRet ) 
	{
		m_ErrorString = JSonCtrl.GetLastErrorStr().c_str();
		return false; 
	}

	/*				
	//"Insp_Result_File":"C:\\ABC\\1234.json" 
	*/

	bRet=JSonCtrl.AddMember(L"Data_Info", object);
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
CString CMES_ITS::GetITSStatusText(int StatusID)
{
	CString str;
	switch ( StatusID )
	{
	case ITS_STATAUS_NONE: str = _T("None");	break;

	case ITS_STATAUS_MES_SET_CMD: str = _T("MES Set Parameters");	break;
	case ITS_STATAUS_MES_SET_RES: str = _T("MES Set Parameters Response");	break;
	case ITS_STATAUS_MES_GET_CMD: str = _T("MES Get Parameters");	break;
	case ITS_STATAUS_MES_GET_RES: str = _T("MES Get Parameters Response");	break;

	case ITS_STATAUS_AOI_SET_CMD: str = _T("AOI Set Parameters");	break;
	case ITS_STATAUS_AOI_SET_RES: str = _T("AOI Set Parameters Response");	break;
	case ITS_STATAUS_AOI_GET_CMD: str = _T("AOI Get Parameters");	break;
	case ITS_STATAUS_AOI_GET_RES: str = _T("AOI Get Parameters Response");	break;

	case ITS_STATAUS_VRS_SET_CMD: str = _T("VRS Set Parameters");	break;
	case ITS_STATAUS_VRS_SET_RES: str = _T("VRS Set Parameters Response");	break;
	case ITS_STATAUS_VRS_GET_CMD: str = _T("VRS Get Parameters");	break;
	case ITS_STATAUS_VRS_GET_RES: str = _T("VRS Get Parameters Response");	break;

	case ITS_STATAUS_AOI_READY_TO_LOAD_CMD: str = _T("AOI Ready To Load");	break;
	case ITS_STATAUS_AOI_READY_TO_LOAD_RES: str = _T("AOI Ready To Load Response");	break;

	case ITS_STATAUS_AOI_LOAd_COMPLETE_CMD: str = _T("AOI Load Complete");	break;
	case ITS_STATAUS_AOI_LOAd_COMPLETE_RES: str = _T("AOI Load Complete Response");	break;

	case ITS_STATAUS_AOI_START_INSPECTION_CMD: str = _T("AOI Start Inspection");	break;
	case ITS_STATAUS_AOI_START_INSPECTION_RES: str = _T("AOI Start Inspection Response");	break;

	case ITS_STATAUS_AOI_INSPECTION_COMPLETE_CMD: str = _T("AOI Inspection Complete");	break;
	case ITS_STATAUS_AOI_INSPECTION_COMPLETE_RES: str = _T("AOI Inspection Complete Response");	break;

	case ITS_STATAUS_AOI_READY_TO_UNLOAD_CMD: str = _T("AOI Ready To Unload");	break;
	case ITS_STATAUS_AOI_READY_TO_UNLOAD_RES: str = _T("AOI Ready To Unload Response");	break;

	case ITS_STATAUS_AOI_UNLOAd_COMPLETE_CMD: str = _T("AOI Unload Complete");	break;
	case ITS_STATAUS_AOI_UNLOAd_COMPLETE_RES: str = _T("AOI Unload Complete Response");	break;

	case ITS_STATAUS_AOI_LOGIN_OUT_CMD: str = _T("AOI Login/Out Parameters");	break;
	case ITS_STATAUS_AOI_LOGIN_OUT_RES: str = _T("AOI Login/Out Parameters Response");	break;		

	case ITS_STATAUS_AOI_CHECK_BARCODE_CMD: str = _T("AOI Check Barcode");	break;
	case ITS_STATAUS_AOI_CHECK_BARCODE_RES: str = _T("AOI Check Barcode Response");	break;		

	case ITS_STATAUS_AOI_INSPECTION_STOP_CMD: str = _T("AOI Inspection Stop");	break;
	case ITS_STATAUS_AOI_INSPECTION_STOP_RES: str = _T("AOI Inspection Stop Response");	break;		

	case ITS_STATAUS_VRS_UPLOAD_SFC_CMD: str = _T("VRS Upload SFC");	break;
	case ITS_STATAUS_VRS_UPLOAD_SFC_RES: str = _T("VRS Upload SFC Response");	break;		
	default:
		str.Format(_T("Undefined Status ID[%d]"), StatusID);
		break;
	}	          
	return str;
}
//-------------------------------------------------------------------------------------//
int CMES_ITS::MapITSComm_ProcessID(int ProcessID)//映射ProcessID
{
	int NewProcessID=0;
	switch ( ProcessID )
	{
	case MES_STATAUS_MES_SET_CMD: NewProcessID=ITS_STATAUS_MES_SET_CMD;	break;
	case MES_STATAUS_MES_SET_RES: NewProcessID=ITS_STATAUS_MES_SET_RES;	break;
	case MES_STATAUS_MES_GET_CMD: NewProcessID=ITS_STATAUS_MES_GET_CMD;	break;
	case MES_STATAUS_MES_GET_RES: NewProcessID=ITS_STATAUS_MES_GET_RES;	break;

	case MES_STATAUS_AOI_SET_CMD: NewProcessID=ITS_STATAUS_AOI_SET_CMD;	break;
	case MES_STATAUS_AOI_SET_RES: NewProcessID=ITS_STATAUS_AOI_SET_RES;	break;
	case MES_STATAUS_AOI_GET_CMD: NewProcessID=ITS_STATAUS_AOI_GET_CMD;	break;
	case MES_STATAUS_AOI_GET_RES: NewProcessID=ITS_STATAUS_AOI_GET_RES;	break;

	case MES_STATAUS_VRS_SET_CMD: NewProcessID=ITS_STATAUS_VRS_SET_CMD;	break;
	case MES_STATAUS_VRS_SET_RES: NewProcessID=ITS_STATAUS_VRS_SET_RES;	break;
	case MES_STATAUS_VRS_GET_CMD: NewProcessID=ITS_STATAUS_VRS_GET_CMD;	break;
	case MES_STATAUS_VRS_GET_RES: NewProcessID=ITS_STATAUS_VRS_GET_RES;	break;

	case MES_STATAUS_AOI_READY_TO_LOAD_CMD: NewProcessID=ITS_STATAUS_AOI_READY_TO_LOAD_CMD;	break;
	case MES_STATAUS_AOI_READY_TO_LOAD_RES: NewProcessID=ITS_STATAUS_AOI_READY_TO_LOAD_RES;	break;

	case MES_STATAUS_AOI_LOAd_COMPLETE_CMD: NewProcessID=ITS_STATAUS_AOI_LOAd_COMPLETE_CMD;	break;
	case MES_STATAUS_AOI_LOAd_COMPLETE_RES: NewProcessID=ITS_STATAUS_AOI_LOAd_COMPLETE_RES;	break;
		
	case MES_STATAUS_AOI_START_INSPECTION_CMD: NewProcessID=ITS_STATAUS_AOI_START_INSPECTION_CMD;	break;
	case MES_STATAUS_AOI_START_INSPECTION_RES: NewProcessID=ITS_STATAUS_AOI_START_INSPECTION_RES;	break;

	case MES_STATAUS_AOI_INSPECTION_COMPLETE_CMD: NewProcessID=ITS_STATAUS_AOI_INSPECTION_COMPLETE_CMD;	break;
	case MES_STATAUS_AOI_INSPECTION_COMPLETE_RES: NewProcessID=ITS_STATAUS_AOI_INSPECTION_COMPLETE_RES;	break;

	case MES_STATAUS_AOI_READY_TO_UNLOAD_CMD: NewProcessID=ITS_STATAUS_AOI_READY_TO_UNLOAD_CMD;	break;
	case MES_STATAUS_AOI_READY_TO_UNLOAD_RES: NewProcessID=ITS_STATAUS_AOI_READY_TO_UNLOAD_RES;	break;

	case MES_STATAUS_AOI_UNLOAd_COMPLETE_CMD: NewProcessID=ITS_STATAUS_AOI_UNLOAd_COMPLETE_CMD;	break;
	case MES_STATAUS_AOI_UNLOAd_COMPLETE_RES: NewProcessID=ITS_STATAUS_AOI_UNLOAd_COMPLETE_RES;	break;

	case MES_STATAUS_AOI_LOGIN_OUT_CMD: NewProcessID=ITS_STATAUS_AOI_LOGIN_OUT_CMD;	break;
	case MES_STATAUS_AOI_LOGIN_OUT_RES: NewProcessID=ITS_STATAUS_AOI_LOGIN_OUT_RES;	break;

	case MES_STATAUS_AOI_CHECK_BARCODE_CMD: NewProcessID=ITS_STATAUS_AOI_CHECK_BARCODE_CMD;	break;
	case MES_STATAUS_AOI_CHECK_BARCODE_RES: NewProcessID=ITS_STATAUS_AOI_CHECK_BARCODE_RES;	break;

	case MES_STATAUS_AOI_INSPECTION_STOP_CMD: NewProcessID=ITS_STATAUS_AOI_INSPECTION_STOP_CMD;	break;
	case MES_STATAUS_AOI_INSPECTION_STOP_RES: NewProcessID=ITS_STATAUS_AOI_INSPECTION_STOP_RES;	break;
	
	case MES_STATAUS_VRS_UPLOAD_SFC_CMD: NewProcessID=ITS_STATAUS_VRS_UPLOAD_SFC_CMD;	break;
	case MES_STATAUS_VRS_UPLOAD_SFC_RES: NewProcessID=ITS_STATAUS_VRS_UPLOAD_SFC_RES;	break;	

	default:
	case MES_STATAUS_NONE:
		NewProcessID = ITS_STATAUS_NONE;
		break;
	}
	return NewProcessID;
}
//-------------------------------------------------------------------------------------//
bool CMES_ITS::CreateMESProcThread()//建立MES執行執行緒
{
	return CreateITSProcThread();
}
//-------------------------------------------------------------------------------------//
bool CMES_ITS::DeleteMESProcThread()//刪除MES執行執行緒
{
	return DeleteITSProcThread();
}
//-------------------------------------------------------------------------------------//
bool CMES_ITS::GetMESConnected()//是否已經連線
{
	return GetITSLinker().GetITSConnected();
}
//-------------------------------------------------------------------------------------//
bool CMES_ITS::ConnectMESLinker()//連線到MES連結軟體
{
	return ConnectITSLinker();
}
//-------------------------------------------------------------------------------------//
bool CMES_ITS::DisconnectMESLinker()//停止連線到MES連結軟體
{
	return DisconnectITSLinker();
}
//-------------------------------------------------------------------------------------//
bool CMES_ITS::ExecMESComm_CheckMESReady()//執行MES溝通-確認MES就緒
{
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_ITS::ExecMESComm_SetSystemParam()//執行MES溝通-設定系統參數
{
	return ExecITSComm_SetSystemParam();
}
//-------------------------------------------------------------------------------------//
bool CMES_ITS::ExecMESComm_SetProjectParam(CAOIProject *ProjectPtr)//執行MES溝通-設定專案參數
{
	return ExecITSComm_SetProjectParam(ProjectPtr);
}
//-------------------------------------------------------------------------------------//
bool CMES_ITS::ExecMESComm_SetProjectLoadFinished(CAOIProject *ProjectPtr, bool bSucc, LPCTSTR Err)//執行MES溝通-專案載入完畢
{
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_ITS::ExecMESComm_SetMachineStatus()//執行MES溝通-設定機台狀態
{
	return ExecITSComm_SetMachineStatus();
}
//-------------------------------------------------------------------------------------//
bool CMES_ITS::ExecMESComm_SetMachineStatus(ONLINE_STATE_MODE Status)//執行MES溝通-設定機台狀態		
{
	return ExecITSComm_SetMachineStatus(Status);
}
//-------------------------------------------------------------------------------------//
bool CMES_ITS::ExecMESComm_ProcessID(int ProcessID)//執行MES溝通-程序運作
{
	const int ITS_ProcessID=MapITSComm_ProcessID(ProcessID);
	return ExecITSComm_ProcessID(ITS_ProcessID);
}
//-------------------------------------------------------------------------------------//
bool CMES_ITS::ExecMESComm_SetAOIExceptionCode(LPCTSTR ErrStr)//執行MES溝通-系統異常碼
{
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_ITS::ExecMESComm_SetUserLogin_out(bool bLogin)//執行MES溝通-設定使用者登入	
{
	return ExecITSComm_SetUserLogin_out(bLogin);
}
//-------------------------------------------------------------------------------------//
bool CMES_ITS::ExecMESComm_CheckBarcode(CAOIProject *ProjectPtr)//執行MES溝通-確認條碼
{
	return ExecITSComm_CheckBarcode(ProjectPtr);
}
//-------------------------------------------------------------------------------------//
size_t CMES_ITS::GetMESRecvNodeCount()	
{
	return GetITSRecvNodeCount();
}
//-------------------------------------------------------------------------------------//
bool CMES_ITS::ProcesMESRecvNodeList(bool bThread)//處理收到MES的訊息列表
{
	return ProcesITSRecvNodeList(bThread);
}
//-------------------------------------------------------------------------------------//
bool CMES_ITS::SendToMESNode(const char *strBuf, bool bAck, int nPPID, DWORD AckTime)//送資料給MES
{
	return SendToITSNode(strBuf, bAck, nPPID, AckTime);
}
//-------------------------------------------------------------------------------------//
#endif//ITS_DISABLE
#endif//MES_DISABLE