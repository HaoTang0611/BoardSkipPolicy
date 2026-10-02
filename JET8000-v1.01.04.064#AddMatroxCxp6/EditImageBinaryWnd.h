#if !defined(AFX_EDITIMAGEBINARYWND_H__518D9B94_91A8_455C_9E7B_1C132DA0BBF1__INCLUDED_)
#define AFX_EDITIMAGEBINARYWND_H__518D9B94_91A8_455C_9E7B_1C132DA0BBF1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditImageBinaryWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditImageBinaryWnd dialog

class CEditImageBinaryWnd : public CBasicDialog
{
// Construction
public:
	CEditImageBinaryWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEditImageBinaryWnd)
	enum { IDD = IDD_EDIT_IMAGE_BINARY_WND };
	CSpinButtonCtrl	m_AdaThresholdCalcSizeSpin;
	CSpinButtonCtrl	m_AdaThresholdGapSpin;
	CSliderCtrl	m_AdaThresholdGapSlider;
	CSpinButtonCtrl	m_AveThresholdBiasSpin;
	CSliderCtrl	m_AveThresholdBiasSlider;
	CSpinButtonCtrl	m_AveThresholdBelowSpin;
	CSliderCtrl	m_AveThresholdBelowSlider;
	CSpinButtonCtrl	m_AveThresholdAboveSpin;
	CSliderCtrl	m_AveThresholdAboveSlider;
	CSpinButtonCtrl	m_DynamicThresholdSpin;
	CSliderCtrl	m_DynamicThresholdSlider;
	CSpinButtonCtrl	m_FixThresholdLowSpin;
	CSliderCtrl	m_FixThresholdLowSlider;
	CSpinButtonCtrl	m_FixThresholdHighSpin;
	CSliderCtrl	m_FixThresholdHighSlider;
	CSpinButtonCtrl	m_WeightingBlueSpin;
	CSliderCtrl	m_WeightingBlueSlider;
	CSpinButtonCtrl	m_WeightingGreenSpin;
	CSliderCtrl	m_WeightingGreenSlider;
	CSpinButtonCtrl	m_WeightingRedSpin;
	CSliderCtrl	m_WeightingRedSlider;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditImageBinaryWnd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:
	//---------------------------------------------------------------------------------//
	void                       SetBinaryParam(CAOIWnd *WndPtr, CAlgBinaryParam *ParamPtr, bool UpdateToUI);
	void                       UpdateBinaryParamToUI();
	void                       AdjustUIForBinaryParam();
	void                       UpdateBinaryWndEnable();
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//
	CAOIWnd                   *m_WndPtr;
	CAlgBinaryParam*           m_BinaryParamPtr;	
	//---------------------------------------------------------------------------------//	
	int                        m_FixedThresholdMax;
	int                        m_FixedThresholdMin;
	int                        m_DynamicThresholdMax;
	int                        m_DynamicThresholdMin;
	int                        m_RelativeBiasMax;
	int                        m_RelativeBiasMin;
	int                        m_RelativeThresholdMax;
	int                        m_RelativeThresholdMin;
	int                        m_AdaptiveThresholdGapMax;
	int                        m_AdaptiveThresholdGapMin;
	int                        m_AdaptiveThresholdCalcSizeMin;
	int                        m_AdaptiveThresholdCalcSizeMax;
	//---------------------------------------------------------------------------------//	
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//
	void                       ChangeDrawModelMode();
	//---------------------------------------------------------------------------------//
	bool                       CheckBinaryParamPtr();	
	void                       UpdateBinaryUIToParam();
	//---------------------------------------------------------------------------------//
	void                       UpdateBinaryParamToParentWnd();
	void                       SendParentWndMessage(UINT message, WPARAM wParam, LPARAM lParam);
	void                       PostParentWndMessage(UINT message, WPARAM wParam, LPARAM lParam);
	//---------------------------------------------------------------------------------//	
	bool                       ExecSaveLogModelWndOperate_Binary(UINT  nGroup, LPCTSTR sKey, LPCTSTR sOld, LPCTSTR sNew);
	//---------------------------------------------------------------------------------//
	bool                       CheckFixThresholdValue(int Th);//½T»{©T©w»Ö­È
	//---------------------------------------------------------------------------------//	
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEditImageBinaryWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnDeltaposWeightingRedSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposWeightingGreenSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposWeightingBlueSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposFixThresholdHighSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposFixThresholdLowSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposDynamicThresholdSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposAveThresholdAboveSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposAveThresholdBelowSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposAveThresholdBiasSpin(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnDeltaposAdaptiveThresholdGapSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposAdaptiveThresholdCalcSizeSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRatioThresholdHighBtn();	
	afx_msg void OnRatioThresholdLowBtn();	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITIMAGEBINARYWND_H__518D9B94_91A8_455C_9E7B_1C132DA0BBF1__INCLUDED_)
