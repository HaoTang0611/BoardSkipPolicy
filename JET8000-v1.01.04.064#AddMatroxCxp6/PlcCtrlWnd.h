#if !defined(AFX_PLCCTRLWND_H__2CC7B280_8248_4B08_9A0F_D747F4F39AC6__INCLUDED_)
#define AFX_PLCCTRLWND_H__2CC7B280_8248_4B08_9A0F_D747F4F39AC6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PlcCtrlWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "PlcLaneStatusPane.h"
#include "PlcTowerLightPane.h"
#include "PlcLaneAdjustPane.h"
#include "PlcParamListPane.h"
#include "PlcNodeListPane.h"
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CPLCCtrlWnd dialog
//-------------------------------------------------------------------------------------//
class CPLCCtrlWnd : public CDialog
{
// Construction
public:
	CPLCCtrlWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPLCCtrlWnd)
	enum { IDD = IDD_PLC_CTRL_WND };
	CTabCtrl	m_PaneTabWnd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPLCCtrlWnd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:

protected:
	//---------------------------------------------------------------------------------//	
	CPLCCtrlPaneLaneStatus     m_LandStatusPane;
	CPLCCtrlPaneTowerLight     m_TowerLightPane;
	CPLCCtrlPaneLaneAdjust     m_LaneAdjustPane;
	CPLCCtrlPaneParamList      m_ParamListPane;          
	CPLCCtrlPaneNodeList       m_NodeListPane;
	//---------------------------------------------------------------------------------//	
	void                       SwitchMultiLanguage();
	void                       AdjustPaneWndPosition();
	void                       ExecSelchangePaneTabWnd();
	//---------------------------------------------------------------------------------//	
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPLCCtrlWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnSelchangePaneTabWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnConnectBtn();
	afx_msg void OnDisconnectBtn();
	afx_msg void OnSaftyPassChk();
	afx_msg void OnSaveParamBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
extern CPLCCtrlWnd  PlcCtrlWnd;
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLCCTRLWND_H__2CC7B280_8248_4B08_9A0F_D747F4F39AC6__INCLUDED_)
