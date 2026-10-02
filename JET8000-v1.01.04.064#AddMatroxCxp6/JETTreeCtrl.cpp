// JETTreeCtrl.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "JETTreeCtrl.h"
//-------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CJETTreeCtrl
//-------------------------------------------------------------------------------------//
CJETTreeCtrl::CJETTreeCtrl()
{
	m_nSBCode = 0;
	m_ScrollBarPosEnd = 0;
	m_ScrollBarPosBegin = 0;
}
//-------------------------------------------------------------------------------------//
CJETTreeCtrl::~CJETTreeCtrl()
{
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CJETTreeCtrl, CBasicTreeCtrl)
	//{{AFX_MSG_MAP(CJETTreeCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CJETTreeCtrl message handlers
//-------------------------------------------------------------------------------------//
void CJETTreeCtrl::SetItemTextColor(HTREEITEM hItem, COLORREF clrText)
{
	std::map<HTREEITEM, TVCOLOR>::iterator iter;
	iter = m_ItemTextColorMap.find(hItem);
	if ( iter != m_ItemTextColorMap.end() )
	{	
		iter->second.clrText   = clrText;	
		iter->second.clrTextBk = CLR_DEFAULT;	
	}
	else
	{	m_ItemTextColorMap[hItem]=TVCOLOR(clrText, CLR_DEFAULT); }
}
//-------------------------------------------------------------------------------//
void CJETTreeCtrl::SetItemTextBKColor(HTREEITEM hItem, COLORREF clrTextBk)
{
	std::map<HTREEITEM, TVCOLOR>::iterator iter;
	iter = m_ItemTextColorMap.find(hItem);
	if ( iter != m_ItemTextColorMap.end() )
	{	
		iter->second.clrText   = CLR_DEFAULT;	
		iter->second.clrTextBk = clrTextBk;	
	}
	else
	{	m_ItemTextColorMap[hItem]=TVCOLOR(CLR_DEFAULT, clrTextBk); }
}
//-------------------------------------------------------------------------------//
BOOL CJETTreeCtrl::DeleteItem(HTREEITEM hItem)
{
	if ( CBasicTreeCtrl::DeleteItem(hItem) == FALSE ) { return FALSE; }
	m_ItemTextColorMap.erase(hItem);
	return TRUE;
}
//-------------------------------------------------------------------------------//
BOOL CJETTreeCtrl::DeleteAllItems()
{
	if ( CBasicTreeCtrl::DeleteAllItems() == FALSE ) { return FALSE; }
	m_ItemTextColorMap.clear();
	return TRUE;
}
//-------------------------------------------------------------------------------//
BOOL CJETTreeCtrl::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class	
	int val=0;
	CWnd *pOwner=CWnd::GetOwner();
	switch ( pMsg->message )
	{	
	case WM_KEYDOWN: 
		switch ( pMsg->wParam )
		{
		case VK_UP:		val = -1;	break;
		case VK_DOWN:	val =  1;	break;
		case VK_PRIOR:	val = -1;	break;
		case VK_NEXT:	val =  1;	break;
		case VK_END:	val =  1;	break;
		case VK_HOME:	val = -1;	break;
		default:			
			break;
		}
		if ( 0 != val )
		{	pOwner->PostMessage(MSG_TREE_CTRL, WPARAM_TREE_VERTICAL_SCROLL_END, val);	}
		break;
	case WM_LBUTTONDOWN:	
	{		
		POINT point;
		UINT uFlag=0;
		point.x = GET_X_LPARAM(pMsg->lParam);
		point.y = GET_Y_LPARAM(pMsg->lParam);		
		HTREEITEM hItem = CBasicTreeCtrl::HitTest(point, &uFlag);
		if ((hItem != NULL) && (TVHT_ONITEM & uFlag))
		{	
			RECT ItemRect;
			RECT CleintRect;
			bool bScrollEnd=true;			
			HTREEITEM hSiblingItem = hItem;
			CString ItemText=GetItemText(hItem);

			GetClientRect(&CleintRect);			
			while ( true )
			{
				hSiblingItem = GetNextSiblingItem(hSiblingItem);
				if ( NULL == hSiblingItem )
				{	break;	}
				GetItemRect(hSiblingItem, &ItemRect, FALSE);
				if ( ItemRect.top > CleintRect.bottom )
				{	
					bScrollEnd = false;	
					break;
				}
			};
			if ( true == bScrollEnd )
			{	pOwner->PostMessage(MSG_TREE_CTRL, WPARAM_TREE_VERTICAL_SCROLL_END, 1);	}			
			uFlag = uFlag;
		}		
	}
		break;
	}	
	return CBasicTreeCtrl::PreTranslateMessage(pMsg);	
}
//-------------------------------------------------------------------------------------//
void CJETTreeCtrl::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;	
	LPNMTVCUSTOMDRAW lptvcd = (LPNMTVCUSTOMDRAW)pNMHDR;
//	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

//	CString str;
//	str.Format("%d\n", lptvcd->nmcd.dwDrawStage);
//	TRACE0(str);
	const int key = lptvcd->nmcd.dwDrawStage;
	const int cdds_prepaint = CDDS_PREPAINT;
	const int cdds_itemprepaint = CDDS_ITEMPREPAINT;

	switch(lptvcd->nmcd.dwDrawStage)
	{	
	case CDDS_PREPAINT:
	{		
		*pResult = CDRF_NOTIFYITEMDRAW;
		return;
	}
    // Modify item text and or background
    case CDDS_ITEMPREPAINT:
    {
		HTREEITEM hItem = (HTREEITEM)(lptvcd->nmcd.dwItemSpec);		

		std::map<HTREEITEM, TVCOLOR>::iterator iter;

		iter = m_ItemTextColorMap.find(hItem);
		if ( iter != m_ItemTextColorMap.end() )
		{
			if ( CLR_DEFAULT != iter->second.clrText )
			{	lptvcd->clrText = iter->second.clrText;	}
		}
		
		if( lptvcd->nmcd.uItemState != (CDIS_FOCUS|CDIS_SELECTED) )
		{			
			if ( iter != m_ItemTextColorMap.end() )
			{
				if ( CLR_DEFAULT != iter->second.clrTextBk )
				{	lptvcd->clrTextBk = iter->second.clrTextBk;		}
			}
		}

		// If you want the sub items the same as the item,
		// set *pResult to CDRF_NEWFONT		
		//*pResult = CDRF_NOTIFYSUBITEMDRAW;
		*pResult = CDRF_DODEFAULT;
		return;
    }
	}
   
}
//-------------------------------------------------------------------------------//
void CJETTreeCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{	
	// TODO: Add your specialized code here and/or call the base class
	int  val=0;
	CWnd *pOwner=CWnd::GetOwner();	
	if ( NULL != pOwner )
	{
		switch ( nSBCode )
		{
		case SB_THUMBTRACK:
		case SB_THUMBPOSITION:
			if ( 0 == m_ScrollBarPosBegin )
			{	m_ScrollBarPosBegin = nPos;	}
			m_ScrollBarPosEnd = nPos;			
			break;
		case SB_ENDSCROLL:
			switch ( m_nSBCode )
			{
			case SB_LINEUP:		val = -1;	break;			
			case SB_LINEDOWN:	val =  1;	break;			
			case SB_PAGEUP:		val = -1;	break;			
			case SB_PAGEDOWN:	val =  1;	break;			
			case SB_THUMBTRACK:
			case SB_THUMBPOSITION:
				val = (int)(m_ScrollBarPosEnd-m_ScrollBarPosBegin);
				break;
			}
			pOwner->PostMessage(MSG_TREE_CTRL, WPARAM_TREE_VERTICAL_SCROLL_END, val);
			m_ScrollBarPosEnd = nPos;
			m_ScrollBarPosBegin = nPos;
			break;
		}		
	}
	m_nSBCode = nSBCode;
	CBasicTreeCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}
//-------------------------------------------------------------------------------//