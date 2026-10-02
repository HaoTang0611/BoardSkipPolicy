// ModelWnd.cpp : implementation file
//

#include "stdafx.h"
#include "jet8000.h"
#include "ModelWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
const UINT ID_EDIT_WND_VIEW     = 101;
const UINT ID_EDIT_IMAGE_VIEW   = 102;
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CModelWnd dialog
//-------------------------------------------------------------------------------------//
CModelWnd::CModelWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CModelWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CModelWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ModelPtr = NULL;
	m_ProjectPtr = NULL;
	m_MainFrameWnd = NULL;
	m_ImageZoom = 1.0;
	m_ImageOffset.x = m_ImageOffset.y = 0.0;;

	::memset(&m_ImageWndRect, 0x00, sizeof(m_ImageWndRect));
	::memset(&m_MousePosLast, 0x00, sizeof(m_MousePosLast));
	::memset(&m_MousePosFirst, 0x00, sizeof(m_MousePosFirst));
	::memset(&m_MousePosCurrent, 0x00, sizeof(m_MousePosCurrent));
	::memset(&m_MousePosImageWnd, 0x00, sizeof(m_MousePosImageWnd));	

	JetAPI::InitialUniFrameList(m_UniFrameList, FRAME_MAX_COUNT);
	m_ImageW = m_UniFrameList[0].ImageW;
	m_ImageH = m_UniFrameList[0].ImageH;
	m_ImageStep = m_UniFrameList[0].ImageStep;
	m_BitCount = m_UniFrameList[0].BitCount;	

	m_BkColor = 0x000000;
	m_ShowImageW = m_ImageW;
	m_ShowImageH = m_ImageH;
	m_ShowImageStep = m_ImageStep;
	m_ShowBitCount = m_BitCount;	

	m_DrawAddRect = false;
	m_ImageIndex = 0;
	m_ShowImagePtr = NULL;	
	m_ShowBufferSize = 0;;
	m_ImageResolution.x = AOIDataCollect.GetCameraResolutionX(PRIMARY_CAMERA_ID);
	m_ImageResolution.y = AOIDataCollect.GetCameraResolutionY(PRIMARY_CAMERA_ID);
}
//-------------------------------------------------------------------------------------//
void CModelWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModelWnd)
	DDX_Control(pDX, MODEL_IMAGE_WND, m_ImageWnd);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CModelWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CModelWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_WM_GETMINMAXINFO()
	ON_WM_SETCURSOR()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_CONTEXTMENU()
	ON_BN_CLICKED(MODEL_TEST_MODE_BTN, OnTestModeBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CModelWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CModelWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CWnd::ShowWindow(SW_SHOWMAXIMIZED);

	CRect rectDummy;
	rectDummy.SetRectEmpty();
	const DWORD dwPaneStype = WS_CHILD | WS_VISIBLE | WS_BORDER;
	if ( !m_ViewModelWnd.Create(dwPaneStype, rectDummy, this, ID_EDIT_WND_VIEW))
	{
		TRACE0("無法建立 [編輯框視窗]\n");
		return -1;      // 無法建立
	}
	if ( !m_ViewModelImage.Create(dwPaneStype, rectDummy, this, ID_EDIT_IMAGE_VIEW))
	{
		TRACE0("無法建立 [編輯影像視窗]\n");
		return -1;      // 無法建立
	}
	HWND hWnd = CWnd::GetSafeHwnd();
	m_MainFrameWnd = AOIDataCollect.GetMainFrameWnd();
	AOIDataCollect.SetMainFrameWnd(hWnd);

	AdjustPaneWndPosition();

	CreateShowBuffer();	
	BuildShowImageBuffer();
	CreateBKImage();
	BuildActiveObjList(m_ModelPtr, false);	
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	AOIDataCollect.SetDrawModelMode(DRAW_MODEL_EDIT);
	m_ViewModelWnd.PostMessage(MSG_EDIT_WND_PROPERTY_WND, WPARAM_BUILD_WND_SELECTED, NULL);	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CModelWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	ReleaseShowImageBuffer();
	AOIDataCollect.SetMainFrameWnd(m_MainFrameWnd);	
	//JetAPI::ClearUniFrameList(m_UniFrameList, FRAME_MAX_COUNT);
}
//-------------------------------------------------------------------------------------//
void CModelWnd::OnSize(UINT nType, int cx, int cy) 
{
	CBaseDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	AdjustPaneWndPosition();
}
//-------------------------------------------------------------------------------------//
BOOL CModelWnd::OnEraseBkgnd(CDC* pDC)
{
	//return TRUE;
	return CBaseDialog::OnEraseBkgnd(pDC);
}
//-------------------------------------------------------------------------------------//
void CModelWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CBaseDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CModelWnd::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default	
	CBaseDialog::OnGetMinMaxInfo(lpMMI);
	lpMMI->ptMinTrackSize.x = 1000;
	lpMMI->ptMinTrackSize.y = 800;
}
//-------------------------------------------------------------------------------------//
BOOL CModelWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	UINT ControlID = pWnd->GetDlgCtrlID();	

	CURSOR_POS_MODE OldCursorMode = m_MousePosMode;
	CURSOR_POS_MODE CursorMode = CheckCursorPosMode(m_MousePosImageWnd);
	m_MousePosMode = CursorMode;
	
	if ( CursorMode != OldCursorMode )
	{	CModelWnd::RedrawWnd();	}
	if ( CURSOR_POS_NONE == CursorMode )
	{	return CBaseDialog::OnSetCursor(pWnd, nHitTest, message);	}

	JetAPI::UpdateCursor(CursorMode); 
	return TRUE;
	//return CBaseDialog::OnSetCursor(pWnd, nHitTest, message);
}
//-------------------------------------------------------------------------------------//
void CModelWnd::AdjustPaneWndPosition()
{
	if ( CWnd::GetSafeHwnd() == NULL ) { return ; }
	if ( m_ImageWnd.GetSafeHwnd() == NULL ) { return ; }	
	if ( m_ViewModelWnd.GetSafeHwnd() == NULL ) { return; }
	if ( m_ViewModelImage.GetSafeHwnd() == NULL ) { return; }


	SIZE WndSize={0};
	SIZE BtnSize={0};
	RECT MainRect={0};
	RECT BtnWndRect={0};	
	RECT PaneWndRect={0};
	RECT PaneImageRect={0};
	RECT ImageWndRect={0};
	CWnd *WndPtr = NULL;	
	const int MarginX=4;
	const int MarginY=4;
	CWnd::GetClientRect(&MainRect);
	const int cx = MainRect.right-MainRect.left;
	const int cy = MainRect.bottom-MainRect.top;
	int       PanelRight=MainRect.right;

	WndPtr = CWnd::GetDlgItem(IDOK);
	if ( NULL!=WndPtr && WndPtr->GetSafeHwnd()!=NULL )
	{
		WndPtr->GetWindowRect(&BtnWndRect);
		this->ScreenToClient(&BtnWndRect);
		JetAPI::GetRectSize(BtnWndRect, BtnSize);
		BtnWndRect.right = cx-MarginX;
		BtnWndRect.left = BtnWndRect.right-BtnSize.cx;
		WndPtr->MoveWindow(&BtnWndRect);

		if ( PanelRight > BtnWndRect.left ) 
		{	PanelRight = BtnWndRect.left; }
	}

	WndPtr = CWnd::GetDlgItem(IDCANCEL);
	if ( NULL!=WndPtr && WndPtr->GetSafeHwnd()!=NULL )
	{
		WndPtr->GetWindowRect(&BtnWndRect);
		this->ScreenToClient(&BtnWndRect);
		JetAPI::GetRectSize(BtnWndRect, BtnSize);
		BtnWndRect.right = cx-MarginX;
		BtnWndRect.left = BtnWndRect.right-BtnSize.cx;
		WndPtr->MoveWindow(&BtnWndRect);

		if ( PanelRight > BtnWndRect.left ) 
		{	PanelRight = BtnWndRect.left; }
	}

	WndPtr = CWnd::GetDlgItem(MODEL_TEST_MODE_BTN);
	if ( NULL!=WndPtr && WndPtr->GetSafeHwnd()!=NULL )
	{
		WndPtr->GetWindowRect(&BtnWndRect);
		this->ScreenToClient(&BtnWndRect);
		JetAPI::GetRectSize(BtnWndRect, BtnSize);
		BtnWndRect.right = cx-MarginX;
		BtnWndRect.left = BtnWndRect.right-BtnSize.cx;
		WndPtr->MoveWindow(&BtnWndRect);
		if ( PanelRight > BtnWndRect.left ) 
		{	PanelRight = BtnWndRect.left; }
	}	

	PaneWndRect.top = 0;
	PaneWndRect.bottom = cy;
	PaneWndRect.right = PanelRight-MarginX;
	PaneWndRect.left = PaneWndRect.right - 240;
	m_ViewModelWnd.MoveWindow(&PaneWndRect, TRUE);	

	PaneImageRect.top = 0;
	PaneImageRect.bottom = cy;
	PaneImageRect.right = PaneWndRect.left-MarginX;
	PaneImageRect.left = PaneImageRect.right - 480;
	JetAPI::GetRectSize(PaneImageRect, WndSize);
	m_ViewModelImage.MoveWindow(&PaneImageRect, TRUE);
	//m_ViewModelImage.SetWindowPos(NULL, 100, PaneImageRect.top, WndSize.cx, WndSize.cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
	
	ImageWndRect.top = MarginY;
	ImageWndRect.bottom = cy-MarginY;
	ImageWndRect.left = MarginX;
	ImageWndRect.right = PaneImageRect.left-MarginX;	
	m_ImageWnd.MoveWindow(&ImageWndRect);
	m_ImageWnd.GetClientRect(&m_ImageWndRect);
	m_ImageWndMemDC1.CreateMemDC(&m_ImageWnd, m_BkColor);
	m_ImageWndMemDC2.CreateMemDC(&m_ImageWnd, m_BkColor);
	this->CreateBKImage();
}
//-------------------------------------------------------------------------------------//
inline  CAOIModel* CModelWnd::GetModelPtr()
{
	return m_ModelPtr;
}
//-------------------------------------------------------------------------------------//
inline CAOIProject* CModelWnd::GetActiveProject()
{
	return this->m_ProjectPtr;
}
//-------------------------------------------------------------------------------------//
unsigned int CModelWnd::GetMaxFrameCount()
{
	return FRAME_MAX_COUNT;
}
//-------------------------------------------------------------------------------------//
inline IMAGE_SIZE CModelWnd::GetFrameImageW() const
{
	return m_ShowImageW;
}
//-------------------------------------------------------------------------------------//
inline IMAGE_SIZE CModelWnd::GetFrameImageH() const
{
	return m_ShowImageH;
}
//-------------------------------------------------------------------------------------//
inline MANIPULATE_MODEL_MODE CModelWnd::GetManiModelMode() const
{
	return AOIDataCollect.GetManipulateModelMode();
}
//-------------------------------------------------------------------------------------//
int CModelWnd::GetEditLineSize()//取得編輯線的尺寸
{	
	const int    LineSizeLevel = AOIDataCollect.GetSystemParameter().m_EditLineSizeLevel;
	return JetAPI::GetEditLineSize(m_ImageZoom, LineSizeLevel);
}
//-------------------------------------------------------------------------------------//
int CModelWnd::GetEditCheckSize()//取得編輯線比較的尺寸
{
	const int    LineSizeLevel = AOIDataCollect.GetSystemParameter().m_EditLineSizeLevel;
	return JetAPI::GetEditCheckSize(m_ImageZoom, LineSizeLevel);
}
//-------------------------------------------------------------------------------------//
bool CModelWnd::PtInControlWnd(const POINT &pt, UINT ID, POINT &pt2)
{
	pt2 = pt;
	if ( ::PtInRect(&m_ImageWndRect, pt) == FALSE )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
void CModelWnd::SwitchMultiLanguage()
{
	size_t  i = 0;	
	UINT    WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_MODEL_WND");	
	//---------------------------------------------------------------------------------//
	WndID = IDD_MODEL_WND;
	WndKey = _T("IDD_MODEL_WND");
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
	WndID = MODEL_TEST_MODE_BTN;
	WndKey = _T("MODEL_TEST_MODE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//	
}
//-------------------------------------------------------------------------------------//
BOOL CModelWnd::PreTranslateMessage(MSG* pMsg)
{
	bool bRedraw = false;
	switch ( pMsg->message )
	{	
	case WM_KEYDOWN:
		switch ( pMsg->wParam )
		{
		case VK_DELETE:
			//OnModelEditDeleteSelect();
			break;
		case VK_LEFT:	
			bRedraw = ExecModifyActiveObjPosKernel(-1, 0);
			break;
		case VK_RIGHT:
			bRedraw = ExecModifyActiveObjPosKernel(1, 0);
			break;
		case VK_UP:
			bRedraw = ExecModifyActiveObjPosKernel(0, -1);
			break;
		case VK_DOWN:
			bRedraw = ExecModifyActiveObjPosKernel(0, 1);
			break;		
		}
		if ( true == bRedraw )
		{	RedrawWnd(); }
		break;
	case WM_KEYUP:
		switch ( pMsg->wParam )
		{
		case VK_ESCAPE:
			AOIDataCollect.CancelGatherColorMode();
			AOIDataCollect.CancelManipulateMainMode();			
			break;		
		default:
			if ( AOIDataCollect.GetCombineColorVrKey() == pMsg->wParam )
			{	AOIDataCollect.CancelGatherColorMode();	}
			break;
		}		
		break;
	}	
	return CBaseDialog::PreTranslateMessage(pMsg);
}
//-------------------------------------------------------------------------------------//
LRESULT CModelWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class	
	WPARAM param=0;
	switch ( message )
	{
	case MSG_EDIT_IMAGE_VIEW_WND:
	case MSG_EDIT_VIEW_3D_WND:
	case MSG_EDIT_VIEW_BLOB_WND:
	case MSG_EDIT_IMAGE_PROCESS_WND:
		if ( m_ViewModelImage.GetSafeHwnd() != NULL )
		{	m_ViewModelImage.SendMessage(message, wParam, lParam); }
		break;
	case MSG_EDIT_MAIN_VIEW_WND:
		switch ( wParam )
		{
		case WPARAM_REDRAW_VIEW_WND://重繪視窗
			RedrawWnd();
			break;
		case WPARAM_UPDATE_VIEW_PART_SELECTED://重繪視窗
			BuildActiveObjList(m_ModelPtr, false);
			RedrawWnd();
			break;
		case WPARAM_SWITCH_FRAME_IMAGE://專案切換-畫面
			UpdateFrameImage();
			RedrawWnd();			
			break;
		case WPARAM_UPDATE_ALG_IMAGE://更新演算法圖像
			UpdateImageByAlgParam();
			break;
		case WPARAM_EXEC_WND_INSPECT://執行檢測框測試
			ExecModelWndInspection(true);
			break;	
		case WPARAM_SHOW_WND_POSITION:
			ExecShowWndPosition();
			break;
		case WPARAM_TOGGLE_ENCHANGE_IMAGE_MODE://切換強化影像模式
			UpdateImageByAlgParam();	
			break;
		case WPARAM_CALC_WND_COLOR:
			ExecCalcWndColor();
			break;
		case WPARAM_EXTRACT_WND_COLOR_FILTER:
			ExecExtractWndColorFilter();
			break;
		}		
		break;
	}
	return CBaseDialog::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void CModelWnd::SetModelPtr(CAOIModel *Ptr)
{
	m_ModelPtr = Ptr;
	if ( NULL != m_ModelPtr )
	{	
		TREGION4D TotalRegion;
		m_ModelPtr->GetModelAttachedPosStage(m_ModelImagePosStage);			
		m_ModelPtr->GetModelTotalRegionStage(TotalRegion);
		m_ModelImagePosStage.x = TotalRegion.GetCpX();
		m_ModelImagePosStage.y = TotalRegion.GetCpY();
	}	
}
//-------------------------------------------------------------------------------------//
void CModelWnd::SetActiveProject(CAOIProject *Ptr)
{
	this->m_ProjectPtr = Ptr;
}
//-------------------------------------------------------------------------------------//
bool CModelWnd::SetUniFrameList(const std::vector<TUNI_FRAME> &UniFrameList)
{	
	size_t  i=0;
	const size_t MaxUnFrameCount = GetMaxFrameCount();
	JetAPI::ClearUniFrameList(m_UniFrameList, MaxUnFrameCount);
	const size_t UniFrameSize = UniFrameList.size();
	const size_t UniFrameCount = MIN(UniFrameSize, MaxUnFrameCount);

	for ( i=0; i<UniFrameCount; i++ )
	{	m_UniFrameList[i] = UniFrameList[i];	}

	return true;
}
//-------------------------------------------------------------------------------------//
void CModelWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	//m_ViewModelWnd.UpdateWindow();
	//m_ViewModelImage.UpdateWindow();
	RedrawWnd();
	// Do not call CBaseDialog::OnPaint() for painting messages
}
//-------------------------------------------------------------------------------------//
void CModelWnd::RedrawWnd()
{	
	CClientDC dc(&m_ImageWnd);
	HDC hDC = dc.GetSafeHdc();
	if ( hDC == NULL ) { return; }
	HDC MemDC1 = this->m_ImageWndMemDC1.GetSafeHdc();
	HDC MemDC2 = this->m_ImageWndMemDC2.GetSafeHdc();	
	if ( MemDC1==NULL || MemDC2==NULL ) { return; }
	RECT Rect = this->m_ImageWndRect;
	::IntersectClipRect(hDC, Rect.left, Rect.top, Rect.right, Rect.bottom);
	
	//HBRUSH hBrush = ::CreateSolidBrush(0x000000);
	//::FillRect(MemDC, &Rect, hBrush);
	//::DeleteObject(hBrush);

	//DrawModel
	::BitBlt(MemDC2, 0, 0, Rect.right, Rect.bottom, MemDC1, 0, 0, SRCCOPY );

	DrawModel(MemDC2);
	DrawAddRect(MemDC2);
	::BitBlt(hDC, 0, 0, Rect.right, Rect.bottom, MemDC2, 0, 0, SRCCOPY );
}
//-------------------------------------------------------------------------------------//
void CModelWnd::CreateBKImage()
{
	HDC hMemDC = m_ImageWndMemDC1.GetSafeHdc();
	if ( NULL == hMemDC ) { return; }
	
	//POINT OffsetPts;
	//OffsetPts.x = JetAPI::Round(this->m_ImageOffset.x*this->m_ImageZoom);
	//OffsetPts.y = JetAPI::Round(this->m_ImageOffset.y*this->m_ImageZoom);
	//OffsetPts.x = JetAPI::Round(this->m_ImageOffset.x);
	//OffsetPts.y = JetAPI::Round(this->m_ImageOffset.y);
	COLORREF clrBK = m_BkColor;	
	if ( ImageAPI.DrawImageToDC(hMemDC, m_ShowImageW, m_ShowImageH, m_ShowImageStep, m_ShowBitCount, m_ShowImagePtr, m_ImageWndRect, m_ImageOffset, m_ImageZoom, clrBK) == false )
	{	return ; }
}
//-------------------------------------------------------------------------------------//
void CModelWnd::DrawModel(HDC hDC)
{
	CAOIModel *ModelPtr = CModelWnd::GetModelPtr();
	if ( NULL == ModelPtr ) { return; }

	TMODEL_DRAW_PARAM DrawParam;
	TPOINT2D ComponentStagePos;
	TPOINT2D StageOffset, CadOffset;
	DRAW_MODEL_MODE DrawModelMode = AOIDataCollect.GetDrawModelMode();

	ModelPtr->GetModelAttachedPosStage(ComponentStagePos);

	const double StageCpx = m_ModelImagePosStage.x;
	const double StageCpy = m_ModelImagePosStage.y;
	const double StageOffsetX = (StageCpx-ComponentStagePos.x);	
	const double StageOffsetY = (StageCpy-ComponentStagePos.y);

	//Cad座標與影像座標為固定方位, 因此先將機台偏差改成Cad偏差, 再來處理
	StageOffset.x = StageOffsetX;
	StageOffset.y = StageOffsetY;
	AOIDataCollect.MapStageOffsetPtToCad(StageOffset, CadOffset);
	const double ImageOffsetX =  CadOffset.x/m_ImageResolution.x;
	const double ImageOffsetY = -CadOffset.y/m_ImageResolution.y;
	const double ViewOffsetX = ImageOffsetX/m_ImageZoom;
	const double ViewOffsetY = ImageOffsetY/m_ImageZoom;

	DrawParam.WndRect = m_ImageWndRect;
	DrawParam.ViewCP.x = DrawParam.ViewCP.y = 0;
	DrawParam.Scale = m_ImageZoom;	
	DrawParam.ViewOffsetX =  m_ImageOffset.x;
	DrawParam.ViewOffsetY =  -m_ImageOffset.y;	
	DrawParam.ResolutionX = m_ImageResolution.x;
	DrawParam.ResolutionY = m_ImageResolution.y;
	DrawParam.ShowEditLine = true;		
	DrawParam.ViewCP.x = -JetAPI::Floor(ViewOffsetX);
	DrawParam.ViewCP.y = -JetAPI::Floor(ViewOffsetY);

	ModelPtr->DrawModel(hDC, DrawModelMode, DrawParam);
}
//-------------------------------------------------------------------------------------//
void CModelWnd::DrawAddRect(HDC hDC)
{
	if ( false == m_DrawAddRect ) { return; }
	if ( CURSOR_POS_NONE != m_MousePosMode ) { return; }
	CAOIModel *ModelPtr = CModelWnd::GetModelPtr();
	if ( NULL == ModelPtr ) { return; }

	HPEN hPen    = ::CreatePen(PS_SOLID, 1, 0x00FF00);
	HPEN hOldPen = (HPEN)::SelectObject(hDC, hPen);

	const double ComponentAngle = ModelPtr->GetModelAttachedAngle();
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComponentAngle);

	POINT pt1 = m_MousePosLast;
	POINT pt2 = m_MousePosFirst;
	PtInControlWnd(m_MousePosLast, MODELEDIT_IMAGE_WND, pt1);
	PtInControlWnd(m_MousePosFirst, MODELEDIT_IMAGE_WND, pt2);
	if ( false == IsExceptionAngle )
	{
		::MoveToEx(hDC, pt1.x, pt1.y, NULL);
		::LineTo(hDC, pt2.x, pt1.y);
		::LineTo(hDC, pt2.x, pt2.y);
		::LineTo(hDC, pt1.x, pt2.y);
		::LineTo(hDC, pt1.x, pt1.y);
	}
	else
	{		
		TPOINT2D Cp, dPt1, dPt2;		
		TPOINT2D CornerPoint[4];
		const double ImageAngle = JetAPI::MapCadAngleToImageAngle(ComponentAngle);
		dPt1 = pt1;
		dPt2 = pt2;		
		Cp.x = (dPt1.x+dPt2.x)*0.5;
		Cp.y = (dPt1.y+dPt2.y)*0.5;		
		JetAPI::RotatePos(-ImageAngle, Cp.x, Cp.y, dPt1);
		JetAPI::RotatePos(-ImageAngle, Cp.x, Cp.y, dPt2);
		CornerPoint[0].x = dPt1.x;	CornerPoint[0].y = dPt1.y;
		CornerPoint[1].x = dPt2.x;	CornerPoint[1].y = dPt1.y;
		CornerPoint[2].x = dPt2.x;	CornerPoint[2].y = dPt2.y;
		CornerPoint[3].x = dPt1.x;	CornerPoint[3].y = dPt2.y;		
		JetAPI::RotateCornerPos(ImageAngle, Cp.x, Cp.y, CornerPoint);
		ImageAPI.DrawPolyLine(hDC, CornerPoint, 4);
	}

	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPen);	hPen = NULL;
}
//-------------------------------------------------------------------------------------//
bool CModelWnd::CreateShowBuffer()
{
	const char fnName[] = "CModelWnd::CreateBuffer";
	ReleaseShowImageBuffer();

	m_ImageW = m_UniFrameList[0].ImageW;
	m_ImageH = m_UniFrameList[0].ImageH;
	m_ImageStep = m_UniFrameList[0].ImageStep;
	m_BitCount = m_UniFrameList[0].BitCount;	

	IMAGE_PTR  Ptr = NULL;
	IMAGE_SIZE BitCount = 24;//直接開最大的
	IMAGE_SIZE ImageStep = JetAPI::GetBMPImagePixelsPerLine(m_ImageW, BitCount, 4);
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, m_ImageH);
	if ( JetMemory.alloc_func(BufferSize, Ptr, fnName, "Ptr") == false )
	{	return false; }
	::memset(Ptr, 0x00, sizeof(IMAGE_DATA)*BufferSize);
	m_ShowImagePtr = Ptr;	
	m_ShowBufferSize = BufferSize;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CModelWnd::ReleaseShowImageBuffer()
{
	JetMemory.free_func(m_ShowImagePtr);	 
	m_ShowBufferSize = 0;;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CModelWnd::UpdateFrameImage()
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr )	{	return false;	}		
	m_ImageIndex = ProjectPtr->GetProjectMapIndex();		
	BuildShowImageBuffer();
	CModelWnd::CreateBKImage();
	CModelWnd::RedrawWnd();
	//m_ViewModelImage.SendMessage(MSG_EDIT_VIEW_3D_WND, WPARAM_UPDATE_3D_DATA, NULL);	
	m_ViewModelImage.SendMessage(MSG_EDIT_IMAGE_VIEW_WND, WPARAM_UPDATE_IMAGE_SWITCH_FRAME, NULL);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CModelWnd::BuildShowImageBuffer()
{
	unsigned int Index = m_ImageIndex;
	const size_t UniFrameCount = FRAME_MAX_COUNT;
	if ( Index >= UniFrameCount ) { return false; }
	TUNI_FRAME UniFrame = m_UniFrameList[Index];

	if ( NULL == UniFrame.ImagePtr ) { return false; }
	if ( NULL == m_ShowImagePtr ) { return false; }

	m_ImageW = UniFrame.ImageW;
	m_ImageH = UniFrame.ImageH;
	m_ImageStep = UniFrame.ImageStep;
	m_BitCount = UniFrame.BitCount;	
	DRAW_IMAGE_MODE DrawImageMode = AOIDataCollect.GetDrawImageMode();

	if ( DRAW_IMAGE_BY_RAW != DrawImageMode )
	{
		AOIDataCollect.SetDrawingImageMode(DRAW_IMAGE_NORMAL);
		AOIDataCollect.ExecEnhanceDisplayImage(m_ImageW, m_ImageH, m_ImageStep, m_BitCount, UniFrame.ImagePtr, m_ShowImagePtr);
	}
	else
	{	AOIDataCollect.SetDrawingImageMode(DRAW_IMAGE_BY_RAW);	}

	m_ShowImageW = m_ImageW;
	m_ShowImageH = m_ImageH;
	m_ShowImageStep = m_ImageStep;
	m_ShowBitCount = m_BitCount;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CModelWnd::SwitchFrameImage()
{	
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }
	m_ImageIndex = ProjectPtr->GetProjectMapIndexNext(m_ImageIndex);
	ProjectPtr->SetProjectMapIndex(m_ImageIndex);
	
	BuildShowImageBuffer();
	CreateBKImage();
	RedrawWnd();	
	//m_ViewModelImage.PostMessage(MSG_EDIT_VIEW_3D_WND, WPARAM_UPDATE_3D_DATA, NULL);
	m_ViewModelImage.SendMessage(MSG_EDIT_IMAGE_VIEW_WND, WPARAM_UPDATE_IMAGE_SWITCH_FRAME, NULL);	
	return true;
}
//-------------------------------------------------------------------------------------//
void CModelWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	POINT pt2={0};
	if ( PtInControlWnd(point, MODELEDIT_IMAGE_WND, pt2) == false )
	{
		CBaseDialog::OnLButtonDown(nFlags, point);
		return;
	}
	CWnd::SetCapture();
	m_MousePosImageWnd = m_MousePosCurrent = m_MousePosFirst = m_MousePosLast = point;		
	m_DrawAddRect = true;	
	CheckActiveObjFocus(m_ActiveObj, m_ActiveBox);
	CBaseDialog::OnLButtonDown(nFlags, point);
}
//-------------------------------------------------------------------------------------//
bool CModelWnd::ExecSaveLogLButtonUp()
{
	if ( NULL == m_ActiveObj.BoxPtr ) { return true; }
	TActiveObj ActiveObj;			
	CheckActiveObjFocus(ActiveObj);	
	if ( m_ActiveObj.BoxPtr != ActiveObj.BoxPtr )
	{
		m_ActiveObj = TActiveObj();
		return true; 
	}

	double dPx=0, dPy=0;
	TREGION4D ModifyRgn;
	//DYNAMIC_DOWNCAST(CAOIBox, ObjPtr->BoxPtr);
	CAOIBox   *BoxPtr = DYNAMIC_DOWNCAST(CAOIBox, ActiveObj.BoxPtr);	
	CAOIWnd   *WndPtr = DYNAMIC_DOWNCAST(CAOIWnd, ActiveObj.WndPtr);
	CAOILand  *LandPtr = DYNAMIC_DOWNCAST(CAOILand, ActiveObj.LandPtr);
	CAOIModel *ModelPtr = DYNAMIC_DOWNCAST(CAOIModel, ActiveObj.ModelPtr);
	CAOIWndRoi *WndRoiPtr = DYNAMIC_DOWNCAST(CAOIWndRoi, ActiveObj.WndRoiPtr);
	CAOIWndMask *WndMaskPtr = DYNAMIC_DOWNCAST(CAOIWndMask, ActiveObj.WndMaskPtr);
	if ( NULL == BoxPtr ) { return true; }

	switch ( m_MousePosMode )
	{
	case CURSOR_POS_INNER://Pos
		dPx = BoxPtr->GetBoxPosX()-m_ActiveBox.GetBoxPosX();
		dPy = BoxPtr->GetBoxPosY()-m_ActiveBox.GetBoxPosY();
		LogOperCtrl.SaveLogModelModifyPos(ModelPtr, BoxPtr, WndPtr, LandPtr, WndRoiPtr, WndMaskPtr, dPx, dPy);
		break;
	case CURSOR_POS_LEFT:
	case CURSOR_POS_RIGHT:
	case CURSOR_POS_TOP:
	case CURSOR_POS_BOTTOM:
	case CURSOR_POS_LEFT_TOP:
	case CURSOR_POS_LEFT_BOTTOM:
	case CURSOR_POS_RIGHT_TOP:
	case CURSOR_POS_RIGHT_BOTTOM://Size
		ModifyRgn.minX = 0;
		ModifyRgn.minY = 0;
		ModifyRgn.maxX = BoxPtr->GetBoxSizeX()-m_ActiveBox.GetBoxSizeX();
		ModifyRgn.maxY = BoxPtr->GetBoxSizeY()-m_ActiveBox.GetBoxSizeY();
		LogOperCtrl.SaveLogModelModifySize(ModelPtr, BoxPtr, WndPtr, LandPtr, WndRoiPtr, WndMaskPtr, ModifyRgn);
		break;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CModelWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	::ReleaseCapture();
	m_MousePosLast = m_MousePosCurrent;	
	m_MousePosImageWnd = m_MousePosCurrent = point;

	MANIPULATE_MODEL_MODE ManiMode = GetManiModelMode();
	if ( MANIPULATE_MODEL_ADD == ManiMode )
	{
		/*
		POINT dPt;
		dPt.x = ::abs(m_MousePosLast.x-m_MousePosFirst.x);
		dPt.y = ::abs(m_MousePosLast.y-m_MousePosFirst.y);
		CAOIModel *ModelPtr = GetModelPtr();
		if ( NULL != ModelPtr )
		{
			if ( dPt.x>2 && dPt.y>2 )
			{	
				LAND_TYPE LandType = ModelPtr->GetModelLandTypeMaster();
				CAOIBox  *BoxPtr = ModelPtr->GetModelBodyBoxPtr();
				CAOIWnd  *WndPtr = ModelPtr->GetModelWndActived();
				CAOILand *LandPtr = ModelPtr->GetModelLandActivted();
				const int LandTypeCount = ModelPtr->GetModelSupportLandTypeCount();
				if ( NULL==LandPtr && BoxPtr->GetBoxSelected()==false && NULL==WndPtr )
				{				
					if ( 1 == LandTypeCount )
					{	ExecModelAddLand(LandType);	}
					else if ( 0==LandTypeCount || 1<LandTypeCount )
					{
						POINT MenuPt = point;
						CWnd::ClientToScreen(&MenuPt);
						CModelWnd::ExecPopupMenu(MenuPt, IDR_MENU_MODEL_ADD); 
					}
				}
				else
				{	CModelWnd::ExecModelAddWnd();	}
				m_DrawAddRect = false;
			}
			else
			{
				m_DrawAddRect = false;
				ExecModelRegionSelect();
				RedrawWnd();
			}
		}
		*/
	}
	else if ( MANIPULATE_MODEL_SELECT==ManiMode || MANIPULATE_MODEL_EDIT==ManiMode )
	{
		m_DrawAddRect = false;
		bool m_ChangeComponentSelected = false;
		if ( false==m_ChangeComponentSelected && CURSOR_POS_NONE==m_MousePosMode )
		{	
			//ExecModelRegionSelect();
			//ExecModelWndInspection();
			//UpdateImageByAlgParam();			
		}
		else if ( CURSOR_POS_NONE!=m_MousePosMode )
		{
			ExecSaveLogLButtonUp();
			ExecModelWndInspection(true);
			UpdateImageByAlgParam();			
		}
		RedrawWnd();
	}
	else if ( MANIPULATE_MODEL_GATHER_COLOR == ManiMode )
	{
		m_DrawAddRect = false;
		const bool CombineColorMode = AOIDataCollect.CheckCombineColorMode();
		ExecGatherColorFilter(CombineColorMode);
		if ( false == CombineColorMode ) 		
		{	AOIDataCollect.CancelGatherColorMode();	 }
		RedrawWnd();
	}
	CBaseDialog::OnLButtonUp(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CModelWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	POINT dPoint;
	m_MousePosLast = m_MousePosCurrent;	
	m_MousePosImageWnd = m_MousePosCurrent = point;
	//CWnd::MapWindowPoints(&m_ImageWnd, &m_MousePosImageWnd, 1);
	dPoint.x = point.x - m_MousePosLast.x;
	dPoint.y = point.y - m_MousePosLast.y;
	if ( this != CWnd::GetCapture() )
	{
		CModelWnd::RedrawWnd();
		CBaseDialog::OnMouseMove(nFlags, point);
		return;
	}

	//JetAPI::UpdateCursor();
	if ( nFlags & MK_LBUTTON )//滑鼠左鍵
	{		
		bool Modify = false;
		MANIPULATE_MODEL_MODE ManiMode = GetManiModelMode();
		if ( MANIPULATE_MODEL_ADD == ManiMode )
		{	Modify = true;	}
		else 
		{
			switch ( m_MousePosMode )
			{
			case CURSOR_POS_INNER:
				Modify = ExecModifyActiveObjPos();
				break;
			case CURSOR_POS_LEFT:
			case CURSOR_POS_RIGHT:
			case CURSOR_POS_TOP:
			case CURSOR_POS_BOTTOM:
			case CURSOR_POS_LEFT_TOP:
			case CURSOR_POS_LEFT_BOTTOM:
			case CURSOR_POS_RIGHT_TOP:
			case CURSOR_POS_RIGHT_BOTTOM:
				Modify = ExecModifyActiveObjSize(m_MousePosMode);
				break;
			}
		}
		if ( MANIPULATE_MODEL_SELECT == ManiMode )
		{	Modify = true;	}
		if ( MANIPULATE_MODEL_GATHER_COLOR == ManiMode )
		{	Modify = true;	}

		Modify = true;
		if ( true == Modify )
		{	RedrawWnd();	}	
	}
	else if (nFlags & MK_RBUTTON )//滑鼠右鍵
	{
		this->m_ImageOffset.x += dPoint.x;
		this->m_ImageOffset.y += dPoint.y;
		CreateBKImage();
		RedrawWnd();
	}	
	CBaseDialog::OnMouseMove(nFlags, point);
}
//-------------------------------------------------------------------------------------//
BOOL CModelWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	if ( ::PtInRect(&m_ImageWndRect, pt) == FALSE )
	{	return CBaseDialog::OnMouseWheel(nFlags, zDelta, pt); }

	double NextImageZoom = m_ImageZoom;
	if ( zDelta > 0 ) 
	{	NextImageZoom *= 1.1;	}
	else
	{	NextImageZoom /= 1.1;	}
	NextImageZoom = AOIDataCollect.AdjustImageZoom(NextImageZoom);
	ImageAPI.CalcImageWndZoom(m_ImageZoom, NextImageZoom, m_ImageOffset);
	m_ImageZoom = NextImageZoom;
	CreateBKImage();
	RedrawWnd();
	return CBaseDialog::OnMouseWheel(nFlags, zDelta, pt);
}
//-------------------------------------------------------------------------------------//
void CModelWnd::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	POINT pt2={0};	
	if ( PtInControlWnd(point, MODELEDIT_IMAGE_WND, pt2) == false )
	{
		CBaseDialog::OnRButtonDown(nFlags, point);
		return;
	}	
	CWnd::SetFocus();
	CWnd::SetCapture();
	m_MousePosImageWnd = m_MousePosCurrent = m_MousePosFirst = m_MousePosLast = point;
	CBaseDialog::OnRButtonDown(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CModelWnd::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	::ReleaseCapture();	
	m_MousePosImageWnd = m_MousePosCurrent = point;
	CBaseDialog::OnRButtonUp(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CModelWnd::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	const UINT CtrlID = pWnd->GetDlgCtrlID();
	POINT dPos, LastPos;
	//不要用m_MousePosLast, 因為可能收不到OnMouseMove

	
	LastPos = point;
	CWnd::ScreenToClient(&LastPos);
	dPos.x = LastPos.x - m_MousePosFirst.x;
	dPos.y = LastPos.y - m_MousePosFirst.y;

	if ( ::abs(dPos.x)>2 || ::abs(dPos.y)>2 )
	{	return; }
	SwitchFrameImage();	
}
//-------------------------------------------------------------------------------------//
bool CModelWnd::ExecShowWndPosition()
{
	const double PosX = AOIDataCollect.GetFovPositionX();
	const double PosY = AOIDataCollect.GetFovPositionY();	
	const double TargetOffsetX = AOIDataCollect.GetFovTargetOffsetX();
	const double TargetOffsetY = AOIDataCollect.GetFovTargetOffsetY();

	AOIDataCollect.ResetFovTargetParam();
	
	TPOINT2D PosCad;
	TPOINT2D PosStage(TargetOffsetX, TargetOffsetY);	
	TPOINT2D ImageRes = m_ImageResolution;
	
	AOIDataCollect.MapStageOffsetPtToCad(PosStage, PosCad);		
	m_ImageOffset.x = -PosCad.x/(ImageRes.x);
	m_ImageOffset.y =  PosCad.y/(ImageRes.y);
	m_ImageOffset.x = m_ImageOffset.x/(m_ImageZoom);
	m_ImageOffset.y = m_ImageOffset.y/(m_ImageZoom);
	
	BuildShowImageBuffer();
	CreateBKImage();
	RedrawWnd();	
	return true;
}
//-------------------------------------------------------------------------------------//
void CModelWnd::UpdateActiveObjList()
{
	CString     str;
	size_t      i=0;
	TRECT4D     Rect;	
	TPOINT2D    CornPoint[4];
	TPOINT2D    CornerPoint[4];	
	CAOIBox    *BoxPtr   = NULL;
	TActiveObj *ObjPtr = NULL;		
	const TPOINT2D Res = m_ImageResolution;
	const TPOINT2D StageCp = m_ModelImagePosStage;		
	const size_t NObjects = this->m_ActiveObjList.size();
	const IMAGE_SIZE ImageW = GetFrameImageW();
	const IMAGE_SIZE ImageH = GetFrameImageH();	
	const DRAW_MODEL_MODE DrawModelMode = AOIDataCollect.GetDrawModelMode();
	for ( i=0; i<NObjects; i++ )
	{
		ObjPtr = &(m_ActiveObjList[i]);
		if ( NULL == ObjPtr ) { continue; }
		if ( NULL == ObjPtr->BoxPtr ) { continue; }
		BoxPtr = (CAOIBox*)(ObjPtr->BoxPtr);

		if ( DRAW_MODEL_RESULT == DrawModelMode )
		{	BoxPtr->GetBoxCornerPosStageRes(CornerPoint);	}
		else
		{	BoxPtr->GetBoxCornerPosStage(CornerPoint); }		
		AOIDataCollect.MapStageCornerToCamera(ImageW, ImageH, Res, CornerPoint, StageCp, CornPoint);//機台4端點對應到影像四點
		JetAPI::PointsToRect(CornPoint, 4, Rect);
		ObjPtr->Rect       = Rect;
		ObjPtr->CornerPts[0] = CornPoint[0];
		ObjPtr->CornerPts[1] = CornPoint[1];
		ObjPtr->CornerPts[2] = CornPoint[2];
		ObjPtr->CornerPts[3] = CornPoint[3];		
	}
}
//-------------------------------------------------------------------------------------//
void CModelWnd::BuildActiveObjList(CAOIModel *ModelPtr, bool ActiveOnly)
{
	bool CheckAddObj = false;
//	if ( AOIDataCollect.GetIsPressVRKey(m_MultiSelKey) == false )
//	{	CheckAddObj = false;}
//	else
//	{	CheckAddObj = true;	}
	this->m_ActiveObjList.clear();
	if ( NULL == ModelPtr ) { return; }

	CString      str;	
	size_t       i=0, j=0, k=0, s=0;
	size_t       LandCount = 0;	
	size_t       WndCount  = 0;
	size_t       BoxWndCount = 0;		
	size_t       WndRoiCount = 0;
	size_t       MaskWndCount = 0;
	CAOIBox     *BoxPtr   = NULL;
	CAOIWnd     *WndPtr   = NULL;
	CAOILand    *LandPtr  = NULL;
	CAOIWndRoi  *WndRoiPtr  = NULL;
	CAOIWndMask *MaskWndPtr  = NULL;
	TActiveObj   ActiveObj;
	const double ComponentAngle = ModelPtr->GetModelAttachedAngle();
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComponentAngle);

	bool ModelActiveOnly = true;	
	bool AddModelRect    = true;
	bool bBoxWndSelected = false;
	bool bWndBoxWndSelected = false;
	bool bLandBoxWndSelected = false;	
	bool bWndRoiBoxSelected = false;
	bool bMaskBoxSelected = false;

	ActiveObj.ModelPtr = ModelPtr;	
	ModelActiveOnly = TRUE;
	
	ActiveObj = TActiveObj();
	ActiveObj.ComponentAngle = ComponentAngle;
	ActiveObj.IsExceptionAngle = IsExceptionAngle;	
	ActiveObj.WndPtr     = NULL;
	ActiveObj.LandPtr    = NULL;
	ActiveObj.BoxPtr     = NULL;
	ActiveObj.WndRoiPtr  = NULL;
	ActiveObj.WndMaskPtr = NULL;
	ActiveObj.SetEditabled(true);
	ActiveObj.PassObj    = false;	

	//if ( false == ActiveOnly ) 
	{		
		WndCount = ModelPtr->GetModelWndCount();			
		for ( j=0; j<WndCount; j++ )
		{
			WndPtr = ModelPtr->GetModelWndPtr(j, false);				
			if ( NULL == WndPtr ) { continue; }
			
			BoxPtr = WndPtr->GetWndBoxPtr();			
			if ( NULL == BoxPtr ) { continue; }
			//if ( BoxPtr->GetBoxEnabled() == false ) { continue; }
			if ( BoxPtr->GetBoxVisibled() == false ) 
			{ 
				BoxPtr->SetBoxSelected(false);
				continue; 
			}			
			if ( true == ActiveOnly )
			{
				if ( BoxPtr->GetBoxSelected() == false ) { continue; }
			}
			//子框
			bWndRoiBoxSelected = false;
			WndRoiCount = WndPtr->GetWndRoiWndCount();
			for ( k=0; k<WndRoiCount; k++ )
			{
				WndRoiPtr = WndPtr->GetWndRoiWndPtr(k, false);
				if ( NULL == WndRoiPtr ) { continue; }
				BoxPtr = WndRoiPtr->GetWndRoiBoxPtr();
				if ( NULL == BoxPtr ) { continue; }
				if ( BoxPtr->GetBoxSelected() == true ) 
				{	bWndRoiBoxSelected = true; }

				ActiveObj.ModelPtr   = ModelPtr;
				ActiveObj.WndPtr     = WndPtr;					
				ActiveObj.LandPtr    = NULL;
				ActiveObj.WndRoiPtr  = WndRoiPtr;
				ActiveObj.WndMaskPtr = NULL;
				ActiveObj.BoxPtr     = BoxPtr;				
				ActiveObj.SetEditabled(BoxPtr->GetBoxEditabled());
				ActiveObj.SetSelected(BoxPtr->GetBoxSelected());
				ActiveObj.SetFocused(BoxPtr->GetBoxActived());
				this->AddActiveObject(ActiveObj, CheckAddObj);
			}
			//遮罩框
			bMaskBoxSelected = false;
			MaskWndCount = WndPtr->GetWndMaskWndCount();
			for ( k=0; k<MaskWndCount; k++ )
			{
				MaskWndPtr = WndPtr->GetWndMaskWndPtr(k, false);
				if ( NULL == MaskWndPtr ) { continue; }
				BoxPtr = MaskWndPtr->GetWndMaskBoxPtr();
				if ( NULL == BoxPtr ) { continue; }
				if ( BoxPtr->GetBoxSelected() == true ) 
				{	bMaskBoxSelected = true; }

				ActiveObj.ModelPtr   = ModelPtr;
				ActiveObj.WndPtr     = WndPtr;					
				ActiveObj.LandPtr    = NULL;
				ActiveObj.WndRoiPtr  = NULL;
				ActiveObj.WndMaskPtr = MaskWndPtr;
				ActiveObj.BoxPtr     = BoxPtr;				
				ActiveObj.SetEditabled(BoxPtr->GetBoxEditabled());
				ActiveObj.SetSelected(BoxPtr->GetBoxSelected());
				ActiveObj.SetFocused(BoxPtr->GetBoxActived());
				this->AddActiveObject(ActiveObj, CheckAddObj);
			}			

			BoxPtr = WndPtr->GetWndBoxPtr();	
			ModelActiveOnly = false;
			ActiveObj.ModelPtr   = ModelPtr;
			ActiveObj.WndPtr     = WndPtr;					
			ActiveObj.LandPtr    = NULL;
			ActiveObj.WndRoiPtr  = NULL;
			ActiveObj.WndMaskPtr = NULL;
			ActiveObj.BoxPtr     = BoxPtr;
			ActiveObj.SetEditabled(BoxPtr->GetBoxEditabled());
			ActiveObj.SetSelected(BoxPtr->GetBoxSelected());
			ActiveObj.SetFocused(BoxPtr->GetBoxActived());
			if ( true==bWndRoiBoxSelected || true==bMaskBoxSelected )
			{	
				ActiveObj.SetSelected(false);
				ActiveObj.SetFocused(false);
			}
			this->AddActiveObject(ActiveObj, CheckAddObj);				
		}
	}	

	//if ( false == ActiveOnly ) 
	{
		LandCount = ModelPtr->GetModelLandCount();
		for ( j=0; j<LandCount; j++ )
		{
			LandPtr = ModelPtr->GetModelLandPtr(j, false);			
			if ( NULL == LandPtr ) { continue; }
			for ( k=0; k<5; k++ )
			{				
				switch ( k )
				{				
				case 0:	BoxPtr = LandPtr->GetLandLeadBoxPtr();	break;
				case 1:	BoxPtr = LandPtr->GetLandPadBoxPtr();	break;
				case 2:	BoxPtr = LandPtr->GetLandLeadShoulderBoxPtr();	break;
				case 3:	BoxPtr = LandPtr->GetLandLeadTipBoxPtr();	break;
				case 4:	BoxPtr = LandPtr->GetLandBodyEdgeBoxPtr();	break;
				default:	BoxPtr = NULL;	break;
				}				
				if ( BoxPtr == NULL ) { continue; }
				if ( NULL == BoxPtr ) { continue; }
				if ( BoxPtr->GetBoxEnabled() == false ) { continue; }
				if ( BoxPtr->GetBoxVisibled() == false ) { continue; }				
				if ( true == ActiveOnly )
				{
					if ( BoxPtr->GetBoxSelected() == false ) { continue; }
				}
				ModelActiveOnly = false;
				ActiveObj.ModelPtr   = ModelPtr;
				ActiveObj.WndPtr     = NULL;
				ActiveObj.WndRoiPtr  = NULL;
				ActiveObj.WndMaskPtr = NULL;
				ActiveObj.LandPtr    = LandPtr;
				ActiveObj.BoxPtr     = BoxPtr;
				ActiveObj.SetEditabled(BoxPtr->GetBoxEditabled());
				ActiveObj.SetSelected(BoxPtr->GetBoxSelected());
				ActiveObj.SetFocused(BoxPtr->GetBoxActived());
				this->AddActiveObject(ActiveObj, CheckAddObj);
			}
		}
	}
	
	AddModelRect    = true;
	if ( true == ActiveOnly )
	{
		if ( (ModelPtr->GetModelBodyBox().GetBoxSelected()==false) || (false==ModelActiveOnly) )
		{	AddModelRect = false;	}
	}		

	if ( true == AddModelRect )
	{
		BoxPtr = ModelPtr->GetModelBodyBoxPtr();
		ActiveObj.ModelPtr   = ModelPtr;		
		ActiveObj.LandPtr    = NULL;
		ActiveObj.WndRoiPtr  = NULL;
		ActiveObj.WndMaskPtr = NULL;
		ActiveObj.BoxPtr     = BoxPtr;
		ActiveObj.WndPtr     = NULL;		
		ActiveObj.ComponentAngle = ComponentAngle;		
		ActiveObj.SetEditabled(BoxPtr->GetBoxEditabled());
		ActiveObj.SetSelected(BoxPtr->GetBoxSelected());
		ActiveObj.SetFocused(BoxPtr->GetBoxActived());
		AddActiveObject(ActiveObj, CheckAddObj);
	}	
	UpdateActiveObjList();
	return ;
}
//-------------------------------------------------------------------------------------//
void CModelWnd::AddActiveObject(const TActiveObj &ActiveObj, bool Check)
{
	if ( Check == true ) 
	{
		size_t i = 0;
		TActiveObj   *ActiveObjPtr = NULL;
		size_t size = this->m_ActiveObjList.size();

		if ( size > 0 ) 
		{
			for ( i=0; i<size; i++ )
			{
				ActiveObjPtr = &(m_ActiveObjList[i]);
				if ( ActiveObjPtr->ModelPtr != ActiveObj.ModelPtr ) 
				{	break;  }
				if ( ActiveObjPtr->BoxPtr != ActiveObj.BoxPtr ) 
				{	break;  }
				if ( ActiveObjPtr->WndPtr != ActiveObj.WndPtr ) 
				{	break;  }
				if ( ActiveObjPtr->LandPtr != ActiveObj.LandPtr ) 
				{	break;  }				
				if ( ActiveObjPtr->WndRoiPtr != ActiveObj.WndRoiPtr ) 
				{	break;  }
				if ( ActiveObjPtr->WndMaskPtr != ActiveObj.WndMaskPtr ) 
				{	break;  }				
			}		
			if ( i == size ) { return; }
		}
	}	

	if ( ActiveObj.ComponentAngle < -1000 )
	{
		Check = Check;
	}
	this->m_ActiveObjList.push_back(ActiveObj);	
}
//-------------------------------------------------------------------------------------//
CURSOR_POS_MODE CModelWnd::CheckCursorPosMode(POINT pt)//確認鼠標座標模式
{
	RECT         Rect;
	TRECT4D      dRect;
	SIZE         szGrid;
	double       Angle = 0;
	size_t       i = 0;	
	CAOIBox      *BoxPtr = NULL;
	CURSOR_POS_MODE CursorMode = CURSOR_POS_NONE;	
	TActiveObj  *ObjPtr = NULL;	
	TActiveObj  *ObjPtrActived = NULL;	
	TPOINT2D     Cp;
	TPOINT2D     WndPt = pt;
	TPOINT2D     ImagePt, ImagePt2;
	TPOINT2D     CornerPoint[4];	
	POINT        ImagePoint;	
	double       ZoomScale = m_ImageZoom;		
	CAOIModel   *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) 
	{	return CursorMode; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return CursorMode; }	
	if ( ModelPtr->GetModelEditMode() == false ) { return CursorMode; }	
	DRAW_MODEL_MODE DrawModelMode = AOIDataCollect.GetDrawModelMode();
	if ( DRAW_MODEL_EDIT != DrawModelMode ) 
	{	return CursorMode; }
	MANIPULATE_MODEL_MODE ManiMode = GetManiModelMode();
	const bool MultiSelectMode = AOIDataCollect.CheckMultiSelectMode();	
	const IMAGE_SIZE ImageW = GetFrameImageW();
	const IMAGE_SIZE ImageH = GetFrameImageH();	
	const size_t NObjects = this->m_ActiveObjList.size();	

	szGrid.cx = GetEditCheckSize();
	szGrid.cy = GetEditCheckSize();	
	ImageAPI.MapWndPtToImagePt_DBL(ImageW, ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, WndPt, ImagePt);	
	Cp.x = Cp.y = 0;
	if ( MANIPULATE_MODEL_EDIT == ManiMode )
	{
		for ( i=0; i<NObjects; i++ )
		{
			ObjPtr = &(m_ActiveObjList[i]);
			if ( false == ObjPtr->GetEditabled() ) { continue; }
			if ( false == ObjPtr->GetSelected() ) { continue; }
			//if ( false == ObjPtr->GetFocused() ) { continue; }			

			BoxPtr = (CAOIBox*)ObjPtr->BoxPtr;
			if ( ObjPtr->IsExceptionAngle == false )
			{	
				JetAPI::Point2DToPoint(ImagePt, ImagePoint);
				JetAPI::Rect4DToRect(ObjPtr->Rect, Rect);				
			}
			else
			{
				Angle = JetAPI::MapCadAngleToImageAngle(ObjPtr->ComponentAngle);
				ImagePt2 = ImagePt;
				CornerPoint[0] = ObjPtr->CornerPts[0];
				CornerPoint[1] = ObjPtr->CornerPts[1];
				CornerPoint[2] = ObjPtr->CornerPts[2];
				CornerPoint[3] = ObjPtr->CornerPts[3];				
				JetAPI::RotateCornerPos(-Angle, Cp.x, Cp.y, CornerPoint);
				JetAPI::RotatePos(-Angle, Cp.x, Cp.y, ImagePt2);
				JetAPI::PointsToRect(CornerPoint, 4, dRect);
				
				JetAPI::Point2DToPoint(ImagePt2, ImagePoint);
				JetAPI::Rect4DToRect(dRect, Rect);				
			}

			CursorMode = JetAPI::CheckCursorPosMode(Rect, szGrid, ImagePoint);
			if( CURSOR_POS_NONE != CursorMode ) 
			{	break;	}
		}
	}
	else if ( MANIPULATE_MODEL_SELECT == ManiMode )
	{	
		ObjPtrActived = NULL;	
		for ( i=0; i<NObjects; i++ )
		{
			ObjPtr = &(m_ActiveObjList[i]);
			if ( false == ObjPtr->GetEditabled() ) { continue; }

			BoxPtr = (CAOIBox*)ObjPtr->BoxPtr;			
			if ( ObjPtr->IsExceptionAngle == false )
			{	
				JetAPI::Point2DToPoint(ImagePt, ImagePoint);
				JetAPI::Rect4DToRect(ObjPtr->Rect, Rect);				
			}
			else
			{
				Angle = JetAPI::MapCadAngleToImageAngle(ObjPtr->ComponentAngle);
				ImagePt2 = ImagePt;
				CornerPoint[0] = ObjPtr->CornerPts[0];
				CornerPoint[1] = ObjPtr->CornerPts[1];
				CornerPoint[2] = ObjPtr->CornerPts[2];
				CornerPoint[3] = ObjPtr->CornerPts[3];
				JetAPI::RotateCornerPos(-Angle, Cp.x, Cp.y, CornerPoint);
				JetAPI::RotatePos(-Angle, Cp.x, Cp.y, ImagePt2);
				JetAPI::PointsToRect(CornerPoint, 4, dRect);

				JetAPI::Point2DToPoint(ImagePt2, ImagePoint);				
				JetAPI::Rect4DToRect(dRect, Rect);				
			}

			CursorMode = JetAPI::CheckCursorPosMode(Rect, szGrid, ImagePoint);
			if( CURSOR_POS_NONE != CursorMode ) 
			{	
				if ( false == MultiSelectMode )
				{
					ModelPtr->UnSelectModel();
					ModelPtr->SetModelWndActived(NULL);
					ModelPtr->SetModelLandActived(NULL);
					ProjectPtr->SetProjectActiveModelWnd(NULL);
					ProjectPtr->SetProjectActiveModelLand(NULL);					
					ResetActiveObjPosSelect();
				}
				ObjPtrActived = ObjPtr;
				ObjPtr->SetFocused(true);
				ObjPtr->SetSelected(true);
				BoxPtr = (CAOIBox*)ObjPtr->BoxPtr;
				if ( NULL != BoxPtr )
				{	BoxPtr->SetBoxSelected(true);	}
				break; 
			}
		}
		if ( NULL!=ObjPtrActived && AOIDataCollect.CheckDoubleSideEdit() == false)
		{
			for ( i=0; i<NObjects; i++ )
			{
				ObjPtr = &(m_ActiveObjList[i]);
				if ( ObjPtr == ObjPtrActived ) { continue; }
				
				ObjPtr->SetFocused(false);
				BoxPtr = (CAOIBox*)ObjPtr->BoxPtr;
				if ( NULL != BoxPtr )
				{	BoxPtr->SetBoxSelected(false);	}				
			}
		}
	}	
	if ( CURSOR_POS_NONE != CursorMode )
	{
		if ( AOIDataCollect.CheckMoveObjectMode() == true )
		{	CursorMode = CURSOR_POS_INNER; }
	}
	return CursorMode;
}
//-------------------------------------------------------------------------------------//
void CModelWnd::ResetActiveObjPosFocus()
{
	size_t       i = 0;	
	TActiveObj  *ObjPtr = NULL;		
	const size_t NObjects = this->m_ActiveObjList.size();	
	for ( i=0; i<NObjects; i++ )
	{
		ObjPtr = &(m_ActiveObjList[i]);
		ObjPtr->SetFocused(false);		
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CModelWnd::ResetActiveObjPosSelect()
{
	size_t       i = 0;	
	TActiveObj  *ObjPtr = NULL;		
	const size_t NObjects = this->m_ActiveObjList.size();	
	for ( i=0; i<NObjects; i++ )
	{
		ObjPtr = &(m_ActiveObjList[i]);
		ObjPtr->SetFocused(false);
		ObjPtr->SetSelected(false);
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CModelWnd::CheckActiveObjFocus(TActiveObj &Obj)
{
	Obj = TActiveObj();
	if ( CURSOR_POS_NONE == m_MousePosMode )
	{	return; }
	const size_t NObjects = this->m_ActiveObjList.size();	
	for ( size_t i=0; i<NObjects; i++ )
	{
		const TActiveObj &ObjRef = m_ActiveObjList[i];
		if ( ObjRef.GetFocused() == false ) { continue; }
		Obj = ObjRef;
		break;
	}
}
//-------------------------------------------------------------------------------------//
void CModelWnd::CheckActiveObjFocus(TActiveObj &Obj, CAOIBox &Box)
{
	CheckActiveObjFocus(m_ActiveObj);	
	if ( NULL == m_ActiveObj.BoxPtr )
	{	
		Box = CAOIBox();	
		return;
	}
			
	CAOIBox *BoxPtr = DYNAMIC_DOWNCAST(CAOIBox, m_ActiveObj.BoxPtr);
	if ( NULL == BoxPtr )
	{	
		Box = CAOIBox();	
		return;
	}
	Box = *(BoxPtr);	
	return;	
}
//-------------------------------------------------------------------------------------//
bool CModelWnd::ExecModifyActiveObjPos()//執行選中物件的座標
{
	const int nWndPx = m_MousePosCurrent.x-m_MousePosLast.x;
	const int nWndPy = m_MousePosCurrent.y-m_MousePosLast.y;
	return ExecModifyActiveObjPosKernel(nWndPx, nWndPy);
}
//-------------------------------------------------------------------------------------//
bool CModelWnd::ExecModifyActiveObjPosKernel(int nWndPx, int nWndPy)//執行選中物件的座標
{
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return false; }
	DRAW_MODEL_MODE DrawModelMode = AOIDataCollect.GetDrawModelMode();
	if ( DRAW_MODEL_EDIT != DrawModelMode ) { return false; }
	if ( ModelPtr->GetModelEditMode() == false ) { return false; }	
	if ( (0==nWndPx) && (0==nWndPy) )	{	return false; }

	size_t      i = 0;	
	CAOIBox     *BoxPtr = NULL;
	CAOIWnd     *WndPtr = NULL;
	CAOILand    *LandPtr = NULL;
	CAOIWndRoi  *WndRoiPtr = NULL;
	CAOIWndMask *WndMaskPtr = NULL;
	TActiveObj  *ObjPtr = NULL;		
	const size_t NObjects = this->m_ActiveObjList.size();
	BoxPtr = NULL;
	for ( i=0; i<NObjects; i++ )
	{
		ObjPtr = &(m_ActiveObjList[i]);
		if ( false == ObjPtr->GetFocused() ) { continue; }
		if ( false == ObjPtr->GetEditabled() ) { continue; }
		BoxPtr = (CAOIBox*)ObjPtr->BoxPtr;
		if ( NULL != BoxPtr )
		{	break;	}
	}
	if ( NULL == BoxPtr ) { return false; }		
	

	const double dImagePx = nWndPx*m_ImageZoom;
	const double dImagePy = nWndPy*m_ImageZoom;
	const double ResX = m_ImageResolution.x;
	const double ResY = m_ImageResolution.y;
	const double dCadPx = dImagePx*ResX;
	const double dCadPy = -1*dImagePy*ResY;//Y軸反向		
	const int    LinkMode = ModelPtr->GetModelWndLinkMode();

	BoxPtr = DYNAMIC_DOWNCAST(CAOIBox, ObjPtr->BoxPtr);
	WndPtr = DYNAMIC_DOWNCAST(CAOIWnd, ObjPtr->WndPtr);
	LandPtr = DYNAMIC_DOWNCAST(CAOILand, ObjPtr->LandPtr);
	WndRoiPtr = DYNAMIC_DOWNCAST(CAOIWndRoi, ObjPtr->WndRoiPtr);
	WndMaskPtr = DYNAMIC_DOWNCAST(CAOIWndMask, ObjPtr->WndMaskPtr);

	ModelPtr->ModifyModelBoxPos(BoxPtr, WndPtr, LandPtr, WndRoiPtr, WndMaskPtr, dCadPx, dCadPy, LinkMode);
	UpdateActiveObjList();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CModelWnd::ExecModifyActiveObjSize(CURSOR_POS_MODE CursorMode)//執行選中物件的尺寸
{
	CAOIModel *ModelPtr = CModelWnd::GetModelPtr();
	if ( NULL == ModelPtr ) { return true; }
	DRAW_MODEL_MODE DrawModelMode = AOIDataCollect.GetDrawModelMode();
	if ( DRAW_MODEL_EDIT != DrawModelMode ) { return true; }
	if ( ModelPtr->GetModelEditMode() == false ) { return true; }	

	size_t      i = 0;	
	CAOIBox     *BoxPtr = NULL;
	CAOIWnd     *WndPtr = NULL;
	CAOILand    *LandPtr = NULL;
	CAOIWndRoi  *WndRoiPtr = NULL;
	CAOIWndMask *WndMaskPtr = NULL;
	TActiveObj  *ObjPtr = NULL;		
	const size_t NObjects = this->m_ActiveObjList.size();
	BoxPtr = NULL;
	for ( i=0; i<NObjects; i++ )
	{
		ObjPtr = &(m_ActiveObjList[i]);
		if ( false == ObjPtr->GetFocused() ) { continue; }
		if ( false == ObjPtr->GetEditabled() ) { continue; }
		BoxPtr = (CAOIBox*)ObjPtr->BoxPtr;
		if ( NULL != BoxPtr )
		{	break;	}
	}
	if ( NULL == BoxPtr ) { return false; }

	TREGION4D dRgn;	
	const double dWndPx = m_MousePosCurrent.x-m_MousePosLast.x;
	const double dWndPy = m_MousePosCurrent.y-m_MousePosLast.y;
	const double dImagePx = dWndPx*m_ImageZoom;
	const double dImagePy = dWndPy*m_ImageZoom;
	const double ResX = m_ImageResolution.x;
	const double ResY = m_ImageResolution.y;	
	const bool   DoubleSideEdit = AOIDataCollect.CheckDoubleSideEdit();		
	const int    LinkMode = ModelPtr->GetModelWndLinkMode();

	if ( true == ObjPtr->IsExceptionAngle )
	{
		double dRevPx=dImagePx*ResX;
		double dRevPy=dImagePy*ResY;		
		double Angle = JetAPI::MapCadAngleToImageAngle(ObjPtr->ComponentAngle);		
		JetAPI::RotatePos(-Angle, 0, 0, dRevPx, dRevPy);
		dRevPx =  dRevPx;
		dRevPy = -dRevPy;		
		JetAPI::CalcModifySizeRegion(CursorMode, DoubleSideEdit, dRevPx, dRevPy, dRgn);
	}
	else
	{
		const double dCadPx = dImagePx*ResX;
		const double dCadPy = -1*dImagePy*ResY;//Y軸反向	
		JetAPI::CalcModifySizeRegion(CursorMode, DoubleSideEdit, dCadPx, dCadPy, dRgn);	
	}
	
	BoxPtr = DYNAMIC_DOWNCAST(CAOIBox, ObjPtr->BoxPtr);
	WndPtr = DYNAMIC_DOWNCAST(CAOIWnd, ObjPtr->WndPtr);
	LandPtr = DYNAMIC_DOWNCAST(CAOILand, ObjPtr->LandPtr);
	WndRoiPtr = DYNAMIC_DOWNCAST(CAOIWndRoi, ObjPtr->WndRoiPtr);
	WndMaskPtr = DYNAMIC_DOWNCAST(CAOIWndMask, ObjPtr->WndMaskPtr);
	ModelPtr->ModifyModelBoxSize(BoxPtr, WndPtr, LandPtr, WndRoiPtr, WndMaskPtr, dRgn, LinkMode);
	UpdateActiveObjList();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CModelWnd::BuildModelUniFrameList(CAOIModel *ModelPtr, std::vector<TUNI_FRAME> &UniFrameList, int nAlign, bool bClone, bool bNoFilter)
{
	//if ( GetShowProjectMapMode() == true ) { return false; }
	const char fnName[] = "CModelWnd::BuildModelUniFrameList";
	char        varName[MAX_JET_PATH]="";
	bool        IsOK = true;
	size_t      i=0;
	IMAGE_SIZE  ImageStep=0;
	size_t      BufferSize=0;
	size_t      NewBufferSize=0;
	TUNI_FRAME  UniFrame;
	IMAGE_PTR   ImagePtrNew=NULL;
	MASK_PTR    MaskPtrNew=NULL;
	SPACE_PTR   SpacePtrNew=NULL;

	JetAPI::ClearUniFrameList(UniFrameList);
	const size_t UniFrameCount = GetMaxFrameCount();
	
	for ( i=0; i<UniFrameCount; i++ )
	{
		UniFrame = m_UniFrameList[i];
		if ( NULL == UniFrame.ImagePtr ) { continue; }
		BufferSize = ImageAPI.CalcBufferSize(UniFrame.ImageStep, UniFrame.ImageH);
		ImageStep = JetAPI::GetBMPImagePixelsPerLine(UniFrame.ImageW, UniFrame.BitCount, nAlign);
		NewBufferSize = ImageAPI.CalcBufferSize(ImageStep, UniFrame.ImageH);
		if ( true == bClone )
		{	
			ImagePtrNew = NULL;
			MaskPtrNew = NULL;
			SpacePtrNew = NULL;			
			if ( NULL != UniFrame.ImagePtr ) 
			{
				::sprintf(varName, "%s#%d", "ImagePtr", i+1);
				if ( JetMemory.alloc_func(BufferSize, ImagePtrNew, fnName, varName) == false ) 
				{	
					IsOK=false;
					JetMemory.free_func(ImagePtrNew);
					JetMemory.free_func(MaskPtrNew);
					JetMemory.free_func(SpacePtrNew);
					break;
				}
				ImageAPI.AlignImageBuffer3(UniFrame.ImageW, UniFrame.ImageH, UniFrame.ImageStep, UniFrame.BitCount, UniFrame.ImagePtr, ImageStep, ImagePtrNew, false);
			}

			if ( NULL != UniFrame.MaskPtr ) 
			{	
				::sprintf(varName, "%s#%d", "MaskPtr", i+1);
				if ( JetMemory.alloc_func(BufferSize, MaskPtrNew, fnName, varName) == false ) 
				{
					IsOK=false;
					JetMemory.free_func(ImagePtrNew);
					JetMemory.free_func(MaskPtrNew);
					JetMemory.free_func(SpacePtrNew);
					break;
				}
				//::memcpy(MaskPtrNew, UniFrame.MaskPtr, sizeof(MASK_DATA)*BufferSize);
				ImageAPI.AlignImageBuffer3(UniFrame.ImageW, UniFrame.ImageH, UniFrame.ImageStep, UniFrame.BitCount, UniFrame.MaskPtr, ImageStep, MaskPtrNew, false);
			}

			if ( NULL != UniFrame.SpacePtr ) 
			{	
				::sprintf(varName, "%s#%d", "SpacePtr", i+1);
				if ( JetMemory.alloc_func(BufferSize, SpacePtrNew, fnName, varName) == false ) 
				{
					IsOK=false;
					JetMemory.free_func(ImagePtrNew);
					JetMemory.free_func(MaskPtrNew);
					JetMemory.free_func(SpacePtrNew);
					break;
				}
				//::memcpy(SpacePtrNew, UniFrame.SpacePtr, sizeof(SPACE_DATA)*BufferSize);
				ImageAPI.AlignSpaceImageBuffer3(UniFrame.ImageW, UniFrame.ImageH, UniFrame.ImageStep, UniFrame.SpacePtr, ImageStep, SpacePtrNew, false);
			}			
			UniFrame.ImagePtr = ImagePtrNew;
			UniFrame.MaskPtr = MaskPtrNew;
			UniFrame.SpacePtr = SpacePtrNew;
			UniFrame.ImageStep = ImageStep;
		}		
		UniFrameList.push_back(UniFrame);		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CModelWnd::GetCurrentFrame(IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, IMAGE_PTR &ImagePtr, SPACE_PTR &SpacePtr, MASK_PTR &MaskPtr)//取得目前影像
{
	if ( m_ImageIndex<0 || m_ImageIndex>=FRAME_MAX_COUNT ) { return false; }

	const unsigned int idx = m_ImageIndex;
	ImageW = m_UniFrameList[idx].ImageW;
	ImageH = m_UniFrameList[idx].ImageH;
	ImageStep = m_UniFrameList[idx].ImageStep;
	BitCount = m_UniFrameList[idx].BitCount;
	ImagePtr = m_UniFrameList[idx].ImagePtr;
	SpacePtr = m_UniFrameList[idx].SpacePtr;
	MaskPtr = m_UniFrameList[idx].MaskPtr;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CModelWnd::GetFrameImage(unsigned int Index, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, IMAGE_PTR &ImagePtr, SPACE_PTR &SpacePtr, MASK_PTR &MaskPtr)//取得目前影像
{
	if ( Index<0 || Index>=FRAME_MAX_COUNT ) { return false; }

	const unsigned int idx = Index;
	ImageW = m_UniFrameList[idx].ImageW;
	ImageH = m_UniFrameList[idx].ImageH;
	ImageStep = m_UniFrameList[idx].ImageStep;
	BitCount = m_UniFrameList[idx].BitCount;
	ImagePtr = m_UniFrameList[idx].ImagePtr;
	SpacePtr = m_UniFrameList[idx].SpacePtr;
	MaskPtr = m_UniFrameList[idx].MaskPtr;	
	return true;
}
//-------------------------------------------------------------------------------------//
void CModelWnd::ExecAlgImage(CAOIModel *ModelPtr, CAOIWnd *WndPtr, CAlgBinaryParam &BinaryParam)
{	
	if ( NULL == ModelPtr ) { return; }
	if ( NULL == WndPtr ) { return; }
	
	const int  nAlign = 4;
	const bool bNoFilter = false;
	std::vector<TUNI_FRAME> ModelUniFrameList;
	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();	
	if ( BuildModelUniFrameList(ModelPtr, ModelUniFrameList, nAlign, true, bNoFilter) == false )
	{	return ; }

	const size_t ModelniFrameCount = ModelUniFrameList.size();
	if ( 0 == ModelniFrameCount ) { return; }	
	RECT       WndRect={0,0,0,0};	
	RECT       ModelRect={0,0,0,0};
	RECT       WndExtRect={0,0,0,0};	
	RECT       ModelMaskRect={0,0,0,0};
	TREGION4D  ModelRegion, WndRegion, WndExtRegion;
	TPOINT2D   ModelRgnCp, ImageSale, ModelImageCp;	
	TUNI_FRAME UniFrame = ModelUniFrameList[0];	
	WndPtr->GetWndRegion(WndRegion);
	WndPtr->GetWndExtendRegion(WndExtRegion);	
	ModelPtr->GetModelTotalRegion(ModelRegion);	

	IMAGE_SIZE ModelImageW = UniFrame.ImageW;
	IMAGE_SIZE ModelImageH = UniFrame.ImageH;
	const double RegionW = ModelRegion.GetWidth();
	const double RegionH = ModelRegion.GetHeight();
	
	ModelRgnCp.x = ModelRegion.GetCpX();
	ModelRgnCp.y = ModelRegion.GetCpY();
	ImageSale.x = ModelImageW;
	ImageSale.y = ModelImageH;
	ImageSale.x = ImageSale.x/RegionW;
	ImageSale.y = ImageSale.y/RegionH;	
	ModelImageCp.x = ModelImageW;
	ModelImageCp.y = ModelImageH;
	ModelImageCp.x = ModelImageCp.x*0.5;
	ModelImageCp.y = ModelImageCp.y*0.5;	

	if ( CAOIModel::CalcModelBoxRegionRect(WndRegion, ModelImageW, ModelImageH, ModelRgnCp, ImageSale, ModelImageCp, WndRect) == false ) 
	{
		JetAPI::ClearUniFrameList(ModelUniFrameList);
		return; 
	}
	if ( CAOIModel::CalcModelBoxRegionRect(WndExtRegion, ModelImageW, ModelImageH, ModelRgnCp, ImageSale, ModelImageCp, WndExtRect) == false ) 
	{
		JetAPI::ClearUniFrameList(ModelUniFrameList);
		return; 
	}

	const bool bTestWnd = true;
	bool       bHeightImage = false;
	MASK_PTR   ModelMaskPtr = NULL;
	IMAGE_PTR  ModelGrayPtr = NULL;
	IMAGE_SIZE ModelMaskW=0, ModelMaskH=0, ModelMaskStep=0, ModelMaskBitCount=0;	
	JetAPI::SizeToRect(ModelImageW, ModelImageH, ModelRect);	
	if ( BINARY_DISABLE == BinaryParam.GetBinaryMode() )
	{	ModelMaskRect = ModelRect; }
	else
	{	AOIDataCollect.AdjustModelBinaryMaskRect(ModelRect, WndExtRect, ModelMaskRect);	 }	

	const unsigned int FrameUniqueID = BinaryParam.GetBinaryFrameUniqueID();
	if ( FRAME_UNIQUE_ID_DLP == FrameUniqueID )
	{	bHeightImage = true;	}
	else
	{	bHeightImage = false; }
	if ( AlgParam.ExecAlgUniFrameBinary(BinaryParam, WndRect, ModelMaskRect, ModelUniFrameList, ModelMaskW, ModelMaskH, ModelMaskStep, ModelMaskBitCount, ModelMaskPtr, ModelGrayPtr, bTestWnd) == false )
	{
		JetAPI::ClearUniFrameList(ModelUniFrameList);
		return;
	}	
	JetAPI::ClearUniFrameList(ModelUniFrameList);

	const char fnName[] = "CModelWnd::ExecAlgImage";	
	TPOINT2D   FovCp;
	TREGION4D  ImageRgn;
	MASK_PTR   MaskPtr=NULL;
	SPACE_PTR  SpacePtr=NULL;
	IMAGE_PTR  ImagePtr = NULL;
	IMAGE_SIZE ImageW=0, ImageH=0, ImageStep=0, BitCount=0;	
	const unsigned int FrameIndex= BinaryParam.GetBinaryFrameIndex();	
	if ( GetFrameImage(FrameIndex, ImageW, ImageH, ImageStep, BitCount, ImagePtr, SpacePtr, MaskPtr) == false ) 
	{		
		JetMemory.free_func(ModelMaskPtr);
		JetMemory.free_func(ModelGrayPtr);
		return; 
	}	
	
	IMAGE_PTR ShowImagePtr = NULL;	
	RECT  ModelRectInFov={0,0,0,0};
	const IMAGE_SIZE ShowBitCount = 24;	
	const IMAGE_SIZE ShowStep = JetAPI::GetBMPImagePixelsPerLine(ImageW, ShowBitCount, 4);	
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	const size_t ShowBufferSize = ImageAPI.CalcBufferSize(ShowStep, ImageH);

	FovCp.x = m_ModelImagePosStage.x;
	FovCp.y = m_ModelImagePosStage.y;	
	//模組範圍
	ModelPtr->GetModelTotalRegionStage(ModelRegion);
	if ( AOIDataCollect.MapStageRegionToCamera(ImageW, ImageH, m_ImageResolution, ModelRegion, FovCp, ImageRgn) == false )
	{			
		JetMemory.free_func(ModelMaskPtr);
		JetMemory.free_func(ModelGrayPtr);
		return; 
	}
	ModelRectInFov.left   = JetAPI::Floor(ImageRgn.minX);
	ModelRectInFov.right  = ModelRectInFov.left+ModelMaskW;
	ModelRectInFov.top    = JetAPI::Floor(ImageRgn.minY);
	ModelRectInFov.bottom = ModelRectInFov.top+ModelMaskH;
	JetAPI::SizeToRect(ImageW, ImageH, ModelRectInFov);	
	if ( ImageAPI.CheckRoiRect(ImageW, ImageH, ModelRectInFov) == false ) 
	{	
		JetMemory.free_func(ModelMaskPtr);
		JetMemory.free_func(ModelGrayPtr);
		return;
	}	
	
	if ( JetMemory.alloc_func(ShowBufferSize, ShowImagePtr, fnName, "ShowImagePtr") == false )
	{	
		JetMemory.free_func(ModelMaskPtr);		
		JetMemory.free_func(ShowImagePtr);
		JetMemory.free_func(ModelGrayPtr);
		return; 
	}		

	//一律轉成彩色計算	
	if ( BufferSize == ShowBufferSize )
	{	::memcpy(ShowImagePtr, ImagePtr, sizeof(IMAGE_DATA)*BufferSize);	}
	else
	{
		if ( ImageAPI.RGBImageToColorImage3(ImageW, ImageH, ImageStep, ImagePtr, ImagePtr, ImagePtr, ShowStep, ShowImagePtr, false) == false )
		{	
			JetMemory.free_func(ModelMaskPtr);			
			JetMemory.free_func(ShowImagePtr);
			JetMemory.free_func(ModelGrayPtr);
			return ;
		}		
	}	
	bool bIsOK=true;	
	BINARY_MODE BinaryMode = BinaryParam.GetBinaryMode();
	IMAGE_SRC_MODE ImageSrcMode = BinaryParam.GetBinaryImageSourceMode();
	if ( BINARY_DISABLE != BinaryMode ) 
	{
		MASK_DATA mask = 0xFF;
		IMAGE_DATA mskR=0xFF, mskG=0xFF, mskB=0x00, mskV=0xFF, Alpha=0;	
		MASK_PTR  ShowMaskPtr = NULL;
		const IMAGE_SIZE MaskBitCount = 8;
		const IMAGE_SIZE MaskStep = JetAPI::GetBMPImagePixelsPerLine(ImageW, MaskBitCount, 4);
		const size_t MaskBufferSize = ImageAPI.CalcBufferSize(MaskStep, ImageH);
		if ( JetMemory.alloc_func(MaskBufferSize, ShowMaskPtr, fnName, "ShowMaskPtr") == false )
		{	
			JetMemory.free_func(ModelMaskPtr);			
			JetMemory.free_func(ShowImagePtr);
			JetMemory.free_func(ModelGrayPtr);
			return;
		}
		//將局部的二值化影像貼上FOV的二值化指標內
		const bool theSameSize=false;
		::memset(ShowMaskPtr, 0x00, sizeof(MASK_DATA)*MaskBufferSize);
		if ( ImageAPI.PasteGrayRoiImage3(ImageW, ImageH, MaskStep, ShowMaskPtr, ModelRectInFov, ModelMaskStep, ModelMaskPtr, false, theSameSize) == false )
		{	
			JetMemory.free_func(ModelMaskPtr);
			JetMemory.free_func(ShowMaskPtr);
			JetMemory.free_func(ShowImagePtr);
			JetMemory.free_func(ModelGrayPtr);
			return; 
		}
		AOIDataCollect.GetMaskImageColor(FrameUniqueID, mskR, mskG, mskB, mskV, Alpha);
		AOIDataCollect.ExecEnhanceDisplayImage(ImageW, ImageH, ShowStep, ShowBitCount, ShowImagePtr, ShowImagePtr);
		if ( 24 == ShowBitCount )//Color Image
		{	bIsOK = ImageAPI.ColorImageApplyMask(ImageW, ImageH, ShowStep, ShowImagePtr, ModelRectInFov, MaskStep, ShowMaskPtr, mask, mskR, mskG, mskB, Alpha);	}
		else
		{	bIsOK = ImageAPI.GrayImageApplyMask(ImageW, ImageH, ShowStep, ShowImagePtr, ModelRectInFov, MaskStep, ShowMaskPtr, mask, mskV, Alpha);	}	
		JetMemory.free_func(ShowMaskPtr);
		if ( false == bIsOK )
		{	
			JetMemory.free_func(ModelMaskPtr);
			JetMemory.free_func(ShowImagePtr);		
			JetMemory.free_func(ModelGrayPtr);
			return;
		}		
	}
	else if ( IMAGE_SRC_COLOR != ImageSrcMode )
	{
		//將局部的灰階影像貼上FOV的灰階指標內		
		if ( false == bHeightImage )
		{
			const bool theSameSize=false;
			if ( 24 == ShowBitCount )//Color Image
			{	bIsOK = ImageAPI.PasteColorRoiImage3(ImageW, ImageH, ShowStep, ShowImagePtr, ModelRectInFov, ModelMaskStep, ModelGrayPtr, ModelGrayPtr, ModelGrayPtr, false, theSameSize);		}
			else
			{	bIsOK = ImageAPI.PasteGrayRoiImage3(ImageW, ImageH, ShowStep, ShowImagePtr, ModelRectInFov, ModelMaskStep, ModelGrayPtr, false, theSameSize);	}			
			if ( false == bIsOK )
			{	
				JetMemory.free_func(ShowImagePtr);
				JetMemory.free_func(ModelGrayPtr);
				return;
			}
		}
		AOIDataCollect.ExecEnhanceDisplayImage(ImageW, ImageH, ShowStep, ShowBitCount, ShowImagePtr, ShowImagePtr);		
	}
	else
	{	AOIDataCollect.ExecEnhanceDisplayImage(ImageW, ImageH, ShowStep, ShowBitCount, ShowImagePtr, ShowImagePtr);	}	
	JetMemory.free_func(ModelMaskPtr);		
	JetMemory.free_func(ModelGrayPtr);
	
	if ( ShowBufferSize > m_ShowBufferSize )
	{
		ReleaseShowImageBuffer();
		m_ShowImageW = ImageW;
		m_ShowImageH = ImageH;
		m_ShowImageStep = ShowStep;
		m_ShowBitCount = ShowBitCount;
		m_ShowImagePtr = ShowImagePtr;
		m_ShowBufferSize = ShowBufferSize;
	}
	else
	{
		m_ShowImageW = ImageW;
		m_ShowImageH = ImageH;
		m_ShowImageStep = ShowStep;
		m_ShowBitCount = ShowBitCount;
		::memcpy(m_ShowImagePtr, ShowImagePtr, sizeof(IMAGE_DATA)*ShowBufferSize);
		JetMemory.free_func(ShowImagePtr);		
	}
	
	CreateBKImage();
	RedrawWnd();	
	return ;
}
//-------------------------------------------------------------------------------------//
bool CModelWnd::ExecModelWndInspection(bool UpdateUI)
{	
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }
	CAOIModel *ModelPtr = CModelWnd::GetModelPtr();
	if ( NULL == ModelPtr ) { return false; }	
	DRAW_MODEL_MODE DrawModelMode = AOIDataCollect.GetDrawModelMode();
	if ( DRAW_MODEL_EDIT != DrawModelMode ) { return true; }	
	const int  nAlign = 4;
	const bool bNoFilter = false;
	std::vector<TUNI_FRAME> UniFrameList;
	if ( BuildModelUniFrameList(ModelPtr, UniFrameList, nAlign, true, bNoFilter) == false )
	{	return false; }
	const size_t UniFrameCount = UniFrameList.size();
	if ( 0 == UniFrameCount ) 
	{	return true; }

	const double AttachedAngle = ModelPtr->GetModelAttachedAngle();
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(AttachedAngle);

	UUID           WndUUID = ProjectPtr->GetProjectActiveModelWndUUID();
	CAOIWnd       *WndPtr = ModelPtr->GetModelWndPtrByUUID(WndUUID);	
	WndPtr = ModelPtr->GetModelWndActived();
	if ( NULL == WndPtr ) 
	{
		JetAPI::ClearUniFrameList(UniFrameList);
		if ( true == UpdateUI )
		{
			m_ViewModelWnd.PostMessage(MSG_EDIT_WND_PROPERTY_WND, WPARAM_UPDATE_WND_SELECTED, NULL);	
			//PostMessageToMainFrameWnd(MSG_EDIT_WND_PROPERTY_WND, WPARAM_UPDATE_WND_SELECTED, NULL);	
		}
		return false; 
	}			

	size_t     i=0;	
	TPOINT2D   RgnCp, Scale, ImageCp;
	TREGION4D  ModelRgn;	
	ModelPtr->GetModelTotalRegion(ModelRgn);	
	TUNI_FRAME UniFrame = UniFrameList[0];
	const IMAGE_SIZE ImageW = UniFrameList[0].ImageW;
	const IMAGE_SIZE ImageH = UniFrameList[0].ImageH;
	const double RegionW = ModelRgn.GetWidth();
	const double RegionH = ModelRgn.GetHeight();	
	TPOINT2D  ModelImageScale = ModelPtr->GetModelImageScale();
	CAOILand *LandPtr = WndPtr->GetWndLandPtr();	

	WndPtr->InitWndInspection(false);
	if ( false == IsExceptionAngle )
	{
		Scale.x = ImageW;
		Scale.y = ImageH;
		Scale.x = Scale.x/RegionW;
		Scale.y = Scale.y/RegionH;	
		RgnCp.x = ModelRgn.GetCpX();
		RgnCp.y = ModelRgn.GetCpY();
		ImageCp.x = ImageW;
		ImageCp.y = ImageH;
		ImageCp.x = ImageCp.x*0.5;
		ImageCp.y = ImageCp.y*0.5;	
		ModelPtr->SetModelImageScale(Scale);
		WndPtr->ExecWndInspection(ModelPtr, ModelRgn, RgnCp, Scale, ImageCp, UniFrameList);	
		JetAPI::ClearUniFrameList(UniFrameList);
	}
	else
	{
		TPOINT2D  ModelCornerPts[4];
		TREGION4D ModelRgnRotated;
		std::vector<TUNI_FRAME> UniFrameListDst;

		ModelPtr->GetModelTotalCornerPts(ModelCornerPts);
		JetAPI::RotateCornerPos(-AttachedAngle, 0, 0, ModelCornerPts);
		JetAPI::CornerPtToRegion(ModelCornerPts, ModelRgnRotated);
		if ( CAOIModel::RotateModelUniFrameList(-AttachedAngle, ModelRgn, ModelRgnRotated, UniFrameList, UniFrameListDst) == false )
		{			
			JetAPI::ClearUniFrameList(UniFrameList);
			return false;
		}
		JetAPI::ClearUniFrameList(UniFrameList);
		const IMAGE_SIZE ModelImageW = UniFrameListDst[0].ImageW;
		const IMAGE_SIZE ModelImageH = UniFrameListDst[0].ImageH;
		const double RgnWRotated = ModelRgnRotated.GetWidth();
		const double RgnHRotated = ModelRgnRotated.GetHeight();
		Scale.x = ModelImageW;
		Scale.y = ModelImageH;
		Scale.x = Scale.x/RgnWRotated;
		Scale.y = Scale.y/RgnHRotated;	
		RgnCp.x = ModelRgnRotated.GetCpX();
		RgnCp.y = ModelRgnRotated.GetCpY();
		ImageCp.x = ModelImageW;
		ImageCp.y = ModelImageH;
		ImageCp.x = ImageCp.x*0.5;
		ImageCp.y = ImageCp.y*0.5;	
		ModelPtr->SetModelImageScale(Scale);		
		WndPtr->RotateWnd(-AttachedAngle, 0, 0);
		WndPtr->ExecWndInspection(ModelPtr, ModelRgnRotated, RgnCp, Scale, ImageCp, UniFrameListDst);	
		WndPtr->RotateWnd(AttachedAngle, 0, 0);
		JetAPI::ClearUniFrameList(UniFrameListDst);
	}
	ModelPtr->SetModelImageScale(ModelImageScale);
	if ( true == UpdateUI )
	{
		m_ViewModelWnd.PostMessage(MSG_EDIT_WND_PROPERTY_WND, WPARAM_BUILD_WND_SELECTED, NULL);	
		//PostMessageToMainFrameWnd(MSG_EDIT_WND_PROPERTY_WND, WPARAM_UPDATE_WND_SELECTED, NULL);	
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CModelWnd::ExecModelComponentInspect()
{
	CAOIModel *ModelPtr = CModelWnd::GetModelPtr();
	if ( NULL == ModelPtr ) { return false; }
	if ( AOIDataCollect.CheckAIServerIsReady(false) == false )	
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return false;
	}

	std::vector<TUNI_FRAME> UniFrameList;	
	const int nAlign = 4;
	const bool bNoFilter = false;
	if ( BuildModelUniFrameList(ModelPtr, UniFrameList, nAlign, true, bNoFilter) == false )
	{	return false; }

	CWndDefectItem TestItem;
	CWndDefectItem AlarmItem;
	const int OpenMPCount = AOIDataCollect.GetOpenMPCount_General();

	TestItem.SetAll(1);
	AlarmItem.SetAll(0);
	ModelPtr->SetModelDefectItemTest(TestItem);
	ModelPtr->SetModelDefectItemAlarm(AlarmItem);
	ModelPtr->InitModelInspection();
	ModelPtr->SetModelOpenMPCount(OpenMPCount);
	ModelPtr->ExecModelInspection(UniFrameList);
	JetAPI::ClearUniFrameList(UniFrameList);
	AOIDataCollect.SetDrawModelMode(DRAW_MODEL_RESULT);
	UpdateActiveObjList();
	RedrawWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
void CModelWnd::UpdateImageByAlgParam()//依據演算法更新畫面
{
	DRAW_MODEL_MODE DrawModelMode = AOIDataCollect.GetDrawModelMode();
	DRAW_IMAGE_MODE DrawImageMode = AOIDataCollect.GetDrawImageMode();
	MANIPULATE_MODEL_MODE ManiMode = AOIDataCollect.GetManipulateModelMode();
	if ( DRAW_IAMGE_BY_ALG != DrawImageMode ) { return ; }	
	if ( MANIPULATE_MODEL_GATHER_COLOR == ManiMode ) { return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }
	CAOIModel *ModelPtr = CModelWnd::GetModelPtr();
	if ( NULL == ModelPtr ) { return; }	
	UUID           WndUUID = ProjectPtr->GetProjectActiveModelWndUUID();
	CAOIWnd       *WndPtr = ModelPtr->GetModelWndPtrByUUID(WndUUID);
	WndPtr = ModelPtr->GetModelWndActived();
	if ( NULL == WndPtr ) { return; }
	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();	
	CAlgBinaryParam  BinaryParam;
	CAlgBinaryParam *BinParamPtr = AlgParam.GetAlgActiveBinParamPtr();	
	if ( NULL == BinParamPtr ) { return ; }
	AOIDataCollect.GetBinaryParamTemp(BinaryParam);
	ExecAlgImage(ModelPtr, WndPtr, BinaryParam);
	if ( DRAW_MODEL_EDIT == DrawModelMode )
	{
		int    DynValue = BinaryParam.GetDynamicThresholdValue();
		double RelValue = BinaryParam.GetRelativeAveThresholdValue();
		BinParamPtr->SetDynamicThresholdValue(DynValue);
		BinParamPtr->SetRelativeAveThresholdValue(RelValue);
	}
	ModelPtr->UnSelectModel();
	WndPtr->SetWndVisibled(true);
	WndPtr->SetWndSelected(true);
	ModelPtr->SetModelWndActived(WndPtr);
	AOIDataCollect.SetDrawingImageMode(DRAW_IAMGE_BY_ALG);	
}
//-------------------------------------------------------------------------------------//
bool CModelWnd::ExecCalcWndColor()//計算檢測框顏色
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return true; }
	UUID           WndUUID = ProjectPtr->GetProjectActiveModelWndUUID();
	CAOIWnd       *WndPtr = ModelPtr->GetModelWndPtrByUUID(WndUUID);	
	WndPtr = ModelPtr->GetModelWndActived();
	if ( NULL == WndPtr ) { return true; }
	CAlgParam       &AlgParam = WndPtr->GetWndAlgParam();	
	CAlgBinaryParam *BinParamPtr = AlgParam.GetAlgActiveBinParamPtr();	
	BINARY_MODE      BinaryMode = BinParamPtr->GetBinaryMode();
	CColorRGBV      *rgbvPtr = BinParamPtr->GetBinaryColorActivePtr();
	if ( NULL == rgbvPtr ) { return true; }

	CColorRGBV   rgbv;
	if ( ExecGetWndColorFilter(rgbv) == false )
	{	return false; }
	rgbv.ExpandColorRGBV(5, false);
	AOIDataCollect.SetColorRGBVTemp(rgbv);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool  CModelWnd::ExecExtractWndColorFilter()//取得檢測框抽色參數
{	
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return true; }
	UUID           WndUUID = ProjectPtr->GetProjectActiveModelWndUUID();
	CAOIWnd       *WndPtr = ModelPtr->GetModelWndPtrByUUID(WndUUID);	
	WndPtr = ModelPtr->GetModelWndActived();
	if ( NULL == WndPtr ) { return true; }
	CAlgParam       &AlgParam = WndPtr->GetWndAlgParam();	
	CAlgBinaryParam *BinParamPtr = AlgParam.GetAlgActiveBinParamPtr();	
	BINARY_MODE      BinaryMode = BinParamPtr->GetBinaryMode();
	CColorRGBV      *rgbvPtr = BinParamPtr->GetBinaryColorActivePtr();
	if ( NULL == rgbvPtr ) { return true; }

	CColorRGBV   rgbv;
	if ( ExecGetWndColorFilter(rgbv) == false )
	{	return false; }

	rgbv.ExpandColorRGBV(5, false);
	rgbvPtr->CheckUsed();
	rgbvPtr->CalcShowColor();
	rgbvPtr->CopyColorRGBV(rgbv);
	rgbvPtr->CheckUsed();	
	rgbvPtr->CalcShowColor();
	rgbv = *rgbvPtr;
	WndPtr->SetWndModified(true);
	ModelPtr->ApplyModelWnd(WndPtr);	
	LogOperCtrl.SaveLogModelWndAlgColorFilterOperateExtractWndColor(WndPtr);
	ProjectPtr->UpdateProjectColorGroup(BinParamPtr);

	CAlgBinaryParam  BinaryParam = *BinParamPtr;
	BinaryParam.ClearBinaryColorList();
	rgbv.SetLogicMode(COLOR_LOGIC_INCLUDE);
	BinaryParam.AddBinaryColor(rgbv);
	ExecAlgImage(ModelPtr, WndPtr, BinaryParam);
	AOIDataCollect.SendMainFrameWndMessage(MSG_EDIT_IMAGE_PROCESS_WND, WPARAM_UPDATE_ALG_COLOR_FILTER, (LPARAM)(WndPtr));		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CModelWnd::ExecGetWndColorFilter(CColorRGBV &rgbv)//取得檢測框抽色參數
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return true; }		
	const double AttachedAngle = ModelPtr->GetModelAttachedAngle();
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(AttachedAngle);
	if ( true == IsExceptionAngle ) { return true; }

	UUID           WndUUID = ProjectPtr->GetProjectActiveModelWndUUID();
	CAOIWnd       *WndPtr = ModelPtr->GetModelWndPtrByUUID(WndUUID);	
	WndPtr = ModelPtr->GetModelWndActived();
	if ( NULL == WndPtr ) { return true; }
	CAlgParam       &AlgParam = WndPtr->GetWndAlgParam();	
	CAlgBinaryParam *BinParamPtr = AlgParam.GetAlgActiveBinParamPtr();	
	BINARY_MODE    BinaryMode = BinParamPtr->GetBinaryMode();
	IMAGE_SRC_MODE ImageSourceMode = BinParamPtr->GetBinaryImageSourceMode();	
	if ( IMAGE_SRC_COLOR != ImageSourceMode ) { return true; }
	if ( BINARY_COLOR_FILTER != BinaryMode ) { return true; }	
	const bool bResetColorGroup=false;
	if ( true == bResetColorGroup )
	{
		BinParamPtr->ResetBinaryColorList();
		BinParamPtr->SetBinaryColorActiveIndex(0);
	}
	CColorRGBV *rgbvPtr = BinParamPtr->GetBinaryColorActivePtr();
	if ( NULL == rgbvPtr ) { return true; }
	
	CString    str;
	MASK_PTR   MaskPtr=NULL;
	SPACE_PTR  SpacePtr=NULL;
	IMAGE_PTR  ImagePtr = NULL;
	IMAGE_SIZE ImageW=0, ImageH=0, ImageStep=0, BitCount=0;	
	TPOINT2D ImageRes = m_ImageResolution;
	if ( GetCurrentFrame(ImageW, ImageH, ImageStep, BitCount, ImagePtr, SpacePtr, MaskPtr) == false ) { return true; }
	if ( NULL == ImagePtr ) { return true; }
	if ( 24 != BitCount ) { return true; }

	bool         bIsOK = false;	
	bool         bSaved = true;	
	TREGION4D    ImageRgn;		
	TREGION4D    StageRgn;
	RECT         RoiRect={0, 0, 0, 0};
	TPOINT2D     StageCp = m_ModelImagePosStage;	
	DRAW_MODEL_MODE DrawModelMode = AOIDataCollect.GetDrawModelMode();
	if ( DRAW_MODEL_RESULT == DrawModelMode ) 
	{	WndPtr->GetWndRegionStageRes(StageRgn);	}
	else
	{	WndPtr->GetWndRegionStage(StageRgn);	}
	AOIDataCollect.MapStageRegionToCamera(ImageW, ImageH, ImageRes, StageRgn, StageCp, ImageRgn);
	JetAPI::Region4DToRect(ImageRgn, RoiRect, false);	
#ifdef _DEBUG
	if ( true == bSaved ) 
	{
		IMAGE_PTR  RoiPtr=NULL;
		IMAGE_SIZE RoiW=0, RoiH=0, RoiStep=0;
		RoiW = RoiRect.right-RoiRect.left;
		RoiH = RoiRect.bottom-RoiRect.top;
		RoiStep = JetAPI::GetBMPImagePixelsPerLine(RoiW, BitCount, 4);
		if ( ImageAPI.ExtractRoiImage(ImageW, ImageH, ImageStep, BitCount, ImagePtr, RoiRect, RoiStep, RoiPtr, false) == true ) 
		{
			str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("ExtractWndColor.PNG"));
			ImageAPI.SaveImage(str, RoiW, RoiH, RoiStep, BitCount, RoiPtr, true);
			JetMemory.free_func(RoiPtr);
		}
	}
#endif//_DEBUG
	if ( ImageAPI.CalcColorImageColorFilter(ImageW, ImageH, ImageStep, ImagePtr, RoiRect, rgbv) == false )
	{	return false;	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CModelWnd::ExecGatherColorFilter(bool CombineColorMode)//吸取抽色參數
{
	const char fnName[] = "CModelWnd::ExecGatherColorFilter";	
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }
	CAOIModel *ModelPtr = CModelWnd::GetModelPtr();
	if ( NULL == ModelPtr ) { return true; }		

	UUID           WndUUID = ProjectPtr->GetProjectActiveModelWndUUID();
	CAOIWnd       *WndPtr = ModelPtr->GetModelWndPtrByUUID(WndUUID);
	WndPtr = ModelPtr->GetModelWndActived();
	if ( NULL == WndPtr ) { return true; }
	CAlgParam       &AlgParam = WndPtr->GetWndAlgParam();	
	CAlgBinaryParam *BinParamPtr = AlgParam.GetAlgActiveBinParamPtr();
	if ( NULL == BinParamPtr ) { return false; }
	BINARY_MODE    BinaryMode = BinParamPtr->GetBinaryMode();
	IMAGE_SRC_MODE ImageSourceMode = BinParamPtr->GetBinaryImageSourceMode();	
	if ( IMAGE_SRC_COLOR != ImageSourceMode ) { return true; }
	if ( BINARY_COLOR_FILTER != BinaryMode ) { return true; }
	CColorRGBV *rgbvPtr = BinParamPtr->GetBinaryColorActivePtr();
	if ( NULL == rgbvPtr ) { return true; }
	
	CString    str;
	MASK_PTR   MaskPtr=NULL;
	SPACE_PTR  SpacePtr=NULL;
	IMAGE_PTR  ImagePtr = NULL;
	IMAGE_SIZE ImageW=0, ImageH=0, ImageStep=0, BitCount=0;	
	if ( CModelWnd::GetCurrentFrame(ImageW, ImageH, ImageStep, BitCount, ImagePtr, SpacePtr, MaskPtr) == false ) { return true; }
	if ( NULL == ImagePtr ) { return true; }
	if ( 24 != BitCount ) { return true; }

	bool                    bIsOK = false;
	TPOINT2D                FovCp;	
	RECT                    RoiRect={0, 0, 0, 0};
	TREGION4D               ImageRgn;	
	BOOL                    bSaved = FALSE;
	
	const double ComponentAngle = ModelPtr->GetModelAttachedAngle();	
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComponentAngle);

	CColorRGBV   rgbv;
	TPOINT2D dImagePt1, dImagePt2;
	POINT nWndPt1 = m_MousePosLast;
	POINT nWndPt2 = m_MousePosFirst;	
	PtInControlWnd(m_MousePosLast, MODELEDIT_IMAGE_WND, nWndPt1);
	PtInControlWnd(m_MousePosFirst, MODELEDIT_IMAGE_WND, nWndPt2);

	ImageAPI.MapWndPtToImagePt_DBL(ImageW, ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, nWndPt1, dImagePt1);	
	ImageAPI.MapWndPtToImagePt_DBL(ImageW, ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, nWndPt2, dImagePt2);	
	ImageRgn.minX = MIN(dImagePt1.x, dImagePt2.x);
	ImageRgn.minY = MIN(dImagePt1.y, dImagePt2.y);
	ImageRgn.maxX = MAX(dImagePt1.x, dImagePt2.x);
	ImageRgn.maxY = MAX(dImagePt1.y, dImagePt2.y);
	JetAPI::Region4DToRect(ImageRgn, RoiRect, false);
	if ( ImageAPI.CalcColorImageColorFilter(ImageW, ImageH, ImageStep, ImagePtr, RoiRect, rgbv) == false )
	{	return false;	}

	rgbv.ExpandColorRGBV(5, false);
	rgbvPtr->CheckUsed();
	rgbvPtr->CalcShowColor();	
	if ( false==CombineColorMode || rgbvPtr->CheckUsed()==false )
	{	rgbvPtr->CopyColorRGBV(rgbv);	}
	else
	{	rgbvPtr->MergeColor(false, &rgbv);	}
	rgbvPtr->CheckUsed();	
	rgbvPtr->CalcShowColor();
	rgbv = *rgbvPtr;
	WndPtr->SetWndModified(true);
	ModelPtr->ApplyModelWnd(WndPtr);
	ProjectPtr->UpdateProjectColorGroup(BinParamPtr);

	CAlgBinaryParam  BinaryParam = *BinParamPtr;
	BinaryParam.ClearBinaryColorList();
	rgbv.SetLogicMode(COLOR_LOGIC_INCLUDE);
	BinaryParam.AddBinaryColor(rgbv);
	ExecAlgImage(ModelPtr, WndPtr, BinaryParam);	
	m_ViewModelImage.SendMessage(MSG_EDIT_IMAGE_PROCESS_WND, WPARAM_UPDATE_ALG_COLOR_FILTER, (LPARAM)(WndPtr));		
	return true;
}
//-------------------------------------------------------------------------------------//
void CModelWnd::OnTestModeBtn() 
{
	// TODO: Add your control notification handler code here
	this->ExecModelComponentInspect();
}
//-------------------------------------------------------------------------------------//