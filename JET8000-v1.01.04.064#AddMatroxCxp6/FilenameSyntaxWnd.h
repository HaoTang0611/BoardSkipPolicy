#if !defined(AFX_FILENAMESYNTAXWND_H__19256774_DA5E_4D6C_9893_59E6E876FBD6__INCLUDED_)
#define AFX_FILENAMESYNTAXWND_H__19256774_DA5E_4D6C_9893_59E6E876FBD6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FilenameSyntaxWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
#include "ParamUni.h"
#include "JETListCtrl.h"
#include "FilenameSyntax.h"
//-------------------------------------------------------------------------------------//
//#define CThisListCtrl_62     CListCtrl//目前使用的列表控制類別	
#define CThisListCtrl_62     CJETListCtrl//目前使用的列表控制類別
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CFilenameSyntaxWnd dialog
//-------------------------------------------------------------------------------------//
class CFilenameSyntaxWnd : public CBaseDialog
{
// Construction
public:
	CFilenameSyntaxWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFilenameSyntaxWnd)
	enum { IDD = IDD_FILENAME_SYNTAX_WND };
	CEdit	m_EditCtrl;	
	CComboBox	m_ComboxCtrl;	
	CComboBox	m_PanelIndexWidthCombox;	
	CComboBox	m_BoardIndexWidthCombox;	
	CComboBox	m_ObjectIndexWidthCombox;	
	CThisListCtrl_62	m_ParamListCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFilenameSyntaxWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
public:
	//---------------------------------------------------------------------------------//
	const CFilenameSyntax&     GetFilenameSyntax() const;
	void                       SetFilenameSyntax(const CFilenameSyntax &Syntax);
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//	
	int                        m_ColIdxAct;
	CParamUni                 *m_ParamActPtr;
	CParamList                 m_ParamList;		
	CFilenameSyntax            m_FilenameSyntax;
	bool                       m_StopParamListBeSelected;	
	//---------------------------------------------------------------------------------//
	int                        GetColIdxAct();
	void                       SetColIdxAct(int Idx);	
	//---------------------------------------------------------------------------------//	
	CParamUni*                 GetActParamUni();
	void                       SetActParamUni(CParamUni *Ptr);	
	//---------------------------------------------------------------------------------//	
	bool                       BuildParamList();
	bool                       BuildParamListWnd();
	CThisListCtrl_62&          GetParamListWndRef();
	bool                       BuildParamListWndHeader();
	//---------------------------------------------------------------------------------//
	bool                       BuildIndexWidthCombox(CComboBox &Combox);
	//---------------------------------------------------------------------------------//
	bool                       UpdateKernalToUI();
	bool                       UpdateUIToKernal();
	bool                       UpdateFilenameText(bool bVerify=false);			
	CString                    GetSyntaxText(FILE_NAME_SYNTAX_MODE Mode);
	bool                       CreateSyntaxode(CFilenameSyntaxNode &SyntaxNode, bool bAckFolder);
	//---------------------------------------------------------------------------------//	
	void                       SwitchMultiLanguage();
	bool                       SetMultiLanauage(UINT ID, LPCTSTR Text, bool bCtrlID=true);
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//	
	bool                       HideCtrlBtn();
	bool                       ExecReleaseParamCtrl();	
	void                       SetDescriptionText(const CParamUni *Ptr);	
	bool                       ExecItemchangedParamListWnd(CThisListCtrl_62 &ListCtrl, int nItem);
	bool                       ExecDblclkParamListWnd(CThisListCtrl_62 &ListCtrl, int nItem, int nSubItem);
	bool                       ExecDblclkParamListWnd_Mode(CThisListCtrl_62 &ListCtrl, int nItem, int nSubItem);
	bool                       ExecDblclkParamListWnd_Folder(CThisListCtrl_62 &ListCtrl, int nItem, int nSubItem);
	bool                       ExecUpdateParamByEdit();
	bool                       ExecUpdateParamByCombox();	
	bool                       ExecUpdateParamByCombox_Mode();	
	bool                       ExecUpdateParamByCombox_Folder();	
	//---------------------------------------------------------------------------------//	
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFilenameSyntaxWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnDestroy();	
	virtual void OnOK();
	afx_msg void OnItemchangedParamListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkParamListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillfocusParamEdit();
	afx_msg void OnSelchangeParamCombo();
	afx_msg void OnKillfocusParamCombo();
	afx_msg void OnAddNodeBtn();
	afx_msg void OnInsertNodeBtn();
	afx_msg void OnDelNodeBtn();
	afx_msg void OnClearBtn();	
	afx_msg void OnResizeBtn();
	afx_msg void OnDelUnusedBtn();
	afx_msg void OnSaveFileBtn();
	afx_msg void OnLoadFileBtn();		
	afx_msg void OnUpdateParamBtn();	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILENAMESYNTAXWND_H__19256774_DA5E_4D6C_9893_59E6E876FBD6__INCLUDED_)
