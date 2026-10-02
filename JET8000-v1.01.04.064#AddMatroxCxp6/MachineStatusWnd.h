#if !defined(AFX_MACHINESTATUSWND_H__D364296A_9917_48EB_B76C_A567EE4A3A72__INCLUDED_)
#define AFX_MACHINESTATUSWND_H__D364296A_9917_48EB_B76C_A567EE4A3A72__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MachineStatusWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMachineStatusWnd dialog
//-------------------------------------------------------------------------------------//
class CMachineStatusWnd : public CDialog
{
// Construction
public:
	CMachineStatusWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMachineStatusWnd)
	enum { IDD = IDD_MACHEIN_STATUS_WND };
	CStatic	m_AlarmWnd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMachineStatusWnd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	//---------------------------------------------------------------------------------//	
	CString                    m_InfoText;
	CString                    m_ErrorString;
	AOI_EXCEPTION_CODE         m_AOIExceptionCode;	
	RECT                       m_AlarmWndRect;
	bool                       m_MachineAlarm;	
	bool                       m_AutoHideReady;
	HFONT                      m_AlarmWndFont;
	int                        m_AlarmWndFontSizeH; 
	//---------------------------------------------------------------------------------//	
	bool                       GetIsPolling() const;
	//---------------------------------------------------------------------------------//		
	void                       ResetMachineAlarm();
	bool                       GetMachineAlarm() const;
	bool                       GetAutoHideReady() const;
	void                       SetMachineAlarm(bool Alarm);
	//---------------------------------------------------------------------------------//	
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//
	void                       RedrawWnd();
	void                       DrawAlarmWnd(HDC hDC);
	void                       ClearAlamWndFont();
	void                       CreateAlamWndFont();
	//---------------------------------------------------------------------------------//
	bool                       CheckMachineReady();
	bool                       CheckMachineReadyFn();
	//---------------------------------------------------------------------------------//
	void                       ResetInfoText();
	void                       SetInfoText(LPCTSTR Text);
	//---------------------------------------------------------------------------------//
	bool                       ExecPollingMachineStatus();
	//---------------------------------------------------------------------------------//	
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMachineStatusWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
extern CMachineStatusWnd MachineStatusWnd;
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MACHINESTATUSWND_H__D364296A_9917_48EB_B76C_A567EE4A3A72__INCLUDED_)
