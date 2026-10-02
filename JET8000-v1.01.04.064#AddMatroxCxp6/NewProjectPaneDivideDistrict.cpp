// NewProjectPaneDivideDistrict.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "NewProjectPaneDivideDistrict.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CNewProjectPaneDivideDistrict dialog
//-------------------------------------------------------------------------------------//
CNewProjectPaneDivideDistrict::CNewProjectPaneDivideDistrict(CWnd* pParent /*=NULL*/)
	: CDialog(CNewProjectPaneDivideDistrict::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewProjectPaneDivideDistrict)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	POINT  Pt={0,0};
	m_Finish = false;
	m_ReBuild = true;
	m_ProjectPtr = NULL;
	m_NewProjectMode = NEW_PROJECT_ONLINE;	
	m_EnableMultiDistrictMode = false;//多段模式

	m_ZoomScale = 1.0;	
	m_ViewOffset.x = m_ViewOffset.y = 0;

	m_LastPos = Pt;	
	m_LBtnUpPos = Pt;
	m_LBtnDownPos = Pt;	
	m_RBtnUpPos = Pt;
	m_RBtnDownPos = Pt;	

	m_ImageW = 0;
	m_ImageH = 0;
	m_ImageStep = 0;
	m_BitCount = 8;
	m_ImagePtr = NULL;
	m_ImageSize = 0;
	m_ImageInfoPtr = NULL;
	m_DividePosX = 0;
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneDivideDistrict::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewProjectPaneDivideDistrict)
	DDX_Control(pDX, DISTRICT_IMAGE_WND, m_ImageWnd);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CNewProjectPaneDivideDistrict, CDialog)
	//{{AFX_MSG_MAP(CNewProjectPaneDivideDistrict)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_PAINT()		
	ON_BN_CLICKED(DISTRICT_ORIENTATION_ROTATE_090_BTN, OnOrientationRotate090Btn)
	ON_BN_CLICKED(DISTRICT_ORIENTATION_ROTATE_180_BTN, OnOrientationRotate180Btn)
	ON_BN_CLICKED(DISTRICT_ORIENTATION_ROTATE_270_BTN, OnOrientationRotate270Btn)
	ON_BN_CLICKED(DISTRICT_ORIENTATION_CENTERED_BTN, OnOrientationCenteredBtn)
	ON_BN_CLICKED(DISTRICT_ORIENTATION_MIRRORX_BTN, OnOrientationMirrorxBtn)
	ON_BN_CLICKED(DISTRICT_ORIENTATION_MIRRORY_BTN, OnOrientationMirroryBtn)	
	ON_BN_CLICKED(DISTRICT_DIVIDE_DISTRICT_BTN, OnDivideDistrictBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CNewProjectPaneDivideDistrict message handlers
//-------------------------------------------------------------------------------------//
BOOL CNewProjectPaneDivideDistrict::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_ImageWnd.GetClientRect(&m_ImageWndRect);
	m_ImageWndMemDC.CreateMemDC(m_ImageWnd, 0x000000);
	m_ImageWndMemDC2.CreateMemDC(m_ImageWnd, 0x000000);
	SwitchMultiLanguage();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneDivideDistrict::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	ClearImageBuffer();
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneDivideDistrict::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if ( CWnd::GetSafeHwnd() == NULL ) { return; }
	RECT WndRect={0,0,0,0};
	const int GapX = 4;
	const int GapY = 4;
	if ( m_ImageWnd.GetSafeHwnd() != NULL )
	{
		m_ImageWnd.GetWindowRect(&WndRect);
		CWnd::ScreenToClient(&WndRect);
		//WndRect.left = GapX;
		//WndRect.top  = GapY;
		WndRect.right = cx-GapX;
		WndRect.bottom = cy-GapY;
		m_ImageWnd.MoveWindow(&WndRect);
		m_ImageWnd.GetClientRect(&m_ImageWndRect);
		m_ImageWndMemDC.CreateMemDC(m_ImageWnd, 0x000000);
		m_ImageWndMemDC2.CreateMemDC(m_ImageWnd, 0x000000);
		CreateBKImage();
		RedrawWnd();
	}
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneDivideDistrict::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if ( TRUE == bShow ) 
	{
		if ( true == m_ReBuild )
		{
			BuildDefaultMap();
			m_ReBuild = false;
		}
		CreateImageBuffer();
		BuildShowImage(0);
		CreateBKImage();
		RedrawWnd();		
		//PostParentWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_SHOW_PROJECT_MAP_WND, FALSE);
	}	
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneDivideDistrict::OnOK() 
{
	// TODO: Add extra validation here
	return;
	CDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneDivideDistrict::OnCancel() 
{
	// TODO: Add extra cleanup here
	return;
	CDialog::OnCancel();
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneDivideDistrict::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	POINT pt = point;
	if ( JetAPI::CheckPtInCtrlWnd(this, pt, DISTRICT_IMAGE_WND, &pt) == false ) 
	{
		CDialog::OnLButtonDown(nFlags, point);
		return;
	}

	CWnd::SetCapture();
	m_LBtnUpPos = m_LBtnDownPos = m_LastPos = pt;
	CDialog::OnLButtonDown(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneDivideDistrict::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	POINT pt = point;
	::ReleaseCapture();
	CWnd::MapWindowPoints(&m_ImageWnd, &pt, 1);
	m_LBtnUpPos = pt;
	CDialog::OnLButtonUp(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneDivideDistrict::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if ( CWnd::GetCapture() != this ) 
	{
		return CDialog::OnMouseMove(nFlags, point);
	}
	POINT Dp={0,0};
	POINT pt = point;
	CWnd::MapWindowPoints(&m_ImageWnd, &pt, 1);	
	Dp.x = pt.x-m_LastPos.x;
	Dp.y = pt.y-m_LastPos.y;
	if ( nFlags&MK_LBUTTON )
	{	
		ExecMoveMap(Dp.x, Dp.y);				
	}
	if ( nFlags&MK_RBUTTON )
	{		
		m_ViewOffset.x += Dp.x;
		m_ViewOffset.y += Dp.y;
		CreateBKImage();
		RedrawWnd();		
	}
	m_LastPos = pt;
	CDialog::OnMouseMove(nFlags, point);
}
//-------------------------------------------------------------------------------------//
BOOL CNewProjectPaneDivideDistrict::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	double NextImageZoom = m_ZoomScale;
	if ( zDelta > 0 ) 
	{	NextImageZoom *= 1.1;	}
	else
	{	NextImageZoom /= 1.1;	}
	NextImageZoom = AOIDataCollect.AdjustImageZoom(NextImageZoom);
	ImageAPI.CalcImageWndZoom(m_ZoomScale, NextImageZoom, m_ViewOffset);
	m_ZoomScale = NextImageZoom;		
	CreateBKImage();
	RedrawWnd();
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneDivideDistrict::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	POINT pt = point;
	if ( JetAPI::CheckPtInCtrlWnd(this, pt, DISTRICT_IMAGE_WND, &pt) == false ) 
	{
		CDialog::OnRButtonDown(nFlags, point);
		return;
	}

	CWnd::SetCapture();
	m_RBtnUpPos = m_RBtnDownPos = m_LastPos = pt;
	CDialog::OnRButtonDown(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneDivideDistrict::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	POINT pt = point;
	::ReleaseCapture();
	CWnd::MapWindowPoints(&m_ImageWnd, &pt, 1);
	m_RBtnUpPos = pt;
	CDialog::OnRButtonUp(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneDivideDistrict::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CDialog::OnPaint() for painting messages
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//

BOOL CNewProjectPaneDivideDistrict::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	switch ( pMsg->message )
	{
	case WM_KEYDOWN:
		switch ( pMsg->wParam )
		{
		case VK_LEFT:
			if ( JetAPI::CheckIsPressVRKey(VK_SHIFT) == true ) 
			{	ExecMoveDivideLine(-10); }
			else
			{	ExecMoveDivideLine(-1); }
			break;
		case VK_RIGHT:
			if ( JetAPI::CheckIsPressVRKey(VK_SHIFT) == true ) 
			{	ExecMoveDivideLine(10); }
			else
			{	ExecMoveDivideLine(1); }
			break;
		}
		break;
	}
	return CDialog::PreTranslateMessage(pMsg);
}
//-------------------------------------------------------------------------------------//
LRESULT CNewProjectPaneDivideDistrict::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneDivideDistrict::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_NEW_PROJECT_PANE_DIVIDE_DISTRICT");
	//---------------------------------------------------------------------------------//
	WndID = IDD_NEW_PROJECT_PANE_DIVIDE_DISTRICT;
	WndKey = _T("IDD_NEW_PROJECT_PANE_DIVIDE_DISTRICT");
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
	WndID = DISTRICT_ORIENTATION_GROUP;
	WndKey = _T("DISTRICT_ORIENTATION_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = DISTRICT_ORIENTATION_CENTERED_BTN;
	WndKey = _T("DISTRICT_ORIENTATION_CENTERED_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = DISTRICT_ORIENTATION_MIRRORX_BTN;
	WndKey = _T("DISTRICT_ORIENTATION_MIRRORX_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = DISTRICT_ORIENTATION_MIRRORY_BTN;
	WndKey = _T("DISTRICT_ORIENTATION_MIRRORY_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = DISTRICT_ORIENTATION_ROTATE_090_BTN;
	WndKey = _T("DISTRICT_ORIENTATION_ROTATE_090_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = DISTRICT_ORIENTATION_ROTATE_180_BTN;
	WndKey = _T("DISTRICT_ORIENTATION_ROTATE_180_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = DISTRICT_ORIENTATION_ROTATE_270_BTN;
	WndKey = _T("DISTRICT_ORIENTATION_ROTATE_270_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//
	WndID = DISTRICT_DIVIDE_DISTRICT_BTN;
	WndKey = _T("DISTRICT_DIVIDE_DISTRICT_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	/*
	WndID = AAAAAAAAAAAAAAAAAAAAAA;
	WndKey = _T("AAAAAAAAAAAAAAAAAAAAAA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	*/
	//---------------------------------------------------------------------------------//
}
//-------------------------------------------------------------------------------------//
CString CNewProjectPaneDivideDistrict::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_NEW_PROJECT_PANE_DIVIDE_DISTRICT");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneDivideDistrict::SendParentWndMessage(UINT message, WPARAM wParam, LPARAM lParam)//發送訊息給父視窗
{
	HWND hWnd = CWnd::GetSafeHwnd();
	AOIDataCollect.SendParentWndMessage(hWnd, message, wParam, lParam);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneDivideDistrict::PostParentWndMessage(UINT message, WPARAM wParam, LPARAM lParam)//發送訊息給父視窗
{
	HWND hWnd = CWnd::GetSafeHwnd();
	AOIDataCollect.PostParentWndMessage(hWnd, message, wParam, lParam);	
	return true;
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneDivideDistrict::SetProjectPtr(CAOIProject *ProjectPtr)
{
	RECT Rect={0,0,0,0};
	m_MapRect_DA=Rect;
	m_MapRect_DB=Rect;
	m_ProjectPtr = ProjectPtr;	
}
//-------------------------------------------------------------------------------------//
DISTRICT_ID  CNewProjectPaneDivideDistrict::GetDistrictID()
{
	return DISTRICT_ID_A;
}
//-------------------------------------------------------------------------------------//
CAOIPanel* CNewProjectPaneDivideDistrict::GetActivePanelPtr()
{
	CAOIProject *ProjectPtr = GetActiveProjectPtr();
	if ( NULL == ProjectPtr ) { return NULL; }
	return ProjectPtr->GetProjectPanelPtrBySelected();
}
//-------------------------------------------------------------------------------------//
CAOIProject* CNewProjectPaneDivideDistrict::GetActiveProjectPtr()
{
	return m_ProjectPtr;
}
//-------------------------------------------------------------------------------------//
NEW_PROJECT_MODE CNewProjectPaneDivideDistrict::GetNewProjectMode() const
{
	return m_NewProjectMode;
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneDivideDistrict::SetNewProjectMode(NEW_PROJECT_MODE Mode)
{
	m_NewProjectMode = Mode;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneDivideDistrict::GetEnableMultiDistrictMode() const
{
	return m_EnableMultiDistrictMode;
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneDivideDistrict::SetEnableMultiDistrictMode(bool Mode)
{
	m_EnableMultiDistrictMode = Mode;	
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneDivideDistrict::ExecNextPane()
{
	if ( CheckFinish() == false ) 
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneDivideDistrict::ExecPrevPane()
{
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneDivideDistrict::ExecFinishPane()
{
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneDivideDistrict::ReInitialPane()
{
	m_ReBuild = true;
	m_MapCTS.Identity();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneDivideDistrict::ClearImageBuffer()
{
	m_ImageW = 0;
	m_ImageH = 0;
	m_ImageStep = 0;
	m_BitCount = 8;
	JetMemory.free_func(m_ImagePtr);
	m_ImagePtr = NULL;
	m_ImageSize = 0;
	if ( NULL != m_ImageInfoPtr ) 
	{	delete[] m_ImageInfoPtr; }
	m_ImageStageRgn=TREGION4D();
	m_ImageRes.x = m_ImageRes.y = 1.0;
	m_ImageInfoPtr = NULL;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneDivideDistrict::CheckFinish()
{
	CString str;
	if ( false == m_Finish ) 
	{
		str.Format(_T("Error, Divide Components First"));
		str = this->LoadMultiLanguageString(str, str);
		JetAPI::ShowMessageBox(str);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneDivideDistrict::SetFinish(bool val)
{
	m_Finish = val;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneDivideDistrict::ExecDivideDistrict()
{
	CAOIProject    *ProjectPtr = GetActiveProjectPtr();
	if ( NULL == ProjectPtr ) { return false; }
	CAOIPanel      *PanelPtr = GetActivePanelPtr();
	if ( NULL == PanelPtr ) { return false; }	
	
	CString        str;
	size_t         i=0;
	POINT          ImageCp;	
	TPOINT2D       CadPos;
	TPOINT2D       StagePos;	
	TRECT4D        ImageRect;
	TRECT4D        StageRect;
	TPOINT2D       MapStageCp;
	DISTRICT_ID    DistrictID;
	CMapCoordinate MapCTSOther=m_MapCTS;
	TREGION4D      MapStageRgn=m_ImageStageRgn;
	TPOINT2D       MapResolution=m_ImageRes;
	IMAGE_SIZE     ImageW = m_ImageW;
	IMAGE_SIZE     ImageH = m_ImageH;
	RECT           WndRect = m_ImageWndRect;
	TPOINT2D       OffsetPt = m_ViewOffset;
	double         ZoomScale = m_ZoomScale;		
	CAOIFd        *FdPtr = NULL;	
	CAOIBoard     *BoardPtr = NULL;
	CAOIBarcode   *BarcodePtr = NULL;
	CAOIComponent *ComponentPtr = NULL;		
	size_t         DistrictCount_DA=0;
	size_t         DistrictCount_DB=0;
	const int      DividePosX = m_DividePosX;
	const DISTRICT_ID ActDistrictID=GetDistrictID();
	const size_t   FdCount = PanelPtr->GetPanelFdCount();
	const size_t   BoardCount = PanelPtr->GetPanelBoardCount();
	const size_t   BarcodeCount = PanelPtr->GetPanelBarcodeCount();
	const size_t   ComponentCount = PanelPtr->GetPanelComponentCount();
	const BOOL     bDivideFd = FALSE;
	const BOOL     bDivideBarcode = FALSE;
	const BOOL     bDivideComponent = TRUE;
	
	MapStageCp.x = MapStageRgn.GetCpX();
	MapStageCp.y = MapStageRgn.GetCpY();	
	ProjectPtr->SetProjectMapDividePos(DividePosX);

	TREGION4D MapStageRgnDA;
	TREGION4D MapStageRgnDB;
	double CadPosX[4], CadPosY[4];
	double StagePosX[4], StagePosY[4];
	CMapCoordinate MapCTS_DA, MapSTC_DA;
	CMapCoordinate MapCTS_DB, MapSTC_DB;
	::memset(CadPosX, 0x00, sizeof(CadPosX));
	::memset(CadPosY, 0x00, sizeof(CadPosY));
	::memset(StagePosX, 0x00, sizeof(StagePosX));
	::memset(StagePosY, 0x00, sizeof(StagePosY));
	ProjectPtr->GetProjectMapTeachRgn_DA(MapStageRgnDA);	
	ProjectPtr->GetProjectMapTeachRgn_DB(MapStageRgnDB);		
	if ( DISTRICT_ID_A == ActDistrictID )
	{
		MapCTS_DA = m_MapCTS;
		CadPosX[0] = 0;	CadPosY[0] = 0;		
		MapCTS_DA.Map2D(CadPosX[0], CadPosY[0], StagePosX[0], StagePosY[0]);				
		MapSTC_DA.CalcMatrix2D(StagePosX, StagePosY, CadPosX, CadPosY, 1);	

		StagePos.x = MapStageRgnDA.GetCpX();
		StagePos.y = MapStageRgnDA.GetCpY();
		MapSTC_DA.Map2D(StagePos.x, StagePos.y, CadPos.x, CadPos.y); 
		CadPosX[0] = CadPos.x;	CadPosY[0] = CadPos.y;
		StagePosX[0] = MapStageRgnDB.GetCpX();	StagePosY[0] = MapStageRgnDB.GetCpY();
		MapCTS_DB.CalcMatrix2D(CadPosX, CadPosY, StagePosX, StagePosY, 1);	
		MapCTSOther = MapCTS_DB;
	}
	if ( DISTRICT_ID_B == ActDistrictID )
	{
		MapCTS_DB = m_MapCTS;
		CadPosX[0] = 0;	CadPosY[0] = 0;		
		MapCTS_DB.Map2D(CadPosX[0], CadPosY[0], StagePosX[0], StagePosY[0]);
		MapSTC_DB.CalcMatrix2D(StagePosX, StagePosY, CadPosX, CadPosY, 1);	

		StagePos.x = MapStageRgnDB.GetCpX();
		StagePos.y = MapStageRgnDB.GetCpY();
		MapSTC_DB.Map2D(StagePos.x, StagePos.y, CadPos.x, CadPos.y); 
		CadPosX[0] = CadPos.x;	CadPosY[0] = CadPos.y;
		StagePosX[0] = MapStageRgnDA.GetCpX();	StagePosY[0] = MapStageRgnDA.GetCpY();
		MapCTS_DA.CalcMatrix2D(CadPosX, CadPosY, StagePosX, StagePosY, 1);	
		MapCTSOther = MapCTS_DA;
	}

	if ( TRUE == bDivideFd )
	{
		for ( i=0; i<FdCount; i++ )
		{
			FdPtr = PanelPtr->GetPanelFdPtr(i, false);
			if ( NULL == FdPtr ) { continue; }
			if ( NULL == FdPtr->GetFdBoardPtr() ) { continue; }
			CadPos = FdPtr->GetFdCadPos();					
			FdPtr->MapFdCadToStagePos(m_MapCTS);
			m_MapCTS.Map2D(CadPos.x, CadPos.y, StagePos.x, StagePos.y);		
			StageRect.left   = StagePos.x;
			StageRect.top    = StagePos.y;
			StageRect.right  = StagePos.x;
			StageRect.bottom = StagePos.y;

			AOIDataCollect.MapStageRectToCamera(ImageW, ImageH, MapResolution, StageRect, MapStageCp, ImageRect);
			ImageCp.x = (ImageRect.left+ImageRect.right)/2;
			ImageCp.y = (ImageRect.top+ImageRect.bottom)/2;		
			if ( m_MapRect_DA.right > m_MapRect_DB.right )
			{
				if ( ImageCp.x < DividePosX )
				{	DistrictID = DISTRICT_ID_B;	}
				else
				{	DistrictID = DISTRICT_ID_A;	}
			}
			else
			{
				if ( ImageCp.x > DividePosX )
				{	DistrictID = DISTRICT_ID_B;	}
				else
				{	DistrictID = DISTRICT_ID_A;	}
			}	
			switch ( DistrictID )
			{
			case DISTRICT_ID_B:	DistrictCount_DB ++;	break;
			case DISTRICT_ID_A:	DistrictCount_DA ++;	break;
			}
			FdPtr->SetFdDistrictID(DistrictID);	
			if ( DistrictID != ActDistrictID )
			{	FdPtr->MapFdCadToStagePos(MapCTSOther);	}
		}				
	}

	if ( TRUE == bDivideBarcode )
	{
		for ( i=0; i<BarcodeCount; i++ )
		{
			BarcodePtr = PanelPtr->GetPanelBarcodePtr(i, false);
			if ( NULL == BarcodePtr ) { continue; }
			CadPos = BarcodePtr->GetBarcodeCadPos();	
			BarcodePtr->MapBarcodeCadToStagePos(m_MapCTS);
			m_MapCTS.Map2D(CadPos.x, CadPos.y, StagePos.x, StagePos.y);		
			StageRect.left   = StagePos.x;
			StageRect.top    = StagePos.y;
			StageRect.right  = StagePos.x;
			StageRect.bottom = StagePos.y;

			AOIDataCollect.MapStageRectToCamera(ImageW, ImageH, MapResolution, StageRect, MapStageCp, ImageRect);
			ImageCp.x = (ImageRect.left+ImageRect.right)/2;
			ImageCp.y = (ImageRect.top+ImageRect.bottom)/2;		
			if ( m_MapRect_DA.right > m_MapRect_DB.right )
			{
				if ( ImageCp.x < DividePosX )
				{	DistrictID = DISTRICT_ID_B;	}
				else
				{	DistrictID = DISTRICT_ID_A;	}
			}
			else
			{
				if ( ImageCp.x > DividePosX )
				{	DistrictID = DISTRICT_ID_B;	}
				else
				{	DistrictID = DISTRICT_ID_A;	}
			}				
			switch ( DistrictID )
			{
			case DISTRICT_ID_B:	DistrictCount_DB ++;	break;
			case DISTRICT_ID_A:	DistrictCount_DA ++;	break;
			}
			BarcodePtr->SetBarcodeDistrictID(DistrictID);
			if ( DistrictID != ActDistrictID )
			{	BarcodePtr->MapBarcodeCadToStagePos(MapCTSOther);	}
		}		
	}


	if ( TRUE == bDivideComponent )
	{
		for ( i=0; i<ComponentCount; i++ )
		{
			ComponentPtr = PanelPtr->GetPanelComponentPtr(i, false);
			if ( NULL == ComponentPtr ) { continue; }
			CadPos = ComponentPtr->GetComponentCadPos();		
			ComponentPtr->MapComponentCadToStagePos(m_MapCTS);
			m_MapCTS.Map2D(CadPos.x, CadPos.y, StagePos.x, StagePos.y);		
			StageRect.left   = StagePos.x;
			StageRect.top    = StagePos.y;
			StageRect.right  = StagePos.x;
			StageRect.bottom = StagePos.y;

			AOIDataCollect.MapStageRectToCamera(ImageW, ImageH, MapResolution, StageRect, MapStageCp, ImageRect);
			ImageCp.x = (ImageRect.left+ImageRect.right)/2;
			ImageCp.y = (ImageRect.top+ImageRect.bottom)/2;		
			if ( m_MapRect_DA.right > m_MapRect_DB.right )
			{
				if ( ImageCp.x < DividePosX )
				{	DistrictID = DISTRICT_ID_B;	}
				else
				{	DistrictID = DISTRICT_ID_A;	}
			}
			else
			{
				if ( ImageCp.x > DividePosX )
				{	DistrictID = DISTRICT_ID_B;	}
				else
				{	DistrictID = DISTRICT_ID_A;	}
			}
			switch ( DistrictID )
			{
			case DISTRICT_ID_B:	DistrictCount_DB ++;	break;
			case DISTRICT_ID_A:	DistrictCount_DA ++;	break;
			}
			ComponentPtr->SetComponentDistrictID(DistrictID);
			if ( DistrictID != ActDistrictID )
			{	ComponentPtr->MapComponentCadToStagePos(MapCTSOther);	}
		}		
	}
	PanelPtr->LayoutPanelBoardListRegion();
	SetFinish(true);

	CString strDistrictA=AOIDataDefine.GetDistrictIDText(DISTRICT_ID_A);
	CString strDistrictB=AOIDataDefine.GetDistrictIDText(DISTRICT_ID_B);
	str.Format(_T("%s:%d, %s:%d"), strDistrictA, DistrictCount_DA, strDistrictB, DistrictCount_DB);
	CWnd::SetDlgItemText(DISTRICT_INFO_EDIT, str);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneDivideDistrict::ExecMoveMap(int x, int y)
{
	double dx = x;
	double dy = y;
	const double ZoomScale = m_ZoomScale;
	const bool SignX = AOIDataCollect.GetStageSignPositiveX();
	const bool SignY = AOIDataCollect.GetStageSignPositiveY();
	dx = dx*ZoomScale;
	dy = dy*ZoomScale;

	dx *= m_ImageRes.x;
	dy *= m_ImageRes.y;

	if ( true == SignX )
	{	dx = dx; }
	else
	{	dx = -dx; }
	if ( true == SignY )
	{	dy = -dy; }
	else
	{	dy = dy; }

	m_MapCTS.MoveMatrix2D(dx, dy);	
	RedrawWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
bool  CNewProjectPaneDivideDistrict::ExecMoveDivideLine(int x)
{
	m_DividePosX += x;
	RedrawWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneDivideDistrict::BuildDefaultMap()
{
	CAOIProject *ProjectPtr = GetActiveProjectPtr();
	if ( NULL == ProjectPtr ) { return false; }
	CAOIPanel   *PanelPtr = ProjectPtr->GetProjectPanelPtrBySelected();
	if ( NULL == PanelPtr ) { return false; }

	int    i=0;
	double CadPosX[4]={0};
	double CadPosY[4]={0};
	double StagePosX[4]={0};
	double StagePosY[4]={0};		
	TREGION4D    MapStageRgn;	
	DISTRICT_ID  DistrictID = GetDistrictID();
	TREGION4D    PanelRgnCad = PanelPtr->GetPanelRgnCad();	

	switch ( DistrictID ) 
	{
	case DISTRICT_ID_A:	ProjectPtr->GetProjectMapTeachRgn_DA(MapStageRgn);	break;
	case DISTRICT_ID_B:	ProjectPtr->GetProjectMapTeachRgn_DB(MapStageRgn);	break;
	}

	CadPosX[0] = PanelRgnCad.GetCpX();
	CadPosY[0] = PanelRgnCad.GetCpY();
	StagePosX[0] = MapStageRgn.GetCpX();
	StagePosY[0] = MapStageRgn.GetCpY();
	m_ImageStageRgn = MapStageRgn;
	m_MapCTS.CalcMatrix2D(CadPosX, CadPosY, StagePosX, StagePosY, 1);	
	ProjectPtr->GetProjectMapLocRect_DA(m_MapRect_DA);
	ProjectPtr->GetProjectMapLocRect_DB(m_MapRect_DB);	

	if ( m_MapRect_DA.right > m_MapRect_DB.right )
	{	m_DividePosX = (m_MapRect_DA.left+m_MapRect_DB.right)/2;	}
	else
	{	m_DividePosX = (m_MapRect_DB.left+m_MapRect_DA.right)/2;	}	

	SetFinish(false);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneDivideDistrict::CreateImageBuffer()
{
	const char fnName[] = "CNewProjectPaneDivideDistrict::CreateImageBuffer";
	ClearImageBuffer();
	CAOIProject *ProjectPtr = GetActiveProjectPtr();
	if ( NULL == ProjectPtr ) { return false; }

	const int  nAlign = 4;
	IMAGE_SIZE MapW=0;
	IMAGE_SIZE MapH=0;
	IMAGE_SIZE MapStep=0;
	IMAGE_SIZE MapBitCount=0;
	IMAGE_PTR  MapPtr=0;
	IMAGE_SIZE ColorBitCnt=24;	
	unsigned int index = 0;
	const DISTRICT_ID DistrictID = GetDistrictID();
	if ( ProjectPtr->GetProjectMapPtr(index, MapW, MapH, MapStep, MapBitCount, MapPtr) == false ) 
	{	return false;	}
	if ( NULL == MapPtr ) 
	{	return false; }

	size_t InfoSize=0;
	if ( ImageAPI.CreateBMPInfoBuffer(m_ImageInfoPtr, InfoSize) == false ) 
	{	return false; }

	IMAGE_PTR    BufferPtr=NULL;
	IMAGE_SIZE   MaxStep = JetAPI::GetBMPImagePixelsPerLine(MapW, ColorBitCnt, nAlign);
	const size_t BufferSize = ImageAPI.CalcBufferSize(MaxStep, MapH);
	if ( JetMemory.alloc_func(BufferSize, BufferPtr, fnName, "BufferPtr") == false ) 
	{	return false;	}
	::memset(BufferPtr, 0x00, sizeof(IMAGE_DATA)*BufferSize);
	m_ImagePtr = BufferPtr;
	m_ImageSize = BufferSize;	
	ProjectPtr->GetProjectMapTeachRgn(DistrictID, m_ImageStageRgn);	
	ProjectPtr->GetProjectMapResolution(m_ImageRes.x, m_ImageRes.y);
	return BuildShowImage(index);	
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneDivideDistrict::BuildShowImage(size_t index)
{
	CAOIProject *ProjectPtr = GetActiveProjectPtr();
	if ( NULL == ProjectPtr ) { return false; }
	if ( NULL == m_ImagePtr ) { return false; }

	const int  nAlign = 4;
	IMAGE_SIZE MapW=0;
	IMAGE_SIZE MapH=0;
	IMAGE_SIZE MapStep=0;
	IMAGE_SIZE MapBitCount=0;
	IMAGE_PTR  MapPtr=0;
	IMAGE_SIZE ColorBitCnt=24;		
	if ( ProjectPtr->GetProjectMapPtr(index, MapW, MapH, MapStep, MapBitCount, MapPtr) == false ) 
	{	return false;	}
	if ( NULL == MapPtr ) 
	{	return false; }
	const size_t BufferSize = MapStep*MapH;
	if ( BufferSize > m_ImageSize )
	{	return false; }

	if ( AOIDataCollect.ExecEnhanceDisplayImage(MapW, MapH, MapStep, MapBitCount, MapPtr, m_ImagePtr) == false ) 
	{	return false; }

	m_ImageW    = MapW;
	m_ImageH    = MapH;
	m_ImageStep = MapStep;
	m_BitCount  = MapBitCount;
	return true;
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneDivideDistrict::RedrawWnd()
{
	if ( m_ImageWnd.GetSafeHwnd() == NULL ) { return; }
	CClientDC dc(&m_ImageWnd);
	HDC hDrawDC=NULL;
	HDC hDC = dc.GetSafeHdc();
	HDC hMemDC = m_ImageWndMemDC.GetSafeHdc();
	HDC hMemDC2 = m_ImageWndMemDC2.GetSafeHdc();
	
	if ( NULL == hDC ) { return; }
	if ( NULL == hMemDC ) { return; }
	if ( NULL == hMemDC2 ) { return; }

	RECT  Rect={0,0,0,0};
	POINT OffsetPt={0,0};	
	IMAGE_SIZE ImageW = m_ImageW;
	IMAGE_SIZE ImageH = m_ImageH;
	RECT  WndRect=m_ImageWndRect;	
	double ZoomScale = m_ZoomScale;
	BOOL bShowRectLine = TRUE;
	BOOL bShowDivideLine = TRUE;

	::IntersectClipRect(hDC, WndRect.left, WndRect.top, WndRect.right, WndRect.bottom);	

	hDrawDC = hMemDC2;
	::BitBlt(hDrawDC, 0, 0, WndRect.right, WndRect.bottom, hMemDC, 0, 0, SRCCOPY );		
	
	HPEN  hPen1 = ::CreatePen(PS_DASH, 1, 0xFFFF00);
	HPEN  hPen2 = ::CreatePen(PS_DASH, 1, 0x0000FF);
	HPEN  hPenD = ::CreatePen(PS_SOLID, 2, 0x00FFFF);
	HPEN  hPenF = ::CreatePen(PS_SOLID, 2, 0x8F8F8F);
	HPEN  hOldPen = (HPEN)(::SelectObject(hDrawDC, hPen1));	

	OffsetPt.x = (int)(m_ViewOffset.x);
	OffsetPt.y = (int)(m_ViewOffset.y);
	if ( TRUE == bShowRectLine )
	{			
		::SelectObject(hDrawDC, hPen2);	
		ImageAPI.MapImageRectToWndRect_INT(ImageW, ImageH, WndRect, OffsetPt, ZoomScale, m_MapRect_DB, Rect);		
		ImageAPI.DrawRectLine(hDrawDC, Rect);

		::SelectObject(hDrawDC, hPen1);	
		ImageAPI.MapImageRectToWndRect_INT(ImageW, ImageH, WndRect, OffsetPt, ZoomScale, m_MapRect_DA, Rect);			
		ImageAPI.DrawRectLine(hDrawDC, Rect);
	}

	if ( TRUE == bShowDivideLine )
	{
		POINT WndPt1={0,0};
		POINT WndPt2={0,0};
		POINT ImagePt1={m_DividePosX,0};
		POINT ImagePt2={m_DividePosX,ImageH};
		ImageAPI.MapImagePtToWndPt_INT(ImageW, ImageH, WndRect, OffsetPt, ZoomScale, ImagePt1, WndPt1);	
		ImageAPI.MapImagePtToWndPt_INT(ImageW, ImageH, WndRect, OffsetPt, ZoomScale, ImagePt2, WndPt2);	
		::SelectObject(hDrawDC, hPenD);	
		::MoveToEx(hDrawDC, WndPt1.x, WndPt1.y, NULL);
		::LineTo(hDrawDC, WndPt2.x, WndPt2.y);
	}
	::SelectObject(hDrawDC, hOldPen);
	::DeleteObject(hPen1);
	::DeleteObject(hPen2);	
	::DeleteObject(hPenD);	
	::DeleteObject(hPenF);	

	const int OldBkMode = ::SetBkMode(hDrawDC, TRANSPARENT);
	DrawProjectBoard(hDrawDC);
	DrawProjectComponent(hDrawDC);
	::SetBkMode(hDrawDC, OldBkMode);
	if ( hDrawDC != hDC )
	{	::BitBlt(hDC, 0, 0, WndRect.right, WndRect.bottom, hDrawDC, 0, 0, SRCCOPY );	}	
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneDivideDistrict::CreateBKImage()
{
	if ( NULL == m_ImagePtr ) { return ; }

	HDC hDC = m_ImageWndMemDC.GetSafeHdc();
	if ( NULL == hDC ) { return; }	
	RECT         Rect = m_ImageWndRect;
	IMAGE_SIZE   ImageW = m_ImageW;
	IMAGE_SIZE   ImageH = m_ImageH;
	IMAGE_SIZE   ImageStep = m_ImageStep;
	IMAGE_SIZE   ImageBitCount = m_BitCount;	
	IMAGE_PTR    ImagePtr = m_ImagePtr;
	if ( NULL == ImagePtr ) { return ; }
	BITMAPINFO *pInfo = m_ImageInfoPtr;
	if ( NULL == pInfo ) { return; }
	
	double    dZoom=1.0;
	TPOINT2D  OffsetPt2D;
	const int nDstX = 0;
	const int nDstY = 0;
	const int nDstW = Rect.right-Rect.left;
	const int nDstH = Rect.bottom-Rect.top;

	const int nSrcX = 0;
	const int nSrcY = 0;
	const int nSrcW = (int)(ImageW);
	const int nSrcH = (int)(ImageH);	
	COLORREF BkColor = 0x000000;
	OffsetPt2D = m_ViewOffset;
	if ( ImageAPI.SetBMPInfo(pInfo, ImageW, ImageH, ImageBitCount) == false ) { return; }
	ImageAPI.DrawImageToDC(hDC, pInfo, ImagePtr, Rect, OffsetPt2D, m_ZoomScale, BkColor);
	return;
	
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneDivideDistrict::DrawProjectBoard(HDC hDC)
{
	CAOIProject    *ProjectPtr = GetActiveProjectPtr();
	if ( NULL == ProjectPtr ) { return; }
	CAOIPanel      *PanelPtr = GetActivePanelPtr();
	if ( NULL == PanelPtr ) { return; }
	if ( NULL == hDC ) { return; }
	
	CString        str;
	size_t         i=0;
	RECT           DrawRect;		
	TRECT4D        DrawRect4D;
	TRECT4D        ImageRect;
	TRECT4D        StageRect;
	TPOINT2D       MapStageCp;		
	TREGION4D      BoardCadRgn;
	TREGION4D      BoardStageRgn;
	TREGION4D      MapStageRgn=m_ImageStageRgn;
	TPOINT2D       MapResolution=m_ImageRes;
	IMAGE_SIZE     ImageW = m_ImageW;
	IMAGE_SIZE     ImageH = m_ImageH;
	RECT           WndRect = m_ImageWndRect;
	TPOINT2D       OffsetPt = m_ViewOffset;
	double         ZoomScale = m_ZoomScale;
	CAOIBoard     *BoardPtr = NULL;	
	DISTRICT_ID    DistrictID = GetDistrictID();	
	const size_t   BoardCount = PanelPtr->GetPanelBoardCount();	
	const TSystemParameter &SystemParam = AOIDataCollect.GetSystemParameter();

	COLORREF      BoardColor=SystemParam.m_BoardColor1;	
	COLORREF      TextColor=SystemParam.m_BoardTextColor;	
	HPEN          hPen = ::CreatePen(PS_SOLID, 2, BoardColor);
	HPEN          hOldPen = (HPEN)(::SelectObject(hDC, hPen));	
	COLORREF      clrText = ::SetTextColor(hDC, TextColor);

	LOGFONT LogFont;
	HFONT   hFont = NULL;
	HFONT   hOldFont = NULL;
	const int FontSize = 32;
	::memset(&LogFont, 0x00, sizeof(LogFont));
	LogFont.lfHeight = FontSize;	
	LogFont.lfWeight = FW_BOLD;
	LogFont.lfCharSet = DEFAULT_CHARSET;
	::_tcscpy(LogFont.lfFaceName, _T("Cambria"));	
	hFont = CreateFontIndirect(&LogFont);
	hOldFont = (HFONT)::SelectObject(hDC, hFont);

	MapStageCp.x = MapStageRgn.GetCpX();
	MapStageCp.y = MapStageRgn.GetCpY();	
	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr = PanelPtr->GetPanelBoardPtr(i, false);
		if ( NULL == BoardPtr ) { continue; }		
		BoardCadRgn = BoardPtr->GetBoardRgnCad();
		m_MapCTS.Map2D(BoardCadRgn.minX, BoardCadRgn.minY, BoardStageRgn.minX, BoardStageRgn.minY);
		m_MapCTS.Map2D(BoardCadRgn.maxX, BoardCadRgn.maxY, BoardStageRgn.maxX, BoardStageRgn.maxY);
		
		StageRect.left   = MIN(BoardStageRgn.minX, BoardStageRgn.maxX);
		StageRect.top    = MIN(BoardStageRgn.minY, BoardStageRgn.maxY);
		StageRect.right  = MAX(BoardStageRgn.minX, BoardStageRgn.maxX);
		StageRect.bottom = MAX(BoardStageRgn.minY, BoardStageRgn.maxY);

		AOIDataCollect.MapStageRectToCamera(ImageW, ImageH, MapResolution, StageRect, MapStageCp, ImageRect);
		ImageAPI.MapImageRectToWndRect_DBL(ImageW, ImageH, WndRect, OffsetPt, ZoomScale, ImageRect, DrawRect4D);

		JetAPI::AdjustRect(DrawRect4D, DrawRect4D);		
		JetAPI::Region4DToRect(DrawRect4D, DrawRect, false);
		if ( DrawRect.right < WndRect.left ) { continue; }
		if ( DrawRect.bottom < WndRect.top ) { continue; }
		if ( DrawRect.left > WndRect.right ) { continue; }
		if ( DrawRect.top > WndRect.bottom ) { continue; }
		ImageAPI.DrawRectLine(hDC, DrawRect);
		str.Format(_T("%d"), i+1);
		::TextOut(hDC, DrawRect.left, DrawRect.top, str, str.GetLength());
	}
	::SelectObject(hDC, hOldFont);
	::DeleteObject(hFont); hFont = NULL;
	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPen); hPen=NULL;
	::SetTextColor(hDC, clrText);
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneDivideDistrict::DrawProjectComponent(HDC hDC)
{
	CAOIProject    *ProjectPtr = GetActiveProjectPtr();
	if ( NULL == ProjectPtr ) { return; }
	CAOIPanel      *PanelPtr = GetActivePanelPtr();
	if ( NULL == PanelPtr ) { return; }
	if ( NULL == hDC ) { return; }
	
	CString        str;
	size_t         i=0;
	POINT          DrawCp;
	RECT           DrawRect;
	TSIZE2D        BodySize;
	TPOINT2D       CadPos;
	TPOINT2D       StagePos;
	TRECT4D        DrawRect4D;
	TRECT4D        ImageRect;
	TRECT4D        StageRect;
	TPOINT2D       MapStageCp;
	DISTRICT_ID    DistrictID;
	TREGION4D      MapStageRgn=m_ImageStageRgn;
	TPOINT2D       MapResolution=m_ImageRes;
	IMAGE_SIZE     ImageW = m_ImageW;
	IMAGE_SIZE     ImageH = m_ImageH;
	RECT           WndRect = m_ImageWndRect;
	TPOINT2D       OffsetPt = m_ViewOffset;
	double         ZoomScale = m_ZoomScale;		
	CAOIComponent *ComponentPtr = NULL;
	const size_t   ComponentCount = PanelPtr->GetPanelComponentCount();

	HPEN          hPenA = ::CreatePen(PS_SOLID, 1, 0xFFFF00);
	HPEN          hPenB = ::CreatePen(PS_SOLID, 1, 0x00FFFF);
	HPEN          hPenN = ::CreatePen(PS_SOLID, 1, 0x0000FF);
	HPEN          hOldPen = (HPEN)(::SelectObject(hDC, hPenA));
	COLORREF      clrText = ::SetTextColor(hDC, 0x2200A0);
	
	MapStageCp.x = MapStageRgn.GetCpX();
	MapStageCp.y = MapStageRgn.GetCpY();	
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = PanelPtr->GetPanelComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }
		CadPos = ComponentPtr->GetComponentCadPos();
		DistrictID = ComponentPtr->GetComponentDistrictID();
		BodySize.cx = ComponentPtr->GetComponentBodySizeW();
		BodySize.cy = ComponentPtr->GetComponentBodySizeH();
		m_MapCTS.Map2D(CadPos.x, CadPos.y, StagePos.x, StagePos.y);

		StageRect.left   = StagePos.x-(BodySize.cx/2);
		StageRect.top    = StagePos.y-(BodySize.cy/2);
		StageRect.right  = StagePos.x+(BodySize.cx/2);
		StageRect.bottom = StagePos.y+(BodySize.cy/2);

		AOIDataCollect.MapStageRectToCamera(ImageW, ImageH, MapResolution, StageRect, MapStageCp, ImageRect);
		ImageAPI.MapImageRectToWndRect_DBL(ImageW, ImageH, WndRect, OffsetPt, ZoomScale, ImageRect, DrawRect4D);

		JetAPI::AdjustRect(DrawRect4D, DrawRect4D);
		JetAPI::Region4DToRect(DrawRect4D, DrawRect, false);		
		if ( DrawRect.right < WndRect.left ) { continue; }
		if ( DrawRect.bottom < WndRect.top ) { continue; }
		if ( DrawRect.left > WndRect.right ) { continue; }
		if ( DrawRect.top > WndRect.bottom ) { continue; }

		DrawCp.x = (DrawRect.left+DrawRect.right)/2;
		DrawCp.y = (DrawRect.top+DrawRect.bottom)/2;

		switch ( DistrictID )
		{
		case DISTRICT_ID_A:	
			::SelectObject(hDC, hPenA);	
			break;
		case DISTRICT_ID_B:
			::SelectObject(hDC, hPenB);	
			break;
		default:
			::SelectObject(hDC, hPenN);
			break;
		}			
		ImageAPI.DrawRectLine(hDC, DrawRect);
		str = ComponentPtr->GetComponentName();
		::TextOut(hDC, DrawRect.left, DrawRect.top, str, str.GetLength());
	}	
	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPenA); hPenA=NULL;
	::DeleteObject(hPenB); hPenB=NULL;
	::DeleteObject(hPenN); hPenN=NULL;	
	::SetTextColor(hDC, clrText);
	return; 
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneDivideDistrict::OnOrientationRotate090Btn() 
{
	// TODO: Add your control notification handler code here
	CAOIPanel   *PanelPtr = GetActivePanelPtr();	
	if ( NULL == PanelPtr ) 
	{ 
		JetAPI::ShowMessageBox(this->m_ErrorString);
		return; 
	}
	PanelPtr->SpinPanel(90);	
	SetFinish(false);
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneDivideDistrict::OnOrientationRotate180Btn() 
{
	// TODO: Add your control notification handler code here
	CAOIPanel   *PanelPtr = GetActivePanelPtr();	
	if ( NULL == PanelPtr ) 
	{ 
		JetAPI::ShowMessageBox(this->m_ErrorString);
		return; 
	}
	PanelPtr->SpinPanel(180);	
	SetFinish(false);
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneDivideDistrict::OnOrientationRotate270Btn() 
{
	// TODO: Add your control notification handler code here
	CAOIPanel   *PanelPtr = GetActivePanelPtr();	
	if ( NULL == PanelPtr ) 
	{ 
		JetAPI::ShowMessageBox(this->m_ErrorString);
		return; 
	}
	PanelPtr->SpinPanel(270);		
	SetFinish(false);
	RedrawWnd();	
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneDivideDistrict::OnOrientationCenteredBtn() 
{
	// TODO: Add your control notification handler code here
	BuildDefaultMap();	
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneDivideDistrict::OnOrientationMirrorxBtn() 
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
	SetFinish(false);
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneDivideDistrict::OnOrientationMirroryBtn() 
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
	SetFinish(false);
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneDivideDistrict::OnDivideDistrictBtn() 
{
	// TODO: Add your control notification handler code here
	ExecDivideDistrict();
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//