#if !defined(AFX_WNDDEFECTITEMWND_H__BD23EBB6_E9DE_492C_8840_490AD3E70665__INCLUDED_)
#define AFX_WNDDEFECTITEMWND_H__BD23EBB6_E9DE_492C_8840_490AD3E70665__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WndDefectItemWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
#include "ParamUni.h"
#include "JETListCtrl.h"
#include "WndDefectItem.h"
//-------------------------------------------------------------------------------------//
#define CThisListCtrl_71     CJETListCtrl//目前使用的列表控制類別	
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CWndDefectItemWnd dialog
//-------------------------------------------------------------------------------------//
class CWndDefectItemWnd : public CBaseDialog
{
// Construction
public:
	CWndDefectItemWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWndDefectItemWnd)
	enum { IDD = IDD_WND_DEFECT_ITEM_WND };
	CEdit	m_EditCtrl;	
	CComboBox	m_ComboxCtrl;	
	CThisListCtrl_71	m_ParamListCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWndDefectItemWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

public:
	//---------------------------------------------------------------------------------//	
	const CWndDefectItem&     GetWndDefectItem() const;
	void                      SetWndDefectItem(const CWndDefectItem &DefectItem); 	
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//					
	CParamUni                 *m_ParamActPtr;	
	CParamList                 m_ParamList;	
	CWndDefectItem             m_WndDefectItem;
	bool                       m_StopParamListBeSelected;	
	//---------------------------------------------------------------------------------//	
	CParamUni*                 GetActParamUni();
	void                       SetActParamUni(CParamUni *Ptr);	
	//---------------------------------------------------------------------------------//
	void                       SwitchMultiLanguage();
	bool                       SetMultiLanauage(UINT ID, LPCTSTR Text, bool bCtrlID=true);
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//	
	bool                       BuildParamList();
	bool                       BuildParamListWnd();
	bool                       BuildParamListWndHeader();
	//---------------------------------------------------------------------------------//	
	void                       SetDescriptionText(const CParamUni *Ptr);
	//---------------------------------------------------------------------------------//
	bool                       ExecReleaseParamCtrl();
	//---------------------------------------------------------------------------------//
	bool                       ExecItemchangedParamListWnd(CThisListCtrl_71 &ListCtrl, int nItem);
	bool                       ExecDblclkParamListWnd(CThisListCtrl_71 &ListCtrl, int nItem, int nSubItem);
	bool                       ExecUpdateParamByEdit();
	bool                       ExecUpdateParamByCombox();
	//---------------------------------------------------------------------------------//	
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWndDefectItemWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnDblclkParamListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillfocusParamListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedParamListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillfocusParamEdit();
	afx_msg void OnSelchangeParamCombo();
	afx_msg void OnKillfocusParamCombo();
	afx_msg void OnEnableAllBtn();
	afx_msg void OnDisableAllBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_WNDDEFECTITEMWND_H__BD23EBB6_E9DE_492C_8840_490AD3E70665__INCLUDED_)
