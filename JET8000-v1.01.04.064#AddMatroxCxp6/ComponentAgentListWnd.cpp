// ComponentAgentListWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "ComponentAgentListWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CComponentAgentListWnd dialog
//-------------------------------------------------------------------------------------//
CComponentAgentListWnd::CComponentAgentListWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CComponentAgentListWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CComponentAgentListWnd)	
	m_MasterPtr = NULL;
	m_ResultPtr = NULL;	
	//}}AFX_DATA_INIT
}
//-------------------------------------------------------------------------------------//
void CComponentAgentListWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CComponentAgentListWnd)
	DDX_Control(pDX, AGN_AGENT_LIST_WND, m_AgentListWnd);	
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CComponentAgentListWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CComponentAgentListWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(LVN_ITEMCHANGED, AGN_AGENT_LIST_WND, OnItemchangedAgentListWnd)
	ON_NOTIFY(NM_DBLCLK, AGN_AGENT_LIST_WND, OnDblclkAgentListWnd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CComponentAgentListWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CComponentAgentListWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	JetAPI::InitialListCtrl(m_AgentListWnd);	
	BuildAgentWndHeader();
	SwitchMultiLanguage();

	BuildAgentListWnd();
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	m_AgentListWnd.SetFocus();	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CComponentAgentListWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CComponentAgentListWnd::OnSize(UINT nType, int cx, int cy) 
{
	CBaseDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if ( CWnd::GetSafeHwnd() == NULL ) { return; }

	CWnd     *WndPtr=NULL;
	const int MarginW=4;
	const int MarginH=4;

	if ( m_AgentListWnd.GetSafeHwnd() != NULL )
	{
		RECT WndRect={0};
		m_AgentListWnd.GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		WndRect.right = cx-MarginW;
		WndRect.bottom = cy-MarginH;
		m_AgentListWnd.MoveWindow(&WndRect);
	}	
}
//-------------------------------------------------------------------------------------//
void CComponentAgentListWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CBaseDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CComponentAgentListWnd::OnOK() 
{
	// TODO: Add extra validation here		
	CAOIComponent *MasterPtr=m_MasterPtr;	
	if ( NULL != MasterPtr )
	{
		CAOIProject *ProjectPtr = MasterPtr->GetComponentProjectPtr();	
		CAOIComponent *ResultPtr=MasterPtr->GetComponentResultPtr();
		if ( NULL!=ProjectPtr && NULL!=ResultPtr )
		{
			bool bSelected=true;
			ProjectPtr->SelectProjectAllComponents(false);
			ResultPtr->SetComponentSelected(bSelected);
			ResultPtr->ChangeComponentSelected(bSelected);
			ProjectPtr->SetProjectActiveComponentPtr(ResultPtr);
		}
	}
	CBaseDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CComponentAgentListWnd::OnCancel() 
{
	// TODO: Add extra cleanup here	
	CAOIComponent *ResultPtr=m_ResultPtr;
	if ( NULL!=m_MasterPtr && NULL!=ResultPtr )
	{	m_MasterPtr->SetComponentResultPtr(ResultPtr);	}
	CBaseDialog::OnCancel();
}
//-------------------------------------------------------------------------------------//
void CComponentAgentListWnd::SetComponentPtr(CAOIComponent *Ptr)
{
	if ( NULL == Ptr ) { return; }	
	if ( Ptr->CheckComponentIsAgent() )
	{
		Ptr = Ptr->GetComponentMasterPtr();
		if ( NULL == Ptr ) { return; }	
	}
	
	TComponentNode AgentNode;
	CAOIProject   *ProjectPtr=Ptr->GetComponentProjectPtr();
	CAOIComponent *ResultPtr=Ptr->GetComponentResultPtr();
	const size_t AgentCount=Ptr->GetComponentAgentCount();
	
	m_MasterPtr=Ptr;
	m_ResultPtr=ResultPtr;

	m_AgentList.clear();
	Ptr->ConvertToComponentNode(AgentNode);
	AgentNode.nTempInt = -1;
	if ( ResultPtr == Ptr )
	{	AgentNode.bFocused = true; }
	else
	{	AgentNode.bFocused = false; }
	m_AgentList.push_back(AgentNode);
	for ( size_t i=0; i<AgentCount; i++ )
	{
		CAOIComponent *AgentPtr=Ptr->GetComponentAgentPtr(i, false);
		if ( NULL == AgentPtr ) { continue; }
		AgentPtr->ConvertToComponentNode(AgentNode);
		AgentNode.nTempInt = (int)(i);
		if ( ResultPtr == AgentPtr )
		{	AgentNode.bFocused = true; }
		else
		{	AgentNode.bFocused = false; }
		m_AgentList.push_back(AgentNode);
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CComponentAgentListWnd::SwitchMultiLanguage()
{
	//---------------------------------------------------------------------------------//	
	SetMultiLanauage(LoadIDAndName(IDD_COMPONENT_AGENT_LIST_WND), false);
	//---------------------------------------------------------------------------------//	
	SetMultiLanauage(LoadIDAndName(IDOK));
	SetMultiLanauage(LoadIDAndName(IDCANCEL));
	//---------------------------------------------------------------------------------//	
	SetMultiLanauage(LoadIDAndName(AGN_COMPONENT_NAME_LABEL));
	SetMultiLanauage(LoadIDAndName(AGN_PART_NUMBER_LABEL));
	SetMultiLanauage(LoadIDAndName(AGN_MODEL_NAME_LABEL));	
	//---------------------------------------------------------------------------------//	
}
//-------------------------------------------------------------------------------------//
bool CComponentAgentListWnd::SetMultiLanauage(UINT ID, LPCTSTR Text, bool bCtrlID)
{
	LPCTSTR Section=_T("IDD_COMPONENT_AGENT_LIST_WND");		
	if ( AOIDataCollect.SwitchMultiLanguageWnd(this, Section, ID, Text, bCtrlID) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
CString CComponentAgentListWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_COMPONENT_AGENT_LIST_WND");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
bool CComponentAgentListWnd::ClearAgentListWnd()
{
	CThisListCtrl_69 &ListCtrl = m_AgentListWnd;	
	JetAPI::ClearListCtrl(ListCtrl, FALSE);		
	//CWnd::SetDlgItemText(COMLIST_INFO_EDIT, _T(""));
	CWnd::SetDlgItemText(AGN_MODEL_NAME_EDIT, _T(""));
	CWnd::SetDlgItemText(AGN_PART_NUMBER_EDIT, _T(""));
	CWnd::SetDlgItemText(AGN_COMPONENT_NAME_EDIT, _T(""));	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CComponentAgentListWnd::BuildAgentListWnd()
{
	ClearAgentListWnd();	
	CThisListCtrl_69 &ListCtrl = m_AgentListWnd;	
	const std::vector<TComponentNode> &AgentList = m_AgentList;

	CString        str;	
	size_t         i=0;
	int            nItem=0, nSubItem=0;		
	const size_t AgentCount = AgentList.size();

	ListCtrl.SetRedraw(FALSE);	
	for ( i=0; i<AgentCount; i++ )
	{
		const TComponentNode &AgentRef=AgentList[i];
		
		nSubItem = 0;
		str.Format(_T("%d"), i+1);
		ListCtrl.InsertItem(nItem, str);
		ListCtrl.SetItemData(nItem, (DWORD_PTR)i);		

		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;
		
		str = AgentRef.sPartNumber;
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		str = AgentRef.sModelName;
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		str = AOIDataDefine.GetModelTypeText((MODEL_TYPE)(AgentRef.dwModelType));
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		str = AOIDataDefine.GetResultIDText((RESULT_ID)(AgentRef.dwResultID));
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		if ( true == AgentRef.bFocused )
		{	str = _T("Y"); }
		else
		{	str = _T("");	}
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		nItem ++;
	}	
	ListCtrl.SetRedraw(TRUE);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CComponentAgentListWnd::UpdateAgentListWnd()
{
	CThisListCtrl_69 &ListCtrl = m_AgentListWnd;		
	
	size_t         i=0, j=0;
	CString        str;	
	unsigned int   AgentIndex=0;
	int            nItem=0;
	int            nSubItem=0;	
	int            nGroupID=0;
	bool           bAlarm=false;
	double         dCalcTime=0.0;
	RESULT_ID      ResultID;
	MODEL_TYPE     ModelType;
	DISTRICT_ID    DistrictID;
	CAOIModel     *ModelPtr = NULL;	
	CAOIComponent *ComponentPtr = NULL;
	const int      ItemCount = ListCtrl.GetItemCount();
	const size_t   AgentCount = m_AgentList.size();

	ListCtrl.SetRedraw(FALSE);	
	for ( i=0; i<ItemCount; i++ )
	{	
		AgentIndex = ListCtrl.GetItemData(i);
		if ( AgentIndex >= AgentCount ) { continue; }
		const TComponentNode &AgentRef = m_AgentList[AgentIndex];		
		
		nItem = i;
		nSubItem = 0;		
		//str.Format(_T("%d"), i+1);		
		//ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;
		
		str = AgentRef.sPartNumber;
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		str = AgentRef.sModelName;
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		str = AOIDataDefine.GetModelTypeText((MODEL_TYPE)(AgentRef.dwModelType));
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		str = AOIDataDefine.GetResultIDText((RESULT_ID)(AgentRef.dwResultID));
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		if ( true == AgentRef.bFocused )
		{	str = _T("Y"); }
		else
		{	str = _T("");	}
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;
	}	
	ListCtrl.SetRedraw(TRUE);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CComponentAgentListWnd::BuildAgentWndHeader()
{
	CString str;
	int   nCol = 0;
	int width  = 64;
	int width2 = 64;
	RECT      Rect;	
	const int nCols = 4;
	const int Align = LVCFMT_LEFT;//LVCFMT_CENTER;LVCFMT_RIGHT, LVCFMT_LEFT

	{
		CThisListCtrl_69 &ListCtrl = m_AgentListWnd;		

		ListCtrl.GetClientRect(&Rect);
		width = 64;
		width2 = (Rect.right-Rect.left-width-64)/nCols;

		str = _T("Index");
		str = AOIDataDefine.GetIndexText();
		ListCtrl.InsertColumn(nCol, str, Align, width);		
		
		nCol ++;
		
		str = _T("Part Number");
		str = AOIDataDefine.GetPartNumberText();
		ListCtrl.InsertColumn(nCol, str, Align, width2);
		nCol ++;		
		
		str = _T("Model Name");
		str = AOIDataDefine.GetModelText();
		ListCtrl.InsertColumn(nCol, str, Align, width2);
		nCol ++;

		str = _T("Model Type");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width2);
		nCol ++;

		str = _T("Result ID");
		str = AOIDataDefine.GetResultText();
		ListCtrl.InsertColumn(nCol, str, Align, width2);
		nCol ++;

		str = _T("Actived");
		//str = LoadMultiLanguageString(str, str);
		str = AOIDataDefine.GetEnableText();
		ListCtrl.InsertColumn(nCol, str, Align, width);
		nCol ++;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CComponentAgentListWnd::UpdateAgentInfo(const TComponentNode &Agent)
{
	CWnd::SetDlgItemText(AGN_MODEL_NAME_EDIT, Agent.sModelName);	
	CWnd::SetDlgItemText(AGN_PART_NUMBER_EDIT, Agent.sPartNumber);
	CWnd::SetDlgItemText(AGN_COMPONENT_NAME_EDIT, Agent.sComponentName);	
	return;
}
//-------------------------------------------------------------------------------------//
void CComponentAgentListWnd::OnItemchangedAgentListWnd(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) 
	{	return;	}
	DWORD Res = pNMListView->uNewState&LVIS_SELECTED;
	if ( Res == 0 ) { return; }	
	Res = pNMListView->uNewState&LVIS_FOCUSED;
	if ( Res == 0 ) { return; }	
	const size_t SelIndex = m_AgentListWnd.GetItemData(nItem);
	const size_t AgentCount = m_AgentList.size();
	if ( SelIndex > AgentCount ) { return; }
	const TComponentNode &AgentRef = m_AgentList[SelIndex];	
	UpdateAgentInfo(AgentRef);
}
//-------------------------------------------------------------------------------------//
void CComponentAgentListWnd::OnDblclkAgentListWnd(NMHDR* pNMHDR, LRESULT* pResult)
{	
	NMITEMACTIVATE *pNMListView = (NMITEMACTIVATE*)pNMHDR;		
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;	
	CThisListCtrl_69 &ListCtrl=m_AgentListWnd;
	const size_t SelIndex = ListCtrl.GetItemData(nItem);
	const size_t AgentCount = m_AgentList.size();
	if ( SelIndex > AgentCount ) { return; }
	TComponentNode &MasterRef=m_AgentList[0];
	TComponentNode &AgentRef = m_AgentList[SelIndex];
	CAOIComponent *AgentPtr = (CAOIComponent*)(AgentRef.pCompoennt);
	CAOIComponent *MasterPtr = (CAOIComponent*)(MasterRef.pCompoennt);
	if ( NULL!=MasterPtr && NULL!=AgentPtr )
	{
		MasterPtr->SetComponentResultPtr(AgentPtr);
		for ( size_t i=0; i<AgentCount; i++ )
		{
			TComponentNode &Ref=m_AgentList[i];
			Ref.bFocused = false;
		}
		AgentRef.bFocused = true;		
		UpdateAgentListWnd();
	}
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//