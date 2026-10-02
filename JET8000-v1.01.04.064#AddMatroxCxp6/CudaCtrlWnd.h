#if !defined(AFX_CUDACTRLWND_H__158DB616_6AB1_46EE_8D58_4B9FBEE2BB39__INCLUDED_)
#define AFX_CUDACTRLWND_H__158DB616_6AB1_46EE_8D58_4B9FBEE2BB39__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CudaCtrlWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CCudaCtrlWnd dialog
//-------------------------------------------------------------------------------------//
#define  CUDA_FUNCTION_MULTI_PHASE_4_2_CAST1        421
#define  CUDA_FUNCTION_MULTI_PHASE_4_2_CAST2        422
#define  CUDA_FUNCTION_MULTI_PHASE_4_2_CAST3        423
#define  CUDA_FUNCTION_MULTI_PHASE_4_2_CAST4        424

#define  CUDA_FUNCTION_MULTI_PHASE_4_4_CAST1        441
#define  CUDA_FUNCTION_MULTI_PHASE_4_4_CAST2        442
#define  CUDA_FUNCTION_MULTI_PHASE_4_4_CAST3        443
#define  CUDA_FUNCTION_MULTI_PHASE_4_4_CAST4        444

#define  CUDA_FUNCTION_MULTI_PHASE_4_4GC_CAST4      474
#define  CUDA_FUNCTION_MULTI_PHASE_4_5GC_CAST4      454
#define  CUDA_FUNCTION_MULTI_PHASE_4_6GC_CAST4      464

#define  CUDA_FUNCTION_MULTI_PHASE_4_2_CAST1_EXP2  4212
#define  CUDA_FUNCTION_MULTI_PHASE_4_2_CAST2_EXP2  4222
#define  CUDA_FUNCTION_MULTI_PHASE_4_2_CAST3_EXP2  4232
#define  CUDA_FUNCTION_MULTI_PHASE_4_2_CAST4_EXP2  4242

#define  CUDA_FUNCTION_MULTI_PHASE_4_4_CAST1_EXP2  4412
#define  CUDA_FUNCTION_MULTI_PHASE_4_4_CAST2_EXP2  4422
#define  CUDA_FUNCTION_MULTI_PHASE_4_4_CAST3_EXP2  4432
#define  CUDA_FUNCTION_MULTI_PHASE_4_4_CAST4_EXP2  4442
//-------------------------------------------------------------------------------------//
class CCudaCtrlWnd : public CBaseDialog
{
// Construction
public:
	CCudaCtrlWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCudaCtrlWnd)
	enum { IDD = IDD_CUDA_CTRL_WND };
	CProgressCtrl	m_ProgressWnd;
	CComboBox	m_CudaFuncComboxWnd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCudaCtrlWnd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
protected:
	//---------------------------------------------------------------------------------//
	void                       InitialParameters();
	void                       SwitchMultiLanguage();	
	//---------------------------------------------------------------------------------//
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCudaCtrlWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnCalculateBtn();
	afx_msg void OnResetCudaBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CUDACTRLWND_H__158DB616_6AB1_46EE_8D58_4B9FBEE2BB39__INCLUDED_)
