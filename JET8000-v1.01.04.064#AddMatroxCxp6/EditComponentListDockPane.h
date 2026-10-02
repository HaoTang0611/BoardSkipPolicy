#pragma once

//-------------------------------------------------------------------------------------//
#include "PageSplitterWnd.h"
#include "JETTreeCtrl.h"
#include "JETPropertyGridCtrl.h"
//-------------------------------------------------------------------------------------//
#include "AOITreeNode.h"
//-------------------------------------------------------------------------------------//
enum EDIT_COMPONENT_LIST_TYPE
{
	EDIT_COMPONENT_LIST_NORMAL = 1,
	EDIT_COMPONENT_LIST_MARK   = 2,
	EDIT_COMPONENT_LIST_RETURN
};
//-------------------------------------------------------------------------------------//
#define CThisTreeCtrl     CJETTreeCtrl//目前使用的列表控制類別	
//-------------------------------------------------------------------------------------//
// CEditComponentListDockPane
//-------------------------------------------------------------------------------------//
class CEditComponentListDockPane : public CDockablePane
{
	DECLARE_DYNAMIC(CEditComponentListDockPane)

public:
	CEditComponentListDockPane();
	virtual ~CEditComponentListDockPane();


public:
	//---------------------------------------------------------------------------------//		
	EDIT_COMPONENT_LIST_TYPE   GetEditComponentListType() const;
	void                       SetEditComponentListType(EDIT_COMPONENT_LIST_TYPE val);	
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//
	COLORREF                   m_clrFd;
	COLORREF                   m_clrPanel;
	COLORREF                   m_clrBoard;
	COLORREF                   m_clrComponent;
	//---------------------------------------------------------------------------------//
	CAOIProject               *m_ProjectPtr;
	DISTRICT_ID                m_DistrictID;	
	bool                       m_MoveToComponent;	
	EDIT_COMPONENT_LIST_TYPE   m_EditComponentListType;
	//---------------------------------------------------------------------------------//
	CPageSplitterWnd           m_wndSplitter;	
	//---------------------------------------------------------------------------------//
	std::vector<CAOITreeNode>  m_PanelTreeNodeList;
	bool                       m_PanelTreeNodeListDone;
	CThisTreeCtrl              m_wndComponentTreeCtrl;
	CImageList                 m_ComponentTreeImageList;
	BOOL                       m_ClickComponentTreeNode;
	BOOL                       m_StopComponentTreeBeClick;
	//---------------------------------------------------------------------------------//	
	HTREEITEM                  m_TreeItemSelected;
	CJETPropertyGridCtrl       m_wndInfomationPropCtrl;
	//---------------------------------------------------------------------------------//
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//
	BOOL                       InitTreeCtrl(CThisTreeCtrl &TreeCtrl);	
	//---------------------------------------------------------------------------------//
	bool                       GetLockUIWnd();
	//---------------------------------------------------------------------------------//	
	void                       CloseProject();
	CAOIProject*               GetActiveProject();
	DISTRICT_ID                GetActiveDistrictID();
	//---------------------------------------------------------------------------------//	
	BOOL                       BuildTreeImageList(CImageList &ImageList);
	//---------------------------------------------------------------------------------//	
	bool                       BuildComponentTreeCtrl(CThisTreeCtrl &TreeCtrl);		
	bool                       UpdateComponentTreeCtrl(CThisTreeCtrl &TreeCtrl, DWORD UpdateFlag);		
	bool                       UpdateComponentTreeCtrlState(CThisTreeCtrl &TreeCtrl, DWORD UpdateFlag);		
	bool                       ClearComponentTreeCtrl(CThisTreeCtrl &TreeCtrl, BOOL &StopTreeBeClick);
	bool                       ShowComponentTreeSelected(CThisTreeCtrl &TreeCtrl);//讓零件樹狀圖到可以看到的狀態	
	bool                       BuildNextComponentTreeCtrl(CThisTreeCtrl &TreeCtrl);
	//---------------------------------------------------------------------------------//	
	bool                       InsertProjectNodeToTreeCtrl(CThisTreeCtrl &TreeCtrl, HTREEITEM hProjectItem);
	bool                       InsertPanelNodeToTreeCtrl(CThisTreeCtrl &TreeCtrl, HTREEITEM hPanelItem, CAOIPanel* pPanel, CAOITreeNode *pPanelTreeNode);
	bool                       InsertPanelNodeToTreeCtrlByTreeNode(CThisTreeCtrl &TreeCtrl, CAOITreeNode *pBoardTreeNode, int AddCount);
	bool                       InsertFdNodeToTreeCtrl(CThisTreeCtrl &TreeCtrl, HTREEITEM hFDItem, CAOIFd *pFd, size_t FDID);
	bool                       InsertComponentWindowToTreeCtrl(CThisTreeCtrl &TreeCtrl, HTREEITEM HComponentItem, CAOIComponent *pComponent);//增加一個零件的節點	
	//---------------------------------------------------------------------------------//	
	UINT                       GetComponentTreeCtrlPopupMenuID(CThisTreeCtrl &TreeCtrl, CPoint Point);	
	//---------------------------------------------------------------------------------//	
	bool                       ExecSelectComponentTreeWndItem(CThisTreeCtrl &TreeCtrl, HTREEITEM hItem, bool RBtn);//執行選取到零件樹狀圖的Item	
	//---------------------------------------------------------------------------------//	
	bool                       ExecDbclickComponentTreeCtrl();	
	//---------------------------------------------------------------------------------//	
	void                       ExecTreeCtrlComponentMenu(CPoint point);
	//---------------------------------------------------------------------------------//
	BOOL                       InitPropList();
	BOOL                       ClearInfoPropCtrl();
	BOOL                       ClearInfoPropCtrl(CJETPropertyGridCtrl &wndPropCtrl);	
	BOOL                       BuildInfoPropCtrl_Fd(CJETPropertyGridCtrl &wndPropCtrl, CAOIFd *FdPtr);
	BOOL                       BuildInfoPropCtrl_Mark(CJETPropertyGridCtrl &wndPropCtrl, CAOIMark *MarkPtr);
	BOOL                       BuildInfoPropCtrl_Panel(CJETPropertyGridCtrl &wndPropCtrl, CAOIPanel *PanelPtr);
	BOOL                       BuildInfoPropCtrl_Board(CJETPropertyGridCtrl &wndPropCtrl, CAOIBoard *BoardPtr);
	BOOL                       BuildInfoPropCtrl_Project(CJETPropertyGridCtrl &wndPropCtrl, CAOIProject *ProjectPtr);
	BOOL                       BuildInfoPropCtrl_Barcode(CJETPropertyGridCtrl &wndPropCtrl, CAOIBarcode *BarcodePtr);
	BOOL                       BuildInfoPropCtrl_Component(CJETPropertyGridCtrl &wndPropCtrl, CAOIComponent *ComponentPtr);
	BOOL                       ExecFdChanged(CJETPropertyGridProperty *pProp, bool &bModified);
	BOOL                       ExecMarkChanged(CJETPropertyGridProperty *pProp, bool &bModified);
	BOOL                       ExecPanelChanged(CJETPropertyGridProperty *pProp, bool &bModified);
	BOOL                       ExecBoardChanged(CJETPropertyGridProperty *pProp, bool &bModified);
	BOOL                       ExecProjectChanged(CJETPropertyGridProperty *pProp, bool &bModified);
	BOOL                       ExecBarcodeChanged(CJETPropertyGridProperty *pProp, bool &bModified);
	BOOL                       ExecComponentChanged(CJETPropertyGridProperty *pProp, bool &bModified);
	//---------------------------------------------------------------------------------//	
	bool                       RemoveComponentTreeItem(CThisTreeCtrl &TreeCtrl);	
	bool                       RemoveTreeItemFdSelected(CThisTreeCtrl &TreeCtrl);//移除選取到的定位點的結點
	bool                       RemoveTreeItemMarkSelected(CThisTreeCtrl &TreeCtrl);//移除選取到的特徵的結點
	bool                       RemoveTreeItemPanelSelected(CThisTreeCtrl &TreeCtrl);//移除選取到的整板的結點
	bool                       RemoveTreeItemBoardSelected(CThisTreeCtrl &TreeCtrl);//移除選取到的單板的結點
	bool                       RemoveTreeItemBarcodeSelected(CThisTreeCtrl &TreeCtrl);//移除選取到的條碼的結點
	bool                       RemoveTreeItemComponentSelected(CThisTreeCtrl &TreeCtrl);//移除選取到的零件的結點
	bool                       RemoveTreeItemList(CThisTreeCtrl &TreeCtrl, std::vector<HTREEITEM> &RemoveItemList);
	//---------------------------------------------------------------------------------//
	bool                       CheckShowMark() const;
	bool                       CheckShowFiducial() const;
	bool                       CheckShowBarcode() const;
	bool                       CheckShowComponent() const;
	//---------------------------------------------------------------------------------//
	bool                       ExecProjectRotation(double Angle);
	bool                       ExecPanelRotation(double Angle);
	bool                       ExecBoardRotation(double Angle);
	bool                       ExecComponentRotation(double Angle);	
	//---------------------------------------------------------------------------------//
	bool                       ExecBarcodeDelete();	
	//---------------------------------------------------------------------------------//
	bool                       ExecBarcodeRotateAny();
	bool                       ExecBarcodeRotation(double Angle);	
	//---------------------------------------------------------------------------------//	
	bool                       ExecMarkDelete();
	bool                       ExecMarkBypass();
	//---------------------------------------------------------------------------------//
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnClickComponentTreeCtrl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRClickComponentTreeCtrl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDbclickComponentTreeCtrl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangedComponentTreeCtrl(NMHDR* pNMHDR, LRESULT* pResult);	
	afx_msg LRESULT OnPropertyChanged(WPARAM wParam, LPARAM lParam);
	afx_msg void OnProjectSortComponent();	
	afx_msg void OnProjectRotate090();
	afx_msg void OnProjectRotate180();
	afx_msg void OnProjectRotate270();
	afx_msg void OnPanelOffset();
	afx_msg void OnPanelMirrorPosX();
	afx_msg void OnPanelMirrorPosY();
	afx_msg void OnPanelRotate090();
	afx_msg void OnPanelRotate180();
	afx_msg void OnPanelRotate270();
	afx_msg void OnPanelDelete();
	afx_msg void OnPanelBypass();	
	afx_msg void OnBoardOffset();
	afx_msg void OnBoardMirrorPosX();
	afx_msg void OnBoardMirrorPosY();
	afx_msg void OnBoardRotate090();
	afx_msg void OnBoardRotate180();
	afx_msg void OnBoardRotate270();
	afx_msg void OnBoardDelete();
	afx_msg void OnBoardBypass();
	afx_msg void OnBoardChangePanel();
	afx_msg void OnBoardSearchComponent();
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
	afx_msg void OnFdDelete();
	afx_msg void OnFdToTeachPos();
	afx_msg void OnFdSetTeachPos();
	afx_msg void OnFdToCurrentPos();	
	afx_msg void OnFdUpdateToOthers();	
	afx_msg void OnBarcodeDelete();
	afx_msg void OnBarcodeRotateAny();
	afx_msg void OnBarcodeUpdateToOthers();	
	afx_msg void OnMarkDelete();
	afx_msg void OnMarkBypass();
	afx_msg void OnMarkSpaceNoiseFilter();
	afx_msg void OnMarkSpaceBasePlane();	
	afx_msg void OnMarkPasteToOtherBoard();	
	afx_msg void OnMarkLocalBasePlaneID();
	afx_msg void OnMarkUpdateToOthers();	
	DECLARE_MESSAGE_MAP()
	//---------------------------------------------------------------------------------//	
};
//-------------------------------------------------------------------------------------//

