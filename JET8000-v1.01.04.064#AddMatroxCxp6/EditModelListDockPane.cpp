// EditModelListDockPane.cpp : 實作檔
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "EditModelListDockPane.h"
//-------------------------------------------------------------------------------------//
#include "InputBoxWnd.h"
#include "InputListWnd.h"
#include "ProjectListWnd.h"
#include "WndDefectItemWnd.h"
#include "ProjectLibraryWnd.h"
#include "SpaceBaseParamWnd.h"
#include "ComponentDefectAlarmWnd.h"
#include "SpaceNoiseFilterParamWnd.h"
//-------------------------------------------------------------------------------------//
#define LIST_ITEM_PATCH_N_PAGE_VALUE     10   //一次多少個頁面
#define LIST_ITEM_PATCH_ENABLE_COUNT   1000
//-------------------------------------------------------------------------------------//
const UINT ID_MODEL_LIST_CTRL      = CPageSplitterWnd::GetIdFromRowCol(0, 0);//AFX_IDW_PANE_FIRST;
const UINT ID_COMPONENT_LIST_CTRL  = CPageSplitterWnd::GetIdFromRowCol(1, 0);//AFX_IDW_PANE_FIRST+16;//注意行列會不同唷
//-------------------------------------------------------------------------------------//
// CEditModelListDockPane
//-------------------------------------------------------------------------------------//
IMPLEMENT_DYNAMIC(CEditModelListDockPane, CDockablePane)
//-------------------------------------------------------------------------------------//
CEditModelListDockPane::CEditModelListDockPane()
{
	m_clrOK = 0x008000;
	m_clrNG = 0x000080;
	m_clrBypass = 0x800000;
	m_clrUnTest = 0x808080;

	m_ProjectPtr = NULL;	
	m_DistrictID = DISTRICT_ID_A;
	m_StopModelListBeSelected = FALSE;
	m_StopComponentListBeSelected = FALSE;	
	m_ComponentIndex = -1;
	m_SwitchModelBtn = false;
	m_MoveToComponent = true;	
}
//-------------------------------------------------------------------------------------//
CEditModelListDockPane::~CEditModelListDockPane()
{
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CEditModelListDockPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_CONTEXTMENU()
	ON_NOTIFY(NM_CLICK, ID_MODEL_LIST_CTRL, OnClickModelListCtrl)
	ON_NOTIFY(NM_DBLCLK, ID_MODEL_LIST_CTRL, OnDbclickModelListCtrl)	
	ON_NOTIFY(LVN_ITEMCHANGED, ID_MODEL_LIST_CTRL, OnItemchangedModelListCtrl)
	ON_NOTIFY(NM_CLICK, ID_COMPONENT_LIST_CTRL, OnClickComponentListCtrl)	
	ON_NOTIFY(NM_DBLCLK, ID_COMPONENT_LIST_CTRL, OnDbclickComponentListCtrl)	
	ON_NOTIFY(LVN_ITEMCHANGED, ID_COMPONENT_LIST_CTRL, OnItemchangedComponentListCtrl)	
	ON_NOTIFY(LVN_ENDSCROLL, ID_COMPONENT_LIST_CTRL, OnEndScrollComponentListCtrl)
	ON_BN_CLICKED(EMPB_SWITCH_PREVIOUS_BTN, OnSwitchPreiousBtn)	
	ON_BN_CLICKED(EMPB_SWITCH_NEXT_BTN, OnSwitchNextBtn)
	ON_COMMAND(MENU_LIST_MODEL_BYPASS, OnModelBypass)
	ON_COMMAND(MENU_LIST_MODEL_SEARCH, OnModelSearch)	
	ON_COMMAND(MENU_LIST_MODEL_APPLY, OnModelApply)	
	ON_COMMAND(MENU_LIST_MODEL_IMPORT, OnModelImport)	
	ON_COMMAND(MENU_LIST_MODEL_CLONE, OnModelClone)	
	ON_COMMAND(MENU_LIST_MODEL_RENAME, OnModelRename)	
	ON_COMMAND(MENU_LIST_MODEL_DELETE, OnModelDelete)		
	ON_COMMAND(MENU_LIST_MODEL_SELECT_ALL, OnModelSelectAll)
	ON_COMMAND(MENU_LIST_MODEL_APPLY_TO_OTHERS, OnModelUpdateToOthers)	
	ON_COMMAND(MENU_LIST_MODEL_SHOW_LIBRARY_WND, OnModelShowLibraryWnd)
	ON_COMMAND(MENU_LIST_MODEL_SAVE_LEAD_REPORT, OnModelSaveLeadReport)	
	ON_COMMAND(MENU_LIST_MODEL_DEFECT_RECHECK_ARS, OnModelDefectRecheckARS)	
	ON_COMMAND(MENU_LIST_MODEL_DEFECT_ESSENTIAL, OnModelDefectEssential)
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
// CEditModelListDockPane 訊息處理常式
//-------------------------------------------------------------------------------------//
int CEditModelListDockPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此加入特別建立的程式碼
	DWORD dwBarStyle = CBRS_LEFT|CBRS_TOOLTIPS|CBRS_FLYBY;		
	if ( !m_wndPaneBar.Create(this, IDD_EDIT_MODEL_LIST_PANE_BAR, dwBarStyle, IDD_EDIT_MODEL_LIST_PANE_BAR) )
	{
		TRACE0("Failed to create Pane Dialogbar Control\n");
		return -1;
	}	

	m_wndSplitter.CreateStatic(this,2,1);	
	DWORD dwListStyle1 = WS_CHILD | WS_VISIBLE | LVS_REPORT  | LVS_SHAREIMAGELISTS | LVS_SINGLESEL | LVS_SHOWSELALWAYS;
	DWORD dwListStyle2 = WS_CHILD | WS_VISIBLE | LVS_REPORT  | LVS_SHAREIMAGELISTS;
	DWORD dwTreeStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS |TVS_EDITLABELS | TVS_SHOWSELALWAYS | TVS_FULLROWSELECT;
	//WC_TREEVIEW, WC_LISTVIEW
	if(!m_wndSplitter.AddWindow(0,0,&m_wndModelListCtrl,WC_LISTVIEW,dwListStyle1,0,CSize(160,400), ID_MODEL_LIST_CTRL))
	{
		TRACE0("Failed to create Page Spliter Context(0, 0)\n");
		return -1;
	}
	
	
	if(!m_wndSplitter.AddWindow(1,0,&m_wndComponentListCtrl,WC_LISTVIEW,dwListStyle2,0,CSize(160,400), ID_COMPONENT_LIST_CTRL))
	{
		TRACE0("Failed to create Page Spliter Context(1, 0)\n");
		return -1;
	}
	
	m_wndModelListCtrl.SetOwner(this);
	m_wndComponentListCtrl.SetOwner(this);

	JetAPI::InitialListCtrl(m_wndModelListCtrl);
	InitModelListCtrl(m_wndModelListCtrl);

	JetAPI::InitialListCtrl(m_wndComponentListCtrl);
	InitComponentListCtrl(m_wndComponentListCtrl);
	return 0;
}
//-------------------------------------------------------------------------------------//
void CEditModelListDockPane::OnDestroy()
{
	CDockablePane::OnDestroy();

	// TODO: 在此加入您的訊息處理常式程式碼
}
//-------------------------------------------------------------------------------------//
void CEditModelListDockPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: 在此加入您的訊息處理常式程式碼
	if ( CWnd::GetSafeHwnd() == NULL ) { return ; }
	if ( m_wndPaneBar.GetSafeHwnd() == NULL ) { return; }
	if ( m_wndSplitter.GetSafeHwnd() == NULL ) { return; }

	CRect rect;
	int cyTlb = 0;	
	cyTlb = m_wndPaneBar.CalcFixedLayout(FALSE, FALSE).cy;
	rect.left = 0; 
	rect.top = 0;
	rect.right = cx;
	rect.bottom = cy;
	//m_wndSplitter.SetRowInfo(0,(cy-cyTlb)/2,25);
	m_wndPaneBar.SetWindowPos(NULL,rect.left, rect.top, rect.Width(), cyTlb, SWP_NOZORDER | SWP_NOACTIVATE);
	m_wndSplitter.SetWindowPos(NULL,rect.left, rect.top+cyTlb, rect.Width(), rect.Height()-cyTlb, SWP_NOZORDER | SWP_NOACTIVATE);
}
//-------------------------------------------------------------------------------------//
void CEditModelListDockPane::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDockablePane::OnShowWindow(bShow, nStatus);

	// TODO: 在此加入您的訊息處理常式程式碼
	if ( TRUE == bShow )
	{	
		BuildModelListCtrl(m_wndModelListCtrl);	
		AOIDataCollect.SetShowUIWndModelList(true);
		//m_wndComponentListCtrl.SetFocus();
	}
	else
	{	
		CloseProject(); 
		AOIDataCollect.SetShowUIWndModelList(false);
	}
}
//-------------------------------------------------------------------------------------//
void CEditModelListDockPane::OnContextMenu(CWnd* pWnd, CPoint point)
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
		if ( ID_MODEL_LIST_CTRL == CtrlID )
		{	ExecModelListMenu(point);	}
		else if ( ID_COMPONENT_LIST_CTRL == CtrlID )
		{	ExecComponentListMenu(point);	}		
	}
}
//-------------------------------------------------------------------------------------//
void CEditModelListDockPane::SwitchMultiLanguage()
{
	/*
	size_t  i = 0;	
	UINT    WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_EDIT_MODEL_LIST_DOCK_PANE");	
	//---------------------------------------------------------------------------------//
	WndID = IDD_EDIT_MODEL_LIST_DOCK_PANE;
	WndKey = _T("IDD_EDIT_MODEL_LIST_DOCK_PANE");
	this->GetWindowText(LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetWindowText(NewLabelText);
	//---------------------------------------------------------------------------------//	
	*/
}
//-------------------------------------------------------------------------------------//
CString CEditModelListDockPane::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_EDIT_MODEL_LIST_DOCK_PANE");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
COLORREF CEditModelListDockPane::GetResultColor(RESULT_ID ResultID)
{
	COLORREF Color;
	const int Divide = 2;
	const TSystemParameter &SystemParam = AOIDataCollect.GetSystemParameter();
	switch ( ResultID )
	{
	case RESULT_ID_NG:
		Color=m_clrNG;
		Color=JetAPI::DivideColor(SystemParam.m_InspectedResultNGColor, Divide);
		break;
	case RESULT_ID_EXCEPTION:
		Color=m_clrNG;	
		Color=SystemParam.m_InspectedResultExceptionColor;	
		break;
	case RESULT_ID_OK:	
		Color=m_clrOK;	
		Color=JetAPI::DivideColor(SystemParam.m_InspectedResultOKColor, Divide);
		break;
	case RESULT_ID_NONE:
		Color=m_clrUnTest;	
		Color=SystemParam.m_InspectedResultUnTestColor;	
		//Color=CLR_DEFAULT;
		break;
	case RESULT_ID_SKIP:
		Color=m_clrBypass;			
		Color=JetAPI::DivideColor(SystemParam.m_InspectedResultSkipColor, Divide);
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
void CEditModelListDockPane::CloseProject()
{
	m_ProjectPtr = NULL;
	ClearComponentListCtrl(m_wndComponentListCtrl);
	ClearModelListCtrl(m_wndModelListCtrl);
}
//-------------------------------------------------------------------------------------//
inline CAOIProject* CEditModelListDockPane::GetActiveProject()
{
	return m_ProjectPtr;
}
//-------------------------------------------------------------------------------------//
inline DISTRICT_ID CEditModelListDockPane::GetActiveDistrictID()
{
	return m_DistrictID;
}
//-------------------------------------------------------------------------------------//
BOOL CEditModelListDockPane::InitModelListCtrl(CThisListCtrl_10 &ListCtrl)
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
	
	str = AOIDataDefine.GetModelText();
	ListCtrl.InsertColumn(nCol, str, Align, width);
	nCol ++;		

	str = AOIDataDefine.GetIndexText();
	ListCtrl.InsertColumn(nCol, str, Align, width2);
	nCol ++;		
	return TRUE;
}
//-------------------------------------------------------------------------------------//
BOOL CEditModelListDockPane::BuildModelListCtrl(CThisListCtrl_10 &ListCtrl)
{
	m_ProjectPtr = NULL;
	if ( ListCtrl.GetSafeHwnd() == NULL ) { return FALSE; }
	ClearComponentListCtrl(m_wndComponentListCtrl);
	ClearModelListCtrl(ListCtrl);
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL == ProjectPtr ) { return TRUE; }
	m_ProjectPtr = ProjectPtr;
	m_DistrictID = ProjectPtr->GetProjectActDistrictID();

	int                   nItem=0;
	int                   nItemUsed=0;
	size_t                i=0, j=0, k=0;
	size_t                NComponents = 0;
	size_t                NIndexMaps = 0;
	size_t                NModelNames = 0;		
	size_t                ModelNameIndex = 0;	
	MODEL_TYPE            ModelType;
	CAOIModel            *ModelPtr = NULL;
	CAOIComponent        *pComponent = NULL;
	const DISTRICT_ID     DistrictID = GetActiveDistrictID();

	std::vector<CSortObj>    ModelNameList;
	std::vector<size_t>      IndexMapList;
	std::vector<MODEL_TYPE>  ModelTypeList;
	std::vector<CString>     GroupNameList;
	CSortObj                 ModelNameNode, *ModelNameNodePtr = NULL;
	CString                  ModelNameS, ModelNameC, GroupNameC, GroupNameS, str;
	
	ModelNameNode.SetSortMode(SORT_BY_TXT);
	NComponents = ProjectPtr->GetProjectComponentCount();	
	for ( i=0; i<NComponents; i++ )
	{
		pComponent = ProjectPtr->GetProjectComponentPtr(i, false);
		if ( NULL == pComponent ) { continue; }		
		pComponent->SetComponentTempIndexModel(-1);
		if ( pComponent->GetComponentDeleted() == true ) { continue; }
		if ( DistrictID != pComponent->GetComponentDistrictID() ) { continue; }

		ModelNameC = pComponent->GetComponentModelName();
		ModelNameC.MakeUpper();

		ModelPtr = pComponent->GetComponentModelPtr();
		ModelType = ModelPtr->GetModelType();

		NModelNames = ModelNameList.size();
		for ( j=0; j<NModelNames; j++ )
		{
			ModelNameNodePtr = &(ModelNameList[j]);
			if ( ModelNameC == ModelNameNodePtr->GetValueStr() )
			{
				pComponent->SetComponentTempIndexModel(j);
				break; 
			}
		}
		if ( j == NModelNames )
		{				
			ModelNameNode.SetID(j);
			ModelNameNode.SetValueStr(ModelNameC);
			ModelNameList.push_back(ModelNameNode);				
			IndexMapList.push_back(-1);
			ModelTypeList.push_back(ModelType);
			pComponent->SetComponentTempIndexModel(j);

			GroupNameC = ModelPtr->GetModelGroupName();
			GroupNameList.push_back(GroupNameC);
		}	
	}

	//依照名稱排序
	std::sort(ModelNameList.begin(), ModelNameList.end());

	//建立引數映射表
	NIndexMaps = IndexMapList.size();
	NModelNames = ModelNameList.size();	
	for ( i=0; i<NModelNames; i++ )
	{
		ModelNameNodePtr = &(ModelNameList[i]);
		ModelNameIndex =  ModelNameNodePtr->GetID();
		if ( ModelNameIndex >= NIndexMaps ) { continue; }
		IndexMapList[ModelNameIndex] = i;
	}
	//更新至零件列表內
	for ( i=0; i<NComponents; i++ )
	{	
		pComponent = ProjectPtr->GetProjectComponentPtr(i, false);
		if ( NULL == pComponent ) { continue; }
		if ( pComponent->GetComponentDeleted() == true ) { continue; }
		ModelNameIndex = pComponent->GetComponentTempIndexModel();
		if ( ModelNameIndex >= NIndexMaps ) { continue; }		

		ModelNameIndex = IndexMapList[ModelNameIndex] ;
		pComponent->SetComponentTempIndexModel(ModelNameIndex);		
	}	
	//---------------------------------------------------------------------------//		
	nItem=0;
	nItemUsed=0;
	ListCtrl.SetRedraw(FALSE);
	m_StopModelListBeSelected = TRUE;
	NModelNames = ModelNameList.size();
	COLORREF UnsetColor = AOIDataCollect.GetColorModelUnset();
	for ( i=0; i<NModelNames; i++ )
	{
		ModelNameNode = ModelNameList[i];

		ModelNameS = ModelNameNode.GetValueStr();
		ModelNameIndex =  ModelNameNode.GetID();
		GroupNameS = GroupNameList[ModelNameIndex];

		m_ModelNameList.push_back(ModelNameS);
		m_GroupNameList.push_back(GroupNameS);

		ModelType = ModelTypeList[ModelNameIndex];
		if ( MODEL_TYPE_NULL == ModelType )
		{	ModelNameS = ModelNameS + _T(" (**)");	}
		else
		{	nItemUsed ++; }

		ListCtrl.InsertItem(nItem, ModelNameS);	
		ListCtrl.SetItemData(nItem, i);

		if ( MODEL_TYPE_NULL == ModelType )
		{	ListCtrl.SetItemTextColor(nItem, UnsetColor); }

		str.Format(_T("%d"), nItem+1);
		ListCtrl.SetItemText(nItem, 1, str);
		nItem ++;
	}
	m_StopModelListBeSelected = FALSE;
	ListCtrl.SetRedraw(TRUE);		
	
	UpdateModelListCtrlTitle(ListCtrl, nItem, nItemUsed);

	if ( nItem > 0 ) 
	{
		pComponent = ProjectPtr->GetProjectActiveComponent();
		if ( NULL == pComponent )
		{	nItem = 0;	}		
		else
		{	nItem = (int)(pComponent->GetComponentTempIndexModel());	}		
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
BOOL CEditModelListDockPane::UpdateModelListCtrl(CThisListCtrl_10 &ListCtrl, bool bForce)
{
	if ( ListCtrl.GetSafeHwnd() == NULL ) { return FALSE; }
	CAOIProject *ProjectPtr = CEditModelListDockPane::GetActiveProject();
	if ( NULL == ProjectPtr ) { return TRUE; }
	CAOIComponent *pComponent = ProjectPtr->GetProjectActiveComponent();
	if ( NULL == pComponent ) { return TRUE; }
	pComponent = pComponent->GetComponentResultPtr();

	CString strGroupName;
	CString strModelName = pComponent->GetComponentModelName();

	int       i=0;
	const int ItemCount = ListCtrl.GetItemCount();

	strModelName.MakeUpper();
	for ( i=0; i<ItemCount; i++ )
	{
		//if ( strModelName != ListCtrl.GetItemText(i, 0) ) { continue; }
		if ( strModelName != m_ModelNameList[i] ) { continue; }		
		strGroupName = m_GroupNameList[i];
		break;
	}
	if ( i == ItemCount ) { return TRUE; }
	const int nItem = i;
	ListCtrl.SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED);		
	const int ShowIndex = JetAPI::GetEnsureVisibleIndex(nItem, ItemCount);
	if ( ShowIndex != nItem )
	{	ListCtrl.EnsureVisible(nItem, FALSE); }
	ListCtrl.EnsureVisible(ShowIndex, FALSE);
	BuildComponentCtrl(m_wndComponentListCtrl, strModelName, strGroupName, bForce);	
	return TRUE;
}
//-------------------------------------------------------------------------------------//
BOOL CEditModelListDockPane::ClearModelListCtrl(CThisListCtrl_10 &ListCtrl)
{
	m_ProjectPtr = NULL;
	m_ComponentIndex = -1;
	m_StopModelListBeSelected = TRUE;	
	ListCtrl.DeleteAllItems();
	m_StopModelListBeSelected = FALSE;	
	m_ModelName = _T("");
	m_GroupName = _T("");
	m_ModelNameList.clear();
	m_GroupNameList.clear();
	return TRUE;
}
//-------------------------------------------------------------------------------------//
BOOL CEditModelListDockPane::RemoveModelListItem(CThisListCtrl_10 &ListCtrl)
{
	return TRUE;
}
//-------------------------------------------------------------------------------------//
bool CEditModelListDockPane::ExecModelListMenu(CPoint point)
{
	CMenu menu;		
	UINT menuID = IDR_MENU_LIST_MODEL;	
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
}
//-------------------------------------------------------------------------------------//
bool CEditModelListDockPane::UpdateModelListCtrlTitle(CThisListCtrl_10 &ListCtrl)
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr )
	{
		UpdateModelListCtrlTitle(ListCtrl, 0, 0);
		return true;
	}

	size_t       i=0;
	size_t       ItemUsed=0;
	CString      ModelName;
	CAOIModel   *ModelPtr = NULL;
	const size_t NModelNames = m_ModelNameList.size();

	ItemUsed = 0;
	for ( i=0; i<NModelNames; i++ )
	{
		ModelName = m_ModelNameList[i];
		ModelPtr = ProjectPtr->GetProjectModelPtrByModelName(ModelName);
		if ( NULL == ModelPtr ) { continue; }
		if ( ModelPtr->GetModelUnsetState() == true ) { continue; }		
		
		ItemUsed ++;
	}	
	UpdateModelListCtrlTitle(ListCtrl, NModelNames, ItemUsed);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditModelListDockPane::UpdateModelListCtrlTitle(CThisListCtrl_10 &ListCtrl, size_t ModelCount, size_t ModelUsed)
{
	CString str;
	CString Name;
	LVCOLUMN col;
	TCHAR buf[MAX_JET_PATH]=_T("");
	::memset(&col, 0x00, sizeof(col));
	str = _T("Model");
	str = LoadMultiLanguageString(str, str);	
	::_stprintf(buf, _T("%s [%d / %d]"), str, ModelUsed, ModelCount);
	col.mask = LVCF_TEXT;//LVCF_WIDTH;
	col.pszText = buf;	
	ListCtrl.SetColumn(0, &col);	
	return true;	
}
//-------------------------------------------------------------------------------------//
BOOL CEditModelListDockPane::InitComponentListCtrl(CThisListCtrl_10 &ListCtrl)
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
	ListCtrl.GetClientRect(&Rect);

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
	return TRUE;
}
//-------------------------------------------------------------------------------------//
BOOL CEditModelListDockPane::BuildComponentCtrl(CThisListCtrl_10 &ListCtrl, LPCTSTR strModelName, LPCTSTR strGroupName, bool bForce)
{
	size_t                i=0;
	bool                  bReBuild=true;
	int                   nItem=0, subIdx=0;
	size_t                NComponents = 0;	
	CAOIComponent        *pComponent = NULL;	
	CString               ModelNameS, ModelNameC, strComponentName, str, ItemText;	
	CAOIProject          *ProjectPtr = GetActiveProject();	
	const DISTRICT_ID     DistrictID = GetActiveDistrictID();

	if ( true==bForce || m_ComponentModelName.CompareNoCase(strModelName)!=0 || m_ComponentGroupName.CompareNoCase(strGroupName)!=0 || NULL==ProjectPtr)
	{	bReBuild = true;	}
	else
	{	bReBuild = false;	}
	
	if ( true == bReBuild )
	{
		ClearComponentListCtrl(ListCtrl);
		if ( NULL == ProjectPtr ) { return TRUE; }
	}	
	
	m_ModelName = strModelName;
	m_GroupName = strGroupName;
	m_ComponentModelName = strModelName;
	m_ComponentGroupName = strGroupName;		
	NComponents = ProjectPtr->GetProjectComponentCount();	
	m_wndPaneBar.SetModelNameEdit(strModelName);	
	m_wndPaneBar.SetGroupNameEdit(strGroupName);	

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
		m_StopComponentListBeSelected = TRUE;
		for ( i=0; i<ItemCount; i++ )
		{
			nItem =i;
			subIdx=0;				
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
			{	ListCtrl.SetItemState(nItem, 0, LVIS_SELECTED); }
			else
			{	ListCtrl.SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED); }

			clrItemText = GetResultColor(ResultID);
			ListCtrl.SetItemTextColor(nItem, clrItemText);
		}
		nItem = ItemCount;
		m_StopComponentListBeSelected = FALSE;	
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

			ModelNameC = pComponent->GetComponentModelName();
			ModelNameC.MakeUpper();
			if ( ModelNameC != strModelName ) { continue; }
		
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
		m_StopComponentListBeSelected = TRUE;
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
		m_StopComponentListBeSelected = FALSE;
		ListCtrl.SetRedraw(TRUE);	
		UpdateComponentListCtrlTitle(ListCtrl, nItem);	
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
BOOL CEditModelListDockPane::ClearComponentListCtrl(CThisListCtrl_10 &ListCtrl)
{
	ClearComponentNodeList();
	m_StopComponentListBeSelected = TRUE;			
	ListCtrl.DeleteAllItems();	
	m_ComponentModelName = _T("");
	m_ComponentGroupName = _T("");	
	m_wndPaneBar.SetModelNameEdit(_T(""));	
	m_wndPaneBar.SetGroupNameEdit(_T(""));
	m_StopComponentListBeSelected = FALSE;
	return TRUE;
}
//-------------------------------------------------------------------------------------//
BOOL CEditModelListDockPane::ShowComponentListSelected(CThisListCtrl_10 &ListCtrl)
{	
	return TRUE;
}
//-------------------------------------------------------------------------------------//
LRESULT CEditModelListDockPane::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
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
			{	BuildModelListCtrl(m_wndModelListCtrl); }
			break;
		case WPARAM_PROJECT_UPDATE:
			if ( CWnd::IsWindowVisible() == TRUE )
			{
				pWnd = (CWnd*)lParam;
				if ( pWnd != this )
				{	UpdateModelListCtrl(m_wndModelListCtrl, false); }
			}
			break;			
		case WPARAM_PROJECT_CLOSE:
			CloseProject();			
			break;				
		case WPARAM_PROJECT_PART_DELETED:
			RemoveModelListItem(m_wndModelListCtrl);
			break;
		case WPARAM_PROJECT_CLOSE_ACTIVE_OBJ:
			break;
		}
		break;
	case MSG_EDIT_PART_LIST_WND:
		switch ( wParam )
		{		
		case WPARAM_BUILD_PART_LIST:
			Res = lParam&LPARAM_BUILD_DOCK_LIST_MODEL;
			if ( 0 != Res )
			{
				BuildModelListCtrl(m_wndModelListCtrl);
				AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_WND_PROPERTY_WND, WPARAM_BUILD_WND_SELECTED, NULL);
			}
			break;
		case WPARAM_UPDATE_PART_LIST:
			UpdateModelListCtrl(m_wndModelListCtrl, false);
			AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_WND_PROPERTY_WND, WPARAM_BUILD_WND_SELECTED, NULL);
			break;	
		case WPARAM_CLEAR_PART_LIST:
			ClearComponentListCtrl(m_wndComponentListCtrl);
			ClearModelListCtrl(m_wndModelListCtrl);
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
void CEditModelListDockPane::OnClickModelListCtrl(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here
	NMITEMACTIVATE *pNMListView = (NMITEMACTIVATE*)pNMHDR;	
	const int nItem = pNMListView->iItem;	
}
//-------------------------------------------------------------------------------------//
void CEditModelListDockPane::OnDbclickModelListCtrl(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here
	NMITEMACTIVATE *pNMListView = (NMITEMACTIVATE*)pNMHDR;	
	const int nItem = pNMListView->iItem;	
	if ( nItem < 0 ) { return; }
	const size_t ModelNameCount=m_ModelNameList.size();
	const size_t GroupNameCount=m_GroupNameList.size();
	size_t Index = m_wndModelListCtrl.GetItemData(nItem);
	if ( Index>=ModelNameCount || Index>=GroupNameCount ) { return; }

	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr )
	{	return; }

	CAOIComponent *ActiveComponentPtr = ProjectPtr->GetProjectActiveComponent();
	AOIDataCollect.CloseActiveComponent(ActiveComponentPtr);
	if ( NULL != ActiveComponentPtr )
	{	ActiveComponentPtr->SetComponentSelected(false);  }

	CString strItem = m_wndModelListCtrl.GetItemText(nItem, 0);
	CString strModel = m_ModelNameList[Index];
	strModel.MakeUpper();
	CAOIComponent *ComponentPtr = NULL;
	const int SubCount = m_wndComponentListCtrl.GetItemCount();		
	if ( SubCount > 0 ) 
	{
		CString strName = m_wndComponentListCtrl.GetItemText(0, 0);
		strName.MakeUpper();
		DWORD_PTR ItemData = m_wndComponentListCtrl.GetItemData(0);
		ComponentPtr = ProjectPtr->GetProjectComponentPtr(ItemData, true);
		//ComponentPtr = ProjectPtr->GetProjectComponentPtrByComponentName(str);
	}	
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
void CEditModelListDockPane::OnItemchangedModelListCtrl(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMITEMACTIVATE *pNMListView = (NMITEMACTIVATE*)pNMHDR;	
	// TODO: Add your control notification handler code here	
	if ( TRUE == m_StopModelListBeSelected ) { return; }
	const int nItem = pNMListView->iItem;	
	if ( nItem < 0 ) { return; }
	DWORD Res = pNMListView->uNewState&LVIS_SELECTED;
	if ( Res == 0 ) { return; }	
	Res = pNMListView->uNewState&LVIS_FOCUSED;
	if ( Res == 0 ) { return; }
	const size_t ModelNameCount=m_ModelNameList.size();
	const size_t GroupNameCount=m_GroupNameList.size();
	size_t Index = m_wndModelListCtrl.GetItemData(nItem);
	if ( Index>=ModelNameCount || Index>=GroupNameCount ) { return; }
	
	CString strItem = m_wndModelListCtrl.GetItemText(nItem, 0);
	CString strModelName = m_ModelNameList[Index];
	CString strGroupName = m_GroupNameList[Index];	
	strModelName.MakeUpper();
	strGroupName.MakeUpper();
	BuildComponentCtrl(m_wndComponentListCtrl, strModelName, strGroupName);
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CEditModelListDockPane::OnClickComponentListCtrl(NMHDR* pNMHDR, LRESULT* pResult)
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
	ItemSelCount=0;
	ItemPos = m_wndComponentListCtrl.GetFirstSelectedItemPosition();
	while ( ItemPos!=NULL )
	{
		nItemSelected = m_wndComponentListCtrl.GetNextSelectedItem(ItemPos);
		ComponentIndex = (unsigned int)m_wndComponentListCtrl.GetItemData(nItemSelected); 
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

	MULTI_BOARD_CTRL_MODE MultiBoardCtrlMode = AOIDataCollect.GetMultiBoardCtrlMode();	
	if ( MULTI_BOARD_CTRL_DISABLE != MultiBoardCtrlMode )
	{
		std::vector<CAOIComponent*> SelComponentList;
		ProjectPtr->GetProjectComponentSelected(SelComponentList);
		ProjectPtr->SelectProjectComponentByMultiBoardCtrlMode(SelComponentList, MultiBoardCtrlMode);
		UpdateComponentListState(m_wndComponentListCtrl);			
	}
	AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_PART_SELECTED, NULL);
	//AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_UPDATE, (LPARAM)(this));
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CEditModelListDockPane::OnDbclickComponentListCtrl(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here	
	unsigned int WindowIndex = 0;
	unsigned int ComponentIndex = 0;
	const int nItem = m_wndComponentListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if ( nItem < 0 ) { return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }	
	
	CAOIWindow    *pWindow = NULL;
	CAOIComponent *pComponent = NULL;		
	ComponentIndex = (unsigned int)m_wndComponentListCtrl.GetItemData(nItem);
	pComponent = ProjectPtr->GetProjectComponentPtr(ComponentIndex, true);
	if ( NULL == pComponent ) { return; }
	AOIDataCollect.MoveStageToComponentOrField(pComponent, true);
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CEditModelListDockPane::OnItemchangedComponentListCtrl(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if ( TRUE == m_StopComponentListBeSelected ) { return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr )	{	return; }
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
	CAOIWindow    *pWindow = NULL;	
	CAOIComponent *pComponent = NULL;
	bool  MultiKey = false;
	const bool PressCtrl = JetAPI::CheckIsPressVRKey(VK_CONTROL);
	const bool PressShift = JetAPI::CheckIsPressVRKey(VK_SHIFT);
	const unsigned int ComponentIndex=(unsigned int)m_wndComponentListCtrl.GetItemData(nItem);
	pComponent = ProjectPtr->GetProjectComponentPtr(ComponentIndex, true);
	WindowIndex = 0;
	pComponent = ProjectPtr->GetProjectActiveComponent();
	AOIDataCollect.CloseActiveComponent(pComponent);
	if ( true==PressCtrl || true==PressShift )
	{
		MultiKey = true;
		WindowIndex = 0;	
	}
	else
	{	
		MultiKey = false;
		//WindowIndex = 0;
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
		UpdateComponentListState(m_wndComponentListCtrl);		
	}
	
	//AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_UPDATE, (LPARAM)(this));
	AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_PART_SELECTED, NULL);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_WND_PROPERTY_WND, WPARAM_BUILD_WND_SELECTED, NULL);
	if ( true == m_MoveToComponent )
	{	AOIDataCollect.MoveStageToComponentOrField(pComponent, false);	}	
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CEditModelListDockPane::OnEndScrollComponentListCtrl(NMHDR* pNMHDR, LRESULT* pResult)
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
}
//-------------------------------------------------------------------------------------//
BOOL CEditModelListDockPane::UpdateComponentListState(CThisListCtrl_10 &ListCtrl)
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
	//CAOIPanel      *PanelPtr = NULL;
	//CAOIBoard      *BoardPtr = NULL;
	CAOIComponent  *pComponent = NULL;	
	const int ItemCount = ListCtrl.GetItemCount();
	
	CAOIModel     *ModelPtr=NULL;
	RESULT_ID      ResultID=RESULT_ID_NONE;
	COLORREF       clrItemText=CLR_DEFAULT;
	COLORREF       clrItemTextBk=CLR_DEFAULT;
	CAOIComponent *ActiveComponentPtr = ProjectPtr->GetProjectActiveComponent();
	ListCtrl.SetRedraw(FALSE);	
	m_StopComponentListBeSelected = TRUE;
	ListCtrl.SetItemState(-1, 0, LVIS_SELECTED);//all items
	for ( i=0; i<ItemCount; i++ )
	{
		ComponentIndex = ListCtrl.GetItemData(i);
		pComponent = ProjectPtr->GetProjectComponentPtr(ComponentIndex, true);
		if ( NULL == pComponent )  { continue; }		
		//PanelPtr = pComponent->GetComponentPanelPtr();
		//if ( NULL == PanelPtr ) { continue; }
		//BoardPtr = pComponent->GetComponentBoardPtr();
		//if ( NULL == BoardPtr ) { continue; }

		strComponentName = pComponent->GetComponentName();		
		ModelPtr = pComponent->GetComponentModelPtr();
		ResultID = ModelPtr->GetModelResultID();		

		subIdx=0;

		//if ( pComponent->GetComponentSelected() == false ) 
		//{	ListCtrl.SetItemState(i, 0, LVIS_SELECTED|LVIS_FOCUSED); }
		//else
		//{	ListCtrl.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED); }
		if ( pComponent->GetComponentSelected() == true ) 
		{	ListCtrl.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED); }

		clrItemText = GetResultColor(ResultID);
		ListCtrl.SetItemTextColor(i, clrItemText);

		//PanelIndex = PanelPtr->GetPanelIndex_Project();
		//BoardIndex = BoardPtr->GetBoardIndex_Panel();
		PanelIndex = pComponent->GetComponentPanelIndex_Project();
		BoardIndex = pComponent->GetComponentBoardIndex_Panel();
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
	m_StopComponentListBeSelected = FALSE;
	ListCtrl.SetRedraw(TRUE);		
	return TRUE;
}
//-------------------------------------------------------------------------------------//
BOOL CEditModelListDockPane::ExecComponentListMenu(CPoint point)
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

	UpdateComponentListState(m_wndComponentListCtrl);
	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	return TRUE;
}
//-------------------------------------------------------------------------------------//
bool CEditModelListDockPane::UpdateComponentListCtrlTitle(CThisListCtrl_10 &ListCtrl, size_t ComponentCount)
{
	CString str;
	CString Name;
	LVCOLUMN col;
	TCHAR buf[MAX_JET_PATH]=_T("");
	const int NodeCount=(int)(m_ComponentNodeList.size());

	::memset(&col, 0x00, sizeof(col));
	str = _T("Component");
	str = LoadMultiLanguageString(str, str);	
	if ( 0==NodeCount || ComponentCount==NodeCount )
	{	::_stprintf(buf, _T("%s [%d]"), str, ComponentCount); }
	else
	{	::_stprintf(buf, _T("%s [%d/%d]"), str, ComponentCount, NodeCount);	}
	col.mask = LVCF_TEXT;//LVCF_WIDTH;
	col.pszText = buf;	
	ListCtrl.SetColumn(0, &col);	
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditModelListDockPane::ClearComponentNodeList()
{
	m_ComponentNodeList.clear();
}
//-------------------------------------------------------------------------------------//
bool CEditModelListDockPane::BuildNextComponentCtrl()
{
	CAOIProject          *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }
	std::vector<CSortObj> &NodeList=m_ComponentNodeList;
	const int NodeCount=(int)(NodeList.size());
	if ( 0 == NodeCount ) { return true; }
	CThisListCtrl_10 &ListCtrl=m_wndComponentListCtrl;
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
	m_StopComponentListBeSelected = TRUE;
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
	m_StopComponentListBeSelected = FALSE;
	ListCtrl.SetRedraw(TRUE);	

	UpdateComponentListCtrlTitle(ListCtrl, nItem);	

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
void CEditModelListDockPane::BuildComponentNodeList(const std::vector<CSortObj> &SortList)
{
	ClearComponentNodeList();
	m_ComponentNodeList = SortList;	
	return;
}
//-------------------------------------------------------------------------------------//
int CEditModelListDockPane::FindItemPreious_Any(CThisListCtrl_10 &ListCtrl)
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return -1; }			

	int        i=0;
	size_t     Index=0;
	int        nItemNext=-1;
	CString    ModelName;
	bool       bFind=false;		
	const int  nItem = ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	const int  ItemCount = ListCtrl.GetItemCount();
	const int  ModelCount = (int)(m_ModelNameList.size());	

	bFind = false;
	nItemNext=-1;
	for ( i=nItem-1; i>=0; i-- )
	{
		Index = ListCtrl.GetItemData(i);
		if ( Index >= ModelCount ) { continue; }
		ModelName = m_ModelNameList[Index];				
		nItemNext = i;
		break;
	}
	if ( -1 == nItemNext )
	{
		for ( i=ItemCount-1; i>nItem; i-- )
		{
			Index = ListCtrl.GetItemData(i);
			if ( Index >= ModelCount ) { continue; }
			ModelName = m_ModelNameList[Index];			
			nItemNext = i;
			break;
		}
	}	
	return nItemNext;		
}
//-------------------------------------------------------------------------------------//
int CEditModelListDockPane::FindItemPreious_Set(CThisListCtrl_10 &ListCtrl)
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return -1; }		

	int        i=0;
	size_t     Index=0;
	int        nItemNext=-1;
	CString    ModelName;
	bool       bFind=false;		
	CAOIModel *ModelPtr = NULL;
	const int  nItem = ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	const int  ItemCount = ListCtrl.GetItemCount();
	const int  ModelCount = (int)(m_ModelNameList.size());	

	bFind = false;
	nItemNext=-1;
	for ( i=nItem-1; i>=0; i-- )
	{
		Index = ListCtrl.GetItemData(i);
		if ( Index >= ModelCount ) { continue; }
		ModelName = m_ModelNameList[Index];
		ModelPtr = ProjectPtr->GetProjectModelPtrByModelName(ModelName);
		if ( NULL == ModelPtr )
		{	continue;	}
		if ( ModelPtr->GetModelUnsetState() == true )
		{	continue; }		
		nItemNext = i;
		break;
	}
	if ( -1 == nItemNext )
	{
		for ( i=ItemCount-1; i>nItem; i-- )
		{
			Index = ListCtrl.GetItemData(i);
			if ( Index >= ModelCount ) { continue; }
			ModelName = m_ModelNameList[Index];
			ModelPtr = ProjectPtr->GetProjectModelPtrByModelName(ModelName);
			if ( NULL == ModelPtr )
			{	continue;	}
			if ( ModelPtr->GetModelUnsetState() == true )
			{	continue; }
			nItemNext = i;
			break;
		}
	}
	return nItemNext;	
}
//-------------------------------------------------------------------------------------//
int CEditModelListDockPane::FindItemPreious_UnSet(CThisListCtrl_10 &ListCtrl)
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return -1; }			

	int        i=0;
	size_t     Index=0;
	int        nItemNext=-1;
	CString    ModelName;
	bool       bFind=false;		
	CAOIModel *ModelPtr = NULL;
	const int  nItem = ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	const int  ItemCount = ListCtrl.GetItemCount();
	const int  ModelCount = (int)(m_ModelNameList.size());	

	bFind = false;
	nItemNext=-1;
	for ( i=nItem-1; i>=0; i-- )
	{
		Index = ListCtrl.GetItemData(i);
		if ( Index >= ModelCount ) { continue; }
		ModelName = m_ModelNameList[Index];
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
			if ( Index >= ModelCount ) { continue; }
			ModelName = m_ModelNameList[Index];
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
void CEditModelListDockPane::OnSwitchPreiousBtn()
{
	int nItemNext = -1;
	SWITCH_MODEL_ITEM_MODE SwitchItemMode;
	CThisListCtrl_10 &ListCtrl = m_wndModelListCtrl;
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

	m_SwitchModelBtn = true;
	m_MoveToComponent = false;	
	ListCtrl.SetItemState(nItemNext, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);	

	const int ItemCount = ListCtrl.GetItemCount();
	const int ShowIndex = JetAPI::GetEnsureVisibleIndex(nItemNext, ItemCount);
	if ( ShowIndex != nItemNext )
	{	ListCtrl.EnsureVisible(nItemNext, FALSE); }
	ListCtrl.EnsureVisible(ShowIndex, FALSE);		
	m_SwitchModelBtn = false;
	m_MoveToComponent = true;
	CAOIComponent *ComponentPtr = ProjectPtr->GetProjectActiveComponent();
	AOIDataCollect.MoveStageToComponentOrField(ComponentPtr, true);
}
//-------------------------------------------------------------------------------------//
int CEditModelListDockPane::FindItemNext_Any(CThisListCtrl_10 &ListCtrl)
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return -1; }			

	int        i=0;
	size_t     Index=-1;
	int        nItemNext=-1;
	CString    ModelName;
	bool       bFind=false;		
	const int  nItem = ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	const int  ItemCount = ListCtrl.GetItemCount();
	const int  ModelCount = (int)(m_ModelNameList.size());	

	bFind = false;
	nItemNext=-1;
	for ( i=nItem+1; i<ItemCount; i++ )
	{		
		Index = ListCtrl.GetItemData(i);
		if ( Index >= ModelCount ) { continue; }
		ModelName = m_ModelNameList[Index];		
		nItemNext = i;
		break;
	}
	if ( -1 == nItemNext )
	{
		for ( i=0; i<nItem; i++ )
		{
			Index = ListCtrl.GetItemData(i);
			if ( Index >= ModelCount ) { continue; }
			ModelName = m_ModelNameList[Index];			
			nItemNext = i;
			break;
		}
	}
	return nItemNext;	
}
//-------------------------------------------------------------------------------------//
int CEditModelListDockPane::FindItemNext_Set(CThisListCtrl_10 &ListCtrl)
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return -1; }			

	int        i=0;
	size_t     Index=-1;
	int        nItemNext=-1;	
	CString    ModelName;
	bool       bFind=false;		
	CAOIModel *ModelPtr = NULL;
	const int  nItem = ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	const int  ItemCount = ListCtrl.GetItemCount();
	const int  ModelCount = (int)(m_ModelNameList.size());	

	bFind = false;
	nItemNext=-1;
	for ( i=nItem+1; i<ItemCount; i++ )
	{
		Index = ListCtrl.GetItemData(i);
		if ( Index >= ModelCount ) { continue; }
		ModelName = m_ModelNameList[Index];
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
			if ( Index >= ModelCount ) { continue; }
			ModelName = m_ModelNameList[Index];
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
int CEditModelListDockPane::FindItemNext_UnSet(CThisListCtrl_10 &ListCtrl)
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return -1; }

	int        i=0;
	size_t     Index=-1;
	int        nItemNext=-1;
	CString    ModelName;
	bool       bFind=false;		
	CAOIModel *ModelPtr = NULL;
	const int  nItem = ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	const int  ItemCount = ListCtrl.GetItemCount();
	const int  ModelCount = (int)(m_ModelNameList.size());	

	bFind = false;
	nItemNext=-1;
	for ( i=nItem+1; i<ItemCount; i++ )
	{
		Index = ListCtrl.GetItemData(i);
		if ( Index >= ModelCount ) { continue; }
		ModelName = m_ModelNameList[Index];
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
			if ( Index >= ModelCount ) { continue; }
			ModelName = m_ModelNameList[Index];
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
void CEditModelListDockPane::OnSwitchNextBtn()
{
	int nItemNext = -1;
	SWITCH_MODEL_ITEM_MODE NextModelType;
	CThisListCtrl_10 &ListCtrl = m_wndModelListCtrl;
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
	m_SwitchModelBtn = true;
	m_MoveToComponent = false;		
	ListCtrl.SetItemState(nItemNext, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);	
	const int ItemCount = ListCtrl.GetItemCount();
	const int ShowIndex = JetAPI::GetEnsureVisibleIndex(nItemNext, ItemCount);
	if ( ShowIndex != nItemNext )
	{	ListCtrl.EnsureVisible(nItemNext, FALSE); }
	ListCtrl.EnsureVisible(ShowIndex, FALSE);
	m_SwitchModelBtn = false;
	m_MoveToComponent = true;
	CAOIComponent *ComponentPtr = ProjectPtr->GetProjectActiveComponent();
	AOIDataCollect.MoveStageToComponentOrField(ComponentPtr, true);
}
//-------------------------------------------------------------------------------------//
void CEditModelListDockPane::OnModelBypass()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	
	CThisListCtrl_10 &ListCtrl = m_wndModelListCtrl;
	const int nItem = ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if ( nItem < 0 ) { return; }
	const size_t ModelNameCount=m_ModelNameList.size();
	const size_t GroupNameCount=m_GroupNameList.size();
	size_t Index = ListCtrl.GetItemData(nItem);
	if ( Index>=ModelNameCount || Index>=GroupNameCount ) { return; }	
	if ( AOIDataCollect.OperateLevelEditFuncBypassComponent() == false )	{	return ; }

	CString ModelName;
	ModelName = m_ModelNameList[Index];
	ModelName.MakeUpper();
	ProjectPtr->SelectProjectAllComponents(false);
	ProjectPtr->SelectProjectComponentsByModelName(ModelName, false);	
	ProjectPtr->SwitchProjectComponentBypassed();	
	LogOperCtrl.SaveLogProjectComponentSelectedBypassed(ProjectPtr);
	UpdateComponentListState(m_wndComponentListCtrl);
}
//-------------------------------------------------------------------------------------//
void CEditModelListDockPane::OnModelSearch()
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	
	CThisListCtrl_10 &ListCtrl = m_wndModelListCtrl;
	const int nItem = ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if ( nItem < 0 ) { return; }
	const size_t ModelNameCount=m_ModelNameList.size();
	const size_t GroupNameCount=m_GroupNameList.size();
	size_t Index = m_wndModelListCtrl.GetItemData(nItem);
	if ( Index>=ModelNameCount || Index>=GroupNameCount ) { return; }

	int          i=0;
	CString      str, str2;
	CString      GroupName;
	CString      ModelName;
	CString      strValue;
	CString      strLabel;
	CString      strCaption;	
	CInputBoxWnd InputBox;
	const int    ModelCount = (int)(m_ModelNameList.size());

	GroupName = m_GroupNameList[Index];
	ModelName = m_ModelNameList[Index];
	strCaption = _T("Input Model Name Wnd");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strLabel = AOIDataDefine.GetModelText();
	strValue = ModelName;
	InputBox.SetParam1(strCaption, strLabel, strValue);
	if ( InputBox.DoModal() == IDCANCEL ) 
	{	return ; }	
	
	int    nNextItem=-1;
	strValue = InputBox.m_DataEdit1;
	for ( i=nItem+1; i<ModelCount; i++ )
	{
		GroupName = m_GroupNameList[i];
		ModelName = m_ModelNameList[i];
		if ( JetAPI::FindTextInString(strValue, ModelName) == false )
		{	continue; }
		nNextItem = i;
		break;
	}	
	if ( -1 == nNextItem )
	{
		for ( i=0; i<ModelCount; i++ )
		{
			GroupName = m_GroupNameList[i];
			ModelName = m_ModelNameList[i];
			if ( JetAPI::FindTextInString(strValue, ModelName) == false )
			{	continue; }
			nNextItem = i;
			break;
		}
	}
	if ( -1 == nNextItem )
	{
		str = _T("Can not find model");
		str = LoadMultiLanguageString(str, str);
		strValue = InputBox.m_DataEdit1;	
		str2.Format(_T("%s [%s]"), str, strValue);
		JetAPI::ShowMessageBox(str2);
		return;
	}
	nNextItem = JetAPI::GetListCtrlItemByData(ListCtrl, nNextItem);
	if ( -1 == nNextItem )
	{	return; }

	const int ItemCount = ListCtrl.GetItemCount();
	ListCtrl.SetItemState(nNextItem, LVIS_SELECTED, LVIS_SELECTED);
	const int ShowIndex = JetAPI::GetEnsureVisibleIndex(nNextItem, ItemCount);
	if ( ShowIndex != nNextItem )
	{	ListCtrl.EnsureVisible(nNextItem, FALSE); }
	ListCtrl.EnsureVisible(ShowIndex, FALSE);		

	BuildComponentCtrl(m_wndComponentListCtrl, ModelName, GroupName);
}
//-------------------------------------------------------------------------------------//
void CEditModelListDockPane::OnModelApply()
{
	ExecModelApply();
	return;
}
//-------------------------------------------------------------------------------------//
void CEditModelListDockPane::OnModelImport()
{
	ExecModelImport();
	return;
}
//-------------------------------------------------------------------------------------//
void CEditModelListDockPane::ExecModelApply()
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	

	CString ModelName;	
	CThisListCtrl_10   &ListCtrl = m_wndModelListCtrl;
	const int nItem = ListCtrl.GetNextItem(-1, LVNI_FOCUSED);
	if ( -1 == nItem ) { return ; }
	const size_t ModelNameCount=m_ModelNameList.size();
	const size_t GroupNameCount=m_GroupNameList.size();
	size_t Index = ListCtrl.GetItemData(nItem);
	if ( Index>=ModelNameCount || Index>=GroupNameCount ) { return ; }
	ModelName = m_ModelNameList[Index];
	
	ExecModelReference(ModelName, ProjectPtr);
	
	ListCtrl.SetItemText(nItem, 0, ModelName);
	ListCtrl.SetItemTextColor(nItem, CLR_DEFAULT);
	UpdateModelListCtrlTitle(m_wndModelListCtrl);
	AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_UPDATE, (LPARAM)this);	
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditModelListDockPane::ExecModelImport()
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	
	CString    str;
	CString    ModelName;
	CString    GroupName;	
	CAOIModel *ModelPtr = NULL;
	MODEL_TYPE ModelType=MODEL_TYPE_NULL;	
	CHIP_SIZE_MODE ChipSizeMode = CHIP_SIZE_NONE;
	CThisListCtrl_10   &ListCtrl = m_wndModelListCtrl;
	const int nItem = ListCtrl.GetNextItem(-1, LVNI_FOCUSED);
	if ( -1 == nItem ) { return ; }
	const size_t ModelNameCount=m_ModelNameList.size();
	const size_t GroupNameCount=m_GroupNameList.size();
	size_t Index = ListCtrl.GetItemData(nItem);
	if ( Index>=ModelNameCount || Index>=GroupNameCount ) { return ; }
	ModelName = m_ModelNameList[Index];

	CProjectListWnd ProjectListWnd;	
	if ( ProjectListWnd.DoModal() == IDCANCEL ) { return ; }
	CString filename = ProjectListWnd.GetSelectedFilename();	
	CString ProjectName = ProjectPtr->GetProjectShowName();
	if ( ProjectName.CompareNoCase(filename) == 0 ) 
	{
		str = _T("Error, Can not import the model from current project!");
		str = LoadMultiLanguageString(str, str);
		JetAPI::ShowMessageBox(str);
		return ;
	}

	CAOIProject *ImportProjectPtr = AOIObjManager.CreateProjectObj();
	if ( NULL == ImportProjectPtr ) 
	{ 
		str = AOIObjManager.GetErrorString();		
		JetAPI::ShowMessageBox(str);
		return ; 
	}		
	
	CString Folder;		
	CString tmpfilename;	
	const bool bLibraryMode = true;
	if ( AOIDataCollect.CreateTempProjectFile(filename, tmpfilename) == false )
	{
		AOIObjManager.DestroyProjectObj(ImportProjectPtr);
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return ;
	}
	if ( ImportProjectPtr->LoadProject(tmpfilename, filename, bLibraryMode) == false )
	{	
		str = ImportProjectPtr->GetErrorString();
		AOIObjManager.DestroyProjectObj(ImportProjectPtr);
		JetAPI::ShowMessageBox(str);
		return ;
	}

	ExecModelReference(ModelName, ImportProjectPtr);

	AOIObjManager.DestroyProjectObj(ImportProjectPtr);
	ImportProjectPtr = NULL;
	ListCtrl.SetItemText(nItem, 0, ModelName);
	ListCtrl.SetItemTextColor(nItem, CLR_DEFAULT);
	UpdateModelListCtrlTitle(m_wndModelListCtrl);
	AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_UPDATE, (LPARAM)this);	
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditModelListDockPane::ExecModelReference(CString ModelName, CAOIProject *LibraryProjectPtr)
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	
	if ( NULL == LibraryProjectPtr ) { return ; }
	
	CString    str;
	TREGION4D  BodyRgn;
	CString    GroupName;
	CAOIModel *ModelPtr = NULL;	
	MODEL_TYPE ModelType = MODEL_TYPE_NULL;
	CHIP_SIZE_MODE ChipSizeMode = CHIP_SIZE_NONE;
	ModelPtr = ProjectPtr->GetProjectModelPtrByModelName(ModelName);
	if ( NULL != ModelPtr )
	{	
		ModelType = ModelPtr->GetModelType(); 
		ModelPtr->GetModelBodyRegion(BodyRgn);
		GroupName = ModelPtr->GetModelGroupName();		
	}
	
	CAOIComponent *ActComponentPtr = ProjectPtr->GetProjectActiveComponent();
	if ( NULL != ActComponentPtr )
	{
		CString ModelName_C=ActComponentPtr->GetComponentModelName();
		if ( ModelName_C.CompareNoCase(ModelName) == 0 )
		{
			CAOIModel *ModelPtr_C=ActComponentPtr->GetComponentModelPtr();
			if ( NULL != ModelPtr_C )
			{
				CAOIBox BodyBox=ModelPtr_C->GetModelBodyBox();
				const double AttatchAngle=ModelPtr_C->GetModelAttachedAngle();
				const bool IsExceptionAngle=JetAPI::CheckIsExceptionAngle(AttatchAngle);
				if ( true == IsExceptionAngle )
				{	BodyBox.RotateBox(-AttatchAngle, 0, 0);	}
				BodyBox.GetBoxRegion(BodyRgn);
			}
		}
	}

	CProjectLibraryWnd ProjectLibraryWnd;
	ProjectLibraryWnd.SetActiveProject(LibraryProjectPtr);
	ProjectLibraryWnd.SetBodyRegion(BodyRgn);
	ProjectLibraryWnd.SetModelType(ModelType);
	ProjectLibraryWnd.SetSearchName(ModelName);	
	if ( ProjectLibraryWnd.DoModal() == IDCANCEL ) 
	{	return ;		}

	CAOIModel *RefModelPtr = ProjectLibraryWnd.GetModelPtr();;
	if ( NULL == RefModelPtr )
	{	return ;		}

	CString NewModelName = ModelName;
	CString RefModelName = RefModelPtr->GetModelName();	
	
	bool         bReplaceModel=false;	
	if ( NULL == ModelPtr ) 
	{	bReplaceModel = false;	}
	else
	{	bReplaceModel = true; }
	ModelPtr = RefModelPtr->CloneModelObj();
	if ( NULL == ModelPtr )
	{
		str = _T("Error, Clone Model Object Fault");
		JetAPI::ShowMessageBox(str);
		return ;
	}

	CString      strName;
	CString      strValue;
	CString      strCaption;
	CInputBoxWnd InputBox;
	MODEL_TYPE ModelTypeExist;
	str = _T("Input Model Group Name Wnd");
	strCaption = LoadMultiLanguageString(str, str);
	str = _T("Group Name");
	strName = LoadMultiLanguageString(str, str);
	str = ModelPtr->GetModelGroupName();
	const MODEL_TYPE ModelType2 = ModelPtr->GetModelType();
	while ( true )
	{
		strValue = str;
		InputBox.SetParam1(strCaption, strName, strValue);
		if ( InputBox.DoModal() == IDCANCEL )
		{	
			str = ModelPtr->GetModelGroupName();
			break; 
		}
		str = InputBox.m_DataEdit1;
		ModelTypeExist = ProjectPtr->CheckProjectModelGroupNameModelType(str);
		if ( ModelType2 == ModelTypeExist )
		{	break; }
		if ( MODEL_TYPE_NULL == ModelTypeExist )
		{	break; }
		continue;
	};
	GroupName = str;
	ModelPtr->SetModelGroupName(str);	

	bool    bScaleModle = true;
	bool    bExecModelSize=false;
	CAOIComponent *ComponentPtr = ProjectPtr->GetProjectActiveComponent();	
	if ( NULL!=ComponentPtr && true==bScaleModle  )
	{	
		str = _T("Do you want to scale the mode?");
		str = LoadMultiLanguageString(str, str);
		double ComponentAngle = ComponentPtr->GetComponentAngle();
		CAOIModel *ModelPtr_C = ComponentPtr->GetComponentModelPtr();
		if ( NULL != ModelPtr_C )
		{
			CAOIBox BodyBox;
			double BodyCx=0, BodyCy=0;			
			BodyBox = ModelPtr_C->GetModelBodyBox();
			BodyBox.RotateBox(-ComponentAngle, 0, 0);
			BodyBox.GetBoxSize(BodyCx, BodyCy);
			if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDYES )
			{	
				bExecModelSize = true;
				ModelPtr->ScaleModel(BodyCx, BodyCy);	
			}			
		}
	}

	size_t  i=0;	
	TREGION4D Region;
	TPOINT2D CornorPos[4];
	CString RefModelFolder;
	CString RefModelBKName;
	CString NewModelFolder;
	CString NewModelBKName;
	MODEL_TYPE NewModelType = ModelPtr->GetModelType();
	const double SpaceRatio = ProjectPtr->GetProjectSpaceToGrayRatio();
	CString NewLibraryFolder = ProjectPtr->GetProjectLibraryFolder();
	CString RefLibraryFolder = LibraryProjectPtr->GetProjectLibraryFolder();
	const double DBL_PRESION = 0.001;	

	ModelPtr->GetModelBodyBox().GetBoxCornerPos(CornorPos);	
	JetAPI::PointsToRegion(CornorPos, 4, Region);
	if ( CHIP_SIZE_NONE == ChipSizeMode)
	{	ChipSizeMode = CAOIModel::FindModelChipSizeMode(NewModelType, Region); }	
	ModelPtr->SetModelChipSizeMode(ChipSizeMode);

	RefModelFolder.Format(_T("%s\\%s"), RefLibraryFolder, RefModelName);
	NewModelFolder.Format(_T("%s\\%s"), NewLibraryFolder, NewModelName);
  	JetAPI::CopyFolderAToFolderB(RefModelFolder, NewModelFolder, false, true, _T(""), -1, -1);
	//更改底圖名字
	for ( i=0; i<FRAME_MAX_COUNT; i++ )
	{	
		RefModelBKName = AOIDataDefine.GetModelBKImageFilename(NewModelFolder, RefModelName, i);
		NewModelBKName = AOIDataDefine.GetModelBKImageFilename(NewModelFolder, NewModelName, i);		
		if ( RefModelBKName.CompareNoCase(NewModelBKName) == 0 ) { continue; }			
		::MoveFile(RefModelBKName, NewModelBKName);
		//::CopyFile(RefModelBKName, NewModelBKName, FALSE);		
		//::DeleteFile(RefModelBKName);
	}

	//更新符合專案的模組參數
	unsigned int   DefaultFrameIndex=0;
	unsigned int   DefaultFrameUniqueID = 0;	
	std::vector<CColorGroup>  ColorGroupList;
	std::vector<unsigned int> FrameIndexMapList;
	ProjectPtr->CloneProjectColorGroupList(ColorGroupList);
	ProjectPtr->BuildProjectFrameIndexMapParam(FrameIndexMapList, DefaultFrameIndex, DefaultFrameUniqueID);			

	ModelPtr->UnSelectModel();
	ModelPtr->SetModelName(NewModelName);
	if ( GroupName.GetLength()>0 && NewModelType==ModelType )
	{	ModelPtr->SetModelGroupName(GroupName);	}
	ModelPtr->SetModelFolderModel(NewModelFolder);
	ModelPtr->AssignModelFolder();
	ModelPtr->SetupkModelModifiedDateTime();
	ModelPtr->CalcModelTotalRegionAll();
	ModelPtr->SetModelBodyBoxActived(true);
	ModelPtr->SetModelNeedSaveFiles(true);
	ModelPtr->SetModelBKImageNeedToGrab(true);	
	ModelPtr->UpdateModelColorGroupLinkIndex(ColorGroupList);
	ModelPtr->UpdateModelFrameIndex(DefaultFrameIndex, DefaultFrameUniqueID, FrameIndexMapList);
	if ( false == bReplaceModel )
	{	ProjectPtr->AddProjectModelPtr(ModelPtr, false); }
	else
	{	
		if ( ProjectPtr->ReplaceProjectModel(ModelPtr) == false )
		{
			AOIObjManager.DestroyModelObj(ModelPtr);			
			return ;
		}
	}

	//計算新的模組屬性
	ComponentPtr = ProjectPtr->GetProjectActiveComponent();	
	if ( NULL!=ComponentPtr && true==bExecModelSize  )
	{	
		ComponentPtr->UpdateComponentModelFromLibrary(ModelPtr);//先更新這顆零件
		CAOIModel *ModelPtr_C = ComponentPtr->GetComponentModelPtr();
		if ( NULL != ModelPtr_C )
		{				
			//計算基本資料			
			bool  bClone = true;
			bool  bGetUniFrameList=false;
			const int nAlign = 4;			
			const bool bNoFilter = false;
			std::vector<TUNI_FRAME> ModelUniFrameList;
			ModelPtr_C->CalcModelTotalRegionAll();
			if ( AOIDataCollect.CheckModelUniFrameListModelPtr(ModelPtr_C) == true )
			{	
				if ( AOIDataCollect.CopyModelUniFrameList(ModelUniFrameList, bClone) == true )
				{	bGetUniFrameList = true;	}
			}

			if ( false == bGetUniFrameList )
			{
				if ( AOIDataCollect.CreateModelUniFrameList(ModelPtr_C, ModelUniFrameList, nAlign, bNoFilter) == true ) 
				{	
					bClone = true;	
					bGetUniFrameList = true;
				#ifdef _DEBUG
					str.Format(_T("%s\\ModelUniFrameTest.PNG"), AOIDataCollect.GetAOITempDirectory());
					ImageAPI.SaveUniFrameImage(str, ModelUniFrameList, true, true, false, false, SpaceRatio);
				#endif//_DEBUG
				}
			}

			if ( true == bGetUniFrameList )
			{
				if ( ModelPtr_C->AnalysisModelProperty(ModelUniFrameList) == true ) 
				{	
					ModelPtr->CloneModelProperty(ModelPtr_C);	
					ModelPtr->ApplyModelPropertyToAlgParam();
				}
				if ( true == bClone )
				{	JetAPI::ClearUniFrameList(ModelUniFrameList);	}			
			}
		}
	}	

	ComponentPtr = ProjectPtr->GetProjectActiveComponent();
	ProjectPtr->SelectProjectAllComponents(false);
	ProjectPtr->SelectProjectComponentsByModelName(NewModelName, false);
	ProjectPtr->ApplyProjectModelToComponentsSelected(ModelPtr);
	ProjectPtr->SelectProjectAllComponents(false);
	if ( NULL != ComponentPtr )
	{	ComponentPtr->SetComponentSelected(true); }	
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditModelListDockPane::OnModelClone()
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	

	CString    str;
	CString    ModelName;
	CAOIModel *ModelPtr = NULL;
	MODEL_TYPE ModelType=MODEL_TYPE_NULL;	
	CThisListCtrl_10   &ListCtrl = m_wndModelListCtrl;
	const int nItem = ListCtrl.GetNextItem(-1, LVNI_FOCUSED);
	if ( -1 == nItem ) { return; }	
	const size_t ModelNameCount=m_ModelNameList.size();
	const size_t GroupNameCount=m_GroupNameList.size();
	size_t Index = ListCtrl.GetItemData(nItem);
	if ( Index>=ModelNameCount || Index>=GroupNameCount ) { return; }
	ModelName = m_ModelNameList[Index];
	ModelPtr = ProjectPtr->GetProjectModelPtrByModelName(ModelName);
	if ( NULL == ModelPtr )
	{
		str = _T("Error, Can not clone the empty model.");
		str = LoadMultiLanguageString(str, str);
		JetAPI::ShowMessageBox(str);
		return;
	}
	
	DWORD        Res=0;	
	CString      strLabel;
	CString      strCaption;
	bool         bReplaceModel=false;
	CInputBoxWnd InputBox;
	CString srcModelName = ModelPtr->GetModelName();
	CString NewModelName = srcModelName;
	POINT  WndCp={0};
	RECT   WndRect={0};
	CWnd::GetWindowRect(&WndRect);
	WndCp.x = (WndRect.left+WndRect.right)/2;
	WndCp.y = (WndRect.top+WndRect.bottom)/2;	
	//InputBox.SetWndPos(WndCp);

	strCaption = _T("Input Model Name");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strLabel = _T("Name:");
	strLabel = LoadMultiLanguageString(strLabel, strLabel);
	while ( true )
	{		
		InputBox.SetParam1(strCaption, strLabel, NewModelName);
		if ( InputBox.DoModal() == IDCANCEL ) { return ; }

		NewModelName = InputBox.m_DataEdit1;
		NewModelName.MakeUpper();
		if ( NewModelName == srcModelName )
		{	continue;	}

		if ( ProjectPtr->CheckProjectModelNameExist(NewModelName) == true ) 
		{
			str.Format(_T("The model exist already, do you want to replace it?"));
			str = LoadMultiLanguageString(str, str);
			Res = JetAPI::ShowMessageBox(str, MB_YESNOCANCEL);
			if ( IDCANCEL == Res )
			{	return ; }
			if ( IDNO == Res )
			{	continue;  }			
			bReplaceModel = true;
		}
		break;
	};	
	NewModelName = InputBox.m_DataEdit1;	
	NewModelName.MakeUpper();
	
	bool IsOK = true;
	ModelPtr = ProjectPtr->CloneProjectModel(srcModelName, NewModelName);
	if ( NULL == ModelPtr )
	{	
		str = ProjectPtr->GetErrorString();
		JetAPI::ShowMessageBox(str);
		return;
	}
	ModelPtr->SetupkModelModifiedDateTime();
	if ( false == bReplaceModel )
	{	ProjectPtr->AddProjectModelPtr(ModelPtr, false); }
	else
	{	
		if ( NULL == ProjectPtr->ReplaceProjectModel(ModelPtr) )
		{	AOIObjManager.DestroyModelObj(ModelPtr); }
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CEditModelListDockPane::OnModelRename()
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	

	CString    str;
	CString    ModelName;
	CAOIModel *ModelPtr = NULL;
	MODEL_TYPE ModelType=MODEL_TYPE_NULL;	
	CThisListCtrl_10   &ListCtrl = m_wndModelListCtrl;
	const int nItem = ListCtrl.GetNextItem(-1, LVNI_FOCUSED);
	if ( -1 == nItem ) { return; }
	const size_t ModelNameCount=m_ModelNameList.size();
	const size_t GroupNameCount=m_GroupNameList.size();
	size_t Index = ListCtrl.GetItemData(nItem);
	if ( Index>=ModelNameCount || Index>=GroupNameCount ) { return; }	
	ModelName = m_ModelNameList[Index];
	ModelPtr = ProjectPtr->GetProjectModelPtrByModelName(ModelName);
	if ( NULL == ModelPtr )
	{	ModelType=MODEL_TYPE_NULL; }	
	else 
	{	ModelType = ModelPtr->GetModelType(); }
	
	CString      strLabel;
	CString      strCaption;
	CInputBoxWnd InputBox;
	CString srcModelName = ModelName;
	CString NewModelName = srcModelName;
	POINT  WndCp={0};
	RECT   WndRect={0};
	CWnd::GetWindowRect(&WndRect);
	WndCp.x = (WndRect.left+WndRect.right)/2;
	WndCp.y = (WndRect.top+WndRect.bottom)/2;	
	//InputBox.SetWndPos(WndCp);

	strCaption = _T("Input Model Name");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strLabel = _T("Name:");
	strLabel = LoadMultiLanguageString(strLabel, strLabel);
	while ( true )
	{		
		InputBox.SetParam1(strCaption, strLabel, NewModelName);
		if ( InputBox.DoModal() == IDCANCEL ) { return ; }

		NewModelName = InputBox.m_DataEdit1;
		NewModelName.MakeUpper();
		NewModelName.TrimLeft();//剔除左邊
		NewModelName.TrimRight();//剔除右邊
		if ( NewModelName.GetLength() == 0 ) 
		{	continue; }
		if ( NewModelName == srcModelName ) { return ; }
		if ( ProjectPtr->CheckProjectModelNameExist(NewModelName) == true ) 
		{	continue; }		
		break;
	};	
	NewModelName = InputBox.m_DataEdit1;	
	NewModelName.MakeUpper();
	
	ProjectPtr->RenameProjectModel(srcModelName, NewModelName);		

	CString ItemText;
	m_ModelNameList[nItem] = NewModelName;
	if ( MODEL_TYPE_NULL == ModelType )
	{	ItemText = NewModelName + _T(" (**)");	}
	else
	{	ItemText = NewModelName; }
	ListCtrl.SetItemText(nItem, 0, ItemText);
	AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_UPDATE, (LPARAM)this);
	return;
}
//-------------------------------------------------------------------------------------//
void CEditModelListDockPane::OnModelDelete()
{	
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	

	CString    str, str2;	
	CString    ModelName;
	CAOIModel *ModelPtr = NULL;
	MODEL_TYPE ModelType=MODEL_TYPE_NULL;	
	CThisListCtrl_10   &ListCtrl = m_wndModelListCtrl;
	const int nItem = ListCtrl.GetNextItem(-1, LVNI_FOCUSED);
	if ( -1 == nItem ) { return; }
	const size_t ModelNameCount=m_ModelNameList.size();
	const size_t GroupNameCount=m_GroupNameList.size();
	size_t Index = ListCtrl.GetItemData(nItem);
	if ( Index>=ModelNameCount || Index>=GroupNameCount ) { return; }
	ModelName = m_ModelNameList[Index];
	ModelPtr = ProjectPtr->GetProjectModelPtrByModelName(ModelName);
	if ( NULL == ModelPtr )
	{	ModelType=MODEL_TYPE_NULL; }	
	else 
	{	ModelType = ModelPtr->GetModelType(); }
	
	DWORD Res=0;
	str2 = _T("Do you want to delete the components linked the Model");
	str2 = LoadMultiLanguageString(str2, str2);
	str.Format(_T("%s [%s] ?"), str2, ModelName);
	Res = JetAPI::ShowMessageBox(str, MB_YESNOCANCEL); 
	if ( IDCANCEL == Res )
	{	return; }	
	
	if ( NULL != ModelPtr )
	{
		ProjectPtr->SelectProjectAllModels(false);
		ModelPtr->SetModelSelected(true);
		ProjectPtr->DeleteProjectModelSelected();
	}	
	
	if ( IDYES == Res )
	{
		if ( AOIDataCollect.OperateLevelEditFuncDelComponent() == true )
		{
			AOIDataCollect.ReleaseModelUniFrameList();
			ProjectPtr->SelectProjectAllComponents(false);
			ProjectPtr->SelectProjectComponentsByModelName(ModelName, false);
			LogOperCtrl.SaveLogProjectComponentSelectedDelete(ProjectPtr);
			ProjectPtr->DeleteProjectComponentSelected();
		}
	}

	CString ItemText;	
	if ( IDNO == Res )
	{
		ItemText = ModelName + _T(" (**)");
		ListCtrl.SetItemText(nItem, 0, ItemText);
	}
	else
	{
		ListCtrl.DeleteItem(nItem);
		ClearComponentListCtrl(m_wndComponentListCtrl);
	}
	AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_UPDATE, (LPARAM)this);
	return;	
}
//-------------------------------------------------------------------------------------//
void CEditModelListDockPane::OnModelSelectAll()
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	
	CThisListCtrl_10 &ListCtrl = m_wndModelListCtrl;
	const int nItem = ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if ( nItem < 0 ) { return; }
	
	CString      str, str2;
	CString      ModelName;		
	ModelName = m_ModelNameList[nItem];
	ProjectPtr->SelectProjectAllComponents(false);
	ProjectPtr->SelectProjectComponentsByModelName(ModelName, false);

	CAOIComponent *ComponentActPtr = ProjectPtr->GetProjectActiveComponent();
	if ( NULL != ComponentActPtr )
	{
		CString ModelName = ComponentActPtr->GetComponentModelName();
		if ( m_ModelName.CompareNoCase(ModelName) != 0 ) 
		{	ComponentActPtr = NULL; }
	}
	if ( NULL == ComponentActPtr )
	{	
		ComponentActPtr = ProjectPtr->GetProjectComponentPtrBySelected();
		if ( NULL != ComponentActPtr ) 
		{	ProjectPtr->SetProjectActiveComponent(ComponentActPtr);	}
	}

	UpdateComponentListState(m_wndComponentListCtrl);
	m_wndComponentListCtrl.SetFocus();
	AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_PART_SELECTED, NULL);
}
//-------------------------------------------------------------------------------------//
void CEditModelListDockPane::OnModelUpdateToOthers()
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	
	CThisListCtrl_10 &ListCtrl = m_wndModelListCtrl;
	const int nItem = ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if ( nItem < 0 ) { return; }
	const size_t ModelNameCount=m_ModelNameList.size();
	const size_t GroupNameCount=m_GroupNameList.size();
	size_t Index = ListCtrl.GetItemData(nItem);
	if ( Index>=ModelNameCount || Index>=GroupNameCount ) { return; }

	CString      str, str2;
	CString      KeyName;
	CString      GroupName;
	CString      ModelName;			
	CString      strValue;
	CString      strLabel;
	CString      strCaption;
	CAOIModel   *ModelPtr = NULL;
	CInputBoxWnd InputBox;	
	CAOIComponent *ComponentActPtr = ProjectPtr->GetProjectActiveComponent();

	GroupName = m_GroupNameList[Index];
	ModelName = m_ModelNameList[Index];
	ModelPtr = ProjectPtr->GetProjectModelPtrByModelName(ModelName);
	if ( NULL == ModelPtr ) { return; }

	strCaption = _T("Update To Other Models");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strLabel = _T("Key Name");
	strLabel = LoadMultiLanguageString(strLabel, strLabel);
	InputBox.SetParam1(strCaption, strLabel, ModelName);
	if ( InputBox.DoModal() == IDCANCEL ) { return ; }
	KeyName = InputBox.m_DataEdit1;

	ProjectPtr->UpdateProjectModellToOtherModels(ModelPtr, KeyName, false);		
	ProjectPtr->SelectProjectAllComponents(false);
	if ( NULL != ComponentActPtr )
	{	ComponentActPtr->SetComponentSelected(true); }
	ProjectPtr->SetProjectActiveComponent(ComponentActPtr);
	//BuildComponentCtrl(m_wndComponentListCtrl, ModelName, GroupName);	
	BuildModelListCtrl(m_wndModelListCtrl);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_WND_PROPERTY_WND, WPARAM_BUILD_WND_SELECTED, NULL);
}
//-------------------------------------------------------------------------------------//
void CEditModelListDockPane::OnModelShowLibraryWnd()
{
	if (AOIDataCollect.GetIsLockUIWnd() == true)
	{	return;	}
	AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_SHOW_PROJECT_LIBRARY_DOCK_PANE, TRUE);
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditModelListDockPane::OnModelSaveLeadReport()
{
	if (AOIDataCollect.GetIsLockUIWnd() == true)
	{	return;	}
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	

	CString    str, str2;	
	CString    ModelName;
	CAOIModel *ModelPtr = NULL;
	MODEL_TYPE ModelType=MODEL_TYPE_NULL;	
	CThisListCtrl_10   &ListCtrl = m_wndModelListCtrl;
	const int nItem = ListCtrl.GetNextItem(-1, LVNI_FOCUSED);
	if ( -1 == nItem ) { return; }
	const size_t ModelNameCount=m_ModelNameList.size();
	const size_t GroupNameCount=m_GroupNameList.size();
	size_t Index = ListCtrl.GetItemData(nItem);
	if ( Index>=ModelNameCount || Index>=GroupNameCount ) { return; }
	ModelName = m_ModelNameList[Index];
	ModelPtr = ProjectPtr->GetProjectModelPtrByModelName(ModelName);
	if ( NULL == ModelPtr )
	{	return; }		
	
	DWORD Res=0;
	bool  SaveLeadReport=false;
	str2 = _T("Do you want to save lead report");
	str2 = LoadMultiLanguageString(str2, str2);
	str.Format(_T("%s [%s] ?"), str2, ModelName);
	Res = JetAPI::ShowMessageBox(str, MB_YESNOCANCEL); 
	if ( IDCANCEL == Res )
	{	return; }

	if ( IDYES == Res )
	{	SaveLeadReport = true; }
	else
	{	SaveLeadReport = false; }

	bool  ApplyAllModels=false;		
	str2 = _T("Do you want to apply to all Models?");
	str = LoadMultiLanguageString(str2, str2);	
	Res = JetAPI::ShowMessageBox(str, MB_YESNOCANCEL|MB_DEFBUTTON2); 
	if ( IDCANCEL == Res )
	{	return; }
	if ( IDYES == Res )
	{	ApplyAllModels = true; }
	else
	{	ApplyAllModels = false; }

	if ( false == ApplyAllModels )
	{
		CString ModelName = ModelPtr->GetModelName();
		ModelPtr->SetModelSaveLeadReport(SaveLeadReport);			
		ProjectPtr->SelectProjectComponentsByModelName(ModelName, false);
		ProjectPtr->SetProjectComponentSelectedSaveLeadReport(SaveLeadReport);
		ProjectPtr->SelectProjectAllComponents(false);
	}
	else
	{		
		ProjectPtr->SelectProjectAllModels(true);
		ProjectPtr->SelectProjectAllComponents(true);
		ProjectPtr->SetProjectModelSelectedSaveLeadReport(SaveLeadReport);
		ProjectPtr->SetProjectComponentSelectedSaveLeadReport(SaveLeadReport);
		ProjectPtr->SelectProjectAllModels(false);
		ProjectPtr->SelectProjectAllComponents(false);
		ModelPtr->SetModelSelected(true);
	}
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditModelListDockPane::OnModelDefectRecheckARS()
{
	if (AOIDataCollect.GetIsLockUIWnd() == true)
	{	return;	}
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	

	CString    str, str2;	
	CString    ModelName;
	CAOIModel *ModelPtr = NULL;	
	CThisListCtrl_10   &ListCtrl = m_wndModelListCtrl;
	const int nItem = ListCtrl.GetNextItem(-1, LVNI_FOCUSED);
	if ( -1 == nItem ) { return; }
	const size_t ModelNameCount=m_ModelNameList.size();
	const size_t GroupNameCount=m_GroupNameList.size();
	size_t Index = ListCtrl.GetItemData(nItem);
	if ( Index>=ModelNameCount || Index>=GroupNameCount ) { return; }
	ModelName = m_ModelNameList[Index];
	ModelPtr = ProjectPtr->GetProjectModelPtrByModelName(ModelName);
	if ( NULL == ModelPtr )
	{	return; }		
	
	DWORD Res=0;	
	CWndDefectItemWnd Wnd;
	CWndDefectItem WndDefectItemOld = ModelPtr->GetModelDefectItemRecheck_ARS();
	Wnd.SetWndDefectItem(WndDefectItemOld);
	if ( Wnd.DoModal() == IDCANCEL )
	{	return; }
	CWndDefectItem WndDefectItemNew = Wnd.GetWndDefectItem();

	bool  ApplyAllModels=false;
	str2 = _T("Do you want to apply to all Models?");
	str = LoadMultiLanguageString(str2, str2);	
	Res = JetAPI::ShowMessageBox(str, MB_YESNOCANCEL|MB_DEFBUTTON2); 
	if ( IDCANCEL == Res )
	{	return; }
	if ( IDYES == Res )
	{	ApplyAllModels = true; }
	else
	{	ApplyAllModels = false; }
	
	if ( false == ApplyAllModels )
	{
		CString ModelName = ModelPtr->GetModelName();		
		ModelPtr->SetModelDefectItemRecheck_ARS(WndDefectItemNew);		
		LogOperCtrl.SaveLogModelWndDefectItemCompare(ModelPtr, _T("Defect Recheck (ARS)"), WndDefectItemOld, WndDefectItemNew); 
		ProjectPtr->SelectProjectComponentsByModelName(ModelName, false);
		ProjectPtr->SetProjectComponentSelectedDefectItemRecheck_ARS(WndDefectItemNew);
		ProjectPtr->SelectProjectAllComponents(false);
	}
	else
	{	
		ProjectPtr->SelectProjectAllModels(true);		
		ProjectPtr->SelectProjectAllComponents(true);
		ProjectPtr->SetProjectModelSelectedDefectItemRecheck_ARS(WndDefectItemNew);		
		ProjectPtr->SetProjectComponentSelectedDefectItemRecheck_ARS(WndDefectItemNew);
		ProjectPtr->SelectProjectAllModels(false);
		ProjectPtr->SelectProjectAllComponents(false);
		ModelPtr->SetModelSelected(true);
	}		
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditModelListDockPane::OnModelDefectEssential()
{
	if (AOIDataCollect.GetIsLockUIWnd() == true)
	{	return;	}
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	
	if ( AOIDataCollect.UserLogin_Supervisor() == false )
	{	return; }

	CString    str, str2;	
	CString    ModelName;
	CAOIModel *ModelPtr = NULL;	
	CThisListCtrl_10   &ListCtrl = m_wndModelListCtrl;
	const int nItem = ListCtrl.GetNextItem(-1, LVNI_FOCUSED);
	if ( -1 == nItem ) { return; }
	const size_t ModelNameCount=m_ModelNameList.size();
	const size_t GroupNameCount=m_GroupNameList.size();
	size_t Index = ListCtrl.GetItemData(nItem);
	if ( Index>=ModelNameCount || Index>=GroupNameCount ) { return; }
	ModelName = m_ModelNameList[Index];
	ModelPtr = ProjectPtr->GetProjectModelPtrByModelName(ModelName);
	if ( NULL == ModelPtr )
	{	return; }		
	
	DWORD Res=0;	
	CWndDefectItemWnd Wnd;	
	CWndDefectItem WndDefectItemOld = ModelPtr->GetModelDefectItemEssential();
	Wnd.SetWndDefectItem(WndDefectItemOld);
	if ( Wnd.DoModal() == IDCANCEL )
	{	return; }
	CWndDefectItem WndDefectItemNew = Wnd.GetWndDefectItem();

	bool  ApplyAllModels=false;
	str2 = _T("Do you want to apply to all Models?");
	str = LoadMultiLanguageString(str2, str2);	
	Res = JetAPI::ShowMessageBox(str, MB_YESNOCANCEL|MB_DEFBUTTON2); 
	if ( IDCANCEL == Res )
	{	return; }
	if ( IDYES == Res )
	{	ApplyAllModels = true; }
	else
	{	ApplyAllModels = false; }	
	if ( false == ApplyAllModels )
	{
		CString ModelName = ModelPtr->GetModelName();		
		ModelPtr->SetModelDefectItemEssential(WndDefectItemNew);
		LogOperCtrl.SaveLogModelWndDefectItemCompare(ModelPtr, _T("Test Essential"), WndDefectItemOld, WndDefectItemNew); 
		ProjectPtr->SelectProjectComponentsByModelName(ModelName, false);
		ProjectPtr->SetProjectComponentSelectedDefectItemEssential(WndDefectItemNew);
		ProjectPtr->SelectProjectAllComponents(false);
	}
	else
	{	
		ProjectPtr->SelectProjectAllModels(true);		
		ProjectPtr->SelectProjectAllComponents(true);
		ProjectPtr->SetProjectModelSelectedDefectItemEssential(WndDefectItemNew);		
		ProjectPtr->SetProjectComponentSelectedDefectItemEssential(WndDefectItemNew);
		ProjectPtr->SelectProjectAllModels(false);
		ProjectPtr->SelectProjectAllComponents(false);
		ModelPtr->SetModelSelected(true);
	}		
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditModelListDockPane::OnComponentOffset()
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
	//UpdateComponentListState(m_wndComponentListCtrl);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);
}
//-------------------------------------------------------------------------------------//
void CEditModelListDockPane::OnComponentSetPos()
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
void CEditModelListDockPane::OnComponentMirrorPosX()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return ; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	
	const size_t ComponentSelCount = ProjectPtr->GetProjectComponentSelectedCount();
	if ( 0 == ComponentSelCount ) { return; }

	ProjectPtr->MirrorXProjectComponentSelected();
	LogOperCtrl.SaveLogProjectComponentSelectedMirrorX(ProjectPtr);
	UpdateComponentListState(m_wndComponentListCtrl);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditModelListDockPane::OnComponentMirrorPosY()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return ; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	
	const size_t ComponentSelCount = ProjectPtr->GetProjectComponentSelectedCount();
	if ( 0 == ComponentSelCount ) { return; }

	ProjectPtr->MirrorYProjectComponentSelected();
	LogOperCtrl.SaveLogProjectComponentSelectedMirrorY(ProjectPtr);
	UpdateComponentListState(m_wndComponentListCtrl);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);
	return ;
}
//-------------------------------------------------------------------------------------//
bool CEditModelListDockPane::ExecComponentRotation(double Angle)
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return true; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }
	const size_t ComponentSelCount = ProjectPtr->GetProjectComponentSelectedCount();
	if ( 0 == ComponentSelCount ) { return false; }

	ProjectPtr->RotateProjectComponentSelected(Angle);
	LogOperCtrl.SaveLogProjectComponentSelectedRotate(ProjectPtr, Angle);
	UpdateComponentListState(m_wndComponentListCtrl);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditModelListDockPane::OnComponentRotate090()
{
	ExecComponentRotation(90.0);
}
//-------------------------------------------------------------------------------------//
void CEditModelListDockPane::OnComponentRotate180()
{
	ExecComponentRotation(180.0);
}
//-------------------------------------------------------------------------------------//
void CEditModelListDockPane::OnComponentRotate270()
{
	ExecComponentRotation(270.0);
}
//-------------------------------------------------------------------------------------//
void CEditModelListDockPane::OnComponentRotateAny()
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
void CEditModelListDockPane::OnComponentRotateReverse()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return ; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	const size_t ComponentSelCount = ProjectPtr->GetProjectComponentSelectedCount();
	if ( 0 == ComponentSelCount ) { return; }

	ProjectPtr->ReverseProjectComponentSelected();
	UpdateComponentListState(m_wndComponentListCtrl);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditModelListDockPane::OnComponentRename()
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
	UpdateComponentListState(m_wndComponentListCtrl);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditModelListDockPane::OnComponentDelete()
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
	BuildComponentCtrl(m_wndComponentListCtrl, m_ModelName, m_GroupName);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditModelListDockPane::OnComponentSetNozzleName()
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
	UpdateComponentListState(m_wndComponentListCtrl);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditModelListDockPane::OnComponentSetPartNumber()
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
	UpdateComponentListState(m_wndComponentListCtrl);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditModelListDockPane::OnComponentSearch()
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	CThisListCtrl_10 &ListCtrl = m_wndComponentListCtrl;	
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
	return;
}
//-------------------------------------------------------------------------------------//
void CEditModelListDockPane::OnComponentSelectAll()
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	int            i=0;
	unsigned int   ComponentIndex=0;
	CAOIComponent *ComponentPtr = NULL;	
	CThisListCtrl_10 &ListCtrl = m_wndComponentListCtrl;	
	const int ItemCount = ListCtrl.GetItemCount();	
	if ( 0 == ItemCount ) { return; }               
	
	ProjectPtr->SelectProjectAllComponents(false);
	m_StopComponentListBeSelected = TRUE;
	for ( i=0; i<ItemCount; i++ )
	{
		ComponentIndex = (unsigned int )(ListCtrl.GetItemData(i));
		ComponentPtr = ProjectPtr->GetProjectComponentPtr(ComponentIndex, true);
		if ( NULL == ComponentPtr ) { continue; }
		ComponentPtr->SetComponentSelected(true);
		ListCtrl.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
	}
	m_StopComponentListBeSelected = FALSE;

	CAOIComponent *ComponentActPtr = ProjectPtr->GetProjectActiveComponent();
	if ( NULL != ComponentActPtr )
	{
		CString ModelName = ComponentActPtr->GetComponentModelName();
		if ( m_ModelName.CompareNoCase(ModelName) != 0 ) 
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
void CEditModelListDockPane::OnComponentBypass()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	
	if ( AOIDataCollect.OperateLevelEditFuncBypassComponent() == false )	{	return ; }
	ProjectPtr->SwitchProjectComponentBypassed();	
	LogOperCtrl.SaveLogProjectComponentSelectedBypassed(ProjectPtr);
	UpdateComponentListState(m_wndComponentListCtrl);
}
//-------------------------------------------------------------------------------------//
void CEditModelListDockPane::OnComponentXBoardUnit()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	ProjectPtr->SwitchProjectComponentXBoardUnit();	
	LogOperCtrl.SaveLogProjectComponentSelectedXBoardUnit(ProjectPtr);	
	UpdateComponentListState(m_wndComponentListCtrl);
}
//-------------------------------------------------------------------------------------//
void CEditModelListDockPane::OnComponentModelIsolated()
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
	UpdateComponentListState(m_wndComponentListCtrl);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_WND_PROPERTY_WND, WPARAM_BUILD_WND_SELECTED, NULL);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);
}
//-------------------------------------------------------------------------------------//
void CEditModelListDockPane::OnComponentRestoreCadPos()
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
	UpdateComponentListState(m_wndComponentListCtrl);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);
}
//-------------------------------------------------------------------------------------//
void CEditModelListDockPane::OnComponentBypass3D()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	
	if ( AOIDataCollect.OperateLevelEditFuncBypassComponent() == false )	{	return ; }
	ProjectPtr->SwitchProjectComponentBypass3D();
	LogOperCtrl.SaveLogProjectComponentSelectedBypass3D(ProjectPtr);
	UpdateComponentListState(m_wndComponentListCtrl);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);
}
//-------------------------------------------------------------------------------------//
void CEditModelListDockPane::OnComponentMaskBaseSetColorIndex()
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
	}
	*/
}
//-------------------------------------------------------------------------------------//
void CEditModelListDockPane::OnComponentSpaceNoiseFilter()
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
void CEditModelListDockPane::OnComponentMaskExtendSizeBody()
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
void CEditModelListDockPane::OnComponentEnableAlarmAOI()
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
void CEditModelListDockPane::OnComponentGroupID()
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
void CEditModelListDockPane::OnComponentGroupOrg()
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
void CEditModelListDockPane::OnComponentToFieldPos()
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
void CEditModelListDockPane::OnComponentEnableSelfField()
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
void CEditModelListDockPane::OnComponentCloneNewModel()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	
	CAOIComponent *ComponentPtr = ProjectPtr->GetProjectActiveComponent();
	if ( NULL == ComponentPtr )	{	return; }	
	CAOIModel *ModelPtr = ComponentPtr->GetComponentModelPtr();
	if ( NULL == ModelPtr ) { return; }
	MODEL_TYPE ModelType = ModelPtr->GetModelType();
	if ( MODEL_TYPE_NULL == ModelType ) { return; }

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

	ExecModelAdd(ModelPtr_New);
	return;
}
//-------------------------------------------------------------------------------------//
void CEditModelListDockPane::OnComponentChangeBoard()
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
void CEditModelListDockPane::OnComponentLocalBasePlaneID()
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
void CEditModelListDockPane::OnComponentDataModelParam()
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
void CEditModelListDockPane::OnComponentSaveWndList()
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
void CEditModelListDockPane::OnComponentFeedbackResultPos()
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
	UpdateComponentListState(m_wndComponentListCtrl);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);
}
//-------------------------------------------------------------------------------------//
bool CEditModelListDockPane::ExecModelAdd(CAOIModel *ModelPtr)
{
	if ( NULL == ModelPtr ) { return false; }
	const size_t ModelNameCnt=m_ModelNameList.size();
	const size_t GroupNameCnt=m_GroupNameList.size();	
	CString strModelName=ModelPtr->GetModelName();
	CString strGroupName=ModelPtr->GetModelGroupName();	

	CString       str;
	CThisListCtrl_10 &ListCtrl=m_wndModelListCtrl;
	int nItem = ListCtrl.GetItemCount();
	COLORREF UnsetColor = AOIDataCollect.GetColorModelUnset();
	
	m_ModelNameList.push_back(strModelName);
	m_GroupNameList.push_back(strGroupName);

	m_StopModelListBeSelected = TRUE;
	ListCtrl.InsertItem(nItem, strModelName);	
	ListCtrl.SetItemData(nItem, ModelNameCnt);
	if ( ModelPtr->CheckModelTypeEnabled() == false )
	{	ListCtrl.SetItemTextColor(nItem, UnsetColor); }
	str.Format(_T("%d"), nItem+1);
	ListCtrl.SetItemText(nItem, 1, str);	
	m_StopModelListBeSelected = FALSE;	
	ListCtrl.SetItemState(nItem, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);	
	return true;
}
//-------------------------------------------------------------------------------------//