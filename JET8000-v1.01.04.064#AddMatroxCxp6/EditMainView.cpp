// EditMainView.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "EditMainView.h"
//-------------------------------------------------------------------------------------//
#include "InputBoxWnd.h"
#include "InputListWnd.h"
#include "ArrayPasteWnd.h"
#include "NewPanelWizardWnd.h"
#include "ProjectMapMaskWnd.h"
#include "ProjectMapSpecRegionWnd.h"
#include "ProjectRegionMapWnd.h"
#include "ProjectCompareWnd.h"
#include "ProjectDivideDistrictWnd.h"
#include "BoardConfigWnd.h"
#include "ComponentDefectAlarmWnd.h"
#include "AutoAddComponentWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CEditMainView
//-------------------------------------------------------------------------------------//
IMPLEMENT_DYNCREATE(CEditMainView, CView)
//-------------------------------------------------------------------------------------//
CEditMainView::CEditMainView()
{	
	m_ProjectPtr = NULL;
	m_MainMode = MENU_MAIN_EDIT_MODE_COMPONENT;
	m_ImageZoom = 1.0;
	m_ImageOffset.x = m_ImageOffset.y = 0;
	m_BkColor = 0x000000;
	m_DrawAddRect = false;	
	m_DrawBoardList = false;
	m_DrawPanelList = false;	
	m_DrawFieldList = false;
	m_DrawComponent = true;
	m_DrawComponentMode = true;
	m_ShowComponentName = true;
	m_ModifyPanelPos = false;
	m_ModifyBoardPos = false;
	m_ModifyComponentPos = false;
	m_ShowPopupMenu = true;

	//m_clrOK = 0x008000;
	//m_clrNG = 0x000080;
	//m_clrUnTest = 0x808080;
	m_clrFdLine = 0x00FFFF;
	m_clrFdText = 0x2200A0;
	m_clrSBLine = 0x00FFFF;
	m_clrFdLineExt = 0xFFFF00;
	m_clrPanelLine = 0x00FFFF;
	m_clrPanelText = 0x2200A0; 
	m_clrBoardLine = 0x00FFFF;
	m_clrBoardText = 0x2200A0;
	m_clrComponentLine = 0x00FFFF;
	m_clrComponentText = 0x2200A0;
	//m_clrBypassed = 0xFF0000;
	m_clrSelected = 0xFFFFFF;

	m_LoadOfflineParam = false;
	m_FrameRatio = 1.0;
	m_ShowImageW = 1024;
	m_ShowImageH = 1024;
	m_ShowImageStep = 1024*3;
	m_ShowBitCount = 24;
	m_ShowImagePtr = NULL;
	m_UpdateTestMapTickCount = 0;
	PreInitUniFrameBuffer();

	CloseProject(true);
}
//-------------------------------------------------------------------------------------//
CEditMainView::~CEditMainView()
{
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CEditMainView, CView)
	//{{AFX_MSG_MAP(CEditMainView)
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
	ON_COMMAND(MENU_MAIN_EDIT_ROTATE_090, OnMainEditRotate090)
	ON_UPDATE_COMMAND_UI(MENU_MAIN_EDIT_ROTATE_090, OnUpdateMainEditRotate090)
	ON_COMMAND(MENU_MAIN_EDIT_ROTATE_180, OnMainEditRotate180)
	ON_UPDATE_COMMAND_UI(MENU_MAIN_EDIT_ROTATE_180, OnUpdateMainEditRotate180)
	ON_COMMAND(MENU_MAIN_EDIT_ROTATE_270, OnMainEditRotate270)
	ON_UPDATE_COMMAND_UI(MENU_MAIN_EDIT_ROTATE_270, OnUpdateMainEditRotate270)
	ON_COMMAND(MENU_MAIN_EDIT_ROTATE_OTHERS, OnMainEditRotateOthers)
	ON_UPDATE_COMMAND_UI(MENU_MAIN_EDIT_ROTATE_OTHERS, OnUpdateMainEditRotateOthers)
	ON_COMMAND(MENU_MAIN_EDIT_ROTATE_REVERSE, OnMainEditRotateReverse)
	ON_UPDATE_COMMAND_UI(MENU_MAIN_EDIT_ROTATE_REVERSE, OnUpdateMainEditRotateReverse)
	ON_COMMAND(MENU_MAIN_EDIT_MIRROR_POS_X, OnMainEditMirrorPosX)
	ON_UPDATE_COMMAND_UI(MENU_MAIN_EDIT_MIRROR_POS_X, OnUpdateMainEditMirrorPosX)
	ON_COMMAND(MENU_MAIN_EDIT_MIRROR_POS_Y, OnMainEditMirrorPosY)
	ON_UPDATE_COMMAND_UI(MENU_MAIN_EDIT_MIRROR_POS_Y, OnUpdateMainEditMirrorPosY)	
	ON_COMMAND(MENU_MAIN_EDIT_MOVE, OnMainEditMove)
	ON_UPDATE_COMMAND_UI(MENU_MAIN_EDIT_MOVE, OnUpdateMainEditMove)
	ON_COMMAND(MENU_MAIN_EDIT_DELETE_SELECTED, OnMainEditDeleteSelected)
	ON_UPDATE_COMMAND_UI(MENU_MAIN_EDIT_DELETE_SELECTED, OnUpdateMainEditDeleteSelected)
	ON_COMMAND(MENU_MAIN_EDIT_DELETE_UNSELECTED, OnMainEditDeleteUnselected)
	ON_UPDATE_COMMAND_UI(MENU_MAIN_EDIT_DELETE_UNSELECTED, OnUpdateMainEditDeleteUnselected)
	ON_COMMAND(MENU_MAIN_EDIT_SET_PART_NUMBER, OnMainEditSetPartNumber)
	ON_UPDATE_COMMAND_UI(MENU_MAIN_EDIT_SET_PART_NUMBER, OnUpdateMainEditSetPartNumber)
	ON_COMMAND(MENU_MAIN_EDIT_SELECT_INVERT_BOARD, OnMainEditSelectInvertBoard)
	ON_UPDATE_COMMAND_UI(MENU_MAIN_EDIT_SELECT_INVERT_BOARD, OnUpdateMainEditSelectInvertBoard)
	ON_COMMAND(MENU_MAIN_EDIT_SELECT_INVERT_PANEL, OnMainEditSelectInvertPanel)
	ON_UPDATE_COMMAND_UI(MENU_MAIN_EDIT_SELECT_INVERT_PANEL, OnUpdateMainEditSelectInvertPanel)
	ON_COMMAND(MENU_MAIN_EDIT_SELECT_INVERT_PROJECT, OnMainEditSelectInvertProject)
	ON_UPDATE_COMMAND_UI(MENU_MAIN_EDIT_SELECT_INVERT_PROJECT, OnUpdateMainEditSelectInvertProject)
	ON_COMMAND(MENU_MAIN_EDIT_MODE_COMPONENT, OnMainEditModeComponent)
	ON_UPDATE_COMMAND_UI(MENU_MAIN_EDIT_MODE_COMPONENT, OnUpdateMainEditModeComponent)
	ON_COMMAND(MENU_MAIN_EDIT_MODE_BOARD, OnMainEditModeBoard)
	ON_UPDATE_COMMAND_UI(MENU_MAIN_EDIT_MODE_BOARD, OnUpdateMainEditModeBoard)
	ON_COMMAND(MENU_MAIN_EDIT_MODE_PANEL, OnMainEditModePanel)
	ON_UPDATE_COMMAND_UI(MENU_MAIN_EDIT_MODE_PANEL, OnUpdateMainEditModePanel)
	ON_COMMAND(MENU_MAIN_EDIT_VIEW_1X1, OnMainEditView1x1)
	ON_UPDATE_COMMAND_UI(MENU_MAIN_EDIT_VIEW_1X1, OnUpdateMainEditView1x1)
	ON_COMMAND(MENU_MAIN_EDIT_VIEW_ALL, OnMainEditViewAll)
	ON_UPDATE_COMMAND_UI(MENU_MAIN_EDIT_VIEW_ALL, OnUpdateMainEditViewAll)
	ON_COMMAND(MENU_MAIN_EDIT_SWITCH_ONLINE_VIEW, OnMainEditSwitchOnlineView)
	ON_UPDATE_COMMAND_UI(MENU_MAIN_EDIT_SWITCH_ONLINE_VIEW, OnUpdateMainEditSwitchOnlineViewl)
	ON_COMMAND(MENU_MAIN_EDIT_SWITCH_EDIT_MODEL, OnMainEditSwitchEditModel)
	ON_UPDATE_COMMAND_UI(MENU_MAIN_EDIT_SWITCH_EDIT_MODEL, OnUpdateMainEditSwitchEditModel)
	ON_COMMAND(MENU_MAIN_EDIT_SWITCH_EDIT_BARCODE, OnMainEditSwitchEditBarcode)
	ON_UPDATE_COMMAND_UI(MENU_MAIN_EDIT_SWITCH_EDIT_BARCODE, OnUpdateMainEditSwitchEditBarcode)
	ON_COMMAND(MENU_MAIN_EDIT_SWITCH_EDIT_FD, OnMainEditSwitchEditFd)
	ON_UPDATE_COMMAND_UI(MENU_MAIN_EDIT_SWITCH_EDIT_FD, OnUpdateMainEditSwitchEditFd)
	ON_COMMAND(MENU_MAIN_EDIT_MANI_SELECT, OnMainEditManiSelect)
	ON_UPDATE_COMMAND_UI(MENU_MAIN_EDIT_MANI_SELECT, OnUpdateMainEditManiSelect)
	ON_COMMAND(MENU_MAIN_EDIT_MANI_SELECT_X_POS, OnMainEditManiSelectXPos)
	ON_UPDATE_COMMAND_UI(MENU_MAIN_EDIT_MANI_SELECT_X_POS, OnUpdateMainEditManiSelectXPos)
	ON_COMMAND(MENU_MAIN_EDIT_MANI_SELECT_Y_POS, OnMainEditManiSelectYPos)
	ON_UPDATE_COMMAND_UI(MENU_MAIN_EDIT_MANI_SELECT_Y_POS, OnUpdateMainEditManiSelectYPos)
	ON_COMMAND(MENU_MAIN_EDIT_MANI_SELECT_ROW_COL, OnMainEditManiSelectRowCol)
	ON_UPDATE_COMMAND_UI(MENU_MAIN_EDIT_MANI_SELECT_ROW_COL, OnUpdateMainEditManiSelectRowCol)
	ON_COMMAND(MENU_MAIN_EDIT_MANI_COPY_TO_FD, OnMainEditManiCopyToFd)
	ON_UPDATE_COMMAND_UI(MENU_MAIN_EDIT_MANI_COPY_TO_FD, OnUpdateMainEditManiCopyToFd)
	ON_COMMAND(MENU_MAIN_EDIT_MANI_ADD, OnMainEditManiAdd)
	ON_UPDATE_COMMAND_UI(MENU_MAIN_EDIT_MANI_ADD, OnUpdateMainEditManiAdd)
	ON_COMMAND(MENU_MAIN_EDIT_MANI_PASTE, OnMainEditManiPaste)
	ON_UPDATE_COMMAND_UI(MENU_MAIN_EDIT_MANI_PASTE, OnUpdateMainEditManiPaste)
	ON_COMMAND(MENU_MAIN_EDIT_MANI_PASTE_ARRAY, OnMainEditManiPasteArray)
	ON_UPDATE_COMMAND_UI(MENU_MAIN_EDIT_MANI_PASTE_ARRAY, OnUpdateMainEditManiPasteArray)
	ON_COMMAND(MENU_MAIN_EDIT_MANI_CLONE, OnMainEditManiClone)
	ON_UPDATE_COMMAND_UI(MENU_MAIN_EDIT_MANI_CLONE, OnUpdateMainEditManiClone)
	ON_COMMAND(MENU_MAIN_EDIT_SET_BYPASS, OnMainEditSetBypass)
	ON_UPDATE_COMMAND_UI(MENU_MAIN_EDIT_SET_BYPASS, OnUpdateMainEditSetBypass)
	ON_COMMAND(MENU_MAIN_EDIT_SET_BYPASS_3D, OnMainEditSetBypass3D)
	ON_UPDATE_COMMAND_UI(MENU_MAIN_EDIT_SET_BYPASS_3D, OnUpdateMainEditSetBypass3D)
	ON_COMMAND(MENU_MAIN_EDIT_SET_MODEL_ISOLATED, OnMainEditSetModelIsolated)
	ON_UPDATE_COMMAND_UI(MENU_MAIN_EDIT_SET_MODEL_ISOLATED, OnUpdateMainEditSetModelIsolated)
	ON_COMMAND(MENU_MAIN_EDIT_SET_MASK_BASE_COLOR_LINK_INDEX, OnMainEditSetMaskBaseColorLinkIndex)
	ON_UPDATE_COMMAND_UI(MENU_MAIN_EDIT_SET_MASK_BASE_COLOR_LINK_INDEX, OnUpdateMainEditSetMaskBaseColorLinkIndex)
	ON_COMMAND(MENU_MAIN_EDIT_SET_COMPONENT_ALARM_AOI, OnMainEditSetComponentAlarmAOI)
	ON_UPDATE_COMMAND_UI(MENU_MAIN_EDIT_SET_COMPONENT_ALARM_AOI, OnUpdateMainEditSetComponentAlarmAOI)
	ON_COMMAND(MENU_MAIN_EDIT_SET_COMPONENT_SAVE_REPORT_ARS, OnMainEditSetComponentSaveReportARS)
	ON_UPDATE_COMMAND_UI(MENU_MAIN_EDIT_SET_COMPONENT_SAVE_REPORT_ARS, OnUpdateMainEditSetComponentSaveReportARS)
	ON_COMMAND(MENU_MAIN_EDIT_CAPTURE_PROJECT_MAP, OnMainEditCaptureProjectMap)
	ON_UPDATE_COMMAND_UI(MENU_MAIN_EDIT_CAPTURE_PROJECT_MAP, OnUpdateMainEditCaptureProjectMap)
	ON_COMMAND(MENU_MAIN_EDIT_DIVIDE_DISTRICT_WND, OnMainEditDivideDistrictWnd)
	ON_UPDATE_COMMAND_UI(MENU_MAIN_EDIT_DIVIDE_DISTRICT_WND, OnUpdateMainEditDivideDistrictWnd)
	ON_COMMAND(MENU_MAIN_EDIT_ALIGN_FIDUCIAL, OnMainEditAlignFiducial)
	ON_UPDATE_COMMAND_UI(MENU_MAIN_EDIT_ALIGN_FIDUCIAL, OnUpdateMainEditAlignFiducial)
	ON_COMMAND(MENU_MAIN_EDIT_VIEW_PROJECT_MAP, OnMainEditViewProjectMap)
	ON_UPDATE_COMMAND_UI(MENU_MAIN_EDIT_VIEW_PROJECT_MAP, OnUpdateMainEditViewProjectMap)
	ON_COMMAND(MENU_MAIN_EDIT_MANI_PASTE_TO_OTHER_BOARDS, OnMainEditManiPasteToOtherBoards)
	ON_UPDATE_COMMAND_UI(MENU_MAIN_EDIT_MANI_PASTE_TO_OTHER_BOARDS, OnUpdateMainEditManiPasteToOtherBoards)
	ON_COMMAND(MENU_MAIN_EDIT_MANI_PASTE_TO_OTHER_PANELS, OnMainEditManiPasteToOtherPanels)
	ON_UPDATE_COMMAND_UI(MENU_MAIN_EDIT_MANI_PASTE_TO_OTHER_PANELS, OnUpdateMainEditManiPasteToOtherPanels)
	ON_COMMAND(MENU_MAIN_EDIT_MANI_PROJECT_MARK, OnMainEditManiProjectMark)
	ON_UPDATE_COMMAND_UI(MENU_MAIN_EDIT_MANI_PROJECT_MARK, OnUpdateMainEditManiProjectMark)
	ON_COMMAND(MENU_MAIN_EDIT_SET_BARCODE_DEVICE_INDEX, OnMainEditSetBarcodeDeviceIndex)
	ON_UPDATE_COMMAND_UI(MENU_MAIN_EDIT_SET_BARCODE_DEVICE_INDEX, OnUpdateMainEditSetBarcodeDeviceIndex)
	ON_COMMAND(MENU_MAIN_EDIT_SET_BARCODE_DEVICE_CODE_INDEX, OnMainEditSetBarcodeDeviceCodeIndex)
	ON_UPDATE_COMMAND_UI(MENU_MAIN_EDIT_SET_BARCODE_DEVICE_CODE_INDEX, OnUpdateMainEditSetBarcodeDeviceCodeIndex)
	ON_COMMAND(MENU_MAIN_EDIT_SAVE_COMPONENT_SAMPLE, OnMainEditSaveComponentSample)
	ON_UPDATE_COMMAND_UI(MENU_MAIN_EDIT_SAVE_COMPONENT_SAMPLE, OnUpdateMainEditSaveComponentSample)
	ON_COMMAND(MENU_MAIN_EDIT_ADD_COMPONENT_SAMPLE, OnMainEditAddComponentSample)
	ON_UPDATE_COMMAND_UI(MENU_MAIN_EDIT_ADD_COMPONENT_SAMPLE, OnUpdateMainEditAddComponentSample)
	ON_COMMAND(MENU_MAIN_EDIT_REPLACE_COMPONENT_SAMPLE, OnMainEditReplaceComponentSample)
	ON_UPDATE_COMMAND_UI(MENU_MAIN_EDIT_REPLACE_COMPONENT_SAMPLE, OnUpdateMainEditReplaceComponentSample)	
	ON_COMMAND(MENU_MAIN_EDIT_COPY_COMPONENT_SAMPLE, OnMainEditCopyComponentSample)
	ON_UPDATE_COMMAND_UI(MENU_MAIN_EDIT_COPY_COMPONENT_SAMPLE, OnUpdateMainEditCopyComponentSample)
	ON_COMMAND(MENU_MAIN_EDIT_PROJECT_MAP_MASK, OnMainEditProjectMapMask)
	ON_UPDATE_COMMAND_UI(MENU_MAIN_EDIT_PROJECT_MAP_MASK, OnUpdateMainEditProjectMapMask)	
	ON_COMMAND(MENU_MAIN_EDIT_PROJECT_MAP_MASK_REGION, OnMainEditProjectMapMaskRegion)
	ON_UPDATE_COMMAND_UI(MENU_MAIN_EDIT_PROJECT_MAP_MASK_REGION, OnUpdateMainEditProjectMapMaskRegion)
	ON_COMMAND(MENU_MAIN_EDIT_PROJECT_COMPARE, OnMainEditProjectCompare)
	ON_UPDATE_COMMAND_UI(MENU_MAIN_EDIT_PROJECT_COMPARE, OnUpdateMainEditProjectCompare)
	ON_COMMAND(MENU_MAIN_EDIT_COMPONENT_COMPARE, OnMainEditComponentCompare)
	ON_UPDATE_COMMAND_UI(MENU_MAIN_EDIT_COMPONENT_COMPARE, OnUpdateMainEditComponentCompare)
	ON_COMMAND(MENU_MAIN_EDIT_FULL_MAP_COMPONENT_CREATE, OnMainEditFullMapComponentCreate)
	ON_UPDATE_COMMAND_UI(MENU_MAIN_EDIT_FULL_MAP_COMPONENT_CREATE, OnUpdateMainEditFullMapComponentCreate)
	ON_COMMAND(MENU_MAIN_EDIT_FULL_MAP_COMPONENT_CLEAR, OnMainEditFullMapComponentClear)
	ON_UPDATE_COMMAND_UI(MENU_MAIN_EDIT_FULL_MAP_COMPONENT_CLEAR, OnUpdateMainEditFullMapComponentClear)
	ON_COMMAND(MENU_MAIN_EDIT_SET_BOARD_ORDER, OnMainEditSetBoardOrder)
	ON_UPDATE_COMMAND_UI(MENU_MAIN_EDIT_SET_BOARD_ORDER, OnUpdateMainEditSetBoardOrder)
	ON_COMMAND(MENU_MAIN_EDIT_MANI_AUTO_ADD, OnMainEditAutoAddComponet)
	ON_UPDATE_COMMAND_UI(MENU_MAIN_EDIT_MANI_AUTO_ADD, OnUpdateMainEditAutoAddComponet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CEditMainView diagnostics
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
void CEditMainView::AssertValid() const
{
	CView::AssertValid();
}
//-------------------------------------------------------------------------------------//
void CEditMainView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CEditMainView message handlers
//-------------------------------------------------------------------------------------//
void CEditMainView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();	
	// TODO: Add your specialized code here and/or call the base class
	SwitchMultiLanguage();
	CWnd::GetClientRect(&m_ImageWndRect);
	m_ImageWndMapDC.CreateMemDC(this, m_BkColor);
	m_ImageWndMemDC.CreateMemDC(this, m_BkColor);
	m_ImageWndMemDC2.CreateMemDC(this, m_BkColor);
	m_ProjectMarkWnd.Create(IDD_PROJECT_MARK_WND, this);

	SetShowPopupMenu(true);
	AOIDataCollect.SetShowFdList(true);	
	AOIDataCollect.SetShowMarkList(false);//true
	AOIDataCollect.SetShowPanelList(true);	
	AOIDataCollect.SetShowBoardList(true);	
	AOIDataCollect.SetShowBarcodeList(true);
	AOIDataCollect.SetShowComponentList(true);
	AOIDataCollect.SetCallbackWnd(CWnd::GetSafeHwnd());
	AOIDataCollect.SetModelAttachedObj(MODEL_ATTACHED_COMPONENT);
	AOIDataCollect.SetManipulateMainMode(MANIPULATE_MAIN_SELECT);	
	//AOIDataCollect.SwitchProjectTaskMode(PROJECT_TASK_NORMAL);

	m_MainMode=MENU_MAIN_EDIT_MODE_COMPONENT;
	SwitchProject();	
	RestoreViewParam();
	CreateBKImage();
	CWnd::PostMessage(MSG_CAMERA_REGRAB_IMAGE, NULL, NULL);		
	//CEditMainView::ExecMoveToStage();//由Frame視窗發送MSG_CAMERA_REGRAB_IMAGE
	return;
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnDestroy() 
{
	CView::OnDestroy();	
	// TODO: Add your message handler code here		
	CloseProject(true);	
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnSize(UINT nType, int cx, int cy) 
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
BOOL CEditMainView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
	return CView::OnEraseBkgnd(pDC);
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CView::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if ( TRUE == bShow )
	{
		AOIDataCollect.SetCallbackWnd(CWnd::GetSafeHwnd());		
	}
	else
	{
		if ( AOIDataCollect.GetOfflineMode() == true )
		{	CalcFovPosition();	}
		BackupViewParam();
		CloseProject(true);	 
	}
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	if ( NULL == pWnd ) { return; }
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
	const bool bGetLockUIWnd = GetLockUIWnd();
	const bool SwitchFrameMode = AOIDataCollect.CheckSwitchFrameMode();		
	if ( false == bGetLockUIWnd )
	{
		if ( true==SwitchFrameMode )
		{	SwitchImage(true);	 }
		else
		{	ExecPopupMenu(point, IDR_MENU_MAIN_POPUP);	}	
	}
	return;
}
//-------------------------------------------------------------------------------------//
BOOL CEditMainView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	UINT ControlID = pWnd->GetDlgCtrlID();	

	CURSOR_POS_MODE OldCursorMode = m_MousePosMode;
	CURSOR_POS_MODE CursorMode = CheckCursorPosMode(m_MousePosImageWnd);
	m_MousePosMode = CursorMode;
	
	//if ( CursorMode != OldCursorMode )
	//{	CEditMainView::RedrawWnd();	}
	if ( CURSOR_POS_NONE == CursorMode )
	{	return CView::OnSetCursor(pWnd, nHitTest, message);	}

	JetAPI::UpdateCursor(CursorMode); 
	return TRUE;

	//return CView::OnSetCursor(pWnd, nHitTest, message);
}
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CEditMainView drawing
//-------------------------------------------------------------------------------------//
void CEditMainView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
	CEditMainView::RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnLButtonDown(UINT nFlags, CPoint point) 
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
	if ( CURSOR_POS_NONE == m_MousePosMode )
	{	SetDrawAddRect(true); }
	else
	{	SetDrawAddRect(false); }
	CView::OnLButtonDown(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	::ReleaseCapture();
	ExecMoveSelectedObjectFinish();
	m_MousePosLast = m_MousePosCurrent;	
	m_MousePosImageWnd = m_MousePosCurrent = point;
	//CWnd::MapWindowPoints(&m_ImageWnd, &m_MousePosImageWnd, 1);		
	SetDrawAddRect(false);	
	if ( CURSOR_POS_NONE == m_MousePosMode )
	{	ExecMainSelectObject(); }
	ExecMainAddObject();
	ExecMainPasteObject();
	ExecMainProjectMark();
	RedrawWnd();
	CView::OnLButtonUp(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CView::OnLButtonDblClk(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnRButtonDown(UINT nFlags, CPoint point) 
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
void CEditMainView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default	
	::ReleaseCapture();
	const bool ProjectMapMode = AOIDataCollect.GetProjectMapMode();
	const bool SwitchFrameMode = AOIDataCollect.CheckSwitchFrameMode();
	MANIPULATE_MAIN_MODE ManiMainMode = AOIDataCollect.GetManipulateMainMode();
	m_MousePosImageWnd = m_MousePosCurrent = m_MousePosLast = point;
	//CWnd::MapWindowPoints(&m_ImageWnd, &m_MousePosImageWnd, 1);

	if ( CheckMousePosMoved() == false )//20241209
	{	
		if ( AOIDataCollect.CancelManipulateMainMode() == true ) 
		{	SetShowPopupMenu(false); }	
	}
	else		
	{	
		if ( false == ProjectMapMode )
		{	AdjustCurrentFrames(); }
		else
		{	CalcFovPosition();  }		
	}
	RedrawWnd();
	CView::OnRButtonUp(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default	
	//CEditMainView::CalcFovPosition();	
	CView::OnRButtonDblClk(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnMouseMove(UINT nFlags, CPoint point) 
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
		MANIPULATE_MAIN_MODE ManiMode=AOIDataCollect.GetManipulateMainMode();
		if ( MANIPULATE_MAIN_ADD == ManiMode )
		{	RedrawWnd(); }
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
			Modify = ExecMoveSelectedObject();
			break;
		case CURSOR_POS_LEFT:
		case CURSOR_POS_RIGHT:
		case CURSOR_POS_TOP:
		case CURSOR_POS_BOTTOM:
		case CURSOR_POS_LEFT_TOP:
		case CURSOR_POS_LEFT_BOTTOM:
		case CURSOR_POS_RIGHT_TOP:
		case CURSOR_POS_RIGHT_BOTTOM:
			//Modify = ExecModifyActiveObjSize(m_MousePosMode);
			break;
		}
		if ( true == Modify )
		{	bToDraw = true;	}
	}
	else if (nFlags & MK_RBUTTON )//滑鼠右鍵
	{
		this->m_ImageOffset.x += dPoint.x;
		this->m_ImageOffset.y += dPoint.y;
		//CEditMainView::CalcFovPosition();
		CEditMainView::CreateBKImage();
		bToDraw = true;		
	}	
	if ( true == m_DrawAddRect )
	{	bToDraw = true;		}	
	if ( true == bToDraw )
	{	RedrawWnd();	}
	CView::OnMouseMove(nFlags, point);
}
//-------------------------------------------------------------------------------------//
BOOL CEditMainView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	double NextImageZoom = m_ImageZoom;
	if ( zDelta > 0 ) 
	{	NextImageZoom *= 1.1;	}
	else
	{	NextImageZoom /= 1.1;	}	
	const bool   OfflineMode = AOIDataCollect.GetOfflineMode();
	const bool   ProjectMapMode  = AOIDataCollect.GetProjectMapMode();
	NextImageZoom = AOIDataCollect.AdjustImageZoom(NextImageZoom);
	ImageAPI.CalcImageWndZoom(m_ImageZoom, NextImageZoom, m_ImageOffset);
	m_ImageZoom = NextImageZoom;
	
	if ( true==OfflineMode && false==ProjectMapMode )
	{
		IMAGE_PTR  ImagePtr=NULL;
		IMAGE_SIZE ImageW=0;
		IMAGE_SIZE ImageH=0;
		IMAGE_SIZE ImageStep=0;
		IMAGE_SIZE BitCount=0;
		if ( GetCurrentImage(ImageW, ImageH, ImageStep, BitCount, ImagePtr) == false ) 
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
	//CalcCursorInfo(point);
	//CalcImageWndLBtn();
	//DrawImage();
	CreateBKImage();
	RedrawWnd();
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}
//-------------------------------------------------------------------------------------//
BOOL CEditMainView::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class		
	bool bRedraw = false;
	switch ( pMsg->message )
	{	
	case WM_KEYDOWN:
		switch ( pMsg->wParam )
		{
		case VK_DELETE:
			OnMainEditDeleteSelected();
			break;
		case VK_LEFT:	
			bRedraw = ExecMoveSelectedObjectKernel(-1, 0);
			break;
		case VK_RIGHT:
			bRedraw = ExecMoveSelectedObjectKernel(1, 0);
			break;
		case VK_UP:
			bRedraw = ExecMoveSelectedObjectKernel(0, -1);
			break;
		case VK_DOWN:
			bRedraw = ExecMoveSelectedObjectKernel(0, 1);
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
		if ( m_SelComponentList.size() != 0 )
		{	AOIDataCollect.SetProjectHasModified(pMsg->wParam, GetActiveProject());	}
		break;
	}	
	return CView::PreTranslateMessage(pMsg);
}
//-------------------------------------------------------------------------------------//
LRESULT CEditMainView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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
			ExecMoveToStage();			
			break;		
		case WPARAM_PROJECT_UPDATE:
			if ( CWnd::IsWindowVisible() == TRUE )
			{
				pWnd = (CWnd*)lParam;
				if ( pWnd != this )
				{
					//CalcFovPosition();//20241210
					//SwitchProject();
					//CreateBKImage();//20241210
					BuildPanelSelected();
					BuildBoardSelected();
					BuildComponentSelected();					
					RedrawWnd();  
				}
			}
			break;		
		case WPARAM_PROJECT_CLOSE:		
			CloseProject(true);
			if ( CWnd::IsWindowVisible() == TRUE )
			{	RedrawWnd(); }
			break;
		case WPARAM_PROJECT_PART_SELECTED:
			BuildComponentSelected();
			RedrawWnd(); 			
			break;
		case WPARAM_PROJECT_PART_DELETED:			
			BuildComponentSelected();
			RedrawWnd(); 			
			break;
		case WPARAM_CALC_CURRENT_FOV_POSITION:
			CalcFovPosition();
			break;
		case WPARAM_PROJECT_SWITCH_MARK:
			break;
		case WPARAM_PROJECT_SWITCH_MAP:
		case WPARAM_PROJECT_SWITCH_LANE:
			SwitchProjectDistrictID(true);
			break;
		case WPARAM_PROJECT_CLOSE_ACTIVE_OBJ:
			ReleaseUniFrameBuffer();
			ReleaseShowImageBuffer();
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
	case MSG_EDIT_MAIN_VIEW_WND:
		switch ( wParam )
		{
		case WPARAM_SWITCH_FRAME_IMAGE:
			SwitchImage(false);
			//UpdateFrameImage();
			//RedrawWnd();
			break;
		case WPARAM_UPDATE_VIEW_PART_SELECTED:
			BuildComponentSelected();
			RedrawWnd();
			break;
		case WPARAM_REDRAW_VIEW_WND:
		case WPARAM_REDRAW_PROJECT_MAP:
			RedrawWnd();
			hWnd = GetSafeHwnd();//否吃掉重複重繪訊息
			JetAPI::RemoveMessage(hWnd, MSG_EDIT_MAIN_VIEW_WND, MSG_EDIT_MAIN_VIEW_WND);
			break;
		case WPARAM_SHOW_WND_POSITION:
			ExecShowWndPosition();
			break;
		case WPARAM_TOGGLE_ENCHANGE_IMAGE_MODE://切換強化影像模式
			SwitchImage(false);			
			break;
		case WPARAM_SET_DRAW_PROJECT_MODE:
			CreateMapImage();
			break;
		case WPARAM_UPDATE_PROJECT_TEST_MAP:
			CreateTestMapImage();			
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
void CEditMainView::SwitchMultiLanguage()
{
	/*
	size_t  i = 0;	
	UINT    WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_EDIT_MAIN_VIEW");	
	//---------------------------------------------------------------------------------//
	WndID = IDD_EDIT_MAIN_VIEW;
	WndKey = _T("IDD_EDIT_MAIN_VIEW");
	this->GetWindowText(LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetWindowText(NewLabelText);
	//---------------------------------------------------------------------------------//	
	*/
}
//-------------------------------------------------------------------------------------//
CString CEditMainView::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_EDIT_MAIN_VIEW");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
bool CEditMainView::CheckMousePosMoved() const//確認滑鼠移動過
{
	const int dx = m_MousePosLast.x-m_MousePosFirst.x;
	const int dy = m_MousePosLast.y-m_MousePosFirst.y;
	if ( abs(dx) > 10 || abs(dy) > 10 )
	{	return true; }
	return false;
}
//-------------------------------------------------------------------------------------//
int CEditMainView::GetEditLineSize()//取得編輯線的尺寸
{	
	const int    LineSizeLevel = AOIDataCollect.GetSystemParameter().m_EditLineSizeLevel;
	return JetAPI::GetEditLineSize(m_ImageZoom, LineSizeLevel);
}
//-------------------------------------------------------------------------------------//
int CEditMainView::GetEditCheckSize()//取得編輯確認的尺寸
{
	const int    LineSizeLevel = AOIDataCollect.GetSystemParameter().m_EditLineSizeLevel;
	return JetAPI::GetEditCheckSize(m_ImageZoom, LineSizeLevel);	
}
//-------------------------------------------------------------------------------------//
inline bool CEditMainView::PtInControlWnd(const POINT &pt, UINT ID, POINT &pt2)
{
	pt2 = pt;
	if ( ::PtInRect(&m_ImageWndRect, pt) == FALSE )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
inline BOOL CEditMainView::MapWndPtToImagePt_DBL(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const RECT &WndRect, const TPOINT2D &OffsetPt, double ZoomScale, const TPOINT2D &WndPt, TPOINT2D &ImagePt)
{
	ImageAPI.MapWndPtToImagePt_DBL(ImageW, ImageH, WndRect, OffsetPt, ZoomScale, WndPt, ImagePt);
	return TRUE;
}
//-------------------------------------------------------------------------------------//
inline BOOL CEditMainView::MapImagePtToWndPt_DBL(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const RECT &WndRect, const TPOINT2D &OffsetPt, double ZoomScale, const TPOINT2D &ImagePt, TPOINT2D &WndPt)
{
	ImageAPI.MapImagePtToWndPt_DBL(ImageW, ImageH, WndRect, OffsetPt, ZoomScale, ImagePt, WndPt);
	return TRUE;
}
//-------------------------------------------------------------------------------------//
bool CEditMainView::ExecRotateObj(double Angle, bool Inverse)
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }

	bool Exected = false;
	if ( false == Inverse )
	{
		switch ( m_MainMode )
		{
		case MENU_MAIN_EDIT_MODE_BOARD:
			Exected = true;
			ProjectPtr->RotateProjectBoardSelected(Angle);
			LogOperCtrl.SaveLogProjectBoardSelectedRotate(ProjectPtr, Angle);
			break;
		case MENU_MAIN_EDIT_MODE_PANEL:
			Exected = true;
			ProjectPtr->RotateProjectPanelSelected(Angle);
			LogOperCtrl.SaveLogProjectPanelSelectedRotate(ProjectPtr, Angle);	
			break;
		case MENU_MAIN_EDIT_MODE_COMPONENT:
			Exected = true;
			ProjectPtr->RotateProjectComponentSelected(Angle);
			LogOperCtrl.SaveLogProjectComponentSelectedRotate(ProjectPtr, Angle);
			break;
		}
	}
	else
	{
		switch ( m_MainMode )
		{
		case MENU_MAIN_EDIT_MODE_BOARD:
			//ProjectPtr->RotateProjectBoardSelected(Angle);
			//LogOperCtrl.SaveLogProjectBoardSelectedRotate(ProjectPtr, Angle);
			break;
		case MENU_MAIN_EDIT_MODE_PANEL:
			//ProjectPtr->RotateProjectPanelSelected(Angle);
			//LogOperCtrl.SaveLogProjectPanelSelectedRotate(ProjectPtr, Angle);	
			break;
		case MENU_MAIN_EDIT_MODE_COMPONENT:
			Exected = true;
			ProjectPtr->ReverseProjectComponentSelected();
			break;
		}		
	}
	if ( false == Exected ) { return true; }
	BuildObjectSelected();	
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_PART_LIST_WND, WPARAM_UPDATE_PART_LIST, TREE_CTRL_UPDATE_TEXT);
	CEditMainView::RedrawWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnMainEditRotate090() 
{
	// TODO: Add your command handler code here
	CEditMainView::ExecRotateObj(90, false);
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnUpdateMainEditRotate090(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd ) 
	{	pCmdUI->Enable(FALSE); }	
	else
	{	pCmdUI->Enable(TRUE); }	
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnMainEditRotate180() 
{
	// TODO: Add your command handler code here
	CEditMainView::ExecRotateObj(180, false);
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnUpdateMainEditRotate180(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd ) 
	{	pCmdUI->Enable(FALSE); }	
	else
	{	pCmdUI->Enable(TRUE); }	
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnMainEditRotate270() 
{
	// TODO: Add your command handler code here
	CEditMainView::ExecRotateObj(270, false);
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnUpdateMainEditRotate270(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd ) 
	{	pCmdUI->Enable(FALSE); }	
	else
	{	pCmdUI->Enable(TRUE); }	
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnMainEditRotateOthers() 
{
	// TODO: Add your command handler code here
	CString      strValue;
	CString      strLabel;
	CString      strCaption;
	CString      strAngle = _T("0");
	CInputBoxWnd InputBox;
	const double DBL_Precesion = DBL_PRECISION;

	strCaption = _T("Rotate Angle");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strLabel = _T("Angle:");
	strLabel = LoadMultiLanguageString(strLabel, strLabel);
	InputBox.SetParam1(strCaption, strLabel, strAngle);
	if ( InputBox.DoModal() == IDCANCEL ) { return ; }
	strAngle = InputBox.m_DataEdit1;
	const double dAngle = ::_tcstod(strAngle, NULL);
	if ( fabs(dAngle) < DBL_Precesion ) { return; }

	CEditMainView::ExecRotateObj(dAngle, false);
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnUpdateMainEditRotateOthers(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd ) 
	{	pCmdUI->Enable(FALSE); }	
	else
	{	pCmdUI->Enable(TRUE); }	
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnMainEditRotateReverse() 
{
	// TODO: Add your command handler code here
	CEditMainView::ExecRotateObj(0, true);
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnUpdateMainEditRotateReverse(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd ) 
	{	pCmdUI->Enable(FALSE); }	
	else
	{	pCmdUI->Enable(TRUE); }	
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnMainEditMirrorPosX() 
{
	// TODO: Add your command handler code here
	ExecMirrorPosX();
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnUpdateMainEditMirrorPosX(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd ) 
	{	pCmdUI->Enable(FALSE); }	
	else
	{	pCmdUI->Enable(TRUE); }	
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnMainEditMirrorPosY() 
{
	// TODO: Add your command handler code here
	ExecMirrorPosY();
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnUpdateMainEditMirrorPosY(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd ) 
	{	pCmdUI->Enable(FALSE); }	
	else
	{	pCmdUI->Enable(TRUE); }	
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnMainEditMove() 
{
	// TODO: Add your command handler code here
	CString      strValue;
	CString      strLabel;
	CString      strCaption;
	CString      strX = _T("0");
	CString      strY = _T("0");
	CInputBoxWnd InputBox;
	const double DBL_Precesion = DBL_PRECISION;

	strCaption = _T("Set Offset Wnd");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strLabel = _T("X:");
	InputBox.SetParam1(strCaption, strLabel, strX);
	if ( InputBox.DoModal() == IDCANCEL ) { return ; }
	strX = InputBox.m_DataEdit1;

	strLabel = _T("Y:");
	InputBox.SetParam1(strCaption, strLabel, strY);
	if ( InputBox.DoModal() == IDCANCEL ) { return ; }
	strY = InputBox.m_DataEdit1;
	const double dX = ::_tcstod(strX, NULL);
	const double dY = ::_tcstod(strY, NULL);
	if ( fabs(dX)<DBL_Precesion && fabs(dY)<DBL_Precesion ) { return; }
	ExecMove(dX, dY);
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnUpdateMainEditMove(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd ) 
	{	pCmdUI->Enable(FALSE); }	
	else
	{	pCmdUI->Enable(TRUE); }	
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnMainEditDeleteSelected() 
{
	// TODO: Add your command handler code here
	ExecDeleteSelected();
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnUpdateMainEditDeleteSelected(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd ) 
	{	pCmdUI->Enable(FALSE); }	
	else
	{	pCmdUI->Enable(TRUE); }	
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnMainEditDeleteUnselected() 
{
	// TODO: Add your command handler code here
	AOIDataCollect.SetManipulateMainMode(MANIPULATE_MAIN_SELECT);
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnUpdateMainEditDeleteUnselected(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd ) 
	{	pCmdUI->Enable(FALSE); }	
	else
	{	pCmdUI->Enable(TRUE); }	
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnMainEditSetPartNumber() 
{
	// TODO: Add your command handler code here	
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }	
	CString      strValue;
	CString      strLabel;
	CString      strCaption;
	CInputBoxWnd InputBox;
	CString      PartNumber;
	CAOIComponent *pComponent = ProjectPtr->GetProjectActiveComponent();
	if ( NULL == pComponent ) { return; }

	strCaption = _T("Set Part Number Wnd");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strLabel = AOIDataDefine.GetPartNumberText();
	PartNumber = pComponent->GetComponentPartNumber();
	InputBox.SetParam1(strCaption, strLabel, PartNumber);
	while ( true ) 
	{
		if ( InputBox.DoModal() == IDCANCEL ) { return ; }
		PartNumber = InputBox.m_DataEdit1;
		PartNumber.MakeUpper();
		PartNumber.TrimLeft();
		PartNumber.TrimRight();
		if ( PartNumber.GetLength() == 0 ) 
		{	continue; }
		break;
	};
	ProjectPtr->SetProjectComponentSelectedPartNumber(PartNumber);
	LogOperCtrl.SaveLogProjectComponentSelectedPartNumber(ProjectPtr, PartNumber);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_PART_LIST_WND, WPARAM_BUILD_PART_LIST, LPARAM_BUILD_DOCK_LIST_EDIT);
	CEditMainView::RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnUpdateMainEditSetPartNumber(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd ) 
	{	pCmdUI->Enable(FALSE); }	
	else
	{	pCmdUI->Enable(TRUE); }	
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnMainEditSelectInvertBoard() 
{
	// TODO: Add your command handler code here
	ExecSelectInvertBoard();
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnUpdateMainEditSelectInvertBoard(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd ) 
	{	pCmdUI->Enable(FALSE); }	
	else
	{	pCmdUI->Enable(TRUE); }	
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnMainEditSelectInvertPanel() 
{
	// TODO: Add your command handler code here
	ExecSelectInvertPanel();
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnUpdateMainEditSelectInvertPanel(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd ) 
	{	pCmdUI->Enable(FALSE); }	
	else
	{	pCmdUI->Enable(TRUE); }	
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnMainEditSelectInvertProject() 
{
	// TODO: Add your command handler code here
	ExecSelectInvertProject();
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnUpdateMainEditSelectInvertProject(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd ) 
	{	pCmdUI->Enable(FALSE); }	
	else
	{	pCmdUI->Enable(TRUE); }	
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnMainEditModeComponent() 
{
	// TODO: Add your command handler code here
	m_MainMode = MENU_MAIN_EDIT_MODE_COMPONENT;	
	m_DrawBoardList = false;
	m_DrawPanelList = false;
	m_DrawComponent = true;
	m_DrawComponentMode = true;
	m_ShowComponentName = true;
	AOIDataCollect.SetManipulateMainMode(MANIPULATE_MAIN_SELECT);
	CEditMainView::RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnUpdateMainEditModeComponent(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if ( MENU_MAIN_EDIT_MODE_COMPONENT == m_MainMode )
	{	pCmdUI->SetCheck(TRUE); }
	else
	{	pCmdUI->SetCheck(FALSE); }
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnMainEditModeBoard() 
{
	// TODO: Add your command handler code here
	CAOIProject *Project = GetActiveProject();
	if ( NULL != Project )
	{	Project->LayoutProjectRegion(); }
	m_MainMode = MENU_MAIN_EDIT_MODE_BOARD;	
	m_DrawBoardList = true;
	m_DrawPanelList = false;
	m_DrawComponent = true;
	m_DrawComponentMode = false;
	m_ShowComponentName = false;	
	AOIDataCollect.SetManipulateMainMode(MANIPULATE_MAIN_SELECT);
	CEditMainView::RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnUpdateMainEditModeBoard(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if ( MENU_MAIN_EDIT_MODE_BOARD == m_MainMode )
	{	pCmdUI->SetCheck(TRUE); }
	else
	{	pCmdUI->SetCheck(FALSE); }
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnMainEditModePanel() 
{
	// TODO: Add your command handler code here
	CAOIProject *Project = GetActiveProject();
	if ( NULL != Project )
	{	Project->LayoutProjectRegion(); }
	m_MainMode = MENU_MAIN_EDIT_MODE_PANEL;	
	m_DrawBoardList = false;
	m_DrawPanelList = true;
	m_DrawComponent = true;
	m_DrawComponentMode = false;
	m_ShowComponentName = false;	
	AOIDataCollect.SetManipulateMainMode(MANIPULATE_MAIN_SELECT);
	CEditMainView::RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnUpdateMainEditModePanel(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if ( MENU_MAIN_EDIT_MODE_PANEL == m_MainMode )
	{	pCmdUI->SetCheck(TRUE); }
	else
	{	pCmdUI->SetCheck(FALSE); }
}
//-------------------------------------------------------------------------------------//
void CEditMainView::RedrawWnd()
{
	CClientDC dc(this);
	HDC hDC = dc.GetSafeHdc();
	HDC hMemDC = m_ImageWndMemDC.GetSafeHdc();
	HDC hMemDC2 = m_ImageWndMemDC2.GetSafeHdc();	
	if ( NULL==hMemDC || NULL==hMemDC2 || NULL==hDC ) 
	{	return; }

	CString str;		
	RECT    WndRect = m_ImageWndRect;
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
	
		::SetTextColor(hMemDC2, 0x00FF00);	
		switch ( m_MainMode )
		{
		case MENU_MAIN_EDIT_MODE_BOARD:
			str = _T("Scope: Board");
			::TextOut(hMemDC2, 8, 8, str, str.GetLength());
			break;
		case MENU_MAIN_EDIT_MODE_PANEL:
			str = _T("Scope: Panel");
			::TextOut(hMemDC2, 8, 8, str, str.GetLength());
			break;
		case MENU_MAIN_EDIT_MODE_COMPONENT:
			str = _T("Scope: Component");
			::TextOut(hMemDC2, 8, 8, str, str.GetLength());
			break;
		}
		DrawComponent(hMemDC2);
		DrawBoardList(hMemDC2);
		DrawPanelList(hMemDC2);
		DrawFieldList(hMemDC2);
		DrawAddRect(hMemDC2);
		DrawCrossLine(hMemDC2);
		DrawCrosshair(hMemDC2);
	}
	::BitBlt(hDC, 0, 0, WndRect.right, WndRect.bottom, hMemDC2, 0, 0, SRCCOPY );	
}
//-------------------------------------------------------------------------------------//
void CEditMainView::CreateBKImage()
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
void CEditMainView::CreateMapImage(bool bTestMap)
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
void CEditMainView::CreateTestMapImage()
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
bool CEditMainView::ExecMainSelectObject()
{
	bool Changed = false;
	MANIPULATE_MAIN_MODE ManiMainMode = AOIDataCollect.GetManipulateMainMode();
	if ( MANIPULATE_MAIN_SELECT != ManiMainMode ) { return true; }

	switch ( m_MainMode )
	{
	case MENU_MAIN_EDIT_MODE_PANEL:	
		Changed = ExecMainSelectPanel();	
		BuildPanelSelected();
		break;
	case MENU_MAIN_EDIT_MODE_BOARD:	
		Changed = ExecMainSelectBoard();	
		BuildBoardSelected();
		break;
	case MENU_MAIN_EDIT_MODE_COMPONENT:
		Changed = ExecMainSelectComponent();	
		BuildComponentSelected();
		break;
	}	
	AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_UPDATE, (LPARAM)(this));	
	//AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_PART_LIST_WND, WPARAM_UPDATE_PART_LIST, TREE_CTRL_UPDATE_STATE);
	return Changed;
}
//-------------------------------------------------------------------------------------//
bool CEditMainView::ExecMainSelectBoard()
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }	

	size_t    i=0;
	TREGION4D Rgn;
	const bool bResultMode = false;
	if ( GetStageSelectRegion(Rgn) == false ) { return false; }
	const double CpX = Rgn.GetCpX();
	const double CpY = Rgn.GetCpY();
	//Rgn.minX = Rgn.maxX = CpX;
	//Rgn.minY = Rgn.maxY = CpY;

	ProjectPtr->ResetProjectActiveIndex();
	if ( AOIDataCollect.CheckMultiSelectMode() == false )
	{	ProjectPtr->SelectProjectAllObjects(false);	}	

	CAOIBoard *pBoard = NULL;
	std::vector<CAOIBoard*> BoardList;
	ProjectPtr->SelectProjectBoardsByStage(Rgn, bResultMode, BoardList);
	const size_t SelCount = BoardList.size();
	if ( 0 == SelCount ) { return false; }	
	for ( i=0; i<SelCount; i++ )
	{
		pBoard = BoardList[i];
		if ( NULL == pBoard ) { continue; }
		pBoard->SetBoardSelected(true);
		pBoard->SelectBoardAllObjects(true);
	}
	pBoard = BoardList[0];
	unsigned int PanelIndex = pBoard->GetBoardPanelIndex_Project();
	unsigned int BoardIndex = pBoard->GetBoardIndex_Project();		
	ProjectPtr->SetProjectActivePanelIndex(PanelIndex);	
	ProjectPtr->SetProjectActiveBoardIndex(BoardIndex);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditMainView::ExecMainSelectPanel()
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }	

	size_t    i=0;
	TREGION4D Rgn;
	const bool bResultMode = false;	
	if ( GetStageSelectRegion(Rgn) == false ) { return false; }
	const double CpX = Rgn.GetCpX();
	const double CpY = Rgn.GetCpY();
	//Rgn.minX = Rgn.maxX = CpX;
	//Rgn.minY = Rgn.maxY = CpY;

	ProjectPtr->ResetProjectActiveIndex();
	if ( AOIDataCollect.CheckMultiSelectMode() == false )
	{	ProjectPtr->SelectProjectAllObjects(false);	}	
	
	CAOIPanel *pPanel = NULL;
	std::vector<CAOIPanel*> PanelList;
	ProjectPtr->SelectProjectPanelsByStage(Rgn, bResultMode, PanelList);
	const size_t SelCount = PanelList.size();
	if ( 0 == SelCount ) { return false; }	
	for ( i=0; i<SelCount; i++ )
	{
		pPanel = PanelList[i];
		if ( NULL == pPanel ) { continue; }
		pPanel->SetPanelSelected(true);
		pPanel->SelectPanelAllObjects(true);
	}
	pPanel = PanelList[0];	
	unsigned int PanelIndex = pPanel->GetPanelIndex_Project();	
	ProjectPtr->SetProjectActivePanelIndex(PanelIndex);		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditMainView::ExecMainSelectComponent()
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }	

	size_t    i=0;
	TREGION4D Rgn;
	const bool bResultMode = false;		
	MULTI_BOARD_CTRL_MODE MultiBoardCtrlMode = AOIDataCollect.GetMultiBoardCtrlMode();
	if ( GetStageSelectRegion(Rgn) == false ) { return false; }
	
	ProjectPtr->ResetProjectActiveIndex();
	if ( AOIDataCollect.CheckMultiSelectMode() == false )	
	{	ProjectPtr->SelectProjectAllObjects(false);	}	

	CAOIComponent *pComponent = NULL;
	std::vector<CAOIComponent*> ComponentList;
	ProjectPtr->SelectProjectComponentsByStage(Rgn, bResultMode, ComponentList);
	ProjectPtr->SelectProjectComponentByMultiBoardCtrlMode(ComponentList, MultiBoardCtrlMode);	
	ProjectPtr->GetProjectComponentSelected(ComponentList);
	const size_t SelCount = ComponentList.size();
	if ( 0 == SelCount ) { return false; }	
	for ( i=0; i<SelCount; i++ )
	{
		pComponent = ComponentList[i];
		if ( NULL == pComponent ) { continue; }
		pComponent->SetComponentSelected(true);
	}
	pComponent = ComponentList[0];
	ProjectPtr->SetProjectActiveComponent(pComponent);	
	return true;
}
//-------------------------------------------------------------------------------------//
inline IMAGE_SIZE CEditMainView::GetMapImageW() const
{
	return m_MapImageW;
}
//-------------------------------------------------------------------------------------//
inline IMAGE_SIZE CEditMainView::GetMapImageH() const
{
	return m_MapImageH;
}
//-------------------------------------------------------------------------------------//
void CEditMainView::PreInitUniFrameBuffer()//預先影像記憶體
{
	int i=0;	
	for ( i=0; i<FRAME_MAX_COUNT; i++ )
	{	JetAPI::InitialUniFrame(m_UniFrameList[i]);	}	
}
//-------------------------------------------------------------------------------------//
void CEditMainView::ReleaseUniFrameBuffer()//釋放影像記憶體	
{
	AOIDataCollect.ReleaseFieldUniFrameList();
	JetAPI::ClearUniFrameList(m_UniFrameList, FRAME_MAX_COUNT);	
}
//-------------------------------------------------------------------------------------//
void CEditMainView::BuildShowImageBuffer()//建立顯示的影像記憶體
{
	const char fnName[] = "CEditMainView::BuildShowImageBuffer";
	CEditMainView::ReleaseShowImageBuffer();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }

	size_t       i=0;
	TUNI_FRAME   UniFrame;
	IMAGE_PTR    ImagePtr = NULL;
	IMAGE_SIZE   ImageW=0, ImageH=0, ImageStep=0, BitCount=0;	
	unsigned int MapIndex = ProjectPtr->GetProjectMapIndex();
	const size_t MaxUniFrameCount = GetMaxFrameCount();
	if ( MapIndex >= MaxUniFrameCount ) { MapIndex = 0; }
	if ( NULL == m_UniFrameList[MapIndex].ImagePtr )
	{	MapIndex = 0;	}	
	UniFrame = m_UniFrameList[MapIndex];
	ImageW    = UniFrame.ImageW;
	ImageH    = UniFrame.ImageH;
	ImageStep = UniFrame.ImageStep;
	BitCount  = UniFrame.BitCount;
	ImagePtr  = UniFrame.ImagePtr;

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
void CEditMainView::ReleaseShowImageBuffer()//釋放顯示影像記憶體	
{
	if ( NULL != m_ShowImagePtr )
	{	JetMemory.free_func(m_ShowImagePtr); }
	m_ShowImageW = 1024;
	m_ShowImageH = 1024;
	m_ShowImageStep = 1024*3;
	m_ShowBitCount = 24;	
}
//-------------------------------------------------------------------------------------//
bool CEditMainView::GetCurrentImage(IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, IMAGE_PTR &ImagePtr)
{
	CAOIProject *ProjectPtr = GetActiveProject();
	bool ShowProjectMapMode = AOIDataCollect.GetProjectMapMode();	
	if ( true == ShowProjectMapMode )
	{
		ImageW    = m_MapImageW;
		ImageH    = m_MapImageH;
		ImageStep = m_MapImageStep;
		BitCount  = m_MapBitCount;
		ImagePtr  = m_MapImagePtr;	
	}
	else
	{
		size_t       i=0;
		unsigned int MapIndex = 0;
		const size_t MaxUniFrameCount = FRAME_MAX_COUNT;
		for ( i=0; i<MaxUniFrameCount; i++ )
		{
			if ( NULL == m_UniFrameList[i].ImagePtr ) { continue; }
			ImageW    = m_UniFrameList[i].ImageW;
			ImageH    = m_UniFrameList[i].ImageH;
			ImageStep = m_UniFrameList[i].ImageStep;
			BitCount  = m_UniFrameList[i].BitCount;
			ImagePtr  = m_UniFrameList[i].ImagePtr;
			break;
		}		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditMainView::GetStageSelectRegion(TREGION4D &StageRgn)//取得選取區域-機台座標
{	
	TPOINT2D  StageCp;
	TPOINT2D  ImageRes;
	IMAGE_SIZE ImageW  = 0;
	IMAGE_SIZE ImageH  = 0;		
	TPOINT2D  WndPt1, ImagePt1, StagePt1;
	TPOINT2D  WndPt2, ImagePt2, StagePt2;

	GetImageInfo(ImageW, ImageH, ImageRes, StageCp);	
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
int CEditMainView::GetMaxFrameCount()
{
	return FRAME_MAX_COUNT;
}
//-------------------------------------------------------------------------------------//
IMAGE_SIZE CEditMainView::GetFrameImageW() const
{
	return m_ShowImageW;
}
//-------------------------------------------------------------------------------------//
IMAGE_SIZE CEditMainView::GetFrameImageH() const
{
	return m_ShowImageH;
}
//-------------------------------------------------------------------------------------//
bool CEditMainView::AdjustCurrentFrames()
{
	//確認是否重新補圖	
	RECT      WndImageRect={0};
	const IMAGE_SIZE ImageW = CEditMainView::GetFrameImageW();
	const IMAGE_SIZE ImageH = CEditMainView::GetFrameImageH();	
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
			BuildShowImageBuffer();
			CreateBKImage();
			RedrawWnd();
		}
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditMainView::FillCurrentFrames(double Ratio)
{
	bool     IsOK;
	size_t   i=0;
	TSIZE2D  Res;
	TPOINT3D Pos;	
	CAMERA_ID CameraID = PRIMARY_CAMERA_ID;
	const size_t MaxFrames = CEditMainView::GetMaxFrameCount();
	IMAGE_SIZE ImageW = AOIDataCollect.GetCameraImageW(CameraID);
	IMAGE_SIZE ImageH = AOIDataCollect.GetCameraImageH(CameraID);
	double     ImageResX = AOIDataCollect.GetCameraResolutionX(CameraID); //取得相機影像解析度
	double     ImageResY = AOIDataCollect.GetCameraResolutionY(CameraID); //取得相機影像解析度 			
	const bool   OfflineMode = AOIDataCollect.GetOfflineMode();	
	const double FOVWum = Ratio*AOIDataCollect.GetFovSizeRealW();
	const double FOVHum = Ratio*AOIDataCollect.GetFovSizeRealH();

	ReleaseUniFrameBuffer();
	ReleaseShowImageBuffer();

	Res.cx = ImageResX;
	Res.cy = ImageResY;
	m_FrameRatio = Ratio;
	ResetImageOffset();
	m_FrameResolution.x = ImageResX;
	m_FrameResolution.y = ImageResY;	
	IsOK = MotionCtrlPtr->GetCurrentPos(Pos.x, Pos.y, Pos.z, OfflineMode);
	if ( false == IsOK )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString()); }
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

	unsigned int ImageIndex = ProjectPtr->GetProjectMapIndex();
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
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditMainView::SwitchFrameImage(bool NextMap)
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }

	TUNI_FRAME   UniFrame;
	const size_t MaxUniFrameCount = GetMaxFrameCount();
	const unsigned int CurrentMapIndex = ProjectPtr->GetProjectMapIndex();
	unsigned int NextMapIndex = ProjectPtr->GetProjectMapIndexNext(CurrentMapIndex);
	if ( true == NextMap )
	{	ProjectPtr->SetProjectMapIndex(NextMapIndex);	}	
	BuildShowImageBuffer();
	CreateBKImage();
	RedrawWnd();	
}
//-------------------------------------------------------------------------------------//
void CEditMainView::UpdateFrameImage()
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr )	{	return;	}	
	FillCurrentFrames(m_FrameRatio);
	BuildShowImageBuffer();
	CreateBKImage();
	RedrawWnd();	
}
//-------------------------------------------------------------------------------------//
void CEditMainView::DrawImage(HDC hDC)
{
	IMAGE_SIZE ImageW = 0;
	IMAGE_SIZE ImageH = 0;
	IMAGE_SIZE ImageStep = 0;
	IMAGE_SIZE BitCount = 0;
	IMAGE_PTR  ImagePtr = NULL;
	const int BltMode = AOIDataCollect.GetStretchBltMode(m_ImageZoom);
	const bool ShowProjectMapMode = AOIDataCollect.GetProjectMapMode();	
	if ( true == ShowProjectMapMode )
	{
		ImageW    = m_MapImageW;
		ImageH    = m_MapImageH;
		ImageStep = m_MapImageStep;
		BitCount  = m_MapBitCount;
		ImagePtr  = m_MapImagePtr;			
	}
	else
	{
		ImageW    = m_ShowImageW;
		ImageH    = m_ShowImageH;
		ImageStep = m_ShowImageStep;
		BitCount  = m_ShowBitCount;
		ImagePtr  = m_ShowImagePtr;
	}
	if ( ImageAPI.DrawImageToDC(hDC, ImageW, ImageH, ImageStep, BitCount, ImagePtr, m_ImageWndRect, m_ImageOffset, m_ImageZoom, 0x00000, BltMode) == false )
	{	return ; }	
}
//-------------------------------------------------------------------------------------//
void CEditMainView::DrawProjectMap(HDC hDC, bool bTestMap)
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
	const int MapIndex = ProjectPtr->GetProjectMapIndex();	
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
void CEditMainView::DrawAddRect(HDC hDC)
{
	if ( false == m_DrawAddRect ) { return; }	

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
void CEditMainView::DrawComponent(HDC hDC)
{
	CAOIProject *Project = GetActiveProject();
	if ( NULL == Project ) { return ; }
	if ( false == m_DrawComponent ) { return; }	

	CString      str;
	size_t       i = 0;		
	COLORREF     Color = 0;
	size_t       SelectedComponents = 0;	
	POINT        Pt={0}, CornerPos[4];
	RECT         Rect={0};
	TPOINT2D     StagePos, ImagePos, WndPt;
	TPOINT2D     StgCornerPos[4], ImgCornerPos[4], WndCornerPos[4];	
	TPOINT2D     ImageStagePos;	
	TPOINT2D     ImageRes;
	IMAGE_SIZE   ImageW = 0;
	IMAGE_SIZE   ImageH = 0;
	TREGION4D    ImageStageRgn;
	TREGION4D    ObjStageRgn, ObjImageRgn;		

	CAOIFd        *FdPtr = NULL;
	CAOIPanel     *PanelPtr = NULL;
	CAOIBoard     *BoardPtr = NULL;
	CAOIModel     *ModelPtr = NULL;
	CAOIComponent *ComponentPtr = NULL;

	TMODEL_DRAW_PARAM DrawParam;
	TPOINT2D ComponentStagePos;
	TPOINT2D StageOffset, CadOffset;

	const RECT   WndRect = m_ImageWndRect;		
	const size_t FdCount = Project->GetProjectFdCount();
	const size_t PanelCount = Project->GetProjectPanelCount();
	const size_t BoardCount = Project->GetProjectBoardCount();
	const size_t ComponentCount = Project->GetProjectComponentCount();	
	const DISTRICT_ID  DistrictID = Project->GetProjectActDistrictID();
	const bool  bDrawRoughLine = Project->CheckProjectComponentUseDrawRoughLine();

	const TSystemParameter &SystemParam = AOIDataCollect.GetSystemParameter();
	const COLORREF  clrOK = SystemParam.m_InspectedResultOKColor;
	const COLORREF  clrNG = SystemParam.m_InspectedResultNGColor;
	const COLORREF  clrSkip = SystemParam.m_InspectedResultSkipColor;
	const COLORREF  clrBypassed = SystemParam.m_InspectedResultBypassColor;
	const COLORREF  clrException = SystemParam.m_InspectedResultExceptionColor;

	GetImageInfo(ImageW, ImageH, ImageRes, ImageStageRgn);
	DrawParam.WndRect = m_ImageWndRect;
	DrawParam.ViewCP.x = DrawParam.ViewCP.y = 0;
	DrawParam.Scale = m_ImageZoom;		
	DrawParam.ViewOffsetX =  m_ImageOffset.x;
	DrawParam.ViewOffsetY =  -m_ImageOffset.y;	
	DrawParam.ResolutionX = ImageRes.x;
	DrawParam.ResolutionY = ImageRes.y;
	DrawParam.ShowEditLine = true;
	
	ImageStagePos.x = ImageStageRgn.GetCpX();
	ImageStagePos.y = ImageStageRgn.GetCpY();

	HPEN hOldPen = NULL;
	COLORREF clrText = ::SetTextColor(hDC, m_clrFdText);
	int BKMode = ::SetBkMode(hDC, TRANSPARENT);

	if ( MENU_MAIN_EDIT_MODE_COMPONENT == m_MainMode )
	{
		HPEN hPenFd = ::CreatePen(PS_SOLID, 1, m_clrFdLine);
		HPEN hPenFdExt = ::CreatePen(PS_SOLID, 1, m_clrFdLineExt);
		hOldPen = (HPEN)(::SelectObject(hDC, hPenFd));

		for ( i=0; i<FdCount; i++ )
		{
			FdPtr = Project->GetProjectFdPtr(i, false);
			if ( NULL == FdPtr ) { continue; }
			if ( DistrictID != FdPtr->GetFdDistrictID() ) { continue; }
			if ( FdPtr->GetFdDeleted() == true ) { continue; }
			StagePos.x = FdPtr->GetFdStagePosX();
			StagePos.y = FdPtr->GetFdStagePosY();
			if ( StagePos.x<ImageStageRgn.minX || StagePos.x>ImageStageRgn.maxX ) { continue; }
			if ( StagePos.y<ImageStageRgn.minY || StagePos.y>ImageStageRgn.maxY ) { continue; }

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
			::SelectObject(hDC, hPenFd);
			DrawRectLine(hDC, CornerPos, 4);
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
			JetAPI::PointsToRect(CornerPos, 4, Rect);		
			DrawCircleLine(hDC, Rect);
			//DrawRectLine(hDC, CornerPos, 4);
		}	
		::SelectObject(hDC, hOldPen);	
		::DeleteObject(hPenFd); hPenFd = NULL;	
		::DeleteObject(hPenFdExt); hPenFdExt = NULL;	
	}
	
	MODEL_TYPE ModelType;
	COMPONENT_TYPE ComponentType;
	bool Bypassed = false;
	HPEN hPenCom = ::CreatePen(PS_SOLID, 1, m_clrComponentLine);
	HPEN hPenComSel = ::CreatePen(PS_SOLID, 7, m_clrSelected);
	HPEN hPenComSelRed = ::CreatePen(PS_SOLID, 3, 0x0000FF);
	HPEN hPenComBypass = ::CreatePen(PS_SOLID, 2, clrBypassed);
	const bool ProjectMapMode = AOIDataCollect.GetProjectMapMode();				
	hOldPen = (HPEN)(::SelectObject(hDC, hPenCom));	
	SelectedComponents = 0;

	if ( false == ProjectMapMode )
	{	m_ShowComponentName = true;	}
	else
	{
		if ( m_ImageZoom < 1.5 ) 
		{	m_ShowComponentName = true; }
		else
		{	m_ShowComponentName = false;	}
	}
	for ( i=0; i<ComponentCount; i++ )
	{	
		ComponentPtr = Project->GetProjectComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }
		if ( DistrictID != ComponentPtr->GetComponentDistrictID() ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }
		if ( ComponentPtr->CheckComponentIsAgent() == true ) { continue; }
		ComponentPtr = ComponentPtr->GetComponentResultPtr();

		ComponentType = ComponentPtr->GetComponentType();
		if ( ComponentPtr->GetComponentSelected() == true ) 
		{	SelectedComponents ++; }		
		if ( AOIDataCollect.CheckComponentTypeVisible(ComponentType) == false )
		{	continue; }
		StagePos.x = ComponentPtr->GetComponentStagePosX();
		StagePos.y = ComponentPtr->GetComponentStagePosY();
		if ( StagePos.x<ImageStageRgn.minX || StagePos.x>ImageStageRgn.maxX ) { continue; }
		if ( StagePos.y<ImageStageRgn.minY || StagePos.y>ImageStageRgn.maxY ) { continue; }
		AOIDataCollect.MapStagePtToCamera(ImageW, ImageH, ImageRes, StagePos, ImageStagePos, ImagePos);
		MapImagePtToWndPt_DBL(ImageW, ImageH, WndRect, m_ImageOffset, m_ImageZoom, ImagePos, WndPt);

		JetAPI::Point2DToPoint(WndPt, Pt);
		if ( Pt.x<0 || Pt.y<0 || Pt.x>WndRect.right || Pt.y>WndRect.bottom ) { continue; }
		
		ComponentPtr->GetComponentBodyStageCornerPos(StgCornerPos);		
		//ComponentPtr->GetComponentRoiStageCornerPos(StgCornerPos);		
		AOIDataCollect.MapStageCornerToCamera(ImageW, ImageH, ImageRes, StgCornerPos, ImageStagePos, ImgCornerPos);		
		MapImagePtToWndPt_DBL(ImageW, ImageH, WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[0], WndCornerPos[0]);
		MapImagePtToWndPt_DBL(ImageW, ImageH, WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[1], WndCornerPos[1]);
		MapImagePtToWndPt_DBL(ImageW, ImageH, WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[2], WndCornerPos[2]);
		MapImagePtToWndPt_DBL(ImageW, ImageH, WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[3], WndCornerPos[3]);

		JetAPI::CornerPt2DToCornerPt(WndCornerPos, CornerPos);
		if ( JetAPI::CheckCornerInRect(CornerPos, WndRect) == false )
		{	continue; }
		JetAPI::CornerPtToRect(CornerPos, Rect);

		if ( true == m_DrawComponentMode )
		{
			bool bDrawModel=true;			
			const int RectW=Rect.right-Rect.left;
			const int RectH=Rect.bottom-Rect.top;
			ModelPtr = ComponentPtr->GetComponentModelPtr();
			if ( RectW<32 || RectH<32 )
			{	bDrawModel = false; }
			if ( true==ProjectMapMode && ModelPtr->GetModelLandCount()>256 )
			{	bDrawModel = false;	}
			
			if ( true == bDrawModel )
			{
				ModelPtr = ComponentPtr->GetComponentModelPtr();
				if ( NULL != ModelPtr )
				{
					ModelType = ModelPtr->GetModelType();
					//if ( MODEL_TYPE_NULL != ModelType )				
					ModelPtr->GetModelAttachedPosStage(ComponentStagePos);
					const double StageOffsetX = (ImageStagePos.x-ComponentStagePos.x);	
					const double StageOffsetY = (ImageStagePos.y-ComponentStagePos.y);

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
					ModelPtr->DrawModel(hDC, DRAW_MODEL_EDIT, DrawParam);
				}
			}
		}

		Bypassed = ComponentPtr->GetComponentBypassed();		
		if ( false == Bypassed )
		{
			switch ( m_MainMode )
			{
			case MENU_MAIN_EDIT_MODE_PANEL:
				PanelPtr = ComponentPtr->GetComponentPanelPtr();
				if ( NULL != PanelPtr )
				{	Bypassed = PanelPtr->GetPanelBypassed();	}
				break;
			case MENU_MAIN_EDIT_MODE_BOARD:
				BoardPtr = ComponentPtr->GetComponentBoardPtr();
				if ( NULL != BoardPtr )
				{	Bypassed = BoardPtr->GetBoardBypassed();	}
				break;
			}		
		}

		if ( ComponentPtr->GetComponentSelected() == true ) 
		{	
			::SelectObject(hDC, hPenComSel);
			if ( true == bDrawRoughLine )
			{
				if ( DrawRectRoughLine(hDC, Rect, m_clrSelected, 4) == true )
				{	continue; }
			}
			DrawRectLine(hDC, CornerPos, 4);

			::SelectObject(hDC, hPenComSelRed);
			DrawRectLine(hDC, CornerPos, 4);			
		}		
		else 
		{
			if ( Bypassed == true )
			{
				Color=clrBypassed;
				::SelectObject(hDC, hPenComBypass);	
			}
			else			
			{
				Color=m_clrComponentLine;
				::SelectObject(hDC, hPenCom);	
			}
			if ( true == bDrawRoughLine )
			{
				if ( DrawRectRoughLine(hDC, Rect, Color, 4) == true )
				{	continue; }
			}
			DrawRectLine(hDC, CornerPos, 4);
		}

		if ( true == m_ShowComponentName )
		{
			str = ComponentPtr->GetComponentName();
			//::TextOut(hDC, Rect.left, Rect.top, str, str.GetLength());
			::TextOut(hDC, Pt.x, Pt.y, str, str.GetLength());
		}
	}
	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPenCom); hPenCom = NULL;	
	::DeleteObject(hPenComSel); hPenComSel = NULL;
	::DeleteObject(hPenComSelRed); hPenComSelRed = NULL;	
	::DeleteObject(hPenComBypass); hPenComBypass = NULL;	
	
	::SetBkMode(hDC, BKMode);
	::SetTextColor(hDC, clrText);

	if ( MENU_MAIN_EDIT_MODE_COMPONENT == m_MainMode )
	{		
		const size_t SelCount = m_SelComponentList.size();
		str.Format(_T("Selected Components:%d[%d]/%d"), SelectedComponents, SelCount, ComponentCount);
		::TextOut(hDC, 8, 32, str, str.GetLength());	
	}
}
//-------------------------------------------------------------------------------------//
void CEditMainView::DrawBoardList(HDC hDC)
{
	CAOIProject *Project = GetActiveProject();
	if ( NULL == Project ) { return ; }
	if ( false == m_DrawBoardList ) { return; }

	CString      str;
	size_t       i = 0;		
	size_t       SelectedBoards = 0;	
	POINT        Pt={0}, CornerPos[4];
	RECT         Rect={0};	
	TPOINT2D     StagePos, ImagePos, WndPt;
	TPOINT2D     StgCornerPos[4], ImgCornerPos[4], WndCornerPos[4];	
	TPOINT2D     ImageStagePos;
	TREGION4D    Region; 	
	TPOINT2D     ImageRes;
	IMAGE_SIZE   ImageW = 0;
	IMAGE_SIZE   ImageH = 0;
	TREGION4D    ImageStageRgn;
	TREGION4D    ObjStageRgn, ObjImageRgn;
	
	unsigned int PanelIndex=0;
	unsigned int BoardIndex=0;
	CAOIFd      *FdPtr = NULL;
	CAOIPanel   *PanelPtr = NULL;
	CAOIBoard   *BoardPtr = NULL;	
	const RECT   WndRect = m_ImageWndRect;	
	DISTRICT_ID  DistrictID = Project->GetProjectActDistrictID();
	const size_t FdCount = Project->GetProjectFdCount();	
	const size_t PanelCount = Project->GetProjectPanelCount();
	const size_t BoardCount = Project->GetProjectBoardCount();		
	
	const TSystemParameter &SystemParam = AOIDataCollect.GetSystemParameter();
	const COLORREF  clrOK = SystemParam.m_InspectedResultOKColor;
	const COLORREF  clrNG = SystemParam.m_InspectedResultNGColor;
	const COLORREF  clrSkip = SystemParam.m_InspectedResultSkipColor;
	const COLORREF  clrBypassed = SystemParam.m_InspectedResultBypassColor;
	const COLORREF  clrException = SystemParam.m_InspectedResultExceptionColor;

	HPEN hPenFd = ::CreatePen(PS_SOLID, 1, m_clrFdLine);
	HPEN hPenFdSel = ::CreatePen(PS_SOLID, 1, m_clrSelected);
	HPEN hOldPen = (HPEN)(::SelectObject(hDC, hPenFd));	
	COLORREF clrText = ::SetTextColor(hDC, 0x2200A0);
	int BKMode = ::SetBkMode(hDC, TRANSPARENT);		
	
	GetImageInfo(ImageW, ImageH, ImageRes, ImageStageRgn);
	ImageStagePos.x = ImageStageRgn.GetCpX();
	ImageStagePos.y = ImageStageRgn.GetCpY();
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = Project->GetProjectFdPtr(i, false);
		if ( NULL == FdPtr ) { continue; }
		if ( FdPtr->GetFdDeleted() == true ) { continue; }
		if ( FdPtr->GetFdBoardIndex_Project() == -1 ) { continue; }

		StagePos.x = FdPtr->GetFdStagePosX();
		StagePos.y = FdPtr->GetFdStagePosY();
		if ( StagePos.x<ImageStageRgn.minX || StagePos.x>ImageStageRgn.maxX ) { continue; }
		if ( StagePos.y<ImageStageRgn.minY || StagePos.y>ImageStageRgn.maxY ) { continue; }

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

		::SelectObject(hDC, hPenFd);	
		DrawRectLine(hDC, CornerPos, 4);
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
		
		::SelectObject(hDC, hPenFdSel);	
		JetAPI::PointsToRect(CornerPos, 4, Rect);		
		DrawCircleLine(hDC, Rect);
		//DrawRectLine(hDC, CornerPos, 4);
	}
	::SetTextColor(hDC, clrText);
	::SelectObject(hDC, hOldPen);	
	::DeleteObject(hPenFd); hPenFd = NULL;	
	::DeleteObject(hPenFdSel); hPenFdSel = NULL;		

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

	HPEN hPenBoard = ::CreatePen(PS_SOLID, 1, m_clrBoardText);	
	HPEN hPenBoardSel = ::CreatePen(PS_SOLID, 2, m_clrSelected);
	HPEN hPenBoardBypass = ::CreatePen(PS_SOLID, 1, clrBypassed);
	hOldPen = (HPEN)(::SelectObject(hDC, hPenBoard));	
	clrText = ::SetTextColor(hDC, 0xFFFFFF);
	SelectedBoards = 0;	
	for ( i=0; i<BoardCount; i++ )
	{	
		BoardPtr = Project->GetProjectBoardPtr(i, false);
		if ( NULL == BoardPtr ) { continue; }
		if ( BoardPtr->GetBoardDeleted() == true ) { continue; }
		if ( BoardPtr->GetBoardSelected() == true )
		{	SelectedBoards ++;	}		
		Region = BoardPtr->GetBoardRgnStage(DistrictID);
		PanelIndex = BoardPtr->GetBoardPanelIndex_Project();
		BoardIndex = BoardPtr->GetBoardIndex_Panel();
		StagePos.x = Region.GetCpX();
		StagePos.y = Region.GetCpY();
		if ( StagePos.x<ImageStageRgn.minX || StagePos.x>ImageStageRgn.maxX ) { continue; }
		if ( StagePos.y<ImageStageRgn.minY || StagePos.y>ImageStageRgn.maxY ) { continue; }
		AOIDataCollect.MapStagePtToCamera(ImageW, ImageH, ImageRes, StagePos, ImageStagePos, ImagePos);
		MapImagePtToWndPt_DBL(ImageW, ImageH, WndRect, m_ImageOffset, m_ImageZoom, ImagePos, WndPt);

		JetAPI::Point2DToPoint(WndPt, Pt);
		if ( Pt.x<0 || Pt.y<0 || Pt.x>WndRect.right || Pt.y>WndRect.bottom ) { continue; }
		
		StgCornerPos[0].x = Region.maxX;	StgCornerPos[0].y = Region.maxY;
		StgCornerPos[1].x = Region.maxX;	StgCornerPos[1].y = Region.minY;
		StgCornerPos[2].x = Region.minX;	StgCornerPos[2].y = Region.minY;
		StgCornerPos[3].x = Region.minX;	StgCornerPos[3].y = Region.maxY;
		AOIDataCollect.MapStageCornerToCamera(ImageW, ImageH, ImageRes, StgCornerPos, ImageStagePos, ImgCornerPos);		
		MapImagePtToWndPt_DBL(ImageW, ImageH, WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[0], WndCornerPos[0]);
		MapImagePtToWndPt_DBL(ImageW, ImageH, WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[1], WndCornerPos[1]);
		MapImagePtToWndPt_DBL(ImageW, ImageH, WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[2], WndCornerPos[2]);
		MapImagePtToWndPt_DBL(ImageW, ImageH, WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[3], WndCornerPos[3]);

		JetAPI::CornerPt2DToCornerPt(WndCornerPos, CornerPos);
		if ( JetAPI::CheckCornerInRect(CornerPos, WndRect) == false )
		{	continue; }

		if ( BoardPtr->GetBoardSelected() == true ) 
		{	::SelectObject(hDC, hPenBoardSel); }
		else if ( BoardPtr->GetBoardBypassed() == true )
		{	::SelectObject(hDC, hPenBoardBypass);	}
		else
		{	::SelectObject(hDC, hPenBoard); }
		DrawRectLine(hDC, CornerPos, 4);

		if ( 1 == PanelCount )
		{	str.Format(_T("%d"), BoardIndex+1); }
		else
		{	str.Format(_T("%d-%d"), PanelIndex+1, BoardIndex+1);	}
		::TextOut(hDC, CornerPos[0].x+4, CornerPos[0].y, str, str.GetLength());
	}
	::SetTextColor(hDC, clrText);
	::SelectObject(hDC, hOldFont);	
	::DeleteObject(hFont);	hFont=NULL;
	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPenBoard); hPenBoard = NULL;	
	::DeleteObject(hPenBoardSel); hPenBoardSel = NULL;			
	::DeleteObject(hPenBoardBypass); hPenBoardBypass = NULL;
	
	::SetBkMode(hDC, BKMode);
	::SetTextColor(hDC, clrText);
	
	if ( MENU_MAIN_EDIT_MODE_BOARD == m_MainMode )
	{		
		const size_t SelCount = m_SelComponentList.size();
		str.Format(_T("Selected Boards:%d/%d, Components:%d"), SelectedBoards, BoardCount, SelCount);		
		::TextOut(hDC, 8, 32, str, str.GetLength());
	}
}
//-------------------------------------------------------------------------------------//
void CEditMainView::DrawPanelList(HDC hDC)
{
	CAOIProject *Project = GetActiveProject();
	if ( NULL == Project ) { return ; }
	if ( false == m_DrawPanelList ) { return; }	

	CString      str;
	size_t       i = 0;		
	size_t       SelectedPanels = 0;	
	POINT        Pt={0}, CornerPos[4];
	RECT         Rect={0};	
	TPOINT2D     StagePos, ImagePos, WndPt;
	TPOINT2D     StgCornerPos[4], ImgCornerPos[4], WndCornerPos[4];	
	TPOINT2D     ImageStagePos;
	TREGION4D    Region; 	
	TPOINT2D     ImageRes;
	IMAGE_SIZE   ImageW = 0;
	IMAGE_SIZE   ImageH = 0;
	TREGION4D    ImageStageRgn;
	TREGION4D    ObjStageRgn, ObjImageRgn;
	
	unsigned int PanelIndex=0;
	CAOIFd      *FdPtr = NULL;
	CAOIPanel   *PanelPtr = NULL;		
	const RECT   WndRect = m_ImageWndRect;		
	const size_t FdCount = Project->GetProjectFdCount();	
	const size_t PanelCount = Project->GetProjectPanelCount();		
	DISTRICT_ID  DistrictID = Project->GetProjectActDistrictID();

	const TSystemParameter &SystemParam = AOIDataCollect.GetSystemParameter();
	const COLORREF  clrOK = SystemParam.m_InspectedResultOKColor;
	const COLORREF  clrNG = SystemParam.m_InspectedResultNGColor;
	const COLORREF  clrSkip = SystemParam.m_InspectedResultSkipColor;
	const COLORREF  clrBypassed = SystemParam.m_InspectedResultBypassColor;
	const COLORREF  clrException = SystemParam.m_InspectedResultExceptionColor;

	HPEN hPenFd = ::CreatePen(PS_SOLID, 1, m_clrFdLine);
	HPEN hPenFdSel = ::CreatePen(PS_SOLID, 1, m_clrSelected);
	HPEN hOldPen = (HPEN)(::SelectObject(hDC, hPenFd));	
	COLORREF clrText = ::SetTextColor(hDC, 0x2200A0);
	int BKMode = ::SetBkMode(hDC, TRANSPARENT);		
	
	GetImageInfo(ImageW, ImageH, ImageRes, ImageStageRgn);
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

		::SelectObject(hDC, hPenFd);	
		DrawRectLine(hDC, CornerPos, 4);
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

		::SelectObject(hDC, hPenFdSel);	
		JetAPI::PointsToRect(CornerPos, 4, Rect);		
		DrawCircleLine(hDC, Rect);
		//DrawRectLine(hDC, CornerPos, 4);
	}
	::SetTextColor(hDC, clrText);
	::SelectObject(hDC, hOldPen);	
	::DeleteObject(hPenFd); hPenFd = NULL;	
	::DeleteObject(hPenFdSel); hPenFdSel = NULL;	
	
	LOGFONT LogFont;
	HFONT   hFont = NULL;
	HFONT   hOldFont = NULL;
	const int FontSize = 48;
	::memset(&LogFont, 0x00, sizeof(LogFont));
	LogFont.lfHeight = FontSize;	
	LogFont.lfWeight = FW_BOLD;
	LogFont.lfCharSet = DEFAULT_CHARSET;
	::_tcscpy(LogFont.lfFaceName, _T("Cambria"));	
	hFont = CreateFontIndirect(&LogFont);
	hOldFont = (HFONT)::SelectObject(hDC, hFont);

	HPEN hPenPanel = ::CreatePen(PS_SOLID, 1, m_clrPanelLine);
	HPEN hPenPanelSel = ::CreatePen(PS_SOLID, 2, m_clrSelected);
	HPEN hPenPanelBypass = ::CreatePen(PS_SOLID, 2, clrBypassed);
	hOldPen = (HPEN)(::SelectObject(hDC, hPenPanel));	
	clrText = ::SetTextColor(hDC, 0xFFFFFF);
	SelectedPanels = 0;
	for ( i=0; i<PanelCount; i++ )
	{	
		PanelPtr = Project->GetProjectPanelPtr(i, false);
		if ( NULL == PanelPtr ) { continue; }
		if ( PanelPtr->GetPanelDeleted() == true ) { continue; }
		if ( PanelPtr->GetPanelSelected() == true ) 
		{	SelectedPanels ++;	}
		Region = PanelPtr->GetPanelRgnStage(DistrictID);
		PanelIndex = PanelPtr->GetPanelIndex_Project();
		StagePos.x = Region.GetCpX();
		StagePos.y = Region.GetCpY();
		if ( StagePos.x<ImageStageRgn.minX || StagePos.x>ImageStageRgn.maxX ) { continue; }
		if ( StagePos.y<ImageStageRgn.minY || StagePos.y>ImageStageRgn.maxY ) { continue; }
		AOIDataCollect.MapStagePtToCamera(ImageW, ImageH, ImageRes, StagePos, ImageStagePos, ImagePos);
		MapImagePtToWndPt_DBL(ImageW, ImageH, WndRect, m_ImageOffset, m_ImageZoom, ImagePos, WndPt);

		JetAPI::Point2DToPoint(WndPt, Pt);
		if ( Pt.x<0 || Pt.y<0 || Pt.x>WndRect.right || Pt.y>WndRect.bottom ) { continue; }
		
		StgCornerPos[0].x = Region.maxX;	StgCornerPos[0].y = Region.maxY;
		StgCornerPos[1].x = Region.maxX;	StgCornerPos[1].y = Region.minY;
		StgCornerPos[2].x = Region.minX;	StgCornerPos[2].y = Region.minY;
		StgCornerPos[3].x = Region.minX;	StgCornerPos[3].y = Region.maxY;
		AOIDataCollect.MapStageCornerToCamera(ImageW, ImageH, ImageRes, StgCornerPos, ImageStagePos, ImgCornerPos);		
		MapImagePtToWndPt_DBL(ImageW, ImageH, WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[0], WndCornerPos[0]);
		MapImagePtToWndPt_DBL(ImageW, ImageH, WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[1], WndCornerPos[1]);
		MapImagePtToWndPt_DBL(ImageW, ImageH, WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[2], WndCornerPos[2]);
		MapImagePtToWndPt_DBL(ImageW, ImageH, WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[3], WndCornerPos[3]);

		JetAPI::CornerPt2DToCornerPt(WndCornerPos, CornerPos);
		if ( JetAPI::CheckCornerInRect(CornerPos, WndRect) == false )
		{	continue; }

		if ( PanelPtr->GetPanelSelected() == true ) 
		{	::SelectObject(hDC, hPenPanelSel);	}
		else if ( PanelPtr->GetPanelBypassed() == true )
		{	::SelectObject(hDC, hPenPanelBypass);	}
		else
		{	::SelectObject(hDC, hPenPanel); }
		DrawRectLine(hDC, CornerPos, 4);

		str.Format(_T("%d"), PanelIndex+1);
		::TextOut(hDC, CornerPos[0].x, CornerPos[0].y, str, str.GetLength());
	}	
	::SetTextColor(hDC, clrText);
	::SelectObject(hDC, hOldFont);
	::DeleteObject(hFont); hFont = NULL;	
	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPenPanel); hPenPanel = NULL;	
	::DeleteObject(hPenPanelSel); hPenPanelSel = NULL;			
	::DeleteObject(hPenPanelBypass); hPenPanelBypass = NULL;			
	
	::SetBkMode(hDC, BKMode);
	::SetTextColor(hDC, clrText);	
	
	if ( MENU_MAIN_EDIT_MODE_PANEL == m_MainMode )
	{		
		const size_t SelCount = m_SelComponentList.size();
		str.Format(_T("Selected Panels:%d/%d, Components:%d"), SelectedPanels, PanelCount, SelCount);		
		::TextOut(hDC, 8, 32, str, str.GetLength());
	}
}
//-------------------------------------------------------------------------------------//
void CEditMainView::DrawFieldList(HDC hDC)
{
	CAOIProject *Project = GetActiveProject();
	if ( NULL == Project ) { return ; }
	if ( false == m_DrawFieldList ) { return; }	
	return;

	CString      str;
	size_t       i = 0;			
	POINT        Pt={0};
	POINT        CornerPos[4]={0};
	RECT         Rect={0};	
	TPOINT2D     StagePos, ImagePos, WndPt;
	TPOINT2D     StgCornerPos[4], ImgCornerPos[4], WndCornerPos[4];	
	TPOINT2D     ImageStagePos;
	TREGION4D    Region; 	
	TPOINT2D     ImageRes;
	IMAGE_SIZE   ImageW = 0;
	IMAGE_SIZE   ImageH = 0;
	TREGION4D    ImageStageRgn;
	TREGION4D    ObjStageRgn, ObjImageRgn;
	
	CAOIField   *FieldPtr = NULL;
	const RECT   WndRect = m_ImageWndRect;		
	const size_t FieldCount = Project->GetProjectInspectionFieldCount();	
	CEditMainView::GetImageInfo(ImageW, ImageH, ImageRes, ImageStageRgn);
	ImageStagePos.x = ImageStageRgn.GetCpX();
	ImageStagePos.y = ImageStageRgn.GetCpY();

	COLORREF clrText = ::SetTextColor(hDC, 0x2200A0);
	int BKMode = ::SetBkMode(hDC, TRANSPARENT);	
	
	HPEN hOldPen = NULL;
	HPEN hPenField = ::CreatePen(PS_SOLID, 1, 0x0000FF);	
	hOldPen = (HPEN)(::SelectObject(hDC, hPenField));		
	for ( i=0; i<FieldCount; i++ )
	{	
		FieldPtr = Project->GetProjectInspectionFieldPtr(i, false);
		if ( NULL == FieldPtr ) { continue; }		
		FieldPtr->GetFieldStageRgn_Inner(Region);
		StagePos.x = Region.GetCpX();
		StagePos.y = Region.GetCpY();
		if ( StagePos.x<ImageStageRgn.minX || StagePos.x>ImageStageRgn.maxX ) { continue; }
		if ( StagePos.y<ImageStageRgn.minY || StagePos.y>ImageStageRgn.maxY ) { continue; }
		AOIDataCollect.MapStagePtToCamera(ImageW, ImageH, ImageRes, StagePos, ImageStagePos, ImagePos);
		MapImagePtToWndPt_DBL(ImageW, ImageH, WndRect, m_ImageOffset, m_ImageZoom, ImagePos, WndPt);

		JetAPI::Point2DToPoint(WndPt, Pt);
		if ( Pt.x<0 || Pt.y<0 || Pt.x>WndRect.right || Pt.y>WndRect.bottom ) { continue; }
		
		StgCornerPos[0].x = Region.maxX;	StgCornerPos[0].y = Region.maxY;
		StgCornerPos[1].x = Region.maxX;	StgCornerPos[1].y = Region.minY;
		StgCornerPos[2].x = Region.minX;	StgCornerPos[2].y = Region.minY;
		StgCornerPos[3].x = Region.minX;	StgCornerPos[3].y = Region.maxY;
		AOIDataCollect.MapStageCornerToCamera(ImageW, ImageH, ImageRes, StgCornerPos, ImageStagePos, ImgCornerPos);		
		MapImagePtToWndPt_DBL(ImageW, ImageH, WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[0], WndCornerPos[0]);
		MapImagePtToWndPt_DBL(ImageW, ImageH, WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[1], WndCornerPos[1]);
		MapImagePtToWndPt_DBL(ImageW, ImageH, WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[2], WndCornerPos[2]);
		MapImagePtToWndPt_DBL(ImageW, ImageH, WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[3], WndCornerPos[3]);

		JetAPI::CornerPt2DToCornerPt(WndCornerPos, CornerPos);
		if ( JetAPI::CheckCornerInRect(CornerPos, WndRect) == false )
		{	continue; }
		
		DrawRectLine(hDC, CornerPos, 4);
	}
	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPenField); hPenField = NULL;	
	
	::SetBkMode(hDC, BKMode);
	::SetTextColor(hDC, clrText);
}
//-------------------------------------------------------------------------------------//
void CEditMainView::DrawCrossLine(HDC hDC)
{
	BOOL bShowCrossLine = TRUE;
	if ( FALSE == bShowCrossLine ) { return ; }

	const RECT   WndRect = m_ImageWndRect;
	const int    nCpX = (WndRect.left+WndRect.right)/2;
	const int    nCpY = (WndRect.top+WndRect.bottom)/2;

	HPEN hPen = ::CreatePen(PS_SOLID, 1, 0x8080FF);
	HPEN hOldPen = NULL;

	hOldPen = (HPEN)::SelectObject(hDC, hPen);
	::MoveToEx(hDC, WndRect.left, nCpY, NULL);
	::LineTo(hDC, WndRect.right, nCpY);

	::MoveToEx(hDC, nCpX, WndRect.top, NULL);
	::LineTo(hDC, nCpX, WndRect.bottom);

	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPen); hPen = NULL;
	return;
}
//-------------------------------------------------------------------------------------//
inline void CEditMainView::DrawCrosshair(HDC hDC)//十字線
{
	const POINT &Pt=m_MousePosCurrent;	
	const RECT &WndRect=m_ImageWndRect;
	MANIPULATE_MAIN_MODE ManiMode=AOIDataCollect.GetManipulateMainMode();
	if ( MANIPULATE_MAIN_ADD != ManiMode ) { return ; }
	ImageAPI.DrawCrosshair(hDC, Pt, WndRect, 0xA0A0A0);	
	return;
}
//-------------------------------------------------------------------------------------//
inline void CEditMainView::DrawRect(HDC hDC, const RECT &Rect)
{
	::MoveToEx(hDC, Rect.left, Rect.top, NULL);
	::LineTo(hDC, Rect.right, Rect.top);
	::LineTo(hDC, Rect.right, Rect.bottom);
	::LineTo(hDC, Rect.left, Rect.bottom);
	::LineTo(hDC, Rect.left, Rect.top);
}
//-------------------------------------------------------------------------------------//
inline void CEditMainView::DrawRectLine(HDC hDC, const POINT pt[], size_t num)
{
	if ( 4 == num )
	{
		::MoveToEx(hDC, pt[0].x, pt[0].y, NULL);		
		::LineTo(hDC, pt[1].x, pt[1].y);
		::LineTo(hDC, pt[2].x, pt[2].y);
		::LineTo(hDC, pt[3].x, pt[3].y);
		::LineTo(hDC, pt[0].x, pt[0].y);
	}
	else
	{
		size_t i=0;
		for ( i=0; i<num; i++ )
		{
			if ( 0 == i ) 
			{	::MoveToEx(hDC, pt[i].x, pt[i].y, NULL);	}
			else
			{	::LineTo(hDC, pt[i].x, pt[i].y);	}
		}
		if ( 1 == num )
		{	::LineTo(hDC, pt[0].x, pt[0].y);	}
	}
}
//-------------------------------------------------------------------------------------//
inline bool CEditMainView::DrawRectRoughLine(HDC hDC, const RECT &Rect, COLORREF color, int Gap)
{
	bool bDraw=true;	
	const int RectW=Rect.right-Rect.left;
	const int RectH=Rect.bottom-Rect.top;
	const int RectX=(Rect.right+Rect.left)/2;
	const int RectY=(Rect.bottom+Rect.top)/2;
	if ( RectW < Gap )
	{
		if ( RectH < Gap )
		{	::SetPixel(hDC, RectX, RectY, color);	}
		else
		{
			::MoveToEx(hDC, RectX, Rect.top, NULL);
			::LineTo(hDC, RectX, Rect.bottom);
		}
	}
	else
	{
		if ( RectH < Gap )
		{
			::MoveToEx(hDC, Rect.left, RectY, NULL);
			::LineTo(hDC, Rect.right, RectY);
		}
		else
		{	bDraw = false;	}
	}
	return bDraw;
}
//-------------------------------------------------------------------------------------//
inline void CEditMainView::DrawCircleLine(HDC hDC, const RECT &Rect)
{
	int x = (Rect.left);
	int y = (Rect.top+Rect.bottom)/2;
	::MoveToEx(hDC, x, y, NULL);		
	::ArcTo(hDC, Rect.left, Rect.top, Rect.right, Rect.bottom, x, y, x, y);
}
//-------------------------------------------------------------------------------------//
bool CEditMainView::ExecMoveToStage()
{
	if ( this->GetLockUIWnd() == true ) { return true; }
	if ( MotionCtrlPtr->WaitForMotionStop() == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return false;
	}
	
	bool   IsOK = true;
	double PosX=0, PosY=0, PosZ=0;
	const bool OfflineMode = GetOfflineMode();	
	IsOK = MotionCtrlPtr->GetCurrentPos(PosX, PosY, PosZ, OfflineMode);
	if ( IsOK == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return false;
	}	
	if ( true == OfflineMode )
	{	return ExecUpdateFov(PosX, PosY, PosZ);	}

	return ExecGrabFov(PosX, PosY, PosZ);
}
//-------------------------------------------------------------------------------------//
bool CEditMainView::ExecShowWndPosition()
{	
	const double PosX = AOIDataCollect.GetFovPositionX();
	const double PosY = AOIDataCollect.GetFovPositionY();	
	const double TargetOffsetX = AOIDataCollect.GetFovTargetOffsetX();
	const double TargetOffsetY = AOIDataCollect.GetFovTargetOffsetY();

	AOIDataCollect.ResetFovTargetParam();
	
	TPOINT2D PosCad;
	TPOINT2D FovStage;
	TPOINT2D ImageRes;
	TPOINT2D FovOffset;
	TPOINT2D PosStage(TargetOffsetX, TargetOffsetY);		
	const bool ProjectMapMode = AOIDataCollect.GetProjectMapMode();
	
	if ( true == ProjectMapMode )
	{	
		ImageRes = m_MapResolution;
		FovStage.x = m_MapStageRgn.GetCpX();
		FovStage.y = m_MapStageRgn.GetCpY();
	}
	else
	{	
		ImageRes = m_FrameResolution;	
		FovStage.x = m_FrameStageRgn.GetCpX();
		FovStage.y = m_FrameStageRgn.GetCpY();
	}	

	//想要的位置與目前圖像的機台位置偏差量
	FovOffset.x = PosX-FovStage.x;
	FovOffset.y = PosY-FovStage.y;
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
bool CEditMainView::ExecGrabFov(double PosX, double PosY, double PosZ)
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
bool CEditMainView::ExecUpdateFov(double PosX, double PosY, double PosZ)
{
	const bool ProjectMapMode = AOIDataCollect.GetProjectMapMode();
	if ( true == ProjectMapMode )
	{	return ExecUpdateFov_Map(PosX, PosY, PosZ);	}
	else
	{	return ExecUpdateFov_Frame(PosX, PosY, PosZ);	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditMainView::ExecUpdateFov_Map(double PosX, double PosY, double PosZ)
{
	TPOINT2D     ImageRes;
	TPOINT2D     StagePos;
	double       ScaleMode = 4;
	CAOIProject *ProjectPtr = GetActiveProject();
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
	const double ZoomX = FovMinW/FovSizeW;
	const double ZoomY = FovMinH/FovSizeH;
	const double ZoomNeed = MAX(ZoomX, ZoomY);	
	const double ZoomNeedUsed = JetAPI::AdjustValue(ZoomNeed, 0.5);

	//視窗轉成影像
	IMAGE_SIZE ImageW = 0;
	IMAGE_SIZE ImageH = 0;
	IMAGE_SIZE ImageStep = 0;
	IMAGE_SIZE BitCount = 0;
	IMAGE_PTR  ImagePtr = NULL;

	TPOINT2D WndPt1, WndPt2;
	TPOINT2D ImagePt1, ImagePt2;
	TPOINT2D StagePt1, StagePt2;

	WndPt1.x = m_ImageWndRect.left;
	WndPt1.y = m_ImageWndRect.top;
	WndPt2.x = m_ImageWndRect.right;
	WndPt2.y = m_ImageWndRect.bottom;

	StagePos.x = PosX;
	StagePos.y = PosY;
	GetImageInfo(ImageW, ImageH, ImageRes, StagePos);	
	m_ImageZoom = ZoomNeedUsed;
	ImageAPI.MapWndPtToImagePt_DBL(ImageW, ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, WndPt1, ImagePt1);	
	ImageAPI.MapWndPtToImagePt_DBL(ImageW, ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, WndPt2, ImagePt2);	

	//影像轉成機台
	AOIDataCollect.MapCameraPtToStage(ImageW, ImageH, ImageRes, ImagePt1, StagePos, StagePt1);
	AOIDataCollect.MapCameraPtToStage(ImageW, ImageH, ImageRes, ImagePt2, StagePos, StagePt2);
	
	double FovViewW = ::fabs(StagePt1.x-StagePt2.x);//FovSizeWd4;
	double FovViewH = ::fabs(StagePt1.y-StagePt2.y);//FovSizeHd4;

	if ( NULL != ProjectPtr )
	{	ScaleMode = ProjectPtr->GetProjectMapScaleMode(); }

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
		m_ImageZoom = m_ImageZoom/ScaleMode;		
	}	
	//if ( m_ImageZoom > 1.0 )
	//{	m_ImageZoom = 1.0; }	
	
	CalcImageOffset();		
	CreateBKImage();
	RedrawWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditMainView::ExecUpdateFov_Frame(double PosX, double PosY, double PosZ)
{
	TPOINT2D     StagePos;
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

	StagePos.x = PosX;
	StagePos.y = PosY;
	if ( m_ImageZoom < FovZoomNeedUsed )
	{	m_ImageZoom = FovZoomNeedUsed; }
	ImageRes.x = AOIDataCollect.GetCameraResolutionX(PRIMARY_CAMERA_ID);
	ImageRes.y = AOIDataCollect.GetCameraResolutionY(PRIMARY_CAMERA_ID);
	if ( GetCurrentImage(ImageW, ImageH, ImageStep, BitCount, ImagePtr)==true && m_ImageWndRect.right>0 )
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
		AOIDataCollect.MapCameraPtToStage(ImageW, ImageH, ImageRes, ImagePt1, StagePos, StagePt1);
		AOIDataCollect.MapCameraPtToStage(ImageW, ImageH, ImageRes, ImagePt2, StagePos, StagePt2);
	
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

	BuildShowImageBuffer();
	CreateBKImage();
	RedrawWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
inline void CEditMainView::SetDrawAddRect(bool Draw)
{
	if ( true == Draw )
	{	Draw = true; }
	else 
	{	Draw = false; }
	m_DrawAddRect = Draw;
}
//-------------------------------------------------------------------------------------//
void CEditMainView::CloseProject(bool ResetData)
{
	m_BoardPtr_Add = NULL;
	if ( true == ResetData )
	{
		m_MainMode = MENU_MAIN_EDIT_MODE_COMPONENT;
		m_ImageZoom = 0.25;	

		m_FrameRatio = 1.0;
		m_FrameResolution.x = 10;
		m_FrameResolution.y = 10;	
	}
	m_ProjectPtr = NULL;
	m_MapZoom = 1.0;
	m_MapImageW = 1024;
	m_MapImageH = 1024;
	m_MapImageStep = 1024;
	m_MapBitCount = 8;
	m_MapImagePtr = NULL;
	m_MapResolution.x = 10;
	m_MapResolution.y = 10;	
	ResetImageOffset();
	m_SelPanelList.clear();
	m_SelBoardList.clear();
	m_SelComponentList.clear();	

	m_ClonePanelList.clear();
	m_CloneBoardList.clear();
	m_CloneComponentList.clear();
	
	m_LoadOfflineParam = false;	
	ReleaseUniFrameBuffer();
	ReleaseShowImageBuffer();
	CreateBKImage();
	CreateMapImage();
	if ( m_ProjectMarkWnd.GetSafeHwnd() != NULL )
	{	
		m_ProjectMarkWnd.SetProjectPtr(NULL);
		if ( m_ProjectMarkWnd.IsWindowVisible() == TRUE )
		{	m_ProjectMarkWnd.ShowWindow(SW_HIDE); }
	}

}
//-------------------------------------------------------------------------------------//
void CEditMainView::SwitchProject()//切換專案
{
	AOIDataCollect.SetShowBarcodeList(false);
	CloseProject(false);
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL == ProjectPtr )	{	return;	}	
	const bool bLock = this->GetLockUIWnd();	
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();	
	const bool ProjectMapMode = AOIDataCollect.GetProjectMapMode();
	const double ScaleMode = ProjectPtr->GetProjectMapScaleMode();//使用專案指定比例	
	const unsigned int CurrentMapIndex = ProjectPtr->GetProjectMapIndex();

	m_ProjectPtr = ProjectPtr;		
	ProjectPtr->GetProjectMapInfo(m_MapResolution, m_MapCadRgn, m_MapStageRgn);	
	ProjectPtr->GetProjectMapCalcRgn(m_MapStageRgn);
	ProjectPtr->GetProjectMapPtr(CurrentMapIndex, m_MapImageW, m_MapImageH, m_MapImageStep, m_MapBitCount, m_MapImagePtr);	
	ProjectPtr->CreateProjectMapShowPtr(CurrentMapIndex, m_MapImagePtr, false);

	if ( false == ProjectMapMode )
	{	m_ImageZoom = 1.00;	}
	else
	{	m_ImageZoom = 1.00/ScaleMode;	}
	CalcImageOffset();
	ProjectPtr->UnSelectProjectModel();
	BuildPanelSelected();
	BuildBoardSelected();
	BuildComponentSelected();
	CreateMapImage();
}
//-------------------------------------------------------------------------------------//
void  CEditMainView::SwitchProjectDistrictID(bool bRedraw)//切換專案
{	
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL == ProjectPtr )	{	return;	}			

	ProjectPtr->GetProjectMapInfo(m_MapResolution, m_MapCadRgn, m_MapStageRgn);	
	ProjectPtr->GetProjectMapCalcRgn(m_MapStageRgn);
	if ( true == bRedraw )
	{	RedrawWnd(); }
	return;
}
//-------------------------------------------------------------------------------------//
inline CAOIProject* CEditMainView::GetActiveProject()
{
	return m_ProjectPtr;
}
//-------------------------------------------------------------------------------------//
void CEditMainView::SwitchMapImage(bool NextMap)//切換專案底圖
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }

	IMAGE_SIZE   MapImageW = 0;
	IMAGE_SIZE   MapImageH = 0;
	IMAGE_SIZE   MapImageStep = 0;
	IMAGE_SIZE   MapBitCount = 0;
	IMAGE_PTR    MapImagePtr = NULL;
	bool        bForce=true;
	const unsigned int CurrentMapIndex = ProjectPtr->GetProjectMapIndex();
	unsigned int NextMapIndex = CurrentMapIndex;
	if ( true == NextMap )
	{	
		bForce = false;
		NextMapIndex = ProjectPtr->GetProjectMapIndexNext(CurrentMapIndex);	
	}
	ProjectPtr->GetProjectMapPtr(NextMapIndex, MapImageW, MapImageH, MapImageStep, MapBitCount, MapImagePtr);
	ProjectPtr->CreateProjectMapShowPtr(NextMapIndex, MapImagePtr, bForce);
	if ( NULL == MapImagePtr )
	{	
		NextMapIndex = 0;
		ProjectPtr->GetProjectMapPtr(NextMapIndex, MapImageW, MapImageH, MapImageStep, MapBitCount, MapImagePtr);
		ProjectPtr->CreateProjectMapShowPtr(NextMapIndex, MapImagePtr, bForce);
	}		
	ProjectPtr->SetProjectMapIndex(NextMapIndex);
	m_MapImageW = MapImageW;
	m_MapImageH = MapImageH;
	m_MapImageStep = MapImageStep;
	m_MapBitCount = MapBitCount;
	m_MapImagePtr = MapImagePtr;
	CreateBKImage();
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CEditMainView::BackupViewParam()//備份顯示參數
{
	double       ScaleMode = 1;
	double       ImageZoom = m_ImageZoom;
	CAOIProject *ProjectPtr = GetActiveProject();
	const bool   ProjectMapMode = AOIDataCollect.GetProjectMapMode();	
	if ( NULL != ProjectPtr )
	{	ScaleMode = ProjectPtr->GetProjectMapScaleMode(); }
	if ( true == ProjectMapMode )
	{	ImageZoom *= ScaleMode;	}
	AOIDataCollect.SetViewImageZoom(ImageZoom);
}
//-------------------------------------------------------------------------------------//
void CEditMainView::RestoreViewParam()//恢復顯示參數
{
	m_ImageZoom = AOIDataCollect.GetViewImageZoom();
}
//-------------------------------------------------------------------------------------//
void CEditMainView::CalcFovPosition()//計算FOV的位置
{
	if ( GetLockUIWnd() == true ) { return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }
	
	TPOINT2D   StageCp;
	TPOINT2D   ImageRes;
	IMAGE_SIZE ImageW=0;
	IMAGE_SIZE ImageH=0;
	TPOINT2D   CadOffset, StageOffset;
	double ImageZoom = m_ImageZoom;			
	const double ImageOffsetX = m_ImageOffset.x;
	const double ImageOffsetY = m_ImageOffset.y;
	CEditMainView::GetImageInfo(ImageW, ImageH, ImageRes, StageCp);	
	CadOffset.x =  -ImageOffsetX*ImageRes.x*ImageZoom;
	CadOffset.y =   ImageOffsetY*ImageRes.y*ImageZoom;
	AOIDataCollect.MapCadOffsetPtToStage(CadOffset, StageOffset);
	const double NewStagePosX = StageCp.x+StageOffset.x;
	const double NewStagePosY = StageCp.y+StageOffset.y;
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();	
	MotionCtrlPtr->XYMoveTo(NewStagePosX, NewStagePosY, OfflineMode);
	MotionCtrlPtr->WaitForMotionStop();
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditMainView::CalcImageOffset()//計算影像顯示移動值
{	
	bool   IsOK = true;
	double PosX=0, PosY=0, PosZ=0;
	TPOINT2D CadOffset, StageOffset;
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();			
	IsOK = MotionCtrlPtr->GetCurrentPos(PosX, PosY, PosZ, OfflineMode);
	if ( IsOK == false )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());	}
	
	TPOINT2D     StageCp;
	TPOINT2D     ImageRes;	
	IMAGE_SIZE   ImageW=0;
	IMAGE_SIZE   ImageH=0;	
	const double ImageZoom = m_ImageZoom;	
	const double NewStagePosX = PosX;
	const double NewStagePosY = PosY;

	GetImageInfo(ImageW, ImageH, ImageRes, StageCp);	
	StageOffset.x = NewStagePosX-StageCp.x;
	StageOffset.y = NewStagePosY-StageCp.y;
	AOIDataCollect.MapStageOffsetPtToCad(StageOffset, CadOffset);
	m_ImageOffset.x = -CadOffset.x/ImageRes.x;
	m_ImageOffset.y =  CadOffset.y/ImageRes.y;
	m_ImageOffset.x /= ImageZoom;
	m_ImageOffset.y /= ImageZoom;
}
//-------------------------------------------------------------------------------------//
void CEditMainView::ResetImageOffset()//復歸顯示移動值	
{
	m_ImageOffset.x = 0;
	m_ImageOffset.y = 0;
}
//-------------------------------------------------------------------------------------//
bool CEditMainView::ExecSelectInvertBoard()
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }

	bool Exected = false;	
	CAOIBoard *pBoard = ProjectPtr->GetProjectActiveBoard();
	if ( NULL == pBoard ) { return false; }
	switch ( m_MainMode )
	{
	case MENU_MAIN_EDIT_MODE_BOARD:		
		break;
	case MENU_MAIN_EDIT_MODE_PANEL:		
		break;
	case MENU_MAIN_EDIT_MODE_COMPONENT:
		Exected = true;
		pBoard->InvertSelectBoardComponent();
		BuildComponentSelected();
		break;
	}
	
	if ( false == Exected ) { return true; }	
	CAOIComponent *pComponent = ProjectPtr->GetProjectComponentPtrBySelected();
	ProjectPtr->SetProjectActiveComponent(pComponent);	
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_PART_LIST_WND, WPARAM_UPDATE_PART_LIST, TREE_CTRL_UPDATE_STATE);
	CEditMainView::RedrawWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditMainView::ExecSelectInvertPanel()
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }

	bool Exected = false;	
	CAOIPanel *pPanel = ProjectPtr->GetProjectActivePanel();
	if ( NULL == pPanel ) { return false; }
	switch ( m_MainMode )
	{
	case MENU_MAIN_EDIT_MODE_BOARD:		
		break;
	case MENU_MAIN_EDIT_MODE_PANEL:		
		break;
	case MENU_MAIN_EDIT_MODE_COMPONENT:
		Exected = true;
		pPanel->InvertSelectPanelComponent();
		BuildComponentSelected();
		break;
	}
	if ( false == Exected ) { return true; }	
	CAOIComponent *pComponent = ProjectPtr->GetProjectComponentPtrBySelected();
	ProjectPtr->SetProjectActiveComponent(pComponent);	
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_PART_LIST_WND, WPARAM_UPDATE_PART_LIST, TREE_CTRL_UPDATE_STATE);
	CEditMainView::RedrawWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditMainView::ExecSelectInvertProject()
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }

	bool Exected = false;	
	const bool    bUpdate = true;
	CAOIPanel     *pPanel = NULL;
	CAOIBoard     *pBoard = NULL;
	CAOIComponent *pComponent = NULL;
	ProjectPtr->SetProjectActiveComponent(pComponent);	
	switch ( m_MainMode )
	{
	case MENU_MAIN_EDIT_MODE_BOARD:
		Exected = true;
		ProjectPtr->InvertSelectProjectBoard(bUpdate);
		pBoard = ProjectPtr->GetProjectBoardPtrBySelected();
		ProjectPtr->SetProjectActiveBoard(pBoard);	
		BuildBoardSelected();
		break;
	case MENU_MAIN_EDIT_MODE_PANEL:
		Exected = true;
		ProjectPtr->InvertSelectProjectPanel(bUpdate);		
		pPanel = ProjectPtr->GetProjectPanelPtrBySelected();
		ProjectPtr->SetProjectActivePanel(pPanel);	
		BuildPanelSelected();
		break;
	case MENU_MAIN_EDIT_MODE_COMPONENT:
		Exected = true;
		ProjectPtr->InvertSelectProjectComponent();
		pComponent = ProjectPtr->GetProjectComponentPtrBySelected();
		ProjectPtr->SetProjectActiveComponent(pComponent);	
		BuildComponentSelected();
		break;
	}	
	if ( false == Exected ) { return true; }		
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_PART_LIST_WND, WPARAM_UPDATE_PART_LIST, TREE_CTRL_UPDATE_STATE);
	CEditMainView::RedrawWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditMainView::ExecDeleteSelected()
{
	bool Exected = false;	
	switch ( m_MainMode )
	{
	case MENU_MAIN_EDIT_MODE_PANEL:
		Exected = ExecDeleteSelectedPanel();		
		break;
	case MENU_MAIN_EDIT_MODE_BOARD:
		Exected = ExecDeleteSelectedBoard();
		break;
	case MENU_MAIN_EDIT_MODE_COMPONENT:
		Exected = ExecDeleteSelectedComponent();
		break;
	}
	AOIDataCollect.SetManipulateMainMode(MANIPULATE_MAIN_SELECT);
	return Exected;
}
//-------------------------------------------------------------------------------------//
bool CEditMainView::ExecDeleteSelectedPanel()
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }
	if ( MENU_MAIN_EDIT_MODE_PANEL != m_MainMode ) { return false; }
	if ( AOIDataCollect.OperateLevelEditFuncDelPanel() == false ) { return false; }

	CString str, str2;
	CString strCount = AOIDataDefine.GetCountText();
	std::vector<CAOIPanel*> SelPanelList;
	ProjectPtr->GetProjectPanelSelected(SelPanelList);
	const size_t SelCount = SelPanelList.size();
	if ( 0 == SelCount ) { return false; }
	str2 = _T("Do you want to delete the panels");
	str2 = this->LoadMultiLanguageString(str2, str2);
	str.Format(_T("%s [%s:%d] ?"), str2, strCount, SelCount);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO )
	{	return false; }

	m_SelPanelList.clear();		
	AOIDataCollect.ReleaseModelUniFrameList();
	LogOperCtrl.SaveLogProjectPanelSelectedDelete(ProjectPtr);
	ProjectPtr->DeleteProjectPanelSelected();
	ProjectPtr->ResetProjectActiveIndex();
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_PART_LIST_WND, WPARAM_BUILD_PART_LIST, LPARAM_BUILD_DOCK_LIST_ALL);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_RESULT_LIST_WND, WPARAM_BUILD_RESULT_LIST, NULL);
	CEditMainView::RedrawWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditMainView::ExecDeleteSelectedBoard()
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }
	if ( MENU_MAIN_EDIT_MODE_BOARD != m_MainMode ) { return false; }
	if ( AOIDataCollect.OperateLevelEditFuncDelBoard() == false ) { return false; }

	CString str, str2;
	CString strCount = AOIDataDefine.GetCountText();
	std::vector<CAOIBoard*> SelBoardList;
	ProjectPtr->GetProjectBoardSelected(SelBoardList);
	const size_t SelCount = SelBoardList.size();
	if ( 0 == SelCount ) { return false; }
	str2 = _T("Do you want to delete the boards");
	str2 = this->LoadMultiLanguageString(str2, str2);
	str.Format(_T("%s [%s:%d] ?"), str2, strCount, SelCount);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO )
	{	return false; }

	m_SelBoardList.clear();
	AOIDataCollect.ReleaseModelUniFrameList();
	LogOperCtrl.SaveLogProjectBoardSelectedDelete(ProjectPtr);
	ProjectPtr->DeleteProjectBoardSelected();	
	ProjectPtr->ResetProjectActiveIndex();
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_PART_LIST_WND, WPARAM_BUILD_PART_LIST, LPARAM_BUILD_DOCK_LIST_ALL);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_RESULT_LIST_WND, WPARAM_BUILD_RESULT_LIST, NULL);	
	CEditMainView::RedrawWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditMainView::ExecDeleteSelectedComponent()
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }
	if ( MENU_MAIN_EDIT_MODE_COMPONENT != m_MainMode ) { return false; }
	if ( AOIDataCollect.OperateLevelEditFuncDelComponent() == false ) { return false; }

	CString str, str2;
	CString strCount = AOIDataDefine.GetCountText();
	std::vector<CAOIComponent*> SelComponentList;
	ProjectPtr->GetProjectComponentSelected(SelComponentList);
	const size_t SelCount = SelComponentList.size();
	if ( 0 == SelCount ) { return false; }
	str2 = _T("Do you want to delete the components");
	str2 = this->LoadMultiLanguageString(str2, str2);
	str.Format(_T("%s [%s:%d] ?"), str2, strCount, SelCount);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO )
	{	return false; }

	m_SelComponentList.clear();
	AOIDataCollect.ReleaseModelUniFrameList();
	LogOperCtrl.SaveLogProjectComponentSelectedDelete(ProjectPtr);
	ProjectPtr->DeleteProjectComponentSelected();		
	ProjectPtr->ResetProjectActiveIndex();
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_PART_LIST_WND, WPARAM_BUILD_PART_LIST, LPARAM_BUILD_DOCK_LIST_ALL);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_RESULT_LIST_WND, WPARAM_BUILD_RESULT_LIST, NULL);	
	CEditMainView::RedrawWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditMainView::ExecMove(double dX, double dY)
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }

	bool Exected = false;	
	switch ( m_MainMode )
	{
	case MENU_MAIN_EDIT_MODE_BOARD:
		Exected = true;			
		ProjectPtr->MoveProjectBoardSelected(dX, dY);
		LogOperCtrl.SaveLogProjectBoardSelectedMove(ProjectPtr, dX, dY);
		break;
	case MENU_MAIN_EDIT_MODE_PANEL:
		Exected = true;		
		ProjectPtr->MoveProjectPanelSelected(dX, dY);
		LogOperCtrl.SaveLogProjectPanelSelectedMove(ProjectPtr, dX, dY);
		break;
	case MENU_MAIN_EDIT_MODE_COMPONENT:
		Exected = true;
		ProjectPtr->MoveProjectComponentSelected(dX, dY);
		LogOperCtrl.SaveLogProjectComponentSelectedMove(ProjectPtr, dX, dY);
		break;
	}	
	if ( false == Exected ) { return true; }
	BuildObjectSelected();
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_PART_LIST_WND, WPARAM_UPDATE_PART_LIST, TREE_CTRL_UPDATE_TEXT);
	CEditMainView::RedrawWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditMainView::ExecMirrorPosX()
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }

	bool Exected = false;	
	switch ( m_MainMode )
	{
	case MENU_MAIN_EDIT_MODE_BOARD:
		Exected = true;			
		ProjectPtr->MirrorXProjectBoardSelected();
		LogOperCtrl.SaveLogProjectBoardSelectedMirrorX(ProjectPtr);
		break;
	case MENU_MAIN_EDIT_MODE_PANEL:
		Exected = true;		
		ProjectPtr->MirrorXProjectPanelSelected();
		LogOperCtrl.SaveLogProjectPanelSelectedMirrorX(ProjectPtr);
		break;
	case MENU_MAIN_EDIT_MODE_COMPONENT:
		Exected = true;
		ProjectPtr->MirrorXProjectComponentSelected();
		LogOperCtrl.SaveLogProjectComponentSelectedMirrorX(ProjectPtr);
		break;
	}
	
	if ( false == Exected ) { return true; }
	BuildObjectSelected();
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_PART_LIST_WND, WPARAM_UPDATE_PART_LIST, TREE_CTRL_UPDATE_TEXT);
	CEditMainView::RedrawWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditMainView::ExecMirrorPosY()
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }

	bool Exected = false;	
	switch ( m_MainMode )
	{
	case MENU_MAIN_EDIT_MODE_BOARD:
		Exected = true;			
		ProjectPtr->MirrorYProjectBoardSelected();
		LogOperCtrl.SaveLogProjectBoardSelectedMirrorY(ProjectPtr);
		break;
	case MENU_MAIN_EDIT_MODE_PANEL:
		Exected = true;		
		ProjectPtr->MirrorYProjectPanelSelected();
		LogOperCtrl.SaveLogProjectPanelSelectedMirrorY(ProjectPtr);
		break;
	case MENU_MAIN_EDIT_MODE_COMPONENT:
		Exected = true;
		ProjectPtr->MirrorYProjectComponentSelected();
		LogOperCtrl.SaveLogProjectComponentSelectedMirrorY(ProjectPtr);
		break;
	}	
	if ( false == Exected ) { return true; }
	BuildObjectSelected();
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_PART_LIST_WND, WPARAM_UPDATE_PART_LIST, TREE_CTRL_UPDATE_TEXT);
	CEditMainView::RedrawWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnMainEditView1x1() 
{
	// TODO: Add your command handler code here
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	const bool ProjectMapMode = AOIDataCollect.GetProjectMapMode();
	const double ScaleMode = ProjectPtr->GetProjectMapScaleMode();//使用專案指定比例
	CalcFovPosition();//計算目前FOV的位置
	if ( true == ProjectMapMode )
	{	m_ImageZoom = 1.00/ScaleMode; }
	else
	{	m_ImageZoom = 1.00; }
	CalcImageOffset();//計算機台在目前影像的偏差量
	CEditMainView::CreateBKImage();
	CEditMainView::RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnUpdateMainEditView1x1(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr ) 
	{	pCmdUI->Enable(FALSE); }	
	else
	{	pCmdUI->Enable(TRUE); }	
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnMainEditViewAll() 
{
	// TODO: Add your command handler code here
	bool ProjectMapMode = AOIDataCollect.GetProjectMapMode();
	if ( false == ProjectMapMode ) { return; }
	const unsigned int ImageW = this->m_MapImageW;
	const unsigned int ImageH = this->m_MapImageH;
	ImageAPI.CalcImageWndFitZoom(ImageW, ImageH, m_ImageWndRect, 1.1, m_ImageZoom);
	ResetImageOffset();
	CreateBKImage();
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnUpdateMainEditViewAll(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	bool bLockUIWnd = GetLockUIWnd();
	bool ProjectMapMode = AOIDataCollect.GetProjectMapMode();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || false==ProjectMapMode ) 
	{	pCmdUI->Enable(FALSE); }	
	else
	{	pCmdUI->Enable(TRUE); }	
}
//-------------------------------------------------------------------------------------//
CURSOR_POS_MODE CEditMainView::CheckCursorPosMode(POINT pt)//確認鼠標座標模式
{
	CURSOR_POS_MODE CursorMode=CURSOR_POS_NONE;	
	MANIPULATE_MAIN_MODE ManiMainMode = AOIDataCollect.GetManipulateMainMode();
	if ( MANIPULATE_MAIN_SELECT != ManiMainMode )
	{	return CursorMode; }
	if ( AOIDataCollect.CheckMoveObjectMode() == false )
	{	return CursorMode; }

	switch ( m_MainMode )
	{
	case MENU_MAIN_EDIT_MODE_PANEL:
		CursorMode = CheckCursorPosMode_Panel(pt);
		break;
	case MENU_MAIN_EDIT_MODE_BOARD:
		CursorMode = CheckCursorPosMode_Board(pt);
		break;
	case MENU_MAIN_EDIT_MODE_COMPONENT:
		CursorMode = CheckCursorPosMode_Component(pt);
		break;
	}
	return CursorMode;
}
//-------------------------------------------------------------------------------------//
CURSOR_POS_MODE CEditMainView::CheckCursorPosMode_Panel(POINT pt)//確認鼠標座標模式
{
	CURSOR_POS_MODE CursorMode = CURSOR_POS_NONE;	
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return CursorMode; }
	if ( MENU_MAIN_EDIT_MODE_PANEL != m_MainMode ) { return CursorMode; }
	const size_t NObjects = m_SelPanelList.size();	
	if ( 0 == NObjects ) { return CursorMode; }

	RECT         Rect;
	TRECT4D      dRect;
	SIZE         szGrid;
	double       Angle = 0;
	size_t       i = 0;			
	TPanelRect  *PanelRectPtr = NULL;		
	TPOINT2D     Cp;
	TPOINT2D     WndPt = pt;
	TPOINT2D     StageCp;
	TPOINT2D     ImagePt, ImagePt2;	
	TPOINT2D     ImageResolution;
	IMAGE_SIZE   ImageW = 0;
	IMAGE_SIZE   ImageH = 0;
	TPOINT2D     StagePt, StagePt2;
	TREGION4D    StageRgn;
	POINT        ImagePoint;	

	szGrid.cx = GetEditCheckSize();
	szGrid.cy = GetEditCheckSize();	
	CEditMainView::GetImageInfo(ImageW, ImageH, ImageResolution, StageCp);	
	ImageAPI.MapWndPtToImagePt_DBL(ImageW, ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, WndPt, ImagePt);	
	AOIDataCollect.MapCameraPtToStage(ImageW, ImageH, ImageResolution, ImagePt, StageCp, StagePt);

	Cp.x = Cp.y = 0;	
	for ( i=0; i<NObjects; i++ )
	{
		PanelRectPtr = &(m_SelPanelList[i]);			

		//Stage Mode
		StageRgn = PanelRectPtr->PanelRgn;
		JetAPI::Point2DToPoint(StagePt, ImagePoint);
		JetAPI::Region4DToRect(StageRgn, Rect, true);
		if ( ::PtInRect(&Rect, ImagePoint) == FALSE ) { continue; }
		CursorMode = CURSOR_POS_INNER;
		break;
	}
	return CursorMode;
}
//-------------------------------------------------------------------------------------//
CURSOR_POS_MODE CEditMainView::CheckCursorPosMode_Board(POINT pt)//確認鼠標座標模式
{
	CURSOR_POS_MODE CursorMode = CURSOR_POS_NONE;	
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return CursorMode; }
	if ( MENU_MAIN_EDIT_MODE_BOARD != m_MainMode ) { return CursorMode; }
	const size_t NObjects = m_SelBoardList.size();	
	if ( 0 == NObjects ) { return CursorMode; }

	RECT         Rect;
	TRECT4D      dRect;
	SIZE         szGrid;
	double       Angle = 0;
	size_t       i = 0;			
	TBoardRect  *BoardRectPtr = NULL;		
	TPOINT2D     Cp;
	TPOINT2D     WndPt = pt;
	TPOINT2D     StageCp;
	TPOINT2D     ImagePt, ImagePt2;
	IMAGE_SIZE   ImageW = 0;
	IMAGE_SIZE   ImageH = 0;
	TPOINT2D     ImageResolution;
	TPOINT2D     StagePt, StagePt2;
	TREGION4D    StageRgn;
	POINT        ImagePoint;
	
	szGrid.cx = GetEditCheckSize();
	szGrid.cy = GetEditCheckSize();	
	CEditMainView::GetImageInfo(ImageW, ImageH, ImageResolution, StageCp);	
	ImageAPI.MapWndPtToImagePt_DBL(ImageW, ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, WndPt, ImagePt);	
	AOIDataCollect.MapCameraPtToStage(ImageW, ImageH, ImageResolution, ImagePt, StageCp, StagePt);

	Cp.x = Cp.y = 0;	
	for ( i=0; i<NObjects; i++ )
	{
		BoardRectPtr = &(m_SelBoardList[i]);			

		//Stage Mode
		StageRgn = BoardRectPtr->BoardRgn;					
		JetAPI::Point2DToPoint(StagePt, ImagePoint);
		JetAPI::Region4DToRect(StageRgn, Rect, true);
		if ( ::PtInRect(&Rect, ImagePoint) == FALSE ) { continue; }
		CursorMode = CURSOR_POS_INNER;
		break;
	}
	return CursorMode;
}
//-------------------------------------------------------------------------------------//
CURSOR_POS_MODE CEditMainView::CheckCursorPosMode_Component(POINT pt)//確認鼠標座標模式
{
	CURSOR_POS_MODE CursorMode = CURSOR_POS_NONE;	
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return CursorMode; }
	if ( MENU_MAIN_EDIT_MODE_COMPONENT != m_MainMode ) { return CursorMode; }
	const size_t NObjects = m_SelComponentList.size();	
	if ( 0 == NObjects ) { return CursorMode; }

	RECT         Rect;
	TRECT4D      dRect;
	SIZE         szGrid;
	double       Angle = 0;
	size_t       i = 0;			
	TComponentRect *ComponentRectPtr = NULL;		
	TPOINT2D     Cp;
	TPOINT2D     WndPt = pt;
	TPOINT2D     StageCp;
	TPOINT2D     ImagePt, ImagePt2;
	TPOINT2D     StagePt, StagePt2; 
	TPOINT2D     CornerPoint[4];
	IMAGE_SIZE   ImageW = 0;
	IMAGE_SIZE   ImageH = 0;
	TPOINT2D     ImageResolution;
	POINT        ImagePoint;
	
	szGrid.cx = GetEditCheckSize();
	szGrid.cy = GetEditCheckSize();
	CEditMainView::GetImageInfo(ImageW, ImageH, ImageResolution, StageCp);	
	ImageAPI.MapWndPtToImagePt_DBL(ImageW, ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, WndPt, ImagePt);	
	AOIDataCollect.MapCameraPtToStage(ImageW, ImageH, ImageResolution, ImagePt, StageCp, StagePt);

	Cp.x = Cp.y = 0;	
	for ( i=0; i<NObjects; i++ )
	{
		ComponentRectPtr = &(m_SelComponentList[i]);		
		//Image Mode
		/*
		if ( false == ComponentRectPtr->IsExceptionAngle)
		{	
			JetAPI::Point2DToPoint(ImagePt, ImagePoint);
			JetAPI::Rect4DToRect(ComponentRectPtr->Rect, Rect);				
		}
		else
		{
			
			Angle = JetAPI::MapCadAngleToImageAngle(ComponentRectPtr->ComponentAngle);
			ImagePt2 = ImagePt;			
			CornerPoint[0] = ComponentRectPtr->CornerPts[0];
			CornerPoint[1] = ComponentRectPtr->CornerPts[1];
			CornerPoint[2] = ComponentRectPtr->CornerPts[2];
			CornerPoint[3] = ComponentRectPtr->CornerPts[3];				
			JetAPI::RotateCornerPos(-Angle, Cp.x, Cp.y, CornerPoint);
			JetAPI::RotatePos(-Angle, Cp.x, Cp.y, ImagePt2);
			JetAPI::PointsToRect(CornerPoint, 4, dRect);
			JetAPI::Point2DToPoint(ImagePt2, ImagePoint);
			JetAPI::Rect4DToRect(dRect, Rect);				
		}
		CursorMode = JetAPI::CheckCursorPosMode(Rect, szGrid, ImagePoint);
		*/

		//Stage Mode
		if ( false == ComponentRectPtr->IsExceptionAngle)
		{	
			JetAPI::Point2DToPoint(StagePt, ImagePoint);
			JetAPI::Rect4DToRect(ComponentRectPtr->Rect, Rect);				
		}
		else
		{
			
			Angle = JetAPI::MapCadAngleToImageAngle(ComponentRectPtr->ComponentAngle);
			StagePt2 = StagePt;			
			CornerPoint[0] = ComponentRectPtr->CornerPts[0];
			CornerPoint[1] = ComponentRectPtr->CornerPts[1];
			CornerPoint[2] = ComponentRectPtr->CornerPts[2];
			CornerPoint[3] = ComponentRectPtr->CornerPts[3];				
			JetAPI::RotateCornerPos(-Angle, Cp.x, Cp.y, CornerPoint);
			JetAPI::RotatePos(-Angle, Cp.x, Cp.y, StagePt2);
			JetAPI::PointsToRect(CornerPoint, 4, dRect);
			JetAPI::Point2DToPoint(StagePt2, ImagePoint);
			JetAPI::Rect4DToRect(dRect, Rect);				
		}
		//CursorMode = JetAPI::CheckCursorPosMode(Rect, szGrid, ImagePoint);

		if ( ::PtInRect(&Rect, ImagePoint) == FALSE ) { continue; }
		CursorMode = CURSOR_POS_INNER;
		break;
	}
	return CursorMode;
}
//-------------------------------------------------------------------------------------//
void CEditMainView::BuildObjectSelected()//更新選到的物件
{
	switch ( m_MainMode )
	{
	case MENU_MAIN_EDIT_MODE_PANEL:
		BuildPanelSelected();
		break;
	case MENU_MAIN_EDIT_MODE_BOARD:
		BuildBoardSelected();
		break;
	case MENU_MAIN_EDIT_MODE_COMPONENT:
		BuildComponentSelected();
		break;
	}
}
//-------------------------------------------------------------------------------------//
void CEditMainView::BuildPanelSelected()//更新選到的整板
{
	m_SelPanelList.clear();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	if ( MENU_MAIN_EDIT_MODE_PANEL != m_MainMode ) { return; }
	
	size_t       i = 0;	
	TPanelRect   PanelRect;
	CAOIPanel   *PanelPtr = NULL;			
	std::vector<CAOIPanel*> SelPanelList;
	DISTRICT_ID  DistrictID = ProjectPtr->GetProjectActDistrictID();

	ProjectPtr->GetProjectPanelSelected(SelPanelList);
	const size_t SelCount = SelPanelList.size();	
	for ( i=0; i<SelCount; i++ )
	{	
		PanelPtr = SelPanelList[i];
		if ( NULL == PanelPtr ) { continue; }
		PanelRect.PanelPtr = PanelPtr;
		PanelRect.PanelRgn = PanelPtr->GetPanelRgnStage(DistrictID);
		PanelRect.PanelIndex = PanelPtr->GetPanelIndex_Project();		
		m_SelPanelList.push_back(PanelRect);		
	}		
	return;
}
//-------------------------------------------------------------------------------------//
void CEditMainView::BuildBoardSelected()//更新選到的單板
{
	m_SelBoardList.clear();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	if ( MENU_MAIN_EDIT_MODE_BOARD!= m_MainMode ) { return; }
	
	size_t       i = 0;
	TBoardRect   BoardRect;
	CAOIBoard   *BoardPtr = NULL;	
	std::vector<CAOIBoard*> SelBoardList;
	DISTRICT_ID  DistrictID = ProjectPtr->GetProjectActDistrictID();
	ProjectPtr->GetProjectBoardSelected(SelBoardList);
	const size_t SelCount = SelBoardList.size();	
	for ( i=0; i<SelCount; i++ )
	{	
		BoardPtr = SelBoardList[i];
		if ( NULL == BoardPtr ) { continue; }
		BoardRect.BoardPtr = BoardPtr;
		BoardRect.BoardRgn = BoardPtr->GetBoardRgnStage(DistrictID);
		BoardRect.PanelIndex = BoardPtr->GetBoardPanelIndex_Project();
		BoardRect.BoardIndex = BoardPtr->GetBoardIndex_Project();	
		m_SelBoardList.push_back(BoardRect);		
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CEditMainView::BuildComponentSelected()//更新選到的零件
{
	m_SelComponentList.clear();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	if ( MENU_MAIN_EDIT_MODE_COMPONENT != m_MainMode ) { return; }

	size_t       i=0;
	TPOINT2D     StagePos;
	TPOINT2D     StgCornerPos[4];	
	TComponentRect ComponentRect;
	CAOIComponent *ComponentPtr = NULL;
	std::vector<CAOIComponent*> SelComponentList;
	ProjectPtr->GetProjectComponentSelected(SelComponentList);
	const size_t SelCount = SelComponentList.size();
	for ( i=0; i<SelCount; i++ )
	{	
		ComponentPtr = SelComponentList[i];
		if ( NULL == ComponentPtr ) { continue; }		
		
		StagePos.x = ComponentPtr->GetComponentStagePosX();
		StagePos.y = ComponentPtr->GetComponentStagePosY();		
		ComponentPtr->GetComponentBodyStageCornerPos(StgCornerPos);		
		//ComponentPtr->GetComponentRoiStageCornerPos(StgCornerPos);
		
		ComponentRect.ComponentPtr = ComponentPtr;
		ComponentRect.PanelIndex = ComponentPtr->GetComponentPanelIndex_Project();
		ComponentRect.BoardIndex = ComponentPtr->GetComponentBoardIndex_Project();
		ComponentRect.ComponentIndex = ComponentPtr->GetComponentIndex_Project();
		ComponentRect.ComponentAngle = ComponentPtr->GetComponentAngle();
		ComponentRect.IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComponentRect.ComponentAngle);
		ComponentRect.CornerPts[0] = StgCornerPos[0];
		ComponentRect.CornerPts[1] = StgCornerPos[1];
		ComponentRect.CornerPts[2] = StgCornerPos[2];
		ComponentRect.CornerPts[3] = StgCornerPos[3];
		JetAPI::PointsToRect(StgCornerPos, 4, ComponentRect.Rect);		
		m_SelComponentList.push_back(ComponentRect);		
	}
	return;
}
//-------------------------------------------------------------------------------------//
bool CEditMainView::GetShowPopupMenu() const
{
	return m_ShowPopupMenu;
}
//-------------------------------------------------------------------------------------//
void CEditMainView::SetShowPopupMenu(bool val)
{
	m_ShowPopupMenu = val;
}
//-------------------------------------------------------------------------------------//
bool CEditMainView::ExecPopupMenu(POINT point, UINT menuID)
{	
	if ( menuID == 0 ) { return false; }
	if ( GetLockUIWnd() == true ) { return true; }
	CMenu menu;
	CPoint CtrlPt = point;
	VERIFY(menu.LoadMenu(menuID));
	AOIDataCollect.SwitchMultiLanguageMenu(menu, menuID);

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);

	CWnd* pWndPopupOwner = this;
	while (pWndPopupOwner->GetStyle() & WS_CHILD)
	{	pWndPopupOwner = pWndPopupOwner->GetParent();	}

	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditMainView::ExecMoveSelectedObject()
{
	bool IsOK = true;
	switch ( m_MainMode )
	{
	case MENU_MAIN_EDIT_MODE_PANEL:
		IsOK = ExecMoveSelectedPanel();
		break;
	case MENU_MAIN_EDIT_MODE_BOARD:
		IsOK = ExecMoveSelectedBoard();
		break;
	case MENU_MAIN_EDIT_MODE_COMPONENT:
		IsOK = ExecMoveSelectedComponent();
		break;
	}
	return IsOK;	
}
//-------------------------------------------------------------------------------------//
bool CEditMainView::ExecMoveSelectedObjectKernel(int nWndPx, int nWndPy)
{
	bool IsOK = true;
	switch ( m_MainMode )
	{
	case MENU_MAIN_EDIT_MODE_PANEL:
		IsOK = ExecMoveSelectedPanelKernel(nWndPx, nWndPy);
		break;
	case MENU_MAIN_EDIT_MODE_BOARD:
		IsOK = ExecMoveSelectedBoardKernel(nWndPx, nWndPy);
		break;
	case MENU_MAIN_EDIT_MODE_COMPONENT:
		IsOK = ExecMoveSelectedComponentKernel(nWndPx, nWndPy);
		break;
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CEditMainView::ExecMoveSelectedPanel()
{
	const int nWndPx = m_MousePosCurrent.x-m_MousePosLast.x;
	const int nWndPy = m_MousePosCurrent.y-m_MousePosLast.y;
	return ExecMoveSelectedPanelKernel(nWndPx, nWndPy);
}
//-------------------------------------------------------------------------------------//
bool CEditMainView::ExecMoveSelectedPanelKernel(int nWndPx, int nWndPy)
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }
	const size_t NObjects = m_SelPanelList.size();	
	if ( 0 == NObjects ) { return false; }
	if ( MENU_MAIN_EDIT_MODE_PANEL != m_MainMode ) { return false; }	 
	if ( (0==nWndPx) && (0==nWndPy) )
	{	return false; }

	size_t          i = 0;		
	TPOINT2D        ImgPos;
	TPOINT2D        CadPos;
	CAOIPanel      *PanelPtr = NULL;
	TPanelRect     *PanelRectPtr = NULL;		
	const TPOINT2D &ImageRes = GetImageResolution();
	DISTRICT_ID     DistrictID = ProjectPtr->GetProjectActDistrictID();

	ImgPos.x = nWndPx*m_ImageZoom;
	ImgPos.y = nWndPy*m_ImageZoom;	
	AOIDataCollect.MapImageOffsetToCad(ImgPos, ImageRes, CadPos);	
	for ( i=0; i<NObjects; i++ )
	{
		PanelRectPtr = &(m_SelPanelList[i]);		
		if ( NULL == PanelRectPtr ) { continue; }
		PanelPtr = PanelRectPtr->PanelPtr;
		if ( NULL == PanelPtr ) { continue; }		
		PanelPtr->MovePanelPos(CadPos.x, CadPos.y);
		PanelRectPtr->PanelRgn = PanelPtr->GetPanelRgnStage(DistrictID);		
	}	
	m_ModifyPanelPos = true;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditMainView::ExecMoveSelectedBoard()
{
	const int nWndPx = m_MousePosCurrent.x-m_MousePosLast.x;
	const int nWndPy = m_MousePosCurrent.y-m_MousePosLast.y;
	return ExecMoveSelectedBoardKernel(nWndPx, nWndPy);
}
//-------------------------------------------------------------------------------------//
bool CEditMainView::ExecMoveSelectedBoardKernel(int nWndPx, int nWndPy)
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }
	const size_t NObjects = m_SelBoardList.size();	
	if ( 0 == NObjects ) { return false; }
	if ( MENU_MAIN_EDIT_MODE_BOARD != m_MainMode ) { return false; }	
	if ( (0==nWndPx) && (0==nWndPy) )
	{	return false; }

	size_t          i = 0;		
	TPOINT2D        ImgPos;
	TPOINT2D        CadPos;
	CAOIBoard      *BoardPtr = NULL;
	TBoardRect     *BoardRectPtr = NULL;		
	const TPOINT2D &ImageRes = GetImageResolution();
	DISTRICT_ID     DistrictID = ProjectPtr->GetProjectActDistrictID();	

	ImgPos.x = nWndPx*m_ImageZoom;
	ImgPos.y = nWndPy*m_ImageZoom;
	AOIDataCollect.MapImageOffsetToCad(ImgPos, ImageRes, CadPos);		
	for ( i=0; i<NObjects; i++ )
	{
		BoardRectPtr = &(m_SelBoardList[i]);		
		if ( NULL == BoardRectPtr ) { continue; }
		BoardPtr = BoardRectPtr->BoardPtr;
		if ( NULL == BoardPtr ) { continue; }		
		BoardPtr->MoveBoardPos(CadPos.x, CadPos.y);
		BoardRectPtr->BoardRgn = BoardPtr->GetBoardRgnStage(DistrictID);		
	}	
	m_ModifyBoardPos = true;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditMainView::ExecMoveSelectedComponent()
{
	const int nWndPx = m_MousePosCurrent.x-m_MousePosLast.x;
	const int nWndPy = m_MousePosCurrent.y-m_MousePosLast.y;
	return ExecMoveSelectedComponentKernel(nWndPx, nWndPy);
}
//-------------------------------------------------------------------------------------//
bool CEditMainView::ExecMoveSelectedComponentKernel(int nWndPx, int nWndPy)
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }
	const size_t NObjects = m_SelComponentList.size();	
	if ( 0 == NObjects ) { return false; }
	if ( MENU_MAIN_EDIT_MODE_COMPONENT != m_MainMode ) { return false; }
	if ( (0==nWndPx) && (0==nWndPy) )
	{	return false; }

	size_t          i = 0;		
	TPOINT2D        ImgPos;
	TPOINT2D        CadPos;
	DISTRICT_ID     DistrictID;
	TPOINT2D        StgCornerPos[4];
	CAOIPanel      *PanelPtr = NULL;
	CAOIBoard      *BoardPtr = NULL;
	CAOIComponent  *ComponentPtr = NULL;	
	CMapCoordinate *MapCTSPtr = NULL;	
	TComponentRect *ComponentRectPtr = NULL;	
	const TPOINT2D &ImageRes = GetImageResolution();
	ImgPos.x = nWndPx*m_ImageZoom;
	ImgPos.y = nWndPy*m_ImageZoom;
	AOIDataCollect.MapImageOffsetToCad(ImgPos, ImageRes, CadPos);		

	for ( i=0; i<NObjects; i++ )
	{
		ComponentRectPtr = &(m_SelComponentList[i]);		
		if ( NULL == ComponentRectPtr ) { continue; }
		ComponentPtr = ComponentRectPtr->ComponentPtr;
		if ( NULL == ComponentPtr ) { continue; }		
		MapCTSPtr = NULL;		
		PanelPtr = ComponentPtr->GetComponentPanelPtr();
		BoardPtr = ComponentPtr->GetComponentBoardPtr();
		DistrictID = ComponentPtr->GetComponentDistrictID();
		if ( NULL==MapCTSPtr && NULL!=BoardPtr ) 
		{	MapCTSPtr = BoardPtr->GetBoardMapCTSPtr(DistrictID);	}
		if ( NULL==MapCTSPtr && NULL!=PanelPtr ) 
		{	MapCTSPtr = PanelPtr->GetPanelMapCTSPtr(DistrictID);	}
		ComponentPtr->MoveComponentPos(CadPos.x, CadPos.y, MapCTSPtr);		

		ComponentPtr->GetComponentBodyStageCornerPos(StgCornerPos);		
		//ComponentPtr->GetComponentRoiStageCornerPos(StgCornerPos);
		ComponentRectPtr->CornerPts[0] = StgCornerPos[0];
		ComponentRectPtr->CornerPts[1] = StgCornerPos[1];
		ComponentRectPtr->CornerPts[2] = StgCornerPos[2];
		ComponentRectPtr->CornerPts[3] = StgCornerPos[3];
		JetAPI::PointsToRect(StgCornerPos, 4, ComponentRectPtr->Rect);		
	}	
	m_ModifyComponentPos = true;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditMainView::ExecMoveSelectedObjectFinish()
{
	bool IsOK = true;
	switch ( m_MainMode )
	{
	case MENU_MAIN_EDIT_MODE_PANEL:
		IsOK = ExecMoveSelectedPanelFinish();
		break;
	case MENU_MAIN_EDIT_MODE_BOARD:
		IsOK = ExecMoveSelectedBoardFinish();
		break;
	case MENU_MAIN_EDIT_MODE_COMPONENT:
		IsOK = ExecMoveSelectedComponentFinish();
		break;
	}
	return IsOK;	
}
//-------------------------------------------------------------------------------------//
bool CEditMainView::ExecMoveSelectedPanelFinish()
{
	if ( false == m_ModifyPanelPos ) { return true; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }	
	const size_t NObjects = m_SelPanelList.size();	
	if ( 0 == NObjects ) { return false; }
	if ( MENU_MAIN_EDIT_MODE_COMPONENT != m_MainMode ) { return false; }

	size_t          i=0, j=0;
	size_t          BoardCount = 0;
	CAOIPanel      *PanelPtr = NULL;			
	CAOIBoard      *BoardPtr = NULL;
	TPanelRect     *PanelRectPtr = NULL;	
	DISTRICT_ID     DistrictID = ProjectPtr->GetProjectActDistrictID();
	for ( i=0; i<NObjects; i++ )
	{
		PanelRectPtr = &(m_SelPanelList[i]);
		PanelPtr = PanelRectPtr->PanelPtr;
		if ( NULL == PanelPtr ) { continue; }
		PanelPtr->CalcPanelMapParam();
		PanelPtr->AssignPanelMapParamToBoards();
		BoardCount = PanelPtr->GetPanelBoardCount();
		for ( j=0; j<BoardCount; j++ )
		{
			BoardPtr = PanelPtr->GetPanelBoardPtr(j, false);
			if ( NULL == BoardPtr ) { continue; }
			BoardPtr->CalcBoardMapParam();
		}
		PanelPtr->LayoutPanelRegion();
	}
	m_ModifyPanelPos = false;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditMainView::ExecMoveSelectedBoardFinish()
{
	if ( false == m_ModifyBoardPos ) { return true; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }
	const size_t NObjects = m_SelBoardList.size();	
	if ( 0 == NObjects ) { return false; }
	if ( MENU_MAIN_EDIT_MODE_BOARD != m_MainMode ) { return false; }	

	size_t          i = 0;			
	CAOIPanel      *PanelPtr = NULL;
	CAOIBoard      *BoardPtr = NULL;
	TBoardRect     *BoardRectPtr = NULL;	
	const size_t    PanelCount = ProjectPtr->GetProjectPanelCount();	
	DISTRICT_ID     DistrictID = ProjectPtr->GetProjectActDistrictID();
	for ( i=0; i<NObjects; i++ )
	{
		BoardRectPtr = &(m_SelBoardList[i]);
		BoardPtr = BoardRectPtr->BoardPtr;
		if ( NULL == BoardPtr ) { continue; }
		BoardPtr->CalcBoardMapParam(DistrictID);		
	}
	ProjectPtr->LayoutProjectRegion();
	m_ModifyBoardPos = false;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditMainView::ExecMoveSelectedComponentFinish()
{
	if ( false == m_ModifyComponentPos ) { return true; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }	
	if ( MENU_MAIN_EDIT_MODE_COMPONENT != m_MainMode ) { return false; }

	ProjectPtr->LayoutProjectRegion();	
	m_ModifyComponentPos = false;
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnMainEditSwitchOnlineView()
{
	AOIDataCollect.PostMainFrameWndMessage(WM_COMMAND, ID_VIEW_ONLINE_FORMVIEW, NULL);
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnUpdateMainEditSwitchOnlineViewl(CCmdUI* pCmdUI)
{
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnMainEditSwitchEditModel()
{
	AOIDataCollect.PostMainFrameWndMessage(WM_COMMAND, ID_VIEW_EDIT_MODEL_VIEW, NULL);
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnUpdateMainEditSwitchEditModel(CCmdUI* pCmdUI)
{
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnMainEditSwitchEditBarcode()
{
	AOIDataCollect.PostMainFrameWndMessage(WM_COMMAND, ID_VIEW_EDIT_BARCODE_VIEW, NULL);
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnUpdateMainEditSwitchEditBarcode(CCmdUI* pCmdUI)
{
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnMainEditSwitchEditFd()
{
	AOIDataCollect.PostMainFrameWndMessage(WM_COMMAND, ID_VIEW_EDIT_FD_VIEW, NULL);
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnUpdateMainEditSwitchEditFd(CCmdUI* pCmdUI)
{
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnMainEditManiSelect() 
{
	// TODO: Add your command handler code here
	m_ClonePanelList.clear();
	m_CloneBoardList.clear();
	m_CloneComponentList.clear();
	AOIDataCollect.SetManipulateMainMode(MANIPULATE_MAIN_SELECT);
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnUpdateMainEditManiSelect(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr )
	{	pCmdUI->Enable(FALSE);}
	else
	{	pCmdUI->SetCheck(MANIPULATE_MAIN_SELECT == AOIDataCollect.GetManipulateMainMode()); }
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnMainEditManiSelectXPos()
{
	// TODO: Add your command handler code here
	CAOIProject *ProjectPtr = GetActiveProject();	
	if ( NULL == ProjectPtr ) { return ; }

	bool bSucc=true;
	const bool bSelXPos=true;
	const bool bSelYPos=false;	
	std::vector<CAOIComponent*> SelComponentList;
	MULTI_BOARD_CTRL_MODE MultiBoardCtrlMode = AOIDataCollect.GetMultiBoardCtrlMode();
	switch ( m_MainMode )
	{
	case MENU_MAIN_EDIT_MODE_PANEL:		
		break;
	case MENU_MAIN_EDIT_MODE_BOARD:		
		break;	
	case MENU_MAIN_EDIT_MODE_COMPONENT:
		ProjectPtr->GetProjectComponentSelected(SelComponentList);	
		bSucc = ProjectPtr->SelectProjectComponentsBySelectedCadCp(SelComponentList, bSelXPos, bSelYPos);
		if ( true==bSucc && MULTI_BOARD_CTRL_DISABLE!=MultiBoardCtrlMode )
		{
			std::vector<CAOIComponent*> NewSelComponentList;
			ProjectPtr->GetProjectComponentSelected(NewSelComponentList);	
			ProjectPtr->SelectProjectComponentByMultiBoardCtrlMode(NewSelComponentList, MultiBoardCtrlMode);
		}
		break;
	}
	if ( false == bSucc )
	{	return; }	
	AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_PART_SELECTED, NULL);
	return;
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnUpdateMainEditManiSelectXPos(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CAOIProject *ProjectPtr = GetActiveProject();		
	if ( NULL == ProjectPtr || MENU_MAIN_EDIT_MODE_COMPONENT!=m_MainMode )
	{	pCmdUI->Enable(FALSE);}
	else
	{	pCmdUI->Enable(TRUE); }
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnMainEditManiSelectYPos()
{
	// TODO: Add your command handler code here
	CAOIProject *ProjectPtr = GetActiveProject();	
	if ( NULL == ProjectPtr ) { return ; }
	bool bSucc=true;
	const bool bSelXPos=false;
	const bool bSelYPos=true;	
	std::vector<CAOIComponent*> SelComponentList;
	MULTI_BOARD_CTRL_MODE MultiBoardCtrlMode = AOIDataCollect.GetMultiBoardCtrlMode();
	switch ( m_MainMode )
	{
	case MENU_MAIN_EDIT_MODE_PANEL:		
		break;
	case MENU_MAIN_EDIT_MODE_BOARD:		
		break;	
	case MENU_MAIN_EDIT_MODE_COMPONENT:
		ProjectPtr->GetProjectComponentSelected(SelComponentList);
		bSucc = ProjectPtr->SelectProjectComponentsBySelectedCadCp(SelComponentList, bSelXPos, bSelYPos);
		if ( true==bSucc && MULTI_BOARD_CTRL_DISABLE!=MultiBoardCtrlMode )
		{
			std::vector<CAOIComponent*> NewSelComponentList;
			ProjectPtr->GetProjectComponentSelected(NewSelComponentList);	
			ProjectPtr->SelectProjectComponentByMultiBoardCtrlMode(NewSelComponentList, MultiBoardCtrlMode);
		}	
		break;
	}
	AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_PART_SELECTED, NULL);
	return;
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnUpdateMainEditManiSelectYPos(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CAOIProject *ProjectPtr = GetActiveProject();	
	if ( NULL == ProjectPtr || MENU_MAIN_EDIT_MODE_COMPONENT!=m_MainMode )
	{	pCmdUI->Enable(FALSE);}
	else
	{	pCmdUI->Enable(TRUE); }
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnMainEditManiSelectRowCol()
{
	// TODO: Add your command handler code here
	CAOIProject *ProjectPtr = GetActiveProject();	
	if ( NULL == ProjectPtr ) { return ; }

	CString         str;
	TListNode       Node;	
	CInputListWnd   EnumWnd;
	DWORD_PTR       OldIndex=0;
	CString         strCaption, strLabel;	
	std::vector<TListNode> NodelList;	
	const int       SelCol = 1;
	const int       SelRow = 2;
	const int       SelOdd = 3;
	const int       SelEven= 4;		

	OldIndex = 0;
	NodelList.clear();
	Node.Data = SelCol;	
	Node.Text = _T("Col");	
	Node.Text = LoadMultiLanguageString(Node.Text, Node.Text);
	NodelList.push_back(Node);

	Node.Data = SelRow;	
	Node.Text = _T("Row");	
	Node.Text = LoadMultiLanguageString(Node.Text, Node.Text);
	NodelList.push_back(Node);
	
	strLabel = _T("Mode");
	strCaption = _T("Set Col/Row Index");
	strLabel = LoadMultiLanguageString(strLabel, strLabel);
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	EnumWnd.SetParam1(strCaption, strLabel, OldIndex, NodelList);
	if ( EnumWnd.GetSelIndex1() < 0 ) 
	{	EnumWnd.SetSelIndex1(0); }
	if ( EnumWnd.DoModal() == IDCANCEL )
	{	return ; }
	const int SelColRowMode = EnumWnd.GetSelData();

	OldIndex = 0;
	NodelList.clear();
	Node.Data = SelOdd;	
	Node.Text = _T("Odd");	
	Node.Text = LoadMultiLanguageString(Node.Text, Node.Text);
	NodelList.push_back(Node);

	Node.Data = SelEven;
	Node.Text = _T("Even");
	Node.Text = LoadMultiLanguageString(Node.Text, Node.Text);
	NodelList.push_back(Node);
	
	strLabel = _T("Mode");
	strCaption = _T("Set Odd/Even");
	strLabel = LoadMultiLanguageString(strLabel, strLabel);
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	EnumWnd.SetParam1(strCaption, strLabel, OldIndex, NodelList);
	if ( EnumWnd.GetSelIndex1() < 0 ) 
	{	EnumWnd.SetSelIndex1(0); }
	if ( EnumWnd.DoModal() == IDCANCEL )
	{	return ; }
	const int SelOddEvenMode = EnumWnd.GetSelData();

	int SelColMode = SEL_COL_ROW_IDX_NONE;
	int SelRowMode = SEL_COL_ROW_IDX_NONE;
	switch ( SelColRowMode )
	{
	case SelCol:
		switch ( SelOddEvenMode )
		{
		case SelOdd: SelColMode=SEL_COL_ROW_IDX_ODD; break;
		case SelEven: SelColMode=SEL_COL_ROW_IDX_EVEN; break;
		}
		break;
	case SelRow:
		switch ( SelOddEvenMode )
		{
		case SelOdd: SelRowMode=SEL_COL_ROW_IDX_ODD; break;
		case SelEven: SelRowMode=SEL_COL_ROW_IDX_EVEN; break;
		}
		break;
	}
	ProjectPtr->SelectProjectComponentsByColRowIndex(SelColMode, SelRowMode);
	AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_PART_SELECTED, NULL);
	return;
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnUpdateMainEditManiSelectRowCol(CCmdUI* pCmdUI)
{
	CAOIProject *ProjectPtr = GetActiveProject();	
	if ( NULL == ProjectPtr || MENU_MAIN_EDIT_MODE_COMPONENT!=m_MainMode )
	{	pCmdUI->Enable(FALSE);}
	else
	{	pCmdUI->Enable(TRUE); }
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnMainEditManiCopyToFd()
{
	// TODO: Add your command handler code here
	CString str;
	CAOIProject *ProjectPtr = GetActiveProject();	
	if ( NULL == ProjectPtr ) { return ; }
	CString FdFolder = ProjectPtr->GetProjectFdFolder();	
	const int FdGroupID = ProjectPtr->GetProjectFdFreeGroupID();	
	const unsigned int FrameIndex = ProjectPtr->GetProjectMapIndex();		
	const unsigned int FrameUniqueID = ProjectPtr->GetProjectFrameUniqueID(FrameIndex, true);
	const TFrameParam *FrameParamPtr = AOIDataCollect.GetSystemFrameParamPtrByUniqueID(FrameUniqueID);	
	if ( NULL == FrameParamPtr ) { return; }

	std::vector<CAOIComponent*> SelComponentList;			
	ProjectPtr->GetProjectComponentSelected(SelComponentList);
	const size_t SelComponentCount=SelComponentList.size();
	if ( 0 == SelComponentCount ) { return; }
	
	str = _T("Do you want to copy the component selected to be the board-fd?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO )
	{	return; }

	TUNI_FRAME UniFrame;
	FRAME_TYPE FrameTYpe = FrameParamPtr->FrameType;
	JetAPI::InitialUniFrame(UniFrame);	
	JetAPI::CreateFolder(FdFolder);
	for ( size_t i=0; i<SelComponentCount; i++ )
	{
		CAOIComponent  *ComponentPtr = SelComponentList[i];
		if ( NULL == ComponentPtr ) { continue; }
		CAOIBoard *BoardPtr = ComponentPtr->GetComponentBoardPtr();
		if ( NULL == BoardPtr ) { continue; }
		CAOIPanel *PanelPtr = ComponentPtr->GetComponentPanelPtr();
		if ( NULL == PanelPtr ) { continue; }		
		DISTRICT_ID DistrictID = ComponentPtr->GetComponentDistrictID();	
		const size_t BoardFdCount = BoardPtr->CalcBoardFdCount(DistrictID);
		if ( BoardFdCount >= BOARD_MAX_FD_COUNT )
		{	continue;	}
	
		CAOIFd *FdPtr = AOIObjManager.CreateFdObj();
		if ( NULL == FdPtr )
		{	continue; }	
		FdPtr->SetFdGroupID(FdGroupID);
		FdPtr->SetFdDistrictID(DistrictID);
		FdPtr->SetFdResultID_AOI(RESULT_ID_OK);
		ProjectPtr->SelectProjectAllFds(false);
		ProjectPtr->AddProjectFdPtr(FdPtr, false);
		PanelPtr->AddPanelFdPtr(FdPtr);
		if ( NULL != BoardPtr )
		{	BoardPtr->AddBoardFdPtr(FdPtr); }
	
		TSIZE2D   szFd, szRoi;	
		TPOINT2D  CadPos, StagePos;
		CadPos = ComponentPtr->GetComponentCadPos();
		StagePos.x = ComponentPtr->GetComponentStagePosX();
		StagePos.y = ComponentPtr->GetComponentStagePosY();
		szFd.cx   = ComponentPtr->GetComponentBodySizeW();
		szFd.cy   = ComponentPtr->GetComponentBodySizeH();
		szRoi.cx = ComponentPtr->GetComponentRoiSizeW();
		szRoi.cy = ComponentPtr->GetComponentRoiSizeH();

		const int FdUniqueID = FdPtr->GetFdUniqueID();				
		CString FdModelFolder = AOIDataDefine.GetFdModelFolder(FdFolder, FdUniqueID);			
		if ( FdPtr->BuildNewFd(CadPos, StagePos, szFd, szRoi, FrameIndex, FrameUniqueID, FrameTYpe, FdModelFolder, UniFrame) == false )
		{	continue; }
	}
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_PART_LIST_WND, WPARAM_BUILD_PART_LIST, LPARAM_BUILD_DOCK_LIST_PART);		
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_PART_LIST_WND, WPARAM_UPDATE_PART_SELECTED, NULL);
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnUpdateMainEditManiCopyToFd(CCmdUI* pCmdUI)
{
	CAOIProject *ProjectPtr = GetActiveProject();	
	if ( NULL == ProjectPtr || MENU_MAIN_EDIT_MODE_COMPONENT!=m_MainMode )
	{	pCmdUI->Enable(FALSE);}
	else
	{	pCmdUI->Enable(TRUE); }
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnMainEditManiAdd() 
{
	// TODO: Add your command handler code here
	m_BoardPtr_Add = NULL;
	m_ComponentName = _T("");
	m_PartNumberName = _T("");
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }

	bool IsOK = true;
	size_t     BoardCount=0;
	CString    str;
	CString    strValue;
	CString    strLabel;
	CString    strCaption;
	CInputBoxWnd  InputBox;
	CAOIPanel *PanelPtr = NULL;
	CAOIBoard *BoardPtr = NULL;
	const size_t PanelCount = ProjectPtr->GetProjectPanelCount();
	if ( 1 == PanelCount )
	{	PanelPtr = ProjectPtr->GetProjectPanelPtr(0, true);	}
	else
	{	PanelPtr = ProjectPtr->GetProjectActivePanel();	}	
	switch ( m_MainMode )
	{
	case MENU_MAIN_EDIT_MODE_PANEL:
		IsOK = ExecMainAddPanel();
		return; 
		break;
	case MENU_MAIN_EDIT_MODE_BOARD:
		IsOK = ExecMainAddBoard();
		return; 
		break;
	case MENU_MAIN_EDIT_MODE_COMPONENT:
		if ( AOIDataCollect.OperateLevelEditFuncAddComponent() == false )
		{	return ; }
		if ( NULL == PanelPtr )
		{
			str = _T("Please Select Panel First");
			str = LoadMultiLanguageString(str, str);
			JetAPI::ShowMessageBox(str);
			return;
		}
		BoardCount = PanelPtr->GetPanelBoardCount();
		if ( 1 == BoardCount )
		{	BoardPtr = PanelPtr->GetPanelBoardPtr(0, true);	}
		else
		{	BoardPtr = ProjectPtr->GetProjectActiveBoard(); }
		if ( NULL == BoardPtr )
		{
			str = _T("Please Select Board First");
			str = LoadMultiLanguageString(str, str);
			JetAPI::ShowMessageBox(str);
			return;
		}
		strValue = _T("Component");
		strLabel = _T("Component Name");
		strLabel = LoadMultiLanguageString(strLabel, strLabel);
		strCaption = _T("Set Component Name");
		strCaption = LoadMultiLanguageString(strCaption, strCaption);		
		InputBox.SetParam1(strCaption, strLabel, strValue);
		while ( true )
		{
			if ( InputBox.DoModal() == IDCANCEL ) { return ; }
			m_ComponentName = InputBox.m_DataEdit1;			
			m_ComponentName.TrimLeft();
			m_ComponentName.TrimRight();
			if ( m_ComponentName.GetLength() == 0 ) 
			{	continue; }
			break;
		};

		strValue = m_ComponentName;
		strLabel = _T("Part Number");
		strLabel = LoadMultiLanguageString(strLabel, strLabel);
		strCaption = _T("Set Part Number Name");
		strCaption = LoadMultiLanguageString(strCaption, strCaption);		
		InputBox.SetParam1(strCaption, strLabel, strValue);
		while ( true ) 
		{
			if ( InputBox.DoModal() == IDCANCEL ) { return ; }
			m_PartNumberName = InputBox.m_DataEdit1;			
			m_PartNumberName.TrimLeft();
			m_PartNumberName.TrimRight();
			if ( m_PartNumberName.GetLength() == 0 ) 
			{	continue; }
			break;
		}

		m_ComponentName.MakeUpper();
		m_PartNumberName.MakeUpper();
		break;
	default:
		IsOK = false;
		break;
	}
	if ( false == IsOK )
	{	return ; }

	m_BoardPtr_Add = BoardPtr;
	AOIDataCollect.SetManipulateMainMode(MANIPULATE_MAIN_ADD);
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnUpdateMainEditManiAdd(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr )
	{	pCmdUI->Enable(FALSE);}
	else
	{	pCmdUI->SetCheck(MANIPULATE_MAIN_ADD == AOIDataCollect.GetManipulateMainMode());	}
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnMainEditManiPaste() 
{
	// TODO: Add your command handler code here
	AOIDataCollect.SetManipulateMainMode(MANIPULATE_MAIN_PASTE);
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnUpdateMainEditManiPaste(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( true==bLockUIWnd || NULL==ProjectPtr )
	{	pCmdUI->Enable(FALSE);}
	else
	{	pCmdUI->SetCheck(MANIPULATE_MAIN_PASTE == AOIDataCollect.GetManipulateMainMode()); }
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnMainEditManiPasteArray() 
{
	// TODO: Add your command handler code here
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd ) { return; }
	AOIDataCollect.SetManipulateMainMode(MANIPULATE_MAIN_SELECT);

	if ( ExecMainCloneObject() == false )
	{	return; }

	CString        str;
	CString        str2;
	CArrayPasteWnd Wnd;
	size_t         SelectedCnt=0;
	TREGION4D      SelectedRgn;	
	TREGION4D      SelectedRgn1;
	TREGION4D      SelectedRgn2;
	CAOIPanel     *pPanel=NULL; 
	CAOIBoard     *pBoard=NULL;
	CAOIComponent *pComponent=NULL;	
	int            ArrayPasteMode=0;	
	bool           bUseMultiSelRgn = false;
	DISTRICT_ID    DistrictID = ProjectPtr->GetProjectActDistrictID();
	switch ( m_MainMode )
	{
	case MENU_MAIN_EDIT_MODE_PANEL:		
		pPanel = m_ClonePanelList[0];		
		ArrayPasteMode = ARRAY_PASTE_PANEL;		
		str = _T("Do you want to matrix clone the panels");
		break;
	case MENU_MAIN_EDIT_MODE_BOARD:
		pBoard = m_CloneBoardList[0];
		ArrayPasteMode = ARRAY_PASTE_BOARD;
		SelectedCnt = m_CloneBoardList.size();
		pBoard->GetBoardRgnStage(DistrictID, SelectedRgn);
		str = _T("Do you want to matrix clone the  boards");		
		if ( SelectedCnt > 1 )
		{
			bUseMultiSelRgn = true;
			for ( size_t i=1; i<SelectedCnt; i++ )
			{
				pBoard = m_CloneBoardList[i];
				if ( NULL == pBoard ) { continue; }
				SelectedRgn1 = SelectedRgn;
				pBoard->GetBoardRgnStage(DistrictID, SelectedRgn2);				
				JetAPI::UnionRegion(SelectedRgn1, SelectedRgn2, SelectedRgn);
			}
			pBoard = m_CloneBoardList[0];
		}
		break;
	case MENU_MAIN_EDIT_MODE_COMPONENT:
		pComponent = m_CloneComponentList[0];
		ArrayPasteMode = ARRAY_PASTE_COMPONENT;		
		str = _T("Do you want to matrix clone the  components");
		break;
	}
	if ( NULL!=pComponent )
	{	pPanel = pComponent->GetComponentPanelPtr(); }
	if ( NULL!=pBoard )
	{	pPanel = pBoard->GetBoardPanelPtr(); }
	if ( NULL == pPanel )
	{	return; }

	if ( str.GetLength() > 0 )
	{
		str = LoadMultiLanguageString(str, str);
		str2.Format(_T("%s?"), str);
		if ( JetAPI::ShowMessageBox(str2, MB_YESNO) == IDNO )
		{	return; }
	}

	Wnd.SetProjectPtr(ProjectPtr);
	Wnd.SetArrayPasteMode(ArrayPasteMode);
	if ( MENU_MAIN_EDIT_MODE_COMPONENT==m_MainMode && NULL!=pComponent )
	{
		pBoard = pComponent->GetComponentBoardPtr();
		if ( NULL != pBoard )
		{	Wnd.SetMapCoordinate(pBoard->GetBoardMapCTSPtr(DistrictID), pBoard->GetBoardMapSTCPtr(DistrictID));	}
		else
		{	Wnd.SetMapCoordinate(pPanel->GetPanelMapCTSPtr(DistrictID), pPanel->GetPanelMapSTCPtr(DistrictID));	}
	}
	else
	{	Wnd.SetMapCoordinate(pPanel->GetPanelMapCTSPtr(DistrictID), pPanel->GetPanelMapSTCPtr(DistrictID));	}	
	if ( 1 == SelectedCnt || true == bUseMultiSelRgn )
	{	Wnd.SetSelectedRgn(SelectedRgn); }

	INT_PTR Res = Wnd.DoModal();
	HWND hWnd = CWnd::GetSafeHwnd();
	AOIDataCollect.SetCallbackWnd(hWnd);
	CWnd::PostMessage(MSG_CAMERA_REGRAB_IMAGE, NULL, NULL);
	if ( IDCANCEL == Res ) 
	{	return; }

	bool  IsOK = true;
	std::vector<POINT>    IdxList;
	std::vector<TPOINT2D> PosList;	
	unsigned int   PanelIndex = -1;
	unsigned int   BoardIndex = -1;
	unsigned int   ComponentIndex = -1;
	const double   PitchX = Wnd.GetColPitch();
	const double   PitchY = Wnd.GetRowPitch();
	const int      NameMode= Wnd.GetNameMode();
	const bool     ChangeSelName = Wnd.GetChangeSelName();

	Wnd.GetIdxList(IdxList);
	Wnd.GetPosList(PosList);
	switch ( m_MainMode )
	{
	case MENU_MAIN_EDIT_MODE_PANEL:
		IsOK = ProjectPtr->ArrayPasteProjectPanelList(m_ClonePanelList, PosList);
		BuildPanelSelected();
		pPanel = ProjectPtr->GetProjectPanelPtrBySelected();
		if ( NULL != pPanel ) 
		{	PanelIndex = pPanel->GetPanelIndex_Project();	}
		break;
	case MENU_MAIN_EDIT_MODE_BOARD:
		IsOK = ProjectPtr->ArrayPasteProjectBoardList(m_CloneBoardList, PosList);	
		BuildBoardSelected();
		pBoard = ProjectPtr->GetProjectBoardPtrBySelected();
		if ( NULL != pBoard ) 
		{
			PanelIndex = pBoard->GetBoardPanelIndex_Project();
			BoardIndex = pBoard->GetBoardIndex_Project();		
		}
		break;
	case MENU_MAIN_EDIT_MODE_COMPONENT:
		IsOK = ProjectPtr->ArrayPasteProjectComponentList(m_CloneComponentList, PosList, IdxList, NameMode, ChangeSelName);		
		BuildComponentSelected();
		pComponent = ProjectPtr->GetProjectComponentPtrBySelected();
		if ( NULL != pComponent ) 
		{
			PanelIndex = pComponent->GetComponentPanelIndex_Project();
			BoardIndex = pComponent->GetComponentBoardIndex_Project();
			ComponentIndex = pComponent->GetComponentIndex_Project();
		}
		break;
	}
	if ( false == IsOK )
	{	JetAPI::ShowMessageBox(ProjectPtr->GetErrorString());	}

	ProjectPtr->ResetProjectActiveIndex();
	ProjectPtr->SetProjectActivePanelIndex(PanelIndex);	
	ProjectPtr->SetProjectActiveBoardIndex(BoardIndex);	
	ProjectPtr->SetProjectActiveComponentIndex(ComponentIndex);	

	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_PART_LIST_WND, WPARAM_BUILD_PART_LIST, LPARAM_BUILD_DOCK_LIST_EDIT);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_PART_LIST_WND, WPARAM_UPDATE_PART_SELECTED, NULL);		
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnUpdateMainEditManiPasteArray(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd ) 
	{	pCmdUI->Enable(FALSE); }	
	else
	{	pCmdUI->Enable(TRUE); }	
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnMainEditManiClone() 
{
	// TODO: Add your command handler code here
	bool IsOK = ExecMainCloneObject();
	RedrawWnd();
	if ( true == IsOK ) 
	{	AOIDataCollect.SetManipulateMainMode(MANIPULATE_MAIN_PASTE);	}
	else
	{	AOIDataCollect.SetManipulateMainMode(MANIPULATE_MAIN_SELECT);	}
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnUpdateMainEditManiClone(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd ) 
	{	pCmdUI->Enable(FALSE); }	
	else
	{	pCmdUI->Enable(TRUE); }	
}
//-------------------------------------------------------------------------------------//
bool CEditMainView::ExecMainCloneObject()
{
	bool IsOK = true;
	switch ( m_MainMode )
	{
	case MENU_MAIN_EDIT_MODE_PANEL:
		IsOK = ExecMainClonePanel();
		break;
	case MENU_MAIN_EDIT_MODE_BOARD:
		IsOK = ExecMainCloneBoard();
		break;
	case MENU_MAIN_EDIT_MODE_COMPONENT:
		IsOK = ExecMainCloneComponent();
		break;
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CEditMainView::ExecMainClonePanel()
{
	m_ClonePanelList.clear();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }	
	if ( MENU_MAIN_EDIT_MODE_PANEL != m_MainMode ) { return false; }	 
	if ( AOIDataCollect.OperateLevelEditFuncAddPanel() == false ) {	return false; }

	size_t         i=0;
	CAOIPanel    *PanelPtr = NULL;
	const size_t  PanelCount = ProjectPtr->GetProjectPanelCount();

	for ( i=0; i<PanelCount; i++ )
	{
		PanelPtr = ProjectPtr->GetProjectPanelPtr(i, false);
		if ( NULL == PanelPtr ) { continue; }
		if ( PanelPtr->GetPanelDeleted() == true ) { continue; }
		if ( PanelPtr->GetPanelSelected() == false ) { continue; }
		//PanelPtr->SetPanelSelected(false);
		//PanelPtr->SelectPanelAllObjects(false);
		m_ClonePanelList.push_back(PanelPtr);
	}	
	const size_t CloneCount = m_ClonePanelList.size();
	if ( 0 == CloneCount ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditMainView::ExecMainCloneBoard()
{	
	m_CloneBoardList.clear();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }	
	if ( MENU_MAIN_EDIT_MODE_BOARD != m_MainMode ) { return false; }	 
	if ( AOIDataCollect.OperateLevelEditFuncAddBoard() == false ) {	return false; }

	size_t         i=0;
	CAOIBoard    *BoardPtr = NULL;
	const size_t  BoardCount = ProjectPtr->GetProjectBoardCount();

	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr = ProjectPtr->GetProjectBoardPtr(i, false);
		if ( NULL == BoardPtr ) { continue; }
		if ( BoardPtr->GetBoardDeleted() == true ) { continue; }
		if ( BoardPtr->GetBoardSelected() == false ) { continue; }
		//BoardPtr->SetBoardSelected(false);
		//BoardPtr->SelectBoardAllObjects(false);
		m_CloneBoardList.push_back(BoardPtr);
	}	
	const size_t CloneCount = m_CloneBoardList.size();
	if ( 0 == CloneCount ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditMainView::ExecMainCloneComponent()
{
	m_CloneComponentList.clear();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }	
	if ( MENU_MAIN_EDIT_MODE_COMPONENT != m_MainMode ) { return false; }	 
	if ( AOIDataCollect.OperateLevelEditFuncAddComponent() == false ) {	return false; }

	size_t         i=0;
	CAOIComponent *ComponentPtr = NULL;
	const size_t   ComponentCount = ProjectPtr->GetProjectComponentCount();

	for ( i=0; i<ComponentCount; i++ )
	{		
		ComponentPtr = ProjectPtr->GetProjectComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }
		if ( ComponentPtr->GetComponentSelected() == false ) { continue; }
		if ( ComponentPtr->CheckComponentIsMasterOrAgent() == false ) { continue; }
		if ( ComponentPtr->CheckComponentIsAgent() )
		{	ComponentPtr = ComponentPtr->GetComponentMasterPtr();	}
		if ( NULL == ComponentPtr ) { continue; }
		ComponentPtr->SetComponentSelected(true);
		ComponentPtr->SetComponentAllAgentSelected(true);
	}

	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = ProjectPtr->GetProjectComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }
		if ( ComponentPtr->GetComponentSelected() == false ) { continue; }
		//ComponentPtr->SetComponentSelected(false);
		m_CloneComponentList.push_back(ComponentPtr);
	}	
	const size_t CloneCount = m_CloneComponentList.size();
	if ( 0 == CloneCount ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditMainView::ExecMainAddObject()
{
	MANIPULATE_MAIN_MODE ManiMainMode = AOIDataCollect.GetManipulateMainMode();
	if ( MANIPULATE_MAIN_ADD != ManiMainMode ) { return true; }

	bool IsOK = true;
	switch ( m_MainMode )
	{
	case MENU_MAIN_EDIT_MODE_PANEL:		
		break;
	case MENU_MAIN_EDIT_MODE_BOARD:
		IsOK = false;
		break;
	case MENU_MAIN_EDIT_MODE_COMPONENT:
		IsOK = ExecMainAddComponent();
		break;
	}
	if ( false == IsOK ) { return false; }
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_PART_LIST_WND, WPARAM_BUILD_PART_LIST, LPARAM_BUILD_DOCK_LIST_EDIT);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_PART_LIST_WND, WPARAM_UPDATE_PART_SELECTED, NULL);	
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CEditMainView::ExecMainAddPanel()
{
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd ) 
	{	return false; }
	if ( AOIDataCollect.OperateLevelEditFuncAddPanel() == false ) {	return false; }

	DWORD              Res;
	CString            str;
	CNewPanelWizardWnd Wnd;	
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();

	if ( false == OfflineMode )
	{
		str = _T("It is recommend to add panel in offline-mode, do you want to continue?");
		str = LoadMultiLanguageString(str, str);
		Res = JetAPI::ShowMessageBox(str, MB_YESNO);
		if ( IDNO == Res ) 
		{	return false; }
	}

	Wnd.SetProjectPtr(ProjectPtr);
	Res = Wnd.DoModal();

	HWND hWnd = GetSafeHwnd();
	AOIDataCollect.SetCallbackWnd(hWnd);
	if ( IDCANCEL == Res ) 
	{	return false; }

	CAOIPanel *PanelPtr = Wnd.GetActivePanelPtr();
	if ( NULL != PanelPtr )
	{
		ProjectPtr->SelectProjectAllComponents(false);
		PanelPtr->SelectPanelAllComponents(true);
		ProjectPtr->ApplyProjectLibraryToComponentsSelected();
		PanelPtr->SelectPanelAllComponents(false);
	}
	m_MainMode = MENU_MAIN_EDIT_MODE_COMPONENT;
	LogOperCtrl.SaveLogProjectPanelSelectedCreate(ProjectPtr);
	//AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_UPDATE, (LPARAM)(this));	
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_PART_LIST_WND, WPARAM_BUILD_PART_LIST, LPARAM_BUILD_DOCK_LIST_EDIT);	
	RedrawWnd();


	if ( false == OfflineMode )
	{
		str = _T("Please Re-Capture Project Offline Images");
		str = LoadMultiLanguageString(str, str);
		Res = JetAPI::ShowMessageBox(str);
		CWnd::PostMessage(WM_COMMAND, MENU_MAIN_EDIT_CAPTURE_PROJECT_MAP, NULL);		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditMainView::ExecMainAddBoard()
{
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd ) 
	{	return false; }
	if ( AOIDataCollect.OperateLevelEditFuncAddBoard() == false ) {	return false; }

	CString    str;
	CAOIPanel *PanelPtr = ProjectPtr->GetProjectActivePanel();
	if ( NULL == PanelPtr )
	{	
		str = _T("Error, No Active Panel");
		JetAPI::ShowMessageBox(str);
		return false; 
	}
	
	DWORD              Res;	
	CNewPanelWizardWnd Wnd;	
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();

	//Wnd.SetPanelPtr(PanelPtr);
	Wnd.SetProjectPtr(ProjectPtr);
	Wnd.SetNewCadMode(NEW_CAD_BOARD);
	Res = Wnd.DoModal();

	HWND hWnd = GetSafeHwnd();
	AOIDataCollect.SetCallbackWnd(hWnd);
	if ( IDCANCEL == Res ) 
	{	
		PanelPtr->SetPanelSelected(true);
		return false; 
	}

	CAOIPanel *NewPanelPtr = Wnd.GetActivePanelPtr();	
	if ( NULL != NewPanelPtr )
	{		
		const bool bByCad = false;//保留Cad
		ProjectPtr->SelectProjectAllComponents(false);
		NewPanelPtr->SelectPanelAllComponents(true);
		ProjectPtr->ApplyProjectLibraryToComponentsSelected();
		NewPanelPtr->SelectPanelAllComponents(false);

		ProjectPtr->SelectProjectAllBoards(false);
		NewPanelPtr->SelectPanelAllBoards(true);		
		ProjectPtr->ChangeProjectBoardSelectedPanel(PanelPtr);		

		ProjectPtr->SelectProjectAllPanels(false);
		NewPanelPtr->SetPanelSelected(true);
		ProjectPtr->DeleteProjectPanelSelected();
		PanelPtr->SetPanelSelected(true);
	}

	m_MainMode = MENU_MAIN_EDIT_MODE_COMPONENT;
	LogOperCtrl.SaveLogProjectBoardSelectedCreate(ProjectPtr);	
	//AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_UPDATE, (LPARAM)(this));	
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_PART_LIST_WND, WPARAM_BUILD_PART_LIST, LPARAM_BUILD_DOCK_LIST_EDIT);	
	RedrawWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditMainView::ExecMainAddComponent()
{	
	CAOIBoard   *BoardPtr = NULL;
	CAOIPanel   *PanelPtr = NULL;
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }
	if ( NULL == m_BoardPtr_Add ) { return false; }
	PanelPtr = m_BoardPtr_Add->GetBoardPanelPtr();
	if ( NULL == PanelPtr ) { return false; }
	if ( AOIDataCollect.OperateLevelEditFuncAddComponent() == false ) {	return false; }

	BoardPtr = m_BoardPtr_Add;

	size_t    i=0, j=0;	
	TREGION4D Rgn;	
	if ( GetStageSelectRegion(Rgn) == false ) { return false; }

	CAMERA_ID CameraID = PRIMARY_CAMERA_ID;
	const double ResX = AOIDataCollect.GetCameraResolutionX(CameraID);
	const double ResY = AOIDataCollect.GetCameraResolutionY(CameraID);
	const double MinW = 2*ResX;//um
	const double MinH = 2*ResY;//um
	const double RgnW = Rgn.GetWidth();
	const double RgnH = Rgn.GetHeight();
	const double RgnCpX = Rgn.GetCpX();
	const double RgnCpY = Rgn.GetCpY();	

	//剔除過小
	if ( RgnW<MinW || RgnH<MinH )
	{	return false; }	
	
	CString      str;
	TPOINT2D     CadPos;
	TPOINT2D     StagePos;
	CString      strPartNumber;
	CString      strComponentName;	
	CString      strValue;
	CString      strLabel;
	CString      strCaption;	
	CInputBoxWnd InputBox;
	double   ComponentAngle=0.0;
	double   ComponentW = RgnW;
	double   ComponentH = RgnH;
	wchar_t  PartNumber[MAX_JET_PATH]=L"";
	wchar_t  ComponentName[MAX_JET_PATH]=L"";		
	CAOIComponent *ComponentPtr = NULL;	
	DISTRICT_ID     DistrictID = ProjectPtr->GetProjectActDistrictID();
	CMapCoordinate *MapCTSPtr = BoardPtr->GetBoardMapCTSPtr(DistrictID);
	CMapCoordinate *MapSTCPtr = BoardPtr->GetBoardMapSTCPtr(DistrictID);

	TBasePlaneParam BasePlaneParam;
	TNoiseFilterParam NoiseFilterParam;
	const int BasePlaneIndex = AOIDataCollect.GetSystemParameter().m_DefaultSpaceBasePlaneIndex;
	const int NoiseFilterIndex = AOIDataCollect.GetSystemParameter().m_DefaultSpaceNoiseFilterIndex;
	if ( BasePlaneIndex >= 0 )
	{	AOIDataCollect.GetSystemBasePlaneParam(BasePlaneIndex, BasePlaneParam);	}
	if ( NoiseFilterIndex >= 0 )
	{	AOIDataCollect.GetSystemNoiseFilterParam(NoiseFilterIndex, NoiseFilterParam);	}

	i=0;
	while ( true )
	{
		if ( 0 == i ) 
		{	strComponentName.Format(_T("%s"), m_ComponentName); }
		else
		{	strComponentName.Format(_T("%s_%d"), m_ComponentName, i+1); }

		if ( BoardPtr->ChceckBoardComponentNameExist(strComponentName) == false )
		{	break; }
		i ++;
	};

	const double MinComponentW=4*ResX;
	const double MinComponentH=4*ResY;
	if ( ComponentW < MinComponentW ) { ComponentW = MinComponentW; }
	if ( ComponentH < MinComponentH ) { ComponentH = MinComponentH; }
	if ( (ComponentW*1.25) < ComponentH ) 
	{		
		CString strAngle = AOIDataDefine.GetAngleText();
		CString strComponent = AOIDataDefine.GetComponentText();
		str.Format(_T("%s %s = 90"), strComponent, strAngle);

		strCaption = _T("Input Component Angle");
		strLabel.Format(_T("%s %s = 90"), strComponent, strAngle);
		strValue = _T("90.0");
		InputBox.SetParam1(strCaption, strLabel, strValue);
		//if ( InputBox.DoModal() == IDOK ) 				
		if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDYES )
		{
			ComponentAngle = ComponentW;
			ComponentW = ComponentH;
			ComponentH = ComponentAngle;
			ComponentAngle = 90.0;	
		}
		else
		{	ComponentAngle = 00.0; }
	}
	else
	{	ComponentAngle = 00.0;	}

	JetAPI::TCHAR2wchar(strComponentName, ComponentName, MAX_JET_PATH);
	JetAPI::TCHAR2wchar(m_PartNumberName, PartNumber, MAX_JET_PATH);

	ComponentPtr = AOIObjManager.CreateComponentObj();
	if ( NULL == ComponentPtr ) { return false; }	

	StagePos.x = RgnCpX;
	StagePos.y = RgnCpY;
	MapSTCPtr->Map2D(StagePos.x, StagePos.y, CadPos.x, CadPos.y);

	CAOIModel *ModelPtr = NULL;
	ProjectPtr->AddProjectComponentPtr(ComponentPtr, false);
	PanelPtr->AddPanelComponentPtr(ComponentPtr);
	BoardPtr->AddBoardComponentPtr(ComponentPtr);
	ModelPtr = ComponentPtr->GetComponentModelPtr();
	if ( NULL != ModelPtr )
	{	ModelPtr->SetModelName(PartNumber);	}

	ComponentPtr->SetComponentName(ComponentName);
	ComponentPtr->SetComponentPartNumber(PartNumber);
	ComponentPtr->SetComponentModelName(PartNumber);
	ComponentPtr->SetComponentAngle(0);
	ComponentPtr->SetComponentRoiSizeW(ComponentW);
	ComponentPtr->SetComponentRoiSizeH(ComponentH);
	ComponentPtr->SetComponentBodySizeW(ComponentW);
	ComponentPtr->SetComponentBodySizeH(ComponentH);		
	ComponentPtr->SetComponentCadPosX(CadPos.x);
	ComponentPtr->SetComponentCadPosY(CadPos.y);
	ComponentPtr->SetComponentOrgCadPosX(CadPos.x);
	ComponentPtr->SetComponentOrgCadPosY(CadPos.y);
	ComponentPtr->SetComponentStagePosX(StagePos.x);
	ComponentPtr->SetComponentStagePosY(StagePos.y);
	ComponentPtr->SetComponentDistrictID(DistrictID);
	ComponentPtr->CalcComponentCadCornerPos();	
	ComponentPtr->LayoutComponentStageCornerPos();	
	ComponentPtr->UpdateComponentParamToModel(false);
	ComponentPtr->SetComponentSpaceBasePlaneParam(BasePlaneParam);
	ComponentPtr->SetComponentSpaceNoiseFilterParam(NoiseFilterParam);

	if ( fabs(ComponentAngle) > 0.001 )
	{	ComponentPtr->RotateComponent(ComponentAngle, CadPos.x, CadPos.y, NULL); }

	CAOIModel *ModelPtrM = ProjectPtr->GetProjectModelPtrByModelName(m_PartNumberName);
	if ( NULL != ModelPtrM )
	{	ComponentPtr->UpdateComponentModelFromLibrary(ModelPtrM);	}

	ProjectPtr->SelectProjectAllComponents(false);
	ComponentPtr->SetComponentSelected(true);
	BoardPtr->LayoutBoardRegion();
	PanelPtr->LayoutPanelRegion();
	ProjectPtr->SetProjectActiveComponent(ComponentPtr);
	LogOperCtrl.SaveLogProjectComponentSelectedCreate(ProjectPtr);
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CEditMainView::ExecMainPasteObject()
{
	MANIPULATE_MAIN_MODE ManiMainMode = AOIDataCollect.GetManipulateMainMode();
	if ( MANIPULATE_MAIN_PASTE != ManiMainMode ) { return true; }

	bool IsOK = true;
	switch ( m_MainMode )
	{
	case MENU_MAIN_EDIT_MODE_PANEL:
		IsOK = ExecMainPastePanel();
		break;
	case MENU_MAIN_EDIT_MODE_BOARD:
		IsOK = ExecMainPasteBoard();
		break;
	case MENU_MAIN_EDIT_MODE_COMPONENT:
		IsOK = ExecMainPasteComponent();
		break;
	}
	if ( false == IsOK ) { return false; }
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_PART_LIST_WND, WPARAM_BUILD_PART_LIST, LPARAM_BUILD_DOCK_LIST_EDIT);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_PART_LIST_WND, WPARAM_UPDATE_PART_SELECTED, NULL);	

	const int ContinuePasteMode = AOIDataCollect.GetSystemParameter().m_ContinuePasteMode;
	if ( FN_DISABLE == ContinuePasteMode )
	{	AOIDataCollect.CancelManipulateMainMode();	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CEditMainView::ExecMainPastePanel()
{
	m_SelPanelList.clear();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }
	if ( AOIDataCollect.OperateLevelEditFuncAddPanel() == false ) {	return false; }

	size_t    i=0, j=0;
	TREGION4D Rgn;
	if ( GetStageSelectRegion(Rgn) == false ) { return false; }
	const double RgnCpX = Rgn.GetCpX();
	const double RgnCpY = Rgn.GetCpY();

	const size_t CloneCount = m_ClonePanelList.size();	
	if ( 0 == CloneCount ) { return true; }
	CAOIPanel *pPanel = m_ClonePanelList[0];	
	DISTRICT_ID  DistrictID = ProjectPtr->GetProjectActDistrictID();

	TPOINT2D StageOffsetPos;
	TREGION4D  PanelStageRgn = pPanel->GetPanelRgnStage(DistrictID);
	const double PanelStageRgnCpX = PanelStageRgn.GetCpX();
	const double PanelStageRgnCpY = PanelStageRgn.GetCpY();	
	StageOffsetPos.x = RgnCpX-PanelStageRgnCpX;
	StageOffsetPos.y = RgnCpY-PanelStageRgnCpY;	
	ProjectPtr->PasteProjectPanelList(m_ClonePanelList, StageOffsetPos.x, StageOffsetPos.y);		

	BuildPanelSelected();
	unsigned int PanelIndex = -1;	
	pPanel = ProjectPtr->GetProjectPanelPtrBySelected();
	if ( NULL != pPanel ) 
	{	PanelIndex = pPanel->GetPanelIndex_Project();	}
	ProjectPtr->ResetProjectActiveIndex();
	ProjectPtr->SetProjectActivePanelIndex(PanelIndex);		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditMainView::ExecMainPasteBoard()
{
	m_SelBoardList.clear();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }
	if ( AOIDataCollect.OperateLevelEditFuncAddBoard() == false ) {	return false; }

	size_t    i=0, j=0;
	TREGION4D Rgn;	
	if ( GetStageSelectRegion(Rgn) == false ) { return false; }
	const double RgnCpX = Rgn.GetCpX();
	const double RgnCpY = Rgn.GetCpY();

	const size_t CloneCount = m_CloneBoardList.size();	
	if ( 0 == CloneCount ) { return true; }	
	
	CAOIBoard   *pBoard = m_CloneBoardList[0];		
	CAOIPanel   *pPanel = pBoard->GetBoardPanelPtr();
	if ( NULL == pPanel ) { return true; }	
	const DISTRICT_ID  DistrictID = ProjectPtr->GetProjectActDistrictID();
	const unsigned int RefBoardIndex = pBoard->GetBoardIndex_Project();	
	
	double       BoardRotateAngle=0;
	int          nAlignComponentIndex=-1;	
	bool         bUsedRotateAngle=false;	
	TPOINT2D     StageOffsetPos;
	TREGION4D    BoardStageRgn = pBoard->GetBoardRgnStage(DistrictID);
	const double BoardStageRgnCpX = BoardStageRgn.GetCpX();
	const double BoardStageRgnCpY = BoardStageRgn.GetCpY();	
	if ( CloneCount > 1 )
	{
		StageOffsetPos.x = RgnCpX-BoardStageRgnCpX;
		StageOffsetPos.y = RgnCpY-BoardStageRgnCpY;		
	}	
	if ( 1 == CloneCount)
	{		
		CString      strValue;
		CString      strLabel;
		CString      strCaption;	
		CInputBoxWnd InputBox;
		strCaption = _T("Set Board Rotate Angle");
		strCaption = LoadMultiLanguageString(strCaption, strCaption);
		strLabel = AOIDataDefine.GetAngleText();
		strValue = _T("0");
		InputBox.SetParam1(strCaption, strLabel, strValue);
		if ( InputBox.DoModal() == IDCANCEL )
		{	return true; }		
		bUsedRotateAngle = true;
		strValue = InputBox.m_DataEdit1;
		BoardRotateAngle=::_ttof(strValue);	

		TListNode     Node;
		CInputListWnd EnumWnd;	
		std::vector<TListNode> NodelList;	
		strLabel = AOIDataDefine.GetComponentText();	
		strCaption = _T("Set Align Component");
		strCaption = LoadMultiLanguageString(strCaption, strCaption);		

		CSortObj              SortNode;
		std::vector<CSortObj> SortList;
		CAOIComponent *ComponentPtr=NULL;
		const size_t BoardComponentCount = pBoard->GetBoardComponentCount();
	
		SortNode.SetSortMode(SORT_BY_TXT);	
		for ( i=0; i<BoardComponentCount; i++ )
		{
			ComponentPtr = pBoard->GetBoardComponentPtr(i, false);
			if ( NULL == ComponentPtr ) { continue; }
			if ( ComponentPtr->CheckComponentIsAgent() == true ) { continue; }

			SortNode.SetPtr(ComponentPtr);
			SortNode.SetID(i);
			SortNode.SetValueStr(ComponentPtr->GetComponentName());		
			SortList.push_back(SortNode);		
		}	
		std::sort(SortList.begin(), SortList.end());	
		const size_t SortNodeCount = SortList.size();
		for ( i=0; i<SortNodeCount; i++ )
		{
			SortNode = SortList[i];
			ComponentPtr = (CAOIComponent*)(SortNode.GetPtr());
			if ( NULL == ComponentPtr ) { continue; }
			Node.Data = SortNode.GetID();
			Node.Text = ComponentPtr->GetComponentName();
			Node.Ptr  = ComponentPtr;
			NodelList.push_back(Node);		
		}	
		//ComboxWnd.SetWndPos(Point);	
		EnumWnd.SetParam1(strCaption, strLabel, -1, NodelList);	
		if ( EnumWnd.DoModal() == IDCANCEL )
		{	return true;	}
			
		int  SelIndex=EnumWnd.GetSelIndex1();
		if ( SelIndex >= 0 )
		{
			void      *Ptr =  EnumWnd.GetSelPtr();
			CString   SelStr = EnumWnd.GetSelText();
			nAlignComponentIndex = (int)(EnumWnd.GetSelData());				
		}		
	}
	ProjectPtr->PasteProjectBoardList(m_CloneBoardList, StageOffsetPos.x, StageOffsetPos.y);	
	
	pBoard = ProjectPtr->GetProjectBoardPtrByLastOne();
	if ( true==bUsedRotateAngle && NULL!=pBoard )
	{	
		const double RotateAngle=BoardRotateAngle;	
		if ( fabs(RotateAngle)>0.0001 )
		{	
			ProjectPtr->RotateProjectBoardSelected(RotateAngle);	
			LogOperCtrl.SaveLogProjectBoardSelectedRotate(ProjectPtr, RotateAngle);
		}

		const int ComponentIndex = (int)(nAlignComponentIndex);	
		CAOIComponent *ComponentPtr = pBoard->GetBoardComponentPtr(ComponentIndex, true);				

		double RgnCadX=0;
		double RgnCadY=0;
		double BoardCadRgnCpX=0;
		double BoardCadRgnCpY=0;
		TPOINT2D CadOffsetPos;
		pPanel->GetPanelMapSTCPtr(DistrictID)->Map2D(RgnCpX, RgnCpY, RgnCadX, RgnCadY);
		pPanel->GetPanelMapSTCPtr(DistrictID)->Map2D(BoardStageRgnCpX, BoardStageRgnCpY, BoardCadRgnCpX, BoardCadRgnCpY);
		if ( NULL == ComponentPtr )
		{
			CadOffsetPos.x = RgnCadX-BoardCadRgnCpX;
			CadOffsetPos.y = RgnCadY-BoardCadRgnCpY;
			StageOffsetPos.x = RgnCpX-BoardStageRgnCpX;
			StageOffsetPos.y = RgnCpY-BoardStageRgnCpY;		
		}	
		else
		{
			double ComponentCadX = ComponentPtr->GetComponentCadPosX();
			double ComponentCadY = ComponentPtr->GetComponentCadPosY();
			double ComponentStageX = ComponentPtr->GetComponentStagePosX();
			double ComponentStageY = ComponentPtr->GetComponentStagePosY();
			StageOffsetPos.x = RgnCpX-ComponentStageX;
			StageOffsetPos.y = RgnCpY-ComponentStageY;

			CadOffsetPos.x = RgnCadX-ComponentCadX;
			CadOffsetPos.y = RgnCadY-ComponentCadY;
		}	
		//ProjectPtr->MoveProjectBoardSelected(StageOffsetPos.x, StageOffsetPos.y);
		ProjectPtr->MoveProjectBoardSelected(CadOffsetPos.x, CadOffsetPos.y);
		LogOperCtrl.SaveLogProjectBoardSelectedMove(ProjectPtr, CadOffsetPos.x, CadOffsetPos.y);
	}	

	BuildBoardSelected();
	unsigned int PanelIndex = -1;
	unsigned int BoardIndex = -1;	
	pBoard = ProjectPtr->GetProjectBoardPtrBySelected();
	if ( NULL != pBoard ) 
	{
		PanelIndex = pBoard->GetBoardPanelIndex_Project();
		BoardIndex = pBoard->GetBoardIndex_Project();		
	}
	ProjectPtr->ResetProjectActiveIndex();
	ProjectPtr->SetProjectActivePanelIndex(PanelIndex);	
	ProjectPtr->SetProjectActiveBoardIndex(BoardIndex);		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditMainView::ExecMainPasteComponent()
{
	m_SelComponentList.clear();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }
	if ( AOIDataCollect.OperateLevelEditFuncAddComponent() == false ) {	return false; }

	size_t    i=0, j=0;	
	TREGION4D Rgn;	
	if ( GetStageSelectRegion(Rgn) == false ) { return false; }

	const double RgnW = Rgn.GetWidth();
	const double RgnH = Rgn.GetHeight();
	const double RgnCpX = Rgn.GetCpX();
	const double RgnCpY = Rgn.GetCpY();

	const size_t CloneCount = m_CloneComponentList.size();	
	if ( 0 == CloneCount ) { return true; }	
	CAOIComponent *pComponent = m_CloneComponentList[0];	
	
	CString      str;
	CString      strValue;
	CString      strLabel;
	CString      strCaption;	
	CInputBoxWnd InputBox;
	double       RotateAngle=0.0;
	TREGION4D    BodyRegion;
	TPOINT2D     StageOffsetPos;	
	CAOIModel   *ModelPtr = pComponent->GetComponentModelPtr();	
	const double ComponentStagePosX = pComponent->GetComponentStagePosX();
	const double ComponentStagePosY = pComponent->GetComponentStagePosY();

	StageOffsetPos.x = RgnCpX-ComponentStagePosX;
	StageOffsetPos.y = RgnCpY-ComponentStagePosY;		
	ModelPtr->GetModelBodyBox().GetBoxRegion(BodyRegion);
	const double BodyW = BodyRegion.GetWidth();
	const double BodyH = BodyRegion.GetHeight();

	bool  bRgnHor = true; 
	bool  bBodyHor = true;	
	if ( (RgnW*1.25) < RgnH ) 
	{	bRgnHor = false;	}
	else
	{	bRgnHor = true; }

	if ( (BodyW*1.25) < BodyH ) 
	{	bBodyHor = false;	}
	else
	{	bBodyHor = true; }
	if ( RgnW<50 || RgnH<50 )
	{	bRgnHor = bBodyHor;	}
	if ( CloneCount > 1 ) 
	{	bRgnHor = bBodyHor;	}

	if ( bBodyHor != bRgnHor ) 
	{		
		CString strAngle = AOIDataDefine.GetAngleText();
		CString strComponent = AOIDataDefine.GetComponentText();
		strCaption = _T("Input Component Angle");
		strLabel.Format(_T("%s %s = 90"), strComponent, strAngle);
		strValue = _T("90.0");
		InputBox.SetParam1(strCaption, strLabel, strValue);
		if ( InputBox.DoModal() == IDOK ) 		
		{	RotateAngle = ::_ttof(InputBox.m_DataEdit1);		}
		else
		{	RotateAngle = 0.0; }
	}
	else
	{	RotateAngle = 00.0;	}
	
	ProjectPtr->PasteProjectComponentList(m_CloneComponentList, StageOffsetPos.x, StageOffsetPos.y, RotateAngle);

	BuildComponentSelected();	
	pComponent = ProjectPtr->GetProjectComponentPtrBySelected();
	ProjectPtr->SetProjectActiveComponent(pComponent);	
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnMainEditSetBypass() 
{
	// TODO: Add your command handler code here
	ExecMainBypassObject();
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnUpdateMainEditSetBypass(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here	
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd ) 
	{	pCmdUI->Enable(FALSE); }	
	else
	{	pCmdUI->Enable(TRUE); }	
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnMainEditSetBypass3D()
{
	ExecMainBypass3DComponent();
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnUpdateMainEditSetBypass3D(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd ) 
	{	pCmdUI->Enable(FALSE); }	
	else
	{	pCmdUI->Enable(TRUE); }	
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnMainEditSetModelIsolated()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	CString str;
	CAOIComponent *ComponentPtr = ProjectPtr->GetProjectActiveComponent();
	if ( NULL == ComponentPtr ) { return; }
	const bool ModelIsolated = ComponentPtr->GetComponentModelIsolated();
	if ( true == ModelIsolated )
	{
		str = _T("Disable Model Isolated will clear the model datas, do you want to continue?");
		str = LoadMultiLanguageString(str, str);
		if ( JetAPI::ShowMessageBox(str, MB_YESNO) != IDYES )
		{	return ; }
	}
	ProjectPtr->SwitchProjectComponentModelIsolated();
	LogOperCtrl.SaveLogProjectComponentSelectedModelIsolated(ProjectPtr);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_PART_LIST_WND, WPARAM_BUILD_PART_LIST, LPARAM_BUILD_DOCK_LIST_EDIT);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_PART_LIST_WND, WPARAM_UPDATE_PART_SELECTED, NULL);	
	return;
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnUpdateMainEditSetModelIsolated(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd ) 
	{	pCmdUI->Enable(FALSE); }	
	else
	{	pCmdUI->Enable(TRUE); }	
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnMainEditSetMaskBaseColorLinkIndex()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }		
	CAOIComponent *ComponentPtr = ProjectPtr->GetProjectActiveComponent();
	if ( NULL == ComponentPtr ) { return ; }

	bool bEnable = true;
	size_t     i=0;
	CString    str;
	CString    strLabel;
	CString    strValue;
	CString    strCaption;		
	CInputListWnd EnumWnd;		
	TListNode              Node;
	std::vector<TListNode> NodelList;			

	DWORD_PTR OldColorIndex = ComponentPtr->GetComponentMaskColorGroupLinkIndex();
	strLabel = _T("Color Index");
	strLabel = LoadMultiLanguageString(strLabel, strLabel);
	strCaption = _T("Set Component Base Mask Color Index");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);

	Node.Data = -1;	
	str = _T("Close");	
	Node.Text = LoadMultiLanguageString(str, str);
	NodelList.push_back(Node);
	for ( i=PROJECT_COLOR_ID_BOARD_BEGIN; i<=PROJECT_COLOR_ID_BOARD_END; i++ )
	{
		Node.Data = i;
		Node.Text = AOIDataDefine.GetProjectColorGroupText(i);
		NodelList.push_back(Node);
	}	
	if ( ComponentPtr->GetComponentMaskEnable_Base() == false )
	{	OldColorIndex = -1; }
	EnumWnd.SetParam1(strCaption, strLabel, OldColorIndex, NodelList);
	if ( EnumWnd.GetSelIndex1() < 0 ) 
	{	EnumWnd.SetSelIndex1(0); }	
	if ( EnumWnd.DoModal() == IDCANCEL )
	{	return ; }		
	const int NewColorIndex = (int)(EnumWnd.GetSelData());
	if ( NewColorIndex < 0 )
	{	ProjectPtr->EnableProjectComponentSelectedMaskFunc_Base(false);	}
	else
	{
		ProjectPtr->EnableProjectComponentSelectedMaskFunc_Base(true);
		ProjectPtr->SetProjectComponentSelectedMaskColorIndex_Base(NewColorIndex); 
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnUpdateMainEditSetMaskBaseColorLinkIndex(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd ) 
	{	pCmdUI->Enable(FALSE); }	
	else
	{	pCmdUI->Enable(TRUE); }	
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnMainEditSetComponentAlarmAOI()
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	
	CAOIComponent *ComponentPtr = ProjectPtr->GetProjectActiveComponent();
	if ( NULL == ComponentPtr )	{	return; }	
	
	CComponentDefectAlarmWnd Wnd;
	Wnd.SetEnableAlarm(ComponentPtr->GetComponentEnableAlarm());
	Wnd.SetEnableAlarmOnAOI(ComponentPtr->GetComponentDefectAlarmEnableOnAOI());
	Wnd.SetEnableAlarmOnARS(ComponentPtr->GetComponentDefectAlarmEnableOnARS());
	Wnd.SetEnableDefectCountOnARS(ComponentPtr->GetComponentDefectCountEnableOnARS());	
	Wnd.SetDefectAlarmAOI(ComponentPtr->GetComponentDefectItemAlarmAOI());
	Wnd.SetDefectAlarmARS(ComponentPtr->GetComponentDefectItemAlarmARS());
	Wnd.SetAlarmParamFromModeAOI(ComponentPtr->GetComponentDefectAlarmFromModeAOI());
	Wnd.SetAlarmParamFromModeARS(ComponentPtr->GetComponentDefectAlarmFromModeARS());
	if ( Wnd.DoModal() == IDCANCEL )
	{	return; }
	const bool bEnableAlarm=Wnd.GetEnableAlarm();
	const bool bEnableAlarmOnAOI=Wnd.GetEnableAlarmOnAOI();
	const bool bEnableAlarmOnARS=Wnd.GetEnableAlarmOnARS();
	const bool bEnableDefectCountOnARS=Wnd.GetEnableDefectCountOnARS();
	const CWndDefectItem &DefectAlarmAOI=Wnd.GetDefectAlarmAOI();
	const CWndDefectItem &DefectAlarmARS=Wnd.GetDefectAlarmARS();
	DEFECT_PARAM_FROM_MODE DefectParamFromModeAOI=Wnd.GetAlarmParamFromModeAOI();
	DEFECT_PARAM_FROM_MODE DefectParamFromModeARS=Wnd.GetAlarmParamFromModeARS();
	ProjectPtr->SetProjectComponentSelectedEnableAlarm(bEnableAlarm);	
	ProjectPtr->SetProjectComponentSelectedEnableAlarmOnAOI(bEnableAlarmOnAOI);
	ProjectPtr->SetProjectComponentSelectedEnableAlarmOnARS(bEnableAlarmOnARS);
	ProjectPtr->SetProjectComponentSelectedEnableDefectCountOnARS(bEnableDefectCountOnARS);
	LogOperCtrl.SaveLogProjectComponentSelectedAlarm(ProjectPtr, bEnableAlarm);
	LogOperCtrl.SaveLogProjectComponentSelectedAlarmOnAOI(ProjectPtr, bEnableAlarmOnAOI);
	LogOperCtrl.SaveLogProjectComponentSelectedAlarmOnARS(ProjectPtr, bEnableAlarmOnARS);
	LogOperCtrl.SaveLogProjectComponentSelectedDefectCountOnARS(ProjectPtr, bEnableDefectCountOnARS);	
	ProjectPtr->SetProjectComponentSelectedDefectAlarmAOI(DefectParamFromModeAOI, DefectAlarmAOI);
	ProjectPtr->SetProjectComponentSelectedDefectAlarmARS(DefectParamFromModeARS, DefectAlarmARS);
	LogOperCtrl.SaveLogProjectComponentSelectedDefectAlarmAOI(ProjectPtr, DefectParamFromModeAOI, DefectAlarmAOI);
	LogOperCtrl.SaveLogProjectComponentSelectedDefectAlarmARS(ProjectPtr, DefectParamFromModeARS, DefectAlarmARS);
	return;
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnUpdateMainEditSetComponentAlarmAOI(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd ) 
	{	pCmdUI->Enable(FALSE); }	
	else
	{	pCmdUI->Enable(TRUE); }	
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnMainEditSetComponentSaveReportARS()
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	
	//CAOIComponent *ComponentPtr = AOIDataCollect.GetActiveComponent();
	//if ( NULL == ComponentPtr )	{	return; }		

	CString str;
	str = _T("Do you want to set the selected components to save report (ARS)?");
	str = LoadMultiLanguageString(str, str);
	DWORD Res = JetAPI::ShowMessageBox(str, MB_YESNOCANCEL);
	if ( IDCANCEL == Res ) { return; }
	const bool bSave = IDYES==Res ? true:false;	
	ProjectPtr->SetProjectComponentSelectedSaveReportARS(bSave);		
	LogOperCtrl.SaveLogProjectComponentSelectedSaveReportARS(ProjectPtr, bSave);	
	return;
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnUpdateMainEditSetComponentSaveReportARS(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd ) 
	{	pCmdUI->Enable(FALSE); }	
	else
	{	pCmdUI->Enable(TRUE); }	
}
//-------------------------------------------------------------------------------------//	
bool CEditMainView::ExecMainBypassObject()
{
	MANIPULATE_MAIN_MODE ManiMainMode = AOIDataCollect.GetManipulateMainMode();
	if ( MANIPULATE_MAIN_SELECT != ManiMainMode ) { return true; }

	bool IsOK = true;
	switch ( m_MainMode )
	{
	case MENU_MAIN_EDIT_MODE_PANEL:
		IsOK = ExecMainBypassPanel();
		break;
	case MENU_MAIN_EDIT_MODE_BOARD:
		IsOK = ExecMainBypassBoard();
		break;
	case MENU_MAIN_EDIT_MODE_COMPONENT:
		IsOK = ExecMainBypassComponent();
		break;
	}
	if ( false == IsOK ) { return false; }
	//AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_PART_LIST_WND, WPARAM_BUILD_PART_LIST, LPARAM_BUILD_DOCK_LIST_EDIT);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_PART_LIST_WND, WPARAM_UPDATE_PART_LIST, TREE_CTRL_UPDATE_STATE);		
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_RESULT_LIST_WND, WPARAM_UPDATE_RESULT_LIST, NULL);		
	
	//AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_PART_LIST_WND, WPARAM_UPDATE_PART_SELECTED, NULL);	
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CEditMainView::ExecMainBypassPanel()
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }
	const size_t SelCount = m_SelPanelList.size();	
	if ( 0 == SelCount ) { return false; }
	if ( MENU_MAIN_EDIT_MODE_PANEL != m_MainMode ) { return false; }	 
	if ( AOIDataCollect.OperateLevelEditFuncBypassPanel() == false )	{	return false; }

	size_t i=0;
	CAOIPanel      *PanelPtr = NULL;
	TPanelRect     *PanelRectPtr = NULL;
	PanelRectPtr = &(m_SelPanelList[0]);
	PanelPtr    =  PanelRectPtr->PanelPtr;
	if ( NULL == PanelPtr ) { return false; }
	bool            Bypass = PanelPtr->GetPanelBypassed();
	if ( true == Bypass ) { Bypass = false; }
	else { Bypass = true; }

	for ( i=0; i<SelCount; i++ )
	{
		PanelRectPtr = &(m_SelPanelList[i]);		
		if ( NULL == PanelRectPtr ) { continue; }
		PanelPtr = PanelRectPtr->PanelPtr;
		if ( NULL == PanelPtr ) { continue; }		
		PanelPtr->SetPanelBypassed(Bypass);
		PanelPtr->UpdatePanelBypassed();
	}		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditMainView::ExecMainBypassBoard()
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }
	const size_t SelCount = m_SelBoardList.size();	
	if ( 0 == SelCount ) { return false; }
	if ( MENU_MAIN_EDIT_MODE_BOARD != m_MainMode ) { return false; }	 
	if ( AOIDataCollect.OperateLevelEditFuncBypassBoard() == false )	{	return false; }

	size_t i=0;
	CAOIBoard      *BoardPtr = NULL;
	TBoardRect     *BoardRectPtr = NULL;
	BoardRectPtr = &(m_SelBoardList[0]);
	BoardPtr    =  BoardRectPtr->BoardPtr;
	if ( NULL == BoardPtr ) { return false; }
	bool            Bypass = BoardPtr->GetBoardBypassed();
	if ( true == Bypass ) { Bypass = false; }
	else { Bypass = true; }

	for ( i=0; i<SelCount; i++ )
	{
		BoardRectPtr = &(m_SelBoardList[i]);		
		if ( NULL == BoardRectPtr ) { continue; }
		BoardPtr = BoardRectPtr->BoardPtr;
		if ( NULL == BoardPtr ) { continue; }		
		BoardPtr->SetBoardBypassed(Bypass);
		BoardPtr->UpdateBoardBypassed();
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditMainView::ExecMainBypassComponent()
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }
	if ( AOIDataCollect.OperateLevelEditFuncBypassComponent() == false )	{	return false; }
	ProjectPtr->SwitchProjectComponentBypassed();
	LogOperCtrl.SaveLogProjectComponentSelectedBypassed(ProjectPtr);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditMainView::ExecMainBypass3DComponent()
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }
	if ( AOIDataCollect.OperateLevelEditFuncBypassComponent() == false )	{	return false; }
	ProjectPtr->SwitchProjectComponentBypass3D();
	LogOperCtrl.SaveLogProjectComponentSelectedBypass3D(ProjectPtr);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditMainView::LockUIWnd(bool bLock)
{
	AOIDataCollect.SetIsLockUIWnd(bLock);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditMainView::GetLockUIWnd() const//取得是否鎖住UIWnd
{
	return AOIDataCollect.GetIsLockUIWnd();
}
//-------------------------------------------------------------------------------------//
bool CEditMainView::ExecInspection_Finish()
{
	SwitchProject();
	m_UpdateTestMapTickCount = 0;
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }		
	
	CString   OfflineFdName;
	CString   OfflineFolder;		
	size_t    RepeatCount=0;
	size_t    MaxRepeatCount=0;
	bool      RepeatTest=false;
	bool      OfflineMode = AOIDataCollect.GetOfflineMode();
	TASK_MODE TaskMode = AOIDataCollect.GetTaskMode(); 			
	OFFLINE_FILE_MODE OfflineFileMode = ProjectPtr->GetProjectOfflineFileMode();
	AOIDataCollect.SetModelAttachedObj(MODEL_ATTACHED_COMPONENT);	
	switch ( TaskMode )
	{
	case TASK_ALIGN_PROJECT:		
		m_ImageZoom = 1.0;
		AOIDataCollect.SetOfflineMode(false);
		AOIDataCollect.SetProjectMapMode(false);		
		break;
	case TASK_INSPECT_PROJECT:
		RepeatTest = true;		
		AOIDataCollect.SendMainFrameWndMessage(MSG_EDIT_RESULT_LIST_WND, WPARAM_BUILD_RESULT_LIST, NULL);
		break;
	case TASK_TUNING_PROJECT:
	case TASK_TUNING_OFFLINE:
		if ( AOIDataCollect.GetIsRepeatTest() == true )
		{
			AOIDataCollect.AddRepeatedTestCount();
			RepeatCount = AOIDataCollect.GetRepeatedTestCount();
			MaxRepeatCount = AOIDataCollect.GetRepeatedTestMaxCount();
			if ( 0==MaxRepeatCount || RepeatCount<MaxRepeatCount )
			{	RepeatTest = true;	}
			else
			{	
				AOIDataCollect.SetIsRepeatTest(false); 
				AOIDataCollect.SetIsRepeatTestUI(false); 
			}

			if ( true == RepeatTest )
			{
				AOIDataCollect.SetIsNeedGrabFiducial(true);
				if ( AOIDataCollect.GetShowUIWndResultList() == true ) 
				{	AOIDataCollect.SendMainFrameWndMessage(MSG_EDIT_RESULT_LIST_WND, WPARAM_BUILD_RESULT_LIST, NULL); }
				if ( AOIDataCollect.ExecInspectionProjectTuning() == false )
				{
					LockUIWnd(false);
					JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
					return false;
				}
			}				
		}
		break;
	default:
		LockUIWnd(false);
		break;
	}		
	
	if ( false == RepeatTest )
	{	ExecInspectionFinishKernel(false);	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditMainView::ExecOnlineInspection_Finish()
{
	ExecInspectionFinishKernel(true);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditMainView::ExecInspectionFinishKernel(bool bOnline)
{
	LockUIWnd(false);
	if ( true == bOnline )
	{
		//m_ImageZoom = 1.0;
		AOIDataCollect.SetOfflineMode(false);
		AOIDataCollect.SetProjectMapMode(false);
		AOIDataCollect.ExecOnlineInspectionFinish();
	}
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }
	AOIDataCollect.SetDrawModelMode(DRAW_MODEL_RESULT);
	if ( AOIDataCollect.SetProjectLightSetting(ProjectPtr) == false )
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	}	
	ExecMoveToStage();
	bool bShowResultWnd = AOIDataCollect.GetShowUIWndResultList();
	const size_t DefectComponentCount = ProjectPtr->GetProjectDefectComponentCount();
	//AOIDataCollect.SendMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_UPDATE, (LPARAM)(this));	
	//if ( AOIDataCollect.GetInspectionFinishShowResultList() == true )		
	{	
		if ( true == bShowResultWnd )
		{	AOIDataCollect.SendMainFrameWndMessage(MSG_EDIT_RESULT_LIST_WND, WPARAM_BUILD_RESULT_LIST, NULL);	}
		else
		{	AOIDataCollect.SendMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_SHOW_RESULT_LIST_DOCK_PANE, TRUE); }
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditMainView::ExecMainCaptureProjectMap()
{
#ifndef OFFLINE_VERSION
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return false; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }	
	CString str;	
	const bool IsNeedGrabFiducial = AOIDataCollect.GetIsNeedGrabFiducial();
	const bool bMultiDistrictMode = ProjectPtr->GetProjectMultiDistrictMode();
	if ( true==IsNeedGrabFiducial && false==bMultiDistrictMode )
	{
		str = _T("Align board first, please");
		str = LoadMultiLanguageString(str, str);
		JetAPI::ShowMessageBox(str);
		return true;
	}

	CString    Folder;		
	CString    FdFilename;
	CString    MapFilename;
	CString    ProjectFolder;
	CString    ProjectFilename;
	CString    OfflineFolder;		
	DISTRICT_ID DistrictID;
	CProjectRegionMapWnd Wnd;

	Wnd.SetProjectPtr(ProjectPtr);	
	ProjectFilename = ProjectPtr->GetProjectFileName();	
	ProjectPtr->ReleaseProjectProgramFieldFrameImageBuffer();
	ProjectPtr->ReleaseProjectInspectionFieldFrameImageBuffer();
	const bool bOfflineMode = AOIDataCollect.GetOfflineMode();
	DistrictID = ProjectPtr->GetProjectActDistrictID();
	JetAPI::ExtractMainFileName(ProjectFilename, ProjectFolder);
	OfflineFolder = ProjectPtr->GetProjectProgramOfflineFolder();
	if ( Wnd.DoModal() == IDCANCEL )
	{	
		MapFilename.Format(_T("%s\\%s.INI"), ProjectFolder, _T("ProjectMap"));
		ProjectPtr->LoadProjectMapFile(MapFilename);
		ProjectPtr->ReleaseProjectProgramFieldFrameImageBuffer();
		ProjectPtr->SetProjectProgramFrameFileName(OfflineFolder);
		if ( true == bOfflineMode ) 
		{	
			FdFilename = AOIDataDefine.GetProjectOfflineFdName(OfflineFolder, DistrictID);
			ProjectPtr->LoadProjectOfflineFdFile(FdFilename);
			if ( AOIDataCollect.GetPreLoadProjectProgramImage() == true )
			{
				HCURSOR hCursor = ::AfxGetApp()->LoadStandardCursor(IDC_WAIT);
				HCURSOR hOldCursor = ::SetCursor(hCursor);
				if ( ProjectPtr->ExecProjectPreLoadProgramImage() == false )
				{	JetAPI::ShowMessageBox(ProjectPtr->GetErrorString());	}
				::SetCursor(hOldCursor);
			}				
		}		
		AOIDataCollect.SetProjectLightSetting(ProjectPtr);
		return true;	
	}	
	if ( ProjectPtr->ApplyProjectLibraryConfiguration() == false )
	{
		str = ProjectPtr->GetErrorString();
		JetAPI::ShowMessageBox(str);
		return false;
	}
	return true;	
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnMainEditCaptureProjectMap() 
{
	// TODO: Add your command handler code here	
#ifndef OFFLINE_VERSION
	if ( ExecMainCaptureProjectMap() == false ) { return; }	
	AOIDataCollect.SetCallbackWnd(GetSafeHwnd()); 	
	SwitchProject();	
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL != ProjectPtr )
	{
		if ( AOIDataCollect.SetProjectLightSetting(ProjectPtr) == false )
		{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString()); }
	}
	ExecMoveToStage();
	AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_MODIFY_MAP, NULL);	
#endif//OFFLINE_VERSION
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnUpdateMainEditCaptureProjectMap(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
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
bool CEditMainView::ExecMainDivideDistrictWnd()
{
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd )
	{	return false; }
	
	CAOIPanel *PanelPtr = ProjectPtr->GetProjectActivePanel();
	if ( NULL == PanelPtr )
	{
		PanelPtr = ProjectPtr->GetProjectPanelPtr(0, true);
		if ( NULL == PanelPtr ) { return false; }
		ProjectPtr->SetProjectActivePanel(PanelPtr);
	}

	CProjectDivideDistrictWnd Wnd;	
	Wnd.SetProjectPtr(ProjectPtr);
	Wnd.DoModal();
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnMainEditDivideDistrictWnd() 
{
	// TODO: Add your command handler code here	
	if ( ExecMainDivideDistrictWnd() == false ) { return; }		
	return;
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnUpdateMainEditDivideDistrictWnd(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd ) 
	{	pCmdUI->Enable(FALSE); }	
	else
	{
		const bool bMultiDivideDistrictMode = ProjectPtr->GetProjectMultiDistrictMode();
		if ( true == bMultiDivideDistrictMode )
		{	pCmdUI->Enable(TRUE);  }
		else
		{	pCmdUI->Enable(FALSE);  }
	}
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnMainEditAlignFiducial() 
{
	// TODO: Add your command handler code here
	CString str;	
	CAOIProject *ProjectPtr = CEditMainView::GetActiveProject();
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
void CEditMainView::OnUpdateMainEditAlignFiducial(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
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
bool CEditMainView::GetOfflineMode() const
{
	bool ShowMapMode = AOIDataCollect.GetProjectMapMode();
	if ( true == ShowMapMode ) { return true; }
	return AOIDataCollect.GetOfflineMode();
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnMainEditViewProjectMap() 
{
	// TODO: Add your command handler code here
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }	
	double PosX=0, PosY=0, PosZ=0;
	const bool bDistrictChange = false;
	const double ImageZoom = m_ImageZoom;//Map Zoom;	
	bool OfflineMode = AOIDataCollect.GetOfflineMode();	
	bool ProjectMapMode = AOIDataCollect.GetProjectMapMode();				
	const int MapScaleMode = ProjectPtr->GetProjectMapScaleMode();
	const double FovW = AOIDataCollect.GetFovSizeRealW();
	const double FovH = AOIDataCollect.GetFovSizeRealH();
	const double ZoomMin = AOIDataCollect.GetImageZoomMin();
	const double ZoomMax = AOIDataCollect.GetImageZoomMax();
	OFFLINE_FILE_MODE OldOfflineFileMode = ProjectPtr->GetProjectOfflineFileMode();
	OFFLINE_FILE_MODE NewOfflineFileMode = ProjectPtr->GetProjectOfflineFileMode();

	if ( true == ProjectMapMode )
	{	
		ProjectMapMode = false; 
		if ( false == OfflineMode )
		{	NewOfflineFileMode = OFFLINE_FILE_INSPECTION;	}
		else
		{	NewOfflineFileMode = OFFLINE_FILE_PROGRAM; }
	}
	else
	{	
		ProjectMapMode = true; 
		NewOfflineFileMode = OFFLINE_FILE_PROGRAM;
	}

	if ( false == ProjectMapMode )
	{	
		CalcFovPosition();
		AOIDataCollect.SetProjectMapMode(ProjectMapMode);
		AOIDataCollect.GetStagePos(PosX, PosY, PosZ);
		m_DrawFieldList = false;
		ResetImageOffset();
		m_ImageZoom = ImageZoom*MapScaleMode;
		if ( m_ImageZoom > ZoomMax ) 
		{	m_ImageZoom = ZoomMax; }
		if ( m_ImageZoom < ZoomMin ) 
		{	m_ImageZoom = ZoomMin; }

		if ( NewOfflineFileMode != OldOfflineFileMode )
		{
			if ( OFFLINE_FILE_PROGRAM==NewOfflineFileMode && false==m_LoadOfflineParam )
			{	m_LoadOfflineParam = true;	}
			if ( ProjectPtr->SwtichProjectOfflineFileMode(NewOfflineFileMode, bDistrictChange, OfflineMode) == false )
			{	JetAPI::ShowMessageBox(ProjectPtr->GetErrorString()); }
		}		
		SwitchProjectDistrictID(false);
		if ( true == OfflineMode )
		{
			FillCurrentFrames(1.0);
			BuildShowImageBuffer();
			CreateBKImage();
			RedrawWnd();
		}
		else
		{	ExecGrabFov(PosX, PosY, PosZ);	}		
	}
	else
	{	
		m_DrawFieldList = true;
		if ( NewOfflineFileMode!=OldOfflineFileMode || false==OfflineMode )
		{	
			if ( ProjectPtr->SwtichProjectOfflineFileMode(NewOfflineFileMode, bDistrictChange, false) == false )
			{	JetAPI::ShowMessageBox(ProjectPtr->GetErrorString()); }
		}
		AOIDataCollect.SetProjectMapMode(ProjectMapMode);		
		m_ImageZoom = ImageZoom/MapScaleMode;
		SwitchProjectDistrictID(false);
		CalcImageOffset();
		CreateBKImage();
		RedrawWnd();
	}	
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnUpdateMainEditViewProjectMap(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	bool ProjectMapMode = AOIDataCollect.GetProjectMapMode();
	if ( true == ProjectMapMode )
	{	pCmdUI->SetCheck(true); }
	else
	{	pCmdUI->SetCheck(false); }
}
//-------------------------------------------------------------------------------------//
const TPOINT2D& CEditMainView::GetImageResolution() const//取得影像解析度
{
	const bool ShowProjectMapMode = AOIDataCollect.GetProjectMapMode();
	if ( true == ShowProjectMapMode )
	{	return m_MapResolution; }
	else
	{	return m_FrameResolution; }
}
//-------------------------------------------------------------------------------------//
void CEditMainView::GetImageInfo(IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, TPOINT2D &ImageRes, TPOINT2D &StageCp)//取得目前畫面資訊
{
	const bool ShowProjectMapMode = AOIDataCollect.GetProjectMapMode();
	if ( true == ShowProjectMapMode )
	{
		ImageW = GetMapImageW();
		ImageH = GetMapImageH();
		ImageRes = m_MapResolution;
		StageCp.x = m_MapStageRgn.GetCpX();
		StageCp.y = m_MapStageRgn.GetCpY();		
	}
	else
	{
		ImageW = GetFrameImageW();
		ImageH = GetFrameImageH();
		ImageRes = m_FrameResolution;
		StageCp.x = m_FrameStageRgn.GetCpX();
		StageCp.y = m_FrameStageRgn.GetCpY();
	}
}
//-------------------------------------------------------------------------------------//
void CEditMainView::GetImageInfo(IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, TPOINT2D &ImageRes, TREGION4D &StageRgn)//取得目前畫面資訊
{
	const bool ShowProjectMapMode = AOIDataCollect.GetProjectMapMode();
	if ( true == ShowProjectMapMode )
	{
		ImageW = GetMapImageW();
		ImageH = GetMapImageH();
		ImageRes = m_MapResolution;
		StageRgn = m_MapStageRgn;
	}
	else
	{
		ImageW = GetFrameImageW();
		ImageH = GetFrameImageH();
		ImageRes = m_FrameResolution;
		StageRgn = m_FrameStageRgn;		
	}
}
//-------------------------------------------------------------------------------------//
void CEditMainView::SwitchImage(bool NextMap)//切換畫面
{
	const bool ShowProjectMapMode = AOIDataCollect.GetProjectMapMode();
	if ( true == ShowProjectMapMode )
	{	SwitchMapImage(NextMap);	}
	else
	{	SwitchFrameImage(NextMap);	}
}
//-------------------------------------------------------------------------------------//
bool CEditMainView::RetrieveCameraImage(WPARAM wParam, LPARAM lParam, bool bCameraCallBack)//取得相機影像
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
	const double Ratio = 1.0;
	double PosX=0, PosY=0, PosZ=0;	
	IMAGE_SIZE ImageW = AOIDataCollect.GetCameraImageW(CameraID);
	IMAGE_SIZE ImageH = AOIDataCollect.GetCameraImageH(CameraID);	
	double     ImageResX = AOIDataCollect.GetCameraResolutionX(CameraID);
	double     ImageResY = AOIDataCollect.GetCameraResolutionY(CameraID);
	const double FOVWum = AOIDataCollect.GetFovSizeRealW();
	const double FOVHum = AOIDataCollect.GetFovSizeRealH();	
	const double FovMinW = AOIDataCollect.GetFovSizeMinW_Zoom();
	const double FovMinH = AOIDataCollect.GetFovSizeMinH_Zoom();	
	const double TargetMinW = AOIDataCollect.GetTargetMinSizeW_Zoom();
	const double TargetMinH = AOIDataCollect.GetTargetMinSizeH_Zoom();
	const double TargetOffsetX = AOIDataCollect.GetFovTargetOffsetX();
	const double TargetOffsetY = AOIDataCollect.GetFovTargetOffsetY();

	MotionCtrlPtr->GetCurrentPos(PosX, PosY, PosZ);	
	m_FrameRatio = Ratio;
	ResetImageOffset();
	m_FrameResolution.x = ImageResX;
	m_FrameResolution.y = ImageResY;
	m_FrameStageRgn.minX = PosX-(FOVWum*0.5);
	m_FrameStageRgn.maxX = PosX+(FOVWum*0.5);
	m_FrameStageRgn.minY = PosY-(FOVHum*0.5);
	m_FrameStageRgn.maxY = PosY+(FOVHum*0.5);	

	TPOINT2D PosCad;
	TPOINT2D ImageRes;		
	TPOINT2D PosStage(TargetOffsetX, TargetOffsetY);		
	AOIDataCollect.MapStageOffsetPtToCad(PosStage, PosCad);	
	ImageRes.x = AOIDataCollect.GetCameraResolutionX(CameraID);
	ImageRes.y = AOIDataCollect.GetCameraResolutionY(CameraID);
	m_ImageOffset.x = -PosCad.x/(ImageRes.x);
	m_ImageOffset.y =  PosCad.y/(ImageRes.y);
	m_ImageOffset.x = m_ImageOffset.x/(m_ImageZoom);
	m_ImageOffset.y = m_ImageOffset.y/(m_ImageZoom);
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
	unsigned int ImageIndex = 0;
	ReleaseUniFrameBuffer();
	ReleaseShowImageBuffer();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr )
	{	ImageIndex = 0; }
	else
	{	ImageIndex = ProjectPtr->GetProjectMapIndex(); }	
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
	//UpdateModelStats();	
	CreateBKImage();
	RedrawWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnMainEditManiPasteToOtherBoards() 
{
	// TODO: Add your command handler code here
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd ) { return; }
	CString   str;
	AOIDataCollect.SetManipulateMainMode(MANIPULATE_MAIN_SELECT);
	if ( ExecMainCloneObject() == false )
	{	return; }		
	if ( ProjectPtr->PasteProjectComponentListToOtherBoards(m_CloneComponentList) == false )
	{
		JetAPI::ShowMessageBox(ProjectPtr->GetErrorString());
		return;
	}

	str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("ProjectPos.TXT"));
	ProjectPtr->SaveProjectObjectPosition(str);

	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_PART_LIST_WND, WPARAM_BUILD_PART_LIST, LPARAM_BUILD_DOCK_LIST_EDIT);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_RESULT_LIST_WND, WPARAM_BUILD_RESULT_LIST, NULL);	
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnUpdateMainEditManiPasteToOtherBoards(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd || MENU_MAIN_EDIT_MODE_COMPONENT!=m_MainMode ) 
	{	pCmdUI->Enable(FALSE); }	
	else
	{	pCmdUI->Enable(TRUE); }	
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnMainEditManiPasteToOtherPanels()
{
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd ) { return; }
	CString   str;
	AOIDataCollect.SetManipulateMainMode(MANIPULATE_MAIN_SELECT);
	if ( ExecMainCloneObject() == false )
	{	return; }		
	if ( ProjectPtr->PasteProjectComponentListToOtherPanels(m_CloneComponentList) == false )
	{
		JetAPI::ShowMessageBox(ProjectPtr->GetErrorString());
		return;
	}

	str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("ProjectPos.TXT"));
	ProjectPtr->SaveProjectObjectPosition(str);

	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_PART_LIST_WND, WPARAM_BUILD_PART_LIST, LPARAM_BUILD_DOCK_LIST_EDIT);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_RESULT_LIST_WND, WPARAM_BUILD_RESULT_LIST, NULL);	
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnUpdateMainEditManiPasteToOtherPanels(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd || MENU_MAIN_EDIT_MODE_COMPONENT!=m_MainMode ) 
	{	pCmdUI->Enable(FALSE); }	
	else
	{	pCmdUI->Enable(TRUE); }	
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnMainEditManiProjectMark() 
{
	// TODO: Add your command handler code here
	bool bLockUIWnd = GetLockUIWnd();
	bool ProjectMapMode = AOIDataCollect.GetProjectMapMode();
	if ( true == bLockUIWnd || true==ProjectMapMode ) { return ; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	m_ProjectMarkWnd.SetProjectPtr(ProjectPtr);
	m_ProjectMarkWnd.ShowWindow(SW_SHOW);
	AOIDataCollect.SetManipulateMainMode(MANIPULATE_MAIN_MARK);
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnUpdateMainEditManiProjectMark(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	bool bLockUIWnd = GetLockUIWnd();
	bool ProjectMapMode = AOIDataCollect.GetProjectMapMode();	
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd || true==ProjectMapMode ) 
	{	pCmdUI->Enable(FALSE); }	
	else
	{	pCmdUI->Enable(TRUE); }	
	pCmdUI->SetCheck(MANIPULATE_MAIN_MARK == AOIDataCollect.GetManipulateMainMode());
}
//-------------------------------------------------------------------------------------//
bool CEditMainView::ExecMainProjectMark()
{
	const char fnName[] = "CEditMainView::ExecMainProjectMark";
	MANIPULATE_MAIN_MODE ManiMainMode = AOIDataCollect.GetManipulateMainMode();
	if ( MANIPULATE_MAIN_MARK != ManiMainMode ) { return true; }
	bool bLockUIWnd = GetLockUIWnd();
	bool ProjectMapMode = AOIDataCollect.GetProjectMapMode();
	if ( true == bLockUIWnd || true==ProjectMapMode ) { return true; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }
	const unsigned int MapIndex = ProjectPtr->GetProjectMapIndex();
	const size_t MaxFrameCount = GetMaxFrameCount();
	if ( MapIndex >= MaxFrameCount ) { return false; }
	
	RECT       RoiRect={0};
	TPOINT2D   StageCp;
	TPOINT3D   StagePos;
	TPOINT2D   ImageRes;
	TREGION4D  ImageRgn;
	TREGION4D  StageRgn;
	IMAGE_SIZE ImageW  = 0;
	IMAGE_SIZE ImageH  = 0;
	IMAGE_SIZE ImageStep = 0;	
	IMAGE_SIZE MarkW  = 0;
	IMAGE_SIZE MarkH  = 0;
	IMAGE_SIZE MarkStep =0;
	IMAGE_SIZE BitCount =0;
	IMAGE_PTR  MarkPtr = NULL;
	IMAGE_PTR  ImagePtr = NULL;
	TUNI_FRAME UniFrame = m_UniFrameList[MapIndex];

	TPOINT2D   WndPt1, ImagePt1, StagePt1;
	TPOINT2D   WndPt2, ImagePt2, StagePt2;

	GetImageInfo(ImageW, ImageH, ImageRes, StageCp);		
	StagePt1 = ImagePt1 = WndPt1 = m_MousePosFirst;
	StagePt2 = ImagePt2 = WndPt2 = m_MousePosLast;
	MapWndPtToImagePt_DBL(ImageW, ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, WndPt1, ImagePt1);
	MapWndPtToImagePt_DBL(ImageW, ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, WndPt2, ImagePt2);
	AOIDataCollect.MapCameraPtToStage(ImageW, ImageH, ImageRes, ImagePt1, StageCp, StagePt1);
	AOIDataCollect.MapCameraPtToStage(ImageW, ImageH, ImageRes, ImagePt2, StageCp, StagePt2);	
	ImageRgn.minX = MIN(ImagePt1.x, ImagePt2.x);
	ImageRgn.maxX = MAX(ImagePt1.x, ImagePt2.x);
	ImageRgn.minY = MIN(ImagePt1.y, ImagePt2.y);
	ImageRgn.maxY = MAX(ImagePt1.y, ImagePt2.y);
	StageRgn.minX = MIN(StagePt1.x, StagePt2.x);
	StageRgn.maxX = MAX(StagePt1.x, StagePt2.x);
	StageRgn.minY = MIN(StagePt1.y, StagePt2.y);
	StageRgn.maxY = MAX(StagePt1.y, StagePt2.y);
	MotionCtrlPtr->GetCurrentPos(StagePos.x, StagePos.y, StagePos.z);
	StagePos.x = StageRgn.GetCpX();
	StagePos.y = StageRgn.GetCpY();	

	ImageW = UniFrame.ImageW;
	ImageH = UniFrame.ImageH;	
	BitCount = UniFrame.BitCount;
	ImageStep = UniFrame.ImageStep;	
	ImagePtr = UniFrame.ImagePtr;
	JetAPI::Region4DToRect(ImageRgn, RoiRect, false);	
	MarkW = RoiRect.right-RoiRect.left;
	MarkH = RoiRect.bottom-RoiRect.top;
	MarkStep = JetAPI::GetBMPImagePixelsPerLine(MarkW, BitCount, 4);
	const size_t MarkSize = ImageAPI.CalcBufferSize(MarkStep, MarkH);
	if ( 0 == MarkSize ) { return false; }

	if ( JetMemory.alloc_func(MarkSize, MarkPtr, fnName, "MarkPtr") == false )
	{	return false; }

	if ( ImageAPI.ExtractRoiImage3(ImageW, ImageH, ImageStep, BitCount, ImagePtr, RoiRect, MarkStep, MarkPtr, false) == false )
	{
		JetMemory.free_func(MarkPtr);
		return false;
	}	
	m_ProjectMarkWnd.SetMarkFrameIndex(MapIndex);
	m_ProjectMarkWnd.SetMarkStagePos(StagePos.x, StagePos.y, StagePos.z);
	m_ProjectMarkWnd.SetMarkImage(MarkW, MarkH, MarkStep, BitCount, MarkPtr);
	m_ProjectMarkWnd.ShowWindow(SW_SHOW);	
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnMainEditSetBarcodeDeviceIndex() 
{
	// TODO: Add your command handler code here	
	switch ( m_MainMode )
	{
	case MENU_MAIN_EDIT_MODE_PANEL:	
		ExecMainSetBarcodeDeviceIndexPanel();
		break;
	case MENU_MAIN_EDIT_MODE_BOARD:
		ExecMainSetBarcodeDeviceIndexBoard();
		break;
	case MENU_MAIN_EDIT_MODE_COMPONENT:		
		break;
	}
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnUpdateMainEditSetBarcodeDeviceIndex(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd || MENU_MAIN_EDIT_MODE_COMPONENT==m_MainMode) 
	{	pCmdUI->Enable(FALSE); }	
	else
	{	pCmdUI->Enable(TRUE); }	
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnMainEditSetBarcodeDeviceCodeIndex() 
{
	// TODO: Add your command handler code here	
	switch ( m_MainMode )
	{
	case MENU_MAIN_EDIT_MODE_PANEL:	
		ExecMainSetBarcodeDeviceCodeIndexPanel();
		break;
	case MENU_MAIN_EDIT_MODE_BOARD:
		ExecMainSetBarcodeDeviceCodeIndexBoard();
		break;
	case MENU_MAIN_EDIT_MODE_COMPONENT:		
		break;
	}
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnUpdateMainEditSetBarcodeDeviceCodeIndex(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd || MENU_MAIN_EDIT_MODE_COMPONENT==m_MainMode) 
	{	pCmdUI->Enable(FALSE); }	
	else
	{	pCmdUI->Enable(TRUE); }	
}
//-------------------------------------------------------------------------------------//
bool CEditMainView::ExecMainSetBarcodeDeviceIndexPanel()
{
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd || MENU_MAIN_EDIT_MODE_COMPONENT==m_MainMode) 
	{	return false; }
	
	CString      strCaption;
	CString      strLabel;
	CString      strValue;
	CInputBoxWnd InputBox;
	CAOIPanel   *PanelPtr = NULL;

	PanelPtr = ProjectPtr->GetProjectPanelPtrBySelected();
	if ( NULL == PanelPtr ) { return false; }
	strCaption = _T("Barcode Device Index Setting");
	strLabel = _T("Barcode Device Index");
	strValue.Format(_T("%d"), PanelPtr->GetPanelBarcodeDeviceIndex()+1);

	strLabel = LoadMultiLanguageString(strLabel, strLabel);
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	InputBox.SetParam1(strCaption, strLabel, strValue);
	if ( InputBox.DoModal() == IDCANCEL ) 
	{	return false;  }
	const unsigned int BoardDeviceIndex = (unsigned int)(::_ttoi(InputBox.m_DataEdit1)-1);
	if ( BoardDeviceIndex<0 || BoardDeviceIndex>=MAX_BARCODE_DEVICE_COUNT ) 
	{	return false; }
	
	ProjectPtr->SetProjectPanelSelectedBarcodeDeviceIndex(BoardDeviceIndex);
	LogOperCtrl.SaveLogProjectPanelSelectedBarcodeDeviceIndex(ProjectPtr, BoardDeviceIndex);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditMainView::ExecMainSetBarcodeDeviceIndexBoard()
{
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd || MENU_MAIN_EDIT_MODE_COMPONENT==m_MainMode) 
	{	return false; }
	
	CString      strCaption;
	CString      strLabel;
	CString      strValue;
	CInputBoxWnd InputBox;	
	CAOIBoard   *BoardPtr = NULL;

	BoardPtr = ProjectPtr->GetProjectBoardPtrBySelected();
	if ( NULL == BoardPtr ) { return false; }
	strCaption = _T("Barcode Device Index Setting");
	strLabel = _T("Barcode Device Index");
	strValue.Format(_T("%d"), BoardPtr->GetBoardBarcodeDeviceIndex()+1);

	strLabel = LoadMultiLanguageString(strLabel, strLabel);
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	InputBox.SetParam1(strCaption, strLabel, strValue);
	if ( InputBox.DoModal() == IDCANCEL ) 
	{	return false;  }
	const unsigned int BoardDeviceIndex = (unsigned int)(::_ttoi(InputBox.m_DataEdit1)-1);
	if ( BoardDeviceIndex<0 || BoardDeviceIndex>=MAX_BARCODE_DEVICE_COUNT ) 
	{	return false; }
	ProjectPtr->SetProjectBoardSelectedBarcodeDeviceIndex(BoardDeviceIndex);	
	LogOperCtrl.SaveLogProjectBoardSelectedBarcodeDeviceIndex(ProjectPtr, BoardDeviceIndex);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditMainView::ExecMainSetBarcodeDeviceCodeIndexPanel()
{
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd || MENU_MAIN_EDIT_MODE_COMPONENT==m_MainMode) 
	{	return false; }

	CString      strCaption;
	CString      strLabel;
	CString      strValue;
	CInputBoxWnd InputBox;
	CAOIPanel   *PanelPtr = NULL;	

	PanelPtr = ProjectPtr->GetProjectPanelPtrBySelected();
	if ( NULL == PanelPtr ) { return false; }

	strCaption = _T("Barcode Code Index Setting");
	strLabel = _T("Barcode Code Index");
	strValue.Format(_T("%d"), PanelPtr->GetPanelBarcodeDeviceCodeIndex()+1);

	strLabel = LoadMultiLanguageString(strLabel, strLabel);
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	InputBox.SetParam1(strCaption, strLabel, strValue);
	if ( InputBox.DoModal() == IDCANCEL ) 
	{	return false;  }
	const int BoardDeviceCodeIndex = ::_ttoi(InputBox.m_DataEdit1)-1;
	if ( BoardDeviceCodeIndex<0 || BoardDeviceCodeIndex>=MAX_BARCODE_DEVICE_CODE_COUNT ) 
	{	return false; }	
	ProjectPtr->SetProjectPanelSelectedBarcodeCodeIndex(BoardDeviceCodeIndex);		
	LogOperCtrl.SaveLogProjectPanelSelectedBarcodeCodeIndex(ProjectPtr, BoardDeviceCodeIndex);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditMainView::ExecMainSetBarcodeDeviceCodeIndexBoard()
{	
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd || MENU_MAIN_EDIT_MODE_COMPONENT==m_MainMode) 
	{	return false; }

	CString      strCaption;
	CString      strLabel;
	CString      strValue;
	CInputBoxWnd InputBox;	
	CAOIBoard   *BoardPtr = NULL;

	BoardPtr = ProjectPtr->GetProjectBoardPtrBySelected();
	if ( NULL == BoardPtr ) { return false; }

	strCaption = _T("Barcode Code Index Setting");
	strLabel = _T("Barcode Code Index");
	strValue = _T("1");

	strLabel = LoadMultiLanguageString(strLabel, strLabel);
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	InputBox.SetParam1(strCaption, strLabel, strValue);
	if ( InputBox.DoModal() == IDCANCEL ) 
	{	return false;  }
	const int BoardDeviceCodeIndex = ::_ttoi(InputBox.m_DataEdit1)-1;
	if ( BoardDeviceCodeIndex<0 || BoardDeviceCodeIndex>=MAX_BARCODE_DEVICE_CODE_COUNT ) 
	{	return false; }
	ProjectPtr->SetProjectBoardSelectedBarcodeCodeIndex(BoardDeviceCodeIndex);
	LogOperCtrl.SaveLogProjectBoardSelectedBarcodeCodeIndex(ProjectPtr, BoardDeviceCodeIndex);
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditMainView::ExecMainEditSaveComponentSample(SAVE_SPC_PART_IMAGE_MODE SaveMode)
{
	CString str;	
	bool IsNeedGrabFiducial = AOIDataCollect.GetIsNeedGrabFiducial();
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( true == bLockUIWnd || NULL==ProjectPtr ) { return; }

	if ( true == IsNeedGrabFiducial )
	{	ProjectPtr->SelectProjectAllFds(true);	 }
	else
	{	ProjectPtr->SelectProjectAllFds(false);	 }		
	ProjectPtr->SelectProjectAllFds(true);
	//ProjectPtr->SelectProjectAllBarcodes(true);
	ProjectPtr->SelectProjectAllBarcodes(false);//不檢測條碼
	ProjectPtr->ResetProjectOnlineTuningDateTime();
	if ( SAVE_SPC_PART_IMAGE_EVERYONE == SaveMode )
	{	
		ProjectPtr->SelectProjectAllComponents(true);
		ProjectPtr->UpdateSelectObjToNeedToCalculateRgn();
		ProjectPtr->SelectProjectAllComponents(false);
		ProjectPtr->SelectProjectActiveComponent(true);
	}	
	if ( SAVE_SPC_PART_IMAGE_SELECTED==SaveMode || SAVE_SPC_PART_IMAGE_SELECTED_ADD==SaveMode || SAVE_SPC_PART_IMAGE_SELECTED_COPY==SaveMode )
	{	ProjectPtr->UpdateSelectObjToNeedToCalculateRgn();	}	
	ProjectPtr->SetProjectSaveSpcPartImageMode(SaveMode);
	ProjectPtr->SetProjectSpcFileSaveEnabled(false);

	AOIDataCollect.SetIsRepeatTest(false);
	AOIDataCollect.SetIsRepeatTestUI(false);
	AOIDataCollect.SetRepeatedTestCount(0);
	AOIDataCollect.SetRepeatedTestMaxCount(0);
	AOIDataCollect.SetCallbackWnd(CWnd::GetSafeHwnd());	
	AOIDataCollect.SetInspectingMode(INSPECTING_TUNNING);
	AOIDataCollect.ReleaseModelUniFrameList();
	SendMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_CLOSE_ACTIVE_OBJ, NULL);
	if ( AOIDataCollect.GetOfflineMode() == false )
	{	AOIDataCollect.SetTaskMode(TASK_TUNING_PROJECT);	}
	else
	{	AOIDataCollect.SetTaskMode(TASK_TUNING_OFFLINE);	}	
	LogOperCtrl.SaveLogProject(ProjectPtr, _T("Save Component Sample Image"));
	if ( AOIDataCollect.ExecInspectionProjectTuning() == false )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return;
	}
	LockUIWnd(true);
	CreateMapImage();
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnMainEditSaveComponentSample() 
{
	// TODO: Add your command handler code here
	CString str;
	str = _T("Do you want to save all component's images?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) != IDYES )
	{	return; }
	ExecMainEditSaveComponentSample(SAVE_SPC_PART_IMAGE_EVERYONE);	
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnUpdateMainEditSaveComponentSample(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd ) 
	{	pCmdUI->Enable(FALSE); }	
	else
	{	pCmdUI->Enable(TRUE); }	
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnMainEditAddComponentSample()
{
	ExecMainEditSaveComponentSample(SAVE_SPC_PART_IMAGE_SELECTED_ADD);	
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnUpdateMainEditAddComponentSample(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd ) 
	{	pCmdUI->Enable(FALSE); }	
	else
	{	pCmdUI->Enable(TRUE); }	
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnMainEditReplaceComponentSample()
{
	ExecMainEditSaveComponentSample(SAVE_SPC_PART_IMAGE_SELECTED);	
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnUpdateMainEditReplaceComponentSample(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd ) 
	{	pCmdUI->Enable(FALSE); }	
	else
	{	pCmdUI->Enable(TRUE); }	
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnMainEditCopyComponentSample()
{	
	ExecMainEditSaveComponentSample(SAVE_SPC_PART_IMAGE_SELECTED_COPY);	
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnUpdateMainEditCopyComponentSample(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd ) 
	{	pCmdUI->Enable(FALSE); }	
	else
	{	pCmdUI->Enable(TRUE); }	
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnMainEditProjectMapMask() 
{
	// TODO: Add your command handler code here
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd ) 
	{	return; }
	CProjectMapMaskWnd Wnd;
	Wnd.SetProjectPtr(ProjectPtr);
	if ( Wnd.DoModal() == IDCANCEL )
	{	return; }

	IMAGE_PTR  MaskPtr=NULL;
	IMAGE_SIZE MaskW=0;
	IMAGE_SIZE MaskH=0;
	IMAGE_SIZE MaskStep=0;
	IMAGE_SIZE MaskBitCount=0;	
	MaskPtr = Wnd.GetMaskImage(MaskW, MaskH, MaskStep, MaskBitCount);
	ProjectPtr->SetProjectMapMaskImage(MaskW, MaskH, MaskStep, MaskBitCount, MaskPtr, true);
	LogOperCtrl.SaveLogProject(ProjectPtr, _T("Edit Project Map Mask"));
	return;
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnUpdateMainEditProjectMapMask(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd ) 
	{	pCmdUI->Enable(FALSE); }	
	else
	{	pCmdUI->Enable(TRUE); }	
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnMainEditProjectMapMaskRegion()
{
	// TODO: Add your control notification handler code here
	if (NULL == m_ProjectPtr) { return; }
	CProjectMapSpecRegionWnd Wnd;
	Wnd.SetProjectPtr(m_ProjectPtr);
	if (Wnd.DoModal() == IDCANCEL)
	{
		return;
	}

	IMAGE_PTR  MaskPtr = NULL;
	IMAGE_SIZE MaskW = 0;
	IMAGE_SIZE MaskH = 0;
	IMAGE_SIZE MaskStep = 0;
	IMAGE_SIZE MaskBitCount = 0;
	TREGION4D StageRegion;

	MaskPtr = Wnd.GetMaskImage(MaskW, MaskH, MaskStep, MaskBitCount);
	m_ProjectPtr->SetProjectMapMaskImage(MaskW, MaskH, MaskStep, MaskBitCount, MaskPtr, true);
	LogOperCtrl.SaveLogProject(m_ProjectPtr, _T("Edit Project Map Mask"));
	if (false == Wnd.GetSpecTotalStageRegion(StageRegion)) { return; }
	//m_ProjectPtr->CreateProjectComponentForSpecRegion(StageRegion);
	//LogOperCtrl.SaveLogProject(m_ProjectPtr, _T("Create [Full-Map] Component (SpecRegion)"));
	AOIDataCollect.SendMainFrameWndMessage(MSG_EDIT_PART_LIST_WND, WPARAM_BUILD_PART_LIST, LPARAM_BUILD_DOCK_LIST_ALL);
	AOIDataCollect.PostMainFrameWndMessage(WM_COMMAND, ID_REBUILD_INSPECTION_FIELD, NULL);

	//m_ProjectPtr->GetProjectMapInfo_DA(MapRes, MapCadRgn, MpaStageRgnDA);
	//m_ProjectPtr->GetProjectMapInfo_DB(MapRes, MapCadRgn, MpaStageRgnDB);

	//CProjectFieldConfigWnd Wnddebug;
	//Wnddebug.SetProjectPtr(m_ProjectPtr);
	//Wnddebug.DoModal();
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnUpdateMainEditProjectMapMaskRegion(CCmdUI * pCmdUI)
{
	// TODO: Add your command update UI handler code here
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd ) 
	{	pCmdUI->Enable(FALSE); }	
	else
	{	pCmdUI->Enable(TRUE); }	
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnMainEditProjectCompare()
{
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd ) 
	{	return; }

	CProjectCompareWnd Wnd;
	Wnd.SetHostProjectPtr(ProjectPtr);
	AOIDataCollect.ReleaseModelUniFrameList();
	Wnd.DoModal();
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_PART_LIST_WND, WPARAM_BUILD_PART_LIST, LPARAM_BUILD_DOCK_LIST_ALL);
	return;
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnUpdateMainEditProjectCompare(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd ) 
	{	pCmdUI->Enable(FALSE); }	
	else
	{	pCmdUI->Enable(TRUE); }
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnMainEditComponentCompare()
{
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd ) 
	{	return; }

	TCHAR szFilters[]=_T("TXT Files (*.TXT)|*.TXT|All Files (*.*)|*.*||");
	CFileDialog dialog (TRUE, _T("TXT;*"), _T("*.TXT"), OFN_FILEMUSTEXIST, szFilters);
	if ( dialog.DoModal() == IDCANCEL )
	{	return ; }

	CString str;
	std::vector<std::wstring> NameList;
	CString filename=dialog.GetPathName();
	if ( AOIDataCollect.LoadNameFile(filename, NameList) == false )
	{
		str = AOIDataCollect.GetErrorString();
		return;
	}	

	size_t          i=0;
	POINT           Point;
	CString         strPanel;
	CString         strLabel;
	CString         strCaption;
	TListNode       Node;
	CInputListWnd   EnumWnd;	
	DWORD_PTR       dwDefault=CHANGE_COMPARE_PARAM_BYPASS;
	std::vector<TListNode> NodelList;		

	strPanel = AOIDataDefine.GetPanelText();
	strLabel = _T("Set Change Mode");
	strLabel = LoadMultiLanguageString(strLabel, strLabel);
	strCaption = _T("Change Mode");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);		
	Node.Data = CHANGE_COMPARE_PARAM_TEST;
	Node.Text = AOIDataDefine.GetTestText();
	NodelList.push_back(Node);
	Node.Data = CHANGE_COMPARE_PARAM_BYPASS;
	Node.Text = AOIDataDefine.GetBypassedText();
	NodelList.push_back(Node);	
	EnumWnd.SetParam1(strCaption, strLabel, dwDefault, NodelList);	
	if ( EnumWnd.DoModal() == IDCANCEL )
	{	return ; }		

	DWORD Res = 0;
	str = _T("Do you want to reset all components?");
	str = LoadMultiLanguageString(str, str);
	Res = JetAPI::ShowMessageBox(str, MB_YESNOCANCEL);
	if ( IDCANCEL == Res ) 
	{	return; }

	bool bResetAll = false;
	if ( IDYES == Res ) { bResetAll = true; }	
	else { bResetAll = false; }
	CHANGE_COMPARE_PARAM_MODE ChangeMode = (CHANGE_COMPARE_PARAM_MODE)(EnumWnd.GetSelData());
	ProjectPtr->ChangeProjectComponentParam(NameList, ChangeMode, bResetAll);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_PART_LIST_WND, WPARAM_UPDATE_PART_STATES, NULL);
	return;
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnUpdateMainEditComponentCompare(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd ) 
	{	pCmdUI->Enable(FALSE); }	
	else
	{	pCmdUI->Enable(TRUE); }
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnMainEditFullMapComponentCreate()
{
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd ) 
	{	return; }

	CString str;
	str = _T("Do you want to create the components for full project map?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO ) 
	{	return; }

	if ( ProjectPtr->CreateProjectComponentForFullProjectMap() == false )
	{
		JetAPI::ShowMessageBox(ProjectPtr->GetErrorString());
		return;
	}
	RedrawWnd();
	LogOperCtrl.SaveLogProject(ProjectPtr, _T("Create [Full-Map] Component"));
	AOIDataCollect.SendMainFrameWndMessage(MSG_EDIT_PART_LIST_WND, WPARAM_BUILD_PART_LIST, LPARAM_BUILD_DOCK_LIST_ALL);
	AOIDataCollect.PostMainFrameWndMessage(WM_COMMAND, ID_REBUILD_INSPECTION_FIELD, NULL);	
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnUpdateMainEditFullMapComponentCreate(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd ) 
	{	pCmdUI->Enable(FALSE); }	
	else
	{	pCmdUI->Enable(TRUE); }
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnMainEditFullMapComponentClear()
{
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd ) 
	{	return; }

	CString str;
	str = _T("Do you want to clear all full-map components?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO ) 
	{	return; }	
	if ( ProjectPtr->DeleteProjectComponentType(COMPONENT_TYPE_FULL_MAP) == false )
	{
		JetAPI::ShowMessageBox(ProjectPtr->GetErrorString());
		return;
	}
	RedrawWnd();
	LogOperCtrl.SaveLogProject(ProjectPtr, _T("Clear [Full-Map] Component"));
	AOIDataCollect.SendMainFrameWndMessage(MSG_EDIT_PART_LIST_WND, WPARAM_BUILD_PART_LIST, LPARAM_BUILD_DOCK_LIST_ALL);
	AOIDataCollect.PostMainFrameWndMessage(WM_COMMAND, ID_REBUILD_INSPECTION_FIELD, NULL);
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnUpdateMainEditFullMapComponentClear(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd ) 
	{	pCmdUI->Enable(FALSE); }	
	else
	{	pCmdUI->Enable(TRUE); }
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnMainEditSetBoardOrder()
{
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd ) 
	{	return; }

	CBoardConfigWnd Wnd;	
	std::vector<CAOIBoard*> BoardListBefore;	
	const size_t ProjectBoardCount=ProjectPtr->GetProjectBoardCount();			
	BoardListBefore.resize(ProjectBoardCount, NULL);
	for ( size_t i=0; i<ProjectBoardCount; i++ )
	{
		CAOIBoard *BoardPtr=ProjectPtr->GetProjectBoardPtr(i, false);
		if ( NULL == BoardPtr ) { continue; }			
		BoardPtr->SetBoardTempInt_01((int)(i));									
		BoardPtr = BoardPtr->CloneBoardObj();
		if ( NULL == BoardPtr ) { continue; }
		BoardPtr->RemoveBoardAllObjects();
		BoardListBefore[i] = BoardPtr;
	}

	Wnd.SetProjectPtr(ProjectPtr);
	Wnd.SetBoardConfigMode(BOARD_CONFIG_ORDER);
	DWORD Res=Wnd.DoModal();
	if ( IDOK == Res )
	{		
		const size_t BoardCountBefore=BoardListBefore.size();
		for ( size_t i=0; i<ProjectBoardCount; i++ )
		{
			CAOIBoard *BoardPtr_After=ProjectPtr->GetProjectBoardPtr(i, true);
			if ( NULL == BoardPtr_After ) { continue; }
			unsigned int BoardIndex=(unsigned int)(BoardPtr_After->GetBoardTempInt_01());
			if ( BoardIndex >= BoardCountBefore ) { continue; }
			CAOIBoard *BoardPtr_Before=BoardListBefore[BoardIndex];				
			if ( NULL == BoardPtr_Before ) { continue; }		
			LogOperCtrl.SaveLogBoardCompare(BoardPtr_Before, BoardPtr_After);
		}
		AOIObjManager.DestroyBoardList(BoardListBefore);		
	}
	AOIDataCollect.SendMainFrameWndMessage(MSG_EDIT_PART_LIST_WND, WPARAM_BUILD_PART_LIST, LPARAM_BUILD_DOCK_LIST_ALL);
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnUpdateMainEditSetBoardOrder(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd ) 
	{	pCmdUI->Enable(FALSE); }	
	else
	{	pCmdUI->Enable(TRUE); }
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnMainEditAutoAddComponet()
{
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd ) 
	{	return; }
	CAutoAddComponentWnd Wnd;
	Wnd.SetProjectPtr(ProjectPtr);
	DWORD Res = Wnd.DoModal();
}
//-------------------------------------------------------------------------------------//
void CEditMainView::OnUpdateMainEditAutoAddComponet(CCmdUI * pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd ) 
	{	pCmdUI->Enable(FALSE); }	
	else
	{	pCmdUI->Enable(TRUE); }
}
//-------------------------------------------------------------------------------------//