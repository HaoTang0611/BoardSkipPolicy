#if !defined(AFX_MODELGROUPWND_H__834A884B_6AFD_46C7_960A_8F6034663094__INCLUDED_)
#define AFX_MODELGROUPWND_H__834A884B_6AFD_46C7_960A_8F6034663094__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModelGroupWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
#include "JETListCtrl.h"
//-------------------------------------------------------------------------------------//
#define CThisListCtrl_38     CJETListCtrl//目前使用的列表控制類別	
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CModelGroupWnd dialog

class CModelGroupWnd : public CBaseDialog
{
// Construction
public:
	CModelGroupWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CModelGroupWnd)
	enum { IDD = IDD_MODEL_GROUP_WND };
	CComboBox	m_IconSizeCombox;
	CThisListCtrl_38	m_GroupListWnd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModelGroupWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

public:
	//---------------------------------------------------------------------------------//
	MODEL_TYPE                 GetDefaultModelType() const;
	void                       SetDefaultModelType(MODEL_TYPE Type);	
	//---------------------------------------------------------------------------------//
	LPCTSTR                    GetDefaultGroupName() const;
	void                       SetDefaultGroupName(LPCTSTR  Name);	
	//---------------------------------------------------------------------------------//	
	LPCTSTR                    GetDefaultImageName() const;
	void                       SetDefaultImageName(LPCTSTR  Name);	
	//---------------------------------------------------------------------------------//	
	CString                    GetGroupNameSelected() const;
	//---------------------------------------------------------------------------------//
	CString                    GetImageNameSelected() const;
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//
	int                        m_ImageNameIndex;
	MODEL_TYPE                 m_DefaultModelType;
	CString                    m_DefaultGroupName;
	CString                    m_DefaultImageName;
	std::vector<CString>       m_ImageNameList;
	CImageList                 m_GroupImageList;
	bool                       m_StopGroupListBeSelected;
	//---------------------------------------------------------------------------------//
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//
	void                       BuildIconSizeCombox();
	//---------------------------------------------------------------------------------//
	bool                       BuildGroupList();
	bool                       ClearGroupListCtrl(CThisListCtrl_38 &ListCtrl);
	bool                       BuildGroupListCtrl(CThisListCtrl_38 &ListCtrl);
	//---------------------------------------------------------------------------------//
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CModelGroupWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnClickGroupListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedGroupListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeIconSizeCombo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODELGROUPWND_H__834A884B_6AFD_46C7_960A_8F6034663094__INCLUDED_)
