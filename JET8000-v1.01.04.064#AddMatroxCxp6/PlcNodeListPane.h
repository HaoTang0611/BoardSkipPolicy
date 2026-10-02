#if !defined(AFX_PLCNODELISTPANE_H__EEE34A1B_26D3_467A_B10D_0413EB5EAB85__INCLUDED_)
#define AFX_PLCNODELISTPANE_H__EEE34A1B_26D3_467A_B10D_0413EB5EAB85__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PlcNodeListPane.h : header file
//
//-------------------------------------------------------------------------------------//
#include "Plc_Basic.h"
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CPLCCtrlPaneNodeList dialog
//-------------------------------------------------------------------------------------//
const int PLC_NODE_LIST_TIMER = 100;
//-------------------------------------------------------------------------------------//
class CPLCCtrlPaneNodeList : public CDialog
{
// Construction
public:
	CPLCCtrlPaneNodeList(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPLCCtrlPaneNodeList)
	enum { IDD = IDD_PLC_NODE_LIST_PANE };
	CListCtrl	m_TempListWnd;
	CListCtrl	m_NodeListWnd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPLCCtrlPaneNodeList)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	//---------------------------------------------------------------------------------//
	std::vector<TPlcNode>      m_PLCNodeList;
	std::vector<TPlcNode>      m_PLCNodeListTemp;
	//---------------------------------------------------------------------------------//
	void                       BuildPlcNodeListWnd();
	void                       BuildPlcTempListWnd();
	void                       UpdatePlcNodeListWnd();
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPLCCtrlPaneNodeList)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnAddPlcNodeBtn();
	afx_msg void OnRemovePlcNodeBtn();
	afx_msg void OnSetNodeListBtn();
	afx_msg void OnSavePlcNodeBtn();
	afx_msg void OnRestorePlcNodeBtn();
	afx_msg void OnGetNodeListBtn();
	afx_msg void OnPlcNodeReadBtn();
	afx_msg void OnPlcNodeWriteBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLCNODELISTPANE_H__EEE34A1B_26D3_467A_B10D_0413EB5EAB85__INCLUDED_)
