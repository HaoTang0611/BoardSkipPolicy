// ProjectCompareWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "ProjectCompareWnd.h"
//-------------------------------------------------------------------------------------//
#include "LoadCadxyWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
#define CMP_RES_NONE              0x0000//相同
#define CMP_RES_ADD               0x0001//要新增
#define CMP_RES_DEL               0x0002//要刪除
#define CMP_RES_BYPASS            0x0004//不檢測

#define CMP_RES_POS               0x0010//位置
#define CMP_RES_ANGLE             0x0040//角度
#define CMP_RES_PART_NUMBER       0x0080//料號
//-------------------------------------------------------------------------------------//
#define CMP_COL_INDEX             0
#define CMP_COL_COMPONENT         1
#define CMP_COL_RESULT            2
#define CMP_COL_RESULT_II         3
//-------------------------------------------------------------------------------------//
#define CMP_PART_VERSION_V2       1
//-------------------------------------------------------------------------------------//
int CALLBACK CompareListCompareFn(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
int CALLBACK CompareListCompareFn(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	if ( NULL == lParamSort ) { return 0; }
	CProjectCompareWnd* pProjectCompareWnd = (CProjectCompareWnd*)lParamSort;
	return pProjectCompareWnd->CompareComponentItem(lParam1, lParam2);
}
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CProjectCompareWnd dialog
//-------------------------------------------------------------------------------------//
CProjectCompareWnd::CProjectCompareWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CProjectCompareWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProjectCompareWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_Merged = true;
	m_RefProjectPtr = NULL;
	m_HostProjectPtr = NULL;
	m_CompareColID = CMP_COL_INDEX;
	m_CompareListSortMode = SORT_ASCEND;
}
//-------------------------------------------------------------------------------------//
void CProjectCompareWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProjectCompareWnd)
	DDX_Control(pDX, PROCMP_REF_PANEL_COMBO, m_RefPanelCombox);
	DDX_Control(pDX, PROCMP_REF_BOARD_COMBO, m_RefBoardCombox);
	DDX_Control(pDX, PROCMP_HOST_PANEL_COMBO, m_HostPanelCombox);
	DDX_Control(pDX, PROCMP_HOST_BOARD_COMBO, m_HostBoardCombox);
	DDX_Control(pDX, PROCMP_REF_COMPONENT_LIST_WND, m_RefComponentListWnd);
	DDX_Control(pDX, PROCMP_HOST_COMPONENT_LIST_WND, m_HostComponentListWnd);
	DDX_Control(pDX, PROCMP_COMPARE_LIST_WND, m_CompareListWnd);	
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CProjectCompareWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CProjectCompareWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_GETMINMAXINFO()
	ON_CBN_SELCHANGE(PROCMP_HOST_PANEL_COMBO, OnSelchangeHostPanelCombo)
	ON_CBN_SELCHANGE(PROCMP_HOST_BOARD_COMBO, OnSelchangeHostBoardCombo)
	ON_CBN_SELCHANGE(PROCMP_REF_PANEL_COMBO, OnSelchangeRefPanelCombo)
	ON_CBN_SELCHANGE(PROCMP_REF_BOARD_COMBO, OnSelchangeRefBoardCombo)
	ON_BN_CLICKED(PROCMP_LOAD_REF_CADXY_BTN, OnLoadRefCadxyBtn)
	ON_BN_CLICKED(PROCMP_COMPARE_BTN, OnCompareBtn)
	ON_NOTIFY(LVN_COLUMNCLICK, PROCMP_COMPARE_LIST_WND, OnColumnclickCompareListWnd)	
	ON_BN_CLICKED(PROCMP_REMOVE_BTN, OnRemoveBtn)
	ON_BN_CLICKED(PROCMP_RESTORE_BTN, OnRestoreBtn)
	ON_BN_CLICKED(PROCMP_MERGE_BTN, OnMergeBtn)
	ON_BN_CLICKED(PROCMP_EXPORT_FILE_BTN, OnExportFileBtn)
	ON_BN_CLICKED(PROCMP_PROJECT_MAP_BTN, OnProjectMapBtn)
	ON_NOTIFY(NM_DBLCLK, PROCMP_COMPARE_LIST_WND, OnDblclkCompareListWnd)
	ON_NOTIFY(NM_DBLCLK, PROCMP_HOST_COMPONENT_LIST_WND, OnDblclkHostComponentListWnd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CProjectCompareWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CProjectCompareWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CWnd::ShowWindow(SW_SHOWMAXIMIZED);
	m_ProjectMapWnd.Create(IDD_PROJECT_MAP_WND, this);

	SwitchMultiLanguage();
	CreateRefProjectObj();

	JetAPI::InitialListCtrl(m_CompareListWnd);
	JetAPI::InitialListCtrl(m_RefComponentListWnd);
	JetAPI::InitialListCtrl(m_HostComponentListWnd);
	BuildCompareListWndHeader();
	BuildRefComponentListWndHeader();
	BuildHostComponentListWndHeader();	

	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	m_ProjectMapWnd.SetProjectPtr(m_HostProjectPtr, false);
	BuildHostPanelCombox();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CProjectCompareWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	DestroyRefProjectObj();
}
//-------------------------------------------------------------------------------------//
void CProjectCompareWnd::OnSize(UINT nType, int cx, int cy) 
{
	CBaseDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if ( CWnd::GetSafeHwnd() == NULL ) { return; }
	
	if ( m_CompareListWnd.GetSafeHwnd() != NULL )
	{
		RECT Rect={0};
		m_CompareListWnd.GetWindowRect(&Rect);
		this->ScreenToClient(&Rect);		
		Rect.right = cx-8;
		Rect.bottom = cy-8;
		m_CompareListWnd.MoveWindow(&Rect);		
	}

	if ( m_RefComponentListWnd.GetSafeHwnd() != NULL )
	{
		RECT Rect={0};
		m_RefComponentListWnd.GetWindowRect(&Rect);
		this->ScreenToClient(&Rect);		
		//Rect.right = cx-4;
		Rect.bottom = cy-8;
		m_RefComponentListWnd.MoveWindow(&Rect);		
	}

	if ( m_HostComponentListWnd.GetSafeHwnd() != NULL )
	{
		RECT Rect={0};
		m_HostComponentListWnd.GetWindowRect(&Rect);
		this->ScreenToClient(&Rect);		
		//Rect.right = cx-4;
		Rect.bottom = cy-8;
		m_HostComponentListWnd.MoveWindow(&Rect);		
	}

}
//-------------------------------------------------------------------------------------//
void CProjectCompareWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CBaseDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CProjectCompareWnd::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default	
	CBaseDialog::OnGetMinMaxInfo(lpMMI);
	lpMMI->ptMinTrackSize.x = 1024;
	lpMMI->ptMinTrackSize.y = 400;
}
//-------------------------------------------------------------------------------------//
void CProjectCompareWnd::OnOK()
{
	// TODO: Add extra validation here
	if ( GetMerged() == false )
	{	OnMergeBtn();	}
	CBaseDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CProjectCompareWnd::OnSelchangeHostPanelCombo() 
{
	// TODO: Add your control notification handler code here
	BuildHostBoardCombox();	
}
//-------------------------------------------------------------------------------------//
void CProjectCompareWnd::OnSelchangeHostBoardCombo() 
{
	// TODO: Add your control notification handler code here
	BuildHostComponentListWnd();
}
//-------------------------------------------------------------------------------------//
void CProjectCompareWnd::OnSelchangeRefPanelCombo() 
{
	// TODO: Add your control notification handler code here
	BuildRefBoardCombox();	
}
//-------------------------------------------------------------------------------------//
void CProjectCompareWnd::OnSelchangeRefBoardCombo() 
{
	// TODO: Add your control notification handler code here
	BuildRefComponentListWnd();
}
//-------------------------------------------------------------------------------------//
void CProjectCompareWnd::OnLoadRefCadxyBtn() 
{
	// TODO: Add your control notification handler code here
	CAOIProject *ProjectPtr = GetRefProjectPtr();
	if ( NULL == ProjectPtr ) { return; }

	CLoadCadxyWnd LoadCadxyWnd;		
	if ( LoadCadxyWnd.SetProjectPtr(ProjectPtr) == false )
	{	return;	}

	ClearCompareListWnd();
	m_ComponentCmpList.clear();	
	
	LoadCadxyWnd.DoModal();
	BuildRefPanelCombox();
}
//-------------------------------------------------------------------------------------//
void CProjectCompareWnd::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_PROJECT_COMPARE_WND");
	//---------------------------------------------------------------------------------//
	WndID = IDD_PROJECT_COMPARE_WND;
	WndKey = _T("IDD_PROJECT_COMPARE_WND");
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
	WndID = PROCMP_HOST_PROJECT_GROUP;
	WndKey = _T("PROCMP_HOST_PROJECT_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROCMP_HOST_PANEL_LABEL;
	WndKey = _T("PROCMP_HOST_PANEL_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROCMP_HOST_BOARD_LABEL;
	WndKey = _T("PROCMP_HOST_BOARD_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROCMP_HOST_COMPONENT_LABEL;
	WndKey = _T("PROCMP_HOST_COMPONENT_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = PROCMP_REF_PROJECT_GROUP;
	WndKey = _T("PROCMP_REF_PROJECT_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROCMP_REF_PANEL_LABEL;
	WndKey = _T("PROCMP_REF_PANEL_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PROCMP_REF_BOARD_LABEL;
	WndKey = _T("PROCMP_REF_BOARD_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROCMP_REF_COMPONENT_LABEL;
	WndKey = _T("PROCMP_REF_COMPONENT_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = PROCMP_LOAD_REF_CADXY_BTN;
	WndKey = _T("PROCMP_LOAD_REF_CADXY_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROCMP_REMOVE_BTN;
	WndKey = _T("PROCMP_REMOVE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = PROCMP_RESTORE_BTN;
	WndKey = _T("PROCMP_RESTORE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROCMP_COMPARE_BTN;
	WndKey = _T("PROCMP_COMPARE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = PROCMP_EXPORT_FILE_BTN;
	WndKey = _T("PROCMP_EXPORT_FILE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = PROCMP_MERGE_BTN;
	WndKey = _T("PROCMP_MERGE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = PROCMP_PROJECT_MAP_BTN;
	WndKey = _T("PROCMP_PROJECT_MAP_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PROCMP_DELETE_CHK;
	WndKey = _T("PROCMP_DELETE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PROCMP_POS_CHK;
	WndKey = _T("PROCMP_POS_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	
	WndID = PROCMP_ANGLE_CHK;
	WndKey = _T("PROCMP_ANGLE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	
	WndID = PROCMP_PART_NUMBER_CHK;
	WndKey = _T("PROCMP_PART_NUMBER_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//
	m_strNone = _T("");

	m_strAdd = _T("Add");
	m_strAdd = LoadMultiLanguageString(m_strAdd, m_strAdd);

	m_strDel = _T("Del");
	m_strDel = LoadMultiLanguageString(m_strDel, m_strDel);

	m_strBypass = _T("Bypass");
	m_strBypass = LoadMultiLanguageString(m_strBypass, m_strBypass);

	m_strPos = _T("Pos");
	m_strPos = LoadMultiLanguageString(m_strPos, m_strPos);	
	m_strAngle = _T("Angle");
	m_strAngle = LoadMultiLanguageString(m_strAngle, m_strAngle);
	m_strPartNumber = _T("PN");
	m_strPartNumber = LoadMultiLanguageString(m_strPartNumber, m_strPartNumber);
	//---------------------------------------------------------------------------------//
}
//-------------------------------------------------------------------------------------//
CString CProjectCompareWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_PROJECT_COMPARE_WND");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
void CProjectCompareWnd::SetHostProjectPtr(CAOIProject *Ptr)
{
	m_HostProjectPtr = Ptr;
}
//-------------------------------------------------------------------------------------//
void CProjectCompareWnd::SetRefProjectPtr(CAOIProject *Ptr)
{
	m_RefProjectPtr = Ptr;
}
//-------------------------------------------------------------------------------------//
inline CAOIProject* CProjectCompareWnd::GetRefProjectPtr()
{
	return m_RefProjectPtr;
}
//-------------------------------------------------------------------------------------//
inline CAOIProject* CProjectCompareWnd::GetHostProjectPtr()
{
	return m_HostProjectPtr;
}
//-------------------------------------------------------------------------------------//
bool CProjectCompareWnd::CreateRefProjectObj()
{
	CAOIProject  *ProjectPtr=NULL;
	ProjectPtr = AOIObjManager.CreateProjectObj();
	if ( NULL == ProjectPtr ) { return false; }
	CAOIPanel *PanelPtr = AOIObjManager.CreatePanelObj();
	if ( NULL == PanelPtr )
	{
		AOIObjManager.DestroyProjectObj(ProjectPtr);
		return false; 
	}
	PanelPtr->SetPanelSelected(true);	
	ProjectPtr->AddProjectPanelPtr(PanelPtr, false);
	ProjectPtr->SetProjectActivePanel(PanelPtr);
	SetRefProjectPtr(ProjectPtr);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectCompareWnd::DestroyRefProjectObj()
{
	if ( NULL == m_RefProjectPtr ) { return true; }
	AOIObjManager.DestroyProjectObj(m_RefProjectPtr);
	m_RefProjectPtr = NULL;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectCompareWnd::BuildHostPanelCombox()
{
	CComboBox &Combox = m_HostPanelCombox;
	JetAPI::ClearCombox(Combox);
	CAOIProject *ProjectPtr = GetHostProjectPtr();
	if ( NULL == ProjectPtr ) { return false; }

	size_t  i=0;
	int     index=0;
	CString str;
	CAOIPanel   *PanelPtr = NULL;
	CAOIPanel   *PanelPtr_First = NULL;
	const size_t PanelCount = ProjectPtr->GetProjectPanelCount();	

	index = 0;
	for ( i=0; i<PanelCount; i++ )
	{
		PanelPtr = ProjectPtr->GetProjectPanelPtr(i, false);
		if ( NULL == PanelPtr ) { continue; }
		if ( PanelPtr->GetPanelDeleted() == true ) { continue; }

		str.Format(_T("%d"), i+1);
		Combox.InsertString(-1, str);
		Combox.SetItemData(index, (DWORD_PTR)PanelPtr);
		if ( NULL == PanelPtr_First )
		{	PanelPtr_First = PanelPtr; }
		index ++;
	}	
	const int ItemCount = Combox.GetCount();
	if ( 0 == ItemCount ) { return true; }
	JetAPI::SetComboxCurSelPtr(Combox, (DWORD_PTR)PanelPtr_First);	
	return BuildHostBoardCombox();;
}
//-------------------------------------------------------------------------------------//
bool CProjectCompareWnd::BuildHostBoardCombox()
{
	CComboBox &Combox = m_HostBoardCombox;
	JetAPI::ClearCombox(Combox);
	CAOIProject *ProjectPtr = GetHostProjectPtr();
	if ( NULL == ProjectPtr ) { return false; }
	const int PanelItemCount = m_HostPanelCombox.GetCount();
	if ( 0 == PanelItemCount ) { return true; }
	CAOIPanel *PanelPtr = (CAOIPanel*)(JetAPI::GetComboxCurSelData(m_HostPanelCombox));
	if ( NULL == PanelPtr ) { return false; }

	size_t  i=0;
	int     index=0;
	CString str;
	CAOIBoard   *BoardPtr = NULL;
	CAOIBoard   *BoardPtr_First = NULL;
	const size_t BoardCount = PanelPtr->GetPanelBoardCount();	

	index = 0;
	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr = PanelPtr->GetPanelBoardPtr(i, false);
		if ( NULL == BoardPtr ) { continue; }
		if ( BoardPtr->GetBoardDeleted() == true ) { continue; }

		str.Format(_T("%d"), i+1);
		Combox.InsertString(-1, str);
		Combox.SetItemData(index, (DWORD_PTR)BoardPtr);
		if ( NULL == BoardPtr_First )
		{	BoardPtr_First = BoardPtr; }
		index ++;
	}	
	const int ItemCount = Combox.GetCount();
	if ( 0 == ItemCount ) { return true; }
	JetAPI::SetComboxCurSelPtr(Combox, (DWORD_PTR)BoardPtr_First);	
	return BuildHostComponentListWnd();
}
//-------------------------------------------------------------------------------------//
bool CProjectCompareWnd::BuildHostComponentListWnd()
{
	CThisListCtrl_18 &ListCtrl = m_HostComponentListWnd;
	JetAPI::ClearListCtrl(ListCtrl, FALSE);
	CAOIProject *ProjectPtr = GetHostProjectPtr();
	if ( NULL == ProjectPtr ) { return false; }
	const int BoardItemCount = m_HostBoardCombox.GetCount();
	if ( 0 == BoardItemCount ) { return true; }
	CAOIBoard *BoardPtr = (CAOIBoard*)(JetAPI::GetComboxCurSelData(m_HostBoardCombox));
	if ( NULL == BoardPtr ) { return false; }

	size_t         i=0;
	CString        str;
	int            nItem=0;
	int            nSubItem=0;	
	CAOIComponent *ComponentPtr = NULL;
	const size_t ComponentCount = BoardPtr->GetBoardComponentCount();

	nItem=0;
	ListCtrl.SetRedraw(FALSE);
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = BoardPtr->GetBoardComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }
		str.Format(_T("%d"), i+1);

		nSubItem = 0;
		ListCtrl.InsertItem(nItem, str);
		ListCtrl.SetItemData(nItem, (DWORD_PTR)ComponentPtr);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		str = ComponentPtr->GetComponentName();
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		nItem ++;
	}
	ListCtrl.SetRedraw(TRUE);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectCompareWnd::BuildHostComponentListWndHeader()
{
	CThisListCtrl_18 &ListCtrl = m_HostComponentListWnd;
	JetAPI::ClearListCtrl(ListCtrl, FALSE);

	CString str;
	int  index = 0;
	int  Width = 48;
	int  Width2= 100;
	int  nAlign = LVCFMT_RIGHT;

	index = 0;
	str = _T("Index");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(index, str, nAlign, Width);	index++;

	str = _T("Component");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(index, str, nAlign, Width2);	index++;	

	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectCompareWnd::BuildRefPanelCombox()
{
	CComboBox &Combox = m_RefPanelCombox;
	JetAPI::ClearCombox(Combox);
	CAOIProject *ProjectPtr = GetRefProjectPtr();
	if ( NULL == ProjectPtr ) { return false; }

	size_t  i=0;
	int     index=0;
	CString str;
	CAOIPanel   *PanelPtr = NULL;
	CAOIPanel   *PanelPtr_First = NULL;
	const size_t PanelCount = ProjectPtr->GetProjectPanelCount();	

	index = 0;
	for ( i=0; i<PanelCount; i++ )
	{
		PanelPtr = ProjectPtr->GetProjectPanelPtr(i, false);
		if ( NULL == PanelPtr ) { continue; }
		if ( PanelPtr->GetPanelDeleted() == true ) { continue; }

		str.Format(_T("%d"), i+1);
		Combox.InsertString(-1, str);
		Combox.SetItemData(index, (DWORD_PTR)PanelPtr);
		if ( NULL == PanelPtr_First )
		{	PanelPtr_First = PanelPtr; }
		index ++;
	}	
	const int ItemCount = Combox.GetCount();
	if ( 0 == ItemCount ) { return true; }
	JetAPI::SetComboxCurSelPtr(Combox, (DWORD_PTR)PanelPtr_First);
	return BuildRefBoardCombox();	
}
//-------------------------------------------------------------------------------------//
bool CProjectCompareWnd::BuildRefBoardCombox()
{
	CComboBox &Combox = m_RefBoardCombox;
	JetAPI::ClearCombox(Combox);
	CAOIProject *ProjectPtr = GetRefProjectPtr();
	if ( NULL == ProjectPtr ) { return false; }	
	const int PanelItemCount = m_RefPanelCombox.GetCount();
	if ( 0 == PanelItemCount ) { return true; }
	CAOIPanel *PanelPtr = (CAOIPanel*)(JetAPI::GetComboxCurSelData(m_RefPanelCombox));
	if ( NULL == PanelPtr ) { return false; }

	size_t  i=0;
	int     index=0;
	CString str;
	CAOIBoard   *BoardPtr = NULL;
	CAOIBoard   *BoardPtr_First = NULL;
	const size_t BoardCount = PanelPtr->GetPanelBoardCount();	

	index = 0;
	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr = PanelPtr->GetPanelBoardPtr(i, false);
		if ( NULL == BoardPtr ) { continue; }
		if ( BoardPtr->GetBoardDeleted() == true ) { continue; }

		str.Format(_T("%d"), i+1);
		Combox.InsertString(-1, str);
		Combox.SetItemData(index, (DWORD_PTR)BoardPtr);
		if ( NULL == BoardPtr_First )
		{	BoardPtr_First = BoardPtr; }
		index ++;
	}	
	const int ItemCount = Combox.GetCount();
	if ( 0 == ItemCount ) { return true; }
	JetAPI::SetComboxCurSelPtr(Combox, (DWORD_PTR)BoardPtr_First);	
	return BuildRefComponentListWnd();
}
//-------------------------------------------------------------------------------------//
bool CProjectCompareWnd::BuildRefComponentListWnd()
{
	CThisListCtrl_18 &ListCtrl = m_RefComponentListWnd;
	JetAPI::ClearListCtrl(ListCtrl, FALSE);
	CAOIProject *ProjectPtr = GetRefProjectPtr();
	if ( NULL == ProjectPtr ) { return false; }	
	const int BoardItemCount = m_RefBoardCombox.GetCount();
	if ( 0 == BoardItemCount ) { return true; }
	CAOIBoard *BoardPtr = (CAOIBoard*)(JetAPI::GetComboxCurSelData(m_RefBoardCombox));
	if ( NULL == BoardPtr ) { return false; }
	
	size_t         i=0;
	CString        str;
	int            nItem=0;
	int            nSubItem=0;	
	CAOIComponent *ComponentPtr = NULL;
	const size_t ComponentCount = BoardPtr->GetBoardComponentCount();

	nItem=0;
	ListCtrl.SetRedraw(FALSE);
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = BoardPtr->GetBoardComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }
		str.Format(_T("%d"), i+1);

		nSubItem = 0;
		ListCtrl.InsertItem(nItem, str);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		str = ComponentPtr->GetComponentName();
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		nItem ++;
	}
	ListCtrl.SetRedraw(TRUE);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectCompareWnd::BuildRefComponentListWndHeader()
{
	CThisListCtrl_18 &ListCtrl = m_RefComponentListWnd;
	JetAPI::ClearListCtrl(ListCtrl, FALSE);

	CString str;
	int  index = 0;
	int  Width = 48;
	int  Width2= 100;
	int  nAlign = LVCFMT_RIGHT;

	index = 0;
	str = _T("Index");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(index, str, nAlign, Width);	index++;

	str = _T("Component");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(index, str, nAlign, Width2);	index++;	

	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectCompareWnd::BuildCompareList()
{
	ClearCompareListWnd();
	m_ComponentCmpList.clear();	
	CAOIProject *ProjectPtr_Ref = GetRefProjectPtr();
	if ( NULL == ProjectPtr_Ref ) { return false; }
	CAOIProject *ProjectPtr_Host = GetHostProjectPtr();
	if ( NULL == ProjectPtr_Host ) { return false; }
	if ( m_RefBoardCombox.GetCount()==0 || m_HostBoardCombox.GetCount()==0 )
	{	return false; }
	CAOIBoard *BoardPtr_Ref = (CAOIBoard*)(JetAPI::GetComboxCurSelData(m_RefBoardCombox));
	if ( NULL == BoardPtr_Ref ) { return false; }
	CAOIBoard *BoardPtr_Host = (CAOIBoard*)(JetAPI::GetComboxCurSelData(m_HostBoardCombox));
	if ( NULL == BoardPtr_Host ) { return false; }

	size_t         i=0;	
	CString        str, str2;
	CString        ComponentName;	
	CAOIComponent *ComponentPtr_Ref = NULL;
	CAOIComponent *ComponentPtr_Host = NULL;	
	CAOIPanel     *PanelPtr_Ref = BoardPtr_Ref->GetBoardPanelPtr();
	CAOIPanel     *PanelPtr_Host = BoardPtr_Host->GetBoardPanelPtr();	
	const DISTRICT_ID DistrictID = ProjectPtr_Host->GetProjectActDistrictID();
	const size_t   ComponentCount_Ref = BoardPtr_Ref->GetBoardComponentCount();
	const size_t   ComponentCount_Host = BoardPtr_Host->GetBoardComponentCount();
	const BOOL     bDeleteChk = CWnd::IsDlgButtonChecked(PROCMP_DELETE_CHK);
	const BOOL     bPosChk = CWnd::IsDlgButtonChecked(PROCMP_POS_CHK);	
	const BOOL     bAngleChk = CWnd::IsDlgButtonChecked(PROCMP_ANGLE_CHK);
	const BOOL     bPartNumberChk = CWnd::IsDlgButtonChecked(PROCMP_PART_NUMBER_CHK);	
	double CadPosX_Ref=0;
	double CadPosY_Ref=0;
	double CadPosX_Host=0;
	double CadPosY_Host=0;
	double StagePosX_Host=0;
	double StagePosY_Host=0;
	CMapCoordinate *BoardMapCTSPtr = BoardPtr_Host->GetBoardMapCTSPtr(DistrictID);
	CMapCoordinate  MapFn;//座標轉換
	if ( CalcCadMap(MapFn) == false )
	{	return false; }

	TComponentCmp  ComponentCmp;
	for ( i=0; i<ComponentCount_Ref; i++ )
	{
		ComponentPtr_Ref = BoardPtr_Ref->GetBoardComponentPtr(i, false);
		if ( NULL == ComponentPtr_Ref ) { continue; }
		ComponentPtr_Ref->SetComponentTempIndex(0);
	}

	//先加入本機專案零件
	for ( i=0; i<ComponentCount_Host; i++ )
	{
		ComponentPtr_Host = BoardPtr_Host->GetBoardComponentPtr(i, false);
		if ( NULL == ComponentPtr_Host ) { continue; }

		ComponentCmp = TComponentCmp();
		ComponentName = ComponentPtr_Host->GetComponentName();
		ComponentPtr_Ref = BoardPtr_Ref->GetBoardComponentPtrByName(ComponentName);

		ComponentCmp.ProjectPtr = ProjectPtr_Host;
		ComponentCmp.PanelPtr = PanelPtr_Host;
		ComponentCmp.BoardPtr = BoardPtr_Host;
		ComponentCmp.ComponentPtr = ComponentPtr_Host;
		ComponentCmp.CadPosX = ComponentPtr_Host->GetComponentCadPosX();
		ComponentCmp.CadPosY = ComponentPtr_Host->GetComponentCadPosY();
		ComponentCmp.CadAngle = ComponentPtr_Host->GetComponentAngle();
		ComponentCmp.StagePosX = ComponentPtr_Host->GetComponentStagePosX();
		ComponentCmp.StagePosY = ComponentPtr_Host->GetComponentStagePosY();
		ComponentCmp.CmpRest = CMP_RES_NONE;		
		if ( NULL == ComponentPtr_Ref ) 
		{
			if ( TRUE == bDeleteChk )
			{	ComponentCmp.CmpRest = CMP_RES_DEL;	 }
			else
			{	ComponentCmp.CmpRest = CMP_RES_BYPASS;	 }
		}
		else
		{	
		#ifdef CMP_PART_VERSION_V2
			if ( TRUE == bPosChk )
			{					
				const double PosTol=10.0;//um
				CadPosX_Ref=ComponentPtr_Ref->GetComponentCadPosX();
				CadPosY_Ref=ComponentPtr_Ref->GetComponentCadPosY();
				MapFn.Map2D(CadPosX_Ref, CadPosY_Ref, CadPosX_Host, CadPosY_Host);//來源座標轉成本專案座標
				if ( fabs(CadPosX_Host-ComponentCmp.CadPosX)>PosTol || fabs(CadPosY_Host-ComponentCmp.CadPosY)>PosTol )
				{					
					BoardMapCTSPtr->Map2D(CadPosX_Host, CadPosY_Host, StagePosX_Host, StagePosY_Host);//來源座標轉成本專案座標
					ComponentCmp.CadPosX = CadPosX_Host;			
					ComponentCmp.CadPosY = CadPosY_Host;
					ComponentCmp.StagePosX = StagePosX_Host;
					ComponentCmp.StagePosY = StagePosY_Host;
					ComponentCmp.CmpRest |= CMP_RES_POS;	
				}
			}	
			if ( TRUE == bAngleChk )
			{	
				double Angle_Ref=ComponentPtr_Ref->GetComponentAngle();
				if ( fabs(Angle_Ref-ComponentCmp.CadAngle) > 0.1 )
				{
					ComponentCmp.CadAngle = Angle_Ref;		
					ComponentCmp.CmpRest |= CMP_RES_ANGLE;	
				}				
			}

			if ( TRUE == bPartNumberChk )
			{
				CString str1 = ComponentPtr_Host->GetComponentPartNumber();
				CString str2 = ComponentPtr_Ref->GetComponentPartNumber();
				if ( str1.CompareNoCase(str2) != 0 )
				{
					ComponentCmp.CmpRest |= CMP_RES_PART_NUMBER;	
					ComponentCmp.ModelName = ComponentPtr_Ref->GetComponentModelName();
					ComponentCmp.PartNumber = ComponentPtr_Ref->GetComponentPartNumber();
				}
			}
		#endif//CMP_PART_VERSION_V2
			ComponentPtr_Ref->SetComponentTempIndex(1); 
		}
		ComponentCmp.CmpRest2 = ComponentCmp.CmpRest;
		if ( CMP_RES_NONE != ComponentCmp.CmpRest )
		{	m_ComponentCmpList.push_back(ComponentCmp); }
	}

	size_t TempI=0;	
	for ( i=0; i<ComponentCount_Ref; i++ )
	{
		ComponentPtr_Ref = BoardPtr_Ref->GetBoardComponentPtr(i, false);
		if ( NULL == ComponentPtr_Ref ) { continue; }
		TempI = ComponentPtr_Ref->GetComponentTempIndex();
		if ( 0 != TempI ) { continue; }
		
		ComponentName = ComponentPtr_Ref->GetComponentName();		
		CadPosX_Ref = ComponentPtr_Ref->GetComponentCadPosX();
		CadPosY_Ref = ComponentPtr_Ref->GetComponentCadPosY();
		MapFn.Map2D(CadPosX_Ref, CadPosY_Ref, CadPosX_Host, CadPosY_Host);//來源座標轉成本專案座標
		BoardMapCTSPtr->Map2D(CadPosX_Host, CadPosY_Host, StagePosX_Host, StagePosY_Host);//來源座標轉成本專案座標

		ComponentCmp = TComponentCmp();
		ComponentCmp.ProjectPtr = ProjectPtr_Ref;
		ComponentCmp.PanelPtr = PanelPtr_Ref;
		ComponentCmp.BoardPtr = BoardPtr_Ref;
		ComponentCmp.ComponentPtr = ComponentPtr_Ref;
		ComponentCmp.CadPosX = CadPosX_Host;
		ComponentCmp.CadPosY = CadPosY_Host;
		ComponentCmp.StagePosX = StagePosX_Host;
		ComponentCmp.StagePosY = StagePosY_Host;
		ComponentCmp.CmpRest = CMP_RES_ADD;		
		ComponentCmp.CmpRest2 = ComponentCmp.CmpRest;
		m_ComponentCmpList.push_back(ComponentCmp);
	}	
	if ( m_ComponentCmpList.size() > 0 )
	{	SetMerged(false);	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectCompareWnd::BuildCompareListWnd()
{
	ClearCompareListWnd();
	CThisListCtrl_18 &ListCtrl = m_CompareListWnd;

	size_t         i=0;
	int            Res=0;
	CString        str;	
	int            nItem=0;
	int            nSubItem=0;
	TComponentCmp *ComponentCmpPtr = NULL;
	const size_t   ComponentCmpCount = m_ComponentCmpList.size();
	
	SetCompareListColID(CMP_COL_INDEX);	
	SetCompareListSortMode(SORT_ASCEND);

	ListCtrl.SetRedraw(FALSE);
	for ( i=0; i<ComponentCmpCount; i++ )
	{
		ComponentCmpPtr = &(m_ComponentCmpList[i]);
		if ( NULL == ComponentCmpPtr ) { continue; }
		if ( NULL == ComponentCmpPtr->ComponentPtr ) { continue; }

		nSubItem = 0;
		str.Format(_T("%d"), i+1);
		ListCtrl.InsertItem(nItem, str);
		ListCtrl.SetItemData(nItem, (DWORD_PTR)i);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;


		str = ComponentCmpPtr->ComponentPtr->GetComponentName();
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		Res = ComponentCmpPtr->CmpRest;
		str = GetCompareResultText(Res);			
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		Res = ComponentCmpPtr->CmpRest2;
		str = GetCompareResultText(Res);				
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//
		nItem ++;
	}
	ListCtrl.SetRedraw(TRUE);	

	SetCompareListColID(CMP_COL_INDEX);	
	SetCompareListSortMode(SORT_ASCEND);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectCompareWnd::ClearCompareListWnd()
{
	CThisListCtrl_18 &ListCtrl = m_CompareListWnd;
	JetAPI::ClearListCtrl(ListCtrl, FALSE);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectCompareWnd::BuildCompareListWndHeader()
{
	CThisListCtrl_18 &ListCtrl = m_CompareListWnd;
	JetAPI::ClearListCtrl(ListCtrl, FALSE);

	CString str;
	int  index = 0;
	int  Width = 48;
	int  Width2= 100;
	int  nAlign = LVCFMT_RIGHT;

	index = 0;
	str = _T("Index");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(index, str, nAlign, Width);	index++;

	str = _T("Component");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(index, str, nAlign, Width2);	index++;	

	str = _T("State");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(index, str, nAlign, Width2);	index++;	

	str = _T("State");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(index, str, nAlign, Width2);	index++;	

	return true;
}
//-------------------------------------------------------------------------------------//
void CProjectCompareWnd::OnCompareBtn() 
{
	// TODO: Add your control notification handler code here	
	if ( BuildCompareList() == false )
	{	return; }
	BuildCompareListWnd();	
	OnProjectMapBtn();
}
//-------------------------------------------------------------------------------------//
CString CProjectCompareWnd::GetCompareResultText(int Res) const
{	
	CString str;
	switch ( Res )
	{
	case CMP_RES_NONE:	str = m_strNone;	break;
	case CMP_RES_ADD:	str = m_strAdd;	break;
	case CMP_RES_DEL:	str = m_strDel;	break;
	case CMP_RES_BYPASS: str = m_strBypass;	break;	
	default:
		if ( 0 != (Res&CMP_RES_POS) )
		{
			if ( str.GetLength() == 0 )
			{	str = m_strPos; }			
			else
			{	str += CString(_T(", "))+m_strPos;	}
		}		
		if ( 0 != (Res&CMP_RES_ANGLE) )
		{
			if ( str.GetLength() == 0 )
			{	str = m_strAngle; }			
			else
			{	str += CString(_T(", "))+m_strAngle;	}
		}
		if ( 0 != (Res&CMP_RES_PART_NUMBER) )
		{
			if ( str.GetLength() == 0 )
			{	str = m_strPartNumber; }			
			else
			{	str += CString(_T(", "))+m_strPartNumber;	}
		}		
		//str.Format(_T("%d"), Res);	
		break;
	}
	return str;
}
//-------------------------------------------------------------------------------------//
void CProjectCompareWnd::SetMerged(bool val)
{
	m_Merged = val;
}
//-------------------------------------------------------------------------------------//
bool CProjectCompareWnd::GetMerged() const
{
	return m_Merged;
}
//-------------------------------------------------------------------------------------//
void CProjectCompareWnd::SetCompareListColID(int val)
{
	m_CompareColID = val;
}
//-------------------------------------------------------------------------------------//
int CProjectCompareWnd::GetCompareListColID() const
{
	return m_CompareColID;
}
//-------------------------------------------------------------------------------------//
void CProjectCompareWnd::SetCompareListSortMode(int val)
{
	m_CompareListSortMode = val;
}
//-------------------------------------------------------------------------------------//
int CProjectCompareWnd::GetCompareListSortMode() const
{
	return m_CompareListSortMode;	
}
//-------------------------------------------------------------------------------------//
int CProjectCompareWnd::CompareComponentItem(size_t index1, size_t index2)
{
	CThisListCtrl_18 &ListCtrl = m_CompareListWnd;
	const int ColID = GetCompareListColID();
	const size_t CompareCount = m_ComponentCmpList.size();
	if ( index1>=CompareCount || index2>=CompareCount )
	{	return 0; }
	int          Res=0;	
	wchar_t     *wsPtr1=NULL;
	wchar_t     *wsPtr2=NULL;
	unsigned int uVal1=0;
	unsigned int uVal2=0;	
	TComponentCmp *CPtr1=&(m_ComponentCmpList[index1]);
	TComponentCmp *CPtr2=&(m_ComponentCmpList[index2]);		
	if ( NULL==CPtr1->ComponentPtr || NULL==CPtr2->ComponentPtr ) { return 0; }
	switch ( ColID )
	{	
	case CMP_COL_INDEX:
		if ( index1 > index2 ) { Res = 1; }
		else if ( index1 < index2 ) { Res = -1; }
		else {	Res=0; }
		break;
	case CMP_COL_COMPONENT:
		wsPtr1 = (wchar_t*)(CPtr1->ComponentPtr->GetComponentName());
		wsPtr2 = (wchar_t*)(CPtr2->ComponentPtr->GetComponentName());
		Res = ::wcscmp(wsPtr1, wsPtr2);	
		break;
	case CMP_COL_RESULT:
		uVal1 = CPtr1->CmpRest;
		uVal2 = CPtr2->CmpRest;
		if ( uVal1 > uVal2 ) { Res = 1; }
		else if ( uVal1 < uVal2 ) { Res = -1; }
		else {	Res=0; }
		break;
	case CMP_COL_RESULT_II:
		uVal1 = CPtr1->CmpRest2;
		uVal2 = CPtr2->CmpRest2;
		if ( uVal1 > uVal2 ) { Res = 1; }
		else if ( uVal1 < uVal2 ) { Res = -1; }
		else {	Res=0; }
		break;
	}	
	int SortMode = GetCompareListSortMode();
	Res = Res*SortMode;	
	return Res;		
}
//-------------------------------------------------------------------------------------//
void CProjectCompareWnd::OnColumnclickCompareListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here	
	int ColID = GetCompareListColID();
	int SortMode = GetCompareListSortMode();	
	const int ColumnsIdx = pNMListView->iSubItem;
	if ( ColID != ColumnsIdx )
	{	SortMode = SORT_ASCEND; }
	else
	{
		if ( SORT_ASCEND == SortMode ) { SortMode = SORT_DESCEND; }
		else {	SortMode = SORT_ASCEND;  }
	}	
	SetCompareListColID(ColumnsIdx);	
	SetCompareListSortMode(SortMode);
	m_CompareListWnd.SortItems(CompareListCompareFn, (DWORD_PTR)this);
	
	const int nItem = m_CompareListWnd.GetNextItem(-1, LVNI_FOCUSED);
	if ( nItem >= 0 ) 
	{	m_CompareListWnd.EnsureVisible(nItem, FALSE); }

	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CProjectCompareWnd::OnRemoveBtn() 
{
	// TODO: Add your control notification handler code here
	CThisListCtrl_18 &ListCtrl = m_CompareListWnd;
		
	CString        str;		
	int            nItem=0;
	size_t         szIndex=0;	
	TComponentCmp *ComponentCmpPtr = NULL;	
	const int ItemCount = ListCtrl.GetItemCount();
	const size_t CompareCount = m_ComponentCmpList.size();

	POSITION pos = ListCtrl.GetFirstSelectedItemPosition();
	if ( NULL == pos ) { return; }

	ListCtrl.SetRedraw(FALSE);
	while (pos)
	{
		nItem = ListCtrl.GetNextSelectedItem(pos);
		szIndex = ListCtrl.GetItemData(nItem);
		if ( szIndex >= CompareCount ) { continue; }
		ComponentCmpPtr = &(m_ComponentCmpList[szIndex]);		
		ComponentCmpPtr->CmpRest2 = CMP_RES_NONE;
		str = GetCompareResultText(CMP_RES_NONE);
		ListCtrl.SetItemText(nItem, CMP_COL_RESULT_II, str);
	};
	ListCtrl.SetRedraw(TRUE);
	ListCtrl.SetFocus();
}
//-------------------------------------------------------------------------------------//
void CProjectCompareWnd::OnRestoreBtn() 
{
	// TODO: Add your control notification handler code here
	CThisListCtrl_18 &ListCtrl = m_CompareListWnd;
		
	int            Res;
	CString        str;		
	int            nItem=0;
	size_t         szIndex=0;	
	TComponentCmp *ComponentCmpPtr = NULL;	
	const int ItemCount = ListCtrl.GetItemCount();
	const size_t CompareCount = m_ComponentCmpList.size();

	POSITION pos = ListCtrl.GetFirstSelectedItemPosition();
	if ( NULL == pos ) { return; }

	ListCtrl.SetRedraw(FALSE);
	while (pos)
	{
		nItem = ListCtrl.GetNextSelectedItem(pos);
		szIndex = ListCtrl.GetItemData(nItem);
		if ( szIndex >= CompareCount ) { continue; }
		ComponentCmpPtr = &(m_ComponentCmpList[szIndex]);
		ComponentCmpPtr->CmpRest2 = ComponentCmpPtr->CmpRest;
		Res = ComponentCmpPtr->CmpRest2;
		str = GetCompareResultText(Res);		
		ListCtrl.SetItemText(nItem, CMP_COL_RESULT_II, str);
	};
	ListCtrl.SetRedraw(TRUE);
	ListCtrl.SetFocus();
}
//-------------------------------------------------------------------------------------//
void CProjectCompareWnd::OnMergeBtn() 
{
	// TODO: Add your control notification handler code here
	int            Res;
	size_t         i=0;
	size_t         j=0;
	CString        str;
	TComponentCmp *ComponentCmpPtr = NULL;
	const size_t CompareCount = m_ComponentCmpList.size();

	CAOIProject *ProjectPtr_Ref = GetRefProjectPtr();
	if ( NULL == ProjectPtr_Ref ) { return ; }
	CAOIProject *ProjectPtr_Host = GetHostProjectPtr();
	if ( NULL == ProjectPtr_Host ) { return ; }
	if ( m_RefBoardCombox.GetCount()==0 || m_HostBoardCombox.GetCount()==0 )
	{	return ; }
	CAOIBoard *BoardPtr_Ref = (CAOIBoard*)(JetAPI::GetComboxCurSelData(m_RefBoardCombox));
	if ( NULL == BoardPtr_Ref ) { return ; }
	CAOIBoard *BoardPtr_Host = (CAOIBoard*)(JetAPI::GetComboxCurSelData(m_HostBoardCombox));
	if ( NULL == BoardPtr_Host ) { return ; }
	CAOIPanel   *PanelPtr_Ref = BoardPtr_Ref->GetBoardPanelPtr();
	CAOIPanel   *PanelPtr_Host = BoardPtr_Host->GetBoardPanelPtr();
	if ( NULL==PanelPtr_Ref ||NULL==PanelPtr_Host ) { return ; }
	const DISTRICT_ID DistrictID = ProjectPtr_Host->GetProjectActDistrictID();
	CMapCoordinate *BoardMapCTSPtr = BoardPtr_Host->GetBoardMapCTSPtr(DistrictID);

	SetMerged(true);
	str = _T("Do you want to merge the components?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO )
	{	return ; }

	//輸出變更資料檔案
	str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("ProjectMerge.TXT"));
	ExecExportFile(str);


	//先刪除要移除的
	bool bDeleted = false;
	ProjectPtr_Host->SelectProjectAllComponents(false);
	for ( i=0; i<CompareCount; i++ )
	{
		ComponentCmpPtr = &(m_ComponentCmpList[i]);
		if ( NULL == ComponentCmpPtr ) { continue; }
		if ( NULL == ComponentCmpPtr->ComponentPtr ) { continue; }
		Res = ComponentCmpPtr->CmpRest2;
		if ( CMP_RES_DEL != Res )
		{	continue; }
		bDeleted = true;
		ComponentCmpPtr->ComponentPtr->SetComponentSelected(true);
		m_ComponentCmpList[i] = TComponentCmp();
	}
	if ( true == bDeleted )
	{
		LogOperCtrl.SaveLogProjectComponentSelectedDelete(ProjectPtr_Host);
		ProjectPtr_Host->DeleteProjectComponentSelected(); 
	}

	//先設定不檢測的
	bool bBypassed = false;
	ProjectPtr_Host->SelectProjectAllComponents(false);
	for ( i=0; i<CompareCount; i++ )
	{
		ComponentCmpPtr = &(m_ComponentCmpList[i]);
		if ( NULL == ComponentCmpPtr ) { continue; }
		if ( NULL == ComponentCmpPtr->ComponentPtr ) { continue; }
		Res = ComponentCmpPtr->CmpRest2;
		if ( CMP_RES_BYPASS != Res )
		{	continue; }
		bBypassed = true;
		ComponentCmpPtr->ComponentPtr->SetComponentSelected(true);
		m_ComponentCmpList[i] = TComponentCmp();
	}
	if ( true == bBypassed )
	{	ProjectPtr_Host->BypassProjectComponentSelected(true);	}

	//加入要新增的
	CAOIModel     *ModelPtr = NULL;
	CAOIComponent *ComponentPtr = NULL;
	std::vector<CAOIComponent*> NewComponentList;
	for ( i=0; i<CompareCount; i++ )
	{
		ComponentCmpPtr = &(m_ComponentCmpList[i]);
		if ( NULL == ComponentCmpPtr ) { continue; }
		if ( NULL == ComponentCmpPtr->ComponentPtr ) { continue; }
		Res = ComponentCmpPtr->CmpRest2;
		if ( CMP_RES_ADD != Res )
		{	continue; }
		
		ComponentPtr = ComponentCmpPtr->ComponentPtr->CloneComponentObj();
		if ( NULL == ComponentPtr ) { continue; }
		ComponentPtr->SetComponentTempIndex(0);
		ModelPtr = ComponentPtr->GetComponentModelPtr();
		if ( NULL == ModelPtr )
		{
			AOIObjManager.DestroyComponentObj(ComponentPtr);
			ComponentPtr = NULL;
			continue; 
		}
		ModelPtr->ClearModelAllObjList();		
		ModelPtr->SetModelType(MODEL_TYPE_NULL);
		ComponentPtr->SetComponentCadPosX(ComponentCmpPtr->CadPosX);
		ComponentPtr->SetComponentCadPosY(ComponentCmpPtr->CadPosY);
		ComponentPtr->SetComponentStagePosX(ComponentCmpPtr->StagePosX);
		ComponentPtr->SetComponentStagePosY(ComponentCmpPtr->StagePosY);
		ComponentPtr->CalcComponentCadCornerPos();
		ComponentPtr->LayoutComponentStageCornerPos();
		ComponentPtr->UpdateComponentParamToModel(false);
		ModelPtr->CalcModelTotalRegionAll();

		NewComponentList.push_back(ComponentPtr);

		ProjectPtr_Host->AddProjectComponentPtr(ComponentPtr, false);
		PanelPtr_Host->AddPanelComponentPtr(ComponentPtr);
		BoardPtr_Host->AddBoardComponentPtr(ComponentPtr);
	}

	
#ifdef CMP_PART_VERSION_V2
	std::wstring wsModelName;
	std::wstring wsPartNumber;
	for ( i=0; i<CompareCount; i++ )
	{
		ComponentCmpPtr = &(m_ComponentCmpList[i]);
		if ( NULL == ComponentCmpPtr ) { continue; }
		if ( NULL == ComponentCmpPtr->ComponentPtr ) { continue; }
		CAOIComponent *ComponentPtr = ComponentCmpPtr->ComponentPtr;
		if ( NULL == ComponentPtr ) { continue; }
		Res = ComponentCmpPtr->CmpRest2;	
		if ( 0 != (CMP_RES_POS&Res) )
		{	
			ComponentPtr->SetComponentCadPosX(ComponentCmpPtr->CadPosX);
			ComponentPtr->SetComponentCadPosY(ComponentCmpPtr->CadPosY);
			ComponentPtr->SetComponentStagePosX(ComponentCmpPtr->StagePosX);
			ComponentPtr->SetComponentStagePosY(ComponentCmpPtr->StagePosY);
			ComponentPtr->CalcComponentCadCornerPos();
			ComponentPtr->LayoutComponentStageCornerPos();		
			ComponentPtr->UpdateComponentParamToModel(false);
		}
		if ( 0 != (CMP_RES_ANGLE&Res) )
		{	
			if ( 0 != (CMP_RES_PART_NUMBER&Res) )
			{	ComponentPtr->SetComponentAngle(ComponentCmpPtr->CadAngle);		}
			else
			{
				double CadPosX=ComponentPtr->GetComponentCadPosX();
				double CadPosY=ComponentPtr->GetComponentCadPosY();
				double AngleDif=ComponentCmpPtr->CadAngle-ComponentPtr->GetComponentAngle();
				ComponentPtr->RotateComponent(AngleDif, CadPosX, CadPosY, BoardMapCTSPtr);
			}
		}	

		if ( 0 == (CMP_RES_PART_NUMBER&Res) )
		{	continue; }		
		if ( ComponentPtr->GetComponentModelIsolated() == true ) { continue; }
		JetAPI::TCHAR2wstring(ComponentCmpPtr->ModelName, wsModelName);
		JetAPI::TCHAR2wstring(ComponentCmpPtr->PartNumber, wsPartNumber);
		ComponentPtr->SetComponentModelIndex(-1);		
		ComponentPtr->SetComponentModelName(wsModelName.c_str());
		ComponentPtr->SetComponentPartNumber(wsPartNumber.c_str());		
		CAOIModel *ModelPtr = ComponentPtr->GetComponentModelPtr();
		if ( NULL != ModelPtr )
		{
			ModelPtr->ClearModelAllObjList();		
			ModelPtr->SetModelType(MODEL_TYPE_NULL);
			ModelPtr->SetModelName(wsModelName.c_str());
		}
		NewComponentList.push_back(ComponentPtr);//接續後面套用資料庫
	}
#endif//CMP_PART_VERSION_V2

	//連動資料庫
	CString        ModelName;
	CString        ModelName2;
	size_t         TempIndex=0;
	size_t         TempIndex2=0;
	CAOIComponent *ComponentPtr2 = NULL;
	const size_t NewComponentCount = NewComponentList.size();
	for ( i=0; i<NewComponentCount; i++ )
	{
		ComponentPtr = (NewComponentList[i]);
		if ( NULL == ComponentPtr )  { continue; }
		TempIndex = ComponentPtr->GetComponentTempIndex();
		if ( 0 != TempIndex ) { continue; }

		ComponentPtr->SetComponentTempIndex(1);
		ModelName = ComponentPtr->GetComponentModelName();
		ModelPtr = ProjectPtr_Host->GetProjectModelPtrByModelName(ModelName);
		if ( NULL == ModelPtr ) { continue; }

		ProjectPtr_Host->SelectProjectAllComponents(false);
		ComponentPtr->SetComponentTempIndex(2);
		ComponentPtr->SetComponentSelected(true);
		for ( j=i; j<NewComponentCount; j++ )
		{
			ComponentPtr2 = (NewComponentList[i]);
			if ( NULL == ComponentPtr2 )  { continue; }
			TempIndex2 = ComponentPtr2->GetComponentTempIndex();
			if ( 0 != TempIndex2 ) { continue; }

			ModelName2 = ComponentPtr2->GetComponentModelName();
			if ( ModelName.CompareNoCase(ModelName2) != 0 ) { continue; }
			ComponentPtr2->SetComponentTempIndex(2);
			ComponentPtr2->SetComponentSelected(true);
		}
		ProjectPtr_Host->ApplyProjectModelToComponentsSelected(ModelPtr);
	}

	m_ComponentCmpList.clear();
	ClearCompareListWnd();
	BuildHostComponentListWnd();
}
//-------------------------------------------------------------------------------------//
void CProjectCompareWnd::OnExportFileBtn() 
{
	// TODO: Add your control notification handler code here
	const size_t CompareCount = m_ComponentCmpList.size();
	if ( 0 == CompareCount ) { return; }

	TCHAR szFilters[]=_T("TXT Files (*.TXT)|*.TXT|All Files (*.*)|*.*||");
	CFileDialog dialog (FALSE, _T("TXT"), _T("*.TXT"), OFN_FILEMUSTEXIST, szFilters);
	if ( dialog.DoModal() == IDCANCEL )
	{	return ; }
	
	CString filename = dialog.GetPathName();
	ExecExportFile(filename);
}
//-------------------------------------------------------------------------------------//
bool CProjectCompareWnd::CalcCadMap(CMapCoordinate &Map)
{
	Map.Identity();
	CAOIProject *ProjectPtr_Ref = GetRefProjectPtr();
	if ( NULL == ProjectPtr_Ref ) { return false; }
	CAOIProject *ProjectPtr_Host = GetHostProjectPtr();
	if ( NULL == ProjectPtr_Host ) { return false; }
	CAOIBoard *BoardPtr_Ref = (CAOIBoard*)(JetAPI::GetComboxCurSelData(m_RefBoardCombox));
	if ( NULL == BoardPtr_Ref ) { return false; }
	CAOIBoard *BoardPtr_Host = (CAOIBoard*)(JetAPI::GetComboxCurSelData(m_HostBoardCombox));
	if ( NULL == BoardPtr_Host ) { return false; }

	size_t         i=0;	
	CString        str, str2;
	CString        ComponentName;	
	CAOIComponent *ComponentPtr_Ref = NULL;
	CAOIComponent *ComponentPtr_Host = NULL;	
	std::vector<CAOIComponent*> SameComponentList;//相同名稱的零件列表
	CAOIPanel     *PanelPtr_Ref = BoardPtr_Ref->GetBoardPanelPtr();
	CAOIPanel     *PanelPtr_Host = BoardPtr_Host->GetBoardPanelPtr();	
	const DISTRICT_ID DistrictID = ProjectPtr_Host->GetProjectActDistrictID();
	const size_t   ComponentCount_Ref = BoardPtr_Ref->GetBoardComponentCount();
	const size_t   ComponentCount_Host = BoardPtr_Host->GetBoardComponentCount();

	//先加入本機專案零件
	for ( i=0; i<ComponentCount_Host; i++ )
	{
		ComponentPtr_Host = BoardPtr_Host->GetBoardComponentPtr(i, false);
		if ( NULL == ComponentPtr_Host ) { continue; }

		ComponentName = ComponentPtr_Host->GetComponentName();
		ComponentPtr_Ref = BoardPtr_Ref->GetBoardComponentPtrByName(ComponentName);
		if ( NULL == ComponentPtr_Ref ) { continue;	}		
		SameComponentList.push_back(ComponentPtr_Host);//相同名稱
	}

	//確認重複零件, 需要重複零件來匹配座標
	const size_t SameComponentCount = SameComponentList.size();
	if ( SameComponentCount < 2 )
	{
		str = _T("Error, the same comopenent name too less");
		str2.Format(_T("%s[%d < 2]"), str, SameComponentCount);
		JetAPI::ShowMessageBox(str2);
		return false;
	}

	//距離最大的兩個零件	
	int    Count=0;
	double RegionCpX=0;
	double RegionCpY=0;
	double ComponentCpX_Host=0;
	double ComponentCpY_Host=0;
	double DistanceX=0, DistanceY=0, DistanceL=0;
	double MaxDistanceRT=0;
	double MaxDistanceRB=0;
	double MaxDistanceLT=0;
	double MaxDistanceLB=0;
	CAOIComponent *ComponentPtr_RT=NULL;
	CAOIComponent *ComponentPtr_RB=NULL;
	CAOIComponent *ComponentPtr_LT=NULL;
	CAOIComponent *ComponentPtr_LB=NULL;
	for ( i=0; i<SameComponentCount; i++ )
	{		
		ComponentPtr_Host = SameComponentList[i];
		if ( NULL == ComponentPtr_Host ) { continue; }
		ComponentCpX_Host = ComponentPtr_Host->GetComponentCadPosX();
		ComponentCpY_Host = ComponentPtr_Host->GetComponentCadPosY();
		RegionCpX += ComponentCpX_Host;
		RegionCpY += ComponentCpY_Host;
		Count ++;
	}
	if ( Count > 0 ) 
	{
		RegionCpX /= Count;
		RegionCpY /= Count;
	}	
	for ( i=0; i<SameComponentCount; i++ )
	{	
		ComponentPtr_Host = SameComponentList[i];
		if ( NULL == ComponentPtr_Host ) { continue; }
		ComponentCpX_Host = ComponentPtr_Host->GetComponentCadPosX();
		ComponentCpY_Host = ComponentPtr_Host->GetComponentCadPosY();
		DistanceX = ComponentCpX_Host-RegionCpX;
		DistanceY = ComponentCpY_Host-RegionCpY;
		DistanceL = sqrt((DistanceX*DistanceX)+(DistanceY*DistanceY));
		if ( ComponentCpX_Host>RegionCpX )//Right
		{
			if ( ComponentCpY_Host>RegionCpY )//Right Top
			{
				if ( MaxDistanceRT < DistanceL ) 
				{					
					MaxDistanceRT = DistanceL; 
					ComponentPtr_RT = ComponentPtr_Host;
				}
			}
			else//Right Bottom
			{
				if ( MaxDistanceRB < DistanceL ) 
				{					
					MaxDistanceRB = DistanceL; 
					ComponentPtr_RB = ComponentPtr_Host;
				}
			}
			continue;
		}
		else//Left
		{
			if ( ComponentCpY_Host>RegionCpY )//Left Top
			{
				if ( MaxDistanceLT < DistanceL ) 
				{	
					MaxDistanceLT = DistanceL; 
					ComponentPtr_LT = ComponentPtr_Host;
				}
			}
			else//Left Bottom
			{
				if ( MaxDistanceLB < DistanceL ) 
				{	
					MaxDistanceLB = DistanceL; 
					ComponentPtr_LB = ComponentPtr_Host;
				}
			}
		}
	}

	//座標轉換
	CMapCoordinate  MapFn;
	size_t       MapCount=0 ;
	const size_t MapMaxCount=2;
	double   SrcX[4]={0};
	double   SrcY[4]={0};
	double   DstX[4]={0};
	double   DstY[4]={0};	
		
	if ( MapCount<MapMaxCount && NULL!=ComponentPtr_RT )
	{
		ComponentName = ComponentPtr_RT->GetComponentName();
		ComponentPtr_Host = ComponentPtr_RT;
		ComponentPtr_Ref = BoardPtr_Ref->GetBoardComponentPtrByName(ComponentName);

		SrcX[MapCount] = ComponentPtr_Ref->GetComponentCadPosX();
		SrcY[MapCount] = ComponentPtr_Ref->GetComponentCadPosY();
		DstX[MapCount] = ComponentPtr_Host->GetComponentCadPosX();
		DstY[MapCount] = ComponentPtr_Host->GetComponentCadPosY();		
		MapCount ++;
	}
	if ( MapCount<MapMaxCount && NULL!=ComponentPtr_LB )
	{
		ComponentName = ComponentPtr_LB->GetComponentName();
		ComponentPtr_Host = ComponentPtr_LB;
		ComponentPtr_Ref = BoardPtr_Ref->GetBoardComponentPtrByName(ComponentName);

		SrcX[MapCount] = ComponentPtr_Ref->GetComponentCadPosX();
		SrcY[MapCount] = ComponentPtr_Ref->GetComponentCadPosY();
		DstX[MapCount] = ComponentPtr_Host->GetComponentCadPosX();
		DstY[MapCount] = ComponentPtr_Host->GetComponentCadPosY();		
		MapCount ++;
	}
	if ( MapCount<MapMaxCount && NULL!=ComponentPtr_RB )
	{
		ComponentName = ComponentPtr_LB->GetComponentName();
		ComponentPtr_Host = ComponentPtr_RB;
		ComponentPtr_Ref = BoardPtr_Ref->GetBoardComponentPtrByName(ComponentName);

		SrcX[MapCount] = ComponentPtr_Ref->GetComponentCadPosX();
		SrcY[MapCount] = ComponentPtr_Ref->GetComponentCadPosY();
		DstX[MapCount] = ComponentPtr_Host->GetComponentCadPosX();
		DstY[MapCount] = ComponentPtr_Host->GetComponentCadPosY();		
		MapCount ++;
	}
	if ( MapCount<MapMaxCount && NULL!=ComponentPtr_LT )
	{
		ComponentName = ComponentPtr_LB->GetComponentName();
		ComponentPtr_Host = ComponentPtr_LT;
		ComponentPtr_Ref = BoardPtr_Ref->GetBoardComponentPtrByName(ComponentName);

		SrcX[MapCount] = ComponentPtr_Ref->GetComponentCadPosX();
		SrcY[MapCount] = ComponentPtr_Ref->GetComponentCadPosY();
		DstX[MapCount] = ComponentPtr_Host->GetComponentCadPosX();
		DstY[MapCount] = ComponentPtr_Host->GetComponentCadPosY();		
		MapCount ++;
	}
	MapFn.CalcMatrix2D(SrcX, SrcY, DstX, DstY, MapCount, true, true);
	Map = MapFn;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectCompareWnd::ExecExportFile(LPCTSTR filename)
{
	if ( NULL==filename ) { return false; }

	FILE   *pfile = NULL;	
	TCHAR   TMode[32] = _T("");
	_tcscpy(TMode, _T("w+"));
	JetAPI::ModifyOpenFileMode_Write(TMode);	
	pfile = ::_tfopen(filename, TMode);
	if ( NULL == pfile ) { return false; }

	int            Res;
	size_t         i=0;	
	CString        str;	
	CString        strRes;
	CString        ComponentName;
	CAOIComponent *ComponentPtr = NULL;
	TComponentCmp *ComponentCmpPtr = NULL;
	const size_t CompareCount = m_ComponentCmpList.size();

	::_ftprintf(pfile, L"Component, State\n");
	for ( i=0; i<CompareCount; i++ )
	{
		ComponentCmpPtr = &(m_ComponentCmpList[i]);
		if ( NULL == ComponentCmpPtr ) { continue; }		
		Res = ComponentCmpPtr->CmpRest2;
		if ( CMP_RES_NONE == Res )	{	continue; }
		ComponentPtr = ComponentCmpPtr->ComponentPtr;
		if ( NULL == ComponentPtr ) { continue; }

		ComponentName = ComponentPtr->GetComponentName();
		strRes = GetCompareResultText(Res);		
		::_ftprintf(pfile, L"%s, %s\n", ComponentName, strRes);		
	}
	::fclose(pfile); pfile=NULL;	
	::ShellExecute(NULL, _T("open"), filename, NULL, NULL, SW_SHOW);
	return true;
}
//-------------------------------------------------------------------------------------//
void CProjectCompareWnd::OnProjectMapBtn() 
{
	// TODO: Add your control notification handler code here
	if ( m_ProjectMapWnd.GetSafeHwnd() == NULL ) { return; }
	if ( m_ProjectMapWnd.IsWindowVisible() == TRUE ) { return; }
	m_ProjectMapWnd.ShowWindow(SW_SHOW);
}
//-------------------------------------------------------------------------------------//
bool CProjectCompareWnd::ExecMoveToStagePos(double PosX, double PosY)
{	
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();
	MotionCtrlPtr->XYMoveTo(PosX, PosY, OfflineMode);
	MotionCtrlPtr->WaitForMotionStop();
	if ( m_ProjectMapWnd.IsWindowVisible() == TRUE )
	{	
		m_ProjectMapWnd.MoveViewToStagePos(PosX, PosY);
		//m_ProjectMapWnd.RedrawWnd(FALSE); 
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CProjectCompareWnd::OnDblclkCompareListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NMITEMACTIVATE *pNMListView = (NMITEMACTIVATE*)pNMHDR;	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) { return; }
	const size_t CompareCount = m_ComponentCmpList.size();	
	const size_t szIndex = m_CompareListWnd.GetItemData(nItem);	
	if ( szIndex >= CompareCount ) { return ; }
	TComponentCmp *ComponentCmpPtr = &(m_ComponentCmpList[szIndex]);
	CAOIProject* ProjectPtr = GetHostProjectPtr();
	CAOIComponent *ComponentPtr = ComponentCmpPtr->ComponentPtr;
	if ( NULL!=ComponentPtr && NULL!=ProjectPtr )
	{
		ProjectPtr->SelectProjectAllComponents(false);
		ComponentPtr->SetComponentSelected(true);
	}
	const double StagePosX = ComponentCmpPtr->StagePosX;
	const double StagePosY = ComponentCmpPtr->StagePosY;
	ExecMoveToStagePos(StagePosX, StagePosY);
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CProjectCompareWnd::OnDblclkHostComponentListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NMITEMACTIVATE *pNMListView = (NMITEMACTIVATE*)pNMHDR;	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) { return; }
	const size_t CompareCount = m_ComponentCmpList.size();	
	CAOIComponent *ComponentPtr = (CAOIComponent*)m_HostComponentListWnd.GetItemData(nItem);	
	if ( NULL == ComponentPtr ) { return; }		
	const double StagePosX = ComponentPtr->GetComponentStagePosX();
	const double StagePosY = ComponentPtr->GetComponentStagePosY();
	ExecMoveToStagePos(StagePosX, StagePosY);
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//