#if !defined(AFX_PROJECTGROUPCONFIGWND_H__27CC652E_9367_422E_AFCD_FC7FD46E84FA__INCLUDED_)
#define AFX_PROJECTGROUPCONFIGWND_H__27CC652E_9367_422E_AFCD_FC7FD46E84FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProjectGroupConfigWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "ParamUni.h"
#include "JETListCtrl.h"
//-------------------------------------------------------------------------------------//
//#define CThisListCtrl_53     CListCtrl//目前使用的列表控制類別	
#define CThisListCtrl_53     CJETListCtrl//目前使用的列表控制類別
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CProjectGroupConfigWnd dialog
//-------------------------------------------------------------------------------------//
class CProjectGroupConfigWnd : public CDialog
{
// Construction
public:
	CProjectGroupConfigWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProjectGroupConfigWnd)
	enum { IDD = IDD_PROJECT_GROUP_CONFIG_WND };
	CListBox	m_WndListBox;
	CListBox	m_NodeListBox;
	CComboBox   m_GroupModeCombox;
	CEdit   m_GroupParamEdit;
	CComboBox   m_GroupParamCombox;
	CThisListCtrl_53	m_GroupListWnd;
	CThisListCtrl_53	m_NodeParamListWnd;
	CThisListCtrl_53	m_GroupParamListWnd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProjectGroupConfigWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
public:
	//---------------------------------------------------------------------------------//		
	void                       SetProjectPtr(CAOIProject *Ptr);
	//---------------------------------------------------------------------------------//		
protected:
	//---------------------------------------------------------------------------------//		
	int                        m_NodeFromeID;
	CString                    m_ErrorString;
	CParamUni                 *m_ParamActPtr;
	CParamList                 m_NodeParamList;
	CParamList                 m_GroupParamList;
	TPartGroupNode            *m_PartGropuNodePtr;//選中的
	//---------------------------------------------------------------------------------//		
	bool                       m_StopGroupListBeSelected;
	bool                       m_StopNodeParamListBeSelected;
	bool                       m_StopGroupParamListBeSelected;
	//---------------------------------------------------------------------------------//		
	CAOIProject               *m_ProjectPtr;
	std::vector<CAOIPartGroup> m_PartGroupList;
	std::vector<CAOIPartGroup> m_PartGroupListBackup;
	CAOIPartGroup*             GetActivePartGroup();
	CAOIProject*               GetActiveProject();
	//---------------------------------------------------------------------------------//
	void                       SetPartGropuNodePtr(TPartGroupNode *Ptr);
	bool                       CheckPartGropuNodePtr(const TPartGroupNode *Ptr);
	//---------------------------------------------------------------------------------//		
	std::vector<CAOIPartGroup>& GetPartGroupList();
	void                       ClearPartGroupList();
	void                       ClonePartGroupList();
	bool                       CheckPartGroupList();//確認專案群組列表
	void                       UpdatePartGroupList();//更新(取得)專案群組列表	
	int                        GetPartGroupFreeGroupID();//取得零件群組群組編號
	void                       ReadPartGroupListResult();
	void                       ReadPartGroupListResult(std::vector<CAOIPartGroup> &GroupList);
	void                       RestoreProjectPartGroupList();//恢復專案群組列表	
	void                       UpdateToProjectPartGroupList();
	void                       UpdateToProjectPartGroupList(const std::vector<CAOIPartGroup> &GroupList);	
	//---------------------------------------------------------------------------------//
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//
	bool                       GetLockUIWnd() const;//取得是否鎖住UIWnd
	//---------------------------------------------------------------------------------//
	CParamUni*                 GetActParamUni();
	void                       SetActParamUni(CParamUni *Ptr);	
	//---------------------------------------------------------------------------------//
	CListBox&	               GetWndListBox();
	CListBox&	               GetNodeListBox();
	CThisListCtrl_53&          GetGroupListWnd();
	CThisListCtrl_53&          GetNodeParamListWnd();
	CThisListCtrl_53&          GetGroupParamListWnd();
	//---------------------------------------------------------------------------------//
	int                        GetGroupListWndItem();
	bool                       BuildGroupListWndHeader();
	bool                       BuildGroupListWnd();
	bool                       UpdateGroupListWnd();
	bool                       UpdateGroupListWnd(PART_GROUP_PARAM_ID ParamID);	
	//---------------------------------------------------------------------------------//
	int                        GetNodeParamListWndItem();
	bool                       BuildNodeParamListWndHeader();
	bool                       BuildNodeParamList(CAOIPartGroup *GroupPtr, TPartGroupNode &GroupNode, int FromID);	
	bool                       ClearNodeParamListWnd();
	bool                       BuildNodeParamListWnd(CAOIPartGroup *GroupPtr, TPartGroupNode &GroupNode, int FromID);		
	//---------------------------------------------------------------------------------//
	int                        GetGroupParamListWndItem();
	bool                       BuildGroupParamListWndHeader();
	bool                       BuildGroupParamList();	
	bool                       ClearGroupParamListWnd();
	bool                       BuildGroupParamListWnd();		
	//---------------------------------------------------------------------------------//
	bool                       ExecReleaseParamCtrl();
	void                       SetDescriptionText(const CParamUni *Ptr);
	bool                       ExecItemchangedParamListWnd(CParamList &ParamList, CThisListCtrl_53 &ListCtrl, int nItem);
	bool                       ExecDblclkParamListWnd(CParamList &ParamList, CThisListCtrl_53 &ListCtrl, int nItem, int nSubItem);
	bool                       ExecUpdateParamByEdit();
	bool                       ExecUpdateParamByCombox();
	//---------------------------------------------------------------------------------//
	bool                       ClearListBox(CListBox &ListBox);	
	bool                       BuildWndListBox(TPartGroupNode &GroupNode, int FromID);	
	//---------------------------------------------------------------------------------//
	bool                       EnableNode1(BOOL bEnable);
	bool                       EnableNode2(BOOL bEnable);
	bool                       EnableNodeList(BOOL bEnable);
	//---------------------------------------------------------------------------------//
	bool                       ExecPartGroupAddBtn();	
	bool                       ExecPartGroupDelBtn();	
	bool                       ExecPartGroupCopyBtn();	
	bool                       ExecPartGroupClearBtn();	
	bool                       ExecPartGroupTestBtn();	
	bool                       ExecPartGroupUpdateBtn();
	bool                       ExecPartGroupDelOthersBtn();		
	bool                       UpdateGroupParamUI(CAOIPartGroup *Ptr);	
	bool                       GetPartGroupNodeName(const TPartGroupNode &GroupNode, bool bAddRes, CString &NodeName);
	//---------------------------------------------------------------------------------//
	TPartGroupNode*            GetSelPartGroupNodePtr();
	bool                       ExecPartGroupNodeAddBtn(TPartGroupNode &GroupNode);	
	bool                       ExecSelPartGroupNode(CAOIPartGroup *GroupPtr, TPartGroupNode &GroupNode, int FromID);	
	bool                       ExecPartGroupNodeAddListBtn(std::vector<TPartGroupNode> &GroupNodeList);
	bool                       ExecSelChangeListBox();	
	//---------------------------------------------------------------------------------//	
// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CProjectGroupConfigWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnPaint();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnItemchangedGroupListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkGroupListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedNodeParamListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkNodeParamListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedGroupParamListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkGroupParamListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillfocusGroupParamEdit();
	afx_msg void OnSelchangeGroupParamCombo();
	afx_msg void OnKillfocusGroupParamCombo();
	afx_msg void OnGroupAddBtn();
	afx_msg void OnGroupCopyBtn();
	afx_msg void OnGroupDeleteBtn();
	afx_msg void OnGroupClearBtn();	
	afx_msg void OnGroupTestBtn();
	afx_msg void OnGroupUpdateBtn();
	afx_msg void OnGroupDelOthersBtn();
	afx_msg void OnGroupNodeAddBtn1();
	afx_msg void OnGroupNodeDeleteBtn1();
	afx_msg void OnSetfocusNodeAddEdit1();
	afx_msg void OnGroupNodeAddBtn2();
	afx_msg void OnGroupNodeDeleteBtn2();
	afx_msg void OnSetfocusNodeAddEdit2();
	afx_msg void OnGroupNodeListAddBtn();
	afx_msg void OnGroupNodeListDeleteBtn();
	afx_msg void OnGroupNodeListClearBtn();	
	afx_msg void OnSelchangeNodeListBox();
	afx_msg void OnGroupNodeListArrangeBtn();
	afx_msg void OnGroupNodeListSortXBtn();
	afx_msg void OnGroupNodeListSortYBtn();
	afx_msg void OnGroupWndSetBtn();
	afx_msg void OnGroupWndSetAllBtn();	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
extern CProjectGroupConfigWnd ProjectGroupConfigWnd;
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROJECTGROUPCONFIGWND_H__27CC652E_9367_422E_AFCD_FC7FD46E84FA__INCLUDED_)
