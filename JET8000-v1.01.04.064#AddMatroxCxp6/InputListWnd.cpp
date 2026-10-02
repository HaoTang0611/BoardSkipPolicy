// InputListWnd.cpp : implementation file
//

#include "stdafx.h"
#include "jet8000.h"
#include "InputListWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CInputListWnd dialog
//-------------------------------------------------------------------------------------//
CInputListWnd::CInputListWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CInputListWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputListWnd)
	m_TitleLabel1 = _T("");
	//}}AFX_DATA_INIT
	m_CurSel = -1;
	m_WndMovePos = false;	
	m_ShowIndexCol=false;
	m_WndPos.x = m_WndPos.y = 0;
	m_StopListBeSelected = false;
}
//-------------------------------------------------------------------------------------//
void CInputListWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputListWnd)
	DDX_Control(pDX, INPUTBOX_LIST_WND, m_ListCtrl);
	DDX_Control(pDX, INPUTBOX_PARAM_COMBO, m_ComboxCtrl);	
	DDX_Text(pDX, INPUTBOX_TITLE_LABEL1, m_TitleLabel1);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CInputListWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CInputListWnd)
	ON_WM_DESTROY()
	ON_NOTIFY(NM_CLICK, INPUTBOX_LIST_WND, OnClickListWnd)
	ON_NOTIFY(LVN_ITEMCHANGED, INPUTBOX_LIST_WND, OnItemchangedListWnd)
	ON_NOTIFY(NM_DBLCLK, INPUTBOX_LIST_WND, OnDblclkListWnd)
	ON_CBN_SELCHANGE(INPUTBOX_PARAM_COMBO, OnSelchangeParamCombo)
	ON_CBN_KILLFOCUS(INPUTBOX_PARAM_COMBO, OnKillfocusParamCombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CInputListWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CInputListWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitListCtrl();
	SwitchMultiLanguage();
	CWnd::SetWindowText(m_WndText);
	if ( true == m_WndMovePos )
	{
		RECT  WndRect = {0};
		SIZE  WndSize;
		CWnd::GetWindowRect(&WndRect);
		WndSize.cx = WndRect.right-WndRect.left;
		WndSize.cy = WndRect.bottom-WndRect.top;
		WndRect.left = m_WndPos.x - (WndSize.cx/2);
		WndRect.right = WndRect.left + WndSize.cx;
		WndRect.top = m_WndPos.y - (WndSize.cy/2);
		WndRect.bottom = WndRect.top + WndSize.cy;
		CWnd::MoveWindow(&WndRect);
		//m_WndMovePos = false;
	}
	UpdateData();
	BuildListCtrl();
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CInputListWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CInputListWnd::OnOK() 
{
	// TODO: Add extra validation here
	m_SelData = TListNode();
	m_SelData.Data = -1;
	m_SelData.Ptr  = NULL;
	m_SelData.Text = _T("");

	CThisListCtrl_39 &ListCtrl = m_ListCtrl;
	const int nItem = ListCtrl.GetNextItem(-1, LVNI_FOCUSED);
	m_CurSel = nItem;
	if ( m_CurSel >= 0 ) 
	{
		TListNode *NodePtr = NULL;	
		NodePtr = (TListNode*)(ListCtrl.GetItemData(m_CurSel));
		if ( NULL != NodePtr )
		{	m_SelData = *NodePtr;	}
	}
	CBaseDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CInputListWnd::SetWndPos(const POINT &Pos)
{
	m_WndPos = Pos;
	m_WndMovePos = true;
}
//-------------------------------------------------------------------------------------//
int CInputListWnd::GetSelIndex1() const
{
	return m_CurSel;
}
//-------------------------------------------------------------------------------------//
TListNode& CInputListWnd::GetSelNode()
{
	return m_SelData;
}
//-------------------------------------------------------------------------------------//
void* CInputListWnd::GetSelPtr() const
{
	return m_SelData.Ptr;
}
//-------------------------------------------------------------------------------------//
DWORD_PTR CInputListWnd::GetSelData() const
{
	return m_SelData.Data;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CInputListWnd::GetSelText() const
{
	return m_SelData.Text;
}
//-------------------------------------------------------------------------------------//
void CInputListWnd::SetSelIndex1(int nSel)
{
	m_CurSel = nSel;
}
//-------------------------------------------------------------------------------------//
void CInputListWnd::GetDataList(std::vector<TListNode> &DataList)
{
	DataList = m_DataList;
}
//-------------------------------------------------------------------------------------//
void CInputListWnd::SetDropList(const std::vector<TListNode> &DropList)
{
	m_DropList = DropList;
}
//-------------------------------------------------------------------------------------//
void CInputListWnd::SetParam1(LPCTSTR WndTxt, LPCTSTR Title, void* Default, const std::vector<TListNode> &DataList)
{
	m_WndText = WndTxt;
	m_TitleLabel1 = Title;
	m_CurSel = SearchIndex(Default, DataList);
	m_DataList = DataList;
}
//-------------------------------------------------------------------------------------//
void CInputListWnd::SetParam1(LPCTSTR WndTxt, LPCTSTR Title, LPCTSTR Default, const std::vector<TListNode> &DataList)
{
	m_WndText = WndTxt;
	m_TitleLabel1 = Title;
	m_CurSel = SearchIndex(Default, DataList);
	m_DataList = DataList;
}
//-------------------------------------------------------------------------------------//
void CInputListWnd::SetParam1(LPCTSTR WndTxt, LPCTSTR Title, DWORD_PTR Default, const std::vector<TListNode> &DataList)
{
	m_WndText = WndTxt;
	m_TitleLabel1 = Title;
	m_CurSel = SearchIndex(Default, DataList);
	m_DataList = DataList;
}
//-------------------------------------------------------------------------------------//
bool CInputListWnd::CheckDropList() const
{
	if ( 0 == m_DropList.size() ) 
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
int CInputListWnd::GetDropListSubItem() const
{
	if ( true == m_ShowIndexCol )
	{	return 2;	}
	return 1;
}
//-------------------------------------------------------------------------------------//
bool CInputListWnd::InitListCtrl()
{
	CThisListCtrl_39 &ListCtrl = m_ListCtrl;

	JetAPI::InitialListCtrl(ListCtrl);

	CString str;
	int   nCol = 0;
	int width  = 64;
	int width2 = 64;
	RECT      Rect;	
	const int Align = LVCFMT_LEFT;//LVCFMT_CENTER;LVCFMT_RIGHT, LVCFMT_LEFT
	const bool bDropList = CheckDropList();

	{	

		ListCtrl.GetClientRect(&Rect);
		if ( false == m_ShowIndexCol )
		{	width2 = (Rect.right-Rect.left-32);	}
		else
		{
			width = 48;
			width2 = (Rect.right-Rect.left-width-32);
			str = AOIDataDefine.GetIndexText();		
			ListCtrl.InsertColumn(nCol, str, Align, width);
			nCol ++;
		}

		if ( true == bDropList )
		{	width2 = width2/2;	}

		str = AOIDataDefine.GetNameText();		
		ListCtrl.InsertColumn(nCol, str, Align, width2);
		nCol ++;
		
		if ( true == bDropList )
		{
			str = AOIDataDefine.GetSetText();			
			ListCtrl.InsertColumn(nCol, str, Align, width2);
			nCol ++;
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CInputListWnd::BuildListCtrl()
{
	CThisListCtrl_39 &ListCtrl = m_ListCtrl;
	m_StopListBeSelected = true;
	JetAPI::ClearListCtrl(ListCtrl, FALSE);	
	m_StopListBeSelected = false;

	CString      str;
	size_t       i=0;	
	int          nItem=0;
	TListNode   *NodePtr=NULL;
	const size_t Count = m_DataList.size();
	const bool bDropList = CheckDropList();
	const int  nDropSubItem = GetDropListSubItem();

	nItem=0;
	m_StopListBeSelected = true;
	ListCtrl.SetRedraw(FALSE);	
	for ( i=0; i<Count; i++ )
	{
		NodePtr = &(m_DataList[i]);

		if ( false == m_ShowIndexCol )
		{
			str = NodePtr->Text;
			ListCtrl.InsertItem(nItem, str);
			ListCtrl.SetItemData(nItem, (DWORD_PTR)(NodePtr));
		}
		else
		{
			str.Format(_T("%d"), nItem+1);
			ListCtrl.InsertItem(nItem, str);
			ListCtrl.SetItemData(nItem, (DWORD_PTR)(NodePtr));

			ListCtrl.SetItemText(nItem, 1, NodePtr->Text);		
		}

		if ( bDropList )
		{	ListCtrl.SetItemText(nItem, nDropSubItem, NodePtr->Text2);	}

		nItem ++;
	}
	ListCtrl.SetRedraw();
	m_StopListBeSelected = false;
	if ( m_CurSel>=0 && m_CurSel<nItem ) 
	{	ListCtrl.SetItemState(m_CurSel, LVIS_FOCUSED|LVIS_SELECTED, LVIS_FOCUSED|LVIS_SELECTED);	}
	ListCtrl.SetFocus();
	return true;
}
//-------------------------------------------------------------------------------------//
int CInputListWnd::SearchIndex(void *Ptr, const std::vector<TListNode> &DataList)
{
	int          nSel = -1;
	size_t       i=0;		
	const size_t Count = DataList.size();

	for ( i=0; i<Count; i++ )
	{
		const TListNode *NodePtr = &(DataList[i]);
		if ( NodePtr->Ptr != Ptr ) { continue; }
		nSel = i;
		break;
	}
	return nSel;
}
//-------------------------------------------------------------------------------------//
int CInputListWnd::SearchIndex(LPCTSTR Text, const std::vector<TListNode> &DataList)
{
	int          nSel = -1;
	size_t       i=0;		
	const size_t Count = DataList.size();

	for ( i=0; i<Count; i++ )
	{
		const TListNode *NodePtr = &(DataList[i]);
		if ( NodePtr->Text.CompareNoCase(Text) != 0 ) { continue; }
		nSel = i;
		break;
	}
	return nSel;
}
//-------------------------------------------------------------------------------------//
int CInputListWnd::SearchIndex(DWORD_PTR Data, const std::vector<TListNode> &DataList)
{
	int          nSel = -1;
	size_t       i=0;	
	const size_t Count = DataList.size();

	for ( i=0; i<Count; i++ )
	{
		const TListNode *NodePtr = &(DataList[i]);
		if ( NodePtr->Data != Data ) { continue; }
		nSel = i;
		break;
	}
	return nSel;
}
//-------------------------------------------------------------------------------------//
void CInputListWnd::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_INPUT_LIST_WND");
	//---------------------------------------------------------------------------------//
	WndID = IDD_INPUT_LIST_WND;
	WndKey = _T("IDD_INPUT_LIST_WND");
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
	//---------------------------------------------------------------------------------/
}
//-------------------------------------------------------------------------------------//
CString CInputListWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_INPUT_LIST_WND");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
void CInputListWnd::OnClickListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here	
	NMITEMACTIVATE *pNMListView = (NMITEMACTIVATE*)pNMHDR;	
	const int nItem = pNMListView->iItem;	
	ExecSelItem(nItem);
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CInputListWnd::OnItemchangedListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here	
	if ( true == m_StopListBeSelected ) 
	{	return ;	}
	DWORD Res = pNMListView->uNewState&LVIS_SELECTED;
	if ( Res == 0 ) { return; }	
	Res = pNMListView->uNewState&LVIS_FOCUSED;
	if ( Res == 0 ) { return; }
	const int nItem = pNMListView->iItem;	
	ExecSelItem(nItem);	
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
bool CInputListWnd::ExecSelItem(int nItem)
{
	if ( nItem < 0 ) { return false; }
	CThisListCtrl_39 &ListCtrl = m_ListCtrl;
	const int ItemCount = ListCtrl.GetItemCount();
	if ( nItem >= ItemCount ) { return false; }

	void *ItemData = (void*)(ListCtrl.GetItemData(nItem));
	if ( NULL == ItemData ) { return false; }

	CString    str;
	TListNode *NodePtr=(TListNode*)ItemData;
	CString    strIndex = AOIDataDefine.GetIndexText();
	str.Format(_T("%s:%d - %s"), strIndex, nItem+1, NodePtr->Text);
	CWnd::SetDlgItemText(INPUTBOX_INFO_EDIT, str);
	return true;
}
//-------------------------------------------------------------------------------------//
void CInputListWnd::OnDblclkListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here	
	NMITEMACTIVATE *pNMListView = (NMITEMACTIVATE*)pNMHDR;	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) { return; }

	if ( CheckDropList() )
	{	ShowDropListWnd(m_ListCtrl, nItem, nSubItem);	}
	else
	{	OnOK();	}
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
bool CInputListWnd::ExecUpdateParamByCombox()
{
	CString ComboxText;
	CComboBox &ComboxCtrl=m_ComboxCtrl;
	const int nCurSel = ComboxCtrl.GetCurSel();
	if ( nCurSel < 0 ) { return true; }
	ComboxCtrl.GetWindowText(ComboxText);

	const int nItem = m_CurSel;
	CThisListCtrl_39 &ListCtrl=m_ListCtrl;
	const int ItemCount = ListCtrl.GetItemCount();
	if ( nItem<0 || nItem>=ItemCount ) { return false; }	
	void *ItemData = (void*)(ListCtrl.GetItemData(nItem));
	if ( NULL == ItemData ) { return false; }
	TListNode *NodePtr=(TListNode*)ItemData;
	const int DropListSubItem = GetDropListSubItem();
	NodePtr->Text2 = ComboxText;	
	ListCtrl.SetItemText(nItem, DropListSubItem, NodePtr->Text2);	
	ListCtrl.SetFocus();	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CInputListWnd::ShowDropListWnd(CThisListCtrl_39 &ListCtrl, int nItem, int nSubItem)
{	
	const int DropListSubItem = GetDropListSubItem();
	if ( DropListSubItem != nSubItem )
	{	return false; }	

	CComboBox &ComboxCtrl=m_ComboxCtrl;	
	const std::vector<TListNode>  &DropList=m_DropList;
	const size_t DropCount = DropList.size();
	if ( 0 == DropCount ) { return true; }
	if ( ComboxCtrl.GetSafeHwnd() == NULL ) { return false ;}

	CRect ItemRect;	
	JetAPI::ClearCombox(ComboxCtrl);
	if ( ListCtrl.GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, ItemRect) == FALSE )
	{	return false; }	

	int nSelIdx = 0;
	int nActIdx = -1;
	RECT CtrlRect = ItemRect;	
	CString ItemText = ListCtrl.GetItemText(nItem, nSubItem);

	ListCtrl.ClientToScreen(&CtrlRect);
	CWnd::ScreenToClient(&CtrlRect);
	::OffsetRect(&CtrlRect, 0, -2);	
	for ( size_t i=0; i<DropCount; i++ )
	{
		const TListNode &NodeRef=DropList[i];
		m_ComboxCtrl.InsertString(nSelIdx, NodeRef.Text);
		m_ComboxCtrl.SetItemData(nSelIdx, NodeRef.Data);
		if ( -1==nActIdx && NodeRef.Text.CompareNoCase(ItemText) == 0 )
		{	nActIdx = nSelIdx;	}

		nSelIdx ++;
	}			
	m_CurSel = nItem;
	ComboxCtrl.SetCurSel(nActIdx);
	//JetAPI::SetComboxCurSel(ComboxCtrl, ItemData);

	ComboxCtrl.MoveWindow(&CtrlRect, FALSE);			
	ComboxCtrl.SetFocus();
	ComboxCtrl.ShowDropDown();
	ComboxCtrl.ShowWindow(SW_SHOW);
	ComboxCtrl.BringWindowToTop();			
	ListCtrl.UpdateWindow();
	ComboxCtrl.Invalidate();	
	return true;
}
//-------------------------------------------------------------------------------------//
void CInputListWnd::OnSelchangeParamCombo() 
{
	// TODO: Add your control notification handler code here
	if ( ExecUpdateParamByCombox() == false )
	{	return; }
	m_ComboxCtrl.ShowWindow(SW_HIDE);	
	JetAPI::ClearCombox(m_ComboxCtrl);	
}
//-------------------------------------------------------------------------------------//
void CInputListWnd::OnKillfocusParamCombo() 
{
	// TODO: Add your control notification handler code here
	if ( m_ComboxCtrl.IsWindowVisible() == FALSE ) { return; }
	ExecUpdateParamByCombox();
	m_ComboxCtrl.ShowWindow(SW_HIDE);	
	JetAPI::ClearCombox(m_ComboxCtrl);	
	return;
}
//-------------------------------------------------------------------------------------//