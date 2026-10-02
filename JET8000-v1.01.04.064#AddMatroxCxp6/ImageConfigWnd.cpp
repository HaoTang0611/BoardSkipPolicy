// ImageConfigWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "ImageConfigWnd.h"
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
// CImageConfigWnd dialog
//-------------------------------------------------------------------------------------//
CImageConfigWnd::CImageConfigWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CImageConfigWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CImageConfigWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_StopSliceItemChanged = false;
	m_StopFrameItemChanged = false;
}
//-------------------------------------------------------------------------------------//
void CImageConfigWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImageConfigWnd)	
	DDX_Control(pDX, IMGCFG_FRAME_SLICE_COMBO_3, m_FrameSliceCombox3);
	DDX_Control(pDX, IMGCFG_FRAME_SLICE_COMBO_2, m_FrameSliceCombox2);
	DDX_Control(pDX, IMGCFG_FRAME_SLICE_COMBO_1, m_FrameSliceCombox1);
	DDX_Control(pDX, IMGCFG_FRAME_TYPE_COMBO, m_FrameTypeCombox);
	DDX_Control(pDX, IMGCFG_SLICE_CAMERA_ID_COMBO, m_CameraCombox);
	DDX_Control(pDX, IMGCFG_SLICE_FUNC_MODE_COMBO, m_FuncModeCombox);	
	DDX_Control(pDX, IMGCFG_SLICE_CALIBRATION_COMBO, m_CaliModeCombox);
	DDX_Control(pDX, IMGCFG_FRAME_LIST_WND, m_FrameListWnd);
	DDX_Control(pDX, IMGCFG_SLICE_LIST_WND, m_SliceListWnd);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CImageConfigWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CImageConfigWnd)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IMGCFG_SLICE_ADD_BTN, OnSliceAddBtn)
	ON_BN_CLICKED(IMGCFG_SLICE_MODIFY_BTN, OnSliceModifyBtn)
	ON_NOTIFY(NM_CLICK, IMGCFG_SLICE_LIST_WND, OnClickSliceListWnd)
	ON_NOTIFY(NM_DBLCLK, IMGCFG_SLICE_LIST_WND, OnDblclkSliceListWnd)
	ON_BN_CLICKED(IMGCFG_SLICE_SAVE_BTN, OnSliceSaveBtn)
	ON_BN_CLICKED(IMGCFG_SLICE_RESET_BTN, OnSliceResetBtn)
	ON_BN_CLICKED(IMGCFG_SLICE_LOAD_BTN, OnSliceLoadBtn)
	ON_BN_CLICKED(IMGCFG_SLICE_DELETE_BTN, OnSliceDeleteBtn)
	ON_NOTIFY(LVN_ITEMCHANGED, IMGCFG_SLICE_LIST_WND, OnItemchangedSliceListWnd)
	ON_BN_CLICKED(IMGCFG_SLICE_CLEAR_BTN, OnSliceClearBtn)
	ON_BN_CLICKED(IMGCFG_FRAME_SAVE_BTN, OnFrameSaveBtn)
	ON_BN_CLICKED(IMGCFG_FRAME_LOAD_BTN, OnFrameLoadBtn)
	ON_BN_CLICKED(IMGCFG_FRAME_ADD_BTN, OnFrameAddBtn)
	ON_BN_CLICKED(IMGCFG_FRAME_MODIFY_BTN, OnFrameModifyBtn)
	ON_BN_CLICKED(IMGCFG_FRAME_DELETE_BTN, OnFrameDeleteBtn)
	ON_BN_CLICKED(IMGCFG_FRAME_CLEAR_BTN, OnFrameClearBtn)
	ON_NOTIFY(NM_CLICK, IMGCFG_FRAME_LIST_WND, OnClickFrameListWnd)
	ON_NOTIFY(LVN_ITEMCHANGED, IMGCFG_FRAME_LIST_WND, OnItemchangedFrameListWnd)
	ON_CBN_SELCHANGE(IMGCFG_FRAME_TYPE_COMBO, OnSelchangeFrameTypeCombo)
	ON_BN_CLICKED(IMGCFG_CHECK_BTN, OnCheckBtn)	
	ON_BN_CLICKED(IMGCFG_SLICE_SET_ALL_GAIN_BTN, OnSliceSetAllGainBtn)
	ON_BN_CLICKED(IMGCFG_SLICE_SET_ALL_EXP_TIME_BTN, OnSliceSetAllExpTimeBtn)
	ON_BN_CLICKED(IMGCFG_SLICE_SET_ALL_TARGET_GRAY_BTN, OnSliceSetAllTargetGrayBtn)
	ON_BN_CLICKED(IMGCFG_SLICE_SET_ALL_VERIFY_TOL_BTN, OnSliceSetAllVerifyTolBtn)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CImageConfigWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CImageConfigWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	size_t i=0;
	SwitchMultiLanguage();
	const bool bDisable3D = AOIDataCollect.GetDisable3D();	

	CCameraCtrl::BuildCameraIDCombox(m_CameraCombox);	
	JetAPI::SetComboxCurSel(m_CameraCombox, PRIMARY_CAMERA_ID);	
	CWnd::SetDlgItemInt(IMGCFG_SLICE_TARGET_GRAY_EDIT, 64);
	CWnd::SetDlgItemInt(IMGCFG_SLICE_CAMERA_EXP_TIME_EDIT, 1000);
	CWnd::SetDlgItemInt(IMGCFG_SLICE_NEXT_GRAB_BT_TIME_EDIT, 0);
	CWnd::SetDlgItemInt(IMGCFG_SLICE_GAIN_EDIT, 1);	

	const int DLPCastCount = LightCtrlBoard.GetDLPCastCount();
	const int LEDChannelCount = LightCtrlBoard.GetLEDChannelCount();

	m_LEDUIChkList.clear();
	m_LEDUIChkList.push_back(IMGCFG_SLICE_LED_ID_CHK_1);
	m_LEDUIChkList.push_back(IMGCFG_SLICE_LED_ID_CHK_2);
	m_LEDUIChkList.push_back(IMGCFG_SLICE_LED_ID_CHK_3);
	m_LEDUIChkList.push_back(IMGCFG_SLICE_LED_ID_CHK_4);
	m_LEDUIChkList.push_back(IMGCFG_SLICE_LED_ID_CHK_5);
	m_LEDUIChkList.push_back(IMGCFG_SLICE_LED_ID_CHK_6);
	m_LEDUIChkList.push_back(IMGCFG_SLICE_LED_ID_CHK_7);
	m_LEDUIChkList.push_back(IMGCFG_SLICE_LED_ID_CHK_8);
	m_LEDUIChkList.push_back(IMGCFG_SLICE_LED_ID_CHK_9);
	m_LEDUIChkList.push_back(IMGCFG_SLICE_LED_ID_CHK_10);
	m_LEDUIChkList.push_back(IMGCFG_SLICE_LED_ID_CHK_11);
	m_LEDUIChkList.push_back(IMGCFG_SLICE_LED_ID_CHK_12);

	m_LEDUIEditList.clear();
	m_LEDUIEditList.push_back(IMGCFG_SLICE_LED_PWR_EDIT_1);
	m_LEDUIEditList.push_back(IMGCFG_SLICE_LED_PWR_EDIT_2);
	m_LEDUIEditList.push_back(IMGCFG_SLICE_LED_PWR_EDIT_3);
	m_LEDUIEditList.push_back(IMGCFG_SLICE_LED_PWR_EDIT_4);
	m_LEDUIEditList.push_back(IMGCFG_SLICE_LED_PWR_EDIT_5);
	m_LEDUIEditList.push_back(IMGCFG_SLICE_LED_PWR_EDIT_6);
	m_LEDUIEditList.push_back(IMGCFG_SLICE_LED_PWR_EDIT_7);
	m_LEDUIEditList.push_back(IMGCFG_SLICE_LED_PWR_EDIT_8);
	m_LEDUIEditList.push_back(IMGCFG_SLICE_LED_PWR_EDIT_9);
	m_LEDUIEditList.push_back(IMGCFG_SLICE_LED_PWR_EDIT_10);
	m_LEDUIEditList.push_back(IMGCFG_SLICE_LED_PWR_EDIT_11);
	m_LEDUIEditList.push_back(IMGCFG_SLICE_LED_PWR_EDIT_12);

	CWnd::SetDlgItemInt(IMGCFG_FRAME_SATURATION_RED_EDIT, 1);
	CWnd::SetDlgItemInt(IMGCFG_FRAME_SATURATION_GREEN_EDIT, 1);
	CWnd::SetDlgItemInt(IMGCFG_FRAME_SATURATION_BLUE_EDIT, 1);

	const size_t LEDUIChkCount = m_LEDUIChkList.size();	
	const size_t LEDUIEditCount = m_LEDUIEditList.size();	
	for ( i=0; i<LEDUIEditCount; i++ )
	{	CWnd::SetDlgItemInt(m_LEDUIEditList[i], 0);	}

	for ( i=LEDChannelCount; i<LEDUIChkCount; i++ )
	{	JetAPI::EnableCtrlWnd(this, m_LEDUIChkList[i], FALSE);	}

	for ( i=LEDChannelCount; i<LEDUIEditCount; i++ )
	{	JetAPI::EnableCtrlWnd(this, m_LEDUIEditList[i], FALSE);	}

	if ( true == bDisable3D )
	{
		JetAPI::ShowCtrlWnd(this, IMGCFG_SLICE_3D_CAST_ENABLE_CHK_1, FALSE);
		JetAPI::ShowCtrlWnd(this, IMGCFG_SLICE_3D_CAST_ENABLE_CHK_2, FALSE);
		JetAPI::ShowCtrlWnd(this, IMGCFG_SLICE_3D_CAST_ENABLE_CHK_3, FALSE);
		JetAPI::ShowCtrlWnd(this, IMGCFG_SLICE_3D_CAST_ENABLE_CHK_4, FALSE);
	}
#ifdef BYPASS_DLP1_USE
	JetAPI::EnableCtrlWnd(this, IMGCFG_SLICE_3D_CAST_ENABLE_CHK_1, FALSE);
#endif//BYPASS_DLP1_USE
#ifdef BYPASS_DLP2_USE
	JetAPI::EnableCtrlWnd(this, IMGCFG_SLICE_3D_CAST_ENABLE_CHK_2, FALSE);
#endif//BYPASS_DLP2_USE
#ifdef BYPASS_DLP3_USE
	JetAPI::EnableCtrlWnd(this, IMGCFG_SLICE_3D_CAST_ENABLE_CHK_3, FALSE);
#endif//BYPASS_DLP3_USE
#ifdef BYPASS_DLP4_USE
	JetAPI::EnableCtrlWnd(this, IMGCFG_SLICE_3D_CAST_ENABLE_CHK_4, FALSE);
#endif//BYPASS_DLP4_USE
	BuildSliceListWndHeader();
	BuildFrameListWndHeader();
	BuildSliceListWnd();
	BuildFrameListWnd();	

	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	AOIDataDefine.BuildLEDCurrentCaliModeCombox(m_CaliModeCombox);	
	AOIDataDefine.BuildSliceFuncModeCombox(m_FuncModeCombox, true, true);
	AOIDataDefine.BuildSystemFrameParamTypeCombox(m_FrameTypeCombox);
	BuildSliceCombox();
	JetAPI::SetComboxCurSel(m_CaliModeCombox, LED_CURRENT_CALI_AVERAGE);
	JetAPI::SetComboxCurSel(m_FuncModeCombox, SLICE_FUNC_2D_IMAGE_GRAY);
	AOIDataCollect.CloneSystemSliceParamList(m_SliceParamBackup);
	AOIDataCollect.CloneSystemFrameParamList(m_FrameParamBackup);	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CImageConfigWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CImageConfigWnd::SwitchMultiLanguage()
{
	size_t  i = 0;	
	UINT    WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_IMAGE_CONFIG_WND");	
	//---------------------------------------------------------------------------------//
	WndID = IDD_IMAGE_CONFIG_WND;
	WndKey = _T("IDD_IMAGE_CONFIG_WND");
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
	WndID = IMGCFG_SLICE_CONFIG_GROUP;
	WndKey = _T("IMGCFG_SLICE_CONFIG_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = IMGCFG_SLICE_CAMERA_ID_LABEL;
	WndKey = _T("IMGCFG_SLICE_CAMERA_ID_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = IMGCFG_SLICE_CAMERA_EXP_TIME_LABEL;
	WndKey = _T("IMGCFG_SLICE_CAMERA_EXP_TIME_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = IMGCFG_SLICE_NEXT_GRAB_BT_TIME_LABEL;
	WndKey = _T("IMGCFG_SLICE_NEXT_GRAB_BT_TIME_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = IMGCFG_SLICE_LED_LABEL;
	WndKey = _T("IMGCFG_SLICE_LED_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = IMGCFG_SLICE_LED_PWR_LABEL;
	WndKey = _T("IMGCFG_SLICE_LED_PWR_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = IMGCFG_SLICE_LED_ID_CHK_1;
	WndKey = _T("IMGCFG_SLICE_LED_ID_CHK_1");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = IMGCFG_SLICE_LED_ID_CHK_2;
	WndKey = _T("IMGCFG_SLICE_LED_ID_CHK_2");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	
	WndID = IMGCFG_SLICE_LED_ID_CHK_3;
	WndKey = _T("IMGCFG_SLICE_LED_ID_CHK_3");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	
	WndID = IMGCFG_SLICE_LED_ID_CHK_4;
	WndKey = _T("IMGCFG_SLICE_LED_ID_CHK_4");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = IMGCFG_SLICE_LED_ID_CHK_5;
	WndKey = _T("IMGCFG_SLICE_LED_ID_CHK_5");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	
	WndID = IMGCFG_SLICE_LED_ID_CHK_6;
	WndKey = _T("IMGCFG_SLICE_LED_ID_CHK_6");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	
	WndID = IMGCFG_SLICE_LED_ID_CHK_7;
	WndKey = _T("IMGCFG_SLICE_LED_ID_CHK_7");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	
	WndID = IMGCFG_SLICE_LED_ID_CHK_8;
	WndKey = _T("IMGCFG_SLICE_LED_ID_CHK_8");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = IMGCFG_SLICE_LED_ID_CHK_9;
	WndKey = _T("IMGCFG_SLICE_LED_ID_CHK_9");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = IMGCFG_SLICE_LED_ID_CHK_10;
	WndKey = _T("IMGCFG_SLICE_LED_ID_CHK_10");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = IMGCFG_SLICE_LED_ID_CHK_11;
	WndKey = _T("IMGCFG_SLICE_LED_ID_CHK_11");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = IMGCFG_SLICE_LED_ID_CHK_12;
	WndKey = _T("IMGCFG_SLICE_LED_ID_CHK_12");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = IMGCFG_SLICE_DLP_ID_CHK;
	WndKey = _T("IMGCFG_SLICE_DLP_ID_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = IMGCFG_SLICE_TARGET_GRAY_LABEL;
	WndKey = _T("IMGCFG_SLICE_TARGET_GRAY_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = IMGCFG_SLICE_GAIN_LABEL;
	WndKey = _T("IMGCFG_SLICE_GAIN_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = IMGCFG_SLICE_VERIFY_TOL_LABEL;
	WndKey = _T("IMGCFG_SLICE_VERIFY_TOL_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = IMGCFG_SLICE_SAVE_BTN;
	WndKey = _T("IMGCFG_SLICE_SAVE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = IMGCFG_SLICE_LOAD_BTN;
	WndKey = _T("IMGCFG_SLICE_LOAD_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = IMGCFG_SLICE_ADD_BTN;
	WndKey = _T("IMGCFG_SLICE_ADD_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = IMGCFG_SLICE_MODIFY_BTN;
	WndKey = _T("IMGCFG_SLICE_MODIFY_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = IMGCFG_SLICE_DELETE_BTN;
	WndKey = _T("IMGCFG_SLICE_DELETE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = IMGCFG_SLICE_RESET_BTN;
	WndKey = _T("IMGCFG_SLICE_RESET_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = IMGCFG_SLICE_CLEAR_BTN;
	WndKey = _T("IMGCFG_SLICE_CLEAR_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = IMGCFG_SLICE_SET_ALL_EXP_TIME_BTN;
	WndKey = _T("IMGCFG_SLICE_SET_ALL_EXP_TIME_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = IMGCFG_SLICE_SET_ALL_TARGET_GRAY_BTN;
	WndKey = _T("IMGCFG_SLICE_SET_ALL_TARGET_GRAY_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = IMGCFG_SLICE_SET_ALL_GAIN_BTN;
	WndKey = _T("IMGCFG_SLICE_SET_ALL_GAIN_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = IMGCFG_SLICE_SET_ALL_VERIFY_TOL_BTN;
	WndKey = _T("IMGCFG_SLICE_SET_ALL_VERIFY_TOL_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	
	WndID = IMGCFG_SLICE_2D_PARAM_GROUP;
	WndKey = _T("IMGCFG_SLICE_2D_PARAM_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = IMGCFG_SLICE_CALIBRATION_LABEL;
	WndKey = _T("IMGCFG_SLICE_CALIBRATION_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = IMGCFG_SLICE_3D_PARAM_GROUP;
	WndKey = _T("IMGCFG_SLICE_3D_PARAM_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = IMGCFG_SLICE_FUNC_MODE_LABEL;
	WndKey = _T("IMGCFG_SLICE_FUNC_MODE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = IMGCFG_SLICE_3D_CAST_ENABLE_CHK_1;
	WndKey = _T("IMGCFG_SLICE_3D_CAST_ENABLE_CHK_1");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = IMGCFG_SLICE_3D_CAST_ENABLE_CHK_2;
	WndKey = _T("IMGCFG_SLICE_3D_CAST_ENABLE_CHK_2");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = IMGCFG_SLICE_3D_CAST_ENABLE_CHK_3;
	WndKey = _T("IMGCFG_SLICE_3D_CAST_ENABLE_CHK_3");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = IMGCFG_SLICE_3D_CAST_ENABLE_CHK_4;
	WndKey = _T("IMGCFG_SLICE_3D_CAST_ENABLE_CHK_4");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//		
	WndID = IMGCFG_FRAME_CONFIG_GROUP;
	WndKey = _T("IMGCFG_FRAME_CONFIG_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = IMGCFG_FRAME_TYPE_LABEL;
	WndKey = _T("IMGCFG_FRAME_TYPE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = IMGCFG_FRAME_NAME_LABEL;
	WndKey = _T("IMGCFG_FRAME_NAME_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = IMGCFG_FRAME_SLICE_LABEL_1;
	WndKey = _T("IMGCFG_FRAME_SLICE_LABEL_1");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = IMGCFG_FRAME_SLICE_LABEL_2;
	WndKey = _T("IMGCFG_FRAME_SLICE_LABEL_2");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = IMGCFG_FRAME_SLICE_LABEL_3;
	WndKey = _T("IMGCFG_FRAME_SLICE_LABEL_3");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = IMGCFG_FRAME_SATURATION_GROUP;
	WndKey = _T("IMGCFG_FRAME_SATURATION_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = IMGCFG_FRAME_SATURATION_RED_LABEL;
	WndKey = _T("IMGCFG_FRAME_SATURATION_RED_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = IMGCFG_FRAME_SATURATION_GREEN_LABEL;
	WndKey = _T("IMGCFG_FRAME_SATURATION_GREEN_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = IMGCFG_FRAME_SATURATION_BLUE_LABEL;
	WndKey = _T("IMGCFG_FRAME_SATURATION_BLUE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		
	
	WndID = IMGCFG_FRAME_SAVE_BTN;
	WndKey = _T("IMGCFG_FRAME_SAVE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = IMGCFG_FRAME_LOAD_BTN;
	WndKey = _T("IMGCFG_FRAME_LOAD_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = IMGCFG_FRAME_ADD_BTN;
	WndKey = _T("IMGCFG_FRAME_ADD_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = IMGCFG_FRAME_MODIFY_BTN;
	WndKey = _T("IMGCFG_FRAME_MODIFY_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = IMGCFG_FRAME_DELETE_BTN;
	WndKey = _T("IMGCFG_FRAME_DELETE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = IMGCFG_FRAME_CLEAR_BTN;
	WndKey = _T("IMGCFG_FRAME_CLEAR_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		
	//---------------------------------------------------------------------------------//
	WndID = IMGCFG_CHECK_BTN;
	WndKey = _T("IMGCFG_CHECK_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
}
//-------------------------------------------------------------------------------------//
CString CImageConfigWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_IMAGE_CONFIG_WND");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
void CImageConfigWnd::OnSliceAddBtn() 
{
	// TODO: Add your control notification handler code here
	CString      str;
	CString      strName;
	int          LEDIdx = 0;
	bool         bUsedLED = false;
	bool         bUsedDLP = false;		
	TSliceParam  Param;
	Param.SliceUniqueID = AOIDataCollect.GetSystemSliceParamFreeUniqueID();
	if ( AOIDataCollect.CheckSystemSliceParamFreeUniqueID(Param.SliceUniqueID) == false )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return;	
	}

	if ( UpdateUIToSliceParam(Param) == false )
	{	return; }

	if ( AOIDataCollect.AddSystemSliceParam(Param, true) == false )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return;
	}
	BuildSliceListWnd();
	BuildSliceCombox();	
}
//-------------------------------------------------------------------------------------//
BOOL CImageConfigWnd::BuildSliceListWnd()
{
	CString str;	
	bool    bRebuild = false;
	size_t  i=0, j=0, k=0;
	size_t  LEDIdx=0;
	size_t  DLPCastCount=0;	
	TSliceParam  *SliceParamPtr = NULL;
	CListCtrl &ListCtrl = m_SliceListWnd;
	const int ItemCount = ListCtrl.GetItemCount();
	const bool bDisable3D = AOIDataCollect.GetDisable3D();	
	const int SliceCount = (int)(AOIDataCollect.GetSystemSliceParamCount());

	m_StopSliceItemChanged = true;
	if ( SliceCount != ItemCount )
	{
		bRebuild = true;
		ListCtrl.DeleteAllItems();
		ListCtrl.SetTextBkColor(0xBFFFFF);
	}
	else
	{	bRebuild = false; }
	for ( i=0; i<SliceCount; i++ )
	{
		SliceParamPtr = AOIDataCollect.GetSystemSliceParamPtr(i, false);
		if ( NULL == SliceParamPtr ) { continue; }
		if ( true == bDisable3D )
		{
			if ( SLICE_FUNC_2D_IMAGE_GRAY != SliceParamPtr->SliceFuncMode )
			{	continue; }
		}

		j = 0;
		str.Format(_T("%d"), i+1);
		if ( true == bRebuild )
		{	ListCtrl.InsertItem(i, str);	}

		ListCtrl.SetItemData(i, SliceParamPtr->SliceUniqueID);

		ListCtrl.SetItemText(i, j, str);
		j ++;
		
		str = SliceParamPtr->SliceName;
		ListCtrl.SetItemText(i, j, str);
		j ++;

		str = CCameraCtrl::GetCameraIDText(SliceParamPtr->SliceCameraID);
		ListCtrl.SetItemText(i, j, str);
		j ++;

		str.Format(_T("%d"), SliceParamPtr->SliceCameraExpTimeus);
		ListCtrl.SetItemText(i, j, str);
		j ++;

		str.Format(_T("%d"), SliceParamPtr->SliceNextGrabBtTimeus);
		ListCtrl.SetItemText(i, j, str);
		j ++;

		str.Format(_T("%d"), SliceParamPtr->SliceTargetGray);
		ListCtrl.SetItemText(i, j, str);
		j ++;
		
		str.Format(_T("%.2f"), SliceParamPtr->SliceGainValue);
		ListCtrl.SetItemText(i, j, str);
		j ++;

		str.Format(_T("%d"), SliceParamPtr->SliceVerifyTolerance);
		ListCtrl.SetItemText(i, j, str);
		j ++;

		switch ( SliceParamPtr->SliceLightTable.LEDCurrCaliMode )
		{
		case LED_CURRENT_CALI_AVERAGE:	str = _T("Ave");	break;
		case LED_CURRENT_CALI_BALANCE:	str = _T("Bal");	break;
		default:
		case LED_CURRENT_CALI_DISABLE:	str = _T("---"); 	break;
		}
		ListCtrl.SetItemText(i, j, str);
		j ++;

		for ( k=0; k<LED_CHANNEL_COUNT; k++ )
		{
			LEDIdx = k;//LED - 01
			if ( FN_DISABLE == SliceParamPtr->SliceLightTable.LEDChannel[LEDIdx].OnOffState )
			{	str = _T("");	}
			else
			{	str.Format(_T("%d%%"), SliceParamPtr->SliceLightTable.LEDChannel[LEDIdx].PowerValue);	}
			ListCtrl.SetItemText(i, j, str);
			j ++;
		}


		DLPCastCount=0;
		for ( k=0; k<DLP_CAST_COUNT; k++ )
		{
			if ( FN_DISABLE == SliceParamPtr->SliceLightTable.DLPCast[k].OnOffState ) 
			{	continue; }
			DLPCastCount ++;			
		}
		if ( 0 == DLPCastCount )
		{	str = _T("");	}
		else
		{	str = _T("Y");	}
		ListCtrl.SetItemText(i, j, str);
		j ++;
		
	}
	m_StopSliceItemChanged = false;
	return TRUE;
}
//-------------------------------------------------------------------------------------//
BOOL CImageConfigWnd::BuildSliceListWndHeader()
{
	JetAPI::InitialListCtrl(m_SliceListWnd);

	int nCol = 0;
	int width = 0, width2 = 0;
	int Align = LVCFMT_CENTER;//LVCFMT_CENTER;LVCFMT_RIGHT
	RECT Rect={0};
	CString str;

	this->m_SliceListWnd.GetClientRect(&Rect);
	width = Rect.right-Rect.left;
	width = width-32-96;
	width = width/(18);//Idx+Camera+LEDx8+DLP+Gray=12
	width2 = width;

	str = _T("Idx");
	str = LoadMultiLanguageString(str, str);
	this->m_SliceListWnd.InsertColumn(nCol, str, Align, 32);
	nCol ++;	

	str = _T("Name");
	str = LoadMultiLanguageString(str, str);
	this->m_SliceListWnd.InsertColumn(nCol, str, Align, width2*1.5);
	nCol ++;	

	str = _T("Camera");
	str = LoadMultiLanguageString(str, str);
	this->m_SliceListWnd.InsertColumn(nCol, str, Align, width2*1.5);
	nCol ++;	

	str = _T("Exp.");
	str = LoadMultiLanguageString(str, str);
	this->m_SliceListWnd.InsertColumn(nCol, str, Align, width2);
	nCol ++;	

	str = _T("Bt.");
	str = LoadMultiLanguageString(str, str);
	this->m_SliceListWnd.InsertColumn(nCol, str, Align, width2);
	nCol ++;

	str = _T("Gray");
	str = LoadMultiLanguageString(str, str);
	this->m_SliceListWnd.InsertColumn(nCol, str, Align, width2);
	nCol ++;

	str = _T("Gain");
	str = LoadMultiLanguageString(str, str);
	this->m_SliceListWnd.InsertColumn(nCol, str, Align, width2);
	nCol ++;

	str = _T("Tol");
	str = LoadMultiLanguageString(str, str);
	this->m_SliceListWnd.InsertColumn(nCol, str, Align, width2);
	nCol ++;

	str = _T("Cali");
	str = LoadMultiLanguageString(str, str);
	this->m_SliceListWnd.InsertColumn(nCol, str, Align, width2);
	nCol ++;

	str = _T("LED01");	
	str = LoadMultiLanguageString(str, str);
	this->m_SliceListWnd.InsertColumn(nCol, str, Align, width2);
	nCol ++;	

	str = _T("LED02");
	str = LoadMultiLanguageString(str, str);
	this->m_SliceListWnd.InsertColumn(nCol, str, Align, width2);
	nCol ++;	

	str = _T("LED03");
	str = LoadMultiLanguageString(str, str);
	this->m_SliceListWnd.InsertColumn(nCol, str, Align, width2);
	nCol ++;	

	str = _T("LED04");
	str = LoadMultiLanguageString(str, str);
	this->m_SliceListWnd.InsertColumn(nCol, str, Align, width2);
	nCol ++;	

	str = _T("LED05");
	str = LoadMultiLanguageString(str, str);
	this->m_SliceListWnd.InsertColumn(nCol, str, Align, width2);
	nCol ++;	

	str = _T("LED06");
	str = LoadMultiLanguageString(str, str);
	this->m_SliceListWnd.InsertColumn(nCol, str, Align, width2);
	nCol ++;	

	str = _T("LED07");
	str = LoadMultiLanguageString(str, str);
	this->m_SliceListWnd.InsertColumn(nCol, str, Align, width2);
	nCol ++;	

	str = _T("LED08");
	str = LoadMultiLanguageString(str, str);
	this->m_SliceListWnd.InsertColumn(nCol, str, Align, width2);
	nCol ++;	

	str = _T("LED09");
	str = LoadMultiLanguageString(str, str);
	this->m_SliceListWnd.InsertColumn(nCol, str, Align, width2);
	nCol ++;	

	str = _T("LED10");
	str = LoadMultiLanguageString(str, str);
	this->m_SliceListWnd.InsertColumn(nCol, str, Align, width2);
	nCol ++;	

	str = _T("LED11");
	str = LoadMultiLanguageString(str, str);
	this->m_SliceListWnd.InsertColumn(nCol, str, Align, width2);
	nCol ++;	

	str = _T("LED12");
	str = LoadMultiLanguageString(str, str);
	this->m_SliceListWnd.InsertColumn(nCol, str, Align, width2);
	nCol ++;	

	str = _T("3D");
	str = LoadMultiLanguageString(str, str);
	this->m_SliceListWnd.InsertColumn(nCol, str, Align, 32);
	nCol ++;	
	
	return TRUE;
}
//-------------------------------------------------------------------------------------//
void CImageConfigWnd::UpdateSliceParamToUI(int index)
{
	TSliceParam  *SliceParamPtr = AOIDataCollect.GetSystemSliceParamPtr(index, true);
	if ( NULL == SliceParamPtr ) { return; }
	UpdateSliceParamToUI(*SliceParamPtr);
}
//-------------------------------------------------------------------------------------//
bool CImageConfigWnd::UpdateUIToSliceParam(TSliceParam &Param)
{
	size_t       i=0;
	CString      str;
	CString      strName;
	CString      strGain;	
	int          LEDIdx = 0;
	bool         bUsedLED = false;
	bool         bUsedDLP = false;
	unsigned int SliceUniqueID = Param.SliceUniqueID;
	const size_t LEDUIChkCount = m_LEDUIChkList.size();
	const size_t LEDUIEditCount = m_LEDUIEditList.size();	

	CWnd::GetDlgItemText(IMGCFG_SLICE_NAME_EDIT, strName);	
	CWnd::GetDlgItemText(IMGCFG_SLICE_GAIN_EDIT, strGain);

	Param.SliceCameraID = (CAMERA_ID)JetAPI::GetComboxCurSelData(m_CameraCombox);
	Param.SliceCameraExpTimeus = CWnd::GetDlgItemInt(IMGCFG_SLICE_CAMERA_EXP_TIME_EDIT);
	Param.SliceTargetGray = CWnd::GetDlgItemInt(IMGCFG_SLICE_TARGET_GRAY_EDIT);	
	Param.SliceVerifyTolerance = CWnd::GetDlgItemInt(IMGCFG_SLICE_VERIFY_TOL_EDIT);	
	Param.SliceGainValue = JetAPI::StrToDbl(strGain);
	Param.SliceNextGrabBtTimeus = CWnd::GetDlgItemInt(IMGCFG_SLICE_NEXT_GRAB_BT_TIME_EDIT);	

	const int TextSize = strName.GetLength();
	/*
	const int BufferSize = (int)(sizeof(Param.SliceName)/sizeof(Param.SliceName[0]));
	if ( TextSize >= BufferSize )
	{
		str.Format(_T("Error, Name too long (%s)"), strName);
		JetAPI::ShowMessageBox(str);
		return false;
	}
	//::_tcscpy(Param.SliceName, strName);	*/
	Param.SliceName = strName;	

	if ( CWnd::IsDlgButtonChecked(IMGFIG_SLICE_DLP_ID_CHK) == TRUE )
	{	bUsedDLP = true;	}

	//不支援手動增加3D			
	if ( SLICE_UNIQUE_ID_DLP == SliceUniqueID )
	{	
		bUsedLED = false;
		bUsedDLP = true;
		LEDIdx = 0;		
		for ( i=0; i<LED_CHANNEL_COUNT; i++ )
		{
			Param.SliceLightTable.LEDChannel[LEDIdx].OnOffState = FN_DISABLE;
			Param.SliceLightTable.LEDChannel[LEDIdx].PowerValue = 0;
			LEDIdx ++;
		}

		Param.SliceLightTable.LightType = LIGHT_DLP;
		Param.SliceFuncMode = (SLICE_FUNC_MODE)(JetAPI::GetComboxCurSelData(m_FuncModeCombox));
		if ( SLICE_FUNC_2D_IMAGE_GRAY == Param.SliceFuncMode )
		{	Param.SliceFuncMode = SLICE_FUNC_3D_4STEP_4STEP_1EXP;	}
		if ( CWnd::IsDlgButtonChecked(IMGCFG_SLICE_3D_CAST_ENABLE_CHK_1) == TRUE )
		{	Param.SliceLightTable.DLPCast[0].OnOffState = FN_ENABLE;  }
		else
		{	Param.SliceLightTable.DLPCast[0].OnOffState = FN_DISABLE;  }

		if ( CWnd::IsDlgButtonChecked(IMGCFG_SLICE_3D_CAST_ENABLE_CHK_2) == TRUE )
		{	Param.SliceLightTable.DLPCast[1].OnOffState = FN_ENABLE;  }
		else
		{	Param.SliceLightTable.DLPCast[1].OnOffState = FN_DISABLE;  }
		
		if ( CWnd::IsDlgButtonChecked(IMGCFG_SLICE_3D_CAST_ENABLE_CHK_3) == TRUE )
		{	Param.SliceLightTable.DLPCast[2].OnOffState = FN_ENABLE;  }
		else
		{	Param.SliceLightTable.DLPCast[2].OnOffState = FN_DISABLE;  }

		if ( CWnd::IsDlgButtonChecked(IMGCFG_SLICE_3D_CAST_ENABLE_CHK_4) == TRUE )
		{	Param.SliceLightTable.DLPCast[3].OnOffState = FN_ENABLE;  }
		else
		{	Param.SliceLightTable.DLPCast[3].OnOffState = FN_DISABLE;  }
				
		unsigned int DLPImageCount=0;
		unsigned int DLPRepeatCount=1;
		unsigned int DLPPhasePatMode=0;
		unsigned int TotalImageCount=0;		
		const size_t DLPCastCount = DLP_CAST_COUNT;
		for ( i=0; i<DLPCastCount; i++ )
		{			
			DLPPhasePatMode=AOIDataCollect.CheckSliceFuncModeDlpPatternMode(Param.SliceFuncMode);
			DLPRepeatCount=AOIDataCollect.CheckFrameLightCountBySliceFuncMode(Param.SliceFuncMode);
			DLPImageCount=AOIDataCollect.CheckSliceFuncModeDlpPatternCountOnce(Param.SliceFuncMode);

			if ( FN_ENABLE == Param.SliceLightTable.DLPCast[i].OnOffState  )
			{	TotalImageCount += (DLPImageCount); }
			Param.SliceLightTable.DLPCast[i].TriggerCount = DLPImageCount;
			Param.SliceLightTable.DLPCast[i].PhasePatMode = DLPPhasePatMode;			
		}
		Param.SliceCameraFrames = TotalImageCount;
	}
	else
	{			
		bUsedLED = true;
		bUsedDLP = false;
		LEDIdx = 0;		
		for ( i=0; i<LED_CHANNEL_COUNT; i++ )
		{
			if ( i < LEDUIChkCount )
			{
				if ( CWnd::IsDlgButtonChecked(m_LEDUIChkList[i]) == TRUE )
				{	Param.SliceLightTable.LEDChannel[LEDIdx].OnOffState = FN_ENABLE;	}
				else
				{	Param.SliceLightTable.LEDChannel[LEDIdx].OnOffState = FN_DISABLE; }
			}
			if ( i < LEDUIEditCount )
			{	Param.SliceLightTable.LEDChannel[LEDIdx].PowerValue = CWnd::GetDlgItemInt(m_LEDUIEditList[i]); }
			LEDIdx ++;
		}	
		
		Param.SliceLightTable.LightType = LIGHT_LED;
		Param.SliceFuncMode = SLICE_FUNC_2D_IMAGE_GRAY;
		Param.SliceLightTable.LEDCurrCaliMode = (LED_CURRENT_CALI_MODE)(JetAPI::GetComboxCurSelData(m_CaliModeCombox));
		Param.SliceLightTable.DLPCast[0].OnOffState = FN_DISABLE; 
		Param.SliceLightTable.DLPCast[1].OnOffState = FN_DISABLE; 
		Param.SliceLightTable.DLPCast[2].OnOffState = FN_DISABLE; 
		Param.SliceLightTable.DLPCast[3].OnOffState = FN_DISABLE; 
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
void CImageConfigWnd::UpdateSliceParamToUI(const TSliceParam &Param)
{
	size_t       i=0;
	CString      str;	
	CString      strGain;		
	int          LEDIdx = 0;
	BOOL         bCheck = FALSE;
	const size_t LEDUIChkCount = m_LEDUIChkList.size();
	const size_t LEDUIEditCount = m_LEDUIEditList.size();	

	strGain.Format(_T("%.2f"), Param.SliceGainValue);
	JetAPI::SetComboxCurSel(m_CameraCombox, Param.SliceCameraID);
	CWnd::SetDlgItemInt(IMGCFG_SLICE_CAMERA_EXP_TIME_EDIT, Param.SliceCameraExpTimeus);
	CWnd::SetDlgItemInt(IMGCFG_SLICE_TARGET_GRAY_EDIT, Param.SliceTargetGray);	
	CWnd::SetDlgItemInt(IMGCFG_SLICE_VERIFY_TOL_EDIT, Param.SliceVerifyTolerance);		
	CWnd::SetDlgItemText(IMGCFG_SLICE_GAIN_EDIT, strGain);	
	CWnd::SetDlgItemText(IMGCFG_SLICE_NAME_EDIT, Param.SliceName);	
	CWnd::SetDlgItemInt(IMGCFG_SLICE_UNIQUE_ID_EDIT, Param.SliceUniqueID);	
	CWnd::SetDlgItemInt(IMGCFG_SLICE_NEXT_GRAB_BT_TIME_EDIT, Param.SliceNextGrabBtTimeus);	

	LEDIdx = 0;
	for ( i=0; i<LED_CHANNEL_COUNT; i++ )
	{
		if ( i<LEDUIChkCount )
		{
			if ( FN_ENABLE == Param.SliceLightTable.LEDChannel[LEDIdx].OnOffState )
			{	CWnd::CheckDlgButton(m_LEDUIChkList[i], TRUE); }
			else
			{	CWnd::CheckDlgButton(m_LEDUIChkList[i], FALSE); }
		}
		if ( i<LEDUIEditCount )
		{	CWnd::SetDlgItemInt(m_LEDUIEditList[i], Param.SliceLightTable.LEDChannel[LEDIdx].PowerValue); }
		LEDIdx ++;
	}	
	
	JetAPI::SetComboxCurSel(m_FuncModeCombox, Param.SliceFuncMode);
	JetAPI::SetComboxCurSel(m_CaliModeCombox, Param.SliceLightTable.LEDCurrCaliMode);

	if ( FN_ENABLE==Param.SliceLightTable.DLPCast[0].OnOffState )
	{	bCheck=TRUE; }
	else
	{	bCheck=FALSE; }
	CWnd::CheckDlgButton(IMGCFG_SLICE_3D_CAST_ENABLE_CHK_1, bCheck);

	if ( FN_ENABLE==Param.SliceLightTable.DLPCast[1].OnOffState )
	{	bCheck=TRUE; }
	else
	{	bCheck=FALSE; }
	CWnd::CheckDlgButton(IMGCFG_SLICE_3D_CAST_ENABLE_CHK_2, bCheck);

	if ( FN_ENABLE==Param.SliceLightTable.DLPCast[2].OnOffState )
	{	bCheck=TRUE; }
	else
	{	bCheck=FALSE; }
	CWnd::CheckDlgButton(IMGCFG_SLICE_3D_CAST_ENABLE_CHK_3, bCheck);

	if ( FN_ENABLE==Param.SliceLightTable.DLPCast[3].OnOffState )
	{	bCheck=TRUE; }
	else
	{	bCheck=FALSE; }
	CWnd::CheckDlgButton(IMGCFG_SLICE_3D_CAST_ENABLE_CHK_4, bCheck);

	if ( FN_ENABLE==Param.SliceLightTable.DLPCast[0].OnOffState || 
		 FN_ENABLE==Param.SliceLightTable.DLPCast[1].OnOffState || 
		 FN_ENABLE==Param.SliceLightTable.DLPCast[2].OnOffState || 
		 FN_ENABLE==Param.SliceLightTable.DLPCast[3].OnOffState )
	{	CWnd::CheckDlgButton(IMGFIG_SLICE_DLP_ID_CHK, TRUE);	}
	else
	{	CWnd::CheckDlgButton(IMGFIG_SLICE_DLP_ID_CHK, FALSE);	}
	return ;
}
//-------------------------------------------------------------------------------------//
void CImageConfigWnd::OnSliceModifyBtn() 
{
	// TODO: Add your control notification handler code here
	CString str;
	TSliceParam Param;
	const int index = this->m_SliceListWnd.GetNextItem(-1, LVNI_SELECTED);	
	TSliceParam  *SliceParamPtr = AOIDataCollect.GetSystemSliceParamPtr(index, true);
	if ( NULL == SliceParamPtr ) { return; }
	Param.SliceUniqueID = SliceParamPtr->SliceUniqueID;
	if ( UpdateUIToSliceParam(Param) == false )
	{	return;	}
	if ( AOIDataCollect.ModifySystemSliceParamByIndex(index, Param) == false )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return;
	}
	BuildSliceListWnd();
	BuildSliceCombox();	
	return;
}
//-------------------------------------------------------------------------------------//
void CImageConfigWnd::OnClickSliceListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NMITEMACTIVATE *pItem = (NMITEMACTIVATE*)pNMHDR;	
	const int nItem = pItem->iItem;	
	//UpdateSliceParamToUI(nItem);
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CImageConfigWnd::OnDblclkSliceListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NMITEMACTIVATE *pItem = (NMITEMACTIVATE*)pNMHDR;	
	const int nItem = pItem->iItem;
	//UpdateSliceParamToUI(nItem);
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CImageConfigWnd::OnSliceSaveBtn() 
{
	// TODO: Add your control notification handler code here
	bool IsLockSystemParam = AOIDataCollect.CheckIsLockSystemParameter();	
	if ( true == IsLockSystemParam )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return; 
	}
	if ( AOIDataCollect.SaveSystemSliceParamINI() == false )
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString()); }
}
//-------------------------------------------------------------------------------------//
void CImageConfigWnd::OnSliceResetBtn() 
{
	// TODO: Add your control notification handler code here
	size_t       i=0;
	const size_t LEDUIChkCount = m_LEDUIChkList.size();
	const size_t LEDUIEditCount = m_LEDUIEditList.size();

	for ( i=0; i<LEDUIChkCount; i++ )
	{	CWnd::CheckDlgButton(m_LEDUIChkList[i], FALSE);		}

	for ( i=0; i<LEDUIEditCount; i++ )
	{	CWnd::SetDlgItemInt(m_LEDUIEditList[i], 0);		}	

	CWnd::CheckDlgButton(IMGFIG_SLICE_DLP_ID_CHK, FALSE);
	//CWnd::SetDlgItemText(IMGCFG_SLICE_NAME_EDIT, _T(""));
}
//-------------------------------------------------------------------------------------//
void CImageConfigWnd::OnSliceLoadBtn() 
{
	// TODO: Add your control notification handler code here
	CString str;
	str = _T("Do you want to load sysem file?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO ) { return; }
	if ( AOIDataCollect.LoadSystemSliceParamINI() == false )
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString()); }
	BuildSliceListWnd();
	BuildSliceCombox();	
}
//-------------------------------------------------------------------------------------//
void CImageConfigWnd::OnSliceDeleteBtn() 
{
	// TODO: Add your control notification handler code here
	CString str, str2;
	const int index = this->m_SliceListWnd.GetNextItem(-1, LVNI_SELECTED);
	str2 = _T("Do you want to delete the slice param");
	str2 = LoadMultiLanguageString(str2, str2);
	str.Format(_T("%s[%d]?"), str2, index+1);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO )
	{	return;	}
	if ( AOIDataCollect.DeleteSystemSliceParamIndex(index) == false )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return;
	}	
	m_SliceListWnd.DeleteItem(index);
	BuildSliceCombox();	
}
//-------------------------------------------------------------------------------------//
void CImageConfigWnd::OnItemchangedSliceListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if ( true == m_StopSliceItemChanged )
	{
		*pResult = 0;
		return;
	}		
	DWORD  Res = pNMListView->uNewState&LVIS_FOCUSED;
	if ( NULL == Res )
	{	return; }
	const int nItem =  pNMListView->iItem;
	UpdateSliceParamToUI(nItem);	
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CImageConfigWnd::OnSliceClearBtn() 
{
	// TODO: Add your control notification handler code here
	CString str;	
	str.Format(_T("Do you want to clear the slice param?"));
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO )
	{	return;	}
	if ( AOIDataCollect.DeleteSystemSliceParamUserDefined() == false )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return;
	}	
	BuildSliceListWnd();
	BuildSliceCombox();
}
//-------------------------------------------------------------------------------------//
void CImageConfigWnd::OnFrameSaveBtn() 
{
	// TODO: Add your control notification handler code here
	bool IsLockSystemParam = AOIDataCollect.CheckIsLockSystemParameter();	
	if ( true == IsLockSystemParam )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return; 
	}
	if ( AOIDataCollect.SaveSystemFrameParamINI() == false )
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString()); }
}
//-------------------------------------------------------------------------------------//
void CImageConfigWnd::OnFrameLoadBtn() 
{
	// TODO: Add your control notification handler code here
	CString str;
	str = _T("Do you want to load sysem file?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO ) { return; }

	if ( AOIDataCollect.LoadSystemFrameParamINI() == false )
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString()); }
	BuildFrameListWnd();
}
//-------------------------------------------------------------------------------------//
void CImageConfigWnd::OnFrameAddBtn() 
{
	// TODO: Add your control notification handler code here
	CString      str;
	CString      strName;
	int          LEDIdx = 0;
	bool         bUsedLED = false;
	bool         bUsedDLP = false;		
	TFrameParam  Param;
	Param.FrameUniqueID = AOIDataCollect.GetSystemFrameParamFreeUniqueID();
	if ( AOIDataCollect.CheckSystemFrameParamFreeUniqueID(Param.FrameUniqueID) == false )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return;	
	}

	if ( UpdateUIToFrameParam(Param) == false )
	{	return; }

	if ( AOIDataCollect.AddSystemFrameParam(Param, true) == false )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return;
	}
	BuildFrameListWnd();
}
//-------------------------------------------------------------------------------------//
void CImageConfigWnd::OnFrameModifyBtn() 
{
	// TODO: Add your control notification handler code here
	CString str;
	TFrameParam Param;
	const int nItem = m_FrameListWnd.GetNextItem(-1, LVNI_SELECTED);	
	if ( nItem < 0 ) { return; }
	const int index = m_FrameListWnd.GetItemData(nItem);

	if ( UpdateUIToFrameParam(Param) == false )
	{	return;	}
	if ( AOIDataCollect.ModifySystemFrameParamIndex(index, Param) == false )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return;
	}
	BuildFrameListWnd();
}
//-------------------------------------------------------------------------------------//
void CImageConfigWnd::OnFrameDeleteBtn() 
{
	// TODO: Add your control notification handler code here
	CString str, str2;
	const int nItem = m_FrameListWnd.GetNextItem(-1, LVNI_SELECTED);	
	if ( nItem < 0 ) { return; }
	const int index = m_FrameListWnd.GetItemData(nItem);
	str2 = _T("Do you want to delete the frame param");
	str2 = LoadMultiLanguageString(str2, str2);
	str.Format(_T("%s[%d]?"), str2, nItem+1);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO )
	{	return;	}
	if ( AOIDataCollect.DeleteSystemFrameParamIndex(index) == false )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return;
	}	
	m_FrameListWnd.DeleteItem(nItem);
}
//-------------------------------------------------------------------------------------//
void CImageConfigWnd::OnFrameClearBtn() 
{
	// TODO: Add your control notification handler code here
	CString str;	
	str.Format(_T("Do you want to clear the frame param?"));
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO )
	{	return;	}
	if ( AOIDataCollect.DeleteSystemFrameParamUserDefined() == false )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return;
	}	
	BuildFrameListWnd();
}
//-------------------------------------------------------------------------------------//
BOOL CImageConfigWnd::BuildFrameListWnd()
{
	CString       str;	
	CString       Name;
	bool          bRebuild = false;	
	int           nItem=0;
	size_t        i=0, j=0, k=0;
	size_t        SliceIndex = 0;
	unsigned int  SliceUniqueID=0;
	TFrameParam  *FrameParamPtr = NULL;
	TSliceParam  *SliceParamPtr = NULL;
	CListCtrl &ListCtrl = m_FrameListWnd;
	const int ItemCount = ListCtrl.GetItemCount();
	const bool bDisable3D = AOIDataCollect.GetDisable3D();	
	const int FrameCount = (int)(AOIDataCollect.GetSystemFrameParamCount());
	CAMERA_IMAGE_MODE CameraImageMode = AOIDataCollect.GetCameraImageMode(PRIMARY_CAMERA_ID);

	nItem = 0;
	bRebuild = true;
	ListCtrl.SetRedraw(FALSE);
	m_StopFrameItemChanged = true;	
	ListCtrl.DeleteAllItems();
	ListCtrl.SetTextBkColor(0xFFFFBF);	
	for ( i=0; i<FrameCount; i++ )
	{
		FrameParamPtr = AOIDataCollect.GetSystemFrameParamPtr(i, false);
		if ( NULL == FrameParamPtr ) { continue; }
		if ( true == bDisable3D )
		{
			if ( FRAME_SPACE == FrameParamPtr->FrameType )
			{	continue; }
		}
		/*
		if ( CAMERA_IMAGE_GRAY == CameraImageMode )
		{
			if ( FRAME_BAYER == FrameParamPtr->FrameType )
			{	continue; }			
		}
		else if ( CAMERA_IMAGE_BAYER == CameraImageMode )
		{
			if ( FRAME_BAYER != FrameParamPtr->FrameType )
			{	continue; }			
		}
		else if ( CAMERA_IMAGE_COLOR == CameraImageMode )
		{
		}
		*/
		j = 0;
		str.Format(_T("%d"), i+1);			
		ListCtrl.InsertItem(nItem, str);	
		ListCtrl.SetItemData(nItem, i);		

		ListCtrl.SetItemText(nItem, j, str);
		j ++;

		//Name
		str = FrameParamPtr->FrameName;
		ListCtrl.SetItemText(nItem, j, str);
		j ++;

		//Type
		str = AOIDataDefine.GetFrameTypeName(FrameParamPtr->FrameType);		
		ListCtrl.SetItemText(nItem, j, str);
		j ++;

		//Slice 1 ID
		SliceUniqueID = FrameParamPtr->FrameSliceID1;
		if ( SLICE_UNIQUE_ID_NULL != SliceUniqueID )
		{	
			SliceIndex = AOIDataCollect.GetSystemSliceParamIndex(SliceUniqueID);	
			SliceParamPtr = AOIDataCollect.GetSystemSliceParamPtr(SliceIndex, true);
			if ( NULL == SliceParamPtr )
			{	str.Format(_T("Err(%d)"), SliceIndex+1);	}
			else
			{	str = SliceParamPtr->SliceName; }
		}
		else
		{	str = _T(" ");	}
		ListCtrl.SetItemText(nItem, j, str);
		j ++;

		//Slice 2 ID
		SliceUniqueID = FrameParamPtr->FrameSliceID2;
		if ( SLICE_UNIQUE_ID_NULL != SliceUniqueID )
		{	
			SliceIndex = AOIDataCollect.GetSystemSliceParamIndex(SliceUniqueID);	
			SliceParamPtr = AOIDataCollect.GetSystemSliceParamPtr(SliceIndex, true);
			if ( NULL == SliceParamPtr )
			{	str.Format(_T("Err(%d)"), SliceIndex+1);	}
			else
			{	str = SliceParamPtr->SliceName; }
		}
		else
		{	str = _T(" ");	}		
		ListCtrl.SetItemText(nItem, j, str);
		j ++;

		//Slice 3 ID
		SliceUniqueID = FrameParamPtr->FrameSliceID3;
		if ( SLICE_UNIQUE_ID_NULL != SliceUniqueID )
		{	
			SliceIndex = AOIDataCollect.GetSystemSliceParamIndex(SliceUniqueID);
			SliceParamPtr = AOIDataCollect.GetSystemSliceParamPtr(SliceIndex, true);
			if ( NULL == SliceParamPtr )
			{	str.Format(_T("Err(%d)"), SliceIndex+1);	}
			else
			{	str = SliceParamPtr->SliceName; }
		}
		else
		{	str = _T(" ");	}
		ListCtrl.SetItemText(nItem, j, str);
		j ++;

		nItem ++;
	}
	m_StopFrameItemChanged = false;
	ListCtrl.SetRedraw(TRUE);
	return TRUE;
}
//-------------------------------------------------------------------------------------//
BOOL CImageConfigWnd::BuildFrameListWndHeader()
{
	CListCtrl &ListCtrl = m_FrameListWnd;
	JetAPI::InitialListCtrl(ListCtrl);

	int nCol = 0;
	int width = 0, width2 = 0;
	int Align = LVCFMT_CENTER;//LVCFMT_CENTER;LVCFMT_RIGHT
	RECT Rect={0};
	CString str;

	ListCtrl.GetClientRect(&Rect);
	width = Rect.right-Rect.left;
	width = width-32;
	width = width/(6);//Idx+NameType+Slice1+Slice2+Slice3
	width2 = width;

	str = _T("Idx");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(nCol, str, Align, 32);
	nCol ++;	

	str = _T("Name");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(nCol, str, Align, width2*1.5);
	nCol ++;	

	str = _T("Type");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(nCol, str, Align, width2*1.5);
	nCol ++;	

	str = _T("Slice1");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(nCol, str, Align, width2);
	nCol ++;	

	str = _T("Slice2");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(nCol, str, Align, width2);
	nCol ++;	

	str = _T("Slice3");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(nCol, str, Align, width2);
	nCol ++;

	return TRUE;
}
//-------------------------------------------------------------------------------------//
void CImageConfigWnd::BuildSliceCombox()
{
	bool bIncludeDLP = true;
	bool bIncludeNone = true;	
	bool bUseCaliMode = true;	
	AOIDataDefine.BuildSystemSliceParamCombox(m_FrameSliceCombox1, bIncludeNone, bIncludeDLP, bUseCaliMode);
	AOIDataDefine.BuildSystemSliceParamCombox(m_FrameSliceCombox2, bIncludeNone, bIncludeDLP, bUseCaliMode);
	AOIDataDefine.BuildSystemSliceParamCombox(m_FrameSliceCombox3, bIncludeNone, bIncludeDLP, bUseCaliMode);
}
//-------------------------------------------------------------------------------------//
void CImageConfigWnd::UpdateFrameParamToUI(int index)
{
	TFrameParam  *FrameParamPtr = AOIDataCollect.GetSystemFrameParamPtr(index, true);
	if ( NULL == FrameParamPtr ) { return; }
	UpdateFrameParamToUI(*FrameParamPtr);
}
//-------------------------------------------------------------------------------------//
void CImageConfigWnd::UpdateFrameParamToUI(const TFrameParam &Param)
{
	CString      str;
	str = Param.FrameName;
	CWnd::SetDlgItemText(IMGCFG_FRAME_NAME_EDIT, str);	
	CWnd::SetDlgItemInt(IMGCFG_FRAME_ID_EDIT, Param.FrameUniqueID);

	JetAPI::SetComboxCurSel(m_FrameTypeCombox, Param.FrameType);
	JetAPI::SetComboxCurSel(m_FrameSliceCombox1, Param.FrameSliceID1);
	JetAPI::SetComboxCurSel(m_FrameSliceCombox2, Param.FrameSliceID2);
	JetAPI::SetComboxCurSel(m_FrameSliceCombox3, Param.FrameSliceID3);	

	str.Format(_T("%.2f"), Param.FrameSaturationRed);
	CWnd::SetDlgItemText(IMGCFG_FRAME_SATURATION_RED_EDIT, str);
	str.Format(_T("%.2f"), Param.FrameSaturationGreen);
	CWnd::SetDlgItemText(IMGCFG_FRAME_SATURATION_GREEN_EDIT, str);
	str.Format(_T("%.2f"), Param.FrameSaturationBlue);
	CWnd::SetDlgItemText(IMGCFG_FRAME_SATURATION_BLUE_EDIT, str);	
	ExecUpdateFrameSliceUI(Param.FrameType);	
}
//-------------------------------------------------------------------------------------//
bool CImageConfigWnd::UpdateUIToFrameParam(TFrameParam &Param)
{	
	CString      str;
	CString      strName;

	CWnd::GetDlgItemText(IMGCFG_FRAME_NAME_EDIT, strName);	
	const int TextSize = strName.GetLength();
	/*const int BufferSize = (int)(sizeof(Param.FrameName)/sizeof(Param.FrameName[0]));
	if ( TextSize >= BufferSize )
	{
		str.Format(_T("Error, Name too long (%s)"), strName);
		JetAPI::ShowMessageBox(str);
		return false;
	}
	::_tcscpy(Param.FrameName, strName);	*/
	Param.FrameName = strName;

	Param.FrameType = (FRAME_TYPE)(JetAPI::GetComboxCurSelData(m_FrameTypeCombox));
	Param.FrameSliceID1 = (JetAPI::GetComboxCurSelData(m_FrameSliceCombox1));
	Param.FrameSliceID2 = (JetAPI::GetComboxCurSelData(m_FrameSliceCombox2));
	Param.FrameSliceID3 = (JetAPI::GetComboxCurSelData(m_FrameSliceCombox3));

	CWnd::GetDlgItemText(IMGCFG_FRAME_SATURATION_RED_EDIT, str);
	Param.FrameSaturationRed = ::_ttof(str);
	CWnd::GetDlgItemText(IMGCFG_FRAME_SATURATION_GREEN_EDIT, str);
	Param.FrameSaturationGreen = ::_ttof(str);
	CWnd::GetDlgItemText(IMGCFG_FRAME_SATURATION_BLUE_EDIT, str);
	Param.FrameSaturationBlue = ::_ttof(str);	

	if ( FRAME_SPACE == Param.FrameType )
	{
		str.Format(_T("Error, can not add/modify 3D frame"), strName);
		JetAPI::ShowMessageBox(str);
		return false;
	}

	if ( SLICE_UNIQUE_ID_DLP == Param.FrameSliceID1 || 
		 SLICE_UNIQUE_ID_DLP == Param.FrameSliceID2 || 
		 SLICE_UNIQUE_ID_DLP == Param.FrameSliceID3 )
	{
		str.Format(_T("Error, can not set 3D Cast Slice"), strName);
		JetAPI::ShowMessageBox(str);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CImageConfigWnd::OnClickFrameListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NMITEMACTIVATE *pItem = (NMITEMACTIVATE*)pNMHDR;	
	const int nItem = pItem->iItem;	
	//UpdateFrameParamToUI(nItem);
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CImageConfigWnd::OnItemchangedFrameListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if ( true == m_StopFrameItemChanged )
	{
		*pResult = 0;
		return;
	}		
	DWORD  Res = pNMListView->uNewState&LVIS_FOCUSED;
	if ( NULL == Res )
	{	return; }
	const int nItem =  pNMListView->iItem;
	if ( nItem < 0 ) { return; }
	const int nIndex = m_FrameListWnd.GetItemData(nItem);
	UpdateFrameParamToUI(nIndex);
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CImageConfigWnd::OnSelchangeFrameTypeCombo() 
{
	// TODO: Add your control notification handler code here
	FRAME_TYPE FrameType = (FRAME_TYPE)(JetAPI::GetComboxCurSelData(m_FrameTypeCombox));

	switch ( FrameType )
	{
	case FRAME_COLOR:		
		break;	
	case FRAME_BAYER:		
		JetAPI::SetComboxCurSel(m_FrameSliceCombox2, SLICE_UNIQUE_ID_NULL);
		JetAPI::SetComboxCurSel(m_FrameSliceCombox3, SLICE_UNIQUE_ID_NULL);		
		break;	
	default://FRAME_NULL
	case FRAME_GRAY:	
	case FRAME_SPACE:
		JetAPI::SetComboxCurSel(m_FrameSliceCombox2, SLICE_UNIQUE_ID_NULL);
		JetAPI::SetComboxCurSel(m_FrameSliceCombox3, SLICE_UNIQUE_ID_NULL);		
		break;
	}
	ExecUpdateFrameSliceUI(FrameType);
}
//-------------------------------------------------------------------------------------//
void CImageConfigWnd::OnOK() 
{
	// TODO: Add extra validation here
	bool IsLockSystemParam = AOIDataCollect.CheckIsLockSystemParameter();	
	if ( true == IsLockSystemParam )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return; 
	}
	CString str;
	if ( CImageConfigWnd::ExecCheckImageConfig() == false )
	{	return;	}	

	str = _T("Do you want to save system image configuration?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDYES )
	{
		AOIDataCollect.SaveSystemSliceParamINI();
		AOIDataCollect.SaveSystemFrameParamINI();
	}
	CBaseDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CImageConfigWnd::OnCancel() 
{
	// TODO: Add extra cleanup here
	AOIDataCollect.SetSystemSliceParamList(m_SliceParamBackup);
	AOIDataCollect.SetSystemFrameParamList(m_FrameParamBackup);
	if ( CImageConfigWnd::ExecCheckImageConfig() == false )
	{	return;	}	
	CBaseDialog::OnCancel();
}
//-------------------------------------------------------------------------------------//
void CImageConfigWnd::OnCheckBtn() 
{
	// TODO: Add your control notification handler code here
	CImageConfigWnd::ExecCheckImageConfig();
}
//-------------------------------------------------------------------------------------//
bool CImageConfigWnd::ExecCheckImageConfig()
{
	if ( AOIDataCollect.CheckImageConfiguration() == false )
	{	
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString()); 
		return false;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CImageConfigWnd::ExecUpdateFrameSliceUI(FRAME_TYPE FrameType)
{
	switch ( FrameType )
	{	
	case FRAME_COLOR:
		m_FrameSliceCombox2.EnableWindow(TRUE);
		m_FrameSliceCombox3.EnableWindow(TRUE);
		break;	
	case FRAME_BAYER:
		m_FrameSliceCombox2.EnableWindow(FALSE);
		m_FrameSliceCombox3.EnableWindow(FALSE);
		break;
	default:
	case FRAME_GRAY:
	case FRAME_SPACE:
		m_FrameSliceCombox2.EnableWindow(FALSE);
		m_FrameSliceCombox3.EnableWindow(FALSE);
		break;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CImageConfigWnd::OnSliceSetAllGainBtn() 
{
	// TODO: Add your control notification handler code here
	CString      str;		
	CString      strGain;
	CString      strValue;
	CString      strLabel;
	CString      strCaption;		
	CInputBoxWnd InputBox;

	CWnd::GetDlgItemText(IMGCFG_SLICE_GAIN_EDIT, strGain);
	strCaption = _T("Input Gain Value");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strLabel = _T("Gain");
	strLabel = LoadMultiLanguageString(strLabel, strLabel);
	strValue = strGain;
	InputBox.SetParam1(strCaption, strLabel, strValue);
	if ( InputBox.DoModal() == IDCANCEL ) 
	{	return ;  }
	const double Gain = ::_ttof(InputBox.m_DataEdit1);
	if ( Gain < 0 ) { return; }
	if ( AOIDataCollect.SetSystemSliceParamGain(Gain) == false )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return;
	}
	BuildSliceListWnd();
	BuildSliceCombox();	
	return;
}
//-------------------------------------------------------------------------------------//
void CImageConfigWnd::OnSliceSetAllExpTimeBtn() 
{
	// TODO: Add your control notification handler code here
	CString      str;		
	CString      strExpTime;
	CString      strValue;
	CString      strLabel;
	CString      strCaption;		
	CInputBoxWnd InputBox;

	CWnd::GetDlgItemText(IMGCFG_SLICE_CAMERA_EXP_TIME_EDIT, strExpTime);
	strCaption = _T("Input Exposure Time");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strLabel = _T("Exposure Time (us)");
	strLabel = LoadMultiLanguageString(strLabel, strLabel);
	strValue = strExpTime;
	InputBox.SetParam1(strCaption, strLabel, strValue);
	if ( InputBox.DoModal() == IDCANCEL ) 
	{	return ;  }
	const unsigned int ExpTime = ::_ttoi(InputBox.m_DataEdit1);
	if ( ExpTime < 0 ) { return; }
	if ( AOIDataCollect.SetSystemSliceParamExposureTime(ExpTime) == false )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return;
	}
	BuildSliceListWnd();
	BuildSliceCombox();	
	return;
}
//-------------------------------------------------------------------------------------//
void CImageConfigWnd::OnSliceSetAllTargetGrayBtn() 
{
	// TODO: Add your control notification handler code here
	CString      str;		
	CString      strGray;
	CString      strValue;
	CString      strLabel;
	CString      strCaption;		
	CInputBoxWnd InputBox;

	CWnd::GetDlgItemText(IMGCFG_SLICE_TARGET_GRAY_EDIT, strGray);
	strCaption = _T("Input Gray");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strLabel = _T("Gray");
	strLabel = LoadMultiLanguageString(strLabel, strLabel);
	strValue = strGray;
	InputBox.SetParam1(strCaption, strLabel, strValue);
	if ( InputBox.DoModal() == IDCANCEL ) 
	{	return ;  }
	const int Gray = ::_ttoi(InputBox.m_DataEdit1);
	if ( Gray < 0 ) { return; }
	if ( AOIDataCollect.SetSystemSliceParamTargetGray(Gray) == false )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return;
	}
	BuildSliceListWnd();
	BuildSliceCombox();	
	return;
}
//-------------------------------------------------------------------------------------//
void CImageConfigWnd::OnSliceSetAllVerifyTolBtn() 
{
	// TODO: Add your control notification handler code here
	CString      str;		
	CString      strTol;
	CString      strValue;
	CString      strLabel;
	CString      strCaption;		
	CInputBoxWnd InputBox;

	CWnd::GetDlgItemText(IMGCFG_SLICE_VERIFY_TOL_EDIT, strTol);
	strCaption = _T("Input Tolerance");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strLabel = _T("Tolerance");
	strLabel = LoadMultiLanguageString(strLabel, strLabel);
	strValue = strTol;
	InputBox.SetParam1(strCaption, strLabel, strValue);
	if ( InputBox.DoModal() == IDCANCEL ) 
	{	return ;  }
	const int Tol = ::_ttoi(InputBox.m_DataEdit1);
	if ( Tol < 0 ) { return; }	
	if ( AOIDataCollect.SetSystemSliceParamVerifyTolerance(Tol) == false )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return;
	}
	BuildSliceListWnd();
	BuildSliceCombox();	
	return;
}
//-------------------------------------------------------------------------------------//