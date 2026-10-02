// ProjectImageWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "ProjectMapWnd.h"
//-------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CProjectMapWnd dialog
//-------------------------------------------------------------------------------------//
CProjectMapWnd::CProjectMapWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CProjectMapWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProjectMapWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ProjectPtr = NULL;
}
//-------------------------------------------------------------------------------------//
void CProjectMapWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProjectMapWnd)
	DDX_Control(pDX, PRGIMAGE_IMAGE_WND, m_ImageWnd);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CProjectMapWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CProjectMapWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	ON_WM_GETMINMAXINFO()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_COMMAND(MENU_PROJECT_MAP_SHOW_FD, OnShowFd)
	ON_COMMAND(MENU_PROJECT_MAP_SHOW_PANEL, OnShowPanel)
	ON_COMMAND(MENU_PROJECT_MAP_SHOW_BOARD, OnShowBoard)
	ON_COMMAND(MENU_PROJECT_MAP_SHOW_BARCODE, OnShowBarcode)
	ON_COMMAND(MENU_PROJECT_MAP_SHOW_COMPONENT, OnShowComponent)	
	ON_COMMAND(MENU_PROJECT_MAP_SHOW_CAMERA, OnShowCamera)	
	ON_COMMAND(MENU_PROJECT_MAP_SHOW_ALL, OnShowAll)		
	ON_COMMAND(MENU_PROJECT_MAP_SHOW_NONE, OnShowNone)
	ON_COMMAND(MENU_PROJECT_MAP_FUNC_NONE, OnFuncNone)		
	ON_COMMAND(MENU_PROJECT_MAP_FUNC_MEASURE, OnFuncMeasure)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CProjectMapWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CProjectMapWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
//	m_ImageWnd.SetProjectPtr(this->m_ProjectPtr);		
	m_ImageWnd.SetShowLBtnPos(false);
	m_ImageWnd.SetShowCameraPos(true);
	m_ImageWnd.SetShowCameraRgn(true);
	m_ImageWnd.SetShowBoard(true);
	m_ImageWnd.SetShowSystem(true);
	m_ImageWnd.SetShowDistrictRect(true);
	m_ImageWnd.SetShowComponentName(false);	
	m_ImageWnd.SetRBtnClickMode(IMAGE_RBTN_CLICK_CTRL_VIEW);
	m_ImageWnd.SetLBtnClickMode(IMAGE_LBTN_CLICK_NULL);	
	m_ImageWnd.SetShowFieldRgn(true);
	m_ImageWnd.BuildSystemRegion();
	SwitchMultiLanguage();
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	
	RECT Rect={0};
	CWnd::GetClientRect(&Rect);
	if ( m_ImageWnd.GetSafeHwnd() != NULL )
	{
		RECT  WndRect={0};
		this->m_ImageWnd.GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		WndRect.left = Rect.left;
		WndRect.top = Rect.top;
		WndRect.right = Rect.right;
		WndRect.bottom = Rect.bottom;
		this->m_ImageWnd.MoveWindow(&WndRect);
		this->m_ImageWnd.ShowFittedZoom();
	}	

	CMenu *pMenu = GetMenu();
	if ( NULL != pMenu )
	{	AOIDataCollect.SwitchMultiLanguageMenu(*pMenu, IDR_MENU_PROJECT_MAP); }
	UpdateCommandUI();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CProjectMapWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here	
}
//-------------------------------------------------------------------------------------//
void CProjectMapWnd::OnSize(UINT nType, int cx, int cy) 
{
	CBaseDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
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
void CProjectMapWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CBaseDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CProjectMapWnd::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_PROJECT_MAP_WND");
	//---------------------------------------------------------------------------------//
	WndID = IDD_PROJECT_MAP_WND;
	WndKey = _T("IDD_PROJECT_MAP_WND");
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
}
//-------------------------------------------------------------------------------------//
void CProjectMapWnd::OnOK() 
{
	// TODO: Add extra validation here
	return ;
	CBaseDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CProjectMapWnd::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CBaseDialog::OnCancel();
}
//-------------------------------------------------------------------------------------//
CAOIProject* CProjectMapWnd::GetProjectPtr() const
{
	return CProjectMapWnd::m_ProjectPtr;
}
//-------------------------------------------------------------------------------------//
void CProjectMapWnd::SetProjectPtr(CAOIProject *ProjectPtr, bool bForce)//設定專案指標
{
	m_ProjectPtr = ProjectPtr;
	if ( NULL == m_ProjectPtr ) 
	{			
		m_ImageWnd.SetProjectPtr(NULL);
		m_ImageWnd.ReleaseImageBuffer();
		return; 
	}
	
	CString    FrameName;
	TPOINT2D   ImageRes;	
	IMAGE_PTR  ImagePtr=NULL;
	CAMERA_ID  CameraID = PRIMARY_CAMERA_ID;
	TREGION4D  RgnCad, RgnStage;
	IMAGE_SIZE ImageW=0, ImageH=0, ImageStep=0, BitCount=0;
	LANE_ID    LaneID = ProjectPtr->GetProjectActLaneID();
	unsigned int MapIndex = ProjectPtr->GetProjectMapIndex();
	unsigned int FrameUniqueID = ProjectPtr->GetProjectFrameUniqueID(MapIndex, true);
	TFrameParam *FrameParamPtr = AOIDataCollect.GetSystemFrameParamPtrByUniqueID(FrameUniqueID);
	if ( NULL != FrameParamPtr )
	{	FrameName = FrameParamPtr->FrameName;	}
	ProjectPtr->GetProjectMapInfo(ImageRes, RgnCad, RgnStage);
	ProjectPtr->GetProjectMapCalcRgn(RgnStage);
	ProjectPtr->GetProjectMapPtr(MapIndex, ImageW, ImageH, ImageStep, BitCount, ImagePtr);	
	ProjectPtr->CreateProjectMapShowPtr(MapIndex, ImagePtr, bForce);

	m_ImageWnd.SetProjectPtr(ProjectPtr);
	m_ImageWnd.SetImageText(FrameName, true);
	m_ImageWnd.SetImageInfo(CameraID, RgnStage, ImageRes, IMAGE_DATA_MAP);
	m_ImageWnd.SetImageBuffer(ImageW, ImageH, ImageStep, BitCount, ImagePtr, false, true, true);
	//m_ImageWnd.ShowFittedZoom();
	//m_ImageWnd.RedrawWnd(FALSE);	
	const bool bFitWndSizeToMap = true;
	if ( true==bFitWndSizeToMap && 0!=ImageW && 0!=ImageH )
	{
		int   WndW=0, WndH=0;
		int   NewWndW=0, NewWndH=0;
		int   ClientW=0, ClientH=0;
		int   NewClientW=0, NewClientH=0;
		int   WndHeadSizeH=0, WndHeadSizeW=0;
		double Scale=1.0;
		double ScaleX=1.0;
		double ScaleY=1.0;
		RECT  WndRect={0};
		RECT  ClientRect={0};

		CWnd::GetWindowRect(&WndRect);
		CWnd::GetClientRect(&ClientRect);
		JetAPI::GetRectSize(WndRect, WndW, WndH);
		JetAPI::GetRectSize(ClientRect, ClientW, ClientH);
		WndHeadSizeW = WndW-ClientW;
		WndHeadSizeH = WndH-ClientH;

		ScaleX = ClientW*1.0/ImageW;
		ScaleY = ClientH*1.0/ImageH;
		NewClientW = (int)(ScaleY*ImageW);
		NewClientH = (int)(ScaleX*ImageH);
		if ( NewClientW > ClientW )
		{	NewClientW = ClientW;	}
		if ( NewClientH > ClientH )
		{	NewClientH = ClientH;	}
		NewWndW = NewClientW+WndHeadSizeW;
		NewWndH = NewClientH+WndHeadSizeH;
		MoveWindow(WndRect.left, WndRect.top, NewWndW, NewWndH, FALSE);
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CProjectMapWnd::SetDrawProjectMode(IMAGE_WND_DRAW_PROJECT_MODE Mode)
{
	m_ImageWnd.SetDrawProjectMode(Mode);
	m_ImageWnd.RedrawWnd(FALSE);
}
//-------------------------------------------------------------------------------------//
void CProjectMapWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CBaseDialog::OnPaint() for painting messages
}
//-------------------------------------------------------------------------------------//
void CProjectMapWnd::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default	
	CBaseDialog::OnGetMinMaxInfo(lpMMI);
	lpMMI->ptMinTrackSize.x = 300;
	lpMMI->ptMinTrackSize.y = 200;
}
//-------------------------------------------------------------------------------------//
LRESULT CProjectMapWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	CWnd *pWnd = NULL;
	switch ( message )
	{
	case MSG_IMAGE_WND_DRAW_NEXT:
		break;
	case MSG_MAIN_FRAME_MESSAGE:
		switch ( wParam )
		{
		case WPARAM_PROJECT_NEW:
		case WPARAM_PROJECT_OPEN:
		case WPARAM_PROJECT_SWITCH:		
			BuildProjectMapWnd();
			break;
		case WPARAM_PROJECT_UPDATE:
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
	}
	return CBaseDialog::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
CImageWnd* CProjectMapWnd::GetProjectImageWnd()
{
	return &(m_ImageWnd);
}
//-------------------------------------------------------------------------------------//
void CProjectMapWnd::RedrawWnd(BOOL bRedrawBK)//重繪視窗
{
	this->m_ImageWnd.RedrawWnd(bRedrawBK);
}
//-------------------------------------------------------------------------------------//
void CProjectMapWnd::BuildProjectMapWnd()
{
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	PROJECT_TASK_MODE ProjectTaskMode = AOIDataCollect.GetProjectTaskMode();
	if ( PROJECT_TASK_OPEN_BARCODE == ProjectTaskMode ) 
	{	ProjectPtr = NULL;	}
	SetProjectPtr(ProjectPtr, true);
}
//-------------------------------------------------------------------------------------//
void CProjectMapWnd::UpdateProjectMapWnd()
{
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL == ProjectPtr )
	{
		ClearProjectMapWnd();
		return;
	}	
	if ( m_ProjectPtr != ProjectPtr ) 
	{		
		SetProjectPtr(ProjectPtr, true);		
		return; 
	}
	
	TPOINT2D   ImageRes;		
	CAMERA_ID  CameraID = PRIMARY_CAMERA_ID;
	TREGION4D  RgnCad, RgnStage;	
	LANE_ID    LaneID = m_ProjectPtr->GetProjectActLaneID();
	m_ProjectPtr->GetProjectMapInfo(ImageRes, RgnCad, RgnStage);	
	m_ProjectPtr->GetProjectMapTeachRgn(RgnStage);	

	this->m_ImageWnd.SetImageInfo(CameraID, RgnStage, ImageRes, IMAGE_DATA_MAP);	
	//this->m_ImageWnd.RedrawWnd(FALSE);
}
//-------------------------------------------------------------------------------------//
void CProjectMapWnd::ClearProjectMapWnd()
{
	m_ProjectPtr = NULL;			
	m_ImageWnd.SetProjectPtr(NULL);
	m_ImageWnd.ReleaseImageBuffer();	
	m_ImageWnd.BuildSystemRegion();
	m_ImageWnd.RedrawWnd(true);
}
//-------------------------------------------------------------------------------------//
void CProjectMapWnd::MoveViewToStagePos(double PosX, double PosY)//移至機台位置
{
	if ( m_ImageWnd.GetSafeHwnd() == NULL ) { return; }
	TREGION4D  StageRgn;
	const double RgnW = 1000;
	const double RgnH = 1000;
	const bool bForceMove = false;
	
	StageRgn.SetRgn(PosX, PosY, RgnW, RgnH);	
	m_ImageWnd.MoveViewToStageRgn(StageRgn, bForceMove);
	//m_ImageWnd.MoveViewToStagePos(PosX, PosY, bForceMove);	
	
}
//-------------------------------------------------------------------------------------//
void CProjectMapWnd::OnRButtonDown(UINT nFlags, CPoint point)
{
	CWnd::OnRButtonDown(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CProjectMapWnd::OnRButtonUp(UINT nFlags, CPoint point)
{	
	CWnd::OnRButtonUp(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CProjectMapWnd::UpdateCommandUI()
{
	if ( CWnd::GetSafeHwnd() == NULL ) { return; }
	//Menu
	bool   bShow = false;
	UINT   ItemID = 0;
	CMenu *pMenu = this->GetMenu();
	if ( pMenu != NULL )
	{	
		//顯示
		ItemID = MENU_PROJECT_MAP_SHOW_FD;
		if ( m_ImageWnd.GetShowFiducial() == true )
		{	pMenu->CheckMenuItem(ItemID, MF_CHECKED); }
		else
		{	pMenu->CheckMenuItem(ItemID, MF_UNCHECKED); }

		ItemID = MENU_PROJECT_MAP_SHOW_PANEL;
		if ( m_ImageWnd.GetShowPanel() == true )
		{	pMenu->CheckMenuItem(ItemID, MF_CHECKED); }
		else
		{	pMenu->CheckMenuItem(ItemID, MF_UNCHECKED); }

		ItemID = MENU_PROJECT_MAP_SHOW_BOARD;
		if ( m_ImageWnd.GetShowBoard() == true )
		{	pMenu->CheckMenuItem(ItemID, MF_CHECKED); }
		else
		{	pMenu->CheckMenuItem(ItemID, MF_UNCHECKED); }

		ItemID = MENU_PROJECT_MAP_SHOW_BARCODE;
		if ( m_ImageWnd.GetShowBarcode() == true )
		{	pMenu->CheckMenuItem(ItemID, MF_CHECKED); }
		else
		{	pMenu->CheckMenuItem(ItemID, MF_UNCHECKED); }

		ItemID = MENU_PROJECT_MAP_SHOW_COMPONENT;
		if ( m_ImageWnd.GetShowComponent() == true )
		{	pMenu->CheckMenuItem(ItemID, MF_CHECKED); }
		else
		{	pMenu->CheckMenuItem(ItemID, MF_UNCHECKED); }
		
		ItemID = MENU_PROJECT_MAP_SHOW_CAMERA;
		if ( m_ImageWnd.GetShowCameraRgn() == true )
		{	pMenu->CheckMenuItem(ItemID, MF_CHECKED); }
		else
		{	pMenu->CheckMenuItem(ItemID, MF_UNCHECKED); }

		//MENU_PROJECT_MAP_SHOW_NONE

		//左鍵功能
		IMAGE_LBTN_CLICK_MODE LBtnClickMode = m_ImageWnd.GetLBtnClickMode();
		ItemID = MENU_PROJECT_MAP_FUNC_NONE;
		if ( IMAGE_LBTN_CLICK_NULL == LBtnClickMode )
		{	pMenu->CheckMenuItem(ItemID, MF_CHECKED); }
		else
		{	pMenu->CheckMenuItem(ItemID, MF_UNCHECKED); }
		
		ItemID = MENU_PROJECT_MAP_FUNC_MEASURE;
		if ( IMAGE_LBTN_CLICK_MEASURE == LBtnClickMode )
		{	pMenu->CheckMenuItem(ItemID, MF_CHECKED); }
		else
		{	pMenu->CheckMenuItem(ItemID, MF_UNCHECKED); }		
	}
}
//-------------------------------------------------------------------------------------//
void CProjectMapWnd::OnShowFd()
{
	bool bShow = m_ImageWnd.GetShowFiducial();
	m_ImageWnd.SetShowFiducial(!bShow);
	m_ImageWnd.RedrawWnd(FALSE);
	UpdateCommandUI();
}
//-------------------------------------------------------------------------------------//
void CProjectMapWnd::OnShowPanel()
{
	bool bShow = m_ImageWnd.GetShowPanel();
	m_ImageWnd.SetShowPanel(!bShow);
	m_ImageWnd.RedrawWnd(FALSE);
	UpdateCommandUI();
}
//-------------------------------------------------------------------------------------//
void CProjectMapWnd::OnShowBoard()
{
	bool bShow = m_ImageWnd.GetShowBoard();
	m_ImageWnd.SetShowBoard(!bShow);
	m_ImageWnd.RedrawWnd(FALSE);
	UpdateCommandUI();
}
//-------------------------------------------------------------------------------------//
void CProjectMapWnd::OnShowBarcode()
{
	bool bShow = m_ImageWnd.GetShowBarcode();
	m_ImageWnd.SetShowBarcode(!bShow);
	m_ImageWnd.RedrawWnd(FALSE);
	UpdateCommandUI();
}
//-------------------------------------------------------------------------------------//
void CProjectMapWnd::OnShowComponent()
{
	bool bShow = m_ImageWnd.GetShowComponent();
	m_ImageWnd.SetShowComponent(!bShow);
	m_ImageWnd.RedrawWnd(FALSE);
	UpdateCommandUI();
}
//-------------------------------------------------------------------------------------//
void CProjectMapWnd::OnShowCamera()
{
	bool bShow = m_ImageWnd.GetShowCameraRgn();
	m_ImageWnd.SetShowCameraRgn(!bShow);
	m_ImageWnd.RedrawWnd(FALSE);
	UpdateCommandUI();
}
//-------------------------------------------------------------------------------------//
void CProjectMapWnd::OnShowAll()
{
	m_ImageWnd.SetShowAll(true);
	m_ImageWnd.RedrawWnd(FALSE);
	UpdateCommandUI();
}
//-------------------------------------------------------------------------------------//
void CProjectMapWnd::OnShowNone()
{
	m_ImageWnd.SetShowAll(false);
	m_ImageWnd.RedrawWnd(FALSE);
	UpdateCommandUI();
}
//-------------------------------------------------------------------------------------//
void CProjectMapWnd::OnFuncNone()
{
	//m_ImageWnd.SetLBtnUpMode(IMAGE_LBTN_UP_NULL);
	m_ImageWnd.SetLBtnClickMode(IMAGE_LBTN_CLICK_NULL);	
	UpdateCommandUI();
}
//-------------------------------------------------------------------------------------//
void CProjectMapWnd::OnFuncMeasure()
{	
	//m_ImageWnd.SetLBtnUpMode(IMAGE_LBTN_UP_MEASURE);	
	m_ImageWnd.SetLBtnClickMode(IMAGE_LBTN_CLICK_MEASURE);	
	UpdateCommandUI();
}
//-------------------------------------------------------------------------------------//