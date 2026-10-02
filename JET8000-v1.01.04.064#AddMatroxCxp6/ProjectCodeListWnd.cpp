// ProjectCodeListWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "ProjectCodeListWnd.h"
//-------------------------------------------------------------------------------------//
#include "InputBoxWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
#define  COL_ID_INDEX                   0
#define  COL_ID_OPEN_CODE               1
#define  COL_ID_FILE_NAME               2
//-------------------------------------------------------------------------------------//
#define  PROJECT_USE_ONE_CODE          false//專案只用1個開檔碼
//-------------------------------------------------------------------------------------//
int CALLBACK OpenCodeListCompareFn(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
int CALLBACK OpenCodeListCompareFn(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	if ( NULL == lParamSort ) { return 0; }
	CProjectCodeListWnd* pOpenCodeListWnd = (CProjectCodeListWnd*)lParamSort;
	return pOpenCodeListWnd->CompareOpenCodeItem(lParam1, lParam2);
}
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CProjectCodeListWnd dialog
//-------------------------------------------------------------------------------------//
CProjectCodeListWnd::CProjectCodeListWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CProjectCodeListWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProjectCodeListWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_EditItemIndex = -1;
	m_EditSubItemIndex = -1;
	m_OpenCodeListColD = COL_ID_INDEX;
	m_OpenCodeSortMode = SORT_ASCEND;
	m_StopOpenCodeListBeSelected = false;
}
//-------------------------------------------------------------------------------------//
void CProjectCodeListWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProjectCodeListWnd)	
	DDX_Control(pDX, PROCODE_EDIT_CTRL, m_EditCtrl);
	DDX_Control(pDX, PROCODE_OPEN_CODE_LIST_WND, m_OpenCodeListWnd);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CProjectCodeListWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CProjectCodeListWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_GETMINMAXINFO()
	ON_NOTIFY(LVN_COLUMNCLICK, PROCODE_OPEN_CODE_LIST_WND, OnColumnclickOpenCodeListWnd)
	ON_NOTIFY(LVN_ITEMCHANGED, PROCODE_OPEN_CODE_LIST_WND, OnItemchangedOpenCodeListWnd)
	ON_NOTIFY(NM_DBLCLK, PROCODE_OPEN_CODE_LIST_WND, OnDblclkOpenCodeListWnd)
	ON_EN_KILLFOCUS(PROCODE_EDIT_CTRL, OnKillfocusEditCtrl)
	ON_BN_CLICKED(PROCODE_ADD_PROJECT_BTN, OnAddProjectBtn)
	ON_BN_CLICKED(PROCODE_DEL_PROJECT_BTN, OnDelProjectBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CProjectCodeListWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CProjectCodeListWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CWnd::ShowWindow(SW_SHOWMAXIMIZED);
	JetAPI::InitialListCtrl(m_OpenCodeListWnd);		
	SwitchMultiLanguage();
	m_ProjectFolder = AOIDataCollect.GetAOIProjectDirectory();
	BuildProjectCodeListWndHeader();	
	BuildProjectCodeListWnd();
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CProjectCodeListWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CProjectCodeListWnd::OnSize(UINT nType, int cx, int cy) 
{
	CBaseDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if ( CWnd::GetSafeHwnd() == NULL ) { return ; }
	const int Gap = 4;
	CThisListCtrl_50 &ListCtrl = m_OpenCodeListWnd;
	if ( ListCtrl.GetSafeHwnd() != NULL )
	{
		RECT   WndRect={0,0,0,0};
		ListCtrl.GetWindowRect(&WndRect);
		CWnd::ScreenToClient(&WndRect);
		WndRect.right = cx-Gap;
		WndRect.bottom = cy-Gap;
		ListCtrl.MoveWindow(&WndRect, TRUE);
	}
}
//-------------------------------------------------------------------------------------//
void CProjectCodeListWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CBaseDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CProjectCodeListWnd::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default	
	CBaseDialog::OnGetMinMaxInfo(lpMMI);

	if (lpMMI->ptMinTrackSize.x < 1280 )
	{	lpMMI->ptMinTrackSize.x = 1280; }

	if (lpMMI->ptMinTrackSize.y < 480)
	{	lpMMI->ptMinTrackSize.y = 480;	}	
}
//-------------------------------------------------------------------------------------//
void CProjectCodeListWnd::SetOpenCodeList(const std::vector<TProjectOpenCode> &OpenCodeList)
{
	m_OpenCodeList = OpenCodeList;
}
//-------------------------------------------------------------------------------------//
void CProjectCodeListWnd::CloneOpenCodeList(std::vector<TProjectOpenCode> &OpenCodeList)
{
	OpenCodeList = m_OpenCodeList;
}
//-------------------------------------------------------------------------------------//
void CProjectCodeListWnd::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_PROJECT_CODE_LIST_WND");
	//---------------------------------------------------------------------------------//
	WndID = IDD_PROJECT_CODE_LIST_WND;
	WndKey = _T("IDD_PROJECT_CODE_LIST_WND");
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
	WndID = PROCODE_FILENAME_LABEL;
	WndKey = _T("PROCODE_FILENAME_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	
	WndID = PROCODE_OPEN_CODE_LABEL;
	WndKey = _T("PROCODE_OPEN_CODE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = PROCODE_ADD_PROJECT_BTN;
	WndKey = _T("PROCODE_ADD_PROJECT_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROCODE_DEL_PROJECT_BTN;
	WndKey = _T("PROCODE_DEL_PROJECT_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//	
}
//-------------------------------------------------------------------------------------//
CString CProjectCodeListWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_PROJECT_CODE_LIST_WND");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
int CProjectCodeListWnd::GetOpenCodeListColD() const
{
	return m_OpenCodeListColD;
}
//-------------------------------------------------------------------------------------//
void CProjectCodeListWnd::SetOpenCodeListColD(int val)
{
	m_OpenCodeListColD = val;
}
//-------------------------------------------------------------------------------------//
int CProjectCodeListWnd::GetOpenCodeSortMode() const
{
	return m_OpenCodeSortMode;
}
//-------------------------------------------------------------------------------------//
void CProjectCodeListWnd::SetOpenCodeSortMode(int val)
{
	m_OpenCodeSortMode = val;
}
//-------------------------------------------------------------------------------------//
bool CProjectCodeListWnd::BuildProjectCodeListWnd()
{	
	CThisListCtrl_50 &ListCtrl = m_OpenCodeListWnd;
	//m_AllFileListCompareID = -1;
	m_StopOpenCodeListBeSelected = true;	
	JetAPI::ClearListCtrl(ListCtrl, FALSE);
	m_StopOpenCodeListBeSelected = false;


	size_t       i=0;
	CString      str, str2;
	int          nItem=0;
	int          nSubItem=0;	
	TProjectOpenCode OpenCode;	
	CString      strFolder = m_ProjectFolder;
	const size_t OpenCodeCount = m_OpenCodeList.size();	
	
	nItem = 0;
	ListCtrl.SetRedraw(FALSE);
	m_StopOpenCodeListBeSelected = true;
	for ( i=0; i<OpenCodeCount; i++ )
	{
		nSubItem = 0;
		str.Format(_T("%d"), i+1);		
		ListCtrl.InsertItem(nItem, str);
		ListCtrl.SetItemData(nItem, (DWORD_PTR)i);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		str = m_OpenCodeList[i].wsOpenCode.c_str();		
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		str2 = m_OpenCodeList[i].wsProjectName.c_str();		
		str.Format(_T("%s\\%s"), strFolder, str2);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;
	}
	m_StopOpenCodeListBeSelected = false;
	ListCtrl.SetRedraw(TRUE);	

	SetOpenCodeListColD(COL_ID_INDEX);
	SetOpenCodeSortMode(SORT_ASCEND);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectCodeListWnd::BuildProjectCodeListWndHeader()
{
	CString str;
	int   nCol = 0;
	int width  = 64;
	int width2 = 64;
	RECT      Rect;	
	const int Align = LVCFMT_LEFT;//LVCFMT_CENTER;LVCFMT_RIGHT, LVCFMT_LEFT

	{
		CThisListCtrl_50 &ListCtrl = m_OpenCodeListWnd;
		
		ListCtrl.GetClientRect(&Rect);
		width = 164;
		width2 = (Rect.right-Rect.left-width-96);

		str = _T("Index");
		str = AOIDataDefine.GetIndexText();
		ListCtrl.InsertColumn(nCol, str, Align, 64);
		nCol ++;

		str = _T("Open Code");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width);
		nCol ++;

		str = _T("Filename");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width2);
		nCol ++;

	}
	return true;
}
//-------------------------------------------------------------------------------------//
int CProjectCodeListWnd::CompareOpenCodeItem(size_t index1, size_t index2)
{
	CThisListCtrl_50 &ListCtrl = m_OpenCodeListWnd;
	const int ColID = GetOpenCodeListColD();
	const size_t OpenCodeCount = m_OpenCodeList.size();
	if ( index1>=OpenCodeCount || index2>=OpenCodeCount )
	{	return 0; }
	int          Res=0;		
	TProjectOpenCode *Ptr1=&(m_OpenCodeList[index1]);
	TProjectOpenCode *Ptr2=&(m_OpenCodeList[index2]);	

	switch ( ColID )
	{	
	case COL_ID_INDEX:
		if ( index1 > index2 ) { Res = 1; }
		else if ( index1 < index2 ) { Res = -1; }
		else {	Res=0; }
		break;
	case COL_ID_OPEN_CODE:		
		Res = ::wcscmp(Ptr1->wsOpenCode.c_str(), Ptr2->wsOpenCode.c_str());	
		break;
	case COL_ID_FILE_NAME:
		Res = ::wcscmp(Ptr1->wsProjectName.c_str(), Ptr2->wsProjectName.c_str());	
		break;
	}

	int SortMode = GetOpenCodeSortMode();
	Res = Res*SortMode;	
	return Res;
}
//-------------------------------------------------------------------------------------//
void CProjectCodeListWnd::ClearItemInfo()
{
	m_EditItemIndex = -1;
	m_EditSubItemIndex = -1;
	CWnd::SetDlgItemText(PROCODE_FILENAME_EDIT, _T(""));
	CWnd::SetDlgItemText(PROCODE_OPEN_CODE_EDIT, _T(""));
}
//-------------------------------------------------------------------------------------//
void CProjectCodeListWnd::UpdateItemInfo(const TProjectOpenCode &OpenCode)
{
	CString  str, str2;
	CString  strFolder = m_ProjectFolder;

	str2 = OpenCode.wsProjectName.c_str();
	str.Format(_T("%s\\%s"), strFolder, str2);
	CWnd::SetDlgItemText(PROCODE_FILENAME_EDIT, str);

	str = OpenCode.wsOpenCode.c_str();	
	CWnd::SetDlgItemText(PROCODE_OPEN_CODE_EDIT, str);
}
//-------------------------------------------------------------------------------------//
bool CProjectCodeListWnd::ExecUpdateParamByEdit()
{
	CString ItemText;	
	m_EditCtrl.ShowWindow(SW_HIDE);
	m_EditCtrl.GetWindowText(ItemText);		
	CThisListCtrl_50 &ListCtrl = m_OpenCodeListWnd;

	const int nItem = m_EditItemIndex;
	const int nSubItem = m_EditSubItemIndex;
	const int ItemCount = ListCtrl.GetItemCount();
	if ( nSubItem <= COL_ID_INDEX )	{ return false; }
	if ( nItem<0 || nItem>=ItemCount )	{ return false; }	
	
	const size_t SelIndex = ListCtrl.GetItemData(nItem);
	const size_t OpenCodeCount = m_OpenCodeList.size();
	if ( SelIndex >= OpenCodeCount ) { return false; }
	
	std::wstring  wsStr;
	JetAPI::TCHAR2wstring(ItemText, wsStr);
	switch ( nSubItem )
	{
	case COL_ID_OPEN_CODE:	m_OpenCodeList[SelIndex].wsOpenCode = wsStr;	break;
	case COL_ID_FILE_NAME:	m_OpenCodeList[SelIndex].wsProjectName = wsStr;	break;		
	}
	UpdateItemInfo(m_OpenCodeList[SelIndex]);
	ListCtrl.SetItemText(nItem, nSubItem, ItemText);	
	ListCtrl.SetFocus();	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectCodeListWnd::ExecItemchangedOpenCodeListWnd(int nItem)
{
	ClearItemInfo();
	CThisListCtrl_50 &ListCtrl = m_OpenCodeListWnd;
	const int ItemCount = ListCtrl.GetItemCount();
	if ( nItem<0 || nItem>=ItemCount )	{	return false; }
	
	CString      str;
	const size_t SelIndex = ListCtrl.GetItemData(nItem);
	const size_t OpenCodeCount = m_OpenCodeList.size();
	if ( SelIndex >= OpenCodeCount ) { return false; }	
	UpdateItemInfo(m_OpenCodeList[SelIndex]);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectCodeListWnd::ExecDblclkOpenCodeListWnd(int nItem, int nSubItem)
{
	CThisListCtrl_50 &ListCtrl = m_OpenCodeListWnd;
	const int ItemCount = ListCtrl.GetItemCount();
	if ( COL_ID_OPEN_CODE != nSubItem ) { return false; }
	if ( nItem<0 || nItem>=ItemCount )	{	return false; }

	CString      str;
	CString      ItemText;
	RECT         CtrlRect;
	CRect        ItemRect;
	const int    Offset = 2;
	const size_t SelIndex = ListCtrl.GetItemData(nItem);
	const size_t OpenCodeCount = m_OpenCodeList.size();
	if ( SelIndex >= OpenCodeCount ) { return false; }
	TProjectOpenCode OpenCode=m_OpenCodeList[SelIndex];
	
	if ( ListCtrl.GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, ItemRect) == FALSE )
	{	return false; }
	m_EditItemIndex = nItem;
	m_EditSubItemIndex = nSubItem;
	ItemText = ListCtrl.GetItemText(nItem, nSubItem);

	CtrlRect = ItemRect;
	ListCtrl.ClientToScreen(&CtrlRect);
	this->ScreenToClient(&CtrlRect);
	::OffsetRect(&CtrlRect, 0, -2);	
	if ( m_EditCtrl.GetSafeHwnd() != NULL )
	{	
		::OffsetRect(&CtrlRect, 1, 1);
		::InflateRect(&CtrlRect, Offset, Offset);			
		m_EditCtrl.SetWindowText(ItemText);
		m_EditCtrl.MoveWindow(&CtrlRect, FALSE);
		m_EditCtrl.SetFocus();
		m_EditCtrl.SetSel(0,-1);
		m_EditCtrl.ShowWindow(SW_SHOW);	
		m_EditCtrl.BringWindowToTop();
		ListCtrl.UpdateWindow();
		m_EditCtrl.Invalidate();			
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectCodeListWnd::ExecAddProjectBtn(LPCTSTR pfilename)
{	
	CString str, str2;	
	CString strShortName;	
	CString strFilename = pfilename;
	CString strFolder = m_ProjectFolder;	
	
	if ( JetAPI::ExtractShortFilename(strFolder, strFilename, strShortName) == false )
	{
		str2 = _T("Error, Add Project Fault");
		str2 = LoadMultiLanguageString(str2, str2);
		str.Format(_T("%s\n%s"), str2, pfilename);
		JetAPI::ShowMessageBox(str);
		return false;
	}

	if ( ExecAddProjectName(strShortName) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectCodeListWnd::ExecAddProjectName(LPCTSTR pfilename)
{	
	if ( NULL == pfilename ) { return false; }	
	CThisListCtrl_50 &ListCtrl = m_OpenCodeListWnd;
	if ( ListCtrl.GetSafeHwnd() == NULL ) { return false; }
	
	CString  str;	
	CString  str2;
	CString  strFolder = m_ProjectFolder;
	int   nSubItem = 0;
	const size_t i = m_OpenCodeList.size();
	const int nItem = ListCtrl.GetItemCount();
	if ( true == PROJECT_USE_ONE_CODE )
	{
		if ( CheckProjectNameExist(pfilename) == true )
		{ 
			str = _T("Error, Project Name already exsit");
			str = LoadMultiLanguageString(str, str);
			JetAPI::ShowMessageBox(str);
			return false; 
		}
	}
	
	CString      strLabel;
	CString      strValue;		
	CString      strCaption;		
	CInputBoxWnd InputBoxWnd;

	strCaption = _T("Input Project Open Code");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strLabel = _T("Project Open Code");
	strLabel = LoadMultiLanguageString(strLabel, strLabel);
	strValue = _T("");
	InputBoxWnd.SetParam1(strCaption, strLabel, strValue);
	if ( InputBoxWnd.DoModal() == IDCANCEL ) { return false; }	
	strValue = InputBoxWnd.m_DataEdit1;

	TProjectOpenCode ProjectOpenCode;
	JetAPI::TCHAR2wstring(pfilename, ProjectOpenCode.wsProjectName);
	JetAPI::TCHAR2wstring(strValue, ProjectOpenCode.wsOpenCode);
	m_OpenCodeList.push_back(ProjectOpenCode);
	
	m_StopOpenCodeListBeSelected = true;
	nSubItem = 0;
	str.Format(_T("%d"), i+1);		
	ListCtrl.InsertItem(nItem, str);
	ListCtrl.SetItemData(nItem, (DWORD_PTR)i);
	ListCtrl.SetItemText(nItem, nSubItem, str);
	nSubItem ++;

	str = ProjectOpenCode.wsOpenCode.c_str();		
	ListCtrl.SetItemText(nItem, nSubItem, str);
	nSubItem ++;

	str2 = ProjectOpenCode.wsProjectName.c_str();		
	str.Format(_T("%s\\%s"), strFolder, str2);
	ListCtrl.SetItemText(nItem, nSubItem, str);
	nSubItem ++;
	m_StopOpenCodeListBeSelected = false;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectCodeListWnd::CheckProjectNameExist(LPCTSTR pfilename)
{
	if ( NULL == pfilename  ) { return false; }

	size_t i=0;
	const std::vector<TProjectOpenCode> &OpenCodeListRef=m_OpenCodeList;
	const size_t OpenCodeCount=OpenCodeListRef.size();

	for ( i=0; i<OpenCodeCount; i++  )
	{
		const TProjectOpenCode &OpenCodeRef=OpenCodeListRef[i];

		CString ProjectName=OpenCodeRef.wsProjectName.c_str();
		if ( ProjectName.CollateNoCase(pfilename) == 0 )
		{	return true; }
	}
	return false;
}
//-------------------------------------------------------------------------------------//
bool CProjectCodeListWnd::ExecDelProjectName(int nItem)
{
	CThisListCtrl_50 &ListCtrl = m_OpenCodeListWnd;
	if ( ListCtrl.GetSafeHwnd() == NULL ) { return false; }
	const int ItemCount = ListCtrl.GetItemCount();
	if ( nItem<0 || nItem>=ItemCount ) { return false; }	
	const size_t OpenCodeCount = m_OpenCodeList.size();
	const size_t ItemIndex = (size_t)(ListCtrl.GetItemData(nItem));
	if ( ItemIndex >= OpenCodeCount ) { return false; }

	CString str, str2;	
	CString strFilename;
	CString strFolder = m_ProjectFolder;	
	CString strShortName = m_OpenCodeList[ItemIndex].wsProjectName.c_str();

	strFilename.Format(_T("%s\\%s"), strFolder, strShortName);
	str2 = _T("Do you want to delete the project open code ?");
	str2 = LoadMultiLanguageString(str2, str2);
	str.Format(_T("%s\n%s"), str2, strFilename);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO )
	{	return true; }

	const int nSubItem = COL_ID_OPEN_CODE;	
	if ( true == PROJECT_USE_ONE_CODE )
	{
		m_OpenCodeList[ItemIndex].wsOpenCode = L"";
		ListCtrl.SetItemText(nItem, nSubItem, _T(""));
	}
	else
	{
		ListCtrl.SetRedraw(FALSE);		
		for ( int i=0; i<ItemCount; i++ )
		{
			const int nItem2=i;
			const size_t ItemIndex2 = (size_t)(ListCtrl.GetItemData(nItem2));
			if ( ItemIndex2 >= OpenCodeCount ) {	continue; }
			CString strShortName2 = m_OpenCodeList[ItemIndex2].wsProjectName.c_str();
			if ( strShortName2.CollateNoCase(strShortName) != 0 ) { continue; }
			m_OpenCodeList[ItemIndex2].wsOpenCode = L"";
			ListCtrl.SetItemText(nItem2, nSubItem, _T(""));
		}		
		ListCtrl.SetRedraw(TRUE);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CProjectCodeListWnd::OnColumnclickOpenCodeListWnd(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	int ColID = GetOpenCodeListColD();
	int SortMode = GetOpenCodeSortMode();	
	const int ColumnsIdx = pNMListView->iSubItem;
	if ( ColID != ColumnsIdx )
	{	SortMode = SORT_ASCEND; }
	else
	{
		if ( SORT_ASCEND == SortMode ) { SortMode = SORT_DESCEND; }
		else {	SortMode = SORT_ASCEND;  }
	}
	SetOpenCodeListColD(ColumnsIdx);	
	SetOpenCodeSortMode(SortMode);	
	m_OpenCodeListWnd.SortItems(OpenCodeListCompareFn, (DWORD_PTR)this);
	
	const int nItem = m_OpenCodeListWnd.GetNextItem(-1, LVNI_FOCUSED);
	if ( nItem >= 0 ) 
	{	m_OpenCodeListWnd.EnsureVisible(nItem, FALSE); }
}
//-------------------------------------------------------------------------------------//
void CProjectCodeListWnd::OnItemchangedOpenCodeListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if ( true == m_StopOpenCodeListBeSelected ) { return; }	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) 
	{	return;	}
	DWORD Res = pNMListView->uNewState&LVIS_SELECTED;
	if ( Res == 0 ) { return; }	
	Res = pNMListView->uNewState&LVIS_FOCUSED;
	if ( Res == 0 ) { return; }	
	ExecItemchangedOpenCodeListWnd(nItem);	
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CProjectCodeListWnd::OnDblclkOpenCodeListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NMITEMACTIVATE *pNMListView = (NMITEMACTIVATE*)pNMHDR;	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;	
	ExecDblclkOpenCodeListWnd(nItem, nSubItem);
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
BOOL CProjectCodeListWnd::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch ( pMsg->message )
	{
	case WM_KEYDOWN:
		switch ( pMsg->wParam )
		{
		case VK_ESCAPE:
			if ( m_EditCtrl.IsWindowVisible() == TRUE )
			{	
				m_EditSubItemIndex=COL_ID_INDEX;
				ExecUpdateParamByEdit();
			}
			return TRUE;
			break;
		case VK_RETURN:
			if ( m_EditCtrl.IsWindowVisible() == TRUE )
			{	ExecUpdateParamByEdit();	}
			return TRUE;
			break;
		}
		break;
	}
	return CBaseDialog::PreTranslateMessage(pMsg);
}
//-------------------------------------------------------------------------------------//
LRESULT CProjectCodeListWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CBaseDialog::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void CProjectCodeListWnd::OnKillfocusEditCtrl() 
{
	// TODO: Add your control notification handler code here
	ExecUpdateParamByEdit();
}
//-------------------------------------------------------------------------------------//
void CProjectCodeListWnd::OnAddProjectBtn() 
{
	// TODO: Add your control notification handler code here
	TCHAR szFilters[]=_T("Project Files (*.PRG)|*.PRG|All Files (*.*)|*.*||");
	CFileDialog dialog(TRUE, _T("PRG"), _T("*.PRG"), OFN_FILEMUSTEXIST, szFilters, this);
	if ( dialog.DoModal() != IDOK )
	{	return ; }
	CString filename = dialog.GetPathName();
	ExecAddProjectBtn(filename);
}
//-------------------------------------------------------------------------------------//
void CProjectCodeListWnd::OnDelProjectBtn() 
{
	// TODO: Add your control notification handler code here
	const int nItem = m_OpenCodeListWnd.GetNextItem(-1, LVNI_SELECTED);
	if ( nItem < 0 ) { return; }
	ExecDelProjectName(nItem);	
}
//-------------------------------------------------------------------------------------//