#if !defined(AFX_LOGOPERVIEWERWND_H__39BC8B40_73A4_478B_A991_94B284266242__INCLUDED_)
#define AFX_LOGOPERVIEWERWND_H__39BC8B40_73A4_478B_A991_94B284266242__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LogOperViewerWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
#include "JETListCtrl.h"
//-------------------------------------------------------------------------------------//
#define CThisListCtrl_55     CJETListCtrl//目前使用的列表控制類別	
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CLogOperViewerWnd dialog
//-------------------------------------------------------------------------------------//
class CLogOperViewerWnd : public CBaseDialog
{
// Construction
public:
	CLogOperViewerWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLogOperViewerWnd)
	enum { IDD = IDD_LOG_OPER_VIEWER_WND };
	CThisListCtrl_55	m_ListWnd;
	CComboBox	m_UserCombox;
	CComboBox	m_ScoreCombox;
	CTime	m_BeginDate;
	CTime	m_BeginTime;
	CTime	m_EndDate;
	CTime	m_EndTime;	
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogOperViewerWnd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	//---------------------------------------------------------------------------------//	
	void                       SetShowFolder(bool bShow);
	int                        CompareLopOperList(size_t index1, size_t index2);
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//	
	int                        m_ColActIndex;
	int                        m_ColSortMode;
	bool                       m_ShowFolder;
	//---------------------------------------------------------------------------------//		
	std::vector<CString>       m_ItemList;
	std::vector<CString>       m_UserList;
	std::vector<CString>       m_ScoreList;	
	CString                    m_FileFolder;
	CString                    m_FileDateEnd;
	CString                    m_FileDateBegin;
	bool                       m_StopItemListBeSelected;
	//---------------------------------------------------------------------------------//		
	int                        GetColActIndex() const;
	void                       SetColActIndex(int val);	
	//---------------------------------------------------------------------------------//	
	int                        GetColSortMode() const;
	void                       SetColSortMode(int val);	
	//---------------------------------------------------------------------------------//		
	bool                       ExecBuildFunc();	
	bool                       CheckReloadFile();
	CString                    GetLogOperFolder();	
	CString                    GetLogOperFileDateEnd();	
	CString                    GetLogOperFileDateBegin();	
	CString                    GetLogOperRangeTimeEnd();	
	CString                    GetLogOperRangeTimeBegin();	
	bool                       FormatTime(LPCTSTR DateTime, CString &Str);		
	bool                       AddTagList(LPCTSTR Tag, std::vector<CString> &List);
	bool                       CheckTagFilter(UINT ChkID, CComboBox &Combox, CString &strFilter);
	bool                       BuildTagCombox(CComboBox &Combox, const std::vector<CString> &List);
	//---------------------------------------------------------------------------------//
	bool                       BuildListWnd();	
	CThisListCtrl_55&          GetBuildListWnd();
	bool                       BuildListWndHeader();	
	//---------------------------------------------------------------------------------//	
	void                       SwitchMultiLanguage();	
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//	
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLogOperViewerWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBuildBtn();
	afx_msg void OnColumnclickListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnFolderBtn();
	afx_msg void OnUserChk();
	afx_msg void OnSelchangeUserCombo();
	afx_msg void OnScoreChk();
	afx_msg void OnSelchangeScoreCombo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGOPERVIEWERWND_H__39BC8B40_73A4_478B_A991_94B284266242__INCLUDED_)
