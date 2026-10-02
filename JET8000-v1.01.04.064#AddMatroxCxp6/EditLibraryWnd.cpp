// EditLibraryWnd.cpp : implementation file
//

#include "stdafx.h"
#include "jet8000.h"
#include "EditLibraryWnd.h"
//-------------------------------------------------------------------------------------//
#include "InputBoxWnd.h"
#include "InputListWnd.h"
#include "ProjectListWnd.h"
#include "ProjectLibraryWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CEditLibraryWnd
//-------------------------------------------------------------------------------------//
//IMPLEMENT_DYNCREATE(CEditLibraryWnd, CDialog)
//-------------------------------------------------------------------------------------//
CEditLibraryWnd::CEditLibraryWnd(CWnd* pParent /*=NULL*/)
	: CDialog(CEditLibraryWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditLibraryWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ProjectPtr = NULL;
	m_StopModelNameListBeSelected = false;
	m_StopModelTypeListBeSelected = false;
	m_StopModelGroupListBeSelected = false;
}
//-------------------------------------------------------------------------------------//
void CEditLibraryWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditLibraryWnd)
	DDX_Control(pDX, EML_MODEL_LIST_WND, m_ModelIconListWnd);
	DDX_Control(pDX, EML_MODEL_GROUP_LIST_WND, m_ModelGroupListWnd);
	DDX_Control(pDX, EML_MODEL_TYPE_LIST_WND, m_ModelTypeListWnd);	
	DDX_Control(pDX, EML_MODEL_FRAME_INDEX_COMBO, m_ModelFrameIndexCombox);		
	DDX_Control(pDX, EML_MODEL_ICON_SIZE_COMBO, m_ModelIconSizeCombox);			
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CEditLibraryWnd, CDialog)
	//{{AFX_MSG_MAP(CEditLibraryWnd)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_NOTIFY(NM_CLICK, EML_MODEL_TYPE_LIST_WND, OnClickModelTypeListWnd)
	ON_NOTIFY(LVN_ITEMCHANGED, EML_MODEL_TYPE_LIST_WND, OnItemchangedModelTypeListWnd)
	ON_BN_CLICKED(EML_MODEL_GROUP_ADD_BTN, OnModelGroupAddBtn)
	ON_BN_CLICKED(EML_MODEL_GROUP_RENAME_BTN, OnModelGroupRenameBtn)	
	ON_NOTIFY(NM_CLICK, EML_MODEL_GROUP_LIST_WND, OnClickModelGroupListWnd)
	ON_NOTIFY(LVN_ITEMCHANGED, EML_MODEL_GROUP_LIST_WND, OnItemchangedModelGroupListWnd)
	ON_BN_CLICKED(EML_MODEL_ADD_BTN, OnModelAddBtn)
	ON_BN_CLICKED(EML_MODEL_DELETE_BTN, OnModelDeleteBtn)
	ON_NOTIFY(NM_CLICK, EML_MODEL_LIST_WND, OnClickModelListWnd)
	ON_NOTIFY(NM_DBLCLK, EML_MODEL_LIST_WND, OnDbclickModelListWnd)	
	ON_NOTIFY(LVN_ITEMCHANGED, EML_MODEL_LIST_WND, OnItemchangedModelListWnd)
	ON_WM_CONTEXTMENU()
	ON_BN_CLICKED(EML_MODEL_APPLY_BTN, OnModelApplyBtn)
	ON_BN_CLICKED(EML_MODEL_ROTATE_BTN, OnModelRotateBtn)	
	ON_BN_CLICKED(EML_MODEL_IMPORT_BTN, OnModelImportBtn)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(EML_SEARCH_MODEL_BTN, OnSearchModelBtn)
	ON_BN_CLICKED(EML_MODEL_CLONE_BTN, OnModelCloneBtn)
	ON_NOTIFY(NM_DBLCLK, EML_MODEL_TYPE_LIST_WND, OnDblclkModelTypeListWnd)
	ON_BN_CLICKED(EML_ARRANGE_LIBRARY_BTN, OnArrangeLibraryBtn)	
	ON_COMMAND(IDR_MENU_LIBRARY_MODEL_RENAME, OnMenuLibraryModelRename)	
	ON_COMMAND(IDR_MENU_LIBRARY_MODEL_APPLY, OnMenuLibraryModelApply)
	ON_COMMAND(IDR_MENU_LIBRARY_MODEL_CLONE, OnMenuLibraryModelClone)
	ON_COMMAND(IDR_MENU_LIBRARY_MODEL_ROTATE_090, OnMenuLibraryModelRotate090)
	ON_COMMAND(IDR_MENU_LIBRARY_MODEL_ROTATE_180, OnMenuLibraryModelRotate180)
	ON_COMMAND(IDR_MENU_LIBRARY_MODEL_ROTATE_270, OnMenuLibraryModelRotate270)
	ON_COMMAND(IDR_MENU_LIBRARY_MODEL_DELETE, OnMenuLibraryModelDelete)	
	ON_COMMAND(IDR_MENU_LIBRARY_MODEL_GROUP_NAME, OnMenuLibraryModelGroupName)	
	ON_COMMAND(IDR_MENU_LIBRARY_MODEL_CHANGE_TYPE, OnMenuLibraryModelChangeType)	
	ON_COMMAND(IDR_MENU_LIBRARY_MODEL_BK_IMAGE_INDEX, OnMenuLibraryModelBKImageIndex)
	ON_COMMAND(IDR_MENU_LIBRARY_MODEL_SAVE_SERVER, OnMenuLibraryModelSaveServer)	
	ON_COMMAND(IDR_MENU_LIBRARY_MODEL_LOAD_SERVER, OnMenuLibraryModelLoadServer)
	ON_CBN_SELCHANGE(EML_MODEL_FRAME_INDEX_COMBO, OnSelchangeModelBKImageIndexCombox)
	ON_CBN_SELCHANGE(EML_MODEL_ICON_SIZE_COMBO, OnSelchangeModelIconSizeCombox)
	ON_BN_CLICKED(EML_MODEL_GROUP_NAME_BTN, OnModelGroupNameBtn)		
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CEditLibraryWnd diagnostics
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
void CEditLibraryWnd::AssertValid() const
{
	CDialog::AssertValid();
}
//-------------------------------------------------------------------------------------//
void CEditLibraryWnd::Dump(CDumpContext& dc) const
{
	CDialog::Dump(dc);
}
#endif //_DEBUG
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CEditLibraryWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CEditLibraryWnd::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add your specialized code here and/or call the base class
	TSystemParameter &SysParam=AOIDataCollect.GetSystemParameter();

	SwitchMultiLanguage();

	BuildModelIconSizeCombox();
	CWnd::CheckDlgButton(EML_SEARCH_MODEL_NAME_CHK, TRUE);
	CWnd::SetDlgItemInt(EML_SEARCH_MODEL_SIZE_W_EDIT, 1000);
	CWnd::SetDlgItemInt(EML_SEARCH_MODEL_SIZE_H_EDIT, 1000);
	CWnd::SetDlgItemInt(EML_SEARCH_MODEL_SIZE_TOL_EDIT, 500);

	JetAPI::InitialListCtrl(m_ModelIconListWnd);
	InitModelGroupListCtrl(m_ModelGroupListWnd);	
	BuildModelTypeListCtrl(m_ModelTypeListWnd);
	AOIDataDefine.BuildProjectMapIndexCombox(m_ModelFrameIndexCombox);	
	JetAPI::SetComboxCurSel(m_ModelFrameIndexCombox, 0);
	CWnd::CheckDlgButton(EML_MODEL_USE_PART_NUMBER_CHK, SysParam.m_ModelNameUsePartNumber);
	return TRUE;
}
//-------------------------------------------------------------------------------------//
void CEditLibraryWnd::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if ( CWnd::GetSafeHwnd() == NULL ) { return; }
	
	if ( m_ModelTypeListWnd.GetSafeHwnd() != NULL )
	{
		RECT WndRect={0};
		m_ModelTypeListWnd.GetWindowRect(&WndRect);
		CWnd::ScreenToClient(&WndRect);
		WndRect.right = cx;		
		m_ModelTypeListWnd.MoveWindow(&WndRect);
		m_ModelTypeListWnd.Arrange(LVA_ALIGNTOP);//LVA_ALIGNTOP
	}
	if ( m_ModelGroupListWnd.GetSafeHwnd() != NULL )
	{
		RECT WndRect={0};
		m_ModelGroupListWnd.GetWindowRect(&WndRect);
		CWnd::ScreenToClient(&WndRect);
		WndRect.bottom = cy;
		m_ModelGroupListWnd.MoveWindow(&WndRect);
	}

	if ( m_ModelIconListWnd.GetSafeHwnd() != NULL )
	{
		RECT WndRect={0};
		m_ModelIconListWnd.GetWindowRect(&WndRect);
		CWnd::ScreenToClient(&WndRect);
		WndRect.right = cx;
		WndRect.bottom = cy;
		m_ModelIconListWnd.MoveWindow(&WndRect);
		m_ModelIconListWnd.Arrange(LVA_ALIGNTOP);//LVA_ALIGNTOP
	}
}
//-------------------------------------------------------------------------------------//
void CEditLibraryWnd::OnDestroy() 
{
	CDialog::OnDestroy();

	// TODO: Add your message handler code here	
	ClearModelTypeListCtrl(m_ModelTypeListWnd);
}
//-------------------------------------------------------------------------------------//
void CEditLibraryWnd::OnOK() 
{
	// TODO: Add extra validation here
	return;
	CDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CEditLibraryWnd::OnCancel() 
{
	// TODO: Add extra cleanup here
	return;	
	CDialog::OnCancel();
}
//-------------------------------------------------------------------------------------//
void CEditLibraryWnd::OnClickModelTypeListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CEditLibraryWnd::OnItemchangedModelTypeListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
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
void CEditLibraryWnd::OnModelGroupAddBtn() 
{
	// TODO: Add your control notification handler code here
	const bool bPartNumberName=CheckUsePartNumberName();
	ExecModelGroupAdd(bPartNumberName);	
}
//-------------------------------------------------------------------------------------//
void CEditLibraryWnd::OnModelGroupRenameBtn() 
{
	// TODO: Add your control notification handler code here
	ExecModelGroupRename();
}
//-------------------------------------------------------------------------------------//
void CEditLibraryWnd::OnClickModelGroupListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CEditLibraryWnd::OnItemchangedModelGroupListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
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
void CEditLibraryWnd::OnModelAddBtn() 
{
	// TODO: Add your control notification handler code here
	const bool bPartNumberName=CheckUsePartNumberName();
	ExecModelAdd(bPartNumberName);
}
//-------------------------------------------------------------------------------------//
void CEditLibraryWnd::OnModelDeleteBtn() 
{
	// TODO: Add your control notification handler code here
	ExecModelDelete();
}
//-------------------------------------------------------------------------------------//
void CEditLibraryWnd::OnClickModelListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here	
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CEditLibraryWnd::OnDbclickModelListWnd(NMHDR* pNMHDR, LRESULT* pResult)
{
	//ExecModelApply();
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CEditLibraryWnd::OnItemchangedModelListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
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
	CWnd::SetDlgItemText(EML_SEARCH_MODEL_NAME_SELECTED_EDIT, strModelName);

	CAOIModel *ModelPtr = (CAOIModel*)m_ModelIconListWnd.GetItemData(nItem);
	if ( NULL == ModelPtr )
	{	AOIDataCollect.DestroyModelPreViewPtr();	}
	else
	{	
		AOIDataCollect.CreateModelPreViewPtr(ModelPtr);	
		AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_REDRAW_VIEW_WND, NULL);	
		AOIDataCollect.PostCallbackWndMessage(MSG_SELF_WND_EXTRA_MESSAGE, WPARAM_FIRST_UI_CALLBACK, NULL);
	}
	
	//BuildModelIconListCtrl
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CEditLibraryWnd::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	if ( NULL != pWnd )
	{
		UINT CtrlID = pWnd->GetDlgCtrlID();
		switch ( CtrlID )
		{
		case EML_MODEL_LIST_WND:
			ExecModelMenu();
			break;
		case EML_MODEL_GROUP_LIST_WND:
			break;
		case EML_MODEL_TYPE_LIST_WND:
			break;
		}		
	}	
}
//-------------------------------------------------------------------------------------//
void CEditLibraryWnd::SwitchMultiLanguage()
{
	size_t  i = 0;	
	UINT    WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_EDIT_LIBRARY_WND");	
	//---------------------------------------------------------------------------------//
	WndID = IDD_EDIT_LIBRARY_WND;
	WndKey = _T("IDD_EDIT_LIBRARY_WND");
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
	WndID = EML_MODEL_GROUP_LABEL;
	WndKey = _T("EML_MODEL_GROUP_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = EML_MODEL_GROUP_ADD_BTN;
	WndKey = _T("EML_MODEL_GROUP_ADD_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = EML_MODEL_GROUP_RENAME_BTN;
	WndKey = _T("EML_MODEL_GROUP_RENAME_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		
	//---------------------------------------------------------------------------------//	
	WndID = EML_MODEL_GROUP;
	WndKey = _T("EML_MODEL_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = EML_MODEL_ADD_BTN;
	WndKey = _T("EML_MODEL_ADD_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = EML_MODEL_CLONE_BTN;
	WndKey = _T("EML_MODEL_CLONE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = EML_MODEL_DELETE_BTN;
	WndKey = _T("EML_MODEL_DELETE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = EML_MODEL_APPLY_BTN;
	WndKey = _T("EML_MODEL_APPLY_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = EML_MODEL_ROTATE_BTN;
	WndKey = _T("EML_MODEL_ROTATE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = EML_MODEL_IMPORT_BTN;
	WndKey = _T("EML_MODEL_IMPORT_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = EML_MODEL_GROUP_NAME_BTN;
	WndKey = _T("EML_MODEL_GROUP_NAME_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = EML_MODEL_USE_PART_NUMBER_CHK;
	WndKey = _T("EML_MODEL_USE_PART_NUMBER_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//	
	WndID = EML_SEARCH_MODEL_GROUP;
	WndKey = _T("EML_SEARCH_MODEL_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = EML_SEARCH_MODEL_NAME_CHK;
	WndKey = _T("EML_SEARCH_MODEL_NAME_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = EML_SEARCH_MODEL_SIZE_CHK;
	WndKey = _T("EML_SEARCH_MODEL_SIZE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = EML_SEARCH_MODEL_BTN;
	WndKey = _T("EML_SEARCH_MODEL_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		
	//---------------------------------------------------------------------------------//	
	WndID = EML_ARRANGE_LIBRARY_BTN;
	WndKey = _T("EML_ARRANGE_LIBRARY_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = EML_MODEL_FRAME_INDEX_LABEL;
	WndKey = _T("EML_MODEL_FRAME_INDEX_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = EML_MODEL_ICON_SIZE_LABEL;
	WndKey = _T("EML_MODEL_ICON_SIZE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		
	//---------------------------------------------------------------------------------//		
}
//-------------------------------------------------------------------------------------//
CString CEditLibraryWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_EDIT_LIBRARY_WND");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
void CEditLibraryWnd::CloseProject()
{
	m_ProjectPtr = NULL;
	//ClearModelTypeListCtrl(m_ModelTypeListWnd);			
	ClearModelGroupListCtrl(m_ModelGroupListWnd);
	ClearModelIconListCtrl(m_ModelIconListWnd);			
}
//-------------------------------------------------------------------------------------//
CAOIProject* CEditLibraryWnd::GetActiveProject()
{
	return m_ProjectPtr;
}
//-------------------------------------------------------------------------------------//
void CEditLibraryWnd::UpdateActiveProject()
{
	CloseProject();
	m_ProjectPtr = AOIDataCollect.GetActiveProject();
}
//-------------------------------------------------------------------------------------//
void CEditLibraryWnd::SetActiveProject(CAOIProject* Ptr)
{
	m_ProjectPtr = Ptr;
}
//-------------------------------------------------------------------------------------//
UINT CEditLibraryWnd::GetModelTypeIcon(MODEL_TYPE ModelType, bool Small)
{
	return AOIDataDefine.GetModelTypeIcon(ModelType, Small);	
}
//-------------------------------------------------------------------------------------//
LRESULT CEditLibraryWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	CWnd *pWnd = NULL;
	switch ( message )
	{
	case MSG_MAIN_FRAME_MESSAGE:
		switch ( wParam )
		{
		case WPARAM_PROJECT_NEW:
		case WPARAM_PROJECT_OPEN:
		case WPARAM_PROJECT_SWITCH:			
			UpdateActiveProject();			
			break;
		case WPARAM_PROJECT_UPDATE:
			if ( CWnd::IsWindowVisible() == TRUE )
			{
				pWnd = (CWnd*)lParam;
				if ( pWnd != this )
				{	UpdateActiveProject();	}
			}
			break;		
		case WPARAM_PROJECT_CLOSE:
			CloseProject();			
			break;		
		case WPARAM_PROJECT_CLOSE_ACTIVE_OBJ:
			break;
		}
		break;
	case MSG_EDIT_LIBRARY_WND:
		switch ( wParam )
		{
		case WPARAM_UPDATE_MODEL_LIST_ICON:
			//ExecToActiveComponent();
			UpdateModelIconListCtrl(m_ModelIconListWnd);
			break;
		case WPARAM_UPDATE_USE_PARTNUMBER_CHK:
			CWnd::CheckDlgButton(EML_MODEL_USE_PART_NUMBER_CHK, lParam);
			break;
		}
		break;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
bool CEditLibraryWnd::CheckUsePartNumberName()//確認是否使用料號名稱
{
	bool bPartNumber=false;
	BOOL bCheck=CWnd::IsDlgButtonChecked(EML_MODEL_USE_PART_NUMBER_CHK);
	if ( FALSE == bCheck )
	{	bPartNumber=false; }
	else
	{	bPartNumber = true;	}
	return bPartNumber;	
}
//-------------------------------------------------------------------------------------//
void CEditLibraryWnd::BuildModelIconSizeCombox()
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
bool CEditLibraryWnd::ClearModelTypeListCtrl(CListCtrl &ListCtrl)
{
	m_StopModelTypeListBeSelected = true;
	JetAPI::ClearListCtrl(ListCtrl, FALSE);
	m_StopModelTypeListBeSelected = false;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditLibraryWnd::BuildModelTypeListCtrl(CListCtrl &ListCtrl)
{
	//this->CloseProject();
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
bool CEditLibraryWnd::SetModelTypeListCtrlItemSlected(MODEL_TYPE ModelType)
{
	size_t       i=0;
	const size_t Count = m_ModelTypeListWnd.GetItemCount();
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
bool CEditLibraryWnd::InitModelGroupListCtrl(CListCtrl &ListCtrl)
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
bool CEditLibraryWnd::ClearModelGroupListCtrl(CListCtrl &ListCtrl)
{
	m_StopModelGroupListBeSelected = true;
	JetAPI::ClearListCtrl(ListCtrl, FALSE);
	m_StopModelGroupListBeSelected = false;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditLibraryWnd::BuildModelGroupListCtrl(CListCtrl &ListCtrl, MODEL_TYPE ModelType, bool bBuildIconList)
{		
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
	{
		BuildModelIconListCtrl(ModelType, ModelGroupList[ActiveIndex], m_ModelIconListWnd);
	}	
	return TRUE;
}
//-------------------------------------------------------------------------------------//
bool CEditLibraryWnd::SetModelGroupListCtrlItemSlected(LPCTSTR  GroupName)
{	
	size_t       i=0;
	CString      strItem;
	const size_t Count = m_ModelGroupListWnd.GetItemCount();
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
bool CEditLibraryWnd::AddModelIconListItem(CAOIModel *ModelPtr)
{
	if ( NULL == ModelPtr ) { return false; }
	CListCtrl   &ListCtrl = m_ModelIconListWnd;
	CImageList  &ImageList = m_ModelIconImageList;

	CString strModelName=ModelPtr->GetModelName();
	const int nItem = ListCtrl.GetItemCount();
	const int nImage = ImageList.GetImageCount();

	strModelName.MakeUpper();
	ListCtrl.InsertItem(nItem, strModelName, nImage);
	ListCtrl.SetItemData(nItem, (DWORD_PTR)ModelPtr);
	ListCtrl.SetItemText(nItem, 0, strModelName);
	ListCtrl.SetItemState(nItem, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	ListCtrl.EnsureVisible(nItem, FALSE);
	ListCtrl.SetFocus();

	CWnd::SetDlgItemText(EML_SEARCH_MODEL_NAME_SELECTED_EDIT, strModelName);
	const int nNextItem = ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditLibraryWnd::ClearModelIconListCtrl(CListCtrl &ListCtrl)
{
	m_StopModelNameListBeSelected = true;
	JetAPI::ClearListCtrl(ListCtrl, FALSE);
	CWnd::SetDlgItemInt(EML_GROUP_MODEL_COUNT_EDIT, 0);
	m_StopModelNameListBeSelected = false;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditLibraryWnd::UpdateModelIconListCtrl(CListCtrl &ListCtrl)
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
bool  CEditLibraryWnd::BuildModelIconListCtrl(MODEL_TYPE ModelType, LPCTSTR  GroupName, CListCtrl &ListCtrl)
{	
	ClearModelIconListCtrl(ListCtrl);
	CAOIProject *Project = GetActiveProject();
	if ( NULL == Project )
	{	return true;	}
	
	UINT         TypeIcon=IDB_MODEL_EMPTY_M_ICON;
	CString      TypeName;	
	const int    IconW = JetAPI::GetComboxCurSelData(m_ModelIconSizeCombox);//128;//144;//80
	const int    IconH = JetAPI::GetComboxCurSelData(m_ModelIconSizeCombox);//128;//144;//80
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
	CWnd::SetDlgItemInt(EML_GROUP_MODEL_COUNT_EDIT, ItemCount);

	ListCtrl.SetRedraw(TRUE);
	ListCtrl.Invalidate();
	ListCtrl.UpdateWindow();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditLibraryWnd::SetModelIconListCtrlItemSlected(LPCTSTR  ModelName)
{
	size_t       i=0;
	CString      strItem;
	const size_t Count = m_ModelIconListWnd.GetItemCount();
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
bool CEditLibraryWnd::ExecModelGroupAdd(bool bPartNumberName)
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }
	if ( AOIDataCollect.OperateLevelEditFuncAddModel() == false ) { return false; }

	TREGION4D rgnBody;
	double    AttachedAngle=0;
	CString   strModelName;
	CString   strPartNumber;
	CString   LibraryFolder = ProjectPtr->GetProjectLibraryFolder();
	CString   GroupName = _T("NewGroup");		
	CAOIComponent *pComponent = ProjectPtr->GetProjectActiveComponent();
	if ( NULL != pComponent )
	{
		AttachedAngle = pComponent->GetComponentAngle();
		const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(AttachedAngle);
		CAOIModel *ModelPtr_C = pComponent->GetComponentModelPtr();
		strModelName = pComponent->GetComponentModelName();
		strPartNumber = pComponent->GetComponentPartNumber();
		if ( true == bPartNumberName )
		{	GroupName = strPartNumber; }
		else
		{	GroupName = strModelName; }		
		GroupName = strModelName;//rgnBody
		if ( NULL != ModelPtr_C )
		{
			if ( true == IsExceptionAngle )
			{
				TPOINT2D CornerPos[4];
				ModelPtr_C->GetModelBodyBox().GetBoxCornerPos(CornerPos);
				JetAPI::RotateCornerPos(-AttachedAngle, CornerPos);
				JetAPI::CornerPtToRegion(CornerPos, rgnBody);
			}
			else
			{	
				TPOINT2D  Cp;
				TREGION4D rgnTmp;
				ModelPtr_C->GetModelBodyBox().GetBoxRegion(rgnBody);
				
				rgnTmp = rgnBody;
				Cp.x = rgnBody.GetCpX();
				Cp.y = rgnBody.GetCpY();
				JetAPI::RotateRegion(-AttachedAngle, Cp.x, Cp.y, rgnTmp, rgnBody);
			}
		}
	}
	const int nTypeItem = m_ModelTypeListWnd.GetNextItem(-1, LVNI_SELECTED);
	if ( nTypeItem <0 ) { return true; }

	int Count = 0;	
	CHIP_SIZE_MODE ChipSizeMode=CHIP_SIZE_NONE;
	MODEL_TYPE ModelType = (MODEL_TYPE)(m_ModelTypeListWnd.GetItemData(nTypeItem));

	if ( CAOIModel::CheckModelTypUseChipSizeMode(ModelType) == true )
	{	ChipSizeMode = CAOIModel::FindModelChipSizeMode(ModelType, rgnBody);	}

	Count = 0;
	while ( true )
	{	
		if ( ProjectPtr->CheckProjectModelGroupNameExist(GroupName) == true ) 
		{	
			Count ++;
			GroupName.Format(_T("NewGroup%d"), Count);
			continue; 
		}	
		break;
	};	
	
	CString      str;
	CString      strLabel;
	CString      strCaption;
	CString      strChipSize;
	CInputBoxWnd InputBox;
	POINT  WndCp={0};
	RECT   WndRect={0};
	CWnd::GetWindowRect(&WndRect);
	WndCp.x = (WndRect.left+WndRect.right)/2;
	WndCp.y = (WndRect.top+WndRect.bottom)/2;	
	InputBox.SetWndPos(WndCp);

	str = _T("Input Group Name");
	str = LoadMultiLanguageString(str, str);
	if ( CHIP_SIZE_NONE==ChipSizeMode || CHIP_SIZE_OTHERS==ChipSizeMode ) 
	{	strCaption = str;	}
	else
	{
		strChipSize = CAOIModel::GetModelChipSizeModeText(ChipSizeMode);
		strCaption.Format(_T("%s [%s]"), str, strChipSize);
	}
	strLabel = _T("Name:");
	strLabel = LoadMultiLanguageString(strLabel, strLabel);
	while ( true )
	{		
		InputBox.SetParam1(strCaption, strLabel, GroupName);
		if ( InputBox.DoModal() == IDCANCEL ) { return true; }

		GroupName = InputBox.m_DataEdit1;	
		GroupName.MakeUpper();
		GroupName.TrimLeft();//剔除左邊空白
		GroupName.TrimRight();//剔除右邊空白
		if ( GroupName.GetLength() == 0 )
		{	continue; }
		if ( ProjectPtr->CheckProjectModelGroupNameExist(ModelType, GroupName) == true ) 
		{
			GroupName = InputBox.m_DataEdit1;
			continue; 
		}		
		break;
	};	
	GroupName = InputBox.m_DataEdit1;	
	CAOIModel *ModelPtr = AOIObjManager.CreateModelObj();
	if ( NULL == ModelPtr ) 
	{
		str = AOIObjManager.GetErrorString();
		JetAPI::ShowMessageBox(str);
		return false; 
	}
	
	CString ModelFolder;
	CString ModelName = GroupName;	
	std::wstring GroupNameM=L"";
	std::wstring ModelNameM=L"";
	unsigned int DefaultFrameIndex=0;
	unsigned int DefaultFrameUniqueID = 0;	
	std::vector<unsigned int> FrameIndexMapList;
	ProjectPtr->BuildProjectFrameIndexMapParam(FrameIndexMapList, DefaultFrameIndex, DefaultFrameUniqueID);		
	
	if ( true == bPartNumberName )
	{	ModelName = strPartNumber; }
	else
	{	ModelName = strModelName; }
	GroupName.MakeUpper();
	ModelName.MakeUpper();	

	strCaption = _T("Input Model Name");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strLabel = _T("Name:");
	strLabel = LoadMultiLanguageString(strLabel, strLabel);
	while ( true ) 
	{
		InputBox.SetParam1(strCaption, strLabel, ModelName);
		if ( InputBox.DoModal() == IDCANCEL )
		{
			AOIObjManager.DestroyModelObj(ModelPtr);
			return true; 
		}

		ModelName = InputBox.m_DataEdit1;	
		ModelName.MakeUpper();
		ModelName.TrimLeft();//剔除左邊空白
		ModelName.TrimRight();//剔除右邊空白
		if ( ModelName.GetLength() == 0 )
		{	continue; }
		if ( ProjectPtr->CheckProjectModelNameExist(ModelName) == true ) 
		{
			ModelName = InputBox.m_DataEdit1;
			continue; 
		}		
		break;
	};

	ModelFolder.Format(_T("%s\\%s"), LibraryFolder, ModelName);
	JetAPI::CreateFolder(ModelFolder);

	JetAPI::TCHAR2wstring(GroupName, GroupNameM);
	JetAPI::TCHAR2wstring(ModelName, ModelNameM);
	ModelPtr->SetModelGroupName(GroupNameM.c_str());
	ModelPtr->SetModelName(ModelNameM.c_str());
	ModelPtr->SetModelFolderModel(ModelFolder);

	double     ComAngle=0;		
	double     BodySizeW=500;
	double     BodySizeH=500;
	if ( NULL != pComponent )
	{	
		TREGION4D  ModelBodyRgn;		
		ComAngle = pComponent->GetComponentAngle();
		BodySizeW = pComponent->GetComponentBodySizeW();
		BodySizeH = pComponent->GetComponentBodySizeH();
		JetAPI::RotateSize(-ComAngle, BodySizeW, BodySizeH);		

		BodySizeW = rgnBody.GetWidth();
		BodySizeH = rgnBody.GetHeight();
	}
	ModelPtr->SetupkModelModifiedDateTime();
	ModelPtr->BuildModelType(ModelType, BodySizeW, BodySizeH);
	ModelPtr->CalcModelTotalRegionAll();
	ModelPtr->UpdateModelFrameIndex(DefaultFrameIndex, DefaultFrameUniqueID, FrameIndexMapList);
	ProjectPtr->AddProjectModelPtr(ModelPtr, false);	
	const int nItem = m_ModelGroupListWnd.GetItemCount();
	m_ModelGroupListWnd.InsertItem(nItem, GroupName);
	m_ModelGroupListWnd.SetItemText(nItem, 0, GroupName);	
	m_ModelGroupListWnd.SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED);
	m_ModelGroupListWnd.EnsureVisible(nItem, FALSE);
	BuildModelIconListCtrl(ModelType, GroupName, m_ModelIconListWnd);
	m_ModelGroupListWnd.SetFocus();

	if ( NULL != pComponent )
	{
		ProjectPtr->SelectProjectAllComponents(false);
		AOIDataCollect.SetDrawModelMode(DRAW_MODEL_EDIT);		
		if ( true == bPartNumberName )
		{	ProjectPtr->SelectProjectComponentsByPartNumber(strPartNumber, true);  }
		else
		{	ProjectPtr->SelectProjectComponentsByModelName(strModelName, true);  }
		ProjectPtr->ApplyProjectModelToComponentsSelected(ModelName);
		pComponent->GetComponentModelPtr()->SetModelBodyBoxActived(true);
		TSIZE2D ImageSizeUm=pComponent->GetComponentFrameImageSize_um();
		pComponent->SetComponentFrameImageSize_um(ImageSizeUm);
		AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_UPDATE, (LPARAM)this);
		AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_SHOW_PROJECT_LIBRARY_DOCK_PANE, FALSE);	
		AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_PART_LIST_WND, WPARAM_BUILD_PART_LIST, LPARAM_BUILD_DOCK_LIST_EDIT);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditLibraryWnd::OnModelApplyBtn() 
{
	// TODO: Add your control notification handler code here
	ExecModelApply();
}
//-------------------------------------------------------------------------------------//
void CEditLibraryWnd::OnModelRotateBtn()
{
	CString      str;
	CString      strValue;
	CString      strLabel;
	CString      strCaption;
	CInputBoxWnd InputBox;
	const double Precision = DBL_PRECISION;
	strCaption = _T("Input Model Rotate Angle Wnd");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strLabel = _T("Rotate Angle:");
	strLabel = LoadMultiLanguageString(strLabel, strLabel);
	strValue = _T("0");
	InputBox.SetParam1(strCaption, strLabel, strValue);
	if ( InputBox.DoModal() == IDCANCEL ) 
	{	return ;  }

	strValue = InputBox.m_DataEdit1;
	const double Angle = JetAPI::StrToDbl(strValue);
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(Angle);
	if ( true ==IsExceptionAngle )
	{
		str = _T("Error, Can not rotate Model to this angle");
		str = LoadMultiLanguageString(str, str);
		JetAPI::ShowMessageBox(str);
		return;
	}
	if ( fabs(Angle)<Precision || fabs(Angle-360.0)<Precision ) { return ; }
	ExecModelRotate(Angle);
}
//-------------------------------------------------------------------------------------//
void CEditLibraryWnd::OnModelImportBtn()
{
	ExecModelImport();
}
//-------------------------------------------------------------------------------------//
bool CEditLibraryWnd::ExecModelGroupRename()
{
	CAOIProject *Project = GetActiveProject();
	if ( NULL == Project ) { return true; }	
	
	int Count = 0;	
	const int nTypeItem = m_ModelTypeListWnd.GetNextItem(-1, LVNI_SELECTED);
	const int nGroupItem = m_ModelGroupListWnd.GetNextItem(-1, LVNI_SELECTED);
	if ( nTypeItem<0 ||nGroupItem<0 ) { return true; }
	MODEL_TYPE ModelType = (MODEL_TYPE)(m_ModelTypeListWnd.GetItemData(nTypeItem));
	CString srcGroupName = m_ModelGroupListWnd.GetItemText(nGroupItem, 0);
	
	CString strLabel;
	CString strCaption;
	CInputBoxWnd InputBox;
	CString NewGroupName = srcGroupName;
	POINT  WndCp={0};
	RECT   WndRect={0};
	CWnd::GetWindowRect(&WndRect);
	WndCp.x = (WndRect.left+WndRect.right)/2;
	WndCp.y = (WndRect.top+WndRect.bottom)/2;	
	InputBox.SetWndPos(WndCp);
	strCaption = _T("Rename Group Name");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strLabel = _T("Name:");
	strLabel = LoadMultiLanguageString(strLabel, strLabel);
	while ( true )
	{		
		InputBox.SetParam1(strCaption, strLabel, NewGroupName);
		if ( InputBox.DoModal() == IDCANCEL ) { return true; }

		NewGroupName = InputBox.m_DataEdit1;
		NewGroupName.MakeUpper();
		NewGroupName.TrimLeft();//剔除左邊空白
		NewGroupName.TrimRight();//剔除右邊空白
		if ( NewGroupName.GetLength() == 0 )
		{	continue; }
		if ( NewGroupName == srcGroupName ) { return true; }		
		if ( Project->CheckProjectModelGroupNameExist(NewGroupName) == true ) 
		{
			NewGroupName = InputBox.m_DataEdit1;
			continue; 
		}		
		break;
	};	
	NewGroupName = InputBox.m_DataEdit1;

	Project->RenameProjectModelGroupName(ModelType, srcGroupName, NewGroupName);	
	m_ModelGroupListWnd.SetItemText(nGroupItem, 0, NewGroupName);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditLibraryWnd::ExecModelMenu()
{
	POINT point;
	UINT menuID = 0;
	::GetCursorPos(&point);
	menuID = IDR_MENU_LIBRARY_MODEL;
	if ( 0 == menuID ) { return false; }
	
	CMenu menu;
	VERIFY(menu.LoadMenu(menuID));
	AOIDataCollect.SwitchMultiLanguageMenu(menu, menuID);

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);

	CWnd* pWndPopupOwner = this;
	while (pWndPopupOwner->GetStyle() & WS_CHILD)
	{	pWndPopupOwner = pWndPopupOwner->GetParent();	}

	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditLibraryWnd::ExecModelAdd(bool bPartNumberName)
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }	
	CAOIComponent *pComponent = ProjectPtr->GetProjectActiveComponent();
	if ( NULL == pComponent ) { return true; }
	if ( AOIDataCollect.OperateLevelEditFuncAddModel() == false ) {	return false; }

	ProjectPtr->SelectProjectAllComponents(false);//關閉其他被選到的零件
	pComponent->SetComponentSelected(true);	
	
	int Count = 0;		
	const int nTypeItem = m_ModelTypeListWnd.GetNextItem(-1, LVNI_SELECTED);
	const int nGroupItem = m_ModelGroupListWnd.GetNextItem(-1, LVNI_SELECTED);
	CString   strGroupAllName = AOIDataDefine.GetModelGroupAllText();
	if ( nTypeItem<0 ||nGroupItem<0 ) { return true; }
	MODEL_TYPE ModelType = (MODEL_TYPE)(m_ModelTypeListWnd.GetItemData(nTypeItem));
	CString srcGroupName = m_ModelGroupListWnd.GetItemText(nGroupItem, 0);
	srcGroupName.MakeUpper();
	if ( strGroupAllName.CompareNoCase(srcGroupName) == 0 ) { return true; }
	
	CString str;
	CString strTitle;
	CString strCaption;
	CString strModelName;
	CString strFullComponentName = pComponent->GetComponentFullName();
	if ( true == bPartNumberName )
	{	strModelName = pComponent->GetComponentPartNumber();	}
	else
	{	strModelName = pComponent->GetComponentModelName();	}
	strModelName.MakeUpper();

	POINT WndCp={0};
	RECT  WndRect={0};
	CInputBoxWnd InputBox;	
	CString NewModelName = strModelName;

	CWnd::GetWindowRect(&WndRect);
	WndCp.x = (WndRect.left+WndRect.right)/2;
	WndCp.y = (WndRect.top+WndRect.bottom)/2;	
	InputBox.SetWndPos(WndCp);

	strTitle = _T("Add Mode Name");
	strTitle = LoadMultiLanguageString(strTitle, strTitle);
	str = _T("Model Name");
	str = LoadMultiLanguageString(str, str);
	strCaption.Format(_T("%s[%s]: "), str, strFullComponentName);
	while ( true )
	{	
		InputBox.SetParam1(strTitle, strCaption, NewModelName);		
		if ( InputBox.DoModal() == IDCANCEL ) { return true; }

		NewModelName = InputBox.m_DataEdit1;
		NewModelName.MakeUpper();
		NewModelName.TrimLeft();//剔除左邊空白
		NewModelName.TrimRight();//剔除右邊空白
		if ( NewModelName.GetLength() == 0 )
		{	continue; }
		if ( ProjectPtr->CheckProjectModelNameExist(NewModelName) == true ) 
		{	continue; }		
		break;
	};

	strModelName = NewModelName;
	strModelName.MakeUpper();	

	double     RoatedAngle = 0.0;
	bool       bCloneModel = false;
	CAOIModel *ModelPtrM = NULL;	
	std::wstring GroupNameM=L"";
	std::wstring ModelNameM=L"";
	CString ModelName;
	CString ModelFolder;
	CString LibraryFolder = ProjectPtr->GetProjectLibraryFolder();
	CString strModelNameC = pComponent->GetComponentModelName();
	CString strPartNumber = pComponent->GetComponentPartNumber();	
	const double ComponentAngle = pComponent->GetComponentAngle();
	CAOIModel *ModelPtrC = pComponent->GetComponentModelPtr();
	MODEL_TYPE ModelTypeC = ModelPtrC->GetModelType();
	unsigned int   DefaultFrameIndex=0;
	unsigned int   DefaultFrameUniqueID = 0;	
	std::vector<unsigned int> FrameIndexMapList;
	ProjectPtr->BuildProjectFrameIndexMapParam(FrameIndexMapList, DefaultFrameIndex, DefaultFrameUniqueID);		

	if ( MODEL_TYPE_NULL==ModelTypeC || ModelType!=ModelTypeC )
	{
		RoatedAngle = 0;
		bCloneModel = false;
		ModelPtrM = AOIObjManager.CreateModelObj();	
	}
	else
	{
		RoatedAngle = ComponentAngle;
		bCloneModel = true;
		ModelPtrM = ModelPtrC->CloneModelObj();		
	}
	if ( NULL == ModelPtrM ) { return FALSE; }	

	JetAPI::TCHAR2wstring(srcGroupName, GroupNameM);
	JetAPI::TCHAR2wstring(strModelName, ModelNameM);

	ModelName = ModelNameM.c_str();
	ModelFolder.Format(_T("%s\\%s"), LibraryFolder, ModelName);
	JetAPI::CreateFolder(ModelFolder);

	ModelPtrM->SetModelGroupName(GroupNameM.c_str());
	ModelPtrM->SetModelName(ModelNameM.c_str());
	ModelPtrM->SetModelFolderModel(ModelFolder);		

	if ( false == bCloneModel )
	{	
		double     ComAngle=0;
		double     BodySizeW=500;
		double     BodySizeH=500;
		if ( NULL != pComponent )
		{	
			TREGION4D  ModelBodyRgn;		
			ComAngle = pComponent->GetComponentAngle();
			BodySizeW = pComponent->GetComponentBodySizeW();
			BodySizeH = pComponent->GetComponentBodySizeH();
			JetAPI::RotateSize(-ComAngle, BodySizeW, BodySizeH);			
		}
		ModelPtrM->BuildModelType(ModelType, BodySizeW, BodySizeH);
	}
	else
	{
		CString OldModelFolder;
		const double DBL_PRESION = 0.001;
		OldModelFolder.Format(_T("%s\\%s"), LibraryFolder, strModelNameC);
		JetAPI::CopyFolderAToFolderB(OldModelFolder, ModelFolder, false, false, _T(""), -1, -1);
		ModelPtrM->AssignModelFolder();
		if ( ::fabs(RoatedAngle) > DBL_PRESION )
		{	ModelPtrM->RotateModel(-RoatedAngle, 0, 0); }

		ModelPtrM->ClearModelAllObjList();
		ModelPtrM->RemoveModelImageFolder();//先行清除舊有的樣版資料夾
		ModelPtrM->CalcModelTotalRegionAll();		
	}
	ModelPtrM->UnSelectModel();
	ModelPtrM->SetupkModelModifiedDateTime();
	ModelPtrM->GetModelBodyBox().SetBoxSelected(true);
	ModelPtrM->CalcModelTotalRegionAll();	
	ModelPtrM->UpdateModelFrameIndex(DefaultFrameIndex, DefaultFrameUniqueID, FrameIndexMapList);
	ProjectPtr->AddProjectModelPtr(ModelPtrM, false);	

	if ( MODEL_TYPE_NULL == ModelTypeC )
	{
		ProjectPtr->SelectProjectAllComponents(false);
		if ( true == bPartNumberName )
		{	ProjectPtr->SelectProjectComponentsByPartNumber(strPartNumber, true); }
		else
		{	ProjectPtr->SelectProjectComponentsByModelName(strModelNameC, true); }
		ProjectPtr->ApplyProjectModelToComponentsSelected(ModelPtrM);
		ProjectPtr->SelectProjectAllComponents(false);
		pComponent->SetComponentSelected(true);	
		pComponent->GetComponentModelPtr()->SetModelBodyBoxActived(true);
	}
	else
	{	
		pComponent->SetComponentSelected(true);
		ProjectPtr->ApplyProjectModelToComponentsSelected(ModelPtrM);
		pComponent->GetComponentModelPtr()->SetModelBodyBoxActived(true);
	}
	TSIZE2D ImageSizeUm=pComponent->GetComponentFrameImageSize_um();
	pComponent->SetComponentFrameImageSize_um(ImageSizeUm);

	const int nLastItem = m_ModelIconListWnd.GetNextItem(-1, LVNI_SELECTED);
	if ( nLastItem >= 0 ) 
	{	m_ModelIconListWnd.SetItemState(nLastItem, 0, LVIS_SELECTED);	}	
	AddModelIconListItem(ModelPtrM);
	AOIDataCollect.SetDrawModelMode(DRAW_MODEL_EDIT);
	AOIDataCollect.PostCallbackWndMessage(WM_COMMAND, MENU_MODEL_EDIT_CREATE_BK_IMAGE, NULL);
	AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_UPDATE, (LPARAM)this);	
	AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_SHOW_PROJECT_LIBRARY_DOCK_PANE, FALSE);	
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_PART_LIST_WND, WPARAM_BUILD_PART_LIST, LPARAM_BUILD_DOCK_LIST_EDIT);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditLibraryWnd::ExecModelClone(bool bPartNumberName)//複製模組
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }	
	CAOIComponent *pComponent = ProjectPtr->GetProjectActiveComponent();
	if ( NULL == pComponent ) { return false; }
	const int ModelIconIndex = m_ModelIconListWnd.GetNextItem(-1, LVNI_SELECTED);
	if ( ModelIconIndex < 0 ) { return false; }
	CAOIModel *ModelPtrL = (CAOIModel*)(m_ModelIconListWnd.GetItemData(ModelIconIndex));	
	if ( ModelPtrL->IsKindOf(RUNTIME_CLASS(CAOIModel)) == FALSE ) { return false; }
	MODEL_TYPE ModelTypeL = ModelPtrL->GetModelType();	
	if ( MODEL_TYPE_NULL == ModelTypeL ) { return false; }
	CString   strModelNameL = ModelPtrL->GetModelName();

	MODEL_TYPE ModelType = ModelTypeL;
	CString   strGroupName = ModelPtrL->GetModelGroupName();

	int Count = 0;
	size_t  i = 0;
	CString str;	
	CString strLable;
	CString strCaption;	
	CString strModelName;
	CString strFullComponentName = pComponent->GetComponentFullName();
	if ( true == bPartNumberName )
	{	strModelName = pComponent->GetComponentPartNumber();	}
	else
	{	strModelName = pComponent->GetComponentModelName();	}
	strModelName.MakeUpper();
	
	DWORD Res=0;
	POINT WndCp={0};
	RECT  WndRect={0};
	bool  bReplaceModel=false;
	CInputBoxWnd InputBox;	
	CString NewModelName = strModelName;

	CWnd::GetWindowRect(&WndRect);
	WndCp.x = (WndRect.left+WndRect.right)/2;
	WndCp.y = (WndRect.top+WndRect.bottom)/2;	
	InputBox.SetWndPos(WndCp);

	strCaption = _T("Input Model Name");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	str = _T("Model Name");
	str = LoadMultiLanguageString(str, str);
	strLable.Format(_T("%s[%s]: "), str, strFullComponentName);
	bReplaceModel = false;
	while ( true )
	{	
		InputBox.SetParam1(strCaption, strLable, NewModelName);		
		if ( InputBox.DoModal() == IDCANCEL ) { return true; }

		bReplaceModel= false;
		NewModelName = InputBox.m_DataEdit1;
		NewModelName.MakeUpper();		
		NewModelName.TrimLeft();//剔除左邊空白
		NewModelName.TrimRight();//剔除右邊空白
		if ( NewModelName.GetLength() == 0 )
		{	continue; }
		if ( ProjectPtr->CheckProjectModelNameExist(NewModelName) == true ) 
		{
			if ( strModelNameL.CompareNoCase(NewModelName) == 0 ) 
			{	return true; }

			str.Format(_T("The model exist already, do you want to replace it?"));
			str = LoadMultiLanguageString(str, str);
			Res = JetAPI::ShowMessageBox(str, MB_YESNOCANCEL);
			if ( IDCANCEL == Res )
			{	return true; }
			if ( IDNO == Res )
			{	continue;  }			
			bReplaceModel = true;
		}		
		break;
	};

	strModelName = NewModelName;
	strModelName.MakeUpper();	
	
	double     RoatedAngle = 0.0;
	bool       bCloneModel = false;
	CAOIModel *ModelPtrM = NULL;	
	std::wstring GroupNameM=L"";
	std::wstring ModelNameM=L"";
	CString ModelName;
	CString ModelFolder;
	CString LibraryFolder = ProjectPtr->GetProjectLibraryFolder();
	CString strModelNameC = pComponent->GetComponentModelName();
	CString strPartNumber = pComponent->GetComponentPartNumber();	
	const double ComponentAngle = pComponent->GetComponentAngle();
	CAOIModel *ModelPtrC = pComponent->GetComponentModelPtr();
	MODEL_TYPE ModelTypeC = ModelPtrC->GetModelType();

	RoatedAngle = 0;
	bCloneModel = true;
	ModelPtrM = ModelPtrL->CloneModelObj();		
	if ( NULL == ModelPtrM ) { return false; }

	JetAPI::TCHAR2wstring(strGroupName, GroupNameM);
	JetAPI::TCHAR2wstring(strModelName, ModelNameM);

	ModelName = ModelNameM.c_str();
	ModelFolder.Format(_T("%s\\%s"), LibraryFolder, ModelName);
	JetAPI::CreateFolder(ModelFolder);

	ModelPtrM->SetModelNeedSaveFiles(true);
	ModelPtrM->SetModelBKImageNeedToGrab(true);
	ModelPtrM->SetModelGroupName(GroupNameM.c_str());
	ModelPtrM->SetModelName(ModelNameM.c_str());
	ModelPtrM->SetModelFolderModel(ModelFolder);		

	if ( false == bCloneModel )
	{
		double     ComAngle=0;
		double     BodySizeW=500;
		double     BodySizeH=500;
		if ( NULL != pComponent )
		{	
			TREGION4D  ModelBodyRgn;		
			ComAngle = pComponent->GetComponentAngle();
			BodySizeW = pComponent->GetComponentBodySizeW();
			BodySizeH = pComponent->GetComponentBodySizeH();
			JetAPI::RotateSize(-ComAngle, BodySizeW, BodySizeH);			
		}
		ModelPtrM->BuildModelType(ModelType, BodySizeW, BodySizeH);	
	}
	else
	{		
		CString OldModelName;
		CString OldModelFolder;
		CString OldModelBKName;
		CString NewModelFolder;
		CString NewModelBKName;
		const double DBL_PRESION = 0.001;
		NewModelFolder = ModelFolder;
		OldModelName = ModelPtrL->GetModelName();		
		OldModelFolder.Format(_T("%s\\%s"), LibraryFolder, strModelNameL);		
  		JetAPI::CopyFolderAToFolderB(OldModelFolder, NewModelFolder, false, true, _T(""), -1, -1);
		//更改底圖名字
		for ( i=0; i<FRAME_MAX_COUNT; i++ )
		{				
			OldModelBKName = AOIDataDefine.GetModelBKImageFilename(NewModelFolder, OldModelName, i);
			NewModelBKName = AOIDataDefine.GetModelBKImageFilename(NewModelFolder, ModelName, i);
			//::MoveFileEx(OldModelBKName, NewModelBKName, MOVEFILE_REPLACE_EXISTING);
			if ( OldModelBKName.CompareNoCase(NewModelBKName) == 0 ) { continue; }			
			::MoveFile(OldModelBKName, NewModelBKName);
			//::CopyFile(OldModelBKName, NewModelBKName, FALSE);
			//::DeleteFile(OldModelBKName);
		}

		if ( ::fabs(RoatedAngle) > DBL_PRESION )
		{	ModelPtrM->RotateModel(-RoatedAngle, 0, 0); }		
	}	
	ModelPtrM->UnSelectModel();
	ModelPtrM->AssignModelFolder();
	ModelPtrM->SetModelBodyBoxActived(true);	
	ModelPtrM->SetupkModelModifiedDateTime();
	ModelPtrM->CalcModelTotalRegionAll();
	if ( false == bReplaceModel )
	{	ProjectPtr->AddProjectModelPtr(ModelPtrM, false); }
	else
	{	
		if ( ProjectPtr->ReplaceProjectModel(ModelPtrM) == false )
		{
			AOIObjManager.DestroyModelObj(ModelPtrM);
			return false;
		}
	}	

	if ( false == bReplaceModel )
	{
		if ( MODEL_TYPE_NULL == ModelTypeC )
		{
			ProjectPtr->SelectProjectAllComponents(false);
			if ( true == bPartNumberName )
			{	ProjectPtr->SelectProjectComponentsByPartNumber(strPartNumber, false); }
			else
			{	ProjectPtr->SelectProjectComponentsByModelName(strModelNameC, false); }
			ProjectPtr->ApplyProjectModelToComponentsSelected(ModelPtrM);
			ProjectPtr->SelectProjectAllComponents(false);
			pComponent->SetComponentSelected(true);	
		}
		else
		{	
			std::vector<CAOIComponent*> SelComponentList;
			str = _T("Do you want to apply to the same part number components?");
			str = LoadMultiLanguageString(str, str);
			if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDYES )
			{
				size_t i=0;
				CString strPartNumberC;
				CAOIComponent *SelComponentPtr = NULL;
				std::set<CString> PartNumberList;
				std::set<CString>::iterator iter;
				ProjectPtr->GetProjectComponentSelected(SelComponentList);
				const size_t SelCount = SelComponentList.size();

				for ( i=0; i<SelCount; i++ )
				{
					SelComponentPtr = SelComponentList[i];
					if ( NULL == SelComponentPtr ) { continue; }
					strPartNumberC = SelComponentPtr->GetComponentPartNumber();
					PartNumberList.insert(strPartNumberC);
				}
		
				//選取相同料號
				for ( iter=PartNumberList.begin(); iter!=PartNumberList.end(); iter++ )
				{	
					strPartNumberC = *iter;
					ProjectPtr->SelectProjectComponentsByPartNumber(strPartNumberC, false);
				}	
			}
			else
			{	pComponent->SetComponentSelected(true); }
			ProjectPtr->ApplyProjectModelToComponentsSelected(ModelPtrM);	
		}
	}
	else
	{
		ProjectPtr->SelectProjectAllComponents(false);
		ProjectPtr->SelectProjectComponentsByModelName(ModelName, false);
		ProjectPtr->ApplyProjectModelToComponentsSelected(ModelPtrM);
		ProjectPtr->SelectProjectAllComponents(false);
		pComponent->SetComponentSelected(true);	
	}
	
	const int nLastItem = m_ModelIconListWnd.GetNextItem(-1, LVNI_SELECTED);
	if ( nLastItem >= 0 ) 
	{	m_ModelIconListWnd.SetItemState(nLastItem, 0, LVIS_SELECTED);	}
	AddModelIconListItem(ModelPtrM);	
	AOIDataCollect.SetDrawModelMode(DRAW_MODEL_EDIT);
	AOIDataCollect.PostCallbackWndMessage(WM_COMMAND, MENU_MODEL_EDIT_CREATE_BK_IMAGE, NULL);
	AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_UPDATE, (LPARAM)this);
	AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_SHOW_PROJECT_LIBRARY_DOCK_PANE, FALSE);	
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_PART_LIST_WND, WPARAM_BUILD_PART_LIST, LPARAM_BUILD_DOCK_LIST_EDIT);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditLibraryWnd::ExecModelRename()
{
	CAOIProject *Project = GetActiveProject();
	if ( NULL == Project ) { return true; }	

	int Count = 0;	
	const int nTypeItem = m_ModelTypeListWnd.GetNextItem(-1, LVNI_SELECTED);
	const int nModelItem = m_ModelIconListWnd.GetNextItem(-1, LVNI_SELECTED);
	if ( nTypeItem<0 ||nModelItem<0 ) { return true; }
	MODEL_TYPE ModelType = (MODEL_TYPE)(m_ModelTypeListWnd.GetItemData(nTypeItem));
	CAOIModel *ModelPtr = (CAOIModel*)(m_ModelIconListWnd.GetItemData(nModelItem));
	if ( NULL == ModelPtr ) { return true;  }
	if ( ModelPtr->IsKindOf(RUNTIME_CLASS(CAOIModel)) == FALSE ) { return true; }
	
	CString      strLabel;
	CString      strCaption;
	CInputBoxWnd InputBox;
	CString srcModelName = ModelPtr->GetModelName();
	CString NewModelName = srcModelName;
	POINT  WndCp={0};
	RECT   WndRect={0};
	CWnd::GetWindowRect(&WndRect);
	WndCp.x = (WndRect.left+WndRect.right)/2;
	WndCp.y = (WndRect.top+WndRect.bottom)/2;	
	InputBox.SetWndPos(WndCp);

	strCaption = _T("Input Model Name");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strLabel = _T("Name:");
	strLabel = LoadMultiLanguageString(strLabel, strLabel);
	while ( true )
	{		
		InputBox.SetParam1(strCaption, strLabel, NewModelName);
		if ( InputBox.DoModal() == IDCANCEL ) { return true; }

		NewModelName = InputBox.m_DataEdit1;
		NewModelName.MakeUpper();
		NewModelName.TrimLeft();//剔除左邊空白
		NewModelName.TrimRight();//剔除右邊空白
		if ( NewModelName.GetLength() == 0 )
		{	continue; }
		if ( NewModelName == srcModelName ) { return true; }
		if ( Project->CheckProjectModelNameExist(NewModelName) == true ) 
		{	continue; }		
		break;
	};	
	NewModelName = InputBox.m_DataEdit1;	
	NewModelName.MakeUpper();
	
	Project->RenameProjectModel(srcModelName, NewModelName);	
	m_ModelIconListWnd.SetItemText(nModelItem, 0, NewModelName);
	AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_UPDATE, (LPARAM)this);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditLibraryWnd::ExecModelDelete()
{
	CAOIProject *Project = GetActiveProject();
	if ( NULL == Project ) { return true; }	
	if ( AOIDataCollect.OperateLevelEditFuncDelModel() == false ) {	return false; }

	int Count = 0;	
	CString str;
	CString str2;	
	bool IncludeComponents = true;
	const int nTypeItem = m_ModelTypeListWnd.GetNextItem(-1, LVNI_SELECTED);
	const int nModelItem = m_ModelIconListWnd.GetNextItem(-1, LVNI_SELECTED);
	if ( nTypeItem<0 ||nModelItem<0 ) { return TRUE; }
	MODEL_TYPE ModelType = (MODEL_TYPE)(m_ModelTypeListWnd.GetItemData(nTypeItem));
	CAOIModel *ModelPtr = (CAOIModel*)(m_ModelIconListWnd.GetItemData(nModelItem));
	if ( NULL == ModelPtr ) { return TRUE;  }
	if ( ModelPtr->IsKindOf(RUNTIME_CLASS(CAOIModel)) == FALSE ) { return true; }	
	
	DWORD   Res=0;
	CString srcModelName = ModelPtr->GetModelName();
	CString strModelFolder = ModelPtr->GetModelFolderModel();
	srcModelName.MakeUpper();

	str = _T("Do you want to delete the components linked the Model");
	str = LoadMultiLanguageString(str, str);
	str2.Format(_T("%s [%s]?"), str, srcModelName);

	Res = JetAPI::ShowMessageBox(str2, MB_YESNOCANCEL|MB_DEFBUTTON2);
	if ( IDCANCEL == Res ) 
	{	return true; }

	if ( IDYES == Res )
	{	IncludeComponents = true;	}
	else
	{	IncludeComponents = false; }
	
	Project->SelectProjectAllModels(false);	
	ModelPtr->SetModelSelected(true);
	Project->DeleteProjectModelSelected();
	JetAPI::RemoveFolder(strModelFolder);

	Project->SelectProjectAllComponents(false);
	Project->SelectProjectComponentsByModelName(srcModelName, false);
	if ( true == IncludeComponents )
	{
		AOIDataCollect.ReleaseModelUniFrameList();
		LogOperCtrl.SaveLogProjectComponentSelectedDelete(Project);
		Project->DeleteProjectComponentSelected();	
	}	
	else
	{	Project->ResetProjectComponentModelSelected();	}

	m_ModelIconListWnd.DeleteItem(nModelItem);	
	//AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_UPDATE, (LPARAM)this);
	AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_PART_DELETED, (LPARAM)this);		
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_PART_LIST_WND, WPARAM_BUILD_PART_LIST, LPARAM_BUILD_DOCK_LIST_ALL);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditLibraryWnd::ExecModelApply()//套用模組至零件上
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }	
	CAOIComponent *pComponent = ProjectPtr->GetProjectActiveComponent();
	if ( NULL == pComponent ) { return true; }

	int Count = 0;	
	const bool IncludeComponents = true;
	const int nTypeItem = m_ModelTypeListWnd.GetNextItem(-1, LVNI_SELECTED);
	const int nModelItem = m_ModelIconListWnd.GetNextItem(-1, LVNI_SELECTED);
	if ( nTypeItem<0 ||nModelItem<0 ) { return TRUE; }
	MODEL_TYPE ModelType = (MODEL_TYPE)(m_ModelTypeListWnd.GetItemData(nTypeItem));
	CAOIModel *ModelPtr = (CAOIModel*)(m_ModelIconListWnd.GetItemData(nModelItem));
	if ( NULL == ModelPtr ) { return TRUE;  }
	if ( ModelPtr->IsKindOf(RUNTIME_CLASS(CAOIModel)) == FALSE ) { return true; }
	
	ModelPtr->UnSelectModel();
	ModelPtr->SetModelBodyBoxActived(true);	

	CString str;
	CString srcModelName = ModelPtr->GetModelName();	
	CString strPartNumber = pComponent->GetComponentPartNumber();
	srcModelName.MakeUpper();
	AOIDataCollect.SetDrawModelMode(DRAW_MODEL_EDIT);

	std::vector<CAOIComponent*> SelComponentList;
	str = _T("Do you want to apply to the same part number components?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDYES )
	{
		size_t i=0;
		CString strPartNumberC;
		CAOIComponent *SelComponentPtr = NULL;
		std::set<CString> PartNumberList;
		std::set<CString>::iterator iter;
		ProjectPtr->GetProjectComponentSelected(SelComponentList);
		const size_t SelCount = SelComponentList.size();

		for ( i=0; i<SelCount; i++ )
		{
			SelComponentPtr = SelComponentList[i];
			if ( NULL == SelComponentPtr ) { continue; }
			strPartNumberC = SelComponentPtr->GetComponentPartNumber();
			PartNumberList.insert(strPartNumberC);
		}
		
		//選取相同料號
		for ( iter=PartNumberList.begin(); iter!=PartNumberList.end(); iter++ )
		{	
			strPartNumberC = *iter;
			ProjectPtr->SelectProjectComponentsByPartNumber(strPartNumberC, false);
		}	
	}
	
	ProjectPtr->ApplyProjectModelToComponentsSelected(srcModelName);	
	AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_UPDATE, (LPARAM)this);
	AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_SHOW_PROJECT_LIBRARY_DOCK_PANE, FALSE);	
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_PART_LIST_WND, WPARAM_BUILD_PART_LIST, LPARAM_BUILD_DOCK_LIST_EDIT);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditLibraryWnd::ExecModelRotate(double Angle)//旋轉模組至零件上
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }
	const int ModelIconIndex = m_ModelIconListWnd.GetNextItem(-1, LVNI_SELECTED);
	if ( ModelIconIndex < 0 ) { return false; }
	CAOIModel *ModelPtrL = (CAOIModel*)(m_ModelIconListWnd.GetItemData(ModelIconIndex));	
	if ( ModelPtrL->IsKindOf(RUNTIME_CLASS(CAOIModel)) == FALSE ) { return false; }
	if ( ModelPtrL->CheckModelTypeEnabled() == false )
	{	return false; }

	CString   strModelNameL = ModelPtrL->GetModelName();
	ModelPtrL->RotateModel(Angle, 0, 0);
	ModelPtrL->RotateModelBKImage(Angle);
	ModelPtrL->SetModelNeedSaveFiles(true);
	ModelPtrL->SetModelBKImageNeedToGrab(true);
	AOIDataCollect.SetDrawModelMode(DRAW_MODEL_EDIT);
	ProjectPtr->SelectProjectComponentsByModelName(strModelNameL, false);
	ProjectPtr->ApplyProjectModelToComponentsSelected(ModelPtrL);	
	AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_UPDATE, (LPARAM)this);
	AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_SHOW_PROJECT_LIBRARY_DOCK_PANE, FALSE);	
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_PART_LIST_WND, WPARAM_BUILD_PART_LIST, LPARAM_BUILD_DOCK_LIST_EDIT);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditLibraryWnd::ExecModelBKImageIndex()//變更模組底圖影像編號
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }
	const int ModelIconIndex = m_ModelIconListWnd.GetNextItem(-1, LVNI_SELECTED);
	if ( ModelIconIndex < 0 ) { return false; }
	CAOIModel *ModelPtrL = (CAOIModel*)(m_ModelIconListWnd.GetItemData(ModelIconIndex));	
	if ( ModelPtrL->IsKindOf(RUNTIME_CLASS(CAOIModel)) == FALSE ) { return false; }
	if ( ModelPtrL->CheckModelTypeEnabled() == false )
	{	return false; }
	
	size_t          i=0;
	POINT           Point;
	CString         strLabel;
	CString         strCaption;
	TListNode       Node;	
	CInputListWnd   EnumWnd;
	std::vector<TListNode> NodelList;	
	std::vector<TFrameParam>  ParamList;	
	ProjectPtr->CloneProjectFrameParamList(ParamList);	
	
	const size_t FrameCount = ParamList.size();
	const DWORD_PTR OldImageIndex = ModelPtrL->GetModelBKImageIndex();

	strLabel = _T("Image Index");
	strLabel = LoadMultiLanguageString(strLabel, strLabel);
	strCaption = _T("Set Model BK Image Index");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	for ( i=0; i<FrameCount; i++ )
	{
		Node.Data = i;
		Node.Text = ParamList[i].FrameName;
		NodelList.push_back(Node);
	}
	::GetCursorPos(&Point);
	EnumWnd.SetWndPos(Point);
	EnumWnd.SetParam1(strCaption, strLabel, OldImageIndex, NodelList);
	if ( EnumWnd.GetSelIndex1() < 0 ) 
	{	EnumWnd.SetSelIndex1(0); }	
	if ( EnumWnd.DoModal() == IDCANCEL )
	{	return true; }

	const int NewImageIndex = (int)(EnumWnd.GetSelData());	
	ModelPtrL->SetModelBKImageIndex(NewImageIndex);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditLibraryWnd::ExecModelImport()//模組匯入
{
	CString  str, str2;
	CProjectListWnd ProjectListWnd;	
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }
	if ( ProjectListWnd.DoModal() == IDCANCEL ) { return true; }
	CString filename = ProjectListWnd.GetSelectedFilename();	
	CString ProjectName = ProjectPtr->GetProjectShowName();
	if ( ProjectName.CompareNoCase(filename) == 0 ) 
	{
		str = _T("Error, Can not import the model from current project!");
		str = LoadMultiLanguageString(str, str);
		JetAPI::ShowMessageBox(str);
		return false;
	}

	CAOIProject *ImportProjectPtr = AOIObjManager.CreateProjectObj();
	if ( NULL == ImportProjectPtr ) 
	{ 
		str = AOIObjManager.GetErrorString();
		JetAPI::ShowMessageBox(str);
		return false; 
	}		
	
	CString Folder;		
	CString tmpfilename;	
	const bool bLibraryMode = true;
	if ( AOIDataCollect.CreateTempProjectFile(filename, tmpfilename) == false )
	{
		AOIObjManager.DestroyProjectObj(ImportProjectPtr);
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return false;
	}
	if ( ImportProjectPtr->LoadProject(tmpfilename, filename, bLibraryMode) == false )
	{	
		AOIObjManager.DestroyProjectObj(ImportProjectPtr);
		JetAPI::ShowMessageBox(ImportProjectPtr->GetErrorString());
		return false;
	}		
	
	size_t                   i=0;
	DWORD                    Res=0;
	CString                  strLabel;
	CString                  strCaption;	
	CAOIModel               *ModelPtr = NULL;
	MODEL_TYPE               ModelType=MODEL_TYPE_NULL;
	const int nTypeItem = m_ModelTypeListWnd.GetNextItem(-1, LVNI_SELECTED);
	if ( nTypeItem >= 0 )
	{	ModelType = (MODEL_TYPE)(m_ModelTypeListWnd.GetItemData(nTypeItem));	}

	CProjectLibraryWnd LibraryWnd;
	LibraryWnd.SetModelType(ModelType);
	LibraryWnd.SetActiveProject(ImportProjectPtr);
	if ( LibraryWnd.DoModal() == IDCANCEL )
	{
		AOIObjManager.DestroyProjectObj(ImportProjectPtr);
		return true;
	}

	ModelPtr = LibraryWnd.GetModelPtr();
	if ( NULL == ModelPtr )
	{
		str = _T("Error, NULL == ModelPtr");
		JetAPI::ShowMessageBox(str);
		AOIObjManager.DestroyProjectObj(ImportProjectPtr);
		return false; 
	}
	if ( ImportProjectPtr->CheckProjectModelValid(ModelPtr) == false )
	{
		str = _T("Error, Project CheckProjectModelValid Fault");
		JetAPI::ShowMessageBox(str);
		AOIObjManager.DestroyProjectObj(ImportProjectPtr);
		return false; 
	}

	bool         bReplaceModel=false;
	CString      GroupName;
	CString      OldModelName;
	CString      NewModelName;	
	CInputBoxWnd InputBox;		
	OldModelName = ModelPtr->GetModelName();
	GroupName    = ModelPtr->GetModelGroupName();
	CAOIComponent *ComponentPtr = ProjectPtr->GetProjectActiveComponent();
	if ( NULL == ComponentPtr ) 
	{	NewModelName = OldModelName; }
	else
	{	NewModelName = ComponentPtr->GetComponentModelName(); }

	strCaption = _T("Input Model Name");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strLabel = _T("Model Name");
	strLabel = LoadMultiLanguageString(strLabel, strLabel);
	while ( true )
	{	
		InputBox.SetParam1(strCaption, strLabel, NewModelName);		
		if ( InputBox.DoModal() == IDCANCEL ) 
		{
			AOIObjManager.DestroyProjectObj(ImportProjectPtr);
			return true; 
		}
		bReplaceModel= false;
		NewModelName = InputBox.m_DataEdit1;
		NewModelName.MakeUpper();		
		if ( ProjectPtr->CheckProjectModelNameExist(NewModelName) == true ) 
		{
			str.Format(_T("The model exist already, do you want to replace it?"));
			str = LoadMultiLanguageString(str, str);
			Res = JetAPI::ShowMessageBox(str, MB_YESNOCANCEL);
			if ( IDCANCEL == Res )
			{
				AOIObjManager.DestroyProjectObj(ImportProjectPtr);
				return true; 
			}
			if ( IDNO == Res )
			{	continue;  }			
			bReplaceModel = true;
		}
		break;
	};
	
	ModelPtr = ModelPtr->CloneModelObj();
	if ( NULL == ModelPtr )
	{
		str = _T("Error, Clone Model Object Fault");
		JetAPI::ShowMessageBox(str);
		AOIObjManager.DestroyProjectObj(ImportProjectPtr);
		return false;
	}
	
	CString OldModelFolder;
	CString OldModelBKName;
	CString NewModelFolder;
	CString NewModelBKName;
	CString NewLibraryFolder = ProjectPtr->GetProjectLibraryFolder();
	CString OldLibraryFolder = ImportProjectPtr->GetProjectLibraryFolder();
	const double DBL_PRESION = 0.001;	
	OldModelFolder.Format(_T("%s\\%s"), OldLibraryFolder, OldModelName);
	NewModelFolder.Format(_T("%s\\%s"), NewLibraryFolder, NewModelName);
  	JetAPI::CopyFolderAToFolderB(OldModelFolder, NewModelFolder, false, true, _T(""), -1, -1);
	//更改底圖名字
	for ( i=0; i<FRAME_MAX_COUNT; i++ )
	{	
		OldModelBKName = AOIDataDefine.GetModelBKImageFilename(NewModelFolder, OldModelName, i);
		NewModelBKName = AOIDataDefine.GetModelBKImageFilename(NewModelFolder, NewModelName, i);		
		if ( OldModelBKName.CompareNoCase(NewModelBKName) == 0 ) { continue; }			
		::MoveFile(OldModelBKName, NewModelBKName);
		//::CopyFile(OldModelBKName, NewModelBKName, FALSE);		
	}	
	AOIObjManager.DestroyProjectObj(ImportProjectPtr);
	ImportProjectPtr = NULL;

	//更新符合專案的模組參數		
	unsigned int   DefaultFrameIndex=0;
	unsigned int   DefaultFrameUniqueID = 0;	
	std::vector<CColorGroup>  ColorGroupList;
	std::vector<unsigned int> FrameIndexMapList;
	ProjectPtr->CloneProjectColorGroupList(ColorGroupList);
	ProjectPtr->BuildProjectFrameIndexMapParam(FrameIndexMapList, DefaultFrameIndex, DefaultFrameUniqueID);			
	
	ModelPtr->UnSelectModel();
	ModelPtr->SetModelName(NewModelName);
	ModelPtr->SetModelFolderModel(NewModelFolder);
	ModelPtr->AssignModelFolder();
	ModelPtr->SetupkModelModifiedDateTime();
	ModelPtr->CalcModelTotalRegionAll();
	ModelPtr->SetModelNeedSaveFiles(true);
	ModelPtr->SetModelBodyBoxActived(true);			
	ModelPtr->SetModelBKImageNeedToGrab(true);	
	ModelPtr->UpdateModelColorGroupLinkIndex(ColorGroupList);
	ModelPtr->UpdateModelFrameIndex(DefaultFrameIndex, DefaultFrameUniqueID, FrameIndexMapList);
	if ( false == bReplaceModel )
	{	ProjectPtr->AddProjectModelPtr(ModelPtr, false); }
	else
	{	
		if ( ProjectPtr->ReplaceProjectModel(ModelPtr) == false )
		{
			AOIObjManager.DestroyModelObj(ModelPtr);			
			return false;
		}
	}
	
	ProjectPtr->SelectProjectAllComponents(false);
	ProjectPtr->SelectProjectComponentsByModelName(NewModelName, false);
	ProjectPtr->ApplyProjectModelToComponentsSelected(ModelPtr);
	ProjectPtr->SelectProjectAllComponents(false);
	if ( NULL != ComponentPtr )
	{	ComponentPtr->SetComponentSelected(true); 	}	

	ModelType = ModelPtr->GetModelType();
	GroupName = ModelPtr->GetModelGroupName();	
	BuildModelGroupListCtrl(m_ModelGroupListWnd, ModelType, false);
	BuildModelIconListCtrl(ModelType, GroupName, m_ModelIconListWnd);

	SetModelTypeListCtrlItemSlected(ModelType);
	SetModelGroupListCtrlItemSlected(GroupName);
	SetModelIconListCtrlItemSlected(NewModelName);
	m_ModelIconListWnd.SetFocus();
	CWnd::SetDlgItemText(EML_SEARCH_MODEL_NAME_SELECTED_EDIT, NewModelName);
	//AOIDataCollect.CreateModelPreViewPtr(ModelPtr);
	//AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_REDRAW_VIEW_WND, NULL);	
	AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_UPDATE, (LPARAM)this);
	AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_SHOW_PROJECT_LIBRARY_DOCK_PANE, FALSE);	
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_PART_LIST_WND, WPARAM_BUILD_PART_LIST, LPARAM_BUILD_DOCK_LIST_EDIT);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditLibraryWnd::ExecModelSaveToServer()//儲存模組至伺服器
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }
	const int ModelIconIndex = m_ModelIconListWnd.GetNextItem(-1, LVNI_SELECTED);
	if ( ModelIconIndex < 0 ) { return false; }
	CAOIModel *ModelPtr = (CAOIModel*)(m_ModelIconListWnd.GetItemData(ModelIconIndex));	
	if ( ModelPtr->IsKindOf(RUNTIME_CLASS(CAOIModel)) == FALSE ) { return false; }
	if ( ModelPtr->CheckModelTypeEnabled() == false )
	{	return false; }

	bool     bLock=false;
	CString  str;
	CString  ModelFileTemp;
	CString  ModelFilename;
	CString  ModelFolderSrc;
	CString  ModelFolderDst;	
	CString  ModelName = ModelPtr->GetModelName();
	CString  LibraryFolder = ProjectPtr->GetProjectLibraryFolder();
	CString  TempFolder = AOIDataCollect.GetAOITempDirectory();
	CString  ServerLibrary = AOIDataCollect.GetAOIServerLibraryFolder();

	ModelPtr->SetupkModelModifiedDateTime();
	ModelFileTemp.Format(_T("%s\\%s"), TempFolder, _T("TempModel.MDL"));
	if ( ModelPtr->SaveModelParamFile(ModelFileTemp) == false )
	{
		str = _T("Error, Save Model File Fault");
		str = LoadMultiLanguageString(str, str);
		JetAPI::ShowMessageBox(str);
		return false;
	}

	if ( ProjectPtr->CreateProjectServerLockFile(bLock) == false )
	{
		::DeleteFile(ModelFileTemp);
		str = ProjectPtr->GetErrorString();
		JetAPI::ShowMessageBox(str);		
		return false;
	}
	if ( true == bLock )
	{
		::DeleteFile(ModelFileTemp);
		str = ProjectPtr->GetErrorString();
		JetAPI::ShowMessageBox(str);
		return false;
	}

	ModelFolderSrc = ModelPtr->GetModelFolderModel();
	ModelFolderDst.Format(_T("%s\\%s"), ServerLibrary, ModelName);
	if ( JetAPI::CopyFolderAToFolderB(ModelFolderSrc, ModelFolderDst, false, true, _T(""), -1, -1) == false )
	{
		::DeleteFile(ModelFileTemp);
		str = _T("Error, Copy Model Folder Fault");
		str = LoadMultiLanguageString(str, str);
		ProjectPtr->DeleteProjectServerLockFile();
		JetAPI::ShowMessageBox(str);		
		return false;
	}

	ModelFilename = AOIDataDefine.GetModelParamFilename(ServerLibrary, ModelName);	
	::CopyFile(ModelFileTemp, ModelFilename, FALSE);
	ProjectPtr->DeleteProjectServerLockFile();
	::DeleteFile(ModelFileTemp);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditLibraryWnd::ExecModelLoadFromServer()//從伺服器載入模組
{
	//ReplaceProjectModel
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }
	const int ModelIconIndex = m_ModelIconListWnd.GetNextItem(-1, LVNI_SELECTED);
	if ( ModelIconIndex < 0 ) { return false; }
	CAOIModel *ModelPtr = (CAOIModel*)(m_ModelIconListWnd.GetItemData(ModelIconIndex));	
	if ( ModelPtr->IsKindOf(RUNTIME_CLASS(CAOIModel)) == FALSE ) { return false; }
	if ( ModelPtr->CheckModelTypeEnabled() == false )
	{	return false; }

	bool       bLock=false;
	CString    str;	
	CString    ModelFileTemp;
	CString    ModelFilename;
	CString    ModelFolderSrc;
	CString    ModelFolderDst;	
	CAOIModel *ModelPtr_Tmp=NULL;
	CString    ModelName = ModelPtr->GetModelName();
	CString    LibraryFolder = ProjectPtr->GetProjectLibraryFolder();
	CString    TempFolder = AOIDataCollect.GetAOITempDirectory();
	CString    ServerLibrary = AOIDataCollect.GetAOIServerLibraryFolder();	
	
	if ( ProjectPtr->CreateProjectServerLockFile(bLock) == false )
	{
		::DeleteFile(ModelFileTemp);
		str = ProjectPtr->GetErrorString();
		JetAPI::ShowMessageBox(str);		
		return false;
	}
	if ( true == bLock )
	{
		::DeleteFile(ModelFileTemp);
		str = ProjectPtr->GetErrorString();
		JetAPI::ShowMessageBox(str);
		return false;
	}

	ModelFilename = AOIDataDefine.GetModelParamFilename(ServerLibrary, ModelName);	
	if ( JetAPI::IsFileExist(ModelFilename) == false )
	{
		ProjectPtr->DeleteProjectServerLockFile();
		str = _T("Error, No Model in Server Library");
		str = LoadMultiLanguageString(str, str);
		JetAPI::ShowMessageBox(str);		
		return true;
	}
	str = _T("Do you want to load Model from Server Library?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) != IDYES )
	{
		ProjectPtr->DeleteProjectServerLockFile();
		return true;	
	}

	ModelFileTemp = AOIDataDefine.GetModelParamFilename(LibraryFolder, ModelName);	
	::CopyFile(ModelFilename, ModelFileTemp, FALSE);	

	ModelPtr_Tmp = AOIObjManager.CreateModelObj();
	if ( NULL == ModelPtr_Tmp )
	{
		ProjectPtr->DeleteProjectServerLockFile();
		str = AOIObjManager.GetErrorString();
		JetAPI::ShowMessageBox(str);
		return false;
	}
	if ( ModelPtr_Tmp->LoadModelParamFile(ModelFileTemp) == false )
	{		
		ProjectPtr->DeleteProjectServerLockFile();
		str = _T("Error, Load Model File Fault");
		str = LoadMultiLanguageString(str, str);
		JetAPI::ShowMessageBox(str);		
		AOIObjManager.DestroyModelObj(ModelPtr_Tmp);
		return false;
	}	
	ModelFolderDst = ModelPtr->GetModelFolderModel();
	ModelFolderSrc.Format(_T("%s\\%s"), ServerLibrary, ModelName);
	if ( JetAPI::CopyFolderAToFolderB(ModelFolderSrc, ModelFolderDst, false, true, _T(""), -1, -1) == false )
	{
		::DeleteFile(ModelFileTemp);
		str = _T("Error, Copy Model Folder Fault");
		str = LoadMultiLanguageString(str, str);
		ProjectPtr->DeleteProjectServerLockFile();
		JetAPI::ShowMessageBox(str);		
		return false;
	}
	ProjectPtr->DeleteProjectServerLockFile();
	::DeleteFile(ModelFileTemp);

	//更新符合專案的模組參數
	unsigned int   DefaultFrameIndex=0;
	unsigned int   DefaultFrameUniqueID = 0;	
	std::vector<CColorGroup>  ColorGroupList;
	std::vector<unsigned int> FrameIndexMapList;
	ProjectPtr->CloneProjectColorGroupList(ColorGroupList);
	ProjectPtr->BuildProjectFrameIndexMapParam(FrameIndexMapList, DefaultFrameIndex, DefaultFrameUniqueID);			

	ModelPtr_Tmp->UnSelectModel();	
	ModelPtr_Tmp->SetModelFolderModel(ModelFolderDst);
	ModelPtr_Tmp->AssignModelFolder();
	ModelPtr_Tmp->SetupkModelModifiedDateTime();
	ModelPtr_Tmp->SetModelBodyBoxActived(true);		
	ModelPtr_Tmp->SetModelNeedSaveFiles(true);
	ModelPtr_Tmp->SetModelBKImageNeedToGrab(true);	
	ModelPtr_Tmp->UpdateModelColorGroupLinkIndex(ColorGroupList);
	ModelPtr_Tmp->UpdateModelFrameIndex(DefaultFrameIndex, DefaultFrameUniqueID, FrameIndexMapList);
	if ( ProjectPtr->ReplaceProjectModel(ModelPtr_Tmp) == NULL )
	{
		AOIObjManager.DestroyModelObj(ModelPtr_Tmp);			
		return false;	
	}	

	CAOIComponent *ComponentPtr = ProjectPtr->GetProjectActiveComponent();
	ProjectPtr->SelectProjectAllComponents(false);
	ProjectPtr->SelectProjectComponentsByModelName(ModelName, false);
	ProjectPtr->ApplyProjectModelToComponentsSelected(ModelPtr);
	ProjectPtr->SelectProjectAllComponents(false);
	if ( NULL != ComponentPtr )
	{	ComponentPtr->SetComponentSelected(true); 	}		
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditLibraryWnd::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: 在此加入您的訊息處理常式程式碼	
	const bool bExec=false;
	if ( true == bExec )
	{
		if ( bShow == TRUE )
		{	ExecShowLibraryWnd();	}
		else
		{	ExecHideLibraryWnd();	}	
	}
}
//-------------------------------------------------------------------------------------//
bool CEditLibraryWnd::ExecToActiveComponent()
{	
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) 
	{	return true; }
	CAOIComponent *pComponent = ProjectPtr->GetProjectActiveComponent();
	if ( NULL == pComponent ) 
	{	return true; }
	
	CString ComponentName = pComponent->GetComponentName();
	CString PartNumber = pComponent->GetComponentPartNumber();
	CWnd::SetDlgItemText(EML_COMPONENT_SELECTED_NAME_EDIT, ComponentName);
	CWnd::SetDlgItemText(EML_SEARCH_MODEL_NAME_SELECTED_EDIT, PartNumber);

	CAOIModel *ModelPtrC = pComponent->GetComponentModelPtr();
	if ( NULL == ModelPtrC ) 
	{	return true; }
	unsigned int ModelIndex = ModelPtrC->GetModelIndex();

	CAOIModel *ModelPtr = ProjectPtr->GetProjectModelPtr(ModelIndex, true);
	if ( NULL == ModelPtr ) 
	{	return true; }
	MODEL_TYPE ModelType = ModelPtr->GetModelType();
	CString    GroupName = ModelPtr->GetModelGroupName();
	GroupName.MakeUpper();

	int        i=0;
	int        nTypeItem = -1;	
	const int  NTypeItems = m_ModelTypeListWnd.GetItemCount();
	
	for ( i=0; i<NTypeItems; i++ )
	{
		if ( ModelType != m_ModelTypeListWnd.GetItemData(i) ) { continue; }
		nTypeItem = i;
		break;
	}
	if ( NTypeItems == i )
	{	return TRUE; }
	m_ModelTypeListWnd.SetItemState(-1, 0, LVIS_SELECTED);//-1::設定至全部
	m_ModelTypeListWnd.SetItemState(nTypeItem, LVIS_SELECTED, LVIS_SELECTED);

	ClearModelIconListCtrl(m_ModelIconListWnd);
	ClearModelGroupListCtrl(m_ModelGroupListWnd);

	//建立模組群組列表
	BuildModelGroupListCtrl(m_ModelGroupListWnd, ModelType, false);
	CString    ItemText;
	int        nGroupItem =-1;	
	const int  NGroupItems = m_ModelGroupListWnd.GetItemCount();
	for ( i=0; i<NGroupItems; i++ )
	{
		ItemText = m_ModelGroupListWnd.GetItemText(i, 0);
		ItemText.MakeUpper();
		if ( ItemText != GroupName ) { continue; }
		nGroupItem = i;
		break;
	}
	if ( NGroupItems == i )
	{	return true; }
	m_ModelGroupListWnd.SetItemState(-1, 0, LVIS_SELECTED);//-1::設定至全部
	m_ModelGroupListWnd.SetItemState(nGroupItem, LVIS_SELECTED, LVIS_SELECTED);

	//建立模組列表
	BuildModelIconListCtrl(ModelType, GroupName, m_ModelIconListWnd);

	int        nModelItem =-1;	
	const int  NModelItems = m_ModelIconListWnd.GetItemCount();
	for ( i=0; i<NModelItems; i++ )
	{		
		if ( ModelPtr != (CAOIModel*)(m_ModelIconListWnd.GetItemData(i)) ) { continue; }
		nModelItem = i;
		break;
	}
	if ( NModelItems == i )
	{	return TRUE; }

	CString ModelName = ModelPtr->GetModelName();
	//m_ModelIconListWnd.SetItemState(-1, ~LVIS_SELECTED, LVIS_SELECTED);//-1::設定至全部
	m_ModelIconListWnd.SetItemState(-1, 0, LVIS_SELECTED);//-1::設定至全部
	m_ModelIconListWnd.SetItemState(nModelItem, LVIS_SELECTED, LVIS_SELECTED);
	m_ModelIconListWnd.EnsureVisible(nModelItem, FALSE);
	m_ModelIconListWnd.SetFocus();
	CWnd::SetDlgItemText(EML_SEARCH_MODEL_NAME_SELECTED_EDIT, ModelName);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditLibraryWnd::ExecShowLibraryWnd()//顯示資料庫視窗
{	
	ExecToActiveComponent();	
	AOIDataCollect.SetDrawModelMode(DRAW_MODEL_EDIT);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditLibraryWnd::ExecHideLibraryWnd()//隱藏資料庫視窗
{
	CListCtrl &ListCtrl=m_ModelIconListWnd;
	const int nItem = ListCtrl.GetNextItem(-1, LVIS_SELECTED);
	if ( nItem >= 0 )
	{	ListCtrl.SetItemState(nItem, 0, LVIS_SELECTED); }
	AOIDataCollect.DestroyModelPreViewPtr();
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditLibraryWnd::OnSearchModelBtn() 
{
	// TODO: Add your control notification handler code here
	CAOIProject *Project = GetActiveProject();
	if ( NULL == Project )	{	return ; }

	CString   str;
	CString   strFindName;
	const int SizeW = CWnd::GetDlgItemInt(EML_SEARCH_MODEL_SIZE_W_EDIT);
	const int SizeH = CWnd::GetDlgItemInt(EML_SEARCH_MODEL_SIZE_H_EDIT);
	const int Tolerance = CWnd::GetDlgItemInt(EML_SEARCH_MODEL_SIZE_TOL_EDIT);
	BOOL bFindName = CWnd::IsDlgButtonChecked(EML_SEARCH_MODEL_NAME_CHK);
	BOOL bFindSize = CWnd::IsDlgButtonChecked(EML_SEARCH_MODEL_SIZE_CHK);
	CWnd::GetDlgItemText(EML_SEARCH_MODEL_NAME_EDIT, strFindName);
	
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

	ModelName = ModelPtr->GetModelName();
	CString GroupName = ModelPtr->GetModelGroupName();
	const MODEL_TYPE ModelType = ModelPtr->GetModelType();
	BuildModelGroupListCtrl(m_ModelGroupListWnd, ModelType, false);
	BuildModelIconListCtrl(ModelType, GroupName, m_ModelIconListWnd);

	SetModelTypeListCtrlItemSlected(ModelType);
	SetModelGroupListCtrlItemSlected(GroupName);
	SetModelIconListCtrlItemSlected(ModelName);
	m_ModelIconListWnd.SetFocus();
	CWnd::SetDlgItemText(EML_SEARCH_MODEL_NAME_SELECTED_EDIT, ModelName);	

	AOIDataCollect.CreateModelPreViewPtr(ModelPtr);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_REDRAW_VIEW_WND, NULL);		
}
//-------------------------------------------------------------------------------------//
void CEditLibraryWnd::OnModelCloneBtn() 
{
	// TODO: Add your control notification handler code here
	const bool bPartNumberName=CheckUsePartNumberName();
	ExecModelClone(bPartNumberName);
	AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_SHOW_PROJECT_LIBRARY_DOCK_PANE, FALSE);		
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_PART_LIST_WND, WPARAM_BUILD_PART_LIST, LPARAM_BUILD_DOCK_LIST_EDIT);
}
//-------------------------------------------------------------------------------------//
void CEditLibraryWnd::OnDblclkModelTypeListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	if ( CWnd::IsWindowVisible() == FALSE )
	{	return;	}
	const bool bPartNumberName=CheckUsePartNumberName();
	ExecModelGroupAdd(bPartNumberName);	
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CEditLibraryWnd::OnArrangeLibraryBtn()
{
	CAOIProject *Project = GetActiveProject();
	if ( NULL == Project )	{	return ; }

	CString str;
	str = _T("Do you want to arrange the project library?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO ) { return; }
	AOIDataCollect.ReleaseModelUniFrameList();
	AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_UPDATE, (LPARAM)(this));

	if ( Project->ArrangeProjectLibrary() == false )
	{
		str = Project->GetErrorString();
		JetAPI::ShowMessageBox(str);
	}	
	const int nItem = m_ModelTypeListWnd.GetNextItem(-1, LVNI_SELECTED);
	if ( nItem < 0  ) { return; }	
	MODEL_TYPE ModelType = (MODEL_TYPE)(m_ModelTypeListWnd.GetItemData(nItem));	
	BuildModelGroupListCtrl(m_ModelGroupListWnd, ModelType, true);			
}
//-------------------------------------------------------------------------------------//
void CEditLibraryWnd::OnMenuLibraryModelRename()
{
	ExecModelRename();
}
//-------------------------------------------------------------------------------------//
void CEditLibraryWnd::OnMenuLibraryModelApply()
{
	ExecModelApply();
}
//-------------------------------------------------------------------------------------//
void CEditLibraryWnd::OnMenuLibraryModelClone()
{
	const bool bPartNumberName=CheckUsePartNumberName();
	ExecModelClone(bPartNumberName);
}
//-------------------------------------------------------------------------------------//
void CEditLibraryWnd::OnMenuLibraryModelRotate090()
{
	ExecModelRotate(90);
}
//-------------------------------------------------------------------------------------//
void CEditLibraryWnd::OnMenuLibraryModelRotate180()
{
	ExecModelRotate(180);
}
//-------------------------------------------------------------------------------------//
void CEditLibraryWnd::OnMenuLibraryModelRotate270()
{
	ExecModelRotate(270);
}
//-------------------------------------------------------------------------------------//
void CEditLibraryWnd::OnMenuLibraryModelDelete()
{
	ExecModelDelete();
}
//-------------------------------------------------------------------------------------//
void CEditLibraryWnd::OnMenuLibraryModelGroupName()
{
	ExecModelModifyGroupName();
}
//-------------------------------------------------------------------------------------//
void CEditLibraryWnd::OnMenuLibraryModelChangeType()
{
	ExecModelChangeModelType();
}
//-------------------------------------------------------------------------------------//
void CEditLibraryWnd::OnMenuLibraryModelBKImageIndex()
{
	ExecModelBKImageIndex();
}
//-------------------------------------------------------------------------------------//
void CEditLibraryWnd::OnMenuLibraryModelSaveServer()
{
	ExecModelSaveToServer();
}
//-------------------------------------------------------------------------------------//
void CEditLibraryWnd::OnMenuLibraryModelLoadServer()
{
	ExecModelLoadFromServer();
}
//-------------------------------------------------------------------------------------//
void CEditLibraryWnd::OnSelchangeModelBKImageIndexCombox()
{	
	const int nTypeItem = (int)(m_ModelTypeListWnd.GetNextItem(-1, LVNI_SELECTED));
	if ( nTypeItem < 0 ) { return; }
	MODEL_TYPE ModelType = (MODEL_TYPE)(m_ModelTypeListWnd.GetItemData(nTypeItem));

	const int nGroupItem = (int)(m_ModelGroupListWnd.GetNextItem(-1, LVNI_SELECTED));
	if ( nGroupItem < 0 ) { return; }
	CString GroupName = m_ModelGroupListWnd.GetItemText(nGroupItem, 0);

	BuildModelIconListCtrl(ModelType, GroupName, m_ModelIconListWnd);
	return;
}
//-------------------------------------------------------------------------------------//
void CEditLibraryWnd::OnSelchangeModelIconSizeCombox()
{
	const int nTypeItem = (int)(m_ModelTypeListWnd.GetNextItem(-1, LVNI_SELECTED));
	if ( nTypeItem < 0 ) { return; }
	MODEL_TYPE ModelType = (MODEL_TYPE)(m_ModelTypeListWnd.GetItemData(nTypeItem));

	const int nGroupItem = (int)(m_ModelGroupListWnd.GetNextItem(-1, LVNI_SELECTED));
	if ( nGroupItem < 0 ) { return; }
	CString GroupName = m_ModelGroupListWnd.GetItemText(nGroupItem, 0);

	BuildModelIconListCtrl(ModelType, GroupName, m_ModelIconListWnd);
	return;
}
//-------------------------------------------------------------------------------------//
bool CEditLibraryWnd::ExecModelModifyGroupName()//變更模組名稱
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }
	const int ModelIconIndex = m_ModelIconListWnd.GetNextItem(-1, LVNI_SELECTED);
	if ( ModelIconIndex < 0 ) { return false; }
	CAOIModel *ModelPtr = (CAOIModel*)(m_ModelIconListWnd.GetItemData(ModelIconIndex));	
	if ( ModelPtr->IsKindOf(RUNTIME_CLASS(CAOIModel)) == FALSE ) { return false; }
	if ( ModelPtr->CheckModelTypeEnabled() == false )
	{	return false; }

	CString      str;
	CString      strValue;
	CString      strLabel;
	CString      strCaption;
	CString      strModelName = ModelPtr->GetModelName();
	CString      strGroupName = ModelPtr->GetModelGroupName();	
	CInputBoxWnd InputBox;
	const double Precision = DBL_PRECISION;
	MODEL_TYPE   ModelType=ModelPtr->GetModelType();

	strCaption = _T("Input Model Group Name Wnd");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strLabel = _T("Group Name");
	strLabel = LoadMultiLanguageString(strLabel, strLabel);
	strValue = strGroupName;
	InputBox.SetParam1(strCaption, strLabel, strValue);
	if ( InputBox.DoModal() == IDCANCEL ) 
	{	return false;  }

	strValue = InputBox.m_DataEdit1;	
	if ( strGroupName.CompareNoCase(strValue) == 0 )
	{	return false; }

	strGroupName.TrimLeft();//剔除左邊空白
	strGroupName.TrimRight();//剔除右邊空白
	if ( strGroupName.GetLength() == 0 )
	{	return false;; }

	if ( ProjectPtr->CheckProjectModelGroupNameExist(ModelType, strValue) == true ) 
	{
		str = _T("Error, the group name exit");
		str = LoadMultiLanguageString(str, str);
		strValue.Format(_T("%s [%s] !!!"), str, InputBox.m_DataEdit1);
		JetAPI::ShowMessageBox(strValue);
		return false;
	}
	strValue = InputBox.m_DataEdit1;
	strValue.MakeUpper();
	ModelPtr->SetModelGroupName(strValue);
	ProjectPtr->BackupProjectComponentSelected();
	ProjectPtr->SelectProjectAllComponents(false);
	ProjectPtr->SelectProjectComponentsByModelName(strModelName, false);
	ProjectPtr->ApplyProjectModelToComponentsSelected(ModelPtr);	
	ProjectPtr->RestoreProjectComponentSelected();
	ExecToActiveComponent();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditLibraryWnd::ExecModelChangeModelType()//變更模組樣式
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }
	const int ModelIconIndex = m_ModelIconListWnd.GetNextItem(-1, LVNI_SELECTED);
	if ( ModelIconIndex < 0 ) { return false; }
	CAOIModel *ModelPtr = (CAOIModel*)(m_ModelIconListWnd.GetItemData(ModelIconIndex));	
	if ( ModelPtr->IsKindOf(RUNTIME_CLASS(CAOIModel)) == FALSE ) { return false; }
	MODEL_TYPE ModelType = ModelPtr->GetModelType();	
	if ( MODEL_TYPE_NULL == ModelType ) { return false; }

	//GetModelSimilarTypeList
	std::vector<MODEL_TYPE> ModelTypeList;
	CAOIModel::GetModelSimilarTypeList(ModelType, ModelTypeList);
	const size_t ModelTypeCount = ModelTypeList.size();
	if ( 0 == ModelTypeCount ) { return false; }

	size_t       i=0;
	CString      str;
	CString      strValue;
	CString      strLabel;
	CString      strCaption;
	MODEL_TYPE   TmpModelType;
	CString      strModelName = ModelPtr->GetModelName();
	CString      strGroupName = ModelPtr->GetModelGroupName();
	CInputBoxWnd InputBox;
	CInputListWnd EnumWnd;
	const double Precision = DBL_PRECISION;

	TListNode Node;
	std::vector<TListNode> NodelList;	
	const DWORD_PTR OldLinkIndex = ModelType;
	
	strLabel = _T("Model Type");
	strLabel = LoadMultiLanguageString(strLabel, strLabel);
	strCaption = _T("Input Model Type Wnd");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	

	for ( i=0; i<ModelTypeCount; i++ )
	{
		TmpModelType = ModelTypeList[i];
		Node.Data = TmpModelType;
		Node.Text = AOIDataDefine.GetModelTypeText(TmpModelType);
		NodelList.push_back(Node);	
	}
	
	POINT Point;
	::GetCursorPos(&Point);
	//ComboxWnd.SetWndPos(Point);
	EnumWnd.SetParam1(strCaption, strLabel, OldLinkIndex, NodelList);
	if ( EnumWnd.GetSelIndex1() < 0 ) 
	{	EnumWnd.SetSelIndex1(0); }	
	if ( EnumWnd.DoModal() == IDCANCEL )
	{	return false; }	
	MODEL_TYPE NewModelType = (MODEL_TYPE)(EnumWnd.GetSelData());		
	if ( NewModelType == ModelType ) { return false; }

	strCaption = _T("Input Model Group Name Wnd");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strLabel = _T("Group Name");
	strLabel = LoadMultiLanguageString(strLabel, strLabel);
	strValue = strGroupName;	
	
	while ( true ) 
	{
		InputBox.SetParam1(strCaption, strLabel, strValue);
		if ( InputBox.DoModal() == IDCANCEL ) 
		{	return false;  }
		strValue = InputBox.m_DataEdit1;	
		if ( ProjectPtr->CheckProjectModelGroupNameExist(NewModelType, strValue) == false ) 
		{	break; }
	};

	strValue = InputBox.m_DataEdit1;
	strValue.MakeUpper();

	ProjectPtr->ChangeProjectModelTypeByName(strModelName, NewModelType, strValue);
	//ModelPtr->SetModelType(NewModelType);
	//ModelPtr->SetModelGroupName(strValue);
	ExecToActiveComponent();	
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditLibraryWnd::OnModelGroupNameBtn()
{
	ExecModelModifyGroupName();
}
//-------------------------------------------------------------------------------------//