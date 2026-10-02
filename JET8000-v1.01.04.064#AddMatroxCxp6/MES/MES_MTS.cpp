// MES_MTS.cpp: implementation of the CMES_MTS class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "MES_MTS.h"
#include "resource.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
#ifndef MES_DISABLE
#ifndef MTS_DISABLE
CMES_MTS MES_MTS;
//-------------------------------------------------------------------------------------//
//MTS執行
unsigned int MTSProcThreadID = 0;//MTS執行執行緒編號
HANDLE MTSProcThreadHandle = NULL;//MTS執行執行緒處理碼
HANDLE MTSProcThreadEvent  = NULL;//MTS執行執行緒事件
unsigned int __stdcall MTSProcThreadFn(void *pParam);
//-------------------------------------------------------------------------------------//
unsigned int __stdcall MTSProcThreadFn(void *pParam)
{
#ifndef MTS_DISABLE
	bool IsOK = true;	
	DWORD  SleepTime = 10;//60 sec exec
	//const size_t ThreadIdx = (size_t)pParam;
	const size_t ThreadIdx = 0;
	CMES_MTS *Ptr=(CMES_MTS*)(pParam);
	THREAD_STATE_MODE   ThreadState = THREAD_STATE_NONE;
	THREAD_COMMAND_MODE ThreadCmd = THREAD_COMMAND_TO_NONE;		
	
	double Time = 0;
	LARGE_INTEGER          nStartTime;
	LARGE_INTEGER          nEndTime;
	TCHAR strBuffer[256]=_T("");

	if ( NULL == Ptr ) { return -1; }
	while ( true )
	{	
		ThreadCmd = MES_MTS.GetMTSProcThreadCmd();
		ThreadState = MES_MTS.GetMTSProcThreadState();
		if ( THREAD_COMMAND_TO_EXIT == ThreadCmd )
		{	break;	}
		if ( THREAD_COMMAND_TO_IDLE == ThreadCmd ) 
		{ 			
			MES_MTS.SetMTSProcThreadState(THREAD_STATE_IDLE);		
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
			MES_MTS.SetMTSProcThreadState(THREAD_STATE_RUNNING);		
			QueryPerformanceCounter(&nStartTime);
			IsOK = MES_MTS.ExecMTSProcFn();
			QueryPerformanceCounter(&nEndTime);			
			Time = (nEndTime.QuadPart - nStartTime.QuadPart) * 1000.0/AOIDataCollect.m_SystemFreq.QuadPart;
			const TSystemParameter &SystemParam = AOIDataCollect.GetSystemParameter();
			if ( FN_ENABLE == SystemParam.m_SaveITSProcThreadLog ) 
			{
				_stprintf(strBuffer, _T("MTSProcThreadFn[%d]: %.2f ms"), ThreadIdx+1, Time);
				AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, strBuffer);			
			}
		}
		::Sleep(SleepTime);
	};
	MES_MTS.SetMTSProcThreadState(THREAD_STATE_NONE);
	if ( NULL != MTSProcThreadEvent )
	{	::SetEvent(MTSProcThreadEvent); }
#endif//MTS_DISABLE
	return 0;
}
//-------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
CMES_MTS::CMES_MTS()
{	
	PreInitMTS();
	InitialMTS();
}
//-------------------------------------------------------------------------------------//
CMES_MTS::~CMES_MTS()
{
	DeleteMTSProcThread();
}
//-------------------------------------------------------------------------------------//
void CMES_MTS::PreInitMTS()
{	
	m_MsgPPID = 0;		
	m_MTSProcThreadState = THREAD_STATE_NONE;
	m_MTSProcThreadCmd = THREAD_COMMAND_TO_NONE;	
}
//-------------------------------------------------------------------------------------//
void CMES_MTS::InitialMTS()
{

}
//-------------------------------------------------------------------------------------//
void CMES_MTS::CloneMTS(const CMES_MTS &other)
{

}
//-------------------------------------------------------------------------------------//
void CMES_MTS::LockMTSProc()//鎖住MTS執行緒同步化
{
	CMES_Imp::LockImpProc();	
}
//-------------------------------------------------------------------------------------//
void CMES_MTS::UnlockMTSProc()//釋放MTS執行緒同步化
{
	CMES_Imp::UnlockImpProc();	
}
//-------------------------------------------------------------------------------------//
bool CMES_MTS::ExecMTSProcFn()                      //執行MTS執行執行緒
{
#ifndef MTS_DISABLE
	const bool bThread = true;
	THREAD_COMMAND_MODE    ThreadCmd;	
	while ( true )
	{
		ThreadCmd = GetMTSProcThreadCmd();
		if ( AOIDataCollect.GetIsSystemReleased() == true ) { return true; }
		if ( THREAD_COMMAND_TO_IDLE == ThreadCmd ) { return true; }
		if ( THREAD_COMMAND_TO_EXIT == ThreadCmd ) { return true; }		
		if ( THREAD_COMMAND_TO_NONE == ThreadCmd ) { return true; }			

		if ( RecvNewMTSRecvNodeList() == false )
		{	
			::Sleep(10);
			continue; 
		}
		if ( ProcesMTSRecvNodeList(bThread) == false ) 
		{	
			::Sleep(10);
			continue; 
		}
		::Sleep(10);
	}
#endif//MTS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_MTS::CreateMTSProcThread()                //建立MTS執行執行緒
{
#ifndef MTS_DISABLE
	if ( DeleteMTSProcThread() == false ) { return false; }	
	m_MTSProcThreadCmd = THREAD_COMMAND_TO_IDLE;	

	MTSProcThreadHandle = (HANDLE)::_beginthreadex(NULL, NULL, &MTSProcThreadFn, (void*)this, NULL, &MTSProcThreadID);	
	if ( NULL == MTSProcThreadHandle )
	{	
		m_MTSProcThreadState = THREAD_STATE_NONE;
		m_ErrorString = _T("Eorror, Create Thread Fault (MTSProcThreadHandle == NULL)");		
		AOIExceptionCodeCtrl.SetAOIExceptionCode_Thread(AOI_EXCEPTION_THREAD_CREATE, m_ErrorString);
		return false;
	}		
	MTSProcThreadEvent = JetAPI::CreateEvent(NULL, TRUE, TRUE, _T("MTS Proc Event"));		
	::SetThreadPriority(MTSProcThreadHandle, THREAD_PRIORITY_BELOW_NORMAL); 
	//::SetThreadAffinityMask(MTSProcThreadHandle, 0x03);//保留2個		
	StartMTSProcThread(false);
#else
	MTSProcThreadHandle = NULL;
	MTSProcThreadEvent = NULL;
#endif//MTS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_MTS::DeleteMTSProcThread()                //刪除MTS執行執行緒
{
#ifndef MTS_DISABLE
	DWORD WaitTime = 10000;//1 sec	
	if ( NULL == MTSProcThreadHandle ) { return true; }		
	SetMTSProcThreadCmd(THREAD_COMMAND_TO_EXIT);	
	DWORD Res = ::WaitForSingleObject(MTSProcThreadHandle, WaitTime);
	::CloseHandle(MTSProcThreadHandle); 
	MTSProcThreadHandle = NULL;
	if ( NULL != MTSProcThreadEvent )
	{	
		::CloseHandle(MTSProcThreadEvent); 
		MTSProcThreadEvent=NULL; 
	}	
#endif//MTS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_MTS::StopMTSProcThread(bool WaitOn)      //開始MTS執行執行緒
{
#ifndef MTS_DISABLE
	size_t i=0;
	if ( NULL == MTSProcThreadHandle ) { return true; }
	if ( NULL != MTSProcThreadEvent )
	{	::ResetEvent(MTSProcThreadEvent); }
	SetMTSProcThreadCmd(THREAD_COMMAND_TO_IDLE);	

	if ( WaitOn == true ) 
	{	
		const size_t MaxCount = 100;
		const size_t SleepTime = 10;
		for ( i=0; i<MaxCount; i++ )
		{	
			if ( THREAD_STATE_NONE == m_MTSProcThreadState )
			{	break; }
			if ( THREAD_STATE_IDLE == m_MTSProcThreadState )
			{	break; }
			//if ( THREAD_STATE_FINISH == m_ITSProcThreadState )//有可能後面才完成, 所以要加上完成確認
			//{	break; }
			::Sleep(SleepTime);
		}
		if ( i == MaxCount )
		{
			m_ErrorString.Format(_T("Error, wait for StopMTSProcThread too long"));			
			return false;
		}		
	}
#endif//MTS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_MTS::StartMTSProcThread(bool WaitOn)      //開始MTS執行執行緒	
{
#ifndef MTS_DISABLE
	size_t i=0;
	if ( NULL == MTSProcThreadHandle ) { return true; }
	if ( NULL != MTSProcThreadEvent )
	{	::ResetEvent(MTSProcThreadEvent); }
	if ( THREAD_STATE_FINISH == m_MTSProcThreadState )
	{  SetMTSProcThreadState(THREAD_STATE_IDLE); }
	SetMTSProcThreadCmd(THREAD_COMMAND_TO_RUN);	

	if ( false == WaitOn )
	{	return true; }
	if ( WaitForMTSProcThreadStart() == false )
	{	return false; }	
#endif//MTS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_MTS::WaitForMTSProcThreadIdle()           //等待MTS執行執行緒閒置
{
#ifndef MTS_DISABLE
	size_t i=0;	
	DWORD Res = 0;		
	const DWORD SleepTime = 100;	
	const size_t MaxCounts = 200000;
	for ( i=0; i<MaxCounts; i++ )
	{
		if ( AOIDataCollect.GetIsSystemReleased() == true ) { return true; }				
		//if ( AOIDataCollect.GetIsSystemException() == true ) { return true; }
		if ( THREAD_STATE_NONE == m_MTSProcThreadState ) { return true; }
		if ( THREAD_STATE_IDLE == m_MTSProcThreadState ) { return true; }
		if ( SleepTime > 0 )
		{	::Sleep(SleepTime); }
	}
	if ( i == MaxCounts )
	{
		m_ErrorString.Format(_T("Error, Wait for WaitForMTSProcThreadIdle too long"));
		AOIExceptionCodeCtrl.SetAOIExceptionCode_Thread(AOI_EXCEPTION_THREAD_WAIT_FOR_IDLE, m_ErrorString);
		return false;
	}	
#endif//MTS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_MTS::WaitForMTSProcThreadStop()           //等待MTS執行執行緒停止
{
#ifndef MTS_DISABLE
	size_t i=0;	
	DWORD Res = 0;		
	const DWORD SleepTime = 100;	
	const size_t MaxCounts = 200000;
	for ( i=0; i<MaxCounts; i++ )
	{
		if ( AOIDataCollect.GetIsSystemReleased() == true ) { return true; }				
		//if ( AOIDataCollect.GetIsSystemException() == true ) { return true; }
		if ( THREAD_STATE_RUNNING != m_MTSProcThreadState ) { return true; }		
		if ( SleepTime > 0 )
		{	::Sleep(SleepTime); }
	}
	if ( i == MaxCounts )
	{
		m_ErrorString.Format(_T("Error, Wait for WaitForMTSProcThreadStop too long"));
		AOIExceptionCodeCtrl.SetAOIExceptionCode_Thread(AOI_EXCEPTION_THREAD_WAIT_FOR_STOP, m_ErrorString);
		return false;
	}	
#endif//MTS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_MTS::WaitForMTSProcThreadStart()          //等待MTS執行執行緒開始
{
#ifndef MTS_DISABLE		
	size_t i=0;
	const size_t MaxCount = 100;
	const size_t SleepTime = 10;
	for ( i=0; i<MaxCount; i++ )
	{
		if ( THREAD_COMMAND_TO_RUN != m_MTSProcThreadCmd )//切入下一個階段
		{	break; }
		if ( THREAD_STATE_IDLE != m_MTSProcThreadState )
		{	break; }
		::Sleep(SleepTime);
	}
	if ( i == MaxCount )
	{
		m_ErrorString.Format(_T("Error, wait for StartMTSProcThread too long"));			
		AOIExceptionCodeCtrl.SetAOIExceptionCode_Thread(AOI_EXCEPTION_THREAD_WAIT_FOR_START, m_ErrorString);
		return false;
	}
#endif//MTS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_MTS::WaitForMTSProcThreadFinish()         //等待MTS執行執行緒結束
{
#ifndef MTS_DISABLE
	size_t i=0;	
	DWORD Res = 0;		
	const DWORD SleepTime = 100;	
	const size_t MaxCounts = 200000;

	if ( NULL == MTSProcThreadEvent ) { return true; }
	for ( i=0; i<MaxCounts; i++ )
	{
		if ( AOIDataCollect.GetIsSystemReleased() == true ) { return true; }				
		if ( AOIDataCollect.GetIsSystemException() == true ) { return true; }
		if ( THREAD_COMMAND_TO_EXIT == m_MTSProcThreadCmd ) { return true; }		
		if ( THREAD_COMMAND_TO_IDLE == m_MTSProcThreadCmd )
		{	break; }

		Res = ::WaitForSingleObject(MTSProcThreadEvent, SleepTime);
		if ( Res != WAIT_TIMEOUT ) 
		{
			if ( THREAD_STATE_RUNNING == m_MTSProcThreadState )
			{
				if ( SleepTime > 0 ) { ::Sleep(SleepTime); }
				continue; 
			}
			break; 
		}
	}
	if ( i == MaxCounts )
	{
		m_ErrorString.Format(_T("Error, Wait for WaitForMTSProcThreadFinish too long"));
		AOIExceptionCodeCtrl.SetAOIExceptionCode_Thread(AOI_EXCEPTION_THREAD_WAIT_FOR_FINISH, m_ErrorString);
		return false;
	}	
#endif//MTS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_MTS::CheckMTSProcThreadState(THREAD_STATE_MODE State)//確認MTS執行執行緒狀態
{
	if ( NULL == MTSProcThreadHandle ) { return true; }
	if ( m_MTSProcThreadState != State )
	{
		m_ErrorString.Format(_T("Error, m_MTSProcThreadState is Exception"));
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CMES_MTS::SetMTSProcThreadState(THREAD_STATE_MODE State)//設定MTS執行執行緒狀態
{
	if ( m_MTSProcThreadState == State ) { return; }
	LockMTSProc();
	m_MTSProcThreadState = State;
	UnlockMTSProc();
}
//-------------------------------------------------------------------------------------//
THREAD_STATE_MODE  CMES_MTS::GetMTSProcThreadState()              //取得MTS執行執行緒狀態
{
	return m_MTSProcThreadState;
}
//-------------------------------------------------------------------------------------//
void CMES_MTS::SetMTSProcThreadCmd(THREAD_COMMAND_MODE Cmd) //設定MTS執行執行緒命令
{
	if ( m_MTSProcThreadCmd == Cmd ) { return; }
	LockMTSProc();
	m_MTSProcThreadCmd = Cmd;
	UnlockMTSProc();
}
//-------------------------------------------------------------------------------------//
THREAD_COMMAND_MODE CMES_MTS::GetMTSProcThreadCmd()	               //取得MTS執行執行緒命令
{
	return m_MTSProcThreadCmd;
}
//-------------------------------------------------------------------------------------//
bool CMES_MTS::ConnectMTSLinker()//連線到MTS連結軟體
{
#ifndef MTS_DISABLE
	CString MTSFolder;
	CString MTSFilename;
	CString Folder = AOIDataCollect.GetAOIDirectory();
	const TSystemParameter &SysParam = AOIDataCollect.GetSystemParameter();

	MTSFolder.Format(_T("%s\\%s"), Folder, _T("MES"));
	if ( JetAPI::CreateFolder(MTSFolder) == false )
	{
		m_ErrorString.Format(_T("Error, Create Foler Fault\n(%s)"), MTSFolder);
		return false;
	}
	m_MTSShareFolder = MTSFolder;
#endif//MTS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_MTS::DisconnectMTSLinker()//停止連線到MTS連結軟體
{
#ifndef MTS_DISABLE	
#endif//MTS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
int CMES_MTS::GetMTSStatusID_Ack(int StatusID)//取得MTS的回應訊息
{
	int AckID=MTS_STATAUS_NONE;
	switch ( StatusID )
	{
	case MTS_STATAUS_MES_SET_CMD:	
		AckID = MTS_STATAUS_MES_SET_RES;
		break;

	case MTS_STATAUS_MES_GET_CMD:	
		AckID = MTS_STATAUS_MES_GET_RES;
		break;

	case MTS_STATAUS_AOI_SET_CMD:	
		AckID = MTS_STATAUS_AOI_SET_RES;
		break;

	case MTS_STATAUS_AOI_GET_CMD:	
		AckID = MTS_STATAUS_AOI_GET_RES;
		break;
	
	case MTS_STATAUS_VRS_SET_CMD:
		AckID = MTS_STATAUS_VRS_SET_RES;
		break;
	case MTS_STATAUS_VRS_GET_CMD:
		AckID = MTS_STATAUS_VRS_GET_RES;
		break;

	case MTS_STATAUS_AOI_READY_TO_LOAD_CMD:	
		AckID = MTS_STATAUS_AOI_READY_TO_LOAD_RES;
		break;

	case MTS_STATAUS_AOI_LOAd_COMPLETE_CMD:
		AckID = MTS_STATAUS_AOI_LOAd_COMPLETE_RES;
		break;

	case MTS_STATAUS_AOI_START_INSPECTION_CMD:	
		AckID = MTS_STATAUS_AOI_START_INSPECTION_RES;
		break;

	case MTS_STATAUS_AOI_INSPECTION_COMPLETE_CMD:	
		AckID = MTS_STATAUS_AOI_INSPECTION_COMPLETE_RES;
		break;

	case MTS_STATAUS_AOI_READY_TO_UNLOAD_CMD:
		AckID = MTS_STATAUS_AOI_READY_TO_UNLOAD_RES;
		break;

	case MTS_STATAUS_AOI_UNLOAd_COMPLETE_CMD:	
		AckID = MTS_STATAUS_AOI_UNLOAd_COMPLETE_RES;
		break;

	case MTS_STATAUS_AOI_LOGIN_OUT_CMD:	
		AckID = MTS_STATAUS_AOI_LOGIN_OUT_RES;
		break;
	case MTS_STATAUS_AOI_CHECK_BARCODE_CMD:
		AckID = MTS_STATAUS_AOI_CHECK_BARCODE_RES;
		break;
	case MTS_STATAUS_AOI_INSPECTION_STOP_CMD:
		AckID = MTS_STATAUS_AOI_INSPECTION_STOP_RES;
		break;

	case MTS_STATAUS_VRS_UPLOAD_SFC_CMD:	
		AckID = MTS_STATAUS_VRS_UPLOAD_SFC_RES;
		break;	
	}
	return AckID;
}
//-------------------------------------------------------------------------------------//
bool CMES_MTS::CheckMTSStatusID_Ack(int StatusID)//確認MTS的回應訊息
{
	bool bAckID = false;
	switch ( StatusID )
	{
	case MTS_STATAUS_MES_SET_RES:
	case MTS_STATAUS_MES_GET_RES:
	case MTS_STATAUS_AOI_SET_RES:
	case MTS_STATAUS_AOI_GET_RES:
	case MTS_STATAUS_VRS_SET_RES:
	case MTS_STATAUS_VRS_GET_RES:
	case MTS_STATAUS_AOI_READY_TO_LOAD_RES:
	case MTS_STATAUS_AOI_LOAd_COMPLETE_RES:
	case MTS_STATAUS_AOI_START_INSPECTION_RES:
	case MTS_STATAUS_AOI_INSPECTION_COMPLETE_RES:
	case MTS_STATAUS_AOI_READY_TO_UNLOAD_RES:
	case MTS_STATAUS_AOI_UNLOAd_COMPLETE_RES:
	case MTS_STATAUS_AOI_LOGIN_OUT_RES:
	case MTS_STATAUS_AOI_CHECK_BARCODE_RES:
	case MTS_STATAUS_VRS_UPLOAD_SFC_RES:
		bAckID = true;
		break;
	default:
		bAckID = false;
		break;
	}
	return bAckID;
}
//-------------------------------------------------------------------------------------//
size_t CMES_MTS::GetMTSRecvNodeCount()
{
	return m_MTSRecvNodeList.size();
}
//-------------------------------------------------------------------------------------//
bool CMES_MTS::RecvNewMTSRecvNodeList()//接收新的MTS訊息列表
{
#ifndef MTS_DISABLE
	size_t i=0;
	std::vector<std::string> RecvNodeList;	

	LockMTSProc();
	//CITSLinker &ITSLinker=GetITSLinker();
	//ITSLinker.CloneITSRecvMsgList(RecvNodeList, true);
	const size_t RecvNodeCount = RecvNodeList.size();	
	for ( i=0; i<RecvNodeCount; i++ )
	{	
		if ( AddMTSRecvNode(RecvNodeList[i]) == false ) 
		{
			UnlockMTSProc();
			m_ErrorString = _T("Error, Receive MTS Node Fault");
			return false;
		}				
	}			
	UnlockMTSProc();
#endif//MTS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_MTS::AddMTSRecvNode(const std::string &sBuff)
{
#ifndef MTS_DISABLE
	TITSCommNode CommNode;
	if ( DecoderMTSPacket(sBuff, CommNode) == false )
	{	return false; }
	m_MTSRecvNodeList.push_back(CommNode);	
#endif//MTS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_MTS::DecoderMTSPacket(const std::string &sBuff, TITSCommNode &sNode)
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
bool CMES_MTS::GetMTSRecvNode(size_t index, bool bCheck, TITSCommNode &sNode)
{
	if ( true == bCheck )
	{
		const size_t Count = m_MTSRecvNodeList.size();
		if ( index >= Count ) 
		{	return false; }
	}
	sNode = m_MTSRecvNodeList[index];
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CMES_MTS::ProcesMTSRecvNodeList(bool bThread)//處理收到MTS的訊息列表
{
#ifndef MTS_DISABLE
	size_t       i=0;
	bool         bBreak=false;
	bool         bRemove = false;
	std::vector<size_t> EraseIndexList;	

	bBreak=false;
	LockMTSProc();
	const size_t RecvCount = m_MTSRecvNodeList.size();
	for ( i=0; i<RecvCount; i++ )
	{
		if ( ProcesMTSRecvNode(bThread, m_MTSRecvNodeList[i], bBreak, bRemove) == false )
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
		m_MTSRecvNodeList.erase(m_MTSRecvNodeList.begin()+EraseIndex);
	}
	UnlockMTSProc();
#endif//MTS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_MTS::ProcesMTSRecvNode(bool bThread, TITSCommNode &sNode, bool &bBreak, bool &bRemove)//處理收到MTS的訊息
{
#ifndef MTS_DISABLE
	sNode.nChkCount ++;	
	//確認是否需要回傳
	if ( 0 == sNode.nAck ) { return true; }
	
	const int StatusRes = GetMTSStatusID_Ack(sNode.nStatus);	
	if ( MTS_STATAUS_NONE == StatusRes ) 
	{	return true; }	
	
	switch ( sNode.nStatus )
	{
	case MTS_STATAUS_MES_SET_CMD:
		if ( ExecMTSComm_MesSetParam(bThread, sNode, bBreak, bRemove) == false ) 
		{	break; }
		break;
	case MTS_STATAUS_MES_GET_CMD:
		break;
	}
	if ( true == bBreak ) 
	{	return true; }

#endif//MTS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_MTS::ExecMTSComm_MesSetParam(bool bThread, TITSCommNode &sNode, bool &bBreak, bool &bRemove)	
{
#ifndef MTS_DISABLE
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
			const int nStatusRet = GetMTSStatusID_Ack(nStatus);
			if ( MTS_STATAUS_NONE != nStatusRet ) 
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
				SendToMTSNode(sBufSend.c_str(), false, sNode.nPPID, 0);
			}
		}
		if ( true==bBreak )
		{	return true; }		
	}	
#endif//MTS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_MTS::ExecMTSComm_SetSystemParam()//執行MTS溝通-設定系統參數
{
#ifndef MTS_DISABLE
	int  nPPID = 0;
	bool bAck = true;
	int  AckTime = 1000;	
	std::wstring wsBuf;
	if ( BuildMTSDoc_SetSystemParam(bAck, AckTime, nPPID, wsBuf) == false )
	{	return false; }

	std::string sBuf;
	UINT ConvertCode = CP_UTF8;
	if ( JetAPI::wchar2string(wsBuf.c_str(), sBuf, ConvertCode) == false ) 
	{
		m_ErrorString.Format(_T("Error, Conver wstring to string fault [s]"), _T("SetSystemParam"));
		return false;
	}
	
	if ( SendToMTSNode(sBuf.c_str(), bAck, nPPID, AckTime) == false ) 
	{	return false; }
#endif//MTS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_MTS::ExecMTSComm_SetProjectParam()//執行MTS溝通-設定專案參數
{
#ifndef MTS_DISABLE
	int  nPPID = 0;
	bool bAck = true;
	int  AckTime = 1000;	
	std::wstring wsBuf;
	if ( BuildMTSDoc_SetProjectParam(bAck, AckTime, nPPID, wsBuf) == false )
	{	return false; }

	std::string sBuf;
	UINT ConvertCode = CP_UTF8;
	if ( JetAPI::wchar2string(wsBuf.c_str(), sBuf, ConvertCode) == false ) 
	{
		m_ErrorString.Format(_T("Error, Conver wstring to string fault [s]"), _T("SetProjectParam"));
		return false;
	}
	
	if ( SendToMTSNode(sBuf.c_str(), bAck, nPPID, AckTime) == false ) 
	{	return false; }
#endif//MTS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_MTS::ExecMTSComm_SetMachineStatus()//執行MTS溝通-設定機台狀態
{
	ONLINE_STATE_MODE eStatus = AOIDataCollect.GetOnlineStateMode();	
	return ExecMTSComm_SetMachineStatus(eStatus);
}
//-------------------------------------------------------------------------------------//
bool CMES_MTS::ExecMTSComm_SetMachineStatus(ONLINE_STATE_MODE Status)//執行MTS溝通-設定機台狀態	
{
#ifndef MTS_DISABLE
	int  nPPID = 0;
	bool bAck = false;
	int  AckTime = 1000;	
	std::wstring wsBuf;	
	if ( BuildMTSDoc_SetMachineStatus(Status, bAck, AckTime, nPPID, wsBuf) == false )
	{	return false; }

	std::string sBuf;
	UINT ConvertCode = CP_UTF8;
	if ( JetAPI::wchar2string(wsBuf.c_str(), sBuf, ConvertCode) == false ) 
	{
		m_ErrorString.Format(_T("Error, Conver wstring to string fault [s]"), _T("SetMachineStatus"));
		return false;
	}
	
	if ( SendToMTSNode(sBuf.c_str(), bAck, nPPID, AckTime) == false ) 
	{	return false; }
#endif//MTS_DISABLE
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CMES_MTS::ExecMTSComm_ProcessID(int ProcessID)//執行MTS溝通-程序運作
{
#ifndef MTS_DISABLE
	if ( MTS_STATAUS_NONE == ProcessID )
	{	return true; }

	int  nPPID = 0;
	bool bAck = true;
	int  AckTime = 1000;	
	std::wstring wsBuf;
	if ( BuildMTSDoc_ProcessID(ProcessID, bAck, AckTime, nPPID, wsBuf) == false )
	{	return false; }

	std::string sBuf;
	UINT ConvertCode = CP_UTF8;
	if ( JetAPI::wchar2string(wsBuf.c_str(), sBuf, ConvertCode) == false ) 
	{
		m_ErrorString.Format(_T("Error, Conver wstring to string fault [ProcID:%d]"), ProcessID);
		return false;
	}
	
	if ( SendToMTSNode(sBuf.c_str(), bAck, nPPID, AckTime) == false ) 
	{	return false; }
#endif//MTS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
int CMES_MTS::GetMTSFreePPID()//取得MTS可用的PPID
{
	int uPPID = m_MsgPPID;
	m_MsgPPID ++;
	if ( m_MsgPPID > 0x0000FFFF ) 
	{	m_MsgPPID = 1; }
	return uPPID;
}
//-------------------------------------------------------------------------------------//
bool CMES_MTS::BuildMTSDoc(int nStatus, bool bAck, int AckTime, int &nPPID, rapidjson::WDocument &Doc)//建立MTS文檔
{
	bool         bRet=false;	
	std::wstring wstrBuffer;
	CString      strStatus;	
	rapidjson::CJsonCtrl JSonCtrl;


	strStatus = GetMTSStatusText(nStatus);
	JetAPI::TCHAR2wstring(strStatus, wstrBuffer);

	nPPID = GetMTSFreePPID();	

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
bool CMES_MTS::BuildMTSDoc_SetSystemParam(bool bAck, int AckTime, int &nPPID, std::wstring& wsBuf)//建立系統資訊文檔
{
	bool bRet=true;	
	TSystemParameter &SysParam = AOIDataCollect.GetSystemParameter();	

	CString    str;
	std::wstring wsBuffer;
	rapidjson::WDocument Doc;
	rapidjson::CJsonCtrl JSonCtrl;
	const UINT ConvertCode = CP_UTF8;	
	CAMERA_ID CameraID =  PRIMARY_CAMERA_ID;
	double ResolutionX = AOIDataCollect.GetCameraResolutionX(CameraID);
	double ResolutionY = AOIDataCollect.GetCameraResolutionY(CameraID);
	
	nPPID = 0;	
	//initial
	Doc.SetObject();
	JSonCtrl.Set(&Doc);
	
	if ( BuildMTSDoc(MTS_STATAUS_AOI_SET_CMD, bAck, AckTime, nPPID, Doc) == false )
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

	str = SysParam.m_MachineMES_CodeName;//MES-TSP//Pegatron
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
bool CMES_MTS::BuildMTSDoc_SetProjectParam(bool bAck, int AckTime, int &nPPID, std::wstring& wsBuf)//建立基本資訊文檔
{
	bool bRet=true;
	CAOIProject  *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) 
	{	return false; }		
	TProjectParameter &ProgParam = ProjectPtr->GetProjectParameter();

	CString    str;
	std::wstring wsBuffer;
	rapidjson::WDocument Doc;
	rapidjson::CJsonCtrl JSonCtrl;
	const UINT ConvertCode = CP_UTF8;
	LANE_ID LaneID = GetActiveLaneID();
	CAMERA_ID CameraID =  PRIMARY_CAMERA_ID;
	double ResolutionX = AOIDataCollect.GetCameraResolutionX(CameraID);
	double ResolutionY = AOIDataCollect.GetCameraResolutionY(CameraID);
	
	nPPID = 0;	
	//initial
	Doc.SetObject();
	JSonCtrl.Set(&Doc);
	
	if ( BuildMTSDoc(MTS_STATAUS_AOI_SET_CMD, bAck, AckTime, nPPID, Doc) == false )
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
bool CMES_MTS::BuildMTSDoc_SetMachineStatus(ONLINE_STATE_MODE Status, bool bAck, int AckTime, int &nPPID, std::wstring& wsBuf)//建立機台運作文檔	
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
	
	if ( BuildMTSDoc(MTS_STATAUS_AOI_SET_CMD, bAck, AckTime, nPPID, Doc) == false )
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
bool CMES_MTS::BuildMTSDoc_ProcessID(int ProcessID, bool bAck, int AckTime, int &nPPID, std::wstring& wsBuf)//建立程序運作文檔
{
	bool bRet=false;			
	CString    str;	
	std::wstring wsBuffer;
	rapidjson::WDocument Doc;
	rapidjson::CJsonCtrl JSonCtrl;
	const UINT ConvertCode = CP_UTF8;
	CAOIProject *ProjectPtr = GetActiveProject();

	nPPID=0;	
	//initial
	Doc.SetObject();
	JSonCtrl.Set(&Doc);
	
	if ( BuildMTSDoc(ProcessID, bAck, AckTime, nPPID, Doc) == false )
	{	return false; }

	if ( MTS_STATAUS_AOI_INSPECTION_COMPLETE_CMD == ProcessID )
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
bool CMES_MTS::SendToMTSNode(const char *strBuf, bool bAck, int nPPID, DWORD AckTime)//送資料給MTS
{
#ifndef MTS_DISABLE
	/*
	CITSLinker &ITSLinker=GetITSLinker();
	if ( ITSLinker.GetITSConnected() == false )
	{
		m_ErrorString = _T("Error, ITS Client Not connected");
		return false;
	}
	*/

	m_MTSSendBuffer = strBuf;
	/*
	if ( ITSLinker.AddITSSendMsg(m_MTSSendBuffer) == false )
	{
		m_ErrorString = _T("Error, MTS Client Send Bytes Fault");
		return false;
	}
	*/

	if ( true == bAck  ) 
	{		
		if ( StopMTSProcThread(true) == false )
		{	return false; }

		CString ErrorString;
		bool bIsOK = WaitForMTSResponse(nPPID, AckTime);
		if ( bIsOK == false ) 
		{	ErrorString = GetErrorString(); }
		StartMTSProcThread(false);
		if ( bIsOK == false ) 
		{	
			SetErrorString(ErrorString); 
			return false;
		}
	}
#endif//MTS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_MTS::WaitForMTSResponse(int nPPID, DWORD AckTime)//等待MTS回傳資料
{
#ifndef MTS_DISABLE
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
	DWORD TimeoutMS=GetMesCommTimeoutMS();
	if ( 0 != TimeoutMS )
	{
		MaxChkCount = MAX(TimeoutMS, AckTime+1000);
		MaxChkCount /= SleepTime;		
	}	
	while ( true ) 
	{	
		LastRecvCount = m_MTSRecvNodeList.size();
		if ( RecvNewMTSRecvNodeList() == false ) 
		{	return false; }

		LockMTSProc();
		CurRecvNodeCount = m_MTSRecvNodeList.size();
		for ( i=LastRecvCount; i<CurRecvNodeCount; i++ )
		{
			TITSCommNode &sNode = m_MTSRecvNodeList[i];				

			if ( sNode.nPPID == nPPID )
			{
				bFound = true;
				int nErrorCode = sNode.nErrorCode;
				bool bAckID = CheckMTSStatusID_Ack(sNode.nStatus);
				CString strErrorCode = sNode.wsErrorCode.c_str();
				m_MTSRecvNodeList.erase(m_MTSRecvNodeList.begin()+i);
				if ( 0 != nErrorCode )
				{
					UnlockMTSProc();
					m_ErrorString.Format(_T("Error, ITS %s"), strErrorCode);
					return false;
				}
				break;
			}
		}	
		UnlockMTSProc();

		if ( true == bFound )
		{	break; }

		ChkCount ++;
		if ( ChkCount >= MaxChkCount ) 
		{	break; }

		::Sleep(SleepTime);
	};

	if ( ChkCount == MaxChkCount )
	{
		m_ErrorString.Format(_T("Error, wait for MTS response too long [PPID%d]"), nPPID);
		return false;
	}
#endif//MTS_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
CString CMES_MTS::GetMTSStatusText(int StatusID)
{
	CString str;
	switch ( StatusID )
	{
	case MTS_STATAUS_NONE: str = _T("None");	break;

	case MTS_STATAUS_MES_SET_CMD: str = _T("MES Set Parameters");	break;
	case MTS_STATAUS_MES_SET_RES: str = _T("MES Set Parameters Response");	break;
	case MTS_STATAUS_MES_GET_CMD: str = _T("MES Get Parameters");	break;
	case MTS_STATAUS_MES_GET_RES: str = _T("MES Get Parameters Response");	break;

	case MTS_STATAUS_AOI_SET_CMD: str = _T("AOI Set Parameters");	break;
	case MTS_STATAUS_AOI_SET_RES: str = _T("AOI Set Parameters Response");	break;
	case MTS_STATAUS_AOI_GET_CMD: str = _T("AOI Get Parameters");	break;
	case MTS_STATAUS_AOI_GET_RES: str = _T("AOI Get Parameters Response");	break;

	case MTS_STATAUS_VRS_SET_CMD: str = _T("VRS Set Parameters");	break;
	case MTS_STATAUS_VRS_SET_RES: str = _T("VRS Set Parameters Response");	break;
	case MTS_STATAUS_VRS_GET_CMD: str = _T("VRS Get Parameters");	break;
	case MTS_STATAUS_VRS_GET_RES: str = _T("VRS Get Parameters Response");	break;

	case MTS_STATAUS_AOI_READY_TO_LOAD_CMD: str = _T("AOI Ready To Load");	break;
	case MTS_STATAUS_AOI_READY_TO_LOAD_RES: str = _T("AOI Ready To Load Response");	break;

	case MTS_STATAUS_AOI_LOAd_COMPLETE_CMD: str = _T("AOI Load Complete");	break;
	case MTS_STATAUS_AOI_LOAd_COMPLETE_RES: str = _T("AOI Load Complete Response");	break;

	case MTS_STATAUS_AOI_START_INSPECTION_CMD: str = _T("AOI Start Inspection");	break;
	case MTS_STATAUS_AOI_START_INSPECTION_RES: str = _T("AOI Start Inspection Response");	break;

	case MTS_STATAUS_AOI_INSPECTION_COMPLETE_CMD: str = _T("AOI Inspection Complete");	break;
	case MTS_STATAUS_AOI_INSPECTION_COMPLETE_RES: str = _T("AOI Inspection Complete Response");	break;

	case MTS_STATAUS_AOI_READY_TO_UNLOAD_CMD: str = _T("AOI Ready To Unload");	break;
	case MTS_STATAUS_AOI_READY_TO_UNLOAD_RES: str = _T("AOI Ready To Unload Response");	break;

	case MTS_STATAUS_AOI_UNLOAd_COMPLETE_CMD: str = _T("AOI Unload Complete");	break;
	case MTS_STATAUS_AOI_UNLOAd_COMPLETE_RES: str = _T("AOI Unload Complete Response");	break;

	case MTS_STATAUS_AOI_LOGIN_OUT_CMD: str = _T("AOI Login/Out Parameters");	break;
	case MTS_STATAUS_AOI_LOGIN_OUT_RES: str = _T("AOI Login/Out Parameters Response");	break;		

	case MTS_STATAUS_AOI_CHECK_BARCODE_CMD: str = _T("AOI Check Barcode");	break;
	case MTS_STATAUS_AOI_CHECK_BARCODE_RES: str = _T("AOI Check Barcode Response");	break;		

	case MTS_STATAUS_AOI_INSPECTION_STOP_CMD: str = _T("AOI Inspection Stop");	break;
	case MTS_STATAUS_AOI_INSPECTION_STOP_RES: str = _T("AOI Inspection Stop Response");	break;		

	case MTS_STATAUS_VRS_UPLOAD_SFC_CMD: str = _T("VRS Upload SFC");	break;
	case MTS_STATAUS_VRS_UPLOAD_SFC_RES: str = _T("VRS Upload SFC Response");	break;		
	default:
		str.Format(_T("Undefined Status ID[%d]"), StatusID);
		break;
	}	          
	return str;
}
//-------------------------------------------------------------------------------------//
int CMES_MTS::MapMTSComm_ProcessID(int ProcessID)//映射ProcessID
{
	int NewProcessID=0;
	switch ( ProcessID )
	{
	case MES_STATAUS_MES_SET_CMD: NewProcessID=MTS_STATAUS_MES_SET_CMD;	break;
	case MES_STATAUS_MES_SET_RES: NewProcessID=MTS_STATAUS_MES_SET_RES;	break;
	case MES_STATAUS_MES_GET_CMD: NewProcessID=MTS_STATAUS_MES_GET_CMD;	break;
	case MES_STATAUS_MES_GET_RES: NewProcessID=MTS_STATAUS_MES_GET_RES;	break;

	case MES_STATAUS_AOI_SET_CMD: NewProcessID=MTS_STATAUS_AOI_SET_CMD;	break;
	case MES_STATAUS_AOI_SET_RES: NewProcessID=MTS_STATAUS_AOI_SET_RES;	break;
	case MES_STATAUS_AOI_GET_CMD: NewProcessID=MTS_STATAUS_AOI_GET_CMD;	break;
	case MES_STATAUS_AOI_GET_RES: NewProcessID=MTS_STATAUS_AOI_GET_RES;	break;

	case MES_STATAUS_VRS_SET_CMD: NewProcessID=MTS_STATAUS_VRS_SET_CMD;	break;
	case MES_STATAUS_VRS_SET_RES: NewProcessID=MTS_STATAUS_VRS_SET_RES;	break;
	case MES_STATAUS_VRS_GET_CMD: NewProcessID=MTS_STATAUS_VRS_GET_CMD;	break;
	case MES_STATAUS_VRS_GET_RES: NewProcessID=MTS_STATAUS_VRS_GET_RES;	break;

	case MES_STATAUS_AOI_READY_TO_LOAD_CMD: NewProcessID=MTS_STATAUS_AOI_READY_TO_LOAD_CMD;	break;
	case MES_STATAUS_AOI_READY_TO_LOAD_RES: NewProcessID=MTS_STATAUS_AOI_READY_TO_LOAD_RES;	break;

	case MES_STATAUS_AOI_LOAd_COMPLETE_CMD: NewProcessID=MTS_STATAUS_AOI_LOAd_COMPLETE_CMD;	break;
	case MES_STATAUS_AOI_LOAd_COMPLETE_RES: NewProcessID=MTS_STATAUS_AOI_LOAd_COMPLETE_RES;	break;
		
	case MES_STATAUS_AOI_START_INSPECTION_CMD: NewProcessID=MTS_STATAUS_AOI_START_INSPECTION_CMD;	break;
	case MES_STATAUS_AOI_START_INSPECTION_RES: NewProcessID=MTS_STATAUS_AOI_START_INSPECTION_RES;	break;

	case MES_STATAUS_AOI_INSPECTION_COMPLETE_CMD: NewProcessID=MTS_STATAUS_AOI_INSPECTION_COMPLETE_CMD;	break;
	case MES_STATAUS_AOI_INSPECTION_COMPLETE_RES: NewProcessID=MTS_STATAUS_AOI_INSPECTION_COMPLETE_RES;	break;

	case MES_STATAUS_AOI_READY_TO_UNLOAD_CMD: NewProcessID=MTS_STATAUS_AOI_READY_TO_UNLOAD_CMD;	break;
	case MES_STATAUS_AOI_READY_TO_UNLOAD_RES: NewProcessID=MTS_STATAUS_AOI_READY_TO_UNLOAD_RES;	break;

	case MES_STATAUS_AOI_UNLOAd_COMPLETE_CMD: NewProcessID=MTS_STATAUS_AOI_UNLOAd_COMPLETE_CMD;	break;
	case MES_STATAUS_AOI_UNLOAd_COMPLETE_RES: NewProcessID=MTS_STATAUS_AOI_UNLOAd_COMPLETE_RES;	break;

	case MES_STATAUS_AOI_LOGIN_OUT_CMD: NewProcessID=MTS_STATAUS_AOI_LOGIN_OUT_CMD;	break;
	case MES_STATAUS_AOI_LOGIN_OUT_RES: NewProcessID=MTS_STATAUS_AOI_LOGIN_OUT_RES;	break;

	case MES_STATAUS_AOI_CHECK_BARCODE_CMD: NewProcessID=MTS_STATAUS_AOI_CHECK_BARCODE_CMD;	break;
	case MES_STATAUS_AOI_CHECK_BARCODE_RES: NewProcessID=MTS_STATAUS_AOI_CHECK_BARCODE_RES;	break;

	case MES_STATAUS_AOI_INSPECTION_STOP_CMD: NewProcessID=MTS_STATAUS_AOI_INSPECTION_STOP_CMD;	break;
	case MES_STATAUS_AOI_INSPECTION_STOP_RES: NewProcessID=MTS_STATAUS_AOI_INSPECTION_STOP_RES;	break;
	
	case MES_STATAUS_VRS_UPLOAD_SFC_CMD: NewProcessID=MTS_STATAUS_VRS_UPLOAD_SFC_CMD;	break;
	case MES_STATAUS_VRS_UPLOAD_SFC_RES: NewProcessID=MTS_STATAUS_VRS_UPLOAD_SFC_RES;	break;	

	default:
	case MES_STATAUS_NONE:
		NewProcessID = MTS_STATAUS_NONE;
		break;
	}
	return NewProcessID;
}
//-------------------------------------------------------------------------------------//
bool CMES_MTS::CreateMESProcThread()//建立MES執行執行緒
{
	return CreateMTSProcThread();
}
//-------------------------------------------------------------------------------------//
bool CMES_MTS::DeleteMESProcThread()//刪除MES執行執行緒
{
	return DeleteMTSProcThread();
}
//-------------------------------------------------------------------------------------//
bool CMES_MTS::GetMESConnected()//是否已經連線
{
	return ReturnNotImplement(_T("CMES_MTS::GetMESConnected"));
}
//-------------------------------------------------------------------------------------//
bool CMES_MTS::ConnectMESLinker()//連線到MES連結軟體
{
	return ConnectMTSLinker();
}
//-------------------------------------------------------------------------------------//
bool CMES_MTS::DisconnectMESLinker()//停止連線到MES連結軟體
{
	return DisconnectMTSLinker();
}
//-------------------------------------------------------------------------------------//
bool CMES_MTS::ExecMESComm_CheckMESReady()//執行MES溝通-確認MES就緒
{
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_MTS::ExecMESComm_SetSystemParam()//執行MES溝通-設定系統參數
{
	return ExecMTSComm_SetSystemParam();
}
//-------------------------------------------------------------------------------------//
bool CMES_MTS::ExecMESComm_SetProjectParam(CAOIProject *ProjectPtr)//執行MES溝通-設定專案參數
{
	return ExecMTSComm_SetProjectParam();
}
//-------------------------------------------------------------------------------------//
bool CMES_MTS::ExecMESComm_SetProjectLoadFinished(CAOIProject *ProjectPtr, bool bSucc, LPCTSTR Err)//執行MES溝通-專案載入完畢
{
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_MTS::ExecMESComm_SetMachineStatus()//執行MES溝通-設定機台狀態
{
	return ExecMTSComm_SetMachineStatus();
}
//-------------------------------------------------------------------------------------//
bool CMES_MTS::ExecMESComm_SetMachineStatus(ONLINE_STATE_MODE Status)//執行MES溝通-設定機台狀態		
{
	return ExecMTSComm_SetMachineStatus(Status);
}
//-------------------------------------------------------------------------------------//
bool CMES_MTS::ExecMESComm_ProcessID(int ProcessID)//執行MES溝通-程序運作
{
	return ExecMTSComm_ProcessID(ProcessID);
}
//-------------------------------------------------------------------------------------//
bool CMES_MTS::ExecMESComm_SetAOIExceptionCode(LPCTSTR ErrStr)//執行MES溝通-系統異常碼
{
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_MTS::ExecMESComm_SetUserLogin_out(bool bLogin)//執行MES溝通-設定使用者登入	
{
	return ReturnNotImplement(_T("CMES_MTS::ExecMESComm_SetUserLogin_out"));
}
//-------------------------------------------------------------------------------------//
bool CMES_MTS::ExecMESComm_CheckBarcode(CAOIProject *ProjectPtr)//執行MES溝通-確認條碼
{
	return ReturnNotImplement(_T("CMES_MTS::ExecMESComm_CheckBarcode"));
}
//-------------------------------------------------------------------------------------//
size_t CMES_MTS::GetMESRecvNodeCount()
{
	return GetMTSRecvNodeCount();
}
//-------------------------------------------------------------------------------------//
bool CMES_MTS::ProcesMESRecvNodeList(bool bThread)//處理收到MES的訊息列表
{
	return ProcesMTSRecvNodeList(bThread);
}
//-------------------------------------------------------------------------------------//
bool CMES_MTS::SendToMESNode(const char *strBuf, bool bAck, int nPPID, DWORD AckTime)//送資料給MES
{
	return SendToMTSNode(strBuf, bAck, nPPID, AckTime);
}
//-------------------------------------------------------------------------------------//
#endif//MTS_DISABLE
#endif//MES_DISABLE