#if !defined(AFX_PROJECTPARAMWND_H__C45311AE_2B3F_4A71_B061_86D689EBF1A9__INCLUDED_)
#define AFX_PROJECTPARAMWND_H__C45311AE_2B3F_4A71_B061_86D689EBF1A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProjectParamWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
#include "ProjectParamBasicPane.h"
#include "ProjectParamPaneAlarm.h"
#include "ProjectParamPaneSave.h"
#include "ProjectParamPaneBarcode.h"
#include "ProjectParamPaneRepair.h"
#include "ProjectParamPaneSpecTest.h"
#include "ProjectParamPaneVersionCode.h"
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CProjectParamWnd dialog
//-------------------------------------------------------------------------------------//
class CProjectParamWnd : public CBaseDialog
{
// Construction
public:
	CProjectParamWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProjectParamWnd)
	enum { IDD = IDD_PROJECT_PARAM_WND };
	CTabCtrl	m_PaneTabWnd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProjectParamWnd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:
	//---------------------------------------------------------------------------------//	
	TProjectParameter&         GetProjectParameter();
	void                       SetProjectParameter(CAOIProject *ProjectPtr, const TProjectParameter &Param);
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//	
	CString                    m_WndText;
	CAOIProject               *m_ProjectPtr;
	TProjectParameter          m_ProjectParameter;
	//---------------------------------------------------------------------------------//	
	CProjectParamPaneBasic     m_ProjectPaneBasic;
	CProjectParamPaneSave      m_ProjectPaneSave;
	CProjectParamPaneBarcode   m_ProjectPaneBarcode;
	CProjectParamPaneAlarm     m_ProjectPaneAlarm;	
	CProjectParamPaneRepair    m_ProjectPanelRepair;
	CProjectParamPaneSpecTest  m_ProjectPaneSpecTest;
	CProjectParamPaneVersionCode m_ProjectVersionCode;
	//---------------------------------------------------------------------------------//	
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	void                       AdjustPaneWndPosition();
	void                       ExecSelchangePaneTabWnd(); 	                           
	//---------------------------------------------------------------------------------//	
	bool                       InitPanelWnd();
	bool                       AddPaneTabWnd(LPCTSTR Title, CWnd &Wnd, CTabCtrl &TabWnd, int &Index);
	//---------------------------------------------------------------------------------//
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProjectParamWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnSelchangePaneTabWnd(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROJECTPARAMWND_H__C45311AE_2B3F_4A71_B061_86D689EBF1A9__INCLUDED_)
