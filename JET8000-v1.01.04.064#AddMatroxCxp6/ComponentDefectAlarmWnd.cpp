// ComponentDefectAlarmWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "ComponentDefectAlarmWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CComponentDefectAlarmWnd dialog
//-------------------------------------------------------------------------------------//
CComponentDefectAlarmWnd::CComponentDefectAlarmWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CComponentDefectAlarmWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CComponentDefectAlarmWnd)
	m_ParamActPtr = NULL;
	m_EnableAlarm = true;
	m_EnableAlarmOnAOI = true;
	m_EnableAlarmOnARS = false;
	m_EnableDefectCountOnARS = false;
	m_StopAlarmListBeSelectedAOI = false;	
	m_StopAlarmListBeSelectedARS = false;
	m_AlarmParamFromModeAOI = DEFECT_PARAM_FROM_PROJECT;
	m_AlarmParamFromModeARS = DEFECT_PARAM_FROM_PROJECT;
	//}}AFX_DATA_INIT
}
//-------------------------------------------------------------------------------------//
void CComponentDefectAlarmWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CComponentDefectAlarmWnd)
	DDX_Control(pDX, COMDEFECT_PARAM_BTN, m_BtnCtrl);
	DDX_Control(pDX, COMDEFECT_PARAM_EDIT, m_EditCtrl);	
	DDX_Control(pDX, COMDEFECT_PARAM_COMBO, m_ComboxCtrl);	
	DDX_Control(pDX, COMDEFECT_ALARM_FROM_COMBO_AOI, m_AlarmComboxAOI);	
	DDX_Control(pDX, COMDEFECT_ALARM_FROM_COMBO_ARS, m_AlarmComboxARS);	
	DDX_Control(pDX, COMDEFECT_ALARM_LIST_WND_AOI, m_AlarmListWndAOI);	
	DDX_Control(pDX, COMDEFECT_ALARM_LIST_WND_ARS, m_AlarmListWndARS);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CComponentDefectAlarmWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CComponentDefectAlarmWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(LVN_ITEMCHANGED, COMDEFECT_ALARM_LIST_WND_AOI, OnItemchangedAlarmListWnd)
	ON_NOTIFY(NM_DBLCLK, COMDEFECT_ALARM_LIST_WND_AOI, OnDblclkAlarmListWnd)
	ON_NOTIFY(LVN_ITEMCHANGED, COMDEFECT_ALARM_LIST_WND_ARS, OnItemchangedAlarmListWndRepair)
	ON_NOTIFY(NM_DBLCLK, COMDEFECT_ALARM_LIST_WND_ARS, OnDblclkAlarmListWndRepair)
	ON_EN_KILLFOCUS(COMDEFECT_PARAM_EDIT, OnKillfocusParamEdit)
	ON_CBN_SELCHANGE(COMDEFECT_PARAM_COMBO, OnSelchangeParamCombo)
	ON_CBN_KILLFOCUS(COMDEFECT_PARAM_COMBO, OnKillfocusParamCombo)
	ON_BN_CLICKED(COMDEFECT_PARAM_BTN, OnParamBtn)
	ON_CBN_SELCHANGE(COMDEFECT_ALARM_FROM_COMBO_AOI, OnSelchangeAlarmFromComboAOI)
	ON_BN_CLICKED(COMDEFECT_ALARM_ENABLE_ALL_BTN_AOI, OnAlarmEnableAllBtnAOI)
	ON_BN_CLICKED(COMDEFECT_ALARM_DISABLE_ALL_BTN_AOI, OnAlarmDisableAllBtnAOI)
	ON_BN_CLICKED(COMDEFECT_ALARM_COPY_ARS_BTN_AOI, OnAlarmCopyArsBtnAOI)
	ON_CBN_SELCHANGE(COMDEFECT_ALARM_FROM_COMBO_ARS, OnSelchangeAlarmFromComboARS)
	ON_BN_CLICKED(COMDEFECT_ALARM_ENABLE_ALL_BTN_ARS, OnAlarmEnableAllBtnARS)
	ON_BN_CLICKED(COMDEFECT_ALARM_DISABLE_ALL_BTN_ARS, OnAlarmDisableAllBtnARS)
	ON_BN_CLICKED(COMDEFECT_ALARM_COPY_AOI_BTN_ARS, OnAlarmCopyAoiBtnARS)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CComponentDefectAlarmWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CComponentDefectAlarmWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	JetAPI::InitialListCtrl(m_AlarmListWndAOI);
	JetAPI::InitialListCtrl(m_AlarmListWndARS);
	SwitchMultiLanguage();	
	BuildAlarmListWndHeader();
	BuildAlarmListWnd();
	CWnd::CheckDlgButton(COMDEFECT_ALARM_CHK, m_EnableAlarm);
	CWnd::CheckDlgButton(COMDEFECT_ALARM_ON_AOI_CHK, m_EnableAlarmOnAOI);
	CWnd::CheckDlgButton(COMDEFECT_ALARM_ON_ARS_CHK, m_EnableAlarmOnARS);	
	CWnd::CheckDlgButton(COMDEFECT_DEFECT_COUNT_ON_ARS_CHK, m_EnableDefectCountOnARS);		
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	AOIDataDefine.BuildDefectParamFromCombox(m_AlarmComboxAOI);
	AOIDataDefine.BuildDefectParamFromCombox(m_AlarmComboxARS);
	JetAPI::SetComboxCurSel(m_AlarmComboxAOI, m_AlarmParamFromModeAOI);
	JetAPI::SetComboxCurSel(m_AlarmComboxARS, m_AlarmParamFromModeARS);

	const bool bShowDefectAlarmARS=false;
	if ( false == bShowDefectAlarmARS )
	{
		const BOOL bShow = FALSE;
		JetAPI::ShowCtrlWnd(this, COMDEFECT_ALARM_GROUP_AOI, bShow);
		JetAPI::ShowCtrlWnd(this, COMDEFECT_ALARM_COPY_ARS_BTN_AOI, bShow);
		
		JetAPI::ShowCtrlWnd(this, COMDEFECT_ALARM_GROUP_ARS, bShow);
		JetAPI::ShowCtrlWnd(this, COMDEFECT_ALARM_LIST_WND_ARS, bShow);		
		JetAPI::ShowCtrlWnd(this, COMDEFECT_ALARM_FROM_LABEL_ARS, bShow);
		JetAPI::ShowCtrlWnd(this, COMDEFECT_ALARM_FROM_COMBO_ARS, bShow);		
		JetAPI::ShowCtrlWnd(this, COMDEFECT_ALARM_ENABLE_ALL_BTN_ARS, bShow);
		JetAPI::ShowCtrlWnd(this, COMDEFECT_ALARM_DISABLE_ALL_BTN_ARS, bShow);
		JetAPI::ShowCtrlWnd(this, COMDEFECT_ALARM_COPY_AOI_BTN_ARS, bShow);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CComponentDefectAlarmWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CComponentDefectAlarmWnd::OnSize(UINT nType, int cx, int cy) 
{
	CBaseDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CComponentDefectAlarmWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CBaseDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
bool CComponentDefectAlarmWnd::GetEnableAlarm() const
{
	return m_EnableAlarm;
}
//-------------------------------------------------------------------------------------//
void CComponentDefectAlarmWnd::SetEnableAlarm(bool val)
{
	m_EnableAlarm = val;
}
//-------------------------------------------------------------------------------------//
bool CComponentDefectAlarmWnd::GetEnableAlarmOnAOI() const
{
	return m_EnableAlarmOnAOI;
}
//-------------------------------------------------------------------------------------//
void CComponentDefectAlarmWnd::SetEnableAlarmOnAOI(bool val)
{
	m_EnableAlarmOnAOI = val;
}
//-------------------------------------------------------------------------------------//
bool CComponentDefectAlarmWnd::GetEnableAlarmOnARS() const
{
	return m_EnableAlarmOnARS;
}
//-------------------------------------------------------------------------------------//
void CComponentDefectAlarmWnd::SetEnableAlarmOnARS(bool val)
{
	m_EnableAlarmOnARS = val;
}
//-------------------------------------------------------------------------------------//
bool CComponentDefectAlarmWnd::GetEnableDefectCountOnARS() const
{
	return m_EnableDefectCountOnARS;
}
//-------------------------------------------------------------------------------------//
void CComponentDefectAlarmWnd::SetEnableDefectCountOnARS(bool val)
{
	m_EnableDefectCountOnARS = val;
}
//-------------------------------------------------------------------------------------//	
const CWndDefectItem& CComponentDefectAlarmWnd::GetDefectAlarmAOI() const
{
	return m_DefectAlarmAOI;
}
//-------------------------------------------------------------------------------------//
void CComponentDefectAlarmWnd::SetDefectAlarmAOI(const CWndDefectItem &val)
{
	m_DefectAlarmAOI = val;
}
//-------------------------------------------------------------------------------------//	
const CWndDefectItem& CComponentDefectAlarmWnd::GetDefectAlarmARS() const
{
	return m_DefectAlarmARS;
}
//-------------------------------------------------------------------------------------//
void CComponentDefectAlarmWnd::SetDefectAlarmARS(const CWndDefectItem &val)
{
	m_DefectAlarmARS = val;
}
//-------------------------------------------------------------------------------------//
DEFECT_PARAM_FROM_MODE CComponentDefectAlarmWnd::GetAlarmParamFromModeAOI() const
{
	return m_AlarmParamFromModeAOI;
}
//-------------------------------------------------------------------------------------//
void CComponentDefectAlarmWnd::SetAlarmParamFromModeAOI(DEFECT_PARAM_FROM_MODE val)
{
	m_AlarmParamFromModeAOI = val;
}
//-------------------------------------------------------------------------------------//
DEFECT_PARAM_FROM_MODE CComponentDefectAlarmWnd::GetAlarmParamFromModeARS() const
{
	return m_AlarmParamFromModeARS;
}
//-------------------------------------------------------------------------------------//
void CComponentDefectAlarmWnd::SetAlarmParamFromModeARS(DEFECT_PARAM_FROM_MODE val)
{
	m_AlarmParamFromModeARS = val;
}
//-------------------------------------------------------------------------------------//
CParamUni* CComponentDefectAlarmWnd::GetActParamUni()
{
	return m_ParamActPtr;
}
//-------------------------------------------------------------------------------------//
void CComponentDefectAlarmWnd::SetActParamUni(CParamUni *Ptr)
{
	m_ParamActPtr = Ptr;
}
//-------------------------------------------------------------------------------------//
void CComponentDefectAlarmWnd::SetDescriptionText(const CParamUni *Ptr)
{
}
//-------------------------------------------------------------------------------------//
bool CComponentDefectAlarmWnd::BuildAlarmListWndHeader(CThisListCtrl_66 &ListCtrl)
{
	CString str;
	int   nCol = 0;
	int width  = 64;
	int width2 = 64;
	RECT      Rect;	
	const int Align = LVCFMT_LEFT;//LVCFMT_CENTER;LVCFMT_RIGHT, LVCFMT_LEFT		

	ListCtrl.GetClientRect(&Rect);
	width = (Rect.right-Rect.left-16)/2;
	str = _T("Item");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(nCol, str, Align, width);
	nCol ++;

	width2 = width;
	str = _T("Information");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(nCol, str, Align, width2);
	nCol ++;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CComponentDefectAlarmWnd::BuildAlarmListWnd(CThisListCtrl_66 &ListCtrl, bool &StopSelected, CWndDefectItem &DefectItem, CParamList &ParamList)
{	
	StopSelected = true;
	JetAPI::ClearListCtrl(ListCtrl, FALSE);
	StopSelected = false;

	int           i=0;
	CString       str;
	CString       strValue;	
	CString       strCaption;	
	int           nItem=0;
	CParamUni    *ParamPtr=NULL;
	const int     nSubItem = 1;
	const UINT    WndCtrlID = ListCtrl.GetDlgCtrlID();

	BuildAlarmList(DefectItem, ParamList);

	const int ParamCount = (int)(ParamList.size());

	nItem=0;
	ListCtrl.SetRedraw(FALSE);
	StopSelected = true;
	for ( i=0; i<ParamCount; i++ )
	{
		CParamUni &ParamUni = ParamList[i];		
		
		ParamUni.SetListCtrl(&ListCtrl);
		ParamUni.SetWndCtrlID(WndCtrlID);
		ParamUni.SetItemIndex(nItem);
		ParamUni.SetSubItemIndex(nSubItem);		

		strCaption = ParamUni.GetCaption();
		strValue = ParamUni.GetParamText();
		ListCtrl.InsertItem(nItem, strCaption);
		ListCtrl.SetItemData(nItem, i);
		ListCtrl.SetItemText(nItem, nSubItem, strCaption);
		ListCtrl.SetItemText(nItem, nSubItem, strValue);
		nItem ++;
	}	
	StopSelected = false;
	ListCtrl.SetRedraw(TRUE);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CComponentDefectAlarmWnd::BuildAlarmList(CWndDefectItem &DefectItem, CParamList &ParamList)
{	
	ParamList.clear();	
	const std::vector<WND_DEFECT_ID> &List=AOIDataCollect.GetWndDefectIDList();
	const size_t Count=List.size();
	for ( size_t i=0; i<Count; i++ )
	{	AddDefectAlarmItem(List[i], DefectItem, ParamList);	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CComponentDefectAlarmWnd::AddDefectAlarmItem(WND_DEFECT_ID WndDefectID, CWndDefectItem &DefectItem, CParamList &ParamList)
{
	CString strValue;
	CString strCaption;
	CString strDescription;
	CParamUni ParamUnit;
	const int WndDefectEnable = DefectItem.GetItemCount(WndDefectID);
	strCaption = AOIDataDefine.GetWndDefectIDText(WndDefectID);
	ParamUnit.SetCaption(strCaption);	
	ParamUnit.SetParamID(WndDefectID);	
	strValue = AOIDataDefine.GetWndDefectItemModeText(WND_DEFECT_ITEM_ENABLE);	ParamUnit.AddSelItem(WND_DEFECT_ITEM_ENABLE, strValue);
	strValue = AOIDataDefine.GetWndDefectItemModeText(WND_DEFECT_ITEM_DISABLE);	ParamUnit.AddSelItem(WND_DEFECT_ITEM_DISABLE, strValue);	
	//strValue = AOIDataDefine.GetWndDefectItemModeText(WND_DEFECT_ITEM_NO_SHOW);	ParamUnit.AddSelItem(WND_DEFECT_ITEM_NO_SHOW, strValue);	
	ParamUnit.SetValue_SEL(WndDefectEnable);	
	ParamUnit.SetDesction(strDescription);		
	ParamList.push_back(ParamUnit);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CComponentDefectAlarmWnd::ExecItemchangedParamListWnd(CThisListCtrl_66 &ListCtrl, CParamList &ParamList, int nItem)
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
	{	m_BtnCtrl.ShowWindow(SW_HIDE);	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CComponentDefectAlarmWnd::ExecDblclkParamListWnd(CThisListCtrl_66 &ListCtrl, CParamList &ParamList, int nItem, int nSubItem, int SetCol)
{
	const int    ItemCount = ListCtrl.GetItemCount();
	if ( nItem<0 || nItem>=ItemCount ) { return false; }
	if ( nSubItem < SetCol ) { return true; }

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
	m_BtnCtrl.ShowWindow(SW_HIDE);
	SetActParamUni(ParamPtr);
	if ( PARAM_DATA_SEL == DataType )
	{
		if ( m_ComboxCtrl.GetSafeHwnd() != NULL )
		{
			nSelIdx = 0;
			JetAPI::ClearCombox(m_ComboxCtrl);
			for ( i=0; i<SelItemCount; i++ )
			{
				if ( ParamPtr->GetSelItem(i, true, nValue, ItemText) == false ) { continue; }
				m_ComboxCtrl.InsertString(nSelIdx, ItemText);
				m_ComboxCtrl.SetItemData(nSelIdx, nValue);
				nSelIdx ++;
			}			
			JetAPI::SetComboxCurSel(m_ComboxCtrl, ParamPtr->GetSelParam());
			m_ComboxCtrl.MoveWindow(&CtrlRect, FALSE);			
			m_ComboxCtrl.SetFocus();
			m_ComboxCtrl.ShowDropDown();
			m_ComboxCtrl.ShowWindow(SW_SHOW);
			m_ComboxCtrl.BringWindowToTop();			
			ListCtrl.UpdateWindow();
			m_ComboxCtrl.Invalidate();
		}	
	}
	else
	{
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
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CComponentDefectAlarmWnd::ExecReleaseParamCtrl()
{
	m_BtnCtrl.ShowWindow(SW_HIDE);
	m_EditCtrl.ShowWindow(SW_HIDE);	
	m_ComboxCtrl.ShowWindow(SW_HIDE);

	m_EditCtrl.SetWindowText(_T(""));
	//m_ParamListCtrl.SetFocus();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CComponentDefectAlarmWnd::ExecUpdateParamByEdit()
{
	CParamUni *ParamPtr = GetActParamUni();
	if ( NULL == ParamPtr ) { return true; }
	PARAM_DATA_TYPE  DataType = ParamPtr->GetDataType();	
	if ( PARAM_DATA_SEL == DataType ) { return true; }
	SetActParamUni(NULL);	

	return true;
}
//-------------------------------------------------------------------------------------//
bool CComponentDefectAlarmWnd::ExecUpdateParamByCombox()
{
	CParamUni *ParamPtr = GetActParamUni();
	if ( NULL == ParamPtr ) { return true; }	
	UINT WndCtrlID = ParamPtr->GetWndCtrlID();
	PARAM_DATA_TYPE  DataType = ParamPtr->GetDataType();
	if ( PARAM_DATA_SEL != DataType ) { return true; }
	SetActParamUni(NULL);

	CString ItemText;		
	const int nCurSel = m_ComboxCtrl.GetCurSel();
	if ( nCurSel < 0 ) { return true; }
	const int Param = (int)(m_ComboxCtrl.GetItemData(nCurSel));
	if ( Param == ParamPtr->GetSelParam() ) { return false; }
	ItemText.Format(_T("%d"),Param);	
	if ( ParamPtr->SetNewValue_SEL(Param) == false )
	{	return false; }
	if ( COMDEFECT_ALARM_LIST_WND_AOI == WndCtrlID )
	{
		WND_DEFECT_ID DefectID = (WND_DEFECT_ID)(ParamPtr->GetParamID());
		m_DefectAlarmAOI.SetItemCount(DefectID, Param);		
	}
	if ( COMDEFECT_ALARM_LIST_WND_ARS == WndCtrlID )
	{
		WND_DEFECT_ID DefectID = (WND_DEFECT_ID)(ParamPtr->GetParamID());
		m_DefectAlarmARS.SetItemCount(DefectID, Param);
	}		
	
	ItemText = ParamPtr->GetParamText();
	CThisListCtrl_66 *pListCtrl = (CThisListCtrl_66*)(ParamPtr->GetListCtrl());
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
bool CComponentDefectAlarmWnd::BuildAlarmListWndHeader()
{
	bool bSucc = true;
	if ( BuildAlarmListWndHeader(m_AlarmListWndAOI) == false )
	{	bSucc = false; }
	if ( BuildAlarmListWndHeader(m_AlarmListWndARS) == false )
	{	bSucc = false; }
	return bSucc;
}
//-------------------------------------------------------------------------------------//
bool CComponentDefectAlarmWnd::BuildAlarmListWnd()
{
 	bool bSucc = true;
	SetActParamUni(NULL);
	if ( BuildAlarmListWnd(m_AlarmListWndAOI, m_StopAlarmListBeSelectedAOI, m_DefectAlarmAOI, m_AlarmListAOI) == false ) 
	{	bSucc = false; }
	if ( BuildAlarmListWnd(m_AlarmListWndARS, m_StopAlarmListBeSelectedARS, m_DefectAlarmARS, m_AlarmListARS) == false ) 
	{	bSucc = false; }
	return true;	
}
//-------------------------------------------------------------------------------------//
void CComponentDefectAlarmWnd::SwitchMultiLanguage()
{
	//---------------------------------------------------------------------------------//	
	SetMultiLanauage(LoadIDAndName(IDD_COMPONENT_DEFECT_ALARM_WND), false);
	//---------------------------------------------------------------------------------//	
	SetMultiLanauage(LoadIDAndName(IDOK));
	SetMultiLanauage(LoadIDAndName(IDCANCEL));
	//---------------------------------------------------------------------------------//		
	SetMultiLanauage(LoadIDAndName(COMDEFECT_ALARM_CHK));
	SetMultiLanauage(LoadIDAndName(COMDEFECT_ALARM_ON_AOI_CHK));
	SetMultiLanauage(LoadIDAndName(COMDEFECT_ALARM_ON_ARS_CHK));
	SetMultiLanauage(LoadIDAndName(COMDEFECT_DEFECT_COUNT_ON_ARS_CHK));	
	//---------------------------------------------------------------------------------//	
	SetMultiLanauage(LoadIDAndName(COMDEFECT_ALARM_GROUP_AOI));
	SetMultiLanauage(LoadIDAndName(COMDEFECT_ALARM_FROM_LABEL_AOI));
	SetMultiLanauage(LoadIDAndName(COMDEFECT_ALARM_ENABLE_ALL_BTN_AOI));
	SetMultiLanauage(LoadIDAndName(COMDEFECT_ALARM_DISABLE_ALL_BTN_AOI));
	SetMultiLanauage(LoadIDAndName(COMDEFECT_ALARM_COPY_ARS_BTN_AOI));	
	//---------------------------------------------------------------------------------//	
	SetMultiLanauage(LoadIDAndName(COMDEFECT_ALARM_GROUP_ARS));
	SetMultiLanauage(LoadIDAndName(COMDEFECT_ALARM_FROM_LABEL_ARS));
	SetMultiLanauage(LoadIDAndName(COMDEFECT_ALARM_ENABLE_ALL_BTN_ARS));
	SetMultiLanauage(LoadIDAndName(COMDEFECT_ALARM_DISABLE_ALL_BTN_ARS));
	SetMultiLanauage(LoadIDAndName(COMDEFECT_ALARM_COPY_AOI_BTN_ARS));	
	//---------------------------------------------------------------------------------//	
}
//-------------------------------------------------------------------------------------//
bool CComponentDefectAlarmWnd::SetMultiLanauage(UINT ID, LPCTSTR Text, bool bCtrlID)
{
	LPCTSTR Section=_T("IDD_COMPONENT_DEFECT_ALARM_WND");		
	if ( AOIDataCollect.SwitchMultiLanguageWnd(this, Section, ID, Text, bCtrlID) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
CString CComponentDefectAlarmWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_COMPONENT_DEFECT_ALARM_WND");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
void CComponentDefectAlarmWnd::OnItemchangedAlarmListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if ( true == m_StopAlarmListBeSelectedAOI ) { return; }	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) 
	{
		SetDescriptionText(NULL);
		return; 
	}
	DWORD Res = pNMListView->uNewState&LVIS_SELECTED;
	if ( Res == 0 ) { return; }	
	Res = pNMListView->uNewState&LVIS_FOCUSED;
	if ( Res == 0 ) { return; }		
	ExecItemchangedParamListWnd(m_AlarmListWndAOI, m_AlarmListAOI, nItem);	
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CComponentDefectAlarmWnd::OnDblclkAlarmListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NMITEMACTIVATE *pNMListView = (NMITEMACTIVATE*)pNMHDR;	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) { return; }

	ExecDblclkParamListWnd(m_AlarmListWndAOI, m_AlarmListAOI, nItem, nSubItem, 1);
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CComponentDefectAlarmWnd::OnItemchangedAlarmListWndRepair(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if ( true == m_StopAlarmListBeSelectedARS ) { return; }	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) 
	{
		SetDescriptionText(NULL);
		return; 
	}
	DWORD Res = pNMListView->uNewState&LVIS_SELECTED;
	if ( Res == 0 ) { return; }	
	Res = pNMListView->uNewState&LVIS_FOCUSED;
	if ( Res == 0 ) { return; }		
	ExecItemchangedParamListWnd(m_AlarmListWndARS, m_AlarmListARS, nItem);	
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CComponentDefectAlarmWnd::OnDblclkAlarmListWndRepair(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NMITEMACTIVATE *pNMListView = (NMITEMACTIVATE*)pNMHDR;	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) { return; }

	ExecDblclkParamListWnd(m_AlarmListWndARS, m_AlarmListARS, nItem, nSubItem, 1);
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CComponentDefectAlarmWnd::OnKillfocusParamEdit() 
{
	// TODO: Add your control notification handler code here
	if ( m_EditCtrl.IsWindowVisible() == FALSE ) { return; }
	ExecUpdateParamByEdit();
	m_EditCtrl.ShowWindow(SW_HIDE);	
	m_EditCtrl.SetWindowText(_T(""));
}
//-------------------------------------------------------------------------------------//
void CComponentDefectAlarmWnd::OnSelchangeParamCombo() 
{
	// TODO: Add your control notification handler code here
	if ( ExecUpdateParamByCombox() == false )
	{	return; }
	m_ComboxCtrl.ShowWindow(SW_HIDE);	
	JetAPI::ClearCombox(m_ComboxCtrl);	
}
//-------------------------------------------------------------------------------------//
void CComponentDefectAlarmWnd::OnKillfocusParamCombo() 
{
	// TODO: Add your control notification handler code here
	if ( m_ComboxCtrl.IsWindowVisible() == FALSE ) { return; }
	ExecUpdateParamByCombox();
	m_ComboxCtrl.ShowWindow(SW_HIDE);	
	JetAPI::ClearCombox(m_ComboxCtrl);	
}
//-------------------------------------------------------------------------------------//
void CComponentDefectAlarmWnd::OnParamBtn() 
{
	// TODO: Add your control notification handler code here
	m_BtnCtrl.ShowWindow(SW_HIDE);
	CParamUni *ParamPtr = GetActParamUni();
	if ( NULL == ParamPtr ) { return; }	
	CWnd      *BtnWndPtr = ParamPtr->GetBtnWndPtr();
	if ( NULL == BtnWndPtr ) { return; }	
}
//-------------------------------------------------------------------------------------//
BOOL CComponentDefectAlarmWnd::PreTranslateMessage(MSG* pMsg) 
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
			if ( m_ComboxCtrl.IsWindowVisible() == TRUE ) 
			{
				ExecUpdateParamByCombox();
				m_ComboxCtrl.ShowWindow(SW_HIDE);						
				JetAPI::ClearCombox(m_ComboxCtrl);
				return TRUE;				
			}
			break;
		case VK_ESCAPE:
			ExecReleaseParamCtrl();			
			break;
		}
		break;
	}
	return CBaseDialog::PreTranslateMessage(pMsg);
}
//-------------------------------------------------------------------------------------//
void CComponentDefectAlarmWnd::OnSelchangeAlarmFromComboAOI() 
{
	// TODO: Add your control notification handler code here
	
}
//-------------------------------------------------------------------------------------//
void CComponentDefectAlarmWnd::OnAlarmEnableAllBtnAOI() 
{
	// TODO: Add your control notification handler code here
	CString str, str2;
	CString strForm=AOIDataDefine.GetDefectFromText(DEFECT_FROM_AOI);
	str = _T("Do you want to enable all defect alarm?");
	str = LoadMultiLanguageString(str, str);
	str2.Format(_T("[%s]%s"), strForm, str);
	if ( JetAPI::ShowMessageBox(str2, MB_YESNO) == IDNO ) 
	{	return; }
	const int nValue = WND_DEFECT_ITEM_ENABLE;
	m_DefectAlarmAOI.SetAll(nValue);	
	BuildAlarmListWnd(m_AlarmListWndAOI, m_StopAlarmListBeSelectedAOI, m_DefectAlarmAOI, m_AlarmListAOI);	
}
//-------------------------------------------------------------------------------------//
void CComponentDefectAlarmWnd::OnAlarmDisableAllBtnAOI() 
{
	// TODO: Add your control notification handler code here
	CString str, str2;
	CString strForm=AOIDataDefine.GetDefectFromText(DEFECT_FROM_AOI);
	str = _T("Do you want to disable all defect alarm?");
	str = LoadMultiLanguageString(str, str);
	str2.Format(_T("[%s]%s"), strForm, str);
	if ( JetAPI::ShowMessageBox(str2, MB_YESNO) == IDNO ) 
	{	return; }
	const int nValue = WND_DEFECT_ITEM_DISABLE;
	m_DefectAlarmAOI.SetAll(nValue);	
	BuildAlarmListWnd(m_AlarmListWndAOI, m_StopAlarmListBeSelectedAOI, m_DefectAlarmAOI, m_AlarmListAOI);	
}
//-------------------------------------------------------------------------------------//
void CComponentDefectAlarmWnd::OnAlarmCopyArsBtnAOI() 
{
	// TODO: Add your control notification handler code here
	CString str, str2;
	CString strForm=AOIDataDefine.GetDefectFromText(DEFECT_FROM_AOI);
	str = _T("Do you want to copy all defect alarm?");
	str = LoadMultiLanguageString(str, str);
	str2.Format(_T("[%s]%s"), strForm, str);
	if ( JetAPI::ShowMessageBox(str2, MB_YESNO) == IDNO ) 
	{	return; }	
	m_DefectAlarmAOI = m_DefectAlarmARS;
	BuildAlarmListWnd(m_AlarmListWndAOI, m_StopAlarmListBeSelectedAOI, m_DefectAlarmAOI, m_AlarmListAOI);	
}
//-------------------------------------------------------------------------------------//
void CComponentDefectAlarmWnd::OnSelchangeAlarmFromComboARS() 
{
	// TODO: Add your control notification handler code here
	
}
//-------------------------------------------------------------------------------------//
void CComponentDefectAlarmWnd::OnAlarmEnableAllBtnARS() 
{
	// TODO: Add your control notification handler code here
	CString str, str2;
	CString strForm=AOIDataDefine.GetDefectFromText(DEFECT_FROM_ARS);
	str = _T("Do you want to enable all defect alarm?");
	str = LoadMultiLanguageString(str, str);
	str2.Format(_T("[%s]%s"), strForm, str);
	if ( JetAPI::ShowMessageBox(str2, MB_YESNO) == IDNO ) 
	{	return; }
	const int nValue = WND_DEFECT_ITEM_ENABLE;
	m_DefectAlarmARS.SetAll(nValue);		
	BuildAlarmListWnd(m_AlarmListWndARS, m_StopAlarmListBeSelectedARS, m_DefectAlarmARS, m_AlarmListARS);	
}
//-------------------------------------------------------------------------------------//
void CComponentDefectAlarmWnd::OnAlarmDisableAllBtnARS() 
{
	// TODO: Add your control notification handler code here
	CString str, str2;
	CString strForm=AOIDataDefine.GetDefectFromText(DEFECT_FROM_ARS);
	str = _T("Do you want to disable all defect alarm?");
	str = LoadMultiLanguageString(str, str);
	str2.Format(_T("[%s]%s"), strForm, str);
	if ( JetAPI::ShowMessageBox(str2, MB_YESNO) == IDNO ) 
	{	return; }
	const int nValue = WND_DEFECT_ITEM_DISABLE;
	m_DefectAlarmARS.SetAll(nValue);		
	BuildAlarmListWnd(m_AlarmListWndARS, m_StopAlarmListBeSelectedARS, m_DefectAlarmARS, m_AlarmListARS);
}
//-------------------------------------------------------------------------------------//
void CComponentDefectAlarmWnd::OnAlarmCopyAoiBtnARS() 
{
	// TODO: Add your control notification handler code here
	CString str, str2;
	CString strForm=AOIDataDefine.GetDefectFromText(DEFECT_FROM_ARS);
	str = _T("Do you want to copy all defect alarm?");
	str = LoadMultiLanguageString(str, str);
	str2.Format(_T("[%s]%s"), strForm, str);
	if ( JetAPI::ShowMessageBox(str2, MB_YESNO) == IDNO ) 
	{	return; }	
	m_DefectAlarmARS = m_DefectAlarmAOI;
	BuildAlarmListWnd(m_AlarmListWndARS, m_StopAlarmListBeSelectedARS, m_DefectAlarmARS, m_AlarmListARS);
}
//-------------------------------------------------------------------------------------//
void CComponentDefectAlarmWnd::OnOK() 
{
	// TODO: Add extra validation here
	m_EnableAlarm = (bool)(CWnd::IsDlgButtonChecked(COMDEFECT_ALARM_CHK));
	m_EnableAlarmOnAOI = (bool)(CWnd::IsDlgButtonChecked(COMDEFECT_ALARM_ON_AOI_CHK));
	m_EnableAlarmOnARS = (bool)(CWnd::IsDlgButtonChecked(COMDEFECT_ALARM_ON_ARS_CHK));
	m_EnableDefectCountOnARS = (bool)(CWnd::IsDlgButtonChecked(COMDEFECT_DEFECT_COUNT_ON_ARS_CHK));
	m_AlarmParamFromModeAOI = (DEFECT_PARAM_FROM_MODE)(JetAPI::GetComboxCurSelData(m_AlarmComboxAOI));
	m_AlarmParamFromModeARS = (DEFECT_PARAM_FROM_MODE)(JetAPI::GetComboxCurSelData(m_AlarmComboxARS));

	CBaseDialog::OnOK();
}
//-------------------------------------------------------------------------------------//