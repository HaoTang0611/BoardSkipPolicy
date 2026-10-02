#if !defined(AFX_FDLISTWND_H__ECA0FA78_4EBD_48A4_8AC0_D27D90060D1B__INCLUDED_)
#define AFX_FDLISTWND_H__ECA0FA78_4EBD_48A4_8AC0_D27D90060D1B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FdListWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
#include "JETListCtrl.h"
//-------------------------------------------------------------------------------------//
//#define CThisListCtrl_46     CListCtrl//目前使用的列表控制類別	
#define CThisListCtrl_46     CJETListCtrl//目前使用的列表控制類別
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CFdListWnd dialog
class CFdListWnd : public CBaseDialog
{
// Construction
public:
	CFdListWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFdListWnd)
	enum { IDD = IDD_FD_LIST_WND };
	CEdit           m_EditCtrl;
	CThisListCtrl_46	m_FdListWnd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFdListWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

public:
	//-------------------------------------------------------------------------//
	void                       SetProjectPtr(CAOIProject *Ptr);
	int                        CompareFdItem(size_t index1, size_t index2);
	//-------------------------------------------------------------------------//
protected:
	//-------------------------------------------------------------------------//	
	int                        m_FdColID;
	int                        m_FdListSortMode;
	bool                       m_StopFdListBeSelected;
	//-------------------------------------------------------------------------//
	int                        m_nItemAct;
	int                        m_nSubItemAct;
	CAOIFd                    *m_FdPtr;
	CAOIProject               *m_ProjectPtr;	
	std::vector<CAOIFd*>       m_FdList; 
	//-------------------------------------------------------------------------//
	CAOIProject*               GetActiveProject();
	//-------------------------------------------------------------------------//
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//-------------------------------------------------------------------------//	
	bool                       BuildFdListWnd();
	bool                       UpdateFdListWnd();
	bool                       BuildFdListWndHeader();
	//-------------------------------------------------------------------------//
	bool                       ClearFdListWnd();
	//-------------------------------------------------------------------------//
	void                       SetFdListColID(int val);
	int                        GetFdListColD() const;
	//-------------------------------------------------------------------------//
	void                       SetFdListSortMode(int val);
	int                        GetFdListSortMode() const;	
	//-------------------------------------------------------------------------//	
	bool                       ExecUpdateParamByEdit();
	bool                       ExecReleaseParamCtrl();
	//-------------------------------------------------------------------------//
	void                       SetItemIndexAct(int nItem, int nSubItem);
	//-------------------------------------------------------------------------//
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFdListWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnColumnclickFdListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedFdListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkFdListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FDLISTWND_H__ECA0FA78_4EBD_48A4_8AC0_D27D90060D1B__INCLUDED_)
