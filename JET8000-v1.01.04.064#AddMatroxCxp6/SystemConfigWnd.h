#if !defined(AFX_SYSTEMCONFIGWND_H__FF30E362_A479_462E_91B0_5CF8064A6938__INCLUDED_)
#define AFX_SYSTEMCONFIGWND_H__FF30E362_A479_462E_91B0_5CF8064A6938__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SystemConfigWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
#include "SystemAIPane.h"
#include "SystemBasicPane.h"
#include "SystemFolderPane.h"
#include "SystemColorPane.h"
#include "SystemParamPane.h"
#include "SystemDefaultPane.h"
#include "SystemLogPane.h"
#include "SystemOnlinePane.h"
#include "SystemAdvancePane.h"
#include "SystemOfflinePane.h"
#include "SystemM2MPane.h"
#include "SystemMESPane.h"
#include "SystemThreadPane.h"
#include "SystemDebugPane.h"
#include "SystemAuthorizationPane.h"
#include "SystemHASIPane.h"
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CSystemConfigWnd dialog
//-------------------------------------------------------------------------------------//
class CSystemConfigWnd : public CBaseDialog
{
// Construction
public:
	CSystemConfigWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSystemConfigWnd)
	enum { IDD = IDD_SYSTEM_CONFIG_WND };
	CTabCtrl	m_PaneTabWnd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSystemConfigWnd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:
	//---------------------------------------------------------------------------------//	
	TSystemParameter           GetSystemParameter() const;
	void                       SetSystemParameter(const TSystemParameter &SysParam);
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//
	TSystemParameter           m_SysParam;
	//---------------------------------------------------------------------------------//
	CString                    m_WndText;
	CSystemBasicPane           m_PaneBasic;
	CSystemFolderPane          m_PaneFolder;
	CSystemColorPane           m_PaneColor;
	CSystemParamPane           m_PaneParam;
	CSystemDefaultPane         m_PaneDefault;
	CSystemLogPane             m_PaneLog;
	CSystemOnlinePane          m_PaneOnline;
	CSystemOfflinePane         m_PaneOffline;
	CSystemAdvancePane         m_PaneAdvance;	
	CSystemAIPane              m_PaneAI;	
	CSystemMESPane             m_PaneMES;
	CSystemM2MPane             m_PaneM2M;
	CSystemThreadPane          m_PaneThread;
	CSystemDebugPane           m_PaneDebug;
	CSystemAuthorizationPane   m_PaneAuthorization;
	CSystemHASIPane            m_PaneHASI;
	//---------------------------------------------------------------------------------//	
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	void                       AdjustPaneWndPosition();
	void                       ExecSelchangePaneTabWnd(); 	                           
	//---------------------------------------------------------------------------------//	
	bool                       InitPanelWnd();
	bool                       AddPaneTabWnd(LPCTSTR Title, CWnd &Wnd, CTabCtrl &TabWnd, int &Index);
	//---------------------------------------------------------------------------------//	
	bool                       SetPaneSystemParameter();
	//---------------------------------------------------------------------------------//	
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSystemConfigWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSelchangePaneTabWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSaveBtn();
	afx_msg void OnLoadBtn();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSTEMCONFIGWND_H__FF30E362_A479_462E_91B0_5CF8064A6938__INCLUDED_)
