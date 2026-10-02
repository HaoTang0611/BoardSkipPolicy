#if !defined(AFX_PROJECTMAPPANE_H__664F906F_A5E3_44C3_B8D2_1FF1F523A832__INCLUDED_)
#define AFX_PROJECTMAPPANE_H__664F906F_A5E3_44C3_B8D2_1FF1F523A832__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProjectMapPane.h : header file
//
//-------------------------------------------------------------------------------------//
#include "resource.h"
#include "ImageWnd.h"
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CProjectMapPane dialog
//-------------------------------------------------------------------------------------//
class CProjectMapPane : public CDialog
{
// Construction
public:
	CProjectMapPane(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProjectMapPane)
	enum { IDD = IDD_PROJECT_MAP_PANE };
	CComboBox	m_FuncCombox;
	CImageWnd	m_ImageWnd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProjectMapPane)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

public:
	//---------------------------------------------------------------------------------//	
	void                       SetShowMark(bool bShow);//顯示專案特徵點
	void                       SetShowFiducial(bool bShow);//顯示專案定位點
	void                       SetShowBarcode(bool bShow);//顯示專案條碼
	void                       SetShowComponent(bool bShow);//顯示專案零件
	void                       SetShowComponentName(bool bShow);//顯示專案零件名稱
	bool                       UpdateProjectMapWndRegion();
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//		
	CAOIProject               *m_ProjectPtr;
	//---------------------------------------------------------------------------------//	
	bool                       BuildFuncComboxWnd();
	//---------------------------------------------------------------------------------//	
	bool                       BuildProjectMapWnd();
	bool                       SwitchProjectMapWnd();
	bool                       UpdateProjectMapWnd();	
	bool                       ClearProjectMapWnd();
	//---------------------------------------------------------------------------------//	
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	void                       AdjustContrlWnd(UINT nType, int cx, int cy);
	//---------------------------------------------------------------------------------//
	bool                       UpdateParamToUI();	
	//---------------------------------------------------------------------------------//
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProjectMapPane)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnShowFdChk();
	afx_msg void OnShowPanelChk();
	afx_msg void OnShowBoardChk();
	afx_msg void OnShowBarcodeChk();
	afx_msg void OnShowComponentChk();
	afx_msg void OnShowCameraChk();
	afx_msg void OnShowAllBtn();
	afx_msg void OnShowNoneBtn();	
	afx_msg void OnSelchangeFuncCombo();	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROJECTMAPPANE_H__664F906F_A5E3_44C3_B8D2_1FF1F523A832__INCLUDED_)
