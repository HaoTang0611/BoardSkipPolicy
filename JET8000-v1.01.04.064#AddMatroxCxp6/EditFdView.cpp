// EditFdView.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "EditFdView.h"
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
// CEditFdView
//-------------------------------------------------------------------------------------//
IMPLEMENT_DYNCREATE(CEditFdView, CView)
//-------------------------------------------------------------------------------------//
CEditFdView::CEditFdView()
{	
	m_ModelPtr = NULL;
	m_ProjectPtr = NULL;
	m_BkColor=0x000000;
	m_ImageZoom = 1.0;
	m_ImageOffset.x = m_ImageOffset.y = 0;
	m_DrawAddRect = false;
	m_DrawEditRect = false;
	m_KeepImageOffset = false;
	m_FovRatio = 1.0;
	m_ImageIndex = 0;
	m_ModifyFdPos = false;
	m_ShowPopupMenu = true;
	m_ShowImageW = 1024;
	m_ShowImageH = 1024;
	m_ShowImageStep = 1024*3;
	m_ShowBitCount = 24;
	m_ShowImagePtr = NULL;
	m_AlignFdMode = ALIGN_FD_PANEL;
	m_UpdateTestMapTickCount = 0;
	CloseProject();	
	PreInitImageBuffer();	
}
//-------------------------------------------------------------------------------------//
CEditFdView::~CEditFdView()
{
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CEditFdView, CView)
	//{{AFX_MSG_MAP(CEditFdView)
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
	ON_COMMAND(MENU_FIDUCIAL_EDIT_ADD_MODE, OnFiducialEditAddMode)
	ON_UPDATE_COMMAND_UI(MENU_FIDUCIAL_EDIT_ADD_MODE, OnUpdateFiducialEditAddMode)	
	ON_COMMAND(MENU_FIDUCIAL_EDIT_EDIT_MODE, OnFiducialEditEditMode)
	ON_UPDATE_COMMAND_UI(MENU_FIDUCIAL_EDIT_EDIT_MODE, OnUpdateFiducialEditEditMode)
	ON_COMMAND(MENU_FIDUCIAL_EDIT_CALIBRATION, OnFiducialEditCalibration)
	ON_UPDATE_COMMAND_UI(MENU_FIDUCIAL_EDIT_CALIBRATION, OnUpdateFiducialEditCalibration)
	ON_COMMAND(MENU_FIDUCIAL_PASTE_TO_OTHER_BOARDS, OnFiducialPasteToOtherBoards)
	ON_UPDATE_COMMAND_UI(MENU_FIDUCIAL_PASTE_TO_OTHER_BOARDS, OnUpdateFiducialPasteToOtherBoards)
	ON_COMMAND(MENU_FIDUCIAL_EDIT_DEL_SELECTED, OnFiducialEditDelSelected)
	ON_UPDATE_COMMAND_UI(MENU_FIDUCIAL_EDIT_DEL_SELECTED, OnUpdateFiducialEditDelSelected)
	ON_COMMAND(MENU_FIDUCIAL_EDIT_ALIGN_PANEL, OnFiducialEditAlignPanel)
	ON_UPDATE_COMMAND_UI(MENU_FIDUCIAL_EDIT_ALIGN_PANEL, OnUpdateFiducialEditAlignPanel)
	ON_COMMAND(MENU_FIDUCIAL_EDIT_ALIGN_PANEL_COMBO, OnFiducialEditAlignPanelCombo)
	ON_UPDATE_COMMAND_UI(MENU_FIDUCIAL_EDIT_ALIGN_PANEL_COMBO, OnUpdateFiducialEditAlignPanelCombo)
	ON_COMMAND(MENU_FIDUCIAL_EDIT_ALIGN_BOARD, OnFiducialEditAlignBoard)
	ON_UPDATE_COMMAND_UI(MENU_FIDUCIAL_EDIT_ALIGN_BOARD, OnUpdateFiducialEditAlignBoard)
	ON_COMMAND(MENU_FIDUCIAL_EDIT_ALIGN_BOARD_COMBO, OnFiducialEditAlignBoardCombo)
	ON_UPDATE_COMMAND_UI(MENU_FIDUCIAL_EDIT_ALIGN_BOARD_COMBO, OnUpdateFiducialEditAlignBoardCombo)
	ON_COMMAND(MENU_FIDUCIAL_EDIT_ALIGN_SET_BOARD_COMBO1, OnFiducialEditAlignSetBoardCombo1)
	ON_UPDATE_COMMAND_UI(MENU_FIDUCIAL_EDIT_ALIGN_SET_BOARD_COMBO1, OnUpdateFiducialEditAlignSetBoardCombo1)
	ON_COMMAND(MENU_FIDUCIAL_EDIT_ALIGN_SET_BOARD_COMBO2, OnFiducialEditAlignSetBoardCombo2)
	ON_UPDATE_COMMAND_UI(MENU_FIDUCIAL_EDIT_ALIGN_SET_BOARD_COMBO2, OnUpdateFiducialEditAlignSetBoardCombo2)
	ON_COMMAND(MENU_FIDUCIAL_EDIT_ALIGN_SET_BOARD_COMBO3, OnFiducialEditAlignSetBoardCombo3)
	ON_UPDATE_COMMAND_UI(MENU_FIDUCIAL_EDIT_ALIGN_SET_BOARD_COMBO3, OnUpdateFiducialEditAlignSetBoardCombo3)
	ON_COMMAND(MENU_FIDUCIAL_EDIT_ALIGN_SET_BOARD_COMBO4, OnFiducialEditAlignSetBoardCombo4)
	ON_UPDATE_COMMAND_UI(MENU_FIDUCIAL_EDIT_ALIGN_SET_BOARD_COMBO4, OnUpdateFiducialEditAlignSetBoardCombo4)
	ON_COMMAND(MENU_FIDUCIAL_EDIT_ALIGN_SET_COMPONENT_COMBO1, OnFiducialEditAlignSetComponentCombo1)
	ON_UPDATE_COMMAND_UI(MENU_FIDUCIAL_EDIT_ALIGN_SET_COMPONENT_COMBO1, OnUpdateFiducialEditAlignSetComponentCombo1)
	ON_COMMAND(MENU_FIDUCIAL_EDIT_ALIGN_SET_COMPONENT_COMBO2, OnFiducialEditAlignSetComponentCombo2)
	ON_UPDATE_COMMAND_UI(MENU_FIDUCIAL_EDIT_ALIGN_SET_COMPONENT_COMBO2, OnUpdateFiducialEditAlignSetComponentCombo2)
	ON_COMMAND(MENU_FIDUCIAL_EDIT_ALIGN_SET_COMPONENT_COMBO3, OnFiducialEditAlignSetComponentCombo3)
	ON_UPDATE_COMMAND_UI(MENU_FIDUCIAL_EDIT_ALIGN_SET_COMPONENT_COMBO3, OnUpdateFiducialEditAlignSetComponentCombo3)
	ON_COMMAND(MENU_FIDUCIAL_EDIT_ALIGN_SET_COMPONENT_COMBO4, OnFiducialEditAlignSetComponentCombo4)
	ON_UPDATE_COMMAND_UI(MENU_FIDUCIAL_EDIT_ALIGN_SET_COMPONENT_COMBO4, OnUpdateFiducialEditAlignSetComponentCombo4)
	ON_COMMAND(MENU_FIDUCIAL_EDIT_ALIGN_SET_COMPONENT1, OnFiducialEditAlignSetComponent1)
	ON_UPDATE_COMMAND_UI(MENU_FIDUCIAL_EDIT_ALIGN_SET_COMPONENT1, OnUpdateFiducialEditAlignSetComponent1)
	ON_COMMAND(MENU_FIDUCIAL_EDIT_ALIGN_SET_COMPONENT2, OnFiducialEditAlignSetComponent2)
	ON_UPDATE_COMMAND_UI(MENU_FIDUCIAL_EDIT_ALIGN_SET_COMPONENT2, OnUpdateFiducialEditAlignSetComponent2)
	ON_COMMAND(MENU_FIDUCIAL_EDIT_ALIGN_SET_COMPONENT3, OnFiducialEditAlignSetComponent3)
	ON_UPDATE_COMMAND_UI(MENU_FIDUCIAL_EDIT_ALIGN_SET_COMPONENT3, OnUpdateFiducialEditAlignSetComponent3)
	ON_COMMAND(MENU_FIDUCIAL_EDIT_ALIGN_SET_COMPONENT4, OnFiducialEditAlignSetComponent4)
	ON_UPDATE_COMMAND_UI(MENU_FIDUCIAL_EDIT_ALIGN_SET_COMPONENT4, OnUpdateFiducialEditAlignSetComponent4)		
	ON_COMMAND(MENU_FIDUCIAL_EDIT_ALIGN_FD, OnFiducialEditAlign)
	ON_UPDATE_COMMAND_UI(MENU_FIDUCIAL_EDIT_ALIGN_FD, OnUpdateFiducialEditAlign)
	ON_COMMAND(MENU_FIDUCIAL_EDIT_ALIGN_FD_ALL, OnFiducialEditAlignFdAll)
	ON_UPDATE_COMMAND_UI(MENU_FIDUCIAL_EDIT_ALIGN_FD_ALL, OnUpdateFiducialEditAlignFdAll)
	ON_COMMAND(MENU_FIDUCIAL_EDIT_ALIGN_FD_SELECTED, OnFiducialEditAlignFdSelected)
	ON_UPDATE_COMMAND_UI(MENU_FIDUCIAL_EDIT_ALIGN_FD_SELECTED, OnUpdateFiducialEditAlignFdSelected)
	ON_COMMAND(MENU_FIDUCIAL_EDIT_PROPERTY_WND, OnFiducialEditPropertyWnd)
	ON_UPDATE_COMMAND_UI(MENU_FIDUCIAL_EDIT_PROPERTY_WND, OnUpdateFiducialEditPropertyWnd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CEditFdView drawing
//-------------------------------------------------------------------------------------//
void CEditFdView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
	CEditFdView::RedrawWnd();
}
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CEditFdView diagnostics
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
void CEditFdView::AssertValid() const
{
	CView::AssertValid();
}
//-------------------------------------------------------------------------------------//
void CEditFdView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CEditFdView message handlers
 void CEditFdView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();	
	// TODO: Add your specialized code here and/or call the base class
	CEditFdView::SwitchMultiLanguage();
	CEditFdView::GetClientRect(&m_ImageWndRect);
	m_ImageWndMapDC.CreateMemDC(this, m_BkColor);
	m_ImageWndMemDC.CreateMemDC(this, m_BkColor);
	m_ImageWndMemDC2.CreateMemDC(this, m_BkColor);

	SetShowPopupMenu(true);
	AOIDataCollect.SetShowFdList(true);	
	AOIDataCollect.SetShowMarkList(false);
	AOIDataCollect.SetShowPanelList(true);	
	AOIDataCollect.SetShowBoardList(true);	
	AOIDataCollect.SetShowBarcodeList(false);
	AOIDataCollect.SetShowComponentList(false);	
	AOIDataCollect.SetCallbackWnd(CWnd::GetSafeHwnd());
	AOIDataCollect.SetDrawModelMode(DRAW_MODEL_EDIT);
	AOIDataCollect.SetModelAttachedObj(MODEL_ATTACHED_FD);	
	//AOIDataCollect.SwitchProjectTaskMode(PROJECT_TASK_NORMAL);

	SwitchProject();	
	RestoreViewParam();
	CreateBKImage();
	//CEditFdView::ExecMoveToStage();//由Frame視窗發送MSG_CAMERA_REGRAB_IMAGE		
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_PART_LIST_WND, WPARAM_BUILD_PART_LIST, LPARAM_BUILD_DOCK_LIST_EDIT);
	CWnd::PostMessage(MSG_CAMERA_REGRAB_IMAGE, NULL, NULL);
	return;
}
//-------------------------------------------------------------------------------------//
void CEditFdView::OnDestroy() 
{
	CView::OnDestroy();	
	// TODO: Add your message handler code here		
	CloseProject();	
}
//-------------------------------------------------------------------------------------//
void CEditFdView::OnSize(UINT nType, int cx, int cy) 
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
BOOL CEditFdView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
	return CView::OnEraseBkgnd(pDC);
}
//-------------------------------------------------------------------------------------//
void CEditFdView::OnShowWindow(BOOL bShow, UINT nStatus) 
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
void CEditFdView::OnContextMenu(CWnd* pWnd, CPoint point) 
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
}
//-------------------------------------------------------------------------------------//
BOOL CEditFdView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	UINT ControlID = pWnd->GetDlgCtrlID();	

	MANIPULATE_MODEL_MODE ManiMode = AOIDataCollect.GetManipulateModelMode();

	CURSOR_POS_MODE CursorMode = CURSOR_POS_NONE;
	CURSOR_POS_MODE OldCursorMode = m_MousePosMode;	
	if ( MANIPULATE_MODEL_CALIBRATION== ManiMode )
	{	CursorMode = CheckCursorPosModeEdit(m_MousePosImageWnd);	}
	else
	{	CursorMode = CheckCursorPosModeFd(m_MousePosImageWnd); }
	m_MousePosMode = CursorMode;
	
	//if ( CursorMode != OldCursorMode )
	//{	CEditFdView::RedrawWnd();	}
	if ( CURSOR_POS_NONE == CursorMode )
	{	return CView::OnSetCursor(pWnd, nHitTest, message);	}

	JetAPI::UpdateCursor(CursorMode); 
	return TRUE;

	//return CView::OnSetCursor(pWnd, nHitTest, message);
}
//-------------------------------------------------------------------------------------//
void CEditFdView::OnLButtonDown(UINT nFlags, CPoint point) 
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
bool CEditFdView::ExecSaveLogLButtonUp()
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
void CEditFdView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	::ReleaseCapture();
	ExecModifySelectedFdFinish();
	m_MousePosLast = m_MousePosCurrent;	
	m_MousePosImageWnd = m_MousePosCurrent = point;
	//CWnd::MapWindowPoints(&m_ImageWnd, &m_MousePosImageWnd, 1);		
	MANIPULATE_MODEL_MODE ManiMode = AOIDataCollect.GetManipulateModelMode();
	
	if ( MANIPULATE_MODEL_ADD == ManiMode )
	{	CEditFdView::ExecAddFd();	}
	else if ( MANIPULATE_MODEL_SELECT==ManiMode || MANIPULATE_MODEL_EDIT==ManiMode )
	{			
		if ( CURSOR_POS_NONE == m_MousePosMode )
		{	
			ExecSelectFd();
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
		{	AOIDataCollect.CancelGatherColorMode();	 }
		RedrawWnd();
	}
	//if ( CURSOR_POS_NONE == m_MousePosMode )
	//{	ExecEditSelectFd(); }
	SetDrawAddRect(false);
	RedrawWnd();
	CView::OnLButtonUp(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CEditFdView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CView::OnLButtonDblClk(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CEditFdView::OnRButtonDown(UINT nFlags, CPoint point) 
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
void CEditFdView::OnRButtonUp(UINT nFlags, CPoint point) 
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
void CEditFdView::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default	
	//CEditFdView::CalcFovPosition();	
	CView::OnRButtonDblClk(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CEditFdView::OnMouseMove(UINT nFlags, CPoint point) 
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
		CEditFdView::RedrawWnd();
		CView::OnMouseMove(nFlags, point);
		return;
	}

	//JetAPI::UpdateCursor();
	if ( nFlags & MK_LBUTTON )//滑鼠左鍵
	{
		bool Modify = false;
		MANIPULATE_MODEL_MODE ManiMode = AOIDataCollect.GetManipulateModelMode();
		switch ( m_MousePosMode )
		{
		case CURSOR_POS_INNER:
			//Modify = ExecMoveSelectedFd();
			if ( MANIPULATE_MODEL_CALIBRATION == ManiMode )
			{	Modify = ExecMoveSelectedEdit();	}
			else
			{	Modify = ExecModifyActiveObjPos(); }
			break;
		case CURSOR_POS_LEFT:
		case CURSOR_POS_RIGHT:
		case CURSOR_POS_TOP:
		case CURSOR_POS_BOTTOM:
		case CURSOR_POS_LEFT_TOP:
		case CURSOR_POS_LEFT_BOTTOM:
		case CURSOR_POS_RIGHT_TOP:
		case CURSOR_POS_RIGHT_BOTTOM:
			//Modify = ExecResizeSelectedFd(m_MousePosMode);
			if ( MANIPULATE_MODEL_CALIBRATION == ManiMode )
			{	Modify = ExecResizeSelectedEdit(m_MousePosMode);	}
			else
			{	Modify = ExecModifyActiveObjSize(m_MousePosMode); }
			break;
		}
		if ( true == Modify )
		{	bToDraw = true;	}
	}
	else if (nFlags & MK_RBUTTON )//滑鼠右鍵
	{
		this->m_ImageOffset.x += dPoint.x;
		this->m_ImageOffset.y += dPoint.y;
		//CEditFdView::CalcFovPosition();
		CEditFdView::CreateBKImage();
		bToDraw = true;		
	}	
	if ( true == m_DrawAddRect )
	{	bToDraw = true;		}	
	if ( true == m_DrawEditRect )
	{	bToDraw = true;		}	
	if ( true == bToDraw )
	{	CEditFdView::RedrawWnd();	}
	CView::OnMouseMove(nFlags, point);
}
//-------------------------------------------------------------------------------------//
BOOL CEditFdView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	double NextImageZoom = m_ImageZoom;
	if ( zDelta > 0 ) 
	{	NextImageZoom *= 1.1;	}
	else
	{	NextImageZoom /= 1.1;	}
	
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();	
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
	//CEditFdView::CalcCursorInfo(point);
	//CEditFdView::CalcImageWndLBtn();
	//CEditFdView::DrawImage();
	CEditFdView::CreateBKImage();
	CEditFdView::RedrawWnd();
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}
//-------------------------------------------------------------------------------------//
BOOL CEditFdView::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class		
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
LRESULT CEditFdView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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
					BuildFdSelected();
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
			BuildFdSelected();
			RedrawWnd(); 			
			break;
		case WPARAM_PROJECT_PART_DELETED:			
			BuildFdSelected();
			RedrawWnd(); 			
			break;
		case WPARAM_CALC_CURRENT_FOV_POSITION:
			CalcFovPosition();
			break;
		case WPARAM_PROJECT_SWITCH_MARK:
			break;
		case WPARAM_PROJECT_CLOSE_ACTIVE_OBJ:
			ResetCaliComponentList();
			ReleaseImageBuffer();	
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
			UpdateFdSelected();	
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
void CEditFdView::SwitchMultiLanguage()
{
	/*
	size_t  i = 0;	
	UINT    WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_EDIT_FD_VIEW");	
	//---------------------------------------------------------------------------------//
	WndID = IDD_EDIT_FD_VIEW;
	WndKey = _T("IDD_EDIT_FD_VIEW");
	this->GetWindowText(LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetWindowText(NewLabelText);
	//---------------------------------------------------------------------------------//	
	*/
}
//-------------------------------------------------------------------------------------//
CString CEditFdView::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_EDIT_FD_VIEW");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
inline bool CEditFdView::PtInControlWnd(const POINT &pt, UINT ID, POINT &pt2)
{
	pt2 = pt;
	if ( ::PtInRect(&m_ImageWndRect, pt) == FALSE )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
inline BOOL CEditFdView::MapWndPtToImagePt_DBL(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const RECT &WndRect, const TPOINT2D &OffsetPt, double ZoomScale, const TPOINT2D &WndPt, TPOINT2D &ImagePt)
{
	ImageAPI.MapWndPtToImagePt_DBL(ImageW, ImageH, WndRect, OffsetPt, ZoomScale, WndPt, ImagePt);
	return TRUE;
}
//-------------------------------------------------------------------------------------//
inline BOOL CEditFdView::MapImagePtToWndPt_DBL(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const RECT &WndRect, const TPOINT2D &OffsetPt, double ZoomScale, const TPOINT2D &ImagePt, TPOINT2D &WndPt)
{
	ImageAPI.MapImagePtToWndPt_DBL(ImageW, ImageH, WndRect, OffsetPt, ZoomScale, ImagePt, WndPt);
	return TRUE;
}
//-------------------------------------------------------------------------------------//
inline BOOL CEditFdView::MapImageRgnToWndRgn_DBL(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const RECT &WndRect, const TPOINT2D &OffsetPt, double ZoomScale, const TREGION4D &ImageRgn, TREGION4D &WndRgn)
{
	ImageAPI.MapImageRgnToWndRgn_DBL(ImageW, ImageH, WndRect, OffsetPt, ZoomScale, ImageRgn, WndRgn);
	return TRUE;
}
//-------------------------------------------------------------------------------------//
inline IMAGE_SIZE CEditFdView::GetFrameImageW_2() const
{
	return m_ShowImageW;
}
//-------------------------------------------------------------------------------------//
inline IMAGE_SIZE CEditFdView::GetFrameImageH_2() const
{
	return m_ShowImageH;
}
//-------------------------------------------------------------------------------------//
IMAGE_SIZE CEditFdView::GetImageW() const
{
	return GetFrameImageW_2();
}
//-------------------------------------------------------------------------------------//
IMAGE_SIZE CEditFdView::GetImageH() const
{
	return GetFrameImageH_2();
}
//-------------------------------------------------------------------------------------//
const TPOINT2D& CEditFdView::GetImageResolution() const
{
	return m_FrameResolution;
}
//-------------------------------------------------------------------------------------//
const TREGION4D& CEditFdView::GetImageStageRgn() const
{
	return m_FrameStageRgn;
}
//-------------------------------------------------------------------------------------//
const TPOINT2D CEditFdView::GetImageStageRgnCp() const
{
	TPOINT2D ImageStageRgnCp;
	const TREGION4D &ImageStageRgn = GetImageStageRgn();
	ImageStageRgnCp.x = ImageStageRgn.GetCpX();
	ImageStageRgnCp.y = ImageStageRgn.GetCpY();
	return ImageStageRgnCp;
}
//-------------------------------------------------------------------------------------//
bool CEditFdView::GetShowProjectMapMode() const
{
	return false;
}
//-------------------------------------------------------------------------------------//
bool CEditFdView::CheckMousePosMoved() const//確認滑鼠移動過
{
	const int dx = m_MousePosLast.x-m_MousePosFirst.x;
	const int dy = m_MousePosLast.y-m_MousePosFirst.y;
	if ( abs(dx) > 10 || abs(dy) > 10 )
	{	return true; }
	return false;
}
//-------------------------------------------------------------------------------------//
void CEditFdView::CloseProject()
{	
	ResetFdModel();
	ResetCaliComponentList();
	ReleaseImageBuffer();	
	ReleaseShowImageBuffer();

	m_SelFdList.clear();	
	m_ProjectPtr = NULL;	
	m_MapZoom = 1.00;
	m_ImageZoom = 1.00;		
	ResetImageOffset();
	m_AlignFdMode = ALIGN_FD_PANEL;
	m_FrameResolution.x = AOIDataCollect.GetCameraResolutionX(PRIMARY_CAMERA_ID);
	m_FrameResolution.y = AOIDataCollect.GetCameraResolutionY(PRIMARY_CAMERA_ID);
	
	CreateBKImage();
	CreateMapImage();	
}
//-------------------------------------------------------------------------------------//
void CEditFdView::SwitchProject()//切換專案
{	
	CEditFdView::CloseProject();
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL == ProjectPtr )	{	return;	}	

	m_ProjectPtr = ProjectPtr;	
	ProjectPtr->ResetProjectActiveIndex();
	ProjectPtr->ResetProjectRibbonIndex();			
	ProjectPtr->SelectProjectAllObjects(false);
	CreateMapImage();

	SendMessageToMainFrameWnd(MSG_RIBBON_BAR_WND, WPARAM_PANEL_COMBOX_BUILD, MENU_FIDUCIAL_EDIT_ALIGN_PANEL_COMBO);
	SendMessageToMainFrameWnd(MSG_RIBBON_BAR_WND, WPARAM_BOARD_COMBOX_BUILD, MENU_FIDUCIAL_EDIT_ALIGN_BOARD_COMBO);	
	SendMessageToMainFrameWnd(MSG_RIBBON_BAR_WND, WPARAM_BOARD_COMBOX_BUILD, MENU_FIDUCIAL_EDIT_ALIGN_SET_BOARD_COMBO1);
	SendMessageToMainFrameWnd(MSG_RIBBON_BAR_WND, WPARAM_COMPONENT_COMBOX_BUILD, MENU_FIDUCIAL_EDIT_ALIGN_SET_COMPONENT_COMBO1);	
	SendMessageToMainFrameWnd(MSG_RIBBON_BAR_WND, WPARAM_BOARD_COMBOX_BUILD, MENU_FIDUCIAL_EDIT_ALIGN_SET_BOARD_COMBO2);	
	SendMessageToMainFrameWnd(MSG_RIBBON_BAR_WND, WPARAM_COMPONENT_COMBOX_BUILD, MENU_FIDUCIAL_EDIT_ALIGN_SET_COMPONENT_COMBO2);	
	SendMessageToMainFrameWnd(MSG_RIBBON_BAR_WND, WPARAM_BOARD_COMBOX_BUILD, MENU_FIDUCIAL_EDIT_ALIGN_SET_BOARD_COMBO3);	
	SendMessageToMainFrameWnd(MSG_RIBBON_BAR_WND, WPARAM_COMPONENT_COMBOX_BUILD, MENU_FIDUCIAL_EDIT_ALIGN_SET_COMPONENT_COMBO3);	
	SendMessageToMainFrameWnd(MSG_RIBBON_BAR_WND, WPARAM_BOARD_COMBOX_BUILD, MENU_FIDUCIAL_EDIT_ALIGN_SET_BOARD_COMBO4);
	SendMessageToMainFrameWnd(MSG_RIBBON_BAR_WND, WPARAM_COMPONENT_COMBOX_BUILD, MENU_FIDUCIAL_EDIT_ALIGN_SET_COMPONENT_COMBO4);	
}
//-------------------------------------------------------------------------------------//
inline CAOIModel* CEditFdView::GetModelPtr()
{
	return m_ModelPtr;	
}
//-------------------------------------------------------------------------------------//
inline CAOIProject* CEditFdView::GetActiveProject()
{
	return m_ProjectPtr;
}
//-------------------------------------------------------------------------------------//
inline bool CEditFdView::CheckCalibrationMode()//確認校正基板模式
{
	MANIPULATE_MODEL_MODE ManiMode = AOIDataCollect.GetManipulateModelMode();
	if ( MANIPULATE_MODEL_CALIBRATION != ManiMode ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditFdView::ResetCaliComponentList()//復歸校正零件列表
{
	size_t i=0;
	const size_t MaxCount = 4;
	m_CaliIndex = 0;
	SetDrawEditRect(false);
	for ( i=0; i<MaxCount; i++ )
	{
		m_CaliComponentIdx[i] = INVALID_INDEX;//校正的零件引數
		m_CaliComponentCadPosX[i] = 0.0;//校正零件的座標-X-Cad
		m_CaliComponentCadPosY[i] = 0.0;//校正零件的座標-Y-Cad
		m_CaliComponentStagePosX[i] = 0.0;//校正零件的座標-X-Stage
		m_CaliComponentStagePosY[i] = 0.0;//校正零件的座標-Y-Stage
		m_CaliComponentRegion[i] = TREGION4D();
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CEditFdView::BuildFdSelected()//更新選到的定位點
{
	ResetFdModel();
	m_SelFdList.clear();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	//if ( MENU_MAIN_EDIT_MODE_PANEL != m_MainMode ) { return; }
	
	size_t     i = 0;	
	TFdRect    FdRect;
	CAOIFd    *FdPtr = NULL;		
	std::vector<CAOIFd*> SelFdList;
	ProjectPtr->GetProjectFdSelected(SelFdList);
	const size_t SelCount = SelFdList.size();	
	for ( i=0; i<SelCount; i++ )
	{	
		FdPtr = SelFdList[i];
		if ( NULL == FdPtr ) { continue; }
		FdRect.FdPtr = FdPtr;
		FdRect.FdRgn = FdPtr->GetFdRgnStage();
		FdRect.FdIndex = FdPtr->GetFdIndex_Project();		
		m_SelFdList.push_back(FdRect);		
	}		

	FdPtr = AOIDataCollect.GetActiveFd();
	if ( NULL == FdPtr )
	{	m_DrawEditRect = false; }
	else
	{
		m_EditRegion = FdPtr->GetFdRgnStage();
		m_DrawEditRect = true;	
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CEditFdView::RedrawWnd()
{
	CClientDC dc(this);
	HDC hDC = dc.GetSafeHdc();
	HDC hMemDC = m_ImageWndMemDC.GetSafeHdc();
	HDC hMemDC2 = m_ImageWndMemDC2.GetSafeHdc();	
	if ( NULL==hMemDC || NULL==hMemDC2 || NULL==hDC ) 
	{	return; }

	CString str;
	RECT    WndRect = m_ImageWndRect;	
	size_t  SelectedFdCount = 0;
	CAOIProject *ProjectPtr = GetActiveProject();		
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
		DrawFd(hMemDC2);			
		DrawObjectList(hMemDC2);
		DrawEditRect(hMemDC2);	
		DrawAddRect(hMemDC2);	
		DrawModel(hMemDC2);
		DrawModelActivedLine(hMemDC2);		
		DrawCrosshair(hMemDC2);
	}
	::BitBlt(hDC, 0, 0, WndRect.right, WndRect.bottom, hMemDC2, 0, 0, SRCCOPY );	
}
//-------------------------------------------------------------------------------------//
void CEditFdView::CreateBKImage()
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
void CEditFdView::CreateMapImage(bool bTestMap)
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
void CEditFdView::CreateTestMapImage()
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
void CEditFdView::DrawFd(HDC hDC)
{
	CAOIProject *Project = GetActiveProject();
	if ( NULL == Project ) { return; }

	CString      str;
	size_t       i = 0;		
	POINT        Pt={0}, CornerPos[4];
	RECT         Rect={0};	
	bool         FdSelected = false;
	const RECT   WndRect = m_ImageWndRect;
	TPOINT2D     StagePos, ImagePos, WndPt;
	TPOINT2D     StgCornerPos[4], ImgCornerPos[4], WndCornerPos[4];	
	TPOINT2D     ImageStagePos;
	TSIZE2D      FdPatExtend, FdRoiExtend;	
	TREGION4D    ObjStageRgn, ObjImageRgn;	
	CAOIFd      *FdPtr = NULL;
	CAOIPanel   *PanelPtr = NULL;
	CAOIBoard   *BoardPtr = NULL;	

	TPOINT2D ComponentStagePos;
	TPOINT2D StageOffset, CadOffset;

	const IMAGE_SIZE   ImageW = GetImageW();
	const IMAGE_SIZE   ImageH = GetImageH();	
	const TPOINT2D     StageCp = GetImageStageRgnCp();
	const TPOINT2D    &ImageRes = GetImageResolution();
	const TREGION4D   &ImageStageRgn = GetImageStageRgn();

	const double StageCpx = StageCp.x;
	const double StageCpy = StageCp.y;	
	const DRAW_MODEL_MODE DrawModelMode = GetDrawModelMode();
	const DRAW_COMPONENT_MODE  DrawComponentMode = AOIDataCollect.GetDrawComponentMode();//顯示零件模式
	const size_t FdCount = Project->GetProjectFdCount();
	const size_t PanelCount = Project->GetProjectPanelCount();
	const size_t BoardCount = Project->GetProjectBoardCount();
	const size_t ComponentCount = Project->GetProjectComponentCount();
	const TSystemParameter &SystemParam = AOIDataCollect.GetSystemParameter();
	const COLORREF  clr1 = SystemParam.m_FdColor1;
	const COLORREF  clr2 = SystemParam.m_FdColor2;
	const COLORREF  clrOK = SystemParam.m_InspectedResultOKColor;
	const COLORREF  clrNG = SystemParam.m_InspectedResultNGColor;
	const COLORREF  clrText = SystemParam.m_FdTextColor;
	const COLORREF  clrSelected = SystemParam.m_FdSelectedColor;	

	HPEN hPenFd    = ::CreatePen(PS_SOLID, 1, clr1);	
	HPEN hPenFdSel = ::CreatePen(PS_SOLID, 2, clrSelected);
	HPEN hPenFdExt = ::CreatePen(PS_SOLID, 1, clr2);
	HPEN hOldPen = (HPEN)(::SelectObject(hDC, hPenFd));	
	COLORREF clrTextOld = ::SetTextColor(hDC, clrText);
	int BKMode = ::SetBkMode(hDC, TRANSPARENT);		

	ImageStagePos.x = ImageStageRgn.GetCpX();
	ImageStagePos.y = ImageStageRgn.GetCpY();
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = Project->GetProjectFdPtr(i, false);
		if ( NULL == FdPtr ) { continue; }
		if ( FdPtr->GetFdDeleted() == true ) { continue; }		
		StagePos.x = FdPtr->GetFdStagePosX();
		StagePos.y = FdPtr->GetFdStagePosY();
		if ( StagePos.x<ImageStageRgn.minX || StagePos.x>ImageStageRgn.maxX ) { continue; }
		if ( StagePos.y<ImageStageRgn.minY || StagePos.y>ImageStageRgn.maxY ) { continue; }
		FdSelected = FdPtr->GetFdSelected();
		AOIDataCollect.MapStagePtToCamera(ImageW, ImageH, ImageRes, StagePos, ImageStagePos, ImagePos);
		MapImagePtToWndPt_DBL(ImageW, ImageH, WndRect, m_ImageOffset, m_ImageZoom, ImagePos, WndPt);
		
		JetAPI::Point2DToPoint(WndPt, Pt);
		if ( Pt.x<0 || Pt.y<0 || Pt.x>WndRect.right || Pt.y>WndRect.bottom ) { continue; }

		FdPtr->GetFdBodyStageCornerPos(StgCornerPos);		
		AOIDataCollect.MapStageCornerToCamera(ImageW, ImageH, ImageRes, StgCornerPos, ImageStagePos, ImgCornerPos);		
		MapImagePtToWndPt_DBL(ImageW, ImageH, WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[0], WndCornerPos[0]);
		MapImagePtToWndPt_DBL(ImageW, ImageH, WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[1], WndCornerPos[1]);
		MapImagePtToWndPt_DBL(ImageW, ImageH, WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[2], WndCornerPos[2]);
		MapImagePtToWndPt_DBL(ImageW, ImageH, WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[3], WndCornerPos[3]);

		JetAPI::CornerPt2DToCornerPt(WndCornerPos, CornerPos);
		if ( JetAPI::CheckCornerInRect(CornerPos, WndRect) == false ) 
		{	continue; }

		if ( true == FdSelected )
		{	::SelectObject(hDC, hPenFdSel);	}
		else
		{	::SelectObject(hDC, hPenFd);	 }
		ImageAPI.DrawPolyLine(hDC, CornerPos, 4);
		str.Format(_T("Fd-%d"), FdPtr->GetFdIndex_Panel()+1);		
		::TextOut(hDC, Pt.x, Pt.y, str, str.GetLength());

		//Roi Extend Range;		
		FdPtr->GetFdRoiStageCornerPos(StgCornerPos);
		AOIDataCollect.MapStageCornerToCamera(ImageW, ImageH, ImageRes, StgCornerPos, ImageStagePos, ImgCornerPos);		
		MapImagePtToWndPt_DBL(ImageW, ImageH, WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[0], WndCornerPos[0]);
		MapImagePtToWndPt_DBL(ImageW, ImageH, WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[1], WndCornerPos[1]);
		MapImagePtToWndPt_DBL(ImageW, ImageH, WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[2], WndCornerPos[2]);
		MapImagePtToWndPt_DBL(ImageW, ImageH, WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[3], WndCornerPos[3]);

		JetAPI::CornerPt2DToCornerPt(WndCornerPos, CornerPos);
		if ( JetAPI::CheckCornerInRect(CornerPos, WndRect) == false ) 
		{	continue; }
		::SelectObject(hDC, hPenFdExt);	
		ImageAPI.DrawPolyLine(hDC, CornerPos, 4);
	}	
	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPenFd);    hPenFd = NULL;		
	::DeleteObject(hPenFdSel); hPenFdSel = NULL;	
	::DeleteObject(hPenFdExt); hPenFdExt = NULL;	

	bool ShowComponent = true;
	bool ShowComponentName = true;
	const bool CalibrationMode = CheckCalibrationMode();
	ShowComponent = AOIDataCollect.GetShowComponentList();
	ShowComponentName = ShowComponent;
	if ( true == ShowComponent )
	{
		CAOIModel     *ModelPtr = NULL;
		CAOIComponent *ComponentPtr = NULL;
		bool           ComponentSelected = false;
		TMODEL_DRAW_PARAM DrawParam;
		const COLORREF  clrCom1 = SystemParam.m_ComponentColor1;
		const COLORREF  clrCom2 = SystemParam.m_ComponentColor2;
	//TPOINT2D ComponentStagePos;
	//TPOINT2D StageOffset, CadOffset;
		HPEN hPenCom    = ::CreatePen(PS_SOLID, 1, clrCom1);	
		HPEN hPenComSel = ::CreatePen(PS_SOLID, 2, clrCom2);	
		hOldPen = (HPEN)(::SelectObject(hDC, hPenCom));	

		DrawParam.WndRect = m_ImageWndRect;
		DrawParam.ViewCP.x = DrawParam.ViewCP.y = 0;
		DrawParam.ResolutionX = ImageRes.x;
		DrawParam.ResolutionY = ImageRes.y;
		DrawParam.Scale = m_ImageZoom;	
		DrawParam.ViewOffsetX =  m_ImageOffset.x;
		DrawParam.ViewOffsetY =  -m_ImageOffset.y;	
		DrawParam.ShowEditLine = true;
		for ( i=0; i<ComponentCount; i++ )
		{	
			ComponentPtr = Project->GetProjectComponentPtr(i, false);
			if ( NULL == ComponentPtr ) { continue; }
			if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }
			ComponentSelected = ComponentPtr->GetComponentSelected();		
			if ( DRAW_COMPONENT_FOCUSED==DrawComponentMode && false==ComponentSelected ) { continue; }
			if ( true==ComponentSelected && DRAW_MODEL_RESULT==DrawModelMode )
			{
				ModelPtr = ComponentPtr->GetComponentModelPtr();
				if ( ModelPtr == m_ModelPtr )
				{	continue;	}			
			}

			StagePos.x = ComponentPtr->GetComponentStagePosX();
			StagePos.y = ComponentPtr->GetComponentStagePosY();
			if ( DRAW_MODEL_RESULT == DrawModelMode )
			{
				StageOffset.x = ComponentPtr->GetComponentStageOffsetX();
				StageOffset.y = ComponentPtr->GetComponentStageOffsetY();
				StagePos.x += StageOffset.x;
				StagePos.y += StageOffset.y;
			}
			if ( StagePos.x<ImageStageRgn.minX || StagePos.x>ImageStageRgn.maxX ) { continue; }
			if ( StagePos.y<ImageStageRgn.minY || StagePos.y>ImageStageRgn.maxY ) { continue; }
			AOIDataCollect.MapStagePtToCamera(ImageW, ImageH, ImageRes, StagePos, ImageStagePos, ImagePos);
			MapImagePtToWndPt_DBL(ImageW, ImageH, WndRect, m_ImageOffset, m_ImageZoom, ImagePos, WndPt);
			
			JetAPI::Point2DToPoint(WndPt, Pt);
			if ( Pt.x<0 || Pt.y<0 || Pt.x>WndRect.right || Pt.y>WndRect.bottom ) { continue; }
		
			ComponentPtr->GetComponentBodyStageCornerPos(StgCornerPos);		
			//ComponentPtr->GetComponentRoiStageCornerPos(StgCornerPos);
			if ( DRAW_MODEL_RESULT == DrawModelMode )
			{
				StgCornerPos[0].x += StageOffset.x;	StgCornerPos[0].y += StageOffset.y;
				StgCornerPos[1].x += StageOffset.x;	StgCornerPos[1].y += StageOffset.y;
				StgCornerPos[2].x += StageOffset.x;	StgCornerPos[2].y += StageOffset.y;
				StgCornerPos[3].x += StageOffset.x;	StgCornerPos[3].y += StageOffset.y;
			}
			AOIDataCollect.MapStageCornerToCamera(ImageW, ImageH, ImageRes, StgCornerPos, ImageStagePos, ImgCornerPos);		
			MapImagePtToWndPt_DBL(ImageW, ImageH, WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[0], WndCornerPos[0]);
			MapImagePtToWndPt_DBL(ImageW, ImageH, WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[1], WndCornerPos[1]);
			MapImagePtToWndPt_DBL(ImageW, ImageH, WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[2], WndCornerPos[2]);
			MapImagePtToWndPt_DBL(ImageW, ImageH, WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[3], WndCornerPos[3]);

			JetAPI::CornerPt2DToCornerPt(WndCornerPos, CornerPos);
			if ( JetAPI::CheckCornerInRect(CornerPos, WndRect) == false ) 
			{	continue; }

			if ( false == ComponentSelected )
			{	SelectObject(hDC, hPenCom); }
			else
			{	SelectObject(hDC, hPenComSel); }			
			ImageAPI.DrawPolyLine(hDC, CornerPos, 4);

			if ( ShowComponentName )
			{
				str = ComponentPtr->GetComponentName();
				//::TextOut(hDC, Rect.left, Rect.top, str, str.GetLength());
				::TextOut(hDC, Pt.x, Pt.y, str, str.GetLength());
			}
			if ( false == ComponentSelected )
			{
				ModelPtr = ComponentPtr->GetComponentModelPtr();
				if ( NULL != ModelPtr )
				{				
					ModelPtr->GetModelAttachedPosStage(ComponentStagePos);
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
					DrawParam.ViewCP.x = -JetAPI::Floor(ViewOffsetX);
					DrawParam.ViewCP.y = -JetAPI::Floor(ViewOffsetY);
				
					DrawParam.ShowWndBox = false;
					DrawParam.ShowEditLine = false;
					ModelPtr->DrawModel(hDC, DrawModelMode, DrawParam);
				}
			}		
		}
		::SelectObject(hDC, hOldPen);
		::DeleteObject(hPenCom); hPenCom = NULL;	
		::DeleteObject(hPenComSel); hPenComSel = NULL;			
	}	
	::SetBkMode(hDC, BKMode);
	::SetTextColor(hDC, clrTextOld);	
}
//-------------------------------------------------------------------------------------//
void CEditFdView::DrawModel(HDC hDC)
{
	const bool CalibrationMode = CheckCalibrationMode();
	if ( true == CalibrationMode ) { return; }

	CAOIModel *ModelPtr = CEditFdView::GetModelPtr();
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
void CEditFdView::DrawImage(HDC hDC)
{
	if ( NULL == m_ShowImagePtr ) { return; }
	const int BltMode = AOIDataCollect.GetStretchBltMode(m_ImageZoom);
	if ( ImageAPI.DrawImageToDC(hDC, m_ShowImageW, m_ShowImageH, m_ShowImageStep, m_ShowBitCount, m_ShowImagePtr, m_ImageWndRect, m_ImageOffset, m_ImageZoom, 0x00000, BltMode) == false )
	{	return ; }
}
//-------------------------------------------------------------------------------------//
void CEditFdView::DrawProjectMap(HDC hDC, bool bTestMap)
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
void CEditFdView::DrawAddRect(HDC hDC)
{
	if ( false == m_DrawAddRect ) { return; }
	if ( CURSOR_POS_NONE != m_MousePosMode ) { return; }
	//CAOIModel *ModelPtr = CEditFdView::GetModelPtr();
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
void CEditFdView::DrawEditRect(HDC hDC)
{
	if ( false == m_DrawEditRect ) { return; }
	//if ( CURSOR_POS_NONE != m_MousePosMode ) { return; }
	
	RECT             EditRect;	
	TREGION4D        WndUIRgn;
	TREGION4D        StageRgn;
	TREGION4D        CameraRgn;
	const RECT       WndRect = m_ImageWndRect;
	const IMAGE_SIZE ImageW = GetImageW();
	const IMAGE_SIZE ImageH = GetImageH();
	const TPOINT2D   StageCp = GetImageStageRgnCp();
	const TPOINT2D  &ImageRes = GetImageResolution();			
	const int HalfSize  = CEditFdView::GetEditLineSize();
	HPEN    hPen    = ::CreatePen(PS_SOLID, 2, 0x0000FF);
	HPEN    hOldPen = (HPEN)::SelectObject(hDC, hPen);
	HBRUSH  hBrush = ::CreateSolidBrush(0x0000FF);
	HBRUSH  hOldBrush = (HBRUSH)(::SelectObject(hDC, hBrush));

	StageRgn = m_EditRegion;
	AOIDataCollect.MapStageRegionToCamera(ImageW, ImageH, ImageRes, StageRgn, StageCp, CameraRgn);
	MapImageRgnToWndRgn_DBL(ImageW, ImageH, WndRect, m_ImageOffset, m_ImageZoom, CameraRgn, WndUIRgn);
	JetAPI::Region4DToRect(WndUIRgn, EditRect, true);
	CAOIBox::DrawEditRect(hDC, EditRect, HalfSize);//繪製選取編輯框	

	::SelectObject(hDC, hOldBrush);
	::DeleteObject(hBrush);	hBrush = NULL;
	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPen);	hPen = NULL;
}
//-------------------------------------------------------------------------------------//
void CEditFdView::DrawBoxInfo(HDC hDC)
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
void CEditFdView::DrawCrosshair(HDC hDC)//十字線
{
	const POINT &Pt=m_MousePosCurrent;	
	const RECT &WndRect=m_ImageWndRect;
	MANIPULATE_MODEL_MODE ManiMode=AOIDataCollect.GetManipulateModelMode();
	if ( MANIPULATE_MODEL_ADD != ManiMode ) { return ; }
	ImageAPI.DrawCrosshair(hDC, Pt, WndRect, 0xA0A0A0);	
	return;
}
//-------------------------------------------------------------------------------------//
void CEditFdView::DrawObjectList(HDC hDC)
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
void CEditFdView::DrawModelActivedLine(HDC hDC)//繪製選取交線
{	
	BOOL bShowLine = AOIDataCollect.GetDrawModelActivedLine();
	if ( FALSE == bShowLine ) { return ; }
	CAOIProject *Project = GetActiveProject();
	if ( NULL == Project ) { return; }
	CAOIFd *FdPtr = Project->GetProjectActiveFd();	
	if ( NULL == FdPtr ) { return ; }
	//if ( ComponentPtr->GetComponentDeleted() == true ) { return; }
	CAOIModel   *ModelPtr = FdPtr->GetFdModelPtr();
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
	const double AttachedAngle = FdPtr->GetFdAngle();
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(AttachedAngle);
	CAOIWnd  *WndPtr  = ModelPtr->GetModelWndActived();
	CAOILand *LandPtr = ModelPtr->GetModelLandActivted();

	CAOIBox  *BoxPtr = NULL;
	if ( NULL != WndPtr )
	{	BoxPtr = WndPtr->GetWndBoxPtr(); }
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
int CEditFdView::GetEditLineSize()//取得編輯線的尺寸
{
	const int    LineSizeLevel = AOIDataCollect.GetSystemParameter().m_EditLineSizeLevel;
	return JetAPI::GetEditLineSize(m_ImageZoom, LineSizeLevel);	
}
//-------------------------------------------------------------------------------------//
int CEditFdView::GetEditCheckSize()//取得編輯線比較的尺寸
{
	const int    LineSizeLevel = AOIDataCollect.GetSystemParameter().m_EditLineSizeLevel;
	return JetAPI::GetEditCheckSize(m_ImageZoom, LineSizeLevel);	

	const int szImage = GetEditLineSize();	
	const TPOINT2D &ImageRes = GetImageResolution();
	const int szCamera = (int)(szImage*m_ImageZoom);	
	const int szStage = (int)(szCamera*ImageRes.x);	
	return szStage;
}
//-------------------------------------------------------------------------------------//
int CEditFdView::GetEditCheckStageSize()//取得編輯線比較的尺寸
{
	const int szImage = GetEditLineSize();	
	const TPOINT2D &ImageRes = GetImageResolution();
	const int szCamera = (int)(szImage*m_ImageZoom);	
	const int szStage = (int)(szCamera*ImageRes.x);	
	return szStage;
}
//-------------------------------------------------------------------------------------//
void CEditFdView::SetDrawAddRect(bool Draw)
{
	if ( false == Draw )
	{	Draw = Draw;	}
	else
	{	Draw = Draw; }
	m_DrawAddRect = Draw;
}
//-------------------------------------------------------------------------------------//
void CEditFdView::SetDrawEditRect(bool Draw)
{	
	m_DrawEditRect = Draw;
}
//-------------------------------------------------------------------------------------//
DRAW_MODEL_MODE CEditFdView::GetDrawModelMode() const
{
	return DRAW_MODEL_EDIT;
	return AOIDataCollect.GetDrawModelMode();	
}
//-------------------------------------------------------------------------------------//
bool CEditFdView::ExecAddFd()
{
	CAOIFd      *FdPtr = NULL;	
	CAOIModel   *ModelPtr = NULL;
	CAOIBoard   *BoardPtr = NULL;	
	CAOIPanel   *PanelPtr = NULL;	
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }
	PanelPtr = ProjectPtr->GetProjectActivePanel();
	if ( NULL != PanelPtr ) 
	{	BoardPtr = ProjectPtr->GetProjectActiveBoard();	}
	else
	{
		PanelPtr = ProjectPtr->GetProjectPanelPtr(0, true);
		if ( NULL == PanelPtr )
		{	return false;  }
	}

	size_t       i=0;
	CString      str;
	CString      strName;
	CString      strValue;
	CString      strCaption;
	CInputBoxWnd InputBox;
	const size_t PanelFdCount = PanelPtr->GetPanelFdCount();
	const size_t PanelBoardCount = PanelPtr->GetPanelBoardCount();
	const DISTRICT_ID DistrictID = ProjectPtr->GetProjectActDistrictID();

	str = _T("Set Board Index");
	str = LoadMultiLanguageString(str, str);
	strCaption.Format(_T("%s [Panel:%d]"), str, PanelPtr->GetPanelIndex_Project()+1);
	str = _T("Board Index");
	str = LoadMultiLanguageString(str, str);
	strName.Format(_T("%s (0 ~ %d) [0:Panel Fiducial]:"), str, PanelBoardCount);

	if ( PanelFdCount < 2 )
	{	strValue = _T("0"); }
	else
	{
		if ( NULL != BoardPtr )
		{	strValue.Format(_T("%d"), BoardPtr->GetBoardIndex_Panel()+1);	}
		else
		{
			for ( i=0; i<PanelBoardCount; i++ )
			{
				BoardPtr = PanelPtr->GetPanelBoardPtr(i, false);
				if ( NULL == BoardPtr ) { continue; }
				if ( BoardPtr->GetBoardFdCount() < 2 ) 
				{	break; }
			}
			if ( i == PanelBoardCount )
			{	strValue = _T("0");  }
			else
			{	strValue.Format(_T("%d"), i+1);	}
		}
	}
	InputBox.SetParam1(strCaption, strName, strValue);
	if ( InputBox.DoModal() == IDCANCEL ) 
	{	return false;  }
	unsigned int PanelBoardIndex = ::_ttoi(InputBox.m_DataEdit1)-1;
	BoardPtr = PanelPtr->GetPanelBoardPtr(PanelBoardIndex, true);
	
	if ( NULL == BoardPtr )
	{
		//check Fiducial Count
		const size_t PanelFdCount = PanelPtr->CalcPanelFdCount(DistrictID);		
		if ( PanelFdCount >= PANEL_MAX_FD_COUNT )
		{
			str.Format(_T("Error, there are more than %d fiducials int the panel[%d]"), PANEL_MAX_FD_COUNT, PanelPtr->GetPanelIndex_Project()+1);
			JetAPI::ShowMessageBox(str);
			return false;
		}
	}
	else 
	{
		const size_t BoardFdCount = BoardPtr->CalcBoardFdCount(DistrictID);
		if ( BoardFdCount >= BOARD_MAX_FD_COUNT )
		{
			str.Format(_T("Error, there are more than %d fiducials int the board[%d]"), BOARD_MAX_FD_COUNT, BoardPtr->GetBoardIndex_Panel()+1);
			JetAPI::ShowMessageBox(str);
			return false;
		}
	}
	
	TREGION4D RgnWndBox;
	TREGION4D RgnImgBox;
	TREGION4D RgnStgBox;
	TREGION4D RgnCadBox;
	double    CadPosX=0, CadPosY=0;
	POINT pt1 = m_MousePosLast;
	POINT pt2 = m_MousePosFirst;	
	
	CString FdFolder;
	CString FdModelFolder;
	const IMAGE_SIZE ImageW = GetImageW();
	const IMAGE_SIZE ImageH = GetImageH();	
	const TPOINT2D   StageCp = GetImageStageRgnCp();
	const TPOINT2D  &ImageRes = GetImageResolution();
	const double BarcodeAngle = 0;
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(BarcodeAngle);
	PtInControlWnd(m_MousePosLast, MODELEDIT_IMAGE_WND, pt1);
	PtInControlWnd(m_MousePosFirst, MODELEDIT_IMAGE_WND, pt2);	

	const bool   LinkMode = true;
	const MODEL_TYPE    ModelType   = MODEL_TYPE_FD;
	const ALG_TYPE      AlgType     = ALG_FD_MATCH;	
	const WND_DEFECT_ID WndDefectID = WND_DEFECT_PAD_ALIGN;	
	const double ExtanedXum = 2000.0;
	const double ExtanedYum = 2000.0;
	const int    FdGroupID = ProjectPtr->GetProjectFdFreeGroupID();	
	unsigned int FrameIndex = ProjectPtr->GetProjectMapIndex();
	unsigned int FrameUniqueID = ProjectPtr->GetProjectFrameUniqueID(FrameIndex, true);
	TFrameParam *FrameParamPtr = AOIDataCollect.GetSystemFrameParamPtrByUniqueID(FrameUniqueID);
	if ( NULL == FrameParamPtr ) 
	{
		str.Format(_T("Error, Search Frame Unique ID Fault [%d]"), FrameUniqueID);
		JetAPI::ShowMessageBox(str);
		return false; 
	}

	RgnWndBox.minX = MIN(pt1.x, pt2.x);
	RgnWndBox.maxX = MAX(pt1.x, pt2.x);
	RgnWndBox.minY = MIN(pt1.y, pt2.y);
	RgnWndBox.maxY = MAX(pt1.y, pt2.y);
	ImageAPI.MapWndRgnToImageRgn_DBL(ImageW, ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, RgnWndBox, RgnImgBox);	
	AOIDataCollect.MapCameraRegionToStage(ImageW, ImageH, ImageRes, RgnImgBox, StageCp, RgnStgBox);
	AOIDataCollect.MapStageOffsetRgnToCad(RgnStgBox, RgnCadBox);

	FdPtr = AOIObjManager.CreateFdObj();
	if ( NULL == FdPtr ) { return false; }	
	FdPtr->SetFdGroupID(FdGroupID);
	FdPtr->SetFdResultID_AOI(RESULT_ID_OK);
	ProjectPtr->SelectProjectAllFds(false);
	ProjectPtr->AddProjectFdPtr(FdPtr, false);
	PanelPtr->AddPanelFdPtr(FdPtr);
	if ( NULL != BoardPtr )
	{	BoardPtr->AddBoardFdPtr(FdPtr); }
	LogOperCtrl.SaveLogProjectFdAdd(FdPtr);

	CMapCoordinate *STCPtr = NULL;
	if ( NULL != BoardPtr )
	{	STCPtr = BoardPtr->GetBoardMapSTCPtr(DistrictID); }
	else
	{	STCPtr = PanelPtr->GetPanelMapSTCPtr(DistrictID); }
	
	TSIZE2D   szFd, szRoi;	
	TPOINT2D  CadPos, StagePos;
	StagePos.x = RgnStgBox.GetCpX();
	StagePos.y = RgnStgBox.GetCpY();
	szFd.cx   = RgnCadBox.GetWidth();
	szFd.cy   = RgnCadBox.GetHeight();
	szRoi.cx = szFd.cx + (ExtanedXum+ExtanedXum);
	szRoi.cy = szFd.cy + (ExtanedYum+ExtanedYum);
	if ( NULL != STCPtr )
	{	STCPtr->Map2D(StagePos.x, StagePos.y, CadPos.x, CadPos.y); }

	const int FdUniqueID = FdPtr->GetFdUniqueID();
	FdFolder = ProjectPtr->GetProjectFdFolder();
	JetAPI::CreateFolder(FdFolder);
	FdModelFolder = AOIDataDefine.GetFdModelFolder(FdFolder, FdUniqueID);
	TUNI_FRAME UniFrame;
	FRAME_TYPE FrameTYpe = FrameParamPtr->FrameType;
	JetAPI::InitialUniFrame(UniFrame);
	if ( FdPtr->BuildNewFd(CadPos, StagePos, szFd, szRoi, FrameIndex, FrameUniqueID, FrameTYpe, FdModelFolder, UniFrame) == false )
	{	return false; }
	
	ModelPtr = FdPtr->GetFdModelPtr();
	MANIPULATE_MODEL_MODE ManiMode = GetManipulateModelModeDefault();
	AOIDataCollect.SetManipulateModelMode(ManiMode);	
	SetModel(ModelPtr);
	UpdateFdModelStats();
	RedrawWnd();

	ProjectPtr->ResetProjectActiveIndex();
	const unsigned int FdIndex = FdPtr->GetFdIndex_Project();
	const unsigned int PanelIndex = FdPtr->GetFdPanelIndex_Project();
	const unsigned int BoardIndex = FdPtr->GetFdBoardIndex_Project();	
	ProjectPtr->SetProjectActiveFdIndex(FdIndex);
	ProjectPtr->SetProjectActivePanelIndex(PanelIndex);
	ProjectPtr->SetProjectActiveBoardIndex(BoardIndex);	

	CAOIWnd *WndPtr = ModelPtr->GetModelWndActived();
	ProjectPtr->SetProjectActiveModelWnd(WndPtr);
	LogOperCtrl.SaveLogModelWndSelectedCreate(ModelPtr);
	SendMessageToMainFrameWnd(MSG_EDIT_PART_LIST_WND, WPARAM_BUILD_PART_LIST, LPARAM_BUILD_DOCK_LIST_PART);
	SendMessageToMainFrameWnd(MSG_EDIT_PART_LIST_WND, WPARAM_UPDATE_PART_SELECTED, NULL);
		
	str.Format(_T("Do you want to Add Fd Pattern?"));
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDYES )
	{	SendMessageToMainFrameWnd(MSG_EDIT_IMAGE_PROCESS_WND, WPARAM_PATTERN_ADD, NULL);	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditFdView::ExecAddFd2()
{
	CAOIFd      *FdPtr = NULL;
	CAOIBox     *BoxPtr = NULL;
	CAOIModel   *ModelPtr = NULL;
	CAOIBoard   *BoardPtr = NULL;	
	CAOIPanel   *PanelPtr = NULL;	
	CAOIProject *ProjectPtr = CEditFdView::GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }
	PanelPtr = ProjectPtr->GetProjectActivePanel();
	if ( NULL != PanelPtr ) 
	{	BoardPtr = ProjectPtr->GetProjectActiveBoard();	}
	else
	{
		PanelPtr = ProjectPtr->GetProjectPanelPtr(0, true);
		if ( NULL == PanelPtr )
		{	return false;  }
	}

	size_t       i=0;
	CString      str;
	CString      strName;
	CString      strValue;
	CString      strCaption;
	CInputBoxWnd InputBox;
	const size_t PanelFdCount = PanelPtr->GetPanelFdCount();
	const size_t PanelBoardCount = PanelPtr->GetPanelBoardCount();
	const DISTRICT_ID DistrictID = ProjectPtr->GetProjectActDistrictID();

	str = _T("Set Board Index");
	str = LoadMultiLanguageString(str, str);
	strCaption.Format(_T("%s [Panel:%d]"), str, PanelPtr->GetPanelIndex_Project()+1);
	str = _T("Board Index");
	str = LoadMultiLanguageString(str, str);
	strName.Format(_T("%s (0 ~ %d) [0:Panel Fiducial]:"), str, PanelBoardCount);

	if ( PanelFdCount < 2 )
	{	strValue = _T("0"); }
	else
	{
		if ( NULL != BoardPtr )
		{	strValue.Format(_T("%d"), BoardPtr->GetBoardIndex_Panel()+1);	}
		else
		{
			for ( i=0; i<PanelBoardCount; i++ )
			{
				BoardPtr = PanelPtr->GetPanelBoardPtr(i, false);
				if ( NULL == BoardPtr ) { continue; }
				if ( BoardPtr->GetBoardFdCount() < 2 ) 
				{	break; }
			}
			if ( i == PanelBoardCount )
			{	strValue = _T("0");  }
			else
			{	strValue.Format(_T("%d"), i+1);	}
		}
	}
	InputBox.SetParam1(strCaption, strName, strValue);
	if ( InputBox.DoModal() == IDCANCEL ) 
	{	return false;  }
	unsigned int PanelBoardIndex = ::_ttoi(InputBox.m_DataEdit1)-1;
	BoardPtr = PanelPtr->GetPanelBoardPtr(PanelBoardIndex, true);
	
	if ( NULL == BoardPtr )
	{
		//check Fiducial Count
		const size_t PanelFdCount = PanelPtr->CalcPanelFdCount(DistrictID);		
		if ( PanelFdCount >= PANEL_MAX_FD_COUNT )
		{
			str.Format(_T("Error, there are more than %d fiducials int the panel[%d]"), PANEL_MAX_FD_COUNT, PanelPtr->GetPanelIndex_Project()+1);
			JetAPI::ShowMessageBox(str);
			return false;
		}
	}
	else 
	{
		const size_t BoardFdCount = BoardPtr->CalcBoardFdCount(DistrictID);
		if ( BoardFdCount >= BOARD_MAX_FD_COUNT )
		{
			str.Format(_T("Error, there are more than %d fiducials int the board[%d]"), BOARD_MAX_FD_COUNT, BoardPtr->GetBoardIndex_Panel()+1);
			JetAPI::ShowMessageBox(str);
			return false;
		}
	}	
	
	TSIZE2D   ModelSize;
	TPOINT2D  ModelStageCp;
	TPOINT2D  StageCp_LA;
	TREGION4D RgnWndBox;
	TREGION4D RgnImgBox;
	TREGION4D RgnStgBox;
	TREGION4D RgnCadBox;
	double    CadPosX=0, CadPosY=0;
	POINT pt1 = m_MousePosLast;
	POINT pt2 = m_MousePosFirst;	
	
	CString FdFolder;
	CString FdModelName;
	CString FdModelFolder;
	const IMAGE_SIZE ImageW = GetImageW();
	const IMAGE_SIZE ImageH = GetImageH();	
	const TPOINT2D   StageCp = GetImageStageRgnCp();
	const TPOINT2D  &ImageRes = GetImageResolution();
	const double BarcodeAngle = 0;
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(BarcodeAngle);
	PtInControlWnd(m_MousePosLast, MODELEDIT_IMAGE_WND, pt1);
	PtInControlWnd(m_MousePosFirst, MODELEDIT_IMAGE_WND, pt2);	

	const bool   LinkMode = true;
	const MODEL_TYPE    ModelType   = MODEL_TYPE_FD;
	const ALG_TYPE      AlgType     = ALG_FD_MATCH;	
	const WND_DEFECT_ID WndDefectID = WND_DEFECT_PAD_ALIGN;	
	const double ExtanedXum = 2000.0;
	const double ExtanedYum = 2000.0;
	const LANE_ID LaneID = AOIDataCollect.GetActiveLaneID();
	const int    FdGroupID = ProjectPtr->GetProjectFdFreeGroupID();		

	RgnWndBox.minX = MIN(pt1.x, pt2.x);
	RgnWndBox.maxX = MAX(pt1.x, pt2.x);
	RgnWndBox.minY = MIN(pt1.y, pt2.y);
	RgnWndBox.maxY = MAX(pt1.y, pt2.y);
	ImageAPI.MapWndRgnToImageRgn_DBL(ImageW, ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, RgnWndBox, RgnImgBox);	
	AOIDataCollect.MapCameraRegionToStage(ImageW, ImageH, ImageRes, RgnImgBox, StageCp, RgnStgBox);
	AOIDataCollect.MapStageOffsetRgnToCad(RgnStgBox, RgnCadBox);

	FdPtr = AOIObjManager.CreateFdObj();
	if ( NULL == FdPtr ) { return false; }	
	ProjectPtr->SelectProjectAllFds(false);
	ProjectPtr->AddProjectFdPtr(FdPtr, false);
	PanelPtr->AddPanelFdPtr(FdPtr);
	if ( NULL != BoardPtr )
	{	BoardPtr->AddBoardFdPtr(FdPtr); }
	LogOperCtrl.SaveLogProjectFdAdd(FdPtr);

	ModelPtr = FdPtr->GetFdModelPtr();
	ModelStageCp.x = RgnStgBox.GetCpX();
	ModelStageCp.y = RgnStgBox.GetCpY();
	ModelSize.cx   = RgnCadBox.GetWidth();
	ModelSize.cy   = RgnCadBox.GetHeight();	
	
	CMapCoordinate *STCPtr = NULL;
	if ( NULL != BoardPtr )
	{	STCPtr = BoardPtr->GetBoardMapSTCPtr(DistrictID); }
	else
	{	STCPtr = PanelPtr->GetPanelMapSTCPtr(DistrictID); }
	if ( NULL != STCPtr )
	{	STCPtr->Map2D(ModelStageCp.x, ModelStageCp.y, CadPosX, CadPosY); }
	
	FdPtr->SetFdLaneID(LaneID);
	FdPtr->SetFdGroupID(FdGroupID);
	FdPtr->SetFdSelected(true);
	FdPtr->SetFdAngle(0);	
	FdPtr->SetFdCadPosX(CadPosX);
	FdPtr->SetFdCadPosY(CadPosY);
	FdPtr->SetFdBodySizeW(ModelSize.cx);
	FdPtr->SetFdBodySizeH(ModelSize.cy);
	FdPtr->SetFdStagePosX(ModelStageCp.x);
	FdPtr->SetFdStagePosY(ModelStageCp.y);
	FdPtr->SetFdResultID_AOI(RESULT_ID_OK);
	StageCp_LA = ModelStageCp;
	AOIDataCollect.MapStagePosToLaneA(StageCp_LA.x, StageCp_LA.y, LaneID);
	FdPtr->SetFdTeachStagePosX(StageCp_LA.x);
	FdPtr->SetFdTeachStagePosY(StageCp_LA.y);	
	FdPtr->SetFdRoiExtendSizeW(ExtanedXum*2);
	FdPtr->SetFdRoiExtendSizeH(ExtanedYum*2);
	FdPtr->CalcFdCadCornerPos();//計算軟體條碼Cad端點座標		
	FdPtr->LayoutFdStageCornerPos();//更新軟體條碼機台端點座標	

	const int FdUniqueID = FdPtr->GetFdUniqueID();
	FdFolder = ProjectPtr->GetProjectFdFolder();
	JetAPI::CreateFolder(FdFolder);
	FdModelFolder = AOIDataDefine.GetFdModelFolder(FdFolder, FdUniqueID);
	JetAPI::CreateFolder(FdModelFolder);
	JetAPI::ExtractTopFolder(FdModelFolder, FdModelName);
	ModelPtr->SetModelName(FdModelName);
	ModelPtr->SetModelFolderModel(FdModelFolder);

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
	WndPtr->SetWndExtendRangeX(ExtanedXum);
	WndPtr->SetWndExtendRangeY(ExtanedYum);
	WndPtr->UpdateWndExtendBox();
	const bool WndRgnLinkAuto = WndPtr->GetWndRgnLinkAuto();

	ModelPtr->UnSelectModel();	
	ModelPtr->AddModelWndPtr(WndPtr, false);	
	if ( true == WndRgnLinkAuto )
	{	ModelPtr->UpdateModelWndRgnByLinkMode();	}

	ModelPtr->CalcModelTotalRegionAll();
	ModelPtr->UpdateModelBodyToFd();

	WndPtr->SetWndSelected(true);
	ModelPtr->SetModelWndActived(WndPtr);		
	WndPtr = ModelPtr->GetModelWndActived();
	MANIPULATE_MODEL_MODE ManiMode = GetManipulateModelModeDefault();
	AOIDataCollect.SetManipulateModelMode(ManiMode);	
	SetModel(ModelPtr);
	UpdateFdModelStats();
	RedrawWnd();

	ProjectPtr->ResetProjectActiveIndex();
	const unsigned int FdIndex = FdPtr->GetFdIndex_Project();
	const unsigned int PanelIndex = FdPtr->GetFdPanelIndex_Project();
	const unsigned int BoardIndex = FdPtr->GetFdBoardIndex_Project();	
	ProjectPtr->SetProjectActiveFdIndex(FdIndex);
	ProjectPtr->SetProjectActivePanelIndex(PanelIndex);
	ProjectPtr->SetProjectActiveBoardIndex(BoardIndex);	
	ProjectPtr->SetProjectActiveModelWnd(WndPtr);
	LogOperCtrl.SaveLogModelWndSelectedCreate(ModelPtr);

	SendMessageToMainFrameWnd(MSG_EDIT_PART_LIST_WND, WPARAM_BUILD_PART_LIST, LPARAM_BUILD_DOCK_LIST_PART);
	SendMessageToMainFrameWnd(MSG_EDIT_PART_LIST_WND, WPARAM_UPDATE_PART_SELECTED, NULL);

	str.Format(_T("Do you want to Add Fd Pattern?"));
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDYES )
	{	SendMessageToMainFrameWnd(MSG_EDIT_IMAGE_PROCESS_WND, WPARAM_PATTERN_ADD, NULL);	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditFdView::ResetFdModel()
{
	m_ActiveObjList.clear();	
	SetModel(NULL);
	MANIPULATE_MODEL_MODE ManiMode = GetManipulateModelModeDefault();
	AOIDataCollect.SetManipulateModelMode(ManiMode);
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditFdView::SetModel(CAOIModel *Ptr)
{
	m_ModelPtr = Ptr;
}
//-------------------------------------------------------------------------------------//
void CEditFdView::UpdateFdModelStats()
{
	CAOIModel *ModelPtr = CEditFdView::GetModelPtr();	
	BuildActiveObjList(ModelPtr, false);
	if ( NULL != ModelPtr )
	{		
		const int nAlign = 4;
		const bool bClone=false;
		const bool bNoFilter=false;		
		std::vector<TUNI_FRAME> UniFrameList;
		BuildModelUniFrameList(ModelPtr, UniFrameList, nAlign, bClone, bNoFilter);
		if ( true == bClone )
		{	JetAPI::ClearUniFrameList(UniFrameList); }
	}
}
//-------------------------------------------------------------------------------------//
void CEditFdView::SendOutUpdatePartList(int UpdateList, int UpdateWnd)
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
bool CEditFdView::GetShowPopupMenu() const
{
	return m_ShowPopupMenu;
}
//-------------------------------------------------------------------------------------//
void CEditFdView::SetShowPopupMenu(bool val)
{
	m_ShowPopupMenu = val;
}
//-------------------------------------------------------------------------------------//
bool CEditFdView::ExecSelectFd()
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
	
	CAOIFd *FdPtr = NULL;
	std::vector<CAOIFd*> FdList;
	Project->SelectProjectFdsByStage(Rgn, bResultMode, FdList);
	const size_t SelCount = FdList.size();
	if ( 0 == SelCount ) { return false; }		
	FdPtr = FdList[0];
	
	FdPtr->SetFdSelected(true);
	const unsigned int FdIndex = FdPtr->GetFdIndex_Project();		
	const unsigned int PanelIndex = FdPtr->GetFdPanelIndex_Project();
	const unsigned int BoardIndex = FdPtr->GetFdBoardIndex_Project();	
	Project->SetProjectActiveFdIndex(FdIndex);	
	Project->SetProjectActivePanelIndex(PanelIndex);	
	Project->SetProjectActiveBoardIndex(BoardIndex);		

	CAOIModel *ModelPtr = FdPtr->GetFdModelPtr();
	SetModel(ModelPtr);
	UpdateFdModelStats();	
	RedrawWnd();

	PostMessageToMainFrameWnd(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_UPDATE, (LPARAM)(this));	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditFdView::ExecMoveSelectedFd()
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }

	const size_t NObjects = m_SelFdList.size();	
	if ( 0 == NObjects ) { return false; }	
	
	const int nWndPx = m_MousePosCurrent.x-m_MousePosLast.x;
	const int nWndPy = m_MousePosCurrent.y-m_MousePosLast.y;
	if ( (0==nWndPx) && (0==nWndPy) )
	{	return false; }

	size_t          i = 0;		
	TPOINT2D        ImgPos;
	TPOINT2D        CadPos;
	CAOIFd         *FdPtr = NULL;
	CAOIPanel      *PanelPtr = NULL;
	CAOIBoard      *BoardPtr = NULL;	
	CMapCoordinate *MapCTSPtr = NULL;	
	TFdRect        *FdRectPtr = NULL;		
	const TPOINT2D &ImageRes = GetImageResolution();
	const DISTRICT_ID  DistrictID = ProjectPtr->GetProjectActDistrictID();

	ImgPos.x = nWndPx*m_ImageZoom;
	ImgPos.y = nWndPy*m_ImageZoom;	
	AOIDataCollect.MapImageOffsetToCad(ImgPos, ImageRes, CadPos);	
	for ( i=0; i<NObjects; i++ )
	{
		FdRectPtr = &(m_SelFdList[i]);		
		if ( NULL == FdRectPtr ) { continue; }
		FdPtr = FdRectPtr->FdPtr;
		if ( NULL == FdPtr ) { continue; }

		MapCTSPtr = NULL;		
		PanelPtr = FdPtr->GetFdPanelPtr();
		BoardPtr = FdPtr->GetFdBoardPtr();
		if ( NULL==MapCTSPtr && NULL!=BoardPtr ) 
		{	MapCTSPtr = BoardPtr->GetBoardMapCTSPtr(DistrictID);	}
		if ( NULL==MapCTSPtr && NULL!=PanelPtr ) 
		{	MapCTSPtr = PanelPtr->GetPanelMapCTSPtr(DistrictID);	}
		FdPtr->MoveFdPos(CadPos.x, CadPos.y, MapCTSPtr);
	}	
	m_ModifyFdPos = true;	

	BuildFdSelected();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditFdView::ExecResizeSelectedFd(CURSOR_POS_MODE CursorMode)
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }

	const size_t NObjects = m_SelFdList.size();	
	if ( 0 == NObjects ) { return false; }	
	
	const int nWndPx = m_MousePosCurrent.x-m_MousePosLast.x;
	const int nWndPy = m_MousePosCurrent.y-m_MousePosLast.y;
	if ( (0==nWndPx) && (0==nWndPy) )
	{	return false; }

	const double dWndPx = nWndPx;
	const double dWndPy = nWndPy;
	const double dImagePx = dWndPx*m_ImageZoom;
	const double dImagePy = dWndPy*m_ImageZoom;
	const TPOINT2D &ImageRes = GetImageResolution();
	const double ResX = ImageRes.x;
	const double ResY = ImageRes.y;
	const bool   DoubleSideEdit = AOIDataCollect.CheckDoubleSideEdit();	

	size_t          i = 0;	
	TREGION4D       dRgn;
	TPOINT2D        ImgPos;
	TPOINT2D        CadPos;
	CAOIFd         *FdPtr = NULL;
	TFdRect        *FdRectPtr = NULL;		

	const double dCadPx = dImagePx*ResX;
	const double dCadPy = -1*dImagePy*ResY;//Y軸反向	
	JetAPI::CalcModifySizeRegion(CursorMode, DoubleSideEdit, dCadPx, dCadPy, dRgn);	

	for ( i=0; i<NObjects; i++ )
	{
		FdRectPtr = &(m_SelFdList[i]);		
		if ( NULL == FdRectPtr ) { continue; }
		FdPtr = FdRectPtr->FdPtr;
		if ( NULL == FdPtr ) { continue; }
		FdPtr->ModifyFdBodyRegion(dRgn);		
	}	
	m_ModifyFdPos = true;	

	BuildFdSelected();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditFdView::ExecModifySelectedFdFinish()
{
	if ( false == m_ModifyFdPos ) { return true; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }		
	ProjectPtr->LayoutProjectRegion();	
	m_ModifyFdPos = false;	
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CEditFdView::ExecMoveSelectedEdit()
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }
	if ( false == m_DrawEditRect ) { return false; }
	
	const int nWndPx = m_MousePosCurrent.x-m_MousePosLast.x;
	const int nWndPy = m_MousePosCurrent.y-m_MousePosLast.y;
	if ( (0==nWndPx) && (0==nWndPy) )
	{	return false; }

	size_t          i = 0;		
	TPOINT2D        ImgPos;
	TPOINT2D        CadPos;
	TPOINT2D        StagePos;
	CAOIFd         *FdPtr = NULL;
	TFdRect        *FdRectPtr = NULL;			
	const TPOINT2D &ImageRes = GetImageResolution();

	ImgPos.x = nWndPx*m_ImageZoom;
	ImgPos.y = nWndPy*m_ImageZoom;	
	AOIDataCollect.MapImageOffsetToCad(ImgPos, ImageRes, CadPos);
	AOIDataCollect.MapCadOffsetPtToStage(CadPos, StagePos);

	this->m_EditRegion.minX += StagePos.x;
	this->m_EditRegion.minY += StagePos.y;
	this->m_EditRegion.maxX += StagePos.x;
	this->m_EditRegion.maxY += StagePos.y;
	switch ( m_CaliIndex )
	{
	case 0: 
	case 1: 
	case 2: 
	case 3: 
		m_CaliComponentRegion[m_CaliIndex] = m_EditRegion; 
		break;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditFdView::ExecResizeSelectedEdit(CURSOR_POS_MODE CursorMode)
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }
	if ( false == m_DrawEditRect ) { return false; }
	
	const int nWndPx = m_MousePosCurrent.x-m_MousePosLast.x;
	const int nWndPy = m_MousePosCurrent.y-m_MousePosLast.y;
	if ( (0==nWndPx) && (0==nWndPy) )
	{	return false; }

	const double dWndPx = nWndPx;
	const double dWndPy = nWndPy;
	const double dImagePx = dWndPx*m_ImageZoom;
	const double dImagePy = dWndPy*m_ImageZoom;
	const TPOINT2D &ImageRes = GetImageResolution();
	const double ResX = ImageRes.x;
	const double ResY = ImageRes.y;
	const bool   DoubleSideEdit = AOIDataCollect.CheckDoubleSideEdit();	

	size_t          i = 0;	
	TREGION4D       dRgn;
	TPOINT2D        ImgPos;
	TPOINT2D        CadPos;
	TPOINT2D        CadPosDif;
	TPOINT2D        StagePosDif;
	CAOIFd         *FdPtr = NULL;
	TFdRect        *FdRectPtr = NULL;		
	const bool SignX = AOIDataCollect.GetStageSignPositiveX();
	const bool SignY = AOIDataCollect.GetStageSignPositiveY();

	CadPosDif.x = dImagePx*ResX;
	CadPosDif.y = -1*dImagePy*ResY;//Y軸反向
	AOIDataCollect.MapCadOffsetPtToStage(CadPosDif, StagePosDif);
	JetAPI::CalcModifySizeRegion(CursorMode, DoubleSideEdit, CadPosDif.x, CadPosDif.y, dRgn);	
	
	if ( true == SignX )
	{
		this->m_EditRegion.minX += dRgn.minX;
		this->m_EditRegion.maxX += dRgn.maxX;
	}
	else
	{
		this->m_EditRegion.minX -= dRgn.maxX;
		this->m_EditRegion.maxX -= dRgn.minX;
	}

	if ( true == SignY )
	{
		this->m_EditRegion.minY += dRgn.minY;	
		this->m_EditRegion.maxY += dRgn.maxY;
	}
	else
	{
		this->m_EditRegion.minY -= dRgn.maxY;	
		this->m_EditRegion.maxY -= dRgn.minY;
	}
	switch ( m_CaliIndex )
	{
	case 0: 
	case 1: 
	case 2: 
	case 3: 
		m_CaliComponentRegion[m_CaliIndex] = m_EditRegion; 
		break;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditFdView::ResetActiveObjPosFocus()
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
void CEditFdView::ResetActiveObjPosSelect()
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
void CEditFdView::CheckActiveObjFocus(TActiveObj &Obj)
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
void CEditFdView::CheckActiveObjFocus(TActiveObj &Obj, CAOIBox &Box)
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
bool CEditFdView::ExecModifyActiveObjPos()//執行選中物件的座標
{
	const int nWndPx = m_MousePosCurrent.x-m_MousePosLast.x;
	const int nWndPy = m_MousePosCurrent.y-m_MousePosLast.y;
	return ExecModifyActiveObjPosKernel(nWndPx, nWndPy);
}
//-------------------------------------------------------------------------------------//
bool CEditFdView::ExecModifyActiveObjPosKernel(int nWndPx, int nWndPy)//執行選中物件的座標
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
bool CEditFdView::ExecModifyActiveObjSize(CURSOR_POS_MODE CursorMode)//執行選中物件的尺寸
{
	CAOIModel *ModelPtr = CEditFdView::GetModelPtr();
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
	ModelPtr->ModifyModelBoxSize(BoxPtr, WndPtr, LandPtr, WndRoiPtr, WndMaskPtr, dRgn, LinkMode);

	BoxPtr = ModelPtr->GetModelBodyBoxPtr();
	ModelPtr->ModifyModelBoxSize(BoxPtr, NULL, NULL, NULL, NULL, dRgn, LinkMode);
	UpdateActiveObjList();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditFdView::ExecCoordinateCalibration(int nComponents)//執行座標校正
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }
	DISTRICT_ID DistrictID = ProjectPtr->GetProjectActDistrictID();
	if ( ALIGN_FD_PANEL == m_AlignFdMode )
	{
		CAOIPanel *PanelPtr = ProjectPtr->GetProjectActivePanel();
		if ( NULL == PanelPtr ) { return false; }

		CMapCoordinate *CTSPtr = PanelPtr->GetPanelMapCTSPtr(DistrictID);//整板的座標轉換-Cad to Stage
		CMapCoordinate *STCPtr = PanelPtr->GetPanelMapSTCPtr(DistrictID);//整板的座標轉換-Stage to Cad		
		CTSPtr->CalcMatrix2D(m_CaliComponentCadPosX, m_CaliComponentCadPosY, m_CaliComponentStagePosX, m_CaliComponentStagePosY, nComponents);
		STCPtr->CalcMatrix2D(m_CaliComponentStagePosX, m_CaliComponentStagePosY, m_CaliComponentCadPosX, m_CaliComponentCadPosY, nComponents);
		PanelPtr->CalcPanelStagePosition(DistrictID);
		PanelPtr->CheckPanelMapCoordinate(DistrictID, 1.0);
		UpdateFdModelStats();
		RedrawWnd();
	}
	if ( ALIGN_FD_BOARD == m_AlignFdMode )
	{
		CAOIBoard *BoardPtr = ProjectPtr->GetProjectActiveBoard();
		if ( NULL == BoardPtr ) { return false; }		
		CMapCoordinate *CTSPtr = BoardPtr->GetBoardMapCTSPtr(DistrictID);//整板的座標轉換-Cad to Stage
		CMapCoordinate *STCPtr = BoardPtr->GetBoardMapSTCPtr(DistrictID);//整板的座標轉換-Stage to Cad		
		const bool BoardMapEnable = BoardPtr->GetBoardMapEnable();

		CTSPtr->CalcMatrix2D(m_CaliComponentCadPosX, m_CaliComponentCadPosY, m_CaliComponentStagePosX, m_CaliComponentStagePosY, nComponents);
		STCPtr->CalcMatrix2D(m_CaliComponentStagePosX, m_CaliComponentStagePosY, m_CaliComponentCadPosX, m_CaliComponentCadPosY, nComponents);		
		BoardPtr->SetBoardMapEnable(true);
		BoardPtr->CalcBoardStagePosition(DistrictID);
		BoardPtr->CheckBoardMapCoordinate(DistrictID);
		BoardPtr->SetBoardMapEnable(BoardMapEnable);
		UpdateFdModelStats();
		RedrawWnd();
	}
	return true;
}
//-------------------------------------------------------------------------------------//
CURSOR_POS_MODE CEditFdView::CheckCursorPosModeFd(POINT pt)//確認鼠標座標模式
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
CURSOR_POS_MODE CEditFdView::CheckCursorPosModeEdit(POINT pt)//確認鼠標座標模式
{
	CURSOR_POS_MODE CursorMode = CURSOR_POS_NONE;	
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return CursorMode; }

	if ( false == m_DrawEditRect ) { return CursorMode; }	

	RECT         Rect;
	TRECT4D      dRect;
	SIZE         szGrid;
	double       Angle = 0;
	size_t       i = 0;			
	RECT         FdRect;
	TFdRect     *FdRectPtr = NULL;		
	TPOINT2D     Cp;
	TPOINT2D     WndPt = pt;
	TPOINT2D     ImagePt, ImagePt2;	
	TPOINT2D     StagePt, StagePt2;
	TREGION4D    StageRgn;
	POINT        ImagePoint;

	const IMAGE_SIZE ImageW = GetImageW();
	const IMAGE_SIZE ImageH = GetImageH();	
	const TPOINT2D   StageCp = GetImageStageRgnCp();
	const TPOINT2D  &ImageRes = GetImageResolution();
	
	szGrid.cx = CEditFdView::GetEditCheckStageSize();
	szGrid.cy = CEditFdView::GetEditCheckStageSize();
	ImageAPI.MapWndPtToImagePt_DBL(ImageW, ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, WndPt, ImagePt);	
	AOIDataCollect.MapCameraPtToStage(ImageW, ImageH, ImageRes, ImagePt, StageCp, StagePt);

	Cp.x = Cp.y = 0;
	//Stage Mode
	StageRgn = m_EditRegion;	
	JetAPI::Point2DToPoint(StagePt, ImagePoint);
	JetAPI::Region4DToRect(StageRgn, FdRect, true);
	CursorMode = JetAPI::CheckCursorPosMode(FdRect, szGrid, ImagePoint, CHECK_CURSOR_MODE_FRAME);		

	const bool SignX = AOIDataCollect.GetStageSignPositiveX();
	const bool SignY = AOIDataCollect.GetStageSignPositiveY();
	CURSOR_POS_MODE CadCursorMode= JetAPI::MapStageCursorPosModeToImageCursorPosMode(CursorMode, SignX, SignY);
	return CadCursorMode;
}
//-------------------------------------------------------------------------------------//
void CEditFdView::PreInitImageBuffer()//預先影像記憶體
{
	int i=0;
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
void CEditFdView::ReleaseImageBuffer()//釋放影像記憶體	
{
	AOIDataCollect.ReleaseFieldUniFrameList();
	JetAPI::ClearUniFrameList(m_UniFrameList, FRAME_MAX_COUNT);	
}
//-------------------------------------------------------------------------------------//
void CEditFdView::BuildShowImageBuffer()//建立顯示的影像記憶體
{
	const char fnName[] = "CEditFdView::BuildShowImageBuffer";
	ReleaseShowImageBuffer();

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
void CEditFdView::ReleaseShowImageBuffer()//釋放顯示影像記憶體	
{
	if ( NULL != m_ShowImagePtr )
	{	JetMemory.free_func(m_ShowImagePtr); }
	m_ShowImageW = 1024;
	m_ShowImageH = 1024;
	m_ShowImageStep = 1024*3;
	m_ShowBitCount = 24;	
}
//-------------------------------------------------------------------------------------//	
bool CEditFdView::GetStageSelectRegion(TREGION4D &StageRgn)//取得選取區域-機台座標
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
bool CEditFdView::GetCurrentFrame(IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, IMAGE_PTR &ImagePtr, SPACE_PTR &SpacePtr, MASK_PTR &MaskPtr)//取得目前影像
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
bool CEditFdView::GetFrameImage(unsigned int Index, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, IMAGE_PTR &ImagePtr, SPACE_PTR &SpacePtr, MASK_PTR &MaskPtr)//取得目前影像
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
int CEditFdView::GetMaxFrameCount()
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
bool CEditFdView::AdjustCurrentFrames()
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

	WndImageRect.left   = RealWndCpx+m_ImageOffset.x-(WndSizeW/2);
	WndImageRect.top    = RealWndCpy+m_ImageOffset.y-(WndSizeH/2);
	WndImageRect.right  = RealWndCpx+m_ImageOffset.x+(WndSizeW/2);
	WndImageRect.bottom = RealWndCpy+m_ImageOffset.y+(WndSizeH/2);

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
			BuildShowImageBuffer();
			CreateBKImage();
			RedrawWnd();
		}
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditFdView::FillCurrentFrames(double Ratio)
{
	size_t   i=0;
	TSIZE2D  Res;
	TPOINT3D Pos;	
	const size_t MaxFrames = CEditFdView::GetMaxFrameCount();
	IMAGE_SIZE ImageW = AOIDataCollect.GetCameraImageW(PRIMARY_CAMERA_ID);
	IMAGE_SIZE ImageH = AOIDataCollect.GetCameraImageH(PRIMARY_CAMERA_ID);
	const double FOVWum = Ratio*AOIDataCollect.GetFovSizeRealW();
	const double FOVHum = Ratio*AOIDataCollect.GetFovSizeRealH();
	
	Pos.z = 0;
	Pos.x = m_FOVPosStage.x;
	Pos.y = m_FOVPosStage.y;	

	Res.cx = m_FrameResolution.x;
	Res.cy = m_FrameResolution.y;

	ReleaseImageBuffer();
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
void CEditFdView::BackupViewParam()//備份顯示參數
{
	AOIDataCollect.SetViewImageZoom(m_ImageZoom);
}
//-------------------------------------------------------------------------------------//
void CEditFdView::RestoreViewParam()//恢復顯示參數
{
	m_ImageZoom = AOIDataCollect.GetViewImageZoom();
}
//-------------------------------------------------------------------------------------//
void CEditFdView::CalcFovPosition()
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
void CEditFdView::ResetImageOffset()//復歸顯示移動值	
{
	m_ImageOffset.x = 0;
	m_ImageOffset.y = 0;
}
//-------------------------------------------------------------------------------------//
void CEditFdView::SwitchFrameImage()
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }
	m_ImageIndex = ProjectPtr->GetProjectMapIndexNext(m_ImageIndex);
	ProjectPtr->SetProjectMapIndex(m_ImageIndex);
	BuildShowImageBuffer();
	CreateBKImage();
	RedrawWnd();	
	return;
}
//-------------------------------------------------------------------------------------//
void CEditFdView::UpdateFrameImage()
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
	return;
}
//-------------------------------------------------------------------------------------//	
bool CEditFdView::ExecMoveToStage()
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
bool CEditFdView::ExecShowWndPosition()
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
bool CEditFdView::ExecGrabFov(double PosX, double PosY, double PosZ)
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
bool CEditFdView::ExecUpdateFov(double PosX, double PosY, double PosZ)
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

	UpdateFdSelected();
	BuildShowImageBuffer();
	CreateBKImage();
	RedrawWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditFdView::GetKeepImageOffset() const//取得是否保持影像偏移值
{
	return m_KeepImageOffset;
}
//-------------------------------------------------------------------------------------//
void CEditFdView::SetKeepImageOffset(bool val)//設定是否保持影像偏移值	
{
	m_KeepImageOffset = val;
}
//-------------------------------------------------------------------------------------//
MANIPULATE_MODEL_MODE CEditFdView::GetManipulateModelModeDefault()//操作模組模式	- 預設
{
	return MANIPULATE_MODEL_EDIT;
}
//-------------------------------------------------------------------------------------//
void CEditFdView::PostMessageToMainFrameWnd(UINT message, WPARAM wParam, LPARAM lParam)
{
	AOIDataCollect.PostMainFrameWndMessage(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void CEditFdView::SendMessageToMainFrameWnd(UINT message, WPARAM wParam, LPARAM lParam)
{
	AOIDataCollect.SendMainFrameWndMessage(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
bool CEditFdView::UpdateFdSelected()
{
	SetModel(NULL);
	m_ActiveObjList.clear();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }

	CAOIModel   *ModelPtr = NULL;
	CAOIFd      *FdPtr = ProjectPtr->GetProjectActiveFd();
	if ( NULL != FdPtr )
	{	
		ModelPtr = FdPtr->GetFdModelPtr();	
		m_EditRegion = FdPtr->GetFdRgnStage();
	}	
	SetModel(ModelPtr);	
	UpdateFdModelStats();	
	RedrawWnd();	
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditFdView::UpdateActiveObjList()
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
void CEditFdView::BuildActiveObjList(CAOIModel *ModelPtr, bool ActiveOnly)
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
void CEditFdView::AddActiveObject(const TActiveObj &ActiveObj, bool Check)
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
bool CEditFdView::UpdateImageByAlgParam()//依據演算法更新畫面
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
bool CEditFdView::ExecCalcWndColor()//計算檢測框顏色
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
bool  CEditFdView::ExecExtractWndColorFilter()//取得檢測框抽色參數
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
bool CEditFdView::ExecGetWndColorFilter(CColorRGBV &rgbv)//取得檢測框抽色參數
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
bool CEditFdView::ExecGatherColorFilter(bool CombineColorMode)//吸取抽色參數
{
	const char fnName[] = "CEditFdView::ExecGatherColorFilter";	
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }
	CAOIModel *ModelPtr = CEditFdView::GetModelPtr();
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
void CEditFdView::ExecAlgImage(CAOIModel *ModelPtr, CAOIWnd *WndPtr, CAlgBinaryParam &BinaryParam)
{
	const unsigned int FrameUniqueID = BinaryParam.GetBinaryFrameUniqueID();
	if ( FRAME_UNIQUE_ID_DLP == FrameUniqueID )//高度值需要Leveling所以不能用Field資料
	{	ExecAlgImage_Model(ModelPtr, WndPtr, BinaryParam);	}
	else
	{	ExecAlgImage_Field(ModelPtr, WndPtr, BinaryParam); }	
}
//-------------------------------------------------------------------------------------//
void CEditFdView::ExecAlgImage_Field(CAOIModel *ModelPtr, CAOIWnd *WndPtr, CAlgBinaryParam &BinaryParam)
{
	if ( NULL == ModelPtr ) { return; }
	if ( NULL == WndPtr ) { return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }

	const int nAlign = 4;
	const bool bClone = true;
	std::vector<TUNI_FRAME> FieldUniFrameList;
	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
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
	RECT BoundaryRect;
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

	const char fnName[] = "CEditFdView::ExecAlgImage_Field";		
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
		JetMemory.free_func(ModelGrayPtr);
		JetMemory.free_func(ModelMaskPtr);
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
}
//-------------------------------------------------------------------------------------//
void CEditFdView::ExecAlgImage_Model(CAOIModel *ModelPtr, CAOIWnd *WndPtr, CAlgBinaryParam &BinaryParam)
{	
	if ( NULL == ModelPtr ) { return; }
	if ( NULL == WndPtr ) { return; }	
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }

	const int  nAlign = 4;
	const bool bNoFilter=false;
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

	unsigned int FrameUniqueID=BinaryParam.GetBinaryFrameUniqueID();
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

	const char fnName[] = "CEditFdView::ExecAlgImage_Model";	
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
		JetMemory.free_func(ModelGrayPtr);
		JetMemory.free_func(ModelMaskPtr);
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
		IMAGE_DATA mskR=0xFF, mskG=0xFF, mskB=0x00, mskV=0xFF, Alpha;	
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
bool CEditFdView::ExecModelWndInspection(bool UpdateUI)
{	
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }
	CAOIModel *ModelPtr = CEditFdView::GetModelPtr();
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
	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	TUNI_FRAME UniFrame = UniFrameList[0];
	const IMAGE_SIZE ImageW = UniFrameList[0].ImageW;
	const IMAGE_SIZE ImageH = UniFrameList[0].ImageH;
	const double RegionW = ModelRgn.GetWidth();
	const double RegionH = ModelRgn.GetHeight();	
	TPOINT2D  ModelImageScale = ModelPtr->GetModelImageScale();
	CAOILand *LandPtr = WndPtr->GetWndLandPtr();	

	ModelPtr->InitModelInspection();
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
bool CEditFdView::ExecModelBarcodeInspect()
{
	CAOIModel *ModelPtr = CEditFdView::GetModelPtr();
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
bool CEditFdView::BuildModelUniFrameList(CAOIModel *ModelPtr, std::vector<TUNI_FRAME> &UniFrameList, int nAlign, bool bClone, bool bNoFilter)
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
		if ( AOIDataCollect.CreateModelUniFrameList(m_UniFrameList, m_FOVPosStage, m_FrameResolution, ModelPtr, UniFrameList, nAlign, bNoFilter) == false )		
		{	AOIDataCollect.ReleaseModelUniFrameList(); }		
		else
		{	AOIDataCollect.SetModelUniFrameList(ModelPtr, StageRgn, UniFrameList, bClone); }
		if ( false == bNoFilter)
		{
			PostMessageToMainFrameWnd(MSG_EDIT_IMAGE_VIEW_WND, WPARAM_UPDATE_IMAGE_MODEL_SELECTED, NULL);		
			//PostMessageToMainFrameWnd(MSG_EDIT_IMAGE_VIEW_WND, WPARAM_UPDATE_IMAGE_MODEL_NO_PROCESS, NULL);		
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditFdView::BuildModelWndUniFrameList(CAOIModel *ModelPtr,CAOIWnd *WndPtr, std::vector<TUNI_FRAME> &UniFrameList, int nAlign)
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
void CEditFdView::OnFiducialEditAddMode() 
{
	// TODO: Add your command handler code here
	if ( AOIDataCollect.OperateLevelEditFuncAddFd() == false ) {	return ; }
	AOIDataCollect.SetShowComponentList(false);
	AOIDataCollect.SetManipulateModelMode(MANIPULATE_MODEL_ADD);
	CEditFdView::RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CEditFdView::OnUpdateFiducialEditAddMode(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	MANIPULATE_MODEL_MODE ManiMode = AOIDataCollect.GetManipulateModelMode();
	if ( MANIPULATE_MODEL_ADD == ManiMode ) 
	{	pCmdUI->SetCheck(TRUE); }
	else
	{	pCmdUI->SetCheck(FALSE); }
}
//-------------------------------------------------------------------------------------//
void CEditFdView::OnFiducialEditEditMode() 
{
	// TODO: Add your command handler code here
	AOIDataCollect.SetShowComponentList(false);
	MANIPULATE_MODEL_MODE ManiMode = GetManipulateModelModeDefault();
	AOIDataCollect.SetManipulateModelMode(ManiMode);
	CEditFdView::UpdateFdModelStats();
	CEditFdView::RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CEditFdView::OnUpdateFiducialEditEditMode(CCmdUI* pCmdUI) 
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
void CEditFdView::OnFiducialEditCalibration() 
{
	// TODO: Add your command handler code here
	AOIDataCollect.SetShowComponentList(true);
	AOIDataCollect.SetManipulateModelMode(MANIPULATE_MODEL_CALIBRATION);	
	CEditFdView::RedrawWnd();

	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL != ProjectPtr )
	{
		if ( true == ProjectPtr->GetProjectRibbonComponentReBuild() )
		{
			ProjectPtr->SetProjectRibbonComponentReBuild(false);
			SendMessageToMainFrameWnd(MSG_RIBBON_BAR_WND, WPARAM_COMPONENT_COMBOX_BUILD, MENU_FIDUCIAL_EDIT_ALIGN_SET_COMPONENT_COMBO1);
			SendMessageToMainFrameWnd(MSG_RIBBON_BAR_WND, WPARAM_COMPONENT_COMBOX_BUILD, MENU_FIDUCIAL_EDIT_ALIGN_SET_COMPONENT_COMBO2);
			SendMessageToMainFrameWnd(MSG_RIBBON_BAR_WND, WPARAM_COMPONENT_COMBOX_BUILD, MENU_FIDUCIAL_EDIT_ALIGN_SET_COMPONENT_COMBO3);
			SendMessageToMainFrameWnd(MSG_RIBBON_BAR_WND, WPARAM_COMPONENT_COMBOX_BUILD, MENU_FIDUCIAL_EDIT_ALIGN_SET_COMPONENT_COMBO4);
		}
	}	
}
//-------------------------------------------------------------------------------------//
void CEditFdView::OnUpdateFiducialEditCalibration(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	MANIPULATE_MODEL_MODE ManiMode = AOIDataCollect.GetManipulateModelMode();
	if ( MANIPULATE_MODEL_CALIBRATION == ManiMode ) 
	{	pCmdUI->SetCheck(TRUE); }
	else
	{	pCmdUI->SetCheck(FALSE); }
}
//-------------------------------------------------------------------------------------//
void CEditFdView::OnFiducialPasteToOtherBoards()
{
	// TODO: Add your command handler code here
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }
	if ( AOIDataCollect.OperateLevelEditFuncAddFd() == false ) { return; }

	CString str;
	str = _T("Do you want to paste the fd to other boards?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO )
	{	return ; }

	std::vector<CAOIFd*> SelFdList;	
	std::vector<CAOIFd*> SelFdList2;
	std::vector<CAOIFd*> SelFdListAdd;
	ProjectPtr->GetProjectBoardFdSelected(SelFdList);	
	ProjectPtr->PasteProjectFdToOtherBoards(SelFdList);	
	ProjectPtr->GetProjectBoardFdSelected(SelFdList2);
	ProjectPtr->FilterProjectFdAdded(SelFdList, SelFdList2, SelFdListAdd);	
	LogOperCtrl.SaveLogProjectFdSelectedClone(SelFdList);
	LogOperCtrl.SaveLogProjectFdSelectedAdd(SelFdListAdd);
	PostMessageToMainFrameWnd(MSG_EDIT_PART_LIST_WND, WPARAM_BUILD_PART_LIST, LPARAM_BUILD_DOCK_LIST_PART);	
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CEditFdView::OnUpdateFiducialPasteToOtherBoards(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	BOOL bEnable=TRUE;
	std::vector<CAOIFd*> SelFdList;	
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL != ProjectPtr ) 
	{	ProjectPtr->GetProjectBoardFdSelected(SelFdList); }
	if ( 0==SelFdList.size() || AOIDataCollect.OperateLevelEditFuncAddFd()==false )
	{	bEnable = FALSE;	}
	else
	{	bEnable=TRUE;	}
	pCmdUI->Enable(bEnable);
}
//-------------------------------------------------------------------------------------//
void CEditFdView::OnFiducialEditDelSelected() 
{
	// TODO: Add your command handler code here
	ExecDeleteFdSelected();
}
//-------------------------------------------------------------------------------------//
void CEditFdView::OnUpdateFiducialEditDelSelected(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr )
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE); }
}
//-------------------------------------------------------------------------------------//
bool CEditFdView::ExecDeleteFdSelected()
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }
	if ( AOIDataCollect.OperateLevelEditFuncDelFd() == false ) { return false; }

	ResetFdModel();
	AOIDataCollect.ReleaseModelUniFrameList();
	LogOperCtrl.SaveLogProjectFdSelectedDelete(ProjectPtr);
	ProjectPtr->DeleteProjectFdSelected();
	ProjectPtr->ResetProjectActiveIndex();
	PostMessageToMainFrameWnd(MSG_EDIT_WND_PROPERTY_WND, WPARAM_BUILD_WND_SELECTED, NULL);	
	PostMessageToMainFrameWnd(MSG_EDIT_PART_LIST_WND, WPARAM_BUILD_PART_LIST, LPARAM_BUILD_DOCK_LIST_PART);	
	CEditFdView::RedrawWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditFdView::ExecMoveToComponent(CAOIComponent *ComponentPtr)
{
	if ( NULL == ComponentPtr ) { return true; }
	TREGION4D      StageRgn;
	ComponentPtr->GetComponentRoiStageRegion(StageRgn);
	const double StagePosX = ComponentPtr->GetComponentStagePosX();
	const double StagePosY = ComponentPtr->GetComponentStagePosY();	
	AOIDataCollect.MoveStageTo(StagePosX, StagePosY, StageRgn);
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditFdView::OnFiducialEditAlignPanel() 
{
	// TODO: Add your command handler code here
	ResetCaliComponentList();
	m_AlignFdMode = ALIGN_FD_PANEL;	
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CEditFdView::OnUpdateFiducialEditAlignPanel(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CAOIProject *ProjectPtr = GetActiveProject();	
	const bool CalibrationMode = CheckCalibrationMode();
	if ( NULL == ProjectPtr || false==CalibrationMode )
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE);	}

	if ( ALIGN_FD_PANEL == m_AlignFdMode )
	{	pCmdUI->SetCheck(TRUE); }
	else
	{	pCmdUI->SetCheck(FALSE); }		
}
//-------------------------------------------------------------------------------------//
void CEditFdView::OnFiducialEditAlignPanelCombo() 
{
	// TODO: Add your command handler code here
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }
	ResetCaliComponentList();
	SendMessageToMainFrameWnd(MSG_RIBBON_BAR_WND, WPARAM_PANEL_COMBOX_SEL_CHANGE, MENU_FIDUCIAL_EDIT_ALIGN_PANEL_COMBO);
	unsigned int PanelIndex = ProjectPtr->GetProjectRibbonPanelIndex();
	if ( INVALID_INDEX == PanelIndex ) { return; }
	CAOIPanel *PanelPtr = ProjectPtr->GetProjectPanelPtr(PanelIndex, true);
	if ( NULL == PanelPtr ) { return; }
	CAOIBoard *BoardPtr = PanelPtr->GetPanelBoardPtr(0, true);
	if ( NULL == BoardPtr ) { return ; }
	unsigned int BoardIndex = BoardPtr->GetBoardIndex_Project();
	ProjectPtr->SetProjectRibbonBoardIndex(BoardIndex);
	SendMessageToMainFrameWnd(MSG_RIBBON_BAR_WND, WPARAM_BOARD_COMBOX_BUILD, MENU_FIDUCIAL_EDIT_ALIGN_BOARD_COMBO);	
	SendMessageToMainFrameWnd(MSG_RIBBON_BAR_WND, WPARAM_BOARD_COMBOX_BUILD, MENU_FIDUCIAL_EDIT_ALIGN_SET_BOARD_COMBO1);
	SendMessageToMainFrameWnd(MSG_RIBBON_BAR_WND, WPARAM_COMPONENT_COMBOX_BUILD, MENU_FIDUCIAL_EDIT_ALIGN_SET_COMPONENT_COMBO1);	
	SendMessageToMainFrameWnd(MSG_RIBBON_BAR_WND, WPARAM_BOARD_COMBOX_BUILD, MENU_FIDUCIAL_EDIT_ALIGN_SET_BOARD_COMBO2);	
	SendMessageToMainFrameWnd(MSG_RIBBON_BAR_WND, WPARAM_COMPONENT_COMBOX_BUILD, MENU_FIDUCIAL_EDIT_ALIGN_SET_COMPONENT_COMBO2);	
	SendMessageToMainFrameWnd(MSG_RIBBON_BAR_WND, WPARAM_BOARD_COMBOX_BUILD, MENU_FIDUCIAL_EDIT_ALIGN_SET_BOARD_COMBO3);	
	SendMessageToMainFrameWnd(MSG_RIBBON_BAR_WND, WPARAM_COMPONENT_COMBOX_BUILD, MENU_FIDUCIAL_EDIT_ALIGN_SET_COMPONENT_COMBO3);	
	SendMessageToMainFrameWnd(MSG_RIBBON_BAR_WND, WPARAM_BOARD_COMBOX_BUILD, MENU_FIDUCIAL_EDIT_ALIGN_SET_BOARD_COMBO4);
	SendMessageToMainFrameWnd(MSG_RIBBON_BAR_WND, WPARAM_COMPONENT_COMBOX_BUILD, MENU_FIDUCIAL_EDIT_ALIGN_SET_COMPONENT_COMBO4);	

	unsigned int AlignPanelIndex = PanelPtr->GetPanelIndex_Project();
	unsigned int AlignBoardIndex = BoardPtr->GetBoardIndex_Project();	
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CEditFdView::OnUpdateFiducialEditAlignPanelCombo(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CAOIProject *ProjectPtr = GetActiveProject();	
	const bool CalibrationMode = CheckCalibrationMode();
	if ( NULL == ProjectPtr || false==CalibrationMode )
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditFdView::OnFiducialEditAlignBoard() 
{
	// TODO: Add your command handler code here
	ResetCaliComponentList();
	m_AlignFdMode = ALIGN_FD_BOARD;	
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CEditFdView::OnUpdateFiducialEditAlignBoard(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CAOIProject *ProjectPtr = GetActiveProject();	
	const bool CalibrationMode = CheckCalibrationMode();
	if ( NULL == ProjectPtr || false==CalibrationMode )
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE); }

	if ( ALIGN_FD_BOARD == m_AlignFdMode )
	{	pCmdUI->SetCheck(TRUE); }
	else
	{	pCmdUI->SetCheck(FALSE); }	
}
//-------------------------------------------------------------------------------------//
void CEditFdView::OnFiducialEditAlignBoardCombo() 
{
	// TODO: Add your command handler code here
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }
	ResetCaliComponentList();
	SendMessageToMainFrameWnd(MSG_RIBBON_BAR_WND, WPARAM_BOARD_COMBOX_SEL_CHANGE, MENU_FIDUCIAL_EDIT_ALIGN_BOARD_COMBO);
	unsigned int BoardIndex = ProjectPtr->GetProjectRibbonBoardIndex();		
	CAOIBoard *BoardPtr = ProjectPtr->GetProjectBoardPtr(BoardIndex, true);
	if ( NULL == BoardPtr ) { return ; }
	CAOIPanel *PanelPtr = BoardPtr->GetBoardPanelPtr();
	if ( NULL == PanelPtr ) { return; }
	
	//更新至校正零件的單板上
	SendMessageToMainFrameWnd(MSG_RIBBON_BAR_WND, WPARAM_BOARD_COMBOX_BUILD, MENU_FIDUCIAL_EDIT_ALIGN_SET_BOARD_COMBO1);
	SendMessageToMainFrameWnd(MSG_RIBBON_BAR_WND, WPARAM_COMPONENT_COMBOX_BUILD, MENU_FIDUCIAL_EDIT_ALIGN_SET_COMPONENT_COMBO1);	
	SendMessageToMainFrameWnd(MSG_RIBBON_BAR_WND, WPARAM_BOARD_COMBOX_BUILD, MENU_FIDUCIAL_EDIT_ALIGN_SET_BOARD_COMBO2);	
	SendMessageToMainFrameWnd(MSG_RIBBON_BAR_WND, WPARAM_COMPONENT_COMBOX_BUILD, MENU_FIDUCIAL_EDIT_ALIGN_SET_COMPONENT_COMBO2);	
	SendMessageToMainFrameWnd(MSG_RIBBON_BAR_WND, WPARAM_BOARD_COMBOX_BUILD, MENU_FIDUCIAL_EDIT_ALIGN_SET_BOARD_COMBO3);	
	SendMessageToMainFrameWnd(MSG_RIBBON_BAR_WND, WPARAM_COMPONENT_COMBOX_BUILD, MENU_FIDUCIAL_EDIT_ALIGN_SET_COMPONENT_COMBO3);	
	SendMessageToMainFrameWnd(MSG_RIBBON_BAR_WND, WPARAM_BOARD_COMBOX_BUILD, MENU_FIDUCIAL_EDIT_ALIGN_SET_BOARD_COMBO4);
	SendMessageToMainFrameWnd(MSG_RIBBON_BAR_WND, WPARAM_COMPONENT_COMBOX_BUILD, MENU_FIDUCIAL_EDIT_ALIGN_SET_COMPONENT_COMBO4);	

	unsigned int AlignPanelIndex = PanelPtr->GetPanelIndex_Project();
	unsigned int AlignBoardIndex = BoardPtr->GetBoardIndex_Project();
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CEditFdView::OnUpdateFiducialEditAlignBoardCombo(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CAOIProject *ProjectPtr = GetActiveProject();	
	const bool CalibrationMode = CheckCalibrationMode();
	if ( NULL == ProjectPtr || false==CalibrationMode || ALIGN_FD_BOARD!=m_AlignFdMode )
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditFdView::OnFiducialEditAlignSetBoardCombo1() 
{
	// TODO: Add your command handler code here
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }
	SetDrawEditRect(false);
	m_CaliComponentIdx[0] = INVALID_INDEX;
	SendMessageToMainFrameWnd(MSG_RIBBON_BAR_WND, WPARAM_BOARD_COMBOX_SEL_CHANGE, MENU_FIDUCIAL_EDIT_ALIGN_SET_BOARD_COMBO1);
	unsigned int BoardIndex = ProjectPtr->GetProjectRibbonBoardIndex();		
	CAOIBoard *BoardPtr = ProjectPtr->GetProjectBoardPtr(BoardIndex, true);
	if ( NULL == BoardPtr ) { return ; }
	CAOIPanel *PanelPtr = BoardPtr->GetBoardPanelPtr();
	if ( NULL == PanelPtr ) { return; }	
	unsigned int SelPanelIndex = PanelPtr->GetPanelIndex_Project();
	unsigned int SelBoardIndex = BoardPtr->GetBoardIndex_Project();

	CAOIComponent *ComponentPtr = BoardPtr->GetBoardComponentPtr(0, true);
	if ( NULL == ComponentPtr ) { return ; }
	unsigned int ComponentIndex = ComponentPtr->GetComponentIndex_Project();
	ProjectPtr->SetProjectRibbonComponentIndex(ComponentIndex);
	SendMessageToMainFrameWnd(MSG_RIBBON_BAR_WND, WPARAM_COMPONENT_COMBOX_BUILD, MENU_FIDUCIAL_EDIT_ALIGN_SET_COMPONENT_COMBO1);	
}
//-------------------------------------------------------------------------------------//
void CEditFdView::OnUpdateFiducialEditAlignSetBoardCombo1(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CAOIProject *ProjectPtr = GetActiveProject();	
	const bool CalibrationMode = CheckCalibrationMode();
	if ( NULL==ProjectPtr || ALIGN_FD_BOARD==m_AlignFdMode || false==CalibrationMode)
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditFdView::OnFiducialEditAlignSetBoardCombo2() 
{
	// TODO: Add your command handler code here
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }
	SetDrawEditRect(false);
	m_CaliComponentIdx[1] = INVALID_INDEX;
	SendMessageToMainFrameWnd(MSG_RIBBON_BAR_WND, WPARAM_BOARD_COMBOX_SEL_CHANGE, MENU_FIDUCIAL_EDIT_ALIGN_SET_BOARD_COMBO2);
	unsigned int BoardIndex = ProjectPtr->GetProjectRibbonBoardIndex();		
	CAOIBoard *BoardPtr = ProjectPtr->GetProjectBoardPtr(BoardIndex, true);
	if ( NULL == BoardPtr ) { return ; }
	CAOIPanel *PanelPtr = BoardPtr->GetBoardPanelPtr();
	if ( NULL == PanelPtr ) { return; }	
	unsigned int SelPanelIndex = PanelPtr->GetPanelIndex_Project();
	unsigned int SelBoardIndex = BoardPtr->GetBoardIndex_Project();

	CAOIComponent *ComponentPtr = BoardPtr->GetBoardComponentPtr(0, true);
	if ( NULL == ComponentPtr ) { return ; }
	unsigned int ComponentIndex = ComponentPtr->GetComponentIndex_Project();
	ProjectPtr->SetProjectRibbonComponentIndex(ComponentIndex);
	SendMessageToMainFrameWnd(MSG_RIBBON_BAR_WND, WPARAM_COMPONENT_COMBOX_BUILD, MENU_FIDUCIAL_EDIT_ALIGN_SET_COMPONENT_COMBO2);
}
//-------------------------------------------------------------------------------------//
void CEditFdView::OnUpdateFiducialEditAlignSetBoardCombo2(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CAOIProject *ProjectPtr = GetActiveProject();	
	const bool CalibrationMode = CheckCalibrationMode();
	if ( NULL==ProjectPtr || ALIGN_FD_BOARD==m_AlignFdMode || INVALID_INDEX==m_CaliComponentIdx[0] || false==CalibrationMode)
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditFdView::OnFiducialEditAlignSetBoardCombo3() 
{
	// TODO: Add your command handler code here
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }
	SetDrawEditRect(false);
	m_CaliComponentIdx[2] = INVALID_INDEX;
	SendMessageToMainFrameWnd(MSG_RIBBON_BAR_WND, WPARAM_BOARD_COMBOX_SEL_CHANGE, MENU_FIDUCIAL_EDIT_ALIGN_SET_BOARD_COMBO3);
	unsigned int BoardIndex = ProjectPtr->GetProjectRibbonBoardIndex();		
	CAOIBoard *BoardPtr = ProjectPtr->GetProjectBoardPtr(BoardIndex, true);
	if ( NULL == BoardPtr ) { return ; }
	CAOIPanel *PanelPtr = BoardPtr->GetBoardPanelPtr();
	if ( NULL == PanelPtr ) { return; }	
	unsigned int SelPanelIndex = PanelPtr->GetPanelIndex_Project();
	unsigned int SelBoardIndex = BoardPtr->GetBoardIndex_Project();

	CAOIComponent *ComponentPtr = BoardPtr->GetBoardComponentPtr(0, true);
	if ( NULL == ComponentPtr ) { return ; }
	unsigned int ComponentIndex = ComponentPtr->GetComponentIndex_Project();
	ProjectPtr->SetProjectRibbonComponentIndex(ComponentIndex);
	SendMessageToMainFrameWnd(MSG_RIBBON_BAR_WND, WPARAM_COMPONENT_COMBOX_BUILD, MENU_FIDUCIAL_EDIT_ALIGN_SET_COMPONENT_COMBO3);	
}
//-------------------------------------------------------------------------------------//
void CEditFdView::OnUpdateFiducialEditAlignSetBoardCombo3(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CAOIProject *ProjectPtr = GetActiveProject();	
	const bool CalibrationMode = CheckCalibrationMode();
	if ( NULL==ProjectPtr || ALIGN_FD_BOARD==m_AlignFdMode || INVALID_INDEX==m_CaliComponentIdx[1] || false==CalibrationMode)
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditFdView::OnFiducialEditAlignSetBoardCombo4() 
{
	// TODO: Add your command handler code here
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }
	SetDrawEditRect(false);
	m_CaliComponentIdx[3] = INVALID_INDEX;
	SendMessageToMainFrameWnd(MSG_RIBBON_BAR_WND, WPARAM_BOARD_COMBOX_SEL_CHANGE, MENU_FIDUCIAL_EDIT_ALIGN_SET_BOARD_COMBO4);	
	unsigned int BoardIndex = ProjectPtr->GetProjectRibbonBoardIndex();		
	CAOIBoard *BoardPtr = ProjectPtr->GetProjectBoardPtr(BoardIndex, true);
	if ( NULL == BoardPtr ) { return ; }
	CAOIPanel *PanelPtr = BoardPtr->GetBoardPanelPtr();
	if ( NULL == PanelPtr ) { return; }	
	unsigned int SelPanelIndex = PanelPtr->GetPanelIndex_Project();
	unsigned int SelBoardIndex = BoardPtr->GetBoardIndex_Project();

	CAOIComponent *ComponentPtr = BoardPtr->GetBoardComponentPtr(0, true);
	if ( NULL == ComponentPtr ) { return ; }
	unsigned int ComponentIndex = ComponentPtr->GetComponentIndex_Project();
	ProjectPtr->SetProjectRibbonComponentIndex(ComponentIndex);
	SendMessageToMainFrameWnd(MSG_RIBBON_BAR_WND, WPARAM_COMPONENT_COMBOX_BUILD, MENU_FIDUCIAL_EDIT_ALIGN_SET_COMPONENT_COMBO4);	
}
//-------------------------------------------------------------------------------------//
void CEditFdView::OnUpdateFiducialEditAlignSetBoardCombo4(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CAOIProject *ProjectPtr = GetActiveProject();	
	const bool CalibrationMode = CheckCalibrationMode();
	if ( NULL==ProjectPtr || ALIGN_FD_BOARD==m_AlignFdMode || INVALID_INDEX==m_CaliComponentIdx[2] || false==CalibrationMode)
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditFdView::OnFiducialEditAlignSetComponentCombo1() 
{
	// TODO: Add your command handler code here
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }
	SendMessageToMainFrameWnd(MSG_RIBBON_BAR_WND, WPARAM_COMPONENT_COMBOX_SEL_CHANGE, MENU_FIDUCIAL_EDIT_ALIGN_SET_COMPONENT_COMBO1);
	unsigned int ComponentIndex = ProjectPtr->GetProjectRibbonComponentIndex();	
	CAOIComponent *ComponentPtr = ProjectPtr->GetProjectComponentPtr(ComponentIndex, true);
	if ( NULL == ComponentPtr ) { return ; }

	CAOIPanel *PanelPtr = ComponentPtr->GetComponentPanelPtr();
	CAOIBoard *BoardPtr = ComponentPtr->GetComponentBoardPtr();
	if ( NULL==PanelPtr || NULL==BoardPtr ) { return; }

	SetDrawEditRect(true);
	unsigned int SelPanelIndex = PanelPtr->GetPanelIndex_Project();
	unsigned int SelBoardIndex = BoardPtr->GetBoardIndex_Project();
	unsigned int SelComponentIndex = ComponentPtr->GetComponentIndex_Project();
	ProjectPtr->SelectProjectAllComponents(false);
	ComponentPtr->SetComponentSelected(true);		
	m_CaliIndex = 0;
	if ( m_CaliComponentIdx[m_CaliIndex] == SelComponentIndex )
	{	m_EditRegion = m_CaliComponentRegion[m_CaliIndex];	}
	else
	{	ComponentPtr->GetComponentRoiStageRegion(m_EditRegion); }
	CEditFdView::ExecMoveToComponent(ComponentPtr);
}
//-------------------------------------------------------------------------------------//
void CEditFdView::OnUpdateFiducialEditAlignSetComponentCombo1(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CAOIProject *ProjectPtr = GetActiveProject();	
	const bool CalibrationMode = CheckCalibrationMode();
	if ( NULL == ProjectPtr || false==CalibrationMode)
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditFdView::OnFiducialEditAlignSetComponentCombo2() 
{
	// TODO: Add your command handler code here
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }
	SendMessageToMainFrameWnd(MSG_RIBBON_BAR_WND, WPARAM_COMPONENT_COMBOX_SEL_CHANGE, MENU_FIDUCIAL_EDIT_ALIGN_SET_COMPONENT_COMBO2);
	unsigned int ComponentIndex = ProjectPtr->GetProjectRibbonComponentIndex();	
	CAOIComponent *ComponentPtr = ProjectPtr->GetProjectComponentPtr(ComponentIndex, true);
	if ( NULL == ComponentPtr ) { return ; }

	CAOIPanel *PanelPtr = ComponentPtr->GetComponentPanelPtr();
	CAOIBoard *BoardPtr = ComponentPtr->GetComponentBoardPtr();
	if ( NULL==PanelPtr || NULL==BoardPtr ) { return; }

	SetDrawEditRect(true);
	unsigned int SelPanelIndex = PanelPtr->GetPanelIndex_Project();
	unsigned int SelBoardIndex = BoardPtr->GetBoardIndex_Project();
	unsigned int SelComponentIndex = ComponentPtr->GetComponentIndex_Project();		
	ProjectPtr->SelectProjectAllComponents(false);
	ComponentPtr->SetComponentSelected(true);	
	m_CaliIndex = 1;
	if ( m_CaliComponentIdx[m_CaliIndex] == SelComponentIndex )
	{	m_EditRegion = m_CaliComponentRegion[m_CaliIndex];	}
	else
	{	ComponentPtr->GetComponentRoiStageRegion(m_EditRegion); }
	CEditFdView::ExecMoveToComponent(ComponentPtr);
}
//-------------------------------------------------------------------------------------//
void CEditFdView::OnUpdateFiducialEditAlignSetComponentCombo2(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CAOIProject *ProjectPtr = GetActiveProject();	
	const bool CalibrationMode = CheckCalibrationMode();
	if ( NULL==ProjectPtr || INVALID_INDEX==m_CaliComponentIdx[0] || false==CalibrationMode)
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditFdView::OnFiducialEditAlignSetComponentCombo3() 
{
	// TODO: Add your command handler code here
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }
	SendMessageToMainFrameWnd(MSG_RIBBON_BAR_WND, WPARAM_COMPONENT_COMBOX_SEL_CHANGE, MENU_FIDUCIAL_EDIT_ALIGN_SET_COMPONENT_COMBO3);
	unsigned int ComponentIndex = ProjectPtr->GetProjectRibbonComponentIndex();	
	CAOIComponent *ComponentPtr = ProjectPtr->GetProjectComponentPtr(ComponentIndex, true);
	if ( NULL == ComponentPtr ) { return ; }

	CAOIPanel *PanelPtr = ComponentPtr->GetComponentPanelPtr();
	CAOIBoard *BoardPtr = ComponentPtr->GetComponentBoardPtr();
	if ( NULL==PanelPtr || NULL==BoardPtr ) { return; }

	SetDrawEditRect(true);
	unsigned int SelPanelIndex = PanelPtr->GetPanelIndex_Project();
	unsigned int SelBoardIndex = BoardPtr->GetBoardIndex_Project();
	unsigned int SelComponentIndex = ComponentPtr->GetComponentIndex_Project();		
	ProjectPtr->SelectProjectAllComponents(false);
	ComponentPtr->SetComponentSelected(true);	
	m_CaliIndex = 2;
	if ( m_CaliComponentIdx[m_CaliIndex] == SelComponentIndex )
	{	m_EditRegion = m_CaliComponentRegion[m_CaliIndex];	}
	else
	{	ComponentPtr->GetComponentRoiStageRegion(m_EditRegion); }
	CEditFdView::ExecMoveToComponent(ComponentPtr);
}
//-------------------------------------------------------------------------------------//
void CEditFdView::OnUpdateFiducialEditAlignSetComponentCombo3(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CAOIProject *ProjectPtr = GetActiveProject();	
	const bool CalibrationMode = CheckCalibrationMode();
	if ( NULL==ProjectPtr || INVALID_INDEX==m_CaliComponentIdx[1] || false==CalibrationMode)
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditFdView::OnFiducialEditAlignSetComponentCombo4() 
{
	// TODO: Add your command handler code here
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }
	SendMessageToMainFrameWnd(MSG_RIBBON_BAR_WND, WPARAM_COMPONENT_COMBOX_SEL_CHANGE, MENU_FIDUCIAL_EDIT_ALIGN_SET_COMPONENT_COMBO4);
	unsigned int ComponentIndex = ProjectPtr->GetProjectRibbonComponentIndex();	
	CAOIComponent *ComponentPtr = ProjectPtr->GetProjectComponentPtr(ComponentIndex, true);
	if ( NULL == ComponentPtr ) { return ; }

	CAOIPanel *PanelPtr = ComponentPtr->GetComponentPanelPtr();
	CAOIBoard *BoardPtr = ComponentPtr->GetComponentBoardPtr();
	if ( NULL==PanelPtr || NULL==BoardPtr ) { return; }

	SetDrawEditRect(true);
	unsigned int SelPanelIndex = PanelPtr->GetPanelIndex_Project();
	unsigned int SelBoardIndex = BoardPtr->GetBoardIndex_Project();
	unsigned int SelComponentIndex = ComponentPtr->GetComponentIndex_Project();
	ProjectPtr->SelectProjectAllComponents(false);
	ComponentPtr->SetComponentSelected(true);	
	m_CaliIndex = 3;
	if ( m_CaliComponentIdx[m_CaliIndex] == SelComponentIndex )
	{	m_EditRegion = m_CaliComponentRegion[m_CaliIndex];	}
	else
	{	ComponentPtr->GetComponentRoiStageRegion(m_EditRegion); }
	CEditFdView::ExecMoveToComponent(ComponentPtr);
}
//-------------------------------------------------------------------------------------//
void CEditFdView::OnUpdateFiducialEditAlignSetComponentCombo4(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CAOIProject *ProjectPtr = GetActiveProject();	
	const bool CalibrationMode = CheckCalibrationMode();
	if ( NULL==ProjectPtr || INVALID_INDEX==m_CaliComponentIdx[2] || false==CalibrationMode)
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditFdView::OnFiducialEditAlignSetComponent1() 
{
	// TODO: Add your command handler code here
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }	
	SendMessageToMainFrameWnd(MSG_RIBBON_BAR_WND, WPARAM_COMPONENT_COMBOX_SEL_CHANGE, MENU_FIDUCIAL_EDIT_ALIGN_SET_COMPONENT_COMBO1);
	unsigned int ComponentIndex = ProjectPtr->GetProjectRibbonComponentIndex();	
	CAOIComponent *ComponentPtr = ProjectPtr->GetProjectComponentPtr(ComponentIndex, true);
	if ( NULL == ComponentPtr ) { return ; }

	CAOIPanel *PanelPtr = ComponentPtr->GetComponentPanelPtr();
	CAOIBoard *BoardPtr = ComponentPtr->GetComponentBoardPtr();
	if ( NULL==PanelPtr || NULL==BoardPtr ) { return; }
	
	unsigned int SelPanelIndex = PanelPtr->GetPanelIndex_Project();
	unsigned int SelBoardIndex = BoardPtr->GetBoardIndex_Project();
	unsigned int SelComponentIndex = ComponentPtr->GetComponentIndex_Project();	
	const int idx = 0;
	m_CaliComponentIdx[idx] = SelComponentIndex;
	m_CaliComponentCadPosX[idx] = ComponentPtr->GetComponentCadPosX();
	m_CaliComponentCadPosY[idx] = ComponentPtr->GetComponentCadPosY();
	m_CaliComponentStagePosX[idx] = m_CaliComponentRegion[idx].GetCpX();
	m_CaliComponentStagePosY[idx] = m_CaliComponentRegion[idx].GetCpY();

	ProjectPtr->ResetProjectActiveIndex();
	ProjectPtr->SetProjectActivePanelIndex(SelPanelIndex);
	ProjectPtr->SetProjectActiveBoardIndex(SelBoardIndex);
	ExecCoordinateCalibration(1);
}
//-------------------------------------------------------------------------------------//
void CEditFdView::OnUpdateFiducialEditAlignSetComponent1(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CAOIProject *ProjectPtr = GetActiveProject();	
	const bool CalibrationMode = CheckCalibrationMode();
	if ( NULL == ProjectPtr || false==CalibrationMode)
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditFdView::OnFiducialEditAlignSetComponent2() 
{
	// TODO: Add your command handler code here
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }	
	SendMessageToMainFrameWnd(MSG_RIBBON_BAR_WND, WPARAM_COMPONENT_COMBOX_SEL_CHANGE, MENU_FIDUCIAL_EDIT_ALIGN_SET_COMPONENT_COMBO2);
	unsigned int ComponentIndex = ProjectPtr->GetProjectRibbonComponentIndex();	
	CAOIComponent *ComponentPtr = ProjectPtr->GetProjectComponentPtr(ComponentIndex, true);
	if ( NULL == ComponentPtr ) { return ; }

	CAOIPanel *PanelPtr = ComponentPtr->GetComponentPanelPtr();
	CAOIBoard *BoardPtr = ComponentPtr->GetComponentBoardPtr();
	if ( NULL==PanelPtr || NULL==BoardPtr ) { return; }

	unsigned int SelPanelIndex = PanelPtr->GetPanelIndex_Project();
	unsigned int SelBoardIndex = BoardPtr->GetBoardIndex_Project();
	unsigned int SelComponentIndex = ComponentPtr->GetComponentIndex_Project();
	const int idx = 1;
	m_CaliComponentIdx[idx] = SelComponentIndex;
	m_CaliComponentCadPosX[idx] = ComponentPtr->GetComponentCadPosX();
	m_CaliComponentCadPosY[idx] = ComponentPtr->GetComponentCadPosY();
	m_CaliComponentStagePosX[idx] = m_CaliComponentRegion[idx].GetCpX();
	m_CaliComponentStagePosY[idx] = m_CaliComponentRegion[idx].GetCpY();

	ProjectPtr->ResetProjectActiveIndex();
	ProjectPtr->SetProjectActivePanelIndex(SelPanelIndex);
	ProjectPtr->SetProjectActiveBoardIndex(SelBoardIndex);
	ExecCoordinateCalibration(2);
}
//-------------------------------------------------------------------------------------//
void CEditFdView::OnUpdateFiducialEditAlignSetComponent2(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CAOIProject *ProjectPtr = GetActiveProject();	
	const bool CalibrationMode = CheckCalibrationMode();
	if ( NULL==ProjectPtr || INVALID_INDEX==m_CaliComponentIdx[0] || false==CalibrationMode)
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditFdView::OnFiducialEditAlignSetComponent3() 
{
	// TODO: Add your command handler code here
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }	
	SendMessageToMainFrameWnd(MSG_RIBBON_BAR_WND, WPARAM_COMPONENT_COMBOX_SEL_CHANGE, MENU_FIDUCIAL_EDIT_ALIGN_SET_COMPONENT_COMBO3);
	unsigned int ComponentIndex = ProjectPtr->GetProjectRibbonComponentIndex();	
	CAOIComponent *ComponentPtr = ProjectPtr->GetProjectComponentPtr(ComponentIndex, true);
	if ( NULL == ComponentPtr ) { return ; }

	CAOIPanel *PanelPtr = ComponentPtr->GetComponentPanelPtr();
	CAOIBoard *BoardPtr = ComponentPtr->GetComponentBoardPtr();
	if ( NULL==PanelPtr || NULL==BoardPtr ) { return; }

	unsigned int SelPanelIndex = PanelPtr->GetPanelIndex_Project();
	unsigned int SelBoardIndex = BoardPtr->GetBoardIndex_Project();
	unsigned int SelComponentIndex = ComponentPtr->GetComponentIndex_Project();		
	const int idx = 2;
	m_CaliComponentIdx[idx] = SelComponentIndex;
	m_CaliComponentCadPosX[idx] = ComponentPtr->GetComponentCadPosX();
	m_CaliComponentCadPosY[idx] = ComponentPtr->GetComponentCadPosY();
	m_CaliComponentStagePosX[idx] = m_CaliComponentRegion[idx].GetCpX();
	m_CaliComponentStagePosY[idx] = m_CaliComponentRegion[idx].GetCpY();

	ProjectPtr->ResetProjectActiveIndex();
	ProjectPtr->SetProjectActivePanelIndex(SelPanelIndex);
	ProjectPtr->SetProjectActiveBoardIndex(SelBoardIndex);
	ExecCoordinateCalibration(3);
}
//-------------------------------------------------------------------------------------//
void CEditFdView::OnUpdateFiducialEditAlignSetComponent3(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CAOIProject *ProjectPtr = GetActiveProject();	
	const bool CalibrationMode = CheckCalibrationMode();
	if ( NULL==ProjectPtr || INVALID_INDEX==m_CaliComponentIdx[1] || false==CalibrationMode)
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditFdView::OnFiducialEditAlignSetComponent4() 
{
	// TODO: Add your command handler code here
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }	
	SendMessageToMainFrameWnd(MSG_RIBBON_BAR_WND, WPARAM_COMPONENT_COMBOX_SEL_CHANGE, MENU_FIDUCIAL_EDIT_ALIGN_SET_COMPONENT_COMBO4);
	unsigned int ComponentIndex = ProjectPtr->GetProjectRibbonComponentIndex();	
	CAOIComponent *ComponentPtr = ProjectPtr->GetProjectComponentPtr(ComponentIndex, true);
	if ( NULL == ComponentPtr ) { return ; }

	CAOIPanel *PanelPtr = ComponentPtr->GetComponentPanelPtr();
	CAOIBoard *BoardPtr = ComponentPtr->GetComponentBoardPtr();
	if ( NULL==PanelPtr || NULL==BoardPtr ) { return; }

	unsigned int SelPanelIndex = PanelPtr->GetPanelIndex_Project();
	unsigned int SelBoardIndex = BoardPtr->GetBoardIndex_Project();
	unsigned int SelComponentIndex = ComponentPtr->GetComponentIndex_Project();		
	const int idx = 3;
	m_CaliComponentIdx[idx] = SelComponentIndex;
	m_CaliComponentCadPosX[idx] = ComponentPtr->GetComponentCadPosX();
	m_CaliComponentCadPosY[idx] = ComponentPtr->GetComponentCadPosY();
	m_CaliComponentStagePosX[idx] = m_CaliComponentRegion[idx].GetCpX();
	m_CaliComponentStagePosY[idx] = m_CaliComponentRegion[idx].GetCpY();

	ProjectPtr->ResetProjectActiveIndex();
	ProjectPtr->SetProjectActivePanelIndex(SelPanelIndex);
	ProjectPtr->SetProjectActiveBoardIndex(SelBoardIndex);
	ExecCoordinateCalibration(4);
}
//-------------------------------------------------------------------------------------//
void CEditFdView::OnUpdateFiducialEditAlignSetComponent4(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CAOIProject *ProjectPtr = GetActiveProject();	
	const bool CalibrationMode = CheckCalibrationMode();
	if ( NULL==ProjectPtr || INVALID_INDEX==m_CaliComponentIdx[2] || false==CalibrationMode)
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditFdView::OnFiducialEditAlign()
{
	CString str;	
	CAOIProject *ProjectPtr = GetActiveProject();
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
void CEditFdView::OnUpdateFiducialEditAlign(CCmdUI* pCmdUI)
{
#ifndef OFFLINE_VERSION
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd ) 
	{	pCmdUI->Enable(FALSE); }	
	else
	{	pCmdUI->Enable(TRUE); }	
#else
	pCmdUI->Enable(FALSE);
#endif//OFFLINE_VERSION
}
//-------------------------------------------------------------------------------------//
void CEditFdView::OnFiducialEditAlignFdAll() 
{
	// TODO: Add your command handler code here
	CString str;	
	BOOL bSaveField = FALSE;//CWnd::IsDlgButtonChecked(DEBUG_SAVE_FIELD_CHK);
	//this->m_ProjectMapWnd.SetDrawProjectMode(IMAGE_WND_DRAW_PROJECT_INSPECTING);	
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	ProjectPtr->SelectProjectAllFds(true);
	ProjectPtr->SelectProjectAllBarcodes(false);
	ProjectPtr->SelectProjectAllComponents(false);
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
	AOIDataCollect.SetIsNeedGrabFiducial(true);
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
void CEditFdView::OnUpdateFiducialEditAlignFdAll(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr )
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE); }
}
//-------------------------------------------------------------------------------------//
void CEditFdView::OnFiducialEditAlignFdSelected() 
{
	// TODO: Add your command handler code here
	CString str;	
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	//ProjectPtr->SelectProjectAllFds(false);
	ProjectPtr->SelectProjectAllBarcodes(false);
	ProjectPtr->SelectProjectAllComponents(false);
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
void CEditFdView::OnUpdateFiducialEditAlignFdSelected(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CAOIModel *ModelPtr = this->GetModelPtr();	
	if ( NULL == ModelPtr )
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE); }
}
//-------------------------------------------------------------------------------------//
void CEditFdView::OnFiducialEditPropertyWnd()
{
	const char fnName[] = "CEditFdView::OnFiducialEditPropertyWnd";
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
	ModelPtr->UpdateModelBodyToFd();
	UpdateFdSelected();
	return;
}
//-------------------------------------------------------------------------------------//
void CEditFdView::OnUpdateFiducialEditPropertyWnd(CCmdUI* pCmdUI)
{
	BOOL bEnable=TRUE;
	bool bLockUIWnd = GetLockUIWnd();	
	CAOIModel *ModelPtr = GetModelPtr();	
	if ( true==bLockUIWnd || NULL==ModelPtr )
	{	bEnable = FALSE; }
	pCmdUI->Enable(bEnable);
}
//-------------------------------------------------------------------------------------//
bool CEditFdView::ExecInspection_Finish()
{	
	SwitchProject();
	m_UpdateTestMapTickCount = 0;
	CAOIProject *ProjectPtr = CEditFdView::GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }	
	
	CString   OfflineFdName;
	CString   OfflineFolder;		
	bool      RepeatTest=false;
	TASK_MODE TaskMode = AOIDataCollect.GetTaskMode(); 	
	OFFLINE_FILE_MODE OfflineFileMode = ProjectPtr->GetProjectOfflineFileMode();	
	AOIDataCollect.SetModelAttachedObj(MODEL_ATTACHED_FD);	
	switch ( TaskMode )
	{
	case TASK_ALIGN_PROJECT:
		LockUIWnd(false);
		AOIDataCollect.SetOfflineMode(false);
		AOIDataCollect.SetProjectMapMode(false);		
		break;
	case TASK_INSPECT_PROJECT:
		RepeatTest = true;		
		//LockUIWnd(false);
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
		ExecMoveToStage();
		SendMessageToMainFrameWnd(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_UPDATE, (LPARAM)(this));
		*/
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditFdView::ExecOnlineInspection_Finish()
{
	ExecInspectionFinishKernel(true);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditFdView::ExecInspectionFinishKernel(bool bOnline)
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
bool CEditFdView::LockUIWnd(bool bLock)//鎖住視窗
{
	AOIDataCollect.SetIsLockUIWnd(bLock);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditFdView::GetLockUIWnd() const//取得是否鎖住UIWnd
{
	return AOIDataCollect.GetIsLockUIWnd();
}
//-------------------------------------------------------------------------------------//
bool CEditFdView::RetrieveCameraImage(WPARAM wParam, LPARAM lParam, bool bCameraCallBack)//取得相機影像
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
	ReleaseImageBuffer();
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
	UpdateFdModelStats();		
	CreateBKImage();
	RedrawWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditFdView::ExecToggleEnhanceImageMode()
{
	bool IsOK = true;
	IsOK = UpdateImageByAlgParam();	
	if ( true == IsOK )
	{	return; }
	UpdateFrameImage();
	return ;
}
//-------------------------------------------------------------------------------------//