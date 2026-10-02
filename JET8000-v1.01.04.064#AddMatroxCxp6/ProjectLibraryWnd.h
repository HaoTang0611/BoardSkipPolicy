#if !defined(AFX_PROJECTLIBRARYWND_H__0E9B988A_E2EC_4F92_8F54_6C68DC8524CE__INCLUDED_)
#define AFX_PROJECTLIBRARYWND_H__0E9B988A_E2EC_4F92_8F54_6C68DC8524CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProjectLibraryWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CProjectLibraryWnd dialog
//-------------------------------------------------------------------------------------//
class CProjectLibraryWnd : public CBaseDialog
{
// Construction
public:
	CProjectLibraryWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProjectLibraryWnd)
	enum { IDD = IDD_PROJECT_LIBRARY_WND };
	CListCtrl	m_ModelIconListWnd;
	CListCtrl	m_ModelGroupListWnd;
	CListCtrl	m_ModelTypeListWnd;
	CComboBox   m_ModelFrameIndexCombox;
	CComboBox   m_ModelIconSizeCombox;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProjectLibraryWnd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	//---------------------------------------------------------------------------------//
	void                       SetActiveProject(CAOIProject* Ptr);
	CAOIModel*                 GetModelPtr();
	void                       SetModelType(MODEL_TYPE Type);
	void                       SetSearchName(LPCTSTR Name);
	void                       SetBodyRegion(const TREGION4D &Rgn);		
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//
	//---------------------------------------------------------------------------------//
private:
	//---------------------------------------------------------------------------------//
	CString                    m_WndText;
	TREGION4D                  m_BodyRgn;	
	MODEL_TYPE                 m_ModelType;	
	CAOIModel                 *m_ModelPtr;
	CAOIProject               *m_ProjectPtr;
	CString                    m_SearchName;
	CImageList                 m_ModelTypeImageList;
	CImageList                 m_ModelIconImageList;
	bool                       m_StopModelNameListBeSelected;
	bool                       m_StopModelTypeListBeSelected;
	bool                       m_StopModelGroupListBeSelected;	
	//---------------------------------------------------------------------------------//
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//
	CAOIProject*               GetActiveProject();			
	//---------------------------------------------------------------------------------//
	void                       AdjustCtrlWndPosition(int cx, int cy);
	void                       BuildModelIconSizeCombox();
	//---------------------------------------------------------------------------------//
	void                       UpdateWndCaptionText(MODEL_TYPE ModelType);
	UINT                       GetModelTypeIcon(MODEL_TYPE ModelType, bool Small);	
	bool                       ClearModelTypeListCtrl(CListCtrl &ListCtrl);
	bool                       BuildModelTypeListCtrl(CListCtrl &ListCtrl);	
	bool                       SetModelTypeListCtrlItemSlected(MODEL_TYPE ModelType);
	//---------------------------------------------------------------------------------//
	bool                       InitModelGroupListCtrl(CListCtrl &ListCtrl);
	bool                       ClearModelGroupListCtrl(CListCtrl &ListCtrl);
	bool                       BuildModelGroupListCtrl(CListCtrl &ListCtrl, MODEL_TYPE ModelType, bool bBuildIconList);		
	bool                       SetModelGroupListCtrlItemSlected(LPCTSTR  GroupName);
	//---------------------------------------------------------------------------------//
	bool                       ClearModelIconListCtrl(CListCtrl &ListCtrl);
	bool                       UpdateModelIconListCtrl(CListCtrl &ListCtrl);
	bool                       BuildModelIconListCtrl(MODEL_TYPE ModelType, LPCTSTR  GroupName, CListCtrl &ListCtrl);	
	bool                       SetModelIconListCtrlItemSlected(LPCTSTR  ModelName);	
	//---------------------------------------------------------------------------------//
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProjectLibraryWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnItemchangedModelTypeListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedModelGroupListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedModelIconListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkModelIconListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSearchBtn();
	afx_msg void OnSelchangeModelBKImageIndexCombox();
	afx_msg void OnSelchangeModelIconSizeCombox();	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROJECTLIBRARYWND_H__0E9B988A_E2EC_4F92_8F54_6C68DC8524CE__INCLUDED_)
