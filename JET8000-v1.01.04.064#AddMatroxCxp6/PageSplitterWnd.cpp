// PageSplitterWnd.cpp : implementation file
//

#include "stdafx.h"
#include "jet8000.h"
#include "PageSplitterWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CPageSplitterWnd
//-------------------------------------------------------------------------------------//
IMPLEMENT_DYNCREATE(CPageSplitterWnd, CBasicSplitterWnd)
//-------------------------------------------------------------------------------------//
int CPageSplitterWnd::GetIdFromRowCol(int row, int col)
{
	ASSERT(row >= 0);	
	ASSERT(col >= 0);	
	return AFX_IDW_PANE_FIRST + row * 16 + col;
}
//-------------------------------------------------------------------------------------//
CPageSplitterWnd::CPageSplitterWnd()
{
}
//-------------------------------------------------------------------------------------//
CPageSplitterWnd::~CPageSplitterWnd()
{
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CPageSplitterWnd, CBasicSplitterWnd)
	//{{AFX_MSG_MAP(CPageSplitterWnd)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CPageSplitterWnd message handlers
BOOL CPageSplitterWnd::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	CWnd *pWnd = CWnd::GetParent();
	*pResult = pWnd->SendMessage(WM_NOTIFY, wParam, lParam);
	return TRUE;
}
//-------------------------------------------------------------------------------------//
BOOL CPageSplitterWnd::AddWindow(int row, int col, CWnd* pWnd,CString clsName, DWORD dwStyle,DWORD dwStyleEx, SIZE sizeInit, UINT Id)
{
#ifdef _DEBUG
	ASSERT_VALID(this);
	ASSERT(row >= 0 && row < m_nRows);
	ASSERT(col >= 0 && col < m_nCols);
	ASSERT(pWnd);

	UINT IDD = IdFromRowCol(row, col);
	//if (GetDlgItem(IdFromRowCol(row, col)) != NULL)
	if (GetDlgItem(Id) != NULL)
	{
		//TRACE(traceAppMsg, 0, "Error: CreateView - pane already exists for row %d, col %d.\n", row, col);
		ASSERT(FALSE);
		return FALSE;
	}
#endif

	// set the initial size for that pane
	m_pColInfo[col].nIdealSize = sizeInit.cx;
	m_pRowInfo[row].nIdealSize = sizeInit.cy;
	ASSERT(pWnd->m_hWnd == NULL);       // not yet created
	// Create with the right size (wrong position)
	CRect rect(CPoint(0,0), sizeInit);
	//if(!pWnd->CreateEx(dwStyleEx,clsName,NULL,dwStyle,rect,this,IdFromRowCol(row, col)))
	if(!pWnd->CreateEx(dwStyleEx,clsName,NULL,dwStyle,rect,this,Id))
	{
		//TRACE(traceAppMsg, 0, "Warning: couldn't create client pane for splitter.\n");
		return FALSE;
	}
	return TRUE;
}
//-------------------------------------------------------------------------------------//