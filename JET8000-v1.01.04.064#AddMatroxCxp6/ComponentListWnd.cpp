// ComponentListWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "ComponentListWnd.h"
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
#define    CMP_COL_PART_NUMBER          4
#define    CMP_COL_NOZZLE               5
#define    CMP_COL_BYPASS               6
#define    CMP_COL_BYPASS_3D            7
#define    CMP_COL_XBOARD_UNIT          8
#define    CMP_COL_GROUP_ID             9
#define    CMP_COL_GROUP_ORG           10
#define    CMP_COL_DISTRICT_ID         11
#define    CMP_COL_SELF_FIELD          12
#define    CMP_COL_SAVE_WND_LIST       13
#define    CMP_COL_MODEL               14
#define    CMP_COL_MODEL_TYPE          15
#define    CMP_COL_MODEL_ISOLATED      16
#define    CMP_COL_MODEL_LAND_COUNT    17
#define    CMP_COL_MODEL_WND_COUNT     18
#define    CMP_COL_RESULT_ID           19
#define    CMP_COL_TOTAL_NG_ALARM      20
#define    CMP_COL_CONTINUE_NG_ALARM   21
#define    CMP_COL_CALC_TIME           22
#define    CMP_COL_NOISE_FILTER_ID     23
#define    CMP_COL_SUB_RGN_COUNT       24
#define    CMP_COL_DIF_OFFSET_X        25
#define    CMP_COL_DIF_OFFSET_Y        26
#define    CMP_COL_DIF_SKEW_ANGLE      27
#define    CMP_COL_DIF_BODY_HEIGHT     28
#define    CMP_COL_GRR_SIGMA_ITEM_IDX  29
//-------------------------------------------------------------------------------------//
int CALLBACK ComponentListCompareFn(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
int CALLBACK ComponentListCompareFn(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	if ( NULL == lParamSort ) { return 0; }
	CComponentListWnd* pComponentListWnd = (CComponentListWnd*)lParamSort;
	return pComponentListWnd->CompareComponentItem(lParam1, lParam2);
}
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CComponentListWnd dialog
//-------------------------------------------------------------------------------------//
CComponentListWnd::CComponentListWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CComponentListWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CComponentListWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nItemAct = -1;
	m_nSubItemAct = -1;
	m_ProjectPtr = NULL;
	m_ComponentPtr = NULL;
	m_ComponentColID = CMP_COL_INDEX;
	m_ComponentListSortMode = SORT_ASCEND;
	m_StopComponentListBeSelected = false;
}
//-------------------------------------------------------------------------------------//
void CComponentListWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CComponentListWnd)	
	DDX_Control(pDX, COMLIST_PARAM_EDIT, m_EditCtrl);	
	DDX_Control(pDX, COMLIST_COMPONENT_LIST_WND, m_ComponentListWnd);	
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CComponentListWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CComponentListWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	ON_NOTIFY(LVN_COLUMNCLICK, COMLIST_COMPONENT_LIST_WND, OnColumnclickComponentListWnd)
	ON_WM_GETMINMAXINFO()
	ON_NOTIFY(LVN_ITEMCHANGED, COMLIST_COMPONENT_LIST_WND, OnItemchangedComponentListWnd)
	ON_NOTIFY(NM_DBLCLK, COMLIST_COMPONENT_LIST_WND, OnDblclkComponentListWnd)
	ON_EN_KILLFOCUS(COMLIST_PARAM_EDIT, OnKillfocusParamEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CComponentListWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CComponentListWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//CWnd::ShowWindow(SW_SHOWMAXIMIZED);
	JetAPI::InitialListCtrl(m_ComponentListWnd);	
	BuildComponentListWndHeader();
	SwitchMultiLanguage();

	BuildComponentListWnd();
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	m_ComponentListWnd.SetFocus();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CComponentListWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CComponentListWnd::OnSize(UINT nType, int cx, int cy) 
{
	CBaseDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if ( CWnd::GetSafeHwnd() == NULL ) { return; }

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
void CComponentListWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CBaseDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CComponentListWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CBaseDialog::OnPaint() for painting messages
}
//-------------------------------------------------------------------------------------//
void CComponentListWnd::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default	
	CBaseDialog::OnGetMinMaxInfo(lpMMI);
	lpMMI->ptMinTrackSize.x = 1280;
	lpMMI->ptMinTrackSize.y = 200;
}
//-------------------------------------------------------------------------------------//
void CComponentListWnd::SetProjectPtr(CAOIProject *Ptr)
{
	m_ProjectPtr = Ptr;		
	m_ComponentPtr = NULL;
	SetComponentListSortMode(1);	
	SetComponentListColID(CMP_COL_INDEX);
}
//-------------------------------------------------------------------------------------//
inline CAOIProject* CComponentListWnd::GetActiveProject()
{
	return m_ProjectPtr;
}
//-------------------------------------------------------------------------------------//
void CComponentListWnd::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_COMPONENT_LIST_WND");
	//---------------------------------------------------------------------------------//
	WndID = IDD_COMPONENT_LIST_WND;
	WndKey = _T("IDD_COMPONENT_LIST_WND");
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
	WndID = COMLIST_SEL_COMPONENT_LABEL;
	WndKey = _T("COMLIST_SEL_COMPONENT_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = COMLIST_SEL_PART_NUMBER_LABEL;
	WndKey = _T("COMLIST_SEL_PART_NUMBER_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = COMLIST_SEL_MODEL_NAME_LABEL;
	WndKey = _T("COMLIST_SEL_MODEL_NAME_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = COMLIST_SEL_NOZZLE_LABEL;
	WndKey = _T("COMLIST_SEL_NOZZLE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
}
//-------------------------------------------------------------------------------------//
CString CComponentListWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_COMPONENT_LIST_WND");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
bool CComponentListWnd::BuildComponentListWnd()
{
	CThisListCtrl_07 &ListCtrl = m_ComponentListWnd;
	CAOIProject *ProjectPtr = GetActiveProject();
	ClearComponentListWnd();
	if ( NULL == ProjectPtr )
	{	return true; }
	
	size_t         i=0, j=0;
	CString        str;
	unsigned int   WndCount=0;
	unsigned int   LandCount=0;
	unsigned int   PanelIndex=0;
	unsigned int   BoardIndex=0;
	int            nItem=0;
	int            nSubItem=0;	
	int            nGroupID=0;
	bool           bAlarm=false;
	double         dCalcTime=0.0;
	RESULT_ID      ResultID;
	DISTRICT_ID    DistrictID;
	MODEL_TYPE     ModelType;	
	CAOIModel     *ModelPtr = NULL;	
	CAOIComponent *ComponentPtr = NULL;	
	const size_t ComponentCount = ProjectPtr->GetProjectComponentCount();

	WndCount = 0;
	LandCount = 0;
	ListCtrl.SetRedraw(FALSE);	
	m_StopComponentListBeSelected = true;
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = ProjectPtr->GetProjectComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->CheckComponentIsAgent() == true ) { continue; }
		ComponentPtr = ComponentPtr->GetComponentResultPtr();

		ModelPtr = ComponentPtr->GetComponentModelPtr();
		if ( NULL == ModelPtr ) { continue; }
		const int GrrSigmaItemIdx=ComponentPtr->GetComponentGrrSigmaItemIdx();
		const TSigmaItem &SigmaItem=ComponentPtr->GetComponentGrrSigmaItem();			

		WndCount += ModelPtr->GetModelWndCount();
		LandCount += ModelPtr->GetModelLandCount();

		PanelIndex = ComponentPtr->GetComponentPanelIndex_Project();
		BoardIndex = ComponentPtr->GetComponentBoardIndex_Panel();

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

		//CMP_COL_PART_NUMBER
		str = ComponentPtr->GetComponentPartNumber();
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//CMP_COL_NOZZLE
		str = ComponentPtr->GetComponentNozzleName();
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//CMP_COL_BYPASS
		if ( ComponentPtr->GetComponentBypassed() == true ) 
		{	str = _T("Y"); }
		else
		{	str = _T(""); }
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//CMP_COL_BYPASS_3D
		if ( ComponentPtr->GetComponentBypass3D() == true ) 
		{	str = _T("Y"); }
		else
		{	str = _T(""); }
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//CMP_COL_XBOARD_UNIT
		if ( ComponentPtr->GetComponentXBoardUnit() == true ) 
		{	str = _T("Y"); }
		else
		{	str = _T(""); }
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//CMP_COL_GROUP_ID
		nGroupID = ComponentPtr->GetComponentGroupID();
		if ( nGroupID > 0 )
		{	str.Format(_T("%d"), nGroupID); }
		else
		{	str = _T(""); }
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//CMP_COL_GROUP_ORG
		if ( ComponentPtr->GetComponentGroupOrg() != false ) 
		{	str = _T("Y"); }
		else
		{	str = _T(""); }
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;		

		//CMP_COL_DISTRICT_ID
		DistrictID = ComponentPtr->GetComponentDistrictID();
		str = AOIDataDefine.GetDistrictIDText(DistrictID);		
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;	
		
		//CMP_COL_SELF_FIELD
		if ( ComponentPtr->GetComponentSelfFieldEnabled() == true ) 
		{	str = _T("Y"); }
		else
		{	str = _T(""); }
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;	
		
		//CMP_COL_SAVE_WND_LIST
		if ( ComponentPtr->GetComponentSaveWndList() )
		{	str = _T("Y");	}
		else
		{	str = _T("");	}
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//CMP_COL_MODEL
		str = ComponentPtr->GetComponentModelName();
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//CMP_COL_MODEL_TYPE
		ModelType = ModelPtr->GetModelType();
		str = AOIDataDefine.GetModelTypeText(ModelType);		
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//CMP_COL_MODEL_ISOLATED
		if ( ComponentPtr->GetComponentModelIsolated() == true ) 
		{	str = _T("Y"); }
		else
		{	str = _T(""); }
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//CMP_COL_MODEL_LAND_COUNT
		str.Format(_T("%d"), ModelPtr->GetModelLandCount());
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//CMP_COL_MODEL_WND_COUNT
		str.Format(_T("%d"), ModelPtr->GetModelWndCount());
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;
		
		//CMP_COL_RESULT_ID
		ResultID = ComponentPtr->GetComponentResultID_AOI();
		str = AOIDataDefine.GetResultIDText(ResultID);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//CMP_COL_TOTAL_NG_ALARM
		bAlarm = ComponentPtr->GetComponentTotalNGCountEnable();
		if ( true == bAlarm ) 
		{	str = _T(""); }
		else
		{	str = _T("N"); }
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//CMP_COL_CONTINUE_NG_ALARM
		bAlarm = ComponentPtr->GetComponentContinueNGCountEnable();
		if ( true == bAlarm ) 
		{	str = _T(""); }
		else
		{	str = _T("N"); }
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//CMP_COL_CALC_TIME
		dCalcTime = ComponentPtr->GetComponentModelPtr()->GetModelInspectedTime();
		str.Format(_T("%.2f"), dCalcTime);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//CMP_COL_NOISE_FILTER_ID
		str.Format(_T("%d"), ComponentPtr->GetComponentSpaceNoiseFilterParam().DataFilterIndex);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//CMP_COL_SUB_RGN_COUNT
		if ( 0 == ComponentPtr->GetComponentSubRgnCount() )
		{	str = _T("");	}
		else
		{	str.Format(_T("%d"), ComponentPtr->GetComponentSubRgnCount());	}		
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//CMP_COL_DIF_OFFSET_X
		str.Format(_T("%.0f"), SigmaItem.OffsetX.Max-SigmaItem.OffsetX.Min);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//CMP_COL_DIF_OFFSET_Y
		str.Format(_T("%.0f"), SigmaItem.OffsetY.Max-SigmaItem.OffsetY.Min);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//CMP_COL_DIF_SKEW_ANGLE
		str.Format(_T("%.2f"), SigmaItem.SkewAngle.Max-SigmaItem.SkewAngle.Min);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//CMP_COL_DIF_BODY_HEIGHT
		str.Format(_T("%.0f"), SigmaItem.BodyHeight.Max-SigmaItem.BodyHeight.Min);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//CMP_COL_GRR_SIGMA_ITEM_IDX
		str.Format(_T("%d"), GrrSigmaItemIdx+1);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//----
		nItem ++;
	}
	m_StopComponentListBeSelected = false;
	ListCtrl.SetRedraw(TRUE);

	SetComponentListColID(CMP_COL_INDEX);
	SetComponentListSortMode(SORT_ASCEND);

	CString strWnd = AOIDataDefine.GetWndText();
	CString strLand = AOIDataDefine.GetLandText();
	CString strCount = AOIDataDefine.GetCountText();	
	CString strComponent = AOIDataDefine.GetComponentText();
	str.Format(_T("%s:%d, %s:%d, %s:%d"), strComponent, ListCtrl.GetItemCount(), strLand, LandCount, strWnd, WndCount);
	CWnd::SetDlgItemText(COMLIST_INFO_EDIT, str);

	str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("ComponentListCtrl.TXT"));
	JetAPI::SaveListCtrl(str, ListCtrl);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CComponentListWnd::UpdateComponentListWnd()
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }
	CThisListCtrl_07 &ListCtrl = m_ComponentListWnd;		
	
	size_t         i=0, j=0;
	CString        str;
	unsigned int   PanelIndex=0;
	unsigned int   BoardIndex=0;
	unsigned int   ComponentIndex=0;
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
	const size_t   ComponentCount = m_ComponentList.size();
	
	ListCtrl.SetRedraw(FALSE);	
	for ( i=0; i<ItemCount; i++ )
	{
		ComponentIndex = ListCtrl.GetItemData(i);
		if ( ComponentIndex >= ComponentCount ) { continue; }
		ComponentPtr = m_ComponentList[ComponentIndex];
		if ( NULL == ComponentPtr ) { continue; }
		ModelPtr = ComponentPtr->GetComponentModelPtr();
		if ( NULL == ModelPtr ) { continue; }
		const int GrrSigmaItemIdx=ComponentPtr->GetComponentGrrSigmaItemIdx();
		const TSigmaItem &SigmaItem=ComponentPtr->GetComponentGrrSigmaItem();	

		PanelIndex = ComponentPtr->GetComponentPanelIndex_Project();
		BoardIndex = ComponentPtr->GetComponentBoardIndex_Panel();

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

		//CMP_COL_PART_NUMBER
		str = ComponentPtr->GetComponentPartNumber();
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//CMP_COL_NOZZLE
		str = ComponentPtr->GetComponentNozzleName();
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;
		
		//CMP_COL_BYPASS
		if ( ComponentPtr->GetComponentBypassed() == true ) 
		{	str = _T("Y"); }
		else
		{	str = _T(""); }
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//CMP_COL_BYPASS_3D
		if ( ComponentPtr->GetComponentBypass3D() == true ) 
		{	str = _T("Y"); }
		else
		{	str = _T(""); }
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//CMP_COL_XBOARD_UNIT
		if ( ComponentPtr->GetComponentXBoardUnit() == true ) 
		{	str = _T("Y"); }
		else
		{	str = _T(""); }
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;		

		//CMP_COL_GROUP_ID
		nGroupID = ComponentPtr->GetComponentGroupID();
		if ( nGroupID > 0 )
		{	str.Format(_T("%d"), nGroupID); }
		else
		{	str = _T(""); }
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//CMP_COL_GROUP_ORG
		if ( ComponentPtr->GetComponentGroupOrg() != false ) 
		{	str = _T("Y"); }
		else
		{	str = _T(""); }
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;	

		//CMP_COL_DISTRICT_ID
		DistrictID = ComponentPtr->GetComponentDistrictID();
		str = AOIDataDefine.GetDistrictIDText(DistrictID);		
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;			

		//CMP_COL_SELF_FIELD
		if ( ComponentPtr->GetComponentSelfFieldEnabled() == true ) 
		{	str = _T("Y"); }
		else
		{	str = _T(""); }
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;	

		//CMP_COL_SAVE_WND_LIST
		if ( ComponentPtr->GetComponentSaveWndList() )
		{	str = _T("Y");	}
		else
		{	str = _T("");	}
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//CMP_COL_MODEL
		str = ComponentPtr->GetComponentModelName();
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//CMP_COL_MODEL_TYPE
		ModelType = ModelPtr->GetModelType();
		str = AOIDataDefine.GetModelTypeText(ModelType);		
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//CMP_COL_MODEL_ISOLATED
		if ( ComponentPtr->GetComponentModelIsolated() == true ) 
		{	str = _T("Y"); }
		else
		{	str = _T(""); }
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//CMP_COL_MODEL_LAND_COUNT
		str.Format(_T("%d"), ModelPtr->GetModelLandCount());
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//CMP_COL_MODEL_WND_COUNT
		str.Format(_T("%d"), ModelPtr->GetModelWndCount());
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;
		
		//CMP_COL_RESULT_ID
		ResultID = ComponentPtr->GetComponentResultID_AOI();
		str = AOIDataDefine.GetResultIDText(ResultID);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;
		
		//CMP_COL_TOTAL_NG_ALARM
		bAlarm = ComponentPtr->GetComponentTotalNGCountEnable();
		if ( true == bAlarm ) 
		{	str = _T(""); }
		else
		{	str = _T("N"); }
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//CMP_COL_CONTINUE_NG_ALARM
		bAlarm = ComponentPtr->GetComponentContinueNGCountEnable();
		if ( true == bAlarm ) 
		{	str = _T(""); }
		else
		{	str = _T("N"); }
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//CMP_COL_CALC_TIME
		dCalcTime = ComponentPtr->GetComponentModelPtr()->GetModelInspectedTime();
		str.Format(_T("%.2f"), dCalcTime);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//CMP_COL_NOISE_FILTER_ID
		str.Format(_T("%d"), ComponentPtr->GetComponentSpaceNoiseFilterParam().DataFilterIndex);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//CMP_COL_SUB_RGN_COUNT		
		if ( 0 == ComponentPtr->GetComponentSubRgnCount() )
		{	str = _T("");	}
		else
		{	str.Format(_T("%d"), ComponentPtr->GetComponentSubRgnCount());	}		
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//CMP_COL_DIF_OFFSET_X
		str.Format(_T("%.0f"), SigmaItem.OffsetX.Max-SigmaItem.OffsetX.Min);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//CMP_COL_DIF_OFFSET_Y
		str.Format(_T("%.0f"), SigmaItem.OffsetY.Max-SigmaItem.OffsetY.Min);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//CMP_COL_DIF_SKEW_ANGLE
		str.Format(_T("%.2f"), SigmaItem.SkewAngle.Max-SigmaItem.SkewAngle.Min);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//CMP_COL_DIF_BODY_HEIGHT
		str.Format(_T("%.0f"), SigmaItem.BodyHeight.Max-SigmaItem.BodyHeight.Min);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//CMP_COL_GRR_SIGMA_ITEM_IDX
		str.Format(_T("%d"), GrrSigmaItemIdx+1);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//----
		nItem ++;
	}	
	ListCtrl.SetRedraw(TRUE);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CComponentListWnd::ClearComponentListWnd()
{
	CThisListCtrl_07 &ListCtrl = m_ComponentListWnd;
	m_StopComponentListBeSelected = true;
	SetItemIndexAct(-1, -1);
	m_ComponentList.clear();
	JetAPI::ClearListCtrl(ListCtrl, FALSE);	
	m_StopComponentListBeSelected = false;
	CWnd::SetDlgItemText(COMLIST_INFO_EDIT, _T(""));
	CWnd::SetDlgItemText(COMLIST_SEL_COMPONENT_EDIT, _T(""));
	CWnd::SetDlgItemText(COMLIST_SEL_PART_NUMBER_EDIT, _T(""));
	CWnd::SetDlgItemText(COMLIST_SEL_MODEL_NAME_EDIT, _T(""));
	CWnd::SetDlgItemText(COMLIST_SEL_NOZZLE_EDIT, _T(""));	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CComponentListWnd::BuildComponentListWndHeader()
{
	CString str;
	int   nCol = 0;
	int width  = 64;
	int width2 = 64;
	RECT      Rect;	
	const int nCols = 18;
	const int Align = LVCFMT_LEFT;//LVCFMT_CENTER;LVCFMT_RIGHT, LVCFMT_LEFT

	{
		CThisListCtrl_07 &ListCtrl = m_ComponentListWnd;		

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
		
		str = _T("Part Number");
		str = AOIDataDefine.GetPartNumberText();
		ListCtrl.InsertColumn(nCol, str, Align, width2);
		nCol ++;

		str = _T("Nozzle Name");
		str = AOIDataDefine.GetNozzleNameText();
		ListCtrl.InsertColumn(nCol, str, Align, width2);
		nCol ++;

		str = _T("Bypassed");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width);
		nCol ++;

		str = _T("Bypass 3D");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width);
		nCol ++;

		str = _T("XBoard");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width);
		nCol ++;

		str = _T("Gropu ID");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width);
		nCol ++;

		str = _T("Gropu ORG");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width);
		nCol ++;

		str = _T("District");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width);
		nCol ++;

		str = _T("Field");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width);
		nCol ++;
		
		//CMP_COL_SAVE_WND_LIST
		str = _T("Save Wnd List");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width);
		nCol ++;

		str = _T("Model");
		str = AOIDataDefine.GetModelText();
		ListCtrl.InsertColumn(nCol, str, Align, width2);
		nCol ++;
		
		str = _T("Model Type");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width2);
		nCol ++;
		
		str = _T("Isolated");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width);
		nCol ++;
		
		str = _T("Land Count");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width);
		nCol ++;

		str = _T("Wnd Count");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width);
		nCol ++;

		str = _T("Result");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width);
		nCol ++;

		str = _T("Alarm(Total)");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width);
		nCol ++;

		str = _T("Alarm(Continue)");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width);
		nCol ++;

		str = _T("Calc Time");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width);
		nCol ++;

		str = _T("NF");
		//str = LoadMultiLanguageString(str, str);		
		ListCtrl.InsertColumn(nCol, str, Align, width);
		nCol ++;

		//CMP_COL_SUB_RGN_COUNT
		str = _T("Sub Rgn");
		//str = LoadMultiLanguageString(str, str);		
		ListCtrl.InsertColumn(nCol, str, Align, width);
		nCol ++;

		//CMP_COL_DIF_OFFSET_X
		str = _T("Dif X");
		//str = LoadMultiLanguageString(str, str);		
		ListCtrl.InsertColumn(nCol, str, Align, width);
		nCol ++;

		//CMP_COL_DIF_OFFSET_Y
		str = _T("Dif Y");
		//str = LoadMultiLanguageString(str, str);		
		ListCtrl.InsertColumn(nCol, str, Align, width);
		nCol ++;

		//CMP_COL_DIF_SKEW_ANGLE
		str = _T("Dif Skew");
		//str = LoadMultiLanguageString(str, str);		
		ListCtrl.InsertColumn(nCol, str, Align, width);
		nCol ++;

		//CMP_COL_DIF_BODY_HEIGHT
		str = _T("Dif Height");
		//str = LoadMultiLanguageString(str, str);		
		ListCtrl.InsertColumn(nCol, str, Align, width);
		nCol ++;

		//CMP_COL_GRR_SIGMA_ITEM_IDX
		str = _T("Dif Idx");
		//str = LoadMultiLanguageString(str, str);		
		ListCtrl.InsertColumn(nCol, str, Align, width);
		nCol ++;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CComponentListWnd::SetComponentListColID(int val)
{
	m_ComponentColID = val;
}
//-------------------------------------------------------------------------------------//
int CComponentListWnd::GetComponentListColD() const
{
	return m_ComponentColID;
}
//-------------------------------------------------------------------------------------//
void CComponentListWnd::SetComponentListSortMode(int val)
{
	m_ComponentListSortMode = val;
}
//-------------------------------------------------------------------------------------//
int CComponentListWnd::GetComponentListSortMode() const
{
	return m_ComponentListSortMode;
}
//-------------------------------------------------------------------------------------//
int CComponentListWnd::CompareComponentItem(size_t index1, size_t index2)
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return 0; }
	CThisListCtrl_07 &ListCtrl = m_ComponentListWnd;
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
	const TSigmaItem &SigmaItem1=CPtr1->GetComponentGrrSigmaItem();	
	const TSigmaItem &SigmaItem2=CPtr2->GetComponentGrrSigmaItem();	

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
	case CMP_COL_PART_NUMBER:
		wsPtr1= (wchar_t*)CPtr1->GetComponentPartNumber();
		wsPtr2= (wchar_t*)CPtr2->GetComponentPartNumber();
		Res = ::wcscmp(wsPtr1, wsPtr2);		
		break;
	case CMP_COL_NOZZLE:
		wsPtr1= (wchar_t*)CPtr1->GetComponentNozzleName();
		wsPtr2= (wchar_t*)CPtr2->GetComponentNozzleName();
		Res = ::wcscmp(wsPtr1, wsPtr2);		
		break;
	case CMP_COL_BYPASS:
		uVal1 = CPtr1->GetComponentBypassed();
		uVal2 = CPtr2->GetComponentBypassed();
		if ( uVal1 > uVal2 ) { Res = 1; }
		else if ( uVal1 < uVal2 ) { Res = -1; }
		else {	Res=0; }
		break;
	case CMP_COL_BYPASS_3D:
		uVal1 = CPtr1->GetComponentBypass3D();
		uVal2 = CPtr2->GetComponentBypass3D();
		if ( uVal1 > uVal2 ) { Res = 1; }
		else if ( uVal1 < uVal2 ) { Res = -1; }
		else {	Res=0; }
		break;
	case CMP_COL_XBOARD_UNIT:
		uVal1 = CPtr1->GetComponentXBoardUnit();
		uVal2 = CPtr2->GetComponentXBoardUnit();
		if ( uVal1 > uVal2 ) { Res = 1; }
		else if ( uVal1 < uVal2 ) { Res = -1; }
		else {	Res=0; }
		break;		
	case CMP_COL_GROUP_ID:
		uVal1 = CPtr1->GetComponentGroupID();
		uVal2 = CPtr2->GetComponentGroupID();
		if ( uVal1 > uVal2 ) { Res = 1; }
		else if ( uVal1 < uVal2 ) { Res = -1; }
		else {	Res=0; }
		break;
	case CMP_COL_GROUP_ORG:
		uVal1 = CPtr1->GetComponentGroupOrg();
		uVal2 = CPtr2->GetComponentGroupOrg();
		if ( uVal1 > uVal2 ) { Res = 1; }
		else if ( uVal1 < uVal2 ) { Res = -1; }
		else {	Res=0; }
		break;
	case CMP_COL_DISTRICT_ID:
		uVal1 = CPtr1->GetComponentDistrictID();
		uVal2 = CPtr2->GetComponentDistrictID();
		if ( uVal1 > uVal2 ) { Res = 1; }
		else if ( uVal1 < uVal2 ) { Res = -1; }
		else {	Res=0; }
		break;
	case CMP_COL_SELF_FIELD:
		uVal1 = CPtr1->GetComponentSelfFieldEnabled();
		uVal2 = CPtr2->GetComponentSelfFieldEnabled();
		if ( uVal1 > uVal2 ) { Res = 1; }
		else if ( uVal1 < uVal2 ) { Res = -1; }
		else {	Res=0; }
		break;
	case CMP_COL_SAVE_WND_LIST:
		uVal1 = CPtr1->GetComponentSaveWndList();
		uVal2 = CPtr2->GetComponentSaveWndList();
		if ( uVal1 > uVal2 ) { Res = 1; }
		else if ( uVal1 < uVal2 ) { Res = -1; }
		else {	Res=0; }
		break;
	case CMP_COL_MODEL:
		wsPtr1= (wchar_t*)CPtr1->GetComponentModelName();
		wsPtr2= (wchar_t*)CPtr2->GetComponentModelName();
		Res = ::wcscmp(wsPtr1, wsPtr2);		
		break;
	case CMP_COL_MODEL_TYPE:
		uVal1 = MPtr1->GetModelType();
		uVal2 = MPtr2->GetModelType();
		if ( uVal1 > uVal2 ) { Res = 1; }
		else if ( uVal1 < uVal2 ) { Res = -1; }
		else {	Res=0; }
		break;
	case CMP_COL_MODEL_ISOLATED:
		uVal1 = CPtr1->GetComponentModelIsolated();
		uVal2 = CPtr2->GetComponentModelIsolated();
		if ( uVal1 > uVal2 ) { Res = 1; }
		else if ( uVal1 < uVal2 ) { Res = -1; }
		else {	Res=0; }
		break;
	case CMP_COL_MODEL_LAND_COUNT:
		uVal1 = MPtr1->GetModelLandCount();
		uVal2 = MPtr2->GetModelLandCount();
		if ( uVal1 > uVal2 ) { Res = 1; }
		else if ( uVal1 < uVal2 ) { Res = -1; }
		else {	Res=0; }
		break;
	case CMP_COL_MODEL_WND_COUNT:
		uVal1 = MPtr1->GetModelWndCount();
		uVal2 = MPtr2->GetModelWndCount();
		if ( uVal1 > uVal2 ) { Res = 1; }
		else if ( uVal1 < uVal2 ) { Res = -1; }
		else {	Res=0; }
		break;
	case CMP_COL_RESULT_ID:
		uVal1 = CPtr1->GetComponentResultID_AOI();
		uVal2 = CPtr2->GetComponentResultID_AOI();
		if ( uVal1 > uVal2 ) { Res = 1; }
		else if ( uVal1 < uVal2 ) { Res = -1; }
		else {	Res=0; }
		break;
	case CMP_COL_TOTAL_NG_ALARM:
		uVal1 = CPtr1->GetComponentTotalNGCountEnable();
		uVal2 = CPtr2->GetComponentTotalNGCountEnable();
		if ( uVal1 > uVal2 ) { Res = 1; }
		else if ( uVal1 < uVal2 ) { Res = -1; }
		else {	Res=0; }
		break;
	case CMP_COL_CONTINUE_NG_ALARM:
		uVal1 = CPtr1->GetComponentContinueNGCountEnable();
		uVal2 = CPtr2->GetComponentContinueNGCountEnable();
		if ( uVal1 > uVal2 ) { Res = 1; }
		else if ( uVal1 < uVal2 ) { Res = -1; }
		else {	Res=0; }
		break;
	case CMP_COL_CALC_TIME:
		dVal1 = MPtr1->GetModelInspectedTime();
		dVal2 = MPtr2->GetModelInspectedTime();
		if ( dVal1 > dVal2 ) { Res = 1; }
		else if ( dVal1 < dVal2 ) { Res = -1; }
		else {	Res=0; }
		break;
	case CMP_COL_NOISE_FILTER_ID:
		uVal1 = CPtr1->GetComponentSpaceNoiseFilterParam().DataFilterIndex;
		uVal2 = CPtr2->GetComponentSpaceNoiseFilterParam().DataFilterIndex;
		if ( uVal1 > uVal2 ) { Res = 1; }
		else if ( uVal1 < uVal2 ) { Res = -1; }
		else {	Res=0; }
		break;
	case CMP_COL_SUB_RGN_COUNT:
		uVal1 = CPtr1->GetComponentSubRgnCount();
		uVal2 = CPtr2->GetComponentSubRgnCount();
		if ( uVal1 > uVal2 ) { Res = 1; }
		else if ( uVal1 < uVal2 ) { Res = -1; }
		else {	Res=0; }
		break;
	case CMP_COL_DIF_OFFSET_X:
		dVal1 = SigmaItem1.OffsetX.Max-SigmaItem1.OffsetX.Min;
		dVal2 = SigmaItem2.OffsetX.Max-SigmaItem2.OffsetX.Min;
		if ( dVal1 > dVal2 ) { Res = 1; }
		else if ( dVal1 < dVal2 ) { Res = -1; }
		else {	Res=0; }
		break;
	case CMP_COL_DIF_OFFSET_Y:
		dVal1 = SigmaItem1.OffsetY.Max-SigmaItem1.OffsetY.Min;
		dVal2 = SigmaItem2.OffsetY.Max-SigmaItem2.OffsetY.Min;
		if ( dVal1 > dVal2 ) { Res = 1; }
		else if ( dVal1 < dVal2 ) { Res = -1; }
		else {	Res=0; }
		break;
	case CMP_COL_DIF_SKEW_ANGLE:
		dVal1 = SigmaItem1.SkewAngle.Max-SigmaItem1.SkewAngle.Min;
		dVal2 = SigmaItem2.SkewAngle.Max-SigmaItem2.SkewAngle.Min;
		if ( dVal1 > dVal2 ) { Res = 1; }
		else if ( dVal1 < dVal2 ) { Res = -1; }
		else {	Res=0; }		
		break;
	case CMP_COL_DIF_BODY_HEIGHT:
		dVal1 = SigmaItem1.BodyHeight.Max-SigmaItem1.BodyHeight.Min;
		dVal2 = SigmaItem2.BodyHeight.Max-SigmaItem2.BodyHeight.Min;
		if ( dVal1 > dVal2 ) { Res = 1; }
		else if ( dVal1 < dVal2 ) { Res = -1; }
		else {	Res=0; }	
		break;
	case CMP_COL_GRR_SIGMA_ITEM_IDX:
		uVal1 = (CPtr1->GetComponentGrrSigmaItemIdx()+1);
		uVal2 = (CPtr2->GetComponentGrrSigmaItemIdx()+1);
		if ( uVal1 > uVal2 ) { Res = 1; }
		else if ( uVal1 < uVal2 ) { Res = -1; }
		else {	Res=0; }
		break;
	default:
		Res = 0;
		break;
	}
	int SortMode = GetComponentListSortMode();
	Res = Res*SortMode;	
	return Res;	
}
//-------------------------------------------------------------------------------------//
void CComponentListWnd::OnColumnclickComponentListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
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
	m_ComponentListWnd.SortItems(ComponentListCompareFn, (DWORD_PTR)this);
	
	const int nItem = m_ComponentListWnd.GetNextItem(-1, LVNI_FOCUSED);
	if ( nItem >= 0 ) 
	{	m_ComponentListWnd.EnsureVisible(nItem, FALSE); }

	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CComponentListWnd::OnItemchangedComponentListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if ( true == m_StopComponentListBeSelected ) { return; }
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) 
	{	return;	}
	DWORD Res = pNMListView->uNewState&LVIS_SELECTED;
	if ( Res == 0 ) { return; }	
	Res = pNMListView->uNewState&LVIS_FOCUSED;
	if ( Res == 0 ) { return; }	
	const size_t SelIndex = m_ComponentListWnd.GetItemData(nItem);
	const size_t ComponentCount = m_ComponentList.size();
	if ( SelIndex > ComponentCount ) { return; }
	CAOIComponent *ComponentPtr = m_ComponentList[SelIndex];
	if ( NULL == ComponentPtr ) { return; }
	CString ComponentName = ComponentPtr->GetComponentName();
	CString PartNumber = ComponentPtr->GetComponentPartNumber();
	CString ModelName = ComponentPtr->GetComponentModelName();
	CString Nozzle = ComponentPtr->GetComponentNozzleName();
	CWnd::SetDlgItemText(COMLIST_SEL_COMPONENT_EDIT, ComponentName);
	CWnd::SetDlgItemText(COMLIST_SEL_PART_NUMBER_EDIT, PartNumber);
	CWnd::SetDlgItemText(COMLIST_SEL_MODEL_NAME_EDIT, ModelName);
	CWnd::SetDlgItemText(COMLIST_SEL_NOZZLE_EDIT, Nozzle);	

	AOIDataCollect.MoveStageToComponentOrField(ComponentPtr, false);		
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CComponentListWnd::OnDblclkComponentListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NMITEMACTIVATE *pNMListView = (NMITEMACTIVATE*)pNMHDR;	
	m_ComponentPtr = NULL;
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	SetItemIndexAct(-1, -1);
	if ( nItem < 0 ) { return; }
	CThisListCtrl_07 &ListCtrl=m_ComponentListWnd;
	const size_t SelIndex = ListCtrl.GetItemData(nItem);
	const size_t ComponentCount = m_ComponentList.size();
	if ( SelIndex > ComponentCount ) { return; }
	CAOIComponent *ComponentPtr = m_ComponentList[SelIndex];
	if ( NULL == ComponentPtr ) { return; }	
	AOIDataCollect.MoveStageToComponentOrField(ComponentPtr, true);

	CString str;
	CRect   ItemRect={0};
	bool    bShowEdit=false;
	SetComponentListColID(nSubItem);
	switch ( nSubItem )
	{
	case CMP_COL_COMPONENT:
		bShowEdit = true;
		str = ComponentPtr->GetComponentName();
		break;
	case CMP_COL_PART_NUMBER:
		bShowEdit = true;
		str = ComponentPtr->GetComponentPartNumber();
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
		m_ComponentPtr = ComponentPtr;
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
void CComponentListWnd::OnKillfocusParamEdit() 
{
	// TODO: Add your control notification handler code here
	ExecUpdateParamByEdit();	
	ExecReleaseParamCtrl();
}
//-------------------------------------------------------------------------------------//
BOOL CComponentListWnd::PreTranslateMessage(MSG* pMsg) 
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
LRESULT CComponentListWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CBaseDialog::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
bool CComponentListWnd::ExecUpdateParamByEdit()
{
	if ( m_EditCtrl.GetSafeHwnd() == NULL ) { return false; }

	m_EditCtrl.ShowWindow(SW_HIDE);
	if ( NULL == m_ComponentPtr ) { return true; }
	const int nItem = m_nItemAct;
	const int nSubItem = m_nSubItemAct;
	if ( -1==nItem || -1==nSubItem ) { return true; }

	CString str;
	CString ItemText;
	bool    bUpdateAll=false;
	const int nColID = GetComponentListColD();
	CAOIBoard     *BoardPtr = NULL;
	CAOIComponent *ComponentPtr=m_ComponentPtr;
	CString ComName = ComponentPtr->GetComponentName();
	CString PartNumber = ComponentPtr->GetComponentPartNumber();
	
	m_EditCtrl.GetWindowText(ItemText);
	ItemText.MakeUpper();	
	switch ( nColID )
	{
	case CMP_COL_COMPONENT:		
		if ( ComName == ItemText ) { return true; }
		BoardPtr = ComponentPtr->GetComponentBoardPtr();
		if ( NULL != BoardPtr )
		{
			if ( BoardPtr->ChceckBoardComponentNameExist(ItemText) == true ) 
			{	return true; }
		}
		ComponentPtr->ChangeComponentName(ItemText);
		m_ComponentListWnd.SetItemText(nItem, nSubItem, ItemText);
		str = _T("Do you want to modify the same name components?");
		str = LoadMultiLanguageString(str, str);
		if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDYES )
		{
			bUpdateAll = true;
			m_ProjectPtr->SetProjectComponentName(ComName, ItemText);		
		}
		break;
	case CMP_COL_PART_NUMBER:
		if ( PartNumber == ItemText ) { return true; }
		ComponentPtr->SetComponentPartNumber(ItemText);
		m_ComponentListWnd.SetItemText(nItem, nSubItem, ItemText);
		str = _T("Do you want to modify the same partnumber components?");
		str = LoadMultiLanguageString(str, str);
		if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDYES )
		{
			bUpdateAll = true;
			m_ProjectPtr->SetProjectComponentPartNumber(PartNumber, ItemText);		
		}
		break;
	}
	if ( true == bUpdateAll ) 
	{	UpdateComponentListWnd();	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CComponentListWnd::ExecReleaseParamCtrl()
{
	m_ComponentPtr = NULL;
	SetItemIndexAct(-1, -1);
	m_EditCtrl.ShowWindow(SW_HIDE);
	m_ComponentListWnd.SetFocus();
	return true;
}
//-------------------------------------------------------------------------------------//
void CComponentListWnd::SetItemIndexAct(int nItem, int nSubItem)
{
	m_nItemAct = nItem;
	m_nSubItemAct = nSubItem;	
}
//-------------------------------------------------------------------------------------//