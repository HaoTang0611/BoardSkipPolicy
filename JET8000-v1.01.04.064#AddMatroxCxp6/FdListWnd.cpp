// FdListWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "FdListWnd.h"
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
#define    CMP_COL_FD_NAME              3
#define    CMP_COL_FD_SORT              4
//-------------------------------------------------------------------------------------//
int CALLBACK FdListCompareFn(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
int CALLBACK FdListCompareFn(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	if ( NULL == lParamSort ) { return 0; }
	CFdListWnd* pFdListWnd = (CFdListWnd*)lParamSort;
	return pFdListWnd->CompareFdItem(lParam1, lParam2);
}
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CFdListWnd dialog
//-------------------------------------------------------------------------------------//
CFdListWnd::CFdListWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CFdListWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFdListWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nItemAct = -1;
	m_nSubItemAct = -1;
	m_ProjectPtr = NULL;
	m_FdPtr = NULL;
	m_FdColID = CMP_COL_INDEX;
	m_FdListSortMode = SORT_ASCEND;
	m_StopFdListBeSelected = false;
}
//-------------------------------------------------------------------------------------//
void CFdListWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFdListWnd)
	DDX_Control(pDX, FDLIST_PARAM_EDIT, m_EditCtrl);	
	DDX_Control(pDX, FDLIST_FD_LIST_WND, m_FdListWnd);	
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CFdListWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CFdListWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_GETMINMAXINFO()
	ON_NOTIFY(LVN_COLUMNCLICK, FDLIST_FD_LIST_WND, OnColumnclickFdListWnd)
	ON_NOTIFY(LVN_ITEMCHANGED, FDLIST_FD_LIST_WND, OnItemchangedFdListWnd)
	ON_NOTIFY(NM_DBLCLK, FDLIST_FD_LIST_WND, OnDblclkFdListWnd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CFdListWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CFdListWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	JetAPI::InitialListCtrl(m_FdListWnd);	
	BuildFdListWndHeader();
	SwitchMultiLanguage();

	BuildFdListWnd();
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	m_FdListWnd.SetFocus();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CFdListWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CFdListWnd::OnSize(UINT nType, int cx, int cy) 
{
	CBaseDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if ( CWnd::GetSafeHwnd() == NULL ) { return; }

	CWnd     *WndPtr=NULL;
	const int MarginW=4;
	const int MarginH=4;

	if ( m_FdListWnd.GetSafeHwnd() != NULL )
	{
		RECT WndRect={0};
		m_FdListWnd.GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		WndRect.right = cx-MarginW;
		WndRect.bottom = cy-MarginH;
		m_FdListWnd.MoveWindow(&WndRect);
	}	
}
//-------------------------------------------------------------------------------------//
void CFdListWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CBaseDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
BOOL CFdListWnd::PreTranslateMessage(MSG* pMsg) 
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
LRESULT CFdListWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CBaseDialog::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void CFdListWnd::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default	
	CBaseDialog::OnGetMinMaxInfo(lpMMI);
	lpMMI->ptMinTrackSize.x = 640;
	lpMMI->ptMinTrackSize.y = 320;
}
//-------------------------------------------------------------------------------------//
void CFdListWnd::SetProjectPtr(CAOIProject *Ptr)
{
	m_ProjectPtr = Ptr;		
	m_FdPtr = NULL;
	SetFdListSortMode(1);	
	SetFdListColID(CMP_COL_INDEX);
}
//-------------------------------------------------------------------------------------//
inline CAOIProject* CFdListWnd::GetActiveProject()
{
	return m_ProjectPtr;
}
//-------------------------------------------------------------------------------------//
void CFdListWnd::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_FD_LIST_WND");
	//---------------------------------------------------------------------------------//
	WndID = IDD_FD_LIST_WND;
	WndKey = _T("IDD_FD_LIST_WND");
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
CString CFdListWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_FD_LIST_WND");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
bool CFdListWnd::BuildFdListWnd()
{
	CThisListCtrl_46 &ListCtrl = m_FdListWnd;
	CAOIProject *ProjectPtr = GetActiveProject();
	ClearFdListWnd();
	if ( NULL == ProjectPtr )
	{	return true; }
	
	size_t         i=0, j=0;
	CString        str;
	unsigned int   PanelIndex=0;
	unsigned int   BoardIndex=0;
	unsigned int   FdIndex=0;
	int            nItem=0;
	int            nSubItem=0;	
	bool           bAlarm=false;
	double         dCalcTime=0.0;
	RESULT_ID      ResultID;
	DISTRICT_ID    DistrictID;
	MODEL_TYPE     ModelType;
	//CAOIModel     *ModelPtr = NULL;
	CAOIFd        *FdPtr = NULL;
	const size_t   FdCount = ProjectPtr->GetProjectFdCount();

	ListCtrl.SetRedraw(FALSE);
	m_StopFdListBeSelected = true;
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = ProjectPtr->GetProjectFdPtr(i, false);
		if ( NULL == FdPtr ) { continue; }		

		FdIndex = FdPtr->GetFdIndex_Panel();
		PanelIndex = FdPtr->GetFdPanelIndex_Project();
		BoardIndex = FdPtr->GetFdBoardIndex_Panel();

		nSubItem = 0;
		str.Format(_T("%d"), i+1);
		ListCtrl.InsertItem(nItem, str);
		ListCtrl.SetItemData(nItem, (DWORD_PTR)m_FdList.size());
		m_FdList.push_back(FdPtr);

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

		//CMP_COL_FD_NAME
		str.Format(_T("Fd-%04d"), FdIndex+1);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//CMP_COL_FD_SORT
		str.Format(_T("%d"), FdPtr->GetFdSortID());
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//----
		nItem ++;
	}
	m_StopFdListBeSelected = false;
	ListCtrl.SetRedraw(TRUE);

	SetFdListColID(CMP_COL_INDEX);	
	SetFdListSortMode(SORT_ASCEND);

	CString strCount = AOIDataDefine.GetCountText();
	str.Format(_T("%s:%d"), strCount, ListCtrl.GetItemCount());
	CWnd::SetDlgItemText(FDLIST_INFO_EDIT, str);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CFdListWnd::UpdateFdListWnd()
{
	CThisListCtrl_46 &ListCtrl = m_FdListWnd;		
	
	size_t         i=0, j=0;
	CString        str;
	unsigned int   FdIndex=0;
	unsigned int   PanelIndex=0;
	unsigned int   BoardIndex=0;	
	int            nItem=0;
	int            nSubItem=0;	
	bool           bAlarm=false;
	double         dCalcTime=0.0;
	RESULT_ID      ResultID;	
	//CAOIModel     *ModelPtr = NULL;
	CAOIFd        *FdPtr = NULL;
	const int      ItemCount = ListCtrl.GetItemCount();
	const size_t   FdCount = m_FdList.size();

	ListCtrl.SetRedraw(FALSE);	
	for ( i=0; i<ItemCount; i++ )
	{
		FdIndex = ListCtrl.GetItemData(i);
		if ( FdIndex >= FdCount ) { continue; }
		FdPtr = m_FdList[FdIndex];
		if ( NULL == FdPtr ) { continue; }		

		PanelIndex = FdPtr->GetFdPanelIndex_Project();
		BoardIndex = FdPtr->GetFdBoardIndex_Panel();

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

		//CMP_COL_FD_NAME
		str.Format(_T("Fd-%04d"), FdIndex+1);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//CMP_COL_FD_SORT
		str.Format(_T("%d"), FdPtr->GetFdSortID());
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;
		//----
		nItem ++;
	}	
	ListCtrl.SetRedraw(TRUE);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CFdListWnd::ClearFdListWnd()
{
	CThisListCtrl_46 &ListCtrl = m_FdListWnd;
	m_StopFdListBeSelected = true;
	SetItemIndexAct(-1, -1);
	m_FdList.clear();
	JetAPI::ClearListCtrl(ListCtrl, FALSE);	
	m_StopFdListBeSelected = false;
	CWnd::SetDlgItemText(FDLIST_INFO_EDIT, _T(""));	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CFdListWnd::BuildFdListWndHeader()
{
	CString str;
	int   nCol = 0;
	int width  = 64;
	int width2 = 64;
	RECT      Rect;	
	const int nCols = 6;
	const int Align = LVCFMT_LEFT;//LVCFMT_CENTER;LVCFMT_RIGHT, LVCFMT_LEFT

	{
		CThisListCtrl_46 &ListCtrl = m_FdListWnd;		

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
		
		str = _T("Fd");
		str = AOIDataDefine.GetFdText();
		ListCtrl.InsertColumn(nCol, str, Align, width2);
		nCol ++;		
		
		str = _T("Sort");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width2);
		nCol ++;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CFdListWnd::SetFdListColID(int val)
{
	m_FdColID = val;
}
//-------------------------------------------------------------------------------------//
int CFdListWnd::GetFdListColD() const
{
	return m_FdColID;
}
//-------------------------------------------------------------------------------------//
void CFdListWnd::SetFdListSortMode(int val)
{
	m_FdListSortMode = val;
}
//-------------------------------------------------------------------------------------//
int CFdListWnd::GetFdListSortMode() const
{
	return m_FdListSortMode;
}
//-------------------------------------------------------------------------------------//
int CFdListWnd::CompareFdItem(size_t index1, size_t index2)
{
	CThisListCtrl_46 &ListCtrl = m_FdListWnd;
	const int ColID = GetFdListColD();
	const size_t FdCount = m_FdList.size();
	if ( index1>=FdCount || index2>=FdCount )
	{	return 0; }
	int          Res=0;	
	wchar_t     *wsPtr1=NULL;
	wchar_t     *wsPtr2=NULL;
	double       dVal1=0.0;
	double       dVal2=0.0;
	unsigned int uVal1=0;
	unsigned int uVal2=0;	
	CAOIFd      *FPtr1=(m_FdList[index1]);
	CAOIFd      *FPtr2=(m_FdList[index2]);
	CAOIModel   *MPtr1=FPtr1->GetFdModelPtr();
	CAOIModel   *MPtr2=FPtr2->GetFdModelPtr();
	if ( NULL==MPtr1 || NULL==MPtr2 ) { return 0; }

	switch ( ColID )
	{	
	case CMP_COL_INDEX:
		if ( index1 > index2 ) { Res = 1; }
		else if ( index1 < index2 ) { Res = -1; }
		else {	Res=0; }
		break;
	case CMP_COL_PANEL:
		uVal1 = FPtr1->GetFdPanelIndex_Project();
		uVal2 = FPtr2->GetFdPanelIndex_Project();
		if ( uVal1 > uVal2 ) { Res = 1; }
		else if ( uVal1 < uVal2 ) { Res = -1; }
		else {	Res=0; }
		break;
	case CMP_COL_BOARD:
		uVal1 = FPtr1->GetFdBoardIndex_Project();
		uVal2 = FPtr2->GetFdBoardIndex_Project();
		if ( uVal1 > uVal2 ) { Res = 1; }
		else if ( uVal1 < uVal2 ) { Res = -1; }
		else {	Res=0; }
		break;
	case CMP_COL_FD_NAME:
		uVal1 = FPtr1->GetFdIndex_Project();
		uVal2 = FPtr2->GetFdIndex_Project();
		if ( uVal1 > uVal2 ) { Res = 1; }
		else if ( uVal1 < uVal2 ) { Res = -1; }
		else {	Res=0; }
		//wsPtr1= (wchar_t*)FPtr1->GetFdName();
		//wsPtr2= (wchar_t*)FPtr2->GetComponentName();
		//Res = ::wcscmp(wsPtr1, wsPtr2);		
		break;	
	case CMP_COL_FD_SORT:
		uVal1 = FPtr1->GetFdSortID();
		uVal2 = FPtr2->GetFdSortID();
		if ( uVal1 > uVal2 ) { Res = 1; }
		else if ( uVal1 < uVal2 ) { Res = -1; }
		else {	Res=0; }
		break;
	default:
		Res = 0;
		break;
	}
	int SortMode = GetFdListSortMode();
	Res = Res*SortMode;	
	return Res;	
}
//-------------------------------------------------------------------------------------//
void CFdListWnd::OnColumnclickFdListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	int ColID = GetFdListColD();
	int SortMode = GetFdListSortMode();	
	const int ColumnsIdx = pNMListView->iSubItem;
	if ( ColID != ColumnsIdx )
	{	SortMode = SORT_ASCEND; }
	else
	{
		if ( SORT_ASCEND == SortMode ) { SortMode = SORT_DESCEND; }
		else {	SortMode = SORT_ASCEND;  }
	}
	SetFdListColID(ColumnsIdx);	
	SetFdListSortMode(SortMode);	
	m_FdListWnd.SortItems(FdListCompareFn, (DWORD_PTR)this);
	
	const int nItem = m_FdListWnd.GetNextItem(-1, LVNI_FOCUSED);
	if ( nItem >= 0 ) 
	{	m_FdListWnd.EnsureVisible(nItem, FALSE); }
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CFdListWnd::OnItemchangedFdListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if ( true == m_StopFdListBeSelected ) { return; }
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) 
	{	return;	}
	DWORD Res = pNMListView->uNewState&LVIS_SELECTED;
	if ( Res == 0 ) { return; }	
	Res = pNMListView->uNewState&LVIS_FOCUSED;
	if ( Res == 0 ) { return; }	
	const size_t SelIndex = m_FdListWnd.GetItemData(nItem);
	const size_t FdCount = m_FdList.size();
	if ( SelIndex > FdCount ) { return; }
	CAOIFd *FdPtr = m_FdList[SelIndex];
	if ( NULL == FdPtr ) { return; }	
	AOIDataCollect.MoveStageToFd(FdPtr, false);		
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CFdListWnd::OnDblclkFdListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NMITEMACTIVATE *pNMListView = (NMITEMACTIVATE*)pNMHDR;	
	m_FdPtr = NULL;
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	SetItemIndexAct(-1, -1);
	if ( nItem < 0 ) { return; }
	CThisListCtrl_46 &ListCtrl=m_FdListWnd;
	const size_t SelIndex = ListCtrl.GetItemData(nItem);
	const size_t FdCount = m_FdList.size();
	if ( SelIndex > FdCount ) { return; }
	CAOIFd *FdPtr = m_FdList[SelIndex];
	if ( NULL == FdPtr ) { return; }	
	AOIDataCollect.MoveStageToFd(FdPtr, true);

	CString str;
	CRect   ItemRect={0};
	bool    bShowEdit=false;
	SetFdListColID(nSubItem);	
	switch ( nSubItem )
	{
	case CMP_COL_FD_SORT:
		bShowEdit = true;
		str.Format(_T("%d"), FdPtr->GetFdSortID());
		break;	
	}
	if ( ListCtrl.GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, ItemRect) == FALSE )
	{	return ; }

	SetItemIndexAct(nItem, nSubItem);
	if ( true == bShowEdit )
	{
		RECT CtrlRect=ItemRect;
		ListCtrl.ClientToScreen(&CtrlRect);
		CWnd::ScreenToClient(&CtrlRect);
		m_FdPtr = FdPtr;
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
bool CFdListWnd::ExecUpdateParamByEdit()
{
	if ( m_EditCtrl.GetSafeHwnd() == NULL ) { return false; }

	m_EditCtrl.ShowWindow(SW_HIDE);
	if ( NULL == m_FdPtr ) { return true; }
	const int nItem = m_nItemAct;
	const int nSubItem = m_nSubItemAct;
	if ( -1==nItem || -1==nSubItem ) { return true; }

	CString str;
	CString ItemText;
	int        nValue=0;
	bool       bUpdateAll=false;
	const int  nColID = GetFdListColD();
	CAOIFd    *FdPtr=m_FdPtr;	

	m_EditCtrl.GetWindowText(ItemText);
	ItemText.MakeUpper();	
	switch ( nColID )
	{
	case CMP_COL_FD_SORT:		
		nValue = ::_ttoi(ItemText);
		if ( nValue >= 0 )
		{
			FdPtr->SetFdSortID(nValue);
			bUpdateAll = true;		
		}
		break;	
	}
	if ( true == bUpdateAll ) 
	{	UpdateFdListWnd();	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CFdListWnd::ExecReleaseParamCtrl()
{
	m_FdPtr = NULL;
	SetItemIndexAct(-1, -1);
	m_EditCtrl.ShowWindow(SW_HIDE);
	m_FdListWnd.SetFocus();
	return true;
}
//-------------------------------------------------------------------------------------//
void CFdListWnd::SetItemIndexAct(int nItem, int nSubItem)
{
	m_nItemAct = nItem;
	m_nSubItemAct = nSubItem;	
}
//-------------------------------------------------------------------------------------//