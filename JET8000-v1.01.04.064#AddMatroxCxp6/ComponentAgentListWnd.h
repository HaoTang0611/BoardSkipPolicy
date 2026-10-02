#if !defined(AFX_COMPONENTAGENTLISTWND_H__F2281BDD_602B_47B3_B39C_E1777349AE64__INCLUDED_)
#define AFX_COMPONENTAGENTLISTWND_H__F2281BDD_602B_47B3_B39C_E1777349AE64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ComponentAgentListWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
#include "JETListCtrl.h"
//-------------------------------------------------------------------------------------//
#define CThisListCtrl_69     CJETListCtrl//目前使用的列表控制類別
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CComponentAgentListWnd dialog
//-------------------------------------------------------------------------------------//
class CComponentAgentListWnd : public CBaseDialog
{
// Construction
public:
	CComponentAgentListWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CComponentAgentListWnd)
	enum { IDD = IDD_COMPONENT_AGENT_LIST_WND };
	CThisListCtrl_69	m_AgentListWnd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CComponentAgentListWnd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:
	//-------------------------------------------------------------------------//
	void                       SetComponentPtr(CAOIComponent *Ptr);	
	//-------------------------------------------------------------------------//
protected:
	//-------------------------------------------------------------------------//		
	CAOIComponent              *m_MasterPtr;
	CAOIComponent              *m_ResultPtr;		
	std::vector<TComponentNode> m_AgentList;
	//-------------------------------------------------------------------------//	
	void                       SwitchMultiLanguage();
	bool                       SetMultiLanauage(UINT ID, LPCTSTR Text, bool bCtrlID=true);
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//-------------------------------------------------------------------------//	
	bool                       ClearAgentListWnd();
	bool                       BuildAgentListWnd();	
	bool                       UpdateAgentListWnd();	
	bool                       BuildAgentWndHeader();	
	void                       UpdateAgentInfo(const TComponentNode &Agent);
	//-------------------------------------------------------------------------//	
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CComponentAgentListWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnItemchangedAgentListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkAgentListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMPONENTAGENTLISTWND_H__F2281BDD_602B_47B3_B39C_E1777349AE64__INCLUDED_)
