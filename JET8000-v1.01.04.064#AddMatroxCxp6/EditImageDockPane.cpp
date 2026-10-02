// EditImageDockPane.cpp : 實作檔
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "resource.h"
#include "EditImageDockPane.h"
//-------------------------------------------------------------------------------------//
const UINT ID_EDIT_IMAGE_VIEW   = 102;
//-------------------------------------------------------------------------------------//
// CEditImageDockPane
//-------------------------------------------------------------------------------------//
IMPLEMENT_DYNAMIC(CEditImageDockPane, CDockablePane)
//-------------------------------------------------------------------------------------//
CEditImageDockPane::CEditImageDockPane()
{

}
//-------------------------------------------------------------------------------------//
CEditImageDockPane::~CEditImageDockPane()
{
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CEditImageDockPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
// CEditImageDockPane 訊息處理常式
//-------------------------------------------------------------------------------------//
int CEditImageDockPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此加入特別建立的程式碼
	CRect rectDummy;
	rectDummy.SetRectEmpty();
	const DWORD dwPaneStype = WS_CHILD | WS_VISIBLE | WS_BORDER;	
	if ( !m_EditImageView.Create(dwPaneStype, rectDummy, this, ID_EDIT_IMAGE_VIEW))
	{
		TRACE0("無法建立 [編輯影像視窗]\n");
		return -1;      // 無法建立
	}
	return 0;
}
//-------------------------------------------------------------------------------------//
void CEditImageDockPane::OnChangeVisualStyle()
{
	/*
	m_ClassViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_CLASS_VIEW_24 : IDB_CLASS_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("無法載入點陣圖: %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	m_ClassViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_ClassViewImages.Add(&bmp, RGB(255, 0, 0));

	m_wndClassView1.SetImageList(&m_ClassViewImages, TVSIL_NORMAL);
	m_wndClassView2.SetImageList(&m_ClassViewImages, TVSIL_NORMAL);
//	m_wndClassView3.SetImageList(&m_ClassViewImages, TVSIL_NORMAL);

	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_SORT_24 : IDR_SORT, 0, 0, TRUE);
	*/
}
//-------------------------------------------------------------------------------------//
void CEditImageDockPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: 在此加入您的訊息處理常式程式碼
	if ( GetSafeHwnd() == NULL ) { return; }
	if ( m_EditImageView.GetSafeHwnd() != NULL )
	{	m_EditImageView.SetWindowPos (NULL, 0, 0, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);}
}
//-------------------------------------------------------------------------------------//
void CEditImageDockPane::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDockablePane::OnShowWindow(bShow, nStatus);

	// TODO: 在此加入您的訊息處理常式程式碼
	if ( TRUE == bShow )
	{
		if ( m_EditImageView.GetSafeHwnd() != NULL )
		{	m_EditImageView.UpdateWindow(); }
	}
}
//-------------------------------------------------------------------------------------//
void CEditImageDockPane::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
LRESULT CEditImageDockPane::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此加入特定的程式碼和 (或) 呼叫基底類別
	HWND hWnd = NULL;
	switch ( message )
	{
	case MSG_MAIN_FRAME_MESSAGE:
		switch ( wParam )
		{
		case WPARAM_PROJECT_NEW:		
		case WPARAM_PROJECT_OPEN:		
		case WPARAM_PROJECT_CLOSE:
		case WPARAM_PROJECT_SWITCH:			
		case WPARAM_PROJECT_UPDATE:
		case WPARAM_PROJECT_PART_SELECTED:
		case WPARAM_PROJECT_PART_DELETED:
		case WPARAM_PROJECT_CLOSE_ACTIVE_OBJ:
			hWnd = m_EditImageView.GetSafeHwnd();
			if ( NULL != hWnd )
			{	::SendMessage(hWnd, message, wParam, lParam);	}
			break;
		default:			
			//SendMessageToMainFrameWnd(message, wParam, lParam);
			break;
		}
		break;
	case MSG_EDIT_MAIN_VIEW_WND:
		SendMessageToMainFrameWnd(message, wParam, lParam);
		break;
	case MSG_EDIT_PART_LIST_WND:
		SendMessageToMainFrameWnd(message, wParam, lParam);
		break;
	case MSG_EDIT_RESULT_LIST_WND:
		SendMessageToMainFrameWnd(message, wParam, lParam);
		break;
	case MSG_EDIT_WND_PROPERTY_WND:	
		SendMessageToMainFrameWnd(message, wParam, lParam);
		break;
	case MSG_EDIT_IMAGE_VIEW_WND:
	case MSG_EDIT_IMAGE_PROCESS_WND:	
	case MSG_EDIT_VIEW_3D_WND:
	case MSG_EDIT_VIEW_BLOB_WND:
		hWnd = m_EditImageView.GetSafeHwnd();
		if ( NULL != hWnd )
		{	::SendMessage(hWnd, message, wParam, lParam);	}		
		break;		
	}	
	return CDockablePane::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void CEditImageDockPane::SendMessageToMainFrameWnd(UINT message, WPARAM wParam, LPARAM lParam)
{
	AOIDataCollect.SendMainFrameWndMessage(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void CEditImageDockPane::PotMessageToMainFrameWnd(UINT message, WPARAM wParam, LPARAM lParam)
{
	AOIDataCollect.PostMainFrameWndMessage(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//