#if !defined(AFX_EDITMODELLISTPANEBAR_H__EAD9EE98_2BC7_4E43_8065_06B662CF9F9A__INCLUDED_)
#define AFX_EDITMODELLISTPANEBAR_H__EAD9EE98_2BC7_4E43_8065_06B662CF9F9A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditModelListPaneBar.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CEditModelListPaneBar dialog
//-------------------------------------------------------------------------------------//
class CEditModelListPaneBar : public CDialogBar
{
// Construction
public:
	CEditModelListPaneBar(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEditModelListPaneBar)
	enum { IDD = IDD_EDIT_MODEL_LIST_PANE_BAR };
	CComboBox	m_SwitchModeCombox;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditModelListPaneBar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHandler);
	//}}AFX_VIRTUAL

public:
	//---------------------------------------------------------------------------------//
	void                       SetModelNameEdit(LPCTSTR str);
	void                       SetGroupNameEdit(LPCTSTR str);
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//	
	bool                       BuildMoveTypeCombox();
	void                       SwitchMultiLanguage();	
	//---------------------------------------------------------------------------------//
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEditModelListPaneBar)	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual LRESULT OnInitDialog(WPARAM, LPARAM);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITMODELLISTPANEBAR_H__EAD9EE98_2BC7_4E43_8065_06B662CF9F9A__INCLUDED_)
