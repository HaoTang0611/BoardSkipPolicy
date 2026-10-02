// ProjectParamPaneRepair.cpp : implementation file
//

#include "stdafx.h"
#include "jet8000.h"
#include "ProjectParamPaneRepair.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
const int SETTING_COL   = 2;//設定的欄位
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CProjectParamPaneRepair dialog
//-------------------------------------------------------------------------------------//
CProjectParamPaneRepair::CProjectParamPaneRepair(CWnd* pParent /*=NULL*/)
	: CDialog(CProjectParamPaneRepair::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProjectParamPaneRepair)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ProjectPtr = NULL;
	m_ParamActPtr = NULL;	
	m_ProParameterPtr = NULL;	
	m_StopParamListBeSelected = false;
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneRepair::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProjectParamPaneRepair)	
	DDX_Control(pDX, PROREPAIR_PARAM_BTN, m_BtnCtrl);
	DDX_Control(pDX, PROREPAIR_PARAM_EDIT, m_EditCtrl);	
	DDX_Control(pDX, PROREPAIR_PARAM_COMBO, m_ComboxCtrl);	
	DDX_Control(pDX, PROREPAIR_PARAM_LIST_WND, m_RepairParamListCtrl);	
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CProjectParamPaneRepair, CDialog)
	//{{AFX_MSG_MAP(CProjectParamPaneRepair)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(LVN_ITEMCHANGED, PROREPAIR_PARAM_LIST_WND, OnItemchangedParamListWnd)
	ON_NOTIFY(NM_DBLCLK, PROREPAIR_PARAM_LIST_WND, OnDblclkParamListWnd)
	ON_EN_KILLFOCUS(PROREPAIR_PARAM_EDIT, OnKillfocusParamEdit)	
	ON_CBN_SELCHANGE(PROREPAIR_PARAM_COMBO, OnSelchangeParamCombo)
	ON_CBN_KILLFOCUS(PROREPAIR_PARAM_COMBO, OnKillfocusParamCombo)
	ON_BN_CLICKED(PROREPAIR_PARAM_BTN, OnParamBtn)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CProjectParamPaneRepair message handlers
//-------------------------------------------------------------------------------------//
BOOL CProjectParamPaneRepair::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SwitchMultiLanguage();
	JetAPI::InitialListCtrl(m_RepairParamListCtrl);		
	BuildParamListWndHeader();
	if ( CWnd::IsWindowVisible() )
	{	BuildRepairParamListWnd();	 }
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneRepair::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneRepair::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if ( CWnd::GetSafeHwnd() == NULL ) { return; }
	if ( m_RepairParamListCtrl.GetSafeHwnd() == NULL ) { return; }
	CWnd *WndPtr = NULL;
	SIZE  WndSize={0};
	RECT  InfoRect={0};
	BOOL  bVisible = CWnd::IsWindowVisible();
	int       nRight = cx;
	int       TopPosY = 0;	
	POINT     OffsetL;
	POINT     OffsetR;
	const int MarginX = 4;
	const int MarginY = 4;	
	const int ListSizeW = (cx-MarginX-MarginX-MarginX)/2;
	
	nRight -= MarginX;	
	OffsetL.x = OffsetL.y = 0;	
	WndPtr = CWnd::GetDlgItem(PROREPAIR_INFO_EDIT);
	if ( NULL!=WndPtr && WndPtr->GetSafeHwnd()!=NULL )
	{
		RECT WndRect={0};
		WndPtr->GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		JetAPI::GetRectSize(WndRect, WndSize);
		WndRect.left = MarginX;
		WndRect.right = cx;		
		WndRect.bottom = cy-MarginY;
		WndRect.top = WndRect.bottom-WndSize.cy;
		WndPtr->MoveWindow(&WndRect, FALSE);
		InfoRect = WndRect;
	}
	else
	{		
		InfoRect.left = 0;	InfoRect.right = cx;
		InfoRect.top = cy; InfoRect.bottom = cy;		
	}	
	
	if ( m_RepairParamListCtrl.GetSafeHwnd() != NULL )
	{
		RECT WndRect={0};
		m_RepairParamListCtrl.GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		JetAPI::GetRectSize(WndRect, WndSize);
		WndRect.left = MarginX;
		WndRect.right = nRight-MarginX;
		WndRect.top = TopPosY+MarginY;
		WndRect.bottom = InfoRect.top-MarginY;
		m_RepairParamListCtrl.MoveWindow(&WndRect, FALSE);		
		if ( TRUE == bVisible )
		{	m_RepairParamListCtrl.Invalidate();	}
	}
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneRepair::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if ( TRUE == bShow )
	{	BuildRepairParamListWnd();	}
}
//-------------------------------------------------------------------------------------//
BOOL CProjectParamPaneRepair::PreTranslateMessage(MSG* pMsg) 
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
	return CDialog::PreTranslateMessage(pMsg);
}
//-------------------------------------------------------------------------------------//
LRESULT CProjectParamPaneRepair::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneRepair::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_PROJECT_PARAM_REPAIR_PANE");
	//---------------------------------------------------------------------------------//
	WndID = IDD_PROJECT_PARAM_REPAIR_PANE;
	WndKey = _T("IDD_PROJECT_PARAM_REPAIR_PANE");
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
}
//-------------------------------------------------------------------------------------//
CString CProjectParamPaneRepair::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_PROJECT_PARAM_REPAIR_PANE");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneRepair::SetProjectParameterPtr(CAOIProject *ProjectPtr, TProjectParameter *Ptr)
{
	this->m_ProjectPtr = ProjectPtr;
	this->m_ProParameterPtr = Ptr;
}
//-------------------------------------------------------------------------------------//
bool CProjectParamPaneRepair::BuildParamListWndHeader()
{
	CString str;
	int   nCol = 0;
	int width  = 64;
	int width2 = 64;
	RECT      Rect;	
	const int Align = LVCFMT_LEFT;//LVCFMT_CENTER;LVCFMT_RIGHT, LVCFMT_LEFT

	{
		CThisListCtrl_25 &ListCtrl = m_RepairParamListCtrl;

		ListCtrl.GetClientRect(&Rect);
		width = (Rect.right-Rect.left-8)/5;
		width2 = 48;
		str = AOIDataDefine.GetIndexText();	
		ListCtrl.InsertColumn(nCol, str, Align, width2);
		nCol ++;

		width2 = width*2;
		str = _T("Item");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width2);
		nCol ++;

		width2 = width*2;
		str = _T("Information");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width2);
		nCol ++;
	}

	return true;
}
//-------------------------------------------------------------------------------------//
CParamUni* CProjectParamPaneRepair::GetActParamUni()
{
	return m_ParamActPtr;
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneRepair::SetActParamUni(CParamUni *Ptr)
{
	m_ParamActPtr = Ptr;
}
//-------------------------------------------------------------------------------------//
bool CProjectParamPaneRepair::BuildRepairParamList()
{
	CThisListCtrl_25 &ListCtrl = m_RepairParamListCtrl;
	m_StopParamListBeSelected = true;
	JetAPI::ClearListCtrl(ListCtrl, FALSE);
	m_StopParamListBeSelected = false;
	if ( NULL == m_ProParameterPtr ) { return true; }

	int       intValue=0;
	size_t    i=0, j=0;
	CString   str;
	CString   strValue;	
	CString   strCaption;	
	CString   strDescription;
	int       nItem=0;
	CParamUni    ParamUnit;	
	PROJECT_PARAM_ID ParamID;	
	TProjectParameter *Ptr = m_ProParameterPtr;	
	const int nSubItem = 1;	
	CParamList &ParamList = m_ParamList;

	ParamList.clear();
	
	//統計瑕疵來源模式-ARS	
	ParamUnit = CParamUni();
	str = _T("Statistic Defect From Mode");
	strCaption = LoadMultiLanguageString(str, str);	
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_STATISTIC_DEFECT_FROM_MODE_ARS;
	ParamUnit.SetParamID((UINT)(ParamID));
	ParamUnit.AddSelItem(DEFECT_FROM_AOI, AOIDataDefine.GetDefectFromText(DEFECT_FROM_AOI));
	ParamUnit.AddSelItem(DEFECT_FROM_ARS, AOIDataDefine.GetDefectFromText(DEFECT_FROM_ARS));	
	ParamUnit.SetValue_SEL(Ptr->m_StatisticDefectFromMode_ARS);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//統計依據時間模式
	ParamUnit = CParamUni();
	str = _T("Statistic By Mode");
	strCaption = LoadMultiLanguageString(str, str);	
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_STATISTIC_BY_MODE_ARS;
	ParamUnit.SetParamID((UINT)(ParamID));		
	ParamUnit.AddSelItem(STATISTIC_BY_NONE, AOIDataDefine.GetDisableText());
	ParamUnit.AddSelItem(STATISTIC_BY_TIME, AOIDataDefine.GetTimeText());
	ParamUnit.AddSelItem(STATISTIC_BY_COUNT, AOIDataDefine.GetCountText());	
	ParamUnit.SetValue_SEL(Ptr->m_StatisticByMode_ARS);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//統計依據時間數量
	ParamUnit = CParamUni();
	str = _T("Time Value");
	strCaption = LoadMultiLanguageString(str, str);	
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_STATISTIC_BY_TIME_VALUE_ARS;
	ParamUnit.SetParamID((UINT)(ParamID));	
	ParamUnit.SetValue_INT(Ptr->m_StatisticByTimeValue_ARS, 0);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//統計依據次數數量
	ParamUnit = CParamUni();
	str = _T("Count Value");
	strCaption = LoadMultiLanguageString(str, str);	
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_STATISTIC_BY_COUNT_VALUE_ARS;
	ParamUnit.SetParamID((UINT)(ParamID));	
	ParamUnit.SetValue_INT(Ptr->m_StatisticByCountValue_ARS, 0);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//警報-檢測良率下限-%
	ParamUnit = CParamUni();
	str = _T("Test Yield Min");
	strCaption = LoadMultiLanguageString(str, str);	
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_ALARM_TEST_YIELD_MIN_ARS;	
	ParamUnit.SetParamID((UINT)(ParamID));	
	ParamUnit.SetValue_DBL(Ptr->m_AlaramTestYieldMin_ARS, 2);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);	
	ParamList.push_back(ParamUnit);

	//警報-整板良率下限-%
	ParamUnit = CParamUni();
	str = _T("Panel Yield Min");
	strCaption = LoadMultiLanguageString(str, str);	
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_ALARM_PANEL_YIELD_MIN_ARS;	
	ParamUnit.SetParamID((UINT)(ParamID));	
	ParamUnit.SetValue_DBL(Ptr->m_AlaramPanelYieldMin_ARS, 2);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);	
	ParamList.push_back(ParamUnit);
	
	//警報-單板良率下限-%
	ParamUnit = CParamUni();
	str = _T("Board Yield Min");
	strCaption = LoadMultiLanguageString(str, str);	
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_ALARM_BOARD_YIELD_MIN_ARS;	
	ParamUnit.SetParamID((UINT)(ParamID));	
	ParamUnit.SetValue_DBL(Ptr->m_AlaramBoardYieldMin_ARS, 2);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);	
	ParamList.push_back(ParamUnit);

	//警報-零件良率下限-%
	ParamUnit = CParamUni();
	str = _T("Component Yield Min");
	strCaption = LoadMultiLanguageString(str, str);	
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_ALARM_COMPONENT_YIELD_MIN_ARS;	
	ParamUnit.SetParamID((UINT)(ParamID));	
	ParamUnit.SetValue_DBL(Ptr->m_AlaramComponentYieldMin_ARS, 6);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);	
	ParamList.push_back(ParamUnit);

	//警報-零件單次瑕疵率上限-%	
	ParamUnit = CParamUni();
	str = _T("Component NG Rate Max");
	strCaption = LoadMultiLanguageString(str, str);	
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_ALARM_COMPONENT_DEFECT_RATE_MAX_ARS;	
	ParamUnit.SetParamID((UINT)(ParamID));	
	ParamUnit.SetValue_DBL(Ptr->m_AlaramComponentDefectRateMax_ARS, 6);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);	
	ParamList.push_back(ParamUnit);

	//警報-每個零件瑕疵數上限
	ParamUnit = CParamUni();
	str = _T("Each Component Max Defect Count");
	strCaption = LoadMultiLanguageString(str, str);	
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_ALARM_EACH_COMPONENT_TOTAL_NG_COUNT_ARS;
	ParamUnit.SetParamID((UINT)(ParamID));	
	ParamUnit.SetValue_INT(Ptr->m_AlaramEachComponentTotalNGCount_ARS, 0);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);
	
	//警報-每個零件連續瑕疵次數上限
	ParamUnit = CParamUni();
	str = _T("Each Component Continue Defect Count");
	strCaption = LoadMultiLanguageString(str, str);	
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_ALARM_EACH_COMPONENT_CONTINUE_NG_COUNT_ARS;
	ParamUnit.SetParamID((UINT)(ParamID));	
	ParamUnit.SetValue_INT(Ptr->m_AlaramEachComponentContinueNGCount_ARS, 0);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CProjectParamPaneRepair::BuildRepairParamListWnd()
{
	SetActParamUni(NULL);
	CThisListCtrl_25 &ListCtrl = m_RepairParamListCtrl;	
	m_StopParamListBeSelected = true;
	JetAPI::ClearListCtrl(ListCtrl, FALSE);
	m_StopParamListBeSelected = false;
	if ( NULL == m_ProParameterPtr ) { return true; }

	int           i=0;
	CString       str;
	CString       strIndex;
	CString       strValue;	
	CString       strCaption;	
	int           nItem=0;
	CParamUni    *ParamPtr=NULL;
	const int     nSubItem1 = 1;		
	const int     nSubItem2 = 2;
	const UINT    WndCtrlID = ListCtrl.GetDlgCtrlID();

	BuildRepairParamList();

	const int ParamCount = (int)(m_ParamList.size());

	nItem=0;
	ListCtrl.SetRedraw(FALSE);
	m_StopParamListBeSelected = true;
	for ( i=0; i<ParamCount; i++ )
	{
		ParamPtr = &(m_ParamList[i]);
		if ( NULL == ParamPtr ) { continue; }		
		
		ParamPtr->SetListCtrl(&ListCtrl);
		ParamPtr->SetWndCtrlID(WndCtrlID);
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
	m_StopParamListBeSelected = false;
	ListCtrl.SetRedraw(TRUE);
	return true;	
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneRepair::SetDescriptionText(const CParamUni *Ptr)
{
	UINT CtrlID = PROREPAIR_INFO_EDIT;
	if ( NULL == Ptr )
	{
		CWnd::SetDlgItemText(CtrlID, _T(""));
		return ;
	}
	CWnd::SetDlgItemText(CtrlID, Ptr->GetDesction());
}
//-------------------------------------------------------------------------------------//
bool CProjectParamPaneRepair::ExecItemchangedParamListWnd(CThisListCtrl_25 &ListCtrl, CParamList &ParamList, int nItem)
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
bool CProjectParamPaneRepair::ExecDblclkParamListWnd(CThisListCtrl_25 &ListCtrl, CParamList &ParamList, int nItem, int nSubItem)
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
bool CProjectParamPaneRepair::ExecReleaseParamCtrl()
{
	m_BtnCtrl.ShowWindow(SW_HIDE);
	m_EditCtrl.ShowWindow(SW_HIDE);	
	m_ComboxCtrl.ShowWindow(SW_HIDE);

	m_EditCtrl.SetWindowText(_T(""));
	//m_ParamListCtrl.SetFocus();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectParamPaneRepair::ExecUpdateParamByEdit()
{
	CParamUni   *ParamPtr = GetActParamUni();
	if ( NULL == ParamPtr ) { return true; }	
	PARAM_DATA_TYPE  DataType = ParamPtr->GetDataType();	
	if ( PARAM_DATA_SEL == DataType ) { return true; }
	SetActParamUni(NULL);

	CString ItemText;	
	PROJECT_PARAM_ID ParamID = (PROJECT_PARAM_ID)(ParamPtr->GetParamID());		
	m_EditCtrl.GetWindowText(ItemText);
	if ( ParamPtr->SetNewValue(ItemText) == false )
	{		
		ItemText = ParamPtr->GetParamText();
		m_EditCtrl.SetWindowText(ItemText);
		return false;
	}
	if ( CAOIProject::SetProjectParameterStringByID(ParamID, *m_ProParameterPtr, ItemText) == false )
	{	return false; }

	CThisListCtrl_25 *pListCtrl = (CThisListCtrl_25*)(ParamPtr->GetListCtrl());
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
bool CProjectParamPaneRepair::ExecUpdateParamByCombox()
{
	CParamUni   *ParamPtr = GetActParamUni();
	if ( NULL == ParamPtr ) { return true; }	
	if ( NULL == m_ProParameterPtr ) { return true; }	
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
	
	if ( PROREPAIR_PARAM_LIST_WND == WndCtrlID )
	{
		PROJECT_PARAM_ID ParamID = (PROJECT_PARAM_ID)(ParamPtr->GetParamID());		
		if ( CAOIProject::SetProjectParameterStringByID(ParamID, *m_ProParameterPtr, ItemText) == false )
		{	return false; }
	}	
	
	ItemText = ParamPtr->GetParamText();
	CThisListCtrl_25 *pListCtrl = (CThisListCtrl_25*)(ParamPtr->GetListCtrl());
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
void CProjectParamPaneRepair::OnItemchangedParamListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if ( true == m_StopParamListBeSelected ) { return; }	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) 
	{
		SetDescriptionText(NULL);
		return; 
	}
	DWORD Res = 0;
	DWORD ResOld = pNMListView->uOldState&LVIS_FOCUSED;
	DWORD ResNew = pNMListView->uNewState&LVIS_FOCUSED;	
	if ( 0!=ResOld && 0==ResNew )
	{
		ExecReleaseParamCtrl();
		return;
	}
	Res = pNMListView->uNewState&LVIS_SELECTED;
	if ( Res == 0 ) { return; }	
	Res = pNMListView->uNewState&LVIS_FOCUSED;
	if ( Res == 0 ) { return; }		
	ExecItemchangedParamListWnd(m_RepairParamListCtrl, m_ParamList, nItem);	
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneRepair::OnDblclkParamListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NMITEMACTIVATE *pNMListView = (NMITEMACTIVATE*)pNMHDR;	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) { return; }

	ExecDblclkParamListWnd(m_RepairParamListCtrl, m_ParamList, nItem, nSubItem);
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneRepair::OnKillfocusParamEdit() 
{
	// TODO: Add your control notification handler code here
	if ( m_EditCtrl.IsWindowVisible() == FALSE ) { return; }
	ExecUpdateParamByEdit();
	m_EditCtrl.ShowWindow(SW_HIDE);	
	m_EditCtrl.SetWindowText(_T(""));
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneRepair::OnSelchangeParamCombo() 
{
	// TODO: Add your control notification handler code here
	if ( ExecUpdateParamByCombox() == false )
	{	return; }
	m_ComboxCtrl.ShowWindow(SW_HIDE);	
	JetAPI::ClearCombox(m_ComboxCtrl);	
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneRepair::OnKillfocusParamCombo() 
{
	// TODO: Add your control notification handler code here
	if ( m_ComboxCtrl.IsWindowVisible() == FALSE ) { return; }
	ExecUpdateParamByCombox();
	m_ComboxCtrl.ShowWindow(SW_HIDE);	
	JetAPI::ClearCombox(m_ComboxCtrl);	
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneRepair::OnParamBtn() 
{
	// TODO: Add your control notification handler code here
	m_BtnCtrl.ShowWindow(SW_HIDE);
	CParamUni   *ParamPtr = GetActParamUni();
	if ( NULL == ParamPtr ) { return; }	
	CWnd      *BtnWndPtr = ParamPtr->GetBtnWndPtr();
	if ( NULL == BtnWndPtr ) { return; }

	CString ItemText = ParamPtr->GetParamText();
	PROJECT_PARAM_ID ParamID = (PROJECT_PARAM_ID)(ParamPtr->GetParamID());	

	if ( JetAPI::OpenFolderDialog(this, ItemText) == false ) { return; }
	if ( CAOIProject::SetProjectParameterStringByID(ParamID, *m_ProParameterPtr, ItemText) == false )
	{	return ; }
	ParamPtr->SetNewValue(ItemText);
	SetActParamUni(NULL);
	
	const int nItem = ParamPtr->GetItemIndex();
	const int nSubItem = ParamPtr->GetSubItemIndex();	
	CThisListCtrl_25 *pListCtrl = (CThisListCtrl_25*)(ParamPtr->GetListCtrl());

	if ( NULL != pListCtrl )
	{
		const int ItemCount = pListCtrl->GetItemCount();
		pListCtrl->SetItemText(nItem, nSubItem, ItemText);
		pListCtrl->SetFocus();
	}
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneRepair::OnOK() 
{
	// TODO: Add extra validation here
	return;
	CDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneRepair::OnCancel() 
{
	// TODO: Add extra cleanup here
	return;
	CDialog::OnCancel();
}
//-------------------------------------------------------------------------------------//