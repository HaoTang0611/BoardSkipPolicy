#if !defined(AFX_USERREGISTERWND_H__20957308_9793_458E_9502_B5DF6585BB79__INCLUDED_)
#define AFX_USERREGISTERWND_H__20957308_9793_458E_9502_B5DF6585BB79__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UserRegisterWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
#include "JETListCtrl.h"
//-------------------------------------------------------------------------------------//
#define CThisListCtrl_35     CJETListCtrl//目前使用的列表控制類別	
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CUserRegisterWnd dialog
//-------------------------------------------------------------------------------------//
class CUserRegisterWnd : public CBaseDialog
{
// Construction
public:
	CUserRegisterWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CUserRegisterWnd)
	enum { IDD = IDD_USER_REGISTER_WND };
	CThisListCtrl_35	m_UserListCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUserRegisterWnd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//		
	TUserNode                  m_LoginUser;
	//---------------------------------------------------------------------------------//		
	std::vector<TUserNode>     m_UserList;
	std::vector<TUserNode>     m_UserListAdd;
	std::vector<TUserNode>     m_UserListDel;
	bool                       m_UserModified;
	bool                       m_StopUserListBeSelected;
	//---------------------------------------------------------------------------------//	
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//	
	bool                       BuildUserListWnd();
	bool                       BuildUserListWndHeader();
	//---------------------------------------------------------------------------------//	
	bool                       DelUserFingerprint(std::vector<TUserNode> &DelList);
	bool                       LogModifyUserList();
	//---------------------------------------------------------------------------------//	
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CUserRegisterWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnSaveBtn();
	afx_msg void OnDelBtn();
	afx_msg void OnNewBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USERREGISTERWND_H__20957308_9793_458E_9502_B5DF6585BB79__INCLUDED_)
