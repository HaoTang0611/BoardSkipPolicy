// ProjectColorWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "ProjectColorWnd.h"
//-------------------------------------------------------------------------------------//
#include "InputBoxWnd.h"
#include "InputListWnd.h"
#include "ProjectListWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CProjectColorWnd dialog
//-------------------------------------------------------------------------------------//
CProjectColorWnd::CProjectColorWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CProjectColorWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProjectColorWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ProjectPtr = NULL;
	m_ImageIndex = 0;	
	m_ShowImageW = 1024;
	m_ShowImageH = 1024;
	m_ShowImageStep = m_ShowImageW*3;
	m_ShowBitCount = 24;	
	m_RawImagePtr = NULL;
	m_ShowImagePtr = NULL;
	m_SystemColorGroupSetIndex = 0;

	m_RGBVMode = COLOR_RGBV_RED;
	m_RGBVWnd.SetTriangleSize(90*2, 156);	

	m_FrameResolution.x = 10;
	m_FrameResolution.y = 10;	
	PreInitUniFrameBuffer();
	m_ColorRGBVIndex = -1;		
	m_ShowColorGroup = true;
	m_StopColorListBeSelected = false;	
	m_StopColorGroupListBeSelected = false;
	m_ColorGroupCtrlID = PROCLR_COLOR_GROUP_BTN_PAD;
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProjectColorWnd)
	DDX_Control(pDX, PROCLR_IMAGE_WND, m_ImageWnd);
	DDX_Control(pDX, PROCLR_COLOR_GROUP_LIST_WND, m_ColorGroupListCtrl);
	DDX_Control(pDX, PROCLR_COLOR_LIST_WND, m_ColorListCtrl);
	DDX_Control(pDX, PROCLR_VALUE_MIN_SPIN, m_ValueMinSpin);
	DDX_Control(pDX, PROCLR_VALUE_MAX_SPIN, m_ValueMaxSpin);	
	DDX_Control(pDX, PROCLR_COLOR_MIN_SPIN, m_ColorMinSpin);
	DDX_Control(pDX, PROCLR_COLOR_MAX_SPIN, m_ColorMaxSpin);
	DDX_Control(pDX, PROCLR_COLOR_RGBV_WND, m_RGBVWnd);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CProjectColorWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CProjectColorWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_CTLCOLOR()
	ON_WM_GETMINMAXINFO()
	ON_NOTIFY(LVN_ITEMCHANGED, PROCLR_COLOR_GROUP_LIST_WND, OnItemchangedColorGroupListWnd)
	ON_NOTIFY(LVN_ITEMCHANGED, PROCLR_COLOR_LIST_WND, OnItemchangedColorListWnd)
	ON_BN_CLICKED(PROCLR_RED_MASTER_CHK, OnRedMasterChk)
	ON_BN_CLICKED(PROCLR_GREEN_MASTER_CHK, OnGreenMasterChk)
	ON_BN_CLICKED(PROCLR_BLUE_MASTER_CHK, OnBlueMasterChk)
	ON_BN_CLICKED(PROCLR_RED_ENABLED_CHK, OnRedEnabledChk)
	ON_BN_CLICKED(PROCLR_GREEN_ENABLED_CHK, OnGreenEnabledChk)
	ON_BN_CLICKED(PROCLR_BLUE_ENABLED_CHK, OnBlueEnabledChk)
	ON_BN_CLICKED(PROCLR_VALUE_ENABLED_CHK, OnValueEnabledChk)
	ON_NOTIFY(UDN_DELTAPOS, PROCLR_COLOR_MAX_SPIN, OnDeltaposColorMaxSpin)
	ON_NOTIFY(UDN_DELTAPOS, PROCLR_COLOR_MIN_SPIN, OnDeltaposColorMinSpin)
	ON_NOTIFY(UDN_DELTAPOS, PROCLR_VALUE_MAX_SPIN, OnDeltaposValueMaxSpin)
	ON_NOTIFY(UDN_DELTAPOS, PROCLR_VALUE_MIN_SPIN, OnDeltaposValueMinSpin)
	ON_BN_CLICKED(PROCLR_FILTER_EXPAND_BTN, OnFilterExpandBtn)
	ON_BN_CLICKED(PROCLR_FILTER_SHIRNK_BTN, OnFilterShirnkBtn)
	ON_BN_CLICKED(PROCLR_COLOR_MAX_BTN, OnColorMaxBtn)
	ON_BN_CLICKED(PROCLR_COLOR_MIN_BTN, OnColorMinBtn)
	ON_BN_CLICKED(PROCLR_VALUE_MAX_BTN, OnValueMaxBtn)
	ON_BN_CLICKED(PROCLR_VALUE_MIN_BTN, OnValueMinBtn)
	ON_BN_CLICKED(PROCLR_RESET_COLOR_BTN, OnResetColorBtn)
	ON_BN_CLICKED(PROCLR_RESET_ALL_BTN, OnResetAllBtn)
	ON_BN_CLICKED(PROCLR_MERGE_ALL_BTN, OnMergeAllBtn)
	ON_BN_CLICKED(PROCLR_GATHER_COLOR_CHK, OnGatherColorChk)
	ON_BN_CLICKED(PROCLR_SHOW_MAP_BTN, OnShowMapBtn)	
	ON_BN_CLICKED(PROCLR_COLOR_GROUP_BTN_PAD, OnColorGroupBtnPad)
	ON_BN_CLICKED(PROCLR_COLOR_GROUP_BTN_VOID, OnColorGroupBtnVoid)
	ON_BN_CLICKED(PROCLR_COLOR_GROUP_BTN_BODY, OnColorGroupBtnBody)
	ON_BN_CLICKED(PROCLR_COLOR_GROUP_BTN_BOARD, OnColorGroupBtnBoard)
	ON_BN_CLICKED(PROCLR_COLOR_GROUP_BTN_SOLDER, OnColorGroupBtnSolder)
	ON_BN_CLICKED(PROCLR_COLOR_GROUP_BTN_OTHERS, OnColorGroupBtnOthers)
	ON_NOTIFY(NM_CLICK, PROCLR_COLOR_GROUP_LIST_WND, OnClickColorGroupListWnd)
	ON_NOTIFY(NM_CLICK, PROCLR_COLOR_LIST_WND, OnClickColorListWnd)
	ON_BN_CLICKED(PROCLR_SEND_SYSTEM_COLOR_BTN, OnSendSystemColorBtn)
	ON_BN_CLICKED(PROCLR_RESET_COLOR_GROUP_LIST_BTN, OnResetColorGroupListBtn)
	ON_BN_CLICKED(PROCLR_LOAD_SYSTEM_COLOR_BTN, OnLoadSystemColorBtn)
	ON_BN_CLICKED(PROCLR_SET_COLOR_GROUP_FRAME_BTN, OnSetColorGroupFrameBtn)
	ON_BN_CLICKED(PROCLR_COLOR_PROJECT_BTN_PAD, OnColorProjectBtnPad)
	ON_BN_CLICKED(PROCLR_COLOR_PROJECT_BTN_VOID, OnColorProjectBtnVoid)
	ON_BN_CLICKED(PROCLR_COLOR_PROJECT_BTN_BODY, OnColorProjectBtnBody)
	ON_BN_CLICKED(PROCLR_COLOR_PROJECT_BTN_BOARD, OnColorProjectBtnBoard)
	ON_BN_CLICKED(PROCLR_COLOR_PROJECT_BTN_SOLDER, OnColorProjectBtnSolder)
	ON_BN_CLICKED(PROCLR_COLOR_PROJECT_BTN_OTHERS, OnColorProjectBtnOthers)
	ON_BN_CLICKED(PROCLR_COLOR_PROJECT_BTN_ALL, OnColorProjectBtnAll)
	ON_BN_CLICKED(PROCLR_GRAY_COLOR_BTN, OnGrayColorBtn)
	ON_BN_CLICKED(PROCLR_SHOW_GROUP_COLOR_BTN, OnShowGroupColorBtn)
	ON_BN_CLICKED(PROCLR_GATHER_SHOW_RAW_CHK, OnGatherShowRawChk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CProjectColorWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CProjectColorWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SwitchMultiLanguage();	
	CWnd::ShowWindow(SW_MAXIMIZE);

	JetAPI::InitialListCtrl(m_ColorListCtrl);
	JetAPI::InitialListCtrl(m_ColorGroupListCtrl);
	m_RGBVWnd.CreateMemDC();
	m_RGBVWnd.SetCallBackWnd(this->GetSafeHwnd());
	
	m_ImageWnd.SetShowCursorInfo(true);
	m_ImageWnd.SetShowCursorLine(false);
	m_ImageWnd.SetShowWndCenterLine(true);	
	m_ImageWnd.SetRBtnUpMode(IMAGE_RBTN_UP_MOVE_STAGE);
	m_ImageWnd.SetRBtnDbClickMode(IMAGE_RBTN_DBCLICK_NULL);

	m_ProjectMapWnd.Create(IDD_PROJECT_MAP_WND, this);
	m_ProjectMapWnd.SetProjectPtr(m_ProjectPtr, false);
	
	switch ( m_RGBVMode )
	{
	case COLOR_RGBV_RED:	CWnd::CheckDlgButton(PROCLR_RED_MASTER_CHK, TRUE);	break;
	case COLOR_RGBV_GREEN:	CWnd::CheckDlgButton(PROCLR_GREEN_MASTER_CHK, TRUE);	break;		
	case COLOR_RGBV_BLUE:	CWnd::CheckDlgButton(PROCLR_BLUE_MASTER_CHK, TRUE);	break;
	}	

	const int ValueMax = COLOR_RGBV_MAX;
	const int ValueMin = COLOR_RGBV_MIN;
	m_ValueMinSpin.SetRange((short)(ValueMin), (short)(ValueMax));
	m_ValueMaxSpin.SetRange((short)(ValueMin), (short)(ValueMax));
	m_ColorMinSpin.SetRange((short)(ValueMin), (short)(ValueMax));
	m_ColorMaxSpin.SetRange((short)(ValueMin), (short)(ValueMax));
	CWnd::SetDlgItemInt(PROCLR_COLOR_MAX_EDIT, ValueMax);
	CWnd::SetDlgItemInt(PROCLR_COLOR_MIN_EDIT, ValueMin);
	CWnd::SetDlgItemInt(PROCLR_VALUE_MAX_EDIT, ValueMax);
	CWnd::SetDlgItemInt(PROCLR_VALUE_MIN_EDIT, ValueMin);
	CWnd::SetDlgItemInt(PROCLR_FILTER_EXPAND_EDIT, 2);

	BuildColorListWndHeader();
	BuildColorGroupListWndHeader();
	BuildColorGroupListWnd();

	BOOL bCheck = TRUE;
	CWnd::CheckDlgButton(PROCLR_GATHER_COLOR_CHK, bCheck);	
	m_ImageWnd.ResetLBtnPos();
	m_ImageWnd.SetShowLBtnPos(bCheck);		

	HWND hWnd = CWnd::GetSafeHwnd();
	AOIDataCollect.SetCallbackWnd(hWnd);	
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	MotionCtrlPtr->SetIsJogMode(true);		
	ExecMoveToStage();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	ReleaseUniFrameBuffer();
	ReleaseShowImageBuffer();
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::OnSize(UINT nType, int cx, int cy) 
{
	CBaseDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if ( CWnd::GetSafeHwnd() == NULL ) { return; }

	CWnd     *WndPtr=NULL;
	const int MarginW=4;
	const int MarginH=4;

	WndPtr = GetDlgItem(PROCLR_IMAGE_WND);
	if ( NULL!=WndPtr && WndPtr->GetSafeHwnd()!=NULL )
	{
		RECT WndRect={0};
		WndPtr->GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		WndRect.right = cx-MarginW;		
		WndRect.bottom = cy-MarginH;
		WndPtr->MoveWindow(&WndRect);
	}
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CBaseDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
HBRUSH CProjectColorWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CBaseDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default	
	CBaseDialog::OnGetMinMaxInfo(lpMMI);
	lpMMI->ptMinTrackSize.x = 1024;
	lpMMI->ptMinTrackSize.y = 768;
} 
//-------------------------------------------------------------------------------------//
LRESULT CProjectColorWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch ( message )
	{
	case MSG_COLOR_FILTER_WND:
		switch ( wParam )
		{
		case WPARAM_UPDATE_COLOR_FILTER:
			UpdateRGBVToUI(m_RGBVWnd.GetColorRGBVPtr());
			BuildColorFilterImage(m_RGBVWnd.GetColorRGBVPtr());
			UpdateColorListWnd();
			break;
		case WPARAM_UPDATE_COLOR_FILTER_BTN_UP:
			break;
		case WPARAM_UPDATE_COLOR_FILTER_RIGHT_BTN_UP:
			SwitchRGBMasterMode();
			break;
		}
		break;
	case MSG_CAMERA_CALLBACK:		
		switch ( wParam )
		{
		case WPARAM_CAMERA_1_CALLBACK:			
		case WPARAM_CAMERA_2_CALLBACK:			
		case WPARAM_CAMERA_3_CALLBACK:			
		case WPARAM_CAMERA_4_CALLBACK:			
		case WPARAM_CAMERA_5_CALLBACK:
			if ( this->RetrieveCameraImage(wParam, lParam, true) == false )
			{	this->LockUIWnd(false);	}
			break;
		}
		break;	
	case MSG_EDIT_MAIN_VIEW_WND:
		switch ( wParam )
		{
		case WPARAM_SWITCH_FRAME_IMAGE:
			ExecMoveToStage();
			break;
		case WPARAM_TOGGLE_ENCHANGE_IMAGE_MODE://切換強化影像模式
			BuildShowImageBuffer(false);
			CreateBKImage();
			RedrawWnd();	
			break;
		}
		break;
	case MSG_CAMERA_REGRAB_IMAGE:
		ExecMoveToStage();
		break;	
	case MSG_IMAGE_WND_NOTIFY_EVENT:
		switch ( wParam )
		{
		case WPARAM_MOUSE_WHEEL:
			//if ( AOIDataCollect.CheckOpenMPUsedGeneral(0) == true )
			//{	BuildColorFilterImage(m_RGBVWnd.GetColorRGBVPtr());	}	
			break;
		case WPARAM_LBUTTON_UP:
			if ( AOIDataCollect.CheckSwitchFrameMode()==false )
			{	CalcColorFilterParam(); }
			break;
		case WPARAM_RBUTTON_UP:
			if ( AOIDataCollect.CheckSwitchFrameMode()==false )
			{	BuildColorFilterImage(m_RGBVWnd.GetColorRGBVPtr()); }
			break;
		case WPARAM_CONTEXT_MENU:
			SwitchFrameImage();
			break;
		}		
		break;
	}
	return CBaseDialog::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::SetColorRGBVIndex(int val)
{
	m_ColorRGBVIndex = val;
}
//-------------------------------------------------------------------------------------//
int CProjectColorWnd::GetColorRGBVIndex() const
{
	return m_ColorRGBVIndex;
}
//-------------------------------------------------------------------------------------//
bool CProjectColorWnd::ClearColorGroupListWnd()
{
	m_StopColorGroupListBeSelected = true;	
	m_ColorGroup.SetColorGroupIndex(-1);
	JetAPI::ClearListCtrl(m_ColorGroupListCtrl, FALSE);		
	m_StopColorGroupListBeSelected = false;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectColorWnd::BuildColorGroupListWnd()
{
	ClearColorGroupListWnd();
	CThisListCtrl_17 &ListCtrl = m_ColorGroupListCtrl;	
	
	size_t       i=0;
	int          nItem=0;
	CString      ItemText;
	CColorGroup *ColorGroupPtr = NULL;
	const size_t ColorGroupCount = m_ColorGroupList.size();

	nItem = 0;	
	ListCtrl.SetRedraw(FALSE);
	m_StopColorGroupListBeSelected = true;
	for ( i=0; i<ColorGroupCount; i++ )
	{
		ColorGroupPtr = &(m_ColorGroupList[i]);
		if ( NULL == ColorGroupPtr ) { continue; }
		if ( CheckColorGroupVisible(i) == false ) { continue; }		

		ItemText.Format(_T("%d"), i+1);
		ListCtrl.InsertItem(nItem, ItemText);
		ListCtrl.SetItemData(nItem, i);
		ListCtrl.SetItemText(nItem, 0, ItemText);

		ItemText = ColorGroupPtr->GetColorGroupName();
		ListCtrl.SetItemText(nItem, 1, ItemText);

		nItem ++;
	}
	m_StopColorGroupListBeSelected = false;
	ListCtrl.SetRedraw(TRUE);
	
	if ( nItem > 0 ) 
	{	ListCtrl.SetItemState(0, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectColorWnd::BuildColorGroupListWndHeader()
{
	CString str;
	int   nCol = 0;
	int width  = 64;
	int width2 = 64;
	RECT      Rect;	
	const int Align = LVCFMT_LEFT;//LVCFMT_CENTER;LVCFMT_RIGHT, LVCFMT_LEFT

	CThisListCtrl_17 &ListCtrl = m_ColorGroupListCtrl;
	ListCtrl.GetClientRect(&Rect);	
	width2 = 48;
	str = AOIDataDefine.GetIndexText();	
	ListCtrl.InsertColumn(nCol, str, Align, width2);
	nCol ++;

	width = (Rect.right-Rect.left-width-8);
	str = _T("Color Name");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(nCol, str, Align, width);
	nCol ++;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectColorWnd::CheckColorGroupVisible(size_t idx)
{
	bool Visible = true;
	switch ( m_ColorGroupCtrlID )
	{
	case PROCLR_COLOR_GROUP_BTN_PAD:
		if ( idx < PROJECT_COLOR_ID_PAD_BEGIN || idx>PROJECT_COLOR_ID_PAD_END ) 
		{	Visible = false; }
		break;
	case PROCLR_COLOR_GROUP_BTN_VOID:
		if ( idx < PROJECT_COLOR_ID_VOID_BEGIN || idx>PROJECT_COLOR_ID_VOID_END ) 
		{	Visible = false; }
		break;
	case PROCLR_COLOR_GROUP_BTN_BODY:
		if ( idx < PROJECT_COLOR_ID_BODY_BEGIN || idx>PROJECT_COLOR_ID_BODY_END ) 
		{	Visible = false; }
		break;
	case PROCLR_COLOR_GROUP_BTN_BOARD:
		if ( idx < PROJECT_COLOR_ID_BOARD_BEGIN || idx>PROJECT_COLOR_ID_BOARD_END ) 
		{	Visible = false; }
		break;		
	case PROCLR_COLOR_GROUP_BTN_SOLDER:
		if ( idx < PROJECT_COLOR_ID_SOLDER_BEGIN || idx>PROJECT_COLOR_ID_SOLDER_END ) 
		{	Visible = false; }
		break;
	case PROCLR_COLOR_GROUP_BTN_OTHERS:
		if ( idx < PROJECT_COLOR_ID_OTHERS_BEGIN || idx>PROJECT_COLOR_ID_OTHERS_END ) 
		{	Visible = false; }
		break;
	}
	return Visible;
}
//-------------------------------------------------------------------------------------//
bool CProjectColorWnd::CheckColorGroupValid()//確認目前顏色群組有效
{
	const size_t index = m_ColorGroup.GetColorGroupIndex();
	if ( INVALID_INDEX == index ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectColorWnd::ClearColorListWnd()
{
	m_StopColorListBeSelected = true;
	JetAPI::ClearListCtrl(m_ColorListCtrl, FALSE);	
	m_StopColorListBeSelected = false;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectColorWnd::BuildColorListWnd()
{
	ClearColorListWnd();
	if ( CheckColorGroupValid() == false ) { return true; }

	size_t       i=0;	
	int          nItem = 0;
	int          nSubItem = 0;
	int          nUnusedIndex=0;
	CString      str;
	COLORREF     rgbvColor=0;	
	COLORREF     TextColor = 0xFFFFFF;
	COLOR_LOGIC_MODE  LogicMode;
	CColorRGBV   *rgbvPtr=NULL;	
	CThisListCtrl_17 &ListCtrl = m_ColorListCtrl;	

	const int    rgbvIndex = GetColorRGBVIndex();
	const size_t ColorCount = m_ColorGroup.GetColorGroupColorCount();
	
	nUnusedIndex = -1;
	ListCtrl.SetRedraw(FALSE);
	m_StopColorListBeSelected = true;
	for ( i=0; i<ColorCount; i++ )
	{
		rgbvPtr = m_ColorGroup.GetColorGroupColorPtr(i, false);
		if ( NULL == rgbvPtr ) { continue; }		
		LogicMode = rgbvPtr->GetLogicMode();
		rgbvColor = rgbvPtr->GetShowColor();

		nSubItem = 0;		
		str.Format(_T("%d"), nItem+1);
		ListCtrl.InsertItem(nItem, str);
		ListCtrl.SetItemData(nItem, (DWORD_PTR) rgbvPtr);		
		ListCtrl.SetItemText(nItem, nSubItem, str);	
		ListCtrl.SetItemTextBkColor(nItem, nSubItem, TextColor);		
		nSubItem++;

		switch ( LogicMode )
		{
		case COLOR_LOGIC_INCLUDE: str = _T("+"); break;
		case COLOR_LOGIC_EXCLUDE: str = _T("-"); break;
		default: str = _T(""); break;
		}
		ListCtrl.SetItemText(nItem, nSubItem, str);	nSubItem++;	

		if ( rgbvPtr->CheckUsed() == true ) 
		{	str = _T("Y"); }
		else
		{	str = _T(""); }		
		ListCtrl.SetItemText(nItem, nSubItem, str);	nSubItem++;					

		ListCtrl.SetItemTextBkColor(nItem, nSubItem, rgbvColor); nSubItem++;

		//找第一個未使用的引數
		if ( nUnusedIndex < 0 ) 
		{
			if ( COLOR_LOGIC_INCLUDE == LogicMode )
			{
				if ( rgbvPtr->CheckUsed() == false )
				{	nUnusedIndex = nItem; }
			}
		}
		nItem ++;
	}	
	m_StopColorListBeSelected = false;
	ListCtrl.SetRedraw(TRUE);	

	m_ShowColorGroup = true;
	SetColorRGBVIndex(-1);	
	//BuildColorFilterImage(NULL);
	//m_RGBVWnd.SetColorRGBVPtr(NULL);
	//return true;

	if ( nItem > 0 ) 
	{		
		int nSelItem = 0;		
		if ( nUnusedIndex < 0 ) { nUnusedIndex = 0; }
		if ( (-1==rgbvIndex) || (rgbvIndex>nItem) )
		{	nSelItem = nUnusedIndex; }
		else
		{	nSelItem = (int)(rgbvIndex); }
		ListCtrl.SetItemState(nSelItem, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectColorWnd::UpdateColorListWnd()
{
	if ( CheckColorGroupValid() == false ) { return true; }
	
	int          i=0;
	int          nItem = 0;
	int          nSubItem = 0;
	CString      str;
	COLORREF     rgbvColor=0;	
	COLOR_LOGIC_MODE  LogicMode;
	CColorRGBV   *rgbvPtr=NULL;
	CThisListCtrl_17 &ListCtrl = m_ColorListCtrl;	
	const int ItemCount = ListCtrl.GetItemCount();
		
	ListCtrl.SetRedraw(FALSE);
	m_StopColorListBeSelected = true;
	for ( i=0; i<ItemCount; i++ )
	{
		rgbvPtr = (CColorRGBV*)ListCtrl.GetItemData(i);
		if ( NULL == rgbvPtr ) { continue; }				
		nItem = i;
		nSubItem = 0;
		LogicMode = rgbvPtr->GetLogicMode();
		rgbvColor = rgbvPtr->GetShowColor();

		str.Format(_T("%d"), nItem+1);			
		ListCtrl.SetItemText(nItem, nSubItem, str);	nSubItem++;

		switch ( LogicMode )
		{
		case COLOR_LOGIC_INCLUDE: str = _T("+"); break;
		case COLOR_LOGIC_EXCLUDE: str = _T("-"); break;
		default: str = _T(""); break;
		}
		ListCtrl.SetItemText(nItem, nSubItem, str);	nSubItem++;	

		if ( rgbvPtr->CheckUsed() == true ) 
		{	str = _T("Y"); }
		else
		{	str = _T(""); }		
		ListCtrl.SetItemText(nItem, nSubItem, str);	nSubItem++;	

		ListCtrl.SetItemTextBkColor(nItem, nSubItem, rgbvColor); nSubItem++;
		nItem ++;
	}
	m_StopColorListBeSelected = false;
	ListCtrl.SetRedraw(TRUE);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectColorWnd::BuildColorListWndHeader()
{
	CThisListCtrl_17 &ListCtrl = m_ColorListCtrl;

	JetAPI::InitialListCtrl(ListCtrl);
	JetAPI::ClearListCtrlHeaderList(ListCtrl);

	CString str;
	int   nCol = 0;
	int width2 = 96;
	RECT      Rect;	
	const int Align = LVCFMT_CENTER;//LVCFMT_CENTER;LVCFMT_RIGHT	

	ListCtrl.GetClientRect(&Rect);
	width2 = (Rect.right-Rect.left-16)/4;

	str = _T("Idx");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(nCol, str, Align, width2);
	nCol ++;

	str = _T("+/-");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(nCol, str, Align, width2);
	nCol ++;

	str = _T("Used");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(nCol, str, Align, width2);
	nCol ++;

	str = _T("Color");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(nCol, str, Align, width2);
	nCol ++;
	return true;
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::SetProjectPtr(CAOIProject *Ptr)
{
	m_ProjectPtr = Ptr;
	m_ColorGroupList.clear();
	if ( NULL == Ptr ) { return; }
	Ptr->CloneProjectColorGroupList(m_ColorGroupList);
}
//-------------------------------------------------------------------------------------//
CAOIProject* CProjectColorWnd::GetActiveProject()
{
	return m_ProjectPtr;
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::GetColorGroupList(std::vector<CColorGroup> &ColorGroupList)
{
	ColorGroupList = m_ColorGroupList;
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_PROJECT_COLOR_WND");
	//---------------------------------------------------------------------------------//
	WndID = IDD_PROJECT_COLOR_WND;
	WndKey = _T("IDD_PROJECT_COLOR_WND");
	this->GetWindowText(LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetWindowText(NewLabelText);
	//---------------------------------------------------------------------------------//
	WndID = IDOK;
	NewLabelText = AOIDataDefine.GetWndOKText();
	this->SetDlgItemText(WndID, NewLabelText);	
	
	WndID = IDCANCEL;
	NewLabelText = AOIDataDefine.GetWndCancelText();
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//
	WndID = PROCLR_COLOR_GROUP_BTN_PAD;
	WndKey = _T("PROCLR_COLOR_GROUP_BTN_PAD");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROCLR_COLOR_GROUP_BTN_VOID;
	WndKey = _T("PROCLR_COLOR_GROUP_BTN_VOID");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROCLR_COLOR_GROUP_BTN_BODY;
	WndKey = _T("PROCLR_COLOR_GROUP_BTN_BODY");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROCLR_COLOR_GROUP_BTN_BOARD;
	WndKey = _T("PROCLR_COLOR_GROUP_BTN_BOARD");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROCLR_COLOR_GROUP_BTN_SOLDER;
	WndKey = _T("PROCLR_COLOR_GROUP_BTN_SOLDER");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROCLR_COLOR_GROUP_BTN_OTHERS;
	WndKey = _T("PROCLR_COLOR_GROUP_BTN_OTHERS");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROCLR_SET_COLOR_GROUP_FRAME_BTN;
	WndKey = _T("PROCLR_SET_COLOR_GROUP_FRAME_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = PROCLR_RESET_COLOR_GROUP_LIST_BTN;
	WndKey = _T("PROCLR_RESET_COLOR_GROUP_LIST_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		
	//---------------------------------------------------------------------------------//
	WndID = PROCLR_COLOR_PROJECT_BTN_PAD;
	WndKey = _T("PROCLR_COLOR_PROJECT_BTN_PAD");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = PROCLR_COLOR_PROJECT_BTN_VOID;
	WndKey = _T("PROCLR_COLOR_PROJECT_BTN_VOID");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = PROCLR_COLOR_PROJECT_BTN_BODY;
	WndKey = _T("PROCLR_COLOR_PROJECT_BTN_BODY");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PROCLR_COLOR_PROJECT_BTN_BOARD;
	WndKey = _T("PROCLR_COLOR_PROJECT_BTN_BOARD");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = PROCLR_COLOR_PROJECT_BTN_SOLDER;
	WndKey = _T("PROCLR_COLOR_PROJECT_BTN_SOLDER");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = PROCLR_COLOR_PROJECT_BTN_OTHERS;
	WndKey = _T("PROCLR_COLOR_PROJECT_BTN_OTHERS");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = PROCLR_COLOR_PROJECT_BTN_ALL;
	WndKey = _T("PROCLR_COLOR_PROJECT_BTN_ALL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = PROCLR_COLOR_LIST_LABEL;
	WndKey = _T("PROCLR_COLOR_LIST_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	
	WndID = PROCLR_RESET_COLOR_BTN;
	WndKey = _T("PROCLR_RESET_COLOR_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROCLR_RESET_ALL_BTN;
	WndKey = _T("PROCLR_RESET_ALL_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROCLR_MERGE_ALL_BTN;
	WndKey = _T("PROCLR_MERGE_ALL_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PROCLR_SHOW_MAP_BTN;
	WndKey = _T("PROCLR_SHOW_MAP_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROCLR_SEND_SYSTEM_COLOR_BTN;
	WndKey = _T("PROCLR_SEND_SYSTEM_COLOR_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROCLR_LOAD_SYSTEM_COLOR_BTN;
	WndKey = _T("PROCLR_LOAD_SYSTEM_COLOR_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROCLR_GATHER_COLOR_CHK;
	WndKey = _T("PROCLR_GATHER_COLOR_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROCLR_RED_MASTER_CHK;
	WndKey = _T("PROCLR_RED_MASTER_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROCLR_GREEN_MASTER_CHK;
	WndKey = _T("PROCLR_GREEN_MASTER_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROCLR_BLUE_MASTER_CHK;
	WndKey = _T("PROCLR_BLUE_MASTER_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROCLR_FILTER_EXPAND_BTN;
	WndKey = _T("PROCLR_FILTER_EXPAND_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROCLR_FILTER_EXPAND_ONLY_COLOR_CHK;
	WndKey = _T("PROCLR_FILTER_EXPAND_ONLY_COLOR_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROCLR_FILTER_SHIRNK_BTN;
	WndKey = _T("PROCLR_FILTER_SHIRNK_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROCLR_VALUE_ENABLED_CHK;
	WndKey = _T("PROCLR_VALUE_ENABLED_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = PROCLR_GRAY_COLOR_BTN;
	WndKey = _T("PROCLR_GRAY_COLOR_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = PROCLR_SHOW_GROUP_COLOR_BTN;
	WndKey = _T("PROCLR_SHOW_GROUP_COLOR_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROCLR_GATHER_SHOW_RAW_CHK;
	WndKey = _T("PROCLR_GATHER_SHOW_RAW_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
}
//-------------------------------------------------------------------------------------//
CString CProjectColorWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_PROJECT_COLOR_WND");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::OnItemchangedColorGroupListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here	
	if ( true == m_StopColorGroupListBeSelected ) { return; }	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) 
	{	return;	}
	DWORD Res = pNMListView->uNewState&LVIS_SELECTED;
	if ( Res == 0 ) { return; }	
	Res = pNMListView->uNewState&LVIS_FOCUSED;
	if ( Res == 0 ) { return; }		
	const size_t ColorGroupIndex = m_ColorGroupListCtrl.GetItemData(nItem);	
	m_ColorGroup = m_ColorGroupList[ColorGroupIndex];

	const unsigned int FrameIndex = m_ColorGroup.GetColorGroupFrameIndex();
	if ( FrameIndex != m_ImageIndex )
	{
		m_ImageIndex = FrameIndex;
		if ( NULL != m_ProjectPtr )
		{	m_ProjectPtr->SetProjectMapIndex(FrameIndex); }
		BuildShowImageBuffer(false);
		CreateBKImage();
		RedrawWnd();
	}

	BuildColorListWnd();	
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::OnItemchangedColorListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if ( true == m_StopColorListBeSelected ) { return; }	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) 
	{	return;	}
	DWORD Res = pNMListView->uNewState&LVIS_SELECTED;
	if ( Res == 0 ) { return; }	
	Res = pNMListView->uNewState&LVIS_FOCUSED;
	if ( Res == 0 ) { return; }	
	
	CColorRGBV *rgbvPtr = (CColorRGBV*)(m_ColorListCtrl.GetItemData(nItem));		
	UpdateRGBVToUI(rgbvPtr);	
	BuildColorFilterImage(rgbvPtr);
	m_RGBVWnd.SetColorRGBVPtr(rgbvPtr);
	m_RGBVWnd.ReDrawWnd();	
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::OnRedMasterChk() 
{
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }

	m_RGBVMode = COLOR_RGBV_RED;	
	CWnd::CheckDlgButton(PROCLR_GREEN_MASTER_CHK, FALSE);
	CWnd::CheckDlgButton(PROCLR_BLUE_MASTER_CHK, FALSE);
	CColorRGBV *rgbvPtr=m_RGBVWnd.GetColorRGBVPtr();
	if ( NULL != rgbvPtr )
	{
		const int nMax = rgbvPtr->GetRedMax();
		const int nMin = rgbvPtr->GetRedMin();
		m_ColorMaxSpin.SetPos(nMax);
		m_ColorMinSpin.SetPos(nMin);
		CWnd::SetDlgItemInt(PROCLR_COLOR_MAX_EDIT, nMax);
		CWnd::SetDlgItemInt(PROCLR_COLOR_MIN_EDIT, nMin);		
	}	
	m_RGBVWnd.SetRGBVMode(m_RGBVMode);
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::OnGreenMasterChk() 
{
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }

	m_RGBVMode = COLOR_RGBV_GREEN;	
	CWnd::CheckDlgButton(PROCLR_RED_MASTER_CHK, FALSE);
	CWnd::CheckDlgButton(PROCLR_BLUE_MASTER_CHK, FALSE);	
	CColorRGBV *rgbvPtr=m_RGBVWnd.GetColorRGBVPtr();
	if ( NULL != rgbvPtr )
	{
		const int nMax = rgbvPtr->GetGreenMax();
		const int nMin = rgbvPtr->GetGreenMin();
		m_ColorMaxSpin.SetPos(nMax);
		m_ColorMinSpin.SetPos(nMin);
		CWnd::SetDlgItemInt(PROCLR_COLOR_MAX_EDIT, nMax);
		CWnd::SetDlgItemInt(PROCLR_COLOR_MIN_EDIT, nMin);		
	}	
	m_RGBVWnd.SetRGBVMode(m_RGBVMode);
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::OnBlueMasterChk() 
{
	// TODO: Add your control notification handler code here
	m_RGBVMode = COLOR_RGBV_BLUE;		
	CWnd::CheckDlgButton(PROCLR_RED_MASTER_CHK, FALSE);
	CWnd::CheckDlgButton(PROCLR_GREEN_MASTER_CHK, FALSE);	
	CColorRGBV *rgbvPtr=m_RGBVWnd.GetColorRGBVPtr();
	if ( NULL != rgbvPtr )
	{
		const int nMax = rgbvPtr->GetBlueMax();
		const int nMin = rgbvPtr->GetBlueMin();
		m_ColorMaxSpin.SetPos(nMax);
		m_ColorMinSpin.SetPos(nMin);
		CWnd::SetDlgItemInt(PROCLR_COLOR_MAX_EDIT, nMax);
		CWnd::SetDlgItemInt(PROCLR_COLOR_MIN_EDIT, nMin);		
	}	
	m_RGBVWnd.SetRGBVMode(m_RGBVMode);
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::OnRedEnabledChk() 
{
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }

	bool bEnabled = false;
	BOOL bCheck = CWnd::IsDlgButtonChecked(PROCLR_RED_ENABLED_CHK);
	if ( TRUE == bCheck ) { bEnabled = true; }
	else { bEnabled = false; }
	CColorRGBV *rgbvPtr=m_RGBVWnd.GetColorRGBVPtr();
	if ( NULL != rgbvPtr )
	{	rgbvPtr->SetRedEnabled(bEnabled); }
	UpdateColorListWnd();
	BuildColorFilterImage(rgbvPtr);

	if ( TRUE == bCheck )
	{	OnRedMasterChk(); }
	else
	{	m_RGBVWnd.ReDrawWnd(); }
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::OnGreenEnabledChk() 
{
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }

	bool bEnabled = false;
	BOOL bCheck = CWnd::IsDlgButtonChecked(PROCLR_GREEN_ENABLED_CHK);
	if ( TRUE == bCheck ) { bEnabled = true; }
	else { bEnabled = false; }
	CColorRGBV *rgbvPtr=m_RGBVWnd.GetColorRGBVPtr();
	if ( NULL != rgbvPtr )
	{	rgbvPtr->SetGreenEnabled(bEnabled); }
	UpdateColorListWnd();
	BuildColorFilterImage(rgbvPtr);

	if ( TRUE == bCheck )
	{	OnGreenMasterChk(); }
	else
	{	m_RGBVWnd.ReDrawWnd(); }
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::OnBlueEnabledChk() 
{
	// TODO: Add your control notification handler code here
	bool bEnabled = false;
	BOOL bCheck = CWnd::IsDlgButtonChecked(PROCLR_BLUE_ENABLED_CHK);
	if ( TRUE == bCheck ) { bEnabled = true; }
	else { bEnabled = false; }
	CColorRGBV *rgbvPtr=m_RGBVWnd.GetColorRGBVPtr();
	if ( NULL != rgbvPtr )
	{	rgbvPtr->SetBlueEnabled(bEnabled); }
	UpdateColorListWnd();
	BuildColorFilterImage(rgbvPtr);

	if ( TRUE == bCheck )
	{	OnBlueMasterChk(); }
	else
	{	m_RGBVWnd.ReDrawWnd(); }
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::OnValueEnabledChk() 
{
	// TODO: Add your control notification handler code here
	bool bEnabled = false;
	BOOL bCheck = CWnd::IsDlgButtonChecked(PROCLR_VALUE_ENABLED_CHK);
	if ( TRUE == bCheck ) { bEnabled = true; }
	else { bEnabled = false; }
	CColorRGBV *rgbvPtr=m_RGBVWnd.GetColorRGBVPtr();
	if ( NULL != rgbvPtr )
	{	rgbvPtr->SetValueEnabled(bEnabled); }
	UpdateColorListWnd();
	BuildColorFilterImage(rgbvPtr);
	m_RGBVWnd.ReDrawWnd();
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::OnDeltaposColorMaxSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }

	CString   str;
	const int nPos = pNMUpDown->iPos;
	const int nDelta = pNMUpDown->iDelta;
	int       nNextPos = nPos+nDelta;
	CColorRGBV *rgbvPtr=m_RGBVWnd.GetColorRGBVPtr();
	if ( NULL == rgbvPtr ) { return; }	
	if ( nNextPos > COLOR_RGBV_MAX ) { nNextPos = COLOR_RGBV_MAX; }
	else if ( nNextPos < COLOR_RGBV_MIN ) { nNextPos = COLOR_RGBV_MIN; }
	const int nLimit = rgbvPtr->GetColorMin(m_RGBVMode)+1;
	if ( nNextPos < nLimit ) 
	{	nNextPos = nLimit;	}
	pNMUpDown->iDelta = nNextPos-nPos;

	str.Format(_T("%d"), nNextPos);
	CWnd::SetDlgItemText(PROCLR_COLOR_MAX_EDIT, str);
	rgbvPtr->SetColorMax(m_RGBVMode, nNextPos);	
	if ( rgbvPtr->GetColorEnabled(m_RGBVMode) == false )
	{
		switch ( m_RGBVMode )
		{
		case COLOR_RGBV_RED: CWnd::CheckDlgButton(PROCLR_RED_ENABLED_CHK, TRUE); 	break;
		case COLOR_RGBV_GREEN: CWnd::CheckDlgButton(PROCLR_GREEN_ENABLED_CHK, TRUE); 	break;
		case COLOR_RGBV_BLUE: CWnd::CheckDlgButton(PROCLR_BLUE_ENABLED_CHK, TRUE); 	break;
		}
		rgbvPtr->SetColorEnabled(m_RGBVMode, true);
		UpdateColorListWnd();
	}
	BuildColorFilterImage(rgbvPtr);
	m_RGBVWnd.ReDrawWnd();
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::OnDeltaposColorMinSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }

	CString   str;
	const int nPos = pNMUpDown->iPos;
	const int nDelta = pNMUpDown->iDelta;
	int       nNextPos = nPos+nDelta;
	CColorRGBV *rgbvPtr=m_RGBVWnd.GetColorRGBVPtr();
	if ( NULL == rgbvPtr ) { return; }
	if ( nNextPos > COLOR_RGBV_MAX ) { nNextPos = COLOR_RGBV_MAX; }
	else if ( nNextPos < COLOR_RGBV_MIN ) { nNextPos = COLOR_RGBV_MIN; }
	const int nLimit = rgbvPtr->GetColorMax(m_RGBVMode)-1;
	if ( nNextPos > nLimit ) 
	{	nNextPos = nLimit;	}
	pNMUpDown->iDelta = nNextPos-nPos;

	str.Format(_T("%d"), nNextPos);
	CWnd::SetDlgItemText(PROCLR_COLOR_MIN_EDIT, str);
	rgbvPtr->SetColorMin(m_RGBVMode, nNextPos);	
	if ( rgbvPtr->GetColorEnabled(m_RGBVMode) == false )
	{
		switch ( m_RGBVMode )
		{
		case COLOR_RGBV_RED: CWnd::CheckDlgButton(PROCLR_RED_ENABLED_CHK, TRUE); 	break;
		case COLOR_RGBV_GREEN: CWnd::CheckDlgButton(PROCLR_GREEN_ENABLED_CHK, TRUE); 	break;
		case COLOR_RGBV_BLUE: CWnd::CheckDlgButton(PROCLR_BLUE_ENABLED_CHK, TRUE); 	break;
		}
		rgbvPtr->SetColorEnabled(m_RGBVMode, true);
		UpdateColorListWnd();
	}
	BuildColorFilterImage(rgbvPtr);
	m_RGBVWnd.ReDrawWnd();
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::OnDeltaposValueMaxSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }

	CString   str;
	const int nPos = pNMUpDown->iPos;
	const int nDelta = pNMUpDown->iDelta;
	int       nNextPos = nPos+nDelta;
	CColorRGBV *rgbvPtr=m_RGBVWnd.GetColorRGBVPtr();
	if ( NULL == rgbvPtr ) { return; }
	if ( nNextPos > COLOR_RGBV_MAX ) { nNextPos = COLOR_RGBV_MAX; }
	else if ( nNextPos < COLOR_RGBV_MIN ) { nNextPos = COLOR_RGBV_MIN; }
	const int nLimit = rgbvPtr->GetValueMin()+1;
	if ( nNextPos < nLimit ) 
	{	nNextPos = nLimit;	}
	pNMUpDown->iDelta = nNextPos-nPos;

	str.Format(_T("%d"), nNextPos);
	CWnd::SetDlgItemText(PROCLR_VALUE_MAX_EDIT, str);
	rgbvPtr->SetValueMax(nNextPos);
	if ( rgbvPtr->GetValueEnabled() == false )
	{
		CWnd::CheckDlgButton(PROCLR_VALUE_ENABLED_CHK, TRUE);
		rgbvPtr->SetValueEnabled(true);
		UpdateColorListWnd();
	}
	BuildColorFilterImage(rgbvPtr);
	m_RGBVWnd.ReDrawWnd();
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::OnDeltaposValueMinSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }

	CString   str;
	const int nPos = pNMUpDown->iPos;
	const int nDelta = pNMUpDown->iDelta;
	int       nNextPos = nPos+nDelta;
	CColorRGBV *rgbvPtr=m_RGBVWnd.GetColorRGBVPtr();
	if ( NULL == rgbvPtr ) { return; }
	if ( nNextPos > COLOR_RGBV_MAX ) { nNextPos = COLOR_RGBV_MAX; }
	else if ( nNextPos < COLOR_RGBV_MIN ) { nNextPos = COLOR_RGBV_MIN; }
	const int nLimit = rgbvPtr->GetValueMax()-1;
	if ( nNextPos > nLimit ) 
	{	nNextPos = nLimit;	}
	pNMUpDown->iDelta = nNextPos-nPos;

	str.Format(_T("%d"), nNextPos);
	CWnd::SetDlgItemText(PROCLR_VALUE_MIN_EDIT, str);
	rgbvPtr->SetValueMin(nNextPos);	
	if ( rgbvPtr->GetValueEnabled() == false )
	{
		CWnd::CheckDlgButton(PROCLR_VALUE_ENABLED_CHK, TRUE);
		rgbvPtr->SetValueEnabled(true);
		UpdateColorListWnd();
	}
	BuildColorFilterImage(rgbvPtr);
	m_RGBVWnd.ReDrawWnd();
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::OnFilterExpandBtn() 
{
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }

	CColorRGBV *rgbvPtr=m_RGBVWnd.GetColorRGBVPtr();
	if ( NULL == rgbvPtr ) { return; }
	bool bOnlyColor = false;
	const int nValue = (int)(CWnd::GetDlgItemInt(PROCLR_FILTER_EXPAND_EDIT));
	if ( CWnd::IsDlgButtonChecked(PROCLR_FILTER_EXPAND_ONLY_COLOR_CHK) == TRUE )
	{	bOnlyColor = true; }
	else
	{	bOnlyColor = false; }
	rgbvPtr->ExpandColorRGBV(nValue, bOnlyColor);
	UpdateRGBVToUI(rgbvPtr);
	BuildColorFilterImage(rgbvPtr);
	m_RGBVWnd.ReDrawWnd();
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::OnFilterShirnkBtn() 
{
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }

	CColorRGBV *rgbvPtr=m_RGBVWnd.GetColorRGBVPtr();
	bool bOnlyColor = false;
	const int nValue = (int)(CWnd::GetDlgItemInt(PROCLR_FILTER_EXPAND_EDIT));
	if ( CWnd::IsDlgButtonChecked(PROCLR_FILTER_EXPAND_ONLY_COLOR_CHK) == TRUE )
	{	bOnlyColor = true; }
	else
	{	bOnlyColor = false; }
	rgbvPtr->ExpandColorRGBV(-nValue, bOnlyColor);
	UpdateRGBVToUI(rgbvPtr);
	BuildColorFilterImage(rgbvPtr);
	m_RGBVWnd.ReDrawWnd();
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::OnColorMaxBtn() 
{
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }

	CString   str;	
	int       nNextPos = COLOR_RGBV_MAX;	
	CColorRGBV *rgbvPtr=m_RGBVWnd.GetColorRGBVPtr();
	if ( NULL == rgbvPtr ) { return; }		
	m_ColorMaxSpin.SetPos(nNextPos);	
	str.Format(_T("%d"), nNextPos);
	CWnd::SetDlgItemText(PROCLR_COLOR_MAX_EDIT, str);
	rgbvPtr->SetColorMax(m_RGBVMode, nNextPos);	
	if ( rgbvPtr->GetColorEnabled(m_RGBVMode) == false )
	{
		switch ( m_RGBVMode )
		{
		case COLOR_RGBV_RED: CWnd::CheckDlgButton(PROCLR_RED_ENABLED_CHK, TRUE); 	break;
		case COLOR_RGBV_GREEN: CWnd::CheckDlgButton(PROCLR_GREEN_ENABLED_CHK, TRUE); 	break;
		case COLOR_RGBV_BLUE: CWnd::CheckDlgButton(PROCLR_BLUE_ENABLED_CHK, TRUE); 	break;
		}
		rgbvPtr->SetColorEnabled(m_RGBVMode, true);
		UpdateColorListWnd();
	}
	BuildColorFilterImage(rgbvPtr);
	m_RGBVWnd.ReDrawWnd();
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::OnColorMinBtn() 
{
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }

	CString   str;	
	int       nNextPos = COLOR_RGBV_MIN;
	CColorRGBV *rgbvPtr=m_RGBVWnd.GetColorRGBVPtr();
	if ( NULL == rgbvPtr ) { return; }
	m_ColorMinSpin.SetPos(nNextPos);	
	str.Format(_T("%d"), nNextPos);
	CWnd::SetDlgItemText(PROCLR_COLOR_MIN_EDIT, str);
	rgbvPtr->SetColorMin(m_RGBVMode, nNextPos);	
	if ( rgbvPtr->GetColorEnabled(m_RGBVMode) == false )
	{
		switch ( m_RGBVMode )
		{
		case COLOR_RGBV_RED: CWnd::CheckDlgButton(PROCLR_RED_ENABLED_CHK, TRUE); 	break;
		case COLOR_RGBV_GREEN: CWnd::CheckDlgButton(PROCLR_GREEN_ENABLED_CHK, TRUE); 	break;
		case COLOR_RGBV_BLUE: CWnd::CheckDlgButton(PROCLR_BLUE_ENABLED_CHK, TRUE); 	break;
		}
		rgbvPtr->SetColorEnabled(m_RGBVMode, true);
		UpdateColorListWnd();
	}
	BuildColorFilterImage(rgbvPtr);
	m_RGBVWnd.ReDrawWnd();	
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::OnValueMaxBtn() 
{
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }

	CString   str;	
	int       nNextPos = COLOR_RGBV_MAX;
	CColorRGBV *rgbvPtr=m_RGBVWnd.GetColorRGBVPtr();
	if ( NULL == rgbvPtr ) { return; }
	m_ValueMaxSpin.SetPos(nNextPos);	
	str.Format(_T("%d"), nNextPos);
	CWnd::SetDlgItemText(PROCLR_VALUE_MAX_EDIT, str);
	rgbvPtr->SetValueMax(nNextPos);
	if ( rgbvPtr->GetValueEnabled() == false )
	{
		CWnd::CheckDlgButton(PROCLR_VALUE_ENABLED_CHK, TRUE);
		rgbvPtr->SetValueEnabled(true);
		UpdateColorListWnd();
	}
	BuildColorFilterImage(rgbvPtr);
	m_RGBVWnd.ReDrawWnd();
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::OnValueMinBtn() 
{
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }

	CString   str;
	int       nNextPos = COLOR_RGBV_MIN;
	CColorRGBV *rgbvPtr=m_RGBVWnd.GetColorRGBVPtr();
	if ( NULL == rgbvPtr ) { return; }	
	m_ValueMinSpin.SetPos(nNextPos);
	str.Format(_T("%d"), nNextPos);
	CWnd::SetDlgItemText(PROCLR_VALUE_MIN_EDIT, str);
	rgbvPtr->SetValueMin(nNextPos);	
	if ( rgbvPtr->GetValueEnabled() == false )
	{
		CWnd::CheckDlgButton(PROCLR_VALUE_ENABLED_CHK, TRUE);
		rgbvPtr->SetValueEnabled(true);
		UpdateColorListWnd();
	}
	BuildColorFilterImage(rgbvPtr);
	m_RGBVWnd.ReDrawWnd();
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::OnResetColorBtn() 
{
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	if ( CheckColorGroupValid() == false ) { return ; }
	size_t i = 0;	
	CColorRGBV   *rgbvPtr=NULL;			
	const int   rgbvIndex = m_ColorListCtrl.GetNextItem(-1, LVNI_SELECTED);	
	rgbvPtr = m_ColorGroup.GetColorGroupColorPtr(rgbvIndex, true);
	if ( NULL == rgbvPtr ) { return ; }

	rgbvPtr->ResetColor();
	UpdateColorListWnd();
	UpdateRGBVToUI(rgbvPtr);
	BuildColorFilterImage(rgbvPtr);
	m_RGBVWnd.SetColorRGBVPtr(rgbvPtr);
	m_RGBVWnd.ReDrawWnd();
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::OnResetAllBtn() 
{
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	if ( CheckColorGroupValid() == false ) { return ; }
	CString str;
	str = _T("Do you want to clear all colors?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO) 
	{	return;	}

	size_t i = 0;	
	CColorRGBV   *rgbvPtr=NULL;		
	const int     rgbvIndex = 0;		
	m_ColorGroup.ResetColorGroupColorList();	
	UpdateColorListWnd();	
	if ( m_ColorListCtrl.GetItemCount() > rgbvIndex )
	{	m_ColorListCtrl.SetItemState(rgbvIndex, TVIS_SELECTED, TVIS_SELECTED);	}
	rgbvPtr = m_ColorGroup.GetColorGroupColorPtr(rgbvIndex, true);
	UpdateRGBVToUI(rgbvPtr);	
	BuildColorFilterImage(rgbvPtr);	
	m_RGBVWnd.SetColorRGBVPtr(rgbvPtr);
	m_RGBVWnd.ReDrawWnd();	
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::OnMergeAllBtn() 
{
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	if ( CheckColorGroupValid() == false ) { return ; }
	size_t i = 0;	
	CColorRGBV   *rgbvPtr=NULL;		
	const int rgbvIndex = 0;	
	m_ColorGroup.MergeColorGroupColor();
	UpdateColorListWnd();	
	if ( m_ColorListCtrl.GetItemCount() > rgbvIndex )
	{	m_ColorListCtrl.SetItemState(rgbvIndex, TVIS_SELECTED, TVIS_SELECTED);	}

	rgbvPtr = m_ColorGroup.GetColorGroupColorPtr(rgbvIndex, true);
	UpdateRGBVToUI(rgbvPtr);	
	BuildColorFilterImage(NULL);
	m_RGBVWnd.SetColorRGBVPtr(rgbvPtr);
	m_RGBVWnd.ReDrawWnd();	
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::OnGatherColorChk() 
{
	// TODO: Add your control notification handler code here	
	BOOL bChk = CWnd::IsDlgButtonChecked(PROCLR_GATHER_COLOR_CHK);	
	m_ImageWnd.ResetLBtnPos();
	m_ImageWnd.SetShowLBtnPos(bChk);	
	m_ImageWnd.RedrawWnd(FALSE);
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::RedrawWnd()
{
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::CreateBKImage()
{
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::UpdateEditValue()
{
	CColorRGBV *rgbvPtr = m_RGBVWnd.GetColorRGBVPtr();
	UpdateRGBVToParam(rgbvPtr);	
	UpdateRGBVToUI(rgbvPtr);
	UpdateColorListWnd();
	BuildColorFilterImage(rgbvPtr);
	return;
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::SwitchRGBMasterMode()
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }

	int RGBVMode = m_RGBVWnd.GetRGBVMode();
	int RGBVMode_Next = RGBVMode;
	switch ( RGBVMode )
	{
	case COLOR_RGBV_RED:	RGBVMode_Next = COLOR_RGBV_GREEN;	break;
	case COLOR_RGBV_GREEN:	RGBVMode_Next = COLOR_RGBV_BLUE;	break;
	case COLOR_RGBV_BLUE:	RGBVMode_Next = COLOR_RGBV_RED;	break;
	}

	if ( RGBVMode_Next == RGBVMode ) { return; }
	switch ( RGBVMode_Next )
	{
	case COLOR_RGBV_RED:	OnRedMasterChk();		break;
	case COLOR_RGBV_GREEN:	OnGreenMasterChk();	break;
	case COLOR_RGBV_BLUE:	OnBlueMasterChk();	break;
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::UpdateRGBVToUI(CColorRGBV *rgbvPtr)
{
	if ( NULL == rgbvPtr ) { return; }
	CString str;	
	int     nValue = 0;
	int     nMax=COLOR_RGBV_MAX;
	int     nMin=COLOR_RGBV_MIN;
	BOOL    bCheck = FALSE; 	

	m_ColorGroup.SetColorGroupActiveColorPtr(rgbvPtr);

	if ( rgbvPtr->GetRedEnabled() == true ) { bCheck = TRUE; }
	else { bCheck = FALSE; }
	CWnd::CheckDlgButton(PROCLR_RED_ENABLED_CHK, bCheck);

	if ( rgbvPtr->GetGreenEnabled() == true ) { bCheck = TRUE; }
	else { bCheck = FALSE; }
	CWnd::CheckDlgButton(PROCLR_GREEN_ENABLED_CHK, bCheck);

	if ( rgbvPtr->GetBlueEnabled() == true ) { bCheck = TRUE; }
	else { bCheck = FALSE; }
	CWnd::CheckDlgButton(PROCLR_BLUE_ENABLED_CHK, bCheck);

	if ( rgbvPtr->GetValueEnabled() == true ) { bCheck = TRUE; }
	else { bCheck = FALSE; }
	CWnd::CheckDlgButton(PROCLR_VALUE_ENABLED_CHK, bCheck);

	nMax = rgbvPtr->GetColorMax(m_RGBVMode);
	nMin = rgbvPtr->GetColorMin(m_RGBVMode);

	m_ColorMaxSpin.SetPos(nMax);
	m_ColorMinSpin.SetPos(nMin);
	str.Format(_T("%d"), nMax);
	CWnd::SetDlgItemText(PROCLR_COLOR_MAX_EDIT, str);
	str.Format(_T("%d"), nMin);
	CWnd::SetDlgItemText(PROCLR_COLOR_MIN_EDIT, str);

	nMax = rgbvPtr->GetValueMax();
	nMin = rgbvPtr->GetValueMin();	
	m_ValueMaxSpin.SetPos(nMax);
	m_ValueMinSpin.SetPos(nMin);
	str.Format(_T("%d"), nMax);
	CWnd::SetDlgItemText(PROCLR_VALUE_MAX_EDIT, str);
	str.Format(_T("%d"), nMin);
	CWnd::SetDlgItemText(PROCLR_VALUE_MIN_EDIT, str);
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::UpdateRGBVToParam(CColorRGBV *rgbvPtr)
{
	if ( NULL == rgbvPtr ) { return; }
	CString str;
	bool    bEnabled = false;
	int     nValue1 = 0;
	int     nValue2 = 0;
	int     nMax=255, nMin=0;
	BOOL    bCheck = FALSE; 
	
	if ( CWnd::IsDlgButtonChecked(PROCLR_RED_ENABLED_CHK) == TRUE ) { bEnabled = true; }
	else { bEnabled = false; }
	rgbvPtr->SetRedEnabled(bEnabled);

	if ( CWnd::IsDlgButtonChecked(PROCLR_GREEN_ENABLED_CHK) == TRUE ) { bEnabled = true; }
	else { bEnabled = false; }
	rgbvPtr->SetGreenEnabled(bEnabled);

	if ( CWnd::IsDlgButtonChecked(PROCLR_BLUE_ENABLED_CHK) == TRUE ) { bEnabled = true; }
	else { bEnabled = false; }
	rgbvPtr->SetBlueEnabled(bEnabled);

	if ( CWnd::IsDlgButtonChecked(PROCLR_VALUE_ENABLED_CHK) == TRUE ) { bEnabled = true; }
	else { bEnabled = false; }
	rgbvPtr->SetValueEnabled(bEnabled);
	
	CWnd::GetDlgItemText(PROCLR_COLOR_MAX_EDIT, str);
	nValue1 = ::_ttoi(str);
	CWnd::GetDlgItemText(PROCLR_COLOR_MIN_EDIT, str);
	nValue2 = ::_ttoi(str);
	nMax = MAX(nValue1, nValue2);
	nMin = MIN(nValue1, nValue2);

	m_ColorMaxSpin.SetPos(nMax);
	m_ColorMinSpin.SetPos(nMin);
	rgbvPtr->SetColorMax(m_RGBVMode, nMax);
	rgbvPtr->SetColorMin(m_RGBVMode, nMin);		

	CWnd::GetDlgItemText(PROCLR_VALUE_MAX_EDIT, str);
	nValue1 = ::_ttoi(str);
	CWnd::GetDlgItemText(PROCLR_VALUE_MIN_EDIT, str);
	nValue2 = ::_ttoi(str);
	nMax = MAX(nValue1, nValue2);
	nMin = MIN(nValue1, nValue2);
	m_ValueMaxSpin.SetPos(nMax);
	m_ValueMinSpin.SetPos(nMin);
	rgbvPtr->SetValueMax(nMax);
	rgbvPtr->SetValueMin(nMin);	
	m_RGBVWnd.ReDrawWnd();
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::CalcColorFilterParam()//計算抽色參數
{
	UINT CtrlID = PROCLR_GATHER_COLOR_CHK;
	BOOL bChk = CWnd::IsDlgButtonChecked(CtrlID);
	if ( FALSE == bChk ) { return; }
	if ( CheckColorGroupValid() == false ) { return; }
	if ( NULL == m_RawImagePtr ) { return; }
	if ( 24 != m_ShowBitCount ) { return; }
	CColorRGBV *rgbvPtr = m_RGBVWnd.GetColorRGBVPtr();
	if ( NULL == rgbvPtr ) { return; }	
	const bool CombineColorMode = AOIDataCollect.CheckCombineColorMode();

	CColorRGBV rgbv;
	RECT       RoiRect={0};
	m_ImageWnd.GetImageEditRect(RoiRect);	
	if ( ImageAPI.CalcColorImageColorFilter(m_ShowImageW, m_ShowImageH, m_ShowImageStep, m_RawImagePtr, RoiRect, rgbv) == false ) 
	{	return; }
	
	rgbv.ExpandColorRGBV(5, false);
	rgbvPtr->CheckUsed();
	rgbvPtr->CalcShowColor();	
	if ( false==CombineColorMode || rgbvPtr->CheckUsed()==false )
	{	rgbvPtr->CopyColorRGBV(rgbv);	}
	else
	{	rgbvPtr->MergeColor(false, &rgbv);	}
	rgbvPtr->CheckUsed();	
	rgbvPtr->CalcShowColor();
	m_ShowColorGroup = false;
	UpdateRGBVToUI(rgbvPtr);
	UpdateColorListWnd();
	BuildColorFilterImage(rgbvPtr);
	m_RGBVWnd.ReDrawWnd();
	return;
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::BuildColorFilterImage(CColorRGBV *rgbvPtr)
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }	
	CColorGroup TempColorGroup;	
	if ( NULL==rgbvPtr )
	{	
		m_ColorGroup.UpdateColorGroupUsed();	
		m_ColorGroup.UpdateColorGroupShowColor();
		TempColorGroup = m_ColorGroup;
	}
	else
	{	
		rgbvPtr->CheckUsed();	
		rgbvPtr->CalcShowColor();
		const size_t Count = m_ColorGroupList.size();
		const size_t Index = m_ColorGroup.GetColorGroupIndex();
		if ( Index < Count )
		{	m_ColorGroupList[Index] = m_ColorGroup;	}

		if ( false == m_ShowColorGroup )
		{
			CColorRGBV  rgbv = *rgbvPtr;
			rgbv.SetLogicMode(COLOR_LOGIC_INCLUDE);
			TempColorGroup.AddColorGroupColor(rgbv);
		}
		else
		{	TempColorGroup = m_ColorGroup; }
	}	 
	
	if ( NULL==m_RawImagePtr || NULL==m_ShowImagePtr )
	{	return; }
	
	CString    str;
	RECT       RoiRect={0};	
	MASK_PTR   MaskPtr=NULL;
	IMAGE_SIZE MaskBitCount=8;
	const bool bOpenMP = true;
	IMAGE_SIZE MaskStep=JetAPI::GetBMPImagePixelsPerLine(m_ShowImageW, MaskBitCount, 4);

	double          fnTime=0;
	LARGE_INTEGER   fnEnd;
	LARGE_INTEGER   fnStart;
	
	JetAPI::SetFuncTimeStart(fnStart);
	m_ImageWnd.CalcImageShowRect(RoiRect);	
	JetAPI::BoundaryRect(m_ShowImageW, m_ShowImageH, RoiRect);
	if ( ImageAPI.ColorImageColorFilter(m_ShowImageW, m_ShowImageH, m_ShowImageStep, m_RawImagePtr, TempColorGroup, RoiRect, MaskStep, MaskPtr, true, bOpenMP) == false )
	{	return ; }	
	JetAPI::SetFuncTimeStart(fnEnd);
	fnTime = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);//計算函式經過時間
	str.Format(_T("ImageAPI::ColorImageColorFilter Time=%.3f ms"), fnTime);
	AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);
	/*
	//測試比較		
	const size_t MaskBufferSize = MaskStep*m_ShowImageH;
	::memset(m_ShowImagePtr, 0xFF, sizeof(MASK_DATA)*MaskBufferSize);//因為只有部分遮罩計算, 所以要新初始化這個記憶體
	if ( ImageAPI.ColorImageColorFilter3(m_ShowImageW, m_ShowImageH, m_ShowImageStep, m_RawImagePtr, TempColorGroup, RoiRect, MaskStep, m_ShowImagePtr, true, false) == false )
	{	return ; }
	size_t i=0;
	size_t Cnt=0;	
	for ( i=0; i<MaskBufferSize; i++ )
	{
		if ( MaskPtr[i] != m_ShowImagePtr[i] )
		{	Cnt ++;  }
	}
	str.Format(_T("ImageAPI::ColorImageColorFilter Check Dif Cnt=%d"), Cnt);
	AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);
	*/

	MASK_DATA  mask = 0xFF;
	IMAGE_DATA mskR=0xFF, mskG=0xFF, mskB=0x00, mskV=0x00, Alpha=0;	
	unsigned int MapIndex = ProjectPtr->GetProjectMapIndex();
	unsigned int FrameUniqueID = ProjectPtr->GetProjectFrameUniqueID(MapIndex, true);	
	AOIDataCollect.GetMaskImageColor(FrameUniqueID, mskR, mskG, mskB, mskV, Alpha);

	JetAPI::SetFuncTimeStart(fnStart);
	AOIDataCollect.ExecEnhanceDisplayImage(m_ShowImageW, m_ShowImageH, m_ShowImageStep, m_ShowBitCount, m_RawImagePtr, m_ShowImagePtr);
	ImageAPI.ColorImageApplyMask(m_ShowImageW, m_ShowImageH, m_ShowImageStep, m_ShowImagePtr, RoiRect, MaskStep, MaskPtr, mask, mskR, mskG, mskB, Alpha);
	JetAPI::SetFuncTimeStart(fnEnd);
	fnTime = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);//計算函式經過時間
	str.Format(_T("ImageAPI::ColorImageApplyMask Time=%.3f ms"), fnTime);
	AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);

	JetMemory.free_func(MaskPtr);

	//m_ImageWnd.SetImageBuffer(m_ShowImageW, m_ShowImageH, m_ShowImageStep, m_ShowBitCount, m_ShowImagePtr, true, false);
	m_ImageWnd.SetImageRawBuffer(m_ShowImageW, m_ShowImageH, m_ShowImageStep, m_ShowBitCount, m_ShowImagePtr, m_RawImagePtr, true, false);	
	//m_ImageWnd.ShowFittedZoom();
	m_ImageWnd.RedrawWnd(FALSE);
}
//-------------------------------------------------------------------------------------//
bool CProjectColorWnd::LockUIWnd(bool bLock)
{
	AOIDataCollect.SetIsLockUIWnd(bLock);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectColorWnd::GetLockUIWnd() const//取得是否鎖住UIWnd
{
	return AOIDataCollect.GetIsLockUIWnd();
}
//-------------------------------------------------------------------------------------//
unsigned int CProjectColorWnd::GetMaxFrameCount() const
{
	return FRAME_MAX_COUNT;
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::PreInitUniFrameBuffer()//預先影像記憶體
{
	int i=0;	
	for ( i=0; i<FRAME_MAX_COUNT; i++ )
	{	JetAPI::InitialUniFrame(m_UniFrameList[i]);	}	
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::ReleaseUniFrameBuffer()//釋放影像記憶體	
{
	JetAPI::ClearUniFrameList(m_UniFrameList, FRAME_MAX_COUNT);
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::BuildShowImageBuffer(bool ResetView)//建立顯示的影像記憶體
{
	const char fnName[] = "CProjectColorWnd::BuildShowImageBuffer";
	ReleaseShowImageBuffer();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }

	size_t       i=0;	
	TUNI_FRAME   UniFrame;
	IMAGE_PTR    ImagePtr = NULL;
	IMAGE_SIZE   ImageW=0, ImageH=0, ImageStep=0, BitCount=0;	
	unsigned int MapIndex = ProjectPtr->GetProjectMapIndex();	
	const size_t MaxUniFrameCount = GetMaxFrameCount();
	if ( MapIndex >= MaxUniFrameCount ) { MapIndex = 0; }
	if ( NULL == m_UniFrameList[MapIndex].ImagePtr )
	{	MapIndex = 0;	}	

	UniFrame = m_UniFrameList[MapIndex];
	ImageW    = UniFrame.ImageW;
	ImageH    = UniFrame.ImageH;
	ImageStep = UniFrame.ImageStep;
	BitCount  = UniFrame.BitCount;
	ImagePtr  = UniFrame.ImagePtr;

	if ( NULL == ImagePtr )
	{
		const size_t ShowBufferSize = ImageAPI.CalcBufferSize(m_ShowImageStep, m_ShowImageH);
		if ( JetMemory.alloc_func(ShowBufferSize, m_RawImagePtr, fnName, "m_RawImagePtr") == false || 
			JetMemory.alloc_func(ShowBufferSize, m_ShowImagePtr, fnName, "m_ShowImagePtr") == false )
		{
			ReleaseShowImageBuffer();
			return;		
		}
		::memset(m_RawImagePtr, 0x00, sizeof(IMAGE_DATA)*ShowBufferSize);
		::memset(m_ShowImagePtr, 0x00, sizeof(IMAGE_DATA)*ShowBufferSize);
	}
	else
	{		
		m_ShowBitCount = 24;
		m_ShowImageW = ImageW;
		m_ShowImageH = ImageH;
		m_ShowImageStep = JetAPI::GetBMPImagePixelsPerLine(ImageW, m_ShowBitCount, 4);
		const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
		const size_t ShowBufferSize = ImageAPI.CalcBufferSize(m_ShowImageStep, m_ShowImageH);		
		if ( JetMemory.alloc_func(ShowBufferSize, m_RawImagePtr, fnName, "m_RawImagePtr") == false || 
			JetMemory.alloc_func(ShowBufferSize, m_ShowImagePtr, fnName, "m_ShowImagePtr") == false )
		{
			ReleaseShowImageBuffer();
			return;		
		}
		if ( ShowBufferSize == BufferSize )
		{	
			::memcpy(m_RawImagePtr, ImagePtr, sizeof(IMAGE_DATA)*BufferSize);	
			::memcpy(m_ShowImagePtr, ImagePtr, sizeof(IMAGE_DATA)*BufferSize);	
		}
		else
		{
			if ( ImageAPI.RGBImageToColorImage3(ImageW, ImageH, ImageStep, ImagePtr, ImagePtr, ImagePtr, m_ShowImageStep, m_RawImagePtr, false) == false )
			{
				ReleaseShowImageBuffer();
				return;
			}
			::memcpy(m_ShowImagePtr, m_RawImagePtr, sizeof(IMAGE_DATA)*ShowBufferSize);				
		}
	}
	
	CString      FrameName;
	unsigned int FrameUniqueID = ProjectPtr->GetProjectFrameUniqueID(MapIndex, true);	
	TFrameParam *FrameParamPtr = AOIDataCollect.GetSystemFrameParamPtrByUniqueID(FrameUniqueID);
	if ( NULL != FrameParamPtr )
	{	FrameName = FrameParamPtr->FrameName;	}

	AOIDataCollect.ExecEnhanceDisplayImage(m_ShowImageW, m_ShowImageH, m_ShowImageStep, m_ShowBitCount, m_RawImagePtr, m_ShowImagePtr);
	CAMERA_ID CameraID = PRIMARY_CAMERA_ID;
	m_ImageWnd.SetImageText(FrameName, true);
	m_ImageWnd.SetImageInfo(CameraID, m_FrameStageRgn, m_FrameResolution, IMAGE_DATA_FOV);
	//m_ImageWnd.SetImageBuffer(m_ShowImageW, m_ShowImageH, m_ShowImageStep, m_ShowBitCount, m_ShowImagePtr, true, ResetView);
	m_ImageWnd.SetImageRawBuffer(m_ShowImageW, m_ShowImageH, m_ShowImageStep, m_ShowBitCount, m_ShowImagePtr, m_RawImagePtr, true, ResetView);	
	//m_ImageWnd.ShowFittedZoom();
	m_ImageWnd.RedrawWnd(FALSE);
	return;
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::ReleaseShowImageBuffer()//釋放顯示影像記憶體	
{	
	if ( NULL != m_RawImagePtr )
	{	JetMemory.free_func(m_RawImagePtr); }
	if ( NULL != m_ShowImagePtr )
	{	JetMemory.free_func(m_ShowImagePtr); }
	m_ShowImageW = 1024;
	m_ShowImageH = 1024;
	m_ShowImageStep = 1024*3;
	m_ShowBitCount = 24;	
}
//-------------------------------------------------------------------------------------//
bool CProjectColorWnd::FillCurrentFrames(double Ratio)
{
	size_t   i=0;
	TSIZE2D  Res;
	TPOINT3D Pos;	
	CAMERA_ID CameraID = PRIMARY_CAMERA_ID;
	const size_t MaxFrames = GetMaxFrameCount();	
	IMAGE_SIZE ImageW = AOIDataCollect.GetCameraImageW(CameraID);
	IMAGE_SIZE ImageH = AOIDataCollect.GetCameraImageH(CameraID);
	const double FOVWum = Ratio*AOIDataCollect.GetFovSizeRealW();
	const double FOVHum = Ratio*AOIDataCollect.GetFovSizeRealH();		

	Res.cx = AOIDataCollect.GetCameraResolutionX(CameraID);
	Res.cy = AOIDataCollect.GetCameraResolutionY(CameraID);
	AOIDataCollect.GetStagePos(Pos.x, Pos.y, Pos.z);

	m_FrameResolution.x = Res.cx;
	m_FrameResolution.y = Res.cy;
	m_FrameStageRgn.SetRgn(Pos.x, Pos.y, FOVWum, FOVHum);

	ReleaseUniFrameBuffer();
	ReleaseShowImageBuffer();

	double FovRatio = Ratio;
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr )
	{	return true; }

	HCURSOR hCursor=NULL;
	HCURSOR hOldCursor=NULL;
	CWinApp *AppPtr = ::AfxGetApp();
	if ( NULL != AppPtr )
	{	
		hCursor = AppPtr->LoadStandardCursor(IDC_WAIT); 
		hOldCursor = ::SetCursor(hCursor);
	}		
		
	ImageW = (IMAGE_SIZE)(ImageW*Ratio);
	ImageH = (IMAGE_SIZE)(ImageH*Ratio);
	OFFLINE_FILE_MODE OfflineFileMode = ProjectPtr->GetProjectOfflineFileMode();
	if ( ProjectPtr->FillCurrentFrame(OfflineFileMode, Pos, Res, ImageW, ImageH, m_UniFrameList, MaxFrames) == false )
	{	JetAPI::ShowMessageBox(ProjectPtr->GetErrorString());	}
	
	unsigned int ImageIndex = ProjectPtr->GetProjectMapIndex();
	for ( i=0; i<FRAME_MAX_COUNT; i++ )
	{
		if ( NULL == m_UniFrameList[i].ImagePtr ) { continue; }		
		m_UniFrameList[i].ImageW = ImageW;
		m_UniFrameList[i].ImageH = ImageH;
	}
	AOIDataCollect.SetFieldUniFrameList(m_FrameStageRgn, m_UniFrameList, MaxFrames);	
	if ( NULL != hOldCursor )
	{	::SetCursor(hOldCursor);	}

	m_ShowImageW = ImageW;
	m_ShowImageH = ImageH;
	m_ShowBitCount = 24;	
	m_ShowImageStep = JetAPI::GetBMPImagePixelsPerLine(m_ShowImageW, m_ShowBitCount, 4);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectColorWnd::ExecMoveToStage()
{
	if ( MotionCtrlPtr->WaitForMotionStop() == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return false;
	}
	
	bool   IsOK = true;
	double PosX=0, PosY=0, PosZ=0;
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();	
	IsOK = MotionCtrlPtr->GetCurrentPos(PosX, PosY, PosZ, OfflineMode);
	if ( IsOK == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return false;
	}	
	if ( true == OfflineMode )
	{	return ExecUpdateFov(PosX, PosY, PosZ);	}
	return ExecGrabFov(PosX, PosY, PosZ);		
}
//-------------------------------------------------------------------------------------//
bool CProjectColorWnd::ExecShowWndPosition()
{	
	const double PosX = AOIDataCollect.GetFovPositionX();
	const double PosY = AOIDataCollect.GetFovPositionY();	
	const double TargetOffsetX = AOIDataCollect.GetFovTargetOffsetX();
	const double TargetOffsetY = AOIDataCollect.GetFovTargetOffsetY();

	AOIDataCollect.ResetFovTargetParam();
	
	BuildShowImageBuffer(true);
	CreateBKImage();
	RedrawWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectColorWnd::ExecGrabFov(double PosX, double PosY, double PosZ)
{
	CString str;
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }		
	
	HWND hWnd = CWnd::GetSafeHwnd();
	AOIDataCollect.SetCallbackWnd(hWnd);	
	//AOIDataCollect.MoveCameraToProjectFocusPos(ProjectPtr);
#ifndef LIGHT_CTRL_DISABLE
	this->LockUIWnd(true);	
	if ( AOIDataCollect.ExecGrabNextUniFrameImage() == false )
	{		
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		this->LockUIWnd(false);
		return FALSE;
	}	
	return true;
#endif//LIGHT_CTRL_DISABLE
	return false;
}
//-------------------------------------------------------------------------------------//
bool CProjectColorWnd::ExecUpdateFov(double PosX, double PosY, double PosZ)
{
	const double FovSizeW = AOIDataCollect.GetFovSizeRealW();
	const double FovSizeH = AOIDataCollect.GetFovSizeRealH();	
	const double FovMinW = AOIDataCollect.GetFovSizeMinW_Zoom();
	const double FovMinH = AOIDataCollect.GetFovSizeMinH_Zoom();	
	const double TargetMinW = AOIDataCollect.GetTargetMinSizeW_Zoom();
	const double TargetMinH = AOIDataCollect.GetTargetMinSizeH_Zoom();
	const double FovSizeWd2 = FovSizeW/2;
	const double FovSizeHd2 = FovSizeH/2;
	const double FovSizeWd4 = FovSizeW/4;
	const double FovSizeHd4 = FovSizeH/4;
	const double ZoomMin = AOIDataCollect.GetImageZoomMin();
	const double ZoomMax = AOIDataCollect.GetImageZoomMax();
	const double TargetOffsetX = AOIDataCollect.GetFovTargetOffsetX();
	const double TargetOffsetY = AOIDataCollect.GetFovTargetOffsetY();
	const double FovZoomX = FovMinW/FovSizeW;
	const double FovZoomY = FovMinH/FovSizeH;
	const double FovZoomNeed = MAX(FovZoomX, FovZoomY);	
	const double FovZoomNeedUsed = JetAPI::AdjustValue(FovZoomNeed, 0.5);

	TPOINT2D ImageRes;
	IMAGE_SIZE ImageW = 0;
	IMAGE_SIZE ImageH = 0;
	IMAGE_SIZE ImageStep = 0;
	IMAGE_SIZE BitCount = 0;
	IMAGE_PTR  ImagePtr = NULL;
	MASK_PTR   MaskPtr=NULL;
	SPACE_PTR  SpacePtr=NULL;
	
	ImageRes.x = AOIDataCollect.GetCameraResolutionX(PRIMARY_CAMERA_ID);
	ImageRes.y = AOIDataCollect.GetCameraResolutionY(PRIMARY_CAMERA_ID);	

	AOIDataCollect.ResetFovTargetParam();
	
	double Ratio = MAX(1.0, FovZoomNeedUsed);
	FillCurrentFrames(Ratio);

	BuildShowImageBuffer(true);	
	BuildColorFilterImage(m_RGBVWnd.GetColorRGBVPtr());
	CreateBKImage();
	RedrawWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectColorWnd::RetrieveCameraImage(WPARAM wParam, LPARAM lParam, bool bCameraCallBack)//取得相機影像
{
	CString str;	
	size_t   i=0;	
	TSIZE2D  Res;
	TPOINT3D Pos;
	IMAGE_SIZE ImageStep=0;
	IMAGE_SIZE BitCount=0;
	size_t   BuffserSize=0;
	CAMERA_ID CameraID = (CAMERA_ID)(wParam);	
	if ( PRIMARY_CAMERA_ID != CameraID ) { return false; }
	if ( LPARAM_CAMERA_BYPASS_CALLBACK == lParam )
	{	return true;	}		
	
	bool bReturn=false;
	if ( CameraCtrl.RetrieveCameraImageCallback(CameraID, bReturn) == false )
	{
		JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());		
		return false; 
	}
	if ( true == bReturn )
	{	return true; }

	LockUIWnd(false);	
	ReleaseUniFrameBuffer();
	ReleaseShowImageBuffer();

	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }

	const double Ratio = 1.0;
	double PosX=0, PosY=0, PosZ=0;
	IMAGE_SIZE ImageW = AOIDataCollect.GetCameraImageW(CameraID);
	IMAGE_SIZE ImageH = AOIDataCollect.GetCameraImageH(CameraID);	
	double     ImageResX = AOIDataCollect.GetCameraResolutionX(CameraID);
	double     ImageResY = AOIDataCollect.GetCameraResolutionY(CameraID);	
	const double FOVWum = Ratio*AOIDataCollect.GetFovSizeRealW();
	const double FOVHum = Ratio*AOIDataCollect.GetFovSizeRealH();

	MotionCtrlPtr->GetCurrentPos(PosX, PosY, PosZ);		
	m_FrameResolution.x = ImageResX;
	m_FrameResolution.y = ImageResY;
	m_FrameStageRgn.SetRgn(PosX, PosY, FOVWum, FOVHum);	

	std::vector<TUNI_FRAME>   UniFrameList;
	std::vector<TFrameParam>  GrabFrameParamList;//影像參數列表			
	AOIDataCollect.GetGrabFrameParamList(GrabFrameParamList);
	const size_t GrabFrameParamCount = GrabFrameParamList.size();
	if ( 0 == GrabFrameParamCount ) { return false; }

	if ( true == bCameraCallBack )
	{	AOIDataCollect.ModifyCameraUniFrame(GrabFrameParamList);	}
	if ( AOIDataCollect.RetrieveCameraUniFrame(GrabFrameParamList, UniFrameList) == false )
	{	
		JetAPI::ClearUniFrameList(UniFrameList);
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());		
		return false;
	}
	if ( NULL == ProjectPtr )
	{	m_ImageIndex = 0; }
	else
	{	m_ImageIndex = ProjectPtr->GetProjectMapIndex(); }	
	const size_t UniFrameCount = UniFrameList.size();
	const size_t MinUniFrameCount = MIN(FRAME_MAX_COUNT, UniFrameCount);
	for ( i=0; i<MinUniFrameCount; i++ )
	{	m_UniFrameList[i] = UniFrameList[i];	}
	for ( i=MinUniFrameCount; i<UniFrameCount; i++ )
	{	JetAPI::ClearUniFrame(UniFrameList[i]);	}
	BuildShowImageBuffer(bCameraCallBack);	
	BuildColorFilterImage(m_RGBVWnd.GetColorRGBVPtr());
	CreateBKImage();
	RedrawWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::OnShowMapBtn() 
{
	// TODO: Add your control notification handler code here
	if ( m_ProjectMapWnd.GetSafeHwnd() == NULL ) { return; }
	m_ProjectMapWnd.ShowWindow(SW_SHOW);
}
//-------------------------------------------------------------------------------------//
BOOL CProjectColorWnd::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	bool bResturn=false;
	switch ( pMsg->message )
	{
	case WM_KEYDOWN:
		switch ( pMsg->wParam )
		{
		case VK_ESCAPE:
			bResturn = true;
			break;
		case VK_RETURN:			
			bResturn = true;
			UpdateEditValue();
			break;
		case VK_ENHANCE_IMAGE:
			AOIDataCollect.ToggleIsEnhanceDisplayImage();
			break;
		}
		if ( true == bResturn )
		{	return TRUE; }
		break;
	}
	return CBaseDialog::PreTranslateMessage(pMsg);
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::OnColorGroupBtnPad() 
{
	// TODO: Add your control notification handler code here
	m_ColorGroupCtrlID = PROCLR_COLOR_GROUP_BTN_PAD;
	BuildColorGroupListWnd();
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::OnColorGroupBtnVoid() 
{
	// TODO: Add your control notification handler code here
	m_ColorGroupCtrlID = PROCLR_COLOR_GROUP_BTN_VOID;
	BuildColorGroupListWnd();
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::OnColorGroupBtnBody() 
{
	// TODO: Add your control notification handler code here
	m_ColorGroupCtrlID = PROCLR_COLOR_GROUP_BTN_BODY;
	BuildColorGroupListWnd();
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::OnColorGroupBtnBoard() 
{
	// TODO: Add your control notification handler code here
	m_ColorGroupCtrlID = PROCLR_COLOR_GROUP_BTN_BOARD;
	BuildColorGroupListWnd();
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::OnColorGroupBtnSolder() 
{
	// TODO: Add your control notification handler code here
	m_ColorGroupCtrlID = PROCLR_COLOR_GROUP_BTN_SOLDER;
	BuildColorGroupListWnd();
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::OnColorGroupBtnOthers() 
{
	// TODO: Add your control notification handler code here
	m_ColorGroupCtrlID = PROCLR_COLOR_GROUP_BTN_OTHERS;
	BuildColorGroupListWnd();
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::OnClickColorGroupListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here	
	m_ShowColorGroup = true;
	BuildColorFilterImage(NULL);	
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::OnClickColorListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CColorRGBV *rgbvPtr = m_RGBVWnd.GetColorRGBVPtr();
	if ( NULL == rgbvPtr ) { return; }
	if ( rgbvPtr->CheckUsed() == true )
	{	m_ShowColorGroup = false;	}
	else
	{	m_ShowColorGroup = true;	}
	BuildColorFilterImage(rgbvPtr);

	if ( rgbvPtr->GetUsed() == false )
	{
		CWnd::CheckDlgButton(PROCLR_GATHER_COLOR_CHK, TRUE);
		OnGatherColorChk();
	}
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::OnSendSystemColorBtn() 
{
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	bool IsLockSystemParam = AOIDataCollect.CheckIsLockSystemParameter();	
	if ( true == IsLockSystemParam )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return; 
	}

	int             i=0;	
	TListNode       Node;
	CString         str;
	CString         strName;
	CString         strLabel;
	CString         strValue;
	CString         strCaption;
	CInputListWnd   EnumWnd;
	CColorGroupSet *ColorGroupSetPtr=NULL;
	std::vector<TListNode> NodelList;		
	const int SystemColorGroupSetCount = (int)(AOIDataCollect.GetSystemColorGroupSetCount());
	for ( i=0; i<SystemColorGroupSetCount; i++ )
	{
		ColorGroupSetPtr = AOIDataCollect.GetSystemColorGroupSetPtr(i, false);
		if ( NULL == ColorGroupSetPtr ) { continue; }
		strName = ColorGroupSetPtr->GetColorGroupSetName();

		Node = TListNode();
		Node.Data = i;		
		Node.Text.Format(_T("%d-%s"), i+1, strName);
		NodelList.push_back(Node);
	}
	str = _T("Save System Color Group");
	str = LoadMultiLanguageString(str, str);
	strCaption.Format(_T("%s"), str);
	strLabel = AOIDataDefine.GetIndexText();
	EnumWnd.SetParam1(strCaption, strLabel, m_SystemColorGroupSetIndex, NodelList);
	if ( EnumWnd.GetSelIndex1() < 0 ) 
	{	EnumWnd.SetSelIndex1(0); }	
	if ( EnumWnd.DoModal() == IDCANCEL )
	{	return ; }	

	//CString str;
	//str = _T("Do you want to set the project color be default colors?");
	//str = LoadMultiLanguageString(str, str);
	//if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDCANCEL ) 
	//{	return; }

	unsigned int NewSelIndex = (unsigned int)(EnumWnd.GetSelData());		 
	ColorGroupSetPtr = AOIDataCollect.GetSystemColorGroupSetPtr(NewSelIndex, true);
	if ( NULL == ColorGroupSetPtr )	{	return;	}

	CInputBoxWnd InputBox;
	strCaption = _T("Set Color Group Set Name");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strLabel = AOIDataDefine.GetNameText();
	strValue = ColorGroupSetPtr->GetColorGroupSetName();
	InputBox.SetParam1(strCaption, strLabel, strValue);
	if ( InputBox.DoModal() == IDCANCEL ) 
	{	return;		}
	
	strValue = InputBox.m_DataEdit1;
	m_SystemColorGroupSetIndex = NewSelIndex;
	ColorGroupSetPtr->SetColorGroupSetName(strValue);
	ColorGroupSetPtr->SetColorGroupSetColorGroupList(m_ColorGroupList);		
	AOIDataCollect.SaveSystemColorGroupSetFile();
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::OnResetColorGroupListBtn() 
{
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }

	CString str;
	str = _T("Do you want to reset all project colors?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDCANCEL ) 
	{	return; }

	size_t       i=0;
	CColorRGBV  *rgbvPtr=NULL;		
	const int    rgbvIndex = 0;	
	const size_t Count = m_ColorGroupList.size();
	for ( i=0; i<Count; i++ )
	{	m_ColorGroupList[i].ResetColorGroupColorList();	}
	m_ColorGroup.ResetColorGroupColorList();

	UpdateColorListWnd();	
	if ( m_ColorListCtrl.GetItemCount() > rgbvIndex )
	{	m_ColorListCtrl.SetItemState(rgbvIndex, TVIS_SELECTED, TVIS_SELECTED);	}
	rgbvPtr = m_ColorGroup.GetColorGroupColorPtr(rgbvIndex, true);
	UpdateRGBVToUI(rgbvPtr);	
	BuildColorFilterImage(rgbvPtr);
	m_RGBVWnd.SetColorRGBVPtr(rgbvPtr);
	m_RGBVWnd.ReDrawWnd();	
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::UpdateColorGroupToList(CColorGroup &ColorGroup)
{
	const size_t idx = ColorGroup.GetColorGroupIndex();
	const size_t Count = m_ColorGroupList.size();
	if ( idx < Count )
	{	m_ColorGroupList[idx] = ColorGroup;	}
	return;
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::OnLoadSystemColorBtn() 
{
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }
	
	int             i=0;
	TListNode       Node;
	CString         str;
	CString         strName;
	CString         strLabel;
	CString         strCaption;
	CInputListWnd   EnumWnd;
	CColorGroupSet *ColorGroupSetPtr=NULL;
	std::vector<TListNode> NodelList;		
	const int SystemColorGroupSetCount = (int)(AOIDataCollect.GetSystemColorGroupSetCount());
	for ( i=0; i<SystemColorGroupSetCount; i++ )
	{
		ColorGroupSetPtr = AOIDataCollect.GetSystemColorGroupSetPtr(i, false);
		if ( NULL == ColorGroupSetPtr ) { continue; }
		strName = ColorGroupSetPtr->GetColorGroupSetName();

		Node = TListNode();
		Node.Data = i;		
		Node.Text.Format(_T("%d-%s"), i+1, strName);
		NodelList.push_back(Node);
	}
	str = _T("Load System Color Group");
	str = LoadMultiLanguageString(str, str);
	strCaption.Format(_T("%s"), str);
	strLabel = AOIDataDefine.GetIndexText();
	EnumWnd.SetParam1(strCaption, strLabel, m_SystemColorGroupSetIndex, NodelList);
	if ( EnumWnd.GetSelIndex1() < 0 ) 
	{	EnumWnd.SetSelIndex1(0); }	
	if ( EnumWnd.DoModal() == IDCANCEL )
	{	return ; }	

	unsigned int NewSelIndex = (unsigned int)(EnumWnd.GetSelData());	
	CColorGroupSet *ColorGroupPtr = AOIDataCollect.GetSystemColorGroupSetPtr(NewSelIndex, true);
	if ( NULL == ColorGroupPtr )
	{	return; }
	const size_t ColorGroupCount = ColorGroupPtr->GetColorGroupSetColorGroupCount();
	if ( MAX_PROJECT_COLOR_COUNT != ColorGroupCount )
	{	return; }

	CColorRGBV  *rgbvPtr=NULL;		
	const int    rgbvIndex = 0;
	const size_t GroupIndex = m_ColorGroup.GetColorGroupIndex();
	const size_t GroupCount = m_ColorGroupList.size();

	m_SystemColorGroupSetIndex = NewSelIndex;
	ColorGroupPtr->CloneColorGroupSetColorGroupList(m_ColorGroupList);
	//轉換該專案的畫面編號
	ProjectPtr->UpdateProjectColorGroupListInfo(m_ColorGroupList);

	if ( GroupIndex < GroupCount )
	{	m_ColorGroup = m_ColorGroupList[GroupIndex];	}
	BuildColorListWnd();	
	if ( m_ColorListCtrl.GetItemCount() > rgbvIndex )
	{	m_ColorListCtrl.SetItemState(rgbvIndex, TVIS_SELECTED, TVIS_SELECTED);	}
	rgbvPtr = m_ColorGroup.GetColorGroupColorPtr(rgbvIndex, true);
	UpdateRGBVToUI(rgbvPtr);	
	BuildColorFilterImage(rgbvPtr);
	m_RGBVWnd.SetColorRGBVPtr(rgbvPtr);
	m_RGBVWnd.ReDrawWnd();	
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::SwitchFrameImage()
{
	CAOIProject *ProjectPtr = GetActiveProject();	
	if ( NULL == ProjectPtr ) { return; }
	const bool SwitchFrameMode = AOIDataCollect.CheckSwitchFrameMode();
	if ( false == SwitchFrameMode ) { return; }
	m_ImageIndex = ProjectPtr->GetProjectMapIndexNext(m_ImageIndex);	
	ProjectPtr->SetProjectMapIndex(m_ImageIndex);
	//ExecMoveToStage();
	BuildShowImageBuffer(false);
	CreateBKImage();
	RedrawWnd();	
	return;
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::OnSetColorGroupFrameBtn() 
{
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }

	CAOIProject *ProjectPtr = GetActiveProject();	
	if ( NULL == ProjectPtr ) { return; }	
	const unsigned int MapIndex = ProjectPtr->GetProjectMapIndex();
	const unsigned int FrameUniqueID = ProjectPtr->GetProjectFrameUniqueID(MapIndex, true);
	if ( FRAME_UNIQUE_ID_NULL == FrameUniqueID ) { return; }

	CString str;
	str = _T("Do you want to set the frame index?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDCANCEL ) 
	{	return; }

	m_ColorGroup.SetColorGroupFrameIndex(MapIndex);
	m_ColorGroup.SetColorGroupFrameUniqueID(FrameUniqueID);	
	UpdateColorGroupToList(m_ColorGroup);
}
//-------------------------------------------------------------------------------------//
bool CProjectColorWnd::ExecLoadProjectColor(UINT GroupBegin, UINT GroupEnd)
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }

	CProjectListWnd ProjectListWnd;
	if ( ProjectListWnd.DoModal() == IDCANCEL ) 
	{	return false; }
	
	CString filename;	
	CString tmpfilename;
	CAOIProject Project;
	CString Folder = AOIDataCollect.GetAOITempDirectory();
	std::vector<CColorGroup> ProjectColorGroupList;	

	filename = ProjectListWnd.GetSelectedFilename();
	tmpfilename.Format(_T("%s\\%s"), Folder, _T("ProjectColorGroup.PRG"));
	::DeleteFile(tmpfilename);
	::Sleep(0);
	::CopyFile(filename, tmpfilename, FALSE);
	::Sleep(0);

	if ( Project.LoadProjectColorGroup(filename, ProjectColorGroupList) == false )
	{	return false; }

	size_t       i=0;
	const size_t ProjectColorGroupCount = ProjectColorGroupList.size();
	const size_t BeginIndex = MIN(GroupBegin, ProjectColorGroupCount);
	const size_t EndIndex   = MIN(GroupEnd, ProjectColorGroupCount);	

	const size_t ColorGroupCount = m_ColorGroupList.size();
	if ( BeginIndex>ColorGroupCount || EndIndex>ColorGroupCount )
	{	return false; }

	CColorGroup ColorGroup;
	unsigned int FrameIndex=0;
	unsigned int FrameUniqueID=0;	
	for ( i=BeginIndex; i<EndIndex; i++ )
	{
		ColorGroup = ProjectColorGroupList[i];
		FrameIndex = ColorGroup.GetColorGroupFrameIndex();
		FrameUniqueID = ColorGroup.GetColorGroupFrameUniqueID();
		FrameIndex = ProjectPtr->GetProjectFrameIndexByUniqueID(FrameUniqueID);
		if ( -1 == FrameIndex )
		{
			FrameIndex = 0;
			FrameUniqueID = ProjectPtr->GetProjectFrameUniqueID(FrameIndex, true);//取得專案的影像參數唯一碼
			if ( FRAME_UNIQUE_ID_NULL == FrameUniqueID ) 
			{	continue; }
		}		
		ColorGroup.SetColorGroupFrameIndex(FrameIndex);
		ColorGroup.SetColorGroupFrameUniqueID(FrameUniqueID);
		m_ColorGroupList[i] = ColorGroup;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::OnColorProjectBtnPad() 
{
	// TODO: Add your control notification handler code here	
	UINT GroupEnd   = PROJECT_COLOR_ID_PAD_END+1;	
	UINT GroupBegin = PROJECT_COLOR_ID_PAD_BEGIN;	
	if ( ExecLoadProjectColor(GroupBegin, GroupEnd) == false )
	{	return; }
	m_ColorGroupCtrlID = PROCLR_COLOR_GROUP_BTN_PAD;
	BuildColorGroupListWnd();
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::OnColorProjectBtnVoid() 
{
	// TODO: Add your control notification handler code here
	UINT GroupEnd   = PROJECT_COLOR_ID_VOID_END+1;	
	UINT GroupBegin = PROJECT_COLOR_ID_VOID_BEGIN;	
	if ( ExecLoadProjectColor(GroupBegin, GroupEnd) == false )
	{	return; }
	m_ColorGroupCtrlID = PROCLR_COLOR_GROUP_BTN_VOID;
	BuildColorGroupListWnd();
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::OnColorProjectBtnBody() 
{
	// TODO: Add your control notification handler code here
	UINT GroupEnd   = PROJECT_COLOR_ID_BODY_END+1;	
	UINT GroupBegin = PROJECT_COLOR_ID_BODY_BEGIN;	
	if ( ExecLoadProjectColor(GroupBegin, GroupEnd) == false )
	{	return; }
	m_ColorGroupCtrlID = PROCLR_COLOR_GROUP_BTN_BODY;
	BuildColorGroupListWnd();
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::OnColorProjectBtnBoard() 
{
	// TODO: Add your control notification handler code here
	UINT GroupEnd   = PROJECT_COLOR_ID_BOARD_END+1;	
	UINT GroupBegin = PROJECT_COLOR_ID_BOARD_BEGIN;	
	if ( ExecLoadProjectColor(GroupBegin, GroupEnd) == false )
	{	return; }
	m_ColorGroupCtrlID = PROCLR_COLOR_GROUP_BTN_BOARD;
	BuildColorGroupListWnd();
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::OnColorProjectBtnSolder() 
{
	// TODO: Add your control notification handler code here
	UINT GroupEnd   = PROJECT_COLOR_ID_SOLDER_END+1;	
	UINT GroupBegin = PROJECT_COLOR_ID_SOLDER_BEGIN;	
	if ( ExecLoadProjectColor(GroupBegin, GroupEnd) == false )
	{	return; }
	m_ColorGroupCtrlID = PROCLR_COLOR_GROUP_BTN_SOLDER;
	BuildColorGroupListWnd();
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::OnColorProjectBtnOthers() 
{
	// TODO: Add your control notification handler code here
	UINT GroupEnd   = PROJECT_COLOR_ID_OTHERS_END+1;	
	UINT GroupBegin = PROJECT_COLOR_ID_OTHERS_BEGIN;	
	if ( ExecLoadProjectColor(GroupBegin, GroupEnd) == false )
	{	return; }
	m_ColorGroupCtrlID = PROCLR_COLOR_GROUP_BTN_OTHERS;
	BuildColorGroupListWnd();
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::OnColorProjectBtnAll() 
{
	// TODO: Add your control notification handler code here
	UINT GroupEnd   = MAX_PROJECT_COLOR_COUNT;
	UINT GroupBegin = PROJECT_COLOR_ID_PAD_BEGIN;	
	if ( ExecLoadProjectColor(GroupBegin, GroupEnd) == false )
	{	return; }	
	BuildColorGroupListWnd();
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::OnGrayColorBtn() 
{
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }

	CColorRGBV *rgbvPtr=m_RGBVWnd.GetColorRGBVPtr();
	if ( NULL == rgbvPtr ) { return; }
	rgbvPtr->GrayColor();
	UpdateColorListWnd();
	UpdateRGBVToUI(rgbvPtr);
	BuildColorFilterImage(rgbvPtr);
	m_RGBVWnd.ReDrawWnd();
	return;
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::OnShowGroupColorBtn() 
{
	// TODO: Add your control notification handler code here
	m_ShowColorGroup = true;
	BuildColorFilterImage(NULL);
	return;
}
//-------------------------------------------------------------------------------------//
void CProjectColorWnd::OnGatherShowRawChk() 
{
	// TODO: Add your control notification handler code here
	return;
}
//-------------------------------------------------------------------------------------//