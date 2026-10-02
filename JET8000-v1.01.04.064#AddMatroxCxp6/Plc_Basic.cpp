// Plc_Basic.cpp: implementation of the CPLC_Basic class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "Plc_Basic.h"
//-------------------------------------------------------------------------------------//
#include <process.h>
#include "Plc_Vigor.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
#if PLC_OBJ_MODE == PLC_OBJ_VIGOR
	CPLC_Basic *PlcCtrlPtr = &PLC_Vigor;
#else
	CPLC_Basic *PlcCtrlPtr = NULL;
	#error PLC Mode is not specified.
#endif//PLC_OBJ_MODE	
//-------------------------------------------------------------------------------------//
unsigned int  PLCPollingThreadID = 0;
HANDLE        PLCPollingThreadHandle = NULL;
unsigned int __stdcall PLCPollingThreadFn(void *pParam);//PLC監控函式
//-------------------------------------------------------------------------------------//
unsigned int __stdcall PLCPollingThreadFn(void *pParam)//PLC監控函式
{	
	if ( NULL == PlcCtrlPtr ) { return 0; }

	int SleepTime = PlcCtrlPtr->GetPLCPollingSleepTime();	
	THREAD_COMMAND_MODE ThreadCmd = THREAD_COMMAND_TO_NONE;
	while ( true )
	{
		ThreadCmd = PlcCtrlPtr->GetPLCPollingThreadCmd();
		if ( THREAD_COMMAND_TO_EXIT == ThreadCmd )
		{
			PlcCtrlPtr->SetPLCPollingThreadState(THREAD_STATE_NONE);
			return 0;
		}
		
		if ( THREAD_COMMAND_TO_IDLE == ThreadCmd )
		{
			PlcCtrlPtr->SetPLCPollingThreadState(THREAD_STATE_IDLE);
			::Sleep(SleepTime);
			continue;
		}

		if ( PlcCtrlPtr->GetPLCIsConnected() == false )
		{	continue; }		

		if ( THREAD_COMMAND_TO_RUN == ThreadCmd ) 
		{
			PlcCtrlPtr->SetPLCPollingThreadState(THREAD_STATE_RUNNING);		
			PlcCtrlPtr->PLC_ReadAllStats(true);
			PlcCtrlPtr->PLC_ReadPlcNodeList(true);
			PlcCtrlPtr->SetPLCPollingThreadState(THREAD_STATE_FINISH);
			continue;
		}
		::Sleep(SleepTime);		 
	};	
	//::_endthreadex(0);
	return 0;
}
//----------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
IMPLEMENT_DYNAMIC(CPLC_Basic, CObject)
//-------------------------------------------------------------------------------------//
CString CPLC_Basic::ObtainPLCParameterDescText(PLC_PARAM_ID ParamID)//取得PLC參數的說明文字		
{
	CString String;	
	CString KeyName;	
	CString Default;
	switch ( ParamID )
	{
	case PLC_PARAM_COMM_DELAY_TIME://PLC通訊延遲時間	
		KeyName = _T("PLC_PARAM_COMM_DELAY_TIME");
		Default = String = _T("PLC Communicate Delay Time");
		break;
	case PLC_PARAM_CHECK_PCB_INSIDE_BEFORE_PCB_IN://進板前確認機台有無板子
		KeyName = _T("PLC_PARAM_CHECK_PCB_INSIDE_BEFORE_PCB_IN");
		Default = String = _T("Check PCB Inside Before PCB In");
		break;
	case PLC_PARAM_PCB_IN_OUT_TIMEOUT_LA://PCB進出板逾時-10s-A軌 
		KeyName = _T("PLC_PARAM_PCB_IN_OUT_TIMEOUT_LA");
		Default = String = _T("PCB In Out Timeout Lane A");
		break;
	case PLC_PARAM_PCB_IN_OUT_TIMEOUT_LB://PCB進出板逾時-10s-B軌
		KeyName = _T("PLC_PARAM_PCB_IN_OUT_TIMEOUT_LB");
		Default = String = _T("PCB In Out Timeout Lane B");
		break;
	case PLC_PARAM_AIR_LOST_CHECK_TIME://氣壓不足監控時間-0.5s
		KeyName = _T("PLC_PARAM_AIR_LOST_CHECK_TIME");
		Default = String = _T("Check Air Lost Time");
		break;	
	case PLC_PARAM_PCB_STOP_DELAY_TIME://定位Sensor感應後持續運轉延遲時間-0.1sec
		KeyName = _T("PLC_PARAM_PCB_STOP_DELAY_TIME");
		Default = String = _T("PCB Stop Delay Time");
		break;
	case PLC_PARAM_CLAMP_ON_OFF_DELAY_TIME://維修模式時-汽缸夾鬆板愈時-1s
		KeyName = _T("PLC_PARAM_CLAMP_ON_OFF_DELAY_TIME");
		Default = String = _T("Clamp On/Off Delay Time");
		break;
	case PLC_PARAM_PCB_OUT_WITH_IN_DELAY_TIME_LA://PCB出板帶進板延遲時間-0.1s-A軌
		KeyName = _T("PLC_PARAM_PCB_OUT_WITH_IN_DELAY_TIME_LA");
		Default = String = _T("PCB Out-With-In Delay Time Lane A");
		break;
	case PLC_PARAM_PCB_OUT_WITH_IN_DELAY_TIME_LB://PCB出板帶進板延遲時間-0.1s-B軌
		KeyName = _T("PLC_PARAM_PCB_OUT_WITH_IN_DELAY_TIME_LB");
		Default = String = _T("PCB Out-With-In Delay Time Lane B");
		break;
	case PLC_PARAM_PCB_OUT_DELAY_TIME_LA://PCB出板後延遲時間-0.1s-A軌
		KeyName = _T("PLC_PARAM_PCB_OUT_DELAY_TIME_LA");
		Default = String = _T("PCB Out Delay Time Lane A");
		break;
	case PLC_PARAM_PCB_OUT_DELAY_TIME_LB://PCB出板後延遲時間-0.1s-B軌
		KeyName = _T("PLC_PARAM_PCB_OUT_DELAY_TIME_LB");
		Default = String = _T("PCB Out Delay Time Lane B");
		break;
	case PLC_PARAM_PCB_CLEAR_TIMEOUT_LA://PCB板清除時間-時間單位:1=100ms
		KeyName = _T("PLC_PARAM_PCB_CLEAR_TIMEOUT_LA");
		Default = String = _T("PLC Clear Timeout Lane A");
		break;
	case PLC_PARAM_PCB_CLEAR_TIMEOUT_LB://PCB板清除時間-時間單位:1=100ms
		KeyName = _T("PLC_PARAM_PCB_CLEAR_TIMEOUT_LB");
		Default = String = _T("PLC Clear Timeout Lane B");
		break;	
	case PLC_PARAM_TURN_ON_LAST_SIGNAL_DELAY_TIME://機台向上一站要板持續時間:1=100ms
		KeyName = _T("PLC_PARAM_TURN_ON_LAST_SIGNAL_DELAY_TIME");
		Default = String = _T("Turn On Last Signal Delay Time");
		break;
	case PLC_PARAM_CHECK_PCB_DOUBLE_IN_PITCH_TIME://入料檢知頻寬設定(2個板間隔時間):1=100ms
		KeyName = _T("PLC_PARAM_CHECK_PCB_DOUBLE_IN_PITCH_TIME");
		Default = String = _T("Check PCB Double In Pitch Time");
		break;
	case PLC_PARAM_CHECK_PCB_DOUBLE_IN_BOARD_TIME://入料檢知時間設定(單板最長時間):1=100ms
		KeyName = _T("PLC_PARAM_CHECK_PCB_DOUBLE_IN_BOARD_TIME");
		Default = String = _T("Check PCB Double In Board Time");
		break;
	case PLC_PARAM_PCB_IN_AUTO_OFF_STOP_BAR_LA://PCB進出自動縮停止塊-A軌
		KeyName = _T("PLC_PARAM_PCB_IN_AUTO_OFF_STOP_BAR_LA");
		Default = String = _T("PCB In Auto Off Stop Bar Lane A");
		break;
	case PLC_PARAM_PCB_IN_AUTO_OFF_STOP_BAR_LB://PCB進出自動縮停止塊-B軌
		KeyName = _T("PLC_PARAM_PCB_IN_AUTO_OFF_STOP_BAR_LB");
		Default = String = _T("PCB In Auto Off Stop Bar Lane B");
		break;
	case PLC_PARAM_PCB_AUTO_RUN_WITH_SIDE_STOP_LA://PCB自動進出板帶停板邊-A軌
		KeyName = _T("PLC_PARAM_PCB_AUTO_RUN_WITH_SIDE_STOP_LA");
		Default = String = _T("PCB Auto Run With Side Stop Lane A");
		break;
	case PLC_PARAM_PCB_AUTO_RUN_WITH_SIDE_STOP_LB://PCB自動進出板帶停板邊-B軌	
		KeyName = _T("PLC_PARAM_PCB_AUTO_RUN_WITH_SIDE_STOP_LB");
		Default = String = _T("PCB Auto Run With Side Stop Lane B");
		break;
	case PLC_PARAM_TURN_OFF_LANE_SENSOR_POWER_LA://關閉軌道感測器電源-A軌
		KeyName = _T("PLC_PARAM_TURN_OFF_LANE_SENSOR_POWER_LA");
		Default = String = _T("Turn Off Conveyer Sensor Power Lane A");
		break;
	case PLC_PARAM_TURN_OFF_LANE_SENSOR_POWER_LB://關閉軌道感測器電源-B軌
		KeyName = _T("PLC_PARAM_TURN_OFF_LANE_SENSOR_POWER_LB");
		Default = String = _T("Turn Off Conveyer Sensor Power Lane B");
		break;
	case PLC_PARAM_PCB_OUT_WITH_CLEAR_OK_NG_SIGNAL://PCB出板時清除OK, NG訊號	
		KeyName = _T("PLC_PARAM_PCB_OUT_WITH_CLEAR_OK_NG_SIGNAL");
		Default = String = _T("PCB Out With Clear OK NG Signal");
		break;
	case PLC_PARAM_ENABLE_CHECK_PCB_DOUBLE_IN://確認PCB重複進板啟用
		KeyName = _T("PLC_PARAM_ENABLE_CHECK_PCB_DOUBLE_IN");
		Default = String = _T("Enable Check PCB Double In");
		break;
	case PLC_PARAM_ENABLE_CHECK_PCB_BARGE_IN://啟用確認檢測時PCB板闖入
		KeyName = _T("PLC_PARAM_ENABLE_CHECK_PCB_BARGE_IN");
		Default = String = _T("Enable Check PCB Barge In");
		break;
	case PLC_PARAM_ENABLE_FAN_ALARM://啟用風扇警報
		KeyName = _T("PLC_PARAM_ENABLE_FAN_ALARM");
		Default = String = _T("Enable Fan Alarm");
		break;
	case PLC_PARAM_ENABLE_OPEN_DOOR_STOP_POWER://啟用開門斷電
		KeyName = _T("PLC_PARAM_ENABLE_OPEN_DOOR_STOP_POWER");
		Default = String = _T("Enable Open-Door Stop Power");
		break;
	case PLC_PARAM_ENABLE_STOPPER_SENSOR_LA://啟用擋板塊感應器-A軌
		KeyName = _T("PLC_PARAM_ENABLE_STOPPER_SENSOR_LA");		
		Default = String = _T("Enable Stopper Sensor Lane A");
		break;
	case PLC_PARAM_ENABLE_STOPPER_SENSOR_LB://啟用擋板塊感應器-B軌
		KeyName = _T("PLC_PARAM_ENABLE_STOPPER_SENSOR_LB");		
		Default = String = _T("Enable Stopper Sensor Lane B");
		break;
	}
	if ( KeyName.GetLength() == 0 ) 
	{
		String = Default;
		return String;
	}
	KeyName = KeyName + _T("_DESCRIPTION");
	AOIDataCollect.GetUILanguageString(_T("PLC_PARAMETER"), KeyName, Default, String);
	return String;
}
//-------------------------------------------------------------------------------------//
bool CPLC_Basic::SetPLCParameterStringByID(PLC_PARAM_ID ParamID, TPLCParameter &Param, LPCTSTR String)//設定PLC參數
{
	CString Err;	
	int     tempI=0;
	double  tempD=0;
	bool IsOK = true;	
	switch ( ParamID )
	{
	case PLC_PARAM_COMM_DELAY_TIME://PLC通訊延遲時間			
		tempI = ::_ttoi(String);
		if ( tempI >= 0 ) 
		{	Param.m_PLCCommDelayTime = tempI; }
		else 
		{	IsOK = false; }
		break;
	case PLC_PARAM_CHECK_PCB_INSIDE_BEFORE_PCB_IN://進板前確認機台有無板子
		tempI = ::_ttoi(String);
		if ( 0 == tempI ) { Param.m_CheckPCBInsideBeforePCBIn = FN_DISABLE; }
		else { Param.m_CheckPCBInsideBeforePCBIn = FN_ENABLE; }		
		break;
	case PLC_PARAM_PCB_IN_OUT_TIMEOUT_LA://PCB進出板逾時-10s-A軌
		tempI = ::_ttoi(String);
		if ( tempI > 0 ) 
		{	Param.m_PCBInOutTimeout_LA = tempI; }
		else 
		{	IsOK = false; }
		break;
	case PLC_PARAM_PCB_IN_OUT_TIMEOUT_LB://PCB進出板逾時-10s-B軌		
		tempI = ::_ttoi(String);
		if ( tempI > 0 ) 
		{	Param.m_PCBInOutTimeout_LB = tempI; }
		else 
		{	IsOK = false; }
		break;
	case PLC_PARAM_AIR_LOST_CHECK_TIME://氣壓不足監控時間-0.5s		
		tempI = ::_ttoi(String);
		if ( tempI > 0 ) 
		{	Param.m_AirLostCheckTime = tempI; }
		else 
		{	IsOK = false; }
		break;	
	case PLC_PARAM_PCB_STOP_DELAY_TIME://定位Sensor感應後持續運轉延遲時間-0.1sec		
		tempI = ::_ttoi(String);
		if ( tempI > 0 ) 
		{	Param.m_PCBStopDelayTime = tempI; }
		else 
		{	IsOK = false; }
		break;
	case PLC_PARAM_CLAMP_ON_OFF_DELAY_TIME://維修模式時-汽缸夾鬆板愈時-1s		
		tempI = ::_ttoi(String);
		if ( tempI > 0 ) 
		{	Param.m_ClampOnOffDelayTime = tempI; }
		else 
		{	IsOK = false; }
		break;
	case PLC_PARAM_PCB_OUT_WITH_IN_DELAY_TIME_LA://PCB出板帶進板延遲時間-0.1s-A軌		
		tempI = ::_ttoi(String);
		if ( tempI > 0 ) 
		{	Param.m_PCBOutWithInDelayTime_LA = tempI; }
		else 
		{	IsOK = false; }
		break;		
	case PLC_PARAM_PCB_OUT_WITH_IN_DELAY_TIME_LB://PCB出板帶進板延遲時間-0.1s-B軌		
		tempI = ::_ttoi(String);
		if ( tempI > 0 ) 
		{	Param.m_PCBOutWithInDelayTime_LB = tempI; }
		else 
		{	IsOK = false; }
		break;
	case PLC_PARAM_PCB_OUT_DELAY_TIME_LA://PCB出板後延遲時間-0.1s-A軌		
		tempI = ::_ttoi(String);
		if ( tempI >= 0 ) 
		{	Param.m_PCBOutDelayTime_LA = tempI; }
		else 
		{	IsOK = false; }
		break;
	case PLC_PARAM_PCB_OUT_DELAY_TIME_LB://PCB出板後延遲時間-0.1s-B軌		
		tempI = ::_ttoi(String);
		if ( tempI >= 0 ) 
		{	Param.m_PCBOutDelayTime_LB = tempI; }
		else 
		{	IsOK = false; }
		break;
	case PLC_PARAM_PCB_CLEAR_TIMEOUT_LA://PCB板清除時間-時間單位:1=100ms		
		tempI = ::_ttoi(String);
		if ( tempI > 0 ) 
		{	Param.m_PCBClearTimeout_LA = tempI; }
		else 
		{	IsOK = false; }
		break;
	case PLC_PARAM_PCB_CLEAR_TIMEOUT_LB://PCB板清除時間-時間單位:1=100ms		
		tempI = ::_ttoi(String);
		if ( tempI > 0 ) 
		{	Param.m_PCBClearTimeout_LB = tempI; }
		else 
		{	IsOK = false; }
		break;	
	case PLC_PARAM_TURN_ON_LAST_SIGNAL_DELAY_TIME://機台向上一站要板持續時間:1=100ms
		tempI = ::_ttoi(String);
		if ( tempI > 0 ) 
		{	Param.m_TurnOnLastSignalDelayTime = tempI; }
		else 
		{	IsOK = false; }
		break;
	case PLC_PARAM_CHECK_PCB_DOUBLE_IN_PITCH_TIME://入料檢知頻寬設定(2個板間隔時間):1=100ms
		tempI = ::_ttoi(String);
		if ( tempI > 0 ) 
		{	Param.m_CheckPCBDoubleInPitchTime = tempI; }
		else 
		{	IsOK = false; }
		break;
	case PLC_PARAM_CHECK_PCB_DOUBLE_IN_BOARD_TIME://入料檢知時間設定(單板最長時間):1=100ms
		tempI = ::_ttoi(String);
		if ( tempI > 0 ) 
		{	Param.m_CheckPCBDoubleInBoardTime = tempI; }
		else 
		{	IsOK = false; }
		break;
	case PLC_PARAM_PCB_IN_AUTO_OFF_STOP_BAR_LA://PCB進出自動縮停止塊-A軌
		tempI = ::_ttoi(String);
		if ( 0 == tempI ) { Param.m_PCBInAutoOffStopBar_LA = FN_DISABLE; }
		else { Param.m_PCBInAutoOffStopBar_LA = FN_ENABLE; }			 
		break;
	case PLC_PARAM_PCB_IN_AUTO_OFF_STOP_BAR_LB://PCB進出自動縮停止塊-B軌
		tempI = ::_ttoi(String);
		if ( 0 == tempI ) { Param.m_PCBInAutoOffStopBar_LB = FN_DISABLE; }
		else { Param.m_PCBInAutoOffStopBar_LB = FN_ENABLE; }
		break;
	case PLC_PARAM_PCB_AUTO_RUN_WITH_SIDE_STOP_LA://PCB自動進出板帶停板邊-A軌
		tempI = ::_ttoi(String);
		if ( 0 == tempI ) { Param.m_PCBAutoRunWithSideStop_LA = FN_DISABLE; }
		else { Param.m_PCBAutoRunWithSideStop_LA = FN_ENABLE; }			 
		break;
	case PLC_PARAM_PCB_AUTO_RUN_WITH_SIDE_STOP_LB://PCB自動進出板帶停板邊-B軌	
		tempI = ::_ttoi(String);
		if ( 0 == tempI ) { Param.m_PCBAutoRunWithSideStop_LB = FN_DISABLE; }
		else { Param.m_PCBAutoRunWithSideStop_LB = FN_ENABLE; }			 
		break;
	case PLC_PARAM_TURN_OFF_LANE_SENSOR_POWER_LA://關閉軌道感測器電源-A軌
		tempI = ::_ttoi(String);
		if ( 0 == tempI ) { Param.m_TurnOffConveyerSensorPower_LA = FN_DISABLE; }
		else { Param.m_TurnOffConveyerSensorPower_LA = FN_ENABLE; }
		break;
	case PLC_PARAM_TURN_OFF_LANE_SENSOR_POWER_LB://關閉軌道感測器電源-B軌
		tempI = ::_ttoi(String);
		if ( 0 == tempI ) { Param.m_TurnOffConveyerSensorPower_LB = FN_DISABLE; }
		else { Param.m_TurnOffConveyerSensorPower_LB = FN_ENABLE; }
		break;
	case PLC_PARAM_PCB_OUT_WITH_CLEAR_OK_NG_SIGNAL://PCB出板時清除OK, NG訊號	
		tempI = ::_ttoi(String);
		if ( 0 == tempI ) { Param.m_PCBOutWithClearOKNGSignal = FN_DISABLE; }
		else { Param.m_PCBOutWithClearOKNGSignal = FN_ENABLE; }
		break;
	case PLC_PARAM_ENABLE_CHECK_PCB_DOUBLE_IN://啟用確認PCB重複進板
		tempI = ::_ttoi(String);
		if ( 0 == tempI ) { Param.m_EnableCheckPCBDoubleIn = FN_DISABLE; }
		else { Param.m_EnableCheckPCBDoubleIn = FN_ENABLE; }
		break;
	case PLC_PARAM_ENABLE_CHECK_PCB_BARGE_IN://啟用確認檢測時PCB板闖入
		tempI = ::_ttoi(String);
		if ( 0 == tempI ) { Param.m_EnableCheckPCBBargeIn = FN_DISABLE; }
		else { Param.m_EnableCheckPCBBargeIn = FN_ENABLE; }
		break;
	case PLC_PARAM_ENABLE_FAN_ALARM://啟用風扇警報
		tempI = ::_ttoi(String);
		if ( 0 == tempI ) { Param.m_EnabledFanAlarm = FN_DISABLE; }
		else { Param.m_EnabledFanAlarm = FN_ENABLE; }
		break;
	case PLC_PARAM_ENABLE_OPEN_DOOR_STOP_POWER://啟用開門斷電
		tempI = ::_ttoi(String);
		if ( 0 == tempI ) { Param.m_EnabledOpenDoorStopPower = FN_DISABLE; }
		else { Param.m_EnabledOpenDoorStopPower = FN_ENABLE; }
		break;
	case PLC_PARAM_ENABLE_STOPPER_SENSOR_LA://啟用擋板塊感應器-A軌
		tempI = ::_ttoi(String);
		if ( 0 == tempI ) { Param.m_EnabledStopperSensor_LA = FN_DISABLE; }
		else { Param.m_EnabledStopperSensor_LA = FN_ENABLE; }
		break;
	case PLC_PARAM_ENABLE_STOPPER_SENSOR_LB://啟用擋板塊感應器-B軌
		tempI = ::_ttoi(String);
		if ( 0 == tempI ) { Param.m_EnabledStopperSensor_LB = FN_DISABLE; }
		else { Param.m_EnabledStopperSensor_LB = FN_ENABLE; }
		break;
	default:
		IsOK = false;
		Err.Format(_T("Error, No PLC Param Define [%d]"), ParamID);
		JetAPI::ShowMessageBox(Err);
		break;
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CPLC_Basic::GetPLCParameterStringByID(PLC_PARAM_ID ParamID, const TPLCParameter &Param, CString &String)//取得PLC參數
{
	CString Err;		
	bool IsOK = true;	
	switch ( ParamID )
	{
	case PLC_PARAM_COMM_DELAY_TIME://PLC通訊延遲時間
		String.Format(_T("%d"), Param.m_PLCCommDelayTime);
		break;
	case PLC_PARAM_CHECK_PCB_INSIDE_BEFORE_PCB_IN://進板前確認機台有無板子		
		String.Format(_T("%d"), Param.m_CheckPCBInsideBeforePCBIn);
		break;
	case PLC_PARAM_PCB_IN_OUT_TIMEOUT_LA://PCB進出板逾時-10s-A軌
		String.Format(_T("%d"), Param.m_PCBInOutTimeout_LA);
		break;
	case PLC_PARAM_PCB_IN_OUT_TIMEOUT_LB://PCB進出板逾時-10s-B軌		
		String.Format(_T("%d"), Param.m_PCBInOutTimeout_LB);
		break;
	case PLC_PARAM_AIR_LOST_CHECK_TIME://氣壓不足監控時間-0.5s		
		String.Format(_T("%d"), Param.m_AirLostCheckTime);
		break;	
	case PLC_PARAM_PCB_STOP_DELAY_TIME://定位Sensor感應後持續運轉延遲時間-0.1sec		
		String.Format(_T("%d"), Param.m_PCBStopDelayTime);
		break;		
	case PLC_PARAM_CLAMP_ON_OFF_DELAY_TIME://維修模式時-汽缸夾鬆板愈時-1s		
		String.Format(_T("%d"), Param.m_ClampOnOffDelayTime);
		break;
	case PLC_PARAM_PCB_OUT_WITH_IN_DELAY_TIME_LA://PCB出板帶進板延遲時間-0.1s-A軌		
		String.Format(_T("%d"), Param.m_PCBOutWithInDelayTime_LA);
		break;
	case PLC_PARAM_PCB_OUT_WITH_IN_DELAY_TIME_LB://PCB出板帶進板延遲時間-0.1s-B軌		
		String.Format(_T("%d"), Param.m_PCBOutWithInDelayTime_LB);
		break;
	case PLC_PARAM_PCB_OUT_DELAY_TIME_LA://PCB出板後延遲時間-0.1s-A軌		
		String.Format(_T("%d"), Param.m_PCBOutDelayTime_LA);
		break;
	case PLC_PARAM_PCB_OUT_DELAY_TIME_LB://PCB出板後延遲時間-0.1s-B軌		
		String.Format(_T("%d"), Param.m_PCBOutDelayTime_LB);
		break;
	case PLC_PARAM_PCB_CLEAR_TIMEOUT_LA://PCB板清除時間-時間單位:1=100ms		
		String.Format(_T("%d"), Param.m_PCBClearTimeout_LA);
		break;
	case PLC_PARAM_PCB_CLEAR_TIMEOUT_LB://PCB板清除時間-時間單位:1=100ms		
		String.Format(_T("%d"), Param.m_PCBClearTimeout_LB);
		break;	
	case PLC_PARAM_TURN_ON_LAST_SIGNAL_DELAY_TIME://機台向上一站要板持續時間:1=100ms
		String.Format(_T("%d"), Param.m_TurnOnLastSignalDelayTime);
		break;
	case PLC_PARAM_CHECK_PCB_DOUBLE_IN_PITCH_TIME://入料檢知頻寬設定(2個板間隔時間):1=100ms
		String.Format(_T("%d"), Param.m_CheckPCBDoubleInPitchTime);
		break;
	case PLC_PARAM_CHECK_PCB_DOUBLE_IN_BOARD_TIME://入料檢知時間設定(單板最長時間):1=100ms
		String.Format(_T("%d"), Param.m_CheckPCBDoubleInBoardTime);
		break;
	case PLC_PARAM_PCB_IN_AUTO_OFF_STOP_BAR_LA://PCB進出自動縮停止塊-A軌
		String.Format(_T("%d"), Param.m_PCBInAutoOffStopBar_LA);
		break;		
	case PLC_PARAM_PCB_IN_AUTO_OFF_STOP_BAR_LB://PCB進出自動縮停止塊-B軌
		String.Format(_T("%d"), Param.m_PCBInAutoOffStopBar_LB);
		break;
	case PLC_PARAM_PCB_AUTO_RUN_WITH_SIDE_STOP_LA://PCB自動進出板帶停板邊-A軌
		String.Format(_T("%d"), Param.m_PCBAutoRunWithSideStop_LA);
		break;
	case PLC_PARAM_PCB_AUTO_RUN_WITH_SIDE_STOP_LB://PCB自動進出板帶停板邊-B軌	
		String.Format(_T("%d"), Param.m_PCBAutoRunWithSideStop_LB);
		break;
	case PLC_PARAM_TURN_OFF_LANE_SENSOR_POWER_LA://關閉軌道感測器電源-A軌
		String.Format(_T("%d"), Param.m_TurnOffConveyerSensorPower_LA);		
		break;
	case PLC_PARAM_TURN_OFF_LANE_SENSOR_POWER_LB://關閉軌道感測器電源-B軌
		String.Format(_T("%d"), Param.m_TurnOffConveyerSensorPower_LB);
		break;
	case PLC_PARAM_PCB_OUT_WITH_CLEAR_OK_NG_SIGNAL://PCB出板時清除OK, NG訊號	
		String.Format(_T("%d"), Param.m_PCBOutWithClearOKNGSignal);
		break;
	case PLC_PARAM_ENABLE_CHECK_PCB_DOUBLE_IN://啟用確認PCB重複進板
		String.Format(_T("%d"), Param.m_EnableCheckPCBDoubleIn);
		break;
	case PLC_PARAM_ENABLE_CHECK_PCB_BARGE_IN://啟用確認檢測時PCB板闖入
		String.Format(_T("%d"), Param.m_EnableCheckPCBBargeIn);
		break;
	case PLC_PARAM_ENABLE_FAN_ALARM://啟用風扇警報
		String.Format(_T("%d"), Param.m_EnabledFanAlarm);
		break;
	case PLC_PARAM_ENABLE_OPEN_DOOR_STOP_POWER://啟用開門斷電
		String.Format(_T("%d"), Param.m_EnabledOpenDoorStopPower);
		break;
	case PLC_PARAM_ENABLE_STOPPER_SENSOR_LA://啟用擋板塊感應器-A軌
		String.Format(_T("%d"), Param.m_EnabledStopperSensor_LA);
		break;
	case PLC_PARAM_ENABLE_STOPPER_SENSOR_LB://啟用擋板塊感應器-B軌
		String.Format(_T("%d"), Param.m_EnabledStopperSensor_LB);
		break;	
	default:
		IsOK = false;
		Err.Format(_T("Error, No PLC Param Define [%d]"), ParamID);
		JetAPI::ShowMessageBox(Err);
		break;
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CPLC_Basic::CheckPlcNodeAddress(const char *Address)
{
	if ( NULL == Address ) { return false; }
	
	bool IsOK = false;
	switch ( Address[0] )
	{
	case 'x':
	case 'X':
	case 'y':
	case 'Y':
	case 'm':
	case 'M':
	case 'd':
	case 'D':
		IsOK = true;
		break;
	default:
		break;
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
CString CPLC_Basic::FormTowerLightModelText(int Mode)//取得塔燈模式文字
{
	CString str, KeyName, Default;
	switch ( Mode )
	{
	case PLC_TOWER_LIGHT_NORMAL:		
		str = Default=_T("Normal");	
		KeyName = _T("PLC_TOWER_LIGHT_NORMAL");
		break;
	case PLC_TOWER_LIGHT_USER_DEFINE:
		str = Default=_T("User Define");
		KeyName = _T("PLC_TOWER_LIGHT_USER_DEFINE");
		break;	
	default:
		str = Default = _T("Undefined"); 
		KeyName = _T("Undefined");
		break;
	}
	AOIDataCollect.GetUILanguageString(_T("PLC_TOWER_LIGHT_MODE"), KeyName, Default, str);	
	return str;
}
//-------------------------------------------------------------------------------------//
CString CPLC_Basic::FormTowerLightStateText(int State)//取得塔燈狀態文字
{
	CString str, KeyName, Default;
	switch ( State )
	{
	case PLC_TOWER_LIGHT_TURN_ON_RED:		
		str = Default=_T("On Red");	
		KeyName = _T("PLC_TOWER_LIGHT_TURN_ON_RED");
		break;
	case PLC_TOWER_LIGHT_TURN_ON_YELLOW:
		str = Default=_T("On Yellow");
		KeyName = _T("PLC_TOWER_LIGHT_TURN_ON_YELLOW");
		break;
	case PLC_TOWER_LIGHT_TURN_ON_GREEN:
		str = Default=_T("On Green");
		KeyName = _T("PLC_TOWER_LIGHT_TURN_ON_GREEN");
		break;
	case PLC_TOWER_LIGHT_FLASH_RED:
		str = Default=_T("Flash Red*");
		KeyName = _T("PLC_TOWER_LIGHT_FLASH_RED");
		break;
	case PLC_TOWER_LIGHT_FLASH_YELLOW:
		str = Default=_T("Flash Yellow*");
		KeyName = _T("PLC_TOWER_LIGHT_FLASH_YELLOW");
		break;
	case PLC_TOWER_LIGHT_FLASH_GREEN:
		str = Default=_T("Flash Green*");
		KeyName = _T("PLC_TOWER_LIGHT_FLASH_GREEN");
		break;
	case PLC_TOWER_LIGHT_TURN_ON_YELLOW_GREEN:
		str = Default=_T("On Yellow Green");	
		KeyName = _T("PLC_TOWER_LIGHT_TURN_ON_YELLOW_GREEN");
		break;
	default:
		str = Default = _T("Undefined"); 
		KeyName = _T("Undefined");
		break;
	}
	AOIDataCollect.GetUILanguageString(_T("PLC_TOWER_LIGHT_STATE"), KeyName, Default, str);	
	return str;
}
//-------------------------------------------------------------------------------------//
bool CPLC_Basic::BuildTowerLightModeCombox(CComboBox &Combox)//建立塔燈模式列表視窗
{	
	size_t       i=0;	
	int          idx=0;	
	CString      str;
	int          Mode=0;

	idx = 0;
	JetAPI::ClearCombox(Combox);	
	
	Mode = PLC_TOWER_LIGHT_NORMAL;
	str = FormTowerLightModelText(Mode);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, (DWORD_PTR)Mode);
	idx ++;

	Mode = PLC_TOWER_LIGHT_USER_DEFINE;
	str = FormTowerLightModelText(Mode);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, (DWORD_PTR)Mode);
	idx ++;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CPLC_Basic::BuildTowerLightStateCombox(CComboBox &Combox)//建立塔燈狀態
{
	size_t       i=0;
	int          idx=0;	
	CString      str;
	int          State=0;

	idx = 0;
	JetAPI::ClearCombox(Combox);	
	
	State = PLC_TOWER_LIGHT_TURN_ON_RED;
	str = FormTowerLightStateText(State);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, (DWORD_PTR)State);
	idx ++;

	State = PLC_TOWER_LIGHT_TURN_ON_YELLOW;
	str = FormTowerLightStateText(State);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, (DWORD_PTR)State);
	idx ++;
	
	State = PLC_TOWER_LIGHT_TURN_ON_GREEN;
	str = FormTowerLightStateText(State);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, (DWORD_PTR)State);
	idx ++;

	State = PLC_TOWER_LIGHT_FLASH_RED;
	str = FormTowerLightStateText(State);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, (DWORD_PTR)State);
	idx ++;

	State = PLC_TOWER_LIGHT_FLASH_YELLOW;
	str = FormTowerLightStateText(State);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, (DWORD_PTR)State);
	idx ++;

	State = PLC_TOWER_LIGHT_FLASH_GREEN;
	str = FormTowerLightStateText(State);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, (DWORD_PTR)State);
	idx ++;

	State = PLC_TOWER_LIGHT_TURN_ON_YELLOW_GREEN;
	str = FormTowerLightStateText(State);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, (DWORD_PTR)State);
	idx ++;

	return true;
}
//-------------------------------------------------------------------------------------//
CPLC_Basic::CPLC_Basic()
{
	::InitializeCriticalSection(&m_csPLC);
	CPLC_Basic::PreInitPLC();
}
//-------------------------------------------------------------------------------------//
CPLC_Basic::~CPLC_Basic()
{
	::DeleteCriticalSection(&m_csPLC);	
}
//-------------------------------------------------------------------------------------//
void CPLC_Basic::LockPLC()
{
	::EnterCriticalSection(&m_csPLC);
}
//-------------------------------------------------------------------------------------//
void CPLC_Basic::UnlockPLC()
{
	::LeaveCriticalSection(&m_csPLC);
}
//-------------------------------------------------------------------------------------//
bool CPLC_Basic::PreInitPLC()
{	
	m_PLCConnectParam = _T("3");	
	
	m_PLCVersionI = 0;
	m_PLCIsConnected = false;

	m_PLCPollingThreadState = THREAD_STATE_NONE;
	m_PLCPollingThreadCmd = THREAD_COMMAND_TO_NONE;
	m_PLCPollingSleepTime = 50;	
	m_PLCPollingLaneAdjustSensor = false;//追蹤軌道調整感測器
	m_PLCNodeCurrentIndex = -1;	
	
	m_HardwareByPass_LA = false;
	m_HardwareByPass_LB = false;
	m_PLC_RightIn = false;
	m_PLC_FanAlarm = false;
	m_PLC_SafetyAlarm = false;
	m_PLC_SafetyAlarmEMS = false;
	m_DualLaneMode = false;//雙軌道模式
	m_DualTowerLight = false;//雙塔燈模式
	m_LastStationLineMode = LAST_STATION_LINE_MODE_4;
	m_ConveryerSensorPower_LA = true;
	m_ConveryerSensorPower_LB = true;

	//輸送帶A軌的感測器	
	m_ConveryerEnabled_LA = false;
	m_ConveryerClamp_LA = false;
	m_ConveryerStopBar_LA = false;
	m_PLC_I_SensorPCBIn_LA = false;
	m_PLC_I_SensorPCBOut_LA = false;
	m_PLC_I_SensorPCBStop_LA = false;
	m_PLC_I_SensorPCBSlow_LA = false;	
	m_PLC_I_SensorPCBStop2_LA = false;
	m_PLC_I_SignalFromLast_LA = false;
	m_PLC_I_SignalFromNext_LA = false;	
	m_PLC_O_SignalLockStation_LA = false;
	m_PLC_O_SignalToLast_LA = false;
	m_PLC_O_SignalToNext_LA = false;
	m_PLC_O_SignalToNextOK_LA = false;
	m_PLC_O_SignalToNextNG_LA = false;
	m_PLC_O_PCBBargeIn_LA = false;
	m_PLC_O_ConveryerStatus_LA = PLC_CONVERYER_STATUS_STOP;

	//輸送帶B軌的感測器	
	m_ConveryerEnabled_LB = false;
	m_ConveryerClamp_LB = false;
	m_ConveryerStopBar_LB = false;
	m_PLC_I_SensorPCBIn_LB = false;
	m_PLC_I_SensorPCBOut_LB = false;
	m_PLC_I_SensorPCBStop_LB = false;
	m_PLC_I_SensorPCBSlow_LB = false;	
	m_PLC_I_SensorPCBStop2_LB = false;
	m_PLC_I_SignalFromLast_LB = false;
	m_PLC_I_SignalFromNext_LB = false;
	m_PLC_O_SignalLockStation_LB = false;
	m_PLC_O_SignalToLast_LB = false;
	m_PLC_O_SignalToNext_LB = false;
	m_PLC_O_SignalToNextOK_LB = false;
	m_PLC_O_SignalToNextNG_LB = false;
	m_PLC_O_PCBBargeIn_LB = false;
	m_PLC_O_ConveryerStatus_LB = PLC_CONVERYER_STATUS_STOP;
	
	//現況訊號
	this->m_PLC_I_AirLost = false;
	this->m_PLC_I_FrontCapOpened = false;
	this->m_PLC_I_RearCapOpened = false;
	this->m_PLC_I_KeySwitchOff = false;
	this->m_PLC_O_LightStop = false;
	this->m_PLC_O_LightStart = false;
	this->m_PLC_I_EMSOn = false;
	m_PLC_I_BtnStop = false;	
	m_PLC_I_BtnStart = false;
	m_PLC_I_BtnReset = false;
	m_PLC_I_OverHeat = false;
	m_PLC_I_FanAlarm = false;

	//電源開關
	this->m_PLC_O_PowerACMotor = false;
	m_PLC_O_LightDay = false;
	//安全檢測
	this->m_SaftyBypass = false;

	//異常
	m_PLCExecAlarm = false;
	this->m_StageAlarm_LA = false;
	this->m_InspectionAlarm_LA = false;
	this->m_StageAlarm_LB = false;
	this->m_InspectionAlarm_LB = false;

	//塔燈
	m_PLC_O_Buzzer_LA = false;
	m_PLC_O_LightTowerRed_LA = false;
	m_PLC_O_LightTowerYel_LA = false;
	m_PLC_O_LightTowerGrn_LA = false;
	m_PLC_O_Buzzer_LB = false;
	m_PLC_O_LightTowerRed_LB = false;
	m_PLC_O_LightTowerYel_LB = false;
	m_PLC_O_LightTowerGrn_LB = false;

	m_TowerLightState_Stop_LA = 0;
	m_TowerLightState_Stop_LB = 0;

	m_PLCParameter = TPLCParameter();
	
	m_PCBAutoRunMode_LA = PLC_PCB_AUTO_RUN_STOP;//自動進出板模式
	this->m_PCBInCheckCount_LA = 0;//進板確認次數
	this->m_PCBOutCheckCount_LA = 0;//出板確認次數
	this->m_PCBBackCheckCount_LA = 0;//回板確認次數
	this->m_PCBOutInCheckCount_LA = 0;//出板帶進板的確認次數

	m_PCBAutoRunMode_LB = PLC_PCB_AUTO_RUN_STOP;//自動進出板模式
	this->m_PCBInCheckCount_LB = 0;//進板確認次數
	this->m_PCBOutCheckCount_LB = 0;//出板確認次數
	this->m_PCBBackCheckCount_LB = 0;//回板確認次數
	this->m_PCBOutInCheckCount_LB = 0;//出板帶進板的確認次數

	this->m_WaitingLastStationTimeStart_LA = 0;	//等待前站訊號開始
	this->m_WaitingLastStationTimeEnd_LA = 0;	//等待前站訊號結束
	this->m_WaitingNextStationTimeStart_LA = 0;	//等待後站訊號開始
	this->m_WaitingNextStationTimeEnd_LA = 0;	//等待後站訊號結束
	this->m_WaitingLastStationTimeStart_LB = 0;	//等待前站訊號開始
	this->m_WaitingLastStationTimeEnd_LB = 0;	//等待前站訊號結束
	this->m_WaitingNextStationTimeStart_LB = 0;	//等待後站訊號開始
	this->m_WaitingNextStationTimeEnd_LB = 0;	//等待後站訊號結束

	this->m_PCBBackTimeStart_LA = 0;//回板時間開始
	this->m_PCBBackTimeEnd_LA = 0;//回板時間結束
	this->m_PCBInTimeStart_LA = 0;//進板時間開始
	this->m_PCBInTimeEnd_LA = 0;//進板時間結束
	this->m_PCBOutTimeStart_LA = 0;//出版時間開始
	this->m_PCBOutTimeEnd_LA = 0;//出版時間結束

	this->m_PCBBackTimeStart_LB = 0;//回板時間開始
	this->m_PCBBackTimeEnd_LB = 0;//回板時間結束
	this->m_PCBInTimeStart_LB = 0;//進板時間開始
	this->m_PCBInTimeEnd_LB = 0;//進板時間結束
	this->m_PCBOutTimeStart_LB = 0;//出版時間開始
	this->m_PCBOutTimeEnd_LB = 0;//出版時間結束
	
	m_LaneAdjust_Fixed14Lane=1;//軌道調整-14軌固定
	m_LaneAdjust_HomeTimeout = 600;//軌道調整-歸零逾時
	m_LaneAdjust_MoveTimeout = 200;//軌道調整-移動逾時	

	m_LaneAdjust_Disable_LA = true;
	m_LaneAdjust_SkewPitch_LA = 10000;//軌道調整-螺紋間距-LA
	m_LaneAdjust_SensorORG_LA = false;//軌道調整-歸零感應器-LA
	m_LaneAdjust_SensorLimit_LA = false;//軌道調整-極限感應器-LA
	m_LaneAdjust_LimitMax_LA = -1;	//軌道調整-最大位置-LA
	m_LaneAdjust_LimitMin_LA = -1;	//軌道調整-最小位置-LA
	m_LaneAdjust_Homed_LA = false;	//軌道調整-是否歸零過-LA
	m_LaneAdjust_MoveSpeed_LA = 800; //軌道調整-移動速度-LA
	m_LaneAdjust_JogMoving_LA = false;//軌道調整-Jog移動中-LA
	m_LaneAdjust_JogSlowSpeed_LA = 300;//軌道調整-搖桿速度-慢-LA
	m_LaneAdjust_JogFastSpeed_LA = 800;//軌道調整-搖桿速度-快-LA
	m_LaneAdjust_HomePos_LA = 48;//軌道調整-歸零位置-LA
	m_LaneAdjust_CurrentPos_LA = 0;//軌道調整-現金位置-LA
	
	m_LaneAdjust_Disable_LB = true;
	m_LaneAdjust_SkewPitch_LB = 10000;//軌道調整-螺紋間距-LB
	m_LaneAdjust_SensorORG_LB = false;//軌道調整-歸零感應器-LB
	m_LaneAdjust_SensorLimit_LB = false;//軌道調整-極限感應器-LB
	m_LaneAdjust_LimitMax_LB = -1;  //軌道調整-最大位置-LB
	m_LaneAdjust_LimitMin_LB = -1;  //軌道調整-最小位置-LB
	m_LaneAdjust_Homed_LB = false;     //軌道調整-是否歸零過-LB
	m_LaneAdjust_MoveSpeed_LB = 800; //軌道調整-移動速度-LB
	m_LaneAdjust_JogMoving_LB = false;//軌道調整-Jog移動中-LB
	m_LaneAdjust_JogSlowSpeed_LB = 300;//軌道調整-搖桿速度-慢-LB
	m_LaneAdjust_JogFastSpeed_LB = 800;//軌道調整-搖桿速度-快-LB
	m_LaneAdjust_HomePos_LB = 48;     //軌道調整-歸零位置-LB	
	m_LaneAdjust_CurrentPos_LB = 0;//軌道調整-現金位置-LB

	this->LoadPLCINIParameter();
	this->SavePLCINIParameter();
	return true;
}
//-------------------------------------------------------------------------------------//
void CPLC_Basic::SetPCBAutoRunMode_LA(int Mode)//自動進出板模式
{
	m_PCBAutoRunMode_LA = Mode;
}
//-------------------------------------------------------------------------------------//
void CPLC_Basic::SetPCBAutoRunMode_LB(int Mode)//自動進出板模式
{
	m_PCBAutoRunMode_LB = Mode;
}
//-------------------------------------------------------------------------------------//
void CPLC_Basic::SetConveryerSensorPower_LA(bool val)//設定軌道感測器電源-A軌
{ 
	m_ConveryerSensorPower_LA=val; 
}
//-------------------------------------------------------------------------------------//
bool CPLC_Basic::GetConveryerSensorPower_LA() const //取得軌道感測器電源-A軌
{ 
	return m_ConveryerSensorPower_LA; 
}
//-------------------------------------------------------------------------------------//
void CPLC_Basic::SetConveryerSensorPower_LB(bool val)//設定軌道感測器電源-B軌 
{ 
	m_ConveryerSensorPower_LB=val; 
}
//-------------------------------------------------------------------------------------//
bool CPLC_Basic::GetConveryerSensorPower_LB() const //取得軌道感測器電源-B軌
{ 
	return m_ConveryerSensorPower_LB; 
}
//-------------------------------------------------------------------------------------//
int CPLC_Basic::CheckTowerLightState(int State)//確認塔燈設定狀態
{
	int NewState = State;
	switch ( State )
	{
	case PLC_TOWER_LIGHT_TURN_ON_RED:
	case PLC_TOWER_LIGHT_TURN_ON_YELLOW:
	case PLC_TOWER_LIGHT_TURN_ON_GREEN:
	case PLC_TOWER_LIGHT_FLASH_RED:
	case PLC_TOWER_LIGHT_FLASH_YELLOW:
	case PLC_TOWER_LIGHT_FLASH_GREEN:
	case PLC_TOWER_LIGHT_TURN_ON_YELLOW_GREEN:
		break;
	default:
		NewState = PLC_TOWER_LIGHT_TURN_ON_GREEN;
		break;
	}
	return NewState;
}
//-----------------------------------------------------------------------//
bool CPLC_Basic::ReturnPLCNotSupportFunc(LPCTSTR fnName)//回傳PLC未支援函式
{
	m_ErrorString.Format(_T("Error, PLC Not Support Func [%s]"), fnName);
	return false;
}
//-----------------------------------------------------------------------//
bool CPLC_Basic::SaveINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pString, LPCTSTR pfilename, CString &Error)
{
	if ( JetAPI::SaveINIData(pSection, pKeyName, pString, pfilename, Error) == false ) 
	{	return false;	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CPLC_Basic::LoadINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pDefault, TCHAR *pString, int StringSize, LPCTSTR pfilename, bool IsCheckLens, CString &Error)
{
	if ( JetAPI::LoadINIData(pSection, pKeyName, pDefault, pString, StringSize, pfilename, IsCheckLens, Error) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CPLC_Basic::SavePLCINIParameter()
{	
	if ( SavePLCINIParameterFn() == false )
	{	
		SetPLCExceptionCode_FileWrite();	
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CPLC_Basic::SavePLCINIParameterFn()
{
#ifndef PLC_OBJ_DISABLE

	CString str;	
	CString Section;
	CString KeyName;
	CString String;
	CString INIfilename;	
	CString ShortName = AOI3D_APP_CAT("PLC.INI");
	const TPLCParameter &Param = GetPLCParameter();

	INIfilename.Format(_T("%s\\%s"), AOIDataCollect.GetAOIDirectory(), ShortName);

	Section.Format(_T("Basic Setting"));

	KeyName.Format(_T("Connect Port"));//連線通訊埠
	String.Format(_T("%s"), this->m_PLCConnectParam);
	if ( SaveINIData(Section, KeyName, String, INIfilename, this->m_ErrorString) == false )
	{	return false; }	

	KeyName.Format(_T("Communication Delay Time"));//溝通的等待時間	
	String.Format(_T("%d"), Param.m_PLCCommDelayTime);
	if ( SaveINIData(Section, KeyName, String, INIfilename, this->m_ErrorString) == false )
	{	return false; }	

	KeyName.Format(_T("Check PCB Inside Before PCB-In"));//進板前確認機台是否有板	
	String.Format(_T("%d"), Param.m_CheckPCBInsideBeforePCBIn);
	if ( SaveINIData(Section, KeyName, String, INIfilename, this->m_ErrorString) == false )
	{	return false; }		
	
	KeyName.Format(_T("PCB In Out Timeout Lane A"));//進出板愈時-100ms
	String.Format(_T("%d"), Param.m_PCBInOutTimeout_LA);
	if ( SaveINIData(Section, KeyName, String, INIfilename, this->m_ErrorString) == false )
	{	return false; }	

	KeyName.Format(_T("PCB In Out Timeout Lane B"));//進出板愈時-100ms
	String.Format(_T("%d"), Param.m_PCBInOutTimeout_LB);
	if ( SaveINIData(Section, KeyName, String, INIfilename, this->m_ErrorString) == false )
	{	return false; }

	KeyName.Format(_T("Air Lost Check Time"));//氣壓不足愈時-5ms
	String.Format(_T("%d"), Param.m_AirLostCheckTime);
	if ( SaveINIData(Section, KeyName, String, INIfilename, this->m_ErrorString) == false )
	{	return false; }	

	KeyName.Format(_T("PCB Stop Delay Time"));//碰觸定位Sensor後皮帶持續運轉多久時間
	String.Format(_T("%d"), Param.m_PCBStopDelayTime);
	if ( SaveINIData(Section, KeyName, String, INIfilename, this->m_ErrorString) == false )
	{	return false; }	

	KeyName.Format(_T("Clamp On Off Delay Time"));//維修模式時-汽缸送氣愈時-0.5s
	String.Format(_T("%d"), Param.m_ClampOnOffDelayTime);
	if ( SaveINIData(Section, KeyName, String, INIfilename, this->m_ErrorString) == false )
	{	return false; }	
	
	KeyName.Format(_T("PCB Out With In Delay Time Lane A"));//同出同進延遲時間-1s		
	String.Format(_T("%d"), Param.m_PCBOutWithInDelayTime_LA);
	if ( SaveINIData(Section, KeyName, String, INIfilename, this->m_ErrorString) == false )
	{	return false; }	

	KeyName.Format(_T("PCB Out With In Delay Time Lane B"));//同出同進延遲時間-1s		
	String.Format(_T("%d"), Param.m_PCBOutWithInDelayTime_LB);
	if ( SaveINIData(Section, KeyName, String, INIfilename, this->m_ErrorString) == false )
	{	return false; }		
	 
	KeyName.Format(_T("PCB Out Delay Time Lane A"));//出板後板延遲時間-1s		
	String.Format(_T("%d"), Param.m_PCBOutDelayTime_LA);
	if ( SaveINIData(Section, KeyName, String, INIfilename, this->m_ErrorString) == false )
	{	return false; }		

	KeyName.Format(_T("PCB Out Delay Time Lane B"));//出板後板延遲時間-1s		
	String.Format(_T("%d"), Param.m_PCBOutDelayTime_LB);
	if ( SaveINIData(Section, KeyName, String, INIfilename, this->m_ErrorString) == false )
	{	return false; }		

	KeyName.Format(_T("PCB Clear Timeout Lane A"));//PCB板清除時間-3s		
	String.Format(_T("%d"), Param.m_PCBClearTimeout_LA);
	if ( SaveINIData(Section, KeyName, String, INIfilename, this->m_ErrorString) == false )
	{	return false; }		

	KeyName.Format(_T("PCB Clear Timeout Lane B"));//PCB板清除時間-3s		
	String.Format(_T("%d"), Param.m_PCBClearTimeout_LB);
	if ( SaveINIData(Section, KeyName, String, INIfilename, this->m_ErrorString) == false )
	{	return false; }

	KeyName.Format(_T("Turn On Last Signal Delay Time"));//機台向上一站要板持續時間:1=100ms
	String.Format(_T("%d"), Param.m_TurnOnLastSignalDelayTime);
	if ( SaveINIData(Section, KeyName, String, INIfilename, this->m_ErrorString) == false )
	{	return false; }
	
	KeyName.Format(_T("Check PCB Double In Pitch Time"));//入料檢知頻寬設定(2個板間隔時間):1=100ms
	String.Format(_T("%d"), Param.m_CheckPCBDoubleInPitchTime);
	if ( SaveINIData(Section, KeyName, String, INIfilename, this->m_ErrorString) == false )
	{	return false; }

	KeyName.Format(_T("Check PCB Double In Board Time"));//入料檢知時間設定(單板最長時間):1=100ms
	String.Format(_T("%d"), Param.m_CheckPCBDoubleInBoardTime);
	if ( SaveINIData(Section, KeyName, String, INIfilename, this->m_ErrorString) == false )
	{	return false; }		

	KeyName.Format(_T("PCB-In Auto Off Stop Bar Lane A"));//PCB進出自動縮停止塊-A軌
	String.Format(_T("%d"), Param.m_PCBInAutoOffStopBar_LA);
	if ( SaveINIData(Section, KeyName, String, INIfilename, this->m_ErrorString) == false )
	{	return false; }

	KeyName.Format(_T("PCB-In Auto Off Stop Bar Lane B"));//PCB進出自動縮停止塊-B軌
	String.Format(_T("%d"), Param.m_PCBInAutoOffStopBar_LB);
	if ( SaveINIData(Section, KeyName, String, INIfilename, this->m_ErrorString) == false )
	{	return false; }

	KeyName.Format(_T("PCB Auto Run With Side Stop Lane A"));//PCB自動進出板帶停板邊-A軌
	String.Format(_T("%d"), Param.m_PCBAutoRunWithSideStop_LA);
	if ( SaveINIData(Section, KeyName, String, INIfilename, this->m_ErrorString) == false )
	{	return false; }
	
	KeyName.Format(_T("PCB Auto Run With Side Stop Lane B"));//PCB自動進出板帶停板邊-B軌
	String.Format(_T("%d"), Param.m_PCBAutoRunWithSideStop_LB);
	if ( SaveINIData(Section, KeyName, String, INIfilename, this->m_ErrorString) == false )
	{	return false; }

	KeyName.Format(_T("Turn Off Conveyer Sensor Power Lane A"));//關閉軌道感測器電源-A軌
	String.Format(_T("%d"), Param.m_TurnOffConveyerSensorPower_LA);
	if ( SaveINIData(Section, KeyName, String, INIfilename, this->m_ErrorString) == false )
	{	return false; }

	KeyName.Format(_T("Turn Off Conveyer Sensor Power Lane B"));//關閉軌道感測器電源-B軌
	String.Format(_T("%d"), Param.m_TurnOffConveyerSensorPower_LB);
	if ( SaveINIData(Section, KeyName, String, INIfilename, this->m_ErrorString) == false )
	{	return false; }

	KeyName.Format(_T("PCB Out With Clear OK NG Signal"));//PCB出板時清除OK, NG訊號
	String.Format(_T("%d"), Param.m_PCBOutWithClearOKNGSignal);
	if ( SaveINIData(Section, KeyName, String, INIfilename, this->m_ErrorString) == false )
	{	return false; }

	KeyName.Format(_T("Enable Check PCB Double In"));//啟用確認PCB重複進板
	String.Format(_T("%d"), Param.m_EnableCheckPCBDoubleIn);
	if ( SaveINIData(Section, KeyName, String, INIfilename, this->m_ErrorString) == false )
	{	return false; }

	KeyName.Format(_T("Enable Check PCB Barge In"));//啟用確認檢測時PCB板闖入
	String.Format(_T("%d"), Param.m_EnableCheckPCBBargeIn);
	if ( SaveINIData(Section, KeyName, String, INIfilename, this->m_ErrorString) == false )
	{	return false; }

	KeyName.Format(_T("Enable Fan Alarm"));//啟用風扇警報
	String.Format(_T("%d"), Param.m_EnabledFanAlarm);
	if ( SaveINIData(Section, KeyName, String, INIfilename, this->m_ErrorString) == false )
	{	return false; }	

	KeyName.Format(_T("Enable Open-Door Stop Power"));//啟用開門警報
	String.Format(_T("%d"), Param.m_EnabledOpenDoorStopPower);
	if ( SaveINIData(Section, KeyName, String, INIfilename, this->m_ErrorString) == false )
	{	return false; }	

	KeyName.Format(_T("Enable Stopper Sensor Lane A"));//啟用擋板塊感應器-A軌
	String.Format(_T("%d"), Param.m_EnabledStopperSensor_LA);
	if ( SaveINIData(Section, KeyName, String, INIfilename, this->m_ErrorString) == false )
	{	return false; }	

	KeyName.Format(_T("Enable Stopper Sensor Lane B"));//啟用擋板塊感應器-B軌
	String.Format(_T("%d"), Param.m_EnabledStopperSensor_LB);
	if ( SaveINIData(Section, KeyName, String, INIfilename, this->m_ErrorString) == false )
	{	return false; }	

	KeyName.Format(_T("Tower Light Mode"));//塔燈模式
	String.Format(_T("%d"), m_TowerLightMode);
	if ( SaveINIData(Section, KeyName, String, INIfilename, this->m_ErrorString) == false )
	{	return false; }	

	//------------------------------------------------------------------------------//
	//塔燈模式
	Section.Format(_T("Tower Light Setting"));	
	KeyName.Format(_T("Tower Light: Stop"));//塔燈-停止中
	String.Format(_T("%d"), m_TowerLightState_Stop);
	if ( SaveINIData(Section, KeyName, String, INIfilename, this->m_ErrorString) == false )
	{	return false; }	

	KeyName.Format(_T("Tower Light: Inspection"));//塔燈-檢測中
	String.Format(_T("%d"), m_TowerLightState_Inspection);
	if ( SaveINIData(Section, KeyName, String, INIfilename, this->m_ErrorString) == false )
	{	return false; }	
	
	KeyName.Format(_T("Tower Light: Bypass"));//塔燈-輸送帶模式
	String.Format(_T("%d"), m_TowerLightState_Bypass);
	if ( SaveINIData(Section, KeyName, String, INIfilename, this->m_ErrorString) == false )
	{	return false; }	
	
	KeyName.Format(_T("Tower Light: Wait for Last Station"));//塔燈-等待上一站訊號
	String.Format(_T("%d"), m_TowerLightState_WaitLast);
	if ( SaveINIData(Section, KeyName, String, INIfilename, this->m_ErrorString) == false )
	{	return false; }	

	KeyName.Format(_T("Tower Light: Wait for Next Station"));//塔燈-等待下一站訊號
	String.Format(_T("%d"), m_TowerLightState_WaitNext);
	if ( SaveINIData(Section, KeyName, String, INIfilename, this->m_ErrorString) == false )
	{	return false; }	
	
	KeyName.Format(_T("Tower Light: PCB In"));//塔燈-進板中
	String.Format(_T("%d"), m_TowerLightState_PCBIn);
	if ( SaveINIData(Section, KeyName, String, INIfilename, this->m_ErrorString) == false )
	{	return false; }	

	KeyName.Format(_T("Tower Light: PCB Out"));//塔燈-出板中
	String.Format(_T("%d"), m_TowerLightState_PCBOut);
	if ( SaveINIData(Section, KeyName, String, INIfilename, this->m_ErrorString) == false )
	{	return false; }	

	KeyName.Format(_T("Tower Light: PCB Back"));//塔燈-回板中
	String.Format(_T("%d"), m_TowerLightState_PCBBack);
	if ( SaveINIData(Section, KeyName, String, INIfilename, this->m_ErrorString) == false )
	{	return false; }
	//------------------------------------------------------------------------------//	
	Section.Format(_T("Lane Width Adjust"));	

	KeyName.Format(_T("Lane Adjust Fixed 1-4 Lane"));//自動調間軌道的14軌固定
	String.Format(_T("%d"), m_LaneAdjust_Fixed14Lane);
	if ( SaveINIData(Section, KeyName, String, INIfilename, this->m_ErrorString) == false )
	{	return false; }

	KeyName.Format(_T("Lane Adjust Home Timeout"));//自動調間軌道的歸零逾時
	String.Format(_T("%d"), m_LaneAdjust_HomeTimeout);
	if ( SaveINIData(Section, KeyName, String, INIfilename, this->m_ErrorString) == false )
	{	return false; }

	KeyName.Format(_T("Lane Adjust Move Timeout"));//自動調間軌道的移動逾時
	String.Format(_T("%d"), m_LaneAdjust_MoveTimeout);
	if ( SaveINIData(Section, KeyName, String, INIfilename, this->m_ErrorString) == false )
	{	return false; }	

	KeyName.Format(_T("Lane Adjust Skew Pitch LA"));//自動調間軌道的螺紋間距
	String.Format(_T("%.2f"), m_LaneAdjust_SkewPitch_LA);
	if ( SaveINIData(Section, KeyName, String, INIfilename, this->m_ErrorString) == false )
	{	return false; }

	KeyName.Format(_T("Lane Adjust Skew Pitch LB"));//自動調間軌道的螺紋間距
	String.Format(_T("%.2f"), m_LaneAdjust_SkewPitch_LB);
	if ( SaveINIData(Section, KeyName, String, INIfilename, this->m_ErrorString) == false )
	{	return false; }

	KeyName.Format(_T("Lane Adjust Limit Max LA"));//自動調間軌道2的最大範圍
	String.Format(_T("%.2f"), m_LaneAdjust_LimitMax_LA);
	if ( SaveINIData(Section, KeyName, String, INIfilename, this->m_ErrorString) == false )
	{	return false; }	

	KeyName.Format(_T("Lane Adjust Limit Max LB"));//自動調間軌道4的最大範圍
	String.Format(_T("%.2f"), m_LaneAdjust_LimitMax_LB);
	if ( SaveINIData(Section, KeyName, String, INIfilename, this->m_ErrorString) == false )
	{	return false; }	
	
	KeyName.Format(_T("Lane Adjust Limit Min LA"));//自動調間軌道2的最小範圍
	String.Format(_T("%.2f"), m_LaneAdjust_LimitMin_LA);
	if ( SaveINIData(Section, KeyName, String, INIfilename, this->m_ErrorString) == false )
	{	return false; }	

	KeyName.Format(_T("Lane Adjust Limit Min LB"));//自動調間軌道4的最小範圍
	String.Format(_T("%.2f"), m_LaneAdjust_LimitMin_LB);
	if ( SaveINIData(Section, KeyName, String, INIfilename, this->m_ErrorString) == false )
	{	return false; }	

	KeyName.Format(_T("Lane Adjust Move Speed LA"));//自動調間軌道2的Run速度
	String.Format(_T("%.2f"), m_LaneAdjust_MoveSpeed_LA);
	if ( SaveINIData(Section, KeyName, String, INIfilename, this->m_ErrorString) == false )
	{	return false; }	
	
	KeyName.Format(_T("Lane Adjust Jog Slow Speed LA"));//自動調間軌道2的Jog速度
	String.Format(_T("%.2f"), m_LaneAdjust_JogSlowSpeed_LA);
	if ( SaveINIData(Section, KeyName, String, INIfilename, this->m_ErrorString) == false )
	{	return false; }	

	KeyName.Format(_T("Lane Adjust Jog Fast Speed LA"));//自動調間軌道2的Jog速度
	String.Format(_T("%.2f"), m_LaneAdjust_JogFastSpeed_LA);
	if ( SaveINIData(Section, KeyName, String, INIfilename, this->m_ErrorString) == false )
	{	return false; }	

	KeyName.Format(_T("Lane Adjust Move Speed LB"));//自動調間軌道4的Run速度
	String.Format(_T("%.2f"), m_LaneAdjust_MoveSpeed_LB);
	if ( SaveINIData(Section, KeyName, String, INIfilename, this->m_ErrorString) == false )
	{	return false; }	

	KeyName.Format(_T("Lane Adjust Jog Slow Speed LB"));//自動調間軌道4的Jog速度
	String.Format(_T("%.2f"), m_LaneAdjust_JogSlowSpeed_LB);
	if ( SaveINIData(Section, KeyName, String, INIfilename, this->m_ErrorString) == false )
	{	return false; }	

	KeyName.Format(_T("Lane Adjust Jog Fast Speed LB"));//自動調間軌道2的Jog速度
	String.Format(_T("%.2f"), m_LaneAdjust_JogFastSpeed_LB);
	if ( SaveINIData(Section, KeyName, String, INIfilename, this->m_ErrorString) == false )
	{	return false; }

	KeyName.Format(_T("Lane Adjust Home Position LA"));//自動調間軌道2的原點位置
	String.Format(_T("%.2f"), m_LaneAdjust_HomePos_LA);
	if ( SaveINIData(Section, KeyName, String, INIfilename, this->m_ErrorString) == false )
	{	return false; }	

	KeyName.Format(_T("Lane Adjust Home Position LB"));//自動調間軌道4的原點位置
	String.Format(_T("%.2f"), m_LaneAdjust_HomePos_LB);
	if ( SaveINIData(Section, KeyName, String, INIfilename, this->m_ErrorString) == false )
	{	return false; }	
	
	//------------------------------------------------------------------------------//	
	
#endif//PLC_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CPLC_Basic::LoadPLCINIParameter()
{
	if ( LoadPLCINIParameterFn() == false )
	{
		SetPLCExceptionCode_FileRead();
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CPLC_Basic::LoadPLCINIParameterFn()
{
#ifndef PLC_OBJ_DISABLE	
	CString str;
	int     TempI=0;
	const size_t Strlen = 128;
	TCHAR    String[Strlen]=_T("");
	CString  Section=_T("");
	CString  KeyName=_T("");
	CString  Default=_T("");
	CString  INIfilename=("");	
	CString  ShortName = AOI3D_APP_CAT("PLC.INI");
	TPLCParameter &Param = GetPLCParameter();

	INIfilename.Format(_T("%s\\%s"), AOIDataCollect.GetAOIDirectory(), ShortName);

	Section.Format(_T("Basic Setting"));

	KeyName.Format(_T("Connect Port"));//連線通訊埠
	Default.Format(_T("%s"), this->m_PLCConnectParam);	
	if ( LoadINIData(Section, KeyName, Default, String, Strlen, INIfilename, false, this->m_ErrorString) == false )
	{	return false; }	
	this->m_PLCConnectParam = String;

	KeyName.Format(_T("Communication Delay Time"));//溝通的等待時間
	Default.Format(_T("%d"), Param.m_PLCCommDelayTime);
	if ( LoadINIData(Section, KeyName, Default, String, Strlen, INIfilename, false, this->m_ErrorString) == false )
	{	return false; }	
	Param.m_PLCCommDelayTime = ::_ttoi(String);
	if ( Param.m_PLCCommDelayTime < 0 ) { Param.m_PLCCommDelayTime = 0; }

	KeyName.Format(_T("Check PCB Inside Before PCB-In"));//進板前確認機台是否有板	
	Default.Format(_T("%d"), Param.m_CheckPCBInsideBeforePCBIn);
	if ( LoadINIData(Section, KeyName, Default, String, Strlen, INIfilename, false, this->m_ErrorString) == false )
	{	return false; }	
	TempI = ::_ttoi(String);
	if ( 0 == TempI ) { Param.m_CheckPCBInsideBeforePCBIn=FN_DISABLE; }
	else { Param.m_CheckPCBInsideBeforePCBIn = FN_ENABLE; }	
	
	KeyName.Format(_T("PCB In Out Timeout Lane A"));//進出板愈時-100ms
	Default.Format(_T("%d"), Param.m_PCBInOutTimeout_LA);
	if ( LoadINIData(Section, KeyName, Default, String, Strlen, INIfilename, false, this->m_ErrorString) == false )
	{	return false; }	
	Param.m_PCBInOutTimeout_LA = ::_ttoi(String);
	if ( Param.m_PCBInOutTimeout_LA < 0 ) { Param.m_PCBInOutTimeout_LA = 100; }

	KeyName.Format(_T("PCB In Out Timeout Lane B"));//進出板愈時-100ms
	Default.Format(_T("%d"), Param.m_PCBInOutTimeout_LB);
	if ( LoadINIData(Section, KeyName, Default, String, Strlen, INIfilename, false, this->m_ErrorString) == false )
	{	return false; }	
	Param.m_PCBInOutTimeout_LB = ::_ttoi(String);
	if ( Param.m_PCBInOutTimeout_LB < 0 ) { Param.m_PCBInOutTimeout_LB = 100; }

	KeyName.Format(_T("Air Lost Check Time"));//氣壓不足愈時-5ms
	Default.Format(_T("%d"), Param.m_AirLostCheckTime);
	if ( LoadINIData(Section, KeyName, Default, String, Strlen, INIfilename, false, this->m_ErrorString) == false )
	{	return false; }	
	Param.m_AirLostCheckTime = ::_ttoi(String);
	if ( Param.m_AirLostCheckTime < 0 ) { Param.m_AirLostCheckTime = 5; }

	KeyName.Format(_T("PCB Stop Delay Time"));//碰觸定位Sensor後皮帶持續運轉多久時間
	Default.Format(_T("%d"), Param.m_PCBStopDelayTime);
	if ( LoadINIData(Section, KeyName, Default, String, Strlen, INIfilename, false, this->m_ErrorString) == false )
	{	return false; }	
	Param.m_PCBStopDelayTime = ::_ttoi(String);
	if ( Param.m_PCBStopDelayTime < 0 ) { Param.m_PCBStopDelayTime = 3; }

	KeyName.Format(_T("Clamp On Off Delay Time"));//維修模式時-汽缸送氣愈時-0.5s
	Default.Format(_T("%d"), Param.m_ClampOnOffDelayTime);
	if ( LoadINIData(Section, KeyName, Default, String, Strlen, INIfilename, false, this->m_ErrorString) == false )
	{	return false; }	
	Param.m_ClampOnOffDelayTime = ::_ttoi(String);	
	if ( Param.m_ClampOnOffDelayTime < 0 ) { Param.m_ClampOnOffDelayTime = 10; }
	
	KeyName.Format(_T("PCB Out With In Delay Time Lane A"));//同出同進延遲時間-1s
	Default.Format(_T("%d"), Param.m_PCBOutWithInDelayTime_LA);
	if ( LoadINIData(Section, KeyName, Default, String, Strlen, INIfilename, false, this->m_ErrorString) == false )
	{	return false; }	
	Param.m_PCBOutWithInDelayTime_LA = ::_ttoi(String);
	if ( Param.m_PCBOutWithInDelayTime_LA < 0 ) { Param.m_PCBOutWithInDelayTime_LA = 1; }

	KeyName.Format(_T("PCB Out With In Delay Time Lane B"));//同出同進延遲時間-1s
	Default.Format(_T("%d"), Param.m_PCBOutWithInDelayTime_LB);
	if ( LoadINIData(Section, KeyName, Default, String, Strlen, INIfilename, false, this->m_ErrorString) == false )
	{	return false; }	
	Param.m_PCBOutWithInDelayTime_LB = ::_ttoi(String);
	if ( Param.m_PCBOutWithInDelayTime_LB < 0 ) { Param.m_PCBOutWithInDelayTime_LB = 1; }		
	
	KeyName.Format(_T("PCB Out Delay Time Lane A"));//出板後板延遲時間-1s		
	Default.Format(_T("%d"), Param.m_PCBOutDelayTime_LA);
	if ( LoadINIData(Section, KeyName, Default, String, Strlen, INIfilename, false, this->m_ErrorString) == false )
	{	return false; }	
	Param.m_PCBOutDelayTime_LA = ::_ttoi(String);
	if ( Param.m_PCBOutDelayTime_LA < 0 ) { Param.m_PCBOutDelayTime_LA = 1; }

	KeyName.Format(_T("PCB Out Delay Time Lane B"));//出板後板延遲時間-1s	
	Default.Format(_T("%d"), Param.m_PCBOutDelayTime_LB);
	if ( LoadINIData(Section, KeyName, Default, String, Strlen, INIfilename, false, this->m_ErrorString) == false )
	{	return false; }	
	Param.m_PCBOutDelayTime_LB = ::_ttoi(String);
	if ( Param.m_PCBOutDelayTime_LB < 0 ) { Param.m_PCBOutDelayTime_LB = 1; }

	KeyName.Format(_T("PCB Clear Timeout Lane A"));//PCB板清除時間-3s		
	Default.Format(_T("%d"), Param.m_PCBClearTimeout_LA);
	if ( LoadINIData(Section, KeyName, Default, String, Strlen, INIfilename, false, this->m_ErrorString) == false )
	{	return false; }	
	Param.m_PCBClearTimeout_LA = ::_ttoi(String);
	if ( Param.m_PCBClearTimeout_LA < 0 ) { Param.m_PCBClearTimeout_LA = 1; }

	KeyName.Format(_T("PCB Clear Timeout Lane B"));//PCB板清除時間-3s		
	Default.Format(_T("%d"), Param.m_PCBClearTimeout_LB);
	if ( LoadINIData(Section, KeyName, Default, String, Strlen, INIfilename, false, this->m_ErrorString) == false )
	{	return false; }	
	Param.m_PCBClearTimeout_LB = ::_ttoi(String);
	if ( Param.m_PCBClearTimeout_LB < 0 ) { Param.m_PCBClearTimeout_LB = 1; }

	KeyName.Format(_T("Turn On Last Signal Delay Time"));//機台向上一站要板持續時間:1=100ms	
	Default.Format(_T("%d"), Param.m_TurnOnLastSignalDelayTime);
	if ( LoadINIData(Section, KeyName, Default, String, Strlen, INIfilename, false, this->m_ErrorString) == false )
	{	return false; }	
	Param.m_TurnOnLastSignalDelayTime = ::_ttoi(String);
	if ( Param.m_TurnOnLastSignalDelayTime < 0 ) { Param.m_TurnOnLastSignalDelayTime = 1; }
	
	KeyName.Format(_T("Check PCB Double In Pitch Time"));//入料檢知頻寬設定(2個板間隔時間):1=100ms	
	Default.Format(_T("%d"), Param.m_CheckPCBDoubleInPitchTime);
	if ( LoadINIData(Section, KeyName, Default, String, Strlen, INIfilename, false, this->m_ErrorString) == false )
	{	return false; }	
	Param.m_CheckPCBDoubleInPitchTime = ::_ttoi(String);
	if ( Param.m_CheckPCBDoubleInPitchTime < 0 ) { Param.m_CheckPCBDoubleInPitchTime = 1; }

	KeyName.Format(_T("Check PCB Double In Board Time"));//入料檢知時間設定(單板最長時間):1=100ms	
	Default.Format(_T("%d"), Param.m_CheckPCBDoubleInBoardTime);
	if ( LoadINIData(Section, KeyName, Default, String, Strlen, INIfilename, false, this->m_ErrorString) == false )
	{	return false; }	
	Param.m_CheckPCBDoubleInBoardTime = ::_ttoi(String);
	if ( Param.m_CheckPCBDoubleInBoardTime < 0 ) { Param.m_CheckPCBDoubleInBoardTime = 1; }

	KeyName.Format(_T("PCB-In Auto Off Stop Bar Lane A"));//PCB進出自動縮停止塊-A軌
	Default.Format(_T("%d"), Param.m_PCBInAutoOffStopBar_LA);
	if ( LoadINIData(Section, KeyName, Default, String, Strlen, INIfilename, false, this->m_ErrorString) == false )
	{	return false; }	
	TempI = ::_ttoi(String);
	if ( 0 == TempI ) { Param.m_PCBInAutoOffStopBar_LA=FN_DISABLE; }
	else { Param.m_PCBInAutoOffStopBar_LA = FN_ENABLE; }	

	KeyName.Format(_T("PCB-In Auto Off Stop Bar Lane B"));//PCB進出自動縮停止塊-B軌
	Default.Format(_T("%d"), Param.m_PCBInAutoOffStopBar_LB);
	if ( LoadINIData(Section, KeyName, Default, String, Strlen, INIfilename, false, this->m_ErrorString) == false )
	{	return false; }	
	TempI = ::_ttoi(String);
	if ( 0 == TempI ) { Param.m_PCBInAutoOffStopBar_LB=FN_DISABLE; }
	else { Param.m_PCBInAutoOffStopBar_LB = FN_ENABLE; }	

	KeyName.Format(_T("PCB Auto Run With Side Stop Lane A"));//PCB自動進出板帶停板邊-A軌
	Default.Format(_T("%d"), Param.m_PCBAutoRunWithSideStop_LA);
	if ( LoadINIData(Section, KeyName, Default, String, Strlen, INIfilename, false, this->m_ErrorString) == false )
	{	return false; }	
	TempI = ::_ttoi(String);
	if ( 0 == TempI ) { Param.m_PCBAutoRunWithSideStop_LA=FN_DISABLE; }
	else { Param.m_PCBAutoRunWithSideStop_LA = FN_ENABLE; }	

	KeyName.Format(_T("PCB Auto Run With Side Stop Lane B"));//PCB自動進出板帶停板邊-B軌
	Default.Format(_T("%d"), Param.m_PCBAutoRunWithSideStop_LB);
	if ( LoadINIData(Section, KeyName, Default, String, Strlen, INIfilename, false, this->m_ErrorString) == false )
	{	return false; }	
	TempI = ::_ttoi(String);
	if ( 0 == TempI ) { Param.m_PCBAutoRunWithSideStop_LB=FN_DISABLE; }
	else { Param.m_PCBAutoRunWithSideStop_LB = FN_ENABLE; }	

	KeyName.Format(_T("Turn Off Conveyer Sensor Power Lane A"));//關閉軌道感測器電源-A軌
	Default.Format(_T("%d"), Param.m_TurnOffConveyerSensorPower_LA);
	if ( LoadINIData(Section, KeyName, Default, String, Strlen, INIfilename, false, this->m_ErrorString) == false )
	{	return false; }	
	TempI = ::_ttoi(String);
	if ( 0 == TempI ) { Param.m_TurnOffConveyerSensorPower_LA=FN_DISABLE; }
	else { Param.m_TurnOffConveyerSensorPower_LA = FN_ENABLE; }	

	KeyName.Format(_T("Turn Off Conveyer Sensor Power Lane B"));//關閉軌道感測器電源-B軌
	Default.Format(_T("%d"), Param.m_TurnOffConveyerSensorPower_LB);
	if ( LoadINIData(Section, KeyName, Default, String, Strlen, INIfilename, false, this->m_ErrorString) == false )
	{	return false; }	
	TempI = ::_ttoi(String);
	if ( 0 == TempI ) { Param.m_TurnOffConveyerSensorPower_LB=FN_DISABLE; }
	else { Param.m_TurnOffConveyerSensorPower_LB = FN_ENABLE; }	

	KeyName.Format(_T("PCB Out With Clear OK NG Signal"));//PCB出板時清除OK, NG訊號
	Default.Format(_T("%d"), Param.m_PCBOutWithClearOKNGSignal);
	if ( LoadINIData(Section, KeyName, Default, String, Strlen, INIfilename, false, this->m_ErrorString) == false )
	{	return false; }	
	TempI = ::_ttoi(String);
	if ( 0 == TempI ) { Param.m_PCBOutWithClearOKNGSignal=FN_DISABLE; }
	else { Param.m_PCBOutWithClearOKNGSignal = FN_ENABLE; }

	KeyName.Format(_T("Enable Check PCB Double In"));//啟用確認PCB重複進板
	Default.Format(_T("%d"), Param.m_EnableCheckPCBDoubleIn);
	if ( LoadINIData(Section, KeyName, Default, String, Strlen, INIfilename, false, this->m_ErrorString) == false )
	{	return false; }	
	TempI = ::_ttoi(String);
	if ( 0 == TempI ) { Param.m_EnableCheckPCBDoubleIn=FN_DISABLE; }
	else { Param.m_EnableCheckPCBDoubleIn = FN_ENABLE; }

	KeyName.Format(_T("Enable Check PCB Barge In"));//啟用確認檢測時PCB板闖入
	Default.Format(_T("%d"), Param.m_EnableCheckPCBBargeIn);
	if ( LoadINIData(Section, KeyName, Default, String, Strlen, INIfilename, false, this->m_ErrorString) == false )
	{	return false; }	
	TempI = ::_ttoi(String);
	if ( 0 == TempI ) { Param.m_EnableCheckPCBBargeIn=FN_DISABLE; }
	else { Param.m_EnableCheckPCBBargeIn = FN_ENABLE; }

	KeyName.Format(_T("Enable Fan Alarm"));//啟用風扇警報
	Default.Format(_T("%d"), Param.m_EnabledFanAlarm);
	if ( LoadINIData(Section, KeyName, Default, String, Strlen, INIfilename, false, this->m_ErrorString) == false )
	{	return false; }	
	TempI = ::_ttoi(String);
	if ( 0 == TempI ) { Param.m_EnabledFanAlarm=FN_DISABLE; }
	else { Param.m_EnabledFanAlarm = FN_ENABLE; }

	KeyName.Format(_T("Enable Open-Door Stop Power"));//啟用開門警報
	Default.Format(_T("%d"), Param.m_EnabledOpenDoorStopPower);
	if ( LoadINIData(Section, KeyName, Default, String, Strlen, INIfilename, false, this->m_ErrorString) == false )
	{	return false; }	
	TempI = ::_ttoi(String);
	if ( 0 == TempI ) { Param.m_EnabledOpenDoorStopPower=FN_DISABLE; }
	else { Param.m_EnabledOpenDoorStopPower = FN_ENABLE; }

	KeyName.Format(_T("Enable Stopper Sensor Lane A"));//啟用擋板塊感應器-A軌
	Default.Format(_T("%d"), Param.m_EnabledStopperSensor_LA);
	if ( LoadINIData(Section, KeyName, Default, String, Strlen, INIfilename, false, this->m_ErrorString) == false )
	{	return false; }	
	TempI = ::_ttoi(String);
	if ( 0 == TempI ) { Param.m_EnabledStopperSensor_LA=FN_DISABLE; }
	else { Param.m_EnabledStopperSensor_LA = FN_ENABLE; }

	KeyName.Format(_T("Enable Stopper Sensor Lane B"));//啟用擋板塊感應器-B軌
	Default.Format(_T("%d"), Param.m_EnabledStopperSensor_LB);
	if ( LoadINIData(Section, KeyName, Default, String, Strlen, INIfilename, false, this->m_ErrorString) == false )
	{	return false; }	
	TempI = ::_ttoi(String);
	if ( 0 == TempI ) { Param.m_EnabledStopperSensor_LB=FN_DISABLE; }
	else { Param.m_EnabledStopperSensor_LB = FN_ENABLE; }

	KeyName.Format(_T("Tower Light Mode"));//塔燈模式
	Default.Format(_T("%d"), m_TowerLightMode);
	if ( LoadINIData(Section, KeyName, Default, String, Strlen, INIfilename, false, this->m_ErrorString) == false )
	{	return false; }	
	this->m_TowerLightMode = ::_ttoi(String);
	switch ( m_TowerLightMode )
	{
	case PLC_TOWER_LIGHT_NORMAL:
	case PLC_TOWER_LIGHT_USER_DEFINE:
		break;
	default:
		this->m_TowerLightMode = PLC_TOWER_LIGHT_NORMAL;
		break;
	}	
	//------------------------------------------------------------------------------//
	//塔燈設定
	Section.Format(_T("Tower Light Setting"));	
	KeyName.Format(_T("Tower Light: Stop"));//塔燈-停止中
	Default.Format(_T("%d"), m_TowerLightState_Stop);
	if ( LoadINIData(Section, KeyName, Default, String, Strlen, INIfilename, false, this->m_ErrorString) == false )
	{	return false; }	
	this->m_TowerLightState_Stop = this->CheckTowerLightState(::_ttoi(String));		
	
	KeyName.Format(_T("Tower Light: Inspection"));//塔燈-檢測中
	Default.Format(_T("%d"), m_TowerLightState_Inspection);
	if ( LoadINIData(Section, KeyName, Default, String, Strlen, INIfilename, false, this->m_ErrorString) == false )
	{	return false; }	
	this->m_TowerLightState_Inspection = this->CheckTowerLightState(::_ttoi(String));	
	
	KeyName.Format(_T("Tower Light: Bypass"));//塔燈-輸送帶模式
	Default.Format(_T("%d"), m_TowerLightState_Bypass);
	if ( LoadINIData(Section, KeyName, Default, String, Strlen, INIfilename, false, this->m_ErrorString) == false )
	{	return false; }	
	this->m_TowerLightState_Bypass = this->CheckTowerLightState(::_ttoi(String));
	
	KeyName.Format(_T("Tower Light: Wait for Last Station"));//塔燈-等待上一站訊號
	Default.Format(_T("%d"), m_TowerLightState_WaitLast);
	if ( LoadINIData(Section, KeyName, Default, String, Strlen, INIfilename, false, this->m_ErrorString) == false )
	{	return false; }	
	this->m_TowerLightState_WaitLast = this->CheckTowerLightState(::_ttoi(String));	

	KeyName.Format(_T("Tower Light: Wait for Next Station"));//塔燈-等待下一站訊號
	Default.Format(_T("%d"), m_TowerLightState_WaitNext);
	if ( LoadINIData(Section, KeyName, Default, String, Strlen, INIfilename, false, this->m_ErrorString) == false )
	{	return false; }	
	this->m_TowerLightState_WaitNext = this->CheckTowerLightState(::_ttoi(String));

	KeyName.Format(_T("Tower Light: PCB In"));//塔燈-進板中
	Default.Format(_T("%d"), m_TowerLightState_PCBIn);
	if ( LoadINIData(Section, KeyName, Default, String, Strlen, INIfilename, false, this->m_ErrorString) == false )
	{	return false; }	
	this->m_TowerLightState_PCBIn = this->CheckTowerLightState(::_ttoi(String));	

	KeyName.Format(_T("Tower Light: PCB Out"));//塔燈-出板中
	Default.Format(_T("%d"), m_TowerLightState_PCBOut);
	if ( LoadINIData(Section, KeyName, Default, String, Strlen, INIfilename, false, this->m_ErrorString) == false )
	{	return false; }	
	this->m_TowerLightState_PCBOut = this->CheckTowerLightState(::_ttoi(String));	

	KeyName.Format(_T("Tower Light: PCB Back"));//塔燈-回板中
	Default.Format(_T("%d"), m_TowerLightState_PCBBack);
	if ( LoadINIData(Section, KeyName, Default, String, Strlen, INIfilename, false, this->m_ErrorString) == false )
	{	return false; }	
	this->m_TowerLightState_PCBBack = this->CheckTowerLightState(::_ttoi(String));	
	//------------------------------------------------------------------------------//	
	Section.Format(_T("Lane Width Adjust"));	

	KeyName.Format(_T("Lane Adjust Fixed 1-4 Lane"));//自動調間軌道的14軌固定
	Default.Format(_T("%d"), m_LaneAdjust_Fixed14Lane);
	if ( LoadINIData(Section, KeyName, Default, String, Strlen, INIfilename, false, this->m_ErrorString) == false )
	{	return false; }	
	m_LaneAdjust_Fixed14Lane = ::_ttoi(String);

	KeyName.Format(_T("Lane Adjust Home Timeout"));//自動調間軌道的歸零逾時
	Default.Format(_T("%d"), m_LaneAdjust_HomeTimeout);
	if ( LoadINIData(Section, KeyName, Default, String, Strlen, INIfilename, false, this->m_ErrorString) == false )
	{	return false; }	
	m_LaneAdjust_HomeTimeout = ::_ttoi(String);	

	KeyName.Format(_T("Lane Adjust Move Timeout"));//自動調間軌道的移動逾時	
	Default.Format(_T("%d"), m_LaneAdjust_MoveTimeout);
	if ( LoadINIData(Section, KeyName, Default, String, Strlen, INIfilename, false, this->m_ErrorString) == false )
	{	return false; }	
	m_LaneAdjust_MoveTimeout = ::_ttoi(String);		 

	KeyName.Format(_T("Lane Adjust Skew Pitch LA"));//自動調間軌道的螺紋間距
	Default.Format(_T("%.2f"), m_LaneAdjust_SkewPitch_LA);
	if ( LoadINIData(Section, KeyName, Default, String, Strlen, INIfilename, false, this->m_ErrorString) == false )
	{	return false; }	
	m_LaneAdjust_SkewPitch_LA = ::_tcstod(String, NULL);	

	KeyName.Format(_T("Lane Adjust Skew Pitch LB"));//自動調間軌道的螺紋間距
	Default.Format(_T("%.2f"), m_LaneAdjust_SkewPitch_LB);
	if ( LoadINIData(Section, KeyName, Default, String, Strlen, INIfilename, false, this->m_ErrorString) == false )
	{	return false; }	
	m_LaneAdjust_SkewPitch_LB = ::_tcstod(String, NULL);	

	KeyName.Format(_T("Lane Adjust Limit Max LA"));//自動調間軌道2的最大範圍
	Default.Format(_T("%.2f"), m_LaneAdjust_LimitMax_LA);
	if ( LoadINIData(Section, KeyName, Default, String, Strlen, INIfilename, false, this->m_ErrorString) == false )
	{	return false; }	
	this->m_LaneAdjust_LimitMax_LA = ::_tcstod(String, NULL);
	if ( this->m_LaneAdjust_LimitMax_LA < 0 ) { this->m_LaneAdjust_LimitMax_LA = 300; }
	else if ( this->m_LaneAdjust_LimitMax_LA < JET_PLC_LANE_ADJUST_LIMIT_MIN ) 
	{ this->m_LaneAdjust_LimitMax_LA = JET_PLC_LANE_ADJUST_LIMIT_MIN; }	

	KeyName.Format(_T("Lane Adjust Limit Max LB"));//自動調間軌道4的最大範圍
	Default.Format(_T("%.2f"), m_LaneAdjust_LimitMax_LB);
	if ( LoadINIData(Section, KeyName, Default, String, Strlen, INIfilename, false, this->m_ErrorString) == false )
	{	return false; }	
	this->m_LaneAdjust_LimitMax_LB = ::_tcstod(String, NULL);
	if ( this->m_LaneAdjust_LimitMax_LB < 0 ) { this->m_LaneAdjust_LimitMax_LB = 300; }
	else if ( this->m_LaneAdjust_LimitMax_LB < JET_PLC_LANE_ADJUST_LIMIT_MIN ) 
	{ this->m_LaneAdjust_LimitMax_LB = JET_PLC_LANE_ADJUST_LIMIT_MIN; }
	
	KeyName.Format(_T("Lane Adjust Limit Min LA"));//自動調間軌道2的最小範圍
	Default.Format(_T("%.2f"), m_LaneAdjust_LimitMin_LA);
	if ( LoadINIData(Section, KeyName, Default, String, Strlen, INIfilename, false, this->m_ErrorString) == false )
	{	return false; }	
	this->m_LaneAdjust_LimitMin_LA = ::_tcstod(String, NULL);
	if ( this->m_LaneAdjust_LimitMin_LA < 0 ) { this->m_LaneAdjust_LimitMin_LA = JET_PLC_LANE_ADJUST_LIMIT_MIN; }
	else if ( this->m_LaneAdjust_LimitMin_LA < JET_PLC_LANE_ADJUST_LIMIT_MIN ) 
	{ this->m_LaneAdjust_LimitMin_LA = JET_PLC_LANE_ADJUST_LIMIT_MIN; }	

	KeyName.Format(_T("Lane Adjust Limit Min LB"));//自動調間軌道4的最小範圍
	Default.Format(_T("%.2f"), m_LaneAdjust_LimitMin_LB);
	if ( LoadINIData(Section, KeyName, Default, String, Strlen, INIfilename, false, this->m_ErrorString) == false )
	{	return false; }	
	this->m_LaneAdjust_LimitMin_LB = ::_tcstod(String, NULL);
	if ( this->m_LaneAdjust_LimitMin_LB < 0 ) { this->m_LaneAdjust_LimitMin_LB = JET_PLC_LANE_ADJUST_LIMIT_MIN; }
	else if ( this->m_LaneAdjust_LimitMin_LB < JET_PLC_LANE_ADJUST_LIMIT_MIN ) 
	{ this->m_LaneAdjust_LimitMin_LB = JET_PLC_LANE_ADJUST_LIMIT_MIN; }		
	
	KeyName.Format(_T("Lane Adjust Move Speed LA"));//自動調間軌道2的Run速度
	Default.Format(_T("%.2f"), m_LaneAdjust_MoveSpeed_LA);
	if ( LoadINIData(Section, KeyName, Default, String, Strlen, INIfilename, false, this->m_ErrorString) == false )
	{	return false; }	
	this->m_LaneAdjust_MoveSpeed_LA = ::_tcstod(String, NULL);
	if ( this->m_LaneAdjust_MoveSpeed_LA < 0 ) { this->m_LaneAdjust_MoveSpeed_LA = 800; }

	KeyName.Format(_T("Lane Adjust Move Speed LB"));//自動調間軌道2的Run速度
	Default.Format(_T("%.2f"), m_LaneAdjust_MoveSpeed_LB);
	if ( LoadINIData(Section, KeyName, Default, String, Strlen, INIfilename, false, this->m_ErrorString) == false )
	{	return false; }	
	this->m_LaneAdjust_MoveSpeed_LB = ::_tcstod(String, NULL);
	if ( this->m_LaneAdjust_MoveSpeed_LB < 0 ) { this->m_LaneAdjust_MoveSpeed_LB = 800; }

	KeyName.Format(_T("Lane Adjust Jog Slow Speed LA"));//自動調間軌道2的Jog速度
	Default.Format(_T("%.2f"), m_LaneAdjust_JogSlowSpeed_LA);
	if ( LoadINIData(Section, KeyName, Default, String, Strlen, INIfilename, false, this->m_ErrorString) == false )
	{	return false; }	
	this->m_LaneAdjust_JogSlowSpeed_LA = ::_tcstod(String, NULL);
	if ( this->m_LaneAdjust_JogSlowSpeed_LA < 0 ) { this->m_LaneAdjust_JogSlowSpeed_LA = 50; }

	KeyName.Format(_T("Lane Adjust Jog Fast Speed LA"));//自動調間軌道2的Jog速度
	Default.Format(_T("%.2f"), m_LaneAdjust_JogFastSpeed_LA);
	if ( LoadINIData(Section, KeyName, Default, String, Strlen, INIfilename, false, this->m_ErrorString) == false )
	{	return false; }	
	this->m_LaneAdjust_JogFastSpeed_LA = ::_tcstod(String, NULL);
	if ( this->m_LaneAdjust_JogFastSpeed_LA < 0 ) { this->m_LaneAdjust_JogFastSpeed_LA = 300; }

	KeyName.Format(_T("Lane Adjust Jog Slow Speed LB"));//自動調間軌道4的Jog速度
	Default.Format(_T("%.2f"), m_LaneAdjust_JogSlowSpeed_LB);
	if ( LoadINIData(Section, KeyName, Default, String, Strlen, INIfilename, false, this->m_ErrorString) == false )
	{	return false; }	
	this->m_LaneAdjust_JogSlowSpeed_LB = ::_tcstod(String, NULL);
	if ( this->m_LaneAdjust_JogSlowSpeed_LB < 0 ) { this->m_LaneAdjust_JogSlowSpeed_LB = 50; }	

	KeyName.Format(_T("Lane Adjust Jog Fast Speed LB"));//自動調間軌道2的Jog速度
	Default.Format(_T("%.2f"), m_LaneAdjust_JogFastSpeed_LB);
	if ( LoadINIData(Section, KeyName, Default, String, Strlen, INIfilename, false, this->m_ErrorString) == false )
	{	return false; }	
	this->m_LaneAdjust_JogFastSpeed_LB = ::_tcstod(String, NULL);
	if ( this->m_LaneAdjust_JogFastSpeed_LB < 0 ) { this->m_LaneAdjust_JogFastSpeed_LB = 300; }

	KeyName.Format(_T("Lane Adjust Home Position LA"));//自動調間軌道2的原點位置
	Default.Format(_T("%.2f"), m_LaneAdjust_HomePos_LA);
	if ( LoadINIData(Section, KeyName, Default, String, Strlen, INIfilename, false, this->m_ErrorString) == false )
	{	return false; }	
	this->m_LaneAdjust_HomePos_LA = ::_tcstod(String, NULL);
	if ( this->m_LaneAdjust_HomePos_LA < 0 ) { this->m_LaneAdjust_HomePos_LA = 48; }

	KeyName.Format(_T("Lane Adjust Home Position LB"));//自動調間軌道4的原點位置
	Default.Format(_T("%.2f"), m_LaneAdjust_HomePos_LB);
	if ( LoadINIData(Section, KeyName, Default, String, Strlen, INIfilename, false, this->m_ErrorString) == false )
	{	return false; }	
	this->m_LaneAdjust_HomePos_LB = ::_tcstod(String, NULL);
	if ( this->m_LaneAdjust_HomePos_LB < 0 ) { this->m_LaneAdjust_HomePos_LB = 48; }
	//------------------------------------------------------------------------------//
	
#endif//PLC_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CPLC_Basic::SavePLCMovingTimeMsg(const char *pContext)//儲存PLC移動時間訊息
{	
#ifndef PLC_OBJ_DISABLE
	const bool bSucc = AOIDataCollect.SaveMovingTimeMsg(pContext);	
	return bSucc;
#endif//PLC_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CPLC_Basic::SavePLCMovingTimeMsg(const wchar_t *pContext)//儲存PLC移動時間訊
{
#ifndef PLC_OBJ_DISABLE
	const bool bSucc = AOIDataCollect.SaveMovingTimeMsg(pContext);	
	return bSucc;
#endif//PLC_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
CString CPLC_Basic::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)//載入多國語系
{
	CString NewLabelText;
	LPCTSTR Section=_T("PLC_BASIC_OBJECT");		
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
void CPLC_Basic::SetPLCExceptionCode_Param(LPCTSTR Err)//設定JET錯誤碼-參數異常
{
	CString str = (NULL!=Err) ? Err:m_ErrorString;
	AOIExceptionCodeCtrl.SetAOIExceptionCode_Param(str);
	//SetPLCExceptionCode(AOI_EXCEPTION_PLC_PARAM, Err);	return;	
	return;
}
//-------------------------------------------------------------------------------------//
void CPLC_Basic::SetPLCExceptionCode_FileRead(LPCTSTR Err)//設定JET錯誤碼-檔案讀取
{
	CString str = (NULL!=Err) ? Err:m_ErrorString;
	AOIExceptionCodeCtrl.SetAOIExceptionCode_FileRead(str);
	//SetPLCExceptionCode(AOI_EXCEPTION_PLC_FILE_READ, Err);	return;	
	return;
}
//-------------------------------------------------------------------------------------//
void CPLC_Basic::SetPLCExceptionCode_FileWrite(LPCTSTR Err)//設定JET錯誤碼-檔案寫入
{
	CString str = (NULL!=Err) ? Err:m_ErrorString;
	AOIExceptionCodeCtrl.SetAOIExceptionCode_FileWrite(str);
	//SetPLCExceptionCode(AOI_EXCEPTION_PLC_FILE_WRITE, Err);	return;	
	return;
}
//-------------------------------------------------------------------------------------//
void CPLC_Basic::SetPLCExceptionCode(DWORD Code, LPCTSTR Err)//設定JET錯誤碼
{		
	CString str = (NULL!=Err) ? Err:m_ErrorString;
	AOIExceptionCodeCtrl.SetAOIExceptionCode_PLC(Code, str);
	return ;
}
//-------------------------------------------------------------------------------------//
bool CPLC_Basic::GetPLCIsConnected() const
{
	return m_PLCIsConnected;
}
//-------------------------------------------------------------------------------------//
bool CPLC_Basic::CheckPLCIsConnected()
{
	if ( GetPLCIsConnected() == false )
	{
		CString str = _T("Error, PLC is disconnected");
		m_ErrorString = CPLC_Basic::LoadMultiLanguageString(str, str);	
		SetPLCExceptionCode(AOI_EXCEPTION_PLC_CONNECT);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CPLC_Basic::GetPLCErrorString()
{
	m_ErrorStringOut=JetAPI::AddKeyToErrorString(m_ErrorString, _T("[PLC]"));
	AOIExceptionCodeCtrl.SetAOIExceptionCode_PLC_Others(m_ErrorStringOut);		
	return m_ErrorStringOut;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CPLC_Basic::GetPLCVersion() const
{
	return this->m_PLCVersion;
}
//-------------------------------------------------------------------------------------//
void CPLC_Basic::SetPLCConnectParam(LPCTSTR val)//設定PLC連線參數
{
	this->m_PLCConnectParam = val;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CPLC_Basic::GetPLCConnectParam() const//取得PLC連線參數
{
	return this->m_PLCConnectParam;
}
//-------------------------------------------------------------------------------------//
TPLCParameter&  CPLC_Basic::GetPLCParameter() //取得PLC連線參數
{ 
	return m_PLCParameter; 
}
//-------------------------------------------------------------------------------------//
const TPLCParameter&  CPLC_Basic::GetPLCParameter() const//取得PLC連線參數
{ 
	return m_PLCParameter; 
}
//-------------------------------------------------------------------------------------//
void CPLC_Basic::SetPLCParameter(const TPLCParameter &Param)//設定PLC參數
{ 
	m_PLCParameter = Param; 
}
//-------------------------------------------------------------------------------------//
void  CPLC_Basic::SetPLCPollingThreadState(THREAD_STATE_MODE State)
{
	if ( State == this->m_PLCPollingThreadState ) { return; }
	this->LockPLC();
	this->m_PLCPollingThreadState = State;	
	this->UnlockPLC();
}
//-------------------------------------------------------------------------------------//
THREAD_STATE_MODE CPLC_Basic::GetPLCPollingThreadState() const
{
	return this->m_PLCPollingThreadState;
}
//-------------------------------------------------------------------------------------//
void CPLC_Basic::SetPLCPollingThreadCmd(THREAD_COMMAND_MODE Cmd)
{
	if ( Cmd == this->m_PLCPollingThreadCmd ) { return ; }
	this->LockPLC();
	this->m_PLCPollingThreadCmd = Cmd;
	this->UnlockPLC();
}
//-------------------------------------------------------------------------------------//
THREAD_COMMAND_MODE CPLC_Basic::GetPLCPollingThreadCmd() const
{
	return this->m_PLCPollingThreadCmd;
}
//-------------------------------------------------------------------------------------//
void CPLC_Basic::SetPLCPollingSleepTime(int time)
{
	this->m_PLCPollingSleepTime = time;
}
//-------------------------------------------------------------------------------------//
int CPLC_Basic::GetPLCPollingSleepTime() const
{
	return this->m_PLCPollingSleepTime;
}
//-------------------------------------------------------------------------------------//
void CPLC_Basic::SetPLCPollingLaneAdjustSensor(bool On)
{
	m_PLCPollingLaneAdjustSensor = On;
}
//-------------------------------------------------------------------------------------//
bool CPLC_Basic::GetPLCPollingLaneAdjustSensor() const
{
	return m_PLCPollingLaneAdjustSensor;
}
//-------------------------------------------------------------------------------------//
bool CPLC_Basic::CreatePLCPollingThread()
{
	if ( this->DeletePLCPollingThread() == false ) { return false; }
	this->m_PLCPollingThreadCmd = THREAD_COMMAND_TO_IDLE;
	PLCPollingThreadHandle = (HANDLE)::_beginthreadex(NULL, NULL, &PLCPollingThreadFn, NULL, NULL, &PLCPollingThreadID);	
	if ( PLCPollingThreadHandle == NULL )
	{
		this->m_PLCPollingThreadState = THREAD_STATE_NONE;			
		CString str = _T("Error, Create Thread Fault (PLCPollingThreadHandle == NULL)");
		m_ErrorString = CPLC_Basic::LoadMultiLanguageString(str, str);	
		AOIExceptionCodeCtrl.SetAOIExceptionCode_Thread(AOI_EXCEPTION_THREAD_CREATE, m_ErrorString);
		return false;
	}	
	::SetThreadPriority(PLCPollingThreadHandle, THREAD_PRIORITY_BELOW_NORMAL);
	//::SetThreadAffinityMask(PLCPollingThreadHandle, 0x03);//保留2個
	return true;
}
//-------------------------------------------------------------------------------------//
bool CPLC_Basic::DeletePLCPollingThread()
{
	if ( NULL == PLCPollingThreadHandle ) {	return true; }
	this->SetPLCPollingThreadCmd(THREAD_COMMAND_TO_EXIT);	

	DWORD Res = ::WaitForSingleObject(PLCPollingThreadHandle, 100000);
	if ( Res == WAIT_TIMEOUT )
	{	::TerminateThread(PLCPollingThreadHandle, 2);	}

	::CloseHandle(PLCPollingThreadHandle);
	PLCPollingThreadHandle = NULL;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CPLC_Basic::StartPLCPollingThread()//開始監控PLC
{
	if ( PLCPollingThreadHandle == NULL ) { return true; }
	this->SetPLCPollingThreadCmd(THREAD_COMMAND_TO_RUN);
	int i=0;
	const int MaxCount = 500;	
	for ( i=0; i<MaxCount; i++ )
	{		
		if ( this->m_PLCPollingThreadCmd != THREAD_COMMAND_TO_RUN )//切入下一個階段
		{	return true; }
		if ( this->m_PLCPollingThreadState != THREAD_STATE_IDLE )
		{	return true;	}
		::Sleep(10);
	}
	CString str = _T("Error, wati for StartPLCPollingThread too long");
	m_ErrorString = CPLC_Basic::LoadMultiLanguageString(str, str);	
	AOIExceptionCodeCtrl.SetAOIExceptionCode_Thread(AOI_EXCEPTION_THREAD_WAIT_FOR_START, m_ErrorString);
	return false;
}
//-------------------------------------------------------------------------------------//
bool CPLC_Basic::StopPLCPollingThread()//停止監控PLC
{
	if ( PLCPollingThreadHandle == NULL ) { return true; }
	this->SetPLCPollingThreadCmd(THREAD_COMMAND_TO_IDLE);
	int i=0;
	const int MaxCount = 500;
	THREAD_STATE_MODE ThreadState = THREAD_STATE_NONE;
	for ( i=0; i<MaxCount; i++ )
	{
		ThreadState = this->GetPLCPollingThreadState();
		if ( THREAD_STATE_IDLE==ThreadState || THREAD_STATE_NONE==ThreadState )
		{	return true;	}
		::Sleep(10);
	}
	CString str = _T("Error, wati for StopPLCPollingThread too long");
	m_ErrorString = CPLC_Basic::LoadMultiLanguageString(str, str);	
	SetPLCExceptionCode(AOI_EXCEPTION_PLC_EXEC_FUNC);
	return false;	
}
//-------------------------------------------------------------------------------------//
void CPLC_Basic::ClearPlcNodeList()//PLC清除節點列表
{
	THREAD_COMMAND_MODE ThreadCommandMode = this->GetPLCPollingThreadCmd();
	if ( ThreadCommandMode == THREAD_COMMAND_TO_RUN )
	{
		if ( this->StopPLCPollingThread() == false )
		{	return; }
	}

	this->m_PLCNodeList.clear();
	this->m_PLCNodeCurrentIndex = -1;

	if ( ThreadCommandMode == THREAD_COMMAND_TO_RUN )
	{	this->StartPLCPollingThread(); }	
}
//-----------------------------------------------------------------------//
int CPLC_Basic::GetPlcNodeCount() const//PLC取得節點數量
{
	return (int)(m_PLCNodeList.size());
}
//-----------------------------------------------------------------------//
TPlcNode* CPLC_Basic::GetPlcNodePtr(int index, bool bCheck)//PLC取得節點指標
{
	if ( bCheck == true )
	{
		const size_t Count = this->m_PLCNodeList.size();
		if ( (index<0) || (index>=Count) )
		{	return NULL; }
	}
	return &m_PLCNodeList[index];
}
//-----------------------------------------------------------------------//
bool CPLC_Basic::AddPlcNode(TPlcNode &Node)//PLC新增節點
{
	THREAD_COMMAND_MODE ThreadCommandMode = this->GetPLCPollingThreadCmd();
	if ( ThreadCommandMode == THREAD_COMMAND_TO_RUN )
	{
		if ( this->StopPLCPollingThread() == false )
		{	return false; }
	}	

	if ( CPLC_Basic::CheckPlcNodeAddress(Node.m_Address) == false )
	{			
		CString str = _T("Error, Address Exception");		
		str = CPLC_Basic::LoadMultiLanguageString(str, str);
		m_ErrorString.Format(_T("%s (%s)"), str, Node.m_Address);		
		SetPLCExceptionCode(AOI_EXCEPTION_PLC_SET_FUNC);
		if ( ThreadCommandMode == THREAD_COMMAND_TO_RUN )
		{	this->StartPLCPollingThread(); }	
		return false;	
	}	

	this->m_PLCNodeList.push_back(Node);
	if ( ThreadCommandMode == THREAD_COMMAND_TO_RUN )
	{	this->StartPLCPollingThread(); }	
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Basic::RemovePlcNode()//PLC刪除節點
{
	THREAD_COMMAND_MODE ThreadCommandMode = this->GetPLCPollingThreadCmd();
	if ( ThreadCommandMode == THREAD_COMMAND_TO_RUN )
	{
		if ( this->StopPLCPollingThread() == false )
		{	return false; }
	}

	size_t i = 0;	
	TPlcNode   *PlcNodePtr = NULL;
	std::vector<TPlcNode>      PLCNodeList = m_PLCNodeList;
	const size_t count = PLCNodeList.size();
	this->m_PLCNodeList.clear();
	this->m_PLCNodeCurrentIndex = -1;
	for ( i=0; i<count; i++ )
	{
		PlcNodePtr = &(PLCNodeList[i]);
		if ( PlcNodePtr->m_Deleted == true ) { continue; }
		m_PLCNodeList.push_back(PLCNodeList[i]);
	}
	
	if ( ThreadCommandMode == THREAD_COMMAND_TO_RUN )
	{	this->StartPLCPollingThread(); }	
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Basic::SetReadAllPlcNode()//設定讀取全部的PLC節點
{
	size_t i = 0;	
	TPlcNode   *PlcNodePtr = NULL;
	const size_t count = m_PLCNodeList.size();	
	for ( i=0; i<count; i++ )
	{
		PlcNodePtr = &(m_PLCNodeList[i]);
		if ( PlcNodePtr->m_Deleted == true ) { continue; }
		PlcNodePtr->m_ToRead = true;		
	}
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Basic::SetReadFnCodePlcNode()//設定讀取只有函式的PLC節點
{
	size_t i = 0;	
	TPlcNode   *PlcNodePtr = NULL;
	const size_t count = m_PLCNodeList.size();	
	for ( i=0; i<count; i++ )
	{
		PlcNodePtr = &(m_PLCNodeList[i]);
		if ( PlcNodePtr->m_Deleted == true ) { continue; }
		if ( PlcNodePtr->m_FnCode == 0 ) 
		{	
			PlcNodePtr->m_ToRead = false;
			continue;
		}
		PlcNodePtr->m_ToRead = true;		
	}
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Basic::GetPlcNodeList(std::vector<TPlcNode> &PLCNodeList)//PLC取得節點列表	
{
	PLCNodeList = this->m_PLCNodeList;
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Basic::SetPlcNodeList(const std::vector<TPlcNode> &PLCNodeList)//PLC設定節點列表	
{
	THREAD_COMMAND_MODE ThreadCommandMode = this->GetPLCPollingThreadCmd();
	if ( ThreadCommandMode == THREAD_COMMAND_TO_RUN )
	{
		if ( this->StopPLCPollingThread() == false )
		{	return false; }
	}
	this->m_PLCNodeCurrentIndex = -1;
	this->m_PLCNodeList = PLCNodeList;
	if ( ThreadCommandMode == THREAD_COMMAND_TO_RUN )
	{	this->StartPLCPollingThread(); }	
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Basic::LoadExtraPlcNodeList()//PLC載入節點列表
{
	if ( LoadExtraPlcNodeListFn() == false )
	{
		SetPLCExceptionCode_FileRead();
		return false;
	}
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Basic::LoadExtraPlcNodeListFn()//PLC載入節點列表
{
	int     i = 0;
	int     index = 0;
	int     NodeCount = 0;
	TPlcNode PLCNode;
	CString INIFile;
	CString Filename = _T("PLCNodeList.INI");
	CString Section = _T("PLC Node List");
	CString KeyName = _T("");
	CString Default = _T("");		
	const size_t StringSize = 64;
	TCHAR   String[StringSize]=_T("");

	INIFile.Format(_T("%s\\%s"), AOIDataCollect.GetAOIDirectory(), Filename);

	//this->m_PLCNodeList.clear();
	
	//Total Node Count;
	Section = _T("Extra Node List");
	KeyName = _T("Extra Node Count");	
	if ( LoadINIData(Section, KeyName, Default, String, StringSize, INIFile, false, m_ErrorString) == false )
	{	return false; }

	index = 0;
	NodeCount = ::_ttoi(String);
	for ( i=0; i<NodeCount; i++ )
	{
		if ( (i+1) < 10 ) 
		{	Section.Format(_T("Node_00%d"), (i+1)); }
		else if ( (i+1) < 100 ) 
		{	Section.Format(_T("Node_0%d"), (i+1)); }
		else
		{	Section.Format(_T("Node_%d"), (i+1)); }

		//名稱
		KeyName = _T("Name");
		if ( LoadINIData(Section, KeyName, Default, String, StringSize, INIFile, false, m_ErrorString) == false )
		{	return false; }
		PLCNode.m_Name = String;

		//位址
		KeyName = _T("Address");
		if ( LoadINIData(Section, KeyName, Default, String, StringSize, INIFile, false, m_ErrorString) == false )
		{	return false; }
		PLCNode.SetAddress(String);

		PLCNode.m_FnCode = 0;		
		PLCNode.m_Value = 0;
		PLCNode.m_Seleted = false;
		PLCNode.m_Deleted = false;
		PLCNode.m_ToRead = false;
		this->m_PLCNodeList.push_back(PLCNode);
	}
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Basic::SaveExtraPlcNodeList()//PLC儲存節點列表
{
	if ( SaveExtraPlcNodeListFn() == false )
	{
		SetPLCExceptionCode_FileWrite();	
		return false;
	}
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Basic::SaveExtraPlcNodeListFn()//PLC儲存節點列表	
{
	int     i = 0;
	int     index=0;
	TPlcNode *PLCNodePtr=NULL;
	CString INIFile;
	CString Filename = _T("PLCNodeList.INI");
	CString Section = _T("PLC Node List");
	CString KeyName = _T("");
	CString String = _T("");		
	const int NodeCount = this->GetPlcNodeCount();	
	
	INIFile.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), Filename);	
	
	//Total Node Count;
	index = 0;
	for ( i=0; i<NodeCount; i++ )
	{
		PLCNodePtr = this->GetPlcNodePtr(i, false);
		if ( NULL == PLCNodePtr ) { continue; }
		if ( PLCNodePtr->m_FnCode != 0 ) { continue; }
		index ++;
	}

	Section = _T("Extra Node List");
	KeyName = _T("Extra Node Count");
	String.Format(_T("%d"), index);
	if ( SaveINIData(Section, KeyName, String, INIFile, m_ErrorString) == false )
	{	return false; }
	
	index = 0;
	for ( i=0; i<NodeCount; i++ )
	{
		PLCNodePtr = this->GetPlcNodePtr(i, false);
		if ( NULL == PLCNodePtr ) { continue; }
		if ( PLCNodePtr->m_FnCode != 0 ) { continue; }

		if ( (index+1) < 10 ) 
		{	Section.Format(_T("Node_00%d"), (index+1)); }
		else if ( (index+1) < 100 ) 
		{	Section.Format(_T("Node_0%d"), (index+1)); }
		else
		{	Section.Format(_T("Node_%d"), (index+1)); }

		//名稱
		KeyName = _T("Name");
		String = PLCNodePtr->m_Name;
		if ( SaveINIData(Section, KeyName, String, INIFile, m_ErrorString) == false )
		{	return false; }		

		//位址
		KeyName = _T("Address");
		String = PLCNodePtr->m_Address;
		if ( SaveINIData(Section, KeyName, String, INIFile, m_ErrorString) == false )
		{	return false; }		

		index ++;
	}
	CString INIFile2;	
	INIFile2.Format(_T("%s\\%s"), AOIDataCollect.GetAOIDirectory(), Filename);	
	::CopyFile(INIFile, INIFile2, FALSE);
	return true;
}
//-----------------------------------------------------------------------//
int CPLC_Basic::GetPCBInOutTimeout_LA() const
{
	return GetPLCParameter().m_PCBInOutTimeout_LA;
}
//--------------------------------------------------------------------------//
int CPLC_Basic::GetPCBInOutTimeout_LB() const
{
	return GetPLCParameter().m_PCBInOutTimeout_LB;
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::IncrementPCBInCount_LA()//疊加進板確認次數
{
	const int MaxCounts = this->GetPCBInOutTimeout_LA()+100;
	this->m_PCBInCheckCount_LA ++;
	if ( this->m_PCBInCheckCount_LA < MaxCounts ) { return true; }
	
	CString str = _T("PCB-In Fault With Timeout");
	m_ErrorString = CPLC_Basic::LoadMultiLanguageString(str, str);
	SetPLCExceptionCode(AOI_EXCEPTION_PLC_PCB_IN_LA);
	m_PCBInTimeEnd_LA = ::GetTickCount();//進板時間結束		
	return false;
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::IncrementPCBOutCount_LA()//疊加出板確認次數
{
	const int MaxCounts = this->GetPCBInOutTimeout_LA()+100;
	this->m_PCBOutCheckCount_LA ++;
	if ( this->m_PCBOutCheckCount_LA < MaxCounts ) { return true; }
	
	CString str = _T("PCB-Out Fault With Timeout");
	m_ErrorString = CPLC_Basic::LoadMultiLanguageString(str, str);
	SetPLCExceptionCode(AOI_EXCEPTION_PLC_PCB_OUT_LA);	
	m_PCBOutTimeEnd_LA = ::GetTickCount();//進板時間結束
	return false;
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::IncrementPCBBackCount_LA()//疊加回板確認次數
{
	const int MaxCounts = this->GetPCBInOutTimeout_LA()+100;
	this->m_PCBBackCheckCount_LA ++;
	if ( this->m_PCBBackCheckCount_LA < MaxCounts ) { return true; }

	if ( true == m_PCBBackOut_LA )
	{
		CString str = _T("PCB-Back-Out Fault With Timeout");	
		m_ErrorString = CPLC_Basic::LoadMultiLanguageString(str, str);
		SetPLCExceptionCode(AOI_EXCEPTION_PLC_PCB_BACK_OUT_LA);
	}
	else
	{
		CString str = _T("PCB-Back Fault With Timeout");
		m_ErrorString = CPLC_Basic::LoadMultiLanguageString(str, str);
		SetPLCExceptionCode(AOI_EXCEPTION_PLC_PCB_BACK_LA);
	}
	m_PCBBackTimeEnd_LA = ::GetTickCount();//進板時間結束	
	return false;
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::IncrementPCBOutInCheckCount_LA()//疊加出板帶進板確認次數
{
	const int MaxCounts = this->GetPCBInOutTimeout_LA()+100;
	this->m_PCBOutInCheckCount_LA ++;
	if ( this->m_PCBOutInCheckCount_LA < MaxCounts ) { return true; }
	
	CString str = _T("PCB-Out with In Fault With Timeout");	
	m_ErrorString = CPLC_Basic::LoadMultiLanguageString(str, str);
	SetPLCExceptionCode(AOI_EXCEPTION_PLC_PCB_OUT_IN_LA);
	m_PCBOutTimeEnd_LA = ::GetTickCount();//進板時間結束	
	return false;
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::IncrementPCBInCount_LB()//疊加進板確認次數
{
	const int MaxCounts = this->GetPCBInOutTimeout_LB()+100;
	this->m_PCBInCheckCount_LB ++;
	if ( this->m_PCBInCheckCount_LB < MaxCounts ) { return true; }
	
	CString str = _T("PCB-In Fault With Timeout");
	m_ErrorString = CPLC_Basic::LoadMultiLanguageString(str, str);
	SetPLCExceptionCode(AOI_EXCEPTION_PLC_PCB_IN_LB);
	m_PCBInTimeEnd_LB = ::GetTickCount();//進板時間結束		
	return false;
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::IncrementPCBOutCount_LB()//疊加出板確認次數
{
	const int MaxCounts = this->GetPCBInOutTimeout_LB()+100;
	this->m_PCBOutCheckCount_LB ++;
	if ( this->m_PCBOutCheckCount_LB < MaxCounts ) { return true; }
	
	CString str = _T("PCB-Out Fault With Timeout");
	m_ErrorString = CPLC_Basic::LoadMultiLanguageString(str, str);
	SetPLCExceptionCode(AOI_EXCEPTION_PLC_PCB_OUT_LB);
	m_PCBOutTimeEnd_LB = ::GetTickCount();//進板時間結束
	return false;
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::IncrementPCBBackCount_LB()//疊加回板確認次數
{
	const int MaxCounts = this->GetPCBInOutTimeout_LB()+100;
	this->m_PCBBackCheckCount_LB ++;
	if ( this->m_PCBBackCheckCount_LB < MaxCounts ) { return true; }

	if ( true == m_PCBBackOut_LB )
	{
		CString str = _T("PCB-Back-Out Fault With Timeout");	
		m_ErrorString = CPLC_Basic::LoadMultiLanguageString(str, str);
		SetPLCExceptionCode(AOI_EXCEPTION_PLC_PCB_BACK_OUT_LB);
	}
	else
	{
		CString str = _T("PCB-Back Fault With Timeout");
		m_ErrorString = CPLC_Basic::LoadMultiLanguageString(str, str);
		SetPLCExceptionCode(AOI_EXCEPTION_PLC_PCB_BACK_LB);
	}
	m_PCBBackTimeEnd_LB = ::GetTickCount();//進板時間結束	
	return false;
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::IncrementPCBOutInCheckCount_LB()//疊加出板帶進板確認次數
{
	const int MaxCounts = this->GetPCBInOutTimeout_LB()+100;
	this->m_PCBOutInCheckCount_LB ++;
	if ( this->m_PCBOutInCheckCount_LB < MaxCounts ) { return true; }
	
	CString str = _T("PCB-Out with In Fault With Timeout");	
	m_ErrorString = CPLC_Basic::LoadMultiLanguageString(str, str);
	SetPLCExceptionCode(AOI_EXCEPTION_PLC_PCB_OUT_IN_LB);
	m_PCBOutTimeEnd_LB = ::GetTickCount();//進板時間結束	
	return false;
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::CheckPLCReady(bool bChkStartLight, bool bAutoReset)//確認PLC狀態OK
{
	CString str;
#ifndef PLC_OBJ_DISABLE	
	if ( GetPLCIsConnected() == false )
	{
		str = _T("Error, PLC is disconnected");
		m_ErrorString = CPLC_Basic::LoadMultiLanguageString(str, str);
		SetPLCExceptionCode(AOI_EXCEPTION_PLC_CONNECT);
		return false;
	}
	if ( GetCurrentEMSStats()==true )
	{
		str = _T("Error, EMS button push down");
		m_ErrorString = CPLC_Basic::LoadMultiLanguageString(str, str);
		SetPLCExceptionCode(AOI_EXCEPTION_PLC_EMERGENCY);
		return false;
	}
	if ( GetCurrentKeySwitchStats()==false )
	{
		str = _T("Error, Key switch turn off");
		m_ErrorString = CPLC_Basic::LoadMultiLanguageString(str, str);
		SetPLCExceptionCode(AOI_EXCEPTION_PLC_KEY_SWITCH);
		return false;
	}		
	if ( GetCurrentOverHeat()==true )
	{		
		str = _T("Error, Over Heat");
		m_ErrorString = CPLC_Basic::LoadMultiLanguageString(str, str);
		SetPLCExceptionCode(AOI_EXCEPTION_PLC_OVER_HEAT);
		return false;
	}
	if ( GetPLC_FanAlarm()==true )
	{		
		str = _T("Error, Fan Alarm");
		m_ErrorString = CPLC_Basic::LoadMultiLanguageString(str, str);
		SetPLCExceptionCode(AOI_EXCEPTION_PLC_FAN_ALARM);
		return false;
	}
	if ( GetSaftyBypass() == false )
	{
		if ( GetSafetyAlarm() == true )
		{
			if ( GetCurrentAirStats() == true )
			{	
				str = _T("Error, Current Air is too low"); 
				m_ErrorString = CPLC_Basic::LoadMultiLanguageString(str, str);
				SetPLCExceptionCode(AOI_EXCEPTION_PLC_AIR_LOST);
				return false;
			}

			if ( GetCurrentFrontCapStats() == true )
			{	
				str = _T("Error, Front Cap is opoened"); 
				m_ErrorString = CPLC_Basic::LoadMultiLanguageString(str, str);
				SetPLCExceptionCode(AOI_EXCEPTION_PLC_DOOR_OPEN);	
				return false;
			}

			if ( GetCurrentRearCapStats() == true )
			{	
				str = _T("Error, Rear Door is opoened"); 
				m_ErrorString = CPLC_Basic::LoadMultiLanguageString(str, str);
				SetPLCExceptionCode(AOI_EXCEPTION_PLC_DOOR_OPEN);
				return false;
			}			
			SetPLCExceptionCode(AOI_EXCEPTION_PLC_SAFTY_ALARM);
		}	
	}
	if ( GetPCBBargeIn_LA() == true )
	{
		str = _T("Error, PCB Barge In LA"); 		
		m_ErrorString = CPLC_Basic::LoadMultiLanguageString(str, str);
		SetPLCExceptionCode(AOI_EXCEPTION_PLC_PCB_BARGE_IN_LA);
		return false;
	}
	if ( GetPCBBargeIn_LB() == true )
	{
		str = _T("Error, PCB Barge In LB"); 		
		m_ErrorString = CPLC_Basic::LoadMultiLanguageString(str, str);
		SetPLCExceptionCode(AOI_EXCEPTION_PLC_PCB_BARGE_IN_LB);
		return false;
	}

	if ( GetSafetyAlarmEMS() == true )
	{
		str = _T("Error, EMS Button push down"); 
		m_ErrorString = CPLC_Basic::LoadMultiLanguageString(str, str);
		SetPLCExceptionCode(AOI_EXCEPTION_PLC_EMERGENCY);
		return false;
	}

	if ( true == bChkStartLight )
	{
		if ( GetCurrentGreenLightStats()==false )
		{
			if ( true ==bAutoReset )
			{
				if ( PushDownResetBtn() ==true )//自動按下復歸燈
				{
					::Sleep(50);
					if ( PushDownStartBtn() == true )//自動啟動綠燈
					{	::Sleep(500);	}
				}
			}
			if ( GetCurrentGreenLightStats()==false )
			{
				str = _T("Error, Start light turn off");
				m_ErrorString = CPLC_Basic::LoadMultiLanguageString(str, str);
				
				const int ErrorCodeMax=100;
				CString strErrLA, strErrLB, strErrLane;
				int ErrorCode_LA=ReadLanePLCErrorCode_LA();
				int ErrorCode_LB=ReadLanePLCErrorCode_LB();
				if ( ErrorCode_LA > ErrorCodeMax ) { ErrorCode_LA=0; }
				if ( ErrorCode_LB > ErrorCodeMax ) { ErrorCode_LB=0; }
				if ( 0 != ErrorCode_LA )
				{	strErrLA = GetPLCErrorCodeText(ErrorCode_LA);	}
				if ( 0 != ErrorCode_LB )
				{	strErrLB = GetPLCErrorCodeText(ErrorCode_LB);	}
				if ( strErrLA==strErrLB )
				{	strErrLane=strErrLA;	}
				else
				{	strErrLane.Format(_T("%s\n%s"), strErrLA, strErrLB);	}
				if ( strErrLane.GetLength() > 0 )
				{
					str = m_ErrorString;
					m_ErrorString.Format(_T("%s\n%s"), str, strErrLane);					
				}
				//SetPLCExceptionCode(AOI_EXCEPTION_PLC_OTHERS);
				SetPLCExceptionCode(AOI_EXCEPTION_PLC_SAFTY_ALARM); 
				return false;
			}
		}	
	}
#endif//PLC_OBJ_DISABLE
	return true;
}
//--------------------------------------------------------------------------//
int CPLC_Basic::GetTowerLightState_Stop(LANE_ID LaneID) const
{
	if ( LANE_ID_B == LaneID )
	{	return m_TowerLightState_Stop_LB;	}
	return m_TowerLightState_Stop_LA;
}
//--------------------------------------------------------------------------//
void CPLC_Basic::SetTowerLightState_Stop(LANE_ID LaneID, int State)
{
	switch ( LaneID )
	{
	case LANE_ID_B:	m_TowerLightState_Stop_LB = State;	break;
	default:
	case LANE_ID_A:	m_TowerLightState_Stop_LA = State;	break;
	}	
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::UpdateTowerLightState_Stop()//更新塔燈狀態-停止
{
#ifndef PLC_OBJ_DISABLE	
	if ( UpdateTowerLightState_Stop(LANE_ID_A) == false )
	{	return false; }	
	if ( UpdateTowerLightState_Stop(LANE_ID_B) == false )
	{	return false; }	
#endif//PLC_OBJ_DISABLE
	return true;
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::UpdateTowerLightState_Stop(LANE_ID LaneID)//更新塔燈狀態-停止
{
#ifndef PLC_OBJ_DISABLE		
	if ( WriteTowerLightState_Stop(LaneID, GetTowerLightState_Stop()) == false )
	{	return false; }	
#endif//PLC_OBJ_DISABLE
	return true;	
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::UpdateTowerLightState_WaitLast(LANE_ID LaneID)//更新塔燈狀態-等上站
{
#ifndef PLC_OBJ_DISABLE		
	if ( WriteTowerLightState_Stop(LaneID, GetTowerLightState_WaitLast()) == false )
	{	return false; }	
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Basic::UpdateTowerLightState_WaitNext(LANE_ID LaneID)//更新塔燈狀態-等下站
{
#ifndef PLC_OBJ_DISABLE		
	if ( WriteTowerLightState_Stop(LaneID, GetTowerLightState_WaitNext()) == false )
	{	return false; }	
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Basic::WriteOnInspection(LANE_ID LaneID, bool On)//檢測中
{
	if ( LANE_ID_B == LaneID )
	{	return WriteOnInspection_LB(On); }
	return WriteOnInspection_LA(On);
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::TurnOnConveryerLED(LANE_ID LaneID, bool On)//軌道LED燈
{
	if ( LANE_ID_B == LaneID )
	{	return TurnOnConveryerLED_LB(On); }
	return TurnOnConveryerLED_LA(On);
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::TurnOnStageAlarm(LANE_ID LaneID)//PLC-設定機台異常
{
	if ( LANE_ID_B == LaneID )
	{	return TurnOnStageAlarm_LB(); }
	return TurnOnStageAlarm_LA();
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::TurnOffStageAlarm(LANE_ID LaneID)//PLC-關閉機台異常
{
	if ( LANE_ID_B == LaneID )
	{	return TurnOffStageAlarm_LB(); }
	return TurnOffStageAlarm_LA();
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::TurnOnInspectAlarm(LANE_ID LaneID)
{
	if ( LANE_ID_B == LaneID )
	{	return TurnOnInspectAlarm_LB(); }
	return TurnOnInspectAlarm_LA();
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::TurnOffInspectAlarm(LANE_ID LaneID)
{
	if ( LANE_ID_B == LaneID )
	{	return TurnOffInspectAlarm_LB(); }
	return TurnOffInspectAlarm_LA();
}
//--------------------------------------------------------------------------//	
bool CPLC_Basic::WriteConveryerClamp(LANE_ID LaneID, bool On)//PLC-是否夾板
{
	if ( LANE_ID_B == LaneID )
	{	return WriteConveryerClamp_LB(On); }
	return WriteConveryerClamp_LA(On);
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::WriteConveryerStopBar(LANE_ID LaneID, bool On)//PLC-停板器
{
	if ( LANE_ID_B == LaneID )
	{	return WriteConveryerStopBar_LB(On); }
	return WriteConveryerStopBar_LA(On);
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::ExecPCBIn(LANE_ID LaneID, bool bWait)//PLC-進板
{
	if ( LANE_ID_B == LaneID )
	{	return ExecPCBIn_LB(bWait); }
	return ExecPCBIn_LA(bWait);
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::WaitForPCBInFinish(LANE_ID LaneID)//PLC-等進板完成
{
	if ( LANE_ID_B == LaneID )
	{	return WaitForPCBInFinish_LB(); }
	return WaitForPCBInFinish_LA();
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::CheckPCBInFinish(LANE_ID LaneID)//PLC-進板完成
{
	if ( LANE_ID_B == LaneID )
	{	return CheckPCBInFinish_LB(); }
	return CheckPCBInFinish_LA();
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::CheckPCBInFault(LANE_ID LaneID)//PLC-進板失敗	
{
	if ( LANE_ID_B == LaneID )
	{	return CheckPCBInFault_LB(); }
	return CheckPCBInFault_LA();
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::ExecPCBIn2nd(LANE_ID LaneID, bool bWait)//PLC-第2段進板
{
	if ( LANE_ID_B == LaneID )
	{	return ExecPCBIn2nd_LB(bWait); }
	return ExecPCBIn2nd_LA(bWait);
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::WaitForPCBIn2ndFinish(LANE_ID LaneID)//PLC-等第2段進板完成
{
	if ( LANE_ID_B == LaneID )
	{	return WaitForPCBIn2ndFinish_LB(); }
	return WaitForPCBIn2ndFinish_LA();
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::CheckPCBIn2ndFinish(LANE_ID LaneID)//PLC-第2段進板完成
{
	if ( LANE_ID_B == LaneID )
	{	return CheckPCBIn2ndFinish_LB(); }
	return CheckPCBIn2ndFinish_LA();
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::CheckPCBIn2ndFault(LANE_ID LaneID)//PLC-第2段進板失敗	
{
	if ( LANE_ID_B == LaneID )
	{	return CheckPCBIn2ndFault_LB(); }
	return CheckPCBIn2ndFault_LA();
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::ExecPCBIn3rd(LANE_ID LaneID, bool bWait)//PLC-第3段進板
{
	if ( LANE_ID_B == LaneID )
	{	return ExecPCBIn3rd_LB(bWait); }
	return ExecPCBIn3rd_LA(bWait);
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::WaitForPCBIn3rdFinish(LANE_ID LaneID)//PLC-等第3段進板完成
{
	if ( LANE_ID_B == LaneID )
	{	return WaitForPCBIn3rdFinish_LB(); }
	return WaitForPCBIn3rdFinish_LA();
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::CheckPCBIn3rdFinish(LANE_ID LaneID)//PLC-第3段進板完成
{
	if ( LANE_ID_B == LaneID )
	{	return CheckPCBIn3rdFault_LB(); }
	return CheckPCBIn3rdFault_LA();
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::CheckPCBIn3rdFault(LANE_ID LaneID)//PLC-第3段進板失敗	
{
	if ( LANE_ID_B == LaneID )
	{	return CheckPCBIn3rdFault_LB(); }
	return CheckPCBIn3rdFault_LA();
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::ExecPCBBack(LANE_ID LaneID, bool bWait)////PLC-退板
{
	if ( LANE_ID_B == LaneID )
	{	return ExecPCBBack_LB(bWait); }
	return ExecPCBBack_LA(bWait);
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::WaitForPCBBackFinish(LANE_ID LaneID)//PLC-等退板完成
{
	if ( LANE_ID_B == LaneID )
	{	return WaitForPCBBackFinish_LB(); }
	return WaitForPCBBackFinish_LA();
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::CheckPCBBackFinish(LANE_ID LaneID)//PLC-退板完成
{
	if ( LANE_ID_B == LaneID )
	{	return CheckPCBBackFinish_LB(); }
	return CheckPCBBackFinish_LA();
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::CheckPCBBackFault(LANE_ID LaneID)//PLC-退板失敗
{
	if ( LANE_ID_B == LaneID )
	{	return CheckPCBBackFault_LB(); }
	return CheckPCBBackFault_LA();
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::ExecPCBBackOut(LANE_ID LaneID, bool bWait)//PLC-退出板
{
	if ( LANE_ID_B == LaneID )
	{	return ExecPCBBackOut_LB(bWait); }
	return ExecPCBBackOut_LA(bWait);
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::WaitForPCBBackOutFinish(LANE_ID LaneID)//PLC-等退出板完成
{
	if ( LANE_ID_B == LaneID )
	{	return WaitForPCBBackOutFinish_LB(); }
	return WaitForPCBBackOutFinish_LA();
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::CheckPCBBackOutFinish(LANE_ID LaneID)//PLC-退出板完成
{
	if ( LANE_ID_B == LaneID )
	{	return CheckPCBBackOutFinish_LB(); }
	return CheckPCBBackOutFinish_LA();
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::CheckPCBBackOutFault(LANE_ID LaneID)//PLC-退出板失敗
{
	if ( LANE_ID_B == LaneID )
	{	return CheckPCBBackOutFault_LB(); }
	return CheckPCBBackOutFault_LA();
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::ExecPCBOut(LANE_ID LaneID, bool bWait)//PLC-出板
{
	if ( LANE_ID_B == LaneID )
	{	return ExecPCBOut_LB(bWait); }
	return ExecPCBOut_LA(bWait);
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::WaitForPCBOutFinish(LANE_ID LaneID)//PLC-等出板完成
{
	if ( LANE_ID_B == LaneID )
	{	return WaitForPCBOutFinish_LB(); }
	return WaitForPCBOutFinish_LA();
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::CheckPCBOutFinish(LANE_ID LaneID)//PLC-出板完成
{
	if ( LANE_ID_B == LaneID )
	{	return CheckPCBOutFinish_LB(); }
	return CheckPCBOutFinish_LA();
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::CheckPCBOutFault(LANE_ID LaneID)//PLC-出板失敗
{
	if ( LANE_ID_B == LaneID )
	{	return CheckPCBOutFault_LB(); }
	return CheckPCBOutFault_LA();
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::ExecPCBOutInside(LANE_ID LaneID, bool bWait)//PLC-出板至機台側邊
{
	if ( LANE_ID_B == LaneID )
	{	return ExecPCBOutInside_LB(bWait); }
	return ExecPCBOutInside_LA(bWait);
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::WaitForPCBOutInsideFinish(LANE_ID LaneID)//PLC-等待出板至機台側邊
{
	if ( LANE_ID_B == LaneID )
	{	return WaitForPCBOutInsideFinish_LB(); }
	return WaitForPCBOutInsideFinish_LA();
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::CheckPCBOutInsideFinish(LANE_ID LaneID)//PLC-出板至機台側邊完成
{
	if ( LANE_ID_B == LaneID )
	{	return CheckPCBOutInsideFinish_LB(); }
	return CheckPCBOutInsideFinish_LA();
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::CheckPCBOutInsideFault(LANE_ID LaneID)//PLC-出板至機台側邊失敗
{
	if ( LANE_ID_B == LaneID )
	{	return CheckPCBOutInsideFault_LB(); }
	return CheckPCBOutInsideFault_LA();
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::ExecPCBOutIn(LANE_ID LaneID, bool bWait)//PLC-出板+進板
{
	if ( LANE_ID_B == LaneID )
	{	return ExecPCBOutIn_LB(bWait); }
	return ExecPCBOutIn_LA(bWait);
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::WaitForPCBOutInFinish(LANE_ID LaneID)//PLC-等待出板+進板
{
	if ( LANE_ID_B == LaneID )
	{	return WaitForPCBOutInFinish_LB(); }
	return WaitForPCBOutInFinish_LA();
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::CheckPCBOutInFinish(LANE_ID LaneID)//PLC-出板+進板完成
{
	if ( LANE_ID_B == LaneID )
	{	return CheckPCBOutInFinish_LB(); }
	return CheckPCBOutInFinish_LA();
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::CheckPCBOutInFault(LANE_ID LaneID)//PLC-出板+進板失敗
{
	if ( LANE_ID_B == LaneID )
	{	return CheckPCBOutInFault_LB(); }
	return CheckPCBOutInFault_LA();
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::ExecPCBReIn(LANE_ID LaneID, bool bWait)//PLC-重新進板
{
	if ( LANE_ID_B == LaneID )
	{	return ExecPCBReIn_LB(bWait); }
	return ExecPCBReIn_LA(bWait);
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::WaitForPCBReInFinish(LANE_ID LaneID)//PLC-等待重新進板完成
{
	if ( LANE_ID_B == LaneID )
	{	return WaitForPCBReInFinish_LB(); }
	return WaitForPCBReInFinish_LA();
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::ExecPCBClear(LANE_ID LaneID, bool bWait)//PLC-清板
{
	if ( LANE_ID_B == LaneID )
	{	return ExecPCBClear_LB(bWait); }
	return ExecPCBClear_LA(bWait);
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::WaitForPCBClearFinish(LANE_ID LaneID)//PLC-等待清板完成
{
	if ( LANE_ID_B == LaneID )
	{	return WaitForPCBClearFinish_LB(); }
	return WaitForPCBClearFinish_LA();
}
//--------------------------------------------------------------------------//
int CPLC_Basic::GetPCBAutoRunMode(LANE_ID LaneID)//自動進出板模式
{
	if ( LANE_ID_B == LaneID )
	{	return GetPCBAutoRunMode_LB(); }
	return GetPCBAutoRunMode_LA();
}
//--------------------------------------------------------------------------//
void CPLC_Basic::SetPCBAutoRunMode(LANE_ID LaneID, int Mode)//自動進出板模式
{
	if ( LANE_ID_B == LaneID )
	{	return SetPCBAutoRunMode_LB(Mode); }
	return SetPCBAutoRunMode_LA(Mode);	
}
//-------------------------------------------------------------------------------------//
bool CPLC_Basic::GetPCBAutoRunPCBChaned(LANE_ID LaneID)//自動進出板PCB改變過
{
	if ( LANE_ID_B == LaneID )
	{	return GetPCBAutoRunPCBChaned_LB(); }
	return GetPCBAutoRunPCBChaned_LA();	
}
//-------------------------------------------------------------------------------------//
void CPLC_Basic::SetPCBAutoRunPCBChaned(LANE_ID LaneID, bool Changed)//自動進出板PCB改變過
{
	if ( LANE_ID_B == LaneID )
	{	return SetPCBAutoRunPCBChaned_LB(Changed); }
	return SetPCBAutoRunPCBChaned_LA(Changed);	
}
//-------------------------------------------------------------------------------------//
int CPLC_Basic::GetPCBAutoRunMode_LA()//自動進出板模式-LA
{
	return m_PCBAutoRunMode_LA;
}
//-----------------------------------------------------------------------//
bool CPLC_Basic::GetPCBAutoRunPCBChaned_LA() const//自動進出板PCB改變過
{
	return m_PCBAutoRunPCBChaned_LA;
}
//-----------------------------------------------------------------------//
void CPLC_Basic::SetPCBAutoRunPCBChaned_LA(bool val)//自動進出板PCB改變過
{
	m_PCBAutoRunPCBChaned_LA = val;
}
//-----------------------------------------------------------------------//
int CPLC_Basic::GetPCBAutoRunMode_LB()//自動進出板模式-LB
{
	return m_PCBAutoRunMode_LB;
}
//-----------------------------------------------------------------------//
bool CPLC_Basic::GetPCBAutoRunPCBChaned_LB() const//自動進出板PCB改變過
{
	return m_PCBAutoRunPCBChaned_LB;
}
//-----------------------------------------------------------------------//
void CPLC_Basic::SetPCBAutoRunPCBChaned_LB(bool val)//自動進出板PCB改變過
{
	m_PCBAutoRunPCBChaned_LB = val;
}
//-----------------------------------------------------------------------//
bool CPLC_Basic::StopPCBAutoOutIn(LANE_ID LaneID, bool bWait)//PLC-停止自動出進板
{
	if ( LANE_ID_B == LaneID )
	{	return StopPCBAutoOutIn_LB(bWait); }
	return StopPCBAutoOutIn_LA(bWait);
}
//-----------------------------------------------------------------------//
bool CPLC_Basic::ExecPCBAutoOutIn(LANE_ID LaneID, bool On, bool bWait)//PLC-自動出進板
{
	if ( LANE_ID_B == LaneID )
	{	return ExecPCBAutoOutIn_LB(On, bWait); }
	return ExecPCBAutoOutIn_LA(On, bWait);
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::WaitForPCBAutoOutInFinish(LANE_ID LaneID)//PLC-等待自動出進板
{
	if ( LANE_ID_B == LaneID )
	{	return WaitForPCBAutoOutInFinish_LB(); }
	return WaitForPCBAutoOutInFinish_LA();
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::ReadPCBAutoOutInFinish(LANE_ID LaneID)//PLC-自動出進板完成
{
	if ( LANE_ID_B == LaneID )
	{	return ReadPCBAutoOutInFinish_LB(); }
	return ReadPCBAutoOutInFinish_LA();
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::CheckPCBAutoOutInFinish(LANE_ID LaneID)//PLC-自動出進板完成
{
	if ( LANE_ID_B == LaneID )
	{	return CheckPCBAutoOutInFinish_LB(); }
	return CheckPCBAutoOutInFinish_LA();
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::CheckPCBAutoOutInFault(LANE_ID LaneID)//PLC-自動出進板失敗
{
	if ( LANE_ID_B == LaneID )
	{	return CheckPCBAutoOutInFault_LB(); }
	return CheckPCBAutoOutInFault_LA();
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::WritePCBAutoOutInFinish(LANE_ID LaneID, bool On)//PLC-寫入自動出進板完成
{
	if ( LANE_ID_B == LaneID )
	{	return WritePCBAutoOutInFinish_LB(On); }
	return WritePCBAutoOutInFinish_LA(On);
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::StopPCBAutoBackIn(LANE_ID LaneID, bool bWait)//PLC-停止自動退進板
{
	if ( LANE_ID_B == LaneID )
	{	return StopPCBAutoBackIn_LB(bWait); }
	return StopPCBAutoBackIn_LA(bWait);
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::ExecPCBAutoBackIn(LANE_ID LaneID, bool On, bool bWait)//PLC-自動退進板
{
	if ( LANE_ID_B == LaneID )
	{	return ExecPCBAutoBackIn_LB(On, bWait); }
	return ExecPCBAutoBackIn_LA(On, bWait);
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::WaitForPCBAutoBackInFinish(LANE_ID LaneID)//PLC-等待自動退進板
{
	if ( LANE_ID_B == LaneID )
	{	return WaitForPCBAutoBackInFinish_LB(); }
	return WaitForPCBAutoBackInFinish_LA();
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::ReadPCBAutoBackInFinish(LANE_ID LaneID)//PLC-自動退進板完成
{
	if ( LANE_ID_B == LaneID )
	{	return ReadPCBAutoBackInFinish_LB(); }
	return ReadPCBAutoBackInFinish_LA();
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::CheckPCBAutoBackInFinish(LANE_ID LaneID)//PLC-自動退進板完成
{
	if ( LANE_ID_B == LaneID )
	{	return CheckPCBAutoBackInFinish_LB(); }
	return CheckPCBAutoBackInFinish_LA();
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::CheckPCBAutoBackInFault(LANE_ID LaneID)//PLC-自動退進板失敗
{
	if ( LANE_ID_B == LaneID )
	{	return CheckPCBAutoBackInFault_LB(); }
	return CheckPCBAutoBackInFault_LA();
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::WritePCBAutoBackInFinish(LANE_ID LaneID, bool On)//PLC-寫入自動退進板完成
{
	if ( LANE_ID_B == LaneID )
	{	return WritePCBAutoBackInFinish_LB(On); }
	return WritePCBAutoBackInFinish_LA(On);
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::WaitForConveryerStopRunning(LANE_ID LaneID)//PLC-等待軌道停止運轉
{
	if ( LANE_ID_B == LaneID )
	{	return WaitForConveryerStopRunning_LB(); }
	return WaitForConveryerStopRunning_LA();
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::ExecConveyorMotorStop(LANE_ID LaneID)//PLC-執行軌道停止
{
	return ExecConveyorMotorRunning(LaneID, false, true, false);	
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::ExecConveyorMotorRunning(LANE_ID LaneID, bool On, bool bPositive, bool Slow)//PLC-執行軌道運轉	
{
	if ( LANE_ID_B == LaneID )
	{	return ExecConveyorMotorRunning_LB(On, bPositive, Slow); }
	return ExecConveyorMotorRunning_LA(On, bPositive, Slow);
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::WriteSignalToLast(LANE_ID LaneID, bool On)//PLC-送訊號給上一站
{
	if ( LANE_ID_B == LaneID )
	{	return WriteSignalToLast_LB(On); }
	return WriteSignalToLast_LA(On);
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::WriteSignalToNext(LANE_ID LaneID, bool On)//PLC-送訊號給下一站
{
	if ( LANE_ID_B == LaneID )
	{	return WriteSignalToNext_LB(On); }
	return WriteSignalToNext_LA(On);
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::WriteOKSignalToNext(LANE_ID LaneID, bool On)//PLC-送OK訊號給下一站
{
	if ( LANE_ID_B == LaneID )
	{	return WriteOKSignalToNext_LB(On); }
	return WriteOKSignalToNext_LA(On);
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::WriteNGSignalToNext(LANE_ID LaneID, bool On)//PLC-送NG訊號給下一站
{
	if ( LANE_ID_B == LaneID )
	{	return WriteNGSignalToNext_LB(On); }
	return WriteNGSignalToNext_LA(On);
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::WriteLockSignal(LANE_ID LaneID, bool On)//PLC-送鎖住訊號
{
	if ( LANE_ID_B == LaneID )
	{	return WriteLockSignal_LB(On); }
	return WriteLockSignal_LA(On);
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::TurnOffConveyerSensorPower(LANE_ID LaneID, bool On)//PLC-關閉軌道感應器電源
{
	if ( LANE_ID_B == LaneID )
	{	return TurnOffConveyerSensorPower_LB(On); }
	return TurnOffConveyerSensorPower_LA(On);
}
//--------------------------------------------------------------------------//
DWORD CPLC_Basic::GetPCBInTime(LANE_ID LaneID) const
{
	if ( LANE_ID_B == LaneID )
	{	return GetPCBInTime_LB(); }
	return GetPCBInTime_LA();
}
//--------------------------------------------------------------------------//
DWORD CPLC_Basic:: GetPCBOutTime(LANE_ID LaneID) const
{
	if ( LANE_ID_B == LaneID )
	{	return GetPCBOutTime_LB(); }
	return GetPCBOutTime_LA();
}
//--------------------------------------------------------------------------//
DWORD CPLC_Basic::GetPCBBackTime(LANE_ID LaneID) const
{
	if ( LANE_ID_B == LaneID )
	{	return GetPCBBackTime_LB(); }
	return GetPCBBackTime_LA();
}
//--------------------------------------------------------------------------//
DWORD CPLC_Basic::GetPCBInTime_LA() const
{
 	if ( m_PCBInTimeEnd_LA < m_PCBInTimeStart_LA ) { return 0; }
	return m_PCBInTimeEnd_LA-m_PCBInTimeStart_LA;	
}
//--------------------------------------------------------------------------//
DWORD CPLC_Basic::GetPCBOutTime_LA() const
{
	if ( m_PCBOutTimeEnd_LA < m_PCBOutTimeStart_LA ) { return 0; }
	return m_PCBOutTimeEnd_LA-m_PCBOutTimeStart_LA;
}
//--------------------------------------------------------------------------//
DWORD CPLC_Basic::GetPCBBackTime_LA() const
{
	if ( m_PCBBackTimeEnd_LA < m_PCBBackTimeStart_LA ) { return 0; }
	return m_PCBBackTimeEnd_LA-m_PCBBackTimeStart_LA;
}
//--------------------------------------------------------------------------//
DWORD CPLC_Basic::GetPCBInTime_LB() const
{
	if ( m_PCBInTimeEnd_LB < m_PCBInTimeStart_LB ) { return 0; }
	return m_PCBInTimeEnd_LB-m_PCBInTimeStart_LB;
}
//--------------------------------------------------------------------------//
DWORD CPLC_Basic::GetPCBOutTime_LB() const
{
	if ( m_PCBOutTimeEnd_LB < m_PCBOutTimeStart_LB ) { return 0; }
	return m_PCBOutTimeEnd_LB-m_PCBOutTimeStart_LB;
}
//--------------------------------------------------------------------------//
DWORD CPLC_Basic::GetPCBBackTime_LB() const
{
	if ( m_PCBBackTimeEnd_LB < m_PCBBackTimeStart_LB ) { return 0; }
	return m_PCBBackTimeEnd_LB-m_PCBBackTimeStart_LB;
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::CheckPCBInside(LANE_ID LaneID) const
{
	if ( LANE_ID_B == LaneID )
	{	return CheckPCBInside_LB(); }
	return CheckPCBInside_LA();
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::GetConveryerClamp(LANE_ID LaneID) const
{
	if ( LANE_ID_B == LaneID )
	{	return GetConveryerClamp_LB(); }
	return GetConveryerClamp_LA();
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::GetConveryerStopBar(LANE_ID LaneID) const
{
	if ( LANE_ID_B == LaneID )
	{	return GetConveryerStopBar_LB(); }
	return GetConveryerStopBar_LA();
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::GetConveryerSensorPCBIn(LANE_ID LaneID) const
{
	if ( LANE_ID_B == LaneID )
	{	return GetConveryerSensorPCBIn_LB(); }
	return GetConveryerSensorPCBIn_LA();
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::GetConveryerSensorPCBOut(LANE_ID LaneID) const
{
	if ( LANE_ID_B == LaneID )
	{	return GetConveryerSensorPCBOut_LB(); }
	return GetConveryerSensorPCBOut_LA();
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::GetConveryerSensorPCBStop(LANE_ID LaneID) const
{
	if ( LANE_ID_B == LaneID )
	{	return GetConveryerSensorPCBStop_LB(); }
	return GetConveryerSensorPCBStop_LA();
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::GetConveryerSensorPCBSlow(LANE_ID LaneID) const
{
	if ( LANE_ID_B == LaneID )
	{	return GetConveryerSensorPCBSlow_LB(); }
	return GetConveryerSensorPCBSlow_LA();
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::GetConveryerSensorPCBStop2(LANE_ID LaneID) const
{
	if ( LANE_ID_B == LaneID )
	{	return GetConveryerSensorPCBStop2_LB(); }
	return GetConveryerSensorPCBStop2_LA();
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::GetLastStationSignal(LANE_ID LaneID) const
{
	if ( LANE_ID_B == LaneID )
	{	return GetLastStationSignal_LB(); }
	return GetLastStationSignal_LA();
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::GetNextStationSignal(LANE_ID LaneID) const
{
	if ( LANE_ID_B == LaneID )
	{	return GetNextStationSignal_LB(); }
	return GetNextStationSignal_LA();
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::GetLockStationSignal(LANE_ID LaneID) const
{
	if ( LANE_ID_B == LaneID )
	{	return GetLockStationSignal_LB(); }
	return GetLockStationSignal_LA();
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::GetSendToLastStation(LANE_ID LaneID) const
{
	if ( LANE_ID_B == LaneID )
	{	return GetSendToLastStation_LB(); }
	return GetSendToLastStation_LA();
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::GetSendToNextStation(LANE_ID LaneID) const
{
	if ( LANE_ID_B == LaneID )
	{	return GetSendToNextStation_LB(); }
	return GetSendToNextStation_LA();
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::GetSendToNextStationOK(LANE_ID LaneID) const
{
	if ( LANE_ID_B == LaneID )
	{	return GetSendToNextStationOK_LB(); }
	return GetSendToNextStationOK_LA();
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::GetSendToNextStationNG(LANE_ID LaneID) const
{
	if ( LANE_ID_B == LaneID )
	{	return GetSendToNextStationNG_LB(); }
	return GetSendToNextStationNG_LA();
}
//--------------------------------------------------------------------------//
int CPLC_Basic::GetConveryerStatus(LANE_ID LaneID) const
{
	if ( LANE_ID_B == LaneID )
	{	return GetConveryerStatus_LB(); }
	return GetConveryerStatus_LA();
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::GetConveryerStatusText(int Val, CString &Text) const
{
	switch ( Val ) 
	{
	case PLC_CONVERYER_STATUS_STOP:				Text = _T("Stop");	break;
	case PLC_CONVERYER_STATUS_PCB_IN_WAITING:	Text = _T("Wait for PCB-In");	break;
	case PLC_CONVERYER_STATUS_PCB_IN_RUNNING:	Text = _T("PCB In Running");	break;

	case PLC_CONVERYER_STATUS_PCB_OUT_WAITING:	Text = _T("Wait for PCB-Out");	break;
	case PLC_CONVERYER_STATUS_PCB_OUT_RUNNING:	Text = _T("PCB-Out Running");	break;
	case PLC_CONVERYER_STATUS_PCB_OUT_FINISH:	Text = _T("PCB-Out Finish");	break;

	case PLC_CONVERYER_STATUS_PCB_BACK_RUNNING:	Text = _T("PCB-Back Running");	break;
	default:
		Text.Format(_T("Unexception[%d]"), Val);
		break;
	}
	return true;
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::CheckPCBInside_LA() const
{	
	if ( true == m_PLC_I_SensorPCBIn_LA ) { return true; }
	if ( true == m_PLC_I_SensorPCBOut_LA ) { return true; }
	if ( true == m_PLC_I_SensorPCBStop_LA ) { return true; }
	if ( true == m_PLC_I_SensorPCBSlow_LA ) { return true; }	
	if ( true == m_PLC_I_SensorPCBStop2_LA ) { return true; }	
	return false;
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::CheckPCBInside_LB() const
{
	if ( true == m_PLC_I_SensorPCBIn_LB ) { return true; }
	if ( true == m_PLC_I_SensorPCBOut_LB ) { return true; }
	if ( true == m_PLC_I_SensorPCBStop_LB ) { return true; }
	if ( true == m_PLC_I_SensorPCBSlow_LB ) { return true; }	
	if ( true == m_PLC_I_SensorPCBStop2_LB ) { return true; }	
	return false;
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::CheckPLCExecAlarm()//確認PLC執行異常
{
	if ( GetPLCExecAlarm() == false ) { return false; }	
	m_ErrorString = _T("Error, PLC Alram, Reset PLC First");
	m_ErrorString = LoadMultiLanguageString(m_ErrorString, m_ErrorString);
	return true;
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::IncrementPCBInCount(LANE_ID LaneID)//疊加進板確認次數
{
	if ( LANE_ID_B == LaneID )
	{	return IncrementPCBInCount_LB(); }
	return IncrementPCBInCount_LA();
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::IncrementPCBOutCount(LANE_ID LaneID)//疊加出板確認次數
{
	if ( LANE_ID_B == LaneID )
	{	return IncrementPCBOutCount_LB(); }
	return IncrementPCBOutCount_LA();
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::IncrementPCBBackCount(LANE_ID LaneID)//疊加回板確認次數
{
	if ( LANE_ID_B == LaneID )
	{	return IncrementPCBBackCount_LB(); }
	return IncrementPCBBackCount_LA();
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::IncrementPCBOutInCheckCount(LANE_ID LaneID)//疊加出板帶進板確認次數
{
	if ( LANE_ID_B == LaneID )
	{	return IncrementPCBOutInCheckCount_LB(); }
	return IncrementPCBOutInCheckCount_LA();
}
//--------------------------------------------------------------------------//
int CPLC_Basic::GetPCBInOutTimeout(LANE_ID LaneID) const
{
	if ( LANE_ID_B == LaneID )
	{	return GetPCBInOutTimeout_LB(); }
	return GetPCBInOutTimeout_LA();
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::GetLaneAdjustFixed14Lane() const//軌道調整-14軌固定
{
	if ( 0 == m_LaneAdjust_Fixed14Lane ) { return false; }
	return true;
}
//--------------------------------------------------------------------------//
int CPLC_Basic::GetLaneAdjustHomeTimeout() const//軌道調整-歸零逾時
{
	return m_LaneAdjust_HomeTimeout;
}
//--------------------------------------------------------------------------//
int CPLC_Basic::GetLaneAdjustMoveTimeout() const//軌道調整-歸零逾時
{
	return m_LaneAdjust_MoveTimeout;
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::ReadLaneAdjustHomeDone(LANE_ID LaneID)
{
	if ( LANE_ID_B == LaneID )
	{	return ReadLaneAdjustHomeDone_LB(); }
	return ReadLaneAdjustHomeDone_LA();
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::CheckLaneAdjustCanMove(LANE_ID LaneID)
{
	if ( LANE_ID_B == LaneID )
	{	return CheckLaneAdjustCanMove_LB(); }
	return CheckLaneAdjustCanMove_LA();
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::GetLaneAdjustCanMove(LANE_ID LaneID) const
{
	if ( LANE_ID_B == LaneID )
	{	return GetLaneAdjustCanMove_LB(); }
	return GetLaneAdjustCanMove_LA();
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::GetLaneAdjustHomeDone(LANE_ID LaneID) const
{
	if ( LANE_ID_B == LaneID )
	{	return GetLaneAdjustHomeDone_LB(); }
	return GetLaneAdjustHomeDone_LA();
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::GetLaneAdjustCanMove_LA() const//間距馬達-是否可以移動
{
	if ( GetPLCExecAlarm() == true ) { return false; }
	if ( GetLaneAdjustDisableBtn_LA() == true ) { return false; }
	if ( GetLaneAdjustHomeDone_LA() == false ) { return false; }
	return true;
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::GetLaneAdjustCanMove_LB() const//間距馬達-是否可以移動
{
	if ( GetPLCExecAlarm() == true ) { return false; }
	if ( GetLaneAdjustDisableBtn_LB() == true ) { return false; }
	if ( GetLaneAdjustHomeDone_LB() == false ) { return false; }
	return true;
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::CheckLaneAdjustCanMove_LA()//間距馬達-是否可以移動
{
	if ( CheckPLCExecAlarm() == true ) { return false; }
	if ( CheckLaneAdjustDisableBtn_LA() == true ) { return false; }
	if ( CheckLaneAdjustHomeDone_LA() == false ) { return false; }
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Basic::CheckLaneAdjustCanMove_LB()//間距馬達-是否可以移動
{
	if ( CheckPLCExecAlarm() == true ) { return false; }
	if ( CheckLaneAdjustDisableBtn_LB() == true ) { return false; }
	if ( CheckLaneAdjustHomeDone_LB() == false ) { return false; }
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Basic::CheckLaneAdjustHomeDone_LA()//間距馬達-是否已經歸零
{
	if ( GetLaneAdjustHomeDone_LA() == true ) { return true; }
	m_ErrorString = _T("Error, Lane Adjust does not home LA");
	m_ErrorString = LoadMultiLanguageString(m_ErrorString, m_ErrorString);
	return false;
}
//-----------------------------------------------------------------------//
bool CPLC_Basic::CheckLaneAdjustHomeDone_LB()//間距馬達-是否已經歸零
{
	if ( GetLaneAdjustHomeDone_LB() == true ) { return true; }
	m_ErrorString = _T("Error, Lane Adjust does not home LB");
	m_ErrorString = LoadMultiLanguageString(m_ErrorString, m_ErrorString);
	return false;
}
//-----------------------------------------------------------------------//
bool CPLC_Basic::CheckLaneAdjustDisableBtn_LA()//間距馬達-是否消磁	
{
	if ( GetLaneAdjustDisableBtn_LA() == false )
	{	return false; }
	m_ErrorString = _T("Error, please push up Lane Adjust button first");
	m_ErrorString = LoadMultiLanguageString(m_ErrorString, m_ErrorString);		
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Basic::CheckLaneAdjustDisableBtn_LB()//間距馬達-是否消磁	
{
	if ( GetLaneAdjustDisableBtn_LB() == false )
	{	return false; }
	m_ErrorString = _T("Error, please push up Lane Adjust button first");
	m_ErrorString = LoadMultiLanguageString(m_ErrorString, m_ErrorString);		
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Basic::ExecLaneAdjustMoveToPos(LANE_ID LaneID, double Pos)
{
	if ( LANE_ID_B == LaneID ) 
	{	return ExecLaneAdjustMoveTo_LB(Pos, true); }
	return ExecLaneAdjustMoveTo_LA(Pos, true);
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::ExecLaneAdjustHomeSearch(LANE_ID LaneID, bool Wait)
{
	if ( LANE_ID_B == LaneID ) 
	{	return ExecLaneAdjustHomeSearch_LB(Wait); }
	return ExecLaneAdjustHomeSearch_LA(Wait);
}
//--------------------------------------------------------------------------//
double CPLC_Basic::GetLaneAdjustCurrentPos(LANE_ID LaneID) const
{
	if ( LANE_ID_B == LaneID ) 
	{	return GetLaneAdjustCurrentPos_LB(); }
	return GetLaneAdjustCurrentPos_LA();
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::SetLaneAdjustCurrentPos(LANE_ID LaneID, double Pos)
{
	if ( LANE_ID_B == LaneID ) 
	{	SetLaneAdjustCurrentPos_LB(Pos); }
	else 
	{	SetLaneAdjustCurrentPos_LA(Pos); }
	return true;
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::GetLaneAdjustHomeDone_LA() const
{
	return m_LaneAdjust_Homed_LA;
}
//--------------------------------------------------------------------------//
double CPLC_Basic::GetLaneAdjustLimitMax_LA() const
{
	return m_LaneAdjust_LimitMax_LA;
}
//--------------------------------------------------------------------------//
double CPLC_Basic::GetLaneAdjustLimitMin_LA() const
{
	return m_LaneAdjust_LimitMin_LA;
}
//--------------------------------------------------------------------------//
double CPLC_Basic::GetLaneAdjustSkewPitch_LA() const//間距馬達-設定移動距離
{
	return m_LaneAdjust_SkewPitch_LA;
}
//--------------------------------------------------------------------------//
double CPLC_Basic::GetLaneAdjustCurrentPos_LA() const
{	
	return m_LaneAdjust_CurrentPos_LA;
}
//--------------------------------------------------------------------------//
void CPLC_Basic::SetLaneAdjustCurrentPos_LA(double val)
{
	m_LaneAdjust_CurrentPos_LA = val;
}
//--------------------------------------------------------------------------//
double CPLC_Basic::GetLaneAdjustJogSlowSpeed_LA() const
{
	return m_LaneAdjust_JogSlowSpeed_LA;
}
//--------------------------------------------------------------------------//
double CPLC_Basic::GetLaneAdjustJogFastSpeed_LA() const
{
	return m_LaneAdjust_JogFastSpeed_LA;
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::GetLaneAdjustSensorORG_LA() const
{
	return m_LaneAdjust_SensorORG_LA;
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::GetLaneAdjustSensorLimit_LA() const
{
	return m_LaneAdjust_SensorLimit_LA;
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::GetLaneAdjustJogMoving_LA() const
{
	return m_LaneAdjust_JogMoving_LA;
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::CheckLaneAdjustPCBInside(LANE_ID LaneID)//間距馬達-確認PCB板在內
{
	if ( LANE_ID_B == LaneID )
	{	return CheckLaneAdjustPCBInside_LB(); }
	return CheckLaneAdjustPCBInside_LA();
}
//--------------------------------------------------------------------------//
double CPLC_Basic::ReadLaneAdjustCurrentPos(LANE_ID LaneID)//間距馬達-取回目前位置	
{
	if ( LANE_ID_B == LaneID )
	{	return ReadLaneAdjustCurrentPos_LB(); }
	return ReadLaneAdjustCurrentPos_LA();
}
//-----------------------------------------------------------------------//
bool CPLC_Basic::GetLaneAdjustDisableBtn_LA() const//間距馬達-是否消磁	
{
	return m_LaneAdjust_Disable_LA;
}
//-----------------------------------------------------------------------//
bool CPLC_Basic::GetLaneAdjustDisableBtn_LB() const//間距馬達-是否消磁	
{
	return m_LaneAdjust_Disable_LB;
}
//-----------------------------------------------------------------------//
bool CPLC_Basic::GetLaneAdjustDisableBtn(LANE_ID LaneID) const
{	
	if ( LANE_ID_B == LaneID )
	{	return GetLaneAdjustDisableBtn_LB(); }
	return GetLaneAdjustDisableBtn_LA();	
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::CheckLaneAdjustPCBInside_LA()
{
	bool Inside = false;
	if ( true == m_PLC_I_SensorPCBIn_LA ) { Inside = true; }
	if ( true == m_PLC_I_SensorPCBOut_LA ) { Inside =  true; }
	if ( true == m_PLC_I_SensorPCBStop_LA ) { Inside =  true; }
	if ( true == m_PLC_I_SensorPCBSlow_LA ) { Inside =  true; }	
	if ( true == m_PLC_I_SensorPCBStop2_LA ) { Inside = true; }
	if ( true == Inside )
	{
		m_ErrorString = _T("Error, there is one board on the conveyer LA");
		m_ErrorString = LoadMultiLanguageString(m_ErrorString, m_ErrorString);	
		return true;
	}
	return false;
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::ExecLaneAdjustHomeSearch_LA(bool Wait)//間距馬達-歸零搜尋
{
	CString str;
#ifndef PLC_OBJ_DISABLE
	const bool bCheckThread = false;
	if ( CheckLaneAdjustDisableBtn_LA() == true )
	{	return false;	}	
	if ( CheckPLCExecAlarm() == true )
	{	return false;	}
	if ( ExecPCBClear_LA(true) == false )
	{	return false;	}
	//::Sleep(1000);
	if ( PLC_ReadConveryerSensor(bCheckThread) == false )
	{	return false;	}
	if ( CheckLaneAdjustPCBInside_LA() == true )
	{	return false;	}
	if ( WriteLaneAdjustToHome_LA(true) == false )
	{	return false; }
	
	if ( false == Wait ) { return true; }

	int i=0;
	const int MaxCount = 300;
	const int SleepTime = 100;	
	for ( i=0; i<MaxCount; i++ )
	{
		if ( ReadLaneAdjustExecption_LA() == true )
		{	
			ReadLaneAdjustCurrentPos_LA();
			ResetLaneAdjustException_LA();
			WriteLaneAdjustToHome_LA(false);
			SetPLCExceptionCode(AOI_EXCEPTION_PLC_LANE_ADJUST_HOME_LA);
			return false;			
		}

		if ( ReadLaneAdjustHomeDone_LA() == true ) 
		{
			ReadLaneAdjustCurrentPos_LA();
			WriteLaneAdjustToHome_LA(false);			
			return true;
		}
		::Sleep(SleepTime);		
	}
	str = _T("Error, Lane Adjust Wait for Home done too long LA");
	str = LoadMultiLanguageString(str, str);
	m_ErrorString = str;
	SetPLCExceptionCode(AOI_EXCEPTION_PLC_LANE_ADJUST_HOME_LA);
	ReadLaneAdjustCurrentPos_LA();
	ResetLaneAdjustException_LA();
	WriteLaneAdjustToHome_LA(false);
	return false;
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Basic::ExecLaneAdjustMoveTo_LA(double Pos, bool Wait)//間距馬達-移動至
{
	CString str;
#ifndef PLC_OBJ_DISABLE
	if ( CheckLaneAdjustDisableBtn_LA() == true )
	{	return false;	}
	if ( CheckLaneAdjustPCBInside_LA() == true )
	{	return false;	}
	if ( WriteLaneAdjustCmdPos_LA(Pos) == false )
	{	return false; }	
	if ( WriteLaneAdjustToMove_LA(true) == false )
	{	return false; }

	if ( false == Wait ) { return true; }

	int i=0;
	const int MaxCount = 300;
	const int SleepTime = 100;	
	for ( i=0; i<MaxCount; i++ )
	{
		if ( ReadLaneAdjustExecption_LA() == true )
		{	
			ResetLaneAdjustException_LA();
			WriteLaneAdjustToMove_LA(false);
			SetPLCExceptionCode(AOI_EXCEPTION_PLC_LANE_ADJUST_MOVE_LA);
			return false;			
		}

		if ( ReadLaneAdjustMoveDone_LA() == true ) 
		{
			SetLaneAdjustCurrentPos_LA(Pos);			
			WriteLaneAdjustToMove_LA(false);			
			return true;
		}
		::Sleep(SleepTime);		
	}
	str = _T("Error, Lane Adjust Wait for Move done too long LA");
	str = LoadMultiLanguageString(str, str);
	m_ErrorString = str;	
	SetPLCExceptionCode(AOI_EXCEPTION_PLC_LANE_ADJUST_MOVE_LA);
	ResetLaneAdjustException_LA();
	WriteLaneAdjustToMove_LA(false);
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Basic::ExecLaneAdjustJogMove_LA(bool Dir, bool Stop)//間距馬達-Jog移動
{
	CString str;
#ifndef PLC_OBJ_DISABLE
	if ( CheckLaneAdjustDisableBtn_LA() == true )
	{	return false;	}
	if ( CheckLaneAdjustPCBInside_LA() == true )
	{	return false;	}

	if ( WriteLaneAdjustJogMove_LA(Dir, Stop) == false )
	{	
		WriteLaneAdjustJogMove_LA(Dir, true);
		return false;
	}
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Basic::GetLaneAdjustHomeDone_LB() const
{
	return m_LaneAdjust_Homed_LB;
}
//--------------------------------------------------------------------------//
double CPLC_Basic::GetLaneAdjustLimitMax_LB() const
{
	return m_LaneAdjust_LimitMax_LB;
}
//--------------------------------------------------------------------------//
double CPLC_Basic::GetLaneAdjustLimitMin_LB() const
{
	return m_LaneAdjust_LimitMin_LB;
}
//--------------------------------------------------------------------------//
double CPLC_Basic::GetLaneAdjustSkewPitch_LB() const//間距馬達-設定移動距離
{
	return m_LaneAdjust_SkewPitch_LB;
}
//--------------------------------------------------------------------------//
double CPLC_Basic::GetLaneAdjustCurrentPos_LB() const
{
	return m_LaneAdjust_CurrentPos_LB;
}
//--------------------------------------------------------------------------//
void CPLC_Basic::SetLaneAdjustCurrentPos_LB(double val)
{
	m_LaneAdjust_CurrentPos_LB = val;
}
//--------------------------------------------------------------------------//
double CPLC_Basic::GetLaneAdjustJogSlowSpeed_LB() const
{
	return m_LaneAdjust_JogSlowSpeed_LB;
}
//--------------------------------------------------------------------------//
double CPLC_Basic::GetLaneAdjustJogFastSpeed_LB() const
{
	return m_LaneAdjust_JogFastSpeed_LB;
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::GetLaneAdjustSensorORG_LB() const
{
	return m_LaneAdjust_SensorORG_LB;
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::GetLaneAdjustSensorLimit_LB() const
{
	return m_LaneAdjust_SensorLimit_LB;
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::GetLaneAdjustJogMoving_LB() const
{
	return m_LaneAdjust_JogMoving_LB;
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::CheckLaneAdjustPCBInside_LB()
{
	bool Inside = false;
	if ( true == m_PLC_I_SensorPCBIn_LB ) { Inside = true; }
	if ( true == m_PLC_I_SensorPCBOut_LB ) { Inside =  true; }
	if ( true == m_PLC_I_SensorPCBStop_LB ) { Inside =  true; }
	if ( true == m_PLC_I_SensorPCBSlow_LB ) { Inside =  true; }
	if ( true == m_PLC_I_SensorPCBStop2_LB ) { Inside = true; }
	if ( true == Inside )
	{
		m_ErrorString = _T("Error, there is one board on the conveyer LB");
		m_ErrorString = LoadMultiLanguageString(m_ErrorString, m_ErrorString);	
		return true;
	}
	return false;
}
//--------------------------------------------------------------------------//
bool CPLC_Basic::ExecLaneAdjustHomeSearch_LB(bool Wait)//間距馬達-歸零搜尋
{
	CString str;
#ifndef PLC_OBJ_DISABLE
	const bool bCheckThread = false;
	if ( CheckLaneAdjustDisableBtn_LB() == true )
	{	return false;	}	
	if ( CheckPLCExecAlarm() == true )
	{	return false;	}
	if ( ExecPCBClear_LB(true) == false )
	{	return false;	}	
	//::Sleep(1000);
	if ( PLC_ReadConveryerSensor(bCheckThread) == false )
	{	return false;	}
	if ( CheckLaneAdjustPCBInside_LB() == true )
	{	return false;	}
	if ( WriteLaneAdjustToHome_LB(true) == false )
	{	return false; }
	
	if ( false == Wait ) { return true; }

	int i=0;
	const int MaxCount = 300;
	const int SleepTime = 100;	
	for ( i=0; i<MaxCount; i++ )
	{
		if ( ReadLaneAdjustExecption_LB() == true )
		{	
			ReadLaneAdjustCurrentPos_LB();
			ResetLaneAdjustException_LB();
			WriteLaneAdjustToHome_LB(false);
			SetPLCExceptionCode(AOI_EXCEPTION_PLC_LANE_ADJUST_HOME_LB);
			return false;			
		}

		if ( ReadLaneAdjustHomeDone_LB() == true ) 
		{
			ReadLaneAdjustCurrentPos_LB();
			WriteLaneAdjustToHome_LB(false);			
			return true;
		}
		::Sleep(SleepTime);		
	}
	str = _T("Error, Lane Adjust Wait for Home done too long LB");
	str = LoadMultiLanguageString(str, str);
	m_ErrorString = str;
	SetPLCExceptionCode(AOI_EXCEPTION_PLC_LANE_ADJUST_HOME_LB);
	ReadLaneAdjustCurrentPos_LB();
	ResetLaneAdjustException_LB();
	WriteLaneAdjustToHome_LB(false);
	return false;
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Basic::ExecLaneAdjustMoveTo_LB(double Pos, bool Wait)//間距馬達-移動至
{
	CString str;
#ifndef PLC_OBJ_DISABLE
	if ( CheckLaneAdjustDisableBtn_LB() == true )
	{	return false;	}
	if ( CheckLaneAdjustPCBInside_LB() == true )
	{	return false;	}
	if ( WriteLaneAdjustCmdPos_LB(Pos) == false )
	{	return false; }
	
	if ( WriteLaneAdjustToMove_LB(true) == false )
	{	return false; }

	if ( false == Wait ) { return true; }

	int i=0;
	const int MaxCount = 300;
	const int SleepTime = 100;	
	for ( i=0; i<MaxCount; i++ )
	{
		if ( ReadLaneAdjustExecption_LB() == true )
		{	
			ResetLaneAdjustException_LB();
			WriteLaneAdjustToMove_LA(false);
			SetPLCExceptionCode(AOI_EXCEPTION_PLC_LANE_ADJUST_MOVE_LB);
			return false;			
		}

		if ( ReadLaneAdjustMoveDone_LB() == true ) 
		{
			SetLaneAdjustCurrentPos_LB(Pos);			
			WriteLaneAdjustToMove_LB(false);			
			return true;
		}
		::Sleep(SleepTime);		
	}
	str = _T("Error, Lane Adjust Wait for Move done too long LB");
	str = LoadMultiLanguageString(str, str);
	m_ErrorString = str;	
	SetPLCExceptionCode(AOI_EXCEPTION_PLC_LANE_ADJUST_MOVE_LB);
	ResetLaneAdjustException_LB();
	WriteLaneAdjustToMove_LB(false);
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Basic::ExecLaneAdjustJogMove_LB(bool Dir, bool Stop)//間距馬達-Jog移動
{
	CString str;
#ifndef PLC_OBJ_DISABLE
	if ( CheckLaneAdjustDisableBtn_LB() == true )
	{	return false;	}
	if ( CheckLaneAdjustPCBInside_LB() == true )
	{	return false;	}

	if ( WriteLaneAdjustJogMove_LB(Dir, Stop) == false )
	{	
		WriteLaneAdjustJogMove_LB(Dir, true);
		return false;
	}
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//