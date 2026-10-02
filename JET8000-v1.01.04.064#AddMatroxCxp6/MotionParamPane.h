#if !defined(AFX_MOTIONPARAMPANE_H__0B359060_2234_4CBE_8C07_942FFA6DF5DA__INCLUDED_)
#define AFX_MOTIONPARAMPANE_H__0B359060_2234_4CBE_8C07_942FFA6DF5DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MotionParamPane.h : header file
//
//-------------------------------------------------------------------------------------//
#include "ParamUni.h"
//-------------------------------------------------------------------------------------//
#define CThisListCtrl_14     CListCtrl//目前使用的列表控制類別	
//-------------------------------------------------------------------------------------//
typedef struct _MotionParamItem
{	
	CParamUni  sParam;
	CString    sUnit;
} TMotionParamItem, *PMotionParamItem;
//-------------------------------------------------------------------------------------//
typedef struct _AxisParamItem
{
	CString    sTitle;
	CParamUni  sParamX;
	CParamUni  sParamY;
	CParamUni  sParamZ;
	CString    sUnit;
} TAxisParamItem, *PAxisParamItem;
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CMotionCtrlPaneParam dialog
//-------------------------------------------------------------------------------------//
class CMotionCtrlPaneParam : public CDialog
{
// Construction
public:
	CMotionCtrlPaneParam(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMotionCtrlPaneParam)
	enum { IDD = IDD_MOTION_PARAM_PANE };
	CThisListCtrl_14	m_ParamListWnd;
	CComboBox	m_ComboxCtrl;
	CEdit	m_EditCtrl;
	CThisListCtrl_14	m_AxisParamListWnd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMotionCtrlPaneParam)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:
	//---------------------------------------------------------------------------------//	
	bool                          BuildAllParamList();
	//---------------------------------------------------------------------------------//
	void                          SaveAllParamToINI();
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//		
	CParamUni                     *m_ParamActPtr;
	//---------------------------------------------------------------------------------//	
	std::vector<TMotionParamItem>  m_ParamList;
	std::vector<TAxisParamItem>    m_AxisParamList;
	//---------------------------------------------------------------------------------//	
	CString                        GetParamIniFilename();
	//---------------------------------------------------------------------------------//	
	bool                       SaveParamToINI();
	//---------------------------------------------------------------------------------//	
	CParamUni*                 GetActParamUni();
	void                       SetActParamUni(CParamUni *Ptr);	
	//---------------------------------------------------------------------------------//
	bool                       BuildParamList();
	bool                       BuildParamListWnd();
	bool                       BuildParamListWndHeader();	
	//---------------------------------------------------------------------------------//	
	bool                       SaveAxisParamToINI();
	bool                       BuildAxisParamList();
	bool                       BuildAxisParamListWnd();
	bool                       BuildAxisParamListWndHeader();	
	//---------------------------------------------------------------------------------//	
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//
	bool                       ExecDblclkParamListWnd(CThisListCtrl_14 &ListCtrl, int nItem, int nSubItem);
	bool                       ExecDblclkAxisParamListWnd(CThisListCtrl_14 &ListCtrl, int nItem, int nSubItem);	
	//---------------------------------------------------------------------------------//
	bool                       ExecReleaseParamCtrl();
	bool                       ExecUpdateParamByEdit();
	bool                       ExecUpdateParamByCombox();
	//---------------------------------------------------------------------------------//
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMotionCtrlPaneParam)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDblclkAxisParamListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillfocusGridEditWnd();
	afx_msg void OnDblclkParamListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillfocusGridCombo();
	afx_msg void OnSelchangeGridCombo();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOTIONPARAMPANE_H__0B359060_2234_4CBE_8C07_942FFA6DF5DA__INCLUDED_)
