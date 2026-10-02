#if !defined(AFX_PLCLANESTATUSPANE_H__355F43F4_D134_45D5_BB95_AA79E90B7036__INCLUDED_)
#define AFX_PLCLANESTATUSPANE_H__355F43F4_D134_45D5_BB95_AA79E90B7036__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PlcLaneStatusPane.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPLCCtrlPaneLaneStatus dialog
//-------------------------------------------------------------------------------------//
class CPLCCtrlPaneLaneStatus : public CDialog
{
// Construction
public:
	CPLCCtrlPaneLaneStatus(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPLCCtrlPaneLaneStatus)
	enum { IDD = IDD_PLC_LANE_STATUS_PANE };
	CStatic	m_PCBRightInWnd;
	CStatic	m_PCBLeftInWnd;
	CStatic	m_KeySwitchWnd;
	CStatic	m_HardwareBypassWnd_LA;	
	CStatic	m_HardwareBypassWnd_LB;	
	CStatic	m_StartLightWnd;
	CStatic	m_CurrentOverHeatWnd;
	CStatic	m_CurrentRearDoorWnd;	
	CStatic	m_CurrentFanWnd;
	CStatic	m_CurrentEMSWnd;
	CStatic	m_CurrentCapWnd;
	CStatic	m_CurrentAirWnd;
	CStatic	m_AlarmRearDoorWnd;
	CStatic	m_AlarmFanWnd;
	CStatic	m_AlarmEMSWnd;
	CStatic	m_AlarmAirWnd;
	CStatic	m_AlarmCapWnd;
	CStatic	m_FromNextSignalWnd_LB;
	CStatic	m_FromLastSignalWnd_LB;
	CStatic	m_PCBInSensorWnd_LB;
	CStatic	m_PCBOutSensorWnd_LB;
	CStatic	m_InPositionSensorWnd_LB;
	CStatic	m_InPositionSensorWnd_LB2;
	CStatic	m_SlowDownSensorWnd_LB;
	CStatic	m_FromNextSignalWnd_LA;
	CStatic	m_FromLastSignalWnd_LA;
	CStatic	m_PCBInSensorWnd_LA;
	CStatic	m_PCBOutSensorWnd_LA;
	CStatic	m_InPositionSensorWnd_LA;
	CStatic	m_InPositionSensorWnd_LA2;
	CStatic	m_SlowDownSensorWnd_LA;		
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPLCCtrlPaneLaneStatus)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
protected:
	//---------------------------------------------------------------------------------//	
	int                        m_RepeatCount;
	//---------------------------------------------------------------------------------//	
	CBitmap                    m_LEDGreen;
	CBitmap                    m_LEDRed;
	CBitmap                    m_LEDGray;
	CBitmap                    m_LEDGreenSmall;
	CBitmap                    m_LEDGraySmall;
	CBitmap                    m_LEDRedSmall;
	//---------------------------------------------------------------------------------//
	int                        m_CurrentAirStats;
	int                        m_CurrentFanStats;
	int                        m_CurrentEMSStats;	
	int                        m_CurrentRearDoorStats;	
	int                        m_CurrentFrontCapStats;	
	int                        m_CurrentKeySwitchStats;
	int                        m_CurrentOverHeatStats;
	int                        m_GreenLightStats;
	int                        m_HardBypassStats_LA;
	int                        m_HardBypassStats_LB;

	//­y¹Dª¬ºA	
	int                        m_LaneLoopMode_LA;
	int                        m_LaneLoopCount_LA;
	int                        m_PCBInDirection;
	int                        m_ConveryerSensorPCBIn_LA;
	int                        m_ConveryerSensorPCBSlow_LA;	
	int                        m_ConveryerSensorPCBStop_LA;
	int                        m_ConveryerSensorPCBOut_LA;
	int                        m_ConveryerSensorPCBStop_LA2;

	int                        m_ConveryerSensorPCBIn_LB;
	int                        m_ConveryerSensorPCBSlow_LB;	
	int                        m_ConveryerSensorPCBStop_LB;
	int                        m_ConveryerSensorPCBOut_LB;
	int                        m_ConveryerSensorPCBStop_LB2;

	int                        m_SignalFromLast_LA;
	int                        m_SignalFromNext_LA;
	int                        m_SignalFromLast_LB;
	int                        m_SignalFromNext_LB;

	int                        m_LaneLoopMode_LB;
	int                        m_LaneLoopCount_LB;
	//---------------------------------------------------------------------------------//
	void                       UpdateLaneStatus();
	void                       EnableLaneStatus();
	void                       SwitchMultiLanguage();
	bool                       ExecLaneLoop_LA();
	bool                       ExecLaneLoop_LB();
	//---------------------------------------------------------------------------------//	
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPLCCtrlPaneLaneStatus)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSendToLastSignalChkLA();
	afx_msg void OnSendToNextSignalChkLA();
	afx_msg void OnSendOkSignalChkLA();
	afx_msg void OnSendNgSignalChkLA();
	afx_msg void OnPCBInBtnLA();
	afx_msg void OnPCBBackBtnLA();
	afx_msg void OnPCBBackOutBtnLA();
	afx_msg void OnPCBOutBtnLA();
	afx_msg void OnPCBReinBtnLA();
	afx_msg void OnPCBInBackRepeatChkLA();
	afx_msg void OnPCBClampChkLA();
	afx_msg void OnPCBStopBarChkLA();
	afx_msg void OnPCBInBtnLB();
	afx_msg void OnPCBBackBtnLB();
	afx_msg void OnPCBBackOutBtnLB();
	afx_msg void OnPCBOutBtnLB();
	afx_msg void OnPCBReInBtnLB();
	afx_msg void OnPCBClampChkLB();
	afx_msg void OnPCBStopBarChkLB();	
	afx_msg void OnSendToLastSignalChkLB();
	afx_msg void OnSendToNextSignalChkLB();
	afx_msg void OnSendOkSignalChkLB();
	afx_msg void OnSendNgSignalChkLB();
	afx_msg void OnPCBInBackRepeatChkLB();
	afx_msg void OnSendStageAlarmChkLA();
	afx_msg void OnSendInspectionAlarmChkLA();
	afx_msg void OnSendStageAlarmChkLB();
	afx_msg void OnSendInspectionAlarmChkLB();	
	afx_msg void OnPCBIn2ndBtnLA();
	afx_msg void OnPCBIn3rdBtnLA();
	afx_msg void OnPCBIn2ndBtnLB();
	afx_msg void OnPCBIn3rdBtnLB();
	afx_msg void OnPCBAutoOutInBtnLA();
	afx_msg void OnPCBAutoBackInBtnLA();
	afx_msg void OnPCBAutoOutInBtnLB();
	afx_msg void OnPCBAutoBackInBtnLB();
	afx_msg void OnMachineStartBtn();
	afx_msg void OnMachineResetBtn();
	afx_msg void OnMachineStopBtn();
	afx_msg void OnPCBClearBtnLA();
	afx_msg void OnPCBClearBtnLB();
	afx_msg void OnPCBOutWithInBtnLA();
	afx_msg void OnPCBOutWithInBtnLB();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLCLANESTATUSPANE_H__355F43F4_D134_45D5_BB95_AA79E90B7036__INCLUDED_)
