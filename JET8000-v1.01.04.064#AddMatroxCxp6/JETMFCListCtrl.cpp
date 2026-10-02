// JETListCtrl.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "JETMFCListCtrl.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CJETMFCListCtrl
//-------------------------------------------------------------------------------------//
CJETMFCListCtrl::CJETMFCListCtrl()
{
}
//-------------------------------------------------------------------------------------//
CJETMFCListCtrl::~CJETMFCListCtrl()
{
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CJETMFCListCtrl, CMFCBasicListCtrl)
	//{{AFX_MSG_MAP(CJETMFCListCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CJETMFCListCtrl message handlers
//-------------------------------------------------------------------------------------//
int CJETMFCListCtrl::GetColumnCount()
{
	int ColumnCount = 0;	
	CMFCHeaderCtrl &HeadCtrl = CMFCListCtrl::GetHeaderCtrl();
	ColumnCount = HeadCtrl.GetItemCount();
	return ColumnCount;
}
//-------------------------------------------------------------------------------------//
inline unsigned int CJETMFCListCtrl::CalcItemColorKey(int nItem, int nSubItem)
{
	if ( -1 == nSubItem )
	{	return (nItem*JET_LIST_MAX_COLUMNS);	}
	return (nItem*JET_LIST_MAX_COLUMNS)+nSubItem+1;
}
//-------------------------------------------------------------------------------------//
inline void CJETMFCListCtrl::DeleteItemColor(int nItem)
{
	unsigned int key = CalcItemColorKey(nItem);
	m_CellTextColorMap.erase(key);	
}
//-------------------------------------------------------------------------------------//
inline void CJETMFCListCtrl::InsertItemColor(int nItem, COLORREF clrText, COLORREF clrTextBk)
{
	return ;
	int       i=0;	
	const int NColumns = GetColumnCount();	
	for ( i=0; i<NColumns; i++ )
	{	InsertItemColor(nItem, i, clrText, clrTextBk);	}
	return;
}
//-------------------------------------------------------------------------------------//
void CJETMFCListCtrl::InsertItemColor(int nItem, int nSubItem, COLORREF clrText, COLORREF clrTextBk)
{
	unsigned int key = CalcItemColorKey(nItem, nSubItem);
	std::map<unsigned int, LVCOLOR>::iterator iter;
	iter = m_CellTextColorMap.find(key);
	if ( iter != m_CellTextColorMap.end() )
	{	
		iter->second.clrText   = clrText;	
		iter->second.clrTextBk = clrTextBk;	
	}
	else
	{	m_CellTextColorMap[key]=LVCOLOR(clrText, clrTextBk); }
}
//-------------------------------------------------------------------------------------//
BOOL CJETMFCListCtrl::SetItemTextColor(int nItem, COLORREF clrText)
{	
	int       i=0;	
	const int NColumns = GetColumnCount();		
	for ( i=0; i<NColumns; i++ )
	{	SetItemTextColor(nItem, i, clrText);	}
	return TRUE;
}
//-------------------------------------------------------------------------------------//
BOOL CJETMFCListCtrl::SetItemTextColor(int nItem, int nSubItem, COLORREF clrText)
{	
	unsigned int key = CalcItemColorKey(nItem, nSubItem);
	std::map<unsigned int, LVCOLOR>::iterator iter;
	iter = m_CellTextColorMap.find(key);
	if ( iter != m_CellTextColorMap.end() )
	{	iter->second.clrText = clrText;	}
	else
	{	m_CellTextColorMap[key]=LVCOLOR(clrText, CLR_DEFAULT); }	
	return TRUE;
}
//-------------------------------------------------------------------------------------//
BOOL CJETMFCListCtrl::SetItemTextBkColor(int nItem, COLORREF clrTextBk)
{
	int       i=0;	
	const int NColumns = GetColumnCount();		
	for ( i=0; i<NColumns; i++ )
	{	SetItemTextBkColor(nItem, i, clrTextBk);	}
	return TRUE;
}
//-------------------------------------------------------------------------------------//
BOOL CJETMFCListCtrl::SetItemTextBkColor(int nItem, int nSubItem, COLORREF clrTextBk)
{	
	unsigned int key = CalcItemColorKey(nItem, nSubItem);	
	std::map<unsigned int, LVCOLOR>::iterator iter;	
	iter = m_CellTextColorMap.find(key);
	if ( iter != m_CellTextColorMap.end() )
	{	iter->second.clrTextBk = clrTextBk;	}
	else
	{	m_CellTextColorMap[key]=LVCOLOR(CLR_DEFAULT, clrTextBk); }	
	return TRUE;
}
//-------------------------------------------------------------------------------------//
int CJETMFCListCtrl::InsertItem(UINT nMask, int nItem, LPCTSTR lpszItem, UINT nState, UINT nStateMask, int nImage, LPARAM lParam)
{
	int Res = CMFCBasicListCtrl::InsertItem(nMask, nItem, lpszItem, nState, nStateMask, nImage, lParam);
	if ( Res < 0 ) { return -1; }
	CJETMFCListCtrl::InsertItemColor(nItem, CLR_DEFAULT, CLR_DEFAULT);
	return Res;
}
//-------------------------------------------------------------------------------------//
int CJETMFCListCtrl::InsertItem(const LVITEM* pItem)
{		
	int Res = CMFCBasicListCtrl::InsertItem(pItem);
	if ( Res < 0 ) { return -1; }
	CJETMFCListCtrl::InsertItemColor(-1, CLR_DEFAULT, CLR_DEFAULT);
	return Res;
}
//-------------------------------------------------------------------------------------//
int CJETMFCListCtrl::InsertItem(int nItem, LPCTSTR lpszItem)
{
	int Res = CMFCBasicListCtrl::InsertItem(nItem, lpszItem);
	if ( Res < 0 ) { return -1; }
	CJETMFCListCtrl::InsertItemColor(nItem, CLR_DEFAULT, CLR_DEFAULT);
	return Res;
}
//-------------------------------------------------------------------------------------//
int CJETMFCListCtrl::InsertItem(int nItem, LPCTSTR lpszItem, int nImage)
{	
	int Res = CMFCBasicListCtrl::InsertItem(nItem, lpszItem, nImage);
	if ( Res < 0 ) { return -1; }
	CJETMFCListCtrl::InsertItemColor(nItem, CLR_DEFAULT, CLR_DEFAULT);
	return Res;
}
//-------------------------------------------------------------------------------------//
BOOL CJETMFCListCtrl::DeleteItem(int nItem)
{	
	if ( FALSE == CMFCBasicListCtrl::DeleteItem(nItem) ) { return FALSE; }
	CJETMFCListCtrl::DeleteItemColor(nItem);
	return TRUE;
}
//-------------------------------------------------------------------------------------//
BOOL CJETMFCListCtrl::DeleteAllItems()
{	
	if ( FALSE == CMFCBasicListCtrl::DeleteAllItems() ) { return FALSE; }
	m_CellTextColorMap.clear();
	return TRUE;
}
//-------------------------------------------------------------------------------------//
BOOL CJETMFCListCtrl::PreTranslateMessage(MSG* pMsg) 
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
		{	pOwner->PostMessage(MSG_LIST_CTRL, WPARAM_LIST_VERTICAL_SCROLL_END, val);	}
		break;
	}	
	return CMFCBasicListCtrl::PreTranslateMessage(pMsg);	
}
//-------------------------------------------------------------------------------------//
void CJETMFCListCtrl::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{	
	LPNMLVCUSTOMDRAW   lplvcd = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);	
	bool  bNewFont=false;
	const int iItem    = (int)(lplvcd->nmcd.dwItemSpec);
	const int iSubItem = lplvcd->iSubItem;
	switch(lplvcd->nmcd.dwDrawStage)
	{
    case CDDS_ITEMPREPAINT:		
    case CDDS_ITEMPREPAINT | CDDS_SUBITEM:
		{
			const int key = CalcItemColorKey(iItem, iSubItem);
			std::map<unsigned int, LVCOLOR>::iterator iter;
			iter = m_CellTextColorMap.find(key);
			if ( iter != m_CellTextColorMap.end() )
			{
				if ( CLR_DEFAULT != iter->second.clrText )
				{
					bNewFont = true;
					lplvcd->clrText = iter->second.clrText; 
				}
				if ( CLR_DEFAULT != iter->second.clrTextBk )
				{
					bNewFont = true;
					lplvcd->clrTextBk = iter->second.clrTextBk; 
				}
			}
			if ( CDDS_ITEMPREPAINT == lplvcd->nmcd.dwDrawStage )
			{
				//CDRF_NOTIFYSUBITEMDRAW要回傳才能往後收到SubItem
				*pResult = CDRF_NOTIFYSUBITEMDRAW;
			}
			else
			{
				if ( true == bNewFont )
				{	*pResult = CDRF_NEWFONT; }
				else
				{	*pResult = CDRF_DODEFAULT; }
			}
			return;
		}				
		break;
    default: 
		break;    
	}

	*pResult = 0;
	*pResult |= CDRF_NOTIFYPOSTPAINT;
	*pResult |= CDRF_NOTIFYITEMDRAW;
	*pResult |= CDRF_NOTIFYSUBITEMDRAW;
}
//-------------------------------------------------------------------------------------//