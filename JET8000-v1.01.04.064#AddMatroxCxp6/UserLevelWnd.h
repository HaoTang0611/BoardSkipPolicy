#if !defined(AFX_USERLEVELWND_H__8E01C1FE_6166_4C0D_9EE6_9DDF87C2BA52__INCLUDED_)
#define AFX_USERLEVELWND_H__8E01C1FE_6166_4C0D_9EE6_9DDF87C2BA52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UserLevelWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
#include "JETListCtrl.h"
//-------------------------------------------------------------------------------------//
#define CThisListCtrl_34     CJETListCtrl//目前使用的列表控制類別	
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CUserLevelWnd dialog
//-------------------------------------------------------------------------------------//
class CUserLevelWnd : public CBaseDialog
{
// Construction
public:
	CUserLevelWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CUserLevelWnd)
	enum { IDD = IDD_USER_LEVEL_WND };
	CThisListCtrl_34	m_LevelListCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUserLevelWnd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	//---------------------------------------------------------------------------------//	
	void                       SetChargeLevel(USER_LEVEL_MODE Lv);
	USER_LEVEL_MODE            GetResultLevel() const;
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//	
	USER_LEVEL_MODE            m_ResultLevel;//結果的權限
	USER_LEVEL_MODE            m_ChargeLevel;//負責的權限
	//---------------------------------------------------------------------------------//	
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//	
	bool                       BuildLevelListWnd();
	bool                       BuildLevelListWndHeader();
	//---------------------------------------------------------------------------------//	

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CUserLevelWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual void OnOK();
	afx_msg void OnDblclkLevelListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USERLEVELWND_H__8E01C1FE_6166_4C0D_9EE6_9DDF87C2BA52__INCLUDED_)
