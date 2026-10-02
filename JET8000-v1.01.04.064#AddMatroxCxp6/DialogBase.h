#if !defined(AFX_DIALOGBASE_H__110E22A0_DA48_47E6_A5AD_6E911993354C__INCLUDED_)
#define AFX_DIALOGBASE_H__110E22A0_DA48_47E6_A5AD_6E911993354C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogBase.h : header file
//
//-------------------------------------------------------------------------------------//
#ifdef DIALOG_BASE_USE
#define CBaseDialog CDialogBase
#else
#define CBaseDialog CDialog
#endif//DIALOG_BASE_USE
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CDialogBase dialog
//-------------------------------------------------------------------------------------//
class CDialogBase : public CDialog
{
// Construction
public:
	CDialogBase(CWnd* pParent = NULL);   // standard constructor
	CDialogBase(UINT nIDTemplate, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogBase)
	enum { IDD = IDD_DIALOG_BASE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogBase)
	public:
	virtual INT_PTR DoModal();

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	bool                       m_bModalMode;
	bool                       CheckModalMode();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogBase)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGBASE_H__110E22A0_DA48_47E6_A5AD_6E911993354C__INCLUDED_)
