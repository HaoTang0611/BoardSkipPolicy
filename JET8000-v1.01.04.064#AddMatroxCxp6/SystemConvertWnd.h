#if !defined(AFX_SYSTEMCONVERTWND_H__1AB5690A_17FC_4E09_A2F6_89276523A819__INCLUDED_)
#define AFX_SYSTEMCONVERTWND_H__1AB5690A_17FC_4E09_A2F6_89276523A819__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SystemConvertWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CSystemConvertWnd dialog
//-------------------------------------------------------------------------------------//
class CSystemConvertWnd : public CBaseDialog
{
// Construction
public:
	CSystemConvertWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSystemConvertWnd)
	enum { IDD = IDD_SYSTEM_CONVERT_WND };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSystemConvertWnd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//---------------------------------------------------------------------------------//
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//		
	bool                       ExecFolderBnt(UINT CtrlID);
	bool                       ExecConvertXYCaliBtn();
	//---------------------------------------------------------------------------------//
protected:

	// Generated message map functions
	//{{AFX_MSG(CSystemConvertWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnSrcFolderBtn();
	afx_msg void OnDstFolderBtn();	
	afx_msg void OnConvertXYCaliBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSTEMCONVERTWND_H__1AB5690A_17FC_4E09_A2F6_89276523A819__INCLUDED_)
