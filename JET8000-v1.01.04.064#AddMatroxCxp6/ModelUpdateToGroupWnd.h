#if !defined(AFX_MODELUPDATETOGROUPWND_H__BB7FEDDF_AD0F_4D5E_8913_F45C31DF95F2__INCLUDED_)
#define AFX_MODELUPDATETOGROUPWND_H__BB7FEDDF_AD0F_4D5E_8913_F45C31DF95F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModelUpdateToGroupWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CModelUpdateToGroupWnd dialog

class CModelUpdateToGroupWnd : public CBaseDialog
{
// Construction
public:
	CModelUpdateToGroupWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CModelUpdateToGroupWnd)
	enum { IDD = IDD_MODEL_UPDATE_TO_GROUP_WND };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModelUpdateToGroupWnd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	//---------------------------------------------------------------------------------//		
	void                       SetModelPtr(CAOIModel *ModelPtr);
	void                       SetUpdateParam(TModelUpdateToGroupParam &Param);
	void                       GetUpdateParam(TModelUpdateToGroupParam &Param);
	//---------------------------------------------------------------------------------//	
protected:
	CAOIModel                 *m_ModelPtr;	
	CAOIModel*                 GetModelPtr();

	TModelUpdateToGroupParam   m_UpdateParam;
	//---------------------------------------------------------------------------------//
	void                       SwitchMultiLanguage();
	//---------------------------------------------------------------------------------//
	void                       UpdateParamToUI();
	void                       UpdateUIToParam();
	//---------------------------------------------------------------------------------//	

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CModelUpdateToGroupWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnUpdateAllRadio();
	afx_msg void OnUpdateSelRadio();
	virtual void OnOK();
	afx_msg void OnUpdateAddOneChk();
	afx_msg void OnUpdateDeleteChk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODELUPDATETOGROUPWND_H__BB7FEDDF_AD0F_4D5E_8913_F45C31DF95F2__INCLUDED_)
