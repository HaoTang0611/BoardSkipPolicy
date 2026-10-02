// ProjectLibraryWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "ProjectLibraryWnd.h"
//-------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CProjectLibraryWnd dialog
//-------------------------------------------------------------------------------------//
CProjectLibraryWnd::CProjectLibraryWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CProjectLibraryWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProjectLibraryWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ModelType = MODEL_TYPE_NULL;	
	m_ModelPtr = NULL;
	m_ProjectPtr = NULL;
	m_StopModelNameListBeSelected = false;
	m_StopModelTypeListBeSelected = false;
	m_StopModelGroupListBeSelected = false;
}
//-------------------------------------------------------------------------------------//
void CProjectLibraryWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProjectLibraryWnd)
	DDX_Control(pDX, PROLIB_MODEL_ICON_LIST_WND, m_ModelIconListWnd);
	DDX_Control(pDX, PROLIB_MODEL_GROUP_LIST_WND, m_ModelGroupListWnd);
	DDX_Control(pDX, PROLIB_MODEL_TYPE_LIST_WND, m_ModelTypeListWnd);
	DDX_Control(pDX, PROLIB_MODEL_FRAME_INDEX_COMBO, m_ModelFrameIndexCombox);	
	DDX_Control(pDX, PROLIB_MODEL_ICON_SIZE_COMBO, m_ModelIconSizeCombox);		
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CProjectLibraryWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CProjectLibraryWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(LVN_ITEMCHANGED, PROLIB_MODEL_TYPE_LIST_WND, OnItemchangedModelTypeListWnd)
	ON_NOTIFY(LVN_ITEMCHANGED, PROLIB_MODEL_GROUP_LIST_WND, OnItemchangedModelGroupListWnd)
	ON_NOTIFY(LVN_ITEMCHANGED, PROLIB_MODEL_ICON_LIST_WND, OnItemchangedModelIconListWnd)
	ON_NOTIFY(NM_DBLCLK, PROLIB_MODEL_ICON_LIST_WND, OnDblclkModelIconListWnd)
	ON_BN_CLICKED(PROLIB_SEARCH_BTN, OnSearchBtn)
	ON_CBN_SELCHANGE(PROLIB_MODEL_FRAME_INDEX_COMBO, OnSelchangeModelBKImageIndexCombox)
	ON_CBN_SELCHANGE(PROLIB_MODEL_ICON_SIZE_COMBO, OnSelchangeModelIconSizeCombox)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CProjectLibraryWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CProjectLibraryWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SwitchMultiLanguage();

	BuildModelIconSizeCombox();
	CWnd::CheckDlgButton(PROLIB_SEARCH_NAME_CHK, TRUE);
	CWnd::SetDlgItemText(PROLIB_SEARCH_NAME_EDIT, m_SearchName);
	CWnd::SetDlgItemInt(PROLIB_SEARCH_MODEL_SIZE_W_EDIT, 1000);
	CWnd::SetDlgItemInt(PROLIB_SEARCH_MODEL_SIZE_H_EDIT, 1000);
	CWnd::SetDlgItemInt(PROLIB_SEARCH_MODEL_SIZE_TOL_EDIT, 500);

	JetAPI::InitialListCtrl(m_ModelIconListWnd);
	InitModelGroupListCtrl(m_ModelGroupListWnd);	
	BuildModelTypeListCtrl(m_ModelTypeListWnd);
	AOIDataDefine.BuildProjectMapIndexCombox(m_ModelFrameIndexCombox);	
	JetAPI::SetComboxCurSel(m_ModelFrameIndexCombox, 0);

	AdjustCtrlWndPosition(960, 800);
	SetModelTypeListCtrlItemSlected(m_ModelType);
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	BuildModelGroupListCtrl(m_ModelGroupListWnd, m_ModelType, true);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CProjectLibraryWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	AOIDataCollect.DestroyModelPreViewPtr();
}
//-------------------------------------------------------------------------------------//
void CProjectLibraryWnd::OnSize(UINT nType, int cx, int cy) 
{
	CBaseDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	AdjustCtrlWndPosition(cx, cy);
}
//-------------------------------------------------------------------------------------//
void CProjectLibraryWnd::AdjustCtrlWndPosition(int cx, int cy)
{
	if ( GetSafeHwnd() == NULL ) { return; }	

	if ( m_ModelTypeListWnd.GetSafeHwnd() != NULL )
	{
		RECT WndRect={0,0,0,0};
		m_ModelTypeListWnd.GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		WndRect.right = cx;
		m_ModelTypeListWnd.MoveWindow(&WndRect);
		m_ModelTypeListWnd.Arrange(LVA_ALIGNTOP);//LVA_ALIGNTOP
	}

	if ( m_ModelGroupListWnd.GetSafeHwnd() != NULL )
	{
		RECT WndRect={0,0,0,0};
		m_ModelGroupListWnd.GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		WndRect.bottom = cy;
		m_ModelGroupListWnd.MoveWindow(&WndRect);		
	}

	if ( m_ModelIconListWnd.GetSafeHwnd() != NULL )
	{
		RECT WndRect={0,0,0,0};
		m_ModelIconListWnd.GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		WndRect.right = cx;
		WndRect.bottom = cy;
		m_ModelIconListWnd.MoveWindow(&WndRect);
		m_ModelIconListWnd.Arrange(LVA_ALIGNTOP);//LVA_ALIGNTOP
	}
}
//-------------------------------------------------------------------------------------//
void CProjectLibraryWnd::BuildModelIconSizeCombox()
{
	int          i=0;
	int          idx=0;
	CString      str;
	DWORD        Param=0;		
	CComboBox &Combox = m_ModelIconSizeCombox;
	idx = 0;
	JetAPI::ClearCombox(Combox);	
	
	Param = 64;
	str.Format(_T("%d"), Param);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;

	Param = 128;
	str.Format(_T("%d"), Param);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;	

	Param = 256;
	str.Format(_T("%d"), Param);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;

	Param = 512;
	str.Format(_T("%d"), Param);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;

	Combox.SetCurSel(1);
	return;
}
//-------------------------------------------------------------------------------------//
void CProjectLibraryWnd::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default	
	CBaseDialog::OnGetMinMaxInfo(lpMMI);
	lpMMI->ptMinTrackSize.x = 920;
	lpMMI->ptMinTrackSize.y = 800;
}
//-------------------------------------------------------------------------------------//
void CProjectLibraryWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CBaseDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CProjectLibraryWnd::OnItemchangedModelTypeListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if ( true == m_StopModelTypeListBeSelected ) { return; }
	const int nItem = pNMListView->iItem;	
	if ( nItem < 0 ) { return; }
	DWORD Res = pNMListView->uNewState&LVIS_SELECTED;
	if ( Res == 0 ) { return; }	
	Res = pNMListView->uNewState&LVIS_FOCUSED;
	if ( Res == 0 ) { return; }

	MODEL_TYPE ModelType = (MODEL_TYPE)(m_ModelTypeListWnd.GetItemData(nItem));	
	BuildModelGroupListCtrl(m_ModelGroupListWnd, ModelType, true);
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CProjectLibraryWnd::OnItemchangedModelGroupListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if ( true == m_StopModelGroupListBeSelected ) { return; }
	const int nItem = pNMListView->iItem;	
	if ( nItem < 0 ) { return; }
	DWORD Res = pNMListView->uNewState&LVIS_SELECTED;
	if ( Res == 0 ) { return; }	
	Res = pNMListView->uNewState&LVIS_FOCUSED;
	if ( Res == 0 ) { return; }

	const int nTypeItem = m_ModelTypeListWnd.GetNextItem(-1, LVNI_SELECTED);
	if ( nTypeItem < 0 ) { return; }
	MODEL_TYPE ModelType = (MODEL_TYPE)(m_ModelTypeListWnd.GetItemData(nTypeItem));
	CString GroupName = m_ModelGroupListWnd.GetItemText(nItem, 0);
	BuildModelIconListCtrl(ModelType, GroupName, m_ModelIconListWnd);
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CProjectLibraryWnd::OnItemchangedModelIconListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if ( true == m_StopModelNameListBeSelected ) { return; }
	const int nItem = pNMListView->iItem;	
	if ( nItem < 0 ) { return; }
	DWORD Res = pNMListView->uNewState&LVIS_SELECTED;
	if ( Res == 0 ) { return; }	
	Res = pNMListView->uNewState&LVIS_FOCUSED;
	if ( Res == 0 ) { return; }

	CString strModelName = m_ModelIconListWnd.GetItemText(nItem, 0);
	CWnd::SetDlgItemText(PROLIB_SELECTED_MODEL_NAME_EDIT, strModelName);

	CAOIModel *ModelPtr = (CAOIModel*)m_ModelIconListWnd.GetItemData(nItem);
	m_ModelPtr = ModelPtr;
	if ( NULL == ModelPtr )
	{	AOIDataCollect.DestroyModelPreViewPtr();	}
	else
	{	
		AOIDataCollect.CreateModelPreViewPtr(ModelPtr);	
		AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_REDRAW_VIEW_WND, NULL);	
	}
	
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CProjectLibraryWnd::OnDblclkModelIconListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnOK();
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CProjectLibraryWnd::SwitchMultiLanguage()
{
	size_t  i = 0;	
	UINT    WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_PROJECT_LIBRARY_WND");	
	//---------------------------------------------------------------------------------//
	WndID = IDD_PROJECT_LIBRARY_WND;
	WndKey = _T("IDD_PROJECT_LIBRARY_WND");
	this->GetWindowText(LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetWindowText(NewLabelText);
	CWnd::GetWindowText(m_WndText);	
	//---------------------------------------------------------------------------------//	
	WndID = IDOK;
	NewLabelText = AOIDataDefine.GetWndOKText();
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = IDCANCEL;
	NewLabelText = AOIDataDefine.GetWndCancelText();
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//		
	WndID = PROLIB_SEARCH_GROUP;
	WndKey = _T("PROLIB_SEARCH_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROLIB_SEARCH_NAME_CHK;
	WndKey = _T("PROLIB_SEARCH_NAME_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROLIB_SEARCH_MODEL_SIZE_CHK;
	WndKey = _T("PROLIB_SEARCH_MODEL_SIZE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROLIB_SEARCH_MODEL_SIZE_W_LABEL;
	WndKey = _T("PROLIB_SEARCH_MODEL_SIZE_W_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROLIB_SEARCH_MODEL_SIZE_H_LABEL;
	WndKey = _T("PROLIB_SEARCH_MODEL_SIZE_H_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROLIB_SEARCH_MODEL_SIZE_TOL_LABEL;
	WndKey = _T("PROLIB_SEARCH_MODEL_SIZE_TOL_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROLIB_SEARCH_BTN;
	WndKey = _T("PROLIB_SEARCH_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROLIB_MODEL_FRAME_INDEX_LABEL;
	WndKey = _T("PROLIB_MODEL_FRAME_INDEX_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROLIB_MODEL_ICON_SIZE_LABEL;
	WndKey = _T("PROLIB_MODEL_ICON_SIZE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//		
}
//-------------------------------------------------------------------------------------//
CString CProjectLibraryWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_PROJECT_LIBRARY_WND");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
CAOIProject* CProjectLibraryWnd::GetActiveProject()
{
	return m_ProjectPtr;
}
//-------------------------------------------------------------------------------------//
void CProjectLibraryWnd::SetActiveProject(CAOIProject* Ptr)
{
	m_ProjectPtr = Ptr;
}
//-------------------------------------------------------------------------------------//
CAOIModel* CProjectLibraryWnd::GetModelPtr()
{
	return m_ModelPtr;
}
//-------------------------------------------------------------------------------------//
void CProjectLibraryWnd::SetModelType(MODEL_TYPE Type)
{
	m_ModelType = Type;
}
//-------------------------------------------------------------------------------------//
void CProjectLibraryWnd::SetSearchName(LPCTSTR Name)
{
	m_SearchName = Name;
}
//-------------------------------------------------------------------------------------//
void CProjectLibraryWnd::SetBodyRegion(const TREGION4D &Rgn)
{
	m_BodyRgn = Rgn;
}
//-------------------------------------------------------------------------------------//
void CProjectLibraryWnd::UpdateWndCaptionText(MODEL_TYPE ModelType)
{
	CString str = m_WndText;
	if ( CAOIModel::CheckModelTypUseChipSizeMode(ModelType) == true )	
	{				
		CHIP_SIZE_MODE ChipSizeMode = CAOIModel::FindModelChipSizeMode(ModelType, m_BodyRgn);
		if ( CHIP_SIZE_NONE==ChipSizeMode || CHIP_SIZE_OTHERS==ChipSizeMode )
		{	str = m_WndText;	}
		else
		{
			CString strChipSize= CAOIModel::GetModelChipSizeModeText(ChipSizeMode);
			str.Format(_T("%s [%s]"), m_WndText, strChipSize);		
		}
	}
	CWnd::SetWindowText(str);
	return;
}
//-------------------------------------------------------------------------------------//
UINT CProjectLibraryWnd::GetModelTypeIcon(MODEL_TYPE ModelType, bool Small)
{
	return AOIDataDefine.GetModelTypeIcon(ModelType, Small);
}
//-------------------------------------------------------------------------------------//
bool CProjectLibraryWnd::ClearModelTypeListCtrl(CListCtrl &ListCtrl)
{
	m_StopModelTypeListBeSelected = true;
	JetAPI::ClearListCtrl(ListCtrl, FALSE);
	m_StopModelTypeListBeSelected = false;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectLibraryWnd::BuildModelTypeListCtrl(CListCtrl &ListCtrl)
{	
	ClearModelTypeListCtrl(ListCtrl);	
	//m_ProjectPtr = AOIDataCollect.GetActiveProject();

	int        nItem=0;
	UINT       TypeIcon=0;
	CString    TypeName;	
	const int  IconW = 80;//80
	const int  IconH = 80;//80
	const bool bSmallIcon = false;
	CBitmap    bmp;
	CSize      SpaceSize;
	COLORREF   clrMask=0x00000000;
	MODEL_TYPE ModelType;
	std::vector<MODEL_TYPE> ModelTypeList;
	CImageList &ImageList = m_ModelTypeImageList;
	CAOIModel::GetModelTypeList(ModelTypeList);	

	ImageList.DeleteImageList();	
	ImageList.Create(IconW, IconH, ILC_COLOR24, 0, 4);	//建立Image列表
	ListCtrl.SetImageList(&ImageList, LVSIL_NORMAL);
	SpaceSize.cx = IconW+8;//8
	SpaceSize.cy = IconH+24;//32
	SpaceSize = ListCtrl.SetIconSpacing(SpaceSize);	
	
	nItem = 0;
	const int ModelTypeCount=(int)(ModelTypeList.size());

	ListCtrl.SetRedraw(FALSE);
	m_StopModelTypeListBeSelected = true;		
	for ( int i=0; i<ModelTypeCount; i++ )
	{
		ModelType = ModelTypeList[i];
		TypeIcon = GetModelTypeIcon(ModelType, bSmallIcon);
		TypeName = AOIDataDefine.GetModelTypeText(ModelType);	
		ListCtrl.InsertItem(nItem, TypeName, nItem);
		ListCtrl.SetItemText(nItem, 0, TypeName);
		ListCtrl.SetItemData(nItem, (DWORD)ModelType);
		bmp.LoadBitmap(TypeIcon);
		ImageList.Add(&bmp, clrMask);	
		bmp.DeleteObject();
		nItem ++;
	}
	m_StopModelTypeListBeSelected = false;
	ListCtrl.SetRedraw(TRUE);
	ListCtrl.Invalidate();
	ListCtrl.UpdateWindow();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectLibraryWnd::SetModelTypeListCtrlItemSlected(MODEL_TYPE ModelType)
{
	int       i=0;
	const int Count = m_ModelTypeListWnd.GetItemCount();
	m_StopModelTypeListBeSelected = true;
	for ( i=0; i<Count; i++ )
	{
		if ( m_ModelTypeListWnd.GetItemData(i) == ModelType )
		{
			m_ModelTypeListWnd.SetItemState(i, LVIS_SELECTED|LVNI_FOCUSED, LVIS_SELECTED|LVNI_FOCUSED);
			break;
		}
	}	
	m_StopModelTypeListBeSelected = false;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectLibraryWnd::InitModelGroupListCtrl(CListCtrl &ListCtrl)
{
	JetAPI::InitialListCtrl(ListCtrl);
	JetAPI::ClearListCtrlHeaderList(ListCtrl);

	CString str;
	int   nCol = 0;
	int width2 = 96;
	RECT      Rect;	
	const int Align = LVCFMT_CENTER;//LVCFMT_CENTER;LVCFMT_RIGHT	

	ListCtrl.GetClientRect(&Rect);
	width2 = (Rect.right-Rect.left-16)/1;
	str = _T("Group");
	ListCtrl.InsertColumn(nCol, str, Align, width2);
	nCol ++;		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectLibraryWnd::ClearModelGroupListCtrl(CListCtrl &ListCtrl)
{
	m_StopModelGroupListBeSelected = true;
	JetAPI::ClearListCtrl(ListCtrl, FALSE);
	m_StopModelGroupListBeSelected = false;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectLibraryWnd::BuildModelGroupListCtrl(CListCtrl &ListCtrl, MODEL_TYPE ModelType, bool bBuildIconList)
{
	UpdateWndCaptionText(ModelType);
	ClearModelIconListCtrl(m_ModelIconListWnd);
	ClearModelGroupListCtrl(ListCtrl);
	CAOIProject *Project = GetActiveProject();
	if ( NULL == Project )
	{	return true;	}	

	CString GroupName;
	std::vector<CString> ModelGroupList;
	Project->GetProjectModelGroupList(ModelType, ModelGroupList);	

	size_t i=0;
	int    nItem=0;
	const size_t GroupCount = ModelGroupList.size();

	nItem=0;
	ListCtrl.SetRedraw(FALSE);
	m_StopModelGroupListBeSelected = true;
	for ( i=0; i<GroupCount; i++ )
	{
		ListCtrl.InsertItem(nItem, ModelGroupList[i]);
		ListCtrl.SetItemText(nItem, 0, ModelGroupList[i]);

		if ( 0 == i ) 
		{	GroupName = ModelGroupList[i]; }
		nItem ++;
	}
	m_StopModelGroupListBeSelected = false;
	int ActiveIndex=-1;
	if ( GroupCount > 1  )
	{	ActiveIndex = 1;	}
	else if ( GroupCount > 0  )
	{	ActiveIndex = 0;	}
	if ( -1 != ActiveIndex )
	{	ListCtrl.SetItemState(ActiveIndex, LVIS_SELECTED, LVIS_SELECTED);	}
	ListCtrl.SetRedraw(TRUE);
	ListCtrl.Invalidate();
	ListCtrl.UpdateWindow();

	if ( true==bBuildIconList && -1!=ActiveIndex ) 	
	{	BuildModelIconListCtrl(ModelType, ModelGroupList[ActiveIndex], m_ModelIconListWnd);	}		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectLibraryWnd::SetModelGroupListCtrlItemSlected(LPCTSTR  GroupName)
{
	int       i=0;
	CString   strItem;
	const int Count = m_ModelGroupListWnd.GetItemCount();
	m_StopModelGroupListBeSelected = true;
	for ( i=0; i<Count; i++ )
	{
		strItem = m_ModelGroupListWnd.GetItemText(i, 0);
		if ( strItem.CompareNoCase(GroupName) == 0 )
		{
			m_ModelGroupListWnd.SetItemState(i, LVIS_SELECTED|LVNI_FOCUSED, LVIS_SELECTED|LVNI_FOCUSED);
			break;
		}
	}	
	m_StopModelGroupListBeSelected = false;		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectLibraryWnd::ClearModelIconListCtrl(CListCtrl &ListCtrl)
{
	m_StopModelNameListBeSelected = true;	
	JetAPI::ClearListCtrl(ListCtrl, FALSE);
	CWnd::SetDlgItemInt(PROLIB_GROUP_MODEL_COUNT_EDIT, 0);
	m_StopModelNameListBeSelected = false;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectLibraryWnd::UpdateModelIconListCtrl(CListCtrl &ListCtrl)
{
	CAOIProject *Project = GetActiveProject();
	if ( NULL == Project )
	{	return true;	}	
	
	CString      TypeName;
	CDib         dib;	
	CBitmap      bmp;	
	HDC			 hMemDC = NULL;	
	HGDIOBJ		 hOldObj = NULL;
	HBRUSH       hBrush = NULL;
	CPalette    *pPalette = NULL;
	HPALETTE	 hPalette = NULL;	
	BITMAPINFO   BitMapInfo;
	BITMAPINFO  *pBitMapInfo = NULL; 
	HBITMAP      hBitMap = NULL;	
	unsigned int ModelBKImageIndex=0;
	const bool  bUseGeneralBKImageIndex=true;
	const int    IconW = JetAPI::GetComboxCurSelData(m_ModelIconSizeCombox);//128;//144;//80
	const int    IconH = JetAPI::GetComboxCurSelData(m_ModelIconSizeCombox);//128;//144;//80
	const unsigned int DefaultBKImageIndex = (unsigned int)(JetAPI::GetComboxCurSelData(m_ModelFrameIndexCombox));

	CSize        SpaceSize;
	COLORREF     clrMaskBk=RGB(0,0,0);
	COLORREF     clrMask=RGB(0,0,0);
	COLORREF     BKClr = 0x00;		
	CImageList  &ImageList = m_ModelIconImageList;
	RECT         IconRect={0, 0, IconW, IconH};

	ImageList.DeleteImageList();	
	ImageList.Create(IconW, IconH, ILC_COLOR24, 0, 4);	//建立Image列表
	ListCtrl.SetImageList(&ImageList, LVSIL_NORMAL);
	SpaceSize.cx = IconW+8;
	SpaceSize.cy = IconH+24;
	SpaceSize = ListCtrl.SetIconSpacing(SpaceSize);	

	int          i=0;
	size_t       LandCount=0;
	bool         IsOK = true;
	bool         bGroupAll = false;
	int          nItem=0;	
	int          BmpAddResultID=0;
	int          nWidth=0, nHeight=0;
	int          nTmpW=0, nTmpH=0;
	int          nItem_W=0, nItem_H=0;
	int          nDW=0, nDH=0;
	double       dTmpRatio=0.0;	
	IMAGE_SIZE   ImageW=0;
	IMAGE_SIZE   ImageH=0;
	IMAGE_SIZE   ImageStep=0;
	IMAGE_SIZE   BitCount=0;
	IMAGE_PTR    ImagePtr=NULL;		
	
	int          nTextX = 8;
	int          nTextY = 8;
	const int    nTextYPitch = 16;
	double       ModelSizeW=0;
	double       ModelSizeH=0;
	TREGION4D    ModelRgn;
	CString      strInfoSize;
	CString      strInfoLand;
	CString      strNoImage;
	CString      ModelName;
	CString      GroupNameM;
	CString      strItemText;
	CString      ModelBkImageName;		
	CAOIModel   *ModelPtr = NULL;
	const int    nAlign = 4;
	const int    ItemCount = ListCtrl.GetItemCount();

	BKClr = ::GetSysColor(COLOR_BTNFACE);
	BKClr = 0x000000;//0xFFFFFF;
	hBrush = ::CreateSolidBrush(BKClr);
	hMemDC = ::CreateCompatibleDC(NULL);
	::memset(&BitMapInfo, 0x00, sizeof(BitMapInfo));
	BitMapInfo.bmiHeader.biSize = sizeof(BitMapInfo.bmiHeader);		
	BitMapInfo.bmiHeader.biWidth = IconW;
	BitMapInfo.bmiHeader.biHeight = IconH;
	BitMapInfo.bmiHeader.biPlanes = 1;
	BitMapInfo.bmiHeader.biBitCount = 24;
	BitMapInfo.bmiHeader.biSizeImage = IconW*IconH*3;
	//hBitMap = ::CreateDIBSection(hMemDC, &BitMapInfo, DIB_RGB_COLORS,NULL, NULL, 0);
	//hOldObj = ::SelectObject(hMemDC, hBitMap);				
	// set stretch mode
	::SetStretchBltMode(hMemDC, COLORONCOLOR);//HALFTONE
	::SetTextColor(hMemDC, 0x0080FF);
	::SetBkMode(hMemDC, TRANSPARENT);

	nItem=0;
	strNoImage = _T("No Image");
	const int strNoImaeLen = strNoImage.GetLength();	
	ListCtrl.SetRedraw(FALSE);
	m_StopModelNameListBeSelected = true;
	for ( i=0; i<ItemCount; i++ )
	{		
		nItem = i;
		ModelPtr = (CAOIModel*)(ListCtrl.GetItemData(nItem)); 
		if ( NULL == ModelPtr ) { continue; }

		ModelPtr->GetModelRegion(ModelRgn);
		ModelSizeW = ModelRgn.GetWidth();
		ModelSizeH = ModelRgn.GetHeight();

		ModelName = ModelPtr->GetModelName();
		LandCount = ModelPtr->GetModelLandCount();		
		ModelBKImageIndex = ModelPtr->GetModelBKImageIndex();
		if ( true == bUseGeneralBKImageIndex )
		{	ModelBKImageIndex = DefaultBKImageIndex; }
		ModelBkImageName = ModelPtr->GetModelBKImageFilename(ModelBKImageIndex);

		nTextX = 8;
		nTextY = 8;
		strInfoSize.Format(_T("W:%.2f, H:%.2f mm"), ModelSizeW/1000.0, ModelSizeH/1000.0);
		strInfoLand.Format(_T("Land:%d"), LandCount);	

		hBitMap = ::CreateDIBSection(hMemDC, &BitMapInfo, DIB_RGB_COLORS,NULL, NULL, 0);
		hOldObj = ::SelectObject(hMemDC, hBitMap);
		::FillRect(hMemDC, &IconRect, hBrush);

		IsOK = ImageAPI.LoadImage(ModelBkImageName, ImageW, ImageH, ImageStep, BitCount, ImagePtr, nAlign, true);		
		if ( false == IsOK )
		{	
			::TextOut(hMemDC, nTextX, nTextY, strNoImage, strNoImaeLen);			
			::SelectObject(hMemDC, hOldObj);
			bmp.Attach(hBitMap);
			BmpAddResultID = ImageList.Add(&bmp, clrMask);			
			bmp.DeleteObject();
			continue;
		}
		if ( AOIDataCollect.ExecEnhanceDisplayImage(ImageW, ImageH, ImageStep, BitCount, ImagePtr, ImagePtr) == false ) 
		{ 
			::TextOut(hMemDC, nTextX, nTextY, strNoImage, strNoImaeLen);
			::SelectObject(hMemDC, hOldObj);
			bmp.Attach(hBitMap);
			BmpAddResultID = ImageList.Add(&bmp, clrMask);			
			bmp.DeleteObject();
			continue; 
		}
		if ( dib.SetImage(ImagePtr, ImageW, ImageH, ImageStep, BitCount, true) == false )
		{
			ImageW = ImageH = 0;
			JetMemory.free_func(ImagePtr);
			::TextOut(hMemDC, nTextX, nTextY, strNoImage, strNoImaeLen);			
			::SelectObject(hMemDC, hOldObj);
			bmp.Attach(hBitMap);
			BmpAddResultID = ImageList.Add(&bmp, clrMask);			
			bmp.DeleteObject();
			continue;
		}
		ImageW = ImageH = 0;
		JetMemory.free_func(ImagePtr);

		pBitMapInfo = dib.GetDIBInfo();
		nWidth = pBitMapInfo->bmiHeader.biWidth;
		nHeight = pBitMapInfo->bmiHeader.biHeight;

		nTmpW = nWidth;
		nTmpH = nHeight;
		dTmpRatio = 1.0;
		if( nTmpW > nTmpH )
		{
			dTmpRatio = nTmpH;
			dTmpRatio = dTmpRatio/nTmpW;
			nTmpW = IconW;
			nTmpH = (int)(nTmpW*dTmpRatio);
		}
		else
		{
			dTmpRatio = nTmpW;
			dTmpRatio = dTmpRatio/nTmpH;
			nTmpH = IconH;
			nTmpW = (int)(nTmpH*dTmpRatio);
		}
		nItem_W = nTmpW;
		nItem_H = nTmpH;		

		pPalette = dib.GetPalette();		
		if(pPalette != NULL)
		{
			hPalette = ::SelectPalette(hMemDC, (HPALETTE)pPalette->GetSafeHandle(), FALSE);
			::RealizePalette(hMemDC);	//maps entries from the current logical palette to the system palette.
		}

		nDW = (IconW-nItem_W)/2;
		nDH = (IconH-nItem_H)/2;
		// populate the thumbnail bitmap bits
		::StretchDIBits(hMemDC, nDW, nDH, 
					nItem_W, nItem_H, 
					0, 0, 
					nWidth,
					nHeight, 
					dib.GetDIBBits(), 
					dib.GetDIBInfo(), 
					BI_RGB, 
					SRCCOPY);
		
		// restore DC object
		::SelectObject(hMemDC, hOldObj);

		// restore DC palette
		if(pPalette != NULL)
		{	::SelectPalette(hMemDC, (HPALETTE)hPalette, FALSE); }		

		// clean up
		//::DeleteObject(hMemDC);	hMemDC = NULL;

		bmp.Attach(hBitMap);
		BmpAddResultID = ImageList.Add(&bmp, clrMask);			
		bmp.DeleteObject();		
	}
	m_StopModelNameListBeSelected = false;
	
	// clean up
	::DeleteObject(hMemDC);	hMemDC = NULL;
	::DeleteObject(hBrush); hBrush=NULL;

	ListCtrl.SetRedraw(TRUE);
	ListCtrl.Invalidate();
	ListCtrl.UpdateWindow();	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectLibraryWnd::BuildModelIconListCtrl(MODEL_TYPE ModelType, LPCTSTR  GroupName, CListCtrl &ListCtrl)
{
	ClearModelIconListCtrl(ListCtrl);
	CAOIProject *Project = GetActiveProject();
	if ( NULL == Project )
	{	return true;	}
	
	UINT         TypeIcon=IDB_MODEL_EMPTY_M_ICON;
	CString      TypeName;	
	const int    IconW = JetAPI::GetComboxCurSelData(m_ModelIconSizeCombox);//128;//144;//80
	const int    IconH = JetAPI::GetComboxCurSelData(m_ModelIconSizeCombox);//;//144;//80
	CDib         dib;	
	CBitmap      bmp;	
	HDC			 hMemDC = NULL;	
	HGDIOBJ		 hOldObj = NULL;
	HBRUSH       hBrush = NULL;
	CPalette    *pPalette = NULL;
	HPALETTE	 hPalette = NULL;	
	BITMAPINFO   BitMapInfo;
	BITMAPINFO  *pBitMapInfo = NULL; 
	HBITMAP      hBitMap = NULL;	
	unsigned int ModelBKImageIndex=0;
	const bool  bUseGeneralBKImageIndex=true;
	const unsigned int DefaultBKImageIndex = (unsigned int)(JetAPI::GetComboxCurSelData(m_ModelFrameIndexCombox));

	CSize        SpaceSize;
	COLORREF     clrMaskBk=RGB(0,0,0);
	COLORREF     clrMask=RGB(0,0,0);
	COLORREF     BKClr = 0x00;		
	CImageList  &ImageList = m_ModelIconImageList;
	RECT         IconRect={0, 0, IconW, IconH};

	ImageList.DeleteImageList();	
	ImageList.Create(IconW, IconH, ILC_COLOR24, 0, 4);	//建立Image列表
	ListCtrl.SetImageList(&ImageList, LVSIL_NORMAL);
	SpaceSize.cx = IconW+8;
	SpaceSize.cy = IconH+24;
	SpaceSize = ListCtrl.SetIconSpacing(SpaceSize);	

	size_t       i=0;
	size_t       LandCount=0;
	bool         IsOK = true;
	bool         bGroupAll = false;
	int          nItem=0;	
	int          BmpAddResultID=0;
	int          nWidth=0, nHeight=0;
	int          nTmpW=0, nTmpH=0;
	int          nItem_W=0, nItem_H=0;
	int          nDW=0, nDH=0;
	double       dTmpRatio=0.0;	
	IMAGE_SIZE   ImageW=0;
	IMAGE_SIZE   ImageH=0;
	IMAGE_SIZE   ImageStep=0;
	IMAGE_SIZE   BitCount=0;
	IMAGE_PTR    ImagePtr=NULL;		
	
	int          nTextX = 8;
	int          nTextY = 8;
	const int    nTextYPitch = 16;
	double       ModelSizeW=0;
	double       ModelSizeH=0;
	TREGION4D    ModelRgn;
	CString      strInfoSize;
	CString      strInfoLand;
	CString      strNoImage;
	CString      ModelName;
	CString      GroupNameM;
	CString      strItemText;
	CString      ModelBkImageName;
	CString      strGroupName = GroupName;
	CString      strGroupAllName = AOIDataDefine.GetModelGroupAllText();
	CAOIModel   *ModelPtr = NULL;
	const int    nAlign = 4;	
	const size_t ModelCount = Project->GetProjectModelCount();
	CSortObj     SortObj;
	CSortObj    *SortPtr = NULL;
	std::vector<CSortObj> SortList;

	if ( strGroupAllName.CompareNoCase(GroupName) == 0 ) 
	{	
		bGroupAll = true;	
		JetAPI::EnableCtrlWnd(this, EML_MODEL_ADD_BTN, FALSE);
	}
	else
	{	
		bGroupAll = false; 
		JetAPI::EnableCtrlWnd(this, EML_MODEL_ADD_BTN, TRUE);
	}
	SortObj.SetSortMode(SORT_BY_TXT);
	for ( i=0; i<ModelCount; i++ )
	{
		ModelPtr = Project->GetProjectModelPtr(i, false);
		if ( NULL == ModelPtr ) { continue; }

		if ( CAOIModel::CheckModelTypeEnabled(ModelType) == true )
		{
			if ( ModelPtr->GetModelType() != ModelType ) { continue; }
		}

		if ( false == bGroupAll )
		{
			GroupNameM = ModelPtr->GetModelGroupName();
			GroupNameM.MakeUpper();
			if ( strGroupName != GroupNameM ) { continue; }
		}

		ModelName = ModelPtr->GetModelName();
		
		SortObj.SetID(i);
		SortObj.SetPtr(ModelPtr);
		SortObj.SetValueStr(ModelName);
		SortList.push_back(SortObj);
	}
	std::sort(SortList.begin(), SortList.end());
	const size_t SortCount = SortList.size();

	BKClr = ::GetSysColor(COLOR_BTNFACE);
	BKClr = 0x000000;//0xFFFFFF;
	hBrush = ::CreateSolidBrush(BKClr);
	hMemDC = ::CreateCompatibleDC(NULL);
	::memset(&BitMapInfo, 0x00, sizeof(BitMapInfo));
	BitMapInfo.bmiHeader.biSize = sizeof(BitMapInfo.bmiHeader);		
	BitMapInfo.bmiHeader.biWidth = IconW;
	BitMapInfo.bmiHeader.biHeight = IconH;
	BitMapInfo.bmiHeader.biPlanes = 1;
	BitMapInfo.bmiHeader.biBitCount = 24;
	BitMapInfo.bmiHeader.biSizeImage = IconW*IconH*3;
	//hBitMap = ::CreateDIBSection(hMemDC, &BitMapInfo, DIB_RGB_COLORS,NULL, NULL, 0);
	//hOldObj = ::SelectObject(hMemDC, hBitMap);				
	// set stretch mode
	::SetStretchBltMode(hMemDC, COLORONCOLOR);//HALFTONE
	::SetTextColor(hMemDC, 0x0080FF);
	::SetBkMode(hMemDC, TRANSPARENT);

	TypeIcon = GetModelTypeIcon(ModelType, false);

	nItem=0;
	strNoImage = _T("No Image");
	const int strNoImaeLen = strNoImage.GetLength();
	strGroupName.MakeUpper();
	ListCtrl.SetRedraw(FALSE);
	m_StopModelNameListBeSelected = true;
	for ( i=0; i<SortCount; i++ )
	{
		SortPtr = &(SortList[i]);
		ModelPtr = (CAOIModel*)(SortPtr->GetPtr());
		if ( NULL == ModelPtr ) { continue; }

		ModelPtr->GetModelRegion(ModelRgn);
		ModelSizeW = ModelRgn.GetWidth();
		ModelSizeH = ModelRgn.GetHeight();

		ModelName = ModelPtr->GetModelName();
		LandCount = ModelPtr->GetModelLandCount();		
		ModelBKImageIndex = ModelPtr->GetModelBKImageIndex();
		if ( true == bUseGeneralBKImageIndex )
		{	ModelBKImageIndex = DefaultBKImageIndex; }
		ModelBkImageName = ModelPtr->GetModelBKImageFilename(ModelBKImageIndex);		

		strItemText = ModelName;		
		ListCtrl.InsertItem(nItem, strItemText, nItem);
		ListCtrl.SetItemData(nItem, (DWORD_PTR)ModelPtr);
		ListCtrl.SetItemText(nItem, 0, strItemText);		
		nItem ++;

		nTextX = 8;
		nTextY = 8;
		strInfoSize.Format(_T("W:%.2f, H:%.2f mm"), ModelSizeW/1000.0, ModelSizeH/1000.0);
		strInfoLand.Format(_T("Land:%d"), LandCount);	

		hBitMap = ::CreateDIBSection(hMemDC, &BitMapInfo, DIB_RGB_COLORS,NULL, NULL, 0);
		hOldObj = ::SelectObject(hMemDC, hBitMap);
		::FillRect(hMemDC, &IconRect, hBrush);

		IsOK = ImageAPI.LoadImage(ModelBkImageName, ImageW, ImageH, ImageStep, BitCount, ImagePtr, nAlign, true);		
		if ( false == IsOK )
		{
			if ( true==bUseGeneralBKImageIndex && 0!=DefaultBKImageIndex )
			{
				ModelBKImageIndex = 0;//重新取編號0的底圖
				ModelBkImageName = ModelPtr->GetModelBKImageFilename(ModelBKImageIndex);
				IsOK = ImageAPI.LoadImage(ModelBkImageName, ImageW, ImageH, ImageStep, BitCount, ImagePtr, nAlign, true);		
			}
			if ( false == IsOK )
			{
				//bmp.LoadBitmap(TypeIcon);
				//BmpAddResultID = ImageList.Add(&bmp, clrMaskBk);	
				//bmp.DeleteObject();			
				::TextOut(hMemDC, nTextX, nTextY, strNoImage, strNoImaeLen);			
				::SelectObject(hMemDC, hOldObj);
				bmp.Attach(hBitMap);
				BmpAddResultID = ImageList.Add(&bmp, clrMask);			
				bmp.DeleteObject();
				continue;
			}
		}
		if ( AOIDataCollect.ExecEnhanceDisplayImage(ImageW, ImageH, ImageStep, BitCount, ImagePtr, ImagePtr) == false ) 
		{ 
			//bmp.LoadBitmap(TypeIcon);
			//BmpAddResultID = ImageList.Add(&bmp, clrMaskBk);	
			//bmp.DeleteObject();	
			::TextOut(hMemDC, nTextX, nTextY, strNoImage, strNoImaeLen);
			::SelectObject(hMemDC, hOldObj);
			bmp.Attach(hBitMap);
			BmpAddResultID = ImageList.Add(&bmp, clrMask);			
			bmp.DeleteObject();
			continue; 
		}
		if ( dib.SetImage(ImagePtr, ImageW, ImageH, ImageStep, BitCount, true) == false )
		{
			ImageW = ImageH = 0;
			JetMemory.free_func(ImagePtr);				
			//bmp.LoadBitmap(TypeIcon);
			//BmpAddResultID = ImageList.Add(&bmp, clrMaskBk);	
			//bmp.DeleteObject();			
			::TextOut(hMemDC, nTextX, nTextY, strNoImage, strNoImaeLen);			
			::SelectObject(hMemDC, hOldObj);
			bmp.Attach(hBitMap);
			BmpAddResultID = ImageList.Add(&bmp, clrMask);			
			bmp.DeleteObject();
			continue;
		}
		ImageW = ImageH = 0;
		JetMemory.free_func(ImagePtr);

		pBitMapInfo = dib.GetDIBInfo();
		nWidth = pBitMapInfo->bmiHeader.biWidth;
		nHeight = pBitMapInfo->bmiHeader.biHeight;

		nTmpW = nWidth;
		nTmpH = nHeight;
		dTmpRatio = 1.0;
		if( nTmpW > nTmpH )
		{
			dTmpRatio = nTmpH;
			dTmpRatio = dTmpRatio/nTmpW;
			nTmpW = IconW;
			nTmpH = (int)(nTmpW*dTmpRatio);
		}
		else
		{
			dTmpRatio = nTmpW;
			dTmpRatio = dTmpRatio/nTmpH;
			nTmpH = IconH;
			nTmpW = (int)(nTmpH*dTmpRatio);
		}
		nItem_W = nTmpW;
		nItem_H = nTmpH;		

		pPalette = dib.GetPalette();		
		if(pPalette != NULL)
		{
			hPalette = ::SelectPalette(hMemDC, (HPALETTE)pPalette->GetSafeHandle(), FALSE);
			::RealizePalette(hMemDC);	//maps entries from the current logical palette to the system palette.
		}

		nDW = (IconW-nItem_W)/2;
		nDH = (IconH-nItem_H)/2;
		// populate the thumbnail bitmap bits
		::StretchDIBits(hMemDC, nDW, nDH, 
					nItem_W, nItem_H, 
					0, 0, 
					nWidth,
					nHeight, 
					dib.GetDIBBits(), 
					dib.GetDIBInfo(), 
					BI_RGB, 
					SRCCOPY);
		
		// restore DC object
		::SelectObject(hMemDC, hOldObj);

		// restore DC palette
		if(pPalette != NULL)
		{	::SelectPalette(hMemDC, (HPALETTE)hPalette, FALSE); }		

		// clean up
		//::DeleteObject(hMemDC);	hMemDC = NULL;

		bmp.Attach(hBitMap);
		BmpAddResultID = ImageList.Add(&bmp, clrMask);			
		bmp.DeleteObject();		
	}
	m_StopModelNameListBeSelected = false;
	
	// clean up
	::DeleteObject(hMemDC);	hMemDC = NULL;
	::DeleteObject(hBrush); hBrush=NULL;

	const int ItemCount = ListCtrl.GetItemCount();
	CWnd::SetDlgItemInt(PROLIB_GROUP_MODEL_COUNT_EDIT, ItemCount);

	ListCtrl.SetRedraw(TRUE);
	ListCtrl.Invalidate();
	ListCtrl.UpdateWindow();	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectLibraryWnd::SetModelIconListCtrlItemSlected(LPCTSTR  ModelName)
{
	int       i=0;
	CString   strItem;
	const int Count = m_ModelIconListWnd.GetItemCount();
	m_StopModelNameListBeSelected = true;
	for ( i=0; i<Count; i++ )
	{
		strItem = m_ModelIconListWnd.GetItemText(i, 0);
		if ( strItem.CompareNoCase(ModelName) == 0 )
		{
			m_ModelIconListWnd.SetItemState(i, LVIS_SELECTED|LVNI_FOCUSED, LVIS_SELECTED|LVNI_FOCUSED);
			break;
		}
	}	
	m_StopModelNameListBeSelected = false;		
	return true;
}
//-------------------------------------------------------------------------------------//
void CProjectLibraryWnd::OnSearchBtn() 
{
	// TODO: Add your control notification handler code here
	CAOIProject *Project = GetActiveProject();
	if ( NULL == Project )	{	return ; }

	CString   str;
	CString   strFindName;
	const int SizeW = CWnd::GetDlgItemInt(PROLIB_SEARCH_MODEL_SIZE_W_EDIT);
	const int SizeH = CWnd::GetDlgItemInt(PROLIB_SEARCH_MODEL_SIZE_H_EDIT);
	const int Tolerance = CWnd::GetDlgItemInt(PROLIB_SEARCH_MODEL_SIZE_TOL_EDIT);
	BOOL bFindName = CWnd::IsDlgButtonChecked(PROLIB_SEARCH_NAME_CHK);
	BOOL bFindSize = CWnd::IsDlgButtonChecked(PROLIB_SEARCH_MODEL_SIZE_CHK);
	CWnd::GetDlgItemText(PROLIB_SEARCH_NAME_EDIT, strFindName);
	
	strFindName.MakeUpper();	

	size_t       i=0;
	TREGION4D    ModelRgn;
	double       ModelSizeW=0;
	double       ModelSizeH=0;
	CString      ModelName;
	CAOIModel   *ModelPtr = NULL;
	const size_t ModelCount = Project->GetProjectModelCount();
	for ( i=0; i<ModelCount; i++ )
	{
		ModelPtr = Project->GetProjectModelPtr(i, false);
		if ( NULL == ModelPtr ) { continue; }

		if ( TRUE == bFindName )
		{
			ModelName = ModelPtr->GetModelName();
			if ( ModelName.CompareNoCase(strFindName) == 0 ) 
			{	break; }
		}

		if ( TRUE == bFindSize ) 
		{
			ModelPtr->GetModelRegion(ModelRgn);
			ModelSizeW = ModelRgn.GetWidth();
			ModelSizeH = ModelRgn.GetHeight();
			if ( fabs(SizeW-ModelSizeW)<Tolerance && fabs(SizeH-ModelSizeH)<Tolerance ) 
			{	break; }
		}
	}	
	if ( i == ModelCount ) 
	{	
		//尋找片斷相同的
		CAOIModel *ModelPtrCur = NULL;
		CListCtrl &ListCtrl = m_ModelIconListWnd;
		const int nItem = ListCtrl.GetNextItem(-1, LVNI_FOCUSED);		
		if ( nItem >= 0 )
		{	ModelPtrCur = (CAOIModel*)(ListCtrl.GetItemData(nItem));	}

		if ( NULL != ModelPtrCur )
		{
			bool bStartFind=false;
			//往後找
			for ( i=0; i<ModelCount; i++ )
			{
				ModelPtr = Project->GetProjectModelPtr(i, false);
				if ( NULL == ModelPtr ) { continue; }
				if ( ModelPtr == ModelPtrCur )
				{
					bStartFind = true;
					continue;
				}
				if ( false == bStartFind ) { continue; }
				ModelName = ModelPtr->GetModelName();
				if ( JetAPI::FindTextInString(strFindName, ModelName) == true )
				{	break;	}
			}
		}
		
		if ( i == ModelCount )
		{
			//全找
			for ( i=0; i<ModelCount; i++ )
			{
				ModelPtr = Project->GetProjectModelPtr(i, false);
				if ( NULL == ModelPtr ) { continue; }
				ModelName = ModelPtr->GetModelName();
				if ( JetAPI::FindTextInString(strFindName, ModelName) == true )
				{	break;	}
			}		
		}
		if ( i == ModelCount )
		{
			str = _T("Can not find the model");
			str = LoadMultiLanguageString(str, str);
			JetAPI::ShowMessageBox(str);
		}
	}

	m_ModelPtr = ModelPtr;
	ModelName = ModelPtr->GetModelName();
	CString GroupName = ModelPtr->GetModelGroupName();
	const MODEL_TYPE ModelType = ModelPtr->GetModelType();
	BuildModelGroupListCtrl(m_ModelGroupListWnd, ModelType, false);
	BuildModelIconListCtrl(ModelType, GroupName, m_ModelIconListWnd);

	SetModelTypeListCtrlItemSlected(ModelType);
	SetModelGroupListCtrlItemSlected(GroupName);
	SetModelIconListCtrlItemSlected(ModelName);
	m_ModelIconListWnd.SetFocus();
	CWnd::SetDlgItemText(PROLIB_SELECTED_MODEL_NAME_EDIT, ModelName);	

	AOIDataCollect.CreateModelPreViewPtr(ModelPtr);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_REDRAW_VIEW_WND, NULL);	
}
//-------------------------------------------------------------------------------------//
void CProjectLibraryWnd::OnSelchangeModelBKImageIndexCombox()
{
	const int nTypeItem = (int)(m_ModelTypeListWnd.GetNextItem(-1, LVNI_SELECTED));
	if ( nTypeItem < 0 ) { return; }
	MODEL_TYPE ModelType = (MODEL_TYPE)(m_ModelTypeListWnd.GetItemData(nTypeItem));

	const int nGroupItem = (int)(m_ModelGroupListWnd.GetNextItem(-1, LVNI_SELECTED));
	if ( nGroupItem < 0 ) { return; }
	CString GroupName = m_ModelGroupListWnd.GetItemText(nGroupItem, 0);

	BuildModelIconListCtrl(ModelType, GroupName, m_ModelIconListWnd);
	return ;
}
//-------------------------------------------------------------------------------------//
void CProjectLibraryWnd::OnSelchangeModelIconSizeCombox()
{	
	const int nTypeItem = (int)(m_ModelTypeListWnd.GetNextItem(-1, LVNI_SELECTED));
	if ( nTypeItem < 0 ) { return; }
	MODEL_TYPE ModelType = (MODEL_TYPE)(m_ModelTypeListWnd.GetItemData(nTypeItem));

	const int nGroupItem = (int)(m_ModelGroupListWnd.GetNextItem(-1, LVNI_SELECTED));
	if ( nGroupItem < 0 ) { return; }
	CString GroupName = m_ModelGroupListWnd.GetItemText(nGroupItem, 0);

	BuildModelIconListCtrl(ModelType, GroupName, m_ModelIconListWnd);
	return ;
}
//-------------------------------------------------------------------------------------//