// BoardListWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "BoardListWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
#define    CMP_COL_INDEX                       0
#define    CMP_COL_PANEL                       1
#define    CMP_COL_BOARD                       2
#define    CMP_COL_TB_SIDE                     3
#define    CMP_COL_BYPASS                      4
#define    CMP_COL_BARCODE                     5
#define    CMP_COL_FD_COUNT                    6
#define    CMP_COL_COMPONENT_COUNT             7
#define    CMP_COL_COMPONENT_BYPASS_COUNT      8
#define    CMP_COL_COMPONENT_XBOARD_UNIT_COUNT 9
//-------------------------------------------------------------------------------------//
int CALLBACK BoardListCompareFn(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
int CALLBACK BoardListCompareFn(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	if ( NULL == lParamSort ) { return 0; }
	CBoardListWnd* pBoardListWnd = (CBoardListWnd*)lParamSort;
	return pBoardListWnd->CompareBoardItem(lParam1, lParam2);
}
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CBoardListWnd dialog
//-------------------------------------------------------------------------------------//
CBoardListWnd::CBoardListWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CBoardListWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBoardListWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nItemAct = -1;
	m_nSubItemAct = -1;
	m_ProjectPtr = NULL;
	m_BoardPtr = NULL;
	m_BoardColID = CMP_COL_INDEX;
	m_BoardListSortMode = SORT_ASCEND;
	m_StopBoardListBeSelected = false;
}
//-------------------------------------------------------------------------------------//
void CBoardListWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBoardListWnd)
	DDX_Control(pDX, BOARDLIST_PARAM_EDIT, m_EditCtrl);	
	DDX_Control(pDX, BOARDLIST_BOARD_LIST_WND, m_BoardListWnd);	
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CBoardListWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CBoardListWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_GETMINMAXINFO()
	ON_NOTIFY(LVN_COLUMNCLICK, BOARDLIST_BOARD_LIST_WND, OnColumnclickBoardListWnd)
	ON_NOTIFY(LVN_ITEMCHANGED, BOARDLIST_BOARD_LIST_WND, OnItemchangedBoardListWnd)
	ON_NOTIFY(NM_DBLCLK, BOARDLIST_BOARD_LIST_WND, OnDblclkBoardListWnd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CBoardListWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CBoardListWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	JetAPI::InitialListCtrl(m_BoardListWnd);	
	BuildBoardListWndHeader();
	SwitchMultiLanguage();

	BuildBoardListWnd();
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	m_BoardListWnd.SetFocus();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CBoardListWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CBoardListWnd::OnSize(UINT nType, int cx, int cy) 
{
	CBaseDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if ( CWnd::GetSafeHwnd() == NULL ) { return; }

	CWnd     *WndPtr=NULL;
	const int MarginW=4;
	const int MarginH=4;

	if ( m_BoardListWnd.GetSafeHwnd() != NULL )
	{
		RECT WndRect={0};
		m_BoardListWnd.GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		WndRect.right = cx-MarginW;
		WndRect.bottom = cy-MarginH;
		m_BoardListWnd.MoveWindow(&WndRect);
	}	
}
//-------------------------------------------------------------------------------------//
void CBoardListWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CBaseDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CBoardListWnd::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default	
	CBaseDialog::OnGetMinMaxInfo(lpMMI);
	lpMMI->ptMinTrackSize.x = 640;
	lpMMI->ptMinTrackSize.y = 320;
}
//-------------------------------------------------------------------------------------//
BOOL CBoardListWnd::PreTranslateMessage(MSG* pMsg) 
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
LRESULT CBoardListWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CBaseDialog::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void CBoardListWnd::SetProjectPtr(CAOIProject *Ptr)
{
	m_ProjectPtr = Ptr;		
	m_BoardPtr = NULL;
	SetBoardListSortMode(1);	
	SetBoardListColID(CMP_COL_INDEX);
}
//-------------------------------------------------------------------------------------//
inline CAOIProject* CBoardListWnd::GetActiveProject()
{
	return m_ProjectPtr;
}
//-------------------------------------------------------------------------------------//
void CBoardListWnd::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_BOARD_LIST_WND");
	//---------------------------------------------------------------------------------//
	WndID = IDD_BOARD_LIST_WND;
	WndKey = _T("IDD_BOARD_LIST_WND");
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
CString CBoardListWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_BOARD_LIST_WND");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
bool CBoardListWnd::BuildBoardListWnd()
{
	CThisListCtrl_47 &ListCtrl = m_BoardListWnd;
	CAOIProject *ProjectPtr = GetActiveProject();
	ClearBoardListWnd();
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
	CAOIBoard     *BoardPtr = NULL;
	const size_t   BoardCount = ProjectPtr->GetProjectBoardCount();

	ListCtrl.SetRedraw(FALSE);
	m_StopBoardListBeSelected = true;
	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr = ProjectPtr->GetProjectBoardPtr(i, false);
		if ( NULL == BoardPtr ) { continue; }
		
		PanelIndex = BoardPtr->GetBoardPanelIndex_Project();
		BoardIndex = BoardPtr->GetBoardIndex_Panel();

		nSubItem = 0;
		str.Format(_T("%d"), i+1);
		ListCtrl.InsertItem(nItem, str);
		ListCtrl.SetItemData(nItem, (DWORD_PTR)m_BoardList.size());
		m_BoardList.push_back(BoardPtr);

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

		//CMP_COL_TB_SIDE
		str = AOIDataDefine.GetBoardSideModeText(BoardPtr->GetBoardSideMode());
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//CMP_COL_BYPASS
		if ( BoardPtr->GetBoardBypassed() == true )
		{	str = _T("Y"); }
		else
		{	str = _T(""); }
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;	

		//CMP_COL_BARCODE
		str = BoardPtr->GetBoardBarcode();
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;	

		//CMP_COL_FD_COUNT
		str.Format(_T("%d"), BoardPtr->GetBoardFdCount());
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;	

		//CMP_COL_COMPONENT_COUNT
		str.Format(_T("%d"), BoardPtr->GetBoardComponentCount());
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//CMP_COL_COMPONENT_BYPASS_COUNT
		str.Format(_T("%d"), BoardPtr->CalcBoardComponentBypassCount());
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;
		
		//CMP_COL_COMPONENT_XBOARD_UNIT_COUNT
		str.Format(_T("%d"), BoardPtr->CalcBoardComponentXBoardUnitCount());
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;		
		//----
		nItem ++;
	}
	m_StopBoardListBeSelected = false;
	ListCtrl.SetRedraw(TRUE);

	SetBoardListColID(CMP_COL_INDEX);	
	SetBoardListSortMode(SORT_ASCEND);	

	CString strCount = AOIDataDefine.GetCountText();
	str.Format(_T("%s:%d"), strCount, ListCtrl.GetItemCount());
	CWnd::SetDlgItemText(BOARDLIST_INFO_EDIT, str);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBoardListWnd::UpdateBoardListWnd()
{
	CThisListCtrl_47 &ListCtrl = m_BoardListWnd;		
	
	size_t         i=0, j=0;
	CString        str;	
	unsigned int   PanelIndex=0;
	unsigned int   BoardIndex=0;	
	int            nItem=0;
	int            nSubItem=0;	
	bool           bAlarm=false;
	double         dCalcTime=0.0;
	RESULT_ID      ResultID;		
	CAOIBoard     *BoardPtr = NULL;
	const int      ItemCount = ListCtrl.GetItemCount();
	const size_t   BoardCount = m_BoardList.size();

	ListCtrl.SetRedraw(FALSE);	
	for ( i=0; i<ItemCount; i++ )
	{
		BoardIndex = ListCtrl.GetItemData(i);
		if ( BoardIndex >= BoardCount ) { continue; }
		BoardPtr = m_BoardList[BoardIndex];
		if ( NULL == BoardPtr ) { continue; }		

		PanelIndex = BoardPtr->GetBoardPanelIndex_Project();
		BoardIndex = BoardPtr->GetBoardIndex_Panel();

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
		
		//CMP_COL_TB_SIDE
		str = AOIDataDefine.GetBoardSideModeText(BoardPtr->GetBoardSideMode());
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//CMP_COL_BYPASS
		if ( BoardPtr->GetBoardBypassed() == true )
		{	str = _T("Y"); }
		else
		{	str = _T(""); }
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;	

		//CMP_COL_BARCODE
		str = BoardPtr->GetBoardBarcode();
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;	
		
		//CMP_COL_FD_COUNT
		str.Format(_T("%d"), BoardPtr->GetBoardFdCount());
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;	

		//CMP_COL_COMPONENT_COUNT
		str.Format(_T("%d"), BoardPtr->GetBoardComponentCount());
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//CMP_COL_COMPONENT_BYPASS_COUNT		
		str.Format(_T("%d"), BoardPtr->CalcBoardComponentBypassCount());
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//CMP_COL_COMPONENT_XBOARD_UNIT_COUNT
		str.Format(_T("%d"), BoardPtr->CalcBoardComponentXBoardUnitCount());
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;
		//----
		nItem ++;
	}	
	ListCtrl.SetRedraw(TRUE);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBoardListWnd::ClearBoardListWnd()
{
	CThisListCtrl_47 &ListCtrl = m_BoardListWnd;
	m_StopBoardListBeSelected = true;
	SetItemIndexAct(-1, -1);
	m_BoardList.clear();
	JetAPI::ClearListCtrl(ListCtrl, FALSE);	
	m_StopBoardListBeSelected = false;
	CWnd::SetDlgItemText(BOARDLIST_INFO_EDIT, _T(""));	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBoardListWnd::BuildBoardListWndHeader()
{
	CString str;
	int   nCol = 0;
	int width  = 64;
	int width2 = 64;
	RECT      Rect;	
	const int nCols = 14;
	const int Align = LVCFMT_LEFT;//LVCFMT_CENTER;LVCFMT_RIGHT, LVCFMT_LEFT

	{
		CThisListCtrl_47 &ListCtrl = m_BoardListWnd;		

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
		
		str = _T("Board");
		str = AOIDataDefine.GetBoardText();
		ListCtrl.InsertColumn(nCol, str, Align, width);
		nCol ++;				

		str = _T("TB Mode");
		str = LoadMultiLanguageString(str, str);
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

		str = _T("Component");
		str = AOIDataDefine.GetComponentText();
		ListCtrl.InsertColumn(nCol, str, Align, width);
		nCol ++;	

		str = _T("Bypass-Part");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width*2);
		nCol ++;

		str = _T("XBoard-Part");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width*2);
		nCol ++;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CBoardListWnd::SetBoardListColID(int val)
{
	m_BoardColID = val;
}
//-------------------------------------------------------------------------------------//
int CBoardListWnd::GetBoardListColD() const
{
	return m_BoardColID;
}
//-------------------------------------------------------------------------------------//
void CBoardListWnd::SetBoardListSortMode(int val)
{
	m_BoardListSortMode = val;
}
//-------------------------------------------------------------------------------------//
int CBoardListWnd::GetBoardListSortMode() const
{
	return m_BoardListSortMode;
}
//-------------------------------------------------------------------------------------//
int CBoardListWnd::CompareBoardItem(size_t index1, size_t index2)
{
	CThisListCtrl_47 &ListCtrl = m_BoardListWnd;
	const int ColID = GetBoardListColD();
	const size_t BoardCount = m_BoardList.size();
	if ( index1>=BoardCount || index2>=BoardCount )
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
	CAOIBoard   *BPtr1=(m_BoardList[index1]);
	CAOIBoard   *BPtr2=(m_BoardList[index2]);	
	if ( NULL==BPtr1 || NULL==BPtr2 ) { return 0; }

	switch ( ColID )
	{	
	case CMP_COL_INDEX:
		if ( index1 > index2 ) { Res = 1; }
		else if ( index1 < index2 ) { Res = -1; }
		else {	Res=0; }
		break;
	case CMP_COL_PANEL:
		uVal1 = BPtr1->GetBoardPanelIndex_Project();
		uVal2 = BPtr2->GetBoardPanelIndex_Project();
		if ( uVal1 > uVal2 ) { Res = 1; }
		else if ( uVal1 < uVal2 ) { Res = -1; }
		else {	Res=0; }
		break;
	case CMP_COL_BOARD:
		uVal1 = BPtr1->GetBoardIndex_Panel();
		uVal2 = BPtr2->GetBoardIndex_Panel();
		if ( uVal1 > uVal2 ) { Res = 1; }
		else if ( uVal1 < uVal2 ) { Res = -1; }
		else {	Res=0; }
		break;
	case CMP_COL_TB_SIDE:
		uVal1 = BPtr1->GetBoardSideMode();
		uVal2 = BPtr2->GetBoardSideMode();
		if ( uVal1 > uVal2 ) { Res = 1; }
		else if ( uVal1 < uVal2 ) { Res = -1; }
		else {	Res=0; }
		break;
	case CMP_COL_BYPASS:
		uVal1 = BPtr1->GetBoardBypassed();
		uVal2 = BPtr2->GetBoardBypassed();
		if ( uVal1 > uVal2 ) { Res = 1; }
		else if ( uVal1 < uVal2 ) { Res = -1; }
		else {	Res=0; }
		break;
	case CMP_COL_BARCODE:
		wsPtr1= (wchar_t*)BPtr1->GetBoardBarcode();
		wsPtr2= (wchar_t*)BPtr2->GetBoardBarcode();
		Res = ::wcscmp(wsPtr1, wsPtr2);		
		break;
	case CMP_COL_FD_COUNT:
		uVal1 = BPtr1->GetBoardFdCount();
		uVal2 = BPtr2->GetBoardFdCount();
		if ( uVal1 > uVal2 ) { Res = 1; }
		else if ( uVal1 < uVal2 ) { Res = -1; }
		else {	Res=0; }
		break;
	case CMP_COL_COMPONENT_COUNT:
		uVal1 = BPtr1->GetBoardComponentCount();
		uVal2 = BPtr2->GetBoardComponentCount();
		if ( uVal1 > uVal2 ) { Res = 1; }
		else if ( uVal1 < uVal2 ) { Res = -1; }
		else {	Res=0; }
		break;
	case CMP_COL_COMPONENT_BYPASS_COUNT:
		uVal1 = BPtr1->CalcBoardComponentBypassCount();
		uVal2 = BPtr2->CalcBoardComponentBypassCount();
		if ( uVal1 > uVal2 ) { Res = 1; }
		else if ( uVal1 < uVal2 ) { Res = -1; }
		else {	Res=0; }
		break;
	case CMP_COL_COMPONENT_XBOARD_UNIT_COUNT:
		uVal1 = BPtr1->CalcBoardComponentXBoardUnitCount();
		uVal2 = BPtr2->CalcBoardComponentXBoardUnitCount();
		if ( uVal1 > uVal2 ) { Res = 1; }
		else if ( uVal1 < uVal2 ) { Res = -1; }
		else {	Res=0; }
		break;
	default:
		Res = 0;
		break;
	}
	int SortMode = GetBoardListSortMode();
	Res = Res*SortMode;	
	return Res;	
}
//-------------------------------------------------------------------------------------//
void CBoardListWnd::OnColumnclickBoardListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	int ColID = GetBoardListColD();
	int SortMode = GetBoardListSortMode();	
	const int ColumnsIdx = pNMListView->iSubItem;
	if ( ColID != ColumnsIdx )
	{	SortMode = SORT_ASCEND; }
	else
	{
		if ( SORT_ASCEND == SortMode ) { SortMode = SORT_DESCEND; }
		else {	SortMode = SORT_ASCEND;  }
	}
	SetBoardListColID(ColumnsIdx);	
	SetBoardListSortMode(SortMode);	
	m_BoardListWnd.SortItems(BoardListCompareFn, (DWORD_PTR)this);
	
	const int nItem = m_BoardListWnd.GetNextItem(-1, LVNI_FOCUSED);
	if ( nItem >= 0 ) 
	{	m_BoardListWnd.EnsureVisible(nItem, FALSE); }
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CBoardListWnd::OnItemchangedBoardListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if ( true == m_StopBoardListBeSelected ) { return; }
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) 
	{	return;	}
	DWORD Res = pNMListView->uNewState&LVIS_SELECTED;
	if ( Res == 0 ) { return; }	
	Res = pNMListView->uNewState&LVIS_FOCUSED;
	if ( Res == 0 ) { return; }	
	const size_t SelIndex = m_BoardListWnd.GetItemData(nItem);
	const size_t FdCount = m_BoardList.size();
	if ( SelIndex > FdCount ) { return; }
	CAOIBoard *BoardPtr = m_BoardList[SelIndex];
	if ( NULL == BoardPtr ) { return; }	
	AOIDataCollect.MoveStageToBoard(BoardPtr, false);		
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CBoardListWnd::OnDblclkBoardListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NMITEMACTIVATE *pNMListView = (NMITEMACTIVATE*)pNMHDR;	
	m_BoardPtr = NULL;
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	SetItemIndexAct(-1, -1);
	if ( nItem < 0 ) { return; }
	CThisListCtrl_47 &ListCtrl=m_BoardListWnd;
	const size_t SelIndex = ListCtrl.GetItemData(nItem);
	const size_t FdCount = m_BoardList.size();
	if ( SelIndex > FdCount ) { return; }
	CAOIBoard *BoardPtr = m_BoardList[SelIndex];
	if ( NULL == BoardPtr ) { return; }	
	AOIDataCollect.MoveStageToBoard(BoardPtr, true);

	CString str;
	CRect   ItemRect={0};
	bool    bShowEdit=false;
	SetBoardListColID(nSubItem);	
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
		m_BoardPtr = BoardPtr;
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
bool CBoardListWnd::ExecUpdateParamByEdit()
{
	if ( m_EditCtrl.GetSafeHwnd() == NULL ) { return false; }

	m_EditCtrl.ShowWindow(SW_HIDE);
	if ( NULL == m_BoardPtr ) { return true; }
	const int nItem = m_nItemAct;
	const int nSubItem = m_nSubItemAct;
	if ( -1==nItem || -1==nSubItem ) { return true; }

	CString str;
	CString ItemText;
	int        nValue=0;
	bool       bUpdateAll=false;
	const int  nColID = GetBoardListColD();
	CAOIBoard *BoardPtr=m_BoardPtr;	

	m_EditCtrl.GetWindowText(ItemText);
	ItemText.MakeUpper();		
	if ( true == bUpdateAll ) 
	{	UpdateBoardListWnd();	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBoardListWnd::ExecReleaseParamCtrl()
{
	m_BoardPtr = NULL;
	SetItemIndexAct(-1, -1);
	m_EditCtrl.ShowWindow(SW_HIDE);
	m_BoardListWnd.SetFocus();
	return true;
}
//-------------------------------------------------------------------------------------//
void CBoardListWnd::SetItemIndexAct(int nItem, int nSubItem)
{
	m_nItemAct = nItem;
	m_nSubItemAct = nSubItem;	
}
//-------------------------------------------------------------------------------------//