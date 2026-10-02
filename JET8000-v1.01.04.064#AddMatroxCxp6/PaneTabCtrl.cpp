// PaneTabCtrl.cpp : 實作檔
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "PaneTabCtrl.h"
//-------------------------------------------------------------------------------------//
// CPaneTabCtrl
//-------------------------------------------------------------------------------------//
IMPLEMENT_DYNAMIC(CPaneTabCtrl, CMFCTabCtrl)
//-------------------------------------------------------------------------------------//
CPaneTabCtrl::CPaneTabCtrl()
{

}
//-------------------------------------------------------------------------------------//
CPaneTabCtrl::~CPaneTabCtrl()
{
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CPaneTabCtrl, CMFCTabCtrl)
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
// CPaneTabCtrl 訊息處理常式
//-------------------------------------------------------------------------------------//
BOOL CPaneTabCtrl::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: 在此加入特定的程式碼和 (或) 呼叫基底類別
	if ( GetSafeHwnd() != NULL )
	{
		CWnd *pWnd = CWnd::GetParent();
		if ( pWnd!=NULL && NULL!=pWnd->GetSafeHwnd() )
		{	
			*pResult = pWnd->SendMessage(WM_NOTIFY, wParam, lParam);	
			return TRUE;
		}	
		pWnd = CWnd::GetOwner();
		if ( pWnd!=NULL && NULL!=pWnd->GetSafeHwnd() )
		{
			*pResult = pWnd->SendMessage(WM_NOTIFY, wParam, lParam);	
			return TRUE;
		}
	}
	return CMFCTabCtrl::OnNotify(wParam, lParam, pResult);
}
//-------------------------------------------------------------------------------------//