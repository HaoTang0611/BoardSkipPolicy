#if !defined(AFX_PHASETIDLPWND_H__6B4903EB_9293_4BA5_889C_F231B8E84596__INCLUDED_)
#define AFX_PHASETIDLPWND_H__6B4903EB_9293_4BA5_889C_F231B8E84596__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Light3DTiDLPWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "Light3DCtrl.h"
//-------------------------------------------------------------------------------------//
const int PHASE_TI_DLP_TIMER_ID      =  100;
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CLight3DTiDLPWnd dialog
//-------------------------------------------------------------------------------------//
class CLight3DTiDLPWnd : public CDialog
{
// Construction
public:
	CLight3DTiDLPWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLight3DTiDLPWnd)
	enum { IDD = IDD_LIGHT_3D_TIDLP_WND };
	CComboBox	m_LEDCurrentIDCombox;
	CComboBox	m_PatternExpNumCombox;
	CComboBox	m_PatternSequenceModeCombox;
	CComboBox	m_PatternBuildPhaseShiftDirectionCombox;
	CListCtrl	m_PatternListWnd;
	CComboBox	m_SequenceTriggerModeCombox;
	CComboBox	m_PatternSouceCombox;
	CComboBox	m_PatternBitRangeCombox;
	CComboBox	m_PatternFlashIndexCombox;
	CComboBox	m_OperationModeCombox;
	CComboBox	m_PatternBuildPhaseShiftCountCombo;
	CComboBox	m_PatternBuildBitDepthCombox;	
	CComboBox	m_PatternTriggerTypeCombox;	
	CComboBox	m_PatternBitDepthCombox;
	CComboBox	m_PatternLEDColorCombox;
	CComboBox	m_Light3DCastIDCombox;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLight3DTiDLPWnd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:
	//---------------------------------------------------------------------------------//	
	//---------------------------------------------------------------------------------//	

protected:
	//---------------------------------------------------------------------------------//		
	LIGHT_3D_CLS_PTR           m_Light3DCastPtr;
	//---------------------------------------------------------------------------------//		
	void                       SwitchMultiLanguage();
	//---------------------------------------------------------------------------------//
	bool                       BuildPatternBuildPhaseShiftCombox(CComboBox &Combox);
	bool                       BuildPatternBuildPhaseDirectionCombox(CComboBox &Combox);	
	//---------------------------------------------------------------------------------//	
	bool                       CheckPhasePtr();	
	int                        GetLEDCurrentID();
	bool                       UpdatePhasePtrToUI();
	bool                       UpdatePhasePtrStatusToUI();
	//---------------------------------------------------------------------------------//
	void                       StartTiDlpTimer();
	void                       KillTiDlpTimer();
	//---------------------------------------------------------------------------------//		
	bool                       RemovePatternItem(int idx);
	bool                       AddPatternItem(int idx, const TDLPPatItem &PatItem);		
	//---------------------------------------------------------------------------------//	
	bool                       DoPatternSendBtn();
	bool                       DoPatternSequenceValidateBtn();
	//---------------------------------------------------------------------------------//

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLight3DTiDLPWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSelchangeProjectIDCombo();
	afx_msg void OnConnectBtn();
	afx_msg void OnDisconnectBtn();	
	afx_msg void OnPatternBuildExportBtn();
	afx_msg void OnResetBtn();
	afx_msg void OnLEDCurrentSetBtn();
	afx_msg void OnLEDCurrentOffBtn();
	afx_msg void OnSaveIniBtn();
	afx_msg void OnLoadIniBtn();
	afx_msg void OnLEDAutoModeChk();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnStatusAutoUpdateChk();
	afx_msg void OnSelchangeOperationModeCombo();
	afx_msg void OnSelchangePatternBitDepthCombo();
	afx_msg void OnPatternAddBtn();
	afx_msg void OnPatternSendBtn();
	afx_msg void OnPatternReadBtn();
	afx_msg void OnPatternClearBtn();
	afx_msg void OnPatternSequenceValidateBtn();
	afx_msg void OnPatternSequencePlayBtn();
	afx_msg void OnPatternSequenceStopBtn();
	afx_msg void OnPatternSequencePauseBtn();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnRetrieveBtn();
	afx_msg void OnPatternDelBtn();
	afx_msg void OnSelchangePatternFlashIndexCombo();
	afx_msg void OnDblclkPatternListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPatternSequencePlayOnceBtn();
	afx_msg void OnPatternSequencePlayRepeatBtn();
	afx_msg void OnSelchangeLEDCurrentIDCombo();	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
extern CLight3DTiDLPWnd Light3DTiDLPWnd;
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PHASETIDLPWND_H__6B4903EB_9293_4BA5_889C_F231B8E84596__INCLUDED_)
