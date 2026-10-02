#if !defined(AFX_EDITRESULTPANEBAR_H__8F633BAB_3C02_40E5_8677_F77A3FDDC9C4__INCLUDED_)
#define AFX_EDITRESULTPANEBAR_H__8F633BAB_3C02_40E5_8677_F77A3FDDC9C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditResultPaneBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditResultPaneBar dialog

class CEditResultPaneBar : public CDialogBar
{
// Construction
public:
	CEditResultPaneBar(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEditResultPaneBar)
	enum { IDD = IDD_EDIT_RESULT_PANE_BAR };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditResultPaneBar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHandler);
	//}}AFX_VIRTUAL

public:
	//---------------------------------------------------------------------------------//
	bool                       GetShowPassChk();
	//---------------------------------------------------------------------------------//
	bool                       SetResultDateTime(LPCTSTR filename);
	bool                       SetResultListText(LPCTSTR filename);
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//	
	void                       SwitchMultiLanguage();
	//---------------------------------------------------------------------------------//
	
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEditResultPaneBar)
	virtual LRESULT OnInitDialog(WPARAM, LPARAM);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITRESULTPANEBAR_H__8F633BAB_3C02_40E5_8677_F77A3FDDC9C4__INCLUDED_)
