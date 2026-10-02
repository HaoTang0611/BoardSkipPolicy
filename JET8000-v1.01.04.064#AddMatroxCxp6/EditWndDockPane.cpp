// EditWndDockPane.cpp : 實作檔
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "JET8000.h"
#include "EditWndDockPane.h"
//-------------------------------------------------------------------------------------//
const UINT ID_EDIT_WND_VIEW     = 101;
//-------------------------------------------------------------------------------------//
// CEditWndDockPane
//-------------------------------------------------------------------------------------//
IMPLEMENT_DYNAMIC(CEditWndDockPane, CDockablePane)
//-------------------------------------------------------------------------------------//
CEditWndDockPane::CEditWndDockPane()
{
	
}
//-------------------------------------------------------------------------------------//
CEditWndDockPane::~CEditWndDockPane()
{
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CEditWndDockPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_CONTEXTMENU()	
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
// CEditWndDockPane 訊息處理常式
//-------------------------------------------------------------------------------------//
int CEditWndDockPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此加入特別建立的程式碼
	CRect rectDummy;
	rectDummy.SetRectEmpty();
	const DWORD dwPaneStype = WS_CHILD | WS_VISIBLE | WS_BORDER;
	if ( !m_EditWndView.Create(dwPaneStype, rectDummy, this, ID_EDIT_WND_VIEW))
	{
		TRACE0("無法建立 [編輯框視窗]\n");
		return -1;      // 無法建立
	}	
	return 0;
}
//-------------------------------------------------------------------------------------//
void CEditWndDockPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: 在此加入您的訊息處理常式程式碼
	if ( GetSafeHwnd() == NULL ) { return; }
	if ( m_EditWndView.GetSafeHwnd() != NULL )
	{	m_EditWndView.SetWindowPos (NULL, 0, 0, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);	}
}
//-------------------------------------------------------------------------------------//
void CEditWndDockPane::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	// TODO: 在此加入您的訊息處理常式程式碼
	//m_wndSplitter.SetFocus();
}
//-------------------------------------------------------------------------------------//
void CEditWndDockPane::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CEditWndDockPane::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDockablePane::OnShowWindow(bShow, nStatus);
	if ( TRUE == bShow ) 
	{		
		AOIDataCollect.SetShowUIWndWndList(true); 
		AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_WND_PROPERTY_WND, WPARAM_BUILD_WND_SELECTED, NULL);		
	}
	else
	{	AOIDataCollect.SetShowUIWndWndList(false);	}
}
//-------------------------------------------------------------------------------------//
void CEditWndDockPane::SetVSDotNetLook(BOOL bSet)
{
	m_EditWndView.SetVSDotNetLook(bSet);	
}
//-------------------------------------------------------------------------------------//
LRESULT CEditWndDockPane::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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
			hWnd = m_EditWndView.GetSafeHwnd();
			if ( NULL != hWnd )
			{	::SendMessage(hWnd, message, wParam, lParam);	}
			break;
		default:
			wParam = wParam;
			//SendMessageToMainFrameWnd(message, wParam, lParam);
			break;
		}
		break;
	case MSG_EDIT_MAIN_VIEW_WND:
		SendMessageToMainFrameWnd(message, wParam, lParam);
		//PostMessageToMainFrameWnd(message, wParam, lParam);
		break;
	case MSG_EDIT_PART_LIST_WND:
		SendMessageToMainFrameWnd(message, wParam, lParam);
		//PostMessageToMainFrameWnd(message, wParam, lParam);
		break;
	case MSG_EDIT_RESULT_LIST_WND:
		SendMessageToMainFrameWnd(message, wParam, lParam);
		//PostMessageToMainFrameWnd(message, wParam, lParam);
		break;
	case MSG_EDIT_IMAGE_VIEW_WND:		
	case MSG_EDIT_IMAGE_PROCESS_WND:		
	case MSG_EDIT_VIEW_3D_WND:		
	case MSG_EDIT_VIEW_BLOB_WND:
		SendMessageToMainFrameWnd(message, wParam, lParam);
		break;
	case MSG_EDIT_WND_PROPERTY_WND:
		hWnd = m_EditWndView.GetSafeHwnd();
		if ( NULL != hWnd )
		{	::SendMessage(hWnd, message, wParam, lParam);	}
		break;
	}	
	return CDockablePane::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void CEditWndDockPane::SendMessageToMainFrameWnd(UINT message, WPARAM wParam, LPARAM lParam)
{
	AOIDataCollect.SendMainFrameWndMessage(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void CEditWndDockPane::PostMessageToMainFrameWnd(UINT message, WPARAM wParam, LPARAM lParam)
{
	AOIDataCollect.PostMainFrameWndMessage(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
