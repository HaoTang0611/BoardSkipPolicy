#if !defined(AFX_MOTIONSTATUSPANE_H__4A16DCF7_CD10_4F37_8D94_72BEA53C6FD9__INCLUDED_)
#define AFX_MOTIONSTATUSPANE_H__4A16DCF7_CD10_4F37_8D94_72BEA53C6FD9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MotionStatusPane.h : header file
//
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CMotionCtrlPaneStatus dialog
//-------------------------------------------------------------------------------------//
const int MOTION_STATUS_TIMER = 100;
const int MOTION_STATUS_TIMER_REPEAT_12 = 210;
const int MOTION_STATUS_TIMER_REPEAT_13 = 220;
const int MOTION_STATUS_TIMER_REPEAT_23 = 230;
//-------------------------------------------------------------------------------------//
enum MOTION_REPEAT_TO_MODE
{
	MOTION_REPEAT_TO_1,
	MOTION_REPEAT_TO_2,
	MOTION_REPEAT_TO_3,
	MOTION_REPEAT_TO_RETURN
};
//-------------------------------------------------------------------------------------//
class CMotionCtrlPaneStatus : public CDialog
{
// Construction
public:
	CMotionCtrlPaneStatus(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMotionCtrlPaneStatus)
	enum { IDD = IDD_MOTION_STATUS_PANE };
	CStatic	m_AlarmWndZ;
	CStatic	m_EmgWndZ;
	CStatic	m_InposWndZ;
	CStatic	m_NLimitWndZ;
	CStatic	m_PLimitWndZ;
	CStatic	m_OrgWndZ;
	CStatic	m_ReadyWndZ;
	CStatic	m_EnableWndZ;
	CStatic	m_AlarmWndY;
	CStatic	m_EmgWndY;
	CStatic	m_InposWndY;
	CStatic	m_NLimitWndY;
	CStatic	m_PLimitWndY;
	CStatic	m_OrgWndY;
	CStatic	m_ReadyWndY;
	CStatic	m_EnableWndY;
	CStatic	m_AlarmWndX;
	CStatic	m_EmgWndX;
	CStatic	m_InposWndX;
	CStatic	m_NLimitWndX;
	CStatic	m_PLimitWndX;
	CStatic	m_OrgWndX;
	CStatic	m_ReadyWndX;
	CStatic	m_EnableWndX;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMotionCtrlPaneStatus)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	//---------------------------------------------------------------------------------//
	void                       EnableUIWnd(BOOL bEnableWnd, UINT CmdID);//鎖住視窗
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//		
	CBitmap                    m_LEDGreen;
	CBitmap                    m_LEDRed;
	CBitmap                    m_LEDGray;
	CBitmap                    m_LEDGreenSmall;
	CBitmap                    m_LEDGraySmall;
	CBitmap                    m_LEDRedSmall;
	//---------------------------------------------------------------------------------//	
	double                     m_CommandPosX1;
	double                     m_CommandPosY1;
	double                     m_CommandPosZ1;

	double                     m_CommandPosX2;
	double                     m_CommandPosY2;
	double                     m_CommandPosZ2;

	double                     m_CommandPosX3;
	double                     m_CommandPosY3;
	double                     m_CommandPosZ3;
	//---------------------------------------------------------------------------------//	
	DWORD                      m_RepeatTime;
	MOTION_REPEAT_TO_MODE      m_RepeatToMode;
	//---------------------------------------------------------------------------------//	
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//
	bool                       EnableAxisUI();
	bool                       UpdateMotionStatus();
	bool                       ReadMotionStatus();
	bool                       ExecGrabImage();
	//---------------------------------------------------------------------------------//
	bool                       ExecRepeatMoveNext12();
	bool                       ExecRepeatMoveNext13();
	bool                       ExecRepeatMoveNext23();
	//---------------------------------------------------------------------------------//
	bool                       UpdateUIToParam();	
	//---------------------------------------------------------------------------------//
	bool                       SaveUIParamFile();
	bool                       LoadUIParamFile();
	CString                    GetUISectionName() const;
	//---------------------------------------------------------------------------------//	
	void                       LockUIWnd(bool bLock, UINT CmdID);//鎖住視窗	
	bool                       GetLockUIWnd() const;//取得是否鎖住UIWnd	
	//---------------------------------------------------------------------------------//	
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMotionCtrlPaneStatus)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnGoToPosBtn1();
	afx_msg void OnGoToPosBtn2();
	afx_msg void OnGoToPosBtn3();
	afx_msg void OnGetPosBtn1();
	afx_msg void OnGetPosBtn2();
	afx_msg void OnGetPosBtn3();
	afx_msg void OnNMovePosBtnX();
	afx_msg void OnPMovePosBtnX();
	afx_msg void OnNMovePosBtnY();
	afx_msg void OnPMovePosBtnY();
	afx_msg void OnNMovePosBtnZ();
	afx_msg void OnPMovePosBtnZ();
	afx_msg void OnGetPosBtnX();
	afx_msg void OnGetPosBtnY();
	afx_msg void OnGetPosBtnZ();
	afx_msg void OnJogChk();	
	afx_msg void OnEnableChkX();
	afx_msg void OnEnableChkY();
	afx_msg void OnEnableChkZ();
	afx_msg void OnHomeBtnX();
	afx_msg void OnHomeBtnY();
	afx_msg void OnHomeBtnZ();
	afx_msg void OnResetBtnX();
	afx_msg void OnResetBtnY();
	afx_msg void OnResetBtnZ();
	afx_msg void OnCheckLimitBtnX();
	afx_msg void OnCheckLimitBtnY();
	afx_msg void OnCheckLimitBtnZ();
	afx_msg void OnStartPosGoBtn();
	afx_msg void OnStartPosSetBtn();
	afx_msg void OnFocusPosGoBtn();
	afx_msg void OnFocusPosSetBtn();
	afx_msg void OnHomeAllBtn();
	afx_msg void OnORGPosGoBtn();
	afx_msg void OnPCBStopPosGoBtnLA();
	afx_msg void OnPCBStopPosSetBtnLA();
	afx_msg void OnPCBInPosGoBtn();
	afx_msg void OnPCBInPosSetBtn();
	afx_msg void OnRepeat12Chk();
	afx_msg void OnRepeat13Chk();
	afx_msg void OnRepeat23Chk();
	afx_msg void OnTimeTestBtn();
	afx_msg void OnPCBStopPosGoBtnLB();
	afx_msg void OnPCBStopPosSetBtnLB();
	afx_msg void OnLeavePosGoBtn();
	afx_msg void OnLeavePosSetBtn();
	afx_msg void OnORGPosSetBtn();
	afx_msg void OnPCBStopPosGoBtnLA2();
	afx_msg void OnPCBStopPosGoBtnLB2();
	afx_msg void OnPCBStopPosSetBtnLA2();
	afx_msg void OnPCBStopPosSetBtnLB2();
	afx_msg void OnSaveUIBtn();
	afx_msg void OnLoadUIBtn();
	afx_msg void OnLaneLedStopPosGoBtnLA();
	afx_msg void OnLaneLedStopPosSetBtnLA();
	afx_msg void OnLaneLedSlowPosGoBtnLA();
	afx_msg void OnLaneLedSlowPosSetBtnLA();
	afx_msg void OnLaneLedStopPosGoBtnLB();
	afx_msg void OnLaneLedStopPosSetBtnLB();
	afx_msg void OnLaneLedSlowPosGoBtnLB();
	afx_msg void OnLaneLedSlowPosSetBtnLB();
	afx_msg void OnLaneLedStopPosGoBtnLA2();
	afx_msg void OnLaneLedStopPosSetBtnLA2();
	afx_msg void OnLaneLedSlowPosGoBtnLA2();
	afx_msg void OnLaneLedSlowPosSetBtnLA2();
	afx_msg void OnLaneLedStopPosGoBtnLB2();
	afx_msg void OnLaneLedStopPosSetBtnLB2();
	afx_msg void OnLaneLedSlowPosGoBtnLB2();
	afx_msg void OnLaneLedSlowPosSetBtnLB2();	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOTIONSTATUSPANE_H__4A16DCF7_CD10_4F37_8D94_72BEA53C6FD9__INCLUDED_)
