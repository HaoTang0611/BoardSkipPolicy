// NewProjectPanelAlignPane.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "NewProjectPanelAlignPane.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CNewProjectPaneAlignPanel dialog
//-------------------------------------------------------------------------------------//
CNewProjectPaneAlignPanel::CNewProjectPaneAlignPanel(CWnd* pParent /*=NULL*/)
	: CDialog(CNewProjectPaneAlignPanel::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewProjectPaneAlignPanel)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	const size_t InvalideIndex = -1;
	m_ProjectPtr = NULL;	
	m_FrameType = FRAME_COLOR;
	m_FrameIndex = 0;
	m_FrameUniqueID = FRAME_UNIQUE_ID_DEFAULT;//取像畫面的唯一碼	
	m_NewProjectMode = NEW_PROJECT_ONLINE;
	m_EnableMultiDistrictMode = false;

	m_ResetView = true;
	m_FovStageX = 0.0;
	m_FovStageY = 0.0;

	this->m_ImageW = 0;
	this->m_ImageH = 0;
	this->m_BitCount = 0;
	this->m_ImageStep = 0;
	this->m_ImageBuffer = NULL;
	this->m_ImageBufferSize = 0;

	this->m_ShowImageW = 0;
	this->m_ShowImageH = 0;
	this->m_ShowImageStep = 0;
	this->m_ShowBitCount = 0;
	this->m_ShowBuffer = NULL;
	this->m_ShowBufferSize = 0;

	m_CaliComponentIdx[0] = InvalideIndex;
	m_CaliComponentIdx[1] = InvalideIndex;
	m_CaliComponentIdx[2] = InvalideIndex;
	m_CaliComponentIdx[3] = InvalideIndex;
	::memset(m_CaliComponentCadPosX, 0x00, sizeof(m_CaliComponentCadPosX));
	::memset(m_CaliComponentCadPosY, 0x00, sizeof(m_CaliComponentCadPosY));
	::memset(m_CaliComponentStagePosX, 0x00, sizeof(m_CaliComponentStagePosX));
	::memset(m_CaliComponentStagePosY, 0x00, sizeof(m_CaliComponentStagePosY));	

	::memset(m_EdgeStagePosX, 0x00, sizeof(m_EdgeStagePosX));	
	::memset(m_EdgeStagePosY, 0x00, sizeof(m_EdgeStagePosY));
	m_DistrictID=DISTRICT_ID_A;
	m_BoxApplyRotation = false;
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneAlignPanel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewProjectPaneAlignPanel)
	DDX_Control(pDX, ALIGNPANEL_ALIGN_COMPONENT_COMBO_4, m_ComponentCombox4);
	DDX_Control(pDX, ALIGNPANEL_ALIGN_COMPONENT_COMBO_3, m_ComponentCombox3);
	DDX_Control(pDX, ALIGNPANEL_ALIGN_COMPONENT_COMBO_2, m_ComponentCombox2);
	DDX_Control(pDX, ALIGNPANEL_ALIGN_COMPONENT_COMBO_1, m_ComponentCombox1);
	DDX_Control(pDX, ALIGNPANEL_IMAGE_WND, m_ImageWnd);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CNewProjectPaneAlignPanel, CDialog)
	//{{AFX_MSG_MAP(CNewProjectPaneAlignPanel)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(ALIGNPANEL_ORIENTATION_ROTATE_090_BTN, OnOrientationRotate090Btn)
	ON_BN_CLICKED(ALIGNPANEL_ORIENTATION_ROTATE_180_BTN, OnOrientationRotate180Btn)
	ON_BN_CLICKED(ALIGNPANEL_ORIENTATION_ROTATE_270_BTN, OnOrientationRotate270Btn)
	ON_BN_CLICKED(ALIGNPANEL_ORIENTATION_MIRRORX_BTN, OnOrientationMirrorXBtn)
	ON_BN_CLICKED(ALIGNPANEL_ORIENTATION_MIRRORY_BTN, OnOrientationMirrorYBtn)
	ON_WM_PAINT()
	ON_BN_CLICKED(ALIGNPANEL_ORIENTATION_CENTERED_BTN, OnOrientationCenteredBtn)
	ON_CBN_SELCHANGE(ALIGNPANEL_ALIGN_COMPONENT_COMBO_1, OnSelchangeAlignComponentCombo1)
	ON_BN_CLICKED(ALIGNPANEL_ALIGN_COMPONENT_SET_BTN_1, OnAlignComponentSetBtn1)
	ON_CBN_SELCHANGE(ALIGNPANEL_ALIGN_COMPONENT_COMBO_2, OnSelchangeAlignComponentCombo2)
	ON_BN_CLICKED(ALIGNPANEL_ALIGN_COMPONENT_SET_BTN_2, OnAlignComponentSetBtn2)
	ON_BN_CLICKED(ALIGNPANEL_ALIGN_COMPONENT_GO_BTN_1, OnAlignComponentGoBtn1)
	ON_BN_CLICKED(ALIGNPANEL_ALIGN_COMPONENT_GO_BTN_2, OnAlignComponentGoBtn2)
	ON_CBN_SELCHANGE(ALIGNPANEL_ALIGN_COMPONENT_COMBO_3, OnSelchangeAlignComponentCombo3)
	ON_BN_CLICKED(ALIGNPANEL_ALIGN_COMPONENT_SET_BTN_3, OnAlignComponentSetBtn3)
	ON_BN_CLICKED(ALIGNPANEL_ALIGN_COMPONENT_GO_BTN_3, OnAlignComponentGoBtn3)
	ON_BN_CLICKED(ALIGNPANEL_ASSIGN_COMPONENT_BTN, OnAssignComponentBtn)
	ON_CBN_SELCHANGE(ALIGNPANEL_ALIGN_COMPONENT_COMBO_4, OnSelchangeAlignComponentCombo4)
	ON_BN_CLICKED(ALIGNPANEL_ALIGN_COMPONENT_SET_BTN_4, OnAlignComponentSetBtn4)
	ON_BN_CLICKED(ALIGNPANEL_ALIGN_COMPONENT_GO_BTN_4, OnAlignComponentGoBtn4)
	ON_BN_CLICKED(ALIGNPANEL_PANEL_EDGE_POS1_GO_BTN, OnPanelEdgePos1GoBtn)
	ON_BN_CLICKED(ALIGNPANEL_PANEL_EDGE_POS2_GO_BTN, OnPanelEdgePos2GoBtn)
	ON_BN_CLICKED(ALIGNPANEL_PANEL_EDGE_POS1_SET_BTN, OnPanelEdgePos1SetBtn)
	ON_BN_CLICKED(ALIGNPANEL_PANEL_EDGE_POS2_SET_BTN, OnPanelEdgePos2SetBtn)
	ON_BN_CLICKED(ALIGNPANEL_PANEL_EDGE_CALC_BTN, OnPanelEdgeCalcBtn)
	ON_BN_CLICKED(ALIGNPANEL_SHOW_CENTER_LINE, OnShowCenterLine)
	ON_BN_CLICKED(ALIGNPANEL_DISTRICT_A_BTN, OnDistrictABtn)
	ON_BN_CLICKED(ALIGNPANEL_DISTRICT_B_BTN, OnDistrictBBtn)
	ON_BN_CLICKED(ALIGNPANEL_APPLY_ROTATION, OnApplyRotation)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CNewProjectPaneAlignPanel message handlers
//-------------------------------------------------------------------------------------//
BOOL CNewProjectPaneAlignPanel::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here	
	m_ImageWnd.GetClientRect(&m_ImageWndRect);
	m_ImageWnd.SetProjectPtr(this->m_ProjectPtr);
	m_ImageWnd.SetShowLBtnPos(true);
	m_ImageWnd.SetRBtnUpMode(IMAGE_RBTN_UP_MOVE_STAGE);
	m_ImageWnd.SetRBtnClickMode(IMAGE_RBTN_CLICK_CTRL_VIEW);	
	m_ImageWnd.SetLBtnClickMode(IMAGE_LBTN_CLICK_MOVE_PANEL_STAGE);	//IMAGE_LBTN_CLICK_MOVE_PANEL
#ifdef _DEBUG
	m_ImageWnd.SetShowCursorInfo(true);
#endif
	SwitchMultiLanguage();	

	SetDlgItemInt(ALIGNPANEL_PANEL_EDGE_CALC_EDIT, 0);
	
	CString str;
	CAMERA_ID CameraID = PRIMARY_CAMERA_ID;	
	DISTRICT_ID DistrictID = GetDistrictID();
	str = AOIDataDefine.GetDistrictIDText(DistrictID);
	CWnd::SetDlgItemText(ALIGNPANEL_DISTRICT_EDIT, str);
	m_BitCount = 8;
	m_ImageW = CameraCtrl.GetCameraImageSizeW(CameraID);
	m_ImageH = CameraCtrl.GetCameraImageSizeH(CameraID);	
	m_ImageStep = JetAPI::GetBMPImagePixelsPerLine(m_ImageW, m_BitCount, 4);
	m_ImageWnd.SetImageBuffer(m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ShowBuffer, false, true);	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneAlignPanel::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	this->m_ImageBuffer = NULL;
	this->m_ImageBufferSize = 0;
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneAlignPanel::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if ( this->m_ImageWnd.GetSafeHwnd() != NULL )
	{
		RECT  WndRect={0};
		this->m_ImageWnd.GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		WndRect.right = cx;
		WndRect.bottom = cy;
		this->m_ImageWnd.MoveWindow(&WndRect);	
		this->m_ImageWnd.GetClientRect(&m_ImageWndRect);
	}
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneAlignPanel::OnOK() 
{
	// TODO: Add extra validation here
	return;
	CDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneAlignPanel::OnCancel() 
{
	// TODO: Add extra cleanup here
	return;
	CDialog::OnCancel();
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneAlignPanel::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_NEW_PROJECT_PANE_ALIGN_PANEL");
	//---------------------------------------------------------------------------------//
	WndID = IDD_NEW_PROJECT_PANE_ALIGN_PANEL;
	WndKey = _T("IDD_NEW_PROJECT_PANE_ALIGN_PANEL");
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
	WndID = ALIGNPANEL_ORIENTATION_GROUP;
	WndKey = _T("ALIGNPANEL_ORIENTATION_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ALIGNPANEL_ORIENTATION_ROTATE_090_BTN;
	WndKey = _T("ALIGNPANEL_ORIENTATION_ROTATE_090_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ALIGNPANEL_ORIENTATION_ROTATE_180_BTN;
	WndKey = _T("ALIGNPANEL_ORIENTATION_ROTATE_180_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ALIGNPANEL_ORIENTATION_ROTATE_270_BTN;
	WndKey = _T("ALIGNPANEL_ORIENTATION_ROTATE_270_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ALIGNPANEL_ORIENTATION_MIRRORX_BTN;
	WndKey = _T("ALIGNPANEL_ORIENTATION_MIRRORX_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ALIGNPANEL_ORIENTATION_MIRRORY_BTN;
	WndKey = _T("ALIGNPANEL_ORIENTATION_MIRRORY_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	
	WndID = ALIGNPANEL_ORIENTATION_CENTERED_BTN;
	WndKey = _T("ALIGNPANEL_ORIENTATION_CENTERED_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = ALIGNPANEL_ALIGNMENT_GROUP;
	WndKey = _T("ALIGNPANEL_ALIGNMENT_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ALIGNPANEL_ASSIGN_COMPONENT_BTN;
	WndKey = _T("ALIGNPANEL_ASSIGN_COMPONENT_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ALIGNPANEL_ALIGN_COMPONENT_LABEL_1;
	WndKey = _T("ALIGNPANEL_ALIGN_COMPONENT_LABEL_1");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	WndID = ALIGNPANEL_ALIGN_COMPONENT_GO_BTN_1;
	WndKey = _T("ALIGNPANEL_ALIGN_COMPONENT_GO_BTN_1");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ALIGNPANEL_ALIGN_COMPONENT_SET_BTN_1;
	WndKey = _T("ALIGNPANEL_ALIGN_COMPONENT_SET_BTN_1");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ALIGNPANEL_ALIGN_COMPONENT_LABEL_2;
	WndKey = _T("ALIGNPANEL_ALIGN_COMPONENT_LABEL_2");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ALIGNPANEL_ALIGN_COMPONENT_GO_BTN_2;
	WndKey = _T("ALIGNPANEL_ALIGN_COMPONENT_GO_BTN_2");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ALIGNPANEL_ALIGN_COMPONENT_SET_BTN_2;
	WndKey = _T("ALIGNPANEL_ALIGN_COMPONENT_SET_BTN_2");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	
	WndID = ALIGNPANEL_ALIGN_COMPONENT_LABEL_3;
	WndKey = _T("ALIGNPANEL_ALIGN_COMPONENT_LABEL_3");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ALIGNPANEL_ALIGN_COMPONENT_GO_BTN_3;
	WndKey = _T("ALIGNPANEL_ALIGN_COMPONENT_GO_BTN_3");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ALIGNPANEL_ALIGN_COMPONENT_SET_BTN_3;
	WndKey = _T("ALIGNPANEL_ALIGN_COMPONENT_SET_BTN_3");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	
	WndID = ALIGNPANEL_ALIGN_COMPONENT_LABEL_4;
	WndKey = _T("ALIGNPANEL_ALIGN_COMPONENT_LABEL_4");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ALIGNPANEL_ALIGN_COMPONENT_GO_BTN_4;
	WndKey = _T("ALIGNPANEL_ALIGN_COMPONENT_GO_BTN_4");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ALIGNPANEL_ALIGN_COMPONENT_SET_BTN_4;
	WndKey = _T("ALIGNPANEL_ALIGN_COMPONENT_SET_BTN_4");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		
	//---------------------------------------------------------------------------------//
	WndID = ALIGNPANEL_PANEL_EDGE_GROUP;
	WndKey = _T("ALIGNPANEL_PANEL_EDGE_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = ALIGNPANEL_PANEL_EDGE_POS1_GO_BTN;
	WndKey = _T("ALIGNPANEL_PANEL_EDGE_POS1_GO_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = ALIGNPANEL_PANEL_EDGE_POS1_SET_BTN;
	WndKey = _T("ALIGNPANEL_PANEL_EDGE_POS1_SET_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = ALIGNPANEL_PANEL_EDGE_POS2_GO_BTN;
	WndKey = _T("ALIGNPANEL_PANEL_EDGE_POS2_GO_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = ALIGNPANEL_PANEL_EDGE_POS2_SET_BTN;
	WndKey = _T("ALIGNPANEL_PANEL_EDGE_POS2_SET_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = ALIGNPANEL_PANEL_EDGE_CALC_BTN;
	WndKey = _T("ALIGNPANEL_PANEL_EDGE_CALC_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = ALIGNPANEL_SHOW_CENTER_LINE;
	WndKey = _T("ALIGNPANEL_SHOW_CENTER_LINE");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//
	WndID = ALIGNPANEL_DISTRICT_A_BTN;
	WndKey = _T("ALIGNPANEL_DISTRICT_A_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = ALIGNPANEL_DISTRICT_B_BTN;
	WndKey = _T("ALIGNPANEL_DISTRICT_B_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = ALIGNPANEL_APPLY_ROTATION;
	WndKey = _T("ALIGNPANEL_APPLY_ROTATION");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneAlignPanel::SetShowBuffer(size_t BufferSize, IMAGE_PTR Ptr)
{
	this->m_ShowBuffer = Ptr;	
	this->m_ShowBufferSize = BufferSize;	
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneAlignPanel::SetImageBuffer(size_t BufferSize, IMAGE_PTR Ptr)
{
	this->m_ImageBuffer = Ptr;	
	this->m_ImageBufferSize = BufferSize;
}
//-------------------------------------------------------------------------------------//
LRESULT CNewProjectPaneAlignPanel::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class	
	switch ( message )
	{
	case MSG_CAMERA_CALLBACK:
		m_ResetView = true;
		if ( this->UpdateFovImage(wParam, lParam, true) == false )
		{	this->LockUIWnd(false); }
		break;
	case MSG_CAMERA_REGRAB_IMAGE:
		this->ExecGrabImage();
		break;
	case MSG_SYSTEM_EXCEPTION_CALLBACK:
		AOIDataCollect.ExecSystemException(wParam, lParam);		
		this->LockUIWnd(false);
		break;
	case MSG_IMAGE_WND_DRAW_NEXT:		
		DrawCtrlWnd(wParam, lParam);
		break;
	case MSG_IMAGE_WND_NOTIFY_EVENT:
		switch ( wParam )
		{
		case WPARAM_MODIFY_STAGE_POS:
			RedrawProjectImageWnd();
			break;
		}
		break;
	case MSG_EDIT_MAIN_VIEW_WND:
		switch ( wParam )
		{
		case WPARAM_SWITCH_FRAME_IMAGE:
			m_ResetView = false;
			if ( this->UpdateFovImage(PRIMARY_CAMERA_ID, lParam, false) == false )
			{	this->LockUIWnd(false); }
			break;
		}
		break;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
 void CNewProjectPaneAlignPanel::SetProjectPtr(CAOIProject *ProjectPtr)
{
	m_ProjectPtr = ProjectPtr;
}
//-------------------------------------------------------------------------------------//
 DISTRICT_ID CNewProjectPaneAlignPanel::GetDistrictID() const
{
	return m_DistrictID;
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneAlignPanel::SetDistrictID(DISTRICT_ID Mode)
{
	m_DistrictID = Mode;
}
//-------------------------------------------------------------------------------------//
 NEW_PROJECT_MODE CNewProjectPaneAlignPanel::GetNewProjectMode() const
{
	return m_NewProjectMode;
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneAlignPanel::SetNewProjectMode(NEW_PROJECT_MODE Mode)
{
	m_NewProjectMode = Mode;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneAlignPanel::GetEnableMultiDistrictMode() const
{ 
	return m_EnableMultiDistrictMode; 
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneAlignPanel::SetEnableMultiDistrictMode(bool Mode)
{ 
	m_EnableMultiDistrictMode = Mode; 
}	
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneAlignPanel::GetEnableModifyPanelDirection() const
{
	if ( GetEnableMultiDistrictMode() == true )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneAlignPanel::LockUIWnd(bool bLock)
{
	UINT  CtrlID = 0;
	BOOL  bEnable = TRUE;	

	if ( true == bLock ) { bEnable = FALSE; }
	else { bEnable = TRUE; }		
	
	BOOL bEnable2 = bEnable;
	if ( TRUE == bEnable )
	{	bEnable2 = GetEnableModifyPanelDirection(); }
	CtrlID = ALIGNPANEL_ORIENTATION_ROTATE_090_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2);
	CtrlID = ALIGNPANEL_ORIENTATION_ROTATE_180_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2);
	CtrlID = ALIGNPANEL_ORIENTATION_ROTATE_270_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2);
	CtrlID = ALIGNPANEL_ORIENTATION_MIRRORX_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2);
	CtrlID = ALIGNPANEL_ORIENTATION_MIRRORY_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2);
	CtrlID = ALIGNPANEL_ORIENTATION_CENTERED_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2);	
	

	//Edit Control
//	CtrlID = REGION_CORNER_WIDTH_EDIT;
//	JetAPI::EnableEditWnd(this, CtrlID, bEnable);
	AOIDataCollect.SetIsLockUIWnd(bLock);
	return true;
}
//-------------------------------------------------------------------------------------//
BOOL CNewProjectPaneAlignPanel::CreateBKDC(bool bResetView)
{
	if ( NULL==m_ShowBuffer ) { return FALSE; }	
	TPOINT2D   ImageRes;
	TPOINT2D   StagePos;
	TREGION4D  StageRgn;
	IMAGE_SIZE ImageW=0, ImageH=0;
	CAMERA_ID  CameraID = PRIMARY_CAMERA_ID;	
	AOIDataCollect.GetFovStageRegionReal(StageRgn);
	AOIDataCollect.GetCameraImageInfo(CameraID, ImageW, ImageH, ImageRes);	
	this->m_ImageWnd.SetImageInfo(CameraID, StageRgn, ImageRes, IMAGE_DATA_FOV);
	this->m_ImageWnd.SetImageBuffer(m_ShowImageW, m_ShowImageH, m_ShowImageStep, m_ShowBitCount, m_ShowBuffer, false, bResetView);	
	CNewProjectPaneAlignPanel::RedrawProjectImageWnd();
	return TRUE;	
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneAlignPanel::RedrawWnd()
{
}
//-------------------------------------------------------------------------------------//
BOOL CNewProjectPaneAlignPanel::DrawCtrlWnd(WPARAM wParam, LPARAM lParam)//在控制像繪圖後重新繪圖
{
	HDC  hDC = (HDC)(lParam);
	UINT CtrlID = (UINT)(wParam);	
	switch ( CtrlID )
	{
	case ALIGNPANEL_IMAGE_WND:
		//::MoveToEx(hDC, 0, 300, NULL);
		//::LineTo(hDC, 600, 300);
		break;
	}	
	return TRUE;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneAlignPanel::UpdateFovImage(WPARAM wParam, LPARAM lParam, bool bCameraCallBack)
{
	bool bGetImage = false;
	if ( AOIDataCollect.GetOfflineMode() == false )
	{
	#ifndef LIGHT_CTRL_DISABLE
		if ( RetrieveCameraUniFrame(wParam, lParam, bCameraCallBack, bGetImage) == false )
		{	return false; }
	#else
		if ( RetrieveCameraImage(wParam, lParam, bCameraCallBack, bGetImage) == false )
		{	return false; }
	#endif//LIGHT_CTRL_DISABLE
		if ( false == bGetImage ) { return true; }
	}
	else
	{
		if ( LoadProgramOfflineImage(wParam, lParam, bGetImage) == false )
		{	return false; }		
	}

	//無適合的資料
	if ( false == bGetImage )
	{	::memset(m_ShowBuffer, 0x00, sizeof(unsigned char)*m_ShowBufferSize);	}
	else
	{
		if ( AOIDataCollect.ExecEnhanceDisplayImage(m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ImageBuffer, m_ShowBuffer) == false )
		{
			JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
			return false;
		}		
	}
	m_ShowImageW = m_ImageW;
	m_ShowImageH = m_ImageH;
	m_ShowImageStep = m_ImageStep;
	m_ShowBitCount = m_BitCount;
	this->CreateBKDC(m_ResetView); 
	this->LockUIWnd(false);			
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneAlignPanel::RetrieveCameraImage(WPARAM wParam, LPARAM lParam, bool bCameraCallBack, bool &bGetImage)//取得相機影像
{
	CAOIProject *ProjectPtr = m_ProjectPtr;
	if ( NULL == ProjectPtr ) {	return false; }
	if ( NULL == this->m_ImageBuffer ) { return false; }
	if ( NULL == this->m_ShowBuffer ) { return false; }

	size_t   i=0;	
	TSIZE2D  Res;
	TPOINT3D Pos;
	IMAGE_SIZE ImageStep=0;
	IMAGE_SIZE BitCount=0;
	size_t   BuffserSize=0;
	CAMERA_ID CameraID = (CAMERA_ID)(wParam);
	if ( PRIMARY_CAMERA_ID != CameraID ) { return false; }		
	
	IMAGE_SIZE ImageW = AOIDataCollect.GetCameraImageW(CameraID);
	IMAGE_SIZE ImageH = AOIDataCollect.GetCameraImageH(CameraID);
	if ( LPARAM_CAMERA_BYPASS_CALLBACK == lParam )
	{
		//str.Format(_T("Image Callback#%d"), CameraCtrl.GetImageCallbackCount(CameraID));		
		return true;	
	}
	
	bool bReturn=false;
	if ( CameraCtrl.RetrieveCameraImageCallback(CameraID, bReturn) == false )
	{
		JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());		
		return false; 
	}
	if ( true == bReturn )
	{	return true; }

	IMAGE_DISPLAY_MODE ImageDisplayMode = AOIDataCollect.MapFrameTypeToImageDisplayMode(m_FrameType);
	//if ( CameraCtrl.FillCameraImage3(CameraID, ImageDisplayMode, m_ShowImageW, m_ShowImageH, m_ShowImageStep, m_ShowBitCount, m_ShowBuffer) == false )
	if ( CameraCtrl.FillCameraImage3(CameraID, ImageDisplayMode, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ImageBuffer) == false )
	{
		JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());
		return false;
	}
	bGetImage = true;

	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneAlignPanel::RetrieveCameraUniFrame(WPARAM wParam, LPARAM lParam, bool bCameraCallBack, bool &bGetImage)//取得相機影像	
{
	CAOIProject *ProjectPtr = m_ProjectPtr;
	if ( NULL == ProjectPtr ) {	return false; }
	if ( NULL == this->m_ImageBuffer ) { return false; }
	if ( NULL == this->m_ShowBuffer ) { return false; }

	size_t   i=0;	
	TSIZE2D  Res;
	TPOINT3D Pos;
	IMAGE_SIZE ImageStep=0;
	IMAGE_SIZE BitCount=0;
	size_t   BuffserSize=0;
	CAMERA_ID CameraID = (CAMERA_ID)(wParam);	
	if ( PRIMARY_CAMERA_ID != CameraID ) { return false; }

	const unsigned int MapIndex = ProjectPtr->GetProjectMapIndex();	
	//const unsigned int MapIndex = 2;	
	IMAGE_SIZE ImageW = AOIDataCollect.GetCameraImageW(CameraID);
	IMAGE_SIZE ImageH = AOIDataCollect.GetCameraImageH(CameraID);
	if ( LPARAM_CAMERA_BYPASS_CALLBACK == lParam )
	{
		//str.Format(_T("Image Callback#%d"), CameraCtrl.GetImageCallbackCount(CameraID));		
		return true;	
	}	
	bool bReturn=false;
	if ( CameraCtrl.RetrieveCameraImageCallback(CameraID, bReturn) == false )
	{
		JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());		
		return false; 
	}
	if ( true == bReturn )
	{	return true; }

	std::vector<TUNI_FRAME>   UniFrameList;
	std::vector<TFrameParam>  GrabFrameParamList;//影像參數列表			
	AOIDataCollect.GetGrabFrameParamList(GrabFrameParamList);
	if ( true == bCameraCallBack )
	{	AOIDataCollect.ModifyCameraUniFrame(GrabFrameParamList);	}
	if ( AOIDataCollect.RetrieveCameraUniFrame(GrabFrameParamList, UniFrameList) == false )
	{	
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());		
		return false;
	}		
	const size_t UniFrameCount = UniFrameList.size();
	if ( 0 == UniFrameCount ) 
	{	return true; }

	TUNI_FRAME UniFrame = UniFrameList[0];
	IMAGE_DISPLAY_MODE ImageDisplayMode = AOIDataCollect.MapFrameTypeToImageDisplayMode(m_FrameType);	
	
	const size_t MaxFrames = UniFrameList.size();
	if ( MapIndex>=0 && MapIndex<MaxFrames )
	{	UniFrame = UniFrameList[MapIndex]; }
	else
	{	UniFrame = UniFrameList[0]; }
	
	BuffserSize = ImageAPI.CalcBufferSize(UniFrame.ImageStep, UniFrame.ImageH);
	if ( NULL!=UniFrame.ImagePtr && BuffserSize <= m_ImageBufferSize )
	{	
		m_ImageW = UniFrame.ImageW;
		m_ImageH = UniFrame.ImageH;
		m_ImageStep = UniFrame.ImageStep;
		m_BitCount = UniFrame.BitCount;		
		::memcpy(m_ImageBuffer, UniFrame.ImagePtr, sizeof(unsigned char)*BuffserSize);
	}	
	else
	{	::memset(m_ImageBuffer, 0x00, sizeof(unsigned char)*m_ImageBufferSize);	}
	JetAPI::ClearUniFrameList(UniFrameList);
	bGetImage = true;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneAlignPanel::LoadProgramOfflineImage(WPARAM wParam, LPARAM lParam, bool &bGetImage)
{
	CAOIProject *ProjectPtr = m_ProjectPtr;
	if ( NULL == ProjectPtr ) {	return false; }
	if ( NULL == this->m_ImageBuffer ) { return false; }
	if ( NULL == this->m_ShowBuffer ) { return false; }

	size_t   i=0;	
	TSIZE2D  Res;
	TPOINT3D Pos;
	IMAGE_SIZE ImageStep=0;
	IMAGE_SIZE BitCount=0;
	size_t   BuffserSize=0;
	CAMERA_ID CameraID = (CAMERA_ID)(wParam);
	if ( PRIMARY_CAMERA_ID != CameraID ) { return false; }

	double PosX=0, PosY=0, PosZ=0;
	const int  MaxFrames = FRAME_MAX_COUNT;
	TUNI_FRAME UniFrameList[FRAME_MAX_COUNT];
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();
	const unsigned int MapIndex = ProjectPtr->GetProjectMapIndex();
	IMAGE_SIZE ImageW = AOIDataCollect.GetCameraImageW(CameraID);
	IMAGE_SIZE ImageH = AOIDataCollect.GetCameraImageH(CameraID);

	if ( MotionCtrlPtr->GetCurrentPos(PosX, PosY, PosZ, OfflineMode) == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return false;
	}
	Pos.x = PosX;
	Pos.y = PosY;
	Res.cx = AOIDataCollect.GetCameraResolutionX(CameraID);
	Res.cy = AOIDataCollect.GetCameraResolutionY(CameraID);

	m_ResetView = JetAPI::CheckMoved(m_FovStageX, m_FovStageY, PosX, PosY);	
	m_FovStageX = PosX;
	m_FovStageY = PosY;	
	
	OFFLINE_FILE_MODE OfflineFileMode = OFFLINE_FILE_PROGRAM;
	if ( ProjectPtr->FillCurrentFrame(OfflineFileMode, Pos, Res, ImageW, ImageH, UniFrameList, MaxFrames) == false )
	{	JetAPI::ShowMessageBox(ProjectPtr->GetErrorString());	}
	else
	{
		if ( MapIndex>=0 && MapIndex<MaxFrames )
		{
			BitCount = UniFrameList[MapIndex].BitCount;
			ImageStep = UniFrameList[MapIndex].ImageStep;	
			BuffserSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
			if ( NULL!=UniFrameList[MapIndex].ImagePtr && BuffserSize <= m_ShowBufferSize )
			{
				bGetImage = true;
				m_ImageW = ImageW;
				m_ImageH = ImageH;
				m_ImageStep = ImageStep;
				m_BitCount = BitCount;
				::memcpy(m_ImageBuffer, UniFrameList[MapIndex].ImagePtr, sizeof(unsigned char)*BuffserSize);
			}
		}
	}	
	JetAPI::ClearUniFrameList(UniFrameList, FRAME_MAX_COUNT);	
	
	return true;
}
//-------------------------------------------------------------------------------------//
BOOL CNewProjectPaneAlignPanel::ExecGrabImage()
{	
	if ( NULL == this->m_ProjectPtr ) { return FALSE; }
#ifndef OFFLINE_VERSION
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();
	if ( true == OfflineMode )
	{
		PostMessage(MSG_CAMERA_CALLBACK, PRIMARY_CAMERA_ID, NULL);
		return TRUE;
	}
	if ( AOIDataCollect.CheckCanGrabNextUniFrameImage() == false )
	{	return TRUE;	}
	this->LockUIWnd(true);
	AOIDataCollect.SetThreadGrabMode(THREAD_GRAB_NONE);	
#ifndef LIGHT_CTRL_DISABLE
	if ( AOIDataCollect.ExecGrabNextUniFrameImage() == false )
	{		
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		this->LockUIWnd(false);
		return FALSE;
	}	
#else
	m_FrameIndex = 0;
	if ( AOIDataCollect.ExecGrabFrameImage(m_FrameUniqueID, m_FrameType) == false )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		this->LockUIWnd(false);
		return FALSE;
	}
#endif//LIGHT_CTRL_DISABLE	
#else
	PostMessage(MSG_CAMERA_CALLBACK, PRIMARY_CAMERA_ID, NULL);
#endif//OFFLINE_VERSION
	return TRUE;
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneAlignPanel::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if ( TRUE == bShow )
	{	
		DISTRICT_ID  DistrictID = GetDistrictID();
		ChangeDistrictID(DistrictID);
		BuildComponentCombox(m_ComponentCombox1);
		BuildComponentCombox(m_ComponentCombox2);
		BuildComponentCombox(m_ComponentCombox3);
		BuildComponentCombox(m_ComponentCombox4);
		InitialComponentCombox();
		m_ImageWnd.ShowFittedZoom();

		BOOL bEnable = FALSE;
		CAD_FILE_CONTENT_MODE CADFileContentMode=AOIDataCollect.GetLoadCADFileContentMode();
		if ( CAD_FILE_CONTENT_FIDUCIAL == CADFileContentMode )
		{	bEnable = FALSE; }
		else
		{	bEnable = TRUE; }
		JetAPI::EnableCtrlWnd(this, ALIGNPANEL_ALIGN_COMPONENT_COMBO_1, bEnable);
		JetAPI::EnableCtrlWnd(this, ALIGNPANEL_ALIGN_COMPONENT_SET_BTN_1, bEnable);
		JetAPI::EnableCtrlWnd(this, ALIGNPANEL_ALIGN_COMPONENT_GO_BTN_1, bEnable);
			
		JetAPI::EnableCtrlWnd(this, ALIGNPANEL_ALIGN_COMPONENT_COMBO_2, bEnable);
		JetAPI::EnableCtrlWnd(this, ALIGNPANEL_ALIGN_COMPONENT_SET_BTN_2, bEnable);
		JetAPI::EnableCtrlWnd(this, ALIGNPANEL_ALIGN_COMPONENT_GO_BTN_2, bEnable);

		JetAPI::EnableCtrlWnd(this, ALIGNPANEL_ALIGN_COMPONENT_COMBO_3, bEnable);
		JetAPI::EnableCtrlWnd(this, ALIGNPANEL_ALIGN_COMPONENT_SET_BTN_3, bEnable);
		JetAPI::EnableCtrlWnd(this, ALIGNPANEL_ALIGN_COMPONENT_GO_BTN_3, bEnable);

		JetAPI::EnableCtrlWnd(this, ALIGNPANEL_ALIGN_COMPONENT_COMBO_4, bEnable);
		JetAPI::EnableCtrlWnd(this, ALIGNPANEL_ALIGN_COMPONENT_SET_BTN_4, bEnable);
		JetAPI::EnableCtrlWnd(this, ALIGNPANEL_ALIGN_COMPONENT_GO_BTN_4, bEnable);

		JetAPI::EnableCtrlWnd(this, ALIGNPANEL_PANEL_EDGE_POS1_GO_BTN, bEnable);
		JetAPI::EnableCtrlWnd(this, ALIGNPANEL_PANEL_EDGE_POS1_SET_BTN, bEnable);
		JetAPI::EnableCtrlWnd(this, ALIGNPANEL_PANEL_EDGE_POS2_GO_BTN, bEnable);
		JetAPI::EnableCtrlWnd(this, ALIGNPANEL_PANEL_EDGE_POS2_SET_BTN, bEnable);
		JetAPI::EnableCtrlWnd(this, ALIGNPANEL_PANEL_EDGE_CALC_BTN, bEnable);
				
		AOIDataCollect.SetCallbackWnd(this->GetSafeHwnd());		
		ExecGrabImage();		
		PostParentWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_SHOW_PROJECT_MAP_WND, TRUE);
	}
}
//-------------------------------------------------------------------------------------//
BOOL CNewProjectPaneAlignPanel::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_LBUTTONDOWN)
	{
		if (false == m_BoxApplyRotation) {
			this->m_ImageWnd.SetImageEditRectAngle(0);
		}
		else {
			CAOIPanel   *PanelPtr = this->GetActivePanelPtr();
			CAOIComponent *ComponentPtr = NULL;
			if (NULL == PanelPtr)
			{
				JetAPI::ShowMessageBox(this->m_ErrorString);
			}
			else {
				const int PanelComponentCount = PanelPtr->GetPanelComponentCount();
				for (int i = 0; i < PanelComponentCount; i++) {
					ComponentPtr = PanelPtr->GetPanelComponentPtr(i, false);
					if (NULL == ComponentPtr) { continue; }
					bool IsSelected = ComponentPtr->GetComponentSelected();
					if (false == IsSelected) { continue; }
					double ComponentAngle = ComponentPtr->GetComponentAngle();
					if (fabs(ComponentAngle) < 0.001) { continue; }
					this->m_ImageWnd.SetImageEditRectAngle(ComponentAngle);
					break;
				}
			}
		}
	}
	if ( MotionCtrlPtr->ExecJogMSG(pMsg->message, pMsg->wParam, pMsg->lParam) == true ) 
	{	return TRUE; }
	return CDialog::PreTranslateMessage(pMsg);
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneAlignPanel::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CDialog::OnPaint() for painting messages
	CNewProjectPaneAlignPanel::RedrawWnd();
}
//-------------------------------------------------------------------------------------//
CAOIProject* CNewProjectPaneAlignPanel::GetActiveProjectPtr()//取得目前取用的專案指標
{
	return m_ProjectPtr;
}
//-------------------------------------------------------------------------------------//
CAOIPanel* CNewProjectPaneAlignPanel::GetActivePanelPtr()//取得目前取用的整板指標
{
	CAOIProject *ProjectPtr = GetActiveProjectPtr();
	if ( NULL == ProjectPtr ) 
	{ 
		this->m_ErrorString.Format(_T("Error, No Active Project"));
		return NULL; 
	}	
	CAOIPanel   *PanelPtr = ProjectPtr->GetProjectPanelPtrBySelected();
	if ( NULL == PanelPtr ) 
	{ 
		this->m_ErrorString.Format(_T("Error, No Active Panel"));
		return NULL; 
	}
	return PanelPtr;
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneAlignPanel::OnOrientationRotate090Btn() 
{
	// TODO: Add your control notification handler code here	
	CAOIPanel   *PanelPtr = GetActivePanelPtr();	
	if ( NULL == PanelPtr ) 
	{ 
		JetAPI::ShowMessageBox(this->m_ErrorString);
		return; 
	}
	PanelPtr->SpinPanel(90);	
	RedrawWindow();
	RedrawProjectImageWnd();
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneAlignPanel::OnOrientationRotate180Btn() 
{
	// TODO: Add your control notification handler code here
	CAOIPanel   *PanelPtr = GetActivePanelPtr();	
	if ( NULL == PanelPtr ) 
	{ 
		JetAPI::ShowMessageBox(this->m_ErrorString);
		return; 
	}
	PanelPtr->SpinPanel(180);	
	RedrawWindow();
	RedrawProjectImageWnd();
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneAlignPanel::OnOrientationRotate270Btn() 
{
	// TODO: Add your control notification handler code here
	CAOIPanel   *PanelPtr = GetActivePanelPtr();	
	if ( NULL == PanelPtr ) 
	{ 
		JetAPI::ShowMessageBox(this->m_ErrorString);
		return; 
	}
	PanelPtr->SpinPanel(270);	
	RedrawWindow();
	RedrawProjectImageWnd();
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneAlignPanel::OnOrientationMirrorXBtn() 
{
	// TODO: Add your control notification handler code here
	CAOIPanel   *PanelPtr = GetActivePanelPtr();	
	if ( NULL == PanelPtr ) 
	{ 
		JetAPI::ShowMessageBox(this->m_ErrorString);
		return; 
	}
	
	TREGION4D RgnCad;	
	RgnCad = PanelPtr->GetPanelRgnCad();	
	const double CadCpX = (RgnCad.minX+RgnCad.maxX)*0.5;
	PanelPtr->MirrorXPanel(CadCpX);	
	RedrawWindow();
	RedrawProjectImageWnd();
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneAlignPanel::OnOrientationMirrorYBtn() 
{
	// TODO: Add your control notification handler code here
	CAOIPanel   *PanelPtr = GetActivePanelPtr();	
	if ( NULL == PanelPtr ) 
	{ 
		JetAPI::ShowMessageBox(this->m_ErrorString);
		return; 
	}

	TREGION4D RgnCad;	
	RgnCad = PanelPtr->GetPanelRgnCad();	
	const double CadCpY = (RgnCad.minY+RgnCad.maxY)*0.5;
	PanelPtr->MirrorYPanel(CadCpY);	
	RedrawWindow();	
	RedrawProjectImageWnd();
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneAlignPanel::OnOrientationCenteredBtn() 
{
	// TODO: Add your control notification handler code here
	CAOIPanel   *PanelPtr = GetActivePanelPtr();
	if ( NULL == PanelPtr ) 
	{ 
		JetAPI::ShowMessageBox(this->m_ErrorString);
		return; 
	}

	double PosX = 0;
	double PosY = 0;
	DISTRICT_ID DistrictID = GetDistrictID();
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();	
	MotionCtrlPtr->GetCurrentPos(PosX, PosY, OfflineMode);	
	//A, B段同時更新
	if ( PanelPtr->SetPanelStagePos(PosX, PosY) == false )
	{	return;	}
	m_ImageWnd.SetProjectPtr(m_ProjectPtr);
	m_ImageWnd.SetShowLBtnPos(false);
	m_ImageWnd.SetLBtnClickMode(IMAGE_LBTN_CLICK_MOVE_PANEL_STAGE);	//IMAGE_LBTN_CLICK_MOVE_PANEL
	RedrawWindow();
	RedrawProjectImageWnd();
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneAlignPanel::OnSelchangeAlignComponentCombo1() 
{
	// TODO: Add your control notification handler code here
	/*
	CAOIPanel   *PanelPtr = this->GetActivePanelPtr();
	if ( NULL == PanelPtr ) 
	{ 
		JetAPI::ShowMessageBox(this->m_ErrorString);
		return; 
	}
	const int ComponentIndex = JetAPI::GetComboxCurSelData(m_ComponentCombox1);	
	CAOIComponent *pComponent = PanelPtr->GetPanelComponentPtr(ComponentIndex, true);
	if ( NULL == pComponent ) { return; }
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();
	MotionCtrlPtr->XYMoveTo(pComponent->GetComponentStagePosX(), pComponent->GetComponentStagePosY(), OfflineMode);
	this->ExecGrabImage();
	*/
	this->m_ImageWnd.SetLBtnClickMode(IMAGE_LBTN_CLICK_EDIT_BOX);
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneAlignPanel::OnAlignComponentSetBtn1() 
{
	// TODO: Add your control notification handler code here	
	CAOIPanel   *PanelPtr = GetActivePanelPtr();
	CAOIProject *ProjectPtr = GetActiveProjectPtr();
	if ( NULL==PanelPtr || NULL==ProjectPtr ) 
	{ 
		JetAPI::ShowMessageBox(this->m_ErrorString);
		return; 
	}	
	const DISTRICT_ID DistrictID = ProjectPtr->GetProjectActDistrictID();
	const unsigned int ComponentIndex = (unsigned int)(JetAPI::GetComboxCurSelData(m_ComponentCombox1));		
	CAOIComponent *pComponent = PanelPtr->GetPanelComponentPtr(ComponentIndex, true);
	if ( NULL == pComponent ) { return; }	

	TRECT4D EditRect;
	CAMERA_ID CameraID = PRIMARY_CAMERA_ID;	
	m_ImageWnd.GetImageEditRect(EditRect);
	if ( fabs(EditRect.right-EditRect.left)<0.0001 || fabs(EditRect.bottom-EditRect.top)<0.0001 )
	{	return;	}
	TPOINT2D ImagePt, StageCP, StagePt;
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();
	MotionCtrlPtr->GetCurrentPos(StageCP.x, StageCP.y, OfflineMode);
	ImagePt.x = (EditRect.left+EditRect.right)*0.5;
	ImagePt.y = (EditRect.top+EditRect.bottom)*0.5;
	AOIDataCollect.MapCameraPtToStage(CameraID, ImagePt, StageCP, StagePt);
	m_CaliComponentIdx[0] = ComponentIndex;
	m_CaliComponentStagePosX[0] = StagePt.x;
	m_CaliComponentStagePosY[0] = StagePt.y;	
	m_CaliComponentCadPosX[0] = pComponent->GetComponentCadPosX();
	m_CaliComponentCadPosY[0] = pComponent->GetComponentCadPosY();

	CMapCoordinate *CTSPtr = PanelPtr->GetPanelMapCTSPtr(DistrictID);//整板的座標轉換-Cad to Stage
	CMapCoordinate *STCPtr = PanelPtr->GetPanelMapSTCPtr(DistrictID);//整板的座標轉換-Stage to Cad
	if ( NULL==CTSPtr || NULL==STCPtr ) { return ; }
	CTSPtr->CalcMatrix2D(m_CaliComponentCadPosX, m_CaliComponentCadPosY, m_CaliComponentStagePosX, m_CaliComponentStagePosY, 1);
	STCPtr->CalcMatrix2D(m_CaliComponentStagePosX, m_CaliComponentStagePosY, m_CaliComponentCadPosX, m_CaliComponentCadPosY, 1);

	double CadXDst=0, CadYDst=0, StageXDst=0, StageYDst=0;
	const double CadXSrc = m_CaliComponentCadPosX[0];
	const double CadYSrc = m_CaliComponentCadPosY[0];
	const double StageXSrc = m_CaliComponentStagePosX[0];
	const double StageYSrc = m_CaliComponentStagePosY[0];
	CTSPtr->Map2D(CadXSrc, CadYSrc, StageXDst, StageYDst);
	STCPtr->Map2D(StageXSrc, StageYSrc, CadXDst, CadYDst);

	this->SetDlgItemInt(ALIGNPANEL_PANEL_EDGE_CALC_EDIT, 0);
	PanelPtr->AssignPanelMapParamToBoards(DistrictID);
	PanelPtr->CalcPanelStagePosition(DistrictID);
	PanelPtr->LayoutPanelBoardListRegion(DistrictID);
	PanelPtr->CheckPanelMapCoordinate(DistrictID, 1.0);
	//if ( 0 == PanelPtr->GetPanelIndex_Project() )
	//{	ProjectPtr->MapProjectMapStageToCadPos(*STCPtr);	}
	RedrawWindow();
	RedrawProjectImageWnd();
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneAlignPanel::OnSelchangeAlignComponentCombo2() 
{
	// TODO: Add your control notification handler code here
	/*
	CAOIPanel   *PanelPtr = this->GetActivePanelPtr();
	if ( NULL == PanelPtr ) 
	{ 
		JetAPI::ShowMessageBox(this->m_ErrorString);
		return; 
	}
	const int ComponentIndex = JetAPI::GetComboxCurSelData(m_ComponentCombox2);	
	CAOIComponent *pComponent = PanelPtr->GetPanelComponentPtr(ComponentIndex, true);
	if ( NULL == pComponent ) { return; }
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();
	MotionCtrlPtr->XYMoveTo(pComponent->GetComponentStagePosX(), pComponent->GetComponentStagePosY(), OfflineMode);
	this->ExecGrabImage();
	*/
	this->m_ImageWnd.SetLBtnClickMode(IMAGE_LBTN_CLICK_EDIT_BOX);
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneAlignPanel::OnAlignComponentSetBtn2() 
{
	// TODO: Add your control notification handler code here
	const size_t InvalideIndex = -1;
	if ( m_CaliComponentIdx[0] == InvalideIndex ) { return; }
	CAOIPanel   *PanelPtr = this->GetActivePanelPtr();
	CAOIProject *ProjectPtr = GetActiveProjectPtr();
	if ( NULL==PanelPtr || NULL==ProjectPtr ) 
	{ 
		JetAPI::ShowMessageBox(this->m_ErrorString);
		return; 
	}	
	const DISTRICT_ID DistrictID = ProjectPtr->GetProjectActDistrictID();
	const unsigned int ComponentIndex2 = (unsigned int)(JetAPI::GetComboxCurSelData(m_ComponentCombox2));	
	CAOIComponent *pComponent2 = PanelPtr->GetPanelComponentPtr(ComponentIndex2, true);
	if ( NULL == pComponent2 ) { return; }	

	TRECT4D EditRect;
	CAMERA_ID CameraID = PRIMARY_CAMERA_ID;
	m_ImageWnd.GetImageEditRect(EditRect);
	if ( fabs(EditRect.right-EditRect.left)<0.0001 || fabs(EditRect.bottom-EditRect.top)<0.0001 )
	{	return;	}
	TPOINT2D ImagePt, StageCP, StagePt;
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();
	MotionCtrlPtr->GetCurrentPos(StageCP.x, StageCP.y, OfflineMode);
	ImagePt.x = (EditRect.left+EditRect.right)*0.5;
	ImagePt.y = (EditRect.top+EditRect.bottom)*0.5;
	AOIDataCollect.MapCameraPtToStage(CameraID, ImagePt, StageCP, StagePt);
	m_CaliComponentIdx[1] = ComponentIndex2;
	m_CaliComponentStagePosX[1] = StagePt.x;
	m_CaliComponentStagePosY[1] = StagePt.y;
	m_CaliComponentCadPosX[1] = pComponent2->GetComponentCadPosX();
	m_CaliComponentCadPosY[1] = pComponent2->GetComponentCadPosY();

	CMapCoordinate *CTSPtr = PanelPtr->GetPanelMapCTSPtr(DistrictID);//整板的座標轉換-Cad to Stage
	CMapCoordinate *STCPtr = PanelPtr->GetPanelMapSTCPtr(DistrictID);//整板的座標轉換-Stage to Cad		
	if ( NULL==CTSPtr || NULL==STCPtr ) { return ; }
	CTSPtr->CalcMatrix2D(m_CaliComponentCadPosX, m_CaliComponentCadPosY, m_CaliComponentStagePosX, m_CaliComponentStagePosY, 2);
	STCPtr->CalcMatrix2D(m_CaliComponentStagePosX, m_CaliComponentStagePosY, m_CaliComponentCadPosX, m_CaliComponentCadPosY, 2);
	PanelPtr->AssignPanelMapParamToBoards(DistrictID);
	PanelPtr->CalcPanelStagePosition(DistrictID);
	PanelPtr->LayoutPanelBoardListRegion(DistrictID);
	PanelPtr->CheckPanelMapCoordinate(DistrictID, 1.0);
	//if ( 0 == PanelPtr->GetPanelIndex_Project() )
	//{	ProjectPtr->MapProjectMapStageToCadPos(*STCPtr);	}
	RedrawWindow();
	RedrawProjectImageWnd();
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneAlignPanel::BuildComponentCombox(CComboBox &Combox)
{
	JetAPI::ClearCombox(Combox);
	if ( NULL==m_ProjectPtr ) 
	{	return ; }
	CAOIPanel   *PanelPtr = this->GetActivePanelPtr();
	if ( NULL == PanelPtr )	{	return;	}
	
	size_t  i=0;
	size_t  ID=0;
	int     idx=0;
	CString str;
	unsigned int   BoardIndex=0;
	CAOIComponent *pComponent = NULL;
	const DISTRICT_ID DistrictID = GetDistrictID();
	const size_t ComponentCount = PanelPtr->GetPanelComponentCount();
	
	CSortObj     SortObj;
	std::vector<CSortObj> SortList;

	SortObj.SetSortMode(SORT_BY_TXT);
	for ( i=0; i<ComponentCount; i++ )
	{
		pComponent = PanelPtr->GetPanelComponentPtr(i, false);
		if ( NULL == pComponent ) { continue; }
		if ( DistrictID != pComponent->GetComponentDistrictID() ) { continue; }

		BoardIndex = pComponent->GetComponentBoardIndex_Panel();
		str.Format(_T("%s@%05d"), pComponent->GetComponentName(), BoardIndex+1);//v1.01.01.057
		SortObj.SetID(i);
		SortObj.SetPtr(pComponent);
		SortObj.SetValueStr(str);
		SortList.push_back(SortObj);
	}
	std::sort(SortList.begin(), SortList.end());
	const size_t SortCount = SortList.size();

	Combox.SetRedraw(FALSE);
	//for ( i=0; i<ComponentCount; i++ )
	for ( i=0; i<SortCount; i++ )
	{
		SortObj = SortList[i];
		//pComponent = PanelPtr->GetPanelComponentPtr(i, false);
		//if ( NULL == pComponent ) { continue; }
		//str = pComponent->GetComponentName();
		//ID = i;

		ID = SortObj.GetID();
		str = SortObj.GetValueStr();
		Combox.InsertString(idx, str);
		Combox.SetItemData(idx, ID);	
		idx ++;
	}	
	Combox.SetRedraw(TRUE);
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneAlignPanel::InitialComponentCombox()//分配零件預設使用四個端點
{		
	CAOIPanel   *PanelPtr = this->GetActivePanelPtr();
	if ( NULL == PanelPtr ) 
	{ 
		JetAPI::ShowMessageBox(this->m_ErrorString);
		return; 
	}		
	const size_t InvalideIndex = -1;
	DISTRICT_ID  DistrictID = GetDistrictID();
	CAOIComponent *CpPtr1=NULL, *CpPtr2=NULL, *CpPtr3=NULL, *CpPtr4=NULL;
	PanelPtr->GetPanelCornerComponent(CpPtr1, CpPtr2, CpPtr3, CpPtr4, DistrictID);

	if ( NULL != CpPtr1 )
	{
		if ( m_CaliComponentIdx[0] == InvalideIndex ) 
		{	JetAPI::SetComboxCurSel(m_ComponentCombox1, CpPtr1->GetComponentIndex_Panel());	}
		else
		{	JetAPI::SetComboxCurSel(m_ComponentCombox1, m_CaliComponentIdx[0]);	}
	}
	if ( NULL != CpPtr2 )
	{	
		if ( m_CaliComponentIdx[1] == InvalideIndex ) 
		{	JetAPI::SetComboxCurSel(m_ComponentCombox2, CpPtr2->GetComponentIndex_Panel());	 }
		else
		{	JetAPI::SetComboxCurSel(m_ComponentCombox2, m_CaliComponentIdx[1]);	}
	}
	if ( NULL != CpPtr3 )
	{	
		if ( m_CaliComponentIdx[2] == InvalideIndex ) 
		{	JetAPI::SetComboxCurSel(m_ComponentCombox3, CpPtr3->GetComponentIndex_Panel());	 }
		else
		{	JetAPI::SetComboxCurSel(m_ComponentCombox3, m_CaliComponentIdx[2]);	}
	}
	if ( NULL != CpPtr4 )
	{	
		if ( m_CaliComponentIdx[3] == InvalideIndex ) 
		{	JetAPI::SetComboxCurSel(m_ComponentCombox4, CpPtr4->GetComponentIndex_Panel());	 }
		else
		{	JetAPI::SetComboxCurSel(m_ComponentCombox4, m_CaliComponentIdx[3]);	}
	}
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneAlignPanel::OnAlignComponentGoBtn1() 
{
	// TODO: Add your control notification handler code here
	CAOIPanel   *PanelPtr = this->GetActivePanelPtr();
	if ( NULL == PanelPtr ) 
	{ 
		JetAPI::ShowMessageBox(this->m_ErrorString);
		return; 
	}	
	const unsigned int ComponentIndex = (unsigned int)(JetAPI::GetComboxCurSelData(m_ComponentCombox1));	
	CAOIComponent *pComponent = PanelPtr->GetPanelComponentPtr(ComponentIndex, true);
	if ( NULL == pComponent ) { return; }
	PanelPtr->SelectPanelAllComponents(false);
	pComponent->SetComponentSelected(true);
	this->m_ImageWnd.SetShowImageCenterLine(false);
	this->m_ImageWnd.SetLBtnClickMode(IMAGE_LBTN_CLICK_EDIT_BOX);
	if (true == m_BoxApplyRotation) {
		const double angle = pComponent->GetComponentAngle();
		this->m_ImageWnd.SetImageEditRectAngle(angle);
	}
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();
	MotionCtrlPtr->XYMoveTo(pComponent->GetComponentStagePosX(), pComponent->GetComponentStagePosY(), OfflineMode);
	this->ExecGrabImage();
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneAlignPanel::OnAlignComponentGoBtn2() 
{
	// TODO: Add your control notification handler code here
	CAOIPanel   *PanelPtr = this->GetActivePanelPtr();
	if ( NULL == PanelPtr ) 
	{ 
		JetAPI::ShowMessageBox(this->m_ErrorString);
		return; 
	}
	const unsigned int ComponentIndex = (unsigned int)(JetAPI::GetComboxCurSelData(m_ComponentCombox2));	
	CAOIComponent *pComponent = PanelPtr->GetPanelComponentPtr(ComponentIndex, true);
	if ( NULL == pComponent ) { return; }
	PanelPtr->SelectPanelAllComponents(false);
	pComponent->SetComponentSelected(true);
	this->m_ImageWnd.SetShowImageCenterLine(false);
	this->m_ImageWnd.SetLBtnClickMode(IMAGE_LBTN_CLICK_EDIT_BOX);
	if (true == m_BoxApplyRotation) {
		const double angle = pComponent->GetComponentAngle();
		this->m_ImageWnd.SetImageEditRectAngle(angle);
	}
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();
	MotionCtrlPtr->XYMoveTo(pComponent->GetComponentStagePosX(), pComponent->GetComponentStagePosY(), OfflineMode);
	this->ExecGrabImage();
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneAlignPanel::OnSelchangeAlignComponentCombo3() 
{
	// TODO: Add your control notification handler code here
	this->m_ImageWnd.SetLBtnClickMode(IMAGE_LBTN_CLICK_EDIT_BOX);
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneAlignPanel::OnAlignComponentSetBtn3() 
{
	// TODO: Add your control notification handler code here
	const size_t InvalideIndex = -1;
	if ( m_CaliComponentIdx[1] == InvalideIndex ) { return; }
	CAOIPanel   *PanelPtr = this->GetActivePanelPtr();
	CAOIProject *ProjectPtr = GetActiveProjectPtr();
	if ( NULL==PanelPtr || NULL==ProjectPtr ) 
	{ 
		JetAPI::ShowMessageBox(this->m_ErrorString);
		return; 
	}	
	const DISTRICT_ID DistrictID = ProjectPtr->GetProjectActDistrictID();
	const unsigned int ComponentIndex3 = (unsigned int)(JetAPI::GetComboxCurSelData(m_ComponentCombox3));	
	CAOIComponent *pComponent3 = PanelPtr->GetPanelComponentPtr(ComponentIndex3, true);
	if ( NULL == pComponent3 ) { return; }	

	TRECT4D EditRect;
	CAMERA_ID CameraID = PRIMARY_CAMERA_ID;
	m_ImageWnd.GetImageEditRect(EditRect);
	if ( fabs(EditRect.right-EditRect.left)<0.0001 || fabs(EditRect.bottom-EditRect.top)<0.0001 )
	{	return;	}
	TPOINT2D ImagePt, StageCP, StagePt;
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();
	MotionCtrlPtr->GetCurrentPos(StageCP.x, StageCP.y, OfflineMode);
	ImagePt.x = (EditRect.left+EditRect.right)*0.5;
	ImagePt.y = (EditRect.top+EditRect.bottom)*0.5;
	AOIDataCollect.MapCameraPtToStage(CameraID, ImagePt, StageCP, StagePt);
	m_CaliComponentIdx[2] = ComponentIndex3;
	m_CaliComponentStagePosX[2] = StagePt.x;
	m_CaliComponentStagePosY[2] = StagePt.y;
	m_CaliComponentCadPosX[2] = pComponent3->GetComponentCadPosX();
	m_CaliComponentCadPosY[2] = pComponent3->GetComponentCadPosY();

	CMapCoordinate *CTSPtr = PanelPtr->GetPanelMapCTSPtr(DistrictID);//整板的座標轉換-Cad to Stage
	CMapCoordinate *STCPtr = PanelPtr->GetPanelMapSTCPtr(DistrictID);//整板的座標轉換-Stage to Cad	
	if ( NULL==CTSPtr || NULL==STCPtr ) { return ; }
	CTSPtr->CalcMatrix2D(m_CaliComponentCadPosX, m_CaliComponentCadPosY, m_CaliComponentStagePosX, m_CaliComponentStagePosY, 3);
	STCPtr->CalcMatrix2D(m_CaliComponentStagePosX, m_CaliComponentStagePosY, m_CaliComponentCadPosX, m_CaliComponentCadPosY, 3);
	PanelPtr->AssignPanelMapParamToBoards(DistrictID);
	PanelPtr->CalcPanelStagePosition(DistrictID);
	PanelPtr->LayoutPanelBoardListRegion(DistrictID);
	PanelPtr->CheckPanelMapCoordinate(DistrictID, 1.0);
	//if ( 0 == PanelPtr->GetPanelIndex_Project() )
	//{	ProjectPtr->MapProjectMapStageToCadPos(*STCPtr);	}
	RedrawWindow();
	RedrawProjectImageWnd();
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneAlignPanel::OnAlignComponentGoBtn3() 
{
	// TODO: Add your control notification handler code here
	CAOIPanel   *PanelPtr = this->GetActivePanelPtr();
	if ( NULL == PanelPtr ) 
	{ 
		JetAPI::ShowMessageBox(this->m_ErrorString);
		return; 
	}
	const unsigned int ComponentIndex = (unsigned int)(JetAPI::GetComboxCurSelData(m_ComponentCombox3));	
	CAOIComponent *pComponent = PanelPtr->GetPanelComponentPtr(ComponentIndex, true);
	if ( NULL == pComponent ) { return; }
	PanelPtr->SelectPanelAllComponents(false);
	pComponent->SetComponentSelected(true);
	this->m_ImageWnd.SetShowImageCenterLine(false);
	this->m_ImageWnd.SetLBtnClickMode(IMAGE_LBTN_CLICK_EDIT_BOX);
	if (true == m_BoxApplyRotation) {
		const double angle = pComponent->GetComponentAngle();
		this->m_ImageWnd.SetImageEditRectAngle(angle);
	}
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();
	MotionCtrlPtr->XYMoveTo(pComponent->GetComponentStagePosX(), pComponent->GetComponentStagePosY(), OfflineMode);
	this->ExecGrabImage();
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneAlignPanel::OnAssignComponentBtn() 
{
	// TODO: Add your control notification handler code here
	this->InitialComponentCombox();
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneAlignPanel::OnSelchangeAlignComponentCombo4() 
{
	// TODO: Add your control notification handler code here
	this->m_ImageWnd.SetLBtnClickMode(IMAGE_LBTN_CLICK_EDIT_BOX);
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneAlignPanel::OnAlignComponentSetBtn4() 
{
	// TODO: Add your control notification handler code here
	const size_t InvalideIndex = -1;
	if ( m_CaliComponentIdx[2] == InvalideIndex ) { return; }
	CAOIPanel   *PanelPtr = this->GetActivePanelPtr();
	CAOIProject *ProjectPtr = GetActiveProjectPtr();
	if ( NULL==PanelPtr || NULL==ProjectPtr ) 
	{ 
		JetAPI::ShowMessageBox(this->m_ErrorString);
		return; 
	}	
	const DISTRICT_ID DistrictID = ProjectPtr->GetProjectActDistrictID();
	const unsigned int ComponentIndex4 = (unsigned int)(JetAPI::GetComboxCurSelData(m_ComponentCombox4));	
	CAOIComponent *pComponent4 = PanelPtr->GetPanelComponentPtr(ComponentIndex4, true);
	if ( NULL == pComponent4 ) { return; }	

	TRECT4D EditRect;
	CAMERA_ID CameraID = PRIMARY_CAMERA_ID;
	m_ImageWnd.GetImageEditRect(EditRect);
	if ( fabs(EditRect.right-EditRect.left)<0.0001 || fabs(EditRect.bottom-EditRect.top)<0.0001 )
	{	return;	}
	TPOINT2D ImagePt, StageCP, StagePt;
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();
	MotionCtrlPtr->GetCurrentPos(StageCP.x, StageCP.y, OfflineMode);	
	ImagePt.x = (EditRect.left+EditRect.right)*0.5;
	ImagePt.y = (EditRect.top+EditRect.bottom)*0.5;
	AOIDataCollect.MapCameraPtToStage(CameraID, ImagePt, StageCP, StagePt);
	m_CaliComponentIdx[3] = ComponentIndex4;
	m_CaliComponentStagePosX[3] = StagePt.x;
	m_CaliComponentStagePosY[3] = StagePt.y;
	m_CaliComponentCadPosX[3] = pComponent4->GetComponentCadPosX();
	m_CaliComponentCadPosY[3] = pComponent4->GetComponentCadPosY();

	CMapCoordinate *CTSPtr = PanelPtr->GetPanelMapCTSPtr(DistrictID);//整板的座標轉換-Cad to Stage
	CMapCoordinate *STCPtr = PanelPtr->GetPanelMapSTCPtr(DistrictID);//整板的座標轉換-Stage to Cad		
	if ( NULL==CTSPtr || NULL==STCPtr ) { return ; }
	CTSPtr->CalcMatrix2D(m_CaliComponentCadPosX, m_CaliComponentCadPosY, m_CaliComponentStagePosX, m_CaliComponentStagePosY, 4);
	STCPtr->CalcMatrix2D(m_CaliComponentStagePosX, m_CaliComponentStagePosY, m_CaliComponentCadPosX, m_CaliComponentCadPosY, 4);
	PanelPtr->AssignPanelMapParamToBoards(DistrictID);
	PanelPtr->CalcPanelStagePosition(DistrictID);
	PanelPtr->LayoutPanelBoardListRegion(DistrictID);
	PanelPtr->CheckPanelMapCoordinate(DistrictID, 1.0);
	//if ( 0 == PanelPtr->GetPanelIndex_Project() )
	//{	ProjectPtr->MapProjectMapStageToCadPos(*STCPtr);	}
	RedrawWindow();
	RedrawProjectImageWnd();
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneAlignPanel::OnAlignComponentGoBtn4() 
{
	// TODO: Add your control notification handler code here
	CAOIPanel   *PanelPtr = this->GetActivePanelPtr();
	if ( NULL == PanelPtr ) 
	{ 
		JetAPI::ShowMessageBox(this->m_ErrorString);
		return; 
	}
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();
	const unsigned int ComponentIndex = (unsigned int)(JetAPI::GetComboxCurSelData(m_ComponentCombox4));	
	CAOIComponent *pComponent = PanelPtr->GetPanelComponentPtr(ComponentIndex, true);
	if ( NULL == pComponent ) { return; }
	PanelPtr->SelectPanelAllComponents(false);
	pComponent->SetComponentSelected(true);
	this->m_ImageWnd.SetShowImageCenterLine(false);
	this->m_ImageWnd.SetLBtnClickMode(IMAGE_LBTN_CLICK_EDIT_BOX);	
	if (true == m_BoxApplyRotation) {
		const double angle = pComponent->GetComponentAngle();
		this->m_ImageWnd.SetImageEditRectAngle(angle);
	}
	MotionCtrlPtr->XYMoveTo(pComponent->GetComponentStagePosX(), pComponent->GetComponentStagePosY(), OfflineMode);
	this->ExecGrabImage();
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneAlignPanel::OnPanelEdgePos1GoBtn() 
{
	// TODO: Add your control notification handler code here
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();
	MotionCtrlPtr->XYMoveTo(m_EdgeStagePosX[0], m_EdgeStagePosY[0], OfflineMode);
	this->m_ImageWnd.SetShowImageCenterLine(true);
	CWnd::CheckDlgButton(ALIGNPANEL_SHOW_CENTER_LINE, TRUE);
	this->ExecGrabImage();
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneAlignPanel::OnPanelEdgePos2GoBtn() 
{
	// TODO: Add your control notification handler code here
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();
	MotionCtrlPtr->XYMoveTo(m_EdgeStagePosX[1], m_EdgeStagePosY[1], OfflineMode);
	this->m_ImageWnd.SetShowImageCenterLine(true);
	CWnd::CheckDlgButton(ALIGNPANEL_SHOW_CENTER_LINE, TRUE);
	this->ExecGrabImage();
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneAlignPanel::OnPanelEdgePos1SetBtn() 
{
	// TODO: Add your control notification handler code here
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();
	MotionCtrlPtr->GetCurrentPos(m_EdgeStagePosX[0], m_EdgeStagePosY[0], OfflineMode);
	this->m_ImageWnd.SetShowImageCenterLine(true);
	CWnd::CheckDlgButton(ALIGNPANEL_SHOW_CENTER_LINE, TRUE);
	CWnd::Invalidate(FALSE);	
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneAlignPanel::OnPanelEdgePos2SetBtn() 
{
	// TODO: Add your control notification handler code here
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();
	MotionCtrlPtr->GetCurrentPos(m_EdgeStagePosX[1], m_EdgeStagePosY[1], OfflineMode);
	this->m_ImageWnd.SetShowImageCenterLine(true);	
	CWnd::CheckDlgButton(ALIGNPANEL_SHOW_CENTER_LINE, TRUE);
	CWnd::Invalidate(FALSE);
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneAlignPanel::OnPanelEdgeCalcBtn() 
{
	// TODO: Add your control notification handler code here
	const size_t InvalideIndex = -1;
	if ( m_CaliComponentIdx[0] == InvalideIndex ) { return; }
	DISTRICT_ID DistrictID = GetDistrictID();
	CAOIPanel   *PanelPtr = this->GetActivePanelPtr();
	CAOIProject *ProjectPtr = GetActiveProjectPtr();
	if ( NULL==PanelPtr || NULL==ProjectPtr ) 
	{ 
		JetAPI::ShowMessageBox(this->m_ErrorString);
		return; 
	}

	CString str;	
	double AngleRad=0, AngleRad2=0, AngleDeg=0, AngleDeg2=0;		
	double dPosX = m_EdgeStagePosX[1]-m_EdgeStagePosX[0];
	double dPosY = m_EdgeStagePosY[1]-m_EdgeStagePosY[0];	
	AngleRad = ::atan2(dPosY, dPosX);	
	AngleDeg = AngleRad*RAD_TO_DEG_DBL;		
	if ( fabs(dPosX) > fabs(dPosY) )//Horizontal
	{
		if ( AngleDeg > 90.0 )
		{	AngleDeg2 = AngleDeg-180.0;	}
		else if ( AngleDeg < -90.0 )
		{	AngleDeg2 = AngleDeg+180.0;	}
		else
		{	AngleDeg2 = AngleDeg;	}
		AngleDeg2 = -AngleDeg2;
	}
	else//Vertical
	{
		if ( AngleDeg < 0 )
		{	AngleDeg2 = AngleDeg+90.0;	}
		else
		{	AngleDeg2 = AngleDeg-90.0;	}
		AngleDeg2 = -AngleDeg2;
	}
	//AngleDeg2 = JetAPI::AdjustRotationAngle(AngleDeg);
	AngleRad2 = AngleDeg2*DEG_TO_RAD_DBL;
	str.Format(_T("%.2f degree"), AngleDeg2);
	this->SetDlgItemText(ALIGNPANEL_PANEL_EDGE_CALC_EDIT, str);		
	const double CadPosX = m_CaliComponentCadPosX[0];
	const double CadPosY = m_CaliComponentCadPosY[0];
	const double StagePosX = m_CaliComponentStagePosX[0];
	const double StagePosY = m_CaliComponentStagePosY[0];	
	const bool SignX = AOIDataCollect.GetStageSignPositiveX();
	const bool SignY = AOIDataCollect.GetStageSignPositiveY();
	CMapCoordinate *CTSPtr = PanelPtr->GetPanelMapCTSPtr(DistrictID);//整板的座標轉換-Cad to Stage
	CMapCoordinate *STCPtr = PanelPtr->GetPanelMapSTCPtr(DistrictID);//整板的座標轉換-Stage to Cad	
	if ( NULL==CTSPtr || NULL==STCPtr ) { return ; }

	double CadAngle = AngleDeg2;
	CTSPtr->CalcMatrix2D(CadPosX, CadPosY, StagePosX, StagePosY, CadAngle, 1.0, 1.0, SignX, SignY);//AngleDeg2

	if ( SignX != SignY )//試出來的
	{	CadAngle = CadAngle; }
	else
	{	CadAngle = -CadAngle; }
	STCPtr->CalcMatrix2D(StagePosX, StagePosY, CadPosX, CadPosY, CadAngle, 1.0, 1.0, SignX, SignY);//-AngleDeg2	
	
	PanelPtr->CalcPanelStagePosition(DistrictID);
	PanelPtr->LayoutPanelBoardListRegion(DistrictID);
	PanelPtr->CheckPanelMapCoordinate(DistrictID, 1000.0);
	//if ( 0 == PanelPtr->GetPanelIndex_Project() )
	//{	ProjectPtr->MapProjectMapStageToCadPos(*STCPtr);	}

	//m_ImageWnd.SetLBtnClickMode(IMAGE_LBTN_CLICK_MOVE_PANEL_STAGE);	//IMAGE_LBTN_CLICK_MOVE_PANEL
	CWnd::Invalidate(FALSE);
	//RedrawWindow();
	//RedrawProjectImageWnd();
}
//-------------------------------------------------------------------------------------//
BOOL CNewProjectPaneAlignPanel::RedrawProjectImageWnd()//重繪專案影像視窗
{
	PostParentWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_REDRAW_PROJECT_MAP, NULL);
	return TRUE;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneAlignPanel::SendParentWndMessage(UINT message, WPARAM wParam, LPARAM lParam)//發送訊息給父視窗
{
	HWND hWnd = CWnd::GetSafeHwnd();
	AOIDataCollect.SendParentWndMessage(hWnd, message, wParam, lParam);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneAlignPanel::PostParentWndMessage(UINT message, WPARAM wParam, LPARAM lParam)//發送訊息給父視窗
{
	HWND hWnd = CWnd::GetSafeHwnd();
	AOIDataCollect.PostParentWndMessage(hWnd, message, wParam, lParam);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneAlignPanel::ExecNextPane()
{	
	CAOIPanel   *PanelPtr = this->GetActivePanelPtr();
	CAOIProject *ProjectPtr = this->m_ProjectPtr;
	if ( NULL == PanelPtr ) 
	{ 
		JetAPI::ShowMessageBox(this->m_ErrorString);
		return false; 
	}
	DISTRICT_ID    DistrictID = GetDistrictID();
	CMapCoordinate *CTSPtr = PanelPtr->GetPanelMapCTSPtr(DistrictID);//整板的座標轉換-Cad to Stage
	CMapCoordinate *STCPtr = PanelPtr->GetPanelMapSTCPtr(DistrictID);//整板的座標轉換-Stage to Cad		
	PanelPtr->SetPanelComponentOrgCadPos();
	PanelPtr->UpdatePanelComponentToModel();
	PanelPtr->LayoutPanelRegion(DistrictID);	

	TPOINT2D  MapRes;
	TREGION4D CadRgn, StageRgn;
	TREGION4D PanelCadRgn   = PanelPtr->GetPanelRgnCad();
	TREGION4D PanelStageRgn = PanelPtr->GetPanelRgnStage(DistrictID);	
	ProjectPtr->GetProjectMapInfo(MapRes, CadRgn, StageRgn);
	/*
	if ( 0 == PanelPtr->GetPanelIndex_Project() )
	{	ProjectPtr->MapProjectMapStageToCadPos(*STCPtr);	}
	*/
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneAlignPanel::ExecPrevPane()
{
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneAlignPanel::ExecFinishPane()
{
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneAlignPanel::ReInitialPane()
{
	return true;
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneAlignPanel::OnShowCenterLine() 
{
	// TODO: Add your control notification handler code here
	BOOL bCheck = CWnd::IsDlgButtonChecked(ALIGNPANEL_SHOW_CENTER_LINE);
	this->m_ImageWnd.SetShowImageCenterLine((bool)bCheck);
	CWnd::Invalidate(FALSE);
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneAlignPanel::ChangeDistrictID(DISTRICT_ID DistrictID)
{	
	CString      str;
	CAOIProject *ProjectPtr = GetActiveProjectPtr();
	if ( NULL == ProjectPtr ) { return false; }

	const bool OfflineMode = AOIDataCollect.GetOfflineMode();
	const LANE_ID ActLaneID = AOIDataCollect.GetActiveLaneID();
	const DISTRICT_ID ActDistrictID = AOIDataCollect.GetActiveDistrictID();	
	if ( ActDistrictID != DistrictID )
	{
		if ( AOIDataCollect.MovePCBToDistrictID(ActLaneID, DistrictID) == false )
		{
			str = AOIDataCollect.GetErrorString();
			JetAPI::ShowMessageBox(str);
			return false;
		}
	}
	m_DistrictID = DistrictID;
	str = AOIDataDefine.GetDistrictIDText(DistrictID);
	CWnd::SetDlgItemText(ALIGNPANEL_DISTRICT_EDIT, str);
	m_ImageWnd.SetImageText(str, true);
	AOIDataCollect.SetActiveDistrictID(DistrictID);
	ProjectPtr->SetProjectActDistrictID(DistrictID, true);

	const size_t InvalideIndex = -1;
	m_CaliComponentIdx[0] = InvalideIndex;
	m_CaliComponentIdx[1] = InvalideIndex;
	m_CaliComponentIdx[2] = InvalideIndex;
	m_CaliComponentIdx[3] = InvalideIndex;
	BuildComponentCombox(m_ComponentCombox1);
	BuildComponentCombox(m_ComponentCombox2);
	BuildComponentCombox(m_ComponentCombox3);
	BuildComponentCombox(m_ComponentCombox4);
	
	CString OfflineFolder = ProjectPtr->GetProjectProgramOfflineFolder();
	CString OfflineFilename = AOIDataDefine.GetProjectOfflineFileName(OfflineFolder, DistrictID);
	if ( true == OfflineMode ) 
	{
		if ( ActDistrictID != DistrictID )
		{	ProjectPtr->LoadProjectProgramOfflineFile(OfflineFilename);	 }
	}

	InitialComponentCombox();	
	m_ImageWnd.SetProjectPtr(ProjectPtr);
	m_ImageWnd.SetShowLBtnPos(false);
	m_ImageWnd.SetLBtnClickMode(IMAGE_LBTN_CLICK_MOVE_PANEL_STAGE);	//IMAGE_LBTN_CLICK_MOVE_PANEL
	RedrawWindow();
	SendParentWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_PROJECT_MAP, NULL);	
	return true;
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneAlignPanel::OnDistrictABtn() 
{
	// TODO: Add your control notification handler code here	
	return;
	ChangeDistrictID(DISTRICT_ID_A);
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneAlignPanel::OnDistrictBBtn() 
{
	// TODO: Add your control notification handler code here
	return;
	ChangeDistrictID(DISTRICT_ID_B);
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneAlignPanel::OnApplyRotation()
{
	// TODO: Add your control notification handler code here
	m_BoxApplyRotation = CWnd::IsDlgButtonChecked(ALIGNPANEL_APPLY_ROTATION);
	return;
}
//-------------------------------------------------------------------------------------//