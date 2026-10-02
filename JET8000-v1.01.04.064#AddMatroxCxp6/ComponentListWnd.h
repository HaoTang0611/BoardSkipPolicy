#if !defined(AFX_COMPONENTLISTWND_H__9912E1F6_77C3_4A1D_914D_5B10425F21A3__INCLUDED_)
#define AFX_COMPONENTLISTWND_H__9912E1F6_77C3_4A1D_914D_5B10425F21A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ComponentListWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
#include "JETListCtrl.h"
//-------------------------------------------------------------------------------------//
//#define CThisListCtrl_07     CListCtrl//目前使用的列表控制類別	
#define CThisListCtrl_07     CJETListCtrl//目前使用的列表控制類別
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CComponentListWnd dialog
//-------------------------------------------------------------------------------------//
class CComponentListWnd : public CBaseDialog
{
// Construction
public:
	CComponentListWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CComponentListWnd)
	enum { IDD = IDD_COMPONENT_LIST_WND };
	CEdit           m_EditCtrl;
	CThisListCtrl_07	m_ComponentListWnd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CComponentListWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

public:
	//-------------------------------------------------------------------------//
	void                       SetProjectPtr(CAOIProject *Ptr);
	int                        CompareComponentItem(size_t index1, size_t index2);
	//-------------------------------------------------------------------------//
protected:
	//-------------------------------------------------------------------------//	
	int                        m_ComponentColID;
	int                        m_ComponentListSortMode;
	bool                       m_StopComponentListBeSelected;
	//-------------------------------------------------------------------------//
	int                        m_nItemAct;
	int                        m_nSubItemAct;
	CAOIProject               *m_ProjectPtr;
	CAOIComponent             *m_ComponentPtr;
	std::vector<CAOIComponent*> m_ComponentList; 
	//-------------------------------------------------------------------------//
	CAOIProject*               GetActiveProject();
	//-------------------------------------------------------------------------//
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//-------------------------------------------------------------------------//	
	bool                       BuildComponentListWnd();
	bool                       UpdateComponentListWnd();
	bool                       BuildComponentListWndHeader();
	//-------------------------------------------------------------------------//
	bool                       ClearComponentListWnd();
	//-------------------------------------------------------------------------//
	void                       SetComponentListColID(int val);
	int                        GetComponentListColD() const;
	//-------------------------------------------------------------------------//
	void                       SetComponentListSortMode(int val);
	int                        GetComponentListSortMode() const;	
	//-------------------------------------------------------------------------//	
	bool                       ExecUpdateParamByEdit();
	bool                       ExecReleaseParamCtrl();
	//-------------------------------------------------------------------------//
	void                       SetItemIndexAct(int nItem, int nSubItem);
	//-------------------------------------------------------------------------//
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CComponentListWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnPaint();
	afx_msg void OnColumnclickComponentListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnItemchangedComponentListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkComponentListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillfocusParamEdit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_COMPONENTLISTWND_H__9912E1F6_77C3_4A1D_914D_5B10425F21A3__INCLUDED_)
