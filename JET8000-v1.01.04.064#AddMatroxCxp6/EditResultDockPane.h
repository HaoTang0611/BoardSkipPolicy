#pragma once


// CEditResultDockPane
//-------------------------------------------------------------------------------------//
#include "PaneTabCtrl.h"
#include "JETListCtrl.h"
#include "PageSplitterWnd.h"
#include "EditResultPaneBar.h"
//-------------------------------------------------------------------------------------//
#define CThisListCtrl_12     CJETListCtrl//目前使用的列表控制類別	
//#define CThisListCtrl_12     CBasicListCtrl;//目前使用的列表控制類別	
//-------------------------------------------------------------------------------------//
class CEditResultDockPane : public CDockablePane
{
	DECLARE_DYNAMIC(CEditResultDockPane)

public:
	CEditResultDockPane();
	virtual ~CEditResultDockPane();

protected:
	//---------------------------------------------------------------------------------//
	CString                    m_SelText;
	//---------------------------------------------------------------------------------//
	COLORREF                   m_clrNG;
	COLORREF                   m_clrOK;
	COLORREF                   m_clrBypass;
	COLORREF                   m_clrUnTest;
	//---------------------------------------------------------------------------------//
	CAOIProject*               m_ProjectPtr;
	CAOIComponent*             m_ComponentPtrAct;
	bool                       m_MoveToComponent;
	//---------------------------------------------------------------------------------//
	CPageSplitterWnd           m_wndSplitter;
	CEditResultPaneBar         m_wndPaneBar;
	//---------------------------------------------------------------------------------//
	std::vector<CAOIComponent*> m_DefectComponentList;
	//---------------------------------------------------------------------------------//	
	CThisListCtrl_12           m_wndDefectListCtrl;	
	CThisListCtrl_12           m_wndDefectWndListCtrl;//瑕疵框列表
	CThisListCtrl_12           m_wndDefectGroupListCtrl;//瑕疵群組列表
	bool                       m_StopDefectListBeSelected;
	bool                       m_StopDefectWndListBeSelected;
	bool                       m_StopDefectGroupListBeSelected;
	//---------------------------------------------------------------------------------//
	void                       OnChangeVisualStyle();
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//	
	void                       CloseProject();
	CAOIProject*               GetActiveProject();	
	CAOIComponent*             GetActiveComponent();
	void                       SetActiveComponent(CAOIComponent* Ptr);
	//---------------------------------------------------------------------------------//	
	bool                       ClearDefectListWnd();
	COLORREF                   GetResultColor(RESULT_ID ResultID);
	//---------------------------------------------------------------------------------//	
	void                       ClearDefectComponentNodeList();
	bool                       BuildNextDefectComponentCtrl();	
	void                       BuildDefectComponentList(const std::vector<CAOIComponent*> &DefectList);
	//---------------------------------------------------------------------------------//	
	bool                       UpdateDefectListCtrlTitle(CThisListCtrl_12 &ListCtrl, size_t nItem);	
	//---------------------------------------------------------------------------------//	
	bool                       BuildDefectComponentListHeader();
	bool                       ClearDefectComponentListWnd();
	bool                       BuildDefectComponentListWnd();
	bool                       BuildDefectComponentListWndKernel();
	bool                       BuildDefectComponentListWndKernel_v1();
	bool                       BuildDefectComponentListWndKernel_v2();
	bool                       UpdateDefectComponentListWnd();
	bool                       UpdateDefectComponentListWndItem(CThisListCtrl_12 &ListCtrl, int nItem, int nActItem);
	bool                       UpdateDefectComponentListSelected();
	bool                       RemoveDefectComponentListSelected();
	bool                       ExecItemchangedComponentListCtrl(int nItem);
	bool                       ExecDefectListMenu(CPoint point);	
	//---------------------------------------------------------------------------------//
	bool                       BuildDefectGroupListHeader();
	bool                       ClearDefectGroupListWnd();
	bool                       BuildDefectGroupListWnd();
	bool                       UpdateDefectGroupListTitle(CAOIComponent *ComponentPtr);
	//---------------------------------------------------------------------------------//
	bool                       BuildDefectWndListHeader();	
	bool                       ClearDefectWndListWnd();
	bool                       BuildDefectWndListWnd(int nGroupListItem);
	//---------------------------------------------------------------------------------//
	bool                       ExecComponentRotation(double Angle);
	//---------------------------------------------------------------------------------//
	bool                       ExecDblclkDefectWndListCtrl();
	bool                       ExecItemchangedDefectWndListCtrl(int nItem);
	//---------------------------------------------------------------------------------//
	bool                       AskProjectMultiBoardSelection();//詢問多聯板選取
	bool                       ExecProjectMultiBoardSelection();//執行多聯板選取
	bool                       ExecProjectOneComponentSelection();//執行單零件選取
	//---------------------------------------------------------------------------------//	
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnClickDefectListCtrl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDbclickDefectListCtrl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedDefectListCtrl(NMHDR* pNMHDR, LRESULT* pResult);	
	afx_msg void OnItemchangedDefectGroupListCtrl(NMHDR* pNMHDR, LRESULT* pResult);	
	afx_msg void OnItemchangedDefectWndListCtrl(NMHDR* pNMHDR, LRESULT* pResult);	
	afx_msg void OnDblclkDefectGroupListCtrl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkDefectWndListCtrl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndScrollDefectListCtrl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSwitchPreiousBtn();
	afx_msg void OnSwitchNextBtn();
	afx_msg void OnComponentRetestBtn();
	afx_msg void OnShowPassChk();	
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
};
//-------------------------------------------------------------------------------------//

