#if !defined(AFX_LOADCADXYWND_H__E120C01E_CAD2_47E3_BAFB_4B4A31F0D15D__INCLUDED_)
#define AFX_LOADCADXYWND_H__E120C01E_CAD2_47E3_BAFB_4B4A31F0D15D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LoadCadxyWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
#include <vector>
#include "AOIProject.h"
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CLoadCadxyWnd dialog
//-------------------------------------------------------------------------------------//
class CLoadCadxyWnd : public CBaseDialog
{
// Construction
public:
	CLoadCadxyWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLoadCadxyWnd)
	enum { IDD = IDD_LOAD_CADXY_WND };
	CComboBox	m_TextFilterCombox;
	CComboBox	m_DataTypeCombox;
	CListCtrl	m_ResultListWnd;
	CListCtrl	m_PreviewListWnd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLoadCadxyWnd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	//---------------------------------------------------------------------------------//	
	bool                       SetProjectPtr(CAOIProject *Ptr);
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//	
	CAOIProject               *m_ProjectPtr;
	//---------------------------------------------------------------------------------//	
	unsigned int               m_StartLine;
	unsigned int               m_CurColIndex;
	CString                    m_Filename;
	CString                    m_ErrorString;
	//---------------------------------------------------------------------------------//	
	std::vector<int>           m_ColTypeList;
	std::vector<char>          m_DelimiterList;		
	std::vector<wchar_t>       m_wDelimiterList;		
	//---------------------------------------------------------------------------------//	
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//	
	double                     GetUnitFactor();
	//---------------------------------------------------------------------------------//
	void                       BuildDataTypeCombox();	
	char                       GetTextFilter();	
	//---------------------------------------------------------------------------------//
	bool                       BuildDelimiterList();
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
	bool                       SaveLoadCADXYResult();	
	bool                       CheckCompoentData(std::vector<CString> &NGList);//確定零件資料是否正確
	bool                       SaveNGComponentList(const std::vector<CString> &NGList);
	bool                       LoadCADXYFile(LPCTSTR pfilename, size_t StartLine, const std::vector<int> &ColumnDef, const std::vector<char> &Delimiters, const std::vector<wchar_t> &wDelimiters, char TextFilter, const double Factor, double ComW, double ComH, char BoardName[]); //將CADXY檔案讀入資料陣列(CADX_CS)
	//---------------------------------------------------------------------------------//
	CString                    GetSaveDefaultFilename();
	bool                       ExecSaveDefaultBtn();
	bool                       ExecLoadDefaultBtn();	
	bool                       ExecMatchAliasName();//合併料號別名
	//---------------------------------------------------------------------------------//	
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLoadCadxyWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnOpenFileBtn();
	afx_msg void OnOpenCSVBtn();
	afx_msg void OnOpenASCBtn();
	afx_msg void OnColumnclickPreviewListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCloseupDataTypeCombox();
	afx_msg void OnSelchangeDataTypeCombox();
	afx_msg void OnPreviewBtn();
	afx_msg void OnLoadBtn();
	afx_msg void OnUnitMMRadio();
	afx_msg void OnUnitInchRadio();
	afx_msg void OnUnitDefineRadio();
	afx_msg void OnSaveDefaultBtn();
	afx_msg void OnLoadDefaultBtn();	
	afx_msg void OnDelimiterTab();
	afx_msg void OnDelimiterComma();
	afx_msg void OnDelimiterSpace();
	afx_msg void OnDelimiterSemicolon();
	afx_msg void OnDelimiterOther();
	afx_msg void OnMatchAliasBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOADCADXYWND_H__E120C01E_CAD2_47E3_BAFB_4B4A31F0D15D__INCLUDED_)
