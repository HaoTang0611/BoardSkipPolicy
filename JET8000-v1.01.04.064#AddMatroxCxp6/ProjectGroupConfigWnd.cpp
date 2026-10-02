// ProjectGroupConfigWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "ProjectGroupConfigWnd.h"
//-------------------------------------------------------------------------------------//
#include "InputBoxWnd.h"
#include "InputListWnd.h"
//-------------------------------------------------------------------------------------//
const int SETTING_COL   = 2;//設定的欄位
//-------------------------------------------------------------------------------------//
const int NODE_FROM_OFF  = 0;
const int NODE_FROM_1    = 1;
const int NODE_FROM_2    = 2;
const int NODE_FROM_LIST = 99;
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
CProjectGroupConfigWnd ProjectGroupConfigWnd;
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CProjectGroupConfigWnd dialog
//-------------------------------------------------------------------------------------//
CProjectGroupConfigWnd::CProjectGroupConfigWnd(CWnd* pParent /*=NULL*/)
	: CDialog(CProjectGroupConfigWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProjectGroupConfigWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT	
	m_ProjectPtr = NULL;
	m_ParamActPtr = NULL;
	m_PartGropuNodePtr = NULL;
}
//-------------------------------------------------------------------------------------//
void CProjectGroupConfigWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProjectGroupConfigWnd)		
	DDX_Control(pDX, PROGRPCFG_GROUP_PARAM_EDIT, m_GroupParamEdit);	
	DDX_Control(pDX, PROGRPCFG_GROUP_PARAM_COMBO, m_GroupParamCombox);	
	DDX_Control(pDX, PROGRPCFG_GROUP_WND_LIST_BOX, m_WndListBox);	
	DDX_Control(pDX, PROGRPCFG_GROUP_NODE_LIST_BOX, m_NodeListBox);	
	DDX_Control(pDX, PROGRPCFG_GROUP_MODE_COMBO, m_GroupModeCombox);		
	DDX_Control(pDX, PROGRPCFG_GROUP_LIST_WND, m_GroupListWnd);	
	DDX_Control(pDX, PROGRPCFG_NODE_PARAM_LIST_WND, m_NodeParamListWnd);
	DDX_Control(pDX, PROGRPCFG_GROUP_PARAM_LIST_WND, m_GroupParamListWnd);		
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CProjectGroupConfigWnd, CDialog)
	//{{AFX_MSG_MAP(CProjectGroupConfigWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	ON_WM_CONTEXTMENU()
	ON_NOTIFY(LVN_ITEMCHANGED, PROGRPCFG_GROUP_LIST_WND, OnItemchangedGroupListWnd)
	ON_NOTIFY(NM_DBLCLK, PROGRPCFG_GROUP_LIST_WND, OnDblclkGroupListWnd)
	ON_NOTIFY(LVN_ITEMCHANGED, PROGRPCFG_NODE_PARAM_LIST_WND, OnItemchangedNodeParamListWnd)
	ON_NOTIFY(NM_DBLCLK, PROGRPCFG_NODE_PARAM_LIST_WND, OnDblclkNodeParamListWnd)
	ON_NOTIFY(LVN_ITEMCHANGED, PROGRPCFG_GROUP_PARAM_LIST_WND, OnItemchangedGroupParamListWnd)
	ON_NOTIFY(NM_DBLCLK, PROGRPCFG_GROUP_PARAM_LIST_WND, OnDblclkGroupParamListWnd)
	ON_EN_KILLFOCUS(PROGRPCFG_GROUP_PARAM_EDIT, OnKillfocusGroupParamEdit)
	ON_CBN_SELCHANGE(PROGRPCFG_GROUP_PARAM_COMBO, OnSelchangeGroupParamCombo)
	ON_CBN_KILLFOCUS(PROGRPCFG_GROUP_PARAM_COMBO, OnKillfocusGroupParamCombo)
	ON_BN_CLICKED(PROGRPCFG_GROUP_ADD_BTN, OnGroupAddBtn)
	ON_BN_CLICKED(PROGRPCFG_GROUP_COPY_BTN, OnGroupCopyBtn)
	ON_BN_CLICKED(PROGRPCFG_GROUP_DELETE_BTN, OnGroupDeleteBtn)
	ON_BN_CLICKED(PROGRPCFG_GROUP_CLEAR_BTN, OnGroupClearBtn)
	ON_BN_CLICKED(PROGRPCFG_GROUP_TEST_BTN, OnGroupTestBtn)
	ON_BN_CLICKED(PROGRPCFG_GROUP_UPDATE_BTN, OnGroupUpdateBtn)	
	ON_BN_CLICKED(PROGRPCFG_GROUP_DEL_OTHERS_BTN, OnGroupDelOthersBtn)	
	ON_BN_CLICKED(PROGRPCFG_GROUP_NODE_ADD_BTN1, OnGroupNodeAddBtn1)
	ON_BN_CLICKED(PROGRPCFG_GROUP_NODE_DELETE_BTN1, OnGroupNodeDeleteBtn1)
	ON_EN_SETFOCUS(PROGRPCFG_GROUP_NODE_EDIT1, OnSetfocusNodeAddEdit1)
	ON_BN_CLICKED(PROGRPCFG_GROUP_NODE_ADD_BTN2, OnGroupNodeAddBtn2)
	ON_BN_CLICKED(PROGRPCFG_GROUP_NODE_DELETE_BTN2, OnGroupNodeDeleteBtn2)
	ON_EN_SETFOCUS(PROGRPCFG_GROUP_NODE_EDIT2, OnSetfocusNodeAddEdit2)
	ON_BN_CLICKED(PROGRPCFG_GROUP_NODE_LIST_ADD_BTN, OnGroupNodeListAddBtn)
	ON_BN_CLICKED(PROGRPCFG_GROUP_NODE_LIST_DELETE_BTN, OnGroupNodeListDeleteBtn)
	ON_BN_CLICKED(PROGRPCFG_GROUP_NODE_LIST_CLEAR_BTN, OnGroupNodeListClearBtn)
	ON_LBN_SELCHANGE(PROGRPCFG_GROUP_NODE_LIST_BOX, OnSelchangeNodeListBox)
	ON_BN_CLICKED(PROGRPCFG_GROUP_NODE_LIST_ARRANGE_BTN, OnGroupNodeListArrangeBtn)
	ON_BN_CLICKED(PROGRPCFG_GROUP_NODE_LIST_SORT_X_BTN, OnGroupNodeListSortXBtn)
	ON_BN_CLICKED(PROGRPCFG_GROUP_NODE_LIST_SORT_Y_BTN, OnGroupNodeListSortYBtn)
	ON_BN_CLICKED(PROGRPCFG_GROUP_WND_SET_BTN, OnGroupWndSetBtn)
	ON_BN_CLICKED(PROGRPCFG_GROUP_WND_SET_ALL_BTN, OnGroupWndSetAllBtn)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CProjectGroupConfigWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CProjectGroupConfigWnd::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	JetAPI::InitialListCtrl(m_GroupListWnd);
	JetAPI::InitialListCtrl(m_NodeParamListWnd);
	JetAPI::InitialListCtrl(m_GroupParamListWnd);	
	SwitchMultiLanguage();	
	BuildGroupListWndHeader();
	BuildNodeParamListWndHeader();
	BuildGroupParamListWndHeader();
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	AOIDataDefine.BuildAOIGroupModeCombox(m_GroupModeCombox);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CProjectGroupConfigWnd::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CProjectGroupConfigWnd::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CProjectGroupConfigWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if ( TRUE == bShow )
	{	UpdatePartGroupList();	}
}
//-------------------------------------------------------------------------------------//
void CProjectGroupConfigWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CDialog::OnPaint() for painting messages
}
//-------------------------------------------------------------------------------------//
void CProjectGroupConfigWnd::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CProjectGroupConfigWnd::OnOK() 
{
	// TODO: Add extra validation here
	if ( GetLockUIWnd() == true ) { return ; }		
	if ( CheckPartGroupList() == false )
	{	return; }
	UpdateToProjectPartGroupList();

	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL != ProjectPtr )
	{	ProjectPtr->AssignProjectPartGroupToPanelBoard();	}
	CDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CProjectGroupConfigWnd::OnCancel() 
{
	// TODO: Add extra cleanup here
	if ( GetLockUIWnd() == true ) { return ; }		
	RestoreProjectPartGroupList();
	SetProjectPtr(NULL);
	CDialog::OnCancel();
}
//-------------------------------------------------------------------------------------//
BOOL CProjectGroupConfigWnd::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch ( pMsg->message )
	{
	case WM_KEYDOWN:
		switch ( pMsg->wParam )
		{
		case VK_RETURN:
			if ( m_GroupParamEdit.IsWindowVisible() == TRUE ) 
			{
				ExecUpdateParamByEdit();
				m_GroupParamEdit.ShowWindow(SW_HIDE);	
				m_GroupParamEdit.SetWindowText(_T(""));
				return TRUE;				
			}
			if ( m_GroupParamCombox.IsWindowVisible() == TRUE ) 
			{
				ExecUpdateParamByCombox();				
				m_GroupParamCombox.ShowWindow(SW_HIDE);
				JetAPI::ClearCombox(m_GroupParamCombox);
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
LRESULT CProjectGroupConfigWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch ( message )
	{
	case MSG_MAIN_FRAME_MESSAGE:
		switch ( wParam )
		{
		case WPARAM_PROJECT_NEW:
		case WPARAM_PROJECT_OPEN:
		case WPARAM_PROJECT_SWITCH:			
			UpdatePartGroupList();
			break;		
		case WPARAM_PROJECT_UPDATE:
			//if ( CWnd::IsWindowVisible() == TRUE )
			//{	UpdatePartGroupList();	}
			break;		
		case WPARAM_PROJECT_CLOSE:		
			UpdatePartGroupList();
			break;
		case WPARAM_PROJECT_PART_SELECTED:
			break;
		case WPARAM_PROJECT_PART_DELETED:
			UpdatePartGroupList();
			break;
		case WPARAM_CALC_CURRENT_FOV_POSITION:			
			break;
		case WPARAM_PROJECT_SWITCH_MARK:
			break;
		case WPARAM_PROJECT_CLOSE_ACTIVE_OBJ:						
			break;
		}
		break;		
	case MSG_EDIT_PART_LIST_WND:
		switch ( wParam )
		{
		case WPARAM_BUILD_PART_LIST:
			if ( CWnd::IsWindowVisible() == TRUE )
			{	UpdatePartGroupList();	}
			break;
		}
		break;
	}
	return CDialog::WindowProc(message, wParam, lParam);	
}
//-------------------------------------------------------------------------------------//
void CProjectGroupConfigWnd::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_PROJECT_GROUP_CONFIG_WND");
	//---------------------------------------------------------------------------------//
	WndID = IDD_PROJECT_GROUP_CONFIG_WND;
	WndKey = _T("IDD_PROJECT_GROUP_CONFIG_WND");
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
	WndID = PROGRPCFG_GROUP_ADD_BTN;
	WndKey = _T("PROGRPCFG_GROUP_ADD_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = PROGRPCFG_GROUP_DELETE_BTN;
	WndKey = _T("PROGRPCFG_GROUP_DELETE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = PROGRPCFG_GROUP_CLEAR_BTN;
	WndKey = _T("PROGRPCFG_GROUP_CLEAR_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROGRPCFG_GROUP_COPY_BTN;
	WndKey = _T("PROGRPCFG_GROUP_COPY_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROGRPCFG_GROUP_TEST_BTN;
	WndKey = _T("PROGRPCFG_GROUP_TEST_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROGRPCFG_GROUP_UPDATE_BTN;
	WndKey = _T("PROGRPCFG_GROUP_UPDATE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROGRPCFG_GROUP_DEL_OTHERS_BTN;
	WndKey = _T("PROGRPCFG_GROUP_DEL_OTHERS_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = PROGRPCFG_GROUP_MODE_LABEL;
	WndKey = _T("PROGRPCFG_GROUP_MODE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = PROGRPCFG_GROUP_NODE_GROUP1;
	WndKey = _T("PROGRPCFG_GROUP_NODE_GROUP1");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROGRPCFG_GROUP_NODE_ADD_BTN1;
	WndKey = _T("PROGRPCFG_GROUP_NODE_ADD_BTN1");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROGRPCFG_GROUP_NODE_DELETE_BTN1;
	WndKey = _T("PROGRPCFG_GROUP_NODE_DELETE_BTN1");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = PROGRPCFG_GROUP_NODE_GROUP2;
	WndKey = _T("PROGRPCFG_GROUP_NODE_GROUP2");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROGRPCFG_GROUP_NODE_ADD_BTN2;
	WndKey = _T("PROGRPCFG_GROUP_NODE_ADD_BTN2");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROGRPCFG_GROUP_NODE_DELETE_BTN2;
	WndKey = _T("PROGRPCFG_GROUP_NODE_DELETE_BTN2");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = PROGRPCFG_GROUP_NODE_LIST_GROUP;
	WndKey = _T("PROGRPCFG_GROUP_NODE_LIST_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROGRPCFG_GROUP_NODE_LIST_ADD_BTN;
	WndKey = _T("PROGRPCFG_GROUP_NODE_LIST_ADD_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROGRPCFG_GROUP_NODE_LIST_DELETE_BTN;
	WndKey = _T("PROGRPCFG_GROUP_NODE_LIST_DELETE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROGRPCFG_GROUP_NODE_LIST_CLEAR_BTN;
	WndKey = _T("PROGRPCFG_GROUP_NODE_LIST_CLEAR_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROGRPCFG_GROUP_NODE_LIST_ARRANGE_BTN;
	WndKey = _T("PROGRPCFG_GROUP_NODE_LIST_ARRANGE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROGRPCFG_GROUP_NODE_LIST_SORT_X_BTN;
	WndKey = _T("PROGRPCFG_GROUP_NODE_LIST_SORT_X_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROGRPCFG_GROUP_NODE_LIST_SORT_Y_BTN;
	WndKey = _T("PROGRPCFG_GROUP_NODE_LIST_SORT_Y_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//	
	WndID = PROGRPCFG_GROUP_WND_LIST_GROUP;
	WndKey = _T("PROGRPCFG_GROUP_WND_LIST_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = PROGRPCFG_GROUP_WND_SET_BTN;
	WndKey = _T("PROGRPCFG_GROUP_WND_SET_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = PROGRPCFG_GROUP_WND_SET_ALL_BTN;
	WndKey = _T("PROGRPCFG_GROUP_WND_SET_ALL_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//
	/*
	WndID = AAAAAAAAAAAAA;
	WndKey = _T("AAAAAAAAAAAAA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	*/
	//---------------------------------------------------------------------------------//
	return;
}
//-------------------------------------------------------------------------------------//
CString CProjectGroupConfigWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_PROJECT_GROUP_CONFIG_WND");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
bool CProjectGroupConfigWnd::GetLockUIWnd() const//取得是否鎖住UIWnd
{
	bool bLock = AOIDataCollect.GetIsLockUIWnd();
	return bLock;
}
//-------------------------------------------------------------------------------------//
CParamUni* CProjectGroupConfigWnd::GetActParamUni()
{
	return m_ParamActPtr;
}
//-------------------------------------------------------------------------------------//
void CProjectGroupConfigWnd::SetActParamUni(CParamUni *Ptr)
{
	m_ParamActPtr = Ptr;
}
//-------------------------------------------------------------------------------------//
void CProjectGroupConfigWnd::SetProjectPtr(CAOIProject *Ptr)
{
	m_ProjectPtr = Ptr;
	m_ErrorString = _T("");
	m_PartGroupList.clear();
	ClonePartGroupList();	
}
//-------------------------------------------------------------------------------------//
CAOIPartGroup* CProjectGroupConfigWnd::GetActivePartGroup()
{		
	const int nItem = GetGroupListWndItem();
	if ( -1 == nItem ) { return NULL; }
	CThisListCtrl_53 &ListCtrl=GetGroupListWnd();	
	const unsigned int Index = ListCtrl.GetItemData(nItem);
	std::vector<CAOIPartGroup> &PartGroupList = GetPartGroupList();
	const size_t GroupCount = PartGroupList.size();
	if ( Index >= GroupCount ) { return NULL; }
	return  &(PartGroupList[Index]);	
}
//-------------------------------------------------------------------------------------//
CAOIProject* CProjectGroupConfigWnd::GetActiveProject()
{
	return m_ProjectPtr;
}
//-------------------------------------------------------------------------------------//
void CProjectGroupConfigWnd::SetPartGropuNodePtr(TPartGroupNode *Ptr)
{
	m_PartGropuNodePtr = Ptr;
}
//-------------------------------------------------------------------------------------//
bool CProjectGroupConfigWnd::CheckPartGropuNodePtr(const TPartGroupNode *Ptr)
{
	if ( m_PartGropuNodePtr != Ptr ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
std::vector<CAOIPartGroup>& CProjectGroupConfigWnd::GetPartGroupList()
{
	return m_PartGroupList;
}
//-------------------------------------------------------------------------------------//
void CProjectGroupConfigWnd::ClearPartGroupList()
{
	m_PartGroupList.clear();
}
//-------------------------------------------------------------------------------------//
void CProjectGroupConfigWnd::ClonePartGroupList()
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }

	size_t       i=0;
	CAOIPartGroup   *PartGroupPtr = NULL;
	std::vector<CAOIPartGroup>  &PartGroupList=GetPartGroupList();
	const size_t PartGroupCount=ProjectPtr->GetProjectPartGroupCount();
	
	PartGroupList.clear();
	for ( i=0; i<PartGroupCount; i++ )
	{
		PartGroupPtr = ProjectPtr->GetProjectPartGroupPtr(i, false);
		if ( NULL == PartGroupPtr ) { continue; }
		PartGroupList.push_back(*PartGroupPtr);
	}
	m_PartGroupListBackup = PartGroupList;
	return;
}
//-------------------------------------------------------------------------------------//
bool CProjectGroupConfigWnd::CheckPartGroupList()//確認專案群組列表
{
	CString strIsNotInOneBoard;
	std::vector<CAOIPartGroup> &PartGroupList=m_PartGroupList;
	const size_t PartGroupCount=PartGroupList.size();
	strIsNotInOneBoard = _T("is not in one Board");
	strIsNotInOneBoard = LoadMultiLanguageString(strIsNotInOneBoard, strIsNotInOneBoard);
	for ( size_t i=0; i<PartGroupCount; i++ )
	{
		CAOIPartGroup &rPartGroup=PartGroupList[i];
		if ( rPartGroup.GetPartGroupInOneBoard() == true )
		{
			if ( rPartGroup.CheckPartGroupNodeInOneBoard() == false )
			{
				CString Error;
				CString GroupText=AOIDataDefine.GetGroupText();
				CString GroupName=rPartGroup.GetPartGroupName();
				Error.Format(_T("[%d] %s [%s:%d] %s"), i+1, GroupName, GroupText, rPartGroup.GetPartGroupGroupID_UI(), strIsNotInOneBoard);
				JetAPI::ShowMessageBox(Error);
				return false;
			}
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CProjectGroupConfigWnd::UpdatePartGroupList()//更新(取得)專案群組列表
{
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	SetProjectPtr(ProjectPtr);
	BuildGroupListWnd();
	SwitchMultiLanguage();
}
//--------------------------------------------------------------------------
void CProjectGroupConfigWnd::RestoreProjectPartGroupList()//恢復專案群組列表
{
	UpdateToProjectPartGroupList(m_PartGroupListBackup);
}
//-------------------------------------------------------------------------------------//
void CProjectGroupConfigWnd::UpdateToProjectPartGroupList()
{	
	UpdateToProjectPartGroupList(m_PartGroupList);
}
//-------------------------------------------------------------------------------------//
int CProjectGroupConfigWnd::GetPartGroupFreeGroupID()//取得零件群組群組編號
{
	size_t     i=0;
	int GroupID = 0;
	int MaxGroupID=-1;
	std::vector<CAOIPartGroup> &PartGroupList=GetPartGroupList();
	const size_t PartGroupCount=PartGroupList.size();	
	for ( i=0; i<PartGroupCount; i++ )
	{
		CAOIPartGroup &PartGroupRef=PartGroupList[i];
		GroupID = PartGroupRef.GetPartGroupGroupID();
		if ( MaxGroupID > GroupID ) { continue; }
		MaxGroupID = GroupID;
	}
	return (MaxGroupID+1);
}
//-------------------------------------------------------------------------------------//
void CProjectGroupConfigWnd::ReadPartGroupListResult()
{
	ReadPartGroupListResult(m_PartGroupList);
}
//-------------------------------------------------------------------------------------//
void CProjectGroupConfigWnd::ReadPartGroupListResult(std::vector<CAOIPartGroup> &PartGroupList)
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }

	size_t       i=0;
	CAOIPartGroup   *PartGroupPtr = NULL;	
	const size_t GroupCount=PartGroupList.size();
	const size_t ProjectPartGroupCount=ProjectPtr->GetProjectPartGroupCount();
	if ( GroupCount != ProjectPartGroupCount ) { return; }
	
	for ( i=0; i<GroupCount; i++ )
	{
		PartGroupPtr = ProjectPtr->GetProjectPartGroupPtr(i, false);
		if ( NULL == PartGroupPtr ) { continue; }
		PartGroupList[i] = *PartGroupPtr;
	}
	return;

}
//-------------------------------------------------------------------------------------//
void CProjectGroupConfigWnd::UpdateToProjectPartGroupList(const std::vector<CAOIPartGroup> &PartGroupList)
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }

	size_t       i=0;
	CAOIPartGroup   *PartGroupPtr = NULL;	
	const size_t GroupCount=PartGroupList.size();
	
	ProjectPtr->ClearProjectAllPartGroups();
	for ( i=0; i<GroupCount; i++ )
	{
		PartGroupPtr = PartGroupList[i].ClonePartGroupObj();
		if ( NULL == PartGroupPtr ) { continue; }
		ProjectPtr->AddProjectPartGroupPtr(PartGroupPtr);		
	}
	return;
}
//-------------------------------------------------------------------------------------//
CListBox& CProjectGroupConfigWnd::GetWndListBox()
{
	return m_WndListBox;
}
//-------------------------------------------------------------------------------------//
CListBox& CProjectGroupConfigWnd::GetNodeListBox()
{
	return m_NodeListBox;
}
//-------------------------------------------------------------------------------------//
CThisListCtrl_53& CProjectGroupConfigWnd::GetGroupListWnd()
{
	return m_GroupListWnd;
}
//-------------------------------------------------------------------------------------//
CThisListCtrl_53& CProjectGroupConfigWnd::GetNodeParamListWnd()
{
	return m_NodeParamListWnd;
}
//-------------------------------------------------------------------------------------//
CThisListCtrl_53& CProjectGroupConfigWnd::GetGroupParamListWnd()
{
	return m_GroupParamListWnd;
}
//-------------------------------------------------------------------------------------//
int CProjectGroupConfigWnd::GetGroupListWndItem()
{
	CThisListCtrl_53 &ListCtrl = GetGroupListWnd();
	if ( ListCtrl.GetSafeHwnd() == NULL ) { return -1; }
	return ListCtrl.GetNextItem(-1, LVNI_FOCUSED);
}
//-------------------------------------------------------------------------------------//
bool CProjectGroupConfigWnd::BuildGroupListWndHeader()
{
	CString str;
	int   nCol = 0;
	int width  = 64;
	int width2 = 64;
	RECT      Rect;	
	const int Align = LVCFMT_LEFT;//LVCFMT_CENTER;LVCFMT_RIGHT, LVCFMT_LEFT
	const int AlignC = LVCFMT_CENTER;//LVCFMT_CENTER;LVCFMT_RIGHT, LVCFMT_LEFT
	{
		CThisListCtrl_53 &ListCtrl = GetGroupListWnd();

		ListCtrl.GetClientRect(&Rect);
		width2 = 48;
		width = (Rect.right-Rect.left-width2-width2-width2-8);

		str = _T("Index");
		str = AOIDataDefine.GetIDText();
		ListCtrl.InsertColumn(nCol, str, Align, width2);
		nCol ++;

		str = _T("Name");
		str = AOIDataDefine.GetNameText();
		ListCtrl.InsertColumn(nCol, str, AlignC, width);
		nCol ++;
		
		str = _T("Group ID");		
		str = AOIDataDefine.GetGroupIDText();
		ListCtrl.InsertColumn(nCol, str, AlignC, width2*2);
		nCol ++;		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectGroupConfigWnd::BuildGroupListWnd()
{	
	ClearNodeParamListWnd();
	ClearGroupParamListWnd();
	CThisListCtrl_53 &ListCtrl = GetGroupListWnd();
	m_StopGroupListBeSelected = true;	
	JetAPI::ClearListCtrl(ListCtrl, FALSE);	
	m_StopGroupListBeSelected = false;

	ClearListBox(GetWndListBox());
	ClearListBox(GetNodeListBox());
	CWnd::SetDlgItemText(PROGRPCFG_GROUP_NODE_EDIT1, _T(""));
	CWnd::SetDlgItemText(PROGRPCFG_GROUP_NODE_EDIT2, _T(""));

	if ( GetLockUIWnd() == true ) { return true; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }		
	
	size_t       i=0;
	CString      str;
	int          nItem=0;	
	CString      strAct;
	CString      strGroupID;	
	CString      strGroupName;	
	CAOIPartGroup   *PartGroupPtr = NULL;
	const int    nSubItem1 = 1;
	const int    nSubItem2 = 2;
	std::vector<CAOIPartGroup>  &PartGroupList=GetPartGroupList();
	const size_t GroupCount=PartGroupList.size();
	
	nItem=0;
	ListCtrl.SetRedraw(FALSE);
	m_StopGroupListBeSelected = true;
	for ( i=0; i<GroupCount; i++ )
	{		
		PartGroupPtr = &(PartGroupList[i]);
		if ( NULL == PartGroupPtr ) { continue; }	

		str.Format(_T("%d"), i+1);		
		strGroupName = PartGroupPtr->GetPartGroupName();
		strGroupID.Format(_T("%d"), PartGroupPtr->GetPartGroupGroupID_UI());

		ListCtrl.InsertItem(nItem, str);
		ListCtrl.SetItemData(nItem, i);				
		ListCtrl.SetItemText(nItem, nSubItem1, strGroupName);
		ListCtrl.SetItemText(nItem, nSubItem2, strGroupID);

		nItem++;
	}
	m_StopGroupListBeSelected = false;
	ListCtrl.SetRedraw(TRUE);

	if ( nItem > 0 ) 
	{
		nItem = 0;
		ListCtrl.SetItemState(nItem, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
		UpdateGroupParamUI(&PartGroupList[0]);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectGroupConfigWnd::UpdateGroupListWnd()
{	
	CThisListCtrl_53 &ListCtrl = GetGroupListWnd();	
	if ( GetLockUIWnd() == true ) { return true; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }		
	
	size_t       i=0;
	CString      str;
	int          nIdx=0;
	int          nItem=0;	
	CString      strAct;
	CString      strGroupID;	
	CString      strGroupName;	
	CAOIPartGroup   *PartGroupPtr = NULL;
	const int    nSubItem1 = 1;
	const int    nSubItem2 = 2;
	std::vector<CAOIPartGroup>  &PartGroupList=GetPartGroupList();
	const int ItemCount=ListCtrl.GetItemCount();
	const size_t GroupCount=PartGroupList.size();	
	
	nItem=0;
	ListCtrl.SetRedraw(FALSE);
	m_StopGroupListBeSelected = true;
	for ( i=0; i<ItemCount; i++ )
	{		
		nItem = i;		
		nIdx = (int)(ListCtrl.GetItemData(nItem));
		if ( nIdx<0 || nIdx>=GroupCount ) { continue; }
		PartGroupPtr = &(PartGroupList[nIdx]);
		if ( NULL == PartGroupPtr )
		{
			strGroupID = _T("");
			strGroupName = _T("");
		}
		else
		{
			strGroupName = PartGroupPtr->GetPartGroupName();
			strGroupID.Format(_T("%d"), PartGroupPtr->GetPartGroupGroupID_UI());
		}
		ListCtrl.SetItemText(nItem, nSubItem1, strGroupName);
		ListCtrl.SetItemText(nItem, nSubItem2, strGroupID);
	}
	m_StopGroupListBeSelected = false;
	ListCtrl.SetRedraw(TRUE);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectGroupConfigWnd::UpdateGroupListWnd(PART_GROUP_PARAM_ID ParamID)
{
	if ( PART_GROUP_PARAM_GROUP_ID==ParamID || PART_GROUP_PARAM_GROUP_NAME==ParamID )
	{	UpdateGroupListWnd();	}
	return true;
}
//-------------------------------------------------------------------------------------//
int CProjectGroupConfigWnd::GetNodeParamListWndItem()
{
	CThisListCtrl_53 &ListCtrl = GetNodeParamListWnd();
	if ( ListCtrl.GetSafeHwnd() == NULL ) { return -1; }
	return ListCtrl.GetNextItem(-1, LVNI_FOCUSED);
}
//-------------------------------------------------------------------------------------//
bool CProjectGroupConfigWnd::BuildNodeParamListWndHeader()
{
	CString str;
	int   nCol = 0;
	int width  = 64;
	int width2 = 64;
	RECT      Rect;	
	const int Align = LVCFMT_LEFT;//LVCFMT_CENTER;LVCFMT_RIGHT, LVCFMT_LEFT
	const int AlignC = LVCFMT_CENTER;//LVCFMT_CENTER;LVCFMT_RIGHT, LVCFMT_LEFT
	{
		CThisListCtrl_53 &ListCtrl = GetNodeParamListWnd();

		ListCtrl.GetClientRect(&Rect);
		width2 = 48;
		width = (Rect.right-Rect.left-width2-8)/3;		

		str = _T("Index");
		str = AOIDataDefine.GetIDText();
		ListCtrl.InsertColumn(nCol, str, Align, width2);
		nCol ++;

		str = _T("Item");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, AlignC, width);
		nCol ++;
		
		str = _T("Information");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, AlignC, width*2);
		nCol ++;		
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectGroupConfigWnd::BuildNodeParamList(CAOIPartGroup *GroupPtr, TPartGroupNode &GroupNode, int FromID)
{
	ClearNodeParamListWnd();
	CParamList &ParamList = m_NodeParamList;	
	if ( NULL == GroupPtr ) { return true; }	

	int       i=0;
	int       intValue=0;
	CString   str;
	CString   strValue;	
	CString   strCaption;	
	CString   strDescription;
	int       nItem=0;
	bool      bAddMapDirParam = false;
	bool      bAddMapCadParam = false;
	CParamUni    ParamUnit;	
	PART_GROUP_PARAM_ID ParamID;	
	const int nSubItem = 1;		
	PART_GROUP_MODE  PartGroupMode=GroupPtr->GetPartGroupMode();
	
	switch ( PartGroupMode )
	{
	case PART_GROUP_DIST_GROUP_COORD_MAP:
		switch ( FromID )
		{
		case NODE_FROM_1:
			break;
		case NODE_FROM_2:		
			bAddMapDirParam = true;
			break;
		case NODE_FROM_LIST:
			bAddMapCadParam = true;
			break;
		}
		break;
	}	
	if ( GetPartGroupNodeName(GroupNode, false, strValue) == true )
	{
		ParamUnit = CParamUni();
		str = _T("Node Name");		
		strCaption = LoadMultiLanguageString(str, str);
		ParamUnit.SetCaption(strCaption);
		ParamID = PART_GROUP_NODE_BEGIN;	
		ParamUnit.SetParamID((UINT)(ParamID));	
		ParamUnit.SetValue_STR(strValue);		
		ParamUnit.SetDesction(strDescription);
		ParamUnit.SetReadOnly(true);
		ParamList.push_back(ParamUnit);
	}

	if ( true == bAddMapDirParam )
	{
		ParamUnit = CParamUni();
		str = _T("Map Direction");		
		strCaption = LoadMultiLanguageString(str, str);
		ParamUnit.SetCaption(strCaption);
		ParamID = PART_GROUP_NODE_MAP_DIR_MODE;	
		ParamUnit.SetParamID((UINT)(ParamID));
		ParamUnit.AddSelItem(PART_GROUP_MAP_DIR_NONE, _T("None"));
		ParamUnit.AddSelItem(PART_GROUP_MAP_DIR_POS_X, _T("+ X"));
		ParamUnit.AddSelItem(PART_GROUP_MAP_DIR_POS_Y, _T("+ Y"));
		//ParamUnit.AddSelItem(PART_GROUP_MAP_DIR_NEG_X, _T("- X"));
		//ParamUnit.AddSelItem(PART_GROUP_MAP_DIR_NEG_Y, _T("- Y"));
		ParamUnit.SetValue_SEL(GroupNode.MapDirMode);		
		ParamUnit.SetDesction(strDescription);
		ParamList.push_back(ParamUnit);
	}

	if ( true == bAddMapCadParam )
	{
		ParamUnit = CParamUni();
		str = _T("User Map Cad Enabled");
		strCaption = LoadMultiLanguageString(str, str);
		ParamUnit.SetCaption(strCaption);
		ParamID = PART_GROUP_NODE_USER_MAP_CAD_ENABLED;	
		ParamUnit.SetParamID((UINT)(ParamID));
		AOIDataDefine.BuildEnableDisableParamUni(ParamUnit);
		ParamUnit.SetValue_SEL(GroupNode.UserMapCadEnable);		
		ParamUnit.SetDesction(strDescription);
		ParamList.push_back(ParamUnit);

		ParamUnit = CParamUni();
		str = _T("User Map Cad Pos X");
		strCaption = LoadMultiLanguageString(str, str);
		ParamUnit.SetCaption(strCaption);
		ParamID = PART_GROUP_NODE_USER_MAP_CAD_POS_X;	
		ParamUnit.SetParamID((UINT)(ParamID));	
		ParamUnit.SetValue_DBL(GroupNode.UserMapCadPosX);		
		ParamUnit.SetDesction(strDescription);
		ParamList.push_back(ParamUnit);
	
		ParamUnit = CParamUni();
		str = _T("User Map Cad Pos Y");
		strCaption = LoadMultiLanguageString(str, str);
		ParamUnit.SetCaption(strCaption);
		ParamID = PART_GROUP_NODE_USER_MAP_CAD_POS_Y;	
		ParamUnit.SetParamID((UINT)(ParamID));	
		ParamUnit.SetValue_DBL(GroupNode.UserMapCadPosY);		
		ParamUnit.SetDesction(strDescription);
		ParamList.push_back(ParamUnit);

		ParamUnit = CParamUni();
		str = _T("User Map Cad Dist.");
		strCaption = LoadMultiLanguageString(str, str);
		ParamUnit.SetCaption(strCaption);
		ParamID = PART_GROUP_NODE_USER_MAP_CAD_DIS_L;	
		ParamUnit.SetParamID((UINT)(ParamID));	
		ParamUnit.SetValue_DBL(GroupNode.UserMapCadDisL);		
		ParamUnit.SetDesction(strDescription);
		ParamList.push_back(ParamUnit);
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectGroupConfigWnd::ClearNodeParamListWnd()
{
	m_NodeParamList.clear();
	CThisListCtrl_53 &ListCtrl = GetNodeParamListWnd();	
	m_StopNodeParamListBeSelected = true;
	JetAPI::ClearListCtrl(ListCtrl, FALSE);
	m_StopNodeParamListBeSelected = false;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectGroupConfigWnd::BuildNodeParamListWnd(CAOIPartGroup *GroupPtr, TPartGroupNode &GroupNode, int FromID)
{
	SetActParamUni(NULL);
	ClearNodeParamListWnd();
	CThisListCtrl_53 &ListCtrl = GetNodeParamListWnd();		
	CAOIPartGroup *PartGroupPtr = GroupPtr;
	if ( NULL == PartGroupPtr ) { return true; }
	if ( GroupNode.CheckPartGroupNodeValid() == false ) { return true; }

	//CAOIComponent *ComponentPtr = GroupNode.ComponentPtr;
	//if ( NULL == ComponentPtr ) { return true; }
	//CAOIModel *ModelPtr = ComponentPtr->GetComponentModelPtr();
	//if ( NULL == ModelPtr ) { return true; }
	PART_GROUP_MODE  PartGroupMode=PartGroupPtr->GetPartGroupMode();
	if ( PART_GROUP_DIST_GROUP_COORD_MAP != PartGroupMode )
	{	return true; }

	int           i=0;
	CString       str;
	CString       strIndex;
	CString       strValue;	
	CString       strCaption;	
	int           nItem=0;
	CParamUni    *ParamPtr=NULL;
	const int     nSubItem1 = 1;		
	const int     nSubItem2 = 2;
	
	BuildNodeParamList(GroupPtr, GroupNode, FromID);

	const int ParamCount = (int)(m_NodeParamList.size());

	nItem=0;
	ListCtrl.SetRedraw(FALSE);
	m_StopNodeParamListBeSelected = true;
	for ( i=0; i<ParamCount; i++ )
	{
		ParamPtr = &(m_NodeParamList[i]);
		if ( NULL == ParamPtr ) { continue; }		
		
		ParamPtr->SetListCtrl(&ListCtrl);
		ParamPtr->SetItemIndex(nItem);
		ParamPtr->SetSubItemIndex(nSubItem2);		

		strIndex.Format(_T("%d"), nItem+1);
		strCaption = ParamPtr->GetCaption();
		strValue = ParamPtr->GetParamText();
		ListCtrl.InsertItem(nItem, strIndex);
		ListCtrl.SetItemData(nItem, i);
		ListCtrl.SetItemText(nItem, nSubItem1, strCaption);
		ListCtrl.SetItemText(nItem, nSubItem2, strValue);
		nItem ++;
	}	
	m_StopNodeParamListBeSelected = false;
	ListCtrl.SetRedraw(TRUE);		
	return true;
}
//-------------------------------------------------------------------------------------//
int CProjectGroupConfigWnd::GetGroupParamListWndItem()
{
	CThisListCtrl_53 &ListCtrl = GetGroupParamListWnd();
	if ( ListCtrl.GetSafeHwnd() == NULL ) { return -1; }
	return ListCtrl.GetNextItem(-1, LVNI_FOCUSED);
}
//-------------------------------------------------------------------------------------//
bool CProjectGroupConfigWnd::BuildGroupParamListWndHeader()
{
	CString str;
	int   nCol = 0;
	int width  = 64;
	int width2 = 64;
	RECT      Rect;	
	const int Align = LVCFMT_LEFT;//LVCFMT_CENTER;LVCFMT_RIGHT, LVCFMT_LEFT
	const int AlignC = LVCFMT_CENTER;//LVCFMT_CENTER;LVCFMT_RIGHT, LVCFMT_LEFT
	{
		CThisListCtrl_53 &ListCtrl = GetGroupParamListWnd();

		ListCtrl.GetClientRect(&Rect);
		width2 = 48;
		width = (Rect.right-Rect.left-width2-8)/3;		

		str = _T("Index");
		str = AOIDataDefine.GetIDText();
		ListCtrl.InsertColumn(nCol, str, Align, width2);
		nCol ++;

		str = _T("Item");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, AlignC, width);
		nCol ++;
		
		str = _T("Information");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, AlignC, width*2);
		nCol ++;		
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectGroupConfigWnd::BuildGroupParamList()
{	
	ClearNodeParamListWnd();
	ClearGroupParamListWnd();
	CParamList &ParamList = m_GroupParamList;
	CThisListCtrl_53 &ListCtrl = GetGroupParamListWnd();
	CAOIPartGroup *PartGroupPtr = GetActivePartGroup();
	if ( NULL == PartGroupPtr ) { return true; }

	int       i=0;
	int       intValue=0;
	CString   str;
	CString   strValue;	
	CString   strCaption;	
	CString   strDescription;
	int       nItem=0;
	CParamUni    ParamUnit;	
	PART_GROUP_PARAM_ID ParamID;	
	const int nSubItem = 1;	
	PART_GROUP_MODE GroupMode = PartGroupPtr->GetPartGroupMode();
	
	ParamUnit = CParamUni();
	str = _T("Group ID");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PART_GROUP_PARAM_GROUP_ID;	
	ParamUnit.SetParamID((UINT)(ParamID));	
	ParamUnit.SetValue_INT(PartGroupPtr->GetPartGroupGroupID_UI(), 1);		
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	ParamUnit = CParamUni();
	str = _T("Group Name");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PART_GROUP_PARAM_GROUP_NAME;	
	ParamUnit.SetParamID((UINT)(ParamID));	
	ParamUnit.SetValue_STR(PartGroupPtr->GetPartGroupName());		
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	ParamUnit = CParamUni();
	str = _T("In One Board");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PART_GROUP_PARAM_IN_ONE_BOARD;	
	ParamUnit.SetParamID((UINT)(ParamID));	
	AOIDataDefine.BuildEnableDisableParamUni(ParamUnit);
	ParamUnit.SetValue_SEL(PartGroupPtr->GetPartGroupInOneBoard());	
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//共線性模式
	if ( PART_GROUP_COLINEARITY == GroupMode )
	{
		ParamUnit = CParamUni();
		str = _T("Colinearity Mode");
		strCaption = LoadMultiLanguageString(str, str);
		ParamUnit.SetCaption(strCaption);
		ParamID = PART_GROUP_PARAM_COLINEARITY_MODE;
		ParamUnit.SetParamID((UINT)(ParamID));	
		str = AOIDataDefine.GetPartGroupColinearityeModeText(PART_GROUP_COLINEARITY_X); ParamUnit.AddSelItem(PART_GROUP_COLINEARITY_X, str);
		str = AOIDataDefine.GetPartGroupColinearityeModeText(PART_GROUP_COLINEARITY_Y); ParamUnit.AddSelItem(PART_GROUP_COLINEARITY_Y, str);
		str = AOIDataDefine.GetPartGroupColinearityeModeText(PART_GROUP_COLINEARITY_SKEW); ParamUnit.AddSelItem(PART_GROUP_COLINEARITY_SKEW, str);		
		str = AOIDataDefine.GetPartGroupColinearityeModeText(PART_GROUP_COLINEARITY_HEIGHT); ParamUnit.AddSelItem(PART_GROUP_COLINEARITY_HEIGHT, str);
		ParamUnit.SetValue_SEL(PartGroupPtr->GetColinearityMode());	
		ParamUnit.SetDesction(strDescription);
		ParamList.push_back(ParamUnit);

		ParamUnit = CParamUni();
		str = _T("Target Mode");
		strCaption = LoadMultiLanguageString(str, str);
		ParamUnit.SetCaption(strCaption);
		ParamID = PART_GROUP_PARAM_COLINEARITY_TARGET_MODE;
		ParamUnit.SetParamID((UINT)(ParamID));	
		str = AOIDataDefine.GetPartGroupTargetModeText(PART_GROUP_TARGET_AVE); ParamUnit.AddSelItem(PART_GROUP_TARGET_AVE, str);
		str = AOIDataDefine.GetPartGroupTargetModeText(PART_GROUP_TARGET_MIN); ParamUnit.AddSelItem(PART_GROUP_TARGET_MIN, str);		
		ParamUnit.SetValue_SEL(PartGroupPtr->GetColinearityTargetMode());	
		ParamUnit.SetDesction(strDescription);
		ParamList.push_back(ParamUnit);

		ParamUnit = CParamUni();
		str = _T("Gap USL");
		strCaption = LoadMultiLanguageString(str, str);
		ParamUnit.SetCaption(strCaption);
		ParamID = PART_GROUP_PARAM_COLINEARITY_GAP_USL;	
		ParamUnit.SetParamID((UINT)(ParamID));	
		ParamUnit.SetValue_DBL(PartGroupPtr->GetColinearityGapUSL());		
		ParamUnit.SetDesction(strDescription);
		ParamList.push_back(ParamUnit);

		ParamUnit = CParamUni();
		str = _T("Gap LSL");
		strCaption = LoadMultiLanguageString(str, str);
		ParamUnit.SetCaption(strCaption);
		ParamID = PART_GROUP_PARAM_COLINEARITY_GAP_LSL;	
		ParamUnit.SetParamID((UINT)(ParamID));	
		ParamUnit.SetValue_DBL(PartGroupPtr->GetColinearityGapLSL());		
		ParamUnit.SetDesction(strDescription);
		ParamList.push_back(ParamUnit);
	}

	if ( PART_GROUP_COLINEARITY_TO_LINE == GroupMode )
	{
		ParamUnit = CParamUni();
		str = _T("Colinearity (Line)Mode");
		strCaption = LoadMultiLanguageString(str, str);
		ParamUnit.SetCaption(strCaption);
		ParamID = PART_GROUP_PARAM_COLINEARITY_MODE;
		ParamUnit.SetParamID((UINT)(ParamID));	
		str = AOIDataDefine.GetPartGroupColinearityeModeText(PART_GROUP_COLINEARITY_X); ParamUnit.AddSelItem(PART_GROUP_COLINEARITY_X, str);
		str = AOIDataDefine.GetPartGroupColinearityeModeText(PART_GROUP_COLINEARITY_Y); ParamUnit.AddSelItem(PART_GROUP_COLINEARITY_Y, str);
		str = AOIDataDefine.GetPartGroupColinearityeModeText(PART_GROUP_COLINEARITY_SKEW); ParamUnit.AddSelItem(PART_GROUP_COLINEARITY_SKEW, str);		
		ParamUnit.SetValue_SEL(PartGroupPtr->GetColinearityMode());	
		ParamUnit.SetDesction(strDescription);
		//ParamList.push_back(ParamUnit);
		
		ParamUnit = CParamUni();
		str = _T("Gap Std");
		strCaption = LoadMultiLanguageString(str, str);
		ParamUnit.SetCaption(strCaption);
		ParamID = PART_GROUP_PARAM_COLINEARITY_GAP_STD;	
		ParamUnit.SetParamID((UINT)(ParamID));	
		ParamUnit.SetValue_DBL(PartGroupPtr->GetColinearityGapStd());
		ParamUnit.SetDesction(strDescription);
		ParamList.push_back(ParamUnit);

		ParamUnit = CParamUni();
		str = _T("Gap USL");
		strCaption = LoadMultiLanguageString(str, str);
		ParamUnit.SetCaption(strCaption);
		ParamID = PART_GROUP_PARAM_COLINEARITY_GAP_USL;	
		ParamUnit.SetParamID((UINT)(ParamID));	
		ParamUnit.SetValue_DBL(PartGroupPtr->GetColinearityGapUSL());		
		ParamUnit.SetDesction(strDescription);
		ParamList.push_back(ParamUnit);

		ParamUnit = CParamUni();
		str = _T("Gap LSL");
		strCaption = LoadMultiLanguageString(str, str);
		ParamUnit.SetCaption(strCaption);
		ParamID = PART_GROUP_PARAM_COLINEARITY_GAP_LSL;	
		ParamUnit.SetParamID((UINT)(ParamID));	
		ParamUnit.SetValue_DBL(PartGroupPtr->GetColinearityGapLSL());		
		ParamUnit.SetDesction(strDescription);
		ParamList.push_back(ParamUnit);		
	}

	bool bShowStdX=false;
	bool bShowStdY=false;
	bool bShowGapX=false;
	bool bShowGapY=false;
	bool bShowGapL=false;
	bool bShowAdd=false;
	bool bShowScale=false;
	switch ( GroupMode )
	{
	case PART_GROUP_DIST_PART_TO_PART:
		bShowStdX = true;	
		bShowStdY = true;
		bShowGapX = true;
		bShowGapY = true;
		bShowGapL = true;
		bShowAdd = true;
		break;
	case PART_GROUP_DIST_GROUP_COORD_MAP:
		bShowGapX = true;
		bShowGapY = true;
		bShowGapL = true;
		bShowScale = true;			
		break;
	default:
		bShowStdX = false;
		bShowStdY = false;
		bShowGapX = true;
		bShowGapY = true;
		bShowGapL = true;
		bShowAdd = false;
		bShowScale = false;
		break;
	}

	if ( PART_GROUP_DIST_PART_TO_PART == GroupMode ||
		 PART_GROUP_DIST_PART_NEIGHBOR == GroupMode ||
		 PART_GROUP_DIST_PART_TO_GROUP == GroupMode ||
		 PART_GROUP_DIST_GROUP_TO_PART == GroupMode ||
		 PART_GROUP_DIST_GROUP_COORD_MAP == GroupMode )
		
	{		
		if ( bShowStdX || bShowStdY )
		{			
			//距離偏差啟用絕對值
			ParamUnit = CParamUni();
			str = _T("ABS Enable");
			strCaption = LoadMultiLanguageString(str, str);
			ParamUnit.SetCaption(strCaption);
			ParamID = PART_GROUP_PARAM_DISTANCE_GAP_ENB_ABS;	
			ParamUnit.SetParamID((UINT)(ParamID));	
			AOIDataDefine.BuildEnableDisableParamUni(ParamUnit);
			ParamUnit.SetValue_SEL(PartGroupPtr->GetDistanceGapEnbAbs());		
			ParamUnit.SetDesction(strDescription);
			ParamList.push_back(ParamUnit);

			//距離偏差啟用標準
			ParamUnit = CParamUni();
			str = _T("STD Gap Enable");
			strCaption = LoadMultiLanguageString(str, str);
			ParamUnit.SetCaption(strCaption);
			ParamID = PART_GROUP_PARAM_DISTANCE_GAP_STD_ENB;	
			ParamUnit.SetParamID((UINT)(ParamID));	
			AOIDataDefine.BuildEnableDisableParamUni(ParamUnit);
			ParamUnit.SetValue_SEL(PartGroupPtr->GetDistanceGapStdEnb());		
			ParamUnit.SetDesction(strDescription);
			ParamList.push_back(ParamUnit);			
		}		
		if ( bShowStdX )
		{
			ParamUnit = CParamUni();
			str = _T("X Gap STD");
			strCaption = LoadMultiLanguageString(str, str);
			ParamUnit.SetCaption(strCaption);
			ParamID = PART_GROUP_PARAM_DISTANCE_GAP_STD_X;	
			ParamUnit.SetParamID((UINT)(ParamID));	
			ParamUnit.SetValue_DBL(PartGroupPtr->GetDistanceGapStdX());		
			ParamUnit.SetDesction(strDescription);
			ParamList.push_back(ParamUnit);
		}

		if ( bShowStdY )
		{
			ParamUnit = CParamUni();
			str = _T("Y Gap STD");
			strCaption = LoadMultiLanguageString(str, str);
			ParamUnit.SetCaption(strCaption);
			ParamID = PART_GROUP_PARAM_DISTANCE_GAP_STD_Y;	
			ParamUnit.SetParamID((UINT)(ParamID));	
			ParamUnit.SetValue_DBL(PartGroupPtr->GetDistanceGapStdY());		
			ParamUnit.SetDesction(strDescription);
			ParamList.push_back(ParamUnit);
		}

		if ( bShowGapX )
		{
			ParamUnit = CParamUni();
			str = _T("X Gap Enable");
			strCaption = LoadMultiLanguageString(str, str);
			ParamUnit.SetCaption(strCaption);
			ParamID = PART_GROUP_PARAM_DISTANCE_GAP_ENB_X;	
			ParamUnit.SetParamID((UINT)(ParamID));	
			str = AOIDataDefine.GetEnableText();	ParamUnit.AddSelItem(FN_ENABLE, str);		
			str = AOIDataDefine.GetDisableText();	ParamUnit.AddSelItem(FN_DISABLE, str);		
			ParamUnit.SetValue_SEL(PartGroupPtr->GetDistanceGapEnbX());		
			ParamUnit.SetDesction(strDescription);
			ParamList.push_back(ParamUnit);

			ParamUnit = CParamUni();
			str = _T("X Gap USL");
			strCaption = LoadMultiLanguageString(str, str);
			ParamUnit.SetCaption(strCaption);
			ParamID = PART_GROUP_PARAM_DISTANCE_GAP_USL_X;	
			ParamUnit.SetParamID((UINT)(ParamID));	
			ParamUnit.SetValue_DBL(PartGroupPtr->GetDistanceGapUSLX());		
			ParamUnit.SetDesction(strDescription);
			ParamList.push_back(ParamUnit);

			ParamUnit = CParamUni();
			str = _T("X Gap LSL");
			strCaption = LoadMultiLanguageString(str, str);
			ParamUnit.SetCaption(strCaption);
			ParamID = PART_GROUP_PARAM_DISTANCE_GAP_LSL_X;	
			ParamUnit.SetParamID((UINT)(ParamID));	
			ParamUnit.SetValue_DBL(PartGroupPtr->GetDistanceGapLSLX());		
			ParamUnit.SetDesction(strDescription);
			ParamList.push_back(ParamUnit);
		}

		if ( bShowGapY )
		{
			ParamUnit = CParamUni();
			str = _T("Y Gap Enable");
			strCaption = LoadMultiLanguageString(str, str);
			ParamUnit.SetCaption(strCaption);
			ParamID = PART_GROUP_PARAM_DISTANCE_GAP_ENB_Y;	
			ParamUnit.SetParamID((UINT)(ParamID));	
			str = AOIDataDefine.GetEnableText();	ParamUnit.AddSelItem(FN_ENABLE, str);		
			str = AOIDataDefine.GetDisableText();	ParamUnit.AddSelItem(FN_DISABLE, str);		
			ParamUnit.SetValue_SEL(PartGroupPtr->GetDistanceGapEnbY());		
			ParamUnit.SetDesction(strDescription);
			ParamList.push_back(ParamUnit);

			ParamUnit = CParamUni();
			str = _T("Y Gap USL");
			strCaption = LoadMultiLanguageString(str, str);
			ParamUnit.SetCaption(strCaption);
			ParamID = PART_GROUP_PARAM_DISTANCE_GAP_USL_Y;	
			ParamUnit.SetParamID((UINT)(ParamID));	
			ParamUnit.SetValue_DBL(PartGroupPtr->GetDistanceGapUSLY());		
			ParamUnit.SetDesction(strDescription);
			ParamList.push_back(ParamUnit);

			ParamUnit = CParamUni();
			str = _T("Y Gap LSL");
			strCaption = LoadMultiLanguageString(str, str);
			ParamUnit.SetCaption(strCaption);
			ParamID = PART_GROUP_PARAM_DISTANCE_GAP_LSL_Y;	
			ParamUnit.SetParamID((UINT)(ParamID));	
			ParamUnit.SetValue_DBL(PartGroupPtr->GetDistanceGapLSLY());		
			ParamUnit.SetDesction(strDescription);
			ParamList.push_back(ParamUnit);
		}

		if ( bShowGapL )
		{
			ParamUnit = CParamUni();
			str = _T("L Gap Enable");
			strCaption = LoadMultiLanguageString(str, str);
			ParamUnit.SetCaption(strCaption);
			ParamID = PART_GROUP_PARAM_DISTANCE_GAP_ENB_L;	
			ParamUnit.SetParamID((UINT)(ParamID));	
			str = AOIDataDefine.GetEnableText();	ParamUnit.AddSelItem(FN_ENABLE, str);		
			str = AOIDataDefine.GetDisableText();	ParamUnit.AddSelItem(FN_DISABLE, str);		
			ParamUnit.SetValue_SEL(PartGroupPtr->GetDistanceGapEnbL());		
			ParamUnit.SetDesction(strDescription);
			ParamList.push_back(ParamUnit);

			ParamUnit = CParamUni();
			str = _T("L Gap USL");
			strCaption = LoadMultiLanguageString(str, str);
			ParamUnit.SetCaption(strCaption);
			ParamID = PART_GROUP_PARAM_DISTANCE_GAP_USL_L;	
			ParamUnit.SetParamID((UINT)(ParamID));	
			ParamUnit.SetValue_DBL(PartGroupPtr->GetDistanceGapUSLL());		
			ParamUnit.SetDesction(strDescription);
			ParamList.push_back(ParamUnit);

			ParamUnit = CParamUni();
			str = _T("L Gap LSL");
			strCaption = LoadMultiLanguageString(str, str);
			ParamUnit.SetCaption(strCaption);
			ParamID = PART_GROUP_PARAM_DISTANCE_GAP_LSL_L;	
			ParamUnit.SetParamID((UINT)(ParamID));	
			ParamUnit.SetValue_DBL(PartGroupPtr->GetDistanceGapLSLL());		
			ParamUnit.SetDesction(strDescription);
			ParamList.push_back(ParamUnit);		
		}

		if ( bShowAdd )
		{
			ParamUnit = CParamUni();
			str = _T("X Gap Add");
			strCaption = LoadMultiLanguageString(str, str);
			ParamUnit.SetCaption(strCaption);
			ParamID = PART_GROUP_PARAM_DISTANCE_GAP_ADD_X;	
			ParamUnit.SetParamID((UINT)(ParamID));	
			ParamUnit.SetValue_DBL(PartGroupPtr->GetDistanceGapAddX());		
			ParamUnit.SetDesction(strDescription);
			ParamList.push_back(ParamUnit);

			ParamUnit = CParamUni();
			str = _T("Y Gap Add");
			strCaption = LoadMultiLanguageString(str, str);
			ParamUnit.SetCaption(strCaption);
			ParamID = PART_GROUP_PARAM_DISTANCE_GAP_ADD_Y;	
			ParamUnit.SetParamID((UINT)(ParamID));	
			ParamUnit.SetValue_DBL(PartGroupPtr->GetDistanceGapAddY());		
			ParamUnit.SetDesction(strDescription);
			ParamList.push_back(ParamUnit);
		}

		if ( bShowScale )
		{
			ParamUnit = CParamUni();
			str = _T("X Gap Scale");
			strCaption = LoadMultiLanguageString(str, str);
			ParamUnit.SetCaption(strCaption);
			ParamID = PART_GROUP_PARAM_DISTANCE_GAP_SCALE_X;	
			ParamUnit.SetParamID((UINT)(ParamID));	
			ParamUnit.SetValue_DBL(PartGroupPtr->GetDistanceGapScaleX());		
			ParamUnit.SetDesction(strDescription);
			ParamList.push_back(ParamUnit);

			ParamUnit = CParamUni();
			str = _T("Y Gap Scale");
			strCaption = LoadMultiLanguageString(str, str);
			ParamUnit.SetCaption(strCaption);
			ParamID = PART_GROUP_PARAM_DISTANCE_GAP_SCALE_Y;	
			ParamUnit.SetParamID((UINT)(ParamID));	
			ParamUnit.SetValue_DBL(PartGroupPtr->GetDistanceGapScaleY());		
			ParamUnit.SetDesction(strDescription);
			ParamList.push_back(ParamUnit);

			ParamUnit = CParamUni();
			str = _T("L Gap Scale");
			strCaption = LoadMultiLanguageString(str, str);
			ParamUnit.SetCaption(strCaption);
			ParamID = PART_GROUP_PARAM_DISTANCE_GAP_SCALE_L;	
			ParamUnit.SetParamID((UINT)(ParamID));	
			ParamUnit.SetValue_DBL(PartGroupPtr->GetDistanceGapScaleL());		
			ParamUnit.SetDesction(strDescription);
			ParamList.push_back(ParamUnit);
		}	
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectGroupConfigWnd::ClearGroupParamListWnd()
{
	m_GroupParamList.clear();
	CThisListCtrl_53 &ListCtrl = GetGroupParamListWnd();	
	m_StopGroupParamListBeSelected = true;
	JetAPI::ClearListCtrl(ListCtrl, FALSE);
	m_StopGroupParamListBeSelected = false;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectGroupConfigWnd::BuildGroupParamListWnd()
{
	SetActParamUni(NULL);
	ClearNodeParamListWnd();
	ClearGroupParamListWnd();
	CThisListCtrl_53 &ListCtrl = GetGroupParamListWnd();		
	CAOIPartGroup *PartGroupPtr = GetActivePartGroup();
	if ( NULL == PartGroupPtr ) { return true; }

	int           i=0;
	CString       str;
	CString       strIndex;
	CString       strValue;	
	CString       strCaption;	
	int           nItem=0;
	CParamUni    *ParamPtr=NULL;
	const int     nSubItem1 = 1;		
	const int     nSubItem2 = 2;
	
	BuildGroupParamList();

	const int ParamCount = (int)(m_GroupParamList.size());

	nItem=0;
	ListCtrl.SetRedraw(FALSE);
	m_StopGroupParamListBeSelected = true;
	for ( i=0; i<ParamCount; i++ )
	{
		ParamPtr = &(m_GroupParamList[i]);
		if ( NULL == ParamPtr ) { continue; }		
		
		ParamPtr->SetListCtrl(&ListCtrl);
		ParamPtr->SetItemIndex(nItem);
		ParamPtr->SetSubItemIndex(nSubItem2);		

		strIndex.Format(_T("%d"), nItem+1);
		strCaption = ParamPtr->GetCaption();
		strValue = ParamPtr->GetParamText();
		ListCtrl.InsertItem(nItem, strIndex);
		ListCtrl.SetItemData(nItem, i);
		ListCtrl.SetItemText(nItem, nSubItem1, strCaption);
		ListCtrl.SetItemText(nItem, nSubItem2, strValue);
		nItem ++;
	}	
	m_StopGroupParamListBeSelected = false;
	ListCtrl.SetRedraw(TRUE);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectGroupConfigWnd::ExecReleaseParamCtrl()
{
	//m_BtnCtrl.ShowWindow(SW_HIDE);
	m_GroupParamEdit.ShowWindow(SW_HIDE);	
	m_GroupParamCombox.ShowWindow(SW_HIDE);
	m_GroupParamEdit.SetWindowText(_T(""));
	m_GroupParamListWnd.SetFocus();
	return true;
}
//-------------------------------------------------------------------------------------//	
void CProjectGroupConfigWnd::SetDescriptionText(const CParamUni *Ptr)
{
	if ( NULL == Ptr ) { return; }

}
//-------------------------------------------------------------------------------------//	
bool CProjectGroupConfigWnd::ExecItemchangedParamListWnd(CParamList &ParamList, CThisListCtrl_53 &ListCtrl, int nItem)
{
	const int    ItemCount = ListCtrl.GetItemCount();
	if ( nItem<0 || nItem>=ItemCount ) { return false; }
	
	const size_t ParamIndex = ListCtrl.GetItemData(nItem);
	const size_t ParamCount = ParamList.size();
	if ( ParamIndex >= ParamCount ) { return false; }	
	CParamUni *ParamPtr=&(ParamList[ParamIndex]);	
	CWnd      *BtnWndPtr = ParamPtr->GetBtnWndPtr();	
	const int nSubItem = ParamPtr->GetSubItemIndex();
	
	SetDescriptionText(ParamPtr);
	if ( NULL!=BtnWndPtr && BtnWndPtr->GetSafeHwnd()!=NULL) 	
	{
		CRect ItemRect;
		if ( ListCtrl.GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, ItemRect) == TRUE )
		{	
			SIZE BtnSize={0};
			RECT BtnRect={0};
			RECT CtrlRect = ItemRect;
			ListCtrl.ClientToScreen(&CtrlRect);
			this->ScreenToClient(&CtrlRect);
			BtnWndPtr->GetWindowRect(&BtnRect);
			JetAPI::GetRectSize(BtnRect, BtnSize);
			BtnRect = CtrlRect;			
			BtnRect.left = BtnRect.right-BtnSize.cx;
			BtnWndPtr->MoveWindow(&BtnRect, FALSE);
			BtnWndPtr->ShowWindow(SW_SHOW);			
			BtnWndPtr->BringWindowToTop();
			ListCtrl.UpdateWindow();
			BtnWndPtr->Invalidate();
			SetActParamUni(ParamPtr);			
		}		
	}
	else
	{	
		//m_BtnCtrl.ShowWindow(SW_HIDE);	
	}	

	return true;
}
//-------------------------------------------------------------------------------------//	
bool CProjectGroupConfigWnd::ExecDblclkParamListWnd(CParamList &ParamList, CThisListCtrl_53 &ListCtrl, int nItem, int nSubItem)
{
	const int    ItemCount = ListCtrl.GetItemCount();
	if ( nItem<0 || nItem>=ItemCount ) { return false; }
	if ( nSubItem < SETTING_COL ) { return true; }
	
	const size_t ParamIndex = ListCtrl.GetItemData(nItem);
	const size_t ParamCount = ParamList.size();
	if ( ParamIndex >= ParamCount ) { return false; }		
	
	size_t          i=0;
	int             nSelIdx=0;
	int             nValue=0;
	CRect           ItemRect;
	RECT            CtrlRect={0};	
	CString         ItemText;	
	const int       Offset = 2;
	CParamUni      *ParamPtr=&(ParamList[ParamIndex]);		
	const bool      ReadOnly = ParamPtr->GetReadOnly();	
	if ( true == ReadOnly ) { return true; }	
	PARAM_DATA_TYPE  DataType = ParamPtr->GetDataType();

	const size_t    SelItemCount = ParamPtr->GetSelItemCount();
	if ( ListCtrl.GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, ItemRect) == FALSE )
	{	return false; }
	ItemText = ListCtrl.GetItemText(nItem, nSubItem);

	CtrlRect = ItemRect;
	ListCtrl.ClientToScreen(&CtrlRect);
	this->ScreenToClient(&CtrlRect);
	::OffsetRect(&CtrlRect, 0, -2);
	//m_BtnCtrl.ShowWindow(SW_HIDE);
	SetActParamUni(ParamPtr);
	if ( PARAM_DATA_SEL == DataType )
	{
		CComboBox &ComboxCtrl=m_GroupParamCombox;
		if ( ComboxCtrl.GetSafeHwnd() != NULL )
		{
			nSelIdx = 0;
			JetAPI::ClearCombox(ComboxCtrl);
			for ( i=0; i<SelItemCount; i++ )
			{
				if ( ParamPtr->GetSelItem(i, true, nValue, ItemText) == false ) { continue; }
				ComboxCtrl.InsertString(nSelIdx, ItemText);
				ComboxCtrl.SetItemData(nSelIdx, nValue);
				nSelIdx ++;
			}			
			JetAPI::SetComboxCurSel(ComboxCtrl, ParamPtr->GetSelParam());
			ComboxCtrl.MoveWindow(&CtrlRect, FALSE);			
			ComboxCtrl.SetFocus();
			ComboxCtrl.ShowDropDown();
			ComboxCtrl.ShowWindow(SW_SHOW);
			ComboxCtrl.BringWindowToTop();			
			ListCtrl.UpdateWindow();
			ComboxCtrl.Invalidate();
		}	
	}
	else
	{
		CEdit &EditCtrl=m_GroupParamEdit;
		if ( EditCtrl.GetSafeHwnd() != NULL )
		{	
			::OffsetRect(&CtrlRect, 1, 1);
			::InflateRect(&CtrlRect, Offset, Offset);			
			EditCtrl.SetWindowText(ItemText);
			EditCtrl.MoveWindow(&CtrlRect, FALSE);
			EditCtrl.SetFocus();
			EditCtrl.SetSel(0,-1);			
			EditCtrl.ShowWindow(SW_SHOW);	
			EditCtrl.BringWindowToTop();
			ListCtrl.UpdateWindow();
			EditCtrl.Invalidate();			
		}
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectGroupConfigWnd::ExecUpdateParamByEdit()
{
	CAOIPartGroup *PartGroupPtr = GetActivePartGroup();
	if ( NULL == PartGroupPtr ) { return true; }	
	CParamUni *ParamPtr = GetActParamUni();
	if ( NULL == ParamPtr ) { return true; }	
	PARAM_DATA_TYPE  DataType = ParamPtr->GetDataType();	
	if ( PARAM_DATA_SEL == DataType ) { return true; }
	SetActParamUni(NULL);

	CString ItemText;	
	CEdit &EditCtrl=m_GroupParamEdit;
	PART_GROUP_PARAM_ID ParamID = (PART_GROUP_PARAM_ID)(ParamPtr->GetParamID());	
	EditCtrl.GetWindowText(ItemText);
	if ( ParamPtr->SetNewValue(ItemText) == false )
	{		
		ItemText = ParamPtr->GetParamText();
		EditCtrl.SetWindowText(ItemText);
		return false;
	}

	if ( PartGroupPtr->CheckPartGroupNodeParamID(ParamID) == true )
	{
		TPartGroupNode *NodePtr=GetSelPartGroupNodePtr();
		if ( NULL == NodePtr )
		{	return false; }
		if ( PartGroupPtr->SetPartGroupNodeParameterStringByID(*NodePtr, ParamID, ItemText) == false )
		{	return false; }		
	}
	else
	{
		if ( PartGroupPtr->SetPartGroupParameterStringByID(ParamID, ItemText) == false )
		{	return false; }
		UpdateGroupListWnd(ParamID);
	}
	
	CThisListCtrl_53 *pListCtrl = (CThisListCtrl_53*)(ParamPtr->GetListCtrl());
	const int nItem = ParamPtr->GetItemIndex();
	const int nSubItem = ParamPtr->GetSubItemIndex();	
	if ( NULL != pListCtrl )
	{
		const int ItemCount = pListCtrl->GetItemCount();
		if ( nItem>=0 && nItem<ItemCount )
		{	
			pListCtrl->SetItemText(nItem, nSubItem, ItemText);	
			pListCtrl->SetFocus();
		}
	}			
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectGroupConfigWnd::ExecUpdateParamByCombox()
{
	CAOIPartGroup *PartGroupPtr = GetActivePartGroup();
	if ( NULL == PartGroupPtr ) { return true; }	
	CParamUni *ParamPtr = GetActParamUni();
	if ( NULL == ParamPtr ) { return true; }	
	PARAM_DATA_TYPE  DataType = ParamPtr->GetDataType();
	if ( PARAM_DATA_SEL != DataType ) { return true; }
	SetActParamUni(NULL);

	CString ItemText;	
	CComboBox &ComboxCtrl=m_GroupParamCombox;
	PART_GROUP_PARAM_ID ParamID = (PART_GROUP_PARAM_ID)(ParamPtr->GetParamID());
	const int nCurSel = ComboxCtrl.GetCurSel();
	if ( nCurSel < 0 ) { return true; }
	const int Param = (int)(ComboxCtrl.GetItemData(nCurSel));
	if ( Param == ParamPtr->GetSelParam() ) { return false; }
	ItemText.Format(_T("%d"),Param);	
	if ( ParamPtr->SetNewValue_SEL(Param) == false )
	{	return false; }

	if ( PartGroupPtr->CheckPartGroupNodeParamID(ParamID) == true )
	{
		TPartGroupNode *NodePtr=GetSelPartGroupNodePtr();
		if ( NULL == NodePtr )
		{	return false; }
		if ( PartGroupPtr->SetPartGroupNodeParameterStringByID(*NodePtr, ParamID, ItemText) == false )
		{	return false; }				
	}
	else
	{
		if ( PartGroupPtr->SetPartGroupParameterStringByID(ParamID, ItemText) == false )
		{	return false; }	
		UpdateGroupListWnd(ParamID);
	}
	
	ItemText = ParamPtr->GetParamText();
	CThisListCtrl_53 *pListCtrl = (CThisListCtrl_53*)(ParamPtr->GetListCtrl());
	const int nItem = ParamPtr->GetItemIndex();
	const int nSubItem = ParamPtr->GetSubItemIndex();	
	if ( NULL != pListCtrl )
	{
		const int ItemCount = pListCtrl->GetItemCount();
		if ( nItem<0 || nItem>=ItemCount ) { return true; }	
		pListCtrl->SetItemText(nItem, nSubItem, ItemText);	
		pListCtrl->SetFocus();
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectGroupConfigWnd::ClearListBox(CListBox &ListBox)
{
	if ( ListBox.GetSafeHwnd() == NULL ) { return true; }

	int       i=0;
	const int Count=ListBox.GetCount();
	for ( i=0; i<Count; i++ )
	{	ListBox.DeleteString(Count-i-1);	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectGroupConfigWnd::BuildWndListBox(TPartGroupNode &GroupNode, int FromID)
{
	CListBox &WndListBox=GetWndListBox();	
	ClearListBox(WndListBox);
	m_NodeFromeID = FromID;
	CAOIComponent *ComponentPtr = GroupNode.ComponentPtr;
	if ( NULL == ComponentPtr ) { return true; }
	CAOIModel *ModelPtr = ComponentPtr->GetComponentModelPtr();
	if ( NULL == ModelPtr ) { return true; }

	CString      str;
	CString      strDefect;
	size_t       i=0;
	int          nItem=0;
	bool         bFit=false;
	ALG_TYPE     AlgType;
	WND_DEFECT_ID WndDefectID;
	CAOIWnd     *WndPtr = NULL;
	CAOILand    *LandPtr = NULL;
	const int    WndIdx = GroupNode.ModelWndIndex;
	const size_t WndCount=ModelPtr->GetModelWndCount();

	nItem=0;	
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = ModelPtr->GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		LandPtr = WndPtr->GetWndLandPtr();
		if ( NULL != LandPtr ) { continue; }
		AlgType = WndPtr->GetWndAlgType();
		switch ( AlgType )
		{
		case ALG_OBJECT_MEASURE:
		case ALG_MODEL_MATCH:
		case ALG_IMAGE_MATCH:
		case ALG_EDGE_SEARCH:
			bFit = true;
			break;
		default:
			bFit = false;
			break;
		}
		if ( false == bFit ) { continue; }
		WndDefectID = WndPtr->GetWndDefectID();
		strDefect = AOIDataDefine.GetWndDefectIDText(WndDefectID);
		if ( i == WndIdx )
		{	str.Format(_T("%04d#%s***"), i+1, strDefect); }
		else
		{	str.Format(_T("%04d#%s"), i+1, strDefect); }

		WndListBox.AddString(str);
		WndListBox.SetItemData(nItem, i);
		nItem ++;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectGroupConfigWnd::EnableNode1(BOOL bEnable)
{
	JetAPI::EnableCtrlWnd(this, PROGRPCFG_GROUP_NODE_ADD_BTN1, bEnable);
	JetAPI::EnableCtrlWnd(this, PROGRPCFG_GROUP_NODE_DELETE_BTN1, bEnable);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectGroupConfigWnd::EnableNode2(BOOL bEnable)
{
	JetAPI::EnableCtrlWnd(this, PROGRPCFG_GROUP_NODE_ADD_BTN2, bEnable);
	JetAPI::EnableCtrlWnd(this, PROGRPCFG_GROUP_NODE_DELETE_BTN2, bEnable);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectGroupConfigWnd::EnableNodeList(BOOL bEnable)
{
	JetAPI::EnableCtrlWnd(this, PROGRPCFG_GROUP_NODE_LIST_ADD_BTN, bEnable);
	JetAPI::EnableCtrlWnd(this, PROGRPCFG_GROUP_NODE_LIST_DELETE_BTN, bEnable);	
	JetAPI::EnableCtrlWnd(this, PROGRPCFG_GROUP_NODE_LIST_CLEAR_BTN, bEnable);		
	JetAPI::EnableCtrlWnd(this, PROGRPCFG_GROUP_NODE_LIST_ARRANGE_BTN, bEnable);	
	JetAPI::EnableCtrlWnd(this, PROGRPCFG_GROUP_NODE_LIST_SORT_X_BTN, bEnable);	
	JetAPI::EnableCtrlWnd(this, PROGRPCFG_GROUP_NODE_LIST_SORT_Y_BTN, bEnable);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectGroupConfigWnd::ExecPartGroupAddBtn()
{
	if ( GetLockUIWnd() == true ) { return true; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }
	
	size_t       i=0;	
	CString      strLabel;
	CString      strValue;
	CString      strCaption;		
	TListNode     ListNode;
	CInputListWnd EnumWnd;	
	std::vector<TListNode> NodelList;
	
	PART_GROUP_MODE GroupMode;
	NodelList.clear();	
	GroupMode = PART_GROUP_COLINEARITY;	
	ListNode.Data = GroupMode;
	ListNode.Text = AOIDataDefine.GetAOIGroupModeText(GroupMode);
	NodelList.push_back(ListNode);		
	
	GroupMode = PART_GROUP_COLINEARITY_TO_LINE;	
	ListNode.Data = GroupMode;
	ListNode.Text = AOIDataDefine.GetAOIGroupModeText(GroupMode);
	NodelList.push_back(ListNode);

	GroupMode = PART_GROUP_DIST_PART_TO_PART;
	ListNode.Data = GroupMode;
	ListNode.Text = AOIDataDefine.GetAOIGroupModeText(GroupMode);
	NodelList.push_back(ListNode);

	GroupMode = PART_GROUP_DIST_PART_NEIGHBOR;
	ListNode.Data = GroupMode;
	ListNode.Text = AOIDataDefine.GetAOIGroupModeText(GroupMode);
	NodelList.push_back(ListNode);	
	
	GroupMode = PART_GROUP_DIST_PART_TO_GROUP;
	ListNode.Data = GroupMode;
	ListNode.Text = AOIDataDefine.GetAOIGroupModeText(GroupMode);
	NodelList.push_back(ListNode);

	GroupMode = PART_GROUP_DIST_GROUP_TO_PART;	
	ListNode.Data = GroupMode;
	ListNode.Text = AOIDataDefine.GetAOIGroupModeText(GroupMode);
	NodelList.push_back(ListNode);

	GroupMode = PART_GROUP_DIST_GROUP_COORD_MAP;	
	ListNode.Data = GroupMode;
	ListNode.Text = AOIDataDefine.GetAOIGroupModeText(GroupMode);
	NodelList.push_back(ListNode);

	strLabel = _T("Group Mode");
	strCaption = _T("Set Group Mode");
	EnumWnd.SetParam1(strCaption, strLabel, -1, NodelList);	
	if ( EnumWnd.DoModal() == IDCANCEL )
	{	return true;	}

	const size_t NodeCount=NodelList.size();
	unsigned int EnumIndex=EnumWnd.GetSelIndex1();
	if ( EnumIndex >= NodeCount )
	{	return true; }
	GroupMode=(PART_GROUP_MODE)(NodelList[EnumIndex].Data);


	CString GroupName;
	CInputBoxWnd InputBox;	
	
	strCaption = _T("Set Group Name");	
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strLabel = AOIDataDefine.GetGroupText();				
	GroupName = AOIDataDefine.GetAOIGroupModeText(GroupMode);
	strValue = GroupName;
	InputBox.SetParam1(strCaption, strLabel, strValue);
	if ( InputBox.DoModal() == IDCANCEL )
	{	return false;	}
	GroupName = InputBox.m_DataEdit1;
	
	CAOIPartGroup  GroupObj;
	const int GroupID = GetPartGroupFreeGroupID();
	std::vector<CAOIPartGroup>  &PartGroupList=GetPartGroupList();
	const unsigned int GroupIndex = (unsigned int)(PartGroupList.size());
	GroupObj.SetPartGroupMode(GroupMode);
	GroupObj.SetPartGroupName(GroupName);
	GroupObj.SetPartGroupGroupID(GroupID);
	GroupObj.SetPartGroupIndex(GroupIndex);	
	GroupObj.SetPartGroupProjectPtr(ProjectPtr);	
	PartGroupList.push_back(GroupObj);
	
	BuildGroupListWnd();		
	UpdateGroupParamUI(&GroupObj);

	CThisListCtrl_53 &GroupListWnd=GetGroupListWnd();
	const int ItemCount=GroupListWnd.GetItemCount();
	if ( ItemCount > 0 )
	{
		int nItem=ItemCount-1;
		GroupListWnd.SetFocus();
		GroupListWnd.EnsureVisible(nItem, FALSE);
		GroupListWnd.SetItemState(nItem, LVIS_SELECTED|LVNI_FOCUSED, LVIS_SELECTED|LVNI_FOCUSED);
	}			
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectGroupConfigWnd::ExecPartGroupDelBtn()
{	
	size_t    i=0;	
	int       idx=0;
	const int nItem = GetGroupListWndItem();
	if ( -1 == nItem ) { return true; }
	const int unsigned Index=m_GroupListWnd.GetItemData(nItem);
	std::vector<CAOIPartGroup> &PartGroupList=GetPartGroupList();
	std::vector<CAOIPartGroup> TmpGroupList=PartGroupList;
	const size_t GroupCount = PartGroupList.size();
	if ( Index >= GroupCount ) 
	{	return true; }

	PartGroupList.clear();
	for ( i=0; i<GroupCount; i++ )
	{
		if ( Index == i ) { continue; }
		TmpGroupList[i].SetPartGroupIndex(idx);
		PartGroupList.push_back(TmpGroupList[i]);
		idx ++;
	}
	BuildGroupListWnd();
	UpdateGroupParamUI(NULL);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectGroupConfigWnd::ExecPartGroupCopyBtn()
{
	if ( GetLockUIWnd() == true ) { return true; }		
	CAOIProject *ProjectPtr=GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }
	CAOIPartGroup  *PartGroupPtr = GetActivePartGroup();
	if ( NULL == PartGroupPtr ) { return true; }
	if ( PartGroupPtr->CheckPartGroupNodeInOneBoard() == false )
	{	return true; }
	CAOIBoard   *RefBoardPtr = PartGroupPtr->GetPartGroupNodeBoardPtr();
	if ( NULL == RefBoardPtr )
	{	return true; }

	size_t          i=0, j=0;
	const CAOIPartGroup RefPartGroup = *PartGroupPtr;
	std::vector<CAOIPartGroup>  &PartGroupList=GetPartGroupList();
	const size_t PartGroupCount=PartGroupList.size();	
	
	CAOIBoard   *BoardPtr = NULL;	
	unsigned int PartGroupIndex=(unsigned int)(PartGroupCount);	
	const size_t BoardCount = ProjectPtr->GetProjectBoardCount();
	for ( j=0; j<BoardCount; j++ )
	{
		BoardPtr = ProjectPtr->GetProjectBoardPtr(j, false);
		if ( NULL == BoardPtr ) { continue; }
		if ( BoardPtr == RefBoardPtr ) { continue; }
		
		CAOIPartGroup PartGroup = RefPartGroup;
		if ( PartGroup.ChangePartGroupNodeBoardPtr(BoardPtr) == false )
		{	continue; }

		PartGroup.SetPartGroupIndex(PartGroupIndex);
		PartGroupList.push_back(PartGroup);
		PartGroupIndex ++;
	}			

	BuildGroupListWnd();
	UpdateGroupParamUI(NULL);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectGroupConfigWnd::ExecPartGroupClearBtn()
{	
	ClearPartGroupList();
	BuildGroupListWnd();
	UpdateGroupParamUI(NULL);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectGroupConfigWnd::ExecPartGroupTestBtn()
{
	if ( GetLockUIWnd() == true ) { return true; }		
	CAOIProject *ProjectPtr=GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }

	UpdateToProjectPartGroupList();	
	ProjectPtr->InitialProjectGroupInspection();
	ProjectPtr->AnalysisProjectGroupInspection();
	ReadPartGroupListResult();
	RestoreProjectPartGroupList();

	CAOIPartGroup *PartGroupPtr = GetActivePartGroup();
	UpdateGroupParamUI(PartGroupPtr);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectGroupConfigWnd::ExecPartGroupUpdateBtn()
{
	if ( GetLockUIWnd() == true ) { return true; }		
	CAOIProject *ProjectPtr=GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }
	CAOIPartGroup  *PartGroupPtr = GetActivePartGroup();
	if ( NULL == PartGroupPtr ) { return true; }
	
	size_t          i=0, j=0;
	const CAOIPartGroup PartGroupObj = *PartGroupPtr;
	std::vector<CAOIPartGroup>  &PartGroupList=GetPartGroupList();
	const size_t PartGroupCount=PartGroupList.size();	
	for ( i=0; i<PartGroupCount; i++ )
	{
		CAOIPartGroup &PartGroupRef=PartGroupList[i];
		if ( PartGroupRef.GetPartGroupGroupID() != PartGroupObj.GetPartGroupGroupID() )
		{	continue; }
		PartGroupRef.CopyPartGroupParam(PartGroupObj);
	}
	UpdateGroupListWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectGroupConfigWnd::ExecPartGroupDelOthersBtn()
{
	if ( GetLockUIWnd() == true ) { return true; }		
	CAOIProject *ProjectPtr=GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }
	CAOIPartGroup  *PartGroupPtr = GetActivePartGroup();
	if ( NULL == PartGroupPtr ) { return true; }

	size_t          i=0, j=0;
	const CAOIPartGroup PartGroupObj = *PartGroupPtr;
	std::vector<CAOIPartGroup>  NewPartGroupList;
	std::vector<CAOIPartGroup>  &PartGroupList=GetPartGroupList();
	const size_t PartGroupCount=PartGroupList.size();	
	for ( i=0; i<PartGroupCount; i++ )
	{
		const CAOIPartGroup *pPartGroup=&(PartGroupList[i]);
		if ( pPartGroup == PartGroupPtr )
		{
			NewPartGroupList.push_back(PartGroupList[i]);
			continue;
		}
		if ( pPartGroup->GetPartGroupGroupID() != PartGroupPtr->GetPartGroupGroupID() )
		{	
			NewPartGroupList.push_back(PartGroupList[i]);
			continue; 
		}
	}
	if ( NewPartGroupList.size() == PartGroupCount )
	{	return true; }

	for ( i=0; i<NewPartGroupList.size(); i++ )
	{
		CAOIPartGroup &PartGroupRef=NewPartGroupList[i];
		PartGroupRef.SetPartGroupIndex(i);
	}
	m_PartGroupList=NewPartGroupList;
	
	BuildGroupListWnd();
	UpdateGroupParamUI(NULL);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectGroupConfigWnd::UpdateGroupParamUI(CAOIPartGroup *Ptr)
{		
	m_NodeFromeID = NODE_FROM_OFF ;
	SetPartGropuNodePtr(NULL);
	ClearNodeParamListWnd();
	ClearGroupParamListWnd();
	CListBox &WndListBox=GetWndListBox();
	CListBox &NodeListBox=GetNodeListBox();
	m_GroupModeCombox.SetCurSel(0);
	ClearListBox(WndListBox);
	ClearListBox(NodeListBox);
	CWnd::SetDlgItemText(PROGRPCFG_GROUP_NODE_EDIT1, _T(""));
	CWnd::SetDlgItemText(PROGRPCFG_GROUP_NODE_EDIT2, _T(""));

	if ( NULL == Ptr ) { return true; }

	CString    str;
	CString    NodeName;	
	TPartGroupNode GroupNode;
	BOOL       bEnable1=FALSE;
	BOOL       bEnable2=FALSE;
	BOOL       bEnableList=FALSE;
	PART_GROUP_MODE GroupMode = Ptr->GetPartGroupMode();
	switch ( GroupMode )
	{
	case PART_GROUP_COLINEARITY:
		bEnableList = TRUE;
		break;
	case PART_GROUP_COLINEARITY_TO_LINE:
		bEnable1 = TRUE;
		bEnable2 = TRUE;
		bEnableList = TRUE;
		break;
	case PART_GROUP_DIST_PART_TO_PART:
		bEnable1 = TRUE;
		bEnable2 = TRUE;
		break;
	case PART_GROUP_DIST_PART_NEIGHBOR:
		bEnableList = TRUE;
		break;
	case PART_GROUP_DIST_PART_TO_GROUP:
		bEnable1 = TRUE;
		bEnableList = TRUE;
		break;
	case PART_GROUP_DIST_GROUP_TO_PART:
		bEnable1 = TRUE;
		bEnableList = TRUE;
		break;
	case PART_GROUP_DIST_GROUP_COORD_MAP:
		bEnable1 = TRUE;
		bEnable2 = TRUE;		
		bEnableList = TRUE;
		break;
	}
	EnableNode1(bEnable1);
	EnableNode2(bEnable2);
	EnableNodeList(bEnableList);
	BuildGroupParamListWnd();

	JetAPI::SetComboxCurSel(m_GroupModeCombox, GroupMode);
	
	Ptr->GetPartGroupNode1(GroupNode);
	if ( GetPartGroupNodeName(GroupNode, false, NodeName) == true )
	{	CWnd::SetDlgItemText(PROGRPCFG_GROUP_NODE_EDIT1, NodeName);	}

	Ptr->GetPartGroupNode2(GroupNode);
	if ( GetPartGroupNodeName(GroupNode, false, NodeName) == true )
	{	CWnd::SetDlgItemText(PROGRPCFG_GROUP_NODE_EDIT2, NodeName);	}	

	size_t       i=0;
	int          nIndex=0;
	TPartGroupNode  *GroupNodePtr=NULL;	
	const size_t NodeCount = Ptr->GetPartGroupNodeCount();
	for ( i=0; i<NodeCount; i++ )
	{
		GroupNodePtr = Ptr->GetPartGroupNodePtr(i, false);
		if ( NULL == GroupNodePtr ) { continue; }
		if ( GetPartGroupNodeName(*GroupNodePtr, true, NodeName) == false )	{	continue;	}
		NodeListBox.AddString(NodeName);
		NodeListBox.SetItemData(nIndex, (i));
		nIndex ++;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectGroupConfigWnd::GetPartGroupNodeName(const TPartGroupNode &GroupNode, bool bAddRes, CString &NodeName)
{
	if ( NULL == GroupNode.ComponentPtr ) { return false; }	
	CAOIComponent *ComponentPtr = GroupNode.ComponentPtr;
	CAOIPanel *PanelPtr = ComponentPtr->GetComponentPanelPtr();
	CAOIBoard *BoardPtr = ComponentPtr->GetComponentBoardPtr();
	const size_t PanelIndex=PanelPtr->GetPanelIndex_Project();
	const size_t BoardIndex=BoardPtr->GetBoardIndex_Panel();
	CString      ComponentName=ComponentPtr->GetComponentName();
	unsigned int WndIndex=GroupNode.ModelWndIndex;
	
	NodeName.Format(_T("P%05d_B%05d_%s#%d"), PanelIndex+1, BoardIndex+1, ComponentName, WndIndex+1);
	if ( true == bAddRes )
	{
		const int ResultLen=GroupNode.ResultText.GetLength();
		if ( ResultLen > 0 ) 
		{
			CString str = NodeName;
			NodeName.Format(_T("%s [%s]"), str, GroupNode.ResultText);
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
TPartGroupNode*  CProjectGroupConfigWnd::GetSelPartGroupNodePtr()
{	
	CAOIPartGroup   *PartGroupPtr = GetActivePartGroup();
	if ( NULL == PartGroupPtr ) { return NULL; }

	TPartGroupNode *NodePtr=NULL;
	switch ( m_NodeFromeID )
	{
	case NODE_FROM_1:	NodePtr = PartGroupPtr->GetPartGroupNodePtr1();	break;
	case NODE_FROM_2:	NodePtr = PartGroupPtr->GetPartGroupNodePtr2();	break;
	case NODE_FROM_LIST:
	{		
		CListBox &ListBox = GetNodeListBox();			
		const int nSel = ListBox.GetCurSel();
		const int ItemCount = ListBox.GetCount();
		if ( nSel<0 || nSel>=ItemCount ) { return NULL; }
		const int nIndex = ListBox.GetItemData(nSel);		
		NodePtr = PartGroupPtr->GetPartGroupNodePtr(nIndex, true);		
	}
		break;	
	}
	return NodePtr;
}
//-------------------------------------------------------------------------------------//
bool CProjectGroupConfigWnd::ExecPartGroupNodeAddBtn(TPartGroupNode &GroupNode)
{
	GroupNode = TPartGroupNode();	
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }
	CAOIComponent *ComponentPtr = ProjectPtr->GetProjectActiveComponent();
	if ( NULL == ComponentPtr ) { return false; }
	
	CAOIPanel *PanelPtr = ComponentPtr->GetComponentPanelPtr();
	CAOIBoard *BoardPtr = ComponentPtr->GetComponentBoardPtr();
	CAOIWnd   *WndPtr = ComponentPtr->GetComponentModelWndPtrByDefectID(WND_DEFECT_PART_ALIGN);

	GroupNode.PanelPtr = PanelPtr;
	GroupNode.BoardPtr = BoardPtr;
	GroupNode.ComponentPtr = ComponentPtr;

	GroupNode.PanelIndex = PanelPtr->GetPanelIndex_Project();
	GroupNode.BoardIndex = BoardPtr->GetBoardIndex_Project();
	GroupNode.ComponentIndex = ComponentPtr->GetComponentIndex_Project();
	GroupNode.ModelWndIndex = 0;	
	if ( NULL != WndPtr )
	{	GroupNode.ModelWndIndex = WndPtr->GetWndIndex(); }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectGroupConfigWnd::ExecSelPartGroupNode(CAOIPartGroup *GroupPtr, TPartGroupNode &GroupNode, int FromID)
{	
	BuildWndListBox(GroupNode, FromID);
	BuildNodeParamListWnd(GroupPtr, GroupNode, FromID);
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }
	CAOIComponent *ComponentPtr = GroupNode.ComponentPtr;
	if ( NULL == ComponentPtr ) { return true; }	
	ProjectPtr->SelectProjectAllComponents(false);
	ComponentPtr->SetComponentSelected(true);
	ProjectPtr->SetProjectActiveComponent(ComponentPtr);	
	//AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_PART_LIST_WND, WPARAM_UPDATE_PART_SELECTED, NULL);
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_PART_LIST_WND, WPARAM_UPDATE_PART_LIST, TREE_CTRL_UPDATE_NULL);	
	AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_PART_SELECTED, NULL);
	return true;
}
//-------------------------------------------------------------------------------------//
bool  CProjectGroupConfigWnd::ExecPartGroupNodeAddListBtn(std::vector<TPartGroupNode> &GroupNodeList)
{	
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }

	size_t     i=0;	
	std::vector<CAOIComponent*> SelComponentList;
	ProjectPtr->GetProjectComponentSelected(SelComponentList);
	const size_t SelCount = SelComponentList.size();

	for ( i=0; i<SelCount; i++ )
	{
		CAOIComponent *ComponentPtr = SelComponentList[i];
		if ( NULL == ComponentPtr ) { continue; }
		CAOIPanel *PanelPtr = ComponentPtr->GetComponentPanelPtr();
		CAOIBoard *BoardPtr = ComponentPtr->GetComponentBoardPtr();
		CAOIWnd   *WndPtr = ComponentPtr->GetComponentModelWndPtrByDefectID(WND_DEFECT_PART_ALIGN);

		TPartGroupNode GroupNode;
		GroupNode.PanelPtr = PanelPtr;
		GroupNode.BoardPtr = BoardPtr;
		GroupNode.ComponentPtr = ComponentPtr;

		GroupNode.PanelIndex = PanelPtr->GetPanelIndex_Project();
		GroupNode.BoardIndex = BoardPtr->GetBoardIndex_Project();
		GroupNode.ComponentIndex = ComponentPtr->GetComponentIndex_Project();
		GroupNode.ModelWndIndex = 0;
		if ( NULL != WndPtr )
		{	GroupNode.ModelWndIndex = WndPtr->GetWndIndex(); }
		GroupNodeList.push_back(GroupNode);
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectGroupConfigWnd::ExecSelChangeListBox()
{
	return true;
}
//-------------------------------------------------------------------------------------//
void CProjectGroupConfigWnd::OnItemchangedGroupListWnd(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here	
	if ( true == m_StopGroupListBeSelected ) { return; }	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) 
	{	return;	}
	DWORD Res = pNMListView->uNewState&LVIS_SELECTED;
	if ( Res == 0 ) { return; }	
	Res = pNMListView->uNewState&LVIS_FOCUSED;
	if ( Res == 0 ) { return; }	
	std::vector<CAOIPartGroup> &PartGroupList=GetPartGroupList();
	CThisListCtrl_53 &GroupListWnd=GetGroupListWnd();
	const size_t GroupCount = PartGroupList.size();
	const size_t SelIndex = GroupListWnd.GetItemData(nItem);	
	if ( SelIndex >= GroupCount ) { return; }	
	UpdateGroupParamUI(&PartGroupList[SelIndex]);
}
//-------------------------------------------------------------------------------------//
void CProjectGroupConfigWnd::OnDblclkGroupListWnd(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here
	NMITEMACTIVATE *pNMListView = (NMITEMACTIVATE*)pNMHDR;	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) { return; }
}
//-------------------------------------------------------------------------------------//
void CProjectGroupConfigWnd::OnItemchangedNodeParamListWnd(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here	
	if ( true == m_StopNodeParamListBeSelected ) { return; }	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) 
	{	return;		}

	DWORD Res = 0;
	Res = pNMListView->uOldState&LVIS_FOCUSED;
	if ( Res != 0 )
	{
		ExecReleaseParamCtrl();
		return;
	}
	Res = pNMListView->uNewState&LVIS_SELECTED;
	if ( Res == 0 ) { return; }	
	Res = pNMListView->uNewState&LVIS_FOCUSED;
	if ( Res == 0 ) { return; }		
	ExecItemchangedParamListWnd(m_NodeParamList, m_NodeParamListWnd, nItem);	
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CProjectGroupConfigWnd::OnDblclkNodeParamListWnd(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMITEMACTIVATE *pNMListView = (NMITEMACTIVATE*)pNMHDR;	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) { return; }
	ExecDblclkParamListWnd(m_NodeParamList, m_NodeParamListWnd, nItem, nSubItem);
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CProjectGroupConfigWnd::OnItemchangedGroupParamListWnd(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here	
	if ( true == m_StopGroupParamListBeSelected ) { return; }	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) 
	{
		SetDescriptionText(NULL);
		return; 
	}
	DWORD Res = 0;
	Res = pNMListView->uOldState&LVIS_FOCUSED;
	if ( Res != 0 )
	{
		ExecReleaseParamCtrl();
		return;
	}
	Res = pNMListView->uNewState&LVIS_SELECTED;
	if ( Res == 0 ) { return; }	
	Res = pNMListView->uNewState&LVIS_FOCUSED;
	if ( Res == 0 ) { return; }		
	ExecItemchangedParamListWnd(m_GroupParamList, m_GroupParamListWnd, nItem);	
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CProjectGroupConfigWnd::OnDblclkGroupParamListWnd(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMITEMACTIVATE *pNMListView = (NMITEMACTIVATE*)pNMHDR;	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) { return; }
	ExecDblclkParamListWnd(m_GroupParamList, m_GroupParamListWnd, nItem, nSubItem);
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CProjectGroupConfigWnd::OnKillfocusGroupParamEdit()
{
	CEdit &EditCtrl = m_GroupParamEdit;
	if ( EditCtrl.IsWindowVisible() == FALSE ) { return; }
	ExecUpdateParamByEdit();
	EditCtrl.ShowWindow(SW_HIDE);	
	EditCtrl.SetWindowText(_T(""));
}
//-------------------------------------------------------------------------------------//
void CProjectGroupConfigWnd::OnSelchangeGroupParamCombo() 
{
	// TODO: Add your control notification handler code here
	if ( ExecUpdateParamByCombox() == false )
	{	return; }
	CComboBox &ComboxCtrl=m_GroupParamCombox;
	ComboxCtrl.ShowWindow(SW_HIDE);	
	JetAPI::ClearCombox(ComboxCtrl);	
}
//-------------------------------------------------------------------------------------//
void CProjectGroupConfigWnd::OnKillfocusGroupParamCombo()
{
	if ( ExecUpdateParamByCombox() == false )
	{	return; }
	CComboBox &ComboxCtrl=m_GroupParamCombox;
	ComboxCtrl.ShowWindow(SW_HIDE);	
	JetAPI::ClearCombox(ComboxCtrl);	
}
//-------------------------------------------------------------------------------------//	
void CProjectGroupConfigWnd::OnGroupAddBtn() 
{
	// TODO: Add your control notification handler code here	
	ExecPartGroupAddBtn();
}
//-------------------------------------------------------------------------------------//
void CProjectGroupConfigWnd::OnGroupCopyBtn() 
{
	// TODO: Add your control notification handler code here
	ExecPartGroupCopyBtn();	
}
//-------------------------------------------------------------------------------------//
void CProjectGroupConfigWnd::OnGroupDeleteBtn() 
{
	// TODO: Add your control notification handler code here
	ExecPartGroupDelBtn();
}
//-------------------------------------------------------------------------------------//
void CProjectGroupConfigWnd::OnGroupClearBtn() 
{
	// TODO: Add your control notification handler code here
	ExecPartGroupClearBtn();
}
//-------------------------------------------------------------------------------------//
void CProjectGroupConfigWnd::OnGroupTestBtn() 
{
	// TODO: Add your control notification handler code here
	ExecPartGroupTestBtn();
}
//-------------------------------------------------------------------------------------//
void CProjectGroupConfigWnd::OnGroupUpdateBtn()
{
	// TODO: Add your control notification handler code here
	ExecPartGroupUpdateBtn();	
}
//-------------------------------------------------------------------------------------//
void CProjectGroupConfigWnd::OnGroupDelOthersBtn()
{
	// TODO: Add your control notification handler code here	
	ExecPartGroupDelOthersBtn();
}
//-------------------------------------------------------------------------------------//
void CProjectGroupConfigWnd::OnGroupNodeAddBtn1() 
{
	// TODO: Add your control notification handler code here		
	CAOIPartGroup   *PartGroupPtr = GetActivePartGroup();
	if ( NULL == PartGroupPtr ) { return; }

	TPartGroupNode GroupNode;
	if ( ExecPartGroupNodeAddBtn(GroupNode) == false ) 
	{	return; }	
	PartGroupPtr->SetPartGroupNode1(GroupNode);
	UpdateGroupParamUI(PartGroupPtr);
	ExecSelPartGroupNode(PartGroupPtr, GroupNode, NODE_FROM_1);	
	return;
}
//-------------------------------------------------------------------------------------//
void CProjectGroupConfigWnd::OnGroupNodeDeleteBtn1() 
{
	// TODO: Add your control notification handler code here	
	CAOIPartGroup   *PartGroupPtr = GetActivePartGroup();
	if ( NULL == PartGroupPtr ) { return; }
	PartGroupPtr->ClearPartGroupNode1();
	UpdateGroupParamUI(PartGroupPtr);
	return;
}
//-------------------------------------------------------------------------------------//
void CProjectGroupConfigWnd::OnSetfocusNodeAddEdit1()
{
	CAOIPartGroup   *PartGroupPtr = GetActivePartGroup();
	if ( NULL == PartGroupPtr ) { return; }
	if ( CheckPartGropuNodePtr(PartGroupPtr->GetPartGroupNodePtr1()) == true ) { return ; }

	TPartGroupNode GroupNode;	
	PartGroupPtr->GetPartGroupNode1(GroupNode);	
	ExecSelPartGroupNode(PartGroupPtr, GroupNode, NODE_FROM_1);
	SetPartGropuNodePtr(PartGroupPtr->GetPartGroupNodePtr1());
	return;
}
//-------------------------------------------------------------------------------------//
void CProjectGroupConfigWnd::OnGroupNodeAddBtn2() 
{
	// TODO: Add your control notification handler code here	
	CAOIPartGroup   *PartGroupPtr = GetActivePartGroup();
	if ( NULL == PartGroupPtr ) { return; }

	TPartGroupNode GroupNode;
	if ( ExecPartGroupNodeAddBtn(GroupNode) == false ) 
	{	return; }	
	PartGroupPtr->SetPartGroupNode2(GroupNode);
	UpdateGroupParamUI(PartGroupPtr);
	ExecSelPartGroupNode(PartGroupPtr, GroupNode, NODE_FROM_2);
	return;
}
//-------------------------------------------------------------------------------------//
void CProjectGroupConfigWnd::OnGroupNodeDeleteBtn2() 
{
	// TODO: Add your control notification handler code here	
	CAOIPartGroup   *PartGroupPtr = GetActivePartGroup();
	if ( NULL == PartGroupPtr ) { return; }
	PartGroupPtr->ClearPartGroupNode2();
	UpdateGroupParamUI(PartGroupPtr);
	return;
}
//-------------------------------------------------------------------------------------//
void CProjectGroupConfigWnd::OnSetfocusNodeAddEdit2()
{
	CAOIPartGroup   *PartGroupPtr = GetActivePartGroup();
	if ( NULL == PartGroupPtr ) { return; }
	if ( CheckPartGropuNodePtr(PartGroupPtr->GetPartGroupNodePtr2()) == true ) { return ; }

	TPartGroupNode GroupNode;	
	PartGroupPtr->GetPartGroupNode2(GroupNode);	
	ExecSelPartGroupNode(PartGroupPtr, GroupNode, NODE_FROM_2);
	SetPartGropuNodePtr(PartGroupPtr->GetPartGroupNodePtr2());
	return;
}
//-------------------------------------------------------------------------------------//
void CProjectGroupConfigWnd::OnGroupNodeListAddBtn() 
{
	// TODO: Add your control notification handler code here	
	CAOIPartGroup   *PartGroupPtr = GetActivePartGroup();
	if ( NULL == PartGroupPtr ) { return; }

	std::vector<TPartGroupNode> GroupNodeList;	
	if ( ExecPartGroupNodeAddListBtn(GroupNodeList) == false ) 
	{	return; }	
	PartGroupPtr->AddPartGroupNodeList(GroupNodeList);
	UpdateGroupParamUI(PartGroupPtr);
	return;
}
//-------------------------------------------------------------------------------------//
void CProjectGroupConfigWnd::OnGroupNodeListDeleteBtn() 
{
	// TODO: Add your control notification handler code here	
	CAOIPartGroup   *PartGroupPtr = GetActivePartGroup();
	if ( NULL == PartGroupPtr ) { return; }	
	PartGroupPtr->RemovePartGroupNodeComponentSelected();
	UpdateGroupParamUI(PartGroupPtr);
	return;
}
//-------------------------------------------------------------------------------------//
void CProjectGroupConfigWnd::OnGroupNodeListClearBtn() 
{
	// TODO: Add your control notification handler code here	
	CAOIPartGroup   *PartGroupPtr = GetActivePartGroup();
	if ( NULL == PartGroupPtr ) { return; }
	PartGroupPtr->ClearPartGroupNodeList();
	UpdateGroupParamUI(PartGroupPtr);
	return;
}
//-------------------------------------------------------------------------------------//
void CProjectGroupConfigWnd::OnSelchangeNodeListBox()
{
	CString str;
	CString strSel;
	CListBox &ListBox = GetNodeListBox();	
	const int nSel = ListBox.GetCurSel();
	const int ItemCount = ListBox.GetCount();
	if ( nSel<0 || nSel>=ItemCount ) { return; }
	const int nIndex = ListBox.GetItemData(nSel);

	CAOIPartGroup *PartGroupPtr = GetActivePartGroup();
	if ( NULL == PartGroupPtr ) { return; }	
	CAOIProject *ProjectPtr = PartGroupPtr->GetPartGroupProjectPtr();
	if ( NULL == ProjectPtr ) { return; }	
	TPartGroupNode *NodePtr = PartGroupPtr->GetPartGroupNodePtr(nIndex, true);
	if ( NULL == NodePtr ) { return; }
	ExecSelPartGroupNode(PartGroupPtr, *NodePtr, NODE_FROM_LIST);
	SetPartGropuNodePtr(NodePtr);
	return;
}
//-------------------------------------------------------------------------------------//
void CProjectGroupConfigWnd::OnGroupNodeListArrangeBtn()
{
	CAOIPartGroup   *PartGroupPtr = GetActivePartGroup();
	if ( NULL == PartGroupPtr ) { return; }
	PartGroupPtr->ArrangePartGroupNodeList();
	UpdateGroupParamUI(PartGroupPtr);	
}
//-------------------------------------------------------------------------------------//
void CProjectGroupConfigWnd::OnGroupNodeListSortXBtn() 
{
	// TODO: Add your control notification handler code here
	CAOIPartGroup   *PartGroupPtr = GetActivePartGroup();
	if ( NULL == PartGroupPtr ) { return; }
	PartGroupPtr->SortPartGroupNodeListByPosX();
	UpdateGroupParamUI(PartGroupPtr);
}
//-------------------------------------------------------------------------------------//
void CProjectGroupConfigWnd::OnGroupNodeListSortYBtn() 
{
	// TODO: Add your control notification handler code here
	CAOIPartGroup   *PartGroupPtr = GetActivePartGroup();
	if ( NULL == PartGroupPtr ) { return; }
	PartGroupPtr->SortPartGroupNodeListByPosY();
	UpdateGroupParamUI(PartGroupPtr);
}
//-------------------------------------------------------------------------------------//
void CProjectGroupConfigWnd::OnGroupWndSetBtn() 
{
	// TODO: Add your control notification handler code here
	CString str;
	CString strSel;	
	CListBox &WndListBox = GetWndListBox();
	const int nWndSel = WndListBox.GetCurSel();
	const int WndItemCount=WndListBox.GetCount();
	if ( nWndSel<0 || nWndSel>=WndItemCount ) { return; }
	const int nWndIndex = WndListBox.GetItemData(nWndSel);

	CAOIPartGroup *PartGroupPtr = GetActivePartGroup();
	if ( NULL == PartGroupPtr ) { return; }		
	TPartGroupNode *NodePtr = GetSelPartGroupNodePtr();
	if ( NULL == NodePtr ) { return; }	
	CAOIProject *ProjectPtr = PartGroupPtr->GetPartGroupProjectPtr();
	if ( NULL == ProjectPtr ) { return; }		
	CAOIComponent *ComponentPtr = NodePtr->ComponentPtr;
	if ( NULL == ComponentPtr ) { return; }
	CAOIModel *ModelPtr = ComponentPtr->GetComponentModelPtr();
	if ( NULL == ModelPtr ) { return; }
	const int WndCount = (int)(ModelPtr->GetModelWndCount());
	if ( nWndIndex<0 || nWndIndex>=WndCount ) { return; }
	NodePtr->ModelWndIndex = nWndIndex;
	BuildWndListBox(*NodePtr, m_NodeFromeID);
	return;
}
//-------------------------------------------------------------------------------------//
void CProjectGroupConfigWnd::OnGroupWndSetAllBtn() 
{
	// TODO: Add your control notification handler code here
	CString str;
	CString strSel;	
	CListBox &WndListBox = GetWndListBox();	
	const int nWndSel = WndListBox.GetCurSel();
	const int WndItemCount=WndListBox.GetCount();
	if ( nWndSel<0 || nWndSel>=WndItemCount ) { return; }
	const int nWndIndex = WndListBox.GetItemData(nWndSel);

	CAOIPartGroup *PartGroupPtr = GetActivePartGroup();
	if ( NULL == PartGroupPtr ) { return; }	
	PartGroupPtr->SetPartGroupNodeListWndIndex(nWndIndex);
	UpdateGroupParamUI(PartGroupPtr);
	return;
}
//-------------------------------------------------------------------------------------//
