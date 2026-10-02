// PanelListWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "PanelListWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
#define    CMP_COL_INDEX                0
#define    CMP_COL_PANEL                1
#define    CMP_COL_BYPASS               2
#define    CMP_COL_BARCODE              3
#define    CMP_COL_FD_COUNT             4
#define    CMP_COL_BOARD_COUNT          5
#define    CMP_COL_COMPONENT_COUNT      6
//-------------------------------------------------------------------------------------//
int CALLBACK PanelListCompareFn(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
int CALLBACK PanelListCompareFn(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	if ( NULL == lParamSort ) { return 0; }
	CPanelListWnd* pPanelListWnd = (CPanelListWnd*)lParamSort;
	return pPanelListWnd->ComparePanelItem(lParam1, lParam2);
}
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CPanelListWnd dialog
//-------------------------------------------------------------------------------------//
CPanelListWnd::CPanelListWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CPanelListWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPanelListWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nItemAct = -1;
	m_nSubItemAct = -1;
	m_ProjectPtr = NULL;
	m_PanelPtr = NULL;
	m_PanelColID = CMP_COL_INDEX;
	m_PanelListSortMode = SORT_ASCEND;
	m_StopPanelListBeSelected = false;
}
//-------------------------------------------------------------------------------------//
void CPanelListWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPanelListWnd)
	DDX_Control(pDX, PANELLIST_PARAM_EDIT, m_EditCtrl);	
	DDX_Control(pDX, PANELLIST_PANEL_LIST_WND, m_PanelListWnd);	
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CPanelListWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CPanelListWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_GETMINMAXINFO()
	ON_NOTIFY(LVN_COLUMNCLICK, PANELLIST_PANEL_LIST_WND, OnColumnclickPanelListWnd)
	ON_NOTIFY(LVN_ITEMCHANGED, PANELLIST_PANEL_LIST_WND, OnItemchangedPanelListWnd)
	ON_NOTIFY(NM_DBLCLK, PANELLIST_PANEL_LIST_WND, OnDblclkPanelListWnd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CPanelListWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CPanelListWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	JetAPI::InitialListCtrl(m_PanelListWnd);	
	BuildPanelListWndHeader();
	SwitchMultiLanguage();

	BuildPanelListWnd();
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	m_PanelListWnd.SetFocus();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CPanelListWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CPanelListWnd::OnSize(UINT nType, int cx, int cy) 
{
	CBaseDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if ( CWnd::GetSafeHwnd() == NULL ) { return; }

	CWnd     *WndPtr=NULL;
	const int MarginW=4;
	const int MarginH=4;

	if ( m_PanelListWnd.GetSafeHwnd() != NULL )
	{
		RECT WndRect={0};
		m_PanelListWnd.GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		WndRect.right = cx-MarginW;
		WndRect.bottom = cy-MarginH;
		m_PanelListWnd.MoveWindow(&WndRect);
	}
}
//-------------------------------------------------------------------------------------//
void CPanelListWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CBaseDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CPanelListWnd::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default	
	CBaseDialog::OnGetMinMaxInfo(lpMMI);
	lpMMI->ptMinTrackSize.x = 640;
	lpMMI->ptMinTrackSize.y = 320;
}
//-------------------------------------------------------------------------------------//
BOOL CPanelListWnd::PreTranslateMessage(MSG* pMsg) 
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
LRESULT CPanelListWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CBaseDialog::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void CPanelListWnd::SetProjectPtr(CAOIProject *Ptr)
{
	m_ProjectPtr = Ptr;		
	m_PanelPtr = NULL;
	SetPanelListSortMode(1);	
	SetPanelListColID(CMP_COL_INDEX);
}
//-------------------------------------------------------------------------------------//
inline CAOIProject* CPanelListWnd::GetActiveProject()
{
	return m_ProjectPtr;
}
//-------------------------------------------------------------------------------------//
void CPanelListWnd::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_PANEL_LIST_WND");
	//---------------------------------------------------------------------------------//
	WndID = IDD_PANEL_LIST_WND;
	WndKey = _T("IDD_PANEL_LIST_WND");
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
CString CPanelListWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_PANEL_LIST_WND");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
bool CPanelListWnd::BuildPanelListWnd()
{
	CThisListCtrl_48 &ListCtrl = m_PanelListWnd;
	CAOIProject *ProjectPtr = GetActiveProject();
	ClearPanelListWnd();
	if ( NULL == ProjectPtr )
	{	return true; }
	
	size_t         i=0, j=0;
	CString        str;	
	unsigned int   PanelIndex=0;	
	int            nItem=0;
	int            nSubItem=0;	
	bool           bAlarm=false;
	double         dCalcTime=0.0;
	RESULT_ID      ResultID;
	CAOIPanel     *PanelPtr = NULL;
	const size_t   PanelCount = ProjectPtr->GetProjectPanelCount();

	ListCtrl.SetRedraw(FALSE);
	m_StopPanelListBeSelected = true;
	for ( i=0; i<PanelCount; i++ )
	{
		PanelPtr = ProjectPtr->GetProjectPanelPtr(i, false);
		if ( NULL == PanelPtr ) { continue; }
		
		PanelIndex = PanelPtr->GetPanelIndex_Project();		

		nSubItem = 0;
		str.Format(_T("%d"), i+1);
		ListCtrl.InsertItem(nItem, str);
		ListCtrl.SetItemData(nItem, (DWORD_PTR)m_PanelList.size());
		m_PanelList.push_back(PanelPtr);

		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;
		
		//CMP_COL_PANEL
		str.Format(_T("%d"), PanelIndex+1);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//CMP_COL_BYPASS
		if ( PanelPtr->GetPanelBypassed() == true )
		{	str = _T("Y"); }
		else
		{	str = _T(""); }
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;	

		//CMP_COL_BARCODE
		str = PanelPtr->GetPanelBarcode();
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;	

		//CMP_COL_FD_COUNT
		str.Format(_T("%d"), PanelPtr->GetPanelFdCount());
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;	

		//CMP_COL_BOARD_COUNT
		str.Format(_T("%d"), PanelPtr->GetPanelBoardCount());
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;	

		//CMP_COL_COMPONENT_COUNT
		str.Format(_T("%d"), PanelPtr->GetPanelComponentCount());
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;	
		//----
		nItem ++;
	}
	m_StopPanelListBeSelected = false;
	ListCtrl.SetRedraw(TRUE);

	SetPanelListColID(CMP_COL_INDEX);	
	SetPanelListSortMode(SORT_ASCEND);	

	CString strCount = AOIDataDefine.GetCountText();
	str.Format(_T("%s:%d"), strCount, ListCtrl.GetItemCount());
	CWnd::SetDlgItemText(PANELLIST_INFO_EDIT, str);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CPanelListWnd::UpdatePanelListWnd()
{
	CThisListCtrl_48 &ListCtrl = m_PanelListWnd;		
	
	size_t         i=0, j=0;
	CString        str;	
	unsigned int   PanelIndex=0;		
	int            nItem=0;
	int            nSubItem=0;	
	bool           bAlarm=false;
	double         dCalcTime=0.0;
	RESULT_ID      ResultID;		
	CAOIPanel     *PanelPtr = NULL;
	const int      ItemCount = ListCtrl.GetItemCount();
	const size_t   PanelCount = m_PanelList.size();

	ListCtrl.SetRedraw(FALSE);	
	for ( i=0; i<ItemCount; i++ )
	{
		PanelIndex = ListCtrl.GetItemData(i);
		if ( PanelIndex >= PanelCount ) { continue; }
		PanelPtr = m_PanelList[PanelIndex];
		if ( NULL == PanelPtr ) { continue; }		

		PanelIndex = PanelPtr->GetPanelIndex_Project();		

		nSubItem = 0;
		//str.Format(_T("%d"), i+1);		
		//ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;
		
		//CMP_COL_PANEL
		str.Format(_T("%d"), PanelIndex+1);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//CMP_COL_BYPASS
		if ( PanelPtr->GetPanelBypassed() == true )
		{	str = _T("Y"); }
		else
		{	str = _T(""); }
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//CMP_COL_BARCODE
		str = PanelPtr->GetPanelBarcode();
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;	

		//CMP_COL_FD_COUNT
		str.Format(_T("%d"), PanelPtr->GetPanelFdCount());
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;	

		//CMP_COL_BOARD_COUNT
		str.Format(_T("%d"), PanelPtr->GetPanelBoardCount());
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;	

		//CMP_COL_COMPONENT_COUNT
		str.Format(_T("%d"), PanelPtr->GetPanelComponentCount());
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;
		//----
		nItem ++;
	}	
	ListCtrl.SetRedraw(TRUE);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CPanelListWnd::ClearPanelListWnd()
{
	CThisListCtrl_48 &ListCtrl = m_PanelListWnd;
	m_StopPanelListBeSelected = true;
	SetItemIndexAct(-1, -1);
	m_PanelList.clear();
	JetAPI::ClearListCtrl(ListCtrl, FALSE);	
	m_StopPanelListBeSelected = false;
	CWnd::SetDlgItemText(PANELLIST_INFO_EDIT, _T(""));	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CPanelListWnd::BuildPanelListWndHeader()
{
	CString str;
	int   nCol = 0;
	int width  = 64;
	int width2 = 64;
	RECT      Rect;	
	const int nCols = 10;
	const int Align = LVCFMT_LEFT;//LVCFMT_CENTER;LVCFMT_RIGHT, LVCFMT_LEFT

	{
		CThisListCtrl_48 &ListCtrl = m_PanelListWnd;		

		ListCtrl.GetClientRect(&Rect);
		width = 48;
		width2 = (Rect.right-Rect.left-width-64)/nCols;

		str = _T("Index");
		str = AOIDataDefine.GetIndexText();		
		ListCtrl.InsertColumn(nCol, str, Align, width);		
		
		nCol ++;
		
		str = _T("Panel");
		str = AOIDataDefine.GetPanelText();
		ListCtrl.InsertColumn(nCol, str, Align, width);
		nCol ++;		

		str = _T("Bypass");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width);
		nCol ++;	

		str = _T("Barcode");
		str = AOIDataDefine.GetBarcodeText();
		ListCtrl.InsertColumn(nCol, str, Align, width*4);
		nCol ++;	

		str = _T("Fd");
		str = AOIDataDefine.GetFdText();
		ListCtrl.InsertColumn(nCol, str, Align, width);
		nCol ++;	

		str = _T("Board");
		str = AOIDataDefine.GetBoardText();
		ListCtrl.InsertColumn(nCol, str, Align, width);
		nCol ++;				

		str = _T("Component");
		str = AOIDataDefine.GetComponentText();
		ListCtrl.InsertColumn(nCol, str, Align, width);
		nCol ++;	
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CPanelListWnd::SetPanelListColID(int val)
{
	m_PanelColID = val;
}
//-------------------------------------------------------------------------------------//
int CPanelListWnd::GetPanelListColD() const
{
	return m_PanelColID;
}
//-------------------------------------------------------------------------------------//
void CPanelListWnd::SetPanelListSortMode(int val)
{
	m_PanelListSortMode = val;
}
//-------------------------------------------------------------------------------------//
int CPanelListWnd::GetPanelListSortMode() const
{
	return m_PanelListSortMode;
}
//-------------------------------------------------------------------------------------//
int CPanelListWnd::ComparePanelItem(size_t index1, size_t index2)
{
	CThisListCtrl_48 &ListCtrl = m_PanelListWnd;
	const int ColID = GetPanelListColD();
	const size_t PanelCount = m_PanelList.size();
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
	CAOIPanel   *PPtr1=(m_PanelList[index1]);
	CAOIPanel   *PPtr2=(m_PanelList[index2]);	
	if ( NULL==PPtr1 || NULL==PPtr2 ) { return 0; }

	switch ( ColID )
	{	
	case CMP_COL_INDEX:
		if ( index1 > index2 ) { Res = 1; }
		else if ( index1 < index2 ) { Res = -1; }
		else {	Res=0; }
		break;
	case CMP_COL_PANEL:
		uVal1 = PPtr1->GetPanelIndex_Project();
		uVal2 = PPtr2->GetPanelIndex_Project();
		if ( uVal1 > uVal2 ) { Res = 1; }
		else if ( uVal1 < uVal2 ) { Res = -1; }
		else {	Res=0; }
		break;	
	case CMP_COL_BYPASS:
		uVal1 = PPtr1->GetPanelBypassed();
		uVal2 = PPtr2->GetPanelBypassed();
		if ( uVal1 > uVal2 ) { Res = 1; }
		else if ( uVal1 < uVal2 ) { Res = -1; }
		else {	Res=0; }
		break;
	case CMP_COL_BARCODE:
		wsPtr1= (wchar_t*)PPtr1->GetPanelBarcode();
		wsPtr2= (wchar_t*)PPtr2->GetPanelBarcode();
		Res = ::wcscmp(wsPtr1, wsPtr2);		
		break;
	case CMP_COL_FD_COUNT:
		uVal1 = PPtr1->GetPanelFdCount();
		uVal2 = PPtr2->GetPanelFdCount();
		if ( uVal1 > uVal2 ) { Res = 1; }
		else if ( uVal1 < uVal2 ) { Res = -1; }
		else {	Res=0; }
		break;
	case CMP_COL_BOARD_COUNT:
		uVal1 = PPtr1->GetPanelBoardCount();
		uVal2 = PPtr2->GetPanelBoardCount();
		if ( uVal1 > uVal2 ) { Res = 1; }
		else if ( uVal1 < uVal2 ) { Res = -1; }
		else {	Res=0; }
		break;
	case CMP_COL_COMPONENT_COUNT:
		uVal1 = PPtr1->GetPanelComponentCount();
		uVal2 = PPtr2->GetPanelComponentCount();
		if ( uVal1 > uVal2 ) { Res = 1; }
		else if ( uVal1 < uVal2 ) { Res = -1; }
		else {	Res=0; }
		break;	
	default:
		Res = 0;
		break;
	}
	int SortMode = GetPanelListSortMode();
	Res = Res*SortMode;	
	return Res;	
}
//-------------------------------------------------------------------------------------//
void CPanelListWnd::OnColumnclickPanelListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	int ColID = GetPanelListColD();
	int SortMode = GetPanelListSortMode();	
	const int ColumnsIdx = pNMListView->iSubItem;
	if ( ColID != ColumnsIdx )
	{	SortMode = SORT_ASCEND; }
	else
	{
		if ( SORT_ASCEND == SortMode ) { SortMode = SORT_DESCEND; }
		else {	SortMode = SORT_ASCEND;  }
	}
	SetPanelListColID(ColumnsIdx);	
	SetPanelListSortMode(SortMode);	
	m_PanelListWnd.SortItems(PanelListCompareFn, (DWORD_PTR)this);
	
	const int nItem = m_PanelListWnd.GetNextItem(-1, LVNI_FOCUSED);
	if ( nItem >= 0 ) 
	{	m_PanelListWnd.EnsureVisible(nItem, FALSE); }
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CPanelListWnd::OnItemchangedPanelListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if ( true == m_StopPanelListBeSelected ) { return; }
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) 
	{	return;	}
	DWORD Res = pNMListView->uNewState&LVIS_SELECTED;
	if ( Res == 0 ) { return; }	
	Res = pNMListView->uNewState&LVIS_FOCUSED;
	if ( Res == 0 ) { return; }	
	const size_t SelIndex = m_PanelListWnd.GetItemData(nItem);
	const size_t FdCount = m_PanelList.size();
	if ( SelIndex > FdCount ) { return; }
	CAOIPanel *PanelPtr = m_PanelList[SelIndex];
	if ( NULL == PanelPtr ) { return; }	
	AOIDataCollect.MoveStageToPanel(PanelPtr, false);		
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CPanelListWnd::OnDblclkPanelListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NMITEMACTIVATE *pNMListView = (NMITEMACTIVATE*)pNMHDR;	
	m_PanelPtr = NULL;
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	SetItemIndexAct(-1, -1);
	if ( nItem < 0 ) { return; }
	CThisListCtrl_48 &ListCtrl=m_PanelListWnd;
	const size_t SelIndex = ListCtrl.GetItemData(nItem);
	const size_t FdCount = m_PanelList.size();
	if ( SelIndex > FdCount ) { return; }
	CAOIPanel *PanelPtr = m_PanelList[SelIndex];
	if ( NULL == PanelPtr ) { return; }	
	AOIDataCollect.MoveStageToPanel(PanelPtr, true);

	CString str;
	CRect   ItemRect={0};
	bool    bShowEdit=false;
	SetPanelListColID(nSubItem);	
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
		m_PanelPtr = PanelPtr;
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
bool CPanelListWnd::ExecUpdateParamByEdit()
{
	if ( m_EditCtrl.GetSafeHwnd() == NULL ) { return false; }

	m_EditCtrl.ShowWindow(SW_HIDE);
	if ( NULL == m_PanelPtr ) { return true; }
	const int nItem = m_nItemAct;
	const int nSubItem = m_nSubItemAct;
	if ( -1==nItem || -1==nSubItem ) { return true; }

	CString str;
	CString ItemText;
	int        nValue=0;
	bool       bUpdateAll=false;
	const int  nColID = GetPanelListColD();
	CAOIPanel *PanelPtr=m_PanelPtr;	

	m_EditCtrl.GetWindowText(ItemText);
	ItemText.MakeUpper();		
	if ( true == bUpdateAll ) 
	{	UpdatePanelListWnd();	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CPanelListWnd::ExecReleaseParamCtrl()
{
	m_PanelPtr = NULL;
	SetItemIndexAct(-1, -1);
	m_EditCtrl.ShowWindow(SW_HIDE);
	m_PanelListWnd.SetFocus();
	return true;
}
//-------------------------------------------------------------------------------------//
void CPanelListWnd::SetItemIndexAct(int nItem, int nSubItem)
{
	m_nItemAct = nItem;
	m_nSubItemAct = nSubItem;	
}
//-------------------------------------------------------------------------------------//