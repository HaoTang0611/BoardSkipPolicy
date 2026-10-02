#if !defined(AFX_SYSTEMINITIALWND_H__D7BAFD2E_00F3_4C40_9093_297F24191330__INCLUDED_)
#define AFX_SYSTEMINITIALWND_H__D7BAFD2E_00F3_4C40_9093_297F24191330__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SystemInitialWnd.h : header file
//
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CSystemInitialWnd dialog
//-------------------------------------------------------------------------------------//
class CSystemInitialWnd : public CDialog
{
// Construction
public:
	CSystemInitialWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSystemInitialWnd)
	enum { IDD = IDD_SYSTEM_INITIAL_WND };
	CStatic m_BarcodeDeviceIcon8;
	CStatic m_BarcodeDeviceIcon7;
	CStatic m_BarcodeDeviceIcon6;
	CStatic m_BarcodeDeviceIcon5;
	CStatic m_BarcodeDeviceIcon4;
	CStatic m_BarcodeDeviceIcon3;
	CStatic m_BarcodeDeviceIcon2;
	CStatic m_BarcodeDeviceIcon1;
	CStatic m_ITSCommImg;
	CStatic m_DTKLibImg;
	CStatic m_HonLibImg;
	CStatic m_AlgLibImg;
	CStatic	m_PhaseCtrlImg;
	CStatic	m_PlcInitImg;
	CStatic	m_MotionInitImg;
	CStatic	m_LightCtrlImg;
	CStatic	m_ImageLibImg;
	CStatic	m_CameraInitImg;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSystemInitialWnd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:
	//---------------------------------------------------------------------------------//	
	bool                       SystemRelease();   //系統釋放
	bool                       SystemInitialize();//系統初始化
	LPCTSTR                    GetErrorString() { return this->m_strInitial; }	
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//	
	CBitmap                    m_LEDGreen;
	CBitmap                    m_LEDRed;
	CBitmap                    m_LEDGray;
	CString                    m_strInitial;
	//---------------------------------------------------------------------------------//	
	void                       RedrawWnd();
	//---------------------------------------------------------------------------------//	
	void                       SwitchMultiLanguage();	
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	void                       UpdateBarcodeDeviceIcon();
	void                       UpdateBarcodeDeviceIconKernel(CBarcode_Basic *Ptr, CStatic &Icon, UINT EditID);
	//---------------------------------------------------------------------------------//
	bool                       SystemInitialize_CameraUnit(DWORD SleepTime, CString &strErr);
	bool                       SystemInitialize_MotionXYZUnit(DWORD SleepTime, CString &strErr);
	bool                       SystemInitialize_PLCUnit(DWORD SleepTime, CString &strErr);
	bool                       SystemInitialize_LightCtrlBoard(DWORD SleepTime, CString &strErr);
	bool                       SystemInitialize_PhaseCtrlBoard(DWORD SleepTime, CString &strErr);
	bool                       SystemInitialize_ImageLibrary(DWORD SleepTime, CString &strErr);
	bool                       SystemInitialize_DTKLibrary(DWORD SleepTime, CString &strErr);
	bool                       SystemInitialize_HonLibrary(DWORD SleepTime, CString &strErr);
	bool                       SystemInitialize_ITS_Comm(DWORD SleepTime, CString &strErr);
	bool                       SystemInitialize_JetAlgLibrary(DWORD SleepTime, CString &strErr);
	bool                       SystemInitialize_BarcodeDevice(DWORD SleepTime, CString &strErr);
	//---------------------------------------------------------------------------------//
	bool                       ExecCameraResetBtn();

	bool                       ExecMotionResetBtn();

	bool                       ExecLightCtrlResetBtn();

	bool                       ExecPhaseCtrlResetBtn();
	//---------------------------------------------------------------------------------//
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSystemInitialWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnCameraResetBtn();
	afx_msg void OnMotionResetBtn();
	afx_msg void OnPlcResetBtn();
	afx_msg void OnLightCtrlResetBtn();
	afx_msg void OnLightCtrlConfigBtn();
	afx_msg void OnPhaseCtrlResetBtn();
	afx_msg void OnPhaseCtrlConfigBtn();
	afx_msg void OnITSCommResetBtn();
	afx_msg void OnBarcodeDeviceResetBtn();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
extern CSystemInitialWnd  SystemInitialWnd;
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSTEMINITIALWND_H__D7BAFD2E_00F3_4C40_9093_297F24191330__INCLUDED_)
