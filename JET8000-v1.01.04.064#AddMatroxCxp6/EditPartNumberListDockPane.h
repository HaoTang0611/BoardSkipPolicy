#pragma once

//-------------------------------------------------------------------------------------//
#include "JETListCtrl.h"
#include "JETMFCListCtrl.h"
#include "PageSplitterWnd.h"
#include "EditPartNumberListPaneBar.h"
//-------------------------------------------------------------------------------------//
#define CThisListCtrl_11     CJETMFCListCtrl//目前使用的列表控制類別	
//-------------------------------------------------------------------------------------//
// CEditPartNumberListDockPane
//-------------------------------------------------------------------------------------//
class CEditPartNumberListDockPane : public CDockablePane
{
	DECLARE_DYNAMIC(CEditPartNumberListDockPane)
public:
	enum SUB_LIST_MODE
	{
		SUB_LIST_COMPONENT = 1,
		SUB_LIST_WINDOW    = 2
	};
//-------------------------------------------------------------------------------------//
public:
	CEditPartNumberListDockPane();
	virtual ~CEditPartNumberListDockPane();


protected:
	//---------------------------------------------------------------------------------//
	COLORREF                   m_clrOK;
	COLORREF                   m_clrNG;
	COLORREF                   m_clrBypass;
	COLORREF                   m_clrUnTest;	
	//---------------------------------------------------------------------------------//
	CAOIProject               *m_ProjectPtr;
	DISTRICT_ID                m_DistrictID;
	unsigned int               m_ComponentIndex;		
	//---------------------------------------------------------------------------------//	
	CEditPartNumberListPaneBar m_wndPaneBar;
	CPageSplitterWnd           m_wndSplitter;
	//---------------------------------------------------------------------------------//
	bool                       m_MoveToComponent;
	bool                       m_SwitchPartNumberBtn;
	CString                    m_PartNumberName;
	std::vector<CString>       m_PartNumberList;
	CThisListCtrl_11           m_wndPartNumberListCtrl;
	BOOL                       m_StopPartNumberListBeSelected;
	//---------------------------------------------------------------------------------//	
	SUB_LIST_MODE              m_SubCtrlMode;
	CThisListCtrl_11           m_wndSubListCtrl;
	CString                    m_ComponentPartNumber;
	std::vector<CSortObj>      m_ComponentNodeList;
	BOOL                       m_StopSubListBeSelected;
	//---------------------------------------------------------------------------------//		
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	BOOL                       InitListCtrl(CThisListCtrl_11 &ListCtrl);		
	//---------------------------------------------------------------------------------//		
	COLORREF                   GetResultColor(RESULT_ID ResultID);
	//---------------------------------------------------------------------------------//
	void                       CloseProject();	
	CAOIProject*               GetActiveProject();	
	DISTRICT_ID                GetActiveDistrictID();
	//---------------------------------------------------------------------------------//		
	BOOL                       InitPartNumberListCtrl(CThisListCtrl_11 &ListCtrl);
	BOOL                       BuildPartNumberListCtrl(CThisListCtrl_11 &ListCtrl);
	BOOL                       UpdatePartNumberListCtrl(CThisListCtrl_11 &ListCtrl, bool bForce=true);
	BOOL                       ClearPartNumberListCtrl(CThisListCtrl_11 &ListCtrl);
	BOOL                       RemovePartNumberListItem(CThisListCtrl_11 &ListCtrl);
	bool                       ExecPartNumberListMenu(CPoint point);
	//---------------------------------------------------------------------------------//
	SUB_LIST_MODE              GetSubListCtrlMode();
	BOOL                       InitSubListCtrl(CThisListCtrl_11 &ListCtrl, SUB_LIST_MODE Mode);	
	BOOL                       BuildSubListCtrl(CThisListCtrl_11 &ListCtrl, LPCTSTR strPartNumber, bool bForce=true);	
	BOOL                       BuildWindowCtrl(CThisListCtrl_11 &ListCtrl, LPCTSTR strPartNumber);	
	BOOL                       BuildComponentCtrl(CThisListCtrl_11 &ListCtrl, LPCTSTR strPartNumber, bool bForce=true);	
	BOOL                       ClearSubListCtrl(CThisListCtrl_11 &ListCtrl);
	BOOL                       ShowSubListSelected(CThisListCtrl_11 &ListCtrl);
	BOOL                       ShowWindowListSelected(CThisListCtrl_11 &ListCtrl);
	BOOL                       ShowComponentListSelected(CThisListCtrl_11 &ListCtrl);
	BOOL                       UpdateComponentListState(CThisListCtrl_11 &ListCtrl);
	BOOL                       ExecComponentListMenu(CPoint point);
	bool                       UpdateSubListCtrlTitle(CThisListCtrl_11 &ListCtrl, size_t nItem);
	//---------------------------------------------------------------------------------//
	void                       ClearComponentNodeList();
	bool                       BuildNextComponentCtrl();	
	void                       BuildComponentNodeList(const std::vector<CSortObj> &SortList);
	//---------------------------------------------------------------------------------//
	void                       ExecWindowModeChk();
	//---------------------------------------------------------------------------------//
	int                        FindItemPreious_Any(CThisListCtrl_11 &ListCtrl);
	int                        FindItemPreious_Set(CThisListCtrl_11 &ListCtrl);
	int                        FindItemPreious_UnSet(CThisListCtrl_11 &ListCtrl);

	int                        FindItemNext_Any(CThisListCtrl_11 &ListCtrl);
	int                        FindItemNext_Set(CThisListCtrl_11 &ListCtrl);
	int                        FindItemNext_UnSet(CThisListCtrl_11 &ListCtrl);
	//---------------------------------------------------------------------------------//
	bool                       ExecComponentRotation(double Angle);
	//---------------------------------------------------------------------------------//
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);	
	afx_msg void OnClickPartNumberListCtrl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDbclickPartNumberListCtrl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedPartNumberListCtrl(NMHDR* pNMHDR, LRESULT* pResult);	
	afx_msg void OnClickSubListCtrl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDbclickSubListCtrl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedSubListCtrl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndScrollSubListCtrl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSwitchPreiousBtn();
	afx_msg void OnSwitchNextBtn();
	afx_msg void OnPartNumberBypass();
	afx_msg void OnPartNumberSearch();	
	afx_msg void OnPartNumberRename();
	afx_msg void OnPartNumberDelete();
	afx_msg void OnPartNumberSelectAll();
	afx_msg void OnComponentOffset();
	afx_msg void OnComponentSetPos();
	afx_msg void OnComponentMirrorPosX();
	afx_msg void OnComponentMirrorPosY();
	afx_msg void OnComponentRotate090();
	afx_msg void OnComponentRotate180();
	afx_msg void OnComponentRotate270();
	afx_msg void OnComponentRotateAny();
	afx_msg void OnComponentRotateReverse();	
	afx_msg void OnComponentRename();
	afx_msg void OnComponentDelete();
	afx_msg void OnComponentSetNozzleName();
	afx_msg void OnComponentSetPartNumber();
	afx_msg void OnComponentSearch();
	afx_msg void OnComponentSelectAll();
	afx_msg void OnComponentBypass();
	afx_msg void OnComponentXBoardUnit();
	afx_msg void OnComponentModelIsolated();
	afx_msg void OnComponentRestoreCadPos();
	afx_msg void OnComponentBypass3D();	
	afx_msg void OnComponentMaskBaseSetColorIndex();	
	afx_msg void OnComponentSpaceNoiseFilter();
	afx_msg void OnComponentMaskExtendSizeBody();
	afx_msg void OnComponentEnableAlarmAOI();	
	afx_msg void OnComponentGroupID();
	afx_msg void OnComponentGroupOrg();		
	afx_msg void OnComponentToFieldPos();	
	afx_msg void OnComponentEnableSelfField();	
	afx_msg void OnComponentCloneNewModel();	
	afx_msg void OnComponentChangeBoard();
	afx_msg void OnComponentLocalBasePlaneID();
	afx_msg void OnComponentDataModelParam();
	afx_msg void OnComponentSaveWndList();
	afx_msg void OnComponentFeedbackResultPos();
	DECLARE_MESSAGE_MAP()
	//---------------------------------------------------------------------------------//
	
};
//-------------------------------------------------------------------------------------//