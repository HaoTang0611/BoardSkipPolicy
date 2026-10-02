#if !defined(AFX_DEFECTLISTWND_H__15586074_1ED3_459F_B076_52A89AF2C754__INCLUDED_)
#define AFX_DEFECTLISTWND_H__15586074_1ED3_459F_B076_52A89AF2C754__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DefectListWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CDefectListWnd dialog
//-------------------------------------------------------------------------------------//
class CDefectListWnd : public CBaseDialog
{
// Construction
public:
	CDefectListWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDefectListWnd)
	enum { IDD = IDD_DEFECT_LIST_WND };
	CListCtrl	m_AlgorithmListWnd;
	CListCtrl	m_DefectListWnd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDefectListWnd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:
	//---------------------------------------------------------------------------------//		
	WND_DEFECT_ID              GetDefectID();
	ALG_TYPE                   GetAlgorithm();
	void                       SetLandPtr(CAOILand *LandPtr);
	//---------------------------------------------------------------------------------//
protected:
	CAOILand                  *m_LandPtr;
	WND_DEFECT_ID              m_DefectID;
	ALG_TYPE                   m_Algorithm;
	//---------------------------------------------------------------------------------//	
	CImageList                 m_DefectImageList;
	CImageList                 m_AlgorithmImageList;
	bool                       m_StopDefectListBeSelected;
	bool                       m_StopAlgorithmListBeSelected;
	//---------------------------------------------------------------------------------//	
	void                       SwitchMultiLanguage();
	//---------------------------------------------------------------------------------//	
	BOOL                       ClearDefectListWnd();
	BOOL                       ClearAlgorithmListWnd();	
	BOOL                       BuildDefectListWnd();
	BOOL                       BuildAlgorithmListWnd();
	bool                       FilterDefectID(WND_DEFECT_ID DefectID);
	bool                       FilterAlgorithm(ALG_TYPE AlgType);
	UINT                       GetDefectListIcon(WND_DEFECT_ID DefectID, bool Small);
	UINT                       GetAlgorithmListIcon(ALG_TYPE AlgType, bool Small);
	//---------------------------------------------------------------------------------//	
	bool                       ExecOnOK();
	//---------------------------------------------------------------------------------//	
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDefectListWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnAlgorithmFilterChk();
	afx_msg void OnClickDefectListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedDefectListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	afx_msg void OnDblclkAlgorithmListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEFECTLISTWND_H__15586074_1ED3_459F_B076_52A89AF2C754__INCLUDED_)
