#if !defined(AFX_TREECTRL_COMPONENT_H__755C6CC2_4063_4EDE_8BF8_01C639D62B90__INCLUDED_)
#define AFX_TREECTRL_COMPONENT_H__755C6CC2_4063_4EDE_8BF8_01C639D62B90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TreeCtrlComponent.h : header file
//
//-------------------------------------------------------------------------------------//
#include "AOIProject.h"
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CTreeCtrlComponent window
//-------------------------------------------------------------------------------------//
class CTreeCtrlComponent : public CTreeCtrl
{
// Construction
public:
	CTreeCtrlComponent();

// Attributes
public:
	//---------------------------------------------------------------------------------//	
	UINT                       GetPopupMenuID(CPoint Point);
	//---------------------------------------------------------------------------------//	
	void                       SetProjectPtr(CAOIProject *ProjectPtr);
	BOOL                       ClearTreeWnd();
	BOOL                       BuildTreeWnd();
	BOOL                       BuildTreeImageList();
	//---------------------------------------------------------------------------------//	
	BOOL                       UpdateComponentTreeWnd(BOOL SelectedOnly);	
	BOOL                       DoUpdateComponentTreeWndItemState(const bool IsWithWindow);
	BOOL                       MakeSureComponentTreeNodeVisible(int ToLevel);//確定Window可以看得到
	BOOL                       DoUpdateComponentTreeItemText(BOOL IsOnlySelected);
	BOOL                       DoUpdateComponentNodeItemText(HTREEITEM HComponentItem, CAOIComponent *pComponent);//修正零件的文字,
	BOOL                       DoDeleteComponentTreeWndItem();
	BOOL                       DoCollapseComponentTreeWnd();
	BOOL                       DoUpdateComponentTreeClick();
	//---------------------------------------------------------------------------------//	
// Operations
public:
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTreeCtrlComponent)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTreeCtrlComponent();

protected:
	//---------------------------------------------------------------------------------//	
	CImageList                 m_TreeImageList;
	//---------------------------------------------------------------------------------//	
	BOOL                       m_ClickComponentTreeNode;
	BOOL                       m_StopComponentTreeBeClick;
	//---------------------------------------------------------------------------------//	
	CAOIProject               *m_ProjectPtr;
	//---------------------------------------------------------------------------------//	
	void                       CloseProject();
	CAOIProject*               GetActiveProject();
	//---------------------------------------------------------------------------------//	
	BOOL                       RemoveTreeItem(HTREEITEM hItem);
	int                        GetTreeNodeLevel(HTREEITEM hItem);//取得節點所存在的階層, 0為最上層, 1為第二層, 2為第三層
	//---------------------------------------------------------------------------------//		
	BOOL                       InsertProjectNodeToTreeCtrl(HTREEITEM hProjectItem);
	BOOL                       InsertPanelNodeToTreeCtrl(HTREEITEM hPanelItem, CAOIPanel* pPanel);
	BOOL                       InsertFdNodeToTreeCtrl(HTREEITEM hFDItem, CAOIFd *pFd, size_t FDID);//增加一個定位點的節點	
	BOOL                       InsertComponentWindowToTreeCtrl(HTREEITEM HComponentItem, CAOIComponent *pComponent);//增加一個零件的節點	
	//---------------------------------------------------------------------------------//	
	BOOL                       DoSelectComponentTreeWndItem(HTREEITEM hItem);//執行選取到零件樹狀圖的Item
	//---------------------------------------------------------------------------------//	
	HTREEITEM                  GetTreeSelectedItem();
	//---------------------------------------------------------------------------------//	
	// Generated message map functions
protected:
	//{{AFX_MSG(CTreeCtrlComponent)
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);		
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TREECTRL_COMPONENT_H__755C6CC2_4063_4EDE_8BF8_01C639D62B90__INCLUDED_)
