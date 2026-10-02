#if !defined(AFX_INPUTBOXWND_H__B84C3316_1E0A_4A9C_B128_E1FBBD131BF5__INCLUDED_)
#define AFX_INPUTBOXWND_H__B84C3316_1E0A_4A9C_B128_E1FBBD131BF5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputBoxWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CInputBoxWnd dialog
//-------------------------------------------------------------------------------------//
class CInputBoxWnd : public CBaseDialog
{
// Construction
public:
	CInputBoxWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInputBoxWnd)
	enum { IDD = IDD_INPUT_BOX_WND };
	CString	m_TitleLabel1;
	CString	m_TitleLabel2;
	CString	m_DataEdit1;
	CString	m_DataEdit2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputBoxWnd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:
	//---------------------------------------------------------------------------------//	
	void                       SetParam1(LPCTSTR WndTxt, LPCTSTR Label, LPCTSTR Default);
	void                       SetParam2(LPCTSTR WndTxt, LPCTSTR Label1, LPCTSTR Default1, LPCTSTR Label2, LPCTSTR Default2);
	void                       SetWndPos(const POINT &Pos);
	void                       SetReadOnly(bool bReadOnly, bool bReadOnly2=true);	
	void                       SetPasswordMode(bool bPassword, bool bPassword2=true);
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//		
	POINT                      m_WndPos;
	CString                    m_WndText;
	int                        m_DataCount;
	bool                       m_WndMovePos;	
	bool                       m_ReadOnly;
	bool                       m_ReadOnly2;
	bool                       m_PasswordMode;
	bool                       m_PasswordMode2;
	//---------------------------------------------------------------------------------//	
	void                       SwitchMultiLanguage();
	//---------------------------------------------------------------------------------//	
	bool                       SetEditReadOnly(UINT CtrlID);
	bool                       SetEditPasswordMode(UINT CtrlID);
	//---------------------------------------------------------------------------------//
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputBoxWnd)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTBOXWND_H__B84C3316_1E0A_4A9C_B128_E1FBBD131BF5__INCLUDED_)
