// EditProjectMapDockPane.cpp : 實作檔
//

#include "stdafx.h"
#include "EditProjectMapDockPane.h"


// CEditProjectMapDockPane
//-------------------------------------------------------------------------------------//
IMPLEMENT_DYNAMIC(CEditProjectMapDockPane, CDockablePane)
//-------------------------------------------------------------------------------------//
CEditProjectMapDockPane::CEditProjectMapDockPane()
{	
}
//-------------------------------------------------------------------------------------//
CEditProjectMapDockPane::~CEditProjectMapDockPane()
{
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CEditProjectMapDockPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_SETFOCUS()
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
// CEditProjectMapDockPane 訊息處理常式
//-------------------------------------------------------------------------------------//
int CEditProjectMapDockPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此加入特別建立的程式碼
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	BOOL bOk = TRUE;	
	/*
	CMenu *pMenu = CWnd::GetMenu();
	if ( NULL != pMenu )
	{
		pMenu->Detach();
		pMenu->LoadMenu(IDR_MENU_PROJECT_MAP);			
	}
	*/	
	bOk = m_Menu.LoadMenu(IDR_MENU_PROJECT_MAP);	
	ASSERT(m_Menu);	
	bOk = SetMenu(NULL);   // Remove and destroy the old menu	
	bOk = SetMenu(&m_Menu);	// Add the new menu
	//this->DrawMenuBar();	

	if ( !m_wndProjectMap.Create(IDD_PROJECT_MAP_PANE, this) )
	//if ( !m_wndProjectMap.Create(IDD_PROJECT_MAP_WND, this) )
	{
		TRACE0("無法建立模組視窗\n");
		return -1;      // 無法建立
	}
	
	//DWORD dwAdd = WS_CHILD|WS_VISIBLE;
	//DWORD dwRemove = WS_OVERLAPPEDWINDOW;//WS_CAPTION|WS_SYSMENU;	
	//dwRemove = 0;
	//m_wndProjectMap.ModifyStyle(dwRemove, dwAdd, 0);
	m_wndProjectMap.SetParent(this);
	
	// 載入影像:	
	OnChangeVisualStyle();	

	return 0;
}
//-------------------------------------------------------------------------------------//
void CEditProjectMapDockPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: 在此加入您的訊息處理常式程式碼
	if ( CWnd::GetSafeHwnd() == NULL ) { return; }
	if ( m_wndProjectMap.GetSafeHwnd() != NULL )
	{
		RECT Rect={0};
		m_wndProjectMap.GetWindowRect(&Rect);
		CWnd::ScreenToClient(&Rect);
		Rect.left = 0;
		Rect.top = 0;
		Rect.right = cx;
		Rect.bottom = cy;
		m_wndProjectMap.MoveWindow(&Rect);
	}
}
//-------------------------------------------------------------------------------------//
void CEditProjectMapDockPane::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDockablePane::OnShowWindow(bShow, nStatus);

	// TODO: 在此加入您的訊息處理常式程式碼
	if ( TRUE == bShow )
	{	
		bool ShowFd = AOIDataCollect.GetShowFdList();
		bool ShowMark = AOIDataCollect.GetShowMarkList();
		bool ShowPanel = AOIDataCollect.GetShowPanelList();
		bool ShowBoard = AOIDataCollect.GetShowBoardList();
		bool ShowBarcode = AOIDataCollect.GetShowBarcodeList();
		bool ShowComponent = AOIDataCollect.GetShowComponentList();		
		m_wndProjectMap.SetShowFiducial(ShowFd);
		m_wndProjectMap.SetShowMark(ShowMark);
		m_wndProjectMap.SetShowBarcode(ShowBarcode);
		m_wndProjectMap.SetShowComponent(ShowComponent);		
		m_wndProjectMap.UpdateProjectMapWndRegion();
		//::PostMessage(m_wndProjectMap.GetSafeHwnd(), MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_UPDATE_FD_ALIGN, (LPARAM)(this));	
	}

}
//-------------------------------------------------------------------------------------//
void CEditProjectMapDockPane::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CEditProjectMapDockPane::OnChangeVisualStyle()
{
}
//-------------------------------------------------------------------------------------//
LRESULT CEditProjectMapDockPane::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此加入特定的程式碼和 (或) 呼叫基底類別
	HWND hWnd = NULL;
	switch ( message )
	{
	case MSG_MAIN_FRAME_MESSAGE:
	case MSG_INSPECTION_CALLBACK:
		hWnd = m_wndProjectMap.GetSafeHwnd();
		if ( NULL != hWnd )
		{
			//if ( m_wndProjectMap.IsWindowVisible() == TRUE )
			{	::SendMessage(hWnd, message, wParam, lParam); }
		}		
		break;	
	}	
	return CDockablePane::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void CEditProjectMapDockPane::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	// TODO: 在此加入您的訊息處理常式程式碼
	m_wndProjectMap.SetFocus();
}
//-------------------------------------------------------------------------------------//
void CEditProjectMapDockPane::ShowPane(BOOL bShow, BOOL bDelay, BOOL bActivate)
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
