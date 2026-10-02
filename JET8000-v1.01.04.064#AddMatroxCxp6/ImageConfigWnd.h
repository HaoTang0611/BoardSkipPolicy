#if !defined(AFX_IMAGECONFIGWND_H__C3F713C9_94F9_498A_B817_0F9D293E5A0F__INCLUDED_)
#define AFX_IMAGECONFIGWND_H__C3F713C9_94F9_498A_B817_0F9D293E5A0F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImageConfigWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CImageConfigWnd dialog
//-------------------------------------------------------------------------------------//
class CImageConfigWnd : public CBaseDialog
{
// Construction
public:
	CImageConfigWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CImageConfigWnd)
	enum { IDD = IDD_IMAGE_CONFIG_WND };
	CComboBox	m_FrameSliceCombox3;
	CComboBox	m_FrameSliceCombox2;
	CComboBox	m_FrameSliceCombox1;
	CComboBox	m_FrameTypeCombox;
	CComboBox	m_CameraCombox;	
	CComboBox	m_FuncModeCombox;	
	CComboBox	m_CaliModeCombox;
	CListCtrl	m_FrameListWnd;
	CListCtrl	m_SliceListWnd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageConfigWnd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
protected:
	//---------------------------------------------------------------------------------//	
	std::vector<UINT>          m_LEDUIChkList;
	std::vector<UINT>          m_LEDUIEditList;
	//---------------------------------------------------------------------------------//	
	std::vector<TSliceParam>   m_SliceParamBackup;
	std::vector<TFrameParam>   m_FrameParamBackup;
	//---------------------------------------------------------------------------------//	
	void                       SwitchMultiLanguage();	
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//			
	bool                       m_StopSliceItemChanged;
	BOOL                       BuildSliceListWnd();
	BOOL                       BuildSliceListWndHeader();	
	//---------------------------------------------------------------------------------//	
	bool                       m_StopFrameItemChanged;
	BOOL                       BuildFrameListWnd();
	BOOL                       BuildFrameListWndHeader();	
	//---------------------------------------------------------------------------------//	
	void                       BuildSliceCombox();
	//---------------------------------------------------------------------------------//	
	void                       UpdateSliceParamToUI(int index);
	void                       UpdateSliceParamToUI(const TSliceParam &Param);	
	bool                       UpdateUIToSliceParam(TSliceParam &Param);
	//---------------------------------------------------------------------------------//	
	void                       UpdateFrameParamToUI(int index);
	void                       UpdateFrameParamToUI(const TFrameParam &Param);
	bool                       UpdateUIToFrameParam(TFrameParam &Param);
	//---------------------------------------------------------------------------------//	
	bool                       ExecCheckImageConfig();
	bool                       ExecUpdateFrameSliceUI(FRAME_TYPE FrameType);
	//---------------------------------------------------------------------------------//	
// Implementation
protected:
	//---------------------------------------------------------------------------------//
	//---------------------------------------------------------------------------------//

	// Generated message map functions
	//{{AFX_MSG(CImageConfigWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSliceAddBtn();
	afx_msg void OnSliceModifyBtn();
	afx_msg void OnClickSliceListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkSliceListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSliceSaveBtn();
	afx_msg void OnSliceResetBtn();
	afx_msg void OnSliceLoadBtn();
	afx_msg void OnSliceDeleteBtn();
	afx_msg void OnItemchangedSliceListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSliceClearBtn();
	afx_msg void OnFrameSaveBtn();
	afx_msg void OnFrameLoadBtn();
	afx_msg void OnFrameAddBtn();
	afx_msg void OnFrameModifyBtn();
	afx_msg void OnFrameDeleteBtn();
	afx_msg void OnFrameClearBtn();
	afx_msg void OnClickFrameListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedFrameListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeFrameTypeCombo();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnCheckBtn();	
	afx_msg void OnSliceSetAllGainBtn();
	afx_msg void OnSliceSetAllExpTimeBtn();
	afx_msg void OnSliceSetAllTargetGrayBtn();
	afx_msg void OnSliceSetAllVerifyTolBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGECONFIGWND_H__C3F713C9_94F9_498A_B817_0F9D293E5A0F__INCLUDED_)
