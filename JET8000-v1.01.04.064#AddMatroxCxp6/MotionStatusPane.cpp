// MotionStatusPane.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "MotionStatusPane.h"
//-------------------------------------------------------------------------------------//
#include "InputBoxWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CMotionCtrlPaneStatus dialog
//-------------------------------------------------------------------------------------//
CMotionCtrlPaneStatus::CMotionCtrlPaneStatus(CWnd* pParent /*=NULL*/)
	: CDialog(CMotionCtrlPaneStatus::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMotionCtrlPaneStatus)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	this->m_CommandPosX1 = 0;
	this->m_CommandPosY1 = 0;
	this->m_CommandPosZ1 = 0;

	this->m_CommandPosX2 = 0;
	this->m_CommandPosY2 = 0;
	this->m_CommandPosZ2 = 0;

	this->m_CommandPosX3 = 0;
	this->m_CommandPosY3 = 0;
	this->m_CommandPosZ3 = 0;
	
	m_RepeatTime = 1000;
	m_RepeatToMode = MOTION_REPEAT_TO_1;
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMotionCtrlPaneStatus)
	DDX_Control(pDX, MSTATUS_ALARM_IMG_Z, m_AlarmWndZ);
	DDX_Control(pDX, MSTATUS_EMG_IMG_Z, m_EmgWndZ);
	DDX_Control(pDX, MSTATUS_INPOS_IMG_Z, m_InposWndZ);
	DDX_Control(pDX, MSTATUS_NLIMIT_IMG_Z, m_NLimitWndZ);
	DDX_Control(pDX, MSTATUS_PLIMIT_IMG_Z, m_PLimitWndZ);
	DDX_Control(pDX, MSTATUS_ORG_IMG_Z, m_OrgWndZ);
	DDX_Control(pDX, MSTATUS_READY_IMG_Z, m_ReadyWndZ);
	DDX_Control(pDX, MSTATUS_ENABLE_IMG_Z, m_EnableWndZ);
	DDX_Control(pDX, MSTATUS_ALARM_IMG_Y, m_AlarmWndY);
	DDX_Control(pDX, MSTATUS_EMG_IMG_Y, m_EmgWndY);
	DDX_Control(pDX, MSTATUS_INPOS_IMG_Y, m_InposWndY);
	DDX_Control(pDX, MSTATUS_NLIMIT_IMG_Y, m_NLimitWndY);
	DDX_Control(pDX, MSTATUS_PLIMIT_IMG_Y, m_PLimitWndY);
	DDX_Control(pDX, MSTATUS_ORG_IMG_Y, m_OrgWndY);
	DDX_Control(pDX, MSTATUS_READY_IMG_Y, m_ReadyWndY);
	DDX_Control(pDX, MSTATUS_ENABLE_IMG_Y, m_EnableWndY);	
	DDX_Control(pDX, MSTATUS_ALARM_IMG_X, m_AlarmWndX);
	DDX_Control(pDX, MSTATUS_EMG_IMG_X, m_EmgWndX);
	DDX_Control(pDX, MSTATUS_INPOS_IMG_X, m_InposWndX);
	DDX_Control(pDX, MSTATUS_NLIMIT_IMG_X, m_NLimitWndX);
	DDX_Control(pDX, MSTATUS_PLIMIT_IMG_X, m_PLimitWndX);
	DDX_Control(pDX, MSTATUS_ORG_IMG_X, m_OrgWndX);
	DDX_Control(pDX, MSTATUS_READY_IMG_X, m_ReadyWndX);
	DDX_Control(pDX, MSTATUS_ENABLE_IMG_X, m_EnableWndX);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CMotionCtrlPaneStatus, CDialog)
	//{{AFX_MSG_MAP(CMotionCtrlPaneStatus)
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_BN_CLICKED(MSTATUS_GO_TO_POS_BTN_1, OnGoToPosBtn1)
	ON_BN_CLICKED(MSTATUS_GO_TO_POS_BTN_2, OnGoToPosBtn2)
	ON_BN_CLICKED(MSTATUS_GO_TO_POS_BTN_3, OnGoToPosBtn3)
	ON_BN_CLICKED(MSTATUS_GET_POS_BTN_1, OnGetPosBtn1)
	ON_BN_CLICKED(MSTATUS_GET_POS_BTN_2, OnGetPosBtn2)
	ON_BN_CLICKED(MSTATUS_GET_POS_BTN_3, OnGetPosBtn3)
	ON_BN_CLICKED(MSTATUS_NMOVE_POS_BTN_X, OnNMovePosBtnX)
	ON_BN_CLICKED(MSTATUS_PMOVE_POS_BTN_X, OnPMovePosBtnX)
	ON_BN_CLICKED(MSTATUS_NMOVE_POS_BTN_Y, OnNMovePosBtnY)
	ON_BN_CLICKED(MSTATUS_PMOVE_POS_BTN_Y, OnPMovePosBtnY)
	ON_BN_CLICKED(MSTATUS_NMOVE_POS_BTN_Z, OnNMovePosBtnZ)
	ON_BN_CLICKED(MSTATUS_PMOVE_POS_BTN_Z, OnPMovePosBtnZ)
	ON_BN_CLICKED(MSTATUS_GET_POS_BTN_X, OnGetPosBtnX)
	ON_BN_CLICKED(MSTATUS_GET_POS_BTN_Y, OnGetPosBtnY)
	ON_BN_CLICKED(MSTATUS_GET_POS_BTN_Z, OnGetPosBtnZ)
	ON_BN_CLICKED(MSTATUS_JOG_CHK, OnJogChk)	
	ON_BN_CLICKED(MSTATUS_ENABLE_CHK_X, OnEnableChkX)
	ON_BN_CLICKED(MSTATUS_ENABLE_CHK_Y, OnEnableChkY)
	ON_BN_CLICKED(MSTATUS_ENABLE_CHK_Z, OnEnableChkZ)
	ON_BN_CLICKED(MSTATUS_HOME_BTN_X, OnHomeBtnX)
	ON_BN_CLICKED(MSTATUS_HOME_BTN_Y, OnHomeBtnY)
	ON_BN_CLICKED(MSTATUS_HOME_BTN_Z, OnHomeBtnZ)
	ON_BN_CLICKED(MSTATUS_RESET_BTN_X, OnResetBtnX)
	ON_BN_CLICKED(MSTATUS_RESET_BTN_Y, OnResetBtnY)
	ON_BN_CLICKED(MSTATUS_RESET_BTN_Z, OnResetBtnZ)
	ON_BN_CLICKED(MSTATUS_CHECK_LIMIT_BTN_X, OnCheckLimitBtnX)
	ON_BN_CLICKED(MSTATUS_CHECK_LIMIT_BTN_Y, OnCheckLimitBtnY)
	ON_BN_CLICKED(MSTATUS_CHECK_LIMIT_BTN_Z, OnCheckLimitBtnZ)
	ON_BN_CLICKED(MSTATUS_START_POS_GO_BTN, OnStartPosGoBtn)
	ON_BN_CLICKED(MSTATUS_START_POS_SET_BTN, OnStartPosSetBtn)
	ON_BN_CLICKED(MSTATUS_FOCUS_POS_GO_BTN, OnFocusPosGoBtn)
	ON_BN_CLICKED(MSTATUS_FOCUS_POS_SET_BTN, OnFocusPosSetBtn)
	ON_BN_CLICKED(MSTATUS_HOME_ALL_BTN, OnHomeAllBtn)
	ON_BN_CLICKED(MSTATUS_ORG_POS_GO_BTN, OnORGPosGoBtn)
	ON_BN_CLICKED(MSTATUS_PCB_STOP_POS_GO_BTN_LA, OnPCBStopPosGoBtnLA)
	ON_BN_CLICKED(MSTATUS_PCB_STOP_POS_SET_BTN_LA, OnPCBStopPosSetBtnLA)
	ON_BN_CLICKED(MSTATUS_PCB_IN_POS_GO_BTN, OnPCBInPosGoBtn)
	ON_BN_CLICKED(MSTATUS_PCB_IN_POS_SET_BTN, OnPCBInPosSetBtn)
	ON_BN_CLICKED(MSTATUS_REPEAT_12_CHK, OnRepeat12Chk)
	ON_BN_CLICKED(MSTATUS_REPEAT_13_CHK, OnRepeat13Chk)
	ON_BN_CLICKED(MSTATUS_REPEAT_23_CHK, OnRepeat23Chk)
	ON_BN_CLICKED(MSTATUS_TIME_TEST_BTN, OnTimeTestBtn)
	ON_BN_CLICKED(MSTATUS_PCB_STOP_POS_GO_BTN_LB, OnPCBStopPosGoBtnLB)
	ON_BN_CLICKED(MSTATUS_PCB_STOP_POS_SET_BTN_LB, OnPCBStopPosSetBtnLB)
	ON_BN_CLICKED(MSTATUS_LEAVE_POS_GO_BTN, OnLeavePosGoBtn)
	ON_BN_CLICKED(MSTATUS_LEAVE_POS_SET_BTN, OnLeavePosSetBtn)
	ON_BN_CLICKED(MSTATUS_ORG_POS_SET_BTN, OnORGPosSetBtn)
	ON_BN_CLICKED(MSTATUS_PCB_STOP_POS_GO_BTN_LA2, OnPCBStopPosGoBtnLA2)
	ON_BN_CLICKED(MSTATUS_PCB_STOP_POS_GO_BTN_LB2, OnPCBStopPosGoBtnLB2)
	ON_BN_CLICKED(MSTATUS_PCB_STOP_POS_SET_BTN_LA2, OnPCBStopPosSetBtnLA2)
	ON_BN_CLICKED(MSTATUS_PCB_STOP_POS_SET_BTN_LB2, OnPCBStopPosSetBtnLB2)
	ON_BN_CLICKED(MSTATUS_SAVE_UI_BTN, OnSaveUIBtn)
	ON_BN_CLICKED(MSTATUS_LOAD_UI_BTN, OnLoadUIBtn)
	ON_BN_CLICKED(MSTATUS_LANE_LED_STOP_POS_GO_BTN_LA, OnLaneLedStopPosGoBtnLA)
	ON_BN_CLICKED(MSTATUS_LANE_LED_STOP_POS_SET_BTN_LA, OnLaneLedStopPosSetBtnLA)
	ON_BN_CLICKED(MSTATUS_LANE_LED_SLOW_POS_GO_BTN_LA, OnLaneLedSlowPosGoBtnLA)
	ON_BN_CLICKED(MSTATUS_LANE_LED_SLOW_POS_SET_BTN_LA, OnLaneLedSlowPosSetBtnLA)
	ON_BN_CLICKED(MSTATUS_LANE_LED_STOP_POS_GO_BTN_LB, OnLaneLedStopPosGoBtnLB)
	ON_BN_CLICKED(MSTATUS_LANE_LED_STOP_POS_SET_BTN_LB, OnLaneLedStopPosSetBtnLB)
	ON_BN_CLICKED(MSTATUS_LANE_LED_SLOW_POS_GO_BTN_LB, OnLaneLedSlowPosGoBtnLB)
	ON_BN_CLICKED(MSTATUS_LANE_LED_SLOW_POS_SET_BTN_LB, OnLaneLedSlowPosSetBtnLB)	
	ON_BN_CLICKED(MSTATUS_LANE_LED_STOP_POS_GO_BTN_LA2, OnLaneLedStopPosGoBtnLA2)
	ON_BN_CLICKED(MSTATUS_LANE_LED_STOP_POS_SET_BTN_LA2, OnLaneLedStopPosSetBtnLA2)
	ON_BN_CLICKED(MSTATUS_LANE_LED_SLOW_POS_GO_BTN_LA2, OnLaneLedSlowPosGoBtnLA2)
	ON_BN_CLICKED(MSTATUS_LANE_LED_SLOW_POS_SET_BTN_LA2, OnLaneLedSlowPosSetBtnLA2)
	ON_BN_CLICKED(MSTATUS_LANE_LED_STOP_POS_GO_BTN_LB2, OnLaneLedStopPosGoBtnLB2)
	ON_BN_CLICKED(MSTATUS_LANE_LED_STOP_POS_SET_BTN_LB2, OnLaneLedStopPosSetBtnLB2)
	ON_BN_CLICKED(MSTATUS_LANE_LED_SLOW_POS_GO_BTN_LB2, OnLaneLedSlowPosGoBtnLB2)
	ON_BN_CLICKED(MSTATUS_LANE_LED_SLOW_POS_SET_BTN_LB2, OnLaneLedSlowPosSetBtnLB2)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CMotionCtrlPaneStatus message handlers
//-------------------------------------------------------------------------------------//
BOOL CMotionCtrlPaneStatus::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here	
	TMotionParameter &MotionParam = MotionCtrlPtr->GetMotionParameter();
	
	CString str;
	this->m_LEDGreen.LoadBitmap(IDB_LED_MEDIAN_GREEN);
	this->m_LEDRed.LoadBitmap(IDB_LED_MEDIAN_RED);
	this->m_LEDGray.LoadBitmap(IDB_LED_MEDIAN_GRAY);

	this->m_LEDGreenSmall.LoadBitmap(IDB_LED_SMALL_GREEN);
	this->m_LEDGraySmall.LoadBitmap(IDB_LED_SMALL_GRAY);
	this->m_LEDRedSmall.LoadBitmap(IDB_LED_SMALL_RED);


	this->SetDlgItemText(MSTATUS_CUR_POS_EDIT_X, _T("0"));
	this->SetDlgItemText(MSTATUS_CUR_POS_EDIT_Y, _T("0"));
	this->SetDlgItemText(MSTATUS_CUR_POS_EDIT_Z, _T("0"));

	this->SetDlgItemText(MSTATUS_COM_POS_EDIT_X1, _T("0"));
	this->SetDlgItemText(MSTATUS_COM_POS_EDIT_X2, _T("0"));
	this->SetDlgItemText(MSTATUS_COM_POS_EDIT_X3, _T("0"));
	this->SetDlgItemText(MSTATUS_COM_POS_EDIT_Y1, _T("0"));
	this->SetDlgItemText(MSTATUS_COM_POS_EDIT_Y2, _T("0"));
	this->SetDlgItemText(MSTATUS_COM_POS_EDIT_Y3, _T("0"));
	this->SetDlgItemText(MSTATUS_COM_POS_EDIT_Z1, _T("0"));
	this->SetDlgItemText(MSTATUS_COM_POS_EDIT_Z2, _T("0"));
	this->SetDlgItemText(MSTATUS_COM_POS_EDIT_Z3, _T("0"));

	this->SetDlgItemText(MSTATUS_MOVE_POS_EDIT_X, _T("10000"));
	this->SetDlgItemText(MSTATUS_MOVE_POS_EDIT_Y, _T("10000"));
	this->SetDlgItemText(MSTATUS_MOVE_POS_EDIT_Z, _T("100"));	
	
	CWnd::CheckDlgButton(MSTATUS_TIME_CHK_X, TRUE);
	CWnd::CheckDlgButton(MSTATUS_TIME_CHK_Y, TRUE);
	str.Format(_T("%.0f"), MotionParam.m_TestTimeDistanceX);
	CWnd::SetDlgItemText(MSTATUS_TIME_EDIT_X, str);
	str.Format(_T("%.0f"), MotionParam.m_TestTimeDistanceY);
	CWnd::SetDlgItemText(MSTATUS_TIME_EDIT_Y, str);
	str.Format(_T("%.0f"), MotionParam.m_TestTimeDistanceZ);
	CWnd::SetDlgItemText(MSTATUS_TIME_EDIT_Z, str);

	CWnd::SetDlgItemText(MSTATUS_TIME_TEST_EDIT, _T("0"));
	CWnd::SetDlgItemInt(MSTATUS_REPEAT_TIME_EDIT, m_RepeatTime);	

	const bool bShowBLed=false;
	JetAPI::ShowCtrlWnd(this, MSTATUS_LANE_LED_STOP_POS_GO_BTN_LB, bShowBLed);
	JetAPI::ShowCtrlWnd(this, MSTATUS_LANE_LED_STOP_POS_SET_BTN_LB, bShowBLed);
	JetAPI::ShowCtrlWnd(this, MSTATUS_LANE_LED_SLOW_POS_GO_BTN_LB, bShowBLed);
	JetAPI::ShowCtrlWnd(this, MSTATUS_LANE_LED_SLOW_POS_SET_BTN_LB, bShowBLed);
	JetAPI::ShowCtrlWnd(this, MSTATUS_LANE_LED_STOP_POS_GO_BTN_LB2, bShowBLed);
	JetAPI::ShowCtrlWnd(this, MSTATUS_LANE_LED_STOP_POS_SET_BTN_LB2, bShowBLed);
	JetAPI::ShowCtrlWnd(this, MSTATUS_LANE_LED_SLOW_POS_GO_BTN_LB2, bShowBLed);
	JetAPI::ShowCtrlWnd(this, MSTATUS_LANE_LED_SLOW_POS_SET_BTN_LB2, bShowBLed);

	EnableAxisUI();
	SwitchMultiLanguage();	
	LoadUIParamFile();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnDestroy()
{
	
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnOK() 
{
	// TODO: Add extra validation here
	return;
	CDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnCancel() 
{
	// TODO: Add extra cleanup here
	return;
	CDialog::OnCancel();
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if ( TRUE == bShow )
	{
		SwitchMultiLanguage();
		this->ReadMotionStatus();
		this->SetTimer(MOTION_STATUS_TIMER, 250, 0);	
	}
	else
	{	this->KillTimer(MOTION_STATUS_TIMER);	}
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnTimer(UINT_PTR nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	BOOL    bCheck = FALSE;
	switch ( nIDEvent )
	{
	case MOTION_STATUS_TIMER:
		this->UpdateMotionStatus();
		break;
	case MOTION_STATUS_TIMER_REPEAT_12:
		CWnd::KillTimer(nIDEvent);
		if ( ExecRepeatMoveNext12() == false )
		{
			LockUIWnd(false, NULL);
			return; 
		}		
		break;		
	case MOTION_STATUS_TIMER_REPEAT_13:
		CWnd::KillTimer(nIDEvent);
		if ( ExecRepeatMoveNext13() == false )
		{
			LockUIWnd(false, NULL);
			return; 
		}		
		break;		
	case MOTION_STATUS_TIMER_REPEAT_23:
		CWnd::KillTimer(nIDEvent);
		if ( ExecRepeatMoveNext23() == false )
		{
			LockUIWnd(false, NULL);
			return; 
		}		
		break;		
	}
	CDialog::OnTimer(nIDEvent);
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::SwitchMultiLanguage()
{
	CString Section=_T("IDD_MOTION_STATUS_PANE");
	CString WndKey;
	int WndID = 0;
	CString LabelText, NewLabelText;
	CWnd *pWnd = NULL;
	//---------------------------------------------------------------------------------//
	WndID = IDD_MOTION_STATUS_PANE;
	WndKey = _T("IDD_MOTION_STATUS_PANE");
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
	//X axis
	WndID = MSTATUS_AXIS_GROUP_X;
	WndKey = _T("MSTATUS_AXIS_GROUP_X");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MSTATUS_ENABLE_CHK_X;
	WndKey = _T("MSTATUS_ENABLE_CHK_X");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MSTATUS_HOME_BTN_X;
	WndKey = _T("MSTATUS_HOME_BTN_X");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MSTATUS_CHECK_LIMIT_BTN_X;
	WndKey = _T("MSTATUS_CHECK_LIMIT_BTN_X");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MSTATUS_RESET_BTN_X;
	WndKey = _T("MSTATUS_RESET_BTN_X");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MSTATUS_ENABLE_LABEL_X;
	WndKey = _T("MSTATUS_ENABLE_LABEL_X");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MSTATUS_READY_LABEL_X;
	WndKey = _T("MSTATUS_READY_LABEL_X");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MSTATUS_ORG_LABEL_X;
	WndKey = _T("MSTATUS_ORG_LABEL_X");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MSTATUS_PLIMIT_LABEL_X;
	WndKey = _T("MSTATUS_PLIMIT_LABEL_X");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MSTATUS_NLIMIT_LABEL_X;
	WndKey = _T("MSTATUS_NLIMIT_LABEL_X");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	
	WndID = MSTATUS_INPOS_LABEL_X;
	WndKey = _T("MSTATUS_INPOS_LABEL_X");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MSTATUS_EMG_LABEL_X;
	WndKey = _T("MSTATUS_EMG_LABEL_X");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MSTATUS_ALARM_LABEL_X;
	WndKey = _T("MSTATUS_ALARM_LABEL_X");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MSTATUS_STATUS_LABEL_X;
	WndKey = _T("MSTATUS_STATUS_LABEL_X");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//	
	//Y axis
	WndID = MSTATUS_AXIS_GROUP_Y;
	WndKey = _T("MSTATUS_AXIS_GROUP_Y");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MSTATUS_ENABLE_CHK_Y;
	WndKey = _T("MSTATUS_ENABLE_CHK_Y");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MSTATUS_HOME_BTN_Y;
	WndKey = _T("MSTATUS_HOME_BTN_Y");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MSTATUS_CHECK_LIMIT_BTN_Y;
	WndKey = _T("MSTATUS_CHECK_LIMIT_BTN_Y");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MSTATUS_RESET_BTN_Y;
	WndKey = _T("MSTATUS_RESET_BTN_Y");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MSTATUS_ENABLE_LABEL_Y;
	WndKey = _T("MSTATUS_ENABLE_LABEL_Y");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MSTATUS_READY_LABEL_Y;
	WndKey = _T("MSTATUS_READY_LABEL_Y");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MSTATUS_ORG_LABEL_Y;
	WndKey = _T("MSTATUS_ORG_LABEL_Y");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MSTATUS_PLIMIT_LABEL_Y;
	WndKey = _T("MSTATUS_PLIMIT_LABEL_Y");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MSTATUS_NLIMIT_LABEL_Y;
	WndKey = _T("MSTATUS_NLIMIT_LABEL_Y");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	
	WndID = MSTATUS_INPOS_LABEL_Y;
	WndKey = _T("MSTATUS_INPOS_LABEL_Y");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MSTATUS_EMG_LABEL_Y;
	WndKey = _T("MSTATUS_EMG_LABEL_Y");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MSTATUS_ALARM_LABEL_Y;
	WndKey = _T("MSTATUS_ALARM_LABEL_Y");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MSTATUS_STATUS_LABEL_Y;
	WndKey = _T("MSTATUS_STATUS_LABEL_Y");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//
	//Z axis
	WndID = MSTATUS_AXIS_GROUP_Z;
	WndKey = _T("MSTATUS_AXIS_GROUP_Z");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MSTATUS_ENABLE_CHK_Z;
	WndKey = _T("MSTATUS_ENABLE_CHK_Z");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MSTATUS_HOME_BTN_Z;
	WndKey = _T("MSTATUS_HOME_BTN_Z");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MSTATUS_CHECK_LIMIT_BTN_Z;
	WndKey = _T("MSTATUS_CHECK_LIMIT_BTN_Z");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MSTATUS_RESET_BTN_Z;
	WndKey = _T("MSTATUS_RESET_BTN_Z");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MSTATUS_ENABLE_LABEL_Z;
	WndKey = _T("MSTATUS_ENABLE_LABEL_Z");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MSTATUS_READY_LABEL_Z;
	WndKey = _T("MSTATUS_READY_LABEL_Z");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MSTATUS_ORG_LABEL_Z;
	WndKey = _T("MSTATUS_ORG_LABEL_Z");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MSTATUS_PLIMIT_LABEL_Z;
	WndKey = _T("MSTATUS_PLIMIT_LABEL_Z");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MSTATUS_NLIMIT_LABEL_Z;
	WndKey = _T("MSTATUS_NLIMIT_LABEL_Z");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	
	WndID = MSTATUS_INPOS_LABEL_Z;
	WndKey = _T("MSTATUS_INPOS_LABEL_Z");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MSTATUS_EMG_LABEL_Z;
	WndKey = _T("MSTATUS_EMG_LABEL_Z");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MSTATUS_ALARM_LABEL_Z;
	WndKey = _T("MSTATUS_ALARM_LABEL_Z");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MSTATUS_STATUS_LABEL_Z;
	WndKey = _T("MSTATUS_STATUS_LABEL_Z");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//	
	WndID = MSTATUS_CONTROL_GROUP;
	WndKey = _T("MSTATUS_CONTROL_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MSTATUS_CUR_POS_LABEL;
	WndKey = _T("MSTATUS_CUR_POS_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MSTATUS_GO_TO_POS_BTN_1;
	WndKey = _T("MSTATUS_GO_TO_POS_BTN_1");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MSTATUS_GO_TO_POS_BTN_2;
	WndKey = _T("MSTATUS_GO_TO_POS_BTN_2");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MSTATUS_GO_TO_POS_BTN_3;
	WndKey = _T("MSTATUS_GO_TO_POS_BTN_3");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MSTATUS_GET_POS_BTN_1;
	WndKey = _T("MSTATUS_GET_POS_BTN_1");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MSTATUS_GET_POS_BTN_2;
	WndKey = _T("MSTATUS_GET_POS_BTN_2");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MSTATUS_GET_POS_BTN_3;
	WndKey = _T("MSTATUS_GET_POS_BTN_3");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MSTATUS_GET_POS_BTN_X;
	WndKey = _T("MSTATUS_GET_POS_BTN_X");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MSTATUS_GET_POS_BTN_Y;
	WndKey = _T("MSTATUS_GET_POS_BTN_Y");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	
	WndID = MSTATUS_GET_POS_BTN_Z;
	WndKey = _T("MSTATUS_GET_POS_BTN_Z");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MSTATUS_NMOVE_POS_BTN_X;
	WndKey = _T("MSTATUS_NMOVE_POS_BTN_X");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MSTATUS_PMOVE_POS_BTN_X;
	WndKey = _T("MSTATUS_PMOVE_POS_BTN_X");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MSTATUS_NMOVE_POS_BTN_Y;
	WndKey = _T("MSTATUS_NMOVE_POS_BTN_Y");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MSTATUS_PMOVE_POS_BTN_Y;
	WndKey = _T("MSTATUS_PMOVE_POS_BTN_Y");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MSTATUS_NMOVE_POS_BTN_Z;
	WndKey = _T("MSTATUS_NMOVE_POS_BTN_Z");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MSTATUS_PMOVE_POS_BTN_Z;
	WndKey = _T("MSTATUS_PMOVE_POS_BTN_Z");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MSTATUS_JOG_CHK;
	WndKey = _T("MSTATUS_JOG_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MSTATUS_SHOW_RAW_POS_CHK;
	WndKey = _T("MSTATUS_SHOW_RAW_POS_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = MSTATUS_HOME_ALL_BTN;
	WndKey = _T("MSTATUS_HOME_ALL_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MSTATUS_START_POS_SET_BTN;
	WndKey = _T("MSTATUS_START_POS_SET_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MSTATUS_SET_POS_GROUP;
	WndKey = _T("MSTATUS_SET_POS_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MSTATUS_START_POS_GO_BTN;
	WndKey = _T("MSTATUS_START_POS_GO_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	
	WndID = MSTATUS_FOCUS_POS_GO_BTN;
	WndKey = _T("MSTATUS_FOCUS_POS_GO_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MSTATUS_FOCUS_POS_SET_BTN;
	WndKey = _T("MSTATUS_FOCUS_POS_SET_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MSTATUS_PCB_STOP_POS_GO_BTN_LA;
	WndKey = _T("MSTATUS_PCB_STOP_POS_GO_BTN_LA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MSTATUS_PCB_STOP_POS_SET_BTN_LA;
	WndKey = _T("MSTATUS_PCB_STOP_POS_SET_BTN_LA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	
	WndID = MSTATUS_PCB_STOP_POS_GO_BTN_LB;
	WndKey = _T("MSTATUS_PCB_STOP_POS_GO_BTN_LB");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MSTATUS_PCB_STOP_POS_SET_BTN_LB;
	WndKey = _T("MSTATUS_PCB_STOP_POS_SET_BTN_LB");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	
	WndID = MSTATUS_PCB_STOP_POS_GO_BTN_LA2;
	WndKey = _T("MSTATUS_PCB_STOP_POS_GO_BTN_LA2");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MSTATUS_PCB_STOP_POS_SET_BTN_LA2;
	WndKey = _T("MSTATUS_PCB_STOP_POS_SET_BTN_LA2");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MSTATUS_PCB_STOP_POS_GO_BTN_LB2;
	WndKey = _T("MSTATUS_PCB_STOP_POS_GO_BTN_LB2");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MSTATUS_PCB_STOP_POS_SET_BTN_LB2;
	WndKey = _T("MSTATUS_PCB_STOP_POS_SET_BTN_LB2");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MSTATUS_PCB_IN_POS_GO_BTN;
	WndKey = _T("MSTATUS_PCB_IN_POS_GO_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MSTATUS_PCB_IN_POS_SET_BTN;
	WndKey = _T("MSTATUS_PCB_IN_POS_SET_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MSTATUS_ORG_POS_GO_BTN;
	WndKey = _T("MSTATUS_ORG_POS_GO_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MSTATUS_ORG_POS_SET_BTN;
	WndKey = _T("MSTATUS_ORG_POS_SET_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MSTATUS_LEAVE_POS_GO_BTN;
	WndKey = _T("MSTATUS_LEAVE_POS_GO_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MSTATUS_LEAVE_POS_SET_BTN;
	WndKey = _T("MSTATUS_LEAVE_POS_SET_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = MSTATUS_TIME_TEST_BTN;
	WndKey = _T("MSTATUS_TIME_TEST_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MSTATUS_REPEAT_LABEL;
	WndKey = _T("MSTATUS_REPEAT_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//
	WndID = MSTATUS_SAVE_UI_BTN;
	WndKey = _T("MSTATUS_SAVE_UI_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MSTATUS_LOAD_UI_BTN;
	WndKey = _T("MSTATUS_LOAD_UI_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//
	WndID = MSTATUS_LANE_LED_STOP_POS_GO_BTN_LA;
	WndKey = _T("MSTATUS_LANE_LED_STOP_POS_GO_BTN_LA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MSTATUS_LANE_LED_STOP_POS_SET_BTN_LA;
	WndKey = _T("MSTATUS_LANE_LED_STOP_POS_SET_BTN_LA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MSTATUS_LANE_LED_SLOW_POS_GO_BTN_LA;
	WndKey = _T("MSTATUS_LANE_LED_SLOW_POS_GO_BTN_LA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MSTATUS_LANE_LED_SLOW_POS_SET_BTN_LA;
	WndKey = _T("MSTATUS_LANE_LED_SLOW_POS_SET_BTN_LA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MSTATUS_LANE_LED_STOP_POS_GO_BTN_LB;
	WndKey = _T("MSTATUS_LANE_LED_STOP_POS_GO_BTN_LB");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MSTATUS_LANE_LED_STOP_POS_SET_BTN_LB;
	WndKey = _T("MSTATUS_LANE_LED_STOP_POS_SET_BTN_LB");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MSTATUS_LANE_LED_SLOW_POS_GO_BTN_LB;
	WndKey = _T("MSTATUS_LANE_LED_SLOW_POS_GO_BTN_LB");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MSTATUS_LANE_LED_SLOW_POS_SET_BTN_LB;
	WndKey = _T("MSTATUS_LANE_LED_SLOW_POS_SET_BTN_LB");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MSTATUS_LANE_LED_STOP_POS_GO_BTN_LA2;
	WndKey = _T("MSTATUS_LANE_LED_STOP_POS_GO_BTN_LA2");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MSTATUS_LANE_LED_STOP_POS_SET_BTN_LA2;
	WndKey = _T("MSTATUS_LANE_LED_STOP_POS_SET_BTN_LA2");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MSTATUS_LANE_LED_SLOW_POS_GO_BTN_LA2;
	WndKey = _T("MSTATUS_LANE_LED_SLOW_POS_GO_BTN_LA2");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MSTATUS_LANE_LED_SLOW_POS_SET_BTN_LA2;
	WndKey = _T("MSTATUS_LANE_LED_SLOW_POS_SET_BTN_LA2");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MSTATUS_LANE_LED_STOP_POS_GO_BTN_LB2;
	WndKey = _T("MSTATUS_LANE_LED_STOP_POS_GO_BTN_LB2");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MSTATUS_LANE_LED_STOP_POS_SET_BTN_LB2;
	WndKey = _T("MSTATUS_LANE_LED_STOP_POS_SET_BTN_LB2");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MSTATUS_LANE_LED_SLOW_POS_GO_BTN_LB2;
	WndKey = _T("MSTATUS_LANE_LED_SLOW_POS_GO_BTN_LB2");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MSTATUS_LANE_LED_SLOW_POS_SET_BTN_LB2;
	WndKey = _T("MSTATUS_LANE_LED_SLOW_POS_SET_BTN_LB2");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//	
}
//-------------------------------------------------------------------------------------//
CString CMotionCtrlPaneStatus::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_MOTION_STATUS_PANE");		
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
bool CMotionCtrlPaneStatus::ReadMotionStatus()
{
	if ( NULL == MotionCtrlPtr ) { return false; }
	
	int Axis = 0;

	if ( MotionCtrlPtr->GetIsJogMode() == true )
	{	this->CheckDlgButton(MSTATUS_JOG_CHK, BST_CHECKED); }
	else
	{	this->CheckDlgButton(MSTATUS_JOG_CHK, BST_UNCHECKED); }

	//X axis	
	Axis = AXIS_X;
	if ( MotionCtrlPtr->GetIsEnable(Axis) == true )
	{	this->CheckDlgButton(MSTATUS_ENABLE_CHK_X, BST_CHECKED); }
	else
	{	this->CheckDlgButton(MSTATUS_ENABLE_CHK_X, BST_UNCHECKED); }

	//Y axis	
	Axis = AXIS_Y;
	if ( MotionCtrlPtr->GetIsEnable(Axis) == true )
	{	this->CheckDlgButton(MSTATUS_ENABLE_CHK_Y, BST_CHECKED); }
	else
	{	this->CheckDlgButton(MSTATUS_ENABLE_CHK_Y, BST_UNCHECKED); }

	//Z axis	
	Axis = AXIS_Z;
	if ( MotionCtrlPtr->GetIsEnable(Axis) == true )
	{	this->CheckDlgButton(MSTATUS_ENABLE_CHK_Z, BST_CHECKED); }
	else
	{	this->CheckDlgButton(MSTATUS_ENABLE_CHK_Z, BST_UNCHECKED); }

	EnableAxisUI();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMotionCtrlPaneStatus::EnableAxisUI()
{
	BOOL bEnable = FALSE;	
	const bool BypassX=MotionCtrlPtr->CheckAxisBypass(AXIS_X);//確認軸跳過
	if ( true == BypassX )
	{	bEnable = FALSE; }
	else
	{	bEnable = TRUE; }

	JetAPI::EnableCtrlWnd(this, MSTATUS_ENABLE_CHK_X, bEnable);
	JetAPI::EnableCtrlWnd(this, MSTATUS_HOME_BTN_X, bEnable);
	JetAPI::EnableCtrlWnd(this, MSTATUS_CHECK_LIMIT_BTN_X, bEnable);
	JetAPI::EnableCtrlWnd(this, MSTATUS_RESET_BTN_X, bEnable);

	JetAPI::EnableEditWnd(this, MSTATUS_COM_POS_EDIT_X1, bEnable);
	JetAPI::EnableEditWnd(this, MSTATUS_COM_POS_EDIT_X2, bEnable);
	JetAPI::EnableEditWnd(this, MSTATUS_COM_POS_EDIT_X3, bEnable);
	JetAPI::EnableEditWnd(this, MSTATUS_TIME_EDIT_X, bEnable);
	JetAPI::EnableCtrlWnd(this, MSTATUS_TIME_CHK_X, bEnable);
	JetAPI::EnableCtrlWnd(this, MSTATUS_GET_POS_BTN_X, bEnable);
	JetAPI::EnableCtrlWnd(this, MSTATUS_NMOVE_POS_BTN_X, bEnable);
	JetAPI::EnableCtrlWnd(this, MSTATUS_PMOVE_POS_BTN_X, bEnable);
	JetAPI::EnableEditWnd(this, MSTATUS_MOVE_POS_EDIT_X, bEnable);	

	const bool BypassY=MotionCtrlPtr->CheckAxisBypass(AXIS_Y);//確認軸跳過
	if ( true == BypassY )
	{	bEnable = FALSE; }
	else
	{	bEnable = TRUE; }
	
	JetAPI::EnableCtrlWnd(this, MSTATUS_ENABLE_CHK_Y, bEnable);
	JetAPI::EnableCtrlWnd(this, MSTATUS_HOME_BTN_Y, bEnable);
	JetAPI::EnableCtrlWnd(this, MSTATUS_CHECK_LIMIT_BTN_Y, bEnable);
	JetAPI::EnableCtrlWnd(this, MSTATUS_RESET_BTN_Y, bEnable);

	JetAPI::EnableEditWnd(this, MSTATUS_COM_POS_EDIT_Y1, bEnable);
	JetAPI::EnableEditWnd(this, MSTATUS_COM_POS_EDIT_Y2, bEnable);
	JetAPI::EnableEditWnd(this, MSTATUS_COM_POS_EDIT_Y3, bEnable);
	JetAPI::EnableEditWnd(this, MSTATUS_TIME_EDIT_Y, bEnable);
	JetAPI::EnableCtrlWnd(this, MSTATUS_TIME_CHK_Y, bEnable);
	JetAPI::EnableCtrlWnd(this, MSTATUS_GET_POS_BTN_Y, bEnable);
	JetAPI::EnableCtrlWnd(this, MSTATUS_NMOVE_POS_BTN_Y, bEnable);
	JetAPI::EnableCtrlWnd(this, MSTATUS_PMOVE_POS_BTN_Y, bEnable);
	JetAPI::EnableEditWnd(this, MSTATUS_MOVE_POS_EDIT_Y, bEnable);	

	const bool BypassZ=MotionCtrlPtr->CheckAxisBypass(AXIS_Z);//確認軸跳過
	if ( true == BypassZ )
	{	bEnable = FALSE; }
	else
	{	bEnable = TRUE; }
	
	JetAPI::EnableCtrlWnd(this, MSTATUS_ENABLE_CHK_Z, bEnable);
	JetAPI::EnableCtrlWnd(this, MSTATUS_HOME_BTN_Z, bEnable);
	JetAPI::EnableCtrlWnd(this, MSTATUS_CHECK_LIMIT_BTN_Z, bEnable);
	JetAPI::EnableCtrlWnd(this, MSTATUS_RESET_BTN_Z, bEnable);

	JetAPI::EnableEditWnd(this, MSTATUS_COM_POS_EDIT_Z1, bEnable);
	JetAPI::EnableEditWnd(this, MSTATUS_COM_POS_EDIT_Z2, bEnable);
	JetAPI::EnableEditWnd(this, MSTATUS_COM_POS_EDIT_Z3, bEnable);
	JetAPI::EnableEditWnd(this, MSTATUS_TIME_EDIT_Z, bEnable);
	JetAPI::EnableCtrlWnd(this, MSTATUS_TIME_CHK_Z, bEnable);
	JetAPI::EnableCtrlWnd(this, MSTATUS_GET_POS_BTN_Z, bEnable);
	JetAPI::EnableCtrlWnd(this, MSTATUS_NMOVE_POS_BTN_Z, bEnable);
	JetAPI::EnableCtrlWnd(this, MSTATUS_PMOVE_POS_BTN_Z, bEnable);
	JetAPI::EnableEditWnd(this, MSTATUS_MOVE_POS_EDIT_Z, bEnable);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMotionCtrlPaneStatus::UpdateMotionStatus()
{
	if ( NULL == MotionCtrlPtr ) { return false; }
	
	CString str;
	int     Axis = 0;
	double  dPos = 0;	
	double PosX=0, PosY=0, PosZ=0;
	const BOOL bUseRawPos = CWnd::IsDlgButtonChecked(MSTATUS_SHOW_RAW_POS_CHK);	
	if ( FALSE == bUseRawPos )
	{	MotionCtrlPtr->GetCurrentPos(PosX, PosY, PosZ); }
	else
	{	MotionCtrlPtr->GetCurrentRawPos(PosX, PosY, PosZ);	}
	//---------------------------------------------------------------------------------//		
	//X axis	
	Axis = AXIS_X;
	if ( MotionCtrlPtr->GetIsEnable(Axis) == true )
	{	this->m_EnableWndX.SetBitmap(this->m_LEDGreen); }
	else
	{	this->m_EnableWndX.SetBitmap(this->m_LEDGray); }

	if ( MotionCtrlPtr->GetIsReady(Axis) == true )
	{	this->m_ReadyWndX.SetBitmap(this->m_LEDGreen); }
	else
	{	this->m_ReadyWndX.SetBitmap(this->m_LEDGray); }

	if ( MotionCtrlPtr->GetIsORG(Axis) == true )
	{	this->m_OrgWndX.SetBitmap(this->m_LEDGreen); }
	else
	{	this->m_OrgWndX.SetBitmap(this->m_LEDGray); }

	if ( MotionCtrlPtr->GetIsPLimit(Axis) == true )
	{	this->m_PLimitWndX.SetBitmap(this->m_LEDRed); }
	else
	{	this->m_PLimitWndX.SetBitmap(this->m_LEDGray); }

	if ( MotionCtrlPtr->GetIsNLimit(Axis) == true )
	{	this->m_NLimitWndX.SetBitmap(this->m_LEDRed); }
	else
	{	this->m_NLimitWndX.SetBitmap(this->m_LEDGray); }

	if ( MotionCtrlPtr->GetIsInPosition(Axis) == true )
	{	this->m_InposWndX.SetBitmap(this->m_LEDGreen); }
	else
	{	this->m_InposWndX.SetBitmap(this->m_LEDGray); }

	if ( MotionCtrlPtr->GetIsEmergencyOn(Axis) == true )
	{	this->m_EmgWndX.SetBitmap(this->m_LEDRed); }
	else
	{	this->m_EmgWndX.SetBitmap(this->m_LEDGray); }

	if ( MotionCtrlPtr->GetIsAlarm(Axis) == true )
	{	this->m_AlarmWndX.SetBitmap(this->m_LEDRed); }
	else
	{	this->m_AlarmWndX.SetBitmap(this->m_LEDGray); }	

	str = MotionCtrlPtr->GetAxisStatus(Axis);
	this->SetDlgItemText(MSTATUS_STATUS_EDIT_X, str);
	
	str.Format(_T("%.0f"), PosX);
	this->SetDlgItemText(MSTATUS_CUR_POS_EDIT_X, str);
	//---------------------------------------------------------------------------------//		
	//Y axis	
	Axis = AXIS_Y;
	if ( MotionCtrlPtr->GetIsEnable(Axis) == true )
	{	this->m_EnableWndY.SetBitmap(this->m_LEDGreen); }
	else
	{	this->m_EnableWndY.SetBitmap(this->m_LEDGray); }

	if ( MotionCtrlPtr->GetIsReady(Axis) == true )
	{	this->m_ReadyWndY.SetBitmap(this->m_LEDGreen); }
	else
	{	this->m_ReadyWndY.SetBitmap(this->m_LEDGray); }

	if ( MotionCtrlPtr->GetIsORG(Axis) == true )
	{	this->m_OrgWndY.SetBitmap(this->m_LEDGreen); }
	else
	{	this->m_OrgWndY.SetBitmap(this->m_LEDGray); }

	if ( MotionCtrlPtr->GetIsPLimit(Axis) == true )
	{	this->m_PLimitWndY.SetBitmap(this->m_LEDRed); }
	else
	{	this->m_PLimitWndY.SetBitmap(this->m_LEDGray); }

	if ( MotionCtrlPtr->GetIsNLimit(Axis) == true )
	{	this->m_NLimitWndY.SetBitmap(this->m_LEDRed); }
	else
	{	this->m_NLimitWndY.SetBitmap(this->m_LEDGray); }

	if ( MotionCtrlPtr->GetIsInPosition(Axis) == true )
	{	this->m_InposWndY.SetBitmap(this->m_LEDGreen); }
	else
	{	this->m_InposWndY.SetBitmap(this->m_LEDGray); }

	if ( MotionCtrlPtr->GetIsEmergencyOn(Axis) == true )
	{	this->m_EmgWndY.SetBitmap(this->m_LEDRed); }
	else
	{	this->m_EmgWndY.SetBitmap(this->m_LEDGray); }

	if ( MotionCtrlPtr->GetIsAlarm(Axis) == true )
	{	this->m_AlarmWndY.SetBitmap(this->m_LEDRed); }
	else
	{	this->m_AlarmWndY.SetBitmap(this->m_LEDGray); }	

	str = MotionCtrlPtr->GetAxisStatus(Axis);
	this->SetDlgItemText(MSTATUS_STATUS_EDIT_Y, str);
	
	str.Format(_T("%.0f"), PosY);
	this->SetDlgItemText(MSTATUS_CUR_POS_EDIT_Y, str);
	//---------------------------------------------------------------------------------//		
	//Z axis	
	Axis = AXIS_Z;
	if ( MotionCtrlPtr->GetIsEnable(Axis) == true )
	{	this->m_EnableWndZ.SetBitmap(this->m_LEDGreen); }
	else
	{	this->m_EnableWndZ.SetBitmap(this->m_LEDGray); }

	if ( MotionCtrlPtr->GetIsReady(Axis) == true )
	{	this->m_ReadyWndZ.SetBitmap(this->m_LEDGreen); }
	else
	{	this->m_ReadyWndZ.SetBitmap(this->m_LEDGray); }

	if ( MotionCtrlPtr->GetIsORG(Axis) == true )
	{	this->m_OrgWndZ.SetBitmap(this->m_LEDGreen); }
	else
	{	this->m_OrgWndZ.SetBitmap(this->m_LEDGray); }

	if ( MotionCtrlPtr->GetIsPLimit(Axis) == true )
	{	this->m_PLimitWndZ.SetBitmap(this->m_LEDRed); }
	else
	{	this->m_PLimitWndZ.SetBitmap(this->m_LEDGray); }

	if ( MotionCtrlPtr->GetIsNLimit(Axis) == true )
	{	this->m_NLimitWndZ.SetBitmap(this->m_LEDRed); }
	else
	{	this->m_NLimitWndZ.SetBitmap(this->m_LEDGray); }

	if ( MotionCtrlPtr->GetIsInPosition(Axis) == true )
	{	this->m_InposWndZ.SetBitmap(this->m_LEDGreen); }
	else
	{	this->m_InposWndZ.SetBitmap(this->m_LEDGray); }

	if ( MotionCtrlPtr->GetIsEmergencyOn(Axis) == true )
	{	this->m_EmgWndZ.SetBitmap(this->m_LEDRed); }
	else
	{	this->m_EmgWndZ.SetBitmap(this->m_LEDGray); }

	if ( MotionCtrlPtr->GetIsAlarm(Axis) == true )
	{	this->m_AlarmWndZ.SetBitmap(this->m_LEDRed); }
	else
	{	this->m_AlarmWndZ.SetBitmap(this->m_LEDGray); }	

	str = MotionCtrlPtr->GetAxisStatus(Axis);
	this->SetDlgItemText(MSTATUS_STATUS_EDIT_Z, str);
	
	str.Format(_T("%.0f"), PosZ);
	this->SetDlgItemText(MSTATUS_CUR_POS_EDIT_Z, str);
	//---------------------------------------------------------------------------------//		
	return true;
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnGoToPosBtn1() 
{
	// TODO: Add your control notification handler code here
	CString str;
	if ( GetLockUIWnd() == true ) { return; }

	UpdateUIToParam();	
	if ( MotionCtrlPtr->XYZMoveTo(m_CommandPosX1, m_CommandPosY1, m_CommandPosZ1) == false )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString()); }	
	ExecGrabImage();
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnGoToPosBtn2() 
{
	// TODO: Add your control notification handler code here
	CString str;
	if ( GetLockUIWnd() == true ) { return; }

	UpdateUIToParam();
	if ( MotionCtrlPtr->XYZMoveTo(m_CommandPosX2, m_CommandPosY2, m_CommandPosZ2) == false )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString()); }	
	ExecGrabImage();
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnGoToPosBtn3() 
{
	// TODO: Add your control notification handler code here
	CString str;
	if ( GetLockUIWnd() == true ) { return; }

	UpdateUIToParam();
	if ( MotionCtrlPtr->XYZMoveTo(m_CommandPosX3, m_CommandPosY3, m_CommandPosZ3) == false )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString()); }	
	ExecGrabImage();
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnGetPosBtn1() 
{
	// TODO: Add your control notification handler code here
	CString str;
	MotionCtrlPtr->GetCurrentPos(m_CommandPosX1, m_CommandPosY1, m_CommandPosZ1);
	str.Format(_T("%.0f"), m_CommandPosX1);
	this->SetDlgItemText(MSTATUS_COM_POS_EDIT_X1, str);
	str.Format(_T("%.0f"), m_CommandPosY1);
	this->SetDlgItemText(MSTATUS_COM_POS_EDIT_Y1, str);
	str.Format(_T("%.0f"), m_CommandPosZ1);
	this->SetDlgItemText(MSTATUS_COM_POS_EDIT_Z1, str);
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnGetPosBtn2() 
{
	// TODO: Add your control notification handler code here
	CString str;
	MotionCtrlPtr->GetCurrentPos(m_CommandPosX2, m_CommandPosY2, m_CommandPosZ2);	
	str.Format(_T("%.0f"), m_CommandPosX2);
	this->SetDlgItemText(MSTATUS_COM_POS_EDIT_X2, str);
	str.Format(_T("%.0f"), m_CommandPosY2);
	this->SetDlgItemText(MSTATUS_COM_POS_EDIT_Y2, str);
	str.Format(_T("%.0f"), m_CommandPosZ2);
	this->SetDlgItemText(MSTATUS_COM_POS_EDIT_Z2, str);
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnGetPosBtn3() 
{
	// TODO: Add your control notification handler code here
	CString str;
	MotionCtrlPtr->GetCurrentPos(m_CommandPosX3, m_CommandPosY3, m_CommandPosZ3);
	str.Format(_T("%.0f"), m_CommandPosX3);
	this->SetDlgItemText(MSTATUS_COM_POS_EDIT_X3, str);
	str.Format(_T("%.0f"), m_CommandPosY3);
	this->SetDlgItemText(MSTATUS_COM_POS_EDIT_Y3, str);
	str.Format(_T("%.0f"), m_CommandPosZ3);
	this->SetDlgItemText(MSTATUS_COM_POS_EDIT_Z3, str);
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnNMovePosBtnX() 
{
	// TODO: Add your control notification handler code here
	CString str;
	if ( GetLockUIWnd() == true ) { return; }

	double Offset=0;
	double PosX=0, PosZ=0, PosY=0;

	this->GetDlgItemText(MSTATUS_MOVE_POS_EDIT_X, str);
	Offset = ::_tcstod(str, NULL);
	
	MotionCtrlPtr->GetCurrentPos(PosX, PosY, PosZ);
	PosX -= Offset;
	if ( MotionCtrlPtr->XYZMoveTo(PosX, PosY, PosZ) == false )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString()); }
	ExecGrabImage();
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnPMovePosBtnX() 
{
	// TODO: Add your control notification handler code here
	CString str;
	if ( GetLockUIWnd() == true ) { return; }

	double Offset=0;
	double PosX=0, PosZ=0, PosY=0;
	
	this->GetDlgItemText(MSTATUS_MOVE_POS_EDIT_X, str);
	Offset = ::_tcstod(str, NULL);
	
	MotionCtrlPtr->GetCurrentPos(PosX, PosY, PosZ);
	PosX += Offset;
	if ( MotionCtrlPtr->XYZMoveTo(PosX, PosY, PosZ) == false )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString()); }
	ExecGrabImage();
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnNMovePosBtnY() 
{
	// TODO: Add your control notification handler code here
	CString str;
	if ( GetLockUIWnd() == true ) { return; }

	double Offset=0;
	double PosX=0, PosZ=0, PosY=0;
	
	this->GetDlgItemText(MSTATUS_MOVE_POS_EDIT_Y, str);
	Offset = ::_tcstod(str, NULL);
	
	MotionCtrlPtr->GetCurrentPos(PosX, PosY, PosZ);
	PosY -= Offset;
	if ( MotionCtrlPtr->XYZMoveTo(PosX, PosY, PosZ) == false )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString()); }
	ExecGrabImage();
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnPMovePosBtnY() 
{
	// TODO: Add your control notification handler code here
	CString str;
	if ( GetLockUIWnd() == true ) { return; }

	double Offset=0;
	double PosX=0, PosZ=0, PosY=0;
	
	this->GetDlgItemText(MSTATUS_MOVE_POS_EDIT_Y, str);
	Offset = ::_tcstod(str, NULL);
	
	MotionCtrlPtr->GetCurrentPos(PosX, PosY, PosZ);
	PosY += Offset;
	if ( MotionCtrlPtr->XYZMoveTo(PosX, PosY, PosZ) == false )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString()); }
	ExecGrabImage();
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnNMovePosBtnZ() 
{
	// TODO: Add your control notification handler code here
	CString str;
	if ( GetLockUIWnd() == true ) { return; }

	double Offset=0;
	double PosX=0, PosZ=0, PosY=0;
	
	this->GetDlgItemText(MSTATUS_MOVE_POS_EDIT_Z, str);
	Offset = ::_tcstod(str, NULL);
	
	MotionCtrlPtr->GetCurrentPos(PosX, PosY, PosZ);
	PosZ -= Offset;
	if ( MotionCtrlPtr->XYZMoveTo(PosX, PosY, PosZ) == false )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString()); }
	ExecGrabImage();
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnPMovePosBtnZ() 
{
	// TODO: Add your control notification handler code here
	CString str;
	if ( GetLockUIWnd() == true ) { return; }

	double Offset=0;
	double PosX=0, PosZ=0, PosY=0;
	
	this->GetDlgItemText(MSTATUS_MOVE_POS_EDIT_Z, str);
	Offset = ::_tcstod(str, NULL);
	
	MotionCtrlPtr->GetCurrentPos(PosX, PosY, PosZ);
	PosZ += Offset;
	if ( MotionCtrlPtr->XYZMoveTo(PosX, PosY, PosZ) == false )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString()); }
	ExecGrabImage();
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnGetPosBtnX() 
{
	// TODO: Add your control notification handler code here
	CString str;
	double PosX=0, PosY=0, PosZ=0;
	MotionCtrlPtr->GetCurrentPos(PosX, PosY, PosZ);		
	m_CommandPosX3 = m_CommandPosX2 = m_CommandPosX1 = PosX;

	str.Format(_T("%.0f"), m_CommandPosX1);
	this->SetDlgItemText(MSTATUS_COM_POS_EDIT_X1, str);
	str.Format(_T("%.0f"), m_CommandPosX2);
	this->SetDlgItemText(MSTATUS_COM_POS_EDIT_X2, str);
	str.Format(_T("%.0f"), m_CommandPosX3);
	this->SetDlgItemText(MSTATUS_COM_POS_EDIT_X3, str);
	
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnGetPosBtnY() 
{
	// TODO: Add your control notification handler code here
	CString str;
	double PosX=0, PosY=0, PosZ=0;
	MotionCtrlPtr->GetCurrentPos(PosX, PosY, PosZ);		
	m_CommandPosY3 = m_CommandPosY2 = m_CommandPosY1 = PosY;

	str.Format(_T("%.0f"), m_CommandPosY1);
	this->SetDlgItemText(MSTATUS_COM_POS_EDIT_Y1, str);
	str.Format(_T("%.0f"), m_CommandPosY2);
	this->SetDlgItemText(MSTATUS_COM_POS_EDIT_Y2, str);
	str.Format(_T("%.0f"), m_CommandPosY3);
	this->SetDlgItemText(MSTATUS_COM_POS_EDIT_Y3, str);
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnGetPosBtnZ() 
{
	// TODO: Add your control notification handler code here
	CString str;
	double PosX=0, PosY=0, PosZ=0;
	MotionCtrlPtr->GetCurrentPos(PosX, PosY, PosZ);	
	m_CommandPosZ3 = m_CommandPosZ2 = m_CommandPosZ1 = PosZ;

	str.Format(_T("%.0f"), m_CommandPosZ1);
	this->SetDlgItemText(MSTATUS_COM_POS_EDIT_Z1, str);
	str.Format(_T("%.0f"), m_CommandPosZ2);
	this->SetDlgItemText(MSTATUS_COM_POS_EDIT_Z2, str);
	str.Format(_T("%.0f"), m_CommandPosZ3);
	this->SetDlgItemText(MSTATUS_COM_POS_EDIT_Z3, str);
}
//-------------------------------------------------------------------------------------//
BOOL CMotionCtrlPaneStatus::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ( MotionCtrlPtr->ExecJogMSG(pMsg->message, pMsg->wParam, pMsg->lParam) == true ) 
	{	return TRUE; }
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
void CMotionCtrlPaneStatus::OnJogChk() 
{
	// TODO: Add your control notification handler code here
	if ( this->IsDlgButtonChecked(MSTATUS_JOG_CHK) == TRUE )
	{	MotionCtrlPtr->SetIsJogMode(true);	}
	else
	{	MotionCtrlPtr->SetIsJogMode(false);	}
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnEnableChkX() 
{
	// TODO: Add your control notification handler code here
	bool IsOK = true;
	UINT CtrlID = MSTATUS_ENABLE_CHK_X;
	BOOL bCheck = this->IsDlgButtonChecked(CtrlID);
	if ( TRUE == bCheck )
	{	IsOK = MotionCtrlPtr->Enable(AXIS_X); }
	else
	{	IsOK = MotionCtrlPtr->Disable(AXIS_X); }
	if ( false == IsOK )
	{	
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());	
		if ( TRUE == bCheck )
		{	this->CheckDlgButton(CtrlID, FALSE); }
		else
		{	this->CheckDlgButton(CtrlID, TRUE); }
	}
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnEnableChkY() 
{
	// TODO: Add your control notification handler code here
	bool IsOK = true;
	UINT CtrlID = MSTATUS_ENABLE_CHK_Y;
	BOOL bCheck = this->IsDlgButtonChecked(CtrlID);
	if ( TRUE == bCheck )
	{	IsOK = MotionCtrlPtr->Enable(AXIS_Y); }
	else
	{	IsOK = MotionCtrlPtr->Disable(AXIS_Y); }
	if ( false == IsOK )
	{	
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());	
		if ( TRUE == bCheck )
		{	this->CheckDlgButton(CtrlID, FALSE); }
		else
		{	this->CheckDlgButton(CtrlID, TRUE); }
	}
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnEnableChkZ() 
{
	// TODO: Add your control notification handler code here
	bool IsOK = true;
	UINT CtrlID = MSTATUS_ENABLE_CHK_Z;
	BOOL bCheck = this->IsDlgButtonChecked(CtrlID);
	if ( TRUE == bCheck )
	{	IsOK = MotionCtrlPtr->Enable(AXIS_Z); }
	else
	{	IsOK = MotionCtrlPtr->Disable(AXIS_Z); }
	if ( false == IsOK )
	{	
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());	
		if ( TRUE == bCheck )
		{	this->CheckDlgButton(CtrlID, FALSE); }
		else
		{	this->CheckDlgButton(CtrlID, TRUE); }
	}
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnHomeBtnX() 
{
	// TODO: Add your control notification handler code here
	bool IsOK = true;
	IsOK = MotionCtrlPtr->Home(AXIS_X);
	if ( false == IsOK )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());	}
	else
	{	JetAPI::ShowMessageBox(AOIDataDefine.GetFinishText()); }
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnHomeBtnY() 
{
	// TODO: Add your control notification handler code here
	bool IsOK = true;
	IsOK = MotionCtrlPtr->Home(AXIS_Y);
	if ( false == IsOK )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());	}
	else
	{	JetAPI::ShowMessageBox(AOIDataDefine.GetFinishText()); }
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnHomeBtnZ() 
{
	// TODO: Add your control notification handler code here
	bool IsOK = true;
	IsOK = MotionCtrlPtr->Home(AXIS_Z);
	if ( false == IsOK )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());	}
	else
	{	JetAPI::ShowMessageBox(AOIDataDefine.GetFinishText()); }
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnResetBtnX() 
{
	// TODO: Add your control notification handler code here
	bool IsOK = true;
	IsOK = MotionCtrlPtr->DoFaultAck(AXIS_X);
	if ( false == IsOK )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());	}	
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnResetBtnY() 
{
	// TODO: Add your control notification handler code here
	bool IsOK = true;
	IsOK = MotionCtrlPtr->DoFaultAck(AXIS_Y);
	if ( false == IsOK )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());	}
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnResetBtnZ() 
{
	// TODO: Add your control notification handler code here
	bool IsOK = true;
	IsOK = MotionCtrlPtr->DoFaultAck(AXIS_Z);
	if ( false == IsOK )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());	}
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnCheckLimitBtnX() 
{
	// TODO: Add your control notification handler code here
	CString str;
	bool IsOK = true;	
	str = _T("Do you want to auto-search X-axis limit?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO )
	{	return; }
	IsOK = MotionCtrlPtr->ExecLimit(AXIS_X);
	if ( false == IsOK )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());	}
	else
	{
		if ( MotionCtrlPtr->SaveMotionParamInternal() == false )
		{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString()); }	
		JetAPI::ShowMessageBox(AOIDataDefine.GetFinishText());
	}
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnCheckLimitBtnY() 
{
	// TODO: Add your control notification handler code here
	CString str;
	bool IsOK = true;
	str = _T("Do you want to auto-search Y-axis limit?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO )
	{	return; }
	IsOK = MotionCtrlPtr->ExecLimit(AXIS_Y);
	if ( false == IsOK )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());	}
	else
	{
		if ( MotionCtrlPtr->SaveMotionParamInternal() == false )
		{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString()); }	
		JetAPI::ShowMessageBox(AOIDataDefine.GetFinishText());
	}
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnCheckLimitBtnZ() 
{
	// TODO: Add your control notification handler code here
	CString str;
	bool IsOK = true;
	str = _T("Do you want to auto-search Z-axis limit?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO )
	{	return; }
	IsOK = MotionCtrlPtr->ExecLimit(AXIS_Z);
	if ( false == IsOK )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());	}
	else
	{
		if ( MotionCtrlPtr->SaveMotionParamInternal() == false )
		{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString()); }	
		JetAPI::ShowMessageBox(AOIDataDefine.GetFinishText());
	}
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnStartPosGoBtn() 
{
	// TODO: Add your control notification handler code here
	if ( GetLockUIWnd() == true ) { return; }
	bool IsOK = true;
	double PosX = MotionCtrlPtr->GetMotionParameter().m_StageStartPosX;
	double PosY = MotionCtrlPtr->GetMotionParameter().m_StageStartPosY;
	IsOK = MotionCtrlPtr->XYMoveTo(PosX, PosY);
	if ( false == IsOK )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());	}
	ExecGrabImage();
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnStartPosSetBtn() 
{
	// TODO: Add your control notification handler code here
	CString str;
	str = _T("Do you want to set XY start position?");
	str = LoadMultiLanguageString(str, str);	
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) != IDYES )
	{	return; }

	bool IsOK = true;
	double PosX = 0;
	double PosY = 0;	
	double PosZ = 0;	
	IsOK = MotionCtrlPtr->GetCurrentPos(PosX, PosY, PosZ);
	if ( false == IsOK )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());	}
	MotionCtrlPtr->GetMotionParameter().m_StageStartPosX = PosX;
	MotionCtrlPtr->GetMotionParameter().m_StageStartPosY = PosY;
	MotionCtrlPtr->SaveMotionParameter();
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnFocusPosGoBtn() 
{
	// TODO: Add your control notification handler code here
	bool IsOK = true;
	double PosZ = MotionCtrlPtr->GetMotionParameter().m_StageStartPosZ;	
	IsOK = MotionCtrlPtr->MoveTo(AXIS_Z, PosZ, MOTION_MOVING_NORMAL);
	if ( false == IsOK )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());	}
	ExecGrabImage();
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnFocusPosSetBtn() 
{
	// TODO: Add your control notification handler code here
	CString str, str2;
	LANE_ID LaneID = AOIDataCollect.GetActiveLaneID();
	CString strLane = AOIDataDefine.GetLaneIDText(LaneID);
	str = _T("Do you want to set Z focus position?");
	str = LoadMultiLanguageString(str, str);
	str2.Format(_T("%s [%s]"), str, strLane);
	if ( JetAPI::ShowMessageBox(str2, MB_YESNO) != IDYES )
	{	return; }

	bool IsOK = true;
	double PosX = 0;
	double PosY = 0;	
	double PosZ = 0;
	IsOK = MotionCtrlPtr->GetCurrentPos(PosX, PosY, PosZ);
	if ( false == IsOK )
	{	
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());	
		return;
	}	
	MotionCtrlPtr->SetFocusPosZ(PosZ, LaneID);
	MotionCtrlPtr->SaveMotionParameter();
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnHomeAllBtn() 
{
	// TODO: Add your control notification handler code here
	if ( GetLockUIWnd() == true ) { return; }

	CString str;
	const bool    bAutoReset=false;
	const bool    bChkStartLight=true;	
	if ( PlcCtrlPtr->CheckPLCReady(bChkStartLight, bAutoReset) == false )
	{
		str = PlcCtrlPtr->GetPLCErrorString();
		JetAPI::ShowMessageBox(str);
		return ;
	}
	const bool bPcbInsideLA=PlcCtrlPtr->CheckPCBInside_LA();
	const bool bPcbInsideLB=PlcCtrlPtr->CheckPCBInside_LB();
	if ( true==bPcbInsideLA || true==bPcbInsideLB )
	{	
		str = _T("PCB Inside, Do you want to Home XYZ?");	
		str = LoadMultiLanguageString(str, str);
		if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO )
		{	return; }
	}
	if ( MotionCtrlPtr->ExecHomeAll(true) == false )
	{
		str = MotionCtrlPtr->GetErrorString();
		JetAPI::ShowMessageBox(str);	
		AOIDataCollect.ShowMessage(0, MSG_MB_OK, MSG_MB_ICONINFORMATION, MSG_MB_DEFBUTTON1, str, true, false);
		return;
	}
	JetAPI::ShowMessageBox(AOIDataDefine.GetFinishText());
	ExecGrabImage();
	ReadMotionStatus();//v1.01.04.061
}
//-------------------------------------------------------------------------------------//
bool CMotionCtrlPaneStatus::ExecGrabImage()
{
	MotionCtrlPtr->WaitForMotionStop();
	AOIDataCollect.PostCallbackWndMessage(MSG_CAMERA_REGRAB_IMAGE, TRUE, NULL);	
	return true;
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnORGPosGoBtn() 
{
	// TODO: Add your control notification handler code here
	if ( GetLockUIWnd() == true ) { return; }

	if ( MotionCtrlPtr->XYZMoveTo(0, 0, 0) == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());	
		return;
	}
	ExecGrabImage();
}
//-------------------------------------------------------------------------------------//
bool CMotionCtrlPaneStatus::ExecRepeatMoveNext12()
{	
	UINT CtrlID = MSTATUS_REPEAT_12_CHK;
	BOOL bCheck = CWnd::IsDlgButtonChecked(CtrlID);
	if ( FALSE == bCheck ) 
	{	return false;	}
	if ( MotionCtrlPtr->WaitForMotionStop() == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		CWnd::CheckDlgButton(CtrlID, FALSE);			
		return false;
	}
	if ( MOTION_REPEAT_TO_1 == m_RepeatToMode )
	{
		m_RepeatToMode = MOTION_REPEAT_TO_2;
		MotionCtrlPtr->XYZMoveTo(m_CommandPosX2, m_CommandPosY2, m_CommandPosZ2);
	}
	else
	{
		m_RepeatToMode = MOTION_REPEAT_TO_1;
		MotionCtrlPtr->XYZMoveTo(m_CommandPosX1, m_CommandPosY1, m_CommandPosZ1);
	}
	SetTimer(MOTION_STATUS_TIMER_REPEAT_12, m_RepeatTime, 0);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMotionCtrlPaneStatus::ExecRepeatMoveNext13()
{
	UINT CtrlID = MSTATUS_REPEAT_13_CHK;
	BOOL bCheck = CWnd::IsDlgButtonChecked(CtrlID);
	if ( FALSE == bCheck ) 
	{	return false;	}
	if ( MotionCtrlPtr->WaitForMotionStop() == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		CWnd::CheckDlgButton(CtrlID, FALSE);			
		return false;
	}
	if ( MOTION_REPEAT_TO_1 == m_RepeatToMode )
	{
		m_RepeatToMode = MOTION_REPEAT_TO_3;
		MotionCtrlPtr->XYZMoveTo(m_CommandPosX3, m_CommandPosY3, m_CommandPosZ3);
	}
	else
	{
		m_RepeatToMode = MOTION_REPEAT_TO_1;
		MotionCtrlPtr->XYZMoveTo(m_CommandPosX1, m_CommandPosY1, m_CommandPosZ1);
	}
	SetTimer(MOTION_STATUS_TIMER_REPEAT_13, m_RepeatTime, 0);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMotionCtrlPaneStatus::ExecRepeatMoveNext23()
{
	UINT CtrlID = MSTATUS_REPEAT_23_CHK;
	BOOL bCheck = CWnd::IsDlgButtonChecked(CtrlID);
	if ( FALSE == bCheck ) 
	{	return false; }
	if ( MotionCtrlPtr->WaitForMotionStop() == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		CWnd::CheckDlgButton(CtrlID, FALSE);			
		return false;
	}
	if ( MOTION_REPEAT_TO_2 == m_RepeatToMode )
	{
		m_RepeatToMode = MOTION_REPEAT_TO_3;
		MotionCtrlPtr->XYZMoveTo(m_CommandPosX3, m_CommandPosY3, m_CommandPosZ3);
	}
	else
	{
		m_RepeatToMode = MOTION_REPEAT_TO_2;
		MotionCtrlPtr->XYZMoveTo(m_CommandPosX2, m_CommandPosY2, m_CommandPosZ2);
		
	}
	SetTimer(MOTION_STATUS_TIMER_REPEAT_23, m_RepeatTime, 0);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMotionCtrlPaneStatus::UpdateUIToParam()
{
	CString str;
	this->GetDlgItemText(MSTATUS_COM_POS_EDIT_X1, str);
	m_CommandPosX1 = ::_tcstod(str, NULL);
	this->GetDlgItemText(MSTATUS_COM_POS_EDIT_Y1, str);
	m_CommandPosY1 = ::_tcstod(str, NULL);
	this->GetDlgItemText(MSTATUS_COM_POS_EDIT_Z1, str);
	m_CommandPosZ1 = ::_tcstod(str, NULL);

	this->GetDlgItemText(MSTATUS_COM_POS_EDIT_X2, str);
	m_CommandPosX2 = ::_tcstod(str, NULL);
	this->GetDlgItemText(MSTATUS_COM_POS_EDIT_Y2, str);
	m_CommandPosY2 = ::_tcstod(str, NULL);
	this->GetDlgItemText(MSTATUS_COM_POS_EDIT_Z2, str);
	m_CommandPosZ2 = ::_tcstod(str, NULL);

	this->GetDlgItemText(MSTATUS_COM_POS_EDIT_X3, str);
	m_CommandPosX3 = ::_tcstod(str, NULL);
	this->GetDlgItemText(MSTATUS_COM_POS_EDIT_Y3, str);
	m_CommandPosY3 = ::_tcstod(str, NULL);
	this->GetDlgItemText(MSTATUS_COM_POS_EDIT_Z3, str);
	m_CommandPosZ3 = ::_tcstod(str, NULL);

	m_RepeatTime = CWnd::GetDlgItemInt(MSTATUS_REPEAT_TIME_EDIT);
	if ( m_RepeatTime < 10 ) { m_RepeatTime = 10; }
	return true;
}
//-------------------------------------------------------------------------------------//
CString CMotionCtrlPaneStatus::GetUISectionName() const
{
	CString Section=_T("IDD_MOTION_STATUS_PANE");	
#ifdef TB_SYSTEM_ONLY_BOT
	Section=_T("IDD_MOTION_STATUS_PANE_BOT");
#endif//TB_SYSTEM_ONLY_BOT
	return Section;
}
//-------------------------------------------------------------------------------------//
bool CMotionCtrlPaneStatus::SaveUIParamFile()
{
	size_t  i = 0;	
	UINT    WndID = 0;
	CString WndKey;
	CString Default;	
	CString Section=GetUISectionName();	
	
	WndID = MSTATUS_COM_POS_EDIT_X1;
	WndKey = _T("MSTATUS_COM_POS_EDIT_X1");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.SaveWndUIParam(Section, WndKey, Default);	

	WndID = MSTATUS_COM_POS_EDIT_Y1;
	WndKey = _T("MSTATUS_COM_POS_EDIT_Y1");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.SaveWndUIParam(Section, WndKey, Default);	

	WndID = MSTATUS_COM_POS_EDIT_Z1;
	WndKey = _T("MSTATUS_COM_POS_EDIT_Z1");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.SaveWndUIParam(Section, WndKey, Default);	

	WndID = MSTATUS_COM_POS_EDIT_X2;
	WndKey = _T("MSTATUS_COM_POS_EDIT_X2");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.SaveWndUIParam(Section, WndKey, Default);	

	WndID = MSTATUS_COM_POS_EDIT_Y2;
	WndKey = _T("MSTATUS_COM_POS_EDIT_Y2");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.SaveWndUIParam(Section, WndKey, Default);	

	WndID = MSTATUS_COM_POS_EDIT_Z2;
	WndKey = _T("MSTATUS_COM_POS_EDIT_Z2");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.SaveWndUIParam(Section, WndKey, Default);	

	WndID = MSTATUS_COM_POS_EDIT_X3;
	WndKey = _T("MSTATUS_COM_POS_EDIT_X3");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.SaveWndUIParam(Section, WndKey, Default);	

	WndID = MSTATUS_COM_POS_EDIT_Y3;
	WndKey = _T("MSTATUS_COM_POS_EDIT_Y3");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.SaveWndUIParam(Section, WndKey, Default);	

	WndID = MSTATUS_COM_POS_EDIT_Z3;
	WndKey = _T("MSTATUS_COM_POS_EDIT_Z3");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.SaveWndUIParam(Section, WndKey, Default);	
	
	WndID = MSTATUS_MOVE_POS_EDIT_X;
	WndKey = _T("MSTATUS_MOVE_POS_EDIT_X");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.SaveWndUIParam(Section, WndKey, Default);	

	WndID = MSTATUS_MOVE_POS_EDIT_Y;
	WndKey = _T("MSTATUS_MOVE_POS_EDIT_Y");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.SaveWndUIParam(Section, WndKey, Default);	

	WndID = MSTATUS_MOVE_POS_EDIT_Z;
	WndKey = _T("MSTATUS_MOVE_POS_EDIT_Z");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.SaveWndUIParam(Section, WndKey, Default);	

	return true;
}
//-------------------------------------------------------------------------------------//
bool CMotionCtrlPaneStatus::LoadUIParamFile()
{
	size_t  i = 0;	
	UINT    WndID = 0;
	CString WndKey;
	CString String;
	CString Default;
	CString Section=GetUISectionName();	
	//---------------------------------------------------------------------------------//
	WndID = MSTATUS_COM_POS_EDIT_X1;
	WndKey = _T("MSTATUS_COM_POS_EDIT_X1");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.LoadWndUIParam(Section, WndKey, Default, String);	
	CWnd::SetDlgItemText(WndID, String);

	WndID = MSTATUS_COM_POS_EDIT_Y1;
	WndKey = _T("MSTATUS_COM_POS_EDIT_Y1");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.LoadWndUIParam(Section, WndKey, Default, String);	
	CWnd::SetDlgItemText(WndID, String);

	WndID = MSTATUS_COM_POS_EDIT_Z1;
	WndKey = _T("MSTATUS_COM_POS_EDIT_Z1");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.LoadWndUIParam(Section, WndKey, Default, String);	
	CWnd::SetDlgItemText(WndID, String);

	WndID = MSTATUS_COM_POS_EDIT_X2;
	WndKey = _T("MSTATUS_COM_POS_EDIT_X2");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.LoadWndUIParam(Section, WndKey, Default, String);	
	CWnd::SetDlgItemText(WndID, String);

	WndID = MSTATUS_COM_POS_EDIT_Y2;
	WndKey = _T("MSTATUS_COM_POS_EDIT_Y2");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.LoadWndUIParam(Section, WndKey, Default, String);	
	CWnd::SetDlgItemText(WndID, String);

	WndID = MSTATUS_COM_POS_EDIT_Z2;
	WndKey = _T("MSTATUS_COM_POS_EDIT_Z2");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.LoadWndUIParam(Section, WndKey, Default, String);	
	CWnd::SetDlgItemText(WndID, String);

	WndID = MSTATUS_COM_POS_EDIT_X3;
	WndKey = _T("MSTATUS_COM_POS_EDIT_X3");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.LoadWndUIParam(Section, WndKey, Default, String);	
	CWnd::SetDlgItemText(WndID, String);

	WndID = MSTATUS_COM_POS_EDIT_Y3;
	WndKey = _T("MSTATUS_COM_POS_EDIT_Y3");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.LoadWndUIParam(Section, WndKey, Default, String);	
	CWnd::SetDlgItemText(WndID, String);

	WndID = MSTATUS_COM_POS_EDIT_Z3;
	WndKey = _T("MSTATUS_COM_POS_EDIT_Z3");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.LoadWndUIParam(Section, WndKey, Default, String);	
	CWnd::SetDlgItemText(WndID, String);
	//---------------------------------------------------------------------------------//
	WndID = MSTATUS_MOVE_POS_EDIT_X;
	WndKey = _T("MSTATUS_MOVE_POS_EDIT_X");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.LoadWndUIParam(Section, WndKey, Default, String);	
	CWnd::SetDlgItemText(WndID, String);

	WndID = MSTATUS_MOVE_POS_EDIT_Y;
	WndKey = _T("MSTATUS_MOVE_POS_EDIT_Y");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.LoadWndUIParam(Section, WndKey, Default, String);	
	CWnd::SetDlgItemText(WndID, String);

	WndID = MSTATUS_MOVE_POS_EDIT_Z;
	WndKey = _T("MSTATUS_MOVE_POS_EDIT_Z");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.LoadWndUIParam(Section, WndKey, Default, String);	
	CWnd::SetDlgItemText(WndID, String);
	//---------------------------------------------------------------------------------//
	return true;
}
//-------------------------------------------------------------------------------------//	
void CMotionCtrlPaneStatus::LockUIWnd(bool bLock, UINT CmdID)//鎖住視窗
{	
	UINT CtrlID=0;
	BOOL bEnable=TRUE;

	if ( true == bLock ) { bEnable = FALSE; }
	else { bEnable = TRUE; }
	EnableUIWnd(bEnable, CmdID);
	AOIDataCollect.SetIsLockUIWnd(bLock);
}
//-------------------------------------------------------------------------------------//
bool CMotionCtrlPaneStatus::GetLockUIWnd() const//取得是否鎖住UIWnd
{
	return AOIDataCollect.GetIsLockUIWnd();
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::EnableUIWnd(BOOL bEnableWnd, UINT CmdID)//鎖住視窗
{
	UINT CtrlID = 0;
	BOOL bEnable = bEnableWnd;
	BOOL bEnable2 = bEnableWnd;

	if ( TRUE == bEnable ) 
	{	CmdID = NULL; }

	if ( MotionCtrlPtr->CheckAxisBypass(AXIS_X) == true )
	{	bEnable2 = FALSE; }
	else
	{	bEnable2 = bEnable; }
	CtrlID = MSTATUS_ENABLE_CHK_X;
	if ( CtrlID != CmdID )
	{	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2); }
	CtrlID = MSTATUS_HOME_BTN_X;
	if ( CtrlID != CmdID )
	{	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2); }
	CtrlID = MSTATUS_CHECK_LIMIT_BTN_X;
	if ( CtrlID != CmdID )
	{	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2); }
	CtrlID = MSTATUS_RESET_BTN_X;
	if ( CtrlID != CmdID )
	{	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2); }
	CtrlID = MSTATUS_NMOVE_POS_BTN_X;
	if ( CtrlID != CmdID )
	{	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2); }
	CtrlID = MSTATUS_PMOVE_POS_BTN_X;
	if ( CtrlID != CmdID )
	{	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2); }

	if ( MotionCtrlPtr->CheckAxisBypass(AXIS_Y) == true )
	{	bEnable2 = FALSE; }
	else
	{	bEnable2 = bEnable; }
	CtrlID = MSTATUS_ENABLE_CHK_Y;
	if ( CtrlID != CmdID )
	{	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2); }
	CtrlID = MSTATUS_HOME_BTN_Y;
	if ( CtrlID != CmdID )
	{	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2); }
	CtrlID = MSTATUS_CHECK_LIMIT_BTN_Y;
	if ( CtrlID != CmdID )
	{	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2); }
	CtrlID = MSTATUS_RESET_BTN_Y;
	if ( CtrlID != CmdID )
	{	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2); }
	CtrlID = MSTATUS_NMOVE_POS_BTN_Y;
	if ( CtrlID != CmdID )
	{	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2); }
	CtrlID = MSTATUS_PMOVE_POS_BTN_Y;
	if ( CtrlID != CmdID )
	{	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2); }
	
	if ( MotionCtrlPtr->CheckAxisBypass(AXIS_Z) == true )
	{	bEnable2 = FALSE; }
	else
	{	bEnable2 = bEnable; }
	CtrlID = MSTATUS_ENABLE_CHK_Z;
	if ( CtrlID != CmdID )
	{	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2); }
	CtrlID = MSTATUS_HOME_BTN_Z;
	if ( CtrlID != CmdID )
	{	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2); }
	CtrlID = MSTATUS_CHECK_LIMIT_BTN_Z;
	if ( CtrlID != CmdID )
	{	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2); }
	CtrlID = MSTATUS_RESET_BTN_Z;
	if ( CtrlID != CmdID )
	{	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2); }
	CtrlID = MSTATUS_NMOVE_POS_BTN_Z;
	if ( CtrlID != CmdID )
	{	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2); }
	CtrlID = MSTATUS_PMOVE_POS_BTN_Z;
	if ( CtrlID != CmdID )
	{	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2); }	

	bEnable2 = bEnable;
	CtrlID = MSTATUS_GO_TO_POS_BTN_1;
	if ( CtrlID != CmdID )
	{	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2); }	
	CtrlID = MSTATUS_GO_TO_POS_BTN_2;
	if ( CtrlID != CmdID )
	{	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2); }	
	CtrlID = MSTATUS_GO_TO_POS_BTN_3;
	if ( CtrlID != CmdID )
	{	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2); }	
	CtrlID = MSTATUS_TIME_TEST_BTN;
	if ( CtrlID != CmdID )
	{	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2); }	
	CtrlID = MSTATUS_HOME_ALL_BTN;
	if ( CtrlID != CmdID )
	{	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2); }	
	CtrlID = MSTATUS_ORG_POS_GO_BTN;
	if ( CtrlID != CmdID )
	{	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2); }	

	CtrlID = MSTATUS_START_POS_GO_BTN;
	if ( CtrlID != CmdID )
	{	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2); }	
	CtrlID = MSTATUS_FOCUS_POS_GO_BTN;
	if ( CtrlID != CmdID )
	{	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2); }	
	CtrlID = MSTATUS_PCB_STOP_POS_GO_BTN_LA;
	if ( CtrlID != CmdID )
	{	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2); }	
	CtrlID = MSTATUS_PCB_STOP_POS_GO_BTN_LB;
	if ( CtrlID != CmdID )
	{	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2); }	
	CtrlID = MSTATUS_PCB_STOP_POS_GO_BTN_LA2;
	if ( CtrlID != CmdID )
	{	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2); }	
	CtrlID = MSTATUS_PCB_STOP_POS_GO_BTN_LB2;
	if ( CtrlID != CmdID )
	{	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2); }		
	CtrlID = MSTATUS_PCB_IN_POS_GO_BTN;
	if ( CtrlID != CmdID )
	{	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2); }	
	CtrlID = MSTATUS_ORG_POS_GO_BTN;
	if ( CtrlID != CmdID )
	{	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2); }	
	CtrlID = MSTATUS_LEAVE_POS_GO_BTN;
	if ( CtrlID != CmdID )
	{	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2); }	
	CtrlID = MSTATUS_LANE_LED_STOP_POS_GO_BTN_LA;
	if ( CtrlID != CmdID )
	{	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2); }	
	CtrlID = MSTATUS_LANE_LED_SLOW_POS_GO_BTN_LA;
	if ( CtrlID != CmdID )
	{	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2); }	
	CtrlID = MSTATUS_LANE_LED_STOP_POS_GO_BTN_LB;
	if ( CtrlID != CmdID )
	{	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2); }	
	CtrlID = MSTATUS_LANE_LED_SLOW_POS_GO_BTN_LB;
	if ( CtrlID != CmdID )
	{	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2); }	
	CtrlID = MSTATUS_LANE_LED_STOP_POS_GO_BTN_LA2;
	if ( CtrlID != CmdID )
	{	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2); }	
	CtrlID = MSTATUS_LANE_LED_SLOW_POS_GO_BTN_LA2;
	if ( CtrlID != CmdID )
	{	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2); }	
	CtrlID = MSTATUS_LANE_LED_STOP_POS_GO_BTN_LB2;
	if ( CtrlID != CmdID )
	{	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2); }	
	CtrlID = MSTATUS_LANE_LED_SLOW_POS_GO_BTN_LB2;
	if ( CtrlID != CmdID )
	{	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2); }	

	CtrlID = MSTATUS_REPEAT_12_CHK;
	if ( CtrlID != CmdID )
	{	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2); }	
	CtrlID = MSTATUS_REPEAT_13_CHK;
	if ( CtrlID != CmdID )
	{	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2); }	
	CtrlID = MSTATUS_REPEAT_23_CHK;
	if ( CtrlID != CmdID )
	{	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2); }	

	//about Edit Ctrl
	if ( MotionCtrlPtr->CheckAxisBypass(AXIS_X) == true )
	{	bEnable2 = FALSE; }
	else
	{	bEnable2 = bEnable; }
	CtrlID = MSTATUS_COM_POS_EDIT_X1;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable2);
	CtrlID = MSTATUS_COM_POS_EDIT_X2;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable2);
	CtrlID = MSTATUS_COM_POS_EDIT_X3;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable2);	
	CtrlID = MSTATUS_TIME_EDIT_X;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable2);
	CtrlID = MSTATUS_MOVE_POS_EDIT_X;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable2);

	if ( MotionCtrlPtr->CheckAxisBypass(AXIS_Y) == true )
	{	bEnable2 = FALSE; }
	else
	{	bEnable2 = bEnable; }
	CtrlID = MSTATUS_COM_POS_EDIT_Y1;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable2);
	CtrlID = MSTATUS_COM_POS_EDIT_Y2;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable2);
	CtrlID = MSTATUS_COM_POS_EDIT_Y3;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable2);
	CtrlID = MSTATUS_TIME_EDIT_Y;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable2);
	CtrlID = MSTATUS_MOVE_POS_EDIT_Y;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable2);

	if ( MotionCtrlPtr->CheckAxisBypass(AXIS_Z) == true )
	{	bEnable2 = FALSE; }
	else
	{	bEnable2 = bEnable; }
	CtrlID = MSTATUS_COM_POS_EDIT_Z1;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable2);
	CtrlID = MSTATUS_COM_POS_EDIT_Z2;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable2);
	CtrlID = MSTATUS_COM_POS_EDIT_Z3;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable2);
	CtrlID = MSTATUS_TIME_EDIT_Z;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable2);
	CtrlID = MSTATUS_MOVE_POS_EDIT_Z;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable2);

	CtrlID = MSTATUS_REPEAT_TIME_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnPCBStopPosGoBtnLA() 
{
	// TODO: Add your control notification handler code here
	if ( GetLockUIWnd() == true ) { return; }
	bool IsOK = true;
	LANE_ID      LaneID = LANE_ID_A;
	const bool RightSide = true;
	double PosX = MotionCtrlPtr->GetMotionPCBStopPosX(LaneID, RightSide);
	double PosY = MotionCtrlPtr->GetMotionPCBStopPosY(LaneID, RightSide);
	IsOK = MotionCtrlPtr->XYMoveTo(PosX, PosY);
	if ( false == IsOK )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());	}
	ExecGrabImage();
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnPCBStopPosSetBtnLA() 
{
	// TODO: Add your control notification handler code here
	CString str;
	str = _T("Do you want to set PCB-Stop position?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) != IDYES )
	{	return; }

	bool IsOK = true;
	double PosX = 0;
	double PosY = 0;	
	double PosZ = 0;	
	IsOK = MotionCtrlPtr->GetCurrentPos(PosX, PosY, PosZ);
	if ( false == IsOK )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());	}
	
	LANE_ID    LaneID = LANE_ID_A;
	const bool RightSide = true;
	MotionCtrlPtr->SetMotionPCBStopPosX(LaneID, RightSide, PosX);
	MotionCtrlPtr->SetMotionPCBStopPosY(LaneID, RightSide, PosY);
	//MotionCtrlPtr->SetMotionPCBStopPosZ(LaneID, RightSide, PosZ);//20211115-改成設定焦距位置來設定
	MotionCtrlPtr->SaveMotionParameter();
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnPCBInPosGoBtn() 
{
	// TODO: Add your control notification handler code here
	if ( GetLockUIWnd() == true ) { return; }
	bool IsOK = true;
	double PosX = MotionCtrlPtr->GetMotionParameter().m_BeforePCBInPosX;
	double PosY = MotionCtrlPtr->GetMotionParameter().m_BeforePCBInPosY;	
	IsOK = MotionCtrlPtr->XYMoveTo(PosX, PosY);
	if ( false == IsOK )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());	}
	ExecGrabImage();
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnPCBInPosSetBtn() 
{
	// TODO: Add your control notification handler code here
	CString str;
	str = _T("Do you want to set before PCB-In position?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) != IDYES )
	{	return; }

	bool IsOK = true;
	double PosX = 0;
	double PosY = 0;	
	double PosZ = 0;	
	IsOK = MotionCtrlPtr->GetCurrentPos(PosX, PosY, PosZ);
	if ( false == IsOK )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());	}
	MotionCtrlPtr->GetMotionParameter().m_BeforePCBInPosX = PosX;
	MotionCtrlPtr->GetMotionParameter().m_BeforePCBInPosY = PosY;
	MotionCtrlPtr->GetMotionParameter().m_BeforePCBInPosZ = PosZ;
	MotionCtrlPtr->SaveMotionParameter();
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnRepeat12Chk() 
{
	// TODO: Add your control notification handler code here
	BOOL bCheck = CWnd::IsDlgButtonChecked(MSTATUS_REPEAT_12_CHK);
	if ( FALSE == bCheck ) { return; }
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd )
	{
		CWnd::CheckDlgButton(MSTATUS_REPEAT_12_CHK, FALSE);
		return;
	}		
	UpdateUIToParam();
	LockUIWnd(true, MSTATUS_REPEAT_12_CHK);
	m_RepeatToMode = MOTION_REPEAT_TO_1;
	MotionCtrlPtr->XYZMoveTo(m_CommandPosX1, m_CommandPosY1, m_CommandPosZ1);
	SetTimer(MOTION_STATUS_TIMER_REPEAT_12, m_RepeatTime, 0);
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnRepeat13Chk() 
{
	// TODO: Add your control notification handler code here
	BOOL bCheck = CWnd::IsDlgButtonChecked(MSTATUS_REPEAT_13_CHK);
	if ( FALSE == bCheck ) { return; }
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd )
	{
		CWnd::CheckDlgButton(MSTATUS_REPEAT_13_CHK, FALSE);
		return;
	}		
	UpdateUIToParam();	
	LockUIWnd(true, MSTATUS_REPEAT_13_CHK);
	m_RepeatToMode = MOTION_REPEAT_TO_1;
	MotionCtrlPtr->XYZMoveTo(m_CommandPosX1, m_CommandPosY1, m_CommandPosZ1);
	SetTimer(MOTION_STATUS_TIMER_REPEAT_13, m_RepeatTime, 0);
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnRepeat23Chk() 
{
	// TODO: Add your control notification handler code here	
	BOOL bCheck = CWnd::IsDlgButtonChecked(MSTATUS_REPEAT_23_CHK);
	if ( FALSE == bCheck ) { return; }
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd )
	{
		CWnd::CheckDlgButton(MSTATUS_REPEAT_23_CHK, FALSE);
		return;
	}	
	UpdateUIToParam();
	LockUIWnd(true, MSTATUS_REPEAT_23_CHK);
	m_RepeatToMode = MOTION_REPEAT_TO_2;
	MotionCtrlPtr->XYZMoveTo(m_CommandPosX2, m_CommandPosY2, m_CommandPosZ2);
	SetTimer(MOTION_STATUS_TIMER_REPEAT_23, m_RepeatTime, 0);
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnTimeTestBtn() 
{
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd )
	{	return;	}

	CString str;	
	LARGE_INTEGER fnStart, fnEnd;	
	double PosX=0, PosY=0, PosZ=0;
	double TagX=0, TagY=0, TagZ=0;
	double DisX=0, DisY=0, DisZ=0;		
	BOOL bUseX = CWnd::IsDlgButtonChecked(MSTATUS_TIME_CHK_X);
	BOOL bUseY = CWnd::IsDlgButtonChecked(MSTATUS_TIME_CHK_Y);
	BOOL bUseZ = CWnd::IsDlgButtonChecked(MSTATUS_TIME_CHK_Z);
	DisX = CWnd::GetDlgItemInt(MSTATUS_TIME_EDIT_X);
	DisY = CWnd::GetDlgItemInt(MSTATUS_TIME_EDIT_Y);
	DisZ = CWnd::GetDlgItemInt(MSTATUS_TIME_EDIT_Z);
	MotionCtrlPtr->SetStopXYCalibration(true);
	MotionCtrlPtr->GetCurrentPos(PosX, PosY, PosZ);
	if ( FALSE == bUseX )
	{	TagX = PosX; }
	else 
	{	TagX = PosX+DisX; }

	if ( FALSE == bUseY )
	{	TagY = PosY; }
	else 
	{	TagY = PosY+DisY; }

	if ( FALSE == bUseZ )
	{	TagZ = PosZ; }
	else 
	{	TagZ = PosZ+DisZ; }
	
	JetAPI::SetFuncTimeStart(fnStart);
	if ( MotionCtrlPtr->XYZMoveTo(TagX, TagY, TagZ) == false )
	{
		MotionCtrlPtr->SetStopXYCalibration(false);
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return;
	}
	if ( MotionCtrlPtr->WaitForMotionStop() == false )
	{
		MotionCtrlPtr->SetStopXYCalibration(false);
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return;
	}
	JetAPI::SetFuncTimeEnd(fnEnd);	
	MotionCtrlPtr->XYZMoveTo(PosX, PosY, PosZ);
	MotionCtrlPtr->SetStopXYCalibration(false);
	
	double Time=JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);
	str.Format(_T("%.0f ms"), Time);
	CWnd::SetDlgItemText(MSTATUS_TIME_TEST_EDIT, str);
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnPCBStopPosGoBtnLB() 
{
	// TODO: Add your control notification handler code here
	if ( GetLockUIWnd() == true ) { return; }
	bool IsOK = true;
	LANE_ID    LaneID = LANE_ID_B;
	const bool RightSide = true;
	double PosX = MotionCtrlPtr->GetMotionPCBStopPosX(LaneID, RightSide);
	double PosY = MotionCtrlPtr->GetMotionPCBStopPosY(LaneID, RightSide);
	IsOK = MotionCtrlPtr->XYMoveTo(PosX, PosY);
	if ( false == IsOK )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());	}
	ExecGrabImage();
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnPCBStopPosSetBtnLB() 
{
	// TODO: Add your control notification handler code here
	CString str;
	str = _T("Do you want to set PCB-Stop position?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) != IDYES )
	{	return; }

	bool IsOK = true;
	double PosX = 0;
	double PosY = 0;	
	double PosZ = 0;	
	IsOK = MotionCtrlPtr->GetCurrentPos(PosX, PosY, PosZ);
	if ( false == IsOK )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());	}
	
	LANE_ID    LaneID = LANE_ID_B;
	const bool RightSide = true;
	MotionCtrlPtr->SetMotionPCBStopPosX(LaneID, RightSide, PosX);
	MotionCtrlPtr->SetMotionPCBStopPosY(LaneID, RightSide, PosY);
	//MotionCtrlPtr->SetMotionPCBStopPosZ(LaneID, RightSide, PosZ);//20211115-改成設定焦距位置來設定
	MotionCtrlPtr->SaveMotionParameter();
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnLeavePosGoBtn() 
{
	// TODO: Add your control notification handler code here
	if ( GetLockUIWnd() == true ) { return; }
	bool IsOK = true;
	double PosX = MotionCtrlPtr->GetMotionParameter().m_StageLeavePosX;
	double PosY = MotionCtrlPtr->GetMotionParameter().m_StageLeavePosY;
	IsOK = MotionCtrlPtr->XYMoveTo(PosX, PosY);
	if ( false == IsOK )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());	}
	ExecGrabImage();
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnLeavePosSetBtn() 
{
	// TODO: Add your control notification handler code here
	CString str;
	str = _T("Do you want to set XY leave position?");
	str = LoadMultiLanguageString(str, str);	
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) != IDYES )
	{	return; }

	bool IsOK = true;
	double PosX = 0;
	double PosY = 0;	
	double PosZ = 0;	
	IsOK = MotionCtrlPtr->GetCurrentPos(PosX, PosY, PosZ);
	if ( false == IsOK )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());	}
	MotionCtrlPtr->GetMotionParameter().m_StageLeavePosX = PosX;
	MotionCtrlPtr->GetMotionParameter().m_StageLeavePosY = PosY;
	MotionCtrlPtr->SaveMotionParameter();
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnORGPosSetBtn() 
{
	// TODO: Add your control notification handler code here
	double       PosX=0, PosY=0, PosZ=0;
	CString      str;
	CString      strValueX;
	CString      strLabelX;
	CString      strValueY;
	CString      strLabelY;
	CString      strCaption;
	CString      strAxis = AOIDataDefine.GetAxisText();
	CInputBoxWnd InputBox;
	TMotionParameter &MotionParam = MotionCtrlPtr->GetMotionParameter();

	MotionCtrlPtr->GetCurrentPos(PosX, PosY, PosZ);
	strCaption = _T("Input Target Position Wnd");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strLabelX.Format(_T("X %s"), strAxis);
	strValueX.Format(_T("%.0f"), PosX);	
	strLabelY.Format(_T("Y %s"), strAxis);
	strValueY.Format(_T("%.0f"), PosY);
	InputBox.SetParam2(strCaption, strLabelX, strValueX, strLabelY, strValueY);
	if ( InputBox.DoModal() == IDCANCEL ) 
	{	return; }

	strValueX = InputBox.m_DataEdit1;
	strValueY = InputBox.m_DataEdit2;
	const double OldORGX = MotionParam.m_HomeOrgOffsetX;//舊的原點-X
	const double OldORGY = MotionParam.m_HomeOrgOffsetY;//舊的原點-Y
	const double OldORGZ = MotionParam.m_HomeOrgOffsetZ;//舊的原點-Z
	const double GlobalX = OldORGX+PosX;//全域的座標-X
	const double GlobalY = OldORGY+PosY;//全域的座標-Y
	const double GlobalZ = OldORGZ+PosZ;//全域的座標-Y
	const double NewPosX = ::_ttof(strValueX);
	const double NewPosY = ::_ttof(strValueY);
	const double NewPosZ = PosZ;
	const double NewORGX = GlobalX-NewPosX;//新的原點-X
	const double NewORGY = GlobalY-NewPosY;//新的原點-Y
	const double NewORGZ = GlobalZ-NewPosZ;//新的原點-Z
	if ( MotionCtrlPtr->ModifyORGPosition(NewORGX, NewORGY, NewORGZ) == false ) 
	{	return; }
	MotionCtrlPtr->SaveMotionParameter();
	const bool bPcbInsideLA=PlcCtrlPtr->CheckPCBInside_LA();
	const bool bPcbInsideLB=PlcCtrlPtr->CheckPCBInside_LB();
	if ( true==bPcbInsideLA || true==bPcbInsideLB )
	{	str = _T("PCB Inside, Do you want to Home XYZ?");	}
	else
	{	str = _T("Do you want to Home XYZ?");	}
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDYES )
	{	
		if ( MotionCtrlPtr->ExecHomeAll(false) == false ) 
		{
			str = MotionCtrlPtr->GetErrorString();
			JetAPI::ShowMessageBox(str);
		}
		else if ( MotionCtrlPtr->XYZMoveTo(NewPosX, NewPosY, NewPosZ) == false )
		{
			str = MotionCtrlPtr->GetErrorString();
			JetAPI::ShowMessageBox(str);		
		}
	}

	bool bSupportConvertXYCali=true;
	if ( true == bSupportConvertXYCali )
	{
		LANE_ID LaneID;
		CString Filename;		
		double OrgOffsetX = OldORGX-NewORGX;
		double OrgOffsetY = OldORGY-NewORGY;
		double OrgOffsetZ = OldORGZ-NewORGZ;
		CString Folder=AOIDataCollect.GetAOIDirectory();
		
		int CountX = 0;
		int CountY = 0;
		std::vector<TDotNode> DotList;		
		//For Lane A	
		DotList.clear();
		LaneID = LANE_ID_A;
		Filename = AOIDataCollect.GetDotNodeCaliFilename(Folder, LaneID);	
		if ( AOIDataCollect.LoadXYDotNodeCaliFile(Filename, CountX, CountY, DotList) == true )
		{
			AOIDataCollect.MoveXYDotNodeList(OrgOffsetX, OrgOffsetY, DotList);		
			AOIDataCollect.SaveXYDotNodeCaliFile(Filename, CountX, CountY, DotList);
		}
		//For Lane B
		DotList.clear();
		LaneID = LANE_ID_B;
		Filename = AOIDataCollect.GetDotNodeCaliFilename(Folder, LaneID);	
		if ( AOIDataCollect.LoadXYDotNodeCaliFile(Filename, CountX, CountY, DotList) == true )
		{
			AOIDataCollect.MoveXYDotNodeList(OrgOffsetX, OrgOffsetY, DotList);			
			AOIDataCollect.SaveXYDotNodeCaliFile(Filename, CountX, CountY, DotList);
		}

		//For Motion XYCali		
		std::vector<TXYCali> XYCaliList;
		CString XYCaliName = MOTION_XY_CALI_FILE;
		Filename.Format(_T("%s\\%s"), Folder, XYCaliName);
		MotionCtrlPtr->LoadMotionXYCali(Filename, XYCaliList);	
		MotionCtrlPtr->MoveMotionXYCali(OrgOffsetX, OrgOffsetY, XYCaliList);//移動運動系統的XY校正表		
		MotionCtrlPtr->SaveMotionXYCali(Filename, XYCaliList);
		MotionCtrlPtr->LoadMotionXYCali();

		str = AOIDataDefine.GetFinishText();
		JetAPI::ShowMessageBox(str);
	}
	else
	{
		str = _T("Change XYZ ORG , You have to calibrate XY-Dot Position");
		str = LoadMultiLanguageString(str, str);
		JetAPI::ShowMessageBox(str);
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnPCBStopPosGoBtnLA2() 
{
	// TODO: Add your control notification handler code here
	if ( GetLockUIWnd() == true ) { return; }
	bool IsOK = true;
	LANE_ID    LaneID = LANE_ID_A;
	const bool RightSide = false;
	double PosX = MotionCtrlPtr->GetMotionPCBStopPosX(LaneID, RightSide);
	double PosY = MotionCtrlPtr->GetMotionPCBStopPosY(LaneID, RightSide);
	IsOK = MotionCtrlPtr->XYMoveTo(PosX, PosY);
	if ( false == IsOK )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());	}
	ExecGrabImage();
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnPCBStopPosGoBtnLB2() 
{
	// TODO: Add your control notification handler code here
	if ( GetLockUIWnd() == true ) { return; }
	bool IsOK = true;
	LANE_ID    LaneID = LANE_ID_B;
	const bool RightSide = false;
	double PosX = MotionCtrlPtr->GetMotionPCBStopPosX(LaneID, RightSide);
	double PosY = MotionCtrlPtr->GetMotionPCBStopPosY(LaneID, RightSide);
	IsOK = MotionCtrlPtr->XYMoveTo(PosX, PosY);
	if ( false == IsOK )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());	}
	ExecGrabImage();
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnPCBStopPosSetBtnLA2() 
{
	// TODO: Add your control notification handler code here
	CString str;
	str = _T("Do you want to set PCB-Stop position?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) != IDYES )
	{	return; }

	bool IsOK = true;
	double PosX = 0;
	double PosY = 0;	
	double PosZ = 0;	
	IsOK = MotionCtrlPtr->GetCurrentPos(PosX, PosY, PosZ);
	if ( false == IsOK )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());	}
	
	LANE_ID    LaneID = LANE_ID_A;
	const bool RightSide = false;
	MotionCtrlPtr->SetMotionPCBStopPosX(LaneID, RightSide, PosX);
	MotionCtrlPtr->SetMotionPCBStopPosY(LaneID, RightSide, PosY);
	//MotionCtrlPtr->SetMotionPCBStopPosZ(LaneID, RightSide, PosZ);//20211115-改成設定焦距位置來設定
	MotionCtrlPtr->SaveMotionParameter();
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnPCBStopPosSetBtnLB2() 
{
	// TODO: Add your control notification handler code here
	CString str;
	str = _T("Do you want to set PCB-Stop position?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) != IDYES )
	{	return; }

	bool IsOK = true;
	double PosX = 0;
	double PosY = 0;	
	double PosZ = 0;	
	IsOK = MotionCtrlPtr->GetCurrentPos(PosX, PosY, PosZ);
	if ( false == IsOK )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());	}
	
	LANE_ID    LaneID = LANE_ID_B;
	const bool RightSide = false;
	MotionCtrlPtr->SetMotionPCBStopPosX(LaneID, RightSide, PosX);
	MotionCtrlPtr->SetMotionPCBStopPosY(LaneID, RightSide, PosY);
	//MotionCtrlPtr->SetMotionPCBStopPosZ(LaneID, RightSide, PosZ);//20211115-改成設定焦距位置來設定
	MotionCtrlPtr->SaveMotionParameter();
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnSaveUIBtn() 
{
	// TODO: Add your control notification handler code here
	SaveUIParamFile();
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnLoadUIBtn() 
{
	// TODO: Add your control notification handler code here
	LoadUIParamFile();
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnLaneLedStopPosGoBtnLA()
{
	// TODO: Add your control notification handler code here	
	if ( GetLockUIWnd() == true ) { return; }
	bool IsOK = true;
	const bool RightSide = true;
	LANE_ID    LaneID = LANE_ID_A;	
	double PosX = MotionCtrlPtr->GetMotionLaneLedStopPosX(LaneID, RightSide);
	double PosY = MotionCtrlPtr->GetMotionLaneLedStopPosY(LaneID, RightSide);
	IsOK = MotionCtrlPtr->XYMoveTo(PosX, PosY);
	if ( false == IsOK )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());	}
	ExecGrabImage();
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnLaneLedStopPosSetBtnLA()
{
	// TODO: Add your control notification handler code here	
	CString str;
	str = _T("Do you want to set LED-Stop position?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) != IDYES )
	{	return; }

	bool IsOK = true;
	double PosX = 0;
	double PosY = 0;	
	double PosZ = 0;	
	IsOK = MotionCtrlPtr->GetCurrentPos(PosX, PosY, PosZ);
	if ( false == IsOK )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());	}
	
	const bool RightSide = true;
	LANE_ID    LaneID = LANE_ID_A;	
	MotionCtrlPtr->SetMotionLaneLedStopPosX(LaneID, RightSide, PosX);
	MotionCtrlPtr->SetMotionLaneLedStopPosY(LaneID, RightSide, PosY);
	//MotionCtrlPtr->SetMotionLaneLedStopPosZ(LaneID, PosZ);//20211115-改成設定焦距位置來設定
	MotionCtrlPtr->SaveMotionParameter();
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnLaneLedSlowPosGoBtnLA()
{
	// TODO: Add your control notification handler code here
	if ( GetLockUIWnd() == true ) { return; }
	bool IsOK = true;
	const bool RightSide = true;
	LANE_ID    LaneID = LANE_ID_A;	
	double PosX = MotionCtrlPtr->GetMotionLaneLedSlowPosX(LaneID, RightSide);
	double PosY = MotionCtrlPtr->GetMotionLaneLedSlowPosY(LaneID, RightSide);
	IsOK = MotionCtrlPtr->XYMoveTo(PosX, PosY);
	if ( false == IsOK )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());	}
	ExecGrabImage();
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnLaneLedSlowPosSetBtnLA()
{
	// TODO: Add your control notification handler code here	
	CString str;
	str = _T("Do you want to set LED-Slow position?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) != IDYES )
	{	return; }

	bool IsOK = true;
	double PosX = 0;
	double PosY = 0;	
	double PosZ = 0;	
	IsOK = MotionCtrlPtr->GetCurrentPos(PosX, PosY, PosZ);
	if ( false == IsOK )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());	}
	
	const bool RightSide = true;
	LANE_ID    LaneID = LANE_ID_A;	
	MotionCtrlPtr->SetMotionLaneLedSlowPosX(LaneID, RightSide, PosX);
	MotionCtrlPtr->SetMotionLaneLedSlowPosY(LaneID, RightSide, PosY);
	//MotionCtrlPtr->SetMotionLaneLedSlowPosZ(LaneID, PosZ);//20211115-改成設定焦距位置來設定
	MotionCtrlPtr->SaveMotionParameter();
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnLaneLedStopPosGoBtnLB()
{
	// TODO: Add your control notification handler code here	
	if ( GetLockUIWnd() == true ) { return; }
	bool IsOK = true;
	const bool RightSide = true;
	LANE_ID    LaneID = LANE_ID_B;	
	double PosX = MotionCtrlPtr->GetMotionLaneLedStopPosX(LaneID, RightSide);
	double PosY = MotionCtrlPtr->GetMotionLaneLedStopPosY(LaneID, RightSide);
	IsOK = MotionCtrlPtr->XYMoveTo(PosX, PosY);
	if ( false == IsOK )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());	}
	ExecGrabImage();
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnLaneLedStopPosSetBtnLB()
{
	// TODO: Add your control notification handler code here	
	CString str;
	str = _T("Do you want to set LED-Stop position?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) != IDYES )
	{	return; }

	bool IsOK = true;
	double PosX = 0;
	double PosY = 0;	
	double PosZ = 0;	
	IsOK = MotionCtrlPtr->GetCurrentPos(PosX, PosY, PosZ);
	if ( false == IsOK )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());	}
	
	const bool RightSide = true;
	LANE_ID    LaneID = LANE_ID_B;	
	MotionCtrlPtr->SetMotionLaneLedStopPosX(LaneID, RightSide, PosX);
	MotionCtrlPtr->SetMotionLaneLedStopPosY(LaneID, RightSide, PosY);
	//MotionCtrlPtr->SetMotionLaneLedStopPosZ(LaneID, RightSide, PosZ);//20211115-改成設定焦距位置來設定
	MotionCtrlPtr->SaveMotionParameter();
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnLaneLedSlowPosGoBtnLB()
{
	// TODO: Add your control notification handler code here	
	if ( GetLockUIWnd() == true ) { return; }
	bool IsOK = true;
	const bool RightSide = true;
	LANE_ID    LaneID = LANE_ID_B;		
	double PosX = MotionCtrlPtr->GetMotionLaneLedSlowPosX(LaneID, RightSide);
	double PosY = MotionCtrlPtr->GetMotionLaneLedSlowPosY(LaneID, RightSide);
	IsOK = MotionCtrlPtr->XYMoveTo(PosX, PosY);
	if ( false == IsOK )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());	}
	ExecGrabImage();
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnLaneLedSlowPosSetBtnLB()	
{
	// TODO: Add your control notification handler code here	
	CString str;
	str = _T("Do you want to set LED-Slow position?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) != IDYES )
	{	return; }

	bool IsOK = true;
	double PosX = 0;
	double PosY = 0;	
	double PosZ = 0;	
	IsOK = MotionCtrlPtr->GetCurrentPos(PosX, PosY, PosZ);
	if ( false == IsOK )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());	}
	
	const bool RightSide = true;
	LANE_ID    LaneID = LANE_ID_B;	
	MotionCtrlPtr->SetMotionLaneLedSlowPosX(LaneID, RightSide, PosX);
	MotionCtrlPtr->SetMotionLaneLedSlowPosY(LaneID, RightSide, PosY);
	//MotionCtrlPtr->SetMotionLaneLedSlowPosZ(LaneID, RightSide, PosZ);//20211115-改成設定焦距位置來設定
	MotionCtrlPtr->SaveMotionParameter();
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnLaneLedStopPosGoBtnLA2()
{
	// TODO: Add your control notification handler code here	
	if ( GetLockUIWnd() == true ) { return; }
	bool IsOK = true;
	const bool RightSide = false;
	LANE_ID    LaneID = LANE_ID_A;	
	double PosX = MotionCtrlPtr->GetMotionLaneLedStopPosX(LaneID, RightSide);
	double PosY = MotionCtrlPtr->GetMotionLaneLedStopPosY(LaneID, RightSide);
	IsOK = MotionCtrlPtr->XYMoveTo(PosX, PosY);
	if ( false == IsOK )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());	}
	ExecGrabImage();
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnLaneLedStopPosSetBtnLA2()
{
	// TODO: Add your control notification handler code here	
	CString str;
	str = _T("Do you want to set LED-Stop position?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) != IDYES )
	{	return; }

	bool IsOK = true;
	double PosX = 0;
	double PosY = 0;	
	double PosZ = 0;	
	IsOK = MotionCtrlPtr->GetCurrentPos(PosX, PosY, PosZ);
	if ( false == IsOK )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());	}
	
	const bool RightSide = false;
	LANE_ID    LaneID = LANE_ID_A;	
	MotionCtrlPtr->SetMotionLaneLedStopPosX(LaneID, RightSide, PosX);
	MotionCtrlPtr->SetMotionLaneLedStopPosY(LaneID, RightSide, PosY);
	//MotionCtrlPtr->SetMotionLaneLedStopPosZ(LaneID, PosZ);//20211115-改成設定焦距位置來設定
	MotionCtrlPtr->SaveMotionParameter();
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnLaneLedSlowPosGoBtnLA2()
{
	// TODO: Add your control notification handler code here
	if ( GetLockUIWnd() == true ) { return; }
	bool IsOK = true;
	const bool RightSide = false;
	LANE_ID    LaneID = LANE_ID_A;	
	double PosX = MotionCtrlPtr->GetMotionLaneLedSlowPosX(LaneID, RightSide);
	double PosY = MotionCtrlPtr->GetMotionLaneLedSlowPosY(LaneID, RightSide);
	IsOK = MotionCtrlPtr->XYMoveTo(PosX, PosY);
	if ( false == IsOK )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());	}
	ExecGrabImage();
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnLaneLedSlowPosSetBtnLA2()
{
	// TODO: Add your control notification handler code here	
	CString str;
	str = _T("Do you want to set LED-Slow position?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) != IDYES )
	{	return; }

	bool IsOK = true;
	double PosX = 0;
	double PosY = 0;	
	double PosZ = 0;	
	IsOK = MotionCtrlPtr->GetCurrentPos(PosX, PosY, PosZ);
	if ( false == IsOK )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());	}
	
	const bool RightSide = false;
	LANE_ID    LaneID = LANE_ID_A;	
	MotionCtrlPtr->SetMotionLaneLedSlowPosX(LaneID, RightSide, PosX);
	MotionCtrlPtr->SetMotionLaneLedSlowPosY(LaneID, RightSide, PosY);
	//MotionCtrlPtr->SetMotionLaneLedSlowPosZ(LaneID, PosZ);//20211115-改成設定焦距位置來設定
	MotionCtrlPtr->SaveMotionParameter();
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnLaneLedStopPosGoBtnLB2()
{
	// TODO: Add your control notification handler code here	
	if ( GetLockUIWnd() == true ) { return; }
	bool IsOK = true;
	const bool RightSide = false;
	LANE_ID    LaneID = LANE_ID_B;	
	double PosX = MotionCtrlPtr->GetMotionLaneLedStopPosX(LaneID, RightSide);
	double PosY = MotionCtrlPtr->GetMotionLaneLedStopPosY(LaneID, RightSide);
	IsOK = MotionCtrlPtr->XYMoveTo(PosX, PosY);
	if ( false == IsOK )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());	}
	ExecGrabImage();
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnLaneLedStopPosSetBtnLB2()
{
	// TODO: Add your control notification handler code here	
	CString str;
	str = _T("Do you want to set LED-Stop position?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) != IDYES )
	{	return; }

	bool IsOK = true;
	double PosX = 0;
	double PosY = 0;	
	double PosZ = 0;	
	IsOK = MotionCtrlPtr->GetCurrentPos(PosX, PosY, PosZ);
	if ( false == IsOK )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());	}
	
	const bool RightSide = false;
	LANE_ID    LaneID = LANE_ID_B;	
	MotionCtrlPtr->SetMotionLaneLedStopPosX(LaneID, RightSide, PosX);
	MotionCtrlPtr->SetMotionLaneLedStopPosY(LaneID, RightSide, PosY);
	//MotionCtrlPtr->SetMotionLaneLedStopPosZ(LaneID, RightSide, PosZ);//20211115-改成設定焦距位置來設定
	MotionCtrlPtr->SaveMotionParameter();
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnLaneLedSlowPosGoBtnLB2()
{
	// TODO: Add your control notification handler code here	
	if ( GetLockUIWnd() == true ) { return; }
	bool IsOK = true;
	const bool RightSide = false;
	LANE_ID    LaneID = LANE_ID_B;		
	double PosX = MotionCtrlPtr->GetMotionLaneLedSlowPosX(LaneID, RightSide);
	double PosY = MotionCtrlPtr->GetMotionLaneLedSlowPosY(LaneID, RightSide);
	IsOK = MotionCtrlPtr->XYMoveTo(PosX, PosY);
	if ( false == IsOK )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());	}
	ExecGrabImage();
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneStatus::OnLaneLedSlowPosSetBtnLB2()	
{
	// TODO: Add your control notification handler code here	
	CString str;
	str = _T("Do you want to set LED-Slow position?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) != IDYES )
	{	return; }

	bool IsOK = true;
	double PosX = 0;
	double PosY = 0;	
	double PosZ = 0;	
	IsOK = MotionCtrlPtr->GetCurrentPos(PosX, PosY, PosZ);
	if ( false == IsOK )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());	}
	
	const bool RightSide = false;
	LANE_ID    LaneID = LANE_ID_B;	
	MotionCtrlPtr->SetMotionLaneLedSlowPosX(LaneID, RightSide, PosX);
	MotionCtrlPtr->SetMotionLaneLedSlowPosY(LaneID, RightSide, PosY);
	//MotionCtrlPtr->SetMotionLaneLedSlowPosZ(LaneID, RightSide, PosZ);//20211115-改成設定焦距位置來設定
	MotionCtrlPtr->SaveMotionParameter();
}
//-------------------------------------------------------------------------------------//