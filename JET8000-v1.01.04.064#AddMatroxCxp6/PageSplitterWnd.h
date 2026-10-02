#if !defined(AFX_PAGESPLITTERWND_H__1F5A0301_6592_42B4_A14B_FF2E8EAC2238__INCLUDED_)
#define AFX_PAGESPLITTERWND_H__1F5A0301_6592_42B4_A14B_FF2E8EAC2238__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageSplitterWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPageSplitterWnd frame with splitter

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
//-------------------------------------------------------------------------------------//
class CPageSplitterWnd : public CBasicSplitterWnd
{
	DECLARE_DYNCREATE(CPageSplitterWnd)
	static int GetIdFromRowCol(int row, int col);
protected:	

// Attributes
protected:
	
public:
	CPageSplitterWnd();           // protected constructor used by dynamic creation
	BOOL AddWindow(int row, int col, CWnd* pWin,CString clsName, DWORD dwStyle,DWORD dwStyleEx, SIZE sizeInit, UINT Id);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPageSplitterWnd)
	protected:	
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPageSplitterWnd();

	// Generated message map functions
	//{{AFX_MSG(CPageSplitterWnd)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGESPLITTERWND_H__1F5A0301_6592_42B4_A14B_FF2E8EAC2238__INCLUDED_)
