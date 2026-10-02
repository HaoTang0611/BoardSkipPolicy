#if !defined(AFX_PLCTOWERLIGHTPANE_H__D9F3DD85_78FF_4F8D_9B99_8195864E440D__INCLUDED_)
#define AFX_PLCTOWERLIGHTPANE_H__D9F3DD85_78FF_4F8D_9B99_8195864E440D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PlcTowerLightPane.h : header file
//
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CPLCCtrlPaneTowerLight dialog
//-------------------------------------------------------------------------------------//
class CPLCCtrlPaneTowerLight : public CDialog
{
// Construction
public:
	CPLCCtrlPaneTowerLight(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPLCCtrlPaneTowerLight)
	enum { IDD = IDD_PLC_TOWER_LIGHT_PANE };
	CComboBox	m_StatePCBBackCombox;
	CComboBox	m_StatePCBOutCombox;
	CComboBox	m_StatePCBInCombox;
	CComboBox	m_StateWaitNextCombox;
	CComboBox	m_StateWaitLastCombox;
	CComboBox	m_StateBypassCombox;
	CComboBox	m_StateInspectionCombox;
	CComboBox	m_StateStopCombox;
	CComboBox	m_TowerLightModeCombox;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPLCCtrlPaneTowerLight)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	//---------------------------------------------------------------------------------//	
	//---------------------------------------------------------------------------------//	

protected:
	//---------------------------------------------------------------------------------//	
	void                       SwitchMultiLanguage();
	//---------------------------------------------------------------------------------//	
	bool                       UpdateLightTowerStateToUI();
	//---------------------------------------------------------------------------------//
	bool                       ExecEnableStateUI();//±Ò¥Îª¬ºA¤¶­±
	bool                       ExecEnableStateUI(bool Enable);//±Ò¥Îª¬ºA¤¶­±
	//---------------------------------------------------------------------------------//

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPLCCtrlPaneTowerLight)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnSelchangeTowerLightModeCombo();
	afx_msg void OnSelchangeStateStopCombo();
	afx_msg void OnSelchangeStateInspectionCombo();
	afx_msg void OnSelchangeStateBypassCombo();
	afx_msg void OnSelchangeStateWaitLastCombo();
	afx_msg void OnSelchangeStateWaitNextCombo();
	afx_msg void OnSelchangeStatePCBInCombo();
	afx_msg void OnSelchangeStatePCBOutCombo();
	afx_msg void OnSelchangeStatePCBBackCombo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLCTOWERLIGHTPANE_H__D9F3DD85_78FF_4F8D_9B99_8195864E440D__INCLUDED_)
