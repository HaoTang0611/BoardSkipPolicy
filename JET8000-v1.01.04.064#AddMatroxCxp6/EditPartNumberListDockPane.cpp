// EditPartNumberListDockPane.cpp : 實作檔
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "EditPartNumberListDockPane.h"
//-------------------------------------------------------------------------------------//
#include "InputBoxWnd.h"
#include "InputListWnd.h"
#include "SpaceBaseParamWnd.h"
#include "ComponentDefectAlarmWnd.h"
#include "SpaceNoiseFilterParamWnd.h"
//-------------------------------------------------------------------------------------//
// CEditPartNumberListDockPane
//-------------------------------------------------------------------------------------//
#define LIST_ITEM_PATCH_N_PAGE_VALUE     10   //一次多少個頁面
#define LIST_ITEM_PATCH_ENABLE_COUNT   1000
//-------------------------------------------------------------------------------------//
const UINT ID_PART_NUMBER_LIST = CPageSplitterWnd::GetIdFromRowCol(0, 0);//AFX_IDW_PANE_FIRST;
const UINT ID_SUB_LIST_CTRL    = CPageSplitterWnd::GetIdFromRowCol(1, 0);//AFX_IDW_PANE_FIRST+16;//注意行列會不同唷
//-------------------------------------------------------------------------------------//
IMPLEMENT_DYNAMIC(CEditPartNumberListDockPane, CDockablePane)
//-------------------------------------------------------------------------------------//
CEditPartNumberListDockPane::CEditPartNumberListDockPane()
{
	m_clrOK = 0x008000;
	m_clrNG = 0x000080;
	m_clrBypass = 0x800000;
	m_clrUnTest = 0x808080;

	m_ProjectPtr = NULL;	
	m_DistrictID = DISTRICT_ID_A;
	m_StopPartNumberListBeSelected = FALSE;
	m_StopSubListBeSelected = FALSE;
	m_SubCtrlMode = SUB_LIST_COMPONENT;
	m_ComponentIndex = -1;
	m_MoveToComponent = true;
	m_SwitchPartNumberBtn = false;
}
//-------------------------------------------------------------------------------------//
CEditPartNumberListDockPane::~CEditPartNumberListDockPane()
{
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CEditPartNumberListDockPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_CONTEXTMENU()	
	ON_NOTIFY(NM_CLICK, ID_PART_NUMBER_LIST, OnClickPartNumberListCtrl)
	ON_NOTIFY(NM_DBLCLK, ID_PART_NUMBER_LIST, OnDbclickPartNumberListCtrl)	
	ON_NOTIFY(LVN_ITEMCHANGED, ID_PART_NUMBER_LIST, OnItemchangedPartNumberListCtrl)
	ON_NOTIFY(NM_CLICK, ID_SUB_LIST_CTRL, OnClickSubListCtrl)	
	ON_NOTIFY(NM_DBLCLK, ID_SUB_LIST_CTRL, OnDbclickSubListCtrl)	
	ON_NOTIFY(LVN_ITEMCHANGED, ID_SUB_LIST_CTRL, OnItemchangedSubListCtrl)
	ON_NOTIFY(LVN_ENDSCROLL, ID_SUB_LIST_CTRL, OnEndScrollSubListCtrl)
	ON_BN_CLICKED(EPNB_SWITCH_PREVIOUS_BTN, OnSwitchPreiousBtn)	
	ON_BN_CLICKED(EPNB_SWITCH_NEXT_BTN, OnSwitchNextBtn)
	ON_COMMAND(MENU_LIST_PART_NUMBER_BYPASS, OnPartNumberBypass)	
	ON_COMMAND(MENU_LIST_PART_NUMBER_SEARCH, OnPartNumberSearch)
	ON_COMMAND(MENU_LIST_PART_NUMBER_RENAME, OnPartNumberRename)
	ON_COMMAND(MENU_LIST_PART_NUMBER_DELETE, OnPartNumberDelete)
	ON_COMMAND(MENU_LIST_PART_NUMBER_SELECT_ALL, OnPartNumberSelectAll)	
	ON_COMMAND(MENU_TREE_COMPONENT_OFFSET, OnComponentOffset)
	ON_COMMAND(MENU_TREE_COMPONENT_SET_POS, OnComponentSetPos)
	ON_COMMAND(MENU_TREE_COMPONENT_MIRROR_POS_X, OnComponentMirrorPosX)
	ON_COMMAND(MENU_TREE_COMPONENT_MIRROR_POS_Y, OnComponentMirrorPosY)
	ON_COMMAND(MENU_TREE_COMPONENT_ROTATE_090, OnComponentRotate090)
	ON_COMMAND(MENU_TREE_COMPONENT_ROTATE_180, OnComponentRotate180)
	ON_COMMAND(MENU_TREE_COMPONENT_ROTATE_270, OnComponentRotate270)
	ON_COMMAND(MENU_TREE_COMPONENT_ROTATE_ANY, OnComponentRotateAny)
	ON_COMMAND(MENU_TREE_COMPONENT_ROTATE_REVERSE, OnComponentRotateReverse)
	ON_COMMAND(MENU_TREE_COMPONENT_RENAME, OnComponentRename)
	ON_COMMAND(MENU_TREE_COMPONENT_DELETE, OnComponentDelete)
	ON_COMMAND(MENU_TREE_COMPONENT_SET_NOZZLE_NAME, OnComponentSetNozzleName)
	ON_COMMAND(MENU_TREE_COMPONENT_SET_PARTNUMBER, OnComponentSetPartNumber)
	ON_COMMAND(MENU_TREE_COMPONENT_SEARCH, OnComponentSearch)
	ON_COMMAND(MENU_TREE_COMPONENT_SELECT_ALL, OnComponentSelectAll)
	ON_COMMAND(MENU_TREE_COMPONENT_BYPASS, OnComponentBypass)
	ON_COMMAND(MENU_TREE_COMPONENT_XBOARD_UNIT, OnComponentXBoardUnit)	
	ON_COMMAND(MENU_TREE_COMPONENT_MODEL_ISOLATED, OnComponentModelIsolated)
	ON_COMMAND(MENU_TREE_COMPONENT_RESTORE_CAD_POS, OnComponentRestoreCadPos)
	ON_COMMAND(MENU_TREE_COMPONENT_BYPASS_3D, OnComponentBypass3D)	
	ON_COMMAND(MENU_TREE_COMPONENT_MASK_BASE_COLOR_LINK_INDEX, OnComponentMaskBaseSetColorIndex)
	ON_COMMAND(MENU_TREE_COMPONENT_SPACE_NOISE_FILTER, OnComponentSpaceNoiseFilter)
	ON_COMMAND(MENU_TREE_COMPONENT_MASK_EXTEND_SIZE_BODY, OnComponentMaskExtendSizeBody)
	ON_COMMAND(MENU_TREE_COMPONENT_ENABLE_ALARM_AOI, OnComponentEnableAlarmAOI)	
	ON_COMMAND(MENU_TREE_COMPONENT_GROUP_ID, OnComponentGroupID)
	ON_COMMAND(MENU_TREE_COMPONENT_GROUP_ORG, OnComponentGroupOrg)		
	ON_COMMAND(MENU_TREE_COMPONENT_TO_FIELD_POS, OnComponentToFieldPos)		
	ON_COMMAND(MENU_TREE_COMPONENT_ENABLE_SELF_FIELD, OnComponentEnableSelfField)
	ON_COMMAND(MENU_TREE_COMPONENT_CLONE_NEW_MODEL, OnComponentCloneNewModel)
	ON_COMMAND(MENU_TREE_COMPONENT_CHANGE_BOARD, OnComponentChangeBoard)
	ON_COMMAND(MENU_TREE_COMPONENT_LOCAL_BASE_PLANE_ID, OnComponentLocalBasePlaneID)
	ON_COMMAND(MENU_TREE_COMPONENT_DATA_MODEL_PARAM, OnComponentDataModelParam)
	ON_COMMAND(MENU_TREE_COMPONENT_SAVE_WND_LIST, OnComponentSaveWndList)
	ON_COMMAND(MENU_TREE_COMPONENT_FEEDBACK_RESULT_POS, OnComponentFeedbackResultPos)
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
// CEditPartNumberListDockPane 訊息處理常式
//-------------------------------------------------------------------------------------//
int CEditPartNumberListDockPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此加入特別建立的程式碼
	DWORD dwBarStyle = CBRS_LEFT|CBRS_TOOLTIPS|CBRS_FLYBY;
	//if ( !m_wndDialogbar.Create(this, IDD_PAGE_DIALOGBAR_PART_NUMBER, dwBarStyle, IDD_PAGE_DIALOGBAR_PART_NUMBER) )
	if ( !m_wndPaneBar.Create(this, IDD_EDIT_PART_NUMBER_LIST_PANE_BAR, dwBarStyle, IDD_EDIT_PART_NUMBER_LIST_PANE_BAR) )
	{
		TRACE0("Failed to create Page Dialogbar Control\n");
		return -1;
	}

	m_wndSplitter.CreateStatic(this,2,1);	
	DWORD dwListStyle1 = WS_CHILD | WS_VISIBLE | LVS_REPORT  | LVS_SHAREIMAGELISTS | LVS_SINGLESEL | LVS_SHOWSELALWAYS;
	DWORD dwListStyle2 = WS_CHILD | WS_VISIBLE | LVS_REPORT  | LVS_SHAREIMAGELISTS;
	DWORD dwTreeStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS |TVS_EDITLABELS | TVS_SHOWSELALWAYS | TVS_FULLROWSELECT;
	//WC_TREEVIEW, WC_LISTVIEW
	if(!m_wndSplitter.AddWindow(0,0,&m_wndPartNumberListCtrl,WC_LISTVIEW,dwListStyle1,0,CSize(160,400), ID_PART_NUMBER_LIST))
	{
		TRACE0("Failed to create Page Spliter Context(0, 0)\n");
		return -1;
	}	
	
	if(!m_wndSplitter.AddWindow(1,0,&m_wndSubListCtrl,WC_LISTVIEW,dwListStyle2,0,CSize(160,400), ID_SUB_LIST_CTRL))
	{
		TRACE0("Failed to create Page Spliter Context(1, 0)\n");
		return -1;
	}
	
	m_wndPartNumberListCtrl.SetOwner(this);
	m_wndSubListCtrl.SetOwner(this);

	JetAPI::InitialListCtrl(m_wndPartNumberListCtrl);
	InitPartNumberListCtrl(m_wndPartNumberListCtrl);

	JetAPI::InitialListCtrl(m_wndSubListCtrl);
	InitSubListCtrl(m_wndSubListCtrl, m_SubCtrlMode);	
	return 0;
}
//-------------------------------------------------------------------------------------//
void CEditPartNumberListDockPane::OnDestroy()
{
	CDockablePane::OnDestroy();

	// TODO: 在此加入您的訊息處理常式程式碼
}
//-------------------------------------------------------------------------------------//
void CEditPartNumberListDockPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: 在此加入您的訊息處理常式程式碼
	if ( CWnd::GetSafeHwnd() == NULL ) { return ; }
	if ( m_wndSplitter.GetSafeHwnd() == NULL ) { return; }
	if ( m_wndPaneBar.GetSafeHwnd() == NULL ) { return; }
	CRect rect;
	int cyTlb = 0;
	cyTlb = m_wndPaneBar.CalcFixedLayout(FALSE, FALSE).cy;
	rect.left = 0; 
	rect.top = 0;
	rect.right = cx;
	rect.bottom = cy;

	m_wndPaneBar.SetWindowPos(NULL,rect.left, rect.top, rect.Width(), cyTlb, SWP_NOZORDER | SWP_NOACTIVATE);
	//m_wndSplitter.SetRowInfo(0,(cy-cyTlb)/2,25);
	m_wndSplitter.SetWindowPos(NULL,rect.left, rect.top+cyTlb, rect.Width(), rect.Height()-cyTlb, SWP_NOZORDER | SWP_NOACTIVATE);
}
//-------------------------------------------------------------------------------------//
void CEditPartNumberListDockPane::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDockablePane::OnShowWindow(bShow, nStatus);

	// TODO: 在此加入您的訊息處理常式程式碼
	if ( TRUE == bShow )
	{	
		BuildPartNumberListCtrl(m_wndPartNumberListCtrl);	
		//m_wndSubListCtrl.SetFocus();
		AOIDataCollect.SetShowUIWndPartNumberList(true);
	}
	else
	{	
		CloseProject(); 
		AOIDataCollect.SetShowUIWndPartNumberList(false);
	}
}
//-------------------------------------------------------------------------------------//
void CEditPartNumberListDockPane::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: 在此加入您的訊息處理常式程式碼
	if ( NULL != pWnd )
	{
		UINT CtrlID = pWnd->GetDlgCtrlID();
		if ( pWnd->IsKindOf(RUNTIME_CLASS(CPageSplitterWnd)) == TRUE )
		{
			CPageSplitterWnd *pSplitterWnd = (CPageSplitterWnd*)pWnd;
			CWnd  *pPane = pSplitterWnd->GetActivePane();
			if ( NULL == pPane )
			{	pPane = pSplitterWnd->GetFocus();	}
			if ( NULL != pPane )
			{	CtrlID = pPane->GetDlgCtrlID();	}			
		}
		if ( ID_PART_NUMBER_LIST == CtrlID )
		{	ExecPartNumberListMenu(point);	}
		else if ( ID_SUB_LIST_CTRL == CtrlID )
		{	ExecComponentListMenu(point);	}		
	}	
}
//-------------------------------------------------------------------------------------//
void CEditPartNumberListDockPane::SwitchMultiLanguage()
{
	/*
	size_t  i = 0;	
	UINT    WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_EDIT_PARTNUMBER_LIST_DOCK_PANE");	//IDD_EDIT_PARTNUMBER_LIST_WND
	//---------------------------------------------------------------------------------//
	WndID = IDD_EDIT_PARTNUMBER_LIST_PAGE;
	WndKey = _T("IDD_EDIT_PARTNUMBER_LIST_DOCK_PANE");
	this->GetWindowText(LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetWindowText(NewLabelText);
	//---------------------------------------------------------------------------------//	
	*/
}
//-------------------------------------------------------------------------------------//
CString CEditPartNumberListDockPane::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_EDIT_PARTNUMBER_LIST_DOCK_PANE");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
BOOL CEditPartNumberListDockPane::InitListCtrl(CThisListCtrl_11 &ListCtrl)
{
	CString str;
	int   nCol = 0;
	const int width2 = 96;
	const int Align = LVCFMT_CENTER;//LVCFMT_CENTER;LVCFMT_RIGHT

	JetAPI::InitialListCtrl(ListCtrl);
	ListCtrl.DeleteAllItems();

	str = _T("Idx");		
	ListCtrl.InsertColumn(nCol, str, Align, 32);
	nCol ++;	

	str = _T("Name");	
	ListCtrl.InsertColumn(nCol, str, Align, width2*1.5);
	nCol ++;	

	str = _T("Test");	
	ListCtrl.InsertColumn(nCol, str, Align, width2*1.5);
	nCol ++;


	int i=0, j=0;
	m_StopPartNumberListBeSelected = TRUE;
	for ( i=0; i<5; i++ )
	{
		str.Format(_T("%d"), i+1);
		ListCtrl.InsertItem(i, str);

		j = 0;
		ListCtrl.SetItemText(i, j, str);
		j ++;


		str.Format(_T("Name:%d"), i+1);
		ListCtrl.SetItemText(i, j, str);
		j++;

		str.Format(_T("Test:%d"), i+1);
		ListCtrl.SetItemText(i, j, str);
		j ++;
	}
	m_StopPartNumberListBeSelected = FALSE;
	return TRUE;
}
//-------------------------------------------------------------------------------------//
void CEditPartNumberListDockPane::OnClickPartNumberListCtrl(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NMITEMACTIVATE *pNMListView = (NMITEMACTIVATE*)pNMHDR;	
	const int nItem = pNMListView->iItem;	
	//UpdateFrameParamToUI(nItem);
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CEditPartNumberListDockPane::OnDbclickPartNumberListCtrl(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here
	NMITEMACTIVATE *pNMListView = (NMITEMACTIVATE*)pNMHDR;	
	const int nItem = pNMListView->iItem;	
	if ( nItem < 0 ) { return; }
	const size_t PartNumberCount=m_PartNumberList.size();
	size_t Index = m_wndPartNumberListCtrl.GetItemData(nItem);
	if ( Index >= PartNumberCount ) { return; }

	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr )
	{	return; }

	CAOIComponent *ActiveComponentPtr = ProjectPtr->GetProjectActiveComponent();
	AOIDataCollect.CloseActiveComponent(ActiveComponentPtr);
	if ( NULL != ActiveComponentPtr )
	{	ActiveComponentPtr->SetComponentSelected(false); }

	CString strItem = m_wndPartNumberListCtrl.GetItemText(nItem, 0);
	CString strPartNumber = m_PartNumberList[Index];
	strPartNumber.MakeUpper();
	CAOIComponent *ComponentPtr = NULL;
	const int SubCount = m_wndSubListCtrl.GetItemCount();		
	if ( SubCount>0 && SUB_LIST_COMPONENT==m_SubCtrlMode ) 
	{
		CString strName = m_wndSubListCtrl.GetItemText(0, 0);
		strName.MakeUpper();
		DWORD_PTR ItemData = m_wndSubListCtrl.GetItemData(0);
		ComponentPtr = ProjectPtr->GetProjectComponentPtr(ItemData, true);
		//ComponentPtr = ProjectPtr->GetProjectComponentPtrByComponentName(str);
	}
	else
	{	ComponentPtr = ProjectPtr->GetProjectComponentPtrByPartNumber(strPartNumber);	 }
	if ( NULL == ComponentPtr )
	{	return; }

	ProjectPtr->SelectProjectAllComponents(false);
	ComponentPtr->SetComponentSelected(true);
	ProjectPtr->SetProjectActiveComponent(ComponentPtr);	

	CAOIModel *ModelPtr = ComponentPtr->GetComponentModelPtr();
	CAOIWnd   *WndPtr = ModelPtr->GetModelWndFirst();	
	ModelPtr->UnSelectModel();
	if ( NULL != WndPtr )
	{	WndPtr->SetWndSelected(true);	}
	else
	{	ModelPtr->SetModelBodyBoxActived(true);	}
	ModelPtr->SetModelWndActived(WndPtr);
	ComponentPtr->SetComponentSelected(true);			
	ProjectPtr->SetProjectActiveModelWnd(WndPtr);
	AOIDataCollect.SendMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_UPDATE, (LPARAM)(this));

	AOIDataCollect.MoveStageToComponentOrField(ComponentPtr, true);	
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CEditPartNumberListDockPane::OnItemchangedPartNumberListCtrl(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMITEMACTIVATE *pNMListView = (NMITEMACTIVATE*)pNMHDR;	
	// TODO: Add your control notification handler code here	
	if ( TRUE == m_StopPartNumberListBeSelected ) { return; }
	const int nItem = pNMListView->iItem;	
	if ( nItem < 0 ) { return; }
	DWORD Res = pNMListView->uNewState&LVIS_SELECTED;
	if ( Res == 0 ) { return; }	
	Res = pNMListView->uNewState&LVIS_FOCUSED;
	if ( Res == 0 ) { return; }	
	const size_t PartNumberCount=m_PartNumberList.size();
	size_t Index = m_wndPartNumberListCtrl.GetItemData(nItem);
	if ( Index >= PartNumberCount ) { return; }

	CString strItem = m_wndPartNumberListCtrl.GetItemText(nItem, 0);
	CString strPartNumber = m_PartNumberList[Index];
	strPartNumber.MakeUpper();
	BuildSubListCtrl(m_wndSubListCtrl, strPartNumber);
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CEditPartNumberListDockPane::OnClickSubListCtrl(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NMITEMACTIVATE *pNMListView = (NMITEMACTIVATE*)pNMHDR;	
	const int nItem = pNMListView->iItem;	
	//UpdateFrameParamToUI(nItem);	
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr )	{	return; }
	CAOIComponent *pComponent = NULL;

	CString      ComName;
	size_t       ItemSelCount=0;
	POSITION     ItemPos = NULL;	
	int          nItemSelected = 0;	
	unsigned int ComponentIndex=0;
	MULTI_BOARD_CTRL_MODE MultiBoardCtrlMode = AOIDataCollect.GetMultiBoardCtrlMode();	
	switch ( m_SubCtrlMode )
	{
	case SUB_LIST_WINDOW:
		break;
	default:		
		//pComponent = ProjectPtr->GetProjectActiveComponent();
		//AOIDataCollect.CloseActiveComponent(pComponent);
		ItemPos = m_wndSubListCtrl.GetFirstSelectedItemPosition();
		while ( ItemPos!=NULL )
		{
			nItemSelected = m_wndSubListCtrl.GetNextSelectedItem(ItemPos);
			ComponentIndex = (unsigned int)m_wndSubListCtrl.GetItemData(nItemSelected); 
			pComponent = ProjectPtr->GetProjectComponentPtr(ComponentIndex, true);
			if ( NULL != pComponent )
			{	
				pComponent->SetComponentSelected(true);	
			#ifdef _DEBUG
				ComName = pComponent->GetComponentName();
			#endif//_DEBUG
			}
			ItemSelCount ++;
		};
		
		if ( MULTI_BOARD_CTRL_DISABLE != MultiBoardCtrlMode )
		{
			std::vector<CAOIComponent*> SelComponentList;
			ProjectPtr->GetProjectComponentSelected(SelComponentList);
			ProjectPtr->SelectProjectComponentByMultiBoardCtrlMode(SelComponentList, MultiBoardCtrlMode);
			UpdateComponentListState(m_wndSubListCtrl);	
		}
		AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_PART_SELECTED, NULL);
		break;
	}		
	//AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_UPDATE, (LPARAM)(this));
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CEditPartNumberListDockPane::OnDbclickSubListCtrl(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here	
	unsigned int WindowIndex = 0;
	unsigned int ComponentIndex = 0;
	const int nItem = m_wndSubListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if ( nItem < 0 ) { return; }	
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }

	TPOINT3D       StagePos;
	TREGION4D      StageRgn;
	bool           GetPos = false;
	CAOIWindow    *pWindow = NULL;
	CAOIComponent *pComponent = NULL;	
	switch ( m_SubCtrlMode )
	{
	case SUB_LIST_WINDOW:
		WindowIndex = (unsigned int)m_wndSubListCtrl.GetItemData(nItem);
		pComponent = ProjectPtr->GetProjectActiveComponent();
		if ( NULL == pComponent ) { return; }
		pWindow = pComponent->GetComponentWindowPtr(WindowIndex, true);
		if ( NULL == pWindow ) { return ; }
		break;
	default:
		ComponentIndex = (unsigned int)m_wndSubListCtrl.GetItemData(nItem);
		pComponent = ProjectPtr->GetProjectComponentPtr(ComponentIndex, true);
		if ( NULL == pComponent ) { return; }
		StagePos = pComponent->GetComponentStagePos();
		pComponent->GetComponentRoiStageRegion(StageRgn);
		GetPos = true;
		break;
	}
	if ( false == GetPos ) { return; }	
	AOIDataCollect.MoveStageTo(StagePos.x, StagePos.y, StageRgn);
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CEditPartNumberListDockPane::OnItemchangedSubListCtrl(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if ( TRUE == m_StopSubListBeSelected ) { return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }
	const int nItem = pNMListView->iItem;	
	if ( nItem < 0 ) { return; }
	DWORD    Res = 0;	
	bool     bFocused = false;
	bool     bSelected = false;
	bool     bDropHilited=false;
	Res = pNMListView->uNewState&LVIS_FOCUSED;
	if ( Res != 0 )	{	bFocused = true; }	
	Res = pNMListView->uNewState&LVIS_SELECTED;
	if ( Res != 0 )	{	bSelected = true; }		
	Res = pNMListView->uChanged&LVIS_DROPHILITED;	
	if ( Res == 0 )	{ bDropHilited = true; }
	if ( false == bFocused ) { return; }	

	unsigned int WindowIndex=0;
	unsigned int ComponentIndex=0;	
	CAOIWindow    *pWindow = NULL;
	CAOIComponent *pComponent = NULL;	
	bool  MultiKey = false;
	const bool PressCtrl = JetAPI::CheckIsPressVRKey(VK_CONTROL);
	const bool PressShift = JetAPI::CheckIsPressVRKey(VK_SHIFT);
	switch ( m_SubCtrlMode )
	{
	case SUB_LIST_WINDOW:			
		ComponentIndex = m_ComponentIndex;
		WindowIndex = (unsigned int)m_wndSubListCtrl.GetItemData(nItem);
		break;
	default:
		pComponent = ProjectPtr->GetProjectActiveComponent();		
		AOIDataCollect.CloseActiveComponent(pComponent);		
		if ( NULL != pComponent )
		{	pComponent->SetComponentSelected(false); }

		WindowIndex = 0;		
		ComponentIndex = (unsigned int)m_wndSubListCtrl.GetItemData(nItem);		
		break;
	}
	if ( true==PressCtrl || true==PressShift )
	{
		MultiKey = true;
		WindowIndex = 0;	
	}
	else
	{
		MultiKey = false;
		ProjectPtr->SelectProjectAllComponents(false); 
	}
	pComponent = ProjectPtr->GetProjectComponentPtr(ComponentIndex, true);
	ProjectPtr->ResetProjectActiveIndex();
	if ( NULL != pComponent )	
	{
		pComponent->SetComponentSelected(true);
		pComponent->ChangeComponentSelected(true);
		ProjectPtr->SetProjectActiveComponent(pComponent);
		pWindow = pComponent->GetComponentWindowPtr(WindowIndex, true);		
		if ( NULL == pWindow )
		{	ProjectPtr->SetProjectActiveComponentWindowIndex(-1);	}
		else
		{	ProjectPtr->SetProjectActiveComponentWindowIndex(WindowIndex);	}			
	}

	MULTI_BOARD_CTRL_MODE MultiBoardCtrlMode = AOIDataCollect.GetMultiBoardCtrlMode();
	if ( (MULTI_BOARD_CTRL_DISABLE!=MultiBoardCtrlMode) && false==MultiKey )
	{		
		std::vector<CAOIComponent*> SelComponentList;
		ProjectPtr->GetProjectComponentSelected(SelComponentList);
		ProjectPtr->SelectProjectComponentByMultiBoardCtrlMode(SelComponentList, MultiBoardCtrlMode);
		UpdateComponentListState(m_wndSubListCtrl);		
	}

	//AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_UPDATE, (LPARAM)(this));
	AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_PART_SELECTED, NULL);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_WND_PROPERTY_WND, WPARAM_BUILD_WND_SELECTED, NULL);
	if ( true == m_MoveToComponent )
	{	AOIDataCollect.MoveStageToComponentOrField(pComponent, false);	}
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CEditPartNumberListDockPane::OnEndScrollSubListCtrl(NMHDR* pNMHDR, LRESULT* pResult)
{	
	NMLVSCROLL* pStateChanged = (NMLVSCROLL*)pNMHDR;	
	// TODO: Add your control notification handler code here
	if ( 0 != pStateChanged->dx )//Hor
	{
	}
	if ( 0 != pStateChanged->dy )//Ver
	{
		if ( pStateChanged->dy > 0 )
		{	BuildNextComponentCtrl();	}
	}
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
LRESULT CEditPartNumberListDockPane::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	DWORD Res = 0;
	CWnd *pWnd = NULL;
	switch ( message )
	{
	case MSG_MAIN_FRAME_MESSAGE:
		switch ( wParam )
		{
		case WPARAM_PROJECT_NEW:
		case WPARAM_PROJECT_OPEN:
		case WPARAM_PROJECT_SWITCH:		
			if ( CWnd::IsWindowVisible() == FALSE )
			{	CloseProject();	}
			else
			{	BuildPartNumberListCtrl(m_wndPartNumberListCtrl); }
			break;
		case WPARAM_PROJECT_UPDATE:
			if ( CWnd::IsWindowVisible() == TRUE )
			{
				pWnd = (CWnd*)lParam;
				if ( pWnd != this )
				{	UpdatePartNumberListCtrl(m_wndPartNumberListCtrl, false); }
			}
			break;			
		case WPARAM_PROJECT_CLOSE:
			CloseProject();
			break;				
		case WPARAM_PROJECT_PART_DELETED:
			RemovePartNumberListItem(m_wndPartNumberListCtrl);
			break;
		case WPARAM_PROJECT_CLOSE_ACTIVE_OBJ:
			break;
		}
		break;
	case MSG_EDIT_PART_LIST_WND:
		switch ( wParam )
		{		
		case WPARAM_BUILD_PART_LIST:
			Res = lParam&LPARAM_BUILD_DOCK_LIST_PART_NUMBER;
			if ( 0 != Res )
			{
				BuildPartNumberListCtrl(m_wndPartNumberListCtrl);
				AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_WND_PROPERTY_WND, WPARAM_BUILD_WND_SELECTED, NULL);
			}
			break;
		case WPARAM_UPDATE_PART_LIST:
			UpdatePartNumberListCtrl(m_wndPartNumberListCtrl, false);
			AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_WND_PROPERTY_WND, WPARAM_BUILD_WND_SELECTED, NULL);
			break;	
		case WPARAM_CLEAR_PART_LIST:
			ClearSubListCtrl(m_wndSubListCtrl);
			ClearPartNumberListCtrl(m_wndPartNumberListCtrl);
			AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_WND_PROPERTY_WND, WPARAM_BUILD_WND_SELECTED, NULL);
			break;		
		}
		break;
	case MSG_LIST_CTRL:
		switch ( wParam )
		{
		case WPARAM_LIST_VERTICAL_SCROLL_END:
			if ( (int)(lParam) > 0 )
			{	BuildNextComponentCtrl();	}			
			break;
		}
		break;
	}
	return CWnd::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
COLORREF CEditPartNumberListDockPane::GetResultColor(RESULT_ID ResultID)
{
	COLORREF Color;
	const int Divide = 2;
	const TSystemParameter &SystemParam = AOIDataCollect.GetSystemParameter();
	switch ( ResultID )
	{
	case RESULT_ID_NG:
		Color=m_clrNG;			
		Color = JetAPI::DivideColor(SystemParam.m_InspectedResultNGColor, Divide);
		break;
	case RESULT_ID_EXCEPTION:
		Color=m_clrNG;	
		Color=SystemParam.m_InspectedResultExceptionColor;	
		break;
	case RESULT_ID_OK:	
		Color=m_clrOK;
		Color = JetAPI::DivideColor(SystemParam.m_InspectedResultOKColor, Divide);		
		break;
	case RESULT_ID_NONE:
		Color=m_clrUnTest;	
		Color=SystemParam.m_InspectedResultUnTestColor;	
		//Color=CLR_DEFAULT;
		break;
	case RESULT_ID_SKIP:
		Color=m_clrBypass;
		Color = JetAPI::DivideColor(SystemParam.m_InspectedResultSkipColor, Divide);		
		break;
	case RESULT_ID_BYPASS:
		Color=m_clrBypass;		
		Color = JetAPI::DivideColor(SystemParam.m_InspectedResultBypassColor, Divide);		
		break;
	default:
		Color=CLR_DEFAULT;	
		break;
	}
	return Color;
}
//-------------------------------------------------------------------------------------//
void CEditPartNumberListDockPane::CloseProject()
{
	m_ProjectPtr = NULL;
	ClearSubListCtrl(m_wndSubListCtrl);
	ClearPartNumberListCtrl(m_wndPartNumberListCtrl);
}
//-------------------------------------------------------------------------------------//
inline CAOIProject* CEditPartNumberListDockPane::GetActiveProject()
{
	return m_ProjectPtr;
}
//-------------------------------------------------------------------------------------//
inline DISTRICT_ID CEditPartNumberListDockPane::GetActiveDistrictID()
{
	return m_DistrictID;
}
//-------------------------------------------------------------------------------------//
BOOL CEditPartNumberListDockPane::InitPartNumberListCtrl(CThisListCtrl_11 &ListCtrl)
{
	CString str;
	int   nCol = 0;
	int width = 64;
	int width2 = 48;
	int dummy = 0;
	RECT      Rect;	
	const int Align = LVCFMT_CENTER;//LVCFMT_CENTER;LVCFMT_RIGHT	

	ListCtrl.GetClientRect(&Rect);
	const int RectW = 192;
	//const int RectW = Rect.right-Rect.left;	

	width = (RectW-width2-dummy)/1;	
	str = AOIDataDefine.GetPartNumberText();
	ListCtrl.InsertColumn(nCol, str, Align, width);
	nCol ++;
	
	str = AOIDataDefine.GetIndexText();
	ListCtrl.InsertColumn(nCol, str, Align, width2);
	nCol ++;
	return TRUE;
}
//-------------------------------------------------------------------------------------//
BOOL CEditPartNumberListDockPane::BuildPartNumberListCtrl(CThisListCtrl_11 &ListCtrl)
{
	m_ProjectPtr = NULL;
	if ( ListCtrl.GetSafeHwnd() == NULL ) { return FALSE; }
	ClearSubListCtrl(m_wndSubListCtrl);
	ClearPartNumberListCtrl(ListCtrl);	
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL == ProjectPtr ) { return TRUE; }
	m_ProjectPtr = ProjectPtr;
	m_DistrictID = ProjectPtr->GetProjectActDistrictID();

	int                   nItem=0;
	size_t                i=0, j=0, k=0;
	size_t                NComponents = 0;
	size_t                NIndexMaps = 0;
	size_t                NPartNumbers = 0;		
	size_t                PartNumberIndex = 0;
	MODEL_TYPE            ModelType;
	CAOIModel            *ModelPtr = NULL;
	CAOIComponent        *pComponent = NULL;
	
	std::vector<CSortObj>   PartNumberList;
	std::vector<size_t>     IndexMapList;
	std::vector<MODEL_TYPE> ModelTypeList;
	CSortObj                PartNumberNode, *PartNumberNodePtr = NULL;
	CString                 PartNumberS, PartNumberC, str;
	const DISTRICT_ID       DistrictID = GetActiveDistrictID();
	
	PartNumberNode.SetSortMode(SORT_BY_TXT);
	NComponents = ProjectPtr->GetProjectComponentCount();	
	for ( i=0; i<NComponents; i++ )
	{
		pComponent = ProjectPtr->GetProjectComponentPtr(i, false);
		if ( NULL == pComponent ) { continue; }		
		pComponent->SetComponentTempIndexPartNumber(-1);
		if ( pComponent->GetComponentDeleted() == true ) { continue; }
		if ( DistrictID != pComponent->GetComponentDistrictID() ) { continue; }

		PartNumberC = pComponent->GetComponentPartNumber();
		PartNumberC.MakeUpper();

		ModelPtr = pComponent->GetComponentModelPtr();
		ModelType = ModelPtr->GetModelType();

		NPartNumbers = PartNumberList.size();
		for ( j=0; j<NPartNumbers; j++ )
		{
			PartNumberNodePtr = &(PartNumberList[j]);
			if ( PartNumberC == PartNumberNodePtr->GetValueStr() )
			{				
				pComponent->SetComponentTempIndexPartNumber(j);				
				break; 
			}
		}
		if ( j == NPartNumbers )
		{	
			PartNumberNode.SetID(j);
			PartNumberNode.SetValueStr(PartNumberC);
			PartNumberList.push_back(PartNumberNode);	
			IndexMapList.push_back(-1);
			ModelTypeList.push_back(ModelType);			
			pComponent->SetComponentTempIndexPartNumber(j);
		}	
	}

	//依照名稱排序
	std::sort(PartNumberList.begin(), PartNumberList.end());

	//建立引數映射表
	NIndexMaps = IndexMapList.size();
	NPartNumbers = PartNumberList.size();	
	for ( i=0; i<NPartNumbers; i++ )
	{
		PartNumberNodePtr = &(PartNumberList[i]);
		PartNumberIndex =  PartNumberNodePtr->GetID();
		if ( PartNumberIndex >= NIndexMaps ) { continue; }
		IndexMapList[PartNumberIndex] = i;
	}
	//更新至零件列表內
	for ( i=0; i<NComponents; i++ )
	{	
		pComponent = ProjectPtr->GetProjectComponentPtr(i, false);
		if ( NULL == pComponent ) { continue; }
		if ( pComponent->GetComponentDeleted() == true ) { continue; }
		PartNumberIndex = pComponent->GetComponentTempIndexPartNumber();
		if ( PartNumberIndex >= NIndexMaps ) { continue; }		

		PartNumberIndex = IndexMapList[PartNumberIndex] ;
		pComponent->SetComponentTempIndexPartNumber(PartNumberIndex);		
	}	
	//---------------------------------------------------------------------------//		
	nItem=0;
	ListCtrl.SetRedraw(FALSE);
	m_StopPartNumberListBeSelected = TRUE;
	NPartNumbers = PartNumberList.size();
	COLORREF UnsetColor = AOIDataCollect.GetColorModelUnset();
	for ( i=0; i<NPartNumbers; i++ )
	{
		PartNumberNode = PartNumberList[i];

		PartNumberS = PartNumberNode.GetValueStr();
		PartNumberIndex =  PartNumberNode.GetID();

		m_PartNumberList.push_back(PartNumberS);
		ModelType = ModelTypeList[PartNumberIndex];
		if ( MODEL_TYPE_NULL == ModelType )
		{	PartNumberS = PartNumberS+_T(" (**)");	}
		ListCtrl.InsertItem(nItem, PartNumberS);
		ListCtrl.SetItemData(nItem, i);

		if ( MODEL_TYPE_NULL == ModelType )
		{	ListCtrl.SetItemTextColor(nItem, UnsetColor); }

		str.Format(_T("%d"), nItem+1);
		ListCtrl.SetItemText(nItem, 1, str);
		nItem ++;		
	}
	m_StopPartNumberListBeSelected = FALSE;
	ListCtrl.SetRedraw(TRUE);		
	
	if ( nItem > 0 ) 
	{
		pComponent = ProjectPtr->GetProjectActiveComponent();
		if ( NULL == pComponent )
		{	nItem = 0;	}		
		else
		{	nItem = (int)(pComponent->GetComponentTempIndexPartNumber());	}		
		ListCtrl.SetItemState(nItem, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);	
		const int ItemCount = ListCtrl.GetItemCount();
		const int ShowIndex = JetAPI::GetEnsureVisibleIndex(nItem, ItemCount);
		if ( ShowIndex != nItem )
		{	ListCtrl.EnsureVisible(nItem, FALSE); }
		ListCtrl.EnsureVisible(ShowIndex, FALSE);
	}
	return TRUE;
}
//-------------------------------------------------------------------------------------//
BOOL CEditPartNumberListDockPane::UpdatePartNumberListCtrl(CThisListCtrl_11 &ListCtrl, bool bForce)
{
	if ( ListCtrl.GetSafeHwnd() == NULL ) { return FALSE; }
	CAOIProject *ProjectPtr = CEditPartNumberListDockPane::GetActiveProject();
	if ( NULL == ProjectPtr ) { return TRUE; }
	CAOIComponent *pComponent = ProjectPtr->GetProjectActiveComponent();
	if ( NULL == pComponent ) { return TRUE; }
	pComponent = pComponent->GetComponentResultPtr();
	CString strPartNumber = pComponent->GetComponentPartNumber();

	int       i=0;
	const int ItemCount = ListCtrl.GetItemCount();

	strPartNumber.MakeUpper();
	for ( i=0; i<ItemCount; i++ )
	{
		//if ( strPartNumber != ListCtrl.GetItemText(i, 0) ) { continue; }
		if ( strPartNumber != m_PartNumberList[i] ) { continue; }
		break;
	}
	if ( i == ItemCount ) { return TRUE; }
	const int nItem = i;
	ListCtrl.SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED);		
	const int ShowIndex = JetAPI::GetEnsureVisibleIndex(nItem, ItemCount);
	if ( ShowIndex != nItem )
	{	ListCtrl.EnsureVisible(nItem, FALSE); }
	ListCtrl.EnsureVisible(ShowIndex, FALSE);
	BuildSubListCtrl(m_wndSubListCtrl, strPartNumber, bForce);
	return TRUE;
}
//-------------------------------------------------------------------------------------//
BOOL CEditPartNumberListDockPane::ClearPartNumberListCtrl(CThisListCtrl_11 &ListCtrl)
{		
	m_ComponentIndex = -1;
	m_StopPartNumberListBeSelected = TRUE;	
	ListCtrl.DeleteAllItems();
	m_StopPartNumberListBeSelected = FALSE;
	m_PartNumberName = _T("");
	m_PartNumberList.clear();
	return TRUE;
}
//-------------------------------------------------------------------------------------//
BOOL CEditPartNumberListDockPane::RemovePartNumberListItem(CThisListCtrl_11 &ListCtrl)
{
	return TRUE;
}
//-------------------------------------------------------------------------------------//
bool CEditPartNumberListDockPane::ExecPartNumberListMenu(CPoint point)
{
	CMenu menu;		
	UINT menuID = IDR_MENU_LIST_PART_NUMBER;	
	if ( menuID == 0 ) { return false; }
	VERIFY(menu.LoadMenu(menuID));
	AOIDataCollect.SwitchMultiLanguageMenu(menu, menuID);

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);

	CWnd* pWndPopupOwner = this;
	while (pWndPopupOwner->GetStyle() & WS_CHILD)
	{	pWndPopupOwner = pWndPopupOwner->GetParent();	}

	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);	
	return true;
	return true;
}
//-------------------------------------------------------------------------------------//
CEditPartNumberListDockPane::SUB_LIST_MODE  CEditPartNumberListDockPane::GetSubListCtrlMode()
{
	return SUB_LIST_COMPONENT;	
	//return SUB_LIST_WINDOW;
}
//-------------------------------------------------------------------------------------//
BOOL CEditPartNumberListDockPane::InitSubListCtrl(CThisListCtrl_11 &ListCtrl, SUB_LIST_MODE Mode)
{
	CString str;
	int   nCol = 0;
	int width1 = 96;
	int width2 = 96;
	int width3 = 96;
	int dummy = 12;
	RECT      Rect;	
	const int Align = LVCFMT_CENTER;//LVCFMT_CENTER;LVCFMT_RIGHT
	
	JetAPI::ClearListCtrlHeaderList(ListCtrl);

	m_SubCtrlMode = Mode;
	ListCtrl.GetClientRect(&Rect);
	switch ( Mode )
	{
	case SUB_LIST_WINDOW:
		width2 = 64;
		width1 = (Rect.right-Rect.left-width2-dummy)/1;
		str = _T("Window");		
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width1);
		nCol ++;		
		
		str = _T("Result");		
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width2);
		nCol ++;		
		break;
	default:
		width2 = 52;
		width3 = 48;
		width1 = (Rect.right-Rect.left-width2-width3-dummy)/1;
		width1 = 100;
		str = _T("Component");		
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width1);
		nCol ++;		
		
		str = _T("Angle");		
		str = LoadMultiLanguageString(str, str);		
		ListCtrl.InsertColumn(nCol, str, Align, width2);
		nCol ++;

		str = _T("Isolated");		
		str = LoadMultiLanguageString(str, str);		
		ListCtrl.InsertColumn(nCol, str, Align, width3);
		nCol ++;
		break;
	}	
	return TRUE;
}
//-------------------------------------------------------------------------------------//
BOOL CEditPartNumberListDockPane::BuildSubListCtrl(CThisListCtrl_11 &ListCtrl, LPCTSTR strPartNumber, bool bForce)
{	
	if ( NULL == strPartNumber ) { return TRUE; }
	BOOL bOK = TRUE;
	switch ( m_SubCtrlMode )
	{
	case SUB_LIST_WINDOW:
		bOK =  BuildWindowCtrl(ListCtrl, strPartNumber);
		break;
	default:
		bOK =  BuildComponentCtrl(ListCtrl, strPartNumber, bForce);
		break;
	}
	if ( FALSE == bOK ) 
	{	return FALSE; }
	m_PartNumberName = strPartNumber;
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_WND_PROPERTY_WND, WPARAM_UPDATE_WND_SELECTED, NULL);
	return TRUE;
}
//-------------------------------------------------------------------------------------//
BOOL CEditPartNumberListDockPane::BuildWindowCtrl(CThisListCtrl_11 &ListCtrl, LPCTSTR strPartNumber)
{	
	size_t                i=0;
	int                   nItem=0, subIdx=0;
	size_t                NComponents = 0;	
	CAOIComponent        *pComponent = NULL;	
	CString               PartNumberS, PartNumberC, strComponent, str;	
	CAOIProject          *Project = GetActiveProject();

	ClearSubListCtrl(ListCtrl);
	if ( NULL == Project ) { return TRUE; }

	const DISTRICT_ID     DistrictID = GetActiveDistrictID();
	NComponents = Project->GetProjectComponentCount();	
	for ( i=0; i<NComponents; i++ )
	{
		pComponent = Project->GetProjectComponentPtr(i, false);
		if ( NULL == pComponent ) { continue; }				
		if ( pComponent->GetComponentDeleted() == true ) { continue; }
		if ( DistrictID != pComponent->GetComponentDistrictID() ) { continue; }

		PartNumberC = pComponent->GetComponentPartNumber();
		PartNumberC.MakeUpper();
		if ( PartNumberC != strPartNumber ) { continue; }		
		m_ComponentIndex = i;
		break;		
	}
	if ( i == NComponents ) { return TRUE; }
	
	nItem=0;	
	subIdx=0;
	CString      strWindow;
	CAOIWindow  *pWindow = NULL;
	const size_t NWindow = pComponent->GetComponentWindowCount();

	ListCtrl.SetRedraw(FALSE);
	m_StopSubListBeSelected = TRUE;
	for ( i=0; i<NWindow; i++ )
	{
		pWindow = pComponent->GetComponentWindowPtr(i, false);
		if ( NULL == pWindow ) { continue; }
		strWindow.Format(_T("Wnd#%d"), i+1);

		subIdx=0;
		//pWindow
		ListCtrl.InsertItem(nItem, strWindow);
		ListCtrl.SetItemData(nItem, i);
		ListCtrl.SetItemText(nItem, subIdx, strWindow);
		subIdx ++;

		str = _T("0");
		ListCtrl.SetItemText(nItem, subIdx, str);
		subIdx ++;

		nItem ++;
	}
	m_StopSubListBeSelected = FALSE;
	ListCtrl.SetRedraw(TRUE);
	return TRUE;
}
//-------------------------------------------------------------------------------------//
BOOL CEditPartNumberListDockPane::BuildComponentCtrl(CThisListCtrl_11 &ListCtrl, LPCTSTR strPartNumber, bool bForce)
{	
	size_t                i=0;
	bool                  bReBuild=true;
	int                   nItem=0, subIdx=0;
	size_t                NComponents = 0;	
	CAOIComponent        *pComponent = NULL;	
	CString               PartNumberS, PartNumberC, strComponentName, str, ItemText;	
	CAOIProject          *ProjectPtr = GetActiveProject();
	const DISTRICT_ID     DistrictID = GetActiveDistrictID();	

	if ( true==bForce || m_ComponentPartNumber.CompareNoCase(strPartNumber)!=0 || NULL==ProjectPtr )
	{	bReBuild = true;	}
	else
	{	bReBuild = false; }

	if ( true == bReBuild )
	{
		ClearSubListCtrl(ListCtrl);
		if ( NULL == ProjectPtr ) { return TRUE; }
	}
	
	m_ComponentPartNumber=strPartNumber;	
	NComponents = ProjectPtr->GetProjectComponentCount();	
	m_wndPaneBar.SetPartNumberNameEdit(strPartNumber);		

	int          nActiveItem=-1;
	unsigned int ComponentIndex = 0;		
	
	CAOIModel     *ModelPtr=NULL;
	RESULT_ID      ResultID=RESULT_ID_NONE;
	COLORREF       clrItemText=CLR_DEFAULT;
	COLORREF       clrItemTextBk=CLR_DEFAULT;
	unsigned int   PanelIndex=-1;
	unsigned int   BoardIndex=-1;
	CAOIPanel     *PanelPtr = NULL;
	CAOIBoard     *BoardPtr = NULL;
	const int      ItemCount=ListCtrl.GetItemCount();	
	CAOIComponent *ActiveComponentPtr = ProjectPtr->GetProjectActiveComponent();

	if ( false == bReBuild )
	{
		ListCtrl.SetRedraw(FALSE);
		m_StopSubListBeSelected = TRUE;
		for ( i=0; i<ItemCount; i++ )
		{
			nItem = i;
			ComponentIndex = (unsigned int)(ListCtrl.GetItemData(i));
			pComponent = ProjectPtr->GetProjectComponentPtr(ComponentIndex, true);
			if ( NULL == pComponent ) { continue; }				
			ModelPtr = pComponent->GetComponentModelPtr();
			ResultID = ModelPtr->GetModelResultID();
			if ( ActiveComponentPtr != NULL )
			{
				if ( ComponentIndex == ActiveComponentPtr->GetComponentIndex_Project() )
				{	nActiveItem = nItem;	}
			}
			else
			{			
				if ( -1 == nActiveItem )
				{	nActiveItem = nItem;	}
			}

			if ( pComponent->GetComponentSelected() == false )
			{	ListCtrl.SetItemState(nItem, 0, LVIS_SELECTED);	}
			else
			{	ListCtrl.SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED); }

			clrItemText = GetResultColor(ResultID);
			ListCtrl.SetItemTextColor(nItem, clrItemText);
		}
		nItem = ItemCount;
		m_StopSubListBeSelected = FALSE;
		ListCtrl.SetRedraw(TRUE);	
		ListCtrl.RedrawWindow();
	}
	else
	{		
		std::vector<CSortObj> ComponentList;	
		CSortObj              ComponentNode, *ComponentNodePtr = NULL;	
		ComponentNode.SetSortMode(SORT_BY_TXT);
		for ( i=0; i<NComponents; i++ )
		{
			pComponent = ProjectPtr->GetProjectComponentPtr(i, false);
			if ( NULL == pComponent ) { continue; }				
			if ( pComponent->GetComponentDeleted() == true ) { continue; }
			if ( DistrictID != pComponent->GetComponentDistrictID() ) { continue; }

			PartNumberC = pComponent->GetComponentPartNumber();
			PartNumberC.MakeUpper();
			if ( PartNumberC != strPartNumber ) { continue; }
		
			strComponentName = pComponent->GetComponentName();
			ComponentNode.SetID((unsigned int)i);
			ComponentNode.SetValueStr(strComponentName);
			ComponentNode.SetPtr(pComponent);
			ComponentList.push_back(ComponentNode);
		}
		//依照名稱排序
		std::sort(ComponentList.begin(), ComponentList.end());
		const size_t ComponentListSize = ComponentList.size();	
		size_t ItemCount=ComponentListSize;
		const int PageCount=ListCtrl.GetCountPerPage();
		if ( ItemCount > LIST_ITEM_PATCH_ENABLE_COUNT )
		{
			BuildComponentNodeList(ComponentList);
			const int nPageValue=LIST_ITEM_PATCH_N_PAGE_VALUE;
			ItemCount=MIN(PageCount*nPageValue, ComponentListSize);		
		}

		nItem = 0;	
	//	ListCtrl.SetFocus();
		ListCtrl.SetRedraw(FALSE);
		m_StopSubListBeSelected = TRUE;
		for ( i=0; i<ItemCount; i++ )
		{
			ComponentNodePtr = &(ComponentList[i]);

			ComponentIndex = ComponentNodePtr->GetID();
			strComponentName = ComponentNodePtr->GetValueStr();
			pComponent = (CAOIComponent*)ComponentNodePtr->GetPtr();

			PanelPtr = pComponent->GetComponentPanelPtr();
			BoardPtr = pComponent->GetComponentBoardPtr();
			if ( NULL==PanelPtr || NULL==BoardPtr ) 
			{	ItemText =  strComponentName;	}
			else
			{
				PanelIndex = PanelPtr->GetPanelIndex_Project();
				BoardIndex = BoardPtr->GetBoardIndex_Panel();
				ItemText = AOIDataDefine.GetComponentFullNameReverse(PanelIndex, BoardIndex, strComponentName);
			}

			ModelPtr = pComponent->GetComponentModelPtr();
			ResultID = ModelPtr->GetModelResultID();
			if ( ActiveComponentPtr != NULL )
			{
				if ( ComponentIndex == ActiveComponentPtr->GetComponentIndex_Project() )
				{	nActiveItem = nItem;	}
			}
			else
			{			
				if ( -1 == nActiveItem )
				{	nActiveItem = nItem;	}
			}

			subIdx=0;
			ListCtrl.InsertItem(nItem, ItemText);
			ListCtrl.SetItemData(nItem, ComponentIndex);

			if ( pComponent->GetComponentSelected() == true )
			{	ListCtrl.SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED); }

			clrItemText = GetResultColor(ResultID);
			ListCtrl.SetItemTextColor(nItem, clrItemText);

			//ListCtrl
			ListCtrl.SetItemText(nItem, subIdx, ItemText);
			subIdx ++;

			str.Format(_T("%.2f"), pComponent->GetComponentAngle());
			ListCtrl.SetItemText(nItem, subIdx, str);
			subIdx ++;

			if ( pComponent->GetComponentModelIsolated() == true ) 
			{	str = _T("Y");	}
			else
			{	str = _T(""); }
			ListCtrl.SetItemText(nItem, subIdx, str);
			subIdx ++;

			nItem ++;
		}	
		m_StopSubListBeSelected = FALSE;
		ListCtrl.SetRedraw(TRUE);	
		UpdateSubListCtrlTitle(ListCtrl, nItem);
		if ( nItem>0 && -1==nActiveItem )
		{	nActiveItem = 0; }
	}

	m_MoveToComponent = false;
	pComponent = ProjectPtr->GetProjectActiveComponent();
	AOIDataCollect.CloseActiveComponent(pComponent);	
	if ( nActiveItem >= 0 )
	{	
		ListCtrl.SetItemState(nActiveItem, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);				
		const int ItemCount = ListCtrl.GetItemCount();
		const int ShowIndex = JetAPI::GetEnsureVisibleIndex(nActiveItem, ItemCount);
		if ( ShowIndex != nActiveItem )
		{	ListCtrl.EnsureVisible(nActiveItem, FALSE); }
		ListCtrl.EnsureVisible(ShowIndex, FALSE);
	}
	m_MoveToComponent = true;		
	return TRUE;
}
//-------------------------------------------------------------------------------------//
BOOL CEditPartNumberListDockPane::ClearSubListCtrl(CThisListCtrl_11 &ListCtrl)
{
	ClearComponentNodeList();
	m_StopSubListBeSelected = TRUE;	
	ListCtrl.DeleteAllItems();
	m_ComponentPartNumber=_T("");
	m_wndPaneBar.SetPartNumberNameEdit(_T(""));
	m_StopSubListBeSelected = FALSE;
	return TRUE;
}
//-------------------------------------------------------------------------------------//
bool CEditPartNumberListDockPane::UpdateSubListCtrlTitle(CThisListCtrl_11 &ListCtrl, size_t nItem)
{
	CString str;	
	CString Name;
	LVCOLUMN col;
	TCHAR buf[64]=_T("");
	const int NodeCount=(int)(m_ComponentNodeList.size());

	::memset(&col, 0x00, sizeof(col));
	str = _T("Component");
	str = LoadMultiLanguageString(str, str);	
	if ( 0==NodeCount || nItem==NodeCount )
	{	::_stprintf(buf, _T("%s [%d]"), str, nItem);	}
	else
	{	::_stprintf(buf, _T("%s [%d/%d]"), str, nItem, NodeCount);	}	
	col.mask = LVCF_TEXT;//LVCF_WIDTH;
	col.pszText = buf;	
	ListCtrl.SetColumn(0, &col);	
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditPartNumberListDockPane::ClearComponentNodeList()
{
	m_ComponentNodeList.clear();
}
//-------------------------------------------------------------------------------------//
bool CEditPartNumberListDockPane::BuildNextComponentCtrl()
{
	CAOIProject          *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }
	std::vector<CSortObj> &NodeList=m_ComponentNodeList;
	const int NodeCount=(int)(NodeList.size());
	if ( 0 == NodeCount ) { return true; }
	CThisListCtrl_11 &ListCtrl=m_wndSubListCtrl;
	const int ItemCount = ListCtrl.GetItemCount();
	if ( ItemCount >= NodeCount )
	{
		NodeList.clear();
		return true;
	}
	if ( ListCtrl.IsItemVisible(ItemCount-1) == FALSE )
	{	return true; }

	CString str;
	CString ItemText;
	int subIdx=0;
	int nActiveItem=-1;		
	int nItem=ItemCount;
	RESULT_ID ResultID;
	COLORREF clrItemText=0;
	CString strComponentName;
	CAOIModel *ModelPtr=NULL;
	CAOIPanel *PanelPtr=NULL;
	CAOIBoard *BoardPtr=NULL;
	CAOIComponent *pComponent=NULL;		
	unsigned int PanelIndex=0;
	unsigned int BoardIndex=0;
	unsigned int ComponentIndex=0;
	const int PageCount=ListCtrl.GetCountPerPage();
	const int nPageValue=LIST_ITEM_PATCH_N_PAGE_VALUE;
	const int NextItemCount=MIN(ItemCount+(PageCount*nPageValue), NodeCount);	
	CAOIComponent *ActiveComponentPtr = ProjectPtr->GetProjectActiveComponent();	

	ListCtrl.SetRedraw(FALSE);
	m_StopSubListBeSelected = TRUE;
	for ( int i=ItemCount; i<NextItemCount; i++ )
	{		
		CSortObj *ComponentNodePtr = &(NodeList[i]);
		ComponentIndex = ComponentNodePtr->GetID();
		strComponentName = ComponentNodePtr->GetValueStr();
		pComponent = (CAOIComponent*)ComponentNodePtr->GetPtr();

		PanelPtr = pComponent->GetComponentPanelPtr();
		BoardPtr = pComponent->GetComponentBoardPtr();
		if ( NULL==PanelPtr || NULL==BoardPtr ) 
		{	ItemText =  strComponentName;	}
		else
		{
			PanelIndex = PanelPtr->GetPanelIndex_Project();
			BoardIndex = BoardPtr->GetBoardIndex_Panel();
			ItemText = AOIDataDefine.GetComponentFullNameReverse(PanelIndex, BoardIndex, strComponentName);
		}

		ModelPtr = pComponent->GetComponentModelPtr();
		ResultID = ModelPtr->GetModelResultID();
		if ( ActiveComponentPtr != NULL )
		{
			if ( ComponentIndex == ActiveComponentPtr->GetComponentIndex_Project() )
			{	nActiveItem = nItem;	}
		}
		else
		{			
			if ( -1 == nActiveItem )
			{	nActiveItem = nItem;	}
		}

		subIdx=0;
		ListCtrl.InsertItem(nItem, ItemText);
		ListCtrl.SetItemData(nItem, ComponentIndex);

		if ( pComponent->GetComponentSelected() == true )
		{	ListCtrl.SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED); }

		clrItemText = GetResultColor(ResultID);
		ListCtrl.SetItemTextColor(nItem, clrItemText);

		//ListCtrl
		ListCtrl.SetItemText(nItem, subIdx, ItemText);
		subIdx ++;

		str.Format(_T("%.2f"), pComponent->GetComponentAngle());
		ListCtrl.SetItemText(nItem, subIdx, str);
		subIdx ++;

		if ( pComponent->GetComponentModelIsolated() == true ) 
		{	str = _T("Y");	}
		else
		{	str = _T(""); }
		ListCtrl.SetItemText(nItem, subIdx, str);
		subIdx ++;

		nItem ++;
	}	
	m_StopSubListBeSelected = FALSE;
	ListCtrl.SetRedraw(TRUE);

	UpdateSubListCtrlTitle(ListCtrl, nItem);

	m_MoveToComponent = false;	
	if ( nActiveItem >= 0 )
	{	
		ListCtrl.SetItemState(nActiveItem, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);				
		const int ItemCount = ListCtrl.GetItemCount();
		const int ShowIndex = JetAPI::GetEnsureVisibleIndex(nActiveItem, ItemCount);
		if ( ShowIndex != nActiveItem )
		{	ListCtrl.EnsureVisible(nActiveItem, FALSE); }
		ListCtrl.EnsureVisible(ShowIndex, FALSE);
	}
	m_MoveToComponent = true;	
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditPartNumberListDockPane::BuildComponentNodeList(const std::vector<CSortObj> &SortList)
{
	ClearComponentNodeList();
	m_ComponentNodeList = SortList;	
	return;
}
//-------------------------------------------------------------------------------------//
void CEditPartNumberListDockPane::ExecWindowModeChk() 
{
	// TODO: Add your control notification handler code here	
	SUB_LIST_MODE SubListMode = GetSubListCtrlMode();	
	if ( SubListMode == m_SubCtrlMode ) { return; }
	CEditPartNumberListDockPane::InitSubListCtrl(m_wndSubListCtrl, SubListMode);

	const int nItem = m_wndPartNumberListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if ( nItem < 0 ) 
	{	
		ClearSubListCtrl(m_wndSubListCtrl);	
		return ;
	}

	CString strPartNumber = m_wndPartNumberListCtrl.GetItemText(nItem, 0);
	strPartNumber = m_PartNumberList[nItem];
	BuildSubListCtrl(m_wndSubListCtrl, strPartNumber);	
}
//-------------------------------------------------------------------------------------//
int CEditPartNumberListDockPane::FindItemPreious_Any(CThisListCtrl_11 &ListCtrl)
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return -1; }			

	int        i=0;
	size_t     Index=0;
	int        nItemNext=-1;
	CString    PartNumber;
	bool       bFind=false;		
	const int  nItem = ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	const int  ItemCount = ListCtrl.GetItemCount();
	const int  PartNumberCount = (int)(m_PartNumberList.size());	

	bFind = false;
	nItemNext=-1;
	for ( i=nItem-1; i>=0; i-- )
	{		
		Index = ListCtrl.GetItemData(i);
		if ( Index >= PartNumberCount ) { continue; }
		PartNumber = m_PartNumberList[Index];				
		nItemNext = i;
		break;
	}
	if ( -1 == nItemNext )
	{
		for ( i=ItemCount-1; i>nItem; i-- )
		{
			Index = ListCtrl.GetItemData(i);
			if ( Index >= PartNumberCount ) { continue; }
			PartNumber = m_PartNumberList[Index];			
			nItemNext = i;
			break;
		}
	}
	return nItemNext;	
}
//-------------------------------------------------------------------------------------//
int CEditPartNumberListDockPane::FindItemPreious_Set(CThisListCtrl_11 &ListCtrl)
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return -1; }		

	int        i=0;
	size_t     Index=0;
	int        nItemNext=-1;	
	CString    ModelName;
	CString    PartNumber;
	bool       bFind=false;		
	CAOIModel *ModelPtr = NULL;
	CAOIComponent *ComponentPtr = NULL;
	const int  nItem = ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	const int  ItemCount = ListCtrl.GetItemCount();
	const int  PartNumberCount = (int)(m_PartNumberList.size());	

	bFind = false;
	nItemNext=-1;
	for ( i=nItem-1; i>=0; i-- )
	{
		Index = ListCtrl.GetItemData(i);
		if ( Index >= PartNumberCount ) { continue; }
		PartNumber = m_PartNumberList[Index];
		ComponentPtr = ProjectPtr->GetProjectComponentPtrByPartNumber(PartNumber);
		if ( NULL == ComponentPtr ) { continue; }
		ModelName = ComponentPtr->GetComponentModelName();
		ModelPtr = ProjectPtr->GetProjectModelPtrByModelName(ModelName);
		if ( NULL == ModelPtr )
		{	continue;	}
		if ( ModelPtr->GetModelUnsetState() == true )
		{	continue;	}
		nItemNext = i;
		break;
	}
	if ( -1 == nItemNext )
	{
		for ( i=ItemCount-1; i>nItem; i-- )
		{
			Index = ListCtrl.GetItemData(i);
			if ( Index >= PartNumberCount ) { continue; }
			PartNumber = m_PartNumberList[Index];
			ComponentPtr = ProjectPtr->GetProjectComponentPtrByPartNumber(PartNumber);
			if ( NULL == ComponentPtr ) { continue; }
			ModelName = ComponentPtr->GetComponentModelName();
			ModelPtr = ProjectPtr->GetProjectModelPtrByModelName(ModelName);
			if ( NULL == ModelPtr )
			{	continue;	}
			if ( ModelPtr->GetModelUnsetState() == true )
			{	continue;	}
			nItemNext = i;
			break;
		}
	}
	return nItemNext;	
}
//-------------------------------------------------------------------------------------//
int CEditPartNumberListDockPane::FindItemPreious_UnSet(CThisListCtrl_11 &ListCtrl)
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return -1; }			

	int        i=0;
	size_t     Index=0;
	int        nItemNext=-1;
	CString    ModelName;
	CString    PartNumber;
	bool       bFind=false;		
	CAOIModel *ModelPtr = NULL;
	CAOIComponent *ComponentPtr = NULL;
	const int  nItem = ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	const int  ItemCount = ListCtrl.GetItemCount();
	const int  PartNumberCount = (int)(m_PartNumberList.size());	

	bFind = false;
	nItemNext=-1;
	for ( i=nItem-1; i>=0; i-- )
	{
		Index = ListCtrl.GetItemData(i);
		if ( Index >= PartNumberCount ) { continue; }
		PartNumber = m_PartNumberList[Index];
		ComponentPtr = ProjectPtr->GetProjectComponentPtrByPartNumber(PartNumber);
		if ( NULL == ComponentPtr ) { continue; }
		ModelName = ComponentPtr->GetComponentModelName();
		ModelPtr = ProjectPtr->GetProjectModelPtrByModelName(ModelName);
		if ( NULL == ModelPtr )
		{
			nItemNext = i;
			break;
		}
		if ( ModelPtr->GetModelUnsetState() == true )
		{
			nItemNext = i;
			break;
		}
	}
	if ( -1 == nItemNext )
	{
		for ( i=ItemCount-1; i>nItem; i-- )
		{
			Index = ListCtrl.GetItemData(i);
			if ( Index >= PartNumberCount ) { continue; }
			PartNumber = m_PartNumberList[Index];
			ComponentPtr = ProjectPtr->GetProjectComponentPtrByPartNumber(PartNumber);
			if ( NULL == ComponentPtr ) { continue; }
			ModelName = ComponentPtr->GetComponentModelName();
			ModelPtr = ProjectPtr->GetProjectModelPtrByModelName(ModelName);
			if ( NULL == ModelPtr )
			{
				nItemNext = i;
				break;
			}
			if ( ModelPtr->GetModelUnsetState() == true )
			{
				nItemNext = i;
				break;
			}
		}
	}
	return nItemNext;	
}
//-------------------------------------------------------------------------------------//
void CEditPartNumberListDockPane::OnSwitchPreiousBtn()
{
	int nItemNext = -1;
	SWITCH_MODEL_ITEM_MODE SwitchItemMode;
	CThisListCtrl_11 &ListCtrl = m_wndPartNumberListCtrl;
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }

	SwitchItemMode = (SWITCH_MODEL_ITEM_MODE)(JetAPI::GetComboxCurSelData(m_wndPaneBar.m_SwitchModeCombox));
	switch ( SwitchItemMode )
	{
	case SWITCH_MODEL_ITEM_SET:
		nItemNext = FindItemPreious_Set(ListCtrl);
		break;
	case SWITCH_MODEL_ITEM_UNSET:
		nItemNext = FindItemPreious_UnSet(ListCtrl);
		break;
	default:
		nItemNext = FindItemPreious_Any(ListCtrl);
		break;
	}
	if ( -1 == nItemNext )
	{	return ; }
	
	m_MoveToComponent = false;
	m_SwitchPartNumberBtn = true;
	ListCtrl.SetItemState(nItemNext, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);		
	const int ItemCount = ListCtrl.GetItemCount();
	const int ShowIndex = JetAPI::GetEnsureVisibleIndex(nItemNext, ItemCount);
	if ( ShowIndex != nItemNext )
	{	ListCtrl.EnsureVisible(nItemNext, FALSE); }
	ListCtrl.EnsureVisible(ShowIndex, FALSE);
	m_MoveToComponent = true;
	m_SwitchPartNumberBtn = false;
	CAOIComponent *ComponentPtr = ProjectPtr->GetProjectActiveComponent();
	AOIDataCollect.MoveStageToComponentOrField(ComponentPtr, true);
}
//-------------------------------------------------------------------------------------//
int CEditPartNumberListDockPane::FindItemNext_Any(CThisListCtrl_11 &ListCtrl)
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return -1; }			

	int        i=0;
	size_t     Index=0;
	int        nItemNext=-1;
	CString    PartNumber;
	bool       bFind=false;		
	const int  nItem = ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	const int  ItemCount = ListCtrl.GetItemCount();
	const int  PartNumberCount = (int)(m_PartNumberList.size());	

	bFind = false;
	nItemNext=-1;
	for ( i=nItem+1; i<ItemCount; i++ )
	{
		Index = ListCtrl.GetItemData(i);
		if ( Index >= PartNumberCount ) { continue; }
		PartNumber = m_PartNumberList[Index];		
		nItemNext = i;
		break;
	}
	if ( -1 == nItemNext )
	{
		for ( i=0; i<nItem; i++ )
		{
			Index = ListCtrl.GetItemData(i);
			if ( Index >= PartNumberCount ) { continue; }
			PartNumber = m_PartNumberList[Index];			
			nItemNext = i;
			break;
		}
	}
	return nItemNext;	
}
//-------------------------------------------------------------------------------------//
int CEditPartNumberListDockPane::FindItemNext_Set(CThisListCtrl_11 &ListCtrl)
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return -1; }			

	int        i=0;
	size_t     Index=0;
	int        nItemNext=-1;
	CString    ModelName;
	CString    PartNumber;
	bool       bFind=false;		
	CAOIModel *ModelPtr = NULL;
	CAOIComponent *ComponentPtr = NULL;
	const int  nItem = ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	const int  ItemCount = ListCtrl.GetItemCount();
	const int  PartNumberCount = (int)(m_PartNumberList.size());	

	bFind = false;
	nItemNext=-1;
	for ( i=nItem+1; i<ItemCount; i++ )
	{
		Index = ListCtrl.GetItemData(i);
		if ( Index >= PartNumberCount ) { continue; }
		PartNumber = m_PartNumberList[Index];
		ComponentPtr = ProjectPtr->GetProjectComponentPtrByPartNumber(PartNumber);
		if ( NULL == ComponentPtr ) { continue; }
		ModelName = ComponentPtr->GetComponentModelName();
		ModelPtr = ProjectPtr->GetProjectModelPtrByModelName(ModelName);
		if ( NULL == ModelPtr )
		{	continue;	}
		if ( ModelPtr->GetModelUnsetState() == true )
		{	continue;	}
		nItemNext = i;
		break;
	}
	if ( -1 == nItemNext )
	{
		for ( i=0; i<nItem; i++ )
		{
			Index = ListCtrl.GetItemData(i);
			if ( Index >= PartNumberCount ) { continue; }
			PartNumber = m_PartNumberList[Index];
			ComponentPtr = ProjectPtr->GetProjectComponentPtrByPartNumber(PartNumber);
			if ( NULL == ComponentPtr ) { continue; }
			ModelName = ComponentPtr->GetComponentModelName();
			ModelPtr = ProjectPtr->GetProjectModelPtrByModelName(ModelName);
			if ( NULL == ModelPtr )
			{	continue;	}
			if ( ModelPtr->GetModelUnsetState() == true )
			{	continue;	}
			nItemNext = i;
			break;
		}
	}
	return nItemNext;	
}
//-------------------------------------------------------------------------------------//
int CEditPartNumberListDockPane::FindItemNext_UnSet(CThisListCtrl_11 &ListCtrl)
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return -1; }

	int        i=0;
	size_t     Index=0;
	int        nItemNext=-1;
	CString    ModelName;
	CString    PartNumber;
	bool       bFind=false;		
	CAOIModel *ModelPtr = NULL;
	CAOIComponent *ComponentPtr = NULL;
	const int  nItem = ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	const int  ItemCount = ListCtrl.GetItemCount();
	const int  PartNumberCount = (int)(m_PartNumberList.size());	

	bFind = false;
	nItemNext=-1;
	for ( i=nItem+1; i<ItemCount; i++ )
	{
		Index = ListCtrl.GetItemData(i);
		if ( Index >= PartNumberCount ) { continue; }
		PartNumber = m_PartNumberList[Index];
		ComponentPtr = ProjectPtr->GetProjectComponentPtrByPartNumber(PartNumber);
		if ( NULL == ComponentPtr ) { continue; }
		ModelName = ComponentPtr->GetComponentModelName();
		ModelPtr = ProjectPtr->GetProjectModelPtrByModelName(ModelName);
		if ( NULL == ModelPtr )
		{
			nItemNext = i;
			break;
		}
		if ( ModelPtr->GetModelUnsetState() == true )
		{
			nItemNext = i;
			break;
		}
	}
	if ( -1 == nItemNext )
	{
		for ( i=0; i<nItem; i++ )
		{
			Index = ListCtrl.GetItemData(i);
			if ( Index >= PartNumberCount ) { continue; }
			PartNumber = m_PartNumberList[Index];
			ComponentPtr = ProjectPtr->GetProjectComponentPtrByPartNumber(PartNumber);
			if ( NULL == ComponentPtr ) { continue; }
			ModelName = ComponentPtr->GetComponentModelName();
			ModelPtr = ProjectPtr->GetProjectModelPtrByModelName(ModelName);
			if ( NULL == ModelPtr )
			{
				nItemNext = i;
				break;
			}
			if ( ModelPtr->GetModelUnsetState() == true )
			{
				nItemNext = i;
				break;
			}
		}
	}
	return nItemNext;	
}
//-------------------------------------------------------------------------------------//
void CEditPartNumberListDockPane::OnSwitchNextBtn()
{
	int nItemNext = -1;
	SWITCH_MODEL_ITEM_MODE NextModelType;
	CThisListCtrl_11 &ListCtrl = m_wndPartNumberListCtrl;
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }

	NextModelType = (SWITCH_MODEL_ITEM_MODE)(JetAPI::GetComboxCurSelData(m_wndPaneBar.m_SwitchModeCombox));

	switch ( NextModelType )
	{
	case SWITCH_MODEL_ITEM_SET:
		nItemNext = FindItemNext_Set(ListCtrl);
		break;
	case SWITCH_MODEL_ITEM_UNSET:
		nItemNext = FindItemNext_UnSet(ListCtrl);
		break;
	default:
		nItemNext = FindItemNext_Any(ListCtrl);
		break;
	}
	if ( -1 == nItemNext ) { return; }	
	m_MoveToComponent = false;
	m_SwitchPartNumberBtn = true;
	ListCtrl.SetItemState(nItemNext, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);	
	const int ItemCount = ListCtrl.GetItemCount();
	const int ShowIndex = JetAPI::GetEnsureVisibleIndex(nItemNext, ItemCount);
	if ( ShowIndex != nItemNext )
	{	ListCtrl.EnsureVisible(nItemNext, FALSE); }
	ListCtrl.EnsureVisible(ShowIndex, FALSE);

	m_MoveToComponent = true;
	m_SwitchPartNumberBtn = false;
	CAOIComponent *ComponentPtr = ProjectPtr->GetProjectActiveComponent();
	AOIDataCollect.MoveStageToComponentOrField(ComponentPtr, true);
}
//-------------------------------------------------------------------------------------//
void CEditPartNumberListDockPane::OnPartNumberBypass()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	
	CThisListCtrl_11 &ListCtrl = m_wndPartNumberListCtrl;
	const int nItem = ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if ( nItem < 0 ) { return; }
	if ( AOIDataCollect.OperateLevelEditFuncBypassComponent() == false )	{	return ; }

	CString PartNumber;
	PartNumber = m_PartNumberList[nItem];
	PartNumber.MakeUpper();
	ProjectPtr->SelectProjectAllComponents(false);
	ProjectPtr->SelectProjectComponentsByPartNumber(PartNumber, false);	
	ProjectPtr->SwitchProjectComponentBypassed();	
	LogOperCtrl.SaveLogProjectComponentSelectedBypassed(ProjectPtr);
	UpdateComponentListState(m_wndSubListCtrl);
}
//-------------------------------------------------------------------------------------//
void CEditPartNumberListDockPane::OnPartNumberSearch()
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	
	CThisListCtrl_11 &ListCtrl = m_wndPartNumberListCtrl;
	const int nItem = ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if ( nItem < 0 ) { return; }
	const size_t PartNumberCount=m_PartNumberList.size();
	size_t Index = m_wndPartNumberListCtrl.GetItemData(nItem);
	if ( Index >= PartNumberCount ) { return; }

	int          i=0;
	CString      str, str2;
	CString      PartNumber;
	CString      strValue;
	CString      strLabel;
	CString      strCaption;	
	CInputBoxWnd InputBox;	

	PartNumber = m_PartNumberList[Index];
	strCaption = _T("Input Part Number Wnd");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strLabel = AOIDataDefine.GetModelText();
	strValue = PartNumber;
	InputBox.SetParam1(strCaption, strLabel, strValue);
	if ( InputBox.DoModal() == IDCANCEL ) 
	{	return ; }	
	
	int    nNextItem=-1;
	strValue = InputBox.m_DataEdit1;
	for ( i=nItem+1; i<PartNumberCount; i++ )
	{
		PartNumber = m_PartNumberList[i];
		if ( JetAPI::FindTextInString(strValue, PartNumber) == false )
		{	continue; }
		nNextItem = i;
		break;
	}	
	if ( -1 == nNextItem )
	{
		for ( i=0; i<PartNumberCount; i++ )
		{
			PartNumber = m_PartNumberList[i];
			if ( JetAPI::FindTextInString(strValue, PartNumber) == false )
			{	continue; }
			nNextItem = i;
			break;
		}
	}
	if ( -1 == nNextItem )
	{
		str = _T("Can not find part number");
		str = LoadMultiLanguageString(str, str);
		strValue = InputBox.m_DataEdit1;	
		str2.Format(_T("%s [%s]"), str, strValue);
		JetAPI::ShowMessageBox(str2);
		return;
	}
	nNextItem = JetAPI::GetListCtrlItemByData(ListCtrl, nNextItem);
	if ( -1 == nNextItem )
	{	return; }

	ListCtrl.SetItemState(nNextItem, LVIS_SELECTED, LVIS_SELECTED);	
	const int ItemCount = ListCtrl.GetItemCount();
	const int ShowIndex = JetAPI::GetEnsureVisibleIndex(nNextItem, ItemCount);
	if ( ShowIndex != nNextItem )
	{	ListCtrl.EnsureVisible(nNextItem, FALSE); }
	ListCtrl.EnsureVisible(ShowIndex, FALSE);

	BuildComponentCtrl(m_wndSubListCtrl, PartNumber);
}
//-------------------------------------------------------------------------------------//
void CEditPartNumberListDockPane::OnPartNumberRename()
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	
	CThisListCtrl_11 &ListCtrl = m_wndPartNumberListCtrl;
	const int nItem = ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if ( nItem < 0 ) { return; }	
	const size_t PartNumberCount=m_PartNumberList.size();
	size_t Index = ListCtrl.GetItemData(nItem);
	if ( Index >= PartNumberCount ) { return; }

	CString PartNumber;
	PartNumber = m_PartNumberList[Index];
	PartNumber.MakeUpper();

	CString      strValue;
	CString      strLabel;
	CString      strCaption;
	CInputBoxWnd InputBox;
	strCaption = _T("Input Part Number Wnd");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strLabel = AOIDataDefine.GetPartNumberText();
	strValue = PartNumber;
	InputBox.SetParam1(strCaption, strLabel, strValue);
	while ( true ) 
	{
		if ( InputBox.DoModal() == IDCANCEL ) 
		{	return ;  }
		strValue = InputBox.m_DataEdit1;
		strValue.MakeUpper();
		strValue.TrimLeft();//剔除左邊
		strValue.TrimRight();//剔除右邊
		if ( strValue.GetLength() == 0 ) 
		{	continue; }
		break;
	};
	PartNumber.MakeUpper();
	ProjectPtr->SelectProjectAllComponents(false);
	ProjectPtr->SelectProjectComponentsByPartNumber(PartNumber, false);	
	ProjectPtr->SetProjectComponentSelectedPartNumber(strValue);
	LogOperCtrl.SaveLogProjectComponentSelectedPartNumber(ProjectPtr, strValue);
	m_PartNumberList[nItem] = strValue;
	ListCtrl.SetItemText(nItem, 0, strValue);
	UpdateComponentListState(m_wndSubListCtrl);
}
//-------------------------------------------------------------------------------------//
void CEditPartNumberListDockPane::OnPartNumberDelete()
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	
	CThisListCtrl_11 &ListCtrl = m_wndPartNumberListCtrl;
	const int nItem = ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if ( nItem < 0 ) { return; }	
	const size_t PartNumberCount=m_PartNumberList.size();
	size_t Index = ListCtrl.GetItemData(nItem);
	if ( Index >= PartNumberCount ) { return; }
	if ( AOIDataCollect.OperateLevelEditFuncDelComponent() == false )	{	return ; }

	CString PartNumber;
	PartNumber = m_PartNumberList[Index];
	PartNumber.MakeUpper();

	CString      str;
	CString      str2;
	str2 = _T("Do you want to delete the components linked the partnumber");
	str2 = LoadMultiLanguageString(str2, str2);
	str.Format(_T("%s [%s] ?"), str2, PartNumber);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO )
	{	return; }
	AOIDataCollect.ReleaseModelUniFrameList();
	ProjectPtr->SelectProjectAllComponents(false);
	ProjectPtr->SelectProjectComponentsByPartNumber(PartNumber, false);	
	LogOperCtrl.SaveLogProjectComponentSelectedDelete(ProjectPtr);
	ProjectPtr->DeleteProjectComponentSelected();
	ListCtrl.DeleteItem(nItem);
	UpdateComponentListState(m_wndSubListCtrl);
	AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_UPDATE, (LPARAM)this);
}
//-------------------------------------------------------------------------------------//
void CEditPartNumberListDockPane::OnPartNumberSelectAll()
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	
	CThisListCtrl_11 &ListCtrl = m_wndPartNumberListCtrl;
	const int nItem = ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if ( nItem < 0 ) { return; }
	CString PartNumber;
	PartNumber = m_PartNumberList[nItem];			
	
	ProjectPtr->SelectProjectAllComponents(false);
	ProjectPtr->SelectProjectComponentsByPartNumber(PartNumber, false);

	CAOIComponent *ComponentActPtr = ProjectPtr->GetProjectActiveComponent();
	if ( NULL != ComponentActPtr )
	{
		CString PartNumber = ComponentActPtr->GetComponentPartNumber();
		if ( m_PartNumberName.CompareNoCase(PartNumber) != 0 ) 
		{	ComponentActPtr = NULL; }
	}
	if ( NULL == ComponentActPtr )
	{	
		ComponentActPtr = ProjectPtr->GetProjectComponentPtrBySelected();
		if ( NULL != ComponentActPtr ) 
		{	ProjectPtr->SetProjectActiveComponent(ComponentActPtr);	}
	}
	UpdateComponentListState(m_wndSubListCtrl);
	m_wndSubListCtrl.SetFocus();

	AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_PART_SELECTED, NULL);
}
//-------------------------------------------------------------------------------------//
BOOL CEditPartNumberListDockPane::ShowSubListSelected(CThisListCtrl_11 &ListCtrl)
{
	switch ( m_SubCtrlMode )
	{
	case SUB_LIST_WINDOW:
		return ShowWindowListSelected(ListCtrl);
		break;
	default:
		return ShowComponentListSelected(ListCtrl);
		break;
	}
	return TRUE;
}
//-------------------------------------------------------------------------------------//
BOOL CEditPartNumberListDockPane::ShowWindowListSelected(CThisListCtrl_11 &ListCtrl)
{
	return TRUE;
}
//-------------------------------------------------------------------------------------//
BOOL CEditPartNumberListDockPane::ShowComponentListSelected(CThisListCtrl_11 &ListCtrl)
{
	return TRUE;
}
//-------------------------------------------------------------------------------------//
BOOL CEditPartNumberListDockPane::UpdateComponentListState(CThisListCtrl_11 &ListCtrl)
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return TRUE; }
	int             i=0;		
	int             subIdx=0;
	unsigned int    PanelIndex=0;
	unsigned int    BoardIndex=0;
	unsigned int    ComponentIndex=0;
	CString         str;
	CString         strComponentName;
	CAOIPanel      *PanelPtr = NULL;
	CAOIBoard      *BoardPtr = NULL;
	CAOIComponent  *pComponent = NULL;	
	const int ItemCount = ListCtrl.GetItemCount();
	
	CAOIModel     *ModelPtr=NULL;
	RESULT_ID      ResultID=RESULT_ID_NONE;
	COLORREF       clrItemText=CLR_DEFAULT;
	COLORREF       clrItemTextBk=CLR_DEFAULT;
	CAOIComponent *ActiveComponentPtr = ProjectPtr->GetProjectActiveComponent();
	ListCtrl.SetRedraw(FALSE);	
	m_StopSubListBeSelected = TRUE;
	for ( i=0; i<ItemCount; i++ )
	{
		ComponentIndex = ListCtrl.GetItemData(i);
		pComponent = ProjectPtr->GetProjectComponentPtr(ComponentIndex, true);
		if ( NULL == pComponent )  { continue; }		
		PanelPtr = pComponent->GetComponentPanelPtr();
		if ( NULL == PanelPtr ) { continue; }
		BoardPtr = pComponent->GetComponentBoardPtr();
		if ( NULL == BoardPtr ) { continue; }

		strComponentName = pComponent->GetComponentName();		
		ModelPtr = pComponent->GetComponentModelPtr();
		ResultID = ModelPtr->GetModelResultID();		

		subIdx=0;

		if ( pComponent->GetComponentSelected() == false ) 
		{	ListCtrl.SetItemState(i, 0, LVIS_SELECTED); }
		else
		{	ListCtrl.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED); }

		clrItemText = GetResultColor(ResultID);
		ListCtrl.SetItemTextColor(i, clrItemText);

		PanelIndex = PanelPtr->GetPanelIndex_Project();
		BoardIndex = BoardPtr->GetBoardIndex_Panel();
		str = AOIDataDefine.GetComponentFullNameReverse(PanelIndex, BoardIndex, strComponentName);
		ListCtrl.SetItemText(i, subIdx, str);
		subIdx ++;

		str.Format(_T("%.2f"), pComponent->GetComponentAngle());
		ListCtrl.SetItemText(i, subIdx, str);
		subIdx ++;		

		if ( pComponent->GetComponentModelIsolated() == true ) 
		{	str = _T("Y");	}
		else
		{	str = _T(""); }
		ListCtrl.SetItemText(i, subIdx, str);
		subIdx ++;
	}
	m_StopSubListBeSelected = FALSE;
	ListCtrl.SetRedraw(TRUE);	
	return TRUE;
}
//-------------------------------------------------------------------------------------//
BOOL CEditPartNumberListDockPane::ExecComponentListMenu(CPoint point)
{
	CMenu menu;		
	UINT menuID = IDR_MENU_TREE_COMPONENT;	
	if ( menuID == 0 ) { return FALSE; }
	VERIFY(menu.LoadMenu(menuID));
	AOIDataCollect.SwitchMultiLanguageMenu(menu, menuID);

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);

	CWnd* pWndPopupOwner = this;
	while (pWndPopupOwner->GetStyle() & WS_CHILD)
	{	pWndPopupOwner = pWndPopupOwner->GetParent();	}

	UpdateComponentListState(m_wndSubListCtrl);
	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	return TRUE;
}
//-------------------------------------------------------------------------------------//
void CEditPartNumberListDockPane::OnComponentOffset()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }

	CString      strCaption;
	CString      strLabelX, strLabelY;
	CString      strX = _T("0");
	CString      strY = _T("0");
	CInputBoxWnd InputBox;
	const double DBL_Precesion = DBL_PRECISION;
	const size_t ComponentSelCount = ProjectPtr->GetProjectComponentSelectedCount();
	if ( 0 == ComponentSelCount ) { return; }

	strCaption = _T("Set Offset Wnd");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strLabelX = _T("X:");
	strLabelY = _T("Y:");
	InputBox.SetParam2(strCaption, strLabelX, strX, strLabelY, strY);
	if ( InputBox.DoModal() == IDCANCEL ) { return ; }
	strX = InputBox.m_DataEdit1;
	strY = InputBox.m_DataEdit2;
	const double dX = ::_tcstod(strX, NULL);
	const double dY = ::_tcstod(strY, NULL);
	if ( fabs(dX)<DBL_Precesion && fabs(dY)<DBL_Precesion ) { return; }
	ProjectPtr->MoveProjectComponentSelected(dX, dY);
	LogOperCtrl.SaveLogProjectComponentSelectedMove(ProjectPtr, dX, dY);
	//UpdateComponentListState(m_wndSubListCtrl);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);
}
//-------------------------------------------------------------------------------------//
void CEditPartNumberListDockPane::OnComponentSetPos()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	CAOIComponent *ComponentPtr = ProjectPtr->GetProjectActiveComponent();
	if ( NULL == ComponentPtr ) { return ; }

	CString      strValue;
	CString      strCaption;
	CString      strLabelX, strLabelY;
	CString      strX = _T("0");
	CString      strY = _T("0");
	const double PosX = ComponentPtr->GetComponentCadPosX();
	const double PosY = ComponentPtr->GetComponentCadPosY();
	CInputBoxWnd InputBox;
	const double DBL_Precesion = DBL_PRECISION;
	const size_t ComponentSelCount = ProjectPtr->GetProjectComponentSelectedCount();
	if ( 0 == ComponentSelCount ) { return; }

	strCaption = _T("Set Position Wnd");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strLabelX = _T("X:");
	strLabelY = _T("Y:");
	strX.Format(_T("%.2f"), PosX);
	strY.Format(_T("%.2f"), PosY);
	InputBox.SetParam2(strCaption, strLabelX, strX, strLabelY, strY);
	if ( InputBox.DoModal() == IDCANCEL ) { return ; }
	strX = InputBox.m_DataEdit1;
	strY = InputBox.m_DataEdit2;	
	const double NewPosX = ::_ttof(strX);
	const double NewPosY = ::_ttof(strY);
	const double dX = NewPosX-PosX;
	const double dY = NewPosY-PosY;
	if ( fabs(dX)<DBL_Precesion && fabs(dY)<DBL_Precesion ) { return; }
	ProjectPtr->MoveProjectComponentSelected(dX, dY);
	LogOperCtrl.SaveLogProjectComponentSelectedMove(ProjectPtr, dX, dY);
	//UpdateComponentListState(m_wndComponentListCtrl);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);
}
//-------------------------------------------------------------------------------------//
void CEditPartNumberListDockPane::OnComponentMirrorPosX()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return ; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	
	const size_t ComponentSelCount = ProjectPtr->GetProjectComponentSelectedCount();

	if ( 0 == ComponentSelCount ) { return; }
	ProjectPtr->MirrorXProjectComponentSelected();
	LogOperCtrl.SaveLogProjectComponentSelectedMirrorX(ProjectPtr);
	UpdateComponentListState(m_wndSubListCtrl);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditPartNumberListDockPane::OnComponentMirrorPosY()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return ; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	
	const size_t ComponentSelCount = ProjectPtr->GetProjectComponentSelectedCount();
	if ( 0 == ComponentSelCount ) { return; }

	ProjectPtr->MirrorYProjectComponentSelected();
	LogOperCtrl.SaveLogProjectComponentSelectedMirrorY(ProjectPtr);
	UpdateComponentListState(m_wndSubListCtrl);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);
	return ;
}
//-------------------------------------------------------------------------------------//
bool CEditPartNumberListDockPane::ExecComponentRotation(double Angle)
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return true; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }	
	const size_t ComponentSelCount = ProjectPtr->GetProjectComponentSelectedCount();
	if ( 0 == ComponentSelCount ) { return false; }

	ProjectPtr->RotateProjectComponentSelected(Angle);
	LogOperCtrl.SaveLogProjectComponentSelectedRotate(ProjectPtr, Angle);
	UpdateComponentListState(m_wndSubListCtrl);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);	
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditPartNumberListDockPane::OnComponentRotate090()
{
	ExecComponentRotation(90.0);
}
//-------------------------------------------------------------------------------------//
void CEditPartNumberListDockPane::OnComponentRotate180()
{
	ExecComponentRotation(180.0);
}
//-------------------------------------------------------------------------------------//
void CEditPartNumberListDockPane::OnComponentRotate270()
{
	ExecComponentRotation(270.0);
}
//-------------------------------------------------------------------------------------//
void CEditPartNumberListDockPane::OnComponentRotateAny()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }

	CString      strValue;
	CString      strLabel;
	CString      strCaption;
	CString      strAngle = _T("0");	
	CInputBoxWnd InputBox;
	const double DBL_Precesion = DBL_PRECISION;
	const size_t ComponentSelCount = ProjectPtr->GetProjectComponentSelectedCount();
	if ( 0 == ComponentSelCount ) { return; }

	strCaption = _T("Set Rotation Angle Wnd");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strLabel = _T("Rotation Angle:");
	strLabel = LoadMultiLanguageString(strLabel, strLabel);
	InputBox.SetParam1(strCaption, strLabel, strAngle);
	if ( InputBox.DoModal() == IDCANCEL ) { return ; }
	strAngle = InputBox.m_DataEdit1;
	const double RotateAngle = ::_tcstod(strAngle, NULL);	
	if ( fabs(RotateAngle)<DBL_Precesion ) { return; }
	ExecComponentRotation(RotateAngle);
}
//-------------------------------------------------------------------------------------//
void CEditPartNumberListDockPane::OnComponentRotateReverse()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return ; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	const size_t ComponentSelCount = ProjectPtr->GetProjectComponentSelectedCount();
	if ( 0 == ComponentSelCount ) { return; }

	ProjectPtr->ReverseProjectComponentSelected();
	UpdateComponentListState(m_wndSubListCtrl);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditPartNumberListDockPane::OnComponentRename()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return ; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	CAOIComponent *ComponentPtr = ProjectPtr->GetProjectActiveComponent();
	if ( NULL == ComponentPtr ) { return; }
	CAOIBoard *BoardPtr = ComponentPtr->GetComponentBoardPtr();
	if ( NULL == BoardPtr ) { return; }

	CString    strLabel;
	CString    strValue;
	CString    strCaption;		
	CInputBoxWnd InputBox;
	CString    strName=ComponentPtr->GetComponentName();

	strCaption = _T("Input Component Name Wnd");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strLabel = _T("Component Name");
	strLabel = LoadMultiLanguageString(strLabel, strLabel);
	strValue = ComponentPtr->GetComponentName();
	while ( true )
	{
		InputBox.SetParam1(strCaption, strLabel, strValue);
		if ( InputBox.DoModal() == IDCANCEL ) { return ; }
		strValue = InputBox.m_DataEdit1;
		strValue.MakeUpper();
		strValue.TrimLeft();//剔除左邊
		strValue.TrimRight();//剔除右邊
		if ( strValue.GetLength() == 0 ) 
		{	continue; }
		if ( BoardPtr->ChceckBoardComponentNameExist(strValue) == false )
		{	break; }
	};
	ProjectPtr->BackupProjectComponentSelected();
	ProjectPtr->SetProjectComponentSelectedComponentName(strName, strValue);
	LogOperCtrl.SaveLogProjectComponentSelectedComponentName(ProjectPtr, strName, strValue);	
	ProjectPtr->RestoreProjectComponentSelected();
	UpdateComponentListState(m_wndSubListCtrl);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditPartNumberListDockPane::OnComponentDelete()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return ; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	if ( AOIDataCollect.OperateLevelEditFuncDelComponent() == false ) { return ; }

	std::vector<CAOIComponent*> SelComponentList;
	ProjectPtr->GetProjectComponentSelected(SelComponentList);
	const size_t SelComponentCount = SelComponentList.size();
	if ( 0 == SelComponentCount ) { return; }

	CString str, str1, str2;
	CString strCount = AOIDataDefine.GetCountText();
	str1 = _T("Do you want to delete the selected components");
	str1 = LoadMultiLanguageString(str1, str1);
	if ( SelComponentCount > 1 ) 
	{	str.Format(_T("%s [%s:%d]?"), str1, strCount, SelComponentCount); }
	else
	{
		CAOIComponent *ComponentPtr = SelComponentList[0];
		str2 = ComponentPtr->GetComponentFullName();
		str.Format(_T("%s [%s]?"), str1, str2);
	}
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) != IDYES )
	{	return; }
	AOIDataCollect.ReleaseModelUniFrameList();
	LogOperCtrl.SaveLogProjectComponentSelectedDelete(ProjectPtr);
	ProjectPtr->DeleteProjectComponentSelected();
	BuildSubListCtrl(m_wndSubListCtrl, m_PartNumberName);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditPartNumberListDockPane::OnComponentSetNozzleName()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return ; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	CAOIComponent *ComponentPtr = ProjectPtr->GetProjectActiveComponent();
	if ( NULL == ComponentPtr ) { return; }	
	const size_t ComponentSelCount = ProjectPtr->GetProjectComponentSelectedCount();
	if ( 0 == ComponentSelCount ) { return; }	

	CString    strLabel;
	CString    strValue;
	CString    strCaption;		
	CInputBoxWnd InputBox;

	strCaption = _T("Input Nozzle Name Wnd");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strLabel = AOIDataDefine.GetNozzleNameText();	
	strValue = ComponentPtr->GetComponentNozzleName();
	InputBox.SetParam1(strCaption, strLabel, strValue);
	if ( InputBox.DoModal() == IDCANCEL ) { return ; }
	strValue = InputBox.m_DataEdit1;
	strValue.MakeUpper();	
	ProjectPtr->SetProjectComponentSelectedNozzleName(strValue);
	LogOperCtrl.SaveLogProjectComponentSelectedNozzlName(ProjectPtr, strValue);
	UpdateComponentListState(m_wndSubListCtrl);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditPartNumberListDockPane::OnComponentSetPartNumber()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return ; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	CAOIComponent *ComponentPtr = ProjectPtr->GetProjectActiveComponent();
	if ( NULL == ComponentPtr ) { return; }	
	const size_t ComponentSelCount = ProjectPtr->GetProjectComponentSelectedCount();
	if ( 0 == ComponentSelCount ) { return; }	

	CString    strLabel;
	CString    strValue;
	CString    strCaption;		
	CInputBoxWnd InputBox;

	strCaption = _T("Input Part Number Wnd");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strLabel = AOIDataDefine.GetPartNumberText();	
	strValue = ComponentPtr->GetComponentPartNumber();
	InputBox.SetParam1(strCaption, strLabel, strValue);
	while ( true ) 
	{
		if ( InputBox.DoModal() == IDCANCEL ) { return ; }
		strValue = InputBox.m_DataEdit1;
		strValue.MakeUpper();	
		strValue.TrimLeft();//剔除左邊
		strValue.TrimRight();//剔除右邊
		if ( strValue.GetLength() == 0 ) 
		{	continue; }
		break;
	};
	ProjectPtr->SetProjectComponentSelectedPartNumber(strValue);
	LogOperCtrl.SaveLogProjectComponentSelectedPartNumber(ProjectPtr, strValue);
	//UpdateComponentListState(m_wndSubListCtrl);
	//AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);
	BuildPartNumberListCtrl(m_wndPartNumberListCtrl);
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditPartNumberListDockPane::OnComponentSearch()
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	CThisListCtrl_11 &ListCtrl = m_wndSubListCtrl;	
	const int ItemCount = ListCtrl.GetItemCount();	
	const int nOldItem = ListCtrl.GetNextItem(-1, LVNI_FOCUSED);

	CString      str, str2;
	CString      strName;
	CString      strValue;
	CString      strLabel;
	CString      strCaption;
	unsigned int   ComponentIndex=0;
	CAOIComponent *ComponentPtr = NULL;
	CInputBoxWnd InputBox;

	strCaption = _T("Search Component");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strLabel = _T("Name:");
	if ( nOldItem >= 0 ) 
	{
		ComponentIndex = (unsigned int )(ListCtrl.GetItemData(nOldItem));
		ComponentPtr = ProjectPtr->GetProjectComponentPtr(ComponentIndex, true);
	}
	if ( NULL != ComponentPtr )
	{	strValue = ComponentPtr->GetComponentName(); }

	InputBox.SetParam1(strCaption, strLabel, strValue);
	if ( InputBox.DoModal() == IDCANCEL ) { return ; }
	strValue = InputBox.m_DataEdit1;

	int   i=0;
	int   nNewItem = -1;	
	for ( i=nOldItem+1; i<ItemCount; i++ )
	{
		ComponentIndex = (unsigned int )(ListCtrl.GetItemData(i));
		ComponentPtr = ProjectPtr->GetProjectComponentPtr(ComponentIndex, true);
		if ( NULL == ComponentPtr ) { continue; }
		strName = ComponentPtr->GetComponentName();
		if ( strValue.CompareNoCase(strName) != 0 ) { continue; }
		nNewItem = i;		
		break;
	}	
	if ( -1 == nNewItem )
	{
		for ( i=0; i<nOldItem; i++ )
		{
			ComponentIndex = (unsigned int )(ListCtrl.GetItemData(i));
			ComponentPtr = ProjectPtr->GetProjectComponentPtr(ComponentIndex, true);
			if ( NULL == ComponentPtr ) { continue; }
			strName = ComponentPtr->GetComponentName();
			if ( strValue.CompareNoCase(strName) != 0 ) { continue; }
			nNewItem = i;			
			break;
		}
	}
	if ( -1 == nNewItem )
	{
		str = _T("Can not find the component");
		str = LoadMultiLanguageString(str, str);
		str2.Format(_T("%s[%s]"), str, strValue);
		JetAPI::ShowMessageBox(str2);
		return ;	
	}	

	m_MoveToComponent = false;
	AOIDataCollect.ResetActiveIndex();		
	ListCtrl.SetItemState(nOldItem, 0, LVIS_SELECTED|LVIS_FOCUSED);
	ListCtrl.SetItemState(nNewItem, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);	
	const int ShowIndex = JetAPI::GetEnsureVisibleIndex(nNewItem, ItemCount);
	if ( ShowIndex != nNewItem )
	{	ListCtrl.EnsureVisible(nNewItem, FALSE); }
	ListCtrl.EnsureVisible(ShowIndex, FALSE);
	ListCtrl.SetFocus();
	m_MoveToComponent = true;
	AOIDataCollect.MoveStageToComponentOrField(ComponentPtr, true);
	AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_UPDATE, (LPARAM)(this));
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditPartNumberListDockPane::OnComponentSelectAll()
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	int            i=0;
	unsigned int   ComponentIndex=0;
	CAOIComponent *ComponentPtr = NULL;		
	CThisListCtrl_11 &ListCtrl = m_wndSubListCtrl;	
	const int ItemCount = ListCtrl.GetItemCount();	
	if ( 0 == ItemCount ) { return; }
	
	ProjectPtr->SelectProjectAllComponents(false);
	m_StopSubListBeSelected = TRUE;
	for ( i=0; i<ItemCount; i++ )
	{
		ComponentIndex = (unsigned int )(ListCtrl.GetItemData(i));
		ComponentPtr = ProjectPtr->GetProjectComponentPtr(ComponentIndex, true);
		if ( NULL == ComponentPtr ) { continue; }
		ComponentPtr->SetComponentSelected(true);
		ListCtrl.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
	}	
	m_StopSubListBeSelected = FALSE;

	CAOIComponent *ComponentActPtr = ProjectPtr->GetProjectActiveComponent();
	if ( NULL != ComponentActPtr )
	{
		CString PartNumber = ComponentActPtr->GetComponentPartNumber();
		if ( m_PartNumberName.CompareNoCase(PartNumber) != 0 ) 
		{	ComponentActPtr = NULL; }
	}
	if ( NULL == ComponentActPtr )
	{	
		ComponentPtr = ProjectPtr->GetProjectComponentPtrBySelected();
		if ( NULL != ComponentPtr ) 
		{	ProjectPtr->SetProjectActiveComponent(ComponentPtr);	}
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CEditPartNumberListDockPane::OnComponentBypass()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	
	if ( AOIDataCollect.OperateLevelEditFuncBypassComponent() == false )	{	return ; }
	ProjectPtr->SwitchProjectComponentBypassed();	
	LogOperCtrl.SaveLogProjectComponentSelectedBypassed(ProjectPtr);
	UpdateComponentListState(m_wndSubListCtrl);
}
//-------------------------------------------------------------------------------------//
void CEditPartNumberListDockPane::OnComponentXBoardUnit()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	ProjectPtr->SwitchProjectComponentXBoardUnit();	
	LogOperCtrl.SaveLogProjectComponentSelectedXBoardUnit(ProjectPtr);	
	UpdateComponentListState(m_wndSubListCtrl);
}
//-------------------------------------------------------------------------------------//
void CEditPartNumberListDockPane::OnComponentModelIsolated()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	CString str;
	CAOIComponent *ComponentPtr = ProjectPtr->GetProjectActiveComponent();
	if ( NULL == ComponentPtr ) { return; }
	const bool ModelIsolated = ComponentPtr->GetComponentModelIsolated();
	if ( true == ModelIsolated )
	{
		str = _T("Disable Model Isolated will clear the model datas, do you want to continue?");
		str = LoadMultiLanguageString(str, str);
		if ( JetAPI::ShowMessageBox(str, MB_YESNO) != IDYES )
		{	return ; }
	}
	ProjectPtr->SwitchProjectComponentModelIsolated();	
	LogOperCtrl.SaveLogProjectComponentSelectedModelIsolated(ProjectPtr);
	UpdateComponentListState(m_wndSubListCtrl);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_WND_PROPERTY_WND, WPARAM_BUILD_WND_SELECTED, NULL);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);
}
//-------------------------------------------------------------------------------------//
void CEditPartNumberListDockPane::OnComponentRestoreCadPos()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	
	CString str;
	str = _T("Do you want to restore CAD Pos.?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO )
	{	return ; }
	ProjectPtr->RestoreProjectComponentCadPosBySelected();
	UpdateComponentListState(m_wndSubListCtrl);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);
}
//-------------------------------------------------------------------------------------//
void CEditPartNumberListDockPane::OnComponentBypass3D()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	
	if ( AOIDataCollect.OperateLevelEditFuncBypassComponent() == false )	{	return ; }
	ProjectPtr->SwitchProjectComponentBypass3D();
	LogOperCtrl.SaveLogProjectComponentSelectedBypass3D(ProjectPtr);
	UpdateComponentListState(m_wndSubListCtrl);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);
}
//-------------------------------------------------------------------------------------//
void CEditPartNumberListDockPane::OnComponentMaskBaseSetColorIndex()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }		
	CAOIComponent *ComponentPtr = ProjectPtr->GetProjectActiveComponent();
	if ( NULL == ComponentPtr ) { return ; }

	CSpaceBaseParamWnd ParamWnd;
	int  nBaseColoeIndex=0;
	bool bBaseColorEnabled=false;	
	TBasePlaneParam BasePlaneParam;
	const double Angle = ComponentPtr->GetComponentAngle();
	int  nLocalPlaneID = ComponentPtr->GetComponentLocalBasePlaneID();

	BasePlaneParam = ComponentPtr->GetComponentSpaceBasePlaneParam();
	bBaseColorEnabled = ComponentPtr->GetComponentMaskEnable_Base();
	nBaseColoeIndex   = ComponentPtr->GetComponentMaskColorGroupLinkIndex();	
	if ( nBaseColoeIndex >= 0 ) { nBaseColoeIndex -= PROJECT_COLOR_ID_BOARD_BEGIN; }

	ParamWnd.SetProjectPtr(ProjectPtr);
	ParamWnd.SetLocalBasePlaneID(nLocalPlaneID);
	ParamWnd.SetBasePlaneParam(BasePlaneParam);
	ParamWnd.SetBaseColorIndex(nBaseColoeIndex);
	ParamWnd.SetBaseColorEnabled(bBaseColorEnabled);	
	if ( ParamWnd.DoModal() == IDCANCEL )
	{	return ; }	

	ParamWnd.GetBasePlaneParam(BasePlaneParam);
	nLocalPlaneID = ParamWnd.GetLocalBasePlaneID();
	nBaseColoeIndex = ParamWnd.GetBaseColorIndex();
	bBaseColorEnabled = ParamWnd.GetBaseColorEnabled();
	const bool bParamSetting = ParamWnd.GetBasePlaneParamSetting();
	const bool bColorSetting = ParamWnd.GetBasePlaneColorSetting();

	if ( true == bParamSetting )
	{	
		ProjectPtr->SetProjectComponentSelectedLocalBasePlaneID(nLocalPlaneID);
		ProjectPtr->SetProjectComponentSelectedSpaceBasePlaneParam(BasePlaneParam, Angle); 
		if ( -1 != BasePlaneParam.BasePlaneIndex )
		{	ProjectPtr->UpdateProjectSpaceBasePlaneParam(BasePlaneParam); }	
	}

	if ( true == bColorSetting ) 
	{
		if ( false==bBaseColorEnabled || -1==nBaseColoeIndex )
		{	ProjectPtr->EnableProjectComponentSelectedMaskFunc_Base(false);	}
		else
		{
			nBaseColoeIndex += PROJECT_COLOR_ID_BOARD_BEGIN;
			ProjectPtr->EnableProjectComponentSelectedMaskFunc_Base(true);
			ProjectPtr->SetProjectComponentSelectedMaskColorIndex_Base(nBaseColoeIndex); 
		}
	}
	AOIDataCollect.ReleaseModelUniFrameList();//需要重新建立，因此釋放模組的圖像資料
	/*
	bool bEnable = true;
	size_t     i=0;
	CString    str;
	CString    strLabel;
	CString    strValue;
	CString    strCaption;		
	CString    strUnset = AOIDataDefine.GetUnsetText();
	CInputComboxWnd ComboxWnd;		
	CColorGroup* ColorGroupPtr=NULL;
	TComboxNode              Node;
	std::vector<TComboxNode> NodelList;			

	DWORD_PTR OldColorIndex = ComponentPtr->GetComponentMaskColorGroupLinkIndex();
	strLabel = _T("Color Index");
	strLabel = LoadMultiLanguageString(strLabel, strLabel);
	strCaption = _T("Set Component Base Mask Color Index");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);

	Node.Data = -1;	
	str = _T("Close");	
	Node.Text = LoadMultiLanguageString(str, str);
	NodelList.push_back(Node);
	for ( i=PROJECT_COLOR_ID_BOARD_BEGIN; i<=PROJECT_COLOR_ID_BOARD_END; i++ )
	{
		Node.Data = i;
		Node.Text = AOIDataDefine.GetProjectColorGroupText(i);
		if ( NULL != ColorGroupPtr ) 
		{
			if ( ColorGroupPtr->CheckColorGroupUsed() == false ) 
			{	
				str.Format(_T("%s  [%s]"), Node.Text, strUnset);
				Node.Text = str;
			}
		}
		NodelList.push_back(Node);
	}	
	if ( ComponentPtr->GetComponentMaskEnable_Base() == false )
	{	OldColorIndex = -1; }
	ComboxWnd.SetParam1(strCaption, strLabel, OldColorIndex, NodelList);
	if ( ComboxWnd.GetSelIndex1() < 0 ) 
	{	ComboxWnd.SetSelIndex1(0); }	
	if ( ComboxWnd.DoModal() == IDCANCEL )
	{	return ; }		
	const int NewColorIndex = (int)(ComboxWnd.GetSelData());	
	if ( NewColorIndex < 0 )
	{	ProjectPtr->EnableProjectComponentSelectedMaskFunc_Base(false);	}
	else
	{
		ProjectPtr->EnableProjectComponentSelectedMaskFunc_Base(true);
		ProjectPtr->SetProjectComponentSelectedMaskColorIndex_Base(NewColorIndex); 
	}*/
}
//-------------------------------------------------------------------------------------//
void CEditPartNumberListDockPane::OnComponentSpaceNoiseFilter()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	
	CAOIComponent *ComponentPtr = ProjectPtr->GetProjectActiveComponent();
	if ( NULL == ComponentPtr )	{	return; }
	CAOIModel     *ModelPtr = ComponentPtr->GetComponentModelPtr();
	if ( NULL == ModelPtr )	{	return; }	

	const bool bClone = false;
	CSpaceNoiseFilterParamWnd Wnd;
	TNoiseFilterParam NoiseFilterParam;
	std::vector<TUNI_FRAME> UniFrameList;
	unsigned int MapIndex = ProjectPtr->GetProjectMapIndex();	
	AOIDataCollect.ReleaseModelUniFrameList();
	AOIDataCollect.SendMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_BUILD_RAW_MODEL_UNI_FRAME_LIST, (LPARAM)(ModelPtr));
	AOIDataCollect.CopyModelUniFrameList(UniFrameList, bClone);	
	NoiseFilterParam = ComponentPtr->GetComponentSpaceNoiseFilterParam();	
	Wnd.SetNoiseFilterParam(NoiseFilterParam);
	Wnd.SetModelUniFrameList(MapIndex, UniFrameList);
	if ( Wnd.DoModal() == IDCANCEL )
	{
		AOIDataCollect.ReleaseModelUniFrameList();//需要重新建立，因此釋放模組的圖像資料
		AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_PART_SELECTED, NULL);	
		return ;	
	}	
	Wnd.GetNoiseFilterParam(NoiseFilterParam);	
	ProjectPtr->SetProjectComponentSelectedSpaceNoiseFilterParam(NoiseFilterParam);
	if ( -1 != NoiseFilterParam.DataFilterIndex )
	{	ProjectPtr->UpdateProjectSpaceNoiseFilterParam(NoiseFilterParam); }
	AOIDataCollect.ReleaseModelUniFrameList();//需要重新建立，因此釋放模組的圖像資料
	AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_PART_SELECTED, NULL);	
}
//-------------------------------------------------------------------------------------//
void CEditPartNumberListDockPane::OnComponentMaskExtendSizeBody()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	
	CAOIComponent *ComponentPtr = ProjectPtr->GetProjectActiveComponent();
	if ( NULL == ComponentPtr )	{	return; }
	double ExtendW = ComponentPtr->GetComponentMaskExtendW_Body();
	double ExtendH = ComponentPtr->GetComponentMaskExtendH_Body();

	CString      strValue;
	CString      strLabel;
	CString      strCaption;
	CInputBoxWnd InputBox;

	strCaption = _T("Input Component Mask Extend Width");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strLabel = _T("Extend Width");
	strLabel = LoadMultiLanguageString(strLabel, strLabel);
	strValue.Format(_T("%.0f"), ExtendW);
	InputBox.SetParam1(strCaption, strLabel, strValue);
	if ( InputBox.DoModal() == IDCANCEL ) 
	{	return ;  }
	ExtendW = JetAPI::StrToDbl(InputBox.m_DataEdit1);

	strCaption = _T("Input Component Mask Extend Height");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strLabel = _T("Extend Height");
	strLabel = LoadMultiLanguageString(strLabel, strLabel);
	strValue.Format(_T("%.0f"), ExtendH);
	InputBox.SetParam1(strCaption, strLabel, strValue);
	if ( InputBox.DoModal() == IDCANCEL ) 
	{	return ;  }
	ExtendH = JetAPI::StrToDbl(InputBox.m_DataEdit1);

	ProjectPtr->SetProjectComponentSelectedMaskExtendSize_Body(ExtendW, ExtendH);	
	LogOperCtrl.SaveLogProjectComponentSelectedMaskExtendSize_Body(ProjectPtr, ExtendW, ExtendH);
}
//-------------------------------------------------------------------------------------//
void CEditPartNumberListDockPane::OnComponentEnableAlarmAOI()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	
	CAOIComponent *ComponentPtr = ProjectPtr->GetProjectActiveComponent();
	if ( NULL == ComponentPtr )	{	return; }	
	
	CComponentDefectAlarmWnd Wnd;
	Wnd.SetEnableAlarm(ComponentPtr->GetComponentEnableAlarm());
	Wnd.SetEnableAlarmOnAOI(ComponentPtr->GetComponentDefectAlarmEnableOnAOI());
	Wnd.SetEnableAlarmOnARS(ComponentPtr->GetComponentDefectAlarmEnableOnARS());
	Wnd.SetEnableDefectCountOnARS(ComponentPtr->GetComponentDefectCountEnableOnARS());	
	Wnd.SetDefectAlarmAOI(ComponentPtr->GetComponentDefectItemAlarmAOI());
	Wnd.SetDefectAlarmARS(ComponentPtr->GetComponentDefectItemAlarmARS());
	Wnd.SetAlarmParamFromModeAOI(ComponentPtr->GetComponentDefectAlarmFromModeAOI());
	Wnd.SetAlarmParamFromModeARS(ComponentPtr->GetComponentDefectAlarmFromModeARS());
	if ( Wnd.DoModal() == IDCANCEL )
	{	return; }
	const bool bEnableAlarm=Wnd.GetEnableAlarm();
	const bool bEnableAlarmOnAOI=Wnd.GetEnableAlarmOnAOI();
	const bool bEnableAlarmOnARS=Wnd.GetEnableAlarmOnARS();
	const bool bEnableDefectCountOnARS=Wnd.GetEnableDefectCountOnARS();
	const CWndDefectItem &DefectAlarmAOI=Wnd.GetDefectAlarmAOI();
	const CWndDefectItem &DefectAlarmARS=Wnd.GetDefectAlarmARS();
	DEFECT_PARAM_FROM_MODE DefectParamFromModeAOI=Wnd.GetAlarmParamFromModeAOI();
	DEFECT_PARAM_FROM_MODE DefectParamFromModeARS=Wnd.GetAlarmParamFromModeARS();
	ProjectPtr->SetProjectComponentSelectedEnableAlarm(bEnableAlarm);	
	ProjectPtr->SetProjectComponentSelectedEnableAlarmOnAOI(bEnableAlarmOnAOI);
	ProjectPtr->SetProjectComponentSelectedEnableAlarmOnARS(bEnableAlarmOnARS);
	ProjectPtr->SetProjectComponentSelectedEnableDefectCountOnARS(bEnableDefectCountOnARS);
	LogOperCtrl.SaveLogProjectComponentSelectedAlarm(ProjectPtr, bEnableAlarm);
	LogOperCtrl.SaveLogProjectComponentSelectedAlarmOnAOI(ProjectPtr, bEnableAlarmOnAOI);
	LogOperCtrl.SaveLogProjectComponentSelectedAlarmOnARS(ProjectPtr, bEnableAlarmOnARS);
	LogOperCtrl.SaveLogProjectComponentSelectedDefectCountOnARS(ProjectPtr, bEnableDefectCountOnARS);	
	ProjectPtr->SetProjectComponentSelectedDefectAlarmAOI(DefectParamFromModeAOI, DefectAlarmAOI);
	ProjectPtr->SetProjectComponentSelectedDefectAlarmARS(DefectParamFromModeARS, DefectAlarmARS);
	LogOperCtrl.SaveLogProjectComponentSelectedDefectAlarmAOI(ProjectPtr, DefectParamFromModeAOI, DefectAlarmAOI);
	LogOperCtrl.SaveLogProjectComponentSelectedDefectAlarmARS(ProjectPtr, DefectParamFromModeARS, DefectAlarmARS);
	return;
}
//-------------------------------------------------------------------------------------//
void CEditPartNumberListDockPane::OnComponentGroupID()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	
	CAOIComponent *ComponentPtr = ProjectPtr->GetProjectActiveComponent();
	if ( NULL == ComponentPtr )	{	return; }	
	
	DWORD        Res=0;
	CString      str;
	CString      strValue;
	CString      strLabel;
	CString      strCaption;
	CInputBoxWnd InputBox;
	const int    GroupID = ComponentPtr->GetComponentGroupID();
	str = _T("Set Component Group ID");
	strCaption = LoadMultiLanguageString(str, str);
	strLabel = _T("Gorup ID (0:Disable)");
	strValue.Format(_T("%d"), GroupID);
	InputBox.SetParam1(strCaption, strLabel, strValue);
	if ( InputBox.DoModal() == IDCANCEL )
	{	return; }
	const int NewGroupID=::_ttoi(InputBox.m_DataEdit1);
	ProjectPtr->SetProjectComponentSelectedGroupID(NewGroupID);
}
//-------------------------------------------------------------------------------------//
void CEditPartNumberListDockPane::OnComponentGroupOrg()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	
	CAOIComponent *ComponentPtr = ProjectPtr->GetProjectActiveComponent();
	if ( NULL == ComponentPtr )	{	return; }	
	
	DWORD        Res=0;
	CString      str;
	bool         bOrg=false;
	str = _T("Do you want to set the component be group org part");
	str = LoadMultiLanguageString(str, str);
	Res = JetAPI::ShowMessageBox(str, MB_YESNOCANCEL);
	if ( IDCANCEL == Res )
	{	return ; }	

	if ( IDNO == Res ) { bOrg = false; }
	else { bOrg = true; }
	ProjectPtr->SetProjectComponentSelectedGroupOrg(bOrg);	
}
//-------------------------------------------------------------------------------------//
void CEditPartNumberListDockPane::OnComponentToFieldPos()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	
	CAOIComponent *ComponentPtr = ProjectPtr->GetProjectActiveComponent();
	if ( NULL == ComponentPtr )	{	return; }	
	if ( AOIDataCollect.MoveStageToComponentField(ComponentPtr, true) == false )
	{		
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return;
	}
}
//-------------------------------------------------------------------------------------//
void CEditPartNumberListDockPane::OnComponentEnableSelfField()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	
	CAOIComponent *ComponentPtr = ProjectPtr->GetProjectActiveComponent();
	if ( NULL == ComponentPtr )	{	return; }	
	CString str;
	DWORD Res=0;
	DWORD DefaultBtn=MB_DEFBUTTON1;	
	bool bEnabled = ComponentPtr->GetComponentSelfFieldEnabled();	
	if ( true == bEnabled ) { DefaultBtn=MB_DEFBUTTON1;	}
	else { DefaultBtn=MB_DEFBUTTON2; }
	str = _T("Do you want to enable component's self field?");
	str = LoadMultiLanguageString(str, str);
	Res = JetAPI::ShowMessageBox(str, MB_YESNOCANCEL|DefaultBtn);
	if ( IDCANCEL == Res ) { return; }
	if ( IDYES == Res ) { bEnabled = true; }
	else { bEnabled = false; }
	ProjectPtr->SetProjectComponentSelectedEnableSelfField(bEnabled);
	LogOperCtrl.SaveLogProjectComponentSelectedSelfField(ProjectPtr, bEnabled);
	return;
}
//-------------------------------------------------------------------------------------//
void CEditPartNumberListDockPane::OnComponentCloneNewModel()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	
	CAOIComponent *ComponentPtr = ProjectPtr->GetProjectActiveComponent();
	if ( NULL == ComponentPtr )	{	return; }	
	CAOIModel *ModelPtr = ComponentPtr->GetComponentModelPtr();
	if ( NULL == ModelPtr ) { return; }
	if ( ModelPtr->CheckModelTypeEnabled() == false )
	{	return; }

	CString    str;
	CAOIModel *ModelPtr_New = NULL;
	const bool bSucc = AOIDataCollect.CloneProjectNewModel(ProjectPtr, ModelPtr, ModelPtr_New);
	if ( false == bSucc )
	{
		str = AOIDataCollect.GetErrorString();
		JetAPI::ShowMessageBox(str);
		return;
	}	
	if ( NULL == ModelPtr_New )
	{	return ; }
	ProjectPtr->AddProjectModelPtr(ModelPtr_New, false);
	ProjectPtr->ApplyProjectModelToComponentsSelected(ModelPtr_New);	
	return;
}
//-------------------------------------------------------------------------------------//
void CEditPartNumberListDockPane::OnComponentChangeBoard()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	
	
	size_t        i=0;	
	TListNode     Node;
	CString       str;	
	CString       strLabel;
	CString       strCaption;	
	TListNode     ListNode;
	CInputListWnd EnumWnd;	
	unsigned int  PanelIndex=0;
	unsigned int  BoardIndex=0;
	std::vector<TListNode> NodelList;	
	const size_t PanelCount=ProjectPtr->GetProjectPanelCount();
	strLabel = AOIDataDefine.GetPanelText();	
	strCaption = AOIDataDefine.GetPanelText();		
	if ( PanelCount > 1 )
	{
		NodelList.clear();
		for ( i=0; i<PanelCount; i++ )
		{
			ListNode.Text.Format(_T("%d"), i+1);
			ListNode.Data = i;
			NodelList.push_back(ListNode);		
		}	
		EnumWnd.SetParam1(strCaption, strLabel, -1, NodelList);	
		if ( EnumWnd.DoModal() == IDCANCEL )
		{	return;	}
		PanelIndex=EnumWnd.GetSelIndex1();
	}
	else
	{	PanelIndex = 0; }
	CAOIPanel *PanelPtr = ProjectPtr->GetProjectPanelPtr(PanelIndex, true);
	if ( NULL == PanelPtr ) { return ; }
	const size_t PanelBoardCount=PanelPtr->GetPanelBoardCount();

	NodelList.clear();
	strLabel = AOIDataDefine.GetBoardText();	
	strCaption = AOIDataDefine.GetBoardText();
	ListNode.Data = -1;
	ListNode.Text = _T("New");	
	NodelList.push_back(ListNode);
	for ( i=0; i<PanelBoardCount; i++ )
	{
		ListNode.Data = i;
		ListNode.Text.Format(_T("%d"), i+1);		
		NodelList.push_back(ListNode);		
	}		
	EnumWnd.SetParam1(strCaption, strLabel, -1, NodelList);	
	if ( EnumWnd.DoModal() == IDCANCEL )
	{	return;	}
	ListNode = EnumWnd.GetSelNode();
	BoardIndex=(unsigned int)(ListNode.Data);
	CAOIBoard *BoardPtr = PanelPtr->GetPanelBoardPtr(BoardIndex, true);
	if ( NULL == BoardPtr )
	{
		BoardPtr = AOIObjManager.CreateBoardObj();
		if ( NULL == BoardPtr ) { return; }
		ProjectPtr->AddProjectBoardPtr(BoardPtr, false);
		PanelPtr->AddPanelBoardPtr(BoardPtr);		
	}
	if ( ProjectPtr->ChangeProjectComponentSelectedBoard(BoardPtr) == false )
	{	
		str = ProjectPtr->GetErrorString();
		JetAPI::ShowMessageBox(str);
	}	
	LogOperCtrl.SaveLogProjectComponentSelectedChangeBoard(ProjectPtr, BoardPtr);
	return;
}
//-------------------------------------------------------------------------------------//
void CEditPartNumberListDockPane::OnComponentLocalBasePlaneID()
{	
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	
	CAOIComponent *ComponentPtr = ProjectPtr->GetProjectActiveComponent();
	if ( NULL == ComponentPtr )	{	return; }	
	
	DWORD        Res=0;
	CString      str;
	CString      strValue;
	CString      strLabel;
	CString      strCaption;
	CInputBoxWnd InputBox;
	const int    BasePlaneID = ComponentPtr->GetComponentLocalBasePlaneID();
	str = _T("Set Local Base Plane ID");
	strCaption = LoadMultiLanguageString(str, str);
	str = _T("Local Base Plane ID");
	strLabel = LoadMultiLanguageString(str, str);
	strValue.Format(_T("%d"), BasePlaneID);
	InputBox.SetParam1(strCaption, strLabel, strValue);
	if ( InputBox.DoModal() == IDCANCEL )
	{	return; }
	const int NewBasePlaneID=::_ttoi(InputBox.m_DataEdit1);
	if ( NewBasePlaneID < 0 ) { return ; }
	ProjectPtr->SetProjectComponentSelectedLocalBasePlaneID(NewBasePlaneID);		
	LogOperCtrl.SaveLogProjectComponentSelectedLocalBasePlaneID(ProjectPtr, NewBasePlaneID);
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditPartNumberListDockPane::OnComponentDataModelParam()
{	
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	
	CAOIComponent *ComponentPtr = ProjectPtr->GetProjectActiveComponent();
	if ( NULL == ComponentPtr )	{	return; }	
	
	DWORD        Res=0;
	CString      str;
	CString      strValue;
	CString      strLabel;
	CString      strCaption;	
	DWORD_PTR     dwDefault=0;
	TListNode     Node;
	CInputListWnd EnumWnd;	
	std::vector<TListNode> NodelList;		
	CString       strLevel=AOIDataDefine.GetLevelText();
	const bool    DataModelEnabled = ComponentPtr->GetComponentDataModelEnabled();
	const int     DataModelLevelID = ComponentPtr->GetComponentDataModelLevelID();
	str = _T("Set Data Model Param");
	strCaption = LoadMultiLanguageString(str, str);
	str = _T("Data Model Param");
	strLabel = LoadMultiLanguageString(str, str);
	
	Node.Data = FN_DISABLE;
	Node.Text = AOIDataDefine.GetDisableText();	
	NodelList.push_back(Node);
	
	std::vector<int> LevelList;
	AOIDataDefine.BuildDataModeLevelList(LevelList);
	const size_t LevelCount=LevelList.size();
	for ( size_t i=0; i<LevelCount; i++ )
	{
		Node.Data = LevelList[i];
		Node.Text.Format(_T("%s %d"), strLevel, Node.Data);
		NodelList.push_back(Node);
	}

	if ( false == DataModelEnabled )
	{	dwDefault = DataModelEnabled; }
	else
	{	dwDefault = DataModelLevelID; }
	EnumWnd.SetParam1(strCaption, strLabel, dwDefault, NodelList);	
	if ( EnumWnd.DoModal() == IDCANCEL )
	{	return; }
	const int NewIndex = (int)(EnumWnd.GetSelData());		
	if ( NewIndex<0 || NewIndex>=NodelList.size() )
	{	return; }
	const int NewParam = NodelList[NewIndex].Data;
	ProjectPtr->SetProjectComponentSelectedDataModelParam(NewParam);
	LogOperCtrl.SaveLogProjectComponentSelectedDataModelParam(ProjectPtr, NewParam);
	return;
}
//-------------------------------------------------------------------------------------//
void CEditPartNumberListDockPane::OnComponentSaveWndList()
{	
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	
	CAOIComponent *ComponentPtr = ProjectPtr->GetProjectActiveComponent();
	if ( NULL == ComponentPtr )	{	return; }	
	CString str;
	DWORD Res=0;
	DWORD DefaultBtn=MB_DEFBUTTON1;	
	bool bEnabled = ComponentPtr->GetComponentSaveWndList();	
	if ( true == bEnabled ) { DefaultBtn=MB_DEFBUTTON1;	}
	else { DefaultBtn=MB_DEFBUTTON2; }
	str = _T("Do you want to save component wnd list?");
	str = LoadMultiLanguageString(str, str);
	Res = JetAPI::ShowMessageBox(str, MB_YESNOCANCEL|DefaultBtn);
	if ( IDCANCEL == Res ) { return; }
	if ( IDYES == Res ) { bEnabled = true; }
	else { bEnabled = false; }
	ProjectPtr->SetProjectComponentSelectedSaveWndList(bEnabled);
	LogOperCtrl.SaveLogProjectComponentSelectedSaveWndList(ProjectPtr, bEnabled);		
	return;
}
//-------------------------------------------------------------------------------------//
void CEditPartNumberListDockPane::OnComponentFeedbackResultPos()
{	
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	
	CString       str, str2;
	CString       strLabel;
	CString       strCaption;
	TListNode     Node;
	CInputListWnd EnumWnd;
	
	std::vector<TListNode> NodelList;	
	const int UsePosPad  = 0;
	const int UsePosBody = 1;	
	strLabel = _T("Result Pos Mode");
	strLabel = LoadMultiLanguageString(strLabel, strLabel);
	strCaption = _T("Set Result Pos Mode");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	Node.Data = UsePosPad;
	Node.Text = AOIDataDefine.GetPadText();		
	NodelList.push_back(Node);

	Node.Data = UsePosBody;
	Node.Text = AOIDataDefine.GetComponentText();	
	NodelList.push_back(Node);
	
	EnumWnd.SetParam1(strCaption, strLabel, (DWORD_PTR)(UsePosBody), NodelList);	
	if ( EnumWnd.DoModal() == IDCANCEL )
	{	return ; }	  
	const int UsePosMode = (int)(EnumWnd.GetSelData());
	const bool bUsePadPos = UsePosMode==UsePosPad ? true:false;

	ProjectPtr->SetProjectComponentSelectedCadPosByResult(bUsePadPos);
	UpdateComponentListState(m_wndSubListCtrl);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);
}
//-------------------------------------------------------------------------------------//