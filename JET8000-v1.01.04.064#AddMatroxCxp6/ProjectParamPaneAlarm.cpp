// ProjectParamPaneAlarm.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "ProjectParamPaneAlarm.h"
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
// CProjectParamPaneAlarm dialog
//-------------------------------------------------------------------------------------//
CProjectParamPaneAlarm::CProjectParamPaneAlarm(CWnd* pParent /*=NULL*/)
	: CDialog(CProjectParamPaneAlarm::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProjectParamPaneAlarm)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ProjectPtr = NULL;
	m_ParamActPtr = NULL;	
	m_ProParameterPtr = NULL;	
	m_StopParamListBeSelected = false;
	m_StopAlarmListBeSelected = false;
	m_StopDefectListBeSelected = false;
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneAlarm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProjectParamPaneAlarm)
	DDX_Control(pDX, PROALARM_PARAM_BTN, m_BtnCtrl);
	DDX_Control(pDX, PROALARM_PARAM_EDIT, m_EditCtrl);	
	DDX_Control(pDX, PROALARM_PARAM_COMBO, m_ComboxCtrl);	
	DDX_Control(pDX, PROALARM_ALARM_LIST_WND, m_DefectAlarmListCtrl);
	DDX_Control(pDX, PROALARM_DEFECT_LIST_WND, m_DefectTestListCtrl);	
	DDX_Control(pDX, PROALARM_PARAM_LIST_WND, m_DefectParamListCtrl);	
	
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CProjectParamPaneAlarm, CDialog)
	//{{AFX_MSG_MAP(CProjectParamPaneAlarm)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()	
	ON_NOTIFY(LVN_ITEMCHANGED, PROALARM_PARAM_LIST_WND, OnItemchangedParamListWnd)
	ON_NOTIFY(NM_DBLCLK, PROALARM_PARAM_LIST_WND, OnDblclkParamListWnd)	
	ON_NOTIFY(LVN_ITEMCHANGED, PROALARM_DEFECT_LIST_WND, OnItemchangedDefectListWnd)
	ON_NOTIFY(NM_DBLCLK, PROALARM_DEFECT_LIST_WND, OnDblclkDefectListWnd)	
	ON_NOTIFY(LVN_ITEMCHANGED, PROALARM_ALARM_LIST_WND, OnItemchangedAlarmListWnd)
	ON_NOTIFY(NM_DBLCLK, PROALARM_ALARM_LIST_WND, OnDblclkAlarmListWnd)
	ON_EN_KILLFOCUS(PROALARM_PARAM_EDIT, OnKillfocusParamEdit)
	ON_CBN_SELCHANGE(PROALARM_PARAM_COMBO, OnSelchangeParamCombo)
	ON_CBN_KILLFOCUS(PROALARM_PARAM_COMBO, OnKillfocusParamCombo)
	ON_BN_CLICKED(PROALARM_PARAM_BTN, OnParamBtn)	
	ON_BN_CLICKED(PROALARM_DEFECT_ENABLE_ALL_BTN, OnDefectEnableAllBtn)
	ON_BN_CLICKED(PROALARM_DEFECT_DISABLE_ALL_BTN, OnDefectDisableAllBtn)
	ON_BN_CLICKED(PROALARM_DEFECT_COPY_ALARM_BTN, OnDefectCopyAlarmBtn)
	ON_BN_CLICKED(PROALARM_ALARM_ENABLE_ALL_BTN, OnAlarmEnableAllBtn)
	ON_BN_CLICKED(PROALARM_ALARM_DISABLE_ALL_BTN, OnAlarmDisableAllBtn)
	ON_BN_CLICKED(PROALARM_ALARM_COPY_DEFECT_BTN, OnAlarmCopyDefectBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CProjectParamPaneAlarm message handlers
//-------------------------------------------------------------------------------------//
BOOL CProjectParamPaneAlarm::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SwitchMultiLanguage();
	JetAPI::InitialListCtrl(m_DefectParamListCtrl);	
	JetAPI::InitialListCtrl(m_DefectAlarmListCtrl);	
	JetAPI::InitialListCtrl(m_DefectTestListCtrl);		
	BuildParamListWndHeader();
	if ( CWnd::IsWindowVisible() )
	{
		BuildDefectParamListWnd();
		BuildDefectAlarmListWnd();
		BuildDefectTestListWnd();
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneAlarm::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneAlarm::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if ( m_DefectTestListCtrl.GetSafeHwnd() == NULL ) { return; }
	CWnd *WndPtr = NULL;
	SIZE  WndSize={0};
	RECT  InfoRect={0};
	BOOL  bVisible = CWnd::IsWindowVisible();
	int       TopPosY = 0;	
	POINT     OffsetL;
	POINT     OffsetR;
	const int MarginX = 4;
	const int MarginY = 4;	
	const int ListSizeW = (cx-MarginX-MarginX-MarginX)/2;

	OffsetL.x = OffsetL.y = 0;
	OffsetR.x = OffsetR.y = 0;
	WndPtr = CWnd::GetDlgItem(PROALARM_DEFECT_ENABLE_ALL_BTN);
	if ( NULL!=WndPtr && WndPtr->GetSafeHwnd()!=NULL )
	{
		RECT WndRect={0};
		WndPtr->GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		OffsetL.x = MarginX-WndRect.left;		
		TopPosY = WndRect.bottom;
	}
	WndPtr = CWnd::GetDlgItem(PROALARM_ALARM_ENABLE_ALL_BTN);
	if ( NULL!=WndPtr && WndPtr->GetSafeHwnd()!=NULL )
	{
		RECT WndRect={0};
		WndPtr->GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		OffsetR.x = cx-MarginX-ListSizeW-WndRect.left;		
	}
	OffsetL.x = OffsetL.y = 0;
	OffsetR.x = OffsetR.y = 0;
	//移動控制項
	JetAPI::MoveCtrlWnd(this, PROALARM_DEFECT_LABEL, OffsetL);
	JetAPI::MoveCtrlWnd(this, PROALARM_DEFECT_ENABLE_ALL_BTN, OffsetL);
	JetAPI::MoveCtrlWnd(this, PROALARM_DEFECT_DISABLE_ALL_BTN, OffsetL);
	JetAPI::MoveCtrlWnd(this, PROALARM_DEFECT_COPY_ALARM_BTN, OffsetL);

	JetAPI::MoveCtrlWnd(this, PROALARM_ALARM_LABEL, OffsetR);
	JetAPI::MoveCtrlWnd(this, PROALARM_ALARM_ENABLE_ALL_BTN, OffsetR);
	JetAPI::MoveCtrlWnd(this, PROALARM_ALARM_DISABLE_ALL_BTN, OffsetR);
	JetAPI::MoveCtrlWnd(this, PROALARM_ALARM_COPY_DEFECT_BTN, OffsetR);

	WndPtr = CWnd::GetDlgItem(PROALARM_INFO_EDIT);
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

	if ( m_DefectTestListCtrl.GetSafeHwnd() != NULL )
	{
		RECT WndRect={0};
		m_DefectTestListCtrl.GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		JetAPI::GetRectSize(WndRect, WndSize);
		//WndRect.left = MarginX;
		//WndRect.right = WndRect.left+ListSizeW;
		WndRect.top = TopPosY+MarginY;
		WndRect.bottom = InfoRect.top-MarginY;
		m_DefectTestListCtrl.MoveWindow(&WndRect, FALSE);		
		if ( TRUE == bVisible )
		{	m_DefectTestListCtrl.Invalidate();	}
	}
	if ( m_DefectAlarmListCtrl.GetSafeHwnd() != NULL )
	{
		RECT WndRect={0};		
		m_DefectAlarmListCtrl.GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		JetAPI::GetRectSize(WndRect, WndSize);
		//WndRect.right = cx-MarginX;
		//WndRect.left = WndRect.right-ListSizeW;
		WndRect.top = TopPosY+MarginY;
		WndRect.bottom = InfoRect.top-MarginY;
		m_DefectAlarmListCtrl.MoveWindow(&WndRect, FALSE);		
		if ( TRUE == bVisible )
		{	m_DefectAlarmListCtrl.Invalidate();	}
	}
	if ( m_DefectParamListCtrl.GetSafeHwnd() != NULL )
	{
		RECT WndRect={0};
		m_DefectParamListCtrl.GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		JetAPI::GetRectSize(WndRect, WndSize);
		//WndRect.left = MarginX;
		//WndRect.right = WndRect.left+ListSizeW;
		WndRect.top = TopPosY+MarginY;
		WndRect.bottom = InfoRect.top-MarginY;
		m_DefectParamListCtrl.MoveWindow(&WndRect, FALSE);		
		if ( TRUE == bVisible )
		{	m_DefectParamListCtrl.Invalidate();	}
	}
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneAlarm::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if ( TRUE == bShow )
	{	
		BuildDefectParamListWnd();
		BuildDefectAlarmListWnd();
		BuildDefectTestListWnd();		
	}
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneAlarm::OnItemchangedParamListWnd(NMHDR* pNMHDR, LRESULT* pResult)
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
	DWORD Res=0;
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
	ExecItemchangedParamListWnd(m_DefectParamListCtrl, m_DefectList, nItem);	
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneAlarm::OnDblclkParamListWnd(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here
	NMITEMACTIVATE *pNMListView = (NMITEMACTIVATE*)pNMHDR;	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) { return; }

	ExecDblclkParamListWnd(m_DefectParamListCtrl, m_ParamList, nItem, nSubItem, SETTING_COL);
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneAlarm::OnItemchangedDefectListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if ( true == m_StopDefectListBeSelected ) { return; }	
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
	ExecItemchangedParamListWnd(m_DefectTestListCtrl, m_DefectList, nItem);	
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneAlarm::OnDblclkDefectListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NMITEMACTIVATE *pNMListView = (NMITEMACTIVATE*)pNMHDR;	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) { return; }

	ExecDblclkParamListWnd(m_DefectTestListCtrl, m_DefectList, nItem, nSubItem, 1);
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneAlarm::OnItemchangedAlarmListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if ( true == m_StopAlarmListBeSelected ) { return; }	
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
	ExecItemchangedParamListWnd(m_DefectAlarmListCtrl, m_AlarmList, nItem);	
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneAlarm::OnDblclkAlarmListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NMITEMACTIVATE *pNMListView = (NMITEMACTIVATE*)pNMHDR;	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) { return; }

	ExecDblclkParamListWnd(m_DefectAlarmListCtrl, m_AlarmList, nItem, nSubItem, 1);
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneAlarm::OnKillfocusParamEdit() 
{
	// TODO: Add your control notification handler code here
	if ( m_EditCtrl.IsWindowVisible() == FALSE ) { return; }
	ExecUpdateParamByEdit();
	m_EditCtrl.ShowWindow(SW_HIDE);	
	m_EditCtrl.SetWindowText(_T(""));
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneAlarm::OnSelchangeParamCombo() 
{
	// TODO: Add your control notification handler code here
	if ( ExecUpdateParamByCombox() == false )
	{	return; }
	m_ComboxCtrl.ShowWindow(SW_HIDE);	
	JetAPI::ClearCombox(m_ComboxCtrl);	
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneAlarm::OnKillfocusParamCombo() 
{
	// TODO: Add your control notification handler code here
	if ( m_ComboxCtrl.IsWindowVisible() == FALSE ) { return; }
	ExecUpdateParamByCombox();
	m_ComboxCtrl.ShowWindow(SW_HIDE);	
	JetAPI::ClearCombox(m_ComboxCtrl);	
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneAlarm::OnParamBtn() 
{
	// TODO: Add your control notification handler code here
	m_BtnCtrl.ShowWindow(SW_HIDE);
	CParamUni *ParamPtr = GetActParamUni();
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
	CThisListCtrl_22 *pListCtrl = (CThisListCtrl_22*)(ParamPtr->GetListCtrl());

	if ( NULL != pListCtrl )
	{
		const int ItemCount = pListCtrl->GetItemCount();
		pListCtrl->SetItemText(nItem, nSubItem, ItemText);
		pListCtrl->SetFocus();
	}
}
//-------------------------------------------------------------------------------------//
BOOL CProjectParamPaneAlarm::PreTranslateMessage(MSG* pMsg) 
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
void CProjectParamPaneAlarm::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_PROJECT_PARAM_ALARM_PANE");
	//---------------------------------------------------------------------------------//
	WndID = IDD_PROJECT_PARAM_ALARM_PANE;
	WndKey = _T("IDD_PROJECT_PARAM_ALARM_PANE");
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
	WndID = PROALARM_DEFECT_LABEL;
	WndKey = _T("PROALARM_DEFECT_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROALARM_DEFECT_ENABLE_ALL_BTN;
	WndKey = _T("PROALARM_DEFECT_ENABLE_ALL_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROALARM_DEFECT_DISABLE_ALL_BTN;
	WndKey = _T("PROALARM_DEFECT_DISABLE_ALL_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROALARM_DEFECT_COPY_ALARM_BTN;
	WndKey = _T("PROALARM_DEFECT_COPY_ALARM_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//
	WndID = PROALARM_ALARM_LABEL;
	WndKey = _T("PROALARM_ALARM_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROALARM_ALARM_ENABLE_ALL_BTN;
	WndKey = _T("PROALARM_ALARM_ENABLE_ALL_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROALARM_ALARM_DISABLE_ALL_BTN;
	WndKey = _T("PROALARM_ALARM_DISABLE_ALL_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROALARM_ALARM_COPY_DEFECT_BTN;
	WndKey = _T("PROALARM_ALARM_COPY_DEFECT_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//	
	WndID = PROALARM_PARAM_LABEL;
	WndKey = _T("PROALARM_PARAM_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//	
}
//-------------------------------------------------------------------------------------//
CString CProjectParamPaneAlarm::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_PROJECT_PARAM_ALARM_PANE");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneAlarm::SetProjectParameterPtr(CAOIProject *ProjectPtr, TProjectParameter *Ptr)
{
	this->m_ProjectPtr = ProjectPtr;
	this->m_ProParameterPtr = Ptr;
}
//-------------------------------------------------------------------------------------//
CParamUni* CProjectParamPaneAlarm::GetActParamUni()
{
	return m_ParamActPtr;
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneAlarm::SetActParamUni(CParamUni *Ptr)
{
	m_ParamActPtr = Ptr;
}
//-------------------------------------------------------------------------------------//
bool CProjectParamPaneAlarm::BuildDefectParamList()
{
	CThisListCtrl_22 &ListCtrl = m_DefectParamListCtrl;
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
	
	//瑕疵來源模式
	ParamUnit = CParamUni();
	str = _T("Statistic Defect From Mode");
	strCaption = LoadMultiLanguageString(str, str);	
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_STATISTIC_DEFECT_FROM_MODE;
	ParamUnit.SetParamID((UINT)(ParamID));
	ParamUnit.AddSelItem(DEFECT_FROM_NONE, AOIDataDefine.GetDefectFromText(DEFECT_FROM_NONE));
	ParamUnit.AddSelItem(DEFECT_FROM_AOI, AOIDataDefine.GetDefectFromText(DEFECT_FROM_AOI));
	ParamUnit.AddSelItem(DEFECT_FROM_ARS, AOIDataDefine.GetDefectFromText(DEFECT_FROM_ARS));	
	ParamUnit.SetValue_SEL(Ptr->m_StatisticDefectFromMode);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//統計依據時間模式
	ParamUnit = CParamUni();
	str = _T("Statistic By Mode");
	strCaption = LoadMultiLanguageString(str, str);	
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_STATISTIC_BY_MODE;
	ParamUnit.SetParamID((UINT)(ParamID));		
	ParamUnit.AddSelItem(STATISTIC_BY_NONE, AOIDataDefine.GetDisableText());
	ParamUnit.AddSelItem(STATISTIC_BY_TIME, AOIDataDefine.GetTimeText());
	ParamUnit.AddSelItem(STATISTIC_BY_COUNT, AOIDataDefine.GetCountText());	
	ParamUnit.SetValue_SEL(Ptr->m_StatisticByMode);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//統計依據時間數量
	ParamUnit = CParamUni();
	str = _T("Time Value");
	strCaption = LoadMultiLanguageString(str, str);	
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_STATISTIC_BY_TIME_VALUE;
	ParamUnit.SetParamID((UINT)(ParamID));	
	ParamUnit.SetValue_INT(Ptr->m_StatisticByTimeValue, 0);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//統計依據次數數量
	ParamUnit = CParamUni();
	str = _T("Count Value");
	strCaption = LoadMultiLanguageString(str, str);	
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_STATISTIC_BY_COUNT_VALUE;
	ParamUnit.SetParamID((UINT)(ParamID));	
	ParamUnit.SetValue_INT(Ptr->m_StatisticByCountValue, 0);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//警報-檢測良率下限-%
	ParamUnit = CParamUni();
	str = _T("Test Yield Min");
	strCaption = LoadMultiLanguageString(str, str);	
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_ALARM_TEST_YIELD_MIN;	
	ParamUnit.SetParamID((UINT)(ParamID));	
	ParamUnit.SetValue_DBL(Ptr->m_AlaramTestYieldMin, 2);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);	
	ParamList.push_back(ParamUnit);

	//警報-整板良率下限-%
	ParamUnit = CParamUni();
	str = _T("Panel Yield Min");
	strCaption = LoadMultiLanguageString(str, str);	
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_ALARM_PANEL_YIELD_MIN;	
	ParamUnit.SetParamID((UINT)(ParamID));	
	ParamUnit.SetValue_DBL(Ptr->m_AlaramPanelYieldMin, 2);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);	
	ParamList.push_back(ParamUnit);
	
	//警報-單板良率下限-%
	ParamUnit = CParamUni();
	str = _T("Board Yield Min");
	strCaption = LoadMultiLanguageString(str, str);	
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_ALARM_BOARD_YIELD_MIN;	
	ParamUnit.SetParamID((UINT)(ParamID));	
	ParamUnit.SetValue_DBL(Ptr->m_AlaramBoardYieldMin, 2);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);	
	ParamList.push_back(ParamUnit);

	//警報-零件良率下限-%
	ParamUnit = CParamUni();
	str = _T("Component Yield Min");
	strCaption = LoadMultiLanguageString(str, str);	
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_ALARM_COMPONENT_YIELD_MIN;	
	ParamUnit.SetParamID((UINT)(ParamID));	
	ParamUnit.SetValue_DBL(Ptr->m_AlaramComponentYieldMin, 6);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);	
	ParamList.push_back(ParamUnit);

	//警報-零件單次瑕疵率上限-%	
	ParamUnit = CParamUni();
	str = _T("Component NG Rate Max");
	strCaption = LoadMultiLanguageString(str, str);	
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_ALARM_COMPONENT_DEFECT_RATE_MAX;	
	ParamUnit.SetParamID((UINT)(ParamID));	
	ParamUnit.SetValue_DBL(Ptr->m_AlaramComponentDefectRateMax, 6);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);	
	ParamList.push_back(ParamUnit);

	//警報-每個零件瑕疵數上限
	ParamUnit = CParamUni();
	str = _T("Each Component Max Defect Count");
	strCaption = LoadMultiLanguageString(str, str);	
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_ALARM_EACH_COMPONENT_TOTAL_NG_COUNT;
	ParamUnit.SetParamID((UINT)(ParamID));	
	ParamUnit.SetValue_INT(Ptr->m_AlaramEachComponentTotalNGCount, 0);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);
	
	//警報-每個零件連續瑕疵次數上限
	ParamUnit = CParamUni();
	str = _T("Each Component Continue Defect Count");
	strCaption = LoadMultiLanguageString(str, str);	
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_ALARM_EACH_COMPONENT_CONTINUE_NG_COUNT;
	ParamUnit.SetParamID((UINT)(ParamID));	
	ParamUnit.SetValue_INT(Ptr->m_AlaramEachComponentContinueNGCount, 0);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//警報鎖機模式			
	ParamUnit = CParamUni();
	str = _T("Alarm Lock Mode");
	strCaption = LoadMultiLanguageString(str, str);	
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_ALARM_LOCK_MODE;
	ParamUnit.SetParamID((UINT)(ParamID));			
	ParamUnit.AddSelItem(ALARM_LOCK_AOI, AOIDataDefine.GetAlarmLockText(ALARM_LOCK_AOI));
	ParamUnit.AddSelItem(ALARM_LOCK_ARS, AOIDataDefine.GetAlarmLockText(ALARM_LOCK_ARS));	
	ParamUnit.SetValue_SEL(Ptr->m_AlarmLockMode);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectParamPaneAlarm::BuildDefectParamListWnd()
{
	SetActParamUni(NULL);
	CThisListCtrl_22 &ListCtrl = m_DefectParamListCtrl;	
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

	BuildDefectParamList();

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
bool CProjectParamPaneAlarm::AddDefectAlarmItem(WND_DEFECT_ID WndDefectID, CWndDefectItem &DefectItem, CParamList &ParamList)
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
bool CProjectParamPaneAlarm::BuildDefectAlarmList()
{
	CThisListCtrl_22 &ListCtrl = m_DefectAlarmListCtrl;
	m_StopAlarmListBeSelected = true;
	JetAPI::ClearListCtrl(ListCtrl, FALSE);
	m_StopAlarmListBeSelected = false;
	if ( NULL == m_ProParameterPtr ) { return true; }

	int       intValue=0;
	CString   str;
	CString   strValue;	
	CString   strCaption;	
	CString   strDescription;	
	int       nItem=0;
	int       WndDefectEnable=0;
	CParamUni    ParamUnit;	
	WND_DEFECT_ID WndDefectID;		
	CWndDefectItem  DefectAlarmItem = m_ProParameterPtr->m_DefectAlarmItem;	
	const int nSubItem = 1;	
	CParamList &ParamList = m_AlarmList;

	ParamList.clear();

	const std::vector<WND_DEFECT_ID> &List=AOIDataCollect.GetWndDefectIDList();
	const size_t Count=List.size();
	for ( size_t i=0; i<Count; i++ )
	{	AddDefectAlarmItem(List[i], DefectAlarmItem, ParamList);	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectParamPaneAlarm::BuildDefectAlarmListWnd()
{
	SetActParamUni(NULL);
	CThisListCtrl_22 &ListCtrl = m_DefectAlarmListCtrl;	
	m_StopAlarmListBeSelected = true;
	JetAPI::ClearListCtrl(ListCtrl, FALSE);
	m_StopAlarmListBeSelected = false;
	if ( NULL == m_ProParameterPtr ) { return true; }

	int           i=0;
	CString       str;
	CString       strValue;	
	CString       strCaption;	
	int           nItem=0;
	CParamUni    *ParamPtr=NULL;
	const int     nSubItem = 1;
	const UINT    WndCtrlID = ListCtrl.GetDlgCtrlID();

	BuildDefectAlarmList();

	const int ParamCount = (int)(m_AlarmList.size());

	nItem=0;
	ListCtrl.SetRedraw(FALSE);
	m_StopAlarmListBeSelected = true;
	for ( i=0; i<ParamCount; i++ )
	{
		ParamPtr = &(m_AlarmList[i]);
		if ( NULL == ParamPtr ) { continue; }		
		
		ParamPtr->SetListCtrl(&ListCtrl);
		ParamPtr->SetWndCtrlID(WndCtrlID);
		ParamPtr->SetItemIndex(nItem);
		ParamPtr->SetSubItemIndex(nSubItem);		

		strCaption = ParamPtr->GetCaption();
		strValue = ParamPtr->GetParamText();
		ListCtrl.InsertItem(nItem, strCaption);
		ListCtrl.SetItemData(nItem, i);
		ListCtrl.SetItemText(nItem, nSubItem, strCaption);
		ListCtrl.SetItemText(nItem, nSubItem, strValue);
		nItem ++;
	}	
	m_StopAlarmListBeSelected = false;
	ListCtrl.SetRedraw(TRUE);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectParamPaneAlarm::AddDefectTestItem(WND_DEFECT_ID WndDefectID, CWndDefectItem &DefectItem, CParamList &ParamList)
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
	strValue = AOIDataDefine.GetWndDefectItemModeText(WND_DEFECT_ITEM_NO_SHOW);	ParamUnit.AddSelItem(WND_DEFECT_ITEM_NO_SHOW, strValue);	
	ParamUnit.SetValue_SEL(WndDefectEnable);	
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectParamPaneAlarm::BuildDefectTestList()
{
	CThisListCtrl_22 &ListCtrl = m_DefectTestListCtrl;
	m_StopDefectListBeSelected = true;
	JetAPI::ClearListCtrl(ListCtrl, FALSE);	
	m_StopDefectListBeSelected = false;
	if ( NULL == m_ProParameterPtr ) { return true; }

	int       intValue=0;
	CString   str;
	CString   strValue;	
	CString   strCaption;	
	CString   strDescription;
	int       nItem=0;
	int       WndDefectEnable=0;
	WND_DEFECT_ID WndDefectID;
	CParamUni    ParamUnit;				
	CWndDefectItem  DefectTestItem = m_ProParameterPtr->m_DefectTestItem;		
	const int nSubItem = 1;		
	CParamList &ParamList = m_DefectList;

	ParamList.clear();	
	
	const std::vector<WND_DEFECT_ID> &List=AOIDataCollect.GetWndDefectIDList();
	const size_t Count=List.size();
	for ( size_t i=0; i<Count; i++ )
	{	AddDefectTestItem(List[i], DefectTestItem, ParamList);	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectParamPaneAlarm::BuildDefectTestListWnd()
{
	SetActParamUni(NULL);
	CThisListCtrl_22 &ListCtrl = m_DefectTestListCtrl;	
	m_StopDefectListBeSelected = true;
	JetAPI::ClearListCtrl(ListCtrl, FALSE);
	m_StopDefectListBeSelected = false;
	if ( NULL == m_ProParameterPtr ) { return true; }

	int           i=0;
	CString       str;
	CString       strValue;	
	CString       strCaption;	
	int           nItem=0;
	CParamUni    *ParamPtr=NULL;
	const int     nSubItem = 1;			
	const UINT    WndCtrlID = ListCtrl.GetDlgCtrlID();

	BuildDefectTestList();

	const int ParamCount = (int)(m_DefectList.size());

	nItem=0;
	ListCtrl.SetRedraw(FALSE);
	m_StopDefectListBeSelected = true;
	for ( i=0; i<ParamCount; i++ )
	{
		ParamPtr = &(m_DefectList[i]);
		if ( NULL == ParamPtr ) { continue; }		
		
		ParamPtr->SetListCtrl(&ListCtrl);
		ParamPtr->SetWndCtrlID(WndCtrlID);
		ParamPtr->SetItemIndex(nItem);
		ParamPtr->SetSubItemIndex(nSubItem);		

		strCaption = ParamPtr->GetCaption();
		strValue = ParamPtr->GetParamText();
		ListCtrl.InsertItem(nItem, strCaption);
		ListCtrl.SetItemData(nItem, i);
		ListCtrl.SetItemText(nItem, nSubItem, strCaption);
		ListCtrl.SetItemText(nItem, nSubItem, strValue);
		nItem ++;
	}	
	m_StopDefectListBeSelected = false;
	ListCtrl.SetRedraw(TRUE);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectParamPaneAlarm::BuildParamListWndHeader()
{
	CString str;
	int   nCol = 0;
	int width  = 64;
	int width2 = 64;
	RECT      Rect;	
	const int Align = LVCFMT_LEFT;//LVCFMT_CENTER;LVCFMT_RIGHT, LVCFMT_LEFT

	{
		CThisListCtrl_22 &ListCtrl = m_DefectParamListCtrl;

		ListCtrl.GetClientRect(&Rect);
		width = (Rect.right-Rect.left-8)/4;
		width2 = 48;
		str = AOIDataDefine.GetIndexText();	
		ListCtrl.InsertColumn(nCol, str, Align, width2);
		nCol ++;

		str = _T("Item");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width*2);
		nCol ++;

		width2 = width;
		str = _T("Information");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width2);
		nCol ++;
	}

	{
		CThisListCtrl_22 &ListCtrl = m_DefectTestListCtrl;

		ListCtrl.GetClientRect(&Rect);
		width = (Rect.right-Rect.left-8)/3;
		str = _T("Item");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width*2);
		nCol ++;

		width2 = width;
		str = _T("Information");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width2);
		nCol ++;
	}

	{
		CThisListCtrl_22 &ListCtrl = m_DefectAlarmListCtrl;

		ListCtrl.GetClientRect(&Rect);
		width = (Rect.right-Rect.left-8)/3;
		str = _T("Item");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width*2);
		nCol ++;

		width2 = width;
		str = _T("Information");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width2);
		nCol ++;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneAlarm::SetDescriptionText(const CParamUni *Ptr)
{
	UINT CtrlID = PROALARM_INFO_EDIT;
	if ( NULL == Ptr )
	{
		CWnd::SetDlgItemText(CtrlID, _T(""));
		return ;
	}
	CWnd::SetDlgItemText(CtrlID, Ptr->GetDesction());
}
//-------------------------------------------------------------------------------------//
bool CProjectParamPaneAlarm::ExecItemchangedParamListWnd(CThisListCtrl_22 &ListCtrl, CParamList &ParamList, int nItem)
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
bool CProjectParamPaneAlarm::ExecDblclkParamListWnd(CThisListCtrl_22 &ListCtrl, CParamList &ParamList, int nItem, int nSubItem, int SetCol)
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
bool CProjectParamPaneAlarm::ExecReleaseParamCtrl()
{
	m_BtnCtrl.ShowWindow(SW_HIDE);
	m_EditCtrl.ShowWindow(SW_HIDE);	
	m_ComboxCtrl.ShowWindow(SW_HIDE);

	m_EditCtrl.SetWindowText(_T(""));
	//m_ParamListCtrl.SetFocus();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectParamPaneAlarm::ExecUpdateParamByEdit()
{
	CParamUni *ParamPtr = GetActParamUni();
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

	CThisListCtrl_22 *pListCtrl = (CThisListCtrl_22*)(ParamPtr->GetListCtrl());
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
bool CProjectParamPaneAlarm::ExecUpdateParamByCombox()
{
	CParamUni *ParamPtr = GetActParamUni();
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
	if ( PROALARM_DEFECT_LIST_WND == WndCtrlID )
	{
		WND_DEFECT_ID DefectID = (WND_DEFECT_ID)(ParamPtr->GetParamID());
		m_ProParameterPtr->m_DefectTestItem.SetItemCount(DefectID, Param);
	}
	if ( PROALARM_ALARM_LIST_WND == WndCtrlID )
	{
		WND_DEFECT_ID DefectID = (WND_DEFECT_ID)(ParamPtr->GetParamID());
		m_ProParameterPtr->m_DefectAlarmItem.SetItemCount(DefectID, Param);		
	}	
	if ( PROALARM_PARAM_LIST_WND == WndCtrlID )
	{
		PROJECT_PARAM_ID ParamID = (PROJECT_PARAM_ID)(ParamPtr->GetParamID());		
		if ( CAOIProject::SetProjectParameterStringByID(ParamID, *m_ProParameterPtr, ItemText) == false )
		{	return false; }
	}	
	
	ItemText = ParamPtr->GetParamText();
	CThisListCtrl_22 *pListCtrl = (CThisListCtrl_22*)(ParamPtr->GetListCtrl());
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
void CProjectParamPaneAlarm::OnDefectEnableAllBtn() 
{
	// TODO: Add your control notification handler code here
	if ( NULL == m_ProParameterPtr ) { return ; }
	CString str;
	str = _T("Do you want to enable all defect test?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO ) 
	{	return; }
	const int nValue = WND_DEFECT_ITEM_ENABLE;
	m_ProParameterPtr->m_DefectTestItem.SetAll(nValue);
	BuildDefectTestListWnd();
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneAlarm::OnDefectDisableAllBtn() 
{
	// TODO: Add your control notification handler code here
	if ( NULL == m_ProParameterPtr ) { return ; }
	CString str;
	str = _T("Do you want to disable all defect test?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO ) 
	{	return; }
	const int nValue = WND_DEFECT_ITEM_DISABLE;
	m_ProParameterPtr->m_DefectTestItem.SetAll(nValue);
	BuildDefectTestListWnd();
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneAlarm::OnDefectCopyAlarmBtn() 
{
	// TODO: Add your control notification handler code here
	if ( NULL == m_ProParameterPtr ) { return ; }
	CString str;
	str = _T("Do you want to copy defect test from defect alarm?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO ) 
	{	return; }
	m_ProParameterPtr->m_DefectTestItem = m_ProParameterPtr->m_DefectAlarmItem;
	BuildDefectTestListWnd();
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneAlarm::OnAlarmEnableAllBtn() 
{
	// TODO: Add your control notification handler code here
	if ( NULL == m_ProParameterPtr ) { return ; }
	CString str;
	str = _T("Do you want to enable all defect alarm?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO ) 
	{	return; }
	const int nValue = WND_DEFECT_ITEM_ENABLE;
	m_ProParameterPtr->m_DefectAlarmItem.SetAll(nValue);
	BuildDefectAlarmListWnd();
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneAlarm::OnAlarmDisableAllBtn() 
{
	// TODO: Add your control notification handler code here
	if ( NULL == m_ProParameterPtr ) { return ; }
	CString str;
	str = _T("Do you want to disable all defect alarm?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO ) 
	{	return; }
	const int nValue = WND_DEFECT_ITEM_DISABLE;
	m_ProParameterPtr->m_DefectAlarmItem.SetAll(nValue);
	BuildDefectAlarmListWnd();
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneAlarm::OnAlarmCopyDefectBtn() 
{
	// TODO: Add your control notification handler code here
	CAOIProject *ProjectPtr = m_ProjectPtr;
	if ( NULL == ProjectPtr ) { return ; }
	if ( NULL == m_ProParameterPtr ) { return ; }
	CString str;
	str = _T("Do you want to copy defect alarm from defect test?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO ) 
	{	return; }
	m_ProParameterPtr->m_DefectAlarmItem = m_ProParameterPtr->m_DefectTestItem;
	ProjectPtr->ModifyProjectWndDefectItemAlarm(m_ProParameterPtr->m_DefectAlarmItem);
	BuildDefectAlarmListWnd();
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneAlarm::OnOK() 
{
	// TODO: Add extra validation here
	return;
	CDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneAlarm::OnCancel() 
{
	// TODO: Add extra cleanup here
	return;
	CDialog::OnCancel();
}
//-------------------------------------------------------------------------------------//
