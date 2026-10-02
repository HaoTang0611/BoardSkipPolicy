// EditResultDockPane.cpp : 實作檔
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "EditResultDockPane.h"
//-------------------------------------------------------------------------------------//
#include "InputBoxWnd.h"
#include "InputListWnd.h"
#include "SpaceBaseParamWnd.h"
#include "ComponentDefectAlarmWnd.h"
#include "SpaceNoiseFilterParamWnd.h"
//-------------------------------------------------------------------------------------//
#define HUGE_LIST_ITEM_COUNT      256
#define LIST_ITEM_PATCH_N_PAGE_VALUE     10   //一次多少個頁面
#define LIST_ITEM_PATCH_ENABLE_COUNT   1000 //啟用零件數
//-------------------------------------------------------------------------------------//
const bool bShowSelCol = false;//顯示選中的
//-------------------------------------------------------------------------------------//
//const UINT ID_DEFECT_COMPONENT_LIST = 100;//AFX_IDW_PANE_FIRST;
//const UINT ID_DEFECT_GROUP_LIST = 200;//AFX_IDW_PANE_FIRST;
//const UINT ID_DEFECT_WND_LIST   = 300;//AFX_IDW_PANE_FIRST;
const UINT ID_DEFECT_COMPONENT_LIST = CPageSplitterWnd::GetIdFromRowCol(0, 0);//AFX_IDW_PANE_FIRST;
const UINT ID_DEFECT_GROUP_LIST     = CPageSplitterWnd::GetIdFromRowCol(1, 0);//AFX_IDW_PANE_FIRST+16;//注意行列會不同唷
const UINT ID_DEFECT_WND_LIST       = CPageSplitterWnd::GetIdFromRowCol(2, 0);//AFX_IDW_PANE_FIRST+16;//注意行列會不同唷
//-------------------------------------------------------------------------------------//
// CEditResultDockPane
//-------------------------------------------------------------------------------------//
IMPLEMENT_DYNAMIC(CEditResultDockPane, CDockablePane)
//-------------------------------------------------------------------------------------//
CEditResultDockPane::CEditResultDockPane()
{
	m_ProjectPtr = NULL;
	m_ComponentPtrAct = NULL;
	m_StopDefectListBeSelected = false;
	m_StopDefectWndListBeSelected = false;
	m_StopDefectGroupListBeSelected = false;

	m_SelText = _T("**");
	m_clrOK = 0x008000;
	m_clrNG = 0x000080;
	m_clrBypass = 0x800000;
	m_clrUnTest = 0x808080;
	m_MoveToComponent = true;
}
//-------------------------------------------------------------------------------------//
CEditResultDockPane::~CEditResultDockPane()
{
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CEditResultDockPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_CONTEXTMENU()
	ON_NOTIFY(NM_CLICK, ID_DEFECT_COMPONENT_LIST, OnClickDefectListCtrl)
	ON_NOTIFY(NM_DBLCLK, ID_DEFECT_COMPONENT_LIST, OnDbclickDefectListCtrl)	
	ON_NOTIFY(LVN_ITEMCHANGED, ID_DEFECT_COMPONENT_LIST, OnItemchangedDefectListCtrl)
	ON_NOTIFY(LVN_ITEMCHANGED, ID_DEFECT_GROUP_LIST, OnItemchangedDefectGroupListCtrl)
	ON_NOTIFY(LVN_ITEMCHANGED, ID_DEFECT_WND_LIST, OnItemchangedDefectWndListCtrl)		
	ON_NOTIFY(NM_DBLCLK, ID_DEFECT_GROUP_LIST, OnDblclkDefectGroupListCtrl)
	ON_NOTIFY(NM_DBLCLK, ID_DEFECT_WND_LIST, OnDblclkDefectWndListCtrl)
	ON_NOTIFY(LVN_ENDSCROLL, ID_DEFECT_COMPONENT_LIST, OnEndScrollDefectListCtrl)
	ON_BN_CLICKED(ERPB_SWITCH_PREVIOUS_BTN, OnSwitchPreiousBtn)
	ON_BN_CLICKED(ERPB_SWITCH_NEXT_BTN, OnSwitchNextBtn)
	ON_BN_CLICKED(ERPB_COMPONENT_RETEST_BTN, OnComponentRetestBtn)
	ON_BN_CLICKED(ERPB_SHOW_PASS_CHK, OnShowPassChk)	
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
// CEditResultDockPane 訊息處理常式
int CEditResultDockPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此加入特別建立的程式碼
	DWORD dwBarStyle = CBRS_LEFT|CBRS_TOOLTIPS|CBRS_FLYBY;	
	if ( !m_wndPaneBar.Create(this, IDD_EDIT_RESULT_PANE_BAR, dwBarStyle, IDD_EDIT_RESULT_PANE_BAR) )
	{
		TRACE0("Failed to create Pane Dialogbar Control\n");
		return -1;
	}	

	RECT   rect={0,0,0,0};
	CWnd::GetClientRect(&rect);
	DWORD dwListStyle1 = WS_CHILD | WS_VISIBLE | LVS_REPORT  | LVS_SHAREIMAGELISTS | LVS_SINGLESEL | LVS_SHOWSELALWAYS;
	
	m_wndSplitter.CreateStatic(this,3,1);		
	if(!m_wndSplitter.AddWindow(0,0,&m_wndDefectListCtrl,WC_LISTVIEW,dwListStyle1,0,CSize(160,400), ID_DEFECT_COMPONENT_LIST))
	{
		TRACE0("Failed to create wnd Defect List Ctrl \n");
		return -1;
	}	
	
	if(!m_wndSplitter.AddWindow(1,0,&m_wndDefectGroupListCtrl,WC_LISTVIEW,dwListStyle1,0,CSize(160,200), ID_DEFECT_GROUP_LIST))
	{
		TRACE0("Failed to create wnd Defect Group List Ctrl \n");
		return -1;
	}	

	if(!m_wndSplitter.AddWindow(2,0,&m_wndDefectWndListCtrl,WC_LISTVIEW,dwListStyle1,0,CSize(160,200), ID_DEFECT_WND_LIST))
	{
		TRACE0("Failed to create wnd Defect Wnd List Ctrl \n");
		return -1;
	}	

	m_wndDefectListCtrl.SetOwner(this);
	m_wndDefectGroupListCtrl.SetOwner(this);
	m_wndDefectWndListCtrl.SetOwner(this);
	
	//if ( m_wndDefectListCtrl.Create(dwListStyle1, rect, this, ID_DEFECT_COMPONENT_LIST) == FALSE )
	//{
	//	TRACE0("Failed to create wnd Defect List Ctrl \n");
	//	return -1;
	//}

	// 載入影像:	
	OnChangeVisualStyle();
		
	JetAPI::InitialListCtrl(m_wndDefectListCtrl);
	JetAPI::InitialListCtrl(m_wndDefectGroupListCtrl);
	JetAPI::InitialListCtrl(m_wndDefectWndListCtrl);
	BuildDefectComponentListHeader();
	BuildDefectGroupListHeader();	
	BuildDefectWndListHeader();
	return 0;
}
//-------------------------------------------------------------------------------------//
void CEditResultDockPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: 在此加入您的訊息處理常式程式碼
	if ( CWnd::GetSafeHwnd() == NULL ) { return; }
	if ( m_wndPaneBar.GetSafeHwnd() == NULL ) { return; }
	if ( m_wndSplitter.GetSafeHwnd() == NULL ) { return; }
	//if ( m_wndDefectListCtrl.GetSafeHwnd() == NULL ) { return; }

	CRect rect;
	int cyTlb = 0;
	cyTlb = m_wndPaneBar.CalcFixedLayout(FALSE, FALSE).cy;
	rect.left = 0; 
	rect.top = 0;
	rect.right = cx;
	rect.bottom = cy;
	
	m_wndPaneBar.SetWindowPos(NULL,rect.left, rect.top, rect.Width(), cyTlb, SWP_NOZORDER | SWP_NOACTIVATE);
	//m_wndDefectListCtrl.SetWindowPos(NULL,rect.left, rect.top+cyTlb, rect.Width(), rect.Height()-cyTlb, SWP_NOZORDER | SWP_NOACTIVATE);
	m_wndSplitter.SetWindowPos(NULL,rect.left, rect.top+cyTlb, rect.Width(), rect.Height()-cyTlb, SWP_NOZORDER | SWP_NOACTIVATE);

	//m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);		
	//m_wndDefectListCtrl.SetWindowPos (NULL, 0, 0, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}
//-------------------------------------------------------------------------------------//
void CEditResultDockPane::OnChangeVisualStyle()
{
	/*
	m_ClassViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_CLASS_VIEW_24 : IDB_CLASS_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("無法載入點陣圖: %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	m_ClassViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_ClassViewImages.Add(&bmp, RGB(255, 0, 0));

	m_wndClassView1.SetImageList(&m_ClassViewImages, TVSIL_NORMAL);
	m_wndClassView2.SetImageList(&m_ClassViewImages, TVSIL_NORMAL);
//	m_wndClassView3.SetImageList(&m_ClassViewImages, TVSIL_NORMAL);

	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_SORT_24 : IDR_SORT, 0, 0, TRUE);
	*/
}
//-------------------------------------------------------------------------------------//
LRESULT CEditResultDockPane::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此加入特定的程式碼和 (或) 呼叫基底類別	
	DWORD Res = 0;
	HWND hWnd = NULL;
	switch ( message )
	{
	case MSG_MAIN_FRAME_MESSAGE:
		switch ( wParam )
		{
		case WPARAM_PROJECT_NEW:
			CloseProject();
			break;
		case WPARAM_PROJECT_OPEN:
			CloseProject();
			break;
		case WPARAM_PROJECT_CLOSE:
			CloseProject();			
			break;
		case WPARAM_PROJECT_SWITCH:
			CloseProject();
			break;
		case WPARAM_PROJECT_UPDATE:
			UpdateDefectComponentListWnd();
			break;
		case WPARAM_PROJECT_UPDATE_FD_ALIGN:
			break;
		case WPARAM_PROJECT_PART_SELECTED:
			break;
		case WPARAM_PROJECT_PART_DELETED:
			break;
		case WPARAM_PROJECT_CLOSE_ACTIVE_OBJ:
			break;
		}
		//hWnd = m_wndComponentListPage.GetSafeHwnd();
		if ( NULL != hWnd )
		{
			//if ( m_wndComponentListPage.IsWindowVisible() == TRUE )
			//{	::SendMessage(hWnd, message, wParam, lParam); }
		}		
		break;
	case MSG_EDIT_PART_LIST_WND:
		switch ( wParam )
		{
		case WPARAM_BUILD_PART_LIST:
			Res = lParam&LPARAM_BUILD_DOCK_LIST_RESULT;
			if ( 0 != Res )
			{	BuildDefectComponentListWnd(); }
			break;
		case WPARAM_UPDATE_PART_LIST:
			AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_UPDATE, (LPARAM)(this));
			break;
		case WPARAM_CLEAR_PART_LIST:
			break;
		case WPARAM_UPDATE_PART_SELECTED:
			AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_UPDATE, (LPARAM)(this));
			break;
		case WPARAM_UPDATE_PART_STATES:
			break;
		}
		break;
	case MSG_EDIT_RESULT_LIST_WND:
		switch ( wParam )
		{
		case WPARAM_BUILD_RESULT_LIST:
			//if ( CWnd::IsWindowVisible() ==  TRUE )
			{	BuildDefectComponentListWnd(); }
			break;
		case WPARAM_UPDATE_RESULT_LIST:
			UpdateDefectComponentListWnd();
			break;
		case WPARAM_CLEAR_RESULT_LIST:
			ClearDefectListWnd();
			break;		
		case WPARAM_UPDATE_RESULT_SELECTED:
			break;		
		}
		//hWnd = m_wndComponentListPage.GetSafeHwnd();
		if ( NULL != hWnd )
		{
			//if ( m_wndComponentListPage.IsWindowVisible() == TRUE )
			//{	::SendMessage(hWnd, message, wParam, lParam); }
		}		
		break;
	case MSG_LIST_CTRL:
		switch ( wParam )
		{
		case WPARAM_LIST_VERTICAL_SCROLL_END:
			if ( (int)(lParam) > 0 )
			{	BuildNextDefectComponentCtrl();	}			
			break;
		}
		break;
	}	
	return CDockablePane::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void CEditResultDockPane::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CWnd::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if ( TRUE == bShow )
	{	
		BuildDefectComponentListWnd();	
		AOIDataCollect.SetShowUIWndResultList(true);
	}
	else
	{	
		ClearDefectComponentListWnd(); 
		AOIDataCollect.SetShowUIWndResultList(false);
	}
}
//-------------------------------------------------------------------------------------//
void CEditResultDockPane::OnContextMenu(CWnd* pWnd, CPoint point)
{
	if ( NULL != pWnd )
	{
		UINT CtrlID = pWnd->GetDlgCtrlID();			
		if ( ID_DEFECT_COMPONENT_LIST == CtrlID )
		{	ExecDefectListMenu(point);	}		
	}	
}
//-------------------------------------------------------------------------------------//
bool CEditResultDockPane::ExecDefectListMenu(CPoint point)
{	
	CMenu menu;		
	UINT menuID = IDR_MENU_TREE_COMPONENT;	
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
void CEditResultDockPane::SwitchMultiLanguage()
{
	/*
	size_t  i = 0;	
	UINT    WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_EDIT_RESULT_DOCK_PANE");	
	//---------------------------------------------------------------------------------//
	WndID = IDD_EDIT_RESULT_DOCK_PANE;
	WndKey = _T("IDD_EDIT_RESULT_DOCK_PANE");
	this->GetWindowText(LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetWindowText(NewLabelText);
	//---------------------------------------------------------------------------------//	
	*/
}
//-------------------------------------------------------------------------------------//
CString CEditResultDockPane::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_EDIT_RESULT_DOCK_PANE");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
void CEditResultDockPane::CloseProject()
{
	m_ProjectPtr = NULL;
	ClearDefectListWnd();
}
//-------------------------------------------------------------------------------------//
bool CEditResultDockPane::ClearDefectListWnd()
{
	m_ComponentPtrAct = NULL;
	ClearDefectWndListWnd();
	ClearDefectGroupListWnd();
	ClearDefectComponentListWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
inline CAOIProject* CEditResultDockPane::GetActiveProject()
{
	return m_ProjectPtr;
}
//-------------------------------------------------------------------------------------//
inline CAOIComponent* CEditResultDockPane::GetActiveComponent()
{
	return m_ComponentPtrAct;
}
//-------------------------------------------------------------------------------------//
inline void CEditResultDockPane::SetActiveComponent(CAOIComponent* Ptr)
{
	m_ComponentPtrAct = Ptr;
}
//-------------------------------------------------------------------------------------//
COLORREF CEditResultDockPane::GetResultColor(RESULT_ID ResultID)
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
bool CEditResultDockPane::BuildDefectGroupListHeader()
{
	CThisListCtrl_12 &ListCtrl = m_wndDefectGroupListCtrl;
	if ( ListCtrl.GetSafeHwnd() == NULL ) { return true; }

	CString   str;
	RECT      rect={0,0,0,0};
	int       CellW=0;
	const int Dummy = 16;
	const int Align = LVCFMT_CENTER;//LVCFMT_CENTER;LVCFMT_RIGHT
	ListCtrl.GetClientRect(&rect);
	const int RectW = rect.right-rect.left-Dummy;

	CellW = 128;		
	//str = LoadMultiLanguageString(str, str);
	str = AOIDataDefine.GetDefectText();
	ListCtrl.InsertColumn(0, str, Align, CellW);

	CellW = 64;
	str = _T("Group");	
	//str = LoadMultiLanguageString(str, str);
	str = AOIDataDefine.GetGroupIDText();	
	ListCtrl.InsertColumn(1, str, Align, CellW);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditResultDockPane::ClearDefectGroupListWnd()
{
	CThisListCtrl_12 &ListCtrl = m_wndDefectGroupListCtrl;
	if ( ListCtrl.GetSafeHwnd() == NULL ) { return true; }	
	UpdateDefectGroupListTitle(NULL);
	m_StopDefectGroupListBeSelected = true;
	JetAPI::ClearListCtrl(ListCtrl, FALSE);	
	m_StopDefectGroupListBeSelected = false;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditResultDockPane::BuildDefectGroupListWnd()
{	
	ClearDefectWndListWnd();
	ClearDefectGroupListWnd();
	CThisListCtrl_12 &ListCtrl = m_wndDefectGroupListCtrl;
	if ( ListCtrl.GetSafeHwnd() == NULL ) { return true; }

	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }	
	CAOIComponent *ComponentPtr = GetActiveComponent();
	if ( NULL == ComponentPtr ) { return true; }	
	if ( ProjectPtr->CheckProjectComponentValid(ComponentPtr) == false ) { return true; }

	CAOIModel *ModelPtr = ComponentPtr->GetComponentModelPtr();
	if ( NULL == ModelPtr ) { return true; }
	
	size_t       i=0, j=0;
	int           nItem=0;
	int           WndGroupID=0;	
	CString       str;
	CString       WndDefectText;
	WND_DEFECT_ID WndDefectID;
	CAOIWnd      *WndPtr = NULL;
	const bool   bHugeRows = true;
	const size_t DefectWndGroupIDCount = ModelPtr->GetModelDefectWndGroupIDCount();			

	nItem=0;
	if ( true == bHugeRows )
	{	ListCtrl.SetRedraw(FALSE);	 }	
	m_StopDefectGroupListBeSelected = true;
	for ( i=0; i<DefectWndGroupIDCount; i++ )
	{	
		WndGroupID = ModelPtr->GetModelDefectWndGroupID(i, false);		
		WndPtr = ModelPtr->GetModelWndPtrByGroupBandID(WndGroupID, -1, false);
		if ( NULL == WndPtr ) { continue; }
		WndDefectID = WndPtr->GetWndDefectID();
		WndDefectText = AOIDataDefine.GetWndDefectIDText(WndDefectID);

		ListCtrl.InsertItem(nItem, WndDefectText);
		ListCtrl.SetItemData(nItem, WndGroupID);
		ListCtrl.SetItemText(nItem, 0, WndDefectText);		

		str.Format(_T("%d"), WndGroupID+1);
		ListCtrl.SetItemText(nItem, 1, str);

		nItem ++;
	}
	m_StopDefectGroupListBeSelected = false;
	if ( true == bHugeRows )
	{	ListCtrl.SetRedraw(TRUE); }

	AOIDataCollect.SetIsModelWndGroupSelChange(true);
	UpdateDefectGroupListTitle(ComponentPtr);

	if ( nItem > 0 ) 
	{
		const int nActItem = 0;
		ListCtrl.SetItemState(nActItem, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
		BuildDefectWndListWnd(nActItem);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditResultDockPane::UpdateDefectGroupListTitle(CAOIComponent *ComponentPtr)
{
	//並更選中零件名稱
	CString Name;
	LVCOLUMN col;
	TCHAR buf[MAX_JET_PATH]=_T("");
	::memset(&col, 0x00, sizeof(col));
	if ( NULL == ComponentPtr )
	{	Name = AOIDataDefine.GetDefectText();	}
	else
	{	Name = ComponentPtr->GetComponentFullName();	}
	::_stprintf(buf, _T("%s"), Name);
	col.mask = LVCF_TEXT;//LVCF_WIDTH;
	col.pszText = buf;	
	m_wndDefectGroupListCtrl.SetColumn(0, &col);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditResultDockPane::BuildDefectWndListHeader()
{
	CThisListCtrl_12 &ListCtrl = m_wndDefectWndListCtrl;
	if ( ListCtrl.GetSafeHwnd() == NULL ) { return true; }

	CString   str;
	RECT      rect={0,0,0,0};
	int       CellW=0;
	const int Dummy = 16;
	const int Align = LVCFMT_CENTER;//LVCFMT_CENTER;LVCFMT_RIGHT
	ListCtrl.GetClientRect(&rect);
	const int RectW = rect.right-rect.left-Dummy;

	CellW = 64;
	str = _T("Wnd");		
	//str = LoadMultiLanguageString(str, str);
	str = AOIDataDefine.GetWndText();
	ListCtrl.InsertColumn(0, str, Align, CellW);

	CellW = 64;
	str = _T("Algorithm");		
	//str = LoadMultiLanguageString(str, str);
	str = AOIDataDefine.GetAlgorithmText();
	ListCtrl.InsertColumn(1, str, Align, CellW);

	CellW = 64;
	str = _T("Result");	
	//str = LoadMultiLanguageString(str, str);
	str = AOIDataDefine.GetResultText();
	ListCtrl.InsertColumn(2, str, Align, CellW);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditResultDockPane::ClearDefectWndListWnd()
{
	CThisListCtrl_12 &ListCtrl = m_wndDefectWndListCtrl;
	if ( ListCtrl.GetSafeHwnd() == NULL ) { return true; }
	m_StopDefectWndListBeSelected = true;	
	JetAPI::ClearListCtrl(ListCtrl, FALSE);	
	m_StopDefectWndListBeSelected = false;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditResultDockPane::BuildDefectWndListWnd(int nGroupListItem)
{
	ClearDefectWndListWnd();
	if ( nGroupListItem < 0 ) { return true; }
	CThisListCtrl_12 &ListCtrl = m_wndDefectWndListCtrl;
	if ( ListCtrl.GetSafeHwnd() == NULL ) { return true; }	

	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }	
	CAOIComponent *ComponentPtr = GetActiveComponent();
	if ( NULL == ComponentPtr ) { return true; }	
	if ( ProjectPtr->CheckProjectComponentValid(ComponentPtr) == false ) { return true; }
	CAOIModel *ModelPtr = ComponentPtr->GetComponentModelPtr();
	if ( NULL == ModelPtr ) { return true; }	
	const int WndGroupID = (int)(m_wndDefectGroupListCtrl.GetItemData(nGroupListItem));


	size_t       i=0;	
	CString      str;	
	int          nItem=0;	
	ALG_TYPE     AlgType;
	CString      AlgTypeText;
	RESULT_ID    WndResultID;	
	CString      WndResultIDText;
	unsigned int WndIndex=0;	
	CAOIWnd     *WndPtr = NULL;	
	const bool   bHugeRows = false;
	const size_t WndCount = ModelPtr->GetModelWndCount();	

	nItem=0;
	if ( true == bHugeRows )
	{	ListCtrl.SetRedraw(FALSE); }
	m_StopDefectWndListBeSelected = true;
	for ( i=0; i<WndCount; i++ )
	{	
		WndPtr = ModelPtr->GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndGroupID() != WndGroupID ) { continue; }
		WndResultID = WndPtr->GetWndResultID();
		WndResultID = WndPtr->GetWndLogicResultID();		
		if ( RESULT_ID_NONE == WndResultID ) { continue; }
		if ( RESULT_ID_OK == WndResultID ) { continue; }
		if ( RESULT_ID_SKIP == WndResultID ) { continue; }
		if ( RESULT_ID_BYPASS == WndResultID ) { continue; }

		AlgType = WndPtr->GetWndAlgType();
		AlgTypeText = AOIDataDefine.GetAlgTypeText(AlgType);

		WndResultIDText = AOIDataDefine.GetResultIDText(WndResultID);

		WndIndex = WndPtr->GetWndIndex();
		str.Format(_T("%d"), WndIndex+1);
		ListCtrl.InsertItem(nItem, str);
		ListCtrl.SetItemData(nItem, WndIndex);
		ListCtrl.SetItemText(nItem, 0, str);		

		str = AlgTypeText;
		ListCtrl.SetItemText(nItem, 1, str);

		str = WndResultIDText;
		ListCtrl.SetItemText(nItem, 2, str);

		nItem ++;
	}
	m_StopDefectWndListBeSelected = false;
	if ( true == bHugeRows )
	{	ListCtrl.SetRedraw(TRUE); }

	if ( nItem > 0 ) 
	{	ExecItemchangedDefectWndListCtrl(0); }
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditResultDockPane::ClearDefectComponentNodeList()
{
	m_DefectComponentList.clear();
}
//-------------------------------------------------------------------------------------//
bool CEditResultDockPane::BuildNextDefectComponentCtrl()
{
	CAOIProject          *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }
	std::vector<CAOIComponent*> &DefectList=m_DefectComponentList;
	const int DefectCount=(int)(DefectList.size());
	if ( 0 == DefectCount ) { return true; }
	CThisListCtrl_12 &ListCtrl=m_wndDefectListCtrl;
	const int ItemCount = ListCtrl.GetItemCount();
	if ( ItemCount >= DefectCount )
	{
		DefectList.clear();
		return true;
	}
	if ( ListCtrl.IsItemVisible(ItemCount-1) == FALSE )
	{	return true; }	

	CString        str;
	size_t         i = 0;	
	int            nItemAct=-1;	
	int            nItem = ItemCount;
	CString        ComponentName;	
	CAOIComponent *ComponentPtr = NULL;	
	CAOIComponent *ComponentPtrAct = ProjectPtr->GetProjectActiveComponent();		
	const bool     bShowPassChk = m_wndPaneBar.GetShowPassChk();
	const int      PageCount=ListCtrl.GetCountPerPage();
	const int      nPageValue=LIST_ITEM_PATCH_N_PAGE_VALUE;
	const int      NextItemCount=MIN(ItemCount+(PageCount*nPageValue), DefectCount);	
	
	nItem = ItemCount;
	ListCtrl.SetRedraw(FALSE);
	m_StopDefectListBeSelected = true;		
	for ( int i=ItemCount; i<NextItemCount; i++ )
	{
		ComponentPtr = DefectList[i];		
		if ( NULL == ComponentPtr ) { continue; }
		if ( true == bShowPassChk  )
		{	ComponentPtr = ComponentPtr;	}
		else
		{	ComponentPtr = ComponentPtr->GetComponentResultPtr();	}
		//DISTRICT_ID DistrictID = ComponentPtr->GetComponentDistrictID();		
		//if ( DistrictID != ActDistrictID ) { continue; }

		if ( ComponentPtrAct == ComponentPtr )
		{	nItemAct = nItem;	}
		ComponentName = ComponentPtr->GetComponentName();

		ListCtrl.InsertItem(nItem, ComponentName);
		ListCtrl.SetItemData(nItem, (DWORD_PTR)ComponentPtr);
		UpdateDefectComponentListWndItem(ListCtrl, nItem, nItemAct);

		nItem ++;		
	}
	m_StopDefectListBeSelected = false;
	ListCtrl.SetRedraw(TRUE);

	UpdateDefectListCtrlTitle(ListCtrl, nItem);
	AOIDataCollect.SetDrawModelMode(DRAW_MODEL_RESULT);	

	if ( nItemAct >= 0 ) 
	{		
		ComponentPtr = ComponentPtrAct;
		SetActiveComponent(ComponentPtr);		
		ListCtrl.SetItemState(nItemAct, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);	
		const int ItemCount = ListCtrl.GetItemCount();
		const int ShowIndex = JetAPI::GetEnsureVisibleIndex(nItemAct, ItemCount);
		if ( ShowIndex != nItemAct )
		{	ListCtrl.EnsureVisible(nItemAct, FALSE); }
		ListCtrl.EnsureVisible(ShowIndex, FALSE);

		str = ComponentPtr->GetComponentFullName();
		BuildDefectGroupListWnd();
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditResultDockPane::BuildDefectComponentList(const std::vector<CAOIComponent*> &DefectList)
{
	ClearDefectComponentNodeList();
	m_DefectComponentList = DefectList;
}
//-------------------------------------------------------------------------------------//
bool CEditResultDockPane::UpdateDefectListCtrlTitle(CThisListCtrl_12 &ListCtrl, size_t nItem)
{
	CString str;	
	CString Name;
	LVCOLUMN col;
	TCHAR buf[64]=_T("");
	const int DefectCount=(int)(m_DefectComponentList.size());

	::memset(&col, 0x00, sizeof(col));
	str = _T("Component");
	str = LoadMultiLanguageString(str, str);	
	if ( 0==DefectCount || nItem==DefectCount )
	{	::_stprintf(buf, _T("%s [%d]"), str, nItem);	}
	else
	{	::_stprintf(buf, _T("%s [%d/%d]"), str, nItem, DefectCount);	}
	col.mask = LVCF_TEXT;//LVCF_WIDTH;
	col.pszText = buf;	
	ListCtrl.SetColumn(0, &col);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditResultDockPane::BuildDefectComponentListHeader()
{
	CThisListCtrl_12 &ListCtrl = m_wndDefectListCtrl;
	if ( ListCtrl.GetSafeHwnd() == NULL ) { return true; }

	CString   str;
	RECT      rect={0,0,0,0};
	int       CellW=0;
	int       nSubItem=0;
	const int Dummy = 16;
	const int Align = LVCFMT_CENTER;//LVCFMT_CENTER;LVCFMT_RIGHT
	ListCtrl.GetClientRect(&rect);
	const int RectW = rect.right-rect.left-Dummy;

	CellW = 64;
	str = _T("Component");	
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(nSubItem, str, Align, CellW);
	nSubItem ++;

	CellW = 48;
	str = _T("Panel");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(nSubItem, str, Align, CellW);
	nSubItem ++;

	CellW = 48;
	str = _T("Board");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(nSubItem, str, Align, CellW);
	nSubItem ++;

	CellW = 48;
	str = _T("Isolated");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(nSubItem, str, Align, CellW);
	nSubItem ++;

	CellW = 48;
	str = _T("M-Field");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(nSubItem, str, Align, CellW);
	nSubItem ++;

	if ( true == bShowSelCol )
	{
		CellW = 32;
		str = _T("Selected");
		//str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nSubItem, str, Align, CellW);
		nSubItem ++;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditResultDockPane::ClearDefectComponentListWnd()
{
	CThisListCtrl_12 &ListCtrl = m_wndDefectListCtrl;
	if ( ListCtrl.GetSafeHwnd() == NULL ) { return true; }

	m_ComponentPtrAct = NULL;	
	m_StopDefectListBeSelected = true;
	JetAPI::ClearListCtrl(ListCtrl, FALSE);
	m_StopDefectListBeSelected = false;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditResultDockPane::BuildDefectComponentListWnd()
{
	bool bIsOK = true;
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }

	ProjectPtr->LockProject();
	bIsOK = BuildDefectComponentListWndKernel();
	ProjectPtr->UnlockProject();
	return bIsOK;
}
//-------------------------------------------------------------------------------------//
bool CEditResultDockPane::BuildDefectComponentListWndKernel()
{
	//return BuildDefectComponentListWndKernel_v1();
	return BuildDefectComponentListWndKernel_v2();
}
//-------------------------------------------------------------------------------------//
bool CEditResultDockPane::BuildDefectComponentListWndKernel_v1()
{
	m_ProjectPtr = NULL;
	ClearDefectWndListWnd();
	ClearDefectGroupListWnd();
	ClearDefectComponentListWnd();
	ClearDefectComponentNodeList();

	CThisListCtrl_12 &ListCtrl = m_wndDefectListCtrl;
	if ( ListCtrl.GetSafeHwnd() == NULL ) { return true; }

	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }
	m_ProjectPtr = ProjectPtr;

	size_t         i = 0;
	int            nItem = 0;
	int            nItemAct=-1;
	int            nSubItem = 0;
	DISTRICT_ID    DistrictID;
	unsigned int   PanelIndex=0;
	unsigned int   BoardIndex=0;		
	CString        str;	
	CString        FullName;
	CString        ComponentName;		
	CString        strDefectCount;	
	size_t         DefectCountDA=0;
	size_t         DefectCountDB=0;
	COLORREF       TextColor=0x000000;	
	RESULT_ID      ModelResultID=RESULT_ID_NONE;
	RESULT_ID      ComponentResultID=RESULT_ID_NONE;
	MODEL_TYPE     ModelType=MODEL_TYPE_NULL;
	CAOIModel     *ModelPtr = NULL;
	CAOIComponent *ComponentPtr = NULL;		
	DISTRICT_ID    ActDistrictID = ProjectPtr->GetProjectActDistrictID();
	CAOIComponent *ComponentPtrAct = ProjectPtr->GetProjectActiveComponent();	
	const size_t   ComponentCount = ProjectPtr->GetProjectComponentCount();
	const size_t   DefectComponentCount = ProjectPtr->GetProjectDefectComponentCount();	
	const bool     bShowPassChk = m_wndPaneBar.GetShowPassChk();
	CString        ResultDateTime = ProjectPtr->GetProjectOnlineTuningDateTime();
	CString        strDistrictA=AOIDataDefine.GetDistrictIDText(DISTRICT_ID_A);
	CString        strDistrictB=AOIDataDefine.GetDistrictIDText(DISTRICT_ID_B);

	nItem = 0;
	ListCtrl.SetRedraw(FALSE);
	m_StopDefectListBeSelected = true;
	if ( true == bShowPassChk )
	{
		for ( i=0; i<ComponentCount; i++ )
		{
			ComponentPtr = ProjectPtr->GetProjectComponentPtr(i, false);
			if ( NULL == ComponentPtr ) { continue; }
			if ( ComponentPtr->CheckComponentIsAgent() == true ) { continue; }
			DistrictID = ComponentPtr->GetComponentDistrictID();
			switch ( DistrictID )
			{
			case DISTRICT_ID_A:	DefectCountDA ++;	break;
			case DISTRICT_ID_B:	DefectCountDB ++;	break;
			}
			if ( DistrictID != ActDistrictID ) { continue; }


			ComponentResultID = ComponentPtr->CheckComponentResultID_AOI();
			if ( RESULT_ID_NG == ComponentResultID ) { continue; }
			if ( RESULT_ID_EXCEPTION == ComponentResultID ) { continue; }

			nSubItem = 0;
			ModelPtr = ComponentPtr->GetComponentModelPtr();		
			ModelResultID = ModelPtr->GetModelResultID();
			TextColor = GetResultColor(ModelResultID);

			PanelIndex = ComponentPtr->GetComponentPanelIndex_Project();
			BoardIndex = ComponentPtr->GetComponentBoardIndex_Panel();
			ComponentName = ComponentPtr->GetComponentName();

			ListCtrl.InsertItem(nItem, ComponentName);
			ListCtrl.SetItemData(nItem, (DWORD_PTR)ComponentPtr);
			ListCtrl.SetItemText(nItem, nSubItem, ComponentName);
			ListCtrl.SetItemTextColor(nItem, TextColor);
			nSubItem ++;

			str.Format(_T("%d"), PanelIndex+1);
			ListCtrl.SetItemText(nItem, nSubItem, str);
			nSubItem ++;

			str.Format(_T("%d"), BoardIndex+1);
			ListCtrl.SetItemText(nItem, nSubItem, str);
			nSubItem ++;

			if ( ModelPtr->GetModelIsolated() == true ) 
			{	str = _T("Y"); }
			else
			{	str = _T(""); }
			ListCtrl.SetItemText(nItem, nSubItem, str);
			nSubItem ++;

			if ( ComponentPtr->GetRgnSubRgnCount() > 0 )
			{	str = _T("Y"); }
			else
			{	str = _T(""); }
			ListCtrl.SetItemText(nItem, nSubItem, str);
			nSubItem ++;

			if ( true == bShowSelCol )
			{
				str = _T("");
				ListCtrl.SetItemText(nItem, nSubItem, str);
				nSubItem ++;
			}

			if ( ComponentPtrAct == ComponentPtr )
			{	nItemAct = nItem;	}

			nItem ++;
		}
	}
	else
	{
		DefectCountDA=0;
		DefectCountDB=0;
		for ( i=0; i<DefectComponentCount; i++ )
		{
			ComponentPtr = ProjectPtr->GetProjectDefectComponentPtr(i, false);
			if ( NULL == ComponentPtr ) { continue; }
			ComponentPtr = ComponentPtr->GetComponentResultPtr();
			DistrictID = ComponentPtr->GetComponentDistrictID();
			switch ( DistrictID )
			{
			case DISTRICT_ID_A:	DefectCountDA ++;	break;
			case DISTRICT_ID_B:	DefectCountDB ++;	break;
			}
			if ( DistrictID != ActDistrictID ) { continue; }

			nSubItem = 0;
			ModelPtr = ComponentPtr->GetComponentModelPtr();
			ModelResultID = ModelPtr->GetModelResultID();
			TextColor = GetResultColor(ModelResultID);

			PanelIndex = ComponentPtr->GetComponentPanelIndex_Project();
			BoardIndex = ComponentPtr->GetComponentBoardIndex_Panel();
			ComponentName = ComponentPtr->GetComponentName();

			ListCtrl.InsertItem(nItem, ComponentName);
			ListCtrl.SetItemData(nItem, (DWORD_PTR)ComponentPtr);
			ListCtrl.SetItemText(nItem, nSubItem, ComponentName);
			ListCtrl.SetItemTextColor(nItem, TextColor);
			nSubItem ++;

			str.Format(_T("%d"), PanelIndex+1);
			ListCtrl.SetItemText(nItem, nSubItem, str);
			nSubItem ++;

			str.Format(_T("%d"), BoardIndex+1);
			ListCtrl.SetItemText(nItem, nSubItem, str);
			nSubItem ++;

			if ( ModelPtr->GetModelIsolated() == true ) 
			{	str = _T("Y"); }
			else
			{	str = _T(""); }
			ListCtrl.SetItemText(nItem, nSubItem, str);
			nSubItem ++;

			if ( ComponentPtr->GetRgnSubRgnCount() > 0 )
			{	str = _T("Y"); }
			else
			{	str = _T(""); }
			ListCtrl.SetItemText(nItem, nSubItem, str);
			nSubItem ++;

			if ( true == bShowSelCol )
			{
				str = _T("");
				ListCtrl.SetItemText(nItem, nSubItem, str);
				nSubItem ++;
			}

			if ( ComponentPtrAct == ComponentPtr )
			{	nItemAct = nItem;	}

			nItem ++;
		}
	}
	m_StopDefectListBeSelected = false;
	ListCtrl.SetRedraw(TRUE);
	m_wndPaneBar.SetResultDateTime(ResultDateTime);
	strDefectCount.Format(_T("%s:%d, %s:%d"), strDistrictA, DefectCountDA, strDistrictB, DefectCountDB);
	m_wndPaneBar.SetResultListText(strDefectCount);

	UpdateDefectListCtrlTitle(ListCtrl, nItem);	
	AOIDataCollect.SetDrawModelMode(DRAW_MODEL_RESULT);	

	if ( nItemAct >= 0 ) 
	{		
		ComponentPtr = ComponentPtrAct;
		SetActiveComponent(ComponentPtr);		
		ListCtrl.SetItemState(nItemAct, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);	
		const int ItemCount = ListCtrl.GetItemCount();
		const int ShowIndex = JetAPI::GetEnsureVisibleIndex(nItemAct, ItemCount);
		if ( ShowIndex != nItemAct )
		{	ListCtrl.EnsureVisible(nItemAct, FALSE); }
		ListCtrl.EnsureVisible(ShowIndex, FALSE);

		str = ComponentPtr->GetComponentFullName();
		BuildDefectGroupListWnd();
	}
	return true;


	ComponentPtr = NULL;
	m_MoveToComponent = false;
	if ( nItem > 0 ) 
	{		
		const int index = 0;
		ListCtrl.SetItemState(index, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);	
		const int ItemCount = ListCtrl.GetItemCount();
		const int ShowIndex = JetAPI::GetEnsureVisibleIndex(index, ItemCount);
		if ( ShowIndex != index )
		{	ListCtrl.EnsureVisible(index, FALSE); }
		ListCtrl.EnsureVisible(ShowIndex, FALSE);

		ComponentPtr = (CAOIComponent*)ListCtrl.GetItemData(index);
		SetActiveComponent(ComponentPtr);
		str = ComponentPtr->GetComponentFullName();
		BuildDefectGroupListWnd();
	}	
	m_MoveToComponent = true;	
	//AOIDataCollect.MoveStageToComponentOrField(ComponentPtr, true);
	//AOIDataCollect.SendMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_UPDATE, (LPARAM)(this));
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditResultDockPane::BuildDefectComponentListWndKernel_v2()
{
	m_ProjectPtr = NULL;
	ClearDefectWndListWnd();
	ClearDefectGroupListWnd();
	ClearDefectComponentListWnd();
	ClearDefectComponentNodeList();

	CThisListCtrl_12 &ListCtrl = m_wndDefectListCtrl;
	if ( ListCtrl.GetSafeHwnd() == NULL ) { return true; }

	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }
	m_ProjectPtr = ProjectPtr;

	size_t         i = 0;
	int            nItem = 0;
	int            nItemAct=-1;
	//int            nSubItem = 0;
	DISTRICT_ID    DistrictID;
	//unsigned int   PanelIndex=0;
	//unsigned int   BoardIndex=0;		
	CString        str;	
	//CString        FullName;
	CString        ComponentName;		
	CString        strDefectCount;	
	size_t         DefectCountDA=0;
	size_t         DefectCountDB=0;
//	COLORREF       TextColor=0x000000;	
	//RESULT_ID      ModelResultID=RESULT_ID_NONE;
	RESULT_ID      ComponentResultID=RESULT_ID_NONE;
	//MODEL_TYPE     ModelType=MODEL_TYPE_NULL;
	//CAOIModel     *ModelPtr = NULL;
	CAOIComponent *ComponentPtr = NULL;		
	DISTRICT_ID    ActDistrictID = ProjectPtr->GetProjectActDistrictID();
	CAOIComponent *ComponentPtrAct = ProjectPtr->GetProjectActiveComponent();	
	const size_t   ComponentCount = ProjectPtr->GetProjectComponentCount();
	const size_t   DefectComponentCount = ProjectPtr->GetProjectDefectComponentCount();	
	const bool     bShowPassChk = m_wndPaneBar.GetShowPassChk();
	CString        ResultDateTime = ProjectPtr->GetProjectOnlineTuningDateTime();
	CString        strDistrictA=AOIDataDefine.GetDistrictIDText(DISTRICT_ID_A);
	CString        strDistrictB=AOIDataDefine.GetDistrictIDText(DISTRICT_ID_B);

	DefectCountDA=0;
	DefectCountDB=0;
	std::vector<CAOIComponent*> DefectList;	
	if ( true == bShowPassChk )
	{
		for ( i=0; i<ComponentCount; i++ )
		{
			ComponentPtr = ProjectPtr->GetProjectComponentPtr(i, false);
			if ( NULL == ComponentPtr ) { continue; }
			if ( ComponentPtr->CheckComponentIsAgent() == true ) { continue; }
			DistrictID = ComponentPtr->GetComponentDistrictID();			
			switch ( DistrictID )
			{
			case DISTRICT_ID_A:	DefectCountDA ++;	break;
			case DISTRICT_ID_B:	DefectCountDB ++;	break;
			}
			if ( DistrictID != ActDistrictID ) { continue; }

			ComponentResultID = ComponentPtr->CheckComponentResultID_AOI();
			if ( RESULT_ID_NG == ComponentResultID ) { continue; }
			if ( RESULT_ID_EXCEPTION == ComponentResultID ) { continue; }
			DefectList.push_back(ComponentPtr);
		}
	}
	else
	{
		for ( i=0; i<DefectComponentCount; i++ )
		{
			ComponentPtr = ProjectPtr->GetProjectDefectComponentPtr(i, false);
			if ( NULL == ComponentPtr ) { continue; }
			CAOIComponent *ComponentPtr2 = ComponentPtr->GetComponentResultPtr();
			DistrictID = ComponentPtr2->GetComponentDistrictID();
			switch ( DistrictID )
			{
			case DISTRICT_ID_A:	DefectCountDA ++;	break;
			case DISTRICT_ID_B:	DefectCountDB ++;	break;
			}
			if ( DistrictID != ActDistrictID ) { continue; }
			DefectList.push_back(ComponentPtr);
		}
	}

	const size_t DefectListSize = DefectList.size();
	size_t ItemCount=DefectListSize;
	const int PageCount=ListCtrl.GetCountPerPage();
	if ( ItemCount > LIST_ITEM_PATCH_ENABLE_COUNT )
	{		
		BuildDefectComponentList(DefectList);
		const int nPageValue=LIST_ITEM_PATCH_N_PAGE_VALUE;
		ItemCount=MIN(PageCount*nPageValue, DefectListSize);
	}

	nItem = 0;
	ListCtrl.SetRedraw(FALSE);
	m_StopDefectListBeSelected = true;
	for ( i=0; i<ItemCount; i++ )
	{
		ComponentPtr = DefectList[i];
		if ( NULL == ComponentPtr ) { continue; }
		if ( true == bShowPassChk  )
		{	ComponentPtr = ComponentPtr;	}
		else
		{	ComponentPtr = ComponentPtr->GetComponentResultPtr();	}
		//DistrictID = ComponentPtr->GetComponentDistrictID();		
		//if ( DistrictID != ActDistrictID ) { continue; }

		if ( ComponentPtrAct == ComponentPtr )
		{	nItemAct = nItem;	}
		ComponentName = ComponentPtr->GetComponentName();

		ListCtrl.InsertItem(nItem, ComponentName);
		ListCtrl.SetItemData(nItem, (DWORD_PTR)ComponentPtr);
		UpdateDefectComponentListWndItem(ListCtrl, nItem, nItemAct);

		nItem ++;		
	}
	m_StopDefectListBeSelected = false;
	ListCtrl.SetRedraw(TRUE);
	m_wndPaneBar.SetResultDateTime(ResultDateTime);
	strDefectCount.Format(_T("%s:%d, %s:%d"), strDistrictA, DefectCountDA, strDistrictB, DefectCountDB);
	m_wndPaneBar.SetResultListText(strDefectCount);

	UpdateDefectListCtrlTitle(ListCtrl, nItem);	
	AOIDataCollect.SetDrawModelMode(DRAW_MODEL_RESULT);	

	if ( nItemAct >= 0 ) 
	{		
		ComponentPtr = ComponentPtrAct;
		SetActiveComponent(ComponentPtr);		
		ListCtrl.SetItemState(nItemAct, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);	
		const int ItemCount = ListCtrl.GetItemCount();
		const int ShowIndex = JetAPI::GetEnsureVisibleIndex(nItemAct, ItemCount);
		if ( ShowIndex != nItemAct )
		{	ListCtrl.EnsureVisible(nItemAct, FALSE); }
		ListCtrl.EnsureVisible(ShowIndex, FALSE);

		str = ComponentPtr->GetComponentFullName();
		BuildDefectGroupListWnd();
	}
	return true;


	ComponentPtr = NULL;
	m_MoveToComponent = false;
	if ( nItem > 0 ) 
	{		
		const int index = 0;
		ListCtrl.SetItemState(index, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);	
		const int ItemCount = ListCtrl.GetItemCount();
		const int ShowIndex = JetAPI::GetEnsureVisibleIndex(index, ItemCount);
		if ( ShowIndex != index )
		{	ListCtrl.EnsureVisible(index, FALSE); }
		ListCtrl.EnsureVisible(ShowIndex, FALSE);

		ComponentPtr = (CAOIComponent*)ListCtrl.GetItemData(index);
		SetActiveComponent(ComponentPtr);
		str = ComponentPtr->GetComponentFullName();
		BuildDefectGroupListWnd();
	}	
	m_MoveToComponent = true;	
	//AOIDataCollect.MoveStageToComponentOrField(ComponentPtr, true);
	//AOIDataCollect.SendMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_UPDATE, (LPARAM)(this));
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditResultDockPane::UpdateDefectComponentListWnd()
{
	CThisListCtrl_12 &ListCtrl = m_wndDefectListCtrl;
	if ( ListCtrl.GetSafeHwnd() == NULL ) { return true; }

	CAOIProject *Project = GetActiveProject();
	if ( NULL == Project ) { return true; }	

	CString        str;
	int            i = 0;
	const int      ItemCount = ListCtrl.GetItemCount();
	const int      nItem = ListCtrl.GetNextItem(-1, LVNI_FOCUSED);
	
	if ( ItemCount > HUGE_LIST_ITEM_COUNT )
	{	ListCtrl.SetRedraw(FALSE);	 }
	for ( i=0; i<ItemCount; i++ )
	{	UpdateDefectComponentListWndItem(ListCtrl, i, nItem);	}
	if ( ItemCount > HUGE_LIST_ITEM_COUNT )
	{	ListCtrl.SetRedraw(TRUE);	 }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditResultDockPane::UpdateDefectComponentListWndItem(CThisListCtrl_12 &ListCtrl, int nItem, int nActItem)
{
	DWORD_PTR ItemData = ListCtrl.GetItemData(nItem);
	if ( NULL == ItemData ) { return false; }	

	CString str;
	int nSubItem = 0;	
	CAOIComponent *ComponentPtr = (CAOIComponent*)(ItemData);	
	//if ( Project->CheckProjectComponentValid(ComponentPtr) == false ) { return false; }

	CString ComponentName = ComponentPtr->GetComponentName();
	CAOIModel *ModelPtr = ComponentPtr->GetComponentModelPtr();
	if ( NULL == ModelPtr ) { return false; }	
	RESULT_ID ModelResultID = ModelPtr->GetModelResultID();
	COLORREF TextColor = GetResultColor(ModelResultID);	
	unsigned int PanelIndex = ComponentPtr->GetComponentPanelIndex_Project();
	unsigned int BoardIndex = ComponentPtr->GetComponentBoardIndex_Panel();	

	nSubItem = 0;	
	ListCtrl.SetItemText(nItem, nSubItem, ComponentName);
	ListCtrl.SetItemTextColor(nItem, TextColor);
	nSubItem ++;

	str.Format(_T("%d"), PanelIndex+1);
	ListCtrl.SetItemText(nItem, nSubItem, str);
	nSubItem ++;

	str.Format(_T("%d"), BoardIndex+1);
	ListCtrl.SetItemText(nItem, nSubItem, str);
	nSubItem ++;

	if ( ModelPtr->GetModelIsolated() == true ) 
	{	str = _T("Y"); }
	else
	{	str = _T(""); }
	ListCtrl.SetItemText(nItem, nSubItem, str);
	nSubItem ++;

	if ( ComponentPtr->GetRgnSubRgnCount() > 0 )
	{	str = _T("Y"); }
	else
	{	str = _T(""); }
	ListCtrl.SetItemText(nItem, nSubItem, str);
	nSubItem ++;

	if ( true == bShowSelCol )
	{
		if ( nItem == nActItem )
		{	str = m_SelText;	}
		else
		{	str = _T("");	}		
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditResultDockPane::UpdateDefectComponentListSelected()
{	
	if ( false == bShowSelCol ) { return true; }

	return true;
	CThisListCtrl_12 &ListCtrl = m_wndDefectListCtrl;
	if ( ListCtrl.GetSafeHwnd() == NULL ) { return true; }

	int        i=0;
	CString    str;
	CString    ItemText;
	const int  nSubItem=4;
	const int  ItemCount = ListCtrl.GetItemCount();
	const int  nItem = ListCtrl.GetNextItem(-1, LVNI_FOCUSED);

	if ( ItemCount > HUGE_LIST_ITEM_COUNT )
	{	ListCtrl.SetRedraw(FALSE);	}
	for ( i=0; i<ItemCount; i++ )
	{
		ItemText = ListCtrl.GetItemText(i, nSubItem);
		if ( nItem == i )
		{	str = m_SelText;	}
		else
		{	str = _T("");	}		
		if ( ItemText == str ) { continue; }
		ListCtrl.SetItemText(i, nSubItem, str);
	}
	if ( ItemCount > HUGE_LIST_ITEM_COUNT )
	{	ListCtrl.SetRedraw(TRUE);	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditResultDockPane::RemoveDefectComponentListSelected()
{	
	CThisListCtrl_12 &ListCtrl = m_wndDefectListCtrl;
	if ( ListCtrl.GetSafeHwnd() == NULL ) { return true; }

	CAOIProject *Project = GetActiveProject();
	if ( NULL == Project ) { return true; }	

	int            i = 0;
	CString        str;	
	CString        ComponentName;	
	CString        strDefectCount;
	COLORREF       TextColor=0x000000;		
	CAOIComponent *ComponentPtr = NULL;
	DISTRICT_ID    DistrictID;
	size_t         DefectCountDA=0;
	size_t         DefectCountDB=0;
	const bool     bShowPassChk = m_wndPaneBar.GetShowPassChk();
	CString        strDistrictA=AOIDataDefine.GetDistrictIDText(DISTRICT_ID_A);
	CString        strDistrictB=AOIDataDefine.GetDistrictIDText(DISTRICT_ID_B);

	std::vector<CAOIComponent*> DefectList=m_DefectComponentList;
	const int DefectCount = DefectList.size();
	m_DefectComponentList.clear();
	for ( i=0; i<DefectCount; i++ )
	{
		ComponentPtr = DefectList[i];
		if ( NULL == ComponentPtr ) { continue; }
		if ( Project->CheckProjectComponentValid(ComponentPtr) == false ) { continue; }
		if ( ComponentPtr->GetComponentSelected() == true ) { continue; }

		if ( false == bShowPassChk )
		{
			DistrictID = ComponentPtr->GetComponentDistrictID();
			switch ( DistrictID )
			{
			case DISTRICT_ID_A:	DefectCountDA ++; break;
			case DISTRICT_ID_B:	DefectCountDB ++; break;
			}
		}
		m_DefectComponentList.push_back(ComponentPtr);
	}	
	strDefectCount.Format(_T("%s:%d, %s:%d"), strDistrictA, DefectCountDA, strDistrictB, DefectCountDB);
	m_wndPaneBar.SetResultListText(strDefectCount);

	std::vector<int> RemoveItemIndexList;
	const int      ItemCount = ListCtrl.GetItemCount();	
	for ( i=0; i<ItemCount; i++ )
	{
		ComponentPtr = (CAOIComponent*)(ListCtrl.GetItemData(i));
		if ( NULL == ComponentPtr ) { continue; }
		if ( Project->CheckProjectComponentValid(ComponentPtr) == false ) { continue; }
		if ( ComponentPtr->GetComponentSelected() == false ) { continue; }
		RemoveItemIndexList.push_back(i);
	}

	//Real Remove List Item
	int   nItem=0;
	const int RemoveItemCount = (int)(RemoveItemIndexList.size()); 
	if ( 0 == RemoveItemCount ) { return true; }
	if ( ItemCount > HUGE_LIST_ITEM_COUNT )
	{	ListCtrl.SetRedraw(FALSE);	 }
	for ( i=0; i<RemoveItemCount; i++ )
	{
		nItem = RemoveItemIndexList[RemoveItemCount-i-1];
		ListCtrl.DeleteItem(nItem);
	}
	if ( ItemCount > HUGE_LIST_ITEM_COUNT )
	{	ListCtrl.SetRedraw(TRUE);	 }

	ClearDefectWndListWnd();
	ClearDefectGroupListWnd();
	UpdateDefectListCtrlTitle(ListCtrl, ListCtrl.GetItemCount());
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditResultDockPane::OnClickDefectListCtrl(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here
	NMITEMACTIVATE *pNMListView = (NMITEMACTIVATE*)pNMHDR;	
	const int nItem = pNMListView->iItem;	
	if ( nItem < 0 ) { return; }

	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CEditResultDockPane::OnDbclickDefectListCtrl(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here
	NMITEMACTIVATE *pNMListView = (NMITEMACTIVATE*)pNMHDR;	
	const int nItem = pNMListView->iItem;	
	if ( nItem < 0 ) { return; }

	CAOIComponent *pComponent = (CAOIComponent*)(m_wndDefectListCtrl.GetItemData(nItem));	
	if ( NULL == pComponent ) { return; }
	//AOIDataCollect.SetDrawModelMode(DRAW_MODEL_RESULT);
	AOIDataCollect.MoveStageToComponentOrField(pComponent, true);	
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
bool CEditResultDockPane::ExecItemchangedComponentListCtrl(int nItem)
{
	ClearDefectWndListWnd();
	ClearDefectGroupListWnd();
	CThisListCtrl_12 &ListCtrl = m_wndDefectListCtrl;
	if ( ListCtrl.GetSafeHwnd() == NULL ) { return true; }
	const int ItemCount = ListCtrl.GetItemCount();
	if ( nItem<-1 || nItem>=ItemCount ) { return true; }

	UpdateDefectComponentListSelected();	
	CString str = m_wndDefectListCtrl.GetItemText(nItem, 0);
	str.MakeUpper();	

	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }
	CAOIComponent *pComponent = ProjectPtr->GetProjectActiveComponent();
	AOIDataCollect.CloseActiveComponent(pComponent);		
	if ( NULL != pComponent )
	{	pComponent->SetComponentSelected(false); }
	
	CAOIComponent *ComponentPtr = (CAOIComponent*)(m_wndDefectListCtrl.GetItemData(nItem));	
	if ( NULL == ComponentPtr ) { return false; }
	if ( ProjectPtr->CheckProjectComponentValid(ComponentPtr) == false )
	{	return false; } 
	ComponentPtr = ComponentPtr->GetComponentResultPtr();

	SetActiveComponent(ComponentPtr);
	ProjectPtr->SelectProjectAllComponents(false);
	ComponentPtr->SetComponentSelected(true);
	ComponentPtr->ChangeComponentSelected(true);
	ProjectPtr->SetProjectActiveComponent(ComponentPtr);

	BuildDefectGroupListWnd();	
	//AOIDataCollect.SetDrawModelMode(DRAW_MODEL_RESULT);
	AOIDataCollect.SendMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_UPDATE, (LPARAM)(this));
	if ( true == m_MoveToComponent)
	{	AOIDataCollect.MoveStageToComponentOrField(ComponentPtr, false); }
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditResultDockPane::OnItemchangedDefectListCtrl(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMITEMACTIVATE *pNMListView = (NMITEMACTIVATE*)pNMHDR;	
	// TODO: Add your control notification handler code here	
	if ( true == m_StopDefectListBeSelected ) { return; }
	const int nItem = pNMListView->iItem;	
	if ( nItem < 0 ) { return; }
	DWORD Res = pNMListView->uNewState&LVIS_SELECTED;
	if ( Res == 0 ) { return; }	
	Res = pNMListView->uNewState&LVIS_FOCUSED;
	if ( Res == 0 ) { return; }	
	
	ExecItemchangedComponentListCtrl(nItem);
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CEditResultDockPane::OnItemchangedDefectGroupListCtrl(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMITEMACTIVATE *pNMListView = (NMITEMACTIVATE*)pNMHDR;	
	// TODO: Add your control notification handler code here	
	if ( true == m_StopDefectGroupListBeSelected ) { return; }
	const int nItem = pNMListView->iItem;	
	if ( nItem < 0 ) { return; }
	DWORD Res = pNMListView->uNewState&LVIS_SELECTED;
	if ( Res == 0 ) { return; }	
	Res = pNMListView->uNewState&LVIS_FOCUSED;
	if ( Res == 0 ) { return; }

	AOIDataCollect.SetIsModelWndGroupSelChange(true);
	BuildDefectWndListWnd(nItem);	
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CEditResultDockPane::OnItemchangedDefectWndListCtrl(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMITEMACTIVATE *pNMListView = (NMITEMACTIVATE*)pNMHDR;	
	// TODO: Add your control notification handler code here
	if ( true == m_StopDefectWndListBeSelected ) { return; }
	const int nItem = pNMListView->iItem;	
	if ( nItem < 0 ) { return; }
	DWORD Res = pNMListView->uNewState&LVIS_SELECTED;
	if ( Res == 0 ) { return; }	
	Res = pNMListView->uNewState&LVIS_FOCUSED;
	if ( Res == 0 ) { return; }

	ExecItemchangedDefectWndListCtrl(nItem);	
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CEditResultDockPane::OnDblclkDefectGroupListCtrl(NMHDR* pNMHDR, LRESULT* pResult)
{	
	const int ItemCount = m_wndDefectWndListCtrl.GetItemCount();
	const int nItem = m_wndDefectWndListCtrl.GetNextItem(-1, LVNI_FOCUSED);
	if ( nItem<0 && ItemCount>0 )
	{	m_wndDefectWndListCtrl.SetItemState(0, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);	}
	ExecDblclkDefectWndListCtrl();
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CEditResultDockPane::OnDblclkDefectWndListCtrl(NMHDR* pNMHDR, LRESULT* pResult)
{
	ExecDblclkDefectWndListCtrl();
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CEditResultDockPane::OnEndScrollDefectListCtrl(NMHDR* pNMHDR, LRESULT* pResult)
{	
	NMLVSCROLL* pStateChanged = (NMLVSCROLL*)pNMHDR;	
	// TODO: Add your control notification handler code here
	if ( 0 != pStateChanged->dx )//Hor
	{
	}
	if ( 0 != pStateChanged->dy )//Ver
	{
		if ( pStateChanged->dy > 0 )
		{	BuildNextDefectComponentCtrl();	}
	}
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CEditResultDockPane::OnSwitchPreiousBtn()
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }			

	int        i=0;
	int        nItemNext=-1;	
	bool       bFind=false;
	CAOIComponent *ComponentPtr = NULL;
	CThisListCtrl_12 &ListCtrl = m_wndDefectListCtrl;
	const int  nItem = ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	const int  ItemCount = ListCtrl.GetItemCount();	

	bFind = false;
	nItemNext=-1;
	for ( i=nItem-1; i>=0; i-- )
	{
		ComponentPtr = (CAOIComponent*)(ListCtrl.GetItemData(i));
		if ( NULL == ComponentPtr ) { continue; }		
		nItemNext = i;
		break;
	}
	if ( -1 == nItemNext )
	{
		for ( i=ItemCount-1; i>nItem; i-- )
		{
			ComponentPtr = (CAOIComponent*)(ListCtrl.GetItemData(i));
			if ( NULL == ComponentPtr ) { continue; }		
			nItemNext = i;
			break;
		}
	}	
	if ( -1 == nItemNext )
	{	return; }

	m_MoveToComponent = false;
	ListCtrl.SetItemState(nItemNext, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);		
	const int ShowIndex = JetAPI::GetEnsureVisibleIndex(nItemNext, ItemCount);
	if ( ShowIndex != nItemNext )
	{	ListCtrl.EnsureVisible(nItemNext, FALSE); }
	ListCtrl.EnsureVisible(ShowIndex, FALSE);

	m_MoveToComponent = true;
	AOIDataCollect.MoveStageToComponentOrField(ComponentPtr, true);
	return;
}
//-------------------------------------------------------------------------------------//
void CEditResultDockPane::OnSwitchNextBtn()
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }			

	int        i=0;
	int        nItemNext=-1;	
	bool       bFind=false;		
	CAOIComponent *ComponentPtr = NULL;
	CThisListCtrl_12 &ListCtrl = m_wndDefectListCtrl;
	const int  nItem = ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	const int  ItemCount = ListCtrl.GetItemCount();	

	bFind = false;
	nItemNext=-1;
	for ( i=nItem+1; i<ItemCount; i++ )
	{
		ComponentPtr = (CAOIComponent*)(ListCtrl.GetItemData(i));
		if ( NULL == ComponentPtr ) { continue; }
		nItemNext = i;
		break;
	}
	if ( -1 == nItemNext )
	{
		for ( i=0; i<nItem; i++ )
		{
			ComponentPtr = (CAOIComponent*)(ListCtrl.GetItemData(i));
			if ( NULL == ComponentPtr ) { continue; }
			nItemNext = i;
			break;
		}
	}
	if ( -1 == nItemNext )
	{	return; }

	m_MoveToComponent = false;
	ListCtrl.SetItemState(nItemNext, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);		
	const int ShowIndex = JetAPI::GetEnsureVisibleIndex(nItemNext, ItemCount);
	if ( ShowIndex != nItemNext )
	{	ListCtrl.EnsureVisible(nItemNext, FALSE); }
	ListCtrl.EnsureVisible(ShowIndex, FALSE);

	m_MoveToComponent = true;
	AOIDataCollect.MoveStageToComponentOrField(ComponentPtr, true);
	return;
}
//-------------------------------------------------------------------------------------//
void CEditResultDockPane::OnComponentRetestBtn()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	

	int    i=0;
	CAOIComponent *ComponentPtr = NULL;
	CThisListCtrl_12 &ListCtrl = m_wndDefectListCtrl;
	const int  ItemCount = ListCtrl.GetItemCount();	

	ProjectPtr->SelectProjectAllComponents(false);
	ProjectPtr->SelectProjectDefectComponentList_All(true);
	/*
	for ( i=0; i<ItemCount; i++ )
	{
		ComponentPtr = (CAOIComponent*)(ListCtrl.GetItemData(i));
		if ( NULL == ComponentPtr ) { continue; }
		ComponentPtr->SetComponentSelected(true);
	}*/	
	AOIDataCollect.SetChangeRibbonTuneID(false);
	AOIDataCollect.PostMainFrameWndMessage(WM_COMMAND, ID_TUNE_SELECTED_COMPONENT, NULL);
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditResultDockPane::OnShowPassChk()
{
	BuildDefectComponentListWnd();	
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditResultDockPane::OnComponentOffset()
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
	ExecProjectMultiBoardSelection();
	ProjectPtr->MoveProjectComponentSelected(dX, dY);
	LogOperCtrl.SaveLogProjectComponentSelectedMove(ProjectPtr, dX, dY);
	ExecProjectOneComponentSelection();
	//UpdateDefectComponentListWnd();	
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);
}
//-------------------------------------------------------------------------------------//
void CEditResultDockPane::OnComponentSetPos()
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
	ExecProjectMultiBoardSelection();
	ProjectPtr->MoveProjectComponentSelected(dX, dY);
	LogOperCtrl.SaveLogProjectComponentSelectedMove(ProjectPtr, dX, dY);
	ExecProjectOneComponentSelection();
	//UpdateDefectComponentListWnd();	
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);
}
//-------------------------------------------------------------------------------------//
void CEditResultDockPane::OnComponentMirrorPosX()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return ; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	
	ExecProjectMultiBoardSelection();
	ProjectPtr->MirrorXProjectComponentSelected();
	LogOperCtrl.SaveLogProjectComponentSelectedMirrorX(ProjectPtr);
	ExecProjectOneComponentSelection();
	UpdateDefectComponentListWnd();	
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditResultDockPane::OnComponentMirrorPosY()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return ; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	
	ExecProjectMultiBoardSelection();
	ProjectPtr->MirrorYProjectComponentSelected();
	LogOperCtrl.SaveLogProjectComponentSelectedMirrorY(ProjectPtr);
	ExecProjectOneComponentSelection();
	UpdateDefectComponentListWnd();	
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);
	return ;
}
//-------------------------------------------------------------------------------------//
bool CEditResultDockPane::ExecComponentRotation(double Angle)
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return true; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }
	const size_t ComponentSelCount = ProjectPtr->GetProjectComponentSelectedCount();
	if ( 0 == ComponentSelCount ) { return false; }

	ExecProjectMultiBoardSelection();
	ProjectPtr->RotateProjectComponentSelected(Angle);
	LogOperCtrl.SaveLogProjectComponentSelectedRotate(ProjectPtr, Angle);
	ExecProjectOneComponentSelection();
	UpdateDefectComponentListWnd();	
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditResultDockPane::OnComponentRotate090()
{
	ExecComponentRotation(90.0);
}
//-------------------------------------------------------------------------------------//
void CEditResultDockPane::OnComponentRotate180()
{
	ExecComponentRotation(180.0);
}
//-------------------------------------------------------------------------------------//
void CEditResultDockPane::OnComponentRotate270()
{
	ExecComponentRotation(270.0);
}
//-------------------------------------------------------------------------------------//
void CEditResultDockPane::OnComponentRotateAny()
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
void CEditResultDockPane::OnComponentRotateReverse()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return ; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	const size_t ComponentSelCount = ProjectPtr->GetProjectComponentSelectedCount();
	if ( 0 == ComponentSelCount ) { return; }

	ExecProjectMultiBoardSelection();
	ProjectPtr->ReverseProjectComponentSelected();
	ExecProjectOneComponentSelection();
	UpdateDefectComponentListWnd();	
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditResultDockPane::OnComponentRename()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return ; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	CAOIComponent *ComponentPtr = GetActiveComponent();
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
	ExecProjectMultiBoardSelection();	
	ProjectPtr->SetProjectComponentSelectedComponentName(strName, strValue);
	LogOperCtrl.SaveLogProjectComponentSelectedComponentName(ProjectPtr, strName, strValue);		
	ExecProjectOneComponentSelection();
	UpdateDefectComponentListWnd();	
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditResultDockPane::OnComponentDelete()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return ; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	if ( AOIDataCollect.OperateLevelEditFuncDelComponent() == false ) { return ; }

	ExecProjectMultiBoardSelection();

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
	{
		ExecProjectOneComponentSelection();
		return; 
	}
	AOIDataCollect.ReleaseModelUniFrameList();
	RemoveDefectComponentListSelected();
	LogOperCtrl.SaveLogProjectComponentSelectedDelete(ProjectPtr);
	ProjectPtr->DeleteProjectComponentSelected();	
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditResultDockPane::OnComponentSetNozzleName()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return ; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	CAOIComponent *ComponentPtr = GetActiveComponent();
	if ( NULL == ComponentPtr ) { return; }	
	ExecProjectMultiBoardSelection();

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
	if ( InputBox.DoModal() == IDCANCEL )
	{
		ExecProjectOneComponentSelection();
		return ; 
	}
	strValue = InputBox.m_DataEdit1;
	strValue.MakeUpper();	
	ProjectPtr->SetProjectComponentSelectedNozzleName(strValue);
	LogOperCtrl.SaveLogProjectComponentSelectedNozzlName(ProjectPtr, strValue);
	ExecProjectOneComponentSelection();
	UpdateDefectComponentListWnd();	
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditResultDockPane::OnComponentSetPartNumber()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return ; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	CAOIComponent *ComponentPtr = GetActiveComponent();
	if ( NULL == ComponentPtr ) { return; }	
	ExecProjectMultiBoardSelection();

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
	while ( true ) 
	{
		InputBox.SetParam1(strCaption, strLabel, strValue);
		if ( InputBox.DoModal() == IDCANCEL )
		{
			ExecProjectOneComponentSelection();
			return ; 
		}
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
	ExecProjectOneComponentSelection();
	UpdateDefectComponentListWnd();	
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditResultDockPane::OnComponentSearch()
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	CThisListCtrl_12 &ListCtrl = m_wndDefectListCtrl;	
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
		ComponentPtr = (CAOIComponent*)(ListCtrl.GetItemData(i));
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
			ComponentPtr = (CAOIComponent*)(ListCtrl.GetItemData(i));
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
void CEditResultDockPane::OnComponentSelectAll()
{
	return;

	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	int            i=0;
	unsigned int   ComponentIndex=0;
	CAOIComponent *ComponentPtr = NULL;	
	CThisListCtrl_12 &ListCtrl = m_wndDefectListCtrl;	
	const int ItemCount = ListCtrl.GetItemCount();	
	if ( 0 == ItemCount ) { return; }
	
	ProjectPtr->SelectProjectAllComponents(false);
	m_StopDefectListBeSelected = true;
	for ( i=0; i<ItemCount; i++ )
	{
		ComponentPtr = (CAOIComponent*)(ListCtrl.GetItemData(i));
		if ( NULL == ComponentPtr ) { continue; }
		ComponentPtr->SetComponentSelected(true);
		ListCtrl.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
	}	
	m_StopDefectListBeSelected = false;
	return;
}
//-------------------------------------------------------------------------------------//
void CEditResultDockPane::OnComponentBypass()
{	
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	
	if ( AOIDataCollect.OperateLevelEditFuncBypassComponent() == false )	{	return ; }
	ExecProjectMultiBoardSelection();
	ProjectPtr->SwitchProjectComponentBypassed();
	LogOperCtrl.SaveLogProjectComponentSelectedBypassed(ProjectPtr);
	ExecProjectOneComponentSelection();
	UpdateDefectComponentListWnd();
}
//-------------------------------------------------------------------------------------//
void CEditResultDockPane::OnComponentXBoardUnit()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	ExecProjectMultiBoardSelection();
	ProjectPtr->SwitchProjectComponentXBoardUnit();
	LogOperCtrl.SaveLogProjectComponentSelectedXBoardUnit(ProjectPtr);	
	ExecProjectOneComponentSelection();
	UpdateDefectComponentListWnd();
}
//-------------------------------------------------------------------------------------//
void CEditResultDockPane::OnComponentModelIsolated()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	CString str;
	CAOIComponent *ComponentPtr = GetActiveComponent();
	if ( NULL == ComponentPtr ) { return; }
	const bool ModelIsolated = ComponentPtr->GetComponentModelIsolated();
	if ( true == ModelIsolated )
	{
		str = _T("Disable Model Isolated will clear the model datas, do you want to continue?");
		str = LoadMultiLanguageString(str, str);
		if ( JetAPI::ShowMessageBox(str, MB_YESNO) != IDYES )
		{	return ; }
	}
	ExecProjectMultiBoardSelection();
	ProjectPtr->SwitchProjectComponentModelIsolated();
	LogOperCtrl.SaveLogProjectComponentSelectedModelIsolated(ProjectPtr);
	ExecProjectOneComponentSelection();
	UpdateDefectComponentListWnd();
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_WND_PROPERTY_WND, WPARAM_BUILD_WND_SELECTED, NULL);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);
}
//-------------------------------------------------------------------------------------//
void CEditResultDockPane::OnComponentRestoreCadPos()
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
	ExecProjectMultiBoardSelection();
	ProjectPtr->RestoreProjectComponentCadPosBySelected();
	ExecProjectOneComponentSelection();
	UpdateDefectComponentListWnd();
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);
}
//-------------------------------------------------------------------------------------//
void CEditResultDockPane::OnComponentBypass3D()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }		
	if ( AOIDataCollect.OperateLevelEditFuncBypassComponent() == false )	{	return ; }
	ExecProjectMultiBoardSelection();
	ProjectPtr->SwitchProjectComponentBypass3D();
	LogOperCtrl.SaveLogProjectComponentSelectedBypass3D(ProjectPtr);
	ExecProjectOneComponentSelection();
	UpdateDefectComponentListWnd();
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);
}
//-------------------------------------------------------------------------------------//
void CEditResultDockPane::OnComponentMaskBaseSetColorIndex()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }		
	CAOIComponent *ComponentPtr = GetActiveComponent();
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
	
	AskProjectMultiBoardSelection();
	ExecProjectMultiBoardSelection();
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
	ExecProjectOneComponentSelection();
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
void CEditResultDockPane::OnComponentSpaceNoiseFilter()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	
	CAOIComponent *ComponentPtr = GetActiveComponent();
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
	ExecProjectMultiBoardSelection();		
	ProjectPtr->SetProjectComponentSelectedSpaceNoiseFilterParam(NoiseFilterParam);
	if ( -1 != NoiseFilterParam.DataFilterIndex )
	{	ProjectPtr->UpdateProjectSpaceNoiseFilterParam(NoiseFilterParam);	}
	ExecProjectOneComponentSelection();
	AOIDataCollect.ReleaseModelUniFrameList();//需要重新建立，因此釋放模組的圖像資料
	AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_PART_SELECTED, NULL);	
}
//-------------------------------------------------------------------------------------//
void CEditResultDockPane::OnComponentMaskExtendSizeBody()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	
	CAOIComponent *ComponentPtr = GetActiveComponent();
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

	ExecProjectMultiBoardSelection();
	ProjectPtr->SetProjectComponentSelectedMaskExtendSize_Body(ExtendW, ExtendH);
	LogOperCtrl.SaveLogProjectComponentSelectedMaskExtendSize_Body(ProjectPtr, ExtendW, ExtendH);
	ExecProjectOneComponentSelection();
}
//-------------------------------------------------------------------------------------//
void CEditResultDockPane::OnComponentEnableAlarmAOI()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	
	CAOIComponent *ComponentPtr = GetActiveComponent();
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

	ExecProjectMultiBoardSelection();
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
	ExecProjectOneComponentSelection();
	return;
	
}
//-------------------------------------------------------------------------------------//
void CEditResultDockPane::OnComponentGroupID()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	
	CAOIComponent *ComponentPtr = GetActiveComponent();
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
void CEditResultDockPane::OnComponentGroupOrg()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	
	CAOIComponent *ComponentPtr = GetActiveComponent();
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
	ExecProjectMultiBoardSelection();
	ProjectPtr->SetProjectComponentSelectedGroupOrg(bOrg);	
	ExecProjectOneComponentSelection();
}
//-------------------------------------------------------------------------------------//
void CEditResultDockPane::OnComponentToFieldPos()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	
	CAOIComponent *ComponentPtr = GetActiveComponent();
	if ( NULL == ComponentPtr )	{	return; }	
	if ( AOIDataCollect.MoveStageToComponentField(ComponentPtr, true) == false )
	{		
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return;
	}
}
//-------------------------------------------------------------------------------------//
void CEditResultDockPane::OnComponentEnableSelfField()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	
	CAOIComponent *ComponentPtr = GetActiveComponent();
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
	ExecProjectMultiBoardSelection();
	ProjectPtr->SetProjectComponentSelectedEnableSelfField(bEnabled);
	LogOperCtrl.SaveLogProjectComponentSelectedSelfField(ProjectPtr, bEnabled);
	ExecProjectOneComponentSelection();
	return;
}
//-------------------------------------------------------------------------------------//
void CEditResultDockPane::OnComponentCloneNewModel()
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	
	CAOIComponent *ComponentPtr = GetActiveComponent();
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
	ExecProjectMultiBoardSelection();
	ProjectPtr->ApplyProjectModelToComponentsSelected(ModelPtr_New);	
	return;
}
//-------------------------------------------------------------------------------------//
void CEditResultDockPane::OnComponentChangeBoard()
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
	ExecProjectMultiBoardSelection();
	if ( ProjectPtr->ChangeProjectComponentSelectedBoard(BoardPtr) == false )
	{	
		str = ProjectPtr->GetErrorString();
		JetAPI::ShowMessageBox(str);
	}	
	LogOperCtrl.SaveLogProjectComponentSelectedChangeBoard(ProjectPtr, BoardPtr);
	return;
}
//-------------------------------------------------------------------------------------//
void CEditResultDockPane::OnComponentLocalBasePlaneID()
{	
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	
	CAOIComponent *ComponentPtr = GetActiveComponent();
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
	ExecProjectMultiBoardSelection();
	ProjectPtr->SetProjectComponentSelectedLocalBasePlaneID(NewBasePlaneID);		
	LogOperCtrl.SaveLogProjectComponentSelectedLocalBasePlaneID(ProjectPtr, NewBasePlaneID);
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditResultDockPane::OnComponentDataModelParam()
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
	ExecProjectMultiBoardSelection();
	ProjectPtr->SetProjectComponentSelectedDataModelParam(NewParam);
	LogOperCtrl.SaveLogProjectComponentSelectedDataModelParam(ProjectPtr, NewParam);
	return;
}
//-------------------------------------------------------------------------------------//
void CEditResultDockPane::OnComponentSaveWndList()
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
	ExecProjectMultiBoardSelection();
	ProjectPtr->SetProjectComponentSelectedSaveWndList(bEnabled);
	LogOperCtrl.SaveLogProjectComponentSelectedSaveWndList(ProjectPtr, bEnabled);		
	return;
}
//-------------------------------------------------------------------------------------//
void CEditResultDockPane::OnComponentFeedbackResultPos()
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

	ExecProjectMultiBoardSelection();
	ProjectPtr->SetProjectComponentSelectedCadPosByResult(bUsePadPos);
	ExecProjectOneComponentSelection();
	UpdateDefectComponentListWnd();
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);
}
//-------------------------------------------------------------------------------------//
bool CEditResultDockPane::ExecDblclkDefectWndListCtrl()
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }		
	CAOIComponent *ComponentPtr = GetActiveComponent();
	if ( NULL == ComponentPtr ) { return true; }
	if ( ProjectPtr->CheckProjectComponentValid(ComponentPtr) == false ) { return true; }
	CAOIModel *ModelPtr = ComponentPtr->GetComponentModelPtr();
	if ( NULL == ModelPtr ) { return true; }
	const int nItem = m_wndDefectWndListCtrl.GetNextItem(-1, LVNI_FOCUSED);
	if ( nItem < 0 ) { return true; }
	const size_t WndIndex = m_wndDefectWndListCtrl.GetItemData(nItem);
	CAOIWnd *WndPtr = ModelPtr->GetModelWndPtr(WndIndex, true);
	if ( NULL == WndPtr ) { return false; }
	
	double    WndPosX=0;
	double    WndPosY=0;
	double    ModelPosX=0;
	double    ModelPosY=0;
	TREGION4D WndRegion;
	TREGION4D ModelRegion;

	ModelPtr->UnSelectModel();
	ModelPtr->SetModelWndActived(WndPtr);
	WndPtr->GetWndRegionStage(WndRegion);
	ModelPtr->GetModelTotalRegionStage(ModelRegion);
	WndPosX = WndRegion.GetCpX();
	WndPosY = WndRegion.GetCpY();
	ModelPosX = ModelRegion.GetCpX();
	ModelPosY = ModelRegion.GetCpY();
	//AOIDataCollect.MoveStageTo(WndPosX, WndPosY, WndRegion);
	if ( AOIDataCollect.CheckModelUniFrameListModelPtr(ModelPtr) == true )
	{	AOIDataCollect.ShowStageToAct(ModelPosX, ModelPosY, ModelRegion, WndRegion);	 }
	else
	{	AOIDataCollect.MoveStageToAct(ModelPosX, ModelPosY, ModelRegion, WndRegion);	}	

	DRAW_MODEL_MODE DrawModelMode = AOIDataCollect.GetDrawModelMode();
	if ( DRAW_MODEL_EDIT == DrawModelMode )
	{	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_EXEC_WND_INSPECT, NULL);	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditResultDockPane::ExecItemchangedDefectWndListCtrl(int nItem)
{
	if ( nItem < 0 ) { return false; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }		
	CAOIComponent *ComponentPtr = GetActiveComponent();
	if ( NULL == ComponentPtr ) { return true; }
	if ( ProjectPtr->CheckProjectComponentValid(ComponentPtr) == false ) { return true; }
	CAOIModel *ModelPtr = ComponentPtr->GetComponentModelPtr();
	if ( NULL == ModelPtr ) { return true; }
	const int ItemCount = m_wndDefectWndListCtrl.GetItemCount();
	if ( nItem >= ItemCount ) { return true; }

	const size_t WndIndex = m_wndDefectWndListCtrl.GetItemData(nItem);
	CAOIWnd *WndPtr = ModelPtr->GetModelWndPtr(WndIndex, true);
	if ( NULL == WndPtr ) { return false; }
	const int WndGroupID = WndPtr->GetWndGroupID();

	ModelPtr->InvisibleModelWnd();
	ModelPtr->UnSelectModelWnd();

	WndPtr->SetWndSelected(true);	
	ModelPtr->SetModelWndVisibledByWndGroupID(WndGroupID, -1, true);
	ModelPtr->SetModelWndActived(WndPtr);
	AOIDataCollect.ExecAutoSwitchWnd3DFrame(ModelPtr, WndPtr);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_WND_PROPERTY_WND, WPARAM_UPDATE_WND_SELECTED, NULL);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditResultDockPane::AskProjectMultiBoardSelection()//執行多聯板選取
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return false; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }	
	const size_t BoardCount=ProjectPtr->GetProjectBoardCount();
	if ( 1 == BoardCount ) { return true; }
	MULTI_BOARD_CTRL_MODE MultiBoardCtrlMode = AOIDataCollect.GetMultiBoardCtrlMode();
	if ( MULTI_BOARD_CTRL_DISABLE != MultiBoardCtrlMode ) { return true; }	

	CString str;
	str = _T("Apply to Other Board Component?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) != IDYES )
	{	return true; }

	std::vector<CAOIComponent*> SelComponentList;
	ProjectPtr->GetProjectComponentSelected(SelComponentList);
	ProjectPtr->SelectProjectComponentByMultiBoardCtrlMode(SelComponentList, MULTI_BOARD_CTRL_BOARD);		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditResultDockPane::ExecProjectMultiBoardSelection()//執行多聯板選取
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return false; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }	
	const bool MultiKey = false;
	MULTI_BOARD_CTRL_MODE MultiBoardCtrlMode = AOIDataCollect.GetMultiBoardCtrlMode();
	if ( MULTI_BOARD_CTRL_DISABLE == MultiBoardCtrlMode ) { return true; }
	if ( true == MultiKey ) { return true; }
		
	std::vector<CAOIComponent*> SelComponentList;
	ProjectPtr->GetProjectComponentSelected(SelComponentList);
	ProjectPtr->SelectProjectComponentByMultiBoardCtrlMode(SelComponentList, MultiBoardCtrlMode);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditResultDockPane::ExecProjectOneComponentSelection()//執行單零件選取
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true )
	{	return false; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }	
	ProjectPtr->SelectProjectAllComponents(false);
	ProjectPtr->SelectProjectActiveComponent(true);	
	return true;
}
//-------------------------------------------------------------------------------------//