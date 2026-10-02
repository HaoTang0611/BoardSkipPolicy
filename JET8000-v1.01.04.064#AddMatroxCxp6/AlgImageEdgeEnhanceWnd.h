#if !defined(AFX_ALGIMAGEEDGEENHANCEWND_H__E11B0267_0BB3_4209_A137_C6417A1BCDAB__INCLUDED_)
#define AFX_ALGIMAGEEDGEENHANCEWND_H__E11B0267_0BB3_4209_A137_C6417A1BCDAB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AlgImageEdgeEnhanceWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
#include "ImageWnd.h"
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CAlgImageEdgeEnhanceWnd dialog
//-------------------------------------------------------------------------------------//
class CAlgImageEdgeEnhanceWnd : public CBaseDialog
{
// Construction
public:
	CAlgImageEdgeEnhanceWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAlgImageEdgeEnhanceWnd)
	enum { IDD = IDD_ALG_IMAGE_EDGE_ENHANCE_WND };
	CImageWnd   m_ImageWnd;
	CComboBox	m_EdgeModeCombox;
	CComboBox	m_Filter1ModeCombox;
	CComboBox	m_Filter2ModeCombox;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAlgImageEdgeEnhanceWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	//---------------------------------------------------------------------------------//
	void                       SetAlgBinaryParam(const CAlgBinaryParam &BinaryParam);
	void                       CopyAlgEdgeEnhanceParam(CAlgBinaryParam &BinaryParam) const;
	//---------------------------------------------------------------------------------//
	void                       SetImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr);
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//	
	IMAGE_SIZE                 m_ImageW;
	IMAGE_SIZE                 m_ImageH;
	IMAGE_SIZE                 m_ImageStep;
	IMAGE_SIZE                 m_BitCount;	
	IMAGE_PTR                  m_EdgePtr;
	IMAGE_PTR                  m_ImagePtr;		
	//---------------------------------------------------------------------------------//	
	CAlgBinaryParam            m_AlgBinaryParam;
	//---------------------------------------------------------------------------------//	
	void                       SwitchMultiLanguage();
	bool                       SetMultiLanauage(UINT ID, LPCTSTR Text, bool bCtrlID=true);
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//
	void                       ReleaseImageBuffer();
	//---------------------------------------------------------------------------------//	
	void                       ExecRedrawBtn();
	void                       UpdateKernelToUI();
	void                       UpdateUIToKernel();	
	void                       UpdateEdgeImageWnd(bool bFirst=false);
	//---------------------------------------------------------------------------------//
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAlgImageEdgeEnhanceWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual void OnOK();
	afx_msg void OnRedrawBtn();
	afx_msg void OnSelchangeEdgeModeCombo();
	afx_msg void OnSelchangeFilter1ModeCombo();
	afx_msg void OnKillfocusFilter1ParamEdit1();
	afx_msg void OnKillfocusFilter1ParamEdit2();
	afx_msg void OnSelchangeFilter2ModeCombo();
	afx_msg void OnKillfocusFilter2ParamEdit1();
	afx_msg void OnKillfocusFilter2ParamEdit2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALGIMAGEEDGEENHANCEWND_H__E11B0267_0BB3_4209_A137_C6417A1BCDAB__INCLUDED_)
