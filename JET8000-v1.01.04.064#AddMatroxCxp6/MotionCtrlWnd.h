#if !defined(AFX_MOTIONCTRLWND_H__550DC743_7721_4446_B0A6_4A117064AB7E__INCLUDED_)
#define AFX_MOTIONCTRLWND_H__550DC743_7721_4446_B0A6_4A117064AB7E__INCLUDED_
//-------------------------------------------------------------------------------------//
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MotionCtrlWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "MotionStatusPane.h"
#include "MotionParamPane.h"
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CMotionCtrlWnd dialog
//-------------------------------------------------------------------------------------//
class CMotionCtrlWnd : public CDialog
{
// Construction
public:
	CMotionCtrlWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMotionCtrlWnd)
	enum { IDD = IDD_MOTION_CTRL_WND };
	CTabCtrl	m_PaneTabWnd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMotionCtrlWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
protected:
	//---------------------------------------------------------------------------------//			
	CMotionCtrlPaneStatus      m_MotionPaneStatus;
	CMotionCtrlPaneParam       m_MotionPaneParam;
	//---------------------------------------------------------------------------------//	
	bool                       InitPanelWnd();
	//---------------------------------------------------------------------------------//	
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//	
	void                       AdjustPaneWndPosition();
	void                       ExecSelchangePaneTabWnd();
	//---------------------------------------------------------------------------------//		
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMotionCtrlWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnSelchangePaneTabWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSaveParamBtn();
	afx_msg void OnLoadParamBtn();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnApplyParamBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
extern CMotionCtrlWnd MotionCtrlWnd;
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOTIONCTRLWND_H__550DC743_7721_4446_B0A6_4A117064AB7E__INCLUDED_)
