// ProjectMapPane.cpp : implementation file
//

#include "stdafx.h"
#include "jet8000.h"
#include "ProjectMapPane.h"
//-------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CProjectMapPane dialog
//-------------------------------------------------------------------------------------//
CProjectMapPane::CProjectMapPane(CWnd* pParent /*=NULL*/)
	: CDialog(CProjectMapPane::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProjectMapPane)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ProjectPtr = NULL;
}
//-------------------------------------------------------------------------------------//
void CProjectMapPane::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProjectMapPane)
	DDX_Control(pDX, PRGMAP_FUNC_COMBO, m_FuncCombox);
	DDX_Control(pDX, PRGMAP_IMAGE_WND, m_ImageWnd);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CProjectMapPane, CDialog)
	//{{AFX_MSG_MAP(CProjectMapPane)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_BN_CLICKED(PRGMAP_SHOW_FD_CHK, OnShowFdChk)
	ON_BN_CLICKED(PRGMAP_SHOW_PANEL_CHK, OnShowPanelChk)
	ON_BN_CLICKED(PRGMAP_SHOW_BOARD_CHK, OnShowBoardChk)
	ON_BN_CLICKED(PRGMAP_SHOW_BARCODE_CHK, OnShowBarcodeChk)
	ON_BN_CLICKED(PRGMAP_SHOW_COMPONENT_CHK, OnShowComponentChk)
	ON_BN_CLICKED(PRGMAP_SHOW_CAMERA_CHK, OnShowCameraChk)
	ON_BN_CLICKED(PRGMAP_SHOW_ALL_BTN, OnShowAllBtn)
	ON_BN_CLICKED(PRGMAP_SHOW_NONE_BTN, OnShowNoneBtn)	
	ON_CBN_SELCHANGE(PRGMAP_FUNC_COMBO, OnSelchangeFuncCombo)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CProjectMapPane message handlers
//-------------------------------------------------------------------------------------//
BOOL CProjectMapPane::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_ImageWnd.SetShowLBtnPos(false);
	m_ImageWnd.SetShowBoard(true);	
	m_ImageWnd.SetShowSystem(true);
	m_ImageWnd.SetShowCameraRgn(true);
	m_ImageWnd.SetShowDistrictRect(true);
	m_ImageWnd.SetShowComponentName(false);	
	m_ImageWnd.SetRBtnClickMode(IMAGE_RBTN_CLICK_CTRL_VIEW);
	m_ImageWnd.SetLBtnClickMode(IMAGE_LBTN_CLICK_NULL);
	m_ImageWnd.SetLBtnDbClickMode(IMAGE_LBTN_DBCLICK_STAGE_MOVE_TO);
	m_ImageWnd.BuildSystemRegion();
	SwitchMultiLanguage();
	BuildFuncComboxWnd();

	RECT Rect={0};
	CWnd::GetClientRect(&Rect);
	const int cx = Rect.right-Rect.left;
	const int cy = Rect.bottom-Rect.top;
	AdjustContrlWnd(SIZE_RESTORED, cx, cy);	
	UpdateParamToUI();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CProjectMapPane::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CProjectMapPane::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	AdjustContrlWnd(nType, cx, cy);	
}
//-------------------------------------------------------------------------------------//
void CProjectMapPane::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
LRESULT CProjectMapPane::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	CWnd *pWnd = NULL;
	switch ( message )
	{
	case MSG_MAIN_FRAME_MESSAGE:
		switch ( wParam )
		{
		case WPARAM_PROJECT_NEW:
		case WPARAM_PROJECT_OPEN:
		case WPARAM_PROJECT_SWITCH:		
		case WPARAM_PROJECT_MODIFY_MAP:			
			BuildProjectMapWnd();			
			break;
		case WPARAM_PROJECT_SWITCH_MAP:
		case WPARAM_PROJECT_SWITCH_LANE:
			SwitchProjectMapWnd();
			break;
		case WPARAM_PROJECT_UPDATE:			
			if ( CWnd::IsWindowVisible() == TRUE )
			{
				pWnd = (CWnd*)lParam;
				if ( pWnd != this )
				{	CWnd::Invalidate();	}
			}
			break;
		case WPARAM_PROJECT_UPDATE_FD_ALIGN:
			UpdateProjectMapWnd();
			if ( CWnd::IsWindowVisible() == TRUE )
			{
				pWnd = (CWnd*)lParam;
				if ( pWnd != this )
				{	CWnd::Invalidate();	}
			}
			break;		
		case WPARAM_PROJECT_CLOSE:		
			ClearProjectMapWnd();
			break;		
		case WPARAM_PROJECT_PART_DELETED:
			break;
		case WPARAM_PROJECT_CLOSE_ACTIVE_OBJ:			
			break;
		}
		break;	
	case MSG_INSPECTION_CALLBACK:
		if ( wParam == WPARAM_INSPECTION_FINISH )
		{
			UpdateProjectMapWnd();
			if ( CWnd::IsWindowVisible() == TRUE )
			{
				pWnd = (CWnd*)lParam;
				if ( pWnd != this )
				{	CWnd::Invalidate();	}
			}
		}		
		break;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void CProjectMapPane::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_PROJECT_MAP_PANE");
	//---------------------------------------------------------------------------------//
	WndID = IDD_PROJECT_MAP_PANE;
	WndKey = _T("IDD_PROJECT_MAP_PANE");
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
	WndID = PRGMAP_SHOW_FD_CHK;
	WndKey = _T("PRGMAP_SHOW_FD_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PRGMAP_SHOW_PANEL_CHK;
	WndKey = _T("PRGMAP_SHOW_PANEL_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PRGMAP_SHOW_BOARD_CHK;
	WndKey = _T("PRGMAP_SHOW_BOARD_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PRGMAP_SHOW_BARCODE_CHK;
	WndKey = _T("PRGMAP_SHOW_BARCODE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PRGMAP_SHOW_COMPONENT_CHK;
	WndKey = _T("PRGMAP_SHOW_COMPONENT_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PRGMAP_SHOW_CAMERA_CHK;
	WndKey = _T("PRGMAP_SHOW_CAMERA_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PRGMAP_SHOW_ALL_BTN;
	WndKey = _T("PRGMAP_SHOW_ALL_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PRGMAP_SHOW_NONE_BTN;
	WndKey = _T("PRGMAP_SHOW_NONE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = PRGMAP_FUNC_LABEL;
	WndKey = _T("PRGMAP_FUNC_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//PRGMAP_FUNC_COMBOX
	//---------------------------------------------------------------------------------//
}
//-------------------------------------------------------------------------------------//
CString CProjectMapPane::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_PROJECT_MAP_PANE");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
void CProjectMapPane::AdjustContrlWnd(UINT nType, int cx, int cy)
{
	if ( CWnd::GetSafeHwnd() == NULL ) { return; }

	if ( this->m_ImageWnd.GetSafeHwnd() != NULL )
	{
		RECT  WndRect={0};
		this->m_ImageWnd.GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);		
		WndRect.right = cx;
		WndRect.bottom = cy;
		this->m_ImageWnd.MoveWindow(&WndRect);
		this->m_ImageWnd.ShowFittedZoom();
	}
}
//-------------------------------------------------------------------------------------//
bool CProjectMapPane::BuildFuncComboxWnd()
{
	size_t       i=0;
	int          idx=0;
	CString      str;
	DWORD        Param=0;		
	CComboBox &Combox = m_FuncCombox;

	idx = 0;
	JetAPI::ClearCombox(Combox);	

	str = _T("Func_None");	
	str = LoadMultiLanguageString(str, str);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, IMAGE_LBTN_CLICK_NULL);
	idx ++;

	str = _T("Func_Measure");
	str = LoadMultiLanguageString(str, str);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, IMAGE_LBTN_CLICK_MEASURE);
	idx ++;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectMapPane::BuildProjectMapWnd()
{
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveTaskProject();	
	PROJECT_TASK_MODE ProjectTaskMode = AOIDataCollect.GetProjectTaskMode();	
	if ( NULL==ProjectPtr || PROJECT_TASK_OPEN_BARCODE==ProjectTaskMode ) 
	{		
		this->ClearProjectMapWnd();		
		return true; 
	}	
	
	TPOINT2D   ImageRes;	
	IMAGE_PTR  ImagePtr=NULL;
	CAMERA_ID  CameraID = PRIMARY_CAMERA_ID;
	TREGION4D  RgnCad, RgnStage;
	IMAGE_SIZE ImageW=0, ImageH=0, ImageStep=0, BitCount=0;
	LANE_ID LaneID = ProjectPtr->GetProjectActLaneID();
	unsigned int MapIndex = ProjectPtr->GetProjectMapIndex();
	
	m_ProjectPtr = ProjectPtr;
	m_ProjectPtr->GetProjectMapInfo(ImageRes, RgnCad, RgnStage);
	m_ProjectPtr->GetProjectMapCalcRgn(RgnStage);
	m_ProjectPtr->GetProjectMapPtr(MapIndex, ImageW, ImageH, ImageStep, BitCount, ImagePtr);
	ProjectPtr->CreateProjectMapShowPtr(MapIndex, ImagePtr, false);		

	m_ImageWnd.SetProjectPtr(m_ProjectPtr);
	m_ImageWnd.SetImageInfo(CameraID, RgnStage, ImageRes, IMAGE_DATA_MAP);
	m_ImageWnd.SetImageBuffer(ImageW, ImageH, ImageStep, BitCount, ImagePtr, false, true, true);
	m_ImageWnd.ShowFittedZoom();
	m_ImageWnd.RedrawWnd(FALSE);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectMapPane::SwitchProjectMapWnd()
{
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveTaskProject();	
	if ( NULL == ProjectPtr ) 
	{		
		this->ClearProjectMapWnd();		
		return true; 
	}	
	
	TPOINT2D   ImageRes;
	CAMERA_ID  CameraID = PRIMARY_CAMERA_ID;
	TREGION4D  RgnCad, RgnStage;	
	LANE_ID LaneID = ProjectPtr->GetProjectActLaneID();
	unsigned int MapIndex = ProjectPtr->GetProjectMapIndex();
	
	m_ProjectPtr = ProjectPtr;
	m_ProjectPtr->GetProjectMapInfo(ImageRes, RgnCad, RgnStage);
	m_ProjectPtr->GetProjectMapCalcRgn(RgnStage);
	
	m_ImageWnd.SetImageInfo(CameraID, RgnStage, ImageRes, IMAGE_DATA_MAP);	
	m_ImageWnd.RedrawWnd(FALSE);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectMapPane::UpdateProjectMapWnd()
{
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveTaskProject();	
	if ( NULL == ProjectPtr ) 
	{		
		this->ClearProjectMapWnd();		
		return true; 
	}
	if ( m_ProjectPtr != ProjectPtr ) 
	{	return BuildProjectMapWnd();	}
	
	if ( UpdateProjectMapWndRegion() == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool  CProjectMapPane::UpdateProjectMapWndRegion()
{
	CAOIProject *ProjectPtr = m_ProjectPtr;
	if ( NULL == ProjectPtr ) 
	{	return true;	}

	TPOINT2D   ImageRes;
	CAMERA_ID  CameraID = PRIMARY_CAMERA_ID;
	TREGION4D  RgnCad, RgnStage;	
	LANE_ID    LaneID = ProjectPtr->GetProjectActLaneID();
	ProjectPtr->GetProjectMapInfo(ImageRes, RgnCad, RgnStage);
	ProjectPtr->GetProjectMapCalcRgn(RgnStage);	
	m_ImageWnd.SetImageInfo(CameraID, RgnStage, ImageRes, IMAGE_DATA_MAP);	
	//this->m_ImageWnd.RedrawWnd(FALSE);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectMapPane::ClearProjectMapWnd()
{
	m_ProjectPtr = NULL;
	m_ImageWnd.SetProjectPtr(NULL);
	m_ImageWnd.ReleaseImageBuffer();
	m_ImageWnd.BuildSystemRegion();
	m_ImageWnd.RedrawWnd(true);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectMapPane::UpdateParamToUI()
{
	DWORD val=0;
	UINT CtrlID=0;	
	bool bShow = true;

	CtrlID = PRGMAP_SHOW_FD_CHK;
	CWnd::CheckDlgButton(CtrlID, m_ImageWnd.GetShowFiducial());

	CtrlID = PRGMAP_SHOW_PANEL_CHK;
	CWnd::CheckDlgButton(CtrlID, m_ImageWnd.GetShowPanel());

	CtrlID = PRGMAP_SHOW_BOARD_CHK;
	CWnd::CheckDlgButton(CtrlID, m_ImageWnd.GetShowBoard());
	
	CtrlID = PRGMAP_SHOW_BARCODE_CHK;
	CWnd::CheckDlgButton(CtrlID, m_ImageWnd.GetShowBarcode());

	CtrlID = PRGMAP_SHOW_COMPONENT_CHK;
	CWnd::CheckDlgButton(CtrlID, m_ImageWnd.GetShowComponent());

	CtrlID = PRGMAP_SHOW_CAMERA_CHK;
	CWnd::CheckDlgButton(CtrlID, m_ImageWnd.GetShowCameraRgn());

	val = m_ImageWnd.GetLBtnClickMode();
	switch ( val )
	{		
	case IMAGE_LBTN_CLICK_MEASURE:
		JetAPI::SetComboxCurSel(m_FuncCombox, val);
		break;
	case IMAGE_LBTN_CLICK_NULL:
	default:
		JetAPI::SetComboxCurSel(m_FuncCombox, IMAGE_LBTN_CLICK_NULL);
		break;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CProjectMapPane::SetShowMark(bool bShow)//顯示專案特徵點
{
	m_ImageWnd.SetShowMark(bShow);
}
//-------------------------------------------------------------------------------------//
void CProjectMapPane::SetShowFiducial(bool bShow)//顯示專案定位點
{
	m_ImageWnd.SetShowFiducial(bShow);
	if ( CWnd::GetSafeHwnd() != NULL )
	{	CWnd::CheckDlgButton(PRGMAP_SHOW_FD_CHK, bShow);	}
}
//-------------------------------------------------------------------------------------//
void CProjectMapPane::SetShowBarcode(bool bShow)//顯示專案條碼
{
	m_ImageWnd.SetShowBarcode(bShow);
	if ( CWnd::GetSafeHwnd() != NULL )
	{	CWnd::CheckDlgButton(PRGMAP_SHOW_BARCODE_CHK, bShow);	}
}
//-------------------------------------------------------------------------------------//
void CProjectMapPane::SetShowComponent(bool bShow)//顯示專案零件
{
	m_ImageWnd.SetShowComponent(bShow);
	if ( CWnd::GetSafeHwnd() != NULL )
	{	CWnd::CheckDlgButton(PRGMAP_SHOW_COMPONENT_CHK, bShow);	}	
}
//-------------------------------------------------------------------------------------//
void CProjectMapPane::SetShowComponentName(bool bShow)//顯示專案零件名稱
{
	m_ImageWnd.SetShowComponentName(bShow);
}
//-------------------------------------------------------------------------------------//
void CProjectMapPane::OnShowFdChk() 
{
	// TODO: Add your control notification handler code here
	BOOL bChk = CWnd::IsDlgButtonChecked(PRGMAP_SHOW_FD_CHK);
	if ( TRUE == bChk ) { m_ImageWnd.SetShowFiducial(true); }
	else { m_ImageWnd.SetShowFiducial(false); }
	m_ImageWnd.RedrawWnd(FALSE);
}
//-------------------------------------------------------------------------------------//
void CProjectMapPane::OnShowPanelChk() 
{
	// TODO: Add your control notification handler code here
	BOOL bChk = CWnd::IsDlgButtonChecked(PRGMAP_SHOW_PANEL_CHK);
	if ( TRUE == bChk ) { m_ImageWnd.SetShowPanel(true); }
	else { m_ImageWnd.SetShowPanel(false); }
	m_ImageWnd.RedrawWnd(FALSE);	
}
//-------------------------------------------------------------------------------------//
void CProjectMapPane::OnShowBoardChk() 
{
	// TODO: Add your control notification handler code here
	BOOL bChk = CWnd::IsDlgButtonChecked(PRGMAP_SHOW_BOARD_CHK);
	if ( TRUE == bChk ) { m_ImageWnd.SetShowBoard(true); }
	else { m_ImageWnd.SetShowBoard(false); }
	m_ImageWnd.RedrawWnd(FALSE);
}
//-------------------------------------------------------------------------------------//
void CProjectMapPane::OnShowBarcodeChk() 
{
	// TODO: Add your control notification handler code here
	BOOL bChk = CWnd::IsDlgButtonChecked(PRGMAP_SHOW_BARCODE_CHK);
	if ( TRUE == bChk ) { m_ImageWnd.SetShowBarcode(true); }
	else { m_ImageWnd.SetShowBarcode(false); }
	m_ImageWnd.RedrawWnd(FALSE);
}
//-------------------------------------------------------------------------------------//
void CProjectMapPane::OnShowComponentChk() 
{
	// TODO: Add your control notification handler code here
	BOOL bChk = CWnd::IsDlgButtonChecked(PRGMAP_SHOW_COMPONENT_CHK);
	if ( TRUE == bChk ) { m_ImageWnd.SetShowComponent(true); }
	else { m_ImageWnd.SetShowComponent(false); }
	m_ImageWnd.RedrawWnd(FALSE);
}
//-------------------------------------------------------------------------------------//
void CProjectMapPane::OnShowCameraChk() 
{
	// TODO: Add your control notification handler code here
	BOOL bChk = CWnd::IsDlgButtonChecked(PRGMAP_SHOW_CAMERA_CHK);
	if ( TRUE == bChk ) { m_ImageWnd.SetShowCameraRgn(true); }
	else { m_ImageWnd.SetShowCameraRgn(false); }
	m_ImageWnd.RedrawWnd(FALSE);
}
//-------------------------------------------------------------------------------------//
void CProjectMapPane::OnShowAllBtn() 
{
	// TODO: Add your control notification handler code here
	m_ImageWnd.SetShowAll(true);
	UpdateParamToUI();
	m_ImageWnd.RedrawWnd(FALSE);
}
//-------------------------------------------------------------------------------------//
void CProjectMapPane::OnShowNoneBtn() 
{
	// TODO: Add your control notification handler code here
	m_ImageWnd.SetShowAll(false);
	UpdateParamToUI();
	m_ImageWnd.RedrawWnd(FALSE);
}
//-------------------------------------------------------------------------------------//
void CProjectMapPane::OnSelchangeFuncCombo() 
{
	// TODO: Add your control notification handler code here
	CComboBox &Combox = m_FuncCombox;
	DWORD_PTR dwData = JetAPI::GetComboxCurSelData(Combox);
	switch ( dwData )
	{
	case IMAGE_LBTN_CLICK_MEASURE:
		m_ImageWnd.SetLBtnClickMode(IMAGE_LBTN_CLICK_MEASURE);
		break;
	case IMAGE_LBTN_CLICK_NULL:		
	default:		
		m_ImageWnd.SetLBtnClickMode(IMAGE_LBTN_CLICK_NULL);
		break;
	}
	return;
}
//-------------------------------------------------------------------------------------//