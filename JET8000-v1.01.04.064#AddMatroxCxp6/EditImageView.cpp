// EditImageView.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "EditImageView.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CEditImageView
//-------------------------------------------------------------------------------------//
CEditImageView::CEditImageView()
{
}
//-------------------------------------------------------------------------------------//
CEditImageView::~CEditImageView()
{
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CEditImageView, CWnd)
	//{{AFX_MSG_MAP(CEditImageView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CEditImageView message handlers
//-------------------------------------------------------------------------------------//
BOOL CEditImageView::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	return CWnd::Create(NULL, _T(""), dwStyle, rect, pParentWnd, nID);
}
//-------------------------------------------------------------------------------------//
int CEditImageView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	//分裂視窗
	if (!m_wndViewTabs.Create(CMFCTabCtrl::STYLE_FLAT, rectDummy, this, 1))
	{
		TRACE0("無法建立輸出索引標籤視窗\n");
		return -1;      // 無法建立
	}
	m_wndViewTabs.SetLocation(CMFCBaseTabCtrl::LOCATION_BOTTOM);
	m_wndViewTabs.SetResizeMode(CMFCTabCtrl::RESIZE_VERT);

	// 建立檢視:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	const DWORD dwListStype = WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT | LVS_SHOWSELALWAYS;
	const DWORD dwPaneStype = WS_CHILD | WS_VISIBLE | WS_BORDER;//WS_VISIBLE

	//if ( !m_wndImageProcess.Create(dwPaneStype, rectDummy, &m_wndViewTabs, IDD_EDIT_IMAGE_BINARY_WND))
	if ( !m_wndImageProcess.Create(IDD_EDIT_IMAGE_PROCESS_WND, &m_wndViewTabs) )
	{
		TRACE0("無法建立 [影像二值化視窗]\n");
		return -1;      // 無法建立
	}
	
#ifndef DISABLE_3D
	//if ( !m_wndImage3DWnd.Create(dwPaneStype, rectDummy, &m_wndViewTabs, IDD_EDIT_IMAGE_VIEW3D_WND))
	if ( !m_wndImage3DWnd.Create(IDD_EDIT_IMAGE_VIEW3D_WND, &m_wndViewTabs) )
	{
		TRACE0("無法建立 [影像3D視窗]\n");
		return -1;      // 無法建立
	}
#endif//DISABLE_3D

	if ( !m_wndImageBlob.Create(IDD_EDIT_IMAGE_BLOB_WND, &m_wndViewTabs) )
	{
		TRACE0("無法建立 [Blob視窗]\n");
		return -1;      // 無法建立
	}
	

	// 載入影像:	
	OnChangeVisualStyle();
		
	m_wndImageProcess.SetOwner(this);
	m_wndImage3DWnd.SetOwner(this);
	m_wndImageBlob.SetOwner(this);

	// 附加清單視窗到索引標籤:
	CString str;
	CString strTabName;
	BOOL bNameValid = TRUE;	
	if ( m_wndImageProcess.GetSafeHwnd() != NULL )
	{
		str = _T("Image");
		strTabName = LoadMultiLanguageString(str, str);
		m_wndViewTabs.AddTab(&m_wndImageProcess, strTabName, IDD_EDIT_IMAGE_BINARY_WND); 
	}
	if ( m_wndImage3DWnd.GetSafeHwnd() != NULL )
	{
		str = _T("3DView");
		strTabName = LoadMultiLanguageString(str, str);
		m_wndViewTabs.AddTab(&m_wndImage3DWnd, strTabName, IDD_EDIT_IMAGE_VIEW3D_WND);		
	}

	if ( m_wndViewTabs.GetSafeHwnd() != NULL )
	{
		str = _T("Blob");
		strTabName = LoadMultiLanguageString(str, str);
		m_wndViewTabs.AddTab(&m_wndImageBlob, strTabName, IDD_EDIT_IMAGE_BLOB_WND);	
	}
	m_wndViewTabs.SetActiveTab(0);	
	return 0;
}
//-------------------------------------------------------------------------------------//
void CEditImageView::OnDestroy() 
{
	CWnd::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CEditImageView::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if ( GetSafeHwnd() == NULL ) { return; }
	if ( m_wndViewTabs.GetSafeHwnd() != NULL )
	{	m_wndViewTabs.SetWindowPos (NULL, 0, 0, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);	}
}
//-------------------------------------------------------------------------------------//
BOOL CEditImageView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	//return TRUE;
	return CWnd::OnEraseBkgnd(pDC);
}
//-------------------------------------------------------------------------------------//
void CEditImageView::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CWnd::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
LRESULT CEditImageView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	HWND hWnd = NULL;
	CWnd *WndPtr = NULL;
	switch ( message )
	{
	case MSG_MAIN_FRAME_MESSAGE:		
		hWnd = m_wndImageProcess.GetSafeHwnd();
		if ( NULL != hWnd )
		{	::SendMessage(hWnd, message, wParam, lParam);	}		

		hWnd = m_wndImage3DWnd.GetSafeHwnd();
		if ( NULL != hWnd )
		{	::SendMessage(hWnd, message, wParam, lParam);	}

		hWnd = m_wndImageBlob.GetSafeHwnd();
		if ( NULL != hWnd )
		{	::SendMessage(hWnd, message, wParam, lParam);	}
		break;
	case MSG_EDIT_MAIN_VIEW_WND:
		WndPtr = CWnd::GetParent();
		if ( NULL!=WndPtr && WndPtr->GetSafeHwnd()!=NULL )
		{
			hWnd = WndPtr->GetSafeHwnd();
			::SendMessage(hWnd, message, wParam, lParam);
		}
		break;
	case MSG_EDIT_IMAGE_VIEW_WND:
		hWnd = m_wndImageProcess.GetSafeHwnd();
		if ( NULL != hWnd )
		{
			if ( m_wndImageProcess.IsWindowVisible() == TRUE )
			{	::SendMessage(hWnd, message, wParam, lParam); }
			else if ( WPARAM_SHOW_IMAGE_PROCESS_PAGE == wParam )
			{	m_wndViewTabs.SetActiveTab(0);	}			
		}	
		hWnd = m_wndImage3DWnd.GetSafeHwnd();
		if ( NULL != hWnd )
		{	
			if ( m_wndImage3DWnd.IsWindowVisible() == TRUE )
			{	::SendMessage(hWnd, message, wParam, lParam); }
			else if ( WPARAM_SHOW_IMAGE_OPENGL_3D_PAGE == wParam )
			{	m_wndViewTabs.SetActiveTab(1);	}			
		}
		hWnd = m_wndImageBlob.GetSafeHwnd();
		if ( NULL != hWnd )
		{
			if ( m_wndImageBlob.IsWindowVisible() == TRUE )
			{	::SendMessage(hWnd, message, wParam, lParam); }
			else if ( WPARAM_SHOW_IMAGE_BLOB_PAGE == wParam )
			{	m_wndViewTabs.SetActiveTab(2);	}			
		}
		break;
	case MSG_EDIT_IMAGE_PROCESS_WND:	
		hWnd = m_wndImageProcess.GetSafeHwnd();
		if ( NULL != hWnd )
		{
			if ( m_wndImageProcess.IsWindowVisible() == TRUE )
			{	::SendMessage(hWnd, message, wParam, lParam); }
		}		
		break;
	case MSG_EDIT_VIEW_3D_WND:
		hWnd = m_wndImage3DWnd.GetSafeHwnd();
		if ( NULL != hWnd )
		{
			if ( m_wndImage3DWnd.IsWindowVisible() == TRUE )
			{	::SendMessage(hWnd, message, wParam, lParam); }
		}
		break;	
	case MSG_EDIT_VIEW_BLOB_WND:
		hWnd = m_wndImageBlob.GetSafeHwnd();
		if ( NULL != hWnd )
		{
			if ( m_wndImageBlob.IsWindowVisible() == TRUE )
			{	::SendMessage(hWnd, message, wParam, lParam); }
		}
		break;
	}	
	return CWnd::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void CEditImageView::OnChangeVisualStyle()
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
CString CEditImageView::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_EDIT_IMAGE_VIEW");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//