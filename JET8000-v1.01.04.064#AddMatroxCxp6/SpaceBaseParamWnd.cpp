// SpaceBaseParamWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "SpaceBaseParamWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CSpaceBaseParamWnd dialog
//-------------------------------------------------------------------------------------//
CSpaceBaseParamWnd::CSpaceBaseParamWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CSpaceBaseParamWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSpaceBaseParamWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ProjectPtr = NULL;
	m_DefaultIndex = -1;
	m_BaseColorIndex = -1;
	m_LocalBasePlaneID = 0;
	m_BaseColorEnabled = false;	
	m_BasePlaneParamSetting = true;
	m_BasePlaneColorSetting = true;
	m_StopBasePlaneListBeSelected = false;
}
//-------------------------------------------------------------------------------------//
void CSpaceBaseParamWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSpaceBaseParamWnd)
	DDX_Control(pDX, SBPW_PARAM_LIST_WND, m_BasePlaneListCtrl);			
	DDX_Control(pDX, SBPW_LEVEL_PROC_TYPE_COMBO, m_BaseProcCombox);	
	DDX_Control(pDX, SBPW_LEVEL_CALC_MODE_COMBO, m_CalcBaseCombox);	
	DDX_Control(pDX, SBPW_LEVEL_AUTO_RGN_MODE_COMBO, m_AutoRegionCombox);		
	DDX_Control(pDX, SBPW_USE_BODY_OUTSIDE_COMBO, m_BodyOutsideCombox);	
	DDX_Control(pDX, SBPW_LEVEL_TOWARD_MODE_COMBO, m_TowardModeCombox);	
	DDX_Control(pDX, SBPW_LEVEL_FILTER_MODEL_COMBO, m_FilterModeCombox);
	DDX_Control(pDX, SBPW_LEVEL_FILTER_MODEL_2D_COMBO, m_FilterMode2DCombox);	
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CSpaceBaseParamWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CSpaceBaseParamWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(SBPW_USE_SIDE_ALL_BTN, OnUseSideAllBtn)
	ON_BN_CLICKED(SBPW_PARAM_SETTING_CHK, OnParamSettingChk)
	ON_BN_CLICKED(SBPW_COLOR_SETTING_CHK, OnColorSettingChk)
	ON_NOTIFY(NM_CLICK, SBPW_PARAM_LIST_WND, OnClickParamListWnd)
	ON_NOTIFY(LVN_ITEMCHANGED, SBPW_PARAM_LIST_WND, OnItemchangedParamListWnd)
	ON_BN_CLICKED(SBPW_SEND_TO_BTN_01, OnSendToBtn01)
	ON_BN_CLICKED(SBPW_SEND_TO_BTN_02, OnSendToBtn02)
	ON_BN_CLICKED(SBPW_SEND_TO_BTN_03, OnSendToBtn03)
	ON_BN_CLICKED(SBPW_SEND_TO_BTN_04, OnSendToBtn04)
	ON_BN_CLICKED(SBPW_SEND_TO_BTN_05, OnSendToBtn05)
	ON_BN_CLICKED(SBPW_SEND_TO_BTN_06, OnSendToBtn06)
	ON_BN_CLICKED(SBPW_SEND_TO_BTN_07, OnSendToBtn07)
	ON_BN_CLICKED(SBPW_SEND_TO_BTN_08, OnSendToBtn08)
	ON_BN_CLICKED(SBPW_UNLINK_BTN, OnUnlinkBtn)
	ON_BN_CLICKED(SBPW_SAVE_PARAM_BTN, OnSaveParamBtn)
	ON_BN_CLICKED(SBPW_LOAD_PARAM_BTN, OnLoadParamBtn)
	ON_CBN_SELCHANGE(SBPW_LEVEL_CALC_MODE_COMBO, OnSelchangeLevelCalcModeCombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CSpaceBaseParamWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CSpaceBaseParamWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	JetAPI::InitialListCtrl(m_BasePlaneListCtrl);	
	BuildFilterMode2DCombox(m_FilterMode2DCombox);
	AOIDataDefine.BuildBasePlaneProcCombox(m_BaseProcCombox);
	AOIDataDefine.BuildCalcBasePlaneCombox(m_CalcBaseCombox);
	AOIDataDefine.BuildBasePlaneAutoRegionCombox(m_AutoRegionCombox);
	AOIDataDefine.BuildBasePlaneBodyOutsideCombox(m_BodyOutsideCombox);	
	AOIDataDefine.BuildBasePlaneTowardCombox(m_TowardModeCombox);
	AOIDataDefine.BuidlSpaceNoiseFilterModeCombox(m_FilterModeCombox);
	BuildBasePlaneListWndHeader();
	BuildBasePlaneListWnd();

	SwitchMultiLanguage();
	LoadProjectBaseColorName();
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());

	UpdateParamToUI();
	UpdateUIEnable();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CSpaceBaseParamWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CSpaceBaseParamWnd::OnSize(UINT nType, int cx, int cy) 
{
	CBaseDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CSpaceBaseParamWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CBaseDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CSpaceBaseParamWnd::OnOK() 
{
	// TODO: Add extra validation here
	UpdateUIToParam();

	const int Index = m_BasePlaneParam.BasePlaneIndex;
	if ( Index>=0 && Index<MAX_SYSTEM_BASE_PLANE_PARAM_COUNT )
	{	AOIDataCollect.SetSystemBasePlaneParam(Index, m_BasePlaneParam);	}
	CBaseDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CSpaceBaseParamWnd::SetProjectPtr(CAOIProject *ProjectPtr)
{
	m_ProjectPtr = ProjectPtr;
}
//-------------------------------------------------------------------------------------//
bool CSpaceBaseParamWnd::GetBaseColorEnabled()
{
	return m_BaseColorEnabled;
}
//-------------------------------------------------------------------------------------//
void CSpaceBaseParamWnd::SetBaseColorEnabled(bool val)
{
	m_BaseColorEnabled = val;
}
//-------------------------------------------------------------------------------------//
int CSpaceBaseParamWnd::GetBaseColorIndex()
{
	return m_BaseColorIndex;
}
//-------------------------------------------------------------------------------------//
void CSpaceBaseParamWnd::SetBaseColorIndex(int val)
{
	m_BaseColorIndex = val;
}
//-------------------------------------------------------------------------------------//
CAOIProject* CSpaceBaseParamWnd::GetProjectPtr()
{
	return m_ProjectPtr;
}
//-------------------------------------------------------------------------------------//
int CSpaceBaseParamWnd::GetLocalBasePlaneID()
{
	return m_LocalBasePlaneID;
}
//-------------------------------------------------------------------------------------//
void CSpaceBaseParamWnd::SetLocalBasePlaneID(int val)
{
	m_LocalBasePlaneID = val;
}
//-------------------------------------------------------------------------------------//
void CSpaceBaseParamWnd::SetBasePlaneParam(const TBasePlaneParam &Param)
{
	m_BasePlaneParam = Param;
	m_BasePlaneParamDefault = Param;
	m_DefaultIndex = Param.BasePlaneIndex;
}
//-------------------------------------------------------------------------------------//
void CSpaceBaseParamWnd::GetBasePlaneParam(TBasePlaneParam &Param)
{
	Param = m_BasePlaneParam;
}
//-------------------------------------------------------------------------------------//
bool CSpaceBaseParamWnd::GetBasePlaneParamSetting()
{
	return m_BasePlaneParamSetting;
}
//-------------------------------------------------------------------------------------//
bool CSpaceBaseParamWnd::GetBasePlaneColorSetting()
{
	return m_BasePlaneColorSetting;
}
//-------------------------------------------------------------------------------------//
void CSpaceBaseParamWnd::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_SPACE_BASE_PARAM_WND");
	//---------------------------------------------------------------------------------//
	WndID = IDD_SPACE_BASE_PARAM_WND;
	WndKey = _T("IDD_SPACE_BASE_PARAM_WND");
	this->GetWindowText(LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetWindowText(NewLabelText);
	//---------------------------------------------------------------------------------//
	NewLabelText = AOIDataDefine.GetWndOKText();
	this->SetDlgItemText(IDOK, NewLabelText);	

	NewLabelText = AOIDataDefine.GetWndCancelText();
	this->SetDlgItemText(IDCANCEL, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = SBPW_LEVEL_GROUP;
	WndKey = _T("SBPW_LEVEL_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SBPW_PARAM_SETTING_CHK;
	WndKey = _T("SBPW_PARAM_SETTING_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SBPW_LEVEL_PROC_TYPE_LABEL;
	WndKey = _T("SBPW_LEVEL_PROC_TYPE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SBPW_LEVEL_CALC_MODE_LABEL;
	WndKey = _T("SBPW_LEVEL_CALC_MODE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SBPW_USE_INNER_CHK;
	WndKey = _T("SBPW_USE_INNER_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SBPW_USE_SIDE_CHK_T;
	WndKey = _T("SBPW_USE_SIDE_CHK_T");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SBPW_USE_SIDE_CHK_L;
	WndKey = _T("SBPW_USE_SIDE_CHK_L");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	
	WndID = SBPW_USE_SIDE_CHK_B;
	WndKey = _T("SBPW_USE_SIDE_CHK_B");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SBPW_USE_SIDE_CHK_R;
	WndKey = _T("SBPW_USE_SIDE_CHK_R");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SBPW_USE_SIDE_ALL_BTN;
	WndKey = _T("SBPW_USE_SIDE_ALL_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SBPW_LEVEL_AUTO_RGN_MODE_LABEL;
	WndKey = _T("SBPW_LEVEL_AUTO_RGN_MODE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SBPW_LEVEL_AUTO_RGN_MAX_GAP_LABEL;
	WndKey = _T("SBPW_LEVEL_AUTO_RGN_MAX_GAP_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = SBPW_USE_BODY_OUTSIDE_LABEL;
	WndKey = _T("SBPW_USE_BODY_OUTSIDE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SBPW_BODY_OUTSIDE_SIZE_X_LABEL;
	WndKey = _T("SBPW_BODY_OUTSIDE_SIZE_X_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SBPW_BODY_OUTSIDE_SIZE_Y_LABEL;
	WndKey = _T("SBPW_BODY_OUTSIDE_SIZE_Y_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SBPW_LEVEL_SYSTEM_NOISE_LABEL;
	WndKey = _T("SBPW_LEVEL_SYSTEM_NOISE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = SBPW_LEVEL_MAX_TILT_ANGLE_LABEL;
	WndKey = _T("SBPW_LEVEL_MAX_TILT_ANGLE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = SBPW_LEVEL_UPPER_RATIO_LABEL;
	WndKey = _T("SBPW_LEVEL_UPPER_RATIO_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = SBPW_LEVEL_LOWER_RATIO_LABEL;
	WndKey = _T("SBPW_LEVEL_LOWER_RATIO_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = SBPW_LEVEL_OFFSET_Z_LABEL;
	WndKey = _T("SBPW_LEVEL_OFFSET_Z_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	
	WndID = SBPW_LEVEL_PLANE_RATIO_LSL_LABEL;
	WndKey = _T("SBPW_LEVEL_PLANE_RATIO_LSL_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = SBPW_LEVEL_PLANE_RATIO_USL_LABEL;
	WndKey = _T("SBPW_LEVEL_PLANE_RATIO_USL_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = SBPW_LEVEL_RANGE_RATIO_MIN_LABEL;
	WndKey = _T("SBPW_LEVEL_RANGE_RATIO_MIN_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = SBPW_LEVEL_RANGE_RATIO_MAX_LABEL;
	WndKey = _T("SBPW_LEVEL_RANGE_RATIO_MAX_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = SBPW_LEVEL_OVER_HIGH_LABEL;
	WndKey = _T("SBPW_LEVEL_OVER_HIGH_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = SBPW_LEVEL_OVER_LOW_LABEL;
	WndKey = _T("SBPW_LEVEL_OVER_LOW_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	
	WndID = SBPW_PLANE_CLIP_ROTATED_OUTER_CHK;
	WndKey = _T("SBPW_PLANE_CLIP_ROTATED_OUTER_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SBPW_LEVEL_LOCAL_PLANE_ID_LABEL;
	WndKey = _T("SBPW_LEVEL_LOCAL_PLANE_ID_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SBPW_LEVEL_PLANE_READING_LABEL;
	WndKey = _T("SBPW_LEVEL_PLANE_READING_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SBPW_LEVEL_TOWARD_MODE_LABEL;
	WndKey = _T("SBPW_LEVEL_TOWARD_MODE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = SBPW_LEVEL_FILTER_GROUP;
	WndKey = _T("SBPW_LEVEL_FILTER_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SBPW_LEVEL_FILTER_MODEL_LABEL;
	WndKey = _T("SBPW_LEVEL_FILTER_MODEL_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SBPW_LEVEL_FILTER_PITCH_LABEL;
	WndKey = _T("SBPW_LEVEL_FILTER_PITCH_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SBPW_LEVEL_FILTER_KER_LABEL;
	WndKey = _T("SBPW_LEVEL_FILTER_KER_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SBPW_LEVEL_FILTER_USE_LABEL;
	WndKey = _T("SBPW_LEVEL_FILTER_USE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = SBPW_PLANE_COLOR_GROUP;
	WndKey = _T("SBPW_PLANE_COLOR_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = SBPW_COLOR_SETTING_CHK;
	WndKey = _T("SBPW_COLOR_SETTING_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = SBPW_PLANE_COLOR_RAD_1;
	WndKey = _T("SBPW_PLANE_COLOR_RAD_1");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SBPW_PLANE_COLOR_RAD_2;
	WndKey = _T("SBPW_PLANE_COLOR_RAD_2");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SBPW_PLANE_COLOR_RAD_3;
	WndKey = _T("SBPW_PLANE_COLOR_RAD_3");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SBPW_PLANE_COLOR_RAD_4;
	WndKey = _T("SBPW_PLANE_COLOR_RAD_4");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SBPW_PLANE_COLOR_RAD_5;
	WndKey = _T("SBPW_PLANE_COLOR_RAD_5");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SBPW_PLANE_COLOR_RAD_6;
	WndKey = _T("SBPW_PLANE_COLOR_RAD_6");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SBPW_PLANE_COLOR_RAD_7;
	WndKey = _T("SBPW_PLANE_COLOR_RAD_7");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SBPW_PLANE_COLOR_RAD_8;
	WndKey = _T("SBPW_PLANE_COLOR_RAD_8");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SBPW_PLANE_COLOR_RAD_OFF;
	WndKey = _T("SBPW_PLANE_COLOR_RAD_OFF");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SBPW_LEVEL_FILTER_MODEL_2D_LABEL;
	WndKey = _T("SBPW_LEVEL_FILTER_MODEL_2D_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SBPW_LEVEL_FILTER_KER_SIZE_2D_LABEL;
	WndKey = _T("SBPW_LEVEL_FILTER_KER_SIZE_2D_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//	
	WndID = SBPW_UNLINK_BTN;
	WndKey = _T("SBPW_UNLINK_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SBPW_SEND_TO_BTN_01;
	WndKey = _T("SBPW_SEND_TO_BTN_01");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SBPW_SEND_TO_BTN_02;
	WndKey = _T("SBPW_SEND_TO_BTN_02");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SBPW_SEND_TO_BTN_03;
	WndKey = _T("SBPW_SEND_TO_BTN_03");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SBPW_SEND_TO_BTN_04;
	WndKey = _T("SBPW_SEND_TO_BTN_04");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SBPW_SEND_TO_BTN_05;
	WndKey = _T("SBPW_SEND_TO_BTN_05");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SBPW_SEND_TO_BTN_06;
	WndKey = _T("SBPW_SEND_TO_BTN_06");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SBPW_SEND_TO_BTN_07;
	WndKey = _T("SBPW_SEND_TO_BTN_07");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SBPW_SEND_TO_BTN_08;
	WndKey = _T("SBPW_SEND_TO_BTN_08");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = SBPW_SAVE_PARAM_BTN;
	WndKey = _T("SBPW_SAVE_PARAM_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SBPW_LOAD_PARAM_BTN;
	WndKey = _T("SBPW_LOAD_PARAM_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//	
}
//-------------------------------------------------------------------------------------//
CString CSpaceBaseParamWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_SPACE_BASE_PARAM_WND");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
bool CSpaceBaseParamWnd::BuildFilterMode2DCombox(CComboBox &Combox)
{
	size_t       i=0;
	int          idx=0;	
	CString      String;
	NOISE_FILTER_MODE FilterMode;
	if ( Combox.GetSafeHwnd() == NULL ) { return false; }
	
	idx = 0;
	JetAPI::ClearCombox(Combox);	
	
	FilterMode = NOISE_FILTER_DISABLE;
	String = AOIDataDefine.GetAlgNoiseFilterModeText(FilterMode);
	Combox.InsertString(-1, String);
	Combox.SetItemData(idx, FilterMode);
	idx ++;

	FilterMode = NOISE_FILTER_OPEN;
	String = AOIDataDefine.GetAlgNoiseFilterModeText(FilterMode);
	Combox.InsertString(-1, String);
	Combox.SetItemData(idx, FilterMode);
	idx ++;

	FilterMode = NOISE_FILTER_CLOSE;
	String = AOIDataDefine.GetAlgNoiseFilterModeText(FilterMode);
	Combox.InsertString(-1, String);
	Combox.SetItemData(idx, FilterMode);
	idx ++;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CSpaceBaseParamWnd::BuildBasePlaneList()
{	
	m_BasePlaneParamList.clear();
	const int DefaultIndex = m_DefaultIndex;//m_NoiseFilterParamDefault.DataFilterIndex;
	AOIDataCollect.CloneSystemBasePlaneParamList(m_BasePlaneParamList);

	const int FilterCount = (int)(m_BasePlaneParamList.size());
	if ( DefaultIndex<0 || DefaultIndex>=FilterCount )
	{	m_BasePlaneParamList.push_back(m_BasePlaneParam);	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CSpaceBaseParamWnd::BuildBasePlaneListWndHeader()
{
	CString str;
	int   nCol = 0;	
	int width  = 0;
	int widthF = 64;
	int width1 = 48;
	int width2 = 64;
	int width3 = 48;
	RECT      Rect;	
	const int Align = LVCFMT_LEFT;//LVCFMT_CENTER;LVCFMT_RIGHT, LVCFMT_LEFT
	CThisListCtrl_41 &ListCtrl = m_BasePlaneListCtrl;

	ListCtrl.GetClientRect(&Rect);
	widthF = (Rect.right-Rect.left-8);
	width1 = 48;
	width3 = 48;

	width = width1;
	str = AOIDataDefine.GetIndexText();	
	ListCtrl.InsertColumn(nCol, str, Align, width);
	nCol ++;
	
	width = (widthF-width1-width3);
	str = _T("Information");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(nCol, str, Align, width);
	nCol ++;

	width = width3;
	str = _T("Use");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(nCol, str, Align, width);
	nCol ++;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CSpaceBaseParamWnd::BuildBasePlaneListWnd()
{
	CThisListCtrl_41 &ListCtrl = m_BasePlaneListCtrl;
	m_StopBasePlaneListBeSelected = true;
	JetAPI::ClearListCtrl(ListCtrl, FALSE);
	m_StopBasePlaneListBeSelected = false;	

	int           i=0;
	CString       str;
	CString       strIndex;
	CString       strValue;	
	CString       strCaption;	
	int           nItem=0;		
	int           nItemSel=-1;
	const int     nSubItem1 = 1;
	const int     nSubItem2 = 2;
	COLORREF      clrTextBkLink = 0x0000FF;
	COLORREF      clrTextBkUnLink = 0xA0A0FF;
	TBasePlaneParam *BasePlanePtr=NULL;
	
	BuildBasePlaneList();
	const int DefaultIndex = m_DefaultIndex;
	const int ParamCount = (int)(m_BasePlaneParamList.size());
	

	nItem=0;
	nItemSel=-1;
	ListCtrl.SetRedraw(FALSE);
	m_StopBasePlaneListBeSelected = true;
	for ( i=0; i<ParamCount; i++ )
	{
		BasePlanePtr = &(m_BasePlaneParamList[i]);
		if ( NULL == BasePlanePtr ) { continue; }		

		strIndex.Format(_T("%d"), nItem+1);		
		strValue = BasePlanePtr->BasePlaneInfoText;
		ListCtrl.InsertItem(nItem, strIndex);
		ListCtrl.SetItemData(nItem, i);		
		ListCtrl.SetItemText(nItem, nSubItem1, strValue);
		if ( -1 == nItemSel )
		{
			if ( DefaultIndex == BasePlanePtr->BasePlaneIndex )
			{
				nItemSel = nItem;
				ListCtrl.SetItemText(nItem, nSubItem2, _T("*"));	
			}
		}
		if ( i >= MAX_SYSTEM_BASE_PLANE_PARAM_COUNT )
		{	ListCtrl.SetItemTextBkColor(nItem, clrTextBkUnLink);	}
		nItem ++;
	}	
	m_StopBasePlaneListBeSelected = false;
	ListCtrl.SetRedraw(TRUE);

	if ( nItemSel > 0 ) 
	{	ListCtrl.SetItemState(nItemSel, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CSpaceBaseParamWnd::LoadProjectBaseColorName()
{
	CAOIProject *ProjectPtr = GetProjectPtr();
	if ( NULL == ProjectPtr ) { return; }
	
	size_t  i=0, j=0;
	UINT    CtrlID=0;
	CString strText, str;
	CString strUnset = AOIDataDefine.GetUnsetText();
	CColorGroup *ColorGroupPtr = NULL;	
	for ( i=PROJECT_COLOR_ID_BOARD_BEGIN; i<=PROJECT_COLOR_ID_BOARD_END; i++ )
	{		
		strText = AOIDataDefine.GetProjectColorGroupText(i);
		ColorGroupPtr = ProjectPtr->GetProjectColorGroupPtr(i, true);
		if ( NULL != ColorGroupPtr ) 
		{
			if ( ColorGroupPtr->CheckColorGroupUsed() == false ) 
			{
				str.Format(_T("%s  [%s]"), strText, strUnset);
				strText = str;
			}
		}

		j = (i-PROJECT_COLOR_ID_BOARD_BEGIN);
		switch ( j )
		{
		case 0:	CtrlID = SBPW_PLANE_COLOR_RAD_1;	break;
		case 1:	CtrlID = SBPW_PLANE_COLOR_RAD_2;	break;
		case 2:	CtrlID = SBPW_PLANE_COLOR_RAD_3;	break;
		case 3:	CtrlID = SBPW_PLANE_COLOR_RAD_4;	break;
		case 4:	CtrlID = SBPW_PLANE_COLOR_RAD_5;	break;
		case 5:	CtrlID = SBPW_PLANE_COLOR_RAD_6;	break;
		case 6:	CtrlID = SBPW_PLANE_COLOR_RAD_7;	break;
		case 7:	CtrlID = SBPW_PLANE_COLOR_RAD_8;	break;
		default:
			CtrlID = -1;
			break;
		}
		if ( -1 == CtrlID ) { continue; }
		CWnd::SetDlgItemText(CtrlID, strText);
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CSpaceBaseParamWnd::UpdateUIEnable()
{
	bool  bEnableSieMode=false;
	bool  bEnbNoiseRange=false;
	bool  bEnbMaxTiltAngle=false;
	bool  bEnbUpperLowerRatio=false;
	bool  bEnbPlaneRatio=false;
	bool  bEnbRangeRatio=false;
	const TBasePlaneParam &Param = m_BasePlaneParam;
	CALC_BASE_PLANE_MODE CalcBasePlaneMode = Param.CalcBasePlaneMode;
	switch ( CalcBasePlaneMode )
	{
	case CALC_BASE_PLANE_DISABLE:
		break;	
	case CALC_BASE_PLANE_AVERAGE://平均值
		break;
	case CALC_BASE_PLANE_CORNER://四個端點
		bEnableSieMode = true;
		bEnbNoiseRange = true;
		bEnbMaxTiltAngle = true;
		bEnbUpperLowerRatio = true;
		break;
	case CALC_BASE_PLANE_ISO_DATA://Iso data
		break;
	case CALC_BASE_PLANE_OTSU://OTSU
		break;
	case CALC_BASE_PLANE_CORNER_ONLY://只有四個端點
		bEnableSieMode = true;
		bEnbNoiseRange = true;
		bEnbMaxTiltAngle = true;
		bEnbUpperLowerRatio = true;
		break;
	case CALC_BASE_PLANE_SURROUND://外圍
		bEnableSieMode = true;
		bEnbNoiseRange = true;
		bEnbMaxTiltAngle = true;
		bEnbUpperLowerRatio = true;
		break;
	case CALC_BASE_PLANE_AUTO_LOWER://自動找最低
		bEnableSieMode = true;
		bEnbNoiseRange = true;
		bEnbPlaneRatio = true;
		bEnbRangeRatio = true;
		break;
	case CALC_BASE_PLANE_PANEL:
		bEnbNoiseRange = true;
		bEnableSieMode = true;
		break;
	case CALC_BASE_PLANE_LOCAL:
		bEnbNoiseRange = true;
		//bEnableSieMode = true;
		break;
	default:
		bEnableSieMode = true;
		bEnbNoiseRange=true;
		bEnbMaxTiltAngle=true;
		bEnbUpperLowerRatio=true;
		bEnbPlaneRatio=true;
		bEnbRangeRatio=true;
		break;
	}
	JetAPI::EnableCtrlWnd(this, SBPW_USE_INNER_CHK, bEnableSieMode);
	JetAPI::EnableCtrlWnd(this, SBPW_USE_SIDE_CHK_T, bEnableSieMode);
	JetAPI::EnableCtrlWnd(this, SBPW_USE_SIDE_CHK_L, bEnableSieMode);
	JetAPI::EnableCtrlWnd(this, SBPW_USE_SIDE_CHK_B, bEnableSieMode);
	JetAPI::EnableCtrlWnd(this, SBPW_USE_SIDE_CHK_R, bEnableSieMode);	
	JetAPI::EnableCtrlWnd(this, SBPW_USE_BODY_OUTSIDE_COMBO, bEnableSieMode);	
	JetAPI::EnableEditWnd(this, SBPW_BODY_OUTSIDE_SIZE_X_EDIT, bEnableSieMode);	
	JetAPI::EnableEditWnd(this, SBPW_BODY_OUTSIDE_SIZE_Y_EDIT, bEnableSieMode);		
	JetAPI::EnableCtrlWnd(this, SBPW_LEVEL_AUTO_RGN_MODE_COMBO, bEnableSieMode);
	JetAPI::EnableEditWnd(this, SBPW_LEVEL_SYSTEM_NOISE_EDIT, bEnbNoiseRange);
	JetAPI::EnableEditWnd(this, SBPW_LEVEL_MAX_TILT_ANGLE_EDIT, bEnbMaxTiltAngle);
	JetAPI::EnableEditWnd(this, SBPW_LEVEL_UPPER_RATIO_EDIT, bEnbUpperLowerRatio);
	JetAPI::EnableEditWnd(this, SBPW_LEVEL_LOWER_RATIO_EDIT, bEnbUpperLowerRatio);
	JetAPI::EnableEditWnd(this, SBPW_LEVEL_PLANE_RATIO_LSL_EDIT, bEnbPlaneRatio);
	JetAPI::EnableEditWnd(this, SBPW_LEVEL_PLANE_RATIO_USL_EDIT, bEnbPlaneRatio);
	JetAPI::EnableEditWnd(this, SBPW_LEVEL_RANGE_RATIO_MIN_EDIT, bEnbRangeRatio);
	JetAPI::EnableEditWnd(this, SBPW_LEVEL_RANGE_RATIO_MAX_EDIT, bEnbRangeRatio);
	return ;
}
//-------------------------------------------------------------------------------------//
void CSpaceBaseParamWnd::UpdateParamToUI()
{
	CString str;
	bool    bCheck=true;
	const TBasePlaneParam &Param = m_BasePlaneParam;
	const DWORD  UseSideMode=Param.UseSideMode;

	CWnd::CheckDlgButton(SBPW_PARAM_SETTING_CHK, m_BasePlaneParamSetting);
	CWnd::CheckDlgButton(SBPW_COLOR_SETTING_CHK, m_BasePlaneColorSetting);
	
	//Leveling //基準面	
	JetAPI::SetComboxCurSel(m_BaseProcCombox, Param.BasePlaneProcType);
	JetAPI::SetComboxCurSel(m_CalcBaseCombox, Param.CalcBasePlaneMode);
	JetAPI::SetComboxCurSel(m_AutoRegionCombox, Param.BasePlaneAutRgnMode);
	JetAPI::SetComboxCurSel(m_BodyOutsideCombox, Param.BodyOutsideMode);	
	JetAPI::SetComboxCurSel(m_TowardModeCombox, Param.BasePlaneToward);	

	CWnd::CheckDlgButton(SBPW_USE_INNER_CHK, Param.UseInnerMode);	
	bCheck = JetAPI::BitMask_Check(UseSideMode, BOX_TOWARD_UP);
	CWnd::CheckDlgButton(SBPW_USE_SIDE_CHK_T, bCheck);
	bCheck = JetAPI::BitMask_Check(UseSideMode, BOX_TOWARD_LEFT);
	CWnd::CheckDlgButton(SBPW_USE_SIDE_CHK_L, bCheck);
	bCheck = JetAPI::BitMask_Check(UseSideMode, BOX_TOWARD_DOWN);
	CWnd::CheckDlgButton(SBPW_USE_SIDE_CHK_B, bCheck);
	bCheck = JetAPI::BitMask_Check(UseSideMode, BOX_TOWARD_RIGHT);
	CWnd::CheckDlgButton(SBPW_USE_SIDE_CHK_R, bCheck);	
	CWnd::SetDlgItemInt(SBPW_LEVEL_AUTO_RGN_MAX_GAP_EDIT, (UINT)(Param.BaePlaneAutRgnMaxGap));

	CWnd::SetDlgItemInt(SBPW_BODY_OUTSIDE_SIZE_X_EDIT, (UINT)(Param.BodyOutsideW));
	CWnd::SetDlgItemInt(SBPW_BODY_OUTSIDE_SIZE_Y_EDIT, (UINT)(Param.BodyOutsideH));

	CWnd::SetDlgItemInt(SBPW_LEVEL_SYSTEM_NOISE_EDIT, Param.SystemNoiseRange);
	CWnd::SetDlgItemInt(SBPW_LEVEL_MAX_TILT_ANGLE_EDIT, Param.MaxTiltAngle);
	CWnd::SetDlgItemInt(SBPW_LEVEL_UPPER_RATIO_EDIT, (UINT)(Param.UpperRatio*100.0));
	CWnd::SetDlgItemInt(SBPW_LEVEL_LOWER_RATIO_EDIT, (UINT)(Param.LowerRatio*100.0));
	CWnd::SetDlgItemInt(SBPW_LEVEL_OFFSET_Z_EDIT, Param.OffsetZ);
	CWnd::SetDlgItemInt(SBPW_LEVEL_PLANE_RATIO_LSL_EDIT, (UINT)(Param.PlaneRatioLSL));
	CWnd::SetDlgItemInt(SBPW_LEVEL_PLANE_RATIO_USL_EDIT, (UINT)(Param.PlaneRatioUSL));
	CWnd::SetDlgItemInt(SBPW_LEVEL_RANGE_RATIO_MIN_EDIT, (UINT)(Param.RangeRatioMin));
	CWnd::SetDlgItemInt(SBPW_LEVEL_RANGE_RATIO_MAX_EDIT, (UINT)(Param.RangeRatioMax));

	CWnd::SetDlgItemInt(SBPW_LEVEL_OVER_HIGH_EDIT, (int)(Param.OverHighFilter));
	CWnd::SetDlgItemInt(SBPW_LEVEL_OVER_LOW_EDIT, (int)(Param.OverLowFilter));
	str.Format(_T("%.0f"), Param.OverHighFilter);
	//CWnd::SetDlgItemText(SBPW_LEVEL_OVER_HIGH_EDIT, str);
	str.Format(_T("%.0f"), Param.OverLowFilter);
	//CWnd::SetDlgItemText(SBPW_LEVEL_OVER_LOW_EDIT, str);
	
	CWnd::SetDlgItemInt(SBPW_LEVEL_LOCAL_PLANE_ID_EDIT, (m_LocalBasePlaneID));	
	CWnd::SetDlgItemInt(SBPW_LEVEL_PLANE_READING_EDIT, (int)(Param.NormalZ));	

	CWnd::CheckDlgButton(SBPW_PLANE_CLIP_ROTATED_OUTER_CHK, Param.RotatedClip);
	
	JetAPI::SetComboxCurSel(m_FilterModeCombox, Param.FilterMode);
	CWnd::SetDlgItemInt(SBPW_LEVEL_FILTER_PITCH_EDIT, Param.FilterPitch);
	CWnd::SetDlgItemInt(SBPW_LEVEL_FILTER_KER_EDIT, Param.FilterKerSize);
	CWnd::SetDlgItemInt(SBPW_LEVEL_FILTER_USE_EDIT, Param.FilterUseSize);
	//CWnd::SetDlgItemInt(SBPW_LEVEL_FILTER_PITCH_EDIT, Param.FilterIterCount);
	
	if ( false == m_BaseColorEnabled )
	{	CWnd::CheckDlgButton(SBPW_PLANE_COLOR_RAD_OFF, TRUE);	}
	else
	{
		switch ( m_BaseColorIndex ) 
		{
		case 0:	CWnd::CheckDlgButton(SBPW_PLANE_COLOR_RAD_1, TRUE); break;
		case 1:	CWnd::CheckDlgButton(SBPW_PLANE_COLOR_RAD_2, TRUE); break;
		case 2:	CWnd::CheckDlgButton(SBPW_PLANE_COLOR_RAD_3, TRUE); break;
		case 3:	CWnd::CheckDlgButton(SBPW_PLANE_COLOR_RAD_4, TRUE); break;
		case 4:	CWnd::CheckDlgButton(SBPW_PLANE_COLOR_RAD_5, TRUE); break;
		case 5:	CWnd::CheckDlgButton(SBPW_PLANE_COLOR_RAD_6, TRUE); break;
		case 6:	CWnd::CheckDlgButton(SBPW_PLANE_COLOR_RAD_7, TRUE); break;
		case 7:	CWnd::CheckDlgButton(SBPW_PLANE_COLOR_RAD_8, TRUE); break;
		}
	}

	JetAPI::SetComboxCurSel(m_FilterMode2DCombox, Param.FilterMode2D);
	CWnd::SetDlgItemInt(SBPW_LEVEL_FILTER_KER_SIZE_2D_EDIT, Param.FilterKerSize2D);	

	CWnd::SetDlgItemText(SBPW_INFO_TEXT_EDIT, Param.BasePlaneInfoText);
	
	return;
}
//-------------------------------------------------------------------------------------//
bool CSpaceBaseParamWnd::UpdateUIToParam()
{
	CString str;
	DWORD   UseSideMode=0;
	TBasePlaneParam &Param = m_BasePlaneParam;

	if ( CWnd::IsDlgButtonChecked(SBPW_PARAM_SETTING_CHK) == TRUE )
	{	m_BasePlaneParamSetting = true; }
	else
	{	m_BasePlaneParamSetting = false; }

	if ( CWnd::IsDlgButtonChecked(SBPW_COLOR_SETTING_CHK) == TRUE )
	{	m_BasePlaneColorSetting = true; }
	else
	{	m_BasePlaneColorSetting = false; }

	//Leveling //基準面	
	Param.BasePlaneProcType = (BASE_PLANE_PROC_TYPE)(JetAPI::GetComboxCurSelData(m_BaseProcCombox));	
	Param.CalcBasePlaneMode = (CALC_BASE_PLANE_MODE)(JetAPI::GetComboxCurSelData(m_CalcBaseCombox));	
	Param.BasePlaneAutRgnMode = (BASE_PLANE_AUTO_REGION_MODE)(JetAPI::GetComboxCurSelData(m_AutoRegionCombox));
	Param.BodyOutsideMode = (BASE_PLANE_BODY_OUTSIDE_MODE)(JetAPI::GetComboxCurSelData(m_BodyOutsideCombox));	
	Param.BasePlaneToward = (BASE_PLANE_TOWARD_MODE)(JetAPI::GetComboxCurSelData(m_TowardModeCombox));		

	if ( CWnd::IsDlgButtonChecked(SBPW_USE_INNER_CHK) == TRUE )
	{	Param.UseInnerMode = true;	}
	else
	{	Param.UseInnerMode = false;	}
	if ( CWnd::IsDlgButtonChecked(SBPW_USE_SIDE_CHK_T) == TRUE )
	{	UseSideMode = JetAPI::BitMask_Add(UseSideMode, BOX_TOWARD_UP);	}
	if ( CWnd::IsDlgButtonChecked(SBPW_USE_SIDE_CHK_L) == TRUE )
	{	UseSideMode = JetAPI::BitMask_Add(UseSideMode, BOX_TOWARD_LEFT);	}
	if ( CWnd::IsDlgButtonChecked(SBPW_USE_SIDE_CHK_B) == TRUE )
	{	UseSideMode = JetAPI::BitMask_Add(UseSideMode, BOX_TOWARD_DOWN);	}
	if ( CWnd::IsDlgButtonChecked(SBPW_USE_SIDE_CHK_R) == TRUE )
	{	UseSideMode = JetAPI::BitMask_Add(UseSideMode, BOX_TOWARD_RIGHT);	}	
	Param.UseSideMode = UseSideMode;
	Param.BaePlaneAutRgnMaxGap = (int)(CWnd::GetDlgItemInt(SBPW_LEVEL_AUTO_RGN_MAX_GAP_EDIT));	
	
	Param.BodyOutsideW = (int)(CWnd::GetDlgItemInt(SBPW_BODY_OUTSIDE_SIZE_X_EDIT));	
	Param.BodyOutsideH = (int)(CWnd::GetDlgItemInt(SBPW_BODY_OUTSIDE_SIZE_Y_EDIT));	

	Param.SystemNoiseRange = (int)(CWnd::GetDlgItemInt(SBPW_LEVEL_SYSTEM_NOISE_EDIT));	
	Param.MaxTiltAngle = (int)(CWnd::GetDlgItemInt(SBPW_LEVEL_MAX_TILT_ANGLE_EDIT));	
	Param.UpperRatio = (int)(CWnd::GetDlgItemInt(SBPW_LEVEL_UPPER_RATIO_EDIT))*0.01;	
	Param.LowerRatio = (int)(CWnd::GetDlgItemInt(SBPW_LEVEL_LOWER_RATIO_EDIT))*0.01;	
	Param.OffsetZ = (int)(CWnd::GetDlgItemInt(SBPW_LEVEL_OFFSET_Z_EDIT));	
	Param.PlaneRatioLSL = (int)(CWnd::GetDlgItemInt(SBPW_LEVEL_PLANE_RATIO_LSL_EDIT));	
	Param.PlaneRatioUSL = (int)(CWnd::GetDlgItemInt(SBPW_LEVEL_PLANE_RATIO_USL_EDIT));	
	Param.RangeRatioMin = (int)(CWnd::GetDlgItemInt(SBPW_LEVEL_RANGE_RATIO_MIN_EDIT));	
	Param.RangeRatioMax = (int)(CWnd::GetDlgItemInt(SBPW_LEVEL_RANGE_RATIO_MAX_EDIT));	
	Param.OverHighFilter = (int)(CWnd::GetDlgItemInt(SBPW_LEVEL_OVER_HIGH_EDIT));	
	Param.OverLowFilter = (int)(CWnd::GetDlgItemInt(SBPW_LEVEL_OVER_LOW_EDIT));

	m_LocalBasePlaneID = (int)(CWnd::GetDlgItemInt(SBPW_LEVEL_LOCAL_PLANE_ID_EDIT));	
	//Param.NormalZ = (int)(CWnd::GetDlgItemInt(SBPW_LEVEL_PLANE_READING_EDIT));	

	if ( CWnd::IsDlgButtonChecked(SBPW_PLANE_CLIP_ROTATED_OUTER_CHK) == TRUE )
	{	Param.RotatedClip = true; }
	else
	{	Param.RotatedClip = false; }	

	if ( Param.PlaneRatioLSL < 0 ) { Param.PlaneRatioLSL = 0; }
	if ( Param.PlaneRatioLSL > 100 ) { Param.PlaneRatioLSL = 100; }
	if ( Param.PlaneRatioUSL < 0 ) { Param.PlaneRatioUSL = 0; }
	if ( Param.PlaneRatioUSL > 100 ) { Param.PlaneRatioUSL = 100; }
	if ( Param.RangeRatioMin < 0 ) { Param.RangeRatioMin = 0; }
	if ( Param.RangeRatioMin > 100 ) { Param.RangeRatioMin = 100; }
	if ( Param.RangeRatioMax < 0 ) { Param.RangeRatioMax = 0; }
	if ( Param.RangeRatioMax > 100 ) { Param.RangeRatioMax = 100; }	


	Param.FilterMode = JetAPI::GetComboxCurSelData(m_FilterModeCombox);	
	Param.FilterPitch = CWnd::GetDlgItemInt(SBPW_LEVEL_FILTER_PITCH_EDIT);	
	Param.FilterKerSize = CWnd::GetDlgItemInt(SBPW_LEVEL_FILTER_KER_EDIT);	
	Param.FilterUseSize = CWnd::GetDlgItemInt(SBPW_LEVEL_FILTER_USE_EDIT);	
	//Param.FilterIterCount = CWnd::GetDlgItemInt(SBPW_LEVEL_FILTER_PITCH_EDIT);	

	m_BaseColorEnabled = true;
	if ( CWnd::IsDlgButtonChecked(SBPW_PLANE_COLOR_RAD_OFF) == TRUE )
	{	m_BaseColorIndex = -1;	}
	else if ( CWnd::IsDlgButtonChecked(SBPW_PLANE_COLOR_RAD_1) == TRUE )
	{	m_BaseColorIndex = 0; }
	else if ( CWnd::IsDlgButtonChecked(SBPW_PLANE_COLOR_RAD_2) == TRUE )
	{	m_BaseColorIndex = 1; }	
	else if ( CWnd::IsDlgButtonChecked(SBPW_PLANE_COLOR_RAD_3) == TRUE )
	{	m_BaseColorIndex = 2; }	
	else if ( CWnd::IsDlgButtonChecked(SBPW_PLANE_COLOR_RAD_4) == TRUE )
	{	m_BaseColorIndex = 3; }	
	else if ( CWnd::IsDlgButtonChecked(SBPW_PLANE_COLOR_RAD_5) == TRUE )
	{	m_BaseColorIndex = 4; }	
	else if ( CWnd::IsDlgButtonChecked(SBPW_PLANE_COLOR_RAD_6) == TRUE )
	{	m_BaseColorIndex = 5; }	
	else if ( CWnd::IsDlgButtonChecked(SBPW_PLANE_COLOR_RAD_7) == TRUE )
	{	m_BaseColorIndex = 6; }	
	else if ( CWnd::IsDlgButtonChecked(SBPW_PLANE_COLOR_RAD_8) == TRUE )
	{	m_BaseColorIndex = 7; }	
	else
	{	m_BaseColorIndex = -1; }	
	if ( -1 == m_BaseColorIndex)
	{	m_BaseColorEnabled = false; }

	Param.FilterMode2D = JetAPI::GetComboxCurSelData(m_FilterMode2DCombox);		
	Param.FilterKerSize2D = CWnd::GetDlgItemInt(SBPW_LEVEL_FILTER_KER_SIZE_2D_EDIT);		

	CWnd::GetDlgItemText(SBPW_INFO_TEXT_EDIT, Param.BasePlaneInfoText);
	return true;
}
//-------------------------------------------------------------------------------------//
void CSpaceBaseParamWnd::SendToSystemBasePlane(int index)
{
	UpdateUIToParam();
	m_DefaultIndex = index;
	m_BasePlaneParam.BasePlaneIndex = index;
	AOIDataCollect.SetSystemBasePlaneParam(index, m_BasePlaneParam);
	BuildBasePlaneListWnd();
}
//-------------------------------------------------------------------------------------//
void CSpaceBaseParamWnd::OnUseSideAllBtn() 
{
	// TODO: Add your control notification handler code here	
	bool bSetAll=false;
	std::vector<UINT> CtrlIDList;
	CtrlIDList.push_back(SBPW_USE_SIDE_CHK_T);
	CtrlIDList.push_back(SBPW_USE_SIDE_CHK_L);
	CtrlIDList.push_back(SBPW_USE_SIDE_CHK_B);
	CtrlIDList.push_back(SBPW_USE_SIDE_CHK_R);
	const size_t Count=CtrlIDList.size();

	bSetAll = false;
	for ( size_t i=0; i<Count; i++ )
	{
		if ( CWnd::IsDlgButtonChecked(CtrlIDList[i]) == FALSE )
		{
			bSetAll = true;
			break;
		}
	}

	for ( size_t i=0; i<Count; i++ )
	{	CWnd::CheckDlgButton(CtrlIDList[i], bSetAll);	}
	return;
}
//-------------------------------------------------------------------------------------//
void CSpaceBaseParamWnd::OnParamSettingChk() 
{
	// TODO: Add your control notification handler code here	
	UINT CtrlID=0;
	BOOL bChk = CWnd::IsDlgButtonChecked(SBPW_PARAM_SETTING_CHK);
	BOOL bEnable = bChk;

	CtrlID = SBPW_LEVEL_CALC_MODE_COMBO;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = SBPW_LEVEL_AUTO_RGN_MODE_COMBO;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = SBPW_LEVEL_SYSTEM_NOISE_EDIT;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = SBPW_LEVEL_MAX_TILT_ANGLE_EDIT;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = SBPW_LEVEL_UPPER_RATIO_EDIT;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = SBPW_LEVEL_LOWER_RATIO_EDIT;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = SBPW_LEVEL_OFFSET_Z_EDIT;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = SBPW_LEVEL_PLANE_RATIO_LSL_EDIT;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = SBPW_LEVEL_PLANE_RATIO_USL_EDIT;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = SBPW_LEVEL_RANGE_RATIO_MIN_EDIT;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = SBPW_LEVEL_RANGE_RATIO_MAX_EDIT;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = SBPW_LEVEL_OVER_HIGH_EDIT;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = SBPW_LEVEL_OVER_LOW_EDIT;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = SBPW_LEVEL_FILTER_MODEL_COMBO;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = SBPW_LEVEL_FILTER_PITCH_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);

	CtrlID = SBPW_LEVEL_FILTER_KER_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);

	CtrlID = SBPW_LEVEL_FILTER_USE_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);

	CtrlID = SBPW_LEVEL_LOCAL_PLANE_ID_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);	

	CtrlID = SBPW_LEVEL_TOWARD_MODE_COMBO;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);	
}
//-------------------------------------------------------------------------------------//
void CSpaceBaseParamWnd::OnColorSettingChk() 
{
	// TODO: Add your control notification handler code here
	UINT CtrlID=0;
	BOOL bChk = CWnd::IsDlgButtonChecked(SBPW_COLOR_SETTING_CHK);
	BOOL bEnable = bChk;

	CtrlID = SBPW_PLANE_COLOR_RAD_1;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = SBPW_PLANE_COLOR_RAD_2;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = SBPW_PLANE_COLOR_RAD_3;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = SBPW_PLANE_COLOR_RAD_4;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = SBPW_PLANE_COLOR_RAD_5;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = SBPW_PLANE_COLOR_RAD_6;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = SBPW_PLANE_COLOR_RAD_7;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = SBPW_PLANE_COLOR_RAD_8;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = SBPW_PLANE_COLOR_RAD_OFF;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = SBPW_LEVEL_FILTER_MODEL_2D_COMBO;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	CtrlID = SBPW_LEVEL_FILTER_KER_SIZE_2D_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);
}
//-------------------------------------------------------------------------------------//
void CSpaceBaseParamWnd::OnClickParamListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CSpaceBaseParamWnd::OnItemchangedParamListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if ( true == m_StopBasePlaneListBeSelected ) { return; }	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) 
	{
		CWnd::SetDlgItemText(SBPW_INFO_TEXT_EDIT, _T(""));		
		return; 
	}
	DWORD Res = 0;	
	Res = pNMListView->uNewState&LVIS_SELECTED;
	if ( Res == 0 ) { return; }	
	Res = pNMListView->uNewState&LVIS_FOCUSED;
	if ( Res == 0 ) { return; }

	const size_t Index = (size_t)(m_BasePlaneListCtrl.GetItemData(nItem));
	const size_t Count = m_BasePlaneParamList.size();
	if ( Index >= Count ) 
	{
		CWnd::SetDlgItemText(SBPW_INFO_TEXT_EDIT, _T(""));		
		return; 
	}
	m_BasePlaneParam = m_BasePlaneParamList[Index];
	UpdateParamToUI();
	UpdateUIEnable();
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CSpaceBaseParamWnd::OnSendToBtn01() 
{
	// TODO: Add your control notification handler code here
	SendToSystemBasePlane(0);
}
//-------------------------------------------------------------------------------------//
void CSpaceBaseParamWnd::OnSendToBtn02() 
{
	// TODO: Add your control notification handler code here
	SendToSystemBasePlane(1);
}
//-------------------------------------------------------------------------------------//
void CSpaceBaseParamWnd::OnSendToBtn03() 
{
	// TODO: Add your control notification handler code here
	SendToSystemBasePlane(2);
}
//-------------------------------------------------------------------------------------//
void CSpaceBaseParamWnd::OnSendToBtn04() 
{
	// TODO: Add your control notification handler code here
	SendToSystemBasePlane(3);
}
//-------------------------------------------------------------------------------------//
void CSpaceBaseParamWnd::OnSendToBtn05() 
{
	// TODO: Add your control notification handler code here
	SendToSystemBasePlane(4);
}
//-------------------------------------------------------------------------------------//
void CSpaceBaseParamWnd::OnSendToBtn06() 
{
	// TODO: Add your control notification handler code here
	SendToSystemBasePlane(5);
}
//-------------------------------------------------------------------------------------//
void CSpaceBaseParamWnd::OnSendToBtn07() 
{
	// TODO: Add your control notification handler code here
	SendToSystemBasePlane(6);
}
//-------------------------------------------------------------------------------------//
void CSpaceBaseParamWnd::OnSendToBtn08() 
{
	// TODO: Add your control notification handler code here
	SendToSystemBasePlane(7);
}
//-------------------------------------------------------------------------------------//
void CSpaceBaseParamWnd::OnUnlinkBtn() 
{
	// TODO: Add your control notification handler code here
	SendToSystemBasePlane(-1);
}
//-------------------------------------------------------------------------------------//
void CSpaceBaseParamWnd::OnSaveParamBtn() 
{
	// TODO: Add your control notification handler code here
	bool IsLockSystemParam = AOIDataCollect.CheckIsLockSystemParameter();	
	if ( true == IsLockSystemParam )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return; 
	}

	CString str;	
	str = _T("Do you want to save space base plane param?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO|MB_DEFBUTTON1) == IDNO )
	{	return; }

	AOIDataCollect.SaveSystemBasePlaneParam();
}
//-------------------------------------------------------------------------------------//
void CSpaceBaseParamWnd::OnLoadParamBtn() 
{
	// TODO: Add your control notification handler code here
	CString str;
	str = _T("Do you want to load space base plane param?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO|MB_DEFBUTTON2) == IDNO )
	{	return; }

	AOIDataCollect.LoadSystemBasePlaneParam();
	BuildBasePlaneListWnd();
}
//-------------------------------------------------------------------------------------//
void CSpaceBaseParamWnd::OnSelchangeLevelCalcModeCombo() 
{
	// TODO: Add your control notification handler code here
	UpdateUIToParam();
	UpdateUIEnable();
}
//-------------------------------------------------------------------------------------//