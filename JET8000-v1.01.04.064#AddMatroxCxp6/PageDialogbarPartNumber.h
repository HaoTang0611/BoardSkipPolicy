#if !defined(AFX_PAGEDIALOGBARPARTNUMBER_H__1FC2457E_6DCF_4031_8411_A722EE243D65__INCLUDED_)
#define AFX_PAGEDIALOGBARPARTNUMBER_H__1FC2457E_6DCF_4031_8411_A722EE243D65__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageDialogbarPartNumber.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CPageDialogbarPartNumber dialog

class CPageDialogbarPartNumber : public CDialogBar
{
// Construction
public:
	CPageDialogbarPartNumber(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPageDialogbarPartNumber)
	enum { IDD = IDD_PAGE_DIALOGBAR_PART_NUMBER };
	CComboBox	m_TypeCombox;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPageDialogbarPartNumber)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHandler);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPageDialogbarPartNumber)
	virtual LRESULT OnInitDialog(WPARAM, LPARAM);		
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEDIALOGBARPARTNUMBER_H__1FC2457E_6DCF_4031_8411_A722EE243D65__INCLUDED_)
