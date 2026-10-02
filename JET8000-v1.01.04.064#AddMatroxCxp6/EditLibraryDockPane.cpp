// EditLibraryDockPane.cpp : 實作檔
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "EditLibraryDockPane.h"
//-------------------------------------------------------------------------------------//
// CEditLibraryDockPane
//-------------------------------------------------------------------------------------//
IMPLEMENT_DYNAMIC(CEditLibraryDockPane, CDockablePane)
//-------------------------------------------------------------------------------------//
CEditLibraryDockPane::CEditLibraryDockPane()
{	
}
//-------------------------------------------------------------------------------------//
CEditLibraryDockPane::~CEditLibraryDockPane()
{
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CEditLibraryDockPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()	
	ON_WM_SHOWWINDOW()
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
// CEditLibraryDockPane 訊息處理常式
//-------------------------------------------------------------------------------------//
int CEditLibraryDockPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此加入特別建立的程式碼
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	if ( !m_wndLibrary.Create(IDD_EDIT_LIBRARY_WND, this) )
	{
		TRACE0("無法建立資料庫視窗\n");
		return -1;      // 無法建立
	}
	m_wndLibrary.SetParent(this);
	
	// 載入影像:	
	OnChangeVisualStyle();	
	
	return 0;
}
//-------------------------------------------------------------------------------------//
void CEditLibraryDockPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: 在此加入您的訊息處理常式程式碼
	if (CWnd::GetSafeHwnd() == NULL ) { return ; }
	
	if ( m_wndLibrary.GetSafeHwnd() != NULL )
	{
		RECT Rect={0};
		m_wndLibrary.GetWindowRect(&Rect);
		CWnd::ScreenToClient(&Rect);
		Rect.left = 0;
		Rect.top = 0;
		Rect.right = cx;
		Rect.bottom = cy;
		m_wndLibrary.MoveWindow(&Rect);
	}
}
//-------------------------------------------------------------------------------------//
void CEditLibraryDockPane::OnChangeVisualStyle()
{
}
//-------------------------------------------------------------------------------------//
LRESULT CEditLibraryDockPane::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此加入特定的程式碼和 (或) 呼叫基底類別
	HWND hWnd = NULL;
	switch ( message )
	{
	case MSG_MAIN_FRAME_MESSAGE:
		hWnd = m_wndLibrary.GetSafeHwnd();
		if ( NULL != hWnd )
		{
			if ( AOIDataCollect.CheckMustUpdateMsg(message, wParam, lParam) == true ) 
			{	::SendMessage(hWnd, message, wParam, lParam);	}
			else
			{
				if ( m_wndLibrary.IsWindowVisible() == TRUE )
				{	::SendMessage(hWnd, message, wParam, lParam);	}
			}
		}		
		break;
	case MSG_EDIT_LIBRARY_WND:
		hWnd = m_wndLibrary.GetSafeHwnd();
		if ( NULL != hWnd )
		{	::SendMessage(hWnd, message, wParam, lParam);	}		
		break;
	}	
	return CDockablePane::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void CEditLibraryDockPane::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDockablePane::OnShowWindow(bShow, nStatus);

	// TODO: 在此加入您的訊息處理常式程式碼
	if ( bShow == TRUE )
	{	m_wndLibrary.ExecShowLibraryWnd();	}
	else
	{	m_wndLibrary.ExecHideLibraryWnd();	}
}
//-------------------------------------------------------------------------------------//
void CEditLibraryDockPane::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CEditLibraryDockPane::ShowPane(BOOL bShow, BOOL bDelay, BOOL bActivate)
{	
	CDockablePane::ShowPane(bShow, bDelay, bActivate);
	//return;//20190307

	if ( IsAutoHideMode() )	//確認是否為自動隱藏
	{
		if ( FALSE == bShow )//是否為不顯示
		{	
			//m_bActive = FALSE;
			if ( CWnd::IsWindowVisible() == TRUE )
			{	
				Slide(FALSE);//執行收回動作
			}
		}
	}	
}
//-------------------------------------------------------------------------------------//