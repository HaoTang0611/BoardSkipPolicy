// PlcLaneStatusPane.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "PlcLaneStatusPane.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
const int PLC_LANE_STATUS_TIMER    = 100;
const int PLC_LANE_REPEAT_LOOP_LA  = 201;
const int PLC_LANE_REPEAT_LOOP_LB  = 202;
//-------------------------------------------------------------------------------------//
#define   LANE_LOOP_MODE_STOP             0
#define   LANE_LOOP_MODE_CHECK_PCB_IN     1
#define   LANE_LOOP_MODE_CHECK_PCB_BACK   2
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CPLCCtrlPaneLaneStatus dialog
//-------------------------------------------------------------------------------------//
CPLCCtrlPaneLaneStatus::CPLCCtrlPaneLaneStatus(CWnd* pParent /*=NULL*/)
	: CDialog(CPLCCtrlPaneLaneStatus::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPLCCtrlPaneLaneStatus)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_CurrentAirStats = -1;
	m_CurrentFanStats = -1;
	m_CurrentEMSStats = -1;	
	m_CurrentRearDoorStats = -1;	
	m_CurrentFrontCapStats = -1;	
	m_CurrentKeySwitchStats = -1;
	m_CurrentOverHeatStats = -1;
	m_GreenLightStats = -1;
	m_HardBypassStats_LA = -1;
	m_HardBypassStats_LB = -1;

	m_PCBInDirection = -1;
	m_ConveryerSensorPCBIn_LA = -1;
	m_ConveryerSensorPCBSlow_LA = -1;
	m_ConveryerSensorPCBStop_LA = -1;
	m_ConveryerSensorPCBOut_LA = -1;
	m_ConveryerSensorPCBStop_LA2 = -1;
	m_ConveryerSensorPCBIn_LB = -1;
	m_ConveryerSensorPCBSlow_LB = -1;
	m_ConveryerSensorPCBStop_LB = -1;
	m_ConveryerSensorPCBOut_LB = -1;
	m_ConveryerSensorPCBStop_LB2 = -1;

	m_LaneLoopMode_LA = LANE_LOOP_MODE_STOP;
	m_LaneLoopCount_LA = 0;
	m_LaneLoopMode_LB = LANE_LOOP_MODE_STOP;
	m_LaneLoopCount_LB = 0;
	m_SignalFromLast_LA = -1;
	m_SignalFromNext_LA = -1;
	m_SignalFromLast_LB = -1;
	m_SignalFromNext_LB = -1;
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneStatus::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPLCCtrlPaneLaneStatus)
	DDX_Control(pDX, PLCLANE_PCB_RIGHT_IN_IMG, m_PCBRightInWnd);
	DDX_Control(pDX, PLCLANE_PCB_LEFT_IN_IMG, m_PCBLeftInWnd);
	DDX_Control(pDX, PLCLANE_KEY_SWITCH_IMG, m_KeySwitchWnd);
	DDX_Control(pDX, PLCLANE_HARDWARE_BYPASS_IMG_LA, m_HardwareBypassWnd_LA);	
	DDX_Control(pDX, PLCLANE_HARDWARE_BYPASS_IMG_LB, m_HardwareBypassWnd_LB);	
	DDX_Control(pDX, PLCLANE_CURRENT_START_LIGHT_IMG, m_StartLightWnd);
	DDX_Control(pDX, PLCLANE_CURRENT_OVER_HEAT_IMG, m_CurrentOverHeatWnd);
	DDX_Control(pDX, PLCLANE_CURRENT_REAR_DOOR_IMG, m_CurrentRearDoorWnd);
	DDX_Control(pDX, PLCLANE_CURRENT_FAN_IMG, m_CurrentFanWnd);
	DDX_Control(pDX, PLCLANE_CURRENT_EMS_IMG, m_CurrentEMSWnd);
	DDX_Control(pDX, PLCLANE_CURRENT_CAP_IMG, m_CurrentCapWnd);
	DDX_Control(pDX, PLCLANE_CURRENT_AIR_IMG, m_CurrentAirWnd);
	DDX_Control(pDX, PLCLANE_ALARM_REAR_DOOR_IMG, m_AlarmRearDoorWnd);
	DDX_Control(pDX, PLCLANE_ALARM_FAN_IMG, m_AlarmFanWnd);
	DDX_Control(pDX, PLCLANE_ALARM_EMS_IMG, m_AlarmEMSWnd);
	DDX_Control(pDX, PLCLANE_ALARM_AIR_IMG, m_AlarmAirWnd);
	DDX_Control(pDX, PLCLANE_ALARM_CAP_IMG, m_AlarmCapWnd);
	DDX_Control(pDX, PLCLANE_FROM_NEXT_SIGNAL_IMG_LB, m_FromNextSignalWnd_LB);
	DDX_Control(pDX, PLCLANE_FROM_LAST_SIGNAL_IMG_LB, m_FromLastSignalWnd_LB);
	DDX_Control(pDX, PLCLANE_PCB_IN_SENSOR_IMG_LB, m_PCBInSensorWnd_LB);
	DDX_Control(pDX, PLCLANE_PCB_OUT_SENSOR_IMG_LB, m_PCBOutSensorWnd_LB);
	DDX_Control(pDX, PLCLANE_IN_POSITION_SENSOR_IMG_LB, m_InPositionSensorWnd_LB);
	DDX_Control(pDX, PLCLANE_IN_POSITION_SENSOR_IMG_LB2, m_InPositionSensorWnd_LB2);
	DDX_Control(pDX, PLCLANE_SLOW_DOWN_SENSOR_IMG_LB, m_SlowDownSensorWnd_LB);
	DDX_Control(pDX, PLCLANE_FROM_NEXT_SIGNAL_IMG_LA, m_FromNextSignalWnd_LA);
	DDX_Control(pDX, PLCLANE_FROM_LAST_SIGNAL_IMG_LA, m_FromLastSignalWnd_LA);
	DDX_Control(pDX, PLCLANE_PCB_IN_SENSOR_IMG_LA, m_PCBInSensorWnd_LA);
	DDX_Control(pDX, PLCLANE_PCB_OUT_SENSOR_IMG_LA, m_PCBOutSensorWnd_LA);
	DDX_Control(pDX, PLCLANE_IN_POSITION_SENSOR_IMG_LA, m_InPositionSensorWnd_LA);
	DDX_Control(pDX, PLCLANE_IN_POSITION_SENSOR_IMG_LA2, m_InPositionSensorWnd_LA2);
	DDX_Control(pDX, PLCLANE_SLOW_DOWN_SENSOR_IMG_LA, m_SlowDownSensorWnd_LA);	
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CPLCCtrlPaneLaneStatus, CDialog)
	//{{AFX_MSG_MAP(CPLCCtrlPaneLaneStatus)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_BN_CLICKED(PLCLANE_SEND_TO_LAST_SIGNAL_CHK_LA, OnSendToLastSignalChkLA)
	ON_BN_CLICKED(PLCLANE_SEND_TO_NEXT_SIGNAL_CHK_LA, OnSendToNextSignalChkLA)
	ON_BN_CLICKED(PLCLANE_SEND_OK_SIGNAL_CHK_LA, OnSendOkSignalChkLA)
	ON_BN_CLICKED(PLCLANE_SEND_NG_SIGNAL_CHK_LA, OnSendNgSignalChkLA)
	ON_BN_CLICKED(PLCLANE_PCB_IN_BTN_LA, OnPCBInBtnLA)
	ON_BN_CLICKED(PLCLANE_PCB_BACK_BTN_LA, OnPCBBackBtnLA)
	ON_BN_CLICKED(PLCLANE_PCB_BACK_OUT_BTN_LA, OnPCBBackOutBtnLA)
	ON_BN_CLICKED(PLCLANE_PCB_OUT_BTN_LA, OnPCBOutBtnLA)
	ON_BN_CLICKED(PLCLANE_PCB_REIN_BTN_LA, OnPCBReinBtnLA)
	ON_BN_CLICKED(PLCLANE_PCB_IN_BACK_REPEAT_CHK_LA, OnPCBInBackRepeatChkLA)
	ON_BN_CLICKED(PLCLANE_PCB_CLAMP_CHK_LA, OnPCBClampChkLA)
	ON_BN_CLICKED(PLCLANE_PCB_STOPBAR_CHK_LA, OnPCBStopBarChkLA)	
	ON_BN_CLICKED(PLCLANE_PCB_IN_BTN_LB, OnPCBInBtnLB)
	ON_BN_CLICKED(PLCLANE_PCB_BACK_BTN_LB, OnPCBBackBtnLB)
	ON_BN_CLICKED(PLCLANE_PCB_BACK_OUT_BTN_LB, OnPCBBackOutBtnLB)
	ON_BN_CLICKED(PLCLANE_PCB_OUT_BTN_LB, OnPCBOutBtnLB)
	ON_BN_CLICKED(PLCLANE_PCB_REIN_BTN_LB, OnPCBReInBtnLB)
	ON_BN_CLICKED(PLCLANE_PCB_CLAMP_CHK_LB, OnPCBClampChkLB)
	ON_BN_CLICKED(PLCLANE_PCB_STOPBAR_CHK_LB, OnPCBStopBarChkLB)	
	ON_BN_CLICKED(PLCLANE_SEND_TO_LAST_SIGNAL_CHK_LB, OnSendToLastSignalChkLB)
	ON_BN_CLICKED(PLCLANE_SEND_TO_NEXT_SIGNAL_CHK_LB, OnSendToNextSignalChkLB)
	ON_BN_CLICKED(PLCLANE_SEND_OK_SIGNAL_CHK_LB, OnSendOkSignalChkLB)
	ON_BN_CLICKED(PLCLANE_SEND_NG_SIGNAL_CHK_LB, OnSendNgSignalChkLB)
	ON_BN_CLICKED(PLCLANE_PCB_IN_BACK_REPEAT_CHK_LB, OnPCBInBackRepeatChkLB)
	ON_BN_CLICKED(PLCLANE_SEND_STAGE_ALARM_CHK_LA, OnSendStageAlarmChkLA)
	ON_BN_CLICKED(PLCLANE_SEND_INSPECTION_ALARM_CHK_LA, OnSendInspectionAlarmChkLA)
	ON_BN_CLICKED(PLCLANE_SEND_STAGE_ALARM_CHK_LB, OnSendStageAlarmChkLB)
	ON_BN_CLICKED(PLCLANE_SEND_INSPECTION_ALARM_CHK_LB, OnSendInspectionAlarmChkLB)	
	ON_BN_CLICKED(PLCLANE_PCB_IN_2ND_BTN_LA, OnPCBIn2ndBtnLA)
	ON_BN_CLICKED(PLCLANE_PCB_IN_3RD_BTN_LA, OnPCBIn3rdBtnLA)
	ON_BN_CLICKED(PLCLANE_PCB_IN_2ND_BTN_LB, OnPCBIn2ndBtnLB)
	ON_BN_CLICKED(PLCLANE_PCB_IN_3RD_BTN_LB, OnPCBIn3rdBtnLB)
	ON_BN_CLICKED(PLCLANE_PCB_AUTO_OUT_IN_BTN_LA, OnPCBAutoOutInBtnLA)
	ON_BN_CLICKED(PLCLANE_PCB_AUTO_BACK_IN_BTN_LA, OnPCBAutoBackInBtnLA)
	ON_BN_CLICKED(PLCLANE_PCB_AUTO_OUT_IN_BTN_LB, OnPCBAutoOutInBtnLB)
	ON_BN_CLICKED(PLCLANE_PCB_AUTO_BACK_IN_BTN_LB, OnPCBAutoBackInBtnLB)
	ON_BN_CLICKED(PLCLANE_MACHINE_START_BTN, OnMachineStartBtn)
	ON_BN_CLICKED(PLCLANE_MACHINE_RESET_BTN, OnMachineResetBtn)
	ON_BN_CLICKED(PLCLANE_MACHINE_STOP_BTN, OnMachineStopBtn)
	ON_BN_CLICKED(PLCLANE_PCB_CLEAR_BTN_LA, OnPCBClearBtnLA)
	ON_BN_CLICKED(PLCLANE_PCB_CLEAR_BTN_LB, OnPCBClearBtnLB)
	ON_BN_CLICKED(PLCLANE_PCB_OUT_WITH_IN_BTN_LA, OnPCBOutWithInBtnLA)
	ON_BN_CLICKED(PLCLANE_PCB_OUT_WITH_IN_BTN_LB, OnPCBOutWithInBtnLB)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CPLCCtrlPaneLaneStatus message handlers
//-------------------------------------------------------------------------------------//
BOOL CPLCCtrlPaneLaneStatus::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	this->m_LEDGreen.LoadBitmap(IDB_LED_MEDIAN_GREEN);
	this->m_LEDRed.LoadBitmap(IDB_LED_MEDIAN_RED);
	this->m_LEDGray.LoadBitmap(IDB_LED_MEDIAN_GRAY);

	this->m_LEDGreenSmall.LoadBitmap(IDB_LED_SMALL_GREEN);
	this->m_LEDGraySmall.LoadBitmap(IDB_LED_SMALL_GRAY);
	this->m_LEDRedSmall.LoadBitmap(IDB_LED_SMALL_RED);

	this->m_RepeatCount = 0;
	this->SwitchMultiLanguage();
	EnableLaneStatus();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneStatus::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneStatus::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneStatus::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if ( TRUE == bShow )
	{
		EnableLaneStatus();
		SwitchMultiLanguage();
		if ( PlcCtrlPtr->GetPLCIsConnected() == true )
		{
			if ( PlcCtrlPtr->GetStageAlarm_LA() == true )
			{	CWnd::CheckDlgButton(PLCLANE_SEND_STAGE_ALARM_CHK_LA, TRUE); }
			else
			{	CWnd::CheckDlgButton(PLCLANE_SEND_STAGE_ALARM_CHK_LA, FALSE); }

			if ( PlcCtrlPtr->GetInspectionAlarm_LA() == true )
			{	CWnd::CheckDlgButton(PLCLANE_SEND_INSPECTION_ALARM_CHK_LA, TRUE); }
			else
			{	CWnd::CheckDlgButton(PLCLANE_SEND_INSPECTION_ALARM_CHK_LA, FALSE); }

			if ( PlcCtrlPtr->GetStageAlarm_LB() == true )
			{	CWnd::CheckDlgButton(PLCLANE_SEND_STAGE_ALARM_CHK_LB, TRUE); }
			else
			{	CWnd::CheckDlgButton(PLCLANE_SEND_STAGE_ALARM_CHK_LB, FALSE); }

			if ( PlcCtrlPtr->GetInspectionAlarm_LB() == true )
			{	CWnd::CheckDlgButton(PLCLANE_SEND_INSPECTION_ALARM_CHK_LB, TRUE); }
			else
			{	CWnd::CheckDlgButton(PLCLANE_SEND_INSPECTION_ALARM_CHK_LB, FALSE); }		
		}
		this->SetTimer(PLC_LANE_STATUS_TIMER, 250, 0);	
	}
	else
	{			
		if ( CWnd::GetSafeHwnd() != NULL )
		{
			CWnd::CheckDlgButton(PLCLANE_PCB_IN_BACK_REPEAT_CHK_LA, FALSE);
			CWnd::CheckDlgButton(PLCLANE_PCB_IN_BACK_REPEAT_CHK_LB, FALSE);
		}
		this->KillTimer(PLC_LANE_REPEAT_LOOP_LA);		
		this->KillTimer(PLC_LANE_REPEAT_LOOP_LB);		
		this->KillTimer(PLC_LANE_STATUS_TIMER);		
	}
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneStatus::OnOK() 
{
	// TODO: Add extra validation here
	return;
	CDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneStatus::OnCancel() 
{
	// TODO: Add extra cleanup here
	return;
	CDialog::OnCancel();
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneStatus::OnTimer(UINT_PTR nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	switch ( nIDEvent )
	{
	case PLC_LANE_STATUS_TIMER:	
		this->UpdateLaneStatus();		
		break;
	case PLC_LANE_REPEAT_LOOP_LA:
		CWnd::KillTimer(PLC_LANE_REPEAT_LOOP_LA);	
		if ( ExecLaneLoop_LA() == false )
		{	
			m_LaneLoopMode_LA = LANE_LOOP_MODE_STOP;	
			CWnd::CheckDlgButton(PLCLANE_PCB_IN_BACK_REPEAT_CHK_LA, FALSE);			
		}
		else
		{	CWnd::SetTimer(PLC_LANE_REPEAT_LOOP_LA, 100, NULL); }
		break;
	case PLC_LANE_REPEAT_LOOP_LB:
		CWnd::KillTimer(PLC_LANE_REPEAT_LOOP_LB);	
		if ( ExecLaneLoop_LB() == false )
		{	
			m_LaneLoopMode_LB = LANE_LOOP_MODE_STOP;	
			CWnd::CheckDlgButton(PLCLANE_PCB_IN_BACK_REPEAT_CHK_LB, FALSE);			
		}
		else
		{	CWnd::SetTimer(PLC_LANE_REPEAT_LOOP_LB, 100, NULL); }
		break;
	}
	CDialog::OnTimer(nIDEvent);
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneStatus::UpdateLaneStatus()
{	
	int nValue = 0;
	const int TurnOn = 1;	
	const TPLCParameter &PlcParam = PlcCtrlPtr->GetPLCParameter();
	nValue = PlcCtrlPtr->GetCurrentAirStats();
	if ( nValue != m_CurrentAirStats )
	{
		if ( TurnOn == nValue )
		{	this->m_CurrentAirWnd.SetBitmap(m_LEDRed); }
		else
		{	this->m_CurrentAirWnd.SetBitmap(m_LEDGreen); }
	}
	m_CurrentAirStats = nValue;

	nValue = PlcCtrlPtr->GetCurrentFrontCapStats();
	if ( nValue != m_CurrentFrontCapStats )
	{
		if ( TurnOn == nValue )
		{	this->m_CurrentCapWnd.SetBitmap(m_LEDRed); }
		else
		{	this->m_CurrentCapWnd.SetBitmap(m_LEDGreen); }
	}
	m_CurrentFrontCapStats = nValue;

	nValue = PlcCtrlPtr->GetCurrentRearCapStats();
	if ( nValue != m_CurrentRearDoorStats )
	{
		if ( TurnOn == nValue )
		{	this->m_CurrentRearDoorWnd.SetBitmap(m_LEDRed); }
		else
		{	this->m_CurrentRearDoorWnd.SetBitmap(m_LEDGreen); }
	}
	m_CurrentRearDoorStats = nValue;

	nValue = PlcCtrlPtr->GetCurrentEMSStats();
	if ( nValue != m_CurrentEMSStats )
	{
		if ( TurnOn == nValue )
		{	this->m_CurrentEMSWnd.SetBitmap(m_LEDRed); }
		else
		{	this->m_CurrentEMSWnd.SetBitmap(m_LEDGray); }
	}
	m_CurrentEMSStats = nValue;

	if ( FN_DISABLE == PlcParam.m_EnabledFanAlarm )
	{	this->m_CurrentFanWnd.SetBitmap(m_LEDGray);	}
	else
	{
		nValue = PlcCtrlPtr->GetCurrentFanAlarm();
		if ( nValue != m_CurrentFanStats )
		{
			if ( TurnOn == nValue )
			{	this->m_CurrentFanWnd.SetBitmap(m_LEDRed); }
			else
			{	this->m_CurrentFanWnd.SetBitmap(m_LEDGreen); }
		}
		m_CurrentFanStats = nValue;	
	}

	nValue = PlcCtrlPtr->GetCurrentKeySwitchStats();
	if ( nValue != m_CurrentKeySwitchStats )
	{
		if ( TurnOn == nValue )
		{	this->m_KeySwitchWnd.SetBitmap(m_LEDGreen); }
		else
		{	this->m_KeySwitchWnd.SetBitmap(m_LEDRed); }
	}
	m_CurrentKeySwitchStats = nValue;

	nValue = PlcCtrlPtr->GetCurrentOverHeat();
	if ( nValue != m_CurrentOverHeatStats )
	{
		if ( TurnOn == nValue )
		{	this->m_CurrentOverHeatWnd.SetBitmap(m_LEDRed); }
		else
		{	this->m_CurrentOverHeatWnd.SetBitmap(m_LEDGray); }
	}
	m_CurrentOverHeatStats = nValue;

	nValue = PlcCtrlPtr->GetHardwareBypass_LA();
	if ( nValue != m_HardBypassStats_LA )
	{
		if ( TurnOn == nValue )
		{	this->m_HardwareBypassWnd_LA.SetBitmap(m_LEDRed); }
		else
		{	this->m_HardwareBypassWnd_LA.SetBitmap(m_LEDGray); }
	}
	m_HardBypassStats_LA = nValue;

	nValue = PlcCtrlPtr->GetHardwareBypass_LB();
	if ( nValue != m_HardBypassStats_LB )
	{
		if ( TurnOn == nValue )
		{	this->m_HardwareBypassWnd_LB.SetBitmap(m_LEDRed); }
		else
		{	this->m_HardwareBypassWnd_LB.SetBitmap(m_LEDGray); }
	}
	m_HardBypassStats_LB = nValue;

	nValue = PlcCtrlPtr->GetCurrentGreenLightStats();
	if ( nValue != m_GreenLightStats )
	{
		if ( TurnOn == nValue )
		{	this->m_StartLightWnd.SetBitmap(m_LEDGreen); }
		else
		{	this->m_StartLightWnd.SetBitmap(m_LEDGray); }
	}
	m_GreenLightStats = nValue;
	
	if ( PlcCtrlPtr->GetSafetyAlarm() == true )
	{
		if ( TurnOn == m_CurrentAirStats )
		{	this->m_AlarmAirWnd.SetBitmap(m_LEDRed);	}
		else
		{	this->m_AlarmAirWnd.SetBitmap(m_LEDGray);	}

		if ( TurnOn == m_CurrentFrontCapStats )
		{	this->m_AlarmCapWnd.SetBitmap(m_LEDRed);	}
		else
		{	this->m_AlarmCapWnd.SetBitmap(m_LEDGray);	}

		if ( TurnOn == m_CurrentRearDoorStats )
		{	this->m_AlarmRearDoorWnd.SetBitmap(m_LEDRed);	}
		else
		{	this->m_AlarmRearDoorWnd.SetBitmap(m_LEDGray);	}
		
	}
	else
	{
		this->m_AlarmAirWnd.SetBitmap(m_LEDGray);
		this->m_AlarmCapWnd.SetBitmap(m_LEDGray);
		this->m_AlarmRearDoorWnd.SetBitmap(m_LEDGray);
	}
	if ( PlcCtrlPtr->GetPLC_FanAlarm() == true )	
	{	this->m_AlarmFanWnd.SetBitmap(m_LEDRed); }
	else
	{	this->m_AlarmFanWnd.SetBitmap(m_LEDGray); }

	if ( PlcCtrlPtr->GetSafetyAlarmEMS() == true )	
	{	this->m_AlarmEMSWnd.SetBitmap(m_LEDRed); }
	else
	{	this->m_AlarmEMSWnd.SetBitmap(m_LEDGray); }

	nValue = PlcCtrlPtr->GetIsPCBRightInDirection();
	if ( nValue != m_PCBInDirection )
	{
		if ( TurnOn == nValue )
		{
			this->m_PCBRightInWnd.SetBitmap(m_LEDGreen);
			this->m_PCBLeftInWnd.SetBitmap(m_LEDGray);	
		}
		else
		{
			this->m_PCBRightInWnd.SetBitmap(m_LEDGray);
			this->m_PCBLeftInWnd.SetBitmap(m_LEDGreen);		
		}
	}
	m_PCBInDirection = nValue;
	
	//Lane A	
	nValue = PlcCtrlPtr->GetConveryerSensorPCBIn_LA();
	if ( nValue != m_ConveryerSensorPCBIn_LA )
	{
		if ( TurnOn == nValue )
		{	this->m_PCBInSensorWnd_LA.SetBitmap(m_LEDGreen); }
		else
		{	this->m_PCBInSensorWnd_LA.SetBitmap(m_LEDGray); }
	}
	m_ConveryerSensorPCBIn_LA = nValue;	

	nValue = PlcCtrlPtr->GetConveryerSensorPCBSlow_LA();
	if ( nValue != m_ConveryerSensorPCBSlow_LA )
	{
		if ( TurnOn == nValue )
		{	this->m_SlowDownSensorWnd_LA.SetBitmap(m_LEDGreen); }
		else
		{	this->m_SlowDownSensorWnd_LA.SetBitmap(m_LEDGray); }
	}
	m_ConveryerSensorPCBSlow_LA = nValue;			

	nValue = PlcCtrlPtr->GetConveryerSensorPCBStop_LA();
	if ( nValue != m_ConveryerSensorPCBStop_LA )
	{
		if ( TurnOn == nValue )
		{	this->m_InPositionSensorWnd_LA.SetBitmap(m_LEDGreen); }
		else
		{	this->m_InPositionSensorWnd_LA.SetBitmap(m_LEDGray); }
	}
	m_ConveryerSensorPCBStop_LA = nValue;	

	nValue = PlcCtrlPtr->GetConveryerSensorPCBOut_LA();
	if ( nValue != m_ConveryerSensorPCBOut_LA )
	{
		if ( TurnOn == nValue )
		{	this->m_PCBOutSensorWnd_LA.SetBitmap(m_LEDGreen); }
		else
		{	this->m_PCBOutSensorWnd_LA.SetBitmap(m_LEDGray); }
	}
	m_ConveryerSensorPCBOut_LA = nValue;
	
	nValue = PlcCtrlPtr->GetConveryerSensorPCBStop2_LA();
	if ( nValue != m_ConveryerSensorPCBStop_LA2 )
	{
		if ( TurnOn == nValue )
		{	this->m_InPositionSensorWnd_LA2.SetBitmap(m_LEDGreen); }
		else
		{	this->m_InPositionSensorWnd_LA2.SetBitmap(m_LEDGray); }
	}
	m_ConveryerSensorPCBStop_LA2 = nValue;	

	nValue = PlcCtrlPtr->GetLastStationSignal_LA();
	if ( nValue != m_SignalFromLast_LA )
	{
		if ( TurnOn == nValue )
		{	this->m_FromLastSignalWnd_LA.SetBitmap(m_LEDGreen); }
		else
		{	this->m_FromLastSignalWnd_LA.SetBitmap(m_LEDGray); }
	}
	m_SignalFromLast_LA = nValue;

	nValue = PlcCtrlPtr->GetNextStationSignal_LA();
	if ( nValue != m_SignalFromNext_LA )
	{
		if ( TurnOn == nValue )
		{	this->m_FromNextSignalWnd_LA.SetBitmap(m_LEDGreen); }
		else
		{	this->m_FromNextSignalWnd_LA.SetBitmap(m_LEDGray); }
	}
	m_SignalFromNext_LA = nValue;	

	//Lane B	
	nValue = PlcCtrlPtr->GetConveryerSensorPCBIn_LB();
	if ( nValue != m_ConveryerSensorPCBIn_LB )
	{
		if ( TurnOn == nValue )
		{	this->m_PCBInSensorWnd_LB.SetBitmap(m_LEDGreen); }
		else
		{	this->m_PCBInSensorWnd_LB.SetBitmap(m_LEDGray); }
	}
	m_ConveryerSensorPCBIn_LB = nValue;	

	nValue = PlcCtrlPtr->GetConveryerSensorPCBSlow_LB();
	if ( nValue != m_ConveryerSensorPCBSlow_LB )
	{
		if ( TurnOn == nValue )
		{	this->m_SlowDownSensorWnd_LB.SetBitmap(m_LEDGreen); }
		else
		{	this->m_SlowDownSensorWnd_LB.SetBitmap(m_LEDGray); }
	}
	m_ConveryerSensorPCBSlow_LB = nValue;	
	
	nValue = PlcCtrlPtr->GetConveryerSensorPCBStop_LB();
	if ( nValue != m_ConveryerSensorPCBStop_LB )
	{
		if ( TurnOn == nValue )
		{	this->m_InPositionSensorWnd_LB.SetBitmap(m_LEDGreen); }
		else
		{	this->m_InPositionSensorWnd_LB.SetBitmap(m_LEDGray); }
	}
	m_ConveryerSensorPCBStop_LB = nValue;
	
	nValue = PlcCtrlPtr->GetConveryerSensorPCBOut_LB();
	if ( nValue != m_ConveryerSensorPCBOut_LB )
	{
		if ( TurnOn == nValue )
		{	this->m_PCBOutSensorWnd_LB.SetBitmap(m_LEDGreen); }
		else
		{	this->m_PCBOutSensorWnd_LB.SetBitmap(m_LEDGray); }
	}
	m_ConveryerSensorPCBOut_LB = nValue;	
	
	nValue = PlcCtrlPtr->GetConveryerSensorPCBStop2_LB();
	if ( nValue != m_ConveryerSensorPCBStop_LB2 )
	{
		if ( TurnOn == nValue )
		{	this->m_InPositionSensorWnd_LB2.SetBitmap(m_LEDGreen); }
		else
		{	this->m_InPositionSensorWnd_LB2.SetBitmap(m_LEDGray); }
	}
	m_ConveryerSensorPCBStop_LB2 = nValue;	
	
	nValue = PlcCtrlPtr->GetLastStationSignal_LB();
	if ( nValue != m_SignalFromLast_LB )
	{
		if ( TurnOn == nValue )
		{	this->m_FromLastSignalWnd_LB.SetBitmap(m_LEDGreen); }
		else
		{	this->m_FromLastSignalWnd_LB.SetBitmap(m_LEDGray); }
	}
	m_SignalFromLast_LB = nValue;

	nValue = PlcCtrlPtr->GetNextStationSignal_LB();
	if ( nValue != m_SignalFromNext_LB )
	{
		if ( TurnOn == nValue )
		{	this->m_FromNextSignalWnd_LB.SetBitmap(m_LEDGreen); }
		else
		{	this->m_FromNextSignalWnd_LB.SetBitmap(m_LEDGray); }
	}
	m_SignalFromNext_LB = nValue;
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneStatus::EnableLaneStatus()
{
	UINT     CtrlID=0;
	BOOL     Enable=TRUE;
	LANE_WORK_MODE LaneWorkMode_LA = AOIDataCollect.GetLaneWorkMode_LA();
	LANE_WORK_MODE LaneWorkMode_LB = AOIDataCollect.GetLaneWorkMode_LB();
	//Lane A
	if ( LANE_WORK_DISABLE == LaneWorkMode_LA )
	{	Enable=FALSE;	}
	else
	{	Enable=TRUE;	}

	CtrlID = PLCLANE_PCB_IN_BTN_LA;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);
	CtrlID = PLCLANE_PCB_BACK_BTN_LA;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);
	CtrlID = PLCLANE_PCB_BACK_OUT_BTN_LA;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);	
	CtrlID = PLCLANE_PCB_CLAMP_CHK_LA;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);
	CtrlID = PLCLANE_PCB_STOPBAR_CHK_LA;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);	
	CtrlID = PLCLANE_PCB_IN_BACK_REPEAT_CHK_LA;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);
	CtrlID = PLCLANE_SEND_TO_LAST_SIGNAL_CHK_LA;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);
	CtrlID = PLCLANE_SEND_TO_NEXT_SIGNAL_CHK_LA;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);
	CtrlID = PLCLANE_SEND_STAGE_ALARM_CHK_LA;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);
	CtrlID = PLCLANE_PCB_OUT_BTN_LA;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);
	CtrlID = PLCLANE_PCB_REIN_BTN_LA;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);
	CtrlID = PLCLANE_PCB_CLEAR_BTN_LA;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);
	CtrlID = PLCLANE_SEND_OK_SIGNAL_CHK_LA;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);
	CtrlID = PLCLANE_SEND_NG_SIGNAL_CHK_LA;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);
	CtrlID = PLCLANE_SEND_INSPECTION_ALARM_CHK_LA;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);
	CtrlID = PLCLANE_PCB_IN_2ND_BTN_LA;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);	
	CtrlID = PLCLANE_PCB_IN_3RD_BTN_LA;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);	
	CtrlID = PLCLANE_PCB_OUT_WITH_IN_BTN_LA;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);	
	CtrlID = PLCLANE_PCB_AUTO_OUT_IN_BTN_LA;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);	
	CtrlID = PLCLANE_PCB_AUTO_BACK_IN_BTN_LA;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);

	//Lane B
	if ( LANE_WORK_DISABLE == LaneWorkMode_LB )
	{	Enable=FALSE;	}
	else
	{	Enable=TRUE;	}

	CtrlID = PLCLANE_PCB_IN_BTN_LB;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);
	CtrlID = PLCLANE_PCB_BACK_BTN_LB;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);
	CtrlID = PLCLANE_PCB_BACK_OUT_BTN_LB;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);
	CtrlID = PLCLANE_PCB_CLAMP_CHK_LB;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);
	CtrlID = PLCLANE_PCB_STOPBAR_CHK_LB;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);	
	CtrlID = PLCLANE_PCB_IN_BACK_REPEAT_CHK_LB;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);
	CtrlID = PLCLANE_SEND_TO_LAST_SIGNAL_CHK_LB;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);
	CtrlID = PLCLANE_SEND_TO_NEXT_SIGNAL_CHK_LB;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);
	CtrlID = PLCLANE_SEND_STAGE_ALARM_CHK_LB;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);
	CtrlID = PLCLANE_PCB_OUT_BTN_LB;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);
	CtrlID = PLCLANE_PCB_REIN_BTN_LB;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);
	CtrlID = PLCLANE_PCB_CLEAR_BTN_LB;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);
	CtrlID = PLCLANE_SEND_OK_SIGNAL_CHK_LB;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);
	CtrlID = PLCLANE_SEND_NG_SIGNAL_CHK_LB;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);
	CtrlID = PLCLANE_SEND_INSPECTION_ALARM_CHK_LB;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);
	CtrlID = PLCLANE_PCB_IN_2ND_BTN_LB;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);	
	CtrlID = PLCLANE_PCB_IN_3RD_BTN_LB;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);	
	CtrlID = PLCLANE_PCB_OUT_WITH_IN_BTN_LB;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);	
	CtrlID = PLCLANE_PCB_AUTO_OUT_IN_BTN_LB;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);	
	CtrlID = PLCLANE_PCB_AUTO_BACK_IN_BTN_LB;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);	
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneStatus::OnSendToLastSignalChkLA() 
{
	// TODO: Add your control notification handler code here
	bool On = false;
	if ( this->IsDlgButtonChecked(PLCLANE_SEND_TO_LAST_SIGNAL_CHK_LA) == TRUE )	
	{	On = true;	}
	else
	{	On = false;	}
	if ( PlcCtrlPtr->WriteSignalToLast_LA(On) == false )
	{	JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString()); }
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneStatus::OnSendToNextSignalChkLA() 
{
	// TODO: Add your control notification handler code here
	bool On = false;
	if ( this->IsDlgButtonChecked(PLCLANE_SEND_TO_NEXT_SIGNAL_CHK_LA) == TRUE )	
	{	On = true;	}
	else
	{	On = false;	}
	if ( PlcCtrlPtr->WriteSignalToNext_LA(On) == false )
	{	JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString()); }
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneStatus::OnSendOkSignalChkLA() 
{
	// TODO: Add your control notification handler code here
	bool On = false;
	if ( this->IsDlgButtonChecked(PLCLANE_SEND_OK_SIGNAL_CHK_LA) == TRUE )	
	{	On = true;	}
	else
	{	On = false;	}
	if ( PlcCtrlPtr->WriteOKSignalToNext_LA(On) == false )
	{	JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString()); }	
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneStatus::OnSendNgSignalChkLA() 
{
	// TODO: Add your control notification handler code here
	bool On = false;
	if ( this->IsDlgButtonChecked(PLCLANE_SEND_NG_SIGNAL_CHK_LA) == TRUE )	
	{	On = true;	}
	else
	{	On = false;	}
	if ( PlcCtrlPtr->WriteNGSignalToNext_LA(On) == false )
	{	JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString()); }
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneStatus::OnPCBInBtnLA() 
{
	// TODO: Add your control notification handler code here
	const bool bStep = true;
	LANE_ID LaneID = LANE_ID_A;
	if ( AOIDataCollect.ExecPCBInProc(LaneID, true, bStep) == false )
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	}
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneStatus::OnPCBBackBtnLA() 
{
	// TODO: Add your control notification handler code here
	const bool bStep = true;
	if ( PlcCtrlPtr->ExecPCBBack_LA(true) == false )
	{	JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString()); }
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneStatus::OnPCBBackOutBtnLA() 
{
	// TODO: Add your control notification handler code here
	const bool bStep = true;
	if ( PlcCtrlPtr->ExecPCBBackOut_LA(true) == false )
	{	JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString()); }
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneStatus::OnPCBOutBtnLA() 
{
	// TODO: Add your control notification handler code here
	const bool bStep = true;
	if ( PlcCtrlPtr->GetNextStationSignal_LA() == false )
	{
		if ( PlcCtrlPtr->ExecPCBOutInside_LA(true) == false )
		{	JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString()); }
	}
	else
	{
		if ( false == AOIDataCollect.GetUIEnablePCBOutButton() )
		{	return;	}
		if ( PlcCtrlPtr->ExecPCBOut_LA(true) == false )
		{	JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString()); }
	}
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneStatus::OnPCBReinBtnLA() 
{
	// TODO: Add your control notification handler code here
	const bool bStep = true;
	LANE_ID LaneID = LANE_ID_A;
	if ( AOIDataCollect.ExecPCBReInProc(LaneID, true, bStep) == false )
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	}
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneStatus::OnPCBInBackRepeatChkLA() 
{
	// TODO: Add your control notification handler code here
	BOOL bCheck = CWnd::IsDlgButtonChecked(PLCLANE_PCB_IN_BACK_REPEAT_CHK_LA);
	if ( FALSE == bCheck ) 
	{	return;		}

	if ( AOIDataCollect.MoveCameraToBeforePCBInPosition(true) == false )
	{	
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	
		return;
	}
	
	m_LaneLoopCount_LA = 0;
	m_LaneLoopMode_LA = LANE_LOOP_MODE_STOP;
	if ( FN_ENABLE == m_ConveryerSensorPCBIn_LA )
	{
		if ( PlcCtrlPtr->ExecPCBIn_LA(false) == false )
		{	
			JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString()); 
			CWnd::CheckDlgButton(PLCLANE_PCB_IN_BACK_REPEAT_CHK_LA, FALSE);
			return;
		}
		m_LaneLoopMode_LA = LANE_LOOP_MODE_CHECK_PCB_IN;		
		CWnd::SetTimer(PLC_LANE_REPEAT_LOOP_LA, 100, NULL);	
	}
	else
	{
		if ( PlcCtrlPtr->ExecPCBBack_LA(false) == false )
		{	
			JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString()); 
			CWnd::CheckDlgButton(PLCLANE_PCB_IN_BACK_REPEAT_CHK_LA, FALSE);
			return;
		}
		m_LaneLoopMode_LA = LANE_LOOP_MODE_CHECK_PCB_BACK;		
		CWnd::SetTimer(PLC_LANE_REPEAT_LOOP_LA, 100, NULL);
	}
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneStatus::OnPCBClampChkLA() 
{
	// TODO: Add your control notification handler code here
	bool On = false;
	if ( this->IsDlgButtonChecked(PLCLANE_PCB_CLAMP_CHK_LA) == TRUE )
	{	On = true; }
	else
	{	On = false; }
	if ( PlcCtrlPtr->WriteConveryerClamp_LA(On) == false )
	{	JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString()); }
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneStatus::OnPCBStopBarChkLA()
{
	bool On = false;
	if ( this->IsDlgButtonChecked(PLCLANE_PCB_STOPBAR_CHK_LA) == TRUE )
	{	On = true; }
	else
	{	On = false; }
	if ( PlcCtrlPtr->WriteConveryerStopBar_LA(On) == false )
	{	JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString()); }
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneStatus::OnPCBInBtnLB() 
{
	// TODO: Add your control notification handler code here
	const bool bStep = true;
	LANE_ID LaneID = LANE_ID_B;
	if ( AOIDataCollect.ExecPCBInProc(LaneID, true, bStep) == false )
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	}
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneStatus::OnPCBBackBtnLB() 
{
	// TODO: Add your control notification handler code here	
	if ( PlcCtrlPtr->ExecPCBBack_LB(true) == false )
	{	JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString()); }
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneStatus::OnPCBBackOutBtnLB() 
{
	// TODO: Add your control notification handler code here	
	if ( PlcCtrlPtr->ExecPCBBackOut_LB(true) == false )
	{	JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString()); }
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneStatus::OnPCBOutBtnLB() 
{
	// TODO: Add your control notification handler code here
	if ( PlcCtrlPtr->GetNextStationSignal_LB() == false )
	{
		if ( PlcCtrlPtr->ExecPCBOutInside_LB(true) == false )
		{	JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString()); }
	}
	else
	{
		if ( false == AOIDataCollect.GetUIEnablePCBOutButton() )
		{	return;	}
		if ( PlcCtrlPtr->ExecPCBOut_LB(true) == false )
		{	JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString()); }
	}
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneStatus::OnPCBReInBtnLB() 
{
	// TODO: Add your control notification handler code here
	const bool bStep = true;
	LANE_ID LaneID = LANE_ID_B;
	if ( AOIDataCollect.ExecPCBReInProc(LaneID, true, bStep) == false )
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	}
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneStatus::OnPCBClampChkLB() 
{
	// TODO: Add your control notification handler code here
	bool On = false;
	if ( this->IsDlgButtonChecked(PLCLANE_PCB_CLAMP_CHK_LB) == TRUE )
	{	On = true; }
	else
	{	On = false; }
	if ( PlcCtrlPtr->WriteConveryerClamp_LB(On) == false )
	{	JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString()); }
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneStatus::OnPCBStopBarChkLB()
{
	bool On = false;
	if ( this->IsDlgButtonChecked(PLCLANE_PCB_STOPBAR_CHK_LB) == TRUE )
	{	On = true; }
	else
	{	On = false; }
	if ( PlcCtrlPtr->WriteConveryerStopBar_LB(On) == false )
	{	JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString()); }
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneStatus::OnSendToLastSignalChkLB() 
{
	// TODO: Add your control notification handler code here
	bool On = false;
	if ( this->IsDlgButtonChecked(PLCLANE_SEND_TO_LAST_SIGNAL_CHK_LB) == TRUE )	
	{	On = true;	}
	else
	{	On = false;	}
	if ( PlcCtrlPtr->WriteSignalToLast_LB(On) == false )
	{	JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString()); }
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneStatus::OnSendToNextSignalChkLB() 
{
	// TODO: Add your control notification handler code here
	bool On = false;
	if ( this->IsDlgButtonChecked(PLCLANE_SEND_TO_NEXT_SIGNAL_CHK_LB) == TRUE )	
	{	On = true;	}
	else
	{	On = false;	}
	if ( PlcCtrlPtr->WriteSignalToNext_LB(On) == false )
	{	JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString()); }
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneStatus::OnSendOkSignalChkLB() 
{
	// TODO: Add your control notification handler code here
	bool On = false;
	if ( this->IsDlgButtonChecked(PLCLANE_SEND_OK_SIGNAL_CHK_LB) == TRUE )	
	{	On = true;	}
	else
	{	On = false;	}
	if ( PlcCtrlPtr->WriteOKSignalToNext_LB(On) == false )
	{	JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString()); }	
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneStatus::OnSendNgSignalChkLB() 
{
	// TODO: Add your control notification handler code here
	bool On = false;
	if ( this->IsDlgButtonChecked(PLCLANE_SEND_NG_SIGNAL_CHK_LB) == TRUE )	
	{	On = true;	}
	else
	{	On = false;	}
	if ( PlcCtrlPtr->WriteNGSignalToNext_LB(On) == false )
	{	JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString()); }
}
//-------------------------------------------------------------------------------------//
BOOL CPLCCtrlPaneLaneStatus::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class	
	switch ( pMsg->message )
	{
	case WM_KEYDOWN:
		switch ( pMsg->wParam )
		{
		case VK_ESCAPE:			
			return TRUE;
			break;
		}
		break;
	}
	return CDialog::PreTranslateMessage(pMsg);
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneStatus::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_PLC_LANE_STATUS_PANE");
	//---------------------------------------------------------------------------------//
	WndID = IDD_PLC_LANE_STATUS_PANE;
	WndKey = _T("IDD_PLC_LANE_STATUS_PANE");
	this->GetWindowText(LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetWindowText(NewLabelText);
	//---------------------------------------------------------------------------------//
	WndID = IDOK;
	NewLabelText = AOIDataDefine.GetWndOKText();
	this->SetDlgItemText(WndID, NewLabelText);	
	
	WndID = IDCANCEL;
	NewLabelText = AOIDataDefine.GetWndCancelText();
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//
	WndID = PLCLANE_STATUS_GROUP;
	WndKey = _T("PLCLANE_STATUS_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCLANE_CURRENT_CAP_LABEL;
	WndKey = _T("PLCLANE_CURRENT_CAP_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCLANE_CURRENT_REAR_DOOR_LABEL;
	WndKey = _T("PLCLANE_CURRENT_REAR_DOOR_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCLANE_CURRENT_AIR_LABEL;
	WndKey = _T("PLCLANE_CURRENT_AIR_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCLANE_CURRENT_EMS_LABEL;
	WndKey = _T("PLCLANE_CURRENT_EMS_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCLANE_CURRENT_FAN_LABEL;
	WndKey = _T("PLCLANE_CURRENT_FAN_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCLANE_KEY_SWITCH_LABEL;
	WndKey = _T("PLCLANE_KEY_SWITCH_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCLANE_CURRENT_OVER_HEAT_LABEL;
	WndKey = _T("PLCLANE_CURRENT_OVER_HEAT_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCLANE_CURRENT_START_LIGHT_LABEL;
	WndKey = _T("PLCLANE_CURRENT_START_LIGHT_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCLANE_HARDWARE_BYPASS_LABEL_LA;
	WndKey = _T("PLCLANE_HARDWARE_BYPASS_LABEL_LA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCLANE_HARDWARE_BYPASS_LABEL_LB;
	WndKey = _T("PLCLANE_HARDWARE_BYPASS_LABEL_LB");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = PLCLANE_ALARM_GROUP;
	WndKey = _T("PLCLANE_ALARM_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCLANE_ALARM_CAP_LABEL;
	WndKey = _T("PLCLANE_ALARM_CAP_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCLANE_ALARM_REAR_DOOR_LABEL;
	WndKey = _T("PLCLANE_ALARM_REAR_DOOR_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PLCLANE_ALARM_AIR_LABEL;
	WndKey = _T("PLCLANE_ALARM_AIR_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = PLCLANE_ALARM_EMS_LABEL;
	WndKey = _T("PLCLANE_ALARM_EMS_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	
	WndID = PLCLANE_ALARM_FAN_LABEL;
	WndKey = _T("PLCLANE_ALARM_FAN_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = PLCLANE_OTHERS_GROUP;
	WndKey = _T("PLCLANE_OTHERS_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PLCLANE_PCB_LEFT_IN_LABEL;
	WndKey = _T("PLCLANE_PCB_LEFT_IN_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PLCLANE_PCB_RIGHT_IN_LABEL;
	WndKey = _T("PLCLANE_PCB_RIGHT_IN_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = PLCLANE_MACHINE_GROUP;
	WndKey = _T("PLCLANE_MACHINE_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PLCLANE_MACHINE_START_BTN;
	WndKey = _T("PLCLANE_MACHINE_START_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PLCLANE_MACHINE_RESET_BTN;
	WndKey = _T("PLCLANE_MACHINE_RESET_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PLCLANE_MACHINE_STOP_BTN;
	WndKey = _T("PLCLANE_MACHINE_STOP_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//
	WndID = PLCLANE_LANE_GROUP_A;
	WndKey = _T("PLCLANE_LANE_GROUP_A");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PLCLANE_PCB_IN_SENSOR_LABEL_LA;
	WndKey = _T("PLCLANE_PCB_IN_SENSOR_LABEL_LA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PLCLANE_SLOW_DOWN_SENSOR_LABEL_LA;
	WndKey = _T("PLCLANE_SLOW_DOWN_SENSOR_LABEL_LA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCLANE_IN_POSITION_SENSOR_LABEL_LA;
	WndKey = _T("PLCLANE_IN_POSITION_SENSOR_LABEL_LA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PLCLANE_IN_POSITION_SENSOR_LABEL_LA2;
	WndKey = _T("PLCLANE_IN_POSITION_SENSOR_LABEL_LA2");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PLCLANE_PCB_OUT_SENSOR_LABEL_LA;
	WndKey = _T("PLCLANE_PCB_OUT_SENSOR_LABEL_LA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PLCLANE_FROM_LAST_SIGNAL_LABEL_LA;
	WndKey = _T("PLCLANE_FROM_LAST_SIGNAL_LABEL_LA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCLANE_FROM_NEXT_SIGNAL_LABEL_LA;
	WndKey = _T("PLCLANE_FROM_NEXT_SIGNAL_LABEL_LA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PLCLANE_PCB_IN_BTN_LA;
	WndKey = _T("PLCLANE_PCB_IN_BTN_LA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PLCLANE_PCB_IN_BTN_LA;
	WndKey = _T("PLCLANE_PCB_IN_BTN_LA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PLCLANE_PCB_IN_2ND_BTN_LA;
	WndKey = _T("PLCLANE_PCB_IN_2ND_BTN_LA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	
	WndID = PLCLANE_PCB_IN_3RD_BTN_LA;
	WndKey = _T("PLCLANE_PCB_IN_3RD_BTN_LA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PLCLANE_PCB_BACK_BTN_LA;
	WndKey = _T("PLCLANE_PCB_BACK_BTN_LA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCLANE_PCB_BACK_OUT_BTN_LA;
	WndKey = _T("PLCLANE_PCB_BACK_OUT_BTN_LA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCLANE_PCB_REIN_BTN_LA;
	WndKey = _T("PLCLANE_PCB_REIN_BTN_LA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PLCLANE_PCB_CLEAR_BTN_LA;
	WndKey = _T("PLCLANE_PCB_CLEAR_BTN_LA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCLANE_PCB_CLAMP_CHK_LA;
	WndKey = _T("PLCLANE_PCB_CLAMP_CHK_LA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PLCLANE_PCB_STOPBAR_CHK_LA;
	WndKey = _T("PLCLANE_PCB_STOPBAR_CHK_LA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PLCLANE_PCB_IN_BACK_REPEAT_CHK_LA;
	WndKey = _T("PLCLANE_PCB_IN_BACK_REPEAT_CHK_LA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCLANE_PCB_OUT_BTN_LA;
	WndKey = _T("PLCLANE_PCB_OUT_BTN_LA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCLANE_SEND_TO_LAST_SIGNAL_CHK_LA;
	WndKey = _T("PLCLANE_SEND_TO_LAST_SIGNAL_CHK_LA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCLANE_SEND_TO_NEXT_SIGNAL_CHK_LA;
	WndKey = _T("PLCLANE_SEND_TO_NEXT_SIGNAL_CHK_LA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PLCLANE_SEND_OK_SIGNAL_CHK_LA;
	WndKey = _T("PLCLANE_SEND_OK_SIGNAL_CHK_LA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PLCLANE_SEND_NG_SIGNAL_CHK_LA;
	WndKey = _T("PLCLANE_SEND_NG_SIGNAL_CHK_LA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	
	WndID = PLCLANE_SEND_STAGE_ALARM_CHK_LA;
	WndKey = _T("PLCLANE_SEND_STAGE_ALARM_CHK_LA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PLCLANE_SEND_INSPECTION_ALARM_CHK_LA;
	WndKey = _T("PLCLANE_SEND_INSPECTION_ALARM_CHK_LA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PLCLANE_PCB_OUT_WITH_IN_BTN_LA;
	WndKey = _T("PLCLANE_PCB_OUT_WITH_IN_BTN_LA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PLCLANE_PCB_AUTO_OUT_IN_BTN_LA;
	WndKey = _T("PLCLANE_PCB_AUTO_OUT_IN_BTN_LA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PLCLANE_PCB_AUTO_BACK_IN_BTN_LA;
	WndKey = _T("PLCLANE_PCB_AUTO_BACK_IN_BTN_LA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//
	WndID = PLCLANE_LANE_GROUP_B;
	WndKey = _T("PLCLANE_LANE_GROUP_B");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PLCLANE_PCB_IN_SENSOR_LABEL_LB;
	WndKey = _T("PLCLANE_PCB_IN_SENSOR_LABEL_LB");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PLCLANE_SLOW_DOWN_SENSOR_LABEL_LB;
	WndKey = _T("PLCLANE_SLOW_DOWN_SENSOR_LABEL_LB");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCLANE_IN_POSITION_SENSOR_LABEL_LB;
	WndKey = _T("PLCLANE_IN_POSITION_SENSOR_LABEL_LB");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PLCLANE_IN_POSITION_SENSOR_LABEL_LB2;
	WndKey = _T("PLCLANE_IN_POSITION_SENSOR_LABEL_LB2");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PLCLANE_PCB_OUT_SENSOR_LABEL_LB;
	WndKey = _T("PLCLANE_PCB_OUT_SENSOR_LABEL_LB");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PLCLANE_FROM_LAST_SIGNAL_LABEL_LB;
	WndKey = _T("PLCLANE_FROM_LAST_SIGNAL_LABEL_LB");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCLANE_FROM_NEXT_SIGNAL_LABEL_LB;
	WndKey = _T("PLCLANE_FROM_NEXT_SIGNAL_LABEL_LB");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PLCLANE_PCB_IN_BTN_LB;
	WndKey = _T("PLCLANE_PCB_IN_BTN_LB");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PLCLANE_PCB_IN_2ND_BTN_LB;
	WndKey = _T("PLCLANE_PCB_IN_2ND_BTN_LB");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	
	WndID = PLCLANE_PCB_IN_3RD_BTN_LB;
	WndKey = _T("PLCLANE_PCB_IN_3RD_BTN_LB");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PLCLANE_PCB_OUT_BTN_LB;
	WndKey = _T("PLCLANE_PCB_OUT_BTN_LB");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PLCLANE_PCB_BACK_BTN_LB;
	WndKey = _T("PLCLANE_PCB_BACK_BTN_LB");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCLANE_PCB_BACK_OUT_BTN_LB;
	WndKey = _T("PLCLANE_PCB_BACK_OUT_BTN_LB");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = PLCLANE_PCB_REIN_BTN_LB;
	WndKey = _T("PLCLANE_PCB_REIN_BTN_LB");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PLCLANE_PCB_CLEAR_BTN_LB;
	WndKey = _T("PLCLANE_PCB_CLEAR_BTN_LB");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCLANE_PCB_CLAMP_CHK_LB;
	WndKey = _T("PLCLANE_PCB_CLAMP_CHK_LB");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PLCLANE_PCB_STOPBAR_CHK_LB;
	WndKey = _T("PLCLANE_PCB_STOPBAR_CHK_LB");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PLCLANE_PCB_IN_BACK_REPEAT_CHK_LB;
	WndKey = _T("PLCLANE_PCB_IN_BACK_REPEAT_CHK_LB");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PLCLANE_SEND_TO_LAST_SIGNAL_CHK_LB;
	WndKey = _T("PLCLANE_SEND_TO_LAST_SIGNAL_CHK_LB");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCLANE_SEND_TO_NEXT_SIGNAL_CHK_LB;
	WndKey = _T("PLCLANE_SEND_TO_NEXT_SIGNAL_CHK_LB");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PLCLANE_SEND_OK_SIGNAL_CHK_LB;
	WndKey = _T("PLCLANE_SEND_OK_SIGNAL_CHK_LB");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PLCLANE_SEND_NG_SIGNAL_CHK_LB;
	WndKey = _T("PLCLANE_SEND_NG_SIGNAL_CHK_LB");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PLCLANE_SEND_STAGE_ALARM_CHK_LB;
	WndKey = _T("PLCLANE_SEND_STAGE_ALARM_CHK_LB");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PLCLANE_SEND_INSPECTION_ALARM_CHK_LB;
	WndKey = _T("PLCLANE_SEND_INSPECTION_ALARM_CHK_LB");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	
	WndID = PLCLANE_PCB_OUT_WITH_IN_BTN_LB;
	WndKey = _T("PLCLANE_PCB_OUT_WITH_IN_BTN_LB");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PLCLANE_PCB_AUTO_OUT_IN_BTN_LB;
	WndKey = _T("PLCLANE_PCB_AUTO_OUT_IN_BTN_LB");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PLCLANE_PCB_AUTO_BACK_IN_BTN_LB;
	WndKey = _T("PLCLANE_PCB_AUTO_BACK_IN_BTN_LB");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//
	/*
	WndID = AAAAAAAAAAAAAAAAAA;
	WndKey = _T("AAAAAAAAAAAAAAAAAA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	*/
}
//-------------------------------------------------------------------------------------//
bool CPLCCtrlPaneLaneStatus::ExecLaneLoop_LA()
{	
	CString str;
	BOOL    bCheck = FALSE;
	DWORD   DelayTime=250;	
	if ( LANE_LOOP_MODE_CHECK_PCB_IN == m_LaneLoopMode_LA )
	{		
		if ( PlcCtrlPtr->CheckPCBInFinish_LA() == true )
		{
			bCheck = CWnd::IsDlgButtonChecked(PLCLANE_PCB_IN_BACK_REPEAT_CHK_LA);	
			if ( FALSE == bCheck ) { return false; }
			::Sleep(DelayTime);
			if ( PlcCtrlPtr->ExecPCBBack_LA(false) == false )
			{				
				JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString());				
				return false;
			}
			m_LaneLoopMode_LA = LANE_LOOP_MODE_CHECK_PCB_BACK;
			return true;
		}
		if ( PlcCtrlPtr->CheckPCBInFault_LA() == true )
		{
			JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString());			
			return false;
		}	
	}

	if ( LANE_LOOP_MODE_CHECK_PCB_BACK == m_LaneLoopMode_LA )
	{
		if ( PlcCtrlPtr->CheckPCBBackFinish_LA() == true )
		{
			m_LaneLoopCount_LA ++;
			str.Format(_T("Cnt:%d"), m_LaneLoopCount_LA);
			CWnd::SetDlgItemText(PLCLANE_PCB_IN_BACK_REPEAT_EDIT_LA, str);

			bCheck = CWnd::IsDlgButtonChecked(PLCLANE_PCB_IN_BACK_REPEAT_CHK_LA);	
			if ( FALSE == bCheck ) { return false; }
			::Sleep(DelayTime);
			if ( PlcCtrlPtr->ExecPCBIn_LA(false) == false )
			{				
				JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString());				
				return false;
			}
			m_LaneLoopMode_LA = LANE_LOOP_MODE_CHECK_PCB_IN;
			return true;
		}
		if ( PlcCtrlPtr->CheckPCBBackFault_LA() == true )
		{
			JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString());			
			return false;
		}	
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CPLCCtrlPaneLaneStatus::ExecLaneLoop_LB()
{
	CString str;
	BOOL    bCheck = FALSE;
	DWORD   DelayTime=250;	
	if ( LANE_LOOP_MODE_CHECK_PCB_IN == m_LaneLoopMode_LB )
	{		
		if ( PlcCtrlPtr->CheckPCBInFinish_LB() == true )
		{
			bCheck = CWnd::IsDlgButtonChecked(PLCLANE_PCB_IN_BACK_REPEAT_CHK_LB);	
			if ( FALSE == bCheck ) { return false; }
			::Sleep(DelayTime);
			if ( PlcCtrlPtr->ExecPCBBack_LB(false) == false )
			{				
				JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString());				
				return false;
			}
			m_LaneLoopMode_LB = LANE_LOOP_MODE_CHECK_PCB_BACK;
			return true;
		}
		if ( PlcCtrlPtr->CheckPCBInFault_LB() == true )
		{
			JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString());			
			return false;
		}	
	}

	if ( LANE_LOOP_MODE_CHECK_PCB_BACK == m_LaneLoopMode_LB )
	{
		if ( PlcCtrlPtr->CheckPCBBackFinish_LB() == true )
		{
			m_LaneLoopCount_LB ++;
			str.Format(_T("Cnt:%d"), m_LaneLoopCount_LB);
			CWnd::SetDlgItemText(PLCLANE_PCB_IN_BACK_REPEAT_EDIT_LB, str);

			bCheck = CWnd::IsDlgButtonChecked(PLCLANE_PCB_IN_BACK_REPEAT_CHK_LB);	
			if ( FALSE == bCheck ) { return false; }
			::Sleep(DelayTime);
			if ( PlcCtrlPtr->ExecPCBIn_LB(false) == false )
			{				
				JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString());				
				return false;
			}
			m_LaneLoopMode_LB = LANE_LOOP_MODE_CHECK_PCB_IN;
			return true;
		}
		if ( PlcCtrlPtr->CheckPCBBackFault_LB() == true )
		{
			JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString());			
			return false;
		}	
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneStatus::OnPCBInBackRepeatChkLB() 
{
	// TODO: Add your control notification handler code here
	BOOL bCheck = CWnd::IsDlgButtonChecked(PLCLANE_PCB_IN_BACK_REPEAT_CHK_LB);
	if ( FALSE == bCheck ) 
	{	return;		}

	if ( AOIDataCollect.MoveCameraToBeforePCBInPosition(true) == false )
	{	
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	
		return;
	}	
	m_LaneLoopCount_LB = 0;
	m_LaneLoopMode_LB = LANE_LOOP_MODE_STOP;
	if ( FN_ENABLE == m_ConveryerSensorPCBIn_LB )
	{
		if ( PlcCtrlPtr->ExecPCBIn_LB(false) == false )
		{	
			JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString()); 
			CWnd::CheckDlgButton(PLCLANE_PCB_IN_BACK_REPEAT_CHK_LB, FALSE);
			return;
		}
		m_LaneLoopMode_LB = LANE_LOOP_MODE_CHECK_PCB_IN;		
		CWnd::SetTimer(PLC_LANE_REPEAT_LOOP_LB, 100, NULL);	
	}
	else
	{
		if ( PlcCtrlPtr->ExecPCBBack_LB(false) == false )
		{	
			JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString()); 
			CWnd::CheckDlgButton(PLCLANE_PCB_IN_BACK_REPEAT_CHK_LB, FALSE);
			return;
		}
		m_LaneLoopMode_LB = LANE_LOOP_MODE_CHECK_PCB_BACK;		
		CWnd::SetTimer(PLC_LANE_REPEAT_LOOP_LB, 100, NULL);
	}	
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneStatus::OnSendStageAlarmChkLA() 
{
	// TODO: Add your control notification handler code here
	bool IsOK = true;
	if ( this->IsDlgButtonChecked(PLCLANE_SEND_STAGE_ALARM_CHK_LA) == TRUE )	
	{	IsOK = PlcCtrlPtr->TurnOnStageAlarm_LA();	}
	else
	{	IsOK = PlcCtrlPtr->TurnOffStageAlarm_LA();	}
	if ( false == IsOK )
	{	JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString()); }
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneStatus::OnSendInspectionAlarmChkLA() 
{
	// TODO: Add your control notification handler code here
	bool IsOK = true;
	if ( this->IsDlgButtonChecked(PLCLANE_SEND_INSPECTION_ALARM_CHK_LA) == TRUE )	
	{	IsOK = PlcCtrlPtr->TurnOnInspectAlarm_LA();	}
	else
	{	IsOK = PlcCtrlPtr->TurnOffInspectAlarm_LA();	}
	if ( false == IsOK )
	{	JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString()); }
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneStatus::OnSendStageAlarmChkLB() 
{
	// TODO: Add your control notification handler code here
	bool IsOK = true;
	if ( this->IsDlgButtonChecked(PLCLANE_SEND_STAGE_ALARM_CHK_LB) == TRUE )	
	{	IsOK = PlcCtrlPtr->TurnOnStageAlarm_LB();	}
	else
	{	IsOK = PlcCtrlPtr->TurnOffStageAlarm_LB();	}
	if ( false == IsOK )
	{	JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString()); }
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneStatus::OnSendInspectionAlarmChkLB() 
{
	// TODO: Add your control notification handler code here
	bool IsOK = true;
	if ( this->IsDlgButtonChecked(PLCLANE_SEND_INSPECTION_ALARM_CHK_LB) == TRUE )	
	{	IsOK = PlcCtrlPtr->TurnOnInspectAlarm_LB();	}
	else
	{	IsOK = PlcCtrlPtr->TurnOffInspectAlarm_LB();	}
	if ( false == IsOK )
	{	JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString()); }
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneStatus::OnPCBIn2ndBtnLA() 
{
	// TODO: Add your control notification handler code here
	const bool bStep = true;
	LANE_ID LaneID = LANE_ID_A;
	if ( AOIDataCollect.ExecPCBIn2ndProc(LaneID, true, bStep) == false )
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	}
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneStatus::OnPCBIn3rdBtnLA() 
{
	// TODO: Add your control notification handler code here
	const bool bStep = true;
	LANE_ID LaneID = LANE_ID_A;
	if ( AOIDataCollect.ExecPCBIn3rdProc(LaneID, true, bStep) == false )
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	}
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneStatus::OnPCBIn2ndBtnLB() 
{
	// TODO: Add your control notification handler code here
	const bool bStep = true;
	LANE_ID LaneID = LANE_ID_B;
	if ( AOIDataCollect.ExecPCBIn2ndProc(LaneID, true, bStep) == false )
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	}
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneStatus::OnPCBIn3rdBtnLB() 
{
	// TODO: Add your control notification handler code here
	const bool bStep = true;
	LANE_ID LaneID = LANE_ID_B;
	if ( AOIDataCollect.ExecPCBIn3rdProc(LaneID, true, bStep) == false )
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	}
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneStatus::OnPCBAutoOutInBtnLA() 
{
	// TODO: Add your control notification handler code here	
	LANE_ID LaneID=LANE_ID_A;
	const bool bWait = true;
	const bool bStep = true;
	if ( AOIDataCollect.ExecPCBAutoOutInProc(LaneID, bWait, bStep) == false)
	{	JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString()); }	
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneStatus::OnPCBAutoBackInBtnLA() 
{
	// TODO: Add your control notification handler code here
	LANE_ID LaneID=LANE_ID_A;
	const bool bWait = true;	
	const bool bStep = true;
	if ( AOIDataCollect.ExecPCBAutoBackInProc(LaneID, bWait, bStep) == false)
	{	JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString()); }
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneStatus::OnPCBAutoOutInBtnLB() 
{
	// TODO: Add your control notification handler code here
	LANE_ID LaneID=LANE_ID_B;
	const bool bWait = true;
	const bool bStep = true;
	if ( AOIDataCollect.ExecPCBAutoOutInProc(LaneID, bWait, bStep) == false)
	{	JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString()); }
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneStatus::OnPCBAutoBackInBtnLB() 
{
	// TODO: Add your control notification handler code here
	LANE_ID LaneID=LANE_ID_B;
	const bool bWait = true;
	const bool bStep = true;
	if ( AOIDataCollect.ExecPCBAutoBackInProc(LaneID, bWait, bStep) == false)
	{	JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString()); }
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneStatus::OnMachineStartBtn() 
{
	// TODO: Add your control notification handler code here
	bool IsOK = true;	
	IsOK = PlcCtrlPtr->PushDownStartBtn();
	if ( false == IsOK )
	{	JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString()); }
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneStatus::OnMachineResetBtn() 
{
	// TODO: Add your control notification handler code here
	bool IsOK = true;	
	IsOK = PlcCtrlPtr->PushDownResetBtn();
	if ( false == IsOK )
	{	JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString()); }

	::Sleep(500);//為了讓左右進料參數更新所做的延遲
	AOIDataCollect.PostMainFrameWndMessage(MSG_RIBBON_BAR_WND, WPARAM_UPDATE_PCB_DIRECTION, NULL);	
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneStatus::OnMachineStopBtn() 
{
	// TODO: Add your control notification handler code here
	bool IsOK = true;	
	IsOK = PlcCtrlPtr->PushDownStopBtn();
	if ( false == IsOK )
	{	JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString()); }	
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneStatus::OnPCBClearBtnLA() 
{
	// TODO: Add your control notification handler code here
	if ( PlcCtrlPtr->ExecPCBClear_LA(true) == false )
	{	JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString()); }
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneStatus::OnPCBClearBtnLB() 
{
	// TODO: Add your control notification handler code here
	if ( PlcCtrlPtr->ExecPCBClear_LB(true) == false )
	{	JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString()); }
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneStatus::OnPCBOutWithInBtnLA() 
{
	// TODO: Add your control notification handler code here
	LANE_ID LaneID=LANE_ID_A;
	const bool bWait = true;
	const bool bStep = true;
	if ( AOIDataCollect.ExecPCBOutInProc(LaneID, bWait, bStep) == false)
	{	JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString()); }
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneStatus::OnPCBOutWithInBtnLB() 
{
	// TODO: Add your control notification handler code here
	LANE_ID LaneID=LANE_ID_B;
	const bool bWait = true;
	const bool bStep = true;
	if ( AOIDataCollect.ExecPCBOutInProc(LaneID, bWait, bStep) == false)
	{	JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString()); }
}
//-------------------------------------------------------------------------------------//