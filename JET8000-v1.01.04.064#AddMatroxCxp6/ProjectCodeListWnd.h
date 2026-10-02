#if !defined(AFX_PROJECTCODELISTWND_H__D09C33C0_FBAC_4C56_BB98_C5EF3528360B__INCLUDED_)
#define AFX_PROJECTCODELISTWND_H__D09C33C0_FBAC_4C56_BB98_C5EF3528360B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProjectCodeListWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
#include "JETListCtrl.h"
//-------------------------------------------------------------------------------------//
//#define CThisListCtrl_50     CListCtrl//目前使用的列表控制類別	
#define CThisListCtrl_50     CJETListCtrl//目前使用的列表控制類別
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CProjectCodeListWnd dialog
//-------------------------------------------------------------------------------------//
class CProjectCodeListWnd : public CBaseDialog
{
// Construction
public:
	CProjectCodeListWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProjectCodeListWnd)
	enum { IDD = IDD_PROJECT_CODE_LIST_WND };
	CEdit               m_EditCtrl;
	CThisListCtrl_50	m_OpenCodeListWnd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProjectCodeListWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

public:
	//---------------------------------------------------------------------------------//
	void                       SetOpenCodeList(const std::vector<TProjectOpenCode> &OpenCodeList);
	void                       CloneOpenCodeList(std::vector<TProjectOpenCode> &OpenCodeList);
	int                        CompareOpenCodeItem(size_t index1, size_t index2);	
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//			
	int                        m_EditItemIndex;
	int                        m_EditSubItemIndex;	
	int                        m_OpenCodeListColD;
	int                        m_OpenCodeSortMode;
	CString                    m_ProjectFolder;
	std::vector<TProjectOpenCode> m_OpenCodeList;
	bool                       m_StopOpenCodeListBeSelected;
	//---------------------------------------------------------------------------------//
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//
	int                        GetOpenCodeListColD() const;
	void                       SetOpenCodeListColD(int val);
	//---------------------------------------------------------------------------------//
	int                        GetOpenCodeSortMode() const;
	void                       SetOpenCodeSortMode(int val);
	//---------------------------------------------------------------------------------//	
	bool                       BuildProjectCodeListWnd();
	bool                       BuildProjectCodeListWndHeader();	
	//---------------------------------------------------------------------------------//
	void                       ClearItemInfo();
	void                       UpdateItemInfo(const TProjectOpenCode &OpenCode);
	//---------------------------------------------------------------------------------//
	bool                       ExecUpdateParamByEdit();
	bool                       ExecItemchangedOpenCodeListWnd(int nItem);
	bool                       ExecDblclkOpenCodeListWnd(int nItem, int nSubItem);
	//---------------------------------------------------------------------------------//
	bool                       ExecAddProjectBtn(LPCTSTR pfilename);
	bool                       ExecAddProjectName(LPCTSTR pfilename);
	bool                       CheckProjectNameExist(LPCTSTR pfilename);
	//---------------------------------------------------------------------------------//
	bool                       ExecDelProjectName(int nItem);
	//---------------------------------------------------------------------------------//
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProjectCodeListWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnColumnclickOpenCodeListWnd(NMHDR* pNMHDR, LRESULT* pResult);	
	afx_msg void OnItemchangedOpenCodeListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkOpenCodeListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillfocusEditCtrl();
	afx_msg void OnAddProjectBtn();
	afx_msg void OnDelProjectBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROJECTCODELISTWND_H__D09C33C0_FBAC_4C56_BB98_C5EF3528360B__INCLUDED_)
