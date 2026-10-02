// ComponentDefectAlarmListWnd.cpp : implementation file
//

#include "stdafx.h"
#include "jet8000.h"
#include "ComponentDefectAlarmListWnd.h"
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
#define    CMP_COL_COMPONENT            3
#define    CMP_COL_ALARM_ENABLED_ON_AOI 4
#define    CMP_COL_ALARM_ENABLED_ON_ARS 5
#define    CMP_COL_ALARM_FROM_MODE      6
#define    CMP_COL_DEFECT_BEGIN         7
//-------------------------------------------------------------------------------------//
int CALLBACK ComponentListCompareFn2(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
int CALLBACK ComponentListCompareFn2(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	if ( NULL == lParamSort ) { return 0; }
	CComponentDefectAlarmListWnd* pComponentListWnd = (CComponentDefectAlarmListWnd*)lParamSort;
	return pComponentListWnd->CompareComponentItem(lParam1, lParam2);
}
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CComponentDefectAlarmListWnd dialog
//-------------------------------------------------------------------------------------//
CComponentDefectAlarmListWnd::CComponentDefectAlarmListWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CComponentDefectAlarmListWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CComponentDefectAlarmListWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT	
	m_ProjectPtr = NULL;
	m_ComponentPtr = NULL;
	m_AlarmFromMode = DEFECT_FROM_AOI;
	m_ComponentColID = CMP_COL_INDEX;	
	m_ComponentListSortMode = SORT_ASCEND;
	m_StopComponentListBeSelected = false;
	m_WndDefectIDList = AOIDataCollect.GetWndDefectIDList();
}
//-------------------------------------------------------------------------------------//
void CComponentDefectAlarmListWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CComponentDefectAlarmListWnd)		
	DDX_Control(pDX, COMDEFECTLIST_ALARM_FROM_COMBO, m_AlarmFromCombox);
	DDX_Control(pDX, COMDEFECTLIST_COMPONENT_LIST_WND, m_ComponentListWnd);	
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CComponentDefectAlarmListWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CComponentDefectAlarmListWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_GETMINMAXINFO()
	ON_NOTIFY(LVN_COLUMNCLICK, COMDEFECTLIST_COMPONENT_LIST_WND, OnColumnclickComponentListWnd)
	ON_CBN_SELCHANGE(COMDEFECTLIST_ALARM_FROM_COMBO, OnSelchangeAlarmFromCombo)
	ON_BN_CLICKED(COMDEFECTLIST_SAVE_FILE_BTN, OnSaveFileBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CComponentDefectAlarmListWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CComponentDefectAlarmListWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CWnd::ShowWindow(SW_MAXIMIZE);
	JetAPI::InitialListCtrl(m_ComponentListWnd);
	
	BuildWndDefectIDList();
	BuildComponentListWndHeader();
	AOIDataDefine.BuildDefectFromCombox(m_AlarmFromCombox);

	SwitchMultiLanguage();	
	JetAPI::SetComboxCurSel(m_AlarmFromCombox, GetAlarmFromMode());
	BuildComponentListWnd(GetBuildAllComponentList());
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	m_ComponentListWnd.SetFocus();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CComponentDefectAlarmListWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CComponentDefectAlarmListWnd::OnSize(UINT nType, int cx, int cy) 
{
	CBaseDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CWnd     *WndPtr=NULL;
	const int MarginW=4;
	const int MarginH=4;

	if ( m_ComponentListWnd.GetSafeHwnd() != NULL )
	{
		RECT WndRect={0};
		m_ComponentListWnd.GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		WndRect.right = cx-MarginW;
		WndRect.bottom = cy-MarginH;
		m_ComponentListWnd.MoveWindow(&WndRect);
	}	
}
//-------------------------------------------------------------------------------------//
void CComponentDefectAlarmListWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CBaseDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CComponentDefectAlarmListWnd::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default	
	CBaseDialog::OnGetMinMaxInfo(lpMMI);
	lpMMI->ptMinTrackSize.x = 810;
	lpMMI->ptMinTrackSize.y = 400;
}
//-------------------------------------------------------------------------------------//
void CComponentDefectAlarmListWnd::OnColumnclickComponentListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	int ColID = GetComponentListColD();
	int SortMode = GetComponentListSortMode();	
	const int ColumnsIdx = pNMListView->iSubItem;
	if ( ColID != ColumnsIdx )
	{	SortMode = SORT_ASCEND; }
	else
	{
		if ( SORT_ASCEND == SortMode ) { SortMode = SORT_DESCEND; }
		else {	SortMode = SORT_ASCEND;  }
	}
	SetComponentListColID(ColumnsIdx);	
	SetComponentListSortMode(SortMode);	
	m_ComponentListWnd.SortItems(ComponentListCompareFn2, (DWORD_PTR)this);
	
	const int nItem = m_ComponentListWnd.GetNextItem(-1, LVNI_FOCUSED);
	if ( nItem >= 0 ) 
	{	m_ComponentListWnd.EnsureVisible(nItem, FALSE); }

	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CComponentDefectAlarmListWnd::OnSelchangeAlarmFromCombo()
{
	// TODO: Add your control notification handler code here
	SetAlarmFromMode((DEFECT_FROM_MODE)(JetAPI::GetComboxCurSelData(m_AlarmFromCombox)));
	BuildComponentListWnd(GetBuildAllComponentList());
}
//-------------------------------------------------------------------------------------//
void CComponentDefectAlarmListWnd::OnSaveFileBtn()
{
	// TODO: Add your control notification handler code here
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }

	TCHAR szFilters[]=_T("TXT Files (*.TXT)|*.TXT|All Files (*.*)|*.*||");
	CFileDialog dialog (FALSE, _T("TXT"), _T("*.TXT"), OFN_FILEMUSTEXIST, szFilters);
	if ( dialog.DoModal() == IDCANCEL )
	{	return ; }

	CString filename=dialog.GetPathName();
	const bool bSucc=ProjectPtr->SaveProjectComponentDefectAlarmFile(filename, GetAlarmFromMode());
	if ( false == bSucc )
	{	JetAPI::ShowMessageBox(ProjectPtr->GetProjectErrorString());	}
	return;
}
//-------------------------------------------------------------------------------------//
void CComponentDefectAlarmListWnd::SetProjectPtr(CAOIProject *Ptr)
{
	m_ProjectPtr = Ptr;		
	m_ComponentPtr = NULL;
	SetComponentListSortMode(1);	
	SetComponentListColID(CMP_COL_INDEX);
}
//-------------------------------------------------------------------------------------//
inline CAOIProject* CComponentDefectAlarmListWnd::GetActiveProject()
{
	return m_ProjectPtr;
}
//-------------------------------------------------------------------------------------//
void CComponentDefectAlarmListWnd::SwitchMultiLanguage()
{
	//---------------------------------------------------------------------------------//	
	SetMultiLanauage(LoadIDAndName(IDD_COMPONENT_DEFECT_ALARM_LIST_WND), false);
	//---------------------------------------------------------------------------------//	
	SetMultiLanauage(LoadIDAndName(IDOK));
	SetMultiLanauage(LoadIDAndName(IDCANCEL));
	//---------------------------------------------------------------------------------//	
	SetMultiLanauage(LoadIDAndName(COMDEFECTLIST_ALARM_FROM_LABEL));
	SetMultiLanauage(LoadIDAndName(COMDEFECTLIST_SAVE_FILE_BTN));
	//SetMultiLanauage(LoadIDAndName(COMLIST_SEL_PART_NUMBER_LABEL));
	//SetMultiLanauage(LoadIDAndName(COMLIST_SEL_MODEL_NAME_LABEL));
	//SetMultiLanauage(LoadIDAndName(COMLIST_SEL_NOZZLE_LABEL));	
	//---------------------------------------------------------------------------------//	
}
//-------------------------------------------------------------------------------------//
bool CComponentDefectAlarmListWnd::SetMultiLanauage(UINT ID, LPCTSTR Text, bool bCtrlID)
{
	LPCTSTR Section=_T("IDD_COMPONENT_DEFECT_ALARM_LIST_WND");		
	if ( AOIDataCollect.SwitchMultiLanguageWnd(this, Section, ID, Text, bCtrlID) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
CString CComponentDefectAlarmListWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_COMPONENT_DEFECT_ALARM_LIST_WND");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
bool CComponentDefectAlarmListWnd::GetBuildAllComponentList() const
{
	return false;
}
//-------------------------------------------------------------------------------------//
bool CComponentDefectAlarmListWnd::BuildComponentListWnd(bool bAll)
{
	CThisListCtrl_67 &ListCtrl = m_ComponentListWnd;
	CAOIProject *ProjectPtr = GetActiveProject();
	ClearComponentListWnd();
	if ( NULL == ProjectPtr )
	{	return true; }
	
	size_t         i=0, j=0;
	CString        str;
	unsigned int   PanelIndex=0;
	unsigned int   BoardIndex=0;
	int            nItem=0;
	int            nSubItem=0;	
	int            nValue=0;
	bool           bAlarm=false;		
	CAOIComponent *ComponentPtr = NULL;
	DEFECT_FROM_MODE AlarmFromMode = GetAlarmFromMode();
	const std::vector<WND_DEFECT_ID> &WndDefectIDList=GetWndDefectIDList();
	const size_t WndDefectIDCount=WndDefectIDList.size();
	const size_t ComponentCount = ProjectPtr->GetProjectComponentCount();

	ListCtrl.SetRedraw(FALSE);
	m_StopComponentListBeSelected = true;
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = ProjectPtr->GetProjectComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }
		
		PanelIndex = ComponentPtr->GetComponentPanelIndex_Project();
		BoardIndex = ComponentPtr->GetComponentBoardIndex_Panel();		
		const CWndDefectItem &DefectItemAOI=ComponentPtr->GetComponentDefectItemAlarmAOI();
		const CWndDefectItem &DefectItemARS=ComponentPtr->GetComponentDefectItemAlarmARS();
		if ( false == bAll )
		{
			if ( DEFECT_FROM_AOI == AlarmFromMode )
			{
				if ( 0 == DefectItemAOI.CalcSum() )
				{	continue; }
			}
			if ( DEFECT_FROM_ARS == AlarmFromMode )
			{
				if ( 0 == DefectItemARS.CalcSum() )
				{	continue; }
			}
		}
		nSubItem = 0;
		str.Format(_T("%d"), i+1);
		ListCtrl.InsertItem(nItem, str);
		ListCtrl.SetItemData(nItem, (DWORD_PTR)m_ComponentList.size());
		m_ComponentList.push_back(ComponentPtr);

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

		//CMP_COL_COMPONENT
		str = ComponentPtr->GetComponentName();
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;
		
		//CMP_COL_ALARM_ENABLED_ON_AOI
		bAlarm = ComponentPtr->GetComponentDefectAlarmEnableOnAOI();
		if ( true == bAlarm ) 
		{	str = _T(""); }
		else
		{	str = _T("N"); }
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//CMP_COL_ALARM_ENABLED_ON_ARS
		bAlarm = ComponentPtr->GetComponentDefectAlarmEnableOnARS();
		if ( true == bAlarm ) 
		{	str = _T(""); }
		else
		{	str = _T("N"); }
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//CMP_COL_ALARM_FROM_MODE
		str = AOIDataDefine.GetDefectParamFromText(ComponentPtr->GetComponentDefectAlarmFromModeAOI());
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		nValue = 0;
		//CMP_COL_DEFECT_BEGIN		
		for ( j=0; j<WndDefectIDCount; j++ )
		{
			WND_DEFECT_ID DefectID=WndDefectIDList[j];
			if ( DEFECT_FROM_AOI == AlarmFromMode )
			{	nValue = DefectItemAOI.GetItemCount(DefectID);	}
			if ( DEFECT_FROM_ARS == AlarmFromMode )
			{	nValue = DefectItemARS.GetItemCount(DefectID);	}
			if ( 0 == nValue )
			{	str = _T(""); }
			else
			{	str = _T("Y"); }
			ListCtrl.SetItemText(nItem, nSubItem, str);
			nSubItem ++;
		}
		//----
		nItem ++;
	}
	m_StopComponentListBeSelected = false;
	ListCtrl.SetRedraw(TRUE);

	SetComponentListColID(CMP_COL_INDEX);
	SetComponentListSortMode(SORT_ASCEND);

	CString strCount = AOIDataDefine.GetCountText();
	str.Format(_T("%s:%d"), strCount, ListCtrl.GetItemCount());
	CWnd::SetDlgItemText(COMDEFECTLIST_INFO_EDIT, str);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CComponentDefectAlarmListWnd::UpdateComponentListWnd()
{
	CThisListCtrl_67 &ListCtrl = m_ComponentListWnd;		
	
	size_t         i=0, j=0;
	CString        str;
	unsigned int   PanelIndex=0;
	unsigned int   BoardIndex=0;
	unsigned int   ComponentIndex=0;
	int            nItem=0;
	int            nSubItem=0;	
	int            nValue=0;
	bool           bAlarm=false;
	CAOIComponent *ComponentPtr = NULL;	
	const int      ItemCount = ListCtrl.GetItemCount();	
	DEFECT_FROM_MODE AlarmFromMode = GetAlarmFromMode();
	const std::vector<WND_DEFECT_ID> &WndDefectIDList=GetWndDefectIDList();
	const size_t WndDefectIDCount=WndDefectIDList.size();
	const size_t   ComponentCount = m_ComponentList.size();

	ListCtrl.SetRedraw(FALSE);	
	for ( i=0; i<ItemCount; i++ )
	{
		ComponentIndex = ListCtrl.GetItemData(i);
		if ( ComponentIndex >= ComponentCount ) { continue; }
		ComponentPtr = m_ComponentList[ComponentIndex];
		if ( NULL == ComponentPtr ) { continue; }
		
		PanelIndex = ComponentPtr->GetComponentPanelIndex_Project();
		BoardIndex = ComponentPtr->GetComponentBoardIndex_Panel();
		const CWndDefectItem &DefectItemAOI=ComponentPtr->GetComponentDefectItemAlarmAOI();
		const CWndDefectItem &DefectItemARS=ComponentPtr->GetComponentDefectItemAlarmARS();

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

		//CMP_COL_COMPONENT
		str = ComponentPtr->GetComponentName();
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;
		
		//CMP_COL_ALARM_ENABLED_ON_AOI
		bAlarm = ComponentPtr->GetComponentDefectAlarmEnableOnAOI();
		if ( true == bAlarm ) 
		{	str = _T(""); }
		else
		{	str = _T("N"); }
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//CMP_COL_ALARM_ENABLED_ON_ARS
		bAlarm = ComponentPtr->GetComponentDefectAlarmEnableOnARS();
		if ( true == bAlarm ) 
		{	str = _T(""); }
		else
		{	str = _T("N"); }
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//CMP_COL_ALARM_FROM_MODE
		str = AOIDataDefine.GetDefectParamFromText(ComponentPtr->GetComponentDefectAlarmFromModeAOI());
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//CMP_COL_DEFECT_BEGIN
		nValue = 0;
		for ( j=0; j<WndDefectIDCount; j++ )
		{
			WND_DEFECT_ID DefectID=WndDefectIDList[j];
			if ( DEFECT_FROM_AOI == AlarmFromMode )
			{	nValue = DefectItemAOI.GetItemCount(DefectID); }
			if ( DEFECT_FROM_ARS == AlarmFromMode )
			{	nValue = DefectItemARS.GetItemCount(DefectID); }
			if ( 0 == nValue )
			{	str = _T(""); }
			else
			{	str = _T("Y"); }
			ListCtrl.SetItemText(nItem, nSubItem, str);
			nSubItem ++;
		}		
		//----
		nItem ++;
	}	
	ListCtrl.SetRedraw(TRUE);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CComponentDefectAlarmListWnd::ClearComponentListWnd()
{
	CThisListCtrl_67 &ListCtrl = m_ComponentListWnd;
	m_StopComponentListBeSelected = true;	
	m_ComponentList.clear();
	JetAPI::ClearListCtrl(ListCtrl, FALSE);	
	m_StopComponentListBeSelected = false;
	CWnd::SetDlgItemText(COMDEFECTLIST_INFO_EDIT, _T(""));
	//CWnd::SetDlgItemText(COMLIST_SEL_COMPONENT_EDIT, _T(""));
	//CWnd::SetDlgItemText(COMLIST_SEL_PART_NUMBER_EDIT, _T(""));
	//CWnd::SetDlgItemText(COMLIST_SEL_MODEL_NAME_EDIT, _T(""));
	//CWnd::SetDlgItemText(COMLIST_SEL_NOZZLE_EDIT, _T(""));	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CComponentDefectAlarmListWnd::BuildComponentListWndHeader()
{
	CString str;
	int   nCol = 0;
	int width  = 64;
	int width2 = 64;
	RECT      Rect;	
	const int nCols = 24;
	const int Align = LVCFMT_LEFT;//LVCFMT_CENTER;LVCFMT_RIGHT, LVCFMT_LEFT
	const std::vector<WND_DEFECT_ID> &WndDefectIDList=GetWndDefectIDList();
	const size_t WndDefectIDCount=WndDefectIDList.size();
	{
		CThisListCtrl_67 &ListCtrl = m_ComponentListWnd;		

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
		
		str = _T("Component");
		str = AOIDataDefine.GetComponentText();
		ListCtrl.InsertColumn(nCol, str, Align, width2);
		nCol ++;		
		
		str = _T("Alarm(On AOI)");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width);
		nCol ++;

		str = _T("Alarm(On ARS)");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width);
		nCol ++;

		str = _T("Use");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width);
		nCol ++;

		for ( size_t i=0; i<WndDefectIDCount; i++ )
		{
			WND_DEFECT_ID WndDefectID=WndDefectIDList[i];
			str = AOIDataDefine.GetWndDefectIDText(WndDefectID);
			ListCtrl.InsertColumn(nCol, str, Align, width);
			nCol ++;
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CComponentDefectAlarmListWnd::BuildWndDefectIDList()
{	
	CAOIProject *ProjectPtr = GetActiveProject();
	m_WndDefectIDList = AOIDataCollect.GetWndDefectIDList();
	const size_t WndDefectIDCount=m_WndDefectIDList.size();
	if ( NULL == ProjectPtr )
	{	return ; }

	CWndDefectItem DefectItem;
	CAOIComponent *ComponentPtr=NULL;
	std::vector<CAOIComponent*> ComponentList;	
	const size_t ComponentCount=ProjectPtr->GetProjectComponentCount();	
	for ( size_t i=0; i<ComponentCount; i++ )
	{
		ComponentPtr=ProjectPtr->GetProjectComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }
		const CWndDefectItem &DefectItemAOI=ComponentPtr->GetComponentDefectItemAlarmAOI();
		const CWndDefectItem &DefectItemARS=ComponentPtr->GetComponentDefectItemAlarmARS();
		DefectItem.AddWndDefectItemCount(DefectItemAOI);
		DefectItem.AddWndDefectItemCount(DefectItemARS);		
	}


	WND_DEFECT_ID WndDefectID;
	std::vector<WND_DEFECT_ID> List;
	for ( size_t i=0; i<WndDefectIDCount; i++ )
	{
		WndDefectID = m_WndDefectIDList[i];
		if ( 0 == DefectItem.GetItemCount(WndDefectID) ) { continue; }
		List.push_back(WndDefectID);
	}
	if ( 0 == List.size() ) { return; }
	m_WndDefectIDList = List;
	return;
}
//-------------------------------------------------------------------------------------//
const std::vector<WND_DEFECT_ID>& CComponentDefectAlarmListWnd::GetWndDefectIDList() const
{
	return m_WndDefectIDList;
}
//-------------------------------------------------------------------------------------//
DEFECT_FROM_MODE CComponentDefectAlarmListWnd::GetAlarmFromMode() const
{
	return m_AlarmFromMode;
}
//-------------------------------------------------------------------------------------//
void CComponentDefectAlarmListWnd::SetAlarmFromMode(DEFECT_FROM_MODE val)
{
	m_AlarmFromMode = val;
}
//-------------------------------------------------------------------------------------//
void CComponentDefectAlarmListWnd::SetComponentListColID(int val)
{
	m_ComponentColID = val;
}
//-------------------------------------------------------------------------------------//
int CComponentDefectAlarmListWnd::GetComponentListColD() const
{
	return m_ComponentColID;
}
//-------------------------------------------------------------------------------------//
void CComponentDefectAlarmListWnd::SetComponentListSortMode(int val)
{
	m_ComponentListSortMode = val;
}
//-------------------------------------------------------------------------------------//
int CComponentDefectAlarmListWnd::GetComponentListSortMode() const
{
	return m_ComponentListSortMode;
}
//-------------------------------------------------------------------------------------//
int CComponentDefectAlarmListWnd::CompareComponentItem(size_t index1, size_t index2)
{
	CThisListCtrl_67 &ListCtrl = m_ComponentListWnd;	
	const int ColID = GetComponentListColD();
	const size_t ComponentCount = m_ComponentList.size();
	if ( index1>=ComponentCount || index2>=ComponentCount )
	{	return 0; }
	int          Res=0;	
	wchar_t     *wsPtr1=NULL;
	wchar_t     *wsPtr2=NULL;
	double       dVal1=0.0;
	double       dVal2=0.0;
	unsigned int uVal1=0;
	unsigned int uVal2=0;	
	CAOIComponent *CPtr1=(m_ComponentList[index1]);
	CAOIComponent *CPtr2=(m_ComponentList[index2]);
	CAOIModel     *MPtr1=CPtr1->GetComponentModelPtr();
	CAOIModel     *MPtr2=CPtr2->GetComponentModelPtr();
	if ( NULL==MPtr1 || NULL==MPtr2 ) { return 0; }
	const std::vector<WND_DEFECT_ID> &WndDefectIDList=GetWndDefectIDList();
	const size_t WndDefectIDCount=WndDefectIDList.size();

	switch ( ColID )
	{	
	case CMP_COL_INDEX:
		if ( index1 > index2 ) { Res = 1; }
		else if ( index1 < index2 ) { Res = -1; }
		else {	Res=0; }
		break;
	case CMP_COL_PANEL:
		uVal1 = CPtr1->GetComponentPanelIndex_Project();
		uVal2 = CPtr2->GetComponentPanelIndex_Project();
		if ( uVal1 > uVal2 ) { Res = 1; }
		else if ( uVal1 < uVal2 ) { Res = -1; }
		else {	Res=0; }
		break;
	case CMP_COL_BOARD:
		uVal1 = CPtr1->GetComponentBoardIndex_Project();
		uVal2 = CPtr2->GetComponentBoardIndex_Project();
		if ( uVal1 > uVal2 ) { Res = 1; }
		else if ( uVal1 < uVal2 ) { Res = -1; }
		else {	Res=0; }
		break;
	case CMP_COL_COMPONENT:
		wsPtr1= (wchar_t*)CPtr1->GetComponentName();
		wsPtr2= (wchar_t*)CPtr2->GetComponentName();
		Res = ::wcscmp(wsPtr1, wsPtr2);		
		break;
	case CMP_COL_ALARM_ENABLED_ON_AOI:
		uVal1 = CPtr1->GetComponentDefectAlarmEnableOnAOI();
		uVal2 = CPtr2->GetComponentDefectAlarmEnableOnAOI();
		if ( uVal1 > uVal2 ) { Res = 1; }
		else if ( uVal1 < uVal2 ) { Res = -1; }
		else {	Res=0; }
		break;
	case CMP_COL_ALARM_ENABLED_ON_ARS:
		uVal1 = CPtr1->GetComponentDefectAlarmEnableOnARS();
		uVal2 = CPtr2->GetComponentDefectAlarmEnableOnARS();
		if ( uVal1 > uVal2 ) { Res = 1; }
		else if ( uVal1 < uVal2 ) { Res = -1; }
		else {	Res=0; }
		break;
	case CMP_COL_ALARM_FROM_MODE:
		uVal1 = CPtr1->GetComponentDefectAlarmFromModeAOI();
		uVal2 = CPtr2->GetComponentDefectAlarmFromModeAOI();
		if ( uVal1 > uVal2 ) { Res = 1; }
		else if ( uVal1 < uVal2 ) { Res = -1; }
		else {	Res=0; }
		break;	
	default:
		if ( ColID < (CMP_COL_DEFECT_BEGIN+WndDefectIDCount) )
		{
			const size_t DefectIdx=ColID-CMP_COL_DEFECT_BEGIN;
			if ( DefectIdx < WndDefectIDCount )
			{
				WND_DEFECT_ID DefectID=WndDefectIDList[DefectIdx];
				uVal1 = CPtr1->GetComponentDefectItemAlarmAOI().GetItemCount(DefectID);
				uVal2 = CPtr2->GetComponentDefectItemAlarmAOI().GetItemCount(DefectID);
				if ( uVal1 > uVal2 ) { Res = 1; }
				else if ( uVal1 < uVal2 ) { Res = -1; }
				else {	Res=0; }
			}
			else
			{	Res = 0; }
		}
		else
		{	Res = 0; }
		break;
	}
	int SortMode = GetComponentListSortMode();
	Res = Res*SortMode;	
	return Res;	
}
//-------------------------------------------------------------------------------------//