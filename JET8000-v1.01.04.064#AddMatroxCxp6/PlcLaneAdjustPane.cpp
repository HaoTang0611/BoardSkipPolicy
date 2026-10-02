// PlcLaneAdjustPane.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "PlcLaneAdjustPane.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
const int PLC_LANE_STATUS_TIMER    = 100;
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CPLCCtrlPaneLaneAdjust dialog
//-------------------------------------------------------------------------------------//
CPLCCtrlPaneLaneAdjust::CPLCCtrlPaneLaneAdjust(CWnd* pParent /*=NULL*/)
	: CDialog(CPLCCtrlPaneLaneAdjust::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPLCCtrlPaneLaneAdjust)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ActiveLane = PLCLANEADJUST_JOG_MOVE_CHK_LA;
	m_KeySwitchStats = -1;
	m_LaneAdjustDisable_LA = -1;
	m_LaneAdjustDisable_LB = -1;

	m_LaneAdjustSensorORG_LA = -1;
	m_LaneAdjustSensorLimit_LA = -1;
	m_ConveryerSensorPCBIn_LA = -1;
	m_ConveryerSensorPCBSlow_LA = -1;
	m_ConveryerSensorPCBStop_LA = -1;
	m_ConveryerSensorPCBOut_LA = -1;
	m_ConveryerSensorPCBStop_LA2 = -1;
	
	m_LaneAdjustSensorORG_LB = -1;
	m_LaneAdjustSensorLimit_LB = -1;
	m_ConveryerSensorPCBIn_LB = -1;
	m_ConveryerSensorPCBSlow_LB = -1;	
	m_ConveryerSensorPCBStop_LB = -1;
	m_ConveryerSensorPCBOut_LB = -1;	
	m_ConveryerSensorPCBStop_LB2 = -1;
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneAdjust::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPLCCtrlPaneLaneAdjust)
	DDX_Control(pDX, PLCLANEADJUST_KEW_SWITCH_ICON, m_KeySwitchWnd);
	DDX_Control(pDX, PLCLANEADJUST_DISABLE_ICON_LA, m_DisableWnd_LA);
	DDX_Control(pDX, PLCLANEADJUST_DISABLE_ICON_LB, m_DisableWnd_LB);
	DDX_Control(pDX, PLCLANEADJUST_ORG_SENSOR_ICON_LB, m_SensorORGWnd_LB);
	DDX_Control(pDX, PLCLANEADJUST_MAX_SENSOR_ICON_LB, m_SensorLimitWnd_LB);
	DDX_Control(pDX, PLCLANEADJUST_SENSOR_PCB_IN_ICON_LB, m_SensorPCBInWnd_LB);
	DDX_Control(pDX, PLCLANEADJUST_SENSOR_PCB_SLOW_ICON_LB, m_SensorPCBSlowWnd_LB);
	DDX_Control(pDX, PLCLANEADJUST_SENSOR_PCB_STOP_ICON_LB, m_SensorPCBStopWnd_LB);
	DDX_Control(pDX, PLCLANEADJUST_SENSOR_PCB_STOP_ICON_LB2, m_SensorPCBStopWnd_LB2);
	DDX_Control(pDX, PLCLANEADJUST_SENSOR_PCB_OUT_ICON_LB, m_SensorPCBOutWnd_LB);			
	DDX_Control(pDX, PLCLANEADJUST_ORG_SENSOR_ICON_LA, m_SensorORGWnd_LA);
	DDX_Control(pDX, PLCLANEADJUST_MAX_SENSOR_ICON_LA, m_SensorLimitWnd_LA);
	DDX_Control(pDX, PLCLANEADJUST_SENSOR_PCB_IN_ICON_LA, m_SensorPCBInWnd_LA);
	DDX_Control(pDX, PLCLANEADJUST_SENSOR_PCB_SLOW_ICON_LA, m_SensorPCBSlowWnd_LA);
	DDX_Control(pDX, PLCLANEADJUST_SENSOR_PCB_STOP_ICON_LA, m_SensorPCBStopWnd_LA);
	DDX_Control(pDX, PLCLANEADJUST_SENSOR_PCB_STOP_ICON_LA2, m_SensorPCBStopWnd_LA2);
	DDX_Control(pDX, PLCLANEADJUST_SENSOR_PCB_OUT_ICON_LA, m_SensorPCBOutWnd_LA);	
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CPLCCtrlPaneLaneAdjust, CDialog)
	//{{AFX_MSG_MAP(CPLCCtrlPaneLaneAdjust)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_BN_CLICKED(PLCLANEADJUST_HOME_BTN_LA, OnHomeBtnLA)
	ON_BN_CLICKED(PLCLANEADJUST_MOVE_TO_BTN_LA, OnMoveToBtnLA)
	ON_BN_CLICKED(PLCLANEADJUST_MAX_LIMIT_BTN_LA, OnMaxLimitBtnLA)	
	ON_BN_CLICKED(PLCLANEADJUST_PCB_IN_BTN_LA, OnPCBInBtnLA)
	ON_BN_CLICKED(PLCLANEADJUST_PCB_OUT_BTN_LA, OnPCBOutBtnLA)
	ON_BN_CLICKED(PLCLANEADJUST_PCB_BACK_BTN_LA, OnPCBBackBtnLA)
	ON_BN_CLICKED(PLCLANEADJUST_JOG_SPEED_FAST_RADIO_LA, OnJogSpeedFastRadioLA)
	ON_BN_CLICKED(PLCLANEADJUST_JOG_SPEED_SLOW_RADIO_LA, OnJogSpeedSlowRadioLA)
	ON_BN_CLICKED(PLCLANEADJUST_JOG_MOVE_CHK_LA, OnJogMoveChkLA)
	ON_BN_CLICKED(PLCLANEADJUST_HOME_BTN_LB, OnHomeBtnLB)
	ON_BN_CLICKED(PLCLANEADJUST_MOVE_TO_BTN_LB, OnMoveToBtnLB)
	ON_BN_CLICKED(PLCLANEADJUST_MAX_LIMIT_BTN_LB, OnMaxLimitBtnLB)
	ON_BN_CLICKED(PLCLANEADJUST_JOG_MOVE_CHK_LB, OnJogMoveChkLB)
	ON_BN_CLICKED(PLCLANEADJUST_JOG_SPEED_FAST_RADIO_LB, OnJogSpeedFastRadioLB)
	ON_BN_CLICKED(PLCLANEADJUST_JOG_SPEED_SLOW_RADIO_LB, OnJogSpeedSlowRadioLB)
	ON_BN_CLICKED(PLCLANEADJUST_PCB_IN_BTN_LB, OnPCBInBtnLB)
	ON_BN_CLICKED(PLCLANEADJUST_PCB_OUT_BTN_LB, OnPCBOutBtnLB)
	ON_BN_CLICKED(PLCLANEADJUST_PCB_BACK_BTN_LB, OnPCBBackBtnLB)
	ON_BN_CLICKED(PLCLANEADJUST_SKEW_PITCH_BTN_LA, OnSkewPitchBtnLA)	
	ON_BN_CLICKED(PLCLANEADJUST_FIXED14LANE_CHK, OnFixed14LaneChk)
	ON_BN_CLICKED(PLCLANEADJUST_PCB_CLEAR_BTN_LA, OnPCBClearBtnLA)
	ON_BN_CLICKED(PLCLANEADJUST_PCB_CLEAR_BTN_LB, OnPCBClearBtnLB)
	ON_BN_CLICKED(PLCLANEADJUST_SKEW_PITCH_BTN_LB, OnSkewPitchBtnLB)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CPLCCtrlPaneLaneAdjust message handlers
//-------------------------------------------------------------------------------------//
BOOL CPLCCtrlPaneLaneAdjust::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString str;
	this->m_LEDGreen.LoadBitmap(IDB_LED_MEDIAN_GREEN);
	this->m_LEDRed.LoadBitmap(IDB_LED_MEDIAN_RED);
	this->m_LEDGray.LoadBitmap(IDB_LED_MEDIAN_GRAY);

	this->m_LEDGreenSmall.LoadBitmap(IDB_LED_SMALL_GREEN);
	this->m_LEDGraySmall.LoadBitmap(IDB_LED_SMALL_GRAY);
	this->m_LEDRedSmall.LoadBitmap(IDB_LED_SMALL_RED);

	m_ActiveLane = PLCLANEADJUST_JOG_MOVE_CHK_LA;

	double SkewPitch_LA = PlcCtrlPtr->GetLaneAdjustSkewPitch_LA();
	str.Format(_T("%.2f"), SkewPitch_LA);
	SetDlgItemText(PLCLANEADJUST_SKEW_PITCH_EDIT_LA, str);

	double SkewPitch_LB = PlcCtrlPtr->GetLaneAdjustSkewPitch_LB();
	str.Format(_T("%.2f"), SkewPitch_LB);
	SetDlgItemText(PLCLANEADJUST_SKEW_PITCH_EDIT_LB, str);

	SetDlgItemText(PLCLANEADJUST_POSITION_EDIT_LA, _T("0"));
	SetDlgItemText(PLCLANEADJUST_MOVE_TO_EDIT_LA, _T("100"));
	SetDlgItemText(PLCLANEADJUST_STATUS_EDIT_LA, _T(""));
	SetDlgItemText(PLCLANEADJUST_ERROR_CODE_EDIT_LA, _T(""));

	SetDlgItemText(PLCLANEADJUST_POSITION_EDIT_LB, _T("0"));
	SetDlgItemText(PLCLANEADJUST_MOVE_TO_EDIT_LB, _T("100"));
	SetDlgItemText(PLCLANEADJUST_STATUS_EDIT_LB, _T(""));
	SetDlgItemText(PLCLANEADJUST_ERROR_CODE_EDIT_LB, _T(""));

	double LimitMaxLA = PlcCtrlPtr->GetLaneAdjustLimitMax_LA();
	str.Format(_T("%.2f"), LimitMaxLA);
	SetDlgItemText(PLCLANEADJUST_MAX_LIMIT_EDIT_LA, str);

	double LimitMaxLB = PlcCtrlPtr->GetLaneAdjustLimitMax_LB();
	str.Format(_T("%.2f"), LimitMaxLB);
	SetDlgItemText(PLCLANEADJUST_MAX_LIMIT_EDIT_LB, str);

	bool Fixed14Lane = PlcCtrlPtr->GetLaneAdjustFixed14Lane();
	CheckDlgButton(PLCLANEADJUST_FIXED14LANE_CHK, Fixed14Lane);

	CheckDlgButton(m_ActiveLane, TRUE);
	CheckDlgButton(PLCLANEADJUST_JOG_SPEED_SLOW_RADIO_LA, TRUE);
	CheckDlgButton(PLCLANEADJUST_JOG_SPEED_SLOW_RADIO_LB, TRUE);
	
	this->SwitchMultiLanguage();
	EnableLaneStatus();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneAdjust::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneAdjust::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneAdjust::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if ( TRUE == bShow )
	{
		EnableLaneStatus();
		SwitchMultiLanguage();
		PlcCtrlPtr->SetPLCPollingLaneAdjustSensor(true);
		if ( PlcCtrlPtr->GetPLCIsConnected() == true )
		{	PlcCtrlPtr->WriteLaneAdjustManual(true);	}
		this->SetTimer(PLC_LANE_STATUS_TIMER, 250, 0);	
	}
	else
	{
		PlcCtrlPtr->SetPLCPollingLaneAdjustSensor(false);
		if ( PlcCtrlPtr->GetPLCIsConnected() == true )
		{	PlcCtrlPtr->WriteLaneAdjustManual(false);	}
		this->KillTimer(PLC_LANE_STATUS_TIMER);		
	}
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneAdjust::OnOK() 
{
	// TODO: Add extra validation here
	return;
	CDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneAdjust::OnCancel() 
{
	// TODO: Add extra cleanup here
	return;
	CDialog::OnCancel();
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneAdjust::OnTimer(UINT_PTR nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	switch ( nIDEvent )
	{
	case PLC_LANE_STATUS_TIMER:	
		this->UpdateLaneStatus();		
		break;	
	}
	CDialog::OnTimer(nIDEvent);
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneAdjust::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_PLC_LANE_ADJUST_PANE");
	//---------------------------------------------------------------------------------//
	WndID = IDD_PLC_LANE_ADJUST_PANE;
	WndKey = _T("IDD_PLC_LANE_ADJUST_PANE");
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
	WndID = PLCLANEADJUST_KEW_SWITCH_LABEL;
	WndKey = _T("PLCLANEADJUST_KEW_SWITCH_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCLANEADJUST_DISABLE_LABEL_LA;
	WndKey = _T("PLCLANEADJUST_DISABLE_LABEL_LA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCLANEADJUST_DISABLE_LABEL_LB;
	WndKey = _T("PLCLANEADJUST_DISABLE_LABEL_LB");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCLANEADJUST_FIXED14LANE_CHK;
	WndKey = _T("PLCLANEADJUST_FIXED14LANE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		
	//---------------------------------------------------------------------------------//
	//Lane A
	WndID = PLCLANEADJUST_LANE_GROUP_A;
	WndKey = _T("PLCLANEADJUST_LANE_GROUP_A");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCLANEADJUST_ORG_SENSOR_LABEL_LA;
	WndKey = _T("PLCLANEADJUST_ORG_SENSOR_LABEL_LA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PLCLANEADJUST_MAX_SENSOR_LABEL_LA;
	WndKey = _T("PLCLANEADJUST_MAX_SENSOR_LABEL_LA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCLANEADJUST_POSITION_LABEL_LA;
	WndKey = _T("PLCLANEADJUST_POSITION_LABEL_LA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCLANEADJUST_HOME_BTN_LA;
	WndKey = _T("PLCLANEADJUST_HOME_BTN_LA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PLCLANEADJUST_MOVE_TO_BTN_LA;
	WndKey = _T("PLCLANEADJUST_MOVE_TO_BTN_LA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCLANEADJUST_MOVE_TO_LABEL_LA;
	WndKey = _T("PLCLANEADJUST_MOVE_TO_LABEL_LA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCLANEADJUST_MAX_LIMIT_LABEL_LA;
	WndKey = _T("PLCLANEADJUST_MAX_LIMIT_LABEL_LA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCLANEADJUST_MAX_LIMIT_BTN_LA;
	WndKey = _T("PLCLANEADJUST_MAX_LIMIT_BTN_LA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCLANEADJUST_JOG_SPEED_LABEL_LA;
	WndKey = _T("PLCLANEADJUST_JOG_SPEED_LABEL_LA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PLCLANEADJUST_JOG_SPEED_FAST_RADIO_LA;
	WndKey = _T("PLCLANEADJUST_JOG_SPEED_FAST_RADIO_LA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCLANEADJUST_JOG_SPEED_SLOW_RADIO_LA;
	WndKey = _T("PLCLANEADJUST_JOG_SPEED_SLOW_RADIO_LA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCLANEADJUST_JOG_MOVE_CHK_LA;
	WndKey = _T("PLCLANEADJUST_JOG_MOVE_CHK_LA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCLANEADJUST_STATUS_LABEL_LA;
	WndKey = _T("PLCLANEADJUST_STATUS_LABEL_LA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCLANEADJUST_ERROR_CODE_LABEL_LA;
	WndKey = _T("PLCLANEADJUST_ERROR_CODE_LABEL_LA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCLANEADJUST_SENSOR_PCB_IN_LABEL_LA;
	WndKey = _T("PLCLANEADJUST_SENSOR_PCB_IN_LABEL_LA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCLANEADJUST_SENSOR_PCB_SLOW_LABEL_LA;
	WndKey = _T("PLCLANEADJUST_SENSOR_PCB_SLOW_LABEL_LA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCLANEADJUST_SENSOR_PCB_STOP_LABEL_LA;
	WndKey = _T("PLCLANEADJUST_SENSOR_PCB_STOP_LABEL_LA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PLCLANEADJUST_SENSOR_PCB_STOP_LABEL_LA2;
	WndKey = _T("PLCLANEADJUST_SENSOR_PCB_STOP_LABEL_LA2");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PLCLANEADJUST_SENSOR_PCB_OUT_LABEL_LA;
	WndKey = _T("PLCLANEADJUST_SENSOR_PCB_OUT_LABEL_LA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCLANEADJUST_PCB_IN_BTN_LA;
	WndKey = _T("PLCLANEADJUST_PCB_IN_BTN_LA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PLCLANEADJUST_PCB_OUT_BTN_LA;
	WndKey = _T("PLCLANEADJUST_PCB_OUT_BTN_LA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCLANEADJUST_PCB_BACK_BTN_LA;
	WndKey = _T("PLCLANEADJUST_PCB_BACK_BTN_LA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCLANEADJUST_PCB_CLEAR_BTN_LA;
	WndKey = _T("PLCLANEADJUST_PCB_CLEAR_BTN_LA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCLANEADJUST_SKEW_PITCH_LABEL_LA;
	WndKey = _T("PLCLANEADJUST_SKEW_PITCH_LABEL_LA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCLANEADJUST_SKEW_PITCH_BTN_LA;
	WndKey = _T("PLCLANEADJUST_SKEW_PITCH_BTN_LA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//
	//Lane B
	WndID = PLCLANEADJUST_LANE_GROUP_B;
	WndKey = _T("PLCLANEADJUST_LANE_GROUP_B");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCLANEADJUST_ORG_SENSOR_LABEL_LB;
	WndKey = _T("PLCLANEADJUST_ORG_SENSOR_LABEL_LB");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PLCLANEADJUST_MAX_SENSOR_LABEL_LB;
	WndKey = _T("PLCLANEADJUST_MAX_SENSOR_LABEL_LB");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCLANEADJUST_POSITION_LABEL_LB;
	WndKey = _T("PLCLANEADJUST_POSITION_LABEL_LB");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCLANEADJUST_HOME_BTN_LB;
	WndKey = _T("PLCLANEADJUST_HOME_BTN_LB");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PLCLANEADJUST_MOVE_TO_BTN_LB;
	WndKey = _T("PLCLANEADJUST_MOVE_TO_BTN_LB");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCLANEADJUST_MOVE_TO_LABEL_LB;
	WndKey = _T("PLCLANEADJUST_MOVE_TO_LABEL_LB");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCLANEADJUST_MAX_LIMIT_LABEL_LB;
	WndKey = _T("PLCLANEADJUST_MAX_LIMIT_LABEL_LB");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCLANEADJUST_MAX_LIMIT_BTN_LB;
	WndKey = _T("PLCLANEADJUST_MAX_LIMIT_BTN_LB");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCLANEADJUST_JOG_SPEED_LABEL_LB;
	WndKey = _T("PLCLANEADJUST_JOG_SPEED_LABEL_LB");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PLCLANEADJUST_JOG_SPEED_FAST_RADIO_LB;
	WndKey = _T("PLCLANEADJUST_JOG_SPEED_FAST_RADIO_LB");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCLANEADJUST_JOG_SPEED_SLOW_RADIO_LB;
	WndKey = _T("PLCLANEADJUST_JOG_SPEED_SLOW_RADIO_LB");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCLANEADJUST_JOG_MOVE_CHK_LB;
	WndKey = _T("PLCLANEADJUST_JOG_MOVE_CHK_LB");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCLANEADJUST_STATUS_LABEL_LB;
	WndKey = _T("PLCLANEADJUST_STATUS_LABEL_LB");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCLANEADJUST_ERROR_CODE_LABEL_LB;
	WndKey = _T("PLCLANEADJUST_ERROR_CODE_LABEL_LB");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCLANEADJUST_SENSOR_PCB_IN_LABEL_LB;
	WndKey = _T("PLCLANEADJUST_SENSOR_PCB_IN_LABEL_LB");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCLANEADJUST_SENSOR_PCB_SLOW_LABEL_LB;
	WndKey = _T("PLCLANEADJUST_SENSOR_PCB_SLOW_LABEL_LB");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCLANEADJUST_SENSOR_PCB_STOP_LABEL_LB;
	WndKey = _T("PLCLANEADJUST_SENSOR_PCB_STOP_LABEL_LB");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PLCLANEADJUST_SENSOR_PCB_STOP_LABEL_LB2;
	WndKey = _T("PLCLANEADJUST_SENSOR_PCB_STOP_LABEL_LB2");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PLCLANEADJUST_SENSOR_PCB_OUT_LABEL_LB;
	WndKey = _T("PLCLANEADJUST_SENSOR_PCB_OUT_LABEL_LB");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCLANEADJUST_PCB_IN_BTN_LB;
	WndKey = _T("PLCLANEADJUST_PCB_IN_BTN_LB");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PLCLANEADJUST_PCB_OUT_BTN_LB;
	WndKey = _T("PLCLANEADJUST_PCB_OUT_BTN_LB");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCLANEADJUST_PCB_BACK_BTN_LB;
	WndKey = _T("PLCLANEADJUST_PCB_BACK_BTN_LB");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = PLCLANEADJUST_PCB_CLEAR_BTN_LB;
	WndKey = _T("PLCLANEADJUST_PCB_CLEAR_BTN_LB");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);			

	WndID = PLCLANEADJUST_SKEW_PITCH_LABEL_LB;
	WndKey = _T("PLCLANEADJUST_SKEW_PITCH_LABEL_LB");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCLANEADJUST_SKEW_PITCH_BTN_LB;
	WndKey = _T("PLCLANEADJUST_SKEW_PITCH_BTN_LB");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//
	/*
	WndID = AAAAAAAAAAAAAAAAAAAAAAAA;
	WndKey = _T("AAAAAAAAAAAAAAAAAAAAAAAA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = AAAAAAAAAAAAAAAAAAAAAAAA;
	WndKey = _T("AAAAAAAAAAAAAAAAAAAAAAAA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	*/
	//---------------------------------------------------------------------------------//	
}
//-------------------------------------------------------------------------------------//
CString CPLCCtrlPaneLaneAdjust::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_PLC_LANE_ADJUST_PANE");		
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
BOOL CPLCCtrlPaneLaneAdjust::PreTranslateMessage(MSG* pMsg) 
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
		case VK_UP:			
			if ( CheckKeySwitchTurnOff() == true )
			{
				if ( PLCLANEADJUST_JOG_MOVE_CHK_LB == m_ActiveLane )
				{
					if ( PlcCtrlPtr->GetLaneAdjustJogMoving_LB() == false )
					{	PlcCtrlPtr->ExecLaneAdjustJogMove_LB(true, false);	}
				}
				else
				{
					if ( PlcCtrlPtr->GetLaneAdjustJogMoving_LA() == false )
					{	PlcCtrlPtr->ExecLaneAdjustJogMove_LA(true, false);	}
				}
			}
			break;
		case VK_DOWN:
			if ( CheckKeySwitchTurnOff() == true )
			{
				if ( PLCLANEADJUST_JOG_MOVE_CHK_LB == m_ActiveLane )
				{
					if ( PlcCtrlPtr->GetLaneAdjustJogMoving_LB() == false )
					{	PlcCtrlPtr->ExecLaneAdjustJogMove_LB(false, false);	}
				}
				else
				{
					if ( PlcCtrlPtr->GetLaneAdjustJogMoving_LA() == false )
					{	PlcCtrlPtr->ExecLaneAdjustJogMove_LA(false, false);	}
				}
			}
			break;
		}
		break;
	case WM_KEYUP:
		switch ( pMsg->wParam )
		{		
		case VK_UP:
			PlcCtrlPtr->ExecLaneAdjustJogMove_LA(true, true);
			PlcCtrlPtr->ExecLaneAdjustJogMove_LB(true, true);
			break;
		case VK_DOWN:
			PlcCtrlPtr->ExecLaneAdjustJogMove_LA(false, true);
			PlcCtrlPtr->ExecLaneAdjustJogMove_LB(false, true);
			break;
		}
		break;
	}
	return CDialog::PreTranslateMessage(pMsg);
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneAdjust::UpdateLaneStatus()
{
	CString   str;
	int       nValue = 0;	
	const int TurnOn = 1;	
	LANE_WORK_MODE LaneWorkMode_LA = AOIDataCollect.GetLaneWorkMode_LA();
	LANE_WORK_MODE LaneWorkMode_LB = AOIDataCollect.GetLaneWorkMode_LB();

	nValue = PlcCtrlPtr->GetCurrentKeySwitchStats();
	if ( nValue != m_KeySwitchStats )
	{
		if ( TurnOn == m_KeySwitchStats )
		{	this->m_KeySwitchWnd.SetBitmap(m_LEDRed); }
		else
		{	this->m_KeySwitchWnd.SetBitmap(m_LEDGreen); }
	}
	m_KeySwitchStats = nValue;	

	nValue = PlcCtrlPtr->GetLaneAdjustDisableBtn_LA();
	if ( nValue != m_LaneAdjustDisable_LA )
	{
		if ( TurnOn == nValue )
		{	this->m_DisableWnd_LA.SetBitmap(m_LEDRed); }
		else
		{	this->m_DisableWnd_LA.SetBitmap(m_LEDGray); }
	}
	m_LaneAdjustDisable_LA = nValue;

	nValue = PlcCtrlPtr->GetLaneAdjustDisableBtn_LB();
	if ( nValue != m_LaneAdjustDisable_LB )
	{
		if ( TurnOn == nValue )
		{	this->m_DisableWnd_LB.SetBitmap(m_LEDRed); }
		else
		{	this->m_DisableWnd_LB.SetBitmap(m_LEDGray); }
	}
	m_LaneAdjustDisable_LB = nValue;

	//Lane A	
	if ( LANE_WORK_DISABLE != LaneWorkMode_LA )
	{
		nValue = PlcCtrlPtr->GetLaneAdjustSensorORG_LA();
		if ( nValue != m_LaneAdjustSensorORG_LA )
		{
			if ( TurnOn == nValue )
			{	this->m_SensorORGWnd_LA.SetBitmap(m_LEDGreen); }
			else
			{	this->m_SensorORGWnd_LA.SetBitmap(m_LEDGray); }
		}
		m_LaneAdjustSensorORG_LA = nValue;	

		nValue = PlcCtrlPtr->GetLaneAdjustSensorLimit_LA();
		if ( nValue != m_LaneAdjustSensorLimit_LA )
		{
			if ( TurnOn == nValue )
			{	this->m_SensorLimitWnd_LA.SetBitmap(m_LEDRed); }
			else
			{	this->m_SensorLimitWnd_LA.SetBitmap(m_LEDGray); }
		}
		m_LaneAdjustSensorLimit_LA = nValue;

		nValue = PlcCtrlPtr->GetConveryerSensorPCBIn_LA();
		if ( nValue != m_ConveryerSensorPCBIn_LA )
		{
			if ( TurnOn == nValue )
			{	this->m_SensorPCBInWnd_LA.SetBitmap(m_LEDGreen); }
			else
			{	this->m_SensorPCBInWnd_LA.SetBitmap(m_LEDGray); }
		}
		m_ConveryerSensorPCBIn_LA = nValue;	

		nValue = PlcCtrlPtr->GetConveryerSensorPCBSlow_LA();
		if ( nValue != m_ConveryerSensorPCBSlow_LA )
		{
			if ( TurnOn == nValue )
			{	this->m_SensorPCBSlowWnd_LA.SetBitmap(m_LEDGreen); }
			else
			{	this->m_SensorPCBSlowWnd_LA.SetBitmap(m_LEDGray); }
		}
		m_ConveryerSensorPCBSlow_LA = nValue;	

		nValue = PlcCtrlPtr->GetConveryerSensorPCBStop_LA();
		if ( nValue != m_ConveryerSensorPCBStop_LA )
		{
			if ( TurnOn == nValue )
			{	this->m_SensorPCBStopWnd_LA.SetBitmap(m_LEDGreen); }
			else
			{	this->m_SensorPCBStopWnd_LA.SetBitmap(m_LEDGray); }
		}
		m_ConveryerSensorPCBStop_LA = nValue;	

		nValue = PlcCtrlPtr->GetConveryerSensorPCBOut_LA();
		if ( nValue != m_ConveryerSensorPCBOut_LA )
		{
			if ( TurnOn == nValue )
			{	this->m_SensorPCBOutWnd_LA.SetBitmap(m_LEDGreen); }
			else
			{	this->m_SensorPCBOutWnd_LA.SetBitmap(m_LEDGray); }
		}
		m_ConveryerSensorPCBOut_LA = nValue;	

		nValue = PlcCtrlPtr->GetConveryerSensorPCBStop2_LA();
		if ( nValue != m_ConveryerSensorPCBStop_LA2 )
		{
			if ( TurnOn == nValue )
			{	this->m_SensorPCBStopWnd_LA2.SetBitmap(m_LEDGreen); }
			else
			{	this->m_SensorPCBStopWnd_LA2.SetBitmap(m_LEDGray); }
		}
		m_ConveryerSensorPCBStop_LA2 = nValue;			

		double CurrentPosLA = PlcCtrlPtr->GetLaneAdjustCurrentPos_LA();
		str.Format(_T("%.2f"), CurrentPosLA);
		CWnd::SetDlgItemText(PLCLANEADJUST_POSITION_EDIT_LA, str);

		nValue = PlcCtrlPtr->ReadLaneAdjustPLCErrorCode_LA();
		CWnd::SetDlgItemInt(PLCLANEADJUST_STATUS_EDIT_LA, nValue);

		nValue = PlcCtrlPtr->ReadLaneAdjustMotorErrorCode_LA();
		CWnd::SetDlgItemInt(PLCLANEADJUST_ERROR_CODE_EDIT_LA, nValue);	
	}

	//Lane B
	if ( LANE_WORK_DISABLE != LaneWorkMode_LB )
	{
		nValue = PlcCtrlPtr->GetLaneAdjustSensorORG_LB();
		if ( nValue != m_LaneAdjustSensorORG_LB )
		{
			if ( TurnOn == nValue )
			{	this->m_SensorORGWnd_LB.SetBitmap(m_LEDGreen); }
			else
			{	this->m_SensorORGWnd_LB.SetBitmap(m_LEDGray); }
		}
		m_LaneAdjustSensorORG_LB = nValue;	

		nValue = PlcCtrlPtr->GetLaneAdjustSensorLimit_LB();
		if ( nValue != m_LaneAdjustSensorLimit_LB )
		{
			if ( TurnOn == nValue )
			{	this->m_SensorLimitWnd_LB.SetBitmap(m_LEDRed); }
			else
			{	this->m_SensorLimitWnd_LB.SetBitmap(m_LEDGray); }
		}
		m_LaneAdjustSensorLimit_LB = nValue;

		nValue = PlcCtrlPtr->GetConveryerSensorPCBIn_LB();
		if ( nValue != m_ConveryerSensorPCBIn_LB )
		{
			if ( TurnOn == nValue )
			{	this->m_SensorPCBInWnd_LB.SetBitmap(m_LEDGreen); }
			else
			{	this->m_SensorPCBInWnd_LB.SetBitmap(m_LEDGray); }
		}
		m_ConveryerSensorPCBIn_LB = nValue;	

		nValue = PlcCtrlPtr->GetConveryerSensorPCBSlow_LB();
		if ( nValue != m_ConveryerSensorPCBSlow_LB )
		{
			if ( TurnOn == nValue )
			{	this->m_SensorPCBSlowWnd_LB.SetBitmap(m_LEDGreen); }
			else
			{	this->m_SensorPCBSlowWnd_LB.SetBitmap(m_LEDGray); }
		}
		m_ConveryerSensorPCBSlow_LB = nValue;	

		nValue = PlcCtrlPtr->GetConveryerSensorPCBStop_LB();
		if ( nValue != m_ConveryerSensorPCBStop_LB )
		{
			if ( TurnOn == nValue )
			{	this->m_SensorPCBStopWnd_LB.SetBitmap(m_LEDGreen); }
			else
			{	this->m_SensorPCBStopWnd_LB.SetBitmap(m_LEDGray); }
		}
		m_ConveryerSensorPCBStop_LB = nValue;	

		nValue = PlcCtrlPtr->GetConveryerSensorPCBOut_LB();
		if ( nValue != m_ConveryerSensorPCBOut_LB )
		{
			if ( TurnOn == nValue )
			{	this->m_SensorPCBOutWnd_LB.SetBitmap(m_LEDGreen); }
			else
			{	this->m_SensorPCBOutWnd_LB.SetBitmap(m_LEDGray); }
		}
		m_ConveryerSensorPCBOut_LB = nValue;	

		nValue = PlcCtrlPtr->GetConveryerSensorPCBStop2_LB();
		if ( nValue != m_ConveryerSensorPCBStop_LB2 )
		{
			if ( TurnOn == nValue )
			{	this->m_SensorPCBStopWnd_LB2.SetBitmap(m_LEDGreen); }
			else
			{	this->m_SensorPCBStopWnd_LB2.SetBitmap(m_LEDGray); }
		}
		m_ConveryerSensorPCBStop_LB2 = nValue;	
		
		double CurrentPosLB = PlcCtrlPtr->GetLaneAdjustCurrentPos_LB();
		str.Format(_T("%.2f"), CurrentPosLB);
		CWnd::SetDlgItemText(PLCLANEADJUST_POSITION_EDIT_LB, str);

		nValue = PlcCtrlPtr->ReadLaneAdjustPLCErrorCode_LB();
		CWnd::SetDlgItemInt(PLCLANEADJUST_STATUS_EDIT_LB, nValue);

		nValue = PlcCtrlPtr->ReadLaneAdjustMotorErrorCode_LB();
		CWnd::SetDlgItemInt(PLCLANEADJUST_ERROR_CODE_EDIT_LB, nValue);		
	}
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneAdjust::EnableLaneStatus()
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

	CtrlID = PLCLANEADJUST_MOVE_TO_EDIT_LA;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);
	CtrlID = PLCLANEADJUST_HOME_BTN_LA;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);
	CtrlID = PLCLANEADJUST_MOVE_TO_BTN_LA;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);
	CtrlID = PLCLANEADJUST_MAX_LIMIT_EDIT_LA;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);
	CtrlID = PLCLANEADJUST_MAX_LIMIT_BTN_LA;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);
	CtrlID = PLCLANEADJUST_JOG_SPEED_FAST_RADIO_LA;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);
	CtrlID = PLCLANEADJUST_JOG_SPEED_SLOW_RADIO_LA;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);
	CtrlID = PLCLANEADJUST_JOG_MOVE_CHK_LA;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);
	CtrlID = PLCLANEADJUST_PCB_IN_BTN_LA;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);
	CtrlID = PLCLANEADJUST_PCB_OUT_BTN_LA;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);
	CtrlID = PLCLANEADJUST_PCB_BACK_BTN_LA;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);
	CtrlID = PLCLANEADJUST_PCB_CLEAR_BTN_LA;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);	

	//Lane B
	if ( LANE_WORK_DISABLE == LaneWorkMode_LB )
	{	Enable=FALSE;	}
	else
	{	Enable=TRUE;	}

	CtrlID = PLCLANEADJUST_MOVE_TO_EDIT_LB;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);
	CtrlID = PLCLANEADJUST_HOME_BTN_LB;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);
	CtrlID = PLCLANEADJUST_MOVE_TO_BTN_LB;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);
	CtrlID = PLCLANEADJUST_MAX_LIMIT_EDIT_LB;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);
	CtrlID = PLCLANEADJUST_MAX_LIMIT_BTN_LB;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);
	CtrlID = PLCLANEADJUST_JOG_SPEED_FAST_RADIO_LB;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);
	CtrlID = PLCLANEADJUST_JOG_SPEED_SLOW_RADIO_LB;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);
	CtrlID = PLCLANEADJUST_JOG_MOVE_CHK_LB;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);
	CtrlID = PLCLANEADJUST_PCB_IN_BTN_LB;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);
	CtrlID = PLCLANEADJUST_PCB_OUT_BTN_LB;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);
	CtrlID = PLCLANEADJUST_PCB_BACK_BTN_LB;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);	
	CtrlID = PLCLANEADJUST_PCB_CLEAR_BTN_LB;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);	

	if ( false == AOIDataCollect.GetUIEnablePCBOutButton() )
	{	
		JetAPI::EnableCtrlWnd(this, PLCLANEADJUST_PCB_OUT_BTN_LA, FALSE);	
		JetAPI::EnableCtrlWnd(this, PLCLANEADJUST_PCB_OUT_BTN_LB, FALSE);	
	}
}
//-------------------------------------------------------------------------------------//
bool CPLCCtrlPaneLaneAdjust::CheckKeySwitchTurnOff(bool bShowMsg)
{
	CString str;
	bool KeySwitchStats=PlcCtrlPtr->GetCurrentKeySwitchStats();
	if ( false == KeySwitchStats ) { return true; }
	if ( true == bShowMsg )
	{
		str = "Error, Turn Off Key Switch First";
		str = LoadMultiLanguageString(str, str);
		JetAPI::ShowMessageBox(str);
	}
	return false;
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneAdjust::OnHomeBtnLA() 
{
	// TODO: Add your control notification handler code here
	if ( CheckKeySwitchTurnOff() == false ) { return; }
	if ( PlcCtrlPtr->ExecLaneAdjustHomeSearch_LA(true) == false )
	{	JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString());	}
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneAdjust::OnMoveToBtnLA() 
{
	// TODO: Add your control notification handler code here
	CString str;
	GetDlgItemText(PLCLANEADJUST_MOVE_TO_EDIT_LA, str);
	double Pos = JetAPI::StrToDbl(str);
	if ( CheckKeySwitchTurnOff() == false ) { return; }
	if ( PlcCtrlPtr->ExecLaneAdjustMoveTo_LA(Pos, true) == false )
	{	JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString());	}
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneAdjust::OnMaxLimitBtnLA() 
{
	// TODO: Add your control notification handler code here
	CString str;
	GetDlgItemText(PLCLANEADJUST_MAX_LIMIT_EDIT_LA, str);
	double Pos = JetAPI::StrToDbl(str);
	if ( PlcCtrlPtr->WriteLaneAdjustLimitMaxPos_LA(Pos) == false )
	{	JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString());	}
	PlcCtrlPtr->SavePLCINIParameter();	
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneAdjust::OnPCBInBtnLA() 
{
	// TODO: Add your control notification handler code here
	const bool bStep = true;
	LANE_ID LaneID = LANE_ID_A;	
	if ( AOIDataCollect.ExecPCBInProc(LaneID, true, bStep) == false )
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	}
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneAdjust::OnPCBOutBtnLA() 
{
	// TODO: Add your control notification handler code here
	const bool bStep = true;
	LANE_ID LaneID = LANE_ID_A;	
	if ( AOIDataCollect.ExecPCBOutProc(LaneID, true, bStep) == false )
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	}
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneAdjust::OnPCBBackBtnLA() 
{
	// TODO: Add your control notification handler code here
	const bool bStep = true;
	LANE_ID LaneID = LANE_ID_A;	
	if ( AOIDataCollect.ExecPCBBackProc(LaneID, true, bStep) == false )
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	}
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneAdjust::OnJogSpeedFastRadioLA() 
{
	// TODO: Add your control notification handler code here
	const double JogSpeed = PlcCtrlPtr->GetLaneAdjustJogFastSpeed_LA();
	if ( PlcCtrlPtr->WriteLaneAdjustJogSpeed_LA(JogSpeed) == false )
	{	
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	
		return ;
	}	
	CWnd *pWnd = this->GetDlgItem(PLCLANEADJUST_MOVE_TO_EDIT_LA);
	if ( pWnd!=NULL && pWnd->GetSafeHwnd()!=NULL )
	{	pWnd->SetFocus();	}
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneAdjust::OnJogSpeedSlowRadioLA() 
{
	// TODO: Add your control notification handler code here
	const double JogSpeed = PlcCtrlPtr->GetLaneAdjustJogSlowSpeed_LA();
	if ( PlcCtrlPtr->WriteLaneAdjustJogSpeed_LA(JogSpeed) == false )
	{	
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	
		return ;
	}	
	CWnd *pWnd = this->GetDlgItem(PLCLANEADJUST_MOVE_TO_EDIT_LA);
	if ( pWnd!=NULL && pWnd->GetSafeHwnd()!=NULL )
	{	pWnd->SetFocus();	}
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneAdjust::OnJogMoveChkLA() 
{
	// TODO: Add your control notification handler code here
	m_ActiveLane = PLCLANEADJUST_JOG_MOVE_CHK_LA;
	CWnd::CheckDlgButton(PLCLANEADJUST_JOG_MOVE_CHK_LB, FALSE);
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneAdjust::OnHomeBtnLB() 
{
	// TODO: Add your control notification handler code here
	if ( CheckKeySwitchTurnOff() == false ) { return; }
	if ( PlcCtrlPtr->ExecLaneAdjustHomeSearch_LB(true) == false )
	{	JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString());	}
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneAdjust::OnMoveToBtnLB() 
{
	// TODO: Add your control notification handler code here
	CString str;
	GetDlgItemText(PLCLANEADJUST_MOVE_TO_EDIT_LB, str);
	double Pos = JetAPI::StrToDbl(str);
	if ( CheckKeySwitchTurnOff() == false ) { return; }
	if ( PlcCtrlPtr->ExecLaneAdjustMoveTo_LB(Pos, true) == false )
	{	JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString());	}
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneAdjust::OnMaxLimitBtnLB() 
{
	// TODO: Add your control notification handler code here
	CString str;
	GetDlgItemText(PLCLANEADJUST_MAX_LIMIT_EDIT_LB, str);
	double Pos = JetAPI::StrToDbl(str);
	if ( PlcCtrlPtr->WriteLaneAdjustLimitMaxPos_LB(Pos) == false )
	{	JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString());	}
	PlcCtrlPtr->SavePLCINIParameter();	
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneAdjust::OnJogMoveChkLB() 
{
	// TODO: Add your control notification handler code here
	m_ActiveLane = PLCLANEADJUST_JOG_MOVE_CHK_LB;
	CWnd::CheckDlgButton(PLCLANEADJUST_JOG_MOVE_CHK_LA, FALSE);
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneAdjust::OnJogSpeedFastRadioLB() 
{
	// TODO: Add your control notification handler code here
	const double JogSpeed = PlcCtrlPtr->GetLaneAdjustJogFastSpeed_LB();
	if ( PlcCtrlPtr->WriteLaneAdjustJogSpeed_LB(JogSpeed) == false )
	{	
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	
		return ;
	}	
	CWnd *pWnd = this->GetDlgItem(PLCLANEADJUST_MOVE_TO_EDIT_LB);
	if ( pWnd!=NULL && pWnd->GetSafeHwnd()!=NULL )
	{	pWnd->SetFocus();	}
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneAdjust::OnJogSpeedSlowRadioLB() 
{
	// TODO: Add your control notification handler code here
	const double JogSpeed = PlcCtrlPtr->GetLaneAdjustJogSlowSpeed_LB();
	if ( PlcCtrlPtr->WriteLaneAdjustJogSpeed_LB(JogSpeed) == false )
	{	
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	
		return ;
	}	
	CWnd *pWnd = this->GetDlgItem(PLCLANEADJUST_MOVE_TO_EDIT_LB);
	if ( pWnd!=NULL && pWnd->GetSafeHwnd()!=NULL )
	{	pWnd->SetFocus();	}
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneAdjust::OnPCBInBtnLB() 
{
	// TODO: Add your control notification handler code here
	const bool bStep = true;
	LANE_ID LaneID = LANE_ID_B;
	if ( AOIDataCollect.ExecPCBInProc(LaneID, true, bStep) == false )
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	}
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneAdjust::OnPCBOutBtnLB() 
{
	// TODO: Add your control notification handler code here
	const bool bStep = true;
	LANE_ID LaneID = LANE_ID_B;
	if ( AOIDataCollect.ExecPCBOutProc(LaneID, true, bStep) == false )
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	}
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneAdjust::OnPCBBackBtnLB() 
{
	// TODO: Add your control notification handler code here
	const bool bStep = true;
	LANE_ID LaneID = LANE_ID_B;
	if ( AOIDataCollect.ExecPCBBackProc(LaneID, true, bStep) == false )
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	}
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneAdjust::OnSkewPitchBtnLA() 
{
	// TODO: Add your control notification handler code here
	CString str;
	CWnd::GetDlgItemText(PLCLANEADJUST_SKEW_PITCH_EDIT_LA, str);
	double SkewPitch = JetAPI::StrToDbl(str);	
	if ( PlcCtrlPtr->WriteLaneAdjustSkewPitch_LA(SkewPitch) == false )
	{	JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString());	}	
	PlcCtrlPtr->SavePLCINIParameter();	
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneAdjust::OnFixed14LaneChk() 
{
	// TODO: Add your control notification handler code here
	bool bOn = true;
	BOOL bChk = CWnd::IsDlgButtonChecked(PLCLANEADJUST_FIXED14LANE_CHK);
	if ( TRUE == bChk ) 
	{	bOn = true; }
	else
	{	bOn = false; }
	if ( PlcCtrlPtr->WriteLaneAdjustFixed14Lane(bOn) == false )
	{	JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString());	}
	PlcCtrlPtr->SavePLCINIParameter();	
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneAdjust::OnPCBClearBtnLA() 
{
	// TODO: Add your control notification handler code here
	const bool bStep = true;
	LANE_ID LaneID = LANE_ID_A;
	if ( AOIDataCollect.ExecPCBClearProc(LaneID, true, bStep) == false )
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	}
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneAdjust::OnPCBClearBtnLB() 
{
	// TODO: Add your control notification handler code here
	const bool bStep = true;
	LANE_ID LaneID = LANE_ID_B;
	if ( AOIDataCollect.ExecPCBClearProc(LaneID, true, bStep) == false )
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	}
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneLaneAdjust::OnSkewPitchBtnLB() 
{
	// TODO: Add your control notification handler code here
	CString str;
	CWnd::GetDlgItemText(PLCLANEADJUST_SKEW_PITCH_EDIT_LB, str);
	double SkewPitch = JetAPI::StrToDbl(str);		
	if ( PlcCtrlPtr->WriteLaneAdjustSkewPitch_LB(SkewPitch) == false )
	{	JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString());	}
	PlcCtrlPtr->SavePLCINIParameter();	
}
//-------------------------------------------------------------------------------------//