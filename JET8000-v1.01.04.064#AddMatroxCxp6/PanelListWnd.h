#if !defined(AFX_PANELLISTWND_H__E5803AEC_50E8_4A73_9B6C_C6FF400B8C48__INCLUDED_)
#define AFX_PANELLISTWND_H__E5803AEC_50E8_4A73_9B6C_C6FF400B8C48__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PanelListWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
#include "JETListCtrl.h"
//-------------------------------------------------------------------------------------//
//#define CThisListCtrl_48     CListCtrl//目前使用的列表控制類別	
#define CThisListCtrl_48     CJETListCtrl//目前使用的列表控制類別
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CPanelListWnd dialog

class CPanelListWnd : public CBaseDialog
{
// Construction
public:
	CPanelListWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPanelListWnd)
	enum { IDD = IDD_PANEL_LIST_WND };
	CEdit           m_EditCtrl;
	CThisListCtrl_48	m_PanelListWnd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPanelListWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

public:
	//-------------------------------------------------------------------------//
	void                       SetProjectPtr(CAOIProject *Ptr);
	int                        ComparePanelItem(size_t index1, size_t index2);
	//-------------------------------------------------------------------------//
protected:
	//-------------------------------------------------------------------------//	
	int                        m_PanelColID;
	int                        m_PanelListSortMode;
	bool                       m_StopPanelListBeSelected;
	//-------------------------------------------------------------------------//
	int                        m_nItemAct;
	int                        m_nSubItemAct;
	CAOIPanel                 *m_PanelPtr;
	CAOIProject               *m_ProjectPtr;	
	std::vector<CAOIPanel*>    m_PanelList; 
	//-------------------------------------------------------------------------//
	CAOIProject*               GetActiveProject();
	//-------------------------------------------------------------------------//
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//-------------------------------------------------------------------------//	
	bool                       BuildPanelListWnd();
	bool                       UpdatePanelListWnd();
	bool                       BuildPanelListWndHeader();
	//-------------------------------------------------------------------------//
	bool                       ClearPanelListWnd();
	//-------------------------------------------------------------------------//
	void                       SetPanelListColID(int val);
	int                        GetPanelListColD() const;
	//-------------------------------------------------------------------------//
	void                       SetPanelListSortMode(int val);
	int                        GetPanelListSortMode() const;	
	//-------------------------------------------------------------------------//	
	bool                       ExecUpdateParamByEdit();
	bool                       ExecReleaseParamCtrl();
	//-------------------------------------------------------------------------//
	void                       SetItemIndexAct(int nItem, int nSubItem);
	//-------------------------------------------------------------------------//
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPanelListWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnColumnclickPanelListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedPanelListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkPanelListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PANELLISTWND_H__E5803AEC_50E8_4A73_9B6C_C6FF400B8C48__INCLUDED_)
