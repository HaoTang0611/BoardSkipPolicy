// Light3DTiDLPWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "Light3DTiDLPWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
const int PATTERN_LIST_SUBITEM_INDEX_IDX               = 0;
const int PATTERN_LIST_SUBITEM_INDEX_TRIGGER_TYPE      = 1;
const int PATTERN_LIST_SUBITEM_INDEX_FLASH_IDX         = 2;
const int PATTERN_LIST_SUBITEM_INDEX_PAT_NUM           = 3;
const int PATTERN_LIST_SUBITEM_INDEX_BIT_DEPTH         = 4;
const int PATTERN_LIST_SUBITEM_INDEX_BIT_RANGE         = 5;
const int PATTERN_LIST_SUBITEM_INDEX_LED_IDX           = 6;
const int PATTERN_LIST_SUBITEM_INDEX_INVERT_PAT        = 7;
const int PATTERN_LIST_SUBITEM_INDEX_INSERT_BLAK       = 8;
const int PATTERN_LIST_SUBITEM_INDEX_BUFFER_SWAP       = 9;
const int PATTERN_LIST_SUBITEM_INDEX_TRIG_OUT_PREV     = 10;
//-------------------------------------------------------------------------------------//
CLight3DTiDLPWnd Light3DTiDLPWnd;
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CLight3DTiDLPWnd dialog
//-------------------------------------------------------------------------------------//
CLight3DTiDLPWnd::CLight3DTiDLPWnd(CWnd* pParent /*=NULL*/)
	: CDialog(CLight3DTiDLPWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLight3DTiDLPWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLPWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLight3DTiDLPWnd)		
	DDX_Control(pDX, TIDLP_LED_CURRENT_ID_COMBO, m_LEDCurrentIDCombox);
	DDX_Control(pDX, TIDLP_PATTERN_EXP_NUM_COMBO, m_PatternExpNumCombox);
	DDX_Control(pDX, TIDLP_PATTERN_SQUENCE_MODE_COMBO, m_PatternSequenceModeCombox);
	DDX_Control(pDX, TIDLP_PATTERN_BUILD_PHASE_SHIFT_DIRECTION_COMBO, m_PatternBuildPhaseShiftDirectionCombox);
	DDX_Control(pDX, TIDLP_PATTERN_LIST_WND, m_PatternListWnd);
	DDX_Control(pDX, TIDLP_SEQUENCE_TRIGGER_MODE_COMBO, m_SequenceTriggerModeCombox);
	DDX_Control(pDX, TIDLP_PATTERN_SOURCE_COMBO, m_PatternSouceCombox);
	DDX_Control(pDX, TIDLP_PATTERN_BIT_RANGE_COMBO, m_PatternBitRangeCombox);
	DDX_Control(pDX, TIDLP_PATTERN_FLASH_INDEX_COMBO, m_PatternFlashIndexCombox);
	DDX_Control(pDX, TIDLP_OPERATION_MODE_COMBO, m_OperationModeCombox);
	DDX_Control(pDX, TIDLP_PATTERN_BUILD_PHASE_SHIFT_COUNT_COMBO, m_PatternBuildPhaseShiftCountCombo);
	DDX_Control(pDX, TIDLP_PATTERN_BUILD_BIT_DEPTH_COMBO, m_PatternBuildBitDepthCombox);	
	DDX_Control(pDX, TIDLP_PATTERN_TRIGGER_TYPE_COMBO, m_PatternTriggerTypeCombox);	
	DDX_Control(pDX, TIDLP_PATTERN_BIT_DEPTH_COMBO, m_PatternBitDepthCombox);
	DDX_Control(pDX, TIDLP_PATTERN_LED_COLOR_COMBO, m_PatternLEDColorCombox);
	DDX_Control(pDX, TIDLP_PROJECT_ID_COMBO, m_Light3DCastIDCombox);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CLight3DTiDLPWnd, CDialog)
	//{{AFX_MSG_MAP(CLight3DTiDLPWnd)
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(TIDLP_PROJECT_ID_COMBO, OnSelchangeProjectIDCombo)
	ON_BN_CLICKED(TIDLP_CONNECT_BTN, OnConnectBtn)
	ON_BN_CLICKED(TIDLP_DISCONNECT_BTN, OnDisconnectBtn)	
	ON_BN_CLICKED(TIDLP_PATTERN_BUILD_EXPORT_BTN, OnPatternBuildExportBtn)
	ON_BN_CLICKED(TIDLP_RESET_BTN, OnResetBtn)
	ON_BN_CLICKED(TIDLP_LED_CURRENT_SET_BTN, OnLEDCurrentSetBtn)
	ON_BN_CLICKED(TIDLP_LED_CURRENT_OFF_BTN, OnLEDCurrentOffBtn)
	ON_BN_CLICKED(TIDLP_SAVE_INI_BTN, OnSaveIniBtn)
	ON_BN_CLICKED(TIDLP_LOAD_INI_BTN, OnLoadIniBtn)
	ON_BN_CLICKED(TIDLP_LED_AUTO_MODE_CHK, OnLEDAutoModeChk)
	ON_WM_TIMER()
	ON_BN_CLICKED(TIDLP_STATUS_AUTO_UPDATE_CHK, OnStatusAutoUpdateChk)
	ON_CBN_SELCHANGE(TIDLP_OPERATION_MODE_COMBO, OnSelchangeOperationModeCombo)
	ON_CBN_SELCHANGE(TIDLP_PATTERN_BIT_DEPTH_COMBO, OnSelchangePatternBitDepthCombo)
	ON_BN_CLICKED(TIDLP_PATTERN_ADD_BTN, OnPatternAddBtn)
	ON_BN_CLICKED(TIDLP_PATTERN_SEND_BTN, OnPatternSendBtn)
	ON_BN_CLICKED(TIDLP_PATTERN_READ_BTN, OnPatternReadBtn)
	ON_BN_CLICKED(TIDLP_PATTERN_CLEAR_BTN, OnPatternClearBtn)
	ON_BN_CLICKED(TIDLP_PATTERN_SEQUENCE_VALIDATE_BTN, OnPatternSequenceValidateBtn)
	ON_BN_CLICKED(TIDLP_PATTERN_SEQUENCE_PLAY_BTN, OnPatternSequencePlayBtn)
	ON_BN_CLICKED(TIDLP_PATTERN_SEQUENCE_STOP_BTN, OnPatternSequenceStopBtn)
	ON_BN_CLICKED(TIDLP_PATTERN_SEQUENCE_PAUSE_BTN, OnPatternSequencePauseBtn)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(TIDLP_RETRIEVE_BTN, OnRetrieveBtn)
	ON_BN_CLICKED(TIDLP_PATTERN_DEL_BTN, OnPatternDelBtn)
	ON_CBN_SELCHANGE(TIDLP_PATTERN_FLASH_INDEX_COMBO, OnSelchangePatternFlashIndexCombo)
	ON_NOTIFY(NM_DBLCLK, TIDLP_PATTERN_LIST_WND, OnDblclkPatternListWnd)
	ON_BN_CLICKED(TIDLP_PATTERN_SEQUENCE_PLAY_ONCE_BTN, OnPatternSequencePlayOnceBtn)
	ON_BN_CLICKED(TIDLP_PATTERN_SEQUENCE_PLAY_REPEAT_BTN, OnPatternSequencePlayRepeatBtn)
	ON_CBN_SELCHANGE(TIDLP_LED_CURRENT_ID_COMBO, OnSelchangeLEDCurrentIDCombo)		
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CLight3DTiDLPWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CLight3DTiDLPWnd::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	JetAPI::InitialListCtrl(m_PatternListWnd);
	CLight3DCtrl::BuildCastIDCombox(m_Light3DCastIDCombox, false);

	AOIDataDefine.BuildDLPLEDCurrentIDCombox(m_LEDCurrentIDCombox);
	AOIDataDefine.BuildDLPOperationModeCombox(m_OperationModeCombox);
	AOIDataDefine.BuildDLPPatternExpNumCombox(m_PatternExpNumCombox);
	AOIDataDefine.BuildDLPPatternSequenceModeCombox(m_PatternSequenceModeCombox);
	AOIDataDefine.BuildDLPPatternLEDColorCombox(m_PatternLEDColorCombox);
	AOIDataDefine.BuildDLPPatternFlashIndexCombox(m_PatternFlashIndexCombox);	
	AOIDataDefine.BuildDLPPatternBitDepthCombox(m_PatternBitDepthCombox);	
	AOIDataDefine.BuildDLPPatternBitRangeCombox(1, m_PatternBitRangeCombox);			
	AOIDataDefine.BuildDLPPatternTriggerTypeCombox(m_PatternTriggerTypeCombox);	
	
	AOIDataDefine.BuildDLPPatternSouceCombox(m_PatternSouceCombox);	
	AOIDataDefine.BuildDLPSequenceTriggerModeCombox(m_SequenceTriggerModeCombox);
	
	this->BuildPatternBuildPhaseShiftCombox(m_PatternBuildPhaseShiftCountCombo);
	this->BuildPatternBuildPhaseDirectionCombox(m_PatternBuildPhaseShiftDirectionCombox);
	AOIDataDefine.BuildDLPPatternBitDepthCombox(m_PatternBuildBitDepthCombox);	
	
	m_Light3DCastIDCombox.SetCurSel(0);	
	m_LEDCurrentIDCombox.SetCurSel(0);	
	m_PatternFlashIndexCombox.SetCurSel(0);	
	m_PatternBitRangeCombox.SetCurSel(0);
	m_PatternSouceCombox.SetCurSel(0);	
	m_SequenceTriggerModeCombox.SetCurSel(0);		
	

	JetAPI::SetComboxCurSel(m_PatternBuildBitDepthCombox, DLP_BIT_DEPTH_6);
	JetAPI::SetComboxCurSel(m_PatternSequenceModeCombox, DLP_PATTERN_SEQUENCE_4_4_M);

	this->SetDlgItemInt(TIDLP_PATTERN_BUILD_START_PHASE_EDIT, 0);
	this->SetDlgItemInt(TIDLP_PATTERN_BUILD_PIXELS_PERIOD_EDIT, 8);
	JetAPI::SetComboxCurSel(m_PatternBuildPhaseShiftDirectionCombox, 2);	
	JetAPI::SetComboxCurSel(m_PatternExpNumCombox, DLP_PATTERN_EXP_NUM_01);

	const int width = 68;
	this->m_PatternListWnd.InsertColumn(0, _T("Idx"), LVCFMT_CENTER, 32);
	this->m_PatternListWnd.InsertColumn(1, _T("Trig_Type"), LVCFMT_CENTER, width);
	this->m_PatternListWnd.InsertColumn(2, _T("Flash"), LVCFMT_CENTER, width);
	this->m_PatternListWnd.InsertColumn(3, _T("BitNum"), LVCFMT_CENTER, width);
	this->m_PatternListWnd.InsertColumn(4, _T("BitDepth"), LVCFMT_CENTER, width);	
	this->m_PatternListWnd.InsertColumn(5, _T("Range"), LVCFMT_CENTER, width);	
	this->m_PatternListWnd.InsertColumn(6, _T("LED"), LVCFMT_CENTER, width);
	this->m_PatternListWnd.InsertColumn(7, _T("Invert"), LVCFMT_CENTER, width);
	this->m_PatternListWnd.InsertColumn(8, _T("Black"), LVCFMT_CENTER, width);
	this->m_PatternListWnd.InsertColumn(9, _T("Swap"), LVCFMT_CENTER, width);
	this->m_PatternListWnd.InsertColumn(10, _T("TrigOutPrev"), LVCFMT_CENTER, width);
	this->m_PatternListWnd.InsertColumn(11, _T("Period"), LVCFMT_CENTER, width);
	this->m_PatternListWnd.InsertColumn(12, _T("Exposure"), LVCFMT_CENTER, width);

	JetAPI::SetComboxCurSel(m_PatternBuildPhaseShiftCountCombo, 4);
	this->CheckDlgButton(TIDLP_LED_ENABLE_RED_CHK, TRUE);
	this->CheckDlgButton(TIDLP_LED_ENABLE_GREEN_CHK, TRUE);
	this->CheckDlgButton(TIDLP_LED_ENABLE_BLUE_CHK, TRUE);
	this->CheckDlgButton(TIDLP_STATUS_AUTO_UPDATE_CHK, TRUE);
	this->CheckDlgButton(TIDLP_CLEAR_DMD_AFTER_EXPOSURE_CHK, TRUE);
	this->CheckDlgButton(TIDLP_PATTERN_SEQUENCE_REPEAT_CHK, TRUE);
	this->CheckDlgButton(TIDLP_PATTERN_BUILD_SIN_PATTERN_EXPORT_BTN, TRUE);

	JetAPI::EnableCtrlWnd(this, TIDLP_PATTERN_SEQUENCE_VALIDATE_BTN, FALSE);
	JetAPI::EnableCtrlWnd(this, TIDLP_PATTERN_SEQUENCE_PLAY_BTN, FALSE);
	JetAPI::EnableCtrlWnd(this, TIDLP_PATTERN_SEQUENCE_PAUSE_BTN, FALSE);
	JetAPI::EnableCtrlWnd(this, TIDLP_PATTERN_SEQUENCE_STOP_BTN, FALSE);

#ifdef LIGHT_3D_TI_DLP_USE_V2
	JetAPI::EnableCtrlWnd(this, TIDLP_PATTERN_TRIGGER_PERIOD_LABEL2, FALSE);
	JetAPI::EnableCtrlWnd(this, TIDLP_PATTERN_EXPOSURE_TIME_LABEL2, FALSE);
	JetAPI::EnableCtrlWnd(this, TIDLP_PATTERN_TRIGGER_PERIOD_EDIT2, FALSE);
	JetAPI::EnableCtrlWnd(this, TIDLP_PATTERN_EXPOSURE_TIME_EDIT2, FALSE);
#endif//LIGHT_3D_TI_DLP_USE_V2
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());

	LIGHT_3D_CAST_ID Light3DID=(LIGHT_3D_CAST_ID)JetAPI::GetComboxCurSelData(m_Light3DCastIDCombox);
	m_Light3DCastPtr = Light3DCtrl.GetLight3DCastPtr(Light3DID);
	this->UpdatePhasePtrToUI();
	this->StartTiDlpTimer();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLPWnd::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLPWnd::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_LIGHT_3D_TIDLP_WND");
	//---------------------------------------------------------------------------------//
	WndID = IDD_LIGHT_3D_TIDLP_WND;
	WndKey = _T("IDD_LIGHT_3D_TIDLP_WND");
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
	WndID = TIDLP_PROJECT_ID_LABLE;
	WndKey = _T("TIDLP_PROJECT_ID_LABLE");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = TIDLP_CONNECT_BTN;
	WndKey = _T("TIDLP_CONNECT_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = TIDLP_DISCONNECT_BTN;
	WndKey = _T("TIDLP_DISCONNECT_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = TIDLP_RESET_BTN;
	WndKey = _T("TIDLP_RESET_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = TIDLP_RETRIEVE_BTN;
	WndKey = _T("TIDLP_RETRIEVE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = TIDLP_DLP_STATUS_GROUP;
	WndKey = _T("TIDLP_DLP_STATUS_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = TIDLP_STATUS_INITIAL_DONE_CHK;
	WndKey = _T("TIDLP_STATUS_INITIAL_DONE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = TIDLP_STATUS_FORCED_SWAP_CHK;
	WndKey = _T("TIDLP_STATUS_FORCED_SWAP_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = TIDLP_STATUS_BUFFER_FREEZE_CHK;
	WndKey = _T("TIDLP_STATUS_BUFFER_FREEZE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = TIDLP_STATUS_SEQ_RUNNING_CHK;
	WndKey = _T("TIDLP_STATUS_SEQ_RUNNING_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = TIDLP_STATUS_SEQ_ERROR_CHK;
	WndKey = _T("TIDLP_STATUS_SEQ_ERROR_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	
	WndID = TIDLP_STATUS_SEQ_ABORT_CHK;
	WndKey = _T("TIDLP_STATUS_SEQ_ABORT_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = TIDLP_STATUS_DRC_ERROR_CHK;
	WndKey = _T("TIDLP_STATUS_DRC_ERROR_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = TIDLP_STATUS_DMD_PARKED_CHK;
	WndKey = _T("TIDLP_STATUS_DMD_PARKED_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = TIDLP_STATUS_AUTO_UPDATE_CHK;
	WndKey = _T("TIDLP_STATUS_AUTO_UPDATE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		
	//---------------------------------------------------------------------------------//
	WndID = TIDLP_LED_CURRENT_GROUP;
	WndKey = _T("TIDLP_LED_CURRENT_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		
	
	WndID = TIDLP_LED_AUTO_MODE_CHK;
	WndKey = _T("TIDLP_LED_AUTO_MODE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = TIDLP_LED_ENABLE_RED_CHK;
	WndKey = _T("TIDLP_LED_ENABLE_RED_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = TIDLP_LED_ENABLE_GREEN_CHK;
	WndKey = _T("TIDLP_LED_ENABLE_GREEN_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = TIDLP_LED_ENABLE_BLUE_CHK;
	WndKey = _T("TIDLP_LED_ENABLE_BLUE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = TIDLP_LED_INVERT_PWM_CHK;
	WndKey = _T("TIDLP_LED_INVERT_PWM_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = TIDLP_LED_CURRENT_RED_LABEL;
	WndKey = _T("TIDLP_LED_CURRENT_RED_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = TIDLP_LED_CURRENT_GREEN_LABEL;
	WndKey = _T("TIDLP_LED_CURRENT_GREEN_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = TIDLP_LED_CURRENT_BLUE_LABEL;
	WndKey = _T("TIDLP_LED_CURRENT_BLUE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = TIDLP_LED_CURRENT_SET_BTN;
	WndKey = _T("TIDLP_LED_CURRENT_SET_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = TIDLP_LED_CURRENT_OFF_BTN;
	WndKey = _T("TIDLP_LED_CURRENT_OFF_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = TIDLP_LED_CURRENT_ID_LABEL;
	WndKey = _T("TIDLP_LED_CURRENT_ID_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//
	WndID = TIDLP_PATTERN_CONFIG_GROUP;
	WndKey = _T("TIDLP_PATTERN_CONFIG_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = TIDLP_PATTERN_LED_COLOR_LABEL;
	WndKey = _T("TIDLP_PATTERN_LED_COLOR_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = TIDLP_PATTERN_BIT_DEPTH_LABEL;
	WndKey = _T("TIDLP_PATTERN_BIT_DEPTH_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = TIDLP_PATTERN_TRIGGER_TYPE_LABEL;
	WndKey = _T("TIDLP_PATTERN_TRIGGER_TYPE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	
	//---------------------------------------------------------------------------------//
	WndID = TIDLP_SEQUENCE_CONFIG_GROUP;
	WndKey = _T("TIDLP_SEQUENCE_CONFIG_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = TIDLP_PATTERN_SOURCE_LABEL;
	WndKey = _T("TIDLP_PATTERN_SOURCE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = TIDLP_SEQUENCE_TRIGGER_MODE_LABEL;
	WndKey = _T("TIDLP_SEQUENCE_TRIGGER_MODE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = TIDLP_PATTERN_TRIGGER_PERIOD_LABEL;
	WndKey = _T("TIDLP_PATTERN_TRIGGER_PERIOD_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = TIDLP_PATTERN_EXPOSURE_TIME_LABEL;
	WndKey = _T("TIDLP_PATTERN_EXPOSURE_TIME_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = TIDLP_PATTERN_TRIGGER_PERIOD_LABEL2;
	WndKey = _T("TIDLP_PATTERN_TRIGGER_PERIOD_LABEL2");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = TIDLP_PATTERN_EXPOSURE_TIME_LABEL2;
	WndKey = _T("TIDLP_PATTERN_EXPOSURE_TIME_LABEL2");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = TIDLP_PATTERN_PATTERN_INVERT_CHK;
	WndKey = _T("TIDLP_PATTERN_PATTERN_INVERT_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = TIDLP_TRIGGER_OUT_PREV_CHK;
	WndKey = _T("TIDLP_TRIGGER_OUT_PREV_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = TIDLP_CLEAR_DMD_AFTER_EXPOSURE_CHK;
	WndKey = _T("TIDLP_CLEAR_DMD_AFTER_EXPOSURE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = TIDLP_PATTERN_ADD_BTN;
	WndKey = _T("TIDLP_PATTERN_ADD_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = TIDLP_PATTERN_SEND_BTN;
	WndKey = _T("TIDLP_PATTERN_SEND_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = TIDLP_PATTERN_READ_BTN;
	WndKey = _T("TIDLP_PATTERN_READ_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = TIDLP_PATTERN_CLEAR_BTN;
	WndKey = _T("TIDLP_PATTERN_CLEAR_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//
	WndID = TIDLP_PATTERN_SEQUENCE_REPEAT_CHK;
	WndKey = _T("TIDLP_PATTERN_SEQUENCE_REPEAT_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		
	
	WndID = TIDLP_PATTERN_BUILD_GROUP;
	WndKey = _T("TIDLP_PATTERN_BUILD_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = TIDLP_PATTERN_BUILD_START_PHASE_LABEL;
	WndKey = _T("TIDLP_PATTERN_BUILD_START_PHASE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = TIDLP_PATTERN_BUILD_PIXELS_PERIOD_LABEL;
	WndKey = _T("TIDLP_PATTERN_BUILD_PIXELS_PERIOD_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = TIDLP_PATTERN_BUILD_BIT_DEPTH_LABEL;
	WndKey = _T("TIDLP_PATTERN_BUILD_BIT_DEPTH_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = TIDLP_PATTERN_BUILD_PHASE_SHIFT_COUNT_LABEL;
	WndKey = _T("TIDLP_PATTERN_BUILD_PHASE_SHIFT_COUNT_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = TIDLP_PATTERN_BUILD_EXPORT_BTN;
	WndKey = _T("TIDLP_PATTERN_BUILD_EXPORT_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	
	WndID = TIDLP_SAVE_INI_BTN;
	WndKey = _T("TIDLP_SAVE_INI_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = TIDLP_LOAD_INI_BTN;
	WndKey = _T("TIDLP_LOAD_INI_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	//---------------------------------------------------------------------------------//
	WndID = TIDLP_PATTERN_SEQUENCE_GROUP;
	WndKey = _T("TIDLP_PATTERN_SEQUENCE_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = TIDLP_PATTERN_SEQUENCE_VALIDATE_BTN;
	WndKey = _T("TIDLP_PATTERN_SEQUENCE_VALIDATE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = TIDLP_PATTERN_SEQUENCE_PLAY_BTN;
	WndKey = _T("TIDLP_PATTERN_SEQUENCE_PLAY_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = TIDLP_PATTERN_SEQUENCE_PAUSE_BTN;
	WndKey = _T("TIDLP_PATTERN_SEQUENCE_PAUSE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = TIDLP_PATTERN_SEQUENCE_STOP_BTN;
	WndKey = _T("TIDLP_PATTERN_SEQUENCE_STOP_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = TIDLP_PATTERN_SEQ_ERR_EXP_PER_OOR;
	WndKey = _T("TIDLP_PATTERN_SEQ_ERR_EXP_PER_OOR");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = TIDLP_PATTERN_SEQ_ERR_PAT_NUMBER_OOR;
	WndKey = _T("TIDLP_PATTERN_SEQ_ERR_PAT_NUMBER_OOR");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = TIDLP_PATTERN_SEQ_ERR_TRIG_OUT_OVELAPS;
	WndKey = _T("TIDLP_PATTERN_SEQ_ERR_TRIG_OUT_OVELAPS");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);  

	WndID = TIDLP_PATTERN_SEQ_ERR_BLACK_VECT_MISS;
	WndKey = _T("TIDLP_PATTERN_SEQ_ERR_BLACK_VECT_MISS");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = TIDLP_PATTERN_SEQ_ERR_EXP_PER_TIME_DIF;
	WndKey = _T("TIDLP_PATTERN_SEQ_ERR_EXP_PER_TIME_DIF");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = TIDLP_PATTERN_SEQ_ERR_SEQUENCE_VALIDATED;
	WndKey = _T("TIDLP_PATTERN_SEQ_ERR_SEQUENCE_VALIDATED");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		
	//---------------------------------------------------------------------------------//	
	WndID = TIDLP_PATTERN_EXP_NUM_LABEL;
	WndKey = _T("TIDLP_PATTERN_EXP_NUM_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		
	//---------------------------------------------------------------------------------//	

	/*
	WndID = TTTTTTTTTTTTTTTTTT;
	WndKey = _T("TTTTTTTTTTTTTTTTTT");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	
	WndID = TTTTTTTTTTTTTTTTTT;
	WndKey = _T("TTTTTTTTTTTTTTTTTT");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = TTTTTTTTTTTTTTTTTT;
	WndKey = _T("TTTTTTTTTTTTTTTTTT");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	
	WndID = TTTTTTTTTTTTTTTTTT;
	WndKey = _T("TTTTTTTTTTTTTTTTTT");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = TTTTTTTTTTTTTTTTTT;
	WndKey = _T("TTTTTTTTTTTTTTTTTT");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	
	WndID = TTTTTTTTTTTTTTTTTT;
	WndKey = _T("TTTTTTTTTTTTTTTTTT");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	*/
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLPWnd::BuildPatternBuildPhaseShiftCombox(CComboBox &Combox)
{
	CString str;
	int     idx=0;		
	int     Param=0;

	JetAPI::ClearCombox(Combox);
	
	Param = 3;
	str = _T("3-Phase");
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;

	
	Param = 4;
	str = _T("4-Phase");		
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;

	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLPWnd::BuildPatternBuildPhaseDirectionCombox(CComboBox &Combox)
{
	CString str;
	int     idx=0;		
	int     Param=0;

	JetAPI::ClearCombox(Combox);
	
	Param = 1;
	str = _T("Vertical");
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;
	
	Param = 2;
	str = _T("Horizontal");
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;

	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLPWnd::CheckPhasePtr()
{
	if ( NULL == m_Light3DCastPtr ) 
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLPWnd::GetLEDCurrentID()
{
	int val = JetAPI::GetComboxCurSelData(m_LEDCurrentIDCombox);
	return val;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLPWnd::OnSelchangeProjectIDCombo() 
{
	// TODO: Add your control notification handler code here
	LIGHT_3D_CAST_ID Light3DID=(LIGHT_3D_CAST_ID)JetAPI::GetComboxCurSelData(m_Light3DCastIDCombox);
	m_Light3DCastPtr = Light3DCtrl.GetLight3DCastPtr(Light3DID);
	this->UpdatePhasePtrToUI();	
	this->m_PatternListWnd.DeleteAllItems();

	JetAPI::EnableCtrlWnd(this, TIDLP_PATTERN_SEQUENCE_VALIDATE_BTN, FALSE);
	JetAPI::EnableCtrlWnd(this, TIDLP_PATTERN_SEQUENCE_PLAY_BTN, FALSE);
	JetAPI::EnableCtrlWnd(this, TIDLP_PATTERN_SEQUENCE_PAUSE_BTN, FALSE);
	JetAPI::EnableCtrlWnd(this, TIDLP_PATTERN_SEQUENCE_STOP_BTN, FALSE);
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLPWnd::UpdatePhasePtrToUI()
{
	if ( this->CheckPhasePtr() == false ) { return false; }
	
	CWnd *pWnd = NULL;
	BOOL  bEnable = TRUE;
	if ( m_Light3DCastPtr->GetDLPIsConnected() == true )
	{	bEnable = FALSE;	}
	else
	{	bEnable = TRUE;	}	
	pWnd = this->GetDlgItem(TIDLP_CONNECT_BTN);
	if ( NULL!=pWnd && NULL!=pWnd->GetSafeHwnd() )
	{	pWnd->EnableWindow(bEnable); }
	
	bEnable = !bEnable;
	pWnd = this->GetDlgItem(TIDLP_DISCONNECT_BTN);
	if ( NULL!=pWnd && NULL!=pWnd->GetSafeHwnd() )
	{	pWnd->EnableWindow(bEnable); }

	CString verDLP;
	CString tagFrm = m_Light3DCastPtr->GetDLPFrmTag();
	CString verLib = m_Light3DCastPtr->GetTiAPIVersion();
	CString verFrm = m_Light3DCastPtr->GetDLPFrmVersion();	
	CString verMcu = m_Light3DCastPtr->GetDLPMcuVersion();

	verDLP.Format(_T("Ti:%s; Frm:%s; Tag:%s; Mcu:%s"), verLib, verFrm, tagFrm, verMcu);		
	CWnd::SetDlgItemText(TIDLP_VERSION_INFO_EDIT, verDLP);

	JetAPI::SetComboxCurSel(m_OperationModeCombox, m_Light3DCastPtr->GetDLPOperationMode());	

	int Red = 0, Green = 0, Blue = 0;
	const int CurrentID = GetLEDCurrentID();
	const TDLPParam &DLPParam = m_Light3DCastPtr->GetDLPParam();	
	m_Light3DCastPtr->GetDLPParamLEDCurrent(Red, Green, Blue, CurrentID);

	this->SetDlgItemInt(TIDLP_LED_CURRENT_RED_EDIT, Red);
	this->SetDlgItemInt(TIDLP_LED_CURRENT_GREEN_EDIT, Green);
	this->SetDlgItemInt(TIDLP_LED_CURRENT_BLUE_EDIT, Blue);

	if ( true == DLPParam.m_LEDEnabled_Auto )
	{	this->CheckDlgButton(TIDLP_LED_AUTO_MODE_CHK, TRUE);	}
	else
	{	this->CheckDlgButton(TIDLP_LED_AUTO_MODE_CHK, FALSE);	}
	this->OnLEDAutoModeChk();

	if ( true == DLPParam.m_LEDEnabled_R )
	{	this->CheckDlgButton(TIDLP_LED_ENABLE_RED_CHK, TRUE);	}
	else
	{	this->CheckDlgButton(TIDLP_LED_ENABLE_RED_CHK, FALSE);	}

	if ( true == DLPParam.m_LEDEnabled_G )
	{	this->CheckDlgButton(TIDLP_LED_ENABLE_GREEN_CHK, TRUE);	}
	else
	{	this->CheckDlgButton(TIDLP_LED_ENABLE_GREEN_CHK, FALSE);	}

	if ( true == DLPParam.m_LEDEnabled_B )
	{	this->CheckDlgButton(TIDLP_LED_ENABLE_BLUE_CHK, TRUE);	}
	else
	{	this->CheckDlgButton(TIDLP_LED_ENABLE_BLUE_CHK, FALSE);	}

	if ( true == DLPParam.m_InvertPWM )
	{	this->CheckDlgButton(TIDLP_LED_INVERT_PWM_CHK, TRUE);	}
	else
	{	this->CheckDlgButton(TIDLP_LED_INVERT_PWM_CHK, FALSE);	}	

	JetAPI::SetComboxCurSel(this->m_PatternLEDColorCombox, DLPParam.m_LEDColor);
	JetAPI::SetComboxCurSel(this->m_PatternBitDepthCombox, DLPParam.m_PatternBitDepth);	
	JetAPI::SetComboxCurSel(this->m_PatternTriggerTypeCombox, DLPParam.m_TrigType);	

	this->SetDlgItemInt(TIDLP_PATTERN_TRIGGER_PERIOD_EDIT, DLPParam.m_PeriodTime_us);	
	this->SetDlgItemInt(TIDLP_PATTERN_EXPOSURE_TIME_EDIT, DLPParam.m_ExposureTime_us);

	this->SetDlgItemInt(TIDLP_PATTERN_TRIGGER_PERIOD_EDIT2, DLPParam.m_PeriodTime2_us);	
	this->SetDlgItemInt(TIDLP_PATTERN_EXPOSURE_TIME_EDIT2, DLPParam.m_ExposureTime2_us);

	const int BitDepth = JetAPI::GetComboxCurSelData(m_PatternBitDepthCombox);
	AOIDataDefine.BuildDLPPatternBitRangeCombox(BitDepth, m_PatternBitRangeCombox);	
	m_PatternBitRangeCombox.SetCurSel(0);
	
	return true;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLPWnd::OnConnectBtn() 
{
	// TODO: Add your control notification handler code here
	if ( this->CheckPhasePtr() == false ) { return; }
	if ( m_Light3DCastPtr->DLPConnect() == false )
	{
		JetAPI::ShowMessageBox(m_Light3DCastPtr->GetErrorString());
		return;
	}
	this->UpdatePhasePtrToUI();
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLPWnd::OnDisconnectBtn() 
{
	// TODO: Add your control notification handler code here
	if ( this->CheckPhasePtr() == false ) { return; }
	if ( m_Light3DCastPtr->DLPDisconnect() == false )
	{
		JetAPI::ShowMessageBox(m_Light3DCastPtr->GetErrorString());		
		return;
	}
	this->UpdatePhasePtrToUI();
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLPWnd::OnPatternBuildExportBtn() 
{
	// TODO: Add your control notification handler code here	
	if ( this->CheckPhasePtr() == false ) { return; }
	IMAGE_SIZE ImageW = m_Light3DCastPtr->GetDLPImageW();
	IMAGE_SIZE ImageH = m_Light3DCastPtr->GetDLPImageH();
	IMAGE_SIZE BitCount = 24;
	IMAGE_SIZE ImageStep = JetAPI::GetBMPImagePixelsPerLine(ImageW, BitCount, 4);
	
	CString  strDir, strPixelPeriod, strBitDepth;
	CString  filename_base, filename, filename2;	
	bool     bVer = false;	
	const int StartPhase = this->GetDlgItemInt(TIDLP_PATTERN_BUILD_START_PHASE_EDIT);
	const int NPixelsPeriod = this->GetDlgItemInt(TIDLP_PATTERN_BUILD_PIXELS_PERIOD_EDIT);
	const int NPeriod = JetAPI::GetComboxCurSelData(m_PatternBuildPhaseShiftCountCombo);
	const int BitDepth = JetAPI::GetComboxCurSelData(m_PatternBuildBitDepthCombox);
	const int DireNum = JetAPI::GetComboxCurSelData(m_PatternBuildPhaseShiftDirectionCombox);
	BOOL      bExportSinPat = this->IsDlgButtonChecked(TIDLP_PATTERN_BUILD_SIN_PATTERN_EXPORT_BTN);

	unsigned char *pImage = NULL;

	TCHAR szFilters[]=_T("BMP Files (*.BMP)|*.BMP|All Files (*.*)|*.*||");
	CFileDialog dialog (FALSE, _T("BMP"), _T("*.BMP"), OFN_FILEMUSTEXIST, szFilters);
	if ( dialog.DoModal() == IDCANCEL )
	{	return ; }

	filename = dialog.GetPathName();	
	
	if ( DireNum == 1 ) 
	{	
		bVer = true; 
		strDir = _T("V");
	}
	else
	{	
		bVer = false; 
		strDir = _T("H");
	}
	switch ( BitDepth )
	{
	case DLP_BIT_DEPTH_1:	strBitDepth=_T("1Bit");	break;
	case DLP_BIT_DEPTH_2:	strBitDepth=_T("2Bit");	break;
	case DLP_BIT_DEPTH_3:	strBitDepth=_T("3Bit");	break;
	case DLP_BIT_DEPTH_4:	strBitDepth=_T("4Bit");	break;
	case DLP_BIT_DEPTH_5:	strBitDepth=_T("5Bit");	break;
	case DLP_BIT_DEPTH_6:	strBitDepth=_T("6Bit");	break;
	case DLP_BIT_DEPTH_7:	strBitDepth=_T("7Bit");	break;
	case DLP_BIT_DEPTH_8:	strBitDepth=_T("8Bit");	break;
	}
	strPixelPeriod.Format(_T("%dP"), NPixelsPeriod);

	const int idx = filename.ReverseFind(_T('.'));
	if ( idx > 0 ) 
	{	filename2 =  filename.Left(idx); }
	else
	{	filename2 =  filename; }
	filename_base.Format(_T("%s_%s_%s_%s"), filename2, strPixelPeriod, strBitDepth, strDir);
	
	if ( TRUE == bExportSinPat )
	{
		const int      P1 = NPixelsPeriod;		
		switch ( NPeriod )
		{
		case 3:
			ImageAPI.GrayImageCreateCosPattern(ImageW, ImageH, ImageStep, pImage, P1, StartPhase+0, bVer);		
			filename2.Format(_T("%s_000.BMP"), filename_base);
			ImageAPI.SaveBMPGrayImage(filename2, ImageW, ImageH, ImageStep, pImage, true);
			ImageAPI.GrayImageCreateCosPattern(ImageW, ImageH, ImageStep, pImage, P1, StartPhase+120, bVer);		
			filename2.Format(_T("%s_120.BMP"), filename_base);
			ImageAPI.SaveBMPGrayImage(filename2, ImageW, ImageH, ImageStep, pImage, true);
			ImageAPI.GrayImageCreateCosPattern(ImageW, ImageH, ImageStep, pImage, P1, StartPhase+240, bVer);		
			filename2.Format(_T("%s_240.BMP"), filename_base);
			ImageAPI.SaveBMPGrayImage(filename2, ImageW, ImageH, ImageStep, pImage, true);
			break;
		case 4:
			ImageAPI.GrayImageCreateCosPattern(ImageW, ImageH, ImageStep, pImage, P1, StartPhase+0, bVer);//0		
			filename2.Format(_T("%s_000.BMP"), filename_base);
			ImageAPI.SaveBMPGrayImage(filename2, ImageW, ImageH, ImageStep, pImage, true);
			ImageAPI.GrayImageCreateCosPattern(ImageW, ImageH, ImageStep, pImage, P1, StartPhase+90, bVer);//90		
			filename2.Format(_T("%s_090.BMP"), filename_base);
			ImageAPI.SaveBMPGrayImage(filename2, ImageW, ImageH, ImageStep, pImage, true);
			ImageAPI.GrayImageCreateCosPattern(ImageW, ImageH, ImageStep, pImage, P1, StartPhase+180, bVer);//180		
			filename2.Format(_T("%s_180.BMP"), filename_base);
			ImageAPI.SaveBMPGrayImage(filename2, ImageW, ImageH, ImageStep, pImage, true);
			ImageAPI.GrayImageCreateCosPattern(ImageW, ImageH, ImageStep, pImage, P1, StartPhase+270, bVer);//270		
			filename2.Format(_T("%s_270.BMP"), filename_base);
			ImageAPI.SaveBMPGrayImage(filename2, ImageW, ImageH, ImageStep, pImage, true);
			break;
		case 5:
			ImageAPI.GrayImageCreateCosPattern(ImageW, ImageH, ImageStep, pImage, P1, StartPhase+0, bVer);		
			filename2.Format(_T("%s_000.BMP"), filename_base);
			ImageAPI.SaveBMPGrayImage(filename2, ImageW, ImageH, ImageStep, pImage, true);
			ImageAPI.GrayImageCreateCosPattern(ImageW, ImageH, ImageStep, pImage, P1, StartPhase+90, bVer);		
			filename2.Format(_T("%s_090.BMP"), filename_base);
			ImageAPI.SaveBMPGrayImage(filename2, ImageW, ImageH, ImageStep, pImage, true);
			ImageAPI.GrayImageCreateCosPattern(ImageW, ImageH, ImageStep, pImage, P1, StartPhase+180, bVer);		
			filename2.Format(_T("%s_180.BMP"), filename_base);
			ImageAPI.SaveBMPGrayImage(filename2, ImageW, ImageH, ImageStep, pImage, true);
			ImageAPI.GrayImageCreateCosPattern(ImageW, ImageH, ImageStep, pImage, P1, StartPhase+270, bVer);		
			filename2.Format(_T("%s_270.BMP"), filename_base);
			ImageAPI.SaveBMPGrayImage(filename2, ImageW, ImageH, ImageStep, pImage, true);
			ImageAPI.GrayImageCreateCosPattern(ImageW, ImageH, ImageStep, pImage, P1, StartPhase+0, bVer);		
			filename2.Format(_T("%s_360.BMP"), filename_base);
			ImageAPI.SaveBMPGrayImage(filename2, ImageW, ImageH, ImageStep, pImage, true);
			break;
		}	
	}

	if ( m_Light3DCastPtr->BuildPatternImage(BitDepth, NPeriod, NPixelsPeriod, bVer, ImageW, ImageH, ImageStep, pImage) == false )
	{
		JetAPI::ShowMessageBox(m_Light3DCastPtr->GetErrorString());
		return;
	}
	filename2.Format(_T("%s.BMP"), filename_base);
	if ( ImageAPI.SaveBMPColorImage(filename2, ImageW, ImageH, ImageStep, pImage, true) == false )
	{
		JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
		return ;
	}
	JetMemory.free_func(pImage);

	::ShellExecute(NULL, _T("open"), filename, NULL, NULL, SW_SHOW);	
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLPWnd::OnResetBtn() 
{
	// TODO: Add your control notification handler code here
	if ( this->CheckPhasePtr() == false ) { return; }
	const DWORD delayTime = 10000;
	if ( m_Light3DCastPtr->ExecDLPSoftwareReset(delayTime) == false )
	{
		JetAPI::ShowMessageBox(m_Light3DCastPtr->GetErrorString());
		return;
	}

	this->UpdatePhasePtrToUI();
	JetAPI::EnableCtrlWnd(this, TIDLP_PATTERN_SEQUENCE_VALIDATE_BTN, FALSE);
	JetAPI::EnableCtrlWnd(this, TIDLP_PATTERN_SEQUENCE_PLAY_BTN, FALSE);
	JetAPI::EnableCtrlWnd(this, TIDLP_PATTERN_SEQUENCE_PAUSE_BTN, FALSE);
	JetAPI::EnableCtrlWnd(this, TIDLP_PATTERN_SEQUENCE_STOP_BTN, FALSE);
	JetAPI::ShowMessageBox(AOIDataDefine.GetFinishText());	
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLPWnd::OnLEDCurrentSetBtn() 
{
	// TODO: Add your control notification handler code here
	if ( this->CheckPhasePtr() == false ) { return; }

	bool AutoMode = false;
	bool EnableR = false;
	bool EnableG = false;
	bool EnableB = false;
	bool InverPWM = false;

	const int CurrentID = GetLEDCurrentID();
	const int Red = this->GetDlgItemInt(TIDLP_LED_CURRENT_RED_EDIT);
	const int Grn = this->GetDlgItemInt(TIDLP_LED_CURRENT_GREEN_EDIT);
	const int Blu = this->GetDlgItemInt(TIDLP_LED_CURRENT_BLUE_EDIT);	
	
	if ( this->IsDlgButtonChecked(TIDLP_LED_AUTO_MODE_CHK) == TRUE )
	{	AutoMode = true; }
	else
	{	AutoMode = false; }

	if ( this->IsDlgButtonChecked(TIDLP_LED_ENABLE_RED_CHK) == TRUE )
	{	EnableR = true; }
	else
	{	EnableR = false; }

	if ( this->IsDlgButtonChecked(TIDLP_LED_ENABLE_GREEN_CHK) == TRUE )
	{	EnableG = true; }
	else
	{	EnableG = false; }

	if ( this->IsDlgButtonChecked(TIDLP_LED_ENABLE_BLUE_CHK) == TRUE )
	{	EnableB = true; }
	else
	{	EnableB = false; }

	if ( this->IsDlgButtonChecked(TIDLP_LED_INVERT_PWM_CHK) == TRUE )
	{	InverPWM = true; }
	else
	{	InverPWM = false; }	

	if ( m_Light3DCastPtr->SetDLPLEDEnable(AutoMode, EnableR, EnableG, EnableB) == false )
	{
		JetAPI::ShowMessageBox(m_Light3DCastPtr->GetErrorString());		
		return;
	}	

	if ( m_Light3DCastPtr->SetDLPLEDPWMInvert(InverPWM) == false )
	{
		JetAPI::ShowMessageBox(m_Light3DCastPtr->GetErrorString());		
		return;
	}	
	
	const bool bUpdate = true;
	if ( m_Light3DCastPtr->SetDLPLEDCurrent(Red, Grn, Blu, bUpdate, CurrentID) == false )
	{
		JetAPI::ShowMessageBox(m_Light3DCastPtr->GetErrorString());		
		return;
	}	
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLPWnd::OnLEDCurrentOffBtn() 
{
	// TODO: Add your control notification handler code here
	if ( this->CheckPhasePtr() == false ) { return; }
	if ( m_Light3DCastPtr->SetDLPLEDEnable(false, false, false, false) == false )
	{
		JetAPI::ShowMessageBox(m_Light3DCastPtr->GetErrorString());		
		return;
	}
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLPWnd::OnSaveIniBtn() 
{
	// TODO: Add your control notification handler code here
	if ( this->CheckPhasePtr() == false ) { return; }
	bool IsLockSystemParam = AOIDataCollect.CheckIsLockSystemParameter();	
	if ( true == IsLockSystemParam )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return; 
	}
	if ( m_Light3DCastPtr->SaveDLPParameter() == false )
	{
		JetAPI::ShowMessageBox(m_Light3DCastPtr->GetErrorString());
		return;
	}
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLPWnd::OnLoadIniBtn() 
{
	// TODO: Add your control notification handler code here
	if ( this->CheckPhasePtr() == false ) { return; }
	if ( m_Light3DCastPtr->LoadDLPParameter() == false )
	{
		JetAPI::ShowMessageBox(m_Light3DCastPtr->GetErrorString());
		return;
	}
	this->UpdatePhasePtrToUI();
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLPWnd::OnLEDAutoModeChk() 
{
	// TODO: Add your control notification handler code here
	BOOL  bEnable = TRUE;
	CWnd *pWnd = NULL;
	BOOL  bCheck = this->IsDlgButtonChecked(TIDLP_LED_AUTO_MODE_CHK);

	if ( bCheck == TRUE ) { bEnable = FALSE; }
	else { bEnable = TRUE; }

	pWnd = this->GetDlgItem(TIDLP_LED_ENABLE_RED_CHK);
	if ( NULL!=pWnd && NULL!=pWnd->GetSafeHwnd() )
	{	pWnd->EnableWindow(bEnable); }
	pWnd = this->GetDlgItem(TIDLP_LED_ENABLE_GREEN_CHK);
	if ( NULL!=pWnd && NULL!=pWnd->GetSafeHwnd() )
	{	pWnd->EnableWindow(bEnable); }
	pWnd = this->GetDlgItem(TIDLP_LED_ENABLE_BLUE_CHK);
	if ( NULL!=pWnd && NULL!=pWnd->GetSafeHwnd() )
	{	pWnd->EnableWindow(bEnable); }
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLPWnd::OnTimer(UINT_PTR nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	BOOL bCheck = FALSE;
	this->KillTimer(nIDEvent);
	switch ( nIDEvent )
	{
	case PHASE_TI_DLP_TIMER_ID:
		this->UpdatePhasePtrStatusToUI();
		bCheck = this->IsDlgButtonChecked(TIDLP_STATUS_AUTO_UPDATE_CHK);
		if ( TRUE == bCheck )
		{	this->StartTiDlpTimer(); }
		break;
	}
	CDialog::OnTimer(nIDEvent);
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLPWnd::StartTiDlpTimer()
{
	CWnd::SetTimer(PHASE_TI_DLP_TIMER_ID, 100, NULL);	
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLPWnd::KillTiDlpTimer()
{
	this->KillTimer(PHASE_TI_DLP_TIMER_ID);
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLPWnd::UpdatePhasePtrStatusToUI()
{
	if ( this->CheckPhasePtr() == false ) { return false; }
	UINT CtrlID = 0;
	bool bCheck = true;	

	CtrlID = TIDLP_STATUS_INITIAL_DONE_CHK;
	if ( m_Light3DCastPtr->GetDLPStatus_InitDone() == true )
	{	this->CheckDlgButton(CtrlID, BST_CHECKED); }
	else
	{	this->CheckDlgButton(CtrlID, BST_UNCHECKED); }

	CtrlID = TIDLP_STATUS_FORCED_SWAP_CHK;
	if ( m_Light3DCastPtr->GetDLPStatus_ForcedSwap() == true )
	{	this->CheckDlgButton(CtrlID, BST_CHECKED); }
	else
	{	this->CheckDlgButton(CtrlID, BST_UNCHECKED); }

	CtrlID = TIDLP_STATUS_BUFFER_FREEZE_CHK;
	if ( m_Light3DCastPtr->GetDLPStatus_BufferFreeze() == true )
	{	this->CheckDlgButton(CtrlID, BST_CHECKED); }
	else
	{	this->CheckDlgButton(CtrlID, BST_UNCHECKED); }

	CtrlID = TIDLP_STATUS_SEQ_RUNNING_CHK;
	if ( m_Light3DCastPtr->GetDLPStatus_SeqRunning() == true )
	{	this->CheckDlgButton(CtrlID, BST_CHECKED); }
	else
	{	this->CheckDlgButton(CtrlID, BST_UNCHECKED); }

	CtrlID = TIDLP_STATUS_SEQ_ERROR_CHK;
	if ( m_Light3DCastPtr->GetDLPStatus_SeqError() == true )
	{	this->CheckDlgButton(CtrlID, BST_CHECKED); }
	else
	{	this->CheckDlgButton(CtrlID, BST_UNCHECKED); }

	CtrlID = TIDLP_STATUS_SEQ_ABORT_CHK;
	if ( m_Light3DCastPtr->GetDLPStatus_SeqAbort() == true )
	{	this->CheckDlgButton(CtrlID, BST_CHECKED); }
	else
	{	this->CheckDlgButton(CtrlID, BST_UNCHECKED); }

	CtrlID = TIDLP_STATUS_DRC_ERROR_CHK;
	if ( m_Light3DCastPtr->GetDLPStatus_DRCError() == true )
	{	this->CheckDlgButton(CtrlID, BST_CHECKED); }
	else
	{	this->CheckDlgButton(CtrlID, BST_UNCHECKED); }

	CtrlID = TIDLP_STATUS_DMD_PARKED_CHK;
	if ( m_Light3DCastPtr->GetDLPStatus_DMDParked() == true )
	{	this->CheckDlgButton(CtrlID, BST_CHECKED); }
	else
	{	this->CheckDlgButton(CtrlID, BST_UNCHECKED); }
	return true;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLPWnd::OnStatusAutoUpdateChk() 
{
	// TODO: Add your control notification handler code here
	BOOL bCheck = this->IsDlgButtonChecked(TIDLP_STATUS_AUTO_UPDATE_CHK);
	if ( TRUE == bCheck )
	{	this->StartTiDlpTimer(); }
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLPWnd::OnSelchangeOperationModeCombo() 
{
	// TODO: Add your control notification handler code here
	if ( this->CheckPhasePtr() == false ) { return ; }
	int OperationMode = JetAPI::GetComboxCurSelData(m_OperationModeCombox);
	if ( m_Light3DCastPtr->SetDLPOperationMode(OperationMode) == false )
	{
		JetAPI::ShowMessageBox(m_Light3DCastPtr->GetErrorString());
		return;
	}
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLPWnd::OnSelchangePatternBitDepthCombo() 
{
	// TODO: Add your control notification handler code here
	const int BitDepth = JetAPI::GetComboxCurSelData(m_PatternBitDepthCombox);
	AOIDataDefine.BuildDLPPatternBitRangeCombox(BitDepth, m_PatternBitRangeCombox);		
	m_PatternBitRangeCombox.SetCurSel(0);
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLPWnd::OnPatternAddBtn() 
{
	// TODO: Add your control notification handler code here	
	if ( this->CheckPhasePtr() == false ) { return ; }

	int i=0;
	int PreFlashIdx=0;
	bool bufSwap=false;  
	CString str;
	CString ItemText;
	int PatternSequenceMode = JetAPI::GetComboxCurSelData(m_PatternSequenceModeCombox);
	
	int TriggerType = 0;	
	const bool IntTrig = false;
	const bool DLPMultiTalbe = LightCtrlBoard.GetDLPMultiTableEnabled();
	const int NItemCount = this->m_PatternListWnd.GetItemCount();	
	const int FlashIdx = JetAPI::GetComboxCurSelData(m_PatternFlashIndexCombox);
	const int LEDIndex = JetAPI::GetComboxCurSelData(m_PatternLEDColorCombox);
	const int BitDepth = JetAPI::GetComboxCurSelData(m_PatternBitDepthCombox);
	const int BitPos = JetAPI::GetComboxCurSelData(m_PatternBitRangeCombox);//firstItem, lastItem	
	const int PatternSource = JetAPI::GetComboxCurSelData(m_PatternSouceCombox);
	const int OperationMode = JetAPI::GetComboxCurSelData(m_OperationModeCombox);
	const int PatternExpNum = JetAPI::GetComboxCurSelData(m_PatternExpNumCombox);
	const bool TrigOutPrev = (bool)(this->IsDlgButtonChecked(TIDLP_TRIGGER_OUT_PREV_CHK));
	const bool InvertPattern = (bool)(this->IsDlgButtonChecked(TIDLP_PATTERN_PATTERN_INVERT_CHK));
	const bool InsertBlack = (bool)(this->IsDlgButtonChecked(TIDLP_CLEAR_DMD_AFTER_EXPOSURE_CHK));
	const int PatNum = BitPos/BitDepth;	
	const int PatPeriod = CWnd::GetDlgItemInt(TIDLP_PATTERN_TRIGGER_PERIOD_EDIT);
	const int PatExpose = CWnd::GetDlgItemInt(TIDLP_PATTERN_EXPOSURE_TIME_EDIT);
	const int PatPeriod2 = CWnd::GetDlgItemInt(TIDLP_PATTERN_TRIGGER_PERIOD_EDIT2);
	const int PatExpose2 = CWnd::GetDlgItemInt(TIDLP_PATTERN_EXPOSURE_TIME_EDIT2);

	if ( DLP_PATTERN_SEQUENCE_4_4_M == PatternSequenceMode )
	{
		//DLP_PATTERN_SEQUENCE_4_2_M
		if ( DLP_PATTERN_EXP_NUM_02 == PatternExpNum )
		{	PatternSequenceMode = DLP_PATTERN_SEQUENCE_4_4_M_2;	}
	}

	if ( DLP_OPERATION_PATTERN_EXP == OperationMode )
	{
		//Make sure the Pattern Exposure and Pattern Period timings are within the spec
		//Don't allow Pattern Exposure > Pattern Period
		if ( PatExpose > PatPeriod )
		{	
			str = _T("Pattern exposure setting voilation, it should be, Pattern Exposure = Pattern Period or (Pattern Period - Pattern Exposure) > 230us");
			JetAPI::ShowMessageBox(str);
			return;
		}
		//If Pattern Exposure != Pattern Period then (Pattern Period - Pattern Exposure) > 230us
		if( (PatExpose!=PatPeriod) && (PatPeriod-PatExpose) <= 230 )
		{
			str = _T("Pattern exposure setting voilation, it should be, Pattern Exposure = Pattern Period or (Pattern Period - Pattern Exposure) > 230us");
			JetAPI::ShowMessageBox(str);
			return;
		}
		m_Light3DCastPtr->GetDLPParam().m_PeriodTime_us = PatPeriod;
		m_Light3DCastPtr->GetDLPParam().m_ExposureTime_us = PatExpose;
		m_Light3DCastPtr->GetDLPParam().m_PeriodTime2_us = PatPeriod2;
		m_Light3DCastPtr->GetDLPParam().m_ExposureTime2_us = PatExpose2;
	}
	else
	{
		m_Light3DCastPtr->GetDLPParam().m_PeriodTime2_us = PatPeriod;
		m_Light3DCastPtr->GetDLPParam().m_ExposureTime2_us = PatExpose;
	}
    //pat_num = firstItem/(ui->BitDepth_combobox->currentIndex()+1);//Update the Data of the listItem with all relevant information for retrieval later.

    //If first item
	if ( DLP_PATTERN_SEQUENCE_DEBUG == PatternSequenceMode )
	{
		if ( NItemCount == 0)
		{
			if(DLP_PATTERN_SOURCE_FLASH==PatternSource)
			{			
				//Trigger type can't be "No Trigger"
				TriggerType = JetAPI::GetComboxCurSelData(m_PatternTriggerTypeCombox);
				if ( TriggerType == DLP_LED_TRIGGER_NO_INPUT)
				{
					str.Format(_T("First Item must be triggered. Please select a Trigger_In_Type other than No Trigger"));
					JetAPI::ShowMessageBox(str);
					return;
				}
				bufSwap = true;//false;
			}
			else
			{
				//In streaming mode, first item has to be triggered by vsync
				bufSwap = true;
			}
		}
		else
		{
			ItemText = this->m_PatternListWnd.GetItemText(NItemCount-1, 2);
			PreFlashIdx = ::_ttoi(ItemText);
			if ( PreFlashIdx != FlashIdx )
			{	bufSwap = true;	}
			else
			{   bufSwap = false;	}
		}

		if( DLP_PATTERN_SOURCE_FLASH==PatternSource )
		{
			TriggerType = JetAPI::GetComboxCurSelData(m_PatternTriggerTypeCombox);        
		}
		else
		{
			if ( bufSwap == true )
			{	TriggerType = DLP_TRIG_TYPE_EXT_POS; }
			else
			{	TriggerType = DLP_TRIG_TYPE_NO_TRIG; }
		}

		TDLPPatItem  PatItem;
		PatItem.sFlashIndex = FlashIdx;
		PatItem.sBitDepth = BitDepth;
		PatItem.sColorIndex = LEDIndex;
		PatItem.sTrigType = TriggerType;
		PatItem.sBitNum = PatNum;	
		PatItem.sInvertPattern = InvertPattern;
		PatItem.sInsertBlack = InsertBlack;
		PatItem.sTrigOutPrev = TrigOutPrev;
		PatItem.sBufSwap = bufSwap;
		PatItem.nExposure = PatExpose;
		PatItem.nPeriod = PatPeriod;

		m_Light3DCastPtr->AddDLPPPatItem(PatItem);
		this->AddPatternItem(NItemCount, PatItem);
	}
	else
	{		
		TDLPPatItem *PatItemPtr=NULL;
		m_Light3DCastPtr->BuildDLPPatternList(PatternSequenceMode, IntTrig, DLPMultiTalbe, LEDIndex);
		this->m_PatternListWnd.DeleteAllItems();
		const size_t PatCount = m_Light3DCastPtr->GetDLPPatCount();
		for(i=0; i<PatCount; i++)
		{
			PatItemPtr = m_Light3DCastPtr->GetDLPPatItemPtr(i, false);
			if ( NULL == PatItemPtr ) { continue; }
			this->AddPatternItem(i, *PatItemPtr);	
		}
	}
	JetAPI::EnableCtrlWnd(this, TIDLP_PATTERN_SEQUENCE_VALIDATE_BTN, FALSE);
	JetAPI::EnableCtrlWnd(this, TIDLP_PATTERN_SEQUENCE_PLAY_BTN, FALSE);
	JetAPI::EnableCtrlWnd(this, TIDLP_PATTERN_SEQUENCE_PAUSE_BTN, FALSE);
	JetAPI::EnableCtrlWnd(this, TIDLP_PATTERN_SEQUENCE_STOP_BTN, FALSE);
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLPWnd::DoPatternSendBtn()
{
	if ( this->CheckPhasePtr() == false ) { return false; }	
	const unsigned int exposure_us = this->GetDlgItemInt(TIDLP_PATTERN_EXPOSURE_TIME_EDIT);
	const unsigned int TrigPeriod_us = this->GetDlgItemInt(TIDLP_PATTERN_TRIGGER_PERIOD_EDIT);
	const unsigned int Exposure_us = this->GetDlgItemInt(TIDLP_PATTERN_EXPOSURE_TIME_EDIT);
	const int PatFrom = JetAPI::GetComboxCurSelData(m_PatternSouceCombox);
	const int TrigMode = JetAPI::GetComboxCurSelData(m_SequenceTriggerModeCombox);
	const bool bRepeat = (bool)(this->IsDlgButtonChecked(TIDLP_PATTERN_SEQUENCE_REPEAT_CHK));
	const int OperationMode = JetAPI::GetComboxCurSelData(m_OperationModeCombox);	

	bool bPatFrmVideo = false;
	bool bTrigIntExt = false;
	bool bExpLut = false;

	if ( DLP_OPERATION_PATTERN_EXP == OperationMode )
	{	bExpLut = true; }
	else
	{	bExpLut = false; }

	if ( DLP_PATTERN_SOURCE_VIDEO_PORT == PatFrom )
	{	bPatFrmVideo = true; }
	if ( DLP_SEQUENCE_TRIGGER_INT_EXT == TrigMode )
	{	bTrigIntExt = true; }

	if ( m_Light3DCastPtr->ExecDLPPatSendAll(bExpLut, TrigPeriod_us, Exposure_us, bPatFrmVideo, bTrigIntExt, bRepeat) == false )
	{	
		JetAPI::ShowMessageBox(m_Light3DCastPtr->GetErrorString());
		return false; 
	}

	this->CheckDlgButton(TIDLP_PATTERN_SEQ_ERR_SEQUENCE_VALIDATED, BST_CHECKED);
	JetAPI::EnableCtrlWnd(this, TIDLP_PATTERN_SEQUENCE_VALIDATE_BTN, TRUE);
	JetAPI::EnableCtrlWnd(this, TIDLP_PATTERN_SEQUENCE_PLAY_BTN, FALSE);
	JetAPI::EnableCtrlWnd(this, TIDLP_PATTERN_SEQUENCE_PAUSE_BTN, FALSE);
	JetAPI::EnableCtrlWnd(this, TIDLP_PATTERN_SEQUENCE_STOP_BTN, FALSE);
	return true;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLPWnd::OnPatternSendBtn() 
{
	// TODO: Add your control notification handler code here
	this->DoPatternSendBtn();
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLPWnd::OnPatternReadBtn() 
{
	// TODO: Add your control notification handler code here	
	this->m_PatternListWnd.DeleteAllItems();
	if ( this->CheckPhasePtr() == false ) { return ; }

	CString str;
	size_t i=0;
	bool bExpLut = false;
	unsigned int TrigPeriod_us=0, Exposure_us=0;
	bool bPatFromVideo=false, bTrigIntExt=false, bRepeat=false;
	const int OperationMode = JetAPI::GetComboxCurSelData(m_OperationModeCombox);
	if ( DLP_OPERATION_PATTERN_EXP == OperationMode )
	{	bExpLut = true; }
	else
	{	bExpLut = false; }

	if ( m_Light3DCastPtr->ExecDLPPatRead(bExpLut, TrigPeriod_us, Exposure_us, bPatFromVideo, bTrigIntExt, bRepeat) == false )
	{
		JetAPI::ShowMessageBox(m_Light3DCastPtr->GetErrorString());
		return;
	}
	
	//時間
	this->SetDlgItemInt(TIDLP_PATTERN_EXPOSURE_TIME_EDIT, Exposure_us);
	this->SetDlgItemInt(TIDLP_PATTERN_TRIGGER_PERIOD_EDIT, TrigPeriod_us);	

	//觸發模式
	if ( false == bTrigIntExt ) 
	{	JetAPI::SetComboxCurSel(m_SequenceTriggerModeCombox, DLP_SEQUENCE_TRIGGER_VSYNC); }
	else
	{	JetAPI::SetComboxCurSel(m_SequenceTriggerModeCombox, DLP_SEQUENCE_TRIGGER_INT_EXT); }

	//顯示模式	
	if ( false == bPatFromVideo ) 
	{	JetAPI::SetComboxCurSel(m_PatternSouceCombox, DLP_PATTERN_SOURCE_FLASH); }
	else
	{	JetAPI::SetComboxCurSel(m_PatternSouceCombox, DLP_PATTERN_SOURCE_VIDEO_PORT); }

	//樣板組態	
	if ( true == bRepeat )
	{	this->CheckDlgButton(TIDLP_PATTERN_SEQUENCE_REPEAT_CHK, BST_CHECKED); }
	else
	{	this->CheckDlgButton(TIDLP_PATTERN_SEQUENCE_REPEAT_CHK, BST_UNCHECKED); }


	TDLPPatItem *PatItemPtr=NULL;
	const size_t PatCount = m_Light3DCastPtr->GetDLPPatCount();
    for(i=0; i<PatCount; i++)
    {
		PatItemPtr = m_Light3DCastPtr->GetDLPPatItemPtr(i, false);
		if ( NULL == PatItemPtr ) { continue; }
		this->AddPatternItem(i, *PatItemPtr);	
	}

	if ( PatCount == 0 )
	{
		JetAPI::EnableCtrlWnd(this, TIDLP_PATTERN_SEQUENCE_VALIDATE_BTN, FALSE);
		JetAPI::EnableCtrlWnd(this, TIDLP_PATTERN_SEQUENCE_PLAY_BTN, FALSE);
		JetAPI::EnableCtrlWnd(this, TIDLP_PATTERN_SEQUENCE_PAUSE_BTN, FALSE);
		JetAPI::EnableCtrlWnd(this, TIDLP_PATTERN_SEQUENCE_STOP_BTN, FALSE);
	}
	else
	{
		JetAPI::EnableCtrlWnd(this, TIDLP_PATTERN_SEQUENCE_VALIDATE_BTN, TRUE);
		JetAPI::EnableCtrlWnd(this, TIDLP_PATTERN_SEQUENCE_PLAY_BTN, TRUE);
		JetAPI::EnableCtrlWnd(this, TIDLP_PATTERN_SEQUENCE_PAUSE_BTN, TRUE);
		JetAPI::EnableCtrlWnd(this, TIDLP_PATTERN_SEQUENCE_STOP_BTN, TRUE);
	}
	return;	
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLPWnd::OnPatternClearBtn() 
{
	// TODO: Add your control notification handler code here
	if ( this->CheckPhasePtr() == false ) { return ; }	
	if ( m_Light3DCastPtr->ExecDLPPatClear() == false )
	{	
		JetAPI::ShowMessageBox(m_Light3DCastPtr->GetErrorString());
		return ; 
	}
	this->m_PatternListWnd.DeleteAllItems();
	JetAPI::EnableCtrlWnd(this, TIDLP_PATTERN_SEQUENCE_VALIDATE_BTN, FALSE);
	JetAPI::EnableCtrlWnd(this, TIDLP_PATTERN_SEQUENCE_PLAY_BTN, FALSE);
	JetAPI::EnableCtrlWnd(this, TIDLP_PATTERN_SEQUENCE_PAUSE_BTN, FALSE);
	JetAPI::EnableCtrlWnd(this, TIDLP_PATTERN_SEQUENCE_STOP_BTN, FALSE);	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLPWnd::RemovePatternItem(int idx)
{
	int     SubIdx=0;	
	CString ItemText;
	const int nItem = this->m_PatternListWnd.GetItemCount();	
	if ( idx<0 || idx>=nItem ) { return false; }
	this->m_PatternListWnd.DeleteItem(idx);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLPWnd::AddPatternItem(int idx, const TDLPPatItem &PatItem)
{
	int     SubIdx=0;	
	CString ItemText;
	const int nItem = this->m_PatternListWnd.GetItemCount();	

	ItemText.Format(_T("%d"), idx);	
	this->m_PatternListWnd.InsertItem(nItem, ItemText);
		
	SubIdx=0;
	this->m_PatternListWnd.SetItemText(nItem, SubIdx, ItemText); 
	SubIdx++;
	
	ItemText = AOIDataDefine.GetDLPPatternTriggerTypeText(PatItem.sTrigType);
	this->m_PatternListWnd.SetItemText(nItem, SubIdx, ItemText); 
	SubIdx++;
	
	ItemText.Format(_T("%d"), PatItem.sFlashIndex);	
	this->m_PatternListWnd.SetItemText(nItem, SubIdx, ItemText); 
	SubIdx++;

	ItemText.Format(_T("%d"), PatItem.sBitNum);	
	this->m_PatternListWnd.SetItemText(nItem, SubIdx, ItemText); 
	SubIdx++;

	ItemText.Format(_T("%d"), PatItem.sBitDepth);	
	this->m_PatternListWnd.SetItemText(nItem, SubIdx, ItemText); 
	SubIdx++;
	
	int BitPosStart=0, BitPosEnd=0;
	AOIDataDefine.CalcDLPBitPosRange(PatItem.sBitDepth, PatItem.sBitNum, BitPosStart, BitPosEnd);
	if ( PatItem.sBitDepth > 1 )
	{	ItemText.Format(_T("%s-%s"), AOIDataDefine.GetDLPBitPosText(BitPosStart), AOIDataDefine.GetDLPBitPosText(BitPosEnd));	}
	else
	{	ItemText.Format(_T("%s"), AOIDataDefine.GetDLPBitPosText(BitPosStart));	}
	this->m_PatternListWnd.SetItemText(nItem, SubIdx, ItemText); 
	SubIdx++;


	ItemText = AOIDataDefine.GetDLPPatternLEDColorText(PatItem.sColorIndex);	
	this->m_PatternListWnd.SetItemText(nItem, SubIdx, ItemText); 
	SubIdx++;

	if ( true == PatItem.sInvertPattern )
	{	ItemText = _T("Y"); }
	else
	{	ItemText = _T(""); }
	this->m_PatternListWnd.SetItemText(nItem, SubIdx, ItemText); 
	SubIdx++;

	if ( true == PatItem.sInsertBlack )
	{	ItemText = _T("Y"); }
	else
	{	ItemText = _T(""); }
	this->m_PatternListWnd.SetItemText(nItem, SubIdx, ItemText); 
	SubIdx++;

	if ( true == PatItem.sBufSwap )
	{	ItemText = _T("Y"); }
	else
	{	ItemText = _T(""); }
	this->m_PatternListWnd.SetItemText(nItem, SubIdx, ItemText); 
	SubIdx++;

	if ( true == PatItem.sTrigOutPrev )
	{	ItemText = _T("Y"); }
	else
	{	ItemText = _T(""); }
	this->m_PatternListWnd.SetItemText(nItem, SubIdx, ItemText); 
	SubIdx++;	

	ItemText.Format(_T("%d"), PatItem.nPeriod);
	this->m_PatternListWnd.SetItemText(nItem, SubIdx, ItemText); 
	SubIdx++;	

	ItemText.Format(_T("%d"), PatItem.nExposure);
	this->m_PatternListWnd.SetItemText(nItem, SubIdx, ItemText); 
	SubIdx++;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLPWnd::DoPatternSequenceValidateBtn()
{
	if ( this->CheckPhasePtr() == false ) { return false; }	

	unsigned int Status=0;
	if ( m_Light3DCastPtr->ExecDLPValidatePatLutData(Status, true) == false )
	{	
		JetAPI::ShowMessageBox(m_Light3DCastPtr->GetErrorString());
		return false;
	}

	this->CheckDlgButton(TIDLP_PATTERN_SEQ_ERR_EXP_PER_OOR, (Status&BIT0)==BIT0);
	this->CheckDlgButton(TIDLP_PATTERN_SEQ_ERR_PAT_NUMBER_OOR, (Status&BIT1)==BIT1);
	this->CheckDlgButton(TIDLP_PATTERN_SEQ_ERR_TRIG_OUT_OVELAPS, (Status&BIT2)==BIT2);	
	this->CheckDlgButton(TIDLP_PATTERN_SEQ_ERR_BLACK_VECT_MISS, (Status&BIT3)==BIT3);
	this->CheckDlgButton(TIDLP_PATTERN_SEQ_ERR_EXP_PER_TIME_DIF, (Status&BIT4)==BIT4);
	if ( 0 != Status )
	{
		JetAPI::EnableCtrlWnd(this, TIDLP_PATTERN_SEQUENCE_VALIDATE_BTN, TRUE);
		JetAPI::EnableCtrlWnd(this, TIDLP_PATTERN_SEQUENCE_PLAY_BTN, FALSE);
		JetAPI::EnableCtrlWnd(this, TIDLP_PATTERN_SEQUENCE_PAUSE_BTN, FALSE);
		JetAPI::EnableCtrlWnd(this, TIDLP_PATTERN_SEQUENCE_STOP_BTN, FALSE);

		return false;
	}
	JetAPI::EnableCtrlWnd(this, TIDLP_PATTERN_SEQUENCE_VALIDATE_BTN, FALSE);
	JetAPI::EnableCtrlWnd(this, TIDLP_PATTERN_SEQUENCE_PLAY_BTN, TRUE);
	JetAPI::EnableCtrlWnd(this, TIDLP_PATTERN_SEQUENCE_PAUSE_BTN, TRUE);
	JetAPI::EnableCtrlWnd(this, TIDLP_PATTERN_SEQUENCE_STOP_BTN, TRUE);
	return true;	
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLPWnd::OnPatternSequenceValidateBtn() 
{
	// TODO: Add your control notification handler code here
	this->DoPatternSequenceValidateBtn();
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLPWnd::OnPatternSequencePlayBtn() 
{
	// TODO: Add your control notification handler code here
	if ( this->CheckPhasePtr() == false ) { return; }
	//CameraStart();
	if ( m_Light3DCastPtr->ExecDLPPattern_Run() == false )
	{
		JetAPI::ShowMessageBox(m_Light3DCastPtr->GetErrorString());
		return;
	}
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLPWnd::OnPatternSequenceStopBtn() 
{
	// TODO: Add your control notification handler code here
	if ( this->CheckPhasePtr() == false ) { return; }
	if ( m_Light3DCastPtr->ExecDLPPattern_Stop() == false )
	{
		JetAPI::ShowMessageBox(m_Light3DCastPtr->GetErrorString());
		return;
	}
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLPWnd::OnPatternSequencePauseBtn() 
{
	// TODO: Add your control notification handler code here
	if ( this->CheckPhasePtr() == false ) { return; }
	if ( m_Light3DCastPtr->ExecDLPPattern_Pause() == false )
	{
		JetAPI::ShowMessageBox(m_Light3DCastPtr->GetErrorString());
		return;
	}
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLPWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if ( TRUE == bShow )
	{
		this->UpdatePhasePtrToUI();	
		this->m_PatternListWnd.DeleteAllItems();

		JetAPI::EnableCtrlWnd(this, TIDLP_PATTERN_SEQUENCE_VALIDATE_BTN, FALSE);
		JetAPI::EnableCtrlWnd(this, TIDLP_PATTERN_SEQUENCE_PLAY_BTN, FALSE);
		JetAPI::EnableCtrlWnd(this, TIDLP_PATTERN_SEQUENCE_PAUSE_BTN, FALSE);
		JetAPI::EnableCtrlWnd(this, TIDLP_PATTERN_SEQUENCE_STOP_BTN, FALSE);
	}
	else
	{	
		AOIDataCollect.UserLogout_Check();	
	}
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLPWnd::OnRetrieveBtn() 
{
	// TODO: Add your control notification handler code here
	if ( this->CheckPhasePtr() == false ) { return; }
	if ( m_Light3DCastPtr->ReadDLPParameter() == false )
	{
		JetAPI::ShowMessageBox(m_Light3DCastPtr->GetErrorString());
		return;
	}
	this->UpdatePhasePtrToUI();
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLPWnd::OnPatternDelBtn() 
{
	// TODO: Add your control notification handler code here
	if ( this->CheckPhasePtr() == false ) { return ; }
	const int nItem = this->m_PatternListWnd.GetNextItem(-1, LVNI_SELECTED);
	if ( nItem == -1 ) { return; }	

	m_Light3DCastPtr->RemoveDLPPatItem(nItem);
	this->RemovePatternItem(nItem);
	JetAPI::EnableCtrlWnd(this, TIDLP_PATTERN_SEQUENCE_VALIDATE_BTN, FALSE);
	JetAPI::EnableCtrlWnd(this, TIDLP_PATTERN_SEQUENCE_PLAY_BTN, FALSE);
	JetAPI::EnableCtrlWnd(this, TIDLP_PATTERN_SEQUENCE_PAUSE_BTN, FALSE);
	JetAPI::EnableCtrlWnd(this, TIDLP_PATTERN_SEQUENCE_STOP_BTN, FALSE);	
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLPWnd::OnSelchangePatternFlashIndexCombo() 
{
	// TODO: Add your control notification handler code here
	this->m_PatternBitRangeCombox.SetCurSel(0);
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLPWnd::OnDblclkPatternListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	if ( this->CheckPhasePtr() == false ) { return; }
	const int nItem = this->m_PatternListWnd.GetNextItem(-1, LVNI_SELECTED);
	if ( nItem < 0 ) { return; }
	const unsigned int exposure_us = this->GetDlgItemInt(TIDLP_PATTERN_EXPOSURE_TIME_EDIT);
	const unsigned int TrigPeriod_us = this->GetDlgItemInt(TIDLP_PATTERN_TRIGGER_PERIOD_EDIT);
	const unsigned int Exposure_us = this->GetDlgItemInt(TIDLP_PATTERN_EXPOSURE_TIME_EDIT);
	const int PatFrom = JetAPI::GetComboxCurSelData(m_PatternSouceCombox);
	const int TrigMode = JetAPI::GetComboxCurSelData(m_SequenceTriggerModeCombox);
	const bool bRepeat = (bool)(this->IsDlgButtonChecked(TIDLP_PATTERN_SEQUENCE_REPEAT_CHK));
	const int OperationMode = JetAPI::GetComboxCurSelData(m_OperationModeCombox);	

	bool bPatFrmVideo = false;
	bool bTrigIntExt = false;
	bool bExpLut = false;

	if ( DLP_OPERATION_PATTERN_EXP == OperationMode )
	{	bExpLut = true; }
	else
	{	bExpLut = false; }
	if ( DLP_PATTERN_SOURCE_VIDEO_PORT == PatFrom )
	{	bPatFrmVideo = true; }
	if ( DLP_SEQUENCE_TRIGGER_INT_EXT == TrigMode )
	{	bTrigIntExt = true; }
	
	this->OnPatternSequenceStopBtn();

	if ( m_Light3DCastPtr->ExecDLPPatSendOne(bExpLut, nItem, TrigPeriod_us, Exposure_us, bPatFrmVideo, bTrigIntExt, bRepeat) == false )
	{
		JetAPI::ShowMessageBox(m_Light3DCastPtr->GetErrorString());
		return;
	}
	this->CheckDlgButton(TIDLP_PATTERN_SEQ_ERR_SEQUENCE_VALIDATED, BST_CHECKED);
	JetAPI::EnableCtrlWnd(this, TIDLP_PATTERN_SEQUENCE_VALIDATE_BTN, TRUE);
	JetAPI::EnableCtrlWnd(this, TIDLP_PATTERN_SEQUENCE_PLAY_BTN, FALSE);
	JetAPI::EnableCtrlWnd(this, TIDLP_PATTERN_SEQUENCE_PAUSE_BTN, FALSE);
	JetAPI::EnableCtrlWnd(this, TIDLP_PATTERN_SEQUENCE_STOP_BTN, FALSE);
	if ( this->DoPatternSequenceValidateBtn() == false )
	{	return; }

	this->OnPatternSequencePlayBtn();
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLPWnd::OnPatternSequencePlayOnceBtn() 
{
	// TODO: Add your control notification handler code here
	if ( this->CheckPhasePtr() == false ) { return; }

	this->OnPatternSequenceStopBtn();
	this->CheckDlgButton(TIDLP_PATTERN_SEQUENCE_REPEAT_CHK, FALSE);
	this->OnLEDCurrentSetBtn();
	if ( this->DoPatternSendBtn() == false )
	{	return; }
	if( this->DoPatternSequenceValidateBtn() == false )
	{	return; }

	//CameraStart();
	if ( m_Light3DCastPtr->ExecDLPPattern_Run() == false )
	{
		JetAPI::ShowMessageBox(m_Light3DCastPtr->GetErrorString());
		return;
	}
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLPWnd::OnPatternSequencePlayRepeatBtn() 
{
	// TODO: Add your control notification handler code here
	if ( this->CheckPhasePtr() == false ) { return; }

	this->OnPatternSequenceStopBtn();
	this->CheckDlgButton(TIDLP_PATTERN_SEQUENCE_REPEAT_CHK, TRUE);
	this->OnLEDCurrentSetBtn();
	if ( this->DoPatternSendBtn() == false )
	{	return; }
	if( this->DoPatternSequenceValidateBtn() == false )
	{	return; }

	//CameraStart();
	if ( m_Light3DCastPtr->ExecDLPPattern_Run() == false )
	{
		JetAPI::ShowMessageBox(m_Light3DCastPtr->GetErrorString());
		return;
	}
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLPWnd::OnSelchangeLEDCurrentIDCombo()
{
	UpdatePhasePtrToUI();
}
//-------------------------------------------------------------------------------------//