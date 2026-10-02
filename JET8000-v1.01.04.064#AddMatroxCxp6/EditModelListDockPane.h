#pragma once
//-------------------------------------------------------------------------------------//
#include "JETListCtrl.h"
#include "JETMFCListCtrl.h"
#include "PageSplitterWnd.h"
#include "EditModelListPaneBar.h"
//-------------------------------------------------------------------------------------//
#define CThisListCtrl_10     CJETMFCListCtrl//目前使用的列表控制類別	
//-------------------------------------------------------------------------------------//
// CEditModelListDockPane
//-------------------------------------------------------------------------------------//
class CEditModelListDockPane : public CDockablePane
{
	DECLARE_DYNAMIC(CEditModelListDockPane)

public:
	CEditModelListDockPane();
	virtual ~CEditModelListDockPane();
	//---------------------------------------------------------------------------------//
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
	CPageSplitterWnd           m_wndSplitter;
	CEditModelListPaneBar      m_wndPaneBar;
	//---------------------------------------------------------------------------------//	
	bool                       m_SwitchModelBtn;
	bool                       m_MoveToComponent;	
	CString                    m_ModelName;
	CString                    m_GroupName;
	std::vector<CString>       m_ModelNameList;
	std::vector<CString>       m_GroupNameList;
	CThisListCtrl_10           m_wndModelListCtrl;
	BOOL                       m_StopModelListBeSelected;
	//---------------------------------------------------------------------------------//		
	CString                    m_ComponentModelName;
	CString                    m_ComponentGroupName;
	CThisListCtrl_10           m_wndComponentListCtrl;
	std::vector<CSortObj>      m_ComponentNodeList;
	BOOL                       m_StopComponentListBeSelected;
	//---------------------------------------------------------------------------------//		
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//		
	COLORREF                   GetResultColor(RESULT_ID ResultID);
	//---------------------------------------------------------------------------------//
	void                       CloseProject();
	CAOIProject*               GetActiveProject();
	DISTRICT_ID                GetActiveDistrictID();
	//---------------------------------------------------------------------------------//		
	BOOL                       InitModelListCtrl(CThisListCtrl_10 &ListCtrl);
	BOOL                       BuildModelListCtrl(CThisListCtrl_10 &ListCtrl);
	BOOL                       UpdateModelListCtrl(CThisListCtrl_10 &ListCtrl, bool bForce=true);
	BOOL                       ClearModelListCtrl(CThisListCtrl_10 &ListCtrl);
	BOOL                       RemoveModelListItem(CThisListCtrl_10 &ListCtrl);
	bool                       ExecModelListMenu(CPoint point);
	bool                       UpdateModelListCtrlTitle(CThisListCtrl_10 &ListCtrl);
	bool                       UpdateModelListCtrlTitle(CThisListCtrl_10 &ListCtrl, size_t ModelCount, size_t ModelUsed);
	//---------------------------------------------------------------------------------//	
	BOOL                       InitComponentListCtrl(CThisListCtrl_10 &ListCtrl);			
	BOOL                       BuildComponentCtrl(CThisListCtrl_10 &ListCtrl, LPCTSTR strModelName, LPCTSTR strGroupName, bool bForce=true);	
	BOOL                       ClearComponentListCtrl(CThisListCtrl_10 &ListCtrl);	
	BOOL                       ShowComponentListSelected(CThisListCtrl_10 &ListCtrl);
	BOOL                       UpdateComponentListState(CThisListCtrl_10 &ListCtrl);
	BOOL                       ExecComponentListMenu(CPoint point);
	bool                       UpdateComponentListCtrlTitle(CThisListCtrl_10 &ListCtrl, size_t ComponentCount);
	//---------------------------------------------------------------------------------//	
	void                       ClearComponentNodeList();
	bool                       BuildNextComponentCtrl();	
	void                       BuildComponentNodeList(const std::vector<CSortObj> &SortList);
	//---------------------------------------------------------------------------------//
	int                        FindItemPreious_Any(CThisListCtrl_10 &ListCtrl);
	int                        FindItemPreious_Set(CThisListCtrl_10 &ListCtrl);
	int                        FindItemPreious_UnSet(CThisListCtrl_10 &ListCtrl);

	int                        FindItemNext_Any(CThisListCtrl_10 &ListCtrl);
	int                        FindItemNext_Set(CThisListCtrl_10 &ListCtrl);
	int                        FindItemNext_UnSet(CThisListCtrl_10 &ListCtrl);
	//---------------------------------------------------------------------------------//
	bool                       ExecComponentRotation(double Angle);
	//---------------------------------------------------------------------------------//	
	void                       ExecModelApply();
	void                       ExecModelImport();
	bool                       ExecModelAdd(CAOIModel *ModelPtr);
	void                       ExecModelReference(CString ModelName, CAOIProject *LibraryProjectPtr);
	//---------------------------------------------------------------------------------//	
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnClickModelListCtrl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDbclickModelListCtrl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedModelListCtrl(NMHDR* pNMHDR, LRESULT* pResult);	
	afx_msg void OnClickComponentListCtrl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDbclickComponentListCtrl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedComponentListCtrl(NMHDR* pNMHDR, LRESULT* pResult);	
	afx_msg void OnEndScrollComponentListCtrl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSwitchPreiousBtn();
	afx_msg void OnSwitchNextBtn();
	afx_msg void OnModelBypass();
	afx_msg void OnModelSearch();
	afx_msg void OnModelApply();
	afx_msg void OnModelImport();	
	afx_msg void OnModelClone();
	afx_msg void OnModelRename();
	afx_msg void OnModelDelete();
	afx_msg void OnModelSelectAll();	
	afx_msg void OnModelUpdateToOthers();	
	afx_msg void OnModelShowLibraryWnd();
	afx_msg void OnModelSaveLeadReport();	
	afx_msg void OnModelDefectRecheckARS();	
	afx_msg void OnModelDefectEssential();	
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
	
};
//-------------------------------------------------------------------------------------//