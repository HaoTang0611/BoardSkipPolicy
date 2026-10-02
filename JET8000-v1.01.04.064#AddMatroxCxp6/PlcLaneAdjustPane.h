#if !defined(AFX_PLCLANEADJUSTPANE_H__0C5C9FDC_5D04_4190_BEE1_42D851ABC845__INCLUDED_)
#define AFX_PLCLANEADJUSTPANE_H__0C5C9FDC_5D04_4190_BEE1_42D851ABC845__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PlcLaneAdjustPane.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPLCCtrlPaneLaneAdjust dialog

class CPLCCtrlPaneLaneAdjust : public CDialog
{
// Construction
public:
	CPLCCtrlPaneLaneAdjust(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPLCCtrlPaneLaneAdjust)
	enum { IDD = IDD_PLC_LANE_ADJUST_PANE };
	CStatic	m_KeySwitchWnd;	
	CStatic	m_DisableWnd_LA;	
	CStatic	m_DisableWnd_LB;		
	CStatic	m_SensorORGWnd_LB;	
	CStatic	m_SensorLimitWnd_LB;	
	CStatic	m_SensorPCBInWnd_LB;
	CStatic	m_SensorPCBSlowWnd_LB;
	CStatic	m_SensorPCBStopWnd_LB;
	CStatic	m_SensorPCBOutWnd_LB;
	CStatic	m_SensorPCBStopWnd_LB2;
	CStatic	m_SensorORGWnd_LA;	
	CStatic	m_SensorLimitWnd_LA;	
	CStatic	m_SensorPCBInWnd_LA;
	CStatic	m_SensorPCBSlowWnd_LA;
	CStatic	m_SensorPCBStopWnd_LA;
	CStatic	m_SensorPCBOutWnd_LA;		
	CStatic	m_SensorPCBStopWnd_LA2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPLCCtrlPaneLaneAdjust)
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
	UINT                       m_ActiveLane;
	CBitmap                    m_LEDGreen;
	CBitmap                    m_LEDRed;
	CBitmap                    m_LEDGray;
	CBitmap                    m_LEDGreenSmall;
	CBitmap                    m_LEDGraySmall;
	CBitmap                    m_LEDRedSmall;	
	//---------------------------------------------------------------------------------//
	int                        m_KeySwitchStats;
	int                        m_LaneAdjustDisable_LA;
	int                        m_LaneAdjustDisable_LB;
	//---------------------------------------------------------------------------------//
	int                        m_LaneAdjustSensorORG_LA;
	int                        m_LaneAdjustSensorLimit_LA;
	int                        m_ConveryerSensorPCBIn_LA;
	int                        m_ConveryerSensorPCBSlow_LA;	
	int                        m_ConveryerSensorPCBStop_LA;
	int                        m_ConveryerSensorPCBOut_LA;
	int                        m_ConveryerSensorPCBStop_LA2;
	
	int                        m_LaneAdjustSensorORG_LB;
	int                        m_LaneAdjustSensorLimit_LB;
	int                        m_ConveryerSensorPCBIn_LB;
	int                        m_ConveryerSensorPCBSlow_LB;	
	int                        m_ConveryerSensorPCBStop_LB;
	int                        m_ConveryerSensorPCBOut_LB;
	int                        m_ConveryerSensorPCBStop_LB2;
	//---------------------------------------------------------------------------------//
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//
	void                       UpdateLaneStatus();
	void                       EnableLaneStatus();
	//---------------------------------------------------------------------------------//
	bool                       CheckKeySwitchTurnOff(bool bShowMsg=true);
	//---------------------------------------------------------------------------------//
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPLCCtrlPaneLaneAdjust)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnHomeBtnLA();
	afx_msg void OnMoveToBtnLA();
	afx_msg void OnMaxLimitBtnLA();	
	afx_msg void OnPCBInBtnLA();
	afx_msg void OnPCBOutBtnLA();
	afx_msg void OnPCBBackBtnLA();
	afx_msg void OnJogSpeedFastRadioLA();
	afx_msg void OnJogSpeedSlowRadioLA();
	afx_msg void OnJogMoveChkLA();
	afx_msg void OnHomeBtnLB();
	afx_msg void OnMoveToBtnLB();
	afx_msg void OnMaxLimitBtnLB();
	afx_msg void OnJogMoveChkLB();
	afx_msg void OnJogSpeedFastRadioLB();
	afx_msg void OnJogSpeedSlowRadioLB();
	afx_msg void OnPCBInBtnLB();
	afx_msg void OnPCBOutBtnLB();
	afx_msg void OnPCBBackBtnLB();
	afx_msg void OnSkewPitchBtnLA();	
	afx_msg void OnFixed14LaneChk();
	afx_msg void OnPCBClearBtnLA();
	afx_msg void OnPCBClearBtnLB();
	afx_msg void OnSkewPitchBtnLB();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLCLANEADJUSTPANE_H__0C5C9FDC_5D04_4190_BEE1_42D851ABC845__INCLUDED_)
