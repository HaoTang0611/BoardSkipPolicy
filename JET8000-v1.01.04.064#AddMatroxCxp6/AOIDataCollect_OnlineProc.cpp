//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "AOIDataCollect.h"
//-------------------------------------------------------------------------------------//
#include "JetLoadDll.h"
#include "InputBoxWnd.h"
#include "InputListWnd.h"
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecSystemRun_OnlineRunLock()//系統運作-線上運作鎖住
{
	bool bLock=GetIsOnlineRunLock();
	if ( false == bLock ) { return true; }
	bool bWaitUserInput=GetIsWaitUserInput();
	if ( true == bWaitUserInput ) { return true; }

	SetIsOnlineRunLock(false);
	m_ErrorString=GetErrorString_OnlineRun();
	PostMainFrameWndMessage(MSG_SYSTEM_EXCEPTION_CALLBACK, NULL, NULL);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecSystemRun_AutoSwitchToOnlineView()//系統運作-自動切換線上畫面
{
	const DWORD LimitTime=GetSystemParameter().m_AutoSwitchToOnlineViewTime;	
	if ( 0 == LimitTime ) { return true; }
	//if ( CheckOnlineFormViewID() == true ) { return true; }	
	if ( CheckRibbonCategoryIndex_OnlineFormView() == true ) { return true; }
	if ( GetIsSystemReleased() == true ) { return true; }
	if ( GetIsSystemException() == true ) { return true; }
	if ( GetIsWaitUserInput() == true ) { return true; }
	if ( CheckWaitUserInputCountZero() == false ) { return true; }	
	//if ( GetIsOnlineRunLock() == true ) { return true; }	

	DWORD CheckTime=LimitTime*1000;
	DWORD LastTickCount=GetUserLastInputTickCount();
	DWORD dTime=GetTickCount()-LastTickCount;
	if ( 0 == LastTickCount ) { return true; }
	if ( dTime < CheckTime ) { return true; }	
	SetUserLastInputTickCount(0);
	PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_SWITCH_TO_ONLINE_VIEW, NULL);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecSystemRun_AutoSwitchToOnlineRemoteCtrlMode()//系統運作-自動切換線上遠端控制模式
{	
	if ( TASK_STATE_RUNNING == GetOnlineTaskState() )	{	return true; }
	if ( CheckAutoSwitchToOnlineRemoteCtrlEnable() == false ) { return true; }
	const DWORD LimitTime=GetSystemParameter().m_AutoSwitchToOnlineRemoteCtrlTime;
	if ( 0 == LimitTime ) { return true; }	
	MES_EQP_CTRL_STATE_MODE MesEqpCtrlStateMode=GetSystemParameter().m_AutoSwitchToOnlineRemoteCtrlMode;	
	if ( MES_EQP_CTRL_STATE_NONE == MesEqpCtrlStateMode ) { return true; }
	if ( MesEqpCtrlStateMode == GetMES_EqpCtrlStateMode() ) { return true; }	
	//if ( CheckOnlineFormViewID() == true ) { return true; }	
	if ( CheckRibbonCategoryIndex_OnlineFormView() == false ) { return true; }
	if ( GetIsSystemReleased() == true ) { return true; }
	if ( GetIsSystemException() == true ) { return true; }
	if ( GetIsWaitUserInput() == true ) { return true; }
	if ( CheckWaitUserInputCountZero() == false ) { return true; }	
	//if ( GetIsOnlineRunLock() == true ) { return true; }	

	UINT WndCmdID = GetAutoSwitchToOnlineRemoteCtrlCmdID(MesEqpCtrlStateMode);
	if ( 0 == WndCmdID ) { return true; }
	DWORD CheckTime=LimitTime;
	DWORD LastTickCount=GetUserLastInputTickCount();
	DWORD dTime=GetTickCount()-LastTickCount;
	if ( 0 == LastTickCount ) { return true; }
	if ( dTime < CheckTime ) { return true; }	
	SetUserLastInputTickCount(0);	
	PostMainFrameWndMessage(WM_COMMAND, WndCmdID, NULL);	
	return true;
}
//-------------------------------------------------------------------------------------//
ONLINE_STATE_MODE CAOIDataCollect::GetNextOnlineStateMode(ONLINE_STATE_MODE OnlineState, LANE_ID LaneID, PCB_OUT_DIRECTION Direction, TEST_RESULT_ID TestResultID) const
{
	bool bPCBStop=false;	
	bool bLastSignal=false;
	bool bNextSignal=false;	
	ONLINE_STATE_MODE OnlineStateNew=OnlineState;
	PCB_OUT_MODE PCBOutMode=GetLanePCBOutMode(LaneID);	
	const bool bMultiLaneModeOff=CheckMultiLaneMode_Off();

	if ( true == bMultiLaneModeOff )
	{
		OnlineStateNew = ONLINE_STATE_INPUT_BARCODE;
		return OnlineStateNew;
	}

	if ( ONLINE_STATE_INSPECTION_STOP==OnlineState )
	{			
		bPCBStop=PlcCtrlPtr->GetConveryerSensorPCBStop(LaneID);		
		switch ( PCBOutMode )
		{
		default:
		case PCB_OUT_NORMAL:
		case PCB_OUT_WITH_IN:
		case PCB_OUT_SIDE_OUT:
		case PCB_OUT_OK_OUT_NG_SIDE:
			if ( true == bPCBStop )
			{	OnlineStateNew = ONLINE_STATE_PCB_READY; }
			else
			{	OnlineStateNew = ONLINE_STATE_INPUT_BARCODE; }
			break;		
		case PCB_OUT_LANE_AUTO:
			if ( true == bPCBStop )
			{	OnlineStateNew = ONLINE_STATE_PCB_READY; }
			else
			{	OnlineStateNew = ONLINE_STATE_PCB_AUTO_RUN_START; }
			break;
		}
		return OnlineStateNew;
	}

	if ( ONLINE_STATE_INSPECTION_FINISH==OnlineState || ONLINE_STATE_WAIT_FOR_REPAIR_VERIFY==OnlineState )
	{		
		bLastSignal=PlcCtrlPtr->GetLastStationSignal(LaneID);
		bNextSignal=PlcCtrlPtr->GetNextStationSignal(LaneID);		
		bool IsNeedReCheckPCBInside = GetIsNeedReCheckPCBInside();
		if ( false == IsNeedReCheckPCBInside )
		{	bPCBStop = true;	}
		else
		{	bPCBStop=PlcCtrlPtr->GetConveryerSensorPCBStop(LaneID);		}
		switch ( PCBOutMode )
		{
		default:
		case PCB_OUT_NORMAL:
		case PCB_OUT_WITH_IN:
			if ( false==IsNeedReCheckPCBInside || true==bPCBStop )
			{	OnlineStateNew = ONLINE_STATE_WAIT_FOR_NEXT_STATION;	}
			else
			{	OnlineStateNew = ONLINE_STATE_INPUT_BARCODE;	}			
			break;
		case PCB_OUT_SIDE_OUT:
			if ( false==IsNeedReCheckPCBInside || true==bPCBStop )
			{
				if ( PCB_OUT_DIR_BACKWARD == Direction )
				{	OnlineStateNew = ONLINE_STATE_PCB_BACK_START;	}
				else if ( PCB_OUT_DIR_BACKWARD_OUT == Direction )
				{	
					if ( false == bNextSignal )
					{	OnlineStateNew = ONLINE_STATE_PCB_BACK_START;	}
					else
					{	OnlineStateNew = ONLINE_STATE_WAIT_FOR_NEXT_STATION;	}
				}
				else
				{
					if ( false == bNextSignal )
					{	OnlineStateNew = ONLINE_STATE_PCB_OUT_INSIDE_START;	}
					else
					{	OnlineStateNew = ONLINE_STATE_WAIT_FOR_NEXT_STATION;	}
				}
			}
			else
			{	OnlineStateNew = ONLINE_STATE_INPUT_BARCODE; }						
			break;
		case PCB_OUT_OK_OUT_NG_SIDE:
			if ( false==IsNeedReCheckPCBInside || true==bPCBStop )
			{
				if ( PCB_OUT_DIR_BACKWARD == Direction )
				{	OnlineStateNew = ONLINE_STATE_PCB_BACK_START;	}
				else if ( PCB_OUT_DIR_BACKWARD_OUT == Direction )
				{
					if ( TEST_RESULT_NG==TestResultID || TEST_RESULT_FD==TestResultID )
					{	OnlineStateNew = ONLINE_STATE_PCB_BACK_START;	}
					else
					{	OnlineStateNew = ONLINE_STATE_WAIT_FOR_NEXT_STATION;	}
				}
				else
				{
					if ( TEST_RESULT_NG==TestResultID || TEST_RESULT_FD==TestResultID )
					{	OnlineStateNew = ONLINE_STATE_PCB_OUT_INSIDE_START;	}
					else
					{	OnlineStateNew = ONLINE_STATE_WAIT_FOR_NEXT_STATION;	}
				}
			}
			else
			{	OnlineStateNew = ONLINE_STATE_INPUT_BARCODE; }		
			break;
		case PCB_OUT_LANE_AUTO:
			OnlineStateNew = ONLINE_STATE_PCB_AUTO_RUN_START;
			break;
		}
		return OnlineStateNew;
	}

	if ( ONLINE_STATE_INPUT_BARCODE == OnlineState )
	{	return ONLINE_STATE_WAIT_FOR_LAST_STATION;	}

	if ( ONLINE_STATE_WAIT_FOR_LAST_STATION == OnlineState )
	{		
		bLastSignal=PlcCtrlPtr->GetLastStationSignal(LaneID);
		if ( false == bLastSignal )
		{	return OnlineStateNew; }

		switch ( PCBOutMode )
		{
		default:
		case PCB_OUT_NORMAL:
		case PCB_OUT_WITH_IN:
		case PCB_OUT_SIDE_OUT:
		case PCB_OUT_OK_OUT_NG_SIDE:
			OnlineStateNew = ONLINE_STATE_PCB_IN_START;	break;
			break;		
		case PCB_OUT_LANE_AUTO:
			OnlineStateNew = ONLINE_STATE_PCB_AUTO_RUN_START;
			break;
		}		
		return OnlineStateNew;
	}
	if ( ONLINE_STATE_WAIT_FOR_NEXT_STATION == OnlineState )
	{		
		bNextSignal=PlcCtrlPtr->GetNextStationSignal(LaneID);
		if ( false == bNextSignal )
		{	return OnlineStateNew; }

		switch ( PCBOutMode )
		{
		default:
		case PCB_OUT_NORMAL:
		case PCB_OUT_SIDE_OUT:
		case PCB_OUT_OK_OUT_NG_SIDE:
			if ( PCB_OUT_DIR_BACKWARD == Direction )
			{	OnlineStateNew = ONLINE_STATE_PCB_BACK_START;	}
			else if ( PCB_OUT_DIR_BACKWARD_OUT == Direction )
			{	OnlineStateNew = ONLINE_STATE_PCB_BACK_OUT_START;	}
			else
			{	OnlineStateNew = ONLINE_STATE_PCB_OUT_START;	}
			break;
		case PCB_OUT_WITH_IN:
			bLastSignal=PlcCtrlPtr->GetLastStationSignal(LaneID);
			if ( true == bLastSignal )
			{	OnlineStateNew = ONLINE_STATE_PCB_OUT_IN_START;	}
			else
			{	OnlineStateNew = ONLINE_STATE_PCB_OUT_START;	}				
			break;		
		case PCB_OUT_LANE_AUTO:
			OnlineStateNew = ONLINE_STATE_PCB_AUTO_RUN_START;
			break;
		}		
		return OnlineStateNew;
	}

	//進板
	if ( ONLINE_STATE_PCB_IN_START == OnlineState )
	{	return ONLINE_STATE_PCB_IN_CHECKING;	}
	if ( ONLINE_STATE_PCB_IN_CHECKING == OnlineState )
	{	return ONLINE_STATE_PCB_IN_FINISH;	}	
	if ( ONLINE_STATE_PCB_IN_FINISH == OnlineState )
	{	return ONLINE_STATE_PCB_READY;	}	

	//出板
	if ( ONLINE_STATE_PCB_OUT_START == OnlineState )
	{	return ONLINE_STATE_PCB_OUT_CHECKING;	}
	if ( ONLINE_STATE_PCB_OUT_CHECKING == OnlineState )
	{	return ONLINE_STATE_PCB_OUT_FINISH;	}	
	if ( ONLINE_STATE_PCB_OUT_FINISH == OnlineState )
	{	return ONLINE_STATE_INPUT_BARCODE;	}

	//停板邊
	if ( ONLINE_STATE_PCB_OUT_INSIDE_START == OnlineState )
	{	return ONLINE_STATE_PCB_OUT_INSIDE_CHECKING;	}
	if ( ONLINE_STATE_PCB_OUT_INSIDE_CHECKING == OnlineState )
	{	return ONLINE_STATE_PCB_OUT_INSIDE_FINISH;	}	
	if ( ONLINE_STATE_PCB_OUT_INSIDE_FINISH == OnlineState )
	{	return ONLINE_STATE_WAIT_FOR_PCB_REMOVED;	}	

	//退板
	if ( ONLINE_STATE_PCB_BACK_START == OnlineState )
	{	return ONLINE_STATE_PCB_BACK_CHECKING;	}
	if ( ONLINE_STATE_PCB_BACK_CHECKING == OnlineState )
	{	return ONLINE_STATE_PCB_BACK_FINISH;	}	
	if ( ONLINE_STATE_PCB_BACK_FINISH == OnlineState )
	{	return ONLINE_STATE_INPUT_BARCODE;	}	

	//退出板
	if ( ONLINE_STATE_PCB_BACK_OUT_START == OnlineState )
	{	return ONLINE_STATE_PCB_BACK_OUT_CHECKING;	}
	if ( ONLINE_STATE_PCB_BACK_OUT_CHECKING == OnlineState )
	{	return ONLINE_STATE_PCB_BACK_OUT_FINISH;	}	
	if ( ONLINE_STATE_PCB_BACK_OUT_FINISH == OnlineState )
	{	return ONLINE_STATE_INPUT_BARCODE;	}	

	//出板帶進板
	if ( ONLINE_STATE_PCB_OUT_IN_START == OnlineState )
	{	return ONLINE_STATE_PCB_OUT_IN_CHECKING;	}
	if ( ONLINE_STATE_PCB_OUT_IN_CHECKING == OnlineState )
	{	return ONLINE_STATE_PCB_OUT_IN_FINISH;	}	
	if ( ONLINE_STATE_PCB_OUT_IN_FINISH == OnlineState )
	{	return ONLINE_STATE_PCB_READY;	}

	//自動進出板
	if ( ONLINE_STATE_PCB_AUTO_RUN_START == OnlineState )
	{	return ONLINE_STATE_PCB_AUTO_RUN_CHECKING;	}
	if ( ONLINE_STATE_PCB_AUTO_RUN_CHECKING == OnlineState )
	{	return ONLINE_STATE_PCB_AUTO_RUN_FINISH;	}	
	if ( ONLINE_STATE_PCB_AUTO_RUN_FINISH == OnlineState )
	{	return ONLINE_STATE_PCB_READY;	}

	//雙軌進出板
	if ( ONLINE_STATE_PCB_DUAL_RUN_START == OnlineState )
	{	return ONLINE_STATE_PCB_DUAL_RUN_CHECKING;	}
	if ( ONLINE_STATE_PCB_DUAL_RUN_CHECKING == OnlineState )
	{	return ONLINE_STATE_PCB_DUAL_RUN_FINISH;	}	
	if ( ONLINE_STATE_PCB_DUAL_RUN_FINISH == OnlineState )
	{	return ONLINE_STATE_PCB_READY;	}

	//ONLINE_STATE_WAIT_FOR_PCB_REMOVED,//等待板子移走
	//ONLINE_STATE_WAIT_FOR_REPAIR_VERIFY,//等待維修站確認
	return OnlineState;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcAutoStopByIdleTime()//線上檢測-自動停機-依據閒置時間
{
	TASK_MODE TaskMode = GetTaskMode();
	TASK_STATE_MODE TaskStateMode=GetOnlineTaskState();
	if ( TASK_INSPECT_PROJECT != TaskMode ) { return true; }
	if ( TASK_STATE_TO_STOP == TaskStateMode ) { return true; }
	const TSystemParameter &SysParam=GetSystemParameter();
	if ( 0 == SysParam.m_OnlineAutoStopByIdleTime ) { return true; }

	ONLINE_STATE_MODE OnlineStateMode=GetOnlineStateMode();
	const bool bIdle=CheckOnlineIdleMode(OnlineStateMode);	
	if ( false == bIdle ) 
	{ 
		SetOnlineFirstIdleTickCount(0);
		return true; 
	}	
	
	DWORD CurrentTickCount=GetTickCount();
	DWORD FirstTickCount=GetOnlineFirstIdleTickCount();
	const DWORD CheckTime_ms=SysParam.m_OnlineAutoStopByIdleTime*60*1000;//minute -> ms
	if ( 0 == FirstTickCount ) 
	{ 
		SetOnlineFirstIdleTickCount(CurrentTickCount);
		return true;
	}

	const DWORD EllapseTime=CurrentTickCount-FirstTickCount;
	if ( EllapseTime < CheckTime_ms )
	{	return true; }
	
	CString str, str2;
	CString ProjectName;
	LANE_ID LaneID=GetActiveLaneID();
	CAOIProject *ProjectPtr=GetActiveProject();
	if ( NULL != ProjectPtr )
	{	ProjectName = ProjectPtr->GetProjectFileMainName();	}

	SetOnlineFirstIdleTickCount(0);
	PlcCtrlPtr->TurnOnInspectAlarm(LaneID);	
	str2=_T("Caution, Online Auto Stop By Idle Time");
	str2=LoadMultiLanguageString_OnlineFunc(str2, str2);
	if ( ProjectName.GetLength() == 0 )
	{	str = str2; }
	else
	{	str.Format(_T("%s\n%s"), ProjectName, str2);	}

	LockUserInput();
	if ( GetIsStopUserInput() == true )
	{ 
		UnlockUserInput();
		return false; 
	}
	SetIsWaitUserInput(true);
	ShowMessageSync(str);
	SetIsWaitUserInput(false);
	UnlockUserInput();

	PlcCtrlPtr->TurnOffInspectAlarm(LaneID);
	SetOnlineTaskState(TASK_STATE_TO_STOP);	

	bool bClearAllProject=false;
	if ( true == bClearAllProject )
	{
		SendMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_CLOSE, NULL);
		DestroyAllProject();	
		PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_SWITCH, NULL);	
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcAutoStopBySpecTime()//線上檢測-自動停機-依據特定時間
{	
	const TSystemParameter &SysParam=GetSystemParameter();	
	ExecOnlineProcAutoStopBySpecTimeFn(SysParam.m_OnlineAutoStopBySpecTime1);
	ExecOnlineProcAutoStopBySpecTimeFn(SysParam.m_OnlineAutoStopBySpecTime2);
	ExecOnlineProcAutoStopBySpecTimeFn(SysParam.m_OnlineAutoStopBySpecTime3);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcAutoStopBySpecTimeFn(__int64 SpecTime)//線上檢測-自動停機-依據特定時間
{
#ifdef ONLINE_AUTO_STOP_USE
	TASK_MODE TaskMode = GetTaskMode();
	TASK_STATE_MODE TaskStateMode=GetOnlineTaskState();
	if ( TASK_INSPECT_PROJECT != TaskMode ) { return true; }
	if ( TASK_STATE_TO_STOP == TaskStateMode ) { return true; }	
	if ( 0 == SpecTime ) { return true; }
	ONLINE_STATE_MODE OnlineStateMode=GetOnlineStateMode();
	const bool bCanStop=CheckOnlineCanAutoStop(OnlineStateMode);	
	if ( false == bCanStop ) {	return true; }	

	CTime TimeChk=SpecTime;
	CTime TimeNow=CTime::GetCurrentTime();	
	CTime TimeFirst=GetOnlineFirstRunDateTime();
	//TimeChk = TimeFirst+CTimeSpan(0, 0, 1, 0);//除錯使用
	
	const int Year=TimeNow.GetYear();
	const int Month=TimeNow.GetMonth();
	const int Day=TimeNow.GetDay();
	const int Sec=01;

	const int HourNow=TimeNow.GetHour();
	const int MinuteNow=TimeNow.GetMinute();

	const int HourChk=TimeChk.GetHour();
	const int MinuteChk=TimeChk.GetMinute();
	
	TimeChk=CTime(Year, Month, Day, HourChk, MinuteChk, Sec);
	TimeNow=CTime(Year, Month, Day, HourNow, MinuteNow, Sec);
	if ( TimeNow < TimeChk ) { return true; }
	if ( TimeFirst > TimeChk ) { return true; }
	
	CString str, str2;	
	CString ProjectName;
	LANE_ID LaneID=GetActiveLaneID();
	CAOIProject *ProjectPtr=GetActiveProject();
	CString strSpecTime=AOIDataDefine.GetOnlineAutoStopBySpecTimeText(SpecTime);

	if ( NULL != ProjectPtr )
	{	ProjectName = ProjectPtr->GetProjectFileMainName();	}

	SetOnlineFirstIdleTickCount(0);
	PlcCtrlPtr->TurnOnInspectAlarm(LaneID);	
	str2=_T("Caution, Online Auto Stop By Idle Spec Time");
	str2=LoadMultiLanguageString_OnlineFunc(str2, str2);
	str.Format(_T("%s [%s]"), str2, strSpecTime);
	if ( ProjectName.GetLength() != 0 )
	{
		str2 = str;
		str.Format(_T("%s\n%s"), ProjectName, str2);
	}

	LockUserInput();
	if ( GetIsStopUserInput() == true )
	{ 
		UnlockUserInput();
		return false; 
	}
	SetIsWaitUserInput(true);
	//ShowMessageSync(str);
	while ( true )
	{
		ShowMessageSync(str);
		//if ( UserLogin_OnlineAutoStop() == true )
		if ( OperateLevelOnlineUnlockAutoStop() == true )
		{	break; }
	};	
	SetIsWaitUserInput(false);
	UnlockUserInput();

	PlcCtrlPtr->TurnOffInspectAlarm(LaneID);
	SetOnlineTaskState(TASK_STATE_TO_STOP);		
	
	bool bClearAllProject=false;
	if ( true == bClearAllProject )
	{
		SendMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_CLOSE, NULL);
		DestroyAllProject();	
		PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_SWITCH, NULL);	
	}
	UserLogout_Check();
#endif//ONLINE_AUTO_STOP_USE
	return true;
}
//-------------------------------------------------------------------------------------//
CAOIProject* CAOIDataCollect::GetOnlineProcParamProject(TOnlineProcParam &Param)
{	
	CAOIProject *ProjectPtr = NULL;
	if ( NULL == Param.pProject )
	{	ProjectPtr = GetActiveProject();	}
	else
	{	ProjectPtr = (CAOIProject*)(Param.pProject); }	
	return ProjectPtr;
}
//-------------------------------------------------------------------------------------//
/// <summary>重設雙軌跳板計數與狀態</summary>
void CAOIDataCollect::ResetBoardSkipPolicies()
{
	m_BoardSkipPolicyLA.Reset();
	m_BoardSkipPolicyLB.Reset();
}
//-------------------------------------------------------------------------------------//
/// <summary>計算 PCB 就緒次數</summary>
void CAOIDataCollect::BeginOnlineBoardSkipPolicy(TOnlineProcParam &Param)
{
	switch ( Param.eLaneID )
	{
	case LANE_ID_A:
		m_BoardSkipPolicyLA.BoardCount();
		break;
	case LANE_ID_B:
		m_BoardSkipPolicyLB.BoardCount();
		break;
	}
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::GetOnlineProcLaneRunBypassMode(TOnlineProcParam &Param) const
{
	bool bBypass=false;
	TASK_MODE TaskMode = Param.eTaskMode;//任務模式		
	if ( TASK_LANE_BYPASS == TaskMode )
	{	bBypass = true; }
	else if ( TASK_INSPECT_PROJECT == TaskMode )
	{
		LANE_ID LaneID = Param.eLaneID;		
		const LANE_STATE_MODE LaneStateMode = GetOnlineLaneTaskState(LaneID);
		if (LANE_STATE_BYPASS == LaneStateMode)
		{	bBypass = true;	}

		bool bPreRunMode = Param.bConveyerPreRunMode;
		if ( false == bPreRunMode )
		{
			LANE_WORK_MODE LaneWorkMode=LANE_WORK_DISABLE;
			LANE_STATE_MODE LaneStateMode;
			switch ( Param.eLaneID )
			{
			case LANE_ID_B: LaneWorkMode=Param.eLaneWorkModeLB; break;
			case LANE_ID_A: LaneWorkMode=Param.eLaneWorkModeLA; break;
			}
			if ( LANE_WORK_BYPASS == LaneWorkMode )
			{	bBypass = true; }

			LaneStateMode = GetOnlineLaneTaskState(Param.eLaneID);
			if (LANE_STATE_BYPASS == LaneStateMode)
			{	bBypass = true;	}

            // 正常運作時，將本張板的跳板決策交給既有直通流程。
            if (false == bBypass && LANE_WORK_RUN == LaneWorkMode)
            {
                switch (LaneID)
                {
                case LANE_ID_A:
                    bBypass = m_BoardSkipPolicyLA.IsCurrentBoardSkipped();
                    break;
                case LANE_ID_B:
                    bBypass = m_BoardSkipPolicyLB.IsCurrentBoardSkipped();
                    break;
                }
            }
		}
	}	
	return bBypass;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::CheckOnlineProcPreMoveCamera(TOnlineProcParam &Param) const//線上檢測-確認是否可提前移動相機
{
	MULTI_LANE_MODE MultiLaneMode = CheckMultiLaneMode();
	if ( MULTI_LANE_1 == MultiLaneMode )
	{	return true;	}
	ONLINE_STATE_MODE OnlineState=GetOnlineStateMode();
	if (ONLINE_STATE_PCB_DUAL_RUN_CHECKING == OnlineState)
	{	return true;	}
	return false;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcLaneBypass(TOnlineProcParam &Param)//線上檢測-軌道直通
{
	LANE_ID LaneID = Param.eLaneID;	
	PCB_OUT_MODE PCBOutMode = GetLanePCBOutMode(LaneID);
	
	Param.dwSleepTime = 10;
	Param.eWndMessageMode = WND_MESSAGE_SEND;
	PlcCtrlPtr->SetPCBAutoRunPCBChaned(LaneID, false);
	if ( PCB_OUT_LANE_AUTO == PCBOutMode )//雙軌道模式
	{	Param.eOnlineStateNew = ONLINE_STATE_PCB_AUTO_RUN_START;	}
	else
	{	
		if ( PlcCtrlPtr->PLC_ReadConveryerSensor(false) == false )
		{
			SetErrorString(PlcCtrlPtr->GetPLCErrorString());
			return false;
		}
		const bool PCBInside = PlcCtrlPtr->CheckPCBInside(LaneID);
		const bool PCBOut = PlcCtrlPtr->GetConveryerSensorPCBOut(LaneID);
		if ( true == PCBInside )
		{
			PlcCtrlPtr->WriteSignalToNext(LaneID, true);
			Param.eOnlineStateNew = ONLINE_STATE_WAIT_FOR_NEXT_STATION;	
		}
		else
		{	
			if ( LAST_STATION_LINE_MODE_2 == Param.nLastStationLineMode )//2線式
			{	PlcCtrlPtr->WriteSignalToLast(LaneID, true); }
			Param.eOnlineStateNew = ONLINE_STATE_WAIT_FOR_LAST_STATION;	
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcInspectionStop(TOnlineProcParam &Param)//線上檢測-檢測停止-首次檢測
{	
	LANE_ID LaneID = Param.eLaneID;			
	TASK_MODE TaskMode = Param.eTaskMode;//任務模式	
	MULTI_LANE_MODE MultiLaneMode = Param.eMultiLaneMode;
	LANE_WORK_MODE LaneWorkMode_LA = Param.eLaneWorkModeLA;
	LANE_WORK_MODE LaneWorkMode_LB = Param.eLaneWorkModeLB;	
	PCB_OUT_DIRECTION PCBOutDirection = Param.ePCBOutDirection;		
	const TSystemParameter &SystemParam = GetSystemParameter();
	Param.eWndMessageMode = WND_MESSAGE_SEND;
	if ( MULTI_LANE_2 == MultiLaneMode )//雙軌道模式
	{	
		bool AutoRunLane_LA = true;
		bool AutoRunLane_LB = true;
		Param.eLaneIDNext = LaneID;
		ONLINE_STATE_MODE OnlineState_LA = GetOnlineStateMode_LA();
		ONLINE_STATE_MODE OnlineState_LB = GetOnlineStateMode_LB();
		CAOIProject *ProjectPtr_A = GetLaneProjectPtr(LANE_ID_A, 0, true);
		CAOIProject *ProjectPtr_B = GetLaneProjectPtr(LANE_ID_B, 0, true);
		bool SensorPcbStop_LA = PlcCtrlPtr->GetConveryerSensorPCBStop(LANE_ID_A);
		bool SensorPcbStop_LB = PlcCtrlPtr->GetConveryerSensorPCBStop(LANE_ID_B);
		if ( TASK_LANE_BYPASS != TaskMode )
		{	
			bool BeforeRetrieveCode = false;
			ONLINE_STATE_MODE LocalOnlineStateMode=ONLINE_STATE_INSPECTION_STOP;
			if ( CheckOpenProjectBarcodeCanRun(Param.eFromMode) )
			{
				if ( ExecOpenProjectBarcodeDeviceStartToRead(LocalOnlineStateMode, LANE_ID_A, BeforeRetrieveCode) == false )//重新開始讀取條碼
				{	return false; }		
				if ( ExecOpenProjectBarcodeDeviceStartToRead(LocalOnlineStateMode, LANE_ID_B, BeforeRetrieveCode) == false )//重新開始讀取條碼
				{	return false;	}	
			}	
			if ( LANE_WORK_RUN == LaneWorkMode_LA )
			{	
				if ( ExecProjectBarcodeDeviceStartToRead(ProjectPtr_A, LocalOnlineStateMode, LANE_ID_A, BeforeRetrieveCode) == false )//重新開始讀取條碼
				{	return false;	}
				if ( false==SensorPcbStop_LA || ONLINE_STATE_INSPECTION_FINISH==OnlineState_LA || ONLINE_STATE_PCB_INSPECTION_PAUSE == OnlineState_LA)
				{	AutoRunLane_LA = true; }
				else
				{	AutoRunLane_LA = false; }
			}
			if ( LANE_WORK_RUN == LaneWorkMode_LB )
			{	
				if ( ExecProjectBarcodeDeviceStartToRead(ProjectPtr_B, LocalOnlineStateMode, LANE_ID_B, BeforeRetrieveCode) == false )//重新開始讀取條碼
				{	return false;	}
				if ( false==SensorPcbStop_LB || ONLINE_STATE_INSPECTION_FINISH==OnlineState_LB || ONLINE_STATE_PCB_INSPECTION_PAUSE == OnlineState_LB)
				{	AutoRunLane_LB = true; }
				else
				{	AutoRunLane_LB = false; }
			}
			bool SensorPcbStop = PlcCtrlPtr->GetConveryerSensorPCBStop(LaneID);
			if ( true == SensorPcbStop )
			{	//軌道上有板子不自動進出板, 直接檢測
				switch ( LaneID )
				{
				case LANE_ID_A:
					if ( LANE_WORK_RUN == LaneWorkMode_LA )
					{
						if ( ONLINE_STATE_INSPECTION_FINISH == OnlineState_LA )
						{	Param.eLaneIDNext = LANE_ID_B;	}
						else
						{	Param.eLaneIDNext = LANE_ID_A;	}
					}
					else
					{	Param.eLaneIDNext = LANE_ID_B;	}
					break;
				case LANE_ID_B:
					if ( LANE_WORK_RUN == LaneWorkMode_LB )
					{
						if ( ONLINE_STATE_INSPECTION_FINISH == OnlineState_LB )
						{	Param.eLaneIDNext = LANE_ID_A;	}
						else
						{	Param.eLaneIDNext = LANE_ID_B;	}
					}
					else
					{	Param.eLaneIDNext = LANE_ID_A;	}
					break;
				}				
				if ( false==AutoRunLane_LA || false==AutoRunLane_LB )
				{	Param.eOnlineStateNew = ONLINE_STATE_PCB_READY;	}
				else
				{	Param.eOnlineStateNew = ONLINE_STATE_PCB_AUTO_RUN_CHECKING; }
			}
			else
			{	Param.eOnlineStateNew = ONLINE_STATE_PCB_AUTO_RUN_CHECKING;	}
		}
		else
		{	Param.eOnlineStateNew = ONLINE_STATE_PCB_AUTO_RUN_CHECKING;	}

		const bool bDualRunMode = CheckDualRunMode();//20230425	
		if ( true == bDualRunMode )
		{
			if ( ONLINE_STATE_PCB_AUTO_RUN_CHECKING == Param.eOnlineStateNew )
			{	Param.eOnlineStateNew = ONLINE_STATE_PCB_DUAL_RUN_CHECKING;	}
		}

		if ( LANE_WORK_DISABLE != LaneWorkMode_LA )
		{	
			if ( true == AutoRunLane_LA )
			{	
				ONLINE_STATE_MODE OnlineStateMoeLane=OnlineState_LA;
				if ( LaneID == LANE_ID_A )
				{	OnlineStateMoeLane = ONLINE_STATE_INSPECTION_STOP; }
				if ( false == bDualRunMode )
				{	ExecPCBAutoRunProc(LANE_ID_A, false, false, PCBOutDirection);	}
				else
				{	ExecPCBDualRunProc(LANE_ID_A, false, false, ProjectPtr_A, PCBOutDirection, OnlineStateMoeLane);	}					
			}					
		}
				
		if ( LANE_WORK_DISABLE != LaneWorkMode_LB )
		{	
			if ( true == AutoRunLane_LB )
			{
				ONLINE_STATE_MODE OnlineStateMoeLane=OnlineState_LB;				
				if ( LaneID == LANE_ID_B )
				{	OnlineStateMoeLane = ONLINE_STATE_INSPECTION_STOP; }
				if ( false == bDualRunMode )
				{	ExecPCBAutoRunProc(LANE_ID_B, false, false, PCBOutDirection); }
				else
				{	ExecPCBDualRunProc(LANE_ID_B, false, false, ProjectPtr_B, PCBOutDirection, OnlineStateMoeLane);	}				
			}					
		}
		if ( TASK_LANE_BYPASS != TaskMode )
		{
			CAOIProject *ProjectPtr = GetActiveProject();
			if ( CheckLandProjectPtr(Param.eLaneIDNext, ProjectPtr) == false )
			{
				ProjectPtr = GetLaneProjectPtr(Param.eLaneIDNext, 0, true);
				if ( NULL == ProjectPtr )
				{	
					CString strLaneID = AOIDataDefine.GetLaneIDText(Param.eLaneIDNext);
					m_ErrorString.Format(_T("Error, No Project [%s]"), strLaneID);
					return false;
				}
				ProjectPtr->SetProjectActLaneID(Param.eLaneIDNext);
				SetActiveProjectIndex(ProjectPtr->GetProjectIndex());
				SendMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_SWITCH, NULL);
			}
			SetActiveLaneID(Param.eLaneIDNext);
		}
	}
	else
	{	
		//單軌道模式
		if ( TASK_LANE_BYPASS == TaskMode )
		{			
			if ( PlcCtrlPtr->CheckPCBInside(LaneID) == true )
			{
				PlcCtrlPtr->WriteSignalToNext(LaneID, true);
				Param.eOnlineStateNew = ONLINE_STATE_WAIT_FOR_NEXT_STATION;	
			}
			else
			{
				if ( LAST_STATION_LINE_MODE_2 == Param.nLastStationLineMode )//2線式
				{	PlcCtrlPtr->WriteSignalToLast(LaneID, true); }
				Param.eOnlineStateNew = ONLINE_STATE_WAIT_FOR_LAST_STATION;	
			}
		}
		else
		{
			bool BeforeRetrieveCode = false;
			bool SensorPcbStop = PlcCtrlPtr->GetConveryerSensorPCBStop(LaneID);
			ONLINE_STATE_MODE LocalOnlineStateMode=ONLINE_STATE_INSPECTION_STOP;
			if ( CheckOpenProjectBarcodeCanRun(Param.eFromMode) )
			{
				if ( ExecOpenProjectBarcodeDeviceStartToRead(LocalOnlineStateMode, LaneID, BeforeRetrieveCode) == false )//重新開始讀取條碼
				{	return false;	}
			}			

			CAOIProject *ProjectPtr = GetActiveProject();
			if ( ExecProjectBarcodeDeviceStartToRead(ProjectPtr, LocalOnlineStateMode, LaneID, BeforeRetrieveCode) == false )//重新開始讀取條碼
			{	return false;	}		
			if ( MULTI_LANE_OFF == MultiLaneMode )
			{	Param.eOnlineStateNew = ONLINE_STATE_INPUT_BARCODE;	}
			else
			{
				if ( true == SensorPcbStop )
				{	Param.eOnlineStateNew = ONLINE_STATE_PCB_READY;	}
				else
				{	Param.eOnlineStateNew = ONLINE_STATE_INPUT_BARCODE;	}
			}			
		}
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcPCBReady(TOnlineProcParam &Param)//線上檢測-PCB就緒
{	
	LANE_ID LaneID = Param.eLaneID;
	TASK_MODE TaskMode = Param.eTaskMode;//任務模式		
	Param.eWndMessageMode = WND_MESSAGE_SEND;
	
	BeginOnlineBoardSkipPolicy(Param);
	if ( GetOnlineProcLaneRunBypassMode(Param) == true )
	{	return ExecOnlineProcLaneBypass(Param);	}

	if ( ONLINE_FROM_CONVEYER_THREAD == Param.eFromMode )
	{
		Param.bExitLoop = true;
		return true;
	}

	if ( TASK_INSPECT_PROJECT == TaskMode )
	{		
		CAOIProject *ProjectPtr = GetActiveProject();
		if ( NULL != ProjectPtr )
		{	ProjectPtr->SetProjectNeedCheckDistrictPos(false);	}
	}

	SetIsIgnoreAutoRetry(true);	
	ONLINE_STATE_MODE LocalOnlineStateMode=ONLINE_STATE_PCB_READY;
	if ( CheckOpenProjectBarcodeCanRun(Param.eFromMode) )
	{			
		SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, _T("ExecOpenProjectBarcodeDeviceRetrieveCode"));
		if ( ExecOpenProjectBarcodeDeviceRetrieveCode(LocalOnlineStateMode, LaneID) == false )//讀取上一筆
		{	return false; }			
	}

	Param.dwSleepTime = 0;
	Param.eOnlineStateNew = ONLINE_STATE_PROJECT_OPEN_CODE;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcInputBarcode(TOnlineProcParam &Param)//線上檢測-輸入條碼
{
	LANE_ID LaneID = Param.eLaneID;		
	Param.eWndMessageMode = WND_MESSAGE_SEND;

	if ( GetOnlineProcLaneRunBypassMode(Param) == true )
	{	return ExecOnlineProcLaneBypass(Param);	}

	if ( CheckOpenProjectBarcodeCanRun(Param.eFromMode) )
	{
		if ( ExecOpenProjectBarcodeHandHeldReading(LaneID) == false )
		{	return false; }	
	}

	CAOIProject *ProjectPtr = GetOnlineProcParamProject(Param);	
	if ( ExecProjectBarcodeHandHeldReading(ProjectPtr, LaneID) == false )
	{	return false; }
	
	if ( MULTI_LANE_OFF != Param.eMultiLaneMode )
	{			
		if ( LAST_STATION_LINE_MODE_2 == Param.nLastStationLineMode )//2線式
		{	PlcCtrlPtr->WriteSignalToLast(LaneID, true); }			
	}
	Param.eOnlineStateNew = ONLINE_STATE_WAIT_FOR_LAST_STATION;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool  CAOIDataCollect::ExecOnlineProcInspectProjectMark(TOnlineProcParam &Param)//線上檢測-檢測專案標誌
{
	LANE_ID LaneID = Param.eLaneID;	
	Param.eWndMessageMode = WND_MESSAGE_SEND;
	const size_t ProjectCount = GetLaneProjectCount(LaneID);
	if ( 1 == ProjectCount )
	{	Param.eOnlineStateNew = ONLINE_STATE_INSPECTION_START;	}
	else
	{
		MULTI_PROJECT_TEST_ORDER_MODE MultiProjectTestOrderMode=GetMultiProjectTestOrderMode();
		if ( MULTI_PROJECT_TEST_ORDER_BY_MARK == MultiProjectTestOrderMode )
		{
			if ( ExecInspectionProjectMark() == false )
			{
				m_ErrorString = GetErrorString();
				return false;	
			}
		}
		Param.eOnlineStateNew = ONLINE_STATE_INSPECTION_START;
	}
	Param.dwSleepTime = 0;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcInspectProjectOpenCode(TOnlineProcParam &Param)//線上檢測-檢測專案開啟條碼
{	
	//SetIsIgnoreAutoRetry(true);	
	const TSystemParameter &SystemParam = GetSystemParameter();
	const bool bOpenProjectFinish = GetOnlineOpenProjectFinish();
	Param.eWndMessageMode = WND_MESSAGE_SEND;
	if ( FN_ENABLE!=SystemParam.m_OnlineOpenProjectCameraBarcode || true==bOpenProjectFinish )
	{
		Param.dwSleepTime = 0;
		Param.eOnlineStateNew = ONLINE_STATE_PROJECT_MARK;
		return true;
	}
	if ( ExecInspectionProjectOpenCode() == false )
	{
		m_ErrorString = GetErrorString();			
		return false;	
	}
	if ( GetOnlineOpenProjectCameraBarcodeTestState() == BARCODE_CAMERA_TEST_STATE_OTHERS )
	{	return true;	}

	Param.dwSleepTime = 0;
	Param.eOnlineStateNew = ONLINE_STATE_PROJECT_MARK;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcInspectionStart(TOnlineProcParam &Param)//線上檢測-檢測開始
{
	LANE_ID LaneID = Param.eLaneID;				
	const TSystemParameter &SystemParam = GetSystemParameter();
	Param.eWndMessageMode = WND_MESSAGE_SEND;
	SetMultiProjectTestResetDone(false, LaneID);

	if ( GetOnlineProcLaneRunBypassMode(Param) == true )
	{	return ExecOnlineProcLaneBypass(Param);	}		
			
	SetIsIgnoreAutoRetry(true);
	PlcCtrlPtr->SetPCBAutoRunPCBChaned(LaneID, false);
	const bool   bOpenProjectFinish = GetOnlineOpenProjectFinish();
	ONLINE_STATE_MODE LocalOnlineStateMode=ONLINE_STATE_INSPECTION_START;
	if ( CheckOpenProjectBarcodeCanRun(Param.eFromMode) )
	{
		if ( false == bOpenProjectFinish )
		{
			SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, _T("ExecOpenProjectBarcodeDeviceStartToRead"));//給檢測前讀取條碼
			if ( ExecOpenProjectBarcodeDeviceStartToRead(LocalOnlineStateMode, LaneID, true) == false )//重新開始讀取條碼
			{	return false;	}
			SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, _T("ExecOpenProjectBarcodeDeviceRetrieveCode"));
			if ( ExecOpenProjectBarcodeDeviceRetrieveCode(LocalOnlineStateMode, LaneID) == false )//讀取上一筆
			{	return false; }
		}
		SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, _T("ExecOpenProjectBarcodeDeviceStartToRead"));//給進板後讀取下一筆條碼
		if ( ExecOpenProjectBarcodeDeviceStartToRead(LocalOnlineStateMode, LaneID, false) == false )//重新開始讀取條碼
		{	return false;	}
	}	

	CAOIProject *ProjectPtr = GetOnlineProcParamProject(Param);
	SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, _T("ExecProjectBarcodeDeviceStartToRead"));//給檢測前讀取條碼
	if ( ExecProjectBarcodeDeviceStartToRead(ProjectPtr, LocalOnlineStateMode, LaneID, true) == false )//重新開始讀取條碼
	{	return false;	}
	SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, _T("ExecProjectBarcodeDeviceRetrieveCode"));
	if ( ExecProjectBarcodeDeviceRetrieveCode(ProjectPtr, LocalOnlineStateMode, LaneID) == false )//讀取上一筆
	{	return false; }
	SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, _T("ExecProjectBarcodeDeviceStartToRead"));//給進板後讀取下一筆條碼
	if ( ExecProjectBarcodeDeviceStartToRead(ProjectPtr, LocalOnlineStateMode, LaneID, false) == false )//重新開始讀取條碼
	{	return false;	}

	SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, _T("ExecReloadProject"));	
	if ( ExecReloadProject(LaneID) == false )
	{
		m_ErrorString = GetErrorString();
		return false; 
	}
	SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, _T("ExecReloadServerProject"));	
	if ( ExecReloadServerProject(LaneID) == false )
	{
		m_ErrorString = GetErrorString();
		return false; 
	}		
	SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, _T("ExecNPM_LoadPCBSerialFile"));	
	if ( ExecNPM_LoadPCBSerialFile(LaneID) == false )
	{
		m_ErrorString = GetErrorString();
		return false; 
	}
	SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, _T("ExecNPM_LoadAPC_FF1_File"));	
	if ( ExecNPM_LoadAPC_FF1_File(LaneID) == false )
	{
		m_ErrorString = GetErrorString();
		return false; 
	}
	SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, _T("ExecNPM_LoadAPC_FF2_File"));	
	if ( ExecNPM_LoadAPC_FF2_File(LaneID) == false )
	{
		m_ErrorString = GetErrorString();
		return false; 
	}
	SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, _T("ExecNPM_LoadAPC_MFB_File"));	
	if ( ExecNPM_LoadAPC_MFB_File(LaneID) == false )
	{
		m_ErrorString = GetErrorString();
		return false; 
	}

	ProjectPtr = GetActiveProject();
	SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, _T("ExecInspectionProject"));	
	if ( NULL != ProjectPtr )
	{
		CString ProjectShowName = ProjectPtr->GetProjectShowName();
		SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, ProjectShowName);	
	}
	if ( ExecInspectionProject() == false )
	{
		m_ErrorString = GetErrorString();
		return false;	
	}
	Param.dwSleepTime = 50;
	Param.eOnlineStateNew = ONLINE_STATE_INSPECTION_WAITING;		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcInspectionWaiting(TOnlineProcParam &Param)//線上檢測-檢測等待中
{	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcInspectFdPanel(TOnlineProcParam &Param)//線上檢測-檢測定位點-整板
{
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcInspectFdBoard(TOnlineProcParam &Param)//線上檢測-檢測定位點-單板
{
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcInspectProject(TOnlineProcParam &Param)//線上檢測-檢測專案
{
	if ( ExecOnlineProcPCBAutoRunCheckingOnInspecting() == false )
	{	return false; }
	Param.eWndMessageMode = WND_MESSAGE_SEND;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcStaticsProject(TOnlineProcParam &Param)//線上檢測-統計專案
{
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcInspectionFinish(TOnlineProcParam &Param)//線上檢測-檢測完畢
{
	LANE_ID LaneID = Param.eLaneID;		
	LANE_ID NextLaneID = Param.eLaneIDNext;		
	TASK_MODE TaskMode = Param.eTaskMode;//任務模式
	PCB_OUT_MODE PCBOutMode = Param.ePCBOutMode;
	ONLINE_FROM_MODE  OnlineFromMode = Param.eFromMode; 
	MULTI_LANE_MODE MultiLaneMode = Param.eMultiLaneMode;	
	PCB_OUT_DIRECTION PCBOutDirection = Param.ePCBOutDirection;
	const TSystemParameter &SystemParam = GetSystemParameter();	
	const bool bDualRunMode = CheckDualRunMode();//20230425	
	CAOIProject *ProjectPtr = GetOnlineProcParamProject(Param);
	Param.eWndMessageMode = WND_MESSAGE_SEND;
	if ( NULL == ProjectPtr )
	{	SetIsOnlineCheckSystemReady(true);	}
	else
	{
		if ( ProjectPtr->GetProjectConveyerPreRunRunning() == false )
		{	SetIsOnlineCheckSystemReady(true);	}
	}	
	
	NextLaneID = LaneID;	
	ResetSwitchMultiLine_NextLaneID();
	if ( MULTI_LANE_2 == MultiLaneMode )//雙軌道模式
	{					
		bool bSucc=true;
		if ( true == bDualRunMode )
		{	bSucc = CheckOtherLaneDualRunFinish(LaneID, NextLaneID);	}		
		else
		{	bSucc = CheckOtherLaneAutoRunFinish(LaneID, NextLaneID);	}
		if ( false == bSucc )//20230425
		{
			Param.eOnlineStateNew = ONLINE_STATE_INSPECTION_STOP;			
			return false;	
		}
	}	
	Param.eLaneIDNext = NextLaneID;
	LANE_STATE_MODE LaneStateMode = AOIDataCollect.GetOnlineLaneTaskState(NextLaneID);

	if ( NextLaneID == LaneID || LANE_STATE_PAUSE <= LaneStateMode)
	{	MoveCameraToBeforePCBInPosition(false); }	
	else
	{	
		if ( TASK_LANE_BYPASS != TaskMode )
		{
			LANE_WORK_MODE LaneWorkMode = GetLaneWorkMode(NextLaneID);					
			if ( LANE_WORK_BYPASS == LaneWorkMode || LANE_STATE_BYPASS == LaneStateMode)
			{	MoveCameraToBeforePCBInPosition(false); }
			else if ( LANE_WORK_RUN == LaneWorkMode )
			{	
				CAOIProject *NextProjectPtr = NULL;
				if ( FN_ENABLE == SystemParam.m_OnlineOpenProjectCameraBarcode )
				{	NextProjectPtr = GetOpenProjectProjectPtr();	}
				else 
				{	NextProjectPtr = GetLaneProjectPtr(NextLaneID, 0, true);	}
				if ( NULL != NextProjectPtr )
				{
					SetSwitchMultiLine_NextLaneID(NextLaneID);
					if ( MoveCameraToProjectFirstPos(NextProjectPtr, NextLaneID, DISTRICT_ID_A, false) == false )
					{	return false; }
				}
			}
		}
	}

	if ( GetOnlineProcLaneRunBypassMode(Param) == false )
	{
		SetIsIgnoreAutoRetry(true);
		bool BeforeRetrieveCode = true;		
		ONLINE_STATE_MODE LocalOnlineStateMode=ONLINE_STATE_INSPECTION_FINISH;				
		if ( CheckOpenProjectBarcodeCanRun(Param.eFromMode) )
		{
			if ( ExecOpenProjectBarcodeDeviceStartToRead(LocalOnlineStateMode, LaneID, BeforeRetrieveCode) == false )//重新開始讀取條碼
			{	return false;	}
			if ( ExecOpenProjectBarcodeDeviceRetrieveCode(LocalOnlineStateMode, LaneID) == false )//讀取上一筆
			{	return false; }		
		}
		
		if ( ExecProjectBarcodeDeviceStartToRead(ProjectPtr, LocalOnlineStateMode, LaneID, BeforeRetrieveCode) == false )
		{	return false;	}
		if ( ExecProjectBarcodeDeviceRetrieveCode(ProjectPtr, LocalOnlineStateMode, LaneID) == false )
		{	return false; }
	}			
		
	if ( true==bDualRunMode && false==Param.bConveyerPreRunMode )
	{	
		if ( ExecPCBDualRunProc(LaneID, false, false, ProjectPtr, PCBOutDirection, ONLINE_STATE_INSPECTION_FINISH) == false )
		{	return false; }
		if (ONLINE_FROM_ONLINE_THREAD == Param.eFromMode)
		{
			LANE_ID OtherLaneID;
			switch ( LaneID )
			{
			case LANE_ID_B: OtherLaneID=LANE_ID_A; break;
			case LANE_ID_A: OtherLaneID=LANE_ID_B; break;
			}
			const size_t OtherLaneIdx=GetConveyerAutoRunLaneIdx(OtherLaneID);
			if ( true == CheckConveyerAutoRunIdle(OtherLaneIdx) )
			{
				CAOIProject *OtherProjectPtr = GetLaneProjectPtr(OtherLaneID, 0, true);
				if ( ExecPCBDualRunProc(OtherLaneID, false, false, OtherProjectPtr, PCBOutDirection, GetOnlineStateMode_Lane(OtherLaneID)) == false )
				{	return false; }	
			}
		}
		Param.eOnlineStateNew = ONLINE_STATE_PCB_DUAL_RUN_CHECKING;
		return true;
	}

	TEST_RESULT_ID TestResultID = ProjectPtr->GetProjectResultCurrent().sResultID;	
	ONLINE_STATE_MODE OnlineStateNew = GetNextOnlineStateMode(ONLINE_STATE_INSPECTION_FINISH, LaneID, PCBOutDirection, TestResultID);
	if ( ONLINE_STATE_WAIT_FOR_NEXT_STATION == OnlineStateNew )
	{	PlcCtrlPtr->WriteSignalToNext(LaneID, true); }	
	Param.eOnlineStateNew = OnlineStateNew;		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcProjectSwitchProcMode(TOnlineProcParam &Param)//線上檢測-專案切換程序
{	
	bool bChagne=false;
	MULTI_PROJECT_TEST_ORDER_MODE MultiProjectTestOrderMode=GetMultiProjectTestOrderMode();
	if ( MULTI_PROJECT_TEST_ORDER_BY_TURN == MultiProjectTestOrderMode )
	{		
		bChagne = true;
		Param.eOnlineStateNew = ONLINE_STATE_PROJECT_SWITCH_BY_TURN;
		if ( ONLINE_FROM_CONVEYER_THREAD == Param.eFromMode )
		{	Param.bExitLoop = true;	}					
	}
	if ( MULTI_PROJECT_TEST_ORDER_ONE_CYCLE_A_B==MultiProjectTestOrderMode || MULTI_PROJECT_TEST_ORDER_ONE_CYCLE_B_A==MultiProjectTestOrderMode )
	{	
		bChagne = true;
		Param.eOnlineStateNew = ONLINE_STATE_PROJECT_SWITCH_BY_TURN_ONE_CYCLE_RESET;
		if ( ONLINE_FROM_CONVEYER_THREAD == Param.eFromMode )
		{	Param.bExitLoop = true;	}
	}	
	return bChagne;//No Change to Project Switch
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcProjectSwitchByTurn(TOnlineProcParam &Param)//線上檢測-專案切換-輪流
{
	LANE_ID LaneID = Param.eLaneID;			
	Param.eWndMessageMode = WND_MESSAGE_POST;

	if ( GetOnlineProcLaneRunBypassMode(Param) == true )
	{	return ExecOnlineProcLaneBypass(Param);	}

	if ( ExecInspectionProjectByTurn(LaneID) == false )
	{	return false; }
	PCB_OUT_MODE PCBOutMode = GetLanePCBOutModeRunning(LaneID);
	switch ( PCBOutMode )
	{
	case PCB_OUT_WITH_IN:	Param.eOnlineStateNew = ONLINE_STATE_PCB_OUT_IN_CHECKING;	break;
	case PCB_OUT_LANE_AUTO:	Param.eOnlineStateNew = ONLINE_STATE_PCB_AUTO_RUN_CHECKING;	break;
	default:
		Param.eOnlineStateNew = ONLINE_STATE_INPUT_BARCODE;
		break;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcProjectSwitchByTurnOneCycleReset(TOnlineProcParam &Param)//線上檢測-專案切換-輪流
{
	LANE_ID LaneID = Param.eLaneID;			
	Param.eWndMessageMode = WND_MESSAGE_POST;

	if ( GetOnlineProcLaneRunBypassMode(Param) == true )
	{	return ExecOnlineProcLaneBypass(Param);	}

	if ( ExecInspectionProjectByTurnOneCycleReset(LaneID) == false )
	{	return false; }
	PCB_OUT_MODE PCBOutMode = GetLanePCBOutModeRunning(LaneID);
	switch ( PCBOutMode )
	{
	case PCB_OUT_WITH_IN:	Param.eOnlineStateNew = ONLINE_STATE_PCB_OUT_IN_CHECKING;	break;
	case PCB_OUT_LANE_AUTO:	Param.eOnlineStateNew = ONLINE_STATE_PCB_AUTO_RUN_CHECKING;	break;
	default:
		Param.eOnlineStateNew = ONLINE_STATE_INPUT_BARCODE;
		break;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcWaitForLastStation(TOnlineProcParam &Param)//線上檢測-等待上一站
{	
	LANE_ID LaneID = Param.eLaneID;	
	bool BypassLastSignal = Param.bBypassLastSignal;
	PCB_OUT_DIRECTION PCBOutDirection = Param.ePCBOutDirection;	
	Param.eWndMessageMode = WND_MESSAGE_POST;
	if ( MULTI_LANE_OFF == Param.eMultiLaneMode )
	{
		const bool bPCBStop=PlcCtrlPtr->GetConveryerSensorPCBStop(LaneID);
		if ( false == bPCBStop )
		{	return true; }
		Param.eOnlineStateNew = ONLINE_STATE_PCB_READY;
		Param.dwSleepTime = 0;
		return true;
	}
	if ( PlcCtrlPtr->UpdateTowerLightState_WaitLast(LaneID) == false )
	{
		SetErrorString(PlcCtrlPtr->GetPLCErrorString());
		return false;
	}
	if ( PCB_OUT_DIR_BACKWARD!=PCBOutDirection || false==BypassLastSignal )
	{				
		if ( PlcCtrlPtr->GetLastStationSignal(LaneID) == false )
		{	return true;	}
	}			
	if ( ExecNPM_CopyPCBSerialFile(LaneID) == false )
	{	return false; }	
	Param.eOnlineStateNew = ONLINE_STATE_PCB_IN_START;
	Param.dwSleepTime = 0;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcWaitForNextStation(TOnlineProcParam &Param)//線上檢測-等待下一站
{	
	const bool bReadPLCNode = true;
	LANE_ID LaneID = Param.eLaneID;		
	LANE_ID NextLaneID = Param.eLaneIDNext;		
	TASK_MODE TaskMode = Param.eTaskMode;//任務模式
	PCB_OUT_MODE PCBOutMode = Param.ePCBOutMode;	
	const bool BypassNextSignal = Param.bBypassNextSignal;	
	PCB_OUT_DIRECTION PCBOutDirection = Param.ePCBOutDirection;	
	const bool NextSignal = PlcCtrlPtr->GetNextStationSignal(LaneID);	
	Param.eWndMessageMode = WND_MESSAGE_POST;
	if ( PlcCtrlPtr->UpdateTowerLightState_WaitNext(LaneID) == false )
	{
		SetErrorString(PlcCtrlPtr->GetPLCErrorString());
		return false;
	}
	if ( PCB_OUT_SIDE_OUT==PCBOutMode || PCB_OUT_OK_OUT_NG_SIDE==PCBOutMode )
	{	//同流向才會進到這裡
		if ( false == NextSignal )
		{
			int   i=0;
			const int WaitForPCBRemovedCountMax = GetSystemParameter().m_CheckPCBRemovedCount;
			for ( i=0; i<WaitForPCBRemovedCountMax; i++ )
			{
				if ( true == bReadPLCNode )
				{
					if ( PlcCtrlPtr->PLC_ReadConveryerSensor(false) == false ) 
					{	
						m_ErrorString = PlcCtrlPtr->GetPLCErrorString();
						return false; 
					}
				}
				if ( PCB_OUT_DIR_BACKWARD_OUT == PCBOutDirection )
				{
					if ( PlcCtrlPtr->GetConveryerSensorPCBIn(LaneID) == true )
					{	return true; }
				}
				else
				{
					if ( PlcCtrlPtr->GetConveryerSensorPCBOut(LaneID) == true )
					{	return true; }
				}
				if ( false == bReadPLCNode )
				{	::Sleep(10); }
			}			
			if ( TASK_LANE_BYPASS != TaskMode )
			{	Param.eOnlineStateNew = ONLINE_STATE_INPUT_BARCODE; }
			else
			{	
				if ( LAST_STATION_LINE_MODE_2 == Param.nLastStationLineMode )//2線式
				{	PlcCtrlPtr->WriteSignalToLast(LaneID, true); }
				Param.eOnlineStateNew = ONLINE_STATE_WAIT_FOR_LAST_STATION; 
			}
			return true;		
		}
	}

	if ( TASK_LANE_BYPASS != TaskMode )
	{
		int UncheckCount=0;
		LANE_WORK_MODE LaneWorkMode=LANE_WORK_DISABLE;
		const int MaxUncheckTestFileCount=GetSystemParameter().m_MaxUncheckTestFileCount;
		switch ( LaneID )
		{
		case LANE_ID_B: LaneWorkMode=Param.eLaneWorkModeLB;	break;
		default:
		case LANE_ID_A: LaneWorkMode=Param.eLaneWorkModeLA;	break;

		}
		if ( LANE_WORK_RUN==LaneWorkMode && MaxUncheckTestFileCount>0 )
		{	
			if ( ExecMESComm_UnCheckTestFile(LaneID, UncheckCount) == true )
			{
				if ( UncheckCount >= MaxUncheckTestFileCount )
				{	return true;	}
			}			
		}
	}

	if ( PCB_OUT_DIR_BACKWARD!=PCBOutDirection || false==BypassNextSignal )
	{
		if ( PlcCtrlPtr->GetNextStationSignal(LaneID) == false )
		{	return true; }
	}	
	//PlcCtrlPtr->WriteSignalToNext(LaneID, true);
	if ( PCB_OUT_DIR_BACKWARD == PCBOutDirection )
	{	Param.eOnlineStateNew = ONLINE_STATE_PCB_BACK_START;	}
	else if ( PCB_OUT_DIR_BACKWARD_OUT == PCBOutDirection )
	{	Param.eOnlineStateNew = ONLINE_STATE_PCB_BACK_OUT_START;	}
	else
	{	
		if ( PCB_OUT_WITH_IN == PCBOutMode )
		{
			if ( PlcCtrlPtr->GetLastStationSignal(LaneID) == false )
			{	Param.eOnlineStateNew = ONLINE_STATE_PCB_OUT_START;	}
			else
			{
				PlcCtrlPtr->WriteSignalToLast(LaneID, true);
				Param.eOnlineStateNew = ONLINE_STATE_PCB_OUT_IN_START; 
			}
		}
		else
		{	Param.eOnlineStateNew = ONLINE_STATE_PCB_OUT_START;	}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcPCBInStart(TOnlineProcParam &Param)//線上檢測-進板開始
{
	LANE_ID LaneID = Param.eLaneID;	
	ONLINE_FROM_MODE FromMode = Param.eFromMode;
	Param.eWndMessageMode = WND_MESSAGE_POST;
	if ( GetOnlineProcLaneRunBypassMode(Param) == false )
	{
		SetIsIgnoreAutoRetry(true);
		bool BeforeRetrieveCode = true;		
		ONLINE_STATE_MODE LocalOnlineStateMode=ONLINE_STATE_PCB_IN_START;
		if ( CheckOpenProjectBarcodeCanRun(Param.eFromMode) )
		{			
			if ( ExecOpenProjectBarcodeDeviceStartToRead(LocalOnlineStateMode, LaneID, BeforeRetrieveCode) == false )//重新開始讀取條碼
			{	return false;	}
			if ( ExecOpenProjectBarcodeDeviceRetrieveCode(LocalOnlineStateMode, LaneID) == false )//讀取上一筆
			{	return false; }		
		}	

		if (true == GetHASI_Enable()) {
			const int HASIQueueSize = GetHASI_SerialFileQueueSize();
			DWORD HASIWaitedTime = 0, HASIDwellTime = 50;
			DWORD HASITimeout = GetHASI_SerialFileDwellTime();
			bool bRead = false;
			if (HASIQueueSize > 0) {
				while (HASIWaitedTime < HASITimeout && false == bRead) {
					bRead = ExecHASI_LoadPCBSerialFile(LaneID);
					Sleep(HASIDwellTime); HASIWaitedTime += HASIDwellTime;
				}
				if (false == bRead) { return false; }
			}
		}

		CAOIProject *ProjectPtr = GetOnlineProcParamProject(Param);
		if ( ExecProjectBarcodeDeviceStartToRead(ProjectPtr, LocalOnlineStateMode, LaneID, BeforeRetrieveCode) == false )
		{	return false;	}
		if ( ExecProjectBarcodeDeviceRetrieveCode(ProjectPtr, LocalOnlineStateMode, LaneID) == false )
		{	return false; }
	}			
	if ( LAST_STATION_LINE_MODE_2 != Param.nLastStationLineMode )//非2線式
	{	PlcCtrlPtr->WriteSignalToLast(LaneID, true); }
	const int  UseCameraPcbIn=GetPCBInUseCameraImageMode();
	if ( FN_ENABLE == UseCameraPcbIn )
	{
		if ( ExecPCBInByCamera(LaneID) == false )
		{	return false;	}
	}
	else
	{
		if ( PlcCtrlPtr->ExecPCBIn(LaneID, false) == false )
		{
			m_ErrorString = PlcCtrlPtr->GetPLCErrorString();
			return false;
		}
	}
	if ( AutoSetupAllLightSetting() == false )
	{	return false; }
	Param.dwSleepTime = 10;
	Param.eOnlineStateNew = ONLINE_STATE_PCB_IN_CHECKING;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcPCBInChecking(TOnlineProcParam &Param)//線上檢測-進板確認
{
	const bool bResetOKNG = true;
	LANE_ID LaneID = Param.eLaneID;
	const bool PreMove = CheckOnlineProcPreMoveCamera(Param);
	if (true == PreMove)//20230425
	{
		CAOIProject *ProjectPtr = GetConveyerFirstProjectPtr(LaneID);
		if ( MoveCameraToConveyerFirstPos(Param, ProjectPtr, bResetOKNG) == false )
		{	return false; }
	}
	const int  UseCameraPcbIn=GetPCBInUseCameraImageMode();
	if ( FN_DISABLE	== UseCameraPcbIn )
	{
		if ( PlcCtrlPtr->CheckPCBInFault(LaneID) == true )
		{
			m_ErrorString = PlcCtrlPtr->GetPLCErrorString();
			return false;
		}
		if ( PlcCtrlPtr->CheckPCBInFinish(LaneID) == false )
		{	return true;	}
	}
	Param.dwSleepTime = 0;
	Param.eWndMessageMode = WND_MESSAGE_POST;
	Param.eOnlineStateNew = ONLINE_STATE_PCB_IN_FINISH;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcPCBInFinish(TOnlineProcParam &Param)//線上檢測-進板完成
{	
	Param.eWndMessageMode = WND_MESSAGE_POST;	
	Param.dwSleepTime = 0;
	CheckExecOnlineAutoCalibration(Param);

	if ( ONLINE_FROM_CONVEYER_THREAD == Param.eFromMode )
	{
		if ( GetOnlineProcLaneRunBypassMode(Param) == false )		
		{	Param.bExitLoop = true;		}
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcPCBOutStart(TOnlineProcParam &Param)//線上檢測-出板開始
{
	LANE_ID LaneID = Param.eLaneID;	
	TASK_MODE TaskMode = Param.eTaskMode;//任務模式	
	Param.eWndMessageMode = WND_MESSAGE_POST;
	SaveControlCenterLog(_T("ExecOnlineProcPCBOutStart"));
	ExecCCSDataChange(LaneID, NULL);
	SetLanePCBOutModeRunning(LaneID, GetLanePCBOutMode(LaneID));
	if ( PlcCtrlPtr->ExecPCBOut(LaneID, false) == false )
	{
		m_ErrorString = PlcCtrlPtr->GetPLCErrorString();
		return false;
	}	
	if ( AutoSetupAllLightSetting() == false )
	{	return false; }
	Param.dwSleepTime = 10;
	Param.eOnlineStateNew = ONLINE_STATE_PCB_OUT_CHECKING;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcPCBOutChecking(TOnlineProcParam &Param)//線上檢測-出板確認
{
	LANE_ID LaneID = Param.eLaneID;	
	Param.eWndMessageMode = WND_MESSAGE_POST;
	if ( PlcCtrlPtr->CheckPCBOutFault(LaneID) == true )
	{
		m_ErrorString = PlcCtrlPtr->GetPLCErrorString();
		return false;
	}
	if ( PlcCtrlPtr->CheckPCBOutFinish(LaneID) == false )
	{	return true; }	
	Param.dwSleepTime = 10;
	Param.eOnlineStateNew = ONLINE_STATE_PCB_OUT_FINISH;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcPCBOutFinish(TOnlineProcParam &Param)//線上檢測-出板完成
{	
	LANE_ID LaneID = Param.eLaneID;		
	Param.eWndMessageMode = WND_MESSAGE_POST;
	if ( GetOnlineProcLaneRunBypassMode(Param) == true )
	{
		if ( LAST_STATION_LINE_MODE_2 == Param.nLastStationLineMode )//2線式
		{	PlcCtrlPtr->WriteSignalToLast(LaneID, true); }
		Param.eOnlineStateNew = ONLINE_STATE_WAIT_FOR_LAST_STATION; 
	}
	else
	{
		if ( ExecOnlineProcProjectSwitchProcMode(Param) == false )
		{	Param.eOnlineStateNew = ONLINE_STATE_INPUT_BARCODE;	}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcPCBOutInsideStart(TOnlineProcParam &Param)//線上檢測-停側邊開始
{
	LANE_ID LaneID = Param.eLaneID;		
	Param.eWndMessageMode = WND_MESSAGE_POST;
	SetLanePCBOutModeRunning(LaneID, GetLanePCBOutMode(LaneID));
	if ( PlcCtrlPtr->ExecPCBOutInside(LaneID, false) == false )
	{
		m_ErrorString = PlcCtrlPtr->GetPLCErrorString();
		return false;
	}
	if ( AutoSetupAllLightSetting() == false )
	{	return false; }
	Param.dwSleepTime = 10;
	Param.eOnlineStateNew = ONLINE_STATE_PCB_OUT_INSIDE_CHECKING;			
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcPCBOutInsideChecking(TOnlineProcParam &Param)//線上檢測-停側邊確認
{
	LANE_ID LaneID = Param.eLaneID;	
	Param.eWndMessageMode = WND_MESSAGE_POST;
	if ( PlcCtrlPtr->CheckPCBOutInsideFault(LaneID) == true )
	{
		m_ErrorString = PlcCtrlPtr->GetPLCErrorString();
		return false;
	}
	if ( PlcCtrlPtr->CheckPCBOutInsideFinish(LaneID) == false )
	{	return true; }
	Param.eOnlineStateNew = ONLINE_STATE_PCB_OUT_INSIDE_FINISH;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcPCBOutInsideFinish(TOnlineProcParam &Param)//線上檢測-停側邊完成
{	
	LANE_ID LaneID = Param.eLaneID;	
	TASK_MODE TaskMode = Param.eTaskMode;//任務模式
	PCB_OUT_MODE PcbOutMode = Param.ePCBOutMode;

	Param.eWndMessageMode = WND_MESSAGE_POST;
	if ( PCB_OUT_OK_OUT_NG_SIDE == PcbOutMode )
	{	Param.eOnlineStateNew = ONLINE_STATE_WAIT_FOR_PCB_REMOVED;	}
	else
	{
		if ( PlcCtrlPtr->WriteSignalToNext(LaneID, true) == false )
		{
			m_ErrorString = PlcCtrlPtr->GetPLCErrorString();
			return false;
		}
		Param.eOnlineStateNew = ONLINE_STATE_WAIT_FOR_NEXT_STATION;
	}
	::Sleep(10);//避免PLC來不及更新, 所以等待
	return true;
}
//-------------------------------------------------------------------------------------//
bool  CAOIDataCollect::ExecOnlineProcPCBBackStart(TOnlineProcParam &Param)//線上檢測-退板開始
{
	LANE_ID LaneID = Param.eLaneID;
	Param.eWndMessageMode = WND_MESSAGE_POST;
	SaveControlCenterLog(_T("ExecOnlineProcPCBBackStart"));
	ExecCCSDataChange(LaneID, NULL);
	SetLanePCBOutModeRunning(LaneID, GetLanePCBOutMode(LaneID));
	if ( PlcCtrlPtr->ExecPCBBack(LaneID, false) == false )
	{
		m_ErrorString = PlcCtrlPtr->GetPLCErrorString();
		return false;
	}
	if ( AutoSetupAllLightSetting() == false )
	{	return false; }
	Param.dwSleepTime = 10;
	Param.eOnlineStateNew = ONLINE_STATE_PCB_BACK_CHECKING;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcPCBBackChecking(TOnlineProcParam &Param)//線上檢測-退板確認
{
	LANE_ID LaneID = Param.eLaneID;	
	Param.eWndMessageMode = WND_MESSAGE_POST;
	if ( PlcCtrlPtr->CheckPCBBackFault(LaneID) == true )
	{
		m_ErrorString = PlcCtrlPtr->GetPLCErrorString();
		return false;
	}
	if ( PlcCtrlPtr->CheckPCBBackFinish(LaneID) == false )
	{	return true; }
	Param.dwSleepTime = 0;
	Param.eOnlineStateNew = ONLINE_STATE_PCB_BACK_FINISH;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcPCBBackFinish(TOnlineProcParam &Param)//線上檢測-退板完成
{	
	LANE_ID LaneID = Param.eLaneID;	
	PCB_OUT_MODE PCBOutMode = Param.ePCBOutMode;	
	PCB_OUT_DIRECTION  PCBOutDirection = Param.ePCBOutDirection;
	Param.eWndMessageMode = WND_MESSAGE_POST;
	if ( PCB_OUT_DIR_BACKWARD_OUT == PCBOutDirection )
	{
		if (  PCB_OUT_OK_OUT_NG_SIDE==PCBOutMode )
		{	Param.eOnlineStateNew = ONLINE_STATE_WAIT_FOR_PCB_REMOVED;	}
		else
		{
			if ( PlcCtrlPtr->WriteSignalToNext(LaneID, true) == false )
			{
				m_ErrorString = PlcCtrlPtr->GetPLCErrorString();
				return false;
			}
			Param.eOnlineStateNew = ONLINE_STATE_WAIT_FOR_NEXT_STATION;
		}
		::Sleep(10);//避免PLC來不及更新, 所以等待
		return true;
	}
	if ( PCB_OUT_SIDE_OUT==PCBOutMode || PCB_OUT_OK_OUT_NG_SIDE==PCBOutMode )
	{	
		Param.eOnlineStateNew = ONLINE_STATE_WAIT_FOR_PCB_REMOVED;	
		::Sleep(10);//避免PLC來不及更新, 所以等待
	}
	else
	{				
		if ( GetOnlineProcLaneRunBypassMode(Param) == true )
		{
			if ( LAST_STATION_LINE_MODE_2 == Param.nLastStationLineMode )//2線式
			{	PlcCtrlPtr->WriteSignalToLast(LaneID, true); }
			Param.eOnlineStateNew = ONLINE_STATE_WAIT_FOR_LAST_STATION; 	
		}
		else
		{
			if ( ExecOnlineProcProjectSwitchProcMode(Param) == false )
			{	Param.eOnlineStateNew = ONLINE_STATE_INPUT_BARCODE;	}	
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcPCBBackOutStart(TOnlineProcParam &Param)//線上檢測-退出板開始
{
	LANE_ID LaneID = Param.eLaneID;
	Param.eWndMessageMode = WND_MESSAGE_POST;
	SaveControlCenterLog(_T("ExecOnlineProcPCBBackOutStart"));
	ExecCCSDataChange(LaneID, NULL);
	SetLanePCBOutModeRunning(LaneID, GetLanePCBOutMode(LaneID));		
	if ( PlcCtrlPtr->ExecPCBBackOut(LaneID, false) == false )
	{
		m_ErrorString = PlcCtrlPtr->GetPLCErrorString();
		return false;
	}	
	if ( AutoSetupAllLightSetting() == false )
	{	return false; }
	Param.dwSleepTime = 10;
	Param.eOnlineStateNew = ONLINE_STATE_PCB_BACK_OUT_CHECKING;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcPCBBackOutChecking(TOnlineProcParam &Param)//線上檢測-退出板確認
{
	LANE_ID LaneID = Param.eLaneID;	
	Param.eWndMessageMode = WND_MESSAGE_POST;	
	if ( PlcCtrlPtr->CheckPCBBackOutFault(LaneID) == true )
	{
		m_ErrorString = PlcCtrlPtr->GetPLCErrorString();
		return false;
	}
	if ( PlcCtrlPtr->CheckPCBBackOutFinish(LaneID) == false )
	{	return true; }	
	Param.dwSleepTime = 0;
	Param.eOnlineStateNew = ONLINE_STATE_PCB_BACK_OUT_FINISH;		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcPCBBackOutFinish(TOnlineProcParam &Param)//線上檢測-退出板完成
{
	LANE_ID LaneID = Param.eLaneID;		
	Param.eWndMessageMode = WND_MESSAGE_POST;
	if ( GetOnlineProcLaneRunBypassMode(Param) == true )
	{
		if ( LAST_STATION_LINE_MODE_2 == Param.nLastStationLineMode )//2線式
		{	PlcCtrlPtr->WriteSignalToLast(LaneID, true); }
		Param.eOnlineStateNew = ONLINE_STATE_WAIT_FOR_LAST_STATION; 
	}
	else
	{
		if ( ExecOnlineProcProjectSwitchProcMode(Param) == false )
		{	Param.eOnlineStateNew = ONLINE_STATE_INPUT_BARCODE;	}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcWaitForPCBRemoved(TOnlineProcParam &Param)//線上檢測-等待板子移除
{		
	LANE_ID LaneID = Param.eLaneID;		
	TASK_MODE TaskMode = Param.eTaskMode;//任務模式
	PCB_OUT_MODE PCBOutMode = Param.ePCBOutMode;
	PCB_OUT_DIRECTION  PCBOutDirection = Param.ePCBOutDirection;
	Param.eWndMessageMode = WND_MESSAGE_POST;

	int   i=0;
	bool bPcbExist=false;
	const bool bReadPLCNode = true;
	const int WaitForPCBRemovedCountMax = GetSystemParameter().m_CheckPCBRemovedCount;
	for ( i=0; i<WaitForPCBRemovedCountMax; i++ )
	{	//持續消失才會切換
		if ( true == bReadPLCNode )
		{
			if ( PlcCtrlPtr->PLC_ReadConveryerSensor(false) == false ) 
			{	
				m_ErrorString = PlcCtrlPtr->GetPLCErrorString();
				return false; 
			}
		}
		if ( PCB_OUT_DIR_FORWARD == PCBOutDirection )//PCB_OUT_OK_OUT_NG_SIDE
		{	bPcbExist = PlcCtrlPtr->GetConveryerSensorPCBOut(LaneID);	}
		else
		{	bPcbExist = PlcCtrlPtr->GetConveryerSensorPCBIn(LaneID);	}
		if ( true == bPcbExist )
		{	return true; }

		if ( false == bReadPLCNode )
		{	::Sleep(10); }
	}
	if ( GetOnlineProcLaneRunBypassMode(Param) == true )
	{
		if ( LAST_STATION_LINE_MODE_2 == Param.nLastStationLineMode )//2線式
		{	PlcCtrlPtr->WriteSignalToLast(LaneID, true); }
		Param.eOnlineStateNew = ONLINE_STATE_WAIT_FOR_LAST_STATION; 
	}
	else
	{
		if ( ExecOnlineProcProjectSwitchProcMode(Param) == false )
		{	Param.eOnlineStateNew = ONLINE_STATE_INPUT_BARCODE;	}	
	}		
	Param.dwSleepTime = 10;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcWaitForRepairVerify(TOnlineProcParam &Param)//線上檢測-等待維修站確認
{	
	LANE_ID LaneID = Param.eLaneID;
	PCB_OUT_MODE PCBOutMode = Param.ePCBOutMode;	
	PCB_OUT_DIRECTION PCBOutDirection = Param.ePCBOutDirection;	
	Param.eWndMessageMode = WND_MESSAGE_POST;

	if ( GetOnlineProcLaneRunBypassMode(Param) == false )				
	{		
		CAOIProject *ProjectPtr = GetOnlineProcParamProject(Param);
		if ( NULL == ProjectPtr )
		{	return false; }
		
		TEST_RESULT_ID TestResultID = TEST_RESULT_NONE;
		DEFECT_HANDLE_MODE DefectHandelMode = ProjectPtr->GetProjectParameter().m_DefectHandleMode;
		if ( DEFECT_HANDLE_WAIT_FOR_REPAIR == DefectHandelMode )//等待維修站
		{						
			if ( ExecRepairResultSignalLaneRepairFn(LaneID, TestResultID) == false )
			{	return false; }
			if ( TEST_RESULT_NONE == TestResultID )
			{	return true; }			
		}
		if ( DEFECT_HANDLE_CONTROL_CENTER == DefectHandelMode )//等待中控中心
		{
			if ( ExecRepairResultSignalLaneControlCenterFn(LaneID, TestResultID) == false )
			{	return false; }
			if ( TEST_RESULT_NONE == TestResultID )
			{	return true; }
		}
				
		ONLINE_STATE_MODE OnlineStateNew = GetNextOnlineStateMode(ONLINE_STATE_WAIT_FOR_REPAIR_VERIFY, LaneID, PCBOutDirection, TestResultID);
		if ( ONLINE_STATE_WAIT_FOR_NEXT_STATION == OnlineStateNew )
		{	PlcCtrlPtr->WriteSignalToNext(LaneID, true); }
		Param.eOnlineStateNew = OnlineStateNew; 	
		ResetSwitchMultiLine_NextLaneID();
		return true;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcPCBOutInStart(TOnlineProcParam &Param)//線上檢測-出板帶進板開始
{
	LANE_ID LaneID = Param.eLaneID;
	Param.eWndMessageMode = WND_MESSAGE_POST;
	SaveControlCenterLog(_T("ExecOnlineProcPCBOutInStart"));
	ExecCCSDataChange(LaneID, NULL);
	SetLanePCBOutModeRunning(LaneID, GetLanePCBOutMode(LaneID));
	if ( PlcCtrlPtr->ExecPCBOutIn(LaneID, false) == false )
	{
		m_ErrorString = PlcCtrlPtr->GetPLCErrorString();
		return false;
	}
	if ( AutoSetupAllLightSetting() == false )
	{	return false; }
	Param.dwSleepTime = 10;
	Param.eOnlineStateNew = ONLINE_STATE_PCB_OUT_IN_CHECKING;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcPCBOutInChecking(TOnlineProcParam &Param)//線上檢測-出板帶進板確認
{
	LANE_ID LaneID = Param.eLaneID;
	if ( GetOnlineProcLaneRunBypassMode(Param) == false )
	{
		const bool MultiProjectTestResetDone=GetMultiProjectTestResetDone(LaneID);
		if ( false == MultiProjectTestResetDone )
		{
			const int ConveryerStatus = PlcCtrlPtr->GetConveryerStatus(LaneID);	
			if ( PLC_CONVERYER_STATUS_PCB_IN_RUNNING == ConveryerStatus )
			{				
				if ( ExecOnlineProcProjectSwitchProcMode(Param) == true )
				{	return true;	}
			}
		}		
	}
	const bool bResetOKNG = true;
	const bool PreMove = CheckOnlineProcPreMoveCamera(Param);
	if (true == PreMove)
	{
		CAOIProject *ProjectPtr = GetConveyerFirstProjectPtr(LaneID);
		if ( MoveCameraToConveyerFirstPos(Param, ProjectPtr, bResetOKNG) == false )
		{	return false; }
	}
	if ( PlcCtrlPtr->CheckPCBOutInFault(LaneID) == true )
	{
		m_ErrorString = PlcCtrlPtr->GetPLCErrorString();
		return false;
	}
	if ( PlcCtrlPtr->CheckPCBOutInFinish(LaneID) == false )
	{	return true; }
	Param.dwSleepTime = 0;
	Param.eWndMessageMode = WND_MESSAGE_POST;
	Param.eOnlineStateNew = ONLINE_STATE_PCB_OUT_IN_FINISH;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcPCBOutInFinish(TOnlineProcParam &Param)//線上檢測-出板帶進板完成
{	
	Param.dwSleepTime = 0;
	Param.eWndMessageMode = WND_MESSAGE_POST;
	CheckExecOnlineAutoCalibration(Param);
	if ( ONLINE_FROM_CONVEYER_THREAD == Param.eFromMode )
	{
		if ( GetOnlineProcLaneRunBypassMode(Param) == false )
		{	Param.bExitLoop = true;	}
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcPCBAutoRunStart(TOnlineProcParam &Param)//線上檢測-自動進出板開始
{
	LANE_ID LaneID = Param.eLaneID;	
	PCB_OUT_DIRECTION PCBOutDirection = Param.ePCBOutDirection;
	Param.eWndMessageMode = WND_MESSAGE_POST;
	SetLanePCBOutModeRunning(LaneID, GetLanePCBOutMode(LaneID));
	if ( ExecPCBAutoRunProc(LaneID, false, false, PCBOutDirection) == false )
	{	return false; }	
	Param.dwSleepTime = 10;
	Param.eOnlineStateNew = ONLINE_STATE_PCB_AUTO_RUN_CHECKING;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcPCBAutoRunChecking(TOnlineProcParam &Param)//線上檢測-自動進出板確認
{	
	LANE_ID LaneID = Param.eLaneID;		
	ONLINE_STATE_MODE NewOnlineState = Param.eOnlineStateNew;		
	Param.eWndMessageMode = WND_MESSAGE_POST;
	if ( GetOnlineProcLaneRunBypassMode(Param) == false )
	{	
		const bool MultiProjectTestResetDone=GetMultiProjectTestResetDone(LaneID);
		if ( false == MultiProjectTestResetDone )
		{
			const int ConveryerStatus = PlcCtrlPtr->GetConveryerStatus(LaneID);	
			if ( PLC_CONVERYER_STATUS_PCB_IN_RUNNING == ConveryerStatus )
			{	
				if ( ExecOnlineProcProjectSwitchProcMode(Param) == true )					
				{	return true;	}
			}
		}
	}	
	if ( SwitchMultiLineOrder(Param, NewOnlineState) == false )
	{	return false;	}
	Param.eLaneID = GetActiveLaneID();
	Param.eOnlineStateNew = NewOnlineState;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcPCBAutoRunFinish(TOnlineProcParam &Param)//線上檢測-自動進出板完成
{	
	TASK_MODE TaskMode = Param.eTaskMode;//任務模式			
	Param.dwSleepTime = 0;	
	Param.eWndMessageMode = WND_MESSAGE_POST;
	CheckExecOnlineAutoCalibration(Param);
	if ( ONLINE_FROM_CONVEYER_THREAD == Param.eFromMode )
	{
		if ( GetOnlineProcLaneRunBypassMode(Param) == false )
		{	Param.bExitLoop = true;	}
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcPCBDualRunStart(TOnlineProcParam &Param)//線上檢測-雙軌進出板開始//20230425
{
	LANE_ID LaneID = Param.eLaneID;		
	CAOIProject *ProjectPtr=GetOnlineProcParamProject(Param);
	PCB_OUT_DIRECTION PCBOutDirection = Param.ePCBOutDirection;
	Param.eWndMessageMode = WND_MESSAGE_POST;
	SetLanePCBOutModeRunning(LaneID, GetLanePCBOutMode(LaneID));
	if ( ExecPCBDualRunProc(LaneID, false, false, ProjectPtr, PCBOutDirection, ONLINE_STATE_INSPECTION_FINISH) == false )
	{	return false; }
	Param.dwSleepTime = 10;
	Param.eOnlineStateNew = ONLINE_STATE_PCB_DUAL_RUN_CHECKING;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcPCBDualRunChecking(TOnlineProcParam &Param)//線上檢測-雙軌進出板確認//20230425
{
	LANE_ID LaneID = Param.eLaneID;		
	ONLINE_STATE_MODE NewOnlineState = Param.eOnlineStateNew;	
	PCB_OUT_DIRECTION PCBOutDirection = Param.ePCBOutDirection;	
	Param.eWndMessageMode = WND_MESSAGE_POST;

	const bool bSwitchProject=false;//雙軌不支援切換專案
	if ( true == bSwitchProject )
	{
		if ( GetOnlineProcLaneRunBypassMode(Param) == false )
		{	
			const bool MultiProjectTestResetDone=GetMultiProjectTestResetDone(LaneID);
			if ( false == MultiProjectTestResetDone )
			{
				const int ConveryerStatus = PlcCtrlPtr->GetConveryerStatus(LaneID);	
				if ( PLC_CONVERYER_STATUS_PCB_IN_RUNNING == ConveryerStatus )
				{	
					if ( ExecOnlineProcProjectSwitchProcMode(Param) == true )					
					{	return true;	}
				}
			}
		}	
	}

	if ( SwitchMultiLineOrder_Dual(Param, NewOnlineState) == false )
	{	return false;	}	
	Param.eLaneID = GetActiveLaneID();
	Param.eOnlineStateNew = NewOnlineState;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcPCBDualRunFinish(TOnlineProcParam &Param)//線上檢測-雙軌進出板完成//20230425	
{
	TASK_MODE TaskMode = Param.eTaskMode;//任務模式
	Param.dwSleepTime = 0;
	Param.eWndMessageMode = WND_MESSAGE_POST;
	CheckExecOnlineAutoCalibration(Param);
	if ( ONLINE_FROM_CONVEYER_THREAD == Param.eFromMode )
	{	Param.bExitLoop = true;	}		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineInputProjectWorkNumber(ONLINE_INPUT_TIMING CurTiming)//執行線上輸入專案工單號碼
{
#ifndef OFFLINE_VERSION
	const TSystemParameter &SysParam=GetSystemParameter();
	ONLINE_INPUT_TIMING OnlineInputTiming = SysParam.m_OnlineInputProjectWorkNumber;
	if ( CurTiming != OnlineInputTiming ) { return true; }
	//if ( MES_EQP_CTRL_STATE_REMOTE == GetMES_EqpCtrlStateMode() )
	//{	return true; }

	CAOIProject *ProjectPtr = GetActiveProject();
	if ( CheckProjectPtr(ProjectPtr) == false ) { return false; }
	
	CString      str;
	CString      strValue;
	CString      strLabel;
	CString      strCaption;	 
	CInputBoxWnd InputBox;

	strLabel = _T("Work Number");
	strLabel = LoadMultiLanguageString_OnlineInput(strLabel, strLabel);
	strCaption = _T("Input Project Work Number");
	strCaption = LoadMultiLanguageString_OnlineInput(strCaption, strCaption);

	strValue = ProjectPtr->GetProjectWorkNumber();
	InputBox.SetParam1(strCaption, strLabel, strValue);
	if ( InputBox.DoModal() == IDCANCEL )
	{
		str = _T("Error, Online Input Project Work Number Fault");
		m_ErrorString = LoadMultiLanguageString_OnlineInput(str, str);		
		return false;
	}

	strValue = InputBox.m_DataEdit1;
	if ( CheckProjectParamNeedToVerifyJsonString(PROJECT_PARAM_WORDK_NUMBER) == true )
	{
		if ( VerifyJsonString(strValue) == false )
		{	return false; }
	}
	ProjectPtr->SetProjectWorkNumber(strValue);		

	const bool bSaveProject=true;//要給後段讀取
	if ( true == bSaveProject )
	{
		CString Filename=ProjectPtr->GetProjectFileName();
		CString FileShowName=ProjectPtr->GetProjectShowName();
		if ( ProjectPtr->SaveProjectFile(Filename) == true )
		{
			if ( ::CopyFile(Filename, FileShowName, FALSE) == FALSE )
			{
				m_ErrorString.Format(_T("Error, Save Project File Fault[%s]"), FileShowName);
				return false;
			}
		}	
		SendToVRS_ProjectList();
	}
	PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_UPDATE, NULL);
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcAutoCalibrationFinish(TOnlineProcParam &Param)//線上檢測-結束
{
	Param.dwSleepTime = 0;
	Param.eWndMessageMode = WND_MESSAGE_POST;
	Param.eOnlineStateNew = ONLINE_STATE_PCB_READY;
	if ( ONLINE_FROM_CONVEYER_THREAD == Param.eFromMode )
	{	Param.bExitLoop = true;	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcAutoCalibration_XYZ_Home(TOnlineProcParam &Param)//線上檢測-自動校正-XYZ歸零
{
	bool IsOK = true;	
	const THREAD_GRAB_MODE ThreadGrabMode=GetThreadGrabMode();	
	SetThreadGrabMode(THREAD_GRAB_ONLINE_CALIBRATION);	
	IsOK = ExecOnlineProcAutoCalibration_XYZ_HomeFn(Param);
	UserLogout_Check();
	SetThreadGrabMode(ThreadGrabMode);		
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcAutoCalibration_XYZ_HomeFn(TOnlineProcParam &Param)//線上檢測-自動校正-XYZ歸零
{
	SetOnlineAutoCalibrationToExec_XYZ_Home(false);
	if ( ExecOnlineProcAutoCalibration_XYZ_HomeMoveToTarget() == false )
	{	return false; }
	if ( ExecOnlineProcAutoCalibration_XYZ_HomeCalibrate() == false )
	{	return false; }
	SetOnlineAutoCalibrationDateTime_XYZ_Home();
	SaveOnlineAutoCalibrationDateTime_XYZ_Home();	
	ExecOnlineProcAutoCalibrationFinish(Param);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcAutoCalibration_XYZ_HomeMoveToTarget()//線上檢測-自動校正-XYZ歸零-移動至原點上
{
	TPOINT3D StagePos;
	StagePos.x = StagePos.y = StagePos.z = 0;	
	if ( MotionCtrlPtr->XYZMoveTo(StagePos.x, StagePos.y, StagePos.z) == false )
	{
		SetErrorString(MotionCtrlPtr->GetErrorString());
		return false;
	}
	SetOnlineAutoCalibrationToExec_XYZ_Home(true);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcAutoCalibration_XYZ_HomeCalibrate()//線上檢測-自動校正-XYZ歸零-執行歸零
{
	if ( GetOnlineAutoCalibrationToExec_XYZ_Home() == false ) { return true; }

	bool bOpenMP = false;
	bool MoveToStartPos = true;
	if ( GetSystemParameter().m_OpenMPCount_General > 0 ) 
	{	bOpenMP = true; }
	if ( MotionCtrlPtr->WaitForMotionStop(false) == false )
	{
		m_ErrorString=MotionCtrlPtr->GetErrorString();
		return false;
	}
	if ( MotionCtrlPtr->ExecHomeAll(MoveToStartPos) == false )
	{
		m_ErrorString=MotionCtrlPtr->GetErrorString();
		return false;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcAutoCalibration_2D_Current(TOnlineProcParam &Param)//線上檢測-自動校正-2D電流
{	
	bool IsOK = true;
	const bool bWait=false;
	const THREAD_GRAB_MODE ThreadGrabMode=GetThreadGrabMode();
	if ( ExecTargetCapOnProc(bWait) == false )
	{	return false; }
	SetThreadGrabMode(THREAD_GRAB_ONLINE_CALIBRATION);	
	IsOK = ExecOnlineProcAutoCalibration_2D_CurrentFn(Param);
	UserLogout_Check();
	SetThreadGrabMode(ThreadGrabMode);	
	if ( ExecTargetCapOffProc(bWait) == false )
	{	return false; }
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcAutoCalibration_2D_CurrentFn(TOnlineProcParam &Param)//線上檢測-自動校正-2D電流
{	
	SetOnlineAutoCalibrationToExec_2DCurrent(false);
	if ( ExecOnlineProcAutoCalibration_2D_CurrentMoveToTarget() == false )
	{	return false; }
	if ( ExecOnlineProcAutoCalibration_2D_CurrentVerify() == false )
	{	return false; }	
	if ( ExecOnlineProcAutoCalibration_2D_CurrentCalibrate() == false )
	{	return false;	}

	SetOnlineAutoCalibrationDateTime_2D_Current();
	SaveOnlineAutoCalibrationDateTime_2D_Current();	
	ExecOnlineProcAutoCalibrationFinish(Param);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcAutoCalibration_2D_CurrentMoveToTarget()//線上檢測-自動校正-2D電流-移動至塊規上
{
	TPOINT3D StagePos;	
	const TCalibrationParameter &CaliParam=GetCalibrationParameter();
	StagePos.x = CaliParam.m_TargetWhitePosX;
	StagePos.y = CaliParam.m_TargetWhitePosY;
	StagePos.z = CaliParam.m_TargetWhitePosZ;	
	if ( MotionCtrlPtr->XYZMoveTo(StagePos.x, StagePos.y, StagePos.z) == false )
	{
		SetErrorString(MotionCtrlPtr->GetErrorString());
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcAutoCalibration_2D_CurrentVerify()//線上檢測-自動校正-2D電流-驗證
{	
	const char fnName[]="CAOIDataCollect::ExecOnlineProcAutoCalibration_2D_CurrentVerify";	
	if ( MotionCtrlPtr->WaitForMotionStop() == false )
	{
		SetErrorString(MotionCtrlPtr->GetErrorString());
		return false;
	}
	if ( WaitForTargetCapOnFinish() == false )
	{	return false; }	
	
	int   i=0, j=0;
	const int MaxCurrent=101;
	const int MaxLEDChannel=LED_CHANNEL_COUNT;
	const bool ResetLight = false;	
	std::vector<TSliceParam> SliceParamList;	
	if ( CloneSystemSliceParamList_No3D(SliceParamList) == false )
	{	return false; }
	const size_t SliceCount=SliceParamList.size();
	if ( 0 == SliceCount )
	{	return true; }	
	
	CString    str;	
	CString    str2;	
	CString    strTmp;		
	CString    Folder;
	CString    strResult;
	CString    strChannel;
	CString    strTarget;
	CString    strReading;
	IMAGE_SIZE ImageW=0;
	IMAGE_SIZE ImageH=0;
	IMAGE_SIZE ImageStep=0;
	IMAGE_SIZE ColorStep=0;
	IMAGE_SIZE BitCount=0;
	IMAGE_SIZE ColorBitCnt=0;
	IMAGE_PTR  RawImagePtr=NULL;	
	IMAGE_PTR  ColorImagePtr=NULL;
	bool bOutOfSpec=false;
	bool bColorCamera=false;
	bool bLedWhiteBalance=false;	
	const int   Channel = 3;
	const bool  bSaveImage=false;
	TImageStat  Statistics[Channel];
	std::vector<CString> StrList;
	const CAMERA_ID CameraID=PRIMARY_CAMERA_ID;	
	CAMERA_IMAGE_MODE CameraImageMode=GetCameraImageMode(CameraID);	
	const size_t BufferSize=CameraCtrl.GetCameraColorImageSize(CameraID);

	Folder.Format(_T("%s\\%s"), GetAOITempDirectory(), _T("Online2DCurrent"));
	CreateDirectory(Folder, NULL);

	if ( JetMemory.alloc_func(BufferSize, RawImagePtr, fnName, "RawImagePtr") == false ||
		 JetMemory.alloc_func(BufferSize, ColorImagePtr, fnName, "ColorImagePtr") == false )
	{
		SetErrorString(JetMemory.GetErrorString());
		JetMemory.free_func(RawImagePtr);
		JetMemory.free_func(ColorImagePtr);
		return false;
	}	
	if ( CAMERA_IMAGE_BAYER==CameraImageMode || CAMERA_IMAGE_COLOR==CameraImageMode )
	{	bColorCamera  = true;	}
	else
	{	bColorCamera  = false;	}

	for ( i=0; i<SliceCount; i++ )
	{
		RECT RoiRect;
		bool bFinish = false;
		int  TargetGray=0;
		int  CurrentCur=0;		
		int  CurrentGrayCur=0;
		int  GrayDifference=0;
		const long TotalFrameImageCount=1;
		std::vector<TSliceParam> ParamList;
		TSliceParam SliceParam=SliceParamList[i];		
		const double SliceGainValue=SliceParam.SliceGainValue;
		const int SliceTargetGray=(int)(SliceParam.SliceTargetGray);
		const int VerifyTolerance=(int)(SliceParam.SliceVerifyTolerance);	
		LED_CURRENT_CALI_MODE CurrCaliMode=SliceParam.SliceLightTable.LEDCurrCaliMode;
		if ( FN_DISABLE == SliceParam.SliceEnabled ) { continue; }
		if ( LED_CURRENT_CALI_DISABLE == CurrCaliMode ) { continue; }		
		bLedWhiteBalance = false;
		ParamList.clear();
		ParamList.push_back(SliceParam);
		if ( CameraCtrl.BatchGrabPrepare2(ParamList, true) == false )
		{
			JetMemory.free_func(RawImagePtr);
			JetMemory.free_func(ColorImagePtr);
			SetErrorString(CameraCtrl.GetErrorString());
			return false;
		}			
		if ( CameraCtrl.BatchGrabStart2(bFinish) == false )
		{
			JetMemory.free_func(RawImagePtr);
			JetMemory.free_func(ColorImagePtr);
			SetErrorString(CameraCtrl.GetErrorString());
			return false;
		}			
		if ( CameraCtrl.WaitForCameraImageCallbackCount(CameraID, TotalFrameImageCount) == false )
		{
			JetMemory.free_func(RawImagePtr);
			JetMemory.free_func(ColorImagePtr);
			SetErrorString(CameraCtrl.GetErrorString());
			return false;
		}
		if ( CameraCtrl.GetCameraImage3(CameraID, ImageW, ImageH, ImageStep, BitCount, RawImagePtr) == false )
		{
			JetMemory.free_func(RawImagePtr);
			JetMemory.free_func(ColorImagePtr);
			SetErrorString(CameraCtrl.GetErrorString());				
			return false; 
		}		
		
		const int ImageCpX=ImageW/2;
		const int ImageCpY=ImageH/2;
		const int RoiSizeX=ImageW/2;
		const int RoiSizeY=ImageH/2;
		RoiRect.left  = ImageCpX-(RoiSizeX/2);
		RoiRect.top   = ImageCpY-(RoiSizeY/2);
		RoiRect.right = RoiRect.left+RoiSizeX;
		RoiRect.bottom= RoiRect.top+RoiSizeY;
		Statistics[0].m_Rect=RoiRect;
		Statistics[1].m_Rect=RoiRect;
		Statistics[2].m_Rect=RoiRect;
		if ( true == bSaveImage )
		{
			str.Format(_T("%s\\Slice[%02d]_CurrentVerify.PNG"), Folder, i+1);
			if ( true == bLedWhiteBalance )
			{	ImageAPI.SaveImage(str, ImageW, ImageH, ColorStep, ColorBitCnt, ColorImagePtr, true);	}
			else
			{	ImageAPI.SaveImage(str, ImageW, ImageH, ImageStep, BitCount, RawImagePtr, true); }
		}

		
		if ( ImageAPI.CalcGrayImageStatistics(ImageW, ImageH, ImageStep, RawImagePtr, Statistics[0]) == false )
		{
			JetMemory.free_func(RawImagePtr);
			JetMemory.free_func(ColorImagePtr);
			SetErrorString(ImageAPI.GetImageApiErrorString());		
			return false;
		}			
			
		CurrentGrayCur=(int)(SliceGainValue*Statistics[0].m_Ave);
		GrayDifference=CurrentGrayCur-SliceTargetGray;
		if ( abs(GrayDifference) > VerifyTolerance )
		{	
			strResult=_T("NG");
			bOutOfSpec = true;
		}
		else
		{	strResult=_T("Pass"); }

		strTmp.Format(_T("Target=%03d, Reading=%03d, Diff=%03d => %s"), SliceTargetGray, CurrentGrayCur, GrayDifference, strResult);
		str.Format(_T("%s %s"), SliceParam.SliceName, strTmp);
		StrList.push_back(str);

		str2=_T("Verify 2D Current");
		str2=LoadMultiLanguageString_OnlineCalibration(str2, str2);
		str.Format(_T("%s-%s %s"), str2, SliceParam.SliceName, strTmp);			
		SaveOnlineAutoCalibrationLog(str);		
	}
	JetMemory.free_func(RawImagePtr);
	JetMemory.free_func(ColorImagePtr);
	
	if ( false == bOutOfSpec )
	{	return true;	}
	
	const TSystemParameter &SysParam=GetSystemParameter();
	FUNC_EXEC_MODE CalibrationMode=SysParam.m_OnlineAutoCalibrationMode_2DCurrent;
	if ( FUNC_EXEC_ASK == CalibrationMode )
	{
		const size_t StrCount=StrList.size();
		for ( i=0; i<StrCount; i++ )
		{
			if ( 0 == i )
			{	str = StrList[i]; }
			else
			{
				strTmp=str;
				str.Format(_T("%s\n%s"), strTmp, StrList[i]);
			}
		}
		strTmp=str;
		str2=_T("Do you want to calibrate 2D Current ?");
		str2=LoadMultiLanguageString_OnlineCalibration(str2, str2);
		str.Format(_T("%s\n%s"), str2, strTmp);

		DWORD Res=0;
		LANE_ID LaneID=LANE_ID_A;	
		PlcCtrlPtr->TurnOnInspectAlarm(LaneID);
		//if ( UserLogin_OnlineCalibration() == false )
		if ( OperateLevelOnlineUnlockCalibration() == false )
		{
			str=GetErrorString();
			SaveOnlineAutoCalibrationLog(str);
			PlcCtrlPtr->TurnOffInspectAlarm(LaneID);
			return false;
		}
		Res=JetAPI::ShowMessageBox(str, MB_YESNOCANCEL);
		PlcCtrlPtr->TurnOffInspectAlarm(LaneID);

		CString UserName=GetCurrentUserName();
		if ( IDCANCEL == Res )
		{
			str2=_T("Error, User Stop Online Calibration 2D Current");
			str2=LoadMultiLanguageString_OnlineCalibration(str2, str2);
			str.Format(_T("%s [%s]"), str2, UserName);
			SaveOnlineAutoCalibrationLog(str);
			SetErrorString(str);
			return false;
		}
		if ( IDNO == Res )
		{	
			str2=_T("Caution, User Cancel Online Calibration 2D Current");
			str2=LoadMultiLanguageString_OnlineCalibration(str2, str2);
			str.Format(_T("%s [%s]"), str2, UserName);
			SaveOnlineAutoCalibrationLog(str);		
			return true;
		}
	}	
	SetOnlineAutoCalibrationToExec_2DCurrent(true);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcAutoCalibration_2D_CurrentCalibrate()//線上檢測-自動校正-2D電流-校正	
{
	if ( GetOnlineAutoCalibrationToExec_2DCurrent() == false ) { return true; }
	const char fnName[]="CAOIDataCollect::ExecOnlineProcAutoCalibration_2D_CurrentCalibrate";	
	if ( MotionCtrlPtr->WaitForMotionStop() == false )
	{
		SetErrorString(MotionCtrlPtr->GetErrorString());
		return false;
	}
	if ( WaitForTargetCapOnFinish() == false )
	{	return false; }
	int   i=0, j=0, k=0;
	const int MaxCurrent=101;
	const int MaxLEDChannel=LED_CHANNEL_COUNT;
	const bool ResetLight = false;	
	std::vector<TSliceParam> SliceParamList;	
	if ( CloneSystemSliceParamList_No3D(SliceParamList) == false )
	{	return false; }
	const size_t SliceCount=SliceParamList.size();
	if ( 0 == SliceCount )
	{	return true; }	
	
	CString    str;	
	CString    str2;
	CString    strTmp;		
	CString    Folder;
	IMAGE_SIZE ImageW=0;
	IMAGE_SIZE ImageH=0;	
	IMAGE_SIZE ImageStep=0;	
	IMAGE_SIZE ColorStep=0;	
	IMAGE_SIZE BitCount=0;	
	IMAGE_SIZE ColorBitCnt=0;
	IMAGE_PTR  RawImagePtr=NULL;
	IMAGE_PTR  ColorImagePtr=NULL;
	bool bColorCamera=false;
	bool bLedWhiteBalance=false;
	const int   Channel = 3;
	const int   ColorChannel=3;
	const bool  bSaveImage=false;
	std::vector<CString> StrList;
	TImageStat  Statistics[Channel];
	const CAMERA_ID CameraID=PRIMARY_CAMERA_ID;	
	CAMERA_IMAGE_MODE CameraImageMode=GetCameraImageMode(CameraID);	
	const size_t BufferSize=CameraCtrl.GetCameraColorImageSize(CameraID);

	Folder.Format(_T("%s\\%s"), GetAOITempDirectory(), _T("Online2DCurrent"));
	CreateDirectory(Folder, NULL);

	if ( JetMemory.alloc_func(BufferSize, RawImagePtr, fnName, "RawImagePtr") == false || 
		 JetMemory.alloc_func(BufferSize, ColorImagePtr, fnName, "ColorImagePtr") == false )
	{
		JetMemory.free_func(RawImagePtr);
		JetMemory.free_func(ColorImagePtr);
		SetErrorString(JetMemory.GetErrorString());
		return false;
	}	
	if ( CAMERA_IMAGE_BAYER==CameraImageMode || CAMERA_IMAGE_COLOR==CameraImageMode )
	{	bColorCamera  = true;	}
	else
	{	bColorCamera  = false;	}	
	for ( i=0; i<SliceCount; i++ )
	{
		RECT RoiRect;
		bool bFinish = false;
		int  CurrentCur=0, CurrentLast=0;
		float  CurrentGrayCur=0, CurrentGrayLast=0;
		const long TotalFrameImageCount=1;
		std::vector<TSliceParam> ParamList;
		TSliceParam SliceParamBef=SliceParamList[i];
		TSliceParam SliceParamAft=SliceParamList[i];
		const double SliceGainValue=SliceParamBef.SliceGainValue;
		const int SliceTargetGray=(int)(SliceParamBef.SliceTargetGray);		
		LED_CURRENT_CALI_MODE CurrCaliMode=SliceParamBef.SliceLightTable.LEDCurrCaliMode;
		if ( FN_DISABLE == SliceParamBef.SliceEnabled ) { continue; }
		if ( LED_CURRENT_CALI_DISABLE == CurrCaliMode ) { continue; }
		bLedWhiteBalance = false;		
		for ( j=0; j<MaxCurrent; j++ )
		{			
			CurrentCur=j;
			SetSliceParam2DCurrent(SliceParamAft, CurrentCur);

			ParamList.clear();
			ParamList.push_back(SliceParamAft);					
			if ( CameraCtrl.BatchGrabPrepare2(ParamList, true) == false )
			{
				JetMemory.free_func(RawImagePtr);
				JetMemory.free_func(ColorImagePtr);
				SetErrorString(CameraCtrl.GetErrorString());
				return false;
			}			
			if ( CameraCtrl.BatchGrabStart2(bFinish) == false )
			{
				JetMemory.free_func(RawImagePtr);
				JetMemory.free_func(ColorImagePtr);
				SetErrorString(CameraCtrl.GetErrorString());
				return false;
			}			
			if ( CameraCtrl.WaitForCameraImageCallbackCount(CameraID, TotalFrameImageCount) == false )
			{
				JetMemory.free_func(RawImagePtr);
				JetMemory.free_func(ColorImagePtr);
				SetErrorString(CameraCtrl.GetErrorString());
				return false;
			}						
			
			if ( CameraCtrl.GetCameraImage3(CameraID, ImageW, ImageH, ImageStep, BitCount, RawImagePtr) == false )
			{
				JetMemory.free_func(RawImagePtr);
				JetMemory.free_func(ColorImagePtr);
				SetErrorString(CameraCtrl.GetErrorString());				
				return false; 
			}
			
			const int ImageCpX=ImageW/2;
			const int ImageCpY=ImageH/2;
			const int RoiSizeX=ImageW/2;
			const int RoiSizeY=ImageH/2;
			RoiRect.left  = ImageCpX-(RoiSizeX/2);
			RoiRect.top   = ImageCpY-(RoiSizeY/2);
			RoiRect.right = RoiRect.left+RoiSizeX;
			RoiRect.bottom= RoiRect.top+RoiSizeY;
			Statistics[0].m_Rect=RoiRect;
			Statistics[1].m_Rect=RoiRect;
			Statistics[2].m_Rect=RoiRect;
			if ( true == bSaveImage )
			{
				str.Format(_T("%s\\Slice[%02d]_Current[%03d].PNG"), Folder, i+1, j);
				ImageAPI.SaveImage(str, ImageW, ImageH, ImageStep, BitCount, RawImagePtr, true);
			}
				
			if ( ImageAPI.CalcGrayImageStatistics(ImageW, ImageH, ImageStep, RawImagePtr, Statistics[0]) == false )
			{
				JetMemory.free_func(RawImagePtr);
				JetMemory.free_func(ColorImagePtr);
				SetErrorString(ImageAPI.GetImageApiErrorString());		
				return false;
			}	

			CurrentGrayCur=(float)(SliceGainValue*Statistics[0].m_Ave);
			if ( CurrentGrayCur > SliceTargetGray )
			{	
				if ( (CurrentGrayCur-SliceTargetGray) > (SliceTargetGray-CurrentGrayLast) )
				{	
					CurrentCur=CurrentLast;
					CurrentGrayCur=CurrentGrayLast;
					SetSliceParam2DCurrent(SliceParamAft, CurrentLast);	
				}
				break;	
			}				
			CurrentLast=CurrentCur;
			CurrentGrayLast=CurrentGrayCur;		
		}		
		SliceParamList[i]=SliceParamAft;
		GetSliceParam2DCurrentCmpText(SliceParamBef, SliceParamAft, strTmp);
		str.Format(_T("%s %s"), SliceParamAft.SliceName, strTmp);
		StrList.push_back(str);		
	}
	JetMemory.free_func(RawImagePtr);
	JetMemory.free_func(ColorImagePtr);

	const size_t StrCount=StrList.size();
	const TSystemParameter &SysParam=GetSystemParameter();
	FUNC_EXEC_MODE CalibrationMode=SysParam.m_OnlineAutoCalibrationMode_2DCurrent;
	if ( FUNC_EXEC_ASK == CalibrationMode )
	{	
		for ( i=0; i<StrCount; i++ )
		{
			if ( 0 == i )
			{	str = StrList[i]; }
			else
			{
				strTmp=str;
				str.Format(_T("%s\n%s"), strTmp, StrList[i]);
			}
		}
		strTmp=str;
		str2=_T("Do you want to update the parameters?");
		str2=LoadMultiLanguageString_OnlineCalibration(str2, str2);
		str.Format(_T("%s\n%s"), str2, strTmp);

		DWORD Res=0;
		LANE_ID LaneID=LANE_ID_A;		
		PlcCtrlPtr->TurnOnInspectAlarm(LaneID);
		//if ( UserLogin_OnlineCalibration() == false )
		if ( OperateLevelOnlineUnlockCalibration() == false )
		{
			str=GetErrorString();
			SaveOnlineAutoCalibrationLog(str);
			PlcCtrlPtr->TurnOffInspectAlarm(LaneID);
			return false;
		}
		Res=JetAPI::ShowMessageBox(str, MB_YESNOCANCEL);
		PlcCtrlPtr->TurnOffInspectAlarm(LaneID);

		CString UserName=GetCurrentUserName();
		if ( IDCANCEL == Res )
		{
			str2=_T("Error, User Stop Online Calibration 2D Current");
			str2=LoadMultiLanguageString_OnlineCalibration(str2, str2);
			str.Format(_T("%s [%s]"), str2, UserName);
			SaveOnlineAutoCalibrationLog(str);
			SetErrorString(str);
			return false;
		}
		if ( IDNO == Res )
		{	
			str2=_T("Caution, User Cancel Update Online Calibration 2D Current");
			str2=LoadMultiLanguageString_OnlineCalibration(str2, str2);
			str.Format(_T("%s [%s]"), str2, UserName);		
			SaveOnlineAutoCalibrationLog(str);			
			return true;
		}		
	}
	if ( UpdateSystemSliceParamList(SliceParamList) == false )
	{
		str2=_T("Error, Calibration 2D Current Update Param Fault");
		str2=LoadMultiLanguageString_OnlineCalibration(str2, str2);
		SaveOnlineAutoCalibrationLog(str2);
		return false;	
	}
	if ( SaveSystemSliceParamINI() == false )
	{
		str2=_T("Error, Calibration 2D Current Save File Fault");
		str2=LoadMultiLanguageString_OnlineCalibration(str2, str2);
		SaveOnlineAutoCalibrationLog(str2);
		return false;	
	}
	str2=_T("Calibration 2D Current");
	str2=LoadMultiLanguageString_OnlineCalibration(str2, str2);
	for ( i=0; i<StrCount; i++ )
	{	
		str.Format(_T("%s-%s"), str2, StrList[i]);
		SaveOnlineAutoCalibrationLog(str);
	}
	str2=_T("Calibration 2D Current Finish");
	str2=LoadMultiLanguageString_OnlineCalibration(str2, str2);
	SaveOnlineAutoCalibrationLog(str2);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcAutoCalibration_3D_Current(TOnlineProcParam &Param)//線上檢測-自動校正-3D電流
{		
	if ( ExecOnlineProcAutoCalibration_3D_CurrentMoveToTarget() == false )
	{	return false; }			
	SaveOnlineAutoCalibrationLog(_T("Calibration 3D Current Finish"));
	
	SetOnlineAutoCalibrationDateTime_3D_Current();
	SaveOnlineAutoCalibrationDateTime_3D_Current();	
	ExecOnlineProcAutoCalibrationFinish(Param);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcAutoCalibration_3D_CurrentMoveToTarget()//線上檢測-自動校正-3D電流-移動至塊規上
{
	TPOINT3D StagePos;	
	const TCalibrationParameter &CaliParam=GetCalibrationParameter();
	StagePos.x = CaliParam.m_TargetWhitePosX;
	StagePos.y = CaliParam.m_TargetWhitePosY;
	StagePos.z = CaliParam.m_TargetWhitePosZ;	
	if ( MotionCtrlPtr->XYZMoveTo(StagePos.x, StagePos.y, StagePos.z) == false )
	{
		SetErrorString(MotionCtrlPtr->GetErrorString());
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcAutoCalibration_3D_VerifyFn()//線上檢測-自動校正-3D驗證
{	
	const int DlpLedColor=GetSystemDlpLedColor();	
	if ( ExecOnlineProcAutoCalibration_3D_VerifyMoveToTarget() == false )
	{	return false; }	
	if ( ExecOnlineProcAutoCalibration_3D_VerifyGrabImage() == false )
	{	return false;	}
	if ( ExecOnlineProcAutoCalibration_3D_VerifyWaitForGrabDone() == false )
	{	return false;	}
	SetSystemDlpLedColor(DlpLedColor);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcAutoCalibration_3D_VerifyMoveToTarget()//線上檢測-自動校正-3D驗證-移動至塊規上
{	
	if ( GetDisable3D() == true ) { return true; }

	TPOINT3D StagePos;	
	const TCalibrationParameter &CaliParam=GetCalibrationParameter();
	StagePos.x = CaliParam.m_TargetWhitePosX;
	StagePos.y = CaliParam.m_TargetWhitePosY;
	StagePos.z = CaliParam.m_TargetWhitePosZ;	
	if ( MotionCtrlPtr->XYZMoveTo(StagePos.x, StagePos.y, StagePos.z) == false )
	{
		SetErrorString(MotionCtrlPtr->GetErrorString());
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcAutoCalibration_3D_VerifyGrabImage()//線上檢測-自動校正-3D驗證-取像
{		
	if ( GetDisable3D() == true ) { return true; }

	const bool ResetLight = false;	
	std::vector<TFrameParam> FrameParamList;
	if ( CloneSystemFrameParamList_Only3D(FrameParamList) == false )
	{	return false; }
	const size_t FrameCount=FrameParamList.size();
	if ( 0 == FrameCount )
	{	return true; }
	
	int   DLPLEDColorUsed=DLP_LED_COLOR_NO;
	const CAMERA_ID CameraID=PRIMARY_CAMERA_ID;
	const int DLPLEDColor = GetOnlineAutoCalibrationDLPLedColor();
	const int DLPLEDColor_ORG = GetSystemDlpLedColor();	

	CameraCtrl.ClearAllCameraCount();//復歸所有相機次數			
	if ( DLP_LED_COLOR_WHITE == DLPLEDColor_ORG )
	{	DLPLEDColorUsed = DLPLEDColor;	}
	else
	{	DLPLEDColorUsed = DLPLEDColor_ORG;	}	
	SetSystemDlpLedColor(DLPLEDColorUsed);
	Light3DCtrl.SetAllLight3DLEDColor(DLPLEDColorUsed);
	SetOnlineAutoCalibrationDLPLedColorUsed(DLPLEDColorUsed);	
	const long RingListSize  = CameraCtrl.GetCameraRingBufferListSize(CameraID);	
	//以下時間花費較久, 所以等待時間往後延
	if ( ExecPrepareFrameImageSetting(FrameParamList, ResetLight) == false )
	{	return false;	}
	if ( MotionCtrlPtr->WaitForMotionStop() == false )
	{
		SetErrorString(MotionCtrlPtr->GetErrorString());
		return false;
	}
	if ( WaitForTargetCapOnFinish() == false )
	{	return false; }

	if ( ExecGrabNextUniFrameImage() == false )
	{	return false;	}
	SetIsNeedResetLightCtrlDLP(true);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcAutoCalibration_3D_VerifyWaitForGrabDone()//線上檢測-自動校正-3D驗證-等待取像結束
{	
	if ( GetDisable3D() == true ) { return true; }

	std::vector<TFrameParam> FrameParamList;
	std::vector<TSliceParam> SliceParamList;
	GetGrabSliceParamList(SliceParamList);
	GetGrabFrameParamList(FrameParamList);
	const size_t FrameCount=FrameParamList.size();
	const size_t SliceCount=SliceParamList.size();
	if ( 0==SliceCount || 0==FrameCount ) { return true; }

	size_t  i=0;
	CString str;
	long FrameImageCount=0;
	const CAMERA_ID CameraID=PRIMARY_CAMERA_ID;
	for ( i=0; i<FrameCount; i++ )
	{
		const TFrameParam &FrameParamRef=FrameParamList[i];
		FrameImageCount += FrameParamRef.FrameImageCount;
	}	
	
	bool bGetFrame_3D=false;
	bool bGetSlice_3D=false;
	for ( i=0; i<FrameCount; i++ )
	{
		const TFrameParam &FrameParamRef=FrameParamList[i];
		if ( FRAME_UNIQUE_ID_DLP != FrameParamRef.FrameUniqueID ) { continue; }
		bGetFrame_3D = true;
		break;
	}	
	const size_t FrameIndex3D = i;

	for ( i=0; i<SliceCount; i++ )
	{
		const TSliceParam &SliceParamRef=SliceParamList[i];
		if ( SLICE_UNIQUE_ID_DLP != SliceParamRef.SliceUniqueID ) { continue; }
		bGetSlice_3D = true;
		break;
	}	
	const size_t SliceIndex3D = i;

	const long TotalFrameImageCount=FrameImageCount;
	if ( CameraCtrl.WaitForCameraImageCallbackCount(CameraID, TotalFrameImageCount) == false )
	{
		SetErrorString(CameraCtrl.GetErrorString());
		return false;
	}

	bool bUse3D=true;
	if ( false==bGetFrame_3D || FrameIndex3D>=FrameCount ) { bUse3D=false; }
	if ( false==bGetSlice_3D || SliceIndex3D>=SliceCount ) { bUse3D=false; }
	if ( false == bUse3D )
	{
		str = _T("Error, ExecOnlineProcAutoCalibration_3D_VerifyWaitForGrabDone Fault [No Use 3D]");
		str=LoadMultiLanguageString_OnlineCalibration(str, str);
		SetErrorString(str);
		return false; 
	}
	
	const TFrameParam &FrameParamRef=FrameParamList[FrameIndex3D];
	const TSliceParam &SliceParamRef=SliceParamList[SliceIndex3D];	
	if ( ExecOnlineProcAutoCalibration_3D_VerifyPlaneCalculate(FrameParamRef, SliceParamRef) == false )
	{	return false; }		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcAutoCalibration_3D_VerifyPlaneCalculate(const TFrameParam &FrameParam, const TSliceParam &SliceParam)//線上檢測-自動校正-3D驗證
{
	if ( GetDisable3D() == true ) { return true; }
	const char fnName[]="CAOIDataCollect::ExecOnlineProcAutoCalibration_3D_VerifyPlaneCalculate";

	CString str;
	CString str2;
	long FrameImageCount=0;
	const LANE_ID LaneID=LANE_ID_A;
	const CAMERA_ID CameraID=PRIMARY_CAMERA_ID;	
	const TSystemParameter &SysParam=GetSystemParameter();
	SLICE_FUNC_MODE SliceFuncMode=SliceParam.SliceFuncMode;			
	const long TotalFrameImageCount=FrameParam.FrameImageCount;	
	const int  FrameExpCount = CheckFrameExpCountBySliceFuncMode(SliceFuncMode);
	const int  FrameLightCount = CheckFrameLightCountBySliceFuncMode(SliceFuncMode);
	const bool bReSortCameraImage = CheckReSortCameraImage(SliceFuncMode);
	const bool bSepareDLP2ExpTable = CheckUseSeparateDLP2ExpTable(SliceFuncMode);
	FUNC_EXEC_MODE CalibrationMode=SysParam.m_OnlineAutoCalibrationMode_3DZeroPlane;

	long  idx=0;
	const long ImageCount = CameraCtrl.GetImageCallbackCount(CameraID);		
	const long RingIndex = CameraCtrl.GetCameraRingBufferCurrentIndex(CameraID);
	const long RingListSize  = CameraCtrl.GetCameraRingBufferListSize(CameraID);	

	size_t  i=0, j=0;
	const size_t MaxImageCount=128;
	const size_t MaxDLPCount = DLP_CAST_COUNT;	
	TCastParam CastParam[MaxDLPCount];
	IMAGE_PTR  ImagePtr[MaxImageCount];		
	IMAGE_SIZE ImageW[MaxImageCount]={0};
	IMAGE_SIZE ImageH[MaxImageCount]={0};
	IMAGE_SIZE ImageStep[MaxImageCount]={0};
	if ( MaxImageCount < TotalFrameImageCount )
	{
		str.Format(_T("Error, ExecOnlineProcAutoCalibration_3D_VerifyPlaneCalculate Buffer Count Exception"));
		SetErrorString(str);
		return false;
	}

	for ( i=0; i<MaxImageCount; i++ )
	{
		ImagePtr[i] = NULL;		
		ImageW[i]=0;
		ImageH[i]=0;
		ImageStep[i]=0;
	}

	idx = RingIndex;
	idx = idx-ImageCount;
	//idx = idx+TotalFrameImageCount;
	if ( idx < 0 ) { idx += RingListSize;  }	

	if ( true==bReSortCameraImage && 1==FrameLightCount )
	{	
		if ( CameraCtrl.ReSortCameraRingBufferImage(CameraID, SliceParam) == false )
		{
			SetErrorString(CameraCtrl.GetErrorString());
			return false; 
		}
	}

	for ( i=0; i<TotalFrameImageCount; i++ )
	{
		if ( CameraCtrl.GetCameraRingBufferImage(CameraID, idx, ImageW[i], ImageH[i], ImageStep[i], ImagePtr[i]) == false )
		{
			SetErrorString(CameraCtrl.GetErrorString());
			return false; 
		}
		idx ++;
	}
	if ( BuildCastParamArray_Calibration(TotalFrameImageCount, ImageW, ImageH, ImageStep, ImagePtr, SliceParam, CastParam) == false )
	{	return false;	}
	

	const bool bSaveImage=false;
	if ( true == bSaveImage )
	{
		str.Format(_T("%s\\%s"), GetAOITempDirectory(), _T("OnlineCalibrationRaw"));
		ImageAPI.SaveCastParamImage(str, CastParam, MaxDLPCount);
	}

	bool IsOK = true;	
	TPhaseNoiseParam NoiseParam;	
	const bool bOpenMP = true;
	const IMAGE_SIZE ImgW=ImageW[0];
	const IMAGE_SIZE ImgH=ImageH[0];
	const IMAGE_SIZE ImgStep=ImageStep[0];	
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImgStep, ImgH);
	const bool bUse2Exp = CheckSliceFuncModeDlpUse2Exp(SliceFuncMode);
	const double GapRatio = SysParam.m_SpaceVerifyMultiCastGapRatio;
	const double GapThreshold = SysParam.m_SpaceVerifyMultiCastGapThreshold;		

	//取得相位雜訊定義參數	
	GetPhaseNoiseDefineParam(NoiseParam);

	for ( j=0; j<MaxDLPCount; j++ )
	{
		const size_t CastIdx=j;
		if ( NULL == CastParam[CastIdx].PtrA1 ) { continue; }
		if ( NULL == CastParam[CastIdx].PtrMask ) { continue; }
		if ( NULL == CastParam[CastIdx].PtrSpace ) { continue; }
		if ( false == bUse2Exp || 1!=FrameLightCount )
		{	IsOK = ImageAPI.PatternCast1ToSpace3(ImgW, ImgH, ImgStep, CastParam[CastIdx], NoiseParam, bOpenMP, CastParam[CastIdx].PtrMask, CastParam[CastIdx].PtrSpace);	}
		else
		{	IsOK = ImageAPI.PatternCast1ToSpace2Exp3(ImgW, ImgH, ImgStep, CastParam[CastIdx], NoiseParam, bOpenMP, CastParam[CastIdx].PtrMask, CastParam[CastIdx].PtrSpace);	}

		if ( false == IsOK )
		{
			SetErrorString(ImageAPI.GetImageApiErrorString());
			for ( i=0; i<MaxDLPCount; i++ )
			{	
				if ( NULL != CastParam[i].PtrMask )
				{	JetMemory.free_func(CastParam[i].PtrMask); }
				CastParam[i].PtrMask = NULL;

				if ( NULL != CastParam[i].PtrSpace )
				{	JetMemory.free_func(CastParam[i].PtrSpace); }
				CastParam[i].PtrSpace = NULL;
			}
			return false;
		}		
	}
	
	int Cnt=0;
	for ( i=0; i<BufferSize; i++ )
	{
		float Val=0;
		float Min= FLT_MAX;
		float Max=-FLT_MAX;
		for ( j=0; j<MaxDLPCount; j++ )
		{
			const size_t CastIdx=j;
			if ( NULL == CastParam[CastIdx].PtrA1 ) { continue; }
			if ( NULL == CastParam[CastIdx].PtrMask ) { continue; }
			if ( NULL == CastParam[CastIdx].PtrSpace ) { continue; }

			Val = CastParam[CastIdx].PtrSpace[i];
			if ( Min > Val ) { Min = Val; }
			if ( Max < Val ) { Max = Val; }
		}
		float Range=Max-Min;
		if ( Range < GapThreshold ) { continue; }
		Cnt ++;
	}

	//先釋放不用的記憶體
	for ( i=0; i<MaxDLPCount; i++ )
	{	
		if ( NULL != CastParam[i].PtrMask )
		{	JetMemory.free_func(CastParam[i].PtrMask); }
		CastParam[i].PtrMask = NULL;

		if ( NULL != CastParam[i].PtrSpace )
		{	JetMemory.free_func(CastParam[i].PtrSpace); }
		CastParam[i].PtrSpace = NULL;
	}


	bool bPass=false;
	CString strResult;
	CString strBigSmal;
	CString strVerify;
	double Ratio=Cnt*100.0;	
	Ratio /= BufferSize;
	if ( Ratio < GapRatio )
	{	
		bPass = true;	
		strBigSmal= _T("<");
		strResult = _T("Pass");
	}
	else
	{	
		bPass = false;	
		strBigSmal= _T(">");
		strResult = _T("Fail");
	}
	str2=_T("3D Verify");
	str2=LoadMultiLanguageString_OnlineCalibration(str2, str2);
	strVerify.Format(_T("%.2f %% [%s %.2f %%] out of range %.2f um => %s"), Ratio, strBigSmal, GapRatio, GapThreshold, strResult);	
	str.Format(_T("%s %s"), str2, strVerify);	
	SaveOnlineAutoCalibrationLog(str);

	if ( true == bPass )
	{	return true;	}
	
	ONLINE_STATE_MODE OnlineStateMode=GetOnlineStateMode_Lane(LaneID);
	if ( FUNC_EXEC_ASK == CalibrationMode )
	{
		DWORD Res=0;		
		PlcCtrlPtr->TurnOnInspectAlarm(LaneID);		
		//if ( UserLogin_OnlineCalibration() == false )
		if ( OperateLevelOnlineUnlockCalibration() == false )
		{
			str=GetErrorString();
			SaveOnlineAutoCalibrationLog(str);
			PlcCtrlPtr->TurnOffInspectAlarm(LaneID);
			return false;
		}
		CString UserName=GetCurrentUserName();
		str2 = _T("Do you want to Calibrate 3D Paremeter ?");
		switch ( OnlineStateMode )
		{
		case ONLINE_STATE_AUTO_CALIBRATION_3D_ZERO_PLANE: str2 = _T("Do you want to Calibrate 3D Zero Plane ?"); break;
		case ONLINE_STATE_AUTO_CALIBRATION_3D_HEIGHT_FACTOR: str2 = _T("Do you want to Calibrate 3D Height Factor ?"); break;
		}
		str2=LoadMultiLanguageString_OnlineCalibration(str2, str2);
		str.Format(_T("%s\n%s"), str2, strVerify);
		Res = JetAPI::ShowMessageBox(str, MB_YESNOCANCEL);		
		PlcCtrlPtr->TurnOffInspectAlarm(LaneID);
		if ( IDCANCEL == Res )
		{
			str2=_T("Error, User Stop Online Calibration 3D Paremeter");
			switch ( OnlineStateMode )
			{
			case ONLINE_STATE_AUTO_CALIBRATION_3D_ZERO_PLANE: str2=_T("Error, User Stop Online Calibration 3D Zero Plane"); break;
			case ONLINE_STATE_AUTO_CALIBRATION_3D_HEIGHT_FACTOR: str2=_T("Error, User Stop Online Calibration 3D Height Factor");; break;
			}
			str2=LoadMultiLanguageString_OnlineCalibration(str2, str2);
			str.Format(_T("%s [%s]"), str2, UserName);			
			SetErrorString(str);
			SaveOnlineAutoCalibrationLog(str);			
			return false;
		}
		if ( IDNO == Res)
		{	
			str2=_T("Caution, User Cancel Online Calibration 3D Paremeter");
			switch ( OnlineStateMode )
			{
			case ONLINE_STATE_AUTO_CALIBRATION_3D_ZERO_PLANE: str2=_T("Caution, User Cancel Online Calibration 3D Zero Plane"); break;
			case ONLINE_STATE_AUTO_CALIBRATION_3D_HEIGHT_FACTOR: str2=_T("Caution, User Cancel Online Calibration 3D Height Factor"); break;
			}
			str2=LoadMultiLanguageString_OnlineCalibration(str2, str2);
			str.Format(_T("%s [%s]"), str2, UserName);
			SaveOnlineAutoCalibrationLog(str);			
			return true; 
		}
	}
	SetOnlineAutoCalibrationToExec_3DZeroPlane(true);	
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcAutoCalibration_3D_ZeroPlane(TOnlineProcParam &Param)//線上檢測-自動校正-相平面
{
	bool IsOK = true;
	const bool bWait=false;
	const THREAD_GRAB_MODE ThreadGrabMode=GetThreadGrabMode();
	if ( ExecTargetCapOnProc(bWait) == false )
	{	return false; }
	SetThreadGrabMode(THREAD_GRAB_ONLINE_CALIBRATION);	
	IsOK = ExecOnlineProcAutoCalibration_3D_ZeroPlaneFn(Param);	
	UserLogout_Check();
	SetThreadGrabMode(ThreadGrabMode);	
	if ( ExecTargetCapOffProc(bWait) == false )
	{	return false; }
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcAutoCalibration_3D_ZeroPlaneFn(TOnlineProcParam &Param)//線上檢測-自動校正-3D相平面
{	
	const int DLPLEDColor=GetSystemDlpLedColor();

	SetOnlineAutoCalibrationToExec_3DZeroPlane(false);	
	if ( ExecOnlineProcAutoCalibration_3D_VerifyFn() == false )
	{	return false; }
	if ( ExecOnlineProcAutoCalibration_3D_ZeroPlaneMoveToTarget() == false )
	{	return false; }
	if ( ExecOnlineProcAutoCalibration_3D_ZeroPlaneGrabImage() == false )
	{	return false;	}
	if ( ExecOnlineProcAutoCalibration_3D_ZeroPlaneWaitForGrabDone() == false )
	{	return false;	}

	SetOnlineAutoCalibrationDateTime_3D_ZeroPlane();
	SaveOnlineAutoCalibrationDateTime_3D_ZeroPlane();

	SetSystemDlpLedColor(DLPLEDColor);	
	Light3DCtrl.SetAllLight3DLEDColor(DLPLEDColor);		

	ExecOnlineProcAutoCalibrationFinish(Param);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcAutoCalibration_3D_ZeroPlaneMoveToTarget()//線上檢測-自動校正-3D相平面-移動至塊規上
{		
	if ( GetDisable3D() == true ) { return true; }

	TPOINT3D StagePos;	
	const TCalibrationParameter &CaliParam=GetCalibrationParameter();
	if ( GetOnlineAutoCalibrationToExec_3DZeroPlane() == false ) { return true; }
	StagePos.x = CaliParam.m_TargetWhitePosX;
	StagePos.y = CaliParam.m_TargetWhitePosY;
	StagePos.z = CaliParam.m_TargetWhitePosZ;
	StagePos.z += CaliParam.m_PhaseZeroPlaneOffsetPosZ;
	if ( MotionCtrlPtr->XYZMoveTo(StagePos.x, StagePos.y, StagePos.z) == false )
	{
		SetErrorString(MotionCtrlPtr->GetErrorString());
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcAutoCalibration_3D_ZeroPlaneGrabImage()//線上檢測-自動校正-3D相平面-取像
{	
	if ( GetDisable3D() == true ) { return true; }
	if ( GetOnlineAutoCalibrationToExec_3DZeroPlane() == false ) { return true; }
	if ( MotionCtrlPtr->WaitForMotionStop() == false )
	{
		SetErrorString(MotionCtrlPtr->GetErrorString());
		return false;
	}
	if ( WaitForTargetCapOnFinish() == false )
	{	return false; }
	const bool ResetLight = false;	
	std::vector<TFrameParam> FrameParamList;
	if ( CloneSystemFrameParamList_Only3D(FrameParamList) == false )
	{	return false; }
	const size_t FrameCount=FrameParamList.size();
	if ( 0 == FrameCount )
	{	return true; }	
	
	bool  CopyToWhite=false;
	int   DLPLEDColorUsed=DLP_LED_COLOR_NO;
	const CAMERA_ID CameraID=PRIMARY_CAMERA_ID;
	const int DLPLEDColor = GetOnlineAutoCalibrationDLPLedColor();
	const int DLPLEDColor_ORG = GetSystemDlpLedColor();	

	CameraCtrl.ClearAllCameraCount();//復歸所有相機次數			
	if ( DLP_LED_COLOR_WHITE == DLPLEDColor_ORG )
	{	
		CopyToWhite = true;
		DLPLEDColorUsed = DLPLEDColor;
	}
	else
	{		
		CopyToWhite = false;
		DLPLEDColorUsed = DLPLEDColor_ORG;
	}	
	SetSystemDlpLedColor(DLPLEDColorUsed);
	Light3DCtrl.SetAllLight3DLEDColor(DLPLEDColorUsed);
	SetOnlineAutoCalibrationDLPLedColorUsed(DLPLEDColorUsed);
	SetOnlineAutoCalibrationDLPZeroPlaneCopyToWhite(CopyToWhite);
	const long RingListSize  = CameraCtrl.GetCameraRingBufferListSize(CameraID);	
	if ( ExecPrepareFrameImageSetting(FrameParamList, ResetLight) == false )
	{	return false;	}
	if ( ExecGrabNextUniFrameImage() == false )
	{	return false;	}
	SetIsNeedResetLightCtrlDLP(true);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcAutoCalibration_3D_ZeroPlaneWaitForGrabDone()//線上檢測-自動校正-3D相平面-等待取像結束
{	
	if ( GetDisable3D() == true ) { return true; }
	if ( GetOnlineAutoCalibrationToExec_3DZeroPlane() == false ) { return true; }

	std::vector<TFrameParam> FrameParamList;
	std::vector<TSliceParam> SliceParamList;
	GetGrabSliceParamList(SliceParamList);
	GetGrabFrameParamList(FrameParamList);
	const size_t FrameCount=FrameParamList.size();
	const size_t SliceCount=SliceParamList.size();
	if ( 0==SliceCount || 0==FrameCount ) { return true; }

	size_t  i=0;
	CString str;
	long FrameImageCount=0;
	const CAMERA_ID CameraID=PRIMARY_CAMERA_ID;
	for ( i=0; i<FrameCount; i++ )
	{
		const TFrameParam &FrameParamRef=FrameParamList[i];
		FrameImageCount += FrameParamRef.FrameImageCount;
	}	
	
	bool bGetFrame_3D=false;
	bool bGetSlice_3D=false;
	for ( i=0; i<FrameCount; i++ )
	{
		const TFrameParam &FrameParamRef=FrameParamList[i];
		if ( FRAME_UNIQUE_ID_DLP != FrameParamRef.FrameUniqueID ) { continue; }
		bGetFrame_3D = true;
		break;
	}	
	const size_t FrameIndex3D = i;

	for ( i=0; i<SliceCount; i++ )
	{
		const TSliceParam &SliceParamRef=SliceParamList[i];
		if ( SLICE_UNIQUE_ID_DLP != SliceParamRef.SliceUniqueID ) { continue; }
		bGetSlice_3D = true;
		break;
	}	
	const size_t SliceIndex3D = i;

	const long TotalFrameImageCount=FrameImageCount;
	if ( CameraCtrl.WaitForCameraImageCallbackCount(CameraID, TotalFrameImageCount) == false )
	{
		SetErrorString(CameraCtrl.GetErrorString());
		return false;
	}

	bool bUse3D=true;
	if ( false==bGetFrame_3D || FrameIndex3D>=FrameCount ) { bUse3D=false; }
	if ( false==bGetSlice_3D || SliceIndex3D>=SliceCount ) { bUse3D=false; }
	if ( false == bUse3D )
	{
		str = _T("Error, ExecOnlineProcAutoCalibration_3D_ZeroPlaneWaitForGrabDone Fault [No Use 3D]");
		str=LoadMultiLanguageString_OnlineCalibration(str, str);
		SetErrorString(str);
		return false; 
	}
	
	const TFrameParam &FrameParamRef=FrameParamList[FrameIndex3D];
	const TSliceParam &SliceParamRef=SliceParamList[SliceIndex3D];	
	if ( ExecOnlineProcAutoCalibration_3D_ZeroPlaneCalculate(FrameParamRef, SliceParamRef) == false )
	{	return false; }		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcAutoCalibration_3D_ZeroPlaneCalculate(const TFrameParam &FrameParam, const TSliceParam &SliceParam)//線上檢測-自動校正-相平面
{	
	if ( GetDisable3D() == true ) { return true; }
	const char fnName[]="CAOIDataCollect::ExecOnlineProcAutoCalibration_3D_ZeroPlaneCalculate";

	CString str;
	CString str2;
	long FrameImageCount=0;
	const LANE_ID LaneID=LANE_ID_A;
	const CAMERA_ID CameraID=PRIMARY_CAMERA_ID;	
	const TSystemParameter &SysParam=GetSystemParameter();
	const TCalibrationParameter &CaliParam=GetCalibrationParameter();	
	SLICE_FUNC_MODE SliceFuncMode=SliceParam.SliceFuncMode;			
	const long TotalFrameImageCount=FrameParam.FrameImageCount;
	const int  FrameExpCount = CheckFrameExpCountBySliceFuncMode(SliceFuncMode);
	const int  FrameLightCount = CheckFrameLightCountBySliceFuncMode(SliceFuncMode);
	const bool bReSortCameraImage = CheckReSortCameraImage(SliceFuncMode);
	const bool bSepareDLP2ExpTable = CheckUseSeparateDLP2ExpTable(SliceFuncMode);
	FUNC_EXEC_MODE CalibrationMode=SysParam.m_OnlineAutoCalibrationMode_3DZeroPlane;

	long  idx=0;
	const long ImageCount = CameraCtrl.GetImageCallbackCount(CameraID);		
	const long RingIndex = CameraCtrl.GetCameraRingBufferCurrentIndex(CameraID);
	const long RingListSize  = CameraCtrl.GetCameraRingBufferListSize(CameraID);	

	size_t  i=0, j=0;
	const size_t MaxImageCount=128;
	const size_t MaxDLPCount = DLP_CAST_COUNT;	
	TCastParam CastParam[MaxDLPCount];
	IMAGE_PTR  ImagePtr[MaxImageCount];		
	IMAGE_SIZE ImageW[MaxImageCount]={0};
	IMAGE_SIZE ImageH[MaxImageCount]={0};
	IMAGE_SIZE ImageStep[MaxImageCount]={0};
	if ( MaxImageCount < TotalFrameImageCount )
	{
		str.Format(_T("Error, ExecOnlineProcAutoCalibration_Calibration Buffer Count Exception"));
		SetErrorString(str);
		return false;
	}

	for ( i=0; i<MaxImageCount; i++ )
	{
		ImagePtr[i] = NULL;		
		ImageW[i]=0;
		ImageH[i]=0;
		ImageStep[i]=0;
	}

	idx = RingIndex;
	idx = idx-ImageCount;
	//idx = idx+TotalFrameImageCount;
	if ( idx < 0 ) { idx += RingListSize;  }

	if ( true==bReSortCameraImage && 1==FrameLightCount )
	{	
		if ( CameraCtrl.ReSortCameraRingBufferImage(CameraID, SliceParam) == false )
		{
			SetErrorString(CameraCtrl.GetErrorString());
			return false; 
		}
	}

	for ( i=0; i<TotalFrameImageCount; i++ )
	{
		if ( CameraCtrl.GetCameraRingBufferImage(CameraID, idx, ImageW[i], ImageH[i], ImageStep[i], ImagePtr[i]) == false )
		{
			SetErrorString(CameraCtrl.GetErrorString());
			return false; 
		}
		idx ++;
	}
	if ( BuildCastParamArray_Calibration(TotalFrameImageCount, ImageW, ImageH, ImageStep, ImagePtr, SliceParam, CastParam) == false )
	{	return false;	}

	const bool bSaveImage=false;
	if ( true == bSaveImage )
	{
		str.Format(_T("%s\\%s"), GetAOITempDirectory(), _T("OnlineCalibrationRaw"));
		ImageAPI.SaveCastParamImage(str, CastParam, MaxDLPCount);
	}

	bool IsOK = true;
	IMAGE_PTR Ptr1[8];
	IMAGE_PTR Ptr2[8];
	IMAGE_PTR Ptr3[8];
	IMAGE_PTR Ptr4[8];		
	IMAGE_PTR MaskPtr=NULL;
	PHASE_PTR PhasePtr=NULL;
	PHASE_PTR ZeroPhasePtr=NULL;	
	const IMAGE_SIZE ImgW=ImageW[0];
	const IMAGE_SIZE ImgH=ImageH[0];
	const IMAGE_SIZE ImgStep=ImageStep[0];	
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImgStep, ImgH);

	TCastParam CastParamObj;		
	TPhaseNoiseParam NoiseParam;
	int        ExpTime1=0, ExpTime2=0;
	double     Per1=8, Per2=12, PerN=0, Gamma=0;		
	const int DLPLEDColor =GetOnlineAutoCalibrationDLPLedColorUsed();
	const bool CopyToWhite=GetOnlineAutoCalibrationDLPZeroPlaneCopyToWhite();	
	
	//先釋放不用的記憶體
	for ( i=0; i<MaxDLPCount; i++ )
	{	
		if ( NULL != CastParam[i].PtrMask )
		{	JetMemory.free_func(CastParam[i].PtrMask); }
		CastParam[i].PtrMask = NULL;

		if ( NULL != CastParam[i].PtrSpace )
		{	JetMemory.free_func(CastParam[i].PtrSpace); }
		CastParam[i].PtrSpace = NULL;
	}

	if ( JetMemory.alloc_func(BufferSize, MaskPtr, fnName, "MaskPtr") == false || 
		 JetMemory.alloc_func(BufferSize, PhasePtr, fnName, "PhasePtr") == false )
	{
		JetMemory.free_func(MaskPtr);
		JetMemory.free_func(PhasePtr);		
		return false;
	}
	
	//取得相位雜訊定義參數	
	GetPhaseNoiseDefineParam(NoiseParam);
	for ( i=0; i<MaxDLPCount; i++ )
	{		
		IsOK = true;
		CastParamObj=CastParam[i];
		Per1 = CastParamObj.PerA;
		Per2 = CastParamObj.PerB;
		Gamma = CastParamObj.Gamma;		
		ExpTime1 = CastParamObj.ExpTimeA;
		ExpTime2 = CastParamObj.ExpTimeB;		
		::memset(Ptr1, 0x00, sizeof(Ptr1));
		::memset(Ptr2, 0x00, sizeof(Ptr2));
		::memset(Ptr3, 0x00, sizeof(Ptr3));
		::memset(Ptr4, 0x00, sizeof(Ptr4));		
		if ( SLICE_FUNC_3D_2STEP_2STEP_1EXP == SliceFuncMode )
		{
			Ptr1[0]=CastParamObj.PtrA1;	Ptr1[1]=CastParamObj.PtrA2;
			Ptr2[0]=CastParamObj.PtrB1;	Ptr2[1]=CastParamObj.PtrB2;	Ptr2[2]=CastParamObj.PtrB3;
			if ( ImageAPI.GrayImage221FrameTo2Phase3(ImgW, ImgH, ImgStep, Ptr1[0], Ptr1[1], Per1, ExpTime1, Ptr2[0], Ptr2[1], Ptr2[2], Per2, ExpTime2, Gamma, ZeroPhasePtr, NoiseParam, MaskPtr, PhasePtr, PerN) == false )		
			{
				IsOK = false;
				break;
			}		
		}
		else if ( SLICE_FUNC_3D_4STEP_2STEP_1EXP==SliceFuncMode || SLICE_FUNC_3D_4STEP_2STEP_2EXP==SliceFuncMode )
		{
			Ptr1[0]=CastParamObj.PtrA1;	Ptr1[1]=CastParamObj.PtrA2; Ptr1[2]=CastParamObj.PtrA3;	Ptr1[3]=CastParamObj.PtrA4;
			Ptr2[0]=CastParamObj.PtrB1;	Ptr2[1]=CastParamObj.PtrB2;
			if ( ImageAPI.GrayImage42FrameTo2Phase3(ImgW, ImgH, ImgStep, Ptr1[0], Ptr1[1], Ptr1[3], Ptr1[4], Per1, ExpTime1, Ptr2[0], Ptr2[1], Per2, ExpTime2, Gamma, ZeroPhasePtr, NoiseParam, MaskPtr, PhasePtr, PerN) == false )		
			{
				IsOK = false;
				break;
			}		
		}
		else if ( SLICE_FUNC_3D_4STEP_4GC_1EXP==SliceFuncMode || SLICE_FUNC_3D_4STEP_4GC_2LIGHT==SliceFuncMode || SLICE_FUNC_3D_4STEP_4GC_2EXP==SliceFuncMode )
		{
			Ptr1[0]=CastParamObj.PtrA1;	Ptr1[1]=CastParamObj.PtrA2; Ptr1[2]=CastParamObj.PtrA3;	Ptr1[3]=CastParamObj.PtrA4;
			Ptr2[0]=CastParamObj.PtrB1;	Ptr2[1]=CastParamObj.PtrB2;	Ptr2[2]=CastParamObj.PtrB3;	Ptr2[3]=CastParamObj.PtrB4;
			if ( ImageAPI.GrayImage44GCFrameTo2Phase3(ImgW, ImgH, ImgStep, Ptr1[0], Ptr1[1], Ptr1[2], Ptr1[3], Per1, ExpTime1, Ptr2[0], Ptr2[1], Ptr2[2], Ptr2[3], Per2, ExpTime2, Gamma, ZeroPhasePtr, NoiseParam, MaskPtr, PhasePtr, PerN) == false )		
			{
				IsOK = false;
				break;
			}		
		}
		else if ( SLICE_FUNC_3D_4STEP_5GC_1EXP==SliceFuncMode || SLICE_FUNC_3D_4STEP_5GC_2LIGHT==SliceFuncMode || SLICE_FUNC_3D_4STEP_5GC_2EXP==SliceFuncMode )
		{
			Ptr1[0]=CastParamObj.PtrA1;	Ptr1[1]=CastParamObj.PtrA2; Ptr1[2]=CastParamObj.PtrA3;	Ptr1[3]=CastParamObj.PtrA4;
			Ptr2[0]=CastParamObj.PtrB1;	Ptr2[1]=CastParamObj.PtrB2;	Ptr2[2]=CastParamObj.PtrB3;	Ptr2[3]=CastParamObj.PtrB4;	Ptr2[4]=CastParamObj.PtrB5;
			if ( ImageAPI.GrayImage45GCFrameTo2Phase3(ImgW, ImgH, ImgStep, Ptr1[0], Ptr1[1], Ptr1[2], Ptr1[3], Per1, ExpTime1, Ptr2[0], Ptr2[1], Ptr2[2], Ptr2[3], Ptr2[4], Per2, ExpTime2, Gamma, ZeroPhasePtr, NoiseParam, MaskPtr, PhasePtr, PerN) == false )
			{
				IsOK = false;
				break;
			}		
		}
		else if ( SLICE_FUNC_3D_4STEP_6GC_1EXP==SliceFuncMode || SLICE_FUNC_3D_4STEP_6GC_2LIGHT==SliceFuncMode || SLICE_FUNC_3D_4STEP_6GC_2EXP==SliceFuncMode )
		{
			Ptr1[0]=CastParamObj.PtrA1;	Ptr1[1]=CastParamObj.PtrA2; Ptr1[2]=CastParamObj.PtrA3;	Ptr1[3]=CastParamObj.PtrA4;
			Ptr2[0]=CastParamObj.PtrB1;	Ptr2[1]=CastParamObj.PtrB2;	Ptr2[2]=CastParamObj.PtrB3;	Ptr2[3]=CastParamObj.PtrB4;	Ptr2[4]=CastParamObj.PtrB5;	Ptr2[5]=CastParamObj.PtrB6;
			if ( ImageAPI.GrayImage46GCFrameTo2Phase3(ImgW, ImgH, ImgStep, Ptr1[0], Ptr1[1], Ptr1[2], Ptr1[3], Per1, ExpTime1, Ptr2[0], Ptr2[1], Ptr2[2], Ptr2[3], Ptr2[4], Ptr2[5], Per2, ExpTime2, Gamma, ZeroPhasePtr, NoiseParam, MaskPtr, PhasePtr, PerN) == false )		
			{
				IsOK = false;
				break;
			}		
		}
		else
		{
			Ptr1[0]=CastParamObj.PtrA1;	Ptr1[1]=CastParamObj.PtrA2; Ptr1[2]=CastParamObj.PtrA3;	Ptr1[3]=CastParamObj.PtrA4;
			Ptr2[0]=CastParamObj.PtrB1;	Ptr2[1]=CastParamObj.PtrB2;	Ptr2[2]=CastParamObj.PtrB3;	Ptr2[3]=CastParamObj.PtrB4;
			if ( ImageAPI.GrayImage4FrameTo2Phase3(ImgW, ImgH, ImgStep, Ptr1[0], Ptr1[1], Ptr1[2], Ptr1[3], Per1, Ptr2[0], Ptr2[1], Ptr2[2], Ptr2[3], Per2, Gamma, ZeroPhasePtr, NoiseParam, MaskPtr, PhasePtr, PerN) == false )
			{
				IsOK = false;
				break;
			}
		}

		LIGHT_3D_CAST_ID CastID = (LIGHT_3D_CAST_ID)(CastParamObj.CastID);
		const int PhaseMode = CCameraCtrl::GetPhaseMode(BATCH_GRAB_STEP_4, BATCH_GRAB_PHASE_M);//BATCH_GRAB_PHASE_M, BATCH_GRAB_PHASE_M2
		if ( FN_ENABLE == CaliParam.m_EnableBasePhaseCorrect )
		{
			PHASE_PTR PhasePtr2=NULL;	
			const int Times=CaliParam.m_BasePhaseCorrectTimes;
			if ( ImageAPI.CorrectPhaseZero_Joe(ImgW, ImgH, ImgStep, PhasePtr, PhasePtr2, Times) == true )
			{
				if ( NULL != PhasePtr2 );
				{	::memcpy(PhasePtr, PhasePtr2, sizeof(PHASE_DATA)*BufferSize);	}
				JetMemory.free_func(PhasePtr2);
			}
		}
		if ( false == CopyToWhite )
		{	Light3DCtrl.SetLight3DPhaseZero(CastID, PhaseMode, DLPLEDColor, ImgW, ImgH, ImgStep, PhasePtr);	}
		else
		{	Light3DCtrl.SetLight3DPhaseZero(CastID, PhaseMode, DLP_LED_COLOR_WHITE, ImgW, ImgH, ImgStep, PhasePtr);	}
	}
	if ( false == IsOK )
	{		
		JetMemory.free_func(MaskPtr);
		JetMemory.free_func(PhasePtr);		
		MaskPtr = NULL;
		PhasePtr = NULL;		
		SetErrorString(ImageAPI.GetImageApiErrorString());
		return false;
	}	
	JetMemory.free_func(MaskPtr);
	JetMemory.free_func(PhasePtr);
	
	if ( Light3DCtrl.SaveAllLight3DCastParameter_PhaseZero() == false )
	{		
		SetErrorString(Light3DCtrl.GetErrorString());
		return false;
	}

	if ( true == CopyToWhite )
	{	
		IMAGE_SIZE PhaseW=0;
		IMAGE_SIZE PhaseH=0;
		IMAGE_SIZE PhaseStep=0;		
		std::vector<LIGHT_3D_CAST_ID> CastIDList;
		Light3DCtrl.GetLight3DCastIDList(CastIDList);
		const size_t CastIDCount=CastIDList.size();
		for ( i=0; i<CastIDCount; i++ )
		{
			LIGHT_3D_CAST_ID CastID = (LIGHT_3D_CAST_ID)(CastIDList[i]);
			const int PhaseMode = CCameraCtrl::GetPhaseMode(BATCH_GRAB_STEP_4, BATCH_GRAB_PHASE_M);//BATCH_GRAB_PHASE_M, BATCH_GRAB_PHASE_M2
			Light3DCtrl.GetLight3DPhaseZero(CastID, PhaseMode, DLP_LED_COLOR_WHITE, PhaseW, PhaseH, PhaseStep, PhasePtr);		
			Light3DCtrl.SetLight3DPhaseZero(CastID, PhaseMode, DLPLEDColor, ImgW, ImgH, ImgStep, PhasePtr);	
		}
		if ( Light3DCtrl.SaveAllLight3DCastParameter_PhaseZero() == false )
		{
			SetErrorString(Light3DCtrl.GetErrorString());
			return false;
		}
	}

	str=_T("Calibration 3D Zero Plane Finish");
	str=LoadMultiLanguageString_OnlineCalibration(str, str);
	SaveOnlineAutoCalibrationLog(str);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcAutoCalibration_3D_HeightFactor(TOnlineProcParam &Param)//線上檢測-自動校正-3D高度比例
{
	SaveOnlineAutoCalibrationLog(_T("Calibration 3D Height Factor Finish"));

	SetOnlineAutoCalibrationDateTime_3D_HeightFactor();
	SaveOnlineAutoCalibrationDateTime_3D_HeightFactor();
	ExecOnlineProcAutoCalibrationFinish(Param);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcBarcodeOpenProjectCheckEnd(ONLINE_OPEN_PROJECT_MODE eOpenMode)//線上檢測-確認是否最後一站
{
	const TSystemParameter &SysParam = GetSystemParameter();
	if ( ONLINE_OPEN_PROJECT_BARCODE_CAMERA == eOpenMode )
	{
		//確認在線條碼開專案模式
		if ( ONLINE_OPEN_PROJECT_BARCODE_DEVICE == SysParam.m_OnlineOpenProjectMode )
		{
			if ( BARCODE_DEVICE_GRAB_BEFORE_INSPECT == SysParam.m_OnlineOpenProjectBarcodeDeviceGrabMode )
			{	return false;  }			
		}
		return true;
	}
	if ( ONLINE_OPEN_PROJECT_BARCODE_DEVICE == eOpenMode )
	{	
		if ( FN_DISABLE == SysParam.m_OnlineOpenProjectCameraBarcode )
		{	return true; }
		if ( ONLINE_OPEN_PROJECT_BARCODE_DEVICE == SysParam.m_OnlineOpenProjectMode )
		{
			if ( BARCODE_DEVICE_GRAB_BEFORE_INSPECT == SysParam.m_OnlineOpenProjectBarcodeDeviceGrabMode )
			{	return true;  }			
		}
		return false;
	}
	if ( ONLINE_OPEN_PROJECT_BARCODE_HANDHELD == eOpenMode )
	{	
		if ( FN_DISABLE == SysParam.m_OnlineOpenProjectCameraBarcode )
		{	return true; }
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcBarcodeOpenProject(const std::vector<std::string> &BarcodeList, ONLINE_OPEN_PROJECT_MODE eOpenMode)//線上檢測-條碼開專案
{
	std::string strBarcode;
	const bool bSucc=ExecOnlineProcBarcodeOpenProject(BarcodeList, eOpenMode, strBarcode);
	return bSucc;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcBarcodeOpenProject(const std::vector<std::string> &BarcodeList, ONLINE_OPEN_PROJECT_MODE eOpenMode, std::string &Barcode)//線上檢測-條碼開專案
{
	Barcode="";
	size_t         i=0, j=0;
	std::wstring   wsValue;
	CString        strError;
	CString        strBarcode;
	LANE_ID LaneID = GetActiveLaneID();	
	std::vector<CString> BarcodeListTmp;
	std::vector<CString> ProjectNameListTmp;	
	const size_t BarcodeCount=BarcodeList.size();
	if ( 0 == BarcodeCount ) { return false; }

	for ( i=0; i<BarcodeCount; i++ )
	{
		std::vector<CString> ProjectNameListLoc;
		strBarcode = CString(BarcodeList[i].c_str());
		JetAPI::TCHAR2wstring(strBarcode, wsValue);
		if ( FindProjectListByCode(wsValue, ProjectNameListLoc) == false )
		{
			strError.Format(_T("Error, Open Project By Barcode Fault [FindProjectListByCode]\n[Barcode::%s]"), strBarcode);
			SetErrorString(strError);	
			return false; 
		}
		size_t ProjectNameCountLoc = ProjectNameListLoc.size();
		for ( j=0; j<ProjectNameCountLoc; j++ )
		{
			BarcodeListTmp.push_back(strBarcode);
			ProjectNameListTmp.push_back(ProjectNameListLoc[j]);	
		}
	}
	
	const size_t BarcodeCountTmp = BarcodeListTmp.size();
	const size_t ProjectNameCountTmp = ProjectNameListTmp.size();
	if ( 0==ProjectNameCountTmp || BarcodeCountTmp!=ProjectNameCountTmp )
	{	
		//確認後面是否還有掃條碼模式
		if ( ExecOnlineProcBarcodeOpenProjectCheckEnd(eOpenMode) == false )
		{	return true; }
		strError.Format(_T("Error, Open Project By Barcode Fault [0 == ProjectNameCount]\n[Barcode::%s]"), strBarcode);
		SetErrorString(strError);	
		return false;
	}
	
	CString FileName;
	CString ProjectName;
	CString FileFolder = GetAOIProjectDirectory();
	std::vector<CString> OpenBarcodeList;
	std::vector<CString> ProjectNameList;
	for ( i=0; i<ProjectNameCountTmp; i++ )
	{
		strBarcode = BarcodeListTmp[i];
		FileName = ProjectNameListTmp[i];
		ProjectName.Format(_T("%s\\%s"), FileFolder, FileName);
		if ( CheckProjectFilenameValided(ProjectName) == false )
		{	continue; }		
		if (JetAPI::IsFileExist(ProjectName) == false)
		{	continue; }
		OpenBarcodeList.push_back(strBarcode);
		ProjectNameList.push_back(FileName);
	}

	const size_t OpenBarcodeCount = OpenBarcodeList.size();
	const size_t ProjectNameCount = ProjectNameList.size();
	if ( 0==ProjectNameCount || ProjectNameCount!=OpenBarcodeCount )
	{
		//確認後面是否還有掃條碼模式
		if ( ExecOnlineProcBarcodeOpenProjectCheckEnd(eOpenMode) == false )
		{	return true; }
		strError.Format(_T("Error, Open Project By Barcode Fault [0 == ProjectNameCount]\n[Barcode::%s]"), strBarcode);
		SetErrorString(strError);	
		return false;
	}
	
	if ( ProjectNameCount > 1 )
	{
		TListNode       Node;
		CString         strCaption;
		CString         strLabel;
		CInputListWnd   EnumWnd;
		INT_PTR         Res=0;
		DWORD_PTR       dwDefault=0;
		std::vector<TListNode> NodelList;		

		strLabel = _T("Project Name");
		strCaption = _T("Select Project");		
		for ( i=0; i<ProjectNameCount; i++ )
		{
			Node.Data = i;
			Node.Text = ProjectNameList[i];
			NodelList.push_back(Node);
		}		
		EnumWnd.SetParam1(strCaption, strLabel, dwDefault, NodelList);	
		PlcCtrlPtr->TurnOnInspectAlarm(LaneID);
		Res = EnumWnd.DoModal();
		PlcCtrlPtr->TurnOffInspectAlarm(LaneID);
		if ( IDCANCEL == Res )
		{			
			strError = _T("Error, User cancel the project selection");
			SetErrorString(strError);	
			return false; 
		}
		size_t NameIndex = (size_t)(EnumWnd.GetSelData());	
		if ( NameIndex >= ProjectNameCount )
		{	
			strError.Format(_T("Error, Open Project By Barcode Fault [NameIndex >= ProjectNameCount]\n[Barcode::%s]"), strBarcode);
			SetErrorString(strError);	
			return false; 
		}
		FileName = ProjectNameList[NameIndex];
		strBarcode = OpenBarcodeList[NameIndex];
	}
	else
	{	
		FileName = ProjectNameList[0];	
		strBarcode = OpenBarcodeList[0];
	}


	CString ProjectShowName;
	CAOIProject *ProjectPtr = NULL;		
	const bool   bOnline = true;
	const unsigned int ProjectIndex = 0;
	ProjectName.Format(_T("%s\\%s"), FileFolder, FileName);
	ProjectPtr = GetProjectPtrByFilename(ProjectName);
	if ( NULL != ProjectPtr )
	{		
		SetActiveProjectPtr(ProjectPtr);
		SetOnlineOpenProjectFinish(true);
		JetAPI::TCHAR2string(strBarcode, Barcode);
		return true;
	}
		
	ProjectPtr = GetProjectPtr(ProjectIndex, true);
	if ( NULL == ProjectPtr ) 
	{ 
		ProjectPtr = AOIObjManager.CreateProjectObj();
		if ( NULL == ProjectPtr ) 
		{
			strError = AOIObjManager.GetErrorString();
			SetErrorString(strError);	
			return false;
		}			
		AddProjectPtr(ProjectPtr, false);
		SetActiveProjectIndex(ProjectIndex);		
	}
	else
	{	
		if ( CloseProject(ProjectPtr, bOnline) == false )
		{	return false; }		
		SetActiveProjectPtr(ProjectPtr);
	}	
	if ( OpenProject(ProjectPtr, ProjectName, bOnline) == false )
	{	return false; }	
	SetOnlineOpenProjectFinish(true);
	JetAPI::TCHAR2string(strBarcode, Barcode);
	SendMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_SWITCH, NULL);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineProcPCBAutoRunCheckingOnInspecting()//線上檢測-自動進出板確認-檢測中
{
	int     nState;		
	bool    bPCBStop=false;
	LANE_ID eLaneID = GetActiveLaneID();	
	ONLINE_STATE_MODE eOnlineState;

	switch ( eLaneID )
	{
	case LANE_ID_A: eLaneID = LANE_ID_B; break;
	case LANE_ID_B: eLaneID = LANE_ID_A; break;		
	}	
	eOnlineState = GetOnlineStateMode_Lane(eLaneID);
	if ( ONLINE_STATE_PCB_AUTO_RUN_CHECKING == eOnlineState )
	{		
		nState = PlcCtrlPtr->GetConveryerStatus(eLaneID);
		bPCBStop = PlcCtrlPtr->GetConveryerSensorPCBStop(eLaneID);
		if ( PLC_CONVERYER_STATUS_STOP==nState && true==bPCBStop )
		{	
			eOnlineState = ONLINE_STATE_PCB_AUTO_RUN_FINISH;
			SetOnlineStateMode_Lane(eOnlineState, eLaneID);
		}
	}	
	return true;
}
//-------------------------------------------------------------------------------------//