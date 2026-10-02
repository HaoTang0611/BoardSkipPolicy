#if !defined(AFX_LOADBOMWND_H__B48E1C74_7B53_47AE_8590_2F3772E953E8__INCLUDED_)
#define AFX_LOADBOMWND_H__B48E1C74_7B53_47AE_8590_2F3772E953E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LoadBomWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CLoadBomWnd dialog

class CLoadBomWnd : public CBaseDialog
{
// Construction
public:
	CLoadBomWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLoadBomWnd)
	enum { IDD = IDD_LOAD_BOM_WND };
	CComboBox	m_DataTypeCombox;
	CListCtrl	m_ResultListWnd;
	CListCtrl	m_PreviewListWnd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLoadBomWnd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:
	//---------------------------------------------------------------------------------//	
	bool                       GetBomNodeList(std::vector<TComponentNode> &List);
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//		
	//unsigned int               m_StartLine;
	unsigned int               m_CurColIndex;
	CString                    m_Filename;
	CString                    m_ErrorString;
	//---------------------------------------------------------------------------------//	
	std::vector<int>           m_ColTypeList;
	std::vector<TComponentNode> m_BomNodeList;
	//---------------------------------------------------------------------------------//	
	void                       SwitchMultiLanguage();
	bool                       SetMultiLanauage(UINT ID, LPCTSTR Text, bool bCtrlID=true);
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//		
	void                       BuildDataTypeCombox();	
	//---------------------------------------------------------------------------------//	
	CString                    GetDataTypeText(int Type);
	bool                       BuildBomLoadParam(TLoadParam_BOM &LoadParam);
	//---------------------------------------------------------------------------------//	
	bool                       BuildPreviewListWnd();
	bool                       BuildPreviewListWndHeader();
	//---------------------------------------------------------------------------------//	
	bool                       BuildResultListWnd();
	bool                       BuildResultListWndHeader();
	//---------------------------------------------------------------------------------//
	bool                       SaveINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pString, LPCTSTR pfilename, CString &Error);
	bool                       LoadINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pDefault, TCHAR *pString, int StringSize, LPCTSTR pfilename, bool IsCheckLens, CString &Error);	
	//---------------------------------------------------------------------------------//
	CString                    GetSaveDefaultFilename();
	bool                       ExecSaveDefaultBtn();
	bool                       ExecLoadDefaultBtn();	
	//---------------------------------------------------------------------------------//
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLoadBomWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);	
	afx_msg void OnOpenCSVBtn();
	afx_msg void OnColumnclickPreviewListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCloseupDataTypeCombox();
	afx_msg void OnSelchangeDataTypeCombox();
	afx_msg void OnPreviewBtn();	
	afx_msg void OnLoadBtn();
	afx_msg void OnSaveDefaultBtn();
	afx_msg void OnLoadDefaultBtn();	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOADBOMWND_H__B48E1C74_7B53_47AE_8590_2F3772E953E8__INCLUDED_)
