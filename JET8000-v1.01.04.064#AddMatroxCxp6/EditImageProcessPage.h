#if !defined(AFX_EDITIMAGEPROCESSPAGE_H__5A652F8C_174C_412B_B1CF_FE5A680D94AD__INCLUDED_)
#define AFX_EDITIMAGEPROCESSPAGE_H__5A652F8C_174C_412B_B1CF_FE5A680D94AD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditImageProcessPage.h : header file
//
//-------------------------------------------------------------------------------------//
#include "AlgParam.h"
#include "EditImageBinaryWnd.h"
#include "EditImageColorFilterWnd.h"
#include "EditImagePatternWnd.h"
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CEditImageProcessPage dialog

class CEditImageProcessPage : public CDialog
{
// Construction
public:
	CEditImageProcessPage(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEditImageProcessPage)
	enum { IDD = IDD_EDIT_IMAGE_PROCESS_WND };
	CComboBox	m_FilterModeCombox2;
	CComboBox	m_FilterParamCombox2;
	CComboBox	m_FilterModeCombox1;
	CComboBox	m_FilterParamCombox1;
	CComboBox	m_BinaryModeCombox;
	CComboBox	m_ImageSourceCombox;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditImageProcessPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
public:
		
protected:
	//---------------------------------------------------------------------------------//	
	CString                    m_WindowText;
	CAOIWnd                   *m_WndPtr;	
	CAOIModel                 *m_ModelPtr;
	CAOIProject               *m_ProjectPtr;    
	CAlgBinaryParam            m_BinaryParam;	
	//---------------------------------------------------------------------------------//
	UUID                       m_uidWnd;//AOI Object的唯一碼
	CEditImageBinaryWnd        m_wndImageBinary;
	CEditImageColorFilterWnd   m_wndColorFilter;
	CEditImagePatternWnd       m_wndImagePattern;
	//---------------------------------------------------------------------------------//
	bool                       m_StopUpdateGainValue;
	//---------------------------------------------------------------------------------//
	void                       CloseProject();
	CAOIProject*               GetActiveProject();
	void                       AdjustUIForWndSelected();
	//---------------------------------------------------------------------------------//
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//
	void                       ChangeDrawModelMode();
	DRAW_MODEL_MODE            GetDrawModelMode() const;
	//---------------------------------------------------------------------------------//	
	void                       PostMessageToMainFrameWnd(UINT message, WPARAM wParam, LPARAM lParam);
	void                       SendMessageToMainFrameWnd(UINT message, WPARAM wParam, LPARAM lParam);
	//---------------------------------------------------------------------------------//
	void                       ResetWndAlgParam();
	void                       ClearWndSelected();
	void                       UpdateWndSelected();
	void                       UpdateWndSelected_Fd();
	void                       UpdateWndSelected_Mark();
	void                       UpdateWndSelected_Barcode();
	void                       UpdateWndSelected_Component();
	bool                       UpdateWndSelectedKernel(CAOIWnd *WndPtr);
	void                       UpdateAlgResult();
	bool                       UpdateAlgParamImage(bool Modified, bool RedrawAlgImae);
	void                       UpdateWndCaptionText(CAOIModel *ModelPtr, CAOIWnd *WndPtr);
	//---------------------------------------------------------------------------------//
	void                       LockUIWnd(bool bLock);
	bool                       GetLockUIWnd() const;//取得是否鎖住UIWnd
	//---------------------------------------------------------------------------------//
	void                       UpdateUIToParam_ImageGain();
	//---------------------------------------------------------------------------------//
	bool                       ExecSaveLogModelWndOperate(LPCTSTR sKey, LPCTSTR sOld, LPCTSTR sNew);
	//---------------------------------------------------------------------------------//
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEditImageProcessPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnSelchangeImageSourceCombo();
	afx_msg void OnSelchangeBinaryModeCombo();
	afx_msg void OnSelchangeFilterModeCombo();
	afx_msg void OnSelchangeFilterParamCombo();
	afx_msg void OnSelchangeFilterModeCombo2();
	afx_msg void OnSelchangeFilterParamCombo2();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnGrayInvertChk();
	afx_msg void OnBinaryInvertChk();
	afx_msg void OnShowRawImageChk();
	afx_msg void OnKillfocusGainParamEdit();
	afx_msg void OnGainParamChk();
	afx_msg void OnChangeGainParamEdit();	
	afx_msg void OnShowAllBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITIMAGEPROCESSPAGE_H__5A652F8C_174C_412B_B1CF_FE5A680D94AD__INCLUDED_)
