#if !defined(AFX_REMOTEPARAMWND_H__B44F604D_4D94_436C_A5D5_9539D9A9C915__INCLUDED_)
#define AFX_REMOTEPARAMWND_H__B44F604D_4D94_436C_A5D5_9539D9A9C915__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RemoteParamWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
#include "ParamUni.h"
#include "JETListCtrl.h"
//-------------------------------------------------------------------------------------//
#define CThisListCtrl_42     CJETListCtrl//ヘ玡ㄏノ北摸	
/////////////////////////////////////////////////////////////////////////////
// CRemoteParamWnd dialog
//-------------------------------------------------------------------------------------//
class CRemoteParamWnd : public CBaseDialog
{
// Construction
public:
	CRemoteParamWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRemoteParamWnd)
	enum { IDD = IDD_REMOTE_PARAM_WND };
	CEdit	m_EditCtrl;
	CButton	m_FoderBtn;
	CComboBox	m_ComboxCtrl;	
	CThisListCtrl_42	m_ParamListCtrl_01;
	CThisListCtrl_42	m_ParamListCtrl_02;
	CThisListCtrl_42	m_ParamListCtrl_03;
	CThisListCtrl_42	m_ParamListCtrl_04;
	CThisListCtrl_42	m_ParamListCtrl_05;
	CThisListCtrl_42	m_ParamListCtrl_06;
	CThisListCtrl_42	m_ParamListCtrl_07;
	CThisListCtrl_42	m_ParamListCtrl_08;
	CThisListCtrl_42	m_ParamListCtrl_09;
	CThisListCtrl_42	m_ParamListCtrl_10;
	CThisListCtrl_42	m_ParamListCtrl_11;
	CThisListCtrl_42	m_ParamListCtrl_12;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRemoteParamWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
public:
	//---------------------------------------------------------------------------------//			
	void                       GetRemoteParamList(std::vector<TRemoteParam> &RemoteParamList);
	void                       SetRemoteParamList(const std::vector<TRemoteParam> &RemoteParamList);
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//			
	CParamUni                 *m_ParamActPtr;	
	std::vector<CParamList>    m_ParamListSet;
	std::vector<TRemoteParam>  m_RemoteParamList;
	bool                       m_StopParamListBeSelected;	
	//---------------------------------------------------------------------------------//	
	int                        MapListCtrlIDToParamListIndex(UINT ListID);//ListID锣ΘListま计		
	CParamUni*                 GetListCtrlParamUniPtr(UINT ListID, size_t Index);//眔把计夹
	//---------------------------------------------------------------------------------//
	CParamUni*                 GetActParamUni();
	void                       SetActParamUni(CParamUni *Ptr);	
	//---------------------------------------------------------------------------------//	
	bool                       BuildParamList();
	bool                       BuildParamListKernel(const TRemoteParam &Param, CParamList &ParamList);
	bool                       ClearParamListWnd();
	bool                       ClearParamListWndKernel(CThisListCtrl_42 &ListCtrl);
	bool                       BuildParamListWnd();
	bool                       BuildParamListWndKernel(CParamList &ParamList, CThisListCtrl_42 &ListCtrl);
	bool                       BuildParamListWndHeader();
	bool                       BuildParamListWndHeaderKernel(CThisListCtrl_42 &ListCtrl);
	//---------------------------------------------------------------------------------//	
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//
	bool                       SetRemoteParamByParamUni(CParamUni *ParamUniPtr, std::vector<TRemoteParam> &ParamList, LPCTSTR String);
	//---------------------------------------------------------------------------------//	
	bool                       ExecReleaseCtrlWnd();
	bool                       ExecReleaseParamCtrl();	
	//---------------------------------------------------------------------------------//
	bool                       ExecItemchangedParamListWnd(CThisListCtrl_42 &ListCtrl, int nItem);
	bool                       ExecDblclkParamListWnd(CThisListCtrl_42 &ListCtrl, int nItem, int nSubItem);
	bool                       ExecUpdateParamByBtn();
	bool                       ExecUpdateParamByEdit();
	bool                       ExecUpdateParamByCombox();
	//---------------------------------------------------------------------------------//	
	bool                       ChangeActIndex(int index);	
	bool                       ExecDeleteRemoteParam(size_t index);
	//---------------------------------------------------------------------------------//
	void                       UpdateParamUI();
	//---------------------------------------------------------------------------------//
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRemoteParamWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnNewBtn();
	afx_msg void OnSaveBtn();
	afx_msg void OnLoadBtn();
	afx_msg void OnClearBtn();
	afx_msg void OnFolderBtn();
	afx_msg void OnKillfocusParamEdit();
	afx_msg void OnSelchangeParamCombo();
	afx_msg void OnKillfocusParamCombo();
	afx_msg void OnItemchangedListWnd01(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListWnd01(NMHDR* pNMHDR, LRESULT* pResult);	
	afx_msg void OnUseChk01();
	afx_msg void OnDelBtn01();
	afx_msg void OnItemchangedListWnd02(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListWnd02(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUseChk02();	
	afx_msg void OnDelBtn02();
	afx_msg void OnItemchangedListWnd03(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListWnd03(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUseChk03();
	afx_msg void OnDelBtn03();
	afx_msg void OnItemchangedListWnd04(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListWnd04(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUseChk04();
	afx_msg void OnDelBtn04();
	afx_msg void OnItemchangedListWnd05(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListWnd05(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUseChk05();
	afx_msg void OnDelBtn05();
	afx_msg void OnItemchangedListWnd06(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListWnd06(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUseChk06();
	afx_msg void OnDelBtn06();
	afx_msg void OnItemchangedListWnd07(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListWnd07(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUseChk07();
	afx_msg void OnDelBtn07();
	afx_msg void OnItemchangedListWnd08(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListWnd08(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUseChk08();
	afx_msg void OnDelBtn08();	
	afx_msg void OnItemchangedListWnd09(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListWnd09(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUseChk09();
	afx_msg void OnDelBtn09();	
	afx_msg void OnItemchangedListWnd10(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListWnd10(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUseChk10();
	afx_msg void OnDelBtn10();	
	afx_msg void OnItemchangedListWnd11(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListWnd11(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUseChk11();
	afx_msg void OnDelBtn11();	
	afx_msg void OnItemchangedListWnd12(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListWnd12(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUseChk12();
	afx_msg void OnDelBtn12();	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REMOTEPARAMWND_H__B44F604D_4D94_436C_A5D5_9539D9A9C915__INCLUDED_)
