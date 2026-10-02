#if !defined(AFX_PROJECTLISTWND_H__E33EA7C7_C086_407C_A5B0_E6ABAA030EE8__INCLUDED_)
#define AFX_PROJECTLISTWND_H__E33EA7C7_C086_407C_A5B0_E6ABAA030EE8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProjectListWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
#include "ImageWnd.h"
#include "JETListCtrl.h"
//-------------------------------------------------------------------------------------//
#define CThisListCtrl_20     CListCtrl//目前使用的列表控制類別	
//#define CThisListCtrl_20     CJETListCtrl//目前使用的列表控制類別
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CProjectListWnd dialog
//-------------------------------------------------------------------------------------//
class CProjectListWnd : public CBaseDialog
{
// Construction
public:
	CProjectListWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProjectListWnd)
	enum { IDD = IDD_PROJECT_LIST_WND };
	CImageWnd   	m_ProjectMapWnd;
	CThisListCtrl_20	m_AllFileListWnd;
	CThisListCtrl_20	m_LatestFileListWnd;
	CComboBox   m_MapIndexCombox;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProjectListWnd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
public:
	//---------------------------------------------------------------------------------//		
	CString                    GetSelectedFilename() const;
	int                        GetAllFileListCompareID() const;
	int                        CompareAllFileList(size_t index1, size_t index2);
	//---------------------------------------------------------------------------------//		
protected:
	//---------------------------------------------------------------------------------//	
	UINT                       m_AllFileListMode;
	CString                    m_SelectedFilename;//選取到的檔名
	//---------------------------------------------------------------------------------//
	std::vector<WIN32_FIND_DATA>  m_AllFilenameList;
	std::vector<CString>       m_LatestFilenameList;
	int                        m_AllFileListCompareID;
	int                        m_AllFileListSortMode;
	//---------------------------------------------------------------------------------//
	bool                       m_StopAllListBeSelected;
	bool                       m_StopLatestListBeSelected;
	//---------------------------------------------------------------------------------//
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//
	bool                       BuildAllFileListWnd();
	bool                       BuildAllFileListWnd_Folder();
	bool                       BuildAllFileListWnd_OpenCode();
	bool                       BuildAllFileListWnd_Search();
	bool                       BuildAllFileListWndHeader();
	//---------------------------------------------------------------------------------//
	bool                       BuildLatestFileListWnd();
	bool                       BuildLatestFileListWndHeader();
	//---------------------------------------------------------------------------------//
	bool                       ExecLoadProjectMap(LPCTSTR pfilename);
	//---------------------------------------------------------------------------------//
	bool                       ExecDeleteFileBtn();
	//---------------------------------------------------------------------------------//
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProjectListWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnItemchangedLatestFileListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkLatestFileListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedAllFileListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkAllFileListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAllFileFolderBtn();
	afx_msg void OnLatestFileOfflineChk();
	virtual void OnOK();
	afx_msg void OnColumnclickAllFileListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAllFileFolderRadio();
	afx_msg void OnAllFileOpenCodeRadio();
	afx_msg void OnClickLatestFileListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickAllFileListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeMapFrameCombo();
	afx_msg void OnAllFileSearchRadio();
	afx_msg void OnSmallMapChk();
	afx_msg void OnUpdateLatestFileBtn();
	afx_msg void OnDeleteFileBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROJECTLISTWND_H__E33EA7C7_C086_407C_A5B0_E6ABAA030EE8__INCLUDED_)
