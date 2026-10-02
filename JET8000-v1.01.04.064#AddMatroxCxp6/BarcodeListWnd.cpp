// BarcodeListWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "BarcodeListWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
#define    CMP_COL_INDEX                0
#define    CMP_COL_PANEL                1
#define    CMP_COL_BOARD                2
#define    CMP_COL_BARCODE              3
//-------------------------------------------------------------------------------------//
int CALLBACK BarcodeListCompareFn(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
int CALLBACK BarcodeListCompareFn(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	if ( NULL == lParamSort ) { return 0; }
	CBarcodeListWnd* pBarcodeListWnd = (CBarcodeListWnd*)lParamSort;
	return pBarcodeListWnd->CompareBarcodeItem(lParam1, lParam2);
}
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CBarcodeListWnd dialog
//-------------------------------------------------------------------------------------//
CBarcodeListWnd::CBarcodeListWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CBarcodeListWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBarcodeListWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nItemAct = -1;
	m_nSubItemAct = -1;
	m_ProjectPtr = NULL;
	m_BarcodePtr = NULL;
	m_BarcodeColID = CMP_COL_INDEX;
	m_BarcodeListSortMode = SORT_ASCEND;
	m_StopBarcodeListBeSelected = false;
}
//-------------------------------------------------------------------------------------//
void CBarcodeListWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBarcodeListWnd)
	DDX_Control(pDX, BARLIST_PARAM_EDIT, m_EditCtrl);	
	DDX_Control(pDX, BARLIST_BARCODE_LIST_WND, m_BarcodeListWnd);	
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CBarcodeListWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CBarcodeListWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_GETMINMAXINFO()
	ON_NOTIFY(LVN_COLUMNCLICK, BARLIST_BARCODE_LIST_WND, OnColumnclickBarcodeListWnd)
	ON_NOTIFY(LVN_ITEMCHANGED, BARLIST_BARCODE_LIST_WND, OnItemchangedBarcodeListWnd)
	ON_NOTIFY(NM_DBLCLK, BARLIST_BARCODE_LIST_WND, OnDblclkBarcodeListWnd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CBarcodeListWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CBarcodeListWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	JetAPI::InitialListCtrl(m_BarcodeListWnd);	
	BuildBarcodeListWndHeader();
	SwitchMultiLanguage();

	BuildBarcodeListWnd();
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	m_BarcodeListWnd.SetFocus();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CBarcodeListWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CBarcodeListWnd::OnSize(UINT nType, int cx, int cy) 
{
	CBaseDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if ( CWnd::GetSafeHwnd() == NULL ) { return; }

	CWnd     *WndPtr=NULL;
	const int MarginW=4;
	const int MarginH=4;

	if ( m_BarcodeListWnd.GetSafeHwnd() != NULL )
	{
		RECT WndRect={0};
		m_BarcodeListWnd.GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		WndRect.right = cx-MarginW;
		WndRect.bottom = cy-MarginH;
		m_BarcodeListWnd.MoveWindow(&WndRect);
	}
}
//-------------------------------------------------------------------------------------//
void CBarcodeListWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CBaseDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CBarcodeListWnd::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default
	
	CBaseDialog::OnGetMinMaxInfo(lpMMI);
	lpMMI->ptMinTrackSize.x = 640;
	lpMMI->ptMinTrackSize.y = 320;
}
//-------------------------------------------------------------------------------------//
BOOL CBarcodeListWnd::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch ( pMsg->message )
	{
	case WM_KEYDOWN:
		switch ( pMsg->wParam )
		{
		case VK_RETURN:
			if ( m_EditCtrl.IsWindowVisible() == TRUE )
			{	
				ExecUpdateParamByEdit(); 
				ExecReleaseParamCtrl();
			}			
			return TRUE;
			break;
		case VK_ESCAPE:
			ExecReleaseParamCtrl();			
			return TRUE;			
			break;
		}
		break;
	}
	return CBaseDialog::PreTranslateMessage(pMsg);
}
//-------------------------------------------------------------------------------------//
LRESULT CBarcodeListWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CBaseDialog::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void CBarcodeListWnd::SetProjectPtr(CAOIProject *Ptr)
{
	m_ProjectPtr = Ptr;		
	m_BarcodePtr = NULL;
	SetBarcodeListSortMode(1);	
	SetBarcodeListColID(CMP_COL_INDEX);
}
//-------------------------------------------------------------------------------------//
inline CAOIProject* CBarcodeListWnd::GetActiveProject()
{
	return m_ProjectPtr;
}
//-------------------------------------------------------------------------------------//
void CBarcodeListWnd::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_BARCODE_LIST_WND");
	//---------------------------------------------------------------------------------//
	WndID = IDD_BARCODE_LIST_WND;
	WndKey = _T("IDD_BARCODE_LIST_WND");
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
	/*
	WndID = AAAAAAAAAAAAAAAAAAAAA;
	WndKey = _T("AAAAAAAAAAAAAAAAAAAAA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		
	*/
	//---------------------------------------------------------------------------------//
}
//-------------------------------------------------------------------------------------//
CString CBarcodeListWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_BARCODE_LIST_WND");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
bool CBarcodeListWnd::BuildBarcodeListWnd()
{
	CThisListCtrl_49 &ListCtrl = m_BarcodeListWnd;
	CAOIProject *ProjectPtr = GetActiveProject();
	ClearBarcodeListWnd();
	if ( NULL == ProjectPtr )
	{	return true; }
	
	size_t         i=0, j=0;
	CString        str;	
	unsigned int   PanelIndex=0;	
	unsigned int   BoardIndex=0;	
	int            nItem=0;
	int            nSubItem=0;	
	bool           bAlarm=false;
	double         dCalcTime=0.0;
	RESULT_ID      ResultID;
	CAOIBarcode   *BarcodePtr = NULL;
	const size_t   BarcodeCount = ProjectPtr->GetProjectBarcodeCount();

	ListCtrl.SetRedraw(FALSE);
	m_StopBarcodeListBeSelected = true;
	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = ProjectPtr->GetProjectBarcodePtr(i, false);
		if ( NULL == BarcodePtr ) { continue; }
		
		PanelIndex = BarcodePtr->GetBarcodePanelIndex_Project();
		BoardIndex = BarcodePtr->GetBarcodeBoardIndex_Panel();

		nSubItem = 0;
		str.Format(_T("%d"), i+1);
		ListCtrl.InsertItem(nItem, str);
		ListCtrl.SetItemData(nItem, (DWORD_PTR)m_BarcodeList.size());
		m_BarcodeList.push_back(BarcodePtr);

		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;
		
		//CMP_COL_PANEL
		str.Format(_T("%d"), PanelIndex+1);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//CMP_COL_BOARD
		str.Format(_T("%d"), BoardIndex+1);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//CMP_COL_BARCODE
		str = BarcodePtr->GetBarcodeResultText();
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;	
		//----
		nItem ++;
	}
	m_StopBarcodeListBeSelected = false;
	ListCtrl.SetRedraw(TRUE);

	SetBarcodeListColID(CMP_COL_INDEX);	
	SetBarcodeListSortMode(SORT_ASCEND);	

	CString strCount = AOIDataDefine.GetCountText();
	str.Format(_T("%s:%d"), strCount, ListCtrl.GetItemCount());
	CWnd::SetDlgItemText(BARLIST_INFO_EDIT, str);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcodeListWnd::UpdateBarcodeListWnd()
{
	CThisListCtrl_49 &ListCtrl = m_BarcodeListWnd;		
	
	size_t         i=0, j=0;
	CString        str;	
	unsigned int   PanelIndex=0;	
	unsigned int   BoardIndex=0;	
	int            nItem=0;
	int            nSubItem=0;	
	bool           bAlarm=false;
	double         dCalcTime=0.0;
	RESULT_ID      ResultID;		
	CAOIBarcode   *BarcodePtr = NULL;
	const int      ItemCount = ListCtrl.GetItemCount();
	const size_t   BarcodeCount = m_BarcodeList.size();

	ListCtrl.SetRedraw(FALSE);	
	for ( i=0; i<ItemCount; i++ )
	{
		PanelIndex = ListCtrl.GetItemData(i);
		if ( PanelIndex >= BarcodeCount ) { continue; }
		BarcodePtr = m_BarcodeList[PanelIndex];
		if ( NULL == BarcodePtr ) { continue; }		

		PanelIndex = BarcodePtr->GetBarcodePanelIndex_Project();
		BoardIndex = BarcodePtr->GetBarcodeBoardIndex_Panel();

		nSubItem = 0;
		//str.Format(_T("%d"), i+1);		
		//ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;
		
		//CMP_COL_PANEL
		str.Format(_T("%d"), PanelIndex+1);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//CMP_COL_BOARD
		str.Format(_T("%d"), BoardIndex+1);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//CMP_COL_BARCODE
		str = BarcodePtr->GetBarcodeResultText();
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;	
		//----
		nItem ++;
	}	
	ListCtrl.SetRedraw(TRUE);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcodeListWnd::ClearBarcodeListWnd()
{
	CThisListCtrl_49 &ListCtrl = m_BarcodeListWnd;
	m_StopBarcodeListBeSelected = true;
	SetItemIndexAct(-1, -1);
	m_BarcodeList.clear();
	JetAPI::ClearListCtrl(ListCtrl, FALSE);	
	m_StopBarcodeListBeSelected = false;
	CWnd::SetDlgItemText(BARLIST_INFO_EDIT, _T(""));	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcodeListWnd::BuildBarcodeListWndHeader()
{
	CString str;
	int   nCol = 0;
	int width  = 64;
	int width2 = 64;
	RECT      Rect;	
	const int nCols = 10;
	const int Align = LVCFMT_LEFT;//LVCFMT_CENTER;LVCFMT_RIGHT, LVCFMT_LEFT

	{
		CThisListCtrl_49 &ListCtrl = m_BarcodeListWnd;		

		ListCtrl.GetClientRect(&Rect);
		width = 64;
		width2 = (Rect.right-Rect.left-width-64)/nCols;

		str = _T("Index");
		str = AOIDataDefine.GetIndexText();		
		ListCtrl.InsertColumn(nCol, str, Align, width);		
		
		nCol ++;
		
		str = _T("Panel");
		str = AOIDataDefine.GetPanelText();
		ListCtrl.InsertColumn(nCol, str, Align, width);
		nCol ++;		

		str = _T("Board");
		str = AOIDataDefine.GetBoardText();
		ListCtrl.InsertColumn(nCol, str, Align, width);
		nCol ++;				

		str = _T("Barcode");
		str = AOIDataDefine.GetBarcodeText();
		ListCtrl.InsertColumn(nCol, str, Align, width*4);
		nCol ++;	
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CBarcodeListWnd::SetBarcodeListColID(int val)
{
	m_BarcodeColID = val;
}
//-------------------------------------------------------------------------------------//
int CBarcodeListWnd::GetBarcodeListColD() const
{
	return m_BarcodeColID;
}
//-------------------------------------------------------------------------------------//
void CBarcodeListWnd::SetBarcodeListSortMode(int val)
{
	m_BarcodeListSortMode = val;
}
//-------------------------------------------------------------------------------------//
int CBarcodeListWnd::GetBarcodeListSortMode() const
{
	return m_BarcodeListSortMode;
}
//-------------------------------------------------------------------------------------//
int CBarcodeListWnd::CompareBarcodeItem(size_t index1, size_t index2)
{
	CThisListCtrl_49 &ListCtrl = m_BarcodeListWnd;
	const int ColID = GetBarcodeListColD();
	const size_t PanelCount = m_BarcodeList.size();
	if ( index1>=PanelCount || index2>=PanelCount )
	{	return 0; }
	int          Res=0;	
	CString      str1;
	CString      str2;
	wchar_t     *wsPtr1=NULL;
	wchar_t     *wsPtr2=NULL;
	double       dVal1=0.0;
	double       dVal2=0.0;
	unsigned int uVal1=0;
	unsigned int uVal2=0;	
	CAOIBarcode *BPtr1=(m_BarcodeList[index1]);
	CAOIBarcode *BPtr2=(m_BarcodeList[index2]);	
	if ( NULL==BPtr1 || NULL==BPtr2 ) { return 0; }

	switch ( ColID )
	{	
	case CMP_COL_INDEX:
		if ( index1 > index2 ) { Res = 1; }
		else if ( index1 < index2 ) { Res = -1; }
		else {	Res=0; }
		break;
	case CMP_COL_PANEL:
		uVal1 = BPtr1->GetBarcodePanelIndex_Project();
		uVal2 = BPtr2->GetBarcodePanelIndex_Project();
		if ( uVal1 > uVal2 ) { Res = 1; }
		else if ( uVal1 < uVal2 ) { Res = -1; }
		else {	Res=0; }
		break;	
	case CMP_COL_BOARD:
		uVal1 = BPtr1->GetBarcodeBoardIndex_Panel();
		uVal2 = BPtr2->GetBarcodeBoardIndex_Panel();
		if ( uVal1 > uVal2 ) { Res = 1; }
		else if ( uVal1 < uVal2 ) { Res = -1; }
		else {	Res=0; }
		break;
	case CMP_COL_BARCODE:
		wsPtr1= (wchar_t*)BPtr1->GetBarcodeResultText();
		wsPtr2= (wchar_t*)BPtr2->GetBarcodeResultText();
		Res = ::wcscmp(wsPtr1, wsPtr2);		
		break;	
	default:
		Res = 0;
		break;
	}
	int SortMode = GetBarcodeListSortMode();
	Res = Res*SortMode;	
	return Res;	
}
//-------------------------------------------------------------------------------------//
void CBarcodeListWnd::OnColumnclickBarcodeListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	int ColID = GetBarcodeListColD();
	int SortMode = GetBarcodeListSortMode();	
	const int ColumnsIdx = pNMListView->iSubItem;
	if ( ColID != ColumnsIdx )
	{	SortMode = SORT_ASCEND; }
	else
	{
		if ( SORT_ASCEND == SortMode ) { SortMode = SORT_DESCEND; }
		else {	SortMode = SORT_ASCEND;  }
	}
	SetBarcodeListColID(ColumnsIdx);	
	SetBarcodeListSortMode(SortMode);	
	m_BarcodeListWnd.SortItems(BarcodeListCompareFn, (DWORD_PTR)this);
	
	const int nItem = m_BarcodeListWnd.GetNextItem(-1, LVNI_FOCUSED);
	if ( nItem >= 0 ) 
	{	m_BarcodeListWnd.EnsureVisible(nItem, FALSE); }
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CBarcodeListWnd::OnItemchangedBarcodeListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if ( true == m_StopBarcodeListBeSelected ) { return; }
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) 
	{	return;	}
	DWORD Res = pNMListView->uNewState&LVIS_SELECTED;
	if ( Res == 0 ) { return; }	
	Res = pNMListView->uNewState&LVIS_FOCUSED;
	if ( Res == 0 ) { return; }	
	const size_t SelIndex = m_BarcodeListWnd.GetItemData(nItem);
	const size_t FdCount = m_BarcodeList.size();
	if ( SelIndex > FdCount ) { return; }
	CAOIBarcode *BarcodePtr = m_BarcodeList[SelIndex];
	if ( NULL == BarcodePtr ) { return; }	
	AOIDataCollect.MoveStageToBarcode(BarcodePtr, false);		
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CBarcodeListWnd::OnDblclkBarcodeListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NMITEMACTIVATE *pNMListView = (NMITEMACTIVATE*)pNMHDR;	
	m_BarcodePtr = NULL;
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	SetItemIndexAct(-1, -1);
	if ( nItem < 0 ) { return; }
	CThisListCtrl_49 &ListCtrl=m_BarcodeListWnd;
	const size_t SelIndex = ListCtrl.GetItemData(nItem);
	const size_t FdCount = m_BarcodeList.size();
	if ( SelIndex > FdCount ) { return; }
	CAOIBarcode *BarcodePtr = m_BarcodeList[SelIndex];
	if ( NULL == BarcodePtr ) { return; }	
	AOIDataCollect.MoveStageToBarcode(BarcodePtr, true);

	CString str;
	CRect   ItemRect={0};
	bool    bShowEdit=false;
	SetBarcodeListColID(nSubItem);	
	switch ( nSubItem )
	{
	//case CMP_COL_FD_SORT:
	//	bShowEdit = true;
	//	str.Format(_T("%d"), FdPtr->GetFdSortID());
	//	break;	
	}
	if ( ListCtrl.GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, ItemRect) == FALSE )
	{	return ; }

	SetItemIndexAct(nItem, nSubItem);
	if ( true == bShowEdit )
	{
		RECT CtrlRect=ItemRect;
		ListCtrl.ClientToScreen(&CtrlRect);
		CWnd::ScreenToClient(&CtrlRect);
		m_BarcodePtr = BarcodePtr;
		m_EditCtrl.MoveWindow(&CtrlRect, FALSE);
		m_EditCtrl.SetWindowText(str);
		m_EditCtrl.SetSel(0,-1);		
		m_EditCtrl.ShowWindow(SW_SHOW);		
		m_EditCtrl.SetFocus();
		m_EditCtrl.BringWindowToTop();
		ListCtrl.UpdateWindow();
		//m_EditCtrl.Invalidate();	
	}
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
bool CBarcodeListWnd::ExecUpdateParamByEdit()
{
	if ( m_EditCtrl.GetSafeHwnd() == NULL ) { return false; }

	m_EditCtrl.ShowWindow(SW_HIDE);
	if ( NULL == m_BarcodePtr ) { return true; }
	const int nItem = m_nItemAct;
	const int nSubItem = m_nSubItemAct;
	if ( -1==nItem || -1==nSubItem ) { return true; }

	CString str;
	CString ItemText;
	int        nValue=0;
	bool       bUpdateAll=false;
	const int  nColID = GetBarcodeListColD();
	CAOIBarcode *BarcodePtr=m_BarcodePtr;	

	m_EditCtrl.GetWindowText(ItemText);
	ItemText.MakeUpper();		
	if ( true == bUpdateAll ) 
	{	UpdateBarcodeListWnd();	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcodeListWnd::ExecReleaseParamCtrl()
{
	m_BarcodePtr = NULL;
	SetItemIndexAct(-1, -1);
	m_EditCtrl.ShowWindow(SW_HIDE);
	m_BarcodeListWnd.SetFocus();
	return true;
}
//-------------------------------------------------------------------------------------//
void CBarcodeListWnd::SetItemIndexAct(int nItem, int nSubItem)
{
	m_nItemAct = nItem;
	m_nSubItemAct = nSubItem;	
}
//-------------------------------------------------------------------------------------//