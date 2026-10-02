#if !defined(AFX_INPUTLISTWND_H__A5B9E370_D479_4A46_9120_50787048246E__INCLUDED_)
#define AFX_INPUTLISTWND_H__A5B9E370_D479_4A46_9120_50787048246E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputListWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
#include "JETListCtrl.h"
//-------------------------------------------------------------------------------------//
#define CThisListCtrl_39     CJETListCtrl//目前使用的列表控制類別	
//-------------------------------------------------------------------------------------//
typedef struct tagListNode
{
	void*       Ptr;	
	CString     Text;	
	CString     Text2;
	DWORD_PTR   Data;
	tagListNode()
	{
		Ptr = NULL;		
		Text = _T("");
		Text2 = _T("");
		Data = 0;
	}
} TListNode, *PListNode;
//-------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CInputListWnd dialog
//-------------------------------------------------------------------------------------//
class CInputListWnd : public CBaseDialog
{
// Construction
public:
	CInputListWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInputListWnd)
	enum { IDD = IDD_INPUT_LIST_WND };
	CString	m_TitleLabel1;
	CComboBox	m_ComboxCtrl;
	CThisListCtrl_39 m_ListCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputListWnd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	//---------------------------------------------------------------------------------//		
	void                       SetWndPos(const POINT &Pos);
	//---------------------------------------------------------------------------------//
	int                        GetSelIndex1() const;
	TListNode&                 GetSelNode();	
	void*                      GetSelPtr() const;
	DWORD_PTR                  GetSelData() const;
	LPCTSTR                    GetSelText() const;	
	//---------------------------------------------------------------------------------//
	void                       SetSelIndex1(int nSel);
	void                       GetDataList(std::vector<TListNode> &DataList);
	void                       SetDropList(const std::vector<TListNode> &DropList);	
	void                       SetParam1(LPCTSTR WndTxt, LPCTSTR Title, void* Default, const std::vector<TListNode> &DataList);
	void                       SetParam1(LPCTSTR WndTxt, LPCTSTR Title, LPCTSTR Default, const std::vector<TListNode> &DataList);
	void                       SetParam1(LPCTSTR WndTxt, LPCTSTR Title, DWORD_PTR Default, const std::vector<TListNode> &DataList);	
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//		
	int                        m_CurSel;
	POINT                      m_WndPos;
	CString                    m_WndText;
	bool                       m_WndMovePos;
	bool                       m_ShowIndexCol;
	bool                       m_StopListBeSelected;
	TListNode                  m_SelData;
	std::vector<TListNode>     m_DataList;
	std::vector<TListNode>     m_DropList;
	//---------------------------------------------------------------------------------//		
	bool                       CheckDropList() const;
	int                        GetDropListSubItem() const;
	//---------------------------------------------------------------------------------//
	bool                       InitListCtrl();
	bool                       BuildListCtrl();
	int                        SearchIndex(void *Ptr, const std::vector<TListNode> &DataList);
	int                        SearchIndex(LPCTSTR Text, const std::vector<TListNode> &DataList);
	int                        SearchIndex(DWORD_PTR Data, const std::vector<TListNode> &DataList);
	//---------------------------------------------------------------------------------//	
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//
	bool                       ExecSelItem(int nItem);
	//---------------------------------------------------------------------------------//
	bool                       ExecUpdateParamByCombox();
	bool                       ShowDropListWnd(CThisListCtrl_39 &ListCtrl, int nItem, int nSubItem);
	//---------------------------------------------------------------------------------//	
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputListWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual void OnOK();
	afx_msg void OnClickListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeParamCombo();
	afx_msg void OnKillfocusParamCombo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTLISTWND_H__A5B9E370_D479_4A46_9120_50787048246E__INCLUDED_)
