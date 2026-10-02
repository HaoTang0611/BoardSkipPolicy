// ProjectMapMaskWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "ProjectMapMaskWnd.h"
//-------------------------------------------------------------------------------------//
#include "InputBoxWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CProjectMapMaskWnd dialog
//-------------------------------------------------------------------------------------//
CProjectMapMaskWnd::CProjectMapMaskWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CProjectMapMaskWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProjectMapMaskWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_CtrlMode = PMM_CTRL_SHPAE_RECT_RADIO;
	m_ShowColorGroup = false;
	m_ColorRGBVIndex = -1;
	m_RGBVMode = COLOR_RGBV_RED;	
	m_RGBVWnd.SetTriangleSize(90*2, 156);	
	m_StopColorListBeSelected = false;
	m_ShowMode = PMM_SHOW_COMBINED_RADIO;
	m_BkColor = 0xE0E0E0;
	m_ImageZoom = 1.0;
	m_ImageOffset = TPOINT2D();
	m_MapIndex = 0;
	m_ProjectMapW = 1024;
	m_ProjectMapH = 1024;
	m_ProjectPtr = NULL;	
	m_ShowImagePtr = NULL;
	m_MapMaskImagePtr = NULL;	
	m_MapMaskImagePtr0 = NULL;
	m_MapMaskImagePtr1 = NULL;
	m_MapMaskImagePtr2 = NULL;
	m_MapMaskImagePtr3 = NULL;
	m_MapMaskImagePtr4 = NULL;
	m_LBtnUpPt.x = m_LBtnUpPt.y = -1;
	m_LBtnDownPt.x = m_LBtnDownPt.y = -1;
	m_RBtnUpPt.x = m_RBtnUpPt.y = -1;
	m_RBtnDownPt.x = m_RBtnDownPt.y = -1;
	m_LastPt.x = m_LastPt.y = -1;
	m_CurrentPt.x = m_CurrentPt.y = -1;
	ResetMaskRect();	
	ClearMaskImageBuffer();
	ClearShowImageBuffer();		
	m_ColorGroup.BuildColorGroup_Test();
}
//-------------------------------------------------------------------------------------//
CProjectMapMaskWnd::CProjectMapMaskWnd(UINT nIDTemplate, CWnd * pParent)
	: CBaseDialog(nIDTemplate, pParent) 
{
	m_CtrlMode = PMM_CTRL_SHPAE_RECT_RADIO;
	m_ShowColorGroup = false;
	m_ColorRGBVIndex = -1;
	m_RGBVMode = COLOR_RGBV_RED;
	m_RGBVWnd.SetTriangleSize(90 * 2, 156);
	m_StopColorListBeSelected = false;
	m_ShowMode = PMM_SHOW_COMBINED_RADIO;
	m_BkColor = 0xE0E0E0;
	m_ImageZoom = 1.0;
	m_ImageOffset = TPOINT2D();
	m_MapIndex = 0;
	m_ProjectMapW = 1024;
	m_ProjectMapH = 1024;
	m_ProjectPtr = NULL;
	m_ShowImagePtr = NULL;
	m_MapMaskImagePtr = NULL;
	m_MapMaskImagePtr0 = NULL;
	m_MapMaskImagePtr1 = NULL;
	m_MapMaskImagePtr2 = NULL;
	m_MapMaskImagePtr3 = NULL;
	m_MapMaskImagePtr4 = NULL;
	m_LBtnUpPt.x = m_LBtnUpPt.y = -1;
	m_LBtnDownPt.x = m_LBtnDownPt.y = -1;
	m_RBtnUpPt.x = m_RBtnUpPt.y = -1;
	m_RBtnDownPt.x = m_RBtnDownPt.y = -1;
	m_LastPt.x = m_LastPt.y = -1;
	m_CurrentPt.x = m_CurrentPt.y = -1;
	ResetMaskRect();
	ClearMaskImageBuffer();
	ClearShowImageBuffer();
	m_ColorGroup.BuildColorGroup_Test();
}
//-------------------------------------------------------------------------------------//
CProjectMapMaskWnd::~CProjectMapMaskWnd()
{
	m_ImageZoom = 1.0;
	ClearMaskImageBuffer();
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProjectMapMaskWnd)
	DDX_Control(pDX, PMM_IMAGE_WND, m_ImageWnd);	
	DDX_Control(pDX, PMM_FRAME_INDEX_COMBO, m_FrameIndexComobx);	
	DDX_Control(pDX, PMM_MASK_INDEX_COMBO, m_MaskIndexComobx);
	DDX_Control(pDX, PMM_COLOR_LIST_WND, m_ColorListCtrl);
	DDX_Control(pDX, PMM_VALUE_MIN_SPIN, m_ValueMinSpin);
	DDX_Control(pDX, PMM_VALUE_MAX_SPIN, m_ValueMaxSpin);	
	DDX_Control(pDX, PMM_COLOR_MIN_SPIN, m_ColorMinSpin);
	DDX_Control(pDX, PMM_COLOR_MAX_SPIN, m_ColorMaxSpin);
	DDX_Control(pDX, PMM_COLOR_RGBV_WND, m_RGBVWnd);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CProjectMapMaskWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CProjectMapMaskWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDBLCLK()	
	ON_CBN_SELCHANGE(PMM_FRAME_INDEX_COMBO, OnSelchangeFrameIndexCombo)
	ON_CBN_SELCHANGE(PMM_MASK_INDEX_COMBO, OnSelchangeMaskIndexCombo)
	ON_BN_CLICKED(PMM_MASK_ADD_BTN, OnMaskAddBtn)
	ON_BN_CLICKED(PMM_MASK_ERASE_BTN, OnMaskEraseBtn)
	ON_BN_CLICKED(PMM_MASK_CLEAR_BTN, OnMaskClearBtn)	
	ON_BN_CLICKED(PMM_SHOW_MASK_RADIO, OnShowMaskRadio)
	ON_BN_CLICKED(PMM_SHOW_IMAGE_RADIO, OnShowImageRadio)
	ON_BN_CLICKED(PMM_SHOW_COMBINED_RADIO, OnShowCombinedRadio)
	ON_BN_CLICKED(PMM_SHOW_COMPONENT_CHK, OnShowComponentChk)
	ON_NOTIFY(LVN_ITEMCHANGED, PMM_COLOR_LIST_WND, OnItemchangedColorListWnd)
	ON_BN_CLICKED(PMM_RED_MASTER_CHK, OnRedMasterChk)
	ON_BN_CLICKED(PMM_GREEN_MASTER_CHK, OnGreenMasterChk)
	ON_BN_CLICKED(PMM_BLUE_MASTER_CHK, OnBlueMasterChk)
	ON_BN_CLICKED(PMM_RED_ENABLED_CHK, OnRedEnabledChk)
	ON_BN_CLICKED(PMM_GREEN_ENABLED_CHK, OnGreenEnabledChk)
	ON_BN_CLICKED(PMM_BLUE_ENABLED_CHK, OnBlueEnabledChk)
	ON_BN_CLICKED(PMM_VALUE_ENABLED_CHK, OnValueEnabledChk)
	ON_NOTIFY(UDN_DELTAPOS, PMM_COLOR_MAX_SPIN, OnDeltaposColorMaxSpin)
	ON_NOTIFY(UDN_DELTAPOS, PMM_COLOR_MIN_SPIN, OnDeltaposColorMinSpin)
	ON_NOTIFY(UDN_DELTAPOS, PMM_VALUE_MAX_SPIN, OnDeltaposValueMaxSpin)
	ON_NOTIFY(UDN_DELTAPOS, PMM_VALUE_MIN_SPIN, OnDeltaposValueMinSpin)
	ON_BN_CLICKED(PMM_FILTER_EXPAND_BTN, OnFilterExpandBtn)
	ON_BN_CLICKED(PMM_FILTER_SHIRNK_BTN, OnFilterShirnkBtn)
	ON_BN_CLICKED(PMM_COLOR_MAX_BTN, OnColorMaxBtn)
	ON_BN_CLICKED(PMM_COLOR_MIN_BTN, OnColorMinBtn)
	ON_BN_CLICKED(PMM_VALUE_MAX_BTN, OnValueMaxBtn)
	ON_BN_CLICKED(PMM_VALUE_MIN_BTN, OnValueMinBtn)
	ON_BN_CLICKED(PMM_RESET_COLOR_BTN, OnResetColorBtn)
	ON_BN_CLICKED(PMM_RESET_ALL_BTN, OnResetAllBtn)
	ON_BN_CLICKED(PMM_MERGE_ALL_BTN, OnMergeAllBtn)
	ON_BN_CLICKED(PMM_CTRL_SHPAE_RECT_RADIO, OnCtrlShpaeRectRadio)
	ON_BN_CLICKED(PMM_CTRL_SHPAE_CIRCLE_RADIO, OnCtrlShpaeCircleRadio)	
	ON_BN_CLICKED(PMM_CTRL_COLOR_FILTER_RADIO, OnCtrlColorFilterRadio)
	ON_BN_CLICKED(PMM_GRAY_COLOR_BTN, OnGrayColorBtn)
	ON_BN_CLICKED(PMM_SHOW_GROUP_COLOR_BTN, OnShowGroupColorBtn)
	ON_BN_CLICKED(PMM_GATHER_SHOW_RAW_CHK, OnGatherShowRawChk)
	ON_BN_CLICKED(PMM_MASK_ERODE_BTN, OnMaskErodeBtn)
	ON_BN_CLICKED(PMM_MASK_DILATE_BTN, OnMaskDilateBtn)
	ON_BN_CLICKED(PMM_MASK_OPEN_BTN, OnMaskOpenBtn)
	ON_BN_CLICKED(PMM_MASK_CLOSE_BTN, OnMaskCloseBtn)
	ON_BN_CLICKED(PMM_MASK_GRAD_BTN, OnMaskGradBtn)
	ON_BN_CLICKED(PMM_MASK_MERGE_BTN, OnMaskMergeBtn)
	ON_BN_CLICKED(PMM_MASK_ERASE_PART_BTN, OnMaskErasePartBtn)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CProjectMapMaskWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CProjectMapMaskWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	//m_ImageWnd.GetClientRect(&m_ImageWndRect);	
	//m_ImageWndMemDC.CreateMemDC(&m_ImageWnd, m_BkColor);	
	//m_ImageWndMemDC2.CreateMemDC(&m_ImageWnd, m_BkColor);
	ShowWindow(SW_SHOWMAXIMIZED);

	JetAPI::InitialListCtrl(m_ColorListCtrl);	
	m_RGBVWnd.CreateMemDC();
	m_RGBVWnd.SetCallBackWnd(this->GetSafeHwnd());

	BuildFrameIndexCombox();
	BuildMaskIndexCombox();
	//CreateMaskImageBuffer();
	CreateMaskImageBuffer_Multi();
	CreateShowImageBuffer();
	SwitchMultiLanguage();		

	CWnd::CheckDlgButton(m_ShowMode, TRUE);
	CWnd::CheckDlgButton(m_CtrlMode, TRUE);
	//CWnd::CheckDlgButton(PMM_USE_ROI_RECT_CHK, TRUE);	
	CWnd::CheckDlgButton(PMM_SHOW_COMPONENT_CHK, TRUE);
	switch ( m_RGBVMode )
	{
	case COLOR_RGBV_RED:	CWnd::CheckDlgButton(PMM_RED_MASTER_CHK, TRUE);	break;
	case COLOR_RGBV_GREEN:	CWnd::CheckDlgButton(PMM_GREEN_MASTER_CHK, TRUE);	break;		
	case COLOR_RGBV_BLUE:	CWnd::CheckDlgButton(PMM_BLUE_MASTER_CHK, TRUE);	break;
	}	

	const int ValueMax = COLOR_RGBV_MAX;
	const int ValueMin = COLOR_RGBV_MIN;
	m_ValueMinSpin.SetRange((short)(ValueMin), (short)(ValueMax));
	m_ValueMaxSpin.SetRange((short)(ValueMin), (short)(ValueMax));
	m_ColorMinSpin.SetRange((short)(ValueMin), (short)(ValueMax));
	m_ColorMaxSpin.SetRange((short)(ValueMin), (short)(ValueMax));
	CWnd::SetDlgItemInt(PMM_COLOR_MAX_EDIT, ValueMax);
	CWnd::SetDlgItemInt(PMM_COLOR_MIN_EDIT, ValueMin);
	CWnd::SetDlgItemInt(PMM_VALUE_MAX_EDIT, ValueMax);
	CWnd::SetDlgItemInt(PMM_VALUE_MIN_EDIT, ValueMin);
	CWnd::SetDlgItemInt(PMM_FILTER_EXPAND_EDIT, 2);

	BuildColorListWndHeader();
	BuildColorListWnd();
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());

	CColorRGBV *rgbvPtr = m_RGBVWnd.GetColorRGBVPtr();
	UpdateRGBVToUI(rgbvPtr);
	ImageAPI.CalcImageWndFitZoom(m_ShowImageW, m_ShowImageH, m_ImageWndRect, 1.0, m_ImageZoom);	
	BuildShowImage();
	DrawShowImage();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::OnOK()
{
	m_MapMaskImagePtr = m_MapMaskImagePtr0;
	CBaseDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	//ClearMaskImageBuffer();//解構子清除
	ClearShowImageBuffer();		
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::OnSize(UINT nType, int cx, int cy) 
{
	CBaseDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if ( GetSafeHwnd() == NULL ) { return; }
	if ( m_ImageWnd.GetSafeHwnd() == NULL ) { return; }

	{
		RECT WndRect={0};
		m_ImageWnd.GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		WndRect.right = cx;
		WndRect.bottom = cy;
		m_ImageWnd.MoveWindow(&WndRect, FALSE);
		m_ImageWnd.GetClientRect(&m_ImageWndRect);	
		m_ImageWndMemDC.CreateMemDC(&m_ImageWnd, m_BkColor);	
		m_ImageWndMemDC2.CreateMemDC(&m_ImageWnd, m_BkColor);	
		DrawShowImage();
	}
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default	
	CBaseDialog::OnGetMinMaxInfo(lpMMI);
	if ( lpMMI->ptMinTrackSize.x < 1024 ) { lpMMI->ptMinTrackSize.x = 1024; }
	if ( lpMMI->ptMinTrackSize.y <  768 ) { lpMMI->ptMinTrackSize.y =  768; }
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::SetProjectPtr(CAOIProject *Ptr)
{	
	m_ImageZoom = 1.0;
	m_ImageOffset = TPOINT2D();
	m_MapIndex = 0;
	m_ProjectMapW = 1024;
	m_ProjectMapH = 1024;
	ClearMaskImageBuffer();
	ClearShowImageBuffer();	
	m_ProjectPtr = Ptr;
	if ( NULL == Ptr ) { return; }
	
	TREGION4D CadRgn;	
	IMAGE_PTR  ImagePtr=NULL;
	IMAGE_SIZE ImageW=0;
	IMAGE_SIZE ImageH=0;
	IMAGE_SIZE ImageStep=0;
	IMAGE_SIZE BitCount=0;
	Ptr->GetProjectMapInfo(m_FrameResolution, CadRgn, m_FrameStageRgn);	
	Ptr->GetProjectMapPtr(m_MapIndex, ImageW, ImageH, ImageStep, BitCount, ImagePtr);
	m_ProjectMapW = ImageW;
	m_ProjectMapH = ImageH;
	return;
}
//-------------------------------------------------------------------------------------//
CAOIProject* CProjectMapMaskWnd::GetActiveProject()
{
	return m_ProjectPtr;
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::SetColorRGBVIndex(int val)
{
	m_ColorRGBVIndex = val;
}
//-------------------------------------------------------------------------------------//
int CProjectMapMaskWnd::GetColorRGBVIndex() const
{
	return m_ColorRGBVIndex;
}
//-------------------------------------------------------------------------------------//
bool CProjectMapMaskWnd::BuildColorListWndHeader()
{
	CThisListCtrl_21 &ListCtrl = m_ColorListCtrl;

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
bool CProjectMapMaskWnd::CheckUseColorFilterMode()
{
	if ( PMM_CTRL_COLOR_FILTER_RADIO == m_CtrlMode ) 
	{	return true; }
	return false;

	UINT CtrlID = PMM_CTRL_COLOR_FILTER_RADIO;
	BOOL bChk = CWnd::IsDlgButtonChecked(CtrlID);	
	if ( FALSE == bChk ) { return false; }	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectMapMaskWnd::ClearColorListWnd()
{
	m_StopColorListBeSelected = true;
	JetAPI::ClearListCtrl(m_ColorListCtrl, FALSE);	
	m_StopColorListBeSelected = false;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectMapMaskWnd::BuildColorListWnd()
{
	ClearColorListWnd();	

	size_t       i=0;	
	int          nItem = 0;
	int          nSubItem = 0;
	int          nUnusedIndex=0;
	CString      str;
	COLORREF     rgbvColor=0;	
	COLORREF     TextColor = 0xFFFFFF;
	COLOR_LOGIC_MODE  LogicMode;
	CColorRGBV   *rgbvPtr=NULL;	
	CThisListCtrl_21 &ListCtrl = m_ColorListCtrl;	

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
bool CProjectMapMaskWnd::UpdateColorListWnd()
{	
	int          i=0;
	int          nItem = 0;
	int          nSubItem = 0;
	CString      str;
	COLORREF     rgbvColor=0;	
	COLOR_LOGIC_MODE  LogicMode;
	CColorRGBV   *rgbvPtr=NULL;
	CThisListCtrl_21 &ListCtrl = m_ColorListCtrl;	
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
IMAGE_PTR CProjectMapMaskWnd::GetMaskImage(IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount)
{
	ImageW = m_MapMaskImageW;
	ImageH = m_MapMaskImageH;
	ImageStep = m_MapMaskImageStep;
	BitCount = m_MapMaskBitCount;	
	return m_MapMaskImagePtr;
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_PROJECT_MAP_MASK_WND");
	//---------------------------------------------------------------------------------//
	WndID = IDD_PROJECT_MAP_MASK_WND;
	WndKey = _T("IDD_PROJECT_MAP_MASK_WND");
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
	WndID = PMM_FRAME_INDEX_LABEL;
	WndKey = _T("PMM_FRAME_INDEX_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PMM_CTRL_GROUP;
	WndKey = _T("PMM_CTRL_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PMM_CTRL_SHPAE_RECT_RADIO;
	WndKey = _T("PMM_CTRL_SHPAE_RECT_RADIO");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PMM_CTRL_SHPAE_CIRCLE_RADIO;
	WndKey = _T("PMM_CTRL_SHPAE_CIRCLE_RADIO");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PMM_CTRL_SHPAE_CIRCLE_RADIO;
	WndKey = _T("PMM_CTRL_SHPAE_CIRCLE_RADIO");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PMM_CTRL_COLOR_FILTER_RADIO;
	WndKey = _T("PMM_CTRL_COLOR_FILTER_RADIO");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = PMM_MASK_ADD_BTN;
	WndKey = _T("PMM_MASK_ADD_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PMM_MASK_ERASE_BTN;
	WndKey = _T("PMM_MASK_ERASE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PMM_MASK_CLEAR_BTN;
	WndKey = _T("PMM_MASK_CLEAR_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PMM_MASK_ERASE_PART_BTN;
	WndKey = _T("PMM_MASK_ERASE_PART_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = PMM_MASK_ERODE_BTN;
	WndKey = _T("PMM_MASK_ERODE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PMM_MASK_DILATE_BTN;
	WndKey = _T("PMM_MASK_DILATE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PMM_MASK_OPEN_BTN;
	WndKey = _T("PMM_MASK_OPEN_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PMM_MASK_CLOSE_BTN;
	WndKey = _T("PMM_MASK_CLOSE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = PMM_SHOW_MASK_RADIO;
	WndKey = _T("PMM_SHOW_MASK_RADIO");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PMM_SHOW_IMAGE_RADIO;
	WndKey = _T("PMM_SHOW_IMAGE_RADIO");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PMM_SHOW_COMBINED_RADIO;
	WndKey = _T("PMM_SHOW_COMBINED_RADIO");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PMM_SHOW_COMPONENT_CHK;
	WndKey = _T("PMM_SHOW_COMPONENT_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PMM_USE_ROI_RECT_CHK;
	WndKey = _T("PMM_USE_ROI_RECT_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//
	WndID = PMM_RED_MASTER_CHK;
	WndKey = _T("PMM_RED_MASTER_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PMM_GREEN_MASTER_CHK;
	WndKey = _T("PMM_GREEN_MASTER_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PMM_BLUE_MASTER_CHK;
	WndKey = _T("PMM_BLUE_MASTER_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PMM_FILTER_EXPAND_BTN;
	WndKey = _T("PMM_FILTER_EXPAND_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PMM_FILTER_EXPAND_ONLY_COLOR_CHK;
	WndKey = _T("PMM_FILTER_EXPAND_ONLY_COLOR_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PMM_FILTER_SHIRNK_BTN;
	WndKey = _T("PMM_FILTER_SHIRNK_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PMM_VALUE_ENABLED_CHK;
	WndKey = _T("PMM_VALUE_ENABLED_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PMM_RESET_COLOR_BTN;
	WndKey = _T("PMM_RESET_COLOR_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PMM_RESET_ALL_BTN;
	WndKey = _T("PMM_RESET_ALL_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PMM_MERGE_ALL_BTN;
	WndKey = _T("PMM_MERGE_ALL_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//	
	WndID = PMM_GRAY_COLOR_BTN;
	WndKey = _T("PMM_GRAY_COLOR_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//	
	WndID = PMM_SHOW_GROUP_COLOR_BTN;
	WndKey = _T("PMM_SHOW_GROUP_COLOR_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PMM_GATHER_SHOW_RAW_CHK;
	WndKey = _T("PMM_GATHER_SHOW_RAW_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//	
	WndID = PMM_MASK_MERGE_BTN;
	WndKey = _T("PMM_MASK_MERGE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PMM_MASK_GRAD_BTN;
	WndKey = _T("PMM_MASK_GRAD_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//	
}
//-------------------------------------------------------------------------------------//
CString CProjectMapMaskWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_PROJECT_MAP_MASK_WND");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CBaseDialog::OnPaint() for painting messages
	RedrewWnd();
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	POINT pt = point;
	if ( JetAPI::CheckPtInCtrlWnd(this, pt, PMM_IMAGE_WND, &pt) == true ) 
	{	
		TPOINT2D WndPt=pt;
		TPOINT2D ImagePt;
		const bool bUseRoiRect=GetUseRoiRectChk();

		m_ShowRgnOuter = true;
		ImageAPI.MapWndPtToImagePt_DBL(m_ShowImageW, m_ShowImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, WndPt, ImagePt);
		m_ImagePt2 = m_ImagePt1 = ImagePt;
		
		if ( true == bUseRoiRect )
		{
			m_ImageRgnRoi.maxX = m_ImageRgnRoi.minX = ImagePt.x;
			m_ImageRgnRoi.maxY = m_ImageRgnRoi.minY = ImagePt.y;
		}
		else
		{
			m_ImageRgnOuter.maxX = m_ImageRgnOuter.minX = ImagePt.x;
			m_ImageRgnOuter.maxY = m_ImageRgnOuter.minY = ImagePt.y;
			m_ImageRgnInner = m_ImageRgnOuter;
		}		
		m_LastPt = m_LBtnUpPt = m_LBtnDownPt = pt;	
		SetCapture();
	}
	
	CBaseDialog::OnLButtonDown(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	::ReleaseCapture();
	POINT pt = point;
	JetAPI::CheckPtInCtrlWnd(this, pt, PMM_IMAGE_WND, &pt);
	m_LBtnUpPt = pt;

	bool bRedraw = false;
	const bool bUseRoiRect=GetUseRoiRectChk();
	pt.x = abs(m_LBtnUpPt.x-m_LBtnDownPt.x);
	pt.y = abs(m_LBtnUpPt.y-m_LBtnDownPt.y);
	if ( true == m_ShowRgnOuter )
	{		
		TPOINT2D ImagePt;
		TPOINT2D WndPt=m_LBtnUpPt;
		bRedraw = true;
		ImageAPI.MapWndPtToImagePt_DBL(m_ShowImageW, m_ShowImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, WndPt, ImagePt);
		m_ImagePt2 = ImagePt;
		if ( true == bUseRoiRect )
		{
			m_ImageRgnRoi.minX = MIN(m_ImagePt1.x, m_ImagePt2.x);
			m_ImageRgnRoi.minY = MIN(m_ImagePt1.y, m_ImagePt2.y);
			m_ImageRgnRoi.maxX = MAX(m_ImagePt1.x, m_ImagePt2.x);
			m_ImageRgnRoi.maxY = MAX(m_ImagePt1.y, m_ImagePt2.y);
		}
		else
		{
			m_ImageRgnOuter.minX = MIN(m_ImagePt1.x, m_ImagePt2.x);
			m_ImageRgnOuter.minY = MIN(m_ImagePt1.y, m_ImagePt2.y);
			m_ImageRgnOuter.maxX = MAX(m_ImagePt1.x, m_ImagePt2.x);
			m_ImageRgnOuter.maxY = MAX(m_ImagePt1.y, m_ImagePt2.y);
			CalcColorFilterParam();
		}		
	}
//	if ( pt.x<2 && pt.y<2 )
//	{	SwitchShowImage();	}	
	m_LBtnUpPt.x = m_LBtnUpPt.y = -1;	
	m_LastPt = m_LBtnDownPt = m_LBtnUpPt;	

	if ( true == bRedraw )
	{	RedrewWnd(); }
	CBaseDialog::OnLButtonUp(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CBaseDialog::OnLButtonDblClk(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	POINT pt = point;
	CWnd::MapWindowPoints(&m_ImageWnd, &pt, 1);	
	m_CurrentPt = pt;
	if ( this != GetCapture() )
	{
		RedrewWnd();
		CBaseDialog::OnMouseMove(nFlags, point);
		return;
	}
	
	UINT Res=0;
	bool bRedraw=false;
	POINT ptoffset;	
	ptoffset.x = m_CurrentPt.x-m_LastPt.x;
	ptoffset.y = m_CurrentPt.y-m_LastPt.y;

	Res = nFlags&MK_LBUTTON;
	if ( 0 != Res )
	{
		if ( true == m_ShowRgnOuter )
		{			
			TPOINT2D WndPt=pt;
			TPOINT2D ImagePt;
			const bool bUseRoiRect=GetUseRoiRectChk();

			bRedraw = true;
			ImageAPI.MapWndPtToImagePt_DBL(m_ShowImageW, m_ShowImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, WndPt, ImagePt);
			m_ImagePt2 = ImagePt;
			if ( true == bUseRoiRect )
			{
				m_ImageRgnRoi.minX = MIN(m_ImagePt1.x, m_ImagePt2.x);
				m_ImageRgnRoi.minY = MIN(m_ImagePt1.y, m_ImagePt2.y);
				m_ImageRgnRoi.maxX = MAX(m_ImagePt1.x, m_ImagePt2.x);
				m_ImageRgnRoi.maxY = MAX(m_ImagePt1.y, m_ImagePt2.y);
			}
			else
			{
				m_ImageRgnOuter.minX = MIN(m_ImagePt1.x, m_ImagePt2.x);
				m_ImageRgnOuter.minY = MIN(m_ImagePt1.y, m_ImagePt2.y);
				m_ImageRgnOuter.maxX = MAX(m_ImagePt1.x, m_ImagePt2.x);
				m_ImageRgnOuter.maxY = MAX(m_ImagePt1.y, m_ImagePt2.y);
			}
		}
	}

	Res = nFlags&MK_RBUTTON;
	if ( 0 != Res )
	{
		bRedraw = true;
		m_ImageOffset.x += ptoffset.x;
		m_ImageOffset.y += ptoffset.y;
		DrawShowImage();		
	}
	if ( true == bRedraw )
	{	RedrewWnd(); }
	m_LastPt = m_CurrentPt;
	CBaseDialog::OnMouseMove(nFlags, point);
}
//-------------------------------------------------------------------------------------//
bool CProjectMapMaskWnd::BuildFrameIndexCombox()
{
	CComboBox &Combox=m_FrameIndexComobx;
	JetAPI::ClearCombox(Combox);
	CAOIProject *ProjectPtr=GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }
	
	std::vector<unsigned int> FrameUniqueIDList;	
	ProjectPtr->GetProjectFrameUniqueIDList(FrameUniqueIDList);
	const size_t FrameUniqueIDCount=FrameUniqueIDList.size();
	for ( size_t i=0; i<FrameUniqueIDCount; i++ )
	{
		CString FrameName;
		unsigned int UniqueID=FrameUniqueIDList[i];
		TFrameParam *FrameParamPtr=AOIDataCollect.GetSystemFrameParamPtrByUniqueID(UniqueID);
		if ( NULL == FrameParamPtr )
		{	FrameName.Format(_T("%d"), i+1);	}
		else
		{	FrameName = FrameParamPtr->FrameName;	}
		Combox.InsertString(i, FrameName);
		Combox.SetItemData(i, i);
	}
	JetAPI::SetComboxCurSel(Combox, m_MapIndex);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectMapMaskWnd::BuildMaskIndexCombox()
{
	CComboBox &Combox = m_MaskIndexComobx;
	m_MaskIndex = 0;
	const size_t nMaskCount = 4;
	JetAPI::ClearCombox(Combox);
	CString ItemName;
	ItemName = _T("Mask merged");
	Combox.InsertString(0, ItemName);
	Combox.SetItemData(0, 0);
	for (size_t i = 1; i<nMaskCount+1; i++)
	{
		ItemName.Format(_T("Mask %d"), i);
		Combox.InsertString(i, ItemName);
		Combox.SetItemData(i, i);
	}
	JetAPI::SetComboxCurSel(Combox, m_MaskIndex);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectMapMaskWnd::ExecMouseWheelMSG(UINT message, WPARAM wParam, LPARAM lParam)
{
	if ( WM_MOUSEWHEEL != message ) { return false; }
	CWnd *pWnd = GetFocus();
	if ( NULL == pWnd ) { return false; }
	if ( this != pWnd )
	{	pWnd = pWnd->GetParent();	}	
	if ( this != pWnd ) { return false; }

	CPoint pt, point;
	point.x = pt.x = GET_X_LPARAM(lParam); 
	point.y = pt.y = GET_Y_LPARAM(lParam); 
	this->ScreenToClient(&point);
	if ( JetAPI::CheckPtInCtrlWnd(this, point, PMM_IMAGE_WND, NULL) == false ) 
	{	return false; }

	UINT nFlags = GET_KEYSTATE_WPARAM(wParam);
	short zDelta = GET_WHEEL_DELTA_WPARAM(wParam);	
	if ( ExecMouseWheelEvent(nFlags, zDelta, pt) == true )
	{	return TRUE; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectMapMaskWnd::ExecMouseWheelEvent(UINT nFlags, short zDelta, CPoint pt)
{
	UINT Res=0;
	Res = nFlags&MK_MBUTTON;
	double NextImageZoom = m_ImageZoom;
	if ( zDelta > 0 ) 
	{	NextImageZoom *= 1.1;	}
	else
	{	NextImageZoom /= 1.1;	}		
	NextImageZoom = AOIDataCollect.AdjustImageZoom(NextImageZoom);
	ImageAPI.CalcImageWndZoom(m_ImageZoom, NextImageZoom, m_ImageOffset);
	m_ImageZoom = NextImageZoom;

	CColorRGBV *rgbvPtr = m_RGBVWnd.GetColorRGBVPtr();
	BuildColorFilterImage(rgbvPtr);
	DrawShowImage();
	RedrewWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
BOOL CProjectMapMaskWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	return CBaseDialog::OnMouseWheel(nFlags, zDelta, pt);
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	POINT pt = point;
	if ( JetAPI::CheckPtInCtrlWnd(this, pt, PMM_IMAGE_WND, &pt) == true ) 
	{	
		m_LastPt = m_RBtnUpPt = m_RBtnDownPt = pt;	
		SetCapture();
	}
	
	CBaseDialog::OnRButtonDown(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	::ReleaseCapture();
	POINT pt = point;
	JetAPI::CheckPtInCtrlWnd(this, pt, PMM_IMAGE_WND, &pt);
	m_RBtnUpPt = pt;

	pt.x = abs(m_RBtnUpPt.x-m_RBtnDownPt.x);
	pt.y = abs(m_RBtnUpPt.y-m_RBtnDownPt.y);
	if ( pt.x<2 && pt.y<2 )
	{	SwitchShowImage();	}
	else
	{
		CColorRGBV *rgbvPtr = m_RGBVWnd.GetColorRGBVPtr();
		BuildColorFilterImage(rgbvPtr);
	}
	m_RBtnUpPt.x = m_RBtnUpPt.y = -1;	
	m_LastPt = m_RBtnDownPt = m_RBtnUpPt;	
	CBaseDialog::OnRButtonUp(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	POINT pt = point;
	if ( JetAPI::CheckPtInCtrlWnd(this, pt, PMM_IMAGE_WND) == true ) 
	{
		//SwitchShowImage();
	}
	CBaseDialog::OnRButtonDblClk(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::ClearMaskImageBuffer()
{
	//if ( NULL != m_MapMaskImagePtr )
	//{	JetMemory.free_func(m_MapMaskImagePtr); }
	if ( NULL != m_MapMaskImagePtr0)
	{	JetMemory.free_func(m_MapMaskImagePtr0); }
	if ( NULL != m_MapMaskImagePtr1)
	{	JetMemory.free_func(m_MapMaskImagePtr1); }
	if ( NULL != m_MapMaskImagePtr2)
	{	JetMemory.free_func(m_MapMaskImagePtr2); }
	if ( NULL != m_MapMaskImagePtr3)
	{	JetMemory.free_func(m_MapMaskImagePtr3); }
	if ( NULL != m_MapMaskImagePtr4)
	{	JetMemory.free_func(m_MapMaskImagePtr4); }
	m_MapMaskImagePtr = NULL;
	m_MapMaskImageW = 1024;
	m_MapMaskImageH = 1024;
	m_MapMaskImageStep = 1024;
	m_MapMaskBitCount = 8;
}
//-------------------------------------------------------------------------------------//
bool CProjectMapMaskWnd::CreateMaskImageBuffer()
{
	ClearMaskImageBuffer();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }

	IMAGE_PTR ImagePtr = NULL;
	const int nAlign = 4;
	const IMAGE_SIZE ImageW = m_ProjectMapW;
	const IMAGE_SIZE ImageH = m_ProjectMapH;
	const IMAGE_SIZE BitCount = 8;
	const IMAGE_SIZE ImageStep = JetAPI::GetBMPImagePixelsPerLine(ImageW, BitCount, nAlign);	
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	if ( JetMemory.alloc_func(BufferSize, ImagePtr, "CProjectMapMaskWnd::CreateMaskImageBuffer()", "MapMaskPtr") == false )
	{	return false; }

	IMAGE_PTR  ProjectMaskPtr=NULL;
	IMAGE_SIZE ProjectMaskW = 0;
	IMAGE_SIZE ProjectMaskH = 0;
	IMAGE_SIZE ProjectMaskStep = 0;
	IMAGE_SIZE ProjectMaskBitCount = 0;
	ProjectMaskPtr = ProjectPtr->GetProjectMapMaskImage(ProjectMaskW, ProjectMaskH, ProjectMaskStep, ProjectMaskBitCount);

	if ( NULL==ProjectMaskPtr ||
		 ImageW != ProjectMaskW ||
		 ImageH != ProjectMaskH ||
		 ImageStep != ProjectMaskStep ||
		 BitCount != ProjectMaskBitCount )
	{	::memset(ImagePtr, 0xFF, sizeof(IMAGE_DATA)*BufferSize);	}
	else
	{	::memcpy(ImagePtr, ProjectMaskPtr, sizeof(IMAGE_DATA)*BufferSize);	}

	m_MapMaskImagePtr = ImagePtr;
	m_MapMaskImageW = ImageW;
	m_MapMaskImageH = ImageH;
	m_MapMaskImageStep = ImageStep;
	m_MapMaskBitCount = BitCount;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectMapMaskWnd::CreateMaskImageBuffer_Multi()
{
	ClearMaskImageBuffer();
	CAOIProject *ProjectPtr = GetActiveProject();
	if (NULL == ProjectPtr) { return true; }

	IMAGE_PTR ImagePtr = NULL;
	IMAGE_PTR ImagePtr1 = NULL;
	IMAGE_PTR ImagePtr2 = NULL;
	IMAGE_PTR ImagePtr3 = NULL;
	IMAGE_PTR ImagePtr4 = NULL;

	const int nAlign = 4;
	const IMAGE_SIZE ImageW = m_ProjectMapW;
	const IMAGE_SIZE ImageH = m_ProjectMapH;
	const IMAGE_SIZE BitCount = 8;
	const IMAGE_SIZE ImageStep = JetAPI::GetBMPImagePixelsPerLine(ImageW, BitCount, nAlign);
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	if (
		(JetMemory.alloc_func(BufferSize, ImagePtr, "CProjectMapMaskWnd::CreateMaskImageBuffer()", "MapMaskPtr") == false) ||
		(JetMemory.alloc_func(BufferSize, ImagePtr1, "CProjectMapMaskWnd::CreateMaskImageBuffer()", "MapMaskPtr1") == false) ||
		(JetMemory.alloc_func(BufferSize, ImagePtr2, "CProjectMapMaskWnd::CreateMaskImageBuffer()", "MapMaskPtr2") == false) ||
		(JetMemory.alloc_func(BufferSize, ImagePtr3, "CProjectMapMaskWnd::CreateMaskImageBuffer()", "MapMaskPtr3") == false) ||
		(JetMemory.alloc_func(BufferSize, ImagePtr4, "CProjectMapMaskWnd::CreateMaskImageBuffer()", "MapMaskPtr4") == false) 
		)
	{
		return false;
	}

	IMAGE_PTR  ProjectMaskPtr = NULL;
	IMAGE_SIZE ProjectMaskW = 0;
	IMAGE_SIZE ProjectMaskH = 0;
	IMAGE_SIZE ProjectMaskStep = 0;
	IMAGE_SIZE ProjectMaskBitCount = 0;
	ProjectMaskPtr = ProjectPtr->GetProjectMapMaskImage(ProjectMaskW, ProjectMaskH, ProjectMaskStep, ProjectMaskBitCount);

	if (NULL == ProjectMaskPtr ||
		ImageW != ProjectMaskW ||
		ImageH != ProjectMaskH ||
		ImageStep != ProjectMaskStep ||
		BitCount != ProjectMaskBitCount)
	{
		::memset(ImagePtr, 0xFF, sizeof(IMAGE_DATA)*BufferSize);
	}
	else
	{
		::memcpy(ImagePtr, ProjectMaskPtr, sizeof(IMAGE_DATA)*BufferSize);
	}
	::memset(ImagePtr1, 0xFF, sizeof(IMAGE_DATA)*BufferSize);
	::memset(ImagePtr2, 0xFF, sizeof(IMAGE_DATA)*BufferSize);
	::memset(ImagePtr3, 0xFF, sizeof(IMAGE_DATA)*BufferSize);
	::memset(ImagePtr4, 0xFF, sizeof(IMAGE_DATA)*BufferSize);

	m_MapMaskImagePtr = ImagePtr;
	m_MapMaskImagePtr0 = ImagePtr;
	m_MapMaskImagePtr1 = ImagePtr1;
	m_MapMaskImagePtr2 = ImagePtr2;
	m_MapMaskImagePtr3 = ImagePtr3;
	m_MapMaskImagePtr4 = ImagePtr4;

	m_MapMaskImageW = ImageW;
	m_MapMaskImageH = ImageH;
	m_MapMaskImageStep = ImageStep;
	m_MapMaskBitCount = BitCount;
	return true;
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::ClearShowImageBuffer()
{
	if ( NULL != m_ShowImagePtr )
	{	JetMemory.free_func(m_ShowImagePtr); }
	m_ShowImagePtr = NULL;
	m_ShowImageW = 1024;
	m_ShowImageH = 1024;
	m_ShowImageStep = 1024*3;
	m_ShowBitCount = 24;
}
//-------------------------------------------------------------------------------------//
bool CProjectMapMaskWnd::CreateShowImageBuffer()
{
	ClearShowImageBuffer();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }
	IMAGE_PTR ImagePtr = NULL;
	const int nAlign = 4;
	const IMAGE_SIZE ImageW = m_ProjectMapW;
	const IMAGE_SIZE ImageH = m_ProjectMapH;
	const IMAGE_SIZE BitCount = 24;
	const IMAGE_SIZE ImageStep = JetAPI::GetBMPImagePixelsPerLine(ImageW, BitCount, nAlign);	
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	if ( JetMemory.alloc_func(BufferSize, ImagePtr, "CProjectMapMaskWnd::CreateShowImageBuffer()", "MapMaskPtr") == false )
	{	return false; }
	::memset(ImagePtr, 0x00, sizeof(IMAGE_DATA)*BufferSize);
	
	m_ShowImagePtr = ImagePtr;
	m_ShowImageW = ImageW;
	m_ShowImageH = ImageH;
	m_ShowImageStep = ImageStep;
	m_ShowBitCount = BitCount;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectMapMaskWnd::BuildShowImage()
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }
	if ( NULL == m_MapMaskImagePtr ) { return false; }
	if ( NULL == m_ShowImagePtr ) { return false; }
	if ( m_ShowImageW != m_MapMaskImageW ) { return false; }
	if ( m_ShowImageH != m_MapMaskImageH ) { return false; }
	IMAGE_PTR ImagePtr = NULL;
	IMAGE_SIZE ImageW = 0;
	IMAGE_SIZE ImageH = 0;
	IMAGE_SIZE BitCount = 0;
	IMAGE_SIZE ImageStep = 0;
	const int MapIndex = m_MapIndex;
	if ( ProjectPtr->GetProjectMapPtr(MapIndex, ImageW, ImageH, ImageStep, BitCount, ImagePtr) == false )
	{	return false; }
	if ( ProjectPtr->CreateProjectMapShowPtr(MapIndex, ImagePtr, false) == false )
	{	return false; }	
	if ( m_ShowImageW != ImageW ) { return false; }
	if ( m_ShowImageH != ImageH ) { return false; }
	if ( NULL == ImagePtr ) { return false; }
	unsigned int FrameUniqueID = ProjectPtr->GetProjectFrameUniqueID(MapIndex, true);
	
	size_t i=0, j=0;
	size_t maskIdx=0, imgIdx=0, showIdx=0;
	unsigned char maskR=0, maskG=0, maskB=0, maskV=0, Alpha=0;
	const size_t BufferSize = ImageAPI.CalcBufferSize(m_ShowImageStep, m_ShowImageH);
	AOIDataCollect.GetMaskImageColor(FrameUniqueID, maskR, maskG, maskB, maskV, Alpha);//取得遮罩影像顏色

	maskR = 255-maskR;
	maskG = 255-maskG;
	maskB = 255-maskB;

	if ( PMM_SHOW_MASK_RADIO == m_ShowMode )
	{
		for ( i=0; i<ImageH; i++ )
		{
			for ( j=0; j<ImageW; j++ )
			{				
				maskIdx = (i*m_MapMaskImageStep)+j;
				showIdx = (i*m_ShowImageStep)+(j*3);
				if ( 0 == m_MapMaskImagePtr[maskIdx] )
				{
					m_ShowImagePtr[showIdx] = maskB;
					m_ShowImagePtr[showIdx+1] = maskG;
					m_ShowImagePtr[showIdx+2] = maskR;
				}
				else
				{	
					m_ShowImagePtr[showIdx] = 255;
					m_ShowImagePtr[showIdx+1] = 255;
					m_ShowImagePtr[showIdx+2] = 255;
				}
		
			}
		}
	}
	else if ( PMM_SHOW_IMAGE_RADIO == m_ShowMode )
	{
		if ( 24 == BitCount )
		{
			for ( i=0; i<ImageH; i++ )
			{
				for ( j=0; j<ImageW; j++ )
				{	
					imgIdx = (i*ImageStep)+(j*3);
					showIdx = (i*m_ShowImageStep)+(j*3);
					m_ShowImagePtr[showIdx] = ImagePtr[imgIdx];
					m_ShowImagePtr[showIdx+1] = ImagePtr[imgIdx+1];
					m_ShowImagePtr[showIdx+2] = ImagePtr[imgIdx+2];					
			
				}
			}
		}
		else
		{
			for ( i=0; i<ImageH; i++ )
			{
				for ( j=0; j<ImageW; j++ )
				{	
					imgIdx = (i*ImageStep)+j;
					showIdx = (i*m_ShowImageStep)+(j*3);
					m_ShowImagePtr[showIdx] = ImagePtr[imgIdx];
					m_ShowImagePtr[showIdx+1] = ImagePtr[imgIdx];
					m_ShowImagePtr[showIdx+2] = ImagePtr[imgIdx];					
				}
			}
		}
	}
	else if ( PMM_SHOW_COMBINED_RADIO == m_ShowMode )
	{
		if ( 24 == BitCount )
		{
			for ( i=0; i<ImageH; i++ )
			{
				for ( j=0; j<ImageW; j++ )
				{				
					maskIdx = (i*m_MapMaskImageStep)+j;
					showIdx = (i*m_ShowImageStep)+(j*3);
					if ( 0 == m_MapMaskImagePtr[maskIdx] )
					{
						m_ShowImagePtr[showIdx] = maskB;
						m_ShowImagePtr[showIdx+1] = maskG;
						m_ShowImagePtr[showIdx+2] = maskR;
					}
					else
					{
						imgIdx = (i*ImageStep)+(j*3);					
						m_ShowImagePtr[showIdx] = ImagePtr[imgIdx];
						m_ShowImagePtr[showIdx+1] = ImagePtr[imgIdx+1];
						m_ShowImagePtr[showIdx+2] = ImagePtr[imgIdx+2];
					}
			
				}
			}
		}
		else
		{
			for ( i=0; i<ImageH; i++ )
			{
				for ( j=0; j<ImageW; j++ )
				{				
					maskIdx = (i*m_MapMaskImageStep)+j;
					showIdx = (i*m_ShowImageStep)+(j*3);
					if ( 0 == m_MapMaskImagePtr[maskIdx] )
					{
						m_ShowImagePtr[showIdx] = maskB;
						m_ShowImagePtr[showIdx+1] = maskG;
						m_ShowImagePtr[showIdx+2] = maskR;
					}
					else
					{
						imgIdx = (i*ImageStep)+j;
						m_ShowImagePtr[showIdx] = ImagePtr[imgIdx];
						m_ShowImagePtr[showIdx+1] = ImagePtr[imgIdx];
						m_ShowImagePtr[showIdx+2] = ImagePtr[imgIdx];
					}
				}
			}
		}
	}
	else 
	{	::memset(m_ShowImagePtr, 0x00, sizeof(IMAGE_DATA)*BufferSize);	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectMapMaskWnd::DrawShowImage()
{	
	HDC hDC = m_ImageWndMemDC.GetSafeHdc();
	if ( NULL == hDC ) { return false; }
	
	RECT WndRect = m_ImageWndRect;
	COLORREF clrBK = m_BkColor;
	double   ZoomScale = m_ImageZoom;
	TPOINT2D ImageOffset = m_ImageOffset;		
	HBRUSH hBrush = ::CreateSolidBrush(clrBK);
	if ( NULL != hBrush )
	{
		::FillRect(hDC, &WndRect, hBrush);
		::DeleteObject(hBrush); hBrush = NULL;
	}
	if ( NULL == m_ShowImagePtr ) { return false; }
	if ( ImageAPI.DrawImageToDC(hDC, m_ShowImageW, m_ShowImageH, m_ShowImageStep, m_ShowBitCount, m_ShowImagePtr, WndRect, ImageOffset, ZoomScale, clrBK) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::SwitchShowImage()
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }	
	m_MapIndex = ProjectPtr->GetProjectMapIndexNext(m_MapIndex);
	ProjectPtr->SetProjectMapIndex(m_MapIndex);
	JetAPI::SetComboxCurSel(m_FrameIndexComobx, m_MapIndex);
	BuildShowImage();
	DrawShowImage();
	RedrewWnd();
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::RedrewWnd()
{
	CClientDC dc(&m_ImageWnd);
	HDC hDC = dc.GetSafeHdc();
	HDC hMemDC = m_ImageWndMemDC.GetSafeHdc();
	HDC hMemDC2 = m_ImageWndMemDC2.GetSafeHdc();	
	if ( NULL==hDC || NULL==hMemDC || NULL==hMemDC2 ) { return ; }

	RECT WndRect=m_ImageWndRect;
	::BitBlt(hMemDC2, 0, 0, m_ImageWndRect.right, m_ImageWndRect.bottom, hMemDC, 0, 0, SRCCOPY );
	::IntersectClipRect(hMemDC2, WndRect.left, WndRect.top, WndRect.right, WndRect.bottom);	

	//Draw Line List
	HDC hDCUsed = hMemDC2;//hDCUsed = hDC;		
	DrawComponent(hDCUsed);
	DrawCursorLine(hDCUsed);
	DrawImageRgn(hDCUsed);	
	DrawImageRoi(hDCUsed);	
	::BitBlt(hDC, 0, 0, m_ImageWndRect.right, m_ImageWndRect.bottom, hDCUsed, 0, 0, SRCCOPY );
	return;
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::DrawRect(HDC hDC, const RECT &Rect)
{
	if ( NULL == hDC ) { return; }
	::MoveToEx(hDC, Rect.left, Rect.top, NULL);
	::LineTo(hDC, Rect.right, Rect.top);
	::LineTo(hDC, Rect.right, Rect.bottom);
	::LineTo(hDC, Rect.left, Rect.bottom);
	::LineTo(hDC, Rect.left, Rect.top);
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::DrawCircle(HDC hDC, const RECT &Rect)
{
	if ( NULL == hDC ) { return; }
	::Arc(hDC, Rect.left, Rect.top, Rect.right, Rect.bottom, Rect.left, Rect.top, Rect.left, Rect.top);
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::DrawRectLine(HDC hDC, const POINT pt[], size_t num)
{
	if ( 4 == num )
	{
		::MoveToEx(hDC, pt[0].x, pt[0].y, NULL);		
		::LineTo(hDC, pt[1].x, pt[1].y);
		::LineTo(hDC, pt[2].x, pt[2].y);
		::LineTo(hDC, pt[3].x, pt[3].y);
		::LineTo(hDC, pt[0].x, pt[0].y);
	}
	else
	{
		size_t i=0;
		for ( i=0; i<num; i++ )
		{
			if ( 0 == i ) 
			{	::MoveToEx(hDC, pt[i].x, pt[i].y, NULL);	}
			else
			{	::LineTo(hDC, pt[i].x, pt[i].y);	}
		}
		if ( 1 == num )
		{	::LineTo(hDC, pt[0].x, pt[0].y);	}
	}
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::DrawImageRgn(HDC hDC)
{
	if ( NULL == hDC ) { return; }
	if ( true == m_ShowRgnOuter ) 
	{
		RECT Rect;
		TREGION4D WndRgn;
		HPEN hPen = ::CreatePen(PS_SOLID, 1, 0xFFFFFF);
		HPEN hOldPen = (HPEN)::SelectObject(hDC, hPen);
		
		ImageAPI.MapImageRgnToWndRgn_DBL(m_ShowImageW, m_ShowImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, m_ImageRgnOuter, WndRgn);
		JetAPI::Region4DToRect(WndRgn, Rect, true);

		switch ( m_CtrlMode )
		{
		case PMM_CTRL_SHPAE_CIRCLE_RADIO:
			DrawCircle(hDC, Rect);			
			break;
		case PMM_CTRL_COLOR_FILTER_RADIO:
			DrawRect(hDC, Rect);
			break;
		default:
			DrawRect(hDC, Rect);
			break;
		}		
		::SelectObject(hDC, hOldPen);
		::DeleteObject(hPen); hPen = NULL;
		/*
		HBRUSH hBrush = ::CreateHatchBrush(HS_DIAGCROSS, 0x00FFFF);
		::FillRect(hDC, &Rect, hBrush);
		::DeleteObject(hBrush); hBrush = NULL;
		//*/
	}
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::DrawImageRoi(HDC hDC)
{
	if ( NULL == hDC ) { return; }
	if ( true == m_ShowRgnOuter ) 
	{
		RECT Rect;
		TREGION4D WndRgn;
		const bool bUseRoiRect=GetUseRoiRectChk();
		//if ( true == bUseRoiRect )
		{
			HPEN hPen = ::CreatePen(PS_SOLID, 1, 0x00FFFF);
			HPEN hOldPen = (HPEN)::SelectObject(hDC, hPen);		
			ImageAPI.MapImageRgnToWndRgn_DBL(m_ShowImageW, m_ShowImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, m_ImageRgnRoi, WndRgn);
			JetAPI::Region4DToRect(WndRgn, Rect, true);
			switch ( m_CtrlMode )
			{
			case PMM_CTRL_SHPAE_CIRCLE_RADIO:
				DrawCircle(hDC, Rect);			
				DrawRect(hDC, Rect);
				break;
			case PMM_CTRL_COLOR_FILTER_RADIO:
				DrawRect(hDC, Rect);
				break;
			default:
				DrawRect(hDC, Rect);
				break;
			}	
			::SelectObject(hDC, hOldPen);
			::DeleteObject(hPen); hPen = NULL;
			/*
			HBRUSH hBrush = ::CreateHatchBrush(HS_DIAGCROSS, 0x00FFFF);
			::FillRect(hDC, &Rect, hBrush);
			::DeleteObject(hBrush); hBrush = NULL;
			//*/
		}
	}
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::DrawComponent(HDC hDC)
{
	if ( NULL == hDC ) { return; }
	BOOL bChk = CWnd::IsDlgButtonChecked(PMM_SHOW_COMPONENT_CHK);
	if ( FALSE == bChk ) { return ; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }

	CString      str;	
	size_t       i = 0;	
	size_t       ComponentFinishCount=0;	
	IMAGE_SIZE   ImageW = m_ShowImageW;
	IMAGE_SIZE   ImageH = m_ShowImageH;
	POINT        Pt={0}, CornerPos[4];
	RECT         Rect={0};	
	RECT         WndRect = m_ImageWndRect;
	TPOINT2D     StagePos, StageOffset, ImagePos, WndPt;
	TPOINT2D     StgCornerPos[4], ImgCornerPos[4], WndCornerPos[4];	
	TPOINT2D     ImageStagePos;	
	TPOINT2D     ImageRes = m_FrameResolution;
	TREGION4D    ImageStageRgn = m_FrameStageRgn;
	TREGION4D    ObjStageRgn, ObjImageRgn;
	REGION_CALC_STATE  RgnCalcState;	
	RESULT_ID ResultID = RESULT_ID_NONE;

	bool         ShowRect = true;
	bool         ShowName = true;	
	COMPONENT_TYPE ComponentType;
	CAOIComponent *ComponentPtr = NULL;	
	const size_t ComponentCount = ProjectPtr->GetProjectComponentCount();	
	const DRAW_MODEL_MODE DrawModelMode = AOIDataCollect.GetDrawModelMode();	
	const TSystemParameter &SystemParam = AOIDataCollect.GetSystemParameter();
	const COLORREF  clr1 = SystemParam.m_ComponentColor1;
	const COLORREF  clr2 = SystemParam.m_ComponentColor2;
	const COLORREF  clrOK = SystemParam.m_InspectedResultOKColor;
	const COLORREF  clrNG = SystemParam.m_InspectedResultNGColor;
	const COLORREF  clrSkip = SystemParam.m_InspectedResultSkipColor;
	const COLORREF  clrText = SystemParam.m_ComponentTextColor;
	const COLORREF  clrSelected = SystemParam.m_ComponentSelectedColor;	
	const COLORREF  clrBypassed = SystemParam.m_InspectedResultBypassColor;	
	const COLORREF  clrException = SystemParam.m_InspectedResultExceptionColor;

	HPEN hPen  = ::CreatePen(PS_SOLID, 1, clr1);
	HPEN hPen2 = ::CreatePen(PS_SOLID, 1, clr2);	
	HPEN hPenOK = ::CreatePen(PS_SOLID, 1, clrOK);
	HPEN hPenNG = ::CreatePen(PS_SOLID, 1, clrNG);
	HPEN hPenSkip = ::CreatePen(PS_SOLID, 1, clrSkip);
	HPEN hPenBypass = ::CreatePen(PS_SOLID, 1, clrBypassed);
	HPEN hPenException = ::CreatePen(PS_SOLID, 1, clrException);
	HPEN hOldPen = (HPEN)(::SelectObject(hDC, hPen));	
	COLORREF clrTextOld = ::SetTextColor(hDC, clrText);
	int BKMode = ::SetBkMode(hDC, TRANSPARENT);

	ImageStagePos.x = ImageStageRgn.GetCpX();
	ImageStagePos.y = ImageStageRgn.GetCpY();		
	if ( m_ImageZoom < 1.5 )
	{	ShowName = true;	}
	else
	{	ShowName = false;	}
	for ( i=0; i<ComponentCount; i++ )
	{	
		ComponentPtr = ProjectPtr->GetProjectComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }
		if ( ComponentPtr->CheckComponentIsAgent() == true ) { continue; }
		ComponentPtr = ComponentPtr->GetComponentResultPtr();

		RgnCalcState = ComponentPtr->GetRgnCalcState();
		ComponentType = ComponentPtr->GetComponentType();
		ResultID = ComponentPtr->GetComponentResultID_AOI();
		if ( RESULT_ID_NONE != ResultID )
		{	ComponentFinishCount ++;	}
		else
		{
			if ( ComponentPtr->CheckComponentModelEnabled() == false )
			{	ComponentFinishCount ++;	}
		}
		if ( false == ShowRect ) { continue; }
		if ( AOIDataCollect.CheckComponentTypeVisible(ComponentType) == false )
		{	continue; }
		StagePos.x = ComponentPtr->GetComponentStagePosX();
		StagePos.y = ComponentPtr->GetComponentStagePosY();
		if ( DRAW_MODEL_RESULT == DrawModelMode )
		{
			StageOffset.x = ComponentPtr->GetComponentStageOffsetX();
			StageOffset.y = ComponentPtr->GetComponentStageOffsetY();
			StagePos.x += StageOffset.x;
			StagePos.y += StageOffset.y;
		}
		if ( StagePos.x<ImageStageRgn.minX || StagePos.x>ImageStageRgn.maxX ) { continue; }
		if ( StagePos.y<ImageStageRgn.minY || StagePos.y>ImageStageRgn.maxY ) { continue; }
		AOIDataCollect.MapStagePtToCamera(ImageW, ImageH, ImageRes, StagePos, ImageStagePos, ImagePos);
		ImageAPI.MapImagePtToWndPt_DBL(ImageW, ImageH, WndRect, m_ImageOffset, m_ImageZoom, ImagePos, WndPt);
		
		JetAPI::Point2DToPoint(WndPt, Pt);
		if ( Pt.x<0 || Pt.y<0 || Pt.x>WndRect.right || Pt.y>WndRect.bottom ) { continue; }
		
		ComponentPtr->GetComponentBodyStageCornerPos(StgCornerPos);
		if ( DRAW_MODEL_RESULT == DrawModelMode )
		{
			StgCornerPos[0].x += StageOffset.x;	StgCornerPos[0].y += StageOffset.y;
			StgCornerPos[1].x += StageOffset.x;	StgCornerPos[1].y += StageOffset.y;
			StgCornerPos[2].x += StageOffset.x;	StgCornerPos[2].y += StageOffset.y;
			StgCornerPos[3].x += StageOffset.x;	StgCornerPos[3].y += StageOffset.y;
		}
		AOIDataCollect.MapStageCornerToCamera(ImageW, ImageH, ImageRes, StgCornerPos, ImageStagePos, ImgCornerPos);		
		ImageAPI.MapImagePtToWndPt_DBL(ImageW, ImageH, WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[0], WndCornerPos[0]);
		ImageAPI.MapImagePtToWndPt_DBL(ImageW, ImageH, WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[1], WndCornerPos[1]);
		ImageAPI.MapImagePtToWndPt_DBL(ImageW, ImageH, WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[2], WndCornerPos[2]);
		ImageAPI.MapImagePtToWndPt_DBL(ImageW, ImageH, WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[3], WndCornerPos[3]);

		JetAPI::CornerPt2DToCornerPt(WndCornerPos, CornerPos);
		if ( JetAPI::CheckCornerInRect(CornerPos, WndRect) == false )
		{	continue; }		
		switch ( ResultID )
		{
		case RESULT_ID_NONE:	::SelectObject(hDC, hPen);	break;
		case RESULT_ID_OK:		::SelectObject(hDC, hPenOK);break;
		case RESULT_ID_NG:		::SelectObject(hDC, hPenNG);break;
		case RESULT_ID_SKIP:	::SelectObject(hDC, hPenSkip);break;
		case RESULT_ID_BYPASS:	::SelectObject(hDC, hPenBypass);break;
		case RESULT_ID_EXCEPTION:
			::SelectObject(hDC, hPenException);
			break;
		default:
			::SelectObject(hDC, hPen2);
			break;
		}

		DrawRectLine(hDC, CornerPos, 4);
		if ( true==ShowName )
		{
			str = ComponentPtr->GetComponentName();
			//::TextOut(hDC, Rect.left, Rect.top, str, str.GetLength());
			::TextOut(hDC, Pt.x, Pt.y, str, str.GetLength());
		}
	}	
	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPen); hPen = NULL;	
	::DeleteObject(hPen2); hPen2 = NULL;			
	::DeleteObject(hPenOK); hPenOK=NULL;
	::DeleteObject(hPenNG); hPenNG=NULL;
	::DeleteObject(hPenSkip); hPenSkip=NULL;
	::DeleteObject(hPenBypass); hPenBypass=NULL;
	::DeleteObject(hPenException); hPenException=NULL;	
	::SetBkMode(hDC, BKMode);
	::SetTextColor(hDC, clrTextOld);
	return;
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::DrawCursorLine(HDC hDC)
{
	bool ShowCursorLine = true;
	if ( NULL == hDC ) { return; }
	if ( true == ShowCursorLine ) 
	{
		POINT pt = m_CurrentPt;
		RECT WndRect = m_ImageWndRect;

		if ( ::PtInRect(&WndRect, pt) == TRUE )
		{
			HPEN hPen = ::CreatePen(PS_DOT, 1, 0xFF00FF);
			HPEN hOldPen = (HPEN)::SelectObject(hDC, hPen);
		
			::MoveToEx(hDC, pt.x, WndRect.top, NULL);
			::LineTo(hDC, pt.x, WndRect.bottom);

			::MoveToEx(hDC, WndRect.left, pt.y, NULL);
			::LineTo(hDC, WndRect.right, pt.y);		
		
			::SelectObject(hDC, hOldPen);
			::DeleteObject(hPen); hPen = NULL;		
		}
	}
	return ;
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::OnSelchangeFrameIndexCombo()
{
	// TODO: Add your control notification handler code here		
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }	
	m_MapIndex = JetAPI::GetComboxCurSelData(m_FrameIndexComobx);
	ProjectPtr->SetProjectMapIndex(m_MapIndex);	
	BuildShowImage();
	DrawShowImage();
	RedrewWnd();
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::OnSelchangeMaskIndexCombo()
{
	const int maskIndex = JetAPI::GetComboxCurSelData(m_MaskIndexComobx);
	switch (maskIndex)
	{
	case 1:
		m_MapMaskImagePtr = m_MapMaskImagePtr1;
		break;
	case 2:
		m_MapMaskImagePtr = m_MapMaskImagePtr2;
		break;
	case 3:
		m_MapMaskImagePtr = m_MapMaskImagePtr3;
		break;
	case 4:
		m_MapMaskImagePtr = m_MapMaskImagePtr4;
		break;
	default:
		m_MapMaskImagePtr = m_MapMaskImagePtr0;
		break;
	}
	BuildShowImage();
	DrawShowImage();
	RedrewWnd();
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::OnMaskAddBtn() 
{
	// TODO: Add your control notification handler code here		
	IMAGE_DATA mask = 0x00;
	switch ( m_CtrlMode )
	{
	case PMM_CTRL_SHPAE_CIRCLE_RADIO:
		if ( true == m_ShowRgnOuter ) 
		{	ModifyMaskImageCircle(mask); }
		break;
	case PMM_CTRL_COLOR_FILTER_RADIO:
		ModifyMaskImageByColorFilter(mask);
		break;
	default:
		if ( true == m_ShowRgnOuter ) 
		{	ModifyMaskImageRect(mask); }
		break;
	}		
	
	//m_ShowRgnOuter = false;	
	//ResetMaskRect();
	BuildShowImage();
	DrawShowImage();
	RedrewWnd();
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::OnMaskEraseBtn() 
{
	// TODO: Add your control notification handler code here			
	IMAGE_DATA mask = 0xFF;
	switch ( m_CtrlMode )
	{
	case PMM_CTRL_SHPAE_CIRCLE_RADIO:
		if ( true == m_ShowRgnOuter ) 
		{	ModifyMaskImageCircle(mask); }
		break;
	case PMM_CTRL_COLOR_FILTER_RADIO:
		ModifyMaskImageByColorFilter(mask);		
		break;
	default:
		if ( true == m_ShowRgnOuter ) 
		{	ModifyMaskImageRect(mask); }
		break;
	}			
	//m_ShowRgnOuter = false;	
	//ResetMaskRect();
	BuildShowImage();
	DrawShowImage();
	RedrewWnd();
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::ResetMaskRect()
{
	m_ShowRgnOuter = false;	
	m_ImageRgnRoi   = TREGION4D();
	m_ImageRgnOuter = TREGION4D();
	m_ImageRgnInner = TREGION4D();
}
//-------------------------------------------------------------------------------------//
bool CProjectMapMaskWnd::GetImageRect(RECT &Rect)
{
	JetAPI::Region4DToRect(m_ImageRgnOuter, Rect, true);	
	JetAPI::BoundaryRect(m_ShowImageW, m_ShowImageH, Rect);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectMapMaskWnd::ModifyMaskImageRect(IMAGE_DATA mask)
{
	if ( NULL == m_MapMaskImagePtr ) { return false; }

	RECT Rect={0};
	size_t  i=0, j=0, idx=0;
	const bool bUseRoiRect=GetUseRoiRectChk();
	if ( true == bUseRoiRect )
	{	JetAPI::Region4DToRect(m_ImageRgnRoi, Rect, true);	}
	else
	{	JetAPI::Region4DToRect(m_ImageRgnOuter, Rect, true); }
	JetAPI::BoundaryRect(m_MapMaskImageW, m_MapMaskImageH, Rect);
	for ( i=Rect.top; i<Rect.bottom; i++ )
	{
		idx = i*m_MapMaskImageStep+Rect.left;
		for ( j=Rect.left; j<Rect.right; j++ )
		{	m_MapMaskImagePtr[idx++]=mask;	}
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectMapMaskWnd::ModifyMaskImageCircle(IMAGE_DATA mask)
{
	if ( NULL == m_MapMaskImagePtr ) { return false; }

	RECT Rect={0};
	size_t  i=0, j=0, idx=0;
	const bool bUseRoiRect=GetUseRoiRectChk();
	if ( true == bUseRoiRect )
	{	JetAPI::Region4DToRect(m_ImageRgnRoi, Rect, true);	}
	else
	{	JetAPI::Region4DToRect(m_ImageRgnOuter, Rect, true); }
	JetAPI::BoundaryRect(m_MapMaskImageW, m_MapMaskImageH, Rect);
	
	double dx=0, dy=0;
	double f1X=0, f1Y=0;//焦點1
	double f2X=0, f2Y=0;//焦點2
	double lp1=0, lp2=0, lp12=0;
	const double RectW = Rect.right-Rect.left;
	const double RectH = Rect.bottom-Rect.top;
	const double CpX = (Rect.left+Rect.right)*0.5;
	const double CpY = (Rect.top+Rect.bottom)*0.5;	
	
	const double la = MAX(RectW, RectH)*0.5;
	const double lb = MIN(RectW, RectH)*0.5;
	const double lc = sqrt((la*la)-(lb*lb));
	const double la2 = 2*la;	
	if ( RectW < RectH )
	{
		f1X = f2X = CpX;
		f1Y = CpY-lc;
		f2Y = CpY+lc;
	}
	else
	{
		f1Y = f2Y = CpY;
		f1X = CpX-lc;
		f2X = CpX+lc;
	}

	for ( i=Rect.top; i<Rect.bottom; i++ )
	{	
		for ( j=Rect.left; j<Rect.right; j++ )
		{	
			dx = j-f1X;
			dy = i-f1Y;
			lp1 = sqrt((dx*dx)+(dy*dy));

			dx = j-f2X;
			dy = i-f2Y;
			lp2 = sqrt((dx*dx)+(dy*dy));

			lp12 = lp1+lp2;
			//點至焦點1與焦點2的距離和大於2倍長軸及為橢圓外
			if ( lp12 > la2 ) { continue; }
			idx = i*m_MapMaskImageStep+j;
			m_MapMaskImagePtr[idx]=mask;	
		}
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectMapMaskWnd::ModifyMaskImageByColorFilter(IMAGE_DATA mask)
{	
	const bool bUseColorFilter = CheckUseColorFilterMode();
	if ( false == bUseColorFilter ) { return true; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }
	if ( NULL == m_MapMaskImagePtr ) { return false; }

	IMAGE_PTR  ImagePtr = NULL;
	IMAGE_SIZE ImageW = 0;
	IMAGE_SIZE ImageH = 0;
	IMAGE_SIZE ImageStep = 0;
	IMAGE_SIZE BitCount = 0;
	const int MapIndex = m_MapIndex;
	if ( ProjectPtr->GetProjectMapPtr(MapIndex, ImageW, ImageH, ImageStep, BitCount, ImagePtr) == false )
	{	return false; }
	const size_t ImageSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	if ( NULL==ImagePtr || NULL==m_ShowImagePtr ) { return false; }	
	
	CString    str;	
	TREGION4D  WndRgn;
	TREGION4D  ImageRgn;
	RECT       RoiRect={0};	
	MASK_PTR   MaskPtr=NULL;
	IMAGE_SIZE MaskBitCount=8;
	const bool bOpenMP = true;
	const bool bUseRoiRect=GetUseRoiRectChk();
	IMAGE_SIZE MaskStep=JetAPI::GetBMPImagePixelsPerLine(m_ShowImageW, MaskBitCount, 4);

	if ( true == bUseRoiRect )
	{
		JetAPI::Region4DToRect(m_ImageRgnRoi, RoiRect, true);
		JetAPI::BoundaryRect(ImageW, ImageH, RoiRect);
	}
	else
	{	JetAPI::SizeToRect(ImageW, ImageH, RoiRect);	}
	if ( 24 == BitCount )
	{
		if ( ImageAPI.ColorImageColorFilter(ImageW, ImageH, ImageStep, ImagePtr, m_ColorGroup, RoiRect, MaskStep, MaskPtr, true, bOpenMP) == false )
		{	return false; }
	}
	else
	{
		if ( ImageAPI.RGBImageColorFilter(ImageW, ImageH, ImageStep, ImagePtr, ImagePtr, ImagePtr, m_ColorGroup, RoiRect, MaskStep, MaskPtr, true) == false )
		{	return false; }
	}
#ifdef _DEBUG
	//str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("ColorMaskFull.PNG"));
	//ImageAPI.SaveImage(str, ImageW, ImageH, MaskStep, MaskBitCount, MaskPtr, true);
#endif//_DEBUG

	if ( ImageH!=m_MapMaskImageH || MaskStep!=m_MapMaskImageStep )
	{
		JetMemory.free_func(MaskPtr);
		return false;
	}

	size_t i=0, j=0, k=0;	
	if ( true == bUseRoiRect )
	{
		for ( i=RoiRect.top; i<RoiRect.bottom; i++ )
		{
			for ( j=RoiRect.left; j<RoiRect.right; j++ )
			{
				k = (i*MaskStep)+j;
				if ( 0 == MaskPtr[k] ) { continue; }
				m_MapMaskImagePtr[k] = mask;
			}
		}
	}
	else
	{
		const size_t MaskSize = ImageAPI.CalcBufferSize(MaskStep, ImageH);
		for ( i=0; i<MaskSize; i++ )
		{
			if ( 0 == MaskPtr[i] ) { continue; }
			m_MapMaskImagePtr[i] = mask;
		}
	}
	JetMemory.free_func(MaskPtr);
	return true;
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::UpdateEditValue()
{
	if ( CheckUseColorFilterMode() == false ) { return; }
	CColorRGBV *rgbvPtr = m_RGBVWnd.GetColorRGBVPtr();
	UpdateRGBVToParam(rgbvPtr);	
	UpdateRGBVToUI(rgbvPtr);
	UpdateColorListWnd();
	BuildColorFilterImage(rgbvPtr);
	return;
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::SwitchRGBMasterMode()
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
void CProjectMapMaskWnd::UpdateRGBVToUI(CColorRGBV *rgbvPtr)
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
	CWnd::CheckDlgButton(PMM_RED_ENABLED_CHK, bCheck);

	if ( rgbvPtr->GetGreenEnabled() == true ) { bCheck = TRUE; }
	else { bCheck = FALSE; }
	CWnd::CheckDlgButton(PMM_GREEN_ENABLED_CHK, bCheck);

	if ( rgbvPtr->GetBlueEnabled() == true ) { bCheck = TRUE; }
	else { bCheck = FALSE; }
	CWnd::CheckDlgButton(PMM_BLUE_ENABLED_CHK, bCheck);

	if ( rgbvPtr->GetValueEnabled() == true ) { bCheck = TRUE; }
	else { bCheck = FALSE; }
	CWnd::CheckDlgButton(PMM_VALUE_ENABLED_CHK, bCheck);

	nMax = rgbvPtr->GetColorMax(m_RGBVMode);
	nMin = rgbvPtr->GetColorMin(m_RGBVMode);

	m_ColorMaxSpin.SetPos(nMax);
	m_ColorMinSpin.SetPos(nMin);
	str.Format(_T("%d"), nMax);
	CWnd::SetDlgItemText(PMM_COLOR_MAX_EDIT, str);
	str.Format(_T("%d"), nMin);
	CWnd::SetDlgItemText(PMM_COLOR_MIN_EDIT, str);

	nMax = rgbvPtr->GetValueMax();
	nMin = rgbvPtr->GetValueMin();	
	m_ValueMaxSpin.SetPos(nMax);
	m_ValueMinSpin.SetPos(nMin);
	str.Format(_T("%d"), nMax);
	CWnd::SetDlgItemText(PMM_VALUE_MAX_EDIT, str);
	str.Format(_T("%d"), nMin);
	CWnd::SetDlgItemText(PMM_VALUE_MIN_EDIT, str);
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::UpdateRGBVToParam(CColorRGBV *rgbvPtr)
{
	if ( NULL == rgbvPtr ) { return; }
	CString str;
	bool    bEnabled = false;
	int     nValue1 = 0;
	int     nValue2 = 0;
	int     nMax=255, nMin=0;
	BOOL    bCheck = FALSE; 
	
	if ( CWnd::IsDlgButtonChecked(PMM_RED_ENABLED_CHK) == TRUE ) { bEnabled = true; }
	else { bEnabled = false; }
	rgbvPtr->SetRedEnabled(bEnabled);

	if ( CWnd::IsDlgButtonChecked(PMM_GREEN_ENABLED_CHK) == TRUE ) { bEnabled = true; }
	else { bEnabled = false; }
	rgbvPtr->SetGreenEnabled(bEnabled);

	if ( CWnd::IsDlgButtonChecked(PMM_BLUE_ENABLED_CHK) == TRUE ) { bEnabled = true; }
	else { bEnabled = false; }
	rgbvPtr->SetBlueEnabled(bEnabled);

	if ( CWnd::IsDlgButtonChecked(PMM_VALUE_ENABLED_CHK) == TRUE ) { bEnabled = true; }
	else { bEnabled = false; }
	rgbvPtr->SetValueEnabled(bEnabled);
	
	CWnd::GetDlgItemText(PMM_COLOR_MAX_EDIT, str);
	nValue1 = ::_ttoi(str);
	CWnd::GetDlgItemText(PMM_COLOR_MIN_EDIT, str);
	nValue2 = ::_ttoi(str);
	nMax = MAX(nValue1, nValue2);
	nMin = MIN(nValue1, nValue2);

	m_ColorMaxSpin.SetPos(nMax);
	m_ColorMinSpin.SetPos(nMin);
	rgbvPtr->SetColorMax(m_RGBVMode, nMax);
	rgbvPtr->SetColorMin(m_RGBVMode, nMin);		

	CWnd::GetDlgItemText(PMM_VALUE_MAX_EDIT, str);
	nValue1 = ::_ttoi(str);
	CWnd::GetDlgItemText(PMM_VALUE_MIN_EDIT, str);
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
void CProjectMapMaskWnd::CalcColorFilterParam()//計算抽色參數
{
	const bool bUseColorFilter = CheckUseColorFilterMode();
	if ( false == bUseColorFilter ) { return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }	
	IMAGE_PTR  ImagePtr = NULL;
	IMAGE_SIZE ImageW = 0;
	IMAGE_SIZE ImageH = 0;
	IMAGE_SIZE ImageStep = 0;
	IMAGE_SIZE BitCount = 0;
	const int MapIndex = m_MapIndex;
	if ( ProjectPtr->GetProjectMapPtr(MapIndex, ImageW, ImageH, ImageStep, BitCount, ImagePtr) == false )
	{	return ; }
	if ( NULL == ImagePtr ) { return; }

	CColorRGBV *rgbvPtr = m_RGBVWnd.GetColorRGBVPtr();
	if ( NULL == rgbvPtr ) { return; }	
	const bool CombineColorMode = AOIDataCollect.CheckCombineColorMode();

	CColorRGBV rgbv;	
	RECT       RoiRect={0};	
	GetImageRect(RoiRect);
	if ( 24 == BitCount )
	{
		if ( ImageAPI.CalcColorImageColorFilter(ImageW, ImageH, ImageStep, ImagePtr, RoiRect, rgbv) == false ) 
		{	return; }
	}
	else
	{
		if ( ImageAPI.CalcRGBImageColorFilter(ImageW, ImageH, ImageStep, ImagePtr, ImagePtr, ImagePtr, RoiRect, rgbv) == false ) 
		{	return; }
	}
	
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
void CProjectMapMaskWnd::BuildColorFilterImage(CColorRGBV *rgbvPtr)
{
	if ( CheckUseColorFilterMode() == false )
	{	return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }
	CColorGroup TempColorGroup;
	if ( NULL == rgbvPtr )
	{	
		m_ColorGroup.UpdateColorGroupUsed();	
		m_ColorGroup.UpdateColorGroupShowColor();
		TempColorGroup = m_ColorGroup;
	}
	else
	{	
		rgbvPtr->CheckUsed();	
		rgbvPtr->CalcShowColor();
		
		if ( false == m_ShowColorGroup )
		{
			CColorRGBV  rgbv = *rgbvPtr;
			rgbv.SetLogicMode(COLOR_LOGIC_INCLUDE);
			TempColorGroup.AddColorGroupColor(rgbv);
		}
		else
		{	TempColorGroup = m_ColorGroup; }
	}	 
	IMAGE_PTR  ShowPtr = NULL;
	IMAGE_PTR  ImagePtr = NULL;
	IMAGE_SIZE ImageW = 0;
	IMAGE_SIZE ImageH = 0;
	IMAGE_SIZE ImageStep = 0;
	IMAGE_SIZE BitCount = 0;
	const int MapIndex = m_MapIndex;
	if ( ProjectPtr->GetProjectMapPtr(MapIndex, ImageW, ImageH, ImageStep, BitCount, ImagePtr) == false )
	{	return ; }
	if ( ProjectPtr->CreateProjectMapShowPtr(MapIndex, ShowPtr, false) == false )
	{	return ; }
	const size_t ImageSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	if ( NULL==ImagePtr || NULL==m_ShowImagePtr || NULL==ShowPtr ) { return; }	
	unsigned int FrameUniqueID = ProjectPtr->GetProjectFrameUniqueID(MapIndex, true);

	CString    str;	
	TREGION4D  WndRgn;
	TREGION4D  ImageRgn;
	RECT       RoiRect={0};	
	MASK_PTR   MaskPtr=NULL;
	IMAGE_SIZE MaskBitCount=8;
	const bool bOpenMP = true;
	IMAGE_SIZE MaskStep=JetAPI::GetBMPImagePixelsPerLine(m_ShowImageW, MaskBitCount, 4);	
	
	double          fnTime=0;
	LARGE_INTEGER   fnEnd;
	LARGE_INTEGER   fnStart;

	JetAPI::SetFuncTimeStart(fnStart);
	WndRgn.minX = 0;
	WndRgn.minY = 0;
	WndRgn.maxX = m_ImageWndRect.right;
	WndRgn.maxY = m_ImageWndRect.bottom;
	ImageAPI.MapWndRgnToImageRgn_DBL(m_ShowImageW, m_ShowImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, WndRgn, ImageRgn);
	JetAPI::Region4DToRect(ImageRgn, RoiRect, true);	
	JetAPI::BoundaryRect(ImageW, ImageH, RoiRect);
	if ( 24 == BitCount )
	{
		if ( ImageAPI.ColorImageColorFilter(ImageW, ImageH, ImageStep, ImagePtr, TempColorGroup, RoiRect, MaskStep, MaskPtr, true, bOpenMP) == false )
		{	return ; }
	}
	else
	{
		if ( ImageAPI.RGBImageColorFilter(ImageW, ImageH, ImageStep, ImagePtr, ImagePtr, ImagePtr, TempColorGroup, RoiRect, MaskStep, MaskPtr, true) == false )
		{	return ; }
	}
#ifdef _DEBUG
	//str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("ColorMaskWnd.PNG"));
	//ImageAPI.SaveImage(str, ImageW, ImageH, MaskStep, MaskBitCount, MaskPtr, true);
#endif//_DEBUG	
	JetAPI::SetFuncTimeStart(fnEnd);
	fnTime = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);//計算函式經過時間
	str.Format(_T("ImageAPI::ColorImageColorFilter Time=%.3f ms"), fnTime);
	AOIDataCollect.SaveUIDrawFuncLog(str);

	MASK_DATA  mask = 0xFF;
	IMAGE_DATA mskR=0xFF, mskG=0xFF, mskB=0x00, mskV=0x00, Alpha=0;	
	AOIDataCollect.GetMaskImageColor(FrameUniqueID, mskR, mskG, mskB, mskV, Alpha);

	JetAPI::SetFuncTimeStart(fnStart);	
	if ( ImageStep == m_ShowImageStep )
	{	::memcpy(m_ShowImagePtr, ShowPtr, sizeof(IMAGE_DATA)*ImageSize);	}
	else
	{	ImageAPI.RGBImageToColorImage3(ImageW, ImageH, ImageStep, ShowPtr, ShowPtr, ShowPtr, m_ShowImageStep, m_ShowImagePtr, false);	}
	ImageAPI.ColorImageApplyMask(m_ShowImageW, m_ShowImageH, m_ShowImageStep, m_ShowImagePtr, RoiRect, MaskStep, MaskPtr, mask, mskR, mskG, mskB, Alpha);
	
	JetAPI::SetFuncTimeStart(fnEnd);
	fnTime = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);//計算函式經過時間
	str.Format(_T("ImageAPI::ColorImageApplyMask Time=%.3f ms"), fnTime);
	AOIDataCollect.SaveUIDrawFuncLog(str);

	JetMemory.free_func(MaskPtr);

	//BuildShowImage();
	DrawShowImage();	
	RedrewWnd();
}
//-------------------------------------------------------------------------------------//
bool CProjectMapMaskWnd::LockUIWnd(bool bLock)
{
	AOIDataCollect.SetIsLockUIWnd(bLock);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectMapMaskWnd::GetLockUIWnd() const//取得是否鎖住UIWnd
{
	return AOIDataCollect.GetIsLockUIWnd();
}
//-------------------------------------------------------------------------------------//
bool CProjectMapMaskWnd::GetUseRoiRectChk()//取得是否使用局部區域
{
	BOOL bChk = CWnd::IsDlgButtonChecked(PMM_USE_ROI_RECT_CHK);
	if ( FALSE == bChk ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::OnMaskClearBtn() 
{
	// TODO: Add your control notification handler code here
	if ( NULL == m_MapMaskImagePtr ) { return ; }
	CString str;
	str = _T("Do you want to clear all Mask ?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) != IDYES )
	{	return; }

	const size_t BufferSize = m_MapMaskImageStep*m_MapMaskImageH;
	::memset(m_MapMaskImagePtr, 0xFF, sizeof(IMAGE_DATA)*BufferSize);		
	BuildShowImage();
	DrawShowImage();
	RedrewWnd();
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::OnShowMaskRadio() 
{
	// TODO: Add your control notification handler code here
	m_ShowMode = PMM_SHOW_MASK_RADIO;
	BuildShowImage();
	DrawShowImage();
	RedrewWnd();
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::OnShowImageRadio() 
{
	// TODO: Add your control notification handler code here
	m_ShowMode = PMM_SHOW_IMAGE_RADIO;
	BuildShowImage();
	DrawShowImage();
	RedrewWnd();
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::OnShowCombinedRadio() 
{
	// TODO: Add your control notification handler code here
	m_ShowMode = PMM_SHOW_COMBINED_RADIO;
	BuildShowImage();
	DrawShowImage();
	RedrewWnd();
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::OnShowComponentChk() 
{
	// TODO: Add your control notification handler code here
	RedrewWnd();
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::OnItemchangedColorListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
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
	m_ShowColorGroup = false;
	CColorRGBV *rgbvPtr = (CColorRGBV*)(m_ColorListCtrl.GetItemData(nItem));		
	UpdateRGBVToUI(rgbvPtr);	
	BuildColorFilterImage(rgbvPtr);
	m_RGBVWnd.SetColorRGBVPtr(rgbvPtr);
	m_RGBVWnd.ReDrawWnd();	
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::OnRedMasterChk() 
{
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }

	m_RGBVMode = COLOR_RGBV_RED;	
	CWnd::CheckDlgButton(PMM_GREEN_MASTER_CHK, FALSE);
	CWnd::CheckDlgButton(PMM_BLUE_MASTER_CHK, FALSE);
	CColorRGBV *rgbvPtr=m_RGBVWnd.GetColorRGBVPtr();
	if ( NULL != rgbvPtr )
	{
		const int nMax = rgbvPtr->GetRedMax();
		const int nMin = rgbvPtr->GetRedMin();
		m_ColorMaxSpin.SetPos(nMax);
		m_ColorMinSpin.SetPos(nMin);
		CWnd::SetDlgItemInt(PMM_COLOR_MAX_EDIT, nMax);
		CWnd::SetDlgItemInt(PMM_COLOR_MIN_EDIT, nMin);		
	}	
	m_RGBVWnd.SetRGBVMode(m_RGBVMode);
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::OnGreenMasterChk() 
{
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }

	m_RGBVMode = COLOR_RGBV_GREEN;	
	CWnd::CheckDlgButton(PMM_RED_MASTER_CHK, FALSE);
	CWnd::CheckDlgButton(PMM_BLUE_MASTER_CHK, FALSE);	
	CColorRGBV *rgbvPtr=m_RGBVWnd.GetColorRGBVPtr();
	if ( NULL != rgbvPtr )
	{
		const int nMax = rgbvPtr->GetGreenMax();
		const int nMin = rgbvPtr->GetGreenMin();
		m_ColorMaxSpin.SetPos(nMax);
		m_ColorMinSpin.SetPos(nMin);
		CWnd::SetDlgItemInt(PMM_COLOR_MAX_EDIT, nMax);
		CWnd::SetDlgItemInt(PMM_COLOR_MIN_EDIT, nMin);		
	}	
	m_RGBVWnd.SetRGBVMode(m_RGBVMode);
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::OnBlueMasterChk() 
{
	// TODO: Add your control notification handler code here
	m_RGBVMode = COLOR_RGBV_BLUE;		
	CWnd::CheckDlgButton(PMM_RED_MASTER_CHK, FALSE);
	CWnd::CheckDlgButton(PMM_GREEN_MASTER_CHK, FALSE);	
	CColorRGBV *rgbvPtr=m_RGBVWnd.GetColorRGBVPtr();
	if ( NULL != rgbvPtr )
	{
		const int nMax = rgbvPtr->GetBlueMax();
		const int nMin = rgbvPtr->GetBlueMin();
		m_ColorMaxSpin.SetPos(nMax);
		m_ColorMinSpin.SetPos(nMin);
		CWnd::SetDlgItemInt(PMM_COLOR_MAX_EDIT, nMax);
		CWnd::SetDlgItemInt(PMM_COLOR_MIN_EDIT, nMin);		
	}	
	m_RGBVWnd.SetRGBVMode(m_RGBVMode);
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::OnRedEnabledChk() 
{
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }

	bool bEnabled = false;
	BOOL bCheck = CWnd::IsDlgButtonChecked(PMM_RED_ENABLED_CHK);
	if ( TRUE == bCheck ) { bEnabled = true; }
	else { bEnabled = false; }
	CColorRGBV *rgbvPtr=m_RGBVWnd.GetColorRGBVPtr();
	if ( NULL != rgbvPtr )
	{	rgbvPtr->SetRedEnabled(bEnabled); }
	UpdateColorListWnd();
	BuildColorFilterImage(rgbvPtr);
	m_RGBVWnd.ReDrawWnd();
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::OnGreenEnabledChk() 
{
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }

	bool bEnabled = false;
	BOOL bCheck = CWnd::IsDlgButtonChecked(PMM_GREEN_ENABLED_CHK);
	if ( TRUE == bCheck ) { bEnabled = true; }
	else { bEnabled = false; }
	CColorRGBV *rgbvPtr=m_RGBVWnd.GetColorRGBVPtr();
	if ( NULL != rgbvPtr )
	{	rgbvPtr->SetGreenEnabled(bEnabled); }
	UpdateColorListWnd();
	BuildColorFilterImage(rgbvPtr);
	m_RGBVWnd.ReDrawWnd();
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::OnBlueEnabledChk() 
{
	// TODO: Add your control notification handler code here
	bool bEnabled = false;
	BOOL bCheck = CWnd::IsDlgButtonChecked(PMM_BLUE_ENABLED_CHK);
	if ( TRUE == bCheck ) { bEnabled = true; }
	else { bEnabled = false; }
	CColorRGBV *rgbvPtr=m_RGBVWnd.GetColorRGBVPtr();
	if ( NULL != rgbvPtr )
	{	rgbvPtr->SetBlueEnabled(bEnabled); }
	UpdateColorListWnd();
	BuildColorFilterImage(rgbvPtr);
	m_RGBVWnd.ReDrawWnd();
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::OnValueEnabledChk() 
{
	// TODO: Add your control notification handler code here
	bool bEnabled = false;
	BOOL bCheck = CWnd::IsDlgButtonChecked(PMM_VALUE_ENABLED_CHK);
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
void CProjectMapMaskWnd::OnDeltaposColorMaxSpin(NMHDR* pNMHDR, LRESULT* pResult) 
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
	CWnd::SetDlgItemText(PMM_COLOR_MAX_EDIT, str);
	rgbvPtr->SetColorMax(m_RGBVMode, nNextPos);	
	if ( rgbvPtr->GetColorEnabled(m_RGBVMode) == false )
	{
		switch ( m_RGBVMode )
		{
		case COLOR_RGBV_RED: CWnd::CheckDlgButton(PMM_RED_ENABLED_CHK, TRUE); 	break;
		case COLOR_RGBV_GREEN: CWnd::CheckDlgButton(PMM_GREEN_ENABLED_CHK, TRUE); 	break;
		case COLOR_RGBV_BLUE: CWnd::CheckDlgButton(PMM_BLUE_ENABLED_CHK, TRUE); 	break;
		}
		rgbvPtr->SetColorEnabled(m_RGBVMode, true);
		UpdateColorListWnd();
	}
	BuildColorFilterImage(rgbvPtr);
	m_RGBVWnd.ReDrawWnd();
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::OnDeltaposColorMinSpin(NMHDR* pNMHDR, LRESULT* pResult) 
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
	CWnd::SetDlgItemText(PMM_COLOR_MIN_EDIT, str);
	rgbvPtr->SetColorMin(m_RGBVMode, nNextPos);	
	if ( rgbvPtr->GetColorEnabled(m_RGBVMode) == false )
	{
		switch ( m_RGBVMode )
		{
		case COLOR_RGBV_RED: CWnd::CheckDlgButton(PMM_RED_ENABLED_CHK, TRUE); 	break;
		case COLOR_RGBV_GREEN: CWnd::CheckDlgButton(PMM_GREEN_ENABLED_CHK, TRUE); 	break;
		case COLOR_RGBV_BLUE: CWnd::CheckDlgButton(PMM_BLUE_ENABLED_CHK, TRUE); 	break;
		}
		rgbvPtr->SetColorEnabled(m_RGBVMode, true);
		UpdateColorListWnd();
	}
	BuildColorFilterImage(rgbvPtr);
	m_RGBVWnd.ReDrawWnd();
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::OnDeltaposValueMaxSpin(NMHDR* pNMHDR, LRESULT* pResult) 
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
	CWnd::SetDlgItemText(PMM_VALUE_MAX_EDIT, str);
	rgbvPtr->SetValueMax(nNextPos);
	if ( rgbvPtr->GetValueEnabled() == false )
	{
		CWnd::CheckDlgButton(PMM_VALUE_ENABLED_CHK, TRUE);
		rgbvPtr->SetValueEnabled(true);
		UpdateColorListWnd();
	}
	BuildColorFilterImage(rgbvPtr);
	m_RGBVWnd.ReDrawWnd();
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::OnDeltaposValueMinSpin(NMHDR* pNMHDR, LRESULT* pResult) 
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
	CWnd::SetDlgItemText(PMM_VALUE_MIN_EDIT, str);
	rgbvPtr->SetValueMin(nNextPos);	
	if ( rgbvPtr->GetValueEnabled() == false )
	{
		CWnd::CheckDlgButton(PMM_VALUE_ENABLED_CHK, TRUE);
		rgbvPtr->SetValueEnabled(true);
		UpdateColorListWnd();
	}
	BuildColorFilterImage(rgbvPtr);
	m_RGBVWnd.ReDrawWnd();
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::OnFilterExpandBtn() 
{
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }

	CColorRGBV *rgbvPtr=m_RGBVWnd.GetColorRGBVPtr();
	if ( NULL == rgbvPtr ) { return; }
	bool bOnlyColor = false;
	const int nValue = (int)(CWnd::GetDlgItemInt(PMM_FILTER_EXPAND_EDIT));
	if ( CWnd::IsDlgButtonChecked(PMM_FILTER_EXPAND_ONLY_COLOR_CHK) == TRUE )
	{	bOnlyColor = true; }
	else
	{	bOnlyColor = false; }
	rgbvPtr->ExpandColorRGBV(nValue, bOnlyColor);
	UpdateRGBVToUI(rgbvPtr);
	BuildColorFilterImage(rgbvPtr);
	m_RGBVWnd.ReDrawWnd();
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::OnFilterShirnkBtn() 
{
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }

	CColorRGBV *rgbvPtr=m_RGBVWnd.GetColorRGBVPtr();
	if ( NULL == rgbvPtr ) { return; }	
	bool bOnlyColor = false;
	const int nValue = (int)(CWnd::GetDlgItemInt(PMM_FILTER_EXPAND_EDIT));
	if ( CWnd::IsDlgButtonChecked(PMM_FILTER_EXPAND_ONLY_COLOR_CHK) == TRUE )
	{	bOnlyColor = true; }
	else
	{	bOnlyColor = false; }
	rgbvPtr->ExpandColorRGBV(-nValue, bOnlyColor);
	UpdateRGBVToUI(rgbvPtr);
	BuildColorFilterImage(rgbvPtr);
	m_RGBVWnd.ReDrawWnd();
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::OnColorMaxBtn() 
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
	CWnd::SetDlgItemText(PMM_COLOR_MAX_EDIT, str);
	rgbvPtr->SetColorMax(m_RGBVMode, nNextPos);	
	if ( rgbvPtr->GetColorEnabled(m_RGBVMode) == false )
	{
		switch ( m_RGBVMode )
		{
		case COLOR_RGBV_RED: CWnd::CheckDlgButton(PMM_RED_ENABLED_CHK, TRUE); 	break;
		case COLOR_RGBV_GREEN: CWnd::CheckDlgButton(PMM_GREEN_ENABLED_CHK, TRUE); 	break;
		case COLOR_RGBV_BLUE: CWnd::CheckDlgButton(PMM_BLUE_ENABLED_CHK, TRUE); 	break;
		}
		rgbvPtr->SetColorEnabled(m_RGBVMode, true);
		UpdateColorListWnd();
	}
	BuildColorFilterImage(rgbvPtr);
	m_RGBVWnd.ReDrawWnd();
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::OnColorMinBtn() 
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
	CWnd::SetDlgItemText(PMM_COLOR_MIN_EDIT, str);
	rgbvPtr->SetColorMin(m_RGBVMode, nNextPos);	
	if ( rgbvPtr->GetColorEnabled(m_RGBVMode) == false )
	{
		switch ( m_RGBVMode )
		{
		case COLOR_RGBV_RED: CWnd::CheckDlgButton(PMM_RED_ENABLED_CHK, TRUE); 	break;
		case COLOR_RGBV_GREEN: CWnd::CheckDlgButton(PMM_GREEN_ENABLED_CHK, TRUE); 	break;
		case COLOR_RGBV_BLUE: CWnd::CheckDlgButton(PMM_BLUE_ENABLED_CHK, TRUE); 	break;
		}
		rgbvPtr->SetColorEnabled(m_RGBVMode, true);
		UpdateColorListWnd();
	}
	BuildColorFilterImage(rgbvPtr);
	m_RGBVWnd.ReDrawWnd();	
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::OnValueMaxBtn() 
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
	CWnd::SetDlgItemText(PMM_VALUE_MAX_EDIT, str);
	rgbvPtr->SetValueMax(nNextPos);
	if ( rgbvPtr->GetValueEnabled() == false )
	{
		CWnd::CheckDlgButton(PMM_VALUE_ENABLED_CHK, TRUE);
		rgbvPtr->SetValueEnabled(true);
		UpdateColorListWnd();
	}
	BuildColorFilterImage(rgbvPtr);
	m_RGBVWnd.ReDrawWnd();
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::OnValueMinBtn() 
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
	CWnd::SetDlgItemText(PMM_VALUE_MIN_EDIT, str);
	rgbvPtr->SetValueMin(nNextPos);	
	if ( rgbvPtr->GetValueEnabled() == false )
	{
		CWnd::CheckDlgButton(PMM_VALUE_ENABLED_CHK, TRUE);
		rgbvPtr->SetValueEnabled(true);
		UpdateColorListWnd();
	}
	BuildColorFilterImage(rgbvPtr);
	m_RGBVWnd.ReDrawWnd();
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::OnResetColorBtn() 
{
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	if ( CheckUseColorFilterMode() == false ) { return ; }
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
void CProjectMapMaskWnd::OnResetAllBtn() 
{
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	if ( CheckUseColorFilterMode() == false ) { return ; }
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
void CProjectMapMaskWnd::OnMergeAllBtn() 
{
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	if ( CheckUseColorFilterMode() == false ) { return ; }
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
void CProjectMapMaskWnd::OnCtrlShpaeRectRadio() 
{
	// TODO: Add your control notification handler code here
	m_CtrlMode = PMM_CTRL_SHPAE_RECT_RADIO;
	BuildShowImage();
	DrawShowImage();
	RedrewWnd();
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::OnCtrlShpaeCircleRadio()
{
	m_CtrlMode = PMM_CTRL_SHPAE_CIRCLE_RADIO;
	BuildShowImage();
	DrawShowImage();
	RedrewWnd();
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::OnCtrlColorFilterRadio() 
{
	// TODO: Add your control notification handler code here
	m_ShowColorGroup = true;
	m_CtrlMode = PMM_CTRL_COLOR_FILTER_RADIO;
	BuildColorFilterImage(NULL);
}
//-------------------------------------------------------------------------------------//
BOOL CProjectMapMaskWnd::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	bool bResturn=false;
	if ( ExecMouseWheelMSG(pMsg->message, pMsg->wParam, pMsg->lParam) == true ) 
	{	return TRUE; }
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
		case VK_DELETE:
			OnMaskEraseBtn();
			break;
		}
		if ( true == bResturn )
		{	return TRUE; }
		break;
	}
	return CBaseDialog::PreTranslateMessage(pMsg);
}
//-------------------------------------------------------------------------------------//
LRESULT CProjectMapMaskWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
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
	}
	return CBaseDialog::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::OnGrayColorBtn() 
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
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::OnShowGroupColorBtn() 
{
	// TODO: Add your control notification handler code here
	m_ShowColorGroup = true;	
	BuildColorFilterImage(NULL);
	return;
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::OnGatherShowRawChk() 
{
	// TODO: Add your control notification handler code here
	
}
//-------------------------------------------------------------------------------------//
bool CProjectMapMaskWnd::ModifyMaskImageErode(int nKenSize)
{
	const char fnName[]="CProjectMapMaskWnd::ModifyMaskImageErode";
	if ( NULL == m_MapMaskImagePtr ) { return false; }
	IMAGE_PTR   MaskPtr=NULL;	
	const int   IterCnt = 1;
	const int   kenSize = nKenSize;
	const IMAGE_SIZE ImageW=m_MapMaskImageW;
	const IMAGE_SIZE ImageH=m_MapMaskImageH;
	const IMAGE_SIZE ImageStep=m_MapMaskImageStep;
	const size_t ImageSize=ImageAPI.CalcBufferSize(ImageStep, ImageH);
	const bool bUseRoiRect=GetUseRoiRectChk();
	if ( 0 == ImageSize ) { return false; }
	if ( JetMemory.alloc_func(ImageSize, MaskPtr, fnName, "MaskPtr") == false )
	{	return false; }

	if ( true == bUseRoiRect )
	{
		RECT      RoiRect={0};
		const int nAlign = 4;
		const bool bReverse=false;
		IMAGE_PTR RoiMaskPtr=NULL;	
		JetAPI::Region4DToRect(m_ImageRgnRoi, RoiRect, true);
		JetAPI::BoundaryRect(m_MapMaskImageW, m_MapMaskImageH, RoiRect);
		const IMAGE_SIZE RoiW=RoiRect.right-RoiRect.left;
		const IMAGE_SIZE RoiH=RoiRect.bottom-RoiRect.top;
		const IMAGE_SIZE RoiStep=JetAPI::GetBMPImagePixelsPerLine(RoiW, 4);
		if ( JetMemory.alloc_func(ImageSize, RoiMaskPtr, fnName, "RoiMaskPtr") == false )
		{	
			JetMemory.free_func(MaskPtr);
			JetMemory.free_func(RoiMaskPtr);
			return false; 
		}
		if ( ImageAPI.ExtractGrayRoiImage3(ImageW, ImageH, ImageStep, m_MapMaskImagePtr, RoiRect, RoiStep, RoiMaskPtr, bReverse) == false )
		{
			JetMemory.free_func(MaskPtr);
			JetMemory.free_func(RoiMaskPtr);
			return false; 
		}
		if ( ImageAPI.DilateGrayImage3(RoiW, RoiH, RoiStep, RoiMaskPtr, kenSize, IterCnt, MaskPtr) == false )
		{
			JetMemory.free_func(MaskPtr);
			JetMemory.free_func(RoiMaskPtr);
			return false; 
		}
		JetMemory.free_func(RoiMaskPtr);

		const bool bTheSameSize=false;
		ImageAPI.PasteGrayRoiImage3(ImageW, ImageH, ImageStep, m_MapMaskImagePtr, RoiRect, RoiStep, MaskPtr, bReverse, bTheSameSize);
	}
	else
	{
		::memcpy(MaskPtr, m_MapMaskImagePtr, sizeof(IMAGE_DATA)*ImageSize);
		//因為遮罩, 所以相反
		ImageAPI.DilateGrayImage3(ImageW, ImageH, ImageStep, MaskPtr, kenSize, IterCnt, m_MapMaskImagePtr);		
	}
	JetMemory.free_func(MaskPtr);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectMapMaskWnd::ModifyMaskImageDilate(int nKenSize)
{	
	const char fnName[]="CProjectMapMaskWnd::ModifyMaskImageDilate";
	if ( NULL == m_MapMaskImagePtr ) { return false; }
	IMAGE_PTR   MaskPtr=NULL;	
	const int   IterCnt = 1;
	const int   kenSize = nKenSize;
	const IMAGE_SIZE ImageW=m_MapMaskImageW;
	const IMAGE_SIZE ImageH=m_MapMaskImageH;
	const IMAGE_SIZE ImageStep=m_MapMaskImageStep;
	const size_t ImageSize=ImageAPI.CalcBufferSize(ImageStep, ImageH);
	const bool bUseRoiRect=GetUseRoiRectChk();
	if ( 0 == ImageSize ) { return false; }
	if ( JetMemory.alloc_func(ImageSize, MaskPtr, fnName, "MaskPtr") == false )
	{	return false; }

	if ( true == bUseRoiRect )
	{
		RECT      RoiRect={0};
		const int nAlign = 4;
		const bool bReverse=false;
		IMAGE_PTR RoiMaskPtr=NULL;	
		JetAPI::Region4DToRect(m_ImageRgnRoi, RoiRect, true);
		JetAPI::BoundaryRect(m_MapMaskImageW, m_MapMaskImageH, RoiRect);
		const IMAGE_SIZE RoiW=RoiRect.right-RoiRect.left;
		const IMAGE_SIZE RoiH=RoiRect.bottom-RoiRect.top;
		const IMAGE_SIZE RoiStep=JetAPI::GetBMPImagePixelsPerLine(RoiW, 4);
		if ( JetMemory.alloc_func(ImageSize, RoiMaskPtr, fnName, "RoiMaskPtr") == false )
		{	
			JetMemory.free_func(MaskPtr);
			JetMemory.free_func(RoiMaskPtr);
			return false; 
		}
		if ( ImageAPI.ExtractGrayRoiImage3(ImageW, ImageH, ImageStep, m_MapMaskImagePtr, RoiRect, RoiStep, RoiMaskPtr, bReverse) == false )
		{
			JetMemory.free_func(MaskPtr);
			JetMemory.free_func(RoiMaskPtr);
			return false; 
		}
		if ( ImageAPI.ErodeGrayImage3(RoiW, RoiH, RoiStep, RoiMaskPtr, kenSize, IterCnt, MaskPtr) == false )
		{
			JetMemory.free_func(MaskPtr);
			JetMemory.free_func(RoiMaskPtr);
			return false; 
		}
		JetMemory.free_func(RoiMaskPtr);

		const bool bTheSameSize=false;
		ImageAPI.PasteGrayRoiImage3(ImageW, ImageH, ImageStep, m_MapMaskImagePtr, RoiRect, RoiStep, MaskPtr, bReverse, bTheSameSize);
	}
	else
	{
		::memcpy(MaskPtr, m_MapMaskImagePtr, sizeof(IMAGE_DATA)*ImageSize);
		//因為遮罩, 所以相反
		ImageAPI.ErodeGrayImage3(ImageW, ImageH, ImageStep, MaskPtr, kenSize, IterCnt, m_MapMaskImagePtr);	
	}
	JetMemory.free_func(MaskPtr);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectMapMaskWnd::ModifyMaskImageOpen(int nKenSize)
{
	const char fnName[]="CProjectMapMaskWnd::ModifyMaskImageOpen";
	if ( NULL == m_MapMaskImagePtr ) { return false; }
	IMAGE_PTR   MaskPtr=NULL;		
	const int   IterCnt = 1;
	const int   kenSize = nKenSize;
	const int   MorphMode = MORPH_CLOSE;//因為遮罩, 所以相反
	const int   ShpaeMode = MORPH_SHAPE_RECT;
	const IMAGE_SIZE ImageW=m_MapMaskImageW;
	const IMAGE_SIZE ImageH=m_MapMaskImageH;
	const IMAGE_SIZE ImageStep=m_MapMaskImageStep;
	const size_t ImageSize=ImageAPI.CalcBufferSize(ImageStep, ImageH);
	const bool bUseRoiRect=GetUseRoiRectChk();
	if ( 0 == ImageSize ) { return false; }
	if ( JetMemory.alloc_func(ImageSize, MaskPtr, fnName, "MaskPtr") == false )
	{	return false; }
	if ( true == bUseRoiRect )
	{
		RECT      RoiRect={0};
		const int nAlign = 4;
		const bool bReverse=false;
		IMAGE_PTR RoiMaskPtr=NULL;	
		JetAPI::Region4DToRect(m_ImageRgnRoi, RoiRect, true);
		JetAPI::BoundaryRect(m_MapMaskImageW, m_MapMaskImageH, RoiRect);
		const IMAGE_SIZE RoiW=RoiRect.right-RoiRect.left;
		const IMAGE_SIZE RoiH=RoiRect.bottom-RoiRect.top;
		const IMAGE_SIZE RoiStep=JetAPI::GetBMPImagePixelsPerLine(RoiW, 4);
		if ( JetMemory.alloc_func(ImageSize, RoiMaskPtr, fnName, "RoiMaskPtr") == false )
		{	
			JetMemory.free_func(MaskPtr);
			JetMemory.free_func(RoiMaskPtr);
			return false; 
		}
		if ( ImageAPI.ExtractGrayRoiImage3(ImageW, ImageH, ImageStep, m_MapMaskImagePtr, RoiRect, RoiStep, RoiMaskPtr, bReverse) == false )
		{
			JetMemory.free_func(MaskPtr);
			JetMemory.free_func(RoiMaskPtr);
			return false; 
		}
		if ( ImageAPI.MorphGrayImage3(RoiW, RoiH, RoiStep, RoiMaskPtr, MorphMode, ShpaeMode, kenSize, IterCnt, MaskPtr) == false )
		{
			JetMemory.free_func(MaskPtr);
			JetMemory.free_func(RoiMaskPtr);
			return false; 
		}
		JetMemory.free_func(RoiMaskPtr);

		const bool bTheSameSize=false;
		ImageAPI.PasteGrayRoiImage3(ImageW, ImageH, ImageStep, m_MapMaskImagePtr, RoiRect, RoiStep, MaskPtr, bReverse, bTheSameSize);
	}
	else
	{
		::memcpy(MaskPtr, m_MapMaskImagePtr, sizeof(IMAGE_DATA)*ImageSize);
		ImageAPI.MorphGrayImage3(ImageW, ImageH, ImageStep, MaskPtr, MorphMode, ShpaeMode, kenSize, IterCnt, m_MapMaskImagePtr);
	}
	JetMemory.free_func(MaskPtr);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectMapMaskWnd::ModifyMaskImageClose(int nKenSize)
{
	const char fnName[]="CProjectMapMaskWnd::ModifyMaskImageClose";
	if ( NULL == m_MapMaskImagePtr ) { return false; }
	IMAGE_PTR   MaskPtr=NULL;		
	const int   IterCnt = 1;
	const int   kenSize = nKenSize;
	const int   MorphMode = MORPH_OPEN;//因為遮罩, 所以相反
	const int   ShpaeMode = MORPH_SHAPE_RECT;
	const IMAGE_SIZE ImageW=m_MapMaskImageW;
	const IMAGE_SIZE ImageH=m_MapMaskImageH;
	const IMAGE_SIZE ImageStep=m_MapMaskImageStep;
	const size_t ImageSize=ImageAPI.CalcBufferSize(ImageStep, ImageH);
	const bool bUseRoiRect=GetUseRoiRectChk();
	if ( 0 == ImageSize ) { return false; }
	if ( JetMemory.alloc_func(ImageSize, MaskPtr, fnName, "MaskPtr") == false )
	{	return false; }
	if ( true == bUseRoiRect )
	{
		RECT      RoiRect={0};
		const int nAlign = 4;
		const bool bReverse=false;
		IMAGE_PTR RoiMaskPtr=NULL;	
		JetAPI::Region4DToRect(m_ImageRgnRoi, RoiRect, true);
		JetAPI::BoundaryRect(m_MapMaskImageW, m_MapMaskImageH, RoiRect);
		const IMAGE_SIZE RoiW=RoiRect.right-RoiRect.left;
		const IMAGE_SIZE RoiH=RoiRect.bottom-RoiRect.top;
		const IMAGE_SIZE RoiStep=JetAPI::GetBMPImagePixelsPerLine(RoiW, 4);
		if ( JetMemory.alloc_func(ImageSize, RoiMaskPtr, fnName, "RoiMaskPtr") == false )
		{	
			JetMemory.free_func(MaskPtr);
			JetMemory.free_func(RoiMaskPtr);
			return false; 
		}
		if ( ImageAPI.ExtractGrayRoiImage3(ImageW, ImageH, ImageStep, m_MapMaskImagePtr, RoiRect, RoiStep, RoiMaskPtr, bReverse) == false )
		{
			JetMemory.free_func(MaskPtr);
			JetMemory.free_func(RoiMaskPtr);
			return false; 
		}
		if ( ImageAPI.MorphGrayImage3(RoiW, RoiH, RoiStep, RoiMaskPtr, MorphMode, ShpaeMode, kenSize, IterCnt, MaskPtr) == false )
		{
			JetMemory.free_func(MaskPtr);
			JetMemory.free_func(RoiMaskPtr);
			return false; 
		}
		JetMemory.free_func(RoiMaskPtr);

		const bool bTheSameSize=false;
		ImageAPI.PasteGrayRoiImage3(ImageW, ImageH, ImageStep, m_MapMaskImagePtr, RoiRect, RoiStep, MaskPtr, bReverse, bTheSameSize);
	}
	else
	{
		::memcpy(MaskPtr, m_MapMaskImagePtr, sizeof(IMAGE_DATA)*ImageSize);
		ImageAPI.MorphGrayImage3(ImageW, ImageH, ImageStep, MaskPtr, MorphMode, ShpaeMode, kenSize, IterCnt, m_MapMaskImagePtr);
	}
	JetMemory.free_func(MaskPtr);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectMapMaskWnd::ModifyMaskImageGradient(int nKenSize)
{
	const char fnName[] = "CProjectMapMaskWnd::ModifyMaskImageGradient";
	if (NULL == m_MapMaskImagePtr) { return false; }
	IMAGE_PTR   MaskPtr = NULL;
	IMAGE_PTR   MaskTempPtr = NULL;
	const int   IterCnt = 1;
	const int   kenSize = nKenSize;
	const int   MorphMode = MORPH_GRADIENT;
	const int   ShpaeMode = MORPH_SHAPE_ELLIPSE;
	const IMAGE_SIZE ImageW = m_MapMaskImageW;
	const IMAGE_SIZE ImageH = m_MapMaskImageH;
	const IMAGE_SIZE ImageStep = m_MapMaskImageStep;
	const size_t ImageSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	const bool bUseRoiRect = GetUseRoiRectChk();
	if (0 == ImageSize) { return false; }
	if (JetMemory.alloc_func(ImageSize, MaskPtr, fnName, "MaskPtr") == false||
		JetMemory.alloc_func(ImageSize, MaskTempPtr, fnName, "MaskPtr") == false)
	{
		JetMemory.free_func(MaskPtr);
		JetMemory.free_func(MaskTempPtr);
		return false;
	}
	if (true == bUseRoiRect)
	{
		RECT      RoiRect = { 0 };
		const int nAlign = 4;
		const bool bReverse = false;
		IMAGE_PTR RoiMaskPtr = NULL;
		JetAPI::Region4DToRect(m_ImageRgnRoi, RoiRect, true);
		JetAPI::BoundaryRect(m_MapMaskImageW, m_MapMaskImageH, RoiRect);
		const IMAGE_SIZE RoiW = RoiRect.right - RoiRect.left;
		const IMAGE_SIZE RoiH = RoiRect.bottom - RoiRect.top;
		const IMAGE_SIZE RoiStep = JetAPI::GetBMPImagePixelsPerLine(RoiW, 4);
		if (JetMemory.alloc_func(ImageSize, RoiMaskPtr, fnName, "RoiMaskPtr") == false)
		{
			JetMemory.free_func(MaskPtr);
			JetMemory.free_func(MaskTempPtr);
			JetMemory.free_func(RoiMaskPtr);
			return false;
		}
		if (ImageAPI.ExtractGrayRoiImage3(ImageW, ImageH, ImageStep, m_MapMaskImagePtr, RoiRect, RoiStep, RoiMaskPtr, bReverse) == false)
		{
			JetMemory.free_func(MaskPtr);
			JetMemory.free_func(MaskTempPtr);
			JetMemory.free_func(RoiMaskPtr);
			return false;
		}
		if (ImageAPI.MorphGrayImage3(RoiW, RoiH, RoiStep, RoiMaskPtr, MorphMode, ShpaeMode, kenSize, IterCnt, MaskPtr) == false)
		{
			JetMemory.free_func(MaskPtr);
			JetMemory.free_func(MaskTempPtr);
			JetMemory.free_func(RoiMaskPtr);
			return false;
		}
		if (ImageAPI.MorphGrayImage3(RoiW, RoiH, RoiStep, RoiMaskPtr, MorphMode, ShpaeMode, kenSize, IterCnt, MaskPtr) == false)
		{
			JetMemory.free_func(MaskPtr);
			JetMemory.free_func(MaskTempPtr);
			JetMemory.free_func(RoiMaskPtr);
			return false;
		}
		if (ImageAPI.MorphGrayImage3(RoiW, RoiH, RoiStep, RoiMaskPtr, MorphMode, ShpaeMode, kenSize, IterCnt, MaskTempPtr) == false)
		{
			JetMemory.free_func(MaskPtr);
			JetMemory.free_func(MaskTempPtr);
			JetMemory.free_func(RoiMaskPtr);
			return false;
		}
		if (ImageAPI.InvertGrayImage(RoiW, RoiH, RoiStep, MaskTempPtr, MaskPtr)) {
			JetMemory.free_func(MaskPtr);
			JetMemory.free_func(MaskTempPtr);
			JetMemory.free_func(RoiMaskPtr);
			return false;
		}
		JetMemory.free_func(RoiMaskPtr);
		const bool bTheSameSize = false;
		ImageAPI.PasteGrayRoiImage3(ImageW, ImageH, ImageStep, m_MapMaskImagePtr, RoiRect, RoiStep, MaskPtr, bReverse, bTheSameSize);
	}
	else
	{
		::memcpy(MaskPtr, m_MapMaskImagePtr, sizeof(IMAGE_DATA)*ImageSize);
		ImageAPI.MorphGrayImage3(ImageW, ImageH, ImageStep, MaskPtr, MorphMode, ShpaeMode, kenSize, IterCnt, MaskTempPtr);
		ImageAPI.InvertGrayImage(ImageW, ImageH, ImageStep, MaskTempPtr, m_MapMaskImagePtr);
	}
	JetMemory.free_func(MaskTempPtr);
	JetMemory.free_func(MaskPtr);
	return true;
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::OnMaskErodeBtn() 
{
	// TODO: Add your control notification handler code here	
	int          KenSize=3;
	CString      str;
	CString      strValue;
	CString      strLabel;
	CString      strCaption;		
	CInputBoxWnd InputBox;
	str = _T("Set Mask Erode Param");
	strLabel = AOIDataDefine.GetSizeText();
	strValue.Format(_T("%d"), KenSize);
	strCaption = LoadMultiLanguageString(str, str);
	InputBox.SetParam1(strCaption, strLabel, strValue);
	if ( InputBox.DoModal() == IDCANCEL ) 
	{	return ;  }
	strValue = InputBox.m_DataEdit1;
	KenSize = ::_ttoi(strValue);
	if ( KenSize < 3 ) { return; }
	ModifyMaskImageErode(KenSize);
	BuildShowImage();
	DrawShowImage();
	RedrewWnd();
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::OnMaskDilateBtn() 
{
	// TODO: Add your control notification handler code here
	int          KenSize=3;
	CString      str;
	CString      strValue;
	CString      strLabel;
	CString      strCaption;		
	CInputBoxWnd InputBox;
	str = _T("Set Mask Dilate Param");
	strLabel = AOIDataDefine.GetSizeText();
	strValue.Format(_T("%d"), KenSize);
	strCaption = LoadMultiLanguageString(str, str);
	InputBox.SetParam1(strCaption, strLabel, strValue);
	if ( InputBox.DoModal() == IDCANCEL ) 
	{	return ;  }
	strValue = InputBox.m_DataEdit1;
	KenSize = ::_ttoi(strValue);
	if ( KenSize < 3 ) { return; }
	ModifyMaskImageDilate(KenSize);
	BuildShowImage();
	DrawShowImage();
	RedrewWnd();
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::OnMaskOpenBtn() 
{
	// TODO: Add your control notification handler code here
	int          KenSize=3;
	CString      str;
	CString      strValue;
	CString      strLabel;
	CString      strCaption;		
	CInputBoxWnd InputBox;
	str = _T("Set Mask Open Param");
	strLabel = AOIDataDefine.GetSizeText();
	strValue.Format(_T("%d"), KenSize);
	strCaption = LoadMultiLanguageString(str, str);
	InputBox.SetParam1(strCaption, strLabel, strValue);
	if ( InputBox.DoModal() == IDCANCEL ) 
	{	return ;  }
	strValue = InputBox.m_DataEdit1;
	KenSize = ::_ttoi(strValue);
	if ( KenSize < 3 ) { return; }
	ModifyMaskImageOpen(KenSize);
	BuildShowImage();
	DrawShowImage();
	RedrewWnd();
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::OnMaskCloseBtn() 
{
	// TODO: Add your control notification handler code here
	int          KenSize=3;
	CString      str;
	CString      strValue;
	CString      strLabel;
	CString      strCaption;		
	CInputBoxWnd InputBox;
	str = _T("Set Mask Close Param");
	strLabel = AOIDataDefine.GetSizeText();
	strValue.Format(_T("%d"), KenSize);	
	strCaption = LoadMultiLanguageString(str, str);
	InputBox.SetParam1(strCaption, strLabel, strValue);
	if ( InputBox.DoModal() == IDCANCEL ) 
	{	return ;  }
	strValue = InputBox.m_DataEdit1;
	KenSize = ::_ttoi(strValue);
	if ( KenSize < 3 ) { return; }
	ModifyMaskImageClose(KenSize);
	BuildShowImage();
	DrawShowImage();
	RedrewWnd();
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::OnMaskGradBtn()
{
	// TODO: Add your control notification handler code here
	int          KenSize = 3;
	CString      str;
	CString      strValue;
	CString      strLabel;
	CString      strCaption;
	CInputBoxWnd InputBox;
	str = _T("Set Mask Gradient Param");
	strLabel = AOIDataDefine.GetSizeText();
	strValue.Format(_T("%d"), KenSize);
	strCaption = LoadMultiLanguageString(str, str);
	InputBox.SetParam1(strCaption, strLabel, strValue);
	if (InputBox.DoModal() == IDCANCEL)
	{
		return;
	}
	strValue = InputBox.m_DataEdit1;
	KenSize = ::_ttoi(strValue);
	if (KenSize < 3) { return; }
	ModifyMaskImageGradient(KenSize);
	BuildShowImage();
	DrawShowImage();
	RedrewWnd();
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::OnMaskMergeBtn()
{
	//m_MapMaskImageW = ImageW;
	//m_MapMaskImageH = ImageH;
	//m_MapMaskImageStep = ImageStep;
	//m_MapMaskBitCount = BitCount;
	int maskIdx;
	for (int i = 0; i<m_MapMaskImageH; i++){
		for (int j = 0; j<m_MapMaskImageW; j++){
			maskIdx = (i*m_MapMaskImageStep) + j;
			m_MapMaskImagePtr0[maskIdx] =
				m_MapMaskImagePtr0[maskIdx] &
				m_MapMaskImagePtr1[maskIdx] &
				m_MapMaskImagePtr2[maskIdx] &
				m_MapMaskImagePtr3[maskIdx] &
				m_MapMaskImagePtr4[maskIdx];
		}
	}
	BuildShowImage();
	DrawShowImage();
	RedrewWnd();
}
//-------------------------------------------------------------------------------------//
bool CProjectMapMaskWnd::CreatePartMask(IMAGE_SIZE &MaskW, IMAGE_SIZE &MaskH, IMAGE_SIZE &MaskStep, IMAGE_PTR &MaskPtr)
{
	const char fnName[] = "CProjectMapMaskWnd::CreatePartMask";
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }
	if ( NULL == m_MapMaskImagePtr ) { return false; }
		
	MASK_DATA  mskPart = 0x00;
	const int  MaskMode = 0x0F;
	TREGION4D  StageRgn=m_FrameStageRgn;
	TPOINT2D   ImageRes=m_FrameResolution;
	IMAGE_PTR  TempPtr = NULL;
	IMAGE_SIZE TempW = m_MapMaskImageW;
	IMAGE_SIZE TempH = m_MapMaskImageH;
	IMAGE_SIZE TempStep = m_MapMaskImageStep;
	const size_t TempBufSize=ImageAPI.CalcBufferSize(TempStep, TempH);
	if ( JetMemory.alloc_func(TempBufSize, TempPtr, fnName, "MaskPtr") == false )
	{	return false; }
	if ( ProjectPtr->CreateProjectPartMaskImage(TempW, TempH, TempStep, TempPtr, StageRgn, ImageRes, MaskMode, mskPart) == false )
	{	
		JetMemory.free_func(TempPtr);
		return false;	
	}
	MaskW = TempW;
	MaskH = TempH;
	MaskStep = TempStep;
	MaskPtr = TempPtr;	
	return true;
}
//-------------------------------------------------------------------------------------//
void CProjectMapMaskWnd::OnMaskErasePartBtn() 
{
	// TODO: Add your control notification handler code here
	if ( NULL == m_MapMaskImagePtr ) { return ; }
	CString str;
	str = _T("Do you want to Erase Part Mask ?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) != IDYES )
	{	return; }

	MASK_PTR   PartPtr=NULL;
	IMAGE_SIZE PartW=0;
	IMAGE_SIZE PartH=0;
	IMAGE_SIZE PartStep=0;	
	IMAGE_SIZE PartBitCnt=8;
	if ( CreatePartMask(PartW, PartH, PartStep, PartPtr) == false )
	{	return ; }
	
	const size_t PartSize = PartStep*PartH;
	const size_t BufferSize = m_MapMaskImageStep*m_MapMaskImageH;
	
	if ( PartSize != BufferSize )
	{
		JetMemory.free_func(PartPtr);
		return ;
	}
	
	CString Folder = AOIDataCollect.GetAOITempDirectory();
	str.Format(_T("%s\\%s"), Folder, _T("ProjectMask_PartMask.PNG"));
	ImageAPI.SaveImage(str, PartW, PartH, PartStep, PartBitCnt, PartPtr, true);


	size_t i=0, j=0, k=0;
	const bool bUseRoiRect=GetUseRoiRectChk();	
	if ( true == bUseRoiRect )
	{
		RECT    RoiRect={0};		
		JetAPI::Region4DToRect(m_ImageRgnRoi, RoiRect, true);		
		JetAPI::BoundaryRect(PartW, PartH, RoiRect);
		for ( i=RoiRect.top; i<RoiRect.bottom; i++ )
		{
			for ( j=RoiRect.left; j<RoiRect.right; j++ )
			{
				k = (i*PartStep)+j;
				if ( 0xFF == PartPtr[k] ) { continue; }
				m_MapMaskImagePtr[k] = 0xFF;
			}
		}
	}
	else
	{
		for ( i=0; i<BufferSize; i++ )
		{
			if ( 0xFF == PartPtr[i] ) { continue; }
			m_MapMaskImagePtr[i] = 0xFF;
		}
	}		
	JetMemory.free_func(PartPtr);
	BuildShowImage();
	DrawShowImage();
	RedrewWnd();
}
//-------------------------------------------------------------------------------------//
