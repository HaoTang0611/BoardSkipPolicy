// EditImageColorFilterWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "EditImageColorFilterWnd.h"
//-------------------------------------------------------------------------------------//
#include "InputListWnd.h"
#include "InputComboxWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CEditImageColorFilterWnd dialog
//-------------------------------------------------------------------------------------//
CEditImageColorFilterWnd::CEditImageColorFilterWnd(CWnd* pParent /*=NULL*/)
	: CBasicDialog(CEditImageColorFilterWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditImageColorFilterWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_WndPtr = NULL;
	m_WndDefectID = WND_DEFECT_NONE;
	m_ColorFilterParamPtr = NULL;	
	m_RGBVMode = COLOR_RGBV_RED;
	m_RGBVWnd.SetTriangleSize(90*2, 156);	
	m_StopFilterListBeSelected = FALSE;
}
//-------------------------------------------------------------------------------------//
void CEditImageColorFilterWnd::DoDataExchange(CDataExchange* pDX)
{
	CBasicDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditImageColorFilterWnd)		
	DDX_Control(pDX, CLRFTR_FILTER_LIST_WND, m_FilterListWnd);
	DDX_Control(pDX, CLRFTR_VALUE_MIN_SPIN, m_ValueMinSpin);
	DDX_Control(pDX, CLRFTR_VALUE_MAX_SPIN, m_ValueMaxSpin);
	DDX_Control(pDX, CLRFTR_COLOR_MIN_SPIN, m_ColorMinSpin);
	DDX_Control(pDX, CLRFTR_COLOR_MAX_SPIN, m_ColorMaxSpin);
	DDX_Control(pDX, CLRFTR_COLOR_RGBV_WND, m_RGBVWnd);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CEditImageColorFilterWnd, CBasicDialog)
	//{{AFX_MSG_MAP(CEditImageColorFilterWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	ON_BN_CLICKED(CLRFTR_RED_MASTER_CHK, OnRedMasterChk)
	ON_BN_CLICKED(CLRFTR_GREEN_MASTER_CHK, OnGreenMasterChk)
	ON_BN_CLICKED(CLRFTR_BLUE_MASTER_CHK, OnBlueMasterChk)
	ON_BN_CLICKED(CLRFTR_RED_ENABLED_CHK, OnRedEnabledChk)
	ON_BN_CLICKED(CLRFTR_GREEN_ENABLED_CHK, OnGreenEnabledChk)
	ON_BN_CLICKED(CLRFTR_BLUE_ENABLED_CHK, OnBlueEnabledChk)
	ON_NOTIFY(UDN_DELTAPOS, CLRFTR_COLOR_MAX_SPIN, OnDeltaposColorMaxSpin)
	ON_NOTIFY(UDN_DELTAPOS, CLRFTR_COLOR_MIN_SPIN, OnDeltaposColorMinSpin)
	ON_NOTIFY(UDN_DELTAPOS, CLRFTR_VALUE_MIN_SPIN, OnDeltaposValueMinSpin)
	ON_NOTIFY(UDN_DELTAPOS, CLRFTR_VALUE_MAX_SPIN, OnDeltaposValueMaxSpin)
	ON_NOTIFY(NM_CLICK, CLRFTR_FILTER_LIST_WND, OnClickFilterListWnd)
	ON_NOTIFY(LVN_ITEMCHANGED, CLRFTR_FILTER_LIST_WND, OnItemchangedFilterListWnd)
	ON_BN_CLICKED(CLRFTR_VALUE_ENABLED_CHK, OnValueEnabledChk)
	ON_BN_CLICKED(CLRFTR_FILTER_EXPAND_BTN, OnFilterExpandBtn)
	ON_BN_CLICKED(CLRFTR_FILTER_SHIRNK_BTN, OnFilterShirnkBtn)
	ON_BN_CLICKED(CLRFTR_GATHER_COLOR_CHK, OnGatherColorChk)
	ON_BN_CLICKED(CLRFTR_RESET_COLOR_BTN, OnResetColorBtn)
	ON_BN_CLICKED(CLRFTR_RESET_ALL_BTN, OnResetAllBtn)
	ON_BN_CLICKED(CLRFTR_MERGE_ALL_BTN, OnMergeAllBtn)
	ON_BN_CLICKED(CLRFTR_COLOR_MAX_BTN, OnColorMaxBtn)
	ON_BN_CLICKED(CLRFTR_COLOR_MIN_BTN, OnColorMinBtn)
	ON_BN_CLICKED(CLRFTR_VALUE_MIN_BTN, OnValueMinBtn)
	ON_BN_CLICKED(CLRFTR_VALUE_MAX_BTN, OnValueMaxBtn)	
	ON_BN_CLICKED(CLRFTR_COLOR_GROUP_BTN_LINK, OnColorGroupBtnLink)
	ON_COMMAND(MENU_LINK_COLOR_DISABLE, OnLinkColorDisable)
	ON_COMMAND(MENU_LINK_COLOR_PAD, OnLinkColorPad)
	ON_COMMAND(MENU_LINK_COLOR_VOID, OnLinkColorVoid)
	ON_COMMAND(MENU_LINK_COLOR_BODY, OnLinkColorBody)
	ON_COMMAND(MENU_LINK_COLOR_BOARD, OnLinkColorBoard)
	ON_COMMAND(MENU_LINK_COLOR_SOLDER, OnLinkColorSolder)
	ON_COMMAND(MENU_LINK_COLOR_OTHERS, OnLinkColorOthers)
	ON_BN_CLICKED(CLRFTR_COLOR_GROUP_BTN_SEND, OnColorGroupBtnSend)
	ON_COMMAND(MENU_SEND_COLOR_PAD, OnSendColorPad)
	ON_COMMAND(MENU_SEND_COLOR_VOID, OnSendColorVoid)
	ON_COMMAND(MENU_SEND_COLOR_BODY, OnSendColorBody)
	ON_COMMAND(MENU_SEND_COLOR_BOARD, OnSendColorBoard)
	ON_COMMAND(MENU_SEND_COLOR_SOLDER, OnSendColorSolder)
	ON_COMMAND(MENU_SEND_COLOR_OTHERS, OnSendColorOthers)		
	ON_BN_CLICKED(CLRFTR_GRAY_COLOR_BTN, OnGrayColorBtn)
	ON_BN_CLICKED(CLRFTR_SHOW_GROUP_COLOR_BTN, OnShowGroupColorBtn)
	ON_BN_CLICKED(CLRFTR_GATHER_SHOW_RAW_CHK, OnGatherShowRawChk)	
	ON_BN_CLICKED(CLRFTR_EXTRACT_WND_COLOR_BTN, OnExtractWndColorBtn)
	ON_BN_CLICKED(CLRFTR_SHOW_WND_COLOR_BTN, OnShowWndColorBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CEditImageColorFilterWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CEditImageColorFilterWnd::OnInitDialog() 
{
	CBasicDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	BuildFilterListWndHeader();
	m_RGBVWnd.CreateMemDC();
	m_RGBVWnd.SetCallBackWnd(this->GetSafeHwnd());

	switch ( m_RGBVMode )
	{
	case COLOR_RGBV_RED:	CWnd::CheckDlgButton(CLRFTR_RED_MASTER_CHK, TRUE);	break;
	case COLOR_RGBV_GREEN:	CWnd::CheckDlgButton(CLRFTR_GREEN_MASTER_CHK, TRUE);	break;		
	case COLOR_RGBV_BLUE:	CWnd::CheckDlgButton(CLRFTR_BLUE_MASTER_CHK, TRUE);	break;
	}

	const int ValueMax = COLOR_RGBV_MAX;
	const int ValueMin = COLOR_RGBV_MIN;
	m_ValueMinSpin.SetRange((short)(ValueMin), (short)(ValueMax));
	m_ValueMaxSpin.SetRange((short)(ValueMin), (short)(ValueMax));
	m_ColorMinSpin.SetRange((short)(ValueMin), (short)(ValueMax));
	m_ColorMaxSpin.SetRange((short)(ValueMin), (short)(ValueMax));
	CWnd::SetDlgItemInt(CLRFTR_COLOR_MAX_EDIT, ValueMax);
	CWnd::SetDlgItemInt(CLRFTR_COLOR_MIN_EDIT, ValueMin);
	CWnd::SetDlgItemInt(CLRFTR_VALUE_MAX_EDIT, ValueMax);
	CWnd::SetDlgItemInt(CLRFTR_VALUE_MIN_EDIT, ValueMin);
	CWnd::SetDlgItemInt(CLRFTR_FILTER_EXPAND_EDIT, 2);

	SwitchMultiLanguage();
	BuildFilterListWnd();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CEditImageColorFilterWnd::OnDestroy() 
{
	CBasicDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CEditImageColorFilterWnd::OnSize(UINT nType, int cx, int cy) 
{
	CBasicDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CEditImageColorFilterWnd::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CBasicDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if ( TRUE == bShow )
	{
	}
	else
	{
		UINT CtrlID = CLRFTR_GATHER_COLOR_CHK;
		BOOL bCheck = CWnd::IsDlgButtonChecked(CtrlID);
		if ( TRUE == bCheck )
		{		
			CWnd::CheckDlgButton(CtrlID, FALSE);
			MANIPULATE_MODEL_MODE  ManiMode = AOIDataCollect.GetManipulateModelModeDefault();
			AOIDataCollect.SetManipulateModelMode(ManiMode);			
		}
	}
}
//-------------------------------------------------------------------------------------//
void CEditImageColorFilterWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CBasicDialog::OnPaint() for painting messages
}
//-------------------------------------------------------------------------------------//
void CEditImageColorFilterWnd::SwitchMultiLanguage()
{
	size_t  i = 0;	
	UINT    WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_EDIT_IMAGE_COLOR_FILTER_WND");	
	//---------------------------------------------------------------------------------//
	WndID = IDD_EDIT_IMAGE_COLOR_FILTER_WND;
	WndKey = _T("IDD_EDIT_IMAGE_COLOR_FILTER_WND");
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
	WndID = CLRFTR_RED_MASTER_CHK;
	WndKey = _T("CLRFTR_RED_MASTER_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CLRFTR_GREEN_MASTER_CHK;
	WndKey = _T("CLRFTR_GREEN_MASTER_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CLRFTR_BLUE_MASTER_CHK;
	WndKey = _T("CLRFTR_BLUE_MASTER_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = CLRFTR_VALUE_ENABLED_CHK;
	WndKey = _T("CLRFTR_VALUE_ENABLED_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = CLRFTR_FILTER_EXPAND_BTN;
	WndKey = _T("CLRFTR_FILTER_EXPAND_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = CLRFTR_FILTER_EXPAND_ONLY_COLOR_CHK;
	WndKey = _T("CLRFTR_FILTER_EXPAND_ONLY_COLOR_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = CLRFTR_FILTER_SHIRNK_BTN;
	WndKey = _T("CLRFTR_FILTER_SHIRNK_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//	
	WndID = CLRFTR_GATHER_COLOR_CHK;
	WndKey = _T("CLRFTR_GATHER_COLOR_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CLRFTR_EXTRACT_WND_COLOR_BTN;
	WndKey = _T("CLRFTR_EXTRACT_WND_COLOR_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = CLRFTR_MERGE_ALL_BTN;
	WndKey = _T("CLRFTR_MERGE_ALL_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CLRFTR_RESET_COLOR_BTN;
	WndKey = _T("CLRFTR_RESET_COLOR_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CLRFTR_RESET_ALL_BTN;
	WndKey = _T("CLRFTR_RESET_ALL_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = CLRFTR_COLOR_GROUP_BTN_LINK;
	WndKey = _T("CLRFTR_COLOR_GROUP_BTN_LINK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CLRFTR_COLOR_GROUP_BTN_SEND;
	WndKey = _T("CLRFTR_COLOR_GROUP_BTN_SEND");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//	
	WndID = CLRFTR_GRAY_COLOR_BTN;
	WndKey = _T("CLRFTR_GRAY_COLOR_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//		
	WndID = CLRFTR_SHOW_GROUP_COLOR_BTN;
	WndKey = _T("CLRFTR_SHOW_GROUP_COLOR_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CLRFTR_GATHER_SHOW_RAW_CHK;
	WndKey = _T("CLRFTR_GATHER_SHOW_RAW_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//		
	WndID = CLRFTR_SHOW_WND_COLOR_BTN;
	WndKey = _T("CLRFTR_SHOW_WND_COLOR_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//		
}
//-------------------------------------------------------------------------------------//
CString CEditImageColorFilterWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_EDIT_IMAGE_COLOR_FILTER_WND");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
void CEditImageColorFilterWnd::ChangeDrawModelMode()
{
	AOIDataCollect.SetDrawModelMode(DRAW_MODEL_EDIT);	
}
//-------------------------------------------------------------------------------------//
DRAW_MODEL_MODE CEditImageColorFilterWnd::GetDrawModelMode() const
{
	return DRAW_MODEL_EDIT;

	DRAW_MODEL_MODE DrawModelMode = AOIDataCollect.GetDrawModelMode();	
	return DrawModelMode;
}
//-------------------------------------------------------------------------------------//
void CEditImageColorFilterWnd::BuildFilterListWndHeader()
{
	CThisListCtrl_09 &ListWnd = m_FilterListWnd;

	JetAPI::InitialListCtrl(ListWnd);
	JetAPI::ClearListCtrlHeaderList(ListWnd);

	CString str;
	int   nCol = 0;
	int width = 0;
	int width2 = 96;
	RECT      Rect;	
	const int Align = LVCFMT_CENTER;//LVCFMT_CENTER;LVCFMT_RIGHT	

	ListWnd.GetClientRect(&Rect);
	width2 = (Rect.right-Rect.left-16)/9;

	str = _T("idx");
	str = LoadMultiLanguageString(str, str);
	width = width2*2;
	ListWnd.InsertColumn(nCol, str, Align, width);
	nCol ++;

	str = _T("+/-");
	str = LoadMultiLanguageString(str, str);
	width = width2*2;
	ListWnd.InsertColumn(nCol, str, Align, width);
	nCol ++;

	str = _T("Used");
	str = LoadMultiLanguageString(str, str);
	width = width2*3;
	ListWnd.InsertColumn(nCol, str, Align, width);
	nCol ++;

	str = _T("color");
	str = LoadMultiLanguageString(str, str);
	width = width2*2;
	ListWnd.InsertColumn(nCol, str, Align, width);
	nCol ++;
	return;
}
//-------------------------------------------------------------------------------------//
void CEditImageColorFilterWnd::ClearFilterListWnd()
{
	m_RGBVIndex = -1;
	m_StopFilterListBeSelected = TRUE;
	JetAPI::ClearListCtrl(m_FilterListWnd, FALSE);
	m_StopFilterListBeSelected = FALSE;
}
//-------------------------------------------------------------------------------------//
void CEditImageColorFilterWnd::BuildFilterListWnd()
{	
	ClearFilterListWnd();
	if ( CheckColorFilterParamPtr() == false ) { return ; }	
	
	size_t       i=0;	
	int          nItem = 0;
	int          nSubItem = 0;
	int          nUnusedIndex=0;	
	CString      str;	
	COLORREF     rgbvColor=0;	
	COLORREF     TextColor = 0xFFFFFF;
	COLOR_LOGIC_MODE  LogicMode;
	CColorRGBV   *rgbvPtr=NULL;	
	CThisListCtrl_09 &ListWnd = m_FilterListWnd;	
	
	rgbvPtr = m_ColorFilterParamPtr->GetBinaryColorActivePtr();
	const size_t ColorCount = m_ColorFilterParamPtr->GetBinaryColorCount();
	const size_t rgbvIndex = m_ColorFilterParamPtr->GetBinaryColorActiveIndex();
	
	nUnusedIndex = -1;
	ListWnd.SetRedraw(FALSE);
	m_StopFilterListBeSelected = TRUE;
	for ( i=0; i<ColorCount; i++ )
	{
		rgbvPtr = m_ColorFilterParamPtr->GetBinaryColorPtr(i, false);
		if ( NULL == rgbvPtr ) { continue; }		
		LogicMode = rgbvPtr->GetLogicMode();
		rgbvColor = rgbvPtr->GetShowColor();

		nSubItem = 0;		
		str.Format(_T("%d"), nItem+1);
		ListWnd.InsertItem(nItem, str);
		ListWnd.SetItemData(nItem, (DWORD_PTR) rgbvPtr);
		ListWnd.SetItemText(nItem, nSubItem, str);	
		ListWnd.SetItemTextBkColor(nItem, nSubItem, TextColor);		
		nSubItem++;

		switch ( LogicMode )
		{
		case COLOR_LOGIC_INCLUDE: str = _T("+"); break;
		case COLOR_LOGIC_EXCLUDE: str = _T("-"); break;
		default: str = _T(""); break;
		}
		ListWnd.SetItemText(nItem, nSubItem, str);	nSubItem++;	

		if ( rgbvPtr->CheckUsed() == true ) 
		{	str = _T("Y"); }
		else
		{	str = _T(""); }		
		ListWnd.SetItemText(nItem, nSubItem, str);	nSubItem++;

		ListWnd.SetItemTextBkColor(nItem, nSubItem, rgbvColor); nSubItem++;

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
	m_StopFilterListBeSelected = FALSE;
	ListWnd.SetRedraw(TRUE);

	if ( nItem > 0 ) 
	{		
		int nSelItem = 0;		
		if ( nUnusedIndex < 0 ) { nUnusedIndex = 0; }
		if ( (-1==rgbvIndex) || (rgbvIndex>nItem) )
		{	nSelItem = nUnusedIndex; }
		else
		{	nSelItem = (int)(rgbvIndex); }		

		m_RGBVIndex = nSelItem;
		ListWnd.SetItemState(nSelItem, LVIS_SELECTED, LVIS_SELECTED);
		rgbvPtr = (CColorRGBV*)(ListWnd.GetItemData(nSelItem));		

		UpdateRGBVToUI(rgbvPtr);		
		m_RGBVWnd.SetColorRGBVPtr(rgbvPtr);
		m_RGBVWnd.ReDrawWnd();		
	}		
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditImageColorFilterWnd::UpdateFilterListWnd()
{
	if ( CheckColorFilterParamPtr() == false ) { return ; }	
	
	size_t       i=0;
	int          nItem = 0;
	int          nSubItem = 0;
	CString      str;
	COLORREF     rgbvColor=0;	
	COLOR_LOGIC_MODE  LogicMode;
	CColorRGBV   *rgbvPtr=NULL;
	CThisListCtrl_09 &ListWnd = m_FilterListWnd;	
	const int ItemCount = ListWnd.GetItemCount();
		
	ListWnd.SetRedraw(FALSE);	
	m_StopFilterListBeSelected = TRUE;
	for ( i=0; i<ItemCount; i++ )
	{
		rgbvPtr = (CColorRGBV*)ListWnd.GetItemData(i);
		if ( NULL == rgbvPtr ) { continue; }				
		nItem = i;
		nSubItem = 0;
		LogicMode = rgbvPtr->GetLogicMode();
		rgbvColor = rgbvPtr->GetShowColor();

		str.Format(_T("%d"), nItem+1);		
		ListWnd.SetItemText(nItem, nSubItem, str);	nSubItem++;

		switch ( LogicMode )
		{
		case COLOR_LOGIC_INCLUDE: str = _T("+"); break;
		case COLOR_LOGIC_EXCLUDE: str = _T("-"); break;
		default: str = _T(""); break;
		}
		ListWnd.SetItemText(nItem, nSubItem, str);	nSubItem++;	

		if ( rgbvPtr->CheckUsed() == true ) 
		{	str = _T("Y"); }
		else
		{	str = _T(""); }		
		ListWnd.SetItemText(nItem, nSubItem, str);	nSubItem++;	

		ListWnd.SetItemTextBkColor(nItem, nSubItem, rgbvColor); nSubItem++;
		nItem ++;
	}
	m_StopFilterListBeSelected = FALSE;	
	ListWnd.SetRedraw(TRUE);
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditImageColorFilterWnd::SetColorFilterParam(CAOIWnd *WndPtr, CAlgBinaryParam *ParamPtr, WND_DEFECT_ID WndDefectID, bool UpdateToUI)
{
	m_RGBVIndex = -1;
	m_WndPtr = WndPtr;
	m_WndDefectID = WndDefectID;
	m_ColorFilterParamPtr = ParamPtr;	
	m_RGBVWnd.ClearColorRGBVTmp();
	if ( CWnd::GetSafeHwnd() == NULL ) { return; }
	if ( true == UpdateToUI )
	{	
		CString  ColorText;
		unsigned int  LinkIndex = -1;		
		if ( NULL != ParamPtr )
		{	LinkIndex = ParamPtr->GetBinaryColorGroupLinkIndex();	 }
		if ( -1 == LinkIndex )
		{	ColorText = AOIDataDefine.GetEnableDisableText(FN_DISABLE); }
		else
		{	ColorText = AOIDataDefine.GetProjectColorGroupText(LinkIndex); }
		CWnd::SetDlgItemText(CLRFTR_COLOR_GROUP_EDIT, ColorText);		
		//CWnd::CheckDlgButton(CLRFTR_GATHER_COLOR_CHK, FALSE);
		BuildFilterListWnd();	
		LockUIWnd(false);
	}
}
//-------------------------------------------------------------------------------------//
bool CEditImageColorFilterWnd::CheckColorFilterParamPtr()
{
	if ( NULL == m_ColorFilterParamPtr ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditImageColorFilterWnd::UpdateRGBVToUI(CColorRGBV *rgbvPtr)
{
	if ( NULL == rgbvPtr ) { return; }
	CString str;	
	int     nValue = 0;
	int     nMax=COLOR_RGBV_MAX;
	int     nMin=COLOR_RGBV_MIN;
	BOOL    bCheck = FALSE; 	

	if ( NULL != m_ColorFilterParamPtr )
	{	m_ColorFilterParamPtr->SetBinaryColorActivePtr(rgbvPtr); }

	if ( rgbvPtr->GetRedEnabled() == true ) { bCheck = TRUE; }
	else { bCheck = FALSE; }
	CWnd::CheckDlgButton(CLRFTR_RED_ENABLED_CHK, bCheck);

	if ( rgbvPtr->GetGreenEnabled() == true ) { bCheck = TRUE; }
	else { bCheck = FALSE; }
	CWnd::CheckDlgButton(CLRFTR_GREEN_ENABLED_CHK, bCheck);

	if ( rgbvPtr->GetBlueEnabled() == true ) { bCheck = TRUE; }
	else { bCheck = FALSE; }
	CWnd::CheckDlgButton(CLRFTR_BLUE_ENABLED_CHK, bCheck);

	if ( rgbvPtr->GetValueEnabled() == true ) { bCheck = TRUE; }
	else { bCheck = FALSE; }
	CWnd::CheckDlgButton(CLRFTR_VALUE_ENABLED_CHK, bCheck);

	nMax = rgbvPtr->GetColorMax(m_RGBVMode);
	nMin = rgbvPtr->GetColorMin(m_RGBVMode);

	m_ColorMaxSpin.SetPos(nMax);
	m_ColorMinSpin.SetPos(nMin);
	str.Format(_T("%d"), nMax);
	CWnd::SetDlgItemText(CLRFTR_COLOR_MAX_EDIT, str);
	str.Format(_T("%d"), nMin);
	CWnd::SetDlgItemText(CLRFTR_COLOR_MIN_EDIT, str);

	nMax = rgbvPtr->GetValueMax();
	nMin = rgbvPtr->GetValueMin();	
	m_ValueMaxSpin.SetPos(nMax);
	m_ValueMinSpin.SetPos(nMin);
	str.Format(_T("%d"), nMax);
	CWnd::SetDlgItemText(CLRFTR_VALUE_MAX_EDIT, str);
	str.Format(_T("%d"), nMin);
	CWnd::SetDlgItemText(CLRFTR_VALUE_MIN_EDIT, str);
}
//-------------------------------------------------------------------------------------//
void CEditImageColorFilterWnd::UpdateRGBVToParam(CColorRGBV *rgbvPtr)
{
	if ( NULL == rgbvPtr ) { return; }
	CString str;
	bool    bEnabled = false;
	int     nValue1 = 0;
	int     nValue2 = 0;
	int     nMax=255, nMin=0;
	BOOL    bCheck = FALSE; 	
	CColorRGBV rgbvOld = *rgbvPtr;

	if ( CWnd::IsDlgButtonChecked(CLRFTR_RED_ENABLED_CHK) == TRUE ) { bEnabled = true; }
	else { bEnabled = false; }
	rgbvPtr->SetRedEnabled(bEnabled);

	if ( CWnd::IsDlgButtonChecked(CLRFTR_GREEN_ENABLED_CHK) == TRUE ) { bEnabled = true; }
	else { bEnabled = false; }
	rgbvPtr->SetGreenEnabled(bEnabled);

	if ( CWnd::IsDlgButtonChecked(CLRFTR_BLUE_ENABLED_CHK) == TRUE ) { bEnabled = true; }
	else { bEnabled = false; }
	rgbvPtr->SetBlueEnabled(bEnabled);

	if ( CWnd::IsDlgButtonChecked(CLRFTR_VALUE_ENABLED_CHK) == TRUE ) { bEnabled = true; }
	else { bEnabled = false; }
	rgbvPtr->SetValueEnabled(bEnabled);
	
	CWnd::GetDlgItemText(CLRFTR_COLOR_MAX_EDIT, str);
	nValue1 = ::_ttoi(str);
	CWnd::GetDlgItemText(CLRFTR_COLOR_MIN_EDIT, str);
	nValue2 = ::_ttoi(str);
	nMax = MAX(nValue1, nValue2);
	nMin = MIN(nValue1, nValue2);

	m_ColorMaxSpin.SetPos(nMax);
	m_ColorMinSpin.SetPos(nMin);
	rgbvPtr->SetColorMax(m_RGBVMode, nMax);
	rgbvPtr->SetColorMin(m_RGBVMode, nMin);		

	CWnd::GetDlgItemText(CLRFTR_VALUE_MAX_EDIT, str);
	nValue1 = ::_ttoi(str);
	CWnd::GetDlgItemText(CLRFTR_VALUE_MIN_EDIT, str);
	nValue2 = ::_ttoi(str);
	nMax = MAX(nValue1, nValue2);
	nMin = MIN(nValue1, nValue2);
	m_ValueMaxSpin.SetPos(nMax);
	m_ValueMinSpin.SetPos(nMin);
	rgbvPtr->SetValueMax(nMax);
	rgbvPtr->SetValueMin(nMin);

	CAOIWnd *WndPtr = m_WndPtr;
	const int rgbvIndex = m_RGBVIndex;	
	LogOperCtrl.SaveLogModelWndAlgColorFilterCompare(WndPtr, rgbvIndex, &rgbvOld, rgbvPtr);

	m_RGBVWnd.ReDrawWnd();
}
//-------------------------------------------------------------------------------------//
bool CEditImageColorFilterWnd::UpdateColorModeToUI(CColorRGBV *rgbvPtr)
{
	if ( NULL == rgbvPtr ) { return false; }
	COLOR_RGBV_MODE ColorMode = rgbvPtr->GetColorMode();

	switch ( ColorMode )
	{
	case COLOR_RGBV_RED:
		ExecRedMasterChk();
		break;
	case COLOR_RGBV_GREEN:
		ExecGreenMasterChk();
		break;
	case COLOR_RGBV_BLUE:
		ExecBlueMasterChk();
		break;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditImageColorFilterWnd::OnOK() 
{
	// TODO: Add extra validation here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return ; }
	TurnOffGatherColor();

	CColorRGBV *rgbvPtr = m_RGBVWnd.GetColorRGBVPtr();
	UpdateRGBVToParam(rgbvPtr);	
	UpdateRGBVToUI(rgbvPtr);
	UpdateFilterListWnd();
	UpdateColorParamToParentWnd(rgbvPtr);
	return;
	CBasicDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CEditImageColorFilterWnd::OnCancel() 
{
	// TODO: Add extra cleanup here
	return;
	CBasicDialog::OnCancel();
}
//-------------------------------------------------------------------------------------//
void CEditImageColorFilterWnd::ExecRedMasterChk()
{
	m_RGBVMode = COLOR_RGBV_RED;	
	CWnd::CheckDlgButton(CLRFTR_GREEN_MASTER_CHK, FALSE);
	CWnd::CheckDlgButton(CLRFTR_BLUE_MASTER_CHK, FALSE);
	CColorRGBV *rgbvPtr=m_RGBVWnd.GetColorRGBVPtr();
	if ( NULL != rgbvPtr )
	{
		const int nMax = rgbvPtr->GetRedMax();
		const int nMin = rgbvPtr->GetRedMin();
		m_ColorMaxSpin.SetPos(nMax);
		m_ColorMinSpin.SetPos(nMin);
		CWnd::SetDlgItemInt(CLRFTR_COLOR_MAX_EDIT, nMax);
		CWnd::SetDlgItemInt(CLRFTR_COLOR_MIN_EDIT, nMin);		
	}	
	m_RGBVWnd.SetRGBVMode(m_RGBVMode);
}
//-------------------------------------------------------------------------------------//
void CEditImageColorFilterWnd::ExecGreenMasterChk()
{
	m_RGBVMode = COLOR_RGBV_GREEN;	
	CWnd::CheckDlgButton(CLRFTR_RED_MASTER_CHK, FALSE);
	CWnd::CheckDlgButton(CLRFTR_BLUE_MASTER_CHK, FALSE);	
	CColorRGBV *rgbvPtr=m_RGBVWnd.GetColorRGBVPtr();
	if ( NULL != rgbvPtr )
	{
		const int nMax = rgbvPtr->GetGreenMax();
		const int nMin = rgbvPtr->GetGreenMin();
		m_ColorMaxSpin.SetPos(nMax);
		m_ColorMinSpin.SetPos(nMin);
		CWnd::SetDlgItemInt(CLRFTR_COLOR_MAX_EDIT, nMax);
		CWnd::SetDlgItemInt(CLRFTR_COLOR_MIN_EDIT, nMin);
	}	
	m_RGBVWnd.SetRGBVMode(m_RGBVMode);
}
//-------------------------------------------------------------------------------------//
void CEditImageColorFilterWnd::ExecBlueMasterChk()
{
	m_RGBVMode = COLOR_RGBV_BLUE;		
	CWnd::CheckDlgButton(CLRFTR_RED_MASTER_CHK, FALSE);
	CWnd::CheckDlgButton(CLRFTR_GREEN_MASTER_CHK, FALSE);	
	CColorRGBV *rgbvPtr=m_RGBVWnd.GetColorRGBVPtr();
	if ( NULL != rgbvPtr )
	{
		const int nMax = rgbvPtr->GetBlueMax();
		const int nMin = rgbvPtr->GetBlueMin();
		m_ColorMaxSpin.SetPos(nMax);
		m_ColorMinSpin.SetPos(nMin);
		CWnd::SetDlgItemInt(CLRFTR_COLOR_MAX_EDIT, nMax);
		CWnd::SetDlgItemInt(CLRFTR_COLOR_MIN_EDIT, nMin);		
	}	
	m_RGBVWnd.SetRGBVMode(m_RGBVMode);
}
//-------------------------------------------------------------------------------------//
void CEditImageColorFilterWnd::SwitchRGBMasterMode()
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
	case COLOR_RGBV_RED:	ExecRedMasterChk();		break;
	case COLOR_RGBV_GREEN:	ExecGreenMasterChk();	break;
	case COLOR_RGBV_BLUE:	ExecBlueMasterChk();	break;
	}
}
//-------------------------------------------------------------------------------------//
void CEditImageColorFilterWnd::OnRedMasterChk() 
{
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	TurnOffGatherColor();
	ExecRedMasterChk();	
}
//-------------------------------------------------------------------------------------//
void CEditImageColorFilterWnd::OnGreenMasterChk() 
{
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	TurnOffGatherColor();
	ExecGreenMasterChk();	
}
//-------------------------------------------------------------------------------------//
void CEditImageColorFilterWnd::OnBlueMasterChk() 
{
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	TurnOffGatherColor();
	ExecBlueMasterChk();
}
//-------------------------------------------------------------------------------------//
void CEditImageColorFilterWnd::OnRedEnabledChk() 
{
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	TurnOffGatherColor();

	bool bEnabled = false;
	BOOL bCheck = CWnd::IsDlgButtonChecked(CLRFTR_RED_ENABLED_CHK);
	if ( TRUE == bCheck ) { bEnabled = true; }
	else { bEnabled = false; }	
	CColorRGBV *rgbvPtr=m_RGBVWnd.GetColorRGBVPtr();
	if ( NULL != rgbvPtr )
	{	
		rgbvPtr->SetRedEnabled(bEnabled); 
		ExecSaveLogModelWndOperateColorFilter(rgbvPtr, COLOR_RGBV_RED, _T("Mode"), !bEnabled, bEnabled);
	}
	UpdateFilterListWnd();
	UpdateColorParamToParentWnd(rgbvPtr);

	if ( TRUE == bCheck ) 
	{	ExecRedMasterChk(); }
	else
	{	m_RGBVWnd.ReDrawWnd(); }
}
//-------------------------------------------------------------------------------------//
void CEditImageColorFilterWnd::OnGreenEnabledChk() 
{
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	TurnOffGatherColor();

	bool bEnabled = false;
	BOOL bCheck = CWnd::IsDlgButtonChecked(CLRFTR_GREEN_ENABLED_CHK);
	if ( TRUE == bCheck ) { bEnabled = true; }
	else { bEnabled = false; }	
	CColorRGBV *rgbvPtr=m_RGBVWnd.GetColorRGBVPtr();
	if ( NULL != rgbvPtr )
	{	
		rgbvPtr->SetGreenEnabled(bEnabled); 
		ExecSaveLogModelWndOperateColorFilter(rgbvPtr, COLOR_RGBV_GREEN, _T("Mode"), !bEnabled, bEnabled);
	}
	UpdateFilterListWnd();
	UpdateColorParamToParentWnd(rgbvPtr);
	
	if ( TRUE == bCheck ) 
	{	ExecGreenMasterChk(); }
	else
	{	m_RGBVWnd.ReDrawWnd(); }
}
//-------------------------------------------------------------------------------------//
void CEditImageColorFilterWnd::OnBlueEnabledChk() 
{
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	TurnOffGatherColor();

	bool bEnabled = false;
	BOOL bCheck = CWnd::IsDlgButtonChecked(CLRFTR_BLUE_ENABLED_CHK);
	if ( TRUE == bCheck ) { bEnabled = true; }
	else { bEnabled = false; }	
	CColorRGBV *rgbvPtr=m_RGBVWnd.GetColorRGBVPtr();
	if ( NULL != rgbvPtr )
	{	
		rgbvPtr->SetBlueEnabled(bEnabled); 
		ExecSaveLogModelWndOperateColorFilter(rgbvPtr, COLOR_RGBV_BLUE, _T("Mode"), !bEnabled, bEnabled);
	}
	UpdateFilterListWnd();
	UpdateColorParamToParentWnd(rgbvPtr);
	
	if ( TRUE == bCheck ) 
	{	ExecBlueMasterChk(); }
	else
	{	m_RGBVWnd.ReDrawWnd(); }
}
//-------------------------------------------------------------------------------------//
void CEditImageColorFilterWnd::OnDeltaposColorMaxSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	TurnOffGatherColor();

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
	CWnd::SetDlgItemText(CLRFTR_COLOR_MAX_EDIT, str);
	rgbvPtr->SetColorMax(m_RGBVMode, nNextPos);	
	if ( rgbvPtr->GetColorEnabled(m_RGBVMode) == false )
	{
		switch ( m_RGBVMode )
		{
		case COLOR_RGBV_RED: CWnd::CheckDlgButton(CLRFTR_RED_ENABLED_CHK, TRUE); 	break;
		case COLOR_RGBV_GREEN: CWnd::CheckDlgButton(CLRFTR_GREEN_ENABLED_CHK, TRUE); 	break;
		case COLOR_RGBV_BLUE: CWnd::CheckDlgButton(CLRFTR_BLUE_ENABLED_CHK, TRUE); 	break;
		}
		rgbvPtr->SetColorEnabled(m_RGBVMode, true);
		ExecSaveLogModelWndOperateColorFilter(rgbvPtr, m_RGBVMode, _T("Mode"), false, true);
		UpdateFilterListWnd();
	}
	ExecSaveLogModelWndOperateColorFilter(rgbvPtr, m_RGBVMode, _T("Upper"), nPos, nNextPos);
	UpdateColorParamToParentWnd(rgbvPtr);
	m_RGBVWnd.ReDrawWnd();
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CEditImageColorFilterWnd::OnDeltaposColorMinSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	TurnOffGatherColor();

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
	CWnd::SetDlgItemText(CLRFTR_COLOR_MIN_EDIT, str);
	rgbvPtr->SetColorMin(m_RGBVMode, nNextPos);	
	if ( rgbvPtr->GetColorEnabled(m_RGBVMode) == false )
	{
		switch ( m_RGBVMode )
		{
		case COLOR_RGBV_RED: CWnd::CheckDlgButton(CLRFTR_RED_ENABLED_CHK, TRUE); 	break;
		case COLOR_RGBV_GREEN: CWnd::CheckDlgButton(CLRFTR_GREEN_ENABLED_CHK, TRUE); 	break;
		case COLOR_RGBV_BLUE: CWnd::CheckDlgButton(CLRFTR_BLUE_ENABLED_CHK, TRUE); 	break;
		}
		rgbvPtr->SetColorEnabled(m_RGBVMode, true);
		ExecSaveLogModelWndOperateColorFilter(rgbvPtr, m_RGBVMode, _T("Mode"), false, true);
		UpdateFilterListWnd();
	}
	ExecSaveLogModelWndOperateColorFilter(rgbvPtr, m_RGBVMode, _T("Lower"), nPos, nNextPos);
	UpdateColorParamToParentWnd(rgbvPtr);
	m_RGBVWnd.ReDrawWnd();
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CEditImageColorFilterWnd::OnDeltaposValueMinSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	TurnOffGatherColor();

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
	CWnd::SetDlgItemText(CLRFTR_VALUE_MIN_EDIT, str);
	rgbvPtr->SetValueMin(nNextPos);	
	if ( rgbvPtr->GetValueEnabled() == false )
	{
		CWnd::CheckDlgButton(CLRFTR_VALUE_ENABLED_CHK, TRUE);
		rgbvPtr->SetValueEnabled(true);
		ExecSaveLogModelWndOperateColorFilter(rgbvPtr, COLOR_RGBV_VALUE, _T("Mode"), false, true);
		UpdateFilterListWnd();
	}
	ExecSaveLogModelWndOperateColorFilter(rgbvPtr, COLOR_RGBV_VALUE, _T("Lower"), nPos, nNextPos);
	UpdateColorParamToParentWnd(rgbvPtr);
	m_RGBVWnd.ReDrawWnd();
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CEditImageColorFilterWnd::OnDeltaposValueMaxSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	TurnOffGatherColor();

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
	CWnd::SetDlgItemText(CLRFTR_VALUE_MAX_EDIT, str);
	rgbvPtr->SetValueMax(nNextPos);
	if ( rgbvPtr->GetValueEnabled() == false )
	{
		CWnd::CheckDlgButton(CLRFTR_VALUE_ENABLED_CHK, TRUE);
		rgbvPtr->SetValueEnabled(true);
		ExecSaveLogModelWndOperateColorFilter(rgbvPtr, COLOR_RGBV_VALUE, _T("Mode"), false, true);
		UpdateFilterListWnd();
	}
	ExecSaveLogModelWndOperateColorFilter(rgbvPtr, COLOR_RGBV_VALUE, _T("Upper"), nPos, nNextPos);	
	UpdateColorParamToParentWnd(rgbvPtr);
	m_RGBVWnd.ReDrawWnd();
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
LRESULT CEditImageColorFilterWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch ( message )
	{
	case MSG_COLOR_FILTER_WND:
		switch ( wParam )
		{
		case WPARAM_UPDATE_COLOR_FILTER:
			TurnOffGatherColor();
			UpdateRGBVToUI(m_RGBVWnd.GetColorRGBVPtr());
			UpdateColorParamToParentWnd(m_RGBVWnd.GetColorRGBVPtr());
			UpdateFilterListWnd();
			break;
		case WPARAM_UPDATE_COLOR_FILTER_BTN_UP:
			ExecSaveLogModelWndOperateColorFilterCheckBack();
			break;
		case WPARAM_UPDATE_COLOR_FILTER_RIGHT_BTN_UP:
			SwitchRGBMasterMode();
			break;
		case WPARAM_UPDATE_WND_COLOR:
			UpdateWndColor();
			break;
		}
		break;
	}
	return CBasicDialog::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void CEditImageColorFilterWnd::OnClickFilterListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NMLISTVIEW* pNMListView = (NMLISTVIEW*)pNMHDR;	
	const int nItem = pNMListView->iItem;	
	if ( nItem < 0 ) 
	{
		TurnOffGatherColor();
		return;		
	}
	m_RGBVIndex = nItem;
	CColorRGBV *rgbvPtr = (CColorRGBV*)(m_FilterListWnd.GetItemData(nItem));		
	if ( NULL == rgbvPtr ) 
	{
		TurnOffGatherColor();
		return;		
	}

	const bool bFilterUsed = true;
	if ( true == bFilterUsed )
	{
		if ( rgbvPtr->CheckUsed() == true )
		{
			TurnOffGatherColor();
			return;		
		}
	}

	CWnd::CheckDlgButton(CLRFTR_GATHER_COLOR_CHK, TRUE);
	ExecGatherColorChk(nItem);
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CEditImageColorFilterWnd::OnItemchangedFilterListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if ( TRUE == m_StopFilterListBeSelected ) { return; }
	
	const int nItem = pNMListView->iItem;	
	if ( nItem < 0 ) 
	{	return;		}
	DWORD Res = pNMListView->uNewState&LVIS_SELECTED;
	if ( Res == 0 ) 
	{	return;		}	
	Res = pNMListView->uNewState&LVIS_FOCUSED;
	if ( Res == 0 ) 
	{	return;		}
	
	m_RGBVIndex = nItem;
	CColorRGBV *rgbvPtr = (CColorRGBV*)(m_FilterListWnd.GetItemData(nItem));

	UpdateRGBVToUI(rgbvPtr);	
	UpdateColorParamToParentWnd(rgbvPtr);
	m_RGBVWnd.SetColorRGBVPtr(rgbvPtr);
	m_RGBVWnd.ReDrawWnd();	

	UpdateColorModeToUI(rgbvPtr);
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CEditImageColorFilterWnd::OnValueEnabledChk() 
{
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	TurnOffGatherColor();

	bool bEnabled = false;
	BOOL bCheck = CWnd::IsDlgButtonChecked(CLRFTR_VALUE_ENABLED_CHK);
	if ( TRUE == bCheck ) { bEnabled = true; }
	else { bEnabled = false; }	
	CColorRGBV *rgbvPtr=m_RGBVWnd.GetColorRGBVPtr();
	if ( NULL != rgbvPtr )
	{	rgbvPtr->SetValueEnabled(bEnabled); }
	UpdateFilterListWnd();
	UpdateColorParamToParentWnd(rgbvPtr);
	m_RGBVWnd.ReDrawWnd();
}
//-------------------------------------------------------------------------------------//
void CEditImageColorFilterWnd::OnFilterExpandBtn() 
{
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	TurnOffGatherColor();

	CColorRGBV *rgbvPtr=m_RGBVWnd.GetColorRGBVPtr();
	if ( NULL == rgbvPtr ) { return; }
	bool bOnlyColor = false;
	const int nValue = (int)(CWnd::GetDlgItemInt(CLRFTR_FILTER_EXPAND_EDIT));
	if ( CWnd::IsDlgButtonChecked(CLRFTR_FILTER_EXPAND_ONLY_COLOR_CHK) == TRUE )
	{	bOnlyColor = true; }
	else
	{	bOnlyColor = false; }
	rgbvPtr->ExpandColorRGBV(nValue, bOnlyColor);
	ExecSaveLogModelWndOperateColorFilter(rgbvPtr, COLOR_RGBV_NONE, _T("Expand"));	
	UpdateRGBVToUI(rgbvPtr);
	UpdateColorParamToParentWnd(rgbvPtr);
	m_RGBVWnd.ReDrawWnd();	
}
//-------------------------------------------------------------------------------------//
void CEditImageColorFilterWnd::OnFilterShirnkBtn() 
{
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	TurnOffGatherColor();

	CColorRGBV *rgbvPtr=m_RGBVWnd.GetColorRGBVPtr();
	bool bOnlyColor = false;
	const int nValue = (int)(CWnd::GetDlgItemInt(CLRFTR_FILTER_EXPAND_EDIT));
	if ( CWnd::IsDlgButtonChecked(CLRFTR_FILTER_EXPAND_ONLY_COLOR_CHK) == TRUE )
	{	bOnlyColor = true; }
	else
	{	bOnlyColor = false; }
	rgbvPtr->ExpandColorRGBV(-nValue, bOnlyColor);
	ExecSaveLogModelWndOperateColorFilter(rgbvPtr, COLOR_RGBV_NONE, _T("Shirnk"));	
	UpdateRGBVToUI(rgbvPtr);
	UpdateColorParamToParentWnd(rgbvPtr);
	m_RGBVWnd.ReDrawWnd();	
}
//-------------------------------------------------------------------------------------//
bool CEditImageColorFilterWnd::UpdateGatherColorCheckButton()
{
	if ( CWnd::GetSafeHwnd() == NULL ) { return true; }

	BOOL bCheck = FALSE;
	MANIPULATE_MODEL_MODE ManiMode = AOIDataCollect.GetManipulateModelMode();
	if ( MANIPULATE_MODEL_GATHER_COLOR == ManiMode )
	{	bCheck = TRUE;	}
	else
	{	bCheck = FALSE; }
	CWnd::CheckDlgButton(CLRFTR_GATHER_COLOR_CHK, bCheck);
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditImageColorFilterWnd::UpdateColorParamToParentWnd(CColorRGBV *rgbvPtr)
{
//	if ( NULL == rgbvPtr ) { return; }
	if ( CheckColorFilterParamPtr() == false ) { return; }
	CAlgBinaryParam BinaryParam;
	//const unsigned int FrameIndex = m_ColorFilterParamPtr->GetBinaryFrameIndex();
	//const unsigned int FrameUniqueID = m_ColorFilterParamPtr->GetBinaryFrameUniqueID();
	AOIDataCollect.GetBinaryParamTemp(BinaryParam);
	//BinaryParam.SetBinaryFrameIndex(FrameIndex);
	//BinaryParam.SetBinaryFrameUniqueID(FrameUniqueID);
	BinaryParam.SetBinaryMode(BINARY_COLOR_FILTER);
	if ( NULL == rgbvPtr )
	{
		m_ColorFilterParamPtr->UpdateBinaryColorUsed();
		m_ColorFilterParamPtr->UpdateBinaryColorShowColor();
		CColorGroup &ColorGroup = m_ColorFilterParamPtr->GetBinaryColorGroup();
		BinaryParam.SetBinaryColorGroup(ColorGroup);
	}
	else
	{
		CColorRGBV rgbv=*rgbvPtr;			
		rgbv.CheckUsed();
		rgbv.CalcShowColor();
		rgbv.SetLogicMode(COLOR_LOGIC_INCLUDE);
		BinaryParam.ResetBinaryColorList();
		BinaryParam.SetBinaryColor(0, rgbv);
	}
	AOIDataCollect.SetBinaryParamTemp(BinaryParam);

	UINT message = MSG_EDIT_IMAGE_PROCESS_WND;
	WPARAM wParam = WPARAM_UPDATE_ALG_PARAM;
	LPARAM lParam = TRUE;
	SendParentWndMessage(message, wParam, lParam);
	//PostParentWndMessage(message, wParam, lParam);	
}
//-------------------------------------------------------------------------------------//
void CEditImageColorFilterWnd::SendParentWndMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND hWnd = CWnd::GetSafeHwnd();
	AOIDataCollect.SendParentWndMessage(hWnd, message, wParam, lParam);		
}
//-------------------------------------------------------------------------------------//
void CEditImageColorFilterWnd::PostParentWndMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND hWnd = CWnd::GetSafeHwnd();
	AOIDataCollect.PostParentWndMessage(hWnd, message, wParam, lParam);		
}
//-------------------------------------------------------------------------------------//
bool CEditImageColorFilterWnd::TurnOffGatherColor()
{	
	CWnd::CheckDlgButton(CLRFTR_GATHER_COLOR_CHK, FALSE);
	MANIPULATE_MODEL_MODE  ManiMode=AOIDataCollect.GetManipulateModelModeDefault();
	AOIDataCollect.SetManipulateModelMode(ManiMode);
	AOIDataCollect.SetDrawImageMode(DRAW_IMAGE_NORMAL);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditImageColorFilterWnd::TurnOnGatherColor(int nItem)
{
	MANIPULATE_MODEL_MODE  DefaultMainMode;
	MANIPULATE_MODEL_MODE  ManiMode = AOIDataCollect.GetManipulateModelMode();	
	switch ( ManiMode )
	{
	case MANIPULATE_MODEL_ADD:
	case MANIPULATE_MODEL_GATHER_COLOR:
		DefaultMainMode = MANIPULATE_MODEL_EDIT;
		break;
	default:
		DefaultMainMode = ManiMode;
		break;
	}
	AOIDataCollect.SetManipulateModelModeDefault(DefaultMainMode);
	AOIDataCollect.SetManipulateModelMode(MANIPULATE_MODEL_GATHER_COLOR);

	const BOOL bShowRaw  = CWnd::IsDlgButtonChecked(CLRFTR_GATHER_SHOW_RAW_CHK);
	DRAW_IMAGE_MODE  DrawMode = AOIDataCollect.GetDrawImageMode();//顯示圖片模式
	if ( DRAW_IAMGE_BY_ALG == DrawMode )
	{	
		if ( TRUE == bShowRaw )//顯示原始圖像
		{	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_SWITCH_FRAME_IMAGE, NULL);	}
		else
		{	
			//顯示抽色後圖像
			CAlgBinaryParam *BinParamPtr = m_ColorFilterParamPtr;
			if ( NULL != BinParamPtr )
			{			
				unsigned int FrameIndex = BinParamPtr->GetBinaryFrameIndex();
				//ProjectPtr->SetProjectMapIndex(FrameIndex);
				AOIDataCollect.SetBinaryParamTemp(*BinParamPtr);
				AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_ALG_IMAGE, NULL);	
			}
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditImageColorFilterWnd::ExecGatherColorChk(int nItem)
{	
	bool bChkPtr = CheckColorFilterParamPtr();	
	if ( nItem<0 || false==bChkPtr ) 
	{		
		TurnOffGatherColor();
		return false; 
	}	
	TurnOnGatherColor(nItem);
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditImageColorFilterWnd::OnGatherColorChk() 
{
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }

	UINT CtrlID = CLRFTR_GATHER_COLOR_CHK;
	BOOL bCheck = CWnd::IsDlgButtonChecked(CtrlID);
	if ( TRUE == bCheck )
	{
		const int nItem = this->m_FilterListWnd.GetNextItem(-1, LVNI_SELECTED);		
		ExecGatherColorChk(nItem);
	}
	else
	{	TurnOffGatherColor();	}	
}
//-------------------------------------------------------------------------------------//
void CEditImageColorFilterWnd::OnResetColorBtn() 
{
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	TurnOffGatherColor();

	if ( CheckColorFilterParamPtr() == false ) { return ; }	
	size_t i = 0;	
	CColorRGBV   *rgbvPtr=NULL;
	const int   rgbvIndex = m_FilterListWnd.GetNextItem(-1, LVNI_SELECTED);	
	rgbvPtr = m_ColorFilterParamPtr->GetBinaryColorPtr(rgbvIndex, true);
	if ( NULL == rgbvPtr ) { return ; }

	rgbvPtr->ResetColor();
	ExecSaveLogModelWndOperateColorFilter(rgbvPtr,COLOR_RGBV_NONE, _T("Reset"));

	UpdateFilterListWnd();
	UpdateRGBVToUI(rgbvPtr);
	UpdateColorParamToParentWnd(rgbvPtr);
	m_RGBVWnd.SetColorRGBVPtr(rgbvPtr);
	m_RGBVWnd.ReDrawWnd();

	CWnd::CheckDlgButton(CLRFTR_GATHER_COLOR_CHK, TRUE);
	ExecGatherColorChk(rgbvIndex);
}
//-------------------------------------------------------------------------------------//
void CEditImageColorFilterWnd::OnResetAllBtn() 
{
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }	
	TurnOffGatherColor();

	if ( CheckColorFilterParamPtr() == false ) { return ; }	
	CString str;
	str = _T("Do you want to clear all colors?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO) 
	{	return;	}

	size_t i = 0;	
	CColorRGBV   *rgbvPtr=NULL;		
	const int rgbvIndex = 0;	
	const size_t ColorCount = m_ColorFilterParamPtr->GetBinaryColorCount();
	for ( i=0; i<ColorCount; i++ )
	{
		rgbvPtr = m_ColorFilterParamPtr->GetBinaryColorPtr(i, false);
		if ( NULL == rgbvPtr ) { continue; }		
		rgbvPtr->ResetColor();
	}
	UpdateFilterListWnd();	
	m_RGBVIndex = rgbvIndex;
	ExecSaveLogModelWndOperateColorFilter(rgbvPtr,COLOR_RGBV_NONE, _T("Reset All"));
	if ( m_FilterListWnd.GetItemCount() > rgbvIndex )
	{	m_FilterListWnd.SetItemState(rgbvIndex, TVIS_SELECTED, TVIS_SELECTED);	}

	rgbvPtr = m_ColorFilterParamPtr->GetBinaryColorPtr(rgbvIndex, true);
	UpdateRGBVToUI(rgbvPtr);	
	UpdateColorParamToParentWnd(rgbvPtr);
	m_RGBVWnd.SetColorRGBVPtr(rgbvPtr);
	m_RGBVWnd.ReDrawWnd();	

	CWnd::CheckDlgButton(CLRFTR_GATHER_COLOR_CHK, TRUE);
	ExecGatherColorChk(rgbvIndex);
}
//-------------------------------------------------------------------------------------//
void CEditImageColorFilterWnd::OnMergeAllBtn() 
{
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	TurnOffGatherColor();

	if ( CheckColorFilterParamPtr() == false ) { return ; }	
	size_t i = 0;	
	CColorRGBV   *rgbvPtr=NULL;		
	const int rgbvIndex = 0;	
	const size_t ColorCount = m_ColorFilterParamPtr->GetBinaryColorCount();

	m_RGBVIndex = rgbvIndex;
	m_ColorFilterParamPtr->MergeBinaryColor();
	ExecSaveLogModelWndOperateColorFilter(rgbvPtr,COLOR_RGBV_NONE, _T("Merge"));
	UpdateFilterListWnd();	
	if ( m_FilterListWnd.GetItemCount() > rgbvIndex )
	{	m_FilterListWnd.SetItemState(rgbvIndex, TVIS_SELECTED, TVIS_SELECTED);	}

	rgbvPtr = m_ColorFilterParamPtr->GetBinaryColorPtr(rgbvIndex, true);
	UpdateRGBVToUI(rgbvPtr);	
	UpdateColorParamToParentWnd(NULL);
	m_RGBVWnd.SetColorRGBVPtr(rgbvPtr);
	m_RGBVWnd.ReDrawWnd();	
}
//-------------------------------------------------------------------------------------//
void CEditImageColorFilterWnd::LockUIWnd(bool bLock)
{
	UINT CtrlID = 0;
	BOOL bEnable = true;	
	DRAW_MODEL_MODE DrawModelMode = GetDrawModelMode();	
	if ( true == bLock ) 
	{	bEnable = FALSE; }
	else
	{	bEnable = TRUE; }
	if ( DRAW_MODEL_RESULT == DrawModelMode )	
	{	bEnable = FALSE;	}
	
	if ( FALSE == bEnable )
	{	this->m_RGBVWnd.SetLockWnd(true); }
	else
	{	this->m_RGBVWnd.SetLockWnd(false); }

	CtrlID = CLRFTR_FILTER_LIST_WND;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = CLRFTR_RED_ENABLED_CHK;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = CLRFTR_GREEN_ENABLED_CHK;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = CLRFTR_BLUE_ENABLED_CHK;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = CLRFTR_RED_MASTER_CHK;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = CLRFTR_GREEN_MASTER_CHK;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = CLRFTR_BLUE_MASTER_CHK;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = CLRFTR_COLOR_MAX_SPIN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = CLRFTR_COLOR_MIN_SPIN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = CLRFTR_COLOR_MAX_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = CLRFTR_COLOR_MIN_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = CLRFTR_VALUE_MIN_SPIN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = CLRFTR_VALUE_MAX_SPIN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = CLRFTR_VALUE_MAX_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = CLRFTR_VALUE_MIN_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = CLRFTR_VALUE_ENABLED_CHK;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = CLRFTR_FILTER_SHIRNK_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = CLRFTR_FILTER_EXPAND_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = CLRFTR_RESET_COLOR_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = CLRFTR_RESET_ALL_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = CLRFTR_GATHER_COLOR_CHK;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = CLRFTR_MERGE_ALL_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = CLRFTR_COLOR_GROUP_BTN_LINK;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);	

	CtrlID = CLRFTR_COLOR_GROUP_BTN_SEND;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);	

	CtrlID = CLRFTR_GRAY_COLOR_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);		

	CtrlID = CLRFTR_EXTRACT_WND_COLOR_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	//Edit Control
	CtrlID = CLRFTR_COLOR_MAX_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);

	CtrlID = CLRFTR_COLOR_MIN_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);

	CtrlID = CLRFTR_VALUE_MAX_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);

	CtrlID = CLRFTR_VALUE_MIN_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);	
}
//-------------------------------------------------------------------------------------//
bool CEditImageColorFilterWnd::GetLockUIWnd() const//取得是否鎖住UIWnd
{
	if ( true == AOIDataCollect.GetIsLockUIWnd() ) { return true; }
	DRAW_MODEL_MODE DrawModelMode = GetDrawModelMode();		
	if ( DRAW_MODEL_RESULT == DrawModelMode )	
	{	return true;	}
	return false;
}
//-------------------------------------------------------------------------------------//
void CEditImageColorFilterWnd::OnColorMaxBtn() 
{
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	TurnOffGatherColor();

	CString   str;	
	int       nNextPos = COLOR_RGBV_MAX;	
	CColorRGBV *rgbvPtr=m_RGBVWnd.GetColorRGBVPtr();
	if ( NULL == rgbvPtr ) { return; }		
	m_ColorMaxSpin.SetPos(nNextPos);	
	str.Format(_T("%d"), nNextPos);
	CWnd::SetDlgItemText(CLRFTR_COLOR_MAX_EDIT, str);
	rgbvPtr->SetColorMax(m_RGBVMode, nNextPos);	
	if ( rgbvPtr->GetColorEnabled(m_RGBVMode) == false )
	{
		switch ( m_RGBVMode )
		{
		case COLOR_RGBV_RED: CWnd::CheckDlgButton(CLRFTR_RED_ENABLED_CHK, TRUE); 	break;
		case COLOR_RGBV_GREEN: CWnd::CheckDlgButton(CLRFTR_GREEN_ENABLED_CHK, TRUE); 	break;
		case COLOR_RGBV_BLUE: CWnd::CheckDlgButton(CLRFTR_BLUE_ENABLED_CHK, TRUE); 	break;
		}
		rgbvPtr->SetColorEnabled(m_RGBVMode, true);
		UpdateFilterListWnd();
	}
	UpdateColorParamToParentWnd(rgbvPtr);
	m_RGBVWnd.ReDrawWnd();
}
//-------------------------------------------------------------------------------------//
void CEditImageColorFilterWnd::OnColorMinBtn() 
{
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	TurnOffGatherColor();

	CString   str;	
	int       nNextPos = COLOR_RGBV_MIN;
	CColorRGBV *rgbvPtr=m_RGBVWnd.GetColorRGBVPtr();
	if ( NULL == rgbvPtr ) { return; }
	m_ColorMinSpin.SetPos(nNextPos);	
	str.Format(_T("%d"), nNextPos);
	CWnd::SetDlgItemText(CLRFTR_COLOR_MIN_EDIT, str);
	rgbvPtr->SetColorMin(m_RGBVMode, nNextPos);	
	if ( rgbvPtr->GetColorEnabled(m_RGBVMode) == false )
	{
		switch ( m_RGBVMode )
		{
		case COLOR_RGBV_RED: CWnd::CheckDlgButton(CLRFTR_RED_ENABLED_CHK, TRUE); 	break;
		case COLOR_RGBV_GREEN: CWnd::CheckDlgButton(CLRFTR_GREEN_ENABLED_CHK, TRUE); 	break;
		case COLOR_RGBV_BLUE: CWnd::CheckDlgButton(CLRFTR_BLUE_ENABLED_CHK, TRUE); 	break;
		}
		rgbvPtr->SetColorEnabled(m_RGBVMode, true);
		UpdateFilterListWnd();
	}
	UpdateColorParamToParentWnd(rgbvPtr);
	m_RGBVWnd.ReDrawWnd();	
}
//-------------------------------------------------------------------------------------//
void CEditImageColorFilterWnd::OnValueMinBtn() 
{
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	TurnOffGatherColor();

	CString   str;
	int       nNextPos = COLOR_RGBV_MIN;
	CColorRGBV *rgbvPtr=m_RGBVWnd.GetColorRGBVPtr();
	if ( NULL == rgbvPtr ) { return; }	
	m_ValueMinSpin.SetPos(nNextPos);
	str.Format(_T("%d"), nNextPos);
	CWnd::SetDlgItemText(CLRFTR_VALUE_MIN_EDIT, str);
	rgbvPtr->SetValueMin(nNextPos);	
	if ( rgbvPtr->GetValueEnabled() == false )
	{
		CWnd::CheckDlgButton(CLRFTR_VALUE_ENABLED_CHK, TRUE);
		rgbvPtr->SetValueEnabled(true);
		UpdateFilterListWnd();
	}
	UpdateColorParamToParentWnd(rgbvPtr);
	m_RGBVWnd.ReDrawWnd();
}
//-------------------------------------------------------------------------------------//
void CEditImageColorFilterWnd::OnValueMaxBtn() 
{
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	TurnOffGatherColor();

	CString   str;	
	int       nNextPos = COLOR_RGBV_MAX;
	CColorRGBV *rgbvPtr=m_RGBVWnd.GetColorRGBVPtr();
	if ( NULL == rgbvPtr ) { return; }
	m_ValueMaxSpin.SetPos(nNextPos);	
	str.Format(_T("%d"), nNextPos);
	CWnd::SetDlgItemText(CLRFTR_VALUE_MAX_EDIT, str);
	rgbvPtr->SetValueMax(nNextPos);
	if ( rgbvPtr->GetValueEnabled() == false )
	{
		CWnd::CheckDlgButton(CLRFTR_VALUE_ENABLED_CHK, TRUE);
		rgbvPtr->SetValueEnabled(true);
		UpdateFilterListWnd();
	}
	UpdateColorParamToParentWnd(rgbvPtr);
	m_RGBVWnd.ReDrawWnd();
}
//-------------------------------------------------------------------------------------//
bool  CEditImageColorFilterWnd::ExecLinkProjectColorGroup(bool bShowDisable, size_t Begin, size_t End)
{
	const bool ListMode=true;
	if ( true==ListMode )
	{	return ExecLinkProjectColorGroupList(bShowDisable, Begin, End);	}
	return ExecLinkProjectColorGroupCombox(bShowDisable, Begin, End);
}
//-------------------------------------------------------------------------------------//
bool CEditImageColorFilterWnd::ExecSendProjectColorGroup(size_t Begin, size_t End)
{
	const bool ListMode=true;
	if ( true==ListMode )
	{	return ExecSendProjectColorGroupList(Begin, End);	}
	return ExecSendProjectColorGroupCombox(Begin, End);
}
//-------------------------------------------------------------------------------------//
bool CEditImageColorFilterWnd::ExecLinkProjectColorGroupList(bool bShowDisable, size_t Begin, size_t End)
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return true; }
	TurnOffGatherColor();

	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }
	if ( CheckColorFilterParamPtr() == false ) { return true; }	

	size_t          i=0;
	POINT           Point;
	CString         str;
	CString         strLabel;
	CString         strCaption;	
	CString         strUnset = AOIDataDefine.GetUnsetText();
	TListNode       Node;		
	CInputListWnd   EnumWnd;
	CColorGroup *ColorGroupPtr = NULL;
	std::vector<TListNode> NodelList;		
	const DWORD_PTR OldLinkIndex = m_ColorFilterParamPtr->GetBinaryColorGroupLinkIndex();
	
	strLabel = _T("Select Color");
	strLabel = LoadMultiLanguageString(strLabel, strLabel);
	strCaption = _T("Select Project Color Group Wnd");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	if ( true == bShowDisable )
	{
		Node.Data = -1;
		Node.Text = AOIDataDefine.GetDisableText();
		NodelList.push_back(Node);
	}
	for ( i=Begin; i<=End; i++ )
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
	::GetCursorPos(&Point);
	EnumWnd.SetWndPos(Point);
	EnumWnd.SetParam1(strCaption, strLabel, OldLinkIndex, NodelList);
	if ( EnumWnd.GetSelIndex1() < 0 ) 
	{	EnumWnd.SetSelIndex1(0); }	
	if ( EnumWnd.DoModal() == IDCANCEL )
	{	return true; }	
	
	const int NewLinkIndex = (int)(EnumWnd.GetSelData());	
	const int OldFrameIndex = m_ColorFilterParamPtr->GetBinaryFrameIndex();
	m_ColorFilterParamPtr->SetBinaryColorGroupLinkIndex(NewLinkIndex);	
	ColorGroupPtr = ProjectPtr->GetProjectColorGroupPtr(NewLinkIndex, true);
	if ( NULL != ColorGroupPtr )
	{	
		CString ColorText = AOIDataDefine.GetProjectColorGroupText(NewLinkIndex);
		CColorGroup ColorGroup = *ColorGroupPtr;
		m_ColorFilterParamPtr->SetBinaryColorGroup(ColorGroup);	
		CWnd::CheckDlgButton(CLRFTR_GATHER_COLOR_CHK, FALSE);
		CWnd::SetDlgItemText(CLRFTR_COLOR_GROUP_EDIT, ColorText);
	}
	
	UpdateColorParamToParentWnd(NULL);
	BuildFilterListWnd();

	const int NewFrameIndex = m_ColorFilterParamPtr->GetBinaryFrameIndex();
	if ( NewFrameIndex != OldFrameIndex )
	{	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_WND_PROPERTY_WND, WPARAM_BUILD_WND_SELECTED, NULL);		}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditImageColorFilterWnd::ExecSendProjectColorGroupList(size_t Begin, size_t End)
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return true; }
	TurnOffGatherColor();

	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }
	if ( CheckColorFilterParamPtr() == false ) { return true; }	

	size_t          i=0;
	POINT           Point;
	CString         str;
	CString         strLabel;
	CString         strCaption;
	CString         strUnset = AOIDataDefine.GetUnsetText();
	TListNode       Node;	
	CInputListWnd   EnumWnd;
	CColorGroup *ColorGroupPtr = NULL;
	std::vector<TListNode> NodelList;	
	const DWORD_PTR OldLinkIndex = m_ColorFilterParamPtr->GetBinaryColorGroupLinkIndex();

	strLabel = _T("Select Color");
	strLabel = LoadMultiLanguageString(strLabel, strLabel);
	strCaption = _T("Select Project Color Group Wnd");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	for ( i=Begin; i<=End; i++ )
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
	::GetCursorPos(&Point);
	EnumWnd.SetWndPos(Point);
	EnumWnd.SetParam1(strCaption, strLabel, OldLinkIndex, NodelList);
	if ( EnumWnd.GetSelIndex1() < 0 ) 
	{	EnumWnd.SetSelIndex1(0); }
	if ( EnumWnd.DoModal() == IDCANCEL )
	{	return true; }

	const int NewLinkIndex = (int)(EnumWnd.GetSelData());	
	m_ColorFilterParamPtr->SetBinaryColorGroupLinkIndex(NewLinkIndex);	
	CString ColorText = AOIDataDefine.GetProjectColorGroupText(NewLinkIndex);
	CColorGroup &ColorGroup = m_ColorFilterParamPtr->GetBinaryColorGroup();
	ProjectPtr->SetProjectColorGroup(NewLinkIndex, ColorGroup);
	
	CWnd::CheckDlgButton(CLRFTR_GATHER_COLOR_CHK, FALSE);	
	CWnd::SetDlgItemText(CLRFTR_COLOR_GROUP_EDIT, ColorText);
	UpdateColorParamToParentWnd(NULL);
	BuildFilterListWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditImageColorFilterWnd::ExecLinkProjectColorGroupCombox(bool bShowDisable, size_t Begin, size_t End)
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return true; }
	TurnOffGatherColor();

	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }
	if ( CheckColorFilterParamPtr() == false ) { return true; }	

	size_t          i=0;
	POINT           Point;
	CString         str;
	CString         strLabel;
	CString         strCaption;	
	CString         strUnset = AOIDataDefine.GetUnsetText();
	TComboxNode     Node;		
	CInputComboxWnd ComboxWnd;
	CColorGroup *ColorGroupPtr = NULL;
	std::vector<TComboxNode> NodelList;		
	const DWORD_PTR OldLinkIndex = m_ColorFilterParamPtr->GetBinaryColorGroupLinkIndex();
	
	strLabel = _T("Select Color");
	strLabel = LoadMultiLanguageString(strLabel, strLabel);
	strCaption = _T("Select Project Color Group Wnd");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	if ( true == bShowDisable )
	{
		Node.Data = -1;
		Node.Text = AOIDataDefine.GetDisableText();
		NodelList.push_back(Node);
	}
	for ( i=Begin; i<=End; i++ )
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
	::GetCursorPos(&Point);
	ComboxWnd.SetWndPos(Point);
	ComboxWnd.SetParam1(strCaption, strLabel, OldLinkIndex, NodelList);
	if ( ComboxWnd.GetSelIndex1() < 0 ) 
	{	ComboxWnd.SetSelIndex1(0); }	
	if ( ComboxWnd.DoModal() == IDCANCEL )
	{	return true; }	
	
	const int NewLinkIndex = (int)(ComboxWnd.GetSelData());	
	const int OldFrameIndex = m_ColorFilterParamPtr->GetBinaryFrameIndex();
	m_ColorFilterParamPtr->SetBinaryColorGroupLinkIndex(NewLinkIndex);	
	ColorGroupPtr = ProjectPtr->GetProjectColorGroupPtr(NewLinkIndex, true);
	if ( NULL != ColorGroupPtr )
	{	
		CString ColorText = AOIDataDefine.GetProjectColorGroupText(NewLinkIndex);
		CColorGroup ColorGroup = *ColorGroupPtr;
		m_ColorFilterParamPtr->SetBinaryColorGroup(ColorGroup);	
		CWnd::CheckDlgButton(CLRFTR_GATHER_COLOR_CHK, FALSE);
		CWnd::SetDlgItemText(CLRFTR_COLOR_GROUP_EDIT, ColorText);
	}
	
	UpdateColorParamToParentWnd(NULL);
	BuildFilterListWnd();

	const int NewFrameIndex = m_ColorFilterParamPtr->GetBinaryFrameIndex();
	if ( NewFrameIndex != OldFrameIndex )
	{	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_WND_PROPERTY_WND, WPARAM_BUILD_WND_SELECTED, NULL);		}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditImageColorFilterWnd::ExecSendProjectColorGroupCombox(size_t Begin, size_t End)
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return true; }
	TurnOffGatherColor();

	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }
	if ( CheckColorFilterParamPtr() == false ) { return true; }	

	size_t          i=0;
	POINT           Point;
	CString         str;
	CString         strLabel;
	CString         strCaption;
	CString         strUnset = AOIDataDefine.GetUnsetText();
	TComboxNode     Node;	
	CInputComboxWnd ComboxWnd;
	CColorGroup *ColorGroupPtr = NULL;
	std::vector<TComboxNode> NodelList;	
	const DWORD_PTR OldLinkIndex = m_ColorFilterParamPtr->GetBinaryColorGroupLinkIndex();

	strLabel = _T("Select Color");
	strLabel = LoadMultiLanguageString(strLabel, strLabel);
	strCaption = _T("Select Project Color Group Wnd");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	for ( i=Begin; i<=End; i++ )
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
	::GetCursorPos(&Point);
	ComboxWnd.SetWndPos(Point);
	ComboxWnd.SetParam1(strCaption, strLabel, OldLinkIndex, NodelList);
	if ( ComboxWnd.GetSelIndex1() < 0 ) 
	{	ComboxWnd.SetSelIndex1(0); }
	if ( ComboxWnd.DoModal() == IDCANCEL )
	{	return true; }

	const int NewLinkIndex = (int)(ComboxWnd.GetSelData());	
	m_ColorFilterParamPtr->SetBinaryColorGroupLinkIndex(NewLinkIndex);	
	CString ColorText = AOIDataDefine.GetProjectColorGroupText(NewLinkIndex);
	CColorGroup &ColorGroup = m_ColorFilterParamPtr->GetBinaryColorGroup();
	ProjectPtr->SetProjectColorGroup(NewLinkIndex, ColorGroup);
	
	CWnd::CheckDlgButton(CLRFTR_GATHER_COLOR_CHK, FALSE);	
	CWnd::SetDlgItemText(CLRFTR_COLOR_GROUP_EDIT, ColorText);
	UpdateColorParamToParentWnd(NULL);
	BuildFilterListWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditImageColorFilterWnd::CheckLinkProjectColorGroup(WND_DEFECT_ID WndDefectID, size_t &Begin, size_t &End)
{
	bool bMatch = true;
	switch ( WndDefectID )
	{
	case WND_DEFECT_PAD_ALIGN:
	case WND_DEFECT_PAD_ADJUST:
		End = PROJECT_COLOR_ID_PAD_END;
		Begin = PROJECT_COLOR_ID_PAD_BEGIN;		
		break;
	case WND_DEFECT_SOLDER_POOR:
	case WND_DEFECT_SOLDER_BRIDGE:
	case WND_DEFECT_SOLDER_BEAD:
		End = PROJECT_COLOR_ID_SOLDER_END;
		Begin = PROJECT_COLOR_ID_SOLDER_BEGIN;		
		break;
	case WND_DEFECT_LEAD_LIFTED:
	case WND_DEFECT_SOLDER_OPEN:		
	case WND_DEFECT_SOLDER_PAD_EXPOSED:
		End = PROJECT_COLOR_ID_VOID_END;
		Begin = PROJECT_COLOR_ID_VOID_BEGIN;		
		break;
		break;
	default:
		bMatch = false;
		Begin = End = -1;
		break;
	} 
	return bMatch;
}
//-------------------------------------------------------------------------------------//
void CEditImageColorFilterWnd::OnColorGroupBtnLink() 
{
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return ; }
	TurnOffGatherColor();

	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	if ( CheckColorFilterParamPtr() == false ) { return ; }		
	
	//size_t LinkIndexEnd=-1;
	//size_t LinkIndexBegin=-1;
	//if ( CheckLinkProjectColorGroup(m_WndDefectID, LinkIndexBegin, LinkIndexEnd) == true )
	//{
	//	ExecLinkProjectColorGroup(true, LinkIndexBegin, LinkIndexEnd);
	//	return;
	//}
	CMenu menu;
	UINT menuID = IDR_MENU_LINK_PROJECT_COLOR;
	if ( menuID == 0 ) { return ; }
	VERIFY(menu.LoadMenu(menuID));
	AOIDataCollect.SwitchMultiLanguageMenu(menu, menuID);

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);

	CWnd* pWndPopupOwner = this;
	while (pWndPopupOwner->GetStyle() & WS_CHILD)
	{	pWndPopupOwner = pWndPopupOwner->GetParent();	}

	POINT point;
	::GetCursorPos(&point);
	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
}
//-------------------------------------------------------------------------------------//
void CEditImageColorFilterWnd::OnLinkColorDisable() 
{
	// TODO: Add your command handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return ; }
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	if ( CheckColorFilterParamPtr() == false ) { return ; }	

	const int NewLinkIndex = -1;
	m_ColorFilterParamPtr->SetBinaryColorGroupLinkIndex(NewLinkIndex);
	CString ColorText = AOIDataDefine.GetEnableDisableText(FN_DISABLE);
	CWnd::CheckDlgButton(CLRFTR_GATHER_COLOR_CHK, FALSE);	
	CWnd::SetDlgItemText(CLRFTR_COLOR_GROUP_EDIT, ColorText);
	UpdateColorParamToParentWnd(NULL);
	BuildFilterListWnd();
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditImageColorFilterWnd::OnLinkColorPad() 
{
	// TODO: Add your command handler code here
	ExecLinkProjectColorGroup(false, PROJECT_COLOR_ID_PAD_BEGIN, PROJECT_COLOR_ID_PAD_END);
}
//-------------------------------------------------------------------------------------//
void CEditImageColorFilterWnd::OnLinkColorVoid() 
{
	// TODO: Add your command handler code here
	ExecLinkProjectColorGroup(false, PROJECT_COLOR_ID_VOID_BEGIN, PROJECT_COLOR_ID_VOID_END);
}
//-------------------------------------------------------------------------------------//
void CEditImageColorFilterWnd::OnLinkColorBody() 
{
	// TODO: Add your command handler code here
	ExecLinkProjectColorGroup(false, PROJECT_COLOR_ID_BODY_BEGIN, PROJECT_COLOR_ID_BODY_END);
}
//-------------------------------------------------------------------------------------//
void CEditImageColorFilterWnd::OnLinkColorBoard() 
{
	// TODO: Add your command handler code here
	ExecLinkProjectColorGroup(false, PROJECT_COLOR_ID_BOARD_BEGIN, PROJECT_COLOR_ID_BOARD_END);
}
//-------------------------------------------------------------------------------------//
void CEditImageColorFilterWnd::OnLinkColorSolder() 
{
	// TODO: Add your command handler code here
	ExecLinkProjectColorGroup(false, PROJECT_COLOR_ID_SOLDER_BEGIN, PROJECT_COLOR_ID_SOLDER_END);
}
//-------------------------------------------------------------------------------------//
void CEditImageColorFilterWnd::OnLinkColorOthers() 
{
	// TODO: Add your command handler code here
	ExecLinkProjectColorGroup(false, PROJECT_COLOR_ID_OTHERS_BEGIN, PROJECT_COLOR_ID_OTHERS_END);
}
//-------------------------------------------------------------------------------------//
void CEditImageColorFilterWnd::OnColorGroupBtnSend() 
{
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return ; }
	TurnOffGatherColor();

	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	if ( CheckColorFilterParamPtr() == false ) { return ; }	
	
	CMenu menu;
	UINT menuID = IDR_MENU_SEND_PROJECT_COLOR;
	if ( menuID == 0 ) { return ; }
	VERIFY(menu.LoadMenu(menuID));
	AOIDataCollect.SwitchMultiLanguageMenu(menu, menuID);

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);

	CWnd* pWndPopupOwner = this;
	while (pWndPopupOwner->GetStyle() & WS_CHILD)
	{	pWndPopupOwner = pWndPopupOwner->GetParent();	}

	POINT point;
	::GetCursorPos(&point);
	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
}
//-------------------------------------------------------------------------------------//
void CEditImageColorFilterWnd::OnSendColorPad() 
{
	// TODO: Add your command handler code here
	ExecSendProjectColorGroup(PROJECT_COLOR_ID_PAD_BEGIN, PROJECT_COLOR_ID_PAD_END);
}
//-------------------------------------------------------------------------------------//
void CEditImageColorFilterWnd::OnSendColorVoid() 
{
	// TODO: Add your command handler code here
	ExecSendProjectColorGroup(PROJECT_COLOR_ID_VOID_BEGIN, PROJECT_COLOR_ID_VOID_END);
}
//-------------------------------------------------------------------------------------//
void CEditImageColorFilterWnd::OnSendColorBody() 
{
	// TODO: Add your command handler code here
	ExecSendProjectColorGroup(PROJECT_COLOR_ID_BODY_BEGIN, PROJECT_COLOR_ID_BODY_END);
}
//-------------------------------------------------------------------------------------//
void CEditImageColorFilterWnd::OnSendColorBoard() 
{
	// TODO: Add your command handler code here
	ExecSendProjectColorGroup(PROJECT_COLOR_ID_BOARD_BEGIN, PROJECT_COLOR_ID_BOARD_END);
}
//-------------------------------------------------------------------------------------//
void CEditImageColorFilterWnd::OnSendColorSolder() 
{
	// TODO: Add your command handler code here
	ExecSendProjectColorGroup(PROJECT_COLOR_ID_SOLDER_BEGIN, PROJECT_COLOR_ID_SOLDER_END);
}
//-------------------------------------------------------------------------------------//
void CEditImageColorFilterWnd::OnSendColorOthers() 
{
	// TODO: Add your command handler code here
	ExecSendProjectColorGroup(PROJECT_COLOR_ID_OTHERS_BEGIN, PROJECT_COLOR_ID_OTHERS_END);
}
//-------------------------------------------------------------------------------------//
void CEditImageColorFilterWnd::OnGrayColorBtn() 
{
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	TurnOffGatherColor();

	CColorRGBV *rgbvPtr=m_RGBVWnd.GetColorRGBVPtr();
	if ( NULL == rgbvPtr ) { return; }
	
	rgbvPtr->GrayColor();
	UpdateFilterListWnd();
	UpdateRGBVToUI(rgbvPtr);
	UpdateColorParamToParentWnd(rgbvPtr);
	m_RGBVWnd.ReDrawWnd();	
	return;	
}
//-------------------------------------------------------------------------------------//
void CEditImageColorFilterWnd::OnShowGroupColorBtn() 
{
	// TODO: Add your control notification handler code here	
	UpdateColorParamToParentWnd(NULL);
	return;
}
//-------------------------------------------------------------------------------------//
void CEditImageColorFilterWnd::OnGatherShowRawChk() 
{
	// TODO: Add your control notification handler code here
	OnGatherColorChk();
}
//-------------------------------------------------------------------------------------//
void CEditImageColorFilterWnd::OnExtractWndColorBtn() 
{
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	if ( CheckColorFilterParamPtr() == false ) { return ; }	

	DWORD     ret;
	CString   str;
	CString   str2;
	CString   strLinkText;
	const int LinkIndex = m_ColorFilterParamPtr->GetBinaryColorGroupLinkIndex();
	if ( -1 != LinkIndex ) 
	{	
		str = _T("Do you want to change the linked-color ?");
		str = LoadMultiLanguageString(str, str);
		strLinkText = AOIDataDefine.GetProjectColorGroupText(LinkIndex);
		str2.Format(_T("%s [%s]"), str, strLinkText);
		ret = JetAPI::ShowMessageBox(str2, MB_YESNO|MB_DEFBUTTON2);
		if ( IDNO == ret ) 
		{	return; }
	}
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_EXTRACT_WND_COLOR_FILTER, 0);	
}
//-------------------------------------------------------------------------------------//
bool CEditImageColorFilterWnd::UpdateWndColor()
{
	CColorRGBV   rgbv;	
	AOIDataCollect.GetColorRGBVTemp(rgbv);	
	this->m_RGBVWnd.SetColorRGBVTmp(rgbv);
	this->m_RGBVWnd.ReDrawWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditImageColorFilterWnd::ExecShowWndColor()
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return true; }
	if ( CheckColorFilterParamPtr() == false ) { return true; }	
	CColorRGBV   rgbv;	
	AOIDataCollect.SetColorRGBVTemp(rgbv);	
	AOIDataCollect.SendMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_CALC_WND_COLOR, 0);	
	UpdateWndColor();
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditImageColorFilterWnd::OnShowWndColorBtn() 
{
	// TODO: Add your control notification handler code here
	ExecShowWndColor();
}
//-------------------------------------------------------------------------------------//
bool CEditImageColorFilterWnd::ExecSaveLogModelWndOperateColorFilterCheckBack()
{	
	CAOIWnd    *WndPtr = m_WndPtr;
	const int   rgbvIndex = m_RGBVIndex;
	CColorRGBV *rgbvPtrNew = m_RGBVWnd.GetColorRGBVPtr();
	CColorRGBV *rgbvPtrOld = m_RGBVWnd.GetColorRGBVOldPtr();
	if ( NULL==rgbvPtrNew || NULL==rgbvPtrOld ) { return true; }
	LogOperCtrl.SaveLogModelWndAlgColorFilterCompare(WndPtr, rgbvIndex, rgbvPtrOld, rgbvPtrNew);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditImageColorFilterWnd::ExecSaveLogModelWndOperateColorFilter(CColorRGBV *rgbvPtr, COLOR_RGBV_MODE RGBV_Mode, LPCTSTR Content)
{
	CAOIWnd *WndPtr = m_WndPtr;
	if ( NULL == WndPtr ) { return false; }
	if ( NULL == rgbvPtr ) { return false; }
	if ( CheckColorFilterParamPtr() == false ) { return false; }
	const int rgbvIndex=m_RGBVIndex;
	CString sOper, sColor, sContnent;	
	CString sFunc = AOIDataDefine.GetAlgBinaryModeText(BINARY_COLOR_FILTER);	
	if ( 0 == RGBV_Mode )
	{	sContnent.Format(_T("%s%02d %s"), sFunc, rgbvIndex+1, Content); }
	else
	{
		sColor = AOIDataDefine.GetAlgColorRGBVModeText(RGBV_Mode);		
		sContnent.Format(_T("%s%02d[%s] %s"), sFunc, rgbvIndex+1, sColor, Content);
	}	
	LogOperCtrl.SaveLogModelWndAlgColorFilterContent(WndPtr, sContnent);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditImageColorFilterWnd::ExecSaveLogModelWndOperateColorFilter(CColorRGBV *rgbvPtr, COLOR_RGBV_MODE RGBV_Mode, LPCTSTR sKey, int nOld, int nNew)
{
	CAOIWnd *WndPtr = m_WndPtr;
	if ( NULL == WndPtr ) { return false; }
	if ( NULL == rgbvPtr ) { return false; }
	if ( CheckColorFilterParamPtr() == false ) { return false; }
	const int rgbvIndex=m_RGBVIndex;
	CString sOper, sOld, sNew, sColor;	
	CString sFunc = AOIDataDefine.GetAlgBinaryModeText(BINARY_COLOR_FILTER);
	sOld.Format(_T("%d"), nOld);
	sNew.Format(_T("%d"), nNew);
	sColor = AOIDataDefine.GetAlgColorRGBVModeText(RGBV_Mode);		
	sOper.Format(_T("%s %s_%02d[%s]"), AOIDataDefine.GetSetText(), sFunc, rgbvIndex+1, sColor);
	LogOperCtrl.SaveLogModelWndAlgColorFilterOperate(WndPtr, sOper, sKey, sOld, sNew);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditImageColorFilterWnd::ExecSaveLogModelWndOperateColorFilter(CColorRGBV *rgbvPtr, COLOR_RGBV_MODE RGBV_Mode, LPCTSTR sKey, bool bOld, bool bNew)
{
	CAOIWnd *WndPtr = m_WndPtr;
	if ( NULL == WndPtr ) { return false; }
	if ( NULL == rgbvPtr ) { return false; }
	if ( CheckColorFilterParamPtr() == false ) { return false; }	
	const int rgbvIndex=m_RGBVIndex;
	CString sOper, sOld, sNew, sColor;	
	CString sFunc = AOIDataDefine.GetAlgBinaryModeText(BINARY_COLOR_FILTER);	
	sOld = AOIDataDefine.GetEnableDisableText(bOld);
	sNew = AOIDataDefine.GetEnableDisableText(bNew);
	sColor = AOIDataDefine.GetAlgColorRGBVModeText(RGBV_Mode);
	sOper.Format(_T("%s %s_%02d[%s]"), AOIDataDefine.GetSetText(), sFunc, rgbvIndex+1, sColor);
	LogOperCtrl.SaveLogModelWndAlgColorFilterOperate(WndPtr, sOper, sKey, sOld, sNew);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditImageColorFilterWnd::ExecSaveLogModelWndOperateColorFilter(CColorRGBV *rgbvPtr, COLOR_RGBV_MODE RGBV_Mode, LPCTSTR sKey, LPCTSTR sOld, LPCTSTR sNew)
{
	CAOIWnd *WndPtr = m_WndPtr;
	if ( NULL == WndPtr ) { return false; }
	if ( NULL == rgbvPtr ) { return false; }
	if ( CheckColorFilterParamPtr() == false ) { return false; }	
	const int rgbvIndex=m_RGBVIndex;
	CString sOper, sColor;		
	CString sFunc = AOIDataDefine.GetAlgBinaryModeText(BINARY_COLOR_FILTER);
	sColor = AOIDataDefine.GetAlgColorRGBVModeText(RGBV_Mode);
	sOper.Format(_T("%s %s_%02d[%s]"), AOIDataDefine.GetSetText(), sFunc, rgbvIndex+1, sColor);
	LogOperCtrl.SaveLogModelWndAlgColorFilterOperate(WndPtr, sOper, sKey, sOld, sNew);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditImageColorFilterWnd::ExecSaveLogModelWndOperateColorFilter(CColorRGBV *rgbvPtr, LPCTSTR sKey, LPCTSTR sOld, LPCTSTR sNew)
{	
	CAOIWnd *WndPtr = m_WndPtr;
	if ( NULL == WndPtr ) { return false; }
	if ( NULL == rgbvPtr ) { return false; }
	if ( CheckColorFilterParamPtr() == false ) { return false; }	
	const int rgbvIndex=m_RGBVIndex;
	CString sOper = AOIDataDefine.GetSetText();
	LogOperCtrl.SaveLogModelWndAlgColorFilterOperate(WndPtr, sOper, sKey, sOld, sNew);
	return true;
}
//-------------------------------------------------------------------------------------//