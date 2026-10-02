#if !defined(AFX_BOARDLISTWND_H__5ED219D8_304A_48EF_82D4_D7BF95DC1611__INCLUDED_)
#define AFX_BOARDLISTWND_H__5ED219D8_304A_48EF_82D4_D7BF95DC1611__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BoardListWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
#include "JETListCtrl.h"
//-------------------------------------------------------------------------------------//
//#define CThisListCtrl_47     CListCtrl//目前使用的列表控制類別	
#define CThisListCtrl_47     CJETListCtrl//目前使用的列表控制類別
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CBoardListWnd dialog

class CBoardListWnd : public CBaseDialog
{
// Construction
public:
	CBoardListWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBoardListWnd)
	enum { IDD = IDD_BOARD_LIST_WND };
	CEdit           m_EditCtrl;
	CThisListCtrl_47	m_BoardListWnd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBoardListWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

public:
	//-------------------------------------------------------------------------//
	void                       SetProjectPtr(CAOIProject *Ptr);
	int                        CompareBoardItem(size_t index1, size_t index2);
	//-------------------------------------------------------------------------//
protected:
	//-------------------------------------------------------------------------//	
	int                        m_BoardColID;
	int                        m_BoardListSortMode;
	bool                       m_StopBoardListBeSelected;
	//-------------------------------------------------------------------------//
	int                        m_nItemAct;
	int                        m_nSubItemAct;
	CAOIBoard                 *m_BoardPtr;
	CAOIProject               *m_ProjectPtr;	
	std::vector<CAOIBoard*>    m_BoardList; 
	//-------------------------------------------------------------------------//
	CAOIProject*               GetActiveProject();
	//-------------------------------------------------------------------------//
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//-------------------------------------------------------------------------//	
	bool                       BuildBoardListWnd();
	bool                       UpdateBoardListWnd();
	bool                       BuildBoardListWndHeader();
	//-------------------------------------------------------------------------//
	bool                       ClearBoardListWnd();
	//-------------------------------------------------------------------------//
	void                       SetBoardListColID(int val);
	int                        GetBoardListColD() const;
	//-------------------------------------------------------------------------//
	void                       SetBoardListSortMode(int val);
	int                        GetBoardListSortMode() const;	
	//-------------------------------------------------------------------------//	
	bool                       ExecUpdateParamByEdit();
	bool                       ExecReleaseParamCtrl();
	//-------------------------------------------------------------------------//
	void                       SetItemIndexAct(int nItem, int nSubItem);
	//-------------------------------------------------------------------------//
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBoardListWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnColumnclickBoardListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedBoardListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkBoardListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BOARDLISTWND_H__5ED219D8_304A_48EF_82D4_D7BF95DC1611__INCLUDED_)
