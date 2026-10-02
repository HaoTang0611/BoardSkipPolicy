// ProjectParamPaneVersionCode.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "ProjectParamPaneVersionCode.h"
//-------------------------------------------------------------------------------------//
#include "InputBoxWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
const int SETTING_COL = 2;
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CProjectParamPaneVersionCode dialog
//-------------------------------------------------------------------------------------//
CProjectParamPaneVersionCode::CProjectParamPaneVersionCode(CWnd* pParent /*=NULL*/)
	: CDialog(CProjectParamPaneVersionCode::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProjectParamPaneVersionCode)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ItemActIdx = -1;
	m_SubItemActIdx = -1;
	m_ProjectPtr = NULL;
	m_ProParameterPtr = NULL;
	m_StopVersionCodeListBeSelected = false;
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneVersionCode::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProjectParamPaneVersionCode)	
	DDX_Control(pDX, PROVERSION_PARAM_EDIT, m_EditCtrl);	
	DDX_Control(pDX, PROVERSION_PARAM_LIST_WND, m_VersionCodeListWnd);	
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CProjectParamPaneVersionCode, CDialog)
	//{{AFX_MSG_MAP(CProjectParamPaneVersionCode)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_EN_KILLFOCUS(PROVERSION_PARAM_EDIT, OnKillfocusParamEdit)	
	ON_NOTIFY(LVN_ITEMCHANGED, PROVERSION_PARAM_LIST_WND, OnItemchangedVersionCodeListWnd)
	ON_NOTIFY(NM_DBLCLK, PROVERSION_PARAM_LIST_WND, OnDblclkVersionCodeListWnd)
	ON_BN_CLICKED(PROVERSION_ADD_CODE_BTN, OnAddCodeBtn)
	ON_BN_CLICKED(PROVERSION_MODIFY_CODE_BTN, OnModifyCodeBtn)
	ON_BN_CLICKED(PROVERSION_COPY_CODE_BTN, OnCopyCodeBtn)
	ON_BN_CLICKED(PROVERSION_DELETE_CODE_BTN, OnDeleteCodeBtn)
	ON_BN_CLICKED(PROVERSION_CLEAR_CODE_BTN, OnClearCodeBtn)
	ON_BN_CLICKED(PROVERSION_ACTIVE_CODE_BTN, OnActiveCodeBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CProjectParamPaneVersionCode message handlers
//-------------------------------------------------------------------------------------//
BOOL CProjectParamPaneVersionCode::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	JetAPI::InitialListCtrl(m_VersionCodeListWnd);

	SwitchMultiLanguage();
	BuildVersionCodeListWndHeader();
	if ( CWnd::IsWindowVisible() )
	{	BuildVersionCodeListWnd(); }
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneVersionCode::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneVersionCode::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if ( m_VersionCodeListWnd.GetSafeHwnd() == NULL ) { return; }
	CWnd *WndPtr = NULL;
	SIZE  WndSize={0};	
	BOOL  bVisible = CWnd::IsWindowVisible();
	const int MarginX = 4;
	const int MarginY = 4;

	if ( m_VersionCodeListWnd.GetSafeHwnd() != NULL )
	{
		RECT WndRect={0};	
		m_VersionCodeListWnd.GetWindowRect(&WndRect);
		CWnd::ScreenToClient(&WndRect);
		WndRect.left = MarginX;
		WndRect.right = cx-MarginX;
		//WndRect.top = MarginY;
		WndRect.bottom = cy-MarginY;
		m_VersionCodeListWnd.MoveWindow(&WndRect, FALSE);		
		if ( TRUE == bVisible )
		{	m_VersionCodeListWnd.Invalidate();	}
	}
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneVersionCode::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if ( TRUE == bShow )
	{	BuildVersionCodeListWnd();	}
}
//-------------------------------------------------------------------------------------//
BOOL CProjectParamPaneVersionCode::PreTranslateMessage(MSG* pMsg)
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
				m_EditCtrl.ShowWindow(SW_HIDE);	
				m_EditCtrl.SetWindowText(_T(""));
				return TRUE;				
			}			
			break;
		case VK_ESCAPE:
			ExecReleaseParamCtrl();
			return TRUE;
			break;
		}
		break;
	}
	return CDialog::PreTranslateMessage(pMsg);
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneVersionCode::OnOK() 
{
	// TODO: Add extra validation here
	return ;
	CDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneVersionCode::OnCancel() 
{
	// TODO: Add extra cleanup here
	return ;
	CDialog::OnCancel();
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneVersionCode::SetProjectParameterPtr(CAOIProject *ProjectPtr, TProjectParameter *Ptr)
{
	m_ProjectPtr = ProjectPtr;
	m_ProParameterPtr = Ptr;
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneVersionCode::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_PROJECT_PARAM_VERSION_CODE_PANE");
	//---------------------------------------------------------------------------------//
	WndID = IDD_PROJECT_PARAM_VERSION_CODE_PANE;
	WndKey = _T("IDD_PROJECT_PARAM_VERSION_CODE_PANE");
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
	WndID = PROVERSION_ADD_CODE_BTN;
	WndKey = _T("PROVERSION_ADD_CODE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROVERSION_MODIFY_CODE_BTN;
	WndKey = _T("PROVERSION_MODIFY_CODE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROVERSION_COPY_CODE_BTN;
	WndKey = _T("PROVERSION_COPY_CODE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROVERSION_DELETE_CODE_BTN;
	WndKey = _T("PROVERSION_DELETE_CODE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROVERSION_CLEAR_CODE_BTN;
	WndKey = _T("PROVERSION_CLEAR_CODE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROVERSION_ACTIVE_CODE_BTN;
	WndKey = _T("PROVERSION_ACTIVE_CODE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
}
//-------------------------------------------------------------------------------------//
CString CProjectParamPaneVersionCode::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_PROJECT_PARAM_VERSION_CODE_PANE");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
CThisListCtrl_51& CProjectParamPaneVersionCode::GetVersionCodeListWnd()
{
	return m_VersionCodeListWnd;
}
//-------------------------------------------------------------------------------------//
int CProjectParamPaneVersionCode::GetVersionCodeListWndItem()
{
	CThisListCtrl_51 &ListCtrl = GetVersionCodeListWnd();
	if ( ListCtrl.GetSafeHwnd() == NULL ) { return -1; }
	return ListCtrl.GetNextItem(-1, LVNI_FOCUSED);
}
//-------------------------------------------------------------------------------------//
bool CProjectParamPaneVersionCode::BuildVersionCodeListWndHeader()
{	
	CString str;
	int   nCol = 0;
	int width  = 64;
	int width2 = 64;
	RECT      Rect;	
	const int Align = LVCFMT_LEFT;//LVCFMT_CENTER;LVCFMT_RIGHT, LVCFMT_LEFT
	const int AlignC = LVCFMT_CENTER;//LVCFMT_CENTER;LVCFMT_RIGHT, LVCFMT_LEFT
	{
		CThisListCtrl_51 &ListCtrl = GetVersionCodeListWnd();

		ListCtrl.GetClientRect(&Rect);
		width = (Rect.right-Rect.left-8)/2;
		width2 = 48;

		str = _T("ID");
		str = AOIDataDefine.GetIDText();
		ListCtrl.InsertColumn(nCol, str, Align, width2);
		nCol ++;

		str = _T("Active");
		str = AOIDataDefine.GetEnableText();		
		ListCtrl.InsertColumn(nCol, str, AlignC, width2);
		nCol ++;

		width2 = width;
		str = _T("Code Name");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width);
		nCol ++;		
	}
	return true;

}
//-------------------------------------------------------------------------------------//
bool CProjectParamPaneVersionCode::BuildVersionCodeListWnd()
{	
	CThisListCtrl_51 &ListCtrl = GetVersionCodeListWnd();
	m_StopVersionCodeListBeSelected = true;
	m_ItemActIdx = -1;
	m_SubItemActIdx = -1;
	JetAPI::ClearListCtrl(ListCtrl, FALSE);	
	m_StopVersionCodeListBeSelected = false;
	if ( NULL == m_ProjectPtr ) { return true; }
	if ( NULL == m_ProParameterPtr ) { return true; }
	
	size_t       i=0;
	CString      str;
	int          nItem=0;
	CString      strAct;
	CString      CodeName;	
	const int    nSubItem1 = 1;
	const int    nSubItem2 = 2;
	TVersionCode* VersoinCodePtr=NULL;
	CAOIProject *ProjectPtr = m_ProjectPtr;			
	const int    ActIndex=m_ProParameterPtr->m_VersionCodeActiveIndex;
	const size_t CodeCount=ProjectPtr->GetProjectVersionCodeCount();
	
	nItem=0;
	ListCtrl.SetRedraw(FALSE);
	m_StopVersionCodeListBeSelected = true;
	for ( i=0; i<CodeCount; i++ )
	{		
		VersoinCodePtr =  ProjectPtr->GetProjectVersionCodePtr(i, false);
		if ( NULL == VersoinCodePtr ) { continue; }
		if ( false == VersoinCodePtr->bEnabled ) { continue; }

		str.Format(_T("%d"), i+1);
		if ( ActIndex != i ) 
		{	strAct = _T("");	}
		else
		{	strAct = _T("*");	}
		CodeName = VersoinCodePtr->wsCodeName.c_str();

		ListCtrl.InsertItem(nItem, str);
		ListCtrl.SetItemData(nItem, i);		
		ListCtrl.SetItemText(nItem, nSubItem1, strAct);
		ListCtrl.SetItemText(nItem, nSubItem2, CodeName);

		nItem++;
	}
	m_StopVersionCodeListBeSelected = false;
	ListCtrl.SetRedraw(TRUE);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectParamPaneVersionCode::ExecReleaseParamCtrl()
{
	m_EditCtrl.ShowWindow(SW_HIDE);	
	m_EditCtrl.SetWindowText(_T(""));
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectParamPaneVersionCode::ExecUpdateParamByEdit()
{
	if ( NULL == m_ProjectPtr ) { return true; }
	if ( NULL == m_ProParameterPtr ) { return true; }	
	const int nItem = m_ItemActIdx;	
	const int nSubItem = m_SubItemActIdx;
	CThisListCtrl_51 &ListCtrl = GetVersionCodeListWnd();	
	const int ItemCount = ListCtrl.GetItemCount();
	if ( nItem<0 || nItem>=ItemCount ) { return false; }
	const int index = (int)(ListCtrl.GetItemData(nItem));
	TVersionCode *VersionCodePtr = m_ProjectPtr->GetProjectVersionCodePtr(index, true);
	if ( NULL == VersionCodePtr ) { return true; }

	CString ItemText;
	m_EditCtrl.GetWindowText(ItemText);	
	JetAPI::TCHAR2wstring(ItemText, VersionCodePtr->wsCodeName);	
	ListCtrl.SetItemText(nItem, nSubItem, ItemText);	
	ListCtrl.SetFocus();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectParamPaneVersionCode::ExecDblclkParamListWnd(CThisListCtrl_51 &ListCtrl, int nItem, int nSubItem)
{	//return true;
	const int    ItemCount = ListCtrl.GetItemCount();
	if ( nItem<0 || nItem>=ItemCount ) { return false; }
	if ( nSubItem != SETTING_COL ) { return true; }
	const size_t ParamIndex = ListCtrl.GetItemData(nItem);	
	
	size_t          i=0;
	int             nSelIdx=0;
	int             nValue=0;
	CRect           ItemRect;
	RECT            CtrlRect={0};	
	CString         ItemText;	
	const int       Offset = 2;		
	if ( ListCtrl.GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, ItemRect) == FALSE )
	{	return false; }	
	ItemText = ListCtrl.GetItemText(nItem, nSubItem);

	CtrlRect = ItemRect;
	ListCtrl.ClientToScreen(&CtrlRect);
	this->ScreenToClient(&CtrlRect);
	::OffsetRect(&CtrlRect, 0, -2);	
	if ( m_EditCtrl.GetSafeHwnd() != NULL )
	{	
		m_ItemActIdx = nItem;
		m_SubItemActIdx = nSubItem;
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
void CProjectParamPaneVersionCode::OnKillfocusParamEdit()
{
	if ( m_EditCtrl.IsWindowVisible() == FALSE ) { return; }
	ExecUpdateParamByEdit();
	m_EditCtrl.ShowWindow(SW_HIDE);	
	m_EditCtrl.SetWindowText(_T(""));
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneVersionCode::OnItemchangedVersionCodeListWnd(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if ( true == m_StopVersionCodeListBeSelected ) { return; }	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) 
	{	return;		}
	DWORD Res = pNMListView->uNewState&LVIS_SELECTED;
	if ( Res == 0 ) { return; }	
	Res = pNMListView->uNewState&LVIS_FOCUSED;
	if ( Res == 0 ) { return; }		
	//ExecItemchangedParamListWnd(m_DropOutParamListCtrl, m_DropOutList, nItem);	
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneVersionCode::OnDblclkVersionCodeListWnd(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMITEMACTIVATE *pNMListView = (NMITEMACTIVATE*)pNMHDR;	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) { return; }
	if ( nSubItem != SETTING_COL ){ return; }
	ExecDblclkParamListWnd(m_VersionCodeListWnd, nItem, nSubItem);
}
//-------------------------------------------------------------------------------------//
bool  CProjectParamPaneVersionCode::ExecAddCodeBtn()
{
	if ( NULL == m_ProjectPtr ) { return true; }
	if ( NULL == m_ProParameterPtr ) { return true; }	
	CThisListCtrl_51 &ListCtrl = GetVersionCodeListWnd();
	
	CString      str;
	CAOIProject *ProjectPtr = m_ProjectPtr;		

	CString      strValue;
	CString      strLabel;
	CString      strCaption;		
	CInputBoxWnd InputBox;
	const size_t NewIdx=ProjectPtr->GetProjectVersionCodeNewIndex();

	str = _T("Set Version Code Name");
	strCaption = LoadMultiLanguageString(str, str);	
	str = _T("Code Name");
	strLabel = LoadMultiLanguageString(str, str);		
	InputBox.SetParam1(strCaption, strLabel, strValue);
	if ( InputBox.DoModal() == IDCANCEL )
	{	return true;	}

	strValue = InputBox.m_DataEdit1;
	if ( ProjectPtr->AddProjectVersionCode(strValue) == false )
	{
		str = ProjectPtr->GetProjectErrorString();
		JetAPI::ShowMessageBox(str);
		return false;
	}
	m_ProParameterPtr->m_VersionCodeActiveIndex = NewIdx;	
	BuildVersionCodeListWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectParamPaneVersionCode::ExecCopyCodeBtn()
{
	if ( NULL == m_ProjectPtr ) { return true; }
	if ( NULL == m_ProParameterPtr ) { return true; }	
	CThisListCtrl_51 &ListCtrl = GetVersionCodeListWnd();
	
	size_t       i=0;
	size_t       idx=0;
	CString      str;
	CString      str2;	
	CAOIProject *ProjectPtr = m_ProjectPtr;	
	const int    nItem=ListCtrl.GetNextItem(-1, LVNI_FOCUSED);
	if ( -1 == nItem ) { return true; }
	const int    index = (int)(ListCtrl.GetItemData(nItem));

	CString      strValue;
	CString      strLabel;
	CString      strCaption;		
	CInputBoxWnd InputBox;
	const size_t NewIdx=ProjectPtr->GetProjectVersionCodeNewIndex();

	str = _T("Set New Version Code Name");
	strCaption = LoadMultiLanguageString(str, str);	
	str = _T("Code Name");
	strLabel = LoadMultiLanguageString(str, str);		
	strValue = ListCtrl.GetItemText(nItem, 2);
	InputBox.SetParam1(strCaption, strLabel, strValue);
	if ( InputBox.DoModal() == IDCANCEL )
	{	return true;	}
	strValue = InputBox.m_DataEdit1;
	
	bool IsOK = true;
	IsOK = ProjectPtr->CopyProjectVersionCode(index, strValue);
	if ( false == IsOK )
	{
		str = ProjectPtr->GetProjectErrorString();
		JetAPI::ShowMessageBox(str);
	}
	else
	{	m_ProParameterPtr->m_VersionCodeActiveIndex = NewIdx;	}
	BuildVersionCodeListWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectParamPaneVersionCode::ExecClearCodeBtn()
{
	if ( NULL == m_ProjectPtr ) { return true; }
	if ( NULL == m_ProParameterPtr ) { return true; }	
	CThisListCtrl_51 &ListCtrl = GetVersionCodeListWnd();
	
	size_t       i=0;
	size_t       idx=0;
	CString      str;
	CString      CodeName;
	CAOIProject *ProjectPtr = m_ProjectPtr;

	str = _T("Do you want to clear all version code?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) != IDYES )
	{	return true; }

	bool IsOK = true;
	IsOK = ProjectPtr->ResetProjectVersionCodeList();
	m_ProParameterPtr->m_VersionCodeActiveIndex = PROJECT_VERSION_CODE_BASE_INDEX;
	if ( false == IsOK )
	{
		str = ProjectPtr->GetProjectErrorString();
		JetAPI::ShowMessageBox(str);
	}
	BuildVersionCodeListWnd();
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CProjectParamPaneVersionCode::ExecDeleteCodeBtn()
{
	if ( NULL == m_ProjectPtr ) { return true; }
	if ( NULL == m_ProParameterPtr ) { return true; }	
	CThisListCtrl_51 &ListCtrl = GetVersionCodeListWnd();
	
	size_t       i=0;
	size_t       idx=0;
	CString      str;
	CString      str2;
	CAOIProject *ProjectPtr = m_ProjectPtr;	
	const int    nItem=ListCtrl.GetNextItem(-1, LVNI_FOCUSED);
	if ( -1 == nItem ) { return true; }
	const int    index = (int)(ListCtrl.GetItemData(nItem));	

	str2 = _T("Do you want to delete the version code");
	str2 = LoadMultiLanguageString(str2, str2);
	str.Format(_T("%s[%02d] ?"), str2, index+1);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) != IDYES )
	{	return true; }

	if ( ProjectPtr->DeleteProjectVersionCode(index) == true )
	{
		if ( m_ProParameterPtr->m_VersionCodeActiveIndex == index )
		{	m_ProParameterPtr->m_VersionCodeActiveIndex = PROJECT_VERSION_CODE_BASE_INDEX; }
	}
	BuildVersionCodeListWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectParamPaneVersionCode::ExecModifyCodeBtn()
{
	if ( NULL == m_ProjectPtr ) { return true; }
	if ( NULL == m_ProParameterPtr ) { return true; }	
	CThisListCtrl_51 &ListCtrl = GetVersionCodeListWnd();
	
	size_t       i=0;
	size_t       idx=0;
	CString      str;		
	CAOIProject *ProjectPtr = m_ProjectPtr;	
	const int    nItem=ListCtrl.GetNextItem(-1, LVNI_FOCUSED);
	if ( -1 == nItem ) { return true; }
	const int    index = (int)(ListCtrl.GetItemData(nItem));	
	TVersionCode *VersionCodePtr = ProjectPtr->GetProjectVersionCodePtr(index, true);
	if ( NULL == VersionCodePtr ) { return true; }

	CString      strValue;
	CString      strLabel;
	CString      strCaption;		
	CInputBoxWnd InputBox;
	str = _T("Set Version Code Name");
	strCaption = LoadMultiLanguageString(str, str);	
	str = _T("Code Name");
	strLabel = LoadMultiLanguageString(str, str);	
	strValue = VersionCodePtr->wsCodeName.c_str();	
	InputBox.SetParam1(strCaption, strLabel, strValue);
	if ( InputBox.DoModal() == IDCANCEL )
	{	return true;	}

	strValue = InputBox.m_DataEdit1;
	JetAPI::TCHAR2wstring(strValue, VersionCodePtr->wsCodeName);	
	BuildVersionCodeListWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectParamPaneVersionCode::ExecActiveCodeBtn()
{
	if ( NULL == m_ProjectPtr ) { return true; }
	if ( NULL == m_ProParameterPtr ) { return true; }	
	CThisListCtrl_51 &ListCtrl = GetVersionCodeListWnd();
	
	size_t       i=0;
	size_t       idx=0;	
	CAOIProject *ProjectPtr = m_ProjectPtr;	
	const int    nItem=ListCtrl.GetNextItem(-1, LVNI_FOCUSED);
	if ( -1 == nItem ) { return true; }
	const int    index = (int)(ListCtrl.GetItemData(nItem));	
	m_ProParameterPtr->m_VersionCodeActiveIndex = index;	
	BuildVersionCodeListWnd();	
	return true;
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneVersionCode::OnAddCodeBtn() 
{
	// TODO: Add your control notification handler code here	
	ExecAddCodeBtn();
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneVersionCode::OnModifyCodeBtn() 
{
	// TODO: Add your control notification handler code here
	ExecModifyCodeBtn();
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneVersionCode::OnCopyCodeBtn() 
{
	// TODO: Add your control notification handler code here
	ExecCopyCodeBtn();
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneVersionCode::OnDeleteCodeBtn() 
{
	// TODO: Add your control notification handler code here
	ExecDeleteCodeBtn();
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneVersionCode::OnClearCodeBtn() 
{
	// TODO: Add your control notification handler code here
	ExecClearCodeBtn();
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneVersionCode::OnActiveCodeBtn() 
{
	// TODO: Add your control notification handler code here
	ExecActiveCodeBtn();
}
//-------------------------------------------------------------------------------------//