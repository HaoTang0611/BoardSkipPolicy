// EditComponentListDockPane.cpp : 實作檔
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "EditComponentListDockPane.h"
//-------------------------------------------------------------------------------------//
#include "TreeCtrlDef.h"
#include "InputBoxWnd.h"
#include "InputListWnd.h"
#include "SpaceBaseParamWnd.h"
#include "ComponentDefectAlarmWnd.h"
#include "SpaceNoiseFilterParamWnd.h"
//-------------------------------------------------------------------------------------//
#define TREE_ITEM_PATCH_ENABLE_COUNT   5000
//-------------------------------------------------------------------------------------//
const UINT ID_COMPONENT_TREE_CTRL  =  CPageSplitterWnd::GetIdFromRowCol(0, 0);//AFX_IDW_PANE_FIRST;
const UINT ID_INFOMATION_PROP_CTRL = CPageSplitterWnd::GetIdFromRowCol(1, 0);//AFX_IDW_PANE_FIRST+16;//注意行列會不同唷
//-------------------------------------------------------------------------------------//
enum INFO_PROPERTY_ID
{
	INFO_PROPERTY_SCOPE_BEGIN,

	INFO_PROJECT_NODE_BEGIN,	
	INFO_PROJECT_FILENAME,
	INFO_PROJECT_MODULE,
	INFO_PROJECT_VERSION,
	INFO_PROJECT_PANEL_SIDE,
	INFO_PROJECT_TEST_WIDTH,
	INFO_PROJECT_TEST_HEIGHT,
	INFO_PROJECT_PANEL_COUNT,
	INFO_PROJECT_BOARD_COUNT,
	INFO_PROJECT_MARK_COUNT,
	INFO_PROJECT_BARCODE_COUNT,
	INFO_PROJECT_COMPONENT_COUNT,	
	INFO_PROJECT_COMPONENT_BYPASS_COUNT,
	INFO_PROJECT_FIELD_COUNT,	
	INFO_PROJECT_BARCODE,
	INFO_PROJECT_BARCODE_DEVICE_INDEX,
	INFO_PROJECT_BARCODE_DEVICE_CODE_INDEX,	
	INFO_PROJECT_NODE_END,

	INFO_PANEL_NODE_BEGIN,
	INFO_PANEL_PANEL_INDEX,	
	INFO_PANEL_BARCODE,
	INFO_PANEL_BOARD_COUNT,
	INFO_PANEL_COMPONENT_COUNT,
	INFO_PANEL_BYPASS,
	INFO_PANEL_BARCODE_ENABLED,
	INFO_PANEL_BARCODE_DEVICE_INDEX,
	INFO_PANEL_BARCODE_DEVICE_CODE_INDEX,
	INFO_PANEL_BOARD_ROW_COUNT,
	INFO_PANEL_BOARD_COL_COUNT,
	INFO_PANEL_BOARD_COL_BLOCK_COUNT,
	INFO_PANEL_NODE_END,

	INFO_BOARD_NODE_BEGIN,
	INFO_BOARD_PANEL_INDEX,
	INFO_BOARD_BOARD_INDEX,
	INFO_BOARD_BOARD_SIDE_MODE,
	INFO_BOARD_BARCODE,
	INFO_BOARD_COMPONENT_COUNT,
	INFO_BOARD_BYPASS,
	INFO_BOARD_FD_ENABLE,
	INFO_BOARD_BARCODE_ENABLED,
	INFO_BOARD_BARCODE_DEVICE_INDEX,
	INFO_BOARD_BARCODE_DEVICE_CODE_INDEX,
	INFO_BOARD_NODE_END,

	INFO_FD_NODE_BEGIN,	
	INFO_FD_NAME,
	INFO_FD_UNIQUE_ID,
	INFO_FD_CAD_XY,
	INFO_FD_STAGE_XY,
	INFO_FD_TEACH_XY,
	INFO_FD_GROUP_ID,
	INFO_FD_PANEL_INDEX,
	INFO_FD_BOARD_INDEX,	
	INFO_FD_SORT_ID,	
	INFO_FD_LOCAL_BASE_PLANE_ID,
	INFO_FD_NODE_END,	

	INFO_MARK_NODE_BEGIN,	
	INFO_MARK_NAME,
	INFO_MARK_UNIQUE_ID,
	INFO_MARK_GROUP_ID,
	INFO_MARK_BYPASSED,
	INFO_MARK_PANEL_INDEX,
	INFO_MARK_BOARD_INDEX,
	INFO_MARK_LOCAL_BASE_PLANE_ID,
	INFO_MARK_LOCAL_PLANE_NORMAL_Z,
	INFO_MARK_NODE_END,	

	INFO_BARCODE_NODE_BEGIN,	
	INFO_BARCODE_NAME,
	INFO_BARCODE_GROUP_ID,
	INFO_BARCODE_PANEL_INDEX,
	INFO_BARCODE_BOARD_INDEX,
	INFO_BARCODE_SPREAD_MODE,
	INFO_BARCODE_BELONG_MODE,
	INFO_BARCODE_ANGLE,
	INFO_BARCODE_RESULT,
	INFO_BARCODE_SAVE_IMAGE,
	INFO_BARCODE_LOCAL_BASE_PLANE_ID,
	INFO_BARCODE_NODE_END,	

	INFO_COMPONENT_NODE_BEGIN,	
	INFO_COMPONENT_NAME,
	INFO_COMPONENT_CAD_XY,
	INFO_COMPONENT_STAGE_XY,
	INFO_COMPONENT_ANGLE,
	INFO_COMPONENT_PART_NUMBER,
	INFO_COMPONENT_MODEL_NAME,
	INFO_COMPONENT_COL_INDEX,
	INFO_COMPONENT_ROW_INDEX,
	INFO_COMPONENT_BYPASS,
	INFO_COMPONENT_BYPASS3D,	
	INFO_COMPONENT_SAVE_IMAGE,
	INFO_COMPONENT_BAD_MARK,
	INFO_COMPONENT_OFFSET_XY,
	INFO_COMPONENT_SKEW_ANGLE,
	INFO_COMPONENT_MODEL_CLASS_ID,
	INFO_COMPONENT_MODEL_ISOLATED,
	INFO_COMPONENT_SELF_FIELD,
	INFO_COMPONENT_LOCAL_BASE_PLANE_ID,
	INFO_COMPONENT_SAVE_REPORT_ARS,
	INFO_COMPONENT_M2M_SCO_USE,
	INFO_COMPONENT_M2M_SAVE_IMAGE,
	INFO_COMPONENT_NODE_END,	

	INFO_PROPERTY_SCOPE_END
};
//-------------------------------------------------------------------------------------//
// CEditComponentListDockPane
//-------------------------------------------------------------------------------------//
IMPLEMENT_DYNAMIC(CEditComponentListDockPane, CDockablePane)
//-------------------------------------------------------------------------------------//
CEditComponentListDockPane::CEditComponentListDockPane()
{
	m_ProjectPtr = NULL;
	m_DistrictID = DISTRICT_ID_A;
	m_ClickComponentTreeNode = FALSE;
	m_StopComponentTreeBeClick = FALSE;	
	m_EditComponentListType = EDIT_COMPONENT_LIST_NORMAL;

	m_clrFd = CLR_DEFAULT;//CLR_DEFAULT
	m_clrPanel = CLR_DEFAULT;
	m_clrBoard = CLR_DEFAULT;
	m_clrComponent = CLR_DEFAULT;
	m_MoveToComponent = true;
}
//-------------------------------------------------------------------------------------//
CEditComponentListDockPane::~CEditComponentListDockPane()
{
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CEditComponentListDockPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_CONTEXTMENU()
	ON_NOTIFY(NM_CLICK, ID_COMPONENT_TREE_CTRL, OnClickComponentTreeCtrl)
	ON_NOTIFY(NM_RCLICK, ID_COMPONENT_TREE_CTRL, OnRClickComponentTreeCtrl)	
	ON_NOTIFY(NM_DBLCLK, ID_COMPONENT_TREE_CTRL, OnDbclickComponentTreeCtrl)
	ON_NOTIFY(TVN_SELCHANGED, ID_COMPONENT_TREE_CTRL, OnSelchangedComponentTreeCtrl)	
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED,OnPropertyChanged)	
	ON_COMMAND(MENU_TREE_PROJECT_SORT_COMPONENT, OnProjectSortComponent)	
	ON_COMMAND(MENU_TREE_PROJECT_ROTATE_090, OnProjectRotate090)	
	ON_COMMAND(MENU_TREE_PROJECT_ROTATE_180, OnProjectRotate180)	
	ON_COMMAND(MENU_TREE_PROJECT_ROTATE_270, OnProjectRotate270)	
	ON_COMMAND(MENU_TREE_PANEL_OFFSET, OnPanelOffset)
	ON_COMMAND(MENU_TREE_PANEL_MIRROR_POS_X, OnPanelMirrorPosX)
	ON_COMMAND(MENU_TREE_PANEL_MIRROR_POS_Y, OnPanelMirrorPosY)
	ON_COMMAND(MENU_TREE_PANEL_ROTATE_090, OnPanelRotate090)
	ON_COMMAND(MENU_TREE_PANEL_ROTATE_180, OnPanelRotate180)
	ON_COMMAND(MENU_TREE_PANEL_ROTATE_270, OnPanelRotate270)
	ON_COMMAND(MENU_TREE_PANEL_DELETE, OnPanelDelete)	
	ON_COMMAND(MENU_TREE_PANEL_BYPASS, OnPanelBypass)
	ON_COMMAND(MENU_TREE_BOARD_OFFSET, OnBoardOffset)
	ON_COMMAND(MENU_TREE_BOARD_MIRROR_POS_X, OnBoardMirrorPosX)
	ON_COMMAND(MENU_TREE_BOARD_MIRROR_POS_Y, OnBoardMirrorPosY)
	ON_COMMAND(MENU_TREE_BOARD_ROTATE_090, OnBoardRotate090)
	ON_COMMAND(MENU_TREE_BOARD_ROTATE_180, OnBoardRotate180)
	ON_COMMAND(MENU_TREE_BOARD_ROTATE_270, OnBoardRotate270)	
	ON_COMMAND(MENU_TREE_BOARD_DELETE, OnBoardDelete)
	ON_COMMAND(MENU_TREE_BOARD_BYPASS, OnBoardBypass)	
	ON_COMMAND(MENU_TREE_BOARD_CHANGE_PANEL, OnBoardChangePanel)	
	ON_COMMAND(MENU_TREE_BOARD_SEARCH_COMPONENT, OnBoardSearchComponent)
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
	ON_COMMAND(MENU_TREE_FD_DELETE, OnFdDelete)
	ON_COMMAND(MENU_TREE_FD_TO_TEACH_POS, OnFdToTeachPos)
	ON_COMMAND(MENU_TREE_FD_SET_TEACH_POS, OnFdSetTeachPos)
	ON_COMMAND(MENU_TREE_FD_TO_CURRENT_POS, OnFdToCurrentPos)		
	ON_COMMAND(MENU_TREE_FD_UPDATE_TO_OTHERS, OnFdUpdateToOthers)
	ON_COMMAND(MENU_TREE_BARCODE_DELETE, OnBarcodeDelete)	
	ON_COMMAND(MENU_TREE_BARCODE_ROTATE_ANY, OnBarcodeRotateAny)
	ON_COMMAND(MENU_TREE_BARCODE_UPDATE_TO_OTHERS, OnBarcodeUpdateToOthers)	
	ON_COMMAND(MENU_TREE_MARK_DELETE, OnMarkDelete)
	ON_COMMAND(MENU_TREE_MARK_BYPASS, OnMarkBypass)	
	ON_COMMAND(MENU_TREE_MARK_SPACE_NOISE_FILTER, OnMarkSpaceNoiseFilter)
	ON_COMMAND(MENU_TREE_MARK_SPACE_BASE_PLANE, OnMarkSpaceBasePlane)	
	ON_COMMAND(MENU_TREE_MARK_PASTE_TO_OTHER_BOARD, OnMarkPasteToOtherBoard)
	ON_COMMAND(MENU_TREE_MARK_LOCAL_BASE_PLANE_ID, OnMarkLocalBasePlaneID)	
	ON_COMMAND(MENU_TREE_MARK_UPDATE_TO_OTHERS, OnMarkUpdateToOthers)		
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
// CEditComponentListDockPane 訊息處理常式
//-------------------------------------------------------------------------------------//
int CEditComponentListDockPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此加入特別建立的程式碼
	DWORD dwPropStyle = WS_CHILD|WS_VISIBLE;

	m_wndSplitter.CreateStatic(this,2,1);

	DWORD dwListStyle = WS_CHILD | WS_VISIBLE | LVS_REPORT  | LVS_SHAREIMAGELISTS | LVS_SHOWSELALWAYS;
	DWORD dwTreeStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS | TVS_FULLROWSELECT;//TVS_EDITLABELS 
	
	if(!m_wndSplitter.AddWindow(0,0,&m_wndComponentTreeCtrl,WC_TREEVIEW,dwTreeStyle,0,CSize(100,600), ID_COMPONENT_TREE_CTRL))
	{
		TRACE0("warning, can not create component tree ctrl\n");
		return -1;
	}
	
	if ( !m_wndSplitter.AddWindow(1, 0, &m_wndInfomationPropCtrl,WC_LISTVIEW,dwPropStyle,0,CSize(200,200), ID_INFOMATION_PROP_CTRL))
	{
		TRACE0("無法建立 [框列表] 方格\n");
		return -1;
	}	
	
	m_wndComponentTreeCtrl.SetOwner(this);
	m_wndInfomationPropCtrl.SetOwner(this);
	
	JetAPI::InitialTreeCtrl(m_wndComponentTreeCtrl);
	InitTreeCtrl(m_wndComponentTreeCtrl);	
	InitPropList();
	BuildTreeImageList(m_ComponentTreeImageList);
	m_wndComponentTreeCtrl.SetImageList(&m_ComponentTreeImageList, TVSIL_NORMAL);
	return 0;
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	// TODO: 在此加入您的訊息處理常式程式碼
	if ( CWnd::GetSafeHwnd() == NULL ) { return ; }
	if ( m_wndSplitter.GetSafeHwnd() == NULL ) { return; }
	CRect rect;
	const int cyTlb = 0;
	rect.left = 0; 
	rect.top = 0;
	rect.right = cx;
	rect.bottom = cy;
	//m_wndSplitter.SetRowInfo(0,(cy-cyTlb)/2,25);
	m_wndSplitter.SetWindowPos(NULL,rect.left, rect.top + cyTlb, rect.Width()  , rect.Height()-cyTlb, SWP_NOZORDER | SWP_NOACTIVATE);
}
//-------------------------------------------------------------------------------------//
BOOL CEditComponentListDockPane::BuildTreeImageList(CImageList &ImageList)
{	
	SIZE ImageSize;
	ImageSize.cx = 16;
	ImageSize.cy = 16;
	ImageList.DeleteImageList();
	if ( ImageList.Create(ImageSize.cx, ImageSize.cy, ILC_COLOR24|ILC_MASK, 0, 10) == false ) 
	{
		TRACE0("Failed to create Object Toolbar Image List\n");
		return false;      // fail to create
	}

	CBitmap bm;
	//bm.LoadBitmap(IDB_PROJECT_TREE_ICON);					ImageList.Add(&bm, RGB(212,208,200));	bm.DeleteObject();//00
	bm.LoadBitmap(IDB_PROJECT_TREE_ICON);					ImageList.Add(&bm, RGB(255,255,255));	bm.DeleteObject();//00
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::SwitchMultiLanguage()
{
	/*
	size_t  i = 0;	
	UINT    WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_EDIT_COMPONENT_LIST_DOCK_PANE");	
	//---------------------------------------------------------------------------------//
	WndID = IDD_EDIT_COMPONENT_LIST_DOCK_PANE;
	WndKey = _T("IDD_EDIT_COMPONENT_LIST_DOCK_PANE");
	this->GetWindowText(LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetWindowText(NewLabelText);
	//---------------------------------------------------------------------------------//
	*/
}
//-------------------------------------------------------------------------------------//
CString CEditComponentListDockPane::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_EDIT_COMPONENT_LIST_DOCK_PANE");		
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
BOOL CEditComponentListDockPane::InitPropList()
{	
	CString str1, str2, str3;	
	CString strTrue, strFalse;
	strTrue = _T("True");
	strTrue = LoadMultiLanguageString(strTrue, strTrue);
	strFalse = _T("False");
	strFalse = LoadMultiLanguageString(strFalse, strFalse);	

	str1 = _T("Name");
	str1 = LoadMultiLanguageString(str1, str1);
	str2 = _T("Value");
	str2 = LoadMultiLanguageString(str2, str2);
	//m_wndInfomationPropCtrl.EnableHeaderCtrl(TRUE, _T("Wnd"), _T("Result"));
	m_wndInfomationPropCtrl.EnableHeaderCtrl(TRUE, str1, str2);
	m_wndInfomationPropCtrl.EnableDescriptionArea(FALSE);
	m_wndInfomationPropCtrl.SetVSDotNetLook();	
	//m_wndInfomationPropCtrl.MarkModifiedProperties();	
	//m_wndInfomationPropCtrl.SetGroupNameFullWidth(bSet);

	m_wndInfomationPropCtrl.SetBoolLabels(strTrue, strFalse);
	return TRUE;
}
//-------------------------------------------------------------------------------------//
LRESULT CEditComponentListDockPane::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此加入特定的程式碼和 (或) 呼叫基底類別
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
			{	CloseProject(); }
			else
			{	BuildComponentTreeCtrl(m_wndComponentTreeCtrl); }
			break;
		case WPARAM_PROJECT_UPDATE:
			if ( CWnd::IsWindowVisible() == TRUE )
			{
				pWnd = (CWnd*)(lParam);
				if ( this != pWnd )
				{	UpdateComponentTreeCtrl(m_wndComponentTreeCtrl, TREE_CTRL_UPDATE_ALL); }
			}
			break;		
		case WPARAM_PROJECT_CLOSE:
			CloseProject();			
			break;
		case WPARAM_PROJECT_PART_SELECTED:
			ShowComponentTreeSelected(m_wndComponentTreeCtrl);
			break;
		case WPARAM_PROJECT_PART_DELETED:
			RemoveComponentTreeItem(m_wndComponentTreeCtrl);
			break;
		}
		break;
	case MSG_EDIT_PART_LIST_WND:
		switch ( wParam )
		{
		case WPARAM_BUILD_PART_LIST:
			Res = lParam&LPARAM_BUILD_DOCK_LIST_PART;
			if ( 0 != Res )
			{
				BuildComponentTreeCtrl(m_wndComponentTreeCtrl);
				AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_WND_PROPERTY_WND, WPARAM_BUILD_WND_SELECTED, NULL);
			}
			break;
		case WPARAM_UPDATE_PART_LIST:
			UpdateComponentTreeCtrl(m_wndComponentTreeCtrl, lParam);
			AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_WND_PROPERTY_WND, WPARAM_BUILD_WND_SELECTED, NULL);
			break;	
		case WPARAM_CLEAR_PART_LIST:		
			ClearComponentTreeCtrl(m_wndComponentTreeCtrl, m_StopComponentTreeBeClick);
			AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_WND_PROPERTY_WND, WPARAM_BUILD_WND_SELECTED, NULL);
			break;
		case WPARAM_UPDATE_PART_SELECTED:
			ShowComponentTreeSelected(m_wndComponentTreeCtrl);
			AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_WND_PROPERTY_WND, WPARAM_BUILD_WND_SELECTED, NULL);
			break;
		case WPARAM_UPDATE_PART_STATES:
			UpdateComponentTreeCtrlState(m_wndComponentTreeCtrl, TREE_CTRL_UPDATE_STATE);
			break;
		}
		break;
	case MSG_TREE_CTRL:
		switch ( wParam )
		{
		case WPARAM_TREE_VERTICAL_SCROLL_END:
			if ( (int)(lParam) > 0 )
			{	BuildNextComponentTreeCtrl(m_wndComponentTreeCtrl);	}
			break;
		}
		break;
	}	
	return CDockablePane::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
bool CEditComponentListDockPane::GetLockUIWnd()
{
	if ( true == AOIDataCollect.GetIsLockUIWnd() )
	{	return true; }
	return false;
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::CloseProject()
{
	m_ProjectPtr = NULL;
	ClearInfoPropCtrl();
	ClearComponentTreeCtrl(m_wndComponentTreeCtrl, m_StopComponentTreeBeClick);	
}
//-------------------------------------------------------------------------------------//
inline CAOIProject* CEditComponentListDockPane::GetActiveProject()
{
	return m_ProjectPtr;
}
//-------------------------------------------------------------------------------------//
inline DISTRICT_ID CEditComponentListDockPane::GetActiveDistrictID()
{
	return m_DistrictID;
}
//-------------------------------------------------------------------------------------//
inline bool CEditComponentListDockPane::CheckShowMark() const
{
	EDIT_COMPONENT_LIST_TYPE EditComponentListType = GetEditComponentListType();
	if ( EDIT_COMPONENT_LIST_MARK == EditComponentListType ) { return true; }
	return AOIDataCollect.GetShowMarkList();	
	bool Show = true;
	UINT ViewID = AOIDataCollect.GetMainViewWndID();
	switch ( ViewID )
	{
	case IDD_EDIT_FD_VIEW:	Show = false;	break;
	case IDD_EDIT_MARK_VIEW:	Show = true; break;
	case IDD_EDIT_BARCODE_VIEW:	Show = false; break;
	default:	Show = true;	break;
	}
	return Show;
}
//-------------------------------------------------------------------------------------//
inline bool CEditComponentListDockPane::CheckShowFiducial() const
{
	EDIT_COMPONENT_LIST_TYPE EditComponentListType = GetEditComponentListType();
	if ( EDIT_COMPONENT_LIST_MARK == EditComponentListType ) { return false; }
	return AOIDataCollect.GetShowFdList();
	bool Show = true;
	UINT ViewID = AOIDataCollect.GetMainViewWndID();
	switch ( ViewID )
	{
	case IDD_EDIT_FD_VIEW:	Show = true;	break;
	case IDD_EDIT_MARK_VIEW:	Show = false; break;
	case IDD_EDIT_BARCODE_VIEW:	Show = false; break;
	default:	Show = true;	break;
	}
	return Show;
}
//-------------------------------------------------------------------------------------//
inline bool CEditComponentListDockPane::CheckShowBarcode() const
{
	EDIT_COMPONENT_LIST_TYPE EditComponentListType = GetEditComponentListType();
	if ( EDIT_COMPONENT_LIST_MARK == EditComponentListType ) { return false; }
	return AOIDataCollect.GetShowBarcodeList();
	bool Show = true;
	UINT ViewID = AOIDataCollect.GetMainViewWndID();
	switch ( ViewID )
	{
	case IDD_EDIT_FD_VIEW:	Show = false;	break;
	case IDD_EDIT_MARK_VIEW:	Show = false; break;
	case IDD_EDIT_BARCODE_VIEW:	Show = true; break;
	default:	Show = true;	break;
	}
	return Show;
}
//-------------------------------------------------------------------------------------//
inline bool CEditComponentListDockPane::CheckShowComponent() const
{
	EDIT_COMPONENT_LIST_TYPE EditComponentListType = GetEditComponentListType();
	if ( EDIT_COMPONENT_LIST_MARK == EditComponentListType ) { return false; }
	return AOIDataCollect.GetShowComponentList();
	bool Show = true;
	UINT ViewID = AOIDataCollect.GetMainViewWndID();
	switch ( ViewID )
	{
	case IDD_EDIT_FD_VIEW:	Show = false;	break;
	case IDD_EDIT_MARK_VIEW:	Show = false; break;
	case IDD_EDIT_BARCODE_VIEW:	Show = false; break;
	default:	Show = true;	break;
	}
	return Show;
}
//-------------------------------------------------------------------------------------//
BOOL CEditComponentListDockPane::InitTreeCtrl(CThisTreeCtrl &TreeCtrl)
{
	//return TRUE;
	HTREEITEM hRoot = TreeCtrl.InsertItem(_T("FakeApp 類別"), 0, 0);
	TreeCtrl.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);

	HTREEITEM hClass = TreeCtrl.InsertItem(_T("CFakeAboutDlg"), 1, 1, hRoot);
	TreeCtrl.InsertItem(_T("CFakeAboutDlg()"), 3, 3, hClass);

	TreeCtrl.Expand(hRoot, TVE_EXPAND);

	hClass = TreeCtrl.InsertItem(_T("CFakeApp"), 1, 1, hRoot);
	TreeCtrl.InsertItem(_T("CFakeApp()"), 3, 3, hClass);
	TreeCtrl.InsertItem(_T("InitInstance()"), 3, 3, hClass);
	TreeCtrl.InsertItem(_T("OnAppAbout()"), 3, 3, hClass);

	hClass = TreeCtrl.InsertItem(_T("CFakeAppDoc"), 1, 1, hRoot);
	TreeCtrl.InsertItem(_T("CFakeAppDoc()"), 4, 4, hClass);
	TreeCtrl.InsertItem(_T("~CFakeAppDoc()"), 3, 3, hClass);
	TreeCtrl.InsertItem(_T("OnNewDocument()"), 3, 3, hClass);

	hClass = TreeCtrl.InsertItem(_T("CFakeAppView"), 1, 1, hRoot);
	TreeCtrl.InsertItem(_T("CFakeAppView()"), 4, 4, hClass);
	TreeCtrl.InsertItem(_T("~CFakeAppView()"), 3, 3, hClass);
	TreeCtrl.InsertItem(_T("GetDocument()"), 3, 3, hClass);
	TreeCtrl.Expand(hClass, TVE_EXPAND);

	hClass = TreeCtrl.InsertItem(_T("CFakeAppFrame"), 1, 1, hRoot);
	TreeCtrl.InsertItem(_T("CFakeAppFrame()"), 3, 3, hClass);
	TreeCtrl.InsertItem(_T("~CFakeAppFrame()"), 3, 3, hClass);
	TreeCtrl.InsertItem(_T("m_wndMenuBar"), 6, 6, hClass);
	TreeCtrl.InsertItem(_T("m_wndToolBar"), 6, 6, hClass);
	TreeCtrl.InsertItem(_T("m_wndStatusBar"), 6, 6, hClass);

	hClass = TreeCtrl.InsertItem(_T("Globals"), 2, 2, hRoot);
	TreeCtrl.InsertItem(_T("theFakeApp"), 5, 5, hClass);
	TreeCtrl.Expand(hClass, TVE_EXPAND);
	return TRUE;
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDockablePane::OnShowWindow(bShow, nStatus);

	// TODO: 在此加入您的訊息處理常式程式碼
	bool Show=true;	
	if ( TRUE == bShow )
	{	
		Show=true;	
		//m_wndComponentTreeCtrl.SetFocus();
		BuildComponentTreeCtrl(m_wndComponentTreeCtrl);
		ShowComponentTreeSelected(m_wndComponentTreeCtrl);
	}
	else
	{	
		Show=false;	
		CloseProject();
	}
	EDIT_COMPONENT_LIST_TYPE EditComponentListType = GetEditComponentListType();
	switch ( EditComponentListType )
	{
	case EDIT_COMPONENT_LIST_MARK:
		AOIDataCollect.SetShowUIWndMarkList(Show);
		break;
	default:
	case EDIT_COMPONENT_LIST_NORMAL:
		AOIDataCollect.SetShowUIWndComponentList(Show);
		break;
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnContextMenu(CWnd* pWnd, CPoint point)
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
				
		if ( ID_COMPONENT_TREE_CTRL == CtrlID )
		{	ExecTreeCtrlComponentMenu(point);	}
		else if ( ID_INFOMATION_PROP_CTRL == CtrlID )
		{
		//	::AfxMessageBox(_T("CEditComponentListDockPane::OnContextMenu-List"));
		}
		else
		{
		//	::AfxMessageBox(_T("CEditComponentListDockPane::OnContextMenu"));
		
		}		
	}	
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::SetEditComponentListType(EDIT_COMPONENT_LIST_TYPE val)
{
	m_EditComponentListType = val;
}
//-------------------------------------------------------------------------------------//
EDIT_COMPONENT_LIST_TYPE CEditComponentListDockPane::GetEditComponentListType() const
{
	return m_EditComponentListType;
}
//-------------------------------------------------------------------------------------//
bool CEditComponentListDockPane::ClearComponentTreeCtrl(CThisTreeCtrl &TreeCtrl, BOOL &StopTreeBeClick)
{
	m_PanelTreeNodeList.clear();
	m_PanelTreeNodeListDone = false;

	StopTreeBeClick = FALSE;
	m_TreeItemSelected = NULL;
	if ( TreeCtrl.GetSafeHwnd() == NULL ) { return true; }
	m_ProjectPtr = NULL;
	StopTreeBeClick = TRUE; 	
	if ( TreeCtrl.DeleteAllItems() == FALSE )
	{ 
		StopTreeBeClick = FALSE;	
		return false; 
	}
	StopTreeBeClick = FALSE;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditComponentListDockPane::BuildComponentTreeCtrl(CThisTreeCtrl &TreeCtrl)
{
	m_ProjectPtr = NULL;	
	if ( TreeCtrl.GetSafeHwnd() == NULL ) { return false; }	
	ClearComponentTreeCtrl(TreeCtrl, m_StopComponentTreeBeClick);
	CAOIProject *Project = AOIDataCollect.GetActiveTaskProject();	
	if ( NULL == Project ) { return true; }
	m_ProjectPtr = Project;
	m_DistrictID = Project->GetProjectActDistrictID();

	LPARAM aa = MakeItemlParam(254, 16777215);//WORD-246bit, 0 ~ 16777216
	unsigned int ItemType=0, ItemIndex=0;
	DecodeItemlParam(aa, ItemType, ItemIndex);
	//LPARAM  MakeItemlParam(WORD ItemType, WORD ItemIndex)//WORD-16bit, 0 ~ 65535
	//void DecodeItemlParam(LPARAM lParam, size_t &ItemType, size_t &ItemIndex)

	//Project
	// +Panel
	//  +FD	
	//  +Barcode
	//  +Board
	//   +FD
	//   +Barcode
	//   +Component
	//    +Window
	CAOIPanel *pPanel = NULL;
	CAOIBarcode *BarcodePtr = NULL;

	size_t i=0;
	const size_t TextLen = 256;
	TCHAR text[TextLen]=_T("");
	TCHAR text2[TextLen]=_T("");
	const DISTRICT_ID DistrictID = GetActiveDistrictID();

	TVINSERTSTRUCT tvInsert;
	tvInsert.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM;
	HTREEITEM TreeItem1=NULL, TreeItem2=NULL, TreeItem3=NULL;			
	
	CString    str;
	CString    Name;	

	TreeCtrl.SetRedraw(FALSE);
	m_StopComponentTreeBeClick = TRUE;
	//---------------------------------------------------------------------------//	
	//專案參數	
	Name = _T("File");
	Name = LoadMultiLanguageString(Name, Name);
	str = Project->GetProjectFileMainName();
	::_stprintf(text, _T("%s: %s"), Name, str);
	tvInsert.hParent = TVI_ROOT;
	tvInsert.item.pszText = text;
	tvInsert.item.iImage = TREE_IMAGE_LIST_PROGRAM;
	tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_PROGRAM;
	tvInsert.item.lParam  = ::MakeItemlParam(TREE_ITEM_TYPE_PROJECT_FILENAME, 0);
	TreeItem1 = TreeCtrl.InsertItem(&tvInsert);

	Name = _T("Module");
	Name = LoadMultiLanguageString(Name, Name);
	str = Project->GetProjectModuleName();
	::_stprintf(text, _T("%s: %s"), Name, str);	
	tvInsert.hParent = TVI_ROOT;
	tvInsert.item.pszText = text;	
	tvInsert.item.iImage = TREE_IMAGE_LIST_PROGRAM;
	tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_PROGRAM;
	tvInsert.item.lParam  = MakeItemlParam(TREE_ITEM_TYPE_PROJECT_MODULE, 0);
	TreeItem1 = TreeCtrl.InsertItem(&tvInsert);
	InsertProjectNodeToTreeCtrl(TreeCtrl, TreeItem1);
	//---------------------------------------------------------------------------//
	const size_t BarcodeCount = Project->GetProjectBarcodeCount();
	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = Project->GetProjectBarcodePtr(i, false);
		if ( NULL == BarcodePtr ) { continue; }
		if ( NULL != BarcodePtr->GetBarcodePanelPtr() ) { continue; }
		if ( NULL != BarcodePtr->GetBarcodeBoardPtr() ) { continue; }

		Name = AOIDataDefine.GetBarcodeText();		
		::_stprintf(text, _T("%s %d"), Name, i+1);
		tvInsert.hParent = NULL;
		tvInsert.item.pszText = text;
		tvInsert.item.iImage = TREE_IMAGE_LIST_BARCODE;
		tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_BARCODE_SELECTED;
		tvInsert.item.lParam  = ::MakeItemlParam(TREE_ITEM_TYPE_BARCODE, i);//Fiducial 引數
		TreeItem1 = TreeCtrl.InsertItem(&tvInsert);
		TreeCtrl.SetItemTextColor(TreeItem1, m_clrComponent);		
	}
	//---------------------------------------------------------------------------//
	const size_t NPanels = Project->GetProjectPanelCount();
	for ( i=0; i<NPanels; i++ )
	{
		pPanel = Project->GetProjectPanelPtr(i, false);
		if ( NULL == pPanel ) { continue; }
		if ( pPanel->GetPanelDeleted() == true ) { continue; }		
		
		//Panel的基本節點
		Name = _T("Panel");		
		Name = AOIDataDefine.GetPanelText();
		::_stprintf(text, _T("%s %d"), Name, i+1);
		tvInsert.hParent = TVI_ROOT;
		tvInsert.item.pszText = text;
		if ( pPanel->GetPanelBypassed() == false )
		{
			tvInsert.item.iImage = TREE_IMAGE_LIST_PANEL;
			tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_PANEL_SELECTED;
		}
		else
		{
			tvInsert.item.iImage = TREE_IMAGE_LIST_PANEL_BYPASS;
			tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_PANEL_BYPASS_SELECTED;
		}
		tvInsert.item.lParam  = MakeItemlParam(TREE_ITEM_TYPE_PANEL, i);
		TreeItem1 = TreeCtrl.InsertItem(&tvInsert);
		//TreeCtrl.SetBkColor(0x00FFFF);
		TreeCtrl.SetItemTextColor(TreeItem1, m_clrPanel);

		CAOITreeNode PanelTreeNode(TreeItem1, pPanel);
		if ( InsertPanelNodeToTreeCtrl(TreeCtrl, TreeItem1, pPanel, &PanelTreeNode) == false )
		{
			m_StopComponentTreeBeClick = FALSE;
			TreeCtrl.SetRedraw(TRUE);
			TreeCtrl.Invalidate();
			TreeCtrl.UpdateWindow();
			ClearComponentTreeCtrl(TreeCtrl, m_StopComponentTreeBeClick);
			return false;
		}
		m_PanelTreeNodeList.push_back(PanelTreeNode);
	}

	bool TreeNodeListDone = true;
	for ( i=0; i<m_PanelTreeNodeList.size(); i++ )
	{
		CAOITreeNode &PanelTreeNode=m_PanelTreeNodeList[i];
		const size_t BoardTreeNodeCount=PanelTreeNode.GetChildNodeCount();
		for ( size_t j=0; j<BoardTreeNodeCount; j++ )
		{
			CAOITreeNode *BoardTreeNodePtr=PanelTreeNode.GetChildNodePtr(j, false);
			if ( NULL == BoardTreeNodePtr ) { continue; }
			if ( BoardTreeNodePtr->GetChildNodeListDone() == false )
			{	
				TreeNodeListDone = false;
				break; 
			}
		}
		if ( false == TreeNodeListDone )
		{	break; }
	}
	m_PanelTreeNodeListDone = TreeNodeListDone;	

	m_StopComponentTreeBeClick = FALSE;
	TreeCtrl.SetRedraw(TRUE);
	TreeCtrl.Invalidate();
	TreeCtrl.UpdateWindow();	
	m_MoveToComponent = true;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditComponentListDockPane::InsertProjectNodeToTreeCtrl(CThisTreeCtrl &TreeCtrl, HTREEITEM hProjectItem)//增加專案資訊
{	
	return true;
	if ( NULL==hProjectItem || NULL==this->m_ProjectPtr ) { return false; }
	CString str, Name;
	size_t i=0, j=0, k=0, s=0, t=0;
	const size_t TextLen = 256;
	TCHAR text[TextLen]=_T("");
	TCHAR text2[TextLen]=_T("");

	TVINSERTSTRUCT tvInsert;
	tvInsert.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM;
	HTREEITEM TreeItem1=NULL;	

	tvInsert.hParent = hProjectItem;
	Name = _T("File");
	Name = LoadMultiLanguageString(Name, Name);
	str = _T("FileName");	
	::_stprintf(text, _T("%s: %s"), Name, str);
	tvInsert.item.pszText = text;
	tvInsert.item.iImage = TREE_IMAGE_LIST_OTHERS;
	tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_OTHERS;
	tvInsert.item.lParam  = ::MakeItemlParam(TREE_ITEM_TYPE_PROJECT_FILENAME, 0);
	TreeItem1 = TreeCtrl.InsertItem(&tvInsert);

	Name = _T("Width");
	Name = LoadMultiLanguageString(Name, Name);
	::_stprintf(text, _T("%s= %.0f um"), Name, 100.0);
	tvInsert.hParent = hProjectItem;
	tvInsert.item.pszText = text;
	tvInsert.item.iImage = TREE_IMAGE_LIST_OTHERS;
	tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_OTHERS;
	tvInsert.item.lParam  = ::MakeItemlParam(TREE_ITEM_TYPE_PROJECT_REGION_W, 0);
	TreeItem1 = TreeCtrl.InsertItem(&tvInsert);

	Name = _T("Height");
	Name = LoadMultiLanguageString(Name, Name);
	::_stprintf(text, _T("%s= %.0f um"), Name, 100.0);
	tvInsert.hParent = hProjectItem;
	tvInsert.item.pszText = text;
	tvInsert.item.iImage = TREE_IMAGE_LIST_OTHERS;
	tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_OTHERS;
	tvInsert.item.lParam  = ::MakeItemlParam(TREE_ITEM_TYPE_PROJECT_REGION_H, 0);
	TreeItem1 = TreeCtrl.InsertItem(&tvInsert);

	Name = _T("Fd Range X");
	Name = LoadMultiLanguageString(Name, Name);
	::_stprintf(text, _T("%s= %.2f mm"), Name, 0.0);
	tvInsert.hParent = hProjectItem;
	tvInsert.item.pszText = text;
	tvInsert.item.iImage = TREE_IMAGE_LIST_OTHERS;
	tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_OTHERS;
	tvInsert.item.lParam  = ::MakeItemlParam(TREE_ITEM_TYPE_PROJECT_FD_RANGE_W, 0);
	TreeItem1 = TreeCtrl.InsertItem(&tvInsert);

	Name = _T("Fd Range Y");
	Name = LoadMultiLanguageString(Name, Name);
	::_stprintf(text, _T("%s= %.2f mm"), Name, 0.0);
	tvInsert.hParent = hProjectItem;
	tvInsert.item.pszText = text;
	tvInsert.item.iImage = TREE_IMAGE_LIST_OTHERS;
	tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_OTHERS;
	tvInsert.item.lParam  = ::MakeItemlParam(TREE_ITEM_TYPE_PROJECT_FD_RANGE_H, 0);
	TreeItem1 = TreeCtrl.InsertItem(&tvInsert);

	CString BarcodeName;
//	BarcodeName = Project->GetBarcodeStats(Project->m_ProjectParameter.m_BarcodeStates);
	::_stprintf(text, _T("Barcode Mode: %s"), BarcodeName);	

	tvInsert.hParent = hProjectItem;
	tvInsert.item.pszText = text;
	tvInsert.item.iImage = TREE_IMAGE_LIST_OTHERS;
	tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_OTHERS;
	tvInsert.item.lParam  = ::MakeItemlParam(TREE_ITEM_TYPE_PROJECT_BARCODE_MODE, 0);;
	TreeItem1 = TreeCtrl.InsertItem(&tvInsert);

//	if ( Project->m_ProjectParameter.m_BarcodeStates!=PROJECT_BARCODE_HARDWARE_STOP )
//	{
//		::_stprintf(text, _T("Barcode: %s"), BarcodeName);
//	}
//	else	
//	{
//		if ( Project->m_ProjectParameter.m_BarcodeAutoExtendMode != BARCODE_AUTO_EXTEND_MODE_DISABLE ) 
//		{	::_stprintf(text, _T("Barcode: %s_Extend"), BarcodeName);	}
//		else
//		{	::_stprintf(text, _T("Barcode: %s"), BarcodeName);	}
//	}

	tvInsert.hParent = hProjectItem;
	tvInsert.item.pszText = text;
	tvInsert.item.iImage = TREE_IMAGE_LIST_OTHERS;
	tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_OTHERS;
	tvInsert.item.lParam  = ::MakeItemlParam(TREE_ITEM_TYPE_PROJECT_BARCODE_MODE, 0);;
	TreeItem1 = TreeCtrl.InsertItem(&tvInsert);	
	return true;
}
//-----------------------------------------------------------------------------//
bool CEditComponentListDockPane::InsertPanelNodeToTreeCtrl(CThisTreeCtrl &TreeCtrl, HTREEITEM hPanelItem, CAOIPanel* pPanel, CAOITreeNode *pPanelTreeNode)//增加一個零件的節點
{	
	if ( NULL==hPanelItem || NULL==pPanel ) { return false; }	
	
	const size_t PanelFdCount      = pPanel->GetPanelFdCount();
	const size_t PanelBarcodeCount = pPanel->GetPanelBarcodeCount();
	const size_t PanelBoardCount   = pPanel->GetPanelBoardCount();	
	
	CString str;
	int    GroupID=0;
	size_t i=0, j=0, k=0;
	size_t BoardFdCount = 0;
	size_t BoardMarkCount = 0;
	size_t BoardBarcodeCount = 0;
	size_t BoardComponentCount=0;	
	size_t FdIndex=0;
	size_t BarcodeIndex=0;
	
	CAOIFd        *pFd = NULL;
	CAOIMark      *pMark = NULL;
	CAOIBoard     *pBoard = NULL;
	CAOIBarcode   *pBarcode = NULL;
	CAOIComponent *pComponent = NULL;	
	CAOIModel     *ModelPtr =NULL;
	MODEL_TYPE     ModelType=MODEL_TYPE_NULL;
	COLORREF       UnsetColor = AOIDataCollect.GetColorModelUnset();
	const DISTRICT_ID DistrictID = GetActiveDistrictID();

	TVINSERTSTRUCT tvInsert;
	tvInsert.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM;	
	HTREEITEM hFDGroupItem=NULL, hFDItem=NULL, hMarkItem=NULL, hBarcodeItem=NULL, hBoardItem=NULL;
	HTREEITEM SubTreeItem2=NULL, hComponentItem=NULL, hGroupItem=NULL;
	const size_t textlen = 256;
	TCHAR text[textlen]=_T("");	
	int ComponentType = 0;
	const bool ShowMark = CheckShowMark();
	const bool ShowFiducial= CheckShowFiducial();
	const bool ShowBarcode = CheckShowBarcode();
	const bool ShowComponent = CheckShowComponent();	
	//---------------------------------------------------------------------------//		
	//定位點
	if ( true == ShowFiducial )
	{
		str = _T("Fiducial");		
		str = AOIDataDefine.GetFdText();
		::_tcscpy(text, str);
		tvInsert.hParent = hPanelItem;
		tvInsert.item.pszText = text;
		tvInsert.item.iImage = TREE_IMAGE_LIST_FD;
		tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_FD;
		tvInsert.item.lParam  = ::MakeItemlParam(TREE_ITEM_TYPE_FD_GROUP, 0);
		hFDGroupItem = TreeCtrl.InsertItem(&tvInsert);
		TreeCtrl.SetItemTextColor(hFDGroupItem, m_clrFd);

		j=0;
		for ( i=0; i<PanelFdCount; i++ )
		{
			pFd = pPanel->GetPanelFdPtr(i, false);
			if ( NULL == pFd ) { continue; }
			if ( NULL != pFd->GetFdBoardPtr() ) { continue; }		
			if ( DistrictID != pFd->GetFdDistrictID() ) { continue; }

			//::strcpy(text, pfd->GetFiducialName());
			str = _T("FD");			
			str = AOIDataDefine.GetFdText();
			::_stprintf(text, _T("%s %d"), str, j+1);
			tvInsert.hParent = hFDGroupItem;
			tvInsert.item.pszText = text;
			tvInsert.item.iImage = TREE_IMAGE_LIST_FD;
			tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_FD_SELECTED;
			tvInsert.item.lParam  = ::MakeItemlParam(TREE_ITEM_TYPE_FD, pFd->GetFdIndex_Project());//Fiducial 引數
			hFDItem = TreeCtrl.InsertItem(&tvInsert);
			TreeCtrl.SetItemTextColor(hFDItem, m_clrComponent);		
			InsertFdNodeToTreeCtrl(TreeCtrl, hFDItem, pFd, i);
			j ++;
		}
	}
	//---------------------------------------------------------------------------//	
	//軟體條碼
	if ( true == ShowBarcode )
	{
		str = _T("Barcode");		
		str = AOIDataDefine.GetBarcodeText();
		::_tcscpy(text, str);
		tvInsert.hParent = hPanelItem;
		tvInsert.item.pszText = text;
		tvInsert.item.iImage = TREE_IMAGE_LIST_BARCODE;
		tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_BARCODE;
		tvInsert.item.lParam  = ::MakeItemlParam(TREE_ITEM_TYPE_BARCODE_GROUP, 0);
		//hFDGroupItem = TreeCtrl.InsertItem(&tvInsert);
		//TreeCtrl.SetItemTextColor(hFDGroupItem, m_clrFd);	
		j=0;
		for ( i=0; i<PanelBarcodeCount; i++ )
		{
			pBarcode = pPanel->GetPanelBarcodePtr(i, false);
			if ( NULL == pBarcode ) { continue; }
			if ( DistrictID != pBarcode->GetBarcodeDistrictID() ) { continue; }
			if ( pBarcode->GetBarcodeBoardPtr() != NULL ) { continue; }			
		
			//::strcpy(text, pfd->GetFiducialName());
			str = _T("Barcode");
			str = AOIDataDefine.GetBarcodeText();
			::_stprintf(text, _T("%s %d"), str, j+1);
			tvInsert.hParent = hPanelItem;
			tvInsert.item.pszText = text;
			tvInsert.item.iImage = TREE_IMAGE_LIST_BARCODE;
			tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_BARCODE_SELECTED;
			tvInsert.item.lParam  = ::MakeItemlParam(TREE_ITEM_TYPE_BARCODE, pBarcode->GetBarcodeIndex_Project());//Fiducial 引數
			hBarcodeItem = TreeCtrl.InsertItem(&tvInsert);
			TreeCtrl.SetItemTextColor(hBarcodeItem, m_clrComponent);
			//InsertFdNodeToTreeCtrl(TreeCtrl, hBarcodeItem, pBarcode, i);
			j ++;
		}	
	}
	//---------------------------------------------------------------------------//	
	//單板
	for ( i=0; i<PanelBoardCount; i++ )
	{
		pBoard = pPanel->GetPanelBoardPtr(i, false);
		if ( NULL == pBoard ) { continue; }
		if ( pBoard->GetBoardDeleted() == true ) { continue; }		

		BoardFdCount = pBoard->GetBoardFdCount();
		BoardMarkCount = pBoard->GetBoardMarkCount();
		BoardBarcodeCount = pBoard->GetBoardBarcodeCount();
		//----------------------------------------------------------------------//
		str = _T("Board");		
		str = AOIDataDefine.GetBoardText();
		::_stprintf(text, _T("%s %d"), str, pBoard->GetBoardIndex_Panel()+1);

		tvInsert.hParent = hPanelItem;
		tvInsert.item.pszText = text;		
		if ( pBoard->GetBoardBypassed() == false )
		{
			tvInsert.item.iImage = TREE_IMAGE_LIST_BOARD;
			tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_BOARD_SELECTED;
		}
		else
		{
			tvInsert.item.iImage = TREE_IMAGE_LIST_BOARD_BYPASS;
			tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_BOARD_BYPASS_SELECTED;			
		}
		tvInsert.item.lParam  = ::MakeItemlParam(TREE_ITEM_TYPE_BOARD, pBoard->GetBoardIndex_Project());//Board Index
		hBoardItem = TreeCtrl.InsertItem(&tvInsert);
		TreeCtrl.SetItemTextColor(hBoardItem, m_clrBoard);		
		
		CAOITreeNode BoardTreeNode(hBoardItem, pBoard);		

		//定位點
		if ( true == ShowFiducial )
		{
			BoardFdCount = pBoard->GetBoardFdCount();		
			if ( BoardFdCount > 0 ) 
			{
				str = _T("Fiducial");				
				str = AOIDataDefine.GetFdText();
				::_tcscpy(text, str);
				tvInsert.hParent = hBoardItem;
				tvInsert.item.pszText = text;
				tvInsert.item.iImage = TREE_IMAGE_LIST_FD;
				tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_FD;
				tvInsert.item.lParam  = ::MakeItemlParam(TREE_ITEM_TYPE_FD_GROUP, 0);
				hFDGroupItem = TreeCtrl.InsertItem(&tvInsert);
				TreeCtrl.SetItemTextColor(hFDGroupItem, m_clrFd);	
				k = 0;
				for ( j=0; j<BoardFdCount; j++ )
				{
					pFd = pBoard->GetBoardFdPtr(j, false);
					if ( NULL == pFd ) { continue; }
					if ( DistrictID != pFd->GetFdDistrictID() ) { continue; }
		
					//::strcpy(text, pfd->GetFiducialName());
					str = _T("FD");					
					str = AOIDataDefine.GetFdText();
					::_stprintf(text, _T("%s %d"), str, k+1);
					tvInsert.hParent = hFDGroupItem;
					tvInsert.item.pszText = text;
					tvInsert.item.iImage = TREE_IMAGE_LIST_FD;
					tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_FD_SELECTED;
					tvInsert.item.lParam  = ::MakeItemlParam(TREE_ITEM_TYPE_FD, pFd->GetFdIndex_Project());//Fiducial 引數
					hFDItem = TreeCtrl.InsertItem(&tvInsert);
					TreeCtrl.SetItemTextColor(hFDItem, m_clrComponent);
					InsertFdNodeToTreeCtrl(TreeCtrl, hFDItem, pFd, j);
					k ++;
				}
			}
		}
		//---------------------------------------------------------------------------//	
		//特徵點
		if ( true == ShowMark )
		{
			BoardMarkCount = pBoard->GetBoardMarkCount();
			if ( BoardMarkCount > 0 ) 
			{
				str = _T("Mark");				
				str = AOIDataDefine.GetMarkText();
				::_tcscpy(text, str);
				tvInsert.hParent = hBoardItem;
				tvInsert.item.pszText = text;
				tvInsert.item.iImage = TREE_IMAGE_LIST_BARCODE;
				tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_BARCODE;
				tvInsert.item.lParam  = ::MakeItemlParam(TREE_ITEM_TYPE_MARK_GROUP, 0);
				//hFDGroupItem = TreeCtrl.InsertItem(&tvInsert);
				//TreeCtrl.SetItemTextColor(hFDGroupItem, m_clrFd);	

				k = 0;
				for ( j=0; j<BoardMarkCount; j++ )
				{
					pMark = pBoard->GetBoardMarkPtr(j, false);
					if ( NULL == pMark ) { continue; }
					if ( DistrictID != pMark->GetMarkDistrictID() ) { continue; }
					GroupID = pMark->GetMarkGroupID();

					//::strcpy(text, pfd->GetFiducialName());
					str = _T("Mark");
					str = AOIDataDefine.GetMarkText();					
					::_stprintf(text, _T("%s %02d#%02d"), str, k+1, GroupID+1);
					tvInsert.hParent = hBoardItem;
					tvInsert.item.pszText = text;
					if ( pMark->GetMarkBypassed() == false )
					{
						tvInsert.item.iImage = TREE_IMAGE_LIST_MARK;
						tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_MARK_SELECTED;
					}
					else
					{
						tvInsert.item.iImage = TREE_IMAGE_LIST_MARK_BYPASS;
						tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_MARK_BYPASS_SELECTED;
					}
					tvInsert.item.lParam  = ::MakeItemlParam(TREE_ITEM_TYPE_MARK, pMark->GetMarkIndex_Project());//Fiducial 引數
					hMarkItem = TreeCtrl.InsertItem(&tvInsert);
					TreeCtrl.SetItemTextColor(hMarkItem, m_clrComponent);
					//InsertFdNodeToTreeCtrl(TreeCtrl, hMarkItem, pMark, j);
					k ++;
				}	
			}
		}
		//---------------------------------------------------------------------------//	
		//軟體條碼
		if ( true == ShowBarcode )
		{
			BoardBarcodeCount = pBoard->GetBoardBarcodeCount();
			if ( BoardBarcodeCount > 0 ) 
			{
				str = _T("Barcode");				
				str = AOIDataDefine.GetBarcodeText();
				::_tcscpy(text, str);
				tvInsert.hParent = hBoardItem;
				tvInsert.item.pszText = text;
				tvInsert.item.iImage = TREE_IMAGE_LIST_BARCODE;
				tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_BARCODE;
				tvInsert.item.lParam  = ::MakeItemlParam(TREE_ITEM_TYPE_BARCODE_GROUP, 0);
				//hFDGroupItem = TreeCtrl.InsertItem(&tvInsert);
				//TreeCtrl.SetItemTextColor(hFDGroupItem, m_clrFd);	
				k = 0;
				for ( j=0; j<BoardBarcodeCount; j++ )
				{
					pBarcode = pBoard->GetBoardBarcodePtr(j, false);
					if ( NULL == pBarcode ) { continue; }
					if ( DistrictID != pBarcode->GetBarcodeDistrictID() ) { continue; }
		
					//::strcpy(text, pfd->GetFiducialName());
					str = _T("Barcode");
					str = AOIDataDefine.GetBarcodeText();
					::_stprintf(text, _T("%s %d"), str, k+1);
					tvInsert.hParent = hBoardItem;
					tvInsert.item.pszText = text;
					tvInsert.item.iImage = TREE_IMAGE_LIST_BARCODE;
					tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_BARCODE_SELECTED;
					tvInsert.item.lParam  = ::MakeItemlParam(TREE_ITEM_TYPE_BARCODE, pBarcode->GetBarcodeIndex_Project());//Fiducial 引數
					hBarcodeItem = TreeCtrl.InsertItem(&tvInsert);
					TreeCtrl.SetItemTextColor(hBarcodeItem, m_clrComponent);
					//InsertFdNodeToTreeCtrl(TreeCtrl, hBarcodeItem, pBarcode, j);
					k ++;
				}	
			}
		}
		//---------------------------------------------------------------------------//
		if ( true == ShowComponent )
		{
			bool ComponentIsAgent=false;
			BoardComponentCount = pBoard->GetBoardComponentCount();
			for ( j=0; j<BoardComponentCount; j++ )
			{
				pComponent = pBoard->GetBoardComponentPtr(j, false);
				if ( NULL == pComponent ) { continue; }
				if ( DistrictID != pComponent->GetComponentDistrictID() ) { continue; }
				//if ( pComponent->GetIsDeleted_C() == true ) { continue; }				
				if ( pComponent->CheckComponentIsAgent() == true ) { continue; }				
				pComponent = pComponent->GetComponentResultPtr();

				if ( NULL != pPanelTreeNode )
				{
					BoardTreeNode.AddChildNode(CAOITreeNode(NULL, pComponent));
					continue;
				}

				ModelPtr = pComponent->GetComponentModelPtr();
				if ( NULL == ModelPtr )
				{	ModelType = MODEL_TYPE_NULL; }
				else
				{	ModelType = ModelPtr->GetModelType(); }

				::_tcscpy(text, pComponent->GetComponentShowName());

				tvInsert.hParent = hBoardItem;
				tvInsert.item.pszText = text;
				if ( pComponent->GetComponentBypassed() == TRUE )
				{
					tvInsert.item.iImage = TREE_IMAGE_LIST_COMPONENT_BYPASS;
					tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_COMPONENT_BYPASS_SELECTED;
				}
				else if ( pComponent->GetComponentXBoardUnit() == true )
				{
					tvInsert.item.iImage = TREE_IMAGE_LIST_COMPONENT_SKIP;
					tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_COMPONENT_SKIP_SELECTED;
				}
				else
				{
					tvInsert.item.iImage = TREE_IMAGE_LIST_COMPONENT;
					tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_COMPONENT_SELECTED;		
				}
				tvInsert.item.lParam  = ::MakeItemlParam(TREE_ITEM_TYPE_COMPONENT, pComponent->GetComponentIndex_Project());//Component Index
				hComponentItem = TreeCtrl.InsertItem(&tvInsert);
				TreeCtrl.SetItemTextColor(hComponentItem, m_clrComponent);
				if ( MODEL_TYPE_NULL == ModelType )
				{	TreeCtrl.SetItemTextColor(hComponentItem, UnsetColor); }
				//----------------------------------------------------------------------//
				if( InsertComponentWindowToTreeCtrl(TreeCtrl, hComponentItem, pComponent) == false ) 
				{	return false; }
			}		
		}

		if ( NULL != pPanelTreeNode )
		{
			const size_t ComponentTreeNodeCount=BoardTreeNode.GetChildNodeCount();
			size_t AddComponentTreeNodeCount=ComponentTreeNodeCount;
			if ( AddComponentTreeNodeCount > TREE_ITEM_PATCH_ENABLE_COUNT )
			{	AddComponentTreeNodeCount = 100; }		
			InsertPanelNodeToTreeCtrlByTreeNode(TreeCtrl, &BoardTreeNode, AddComponentTreeNodeCount);		
			pPanelTreeNode->AddChildNode(BoardTreeNode);
		}
	}	
	return true;
}
//--------------------------------------------------------------------------------//
bool CEditComponentListDockPane::InsertPanelNodeToTreeCtrlByTreeNode(CThisTreeCtrl &TreeCtrl, CAOITreeNode *pBoardTreeNode, int AddCount)
{
	if ( NULL == pBoardTreeNode ) { return false; }
	if ( pBoardTreeNode->GetChildNodeListDone() == true ) { return true; }
	HTREEITEM hBoardItem = pBoardTreeNode->GetHItem();
	if ( NULL == hBoardItem ) { return false; }
	CAOIBoard *pBoard = DYNAMIC_DOWNCAST(CAOIBoard, pBoardTreeNode->GetObjPtr());
	if ( NULL == pBoard ) { return false; }

	CAOIComponent *pComponent = NULL;	
	CAOIModel     *ModelPtr =NULL;
	MODEL_TYPE     ModelType=MODEL_TYPE_NULL;
	COLORREF       UnsetColor = AOIDataCollect.GetColorModelUnset();

	TVINSERTSTRUCT tvInsert;
	HTREEITEM hComponentItem=NULL;
	tvInsert.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM;		
	const size_t textlen = 256;
	TCHAR text[textlen]=_T("");	
	int ComponentType = 0;
	const bool ShowComponent = CheckShowComponent();	
	//---------------------------------------------------------------------------//
	if ( true == ShowComponent )
	{			
		int AddCnt=0;
		const int LastIndex=pBoardTreeNode->GetLastChildAdded();
		const int ChildCount=(int)(pBoardTreeNode->GetChildNodeCount());
		for ( int j=(LastIndex+1); j<ChildCount; j++ )
		{
			CAOITreeNode *ChildNodePtr=pBoardTreeNode->GetChildNodePtr(j, false);
			if ( NULL == ChildNodePtr ) { continue; }
			if ( ChildNodePtr->GetHItem() != NULL ) { continue; }
			pComponent = DYNAMIC_DOWNCAST(CAOIComponent, ChildNodePtr->GetObjPtr());
			if ( NULL == pComponent ) { continue; }

			ModelPtr = pComponent->GetComponentModelPtr();
			if ( NULL == ModelPtr )
			{	ModelType = MODEL_TYPE_NULL; }
			else
			{	ModelType = ModelPtr->GetModelType(); }

			::_tcscpy(text, pComponent->GetComponentShowName());
				
			tvInsert.hParent = hBoardItem;
			tvInsert.item.pszText = text;
			if ( pComponent->GetComponentBypassed() == true )
			{
				tvInsert.item.iImage = TREE_IMAGE_LIST_COMPONENT_BYPASS;
				tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_COMPONENT_BYPASS_SELECTED;
			}
			else if ( pComponent->GetComponentXBoardUnit() == true )
			{
				tvInsert.item.iImage = TREE_IMAGE_LIST_COMPONENT_SKIP;
				tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_COMPONENT_SKIP_SELECTED;
			}
			else
			{
				tvInsert.item.iImage = TREE_IMAGE_LIST_COMPONENT;
				tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_COMPONENT_SELECTED;		
			}
			tvInsert.item.lParam  = ::MakeItemlParam(TREE_ITEM_TYPE_COMPONENT, pComponent->GetComponentIndex_Project());//Component Index
			hComponentItem = TreeCtrl.InsertItem(&tvInsert);			
			ChildNodePtr->SetHItem(hComponentItem);
			TreeCtrl.SetItemTextColor(hComponentItem, m_clrComponent);
			if ( MODEL_TYPE_NULL == ModelType )
			{	TreeCtrl.SetItemTextColor(hComponentItem, UnsetColor); }
			if ( pComponent->GetComponentSelected() )
			{	TreeCtrl.SetItemState(hComponentItem, TVIS_SELECTED, TVIS_SELECTED);	}
			else
			{	TreeCtrl.SetItemState(hComponentItem, NULL, TVIS_SELECTED);	}
			//----------------------------------------------------------------------//
			if( InsertComponentWindowToTreeCtrl(TreeCtrl, hComponentItem, pComponent) == false ) 
			{	return false; }

			pBoardTreeNode->SetLastChildAdded(j);
			AddCnt ++;
			if ( AddCnt >= AddCount )
			{	break; }
		}		
	}	
	return true;
}
//--------------------------------------------------------------------------------//
bool CEditComponentListDockPane::InsertFdNodeToTreeCtrl(CThisTreeCtrl &TreeCtrl, HTREEITEM hFDItem, CAOIFd *pFd, size_t FDID)//增加一個定位點的節點	
{
	return true;
	if ( NULL==hFDItem || NULL==pFd ) { return false; }

	TVINSERTSTRUCT tvInsert;
	tvInsert.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM;
	
	CString str;
	HTREEITEM SubTreeItem=NULL;
	const size_t textlen = 256;
	TCHAR text[textlen]=_T("");	
	
	str = _T("Image Type");
	str = LoadMultiLanguageString(str, str);
	::_stprintf(text, _T("%s Normal"), str);
	tvInsert.hParent = hFDItem;
	tvInsert.item.pszText = text;
	tvInsert.item.iImage = TREE_IMAGE_LIST_OTHERS;
	tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_OTHERS;
	tvInsert.item.lParam  = FDID;//Fiducial 引數
	SubTreeItem = TreeCtrl.InsertItem(&tvInsert);
	
	//::_stprintf(text, "Golden Pos= (%.3f, %.3f)mm", pfd->GetFDStagePositionX()/1000.0, pfd->GetFDStagePositionY()/1000.0);
	str = _T("Teach Pos.");
	str = LoadMultiLanguageString(str, str);
	::_stprintf(text, _T("%s (%.0f, %.0f)"), str, 0.0, 0.0);
	tvInsert.hParent = hFDItem;
	tvInsert.item.pszText = text;
	tvInsert.item.iImage = TREE_IMAGE_LIST_OTHERS;
	tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_OTHERS;
	tvInsert.item.lParam  = FDID;//Fiducial 引數
	SubTreeItem = TreeCtrl.InsertItem(&tvInsert);
	
	//::_stprintf(text, "Golden Score= %.2f%%", pfd->GetFDScore() );		
	str = _T("Score");
	str = LoadMultiLanguageString(str, str);
	::_stprintf(text, _T("%s %.2f"), str, 0.0);
	tvInsert.hParent = hFDItem;
	tvInsert.item.pszText = text;
	tvInsert.item.iImage = TREE_IMAGE_LIST_OTHERS;
	tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_OTHERS;
	tvInsert.item.lParam  = FDID;//Fiducial 引數
	SubTreeItem = TreeCtrl.InsertItem(&tvInsert);		

	//::_stprintf(text, "Current Pos= (%.3f, %.3f)mm", pfd->GetCalibrateStagePosX()/1000.0, pfd->GetCalibrateStagePosY()/1000.0);		
	str = _T("Cur. Pos.");
	str = LoadMultiLanguageString(str, str);
	::_stprintf(text, _T("%s (%.0f, %.0f)"), str, 0.0, 0.0);	
	tvInsert.hParent = hFDItem;
	tvInsert.item.pszText = text;
	tvInsert.item.iImage = TREE_IMAGE_LIST_OTHERS;
	tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_OTHERS;
	tvInsert.item.lParam  = FDID;//Fiducial 引數
	SubTreeItem = TreeCtrl.InsertItem(&tvInsert);		

	//::_stprintf(text, "Exposure Time= %d us", pfd->GetFDExposureTime());			
	str = _T("Exp. Time");
	str = LoadMultiLanguageString(str, str);
	::_stprintf(text, _T("%s %.0f"), str, 0.0);
	tvInsert.hParent = hFDItem;
	tvInsert.item.pszText = text;
	tvInsert.item.iImage = TREE_IMAGE_LIST_OTHERS;
	tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_OTHERS;
	tvInsert.item.lParam  = FDID;//Fiducial 引數
	SubTreeItem = TreeCtrl.InsertItem(&tvInsert);		

	//str = CAlgImg::GetAlgImageSourceText(pfd->GetFDImageSource());
	//::_stprintf(text, "Binary Mode= %s", str);		
	::_tcscpy(text, _T("Binary Mode"));
	str = _T("Binary Mode");
	str = LoadMultiLanguageString(str, str);
	::_stprintf(text, _T("%s %s"), str, _T("None"));
	tvInsert.hParent = hFDItem;
	tvInsert.item.pszText = text;
	tvInsert.item.iImage = TREE_IMAGE_LIST_OTHERS;
	tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_OTHERS;
	tvInsert.item.lParam  = FDID;//Fiducial 引數
	SubTreeItem = TreeCtrl.InsertItem(&tvInsert);
	return true;
}
//--------------------------------------------------------------------------------//
bool CEditComponentListDockPane::InsertComponentWindowToTreeCtrl(CThisTreeCtrl &TreeCtrl, HTREEITEM HComponentItem, CAOIComponent *pComponent)//增加一個零件的節點	
{
	return true;

	if ( NULL==HComponentItem || NULL==pComponent ) { return false; }	
	//---------------------------------------------------------------------//		
	CString str;
	TVINSERTSTRUCT tvInsert;
	tvInsert.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM;	
	HTREEITEM NewComponentItem = NULL;
	HTREEITEM hModelItem = NULL;
	HTREEITEM hWindowItem = NULL;
	HTREEITEM hWndGroupltem = NULL;	
	
	int            i=0, j=0;
	int            WndGrupID = 0;	
	CString        WndItemText;	
//	CAOIWindow    *pWindow = NULL;	
//	CWndObj       *WndPtr = NULL;
//	CLandObj      *LandPtr = NULL;

	const size_t TextLen = 128;
	TCHAR ItemName[TextLen]=_T("");	
//	const int NWindows = pComponent->GetNWindows_C();
//	CModelObj  *ModelPtr = pComponent->GetComponentModelPtrActived();
	
	//Part Number Name
	str = pComponent->GetComponentPartNumber();
	::_tcscpy(ItemName, str);
	tvInsert.hParent = HComponentItem;
	tvInsert.item.pszText = ItemName;
	tvInsert.item.iImage = TREE_IMAGE_LIST_OTHERS;
	tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_OTHERS;
	tvInsert.item.lParam  = ::MakeItemlParam(TREE_ITEM_TYPE_COMPONENT_PART_NUMBER, 0);	
	hModelItem = TreeCtrl.InsertItem(&tvInsert);	
	//----------------------------------------------------------------------//		
	//Position
	pComponent->GetComponentLocationText(ItemName);	
	tvInsert.hParent = HComponentItem;
	tvInsert.item.pszText = ItemName;
	tvInsert.item.iImage = TREE_IMAGE_LIST_OTHERS;
	tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_OTHERS;
	tvInsert.item.lParam  = ::MakeItemlParam(TREE_ITEM_TYPE_COMPONENT_POSITION, 0);
	hModelItem = TreeCtrl.InsertItem(&tvInsert);
	//----------------------------------------------------------------------//
	pComponent->GetComponentOffsetText(ItemName);	
	tvInsert.hParent = HComponentItem;
	tvInsert.item.pszText = ItemName;
	tvInsert.item.iImage = TREE_IMAGE_LIST_OTHERS;
	tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_OTHERS;
	tvInsert.item.lParam  = ::MakeItemlParam(TREE_ITEM_TYPE_COMPONENT_OFFSET, 0);
	hModelItem = TreeCtrl.InsertItem(&tvInsert);
	//----------------------------------------------------------------------//	
	
	//if ( ModelPtr != NULL )
	//{	::_stprintf(ItemName, _T("%s"), ModelPtr->GetModelName());	}
	//else
	//{	::_stprintf(ItemName, _T("%s"), "NULL"); }
	str = pComponent->GetComponentModelName();
	::_stprintf(ItemName, _T("%s"), str);
	tvInsert.hParent = HComponentItem;
	tvInsert.item.pszText = ItemName;
	tvInsert.item.iImage = TREE_IMAGE_LIST_MODEL;
	tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_MODEL;
	tvInsert.item.lParam  = ::MakeItemlParam(TREE_ITEM_TYPE_COMPONENT_MODE, 0);;//Com Tree:-1, Model Tree:Component Cluster Index, pComponent->GetModelIndex_C();
	hModelItem = TreeCtrl.InsertItem(&tvInsert);	
	return true;
}
//--------------------------------------------------------------------------------//
bool CEditComponentListDockPane::UpdateComponentTreeCtrl(CThisTreeCtrl &TreeCtrl, DWORD UpdateFlag)
{	
	CAOIProject *Project = GetActiveProject();
	if ( NULL == Project ) 
	{ 
		ClearComponentTreeCtrl(TreeCtrl, m_StopComponentTreeBeClick);		
		return true; 
	}

	//注意SowComponentTreeSelected會將部分的Item給修改掉
	if ( ShowComponentTreeSelected(TreeCtrl) == false )
	{	return false; }

	if ( UpdateComponentTreeCtrlState(TreeCtrl, UpdateFlag) == false )
	{	return false; }	
	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditComponentListDockPane::UpdateComponentTreeCtrlState(CThisTreeCtrl &TreeCtrl, DWORD UpdateFlag)
{
	if ( NULL == TreeCtrl.GetSafeHwnd() ) { return false; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }

	int i=0;
	int GroupID = 0;
	unsigned int Index=0;
	CString      str;	
	CString      ItemText;
	DWORD_PTR    ItemData = 0;
	unsigned int ItemType = 0;
	unsigned int ItemIndex = 0;
	HTREEITEM  hFdItem = NULL;
	HTREEITEM  hPanelItem = NULL;
	HTREEITEM  hBoardItem = NULL;
	HTREEITEM  hComponentItem = NULL;
	HTREEITEM  hGroupItem = NULL;
	HTREEITEM  hWindowItem = NULL;
	HTREEITEM  hWndGroupItem = NULL;

	CAOIFd        *FdPtr = NULL;
	CAOIMark      *pMark = NULL;
	CAOIPanel     *pPanel = NULL;
	CAOIBoard     *pBoard = NULL;
	CAOIBarcode   *pBarcode = NULL;
	CAOIComponent *pComponent = NULL;
	CAOIWindow    *pWindow = NULL;
	CAOIModel     *ModelPtr = NULL;	
	bool           bUpdateTxt = FALSE;
	bool           bUpdateState = FALSE;
	MODEL_TYPE     ModelType = MODEL_TYPE_NULL;
	COLORREF       UnsetColor = AOIDataCollect.GetColorModelUnset();

	if ( (UpdateFlag&TREE_CTRL_UPDATE_TEXT) == 0 ) 
	{	bUpdateTxt = false;	}
	else
	{	bUpdateTxt = true;	}

	if ( (UpdateFlag&TREE_CTRL_UPDATE_STATE) == 0 ) 
	{	bUpdateState = false;	}
	else
	{	bUpdateState = true;	}

#ifdef _DEBUG
	CString FdName, PanelName, BoardName, ComName, GroupName, WndGroupName, WindowName;
#endif

	TreeCtrl.SetRedraw(FALSE);
	hPanelItem = TreeCtrl.GetRootItem();
	while ( NULL != hPanelItem )
	{
	#ifdef _DEBUG
		PanelName = TreeCtrl.GetItemText(hPanelItem);
	#endif
		ItemData = TreeCtrl.GetItemData(hPanelItem);		
		::DecodeItemlParam(ItemData, ItemType, ItemIndex);		
		if ( TREE_ITEM_TYPE_PANEL != ItemType )		
		{
			hPanelItem = TreeCtrl.GetNextSiblingItem(hPanelItem);
			continue;
		}
		
		pPanel = ProjectPtr->GetProjectPanelPtr(ItemIndex, true);
		if ( NULL == pPanel )
		{
			hPanelItem = TreeCtrl.GetNextSiblingItem(hPanelItem);
			continue;
		}
		
		if ( true == bUpdateState )
		{
			if ( pPanel->GetPanelBypassed() == false )
			{	TreeCtrl.SetItemImage(hPanelItem, TREE_IMAGE_LIST_PANEL, TREE_IMAGE_LIST_PANEL_SELECTED);	}
			else
			{	TreeCtrl.SetItemImage(hPanelItem, TREE_IMAGE_LIST_PANEL_BYPASS, TREE_IMAGE_LIST_PANEL_BYPASS_SELECTED);	}				
			
			if ( pPanel->GetPanelSelected() == true ) 
			{	TreeCtrl.SetItemState(hPanelItem, TVIS_SELECTED, TVIS_SELECTED); }
			else
			{	TreeCtrl.SetItemState(hPanelItem, NULL, TVIS_SELECTED);	}		
		}
		if ( true == bUpdateTxt )
		{
			str = _T("Panel");
			str = AOIDataDefine.GetPanelText();			
			ItemText.Format(_T("%s %d"), str, ItemIndex+1);
			TreeCtrl.SetItemText(hPanelItem, ItemText);
		}
		hBoardItem = TreeCtrl.GetChildItem(hPanelItem);
		while ( NULL!=hBoardItem  )
		{
		#ifdef _DEBUG
			BoardName = TreeCtrl.GetItemText(hBoardItem);
		#endif
			ItemData = TreeCtrl.GetItemData(hBoardItem);		
			::DecodeItemlParam(ItemData, ItemType, ItemIndex);

			if ( TREE_ITEM_TYPE_FD_GROUP == ItemType )
			{	
				hFdItem = TreeCtrl.GetChildItem(hBoardItem);
				while ( hFdItem!=NULL )
				{
				#ifdef _DEBUG
					FdName = TreeCtrl.GetItemText(hFdItem);
				#endif				
					ItemData = TreeCtrl.GetItemData(hFdItem);		
					::DecodeItemlParam(ItemData, ItemType, ItemIndex);
					if ( TREE_ITEM_TYPE_FD != ItemType )
					{
						hFdItem = TreeCtrl.GetNextSiblingItem(hFdItem);
						continue;
					}
					
					FdPtr = ProjectPtr->GetProjectFdPtr(ItemIndex, true);
					if ( NULL == FdPtr )
					{
						hFdItem = TreeCtrl.GetNextSiblingItem(hFdItem);
						continue;
					}
					if ( true == bUpdateState )
					{	
						if ( FdPtr->GetFdSelected() == true ) 
						{	TreeCtrl.SetItemState(hFdItem, TVIS_SELECTED, TVIS_SELECTED); }
						else
						{	TreeCtrl.SetItemState(hFdItem, NULL, TVIS_SELECTED);	}
					}
					if ( true == bUpdateTxt )
					{
						if ( NULL == pPanel )
						{	Index = FdPtr->GetFdIndex_Panel();	}
						else
						{	Index = pPanel->CalcPanelFdInex(FdPtr); }
						str = _T("FD");
						str = AOIDataDefine.GetFdText();
						ItemText.Format(_T("%s %d"), str, Index+1);
						TreeCtrl.SetItemText(hFdItem, ItemText);
					}					
					hFdItem = TreeCtrl.GetNextSiblingItem(hFdItem);
				};
				hBoardItem = TreeCtrl.GetNextSiblingItem(hBoardItem);
				continue;
			}

			if ( TREE_ITEM_TYPE_BARCODE == ItemType )
			{
				pBarcode = ProjectPtr->GetProjectBarcodePtr(ItemIndex, true);
				if ( NULL == pBarcode ) 
				{
					hBoardItem = TreeCtrl.GetNextSiblingItem(hBoardItem);
					continue;
				}
				if ( true == bUpdateState )
				{	
					if ( pBarcode->GetBarcodeSelected() == true ) 
					{	TreeCtrl.SetItemState(hBoardItem, TVIS_SELECTED, TVIS_SELECTED); }
					else
					{	TreeCtrl.SetItemState(hBoardItem, NULL, TVIS_SELECTED);	}
				}
				if ( true == bUpdateTxt )
				{
					if ( NULL == pPanel )
					{	Index = pBarcode->GetBarcodeIndex_Panel();	}
					else
					{	Index = pPanel->CalcPanelBarcodeInex(pBarcode); }
					str = _T("Barcode");
					str = AOIDataDefine.GetBarcodeText();
					ItemText.Format(_T("%s %d"), str, Index+1);
					TreeCtrl.SetItemText(hBoardItem, ItemText);
				}
				hBoardItem = TreeCtrl.GetNextSiblingItem(hBoardItem);
				continue;
			}

			if ( TREE_ITEM_TYPE_BOARD != ItemType  ) 
			{
				hBoardItem = TreeCtrl.GetNextSiblingItem(hBoardItem);
				continue;
			}

			pBoard = ProjectPtr->GetProjectBoardPtr(ItemIndex, true);
			if ( NULL == pBoard ) 
			{
				hBoardItem = TreeCtrl.GetNextSiblingItem(hBoardItem);
				continue;
			}
			
			if ( true == bUpdateState )
			{
				if ( pBoard->GetBoardBypassed() == false )
				{	TreeCtrl.SetItemImage(hBoardItem, TREE_IMAGE_LIST_BOARD, TREE_IMAGE_LIST_BOARD_SELECTED);	}
				else
				{	TreeCtrl.SetItemImage(hBoardItem, TREE_IMAGE_LIST_BOARD_BYPASS, TREE_IMAGE_LIST_BOARD_BYPASS_SELECTED);	}
				
				if ( pBoard->GetBoardSelected() == true ) 
				{	TreeCtrl.SetItemState(hBoardItem, TVIS_SELECTED, TVIS_SELECTED); }
				else
				{	TreeCtrl.SetItemState(hBoardItem, NULL, TVIS_SELECTED);	}
			}
			if ( true == bUpdateTxt )
			{
				str = _T("Board");
				str = AOIDataDefine.GetBoardText();
				ItemText.Format(_T("%s %d"), str, pBoard->GetBoardIndex_Panel()+1);
				TreeCtrl.SetItemText(hBoardItem, ItemText);
			}

			hComponentItem = TreeCtrl.GetChildItem(hBoardItem);
			while ( hComponentItem!=NULL )
			{
			#ifdef _DEBUG
				ComName = TreeCtrl.GetItemText(hComponentItem);
			#endif				
				ItemData = TreeCtrl.GetItemData(hComponentItem);		
				::DecodeItemlParam(ItemData, ItemType, ItemIndex);
				
				if ( TREE_ITEM_TYPE_FD_GROUP == ItemType )
				{	
					hFdItem = TreeCtrl.GetChildItem(hComponentItem);
					while ( hFdItem!=NULL )
					{
					#ifdef _DEBUG
						FdName = TreeCtrl.GetItemText(hFdItem);
					#endif				
						ItemData = TreeCtrl.GetItemData(hFdItem);		
						::DecodeItemlParam(ItemData, ItemType, ItemIndex);
						if ( TREE_ITEM_TYPE_FD != ItemType )
						{
							hFdItem = TreeCtrl.GetNextSiblingItem(hFdItem);
							continue;
						}
					
						FdPtr = ProjectPtr->GetProjectFdPtr(ItemIndex, true);
						if ( NULL == FdPtr )
						{
							hFdItem = TreeCtrl.GetNextSiblingItem(hFdItem);
							continue;
						}
						if ( true == bUpdateState )
						{	
							if ( FdPtr->GetFdSelected() == true ) 
							{	TreeCtrl.SetItemState(hFdItem, TVIS_SELECTED, TVIS_SELECTED); }
							else
							{	TreeCtrl.SetItemState(hFdItem, NULL, TVIS_SELECTED);	}
						}
						if ( true == bUpdateTxt )
						{	
							str = _T("FD");
							str = AOIDataDefine.GetFdText();
							ItemText.Format(_T("%s %d"), str, FdPtr->GetFdIndex_Board()+1);
							TreeCtrl.SetItemText(hFdItem, ItemText);
						}					
						hFdItem = TreeCtrl.GetNextSiblingItem(hFdItem);
					};
					hComponentItem = TreeCtrl.GetNextSiblingItem(hComponentItem);
					continue;
				}
				
				if ( TREE_ITEM_TYPE_BARCODE == ItemType )
				{
					pBarcode = ProjectPtr->GetProjectBarcodePtr(ItemIndex, true);
					if ( NULL == pBarcode ) 
					{
						hComponentItem = TreeCtrl.GetNextSiblingItem(hComponentItem);
						continue;
					}
					if ( true == bUpdateState )
					{	
						if ( pBarcode->GetBarcodeSelected() == true ) 
						{	TreeCtrl.SetItemState(hComponentItem, TVIS_SELECTED, TVIS_SELECTED); }
						else
						{	TreeCtrl.SetItemState(hComponentItem, NULL, TVIS_SELECTED);	}
					}
					if ( true == bUpdateTxt )
					{
						str = _T("Barcode");
						str = AOIDataDefine.GetBarcodeText();
						ItemText.Format(_T("%s %d"), str, pBarcode->GetBarcodeIndex_Board()+1);
						TreeCtrl.SetItemText(hComponentItem, ItemText);
					}
					hComponentItem = TreeCtrl.GetNextSiblingItem(hComponentItem);
					continue;
				}
				
				if ( TREE_ITEM_TYPE_MARK == ItemType )
				{
					pMark = ProjectPtr->GetProjectMarkPtr(ItemIndex, true);
					if ( NULL == pMark ) 
					{
						hComponentItem = TreeCtrl.GetNextSiblingItem(hComponentItem);
						continue;
					}
					if ( true == bUpdateState )
					{	
						if ( pMark->GetMarkBypassed() == false )
						{	TreeCtrl.SetItemImage(hComponentItem, TREE_IMAGE_LIST_MARK, TREE_IMAGE_LIST_MARK_SELECTED);	}
						else
						{	TreeCtrl.SetItemImage(hComponentItem, TREE_IMAGE_LIST_MARK_BYPASS, TREE_IMAGE_LIST_MARK_BYPASS_SELECTED);	}

						if ( pMark->GetMarkSelected() == true ) 
						{	TreeCtrl.SetItemState(hComponentItem, TVIS_SELECTED, TVIS_SELECTED); }
						else
						{	TreeCtrl.SetItemState(hComponentItem, NULL, TVIS_SELECTED);	}
					}
					if ( true == bUpdateTxt )
					{
						str = _T("Mark");
						str = AOIDataDefine.GetMarkText();
						GroupID = pMark->GetMarkGroupID();
						ItemText.Format(_T("%s %02d#%02d"), str, pMark->GetMarkIndex_Board()+1, GroupID+1);
						TreeCtrl.SetItemText(hComponentItem, ItemText);
					}
					hComponentItem = TreeCtrl.GetNextSiblingItem(hComponentItem);
					continue;
				}	

				if ( TREE_ITEM_TYPE_COMPONENT != ItemType ) 
				{	
					hComponentItem = TreeCtrl.GetNextSiblingItem(hComponentItem);
					continue; 
				}
				pComponent = ProjectPtr->GetProjectComponentPtr(ItemIndex, true);
				if ( NULL == pComponent ) 
				{
					hComponentItem = TreeCtrl.GetNextSiblingItem(hComponentItem);
					continue; 
				}

				if ( true == bUpdateState )
				{
					if ( pComponent->GetComponentBypassed() == false )
					{	
						if ( pComponent->GetComponentXBoardUnit() == false )
						{	TreeCtrl.SetItemImage(hComponentItem, TREE_IMAGE_LIST_COMPONENT, TREE_IMAGE_LIST_COMPONENT_SELECTED);	}
						else
						{	TreeCtrl.SetItemImage(hComponentItem, TREE_IMAGE_LIST_COMPONENT_SKIP, TREE_IMAGE_LIST_COMPONENT_SKIP_SELECTED);	}
					}
					else
					{	TreeCtrl.SetItemImage(hComponentItem, TREE_IMAGE_LIST_COMPONENT_BYPASS, TREE_IMAGE_LIST_COMPONENT_BYPASS_SELECTED);	}				
					
					if ( pComponent->GetComponentSelected() == true ) 
					{	TreeCtrl.SetItemState(hComponentItem, TVIS_SELECTED, TVIS_SELECTED);	}
					else
					{	TreeCtrl.SetItemState(hComponentItem, NULL, TVIS_SELECTED);	}
				}

				if ( true == bUpdateTxt )
				{
					ItemText = pComponent->GetComponentShowName();					
					TreeCtrl.SetItemText(hComponentItem, ItemText);
				}
				ModelPtr = pComponent->GetComponentModelPtr();
				if ( NULL == ModelPtr )
				{	ModelType = MODEL_TYPE_NULL; }
				else
				{	ModelType = ModelPtr->GetModelType(); }
				if ( MODEL_TYPE_NULL == ModelType )
				{	TreeCtrl.SetItemTextColor(hComponentItem, UnsetColor); }
				else
				{	TreeCtrl.SetItemTextColor(hComponentItem, m_clrComponent); }

				hGroupItem = TreeCtrl.GetChildItem(hComponentItem);
				while ( hGroupItem != NULL )
				{					
				#ifdef _DEBUG
					GroupName = TreeCtrl.GetItemText(hGroupItem);
				#endif						
					ItemData = TreeCtrl.GetItemData(hGroupItem);		
					::DecodeItemlParam(ItemData, ItemType, ItemIndex);
					if ( ItemType==TREE_ITEM_TYPE_COMPONENT_PACKAGE || 
						 ItemType==TREE_ITEM_TYPE_COMPONENT_PART_NUMBER || 
						 ItemType==TREE_ITEM_TYPE_COMPONENT_POSITION ||
						 ItemType==TREE_ITEM_TYPE_COMPONENT_OFFSET
						)
					{
						if ( true == bUpdateTxt )
						{
							if ( TREE_ITEM_TYPE_COMPONENT_PACKAGE == ItemType )
							{
								ItemText = pComponent->GetComponentModelName();					
								TreeCtrl.SetItemText(hGroupItem, ItemText);
							}
							if ( TREE_ITEM_TYPE_COMPONENT_PART_NUMBER == ItemType )
							{
								ItemText = pComponent->GetComponentPartNumber();					
								TreeCtrl.SetItemText(hGroupItem, ItemText);
							}
							if ( TREE_ITEM_TYPE_COMPONENT_POSITION == ItemType )
							{	 
								pComponent->GetComponentLocationText(ItemText);
								TreeCtrl.SetItemText(hGroupItem, ItemText);
							}
							if ( TREE_ITEM_TYPE_COMPONENT_OFFSET == ItemType )
							{	 
								pComponent->GetComponentOffsetText(ItemText);
								TreeCtrl.SetItemText(hGroupItem, ItemText);
							}
						}						
						hGroupItem = TreeCtrl.GetNextSiblingItem(hGroupItem);
						continue;
					}

					hWndGroupItem = TreeCtrl.GetChildItem(hGroupItem);//Wnd Group
					while ( hWndGroupItem != NULL )
					{
					#ifdef _DEBUG
						WndGroupName = TreeCtrl.GetItemText(hWndGroupItem);
					#endif
						hWindowItem = TreeCtrl.GetChildItem(hWndGroupItem);//First Window Item
						while ( hWindowItem!= NULL)
						{
						#ifdef _DEBUG
							WindowName = TreeCtrl.GetItemText(hWindowItem);
						#endif								
							ItemData = TreeCtrl.GetItemData(hWindowItem);		
							::DecodeItemlParam(ItemData, ItemType, ItemIndex);
							if ( TREE_ITEM_TYPE_WINDOW != ItemType )
							{
								hWindowItem = TreeCtrl.GetNextSiblingItem(hWindowItem);
								continue;
							}

							pWindow = pComponent->GetComponentWindowPtr(ItemIndex, true);
							if ( NULL == pWindow ) 
							{
								hWindowItem = TreeCtrl.GetNextSiblingItem(hWindowItem);
								continue;
							}

							if ( true == bUpdateState )
							{
								if ( pWindow->GetWindowSelected() == true )
								{	TreeCtrl.SetItemState(hWindowItem, TVIS_SELECTED, TVIS_SELECTED);	}
								else
								{	TreeCtrl.SetItemState(hWindowItem, NULL, TVIS_SELECTED);	}
							}
							if ( true == bUpdateTxt )
							{
							}
							hWindowItem = TreeCtrl.GetNextSiblingItem(hWindowItem);
						};
						hWndGroupItem = TreeCtrl.GetNextSiblingItem(hWndGroupItem);
					};						
					hGroupItem = TreeCtrl.GetNextSiblingItem(hGroupItem);
				};								
				hComponentItem = TreeCtrl.GetNextSiblingItem(hComponentItem);
			};			
			hBoardItem = TreeCtrl.GetNextSiblingItem(hBoardItem);	
		};		
		hPanelItem = TreeCtrl.GetNextSiblingItem(hPanelItem);	
	};	
	TreeCtrl.SetRedraw(TRUE);
	return true;
}
//-------------------------------------------------------------------------------------//
UINT CEditComponentListDockPane::GetComponentTreeCtrlPopupMenuID(CThisTreeCtrl &TreeCtrl, CPoint Point)
{
	UINT MenuID = 0;
	UINT uFlags = 0;
	HTREEITEM hItem = TreeCtrl.HitTest(Point, &uFlags);
	if ( NULL == hItem ) { return MenuID; }
	m_TreeItemSelected = hItem;
#ifdef _DEBUG
	CString ItemText = TreeCtrl.GetItemText(hItem);
#endif
	
	unsigned int ItemType=0, ItemIndex=0;
	DWORD_PTR    ItemData = TreeCtrl.GetItemData(hItem);
	::DecodeItemlParam(ItemData, ItemType, ItemIndex);	
	switch ( ItemType )
	{
	case TREE_ITEM_TYPE_PROJECT:
	case TREE_ITEM_TYPE_PROJECT_FILENAME:
	case TREE_ITEM_TYPE_PROJECT_MODULE:		
	case TREE_ITEM_TYPE_PROJECT_REGION_W:
	case TREE_ITEM_TYPE_PROJECT_REGION_H:
	case TREE_ITEM_TYPE_PROJECT_FD_RANGE_W:
	case TREE_ITEM_TYPE_PROJECT_FD_RANGE_H:
	case TREE_ITEM_TYPE_PROJECT_BARCODE_MODE:	
		MenuID = IDR_MENU_TREE_PROJECT;
		break;
	case TREE_ITEM_TYPE_PANEL:
		MenuID = IDR_MENU_TREE_PANEL;
		break;		
	case TREE_ITEM_TYPE_FD:
		MenuID = IDR_MENU_TREE_FD;
		break;
	case TREE_ITEM_TYPE_MARK:
		MenuID = IDR_MENU_TREE_MARK;
		break;
	case TREE_ITEM_TYPE_BARCODE:
		MenuID = IDR_MENU_TREE_BARCODE;
		break;
	case TREE_ITEM_TYPE_BOARD:
		MenuID = IDR_MENU_TREE_BOARD;
		break;
	case TREE_ITEM_TYPE_COMPONENT:
	case TREE_ITEM_TYPE_COMPONENT_PACKAGE:
	case TREE_ITEM_TYPE_COMPONENT_PART_NUMBER:
	case TREE_ITEM_TYPE_COMPONENT_POSITION:
	case TREE_ITEM_TYPE_COMPONENT_OFFSET:
	case TREE_ITEM_TYPE_COMPONENT_MODE:
		MenuID = IDR_MENU_TREE_COMPONENT;
		break;
	case TREE_ITEM_TYPE_MODEL:
		//MenuID = IDR_MENU_TREE_COMPONENT;
		break;
	case TREE_ITEM_TYPE_WINDOW:
		//MenuID = IDR_MENU_TREE_COMPONENT;
		break;
	}
	return MenuID;
}
//--------------------------------------------------------------------------------//
bool CEditComponentListDockPane::ExecSelectComponentTreeWndItem(CThisTreeCtrl &TreeCtrl, HTREEITEM hItem, bool RBtn)//執行選取到零件樹狀圖的Item
{	
	if ( TRUE == m_StopComponentTreeBeClick ) { return true; }		
//	AOIDataCollect.SetCallBackHWND(AOIDataCollect.GetViewHWND());
//	this->ResetClickStatus();	
	if ( hItem == NULL ) { return true; }
	//-----------------------------------------------------------------------//	
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }		
	
	//初始化回傳直	
	unsigned int FdIdx = -1;
	unsigned int MarkIdx = -1;
	unsigned int PanelIdx = -1;
	unsigned int BoardIdx = -1;
	unsigned int BarcodeIdx = -1;
	unsigned int ComponentIdx = -1;
	unsigned int WindowIdx = -1;
		
	DWORD_PTR    ItemData = 0;
	unsigned int ItemType = 0;
	unsigned int ItemIndex = 0;	
	HTREEITEM hItemParent = hItem;
	HTREEITEM hItemChild  = hItem;
	HTREEITEM hItemTemp   = hItem;

	CString str;
#ifdef _DEBUG
	str = TreeCtrl.GetItemText(hItem);
#endif
	ItemData = TreeCtrl.GetItemData(hItem);
	::DecodeItemlParam(ItemData, ItemType, ItemIndex);
	
	CAOIFd        *pFd=NULL;
	CAOIMark      *pMark=NULL;
	CAOIPanel     *pPanel = NULL;
	CAOIBoard     *pBoard = NULL;
	CAOIBarcode   *pBarcode = NULL;
	CAOIComponent *pComponent = NULL;
	CAOIWindow    *pWindow = NULL;

	pComponent = ProjectPtr->GetProjectActiveComponent();
	//AOIDataCollect.CloseActiveComponent(pComponent);	
	bool AutoExpand_C = false;
	ProjectPtr->ResetProjectActiveIndex();	
	AOIDataCollect.CloseActiveComponent(pComponent);
	const bool MultiSelectMode = AOIDataCollect.CheckMultiSelectMode();
	if ( MultiSelectMode == false )
	{	ProjectPtr->SelectProjectAllObjects(false);	}

	//取得點的的結點是第幾層
	const int NodeLevel = JetAPI::GetTreeNodeLevel(TreeCtrl, hItem);	
	switch ( ItemType )
	{
	case TREE_ITEM_TYPE_PROJECT:
	case TREE_ITEM_TYPE_PROJECT_MODULE:
	case TREE_ITEM_TYPE_PROJECT_FILENAME:
		TreeCtrl.Expand(hItem, TVE_EXPAND);
		ProjectPtr->SelectProjectAllObjects(true);
		break;
	case TREE_ITEM_TYPE_PANEL:
		pPanel = ProjectPtr->GetProjectPanelPtr(ItemIndex, true);
		if ( NULL == pPanel ) { return FALSE; }
		PanelIdx = ItemIndex;
		pPanel->SetPanelSelected(true);
		pPanel->SelectPanelAllObjects(true);		
		//AutoExpand_C = true;
		break;
	case TREE_ITEM_TYPE_FD_GROUP:
		break;
	case TREE_ITEM_TYPE_FD:
		pFd = ProjectPtr->GetProjectFdPtr(ItemIndex, true);
		if ( NULL == pFd ) { return FALSE; }
		FdIdx = ItemIndex;
		pFd->SetFdSelected(true);
		pBoard = pFd->GetFdBoardPtr();
		if ( NULL != pBoard )
		{	BoardIdx = pBoard->GetBoardIndex_Project(); }
		pPanel = pFd->GetFdPanelPtr();
		if ( NULL != pPanel )
		{	PanelIdx = pPanel->GetPanelIndex_Project(); }		
		break;
	case TREE_ITEM_TYPE_BARCODE:
		pBarcode = ProjectPtr->GetProjectBarcodePtr(ItemIndex, true);
		if ( NULL == pBarcode ) { return FALSE; }
		BarcodeIdx = ItemIndex;
		pBarcode->SetBarcodeSelected(true);
		pBoard = pBarcode->GetBarcodeBoardPtr();
		if ( NULL != pBoard )
		{	BoardIdx = pBoard->GetBoardIndex_Project(); }
		pPanel = pBarcode->GetBarcodePanelPtr();
		if ( NULL != pPanel )
		{	PanelIdx = pPanel->GetPanelIndex_Project(); }				
		break;
	case TREE_ITEM_TYPE_BOARD:
		pBoard = ProjectPtr->GetProjectBoardPtr(ItemIndex, true);
		if ( NULL == pBoard ) { return FALSE; }
		pPanel = pBoard->GetBoardPanelPtr();
		if ( NULL != pPanel )
		{	PanelIdx = pPanel->GetPanelIndex_Project(); }
		BoardIdx = ItemIndex;
		pBoard->SetBoardSelected(true);
		pBoard->SelectBoardAllObjects(true);		
		//AutoExpand_C = true;
		break;
	case TREE_ITEM_TYPE_MARK:
		pMark = ProjectPtr->GetProjectMarkPtr(ItemIndex, true);
		if ( NULL == pMark ) { return FALSE; }
		MarkIdx = ItemIndex;
		pMark->SetMarkSelected(true);
		pBoard = pMark->GetMarkBoardPtr();
		if ( NULL != pBoard )
		{	BoardIdx = pBoard->GetBoardIndex_Project(); }
		pPanel = pMark->GetMarkPanelPtr();
		if ( NULL != pPanel )
		{	PanelIdx = pPanel->GetPanelIndex_Project(); }				
		break;
	case TREE_ITEM_TYPE_COMPONENT:
	case TREE_ITEM_TYPE_COMPONENT_PACKAGE:
	case TREE_ITEM_TYPE_COMPONENT_PART_NUMBER:
	case TREE_ITEM_TYPE_COMPONENT_POSITION:
	case TREE_ITEM_TYPE_COMPONENT_OFFSET:
	case TREE_ITEM_TYPE_COMPONENT_MODE:
		pComponent = ProjectPtr->GetProjectComponentPtr(ItemIndex, true);
		if ( NULL == pComponent ) { return FALSE; }
		ComponentIdx = ItemIndex;
		pBoard = pComponent->GetComponentBoardPtr();
		if ( NULL != pBoard )
		{	BoardIdx = pBoard->GetBoardIndex_Project(); }
		pPanel = pComponent->GetComponentPanelPtr();
		if ( NULL != pPanel )
		{	PanelIdx = pPanel->GetPanelIndex_Project(); }
		pComponent->SetComponentSelected(true);	
		//AutoExpand_C = true;
		break;	
	case TREE_ITEM_TYPE_MODEL:
		break;
	case TREE_ITEM_TYPE_WINDOW:
		break;
	}
	
	ProjectPtr->SetProjectActiveFdIndex(FdIdx);	
	ProjectPtr->SetProjectActiveMarkIndex(MarkIdx);
	ProjectPtr->SetProjectActivePanelIndex(PanelIdx);
	ProjectPtr->SetProjectActiveBoardIndex(BoardIdx);
	ProjectPtr->SetProjectActiveBarcodeIndex(BarcodeIdx);
	ProjectPtr->SetProjectActiveComponentIndex(ComponentIdx);
	ProjectPtr->SetProjectActiveComponentWindowIndex(WindowIdx);		
	MULTI_BOARD_CTRL_MODE MultiBoardCtrlMode = AOIDataCollect.GetMultiBoardCtrlMode();

	bool  bUpdateStats = false;
	if ( TREE_ITEM_TYPE_COMPONENT == ItemType ||
		 TREE_ITEM_TYPE_COMPONENT_PACKAGE == ItemType ||
		 TREE_ITEM_TYPE_COMPONENT_PART_NUMBER == ItemType ||
		 TREE_ITEM_TYPE_COMPONENT_POSITION == ItemType ||
		 TREE_ITEM_TYPE_COMPONENT_OFFSET == ItemType ||
		 TREE_ITEM_TYPE_COMPONENT_MODE == ItemType	)
	{	
		bUpdateStats = true;
		if ( MULTI_BOARD_CTRL_DISABLE != MultiBoardCtrlMode )
		{
			std::vector<CAOIComponent*> SelComponentList;
			ProjectPtr->GetProjectComponentSelected(SelComponentList);
			ProjectPtr->SelectProjectComponentByMultiBoardCtrlMode(SelComponentList, MultiBoardCtrlMode);
		}
		AOIDataCollect.SetModelAttachedObj(MODEL_ATTACHED_COMPONENT);
		BuildInfoPropCtrl_Component(m_wndInfomationPropCtrl, pComponent);
	}
	else if ( TREE_ITEM_TYPE_FD_GROUP == ItemType ) 
	{	
		bUpdateStats = true;	
		ClearInfoPropCtrl();
	}
	else if ( TREE_ITEM_TYPE_FD == ItemType )
	{
		bUpdateStats = true;	
		AOIDataCollect.SetModelAttachedObj(MODEL_ATTACHED_FD);
		BuildInfoPropCtrl_Fd(m_wndInfomationPropCtrl, pFd);
	}
	else if ( TREE_ITEM_TYPE_MARK == ItemType )
	{
		bUpdateStats = true;
		AOIDataCollect.SetModelAttachedObj(MODEL_ATTACHED_MARK);
		BuildInfoPropCtrl_Mark(m_wndInfomationPropCtrl, pMark);
	}
	else if ( TREE_ITEM_TYPE_BARCODE == ItemType )
	{
		bUpdateStats = true;		
		AOIDataCollect.SetModelAttachedObj(MODEL_ATTACHED_BARCODE);
		BuildInfoPropCtrl_Barcode(m_wndInfomationPropCtrl, pBarcode);
	}
	else if ( TREE_ITEM_TYPE_BOARD == ItemType )
	{
		bUpdateStats = true;
		BuildInfoPropCtrl_Board(m_wndInfomationPropCtrl, pBoard);		
	}
	else if ( TREE_ITEM_TYPE_PANEL == ItemType )
	{
		bUpdateStats = true;		
		BuildInfoPropCtrl_Panel(m_wndInfomationPropCtrl, pPanel);		
	}
	else if ( TREE_ITEM_TYPE_PROJECT == ItemType || 
		TREE_ITEM_TYPE_PROJECT_FILENAME == ItemType ||
		TREE_ITEM_TYPE_PROJECT_MODULE == ItemType ||		
		TREE_ITEM_TYPE_PROJECT_REGION_W == ItemType ||
		TREE_ITEM_TYPE_PROJECT_REGION_H == ItemType ||
		TREE_ITEM_TYPE_PROJECT_FD_RANGE_W == ItemType ||
		TREE_ITEM_TYPE_PROJECT_FD_RANGE_H == ItemType ||
		TREE_ITEM_TYPE_PROJECT_BARCODE_MODE == ItemType	)
	{
		bUpdateStats = true;		
		BuildInfoPropCtrl_Project(m_wndInfomationPropCtrl, ProjectPtr);	
	}	
	else
	{
		bUpdateStats = false;		
		ClearInfoPropCtrl();		
	}
	//bUpdateStats = false;
	if ( true==bUpdateStats || true==MultiSelectMode )//延後更新, 否則會被預設狀態給覆蓋
	{	CWnd::PostMessage(MSG_EDIT_PART_LIST_WND, WPARAM_UPDATE_PART_STATES, NULL);	}
	//AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_PART_SELECTED, NULL);	
	if ( false == RBtn )
	{
		UINT ViewWndID = AOIDataCollect.GetMainViewWndID();	
		if ( AOIDataCollect.CheckOnlineFormViewID(ViewWndID) == false )
		{		
			AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_PART_SELECTED, NULL);
			AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_WND_PROPERTY_WND, WPARAM_BUILD_WND_SELECTED, NULL);
			//AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_UPDATE, (LPARAM)(this));
		}
		if ( true == m_MoveToComponent )
		{	AOIDataCollect.MoveStageToComponentOrField(pComponent, false);	 }
	}
	return true;
}
//--------------------------------------------------------------------------------//
BOOL CEditComponentListDockPane::ClearInfoPropCtrl()
{
	m_wndInfomationPropCtrl.SetRedraw(FALSE);
	ClearInfoPropCtrl(m_wndInfomationPropCtrl);
	m_wndInfomationPropCtrl.SetRedraw(TRUE);
	m_wndInfomationPropCtrl.RedrawWindow();
	return TRUE;
}
//-------------------------------------------------------------------------------------//
BOOL CEditComponentListDockPane::ClearInfoPropCtrl(CJETPropertyGridCtrl &wndPropCtrl)
{	
	wndPropCtrl.RemoveAll();	
	return TRUE;
}
//-------------------------------------------------------------------------------------//
BOOL CEditComponentListDockPane::BuildInfoPropCtrl_Project(CJETPropertyGridCtrl &wndPropCtrl, CAOIProject *ProjectPtr)
{
	ClearInfoPropCtrl(wndPropCtrl);
	if ( NULL == ProjectPtr ) { return TRUE; }

	bool         bEnabled=false;	
	unsigned int BarcodeDeviceIndex = 0;
	unsigned int BarcodeDeviceCodeIndex = 0;
	CString      str, strCaption, strValue, strDescr;
	CJETPropertyGridProperty *pProp = NULL;	
	CJETPropertyGridProperty *pCategory = NULL;	
	//CJETPropertyGridProperty *pCategory = NULL;	
	const size_t MarkCount = ProjectPtr->GetProjectMarkCount();
	const size_t PanelCount = ProjectPtr->GetProjectPanelCount();
	const size_t BoardCount = ProjectPtr->GetProjectBoardCount();
	const size_t BarcodeCount = ProjectPtr->GetProjectBarcodeCount();	
	const size_t ComponentCount = ProjectPtr->GetProjectComponentCount();
	const size_t ComponentBypassCount = ProjectPtr->GetProjectComponentBypassCount();
	const size_t FieldCount = ProjectPtr->GetProjectInspectionFieldCount();
	TProjectParameter &Param = ProjectPtr->GetProjectParameter();

	const BOOL bRedraw = FALSE;
	const BOOL bAdjustLayou = FALSE;

	wndPropCtrl.SetRedraw(FALSE);	
/*
	strCaption = _T("File");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	str = ProjectPtr->GetProjectShowName();
	JetAPI::ExtractMainFileNameNoPath(str, strValue);
	pProp = new CJETPropertyGridProperty(strCaption, strValue);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_PROJECT_FILENAME);
	pProp->SetData((DWORD_PTR)ProjectPtr);
	pProp->Enable(FALSE);
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);
	*/
	strCaption = _T("Module");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue = Param.m_ProjectModuleName.c_str();
	pProp = new CJETPropertyGridProperty(strCaption, strValue);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_PROJECT_MODULE);
	pProp->SetData((DWORD_PTR)ProjectPtr);	
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);

	strCaption = _T("Version");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue = Param.m_ProjectVersion.c_str();
	pProp = new CJETPropertyGridProperty(strCaption, strValue);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_PROJECT_VERSION);
	pProp->SetData((DWORD_PTR)ProjectPtr);	
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);
	/*
	strCaption = _T("Side");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue = AOIDataDefine.GetPanelSideModeText(Param.m_ProjectPanelSideMode);
	pProp = new CJETPropertyGridProperty(strCaption, strValue);
	if ( NULL == pProp ) { return FALSE; }
	pProp->AllowEdit(FALSE);
	pProp->SetID(INFO_PROJECT_PANEL_SIDE);
	pProp->SetData((DWORD_PTR)ProjectPtr);		
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);	
	*/
	strCaption = AOIDataDefine.GetWidthText();
	strValue.Format(_T("%.2f"), Param.m_TestSizeWidth);
	pProp = new CJETPropertyGridProperty(strCaption, strValue);
	if ( NULL == pProp ) { return FALSE; }	
	pProp->SetID(INFO_PROJECT_TEST_WIDTH);
	pProp->SetData((DWORD_PTR)ProjectPtr);		
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);

	strCaption = AOIDataDefine.GetHeightText();
	strValue.Format(_T("%.2f"), Param.m_TestSizeHeight);
	pProp = new CJETPropertyGridProperty(strCaption, strValue);
	if ( NULL == pProp ) { return FALSE; }	
	pProp->SetID(INFO_PROJECT_TEST_HEIGHT);
	pProp->SetData((DWORD_PTR)ProjectPtr);		
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);	

	strCaption = AOIDataDefine.GetBarcodeText();
	strValue = ProjectPtr->GetProjectBarcode();
	pProp = new CJETPropertyGridProperty(strCaption, strValue);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_PROJECT_BARCODE);
	pProp->SetData((DWORD_PTR)ProjectPtr);	
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);	

	strCaption = _T("Panel Count");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%d"), PanelCount);
	pProp = new CJETPropertyGridProperty(strCaption, strValue);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_PROJECT_PANEL_COUNT);
	pProp->SetData((DWORD_PTR)ProjectPtr);
	pProp->Enable(FALSE);
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);

	strCaption = _T("Board Count");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%d"), BoardCount);
	pProp = new CJETPropertyGridProperty(strCaption, strValue);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_PROJECT_BOARD_COUNT);
	pProp->SetData((DWORD_PTR)ProjectPtr);
	pProp->Enable(FALSE);
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);

	strCaption = _T("Mark Count");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%d"), MarkCount);
	pProp = new CJETPropertyGridProperty(strCaption, strValue);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_PROJECT_MARK_COUNT);
	pProp->SetData((DWORD_PTR)ProjectPtr);
	pProp->AllowEdit(FALSE);
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);		

	strCaption = _T("Barcode Count");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%d"), BarcodeCount);
	pProp = new CJETPropertyGridProperty(strCaption, strValue);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_PROJECT_BARCODE_COUNT);
	pProp->SetData((DWORD_PTR)ProjectPtr);
	pProp->AllowEdit(FALSE);
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);	

	strCaption = _T("Component Count");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%d"), ComponentCount);
	pProp = new CJETPropertyGridProperty(strCaption, strValue);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_PROJECT_COMPONENT_COUNT);
	pProp->SetData((DWORD_PTR)ProjectPtr);
	pProp->AllowEdit(FALSE);
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);			
	
	strCaption = _T("Component Bypass Count");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%d"), ComponentBypassCount);
	pProp = new CJETPropertyGridProperty(strCaption, strValue);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_PROJECT_COMPONENT_BYPASS_COUNT);
	pProp->SetData((DWORD_PTR)ProjectPtr);
	pProp->AllowEdit(FALSE);
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);		
	
	strCaption = _T("Field Count");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%d"), FieldCount);
	pProp = new CJETPropertyGridProperty(strCaption, strValue);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_PROJECT_FIELD_COUNT);
	pProp->SetData((DWORD_PTR)ProjectPtr);
	pProp->AllowEdit(FALSE);
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);		

	strDescr = _T("");
	strCaption = _T("Barcode Device");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	BarcodeDeviceIndex = ProjectPtr->GetProjectBarcodeDeviceIndex();	
	pProp = CreateGridPropertyBarcodeDeviceIndexList(strCaption, BarcodeDeviceIndex, ProjectPtr, strDescr);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_PROJECT_BARCODE_DEVICE_INDEX);
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);	

	strDescr = _T("");
	strCaption = _T("Code Index");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	BarcodeDeviceCodeIndex = ProjectPtr->GetProjectBarcodeDeviceCodeIndex();	
	pProp = CreateGridPropertyBarcodeDeviceCodeIndexList(strCaption, BarcodeDeviceCodeIndex, ProjectPtr, strDescr);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_PROJECT_BARCODE_DEVICE_CODE_INDEX);
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);
	
	/*
	INFO_PROJECT_NODE_BEGIN,		
	INFO_PROJECT_NODE_END,
	*/
	if ( FALSE == bAdjustLayou )
	{	wndPropCtrl.AdjustLayout(); }
	wndPropCtrl.SetRedraw(TRUE);
	return TRUE;
}
//-------------------------------------------------------------------------------------//
BOOL CEditComponentListDockPane::BuildInfoPropCtrl_Panel(CJETPropertyGridCtrl &wndPropCtrl, CAOIPanel *PanelPtr)
{
	ClearInfoPropCtrl(wndPropCtrl);
	if ( NULL == PanelPtr ) { return TRUE; }

	bool         bEnabled=false;	
	unsigned int BarcodeDeviceIndex = 0;
	unsigned int BarcodeDeviceCodeIndex = 0;
	CString      str, strCaption, strValue, strDescr;
	CJETPropertyGridProperty *pProp = NULL;	

	const BOOL bRedraw = FALSE;
	const BOOL bAdjustLayou = FALSE;

	wndPropCtrl.SetRedraw(FALSE);	

	strCaption = _T("Panel Index");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%d"), PanelPtr->GetPanelIndex_Project()+1);
	pProp = new CJETPropertyGridProperty(strCaption, strValue);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_PANEL_PANEL_INDEX);
	pProp->SetData((DWORD_PTR)PanelPtr);
	pProp->Enable(FALSE);
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);

	strCaption = _T("Barcode");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue = PanelPtr->GetPanelBarcode();
	pProp = new CJETPropertyGridProperty(strCaption, strValue);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_PANEL_BARCODE);
	pProp->SetData((DWORD_PTR)PanelPtr);	
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);

	strCaption = _T("Board Count");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%d"), PanelPtr->GetPanelBoardCount());
	pProp = new CJETPropertyGridProperty(strCaption, strValue);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_PANEL_BOARD_COUNT);
	pProp->SetData((DWORD_PTR)PanelPtr);
	pProp->Enable(FALSE);
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);

	strCaption = _T("Component Count");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%d"), PanelPtr->GetPanelComponentCount());
	pProp = new CJETPropertyGridProperty(strCaption, strValue);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_PANEL_COMPONENT_COUNT);
	pProp->SetData((DWORD_PTR)PanelPtr);
	pProp->Enable(FALSE);
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);	

	strCaption = _T("Bypass");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	bEnabled = PanelPtr->GetPanelBypassed();	
	pProp = new CJETPropertyGridProperty(strCaption, (_variant_t)bEnabled, strDescr);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_PANEL_BYPASS);
	pProp->SetData((DWORD_PTR)PanelPtr);	
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);

	strCaption = _T("Barcode Enabled");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	bEnabled = PanelPtr->GetPanelBarcodeEnabled();
	pProp = new CJETPropertyGridProperty(strCaption, (_variant_t)bEnabled, strDescr);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_PANEL_BARCODE_ENABLED);
	pProp->SetData((DWORD_PTR)PanelPtr);	
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);

	strDescr = _T("");
	strCaption = _T("Barcode Device");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	BarcodeDeviceIndex = PanelPtr->GetPanelBarcodeDeviceIndex();	
	pProp = CreateGridPropertyBarcodeDeviceIndexList(strCaption, BarcodeDeviceIndex, PanelPtr, strDescr);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_PANEL_BARCODE_DEVICE_INDEX);
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);	

	strDescr = _T("");
	strCaption = _T("Code Index");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	BarcodeDeviceCodeIndex = PanelPtr->GetPanelBarcodeDeviceCodeIndex();	
	pProp = CreateGridPropertyBarcodeDeviceCodeIndexList(strCaption, BarcodeDeviceCodeIndex, PanelPtr, strDescr);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_PANEL_BARCODE_DEVICE_CODE_INDEX);
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);

	strCaption = _T("Board Row Count");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%d"), PanelPtr->GetPanelBoardRowCount());
	pProp = new CJETPropertyGridProperty(strCaption, strValue);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_PANEL_BOARD_ROW_COUNT);
	pProp->SetData((DWORD_PTR)PanelPtr);	
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);

	strCaption = _T("Board Col Count");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%d"), PanelPtr->GetPanelBoardColCount());
	pProp = new CJETPropertyGridProperty(strCaption, strValue);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_PANEL_BOARD_COL_COUNT);
	pProp->SetData((DWORD_PTR)PanelPtr);	
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);

	strCaption = _T("Board Col Block Count");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%d"), PanelPtr->GetPanelBoardColBlockCount());
	pProp = new CJETPropertyGridProperty(strCaption, strValue);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_PANEL_BOARD_COL_BLOCK_COUNT);
	pProp->SetData((DWORD_PTR)PanelPtr);	
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);

	if ( FALSE == bAdjustLayou )
	{	wndPropCtrl.AdjustLayout(); }
	wndPropCtrl.SetRedraw(TRUE);
	return TRUE;
}
//-------------------------------------------------------------------------------------//
BOOL CEditComponentListDockPane::BuildInfoPropCtrl_Board(CJETPropertyGridCtrl &wndPropCtrl, CAOIBoard *BoardPtr)
{
	ClearInfoPropCtrl(wndPropCtrl);
	if ( NULL == BoardPtr ) { return TRUE; }

	bool         bEnabled=false;	
	unsigned int BarcodeDeviceIndex = 0;
	unsigned int BarcodeDeviceCodeIndex = 0;
	CString      str, strCaption, strValue, strDescr;
	CJETPropertyGridProperty *pProp = NULL;	

	const BOOL bRedraw = FALSE;
	const BOOL bAdjustLayou = FALSE;

	wndPropCtrl.SetRedraw(FALSE);	

	strCaption = _T("Panel Index");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%d"), BoardPtr->GetBoardPanelIndex_Project()+1);
	pProp = new CJETPropertyGridProperty(strCaption, strValue);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_BOARD_PANEL_INDEX);
	pProp->SetData((DWORD_PTR)BoardPtr);
	pProp->Enable(FALSE);
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);

	strCaption = _T("Board Index");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%d"), BoardPtr->GetBoardIndex_Panel()+1);
	pProp = new CJETPropertyGridProperty(strCaption, strValue);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_BOARD_BOARD_INDEX);
	pProp->SetData((DWORD_PTR)BoardPtr);
	pProp->Enable(FALSE);
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);

	strCaption = _T("TB Side");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);		
	strValue = AOIDataDefine.GetBoardSideModeText(BoardPtr->GetBoardSideMode());
	pProp = new CJETPropertyGridProperty(strCaption, strValue);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_BOARD_BOARD_SIDE_MODE);
	pProp->SetData((DWORD_PTR)BoardPtr);	
	pProp->AddOption(AOIDataDefine.GetBoardSideModeText(BOARD_SIDE_TOP));
	pProp->AddOption(AOIDataDefine.GetBoardSideModeText(BOARD_SIDE_BOT));	
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);

	strCaption = _T("Barcode");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue = BoardPtr->GetBoardBarcode();
	pProp = new CJETPropertyGridProperty(strCaption, strValue);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_BOARD_BARCODE);
	pProp->SetData((DWORD_PTR)BoardPtr);	
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);

	strCaption = _T("Component Count");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%d"), BoardPtr->GetBoardComponentCount());
	pProp = new CJETPropertyGridProperty(strCaption, strValue);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_BOARD_COMPONENT_COUNT);
	pProp->SetData((DWORD_PTR)BoardPtr);
	pProp->Enable(FALSE);
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);

	strCaption = _T("Bypass");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	bEnabled = BoardPtr->GetBoardBypassed();	
	pProp = new CJETPropertyGridProperty(strCaption, (_variant_t)bEnabled, strDescr);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_BOARD_BYPASS);
	pProp->SetData((DWORD_PTR)BoardPtr);	
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);

	strCaption = _T("Fd Enable");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	bEnabled = BoardPtr->GetBoardMapEnable();
	pProp = new CJETPropertyGridProperty(strCaption, (_variant_t)bEnabled, strDescr);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_BOARD_FD_ENABLE);
	pProp->SetData((DWORD_PTR)BoardPtr);	
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);

	strCaption = _T("Barcode Enabled");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	bEnabled = BoardPtr->GetBoardBarcodeEnabled();
	pProp = new CJETPropertyGridProperty(strCaption, (_variant_t)bEnabled, strDescr);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_BOARD_BARCODE_ENABLED);
	pProp->SetData((DWORD_PTR)BoardPtr);	
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);	

	strDescr = _T("");
	strCaption = _T("Barcode Device");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	BarcodeDeviceIndex = BoardPtr->GetBoardBarcodeDeviceIndex();	
	pProp = CreateGridPropertyBarcodeDeviceIndexList(strCaption, BarcodeDeviceIndex, BoardPtr, strDescr);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_BOARD_BARCODE_DEVICE_INDEX);
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);	

	strDescr = _T("");
	strCaption = _T("Code Index");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	BarcodeDeviceCodeIndex = BoardPtr->GetBoardBarcodeDeviceCodeIndex();	
	pProp = CreateGridPropertyBarcodeDeviceCodeIndexList(strCaption, BarcodeDeviceCodeIndex, BoardPtr, strDescr);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_BOARD_BARCODE_DEVICE_CODE_INDEX);
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);	

	if ( FALSE == bAdjustLayou )
	{	wndPropCtrl.AdjustLayout(); }
	wndPropCtrl.SetRedraw(TRUE);
	return TRUE;
}
//-------------------------------------------------------------------------------------//
BOOL CEditComponentListDockPane::BuildInfoPropCtrl_Fd(CJETPropertyGridCtrl &wndPropCtrl, CAOIFd *FdPtr)
{
	ClearInfoPropCtrl(wndPropCtrl);
	if ( NULL == FdPtr ) { return TRUE; }

	bool         bEnabled=false;
	//BOOL         bEnabled=TRUE;
	CString      str, strCaption, strValue, strDescr;
	CJETPropertyGridProperty *pProp = NULL;	
	CJETPropertyGridProperty *pCategory = NULL;	
	//CJETPropertyGridProperty *pCategory = NULL;	
	
	const BOOL bRedraw = FALSE;
	const BOOL bAdjustLayou = FALSE;

	wndPropCtrl.SetRedraw(FALSE);	

	strCaption = _T("Name");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue = _T("Fiducial");
	strValue = LoadMultiLanguageString(strValue, strValue);	
	pProp = new CJETPropertyGridProperty(strCaption, strValue);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_FD_NAME);
	pProp->SetData((DWORD_PTR)FdPtr);
	pProp->Enable(FALSE);
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);

	strCaption = _T("Unique ID");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);		
	strValue.Format(_T("%d"), FdPtr->GetFdUniqueID()+1);
	pProp = new CJETPropertyGridProperty(strCaption, strValue);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_FD_UNIQUE_ID);
	pProp->SetData((DWORD_PTR)FdPtr);	
	pProp->AllowEdit(FALSE);
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);

	strCaption = _T("Cad Pos");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("(%.0f, %.0f)"), FdPtr->GetFdCadPosX(), FdPtr->GetFdCadPosY());
	pProp = new CJETPropertyGridProperty(strCaption, strValue);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_FD_CAD_XY);
	pProp->SetData((DWORD_PTR)FdPtr);
	pProp->AllowEdit(FALSE);
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);

	strCaption = _T("Stage Pos");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("(%.0f, %.0f)"), FdPtr->GetFdStagePosX(), FdPtr->GetFdStagePosY());
	pProp = new CJETPropertyGridProperty(strCaption, strValue);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_FD_STAGE_XY);
	pProp->SetData((DWORD_PTR)FdPtr);
	pProp->AllowEdit(FALSE);
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);	

	strCaption = _T("Teach Pos");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("(%.0f, %.0f)"), FdPtr->GetFdTeachStagePosX(), FdPtr->GetFdTeachStagePosY());
	pProp = new CJETPropertyGridProperty(strCaption, strValue);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_FD_TEACH_XY);
	pProp->SetData((DWORD_PTR)FdPtr);
	pProp->AllowEdit(FALSE);
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);	

	strCaption = _T("Group ID");
	strCaption = AOIDataDefine.GetGroupIDText();
	strValue.Format(_T("%d"), FdPtr->GetFdGroupID()+1);
	pProp = new CJETPropertyGridProperty(strCaption, strValue);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_FD_GROUP_ID);
	pProp->SetData((DWORD_PTR)FdPtr);	
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);

	strCaption = _T("Panel");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%d"), FdPtr->GetFdPanelIndex_Project()+1);
	pProp = new CJETPropertyGridProperty(strCaption, strValue);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_FD_PANEL_INDEX);
	pProp->SetData((DWORD_PTR)FdPtr);
	pProp->AllowEdit(FALSE);
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);

	strCaption = _T("Board");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%d"), FdPtr->GetFdBoardIndex_Panel()+1);
	pProp = new CJETPropertyGridProperty(strCaption, strValue);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_FD_BOARD_INDEX);
	pProp->SetData((DWORD_PTR)FdPtr);
	pProp->AllowEdit(FALSE);
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);

	strCaption = _T("Sort ID");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%d"), FdPtr->GetFdSortID());
	pProp = new CJETPropertyGridProperty(strCaption, strValue);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_FD_SORT_ID);
	pProp->SetData((DWORD_PTR)FdPtr);
	//pProp->Enable(FALSE);
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);

	strCaption = _T("Local Base Plane ID");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%d"), FdPtr->GetFdLocalBasePlaneID());
	pProp = new CJETPropertyGridProperty(strCaption, strValue);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_FD_LOCAL_BASE_PLANE_ID);
	pProp->SetData((DWORD_PTR)FdPtr);	
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);	

	//INFO_FD_NODE_BEGIN,		
	//INFO_FD_NODE_END,	
	if ( FALSE == bAdjustLayou )
	{	wndPropCtrl.AdjustLayout(); }
	wndPropCtrl.SetRedraw(TRUE);	
	return TRUE;
}
//-------------------------------------------------------------------------------------//
BOOL CEditComponentListDockPane::BuildInfoPropCtrl_Mark(CJETPropertyGridCtrl &wndPropCtrl, CAOIMark *MarkPtr)
{
	ClearInfoPropCtrl(wndPropCtrl);
	if ( NULL == MarkPtr ) { return TRUE; }

	bool         bEnabled=false;
	//BOOL         bEnabled=TRUE;
	CString      str, strCaption, strValue, strDescr;
	CJETPropertyGridProperty *pProp = NULL;	
	CJETPropertyGridProperty *pCategory = NULL;	
	//CJETPropertyGridProperty *pCategory = NULL;	
	
	const BOOL bRedraw = FALSE;
	const BOOL bAdjustLayou = FALSE;

	wndPropCtrl.SetRedraw(FALSE);	

	strCaption = _T("Name");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue = _T("Mark");
	strValue = LoadMultiLanguageString(strValue, strValue);	
	pProp = new CJETPropertyGridProperty(strCaption, strValue);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_MARK_NAME);
	pProp->SetData((DWORD_PTR)MarkPtr);
	pProp->Enable(FALSE);
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);

	strCaption = _T("Unique ID");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);		
	strValue.Format(_T("%d"), MarkPtr->GetMarkUniqueID()+1);
	pProp = new CJETPropertyGridProperty(strCaption, strValue);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_MARK_UNIQUE_ID);
	pProp->SetData((DWORD_PTR)MarkPtr);	
	pProp->AllowEdit(FALSE);
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);

	strCaption = _T("Group ID");
	strCaption = AOIDataDefine.GetGroupIDText();
	strValue.Format(_T("%d"), MarkPtr->GetMarkGroupID()+1);
	pProp = new CJETPropertyGridProperty(strCaption, strValue);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_MARK_GROUP_ID);
	pProp->SetData((DWORD_PTR)MarkPtr);	
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);
	
	strCaption = _T("Bypass");
	strCaption = AOIDataDefine.GetBypassedText();	
	bEnabled = MarkPtr->GetMarkBypassed();
	pProp = new CJETPropertyGridProperty(strCaption, (_variant_t)bEnabled, strDescr);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_MARK_BYPASSED);
	pProp->SetData((DWORD_PTR)MarkPtr);	
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);

	strCaption = _T("Panel");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%d"), MarkPtr->GetMarkPanelIndex_Project()+1);
	pProp = new CJETPropertyGridProperty(strCaption, strValue);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_MARK_PANEL_INDEX);
	pProp->SetData((DWORD_PTR)MarkPtr);
	pProp->AllowEdit(FALSE);
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);

	strCaption = _T("Board");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%d"), MarkPtr->GetMarkBoardIndex_Panel()+1);
	pProp = new CJETPropertyGridProperty(strCaption, strValue);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_MARK_BOARD_INDEX);
	pProp->SetData((DWORD_PTR)MarkPtr);
	pProp->AllowEdit(FALSE);
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);
	
	strCaption = _T("Local Base Plane ID");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%d"), MarkPtr->GetMarkLocalBasePlaneID());
	pProp = new CJETPropertyGridProperty(strCaption, strValue);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_MARK_LOCAL_BASE_PLANE_ID);
	pProp->SetData((DWORD_PTR)MarkPtr);	
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);		

	strCaption = _T("Local Base Plane Z");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.2f"), MarkPtr->GetMarkSpaceBasePlaneParam().NormalZ);
	pProp = new CJETPropertyGridProperty(strCaption, strValue);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_MARK_LOCAL_PLANE_NORMAL_Z);
	pProp->SetData((DWORD_PTR)MarkPtr);	
	pProp->AllowEdit(FALSE);
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);		
	
	//INFO_MARK_NODE_BEGIN,		
	//INFO_MARK_NODE_END,	
	if ( FALSE == bAdjustLayou )
	{	wndPropCtrl.AdjustLayout(); }
	wndPropCtrl.SetRedraw(TRUE);	
	return TRUE;
}
//-------------------------------------------------------------------------------------//
BOOL CEditComponentListDockPane::BuildInfoPropCtrl_Component(CJETPropertyGridCtrl &wndPropCtrl, CAOIComponent *ComponentPtr)
{
	ClearInfoPropCtrl(wndPropCtrl);
	if ( NULL == ComponentPtr ) { return TRUE; }	

	int          i=0;
	int          ModelClassID=0;
	bool         bEnabled=false;	
	bool         ModelIsolated=false;	
	CString      str, strCaption, strValue, strDescr;	
	CJETPropertyGridProperty *pProp = NULL;	
	CJETPropertyGridProperty *pCategory = NULL;	
	SAVE_TEST_IMAGE_MODE      SaveTestImageMode;
	//CJETPropertyGridProperty *pCategory = NULL;	
	DRAW_MODEL_MODE DrawModelMode = AOIDataCollect.GetDrawModelMode();
	
	const BOOL bRedraw = FALSE;
	const BOOL bAdjustLayou = FALSE;

	wndPropCtrl.SetRedraw(FALSE);	

	strCaption = _T("Component Name");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue = ComponentPtr->GetComponentName();
	pProp = new CJETPropertyGridProperty(strCaption, strValue);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_COMPONENT_NAME);
	pProp->SetData((DWORD_PTR)ComponentPtr);
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);

	strCaption = _T("Cad Pos");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("(%.0f, %.0f)"), ComponentPtr->GetComponentCadPosX(), ComponentPtr->GetComponentCadPosY());
	pProp = new CJETPropertyGridProperty(strCaption, strValue);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_COMPONENT_CAD_XY);
	pProp->SetData((DWORD_PTR)ComponentPtr);
	pProp->Enable(FALSE);
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);
	
	
	double StagePosX=0.0, StagePosY=0.0;	
	if ( DRAW_MODEL_RESULT == DrawModelMode )
	{
		strCaption = _T("Stage Res");
		StagePosX = ComponentPtr->GetComponentStageResultX();
		StagePosY = ComponentPtr->GetComponentStageResultY();
	}
	else
	{
		strCaption = _T("Stage Pos");
		StagePosX = ComponentPtr->GetComponentStagePosX();
		StagePosY = ComponentPtr->GetComponentStagePosY();
	}
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("(%.0f, %.0f)"), StagePosX, StagePosY);
	pProp = new CJETPropertyGridProperty(strCaption, strValue);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_COMPONENT_STAGE_XY);
	pProp->SetData((DWORD_PTR)ComponentPtr);
	pProp->Enable(FALSE);
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);	

	strCaption = _T("Angle");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%.2f"), ComponentPtr->GetComponentAngle());
	pProp = new CJETPropertyGridProperty(strCaption, strValue);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_COMPONENT_ANGLE);
	pProp->SetData((DWORD_PTR)ComponentPtr);
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);

	strCaption = _T("Part Number");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue = ComponentPtr->GetComponentPartNumber();
	pProp = new CJETPropertyGridProperty(strCaption, strValue);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_COMPONENT_PART_NUMBER);
	pProp->SetData((DWORD_PTR)ComponentPtr);
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);

	strCaption = _T("Model Number");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue = ComponentPtr->GetComponentModelName();
	pProp = new CJETPropertyGridProperty(strCaption, strValue);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_COMPONENT_MODEL_NAME);
	pProp->SetData((DWORD_PTR)ComponentPtr);
	pProp->Enable(FALSE);
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);
	
	strCaption = _T("X Index");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%d"), ComponentPtr->GetComponentColIndex()+1);
	pProp = new CJETPropertyGridProperty(strCaption, strValue);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_COMPONENT_COL_INDEX);
	pProp->SetData((DWORD_PTR)ComponentPtr);	
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);

	strCaption = _T("Y Index");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%d"), ComponentPtr->GetComponentRowIndex()+1);
	pProp = new CJETPropertyGridProperty(strCaption, strValue);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_COMPONENT_ROW_INDEX);
	pProp->SetData((DWORD_PTR)ComponentPtr);	
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);

	strCaption = _T("Bypass");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	bEnabled = ComponentPtr->GetComponentBypassed();
	pProp = new CJETPropertyGridProperty(strCaption, (_variant_t)bEnabled, strDescr);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_COMPONENT_BYPASS);
	pProp->SetData((DWORD_PTR)ComponentPtr);	
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);

	strCaption = _T("Bypass 3D");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	bEnabled = ComponentPtr->GetComponentBypass3D();
	pProp = new CJETPropertyGridProperty(strCaption, (_variant_t)bEnabled, strDescr);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_COMPONENT_BYPASS3D);
	pProp->SetData((DWORD_PTR)ComponentPtr);	
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);	
	
	strCaption = _T("Save Image");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);		
	SaveTestImageMode = ComponentPtr->GetComponentSaveTestImageMode();
	pProp = CreateGridPropertySaveTestImageModeList(strCaption, SaveTestImageMode, ComponentPtr, strDescr);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_COMPONENT_SAVE_IMAGE);
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);

	strCaption = _T("Bad Mark");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	bEnabled = ComponentPtr->GetComponentXBoardUnit();
	pProp = new CJETPropertyGridProperty(strCaption, (_variant_t)bEnabled, strDescr);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_COMPONENT_BAD_MARK);
	pProp->SetData((DWORD_PTR)ComponentPtr);	
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);

	strCaption = _T("Offset");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("(%.0f, %.0f)"), ComponentPtr->GetComponentResultOffsetX(), ComponentPtr->GetComponentResultOffsetY());
	pProp = new CJETPropertyGridProperty(strCaption, strValue, strDescr);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_COMPONENT_OFFSET_XY);
	pProp->SetData((DWORD_PTR)ComponentPtr);	
	pProp->AllowEdit(FALSE);
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);

	strCaption = _T("Skew");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%.2f"), ComponentPtr->GetComponentResultSkewAngle());
	pProp = new CJETPropertyGridProperty(strCaption, strValue);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_COMPONENT_SKEW_ANGLE);
	pProp->SetData((DWORD_PTR)ComponentPtr);
	pProp->AllowEdit(FALSE);
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);	

#ifdef MULTI_CLASS_USE
	strCaption = _T("Model Class ID");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	ModelClassID = ComponentPtr->GetComponentModelClassID();
	if ( ModelClassID == MODEL_CLASS_ID_NONE )
	{	strValue = AOIDataDefine.GetDisableText(); }
	else
	{	strValue.Format(_T("%02d"), ModelClassID); }
	pProp = new CJETPropertyGridProperty(strCaption, strValue, strDescr);	
	if ( NULL == pProp ) { return FALSE; }
	str = AOIDataDefine.GetDisableText(); pProp->AddOption(str);
	for ( i=0; i<MODEL_CLASS_ID_COUNT; i++ )
	{	str.Format(_T("%02d"), i);	pProp->AddOption(str);	}
	pProp->SetID(INFO_COMPONENT_MODEL_CLASS_ID);
	pProp->SetData((DWORD_PTR)ComponentPtr);	
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);
#endif//MULTI_CLASS_USE

	strCaption = _T("Model Isolated");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	bEnabled = ComponentPtr->GetComponentModelIsolated();
	pProp = new CJETPropertyGridProperty(strCaption, (_variant_t)bEnabled, strDescr);	
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_COMPONENT_MODEL_ISOLATED);
	pProp->SetData((DWORD_PTR)ComponentPtr);
	//pProp->Enable(FALSE);
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);

	strCaption = _T("Self Field");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	bEnabled = ComponentPtr->GetComponentSelfFieldEnabled();
	pProp = new CJETPropertyGridProperty(strCaption, (_variant_t)bEnabled, strDescr);	
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_COMPONENT_SELF_FIELD);
	pProp->SetData((DWORD_PTR)ComponentPtr);
	pProp->Enable(FALSE);
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);

	strCaption = _T("Local Base Plane ID");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%d"), ComponentPtr->GetComponentLocalBasePlaneID());
	pProp = new CJETPropertyGridProperty(strCaption, strValue);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_COMPONENT_LOCAL_BASE_PLANE_ID);
	pProp->SetData((DWORD_PTR)ComponentPtr);	
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);		

	strCaption = _T("Save Report(ARS)");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	bEnabled = ComponentPtr->GetComponentSaveReport_ARS();
	pProp = new CJETPropertyGridProperty(strCaption, (_variant_t)bEnabled, strDescr);	
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_COMPONENT_SAVE_REPORT_ARS);
	pProp->SetData((DWORD_PTR)ComponentPtr);	
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);

	if (true == AOIDataCollect.GetHASI_Enable()) {
		//爐前功能沒驗證
		//strCaption = _T("HAS I Enable");
		//strCaption = LoadMultiLanguageString(strCaption, strCaption);
		//bool bEnabled = ComponentPtr->GetComponentHASI_SPIOffset_Enable();

		//pProp = new CJETPropertyGridProperty(strCaption, (_variant_t)bEnabled, strDescr);
		//if (NULL == pProp) { return FALSE; }
		//pProp->SetID(INFO_COMPONENT_M2M_SCO_USE);
		//pProp->SetData((DWORD_PTR)ComponentPtr);
		//wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);

		strCaption = _T("Save Image (HAS I)");
		strCaption = LoadMultiLanguageString(strCaption, strCaption);
		//bEnabled = ComponentPtr->GetComponentModelIsolated();
		bEnabled = ComponentPtr->GetComponentHASI_SaveImage();
		pProp = new CJETPropertyGridProperty(strCaption, (_variant_t)bEnabled, strDescr);
		if (NULL == pProp) { return FALSE; }
		pProp->SetID(INFO_COMPONENT_M2M_SAVE_IMAGE);
		pProp->SetData((DWORD_PTR)ComponentPtr);
		wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);
	}

	if ( FALSE == bAdjustLayou )
	{	wndPropCtrl.AdjustLayout(); }
	wndPropCtrl.SetRedraw(TRUE);	
	return TRUE;
}
//-------------------------------------------------------------------------------------//
BOOL CEditComponentListDockPane::BuildInfoPropCtrl_Barcode(CJETPropertyGridCtrl &wndPropCtrl, CAOIBarcode *BarcodePtr)
{
	ClearInfoPropCtrl(wndPropCtrl);
	if ( NULL == BarcodePtr ) { return TRUE; }

	bool         bEnabled=false;
	//BOOL         bEnabled=TRUE;
	CString      str, strCaption, strValue, strDescr;
	CJETPropertyGridProperty *pProp = NULL;	
	CJETPropertyGridProperty *pCategory = NULL;	
	BARCODE_SPREAD_MODE       BarcodeSpreadMode;
	BARCODE_BELONG_MODE       BarcodeBelongMode;
	SAVE_TEST_IMAGE_MODE      SaveTestImageMode;	
	//CJETPropertyGridProperty *pCategory = NULL;	
	const double BarcodeAngle = BarcodePtr->GetBarcodeAngle();
	const unsigned int PanelIndex = BarcodePtr->GetBarcodePanelIndex_Project();
	const unsigned int BoardIndex = BarcodePtr->GetBarcodeBoardIndex_Panel();
	
	const BOOL bRedraw = FALSE;
	const BOOL bAdjustLayou = FALSE;

	wndPropCtrl.SetRedraw(FALSE);	

	strCaption = _T("Name");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue = _T("Barcode");
	strValue = LoadMultiLanguageString(strValue, strValue);	
	pProp = new CJETPropertyGridProperty(strCaption, strValue);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_BARCODE_NAME);
	pProp->SetData((DWORD_PTR)BarcodePtr);
	pProp->Enable(FALSE);
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);
	
	strCaption = _T("Group ID");
	strCaption = AOIDataDefine.GetGroupIDText();
	strValue.Format(_T("%d"), BarcodePtr->GetBarcodeGroupID()+1);
	pProp = new CJETPropertyGridProperty(strCaption, strValue);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_BARCODE_GROUP_ID);
	pProp->SetData((DWORD_PTR)BarcodePtr);	
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);

	strCaption = _T("Panel");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%d"), PanelIndex+1);
	pProp = new CJETPropertyGridProperty(strCaption, strValue);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_BARCODE_PANEL_INDEX);
	pProp->SetData((DWORD_PTR)BarcodePtr);
	pProp->AllowEdit(FALSE);
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);

	strCaption = _T("Board");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%d"), BoardIndex+1);
	pProp = new CJETPropertyGridProperty(strCaption, strValue);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_BARCODE_BOARD_INDEX);
	pProp->SetData((DWORD_PTR)BarcodePtr);
	pProp->AllowEdit(FALSE);
	if ( -1 == BoardIndex )
	{	pProp->Show(FALSE); }
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);
	
	strCaption = _T("Spread");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	BarcodeSpreadMode = BarcodePtr->GetBarcodeSpreadMode();	
	pProp = CreateGridPropertyBarcodeSpreadModeList(strCaption, BarcodeSpreadMode, BarcodePtr, strDescr);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_BARCODE_SPREAD_MODE);	
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);	

	strCaption = _T("Belong");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);		
	BarcodeBelongMode = BarcodePtr->GetBarcodeBelongMode();
	if ( -1 == BoardIndex )
	{	pProp = CreateGridPropertyBarcodeBelongModeList(strCaption, BarcodeBelongMode, BarcodePtr, strDescr, BARCODE_BELONG_PANEL); }
	else
	{	pProp = CreateGridPropertyBarcodeBelongModeList(strCaption, BarcodeBelongMode, BarcodePtr, strDescr, BARCODE_BELONG_BOARD); }
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_BARCODE_BELONG_MODE);
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);	

	strCaption = _T("Angle");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%.2f"), BarcodeAngle);
	pProp = new CJETPropertyGridProperty(strCaption, strValue);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_BARCODE_ANGLE);
	pProp->SetData((DWORD_PTR)BarcodePtr);
	pProp->AllowEdit(FALSE);
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);	

	strCaption = _T("Result");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue = BarcodePtr->GetBarcodeResultText();
	pProp = new CJETPropertyGridProperty(strCaption, strValue);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_BARCODE_RESULT);
	pProp->SetData((DWORD_PTR)BarcodePtr);		
	pProp->AllowEdit(FALSE);
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);

	strCaption = _T("Save Image");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);		
	SaveTestImageMode = BarcodePtr->GetBarcodeSaveTestImageMode();
	pProp = CreateGridPropertySaveTestImageModeList(strCaption, SaveTestImageMode, BarcodePtr, strDescr);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_BARCODE_SAVE_IMAGE);
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);

	strCaption = _T("Local Base Plane ID");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%d"), BarcodePtr->GetBarcodeLocalBasePlaneID());
	pProp = new CJETPropertyGridProperty(strCaption, strValue);
	if ( NULL == pProp ) { return FALSE; }
	pProp->SetID(INFO_BARCODE_LOCAL_BASE_PLANE_ID);
	pProp->SetData((DWORD_PTR)BarcodePtr);	
	wndPropCtrl.AddProperty(pProp, bRedraw, bAdjustLayou);		

	//INFO_BARCODE_NODE_BEGIN,		
	//INFO_BARCODE_NODE_END,	

	if ( FALSE == bAdjustLayou )
	{	wndPropCtrl.AdjustLayout(); }
	wndPropCtrl.SetRedraw(TRUE);	
	return TRUE;
}
//-------------------------------------------------------------------------------------//
bool CEditComponentListDockPane::RemoveComponentTreeItem(CThisTreeCtrl &TreeCtrl)
{
	CAOIProject *Project = CEditComponentListDockPane::GetActiveProject();
	if ( NULL == Project ) 
	{ 
		m_ProjectPtr = NULL;
		ClearComponentTreeCtrl(TreeCtrl, m_StopComponentTreeBeClick);		
		return true; 
	}

	if ( m_ProjectPtr != Project )
	{	return BuildComponentTreeCtrl(TreeCtrl);	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditComponentListDockPane::ShowComponentTreeSelected(CThisTreeCtrl &TreeCtrl)//讓零件樹狀圖到可以看到的狀態
{
	if ( NULL == this->GetSafeHwnd() ) { return false; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }

	int i=0;
	CString str;
	int          ToLevel = 0;
	DWORD_PTR    ItemData = 0;
	unsigned int ItemType = 0;
	unsigned int ItemIndex = 0;
	HTREEITEM  hFdItem = NULL;
	HTREEITEM  hPanelItem = NULL;
	HTREEITEM  hBoardItem = NULL;
	HTREEITEM  hComponentItem = NULL;
	HTREEITEM  hGroupItem = NULL;
	HTREEITEM  hWindowItem = NULL;
	HTREEITEM  hWndGroupItem = NULL;

	CAOIFd        *pFd = NULL;
	CAOIMark      *pMark = NULL;
	CAOIPanel     *pPanel = NULL;
	CAOIBoard     *pBoard = NULL;
	CAOIBarcode   *pBarcode = NULL;
	CAOIComponent *pComponent = NULL;
	
	bool IsFind = false;
	const bool ShowMark = CheckShowMark();
	const bool ShowFiducial= CheckShowFiducial();
	const bool ShowBarcode = CheckShowBarcode();	
	const bool ShowComponent = CheckShowComponent();

	const unsigned int InvalidIndex = INVALID_INDEX;
	const unsigned int FdIndex = ProjectPtr->GetProjectActiveFdIndex();
	const unsigned int MarkIndex = ProjectPtr->GetProjectActiveMarkIndex();
	const unsigned int PanelIndex = ProjectPtr->GetProjectActivePanelIndex();
	const unsigned int BoardIndex = ProjectPtr->GetProjectActiveBoardIndex();
	const unsigned int BarcodeIndex = ProjectPtr->GetProjectActiveBarcodeIndex();
	const unsigned int ComponentIndex = ProjectPtr->GetProjectActiveComponentIndex();
	const unsigned int WindowIndex = ProjectPtr->GetProjectActiveComponentWindowIndex();
	
	if ( InvalidIndex != PanelIndex )
	{	
		ToLevel = TREE_NODE_PANEL_ID;
		if ( true==ShowFiducial && InvalidIndex!=FdIndex )
		{	ToLevel = TREE_NODE_FD_ID;	}
		else if ( true==ShowBarcode && InvalidIndex!=BarcodeIndex )
		{	ToLevel = TREE_NODE_BARDOE_ID;	}
		else if ( InvalidIndex != BoardIndex )
		{
			ToLevel = TREE_NODE_BOARD_ID;
			if ( true==ShowMark && InvalidIndex!=MarkIndex )
			{	ToLevel = TREE_NODE_MARK_ID;	}
			if ( true==ShowComponent && InvalidIndex!=ComponentIndex )
			{
				ToLevel = TREE_NODE_COMPONENT_ID;
				if ( InvalidIndex != WindowIndex )
				{	ToLevel = TREE_NODE_WINDOW_ID;	}
			}
		}
	}

#ifdef _DEBUG
	CString FdName, PanelName, BoardName, ComName, GroupName, WndGroupName, WindowName;
#endif

	hPanelItem = TreeCtrl.GetRootItem();	
	this->m_StopComponentTreeBeClick = TRUE;	
	//CThisTreeCtrl::SelectItem(NULL);	
	while ( NULL != hPanelItem )
	{
	#ifdef _DEBUG
		PanelName = TreeCtrl.GetItemText(hPanelItem);
	#endif
		ItemData = TreeCtrl.GetItemData(hPanelItem);		
		::DecodeItemlParam(ItemData, ItemType, ItemIndex);		
		if ( TREE_ITEM_TYPE_PANEL!=ItemType || ItemIndex!=PanelIndex )		
		{
			hPanelItem = TreeCtrl.GetNextSiblingItem(hPanelItem);
			continue;
		}			
		
		if ( ToLevel == TREE_NODE_PANEL_ID )
		{
			TreeCtrl.SelectItem(hPanelItem);
			TreeCtrl.SetItemState(hPanelItem, TVIS_SELECTED, TVIS_SELECTED);
			TreeCtrl.Expand(hPanelItem, TVE_EXPAND);
			TreeCtrl.EnsureVisible(hPanelItem);
			this->m_StopComponentTreeBeClick = FALSE;
			IsFind = true;

			pPanel = ProjectPtr->GetProjectPanelPtr(PanelIndex, true);
			BuildInfoPropCtrl_Panel(m_wndInfomationPropCtrl, pPanel);
			return true;
		}

		hBoardItem = TreeCtrl.GetChildItem(hPanelItem);
		while ( NULL!=hBoardItem  )
		{
		#ifdef _DEBUG
			BoardName = TreeCtrl.GetItemText(hBoardItem);
		#endif
			ItemData = TreeCtrl.GetItemData(hBoardItem);		
			::DecodeItemlParam(ItemData, ItemType, ItemIndex);

			if ( TREE_ITEM_TYPE_FD_GROUP==ItemType && (ToLevel==TREE_NODE_FD_ID) )//只選到定位點
			{
				hFdItem = TreeCtrl.GetChildItem(hBoardItem);
				while ( hFdItem!=NULL )
				{
				#ifdef _DEBUG
					FdName = TreeCtrl.GetItemText(hFdItem);
				#endif				
					ItemData = TreeCtrl.GetItemData(hFdItem);		
					::DecodeItemlParam(ItemData, ItemType, ItemIndex);
					if ( TREE_ITEM_TYPE_FD !=ItemType )//只選到定位點
					{
						hFdItem = TreeCtrl.GetNextSiblingItem(hFdItem);
						continue;
					}					
					if ( ItemIndex == FdIndex )
					{
						TreeCtrl.SelectItem(hFdItem);
						TreeCtrl.SetItemState(hFdItem, TVIS_SELECTED, TVIS_SELECTED);
						//TreeCtrl.Expand(hBoardItem, TVE_EXPAND);
						TreeCtrl.EnsureVisible(hFdItem);
						this->m_StopComponentTreeBeClick = FALSE;
						IsFind = true;

						pFd = ProjectPtr->GetProjectFdPtr(FdIndex, true);
						BuildInfoPropCtrl_Fd(m_wndInfomationPropCtrl, pFd);
						return true;
					}
					hFdItem = TreeCtrl.GetNextSiblingItem(hFdItem);
				};
				hBoardItem = TreeCtrl.GetNextSiblingItem(hBoardItem);
				continue;
			}

			if ( (TREE_ITEM_TYPE_BARCODE==ItemType) && (ToLevel==TREE_NODE_BARDOE_ID) )//只選到條碼
			{
				if ( ItemIndex == BarcodeIndex )
				{
					TreeCtrl.SelectItem(hBoardItem);
					TreeCtrl.SetItemState(hBoardItem, TVIS_SELECTED, TVIS_SELECTED);
					//TreeCtrl.Expand(hBoardItem, TVE_EXPAND);
					TreeCtrl.EnsureVisible(hBoardItem);
					this->m_StopComponentTreeBeClick = FALSE;
					IsFind = true;

					pBarcode = ProjectPtr->GetProjectBarcodePtr(BarcodeIndex, true);
					BuildInfoPropCtrl_Barcode(m_wndInfomationPropCtrl, pBarcode);
					return true;
				}
			}

			if ( TREE_ITEM_TYPE_BOARD!=ItemType || ItemIndex!=BoardIndex ) 
			{
				hBoardItem = TreeCtrl.GetNextSiblingItem(hBoardItem);
				continue;
			}
			
			if ( ToLevel == TREE_NODE_BOARD_ID )//只選到單板
			{
				if ( ItemIndex == BoardIndex )
				{
					TreeCtrl.SelectItem(hBoardItem);
					TreeCtrl.SetItemState(hBoardItem, TVIS_SELECTED, TVIS_SELECTED);
					//TreeCtrl.Expand(hBoardItem, TVE_EXPAND);
					TreeCtrl.EnsureVisible(hBoardItem);
					this->m_StopComponentTreeBeClick = FALSE;
					IsFind = true;

					pBoard = ProjectPtr->GetProjectBoardPtr(BoardIndex, true);
					BuildInfoPropCtrl_Board(m_wndInfomationPropCtrl, pBoard);
					return true;
				}
			}

			hComponentItem = TreeCtrl.GetChildItem(hBoardItem);
			while ( hComponentItem!=NULL )
			{
			#ifdef _DEBUG
				ComName = TreeCtrl.GetItemText(hComponentItem);
			#endif				
				ItemData = TreeCtrl.GetItemData(hComponentItem);		
				::DecodeItemlParam(ItemData, ItemType, ItemIndex);

				if ( TREE_ITEM_TYPE_FD_GROUP==ItemType && (ToLevel==TREE_NODE_FD_ID) )//只選到定位點
				{
					hFdItem = TreeCtrl.GetChildItem(hComponentItem);
					while ( hFdItem!=NULL )
					{
					#ifdef _DEBUG
						FdName = TreeCtrl.GetItemText(hFdItem);
					#endif				
						ItemData = TreeCtrl.GetItemData(hFdItem);		
						::DecodeItemlParam(ItemData, ItemType, ItemIndex);
						if ( TREE_ITEM_TYPE_FD !=ItemType )//只選到定位點
						{
							hFdItem = TreeCtrl.GetNextSiblingItem(hFdItem);
							continue;
						}					
						if ( ItemIndex == FdIndex )
						{
							TreeCtrl.SelectItem(hFdItem);
							TreeCtrl.SetItemState(hFdItem, TVIS_SELECTED, TVIS_SELECTED);
							//TreeCtrl.Expand(hBoardItem, TVE_EXPAND);
							TreeCtrl.EnsureVisible(hFdItem);
							this->m_StopComponentTreeBeClick = FALSE;
							IsFind = true;

							pFd = ProjectPtr->GetProjectFdPtr(FdIndex, true);
							BuildInfoPropCtrl_Fd(m_wndInfomationPropCtrl, pFd);
							return true;
						}
						hFdItem = TreeCtrl.GetNextSiblingItem(hFdItem);
					};
					hComponentItem = TreeCtrl.GetNextSiblingItem(hComponentItem);
					continue;
				}				

				if ( (TREE_ITEM_TYPE_BARCODE==ItemType) && (ToLevel==TREE_NODE_BARDOE_ID) )//只選到條碼
				{
					if ( ItemIndex == BarcodeIndex )
					{
						TreeCtrl.SelectItem(hComponentItem);
						TreeCtrl.SetItemState(hComponentItem, TVIS_SELECTED, TVIS_SELECTED);
						//TreeCtrl.Expand(hComponentItem, TVE_EXPAND);
						TreeCtrl.EnsureVisible(hComponentItem);
						this->m_StopComponentTreeBeClick = FALSE;
						IsFind = true;

						pBarcode = ProjectPtr->GetProjectBarcodePtr(BarcodeIndex, true);
						BuildInfoPropCtrl_Barcode(m_wndInfomationPropCtrl, pBarcode);
						return true;
					}
				}
				
				if ( (TREE_ITEM_TYPE_MARK==ItemType) && (ToLevel==TREE_NODE_MARK_ID) )//只選到特徵點
				{
					if ( ItemIndex == MarkIndex )
					{
						TreeCtrl.SelectItem(hComponentItem);
						TreeCtrl.SetItemState(hComponentItem, TVIS_SELECTED, TVIS_SELECTED);
						//TreeCtrl.Expand(hComponentItem, TVE_EXPAND);
						TreeCtrl.EnsureVisible(hComponentItem);
						this->m_StopComponentTreeBeClick = FALSE;
						IsFind = true;

						pMark = ProjectPtr->GetProjectMarkPtr(MarkIndex, true);
						BuildInfoPropCtrl_Mark(m_wndInfomationPropCtrl, pMark);
						return true;
					}
				}

				if ( TREE_ITEM_TYPE_COMPONENT!=ItemType || ItemIndex!=ComponentIndex ) 
				{	
					TreeCtrl.Expand(hComponentItem, TVE_COLLAPSE);
					hComponentItem = TreeCtrl.GetNextSiblingItem(hComponentItem);
					continue; 
				}
				
				if ( ToLevel == TREE_NODE_COMPONENT_ID )//只選到零件
				{
					if ( ItemIndex == ComponentIndex )
					{
						TreeCtrl.SelectItem(hComponentItem);
						TreeCtrl.SetItemState(hComponentItem, TVIS_SELECTED, TVIS_SELECTED);
						//TreeCtrl.Expand(hComponentItem, TVE_EXPAND);
						TreeCtrl.EnsureVisible(hComponentItem);
						this->m_StopComponentTreeBeClick = FALSE;						
						IsFind = true;
						
						pComponent = ProjectPtr->GetProjectComponentPtr(ComponentIndex, true);
						BuildInfoPropCtrl_Component(m_wndInfomationPropCtrl, pComponent);
						return true;						
					}
				}

				if ( ToLevel == TREE_NODE_WINDOW_ID )//選到檢測框
				{
					hGroupItem = TreeCtrl.GetChildItem(hComponentItem);
					while ( hGroupItem != NULL )
					{					
					#ifdef _DEBUG
						GroupName = TreeCtrl.GetItemText(hGroupItem);
					#endif						
						ItemData = TreeCtrl.GetItemData(hGroupItem);		
						::DecodeItemlParam(ItemData, ItemType, ItemIndex);
						if ( ItemType==TREE_ITEM_TYPE_COMPONENT_PACKAGE || 
							 ItemType==TREE_ITEM_TYPE_COMPONENT_PART_NUMBER || 
							 ItemType==TREE_ITEM_TYPE_COMPONENT_POSITION ||
							 ItemType==TREE_ITEM_TYPE_COMPONENT_OFFSET
							)
						{
							hGroupItem = TreeCtrl.GetNextSiblingItem(hGroupItem);
							continue;
						}

						//Collapse Other Group
						hWndGroupItem = TreeCtrl.GetChildItem(hGroupItem);
						while ( hWndGroupItem != NULL )
						{					
							TreeCtrl.Expand(hWndGroupItem, TVE_COLLAPSE);
							hWndGroupItem = TreeCtrl.GetNextSiblingItem(hWndGroupItem);
						};

						hWndGroupItem = TreeCtrl.GetChildItem(hGroupItem);//Wnd Group
						while ( hWndGroupItem != NULL )
						{
						#ifdef _DEBUG
							WndGroupName = TreeCtrl.GetItemText(hWndGroupItem);
						#endif
							hWindowItem = TreeCtrl.GetChildItem(hWndGroupItem);//First Window Item
							while ( hWindowItem!= NULL)
							{
							#ifdef _DEBUG
								WindowName = TreeCtrl.GetItemText(hWindowItem);
							#endif								
								ItemData = TreeCtrl.GetItemData(hWindowItem);		
								::DecodeItemlParam(ItemData, ItemType, ItemIndex);
								if ( TREE_ITEM_TYPE_WINDOW!=ItemType || ItemIndex!=WindowIndex )
								{
									hWindowItem = TreeCtrl.GetNextSiblingItem(hWindowItem);
									continue;
								}

								if ( ItemIndex == WindowIndex )
								{
									TreeCtrl.SelectItem(hWindowItem);
									TreeCtrl.SetItemState(hWindowItem, TVIS_SELECTED, TVIS_SELECTED);								
									TreeCtrl.EnsureVisible(hWindowItem);
									this->m_StopComponentTreeBeClick = FALSE;
									IsFind = true;

									pComponent = ProjectPtr->GetProjectComponentPtr(ComponentIndex, true);
									BuildInfoPropCtrl_Component(m_wndInfomationPropCtrl, pComponent);
									return true;
								}		
								if ( IsFind == true ) { break; }	
								hWindowItem = TreeCtrl.GetNextSiblingItem(hWindowItem);
							};							
							if ( IsFind == true ) { break; }
							hWndGroupItem = TreeCtrl.GetNextSiblingItem(hWndGroupItem);
						};
						if ( IsFind == true ) { break; }
						hGroupItem = TreeCtrl.GetNextSiblingItem(hGroupItem);
					};
				}
				if ( IsFind == true ) { break; }
				hComponentItem = TreeCtrl.GetNextSiblingItem(hComponentItem);
			};
			if ( IsFind == true ) { break; }
			hBoardItem = TreeCtrl.GetNextSiblingItem(hBoardItem);	
		};
		if ( IsFind == true ) { break; }
		hPanelItem = TreeCtrl.GetNextSiblingItem(hPanelItem);	
	};	
	this->m_StopComponentTreeBeClick = FALSE;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditComponentListDockPane::BuildNextComponentTreeCtrl(CThisTreeCtrl &TreeCtrl)
{	
	if ( true == m_PanelTreeNodeListDone ) { return true; }
		
	int LastNodeIdx = 0;	
	bool bAddNewOne=false;
	UINT ItemState = 0;	
	RECT ItemRect={0,0,0,0};
	RECT TreeClientRect={0,0,0,0};	
	TreeCtrl.GetClientRect(&TreeClientRect);
	std::vector<CAOITreeNode> &PanelTreeNodeList=m_PanelTreeNodeList;
	
	bAddNewOne = false;
	m_PanelTreeNodeListDone = true;
	const size_t PanelTreeNodeCount=PanelTreeNodeList.size();

	TreeCtrl.SetRedraw(FALSE);
	m_StopComponentTreeBeClick = TRUE;
	for ( size_t i=0; i<PanelTreeNodeCount; i++ )
	{
		CAOITreeNode &PanelTreeNode=PanelTreeNodeList[i];
		HTREEITEM hPanelItem = PanelTreeNode.GetHItem();
		if ( NULL == hPanelItem ) { continue; }
		ItemState = TreeCtrl.GetItemState(hPanelItem, TVIS_EXPANDED);
		if ( TVIS_EXPANDED != (TVIS_EXPANDED&ItemState) ) 
		{	continue; }

		const size_t BoardTreeNodeCount=PanelTreeNode.GetChildNodeCount();
		for ( size_t j=0; j<BoardTreeNodeCount; j++ )
		{
			CAOITreeNode *BoardTreeNodePtr=PanelTreeNode.GetChildNodePtr(j, false);
			if ( NULL == BoardTreeNodePtr ) { continue; }
			if ( BoardTreeNodePtr->GetChildNodeListDone() == true ) { continue; }
			m_PanelTreeNodeListDone = false;
			HTREEITEM hBoardItem = BoardTreeNodePtr->GetHItem();
			if ( NULL == hBoardItem ) { continue; }
			ItemState = TreeCtrl.GetItemState(hBoardItem, TVIS_EXPANDED);
			if ( TVIS_EXPANDED != (TVIS_EXPANDED&ItemState) ) 
			{	continue; }

			//int PageCount=10;
			int PageCount=2;
			LastNodeIdx = BoardTreeNodePtr->GetLastChildAdded();
			CAOITreeNode *ComponentTreeNodePtr=BoardTreeNodePtr->GetChildNodePtr(LastNodeIdx, true);
			if ( NULL != ComponentTreeNodePtr )
			{
				HTREEITEM hComponentItem = ComponentTreeNodePtr->GetHItem();
				if ( NULL == hComponentItem ) { continue; }
				TreeCtrl.GetItemRect(hComponentItem, &ItemRect, FALSE);
				if ( ItemRect.top > TreeClientRect.bottom )
				{	continue;	}
				PageCount=(TreeClientRect.bottom-TreeClientRect.top)/(ItemRect.bottom-ItemRect.top);
			}
			bAddNewOne = true;
			InsertPanelNodeToTreeCtrlByTreeNode(TreeCtrl, BoardTreeNodePtr, 10*PageCount);
		}
	}	
	m_StopComponentTreeBeClick = FALSE;
	TreeCtrl.SetRedraw(TRUE);
	TreeCtrl.Invalidate();
	TreeCtrl.UpdateWindow();	

	//if ( true == bAddNewOne )
	//{	ShowComponentTreeSelected(TreeCtrl);	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::ExecTreeCtrlComponentMenu(CPoint point)
{
	//::AfxMessageBox(_T("CTreeCtrlComponent::OnContextMenu"));
	CPoint CtrlPt = point;
	this->m_wndComponentTreeCtrl.ScreenToClient(&CtrlPt);
	UINT menuID = GetComponentTreeCtrlPopupMenuID(m_wndComponentTreeCtrl, CtrlPt);
	if ( NULL == menuID ) { return; }

	CMenu menu;		
	if ( menuID == 0 ) { return ; }
	VERIFY(menu.LoadMenu(menuID));
	AOIDataCollect.SwitchMultiLanguageMenu(menu, menuID);

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);

	CWnd* pWndPopupOwner = this;
	while (pWndPopupOwner->GetStyle() & WS_CHILD)
	{	pWndPopupOwner = pWndPopupOwner->GetParent();	}

	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	return ;
}
//-------------------------------------------------------------------------------------//
BOOL CEditComponentListDockPane::ExecProjectChanged(CJETPropertyGridProperty *pProp, bool &bModified)
{
	if ( NULL == pProp ) { return FALSE; }	
	CAOIProject *ProjectPtr = (CAOIProject*)(pProp->GetData());
	if ( NULL == ProjectPtr ) { return FALSE; }
	if ( ProjectPtr->IsKindOf(RUNTIME_CLASS(CAOIProject)) == FALSE ) { return FALSE; }

	bool          bChanged = false;	
	int           nValue=0, nReading=0;
	bool          bValue=false, bReading=false, bEnabled=true;
	BOOL          BValue=FALSE, BReading=FALSE;
	double        dValue=0.0, dReading=0.0;		
	PANEL_SIDE_MODE PanelSideMode;
	CString       strValue;
	CString       strName = pProp->GetName();
	CString       strDescr = pProp->GetDescription();
	COleVariant   vtValue = pProp->GetValue();
	COleVariant   vtValueOld = pProp->GetOriginalValue();
	DWORD_PTR     dwData = pProp->GetData();	
	CJETPropertyGridProperty *pProp2 = NULL;
	INFO_PROPERTY_ID ParamID = (INFO_PROPERTY_ID)(pProp->GetID());	
	TProjectParameter &Param = ProjectPtr->GetProjectParameter();

	switch ( ParamID )
	{
	case INFO_PROJECT_FILENAME:		
		break;
	case INFO_PROJECT_MODULE:
		strValue = pProp->GetValue();
		ProjectPtr->SetProjectModuleName(strValue);
		bModified = true;
		break;
	case INFO_PROJECT_VERSION:
		strValue = pProp->GetValue();
		ProjectPtr->SetProjectVersion(strValue);
		bModified = true;
		break;
	case INFO_PROJECT_PANEL_SIDE:
		strValue = pProp->GetValue();
		PanelSideMode = AOIDataDefine.FindPanelSideModeByText(strValue);
		ProjectPtr->SetProjectPanelSideMode(PanelSideMode);
		bModified = true;
		break;
	case INFO_PROJECT_TEST_WIDTH:
		strValue = pProp->GetValue();
		Param.m_TestSizeWidth = ::_ttof(strValue);
		bModified = true;
		break;
	case INFO_PROJECT_TEST_HEIGHT:
		strValue = pProp->GetValue();
		Param.m_TestSizeHeight = ::_ttof(strValue);
		bModified = true;
		break;
	case INFO_PROJECT_BARCODE:
		strValue = ProjectPtr->GetProjectBarcode();
		pProp->SetValue(strValue);
		break;
	case INFO_PROJECT_PANEL_COUNT:
		break;
	case INFO_PROJECT_BOARD_COUNT:		
		break;
	case INFO_PROJECT_MARK_COUNT:
		break;
	case INFO_PROJECT_BARCODE_COUNT:
		break;
	case INFO_PROJECT_COMPONENT_COUNT:
		break;
	case INFO_PROJECT_COMPONENT_BYPASS_COUNT:
		break;
	case INFO_PROJECT_FIELD_COUNT:
		break;
	case INFO_PROJECT_BARCODE_DEVICE_INDEX:
		strValue = pProp->GetValue();		
		nValue = JetAPI::StrToInt(strValue);
		nValue = nValue - 1;
		if ( nValue >=0 && nValue<MAX_BARCODE_DEVICE_COUNT )
		{
			ProjectPtr->SetProjectBarcodeDeviceIndex(nValue);			
			bModified = true;
		}
		break;
	case INFO_PROJECT_BARCODE_DEVICE_CODE_INDEX:
		strValue = pProp->GetValue();
		nValue = JetAPI::StrToInt(strValue);
		nValue = nValue - 1;
		if ( nValue >=0 && nValue<MAX_BARCODE_DEVICE_CODE_COUNT )
		{	
			ProjectPtr->SetProjectBarcodeDeviceCodeIndex(nValue);
			bModified = true;
		}		
		break;
	}	
	return TRUE;
}
//-------------------------------------------------------------------------------------//
BOOL CEditComponentListDockPane::ExecPanelChanged(CJETPropertyGridProperty *pProp, bool &bModified)
{
	if ( NULL == pProp ) { return FALSE; }
	CAOIPanel *PanelPtr = (CAOIPanel*)(pProp->GetData());
	if ( NULL == PanelPtr ) { return FALSE; }
	if ( PanelPtr->IsKindOf(RUNTIME_CLASS(CAOIPanel)) == FALSE ) { return FALSE; }

	bool          bChanged = false;	
	int           nValue=0, nReading=0;
	bool          bValue=false, bReading=false, bEnabled=true;
	BOOL          BValue=FALSE, BReading=FALSE;
	double        dValue=0.0, dReading=0.0;		
	CString       strValue;
	CString       strName = pProp->GetName();
	CString       strDescr = pProp->GetDescription();
	COleVariant   vtValue = pProp->GetValue();
	COleVariant   vtValueOld = pProp->GetOriginalValue();
	DWORD_PTR     dwData = pProp->GetData();	
	CAOIPanel     PanelObj = *PanelPtr;
	CJETPropertyGridProperty *pProp2 = NULL;
	INFO_PROPERTY_ID ParamID = (INFO_PROPERTY_ID)(pProp->GetID());	

	switch ( ParamID )
	{
	case INFO_PANEL_PANEL_INDEX:		
		break;	
	case INFO_PANEL_BARCODE:
		strValue = PanelPtr->GetPanelBarcode();
		pProp->SetValue(strValue);
		break;
	case INFO_PANEL_BOARD_COUNT:
		break;
	case INFO_PANEL_COMPONENT_COUNT:
		break;
	case INFO_PANEL_BYPASS:
		if ( AOIDataCollect.OperateLevelEditFuncBypassPanel() == false ) 
		{
			pProp->SetValue(pProp->GetOriginalValue());	
			return FALSE; 
		}
		if ( FALSE == vtValue.boolVal )
		{	PanelPtr->SetPanelBypassed(false); }
		else
		{	PanelPtr->SetPanelBypassed(true); }
		bModified = true;
		break;
	case INFO_PANEL_BARCODE_ENABLED:
		if ( FALSE == vtValue.boolVal )
		{	PanelPtr->SetPanelBarcodeEnabled(false); }
		else
		{	PanelPtr->SetPanelBarcodeEnabled(true); }
		bModified = true;
		break;
	case INFO_PANEL_BARCODE_DEVICE_INDEX:
		strValue = pProp->GetValue();		
		nValue = JetAPI::StrToInt(strValue);
		nValue = nValue - 1;		
		if ( nValue >=0 && nValue<MAX_BARCODE_DEVICE_COUNT )
		{
			PanelPtr->SetPanelBarcodeDeviceIndex(nValue);			
			bModified = true;
		}
		break;
	case INFO_PANEL_BARCODE_DEVICE_CODE_INDEX:
		strValue = pProp->GetValue();
		nValue = JetAPI::StrToInt(strValue);
		nValue = nValue - 1;
		if ( nValue >=0 && nValue<MAX_BARCODE_DEVICE_CODE_COUNT )
		{	
			PanelPtr->SetPanelBarcodeDeviceCodeIndex(nValue);
			bModified = true;
		}		
		break;
	case INFO_PANEL_BOARD_ROW_COUNT:
		strValue = pProp->GetValue();
		nValue = JetAPI::StrToInt(strValue);		
		if ( nValue >=0 )
		{	
			PanelPtr->SetPanelBoardRowCount(nValue);
			bModified = true;
		}
		break;
	case INFO_PANEL_BOARD_COL_COUNT:
		strValue = pProp->GetValue();
		nValue = JetAPI::StrToInt(strValue);		
		if ( nValue >=0 )
		{	
			PanelPtr->SetPanelBoardColCount(nValue);
			bModified = true;
		}
		break;
	case INFO_PANEL_BOARD_COL_BLOCK_COUNT:
		strValue = pProp->GetValue();
		nValue = JetAPI::StrToInt(strValue);		
		if ( nValue >=0 )
		{	
			PanelPtr->SetPanelBoardColBlockCount(nValue);
			bModified = true;
		}
		break;
	}	
	if ( true == bModified )
	{	LogOperCtrl.SaveLogPanelCompare(&PanelObj, PanelPtr);	}
	return TRUE;
}
//-------------------------------------------------------------------------------------//
BOOL CEditComponentListDockPane::ExecBoardChanged(CJETPropertyGridProperty *pProp, bool &bModified)
{
	if ( NULL == pProp ) { return FALSE; }
	CAOIBoard *BoardPtr = (CAOIBoard*)(pProp->GetData());
	if ( NULL == BoardPtr ) { return FALSE; }
	if ( BoardPtr->IsKindOf(RUNTIME_CLASS(CAOIBoard)) == FALSE ) { return FALSE; }

	bool          bChanged = false;	
	int           nValue=0, nReading=0;
	bool          bValue=false, bReading=false, bEnabled=true;
	BOOL          BValue=FALSE, BReading=FALSE;
	double        dValue=0.0, dReading=0.0;		
	CString       strValue;
	CString       strName = pProp->GetName();
	CString       strDescr = pProp->GetDescription();
	COleVariant   vtValue = pProp->GetValue();
	COleVariant   vtValueOld = pProp->GetOriginalValue();
	DWORD_PTR     dwData = pProp->GetData();	
	CAOIBoard     BoardObj = *BoardPtr;
	CJETPropertyGridProperty *pProp2 = NULL;
	INFO_PROPERTY_ID ParamID = (INFO_PROPERTY_ID)(pProp->GetID());	

	switch ( ParamID )
	{
	case INFO_BOARD_PANEL_INDEX:		
		break;
	case INFO_BOARD_BOARD_INDEX:
		break;
	case INFO_BOARD_BOARD_SIDE_MODE:
		strValue = pProp->GetValue();
		BoardPtr->SetBoardSideMode(AOIDataDefine.FindBoardSideModeByText(strValue));		
		bModified = true;
		break;
	case INFO_BOARD_BARCODE:
		strValue = BoardPtr->GetBoardBarcode();
		pProp->SetValue(strValue);
		break;
	case INFO_BOARD_COMPONENT_COUNT:
		break;
	case INFO_BOARD_BYPASS:
		if ( AOIDataCollect.OperateLevelEditFuncBypassBoard() == false )	
		{
			pProp->SetValue(pProp->GetOriginalValue());	
			return FALSE; 
		}
		if ( FALSE == vtValue.boolVal )
		{	BoardPtr->SetBoardBypassed(false); }
		else
		{	BoardPtr->SetBoardBypassed(true); }
		bModified = true;
		break;
	case INFO_BOARD_FD_ENABLE:
		if ( FALSE == vtValue.boolVal )
		{	BoardPtr->SetBoardMapEnable(false); }
		else
		{	BoardPtr->SetBoardMapEnable(true); }
		bModified = true;
		break;
	case INFO_BOARD_BARCODE_ENABLED:
		if ( FALSE == vtValue.boolVal )
		{	BoardPtr->SetBoardBarcodeEnabled(false); }
		else
		{	BoardPtr->SetBoardBarcodeEnabled(true); }
		bModified = true;
		break;
	case INFO_BOARD_BARCODE_DEVICE_INDEX:
		strValue = pProp->GetValue();		
		nValue = JetAPI::StrToInt(strValue);
		nValue = nValue - 1;			
		if ( nValue >=0 && nValue<MAX_BARCODE_DEVICE_COUNT )
		{
			BoardPtr->SetBoardBarcodeDeviceIndex(nValue);		
			bModified = true;
		}
		break;
	case INFO_BOARD_BARCODE_DEVICE_CODE_INDEX:
		strValue = pProp->GetValue();
		nValue = JetAPI::StrToInt(strValue);
		nValue = nValue - 1;
		if ( nValue >=0 && nValue<MAX_BARCODE_DEVICE_CODE_COUNT )
		{
			BoardPtr->SetBoardBarcodeDeviceCodeIndex(nValue);		
			bModified = true;
		}		
		break;
	}
	if ( true == bModified )
	{	LogOperCtrl.SaveLogBoardCompare(&BoardObj, BoardPtr);	}
	return TRUE;
}
//-------------------------------------------------------------------------------------//
BOOL CEditComponentListDockPane::ExecFdChanged(CJETPropertyGridProperty *pProp, bool &bModified)
{
	if ( NULL == pProp ) { return FALSE; }
	CAOIFd *FdPtr = (CAOIFd*)(pProp->GetData());
	if ( NULL == FdPtr ) { return FALSE; }
	if ( FdPtr->IsKindOf(RUNTIME_CLASS(CAOIFd)) == FALSE ) { return FALSE; }

	bool          bChanged = false;	
	int           nValue=0, nReading=0;
	bool          bValue=false, bReading=false, bEnabled=true;
	BOOL          BValue=FALSE, BReading=FALSE;
	double        dValue=0.0, dReading=0.0;		
	int           GroupID= 0;
	unsigned int  SortID = 0;
	CString       strValue;
	CString       strName = pProp->GetName();
	CString       strDescr = pProp->GetDescription();
	COleVariant   vtValue = pProp->GetValue();
	COleVariant   vtValueOld = pProp->GetOriginalValue();
	DWORD_PTR     dwData = pProp->GetData();
	CAOIFd        FdObj = *FdPtr;
	CJETPropertyGridProperty *pProp2 = NULL;
	INFO_PROPERTY_ID ParamID = (INFO_PROPERTY_ID)(pProp->GetID());	

	switch ( ParamID )
	{
	case INFO_FD_NAME:
		break;
	case INFO_FD_UNIQUE_ID:
		break;
	case INFO_FD_CAD_XY:
		break;
	case INFO_FD_STAGE_XY:
		break;
	case INFO_FD_TEACH_XY:
		break;
	case INFO_FD_GROUP_ID:
		strValue = pProp->GetValue();
		strValue.MakeUpper();		
		GroupID = ::_ttoi(strValue)-1;
		if ( GroupID < 0 )
		{	GroupID = -1; }
		FdPtr->SetFdGroupID(GroupID);		
		break;
	case INFO_FD_PANEL_INDEX:
		break;
	case INFO_FD_BOARD_INDEX:
		break;
	case INFO_FD_SORT_ID:
		strValue = pProp->GetValue();
		strValue.MakeUpper();		
		SortID = ::_ttoi(strValue);
		FdPtr->SetFdSortID(SortID);		
		break;
	case INFO_FD_LOCAL_BASE_PLANE_ID:
		strValue = pProp->GetValue();
		nValue = ::_ttoi(strValue);
		if ( nValue >= 0 )
		{	FdPtr->SetFdLocalBasePlaneID(nValue); }		
		bModified = true;
		break;
	}
	if ( true == bModified )
	{	LogOperCtrl.SaveLogFdCompare(&FdObj, FdPtr);	}
	FdObj.GetFdModelPtr()->SetModelAutoDeleteImageFolder(false);
	return TRUE;
}
//-------------------------------------------------------------------------------------//
BOOL CEditComponentListDockPane::ExecMarkChanged(CJETPropertyGridProperty *pProp, bool &bModified)
{
	if ( NULL == pProp ) { return FALSE; }
	CAOIMark *MarkPtr = (CAOIMark*)(pProp->GetData());
	if ( NULL == MarkPtr ) { return FALSE; }
	if ( MarkPtr->IsKindOf(RUNTIME_CLASS(CAOIMark)) == FALSE ) { return FALSE; }

	bool          bChanged = false;	
	int           nValue=0, nReading=0;
	bool          bValue=false, bReading=false, bEnabled=true;
	BOOL          BValue=FALSE, BReading=FALSE;
	double        dValue=0.0, dReading=0.0;		
	int           GroupID= 0;
	unsigned int  SortID = 0;
	CString       strValue;
	CString       strName = pProp->GetName();
	CString       strDescr = pProp->GetDescription();
	COleVariant   vtValue = pProp->GetValue();
	COleVariant   vtValueOld = pProp->GetOriginalValue();
	DWORD_PTR     dwData = pProp->GetData();	
	CAOIMark      MarkObj = *MarkPtr;
	CJETPropertyGridProperty *pProp2 = NULL;
	INFO_PROPERTY_ID ParamID = (INFO_PROPERTY_ID)(pProp->GetID());	

	switch ( ParamID )
	{
	case INFO_MARK_NAME:
		break;
	case INFO_MARK_UNIQUE_ID:
		break;
	case INFO_MARK_GROUP_ID:
		strValue = pProp->GetValue();
		strValue.MakeUpper();		
		GroupID = ::_ttoi(strValue)-1;
		if ( GroupID < 0 )
		{	GroupID = -1; }
		MarkPtr->SetMarkGroupID(GroupID);		
		break;
	case INFO_MARK_BYPASSED:
		if ( AOIDataCollect.OperateLevelEditFuncBypassMark() == false )	
		{
			pProp->SetValue(pProp->GetOriginalValue());	
			return FALSE; 
		}
		if ( FALSE == vtValue.boolVal )
		{	MarkPtr->SetMarkBypassed(false); }
		else
		{	MarkPtr->SetMarkBypassed(true); }
		bModified = true;
		break;
	case INFO_MARK_PANEL_INDEX:
		break;
	case INFO_MARK_BOARD_INDEX:
		break;	
	case INFO_MARK_LOCAL_BASE_PLANE_ID:
		strValue = pProp->GetValue();
		nValue = ::_ttoi(strValue);
		if ( nValue >= 0 )
		{	MarkPtr->SetMarkLocalBasePlaneID(nValue); }		
		bModified = true;
		break;
	case INFO_MARK_LOCAL_PLANE_NORMAL_Z:
		break;
	}
	if ( true == bModified )
	{	LogOperCtrl.SaveLogMarkCompare(&MarkObj, MarkPtr);	}
	MarkObj.GetMarkModelPtr()->SetModelAutoDeleteImageFolder(false);
	return TRUE;
}
//-------------------------------------------------------------------------------------//
BOOL  CEditComponentListDockPane::ExecComponentChanged(CJETPropertyGridProperty *pProp, bool &bModified)
{
	if ( NULL == pProp ) { return FALSE; }	
	CAOIComponent *ComponentPtr = (CAOIComponent*)(pProp->GetData());
	if ( NULL == ComponentPtr ) { return FALSE; }
	if ( ComponentPtr->IsKindOf(RUNTIME_CLASS(CAOIComponent)) == FALSE ) { return FALSE; }
	CAOIPanel     *PanelPtr = ComponentPtr->GetComponentPanelPtr();
	CAOIBoard     *BoardPtr = ComponentPtr->GetComponentBoardPtr();
	CAOIProject   *ProjectPtr = ComponentPtr->GetComponentProjectPtr();
	if ( NULL==BoardPtr || NULL==PanelPtr || NULL==ProjectPtr ) { return FALSE; }
	
	CMapCoordinate *MapCTSPtr = NULL;
	bool          bChanged = false;	
	int           nValue=0, nReading=0;
	bool          bValue=false, bReading=false, bEnabled=true;
	BOOL          BValue=FALSE, BReading=FALSE;
	double        dValue=0.0, dReading=0.0;	
	double        CadPosX=0, CadPosY=0;	
	CString       str;
	CString       strValue;
	CString       strName = pProp->GetName();
	CString       strDescr = pProp->GetDescription();
	COleVariant   vtValue = pProp->GetValue();
	COleVariant   vtValueOld = pProp->GetOriginalValue();
	DWORD_PTR     dwData = pProp->GetData();
	CAOIComponent ComponentObj = *ComponentPtr;
	CJETPropertyGridProperty *pProp2 = NULL;
	INFO_PROPERTY_ID ParamID = (INFO_PROPERTY_ID)(pProp->GetID());	
	SAVE_TEST_IMAGE_MODE SaveTestImageMode;
	DISTRICT_ID     DistrictID = ComponentPtr->GetComponentDistrictID();

	MapCTSPtr = BoardPtr->GetBoardMapCTSPtr(DistrictID);
	if ( NULL == MapCTSPtr )
	{	MapCTSPtr = PanelPtr->GetPanelMapCTSPtr(DistrictID);	}
	switch ( ParamID )
	{
	case INFO_COMPONENT_NAME:
		strValue = pProp->GetValue();
		strValue.MakeUpper();
		if ( BoardPtr->ChceckBoardComponentNameExist(strValue) == false )
		{			
			ComponentPtr->ChangeComponentName(strValue);
			bModified = true;
		}
		else
		{
			strValue = ComponentPtr->GetComponentName();
			pProp->SetValue(strValue);
		}
		break;
	case INFO_COMPONENT_CAD_XY:
		break;
	case INFO_COMPONENT_STAGE_XY:
		break;
	case INFO_COMPONENT_ANGLE:
		strValue = pProp->GetValue();
		dValue = JetAPI::StrToDbl(strValue);
		dReading = ComponentPtr->GetComponentAngle();
		CadPosX = ComponentPtr->GetComponentCadPosX();
		CadPosY = ComponentPtr->GetComponentCadPosY();
		ComponentPtr->RotateComponent(dValue-dReading, CadPosX, CadPosY, MapCTSPtr);
		bModified = true;
		break;
	case INFO_COMPONENT_PART_NUMBER:
		strValue = pProp->GetValue();
		strValue.MakeUpper();		
		ComponentPtr->SetComponentPartNumber(strValue);
		bModified = true;
		break;
	case INFO_COMPONENT_MODEL_NAME:
		strValue = pProp->GetValue();
		strValue.MakeUpper();		
		ComponentPtr->SetComponentModelName(strValue);
		break;
	case INFO_COMPONENT_COL_INDEX:
		strValue = pProp->GetValue();
		nValue = ::_ttoi(strValue)-1;
		ComponentPtr->SetComponentColIndex(nValue);
		bModified = true;	
		break;
	case INFO_COMPONENT_ROW_INDEX:
		strValue = pProp->GetValue();
		nValue = ::_ttoi(strValue)-1;
		ComponentPtr->SetComponentRowIndex(nValue);
		bModified = true;	
		break;
	case INFO_COMPONENT_BYPASS:
		if ( AOIDataCollect.OperateLevelEditFuncBypassComponent() == false )	
		{
			pProp->SetValue(pProp->GetOriginalValue());	
			return FALSE; 
		}
		if ( FALSE == vtValue.boolVal )
		{	ComponentPtr->SetComponentBypassed(false); }
		else
		{	ComponentPtr->SetComponentBypassed(true); }
		bModified = true;
		break;
	case INFO_COMPONENT_BYPASS3D:
		if ( AOIDataCollect.OperateLevelEditFuncBypassComponent() == false )	
		{
			pProp->SetValue(pProp->GetOriginalValue());	
			return FALSE; 
		}
		if ( FALSE == vtValue.boolVal )
		{	ComponentPtr->SetComponentBypass3D(false); }
		else
		{	ComponentPtr->SetComponentBypass3D(true); }
		bModified = true;
		break;
	case INFO_COMPONENT_SAVE_IMAGE:
		strValue = pProp->GetValue();
		SaveTestImageMode = AOIDataDefine.FindSaveTestImageModeByText(strValue);
		ComponentPtr->SetComponentSaveTestImageMode(SaveTestImageMode);
		bModified = true;
		break;
	case INFO_COMPONENT_BAD_MARK:
		if ( FALSE == vtValue.boolVal )
		{	ComponentPtr->ChangeComponentXBoardUnit(false); }
		else
		{	ComponentPtr->ChangeComponentXBoardUnit(true); }
		bModified = true;
		break;
	case INFO_COMPONENT_OFFSET_XY:
		break;
	case INFO_COMPONENT_SKEW_ANGLE:
		break;
	case INFO_COMPONENT_MODEL_CLASS_ID:
		strValue = pProp->GetValue();		
		if ( strValue==AOIDataDefine.GetDisableText() )
		{	nValue = MODEL_CLASS_ID_NONE;	}
		else
		{	nValue = JetAPI::StrToInt(strValue);	}
		if ( nValue >=MODEL_CLASS_ID_NONE && nValue<MODEL_CLASS_ID_COUNT )
		{
			ComponentPtr->SetComponentModelClassID(nValue);		
			bModified = true;
		}
		break;
	case INFO_COMPONENT_MODEL_ISOLATED:
		ProjectPtr->SelectProjectAllComponents(false);
		ProjectPtr->SetProjectActiveComponent(ComponentPtr);
		ComponentPtr->SetComponentSelected(true);
		bValue = ComponentPtr->GetComponentModelIsolated();		
		if ( true == bValue )
		{
			str = _T("Disable Model Isolated will clear the model datas, do you want to continue?");
			str = LoadMultiLanguageString(str, str);
			if ( JetAPI::ShowMessageBox(str, MB_YESNO) != IDYES )
			{	
				pProp->SetValue((_variant_t)bValue);
				return TRUE; 
			}
		}		
		ProjectPtr->SwitchProjectComponentModelIsolated();
		LogOperCtrl.SaveLogProjectComponentSelectedModelIsolated(ProjectPtr);
		AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_WND_PROPERTY_WND, WPARAM_BUILD_WND_SELECTED, NULL);
		AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);
		break;
	case INFO_COMPONENT_SELF_FIELD:
		break;
	case INFO_COMPONENT_LOCAL_BASE_PLANE_ID:
		strValue = pProp->GetValue();
		nValue = ::_ttoi(strValue);
		if ( nValue >= 0 )
		{	ComponentPtr->SetComponentLocalBasePlaneID(nValue); }
		bModified = true;
		break;
	case INFO_COMPONENT_SAVE_REPORT_ARS:
		if ( FALSE == vtValue.boolVal )
		{	ComponentPtr->SetComponentSaveReport_ARS(false); }
		else
		{	ComponentPtr->SetComponentSaveReport_ARS(true); }
		bModified = true;	
		break;
	case INFO_COMPONENT_M2M_SCO_USE:
		if (FALSE == vtValue.boolVal) { ComponentPtr->SetComponentHASI_SPIOffset_Enable(false); }
		else { ComponentPtr->SetComponentHASI_SPIOffset_Enable(true); }
		bModified = true;
		break;
	case INFO_COMPONENT_M2M_SAVE_IMAGE:
		if (FALSE == vtValue.boolVal) { ComponentPtr->SetComponentHASI_SaveImage(false); }
		else { ComponentPtr->SetComponentHASI_SaveImage(true); }
		bModified = true;
		break;
	}	

	if ( true == bModified )
	{	LogOperCtrl.SaveLogComponentCompare(&ComponentObj, ComponentPtr);	}
	ComponentObj.GetComponentModelPtr()->SetModelAutoDeleteImageFolder(false);
	return TRUE;
}
//-------------------------------------------------------------------------------------//
BOOL CEditComponentListDockPane::ExecBarcodeChanged(CJETPropertyGridProperty *pProp, bool &bModified)
{
	if ( NULL == pProp ) { return FALSE; }	
	CAOIBarcode *BarcodePtr = (CAOIBarcode*)(pProp->GetData());
	if ( NULL == BarcodePtr ) { return FALSE; }
	if ( BarcodePtr->IsKindOf(RUNTIME_CLASS(CAOIBarcode)) == FALSE ) { return FALSE; }
	
	int           GroupID=0;
	bool          bChanged = false;			
	int           nValue=0, nReading=0;
	bool          bValue=false, bReading=false, bEnabled=true;
	BOOL          BValue=FALSE, BReading=FALSE;
	double        dValue=0.0, dReading=0.0;			
	CString       strValue;
	CString       strName = pProp->GetName();
	CString       strDescr = pProp->GetDescription();
	COleVariant   vtValue = pProp->GetValue();
	COleVariant   vtValueOld = pProp->GetOriginalValue();
	DWORD_PTR     dwData = pProp->GetData();	
	CJETPropertyGridProperty *pProp2 = NULL;
	INFO_PROPERTY_ID ParamID = (INFO_PROPERTY_ID)(pProp->GetID());	
	BARCODE_SPREAD_MODE       BarcodeSpreadMode;
	BARCODE_BELONG_MODE       BarcodeBelongMode;
	SAVE_TEST_IMAGE_MODE      SaveTestImageMode;

	switch ( ParamID )
	{
	case INFO_BARCODE_NAME:		
		break;
	case INFO_BARCODE_GROUP_ID:
		strValue = pProp->GetValue();
		strValue.MakeUpper();		
		GroupID = ::_ttoi(strValue)-1;
		if ( GroupID < 0 )
		{	GroupID = -1; }
		BarcodePtr->SetBarcodeGroupID(GroupID);		
		LogOperCtrl.SaveLogBarcodeOperate(BarcodePtr, _T("Set"), _T("Group ID"), GroupID+1);
		break;
	case INFO_BARCODE_PANEL_INDEX:
		break;
	case INFO_BARCODE_BOARD_INDEX:
		break;
	case INFO_BARCODE_SPREAD_MODE:
		strValue = pProp->GetValue();
		BarcodeSpreadMode = AOIDataDefine.FindBarcodeSpreadModeByText(strValue);
		BarcodePtr->SetBarcodeSpreadMode(BarcodeSpreadMode);
		bModified = true;
		LogOperCtrl.SaveLogBarcodeOperate(BarcodePtr, _T("Set"), _T("Barcode Spread Mode"), strValue);
		break;
	case INFO_BARCODE_BELONG_MODE:
		strValue = pProp->GetValue();
		BarcodeBelongMode = AOIDataDefine.FindBarcodeBelongModeByText(strValue);
		BarcodePtr->SetBarcodeBelongMode(BarcodeBelongMode);
		bModified = true;
		LogOperCtrl.SaveLogBarcodeOperate(BarcodePtr, _T("Set"), _T("Save Barcode Belong Mode"), strValue);
		break;
	case INFO_BARCODE_ANGLE:
		break;
	case INFO_BARCODE_RESULT:
		strValue = BarcodePtr->GetBarcodeResultText();
		pProp->SetValue(strValue);		
		break;	
	case INFO_BARCODE_SAVE_IMAGE:
		strValue = pProp->GetValue();
		SaveTestImageMode = AOIDataDefine.FindSaveTestImageModeByText(strValue);
		BarcodePtr->SetBarcodeSaveTestImageMode(SaveTestImageMode);
		bModified = true;
		LogOperCtrl.SaveLogBarcodeOperate(BarcodePtr, _T("Set"), _T("Save Test Image Mode"), strValue);
		break;
	case INFO_BARCODE_LOCAL_BASE_PLANE_ID:
		strValue = pProp->GetValue();
		nValue = ::_ttoi(strValue);
		if ( nValue >= 0 )
		{	BarcodePtr->SetBarcodeLocalBasePlaneID(nValue); }		
		bModified = true;
		LogOperCtrl.SaveLogBarcodeOperate(BarcodePtr, _T("Set"), _T("Local Base Plane ID"), nValue);
		break;
	}	
	return TRUE;
}
//-------------------------------------------------------------------------------------//
bool CEditComponentListDockPane::RemoveTreeItemFdSelected(CThisTreeCtrl &TreeCtrl)//移除選取到的定位點的結點
{
	if ( NULL == TreeCtrl.GetSafeHwnd() ) { return false; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }

	int i=0;
	unsigned int Index=0;
	CString      str;	
	CString      ItemText;
	DWORD_PTR    ItemData = 0;
	unsigned int ItemType = 0;
	unsigned int ItemIndex = 0;
	HTREEITEM  hFdItem = NULL;
	HTREEITEM  hPanelItem = NULL;
	HTREEITEM  hBoardItem = NULL;
	HTREEITEM  hComponentItem = NULL;
	HTREEITEM  hGroupItem = NULL;
	HTREEITEM  hWindowItem = NULL;
	HTREEITEM  hWndGroupItem = NULL;
	std::vector<HTREEITEM> RemoveItemList;

	CAOIFd        *FdPtr = NULL;
	CAOIPanel     *pPanel = NULL;
	CAOIBoard     *pBoard = NULL;
	CAOIBarcode   *pBarcode = NULL;
	CAOIComponent *pComponent = NULL;
	CAOIWindow    *pWindow = NULL;	

#ifdef _DEBUG
	CString FdName, PanelName, BoardName, ComName, GroupName, WndGroupName, WindowName;
#endif
	
	hPanelItem = TreeCtrl.GetRootItem();
	while ( NULL != hPanelItem )
	{
	#ifdef _DEBUG
		PanelName = TreeCtrl.GetItemText(hPanelItem);
	#endif
		ItemData = TreeCtrl.GetItemData(hPanelItem);		
		::DecodeItemlParam(ItemData, ItemType, ItemIndex);		
		if ( TREE_ITEM_TYPE_PANEL != ItemType )		
		{
			hPanelItem = TreeCtrl.GetNextSiblingItem(hPanelItem);
			continue;
		}
		
		pPanel = ProjectPtr->GetProjectPanelPtr(ItemIndex, true);
		if ( NULL == pPanel )
		{
			hPanelItem = TreeCtrl.GetNextSiblingItem(hPanelItem);
			continue;
		}

		hBoardItem = TreeCtrl.GetChildItem(hPanelItem);
		while ( NULL!=hBoardItem  )
		{
		#ifdef _DEBUG
			BoardName = TreeCtrl.GetItemText(hBoardItem);
		#endif
			ItemData = TreeCtrl.GetItemData(hBoardItem);		
			::DecodeItemlParam(ItemData, ItemType, ItemIndex);

			if ( TREE_ITEM_TYPE_FD_GROUP == ItemType )
			{	
				hFdItem = TreeCtrl.GetChildItem(hBoardItem);
				while ( hFdItem!=NULL )
				{
				#ifdef _DEBUG
					FdName = TreeCtrl.GetItemText(hFdItem);
				#endif				
					ItemData = TreeCtrl.GetItemData(hFdItem);		
					::DecodeItemlParam(ItemData, ItemType, ItemIndex);
					if ( TREE_ITEM_TYPE_FD != ItemType )
					{
						hFdItem = TreeCtrl.GetNextSiblingItem(hFdItem);
						continue;
					}
					
					FdPtr = ProjectPtr->GetProjectFdPtr(ItemIndex, true);
					if ( NULL == FdPtr )
					{
						hFdItem = TreeCtrl.GetNextSiblingItem(hFdItem);
						continue;
					}
					if ( FdPtr->GetFdSelected() == true ) 
					{	RemoveItemList.push_back(hFdItem); }					
					hFdItem = TreeCtrl.GetNextSiblingItem(hFdItem);
				};
				hBoardItem = TreeCtrl.GetNextSiblingItem(hBoardItem);
				continue;
			}

			if ( TREE_ITEM_TYPE_BARCODE == ItemType )
			{
				pBarcode = ProjectPtr->GetProjectBarcodePtr(ItemIndex, true);
				if ( NULL == pBarcode ) 
				{
					hBoardItem = TreeCtrl.GetNextSiblingItem(hBoardItem);
					continue;
				}				
				hBoardItem = TreeCtrl.GetNextSiblingItem(hBoardItem);
				continue;
			}

			if ( TREE_ITEM_TYPE_BOARD != ItemType  ) 
			{
				hBoardItem = TreeCtrl.GetNextSiblingItem(hBoardItem);
				continue;
			}

			pBoard = ProjectPtr->GetProjectBoardPtr(ItemIndex, true);
			if ( NULL == pBoard ) 
			{
				hBoardItem = TreeCtrl.GetNextSiblingItem(hBoardItem);
				continue;
			}
			hComponentItem = TreeCtrl.GetChildItem(hBoardItem);
			while ( hComponentItem!=NULL )
			{
			#ifdef _DEBUG
				ComName = TreeCtrl.GetItemText(hComponentItem);
			#endif				
				ItemData = TreeCtrl.GetItemData(hComponentItem);		
				::DecodeItemlParam(ItemData, ItemType, ItemIndex);
				
				if ( TREE_ITEM_TYPE_FD_GROUP == ItemType )
				{	
					hFdItem = TreeCtrl.GetChildItem(hComponentItem);
					while ( hFdItem!=NULL )
					{
					#ifdef _DEBUG
						FdName = TreeCtrl.GetItemText(hFdItem);
					#endif				
						ItemData = TreeCtrl.GetItemData(hFdItem);		
						::DecodeItemlParam(ItemData, ItemType, ItemIndex);
						if ( TREE_ITEM_TYPE_FD != ItemType )
						{
							hFdItem = TreeCtrl.GetNextSiblingItem(hFdItem);
							continue;
						}
					
						FdPtr = ProjectPtr->GetProjectFdPtr(ItemIndex, true);
						if ( NULL == FdPtr )
						{
							hFdItem = TreeCtrl.GetNextSiblingItem(hFdItem);
							continue;
						}						
						if ( FdPtr->GetFdSelected() == true ) 
						{	RemoveItemList.push_back(hFdItem); }									
						hFdItem = TreeCtrl.GetNextSiblingItem(hFdItem);
					};
					hComponentItem = TreeCtrl.GetNextSiblingItem(hComponentItem);
					continue;
				}
				hComponentItem = TreeCtrl.GetNextSiblingItem(hComponentItem);
			};			
			hBoardItem = TreeCtrl.GetNextSiblingItem(hBoardItem);	
		};		
		hPanelItem = TreeCtrl.GetNextSiblingItem(hPanelItem);	
	};	
	if ( RemoveTreeItemList(TreeCtrl, RemoveItemList) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditComponentListDockPane::RemoveTreeItemMarkSelected(CThisTreeCtrl &TreeCtrl)//移除選取到的特徵的結點
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }
	if ( TreeCtrl.GetSafeHwnd() == NULL ) { return false; }

	unsigned int Index=0;
	CString      str;	
	CString      ItemText;
	DWORD_PTR    ItemData = 0;
	unsigned int ItemType = 0;
	unsigned int ItemIndex = 0;
	HTREEITEM  hFdItem = NULL;
	HTREEITEM  hPanelItem = NULL;
	HTREEITEM  hBoardItem = NULL;
	HTREEITEM  hComponentItem = NULL;
	HTREEITEM  hGroupItem = NULL;
	HTREEITEM  hWindowItem = NULL;
	HTREEITEM  hWndGroupItem = NULL;
	std::vector<HTREEITEM> RemoveItemList;

	CAOIFd        *FdPtr = NULL;
	CAOIMark      *pMark = NULL;
	CAOIPanel     *pPanel = NULL;
	CAOIBoard     *pBoard = NULL;
	CAOIBarcode   *pBarcode = NULL;
	CAOIComponent *pComponent = NULL;
	CAOIWindow    *pWindow = NULL;	
	
#ifdef _DEBUG
	CString FdName, PanelName, BoardName, ComName, GroupName, WndGroupName, WindowName;
#endif
	
	hPanelItem = TreeCtrl.GetRootItem();
	while ( NULL != hPanelItem )
	{
	#ifdef _DEBUG
		PanelName = TreeCtrl.GetItemText(hPanelItem);
	#endif
		ItemData = TreeCtrl.GetItemData(hPanelItem);		
		::DecodeItemlParam(ItemData, ItemType, ItemIndex);		
		if ( TREE_ITEM_TYPE_PANEL != ItemType )		
		{
			hPanelItem = TreeCtrl.GetNextSiblingItem(hPanelItem);
			continue;
		}
		
		pPanel = ProjectPtr->GetProjectPanelPtr(ItemIndex, true);
		if ( NULL == pPanel )
		{
			hPanelItem = TreeCtrl.GetNextSiblingItem(hPanelItem);
			continue;
		}
		hBoardItem = TreeCtrl.GetChildItem(hPanelItem);
		while ( NULL!=hBoardItem  )
		{
		#ifdef _DEBUG
			BoardName = TreeCtrl.GetItemText(hBoardItem);
		#endif
			ItemData = TreeCtrl.GetItemData(hBoardItem);		
			::DecodeItemlParam(ItemData, ItemType, ItemIndex);

			if ( TREE_ITEM_TYPE_FD_GROUP == ItemType )
			{	
				hFdItem = TreeCtrl.GetChildItem(hBoardItem);
				while ( hFdItem!=NULL )
				{
				#ifdef _DEBUG
					FdName = TreeCtrl.GetItemText(hFdItem);
				#endif				
					ItemData = TreeCtrl.GetItemData(hFdItem);		
					::DecodeItemlParam(ItemData, ItemType, ItemIndex);
					if ( TREE_ITEM_TYPE_FD != ItemType )
					{
						hFdItem = TreeCtrl.GetNextSiblingItem(hFdItem);
						continue;
					}
					
					FdPtr = ProjectPtr->GetProjectFdPtr(ItemIndex, true);
					if ( NULL == FdPtr )
					{
						hFdItem = TreeCtrl.GetNextSiblingItem(hFdItem);
						continue;
					}						
					hFdItem = TreeCtrl.GetNextSiblingItem(hFdItem);
				};
				hBoardItem = TreeCtrl.GetNextSiblingItem(hBoardItem);
				continue;
			}

			if ( TREE_ITEM_TYPE_BARCODE == ItemType )
			{
				pBarcode = ProjectPtr->GetProjectBarcodePtr(ItemIndex, true);
				if ( NULL == pBarcode ) 
				{
					hBoardItem = TreeCtrl.GetNextSiblingItem(hBoardItem);
					continue;
				}				
				hBoardItem = TreeCtrl.GetNextSiblingItem(hBoardItem);
				continue;
			}

			if ( TREE_ITEM_TYPE_BOARD != ItemType  ) 
			{
				hBoardItem = TreeCtrl.GetNextSiblingItem(hBoardItem);
				continue;
			}

			pBoard = ProjectPtr->GetProjectBoardPtr(ItemIndex, true);
			if ( NULL == pBoard ) 
			{
				hBoardItem = TreeCtrl.GetNextSiblingItem(hBoardItem);
				continue;
			}
			hComponentItem = TreeCtrl.GetChildItem(hBoardItem);
			while ( hComponentItem!=NULL )
			{
			#ifdef _DEBUG
				ComName = TreeCtrl.GetItemText(hComponentItem);
			#endif				
				ItemData = TreeCtrl.GetItemData(hComponentItem);		
				::DecodeItemlParam(ItemData, ItemType, ItemIndex);
				
				if ( TREE_ITEM_TYPE_FD_GROUP == ItemType )
				{	
					hFdItem = TreeCtrl.GetChildItem(hComponentItem);
					while ( hFdItem!=NULL )
					{
					#ifdef _DEBUG
						FdName = TreeCtrl.GetItemText(hFdItem);
					#endif				
						ItemData = TreeCtrl.GetItemData(hFdItem);		
						::DecodeItemlParam(ItemData, ItemType, ItemIndex);
						if ( TREE_ITEM_TYPE_FD != ItemType )
						{
							hFdItem = TreeCtrl.GetNextSiblingItem(hFdItem);
							continue;
						}
					
						FdPtr = ProjectPtr->GetProjectFdPtr(ItemIndex, true);
						if ( NULL == FdPtr )
						{
							hFdItem = TreeCtrl.GetNextSiblingItem(hFdItem);
							continue;
						}										
						hFdItem = TreeCtrl.GetNextSiblingItem(hFdItem);
					};
					hComponentItem = TreeCtrl.GetNextSiblingItem(hComponentItem);
					continue;
				}
				
				if ( TREE_ITEM_TYPE_BARCODE == ItemType )
				{
					pBarcode = ProjectPtr->GetProjectBarcodePtr(ItemIndex, true);
					if ( NULL == pBarcode ) 
					{
						hComponentItem = TreeCtrl.GetNextSiblingItem(hComponentItem);
						continue;
					}					
					hComponentItem = TreeCtrl.GetNextSiblingItem(hComponentItem);
					continue;
				}
				
				if ( TREE_ITEM_TYPE_MARK == ItemType )
				{
					pMark = ProjectPtr->GetProjectMarkPtr(ItemIndex, true);
					if ( NULL == pMark ) 
					{
						hComponentItem = TreeCtrl.GetNextSiblingItem(hComponentItem);
						continue;
					}
					if ( pMark->GetMarkSelected() == true )
					{	RemoveItemList.push_back(hComponentItem);	}
					hComponentItem = TreeCtrl.GetNextSiblingItem(hComponentItem);
					continue;
				}						
				hComponentItem = TreeCtrl.GetNextSiblingItem(hComponentItem);
			};			
			hBoardItem = TreeCtrl.GetNextSiblingItem(hBoardItem);	
		};		
		hPanelItem = TreeCtrl.GetNextSiblingItem(hPanelItem);	
	};	

	if ( RemoveTreeItemList(TreeCtrl, RemoveItemList) == false )
	{	return false; }
	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditComponentListDockPane::RemoveTreeItemPanelSelected(CThisTreeCtrl &TreeCtrl)//移除選取到的整板的結點
{
	if ( NULL == TreeCtrl.GetSafeHwnd() ) { return false; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }

	int i=0;
	unsigned int Index=0;
	CString      str;	
	CString      ItemText;
	DWORD_PTR    ItemData = 0;
	unsigned int ItemType = 0;
	unsigned int ItemIndex = 0;
	HTREEITEM  hFdItem = NULL;
	HTREEITEM  hPanelItem = NULL;
	HTREEITEM  hBoardItem = NULL;
	HTREEITEM  hComponentItem = NULL;
	HTREEITEM  hGroupItem = NULL;
	HTREEITEM  hWindowItem = NULL;
	HTREEITEM  hWndGroupItem = NULL;
	std::vector<HTREEITEM> RemoveItemList;

	CAOIFd        *FdPtr = NULL;
	CAOIPanel     *pPanel = NULL;
	CAOIBoard     *pBoard = NULL;
	CAOIBarcode   *pBarcode = NULL;
	CAOIComponent *pComponent = NULL;
	CAOIWindow    *pWindow = NULL;	

#ifdef _DEBUG
	CString FdName, PanelName, BoardName, ComName, GroupName, WndGroupName, WindowName;
#endif
	
	hPanelItem = TreeCtrl.GetRootItem();
	while ( NULL != hPanelItem )
	{
	#ifdef _DEBUG
		PanelName = TreeCtrl.GetItemText(hPanelItem);
	#endif
		ItemData = TreeCtrl.GetItemData(hPanelItem);		
		::DecodeItemlParam(ItemData, ItemType, ItemIndex);		
		if ( TREE_ITEM_TYPE_PANEL != ItemType )		
		{
			hPanelItem = TreeCtrl.GetNextSiblingItem(hPanelItem);
			continue;
		}
		
		pPanel = ProjectPtr->GetProjectPanelPtr(ItemIndex, true);
		if ( NULL == pPanel )
		{
			hPanelItem = TreeCtrl.GetNextSiblingItem(hPanelItem);
			continue;
		}		
		if ( pPanel->GetPanelSelected() == true ) 
		{	RemoveItemList.push_back(hPanelItem);	}		
		hPanelItem = TreeCtrl.GetNextSiblingItem(hPanelItem);	
	};	
	
	if ( RemoveTreeItemList(TreeCtrl, RemoveItemList) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditComponentListDockPane::RemoveTreeItemBoardSelected(CThisTreeCtrl &TreeCtrl)//移除選取到的單板的結點
{
	if ( NULL == TreeCtrl.GetSafeHwnd() ) { return false; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }

	int i=0;
	unsigned int Index=0;
	CString      str;	
	CString      ItemText;
	DWORD_PTR    ItemData = 0;
	unsigned int ItemType = 0;
	unsigned int ItemIndex = 0;
	HTREEITEM  hFdItem = NULL;
	HTREEITEM  hPanelItem = NULL;
	HTREEITEM  hBoardItem = NULL;
	HTREEITEM  hComponentItem = NULL;
	HTREEITEM  hGroupItem = NULL;
	HTREEITEM  hWindowItem = NULL;
	HTREEITEM  hWndGroupItem = NULL;
	std::vector<HTREEITEM> RemoveItemList;

	CAOIFd        *FdPtr = NULL;
	CAOIPanel     *pPanel = NULL;
	CAOIBoard     *pBoard = NULL;
	CAOIBarcode   *pBarcode = NULL;
	CAOIComponent *pComponent = NULL;
	CAOIWindow    *pWindow = NULL;	

#ifdef _DEBUG
	CString FdName, PanelName, BoardName, ComName, GroupName, WndGroupName, WindowName;
#endif
	
	hPanelItem = TreeCtrl.GetRootItem();
	while ( NULL != hPanelItem )
	{
	#ifdef _DEBUG
		PanelName = TreeCtrl.GetItemText(hPanelItem);
	#endif
		ItemData = TreeCtrl.GetItemData(hPanelItem);		
		::DecodeItemlParam(ItemData, ItemType, ItemIndex);		
		if ( TREE_ITEM_TYPE_PANEL != ItemType )		
		{
			hPanelItem = TreeCtrl.GetNextSiblingItem(hPanelItem);
			continue;
		}
		
		pPanel = ProjectPtr->GetProjectPanelPtr(ItemIndex, true);
		if ( NULL == pPanel )
		{
			hPanelItem = TreeCtrl.GetNextSiblingItem(hPanelItem);
			continue;
		}		
		
		hBoardItem = TreeCtrl.GetChildItem(hPanelItem);
		while ( NULL!=hBoardItem  )
		{
		#ifdef _DEBUG
			BoardName = TreeCtrl.GetItemText(hBoardItem);
		#endif
			ItemData = TreeCtrl.GetItemData(hBoardItem);		
			::DecodeItemlParam(ItemData, ItemType, ItemIndex);

			if ( TREE_ITEM_TYPE_FD_GROUP == ItemType )
			{	
				hFdItem = TreeCtrl.GetChildItem(hBoardItem);
				while ( hFdItem!=NULL )
				{
				#ifdef _DEBUG
					FdName = TreeCtrl.GetItemText(hFdItem);
				#endif				
					ItemData = TreeCtrl.GetItemData(hFdItem);		
					::DecodeItemlParam(ItemData, ItemType, ItemIndex);
					if ( TREE_ITEM_TYPE_FD != ItemType )
					{
						hFdItem = TreeCtrl.GetNextSiblingItem(hFdItem);
						continue;
					}
					
					FdPtr = ProjectPtr->GetProjectFdPtr(ItemIndex, true);
					if ( NULL == FdPtr )
					{
						hFdItem = TreeCtrl.GetNextSiblingItem(hFdItem);
						continue;
					}						
					hFdItem = TreeCtrl.GetNextSiblingItem(hFdItem);
				};
				hBoardItem = TreeCtrl.GetNextSiblingItem(hBoardItem);
				continue;
			}

			if ( TREE_ITEM_TYPE_BARCODE == ItemType )
			{
				pBarcode = ProjectPtr->GetProjectBarcodePtr(ItemIndex, true);
				if ( NULL == pBarcode ) 
				{
					hBoardItem = TreeCtrl.GetNextSiblingItem(hBoardItem);
					continue;
				}				
				hBoardItem = TreeCtrl.GetNextSiblingItem(hBoardItem);
				continue;
			}

			if ( TREE_ITEM_TYPE_BOARD != ItemType  ) 
			{
				hBoardItem = TreeCtrl.GetNextSiblingItem(hBoardItem);
				continue;
			}

			pBoard = ProjectPtr->GetProjectBoardPtr(ItemIndex, true);
			if ( NULL == pBoard ) 
			{
				hBoardItem = TreeCtrl.GetNextSiblingItem(hBoardItem);
				continue;
			}			
			if ( pBoard->GetBoardSelected() == true ) 
			{	RemoveItemList.push_back(hBoardItem);	}
			hBoardItem = TreeCtrl.GetNextSiblingItem(hBoardItem);	
		};		
		hPanelItem = TreeCtrl.GetNextSiblingItem(hPanelItem);	
	};	

	if ( RemoveTreeItemList(TreeCtrl, RemoveItemList) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditComponentListDockPane::RemoveTreeItemBarcodeSelected(CThisTreeCtrl &TreeCtrl)//移除選取到的條碼的結點
{
	if ( NULL == TreeCtrl.GetSafeHwnd() ) { return false; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }

	int i=0;
	unsigned int Index=0;
	CString      str;	
	CString      ItemText;
	DWORD_PTR    ItemData = 0;
	unsigned int ItemType = 0;
	unsigned int ItemIndex = 0;
	HTREEITEM  hFdItem = NULL;
	HTREEITEM  hPanelItem = NULL;
	HTREEITEM  hBoardItem = NULL;
	HTREEITEM  hComponentItem = NULL;
	HTREEITEM  hGroupItem = NULL;
	HTREEITEM  hWindowItem = NULL;
	HTREEITEM  hWndGroupItem = NULL;
	std::vector<HTREEITEM> RemoveItemList;

	CAOIFd        *FdPtr = NULL;
	CAOIPanel     *pPanel = NULL;
	CAOIBoard     *pBoard = NULL;
	CAOIBarcode   *pBarcode = NULL;
	CAOIComponent *pComponent = NULL;
	CAOIWindow    *pWindow = NULL;
	
#ifdef _DEBUG
	CString FdName, PanelName, BoardName, ComName, GroupName, WndGroupName, WindowName;
#endif
	
	hPanelItem = TreeCtrl.GetRootItem();
	while ( NULL != hPanelItem )
	{
	#ifdef _DEBUG
		PanelName = TreeCtrl.GetItemText(hPanelItem);
	#endif
		ItemData = TreeCtrl.GetItemData(hPanelItem);		
		::DecodeItemlParam(ItemData, ItemType, ItemIndex);		
		if ( TREE_ITEM_TYPE_PANEL != ItemType )		
		{
			hPanelItem = TreeCtrl.GetNextSiblingItem(hPanelItem);
			continue;
		}
		
		pPanel = ProjectPtr->GetProjectPanelPtr(ItemIndex, true);
		if ( NULL == pPanel )
		{
			hPanelItem = TreeCtrl.GetNextSiblingItem(hPanelItem);
			continue;
		}		
		
		hBoardItem = TreeCtrl.GetChildItem(hPanelItem);
		while ( NULL!=hBoardItem  )
		{
		#ifdef _DEBUG
			BoardName = TreeCtrl.GetItemText(hBoardItem);
		#endif
			ItemData = TreeCtrl.GetItemData(hBoardItem);		
			::DecodeItemlParam(ItemData, ItemType, ItemIndex);

			if ( TREE_ITEM_TYPE_FD_GROUP == ItemType )
			{	
				hBoardItem = TreeCtrl.GetNextSiblingItem(hBoardItem);
				continue;
			}

			if ( TREE_ITEM_TYPE_BARCODE == ItemType )
			{
				pBarcode = ProjectPtr->GetProjectBarcodePtr(ItemIndex, true);
				if ( NULL == pBarcode ) 
				{
					hBoardItem = TreeCtrl.GetNextSiblingItem(hBoardItem);
					continue;
				}
				if ( pBarcode->GetBarcodeSelected() == true ) 
				{	RemoveItemList.push_back(hBoardItem);	}
				hBoardItem = TreeCtrl.GetNextSiblingItem(hBoardItem);
				continue;
			}
			if ( TREE_ITEM_TYPE_BOARD != ItemType  ) 
			{
				hBoardItem = TreeCtrl.GetNextSiblingItem(hBoardItem);
				continue;
			}

			pBoard = ProjectPtr->GetProjectBoardPtr(ItemIndex, true);
			if ( NULL == pBoard ) 
			{
				hBoardItem = TreeCtrl.GetNextSiblingItem(hBoardItem);
				continue;
			}

			hComponentItem = TreeCtrl.GetChildItem(hBoardItem);
			while ( hComponentItem!=NULL )
			{
			#ifdef _DEBUG
				ComName = TreeCtrl.GetItemText(hComponentItem);
			#endif				
				ItemData = TreeCtrl.GetItemData(hComponentItem);		
				::DecodeItemlParam(ItemData, ItemType, ItemIndex);
				
				if ( TREE_ITEM_TYPE_FD_GROUP == ItemType )
				{	
					hComponentItem = TreeCtrl.GetNextSiblingItem(hComponentItem);
					continue;
				}
				
				if ( TREE_ITEM_TYPE_BARCODE == ItemType )
				{
					pBarcode = ProjectPtr->GetProjectBarcodePtr(ItemIndex, true);
					if ( NULL == pBarcode ) 
					{
						hComponentItem = TreeCtrl.GetNextSiblingItem(hComponentItem);
						continue;
					}						
					if ( pBarcode->GetBarcodeSelected() == true ) 
					{	RemoveItemList.push_back(hComponentItem);	}					
					hComponentItem = TreeCtrl.GetNextSiblingItem(hComponentItem);
					continue;
				}
				
				if ( TREE_ITEM_TYPE_COMPONENT != ItemType ) 
				{	
					hComponentItem = TreeCtrl.GetNextSiblingItem(hComponentItem);
					continue; 
				}
				pComponent = ProjectPtr->GetProjectComponentPtr(ItemIndex, true);
				if ( NULL == pComponent ) 
				{
					hComponentItem = TreeCtrl.GetNextSiblingItem(hComponentItem);
					continue; 
				}					
				hComponentItem = TreeCtrl.GetNextSiblingItem(hComponentItem);
			};			
			hBoardItem = TreeCtrl.GetNextSiblingItem(hBoardItem);	
		};		
		hPanelItem = TreeCtrl.GetNextSiblingItem(hPanelItem);	
	};	
	
	if ( RemoveTreeItemList(TreeCtrl, RemoveItemList) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditComponentListDockPane::RemoveTreeItemComponentSelected(CThisTreeCtrl &TreeCtrl)//移除選取到的零件的結點
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }
	if ( TreeCtrl.GetSafeHwnd() == NULL ) { return false; }

	unsigned int Index=0;
	CString      str;	
	CString      ItemText;
	DWORD_PTR    ItemData = 0;
	unsigned int ItemType = 0;
	unsigned int ItemIndex = 0;
	HTREEITEM  hFdItem = NULL;
	HTREEITEM  hPanelItem = NULL;
	HTREEITEM  hBoardItem = NULL;
	HTREEITEM  hComponentItem = NULL;
	HTREEITEM  hGroupItem = NULL;
	HTREEITEM  hWindowItem = NULL;
	HTREEITEM  hWndGroupItem = NULL;
	std::vector<HTREEITEM> RemoveItemList;

	CAOIFd        *FdPtr = NULL;
	CAOIPanel     *pPanel = NULL;
	CAOIBoard     *pBoard = NULL;
	CAOIBarcode   *pBarcode = NULL;
	CAOIComponent *pComponent = NULL;
	CAOIWindow    *pWindow = NULL;	
	
#ifdef _DEBUG
	CString FdName, PanelName, BoardName, ComName, GroupName, WndGroupName, WindowName;
#endif
	
	hPanelItem = TreeCtrl.GetRootItem();
	while ( NULL != hPanelItem )
	{
	#ifdef _DEBUG
		PanelName = TreeCtrl.GetItemText(hPanelItem);
	#endif
		ItemData = TreeCtrl.GetItemData(hPanelItem);		
		::DecodeItemlParam(ItemData, ItemType, ItemIndex);		
		if ( TREE_ITEM_TYPE_PANEL != ItemType )		
		{
			hPanelItem = TreeCtrl.GetNextSiblingItem(hPanelItem);
			continue;
		}
		
		pPanel = ProjectPtr->GetProjectPanelPtr(ItemIndex, true);
		if ( NULL == pPanel )
		{
			hPanelItem = TreeCtrl.GetNextSiblingItem(hPanelItem);
			continue;
		}
		hBoardItem = TreeCtrl.GetChildItem(hPanelItem);
		while ( NULL!=hBoardItem  )
		{
		#ifdef _DEBUG
			BoardName = TreeCtrl.GetItemText(hBoardItem);
		#endif
			ItemData = TreeCtrl.GetItemData(hBoardItem);		
			::DecodeItemlParam(ItemData, ItemType, ItemIndex);

			if ( TREE_ITEM_TYPE_FD_GROUP == ItemType )
			{	
				hFdItem = TreeCtrl.GetChildItem(hBoardItem);
				while ( hFdItem!=NULL )
				{
				#ifdef _DEBUG
					FdName = TreeCtrl.GetItemText(hFdItem);
				#endif				
					ItemData = TreeCtrl.GetItemData(hFdItem);		
					::DecodeItemlParam(ItemData, ItemType, ItemIndex);
					if ( TREE_ITEM_TYPE_FD != ItemType )
					{
						hFdItem = TreeCtrl.GetNextSiblingItem(hFdItem);
						continue;
					}
					
					FdPtr = ProjectPtr->GetProjectFdPtr(ItemIndex, true);
					if ( NULL == FdPtr )
					{
						hFdItem = TreeCtrl.GetNextSiblingItem(hFdItem);
						continue;
					}						
					hFdItem = TreeCtrl.GetNextSiblingItem(hFdItem);
				};
				hBoardItem = TreeCtrl.GetNextSiblingItem(hBoardItem);
				continue;
			}

			if ( TREE_ITEM_TYPE_BARCODE == ItemType )
			{
				pBarcode = ProjectPtr->GetProjectBarcodePtr(ItemIndex, true);
				if ( NULL == pBarcode ) 
				{
					hBoardItem = TreeCtrl.GetNextSiblingItem(hBoardItem);
					continue;
				}				
				hBoardItem = TreeCtrl.GetNextSiblingItem(hBoardItem);
				continue;
			}

			if ( TREE_ITEM_TYPE_BOARD != ItemType  ) 
			{
				hBoardItem = TreeCtrl.GetNextSiblingItem(hBoardItem);
				continue;
			}

			pBoard = ProjectPtr->GetProjectBoardPtr(ItemIndex, true);
			if ( NULL == pBoard ) 
			{
				hBoardItem = TreeCtrl.GetNextSiblingItem(hBoardItem);
				continue;
			}
			hComponentItem = TreeCtrl.GetChildItem(hBoardItem);
			while ( hComponentItem!=NULL )
			{
			#ifdef _DEBUG
				ComName = TreeCtrl.GetItemText(hComponentItem);
			#endif				
				ItemData = TreeCtrl.GetItemData(hComponentItem);		
				::DecodeItemlParam(ItemData, ItemType, ItemIndex);
				
				if ( TREE_ITEM_TYPE_FD_GROUP == ItemType )
				{	
					hFdItem = TreeCtrl.GetChildItem(hComponentItem);
					while ( hFdItem!=NULL )
					{
					#ifdef _DEBUG
						FdName = TreeCtrl.GetItemText(hFdItem);
					#endif				
						ItemData = TreeCtrl.GetItemData(hFdItem);		
						::DecodeItemlParam(ItemData, ItemType, ItemIndex);
						if ( TREE_ITEM_TYPE_FD != ItemType )
						{
							hFdItem = TreeCtrl.GetNextSiblingItem(hFdItem);
							continue;
						}
					
						FdPtr = ProjectPtr->GetProjectFdPtr(ItemIndex, true);
						if ( NULL == FdPtr )
						{
							hFdItem = TreeCtrl.GetNextSiblingItem(hFdItem);
							continue;
						}										
						hFdItem = TreeCtrl.GetNextSiblingItem(hFdItem);
					};
					hComponentItem = TreeCtrl.GetNextSiblingItem(hComponentItem);
					continue;
				}
				
				if ( TREE_ITEM_TYPE_BARCODE == ItemType )
				{
					pBarcode = ProjectPtr->GetProjectBarcodePtr(ItemIndex, true);
					if ( NULL == pBarcode ) 
					{
						hComponentItem = TreeCtrl.GetNextSiblingItem(hComponentItem);
						continue;
					}					
					hComponentItem = TreeCtrl.GetNextSiblingItem(hComponentItem);
					continue;
				}
				
				if ( TREE_ITEM_TYPE_COMPONENT != ItemType ) 
				{	
					hComponentItem = TreeCtrl.GetNextSiblingItem(hComponentItem);
					continue; 
				}
				pComponent = ProjectPtr->GetProjectComponentPtr(ItemIndex, true);
				if ( NULL == pComponent ) 
				{
					hComponentItem = TreeCtrl.GetNextSiblingItem(hComponentItem);
					continue; 
				}
				if ( pComponent->GetComponentSelected() == true ) 
				{	RemoveItemList.push_back(hComponentItem);	}
				hComponentItem = TreeCtrl.GetNextSiblingItem(hComponentItem);
			};			
			hBoardItem = TreeCtrl.GetNextSiblingItem(hBoardItem);	
		};		
		hPanelItem = TreeCtrl.GetNextSiblingItem(hPanelItem);	
	};	

	if ( RemoveTreeItemList(TreeCtrl, RemoveItemList) == false )
	{	return false; }
	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditComponentListDockPane::RemoveTreeItemList(CThisTreeCtrl &TreeCtrl, std::vector<HTREEITEM> &RemoveItemList)
{
	if ( TreeCtrl.GetSafeHwnd() == NULL ) { return false; }
	const size_t RemoveItemCount = RemoveItemList.size();
	if ( 0 == RemoveItemCount ) { return true; }

	size_t i=0;
	TreeCtrl.SetRedraw(FALSE);
	for ( i=0; i<RemoveItemCount; i++ )
	{	TreeCtrl.DeleteItem(RemoveItemList[i]);	}
	TreeCtrl.SetRedraw(TRUE);	
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnClickComponentTreeCtrl(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here	
	HTREEITEM hItem = JetAPI::GetTreeSelectedItem(m_wndComponentTreeCtrl);
#ifdef _DEBUG
	CString str1 = m_wndComponentTreeCtrl.GetItemText(hItem);	
#endif
	if ( NULL == hItem ) 
	{	return ;	}
	const bool RBtn = false;
	m_ClickComponentTreeNode = TRUE;
	ExecSelectComponentTreeWndItem(m_wndComponentTreeCtrl, hItem, RBtn);	
	*pResult = 0;
}
//--------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnRClickComponentTreeCtrl(NMHDR* pNMHDR, LRESULT* pResult)
{
	HTREEITEM hItem = JetAPI::GetTreeSelectedItem(m_wndComponentTreeCtrl);
#ifdef _DEBUG
	CString str1 = m_wndComponentTreeCtrl.GetItemText(hItem);	
#endif
	if ( NULL == hItem ) 
	{	return ;	}
	const bool RBtn = true;
	m_ClickComponentTreeNode = TRUE;
	ExecSelectComponentTreeWndItem(m_wndComponentTreeCtrl, hItem, RBtn);	
	*pResult = 0;
}
//--------------------------------------------------------------------------------//
bool CEditComponentListDockPane::ExecDbclickComponentTreeCtrl()
{
	CThisTreeCtrl &TreeCtrl = m_wndComponentTreeCtrl;
	HTREEITEM  hItem = TreeCtrl.GetSelectedItem();
	if ( NULL == hItem ) 
	{	return true;	}

	CAOIProject *Project = GetActiveProject();
	if ( NULL == Project ) 
	{	return true;	}

	TPOINT3D       StagePos;
	TREGION4D      StageRgn;
	bool           GetPos=false;		
	unsigned int   ItemType=0, ItemIndex=0;
	CAOIFd        *pFd = NULL;	
	CAOIMark      *pMark = NULL;
	CAOIPanel     *pPanel = NULL;
	CAOIBoard     *pBoard = NULL;
	CAOIModel     *ModelPtr = NULL;
	CAOIBarcode   *BarcodePtr = NULL;
	CAOIComponent *pComponent = NULL;
	DWORD_PTR      ItemData = TreeCtrl.GetItemData(hItem);
	DRAW_MODEL_MODE DrawModelMode = AOIDataCollect.GetDrawModelMode();

	::DecodeItemlParam(ItemData, ItemType, ItemIndex);		

	GetPos = false;
	switch ( ItemType )
	{
	case TREE_ITEM_TYPE_PROJECT:
	case TREE_ITEM_TYPE_PROJECT_FILENAME:
	case TREE_ITEM_TYPE_PROJECT_MODULE:		
	case TREE_ITEM_TYPE_PROJECT_REGION_W:
	case TREE_ITEM_TYPE_PROJECT_REGION_H:
	case TREE_ITEM_TYPE_PROJECT_FD_RANGE_W:
	case TREE_ITEM_TYPE_PROJECT_FD_RANGE_H:
	case TREE_ITEM_TYPE_PROJECT_BARCODE_MODE:				
		break;
	case TREE_ITEM_TYPE_PANEL:			
		pPanel = Project->GetProjectPanelPtr(ItemIndex, true);
		break;		
	case TREE_ITEM_TYPE_FD:
		pFd = Project->GetProjectFdPtr(ItemIndex, true);
		if ( NULL != pFd )
		{
			GetPos = true;			
			StagePos = pFd->GetFdStagePos();			
			double FdSizeW = pFd->GetFdBodySizeW();
			double FdSizeH = pFd->GetFdBodySizeH();
			StageRgn.SetRgn(StagePos.x, StagePos.y, FdSizeW, FdSizeH);
		}		
		break;		
	case TREE_ITEM_TYPE_MARK:
		pMark = Project->GetProjectMarkPtr(ItemIndex, true);
		if ( NULL != pMark )
		{
			GetPos = true;			
			StagePos = pMark->GetMarkStagePos();
			double MarkSizeW = pMark->GetMarkBodySizeW();
			double MarkSizeH = pMark->GetMarkBodySizeH();
			StageRgn.SetRgn(StagePos.x, StagePos.y, MarkSizeW, MarkSizeH);
		}
		break;
	case TREE_ITEM_TYPE_BARCODE:
		BarcodePtr = Project->GetProjectBarcodePtr(ItemIndex, true);
		if ( NULL != BarcodePtr )
		{
			GetPos = true;			
			StagePos = BarcodePtr->GetBarcodeStagePos();			
			double BarcodeSizeW = BarcodePtr->GetBarcodeBodySizeW();
			double BarcodeSizeH = BarcodePtr->GetBarcodeBodySizeH();
			StageRgn.SetRgn(StagePos.x, StagePos.y, BarcodeSizeW, BarcodeSizeH);
		}		
		break;
	case TREE_ITEM_TYPE_BOARD:
		pBoard = Project->GetProjectBoardPtr(ItemIndex, true);
		break;		
	case TREE_ITEM_TYPE_COMPONENT:
	case TREE_ITEM_TYPE_COMPONENT_PACKAGE:
	case TREE_ITEM_TYPE_COMPONENT_PART_NUMBER:
	case TREE_ITEM_TYPE_COMPONENT_POSITION:
	case TREE_ITEM_TYPE_COMPONENT_OFFSET:
	case TREE_ITEM_TYPE_COMPONENT_MODE:
		//StagePos
		pComponent = Project->GetProjectComponentPtr(ItemIndex, true);
		if ( NULL != pComponent )
		{
			GetPos = true;			
			StagePos = pComponent->GetComponentStagePos();			
			if ( DRAW_MODEL_RESULT == DrawModelMode )
			{
				StagePos.x += pComponent->GetComponentStageOffsetX();
				StagePos.y += pComponent->GetComponentStageOffsetY();
			}
			pComponent->GetComponentRoiStageRegion(StageRgn);
			ModelPtr = pComponent->GetComponentModelPtr();
			if ( NULL != ModelPtr )
			{	
				ModelPtr->UnSelectModel();
				ModelPtr->SetModelBodyBoxActived(true);				
			}
		}		
		break;
	case TREE_ITEM_TYPE_MODEL:
		//StagePos
		break;
	case TREE_ITEM_TYPE_WINDOW:
		//StagePos
		GetPos = true;
		break;
	}			
	
	if ( true == GetPos )
	{	AOIDataCollect.MoveStageTo(StagePos.x, StagePos.y, StageRgn);	}		
	//JetAPI::PostFrameWndMessage(this, MSG_TREE_WND_MOVE_TO_ACTIVE_OBJ, NULL, NULL);
	//AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_UPDATE_PART_LIST, (LPARAM)(this));	
	return true;
}
//--------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnDbclickComponentTreeCtrl(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here		
	bool IsOK = ExecDbclickComponentTreeCtrl();	
	*pResult = 0;
}
//--------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnSelchangedComponentTreeCtrl(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if ( TRUE == m_StopComponentTreeBeClick ) { return ; }	
	if ( TRUE == m_ClickComponentTreeNode )
	{	
		//UpdateComponentTreeCtrlState(m_wndComponentTreeCtrl); 
		m_ClickComponentTreeNode = FALSE;
		return;
	}	
	const bool RBtn = false;
	HTREEITEM hItem = ((NM_TREEVIEW*) pNMTreeView)->itemNew.hItem;
	if ( NULL == hItem )  { return; }	
	ExecSelectComponentTreeWndItem(m_wndComponentTreeCtrl, hItem, RBtn);	
	*pResult = 0;
}
//--------------------------------------------------------------------------------//
LRESULT CEditComponentListDockPane::OnPropertyChanged(WPARAM wParam, LPARAM lParam)
{
	CJETPropertyGridProperty *pProp = (CJETPropertyGridProperty*)lParam;
	if ( NULL == pProp ) { return 0; }
	if ( pProp->IsKindOf(RUNTIME_CLASS(CJETPropertyGridProperty))==FALSE )
	{	return 0; }

	INFO_PROPERTY_ID ParamID = (INFO_PROPERTY_ID)(pProp->GetID());	

	bool bModified = false;	
	if ( ParamID>=INFO_PROJECT_NODE_BEGIN && ParamID<=INFO_PROJECT_NODE_END)
	{	ExecProjectChanged(pProp, bModified);	}
	else if ( ParamID>=INFO_PANEL_NODE_BEGIN && ParamID<=INFO_PANEL_NODE_END)
	{	ExecPanelChanged(pProp, bModified);	}
	else if ( ParamID>=INFO_BOARD_NODE_BEGIN && ParamID<=INFO_BOARD_NODE_END)
	{	ExecBoardChanged(pProp, bModified);	}
	else if ( ParamID>=INFO_FD_NODE_BEGIN && ParamID<=INFO_FD_NODE_END)
	{	ExecFdChanged(pProp, bModified);	}	
	else if ( ParamID>=INFO_MARK_NODE_BEGIN && ParamID<=INFO_MARK_NODE_END)
	{	ExecMarkChanged(pProp, bModified);	}
	else if ( ParamID>=INFO_BARCODE_NODE_BEGIN && ParamID<=INFO_BARCODE_NODE_END)
	{	ExecBarcodeChanged(pProp, bModified);	}
	else if ( ParamID>=INFO_COMPONENT_NODE_BEGIN && ParamID<=INFO_COMPONENT_NODE_END)
	{	ExecComponentChanged(pProp, bModified);	}	

	if ( true == bModified ) 
	{
		AOIDataCollect.SetManipulateMainMode(MANIPULATE_MAIN_SELECT);
		UpdateComponentTreeCtrlState(m_wndComponentTreeCtrl, TREE_CTRL_UPDATE_ALL);	
		AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_REDRAW_VIEW_WND, NULL);
	}
	return 0;
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnProjectSortComponent() 
{
	// TODO: Add your command handler code here
	CString      str;
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }
	str = _T("Do you want to sort components by name?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO ) { return; }
	if ( ProjectPtr->SortProjectComponentList() == false )
	{	JetAPI::ShowMessageBox(ProjectPtr->GetErrorString()); }
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_PART_LIST_WND, WPARAM_BUILD_PART_LIST, LPARAM_BUILD_DOCK_LIST_ALL);
}
//-------------------------------------------------------------------------------------//
bool CEditComponentListDockPane::ExecProjectRotation(double Angle)
{
//#ifndef OFFLINE_VERSION
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return false; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }	

	CString str, str1, str2;
	str = _T("Do you want to rotate the project");
	str = LoadMultiLanguageString(str, str);
	str1.Format(_T("%s [%.0f]?"), str, Angle);
	if ( JetAPI::ShowMessageBox(str1, MB_YESNO) != IDYES )
	{	return false; }	
	AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_CLOSE, NULL);	
	if ( ProjectPtr->RotateProject(Angle) == false ) 
	{
		str = ProjectPtr->GetErrorString();
		JetAPI::ShowMessageBox(str);
	}

	str1 = _T("Verify Fiducial Position, please.");
	str2 = _T("Re-Capture Project Map, Please");
	str1 = LoadMultiLanguageString(str1, str1);
	str2 = LoadMultiLanguageString(str2, str2);
	str.Format(_T("%s\n%s"), str1, str2);
	JetAPI::ShowMessageBox(str);
	//AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_REDRAW_VIEW_WND, NULL);	
	AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_OPEN, NULL);	
//#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnProjectRotate090()
{
	ExecProjectRotation(90);
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnProjectRotate180()
{
	ExecProjectRotation(180);
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnProjectRotate270()
{
	ExecProjectRotation(270);
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnPanelOffset()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	

	CString str, str1;
	CString strX, strY;
	CString strLabel;
	CString strCaption;
	CInputBoxWnd InputBox;	
	const size_t PanelSelCount = ProjectPtr->GetProjectPanelSelectedCount();
	if ( 0 == PanelSelCount ) { return; }

	strCaption = _T("Set Offset Wnd");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strLabel = _T("X:");
	InputBox.SetParam1(strCaption, strLabel, strX);
	if ( InputBox.DoModal() == IDCANCEL ) { return ; }
	strX = InputBox.m_DataEdit1;

	strLabel = _T("Y:");
	InputBox.SetParam1(strCaption, strLabel, strY);
	if ( InputBox.DoModal() == IDCANCEL ) { return ; }
	strY = InputBox.m_DataEdit1;
	const double OffsetX = ::_tcstod(strX, NULL);
	const double OffsetY = ::_tcstod(strY, NULL);
	ProjectPtr->MoveProjectPanelSelected(OffsetX, OffsetY);
	LogOperCtrl.SaveLogProjectPanelSelectedMove(ProjectPtr, OffsetX, OffsetY);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_REDRAW_VIEW_WND, NULL);
	return;
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnPanelMirrorPosX()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	

	CString str, str1;	
	CString strCount = AOIDataDefine.GetCountText();
	const size_t PanelSelCount = ProjectPtr->GetProjectPanelSelectedCount();
	if ( 0 == PanelSelCount ) { return; }

	str = _T("Do you want to mirror the panels");
	str = LoadMultiLanguageString(str, str);
	str1.Format(_T("%s [%s:%d] ?"), str, strCount, PanelSelCount);
	if ( JetAPI::ShowMessageBox(str1, MB_YESNO) != IDYES )
	{	return; }	
	ProjectPtr->MirrorXProjectPanelSelected();
	LogOperCtrl.SaveLogProjectPanelSelectedMirrorX(ProjectPtr);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_REDRAW_VIEW_WND, NULL);
	return;
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnPanelMirrorPosY()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	

	CString str, str1;
	CString strCount = AOIDataDefine.GetCountText();
	const size_t PanelSelCount = ProjectPtr->GetProjectPanelSelectedCount();
	if ( 0 == PanelSelCount ) { return; }

	str = _T("Do you want to mirror the panels");
	str = LoadMultiLanguageString(str, str);
	str1.Format(_T("%s [%s:%d] ?"), str, strCount, PanelSelCount);
	if ( JetAPI::ShowMessageBox(str1, MB_YESNO) != IDYES )
	{	return; }	
	ProjectPtr->MirrorYProjectPanelSelected();
	LogOperCtrl.SaveLogProjectPanelSelectedMirrorY(ProjectPtr);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_REDRAW_VIEW_WND, NULL);
	return;
}
//-------------------------------------------------------------------------------------//
bool CEditComponentListDockPane::ExecPanelRotation(double Angle)
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return false; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }	

	CString str, str1;	
	CString strCount = AOIDataDefine.GetCountText();
	const size_t PanelSelCount = ProjectPtr->GetProjectPanelSelectedCount();
	if ( 0 == PanelSelCount ) { return false; }

	str = _T("Do you want to rotate the panels");
	str = LoadMultiLanguageString(str, str);
	str1.Format(_T("%s %.0f [%s:%d] ?"), str, Angle, strCount, PanelSelCount);
	if ( JetAPI::ShowMessageBox(str1, MB_YESNO) != IDYES )
	{	return false; }	
	ProjectPtr->RotateProjectPanelSelected(Angle);
	LogOperCtrl.SaveLogProjectPanelSelectedRotate(ProjectPtr, Angle);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_REDRAW_VIEW_WND, NULL);	
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnPanelRotate090()
{
	ExecPanelRotation(90.0);
	return;
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnPanelRotate180()
{
	ExecPanelRotation(180.0);
	return;
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnPanelRotate270()
{
	ExecPanelRotation(270.0);
	return;
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnPanelDelete()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	
	if ( AOIDataCollect.OperateLevelEditFuncDelPanel() == false ) { return ; }

	CString str, str1;
	CString strCount = AOIDataDefine.GetCountText();
	CString strIndex = AOIDataDefine.GetIndexText();
	std::vector<CAOIPanel*> SelPanelList;
	ProjectPtr->GetProjectPanelSelected(SelPanelList);
	const size_t PanelSelCount = SelPanelList.size();
	if ( 0 == PanelSelCount ) { return; }
	CAOIPanel *PanelPtr = ProjectPtr->GetProjectActivePanel();
	if ( NULL == PanelPtr ) { return; }

	str = _T("Do you want to delete the panels");
	str = LoadMultiLanguageString(str, str);
	if ( PanelSelCount > 1 )
	{	str1.Format(_T("%s [%s:%d]?"), str, strCount, PanelSelCount); }
	else
	{	str1.Format(_T("%s [%s:%d]?"), str, strIndex, PanelPtr->GetPanelIndex_Project()+1);		}
	if ( JetAPI::ShowMessageBox(str1, MB_YESNO) != IDYES )
	{	return; }

	AOIDataCollect.ReleaseModelUniFrameList();
	//RemoveTreeItemPanelSelected(m_wndComponentTreeCtrl);//使用後Index會對不起來
	ProjectPtr->SetProjectActivePanel(NULL);
	LogOperCtrl.SaveLogProjectPanelSelectedDelete(ProjectPtr);
	ProjectPtr->DeleteProjectPanelSelected();	
	BuildComponentTreeCtrl(m_wndComponentTreeCtrl);
	ShowComponentTreeSelected(m_wndComponentTreeCtrl);
	AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_UPDATE, (LPARAM)(this));
	//AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);
	return;
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnPanelBypass()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	
	if ( AOIDataCollect.OperateLevelEditFuncBypassPanel() == false ) { return ; }

	CString str, str1;
	CString strCount = AOIDataDefine.GetCountText();	
	const size_t PanelSelCount = ProjectPtr->GetProjectPanelSelectedCount();
	if ( 0 == PanelSelCount ) { return; }

	str = _T("Do you want to bypass the panels");
	str = LoadMultiLanguageString(str, str);
	str1.Format(_T("%s [%s:%d]?"), str, strCount, PanelSelCount);
	if ( JetAPI::ShowMessageBox(str1, MB_YESNO) != IDYES )
	{	return; }

	ProjectPtr->SwitchProjectPanelBypassed();
	LogOperCtrl.SaveLogProjectPanelSelectedBypassed(ProjectPtr);
	UpdateComponentTreeCtrl(m_wndComponentTreeCtrl, TREE_CTRL_UPDATE_STATE);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_REDRAW_VIEW_WND, NULL);
	return;
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnBoardOffset()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	
	CString str, str1;
	CString strX, strY;
	CString strLabel;
	CString strCaption;
	CInputBoxWnd InputBox;	
	const size_t BoardSelCount = ProjectPtr->GetProjectBoardSelectedCount();	
	if ( 0 == BoardSelCount ) { return; }

	strCaption = _T("Set Offset Wnd");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strLabel = _T("X:");
	InputBox.SetParam1(strCaption, strLabel, strX);
	if ( InputBox.DoModal() == IDCANCEL ) { return ; }
	strX = InputBox.m_DataEdit1;

	strLabel = _T("Y:");
	InputBox.SetParam1(strCaption, strLabel, strY);
	if ( InputBox.DoModal() == IDCANCEL ) { return ; }
	strY = InputBox.m_DataEdit1;
	const double OffsetX = ::_tcstod(strX, NULL);
	const double OffsetY = ::_tcstod(strY, NULL);
	ProjectPtr->MoveProjectBoardSelected(OffsetX, OffsetY);
	LogOperCtrl.SaveLogProjectBoardSelectedMove(ProjectPtr, OffsetX, OffsetY);	
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_REDRAW_VIEW_WND, NULL);
	return;
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnBoardMirrorPosX()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }

	CString str, str1;
	CString strCount = AOIDataDefine.GetCountText();
	const size_t BoardSelCount = ProjectPtr->GetProjectBoardSelectedCount();	
	if ( 0 == BoardSelCount ) { return; }

	str = _T("Do you want to mirror the boards");
	str = LoadMultiLanguageString(str, str);
	str1.Format(_T("%s [%s:%d]?"), str, strCount, BoardSelCount);
	if ( JetAPI::ShowMessageBox(str1, MB_YESNO) != IDYES )
	{	return; }	
	
	ProjectPtr->MirrorXProjectBoardSelected();
	LogOperCtrl.SaveLogProjectBoardSelectedMirrorX(ProjectPtr);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_REDRAW_VIEW_WND, NULL);
	return;
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnBoardMirrorPosY()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	
	CString str, str1;
	CString strCount = AOIDataDefine.GetCountText();
	const size_t BoardSelCount = ProjectPtr->GetProjectBoardSelectedCount();
	if ( 0 == BoardSelCount ) { return; }

	str = _T("Do you want to mirror the boards");
	str = LoadMultiLanguageString(str, str);	
	str1.Format(_T("%s [%s:%d]?"), str, strCount, BoardSelCount);
	if ( JetAPI::ShowMessageBox(str1, MB_YESNO) != IDYES )
	{	return; }	
	
	ProjectPtr->MirrorYProjectBoardSelected();
	LogOperCtrl.SaveLogProjectBoardSelectedMirrorY(ProjectPtr);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_REDRAW_VIEW_WND, NULL);
	return;
}
//-------------------------------------------------------------------------------------//
bool CEditComponentListDockPane::ExecBoardRotation(double Angle)
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return false; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }

	CString str, str1;
	CString strCount = AOIDataDefine.GetCountText();
	const size_t BoardSelCount = ProjectPtr->GetProjectBoardSelectedCount();
	if ( 0 == BoardSelCount ) { return false; }

	str = _T("Do you want to rotate the boards");
	str = LoadMultiLanguageString(str, str);
	str1.Format(_T("%s %.0f [%s:%d] ?"), str, Angle, strCount, BoardSelCount);
	if ( JetAPI::ShowMessageBox(str1, MB_YESNO) != IDYES )
	{	return false; }	
	ProjectPtr->RotateProjectBoardSelected(Angle);
	LogOperCtrl.SaveLogProjectBoardSelectedRotate(ProjectPtr, Angle);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_REDRAW_VIEW_WND, NULL);
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnBoardRotate090()
{
	ExecBoardRotation(90.0);
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnBoardRotate180()
{
	ExecBoardRotation(180.0);
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnBoardRotate270()
{
	ExecBoardRotation(270.0);
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnBoardDelete()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	if ( AOIDataCollect.OperateLevelEditFuncDelBoard() == false ) { return ; }

	CString str, str1, str2;
	CString strCount = AOIDataDefine.GetCountText();
	CString strIndex = AOIDataDefine.GetIndexText();
	std::vector<CAOIBoard*> SelBoardList;
	ProjectPtr->GetProjectBoardSelected(SelBoardList);
	const size_t BoardSelCount = SelBoardList.size();
	if ( 0 == BoardSelCount ) { return; }
	CAOIBoard *BoardPtr = ProjectPtr->GetProjectActiveBoard();
	if ( NULL == BoardPtr ) { return; }
	CAOIPanel *PanelPtr = BoardPtr->GetBoardPanelPtr();

	str = _T("Do you want to delete the boards");
	str = LoadMultiLanguageString(str, str);
	if ( BoardSelCount > 1 )
	{	str1.Format(_T("%s [%s:%d]?"), str, strCount, BoardSelCount); }
	else
	{		
		unsigned int PanelIndex = BoardPtr->GetBoardPanelIndex_Project();
		unsigned int BoardIndex = BoardPtr->GetBoardIndex_Panel();
		str1.Format(_T("%s [%s:%d-%d]?"), str, strIndex, PanelIndex+1, BoardIndex+1);
	}
	if ( JetAPI::ShowMessageBox(str1, MB_YESNO) != IDYES )
	{	return; }

	AOIDataCollect.ReleaseModelUniFrameList();
	//RemoveTreeItemBoardSelected(m_wndComponentTreeCtrl);//使用後Index會對不起來	
	ProjectPtr->SetProjectActivePanel(PanelPtr);
	LogOperCtrl.SaveLogProjectBoardSelectedDelete(ProjectPtr);
	ProjectPtr->DeleteProjectBoardSelected();
	BuildComponentTreeCtrl(m_wndComponentTreeCtrl);	
	ShowComponentTreeSelected(m_wndComponentTreeCtrl);	
	AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_UPDATE, (LPARAM)(this));
	//AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);
	return;
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnBoardBypass()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	if ( AOIDataCollect.OperateLevelEditFuncBypassBoard() == false )	{	return ; }

	CString str, str1;
	CString strCount = AOIDataDefine.GetCountText();
	size_t BoardSelCount = ProjectPtr->GetProjectBoardSelectedCount();	
	if ( 0 == BoardSelCount ) { return; }

	str = _T("Do you want to bypass the boards");
	str = LoadMultiLanguageString(str, str);
	str1.Format(_T("%s [%s:%d]?"), str, strCount, BoardSelCount);
	if ( JetAPI::ShowMessageBox(str1, MB_YESNO) != IDYES )
	{	return; }	
	
	ProjectPtr->SwitchProjectBoardBypassed();
	LogOperCtrl.SaveLogProjectBoardSelectedBypassed(ProjectPtr);
	UpdateComponentTreeCtrl(m_wndComponentTreeCtrl, TREE_CTRL_UPDATE_STATE);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_REDRAW_VIEW_WND, NULL);
	return;
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnBoardChangePanel()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }

	CString str, str1;
	CString strCount = AOIDataDefine.GetCountText();
	size_t PanelCount = ProjectPtr->GetProjectPanelCount();
	size_t BoardSelCount = ProjectPtr->GetProjectBoardSelectedCount();		
	if ( 0 == BoardSelCount ) { return; }
	CAOIBoard      *BoardPtr = ProjectPtr->GetProjectBoardPtrBySelected();
	if ( NULL == BoardPtr ) { return; }

	size_t          i=0;
	POINT           Point;
	CString         strPanel;
	CString         strLabel;
	CString         strCaption;
	TListNode       Node;
	CInputListWnd   EnumWnd;
	DWORD_PTR       dwDefault=BoardPtr->GetBoardPanelIndex_Project();
	CAOIPanel      *PanelPtr = NULL;
	
	std::vector<TListNode> NodelList;	
	
	strPanel = AOIDataDefine.GetPanelText();
	strLabel = _T("Select Panel");
	strLabel = LoadMultiLanguageString(strLabel, strLabel);
	strCaption = _T("Change Board to Other Panel");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);		
	for ( i=0; i<PanelCount; i++ )
	{
		Node.Data = i;
		Node.Text.Format(_T("%s[%d]"), strPanel, i+1);		
		NodelList.push_back(Node);
	}
	::GetCursorPos(&Point);
	//ComboxWnd.SetWndPos(Point);	
	EnumWnd.SetParam1(strCaption, strLabel, dwDefault, NodelList);	
	if ( EnumWnd.DoModal() == IDCANCEL )
	{	return ; }		
	const int PanelIndex = (int)(EnumWnd.GetSelData());	
	PanelPtr = ProjectPtr->GetProjectPanelPtr(PanelIndex, true);
	if ( NULL == PanelPtr ) 
	{	return; }	
	ProjectPtr->ChangeProjectBoardSelectedPanel(PanelPtr);
	PostMessage(MSG_EDIT_PART_LIST_WND, WPARAM_BUILD_PART_LIST, LPARAM_BUILD_DOCK_LIST_PART);	
	return;
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnBoardSearchComponent()
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }

	CThisTreeCtrl &TreeCtrl= m_wndComponentTreeCtrl;	

	CString    str, str2;
	CString    strCaption;
	CString    strValue;
	CString    strLabel;
	CInputBoxWnd  InputBox;
	CAOIBoard      *BoardPtr = ProjectPtr->GetProjectActiveBoard();
	if ( NULL == BoardPtr ) { return; }

	strCaption = _T("Search Component");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strLabel = AOIDataDefine.GetNameText();
	InputBox.SetParam1(strCaption, strLabel, strValue);
	if ( InputBox.DoModal() == IDCANCEL ) { return ; }
	strValue = InputBox.m_DataEdit1;
	CAOIComponent *ComponentPtr = BoardPtr->GetBoardComponentPtrByName(strValue);
	if ( NULL == ComponentPtr )
	{
		str = _T("Can not find the component");
		str = LoadMultiLanguageString(str, str);
		str2.Format(_T("%s[%s]"), str, strValue);
		JetAPI::ShowMessageBox(str2);
		return;
	}	
	m_MoveToComponent = false;
	ProjectPtr->SelectProjectAllPanels(false);
	ProjectPtr->SelectProjectAllBoards(false);
	ProjectPtr->SelectProjectAllComponents(false);
	ComponentPtr->SetComponentSelected(true);
	ProjectPtr->SetProjectActiveComponent(ComponentPtr);		
	UpdateComponentTreeCtrlState(TreeCtrl, TREE_CTRL_UPDATE_STATE);
	ShowComponentTreeSelected(TreeCtrl);
	m_MoveToComponent = true;
	AOIDataCollect.MoveStageToComponentOrField(ComponentPtr, true);
	AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_UPDATE, (LPARAM)(this));	
	return;
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnComponentOffset()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	CAOIComponent *ComponentPtr = ProjectPtr->GetProjectActiveComponent();
	if ( NULL == ComponentPtr ) { return ; }
	
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
	BuildInfoPropCtrl_Component(m_wndInfomationPropCtrl, ComponentPtr);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnComponentSetPos()
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
	BuildInfoPropCtrl_Component(m_wndInfomationPropCtrl, ComponentPtr);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnComponentMirrorPosX()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return ; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }		
	const size_t ComponentSelCount = ProjectPtr->GetProjectComponentSelectedCount();
	if ( 0 == ComponentSelCount ) { return; }

	ProjectPtr->MirrorXProjectComponentSelected();
	LogOperCtrl.SaveLogProjectComponentSelectedMirrorX(ProjectPtr);
	UpdateComponentTreeCtrlState(m_wndComponentTreeCtrl, TREE_CTRL_UPDATE_TEXT);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnComponentMirrorPosY()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return ; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	
	const size_t ComponentSelCount = ProjectPtr->GetProjectComponentSelectedCount();
	if ( 0 == ComponentSelCount ) { return; }

	ProjectPtr->MirrorYProjectComponentSelected();
	LogOperCtrl.SaveLogProjectComponentSelectedMirrorY(ProjectPtr);
	UpdateComponentTreeCtrlState(m_wndComponentTreeCtrl, TREE_CTRL_UPDATE_TEXT);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);
	return ;
}
//-------------------------------------------------------------------------------------//
bool CEditComponentListDockPane::ExecComponentRotation(double Angle)
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return true; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }
	const size_t ComponentSelCount = ProjectPtr->GetProjectComponentSelectedCount();
	if ( 0 == ComponentSelCount ) { return false; }

	ProjectPtr->RotateProjectComponentSelected(Angle);
	LogOperCtrl.SaveLogProjectComponentSelectedRotate(ProjectPtr, Angle);
	UpdateComponentTreeCtrlState(m_wndComponentTreeCtrl, TREE_CTRL_UPDATE_TEXT);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnComponentRotate090()
{
	ExecComponentRotation(90.0);
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnComponentRotate180()
{
	ExecComponentRotation(180.0);
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnComponentRotate270()
{
	ExecComponentRotation(270.0);
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnComponentRotateAny()
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
void CEditComponentListDockPane::OnComponentRotateReverse()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return ; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	const size_t ComponentSelCount = ProjectPtr->GetProjectComponentSelectedCount();
	if ( 0 == ComponentSelCount ) { return; }

	ProjectPtr->ReverseProjectComponentSelected();
	UpdateComponentTreeCtrlState(m_wndComponentTreeCtrl, TREE_CTRL_UPDATE_TEXT);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnComponentRename()
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
	UpdateComponentTreeCtrlState(m_wndComponentTreeCtrl, TREE_CTRL_UPDATE_TEXT);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnComponentDelete()
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
	CAOIComponent *ComponentPtr = ProjectPtr->GetProjectActiveComponent();
	if ( NULL == ComponentPtr ) { return; }
	CAOIBoard *BoardPtr = ComponentPtr->GetComponentBoardPtr();

	CString str, str1, str2;
	CString strCount = AOIDataDefine.GetCountText();
	str1 = _T("Do you want to delete the selected components");
	str1 = LoadMultiLanguageString(str1, str1);
	if ( SelComponentCount > 1 ) 
	{	str.Format(_T("%s [%s:%d]?"), str1, strCount, SelComponentCount); }
	else
	{		
		str2 = ComponentPtr->GetComponentFullName();
		str.Format(_T("%s [%s]?"), str1, str2);
	}
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) != IDYES )
	{	return; }
	AOIDataCollect.ReleaseModelUniFrameList();
	//RemoveTreeItemComponentSelected(m_wndComponentTreeCtrl);//使用後Index會對不起來
	ProjectPtr->SetProjectActiveBoard(BoardPtr);
	LogOperCtrl.SaveLogProjectComponentSelectedDelete(ProjectPtr);
	ProjectPtr->DeleteProjectComponentSelected();	
	BuildComponentTreeCtrl(m_wndComponentTreeCtrl);	
	ShowComponentTreeSelected(m_wndComponentTreeCtrl);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnComponentSetNozzleName()
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
	UpdateComponentTreeCtrlState(m_wndComponentTreeCtrl, TREE_CTRL_UPDATE_TEXT);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnComponentSetPartNumber()
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
	UpdateComponentTreeCtrlState(m_wndComponentTreeCtrl, TREE_CTRL_UPDATE_TEXT);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnComponentSearch()
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }

	CThisTreeCtrl &TreeCtrl= m_wndComponentTreeCtrl;	

	CString    str, str2;
	CString    strCaption;
	CString    strValue;
	CString    strLabel;
	CInputBoxWnd  InputBox;	
	CAOIPanel   *PanelPtr = NULL;
	CAOIBoard   *BoardPtr = NULL;
	CAOIComponent *ComponentPtrAct = ProjectPtr->GetProjectActiveComponent();
	unsigned int ComponentIndex=0;
	unsigned int PanelIndex = ProjectPtr->GetProjectActivePanelIndex();
	unsigned int BoardIndex = ProjectPtr->GetProjectActiveBoardIndex();
	const size_t PanelCount = ProjectPtr->GetProjectPanelCount();
	
	if ( NULL != ComponentPtrAct )
	{
		PanelPtr = ComponentPtrAct->GetComponentPanelPtr();
		BoardPtr = ComponentPtrAct->GetComponentBoardPtr();
		if ( NULL != PanelPtr )
		{	PanelIndex = PanelPtr->GetPanelIndex_Project(); }
		if ( NULL != BoardPtr )
		{	BoardIndex = BoardPtr->GetBoardIndex_Panel(); }
	}

	if ( NULL == PanelPtr )
	{
		if ( PanelCount > 1 ) 
		{
			strCaption = _T("Select Panel");
			strCaption = LoadMultiLanguageString(strCaption, strCaption);
			strLabel = _T("Panel:");
			strValue.Format(_T("%d"), PanelIndex+1);
			InputBox.SetParam1(strCaption, strLabel, strValue);
			if ( InputBox.DoModal() == IDCANCEL ) { return ; }
			strValue = InputBox.m_DataEdit1;
			PanelIndex = JetAPI::StrToInt(strValue)-1;				
		}
		PanelPtr = ProjectPtr->GetProjectPanelPtr(PanelIndex, true);
		if ( NULL == PanelPtr ) { return; }	
	}

	if ( NULL == BoardPtr )
	{
		BoardPtr = ProjectPtr->GetProjectBoardPtr(BoardIndex, true);
		const size_t BoardCount = PanelPtr->GetPanelBoardCount();
		if ( BoardCount > 1 )
		{
			strCaption = _T("Select Board");
			strCaption = LoadMultiLanguageString(strCaption, strCaption);
			strLabel = _T("Board:");
			BoardIndex = BoardPtr->GetBoardIndex_Panel();
			strValue.Format(_T("%d"), BoardIndex+1);
			InputBox.SetParam1(strCaption, strLabel, strValue);
			if ( InputBox.DoModal() == IDCANCEL ) { return ; }
			strValue = InputBox.m_DataEdit1;
			BoardIndex = JetAPI::StrToInt(strValue)-1;
			BoardPtr = PanelPtr->GetPanelBoardPtr(BoardIndex, true);
		}
		if ( NULL == BoardPtr ) { return ; }	
	}	

	strCaption = _T("Search Component");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strLabel = AOIDataDefine.GetNameText();
	InputBox.SetParam1(strCaption, strLabel, strValue);
	if ( InputBox.DoModal() == IDCANCEL ) { return ; }
	strValue = InputBox.m_DataEdit1;
	CAOIComponent *ComponentPtr = BoardPtr->GetBoardComponentPtrByName(strValue);
	if ( NULL == ComponentPtr )
	{
		str = _T("Can not find the component");
		str = LoadMultiLanguageString(str, str);
		str2.Format(_T("%s[%s]"), str, strValue);
		JetAPI::ShowMessageBox(str2);
		return;
	}
	
	m_MoveToComponent = false;
	ProjectPtr->SelectProjectAllComponents(false);
	ComponentPtr->SetComponentSelected(true);
	ProjectPtr->SetProjectActiveComponent(ComponentPtr);
	ShowComponentTreeSelected(m_wndComponentTreeCtrl);
	m_MoveToComponent = true;
	AOIDataCollect.MoveStageToComponentOrField(ComponentPtr, true);
	AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_UPDATE, (LPARAM)(this));	
	return;
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnComponentSelectAll()
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	CThisTreeCtrl &TreeCtrl= m_wndComponentTreeCtrl;
	CAOIComponent *ComponentPtr = ProjectPtr->GetProjectActiveComponent();
	if ( NULL == ComponentPtr ) { return; }
	CAOIBoard *BoardPtr = ComponentPtr->GetComponentBoardPtr();
	if ( NULL == BoardPtr ) { return; }

	ProjectPtr->SelectProjectAllComponents(false);
	BoardPtr->SelectBoardAllComponents(true);
	UpdateComponentTreeCtrl(m_wndComponentTreeCtrl, TREE_CTRL_UPDATE_STATE);
	return;
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnComponentBypass()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	
	if ( AOIDataCollect.OperateLevelEditFuncBypassComponent() == false )	{	return ; }
	ProjectPtr->SwitchProjectComponentBypassed();
	LogOperCtrl.SaveLogProjectComponentSelectedBypassed(ProjectPtr);
	UpdateComponentTreeCtrlState(m_wndComponentTreeCtrl, TREE_CTRL_UPDATE_STATE);
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnComponentXBoardUnit()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	ProjectPtr->SwitchProjectComponentXBoardUnit();
	LogOperCtrl.SaveLogProjectComponentSelectedXBoardUnit(ProjectPtr);	
	UpdateComponentTreeCtrlState(m_wndComponentTreeCtrl, TREE_CTRL_UPDATE_STATE);
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnComponentModelIsolated()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	
	CAOIComponent *ComponentPtr = ProjectPtr->GetProjectActiveComponent();
	if ( NULL == ComponentPtr ) { return; }
	if ( AOIDataCollect.OperateLevelEditFuncAddModel() == false ) {	return ; }

	CString str;
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
	UpdateComponentTreeCtrlState(m_wndComponentTreeCtrl, TREE_CTRL_UPDATE_STATE);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_WND_PROPERTY_WND, WPARAM_BUILD_WND_SELECTED, NULL);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnComponentRestoreCadPos()
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
	UpdateComponentTreeCtrlState(m_wndComponentTreeCtrl, TREE_CTRL_UPDATE_STATE);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnComponentBypass3D()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	
	if ( AOIDataCollect.OperateLevelEditFuncBypassComponent() == false )	{	return ; }
	ProjectPtr->SwitchProjectComponentBypass3D();
	LogOperCtrl.SaveLogProjectComponentSelectedBypass3D(ProjectPtr);
	UpdateComponentTreeCtrlState(m_wndComponentTreeCtrl, TREE_CTRL_UPDATE_STATE);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnFdDelete() 
{
	// TODO: Add your command handler code here
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	if ( NULL == m_TreeItemSelected ) { return; }
	if ( AOIDataCollect.OperateLevelEditFuncDelFd() == false ) { return ; }

	CThisTreeCtrl &TreeCtrl = m_wndComponentTreeCtrl;
	unsigned int ItemType=0, ItemIndex=0;
	DWORD_PTR    ItemData = TreeCtrl.GetItemData(m_TreeItemSelected);
	::DecodeItemlParam(ItemData, ItemType, ItemIndex);	
	if ( TREE_ITEM_TYPE_FD != ItemType ) { return; }

	CAOIFd *FdPtr = NULL;
	FdPtr = ProjectPtr->GetProjectFdPtr(ItemIndex, true);
	if ( NULL == FdPtr ) { return; }
	AOIDataCollect.ReleaseModelUniFrameList();
	ProjectPtr->SelectProjectAllFds(false);
	FdPtr->SetFdSelected(true);
	ProjectPtr->ResetProjectActiveIndex();
	ProjectPtr->DeleteProjectFdSelected();

	BuildComponentTreeCtrl(TreeCtrl);
	//TreeCtrl.DeleteItem(m_TreeItemSelected);
	//m_TreeItemSelected = NULL;
	//UpdateComponentTreeCtrlState(TreeCtrl, TREE_CTRL_UPDATE_ALL);
	AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_PART_DELETED, (WPARAM)this);
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnFdToTeachPos()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	if ( NULL == m_TreeItemSelected ) { return; }

	CThisTreeCtrl &TreeCtrl = m_wndComponentTreeCtrl;
	unsigned int ItemType=0, ItemIndex=0;
	DWORD_PTR    ItemData = TreeCtrl.GetItemData(m_TreeItemSelected);
	::DecodeItemlParam(ItemData, ItemType, ItemIndex);	
	if ( TREE_ITEM_TYPE_FD != ItemType ) { return; }

	size_t   i=0;	
	CAOIFd *FdPtr = NULL;
	const size_t FdCount = ProjectPtr->GetProjectFdCount();
	FdPtr = ProjectPtr->GetProjectFdPtr(ItemIndex, true);
	if ( NULL == FdPtr ) { return; }

	TREGION4D     StageRgn;
	const LANE_ID LaneID = AOIDataCollect.GetActiveLaneID();
	const double  FdSizeW = FdPtr->GetFdBodySizeW();
	const double  FdSizeH = FdPtr->GetFdBodySizeH();
	TPOINT3D      StagePos = FdPtr->GetFdTeachStagePos();			
	AOIDataCollect.MapStagePosLaneByLaneID(StagePos.x, StagePos.y, LaneID);
	StageRgn.SetRgn(StagePos.x, StagePos.y, FdSizeW, FdSizeH);	
	AOIDataCollect.MoveStageTo(StagePos.x, StagePos.y, StageRgn);
	return;
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnFdSetTeachPos() 
{
	// TODO: Add your command handler code here
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	if ( NULL == m_TreeItemSelected ) { return; }

	CThisTreeCtrl &TreeCtrl = m_wndComponentTreeCtrl;
	unsigned int ItemType=0, ItemIndex=0;
	DWORD_PTR    ItemData = TreeCtrl.GetItemData(m_TreeItemSelected);
	::DecodeItemlParam(ItemData, ItemType, ItemIndex);	
	if ( TREE_ITEM_TYPE_FD != ItemType ) { return; }

	size_t   i=0;	
	CString  str, str2, strLane;
	TPOINT3D NewStagePos;	
	TPOINT3D OldStagePos;
	TPOINT3D StageOffset;
	TPOINT3D LaneStagePos;
	CAOIFd *FdPtr = NULL;
	const LANE_ID LaneID = AOIDataCollect.GetActiveLaneID();
	const size_t  FdCount = ProjectPtr->GetProjectFdCount();
	const bool    bMultiDistrictMode = ProjectPtr->GetProjectMultiDistrictMode();
	FdPtr = ProjectPtr->GetProjectFdPtr(ItemIndex, true);
	if ( NULL == FdPtr ) { return; }

	strLane = AOIDataDefine.GetLaneIDText(LaneID);
	str = _T("Do you want to set current position to be the fd teach position?");
	str = LoadMultiLanguageString(str, str);
	str2.Format(_T("%s [%s]"), str, strLane);
	if ( JetAPI::ShowMessageBox(str2, MB_YESNO) == IDNO ) { return; }

	MotionCtrlPtr->GetCurrentPos(NewStagePos.x, NewStagePos.y, NewStagePos.z);
	OldStagePos = FdPtr->GetFdTeachStagePos();
	AOIDataCollect.MapStagePosLaneByLaneID(OldStagePos.x, OldStagePos.y, LaneID);
	StageOffset.x = NewStagePos.x - OldStagePos.x;
	StageOffset.y = NewStagePos.y - OldStagePos.y;
	StageOffset.z = NewStagePos.z - OldStagePos.z;

	TREGION4D MapStageRgn;
	TREGION4D MapTeachRgn;
	TREGION4D LocStageRgn;
	ProjectPtr->GetProjectMapStageRgn_DA(MapStageRgn);
	ProjectPtr->GetProjectMapTeachRgn_DA(MapTeachRgn);
	ProjectPtr->GetProjectMapLocStageRgn_DA(LocStageRgn);
	MapStageRgn.Move(StageOffset.x, StageOffset.y);
	MapTeachRgn.Move(StageOffset.x, StageOffset.y);
	LocStageRgn.Move(StageOffset.x, StageOffset.y);
	ProjectPtr->SetProjectMapStageRgn_DA(MapStageRgn);
	ProjectPtr->SetProjectMapTeachRgn_DA(MapTeachRgn);
	ProjectPtr->SetProjectMapLocStageRgn_DA(LocStageRgn);
	if ( true == bMultiDistrictMode )
	{
		ProjectPtr->GetProjectMapStageRgn_DB(MapStageRgn);
		ProjectPtr->GetProjectMapTeachRgn_DB(MapTeachRgn);
		ProjectPtr->GetProjectMapLocStageRgn_DB(LocStageRgn);
		MapStageRgn.Move(StageOffset.x, StageOffset.y);
		MapTeachRgn.Move(StageOffset.x, StageOffset.y);
		LocStageRgn.Move(StageOffset.x, StageOffset.y);
		ProjectPtr->SetProjectMapStageRgn_DB(MapStageRgn);
		ProjectPtr->SetProjectMapTeachRgn_DB(MapTeachRgn);
		ProjectPtr->SetProjectMapLocStageRgn_DB(LocStageRgn);
	}

	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = ProjectPtr->GetProjectFdPtr(i, false);
		if ( NULL == FdPtr ) { continue; }
		OldStagePos = FdPtr->GetFdTeachStagePos();		

		NewStagePos.x = StageOffset.x + OldStagePos.x;
		NewStagePos.y = StageOffset.y + OldStagePos.y;
		NewStagePos.z = StageOffset.z + OldStagePos.z;

		FdPtr->SetFdLaneID(LaneID);
		FdPtr->SetFdTeachStagePosX(NewStagePos.x);
		FdPtr->SetFdTeachStagePosY(NewStagePos.y);		
		//FdPtr->SetFdTeachStagePosZ(NewStagePos.z);		

		LaneStagePos = NewStagePos;
		AOIDataCollect.MapStagePosLaneByLaneID(LaneStagePos.x, LaneStagePos.y, LaneID);
		FdPtr->SetFdStagePosX(LaneStagePos.x);
		FdPtr->SetFdStagePosY(LaneStagePos.y);				
		FdPtr->LayoutFdStageCornerPos();		
	}
	AOIDataCollect.SetIsNeedGrabFiducial(true, LANE_ID_A);
	AOIDataCollect.SetIsNeedGrabFiducial(true, LANE_ID_B);
	FdPtr = ProjectPtr->GetProjectFdPtr(ItemIndex, true);
	if ( NULL == FdPtr ) { return; }		
	this->BuildInfoPropCtrl_Fd(m_wndInfomationPropCtrl, FdPtr);	
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnFdToCurrentPos()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	if ( NULL == m_TreeItemSelected ) { return; }

	CThisTreeCtrl &TreeCtrl = m_wndComponentTreeCtrl;
	unsigned int ItemType=0, ItemIndex=0;
	DWORD_PTR    ItemData = TreeCtrl.GetItemData(m_TreeItemSelected);
	::DecodeItemlParam(ItemData, ItemType, ItemIndex);	
	if ( TREE_ITEM_TYPE_FD != ItemType ) { return; }

	size_t   i=0;	
	CAOIFd *FdPtr = NULL;
	const size_t FdCount = ProjectPtr->GetProjectFdCount();
	FdPtr = ProjectPtr->GetProjectFdPtr(ItemIndex, true);
	if ( NULL == FdPtr ) { return; }

	TREGION4D StageRgn;	
	TPOINT3D  StagePos = FdPtr->GetFdStagePos();
	const double  FdSizeW = FdPtr->GetFdBodySizeW();
	const double  FdSizeH = FdPtr->GetFdBodySizeH();	
	StageRgn.SetRgn(StagePos.x, StagePos.y, FdSizeW, FdSizeH);	
	AOIDataCollect.MoveStageTo(StagePos.x, StagePos.y, StageRgn);
	return;
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnFdUpdateToOthers()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	if ( NULL == m_TreeItemSelected ) { return; }

	CThisTreeCtrl &TreeCtrl = m_wndComponentTreeCtrl;
	unsigned int ItemType=0, ItemIndex=0;
	DWORD_PTR    ItemData = TreeCtrl.GetItemData(m_TreeItemSelected);
	::DecodeItemlParam(ItemData, ItemType, ItemIndex);	
	if ( TREE_ITEM_TYPE_FD != ItemType ) { return; }
	
	CAOIFd *FdPtr = ProjectPtr->GetProjectFdPtr(ItemIndex, true);
	if ( NULL == FdPtr ) { return; }

	CString str;
	str = _T("Do you want to update to other Fds?");
	str = this->LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) != IDYES )
	{	return; }

	ProjectPtr->UpdateProjectFdToOtherByGroupID(FdPtr);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_REDRAW_VIEW_WND, NULL);
	return;
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnComponentMaskBaseSetColorIndex()
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
		ColorGroupPtr = ProjectPtr->GetProjectColorGroupPtr(i, true);
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
void CEditComponentListDockPane::OnComponentSpaceNoiseFilter()
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
void CEditComponentListDockPane::OnComponentMaskExtendSizeBody()
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
void CEditComponentListDockPane::OnComponentEnableAlarmAOI()
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
void CEditComponentListDockPane::OnComponentGroupID()
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

	strCaption = strCaption;
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnComponentGroupOrg()
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
void CEditComponentListDockPane::OnComponentToFieldPos()
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
void CEditComponentListDockPane::OnComponentEnableSelfField()
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
void CEditComponentListDockPane::OnComponentCloneNewModel()
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
	if ( AOIDataCollect.OperateLevelEditFuncAddModel() == false ) {	return ; }

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
void CEditComponentListDockPane::OnComponentChangeBoard()
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
	BuildComponentTreeCtrl(m_wndComponentTreeCtrl);	
	ShowComponentTreeSelected(m_wndComponentTreeCtrl);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);
	return;
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnComponentLocalBasePlaneID()
{	
	if ( GetLockUIWnd() == true )
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
	BuildInfoPropCtrl_Component(m_wndInfomationPropCtrl, ComponentPtr);
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnComponentDataModelParam()
{	
	if ( GetLockUIWnd() == true )
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
	BuildInfoPropCtrl_Component(m_wndInfomationPropCtrl, ComponentPtr);
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnComponentSaveWndList()
{	
	if ( GetLockUIWnd() == true )
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
	BuildInfoPropCtrl_Component(m_wndInfomationPropCtrl, ComponentPtr);
	return;
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnComponentFeedbackResultPos()
{	
	if ( GetLockUIWnd() == true )
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
	UpdateComponentTreeCtrlState(m_wndComponentTreeCtrl, TREE_CTRL_UPDATE_STATE);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnBarcodeDelete()
{
	bool IsOK = ExecBarcodeDelete();	
	return ;
}
//-------------------------------------------------------------------------------------//
bool CEditComponentListDockPane::ExecBarcodeDelete()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return true; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }
	if ( AOIDataCollect.OperateLevelEditFuncDelBarcode() == false ) { return false; }

	std::vector<CAOIBarcode*> SelBarcodeList;
	ProjectPtr->GetProjectBarcodeSelected(SelBarcodeList);	
	const size_t SelBarcodeCount = SelBarcodeList.size();
	if ( 0 == SelBarcodeCount ) { return true; }
	CAOIBarcode *BarcodePtr = ProjectPtr->GetProjectActiveBarcode();
	if ( NULL == BarcodePtr ) { return true; }
	CAOIPanel *PanelPtr = BarcodePtr->GetBarcodePanelPtr();
	CAOIBoard *BoardPtr = BarcodePtr->GetBarcodeBoardPtr();

	CString str, str1, str2;
	CString strPanel = AOIDataDefine.GetPanelText();
	CString strBoard = AOIDataDefine.GetBoardText();
	CString strCount = AOIDataDefine.GetCountText();
	str1 = _T("Do you want to delete the selected barcodes");
	str1 = LoadMultiLanguageString(str1, str1);
	if ( SelBarcodeCount > 1 ) 
	{	str.Format(_T("%s [%s:%d]?"), str1, strCount, SelBarcodeCount); }
	else
	{			
		unsigned int PanelIndex = -1;
		unsigned int BoardIndex = -1;		
		unsigned int BarcodeIndex = -1;
		BarcodeIndex = BarcodePtr->GetBarcodeIndex_Project();
		if ( NULL != PanelPtr ) 
		{	
			PanelIndex = PanelPtr->GetPanelIndex_Project();	
			if ( NULL != BoardPtr ) 
			{	
				BoardIndex = BoardPtr->GetBoardIndex_Panel();	
				str2.Format(_T("%s-%d, %s-%d"), strPanel, PanelIndex+1, strBoard, BoardIndex+1);
			}		
			else
			{	str2.Format(_T("%s-%d"), strPanel, PanelIndex+1);	}	
		}
		else
		{	str2.Format(_T("%d"), BarcodeIndex+1); }
		str.Format(_T("%s [%s]?"), str1, str2);
	}
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) != IDYES )
	{	return true; }
	AOIDataCollect.ReleaseModelUniFrameList();
	//RemoveTreeItemComponentSelected(m_wndComponentTreeCtrl);//使用後Index會對不起來
	if ( NULL != PanelPtr ) 
	{	ProjectPtr->SetProjectActivePanel(PanelPtr);	}
	if ( NULL != BoardPtr )
	{	ProjectPtr->SetProjectActiveBoard(BoardPtr); }
	LogOperCtrl.SaveLogProjectBarcodeSelectedDelete(ProjectPtr);	
	ProjectPtr->DeleteProjectBarcodeSelected();	
	BuildComponentTreeCtrl(m_wndComponentTreeCtrl);	
	ShowComponentTreeSelected(m_wndComponentTreeCtrl);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnBarcodeRotateAny()
{
	bool IsOK = ExecBarcodeRotateAny();	
	return ;
}
//-------------------------------------------------------------------------------------//
bool CEditComponentListDockPane::ExecBarcodeRotateAny()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return true; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }

	CString      strValue;
	CString      strLabel;
	CString      strCaption;
	CString      strAngle = _T("0");	
	CInputBoxWnd InputBox;
	const double DBL_Precesion = DBL_PRECISION;
	const size_t BarcodeSelCount = ProjectPtr->GetProjectBarcodeSelectedCount();
	if ( 0 == BarcodeSelCount ) { return true; }

	strCaption = _T("Set Rotation Angle Wnd");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strLabel = _T("Rotation Angle:");
	strLabel = LoadMultiLanguageString(strLabel, strLabel);
	InputBox.SetParam1(strCaption, strLabel, strAngle);
	if ( InputBox.DoModal() == IDCANCEL ) { return true; }
	strAngle = InputBox.m_DataEdit1;
	const double RotateAngle = ::_tcstod(strAngle, NULL);	
	if ( fabs(RotateAngle)<DBL_Precesion ) { return true; }
	return ExecBarcodeRotation(RotateAngle);
}
//-------------------------------------------------------------------------------------//
bool CEditComponentListDockPane::ExecBarcodeRotation(double Angle)
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return true; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }
	const size_t BarcodeSelCount = ProjectPtr->GetProjectBarcodeSelectedCount();
	if ( 0 == BarcodeSelCount ) { return false; }
	
	ProjectPtr->RotateProjectBarcodeSelected(Angle);
	LogOperCtrl.SaveLogProjectBarcodeSelectedRotate(ProjectPtr, Angle);		
	UpdateComponentTreeCtrlState(m_wndComponentTreeCtrl, TREE_CTRL_UPDATE_TEXT);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnBarcodeUpdateToOthers()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	if ( NULL == m_TreeItemSelected ) { return; }

	CThisTreeCtrl &TreeCtrl = m_wndComponentTreeCtrl;
	unsigned int ItemType=0, ItemIndex=0;
	DWORD_PTR    ItemData = TreeCtrl.GetItemData(m_TreeItemSelected);
	::DecodeItemlParam(ItemData, ItemType, ItemIndex);	
	if ( TREE_ITEM_TYPE_BARCODE != ItemType ) { return; }
	
	CAOIBarcode *BarcodePtr = ProjectPtr->GetProjectBarcodePtr(ItemIndex, true);
	if ( NULL == BarcodePtr ) { return; }

	CString str;
	str = _T("Do you want to update to other Barcodes?");
	str = this->LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) != IDYES )
	{	return; }

	ProjectPtr->UpdateProjectBarcodeToOtherByGroupID(BarcodePtr);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_REDRAW_VIEW_WND, NULL);
	return;
}
//-------------------------------------------------------------------------------------//
bool CEditComponentListDockPane::ExecMarkDelete()
{
	if ( GetLockUIWnd() == true )
	{	return true; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }
	if ( AOIDataCollect.OperateLevelEditFuncDelMark() == false ) { return false; }

	std::vector<CAOIMark*> SelMarkList;
	ProjectPtr->GetProjectMarkSelected(SelMarkList);	
	const size_t SelMarkCount = SelMarkList.size();
	if ( 0 == SelMarkCount ) { return true; }
	CAOIMark *MarkPtr = ProjectPtr->GetProjectActiveMark();
	if ( NULL == MarkPtr ) { return true; }
	CAOIPanel *PanelPtr = MarkPtr->GetMarkPanelPtr();
	CAOIBoard *BoardPtr = MarkPtr->GetMarkBoardPtr();

	CString str, str1, str2;
	CString strPanel = AOIDataDefine.GetPanelText();
	CString strBoard = AOIDataDefine.GetBoardText();
	CString strCount = AOIDataDefine.GetCountText();
	str1 = _T("Do you want to delete the selected marks");
	str1 = LoadMultiLanguageString(str1, str1);
	if ( SelMarkCount > 1 ) 
	{	str.Format(_T("%s [%s:%d]?"), str1, strCount, SelMarkCount); }
	else
	{			
		unsigned int MarkIndex = -1;
		unsigned int PanelIndex = -1;
		unsigned int BoardIndex = -1;
		MarkIndex = MarkPtr->GetMarkIndex_Project();
		if ( NULL != PanelPtr ) 
		{	
			PanelIndex = PanelPtr->GetPanelIndex_Project();	
			if ( NULL != BoardPtr ) 
			{	
				BoardIndex = BoardPtr->GetBoardIndex_Panel();	
				str2.Format(_T("%s-%d, %s-%d"), strPanel, PanelIndex+1, strBoard, BoardIndex+1);
			}		
			else
			{	str2.Format(_T("%s-%d"), strPanel, PanelIndex+1);	}	
		}
		else
		{	str2.Format(_T("%d"), MarkIndex+1); }
		str.Format(_T("%s [%s]?"), str1, str2);
	}
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) != IDYES )
	{	return true; }
	AOIDataCollect.ReleaseModelUniFrameList();
	//RemoveTreeItemComponentSelected(m_wndComponentTreeCtrl);//使用後Index會對不起來
	if ( NULL != PanelPtr ) 
	{	ProjectPtr->SetProjectActivePanel(PanelPtr);	}
	if ( NULL != BoardPtr )
	{	ProjectPtr->SetProjectActiveBoard(BoardPtr); }
	ProjectPtr->DeleteProjectMarkSelected();	
	BuildComponentTreeCtrl(m_wndComponentTreeCtrl);	
	ShowComponentTreeSelected(m_wndComponentTreeCtrl);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);	
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnMarkDelete()
{
	bool IsOK = ExecMarkDelete();
	return;
}
//-------------------------------------------------------------------------------------//
bool CEditComponentListDockPane::ExecMarkBypass()
{
	if ( GetLockUIWnd() == true )
	{	return true; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }
	if ( AOIDataCollect.OperateLevelEditFuncBypassMark() == false )	{	return false; }

	std::vector<CAOIMark*> SelMarkList;
	ProjectPtr->GetProjectMarkSelected(SelMarkList);	
	const size_t SelMarkCount = SelMarkList.size();
	if ( 0 == SelMarkCount ) { return true; }
	CAOIMark *MarkPtr = ProjectPtr->GetProjectActiveMark();
	if ( NULL == MarkPtr ) { return true; }	
	ProjectPtr->BypassProjectMarkSelected();	
	UpdateComponentTreeCtrlState(m_wndComponentTreeCtrl, TREE_CTRL_UPDATE_STATE);		
	return true;
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnMarkBypass()
{
	bool IsOK = ExecMarkBypass();
	return;
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnMarkSpaceNoiseFilter()
{
	if ( GetLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	
	CAOIMark *MarkPtr = ProjectPtr->GetProjectActiveMark();
	if ( NULL == MarkPtr ) { return ; }
	CAOIModel   *ModelPtr = MarkPtr->GetMarkModelPtr();
	if ( NULL == ModelPtr )	{	return; }	

	const bool bClone = false;
	CSpaceNoiseFilterParamWnd Wnd;	
	TNoiseFilterParam NoiseFilterParam;
	std::vector<TUNI_FRAME> UniFrameList;	
	unsigned int MapIndex = ProjectPtr->GetProjectMapIndex();	
	AOIDataCollect.ReleaseModelUniFrameList();
	AOIDataCollect.SendMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_BUILD_RAW_MODEL_UNI_FRAME_LIST, (LPARAM)(ModelPtr));
	AOIDataCollect.CopyModelUniFrameList(UniFrameList, bClone);	
	NoiseFilterParam = MarkPtr->GetMarkSpaceNoiseFilterParam();		
	Wnd.SetNoiseFilterParam(NoiseFilterParam);	
	Wnd.SetModelUniFrameList(MapIndex, UniFrameList);
	if ( Wnd.DoModal() == IDCANCEL )
	{	
		AOIDataCollect.ReleaseModelUniFrameList();//需要重新建立，因此釋放模組的圖像資料
		AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_PART_SELECTED, NULL);	
		return ; 
	}		
	Wnd.GetNoiseFilterParam(NoiseFilterParam);	
	ProjectPtr->SetProjectMarkSelectedSpaceNoiseFilterParam(NoiseFilterParam);
	if ( -1 != NoiseFilterParam.DataFilterIndex )
	{	ProjectPtr->UpdateProjectSpaceNoiseFilterParam(NoiseFilterParam); }	
	AOIDataCollect.ReleaseModelUniFrameList();//需要重新建立，因此釋放模組的圖像資料
	AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_PART_SELECTED, NULL);	
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnMarkSpaceBasePlane()
{
	if ( GetLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }		
	CAOIMark *MarkPtr = ProjectPtr->GetProjectActiveMark();
	if ( NULL == MarkPtr ) { return ; }
	
	CSpaceBaseParamWnd ParamWnd;
	int  nBaseColoeIndex=0;
	bool bBaseColorEnabled=false;	
	TBasePlaneParam BasePlaneParam;
	
	BasePlaneParam = MarkPtr->GetMarkSpaceBasePlaneParam();
	bBaseColorEnabled = MarkPtr->GetMarkMaskEnable_Base();
	nBaseColoeIndex   = MarkPtr->GetMarkMaskColorGroupLinkIndex();	
	if ( nBaseColoeIndex >= 0 ) { nBaseColoeIndex -= PROJECT_COLOR_ID_BOARD_BEGIN; }

	ParamWnd.SetProjectPtr(ProjectPtr);
	ParamWnd.SetBasePlaneParam(BasePlaneParam);
	ParamWnd.SetBaseColorIndex(nBaseColoeIndex);
	ParamWnd.SetBaseColorEnabled(bBaseColorEnabled);	
	if ( ParamWnd.DoModal() == IDCANCEL )
	{	return ; }	

	ParamWnd.GetBasePlaneParam(BasePlaneParam);
	nBaseColoeIndex = ParamWnd.GetBaseColorIndex();
	bBaseColorEnabled = ParamWnd.GetBaseColorEnabled();
	const bool bParamSetting = ParamWnd.GetBasePlaneParamSetting();
	const bool bColorSetting = ParamWnd.GetBasePlaneColorSetting();

	if ( true == bParamSetting )
	{	
		ProjectPtr->SetProjectMarkSelectedSpaceBasePlaneParam(BasePlaneParam); 
		if ( -1 != BasePlaneParam.BasePlaneIndex )
		{	ProjectPtr->UpdateProjectSpaceBasePlaneParam(BasePlaneParam); }	
	}

	if ( true == bColorSetting ) 
	{
		if ( false==bBaseColorEnabled || -1==nBaseColoeIndex )
		{	ProjectPtr->EnableProjectMarkSelectedMaskFunc_Base(false);	}
		else
		{
			nBaseColoeIndex += PROJECT_COLOR_ID_BOARD_BEGIN;
			ProjectPtr->EnableProjectMarkSelectedMaskFunc_Base(true);
			ProjectPtr->SetProjectMarkSelectedMaskColorIndex_Base(nBaseColoeIndex); 
		}
	}
	AOIDataCollect.ReleaseModelUniFrameList();//需要重新建立，因此釋放模組的圖像資料
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnMarkPasteToOtherBoard()
{
	if ( GetLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }

	CAOIMark *MarkPtr = ProjectPtr->GetProjectActiveMark();
	if ( NULL == MarkPtr ) { return ; }
	CString str;
	str = _T("Do you want to paste the mark to other boards?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO )
	{	return ; }

	std::vector<CAOIMark*> MarkList;
	ProjectPtr->GetProjectMarkSelected(MarkList);	
	ProjectPtr->PasteProjectMarkToOtherBoards(MarkList);	
	BuildComponentTreeCtrl(m_wndComponentTreeCtrl);	
	ShowComponentTreeSelected(m_wndComponentTreeCtrl);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);	
	return;
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnMarkLocalBasePlaneID()
{
	if ( GetLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	
	CAOIMark *MarkPtr = ProjectPtr->GetProjectActiveMark();
	if ( NULL == MarkPtr )	{	return; }	
	
	DWORD        Res=0;
	CString      str;
	CString      strValue;
	CString      strLabel;
	CString      strCaption;
	CInputBoxWnd InputBox;
	const int    BasePlaneID = MarkPtr->GetMarkLocalBasePlaneID();
	str = _T("Set Local Base Plane ID");
	strCaption = LoadMultiLanguageString(str, str);
	str = _T("Local Base Plane ID (0:Disable)");
	strLabel = LoadMultiLanguageString(str, str);
	strValue.Format(_T("%d"), BasePlaneID);
	InputBox.SetParam1(strCaption, strLabel, strValue);
	if ( InputBox.DoModal() == IDCANCEL )
	{	return; }
	const int NewBasePlaneID=::_ttoi(InputBox.m_DataEdit1);
	ProjectPtr->SetProjectMarkSelectedLocalBasePlaneID(NewBasePlaneID);
	BuildInfoPropCtrl_Mark(m_wndInfomationPropCtrl, MarkPtr);
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditComponentListDockPane::OnMarkUpdateToOthers()
{
	if ( GetLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	if ( NULL == m_TreeItemSelected ) { return; }

	CThisTreeCtrl &TreeCtrl = m_wndComponentTreeCtrl;
	unsigned int ItemType=0, ItemIndex=0;
	DWORD_PTR    ItemData = TreeCtrl.GetItemData(m_TreeItemSelected);
	::DecodeItemlParam(ItemData, ItemType, ItemIndex);	
	if ( TREE_ITEM_TYPE_MARK != ItemType ) { return; }
	
	CAOIMark *MarkPtr = ProjectPtr->GetProjectMarkPtr(ItemIndex, true);
	if ( NULL == MarkPtr ) { return; }

	CString str;
	str = _T("Do you want to update to other Marks?");
	str = this->LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) != IDYES )
	{	return; }

	ProjectPtr->UpdateProjectMarkToOtherByGroupID(MarkPtr);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_REDRAW_VIEW_WND, NULL);
	return;
}
//-------------------------------------------------------------------------------------//
