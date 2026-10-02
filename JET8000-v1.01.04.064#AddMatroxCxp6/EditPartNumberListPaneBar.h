#if !defined(AFX_EDITPARTNUMBERLISTPANEBAR_H__5E47B3F4_300F_402D_BE77_71DC7D1D97F8__INCLUDED_)
#define AFX_EDITPARTNUMBERLISTPANEBAR_H__5E47B3F4_300F_402D_BE77_71DC7D1D97F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditPartNumberListPaneBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditPartNumberListPaneBar dialog

class CEditPartNumberListPaneBar : public CDialogBar
{
// Construction
public:
	CEditPartNumberListPaneBar(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEditPartNumberListPaneBar)
	enum { IDD = IDD_EDIT_PART_NUMBER_LIST_PANE_BAR };
	CComboBox	m_SwitchModeCombox;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditPartNumberListPaneBar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHandler);
	//}}AFX_VIRTUAL
	
public:
	//---------------------------------------------------------------------------------//
	void                       SetPartNumberNameEdit(LPCTSTR str);
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//	
	bool                       BuildMoveTypeCombox();
	void                       SwitchMultiLanguage();
	//---------------------------------------------------------------------------------//
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEditPartNumberListPaneBar)
	virtual LRESULT OnInitDialog(WPARAM, LPARAM);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITPARTNUMBERLISTPANEBAR_H__5E47B3F4_300F_402D_BE77_71DC7D1D97F8__INCLUDED_)
