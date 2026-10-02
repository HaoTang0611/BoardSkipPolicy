// EditWndView.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "EditWndView.h"
//-------------------------------------------------------------------------------------//
#include "JetBlob.h"
#include "InputBoxWnd.h"
#include "InputListWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
const UINT ID_WND_CLASS_COMBO     = 1;
const UINT ID_WND_CLASS_COPY_BTN  = 2;
const UINT ID_WND_CLASS_CLEAR_BTN = 3;
//-------------------------------------------------------------------------------------//
const UINT ID_WND_GROUP_LIST = CPageSplitterWnd::GetIdFromRowCol(0, 0);//AFX_IDW_PANE_FIRST;
const UINT ID_WND_OBJ_LIST   = CPageSplitterWnd::GetIdFromRowCol(1, 0);//AFX_IDW_PANE_FIRST;
const UINT ID_WND_PARAM_LIST = CPageSplitterWnd::GetIdFromRowCol(2, 0);//AFX_IDW_PANE_FIRST+16;//注意行列會不同唷
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CEditWndView
//-------------------------------------------------------------------------------------//
CEditWndView::CEditWndView()
{
	m_ModelPtr = NULL;	
	m_ProjectPtr = NULL;
	m_ActiveWndPtr = NULL;	

	m_clrOK = 0x008000;
	m_clrNG = 0x000080;
	m_clrWarnning = 0x207FFF;
	m_clrException = 0x0000FF;
	m_clrSkip = 0x800000;
	m_clrBypass = 0x800000;
	m_clrUnTest = 0x808080;	
	m_PropGridParam.SetExpand(false);
}
//-------------------------------------------------------------------------------------//
CEditWndView::~CEditWndView()
{
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CEditWndView, CWnd)
	//{{AFX_MSG_MAP(CEditWndView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(ID_WND_CLASS_COPY_BTN, OnClassCopyBtn)
	ON_BN_CLICKED(ID_WND_CLASS_CLEAR_BTN, OnClassClearBtn)
	ON_CBN_SELCHANGE(ID_WND_CLASS_COMBO, OnSelchangeClassCombo)	
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_LCLICKED,OnPropertyLClicked)
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_RCLICKED,OnPropertyRClicked)
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_LDBCLICK,OnPropertyLDbClick)
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_RDBCLICK,OnPropertyRDbClick)
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED,OnPropertyChanged)
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_SEL_CHANGED,OnPropertySelChanged)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CEditWndView message handlers
//-------------------------------------------------------------------------------------//
BOOL CEditWndView::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	return CWnd::Create(NULL, _T(""), dwStyle, rect, pParentWnd, nID);
}
//-------------------------------------------------------------------------------------//
int CEditWndView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	CString str;
	CRect rectDummy;
	const int ClassIDCount=MODEL_CLASS_ID_COUNT;
	rectDummy.SetRectEmpty();

	// 建立下拉式方塊:
	CString strClass=AOIDataDefine.GetClassText();
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | CBS_SORT | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	if (!m_wndClassCombo.Create(dwViewStyle, rectDummy, this, ID_WND_CLASS_COMBO))
	{
		TRACE0("無法建立 [屬性] 下拉式方塊\n");
		return -1;      // 無法建立
	}
	CString strCopyBtn=_T("Copy");
	CString strClearBtn=_T("Clear");
	const DWORD dwBtnStyle = WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON;
	strCopyBtn = this->LoadMultiLanguageString(_T("Class Copy"), strCopyBtn);
	strClearBtn = this->LoadMultiLanguageString(_T("Class Clear"), strClearBtn);	
	if (!m_wndClassCopyBtn.Create(strCopyBtn, dwBtnStyle, rectDummy, this, ID_WND_CLASS_COPY_BTN))
	{
		TRACE0("無法建立 [類別-複製] 按鈕\n");
		return -1;      // 無法建立
	}
	if (!m_wndClassClearBtn.Create(strClearBtn, dwBtnStyle, rectDummy, this, ID_WND_CLASS_CLEAR_BTN))
	{
		TRACE0("無法建立 [類別-刪除] 按鈕\n");
		return -1;      // 無法建立
	}	
	for ( int i=0; i<ClassIDCount; i++ )
	{	
		if ( 0 == i )
		{	str = _T("------");	}
		else
		{	str.Format(_T("%s %02d"), strClass, i);	}		
		m_wndClassCombo.AddString(str);
		m_wndClassCombo.SetItemData(i, i);
	}	
	m_wndClassCombo.SetCurSel(0);
#ifndef MULTI_CLASS_USE
	m_wndClassCombo.EnableWindow(FALSE);
#endif//MULTI_CLASS_USE

	m_wndSplitter.CreateStatic(this,3,1);
	DWORD dwPropStyle = WS_CHILD|WS_VISIBLE;
	m_wndGroupList.SetLeftColumnWidthRatio(0.6);
	m_wndWndList.SetLeftColumnWidthRatio(0.5);
	m_wndWndParam.SetLeftColumnWidthRatio(0.45);	
	//if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
	if ( !m_wndSplitter.AddWindow(0, 0, &m_wndGroupList,WC_LISTVIEW,dwPropStyle,0,CSize(200,200), ID_WND_GROUP_LIST))
	{
		TRACE0("無法建立 [群組] 方格\n");
		return -1;
	}	

	if ( !m_wndSplitter.AddWindow(1, 0, &m_wndWndList,WC_LISTVIEW,dwPropStyle,0,CSize(200,200), ID_WND_OBJ_LIST))
	{
		TRACE0("無法建立 [框列表] 方格\n");
		return -1;
	}	

	if ( !m_wndSplitter.AddWindow(2, 0, &m_wndWndParam,WC_LISTVIEW,dwPropStyle,0,CSize(200,400), ID_WND_PARAM_LIST))
	{
		TRACE0("無法建立 [框參數] 方格\n");
		return -1;
	}	
	m_wndGroupList.SetOwner(this);
	m_wndWndList.SetOwner(this);
	m_wndWndParam.SetOwner(this);

	SetPropListFont();
	InitPropList();	

	//BuildWndGroupList();
	UpdateClassComboxID();
	BuildWndParamList();
	
	//m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_PROPERTIES);
	//m_wndToolBar.LoadToolBar(IDR_PROPERTIES, 0, 0, TRUE /* 已鎖定 */);
	//m_wndToolBar.CleanUpLockedImages();
	//m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_PROPERTIES_HC : IDR_PROPERTIES, 0, 0, TRUE /* 鎖定 */);

	//m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	//m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	//m_wndToolBar.SetOwner(this);

	// 所有命令都將經由此控制項傳送，而不是經由父框架:
	//m_wndToolBar.SetRouteCommandsViaFrame(FALSE);
	AdjustLayout();
	return 0;
}
//-------------------------------------------------------------------------------------//
void CEditWndView::OnDestroy() 
{
	CWnd::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CEditWndView::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	AdjustLayout();
}
//-------------------------------------------------------------------------------------//
BOOL CEditWndView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	//return true;
	return CWnd::OnEraseBkgnd(pDC);
}
//-------------------------------------------------------------------------------------//
void CEditWndView::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CWnd::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if ( TRUE == bShow )
	{
		int b = 4; 
		b = 7;
	}
}
//-------------------------------------------------------------------------------------//
LRESULT CEditWndView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class	
	HWND hWnd = NULL;
	switch ( message )
	{
	case MSG_MAIN_FRAME_MESSAGE:		
		switch ( wParam )
		{
		case WPARAM_PROJECT_NEW:
		case WPARAM_PROJECT_OPEN:
			BuildModelWndProp((CWnd*)lParam);
			break;
		case WPARAM_PROJECT_CLOSE:
			CloseProject();			
			break;
		case WPARAM_PROJECT_SWITCH:
			BuildModelWndProp(NULL);
			break;
		case WPARAM_PROJECT_UPDATE:
			BuildModelWndProp((CWnd*)lParam);
			break;
		case WPARAM_PROJECT_PART_DELETED:
			ClearModelWndProp();
			break;
		case WPARAM_PROJECT_CLOSE_ACTIVE_OBJ:
			ClearModelWndProp();
			break;
		}
		break;
	case MSG_EDIT_WND_PROPERTY_WND:
		switch ( wParam )
		{
		case WPARAM_BUILD_WND_SELECTED:
			BuildModelWndProp(NULL);
			//BuildWndGroupList();
			//UpdateClassComboxID();
			RefreshWndPtr();			
			if ( NULL != m_ActiveWndPtr )
			{
				if ( m_ActiveWndPtr->GetWndUIUpated_Param() == false )
				{
					UpdateWndGrupListSelected();
					const int WndGroupID = m_ActiveWndPtr->GetWndGroupID();
					BuildWndObjList(m_ModelPtr, WndGroupID);
					UpdateWndObjListSelected();				
				}
				else
				{	hWnd = NULL; }
			}
			else
			{	SendMessageToMainFrameWnd(MSG_EDIT_IMAGE_VIEW_WND, WPARAM_UPDATE_IMAGE_WND_SELECTED, (LPARAM)(NULL));	}
			break;
		case WPARAM_UPDATE_WND_SELECTED:
			RefreshWndPtr();
			if ( NULL != m_ActiveWndPtr )
			{
				if ( m_ActiveWndPtr->GetWndUIUpated_Param() == false )
				{
					UpdateWndGrupListSelected();
					UpdateWndObjListSelected();				
				}
				else
				{	hWnd = NULL; }
			}
			else
			{	SendMessageToMainFrameWnd(MSG_EDIT_IMAGE_VIEW_WND, WPARAM_UPDATE_IMAGE_WND_SELECTED, (LPARAM)(NULL));	}
			break;
		case WPARAM_REBUILD_WND_PARAM_LIST:
			BuildWndParamList();
			break;
		case WPARAM_EXEC_WND_ROI_ADD_AUTO:
			ExecWndParamListChanged_WndRoi_Add_Auto((CAOIWnd*)(lParam));
			break;
		}
		break;
	}	
	return CWnd::WindowProc(message, wParam, lParam);	
}
//-------------------------------------------------------------------------------------//
void CEditWndView::InitPropList()
{
	CString str1, str2, str3;
	CString strTrue, strFalse;
	strTrue = _T("True");
	strTrue = LoadMultiLanguageString(strTrue, strTrue);
	strFalse = _T("False");
	strFalse = LoadMultiLanguageString(strFalse, strFalse);	

	m_wndGroupList.EnableHeaderCtrl(FALSE);
	m_wndGroupList.EnableDescriptionArea(FALSE);
	m_wndGroupList.SetVSDotNetLook();
	m_wndGroupList.MarkModifiedProperties();
	m_wndGroupList.SetEnableLBtnDbClick(FALSE);
	m_wndGroupList.SetBoolLabels(strTrue, strFalse);
	
	str1 = _T("Wnd");
	str1 = LoadMultiLanguageString(str1, str1);
	str2 = _T("Result");
	str2 = LoadMultiLanguageString(str2, str2);
	//m_wndWndList.EnableHeaderCtrl(TRUE, _T("Wnd"), _T("Result"));
	m_wndWndList.EnableHeaderCtrl(TRUE, str1, str2);
	m_wndWndList.EnableDescriptionArea(FALSE);
	m_wndWndList.SetVSDotNetLook();
	//m_wndWndList.MarkModifiedProperties();	
	m_wndWndList.SetBoolLabels(strTrue, strFalse);

	m_wndWndParam.EnableHeaderCtrl(FALSE);
	m_wndWndParam.EnableDescriptionArea(FALSE);
	m_wndWndParam.SetVSDotNetLook();
	m_wndWndParam.MarkModifiedProperties();	
	m_wndWndParam.SetBoolLabels(strTrue, strFalse);
}
//-------------------------------------------------------------------------------------//
void CEditWndView::SetVSDotNetLook(BOOL bSet)
{
	m_wndGroupList.SetVSDotNetLook(bSet);
	m_wndGroupList.SetGroupNameFullWidth(bSet);
	
	m_wndWndList.SetVSDotNetLook(bSet);
	m_wndWndList.SetGroupNameFullWidth(bSet);

	m_wndWndParam.SetVSDotNetLook(bSet);
	m_wndWndParam.SetGroupNameFullWidth(bSet);
}
//-------------------------------------------------------------------------------------//
void CEditWndView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)	{	return;	}
	if ( m_wndClassCombo.GetSafeHwnd() == NULL ) { return; }

	CRect rectClient,rectCombo;
	GetClientRect(rectClient);
	m_wndClassCombo.GetWindowRect(&rectCombo);
	const int cx = rectClient.Width();
	const int cy = rectClient.Height();
	if ( 0==cx || 0==cy ) { return; }

	int cxCmb = rectClient.Width();
	int cyCmb = rectCombo.Size().cy;
	int cyTlb = 0;
	int cxBtn = 48;
	int cyBtn = cyCmb;	

	if ( m_wndClassCopyBtn.GetSafeHwnd()!=NULL || m_wndClassClearBtn.GetSafeHwnd() != NULL )
	{
		int rectBtnL=rectClient.right;
		if ( m_wndClassClearBtn.GetSafeHwnd() != NULL )
		{	
			rectBtnL -= cxBtn;
			m_wndClassClearBtn.SetWindowPos(NULL, rectBtnL, rectClient.top, cxBtn, cyBtn, SWP_NOACTIVATE | SWP_NOZORDER);	
		}
		if ( m_wndClassCopyBtn.GetSafeHwnd() != NULL )
		{
			rectBtnL -= cxBtn;
			m_wndClassCopyBtn.SetWindowPos(NULL, rectBtnL, rectClient.top, cxBtn, cyBtn, SWP_NOACTIVATE | SWP_NOZORDER);	
		}
		cxCmb = rectBtnL-rectClient.left;
	}
	else
	{	cyBtn = 0;	}	
	
	if ( m_wndClassCombo.GetSafeHwnd() != NULL )
	{	m_wndClassCombo.SetWindowPos(NULL, rectClient.left, rectClient.top, cxCmb, 200, SWP_NOACTIVATE | SWP_NOZORDER); }
	if ( m_wndToolBar.GetSafeHwnd() != NULL )
	{
		cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;
		m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER); 
	}

	if ( m_wndSplitter.GetSafeHwnd() != NULL )
	{	m_wndSplitter.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb + cyTlb, rectClient.Width(), rectClient.Height() -(cyCmb+cyTlb), SWP_NOACTIVATE | SWP_NOZORDER); }

	/*
	if ( m_wndGroupList.GetSafeHwnd() != NULL )
	{	m_wndGroupList.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb + cyTlb, rectClient.Width(), rectClient.Height() -(cyCmb+cyTlb), SWP_NOACTIVATE | SWP_NOZORDER); }

	if ( m_wndWndParam.GetSafeHwnd() != NULL )
	{	m_wndWndParam.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb + cyTlb, rectClient.Width(), rectClient.Height() -(cyCmb+cyTlb), SWP_NOACTIVATE | SWP_NOZORDER); }
	*/
}
//-------------------------------------------------------------------------------------//
void CEditWndView::SetPropListFont()
{
	const TSystemParameter &SysParam=AOIDataCollect.GetSystemParameter();
	const int nAdd=SysParam.m_UIWndFontAddSize;
	if ( 0 != nAdd ) { return ; }

	LOGFONT lf;
	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);
	afxGlobalData.fontRegular.GetLogFont(&lf);
	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	::DeleteObject(m_fntPropList.Detach());
	m_fntPropList.CreateFontIndirect(&lf);
	
	m_wndGroupList.SetFont(&m_fntPropList);
	m_wndWndList.SetFont(&m_fntPropList);
	m_wndWndParam.SetFont(&m_fntPropList);	
	m_wndClassCombo.SetFont(&m_fntPropList);
	m_wndClassCopyBtn.SetFont(&m_fntPropList);
	m_wndClassClearBtn.SetFont(&m_fntPropList);
}
//-------------------------------------------------------------------------------------//
void CEditWndView::PostMessageToMainFrameWnd(UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND hWnd = CWnd::GetSafeHwnd();
	AOIDataCollect.PostParentWndMessage(hWnd, message, wParam, lParam);	
	//AOIDataCollect.PostMainFrameWndMessage(message, wParam, lParam);	
}
//-------------------------------------------------------------------------------------//
void CEditWndView::SendMessageToMainFrameWnd(UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND hWnd = CWnd::GetSafeHwnd();
	AOIDataCollect.SendParentWndMessage(hWnd, message, wParam, lParam);	
	//AOIDataCollect.SendMainFrameWndMessage(message, wParam, lParam);	
}
//-------------------------------------------------------------------------------------//
void CEditWndView::SwitchMultiLanguage()
{
	/*
	size_t  i = 0;	
	UINT    WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_EDIT_WND_VIEW");	
	//---------------------------------------------------------------------------------//
	WndID = IDD_EDIT_WND_VIEW;
	WndKey = _T("IDD_EDIT_WND_VIEW");
	this->GetWindowText(LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetWindowText(NewLabelText);
	//---------------------------------------------------------------------------------//	
	*/
}
//-------------------------------------------------------------------------------------//
CString CEditWndView::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_EDIT_WND_VIEW");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
} 
//-------------------------------------------------------------------------------------//
COLORREF CEditWndView::GetResultColor(RESULT_ID ResultID, RESULT_ID LogicResultID)
{
	COLORREF Color;
	switch ( ResultID )
	{
	case RESULT_ID_NG:
		if ( RESULT_ID_OK == LogicResultID )
		{	Color=m_clrWarnning;	}
		else
		{	Color=m_clrNG;	}
		break;
	case RESULT_ID_EXCEPTION:
		Color=m_clrException;
		break;
	case RESULT_ID_OK:	
		Color=m_clrOK;	
		break;
	case RESULT_ID_SKIP:
		Color=m_clrSkip;	
		break;
	case RESULT_ID_BYPASS:
		Color=m_clrBypass;	
		break;
	case RESULT_ID_NONE:
		Color=m_clrUnTest;	
		break;	
	default:
		Color=CLR_DEFAULT;	
		break;
	}
	return Color;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::BuildModelWndProp(CWnd *SrcWndPtr)
{
	BOOL IsOK = TRUE;
	MODEL_ATTACHED_OBJ ModelAttachedObj = AOIDataCollect.GetModelAttachedObj();			
	switch ( ModelAttachedObj )	
	{
	case MODEL_ATTACHED_FD:
		IsOK = BuildModelWndProp_FD(SrcWndPtr);		
		break;
	case MODEL_ATTACHED_MARK:
		IsOK = BuildModelWndProp_Mark(SrcWndPtr);		
		break;
	case MODEL_ATTACHED_BARCODE:
		IsOK = BuildModelWndProp_Barcode(SrcWndPtr);
		break;
	case MODEL_ATTACHED_COMPONENT:
		IsOK = BuildModelWndProp_Component(SrcWndPtr);
		break;
	}
	return IsOK; 
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::BuildModelWndProp_FD(CWnd *SrcWndPtr)
{
	m_ProjectPtr = NULL;
	CAOIProject   *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL == ProjectPtr )
	{
		ClearModelWndProp();
		return true;
	}

	CAOIFd *FdPtr = ProjectPtr->GetProjectActiveFd();
	if ( NULL == FdPtr )
	{
		ClearModelWndProp();
		return true;
	}

	CAOIModel *ModelPtr = FdPtr->GetFdModelPtr();
	if ( NULL == ModelPtr )
	{
		ClearModelWndProp();
		return true;
	}
	m_ProjectPtr = ProjectPtr;
	if ( this == SrcWndPtr ) { return true; }
	int WndGroupID = -1;
	m_ModelPtr = ModelPtr;
	int ActClassID = ModelPtr->GetModelActClassID();
	CAOIWnd *WndPtr = ModelPtr->GetModelWndActived();
	if ( NULL != WndPtr )
	{	
		SetActiveWndPtr(WndPtr);
		WndGroupID = WndPtr->GetWndGroupID();
		if ( WndPtr->CheckWndClassIDUsed(ActClassID) == false )
		{	m_ActiveWndPtr = WndPtr = NULL; }		
	}
	else
	{	m_ActiveWndPtr = NULL;	}	
	ProjectPtr->SetProjectActiveModelWnd(WndPtr);

	UpdateClassComboxID();
	//BuildWndGroupList();
	UpdateWndGrupListSelected();
	BuildWndObjList(m_ModelPtr, WndGroupID);
	UpdateWndObjListSelected();	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::BuildModelWndProp_Mark(CWnd *SrcWndPtr)
{
	m_ProjectPtr = NULL;	
	CAOIProject   *ProjectPtr = AOIDataCollect.GetActiveTaskProject();
	if ( NULL == ProjectPtr )
	{
		ClearModelWndProp();
		return true;
	}

	CAOIMark *pMark = ProjectPtr->GetProjectActiveMark();
	if ( NULL == pMark )
	{
		ClearModelWndProp();
		return true;
	}

	CAOIModel *ModelPtr = pMark->GetMarkModelPtr();
	if ( NULL == ModelPtr )
	{
		ClearModelWndProp();
		return true;
	}
	m_ProjectPtr = ProjectPtr;
	if ( this == SrcWndPtr ) { return true; }
	int WndGroupID = -1;
	m_ModelPtr = ModelPtr;
	int ActClassID = ModelPtr->GetModelActClassID();
	CAOIWnd *WndPtr = ModelPtr->GetModelWndActived();
	if ( NULL != WndPtr )
	{	
		SetActiveWndPtr(WndPtr);
		WndGroupID = WndPtr->GetWndGroupID();
		if ( WndPtr->CheckWndClassIDUsed(ActClassID) == false )
		{	m_ActiveWndPtr = WndPtr = NULL; }		
	}
	else
	{	m_ActiveWndPtr = NULL;	}	
	ProjectPtr->SetProjectActiveModelWnd(WndPtr);
	
	UpdateClassComboxID();
	//BuildWndGroupList();	
	UpdateWndGrupListSelected();
	BuildWndObjList(m_ModelPtr, WndGroupID);
	UpdateWndObjListSelected();	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::BuildModelWndProp_Barcode(CWnd *SrcWndPtr)
{
	m_ProjectPtr = NULL;	
	CAOIProject   *ProjectPtr = AOIDataCollect.GetActiveTaskProject();
	if ( NULL == ProjectPtr )
	{
		ClearModelWndProp();
		return true;
	}

	CAOIBarcode *pBarcode = ProjectPtr->GetProjectActiveBarcode();
	if ( NULL == pBarcode )
	{
		ClearModelWndProp();
		return true;
	}

	CAOIModel *ModelPtr = pBarcode->GetBarcodeModelPtr();
	if ( NULL == ModelPtr )
	{
		ClearModelWndProp();
		return true;
	}
	m_ProjectPtr = ProjectPtr;
	if ( this == SrcWndPtr ) { return true; }
	int WndGroupID = -1;
	m_ModelPtr = ModelPtr;
	int ActClassID = ModelPtr->GetModelActClassID();
	CAOIWnd *WndPtr = ModelPtr->GetModelWndActived();
	if ( NULL != WndPtr )
	{	
		SetActiveWndPtr(WndPtr);
		WndGroupID = WndPtr->GetWndGroupID();
		if ( WndPtr->CheckWndClassIDUsed(ActClassID) == false )
		{	m_ActiveWndPtr = WndPtr = NULL; }		
	}
	else
	{	m_ActiveWndPtr = NULL;	}	
	ProjectPtr->SetProjectActiveModelWnd(WndPtr);

	UpdateClassComboxID();
	//BuildWndGroupList();
	UpdateWndGrupListSelected();
	BuildWndObjList(m_ModelPtr, WndGroupID);
	UpdateWndObjListSelected();	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::BuildModelWndProp_Component(CWnd *SrcWndPtr)
{
	m_ProjectPtr = NULL;
	CAOIProject   *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL == ProjectPtr )
	{
		ClearModelWndProp();
		return true;
	}

	CAOIComponent *pComponent = ProjectPtr->GetProjectActiveComponent();
	if ( NULL == pComponent )
	{
		ClearModelWndProp();
		return true;
	}

	CAOIModel *ModelPtr = pComponent->GetComponentModelPtr();
	if ( NULL == ModelPtr )
	{
		ClearModelWndProp();
		return true;
	}
	m_ProjectPtr = ProjectPtr;
	if ( this == SrcWndPtr ) { return true; }
	int WndGroupID = -1;
	m_ModelPtr = ModelPtr;
	int ActClassID = ModelPtr->GetModelActClassID();
	CAOIWnd *WndPtr = ModelPtr->GetModelWndActived();
	if ( NULL != WndPtr )
	{
		if ( WndPtr->CheckWndClassIDUsed(ActClassID) == false )
		{	WndPtr = NULL; }
	}
	if ( NULL == WndPtr )
	{
		const bool bFirstWnd=false;
		if ( true == bFirstWnd )
		{
			WndPtr = ModelPtr->GetModelWndFirst();
			ModelPtr->SetModelWndActived(WndPtr);
		}
	}
	if ( NULL != WndPtr )
	{			
		SetActiveWndPtr(WndPtr);
		WndGroupID = WndPtr->GetWndGroupID();		
	}
	else
	{	m_ActiveWndPtr = NULL;	}
	ProjectPtr->SetProjectActiveModelWnd(WndPtr);

	UpdateClassComboxID();
	//BuildWndGroupList();
	UpdateWndGrupListSelected();
	BuildWndObjList(m_ModelPtr, WndGroupID);
	UpdateWndObjListSelected();	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::UpdateModelWndProp()
{
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::ClearModelWndProp()
{
	m_ModelPtr = NULL;
	m_ActiveWndPtr = NULL;	
	ClearWndGroupList();
	ClearWndObjList();
	ClearWndParamList();
	return true;
}
//-------------------------------------------------------------------------------------//
CAOIModel* CEditWndView::GetModelPtr()
{
	return m_ModelPtr;
}
//-------------------------------------------------------------------------------------//
void CEditWndView::CloseProject()
{
	m_ProjectPtr = NULL;
	ClearModelWndProp();	
}
//-------------------------------------------------------------------------------------//
CAOIProject* CEditWndView::GetActiveProject()
{
	return m_ProjectPtr;
}
//-------------------------------------------------------------------------------------//
void CEditWndView::SetActiveWndPtr(CAOIWnd *WndPtr)
{
	if ( NULL != WndPtr )
	{	WndPtr->SetWndUIUpated_Param(false); }
	m_ActiveWndPtr = WndPtr;
	CheckWndClassIDVisible(WndPtr);
	return;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::CheckWndClassIDVisible(CAOIWnd *WndPtr)
{
	if ( NULL == WndPtr ) { return true; }
	if ( WndPtr->CheckWndClassIDUsed(m_ActClassID) == true )
	{	return true; }

	const int WndGroupID = WndPtr->GetWndGroupID();
	const int WndClassID = WndPtr->GetWndClassID();				
	JetAPI::SetComboxCurSel(m_wndClassCombo, WndClassID);
	BuildWndGroupListKernel(WndClassID);				
	UpdateWndGrupListSelected();
	BuildWndObjList(m_ModelPtr, WndGroupID);
	UpdateWndObjListSelected();	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::ResetClassCombox()
{
	m_wndClassCombo.SetCurSel(0);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::UpdateClassComboxID()
{	
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return true; }

	UpdateTextColor();	
	ResetClassCombox();
	ClearWndGroupList();
	ClearWndObjList();
	ClearWndParamList();		
	CAOIModel   *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return true; }
	CAOIRgn     *AttachedObj = ModelPtr->GetModelAttachedPtr();
	if ( NULL == AttachedObj ) { return true; }	

	const int ActClassID = ModelPtr->GetModelActClassID();
	JetAPI::SetComboxCurSel(m_wndClassCombo, ActClassID);

	bool bOK =true;
	m_wndGroupList.ShowWindow(SW_HIDE);
	bOK = BuildWndGroupListKernel(ActClassID);
	m_wndGroupList.ShowWindow(SW_SHOW);	
	return bOK;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::ShowWndGroupItem()
{
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return true; }
	if ( m_wndGroupList.GetSafeHwnd() == NULL ) { return true; }
	
	CAOIWnd *WndPtr = ModelPtr->GetModelWndActived();
	if ( NULL == WndPtr ) { return true; }
	CJETPropertyGridCtrl &wndPropList = m_wndGroupList;
	const int PropertyCount = wndPropList.GetPropertyCount();		

	int i=0, j=0;
	int WndGroupID2=0;
	int SubPropertyCount = 0;
	CMFCPropertyGridProperty *pProp=NULL;
	CMFCPropertyGridProperty *pPropSub=NULL;
	const int WndGroupID = WndPtr->GetWndGroupID();
	for ( i=0; i<PropertyCount; i++  )
	{
		pProp = wndPropList.GetProperty(i);
		if ( NULL == pProp ) { continue; }
		SubPropertyCount = pProp->GetSubItemsCount();
		for ( j=0; j<SubPropertyCount; j++ )
		{
			pPropSub = pProp->GetSubItem(j);
			if ( NULL == pPropSub ) { continue; }
			WndGroupID2 = pPropSub->GetData();
			if ( WndGroupID2 != WndGroupID ) { continue; }					
			wndPropList.EnsureVisible(pPropSub, FALSE);
			return true;
		}		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::BuildWndGroupList()
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return true; }
	const int ActClassID = 0;

	bool bOK = true;
	m_wndGroupList.ShowWindow(SW_HIDE);
	bOK = BuildWndGroupListKernel(ActClassID);
	m_wndGroupList.ShowWindow(SW_SHOW);
	return bOK;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::BuildWndGroupListKernel(int ActClassID)
{
	//CMFCPropertyGridProperty::m_strFormatFloat = _T("%f");	
	//CMFCPropertyGridProperty::m_strFormatDouble = _T("%.2f");
	UpdateTextColor();
	ClearWndGroupList();
	ClearWndObjList();
	ClearWndParamList();		
	CAOIModel   *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return true; }
	CAOIRgn     *AttachedObj = ModelPtr->GetModelAttachedPtr();
	if ( NULL == AttachedObj ) { return true; }	
	//const int ActClassID = ModelPtr->GetModelActClassID();
	const AOI_OBJ_TYPE AttachedType = AttachedObj->GetObjType();
	CJETPropertyGridCtrl &wndPropList = m_wndGroupList;

	ALG_TYPE     AlgType;
	COLORREF     clrText;
	int          WndGroupID=0;	
	int          LandGroupID=0;	
	bool         WndEnabled=true;
	double       FillImageTime=0.0;
	size_t       i=0, j=0, k=0, WndGroupCount=0;
	CString      str, str2; 
	CString      strCaption, strValue, strDescr;
	CAOIBox     *BoxPtr = NULL;
	CAOIWnd     *WndPtr = NULL;
	CAOIWnd     *WndPtr2 = NULL;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
	CAOILand    *LandPtr = NULL;	
	std::vector<int> WndGroupList;	
	CString       AttachedName;
	RESULT_ID     AlgResultID = RESULT_ID_NONE;
	RESULT_ID     WndGroupResultID=RESULT_ID_NONE;
	RESULT_ID     WndGroupLogicResultID=RESULT_ID_NONE;	
	WND_DEFECT_ID WndDefectID = WND_DEFECT_NONE;	
	const double  ModelInspectedTime = ModelPtr->GetModelInspectedTime();
	const int     MaxWndGroupID = ModelPtr->GetModelWndFreeGroupID();
	const int     MaxLandGroupID = ModelPtr->GetModelLandFreeGroupID();
	const size_t  ModelWndCount = ModelPtr->GetModelWndCount();
	const size_t  ModelLandCount = ModelPtr->GetModelLandCount();

	m_ActClassID = ActClassID;
	AOIDataCollect.SetIsModelWndGroupSelChange(true);
	if ( AOI_OBJ_FD == AttachedType )
	{
		str = AOIDataDefine.GetFdText();
		CAOIFd   *FdPtr = ModelPtr->GetModelFdPtr();
		if ( NULL != FdPtr )
		{
			FillImageTime = FdPtr->GetFdFillImageTime();
			AttachedName.Format(_T("%s %d"), str, FdPtr->GetFdIndex_Project()+1);
		}
		else
		{	AttachedName = _T("Fd-Undefined"); }
	}
	else if ( AOI_OBJ_BARCODE == AttachedType )
	{
		str = AOIDataDefine.GetBarcodeText();
		CAOIBarcode   *BarcodePtr = ModelPtr->GetModelBarcodePtr();				
		if ( NULL != BarcodePtr )
		{
			FillImageTime = BarcodePtr->GetBarcodeFillImageTime();
			AttachedName.Format(_T("%s %d"), str, BarcodePtr->GetBarcodeIndex_Project()+1);
		}
		else
		{	AttachedName = _T("Barcode-Undefined");	}
	}
	else if ( AOI_OBJ_MARK == AttachedType )
	{
		str = AOIDataDefine.GetMarkText();
		CAOIMark   *MarkPtr = ModelPtr->GetModelMarkPtr();				
		if ( NULL != MarkPtr )
		{
			FillImageTime = MarkPtr->GetMarkFillImageTime();			
			AttachedName.Format(_T("%s %04d"), str, MarkPtr->GetMarkIndex_Project()+1);
		}
		else
		{	AttachedName = _T("Mark-Undefined");	}
	}
	else if ( AOI_OBJ_COMPONENT == AttachedType )
	{
		str = AOIDataDefine.GetComponentText();
		CAOIComponent *ComponentPtr = ModelPtr->GetModelComponentPtr();			
		if ( NULL != ComponentPtr )
		{
			FillImageTime = ComponentPtr->GetComponentFillImageTime();
			AttachedName.Format(_T("%s %s"), str, ComponentPtr->GetComponentName());
		}
		else
		{	AttachedName = _T("Component-Undefined");	}		
	}
	else
	{	AttachedName = _T("Undefined");	}
	

	//Body Score	
	const BOOL bRedraw = FALSE;
	const BOOL bAdjustLayou = FALSE;
	CJETPropertyGridProperty* pWndItem = NULL;
	CJETPropertyGridProperty* pGroupLand = NULL;
	CJETPropertyGridProperty* pGroupBody = NULL;
	CJETPropertyGridProperty* pGroupModel = NULL;

	strCaption.Format(_T("%s T:%.0f+%.0f"), AttachedName, ModelInspectedTime, FillImageTime);

	wndPropList.SetRedraw(FALSE);
	/*
	//移至細部設定裡面
	pGroupModel = new CJETPropertyGridProperty(strCaption);//Model
	if ( NULL == pGroupModel ) { return false; }	
	pGroupModel->SetID(WND_GROUP_PROPERTY_MODEL_PARAM);

	if ( BuildWndGroupList_ModelExtendRange(&wndPropList, pGroupModel, ModelPtr) == false )
	{
		delete pGroupModel; pGroupModel=NULL;		
		return false;
	}
	pGroupModel->Expand(FALSE);
	wndPropList.AddProperty(pGroupModel, bRedraw, bAdjustLayou);
	*/
	str = _T("Body");
	str = LoadMultiLanguageString(str, str);
	str2.Format(_T("%s-%s"), strCaption, str);
	pGroupBody = new CJETPropertyGridProperty(str2);//Body
	if ( NULL == pGroupBody ) { return false; }	
	pGroupBody->SetID(WND_GROUP_PROPERTY_BODY_GROUP);
	pGroupBody->SetData(0);	

	WndGroupList.clear();
	for ( i=0; i<ModelWndCount; i++ )
	{
		WndPtr = ModelPtr->GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndLandPtr() != NULL ) { continue; }
		if ( WndPtr->CheckWndClassIDUsed(ActClassID) == false ) { continue; }
		WndGroupID = WndPtr->GetWndGroupID();		
		WndGroupCount = WndGroupList.size();
		for ( j=0; j<WndGroupCount; j++ )
		{
			if ( WndGroupID==WndGroupList[j] )
			{	break; }			
		}                                                                     
		if ( j != WndGroupCount ) { continue; }
		WndGroupList.push_back(WndGroupID);

		str = _T("Wnd in Body");		
		strDescr = LoadMultiLanguageString(str, str);
		WndEnabled = WndPtr->GetWndEnabled();		
		WndDefectID = WndPtr->GetWndDefectID();		
		AlgType     = WndPtr->GetWndAlgParam().GetAlgType();
		WndEnabled = ModelPtr->CheckModelWndGroupEnabled(WndGroupID);
		pWndItem = CreateGridPropertyAlgorithmList(AttachedType, WndGroupID, WndDefectID, AlgType, NULL, strDescr);
		if ( NULL == pWndItem ) { continue; }
		pWndItem->SetID(WND_GROUP_PROPERTY_WND_GROUP);
		pWndItem->SetCheckValue(WndEnabled);
		//pWndItem->Enable(FALSE);

		WndGroupResultID = ModelPtr->CheckModelWndGroupResultID(WndGroupID);
		WndGroupLogicResultID = ModelPtr->CheckModelWndGroupLogicResultID(WndGroupID);
		clrText = GetResultColor(WndGroupResultID, WndGroupLogicResultID);
		pWndItem->SetValueTextColor(clrText);
		pGroupBody->AddSubItem(pWndItem);
	}
	wndPropList.AddProperty(pGroupBody, bRedraw, bAdjustLayou);

	//Land
	for ( LandGroupID=0; LandGroupID<MaxLandGroupID; LandGroupID++ )
	{
		for ( i=0; i<ModelLandCount; i++ )
		{
			LandPtr = ModelPtr->GetModelLandPtr(i, false);
			if ( NULL == LandPtr ) { continue; }
			if ( LandGroupID == LandPtr->GetLandGroupID() ) { break; }
		}
		if ( i == ModelLandCount ) { continue; }

		str = _T("Land Group");
		str = LoadMultiLanguageString(str, str);		
		strCaption.Format(_T("%s_%d"), str, LandGroupID+1);
		pGroupLand = new CJETPropertyGridProperty(strCaption);
		if ( NULL == pGroupLand ) { return false; }	
		pGroupLand->SetID(WND_GROUP_PROPERTY_LAND_GROUP);
		pGroupLand->SetData(LandGroupID);
		
		WndGroupList.clear();
		for ( i=0; i<ModelWndCount; i++ )
		{
			WndPtr = ModelPtr->GetModelWndPtr(i, false);
			if ( NULL == WndPtr ) { continue; }
			LandPtr = WndPtr->GetWndLandPtr();
			if ( NULL == LandPtr ) { continue; }
			if ( LandGroupID != LandPtr->GetLandGroupID() ) { continue; }
			if ( WndPtr->CheckWndClassIDUsed(ActClassID) == false ) { continue; }
			
			WndGroupID = WndPtr->GetWndGroupID();
			WndGroupCount = WndGroupList.size();
			for ( j=0; j<WndGroupCount; j++ )
			{
				if ( WndGroupID==WndGroupList[j] )
				{	break; }			
			}
			if ( j != WndGroupCount ) { continue; }
			WndGroupList.push_back(WndGroupID);

			WndEnabled  = WndPtr->GetWndEnabled();
			WndDefectID = WndPtr->GetWndDefectID();
			AlgType     = WndPtr->GetWndAlgParam().GetAlgType();
			WndEnabled = ModelPtr->CheckModelWndGroupEnabled(WndGroupID);

			str = _T("Wnd in Land Group");
			str = LoadMultiLanguageString(str, str);
			strDescr.Format(_T("%s_%d"), str, LandGroupID+1);
			pWndItem = CreateGridPropertyAlgorithmList(AttachedType, WndGroupID, WndDefectID, AlgType, LandPtr, strDescr);
			if ( NULL == pWndItem ) { continue; }
			pWndItem->SetID(WND_GROUP_PROPERTY_WND_GROUP);
			pWndItem->SetCheckValue(WndEnabled);
			//pWndItem->Enable(FALSE);

			WndGroupResultID = ModelPtr->CheckModelWndGroupResultID(WndGroupID);
			WndGroupLogicResultID = ModelPtr->CheckModelWndGroupLogicResultID(WndGroupID);
			clrText = GetResultColor(WndGroupResultID, WndGroupLogicResultID);
			pWndItem->SetValueTextColor(clrText);
			pGroupLand->AddSubItem(pWndItem);			
		}
		wndPropList.AddProperty(pGroupLand, bRedraw, bAdjustLayou);
	}	

	if ( FALSE == bAdjustLayou )
	{	wndPropList.AdjustLayout(); }
	wndPropList.SetRedraw(TRUE);

	//移至目前選到的檢測框
	ShowWndGroupItem();	
	//
	//m_wndGroupList.Invalidate();
	//m_wndWndParam.Invalidate();
	//m_wndWndParam.RedrawWindow();
	this->Invalidate();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::BuildWndGroupList_ModelExtendRange(CJETPropertyGridCtrl *pCtrl, CJETPropertyGridProperty *pGroup, CAOIModel *ModelPtr)
{
	//return true;

	if ( NULL == pCtrl ) { return false; }
	if ( NULL == pGroup ) { return false; }	
	if ( NULL == ModelPtr ) { return false; }
	
	CString str;
	CString strCaption, strValue, strDescr, strUnit;
	CJETPropertyGridProperty *pParamItem = NULL;
	CJETPropertyGridProperty *pParamItemSub = NULL;
	const double dExtendX = ModelPtr->GetModelExtendRangeX();
	const double dExtendY = ModelPtr->GetModelExtendRangeY();
	
	strUnit = _T("um");	
	str = _T("Extend Range");
	strCaption = LoadMultiLanguageString(str, str);		
	strValue.Format(_T("%.0f"), dExtendX);
	pParamItem = new CJETPropertyGridProperty(strCaption, (DWORD_PTR)ModelPtr, TRUE);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_GROUP_PROPERTY_MODEL_EXTEND);
//	pParamItem->SetReading(strUnit);		
//	pGroup->AddSubItem(pParamItem);
	
	strUnit = _T("um");
	str = _T("X-Dir");
	strCaption = LoadMultiLanguageString(str, str);			
	strValue.Format(_T("%.0f"), dExtendX);
	pParamItemSub = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)ModelPtr);	
	if ( NULL == pParamItemSub ) { return false; }	
	pParamItemSub->SetID(WND_GROUP_PROPERTY_MODEL_EXTEND_X);
	pParamItemSub->SetReading(strUnit);		
	pParamItem->AddSubItem(pParamItemSub);
//	pGroup->AddSubItem(pParamItem);	

	str = _T("Y-Dir");
	strCaption = LoadMultiLanguageString(str, str);		
	strValue.Format(_T("%.0f"), dExtendY);
	pParamItemSub = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)ModelPtr);	
	if ( NULL == pParamItemSub ) { return false; }	
	pParamItemSub->SetID(WND_GROUP_PROPERTY_MODEL_EXTEND_Y);
	pParamItemSub->SetReading(strUnit);		
	pParamItem->AddSubItem(pParamItemSub);
//	pGroup->AddSubItem(pParamItem);
	
	//pCtrl->AddProperty(pParamItem);
	pGroup->AddSubItem(pParamItem);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::ClearWndGroupList()
{
	m_wndGroupList.RemoveAll();	
	//m_wndGroupList.AdjustLayout();
	m_wndGroupList.Invalidate();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::UpdateWndGrupListSelected()
{
	if ( NULL == m_ActiveWndPtr ) { return true; }
	if ( NULL == m_ModelPtr ) { return true; }	
	const int WndGroupID = m_ActiveWndPtr->GetWndGroupID();
	const int PropCount = m_wndGroupList.GetPropertyCount();	
	CMFCPropertyGridProperty *pProp=NULL ;	
	CMFCPropertyGridProperty *pPropLast=m_wndGroupList.GetCurSel();
	pProp = m_wndGroupList.FindItemByData((DWORD_PTR)(WndGroupID), TRUE);
	m_wndGroupList.SetCurSel(NULL, TRUE);
	if ( NULL != pProp )
	{	m_wndGroupList.SetCurSel(pProp, TRUE);	}
	else
	{	m_wndGroupList.SetCurSel(NULL, TRUE);	}	
	if ( NULL==pPropLast || pProp!=pPropLast )
	{	AOIDataCollect.SetIsModelWndGroupSelChange(true); }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::RefreshWndPtr()
{
	if ( NULL == m_ModelPtr )
	{	
		m_ActiveWndPtr = NULL;
		return true; 
	}
	CAOIWnd *WndPtr = m_ModelPtr->GetModelWndActived();	
	if ( WndPtr == m_ActiveWndPtr )
	{	return true;	}
	SetActiveWndPtr(WndPtr);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::BuildWndParamList()
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return true; }

	CAOIWnd *WndPtr = m_ActiveWndPtr;
	CAOIModel *ModelPtr = GetModelPtr();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ModelPtr || NULL==m_ActiveWndPtr )
	{
		ClearWndParamList();	
		SendMessageToMainFrameWnd(MSG_EDIT_IMAGE_VIEW_WND, WPARAM_UPDATE_IMAGE_WND_SELECTED, (LPARAM)(NULL));
		return true;
	}		
	if ( ModelPtr->CheckModelWndValid(WndPtr) == false )
	{
		ClearWndParamList();	
		SetActiveWndPtr(NULL);
		SendMessageToMainFrameWnd(MSG_EDIT_IMAGE_VIEW_WND, WPARAM_UPDATE_IMAGE_WND_SELECTED, (LPARAM)(NULL));
		return true;
	}
	if ( WndPtr->GetWndUIUpated_Param() == true ) 
	{	return true; }

	ClearWndParamList();	

	const bool bEnableUIWnd = GetEnableUIWnd();	
	if ( NULL != ProjectPtr )
	{	ProjectPtr->SetProjectActiveModelWnd(WndPtr); }
	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();	
	ALG_TYPE AlgType = AlgParam.GetAlgType();
	
	CAlgBinaryParam &BinaryParam = AlgParam.GetAlgImageBinParam();
	//BinaryParam.SetBinaryMode(BINARY_DISABLE);//先關閉二值化
	AOIDataCollect.SetBinaryParamTemp(BinaryParam);

	bool IsOK = true;
	//m_wndWndParam.ShowWindow(SW_HIDE);
	switch ( AlgType )
	{
	case ALG_BRIGHT_RATIO:	IsOK = BuildWndParamList_BrightRatio(ProjectPtr, WndPtr);	break;	
	case ALG_OUTER_SHORT:	IsOK = BuildWndParamList_OuterShort(ProjectPtr, WndPtr);	break;
	case ALG_BLOB_COUNT:	IsOK = BuildWndParamList_BlobCount(ProjectPtr, WndPtr);		break;
	case ALG_BODY_TILT:		IsOK = BuildWndParamList_BodyTilt(ProjectPtr, WndPtr);		break;
	case ALG_BARCODE_RECOGNIZE:	IsOK = BuildWndParamList_BarcodeRecognize(ProjectPtr, WndPtr);	break;
	case ALG_OBJECT_MEASURE:IsOK = BuildWndParamList_ObjectMeasure(ProjectPtr, WndPtr);	break;
	case ALG_COLOR_CODE:	IsOK = BuildWndParamList_ColorCode(ProjectPtr, WndPtr);		break;
	case ALG_MODEL_MATCH:	IsOK = BuildWndParamList_ModelMatch(ProjectPtr, WndPtr);	break;
	case ALG_IMAGE_MATCH:	IsOK = BuildWndParamList_ImageMatch(ProjectPtr, WndPtr);	break;
	case ALG_CHAR_VERIFY:	IsOK = BuildWndParamList_CharVerify(ProjectPtr, WndPtr);	break;
	case ALG_FD_MATCH:		IsOK = BuildWndParamList_FdMatch(ProjectPtr, WndPtr);		break;
	case ALG_EDGE_SEARCH:	IsOK = BuildWndParamList_EdgeSearch(ProjectPtr, WndPtr);	break;
	case ALG_SHAPE_VERIFY:	IsOK = BuildWndParamList_ShapeVerify(ProjectPtr, WndPtr);	break;
	case ALG_ANGLE_MEASURE:	IsOK = BuildWndParamList_AngleMeasure(ProjectPtr, WndPtr);	break;
	case ALG_PIXEL_COMPARE:	IsOK = BuildWndParamList_PixelCompare(ProjectPtr, WndPtr);	break;
	case ALG_WIDTH_RATIO:	IsOK = BuildWndParamList_WidthRatio(ProjectPtr, WndPtr);	break;
	case ALG_HEIGHT:		IsOK = BuildWndParamList_ResinHight(ProjectPtr, WndPtr);	break;	
	case ALG_WIRE_WIDTH:	IsOK = BuildWndParamList_WireWidth(ProjectPtr, WndPtr);	break;
	case ALG_SOLDER_WETTING:IsOK = BuildWndParamList_SolderWetting(ProjectPtr, WndPtr);	break;
	case ALG_MEASURE_BLACK_GLUE: IsOK = BuildWndParamList_MeasureBlackGlue(ProjectPtr, WndPtr); break;
	case ALG_MEASURE_FLUX_AREA: IsOK = BuildWndParamList_MeasureFluxArea(ProjectPtr, WndPtr); break;
	case ALG_MEASURE_CPU_PIN: IsOK = BuildWndParamList_MeasureCpuPin(ProjectPtr, WndPtr); break;
	case ALG_MEASURE_SIP_DISTANCE: IsOK = BuildWndParamList_MeasureSIP(ProjectPtr, WndPtr);	break;
	case ALG_MEASURE_CONNECTOR: IsOK = BuildWndParamList_MeasureConnector(ProjectPtr, WndPtr);	break;
	case ALG_MEASURE_CONNECTOR_PIN:IsOK = BuildWndParamList_MeasureConnector_Pin(ProjectPtr, WndPtr);	break;
		
	}
	if ( false == IsOK )
	{
		const BOOL bRedraw = FALSE;
		const BOOL bAdjustLayou = FALSE;
		if ( FALSE == bAdjustLayou )
		{	m_wndWndParam.AdjustLayout(); }
		m_wndWndParam.SetRedraw(TRUE);
	}
	//m_wndWndParam.ShowWindow(SW_SHOW);
	m_wndWndParam.EnableWindow(bEnableUIWnd);
	WndPtr->SetWndUIUpated_Param(true);
	AOIDataCollect.ExecAutoSwitchWnd3DFrame(ModelPtr, WndPtr);//20250609
	SendMessageToMainFrameWnd(MSG_EDIT_IMAGE_VIEW_WND, WPARAM_UPDATE_IMAGE_WND_SELECTED, (LPARAM)(WndPtr));	
	return IsOK;
}
//-------------------------------------------------------------------------------------//
CString CEditWndView::FormWndParamListCategoryName(CAOIWnd *WndPtr)
{	
	CString strCaption;
	if ( NULL == WndPtr ) { return strCaption; }

	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();	
	CString    AlgTypeText = WndPtr->GetWndAlgTypeText();
	unsigned int WndIndex = WndPtr->GetWndIndex();
	const int  WndGroupID = WndPtr->GetWndGroupID();
	const int  WndClassID = WndPtr->GetWndClassID();
	const WND_LOGIC_TYPE    WndLogicType = WndPtr->GetWndLogicType();

	//strCaption = AlgTypeText;
	switch ( WndLogicType )
	{
	case WND_LOGIC_GROUP_ID:
		strCaption.Format(_T("%s [Wnd:%d GP:%d] [L-GP]"), AlgTypeText, WndIndex+1, WndGroupID+1);
		break;
	case WND_LOGIC_DEFECT_ID:
		strCaption.Format(_T("%s [Wnd:%d GP:%d] [L-DF]"), AlgTypeText, WndIndex+1, WndGroupID+1);
		break;
	default:
		if ( 0 == WndClassID )
		{	strCaption.Format(_T("%s [Wnd:%d GP:%d]"), AlgTypeText, WndIndex+1, WndGroupID+1); }
		else
		{	strCaption.Format(_T("%s [Wnd:%d CLS:%d]"), AlgTypeText, WndIndex+1, WndClassID); }
		break;
	}	
	return strCaption;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::BuildWndParamList_RegionLink(CJETPropertyGridProperty *pGroup, CAOIWnd *WndPtr, bool bXYRatio)
{
	if ( NULL == WndPtr ) { return false; }	
	if ( NULL == pGroup ) { return false; }	

	CString strCaption, strValue, strDescr;
	CJETPropertyGridProperty *pParamItem = NULL;		

	strCaption = _T("Region Link");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	pParamItem = CreateGridPropertyWndRgnLinkModeList(strCaption, WndPtr, strDescr);
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_WND_RGN_LINK_MODE);	
	pGroup->AddSubItem(pParamItem);	

	if ( true == bXYRatio )
	{
		const double LinkRatioX = WndPtr->GetWndRgnLinkRatioX();
		strCaption = _T("Link RatioX ");
		strCaption = LoadMultiLanguageString(strCaption, strCaption);
		strValue.Format(_T("%.0f"), LinkRatioX);
		pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
		if ( NULL == pParamItem ) { return false; }	
		pParamItem->SetID(WND_ALG_PROPERTY_WND_RGN_LINK_RATIO_X);	
		pGroup->AddSubItem(pParamItem);

		const double LinkRatioY = WndPtr->GetWndRgnLinkRatioY();
		strCaption = _T("Link RatioY");
		strCaption = LoadMultiLanguageString(strCaption, strCaption);
		strValue.Format(_T("%.0f"), LinkRatioY);
		pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
		if ( NULL == pParamItem ) { return false; }	
		pParamItem->SetID(WND_ALG_PROPERTY_WND_RGN_LINK_RATIO_Y);	
		pGroup->AddSubItem(pParamItem);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::BuildWndParamList_WndDefectID(CJETPropertyGridProperty *pGroup, CAOIProject *Project, CAOIWnd *WndPtr)
{
	if ( NULL == WndPtr ) { return false; }
	if ( NULL == Project ) { return false; }
	if ( NULL == pGroup ) { return false; }		

	TFrameParam *FramePaamPtr = NULL;
	CString strCaption, strValue, strDescr;
	CJETPropertyGridProperty *pParamItem = NULL;		
	return true;

	bool       bEnabled=true;
	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();	

	//瑕疵代碼
	strCaption = _T("Defect Type");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	bEnabled = WndPtr->GetWndEnabled();
	pParamItem = CreateGridPropertyWndDefectList(strCaption, WndPtr, strDescr);
	if ( NULL == pParamItem ) { return false; }
	pParamItem->SetCheckValue(bEnabled);
	pParamItem->SetID(WND_ALG_PROPERTY_WND_DEFECT_ID);
	//pParamItem->Enable(FALSE);
	pGroup->AddSubItem(pParamItem);
	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::BuildWndParamList_AlgFrameIndex(CJETPropertyGridProperty *pGroup, CAOIProject *Project, CAOIWnd *WndPtr)
{
	if ( NULL == WndPtr ) { return false; }
	if ( NULL == Project ) { return false; }
	if ( NULL == pGroup ) { return false; }	
	
	CString strCaption, strValue, strDescr;
	CJETPropertyGridProperty *pParamItem = NULL;
	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	const CAlgBinaryParam &BinParam=AlgParam.GetAlgImageBinParam();	
	const unsigned int FrameUniqueID = BinParam.GetBinaryFrameUniqueID();

	//畫面列表
	strCaption = _T("Alg. Frame");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	pParamItem = CreateGridPropertyWndFrameList(strCaption, Project, WndPtr, FrameUniqueID, strDescr);
	if ( NULL == pParamItem ) { return false; }
	pParamItem->SetID(WND_ALG_PROPERTY_ALG_FRAME_ID);
	pGroup->AddSubItem(pParamItem);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::BuildWndParamList_MaskFrameIndex(CJETPropertyGridProperty *pGroup, CAOIProject *Project, CAOIWnd *WndPtr)
{
	if ( NULL == WndPtr ) { return false; }
	if ( NULL == Project ) { return false; }
	if ( NULL == pGroup ) { return false; }	
	
	CString strCaption, strValue, strDescr;
	CJETPropertyGridProperty *pParamItem = NULL;
	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	const CAlgBinaryParam &BinParam=AlgParam.GetAlgMaskBinParam();	
	const unsigned int FrameUniqueID = BinParam.GetBinaryFrameUniqueID();
	if ( AlgParam.CheckAlgMaskBinFrameUsed() == false ) { return true; }
	
	//畫面列表	
	strCaption = _T("Mask Frame");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	pParamItem = CreateGridPropertyWndFrameList(strCaption, Project, WndPtr, FrameUniqueID, strDescr);
	if ( NULL == pParamItem ) { return false; }
	pParamItem->SetID(WND_ALG_PROPERTY_MASK_FRAME_ID);	
	pGroup->AddSubItem(pParamItem);

	//遮罩功能
	MASK_FUNC_MODE MaskFuncMode;
	strCaption = _T("Mask Func");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	MaskFuncMode = BinParam.GetMaskFuncMode();
	strValue=AOIDataDefine.GetAlgMaskFuncModeText(MaskFuncMode);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }		
	strValue=AOIDataDefine.GetAlgMaskFuncModeText(MASK_FUNC_CALC);	pParamItem->AddOption(strValue);
	strValue=AOIDataDefine.GetAlgMaskFuncModeText(MASK_FUNC_ERASE);	pParamItem->AddOption(strValue);	
	pParamItem->SetID(WND_ALG_PROPERTY_MASK_FUNC_MODE);
	pGroup->AddSubItem(pParamItem);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::BuildWndParamList_MatchOffset(CJETPropertyGridProperty *pGroup, CAOIProject *Project, CAOIWnd *WndPtr, bool bX, bool bY, bool bA, bool bS)
{
	if ( NULL == WndPtr ) { return false; }
	if ( NULL == Project ) { return false; }
	if ( NULL == pGroup ) { return false; }	

	const COLORREF  clrOK = m_clrOK;
	const COLORREF  clrNG = m_clrNG;
	const COLORREF  clrBypass = m_clrBypass;	
	const COLORREF  clrUnTest = m_clrUnTest;

	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();

	bool    bIsPass = true;
	CString strCaption, strValue, strDescr, strUnit;
	CString strReadingX, strReadingY, strReadingA, strReadingSim, strReading;
	CJETPropertyGridProperty* pParamItem = NULL;

	const double dXMax = AlgParam.GetAlgOffsetXUSL();
	const double dXMin = AlgParam.GetAlgOffsetXLSL();
	const bool   bXEnabed = AlgParam.GetAlgOffsetXEnabled();
	const double dYMax = AlgParam.GetAlgOffsetYUSL();
	const double dYMin = AlgParam.GetAlgOffsetYLSL();
	const bool   bYEnabed = AlgParam.GetAlgOffsetYEnabled();
	const double dAMax = AlgParam.GetAlgSkewUSL();
	const double dAMin = AlgParam.GetAlgSkewLSL();
	const bool   bAEnabed = AlgParam.GetAlgSkewEnabled();
	const double dSimUSL = AlgParam.GetAlgPatternSimilarityUSL();
	const double dSimLSL = AlgParam.GetAlgPatternSimilarityLSL();
	const double dXReading = AlgParam.GetAlgOffsetXReading();
	const double dYReading = AlgParam.GetAlgOffsetYReading();
	const double dAReading = AlgParam.GetAlgSkewReading();
	const double dSimReading = AlgParam.GetAlgPatternSimilarityReading();

	strReadingX.Format(_T("%.0f"), dXReading);
	strReadingY.Format(_T("%.0f"), dYReading);
	strReadingA.Format(_T("%.2f"), dAReading);
	strReadingSim.Format(_T("%.0f"), dSimReading);

	if ( true==bX || true==bXEnabed )
	{
		strCaption = _T("X USL");	
		strCaption = LoadMultiLanguageString(strCaption, strCaption);	
		strValue.Format(_T("%.0f"), dXMax);
		pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
		if ( NULL == pParamItem ) { return false; }	
		pParamItem->SetID(WND_ALG_PROPERTY_PATTERN_MATCH_OFFSET_X_USL);
		pParamItem->SetReading(strReadingX);
		pParamItem->SetCheckValue(bXEnabed);
		if ( false == bXEnabed )
		{	pParamItem->SetReadingTextColor(clrBypass);	}
		else
		{
			bIsPass = CAlgParam::CheckOK_PatternMatchOffsetX(AlgParam);
			if ( false == bIsPass )
			{	pParamItem->SetReadingTextColor(clrNG); }
			else
			{	pParamItem->SetReadingTextColor(clrOK); }
		}
		pGroup->AddSubItem(pParamItem);
	}

	if ( true==bY || true==bYEnabed )
	{
		strCaption = _T("Y USL");	
		strCaption = LoadMultiLanguageString(strCaption, strCaption);	
		strValue.Format(_T("%.0f"), dYMax);
		pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
		if ( NULL == pParamItem ) { return false; }	
		pParamItem->SetID(WND_ALG_PROPERTY_PATTERN_MATCH_OFFSET_Y_USL);
		pParamItem->SetReading(strReadingY);	
		pParamItem->SetCheckValue(bYEnabed);
		if ( false == bYEnabed )
		{	pParamItem->SetReadingTextColor(clrBypass);	}
		else
		{
			bIsPass = CAlgParam::CheckOK_PatternMatchOffsetY(AlgParam);
			if ( false == bIsPass )
			{	pParamItem->SetReadingTextColor(clrNG); }
			else
			{	pParamItem->SetReadingTextColor(clrOK); }
		}
		pGroup->AddSubItem(pParamItem);	
	}

	const bool bLEnabed = AlgParam.GetAlgOffsetLEnabled();		
	const int ShowAlgOffsetLParam = AOIDataCollect.GetSystemParameter().m_ShowAlgOffsetLParam;
	if ( (FN_ENABLE==ShowAlgOffsetLParam ) || true==bLEnabed )
	{
		CString strReadingL;
		const double dxyLMax = AlgParam.GetAlgOffsetLUSL();
		const double dxyLMin = AlgParam.GetAlgOffsetLLSL();		
		strCaption = _T("L USL");	
		strCaption = LoadMultiLanguageString(strCaption, strCaption);	
		strValue.Format(_T("%.0f"), dxyLMax);
		strReadingL.Format(_T("%.2f"), AlgParam.GetAlgOffsetLReading());
		pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
		if ( NULL == pParamItem ) { return false; }	
		pParamItem->SetID(WND_ALG_PROPERTY_PATTERN_MATCH_OFFSET_L_USL);
		pParamItem->SetReading(strReadingL);	
		pParamItem->SetCheckValue(bLEnabed);
		if ( false == bLEnabed )
		{	pParamItem->SetReadingTextColor(clrBypass);	}
		else
		{
			bIsPass = CAlgParam::CheckOK_PatternMatchOffsetL(AlgParam);
			if ( false == bIsPass )
			{	pParamItem->SetReadingTextColor(clrNG); }
			else
			{	pParamItem->SetReadingTextColor(clrOK); }
		}
		pGroup->AddSubItem(pParamItem);	
	}

	const bool bxyAEnabed = AlgParam.GetAlgOffsetAEnabled();		
	const int ShowAlgOffsetAParam = AOIDataCollect.GetSystemParameter().m_ShowAlgOffsetAParam;
	if ( FN_ENABLE==ShowAlgOffsetAParam || true==bxyAEnabed )
	{
		CString strReadingxyA;
		const double dxyAMax = AlgParam.GetAlgOffsetAUSL();
		const double dxyAMin = AlgParam.GetAlgOffsetALSL();		
		strCaption = _T("A USL");	
		//strCaption = LoadMultiLanguageString(strCaption, strCaption);	
		strValue.Format(_T("%.2f"), dxyAMax);
		strReadingxyA.Format(_T("%.2f"), AlgParam.GetAlgOffsetAReading());
		pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
		if ( NULL == pParamItem ) { return false; }	
		pParamItem->SetID(WND_ALG_PROPERTY_PATTERN_MATCH_OFFSET_A_USL);
		pParamItem->SetReading(strReadingxyA);	
		pParamItem->SetCheckValue(bxyAEnabed);
		if ( false == bxyAEnabed )
		{	pParamItem->SetReadingTextColor(clrBypass);	}
		else
		{
			bIsPass = CAlgParam::CheckOK_PatternMatchOffsetA(AlgParam);
			if ( false == bIsPass )
			{	pParamItem->SetReadingTextColor(clrNG); }
			else
			{	pParamItem->SetReadingTextColor(clrOK); }
		}
		pGroup->AddSubItem(pParamItem);	
	}

	if ( true==bA || true==bAEnabed )
	{
		strCaption = _T("Angle USL");	
		strCaption = LoadMultiLanguageString(strCaption, strCaption);	
		strValue.Format(_T("%.2f"), dAMax);
		pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
		if ( NULL == pParamItem ) { return false; }	
		pParamItem->SetID(WND_ALG_PROPERTY_PATTERN_MATCH_SKEW_USL);
		pParamItem->SetReading(strReadingA);	
		pParamItem->SetCheckValue(bAEnabed);
		if ( false == bAEnabed )
		{	pParamItem->SetReadingTextColor(clrBypass);	}
		else
		{
			bIsPass = CAlgParam::CheckOK_PatternMatchSkewAngle(AlgParam);
			if ( false == bIsPass )
			{	pParamItem->SetReadingTextColor(clrNG); }
			else
			{	pParamItem->SetReadingTextColor(clrOK); }
		}
		pGroup->AddSubItem(pParamItem);	
	}

	if ( true == bS )
	{
		strCaption = _T("Score LSL");	
		strCaption = LoadMultiLanguageString(strCaption, strCaption);	
		strValue.Format(_T("%.0f"), dSimLSL);
		pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
		if ( NULL == pParamItem ) { return false; }	
		pParamItem->SetID(WND_ALG_PROPERTY_PATTERN_MATCH_SIMILARITY_LSL);
		pParamItem->SetReading(strReadingSim);	
		bIsPass = CAlgParam::CheckOK_PatternMatchScore(AlgParam);
		if ( false == bIsPass )
		{	pParamItem->SetReadingTextColor(clrNG); }
		else
		{	pParamItem->SetReadingTextColor(clrOK); }	
		pGroup->AddSubItem(pParamItem);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::BuildWndParamList_ModelMaskFlag(CJETPropertyGridCtrl &GridCtrl, CAOIProject *Project, CAOIWnd *WndPtr)
{	
	if ( NULL == WndPtr ) { return false; }
	if ( NULL == Project ) { return false; }

	const BOOL bRedraw = FALSE;
	const BOOL bAdjustLayou = FALSE;
	CString strCaption, strValue, strDescr, strUnit;
	CJETPropertyGridProperty *pGroup=NULL;	
	CJETPropertyGridProperty *pParamItem = NULL;
	WND_DEFECT_ID WndDefectID = WndPtr->GetWndDefectID();
	const int ModelMaskFlag = WndPtr->GetWndModelMaskFlag();
	const bool MaskPad = JetAPI::BitMask_Check(ModelMaskFlag, MODEL_MASK_PAD);
	const bool MaskBody = JetAPI::BitMask_Check(ModelMaskFlag, MODEL_MASK_BODY);
	const bool MaskBodyNoLead = JetAPI::BitMask_Check(ModelMaskFlag, MODEL_MASK_BODY_NO_LEAD);
	const bool MaskLead = JetAPI::BitMask_Check(ModelMaskFlag, MODEL_MASK_LEAD);
	const bool MaskLeadTip = JetAPI::BitMask_Check(ModelMaskFlag, MODEL_MASK_LEAD_TIP);
	const bool MaskLeadShoulder = JetAPI::BitMask_Check(ModelMaskFlag, MODEL_MASK_LEAD_SHOULDER);

	strCaption = _T("Model Mask");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	pGroup = new CJETPropertyGridProperty(strCaption);
	if ( NULL == pGroup ) { return false; }	
	pGroup->SetID(WND_ALG_PROPERTY_MODEL_MASK);		
	pGroup->SetData((DWORD_PTR)WndPtr);	

	if ( WndDefectID!=WND_DEFECT_PAD_ALIGN && WndDefectID!=WND_DEFECT_PAD_ADJUST )
	{
		strValue = _T("");
		strCaption = AOIDataDefine.GetModelMaskText(MODEL_MASK_PAD);	
		pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
		if ( NULL == pParamItem ) { return false; }	
		pParamItem->SetID(WND_ALG_PROPERTY_MODEL_MASK_PAD);	
		pParamItem->SetCheckValue(MaskPad);
		pGroup->AddSubItem(pParamItem);
	}

	//WND_DEFECT_PART_ALIGN		 =  0x00000002,//本體定位
	strValue = _T("");
	strCaption = AOIDataDefine.GetModelMaskText(MODEL_MASK_BODY);	
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_MODEL_MASK_BODY);	
	pParamItem->SetCheckValue(MaskBody);
	pParamItem->AllowEdit(FALSE);
	pGroup->AddSubItem(pParamItem);

	strValue = _T("");
	strCaption = AOIDataDefine.GetModelMaskText(MODEL_MASK_BODY_NO_LEAD);	
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_MODEL_MASK_BODY_NO_LEAD);	
	pParamItem->SetCheckValue(MaskBodyNoLead);
	pParamItem->AllowEdit(FALSE);
	pGroup->AddSubItem(pParamItem);	

	if ( WndDefectID!=WND_DEFECT_LEAD_ADJUST )
	{
		strValue = _T("");
		strCaption = AOIDataDefine.GetModelMaskText(MODEL_MASK_LEAD);	
		pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
		if ( NULL == pParamItem ) { return false; }	
		pParamItem->SetID(WND_ALG_PROPERTY_MODEL_MASK_LEAD);	
		pParamItem->SetCheckValue(MaskLead);
		pParamItem->AllowEdit(FALSE);
		pGroup->AddSubItem(pParamItem);
	
		strValue = _T("");
		strCaption = AOIDataDefine.GetModelMaskText(MODEL_MASK_LEAD_TIP);
		pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
		if ( NULL == pParamItem ) { return false; }	
		pParamItem->SetID(WND_ALG_PROPERTY_MODEL_MASK_LEAD_TIP);	
		pParamItem->SetCheckValue(MaskLeadTip);
		pParamItem->AllowEdit(FALSE);
		pGroup->AddSubItem(pParamItem);
	
		strValue = _T("");
		strCaption = AOIDataDefine.GetModelMaskText(MODEL_MASK_LEAD_SHOULDER);
		pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
		if ( NULL == pParamItem ) { return false; }	
		pParamItem->SetID(WND_ALG_PROPERTY_MODEL_MASK_LEAD_SHOULDER);	
		pParamItem->SetCheckValue(MaskLeadShoulder);
		pParamItem->AllowEdit(FALSE);
		pGroup->AddSubItem(pParamItem);
	}

	strValue = _T("");
	strCaption = _T("Clear");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_MODEL_MASK_CLEAR);		
	pParamItem->AllowEdit(FALSE);
	pParamItem->SetHasUserBtn();
	pGroup->AddSubItem(pParamItem);	

	pGroup->Expand(FALSE);	
	GridCtrl.AddProperty(pGroup, bRedraw, bAdjustLayou);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::BuildWndParamList_ScaleRatio(CJETPropertyGridCtrl &GridCtrl, CAOIProject *Project, CAOIWnd *WndPtr)
{
	if ( NULL == WndPtr ) { return false; }
	if ( NULL == Project ) { return false; }
	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	const COLORREF  clrOK = m_clrOK;
	const COLORREF  clrNG = m_clrNG;
	const COLORREF  clrUnTest = m_clrUnTest;	

	const BOOL bRedraw = FALSE;
	const BOOL bAdjustLayou = FALSE;
	bool    bIsPass = true;
	CString strCaption, strValue, strDescr, strUnit;
	CString strReadingSMax, strReadingSMin;	
	CJETPropertyGridProperty *pGroup=NULL;
	CJETPropertyGridProperty *pParamItem = NULL;
	const double dSMax = AlgParam.GetAlgScaleUSL();
	const double dSMin = AlgParam.GetAlgScaleLSL();
	const bool   bSEnabed = AlgParam.GetAlgScaleEnabled();	
	const double dSXReading = AlgParam.GetAlgScaleXReading();	
	const double dSYReading = AlgParam.GetAlgScaleYReading();	
	const double dSMaxReading = MAX(dSXReading, dSYReading);
	const double dSMinReading = MIN(dSXReading, dSYReading);
	strReadingSMax.Format(_T("%.0f"), dSMaxReading);	
	strReadingSMin.Format(_T("%.0f"), dSMinReading);	

	strCaption = _T("Scale Ratio");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	pGroup = new CJETPropertyGridProperty(strCaption);
	if ( NULL == pGroup ) { return false; }	
	pGroup->SetID(WND_ALG_PROPERTY_PATTERN_MATCH_SCALE_NODE);	
	pGroup->SetData((DWORD_PTR)WndPtr);
	//pGroupBasic->AddSubItem(pGroupScale);

	strCaption = _T("Enable");	
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue = _T("");
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_PATTERN_MATCH_SCALE_ENABLE);
	pParamItem->SetCheckValue(bSEnabed);
	pParamItem->AllowEdit(FALSE);
	pGroup->AddSubItem(pParamItem);	

	strCaption = _T("Scale USL");	
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%.0f"), dSMax);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_PATTERN_MATCH_SCALE_USL);
	pParamItem->SetReading(strReadingSMax);		
	bIsPass = CAlgParam::CheckOK_PatternMatchScaleUSL(AlgParam);
	if ( false == bIsPass )
	{	pParamItem->SetReadingTextColor(clrNG); }
	else
	{	pParamItem->SetReadingTextColor(clrOK); }
	pGroup->AddSubItem(pParamItem);	

	strCaption = _T("Scale LSL");	
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%.0f"), dSMin);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_PATTERN_MATCH_SCALE_LSL);
	pParamItem->SetReading(strReadingSMin);		
	bIsPass = CAlgParam::CheckOK_PatternMatchScaleLSL(AlgParam);
	if ( false == bIsPass )
	{	pParamItem->SetReadingTextColor(clrNG); }
	else
	{	pParamItem->SetReadingTextColor(clrOK); }
	pGroup->AddSubItem(pParamItem);	
	pGroup->Expand(bSEnabed);	
	GridCtrl.AddProperty(pGroup, bRedraw, bAdjustLayou);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::BuildWndParamList_ExtendRange(CJETPropertyGridCtrl &GridCtrl, CAOIProject *Project, CAOIWnd *WndPtr)
{
	//if ( NULL == pCtrl ) { return false; }
	if ( NULL == WndPtr ) { return false; }
	if ( NULL == Project ) { return false; }
	//if ( NULL == pGroup ) { return false; }

	const BOOL bRedraw = FALSE;
	const BOOL bAdjustLayou = FALSE;
	CString strCaption, strValue, strDescr, strUnit;
	CJETPropertyGridProperty *pGroup=NULL;
	//CJETPropertyGridProperty *pParamItem = NULL;
	CJETPropertyGridProperty *pParamItemSub = NULL;
	const double dExtendX = WndPtr->GetWndExtendRangeX();
	const double dExtendY = WndPtr->GetWndExtendRangeY();	

	strCaption = _T("Ext. Range");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	pGroup = new CJETPropertyGridProperty(strCaption);
	if ( NULL == pGroup ) { return false; }	
	pGroup->SetID(WND_ALG_PROPERTY_EXTEND_RANGE);
	pGroup->SetData((DWORD_PTR)WndPtr);

	/*
	strUnit = _T("um");	
	strCaption = _T("Ext. Range");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	pParamItem = new CJETPropertyGridProperty(strCaption, (DWORD_PTR)WndPtr, TRUE);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_EXTEND_RANGE);
	pParamItem->SetData((DWORD_PTR)WndPtr);		
	//pParamItem->SetReading(strUnit);	
	*/

	strUnit = _T("um");	
	strCaption = _T("X Dir.");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.0f"), dExtendX);
	pParamItemSub = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItemSub ) { return false; }	
	pParamItemSub->SetID(WND_ALG_PROPERTY_EXTEND_RANGE_X);
	//pParamItemSub->EnableSpinControl(TRUE, 0, 300);
	pParamItemSub->SetReading(strUnit);		
	pGroup->AddSubItem(pParamItemSub);

	strUnit = _T("um");	
	strCaption = _T("Y Dir.");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.0f"), dExtendY);
	pParamItemSub = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItemSub ) { return false; }	
	pParamItemSub->SetID(WND_ALG_PROPERTY_EXTEND_RANGE_Y);
	//pParamItemSub->EnableSpinControl(TRUE, 0, 300);
	pParamItemSub->SetReading(strUnit);		
	pGroup->AddSubItem(pParamItemSub);	

	//pGroup->AddSubItem(pParamItem);	
	//GridCtrl.AddProperty(pParamItem, bRedraw, bAdjustLayou);
	pGroup->Expand(FALSE);
	GridCtrl.AddProperty(pGroup, bRedraw, bAdjustLayou);

	/*
	strUnit = _T("um");
	strCaption = _T("X外擴範圍");
	strValue.Format(_T("%.0f"), dExtendX);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_EXTEND_RANGE_X);
	pParamItem->SetReading(strUnit);		
	pGroup->AddSubItem(pParamItem);

	strCaption = _T("Y外擴範圍");
	strValue.Format(_T("%.0f"), dExtendY);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_EXTEND_RANGE_Y);
	pParamItem->SetReading(strUnit);		
	pGroup->AddSubItem(pParamItem);
	*/
	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::BuildWndParamList_PixelCompare(CJETPropertyGridCtrl &GridCtrl, CAOIProject *Project, CAOIWnd *WndPtr)
{
	if ( NULL == WndPtr ) { return false; }
	if ( NULL == Project ) { return false; }
	if ( GridCtrl.GetSafeHwnd() == NULL ) { return false; }
	
	BOOL  bExpand = FALSE;
	const BOOL bRedraw = FALSE;
	const BOOL bAdjustLayou = FALSE;
	CString strTitle;
	CString strCaption, strValue, strDescr, strUnit, strReading;
	const COLORREF  clrOK = m_clrOK;
	const COLORREF  clrNG = m_clrNG;
	const COLORREF  clrUnTest = m_clrUnTest;
	
	CJETPropertyGridProperty *pGroup=NULL;
	CJETPropertyGridProperty *pParamItem = NULL;
	CJETPropertyGridProperty *pParamItemSub = NULL;	
	TALG_PARAM_IMAGE_MATCH &imParam = WndPtr->GetWndAlgParam().GetAlgParamImageMatch();
	
	const bool bEnabled = imParam.imPxlCmpEnabled;
	const int nDarkLevel = imParam.imPxlCmpDarkLevel;
	const int nTolerance = imParam.imPxlCmpTolerance;
	const int nGaussianSize = imParam.imPxlCmpGaussianSize;
	const int nOpenSize = imParam.imPxlCmpOpenSize;
	const int nCloseSize = imParam.imPxlCmpCloseSize;
	const double dXSizeMin = imParam.imPxlCmpXSizeMin;
	const double dYSizeMin = imParam.imPxlCmpYSizeMin;
	const double dAreaMin = imParam.imPxlCmpAreaMin;
	const int nCountUSL = imParam.imPxlCmpCountUSL;
	const int nCountLSL = imParam.imPxlCmpCountLSL;
	const int nCountNum = imParam.imPxlCmpCountNum;
	
	strCaption = _T("Pixel Compare");	
	strTitle = strCaption = LoadMultiLanguageString(strCaption, strCaption);
	pGroup = new CJETPropertyGridProperty(strCaption);
	if ( NULL == pGroup ) { return false; }	
	pGroup->SetID(WND_ALG_PROPERTY_IMAGE_MATCH_PXL_CMP_ENABLED);
	pGroup->SetCheckValue(bEnabled);
	pGroup->SetData((DWORD_PTR)WndPtr);

	strCaption = _T("Dark Level");	
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%d"), nDarkLevel);	
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_IMAGE_MATCH_PXL_CMP_DARK_LEVEL);	
	pGroup->AddSubItem(pParamItem);	

	strCaption = _T("Tolerance");	
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%d"), nTolerance);	
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_IMAGE_MATCH_PXL_CMP_TOLERANCE);	
	pGroup->AddSubItem(pParamItem);	
	
	strCaption = _T("Smooth Size");	
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%d"), nGaussianSize);	
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_IMAGE_MATCH_PXL_CMP_GAUSSIAN_SIZE);	
	pGroup->AddSubItem(pParamItem);	

	strCaption = _T("Open Size");	
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%d"), nOpenSize);	
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_IMAGE_MATCH_PXL_CMP_OPEN_SIZE);	
	pGroup->AddSubItem(pParamItem);	

	strCaption = _T("Close Size");	
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%d"), nCloseSize);	
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_IMAGE_MATCH_PXL_CMP_CLOSE_SIZE);	
	pGroup->AddSubItem(pParamItem);	
	
	strUnit = _T("um");
	strCaption = _T("X Size LSL");	
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%.0f"), dXSizeMin);	
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }		
	pParamItem->SetReading(strUnit);
	pParamItem->SetID(WND_ALG_PROPERTY_IMAGE_MATCH_PXL_CMP_X_SIZE_MIN);	
	pGroup->AddSubItem(pParamItem);	

	strCaption = _T("Y Size LSL");	
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%.0f"), dYSizeMin);	
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetReading(strUnit);
	pParamItem->SetID(WND_ALG_PROPERTY_IMAGE_MATCH_PXL_CMP_Y_SIZE_MIN);	
	pGroup->AddSubItem(pParamItem);	

	strUnit = _T("um^2");
	strCaption = _T("Area LSL");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%.0f"), dAreaMin);	
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }		
	pParamItem->SetReading(strUnit);
	pParamItem->SetID(WND_ALG_PROPERTY_IMAGE_MATCH_PXL_CMP_AREA_MIN);	
	pGroup->AddSubItem(pParamItem);	

	strCaption = _T("Count USL");	
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%d"), nCountUSL);
	strReading.Format(_T("%d"), nCountNum);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_IMAGE_MATCH_PXL_CMP_COUNT_USL);
	pParamItem->SetReading(strReading);	
	if ( nCountNum>nCountUSL || nCountNum<nCountLSL )
	{	pParamItem->SetReadingTextColor(clrNG);	}
	else
	{	pParamItem->SetReadingTextColor(clrOK);	}
	pGroup->AddSubItem(pParamItem);	

	if ( true == bEnabled )
	{
		if ( nCountNum>nCountUSL || nCountNum<nCountLSL )
		{	
			bExpand = TRUE;
			strValue = AOIDataDefine.GetCountText();
			strCaption.Format(_T("%s [%s:%d]"), strTitle, strValue, nCountNum);			
		}
		else
		{	strCaption.Format(_T("%s [%s]"), strTitle, _T("OK"));	}
		pGroup->SetName(strCaption);
	}

	pGroup->Expand(bExpand);
	GridCtrl.AddProperty(pGroup, bRedraw, bAdjustLayou);
	return true;
}
//-------------------------------------------------------------------------------------//
bool  CEditWndView::BuildWndParamList_LogicParam(CJETPropertyGridCtrl &GridCtrl, CAOIProject *Project, CAOIWnd *WndPtr)
{
	if ( NULL == WndPtr ) { return false; }
	if ( NULL == Project ) { return false; }
	//if ( NULL == pGroup ) { return false; }	
	
	CString    str;
	const BOOL bRedraw = FALSE;
	const BOOL bAdjustLayou = FALSE;	
	CString strCaption, strValue, strDescr;
	CString    strID = AOIDataDefine.GetIDText();	
	CJETPropertyGridProperty *pGroup=NULL;
	CJETPropertyGridProperty *pParamItem = NULL;
	const int LogicGroupID = WndPtr->GetWndLogicGroupID();

	str = _T("Logic");
	str = LoadMultiLanguageString(str, str);
	strCaption.Format(_T("%s [%s:%d]"), str, strID, LogicGroupID+1);
	pGroup = new CJETPropertyGridProperty(strCaption);
	if ( NULL == pGroup ) { return false; }	
	pGroup->SetID(WND_ALG_PROPERTY_WND_LOGIC_GROUP);
	pGroup->SetData((DWORD_PTR)WndPtr);

	//邏輯樣式
	strCaption = _T("Logic Type");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	pParamItem = CreateGridPropertyWndLogicTypeList(strCaption, WndPtr, strDescr);
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_WND_LOGIC_TYPE);	
	pGroup->AddSubItem(pParamItem);

	//邏輯群組編號
	strCaption = _T("Logic Group ID");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	pParamItem = CreateGridPropertyWndLogicGroupIDList(strCaption, WndPtr, strDescr);
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_WND_LOGIC_GROUP_ID);	
	pGroup->AddSubItem(pParamItem);		

	pGroup->Expand(FALSE);
	GridCtrl.AddProperty(pGroup, bRedraw, bAdjustLayou);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::BuildWndParamList_BaseValue(CJETPropertyGridCtrl &GridCtrl, CAOIProject *Project, CAOIWnd *WndPtr)
{
	if ( NULL == WndPtr ) { return false; }
	if ( NULL == Project ) { return false; }
	//if ( NULL == pGroup ) { return false; }	
		
	CString    str;
	const BOOL bRedraw = FALSE;
	const BOOL bAdjustLayou = FALSE;
	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	CString strCaption, strValue, strDescr;	
	CJETPropertyGridProperty *pGroup=NULL;
	CJETPropertyGridProperty *pParamItem = NULL;
	CString    strID = AOIDataDefine.GetIDText();
	CString    strResult = AOIDataDefine.GetResultText();	
	const int  nGroupID = AlgParam.GetAlgBaseValueGroupID()+1;
	const bool bEnabled = AlgParam.GetAlgBaseValueEnabled();
	const double Reading = AlgParam.GetAlgBaseValueReading();

	str = _T("Base Value");
	str = LoadMultiLanguageString(str, str);
	strCaption.Format(_T("%s [%s:%.0f %s:%d ]"), str, strResult, Reading, strID, nGroupID);
	pGroup = new CJETPropertyGridProperty(strCaption);
	if ( NULL == pGroup ) { return false; }	
	pGroup->SetID(WND_ALG_PROPERTY_ALG_BASE_VALUE_GROUP);
	pGroup->SetData((DWORD_PTR)WndPtr);	

	//基準值啟用
	strCaption = _T("Base Value");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%.0f"), Reading);	
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_ALG_BASE_VALUE_ENABLED);	
	pParamItem->SetCheckValue(bEnabled);		
	pParamItem->AllowEdit(FALSE);
	pGroup->AddSubItem(pParamItem);		

	//基準值群組編號
	strCaption = _T("Base Value Group ID");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	pParamItem = CreateGridPropertyAlgBaseValueGroupIDList(strCaption, WndPtr, strDescr);
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_ALG_BASE_VALUE_GROUP_ID);	
	pGroup->AddSubItem(pParamItem);	

	pGroup->Expand(FALSE);
	GridCtrl.AddProperty(pGroup, bRedraw, bAdjustLayou);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::BuildWndParamList_SaveDefectImage(CJETPropertyGridProperty *pGroup, CAOIProject *Project, CAOIWnd *WndPtr)
{
	if ( NULL == WndPtr ) { return false; }
	if ( NULL == Project ) { return false; }
	if ( NULL == pGroup ) { return false; }		
	
	CString str;
	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	CString strCaption, strValue, strDescr;
	CJETPropertyGridProperty *pParamItem = NULL;			
	const bool bEnabled = AlgParam.GetAlgSaveDefectImageEnabled();
	
	//儲存瑕疵圖像		
	strCaption = _T("Save Defect Image");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	//pParamItem =  new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	pParamItem = new CJETPropertyGridProperty(strCaption, (_variant_t)bEnabled, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_ALG_SAVE_DEFECT_IMAGE);	
	//pParamItem->SetCheckValue(bEnabled);	
	pGroup->AddSubItem(pParamItem);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::BuildWndParamList_FollowMode(CJETPropertyGridProperty *pGroup, CAOIProject *Project, CAOIWnd *WndPtr)
{
	if ( NULL == WndPtr ) { return false; }
	if ( NULL == Project ) { return false; }
	if ( NULL == pGroup ) { return false; }	
	
	CString strCaption, strValue, strDescr;
	CJETPropertyGridProperty *pParamItem = NULL;			
	
	//跟隨補正
	strCaption = _T("Follow Mode");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	pParamItem = CreateGridPropertyWndFollowModeList(strCaption, WndPtr, strDescr);
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_WND_FOLLOW_MODE);	
	pGroup->AddSubItem(pParamItem);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::BuildWndParamList_ResultText(CJETPropertyGridProperty *pGroup, CAOIProject *Project, CAOIWnd *WndPtr)
{
	if ( NULL == WndPtr ) { return false; }
	if ( NULL == Project ) { return false; }
	if ( NULL == pGroup ) { return false; }	

	CString strCaption, strValue, strDescr;
	CJETPropertyGridProperty *pParamItem = NULL;			
	
	RESULT_ID ResultID = WndPtr->GetWndResultID();
	RESULT_ID LogicResultID = WndPtr->GetWndLogicResultID();
	COLORREF clrText = GetResultColor(ResultID, LogicResultID);

	//結果文字
	strCaption = _T("Result");	
	strCaption = AOIDataDefine.GetResultText();
	strValue = WndPtr->GetWndResultText();
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_WND_RESULT_TEXT);		
	pParamItem->SetValueTextColor(clrText);
	pParamItem->AllowEdit(FALSE);
	pGroup->AddSubItem(pParamItem);		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::BuildWndParamList_SyncMoveMode(CJETPropertyGridProperty *pGroup, CAOIProject *Project, CAOIWnd *WndPtr)
{
	if ( NULL == WndPtr ) { return false; }
	if ( NULL == Project ) { return false; }
	if ( NULL == pGroup ) { return false; }	
	
	CString strCaption, strValue, strDescr;
	CJETPropertyGridProperty *pParamItem = NULL;			
	
	//連動模式
	strCaption = _T("Sync Move Mode");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	pParamItem = CreateGridPropertyWndSyncMoveModeList(strCaption, WndPtr, strDescr);
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_WND_SYNC_MOVE_MODE);	
	pGroup->AddSubItem(pParamItem);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::BuildWndParamList_ConstrainMode(CJETPropertyGridProperty *pGroup, CAOIProject *Project, CAOIWnd *WndPtr)
{
	if ( NULL == WndPtr ) { return false; }
	if ( NULL == Project ) { return false; }
	if ( NULL == pGroup ) { return false; }	
	
	CString strCaption, strValue, strDescr;
	CJETPropertyGridProperty *pParamItem = NULL;			
	
	//侷限模式
	strCaption = _T("Constrain Mode");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	pParamItem = CreateGridPropertyWndConstrainModeList(strCaption, WndPtr, strDescr);
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_WND_CONSTRAIN_MODE);	
	pGroup->AddSubItem(pParamItem);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::BuildWndParamList_ClassID(CJETPropertyGridProperty *pGroup, CAOIProject *Project, CAOIWnd *WndPtr)
{
	if ( NULL == WndPtr ) { return false; }
	if ( NULL == Project ) { return false; }
	if ( NULL == pGroup ) { return false; }	
	
	bool    bShow=false;
	CString str;
	CString strCaption, strValue, strDescr;
	CJETPropertyGridProperty *pParamItem = NULL;			
	const int  ClassID = WndPtr->GetWndClassID();
	
	//類別編號		
	strCaption = _T("Class ID");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%d"), ClassID);	
	pParamItem =  new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_WND_CLASS_ID);	
	
	for ( int i=0; i<MODEL_CLASS_ID_COUNT; i++ )
	{
		str.Format(_T("%d"), i);
		pParamItem->AddOption(str);
	}
	pParamItem->AllowEdit(FALSE);
	pGroup->AddSubItem(pParamItem);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::BuildWndParamList_BoxShape(CJETPropertyGridProperty *pGroup, CAOIProject *Project, CAOIWnd *WndPtr)
{
	if ( NULL == WndPtr ) { return false; }
	if ( NULL == Project ) { return false; }
	if ( NULL == pGroup ) { return false; }	
	
	bool    bShow=false;
	CString strCaption, strValue, strDescr;
	CJETPropertyGridProperty *pParamItem = NULL;			
	BOX_SHAPE_MODE WndShapeMode = WndPtr->GetWndShapeMode();
	
	//框外型樣式
	strCaption = _T("Box Shape");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	pParamItem = CreateGridPropertyWndBoxShapeModeList(strCaption, WndPtr, strDescr);
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_WND_BOX_SHAPE_MODE);	
	pGroup->AddSubItem(pParamItem);
	
	strCaption = _T("Shape Param");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.2f"), WndPtr->GetWndShapeParam());
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }
	pParamItem->SetID(WND_ALG_PROPERTY_WND_BOX_SHAPE_PARAM_1);
	pGroup->AddSubItem(pParamItem);
	bShow = CAOIBox::CheckBoxShapeUseParam1(WndShapeMode);
	pParamItem->Show(bShow);	
	
	strCaption = _T("Shape Param2");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.2f"), WndPtr->GetWndShapeParam2());
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }
	pParamItem->SetID(WND_ALG_PROPERTY_WND_BOX_SHAPE_PARAM_2);
	pGroup->AddSubItem(pParamItem);
	bShow = CAOIBox::CheckBoxShapeUseParam2(WndShapeMode);
	pParamItem->Show(bShow);
	return true;
}
//-------------------------------------------------------------------------------------//
bool  CEditWndView::BuildWndParamList_DefectGroupID(CJETPropertyGridProperty *pGroup, CAOIProject *Project, CAOIWnd *WndPtr)
{
	if ( NULL == WndPtr ) { return false; }
	if ( NULL == Project ) { return false; }
	if ( NULL == pGroup ) { return false; }	

	CString strCaption, strValue, strDescr;
	CJETPropertyGridProperty *pParamItem = NULL;			

	//瑕疵群組代碼	
	strCaption = _T("Defect Group");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%d"), WndPtr->GetWndDefectGroupID()+1);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }
	pParamItem->SetID(WND_ALG_PROPERTY_DEFECT_GROUP_ID);
	pGroup->AddSubItem(pParamItem);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::BuildWndParamList_AdvanceGeneral(CJETPropertyGridProperty *pGroup, CAOIProject *Project, CAOIWnd *WndPtr)
{
	if ( NULL == WndPtr ) { return false; }
	if ( NULL == Project ) { return false; }
	if ( NULL == pGroup ) { return false; }	
	ALG_TYPE AlgType = WndPtr->GetWndAlgType();
	const bool bUsedShapeMode = WndPtr->CheckWndUsedShapeMode(AlgType);

	if ( BuildWndParamList_SaveDefectImage(pGroup, Project, WndPtr) == false )
	{	return false; }

	if ( true == bUsedShapeMode )
	{
		if ( BuildWndParamList_BoxShape(pGroup, Project, WndPtr) == false )	
		{	return false;	}
	}

	if ( BuildWndParamList_FollowMode(pGroup, Project, WndPtr) == false )
	{	return false;	}	

	if ( BuildWndParamList_ConstrainMode(pGroup, Project, WndPtr) == false )
	{	return false;	}	

	if ( BuildWndParamList_SyncMoveMode(pGroup, Project, WndPtr) == false )
	{	return false;	}	

	if ( BuildWndParamList_DefectGroupID(pGroup, Project, WndPtr) == false )
	{	return false; }
	
	if ( BuildWndParamList_ClassID(pGroup, Project, WndPtr) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::BuildWndParamList_AdvancePatMatch(CJETPropertyGridProperty *pGroup, CAOIProject *Project, CAOIWnd *WndPtr)
{
	if ( NULL == WndPtr ) { return false; }
	if ( NULL == Project ) { return false; }
	if ( NULL == pGroup ) { return false; }	

	CString     strCaption, strValue, strDescr;	
	CJETPropertyGridProperty* pParamItem = NULL;
	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();		
	const float  fAngleExpand = AlgParam.GetAlgPatternAngleExpand();
	const double fScaleExpand = AlgParam.GetAlgPatternScaleExpand();
	const bool   bScaleIsotropic = AlgParam.GetAlgPatternScaleIsotropic();	
	const int    nMinReduceArea = AlgParam.GetAlgPatternMinReducedArea();;
	const int    nFinalReduction = AlgParam.GetAlgPatternFinalReduction();
	const bool   bAdvancedLearning = AlgParam.GetAlgPatternAdvancedLearning();

	strCaption = _T("Angle Expand");	
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%.0f"), fAngleExpand);	
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL != pParamItem )
	{
		pParamItem->SetData((DWORD_PTR)WndPtr);
		pParamItem->SetID(WND_ALG_PROPERTY_PATTERN_MATCH_ANGLE_EXPAND);
		pGroup->AddSubItem(pParamItem);
	}

	strCaption = _T("Scale Expand");	
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%.0f"), fScaleExpand);	
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL != pParamItem )
	{	
		pParamItem->SetData((DWORD_PTR)WndPtr);
		pParamItem->SetReading(_T("%"));
		pParamItem->SetID(WND_ALG_PROPERTY_PATTERN_MATCH_SCALE_EXPAND);				
		pGroup->AddSubItem(pParamItem);
	}

	strCaption = _T("Scale Isotropic");	
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue = _T("");
	//pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	pParamItem = new CJETPropertyGridProperty(strCaption, (_variant_t)bScaleIsotropic, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL != pParamItem )
	{			
		pParamItem->SetData((DWORD_PTR)WndPtr);		
		//pParamItem->SetCheckValue(bScaleIsotropic);
		//pParamItem->AllowEdit(FALSE);
		pParamItem->SetID(WND_ALG_PROPERTY_PATTERN_MATCH_SCALE_ISOTROPIC);				
		pGroup->AddSubItem(pParamItem);
	}

	strCaption = _T("Min Area");	
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	pParamItem = CreateGridPropertyMatchMinReduceAreaList(strCaption, nMinReduceArea, strDescr);
	if ( NULL != pParamItem )
	{
		pParamItem->SetData((DWORD_PTR)WndPtr);
		pParamItem->SetID(WND_ALG_PROPERTY_PATTERN_MATCH_MIN_REDUCE_AREA);
		pGroup->AddSubItem(pParamItem);
	}
	/*
	strCaption = _T("Advanced Learning");	
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	//pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	pParamItem = new CJETPropertyGridProperty(strCaption, (_variant_t)bAdvancedLearning, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL != pParamItem )
	{			
		pParamItem->SetData((DWORD_PTR)WndPtr);		
		//pParamItem->SetCheckValue(bScaleIsotropic);
		//pParamItem->AllowEdit(FALSE);
		pParamItem->SetID(WND_ALG_PROPERTY_PATTERN_MATCH_ADVANCED_LEARNING);				
		pGroup->AddSubItem(pParamItem);
	}
	*/
	
	strCaption = _T("Final Reduction");	
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	pParamItem = CreateGridPropertyMatchFinalReductionList(strCaption, nFinalReduction, strDescr);
	if ( NULL != pParamItem )
	{
		pParamItem->SetData((DWORD_PTR)WndPtr);
		pParamItem->SetID(WND_ALG_PROPERTY_PATTERN_MATCH_FINAL_REDUCTION);
		pGroup->AddSubItem(pParamItem);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::BuildWndParamList_WndRoi(CJETPropertyGridCtrl &GridCtrl, CAOIProject *Project, CAOIWnd *WndPtr)
{
	if ( NULL == WndPtr ) { return false; }
	if ( NULL == Project ) { return false; }
	//if ( NULL == pCtrl ) { return false; }	
	
	const BOOL bRedraw = FALSE;
	const BOOL bAdjustLayou = FALSE;
	CString strUnit, strReading;
	CString strCaption, strValue, strDescr;	
	CJETPropertyGridProperty* pParentItem = NULL;
	CJETPropertyGridProperty* pParamItem = NULL;
	CJETPropertyGridProperty* pGroupBasic = NULL;	
	
	strCaption = _T("Wnd Roi");	
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	pGroupBasic = new CJETPropertyGridProperty(strCaption);
	if ( NULL == pGroupBasic ) { return false; }	
	pGroupBasic->SetID(WND_ALG_PROPERTY_WND_ROI_BEGIN);
	pGroupBasic->SetData((DWORD_PTR)WndPtr);
	
	strCaption = _T("Add");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue = _T("Exec");
	strValue = LoadMultiLanguageString(strValue, strValue);	
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }
	pParamItem->SetID(WND_ALG_PROPERTY_WND_ROI_ADD);	
	pParamItem->SetHasUserBtn();
	pGroupBasic->AddSubItem(pParamItem);

	strCaption = _T("Delete");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue = _T("Exec");
	strValue = LoadMultiLanguageString(strValue, strValue);	
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }
	pParamItem->SetID(WND_ALG_PROPERTY_WND_ROI_DELETE);	
	pParamItem->SetHasUserBtn();
	pGroupBasic->AddSubItem(pParamItem);

	strCaption = _T("Clear");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue = _T("Exec");
	strValue = LoadMultiLanguageString(strValue, strValue);	
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }
	pParamItem->SetID(WND_ALG_PROPERTY_WND_ROI_CLEAR);	
	pParamItem->SetHasUserBtn();
	pGroupBasic->AddSubItem(pParamItem);

	strCaption = _T("Auto-Add");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue = _T("Exec");
	strValue = LoadMultiLanguageString(strValue, strValue);	
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }
	pParamItem->SetID(WND_ALG_PROPERTY_WND_ROI_ADD_AUTO);	
	pParamItem->SetHasUserBtn();
	pGroupBasic->AddSubItem(pParamItem);
	//WND_ALG_PROPERTY_WND_ROI_END,
	GridCtrl.AddProperty(pGroupBasic, bRedraw, bAdjustLayou);		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::BuildWndParamList_MaskBox(CJETPropertyGridCtrl &GridCtrl, CAOIProject *Project, CAOIWnd *WndPtr)
{
	return true;
	if ( NULL == WndPtr ) { return false; }
	if ( NULL == Project ) { return false; }
	//if ( NULL == pCtrl ) { return false; }	
	if ( WndPtr->CheckWndAlgUsedMaskWnd() == false ) { return true; }
	
	bool    bShow=true;
	double  dValue=0.0;	
	double  dShapeParam=0.0;
	double  dShapeParam2=0.0;
	const BOOL bRedraw = FALSE;
	const BOOL bAdjustLayou = FALSE;
	CString strUnit, strReading;
	CString strCaption, strValue, strDescr;	
	CAOIWndMask *WndMaskPtr = WndPtr->GetWndMaskWndActived();
	CJETPropertyGridProperty* pParentItem = NULL;
	CJETPropertyGridProperty* pParamItem = NULL;
	CJETPropertyGridProperty* pGroupBasic = NULL;	

	if ( NULL != WndMaskPtr )
	{	
		dShapeParam = WndMaskPtr->GetWndMaskShapeParam(); 
		dShapeParam2= WndMaskPtr->GetWndMaskShapeParam2(); 
	}
	
	strCaption = _T("Mask Box");	
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	pGroupBasic = new CJETPropertyGridProperty(strCaption);
	if ( NULL == pGroupBasic ) { return false; }	
	pGroupBasic->SetID(WND_ALG_PROPERTY_WND_MASK_BOX_BEGIN);
	pGroupBasic->SetData((DWORD_PTR)WndPtr);
	
	strCaption = _T("Add");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue = _T("Exec");
	strValue = LoadMultiLanguageString(strValue, strValue);	
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }
	pParamItem->SetID(WND_ALG_PROPERTY_WND_MASK_BOX_ADD);	
	pParamItem->SetHasUserBtn();
	pGroupBasic->AddSubItem(pParamItem);

	strCaption = _T("Rotate");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue = _T("Exec");
	strValue = LoadMultiLanguageString(strValue, strValue);	
	//pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);		
	pParamItem = CreateGridPropertyRotateAngleList(strCaption, WndPtr, strDescr);
	if ( NULL == pParamItem ) { return false; }
	pParamItem->SetID(WND_ALG_PROPERTY_WND_MASK_BOX_ROTATE);	
	//pParamItem->SetHasUserBtn();
	pGroupBasic->AddSubItem(pParamItem);		

	strCaption = _T("Shape Mode");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue = _T("Exec");
	strValue = LoadMultiLanguageString(strValue, strValue);	
	//pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	pParamItem = CreateGridPropertyWndBoxShapeModeList(strCaption, WndPtr, strDescr);
	if ( NULL == pParamItem ) { return false; }
	pParamItem->SetID(WND_ALG_PROPERTY_WND_MASK_BOX_SHAPE_MODE);	
	//pParamItem->SetHasUserBtn();
	pGroupBasic->AddSubItem(pParamItem);	
	if ( NULL == WndMaskPtr ) 
	{	bShow = false; }
	else
	{	bShow = true; }
	pParamItem->Show(bShow);

	strCaption = _T("Shape Param");	
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.2f"), dShapeParam);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }
	pParamItem->SetID(WND_ALG_PROPERTY_WND_MASK_BOX_SHAPE_PARAM_1);	
	//pParamItem->SetHasUserBtn();
	pGroupBasic->AddSubItem(pParamItem);
	if ( NULL == WndMaskPtr ) 
	{	bShow = false; }
	else
	{	bShow = true; }
	pParamItem->Show(bShow);
	
	strCaption = _T("Shape Param2");	
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.2f"), dShapeParam2);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }
	pParamItem->SetID(WND_ALG_PROPERTY_WND_MASK_BOX_SHAPE_PARAM_2);	
	//pParamItem->SetHasUserBtn();
	pGroupBasic->AddSubItem(pParamItem);
	if ( NULL == WndMaskPtr ) 
	{	bShow = false; }
	else
	{	bShow = true; }
	pParamItem->Show(bShow);

	strCaption = _T("Delete");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue = _T("Exec");
	strValue = LoadMultiLanguageString(strValue, strValue);	
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }
	pParamItem->SetID(WND_ALG_PROPERTY_WND_MASK_BOX_DELETE);	
	pParamItem->SetHasUserBtn();
	pGroupBasic->AddSubItem(pParamItem);
	
	strCaption = _T("Clear");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue = _T("Exec");
	strValue = LoadMultiLanguageString(strValue, strValue);	
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }
	pParamItem->SetID(WND_ALG_PROPERTY_WND_MASK_BOX_CLEAR);	
	pParamItem->SetHasUserBtn();
	pGroupBasic->AddSubItem(pParamItem);

	//WND_ALG_PROPERTY_WND_MASK_BOX_END,
	pGroupBasic->Expand(FALSE);
	GridCtrl.AddProperty(pGroupBasic, bRedraw, bAdjustLayou);		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::BuildWndParamList_WidthRatio(CAOIProject *Project, CAOIWnd *WndPtr)
{//copy Birght Ratio
	if (NULL == WndPtr) { return false; }
	if (NULL == Project) { return false; }

	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	TALG_PARAM_IPC_PRODUCT  &ipcParam = AlgParam.GetAlgParamIPC();

	CString strCaption, strValue, strReading, strDescr;
	CJETPropertyGridProperty* pParamItem = NULL;
	CJETPropertyGridProperty* pGroupBasic = NULL;
	CJETPropertyGridProperty* pGroupAdvanced = NULL;

	const BOOL bRedraw = FALSE;
	const BOOL bAdjustLayou = FALSE;
	CJETPropertyGridCtrl &wndPropList = m_wndWndParam;
	wndPropList.SetRedraw(FALSE);

	strCaption = FormWndParamListCategoryName(WndPtr);
	pGroupBasic = new CJETPropertyGridProperty(strCaption);
	if (NULL == pGroupBasic) { return FALSE; }
	pGroupBasic->SetID(WND_ALG_PROPERTY_BASIC_BEGIN);
	pGroupBasic->SetData((DWORD_PTR)WndPtr);
	wndPropList.AddProperty(pGroupBasic, bRedraw, bAdjustLayou);
	if (BuildWndParamList_WndDefectID(pGroupBasic, Project, WndPtr) == false)
	{
		return false;
	}
	if (BuildWndParamList_AlgFrameIndex(pGroupBasic, Project, WndPtr) == false)
	{
		return false;
	}

	//增加參數
	int continuous = ipcParam.ipcContinuousPixel;
	int gap = ipcParam.ipcGapPixel;
	int minPixel = 0;
	int widthRatio = 0;

	//minPixel = blobParam.bcCountLSL;
	//if (minPixel < 1) 
	//{
	//	minPixel = 1;
	//	blobParam.bcCountLSL = 1;
	//}

	strCaption = _T("Gap");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%d"), gap);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);
	if (NULL == pParamItem) { return FALSE; }
	pParamItem->SetID(WND_ALG_PROPERTY_IPC_WIDTH_GAP);
	//pParamItem->SetReading(strReading);
	//pParamItem->SetHasUserBtn();
	pGroupBasic->AddSubItem(pParamItem);

	strCaption = _T("Continuous");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%d"), continuous);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);
	if (NULL == pParamItem) { return FALSE; }
	pParamItem->SetID(WND_ALG_PROPERTY_IPC_CONTINUOUS_SET);
	//pParamItem->SetReading(strReading);
	//pParamItem->SetHasUserBtn();
	pGroupBasic->AddSubItem(pParamItem);

	//strCaption = _T("Min Count");
	//strCaption = LoadMultiLanguageString(strCaption, strCaption);
	//strValue.Format(_T("%d"), minPixel);
	//pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);
	//if (NULL == pParamItem) { return FALSE; }
	//pParamItem->SetID(WND_ALG_PROPERTY_BLOB_WIDTH_SIZE);
	//pParamItem->SetReading(strReading);
	//pParamItem->SetHasUserBtn();
	//pGroupBasic->AddSubItem(pParamItem);

	widthRatio = ipcParam.ipcWidthRatio;
	if (widthRatio < 1) { widthRatio = 1; }
	if (widthRatio > 100) { widthRatio = 100; }
	strCaption = _T("Width Ratio");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%d"), widthRatio);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);
	if (NULL == pParamItem) { return FALSE; }
	pParamItem->SetID(WND_ALG_PROPERTY_IPC_WIDTH_RATIO);
	//strReading.Format(_T("%d"), blobParam.bcFillRatioMax);
	//pParamItem->SetReading(strReading);
	//pParamItem->SetHasUserBtn();
	pGroupBasic->AddSubItem(pParamItem);

	//結果文字顯示
	if (BuildWndParamList_ResultText(pGroupBasic, Project, WndPtr) == false)
	{
		return false;
	}

	if (BuildWndParamList_ModelMaskFlag(wndPropList, Project, WndPtr) == false)
	{
		return false;
	}

	if (BuildWndParamList_GroupCompare(wndPropList, Project, WndPtr) == FALSE)
	{
		return false;
	}

	//基準數值
	//if (BuildWndParamList_BaseValue(wndPropList, Project, WndPtr) == false)
	//{
	//	return FALSE;
	//}

	//邏輯設定
	if (BuildWndParamList_LogicParam(wndPropList, Project, WndPtr) == false)
	{
		return false;
	}

	//遮罩框
	//if (BuildWndParamList_MaskBox(wndPropList, Project, WndPtr) == FALSE)
	//{
	//	return false;
	//}

	//進階設定
	strCaption = _T("Advance");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	pGroupAdvanced = new CJETPropertyGridProperty(strCaption);
	if (NULL == pGroupAdvanced) { return FALSE; }
	pGroupAdvanced->SetID(WND_ALG_PROPERTY_ADVANCE_BEGIN);
	pGroupAdvanced->SetData((DWORD_PTR)WndPtr);
	wndPropList.AddProperty(pGroupAdvanced, bRedraw, bAdjustLayou);

	if (BuildWndParamList_AdvanceGeneral(pGroupAdvanced, Project, WndPtr) == FALSE)
	{
		return false;
	}
	pGroupAdvanced->Expand(FALSE);

	if (FALSE == bAdjustLayou)
	{
		wndPropList.AdjustLayout();
	}
	wndPropList.SetRedraw(TRUE);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::BuildWndParamList_ROICompare(CJETPropertyGridCtrl &GridCtrl, CAOIProject *Project, CAOIWnd *WndPtr)
{
	if (NULL == WndPtr) { return false; }
	if (NULL == Project) { return false; }
	//if ( NULL == pGroup ) { return false; }
	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	const TALG_PARAM_IPC_PRODUCT  &ipcParam = AlgParam.GetAlgParamIPC();

	CString    str;
	const BOOL bRedraw = FALSE;
	const BOOL bAdjustLayou = FALSE;
	CString	strCaption, strValue, strReading, strDescr;
	//CString    strID = AOIDataDefine.GetIDText();
	CJETPropertyGridProperty *pGroup = NULL;
	CJETPropertyGridProperty *pParamItem = NULL;
	//const int LogicGroupID = WndPtr->GetWndLogicGroupID();

	strCaption = _T("Rect Comparison");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	pGroup = new CJETPropertyGridProperty(strCaption);
	if (NULL == pGroup) { return false; }
	pGroup->SetID(WND_ALG_PROPERTY_IPC_NODE);
	pGroup->SetData((DWORD_PTR)WndPtr);

	int yValue = 0;
	int xValue = 0;

	strCaption = _T("Enable");
	strValue = _T("");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);
	if (NULL == pParamItem) { return FALSE; }
	pParamItem->SetID(WND_ALG_PROPERTY_IPC_ENABLED);
	pParamItem->SetCheckValue(ipcParam.ipcEnabled);
	pParamItem->AllowEdit(FALSE);
	pGroup->AddSubItem(pParamItem);

	xValue = ipcParam.ipcX;
	strCaption = _T("Horizontal");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%d"), xValue);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);
	if (NULL == pParamItem) { return FALSE; }
	pParamItem->SetID(WND_ALG_PROPERTY_IPC_XVALUE);
	strReading.Format(_T("%d"), ipcParam.ipcX);
	pGroup->AddSubItem(pParamItem);

	yValue = ipcParam.ipcY;
	strCaption = _T("Vertical");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%d"), yValue);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);
	if (NULL == pParamItem) { return FALSE; }
	pParamItem->SetID(WND_ALG_PROPERTY_IPC_YVALUE);
	strReading.Format(_T("%d"), ipcParam.ipcY);
	pGroup->AddSubItem(pParamItem);

	//結果文字顯示
	if (BuildWndParamList_ResultText(pGroup, Project, WndPtr) == false)
	{
		return false;
	}

	pGroup->Expand(TRUE);
	GridCtrl.AddProperty(pGroup, bRedraw, bAdjustLayou);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::BuildWndParamList_ResinHight(CAOIProject *Project, CAOIWnd *WndPtr)
{
	if (NULL == WndPtr) { return false; }
	if (NULL == Project) { return false; }

	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	//CAOIModel *model = WndPtr->GetWndModelPtr();
	//model->get

	TALG_PARAM_RESIN_HEIGHT &hightDetectParm = AlgParam.GetAlgParamResinHeight();

	CString strCaption, strValue, strReading, strDescr;
	CJETPropertyGridProperty* pParamItem = NULL;
	CJETPropertyGridProperty* pGroupBasic = NULL;
	CJETPropertyGridProperty* pGroupAdvanced = NULL;

	const BOOL bRedraw = FALSE;
	const BOOL bAdjustLayou = FALSE;
	CJETPropertyGridCtrl &wndPropList = m_wndWndParam;
	wndPropList.SetRedraw(FALSE);

	strCaption = _T("3D Detect");
	//strCaption = LoadMultiLanguageString(strCaption, strCaption);
	pGroupBasic = new CJETPropertyGridProperty(strCaption);
	if (NULL == pGroupBasic) { return FALSE; }
	pGroupBasic->SetID(WND_ALG_PROPERTY_RESIN_BEGIN);
	pGroupBasic->SetData((DWORD_PTR)WndPtr);
	wndPropList.AddProperty(pGroupBasic, bRedraw, bAdjustLayou);



	//增加參數
	if (WndPtr->GetWndDefectID() == WND_DEFECT_PART_ALIGN)
	{
		strCaption = _T("Enable");
		strValue = _T("");
		strCaption = LoadMultiLanguageString(strCaption, strCaption);
		pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);
		if (NULL == pParamItem) { return FALSE; }
		pParamItem->SetID(WND_ALG_PROPERTY_RESIN_ENABLED);
		pParamItem->SetCheckValue(hightDetectParm.enabled);
		pParamItem->AllowEdit(FALSE);
		if ( FN_DISABLE == AOIDataCollect.GetSystemParameter().m_ResinHeightAlignEnabled )
		{	
			pParamItem->SetValueTextColor(m_clrException);
			pParamItem->SetValue(AOIDataDefine.GetDisableText());
		}
		pGroupBasic->AddSubItem(pParamItem);

		strCaption = _T("NG check");
		strCaption = LoadMultiLanguageString(strCaption, strCaption);
		pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);
		if (NULL == pParamItem) { return FALSE; }
		pParamItem->SetID(WND_ALG_PROPERTY_RESIN_NG_CHECK);
		pParamItem->SetCheckValue(hightDetectParm.enableDoubleCheck);
		pParamItem->AllowEdit(FALSE);
		pGroupBasic->AddSubItem(pParamItem);

		strCaption = _T("Type");
		strCaption = LoadMultiLanguageString(strCaption, strCaption);
		strValue = AOIDataDefine.GetAlgHeightDetectionTypeText(hightDetectParm.nInspectionType);
		pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);
		if (NULL == pParamItem) { return false; }
		strValue = AOIDataDefine.GetAlgHeightDetectionTypeText(1);	pParamItem->AddOption(strValue);
		strValue = AOIDataDefine.GetAlgHeightDetectionTypeText(2);	pParamItem->AddOption(strValue);
		pParamItem->SetID(WND_ALG_PROPERTY_RESIN_TYPE);
		pGroupBasic->AddSubItem(pParamItem);

		//pParamItem->AllowEdit(FALSE);
		//pGroupBasic->AddSubItem(pParamItem);

		strCaption = _T("Caculate Mode");
		strCaption = LoadMultiLanguageString(strCaption, strCaption);
		strValue = AOIDataDefine.GetAlgHeightDetectionOutputTypeText(hightDetectParm.nOutputType);
		pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);
		if (NULL == pParamItem) { return false; }
		pParamItem->AddOption(strValue);
		strValue = AOIDataDefine.GetAlgHeightDetectionOutputTypeText(1); pParamItem->AddOption(strValue);
		strValue = AOIDataDefine.GetAlgHeightDetectionOutputTypeText(2); pParamItem->AddOption(strValue);
		pParamItem->SetID(WND_ALG_PROPERTY_RESIN_OUTPUT_TYPE);
		pGroupBasic->AddSubItem(pParamItem);

		strCaption = _T("Direction");
		strCaption = LoadMultiLanguageString(strCaption, strCaption);
		strValue = AOIDataDefine.GetAlgHeightDetectionDirectionText(hightDetectParm.nDirection);
		pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);
		if (NULL == pParamItem) { return false; }
		pParamItem->AddOption(strValue);
		strValue = AOIDataDefine.GetAlgHeightDetectionDirectionText(1);	pParamItem->AddOption(strValue);
		strValue = AOIDataDefine.GetAlgHeightDetectionDirectionText(2);	pParamItem->AddOption(strValue);
		strValue = AOIDataDefine.GetAlgHeightDetectionDirectionText(3);	pParamItem->AddOption(strValue);
		strValue = AOIDataDefine.GetAlgHeightDetectionDirectionText(4);	pParamItem->AddOption(strValue);
		strValue = AOIDataDefine.GetAlgHeightDetectionDirectionText(5);	pParamItem->AddOption(strValue);
		pParamItem->SetID(WND_ALG_PROPERTY_RESIN_DIRECTION);
		pGroupBasic->AddSubItem(pParamItem);

		strCaption = _T("Measure Range");
		strCaption = LoadMultiLanguageString(strCaption, strCaption);
		strValue.Format(_T("%d"), hightDetectParm.nMeasureRange);
		pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);
		if (NULL == pParamItem) { return FALSE; }
		pParamItem->SetID(WND_ALG_PROPERTY_RESIN_RANGE);
		pGroupBasic->AddSubItem(pParamItem);

		strCaption = _T("Step Z(um)");
		strCaption = LoadMultiLanguageString(strCaption, strCaption);
		strValue.Format(_T("%d"), hightDetectParm.nStepZ);
		pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);
		if (NULL == pParamItem) { return FALSE; }
		pParamItem->SetID(WND_ALG_PROPERTY_RESIN_STEP_Z);
		pGroupBasic->AddSubItem(pParamItem);

		strCaption = _T("Shift");
		strCaption = LoadMultiLanguageString(strCaption, strCaption);
		strValue.Format(_T("%d"), hightDetectParm.nShift_Tin);
		pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);
		if (NULL == pParamItem) { return FALSE; }
		pParamItem->SetID(WND_ALG_PROPERTY_RESIN_SHIFT);
		pGroupBasic->AddSubItem(pParamItem);

		strCaption = _T("ThresholdZ Width");
		strCaption = LoadMultiLanguageString(strCaption, strCaption);
		strValue.Format(_T("%d"), hightDetectParm.nThresholdZ_Width);
		pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);
		if (NULL == pParamItem) { return FALSE; }
		pParamItem->SetID(WND_ALG_PROPERTY_RESIN_THRESHOLD_WIDTH);
		pGroupBasic->AddSubItem(pParamItem);

		strCaption = _T("ThresholdZ Height");
		strCaption = LoadMultiLanguageString(strCaption, strCaption);
		strValue.Format(_T("%d"), hightDetectParm.nThresholdZ_Height);
		pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);
		if (NULL == pParamItem) { return FALSE; }
		pParamItem->SetID(WND_ALG_PROPERTY_RESIN_THRESHOLD_HEIGHT);
		pGroupBasic->AddSubItem(pParamItem);

		if ( false ==hightDetectParm.enabled )
		{	pGroupBasic->Expand(FALSE);	}		
	}

	if (WndPtr->GetWndDefectID() != WND_DEFECT_PART_ALIGN)
	{
		strCaption = _T("Height Measure Mode");
		strCaption = LoadMultiLanguageString(strCaption, strCaption);
		strValue = AOIDataDefine.GetAlgHeightDetectionMeasureModeText(hightDetectParm.nPartHeightMeasureMode);
		pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);
		if (NULL == pParamItem) { return false; }
		pParamItem->AddOption(strValue);
		strValue = AOIDataDefine.GetAlgHeightDetectionMeasureModeText(0);	pParamItem->AddOption(strValue);
		strValue = AOIDataDefine.GetAlgHeightDetectionMeasureModeText(1);	pParamItem->AddOption(strValue);
		pParamItem->SetID(WND_ALG_PROPERTY_RESIN_HEIGHT_MEASURE_MODE);
		pGroupBasic->AddSubItem(pParamItem);


		strCaption = _T("Height Ratio");
		strCaption = LoadMultiLanguageString(strCaption, strCaption);
		strValue.Format(_T("%d"), hightDetectParm.nPartHeightThreshold);
		pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);
		if (NULL == pParamItem) { return FALSE; }
		pParamItem->SetID(WND_ALG_PROPERTY_RESIN_HEIGHT_VALUE);
		pGroupBasic->AddSubItem(pParamItem);
	}

	//結果文字顯示
	if (BuildWndParamList_ResultText(pGroupBasic, Project, WndPtr) == false)
	{
		return false;
	}

	if (FALSE == bAdjustLayou)
	{
		wndPropList.AdjustLayout();
	}
	wndPropList.SetRedraw(TRUE);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::BuildWndParamList_WireWidth(CAOIProject *Project, CAOIWnd *WndPtr)
{
	if (NULL == WndPtr) { return false; }
	if (NULL == Project) { return false; }

	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();

	TALG_PARAM_WIRE_WIDTH &wireWidthParm = AlgParam.GetAlgParamWireWidth();

	CString strCaption, strValue, strReading, strDescr;
	CJETPropertyGridProperty* pParamItem = NULL;
	CJETPropertyGridProperty* pGroupBasic = NULL;
	CJETPropertyGridProperty* pGroupAdvanced = NULL;

	const BOOL bRedraw = FALSE;
	const BOOL bAdjustLayou = FALSE;
	CJETPropertyGridCtrl &wndPropList = m_wndWndParam;
	wndPropList.SetRedraw(FALSE);

	strCaption = _T("Wire Detect");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	pGroupBasic = new CJETPropertyGridProperty(strCaption);
	if (NULL == pGroupBasic) { return FALSE; }
	pGroupBasic->SetID(WND_ALG_PROPERTY_WIRE_WIDTH_BEGIN);
	pGroupBasic->SetData((DWORD_PTR)WndPtr);
	wndPropList.AddProperty(pGroupBasic, bRedraw, bAdjustLayou);

	if (BuildWndParamList_AlgFrameIndex(pGroupBasic, Project, WndPtr) == false)
	{
		return false;
	}
	//增加參數
	strCaption = _T("Filter Enable");
	strValue = _T("");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);
	if (NULL == pParamItem) { return FALSE; }
	pParamItem->SetID(WND_ALG_PROPERTY_WIRE_WIDTH_FILTER_ENABLED);
	pParamItem->SetCheckValue(wireWidthParm.bFilter);
	pParamItem->AllowEdit(FALSE);
	pGroupBasic->AddSubItem(pParamItem);

	strCaption = _T("Filter Size");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%d"), wireWidthParm.nFilterSize);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);
	if (NULL == pParamItem) { return FALSE; }
	pParamItem->SetID(WND_ALG_PROPERTY_WIRE_WIDTH_FILTER_SIZE);
	pGroupBasic->AddSubItem(pParamItem);

	strCaption = _T("Low Thres");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%d"), wireWidthParm.nLowThres);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);
	if (NULL == pParamItem) { return FALSE; }
	pParamItem->SetID(WND_ALG_PROPERTY_WIRE_WIDTH_EDGE_LOW_THRESHOLD);
	pGroupBasic->AddSubItem(pParamItem);

	strCaption = _T("Hight Thres");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%d"), wireWidthParm.nHeightThres);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);
	if (NULL == pParamItem) { return FALSE; }
	pParamItem->SetID(WND_ALG_PROPERTY_WIRE_WIDTH_EDGE_HIGHT_THRESHOLD);
	pGroupBasic->AddSubItem(pParamItem);

	strCaption = _T("Width USL");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.1f"), wireWidthParm.widthUSL);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);
	if (NULL == pParamItem) { return FALSE; }
	pParamItem->SetID(WND_ALG_PROPERTY_WIRE_WIDTH_VALUE_USL);
	pGroupBasic->AddSubItem(pParamItem);

	strCaption = _T("Width LSL");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.1f"), wireWidthParm.widthLSL);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);
	if (NULL == pParamItem) { return FALSE; }
	pParamItem->SetID(WND_ALG_PROPERTY_WIRE_WIDTH_VALUE_LSL);
	pGroupBasic->AddSubItem(pParamItem);

	//結果文字顯示
	if (BuildWndParamList_ResultText(pGroupBasic, Project, WndPtr) == false)
	{
		return false;
	}

	if (FALSE == bAdjustLayou)
	{
		wndPropList.AdjustLayout();
	}
	wndPropList.SetRedraw(TRUE);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::BuildWndParamList_PixelCompare(CAOIProject *Project, CAOIWnd *WndPtr)
{
	if ( NULL == WndPtr ) { return false; }
	if ( NULL == Project ) { return false; }	
	
	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	CString    AlgTypeText = WndPtr->GetWndAlgTypeText();
	unsigned int WndIndex = WndPtr->GetWndIndex();
	const int  WndGroupID = WndPtr->GetWndGroupID();
	const TPropGridParam &PropGridParam = GetPropGridParam();
	const double WndInspectedTime = WndPtr->GetWndInspectedTime();	
	const int    PatternPolarity = AlgParam.GetAlgPatternPolarity();	
	const TALG_PARAM_PIXEL_COMPARE &pcParam = AlgParam.GetAlgParamPixelCompare();	

	const COLORREF  clrOK = m_clrOK;
	const COLORREF  clrNG = m_clrNG;
	const COLORREF  clrUnTest = m_clrUnTest;

	bool    bIsPass = true;
	CString strCaption, strValue, strDescr;
	CString strUnit, strReading;
	CJETPropertyGridProperty* pParamItem = NULL;
	CJETPropertyGridProperty* pGroupBasic = NULL;
	CJETPropertyGridProperty* pGroupPat = NULL;	
	CJETPropertyGridProperty* pGroupCmp = NULL;
	CJETPropertyGridProperty* pGroupAdvanced = NULL;
	CJETPropertyGridProperty* pGroupResult = NULL;

	const BOOL bRedraw = FALSE;
	const BOOL bAdjustLayou = FALSE;
	CJETPropertyGridCtrl &wndPropList = m_wndWndParam;
	wndPropList.SetRedraw(FALSE);
	
	strCaption = FormWndParamListCategoryName(WndPtr);	
	pGroupBasic = new CJETPropertyGridProperty(strCaption);
	if ( NULL == pGroupBasic ) { return false; }	
	pGroupBasic->SetID(WND_ALG_PROPERTY_BASIC_BEGIN);
	pGroupBasic->SetData((DWORD_PTR)WndPtr);
	wndPropList.AddProperty(pGroupBasic, bRedraw, bAdjustLayou);
	if ( BuildWndParamList_WndDefectID(pGroupBasic, Project, WndPtr) == false )
	{	return false;	}	
	if ( BuildWndParamList_AlgFrameIndex(pGroupBasic, Project, WndPtr) == false )
	{	return false;	}

	//增加Pixel Compare 參數		
	const int nCellExtSize = pcParam.pcCellExtSize;	
	const int nCountUSL = pcParam.pcBlobCountUSL;
	const int nCountLSL = pcParam.pcBlobCountLSL;	
	const int nCountNum = pcParam.pcBlobCountNum;	

	const bool bShowX=AlgParam.CheckAlgShowOffsetByAlgType(AlgParam.GetAlgType());//true;
	const bool bShowY=AlgParam.CheckAlgShowOffsetByAlgType(AlgParam.GetAlgType());//true;
	const bool bShowA=AlgParam.CheckAlgShowSkewByAlgType(AlgParam.GetAlgType());//true;
	const bool bShowS=AlgParam.CheckAlgShowScaleByAlgType(AlgParam.GetAlgType());//false;
	if ( BuildWndParamList_MatchOffset(pGroupBasic, Project, WndPtr, bShowX, bShowY, bShowA, bShowS) == false )
	{	return false; }

	strUnit = _T("Count");
	strReading.Format(_T("%d"), nCountNum);

	strCaption = _T("Cell Ext Size");	
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%d"), nCellExtSize);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_PIXEL_COMPARE_CELL_EXT_SIZE);
	pParamItem->SetReading(AOIDataDefine.GetPixelText());		
	pGroupBasic->AddSubItem(pParamItem);

	strCaption = _T("Smooth Size");	
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%d"), pcParam.pcImgBlurSize);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_PIXEL_COMPARE_IMG_BLUR_SIZE);
	pParamItem->SetReading(AOIDataDefine.GetPixelText());		
	pParamItem->Show(FALSE, FALSE);//先不開放
	pGroupBasic->AddSubItem(pParamItem);	
	
	strCaption = _T("Size LSL");	
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%.0f"), pcParam.pcBlobMinSizeD);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_PIXEL_COMPARE_BLOB_MIN_SIZE_D);
	pParamItem->SetReading(_T("um"));		
	pGroupBasic->AddSubItem(pParamItem);
	
	strCaption = _T("Count USL");	
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%d"), nCountUSL);
	strReading.Format(_T("%d"), nCountNum);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_PIXEL_COMPARE_BLOB_COUNT_USL);
	pParamItem->SetReading(strReading);	
	bIsPass = CAlgParam::CheckOK_PixelCompareUSL(pcParam);
	if ( false == bIsPass )
	{	pParamItem->SetReadingTextColor(clrNG); }
	else
	{	pParamItem->SetReadingTextColor(clrOK); }	
	pGroupBasic->AddSubItem(pParamItem);

	//結果文字顯示
	if ( BuildWndParamList_ResultText(pGroupBasic, Project, WndPtr) == false )
	{	return false; }

#pragma region Pattern_Image
	//Pattern Image Score
	strCaption = _T("Pattern Image");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	pGroupPat = new CJETPropertyGridProperty(strCaption);
	if ( NULL == pGroupPat ) 
	{	return false;	}	
	pGroupPat->SetID(WND_ALG_PROPERTY_PIXEL_COMPARE_PAT_SCOPE);	
	pGroupPat->SetData((DWORD_PTR)WndPtr);	
	wndPropList.AddProperty(pGroupPat, bRedraw, bAdjustLayou);

	strCaption = _T("Light Level");	
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%d"), pcParam.pcPatLightLevel);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_PIXEL_COMPARE_PAT_LIGHT_LEVEL);
	pParamItem->SetReading(AOIDataDefine.GetGrayText());		
	pGroupPat->AddSubItem(pParamItem);

	strCaption = _T("Dark Level");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%d"), pcParam.pcPatDarkLevel);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_PIXEL_COMPARE_PAT_DARK_LEVEL);
	pParamItem->SetReading(AOIDataDefine.GetGrayText());		
	pGroupPat->AddSubItem(pParamItem);

	strCaption = _T("Erode Size");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%d"), pcParam.pcPatErodeSize);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_PIXEL_COMPARE_PAT_ERODE_SIZE);
	pParamItem->SetReading(AOIDataDefine.GetPixelText());		
	pGroupPat->AddSubItem(pParamItem);
	
	const bool bPatPureColor = (bool)(pcParam.pcPatPureColor);
	strCaption = _T("Pure Color");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	pParamItem = new CJETPropertyGridProperty(strCaption, (_variant_t)(bPatPureColor), strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_PIXEL_COMPARE_PAT_PURE_COLOR);	
	pGroupPat->AddSubItem(pParamItem);	

	pGroupPat->Expand(PropGridParam.bPixelCompare_PatScopeExpand);
#pragma endregion Pattern_Image

#pragma region Compare_Image
	//Compare Image Score
	strCaption = _T("Compare Image");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	pGroupCmp = new CJETPropertyGridProperty(strCaption);
	if ( NULL == pGroupPat ) 
	{	return false;	}	
	pGroupCmp->SetID(WND_ALG_PROPERTY_PIXEL_COMPARE_CMP_SCOPE);	
	pGroupCmp->SetData((DWORD_PTR)WndPtr);	
	wndPropList.AddProperty(pGroupCmp, bRedraw, bAdjustLayou);

	strCaption = _T("Light Gain");	
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%.2f"), pcParam.pcCmpLightGain);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_PIXEL_COMPARE_CMP_LIGHT_GAIN);
	pParamItem->SetReading(AOIDataDefine.GetRatioText());		
	pGroupCmp->AddSubItem(pParamItem);

	strCaption = _T("Dark Gain");	
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%.2f"), pcParam.pcCmpDarkGain);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_PIXEL_COMPARE_CMP_DARK_GAIN);
	pParamItem->SetReading(AOIDataDefine.GetRatioText());		
	pGroupCmp->AddSubItem(pParamItem);

	strCaption = _T("Tolerance");	
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%d"), pcParam.pcCmpTolerance);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_PIXEL_COMPARE_CMP_TOLERANCE);
	pParamItem->SetReading(AOIDataDefine.GetGrayText());		
	pGroupCmp->AddSubItem(pParamItem);

	strCaption = _T("Smooth Size");	
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%d"), pcParam.pcCmpGaussianSize);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_PIXEL_COMPARE_CMP_GAUSSIAN_SIZE);
	pParamItem->SetReading(AOIDataDefine.GetPixelText());		
	pGroupCmp->AddSubItem(pParamItem);

	strCaption = _T("Open Size");	
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%d"), pcParam.pcCmpOpenSize);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_PIXEL_COMPARE_CMP_OPEN_SIZE);
	pParamItem->SetReading(AOIDataDefine.GetPixelText());		
	pGroupCmp->AddSubItem(pParamItem);

	strCaption = _T("Close Size");	
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%d"), pcParam.pcCmpCloseSize);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_PIXEL_COMPARE_CMP_CLOSE_SIZE);
	pParamItem->SetReading(AOIDataDefine.GetPixelText());		
	pGroupCmp->AddSubItem(pParamItem);

	pGroupCmp->Expand(PropGridParam.bPixelCompare_CmpScopeExpand);
#pragma endregion Compare_Image

	if ( BuildWndParamList_ExtendRange(wndPropList, Project, WndPtr) == false )
	{	return false;	}

	//子框參數
	if ( BuildWndParamList_WndRoi(wndPropList, Project, WndPtr) == false )
	{	return false;	}

	//邏輯參數
	if ( BuildWndParamList_LogicParam(wndPropList, Project, WndPtr) == false )
	{	return false; }	

	//進階設定
	strCaption = _T("Advance");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	pGroupAdvanced = new CJETPropertyGridProperty(strCaption);
	if ( NULL == pGroupAdvanced ) { return false; }	
	pGroupAdvanced->SetID(WND_ALG_PROPERTY_ADVANCE_BEGIN);
	pGroupAdvanced->SetData((DWORD_PTR)WndPtr);	
	wndPropList.AddProperty(pGroupAdvanced, bRedraw, bAdjustLayou);
	if ( BuildWndParamList_AdvanceGeneral(pGroupAdvanced, Project, WndPtr) == false )	
	{	return false;	}

	if ( BuildWndParamList_AdvancePatMatch(pGroupAdvanced, Project, WndPtr) == false )	
	{	return false;	}
	pGroupAdvanced->Expand(FALSE);

	if ( FALSE == bAdjustLayou )
	{	wndPropList.AdjustLayout(); }
	wndPropList.SetRedraw(TRUE);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::ClearWndParamList()
{
	m_wndWndParam.RemoveAll();	
	//m_wndWndParam.AdjustLayout();
	m_wndWndParam.Invalidate();
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditWndView::OnClassCopyBtn()
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return ; }
	CAOIModel   *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return ; }
	CAOIRgn     *AttachedObj = ModelPtr->GetModelAttachedPtr();
	if ( NULL == AttachedObj ) { return ; }	

	CString    str, str1;	
	CComboBox &Combox = m_wndClassCombo;
	const int  ActClassID = (int)(JetAPI::GetComboxCurSelData(Combox));
	str = _T("Do you want to copy the wnds with class ID");
	str = LoadMultiLanguageString(str, str);
	str1.Format(_T("%s (ID:%d) ?"), str, ActClassID);
	if ( JetAPI::ShowMessageBox(str1, MB_YESNO) != IDYES )
	{	return; }
	ModelPtr->CloneModelWndWithClassID(ActClassID);
	CAOIWnd *WndPtr = ModelPtr->GetModelWndActived();
	if ( NULL != WndPtr )
	{
		int WndClassID = WndPtr->GetWndClassID();
		ModelPtr->SetModelActClassID(WndClassID);
	}
	UpdateClassComboxID();
	return;
}
//-------------------------------------------------------------------------------------//
void CEditWndView::OnClassClearBtn()
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return ; }
	CAOIModel   *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return ; }
	CAOIRgn     *AttachedObj = ModelPtr->GetModelAttachedPtr();
	if ( NULL == AttachedObj ) { return ; }	

	CString    str, str1;	
	CComboBox &Combox = m_wndClassCombo;
	const int  ActClassID = (int)(JetAPI::GetComboxCurSelData(Combox));
	str = _T("Do you want to delete the wnds with class ID");	
	str = LoadMultiLanguageString(str, str);
	str1.Format(_T("%s (ID:%d) ?"), str, ActClassID);
	if ( JetAPI::ShowMessageBox(str1, MB_YESNO) != IDYES )
	{	return; }
	ModelPtr->DeleteModelWndWithClassID(ActClassID);
	UpdateClassComboxID();
	return;
}
//-------------------------------------------------------------------------------------//
void CEditWndView::OnSelchangeClassCombo() 
{
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return ; }
	CAOIModel   *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return ; }
	CAOIRgn     *AttachedObj = ModelPtr->GetModelAttachedPtr();
	if ( NULL == AttachedObj ) { return ; }	

	CComboBox &Combox = m_wndClassCombo;
	const int  ActClassID = (int)(JetAPI::GetComboxCurSelData(Combox));	
	ModelPtr->SetModelActClassID(ActClassID);
	UpdateClassComboxID();
	return;
}
//-------------------------------------------------------------------------------------//
LRESULT CEditWndView::OnPropertyLClicked(WPARAM wParam, LPARAM lParam)
{
	CJETPropertyGridProperty *pProp = (CJETPropertyGridProperty*)lParam;
	if ( NULL == pProp ) { return 0; }
	if ( pProp->IsKindOf(RUNTIME_CLASS(CJETPropertyGridProperty))==FALSE )
	{	return 0; }

	if ( ID_WND_GROUP_LIST == wParam )
	{	ExecWndGroupListLClicked(pProp);	}
	else if ( ID_WND_OBJ_LIST == wParam )
	{	ExecWndObjListLClicked(pProp); }
	else if ( ID_WND_PARAM_LIST == wParam )
	{	ExecWndParamListLClicked(pProp);	}
	return 0;
}
//-------------------------------------------------------------------------------------//
LRESULT CEditWndView::OnPropertyRClicked(WPARAM wParam, LPARAM lParam)
{
	CJETPropertyGridProperty *pProp = (CJETPropertyGridProperty*)lParam;
	if ( NULL == pProp ) { return 0; }
	if ( pProp->IsKindOf(RUNTIME_CLASS(CJETPropertyGridProperty))==FALSE )
	{	return 0; }

	CString strName = pProp->GetName();
	CString strValue = pProp->GetValue();
	CString strDescr = pProp->GetDescription();

	if ( ID_WND_GROUP_LIST == wParam )
	{	ExecWndGroupListRClicked(pProp);	}
	else if ( ID_WND_OBJ_LIST == wParam )
	{	ExecWndObjListRClicked(pProp); }
	else if ( ID_WND_PARAM_LIST == wParam )
	{	ExecWndParamListRClicked(pProp);	}
	return 0;
}
//-------------------------------------------------------------------------------------//
LRESULT CEditWndView::OnPropertyLDbClick(WPARAM wParam, LPARAM lParam)
{
	CJETPropertyGridProperty *pProp = (CJETPropertyGridProperty*)lParam;
	if ( NULL == pProp ) { return 0; }
	if ( pProp->IsKindOf(RUNTIME_CLASS(CJETPropertyGridProperty))==FALSE )
	{	return 0; }

	if ( ID_WND_GROUP_LIST == wParam )
	{	ExecWndGroupListLDbClick(pProp);	}
	else if ( ID_WND_OBJ_LIST == wParam )
	{	ExecWndObjListLDbClick(pProp); }
	else if ( ID_WND_PARAM_LIST == wParam )
	{	ExecWndParamListLDbClick(pProp);	}	
	return 0;
}
//-------------------------------------------------------------------------------------//
LRESULT CEditWndView::OnPropertyRDbClick(WPARAM wParam, LPARAM lParam)
{
	CJETPropertyGridProperty *pProp = (CJETPropertyGridProperty*)lParam;
	if ( NULL == pProp ) { return 0; }
	if ( pProp->IsKindOf(RUNTIME_CLASS(CJETPropertyGridProperty))==FALSE )
	{	return 0; }

	if ( ID_WND_GROUP_LIST == wParam )
	{	ExecWndGroupListRDbClick(pProp);	}
	else if ( ID_WND_OBJ_LIST == wParam )
	{	ExecWndObjListRDbClick(pProp); }
	else if ( ID_WND_PARAM_LIST == wParam )
	{	ExecWndParamListRDbClick(pProp);	}
	return 0;
}
//-------------------------------------------------------------------------------------//
LRESULT CEditWndView::OnPropertyChanged(WPARAM wParam, LPARAM lParam)
{
	CJETPropertyGridProperty *pProp = (CJETPropertyGridProperty*)lParam;
	if ( NULL == pProp ) { return 0; }
	if ( pProp->IsKindOf(RUNTIME_CLASS(CJETPropertyGridProperty))==FALSE )
	{	return 0; }

	if ( ID_WND_GROUP_LIST == wParam )
	{	ExecWndGroupListChanged(pProp);	}
	else if ( ID_WND_OBJ_LIST == wParam )
	{	ExecWndObjListChanged(pProp); }
	else if ( ID_WND_PARAM_LIST == wParam )
	{	ExecWndParamListChanged(pProp);	}
	return 0;
}
//-------------------------------------------------------------------------------------//
LRESULT CEditWndView::OnPropertySelChanged(WPARAM wParam, LPARAM lParam)
{
	CJETPropertyGridProperty *pProp = (CJETPropertyGridProperty*)lParam;
	if ( NULL == pProp ) { return 0; }
	if ( pProp->IsKindOf(RUNTIME_CLASS(CJETPropertyGridProperty))==FALSE )
	{	return 0; }

	if ( ID_WND_GROUP_LIST == wParam )
	{	ExecWndGroupListSelChanged(pProp);	}
	else if ( ID_WND_OBJ_LIST == wParam )
	{	ExecWndObjListSelChanged(pProp); }
	else if ( ID_WND_PARAM_LIST == wParam )
	{	ExecWndParamListSelChanged(pProp);	}
	return 0;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::ExecWndGroupListLClicked(CJETPropertyGridProperty *pProp)
{
	if ( NULL == pProp ) { return false; }
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return true; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }

	CAOIWnd *WndPtr = NULL;	
	DWORD_PTR Data = pProp->GetData();
	const int Level = pProp->GetHierarchyLevel();
	const int WndGroupID = (int)(pProp->GetData());
	WND_GROUP_PROPERTY_ID ParamID = (WND_GROUP_PROPERTY_ID)(pProp->GetID());
	AOIDataCollect.SetIsModelWndGroupSelChange(true);
	if ( ParamID>=WND_GROUP_PROPERTY_MODEL_PARAM && ParamID<=WND_GROUP_PROPERTY_MODEL_PARAM_END )
	{	
		ModelPtr->InvisibleModelWnd();
		ModelPtr->UnSelectModel();
		ModelPtr->SetModelBodyBoxActived(true);

		ProjectPtr->SetProjectActiveModelBox(NULL);
		ProjectPtr->SetProjectActiveModelWnd(NULL);
		ProjectPtr->SetProjectActiveModelLand(NULL); 
		ClearWndObjList();
		ClearWndParamList();

		SendMessageToMainFrameWnd(MSG_EDIT_IMAGE_VIEW_WND, WPARAM_UPDATE_IMAGE_WND_SELECTED, NULL);		
		PostMessageToMainFrameWnd(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);//
		return true;
	}

	ModelPtr->InvisibleModelWnd();	
	ModelPtr->SetModelWndVisibledByWndGroupID(WndGroupID, -1, true);	
	WndPtr = ModelPtr->GetModelWndPtrByGroupID(WndGroupID, -1, true);

	BuildWndObjList(ModelPtr, WndGroupID);
	const int WndObjCount = m_wndWndList.GetPropertyCount();
	if ( WndObjCount > 0 ) 
	{
		CMFCPropertyGridProperty *pProp = m_wndWndList.GetProperty(0);
		WndPtr = (CAOIWnd*)(pProp->GetData());
		SetActiveWndPtr(WndPtr);
		ModelPtr->UnSelectModel();
		WndPtr->SetWndSelected(true);
		UpdateWndObjListSelected();
		//BuildWndParamList();//UpdateWndObjListSelected會自動呼叫ExecWndObjListSelChanged
	}
	else
	{	ClearWndParamList();	}
	//ModelPtr->SetModelLandActived(NULL);
	ModelPtr->SetModelWndActived(WndPtr);
	if ( NULL != WndPtr )
	{
		CAlgParam       &AlgParam = WndPtr->GetWndAlgParam();
		CAlgBinaryParam *BinParamPtr = AlgParam.GetAlgImageBinParamPtr();
		const unsigned int FrameUniqueID = BinParamPtr->GetBinaryFrameUniqueID();
		const unsigned int FrameIndex = ProjectPtr->GetProjectFrameIndexByUniqueID(FrameUniqueID);
		if ( ProjectPtr->SetProjectMapIndex(FrameIndex) == true )		
		{	PostMessageToMainFrameWnd(MSG_EDIT_MAIN_VIEW_WND, WPARAM_SWITCH_FRAME_IMAGE, NULL);	}
	}	
	else
	{
		//PostMessageToMainFrameWnd(MSG_EDIT_PART_LIST_WND, WPARAM_UPDATE_PART_SELECTED, NULL);//	
		SendMessageToMainFrameWnd(MSG_EDIT_IMAGE_VIEW_WND, WPARAM_UPDATE_IMAGE_WND_SELECTED, (LPARAM)(WndPtr));		
	}
	PostMessageToMainFrameWnd(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);//
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::ExecWndGroupListRClicked(CJETPropertyGridProperty *pProp)
{	
	if ( NULL == pProp ) { return false; }

	CString strValue(pProp->GetValue());	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::ExecWndGroupListLDbClick(CJETPropertyGridProperty *pProp)
{
	if ( NULL == pProp ) { return false; }
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return true; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }

	DWORD_PTR Data = pProp->GetData();
	const int Level = pProp->GetHierarchyLevel();
	const int WndGroupID = (int)(pProp->GetData());
	WND_GROUP_PROPERTY_ID ParamID = (WND_GROUP_PROPERTY_ID)(pProp->GetID());
	if ( ParamID>=WND_GROUP_PROPERTY_MODEL_PARAM && ParamID<=WND_GROUP_PROPERTY_MODEL_PARAM_END )
	{	return true;	}		
	CMFCPropertyGridProperty *pProp2 = m_wndWndList.GetProperty(0);
	if ( NULL == pProp2 ) { return true; }
	CAOIWnd      *WndPtr = (CAOIWnd*)(pProp2->GetData());
	if ( NULL == WndPtr ) { return true; }	
	if ( ModelPtr->CheckModelWndValid(WndPtr) == false ) { return true; }
	if ( WndPtr->GetWndGroupID() != WndGroupID ) { return true; }

	double    WndPosX=0;
	double    WndPosY=0;
	double    ModelPosX=0;
	double    ModelPosY=0;
	TREGION4D WndRegion;
	TREGION4D ModelRegion;
	const int WndIndex = WndPtr->GetWndIndex();
	
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
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::ExecWndGroupListRDbClick(CJETPropertyGridProperty *pProp)
{
	if ( NULL == pProp ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::ExecWndGroupListChanged(CJETPropertyGridProperty *pProp)
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return true; }
	if ( NULL == pProp ) { return false; }
	CAOIModel   *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return true; }

	ALG_TYPE     AlgType=ALG_EMPTY;
	bool         bEnabled = false;
	bool         bChanged = false;
	bool         ModelRgnChanged = false;
	int          LandGroupID=0;
	int          WndGroupID=0;
	double       dValue=0.0, dValue2=0.0;
	CString      strValue;
	CString      strName = pProp->GetName();
	CString      strDescr = pProp->GetDescription();	
	CString      strNewValue(pProp->GetValue());
	CString      strOldValue(pProp->GetOriginalValue());
	const bool   bEnabledOld = pProp->GetOriginalCheckValue();
	DWORD_PTR    dwData = pProp->GetData();	
	WND_GROUP_PROPERTY_ID ParamID = (WND_GROUP_PROPERTY_ID)(pProp->GetID());
	CJETPropertyGridProperty *pProp2 = NULL;

	switch ( ParamID )
	{
	case WND_GROUP_PROPERTY_MODEL_PARAM:
		break;
	case WND_GROUP_PROPERTY_MODEL_EXTEND:		
		pProp2 = m_wndGroupList.FindItemByID(WND_GROUP_PROPERTY_MODEL_EXTEND_X);
		if ( NULL != pProp2 )
		{
			strValue = pProp2->GetValue();
			dValue = ::_tcstod(strValue, NULL);		
			if ( dValue < 0 )
			{	dValue = 0; }
			ModelPtr->SetModelExtendRangeX(dValue);
			strValue.Format(_T("%.0f"), dValue);		
			pProp2->SetValue(strValue);
			dValue2 = dValue;
			ModelRgnChanged = true;			
		}
		pProp2 = m_wndGroupList.FindItemByID(WND_GROUP_PROPERTY_MODEL_EXTEND_Y);
		if ( NULL != pProp2 )
		{
			if ( pProp2->IsModified() == TRUE )
			{
				strValue = pProp2->GetValue();
				dValue = ::_tcstod(strValue, NULL);		
				if ( dValue < 0 )
				{	dValue = 0; }				
			}
			else
			{	dValue = dValue2; }
			ModelPtr->SetModelExtendRangeY(dValue);
			strValue.Format(_T("%.0f"), dValue);		
			pProp2->SetValue(strValue);		
			ModelRgnChanged = true;
		}
		break;
	case WND_GROUP_PROPERTY_MODEL_EXTEND_X:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue < 0 )
		{	dValue = 0; }
		ModelPtr->SetModelExtendRangeX(dValue);
		strValue.Format(_T("%.0f"), dValue);		
		pProp->SetValue(strValue);		
		ModelRgnChanged = true;
		break;
	case WND_GROUP_PROPERTY_MODEL_EXTEND_Y:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue < 0 )
		{	dValue = 0; }
		ModelPtr->SetModelExtendRangeY(dValue);
		strValue.Format(_T("%.0f"), dValue);		
		pProp->SetValue(strValue);		
		ModelRgnChanged = true;
		break;

	case WND_GROUP_PROPERTY_BODY_GROUP:
		break;
	case WND_GROUP_PROPERTY_LAND_GROUP:
		break;
	case WND_GROUP_PROPERTY_WND_GROUP:
		if ( AOIDataCollect.OperateLevelEditFuncBypassModelWnd() == false )
		{
			pProp->SetCheckValue(pProp->GetOriginalCheckValue());
			pProp->SetValue(pProp->GetOriginalValue());//重繪(SetCheckValue)不會重繪
			return false; 
		}
		strValue = pProp->GetValue();
		bEnabled = pProp->GetCheckValue();
		WndGroupID = (int)(pProp->GetData());

		if ( bEnabledOld != bEnabled )
		{
			pProp->SetCheckValue(bEnabled);
			ModelPtr->SetModelWndEnabledByWndGroupID(WndGroupID, bEnabled);
			bChanged = true;
		}		
		AlgType = AOIDataDefine.FindAlgTypeByText(strValue);		
		if ( ALG_EMPTY != AlgType )
		{	
			ModelPtr->SetModelWndAlgTypeByWndGroupID(WndGroupID, AlgType);	
			bChanged = true;
		}		
		break;
	case WND_GROUP_PROPERTY_LOGIC_GROUP:
		break;
	}

	if ( true == ModelRgnChanged )
	{	ModelPtr->CalcModelTotalRegionAll();	}

	if ( true == bChanged )
	{
		LPCTSTR SetText=AOIDataDefine.GetSetText();		
		LogOperCtrl.SaveLogModelOperate(ModelPtr, SetText, strName, strOldValue, strNewValue);	
		BuildWndParamList();	
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::ExecWndGroupListSelChanged(CJETPropertyGridProperty *pProp)
{
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::BuildWndObjList(CAOIModel *ModelPtr, int WndGroupID)
{	
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return true; }

	ClearWndObjList();
	ClearWndParamList();
	if ( NULL == ModelPtr ) { return true; }	
	int       i=0, j=0;	
	int       WndIdx=0;
	CString   str;
	CString   str1, str2;
	CString   strLogic, strTest;
	CString   strCaption, strValue, strResult;
	COLORREF  Color;
	BOOL      WndEnabled=TRUE;
	CAOIWnd  *WndPtr = NULL;
	bool      bCheckLogicNG=false;
	double    WndInspectedTime = 0.0;
	WND_LOGIC_TYPE WndLogicType=WND_LOGIC_NONE;
	RESULT_ID TargetResultID = RESULT_ID_NONE;			
	RESULT_ID ResultID = RESULT_ID_NONE;
	RESULT_ID LogicResultID = RESULT_ID_NONE;
	WND_DEFECT_ID WndDefectID = WND_DEFECT_NONE;
	const size_t WndCount = ModelPtr->GetModelWndCount();
	CJETPropertyGridProperty* pWndItem = NULL;
	CJETPropertyGridCtrl &wndPropList = m_wndWndList;		
	
	//修正標題顯示
	WndPtr = ModelPtr->GetModelWndPtrByGroupBandID(WndGroupID, -1, false);
	if ( NULL != WndPtr )
	{
		str2 = _T("Result");
		str2 = LoadMultiLanguageString(str2, str2);	
		WndDefectID = WndPtr->GetWndDefectID();
		str = AOIDataDefine.GetWndDefectIDText(WndDefectID);
		str1.Format(_T("%s[%d]"), str, WndGroupID+1);
		m_wndWndList.EnableHeaderCtrl(TRUE, str1, str2);
	}	

	strTest = _T("Test");
	strLogic = _T("Logic");	
	strTest = LoadMultiLanguageString(strTest, strTest);
	strLogic = LoadMultiLanguageString(strLogic, strLogic);

	bool bSortMode=true;
	BOOL bRedraw = FALSE;
	BOOL bAdjustLayou = FALSE;	
	//先加入不良的, 再加入未檢測, 後加入良品的
if ( true == bSortMode )
{
	int        IntVal=0;
	CSortObj   SortObj;
	std::vector<CSortObj> SortList;
	SortObj.SetSortMode(SORT_BY_INT);
	
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = ModelPtr->GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }		
		if ( WndPtr->GetWndGroupID() != WndGroupID ) { continue; }			
		ResultID = WndPtr->GetWndResultID();
		LogicResultID = WndPtr->GetWndLogicResultID();
		switch ( ResultID )
		{
		case RESULT_ID_EXCEPTION:	IntVal=0; break;
		case RESULT_ID_NG:
			if ( RESULT_ID_OK == LogicResultID )
			{	IntVal=5; }
			else
			{	IntVal=1; }
			break;
		case RESULT_ID_NONE:		IntVal=6; break;
		case RESULT_ID_OK:			IntVal=7; break;
		case RESULT_ID_SKIP:		IntVal=8; break;
		case RESULT_ID_BYPASS:		IntVal=9; break;
		default:
			break;
		}
		SortObj.SetID(i);
		SortObj.SetPtr(WndPtr);
		SortObj.SetValueInt(IntVal);
		SortList.push_back(SortObj);
	}
	std::sort(SortList.begin(), SortList.end());
	const size_t SortCount=SortList.size();
	wndPropList.ShowWindow(SW_HIDE);	
	for ( i=0; i<SortCount; i++ )
	{
		WndPtr = (CAOIWnd*)(SortList[i].GetPtr());
		if ( NULL == WndPtr ) { continue; }

		WndIdx           = SortList[i].GetID();
		WndEnabled       = WndPtr->GetWndEnabled();
		WndLogicType     = WndPtr->GetWndLogicType();
		WndInspectedTime = WndPtr->GetWndInspectedTime();			
		ResultID         = WndPtr->GetWndResultID();
		LogicResultID    = WndPtr->GetWndLogicResultID();
		//strCaption.Format(_T("Wnd#%d_GP%d"), i+1, WndGroupID+1);
		str = _T("Wnd");
		str = LoadMultiLanguageString(str, str);
		if ( WND_LOGIC_NONE == WndLogicType) 
		{	strCaption.Format(_T("%s_%s"), str, AOIDataDefine.GetWndIndexText(WndIdx)); }
		else
		{	strCaption.Format(_T("%s_%s %s"), str, AOIDataDefine.GetWndIndexText(WndIdx), strLogic); }
		strResult = AOIDataDefine.GetResultIDText(ResultID);
		strValue.Format(_T("%s [%.1fms]"), strResult, WndInspectedTime);
		pWndItem = new CJETPropertyGridProperty(strCaption, strValue);
		if ( NULL == pWndItem ) { continue; }
			
		//pWndItem->Enable(FALSE);
		pWndItem->AllowEdit(FALSE);
		pWndItem->SetData((DWORD_PTR)(WndPtr));					
		pWndItem->SetID(WND_OBJ_PROPERTY_ENABLE);
		pWndItem->SetCheckValue(WndEnabled);

		Color = GetResultColor(ResultID, LogicResultID);			
		pWndItem->SetValueTextColor(Color);		
		wndPropList.AddProperty(pWndItem, bRedraw, bAdjustLayou);
	}	
	if ( FALSE == bAdjustLayou )
	{	wndPropList.AdjustLayout(); }
	wndPropList.ShowWindow(SW_SHOW);
}
else//bNewMode=false;
{
	//先整理暫時變數
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = ModelPtr->GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		WndPtr->SetWndTempInt(FN_DISABLE);
	}
	wndPropList.ShowWindow(SW_HIDE);	
	for ( j=0; j<7; j++ )
	{
		switch ( j ) 
		{
		case 0:	TargetResultID=RESULT_ID_EXCEPTION; break; 
		case 1:	TargetResultID=RESULT_ID_NG; break; 
		case 2:	TargetResultID=RESULT_ID_NG; break; 
		case 3:	TargetResultID=RESULT_ID_NONE; break; 
		case 4:	TargetResultID=RESULT_ID_OK; break;		
		case 5:	TargetResultID=RESULT_ID_SKIP; break;
		case 6:	TargetResultID=RESULT_ID_BYPASS; break;		
		}
		if ( 1 == j )//使用邏輯瑕疵
		{	bCheckLogicNG = true;	}
		else
		{	bCheckLogicNG = false;	}
		for ( i=0; i<WndCount; i++ )
		{
			WndPtr = ModelPtr->GetModelWndPtr(i, false);
			if ( NULL == WndPtr ) { continue; }
			if ( WndPtr->GetWndTempInt() != FN_DISABLE ) { continue; }
			if ( WndPtr->GetWndGroupID() != WndGroupID ) { continue; }			
			ResultID = WndPtr->GetWndResultID();
			LogicResultID = WndPtr->GetWndLogicResultID();
			if ( false == bCheckLogicNG )
			{
				if ( TargetResultID != ResultID ) { continue; }				
			}
			else
			{
				if ( TargetResultID != LogicResultID ) { continue; }
			}

			WndPtr->SetWndTempInt(FN_ENABLE);//加過勿加
			WndIdx           = (int)(i);
			WndEnabled       = WndPtr->GetWndEnabled();
			WndLogicType     = WndPtr->GetWndLogicType();
			WndInspectedTime = WndPtr->GetWndInspectedTime();			
			//strCaption.Format(_T("Wnd#%d_GP%d"), i+1, WndGroupID+1);
			str = _T("Wnd");
			str = LoadMultiLanguageString(str, str);
			if ( WND_LOGIC_NONE == WndLogicType) 
			{	strCaption.Format(_T("%s_%s"), str, AOIDataDefine.GetWndIndexText(WndIdx)); }
			else
			{	strCaption.Format(_T("%s_%s %s"), str, AOIDataDefine.GetWndIndexText(WndIdx), strLogic); }
			strResult = AOIDataDefine.GetResultIDText(ResultID);
			strValue.Format(_T("%s [%.1fms]"), strResult, WndInspectedTime);
			pWndItem = new CJETPropertyGridProperty(strCaption, strValue);
			if ( NULL == pWndItem ) { continue; }
			
			//pWndItem->Enable(FALSE);
			pWndItem->AllowEdit(FALSE);
			pWndItem->SetData((DWORD_PTR)(WndPtr));					
			pWndItem->SetID(WND_OBJ_PROPERTY_ENABLE);
			pWndItem->SetCheckValue(WndEnabled);

			Color = GetResultColor(ResultID, LogicResultID);			
			pWndItem->SetValueTextColor(Color);			
			wndPropList.AddProperty(pWndItem, bRedraw, bAdjustLayou);
		}
	}	
	if ( FALSE == bAdjustLayou )
	{	wndPropList.AdjustLayout(); }
	wndPropList.ShowWindow(SW_SHOW);	
}//bNewMode	

	int PropCount = wndPropList.GetPropertyCount();
	if ( PropCount > 0 ) 
	{	
		HDITEM hdItem;		
		hdItem.mask = HDI_TEXT;
		const int    ColID=1;
		const size_t Len=1024;
		TCHAR TxtBuf[Len];
		CMFCHeaderCtrl &HeaderCtrl=m_wndWndList.GetHeaderCtrl();
		//hdItem.pszText = (LPTSTR) lpszLeftColumn;
		hdItem.pszText = (LPTSTR) TxtBuf;
		hdItem.cchTextMax = Len-1;
		HeaderCtrl.GetItem(ColID, &hdItem);

		str = TxtBuf;
		::_stprintf(TxtBuf, _T("%s[%d]"), str, PropCount);		

		hdItem.pszText = (LPTSTR) TxtBuf;		
		hdItem.cchTextMax = static_cast<int>(_tcslen(TxtBuf)) + 1;
		HeaderCtrl.SetItem(ColID, &hdItem);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::ClearWndObjList()
{
	CString str1, str2;
	str1 = _T("Wnd");
	str1 = LoadMultiLanguageString(str1, str1);
	str2 = _T("Result");
	str2 = LoadMultiLanguageString(str2, str2);	
	m_wndWndList.EnableHeaderCtrl(TRUE, str1, str2);

	m_wndWndList.RemoveAll();	
	//m_wndWndList.AdjustLayout();
	m_wndWndList.Invalidate();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::UpdateWndObjListSelected()
{
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL==ModelPtr || NULL == m_ActiveWndPtr ) 
	{ 
		ClearWndObjList();
		ClearWndParamList();
		return true; 
	}		

	int       i=0, j=0;
	int       nData = 0;		
	CString   strValue;
	CString   strResult;
	COLORREF  Color=0x000000;
	CAOIWnd  *WndPtr = NULL;
	DWORD_PTR PropData = 0;
	RESULT_ID ResultID = RESULT_ID_NONE;	
	RESULT_ID LogicResultID = RESULT_ID_NONE;	
	const double WndInspectedTime = m_ActiveWndPtr->GetWndInspectedTime();
	const int WndGroupID = m_ActiveWndPtr->GetWndGroupID();
	const int PropCount = m_wndWndList.GetPropertyCount();
	CMFCPropertyGridProperty* pProp=NULL ;	
	pProp = m_wndWndList.FindItemByData((DWORD_PTR)(m_ActiveWndPtr), TRUE);
	
	if ( NULL == pProp )
	{
		BuildWndObjList(ModelPtr, WndGroupID);
		pProp = m_wndWndList.FindItemByData((DWORD_PTR)(m_ActiveWndPtr), TRUE);
	}

	if ( NULL != pProp )
	{	
		CJETPropertyGridProperty* pWndItem = (CJETPropertyGridProperty*)(pProp);
		WndPtr = (CAOIWnd*)(pProp->GetData());		
		ResultID = WndPtr->GetWndResultID();
		LogicResultID = WndPtr->GetWndLogicResultID();
		strResult = AOIDataDefine.GetResultIDText(ResultID);
		Color = GetResultColor(ResultID, LogicResultID);
		strValue.Format(_T("%s [%.1fms]"), strResult, WndInspectedTime);		
		pWndItem->SetValue(strValue);		
		pWndItem->SetValueTextColor(Color);
		this->m_wndWndList.SetCurSel(pProp, TRUE);	
		this->m_wndWndList.EnsureVisible(pProp, FALSE);		
	}
	else
	{	
		ClearWndObjList(); 
		ClearWndParamList();
	}

	ShowWndGroupItem();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::ExecWndObjListLClicked(CJETPropertyGridProperty *pProp)
{
	return true;//由ExecWndObjListSelChanged發送

	if ( NULL == pProp ) { return false; }
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return true; }
	CAOIProject *Project = GetActiveProject();
	if ( NULL == Project ) { return true; }
	CAOIWnd      *WndPtr = (CAOIWnd*)(pProp->GetData());
	if ( NULL == WndPtr ) { return false; }
	if ( ModelPtr->CheckModelWndValid(WndPtr) == false ) { return false; }
	const int WndGroupID = WndPtr->GetWndGroupID();
	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();

	ModelPtr->UnSelectModel();
	ModelPtr->InvisibleModelWnd();	
	WndPtr->SetWndSelected(true);	
	ModelPtr->SetModelWndActived(WndPtr);
	ModelPtr->SetModelWndVisibledByWndGroupID(WndGroupID, -1, true);	
	AlgParam.SetAlgBinParamActived(BIN_PARAM_BELONG_TO_ALG_IMAGE);
	SetActiveWndPtr(WndPtr);
	BuildWndParamList();
	PostMessageToMainFrameWnd(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::ExecWndObjListRClicked(CJETPropertyGridProperty *pProp)
{
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::ExecWndObjListLDbClick(CJETPropertyGridProperty *pProp)
{
	if ( NULL == pProp ) { return false; }
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return true; }
	CAOIProject *Project = GetActiveProject();
	if ( NULL == Project ) { return true; }
	CAOIWnd      *WndPtr = (CAOIWnd*)(pProp->GetData());
	if ( NULL == WndPtr ) { return false; }	
	if ( ModelPtr->CheckModelWndValid(WndPtr) == false ) { return false; }

	double    WndPosX=0;
	double    WndPosY=0;
	double    ModelPosX=0;
	double    ModelPosY=0;
	TREGION4D WndRegion;
	TREGION4D ModelRegion;
	const int WndIndex = WndPtr->GetWndIndex();
	WndPtr->GetWndRegionStage(WndRegion);
	ModelPtr->GetModelTotalRegionStage(ModelRegion);
	WndPosX = WndRegion.GetCpX();
	WndPosY = WndRegion.GetCpY();
	ModelPosX = ModelRegion.GetCpX();
	ModelPosY = ModelRegion.GetCpY();	
	if ( AOIDataCollect.CheckModelUniFrameListModelPtr(ModelPtr) == true )
	{	AOIDataCollect.ShowStageToAct(ModelPosX, ModelPosY, ModelRegion, WndRegion);	 }
	else
	{	AOIDataCollect.MoveStageToAct(ModelPosX, ModelPosY, ModelRegion, WndRegion);	}

	DRAW_MODEL_MODE DrawModelMode = AOIDataCollect.GetDrawModelMode();
	if ( DRAW_MODEL_EDIT == DrawModelMode )
	{	
		PostMessageToMainFrameWnd(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_ALG_IMAGE, NULL);
		//PostMessageToMainFrameWnd(MSG_EDIT_MAIN_VIEW_WND, WPARAM_EXEC_WND_INSPECT, NULL);	
	}
	else
	{	PostMessageToMainFrameWnd(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_ALG_IMAGE, NULL);	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::ExecWndObjListRDbClick(CJETPropertyGridProperty *pProp)
{
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::ExecWndObjListChanged(CJETPropertyGridProperty *pProp)
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return true; }
	if ( NULL == pProp ) { return false; }		
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return true; }
	CAOIProject *Project = CEditWndView::GetActiveProject();
	if ( NULL == Project ) { return true; }
	CAOIWnd      *WndPtr = (CAOIWnd*)(pProp->GetData());
	if ( NULL == WndPtr ) { return false; }
	if ( ModelPtr->CheckModelWndValid(WndPtr) == false ) { return false; }	

	bool     bChanged=false;
	bool     bEnabled=true;
	bool     bWndEnabled=true;
	CString      strName = pProp->GetName();	
	CString      strNewValue(pProp->GetValue());
	CString      strOldValue(pProp->GetOriginalValue());
	BOOL bOK = TRUE;
	TWND_PARAM_CHANGED_RESULT Changed;
	WND_OBJ_PROPERTY_ID ParamID = (WND_OBJ_PROPERTY_ID)(pProp->GetID());	

	switch ( ParamID )
	{
	case WND_OBJ_PROPERTY_ENABLE:		
		if ( AOIDataCollect.OperateLevelEditFuncBypassModelWnd() == false )
		{	
			pProp->SetCheckValue(pProp->GetOriginalCheckValue());
			pProp->SetValue(pProp->GetOriginalValue());//重繪(SetCheckValue)不會重繪
			return false; 
		}
		bEnabled = pProp->GetCheckValue();	
		bWndEnabled = WndPtr->GetWndEnabled();
		if ( bWndEnabled != bEnabled )
		{
			strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
			strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
			WndPtr->SetWndEnabled(bEnabled);
			bChanged = true;
		}		
		break;
	}	
	if ( true == bChanged )
	{		
		LPCTSTR SetText=AOIDataDefine.GetSetText();		
		LogOperCtrl.SaveLogModelOperate(ModelPtr, SetText, strName, strOldValue, strNewValue);	

		WndPtr->SetWndModified(true);
		ModelPtr->ApplyModelWnd(WndPtr);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::ExecWndObjListSelChanged(CJETPropertyGridProperty *pProp)
{
	if ( NULL == pProp ) { return false; }
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return true; }
	CAOIProject *Project = GetActiveProject();
	//if ( NULL == Project ) { return true; }
	CAOIWnd      *WndPtr = (CAOIWnd*)(pProp->GetData());
	if ( NULL == WndPtr ) { return false; }	
	if ( ModelPtr->CheckModelWndValid(WndPtr) == false ) { return false; }		
	CString    ModelName = ModelPtr->GetModelName();
	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	unsigned int WndIdx = WndPtr->GetWndIndex();
	const int WndGroupID = WndPtr->GetWndGroupID();
	//unsigned int FrameIndex = AlgParam.GetAlgImageBinParam().GetBinaryFrameIndex();
	//unsigned int FrameUniqueID = AlgParam.GetAlgImageBinParam().GetBinaryFrameUniqueID();
	//UINT EditImagePageWndID = AOIDataCollect.GetEditImagePageWndID();
	
	ModelPtr->InvisibleModelWnd();	
	ModelPtr->SetModelWndVisibledByWndGroupID(WndGroupID, -1, true);		
	if ( AOIDataCollect.CheckMultiSelectMode() == false )
	{	ModelPtr->UnSelectModel(); }
	else
	{	ModelPtr->SetModelWndOtherGroupIDSelected(WndGroupID, false); }

	WndPtr->SetWndVisibled(true);
	WndPtr->SetWndSelected(true);	
	WndPtr->SetWndAllRoiWndActived(false);
	WndPtr->SetWndAllRoiWndSelected(false);	
	WndPtr->SetWndAllMaskWndSelected(false);
	ModelPtr->SetModelWndActived(WndPtr);
	AlgParam.SetAlgBinParamActived(BIN_PARAM_BELONG_TO_ALG_IMAGE);//要加此行
	SetActiveWndPtr(WndPtr);
	if ( NULL != Project )
	{	Project->ActiveProjectComponentModelWnd(ModelName, ModelPtr, WndIdx); }
	BuildWndParamList();		
	AOIDataCollect.ExecAutoSwitchWnd3DFrame(ModelPtr, WndPtr);
	PostMessageToMainFrameWnd(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::ExecWndParamListLClicked(CJETPropertyGridProperty *pProp)
{
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == pProp ) { return false; }
	if ( NULL == ModelPtr ) { return false; }

	CString strName = pProp->GetName();
	CString strValue = pProp->GetValue();
	CString strDescr = pProp->GetDescription();
	DWORD_PTR Data = pProp->GetData();
	CAOIWnd *WndPtr = (CAOIWnd*)(Data);
	const bool bExpand=pProp->IsExpanded();
	TPropGridParam &PropGridParam=GetPropGridParam();
	WND_ALG_PROPERTY_ID ParamID = (WND_ALG_PROPERTY_ID)(pProp->GetID());		
	//PropGridParam.AAAAAAAAAA=bExpand;
	switch ( ParamID )
	{
	case WND_ALG_PROPERTY_WND_ROI_BEGIN:			PropGridParam.bGeneral_WndRoiExpand=bExpand;	break;
	case WND_ALG_PROPERTY_WND_RGN_LINK_MODE:		PropGridParam.bGeneral_RgnLinkExpand=bExpand;	break;	
	case WND_ALG_PROPERTY_WND_MASK_BOX_BEGIN:		PropGridParam.bGeneral_BoxMaskExpand=bExpand;	break;
	case WND_ALG_PROPERTY_MODEL_MASK:				PropGridParam.bGeneral_ModelMaskExpand=bExpand;	break;
	case WND_ALG_PROPERTY_WND_LOGIC_GROUP:			PropGridParam.bGeneral_WndLogicExpand=bExpand;	break;
	case WND_ALG_PROPERTY_EXTEND_RANGE:				PropGridParam.bGeneral_ExtendRangeExpand=bExpand;	break;
	case WND_ALG_PROPERTY_AIMODEL_BEGIN:            PropGridParam.bGeneral_AIModelExpand=bExpand;	break;

	case WND_ALG_PROPERTY_PATTERN_MATCH_SCALE_NODE:	PropGridParam.bPattern_ScaleScopeExpand=bExpand;	break;

	case WND_ALG_PROPERTY_BRIGHT_RATIO_TOLERANCE_ENABLED:PropGridParam.bBrightRatio_ToleranceExpand=bExpand;	break;
	case WND_ALG_PROPERTY_BRIGHT_RATIO_RATIO_ENABLED:	PropGridParam.bBrightRatio_RatioExpand=bExpand;		break;
	case WND_ALG_PROPERTY_BRIGHT_RATIO_RANGE_ENABLED:	PropGridParam.bBrightRatio_RangeExpand=bExpand;		break;
	case WND_ALG_PROPERTY_BRIGHT_RATIO_CONTRAST_ENABLED:PropGridParam.bBrightRatio_ContrastExpand=bExpand;	break;
	case WND_ALG_PROPERTY_BRIGHT_RATIO_XLINE_ENABLED:	PropGridParam.bBrightRatio_ThroughXExpand=bExpand;	break;
	case WND_ALG_PROPERTY_BRIGHT_RATIO_YLINE_ENABLED:	PropGridParam.bBrightRatio_ThroughYExpand=bExpand;	break;

	case WND_ALG_PROPERTY_IPC_NODE:						PropGridParam.bWndCompare_ROIExpand = bExpand;		break;
	case WND_ALG_PROPERTY_OUTER_SHORT_ENABLED_R:		PropGridParam.bOuterShort_RightExpand=bExpand;		break;
	case WND_ALG_PROPERTY_OUTER_SHORT_ENABLED_T:		PropGridParam.bOuterShort_UpExpand=bExpand;			break;
	case WND_ALG_PROPERTY_OUTER_SHORT_ENABLED_L:		PropGridParam.bOuterShort_LeftExpand=bExpand;		break;
	case WND_ALG_PROPERTY_OUTER_SHORT_ENABLED_B:		PropGridParam.bOuterShort_DowndExpand=bExpand;		break;

	case WND_ALG_PROPERTY_PIXEL_COMPARE_PAT_SCOPE:		PropGridParam.bPixelCompare_PatScopeExpand=bExpand;	break;
	case WND_ALG_PROPERTY_PIXEL_COMPARE_CMP_SCOPE:		PropGridParam.bPixelCompare_CmpScopeExpand=bExpand;	break;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::ExecWndParamListRClicked(CJETPropertyGridProperty *pProp)
{
	if ( NULL == pProp ) { return false; }
	CString strName = pProp->GetName();
	CString strValue = pProp->GetValue();
	CString strDescr = pProp->GetDescription();

	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::ExecWndParamListLDbClick(CJETPropertyGridProperty *pProp)
{
	if ( NULL == pProp ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::ExecWndParamListRDbClick(CJETPropertyGridProperty *pProp)
{
	if ( NULL == pProp ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::ExecWndParamListChanged(CJETPropertyGridProperty *pProp)
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return true; }
	if ( NULL == pProp ) { return false; }		
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return true; }
	CAOIProject *Project = CEditWndView::GetActiveProject();
	if ( NULL == Project ) { return true; }
	CAOIWnd      *WndPtr = (CAOIWnd*)(pProp->GetData());
	if ( NULL == WndPtr ) { return false; }
	if ( ModelPtr->CheckModelWndValid(WndPtr) == false ) { return false; }	
	
	BOOL bOK = TRUE;
	TWND_PARAM_CHANGED_RESULT Changed;
	WND_ALG_PROPERTY_ID ParamID = (WND_ALG_PROPERTY_ID)(pProp->GetID());	

	if ( WND_ALG_PROPERTY_BRIGHT_RATIO_BEGIN<=ParamID && WND_ALG_PROPERTY_BRIGHT_RATIO_END>=ParamID )
	{	bOK = ExecWndParamListChanged_BrightRatio(pProp, Changed);	}	
	else if ( WND_ALG_PROPERTY_OUTER_SHORT_BEGIN<=ParamID && WND_ALG_PROPERTY_OUTER_SHORT_END>=ParamID )
	{	bOK = ExecWndParamListChanged_OuterShort(pProp, Changed);	}
	else if ( WND_ALG_PROPERTY_BLOB_BEGIN<=ParamID && WND_ALG_PROPERTY_BLOB_END>=ParamID )
	{	bOK = ExecWndParamListChanged_BlobCount(pProp, Changed);	}
	else if ( WND_ALG_PROPERTY_TILT_BEGIN<=ParamID && WND_ALG_PROPERTY_TILT_END>=ParamID )
	{	bOK = ExecWndParamListChanged_BodyTilt(pProp, Changed);	}
	else if ( WND_ALG_PROPERTY_BARCODE_BEGIN<=ParamID && WND_ALG_PROPERTY_BARCODE_END>=ParamID )
	{	bOK = ExecWndParamListChanged_BarcodeRecognize(pProp, Changed);	}
	else if ( WND_ALG_PROPERTY_OBJECT_MEASURE_BEGIN<=ParamID && WND_ALG_PROPERTY_OBJECT_MEASURE_END>=ParamID )
	{	bOK = ExecWndParamListChanged_ObjectMeasure(pProp, Changed);	}
	else if ( WND_ALG_PROPERTY_MODEL_MATCH_BEGIN<=ParamID && WND_ALG_PROPERTY_MODEL_MATCH_END>=ParamID )
	{	bOK = ExecWndParamListChanged_ModelMatch(pProp, Changed);	}
	else if ( WND_ALG_PROPERTY_IMAGE_MATCH_BEGIN<=ParamID && WND_ALG_PROPERTY_IMAGE_MATCH_END>=ParamID )
	{	bOK = ExecWndParamListChanged_ImageMatch(pProp, Changed);	}
	else if ( WND_ALG_PROPERTY_CHAR_VERIFY_BEGIN<=ParamID && WND_ALG_PROPERTY_CHAR_VERIFY_END>=ParamID )
	{	bOK = ExecWndParamListChanged_CharVerify(pProp, Changed);	}	
	else if ( WND_ALG_PROPERTY_COLOR_CODE_BEGIN<=ParamID && WND_ALG_PROPERTY_COLOR_CODE_END>=ParamID )
	{	bOK = ExecWndParamListChanged_ColorCode(pProp, Changed);	}
	else if ( WND_ALG_PROPERTY_FD_MATCH_BEGIN<=ParamID && WND_ALG_PROPERTY_FD_MATCH_END>=ParamID )
	{	bOK = ExecWndParamListChanged_FdMatch(pProp, Changed);	}
	else if ( WND_ALG_PROPERTY_EDGE_SEARCH_BEGIN<=ParamID && WND_ALG_PROPERTY_EDGE_SEARCH_END>=ParamID )
	{	bOK = ExecWndParamListChanged_EdgeSearch(pProp, Changed);	}
	else if ( WND_ALG_PROPERTY_SHAPE_VERIFY_BEGIN<=ParamID && WND_ALG_PROPERTY_SHAPE_VERIFY_END>=ParamID )
	{	bOK = ExecWndParamListChanged_ShapeVerify(pProp, Changed);	}
	else if ( WND_ALG_PROPERTY_ANGLE_MEASURE_BEGIN<=ParamID && WND_ALG_PROPERTY_ANGLE_MEASURE_END>=ParamID )
	{	bOK = ExecWndParamListChanged_AngleMeasure(pProp, Changed);	}	
	else if ( WND_ALG_PROPERTY_PIXEL_COMPARE_BEGIN<=ParamID && WND_ALG_PROPERTY_PIXEL_COMPARE_END>=ParamID )
	{	bOK = ExecWndParamListChanged_PixelCompare(pProp, Changed);	}
	else if ( WND_ALG_PROPERTY_GROUP_COMPARE_BEGIN<=ParamID && WND_ALG_PROPERTY_GROUP_COMPARE_END>=ParamID )
	{	bOK = ExecWndParamListChanged_GroupCompare(pProp, Changed);	}
	else if ( WND_ALG_PROPERTY_PATTERN_MATCH_BEGIN<=ParamID && WND_ALG_PROPERTY_PATTERN_MATCH_END>=ParamID )
	{	bOK = ExecWndParamListChanged_PatternMatch(pProp, Changed);	}
	else if (WND_ALG_PROPERTY_IPC_BEGIN <= ParamID && WND_ALG_PROPERTY_IPC_END >= ParamID)
	{	bOK = ExecWndParamListChanged_IPC(pProp, Changed); }
	else if (WND_ALG_PROPERTY_RESIN_BEGIN <= ParamID && WND_ALG_PROPERTY_RESIN_END >= ParamID)
	{	bOK = ExecWndParamListChanged_ResinHeight(pProp, Changed); }
	else if (WND_ALG_PROPERTY_AIMODEL_BEGIN <= ParamID && WND_ALG_PROPERTY_AIMODEL_END >= ParamID)
	{	bOK = ExecWndParamListChanged_AIModel(pProp, Changed); }	
	else if (WND_ALG_PROPERTY_SOLDER_WETTING_BEGIN <= ParamID && WND_ALG_PROPERTY_SOLDER_WETTING_END >= ParamID)
	{	bOK = ExecWndParamListChanged_SolderWetting(pProp, Changed); }		
	else if (WND_ALG_PROPERTY_WIRE_WIDTH_BEGIN <= ParamID && WND_ALG_PROPERTY_WIRE_WIDTH_END >= ParamID)
	{   bOK = ExecWndParamListChanged_WireWidth(pProp, Changed); }
	else if (WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_BEGIN <= ParamID && WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_END >= ParamID)
	{	bOK = ExecWndParamListChanged_MeasureBlackGlue(pProp, Changed); }
	else if (WND_ALG_PROPERTY_MEASURE_FLUX_AREA_BEGIN <= ParamID && WND_ALG_PROPERTY_MEASURE_FLUX_AREA_END >= ParamID)
	{	bOK = ExecWndParamListChanged_MeasureFluxArea(pProp, Changed); }
	else if (WND_ALG_PROPERTY_MEASURE_CPU_PIN_BEGIN <= ParamID && WND_ALG_PROPERTY_MEASURE_CPU_PIN_END >= ParamID)
	{	bOK = ExecWndParamListChanged_MeasureCpuPin(pProp, Changed); }
	else if (WND_ALG_PROPERTY_MEASURE_SIP_BEGIN <= ParamID && WND_ALG_PROPERTY_MEASURE_SIP_END >= ParamID)
	{	bOK = ExecWndParamListChanged_MeasureSIP(pProp, Changed);}
	else if (WND_ALG_PROPERTY_MEASURE_CONNECTOR_BEGIN <= ParamID && WND_ALG_PROPERTY_MEASURE_CONNECTOR_END >= ParamID)
	{	bOK = ExecWndParamListChanged_MeasureConnector(pProp, Changed);}
	else		
	{	bOK = ExecWndParamListChanged_General(pProp, Changed);	}
	if ( FALSE == bOK )
	{	return false; }

	if ( ExecWndParamChangedUpdate(ModelPtr, WndPtr, Changed) == false )
	{	return false; }
	return bOK;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::CheckShowAlgAngleMeasureBaseLineMode(ANGLE_MEASURE_MODE Mode) const
{
	bool bShow=true;
	switch ( Mode )
	{
	case ANGLE_MEASURE_TILT: bShow = false;	break;
	default:
	case ANGLE_MEASURE_SKEW: bShow = true;	break;
	}
	return bShow;	      
}
//-------------------------------------------------------------------------------------//

bool CEditWndView::ExecWndParamChangedUpdate(CAOIModel *ModelPtr, CAOIWnd *WndPtr, TWND_PARAM_CHANGED_RESULT Changed)
{
	if ( NULL==ModelPtr || NULL==WndPtr ) { return false; }
	if ( false == Changed.bParamChanged ) { return true; }
	
	bool UpdateSelected = false;
	if ( true == Changed.bWndRgnChanged )
	{	
		WndPtr->UpdateWndExtendBox();	
		//ModelPtr->UpdateModelWndRgnByLinkMode();
		ModelPtr->SetModelModifiedCount(true);
		ModelPtr->SetModelNeedSaveFiles(true);
	}
	WndPtr->SetWndModified(true);
	WndPtr->SetWndUIUpated_Param(false);
	ModelPtr->ApplyModelWnd(WndPtr);

	LPCTSTR SetText=AOIDataDefine.GetSetText();
	CString AlgText=WndPtr->GetWndAlgTypeText();
	LogOperCtrl.SaveLogModelWndOperate(WndPtr, SetText, AlgText, Changed.sValueName, Changed.sValueOld, Changed.sValueNew);	

	if ( true == Changed.bWndRgnChanged )
	{
		UpdateSelected = true;
		ModelPtr->UpdateModelWndRgnByLinkMode();	
		ModelPtr->SetModelModifiedCount(true);
		ModelPtr->SetModelNeedSaveFiles(true);
		ModelPtr->CalcModelTotalRegionAll();
		ModelPtr->UpdateModelRegionToAttached();
	}
	//CAOIComponent *ComponentPtr = ModelPtr->GetModelComponentPtr();
	//const bool ModelIsolated = ComponentPtr->GetComponentModelIsolated();
	//if ( false == ModelIsolated )
	//{
	//	if ( ModelPtr->GetModelModifiedCount() == true ) 
	//	{	Project->ApplyProjectCompnentModelToLibraryModel(ComponentPtr, true); }
	//	else
	//	{	Project->SyncProjectCompnentModelToLibraryModel(ComponentPtr, true, true); }
	//}

	DRAW_MODEL_MODE DrawModelMode = AOIDataCollect.GetDrawModelMode();
	if ( DRAW_MODEL_EDIT != DrawModelMode )
	{
		UpdateSelected = true;
		AOIDataCollect.SetDrawModelMode(DRAW_MODEL_EDIT);
	}

	if ( true == Changed.bFrameIndexChagned )
	{
		CAlgBinaryParam &BinaryParam = WndPtr->GetWndAlgParam().GetAlgImageBinParam();			
		AOIDataCollect.SetBinaryParamTemp(BinaryParam);

		CAOIProject *ProjectPtr=GetActiveProject();
		if ( NULL != ProjectPtr )
		{	ProjectPtr->UpdateProjectColorGroup(BinaryParam); }

		PostMessageToMainFrameWnd(MSG_EDIT_MAIN_VIEW_WND, WPARAM_SWITCH_FRAME_IMAGE, NULL);
	}
	if ( true == UpdateSelected )
	{	PostMessageToMainFrameWnd(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);}						
	SendMessageToMainFrameWnd(MSG_EDIT_IMAGE_VIEW_WND, WPARAM_UPDATE_IMAGE_WND_SELECTED, (LPARAM)(WndPtr));

	//CEditWndView::PostMessageToMainFrameWnd(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_ALG_IMAGE, NULL);
	//Changed.bReBuildWndUI = false;
	if ( DRAW_MODEL_EDIT==DrawModelMode && true==Changed.bReBuildWndUI )
	{	
		PostMessageToMainFrameWnd(MSG_EDIT_MAIN_VIEW_WND, WPARAM_EXEC_WND_INSPECT, NULL); 
		//CWnd::PostMessage(MSG_EDIT_WND_PROPERTY_WND, WPARAM_BUILD_WND_SELECTED, NULL);			
	}		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::ExecWndParamListSelChanged(CJETPropertyGridProperty *pProp)
{
	if ( NULL == pProp ) { return false; }
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return true; }
	CAOIProject *Project = GetActiveProject();
	if ( NULL == Project ) { return true; }
	CAOIWnd      *WndPtr = (CAOIWnd*)(pProp->GetData());
	if ( NULL == WndPtr ) { return false; }
	CAlgParam     &AlgParam = WndPtr->GetWndAlgParam();	
	bool           bChangeFrameID = false;
	CAlgBinaryParam *BinParamPtr = NULL;
	CAlgBinaryParam &ImageBinParam = AlgParam.GetAlgImageBinParam();
	const unsigned int CurrentFrameIndex = Project->GetProjectMapIndex();
	WND_ALG_PROPERTY_ID ParamID = (WND_ALG_PROPERTY_ID)(pProp->GetID());
	BIN_PARAM_BELONG_TO BinParamBelongTo = AlgParam.GetAlgBinParamActived();
	switch ( ParamID )
	{			
	case WND_ALG_PROPERTY_ALG_FRAME_ID:
		if ( BIN_PARAM_BELONG_TO_ALG_IMAGE != BinParamBelongTo )
		{	
			bChangeFrameID = true;
			AlgParam.SetAlgBinParamActived(BIN_PARAM_BELONG_TO_ALG_IMAGE);
		}	
		break;	
	case WND_ALG_PROPERTY_MASK_FRAME_ID:
	case WND_ALG_PROPERTY_MASK_FUNC_MODE:
		if ( BIN_PARAM_BELONG_TO_MASK_IMAGE != BinParamBelongTo )
		{	
			bChangeFrameID = true;
			AlgParam.SetAlgBinParamActived(BIN_PARAM_BELONG_TO_MASK_IMAGE);			
		}		
		break;

#ifdef ALG_MEASURE_BLACK_GLUE_USE
	case WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_FRAME_ID_01:
		if ( CurrentFrameIndex != AlgParam.GetAlgParamMeasureBlackGlue().bgFrameIndex1 )
		{	
			bChangeFrameID = true;
			AlgParam.SetAlgBinParamActived(BIN_PARAM_BELONG_TO_ALG_IMAGE);
			ImageBinParam.SetBinaryFrameIndex(AlgParam.GetAlgParamMeasureBlackGlue().bgFrameIndex1);
			ImageBinParam.SetBinaryFrameUniqueID(AlgParam.GetAlgParamMeasureBlackGlue().bgFrameUniqueID1);
		}
		break;
	case WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_FRAME_ID_02:
		if ( CurrentFrameIndex != AlgParam.GetAlgParamMeasureBlackGlue().bgFrameIndex2 )
		{	
			bChangeFrameID = true;
			AlgParam.SetAlgBinParamActived(BIN_PARAM_BELONG_TO_ALG_IMAGE);
			ImageBinParam.SetBinaryFrameIndex(AlgParam.GetAlgParamMeasureBlackGlue().bgFrameIndex2);
			ImageBinParam.SetBinaryFrameUniqueID(AlgParam.GetAlgParamMeasureBlackGlue().bgFrameUniqueID2);
		}
		break;
	case WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_FRAME_ID_03:
		if ( CurrentFrameIndex != AlgParam.GetAlgParamMeasureBlackGlue().bgFrameIndex3 )
		{	
			bChangeFrameID = true;
			AlgParam.SetAlgBinParamActived(BIN_PARAM_BELONG_TO_ALG_IMAGE);
			ImageBinParam.SetBinaryFrameIndex(AlgParam.GetAlgParamMeasureBlackGlue().bgFrameIndex3);
			ImageBinParam.SetBinaryFrameUniqueID(AlgParam.GetAlgParamMeasureBlackGlue().bgFrameUniqueID3);
		}
		break;
	case WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_FRAME_ID_04:
		if ( CurrentFrameIndex != AlgParam.GetAlgParamMeasureBlackGlue().bgFrameIndex4 )
		{	
			bChangeFrameID = true;
			AlgParam.SetAlgBinParamActived(BIN_PARAM_BELONG_TO_ALG_IMAGE);
			ImageBinParam.SetBinaryFrameIndex(AlgParam.GetAlgParamMeasureBlackGlue().bgFrameIndex4);
			ImageBinParam.SetBinaryFrameUniqueID(AlgParam.GetAlgParamMeasureBlackGlue().bgFrameUniqueID4);
		}
		break;
#endif//ALG_MEASURE_BLACK_GLUE_USE

#ifdef ALG_MEASURE_FLUX_AREA_USE
	case WND_ALG_PROPERTY_MEASURE_FLUX_AREA_FRAME_ID_01:
		if ( CurrentFrameIndex != AlgParam.GetAlgParamMeasureFluxArea().faFrameIndex1 )
		{	
			bChangeFrameID = true;
			AlgParam.SetAlgBinParamActived(BIN_PARAM_BELONG_TO_ALG_IMAGE);
			ImageBinParam.SetBinaryFrameIndex(AlgParam.GetAlgParamMeasureFluxArea().faFrameIndex1);
			ImageBinParam.SetBinaryFrameUniqueID(AlgParam.GetAlgParamMeasureFluxArea().faFrameUniqueID1);
		}
		break;
#endif//ALG_MEASURE_FLUX_AREA_USE

	default:
		if ( BIN_PARAM_BELONG_TO_ALG_IMAGE != BinParamBelongTo )
		{	
			bChangeFrameID = true;
			AlgParam.SetAlgBinParamActived(BIN_PARAM_BELONG_TO_ALG_IMAGE);			
		}
		break;
	}

	if ( true == bChangeFrameID )
	{
		BinParamPtr = AlgParam.GetAlgActiveBinParamPtr();
		if ( NULL != BinParamPtr )
		{
			const unsigned int FrameUniqueID = BinParamPtr->GetBinaryFrameUniqueID();
			const unsigned int FrameIndex = Project->GetProjectFrameIndexByUniqueID(FrameUniqueID);			
			if ( Project->SetProjectMapIndex(FrameIndex) == true )
			{	
				//CEditWndView::PostMessageToMainFrameWnd(MSG_EDIT_MAIN_VIEW_WND, WPARAM_SWITCH_FRAME_IMAGE, NULL);
			}
		}
		WndPtr->SetWndUIUpated_Param(false);
		SendMessageToMainFrameWnd(MSG_EDIT_IMAGE_VIEW_WND, WPARAM_UPDATE_IMAGE_WND_SELECTED, (LPARAM)(WndPtr));
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::ExecWndParamListChanged_General(CJETPropertyGridProperty *pProp, TWND_PARAM_CHANGED_RESULT &Changed)
{
	if ( NULL == pProp ) { return false; }
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return true; }
	CAOIProject *Project = GetActiveProject();
	if ( NULL == Project ) { return true; }
	CAOIWnd      *WndPtr = (CAOIWnd*)(pProp->GetData());
	if ( NULL == WndPtr ) { return false; }
	CAlgParam       &AlgParam = WndPtr->GetWndAlgParam();	
	CAlgBinaryParam *AlgBinParamPtr = AlgParam.GetAlgImageBinParamPtr();
	CAlgBinaryParam *MskBinParamPtr = AlgParam.GetAlgMaskBinParamPtr();

	int           LandGroupID=0;	
	bool          bShow = true;
	bool          WndEnabled = false;
	bool          bWndRgnChanged=false;
	bool          bReBuildWndUI = true;
	bool          AlgFrameIndexChanged=false;
	unsigned int  FrameIndex = 0;
	unsigned int  FrameUniqueID = 0;
	TFrameParam  *FrameParamPtr = NULL;	
	BINARY_MODE       BinaryMode=BINARY_DISABLE;
	MASK_FUNC_MODE    MaskFuncMode=MASK_FUNC_RETURN;
	BOX_SHAPE_MODE    BoxShapeMode=BOX_SHAPE_RETURN;
	WND_DEFECT_ID     WndDefectID = WND_DEFECT_NONE;	
	WND_LOGIC_TYPE    WndLogicType = WND_LOGIC_NONE;
	WND_FOLLOW_MODE   WndFollowMode=WND_FOLLOW_NONE;	
	WND_RGN_LINK_MODE WndRgnLinkMode=WND_RGN_LINK_NONE;
	WND_SYNC_MOVE_MODE WndSyncMoveMode=WND_SYNC_MOVE_NONE;
	WND_CONSTRAIN_MODE WndConstrainMode=WND_CONSTRAIN_DISABLE;
	bool          bChanged = false;	
	bool          bBoolParam = false;
	bool          bChangeFrame=false;	
	int           nValue=0, nReading=0;
	bool          bValue=false, bReading=false, bEnabled=true;
	BOOL          BValue=FALSE, BReading=FALSE;
	double        dValue=0.0, dReading=0.0, dValue2=0.0;	
	CString       strValue;
	CString       str, str2, str3;
	CString       strName = pProp->GetName();
	CString       strDescr = pProp->GetDescription();
	COleVariant   vtValue = pProp->GetValue();	
	CString       strNewValue(pProp->GetValue());
	CString       strOldValue(pProp->GetOriginalValue());
	DWORD_PTR     dwData = pProp->GetData();	
	BOOL          bClickBtn = pProp->GetClickUserBtn();
	CJETPropertyGridProperty *pProp2 = NULL;	
	WND_ALG_PROPERTY_ID ParamID = (WND_ALG_PROPERTY_ID)(pProp->GetID());	
	CJETPropertyGridCtrl &wndPropList = m_wndWndParam;
	const int ColorGroupLinkIndex = AlgBinParamPtr->GetBinaryColorGroupLinkIndex();
	bReBuildWndUI = false;
	switch ( ParamID )
	{
	case WND_ALG_PROPERTY_SCOPE_BEGIN:
		break;

	case WND_ALG_PROPERTY_BASIC_BEGIN:
		break;	
	case WND_ALG_PROPERTY_WND_DEFECT_ID:
		strValue = pProp->GetValue();
		WndEnabled = pProp->GetCheckValue();
		WndDefectID = AOIDataDefine.FindWndDefectIDByDefectText(strValue);
		if ( WND_DEFECT_NONE != WndDefectID )
		{	WndPtr->SetWndDefectID(WndDefectID);	}
		WndPtr->SetWndEnabled(WndEnabled);		
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_DEFECT_GROUP_ID:
		strValue = pProp->GetValue();		
		nValue = ::_ttoi(strValue)-1;
		if ( nValue < 0 ) { nValue = -1; }
		WndPtr->SetWndDefectGroupID(nValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_ALG_FRAME_ID:
		bChangeFrame=false;
		strValue = pProp->GetValue();
		BinaryMode=AlgBinParamPtr->GetBinaryMode();
		FrameUniqueID = DecodeFrameUniqueID(strValue);		
		FrameIndex = Project->GetProjectFrameIndexByUniqueID(FrameUniqueID);
		FrameParamPtr = AOIDataCollect.GetSystemFrameParamPtrByUniqueID(FrameUniqueID);		
		if ( -1!= FrameIndex && NULL!=FrameParamPtr )
		{	
			if ( ColorGroupLinkIndex<0 || BINARY_COLOR_FILTER!=BinaryMode) 
			{	bChangeFrame = true; }
			else
			{
				str2 = _T("The frame link to Project Color Group");
				str2 = LoadMultiLanguageString(str2, str2);
				str3.Format(_T("%s [%d]"), str2, ColorGroupLinkIndex+1);
				str2 = _T("Do you want to change the Frame?");
				str2 = LoadMultiLanguageString(str2, str2);
				str.Format(_T("%s\n%s"), str3, str2);
				if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDYES )
				{	bChangeFrame = true; }
			}
		}
		if ( false == bChangeFrame )
		{
			strValue = pProp->GetOriginalValue();			
			pProp->SetValue(strValue);
		}
		else
		{
			Project->SetProjectMapIndex(FrameIndex);			
			WndPtr->SetWndAlgImageFrameIndex(FrameIndex);
			WndPtr->SetWndAlgImageFrameUniqueID(FrameUniqueID);
			CAlgParam::AdjustAlgBinaryParam(FrameParamPtr->FrameType, *AlgBinParamPtr);				
			bChanged = true;
			AlgFrameIndexChanged = true;
		}
		AlgParam.SetAlgBinParamActived(BIN_PARAM_BELONG_TO_ALG_IMAGE);
		break;	
	case WND_ALG_PROPERTY_MASK_FRAME_ID:
		bChangeFrame=false;
		strValue = pProp->GetValue();
		bEnabled = pProp->GetCheckValue();
		BinaryMode=MskBinParamPtr->GetBinaryMode();
		FrameUniqueID = DecodeFrameUniqueID(strValue);
		FrameIndex = Project->GetProjectFrameIndexByUniqueID(FrameUniqueID);
		FrameParamPtr = AOIDataCollect.GetSystemFrameParamPtrByUniqueID(FrameUniqueID);
		if ( -1!= FrameIndex && NULL!=FrameParamPtr )
		{	
			if ( ColorGroupLinkIndex<0 || BINARY_COLOR_FILTER!=BinaryMode) 
			{	bChangeFrame = true; }
			else
			{
				str2 = _T("The frame link to Project Color Group");
				str2 = LoadMultiLanguageString(str2, str2);
				str3.Format(_T("%s [%d]"), str2, ColorGroupLinkIndex+1);
				str2 = _T("Do you want to change the Frame?");
				str2 = LoadMultiLanguageString(str2, str2);
				str.Format(_T("%s\n%s"), str3, str2);
				if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDYES )
				{	bChangeFrame = true; }
			}			
		}
		if ( false == bChangeFrame )
		{
			strValue = pProp->GetOriginalValue();			
			pProp->SetValue(strValue);
		}
		else
		{
			Project->SetProjectMapIndex(FrameIndex);			
			MskBinParamPtr->SetBinaryFrameIndex(FrameIndex);			
			MskBinParamPtr->SetBinaryFrameUniqueID(FrameUniqueID);
			CAlgParam::AdjustAlgBinaryParam(FrameParamPtr->FrameType, *MskBinParamPtr);				
			bChanged = true;		
			AlgFrameIndexChanged = true;
		}
		//AlgParam.SetAlgBinParamActived(BIN_PARAM_BELONG_TO_MASK_IMAGE);
		break;
	case WND_ALG_PROPERTY_MASK_FUNC_MODE:
		strValue = pProp->GetValue();		
		MaskFuncMode = AOIDataDefine.FindAlgMaskFuncModeByText(strValue);
		MskBinParamPtr->SetMaskFuncMode(MaskFuncMode);
		bChanged = true;		
		break;
	case WND_ALG_PROPERTY_WND_RGN_LINK_MODE:
		strValue = pProp->GetValue();		
		WndRgnLinkMode = AOIDataDefine.FindWndRgnLinkModeByLinkModeText(strValue);
		if ( WND_RGN_LINK_NONE == WndRgnLinkMode )
		{	WndPtr->SetWndRgnLinkAuto(false); }
		else
		{	WndPtr->SetWndRgnLinkAuto(true); }
		WndPtr->SetWndRgnLinkMode(WndRgnLinkMode);		

		dValue = 100;
		WndPtr->SetWndRgnLinkRatioX(dValue);
		WndPtr->SetWndRgnLinkRatioY(dValue);
		pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_WND_RGN_LINK_RATIO_X);
		if ( NULL != pProp2 )
		{	
			str.Format(_T("%.2f"), dValue); 
			pProp2->SetValue(str);
			pProp2->Redraw();
		}
		pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_WND_RGN_LINK_RATIO_Y);
		if ( NULL != pProp2 )
		{	
			str.Format(_T("%.2f"), dValue); 
			pProp2->SetValue(str);
			pProp2->Redraw();
		}
		bChanged = true;
		bWndRgnChanged = true;
		break;
	case WND_ALG_PROPERTY_WND_RGN_LINK_RATIO_X:
		strValue = pProp->GetValue();
		dValue = JetAPI::StrToDbl(strValue);
		WndPtr->SetWndRgnLinkRatioX(dValue);
		bChanged = true;
		bWndRgnChanged = true;
		break;
	case WND_ALG_PROPERTY_WND_RGN_LINK_RATIO_Y:
		strValue = pProp->GetValue();
		dValue = JetAPI::StrToDbl(strValue);
		WndPtr->SetWndRgnLinkRatioY(dValue);
		bChanged = true;
		bWndRgnChanged = true;
		break;
	case WND_ALG_PROPERTY_EXTEND_RANGE:
		pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_EXTEND_RANGE_X);
		if ( NULL != pProp2 )
		{
			strValue = pProp2->GetValue();
			dValue = ::_tcstod(strValue, NULL);		
			if ( dValue < 0 )
			{	dValue = 0; }			
			strValue.Format(_T("%.0f"), dValue);		
			pProp2->SetValue(strValue);
			dValue2 = dValue;
			WndPtr->SetWndExtendRangeX(dValue);
			bChanged = true;
			bWndRgnChanged = true;			
		}
		pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_EXTEND_RANGE_Y);
		if ( NULL != pProp2 )
		{
			if ( pProp2->IsModified() == TRUE )
			{
				strValue = pProp2->GetValue();
				dValue = ::_tcstod(strValue, NULL);		
				if ( dValue < 0 )
				{	dValue = 0; }				
			}
			else
			{	dValue = dValue2; }			
			strValue.Format(_T("%.0f"), dValue);		
			pProp2->SetValue(strValue);		
			WndPtr->SetWndExtendRangeY(dValue);
			bChanged = true;
			bWndRgnChanged = true;			
		}
		break;
	case WND_ALG_PROPERTY_EXTEND_RANGE_X:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue < 0 )
		{	dValue = 0; }
		WndPtr->SetWndExtendRangeX(dValue);		
		strValue.Format(_T("%.0f"), dValue);		
		pProp->SetValue(strValue);
		bChanged = true;
		bReBuildWndUI = false;
		bWndRgnChanged = true;		
		break;
	case WND_ALG_PROPERTY_EXTEND_RANGE_Y:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue < 0 )
		{	dValue = 0; }		
		WndPtr->SetWndExtendRangeY(dValue);
		strValue.Format(_T("%.0f"), dValue);		
		pProp->SetValue(strValue);
		bChanged = true;
		bReBuildWndUI = false;
		bWndRgnChanged = true;
		break;
	case WND_ALG_PROPERTY_MODEL_MASK:		
		break;
	case WND_ALG_PROPERTY_MODEL_MASK_PAD:
		bChanged = true;
		bEnabled = pProp->GetCheckValue();
		bBoolParam = true;
		strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
		strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
		if ( true == bEnabled )
		{	WndPtr->AddWndModelMaskFlag(MODEL_MASK_PAD); }
		else
		{	WndPtr->RemoveWndModelMaskFlag(MODEL_MASK_PAD); }
		break;
	case WND_ALG_PROPERTY_MODEL_MASK_BODY:
		bChanged = true;
		bEnabled = pProp->GetCheckValue();
		bBoolParam = true;
		strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
		strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
		if ( true == bEnabled )
		{	WndPtr->AddWndModelMaskFlag(MODEL_MASK_BODY); }
		else
		{	WndPtr->RemoveWndModelMaskFlag(MODEL_MASK_BODY); }
		break;
	case WND_ALG_PROPERTY_MODEL_MASK_BODY_NO_LEAD:
		bChanged = true;
		bEnabled = pProp->GetCheckValue();
		bBoolParam = true;
		strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
		strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
		if ( true == bEnabled )
		{	WndPtr->AddWndModelMaskFlag(MODEL_MASK_BODY_NO_LEAD); }
		else
		{	WndPtr->RemoveWndModelMaskFlag(MODEL_MASK_BODY_NO_LEAD); }
		break;	
	case WND_ALG_PROPERTY_MODEL_MASK_LEAD:
		bChanged = true;
		bEnabled = pProp->GetCheckValue();
		bBoolParam = true;
		strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
		strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
		if ( true == bEnabled )
		{	WndPtr->AddWndModelMaskFlag(MODEL_MASK_LEAD); }
		else
		{	WndPtr->RemoveWndModelMaskFlag(MODEL_MASK_LEAD); }
		break;
	case WND_ALG_PROPERTY_MODEL_MASK_LEAD_TIP:
		bChanged = true;
		bEnabled = pProp->GetCheckValue();
		bBoolParam = true;
		strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
		strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
		if ( true == bEnabled )
		{	WndPtr->AddWndModelMaskFlag(MODEL_MASK_LEAD_TIP); }
		else
		{	WndPtr->RemoveWndModelMaskFlag(MODEL_MASK_LEAD_TIP); }
		break;
	case WND_ALG_PROPERTY_MODEL_MASK_LEAD_SHOULDER:
		bChanged = true;
		bEnabled = pProp->GetCheckValue();
		bBoolParam = true;
		strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
		strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
		if ( true == bEnabled )
		{	WndPtr->AddWndModelMaskFlag(MODEL_MASK_LEAD_SHOULDER); }
		else
		{	WndPtr->RemoveWndModelMaskFlag(MODEL_MASK_LEAD_SHOULDER); }
		break;
	case WND_ALG_PROPERTY_MODEL_MASK_CLEAR:
		if ( TRUE == bClickBtn )
		{	
			bChanged = true;
			bReBuildWndUI = true;
			WndPtr->SetWndModelMaskFlag(MODEL_MASK_NONE);
		}
		break;
	case WND_ALG_PROPERTY_BASIC_END:
		break;

	case WND_ALG_PROPERTY_WND_ROI_BEGIN:
		break;
	case WND_ALG_PROPERTY_WND_ROI_ADD:
		if ( TRUE == bClickBtn )
		{	
			bReBuildWndUI = false;
			bChanged = ExecWndParamListChanged_WndRoi_Add(WndPtr);				
		}
		break;
	case WND_ALG_PROPERTY_WND_ROI_DELETE:
		if ( TRUE == bClickBtn )
		{
			bReBuildWndUI = false;
			bChanged = ExecWndParamListChanged_WndRoi_Delete(WndPtr);	
		}
		break;
	case WND_ALG_PROPERTY_WND_ROI_CLEAR:
		if ( TRUE == bClickBtn )
		{
			bReBuildWndUI = false;
			bChanged = ExecWndParamListChanged_WndRoi_ClearList(WndPtr);	
		}
		break;
	case WND_ALG_PROPERTY_WND_ROI_ADD_AUTO:
		if ( TRUE == bClickBtn )
		{
			bReBuildWndUI = false;
			bChanged = ExecWndParamListChanged_WndRoi_Add_Auto(WndPtr);	
		}
		break;
	case WND_ALG_PROPERTY_WND_ROI_END:
		break;

	case WND_ALG_PROPERTY_WND_MASK_BOX_BEGIN:
		break;
	case WND_ALG_PROPERTY_WND_MASK_BOX_ADD:
		if ( TRUE == bClickBtn )
		{
			bReBuildWndUI = false;
			bChanged = ExecWndParamListChanged_MaskBox_Add(WndPtr);	
		}
		break;
	case WND_ALG_PROPERTY_WND_MASK_BOX_ROTATE:
		strValue = pProp->GetValue();
		dValue = JetAPI::StrToDbl(strValue);
		//if ( TRUE == bClickBtn )
		{
			bReBuildWndUI = false;
			bChanged = ExecWndParamListChanged_MaskBox_Rotate(WndPtr, dValue);	
		}
		break;
	case WND_ALG_PROPERTY_WND_MASK_BOX_SHAPE_MODE:
		strValue = pProp->GetValue();
		BoxShapeMode = AOIDataDefine.FindBoxShapeModeByText(strValue);
		//if ( TRUE == bClickBtn )
		{
			bReBuildWndUI = false;
			bChanged = ExecWndParamListChanged_MaskBox_ShapeMode(WndPtr, BoxShapeMode);	
		}
		break;
	case WND_ALG_PROPERTY_WND_MASK_BOX_SHAPE_PARAM_1:
		strValue = pProp->GetValue();
		dValue = ::_ttof(strValue);
		//if ( TRUE == bClickBtn )
		{
			bReBuildWndUI = false;
			bChanged = ExecWndParamListChanged_MaskBox_ShapeParam(WndPtr, dValue);	
		}
		break;
	case WND_ALG_PROPERTY_WND_MASK_BOX_SHAPE_PARAM_2:
		strValue = pProp->GetValue();
		dValue = ::_ttof(strValue);
		//if ( TRUE == bClickBtn )
		{
			bReBuildWndUI = false;
			bChanged = ExecWndParamListChanged_MaskBox_ShapeParam(WndPtr, dValue);	
		}
		break;
	case WND_ALG_PROPERTY_WND_MASK_BOX_DELETE:
		if ( TRUE == bClickBtn )
		{
			bReBuildWndUI = false;
			bChanged = ExecWndParamListChanged_MaskBox_Delete(WndPtr);	
		}
		break;
	case WND_ALG_PROPERTY_WND_MASK_BOX_CLEAR:
		if ( TRUE == bClickBtn )
		{
			bReBuildWndUI = false;
			bChanged = ExecWndParamListChanged_MaskBox_ClearList(WndPtr);	
		}
		break;
	case WND_ALG_PROPERTY_WND_MASK_BOX_END:
		break;

	case WND_ALG_PROPERTY_ADVANCE_BEGIN:
		break;
	case WND_ALG_PROPERTY_WND_LOGIC_TYPE:
		strValue = pProp->GetValue();		
		WndLogicType = AOIDataDefine.FindWndLogicTypeByText(strValue);
		WndPtr->SetWndLogicType(WndLogicType);		
		bChanged = true;
		bReBuildWndUI = false;
		break;
	case WND_ALG_PROPERTY_WND_LOGIC_GROUP_ID:
		strValue = pProp->GetValue();		
		nValue = ::_ttoi(strValue)-1;
		WndPtr->SetWndLogicGroupID(nValue);		
		bChanged = true;
		bReBuildWndUI = false;
		break;
	case WND_ALG_PROPERTY_ALG_BASE_VALUE_GROUP:
		break;
	case WND_ALG_PROPERTY_ALG_BASE_VALUE_ENABLED:
		bEnabled = pProp->GetCheckValue();		
		bBoolParam = true;
		strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
		strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
		AlgParam.SetAlgBaseValueEnabled(bEnabled);		
		bChanged = true;		
		break;
	case WND_ALG_PROPERTY_ALG_BASE_VALUE_GROUP_ID:
		strValue = pProp->GetValue();		
		nValue = ::_ttoi(strValue)-1;
		AlgParam.SetAlgBaseValueGroupID(nValue);		
		bChanged = true;
		bReBuildWndUI = false;
		break;
	case WND_ALG_PROPERTY_ALG_SAVE_DEFECT_IMAGE:
		//bEnabled = pProp->GetCheckValue();
		bEnabled = (bool)(vtValue.boolVal);

		bBoolParam = true;
		strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
		strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
		AlgParam.SetAlgSaveDefectImageEnabled(bEnabled);		
		bChanged = true;	
		break;
	case WND_ALG_PROPERTY_WND_FOLLOW_MODE:
		strValue = pProp->GetValue();		
		WndFollowMode = AOIDataDefine.FindWndFollowModeByLinkModeText(strValue);
		WndPtr->SetWndFollowMode(WndFollowMode);		
		bChanged = true;
		bReBuildWndUI = false;
		break;	
	case WND_ALG_PROPERTY_WND_CONSTRAIN_MODE:
		strValue = pProp->GetValue();		
		WndConstrainMode = AOIDataDefine.FindWndConstrainModeByText(strValue);
		WndPtr->SetWndConstrainMode(WndConstrainMode);		
		bChanged = true;
		bReBuildWndUI = false;
		break;
	case WND_ALG_PROPERTY_WND_SYNC_MOVE_MODE:
		strValue = pProp->GetValue();		
		WndSyncMoveMode = AOIDataDefine.FindWndSyncMoveModeByText(strValue);
		WndPtr->SetWndSyncMoveMode(WndSyncMoveMode);		
		bChanged = true;
		bReBuildWndUI = false;		
		break;
	case WND_ALG_PROPERTY_WND_CLASS_ID:
		strValue = pProp->GetValue();		
		nValue = ::_ttoi(strValue);		
		WndPtr->SetWndClassID(nValue);		
		bChanged = true;
		bReBuildWndUI = false;		
		break;
	case WND_ALG_PROPERTY_WND_BOX_SHAPE_MODE:
		strValue = pProp->GetValue();		
		BoxShapeMode = AOIDataDefine.FindBoxShapeModeByText(strValue);
		WndPtr->SetWndShapeMode(BoxShapeMode);		
		bChanged = true;
		bReBuildWndUI = false;
		bShow = CAOIBox::CheckBoxShapeUseParam1(BoxShapeMode);
		pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_WND_BOX_SHAPE_PARAM_1);
		if ( NULL != pProp2 )
		{	pProp2->Show(bShow); }

		bShow = CAOIBox::CheckBoxShapeUseParam2(BoxShapeMode);
		pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_WND_BOX_SHAPE_PARAM_2);
		if ( NULL != pProp2 )
		{	pProp2->Show(bShow); }		
		break;
	case WND_ALG_PROPERTY_WND_BOX_SHAPE_PARAM_1:
		strValue = pProp->GetValue();
		dValue = ::_ttof(strValue);		
		WndPtr->SetWndShapeParam(dValue);
		bChanged = true;
		bReBuildWndUI = false;
		break;
	case WND_ALG_PROPERTY_WND_BOX_SHAPE_PARAM_2:
		strValue = pProp->GetValue();
		dValue = ::_ttof(strValue);		
		WndPtr->SetWndShapeParam2(dValue);
		bChanged = true;
		bReBuildWndUI = false;		
		break;
	case WND_ALG_PROPERTY_ADVANCE_END:
		break;

	case WND_ALG_PROPERTY_RESULT_BEGIN:
		break;
	case WND_ALG_PROPERTY_RESULT_END:
		break;
	case WND_ALG_PROPERTY_SCOPE_END:
		break;
	}
	//bReBuildWndUI = false;
	if ( false == bBoolParam )
	{	strNewValue = pProp->GetValue();	}
	Changed.sValueName = strName;
	Changed.sValueOld = strOldValue;
	Changed.sValueNew = strNewValue;	

	Changed.bParamChanged = bChanged;
	Changed.bReBuildWndUI = bReBuildWndUI;
	Changed.bWndRgnChanged = bWndRgnChanged;
	Changed.bFrameIndexChagned = AlgFrameIndexChanged;		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::ExecWndParamListChanged_PatternMatch(CJETPropertyGridProperty *pProp, TWND_PARAM_CHANGED_RESULT &Changed)
{
	if ( NULL == pProp ) { return false; }
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return true; }
	CAOIProject *Project = GetActiveProject();
	if ( NULL == Project ) { return true; }
	CAOIWnd      *WndPtr = (CAOIWnd*)(pProp->GetData());
	if ( NULL == WndPtr ) { return false; }
	CAlgParam       &AlgParam = WndPtr->GetWndAlgParam();	
	CAlgBinaryParam *AlgBinParamPtr = AlgParam.GetAlgImageBinParamPtr();
	CAlgBinaryParam *MskBinParamPtr = AlgParam.GetAlgMaskBinParamPtr();

	const COLORREF  clrOK = m_clrOK;
	const COLORREF  clrNG = m_clrNG;
	const COLORREF  clrUnTest = m_clrUnTest;

	int           LandGroupID=0;		
	bool          bIsPass = true;
	bool          WndEnabled = false;
	bool          WndRgnChanged=false;
	bool          bReBuildWndUI = false;
	bool          AlgFrameIndexChanged=false;
	unsigned int  FrameIndex = 0;
	unsigned int  FrameUniqueID = 0;
	TFrameParam  *FrameParamPtr = NULL;	
	WND_DEFECT_ID     WndDefectID = WND_DEFECT_NONE;	
	WND_LOGIC_TYPE    WndLogicType = WND_LOGIC_NONE;
	WND_FOLLOW_MODE   WndFollowMode=WND_FOLLOW_NONE;
	WND_RGN_LINK_MODE WndRgnLinkMode=WND_RGN_LINK_NONE;
	bool          bChanged = false;	
	bool          bBoolParam = false;
	int           nValue=0, nReading=0;
	bool          bValue=false, bReading=false, bEnabled=true;
	BOOL          BValue=FALSE, BReading=FALSE;
	double        dValue=0.0, dReading=0.0, dValue2=0.0;	
	CString       strValue;
	CString       strName = pProp->GetName();
	CString       strDescr = pProp->GetDescription();
	COleVariant   vtValue = pProp->GetValue();	
	CString       strNewValue(pProp->GetValue());
	CString       strOldValue(pProp->GetOriginalValue());
	DWORD_PTR     dwData = pProp->GetData();	
	CJETPropertyGridProperty *pProp2 = NULL;	
	WND_ALG_PROPERTY_ID ParamID = (WND_ALG_PROPERTY_ID)(pProp->GetID());	
	CJETPropertyGridCtrl &wndPropList = m_wndWndParam;

	switch ( ParamID )
	{
	case WND_ALG_PROPERTY_PATTERN_MATCH_OFFSET_X_USL:
		strValue = pProp->GetValue();
		bEnabled = pProp->GetCheckValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue < 0 )
		{	dValue = 0.0; }
		if ( bEnabled != AlgParam.GetAlgOffsetXEnabled() )
		{
			bBoolParam = true;
			strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
			strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
		}
		AlgParam.SetAlgOffsetXUSL( dValue);
		AlgParam.SetAlgOffsetXLSL(-dValue);
		AlgParam.SetAlgOffsetXEnabled(bEnabled);
		if ( true == AlgParam.GetAlgOffsetXEnabled() )
		{
			bIsPass = CAlgParam::CheckOK_PatternMatchOffsetX(AlgParam);
			if ( false == bIsPass )
			{	pProp->SetReadingTextColor(clrNG); }
			else
			{	pProp->SetReadingTextColor(clrOK); }
		}
		else
		{	pProp->SetReadingTextColor(clrUnTest);	}		
		strValue.Format(_T("%.0f"), dValue);
		pProp->SetValue(strValue);		
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_PATTERN_MATCH_OFFSET_X_LSL:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue > 0 )
		{	dValue = 0.0; }				
		AlgParam.SetAlgOffsetXLSL(dValue);		
		if ( true == AlgParam.GetAlgOffsetXEnabled() )
		{
			bIsPass = CAlgParam::CheckOK_PatternMatchOffsetXLSL(AlgParam);
			if ( false == bIsPass )
			{	pProp->SetReadingTextColor(clrNG); }
			else
			{	pProp->SetReadingTextColor(clrOK); }
		}
		else
		{	pProp->SetReadingTextColor(clrUnTest);	}		
		strValue.Format(_T("%.0f"), dValue);
		pProp->SetValue(strValue);		
		bChanged = true;
		break;	
	case WND_ALG_PROPERTY_PATTERN_MATCH_OFFSET_Y_USL:
		strValue = pProp->GetValue();
		bEnabled = pProp->GetCheckValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue < 0 )
		{	dValue = 0.0; }		
		if ( bEnabled != AlgParam.GetAlgOffsetYEnabled() )
		{
			bBoolParam = true;
			strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
			strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
		}
		AlgParam.SetAlgOffsetYUSL( dValue);
		AlgParam.SetAlgOffsetYLSL(-dValue);
		AlgParam.SetAlgOffsetYEnabled(bEnabled);
		if ( true == AlgParam.GetAlgOffsetYEnabled() )
		{
			bIsPass = CAlgParam::CheckOK_PatternMatchOffsetY(AlgParam);
			if ( false == bIsPass )
			{	pProp->SetReadingTextColor(clrNG); }
			else
			{	pProp->SetReadingTextColor(clrOK); }
		}
		else
		{	pProp->SetReadingTextColor(clrUnTest);	}		
		strValue.Format(_T("%.0f"), dValue);
		pProp->SetValue(strValue);		
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_PATTERN_MATCH_OFFSET_Y_LSL:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue > 0 )
		{	dValue = 0.0; }				
		AlgParam.SetAlgOffsetYLSL(dValue);		
		if ( true == AlgParam.GetAlgOffsetYEnabled() )
		{
			bIsPass = CAlgParam::CheckOK_PatternMatchOffsetYLSL(AlgParam);
			if ( false == bIsPass )
			{	pProp->SetReadingTextColor(clrNG); }
			else
			{	pProp->SetReadingTextColor(clrOK); }
		}
		else
		{	pProp->SetReadingTextColor(clrUnTest);	}		
		strValue.Format(_T("%.0f"), dValue);
		pProp->SetValue(strValue);		
		bChanged = true;
		break;	
	case WND_ALG_PROPERTY_PATTERN_MATCH_OFFSET_L_USL:
		strValue = pProp->GetValue();
		bEnabled = pProp->GetCheckValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue < 0 )
		{	dValue = 0.0; }
		if ( bEnabled != AlgParam.GetAlgOffsetLEnabled() )
		{
			bBoolParam = true;
			strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
			strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
		}
		AlgParam.SetAlgOffsetLUSL( dValue);
		//AlgParam.SetAlgOffsetLLSL(-dValue);
		AlgParam.SetAlgOffsetLEnabled(bEnabled);
		if ( true == AlgParam.GetAlgOffsetLEnabled() )
		{
			bIsPass = CAlgParam::CheckOK_PatternMatchOffsetL(AlgParam);
			if ( false == bIsPass )
			{	pProp->SetReadingTextColor(clrNG); }
			else
			{	pProp->SetReadingTextColor(clrOK); }
		}
		else
		{	pProp->SetReadingTextColor(clrUnTest);	}		
		strValue.Format(_T("%.0f"), dValue);
		pProp->SetValue(strValue);		
		pProp->SetOriginalValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_PATTERN_MATCH_OFFSET_L_LSL:
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue > 0 )
		{	dValue = 0.0; }				
		AlgParam.SetAlgOffsetALSL(dValue);		
		if ( true == AlgParam.GetAlgOffsetLEnabled() )
		{
			bIsPass = CAlgParam::CheckOK_PatternMatchOffsetLLSL(AlgParam);
			if ( false == bIsPass )
			{	pProp->SetReadingTextColor(clrNG); }
			else
			{	pProp->SetReadingTextColor(clrOK); }
		}
		else
		{	pProp->SetReadingTextColor(clrUnTest);	}		
		strValue.Format(_T("%.0f"), dValue);
		pProp->SetValue(strValue);
		pProp->SetOriginalValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_PATTERN_MATCH_OFFSET_A_USL:
		strValue = pProp->GetValue();
		bEnabled = pProp->GetCheckValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue < 0 )
		{	dValue = 0.0; }
		if ( bEnabled != AlgParam.GetAlgOffsetAEnabled() )
		{
			bBoolParam = true;
			strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
			strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
		}
		AlgParam.SetAlgOffsetAUSL( dValue);
		AlgParam.SetAlgOffsetALSL(-dValue);
		AlgParam.SetAlgOffsetAEnabled(bEnabled);
		if ( true == AlgParam.GetAlgOffsetAEnabled() )
		{
			bIsPass = CAlgParam::CheckOK_PatternMatchOffsetA(AlgParam);
			if ( false == bIsPass )
			{	pProp->SetReadingTextColor(clrNG); }
			else
			{	pProp->SetReadingTextColor(clrOK); }
		}
		else
		{	pProp->SetReadingTextColor(clrUnTest);	}		
		strValue.Format(_T("%.2f"), dValue);
		pProp->SetValue(strValue);		
		pProp->SetOriginalValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_PATTERN_MATCH_OFFSET_A_LSL:
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue > 0 )
		{	dValue = 0.0; }				
		AlgParam.SetAlgOffsetALSL(dValue);		
		if ( true == AlgParam.GetAlgOffsetAEnabled() )
		{
			bIsPass = CAlgParam::CheckOK_PatternMatchOffsetALSL(AlgParam);
			if ( false == bIsPass )
			{	pProp->SetReadingTextColor(clrNG); }
			else
			{	pProp->SetReadingTextColor(clrOK); }
		}
		else
		{	pProp->SetReadingTextColor(clrUnTest);	}		
		strValue.Format(_T("%.2f"), dValue);
		pProp->SetValue(strValue);
		pProp->SetOriginalValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_PATTERN_MATCH_SKEW_USL:
		strValue = pProp->GetValue();
		bEnabled = pProp->GetCheckValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue < 0 )
		{	dValue = 0.0; }		
		if ( bEnabled != AlgParam.GetAlgSkewEnabled() )
		{
			bBoolParam = true;
			strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
			strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
		}
		AlgParam.SetAlgSkewUSL( dValue);
		AlgParam.SetAlgSkewLSL(-dValue);		
		AlgParam.SetAlgSkewEnabled(bEnabled);
		if ( true == AlgParam.GetAlgSkewEnabled() )
		{
			bIsPass = CAlgParam::CheckOK_PatternMatchSkewAngle(AlgParam);
			if ( false == bIsPass )
			{	pProp->SetReadingTextColor(clrNG); }
			else
			{	pProp->SetReadingTextColor(clrOK); }
		}
		else
		{	pProp->SetReadingTextColor(clrUnTest);	}		
		strValue.Format(_T("%.2f"), dValue);
		pProp->SetValue(strValue);		
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_PATTERN_MATCH_SKEW_LSL:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue > 0 )
		{	dValue = 0.0; }	
		AlgParam.SetAlgSkewLSL(dValue);
		if ( true == AlgParam.GetAlgSkewEnabled() )
		{
			bIsPass = CAlgParam::CheckOK_PatternMatchSkewAngleLSL(AlgParam);
			if ( false == bIsPass )
			{	pProp->SetReadingTextColor(clrNG); }
			else
			{	pProp->SetReadingTextColor(clrOK); }
		}
		else
		{	pProp->SetReadingTextColor(clrUnTest);	}		
		strValue.Format(_T("%.2f"), dValue);
		pProp->SetValue(strValue);		
		bChanged = true;
		break;	
	case WND_ALG_PROPERTY_PATTERN_MATCH_SCALE_NODE:
		break;
	case WND_ALG_PROPERTY_PATTERN_MATCH_SCALE_ENABLE:
		strValue = pProp->GetValue();
		bEnabled = pProp->GetCheckValue();		
		bBoolParam = true;
		strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
		strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
		AlgParam.SetAlgScaleEnabled(bEnabled);
		/*
		dReading = AlgParam.GetAlgScaleMaxReading();
		if ( true == AlgParam.GetAlgScaleEnabled() )
		{
			if ( dReading > dValue )
			{	pProp->SetReadingTextColor(clrNG); }
			else
			{	pProp->SetReadingTextColor(clrOK); }
		}
		else
		{	pProp->SetReadingTextColor(clrUnTest);	}		
		strValue.Format(_T("%.2f"), dValue);
		pProp->SetValue(strValue);		
		const double dSXReading = AlgParam.GetAlgScaleXReading();	
		const double dSYReading = AlgParam.GetAlgScaleYReading();	
		*/
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_PATTERN_MATCH_SCALE_USL:
		strValue = pProp->GetValue();		
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue < 0 )
		{	dValue = 0.0; }		
		AlgParam.SetAlgScaleUSL(dValue);		
		if ( true == AlgParam.GetAlgScaleEnabled() )
		{
			bIsPass = CAlgParam::CheckOK_PatternMatchScaleUSL(AlgParam);
			if ( false == bIsPass )
			{	pProp->SetReadingTextColor(clrNG); }
			else
			{	pProp->SetReadingTextColor(clrOK); }
		}
		else
		{	pProp->SetReadingTextColor(clrUnTest);	}		
		strValue.Format(_T("%.0f"), dValue);
		pProp->SetValue(strValue);		
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_PATTERN_MATCH_SCALE_LSL:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue < 0 )
		{	dValue = 0.0; }	
		AlgParam.SetAlgScaleLSL(dValue);		
		if ( true == AlgParam.GetAlgScaleEnabled() )
		{
			bIsPass = CAlgParam::CheckOK_PatternMatchScaleLSL(AlgParam);
			if ( false == bIsPass )
			{	pProp->SetReadingTextColor(clrNG); }
			else
			{	pProp->SetReadingTextColor(clrOK); }
		}
		else
		{	pProp->SetReadingTextColor(clrUnTest);	}		
		strValue.Format(_T("%.0f"), dValue);
		pProp->SetValue(strValue);		
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_PATTERN_MATCH_SIMILARITY_USL:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue > 100 ) { dValue = 100.0; }
		else if ( dValue < 0 )	{	dValue = 0.0; }
		if ( dValue < AlgParam.GetAlgPatternSimilarityLSL() ) 
		{	dValue = AlgParam.GetAlgPatternSimilarityLSL();	}
		AlgParam.SetAlgPatternSimilarityUSL(dValue);			
		bIsPass = CAlgParam::CheckOK_PatternMatchScoreUSL(AlgParam);
		if ( false == bIsPass )
		{	pProp->SetReadingTextColor(clrNG); }
		else
		{	pProp->SetReadingTextColor(clrOK); }
		strValue.Format(_T("%.0f"), dValue);
		pProp->SetValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_PATTERN_MATCH_SIMILARITY_LSL:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue > 100 ) { dValue = 100.0; }
		else if ( dValue < 0 )	{	dValue = 0.0; }
		if ( dValue > AlgParam.GetAlgPatternSimilarityUSL() ) 
		{	dValue = AlgParam.GetAlgPatternSimilarityUSL();	}
		AlgParam.SetAlgPatternSimilarityLSL(dValue);		
		bIsPass = CAlgParam::CheckOK_PatternMatchScoreLSL(AlgParam);
		if ( false == bIsPass )
		{	pProp->SetReadingTextColor(clrNG); }
		else
		{	pProp->SetReadingTextColor(clrOK); }
		strValue.Format(_T("%.0f"), dValue);
		pProp->SetValue(strValue);
		bChanged = true;
		break;		
	case WND_ALG_PROPERTY_PATTERN_MATCH_ANGLE_EXPAND:
		strValue = pProp->GetValue();
		dValue = JetAPI::StrToDbl(strValue);
		if ( dValue >= 1.0 )
		{
			bChanged = true;			
			AlgParam.SetAlgPatternAngleExpand(dValue); 			
		}
		break;	
	case WND_ALG_PROPERTY_PATTERN_MATCH_SCALE_EXPAND:
		strValue = pProp->GetValue();
		nValue = ::_ttoi(strValue);
		if ( nValue >= 0 && nValue < 99 )
		{
			AlgParam.SetAlgPatternScaleExpand(nValue); 		
			bChanged = true;
		}
		break;	
	case WND_ALG_PROPERTY_PATTERN_MATCH_SCALE_ISOTROPIC:
		//bEnabled = pProp->GetCheckValue();
		//AlgParam.SetAlgPatternScaleIsotropic(bEnabled);
		bEnabled = (bool)(vtValue.boolVal);
		bBoolParam = true;		
		strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
		strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
		if ( FALSE == vtValue.boolVal )
		{	AlgParam.SetAlgPatternScaleIsotropic(false); }
		else
		{	AlgParam.SetAlgPatternScaleIsotropic(true); }		
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_PATTERN_MATCH_MIN_REDUCE_AREA:
		strValue = pProp->GetValue();
		nValue = ::_ttoi(strValue);		
		AlgParam.SetAlgPatternMinReducedArea(nValue);		
		bChanged = true;		
		break;
	case WND_ALG_PROPERTY_PATTERN_MATCH_FINAL_REDUCTION:
		strValue = pProp->GetValue();
		nValue = ::_ttoi(strValue);		
		AlgParam.SetAlgPatternFinalReduction(nValue);		
		bChanged = true;		
		break;
	case WND_ALG_PROPERTY_PATTERN_MATCH_ADVANCED_LEARNING:
		bEnabled = (bool)(vtValue.boolVal);		
		bBoolParam = true;
		strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
		strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
		if ( FALSE == vtValue.boolVal )
		{	AlgParam.SetAlgPatternAdvancedLearning(false); }
		else
		{	AlgParam.SetAlgPatternAdvancedLearning(true); }		
		bChanged = true;
		break;
	//case 
		
	}	

	if ( false == bBoolParam )
	{	strNewValue = pProp->GetValue();	}
	Changed.sValueName = strName;
	Changed.sValueOld = strOldValue;
	Changed.sValueNew = strNewValue;	

	Changed.bParamChanged = bChanged;
	Changed.bReBuildWndUI = bReBuildWndUI;
	Changed.bWndRgnChanged = WndRgnChanged;	
	Changed.bFrameIndexChagned = AlgFrameIndexChanged;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::ExecWndParamListChanged_PixelCompare(CJETPropertyGridProperty *pProp, TWND_PARAM_CHANGED_RESULT &Changed)
{
	if ( NULL == pProp ) { return false; }
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return true; }
	CAOIProject *Project = GetActiveProject();
	if ( NULL == Project ) { return true; }
	CAOIWnd      *WndPtr = (CAOIWnd*)(pProp->GetData());
	if ( NULL == WndPtr ) { return false; }
	CAlgParam   &AlgParam = WndPtr->GetWndAlgParam();		
	TALG_PARAM_PIXEL_COMPARE   &pcParam = AlgParam.GetAlgParamPixelCompare();
	
	const COLORREF  clrOK = m_clrOK;
	const COLORREF  clrNG = m_clrNG;
	const COLORREF  clrUnTest = m_clrUnTest;

	bool          bIsPass = true;
	bool          bChanged = false;	
	bool          bBoolParam = false;
	int           nValue=0, nReading=0;
	bool          bValue=false, bReading=false, bEnabled=true;
	BOOL          BValue=FALSE, BReading=FALSE;
	double        dValue=0.0, dReading=0.0;	
	CString       strValue;
	CString       strName = pProp->GetName();
	CString       strDescr = pProp->GetDescription();
	COleVariant   vtValue = pProp->GetValue();
	CString       strNewValue(pProp->GetValue());
	CString       strOldValue(pProp->GetOriginalValue());
	DWORD_PTR     dwData = pProp->GetData();	
	BOOL          bClickBtn = pProp->GetClickUserBtn();
	CJETPropertyGridProperty *pProp2 = NULL;	
	
	WND_ALG_PROPERTY_ID ParamID = (WND_ALG_PROPERTY_ID)(pProp->GetID());	
	CJETPropertyGridCtrl &wndPropList = m_wndWndParam;	
	switch ( ParamID )
	{			
	case WND_ALG_PROPERTY_PIXEL_COMPARE_CELL_EXT_SIZE:
		strValue = pProp->GetValue();
		nValue = ::_ttoi(strValue);	
		if ( nValue < 0 ) 
		{	nValue = 0; }
		pcParam.pcCellExtSize = nValue;
		strValue.Format(_T("%d"), nValue);
		pProp->SetValue(strValue);
		pProp->SetOriginalValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_PIXEL_COMPARE_IMG_BLUR_SIZE:
		strValue = pProp->GetValue();
		nValue = ::_ttoi(strValue);	
		if ( nValue < 0 ) 
		{	nValue = 0; }
		pcParam.pcImgBlurSize = nValue;
		strValue.Format(_T("%d"), nValue);
		pProp->SetValue(strValue);
		pProp->SetOriginalValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_PIXEL_COMPARE_PAT_SCOPE:		
		break;	
	case WND_ALG_PROPERTY_PIXEL_COMPARE_PAT_LIGHT_LEVEL:
		strValue = pProp->GetValue();
		nValue = ::_ttoi(strValue);	
		if ( nValue < 0 ) 
		{	nValue = 0; }
		if ( nValue < pcParam.pcPatDarkLevel )
		{	nValue = pcParam.pcPatDarkLevel; }
		pcParam.pcPatLightLevel = nValue;
		strValue.Format(_T("%d"), nValue);		
		pProp->SetValue(strValue);
		pProp->SetOriginalValue(strValue);		
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_PIXEL_COMPARE_PAT_DARK_LEVEL:
		strValue = pProp->GetValue();
		nValue = ::_ttoi(strValue);	
		if ( nValue < 0 ) 
		{	nValue = 0; }
		if ( nValue > pcParam.pcPatLightLevel )
		{	nValue = pcParam.pcPatLightLevel; }
		pcParam.pcPatDarkLevel = nValue;
		strValue.Format(_T("%d"), nValue);
		pProp->SetValue(strValue);
		pProp->SetOriginalValue(strValue);		
		bChanged = true;
		break;	
	case WND_ALG_PROPERTY_PIXEL_COMPARE_PAT_ERODE_SIZE:
		strValue = pProp->GetValue();
		nValue = ::_ttoi(strValue);	
		if ( nValue < 0 ) 
		{	nValue = 0; }		
		pcParam.pcPatErodeSize = nValue;
		strValue.Format(_T("%d"), nValue);
		pProp->SetValue(strValue);
		pProp->SetOriginalValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_PIXEL_COMPARE_PAT_PURE_COLOR:
		bEnabled = (bool)(vtValue.boolVal);
		bBoolParam = true;
		pcParam.pcPatPureColor = bEnabled;
		strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
		strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);		
		bChanged = true;
		break;

	case WND_ALG_PROPERTY_PIXEL_COMPARE_CMP_SCOPE:				
		break;
	case WND_ALG_PROPERTY_PIXEL_COMPARE_CMP_LIGHT_GAIN:
		strValue = pProp->GetValue();
		dValue = ::_ttof(strValue);	
		if ( dValue < 0 ) 
		{	dValue = 0; }
		pcParam.pcCmpLightGain = dValue;
		strValue.Format(_T("%.2f"), dValue);
		pProp->SetValue(strValue);
		pProp->SetOriginalValue(strValue);		
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_PIXEL_COMPARE_CMP_DARK_GAIN:
		strValue = pProp->GetValue();
		dValue = ::_ttof(strValue);	
		if ( dValue < 0 ) 
		{	dValue = 0; }
		pcParam.pcCmpDarkGain = dValue;
		strValue.Format(_T("%.2f"), dValue);
		pProp->SetValue(strValue);
		pProp->SetOriginalValue(strValue);		
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_PIXEL_COMPARE_CMP_TOLERANCE:
		strValue = pProp->GetValue();
		nValue = ::_ttoi(strValue);	
		if ( nValue < 0 ) 
		{	nValue = 0; }
		pcParam.pcCmpTolerance = nValue;
		strValue.Format(_T("%d"), nValue);
		pProp->SetValue(strValue);
		pProp->SetOriginalValue(strValue);		
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_PIXEL_COMPARE_CMP_GAUSSIAN_SIZE:
		strValue = pProp->GetValue();
		nValue = ::_ttoi(strValue);	
		if ( nValue < 0 ) 
		{	nValue = 0; }
		pcParam.pcCmpGaussianSize = nValue;
		strValue.Format(_T("%d"), nValue);
		pProp->SetValue(strValue);
		pProp->SetOriginalValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_PIXEL_COMPARE_CMP_OPEN_SIZE:
		strValue = pProp->GetValue();
		nValue = ::_ttoi(strValue);	
		if ( nValue < 0 ) 
		{	nValue = 0; }
		pcParam.pcCmpOpenSize = nValue;
		strValue.Format(_T("%d"), nValue);
		pProp->SetValue(strValue);
		pProp->SetOriginalValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_PIXEL_COMPARE_CMP_CLOSE_SIZE:
		strValue = pProp->GetValue();
		nValue = ::_ttoi(strValue);	
		if ( nValue < 0 ) 
		{	nValue = 0; }
		pcParam.pcCmpCloseSize = nValue;
		strValue.Format(_T("%d"), nValue);
		pProp->SetValue(strValue);
		pProp->SetOriginalValue(strValue);
		bChanged = true;
		break;

	case WND_ALG_PROPERTY_PIXEL_COMPARE_BLOB_MIN_SIZE_D:
		strValue = pProp->GetValue();
		dValue = ::_ttof(strValue);	
		if ( dValue < 0 ) 
		{	dValue = 0; }
		pcParam.pcBlobMinSizeD = dValue;
		strValue.Format(_T("%.0f"), dValue);
		pProp->SetValue(strValue);
		pProp->SetOriginalValue(strValue);
		bChanged = true;
		break;	
	case WND_ALG_PROPERTY_PIXEL_COMPARE_BLOB_COUNT_USL:
		strValue = pProp->GetValue();
		nValue = ::_ttoi(strValue);
		if ( nValue < 0 )	{	nValue = 0; }
		if ( nValue < pcParam.pcBlobCountLSL ) 
		{	nValue = pcParam.pcBlobCountLSL;	}
		pcParam.pcBlobCountUSL = nValue;
		bIsPass = CAlgParam::CheckOK_PixelCompareUSL(pcParam);
		if ( false == bIsPass )
		{	pProp->SetReadingTextColor(clrNG); }
		else
		{	pProp->SetReadingTextColor(clrOK); }
		strValue.Format(_T("%d"), nValue);
		pProp->SetValue(strValue);
		pProp->SetOriginalValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_PIXEL_COMPARE_BLOB_COUNT_LSL:
		strValue = pProp->GetValue();
		nValue = ::_ttoi(strValue);		
		if ( nValue < 0 )	{	nValue = 0.0; }
		if ( nValue > pcParam.pcBlobCountUSL ) 
		{	nValue = pcParam.pcBlobCountUSL;	}
		pcParam.pcBlobCountLSL = nValue;				
		bIsPass = CAlgParam::CheckOK_PixelCompareLSL(pcParam);
		if ( false == bIsPass )
		{	pProp->SetReadingTextColor(clrNG); }
		else
		{	pProp->SetReadingTextColor(clrOK); }
		strValue.Format(_T("%d"), nValue);
		pProp->SetValue(strValue);
		pProp->SetOriginalValue(strValue);
		bChanged = true;
		break;	
	}

	if ( false == bBoolParam  )
	{	strNewValue = pProp->GetValue();	}
	Changed.sValueName = strName;
	Changed.sValueOld = strOldValue;
	Changed.sValueNew = strNewValue;	

	Changed.bParamChanged = bChanged;		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::ExecWndParamListChanged_IPC(CJETPropertyGridProperty *pProp, TWND_PARAM_CHANGED_RESULT &Changed)
{
	if (NULL == pProp) { return false; }
	CAOIModel *ModelPtr = GetModelPtr();
	if (NULL == ModelPtr) { return true; }
	CAOIProject *Project = GetActiveProject();
	if (NULL == Project) { return true; }
	CAOIWnd      *WndPtr = (CAOIWnd*)(pProp->GetData());
	if (NULL == WndPtr) { return false; }

	CAlgParam   &AlgParam = WndPtr->GetWndAlgParam();
	tagALG_PARAM_IPC_PRODUCT  &Param = AlgParam.GetAlgParamIPC();

	bool          bChanged = false;
	bool          bBoolParam = false;
	int           nValue = 0;
	bool          bEnabled = true;
	CString       strValue;
	CString       strName = pProp->GetName();
	CString       strNewValue(pProp->GetValue());
	CString       strOldValue(pProp->GetOriginalValue());
	WND_ALG_PROPERTY_ID ParamID = (WND_ALG_PROPERTY_ID)(pProp->GetID());

	switch (ParamID)
	{
	case WND_ALG_PROPERTY_IPC_NODE:
		break;
	case WND_ALG_PROPERTY_IPC_ENABLED:
		bEnabled = pProp->GetCheckValue();
		bBoolParam = true;
		strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
		strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
		Param.ipcEnabled = bEnabled;
		//pProp->Expand(bEnabled);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_IPC_XVALUE:
		strValue = pProp->GetValue();
		nValue = ::_ttoi(strValue);
		if (nValue > 100) { nValue = 100; }
		if (nValue < 0) { nValue = 0; }
		strValue.Format(_T("%d"), nValue);
		Param.ipcX = nValue;
		pProp->SetValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_IPC_YVALUE:
		strValue = pProp->GetValue();
		nValue = ::_ttoi(strValue);
		if (nValue > 100) { nValue = 100; }
		if (nValue < 0) { nValue = 0; }
		strValue.Format(_T("%d"), nValue);
		Param.ipcY = nValue;
		pProp->SetValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_IPC_CONTINUOUS_SET:
		strValue = pProp->GetValue();
		nValue = ::_ttoi(strValue);
		if (nValue < 0) { nValue = 0; }
		if (nValue > 100) { nValue = 100; }
		Param.ipcContinuousPixel = nValue;
		strValue.Format(_T("%d"), nValue);
		pProp->SetValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_IPC_WIDTH_GAP:
		strValue = pProp->GetValue();
		nValue = ::_ttoi(strValue);
		//if (nValue < 1) { nValue = 1; }
		Param.ipcGapPixel = nValue;
		strValue.Format(_T("%d"), nValue);
		pProp->SetValue(strValue);
		bChanged = true;
		break;
		//case WND_ALG_PROPERTY_BLOB_WIDTH_SIZE:	// 預備 沒再用
		//	strValue = pProp->GetValue();
		//	nValue = ::_ttoi(strValue);
		//	if (nValue < 1) { nValue = 1; }
		//	blobParam.bcCountLSL = nValue;
		//	strValue.Format(_T("%d"), nValue);
		//	pProp->SetValue(strValue);
		//	bChanged = true;
		//	break;
	case WND_ALG_PROPERTY_IPC_WIDTH_RATIO:
		strValue = pProp->GetValue();
		nValue = ::_ttoi(strValue);
		if (nValue < 1) { nValue = 1; }
		if (nValue > 100) { nValue = 100; }
		strValue.Format(_T("%d"), nValue);
		Param.ipcWidthRatio = nValue;
		pProp->SetValue(strValue);
		bChanged = true;
		break;
	}

	if (false == bBoolParam)
	{
		strNewValue = pProp->GetValue();
	}
	Changed.sValueName = strName;
	Changed.sValueOld = strOldValue;
	Changed.sValueNew = strNewValue;

	Changed.bParamChanged = bChanged;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::ExecWndParamListChanged_ResinHeight(CJETPropertyGridProperty *pProp, TWND_PARAM_CHANGED_RESULT &Changed)
{
	if (NULL == pProp) { return false; }
	CAOIModel *ModelPtr = GetModelPtr();
	if (NULL == ModelPtr) { return true; }
	CAOIProject *Project = GetActiveProject();
	if (NULL == Project) { return true; }
	CAOIWnd      *WndPtr = (CAOIWnd*)(pProp->GetData());
	if (NULL == WndPtr) { return false; }

	CAlgParam   &AlgParam = WndPtr->GetWndAlgParam();
	TALG_PARAM_RESIN_HEIGHT &Param = AlgParam.GetAlgParamResinHeight();

	int nValue;
	bool bChanged = false;
	bool bEnabled = true;
	bool bBoolParam = false;
	CString strValue;
	WND_ALG_PROPERTY_ID ParamID = (WND_ALG_PROPERTY_ID)(pProp->GetID());
	CString       strName = pProp->GetName();
	CString       strNewValue(pProp->GetValue());
	CString       strOldValue(pProp->GetOriginalValue());

	switch (ParamID)
	{
	case WND_ALG_PROPERTY_RESIN_ENABLED:
		bEnabled = pProp->GetCheckValue();
		bBoolParam = true;
		strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
		strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
		Param.enabled = bEnabled;
		//pProp->Expand(bEnabled);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_RESIN_NG_CHECK:
		bEnabled = pProp->GetCheckValue();
		bBoolParam = true;
		strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
		strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
		Param.enableDoubleCheck = bEnabled;
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_RESIN_TYPE:
		strValue = pProp->GetValue();
		Param.nInspectionType = AOIDataDefine.FindAlgHeightDetectionTypeByText(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_RESIN_OUTPUT_TYPE:
		strValue = pProp->GetValue();
		Param.nOutputType = AOIDataDefine.FindAlgHeightDetectionOutputTypeByText(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_RESIN_DIRECTION:
		strValue = pProp->GetValue();
		Param.nDirection = AOIDataDefine.FindAlgHeightDetectionDirectionByText(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_RESIN_RANGE:
		strValue = pProp->GetValue();
		nValue = ::_ttoi(strValue);
		if (nValue < 0) { nValue = 0; }
		Param.nMeasureRange = nValue;
		strValue.Format(_T("%d"), nValue);
		pProp->SetValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_RESIN_STEP_Z:
		strValue = pProp->GetValue();
		nValue = ::_ttoi(strValue);
		if (nValue < 0) { nValue = 0; }
		Param.nStepZ = nValue;
		strValue.Format(_T("%d"), nValue);
		pProp->SetValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_RESIN_SHIFT:
		strValue = pProp->GetValue();
		nValue = ::_ttoi(strValue);
		if (nValue < 0) { nValue = 0; }
		Param.nShift_Tin = nValue;
		strValue.Format(_T("%d"), nValue);
		pProp->SetValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_RESIN_THRESHOLD_WIDTH:
		strValue = pProp->GetValue();
		nValue = ::_ttoi(strValue);
		if (nValue < 0) { nValue = 0; }
		Param.nThresholdZ_Width = nValue;
		strValue.Format(_T("%d"), nValue);
		pProp->SetValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_RESIN_THRESHOLD_HEIGHT:
		strValue = pProp->GetValue();
		nValue = ::_ttoi(strValue);
		if (nValue < 0) { nValue = 0; }
		Param.nThresholdZ_Height = nValue;
		strValue.Format(_T("%d"), nValue);
		pProp->SetValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_RESIN_HEIGHT_MEASURE_MODE:
		strValue = pProp->GetValue();
		Param.nPartHeightMeasureMode = AOIDataDefine.FindAlgHeightDetectionMeasureModeByText(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_RESIN_HEIGHT_VALUE:
		strValue = pProp->GetValue();
		nValue = ::_ttoi(strValue);
		if (Param.nPartHeightMeasureMode == 0)
		{
			if (nValue > 100) { nValue = 100; }
			if (nValue < 0) { nValue = 0; }
		}
		else
			if (nValue < 0) { nValue = 0; }
		Param.nPartHeightThreshold = nValue;
		strValue.Format(_T("%d"), nValue);
		pProp->SetValue(strValue);
		bChanged = true;
		break;
	}

	if (false == bBoolParam)
	{
		strNewValue = pProp->GetValue();
	}
	Changed.sValueName = strName;
	Changed.sValueOld = strOldValue;
	Changed.sValueNew = strNewValue;
	Changed.bReBuildWndUI = false;
	Changed.bParamChanged = bChanged;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::ExecWndParamListChanged_WireWidth(CJETPropertyGridProperty *pProp, TWND_PARAM_CHANGED_RESULT &Changed)
{
	if (NULL == pProp) { return false; }
	CAOIModel *ModelPtr = GetModelPtr();
	if (NULL == ModelPtr) { return true; }
	CAOIProject *Project = GetActiveProject();
	if (NULL == Project) { return true; }
	CAOIWnd      *WndPtr = (CAOIWnd*)(pProp->GetData());
	if (NULL == WndPtr) { return false; }

	CAlgParam   &AlgParam = WndPtr->GetWndAlgParam();
	TALG_PARAM_WIRE_WIDTH &wireWidthParm = AlgParam.GetAlgParamWireWidth();

	int nValue;
	bool bChanged = false;
	bool bEnabled = true;
	bool bBoolParam = false;
	CString strValue;
	WND_ALG_PROPERTY_ID ParamID = (WND_ALG_PROPERTY_ID)(pProp->GetID());
	CString       strName = pProp->GetName();
	CString       strNewValue(pProp->GetValue());
	CString       strOldValue(pProp->GetOriginalValue());

	switch (ParamID)
	{
	case WND_ALG_PROPERTY_WIRE_WIDTH_FILTER_ENABLED:
		bEnabled = pProp->GetCheckValue();
		bBoolParam = true;
		strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
		strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
		wireWidthParm.bFilter = bEnabled;
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_WIRE_WIDTH_FILTER_SIZE:
		strValue = pProp->GetValue();
		nValue = ::_ttoi(strValue);
		if (nValue < 0) { nValue = 0; }
		wireWidthParm.nFilterSize = nValue;
		strValue.Format(_T("%d"), nValue);
		pProp->SetValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_WIRE_WIDTH_EDGE_LOW_THRESHOLD:
		strValue = pProp->GetValue();
		nValue = ::_ttoi(strValue);
		if (nValue < 0) { nValue = 0; }
		wireWidthParm.nLowThres = nValue;
		strValue.Format(_T("%d"), nValue);
		pProp->SetValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_WIRE_WIDTH_EDGE_HIGHT_THRESHOLD:
		strValue = pProp->GetValue();
		nValue = ::_ttoi(strValue);
		if (nValue < 0) { nValue = 0; }
		wireWidthParm.nHeightThres = nValue;
		strValue.Format(_T("%d"), nValue);
		pProp->SetValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_WIRE_WIDTH_VALUE_USL:
		strValue = pProp->GetValue();
		nValue = ::_ttoi(strValue);
		if (nValue < 0) { nValue = 0; }
		wireWidthParm.widthUSL = nValue;
		strValue.Format(_T("%.1f"), nValue);
		pProp->SetValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_WIRE_WIDTH_VALUE_LSL:
		strValue = pProp->GetValue();
		nValue = ::_ttoi(strValue);
		if (nValue < 0) { nValue = 0; }
		wireWidthParm.widthLSL = nValue;
		strValue.Format(_T("%.1f"), nValue);
		pProp->SetValue(strValue);
		bChanged = true;
		break;
	}

	if (false == bBoolParam)
	{
		strNewValue = pProp->GetValue();
	}
	Changed.sValueName = strName;
	Changed.sValueOld = strOldValue;
	Changed.sValueNew = strNewValue;
	Changed.bReBuildWndUI = false;
	Changed.bParamChanged = bChanged;
	return true;
}
//-------------------------------------------------------------------------------------//
unsigned int CEditWndView::DecodeFrameUniqueID(LPCTSTR ItemText)//解出畫面的唯一碼
{
	CString strID;
	CString strText = ItemText;
	unsigned int FrameUniqueID = FRAME_UNIQUE_ID_NULL;
	
	strText.MakeUpper();
	int i=0;
	const int strLen = strText.GetLength();
	for ( i=0; i<strLen; i++ )
	{
		if ( strText[i] == _T('[') )
		{	strID = _T("");	}
		else if ( strText[i] == _T(']') )
		{	break;	}
		else
		{	strID.AppendChar(strText[i]);	}
	}

	if ( strID.GetLength() == 0 ) 
	{	return FRAME_UNIQUE_ID_NULL; }

	FrameUniqueID = (unsigned int)::_ttoi(strID);
	return FrameUniqueID;
}
//-------------------------------------------------------------------------------------//
void CEditWndView::LockUIWnd(bool bLock)
{
	UINT CtrlID = 0;
	BOOL bEnable = true;	
	DRAW_MODEL_MODE DrawModelMode = AOIDataCollect.GetDrawModelMode();	
	if ( true == bLock )
	{	bEnable = FALSE; }
	else
	{	bEnable = TRUE;	}
	if ( DRAW_MODEL_RESULT == DrawModelMode )	
	{	bEnable = FALSE;	}

	m_wndClassCombo.EnableWindow(bEnable);
	m_wndToolBar.EnableWindow(bEnable);
	m_wndGroupList.EnableWindow(bEnable);
	m_wndWndList.EnableWindow(bEnable);
	m_wndWndParam.EnableWindow(bEnable);
	m_wndSplitter.EnableWindow(bEnable);
	return;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::GetLockUIWnd() const//取得是否鎖住UIWnd
{
	bool IsOnInspection = AOIDataCollect.GetIsOnInspection();
	if ( true == IsOnInspection )
	{	return true; }
	return false;

	if ( AOIDataCollect.GetIsLockUIWnd() == true ) 
	{	return true; }
	return false;

	DRAW_MODEL_MODE DrawModelMode = AOIDataCollect.GetDrawModelMode();
	if ( DRAW_MODEL_RESULT == DrawModelMode )
	{	return true; }
	return false;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::GetEnableUIWnd() const//取得是否啟用UI視窗
{
	if ( AOIDataCollect.GetIsLockUIWnd() == true ) 
	{	return false; }	

	return true;
	DRAW_MODEL_MODE DrawModelMode = AOIDataCollect.GetDrawModelMode();
	if ( DRAW_MODEL_RESULT == DrawModelMode )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::UpdateTextColor()//更新文字顏色
{
	int R=0, G=0, B=0;
	int Divide=2;
	COLORREF Color=0;
	const TSystemParameter &SystemParam = AOIDataCollect.GetSystemParameter();
	m_clrOK = JetAPI::DivideColor(SystemParam.m_InspectedResultOKColor, Divide);
	m_clrNG = JetAPI::DivideColor(SystemParam.m_InspectedResultNGColor, Divide);
	m_clrSkip = JetAPI::DivideColor(SystemParam.m_InspectedResultSkipColor, Divide);
	m_clrBypass = JetAPI::DivideColor(SystemParam.m_InspectedResultBypassColor, Divide);
	m_clrUnTest = SystemParam.m_InspectedResultUnTestColor;
	m_clrWarnning = SystemParam.m_InspectedResultWarningColor;
	m_clrException = SystemParam.m_InspectedResultExceptionColor;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::ExecWndParamListChanged_WndRoi_Add(CAOIWnd *WndPtr)
{
	if ( NULL == WndPtr ) { return false; }
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return false; }

	CAOIWndRoi    *WndRoiPtr = NULL;
	if ( ModelPtr->AddModelWndRoiWnd(WndPtr) == false )
	{	return false; }

	WndRoiPtr = WndPtr->GetWndRoiWndActived();
	if ( NULL == WndRoiPtr ) { return false; }
	LogOperCtrl.SaveLogWndRoiSelectedCreate(WndPtr);

	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	if ( true == WndRoiPtr->GetWndRoiBinaryParamEnabled() ) 
	{	AlgParam.SetAlgBinParamActived(BIN_PARAM_BELONG_TO_ROI_IMAGE);	}
	else
	{	AlgParam.SetAlgBinParamActived(BIN_PARAM_BELONG_TO_ALG_IMAGE);	}	
	PostMessageToMainFrameWnd(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::ExecWndParamListChanged_WndRoi_Delete(CAOIWnd *WndPtr)
{
	if ( NULL == WndPtr ) { return false; }
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return false; }

	CString str;
	str = _T("Do you want to delete selected roi wnd?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO )
	{	return false; }	

	LogOperCtrl.SaveLogWndRoiSelectedDelete(WndPtr);
	ModelPtr->DeleteModelWndRoiWnd(WndPtr);

	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	AlgParam.SetAlgBinParamActived(BIN_PARAM_BELONG_TO_ALG_IMAGE);
	PostMessageToMainFrameWnd(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);			
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::ExecWndParamListChanged_WndRoi_ClearList(CAOIWnd *WndPtr)
{
	if ( NULL == WndPtr ) { return false; }
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return false; }

	CString str;
	str = _T("Do you want to clear all roi wnd?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO )
	{	return false; }

	LogOperCtrl.SaveLogWndRoiSelectedClearAll(WndPtr);
	ModelPtr->ClearModelWndRoiWndList(WndPtr);

	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	AlgParam.SetAlgBinParamActived(BIN_PARAM_BELONG_TO_ALG_IMAGE);
	PostMessageToMainFrameWnd(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::ExecWndParamListChanged_WndRoi_Add_Auto(CAOIWnd *WndPtr)
{
	if ( NULL == WndPtr ) { return false; }
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return false; }

	bool bResult = false;
	ALG_TYPE AlgType = WndPtr->GetWndAlgType();
	if ( ALG_CHAR_VERIFY == AlgType )
	{	bResult = ExecWndParamListChanged_WndRoi_Add_Auto_Text(WndPtr);	}

	if ( ALG_COLOR_CODE == AlgType )
	{	bResult = ExecWndParamListChanged_WndRoi_Add_Auto_Color(WndPtr);	}

	if ( ALG_PIXEL_COMPARE == AlgType )
	{	bResult = ExecWndParamListChanged_WndRoi_Add_Auto_Text(WndPtr);	}

	if ( ALG_BLOB_COUNT == AlgType )
	{	bResult = ExecWndParamListChanged_WndRoi_Add_Auto_Text(WndPtr);	}
	return bResult;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::ExecWndParamListChanged_WndRoi_Add_Auto_Text(CAOIWnd *WndPtr)
{
	if ( NULL == WndPtr ) { return false; }
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return false; }	

	CString str;
	const double ComponentAngle = ModelPtr->GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComponentAngle);
	const bool bNoGo=false;
	if ( true == bNoGo )
	{
		str = _T("Error, It does not support Exception-Angle Part");
		str = LoadMultiLanguageString(str, str);
		JetAPI::ShowMessageBox(str);
		return false;		
	}

	const int nAlign = 4;
	const bool bClone = true;
	std::vector<TUNI_FRAME> UniFrameList;
	ALG_TYPE AlgType = WndPtr->GetWndAlgType();
	if ( AOIDataCollect.CopyModelUniFrameList(UniFrameList, bClone) == false )
	{	return false; }				
	const size_t UniFrameCount = UniFrameList.size();
	if ( 0 == UniFrameCount ) 
	{	return false; }
	CAlgBinaryParam    BinaryParam = WndPtr->GetWndAlgParam().GetAlgImageBinParam();
	const unsigned int FrameIndex = BinaryParam.GetBinaryFrameIndex();
	if ( FrameIndex >= UniFrameCount )
	{	
		JetAPI::ClearUniFrameList(UniFrameList);
		return false; 
	}
	
	RECT       RoiRect={0, 0, 0, 0};
	RECT       WndRect={0, 0, 0, 0};
	RECT       WndExtRect={0, 0, 0, 0};
	RECT       ModelRect={0, 0, 0, 0};
	TREGION4D  ModelRgn, WndRgn, WndExtRgn;	
	TPOINT2D   RgnCp, Scale, ImageCp;
	ModelPtr->GetModelTotalRegion(ModelRgn);	
	
	IMAGE_PTR  WndImagePtr=NULL;
	IMAGE_SIZE WndImageW=0;
	IMAGE_SIZE WndImageH=0;
	IMAGE_SIZE WndImageStep=0;	
	bool UsingWndExtendBox = false;

	TUNI_FRAME UniFrame = UniFrameList[FrameIndex];
	const IMAGE_PTR  ImagePtr = UniFrame.ImagePtr;
	const IMAGE_SIZE ImageW = UniFrame.ImageW;
	const IMAGE_SIZE ImageH = UniFrame.ImageH;
	const IMAGE_SIZE ImageStep = UniFrame.ImageStep;
	const IMAGE_SIZE BitCount  = UniFrame.BitCount;
	const double RegionW = ModelRgn.GetWidth();
	const double RegionH = ModelRgn.GetHeight();		
	BINARY_MODE BinaryMode = BinaryParam.GetBinaryMode();
	IMAGE_SRC_MODE ImageSourceMode = BinaryParam.GetBinaryImageSourceMode();
	const int SynWR = BinaryParam.GetBinarySynthesisWR();
	const int SynWG = BinaryParam.GetBinarySynthesisWG();
	const int SynWB = BinaryParam.GetBinarySynthesisWB();
	const bool WndExtendBoxUsed = WndPtr->GetWndExtendBoxUsed();
	const bool bSaveDebug=true;
	DRAW_MODEL_MODE DrawModelMode = AOIDataCollect.GetDrawModelMode();

	UsingWndExtendBox = WndExtendBoxUsed;
	if ( BINARY_DYNAMIC_THRESHOLD != BinaryMode )
	{	UsingWndExtendBox = false;	}
	UsingWndExtendBox = false;

	if ( false == IsExceptionAngle )
	{	
		if (DRAW_MODEL_RESULT == DrawModelMode )
		{
			WndPtr->GetWndRegionRes(WndRgn);
			WndPtr->GetWndExtendBox().GetBoxRegionRes(WndExtRgn);
		}
		else
		{
			WndPtr->GetWndRegion(WndRgn);
			WndPtr->GetWndExtendBox().GetBoxRegion(WndExtRgn);
		}
		JetAPI::SizeToRect(ImageW, ImageH, ModelRect);
		JetAPI::MapRegionToRect(ModelRgn, WndRgn, ModelRect, WndRect);
		JetAPI::MapRegionToRect(ModelRgn, WndExtRgn, ModelRect, WndExtRect);

		if ( false == UsingWndExtendBox )
		{	RoiRect = WndRect;	}
		else
		{	RoiRect = WndExtRect;	}

		WndImageW = RoiRect.right-RoiRect.left;
		WndImageH = RoiRect.bottom-RoiRect.top;
		WndImageStep = JetAPI::GetBMPImagePixelsPerLine(WndImageW, BitCount, nAlign);
		if ( ImageAPI.ExtractRoiImage(ImageW, ImageH, ImageStep, BitCount, ImagePtr, RoiRect, WndImageStep, WndImagePtr, false) == false )
		{
			JetAPI::ClearUniFrameList(UniFrameList);
			return false;
		}		
		JetAPI::ClearUniFrameList(UniFrameList);
	}
	else
	{
		TPOINT2D  WndCornerPts[4];
		TREGION4D WndRgnRotated;
		TPOINT2D  WndExtCornerPts[4];
		TREGION4D WndExtRgnRotated;
		TPOINT2D  ModelCornerPts[4];
		TREGION4D ModelRgnRotated;
		std::vector<TUNI_FRAME> UniFrameListDst;
		
		if (DRAW_MODEL_RESULT == DrawModelMode )
		{
			WndPtr->GetWndCornerPosRes(WndCornerPts);
			WndPtr->GetWndExtendBox().GetBoxCornerPosRes(WndExtCornerPts);
		}
		else
		{
			WndPtr->GetWndCornerPos(WndCornerPts);
			WndPtr->GetWndExtendBox().GetBoxCornerPos(WndExtCornerPts);
		}
		ModelPtr->GetModelTotalCornerPts(ModelCornerPts);
		JetAPI::RotateCornerPos(-ComponentAngle, 0, 0, WndCornerPts);
		JetAPI::RotateCornerPos(-ComponentAngle, 0, 0, WndExtCornerPts);
		JetAPI::RotateCornerPos(-ComponentAngle, 0, 0, ModelCornerPts);
		JetAPI::CornerPtToRegion(WndCornerPts, WndRgnRotated);
		JetAPI::CornerPtToRegion(WndExtCornerPts, WndExtRgnRotated);
		JetAPI::CornerPtToRegion(ModelCornerPts, ModelRgnRotated);
		if ( CAOIModel::RotateModelUniFrameList(-ComponentAngle, ModelRgn, ModelRgnRotated, UniFrameList, UniFrameListDst) == false )
		{			
			JetAPI::ClearUniFrameList(UniFrameList);
			return false;
		}
		JetAPI::ClearUniFrameList(UniFrameList);

		const IMAGE_PTR  ModelImagePtr = UniFrameListDst[FrameIndex].ImagePtr;
		const IMAGE_SIZE ModelImageW = UniFrameListDst[FrameIndex].ImageW;
		const IMAGE_SIZE ModelImageH = UniFrameListDst[FrameIndex].ImageH;
		const IMAGE_SIZE ModelImageStep = UniFrameListDst[FrameIndex].ImageStep;	
		const double RgnWRotated = ModelRgnRotated.GetWidth();
		const double RgnHRotated = ModelRgnRotated.GetHeight();
				
		JetAPI::SizeToRect(ModelImageW, ModelImageH, ModelRect);
		JetAPI::MapRegionToRect(ModelRgnRotated, WndRgnRotated, ModelRect, WndRect);
		JetAPI::MapRegionToRect(ModelRgnRotated, WndExtRgnRotated, ModelRect, WndExtRect);

		if ( false == UsingWndExtendBox )
		{	RoiRect = WndRect;	}
		else
		{	RoiRect = WndExtRect;	}
		WndImageW = RoiRect.right-RoiRect.left;
		WndImageH = RoiRect.bottom-RoiRect.top;
		WndImageStep = JetAPI::GetBMPImagePixelsPerLine(WndImageW, BitCount, nAlign);
		if ( ImageAPI.ExtractRoiImage(ModelImageW, ModelImageH, ModelImageStep, BitCount, ModelImagePtr, RoiRect, WndImageStep, WndImagePtr, false) == false )
		{
			JetAPI::ClearUniFrameList(UniFrameListDst);
			return false;
		}		
		JetAPI::ClearUniFrameList(UniFrameListDst);

		WndRgn = WndRgnRotated;
		WndExtRgn = WndExtRgnRotated;
	}
	if ( NULL == WndImagePtr ) 
	{	return false; } 
	if ( true == bSaveDebug )
	{
		str.Format(_T("%s\\%s.PNG"), AOIDataCollect.GetAOITempDirectory(), _T("WndRoiAutoAddImage"));
		ImageAPI.SaveImage(str, WndImageW, WndImageH, WndImageStep, BitCount, WndImagePtr, true);
	}

	const size_t RoiWndCount=WndPtr->GetWndRoiWndCount();
	if ( RoiWndCount > 0 )
	{
		str = _T("Do you want to auto-add all roi wnds?");
		str = LoadMultiLanguageString(str, str);
		if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO )
		{
			JetMemory.free_func(WndImagePtr);
			return false; 
		}	
	}

	//分析Wnd的字元數
	bool       bIsOK = true;
	RECT       BinRect={0,0,0,0};
	IMAGE_PTR  BinImagePtr=NULL;
	IMAGE_SIZE BinImageW = WndImageW;
	IMAGE_SIZE BinImageH = WndImageH;
	IMAGE_SIZE BinBitCount  = 8;	
	IMAGE_SIZE BinImageStep = JetAPI::GetBMPImagePixelsPerLine(BinImageW, BinBitCount, nAlign);	

	JetAPI::SizeToRect(BinImageW, BinImageH, BinRect);
	if ( BINARY_DISABLE == BinaryMode )
	{
		int        Threshold=0;
		IMAGE_PTR  GrayImagePtr=NULL;		
		if ( 24 == BitCount )
		{	bIsOK = ImageAPI.ColorImageToGrayImage(WndImageW, WndImageH, WndImageStep, WndImagePtr, BinRect, BinImageStep, GrayImagePtr, ImageSourceMode, SynWR, SynWG, SynWB, false);	}
		else
		{
			bIsOK = true;
			GrayImagePtr = WndImagePtr;	
		}
		if ( false == bIsOK )
		{
			if ( 24 == BitCount )
			{	JetMemory.free_func(GrayImagePtr); }	
			JetMemory.free_func(BinImagePtr);
			JetMemory.free_func(WndImagePtr);
			return false;
		}
		if ( true == bSaveDebug )
		{
			str.Format(_T("%s\\%s.PNG"), AOIDataCollect.GetAOITempDirectory(), _T("WndRoiAutoAddImageGry"));
			ImageAPI.SaveImage(str, WndImageW, WndImageH, BinImageStep, BinBitCount, GrayImagePtr, true);
		}
		const double OffsetValue=0.0;
		const double GrainValue=BinaryParam.GetGrayGainValue();
		const bool  bGainEnabled=BinaryParam.CheckGrayGainEnabed();
		if ( true == bGainEnabled )
		{
			if ( true == bSaveDebug )
			{
				ImageAPI.ImageOffsetGain3(WndImageW, WndImageH, BinImageStep, BinBitCount, GrayImagePtr, GrayImagePtr, OffsetValue, GrainValue);
				str.Format(_T("%s\\%s.PNG"), AOIDataCollect.GetAOITempDirectory(), _T("WndRoiAutoAddImageGryGain"));
				ImageAPI.SaveImage(str, WndImageW, WndImageH, BinImageStep, BinBitCount, GrayImagePtr, true);
			}
		}
		bIsOK = ImageAPI.CalcGrayImageOTSUThreshold(WndImageW, WndImageH, BinImageStep, GrayImagePtr, BinRect, Threshold);
		if ( false == bIsOK )
		{
			if ( 24 == BitCount )
			{	JetMemory.free_func(GrayImagePtr); }	
			JetMemory.free_func(BinImagePtr);
			JetMemory.free_func(WndImagePtr);
			return false;
		}	
		bIsOK = ImageAPI.BinaryGrayImage(WndImageW, WndImageH, BinImageStep, GrayImagePtr, BinRect, BinImageStep, BinImagePtr, Threshold, 255);
		if ( false == bIsOK )
		{
			if ( 24 == BitCount )
			{	JetMemory.free_func(GrayImagePtr); }	
			JetMemory.free_func(BinImagePtr);
			JetMemory.free_func(WndImagePtr);
			return false;
		}
		if ( 24 == BitCount )
		{	JetMemory.free_func(GrayImagePtr); }	
		GrayImagePtr = NULL;

		//白字黑底嗎?
		str = _T("Is it white word on dark ground?");
		str = LoadMultiLanguageString(str, str);
		if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO )
		{	ImageAPI.InvertGrayImage3(WndImageW, WndImageH, BinImageStep, BinImagePtr, BinImagePtr);	}

		//Open	
		const int OpenIterCnt = 1;
		const int OpenKenSize = 3;
		IMAGE_PTR TempImagePtr = WndImagePtr;//暫借指標空間
		WndImagePtr = NULL;
		bIsOK = ImageAPI.MorphGrayImage3(WndImageW, WndImageH, BinImageStep, BinImagePtr, MORPH_OPEN, MORPH_SHAPE_RECT, OpenKenSize, OpenIterCnt, TempImagePtr);
		if ( false == bIsOK )
		{
			JetMemory.free_func(BinImagePtr);
			JetMemory.free_func(TempImagePtr);
			return false;
		}
		if ( true == bSaveDebug )
		{
			str.Format(_T("%s\\%s.PNG"), AOIDataCollect.GetAOITempDirectory(), _T("WndRoiAutoAddImageBinOpen"));
			ImageAPI.SaveImage(str, WndImageW, WndImageH, BinImageStep, BinBitCount, TempImagePtr, true);
		}

		//Close
		const int CloseIterCnt = 1;
		const int CloseKenSize = 3;
		bIsOK = ImageAPI.MorphGrayImage3(WndImageW, WndImageH, BinImageStep, TempImagePtr, MORPH_CLOSE, MORPH_SHAPE_RECT, CloseKenSize, CloseIterCnt, BinImagePtr);
		if ( false == bIsOK )
		{
			JetMemory.free_func(BinImagePtr);
			JetMemory.free_func(TempImagePtr);
			return false;
		}
		if ( true == bSaveDebug )
		{
			str.Format(_T("%s\\%s.PNG"), AOIDataCollect.GetAOITempDirectory(), _T("WndRoiAutoAddImageBinClose"));
			ImageAPI.SaveImage(str, WndImageW, WndImageH, BinImageStep, BinBitCount, BinImagePtr, true);
		}
		JetMemory.free_func(TempImagePtr);
	}
	else
	{
		bIsOK = WndPtr->GetWndAlgParam().ExecAlgImageBinary_Rect(BinaryParam, BinRect, BinRect, WndImageW, WndImageH, WndImageStep, BitCount, WndImagePtr, NULL, NULL, nAlign, BinImagePtr);
		if ( false == bIsOK )
		{	
			JetMemory.free_func(WndImagePtr);
			return false; 
		}
	}
	if ( true == bSaveDebug )
	{
		str.Format(_T("%s\\%s.PNG"), AOIDataCollect.GetAOITempDirectory(), _T("WndRoiAutoAddImageBin"));
		ImageAPI.SaveImage(str, WndImageW, WndImageH, BinImageStep, BinBitCount, BinImagePtr, true);
	}


	//Blob
	RECT     CalcRect={0,0,0,0};
	CJetBlob BlobDetector;
	BlobDetector.InitialBlob();
	BlobDetector.SetBlobSortMode(BLOB_SORT_PIXELS);
	BlobDetector.SetBlobConnectivity(BLOB_CONNECTIVITY_4);

	if ( false == UsingWndExtendBox )
	{	JetAPI::SizeToRect(WndImageW, WndImageH, CalcRect);	}
	else
	{
		const int WndW = WndRect.right-WndRect.left;
		const int WndH = WndRect.bottom-WndRect.top;
		const int WndExtW = WndExtRect.right-WndExtRect.left;
		const int WndExtH = WndExtRect.bottom-WndExtRect.top;
		const int nExtW2 = (WndExtW-WndW)/2;
		const int nExtH2 = (WndExtH-WndH)/2;
		CalcRect.left = nExtW2;
		CalcRect.top =  nExtH2;
		CalcRect.right = WndImageW-nExtW2;
		CalcRect.bottom = WndImageH-nExtH2;
	}	

	bIsOK = BlobDetector.GrayImageRoiBlobDetect(WndImageW, WndImageH, BinImageStep, BinImagePtr, CalcRect, 128, 255);
	JetMemory.free_func(BinImagePtr);	
	JetMemory.free_func(WndImagePtr);
	if ( false == bIsOK )
	{	return false;	}
	
	size_t       i=0, j=0;
	int          BlobW=0, BlobH=0, BlobArea=0;
	RECT         BlobRect={0,0,0,0};
	double       WndRoiSizeW=0;
	double       WndRoiSizeH=0;
	TBlobResult *BlobPtr=NULL;
	TREGION4D    WndRoiRgn;
	std::vector<TREGION4D> WndRoiRgnList;
	const size_t BlobCount = BlobDetector.GetBlobCount();	
	JetAPI::SizeToRect(WndImageW, WndImageH, RoiRect);

	BlobRect = RoiRect;
	if ( false == UsingWndExtendBox )
	{	JetAPI::MapRectToRegion(RoiRect, BlobRect, WndRgn, WndRoiRgn); }
	else
	{	JetAPI::MapRectToRegion(RoiRect, BlobRect, WndExtRgn, WndRoiRgn); }

	for ( i=0; i<BlobCount; i++ )
	{
		BlobPtr = BlobDetector.GetBlobPtr(i, false);
		if ( NULL == BlobPtr ) { continue; }
		//BlobRect = BlobPtr->m_BlobRectRaw;
		BlobRect = BlobPtr->m_BlobRect;//20230628-Blob		
		if ( ALG_PIXEL_COMPARE == AlgType )
		{	::InflateRect(&BlobRect, 4, 4);	}
		else
		{	::InflateRect(&BlobRect, 2, 2); }
		BlobW = BlobRect.right-BlobRect.left;
		BlobH = BlobRect.bottom-BlobRect.top;
		BlobArea = BlobPtr->m_BlobPixels;

		JetAPI::BoundaryRect(CalcRect, BlobRect);
		if ( false == UsingWndExtendBox )
		{	JetAPI::MapRectToRegion(RoiRect, BlobRect, WndRgn, WndRoiRgn); }
		else
		{	JetAPI::MapRectToRegion(RoiRect, BlobRect, WndExtRgn, WndRoiRgn); }
		WndRoiSizeW = WndRoiRgn.GetWidth();
		WndRoiSizeH = WndRoiRgn.GetHeight();
		
		if ( ALG_PIXEL_COMPARE==AlgType || ALG_BLOB_COUNT==AlgType )
		{
			if ( WndRoiSizeW<60 || WndRoiSizeH<60 )//過濾特別小的
			{	continue;	}
		}
		else
		{
			if ( WndRoiSizeW<60 || WndRoiSizeH<60 )//過濾特別小的
			{	continue;	}
			if ( WndRoiSizeW<180 && WndRoiSizeH<180 )//過濾次小的
			{	continue;	}
		}

		WndRoiRgnList.push_back(WndRoiRgn);
	}
	size_t WndRoiRgnCount = WndRoiRgnList.size();	

	CString strLabel;
	CString strDefaultX;
	CString strDefaultY;
	CString strCaption;
	CString strVer = AOIDataDefine.GetVerticalText();
	CString strHor = AOIDataDefine.GetHorizontalText();
	CString strCount = AOIDataDefine.GetCountText();
	TListNode       Node;	
	CInputBoxWnd    InputBox;
	CInputListWnd   EnumWnd;
	DWORD_PTR       OldIndex=0;
	const int COUNT_MODE_BLOB = 0;//Blob找到
	const int COUNT_MODE_HOR  = 1;//水平
	const int COUNT_MODE_VER  = 2;//垂直
	std::vector<TListNode> NodelList;
	strCaption = _T("Sub Wnd Count");
	strLabel = _T("Sub Wnd Count");
	Node.Data = COUNT_MODE_BLOB; Node.Text.Format(_T("Auto %d"), WndRoiRgnCount); NodelList.push_back(Node);
	Node.Data = COUNT_MODE_HOR; Node.Text = _T("Divide X");	NodelList.push_back(Node);
	Node.Data = COUNT_MODE_VER; Node.Text = _T("Divide Y");		NodelList.push_back(Node);
	EnumWnd.SetParam1(strCaption, strLabel, OldIndex, NodelList);	
	if ( EnumWnd.DoModal() == IDCANCEL )
	{	return false; }
	const int SelMode = (int)(EnumWnd.GetSelData());	
	if ( SelMode > 0 )
	{		
		int CountX=1;
		int CountY=1;
		switch ( SelMode )
		{
		case COUNT_MODE_HOR: 			
			CountX = WndRoiRgnCount;
			break;
		case COUNT_MODE_VER:
			CountY = WndRoiRgnCount;
			break;		
		}
		strCaption = _T("Input the Count");		
		strDefaultX.Format(_T("%d"), CountX);
		strDefaultY.Format(_T("%d"), CountY);		
		InputBox.SetParam2(strCaption, _T("X"), strDefaultX, _T("Y"), strDefaultY);
		if ( InputBox.DoModal() == IDCANCEL )
		{	return false; }

		TREGION4D CalcRgn;
		CountX = ::_ttoi(InputBox.m_DataEdit1);
		CountY = ::_ttoi(InputBox.m_DataEdit2);
		if ( CountX<1 || CountY<1 ) { return  false; }
		if ( false == UsingWndExtendBox ) { CalcRgn = WndRgn; }
		else { CalcRgn = WndExtRgn; }						
		double MarginX=100;
		double MarginY=100;
		double RoiPitchX=0;
		double RoiPitchY=0;
		double MarginMax=200;
		const double CalcRgnW=CalcRgn.GetWidth();
		const double CalcRgnH=CalcRgn.GetHeight();
		
		WndRoiRgnList.clear();			
		MarginX = MIN(CalcRgnW/10, MarginMax);
		MarginY = MIN(CalcRgnH/10, MarginMax);
		if ( ALG_CHAR_VERIFY == AlgType )
		{	MarginX = MarginY = 0.0;	}
		RoiPitchX = (CalcRgnW-MarginX-MarginX)/CountX;
		RoiPitchY = (CalcRgnH-MarginY-MarginY)/CountY;
		for ( i=0; i<CountY; i++ )
		{
			for ( j=0; j<CountX; j++ )
			{
				WndRoiRgn.minX = CalcRgn.minX+(j*RoiPitchX)+MarginX;
				WndRoiRgn.maxX = WndRoiRgn.minX+RoiPitchX;
				WndRoiRgn.minY = CalcRgn.minY+(i*RoiPitchY)+MarginY;
				WndRoiRgn.maxY = WndRoiRgn.minY+RoiPitchY;
				WndRoiRgnList.push_back(WndRoiRgn);
			}
		}		
	}
	WndRoiRgnCount = WndRoiRgnList.size();		
	if ( 0 == WndRoiRgnCount )
	{	return false; }

	const size_t PatternCount = WndPtr->GetWndAlgParam().GetAlgPatternCount();
	if ( PatternCount > 0 ) 
	{
		//清除原有樣板嗎?
		str = _T("Do you want to clear all patterns?");
		str = LoadMultiLanguageString(str, str);
		if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDYES )
		{	
			WndPtr->GetWndAlgParam().ClearAlgPatternFiles();	
			LogOperCtrl.SaveLogModelWndAlgPatternContentClearAll(WndPtr);
		}
	}	
	ModelPtr->ApplyModelWnd(WndPtr);
	ModelPtr->ClearModelWndRoiWndList(WndPtr);

	bIsOK = true;	
	for ( i=0; i<WndRoiRgnCount; i++ )
	{
		WndRoiRgn = WndRoiRgnList[i];
		if ( ModelPtr->AddModelWndRoiWnd(WndPtr, WndRoiRgn) == false )
		{	bIsOK = false; }
	}
	
	CAOIWndRoi    *WndRoiPtr = NULL;
	WndRoiPtr = WndPtr->GetWndRoiWndSelected();
	WndPtr->SetWndRoiWndActived(WndRoiPtr);	
	LogOperCtrl.SaveLogWndRoiSelectedCreate(WndPtr);

	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	if ( NULL == WndRoiPtr )
	{	AlgParam.SetAlgBinParamActived(BIN_PARAM_BELONG_TO_ALG_IMAGE); }
	else
	{
		if ( true == WndRoiPtr->GetWndRoiBinaryParamEnabled() ) 
		{	AlgParam.SetAlgBinParamActived(BIN_PARAM_BELONG_TO_ROI_IMAGE);	}
		else
		{	AlgParam.SetAlgBinParamActived(BIN_PARAM_BELONG_TO_ALG_IMAGE);	}	
	}

	SendMessageToMainFrameWnd(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);		

	bool AddPattern=false;
	if ( ALG_BLOB_COUNT == AlgType )
	{	AddPattern = false;	}
	else
	{
		if ( BINARY_DISABLE != BinaryMode )
		{	AddPattern = true;	}
		else
		{
			str = _T("Do you want to add the pattern?");
			str = LoadMultiLanguageString(str, str);
			if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDYES )
			{	AddPattern = true;	}
		}		
		if ( true == AddPattern )
		{	
			SendMessageToMainFrameWnd(MSG_EDIT_IMAGE_PROCESS_WND, WPARAM_PATTERN_ADD, NULL); 
			SendMessageToMainFrameWnd(MSG_EDIT_IMAGE_PROCESS_WND, WPARAM_PATTERN_TEXT, NULL);
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::ExecWndParamListChanged_WndRoi_Add_Auto_Color(CAOIWnd *WndPtr)
{
	CString str;
	str = _T("Error, Not Support Auto-add Roi for Color");
	//LogOperCtrl.SaveLogWndRoiSelectedCreate(WndPtr);
	JetAPI::ShowMessageBox(str);
	return false;

	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::ExecWndParamListChanged_MaskBox_Add(CAOIWnd *WndPtr)
{	
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == WndPtr ) { return false; }	
	if ( NULL == ModelPtr ) { return false; }

	CAOIWndMask    *MaskWndPtr = NULL;
	if ( ModelPtr->AddModelWndMaskWnd(WndPtr) == false )
	{	return false; }

	MaskWndPtr = WndPtr->GetWndMaskWndActived();
	if ( NULL == MaskWndPtr ) { return false; }
	LogOperCtrl.SaveLogWndMaskSelectedCreate(WndPtr);

	//CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	//if ( true == WndMaskPtr->GetWndRoiBinaryParamEnabled() ) 
	//{	AlgParam.SetAlgBinParamActived(BIN_PARAM_BELONG_TO_ROI_IMAGE);	}
	//else
	//{	AlgParam.SetAlgBinParamActived(BIN_PARAM_BELONG_TO_ALG_IMAGE);	}	
	PostMessageToMainFrameWnd(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::ExecWndParamListChanged_MaskBox_Rotate(CAOIWnd *WndPtr, double Angle)
{	
	if ( NULL == WndPtr ) { return false; }
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return false; }
	if ( fabs(Angle)<0.0001 || fabs(Angle-360.0)<0.00001 ) { return false; }
	CAOIWndMask *MaskWndPtr = WndPtr->GetWndMaskWndActived();
	if ( NULL == MaskWndPtr ) { return false; }

	//double  Angle = 90;
	ModelPtr->SpinModelWndMaskWndSelected(WndPtr, Angle);
	LogOperCtrl.SaveLogWndMaskSelectedRotate(WndPtr, Angle);

	//CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	//AlgParam.SetAlgBinParamActived(BIN_PARAM_BELONG_TO_ALG_IMAGE);
	PostMessageToMainFrameWnd(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);			
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::ExecWndParamListChanged_MaskBox_ShapeMode(CAOIWnd *WndPtr, BOX_SHAPE_MODE BoxShapeMode)
{
	if ( NULL == WndPtr ) { return false; }
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return false; }
	CAOIWndMask *MaskWndPtr = WndPtr->GetWndMaskWndActived();
	if ( NULL == MaskWndPtr ) { return false; }

	ModelPtr->SetModelWndMaskWndShapeMode(WndPtr, BoxShapeMode);
	LogOperCtrl.SaveLogWndMaskSelectedShapeMode(WndPtr, BoxShapeMode);

	//CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	//AlgParam.SetAlgBinParamActived(BIN_PARAM_BELONG_TO_ALG_IMAGE);
	PostMessageToMainFrameWnd(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);			
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::ExecWndParamListChanged_MaskBox_ShapeParam(CAOIWnd *WndPtr, double ShapeParam)
{
	if ( NULL == WndPtr ) { return false; }
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return false; }
	CAOIWndMask *MaskWndPtr = WndPtr->GetWndMaskWndActived();
	if ( NULL == MaskWndPtr ) { return false; }

	ModelPtr->SetModelWndMaskWndShapeParam(WndPtr, ShapeParam);
	LogOperCtrl.SaveLogWndMaskSelectedShapeParam(WndPtr, ShapeParam);

	//CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	//AlgParam.SetAlgBinParamActived(BIN_PARAM_BELONG_TO_ALG_IMAGE);
	PostMessageToMainFrameWnd(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);			
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::ExecWndParamListChanged_MaskBox_ShapeParam2(CAOIWnd *WndPtr, double ShapeParam)
{
	if ( NULL == WndPtr ) { return false; }
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return false; }
	CAOIWndMask *MaskWndPtr = WndPtr->GetWndMaskWndActived();
	if ( NULL == MaskWndPtr ) { return false; }

	ModelPtr->SetModelWndMaskWndShapeParam2(WndPtr, ShapeParam);
	LogOperCtrl.SaveLogWndMaskSelectedShapeParam2(WndPtr, ShapeParam);

	//CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	//AlgParam.SetAlgBinParamActived(BIN_PARAM_BELONG_TO_ALG_IMAGE);
	PostMessageToMainFrameWnd(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);			
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::ExecWndParamListChanged_MaskBox_Delete(CAOIWnd *WndPtr)
{
	if ( NULL == WndPtr ) { return false; }
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return false; }
	CString str;
	str = _T("Do you want to delete selected mask box?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO )
	{	return false; }	

	LogOperCtrl.SaveLogWndMaskSelectedDelete(WndPtr);
	ModelPtr->DeleteModelWndMaskWnd(WndPtr);
	//CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	//AlgParam.SetAlgBinParamActived(BIN_PARAM_BELONG_TO_ALG_IMAGE);
	PostMessageToMainFrameWnd(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);			
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::ExecWndParamListChanged_MaskBox_ClearList(CAOIWnd *WndPtr)
{
	if ( NULL == WndPtr ) { return false; }
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return false; }
	CString str;
	str = _T("Do you want to clear all mask box?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO )
	{	return false; }

	LogOperCtrl.SaveLogWndMaskSelectedClearAll(WndPtr);
	ModelPtr->ClearModelWndMaskWndList(WndPtr);
	//CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	//AlgParam.SetAlgBinParamActived(BIN_PARAM_BELONG_TO_ALG_IMAGE);
	PostMessageToMainFrameWnd(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);		
	return true;
}
//-------------------------------------------------------------------------------------//