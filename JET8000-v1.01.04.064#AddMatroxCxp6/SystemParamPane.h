#if !defined(AFX_SYSTEMPARAMPANE_H__2A82F52D_C8B2_4456_AFDC_2CBA15FBFEF7__INCLUDED_)
#define AFX_SYSTEMPARAMPANE_H__2A82F52D_C8B2_4456_AFDC_2CBA15FBFEF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SystemParamPane.h : header file
//
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CSystemParamPane dialog
//-------------------------------------------------------------------------------------//
class CSystemParamPane : public CDialog
{
// Construction
public:
	CSystemParamPane(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSystemParamPane)
	enum { IDD = IDD_SYSTEM_PARAM_PANE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSystemParamPane)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	//---------------------------------------------------------------------------------//	
	void                       SetSystemParameterPtr(TSystemParameter *Ptr);
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//	
	TSystemParameter          *m_SysParameterPtr;	
	//---------------------------------------------------------------------------------//	
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//	

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSystemParamPane)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSTEMPARAMPANE_H__2A82F52D_C8B2_4456_AFDC_2CBA15FBFEF7__INCLUDED_)
