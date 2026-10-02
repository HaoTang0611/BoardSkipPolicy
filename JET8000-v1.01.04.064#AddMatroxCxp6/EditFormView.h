// EditFormView.h : interface of the CEditFormView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_EDITFORMVIEW_H__875828F7_5430_43BD_B610_908214688A5B__INCLUDED_)
#define AFX_EDITFORMVIEW_H__875828F7_5430_43BD_B610_908214688A5B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include "MainDoc.h"
//-------------------------------------------------------------------------------------//
class CEditFormView : public CFormView
{
protected: // create from serialization only
	CEditFormView();
	DECLARE_DYNCREATE(CEditFormView)

public:
	//{{AFX_DATA(CEditFormView)
	enum{ IDD = IDD_EDIT_FORMVIEW };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:
	CMainDoc* GetDocument();

// Operations
public:

protected:	


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditFormView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL	
// Implementation
public:
	virtual ~CEditFormView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CEditFormView)
	afx_msg void OnDestroy();	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:	
};
//-------------------------------------------------------------------------------------//
#ifndef _DEBUG  // debug version in EditFormView.cpp
inline CMainDoc* CEditFormView::GetDocument()
   { return (CMainDoc*)m_pDocument; }
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITFORMVIEW_H__875828F7_5430_43BD_B610_908214688A5B__INCLUDED_)
