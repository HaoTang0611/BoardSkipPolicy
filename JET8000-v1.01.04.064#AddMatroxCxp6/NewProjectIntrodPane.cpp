// NewProjectIntrodPane.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "NewProjectIntrodPane.h"
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
// CNewProjectPaneIntroduction dialog
//-------------------------------------------------------------------------------------//
CNewProjectPaneIntroduction::CNewProjectPaneIntroduction(CWnd* pParent /*=NULL*/)
	: CDialog(CNewProjectPaneIntroduction::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewProjectPaneIntroduction)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ProjectPtr = NULL;
	m_IsNewProject = true;
	m_NewProjectMode = NEW_PROJECT_ONLINE;
	m_EnableMultiDistrictMode = false;
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneIntroduction::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewProjectPaneIntroduction)
	DDX_Control(pDX, INTROD_PCB_LANE_COMBO, m_LaneCombox);	
	DDX_Control(pDX, INTROD_PANEL_SIDE_COMBO, m_PanelSideCombox);		
	DDX_Control(pDX, INTROD_DLP_LED_COLOR_COMBO, m_DlpLEDColorCombox);
	DDX_Control(pDX, INTROD_HEIGHT_RATIO_COMBO, m_HeightRatioCombox);
	DDX_Control(pDX, INTROD_FOV_WIDTH_SIZE_MODE_COMBO, m_FieldSizeModeComboxW);
	DDX_Control(pDX, INTROD_FOV_HEIGHT_SIZE_MODE_COMBO, m_FieldSizeModeComboxH);
	DDX_Control(pDX, INTROD_FRAME_COMBO_08, m_FrameCombox08);
	DDX_Control(pDX, INTROD_FRAME_COMBO_07, m_FrameCombox07);
	DDX_Control(pDX, INTROD_FRAME_COMBO_06, m_FrameCombox06);
	DDX_Control(pDX, INTROD_FRAME_COMBO_05, m_FrameCombox05);
	DDX_Control(pDX, INTROD_FRAME_COMBO_04, m_FrameCombox04);
	DDX_Control(pDX, INTROD_FRAME_COMBO_03, m_FrameCombox03);
	DDX_Control(pDX, INTROD_FRAME_COMBO_02, m_FrameCombox02);
	DDX_Control(pDX, INTROD_FRAME_COMBO_01, m_FrameCombox01);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CNewProjectPaneIntroduction, CDialog)
	//{{AFX_MSG_MAP(CNewProjectPaneIntroduction)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(INTROD_FILE_NAME_FORMAT_BTN, OnFileNameFormatBtn)
	ON_BN_CLICKED(INTROD_FILE_FOLDER_BROWSER_BTN, OnFileFolderBrowserBtn)
	ON_BN_CLICKED(INTROD_FRAME_SHOW_CHK, OnFrameShowChk)
	ON_BN_CLICKED(INTROD_PCB_IN_BTN, OnPCBInBtn)
	ON_BN_CLICKED(INTROD_PCB_BACK_BTN, OnPCBBackBtn)
	ON_BN_CLICKED(INTROD_PCB_OUT_BTN, OnPCBOutBtn)
	ON_BN_CLICKED(INTROD_PCB_CLAMP_ON_BTN, OnPCBClampOnBtn)
	ON_BN_CLICKED(INTROD_LANE_WIDTH_GET_BTN, OnLaneWidthGetBtn)
	ON_BN_CLICKED(INTROD_LANE_WIDTH_SET_BTN, OnLaneWidthSetBtn)
	ON_BN_CLICKED(INTROD_PCB_IN_2ND_BTN, OnPCBIn2ndBtn)
	ON_BN_CLICKED(INTROD_PCB_IN_3RD_BTN, OnPCBIn3rdBtn)
	ON_BN_CLICKED(INTROD_LANE_ADJUST_WIDTH_BTN, OnLaneAdjustWidthBtn)	
	ON_BN_CLICKED(INTROD_FRAME_CLOSE_ALL_BTN, OnFrameCloseAllBtn)
	ON_BN_CLICKED(INTROD_FRAME_DEFAULT_BTN, OnFrameDefaultBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CNewProjectPaneIntroduction message handlers
//-------------------------------------------------------------------------------------//
BOOL CNewProjectPaneIntroduction::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	this->SwitchMultiLanguage();	
	CString str;
	CString strDateTime;
	CString strFolder = AOIDataCollect.GetAOIProjectDirectory();
	LANE_ID LaneID = AOIDataCollect.GetActiveLaneID();	
	const bool IsNewProject = GetIsNewProject();
	LANE_WORK_MODE LaneWorkMode_LA = AOIDataCollect.GetLaneWorkMode_LA();
	LANE_WORK_MODE LaneWorkMode_LB = AOIDataCollect.GetLaneWorkMode_LB();
	const double LaneWidth = PlcCtrlPtr->ReadLaneAdjustCurrentPos(LaneID);	
	AOIDataDefine.BuildLaneIDCombox(m_LaneCombox);	
	AOIDataDefine.BuildProjectFieldSizeModeCombox(m_FieldSizeModeComboxW);
	AOIDataDefine.BuildProjectFieldSizeModeCombox(m_FieldSizeModeComboxH);
	AOIDataDefine.BuildPanelSideModeCombox(m_PanelSideCombox);

	InitProjectFrameCombox();
	BuildProjectFrameCombox();
	OnFrameShowChk();	

	JetAPI::GetTime(strDateTime, CTime::GetCurrentTime());
	JetAPI::SetComboxCurSel(m_FieldSizeModeComboxW, FIELD_SIZE_100);		
	JetAPI::SetComboxCurSel(m_FieldSizeModeComboxH, FIELD_SIZE_100);		

	str.Format(_T("%.2f"), LaneWidth);
	CWnd::SetDlgItemText(INTROD_LANE_WIDTH_EDIT, str);
	CWnd::SetDlgItemText(INTROD_MODULE_NAME_EDIT, strDateTime);//_T("Module")
	CWnd::SetDlgItemText(INTROD_PROJECT_VERSION_EDIT, _T(""));		
	JetAPI::SetComboxCurSel(m_PanelSideCombox, PANEL_SIDE_TOP);		
	CWnd::SetDlgItemText(INTROD_PRODUCT_WORK_NUMBER_EDIT, _T("Work_Number"));
	CWnd::SetDlgItemText(INTROD_FILE_NAME_EDIT, _T("FileName"));
	CWnd::SetDlgItemText(INTROD_FILE_FOLDER_EDIT, strFolder);		
	
	if ( false == IsNewProject )
	{
		if ( LANE_WORK_DISABLE != LaneWorkMode_LB )
		{	LaneID = LANE_ID_B;	 }
		if ( LANE_WORK_DISABLE != LaneWorkMode_LA )
		{	LaneID = LANE_ID_A;	 }	
	}
	JetAPI::SetComboxCurSel(m_LaneCombox, LaneID);

#ifdef OFFLINE_VERSION
	BOOL bEnable = FALSE;
	JetAPI::EnableCtrlWnd(this, INTROD_LANE_WIDTH_GET_BTN, bEnable);
	JetAPI::EnableCtrlWnd(this, INTROD_LANE_WIDTH_SET_BTN, bEnable);	
	JetAPI::EnableCtrlWnd(this, INTROD_HEIGHT_RATIO_COMBO, bEnable);	
	JetAPI::EnableCtrlWnd(this, INTROD_DLP_LED_COLOR_COMBO, bEnable);	
	JetAPI::EnableCtrlWnd(this, INTROD_FRAME_SHOW_CHK, bEnable);
	JetAPI::EnableCtrlWnd(this, INTROD_PCB_LANE_COMBO, bEnable);
	JetAPI::EnableCtrlWnd(this, INTROD_PCB_IN_BTN, bEnable);
	JetAPI::EnableCtrlWnd(this, INTROD_PCB_BACK_BTN, bEnable);
	JetAPI::EnableCtrlWnd(this, INTROD_PCB_OUT_BTN, bEnable);
	JetAPI::EnableCtrlWnd(this, INTROD_PCB_CLAMP_ON_BTN, bEnable);
	JetAPI::EnableCtrlWnd(this, INTROD_PCB_IN_2ND_BTN, bEnable);
	JetAPI::EnableCtrlWnd(this, INTROD_PCB_IN_3RD_BTN, bEnable);	
	JetAPI::EnableCtrlWnd(this, INTROD_LANE_ADJUST_WIDTH_BTN, bEnable);	
#else
	const bool LaneAdjustDisable = PlcCtrlPtr->GetLaneAdjustDisableBtn(LaneID);
	if ( true == LaneAdjustDisable )
	{	
		JetAPI::EnableCtrlWnd(this, INTROD_LANE_WIDTH_GET_BTN, FALSE); 
		JetAPI::EnableCtrlWnd(this, INTROD_LANE_WIDTH_SET_BTN, FALSE); 		
	}
#endif//OFFLINE_VERSION
	if ( false == AOIDataCollect.GetUIEnablePCBOutButton() )
	{	JetAPI::EnableCtrlWnd(this, INTROD_PCB_OUT_BTN, FALSE);	}

	MACHINE_CAMERA_SIDE CameraSide = AOIDataCollect.GetMachineCameraSide();
	switch ( CameraSide )
	{
	case MACHINE_CAMERA_TOP:	JetAPI::SetComboxCurSel(m_PanelSideCombox, PANEL_SIDE_TOP);	break;
	case MACHINE_CAMERA_BOT:	JetAPI::SetComboxCurSel(m_PanelSideCombox, PANEL_SIDE_BOTTOM);	break;
	}

	if ( true == IsNewProject )
	{	OnFileNameFormatBtn();	}
	else
	{
		CAOIProject *ProjectPtr=m_ProjectPtr;
		if ( NULL != ProjectPtr )
		{
			CString strModule=ProjectPtr->GetProjectModuleName();
			CString strVersion=ProjectPtr->GetProjectVersion();
			CString strWorkNumber=ProjectPtr->GetProjectWorkNumber();
			CString strFileMainName=ProjectPtr->GetProjectFileMainName();
			CWnd::SetDlgItemText(INTROD_MODULE_NAME_EDIT, strModule);
			CWnd::SetDlgItemText(INTROD_PROJECT_VERSION_EDIT, strVersion);		
			CWnd::SetDlgItemText(INTROD_PRODUCT_WORK_NUMBER_EDIT, strWorkNumber);
			CWnd::SetDlgItemText(INTROD_FILE_NAME_EDIT, strFileMainName);			
		}
		JetAPI::EnableCtrlWnd(this, INTROD_PCB_LANE_COMBO, FALSE);	
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneIntroduction::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneIntroduction::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneIntroduction::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if ( TRUE == bShow )
	{
		HWND hWnd = CWnd::GetSafeHwnd();
		AOIDataCollect.PostParentWndMessage(hWnd, MSG_EDIT_MAIN_VIEW_WND, WPARAM_SHOW_PROJECT_MAP_WND, FALSE);
	}
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneIntroduction::OnOK() 
{
	// TODO: Add extra validation here
	return;
	CDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneIntroduction::OnCancel() 
{
	// TODO: Add extra cleanup here
	return;
	CDialog::OnCancel();
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneIntroduction::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_NEW_PROJECT_PANE_INTROD");
	//---------------------------------------------------------------------------------//
	WndID = IDD_NEW_PROJECT_PANE_INTROD;
	WndKey = _T("IDD_NEW_PROJECT_PANE_INTROD");
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
	WndID = INTROD_PROJECT_PARAM_GROUP;
	WndKey = _T("INTROD_PROJECT_PARAM_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = INTROD_HEIGHT_RATIO_LABEL;
	WndKey = _T("INTROD_HEIGHT_RATIO_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = INTROD_DLP_LED_COLOR_LABEL;
	WndKey = _T("INTROD_DLP_LED_COLOR_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = INTROD_FOV_WIDTH_SIZE_MODE_LABEL;
	WndKey = _T("INTROD_FOV_WIDTH_SIZE_MODE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = INTROD_FOV_HEIGHT_SIZE_MODE_LABEL;
	WndKey = _T("INTROD_FOV_HEIGHT_SIZE_MODE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = INTROD_LANE_WIDTH_LABEL;
	WndKey = _T("INTROD_LANE_WIDTH_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = INTROD_LANE_WIDTH_SET_BTN;
	WndKey = _T("INTROD_LANE_WIDTH_SET_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = INTROD_LANE_WIDTH_GET_BTN;
	WndKey = _T("INTROD_LANE_WIDTH_GET_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = INTROD_MODULE_NAME_LABEL;
	WndKey = _T("INTROD_MODULE_NAME_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = INTROD_PROJECT_VERSION_LABEL;
	WndKey = _T("INTROD_PROJECT_VERSION_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = INTROD_PRODUCT_SIDE_LABEL;
	WndKey = _T("INTROD_PRODUCT_SIDE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = INTROD_PRODUCT_WORK_NUMBER_LABEL;
	WndKey = _T("INTROD_PRODUCT_WORK_NUMBER_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = INTROD_FILE_NAME_LABEL;
	WndKey = _T("INTROD_FILE_NAME_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = INTROD_FILE_NAME_FORMAT_BTN;
	WndKey = _T("INTROD_FILE_NAME_FORMAT_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = INTROD_FILE_FOLDER_LABEL;
	WndKey = _T("INTROD_FILE_FOLDER_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = INTROD_FILE_FOLDER_BROWSER_BTN;
	WndKey = _T("INTROD_FILE_FOLDER_BROWSER_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	//---------------------------------------------------------------------------------//
	WndID = INTROD_FRAME_CONFIG_GROUP;
	WndKey = _T("INTROD_FRAME_CONFIG_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = INTROD_FRAME_SHOW_CHK;
	WndKey = _T("INTROD_FRAME_SHOW_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = INTROD_FRAME_LABEL_01;
	WndKey = _T("INTROD_FRAME_LABEL_01");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = INTROD_FRAME_LABEL_02;
	WndKey = _T("INTROD_FRAME_LABEL_02");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = INTROD_FRAME_LABEL_03;
	WndKey = _T("INTROD_FRAME_LABEL_03");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = INTROD_FRAME_LABEL_04;
	WndKey = _T("INTROD_FRAME_LABEL_04");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = INTROD_FRAME_LABEL_05;
	WndKey = _T("INTROD_FRAME_LABEL_05");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = INTROD_FRAME_LABEL_06;
	WndKey = _T("INTROD_FRAME_LABEL_06");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = INTROD_FRAME_LABEL_07;
	WndKey = _T("INTROD_FRAME_LABEL_07");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = INTROD_FRAME_LABEL_08;
	WndKey = _T("INTROD_FRAME_LABEL_08");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = INTROD_FRAME_DEFAULT_BTN;
	WndKey = _T("INTROD_FRAME_DEFAULT_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = INTROD_FRAME_CLOSE_ALL_BTN;
	WndKey = _T("INTROD_FRAME_CLOSE_ALL_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//
	WndID = INTROD_PCB_CTRL_GROUP;
	WndKey = _T("INTROD_PCB_CTRL_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	
	WndID = INTROD_PCB_LANE_LABEL;
	WndKey = _T("INTROD_PCB_LANE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = INTROD_PCB_IN_BTN;
	WndKey = _T("INTROD_PCB_IN_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = INTROD_PCB_BACK_BTN;
	WndKey = _T("INTROD_PCB_BACK_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = INTROD_PCB_OUT_BTN;
	WndKey = _T("INTROD_PCB_OUT_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = INTROD_PCB_CLAMP_ON_BTN;
	WndKey = _T("INTROD_PCB_CLAMP_ON_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = INTROD_PCB_IN_2ND_BTN;
	WndKey = _T("INTROD_PCB_IN_2ND_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = INTROD_PCB_IN_3RD_BTN;
	WndKey = _T("INTROD_PCB_IN_3RD_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = INTROD_LANE_ADJUST_WIDTH_BTN;
	WndKey = _T("INTROD_LANE_ADJUST_WIDTH_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//
}
//-------------------------------------------------------------------------------------//
CString CNewProjectPaneIntroduction::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_NEW_PROJECT_PANE_INTROD");		
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
LRESULT CNewProjectPaneIntroduction::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneIntroduction::SetProjectPtr(CAOIProject *ProjectPtr)
{
	this->m_ProjectPtr = ProjectPtr;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneIntroduction::GetIsNewProject() const
{
	return m_IsNewProject;
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneIntroduction::SetIsNewProject(bool Val)
{
	m_IsNewProject = Val;
}
//-------------------------------------------------------------------------------------//
NEW_PROJECT_MODE CNewProjectPaneIntroduction::GetNewProjectMode() const
{
	return m_NewProjectMode;
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneIntroduction::SetNewProjectMode(NEW_PROJECT_MODE Mode)
{
	m_NewProjectMode = Mode;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneIntroduction::GetEnableMultiDistrictMode() const
{ 
	return m_EnableMultiDistrictMode; 
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneIntroduction::SetEnableMultiDistrictMode(bool Mode)
{ 
	m_EnableMultiDistrictMode = Mode; 
}	
//-------------------------------------------------------------------------------------//
void CNewProjectPaneIntroduction::OnFileNameFormatBtn() 
{
	// TODO: Add your control notification handler code here
	CString str;
	CString Side;
	CString Module;
	CString Version;	
	CString FileName;
	PANEL_SIDE_MODE SideMode;

	CWnd::GetDlgItemText(INTROD_MODULE_NAME_EDIT, Module);
	CWnd::GetDlgItemText(INTROD_PROJECT_VERSION_EDIT, Version);
	
	SideMode = (PANEL_SIDE_MODE)JetAPI::GetComboxCurSelData(m_PanelSideCombox);
	switch ( SideMode )
	{
	case PANEL_SIDE_TOP:	Side=_T("T");	break;
	case PANEL_SIDE_BOTTOM:	Side=_T("B");	break;
	case PANEL_SIDE_HYBRID:	Side=_T("");	break;
	}	

	FileName = Module;
	if ( Version.GetLength() != 0 ) 
	{
		str = FileName;
		FileName.Format(_T("%s_%s"), str, Version);
	}
	if ( Side.GetLength() != 0 ) 
	{
		str = FileName;
		FileName.Format(_T("%s_%s"), str, Side);
	}
	/*
	if ( Side.GetLength() == 0 ) 
	{	FileName.Format(_T("%s_%s"), Module, Version);	}
	else
	{	FileName.Format(_T("%s_%s_%s"), Module, Version, Side); }
	*/
	CWnd::SetDlgItemText(INTROD_FILE_NAME_EDIT, FileName);
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneIntroduction::OnFileFolderBrowserBtn() 
{
	// TODO: Add your control notification handler code here
	CString Folder;
	CWnd::GetDlgItemText(INTROD_FILE_FOLDER_EDIT, Folder);
	if ( JetAPI::OpenFolderDialog(this, Folder) == false ) { return ; }	
	CWnd::SetDlgItemText(INTROD_FILE_FOLDER_EDIT, Folder);
	::SetCurrentDirectory(Folder);	
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneIntroduction::OnFrameShowChk() 
{
	// TODO: Add your control notification handler code here
	CWnd *pWnd = NULL;
	UINT  CtrlID = 0;
	BOOL  bShow = TRUE;
	BOOL  bChecked = CWnd::IsDlgButtonChecked(INTROD_FRAME_SHOW_CHK);
	if ( TRUE == bChecked ) { bShow = TRUE; }
	else { bShow = FALSE; }

	CtrlID = INTROD_HEIGHT_RATIO_LABEL;
	JetAPI::ShowCtrlWnd(this, CtrlID, bShow);
	CtrlID = INTROD_HEIGHT_RATIO_COMBO;
	JetAPI::ShowCtrlWnd(this, CtrlID, bShow);
	CtrlID = INTROD_DLP_LED_COLOR_LABEL;
	JetAPI::ShowCtrlWnd(this, CtrlID, bShow);
	CtrlID = INTROD_DLP_LED_COLOR_COMBO;
	JetAPI::ShowCtrlWnd(this, CtrlID, bShow);

	CtrlID = INTROD_FOV_WIDTH_SIZE_MODE_LABEL;
	JetAPI::ShowCtrlWnd(this, CtrlID, bShow);
	CtrlID = INTROD_FOV_WIDTH_SIZE_MODE_COMBO;
	JetAPI::ShowCtrlWnd(this, CtrlID, bShow);
	CtrlID = INTROD_FOV_HEIGHT_SIZE_MODE_LABEL;
	JetAPI::ShowCtrlWnd(this, CtrlID, bShow);
	CtrlID = INTROD_FOV_HEIGHT_SIZE_MODE_COMBO;
	JetAPI::ShowCtrlWnd(this, CtrlID, bShow);

	CtrlID = INTROD_FRAME_CONFIG_GROUP;
	JetAPI::ShowCtrlWnd(this, CtrlID, bShow);

	CtrlID = INTROD_FRAME_LABEL_01;
	JetAPI::ShowCtrlWnd(this, CtrlID, bShow);
	CtrlID = INTROD_FRAME_COMBO_01;
	JetAPI::ShowCtrlWnd(this, CtrlID, bShow);
	CtrlID = INTROD_FRAME_LABEL_02;
	JetAPI::ShowCtrlWnd(this, CtrlID, bShow);
	CtrlID = INTROD_FRAME_COMBO_02;
	JetAPI::ShowCtrlWnd(this, CtrlID, bShow);
	CtrlID = INTROD_FRAME_LABEL_03;
	JetAPI::ShowCtrlWnd(this, CtrlID, bShow);
	CtrlID = INTROD_FRAME_COMBO_03;
	JetAPI::ShowCtrlWnd(this, CtrlID, bShow);
	CtrlID = INTROD_FRAME_LABEL_04;
	JetAPI::ShowCtrlWnd(this, CtrlID, bShow);
	CtrlID = INTROD_FRAME_COMBO_04;
	JetAPI::ShowCtrlWnd(this, CtrlID, bShow);
	CtrlID = INTROD_FRAME_LABEL_05;
	JetAPI::ShowCtrlWnd(this, CtrlID, bShow);
	CtrlID = INTROD_FRAME_COMBO_05;
	JetAPI::ShowCtrlWnd(this, CtrlID, bShow);
	CtrlID = INTROD_FRAME_LABEL_06;
	JetAPI::ShowCtrlWnd(this, CtrlID, bShow);
	CtrlID = INTROD_FRAME_COMBO_06;
	JetAPI::ShowCtrlWnd(this, CtrlID, bShow);
	CtrlID = INTROD_FRAME_LABEL_07;
	JetAPI::ShowCtrlWnd(this, CtrlID, bShow);
	CtrlID = INTROD_FRAME_COMBO_07;
	JetAPI::ShowCtrlWnd(this, CtrlID, bShow);
	CtrlID = INTROD_FRAME_LABEL_08;
	JetAPI::ShowCtrlWnd(this, CtrlID, bShow);
	CtrlID = INTROD_FRAME_COMBO_08;
	JetAPI::ShowCtrlWnd(this, CtrlID, bShow);

	CtrlID = INTROD_FRAME_DEFAULT_BTN;
	JetAPI::ShowCtrlWnd(this, CtrlID, bShow);
	CtrlID = INTROD_FRAME_CLOSE_ALL_BTN;
	JetAPI::ShowCtrlWnd(this, CtrlID, bShow);
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneIntroduction::InitProjectFrameCombox()
{	
	AOIDataDefine.BuildSystemFrameParamCombox(m_FrameCombox01, true, true);	
	AOIDataDefine.BuildSystemFrameParamCombox(m_FrameCombox02, true, true);
	AOIDataDefine.BuildSystemFrameParamCombox(m_FrameCombox03, true, true);
	AOIDataDefine.BuildSystemFrameParamCombox(m_FrameCombox04, true, true);
	AOIDataDefine.BuildSystemFrameParamCombox(m_FrameCombox05, true, true);
	AOIDataDefine.BuildSystemFrameParamCombox(m_FrameCombox06, true, true);
	AOIDataDefine.BuildSystemFrameParamCombox(m_FrameCombox07, true, true);
	AOIDataDefine.BuildSystemFrameParamCombox(m_FrameCombox08, true, true);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneIntroduction::BuildProjectFrameCombox()
{
	CAOIProject *ProjectPtr = m_ProjectPtr;
	if ( NULL == ProjectPtr ) { return false; }

	size_t i = 0;
	unsigned int FrameUniqueID=FRAME_UNIQUE_ID_NULL;
	std::vector<unsigned int> FrameUniqueIDList;
	ProjectPtr->CloneProjectFrameUniqueIDList(FrameUniqueIDList);
	const size_t FrameUniqueIDCount = FrameUniqueIDList.size();

	CloaseAllProjectFrameCombox();
	for ( i=0; i<FrameUniqueIDCount; i++ )
	{
		if ( i >= FRAME_MAX_COUNT ) { continue; }
		FrameUniqueID = FrameUniqueIDList[i];
		switch ( i )
		{
		case 0:	JetAPI::SetComboxCurSel(m_FrameCombox01, FrameUniqueID);	break;
		case 1:	JetAPI::SetComboxCurSel(m_FrameCombox02, FrameUniqueID);	break;
		case 2:	JetAPI::SetComboxCurSel(m_FrameCombox03, FrameUniqueID);	break;
		case 3:	JetAPI::SetComboxCurSel(m_FrameCombox04, FrameUniqueID);	break;
		case 4:	JetAPI::SetComboxCurSel(m_FrameCombox05, FrameUniqueID);	break;
		case 5:	JetAPI::SetComboxCurSel(m_FrameCombox06, FrameUniqueID);	break;
		case 6:	JetAPI::SetComboxCurSel(m_FrameCombox07, FrameUniqueID);	break;
		case 7:	JetAPI::SetComboxCurSel(m_FrameCombox08, FrameUniqueID);	break;
		}
	}	
	
	AOIDataDefine.BuidlProjectSpaceToGrayRatioCombox(m_HeightRatioCombox);
	const int SpaceRatioMode = ProjectPtr->GetProjectSpaceToGrayRatioMode();
	JetAPI::SetComboxCurSel(m_HeightRatioCombox, SpaceRatioMode);

	AOIDataDefine.BuidlProjectDlpLedColorCombox(m_DlpLEDColorCombox);
	const int DlpLedColor = ProjectPtr->GetProjectDlpLedColorMode();
	JetAPI::SetComboxCurSel(m_DlpLEDColorCombox, DlpLedColor);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneIntroduction::CloaseAllProjectFrameCombox()
{
	unsigned int FrameUniqueID=FRAME_UNIQUE_ID_NULL;
	JetAPI::SetComboxCurSel(m_FrameCombox01, FrameUniqueID);
	JetAPI::SetComboxCurSel(m_FrameCombox02, FrameUniqueID);
	JetAPI::SetComboxCurSel(m_FrameCombox03, FrameUniqueID);
	JetAPI::SetComboxCurSel(m_FrameCombox04, FrameUniqueID);
	JetAPI::SetComboxCurSel(m_FrameCombox05, FrameUniqueID);
	JetAPI::SetComboxCurSel(m_FrameCombox06, FrameUniqueID);
	JetAPI::SetComboxCurSel(m_FrameCombox07, FrameUniqueID);
	JetAPI::SetComboxCurSel(m_FrameCombox08, FrameUniqueID);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneIntroduction::BuildFrameUniqueIDList(std::vector<unsigned int> &FrameUniqueIDList)
{
	bool b3DMode = false;
	FrameUniqueIDList.clear();

	b3DMode = false;
	if ( AddFrameUniqueIDList(m_FrameCombox01, FrameUniqueIDList, b3DMode) == false ) { return false; }
	if ( AddFrameUniqueIDList(m_FrameCombox02, FrameUniqueIDList, b3DMode) == false ) { return false; }
	if ( AddFrameUniqueIDList(m_FrameCombox03, FrameUniqueIDList, b3DMode) == false ) { return false; }
	if ( AddFrameUniqueIDList(m_FrameCombox04, FrameUniqueIDList, b3DMode) == false ) { return false; }
	if ( AddFrameUniqueIDList(m_FrameCombox05, FrameUniqueIDList, b3DMode) == false ) { return false; }
	if ( AddFrameUniqueIDList(m_FrameCombox06, FrameUniqueIDList, b3DMode) == false ) { return false; }
	if ( AddFrameUniqueIDList(m_FrameCombox07, FrameUniqueIDList, b3DMode) == false ) { return false; }
	if ( AddFrameUniqueIDList(m_FrameCombox08, FrameUniqueIDList, b3DMode) == false ) { return false; }

	b3DMode = true;
	if ( AddFrameUniqueIDList(m_FrameCombox01, FrameUniqueIDList, b3DMode) == false ) { return false; }
	if ( AddFrameUniqueIDList(m_FrameCombox02, FrameUniqueIDList, b3DMode) == false ) { return false; }
	if ( AddFrameUniqueIDList(m_FrameCombox03, FrameUniqueIDList, b3DMode) == false ) { return false; }
	if ( AddFrameUniqueIDList(m_FrameCombox04, FrameUniqueIDList, b3DMode) == false ) { return false; }
	if ( AddFrameUniqueIDList(m_FrameCombox05, FrameUniqueIDList, b3DMode) == false ) { return false; }
	if ( AddFrameUniqueIDList(m_FrameCombox06, FrameUniqueIDList, b3DMode) == false ) { return false; }
	if ( AddFrameUniqueIDList(m_FrameCombox07, FrameUniqueIDList, b3DMode) == false ) { return false; }
	if ( AddFrameUniqueIDList(m_FrameCombox08, FrameUniqueIDList, b3DMode) == false ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneIntroduction::AddFrameUniqueIDList(CComboBox &Combox, std::vector<unsigned int> &FrameUniqueIDList, bool b3DMode)
{
	if ( Combox.GetSafeHwnd() == NULL ) { return false; }

	unsigned int FrameUniqueID=FRAME_UNIQUE_ID_NULL;
	FrameUniqueID = (unsigned int)JetAPI::GetComboxCurSelData(Combox);
	if ( FRAME_UNIQUE_ID_NULL == FrameUniqueID )
	{	return true; }

	if ( true == b3DMode )
	{
		if ( FRAME_UNIQUE_ID_DLP != FrameUniqueID ) { return true; }		
	}
	else
	{		
		if ( FRAME_UNIQUE_ID_DLP == FrameUniqueID ) { return true; }
	}
	FrameUniqueIDList.push_back(FrameUniqueID);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneIntroduction::ExecNextPane()
{	
	CAOIProject *ProjectPtr = m_ProjectPtr;
	if ( NULL == ProjectPtr ) { return false; }
	
	size_t  i=0;
	double  fLaneWidth=0;
	CString str;
	CString str2;	
	CString Module;
	CString Version;
	CString FileName;
	CString FileFolder;
	CString strLaneWidth;
	CString WorkOrder;
	CString WorkNumber;	
	CString FullFileName;
	CString ExtName=_T("PRG");	
	const bool IsNewProject = GetIsNewProject();
	CWnd::GetDlgItemText(INTROD_LANE_WIDTH_EDIT, strLaneWidth);
	CWnd::GetDlgItemText(INTROD_MODULE_NAME_EDIT, Module);
	CWnd::GetDlgItemText(INTROD_PROJECT_VERSION_EDIT, Version);		
	CWnd::GetDlgItemText(INTROD_PRODUCT_WORK_NUMBER_EDIT, WorkNumber);
	CWnd::GetDlgItemText(INTROD_FILE_NAME_EDIT, FileName);
	CWnd::GetDlgItemText(INTROD_FILE_FOLDER_EDIT, FileFolder);
	LANE_ID LaneID = (LANE_ID)(JetAPI::GetComboxCurSelData(m_LaneCombox));
	PANEL_SIDE_MODE PanelSideMode = (PANEL_SIDE_MODE)(JetAPI::GetComboxCurSelData(m_PanelSideCombox));
	FIELD_SIZE_MODE FieldSizeModeW = (FIELD_SIZE_MODE)(JetAPI::GetComboxCurSelData(m_FieldSizeModeComboxW));		
	FIELD_SIZE_MODE FieldSizeModeH = (FIELD_SIZE_MODE)(JetAPI::GetComboxCurSelData(m_FieldSizeModeComboxH));

	FullFileName.Format(_T("%s\\%s.%s"), FileFolder, FileName, ExtName);
	if ( AOIDataCollect.CheckProjectFilenameValided(FullFileName) == false )
	{
		str = AOIDataCollect.GetErrorString();
		JetAPI::ShowMessageBox(str);
		return false;
	}
	if ( AOIDataCollect.CheckProjectParamNeedToVerifyJsonString(PROJECT_PARAM_WORDK_NUMBER) == true )
	{
		if ( AOIDataCollect.VerifyJsonString(WorkNumber) == false )
		{
			str = AOIDataCollect.GetErrorString();
			JetAPI::ShowMessageBox(str);
			return false;
		}
	}

	std::vector<unsigned int> FrameUniqueIDList;
	unsigned int FrameUniqueID=FRAME_UNIQUE_ID_NULL;
	BuildFrameUniqueIDList(FrameUniqueIDList);	
	const size_t FrameUniqueIDCount = FrameUniqueIDList.size();
	if ( 0 == FrameUniqueIDCount )
	{
		str.Format(_T("Error, there is no any one frame image in the project!"));
		JetAPI::ShowMessageBox(str);
		return false;
	}
	if ( ProjectPtr->BuildProjectImageConfig(FrameUniqueIDList) == false )
	{
		str = ProjectPtr->GetErrorString();
		JetAPI::ShowMessageBox(str);
		return false;
	}	

	const int DlpLedColor = (int)(JetAPI::GetComboxCurSelData(m_DlpLEDColorCombox));
	const int SpaceRatioMode = (int)(JetAPI::GetComboxCurSelData(m_HeightRatioCombox));

	ProjectPtr->SetProjectDlpLedColorMode(DlpLedColor);
	ProjectPtr->SetProjectSpaceToGrayRatioMode(SpaceRatioMode);	
	AOIDataCollect.SetSystemDlpLedColor(DlpLedColor);

	if ( JetAPI::IsFolderExist(FileFolder) == false ) 
	{		
		str = _T("Error, the Folder doest not exit!");
		str = LoadMultiLanguageString(str, str);
		str2.Format(_T("%s\n%s"), str, FileFolder);
		JetAPI::ShowMessageBox(str2);
		return false;
	}
	if ( true==IsNewProject && JetAPI::IsFileExist(FullFileName)==true )
	{
		str = _T("Warning, the file exist, do you want to override it?");
		str = LoadMultiLanguageString(str, str);
		str2.Format(_T("%s\n%s"), str, FullFileName);
		if ( JetAPI::ShowMessageBox(str2, MB_YESNO) == IDNO ) 
		{	return false; }
	}			
	
	CString DateTime;
	CString SysTempFolder;
	CString ProjectFolder;
	CString FiducialFolder;
	CString ProjectMapName;
	CString LibraryFolder;
	CString ProjectMainName;
	CString PartLibraryFolder;	
	CString ProjectTmpFileName;
	CString SysTempProjectFolder;
	CString ProgramOfflinelFolder;
	CString InspectionOfflineFolder;
	CString ProgramOfflinelFolderSrc=ProjectPtr->GetProjectProgramOfflineFolder();
	CString InspectionOfflineFolderSrc=ProjectPtr->GetProjectInspectionOfflineFolder();
	double FocusOffsetZ=AOIDataCollect.CalcProjectFocusOffset(LaneID);
	if ( false == IsNewProject )
	{	FocusOffsetZ = ProjectPtr->GetProjectFocusPosOffset();	}
	
	JetAPI::GetTime(DateTime, CTime::GetCurrentTime());
	SysTempFolder = AOIDataCollect.GetAOITempDirectory();
	JetAPI::ExtractMainFileNameNoPath(FullFileName, ProjectMainName);		
	ProjectTmpFileName = AOIDataDefine.GetProjectTempFilename(FullFileName);
	if ( false == IsNewProject )
	{
		bool bTheSameFilename=false;
		const int nDateTime=14;//YYYYMMDDhhmmss
		CString TempMainNameSrc;
		CString TempFilenameSrc = ProjectPtr->GetProjectFileName();
		CString FileShowNameSrc = ProjectPtr->GetProjectShowName();
		JetAPI::ExtractMainFileNameNoPath(TempFilenameSrc, TempMainNameSrc);
		if ( FullFileName.CompareNoCase(FileShowNameSrc)==0 && TempMainNameSrc.GetLength()>nDateTime )
		{
			CString sDateTime=TempMainNameSrc.Right(nDateTime);
			if ( sDateTime.GetLength() == nDateTime )
			{	
				bTheSameFilename=true;
				DateTime = sDateTime;
				ProjectTmpFileName = TempFilenameSrc;
			}			
		}		
	}
	SysTempProjectFolder.Format(_T("%s\\%s_%s"), SysTempFolder, ProjectMainName, DateTime);
	InspectionOfflineFolder = AOIDataDefine.GetProjectOfflineFolderName(SysTempProjectFolder);		

	ProjectFolder.Format(_T("%s\\%s"), FileFolder, FileName);
	JetAPI::ExtractMainFileName(ProjectTmpFileName, ProjectFolder);
	::CreateDirectory(ProjectFolder, NULL);	::Sleep(0);
	::CreateDirectory(SysTempProjectFolder, NULL);	::Sleep(0);
	::CreateDirectory(InspectionOfflineFolder, NULL);	::Sleep(0);	

	FiducialFolder = AOIDataDefine.GetProjectFdFolderName(ProjectFolder);
	LibraryFolder  = AOIDataDefine.GetProjectLibraryFolderName(ProjectFolder);
	PartLibraryFolder = AOIDataDefine.GetProjectPartLibraryFolderName(ProjectFolder);
	ProjectMapName = AOIDataDefine.GetProjectMapFileName(ProjectFolder);
	ProgramOfflinelFolder = AOIDataDefine.GetProjectOfflineFolderName(ProjectFolder);

	fLaneWidth = JetAPI::StrToDbl(strLaneWidth);
	::CreateDirectory(FileFolder, NULL);
	::CreateDirectory(FiducialFolder, NULL);
	::CreateDirectory(LibraryFolder, NULL);
	::CreateDirectory(PartLibraryFolder, NULL);	
	::CreateDirectory(ProgramOfflinelFolder, NULL);	
	ProjectPtr->SetProjectFileName(ProjectTmpFileName);	
	ProjectPtr->SetProjectShowName(FullFileName);

	AOIDataCollect.SetActiveLaneID(LaneID);		
	ProjectPtr->SetProjectActLaneID(LaneID);	
	ProjectPtr->SetProjectFocusPosOffset(FocusOffsetZ);
	ProjectPtr->SetProjectFieldSizeModeW(FieldSizeModeW);
	ProjectPtr->SetProjectFieldSizeModeH(FieldSizeModeH);
	ProjectPtr->SetProjectLaneWidth(fLaneWidth);
	ProjectPtr->SetProjectModuleName(Module);
	ProjectPtr->SetProjectVersion(Version);	
	ProjectPtr->SetProjectPanelSideMode(PanelSideMode);	
	ProjectPtr->SetProjectWorkNumber(WorkNumber);

	ProjectPtr->SetProjectFdFolder(FiducialFolder);
	ProjectPtr->SetProjectLibraryFolder(LibraryFolder);
	ProjectPtr->SetProjectPartLibraryFolder(PartLibraryFolder);
	ProjectPtr->SetProjectProgramOfflineFolder(ProgramOfflinelFolder);
	ProjectPtr->SetProjectInspectionOfflineFolder(InspectionOfflineFolder);
	ProjectPtr->SetProjectInspectionOfflineFolderDefault(InspectionOfflineFolder);	

	ProjectPtr->CreateProjectServerLibraryFolder();
	if ( false == IsNewProject )
	{
		double FocusPosZ = ProjectPtr->GetProjectFocusPos();
		ProjectPtr->SetProjectProgramOfflineFolder(ProgramOfflinelFolderSrc);
		if ( MotionCtrlPtr->MoveTo(AXIS_Z, FocusPosZ, MOTION_MOVING_NORMAL) == false )
		{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());	}
		if ( MotionCtrlPtr->WaitForMotionStop() == false )
		{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());	}
	}
	/*
	if ( AOIDataCollect.SetProjectLightSetting(ProjectPtr) == false )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());		
		return FALSE;
	}*/
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneIntroduction::ExecPrevPane()
{	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneIntroduction::ExecFinishPane()
{
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneIntroduction::ReInitialPane()
{
	return true;
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneIntroduction::OnPCBInBtn() 
{
	// TODO: Add your control notification handler code here	
	const bool bStep = true;
	LANE_ID LaneID = (LANE_ID)(JetAPI::GetComboxCurSelData(m_LaneCombox));
	if ( AOIDataCollect.ExecPCBInProc(LaneID, true, bStep) == false )
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	}
	return ;
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneIntroduction::OnPCBBackBtn() 
{
	// TODO: Add your control notification handler code here	
	const bool bStep = true;
	LANE_ID LaneID = (LANE_ID)(JetAPI::GetComboxCurSelData(m_LaneCombox));
	if ( AOIDataCollect.ExecPCBBackProc(LaneID, true, bStep) == false )
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	}
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneIntroduction::OnPCBOutBtn() 
{
	// TODO: Add your control notification handler code here	
	const bool bStep = true;
	LANE_ID LaneID = (LANE_ID)(JetAPI::GetComboxCurSelData(m_LaneCombox));
	if ( AOIDataCollect.ExecPCBOutProc(LaneID, true, bStep) == false )
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	}
	return ;
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneIntroduction::OnPCBClampOnBtn() 
{
	// TODO: Add your control notification handler code here	
	const bool bStep = true;
	LANE_ID LaneID = (LANE_ID)(JetAPI::GetComboxCurSelData(m_LaneCombox));
	if ( AOIDataCollect.ExecPCBClampOnProc(LaneID) == false )
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	}
	return ;
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneIntroduction::OnLaneWidthGetBtn() 
{
	// TODO: Add your control notification handler code here
	CString str;
	LANE_ID LaneID = (LANE_ID)(JetAPI::GetComboxCurSelData(m_LaneCombox));
	double LaneWidth = PlcCtrlPtr->ReadLaneAdjustCurrentPos(LaneID);
	str.Format(_T("%.2f"), LaneWidth);
	CWnd::SetDlgItemText(INTROD_LANE_WIDTH_EDIT, str);
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneIntroduction::OnLaneWidthSetBtn() 
{
	// TODO: Add your control notification handler code here
	CString str;	
	LANE_ID LaneID = (LANE_ID)(JetAPI::GetComboxCurSelData(m_LaneCombox));
	CWnd::GetDlgItemText(INTROD_LANE_WIDTH_EDIT, str);
	double LaneWidth = JetAPI::StrToDbl(str);	
	if ( AOIDataCollect.ExecLaneAdjustWidth(LaneID, LaneWidth) == false )
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString()); }
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneIntroduction::OnPCBIn2ndBtn() 
{
	// TODO: Add your control notification handler code here
	const bool bStep = true;
	LANE_ID LaneID = (LANE_ID)(JetAPI::GetComboxCurSelData(m_LaneCombox));
	if ( AOIDataCollect.ExecPCBIn2ndProc(LaneID, true, bStep) == false )
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	}
	return ;
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneIntroduction::OnPCBIn3rdBtn() 
{
	// TODO: Add your control notification handler code here
	const bool bStep = true;
	LANE_ID LaneID = (LANE_ID)(JetAPI::GetComboxCurSelData(m_LaneCombox));
	if ( AOIDataCollect.ExecPCBIn3rdProc(LaneID, true, bStep) == false )
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	}
	return ;
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneIntroduction::OnLaneAdjustWidthBtn() 
{
	// TODO: Add your control notification handler code here	
	LANE_ID LaneID = (LANE_ID)(JetAPI::GetComboxCurSelData(m_LaneCombox));
	if ( PlcCtrlPtr->CheckLaneAdjustCanMove(LaneID) == false )
	{
		JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString());
		return;
	}

	CString      strValue;
	CString      strLabel;
	CString      strCaption;	
	CInputBoxWnd InputBox;
	double Pos = PlcCtrlPtr->ReadLaneAdjustCurrentPos(LaneID);
	
	strValue.Format(_T("%.3f"), Pos);
	strLabel = _T("Width");
	strLabel = LoadMultiLanguageString(strLabel, strLabel);
	strCaption = _T("Set Lane Adjust Width");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	InputBox.SetParam1(strCaption, strLabel, strValue);
	if ( InputBox.DoModal() == IDCANCEL ) 
	{	return;  }
	
	double NewPos = JetAPI::StrToDbl(InputBox.m_DataEdit1);
	if ( AOIDataCollect.ExecLaneAdjustWidth(LaneID, NewPos) == false )
	{	
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	
		return;
	}
	OnLaneWidthGetBtn();
	return ;
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneIntroduction::OnFrameCloseAllBtn() 
{
	// TODO: Add your control notification handler code here
	CloaseAllProjectFrameCombox();
	return;
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneIntroduction::OnFrameDefaultBtn() 
{
	// TODO: Add your control notification handler code here
	BuildProjectFrameCombox();
}
//-------------------------------------------------------------------------------------//