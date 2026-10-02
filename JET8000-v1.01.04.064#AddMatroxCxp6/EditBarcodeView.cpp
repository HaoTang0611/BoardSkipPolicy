// EditBarcodeView.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "EditBarcodeView.h"
//-------------------------------------------------------------------------------------//
#include "AOIBox.h"
#include "InputBoxWnd.h"
#include "ModelPropertyWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CEditBarcodeView
//-------------------------------------------------------------------------------------//
IMPLEMENT_DYNCREATE(CEditBarcodeView, CView)
//-------------------------------------------------------------------------------------//
CEditBarcodeView::CEditBarcodeView()
{
	m_BkColor = 0x000000;
	m_ModelPtr = NULL;	
	m_ProjectPtr = NULL;
	m_ImageZoom = 1.0;
	m_DrawAddRect = false;
	m_DrawEditRect = false;
	m_KeepImageOffset = false;
	m_ShowPopupMenu = false;
	m_FovRatio = 1.0;
	m_ImageIndex = 0;	
	m_ShowImageW = 1024;
	m_ShowImageH = 1024;
	m_ShowImageStep = 1024*3;
	m_ShowBitCount = 24;
	m_ShowImagePtr = NULL;
	m_UpdateTestMapTickCount = 0;
	PreInitUniFrameBuffer();	
}
//-------------------------------------------------------------------------------------//
CEditBarcodeView::~CEditBarcodeView()
{
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CEditBarcodeView, CView)
	//{{AFX_MSG_MAP(CEditBarcodeView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_WM_CONTEXTMENU()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(MENU_SOFTWARE_BARCODE_ADD_MODE, OnSoftwareBarcodeAddMode)
	ON_UPDATE_COMMAND_UI(MENU_SOFTWARE_BARCODE_ADD_MODE, OnUpdateSoftwareBarcodeAddMode)
	ON_COMMAND(MENU_SOFTWARE_BARCODE_EDIT_MODE, OnSoftwareBarcodeEditMode)
	ON_UPDATE_COMMAND_UI(MENU_SOFTWARE_BARCODE_EDIT_MODE, OnUpdateSoftwareBarcodeEditMode)
	ON_COMMAND(MENU_SOFTWARE_BARCODE_DEL_SELECTED, OnSoftwareBarcodeDelSelected)
	ON_UPDATE_COMMAND_UI(MENU_SOFTWARE_BARCODE_DEL_SELECTED, OnUpdateSoftwareBarcodeDelSelected)
	ON_COMMAND(MENU_SOFTWARE_BARCODE_DEL_SELECTED_WND, OnSoftwareBarcodeDelSelectedWnd)
	ON_UPDATE_COMMAND_UI(MENU_SOFTWARE_BARCODE_DEL_SELECTED_WND, OnUpdateSoftwareBarcodeDelSelectedWnd)
	ON_COMMAND(MENU_SOFTWARE_BARCODE_DEL_OTHERS, OnSoftwareBarcodeDelOthers)
	ON_UPDATE_COMMAND_UI(MENU_SOFTWARE_BARCODE_DEL_OTHERS, OnUpdateSoftwareBarcodeDelOthers)
	ON_COMMAND(MENU_SOFTWARE_BARCODE_DEL_OTHER_WNDS, OnSoftwareBarcodeDelOtherWnds)
	ON_UPDATE_COMMAND_UI(MENU_SOFTWARE_BARCODE_DEL_OTHER_WNDS, OnUpdateSoftwareBarcodeDelOtherWnds)
	ON_COMMAND(MENU_SOFTWARE_BARCODE_PASTE_TO_OTHER_BOARDS, OnSoftwareBarcodePasteToOtherBoards)
	ON_UPDATE_COMMAND_UI(MENU_SOFTWARE_BARCODE_PASTE_TO_OTHER_BOARDS, OnUpdateSoftwareBarcodePasteToOtherBoards)
	ON_COMMAND(MENU_SOFTWARE_BARCODE_PASTE_TO_OTHER_PANELS, OnSoftwareBarcodePasteToOtherPanels)
	ON_UPDATE_COMMAND_UI(MENU_SOFTWARE_BARCODE_PASTE_TO_OTHER_PANELS, OnUpdateSoftwareBarcodePasteToOtherPanels)
	ON_COMMAND(MENU_SOFTWARE_BARCODE_ADD_BARCODE_WND, OnSoftwareBarcodeAddBarcodeWnd)
	ON_UPDATE_COMMAND_UI(MENU_SOFTWARE_BARCODE_ADD_BARCODE_WND, OnUpdateSoftwareBarcodeAddBarcodeWnd)	
	ON_COMMAND(MENU_SOFTWARE_BARCODE_INSPECT_ALL, OnSoftwareBarcodeInspectAll)
	ON_UPDATE_COMMAND_UI(MENU_SOFTWARE_BARCODE_INSPECT_ALL, OnUpdateSoftwareBarcodeInspectAll)	
	ON_COMMAND(MENU_SOFTWARE_BARCODE_INSPECT_SELECTED, OnSoftwareBarcodeInspectSelected)
	ON_UPDATE_COMMAND_UI(MENU_SOFTWARE_BARCODE_INSPECT_SELECTED, OnUpdateSoftwareBarcodeInspectSelected)
	ON_COMMAND(MENU_SOFTWARE_BARCODE_DEL_ALL, OnSoftwareBarcodeDelAll)
	ON_UPDATE_COMMAND_UI(MENU_SOFTWARE_BARCODE_DEL_ALL, OnUpdateSoftwareBarcodeDelAll)
	ON_COMMAND(MENU_SOFTWARE_BARCODE_DEL_ALL, OnSoftwareBarcodeDelAll)
	ON_UPDATE_COMMAND_UI(MENU_SOFTWARE_BARCODE_DEL_ALL, OnUpdateSoftwareBarcodeDelAll)
	ON_COMMAND(MENU_SOFTWARE_BARCODE_ALIGN_FIDUCIAL, OnSoftwareBarcodeAlignFiducial)
	ON_UPDATE_COMMAND_UI(MENU_SOFTWARE_BARCODE_ALIGN_FIDUCIAL, OnUpdateSoftwareBarcodeAlignFiducial)
	ON_COMMAND(MENU_SOFTWARE_BARCODE_PROPERTY_WND, OnSoftwareBarcodePropertyWnd)
	ON_UPDATE_COMMAND_UI(MENU_SOFTWARE_BARCODE_PROPERTY_WND, OnUpdateSoftwareBarcodePropertyWnd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CEditBarcodeView drawing
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CEditBarcodeView diagnostics

#ifdef _DEBUG
void CEditBarcodeView::AssertValid() const
{
	CView::AssertValid();	
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CEditBarcodeView message handlers
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();	
	// TODO: Add your specialized code here and/or call the base class
	SwitchMultiLanguage();
	GetClientRect(&m_ImageWndRect);
	m_ImageWndMapDC.CreateMemDC(this, m_BkColor);
	m_ImageWndMemDC.CreateMemDC(this, m_BkColor);
	m_ImageWndMemDC2.CreateMemDC(this, m_BkColor);

	SetShowPopupMenu(true);	
	AOIDataCollect.SetShowFdList(false);	
	AOIDataCollect.SetShowPanelList(true);	
	AOIDataCollect.SetShowBoardList(true);	
	AOIDataCollect.SetShowBarcodeList(true);
	AOIDataCollect.SetShowComponentList(false);
	AOIDataCollect.SetCallbackWnd(CWnd::GetSafeHwnd());
	AOIDataCollect.SetModelAttachedObj(MODEL_ATTACHED_BARCODE);	
	//AOIDataCollect.SwitchProjectTaskMode(PROJECT_TASK_NORMAL);	
	SwitchProject();
	RestoreViewParam();	
	CreateBKImage();
	//CEditBarcodeView::ExecMoveToStage();//由Frame視窗發送MSG_CAMERA_REGRAB_IMAGE
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_PART_LIST_WND, WPARAM_BUILD_PART_LIST, LPARAM_BUILD_DOCK_LIST_EDIT);
	CWnd::PostMessage(MSG_CAMERA_REGRAB_IMAGE, NULL, NULL);
	return;
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::OnDestroy() 
{
	CView::OnDestroy();	
	// TODO: Add your message handler code here		
	CloseProject();	
	AOIDataCollect.SwitchProjectTaskMode(PROJECT_TASK_NORMAL);
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);	
	// TODO: Add your message handler code here
	if ( CWnd::GetSafeHwnd() == NULL ) { return; }	
	//if ( m_ImageWnd.GetSafeHwnd() != NULL )
	if ( CWnd::GetSafeHwnd() != NULL )
	{
		RECT  WndRect={0};
		const int Margin = 4;		
		CWnd::GetClientRect(&m_ImageWndRect);
		m_ImageWndMapDC.CreateMemDC(this, m_BkColor);
		m_ImageWndMemDC.CreateMemDC(this, m_BkColor);
		m_ImageWndMemDC2.CreateMemDC(this, m_BkColor);
		CreateBKImage();
		CreateMapImage();
		RedrawWnd();
	}
}
//-------------------------------------------------------------------------------------//
BOOL CEditBarcodeView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
	return CView::OnEraseBkgnd(pDC);
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CView::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if ( TRUE == bShow )
	{	
		AOIDataCollect.SetDrawModelMode(DRAW_MODEL_EDIT);
		AOIDataCollect.SetCallbackWnd(CWnd::GetSafeHwnd());
		AOIDataCollect.SetManipulateModelMode(MANIPULATE_MODEL_EDIT);
	}
	else
	{
		if ( AOIDataCollect.GetOfflineMode() == true )
		{	CalcFovPosition();	}
		BackupViewParam();
		CloseProject();	 
	}
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	const UINT CtrlID = pWnd->GetDlgCtrlID();
	POINT dPos, LastPos;
	//不要用m_MousePosLast, 因為可能收不到OnMouseMove
	if ( GetShowPopupMenu() == false )
	{
		SetShowPopupMenu(true);
		return;
	}
	LastPos = point;
	CWnd::ScreenToClient(&LastPos);
	dPos.x = LastPos.x - m_MousePosFirst.x;
	dPos.y = LastPos.y - m_MousePosFirst.y;

	if ( ::abs(dPos.x)>2 || ::abs(dPos.y)>2 )
	{	return; }

	AOIDataCollect.CancelGatherColorMode();
	const bool SwitchFrameMode = AOIDataCollect.CheckSwitchFrameMode();
	if ( true == SwitchFrameMode )
	{	SwitchFrameImage();	 }
	//ExecPopupMenu(point);
}
//-------------------------------------------------------------------------------------//
BOOL CEditBarcodeView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	UINT ControlID = pWnd->GetDlgCtrlID();	

	CURSOR_POS_MODE OldCursorMode = m_MousePosMode;
	CURSOR_POS_MODE CursorMode = CheckCursorPosModeBarcode(m_MousePosImageWnd);
	m_MousePosMode = CursorMode;
	
	//if ( CursorMode != OldCursorMode )
	//{	CEditBarcodeView::RedrawWnd();	}
	if ( CURSOR_POS_NONE == CursorMode )
	{	return CView::OnSetCursor(pWnd, nHitTest, message);	}

	JetAPI::UpdateCursor(CursorMode); 
	return TRUE;

	//return CView::OnSetCursor(pWnd, nHitTest, message);
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	POINT pt2={0};
	if ( PtInControlWnd(point, MODELEDIT_IMAGE_WND, pt2) == false )
	{
		CView::OnLButtonDown(nFlags, point);
		return;
	}
	CWnd::SetCapture();
	m_MousePosImageWnd = m_MousePosCurrent = m_MousePosFirst = m_MousePosLast = point;		
	//CWnd::MapWindowPoints(&m_ImageWnd, &m_MousePosImageWnd, 1);	
	SetDrawAddRect(true);
	RedrawWnd();
	CheckActiveObjFocus(m_ActiveObj, m_ActiveBox);	
	CView::OnLButtonDown(nFlags, point);
}
//-------------------------------------------------------------------------------------//
bool CEditBarcodeView::ExecSaveLogLButtonUp()
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
void CEditBarcodeView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	::ReleaseCapture();	
	m_MousePosLast = m_MousePosCurrent;	
	m_MousePosImageWnd = m_MousePosCurrent = point;
	//CWnd::MapWindowPoints(&m_ImageWnd, &m_MousePosImageWnd, 1);		
	MANIPULATE_MODEL_MODE ManiMode = AOIDataCollect.GetManipulateModelMode();
	
	if ( MANIPULATE_MODEL_ADD == ManiMode )
	{	CEditBarcodeView::ExecAddBarcode();	}
	else if ( MANIPULATE_MODEL_SELECT==ManiMode || MANIPULATE_MODEL_EDIT==ManiMode )
	{			
		if ( CURSOR_POS_NONE == m_MousePosMode )
		{	
			ExecSelectBarcode();
			ExecModelWndInspection(true);
			//UpdateImageByAlgParam();			
		}
		else if ( CURSOR_POS_NONE != m_MousePosMode )
		{
			ExecSaveLogLButtonUp();
			ExecModelWndInspection(true);
			UpdateImageByAlgParam();			
		}		
		RedrawWnd();
	}
	else if ( MANIPULATE_MODEL_GATHER_COLOR == ManiMode )
	{			
		const bool CombineColorMode = AOIDataCollect.CheckCombineColorMode();
		ExecGatherColorFilter(CombineColorMode);		
		if ( false == CombineColorMode ) 
		{	AOIDataCollect.CancelGatherColorMode();	}		
		RedrawWnd();
	}	
	SetDrawAddRect(false);
	RedrawWnd();
	CView::OnLButtonUp(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CView::OnLButtonDblClk(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	POINT pt2={0};
	SetDrawAddRect(false);
	if ( PtInControlWnd(point, MODELEDIT_IMAGE_WND, pt2) == false )
	{
		CView::OnRButtonDown(nFlags, point);
		return;
	}
	CWnd::SetFocus();
	CWnd::SetCapture();
	m_MousePosImageWnd = m_MousePosCurrent = m_MousePosFirst = m_MousePosLast = point;
	//CWnd::MapWindowPoints(&m_ImageWnd, &m_MousePosImageWnd, 1);
	CView::OnRButtonDown(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	POINT dp = {0};
	::ReleaseCapture();		
	m_MousePosImageWnd = m_MousePosCurrent = m_MousePosLast = point;	
	if ( AOIDataCollect.CancelManipulateMainMode() == true ) 
	{	SetShowPopupMenu(false); }
	//CWnd::MapWindowPoints(&m_ImageWnd, &m_MousePosImageWnd, 1);
	if ( CheckMousePosMoved() )//20241209
	{	AdjustCurrentFrames();	}	
	CView::OnRButtonUp(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default	
	//CEditBarcodeView::CalcFovPosition();	
	CView::OnRButtonDblClk(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default	
	POINT dPoint;
	bool  bToDraw = false;
	m_MousePosLast = m_MousePosCurrent;	
	m_MousePosImageWnd = m_MousePosCurrent = point;
	//CWnd::MapWindowPoints(&m_ImageWnd, &m_MousePosImageWnd, 1);
	dPoint.x = point.x - m_MousePosLast.x;
	dPoint.y = point.y - m_MousePosLast.y;
	if ( this != CWnd::GetCapture() )
	{	
		CEditBarcodeView::RedrawWnd();
		CView::OnMouseMove(nFlags, point);
		return;
	}

	//JetAPI::UpdateCursor();
	if ( nFlags & MK_LBUTTON )//滑鼠左鍵
	{
		bool Modify = false;
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
		if ( true == Modify )
		{	bToDraw = true;	}
	}
	else if (nFlags & MK_RBUTTON )//滑鼠右鍵
	{
		this->m_ImageOffset.x += dPoint.x;
		this->m_ImageOffset.y += dPoint.y;
		//CEditBarcodeView::CalcFovPosition();
		CEditBarcodeView::CreateBKImage();
		bToDraw = true;		
	}	
	if ( true == m_DrawAddRect )
	{	bToDraw = true;		}	
	if ( true == m_DrawEditRect )
	{	bToDraw = true;		}	
	if ( true == bToDraw )
	{	CEditBarcodeView::RedrawWnd();	}
	CView::OnMouseMove(nFlags, point);
}
//-------------------------------------------------------------------------------------//
BOOL CEditBarcodeView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	double NextImageZoom = m_ImageZoom;
	if ( zDelta > 0 ) 
	{	NextImageZoom *= 1.1;	}
	else
	{	NextImageZoom /= 1.1;	}
	
	const bool   OfflineMode = AOIDataCollect.GetOfflineMode();		
	const bool   ShowProjectMapMode = GetShowProjectMapMode();
	NextImageZoom = AOIDataCollect.AdjustImageZoom(NextImageZoom);
	ImageAPI.CalcImageWndZoom(m_ImageZoom, NextImageZoom, m_ImageOffset);
	m_ImageZoom = NextImageZoom;		

	if ( true==OfflineMode && false==ShowProjectMapMode )
	{
		MASK_PTR   MaskPtr=NULL;
		SPACE_PTR  SpacePtr=NULL;
		IMAGE_PTR  ImagePtr=NULL;
		IMAGE_SIZE ImageW=0;
		IMAGE_SIZE ImageH=0;
		IMAGE_SIZE ImageStep=0;
		IMAGE_SIZE BitCount=0;
		if ( GetCurrentFrame(ImageW, ImageH, ImageStep, BitCount, ImagePtr, SpacePtr, MaskPtr) == false ) 
		{	return TRUE; }

		//確認是否重新補圖
		if ( NULL!=ImagePtr && m_ImageZoom>1 )
		{			
			const int RealWndW = m_ImageWndRect.right-m_ImageWndRect.left;
			const int RealWndH = m_ImageWndRect.bottom-m_ImageWndRect.top;
			const int WndSizeW = (int)(ImageW/m_ImageZoom);
			const int WndSizeH = (int)(ImageH/m_ImageZoom);
			IMAGE_SIZE BasicImageW = AOIDataCollect.GetCameraImageW(PRIMARY_CAMERA_ID);
			IMAGE_SIZE BasicImageH = AOIDataCollect.GetCameraImageH(PRIMARY_CAMERA_ID);	

			if ( WndSizeW<RealWndW || WndSizeH<RealWndH )
			{	
				double Ratio = 1;
				double RatioW = (double)(ImageW);
				double RatioH = (double)(ImageH);
				double dBImageW = (double)(BasicImageW);
				double dBImageH = (double)(BasicImageH);
				RatioW = RatioW/dBImageW;
				RatioH = RatioH/dBImageH;
				//每次擴增0.5個FOV
				RatioW = RatioW+0.5;
				RatioH = RatioH+0.5;
				Ratio = MAX(RatioW, RatioH);
				if ( Ratio < 1.0 ) 
				{	Ratio = 1.0; }
				FillCurrentFrames(Ratio);
				BuildShowImageBuffer();
			}		
		}
	}
	//POINT point = pt;
	//CWnd::ScreenToClient(&point);
	//CEditBarcodeView::CalcCursorInfo(point);
	//CEditBarcodeView::CalcImageWndLBtn();
	//CEditBarcodeView::DrawImage();
	CEditBarcodeView::CreateBKImage();
	CEditBarcodeView::RedrawWnd();
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}
//-------------------------------------------------------------------------------------//
BOOL CEditBarcodeView::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class		
	bool bRedraw=false;
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
			AOIDataCollect.SetDrawModelMode(DRAW_MODEL_EDIT);
			break;
		default:
			if ( AOIDataCollect.GetCombineColorVrKey() == pMsg->wParam )
			{	AOIDataCollect.CancelGatherColorMode();	}
			break;
		}
		AOIDataCollect.SetProjectHasModified(pMsg->wParam, GetActiveProject(), m_ActiveObjList);
		break;
	}	
	return CView::PreTranslateMessage(pMsg);
}
//-------------------------------------------------------------------------------------//
LRESULT CEditBarcodeView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{	
	HWND  hWnd = NULL;
	CWnd *pWnd = NULL;
	switch ( message )
	{	
	case MSG_MAIN_FRAME_MESSAGE:
		switch ( wParam )
		{
		case WPARAM_PROJECT_NEW:
		case WPARAM_PROJECT_OPEN:
		case WPARAM_PROJECT_SWITCH:			
			SwitchProject();			
			CreateBKImage();
			RedrawWnd();
			break;		
		case WPARAM_PROJECT_UPDATE:
			if ( CWnd::IsWindowVisible() == TRUE )
			{
				pWnd = (CWnd*)lParam;
				if ( pWnd != this )
				{
					UpdateBarcodeSelected();	
					RedrawWnd();  
				}
			}
			break;		
		case WPARAM_PROJECT_CLOSE:		
			CloseProject();
			if ( CWnd::IsWindowVisible() == TRUE )
			{	RedrawWnd(); }
			break;
		case WPARAM_PROJECT_PART_SELECTED:			
			UpdateBarcodeSelected();	
			break;
		case WPARAM_PROJECT_PART_DELETED:
			UpdateBarcodeSelected();	
			break;
		case WPARAM_CALC_CURRENT_FOV_POSITION:
			CalcFovPosition();
			break;
		case WPARAM_PROJECT_SWITCH_MARK:
			break;
		case WPARAM_PROJECT_CLOSE_ACTIVE_OBJ:			
			ResetBarcodeModel();
			ReleaseUniFrameBuffer();	
			ReleaseShowImageBuffer();
			break;
		}
		break;
	case MSG_EDIT_MAIN_VIEW_WND:
		switch ( wParam )
		{
		case WPARAM_SWITCH_FRAME_IMAGE:
			UpdateFrameImage();			
			break;
		case WPARAM_UPDATE_VIEW_PART_SELECTED:
			UpdateBarcodeSelected();	
			break;
		case WPARAM_REDRAW_VIEW_WND:
		case WPARAM_REDRAW_PROJECT_MAP:
			RedrawWnd();
			hWnd = GetSafeHwnd();//否吃掉重複重繪訊息
			JetAPI::RemoveMessage(hWnd, MSG_EDIT_MAIN_VIEW_WND, MSG_EDIT_MAIN_VIEW_WND);
			break;
		case WPARAM_UPDATE_ALG_IMAGE:		
			UpdateImageByAlgParam();			
			break;
		case WPARAM_EXEC_WND_INSPECT:
			//ExecModelWndInspection();
			break;
		case WPARAM_CALC_WND_COLOR:
			ExecCalcWndColor();
			break;
		case WPARAM_EXTRACT_WND_COLOR_FILTER:
			ExecExtractWndColorFilter();
			break;
		case WPARAM_SHOW_WND_POSITION:
			ExecShowWndPosition();
			break;
		case WPARAM_TOGGLE_ENCHANGE_IMAGE_MODE://切換強化影像模式
			ExecToggleEnhanceImageMode();	
			break;
		case WPARAM_SET_DRAW_PROJECT_MODE:
			CreateMapImage();
			break;
		case WPARAM_UPDATE_PROJECT_TEST_MAP:
			CreateTestMapImage();			
			break;
		case WPARAM_BUILD_RAW_MODEL_UNI_FRAME_LIST://建立原始模組通用影像列表
		{	
			const int  nAlign = 4;
			const bool bClone = false;
			const bool bNoFilter = true;
			CAOIModel *ModelPtr = (CAOIModel*)(lParam);
			std::vector<TUNI_FRAME> UniFrameList;			
			BuildModelUniFrameList(ModelPtr, UniFrameList, nAlign, bClone, bNoFilter);
		}
			break;
		}
		break;
	case MSG_CAMERA_CALLBACK:		
		if ( AOIDataCollect.CheckCanRetrieveCameraImage() == false )
		{	break; }
		switch ( wParam )
		{
		case WPARAM_CAMERA_1_CALLBACK:			
		case WPARAM_CAMERA_2_CALLBACK:			
		case WPARAM_CAMERA_3_CALLBACK:			
		case WPARAM_CAMERA_4_CALLBACK:			
		case WPARAM_CAMERA_5_CALLBACK:
			if ( this->RetrieveCameraImage(wParam, lParam, true) == false )
			{	this->LockUIWnd(false);	}
			break;
		}
		break;		
	case MSG_CAMERA_REGRAB_IMAGE:
		ExecMoveToStage();
		break;	
	case MSG_INSPECTION_CALLBACK:
		switch ( wParam )
		{		
		case WPARAM_INSPECTION_FINISH://檢測狀態-檢測結束
			ExecInspection_Finish();
			break;
		case WPARAM_INSPECTION_ONLINE_FINISH:
			ExecOnlineInspection_Finish();
			break;	
		}			
		break;
	case MSG_SYSTEM_EXCEPTION_CALLBACK:
		LockUIWnd(false);
		AOIDataCollect.ExecSystemException(wParam, lParam);		
		break;
	}
	return CView::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::SwitchMultiLanguage()
{
	/*
	size_t  i = 0;	
	UINT    WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_EDIT_BARCODE_VIEW");	
	//---------------------------------------------------------------------------------//
	WndID = IDD_EDIT_BARCODE_VIEW;
	WndKey = _T("IDD_EDIT_BARCODE_VIEW");
	this->GetWindowText(LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetWindowText(NewLabelText);
	//---------------------------------------------------------------------------------//	
	*/
}
//-------------------------------------------------------------------------------------//
CString CEditBarcodeView::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_EDIT_BARCODE_VIEW");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
inline bool CEditBarcodeView::PtInControlWnd(const POINT &pt, UINT ID, POINT &pt2)
{
	pt2 = pt;
	if ( ::PtInRect(&m_ImageWndRect, pt) == FALSE )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
inline BOOL CEditBarcodeView::MapWndPtToImagePt_DBL(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const RECT &WndRect, const TPOINT2D &OffsetPt, double ZoomScale, const TPOINT2D &WndPt, TPOINT2D &ImagePt)
{
	ImageAPI.MapWndPtToImagePt_DBL(ImageW, ImageH, WndRect, OffsetPt, ZoomScale, WndPt, ImagePt);
	return TRUE;
}
//-------------------------------------------------------------------------------------//
inline BOOL CEditBarcodeView::MapImagePtToWndPt_DBL(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const RECT &WndRect, const TPOINT2D &OffsetPt, double ZoomScale, const TPOINT2D &ImagePt, TPOINT2D &WndPt)
{
	ImageAPI.MapImagePtToWndPt_DBL(ImageW, ImageH, WndRect, OffsetPt, ZoomScale, ImagePt, WndPt);
	return TRUE;
}
//-------------------------------------------------------------------------------------//
inline BOOL CEditBarcodeView::MapImageRgnToWndRgn_DBL(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const RECT &WndRect, const TPOINT2D &OffsetPt, double ZoomScale, const TREGION4D &ImageRgn, TREGION4D &WndRgn)
{
	ImageAPI.MapImageRgnToWndRgn_DBL(ImageW, ImageH, WndRect, OffsetPt, ZoomScale, ImageRgn, WndRgn);
	return TRUE;
}
//-------------------------------------------------------------------------------------//
inline IMAGE_SIZE CEditBarcodeView::GetFrameImageW_2() const
{
	return m_ShowImageW;
}
//-------------------------------------------------------------------------------------//
inline IMAGE_SIZE CEditBarcodeView::GetFrameImageH_2() const
{
	return m_ShowImageH;
}
//-------------------------------------------------------------------------------------//
IMAGE_SIZE CEditBarcodeView::GetImageW() const
{
	return GetFrameImageW_2();
}
//-------------------------------------------------------------------------------------//
IMAGE_SIZE CEditBarcodeView::GetImageH() const
{
	return GetFrameImageH_2();
}
//-------------------------------------------------------------------------------------//
const TPOINT2D& CEditBarcodeView::GetImageResolution() const
{
	return m_FrameResolution;
}
//-------------------------------------------------------------------------------------//
const TREGION4D& CEditBarcodeView::GetImageStageRgn() const
{
	return m_FrameStageRgn;
}
//-------------------------------------------------------------------------------------//
const TPOINT2D CEditBarcodeView::GetImageStageRgnCp() const
{
	TPOINT2D ImageStageRgnCp;
	const TREGION4D &ImageStageRgn = GetImageStageRgn();
	ImageStageRgnCp.x = ImageStageRgn.GetCpX();
	ImageStageRgnCp.y = ImageStageRgn.GetCpY();
	return ImageStageRgnCp;
}
//-------------------------------------------------------------------------------------//
bool CEditBarcodeView::GetShowProjectMapMode() const
{
	return false;
}
//-------------------------------------------------------------------------------------//
bool CEditBarcodeView::CheckMousePosMoved() const//確認滑鼠移動過
{
	const int dx = m_MousePosLast.x-m_MousePosFirst.x;
	const int dy = m_MousePosLast.y-m_MousePosFirst.y;
	if ( abs(dx) > 10 || abs(dy) > 10 )
	{	return true; }
	return false;
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::CloseProject()
{	
	ResetBarcodeModel();
	ReleaseUniFrameBuffer();	
	ReleaseShowImageBuffer();
	
	m_ProjectPtr = NULL;
	m_MapZoom = 1.00;
	m_ImageZoom = 1.00;		
	ResetImageOffset();
	m_FrameResolution.x = AOIDataCollect.GetCameraResolutionX(PRIMARY_CAMERA_ID);
	m_FrameResolution.y = AOIDataCollect.GetCameraResolutionY(PRIMARY_CAMERA_ID);
	CreateBKImage();
	CreateMapImage();

	HBRUSH hBrush = NULL;
	RECT Rect = m_ImageWndRect;
	HDC hMemDC = m_ImageWndMemDC.GetSafeHdc();
	HDC hMemDC2 = m_ImageWndMemDC2.GetSafeHdc();	
	hBrush = ::CreateSolidBrush(m_BkColor);
	if ( NULL != hBrush )
	{
		if ( NULL != hMemDC )
		{	::FillRect(hMemDC, &Rect, hBrush); }
		if ( NULL != hMemDC2 )
		{	::FillRect(hMemDC2, &Rect, hBrush); }
		::DeleteObject(hBrush); hBrush=NULL;
	}
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::SwitchProject()//切換專案
{	
	CloseProject();	
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveTaskProject();	
	if ( NULL == ProjectPtr )	{	return;	}

	m_ProjectPtr = ProjectPtr;	
	ProjectPtr->ResetProjectActiveIndex();	
	CreateBKImage();
	CreateMapImage();
}
//-------------------------------------------------------------------------------------//
inline CAOIProject* CEditBarcodeView::GetActiveProject()
{
	return m_ProjectPtr;
}
//-------------------------------------------------------------------------------------//
inline CAOIModel* CEditBarcodeView::GetModelPtr()
{
	return m_ModelPtr;	
}
//-------------------------------------------------------------------------------------//
inline CAOIBarcode* CEditBarcodeView::GetModelBarcodePtr()
{
	CAOIModel *ModelPtr=GetModelPtr();
	if ( NULL == ModelPtr ) { return NULL; }
	return ModelPtr->GetModelBarcodePtr();
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::RedrawWnd()
{
	CClientDC dc(this);
	HDC hDC = dc.GetSafeHdc();
	HDC hMemDC = m_ImageWndMemDC.GetSafeHdc();
	HDC hMemDC2 = m_ImageWndMemDC2.GetSafeHdc();	
	if ( NULL==hMemDC || NULL==hMemDC2 || NULL==hDC ) 
	{	return; }

	CString str;
	size_t  SelectedFdCount = 0;
	RECT    WndRect = m_ImageWndRect;	
	CAOIProject *ProjectPtr = this->GetActiveProject();		
	ONLINE_STATE_MODE OnlineStateGUI = AOIDataCollect.GetOnlineStateMode_GUI();
	if ( ONLINE_STATE_INSPECTION_STOP != OnlineStateGUI )
	{
		HDC hMapDC = m_ImageWndMapDC.GetSafeHdc();
		if ( NULL != hMapDC )
		{	::BitBlt(hMemDC2, 0, 0, WndRect.right, WndRect.bottom, hMapDC, 0, 0, SRCCOPY ); }
		::IntersectClipRect(hMemDC2, WndRect.left, WndRect.top, WndRect.right, WndRect.bottom);	
		AOIDataCollect.DrawEditViewInspection(hMemDC2, WndRect, OnlineStateGUI, ProjectPtr, m_MapZoom);	
	}
	else
	{
		::BitBlt(hMemDC2, 0, 0, WndRect.right, WndRect.bottom, hMemDC, 0, 0, SRCCOPY );
		::IntersectClipRect(hMemDC2, WndRect.left, WndRect.top, WndRect.right, WndRect.bottom);	
	
		if ( NULL != ProjectPtr )
		{	SelectedFdCount = ProjectPtr->GetProjectFdSelectedCount();	}

		::SetTextColor(hMemDC2, 0x00FF00);
	
		DrawBoxInfo(hMemDC2);	
		DrawBarcode(hMemDC2);			
		DrawObjectList(hMemDC2);
		DrawAddRect(hMemDC2);	
		DrawModel(hMemDC2);
		DrawModelActivedLine(hMemDC2);
		DrawCrosshair(hMemDC2);
	}
	::BitBlt(hDC, 0, 0, WndRect.right, WndRect.bottom, hMemDC2, 0, 0, SRCCOPY );	
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::CreateBKImage()
{
	HDC hMemDC = m_ImageWndMemDC.GetSafeHdc();
	if ( NULL == hMemDC ) { return; }	
	HBRUSH hBrush = ::CreateSolidBrush(m_BkColor);
	if ( NULL != hBrush )
	{
		::FillRect(hMemDC, &m_ImageWndRect, hBrush); 
		::DeleteObject(hBrush);	hBrush = NULL;	
	}
	DrawImage(hMemDC);
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::CreateMapImage(bool bTestMap)
{
	HDC hMemDC = m_ImageWndMapDC.GetSafeHdc();
	if ( NULL == hMemDC ) { return; }	
	HBRUSH hBrush = ::CreateSolidBrush(m_BkColor);
	if ( NULL != hBrush )
	{
		::FillRect(hMemDC, &m_ImageWndRect, hBrush); 
		::DeleteObject(hBrush);	hBrush = NULL;	
	}
	DrawProjectMap(hMemDC, bTestMap);
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::CreateTestMapImage()
{
	CAOIProject *ProjectPtr=GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }
	DWORD TestMapTickCount=ProjectPtr->GetProjectTestMapTickCount(0);
	if ( TestMapTickCount < m_UpdateTestMapTickCount ) { return; }
	m_UpdateTestMapTickCount=GetTickCount();
	CreateMapImage(true);
	m_UpdateTestMapTickCount=GetTickCount();
	return;
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::DrawBarcode(HDC hDC)
{
//	return;
	CAOIProject *Project = GetActiveProject();
	if ( NULL == Project ) { return; }

	CString      str;
	size_t       i = 0;		
	POINT        Pt={0}, CornerPos[4];
	RECT         Rect={0};	
	bool         BarcodeSelected = false;
	const RECT   WndRect = m_ImageWndRect;
	TPOINT2D     StagePos, ImagePos, WndPt;
	TPOINT2D     StgCornerPos[4], ImgCornerPos[4], WndCornerPos[4];	
	TPOINT2D     ImageStagePos;	
	TREGION4D    ObjStageRgn, ObjImageRgn;		
	CAOIPanel   *PanelPtr = NULL;
	CAOIBoard   *BoardPtr = NULL;	
	CAOIBarcode *BarcodePtr = NULL;
	TPOINT2D     StageOffset, CadOffset;

	const IMAGE_SIZE   ImageW = GetImageW();
	const IMAGE_SIZE   ImageH = GetImageH();	
	const TPOINT2D     StageCp = GetImageStageRgnCp();
	const TPOINT2D    &ImageRes = GetImageResolution();
	const TREGION4D   &ImageStageRgn = GetImageStageRgn();

	const double StageCpx = StageCp.x;
	const double StageCpy = StageCp.y;
	const DRAW_MODEL_MODE DrawModelMode = GetDrawModelMode();
	const DRAW_COMPONENT_MODE  DrawComponentMode = AOIDataCollect.GetDrawComponentMode();//顯示零件模式	
	const size_t PanelCount = Project->GetProjectPanelCount();
	const size_t BoardCount = Project->GetProjectBoardCount();
	const size_t BarcodeCount = Project->GetProjectBarcodeCount();
	const TSystemParameter &SystemParam = AOIDataCollect.GetSystemParameter();
	const COLORREF  clr1 = SystemParam.m_BarcodeColor1;
	const COLORREF  clr2 = SystemParam.m_BarcodeColor2;
	const COLORREF  clrText = SystemParam.m_BarcodeTextColor;
	const COLORREF  clrSelect = SystemParam.m_BarcodeSelectedColor;

	HPEN hPenBarcode    = ::CreatePen(PS_SOLID, 1, clr1);	
	HPEN hPenBarcodeSel = ::CreatePen(PS_SOLID, 2, clrSelect);	
	HPEN hOldPen = (HPEN)(::SelectObject(hDC, hPenBarcode));	
	COLORREF clrTextOld = ::SetTextColor(hDC, clrText);
	int BKMode = ::SetBkMode(hDC, TRANSPARENT);		

	ImageStagePos.x = ImageStageRgn.GetCpX();
	ImageStagePos.y = ImageStageRgn.GetCpY();
	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = Project->GetProjectBarcodePtr(i, false);
		if ( NULL == BarcodePtr ) { continue; }
		if ( BarcodePtr->GetBarcodeDeleted() == true ) { continue; }

		StagePos.x = BarcodePtr->GetBarcodeStagePosX();
		StagePos.y = BarcodePtr->GetBarcodeStagePosY();
		if ( StagePos.x<ImageStageRgn.minX || StagePos.x>ImageStageRgn.maxX ) { continue; }
		if ( StagePos.y<ImageStageRgn.minY || StagePos.y>ImageStageRgn.maxY ) { continue; }
		BarcodeSelected = BarcodePtr->GetBarcodeSelected();
		AOIDataCollect.MapStagePtToCamera(ImageW, ImageH, ImageRes, StagePos, ImageStagePos, ImagePos);
		MapImagePtToWndPt_DBL(ImageW, ImageH, WndRect, m_ImageOffset, m_ImageZoom, ImagePos, WndPt);

		JetAPI::Point2DToPoint(WndPt, Pt);
		if ( Pt.x<0 || Pt.y<0 || Pt.x>WndRect.right || Pt.y>WndRect.bottom ) { continue; }

		BarcodePtr->GetBarcodeBodyStageCornerPos(StgCornerPos);		
		AOIDataCollect.MapStageCornerToCamera(ImageW, ImageH, ImageRes, StgCornerPos, ImageStagePos, ImgCornerPos);		
		MapImagePtToWndPt_DBL(ImageW, ImageH, WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[0], WndCornerPos[0]);
		MapImagePtToWndPt_DBL(ImageW, ImageH, WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[1], WndCornerPos[1]);
		MapImagePtToWndPt_DBL(ImageW, ImageH, WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[2], WndCornerPos[2]);
		MapImagePtToWndPt_DBL(ImageW, ImageH, WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[3], WndCornerPos[3]);

		JetAPI::CornerPt2DToCornerPt(WndCornerPos, CornerPos);
		if ( JetAPI::CheckCornerInRect(CornerPos, WndRect) == false ) 
		{	continue; }

		if ( true == BarcodeSelected )
		{	::SelectObject(hDC, hPenBarcodeSel);	}
		else
		{	::SelectObject(hDC, hPenBarcode);	 }
		ImageAPI.DrawPolyLine(hDC, CornerPos, 4);
		str.Format(_T("Barcode-%d"), BarcodePtr->GetBarcodeIndex_Project()+1);		
		::TextOut(hDC, Pt.x, Pt.y, str, str.GetLength());
		
	}	
	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPenBarcode);    hPenBarcode = NULL;		
	::DeleteObject(hPenBarcodeSel); hPenBarcodeSel = NULL;		
	::SetTextColor(hDC, clrTextOld);
	::SetBkMode(hDC, BKMode);	
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::DrawModel(HDC hDC)
{
	CAOIModel *ModelPtr = CEditBarcodeView::GetModelPtr();
	if ( NULL == ModelPtr ) { return; }

	TMODEL_DRAW_PARAM DrawParam;
	TPOINT2D ComponentStagePos;
	TPOINT2D StageOffset, CadOffset;
	const TPOINT2D StageCp = GetImageStageRgnCp();
	const TPOINT2D &ImageRes = GetImageResolution();
	DRAW_MODEL_MODE DrawModelMode = GetDrawModelMode();

	ModelPtr->GetModelAttachedPosStage(ComponentStagePos);

	const double StageCpx = StageCp.x;
	const double StageCpy = StageCp.y;
	const double StageOffsetX = (StageCpx-ComponentStagePos.x);	
	const double StageOffsetY = (StageCpy-ComponentStagePos.y);

	//Cad座標與影像座標為固定方位, 因此先將機台偏差改成Cad偏差, 再來處理
	StageOffset.x = StageOffsetX;
	StageOffset.y = StageOffsetY;
	AOIDataCollect.MapStageOffsetPtToCad(StageOffset, CadOffset);

	const double ImageOffsetX =  CadOffset.x/ImageRes.x;
	const double ImageOffsetY = -CadOffset.y/ImageRes.y;
	const double ViewOffsetX = ImageOffsetX/m_ImageZoom;
	const double ViewOffsetY = ImageOffsetY/m_ImageZoom;

	DrawParam.WndRect = m_ImageWndRect;
	DrawParam.ViewCP.x = DrawParam.ViewCP.y = 0;
	DrawParam.Scale = m_ImageZoom;	
	DrawParam.ViewOffsetX =  m_ImageOffset.x;
	DrawParam.ViewOffsetY =  -m_ImageOffset.y;	
	DrawParam.ResolutionX = ImageRes.x;
	DrawParam.ResolutionY = ImageRes.y;
	DrawParam.ShowEditLine = true;	
	DrawParam.ViewCP.x = -JetAPI::Floor(ViewOffsetX);
	DrawParam.ViewCP.y = -JetAPI::Floor(ViewOffsetY);

	ModelPtr->DrawModel(hDC, DrawModelMode, DrawParam);
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::DrawImage(HDC hDC)
{
	if ( NULL == m_ShowImagePtr ) { return; }
	const int BltMode = AOIDataCollect.GetStretchBltMode(m_ImageZoom);
	if ( ImageAPI.DrawImageToDC(hDC, m_ShowImageW, m_ShowImageH, m_ShowImageStep, m_ShowBitCount, m_ShowImagePtr, m_ImageWndRect, m_ImageOffset, m_ImageZoom, 0x00000, BltMode) == false )
	{	return ; }
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::DrawProjectMap(HDC hDC, bool bTestMap)
{
	if ( NULL == hDC ) { return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }

	TPOINT2D    Offset;
	bool        bForce = false;
	double      ImageZoom = 1.0;
	IMAGE_SIZE  MapW = 0;
	IMAGE_SIZE  MapH = 0;
	IMAGE_SIZE  MapStep = 0;
	IMAGE_SIZE  BitCount = 0;
	IMAGE_PTR   MapPtr = NULL;
	RECT        WndRect = m_ImageWndRect;
	const int MapIndex = m_ImageIndex;//ProjectPtr->GetProjectMapIndex();	
	if ( true == bTestMap ) { bForce = true; }
	if ( ProjectPtr->GetProjectMapPtr(MapIndex, MapW, MapH, MapStep, BitCount, MapPtr) == false )
	{	return ; }

	ProjectPtr->CreateProjectMapShowPtr(MapIndex, MapPtr, bForce, bTestMap);
	ImageAPI.CalcImageWndFitZoom(MapW, MapH, WndRect, 1.0, ImageZoom);//計算影像視窗縮放參數	
	ImageAPI.DrawImageToDC(hDC, MapW, MapH, MapStep, BitCount, MapPtr, WndRect, Offset, ImageZoom, m_BkColor);	
	m_MapZoom = ImageZoom;
	return;
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::DrawAddRect(HDC hDC)
{
	if ( false == m_DrawAddRect ) { return; }
	if ( CURSOR_POS_NONE != m_MousePosMode ) { return; }
	//CAOIModel *ModelPtr = CEditBarcodeView::GetModelPtr();
	//if ( NULL == ModelPtr ) { return; }

	HPEN hPen    = ::CreatePen(PS_SOLID, 1, 0x00FF00);
	HPEN hOldPen = (HPEN)::SelectObject(hDC, hPen);

	const double ComponentAngle = 0;
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
void CEditBarcodeView::DrawBoxInfo(HDC hDC)
{
	size_t      i = 0;		
	TActiveObj  *ObjPtr = NULL;			
	const size_t NObjects = this->m_ActiveObjList.size();	
	CString  str, str2, strPixel;
	//Draw Curpos	
	int      IR=0, IG=0, IB=0, IV=0;
	TSIZE2D  StageSize;
	TPOINT2D WndPt = m_MousePosImageWnd;
	TPOINT2D ImagePt, StagePt;
	TPOINT2D ImagePt1, StagePt1;
	TPOINT2D ImagePt2, StagePt2;	

	int SpaceIndex = -1;
	IMAGE_SIZE ImageW = 0;
	IMAGE_SIZE ImageH = 0;
	IMAGE_SIZE ImageStep = 0;
	IMAGE_SIZE BitCount = 0;
	IMAGE_PTR  ImagePtr = NULL;
	MASK_PTR   MaskPtr=NULL;
	SPACE_PTR  SpacePtr=NULL;	
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL != ProjectPtr )
	{
		SpaceIndex = ProjectPtr->GetProjectMapIndex3D();
		m_ImageIndex = ProjectPtr->GetProjectMapIndex(); 
	}
	if ( GetCurrentFrame(ImageW, ImageH, ImageStep, BitCount, ImagePtr, SpacePtr, MaskPtr) == false )
	{	return;		}

	IMAGE_SIZE SpaceW = 0;
	IMAGE_SIZE SpaceH = 0;
	IMAGE_SIZE SpaceStep = 0;
	IMAGE_SIZE SpaceBitCount = 0;
	IMAGE_PTR  SpaceImagePtr = NULL;
	MASK_PTR   SpaceMaskPtr=NULL;
	SPACE_PTR  SpaceSpacePtr=NULL;
	if ( SpaceIndex != m_ImageIndex )
	{	GetFrameImage(SpaceIndex, SpaceW, SpaceH, SpaceStep, SpaceBitCount, SpaceImagePtr, SpaceSpacePtr, SpaceMaskPtr);	}

	const int nImageW = (int)(ImageW);
	const int nImageH = (int)(ImageH);
	const int nImageStep = (int)(ImageStep);
	const TPOINT2D StageCp = GetImageStageRgnCp();
	const TPOINT2D &ImageRes = GetImageResolution();
	MapWndPtToImagePt_DBL(ImageW, ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, WndPt, ImagePt);	
	MapWndPtToImagePt_DBL(ImageW, ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, m_MousePosFirst, ImagePt1);	
	MapWndPtToImagePt_DBL(ImageW, ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, m_MousePosLast, ImagePt2);	
	AOIDataCollect.MapCameraPtToStage(ImageW, ImageH, ImageRes, ImagePt, StageCp, StagePt);	
	AOIDataCollect.MapCameraPtToStage(ImageW, ImageH, ImageRes, ImagePt1, StageCp, StagePt1);	
	AOIDataCollect.MapCameraPtToStage(ImageW, ImageH, ImageRes, ImagePt2, StageCp, StagePt2);
	StageSize.cx = fabs(StagePt1.x-StagePt2.x);
	StageSize.cy = fabs(StagePt1.y-StagePt2.y);
	str.Format(_T("First(%d, %d), Last(%d, %d), WndPos(%.0f, %.0f), ImagePos(%.0f, %.0f), StageSize(%.0f, %.0f)"), m_MousePosFirst.x, m_MousePosFirst.y, m_MousePosLast.x, m_MousePosLast.y, WndPt.x, WndPt.y, ImagePt.x, ImagePt.y, StageSize.cx, StageSize.cy);

	//Draw Color Value
	if ( ImagePtr==NULL || ImagePt.x<0 || ImagePt.y<0 || ImagePt.x>=nImageW || ImagePt.y>=nImageH )
	{	strPixel = _T("");	}
	else
	{
		int index = 0;
		double    SpaceHeight=0;
		const int nImageX=JetAPI::Floor(ImagePt.x);
		const int nImageY=JetAPI::Floor(ImagePt.y);		
		const double BaseHeight = AOIDataCollect.GetSpaceBaseHeight();
		if ( NULL != SpaceSpacePtr )
		{
			index = (nImageY*SpaceStep)+nImageX;
			SpaceHeight = SpaceSpacePtr[index]-BaseHeight;
		}
		switch ( BitCount )
		{
		case 8:	
			index = (nImageY*nImageStep)+nImageX;
			if ( NULL == SpacePtr )
			{	
				if ( NULL == SpaceSpacePtr )
				{	strPixel.Format(_T("Gray=(%d)"), ImagePtr[index]); }
				else
				{	strPixel.Format(_T("Gray=(%d), Height=%.0fum"), ImagePtr[index], SpaceHeight);	}
			}
			else
			{	strPixel.Format(_T("Height=%.0fum"), SpacePtr[index]-BaseHeight);	}
			break;
		case 24:
			index = (nImageY*nImageStep)+(nImageX*3);
			ImageAPI.RGBConvertToRGBV(ImagePtr[index+2], ImagePtr[index+1], ImagePtr[index], IR, IG, IB, IV);
			if ( NULL == SpaceSpacePtr )
			{	strPixel.Format(_T("RGB=(%d, %d, %d), RGBV=(%d, %d, %d, %d)"), ImagePtr[index+2], ImagePtr[index+1], ImagePtr[index], IR, IG, IB, IV);	}
			else
			{	strPixel.Format(_T("RGB=(%d, %d, %d), RGBV=(%d, %d, %d, %d), Height=%.0fum"), ImagePtr[index+2], ImagePtr[index+1], ImagePtr[index], IR, IG, IB, IV, SpaceHeight);	}
			break;
		default:			
			break;
		}		
		//str = str+CString(_T(", "))+strPixel;
	}	
#ifdef _DEBUG
	::TextOut(hDC, 8, 28, str, str.GetLength());	
#endif//_DEBUG

	str.Format(_T("Roi Size(%.0f, %.0f)"), StageSize.cx, StageSize.cy);
	::TextOut(hDC, m_ImageWndRect.right-164, m_ImageWndRect.bottom-24, str, str.GetLength());

	if ( strPixel.GetLength() > 0 ) 
	{	::TextOut(hDC, 8, m_ImageWndRect.bottom-24, strPixel, strPixel.GetLength());	}
	
	if ( NULL != ProjectPtr )
	{		
		str = ProjectPtr->GetProjectMapIndexName();
		//::TextOut(hDC, m_ImageWndRect.right-96, m_ImageWndRect.bottom-24, str, str.GetLength());
		::TextOut(hDC, m_ImageWndRect.right-96, 8, str, str.GetLength());		
	}
	
	for ( i=0; i<NObjects; i++ )
	{
		ObjPtr = &(m_ActiveObjList[i]);
		if ( NULL == ObjPtr ) { continue; }
		if ( false == ObjPtr->GetFocused() ) { continue; }
		break;
	}
	if ( NObjects == i ) { return; }

	CAOIBox *BoxPtr = (CAOIBox*)ObjPtr->BoxPtr;
	if ( NULL == BoxPtr ) { return; }

	TPOINT2D Pos;
	TSIZE2D  Size;
	TPOINT2D CornerPos[4];
	BoxPtr->GetBoxPos(Pos);
	BoxPtr->GetBoxSize(Size);
	BoxPtr->GetBoxCornerPosStage(CornerPos);

	::SetTextColor(hDC, 0x8FFFFF);
#ifdef _DEBUG
	str.Format(_T("Pos(%.0f, %.0f), Size(%.0f, %.0f), Stage( [%.0f, %.0f], [%.0f, %.0f], [%.0f, %.0f], [%.0f, %.0f] )"), Pos.x, Pos.y, Size.cx, Size.cy, CornerPos[0].x, CornerPos[0].y, CornerPos[1].x, CornerPos[1].y, CornerPos[2].x, CornerPos[2].y, CornerPos[3].x, CornerPos[3].y);
	::TextOut(hDC, 8, 48, str, str.GetLength());
#endif//_DEBUG		
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::DrawCrosshair(HDC hDC)//十字線
{
	const POINT &Pt=m_MousePosCurrent;	
	const RECT &WndRect=m_ImageWndRect;
	MANIPULATE_MODEL_MODE ManiMode=AOIDataCollect.GetManipulateModelMode();
	if ( MANIPULATE_MODEL_ADD != ManiMode ) { return ; }
	ImageAPI.DrawCrosshair(hDC, Pt, WndRect, 0xA0A0A0);	
	return;
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::DrawObjectList(HDC hDC)
{
	size_t      i = 0;		
	TActiveObj  *ObjPtr = NULL;
	RECT         nRect={0};
	RECT         nRect2={0};	
	double       ImageZoom = m_ImageZoom;
	TRECT4D      dImageRect, dWndRect;
	TPOINT2D     ImageCornerPts[4], WndCornerPts[4];
	TPOINT2D     ImageOffset=m_ImageOffset;
	const size_t NObjects = this->m_ActiveObjList.size();
	const IMAGE_SIZE ImageW = GetImageW();
	const IMAGE_SIZE ImageH = GetImageH();	

	HPEN hPen    = ::CreatePen(PS_SOLID, 1, 0x00FF00);
	HPEN hPenSel = ::CreatePen(PS_SOLID, 1, 0x0000FF);
	HPEN hOldPen = (HPEN)::SelectObject(hDC, hPen);

	ImageOffset.x =  m_ImageOffset.x;
	ImageOffset.y =  m_ImageOffset.y;
	for ( i=0; i<NObjects; i++ )
	{
		ObjPtr = &(m_ActiveObjList[i]);
		if ( NULL == ObjPtr ) { continue; }

		if ( true == ObjPtr->GetFocused() )
		{	::SelectObject(hDC, hPenSel);	}
		else
		{	::SelectObject(hDC, hPen);	}
		
		if ( false == ObjPtr->IsExceptionAngle )
		{
			dImageRect   = ObjPtr->Rect;			
			ImageAPI.MapImageRectToWndRect_DBL(ImageW, ImageH, m_ImageWndRect, ImageOffset, ImageZoom, dImageRect, dWndRect);
			JetAPI::Rect4DToRect(dWndRect, nRect);		
			ImageAPI.DrawRectLine(hDC, nRect);
		}
		else
		{
			ImageCornerPts[0] = ObjPtr->CornerPts[0];
			ImageCornerPts[1] = ObjPtr->CornerPts[1];
			ImageCornerPts[2] = ObjPtr->CornerPts[2];
			ImageCornerPts[3] = ObjPtr->CornerPts[3];
			
			ImageAPI.MapImagePtToWndPt_DBL(ImageW, ImageH, m_ImageWndRect, ImageOffset, ImageZoom, ImageCornerPts[0], WndCornerPts[0]);
			ImageAPI.MapImagePtToWndPt_DBL(ImageW, ImageH, m_ImageWndRect, ImageOffset, ImageZoom, ImageCornerPts[1], WndCornerPts[1]);
			ImageAPI.MapImagePtToWndPt_DBL(ImageW, ImageH, m_ImageWndRect, ImageOffset, ImageZoom, ImageCornerPts[2], WndCornerPts[2]);
			ImageAPI.MapImagePtToWndPt_DBL(ImageW, ImageH, m_ImageWndRect, ImageOffset, ImageZoom, ImageCornerPts[3], WndCornerPts[3]);
			ImageAPI.DrawPolyLine(hDC, WndCornerPts, 4);
		}
	}
	
	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPen); hPen = NULL;
	::DeleteObject(hPenSel); hPenSel = NULL;
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::DrawModelActivedLine(HDC hDC)//繪製選取交線
{	
	BOOL bShowLine = AOIDataCollect.GetDrawModelActivedLine();
	if ( FALSE == bShowLine ) { return ; }
	CAOIProject *Project = GetActiveProject();
	if ( NULL == Project ) { return; }
	CAOIBarcode *BarcodePtr = Project->GetProjectActiveBarcode();
	if ( NULL == BarcodePtr ) { return; }
	//if ( ComponentPtr->GetComponentDeleted() == true ) { return; }
	CAOIModel   *ModelPtr = BarcodePtr->GetBarcodeModelPtr();
	if ( NULL == ModelPtr ) { return; }	

	CString      str;
	size_t       i = 0;		
	POINT        Pt={0};
	RECT         CornerRect={0};
	TRECT4D      ImgCornerRect4d;
	TRECT4D      WndCornerRect4d;
	bool         ComponentSelected = false;
	const RECT   WndRect = m_ImageWndRect;
	TPOINT2D     StagePos, ImagePos, WndPt;
	TPOINT2D     StgCornerPos[4], ImgCornerPos[4], WndCornerPos[4];			
	double       ImageZoom = m_ImageZoom;	
	
	const IMAGE_SIZE   ImageW = GetImageW();
	const IMAGE_SIZE   ImageH = GetImageH();	
	const TPOINT2D     StageCp = GetImageStageRgnCp();
	const TPOINT2D    &ImageRes = GetImageResolution();
	const TPOINT2D    &ImageOffset = m_ImageOffset;
	const TREGION4D   &ImageStageRgn = GetImageStageRgn();	
	
	const double StageCpx = StageCp.x;
	const double StageCpy = StageCp.y;	
	const DRAW_MODEL_MODE DrawModelMode = GetDrawModelMode();
	const double BarcodeAngle = BarcodePtr->GetBarcodeAngle();
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(BarcodeAngle);
	CAOIWnd  *WndPtr  = ModelPtr->GetModelWndActived();
	CAOILand *LandPtr = ModelPtr->GetModelLandActivted();

	CAOIBox  *BoxPtr = NULL;
	if ( NULL != WndPtr )
	{	
		BoxPtr = WndPtr->GetWndBoxPtr(); 
		LandPtr = WndPtr->GetWndLandPtr();
	}
	else if ( NULL != LandPtr )
	{	BoxPtr = LandPtr->GetLandBoxPtr();	}
	else
	{	BoxPtr = ModelPtr->GetModelBodyBoxPtr(); }

	if ( DRAW_MODEL_RESULT == DrawModelMode )
	{	BoxPtr->GetBoxCornerPosStageRes(StgCornerPos);	}
	else
	{	BoxPtr->GetBoxCornerPosStage(StgCornerPos); }		
	AOIDataCollect.MapStageCornerToCamera(ImageW, ImageH, ImageRes, StgCornerPos, StageCp, ImgCornerPos);//機台4端點對應到影像四點
	JetAPI::PointsToRect(ImgCornerPos, 4, ImgCornerRect4d);
	
	HPEN hPen  = ::CreatePen(PS_SOLID, 1, 0x8080FF);	//PS_DASH
	HPEN hOldPen = (HPEN)(::SelectObject(hDC, hPen));

	if ( false == IsExceptionAngle )
	{	
		ImageAPI.MapImageRectToWndRect_DBL(ImageW, ImageH, m_ImageWndRect, ImageOffset, ImageZoom, ImgCornerRect4d, WndCornerRect4d);
		JetAPI::Rect4DToRect(WndCornerRect4d, CornerRect);
	}
	else
	{	
		ImageAPI.MapImagePtToWndPt_DBL(ImageW, ImageH, m_ImageWndRect, ImageOffset, ImageZoom, ImgCornerPos[0], WndCornerPos[0]);
		ImageAPI.MapImagePtToWndPt_DBL(ImageW, ImageH, m_ImageWndRect, ImageOffset, ImageZoom, ImgCornerPos[1], WndCornerPos[1]);
		ImageAPI.MapImagePtToWndPt_DBL(ImageW, ImageH, m_ImageWndRect, ImageOffset, ImageZoom, ImgCornerPos[2], WndCornerPos[2]);
		ImageAPI.MapImagePtToWndPt_DBL(ImageW, ImageH, m_ImageWndRect, ImageOffset, ImageZoom, ImgCornerPos[3], WndCornerPos[3]);
		JetAPI::PointsToRect(WndCornerPos, 4, WndCornerRect4d);
		JetAPI::Rect4DToRect(WndCornerRect4d, CornerRect);
		ImageAPI.DrawRectLine(hDC, CornerRect);//斜角度補上外框
	}	
	const int nCornerCpX = (CornerRect.left+CornerRect.right)/2;
	const int nCornerCpY = (CornerRect.top+CornerRect.bottom)/2;	

	::MoveToEx(hDC, WndRect.left, nCornerCpY, NULL);
	::LineTo(hDC, CornerRect.left, nCornerCpY);
	::MoveToEx(hDC, WndRect.right, nCornerCpY, NULL);
	::LineTo(hDC, CornerRect.right, nCornerCpY);

	::MoveToEx(hDC, nCornerCpX, WndRect.top, NULL);
	::LineTo(hDC, nCornerCpX, CornerRect.top);
	::MoveToEx(hDC, nCornerCpX, WndRect.bottom, NULL);
	::LineTo(hDC, nCornerCpX, CornerRect.bottom);

	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPen); hPen = NULL;
}
//-------------------------------------------------------------------------------------//
int CEditBarcodeView::GetEditLineSize()//取得編輯線的尺寸
{	
	const int    LineSizeLevel = AOIDataCollect.GetSystemParameter().m_EditLineSizeLevel;
	return JetAPI::GetEditLineSize(m_ImageZoom, LineSizeLevel);	
}
//-------------------------------------------------------------------------------------//
int CEditBarcodeView::GetEditCheckSize()//取得編輯線比較的尺寸
{
	const int    LineSizeLevel = AOIDataCollect.GetSystemParameter().m_EditLineSizeLevel;
	return JetAPI::GetEditCheckSize(m_ImageZoom, LineSizeLevel);	

	const int szImage = GetEditLineSize();
	return szImage;

	const TPOINT2D &ImageRes = GetImageResolution();
	const int szCamera = (int)(szImage*m_ImageZoom);	
	const int szStage = (int)(szCamera*ImageRes.x);	
	return szStage;
}
//-------------------------------------------------------------------------------------//
DRAW_MODEL_MODE CEditBarcodeView::GetDrawModelMode() const
{
	return DRAW_MODEL_EDIT;
	return AOIDataCollect.GetDrawModelMode();	
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::SetDrawAddRect(bool Draw)
{
	if ( false == Draw )
	{	Draw = Draw;	}
	else
	{	Draw = Draw; }
	m_DrawAddRect = Draw;
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::SetDrawEditRect(bool Draw)
{	
	m_DrawEditRect = Draw;
}
//-------------------------------------------------------------------------------------//
bool CEditBarcodeView::ExecAddBarcode()
{
	CAOIBox     *BoxPtr = NULL;
	CAOIModel   *ModelPtr = NULL;
	CAOIBoard   *BoardPtr = NULL;	
	CAOIPanel   *PanelPtr = NULL;
	CAOIBarcode *BarcodePtr = NULL;
	CAOIProject *ProjectPtr = CEditBarcodeView::GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }
	BoardPtr = ProjectPtr->GetProjectActiveBoard();
	PanelPtr = ProjectPtr->GetProjectActivePanel();	
	if ( NULL != BoardPtr )
	{	PanelPtr = BoardPtr->GetBoardPanelPtr();	}	
	if ( NULL == PanelPtr )
	{	PanelPtr = ProjectPtr->GetProjectPanelPtr(0, true);	}

	size_t       i=0;	
	CString      str;			
	DISTRICT_ID  DistrictID = ProjectPtr->GetProjectActDistrictID();	
	const int    BarcodeGroupID = ProjectPtr->GetProjectBarcodeFreeGroupID();
	if ( NULL != PanelPtr )
	{	
		CString      strName;
		CString      strValue;
		CString      strCaption;
		CInputBoxWnd InputBox;	
		const size_t PanelBoardCount = PanelPtr->GetPanelBoardCount();
		const size_t PanelBarcodeCount = PanelPtr->GetPanelBarcodeCount();

		str = _T("Set Board Index");
		str = LoadMultiLanguageString(str, str);
		strCaption.Format(_T("%s [Panel:%d]"), str, PanelPtr->GetPanelIndex_Project()+1);
		str = _T("Board Index");
		str = LoadMultiLanguageString(str, str);
		strName.Format(_T("%s (0 ~ %d) [0:Panel Barcode]:"), str, PanelBoardCount);	

		if ( NULL == BoardPtr )
		{	strValue = _T("0");	}
		else
		{	strValue.Format(_T("%d"), BoardPtr->GetBoardIndex_Panel()+1);	}
		InputBox.SetParam1(strCaption, strName, strValue);
		if ( InputBox.DoModal() == IDCANCEL ) 
		{	return false;  }
		unsigned int PanelBoardIndex = ::_ttoi(InputBox.m_DataEdit1)-1;
		BoardPtr = PanelPtr->GetPanelBoardPtr(PanelBoardIndex, true);		
	}	
	
	TSIZE2D   ModelSize;
	TPOINT2D  ModelStageCp;
	TREGION4D RgnWndBox;
	TREGION4D RgnImgBox;
	TREGION4D RgnStgBox;
	TREGION4D RgnCadBox;
	double    CadPosX=0, CadPosY=0;
	POINT pt1 = m_MousePosLast;
	POINT pt2 = m_MousePosFirst;	
	
	const IMAGE_SIZE ImageW = GetImageW();
	const IMAGE_SIZE ImageH = GetImageH();	
	const TPOINT2D   StageCp = GetImageStageRgnCp();
	const TPOINT2D  &ImageRes = GetImageResolution();
	const double BarcodeAngle = 0;
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(BarcodeAngle);
	PtInControlWnd(m_MousePosLast, MODELEDIT_IMAGE_WND, pt1);
	PtInControlWnd(m_MousePosFirst, MODELEDIT_IMAGE_WND, pt2);	

	const bool   LinkMode = true;
	const MODEL_TYPE    ModelType   = MODEL_TYPE_BARCODE;
	const ALG_TYPE      AlgType     = ALG_BARCODE_RECOGNIZE;	
	const WND_DEFECT_ID WndDefectID = WND_DEFECT_BODY_WRONG_CODE;	

	RgnWndBox.minX = MIN(pt1.x, pt2.x);
	RgnWndBox.maxX = MAX(pt1.x, pt2.x);
	RgnWndBox.minY = MIN(pt1.y, pt2.y);
	RgnWndBox.maxY = MAX(pt1.y, pt2.y);
	ImageAPI.MapWndRgnToImageRgn_DBL(ImageW, ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, RgnWndBox, RgnImgBox);	
	AOIDataCollect.MapCameraRegionToStage(ImageW, ImageH, ImageRes, RgnImgBox, StageCp, RgnStgBox);
	AOIDataCollect.MapStageOffsetRgnToCad(RgnStgBox, RgnCadBox);

	BarcodePtr = AOIObjManager.CreateBarcodeObj();
	if ( NULL == BarcodePtr ) { return false; }	
	BarcodePtr->SetBarcodeGroupID(BarcodeGroupID);
	ProjectPtr->SelectProjectAllBarcodes(false);
	ProjectPtr->AddProjectBarcodePtr(BarcodePtr, false);
	if ( NULL != PanelPtr )
	{	PanelPtr->AddPanelBarcodePtr(BarcodePtr); }
	if ( NULL != BoardPtr )
	{	BoardPtr->AddBoardBarcodePtr(BarcodePtr); }

	ModelPtr = BarcodePtr->GetBarcodeModelPtr();
	ModelStageCp.x = RgnStgBox.GetCpX();
	ModelStageCp.y = RgnStgBox.GetCpY();
	ModelSize.cx   = RgnCadBox.GetWidth();
	ModelSize.cy   = RgnCadBox.GetHeight();
	
	if ( NULL != PanelPtr )
	{
		CMapCoordinate *STCPtr = NULL;
		if ( NULL != BoardPtr )
		{	STCPtr = BoardPtr->GetBoardMapSTCPtr(DistrictID); }
		else
		{	STCPtr = PanelPtr->GetPanelMapSTCPtr(DistrictID); }
		if ( NULL != STCPtr )
		{	STCPtr->Map2D(ModelStageCp.x, ModelStageCp.y, CadPosX, CadPosY); }
	}
	else
	{
		CMapCoordinate MapSTC;		
		ProjectPtr->GetProjectMapSTC(DistrictID, MapSTC);
		MapSTC.Map2D(ModelStageCp.x, ModelStageCp.y, CadPosX, CadPosY);
	}

	BarcodePtr->SetBarcodeSelected(true);
	BarcodePtr->CheckBarcodeBelongMode();
	BarcodePtr->SetBarcodeAngle(0);	
	BarcodePtr->SetBarcodeCadPosX(CadPosX);
	BarcodePtr->SetBarcodeCadPosY(CadPosY);		
	BarcodePtr->SetBarcodeRoiSizeW(ModelSize.cx);
	BarcodePtr->SetBarcodeRoiSizeH(ModelSize.cy);
	BarcodePtr->SetBarcodeBodySizeW(ModelSize.cx);
	BarcodePtr->SetBarcodeBodySizeH(ModelSize.cy);
	BarcodePtr->SetBarcodeStagePosX(ModelStageCp.x);
	BarcodePtr->SetBarcodeStagePosY(ModelStageCp.y);
	BarcodePtr->SetBarcodeDistrictID(DistrictID);
	BarcodePtr->CalcBarcodeCadCornerPos();//計算軟體條碼Cad端點座標		
	BarcodePtr->LayoutBarcodeStageCornerPos();//更新軟體條碼機台端點座標
	
	const int BarcodeUniqueID = BarcodePtr->GetBarcodeUniqueID();
	ModelPtr->SetModelType(ModelType);
	ModelPtr->SetModelAttachedAngle(0);
	ModelPtr->SetModelAttachedPosCad(CadPosX, CadPosY);
	ModelPtr->SetModelAttachedPosStage(ModelStageCp);

	BoxPtr = ModelPtr->GetModelBodyBoxPtr();
	if ( NULL != BoxPtr )
	{
		BoxPtr->SetBoxSizeX(ModelSize.cx);
		BoxPtr->SetBoxSizeY(ModelSize.cy);		
		BoxPtr->LayoutBoxCornerPos();		
		BoxPtr->ResetBoxRegionRes();
	}

	if ( false == IsExceptionAngle )
	{
		RgnWndBox.minX = MIN(pt1.x, pt2.x);
		RgnWndBox.maxX = MAX(pt1.x, pt2.x);
		RgnWndBox.minY = MIN(pt1.y, pt2.y);
		RgnWndBox.maxY = MAX(pt1.y, pt2.y);
		ImageAPI.MapWndRgnToImageRgn_DBL(ImageW, ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, RgnWndBox, RgnImgBox);	
		AOIDataCollect.MapCameraRegionToStage(ImageW, ImageH, ImageRes, RgnImgBox, StageCp, RgnStgBox);
		RgnStgBox.minX -= ModelStageCp.x;
		RgnStgBox.minY -= ModelStageCp.y;
		RgnStgBox.maxX -= ModelStageCp.x;
		RgnStgBox.maxY -= ModelStageCp.y;		
		AOIDataCollect.MapStageOffsetRgnToCad(RgnStgBox, RgnCadBox);
	}
	else
	{
		TPOINT2D Cp, dPt1, dPt2;		
		TPOINT2D CornerPoint[4];
		const double ImageAngle = JetAPI::MapCadAngleToImageAngle(BarcodeAngle);
		dPt1 = pt1;
		dPt2 = pt2;		
		Cp.x = (dPt1.x+dPt2.x)*0.5;
		Cp.y = (dPt1.y+dPt2.y)*0.5;		
		JetAPI::RotatePos(-ImageAngle, Cp.x, Cp.y, dPt1);
		JetAPI::RotatePos(-ImageAngle, Cp.x, Cp.y, dPt2);		

		RgnWndBox.minX = MIN(dPt1.x, dPt2.x);
		RgnWndBox.maxX = MAX(dPt1.x, dPt2.x);
		RgnWndBox.minY = MIN(dPt1.y, dPt2.y);
		RgnWndBox.maxY = MAX(dPt1.y, dPt2.y);

		ImageAPI.MapWndRgnToImageRgn_DBL(ImageW, ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, RgnWndBox, RgnImgBox);	
		AOIDataCollect.MapCameraRegionToStage(ImageW, ImageH, ImageRes, RgnImgBox, StageCp, RgnStgBox);
		RgnStgBox.minX -= ModelStageCp.x;
		RgnStgBox.minY -= ModelStageCp.y;
		RgnStgBox.maxX -= ModelStageCp.x;
		RgnStgBox.maxY -= ModelStageCp.y;
		AOIDataCollect.MapStageOffsetRgnToCad(RgnStgBox, RgnCadBox);
	}
	
	CAOIWnd *WndPtr = ModelPtr->CreateModelWnd(WndDefectID, RgnCadBox, NULL);
	if ( NULL == WndPtr )
	{
		JetAPI::ShowMessageBox(_T("Error, Create Model Wnd Ptr Fault"));
		return false;
	}	
	WndPtr->GetWndAlgParam().ChangeAlgType(AlgType, true);//順序不要顛倒
	WndPtr->ChangeWndDefectID(ModelType, WndDefectID);	
	WndPtr->UpdateWndExtendBox();
	const bool WndRgnLinkAuto = WndPtr->GetWndRgnLinkAuto();

	ModelPtr->UnSelectModel();	
	ModelPtr->AddModelWndPtr(WndPtr, false);	
	if ( true == WndRgnLinkAuto )
	{	ModelPtr->UpdateModelWndRgnByLinkMode();	}

	ModelPtr->CalcModelTotalRegionAll();
	ModelPtr->UpdateModelBodyToBarcode();

	WndPtr->SetWndSelected(true);
	ModelPtr->SetModelWndActived(WndPtr);		
	WndPtr = ModelPtr->GetModelWndActived();
	MANIPULATE_MODEL_MODE ManiMode = GetManipulateModelModeDefault();
	AOIDataCollect.SetManipulateModelMode(ManiMode);	
	SetModel(ModelPtr);
	UpdateBarcodeModelStats();
	RedrawWnd();

	ProjectPtr->ResetProjectActiveIndex();
	const unsigned int PanelIndex = BarcodePtr->GetBarcodePanelIndex_Project();
	const unsigned int BoardIndex = BarcodePtr->GetBarcodeBoardIndex_Project();
	const unsigned int BarcodeIndex = BarcodePtr->GetBarcodeIndex_Project();
	ProjectPtr->SetProjectActivePanelIndex(PanelIndex);
	ProjectPtr->SetProjectActiveBoardIndex(BoardIndex);
	ProjectPtr->SetProjectActiveBarcodeIndex(BarcodeIndex);
	ProjectPtr->SetProjectActiveModelWnd(WndPtr);	
	LogOperCtrl.SaveLogModelWndSelectedCreate(ModelPtr);

	SendMessageToMainFrameWnd(MSG_EDIT_PART_LIST_WND, WPARAM_BUILD_PART_LIST, LPARAM_BUILD_DOCK_LIST_PART);
	SendMessageToMainFrameWnd(MSG_EDIT_PART_LIST_WND, WPARAM_UPDATE_PART_SELECTED, NULL);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditBarcodeView::ResetBarcodeModel()
{		
	SetModel(NULL);
	m_ActiveObjList.clear();
	MANIPULATE_MODEL_MODE ManiMode = GetManipulateModelModeDefault();
	AOIDataCollect.SetManipulateModelMode(ManiMode);
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::SetModel(CAOIModel *Ptr)
{
	m_ModelPtr = Ptr;
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::UpdateBarcodeModelStats()
{
	CAOIModel *ModelPtr = GetModelPtr();	
	BuildActiveObjList(ModelPtr, false);
	if ( NULL != ModelPtr )
	{		
		const int  nAlign = 4;
		const bool bClone=false;
		const bool bNoFilter=false;		
		std::vector<TUNI_FRAME> UniFrameList;
		BuildModelUniFrameList(ModelPtr, UniFrameList, nAlign, bClone, bNoFilter);
		if ( true == bClone )
		{	JetAPI::ClearUniFrameList(UniFrameList); }
	}
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::SendOutUpdatePartList(int UpdateList, int UpdateWnd)
{	
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }
	ProjectPtr->SetProjectActiveModelWnd(NULL);

	switch ( UpdateList )
	{
	case MSG_MODE_BUILD:
		SendMessageToMainFrameWnd(MSG_EDIT_PART_LIST_WND, WPARAM_BUILD_PART_LIST, NULL);
		break;
	case MSG_MODE_UPDATE:
		SendMessageToMainFrameWnd(MSG_EDIT_PART_LIST_WND, WPARAM_UPDATE_PART_LIST, TREE_CTRL_UPDATE_NULL);
		break;	
	}	

	switch ( UpdateWnd )
	{
	case MSG_MODE_BUILD:
		SendMessageToMainFrameWnd(MSG_EDIT_WND_PROPERTY_WND, WPARAM_BUILD_WND_SELECTED, NULL);
		break;
	case MSG_MODE_UPDATE:
		SendMessageToMainFrameWnd(MSG_EDIT_WND_PROPERTY_WND, WPARAM_UPDATE_WND_SELECTED, NULL);
		break;
	}
	return;
}
//-------------------------------------------------------------------------------------//
bool CEditBarcodeView::GetShowPopupMenu() const
{
	return m_ShowPopupMenu;
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::SetShowPopupMenu(bool val)
{
	m_ShowPopupMenu = val;
}
//-------------------------------------------------------------------------------------//
bool CEditBarcodeView::ExecSelectBarcode()
{	
	CAOIProject *Project = GetActiveProject();
	if ( NULL == Project ) { return false; }	

	size_t    i=0;
	TREGION4D Rgn;
	const bool bResultMode = false;	
	if ( GetStageSelectRegion(Rgn) == false ) { return false; }
	const double CpX = Rgn.GetCpX();
	const double CpY = Rgn.GetCpY();

	Rgn.minX = Rgn.maxX = CpX;
	Rgn.minY = Rgn.maxY = CpY;

	m_DrawEditRect = false;
	m_EditRegion = TREGION4D();
	
	Project->ResetProjectActiveIndex();
	if ( AOIDataCollect.CheckMultiSelectMode() == false )
	{	Project->SelectProjectAllObjects(false);	}	
	
	CAOIBarcode *BarcodePtr = NULL;
	std::vector<CAOIBarcode*> BarcodeList;
	Project->SelectProjectBarcodesByStage(Rgn, bResultMode, BarcodeList);
	const size_t SelCount = BarcodeList.size();
	if ( 0 == SelCount ) { return false; }		
	BarcodePtr = BarcodeList[0];
	
	BarcodePtr->SetBarcodeSelected(true);
	const unsigned int PanelIndex = BarcodePtr->GetBarcodePanelIndex_Project();
	const unsigned int BoardIndex = BarcodePtr->GetBarcodeBoardIndex_Project();
	const unsigned int BarcodeIndex = BarcodePtr->GetBarcodeIndex_Project();		
	Project->SetProjectActivePanelIndex(PanelIndex);	
	Project->SetProjectActiveBoardIndex(BoardIndex);	
	Project->SetProjectActiveBarcodeIndex(BarcodeIndex);	

	CAOIModel *ModelPtr = BarcodePtr->GetBarcodeModelPtr();
	SetModel(ModelPtr);
	UpdateBarcodeModelStats();	
	RedrawWnd();

	PostMessageToMainFrameWnd(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_UPDATE, (LPARAM)(this));	
	return true;
}
//-------------------------------------------------------------------------------------//
CURSOR_POS_MODE CEditBarcodeView::CheckCursorPosModeBarcode(POINT pt)//確認鼠標座標模式
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
	const size_t NObjects = this->m_ActiveObjList.size();	
	CAOIModel   *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) 
	{	return CursorMode; }	
	if ( ModelPtr->GetModelEditMode() == false )
	{	return CursorMode; }	
	DRAW_MODEL_MODE DrawModelMode = GetDrawModelMode();
	if ( DRAW_MODEL_EDIT != DrawModelMode ) 
	{	return CursorMode; }
	MANIPULATE_MODEL_MODE ManiMode = AOIDataCollect.GetManipulateModelMode();
	const IMAGE_SIZE ImageW = GetImageW();
	const IMAGE_SIZE ImageH = GetImageH();	

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
			//if ( false == ObjPtr->Focused ) { continue; }			

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
void CEditBarcodeView::PreInitUniFrameBuffer()//預先影像記憶體
{	
	IMAGE_SIZE ImageW = 1024;
	IMAGE_SIZE ImageH = 1024;
	
	m_ImageIndex = 0;
	m_FrameStageRgn.maxX = (ImageW/2)*m_FrameResolution.x;
	m_FrameStageRgn.maxY = (ImageH/2)*m_FrameResolution.y;
	m_FrameStageRgn.minX = -m_FrameStageRgn.maxX;
	m_FrameStageRgn.minY = -m_FrameStageRgn.maxY;
	JetAPI::InitialUniFrameList(m_UniFrameList, FRAME_MAX_COUNT);
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::ReleaseUniFrameBuffer()//釋放影像記憶體	
{
	AOIDataCollect.ReleaseFieldUniFrameList();
	JetAPI::ClearUniFrameList(m_UniFrameList, FRAME_MAX_COUNT);	
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::BuildShowImageBuffer()//建立顯示的影像記憶體
{
	const char fnName[] = "CEditBarcodeView::BuildShowImageBuffer";
	CEditBarcodeView::ReleaseShowImageBuffer();

	MASK_PTR   MaskPtr=NULL;
	SPACE_PTR  SpacePtr=NULL;
	IMAGE_PTR  ImagePtr = NULL;
	IMAGE_SIZE ImageW=0, ImageH=0, ImageStep=0, BitCount=0;
	if ( GetCurrentFrame(ImageW, ImageH, ImageStep, BitCount, ImagePtr, SpacePtr, MaskPtr) == false ) { return; }
	if ( NULL == ImagePtr )
	{
		const size_t ShowBufferSize = ImageAPI.CalcBufferSize(m_ShowImageStep, m_ShowImageH);
		if ( JetMemory.alloc_func(ShowBufferSize, m_ShowImagePtr, fnName, "m_ShowImagePtr") == false )
		{	return;		}
		::memset(m_ShowImagePtr, 0x00, sizeof(IMAGE_DATA)*ShowBufferSize);
	}
	else
	{
		m_ShowBitCount = 24;
		m_ShowImageW = ImageW;
		m_ShowImageH = ImageH;
		m_ShowImageStep = JetAPI::GetBMPImagePixelsPerLine(ImageW, m_ShowBitCount, 4);
		const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
		const size_t ShowBufferSize = ImageAPI.CalcBufferSize(m_ShowImageStep, m_ShowImageH);
		if ( JetMemory.alloc_func(ShowBufferSize, m_ShowImagePtr, fnName, "m_ShowImagePtr") == false )
		{	return;		}
		if ( ShowBufferSize == BufferSize )
		{	::memcpy(m_ShowImagePtr, ImagePtr, sizeof(IMAGE_DATA)*BufferSize);	}
		else
		{
			if ( ImageAPI.RGBImageToColorImage3(ImageW, ImageH, ImageStep, ImagePtr, ImagePtr, ImagePtr, m_ShowImageStep, m_ShowImagePtr, false) == false )
			{
				ReleaseShowImageBuffer();
				return;
			}
		}
	}

	DRAW_IMAGE_MODE DrawImageMode = AOIDataCollect.GetDrawImageMode();
	if ( DRAW_IMAGE_BY_RAW != DrawImageMode )
	{
		AOIDataCollect.SetDrawingImageMode(DRAW_IMAGE_NORMAL);
		AOIDataCollect.ExecEnhanceDisplayImage(m_ShowImageW, m_ShowImageH, m_ShowImageStep, m_ShowBitCount, m_ShowImagePtr, m_ShowImagePtr);
	}
	else
	{	AOIDataCollect.SetDrawingImageMode(DRAW_IMAGE_BY_RAW);	}
	return;
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::ReleaseShowImageBuffer()//釋放顯示影像記憶體	
{
	if ( NULL != m_ShowImagePtr )
	{	JetMemory.free_func(m_ShowImagePtr); }
	m_ShowImageW = 1024;
	m_ShowImageH = 1024;
	m_ShowImageStep = 1024*3;
	m_ShowBitCount = 24;	
}
//-------------------------------------------------------------------------------------//	
bool CEditBarcodeView::GetStageSelectRegion(TREGION4D &StageRgn)//取得選取區域-機台座標
{
	TPOINT2D  WndPt1, ImagePt1, StagePt1;
	TPOINT2D  WndPt2, ImagePt2, StagePt2;
	const IMAGE_SIZE ImageW = GetImageW();
	const IMAGE_SIZE ImageH = GetImageH();
	const TPOINT2D  StageCp = GetImageStageRgnCp();
	const TPOINT2D &ImageRes = GetImageResolution();

	StagePt1 = ImagePt1 = WndPt1 = m_MousePosFirst;
	StagePt2 = ImagePt2 = WndPt2 = m_MousePosLast;
	MapWndPtToImagePt_DBL(ImageW, ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, WndPt1, ImagePt1);
	MapWndPtToImagePt_DBL(ImageW, ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, WndPt2, ImagePt2);
	AOIDataCollect.MapCameraPtToStage(ImageW, ImageH, ImageRes, ImagePt1, StageCp, StagePt1);
	AOIDataCollect.MapCameraPtToStage(ImageW, ImageH, ImageRes, ImagePt2, StageCp, StagePt2);
	StageRgn.minX = MIN(StagePt1.x, StagePt2.x);
	StageRgn.maxX = MAX(StagePt1.x, StagePt2.x);
	StageRgn.minY = MIN(StagePt1.y, StagePt2.y);
	StageRgn.maxY = MAX(StagePt1.y, StagePt2.y);
	return true;
}
//-------------------------------------------------------------------------------------//	
bool CEditBarcodeView::GetCurrentFrame(IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, IMAGE_PTR &ImagePtr, SPACE_PTR &SpacePtr, MASK_PTR &MaskPtr)//取得目前影像
{
	if ( GetShowProjectMapMode() == true ) { return false; }
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
bool CEditBarcodeView::GetFrameImage(unsigned int Index, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, IMAGE_PTR &ImagePtr, SPACE_PTR &SpacePtr, MASK_PTR &MaskPtr)//取得目前影像
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
int CEditBarcodeView::GetMaxFrameCount()
{
	int FrameMaxCount = 1;
#ifdef _X64
	FrameMaxCount = FRAME_MAX_COUNT;
#else
	FrameMaxCount = FRAME_MAX_COUNT;//1
#endif//_X64
	return FrameMaxCount;
}
//-------------------------------------------------------------------------------------//
bool CEditBarcodeView::AdjustCurrentFrames()
{
	//確認是否重新補圖	
	RECT      WndImageRect={0};
	const IMAGE_SIZE ImageW = GetImageW();
	const IMAGE_SIZE ImageH = GetImageH();	
	const int RealWndW = m_ImageWndRect.right-m_ImageWndRect.left;
	const int RealWndH = m_ImageWndRect.bottom-m_ImageWndRect.top;
	const int RealWndCpx = RealWndW/2;
	const int RealWndCpy = RealWndH/2;
	const int WndSizeW = (int)(ImageW/m_ImageZoom);
	const int WndSizeH = (int)(ImageH/m_ImageZoom);	
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();
	IMAGE_SIZE BasicImageW = AOIDataCollect.GetCameraImageW(PRIMARY_CAMERA_ID);
	IMAGE_SIZE BasicImageH = AOIDataCollect.GetCameraImageH(PRIMARY_CAMERA_ID);	

	double Ratio = 1;
	double RatioW = (double)(ImageW);
	double RatioH = (double)(ImageH);
	double dBImageW = (double)(BasicImageW);
	double dBImageH = (double)(BasicImageH);
	RatioW = RatioW/dBImageW;
	RatioH = RatioH/dBImageH;
			
	Ratio = MAX(RatioW, RatioH);
	if ( Ratio < 1.0 ) 
	{	Ratio = 1.0; }

	WndImageRect.left   = (LONG)(RealWndCpx+m_ImageOffset.x-(WndSizeW/2));
	WndImageRect.top    = (LONG)(RealWndCpy+m_ImageOffset.y-(WndSizeH/2));
	WndImageRect.right  = (LONG)(RealWndCpx+m_ImageOffset.x+(WndSizeW/2));
	WndImageRect.bottom = (LONG)(RealWndCpy+m_ImageOffset.y+(WndSizeH/2));

	if ( WndImageRect.left > m_ImageWndRect.left || 
		 WndImageRect.top > m_ImageWndRect.top ||
		 WndImageRect.right < m_ImageWndRect.right ||
		 WndImageRect.bottom < m_ImageWndRect.bottom )
	{	
		CalcFovPosition();
		if ( false == OfflineMode )
		{	CWnd::PostMessage(MSG_CAMERA_REGRAB_IMAGE, NULL, NULL);	}
		else
		{
			FillCurrentFrames(Ratio);
			CEditBarcodeView::BuildShowImageBuffer();
			CEditBarcodeView::CreateBKImage();
			CEditBarcodeView::RedrawWnd();
		}
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditBarcodeView::FillCurrentFrames(double Ratio)
{	
	size_t   i=0;
	TSIZE2D  Res;
	TPOINT3D Pos;	
	const size_t MaxFrames = CEditBarcodeView::GetMaxFrameCount();
	IMAGE_SIZE ImageW = AOIDataCollect.GetCameraImageW(PRIMARY_CAMERA_ID);
	IMAGE_SIZE ImageH = AOIDataCollect.GetCameraImageH(PRIMARY_CAMERA_ID);
	const double FOVWum = Ratio*AOIDataCollect.GetFovSizeRealW();
	const double FOVHum = Ratio*AOIDataCollect.GetFovSizeRealH();
	
	Pos.z = 0;
	Pos.x = m_FOVPosStage.x;
	Pos.y = m_FOVPosStage.y;

	Res.cx = m_FrameResolution.x;
	Res.cy = m_FrameResolution.y;

	ReleaseUniFrameBuffer();
	ReleaseShowImageBuffer();

	m_FovRatio = Ratio;	
	m_FrameStageRgn.minX = Pos.x-(FOVWum*0.5);
	m_FrameStageRgn.maxX = Pos.x+(FOVWum*0.5);
	m_FrameStageRgn.minY = Pos.y-(FOVHum*0.5);
	m_FrameStageRgn.maxY = Pos.y+(FOVHum*0.5);

	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr )
	{	return true; }

	HCURSOR hCursor=NULL;
	HCURSOR hOldCursor=NULL;
	CWinApp *AppPtr = ::AfxGetApp();
	if ( NULL != AppPtr )
	{	
		hCursor = AppPtr->LoadStandardCursor(IDC_WAIT); 
		hOldCursor = ::SetCursor(hCursor);
	}		
		
	ImageW = (IMAGE_SIZE)(ImageW*Ratio);
	ImageH = (IMAGE_SIZE)(ImageH*Ratio);
	OFFLINE_FILE_MODE OfflineFileMode = ProjectPtr->GetProjectOfflineFileMode();
	if ( ProjectPtr->FillCurrentFrame(OfflineFileMode, Pos, Res, ImageW, ImageH, m_UniFrameList, MaxFrames) == false )
	{	JetAPI::ShowMessageBox(ProjectPtr->GetErrorString());	}

	m_ImageIndex = ProjectPtr->GetProjectMapIndex();
	for ( i=0; i<FRAME_MAX_COUNT; i++ )
	{
		if ( NULL == m_UniFrameList[i].ImagePtr ) { continue; }		
		m_UniFrameList[i].ImageW = ImageW;
		m_UniFrameList[i].ImageH = ImageH;
	}
	AOIDataCollect.SetFieldUniFrameList(m_FrameStageRgn, m_UniFrameList, MaxFrames);	
	if ( NULL != hOldCursor )
	{	::SetCursor(hOldCursor);	}		

	m_ShowImageW = ImageW;
	m_ShowImageH = ImageH;		
	m_ShowBitCount = 24;	
	m_ShowImageStep = JetAPI::GetBMPImagePixelsPerLine(m_ShowImageW, m_ShowBitCount, 4);	
	UpdateActiveObjList();		
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::BackupViewParam()//備份顯示參數
{
	AOIDataCollect.SetViewImageZoom(m_ImageZoom);
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::RestoreViewParam()//恢復顯示參數
{
	m_ImageZoom = AOIDataCollect.GetViewImageZoom();
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::CalcFovPosition()
{
	if ( GetLockUIWnd() == true ) { return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }

	TPOINT2D CadOffset, StageOffset;
	const double ImageZoom = m_ImageZoom;
	const TPOINT2D StageCp = GetImageStageRgnCp();
	const TPOINT2D &ImageRes = GetImageResolution();	
	const double ImageResolutionX = ImageRes.x;
	const double ImageResolutionY = ImageRes.y;
	const double StageCpx = StageCp.x;
	const double StageCpy = StageCp.y;
	const double ImageOffsetX = m_ImageOffset.x;
	const double ImageOffsetY = m_ImageOffset.y;
	CadOffset.x =  -ImageOffsetX*ImageResolutionX*ImageZoom;
	CadOffset.y =   ImageOffsetY*ImageResolutionY*ImageZoom;
	AOIDataCollect.MapCadOffsetPtToStage(CadOffset, StageOffset);
	const double NewStagePosX = StageCpx+StageOffset.x;
	const double NewStagePosY = StageCpy+StageOffset.y;	
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();	
	MotionCtrlPtr->XYMoveTo(NewStagePosX, NewStagePosY, OfflineMode);
	MotionCtrlPtr->WaitForMotionStop();
	ResetImageOffset();
	m_FOVPosStage.x = NewStagePosX;
	m_FOVPosStage.y = NewStagePosY;
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::ResetImageOffset()//復歸顯示移動值	
{
	m_ImageOffset.x = 0;
	m_ImageOffset.y = 0;
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::SwitchFrameImage()
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }
	m_ImageIndex = ProjectPtr->GetProjectMapIndexNext(m_ImageIndex);	
	ProjectPtr->SetProjectMapIndex(m_ImageIndex);
	BuildShowImageBuffer();
	CreateBKImage();
	RedrawWnd();	
	
	PostMessageToMainFrameWnd(MSG_EDIT_IMAGE_VIEW_WND, WPARAM_UPDATE_IMAGE_SWITCH_FRAME, NULL);	
	//PostMessageToMainFrameWnd(MSG_EDIT_VIEW_3D_WND, WPARAM_UPDATE_3D_DATA, NULL);	
	return;
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::UpdateFrameImage()
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr )	{	return;	}		
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();
	m_ImageIndex = ProjectPtr->GetProjectMapIndex();
	if ( false == OfflineMode )
	{
		double PosX=0, PosY=0, PosZ=0;	
		if ( AOIDataCollect.GetStagePos(PosX, PosY, PosZ) == false )
		{
			JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
			return;
		}
		SetKeepImageOffset(true);
		ExecGrabFov(PosX, PosY, PosZ);
		return ;
	}
	FillCurrentFrames(m_FovRatio);
	BuildShowImageBuffer();
	CreateBKImage();
	RedrawWnd();
	PostMessageToMainFrameWnd(MSG_EDIT_IMAGE_VIEW_WND, WPARAM_UPDATE_IMAGE_SWITCH_FRAME, NULL);	
	//PostMessageToMainFrameWnd(MSG_EDIT_VIEW_3D_WND, WPARAM_UPDATE_3D_DATA, NULL);	
	return;
}
//-------------------------------------------------------------------------------------//	
bool CEditBarcodeView::ExecMoveToStage()
{
	if ( this->GetLockUIWnd() == true ) { return true; }
	if ( MotionCtrlPtr->WaitForMotionStop() == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return false;
	}
	
	bool   IsOK = true;
	double PosX=0, PosY=0, PosZ=0;
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();		
	IsOK = MotionCtrlPtr->GetCurrentPos(PosX, PosY, PosZ, OfflineMode);
	if ( IsOK == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return false;
	}	
	ResetImageOffset();
	if ( true == OfflineMode )
	{	return ExecUpdateFov(PosX, PosY, PosZ);	}

	return ExecGrabFov(PosX, PosY, PosZ);
}
//-------------------------------------------------------------------------------------//	
bool CEditBarcodeView::ExecShowWndPosition()
{	
	const double PosX = AOIDataCollect.GetFovPositionX();
	const double PosY = AOIDataCollect.GetFovPositionY();	
	const double TargetOffsetX = AOIDataCollect.GetFovTargetOffsetX();
	const double TargetOffsetY = AOIDataCollect.GetFovTargetOffsetY();

	AOIDataCollect.ResetFovTargetParam();
	
	TPOINT2D PosCad;
	TPOINT2D FovOffset;
	TPOINT2D PosStage(TargetOffsetX, TargetOffsetY);
	const TPOINT2D StageCp = GetImageStageRgnCp();
	const TPOINT2D &ImageRes = GetImageResolution();	

	//想要的位置與目前圖像的機台位置偏差量
	FovOffset.x = PosX-StageCp.x;
	FovOffset.y = PosY-StageCp.y;
	//疊加機台的偏差量
	PosStage.x += FovOffset.x;
	PosStage.y += FovOffset.y;
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
bool CEditBarcodeView::ExecGrabFov(double PosX, double PosY, double PosZ)
{
	CString str;
	if ( this->GetLockUIWnd() == true ) { return true; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }		
	
	HWND hWnd = CWnd::GetSafeHwnd();
	AOIDataCollect.SetCallbackWnd(hWnd);
	//AOIDataCollect.MoveCameraToProjectFocusPos(ProjectPtr);
#ifndef LIGHT_CTRL_DISABLE
	this->LockUIWnd(true);	
	if ( AOIDataCollect.ExecGrabNextUniFrameImage() == false )
	{		
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		this->LockUIWnd(false);
		return FALSE;
	}	
	return true;
#endif//LIGHT_CTRL_DISABLE
	return false;
}
//-------------------------------------------------------------------------------------//	
bool CEditBarcodeView::ExecUpdateFov(double PosX, double PosY, double PosZ)
{
	const double FovSizeW = AOIDataCollect.GetFovSizeRealW();
	const double FovSizeH = AOIDataCollect.GetFovSizeRealH();	
	const double FovMinW = AOIDataCollect.GetFovSizeMinW_Zoom();
	const double FovMinH = AOIDataCollect.GetFovSizeMinH_Zoom();	
	const double TargetMinW = AOIDataCollect.GetTargetMinSizeW_Zoom();
	const double TargetMinH = AOIDataCollect.GetTargetMinSizeH_Zoom();
	const double FovSizeWd2 = FovSizeW/2;
	const double FovSizeHd2 = FovSizeH/2;
	const double FovSizeWd4 = FovSizeW/4;
	const double FovSizeHd4 = FovSizeH/4;
	const double ZoomMin = AOIDataCollect.GetImageZoomMin();
	const double ZoomMax = AOIDataCollect.GetImageZoomMax();
	const double TargetOffsetX = AOIDataCollect.GetFovTargetOffsetX();
	const double TargetOffsetY = AOIDataCollect.GetFovTargetOffsetY();
	const double FovZoomX = FovMinW/FovSizeW;
	const double FovZoomY = FovMinH/FovSizeH;
	const double FovZoomNeed = MAX(FovZoomX, FovZoomY);	
	const double FovZoomNeedUsed = JetAPI::AdjustValue(FovZoomNeed, 0.5);

	TPOINT2D ImageRes;
	IMAGE_SIZE ImageW = 0;
	IMAGE_SIZE ImageH = 0;
	IMAGE_SIZE ImageStep = 0;
	IMAGE_SIZE BitCount = 0;
	IMAGE_PTR  ImagePtr = NULL;
	MASK_PTR   MaskPtr=NULL;
	SPACE_PTR  SpacePtr=NULL;
	
	if ( m_ImageZoom < FovZoomNeedUsed )
	{	m_ImageZoom = FovZoomNeedUsed; }
	ImageRes.x = AOIDataCollect.GetCameraResolutionX(PRIMARY_CAMERA_ID);
	ImageRes.y = AOIDataCollect.GetCameraResolutionY(PRIMARY_CAMERA_ID);
	if ( GetCurrentFrame(ImageW, ImageH, ImageStep, BitCount, ImagePtr, SpacePtr, MaskPtr)==true && m_ImageWndRect.right>0 )
	{
		//視窗轉成影像
		TPOINT2D WndPt1, WndPt2;
		TPOINT2D ImagePt1, ImagePt2;
		TPOINT2D StagePt1, StagePt2;

		WndPt1.x = m_ImageWndRect.left;
		WndPt1.y = m_ImageWndRect.top;
		WndPt2.x = m_ImageWndRect.right;
		WndPt2.y = m_ImageWndRect.bottom;

		ImageAPI.MapWndPtToImagePt_DBL(ImageW, ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, WndPt1, ImagePt1);	
		ImageAPI.MapWndPtToImagePt_DBL(ImageW, ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, WndPt2, ImagePt2);	

		//影像轉成機台
		AOIDataCollect.MapCameraPtToStage(ImageW, ImageH, ImageRes, ImagePt1, m_FOVPosStage, StagePt1);
		AOIDataCollect.MapCameraPtToStage(ImageW, ImageH, ImageRes, ImagePt2, m_FOVPosStage, StagePt2);
	
		double FovViewW = ::fabs(StagePt1.x-StagePt2.x);//FovSizeWd4;
		double FovViewH = ::fabs(StagePt1.y-StagePt2.y);//FovSizeHd4;
		if ( FovMinW>FovViewW || FovMinH>FovViewH )
		{
			FovViewW = FovSizeWd4;
			FovViewH = FovSizeHd4;
			
			const double ZoomX = TargetMinW/FovViewW;
			const double ZoomY = TargetMinH/FovViewH;
			const double ZoomBigger = MAX(ZoomX, ZoomY);
			const double ZoomMin2 = AOIDataCollect.GetImageZoomMin_Act();
			m_ImageZoom = MIN(ZoomBigger, ZoomMax);
			m_ImageZoom = MAX(m_ImageZoom, ZoomMin2);
		}	
	}
	else
	{	m_ImageZoom = 1.0;	}

	//if ( m_ImageZoom > 1.0 )
	//{	m_ImageZoom = 1.0; }	
	
	//PostMessageToMainFrameWnd(MSG_MAIN_FRAME_MESSAGE, WPARAM_SHOW_PROJECT_MAP_DOCK_PANE, FALSE);	

	ResetImageOffset();
	m_FOVPosStage.x = PosX;
	m_FOVPosStage.y = PosY;
	m_FrameResolution = ImageRes;	
	
	AOIDataCollect.ResetFovTargetParam();
	
	double Ratio = MAX(1.0, FovZoomNeedUsed);
	FillCurrentFrames(Ratio);

	TPOINT2D PosCad;
	TPOINT2D PosStage(TargetOffsetX, TargetOffsetY);	
	AOIDataCollect.MapStageOffsetPtToCad(PosStage, PosCad);	
	m_ImageOffset.x = -PosCad.x/(ImageRes.x);
	m_ImageOffset.y =  PosCad.y/(ImageRes.y);
	m_ImageOffset.x = m_ImageOffset.x/(m_ImageZoom);
	m_ImageOffset.y = m_ImageOffset.y/(m_ImageZoom);

	UpdateBarcodeSelected();
	//UpdateActiveObjList();
	BuildShowImageBuffer();
	CreateBKImage();
	RedrawWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditBarcodeView::GetKeepImageOffset() const//取得是否保持影像偏移值
{
	return m_KeepImageOffset;
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::SetKeepImageOffset(bool val)//設定是否保持影像偏移值	
{
	m_KeepImageOffset = val;
}
//-------------------------------------------------------------------------------------//
MANIPULATE_MODEL_MODE CEditBarcodeView::GetManipulateModelModeDefault()//操作模組模式	- 預設
{
	return MANIPULATE_MODEL_EDIT;
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::PostMessageToMainFrameWnd(UINT message, WPARAM wParam, LPARAM lParam)
{
	AOIDataCollect.PostMainFrameWndMessage(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::SendMessageToMainFrameWnd(UINT message, WPARAM wParam, LPARAM lParam)
{	
	AOIDataCollect.SendMainFrameWndMessage(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::OnSoftwareBarcodeAddMode() 
{
	// TODO: Add your command handler code here	
	if ( AOIDataCollect.OperateLevelEditFuncAddBarcode() == false ) {	return ; }
	AOIDataCollect.SetManipulateModelMode(MANIPULATE_MODEL_ADD);	
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::OnUpdateSoftwareBarcodeAddMode(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	MANIPULATE_MODEL_MODE ManiMode = AOIDataCollect.GetManipulateModelMode();
	if ( MANIPULATE_MODEL_ADD == ManiMode ) 
	{	pCmdUI->SetCheck(TRUE); }
	else
	{	pCmdUI->SetCheck(FALSE); }
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::OnSoftwareBarcodeEditMode() 
{
	// TODO: Add your command handler code here
	MANIPULATE_MODEL_MODE ManiMode = GetManipulateModelModeDefault();
	AOIDataCollect.SetManipulateModelMode(ManiMode);
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::OnUpdateSoftwareBarcodeEditMode(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	MANIPULATE_MODEL_MODE ManiModeDef = GetManipulateModelModeDefault();
	MANIPULATE_MODEL_MODE ManiMode = AOIDataCollect.GetManipulateModelMode();
	if ( ManiModeDef == ManiMode ) 
	{	pCmdUI->SetCheck(TRUE); }
	else
	{	pCmdUI->SetCheck(FALSE); }
}
//-------------------------------------------------------------------------------------//
bool CEditBarcodeView::UpdateBarcodeSelected()
{
	SetModel(NULL);
	m_ActiveObjList.clear();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }

	CAOIModel   *ModelPtr = NULL;
	CAOIBarcode *pBarcode = ProjectPtr->GetProjectActiveBarcode();	
	if ( NULL != pBarcode )
	{	ModelPtr = pBarcode->GetBarcodeModelPtr();	}	
	SetModel(ModelPtr);	
	UpdateBarcodeModelStats();

	MANIPULATE_MODEL_MODE ManiMode = GetManipulateModelModeDefault();
	AOIDataCollect.SetManipulateModelMode(ManiMode);	
	RedrawWnd();	
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::UpdateActiveObjList()
{
	CString     str;
	size_t      i=0;
	TRECT4D     Rect;	
	TPOINT2D    CornPoint[4];
	TPOINT2D    CornerPoint[4];	
	CAOIBox    *BoxPtr   = NULL;
	TActiveObj *ObjPtr = NULL;
	const IMAGE_SIZE ImageW = GetImageW();
	const IMAGE_SIZE ImageH = GetImageH();	
	const TPOINT2D StageCp = GetImageStageRgnCp();
	const TPOINT2D &ImageRes = GetImageResolution();
	const size_t NObjects = this->m_ActiveObjList.size();	
	const DRAW_MODEL_MODE DrawModelMode = GetDrawModelMode();

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
		AOIDataCollect.MapStageCornerToCamera(ImageW, ImageH, ImageRes, CornerPoint, StageCp, CornPoint);//機台4端點對應到影像四點
		JetAPI::PointsToRect(CornPoint, 4, Rect);
		ObjPtr->Rect       = Rect;
		ObjPtr->CornerPts[0] = CornPoint[0];
		ObjPtr->CornerPts[1] = CornPoint[1];
		ObjPtr->CornerPts[2] = CornPoint[2];
		ObjPtr->CornerPts[3] = CornPoint[3];		
	}
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::BuildActiveObjList(CAOIModel *ModelPtr, bool ActiveOnly)
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
				BoxPtr = MaskWndPtr->GetWndMaskBoxPtr();;
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
			BoxPtr->SetBoxSelected(true);
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
		BoxPtr->SetBoxSelected(false);
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
		this->AddActiveObject(ActiveObj, CheckAddObj);
	}	
	UpdateActiveObjList();
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::AddActiveObject(const TActiveObj &ActiveObj, bool Check)
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
void CEditBarcodeView::ResetActiveObjPosFocus()
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
void CEditBarcodeView::ResetActiveObjPosSelect()
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
void CEditBarcodeView::CheckActiveObjFocus(TActiveObj &Obj)
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
void CEditBarcodeView::CheckActiveObjFocus(TActiveObj &Obj, CAOIBox &Box)
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
bool CEditBarcodeView::ExecModifyActiveObjPos()//執行選中物件的座標
{
	const int nWndPx = m_MousePosCurrent.x-m_MousePosLast.x;
	const int nWndPy = m_MousePosCurrent.y-m_MousePosLast.y;
	return ExecModifyActiveObjPosKernel(nWndPx, nWndPy);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditBarcodeView::ExecModifyActiveObjPosKernel(int nWndPx, int nWndPy)//執行選中物件的座標
{
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return false; }
	DRAW_MODEL_MODE DrawModelMode = GetDrawModelMode();
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
	const TPOINT2D &ImageRes = GetImageResolution();
	const double ResX = ImageRes.x;
	const double ResY = ImageRes.y;
	const double dCadPx = dImagePx*ResX;
	const double dCadPy = -1*dImagePy*ResY;//Y軸反向
	const int    LinkMode = ModelPtr->GetModelWndLinkMode();

	BoxPtr = DYNAMIC_DOWNCAST(CAOIBox, ObjPtr->BoxPtr);
	WndPtr = DYNAMIC_DOWNCAST(CAOIWnd, ObjPtr->WndPtr);
	LandPtr = DYNAMIC_DOWNCAST(CAOILand, ObjPtr->LandPtr);
	WndRoiPtr = DYNAMIC_DOWNCAST(CAOIWndRoi, ObjPtr->WndRoiPtr);
	WndMaskPtr = DYNAMIC_DOWNCAST(CAOIWndMask, ObjPtr->WndMaskPtr);

	BoxPtr = ModelPtr->GetModelBodyBoxPtr();
	ModelPtr->ModifyModelBoxPos(BoxPtr, NULL, NULL, NULL, NULL, dCadPx, dCadPy, LinkMode);	
	UpdateActiveObjList();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditBarcodeView::ExecModifyActiveObjSize(CURSOR_POS_MODE CursorMode)//執行選中物件的尺寸
{
	CAOIModel *ModelPtr = CEditBarcodeView::GetModelPtr();
	if ( NULL == ModelPtr ) { return true; }
	DRAW_MODEL_MODE DrawModelMode = GetDrawModelMode();
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
	const TPOINT2D &ImageRes = GetImageResolution();
	const double dWndPx = m_MousePosCurrent.x-m_MousePosLast.x;
	const double dWndPy = m_MousePosCurrent.y-m_MousePosLast.y;
	const double dImagePx = dWndPx*m_ImageZoom;
	const double dImagePy = dWndPy*m_ImageZoom;
	const double ResX = ImageRes.x;
	const double ResY = ImageRes.y;	
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
	//ModelPtr->ModifyModelBoxSize(BoxPtr, WndPtr, LandPtr, WndRoiPtr, WndMaskPtr, dRgn, LinkMode);

	BoxPtr = ModelPtr->GetModelBodyBoxPtr();
	ModelPtr->ModifyModelBoxSize(BoxPtr, NULL, NULL, NULL, NULL, dRgn, LinkMode);
	ModelPtr->UpdateModelBodySizeToAllWnds();
	UpdateActiveObjList();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditBarcodeView::UpdateImageByAlgParam()//依據演算法更新畫面
{
	DRAW_MODEL_MODE DrawModelMode = GetDrawModelMode();
	DRAW_IMAGE_MODE DrawImageMode = AOIDataCollect.GetDrawImageMode();
	MANIPULATE_MODEL_MODE ManiMode = AOIDataCollect.GetManipulateModelMode();
	if ( DRAW_IAMGE_BY_ALG != DrawImageMode ) { return false; }	
	if ( MANIPULATE_MODEL_GATHER_COLOR == ManiMode ) { return false; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return false; }	
	UUID           WndUUID = ProjectPtr->GetProjectActiveModelWndUUID();
	CAOIWnd       *WndPtr = ModelPtr->GetModelWndPtrByUUID(WndUUID);
	WndPtr = ModelPtr->GetModelWndActived();
	if ( NULL == WndPtr ) { return false; }	
	CAlgBinaryParam  BinaryParam;
	CAlgParam       &AlgParam = WndPtr->GetWndAlgParam();	
	CAlgBinaryParam *BinParamPtr = AlgParam.GetAlgActiveBinParamPtr();	
	if ( NULL == BinParamPtr ) { return false; }
	unsigned int FrameIndex = BinParamPtr->GetBinaryFrameIndex();
	ProjectPtr->SetProjectMapIndex(FrameIndex);
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
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditBarcodeView::ExecCalcWndColor()//計算檢測框顏色
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
bool  CEditBarcodeView::ExecExtractWndColorFilter()//取得檢測框抽色參數
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
	SendMessageToMainFrameWnd(MSG_EDIT_IMAGE_PROCESS_WND, WPARAM_UPDATE_ALG_COLOR_FILTER, (LPARAM)(WndPtr));	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditBarcodeView::ExecGetWndColorFilter(CColorRGBV &rgbv)//取得檢測框抽色參數
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
	const TPOINT2D  StageCp = GetImageStageRgnCp();
	const TPOINT2D &ImageRes = GetImageResolution();
	if ( GetCurrentFrame(ImageW, ImageH, ImageStep, BitCount, ImagePtr, SpacePtr, MaskPtr) == false ) { return true; }
	if ( NULL == ImagePtr ) { return true; }
	if ( 24 != BitCount ) { return true; }

	bool         bIsOK = false;	
	bool         bSaved = true;	
	TREGION4D    ImageRgn;		
	TREGION4D    StageRgn;
	RECT         RoiRect={0, 0, 0, 0};	
	DRAW_MODEL_MODE DrawModelMode = GetDrawModelMode();
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
bool CEditBarcodeView::ExecGatherColorFilter(bool CombineColorMode)//吸取抽色參數
{
	const char fnName[] = "CEditBarcodeView::ExecGatherColorFilter";	
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }
	CAOIModel *ModelPtr = CEditBarcodeView::GetModelPtr();
	if ( NULL == ModelPtr ) { return true; }		

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
	CColorRGBV *rgbvPtr = BinParamPtr->GetBinaryColorActivePtr();
	if ( NULL == rgbvPtr ) { return true; }
	
	CString    str;
	MASK_PTR   MaskPtr=NULL;
	SPACE_PTR  SpacePtr=NULL;
	IMAGE_PTR  ImagePtr = NULL;
	IMAGE_SIZE ImageW=0, ImageH=0, ImageStep=0, BitCount=0;	
	if ( GetCurrentFrame(ImageW, ImageH, ImageStep, BitCount, ImagePtr, SpacePtr, MaskPtr) == false ) { return true; }
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
	
	SendMessageToMainFrameWnd(MSG_EDIT_IMAGE_PROCESS_WND, WPARAM_UPDATE_ALG_COLOR_FILTER, (LPARAM)(WndPtr));	
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::ExecAlgImage(CAOIModel *ModelPtr, CAOIWnd *WndPtr, CAlgBinaryParam &BinaryParam)
{
	const unsigned int FrameUniqueID = BinaryParam.GetBinaryFrameUniqueID();
	if ( FRAME_UNIQUE_ID_DLP == FrameUniqueID )//高度值需要Leveling所以不能用Field資料
	{	ExecAlgImage_Model(ModelPtr, WndPtr, BinaryParam);	}
	else
	{	ExecAlgImage_Field(ModelPtr, WndPtr, BinaryParam); }
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::ExecAlgImage_Field(CAOIModel *ModelPtr, CAOIWnd *WndPtr, CAlgBinaryParam &BinaryParam)
{
	if ( NULL == ModelPtr ) { return; }
	if ( NULL == WndPtr ) { return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }

	const int nAlign = 4;
	const bool bClone = true;
	std::vector<TUNI_FRAME> FieldUniFrameList;
	CAlgParam  &AlgParam = WndPtr->GetWndAlgParam();
	if ( AOIDataCollect.CopyFieldUniFrameList(FieldUniFrameList, bClone) == false )
	{	return ; }

	size_t       i=0;
	const size_t FieldUniFrameCount = FieldUniFrameList.size();
	if ( 0 == FieldUniFrameCount ) { return; }
	if ( true == bClone )
	{
		double nX=0, nY=0, nZ=0, OffsetZ=0, OverHigh=0, OverLow=0;
		nX = ModelPtr->GetModelSpaceLeveingParamX();
		nY = ModelPtr->GetModelSpaceLeveingParamY();
		nZ = ModelPtr->GetModelSpaceLeveingParamZ();
		OffsetZ = ModelPtr->GetModelSpaceLeveingOffsetZ();
		OverHigh = ModelPtr->GetModelSpaceLeveingOverHigh();
		OverLow = ModelPtr->GetModelSpaceLeveingOverLow();		
		if ( AOIDataCollect.ModifyFieldSpaceImage(FieldUniFrameList, 0, 0, nZ, OffsetZ, OverHigh, OverLow) == false )
		{
			if ( true == bClone )
			{	JetAPI::ClearUniFrameList(FieldUniFrameList); }
			FieldUniFrameList.clear();
			return;
		}
	}
	
	RECT       WndRect={0,0,0,0};		
	RECT       ModelRect={0,0,0,0};
	RECT       WndExtRect={0,0,0,0};	
	RECT       ModelMaskRect={0,0,0,0};
	TREGION4D  WndImageRgn;
	TREGION4D  ModelImageRgn;
	TREGION4D  WndExtImageRgn;
	TREGION4D  WndRegionStage;
	TREGION4D  WndExtRegionStage;
	TREGION4D  ModelRegionStage;
	TREGION4D  FieldRegionStage;	
	TPOINT2D   ImageRes, FieldCpStage;	
	const double AttachedAngle = ModelPtr->GetModelAttachedAngle();
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(AttachedAngle);
	TUNI_FRAME FieldUniFrame = FieldUniFrameList[0];	
	
	WndPtr->GetWndRegionStage(WndRegionStage);
	WndPtr->GetWndExtendRegionStage(WndExtRegionStage);	
	ModelPtr->GetModelTotalRegionStage(ModelRegionStage);
	FieldRegionStage = AOIDataCollect.GetFieldUniFrameStageRegion();

	IMAGE_SIZE FieldImageW = FieldUniFrame.ImageW;
	IMAGE_SIZE FieldImageH = FieldUniFrame.ImageH;
	const double FieldRegionW = FieldRegionStage.GetWidth();
	const double FieldRegionH = FieldRegionStage.GetHeight();
	
	FieldCpStage.x = FieldRegionStage.GetCpX();
	FieldCpStage.y = FieldRegionStage.GetCpY();
	ImageRes.x = FieldRegionW;
	ImageRes.y = FieldRegionH;
	ImageRes.x = ImageRes.x/FieldImageW;
	ImageRes.y = ImageRes.y/FieldImageH;

	if ( AOIDataCollect.MapStageRegionToCamera(FieldImageW, FieldImageH, ImageRes, WndRegionStage, FieldCpStage, WndImageRgn) == false || 
		 AOIDataCollect.MapStageRegionToCamera(FieldImageW, FieldImageH, ImageRes, WndExtRegionStage, FieldCpStage, WndExtImageRgn) == false || 
		 AOIDataCollect.MapStageRegionToCamera(FieldImageW, FieldImageH, ImageRes, ModelRegionStage, FieldCpStage, ModelImageRgn) == false )
	{
		if ( true == bClone )
		{	JetAPI::ClearUniFrameList(FieldUniFrameList); }
		FieldUniFrameList.clear();
		return; 
	}
	JetAPI::Region4DToRect(WndImageRgn, WndRect, true);
	JetAPI::Region4DToRect(ModelImageRgn, ModelRect, true);
	JetAPI::Region4DToRect(WndExtImageRgn, WndExtRect, true);
	if ( WndRect.left<0 || WndRect.top <0 || WndRect.right>FieldImageW || WndRect.bottom>FieldImageH ) 
	{
		if ( true == bClone )
		{	JetAPI::ClearUniFrameList(FieldUniFrameList); }
		FieldUniFrameList.clear();
		return; 
	}
	RECT BoundaryRect={0,0,0,0};	
	JetAPI::SizeToRect(FieldImageW, FieldImageH, BoundaryRect);
	JetAPI::BoundaryRect(BoundaryRect, ModelRect);
	if ( BINARY_DISABLE == BinaryParam.GetBinaryMode() )
	{	ModelMaskRect = ModelRect; }
	else
	{	AOIDataCollect.AdjustModelBinaryMaskRect(ModelRect, WndExtRect, ModelMaskRect);	 }

	const bool bTestWnd = true;
	bool       bHeightImage = false;	
	MASK_PTR   ModelMaskPtr = NULL;
	IMAGE_PTR  ModelGrayPtr = NULL;
	IMAGE_SIZE ModelMaskW=0, ModelMaskH=0, ModelMaskStep=0, ModelMaskBitCount=0;	
	const unsigned int FrameUniqueID = BinaryParam.GetBinaryFrameUniqueID();
	if ( FRAME_UNIQUE_ID_DLP == FrameUniqueID )
	{	bHeightImage = true;	}
	else
	{	bHeightImage = false; }
	if ( AlgParam.ExecAlgUniFrameBinary(BinaryParam, WndRect, ModelMaskRect, FieldUniFrameList, ModelMaskW, ModelMaskH, ModelMaskStep, ModelMaskBitCount, ModelMaskPtr, ModelGrayPtr, bTestWnd) == false )
	{
		if ( true == bClone )
		{	JetAPI::ClearUniFrameList(FieldUniFrameList); }
		FieldUniFrameList.clear();		
		return;
	}	
	if ( true == bClone )
	{	JetAPI::ClearUniFrameList(FieldUniFrameList); }
	FieldUniFrameList.clear();	

	const char fnName[] = "CEditBarcodeView::ExecAlgImage_Model";	
	TREGION4D  ImageRgn;
	MASK_PTR   MaskPtr=NULL;
	SPACE_PTR  SpacePtr=NULL;
	IMAGE_PTR  ImagePtr = NULL;
	IMAGE_SIZE ImageW=0, ImageH=0, ImageStep=0, BitCount=0;	
	unsigned int FrameIndex= BinaryParam.GetBinaryFrameIndex();	
	ProjectPtr->SetProjectMapIndex(FrameIndex);
	FrameIndex = ProjectPtr->GetProjectMapIndex();
	if ( GetFrameImage(FrameIndex, ImageW, ImageH, ImageStep, BitCount, ImagePtr, SpacePtr, MaskPtr) == false ) 
	{	
		JetMemory.free_func(ModelMaskPtr);
		JetMemory.free_func(ModelGrayPtr);
		return; 
	}	
	
	IMAGE_PTR ShowImagePtr = NULL;	
	const IMAGE_SIZE ShowBitCount = 24;	
	const IMAGE_SIZE ShowStep = JetAPI::GetBMPImagePixelsPerLine(ImageW, ShowBitCount, 4);
	const TPOINT2D  FovCp = GetImageStageRgnCp();
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	const size_t ShowBufferSize = ImageAPI.CalcBufferSize(ShowStep, ImageH);

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
	const bool theSameSize=true;
	if ( BINARY_DISABLE != BinaryParam.GetBinaryMode() ) 
	{
		MASK_DATA mask = 0xFF;
		IMAGE_DATA mskR=0xFF, mskG=0xFF, mskB=0x00, mskV=0xFF, Alpha=0;		
		AOIDataCollect.GetMaskImageColor(FrameUniqueID, mskR, mskG, mskB, mskV, Alpha);
		AOIDataCollect.ExecEnhanceDisplayImage(ImageW, ImageH, ShowStep, ShowBitCount, ShowImagePtr, ShowImagePtr);
		if ( 24 == ShowBitCount )//Color Image
		{	bIsOK = ImageAPI.ColorImageApplyMask(ImageW, ImageH, ShowStep, ShowImagePtr, ModelMaskRect, ModelMaskStep, ModelMaskPtr, mask, mskR, mskG, mskB, Alpha);	}
		else
		{	bIsOK = ImageAPI.GrayImageApplyMask(ImageW, ImageH, ShowStep, ShowImagePtr, ModelMaskRect, ModelMaskStep, ModelMaskPtr, mask, mskV, Alpha);	}
		if ( false == bIsOK )
		{	
			JetMemory.free_func(ModelMaskPtr);
			JetMemory.free_func(ShowImagePtr);
			JetMemory.free_func(ModelGrayPtr);
			return;
		}		
	}
	else if ( IMAGE_SRC_COLOR != BinaryParam.GetBinaryImageSourceMode() )
	{
		//將局部的灰階影像貼上FOV的灰階指標內		
		if ( false == bHeightImage )
		{
			if ( 24 == ShowBitCount )//Color Image
			{	bIsOK = ImageAPI.PasteColorRoiImage3(ImageW, ImageH, ShowStep, ShowImagePtr, ModelMaskRect, ModelMaskStep, ModelGrayPtr, ModelGrayPtr, ModelGrayPtr, false, theSameSize);		}
			else
			{	bIsOK = ImageAPI.PasteGrayRoiImage3(ImageW, ImageH, ShowStep, ShowImagePtr, ModelMaskRect, ModelMaskStep, ModelGrayPtr, false, theSameSize);	}			
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
	{
		const double Offset=0.0;
		const double GainValue = BinaryParam.GetGrayGainValue();
		const bool bGrayGainEnabled = BinaryParam.CheckGrayGainEnabed();
		if ( true == bGrayGainEnabled )
		{	ImageAPI.ImageOffsetGain3(ImageW, ImageH, ShowStep, ShowBitCount, ShowImagePtr, ModelMaskRect, ShowImagePtr, Offset, GainValue);	}
		AOIDataCollect.ExecEnhanceDisplayImage(ImageW, ImageH, ShowStep, ShowBitCount, ShowImagePtr, ShowImagePtr);	
	}	
	JetMemory.free_func(ModelMaskPtr);	
	JetMemory.free_func(ModelGrayPtr);

	ReleaseShowImageBuffer();

	m_ShowImageW = ImageW;
	m_ShowImageH = ImageH;
	m_ShowImageStep = ShowStep;
	m_ShowBitCount = ShowBitCount;
	m_ShowImagePtr = ShowImagePtr;
	
	CreateBKImage();
	RedrawWnd();	
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::ExecAlgImage_Model(CAOIModel *ModelPtr, CAOIWnd *WndPtr, CAlgBinaryParam &BinaryParam)
{	
	if ( NULL == ModelPtr ) { return; }
	if ( NULL == WndPtr ) { return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }

	const int  nAlign = 4;
	const bool bNoFilter=false;
	CAlgParam  &AlgParam = WndPtr->GetWndAlgParam();	
	std::vector<TUNI_FRAME> ModelUniFrameList;
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

	unsigned int FrameUniqueID = BinaryParam.GetBinaryFrameUniqueID();
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

	const char fnName[] = "CEditBarcodeView::ExecAlgImage_Model";	
	TPOINT2D   FovCp;
	TREGION4D  ImageRgn;
	MASK_PTR   MaskPtr=NULL;
	SPACE_PTR  SpacePtr=NULL;
	IMAGE_PTR  ImagePtr = NULL;
	IMAGE_SIZE ImageW=0, ImageH=0, ImageStep=0, BitCount=0;	
	unsigned int FrameIndex= BinaryParam.GetBinaryFrameIndex();	
	ProjectPtr->SetProjectMapIndex(FrameIndex);
	FrameIndex = ProjectPtr->GetProjectMapIndex();
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
	const TPOINT2D StageCp = GetImageStageRgnCp();	
	const TPOINT2D &ImageRes = GetImageResolution();
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	const size_t ShowBufferSize = ImageAPI.CalcBufferSize(ShowStep, ImageH);

	FovCp.x = StageCp.x;
	FovCp.y = StageCp.y;	
	//模組範圍
	ModelPtr->GetModelTotalRegionStage(ModelRegion);
	if ( AOIDataCollect.MapStageRegionToCamera(ImageW, ImageH, ImageRes, ModelRegion, FovCp, ImageRgn) == false )
	{	
		JetMemory.free_func(ModelMaskPtr);
		JetMemory.free_func(ModelGrayPtr);
		return; 
	}
	ModelRectInFov.left   = JetAPI::Floor(ImageRgn.minX);
	ModelRectInFov.right  = ModelRectInFov.left+ModelMaskW;
	ModelRectInFov.top    = JetAPI::Floor(ImageRgn.minY);
	ModelRectInFov.bottom = ModelRectInFov.top+ModelMaskH;
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
	const bool theSameSize=false;
	if ( BINARY_DISABLE != BinaryParam.GetBinaryMode() ) 
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
	else if ( IMAGE_SRC_COLOR != BinaryParam.GetBinaryImageSourceMode() )
	{
		//將局部的灰階影像貼上FOV的灰階指標內		
		if ( false == bHeightImage )
		{	
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
	{
		const double Offset=0.0;
		const double GainValue = BinaryParam.GetGrayGainValue();
		const bool bGrayGainEnabled = BinaryParam.CheckGrayGainEnabed();
		if ( true == bGrayGainEnabled )
		{	ImageAPI.ImageOffsetGain3(ImageW, ImageH, ShowStep, ShowBitCount, ShowImagePtr, ModelRectInFov, ShowImagePtr, Offset, GainValue);	}
		AOIDataCollect.ExecEnhanceDisplayImage(ImageW, ImageH, ShowStep, ShowBitCount, ShowImagePtr, ShowImagePtr);	
	}	
	JetMemory.free_func(ModelMaskPtr);	
	JetMemory.free_func(ModelGrayPtr);

	ReleaseShowImageBuffer();

	m_ShowImageW = ImageW;
	m_ShowImageH = ImageH;
	m_ShowImageStep = ShowStep;
	m_ShowBitCount = ShowBitCount;
	m_ShowImagePtr = ShowImagePtr;
	
	CreateBKImage();
	RedrawWnd();	
	return ;
}
//-------------------------------------------------------------------------------------//
bool CEditBarcodeView::ExecModelWndInspection(bool UpdateUI)
{	
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }
	CAOIModel *ModelPtr = CEditBarcodeView::GetModelPtr();
	if ( NULL == ModelPtr ) { return false; }	
	DRAW_MODEL_MODE DrawModelMode = GetDrawModelMode();
	if ( DRAW_MODEL_EDIT != DrawModelMode ) { return true; }	
	const int  nAlign = 4;
	const bool bNoFilter=false;
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
		{	PostMessageToMainFrameWnd(MSG_EDIT_WND_PROPERTY_WND, WPARAM_UPDATE_WND_SELECTED, NULL);	 }
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
	{	PostMessageToMainFrameWnd(MSG_EDIT_WND_PROPERTY_WND, WPARAM_BUILD_WND_SELECTED, NULL);	 }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditBarcodeView::ExecModelBarcodeInspect()
{
	CAOIModel *ModelPtr = CEditBarcodeView::GetModelPtr();
	if ( NULL == ModelPtr ) { return false; }
	const int  nAlign = 4;
	const bool bNoFilter=false;
	std::vector<TUNI_FRAME> UniFrameList;	
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
bool CEditBarcodeView::BuildModelUniFrameList(CAOIModel *ModelPtr, std::vector<TUNI_FRAME> &UniFrameList, int nAlign, bool bClone, bool bNoFilter)
{
	if ( GetShowProjectMapMode() == true ) { return false; }
	CAOIWnd   *WndPtr =NULL;
	TREGION4D StageRgn;	
	ModelPtr->CalcModelTotalRegionAll();	
	ModelPtr->GetModelTotalRegionStage(StageRgn);
	WndPtr = ModelPtr->GetModelWndActived();
	if ( AOIDataCollect.CheckModelUniFrameListModelPtr(ModelPtr) == true )
	{	
		if ( AOIDataCollect.CopyModelUniFrameList(UniFrameList, bClone) == false )
		{	return false; }
		PostMessageToMainFrameWnd(MSG_EDIT_IMAGE_VIEW_WND, WPARAM_UPDATE_IMAGE_WND_SELECTED_NO_PROCESS, NULL);		
	}
	else
	{
		const bool bNoFilter2 = true;
		if ( AOIDataCollect.CreateModelUniFrameList(m_UniFrameList, m_FOVPosStage, m_FrameResolution, ModelPtr, UniFrameList, nAlign, bNoFilter2) == false )		
		{	AOIDataCollect.ReleaseModelUniFrameList(); }		
		else
		{	AOIDataCollect.SetModelUniFrameList(ModelPtr, StageRgn, UniFrameList, bClone);	}		
		if ( false == bNoFilter )
		{
			PostMessageToMainFrameWnd(MSG_EDIT_IMAGE_VIEW_WND, WPARAM_UPDATE_IMAGE_MODEL_SELECTED, NULL);
			//PostMessageToMainFrameWnd(MSG_EDIT_IMAGE_VIEW_WND, WPARAM_UPDATE_IMAGE_MODEL_NO_PROCESS, NULL);		
		}
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditBarcodeView::BuildModelWndUniFrameList(CAOIModel *ModelPtr,CAOIWnd *WndPtr, std::vector<TUNI_FRAME> &UniFrameList, int nAlign)
{
	if ( NULL == ModelPtr ) { return false; }
	if ( NULL == WndPtr ) { return false; }	

	CString                 str;
	bool                    Exception=false;
	size_t                  i=0, j=0;
	size_t                  UniFrameCount=0;
	IMAGE_SIZE              ImageW=0;
	IMAGE_SIZE              ImageH=0;
	IMAGE_SIZE              ImageStep=0;
	IMAGE_SIZE              BitCount=0;
	IMAGE_PTR               ImagePtr=0;
	MASK_PTR                MaskPtr=0;
	SPACE_PTR               SpacePtr=0;

	TPOINT2D                FovCp;
	TREGION4D               CadRgn;
	TREGION4D               ImageRgn;
	TREGION4D               StageRgn;
	RECT                    RoiRect={0};
	IMAGE_SIZE              RoiW=0;
	IMAGE_SIZE              RoiH=0;
	IMAGE_SIZE              RoiStep=0;
	IMAGE_SIZE              RoiBitCount=0;
	IMAGE_PTR               RoiImagePtr=0;
	MASK_PTR                RoiMaskPtr=0;
	SPACE_PTR               RoiSpacePtr=0;
	TUNI_FRAME             *UniFramePtr=NULL;
	TUNI_FRAME              UniFrame;	
	BOOL                    bSave = FALSE;	
	
	FovCp.x = m_FOVPosStage.x;
	FovCp.y = m_FOVPosStage.y;
	Exception=false;
	WndPtr->GetWndRegionStage(StageRgn);
	for ( i=0; i<FRAME_MAX_COUNT; i++ )
	{
		UniFramePtr = &(m_UniFrameList[i]);
		if ( NULL == UniFramePtr ) { continue; }
		if ( NULL == UniFramePtr->ImagePtr ) { continue; }

		ImageW = UniFramePtr->ImageW;
		ImageH = UniFramePtr->ImageH;
		ImageStep = UniFramePtr->ImageStep;
		BitCount = UniFramePtr->BitCount;
		ImagePtr = UniFramePtr->ImagePtr;
		MaskPtr = UniFramePtr->MaskPtr;
		SpacePtr = UniFramePtr->SpacePtr;
		if ( AOIDataCollect.MapStageRegionToCamera(ImageW, ImageH, m_FrameResolution, StageRgn, FovCp, ImageRgn) == false )
		{
			Exception = true;
			break;
		}
		JetAPI::Region4DToRect(ImageRgn, RoiRect, false);
		JetAPI::AdjustRectByAlignW(RoiRect, nAlign);		
		if ( RoiRect.left<0 || RoiRect.right<0 || RoiRect.top<0 || RoiRect.bottom<0 || 
			 RoiRect.left>=ImageW || RoiRect.right>=ImageW || RoiRect.top>=ImageH || RoiRect.bottom>=ImageH ) 
		{
			Exception = true;
			break;
		}
		RoiW = RoiRect.right-RoiRect.left;
		RoiH = RoiRect.bottom-RoiRect.top;
		RoiBitCount = m_UniFrameList[i].BitCount;
		RoiStep = JetAPI::GetBMPImagePixelsPerLine(RoiW, RoiBitCount, 4);
		if ( NULL != ImagePtr )
		{	
			if ( ImageAPI.ExtractRoiImage(ImageW, ImageH, ImageStep, BitCount, ImagePtr, RoiRect, RoiStep, RoiImagePtr, false)==false )
			{
				Exception = true;
				break;
			}
		}
		if ( NULL != MaskPtr )
		{	
			if ( ImageAPI.ExtractRoiImage(ImageW, ImageH, ImageStep, BitCount, MaskPtr, RoiRect, RoiStep, RoiMaskPtr, false)==false )
			{
				Exception = true;
				break;
			}
		}
		if ( NULL != SpacePtr )
		{	
			if ( ImageAPI.ExtractSpaceGrayRoiImage(ImageW, ImageH, ImageStep, SpacePtr, RoiRect, RoiStep, RoiSpacePtr, false)==false )
			{
				Exception = true;
				break;
			}
		}

		UniFrame.ImageW = RoiW;
		UniFrame.ImageH = RoiH;
		UniFrame.ImageStep = RoiStep;
		UniFrame.BitCount = RoiBitCount;
		UniFrame.ImagePtr = RoiImagePtr;
		UniFrame.MaskPtr = RoiMaskPtr;
		UniFrame.SpacePtr = RoiSpacePtr;
		UniFrameList.push_back(UniFrame);
		UniFrame = TUNI_FRAME();
		RoiImagePtr = NULL;
		RoiMaskPtr = NULL;
		RoiSpacePtr = NULL;
	}
	if ( true == Exception )
	{
		UniFrameCount = UniFrameList.size();
		for ( j=0; j<UniFrameCount; j++ )
		{
			UniFrame = UniFrameList[j];
			JetMemory.free_func(UniFrame.ImagePtr);
			JetMemory.free_func(UniFrame.MaskPtr);
			JetMemory.free_func(UniFrame.SpacePtr);
		}
		UniFrameList.clear();
		return false;
	}

#ifdef _DEBUG
	bSave = TRUE;
	if ( bSave == TRUE )
	{
		UniFrameCount = UniFrameList.size();
		for ( j=0; j<UniFrameCount; j++ )
		{
			UniFrame = UniFrameList[j];

			RoiW = UniFrame.ImageW;
			RoiH = UniFrame.ImageH;
			RoiStep = UniFrame.ImageStep;
			RoiBitCount = UniFrame.BitCount;
			RoiImagePtr = UniFrame.ImagePtr;
			RoiMaskPtr = UniFrame.MaskPtr;
			RoiSpacePtr = UniFrame.SpacePtr;

			if ( NULL != RoiImagePtr )
			{
				str.Format(_T("%s\\WndImage%d.BMP"), AOIDataCollect.GetAOITempDirectory(), j+1);
				ImageAPI.SaveBMPImage(str, RoiW, RoiH, RoiStep, RoiBitCount, RoiImagePtr, true);
			}
			if ( NULL != RoiMaskPtr )
			{
				str.Format(_T("%s\\WndMask%d.BMP"), AOIDataCollect.GetAOITempDirectory(), j+1);
				ImageAPI.SaveBMPImage(str, RoiW, RoiH, RoiStep, RoiBitCount, RoiMaskPtr, true);
			}
		}		
	}
#endif//_DEBUG
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::OnSoftwareBarcodeDelSelected() 
{
	// TODO: Add your command handler code here
	ExecDeleteBarcodeSelected();
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::OnUpdateSoftwareBarcodeDelSelected(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr )
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE); }
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::OnSoftwareBarcodeDelSelectedWnd()
{
	ExecDeleteBarcodeSelectedWnd();	
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::OnUpdateSoftwareBarcodeDelSelectedWnd(CCmdUI* pCmdUI)
{
	size_t WndCount=0;
	CAOIModel *ModelPtr=NULL;
	CAOIBarcode *BarcodePtr = GetModelBarcodePtr();
	if ( NULL != BarcodePtr )
	{
		ModelPtr = BarcodePtr->GetBarcodeModelPtr();
		if ( NULL != ModelPtr )
		{	WndCount = ModelPtr->GetModelWndCount(); }
	}
	if ( WndCount < 2 )
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::OnSoftwareBarcodeDelOthers() 
{
	// TODO: Add your command handler code here
	ExecDeleteBarcodeOthers();
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::OnUpdateSoftwareBarcodeDelOthers(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr )
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE); }
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::OnSoftwareBarcodeDelOtherWnds()
{
	// TODO: Add your command handler code here
	ExecDeleteBarcodeOtherWnds();
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::OnUpdateSoftwareBarcodeDelOtherWnds(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CAOIBarcode *BarcodePtr = GetModelBarcodePtr();
	if ( NULL == BarcodePtr )
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE); }
}
//-------------------------------------------------------------------------------------//
bool CEditBarcodeView::ExecDeleteBarcodeAll()
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }
	if ( AOIDataCollect.OperateLevelEditFuncDelBarcode() == false ) { return false; }

	DWORD   Res=0;
	CString str;
	str = _T("Do you wnat to clear all software barcodes?");
	str = LoadMultiLanguageString(str, str);
	Res = JetAPI::ShowMessageBox(str, MB_YESNO);
	if ( IDNO == Res ) { return true; }

	ResetBarcodeModel();
	AOIDataCollect.ReleaseModelUniFrameList();
	ProjectPtr->SelectProjectAllBarcodes(true);
	LogOperCtrl.SaveLogProjectBarcodeSelectedDelete(ProjectPtr);
	ProjectPtr->DeleteProjectBarcodeSelected();
	ProjectPtr->ResetProjectActiveIndex();
	PostMessageToMainFrameWnd(MSG_EDIT_WND_PROPERTY_WND, WPARAM_BUILD_WND_SELECTED, NULL);	
	PostMessageToMainFrameWnd(MSG_EDIT_PART_LIST_WND, WPARAM_BUILD_PART_LIST, LPARAM_BUILD_DOCK_LIST_PART);	
	RedrawWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditBarcodeView::ExecDeleteBarcodeOthers()
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }
	if ( AOIDataCollect.OperateLevelEditFuncDelBarcode() == false ) { return false; }

	ResetBarcodeModel();
	ProjectPtr->DeleteProjectBarcodeUnselected();
	ProjectPtr->ResetProjectActiveIndex();	
	PostMessageToMainFrameWnd(MSG_EDIT_PART_LIST_WND, WPARAM_BUILD_PART_LIST, LPARAM_BUILD_DOCK_LIST_PART);	
	RedrawWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditBarcodeView::ExecDeleteBarcodeOtherWnds()
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return true; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }
	CAOIBarcode *BarcodePtr = GetModelBarcodePtr();
	if ( NULL == BarcodePtr ) { return true; }
	CAOIModel *ModelPtr = BarcodePtr->GetBarcodeModelPtr();
	if ( NULL == ModelPtr ) { return true; }
	CAOIWnd *WndPtr = ModelPtr->GetModelWndActived();
	if ( NULL == WndPtr ) { return true; }
	const size_t WndCount = ModelPtr->GetModelWndCount();
	if ( 1 == WndCount ) { return true; }
	if ( AOIDataCollect.OperateLevelEditFuncDelBarcode() == false ) { return false; }

	CString str, str1;		
	CString strBarcode=BarcodePtr->GetBarcodeFullName();
	str = _T("Do you want to delete the other wnds");
	str = LoadMultiLanguageString(str, str);	
	str1.Format(_T("%s\n%s"), strBarcode, str); 
	if ( JetAPI::ShowMessageBox(str1, MB_YESNO) != IDYES )
	{	return true; }

	const bool   bLinkMode = true;
	ModelPtr->SetModelWndSelected(true);
	WndPtr->SetWndSelected(false);
	LogOperCtrl.SaveLogModelWndSelectedDelete(ModelPtr);
	ModelPtr->DeleteModelWndSelected(bLinkMode);
	WndPtr = ModelPtr->GetModelWndPtr(0, true);
	if ( NULL != WndPtr )
	{
		WndPtr->SetWndSelected(true);
		ModelPtr->SetModelWndActived(WndPtr);
	}
	UpdateBarcodeModelStats();
	RedrawWnd();

	ProjectPtr->SetProjectActiveBarcodePtr(BarcodePtr);
	SendMessageToMainFrameWnd(MSG_EDIT_PART_LIST_WND, WPARAM_BUILD_PART_LIST, LPARAM_BUILD_DOCK_LIST_PART);
	SendMessageToMainFrameWnd(MSG_EDIT_PART_LIST_WND, WPARAM_UPDATE_PART_SELECTED, NULL);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditBarcodeView::ExecDeleteBarcodeSelected()
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }
	if ( AOIDataCollect.OperateLevelEditFuncDelBarcode() == false ) { return false; }

	ResetBarcodeModel();
	AOIDataCollect.ReleaseModelUniFrameList();
	LogOperCtrl.SaveLogProjectBarcodeSelectedDelete(ProjectPtr);	
	ProjectPtr->DeleteProjectBarcodeSelected();
	ProjectPtr->ResetProjectActiveIndex();
	PostMessageToMainFrameWnd(MSG_EDIT_WND_PROPERTY_WND, WPARAM_BUILD_WND_SELECTED, NULL);	
	PostMessageToMainFrameWnd(MSG_EDIT_PART_LIST_WND, WPARAM_BUILD_PART_LIST, LPARAM_BUILD_DOCK_LIST_PART);	
	RedrawWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditBarcodeView::ExecDeleteBarcodeSelectedWnd()
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return true; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }
	CAOIBarcode *BarcodePtr = GetModelBarcodePtr();
	if ( NULL == BarcodePtr ) { return true; }
	CAOIModel *ModelPtr = BarcodePtr->GetBarcodeModelPtr();
	if ( NULL == ModelPtr ) { return true; }
	CAOIWnd *WndPtr = ModelPtr->GetModelWndActived();
	if ( NULL == WndPtr ) { return true; }
	const size_t WndCount = ModelPtr->GetModelWndCount();
	if ( WndCount < 2 ) { return true; }
	if ( AOIDataCollect.OperateLevelEditFuncDelBarcode() == false ) { return false; }

	CString str, str1;	
	CString strIndex=AOIDataDefine.GetIndexText();
	CString strBarcode=BarcodePtr->GetBarcodeFullName();
	str = _T("Do you want to delete the wnd");
	str = LoadMultiLanguageString(str, str);
	str1.Format(_T("%s[%s:%d]?"), str, strIndex, WndPtr->GetWndIndex()+1);
	str.Format(_T("%s\n%s"), strBarcode, str1); 
	str1 = str;
	
	if ( JetAPI::ShowMessageBox(str1, MB_YESNO) != IDYES )
	{	return true; }

	const bool   bLinkMode = true;
	LogOperCtrl.SaveLogModelWndSelectedDelete(ModelPtr);
	ModelPtr->DeleteModelWndSelected(bLinkMode);
	WndPtr = ModelPtr->GetModelWndPtr(0, true);
	if ( NULL != WndPtr )
	{
		WndPtr->SetWndSelected(true);
		ModelPtr->SetModelWndActived(WndPtr);
	}
	UpdateBarcodeModelStats();
	RedrawWnd();

	ProjectPtr->SetProjectActiveBarcodePtr(BarcodePtr);
	SendMessageToMainFrameWnd(MSG_EDIT_PART_LIST_WND, WPARAM_BUILD_PART_LIST, LPARAM_BUILD_DOCK_LIST_PART);
	SendMessageToMainFrameWnd(MSG_EDIT_PART_LIST_WND, WPARAM_UPDATE_PART_SELECTED, NULL);	
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::OnSoftwareBarcodePasteToOtherBoards() 
{
	// TODO: Add your command handler code here
	ExecPasteBarcodeToOtherBoards();
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::OnUpdateSoftwareBarcodePasteToOtherBoards(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr )
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE); }
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::OnSoftwareBarcodePasteToOtherPanels()
{
	// TODO: Add your command handler code here
	ExecPasteBarcodeToOtherPanels();
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::OnUpdateSoftwareBarcodePasteToOtherPanels(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr )
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE); }
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::OnSoftwareBarcodeAddBarcodeWnd()
{
	// TODO: Add your command handler code here
	ExecAddBarcodeWnd();
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::OnUpdateSoftwareBarcodeAddBarcodeWnd(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CAOIBarcode *BarcodePtr = GetModelBarcodePtr();
	if ( NULL == BarcodePtr )
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE); }
}
//-------------------------------------------------------------------------------------//
bool CEditBarcodeView::ExecPasteBarcodeToOtherBoards()
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }
	CAOIBarcode *BarcodePtr = ProjectPtr->GetProjectActiveBarcode();
	if ( NULL == BarcodePtr ) { return false; }
	if ( AOIDataCollect.OperateLevelEditFuncAddBarcode() == false ) {	return false; }

	std::vector<CAOIBarcode*> BarcodeList;
	ProjectPtr->GetProjectBarcodeSelected(BarcodeList);	
	ProjectPtr->PasteProjectBarcodeToOtherBoards(BarcodeList);
	SendMessageToMainFrameWnd(MSG_EDIT_PART_LIST_WND, WPARAM_BUILD_PART_LIST, LPARAM_BUILD_DOCK_LIST_PART);
	SendMessageToMainFrameWnd(MSG_EDIT_PART_LIST_WND, WPARAM_UPDATE_PART_SELECTED, NULL);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditBarcodeView::ExecPasteBarcodeToOtherPanels()
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }
	CAOIBarcode *BarcodePtr = ProjectPtr->GetProjectActiveBarcode();
	if ( NULL == BarcodePtr ) { return false; }
	if ( AOIDataCollect.OperateLevelEditFuncAddBarcode() == false ) {	return false; }

	std::vector<CAOIBarcode*> BarcodeList;
	ProjectPtr->GetProjectBarcodeSelected(BarcodeList);	
	ProjectPtr->PasteProjectBarcodeToOtherPanels(BarcodeList);
	SendMessageToMainFrameWnd(MSG_EDIT_PART_LIST_WND, WPARAM_BUILD_PART_LIST, LPARAM_BUILD_DOCK_LIST_PART);
	SendMessageToMainFrameWnd(MSG_EDIT_PART_LIST_WND, WPARAM_UPDATE_PART_SELECTED, NULL);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditBarcodeView::ExecAddBarcodeWnd()
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return true; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }
	CAOIBarcode *BarcodePtr = GetModelBarcodePtr();
	if ( NULL == BarcodePtr ) { return false; }
	CAOIModel *ModelPtr = BarcodePtr->GetBarcodeModelPtr();
	if ( NULL == ModelPtr ) { return false; }
	if ( AOIDataCollect.OperateLevelEditFuncAddBarcode() == false ) {	return false; }

	CString str;
	str = _T("Do you want to add new barcode wnd?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO )
	{	return true; }

	CAOIWnd *WndPtr = ModelPtr->GetModelWndActived();
	const int WndGroupID=ModelPtr->GetModelWndFreeGroupID();
	if ( NULL == WndPtr )
	{	WndPtr = ModelPtr->GetModelWndPtr(0, true);	}
	if ( NULL == WndPtr )
	{	return false; }
	WndPtr->SetWndLogicType(WND_LOGIC_DEFECT_ID);

	WndPtr = WndPtr->CloneWndObj();		
	if ( NULL == WndPtr )
	{	return false; }
	
	ModelPtr->UnSelectModel();	
	ModelPtr->AddModelWndPtr(WndPtr, false);
	ModelPtr->CalcModelTotalRegionAll();
	ModelPtr->UpdateModelRegionToAttached();

	WndPtr->SetWndSelected(true);
	WndPtr->SetWndGroupID(WndGroupID);
	ModelPtr->SetModelWndActived(WndPtr);		
	WndPtr = ModelPtr->GetModelWndActived();		
	UpdateBarcodeModelStats();
	RedrawWnd();

	ProjectPtr->SetProjectActiveBarcodePtr(BarcodePtr);	
	LogOperCtrl.SaveLogModelWndSelectedCreate(ModelPtr);

	SendMessageToMainFrameWnd(MSG_EDIT_PART_LIST_WND, WPARAM_BUILD_PART_LIST, LPARAM_BUILD_DOCK_LIST_PART);
	SendMessageToMainFrameWnd(MSG_EDIT_PART_LIST_WND, WPARAM_UPDATE_PART_SELECTED, NULL);	
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::OnSoftwareBarcodeInspectAll() 
{
	// TODO: Add your command handler code here
	CString str;	
	BOOL bSaveField = FALSE;//CWnd::IsDlgButtonChecked(DEBUG_SAVE_FIELD_CHK);
	//this->m_ProjectMapWnd.SetDrawProjectMode(IMAGE_WND_DRAW_PROJECT_INSPECTING);	
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();
	bool IsNeedGrabFiducial = AOIDataCollect.GetIsNeedGrabFiducial();

	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	
	ProjectPtr->SelectProjectAllBarcodes(true);
	ProjectPtr->SelectProjectAllComponents(false);
	ProjectPtr->SelectProjectFdForTuning(OfflineMode, IsNeedGrabFiducial);
	ProjectPtr->ResetProjectOnlineTuningDateTime();
	ProjectPtr->SetProjectSaveSpcPartImageMode(SAVE_SPC_PART_IMAGE_DISABLE);
	ProjectPtr->UpdateSelectObjToNeedToCalculateRgn();
	ProjectPtr->SetProjectSpcFileSaveEnabled(false);	

	if ( TRUE == bSaveField )
	{	AOIDataCollect.SetSaveOfflineFiles(true); }
	else
	{	AOIDataCollect.SetSaveOfflineFiles(false); }

	AOIDataCollect.SetIsRepeatTest(false);
	AOIDataCollect.SetIsRepeatTestUI(false);
	AOIDataCollect.SetCallbackWnd(CWnd::GetSafeHwnd());
	AOIDataCollect.SetInspectingMode(INSPECTING_SELECTED);	
	AOIDataCollect.ReleaseModelUniFrameList();
	SendMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_CLOSE_ACTIVE_OBJ, NULL);
	if ( AOIDataCollect.GetOfflineMode() == false )
	{	AOIDataCollect.SetTaskMode(TASK_TUNING_PROJECT);	}
	else
	{	AOIDataCollect.SetTaskMode(TASK_TUNING_OFFLINE);	}			
	if ( AOIDataCollect.ExecInspectionProjectTuning() == false )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return;
	}
	LockUIWnd(true);	
	CreateMapImage();
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::OnUpdateSoftwareBarcodeInspectAll(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) 
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE); }
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::OnSoftwareBarcodeInspectSelected() 
{
	// TODO: Add your command handler code here
	CString str;	
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();
	bool IsNeedGrabFiducial = AOIDataCollect.GetIsNeedGrabFiducial();	
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	//ProjectPtr->SelectProjectAllFds(false);
	//ProjectPtr->SelectProjectAllBarcodes(true);
	ProjectPtr->SelectProjectAllComponents(false);
	ProjectPtr->SelectProjectFdForTuning(OfflineMode, IsNeedGrabFiducial);
	ProjectPtr->ResetProjectOnlineTuningDateTime();
	ProjectPtr->SetProjectSaveSpcPartImageMode(SAVE_SPC_PART_IMAGE_DISABLE);
	ProjectPtr->UpdateSelectObjToNeedToCalculateRgn();
	ProjectPtr->SetProjectSpcFileSaveEnabled(false);	

	AOIDataCollect.SetIsRepeatTest(false);
	AOIDataCollect.SetIsRepeatTestUI(false);
	AOIDataCollect.SetCallbackWnd(CWnd::GetSafeHwnd());
	AOIDataCollect.SetInspectingMode(INSPECTING_SELECTED);	
	AOIDataCollect.ReleaseModelUniFrameList();
	SendMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_CLOSE_ACTIVE_OBJ, NULL);
	if ( AOIDataCollect.GetOfflineMode() == false )
	{	AOIDataCollect.SetTaskMode(TASK_TUNING_PROJECT);	}
	else
	{	AOIDataCollect.SetTaskMode(TASK_TUNING_OFFLINE);	}			
	if ( AOIDataCollect.ExecInspectionProjectTuning() == false )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return;
	}
	LockUIWnd(true);	
	CreateMapImage();
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::OnUpdateSoftwareBarcodeInspectSelected(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr )
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE); }
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::OnSoftwareBarcodeDelAll() 
{
	// TODO: Add your command handler code here
	ExecDeleteBarcodeAll();
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::OnUpdateSoftwareBarcodeDelAll(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) 
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE); }
}
//-------------------------------------------------------------------------------------//
bool CEditBarcodeView::ExecInspection_Finish()
{	
	SwitchProject();
	m_UpdateTestMapTickCount = 0;
	CAOIProject *ProjectPtr = CEditBarcodeView::GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }	
	
	CString   OfflineFdName;
	CString   OfflineFolder;
	bool      RepeatTest=false;
	TASK_MODE TaskMode = AOIDataCollect.GetTaskMode(); 	
	OFFLINE_FILE_MODE OfflineFileMode = ProjectPtr->GetProjectOfflineFileMode();
	AOIDataCollect.SetModelAttachedObj(MODEL_ATTACHED_BARCODE);	
	switch ( TaskMode )
	{
	case TASK_ALIGN_PROJECT:
		LockUIWnd(false);
		AOIDataCollect.SetOfflineMode(false);
		AOIDataCollect.SetProjectMapMode(false);
		break;
	case TASK_INSPECT_PROJECT:
		RepeatTest = true;
		break;
	default:
		LockUIWnd(false);
		break;
	}	
	if ( false == RepeatTest )
	{
		ExecInspectionFinishKernel(false);
		/*
		if ( AOIDataCollect.SetProjectLightSetting(ProjectPtr) == false )
		{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString()); }		
		if ( AOIDataCollect.MoveStageToInspectionFinishComponent(ProjectPtr) == false )
		{	ExecMoveToStage(); }
		SendMessageToMainFrameWnd(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_UPDATE, (LPARAM)(this));
		*/
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditBarcodeView::ExecOnlineInspection_Finish()
{
	ExecInspectionFinishKernel(true);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditBarcodeView::ExecInspectionFinishKernel(bool bOnline)
{
	LockUIWnd(false);
	if ( true == bOnline )
	{
		AOIDataCollect.SetOfflineMode(false);
		AOIDataCollect.SetProjectMapMode(false);
		AOIDataCollect.ExecOnlineInspectionFinish();
	}
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }	
	AOIDataCollect.SetDrawModelMode(DRAW_MODEL_EDIT);
	if ( AOIDataCollect.SetProjectLightSetting(ProjectPtr) == false )
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	}		
	ExecMoveToStage();
	bool bShowResultWnd = AOIDataCollect.GetShowUIWndResultList();
	SendMessageToMainFrameWnd(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_UPDATE, (LPARAM)(this));	
	if ( true == bShowResultWnd )	
	{	SendMessageToMainFrameWnd(MSG_EDIT_RESULT_LIST_WND, WPARAM_BUILD_RESULT_LIST, NULL); }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditBarcodeView::LockUIWnd(bool bLock)//鎖住視窗
{
	AOIDataCollect.SetIsLockUIWnd(bLock);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditBarcodeView::GetLockUIWnd() const//取得是否鎖住UIWnd
{
	return AOIDataCollect.GetIsLockUIWnd();
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::OnSoftwareBarcodeAlignFiducial() 
{
	// TODO: Add your command handler code here
	CString str;	
	CAOIProject *ProjectPtr = CEditBarcodeView::GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	
	CAOIComponent *ComponentPtr = ProjectPtr->GetProjectActiveComponent();	
	AOIDataCollect.CloseActiveComponent(ComponentPtr);	
	
	AOIDataCollect.SetIsRepeatTest(false);
	AOIDataCollect.SetIsRepeatTestUI(false);
	AOIDataCollect.SetIsNeedGrabFiducial(true);
	AOIDataCollect.SetCallbackWnd(CWnd::GetSafeHwnd());
	AOIDataCollect.SetTaskMode(TASK_ALIGN_PROJECT);	
	AOIDataCollect.ReleaseModelUniFrameList();
	SendMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_CLOSE_ACTIVE_OBJ, NULL);
	if ( AOIDataCollect.ExecInspectionProjectTuning() == false )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return;
	}
	LockUIWnd(true);	
	CreateMapImage();
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::OnUpdateSoftwareBarcodeAlignFiducial(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
#ifndef OFFLINE_VERSION
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd ) 
	{	pCmdUI->Enable(FALSE); }	
	else
	{
		size_t FdCount = ProjectPtr->GetProjectFdCount();
		if ( 0 == FdCount )
		{	pCmdUI->Enable(FALSE);	}
		else
		{	pCmdUI->Enable(TRUE); }
	}	
#else
	pCmdUI->Enable(FALSE);
#endif//OFFLINE_VERSION
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::OnSoftwareBarcodePropertyWnd()
{
	const char fnName[] = "CEditBarcodeView::OnSoftwareBarcodePropertyWnd";
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }	
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return; }	
	if ( ModelPtr->CheckModelTypeEnabled() == false )
	{	return; }
	
	CAOIModel *ModelPtrTmp = ModelPtr->CloneModelObj();
	if ( NULL == ModelPtrTmp ) { return; }
	const bool bCloned = true;		
	std::vector<TUNI_FRAME> UniFrameList;	
	const double AttachedAngle = ModelPtrTmp->GetModelAttachedAngle();
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(AttachedAngle);	

	AOIDataCollect.CopyModelUniFrameList(UniFrameList, bCloned);
	AOIDataCollect.SetDrawImageMode(DRAW_IMAGE_NORMAL);	
	if ( true == IsExceptionAngle ) 
	{
		size_t      i=0;
		TUNI_FRAME  UniFrameTmp;	
		std::vector<TUNI_FRAME> UniFrameListTmp;	
		const size_t FrameCount = UniFrameList.size();
		for ( i=0; i<FrameCount; i++ )
		{
			if ( ImageAPI.RotateUniImage(-AttachedAngle, UniFrameList[i], 4, fnName, UniFrameTmp) == false ) 
			{
				JetAPI::ClearUniFrameList(UniFrameList);
				JetAPI::ClearUniFrameList(UniFrameListTmp);
				return;
			}
			UniFrameListTmp.push_back(UniFrameTmp);
		}
		JetAPI::ClearUniFrameList(UniFrameList);
		UniFrameList = UniFrameListTmp;
		ModelPtrTmp->RotateModel(-AttachedAngle, 0, 0);
	}
	
	CModelPropertyWnd ModelProptyWnd;
	ModelProptyWnd.SetModelPtr(ModelPtrTmp);
	ModelProptyWnd.SetUniFrameList(UniFrameList);
	if ( ModelProptyWnd.DoModal() == IDCANCEL )
	{
		JetAPI::ClearUniFrameList(UniFrameList);	
		AOIObjManager.DestroyModelObj(ModelPtrTmp);
		return;
	}
	JetAPI::ClearUniFrameList(UniFrameList);
	if ( true == IsExceptionAngle ) 
	{	ModelPtrTmp->RotateModel(AttachedAngle, 0, 0);	}
	ModelPtr->CopyModelProperty(ModelPtrTmp);
	AOIObjManager.DestroyModelObj(ModelPtrTmp);	

	ModelPtr->CalcModelTotalRegionAll();
	ModelPtr->UpdateModelRegionToAttached();	
	UpdateBarcodeSelected();
	return;
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::OnUpdateSoftwareBarcodePropertyWnd(CCmdUI* pCmdUI)
{
	BOOL bEnable=TRUE;
	bool bLockUIWnd = GetLockUIWnd();	
	CAOIModel *ModelPtr = GetModelPtr();	
	if ( true==bLockUIWnd || NULL==ModelPtr )
	{	bEnable = FALSE; }
	pCmdUI->Enable(bEnable);
}
//-------------------------------------------------------------------------------------//
bool CEditBarcodeView::RetrieveCameraImage(WPARAM wParam, LPARAM lParam, bool bCameraCallBack)//取得相機影像
{
	CString str;	
	size_t   i=0;	
	TSIZE2D  Res;
	TPOINT3D Pos;
	IMAGE_SIZE ImageStep=0;
	IMAGE_SIZE BitCount=0;
	size_t   BuffserSize=0;
	CAMERA_ID CameraID = (CAMERA_ID)(wParam);	
	if ( PRIMARY_CAMERA_ID != CameraID ) { return false; }
	if ( LPARAM_CAMERA_BYPASS_CALLBACK == lParam )
	{	return true;	}	
	
	bool bReturn=false;
	if ( CameraCtrl.RetrieveCameraImageCallback(CameraID, bReturn) == false )
	{
		JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());		
		return false; 
	}
	if ( true == bReturn )
	{	return true; }

	LockUIWnd(false);
	TPOINT2D ImageRes;
	const double Ratio = 1.0;
	double PosX=0, PosY=0, PosZ=0;
	const double FOVWum = AOIDataCollect.GetFovSizeRealW();
	const double FOVHum = AOIDataCollect.GetFovSizeRealH();	
	const double FovMinW = AOIDataCollect.GetFovSizeMinW_Zoom();
	const double FovMinH = AOIDataCollect.GetFovSizeMinH_Zoom();	
	const double TargetMinW = AOIDataCollect.GetTargetMinSizeW_Zoom();
	const double TargetMinH = AOIDataCollect.GetTargetMinSizeH_Zoom();
	const double TargetOffsetX = AOIDataCollect.GetFovTargetOffsetX();
	const double TargetOffsetY = AOIDataCollect.GetFovTargetOffsetY();

	MotionCtrlPtr->GetCurrentPos(PosX, PosY, PosZ);
	ImageRes.x = AOIDataCollect.GetCameraResolutionX(CameraID);
	ImageRes.y = AOIDataCollect.GetCameraResolutionY(CameraID);

	m_FovRatio = Ratio;
	m_FOVPosStage.x = PosX;
	m_FOVPosStage.y = PosY;
	//ResetImageOffset();
	m_FrameResolution = ImageRes;
	m_FrameStageRgn.minX = PosX-(FOVWum*0.5);
	m_FrameStageRgn.maxX = PosX+(FOVWum*0.5);
	m_FrameStageRgn.minY = PosY-(FOVHum*0.5);
	m_FrameStageRgn.maxY = PosY+(FOVHum*0.5);

	TPOINT2D PosCad;	
	TPOINT2D PosStage(TargetOffsetX, TargetOffsetY);		
	const bool bKeepImageOffset = GetKeepImageOffset();
	AOIDataCollect.MapStageOffsetPtToCad(PosStage, PosCad);		
	if ( false == bKeepImageOffset )
	{
		m_ImageOffset.x = -PosCad.x/(ImageRes.x);
		m_ImageOffset.y =  PosCad.y/(ImageRes.y);
		m_ImageOffset.x = m_ImageOffset.x/(m_ImageZoom);
		m_ImageOffset.y = m_ImageOffset.y/(m_ImageZoom);
	}
	SetKeepImageOffset(false);
	AOIDataCollect.SetFovTargetOffsetX(0);
	AOIDataCollect.SetFovTargetOffsetY(0);

	std::vector<TUNI_FRAME>   UniFrameList;
	std::vector<TFrameParam>  GrabFrameParamList;//影像參數列表			
	AOIDataCollect.GetGrabFrameParamList(GrabFrameParamList);
	const size_t GrabFrameParamCount = GrabFrameParamList.size();
	if ( 0 == GrabFrameParamCount ) { return false; }

	if ( true == bCameraCallBack )
	{	AOIDataCollect.ModifyCameraUniFrame(GrabFrameParamList);	}
	if ( AOIDataCollect.RetrieveCameraUniFrame(GrabFrameParamList, UniFrameList) == false )
	{	
		JetAPI::ClearUniFrameList(UniFrameList);
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());		
		return false;
	}
	ReleaseUniFrameBuffer();
	ReleaseShowImageBuffer();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr )
	{	m_ImageIndex = 0; }
	else
	{	m_ImageIndex = ProjectPtr->GetProjectMapIndex(); }	
	const size_t MaxFrames = GetMaxFrameCount();
	const size_t UniFrameCount = UniFrameList.size();
	const size_t MinUniFrameCount = MIN(MaxFrames, UniFrameCount);
	for ( i=0; i<MinUniFrameCount; i++ )
	{	m_UniFrameList[i] = UniFrameList[i];	}
	for ( i=MinUniFrameCount; i<UniFrameCount; i++ )
	{	JetAPI::ClearUniFrame(UniFrameList[i]);	}
	AOIDataCollect.SetFieldUniFrameList(m_FrameStageRgn, m_UniFrameList, MaxFrames);

	BuildShowImageBuffer();
	//AOIDataCollect.ReleaseModelUniFrameList();//因為變更了FOV的位置與圖檔所以清除原有的模組資料	
	UpdateBarcodeModelStats();		
	CreateBKImage();
	RedrawWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditBarcodeView::ExecToggleEnhanceImageMode()
{
	bool IsOK = true;
	IsOK = UpdateImageByAlgParam();	
	if ( true == IsOK )
	{	return; }
	UpdateFrameImage();
	return;
}
//-------------------------------------------------------------------------------------//