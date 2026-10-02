// ProjectParamBasicPane.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "ProjectParamBasicPane.h"
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
// CProjectParamPaneBasic dialog
//-------------------------------------------------------------------------------------//
CProjectParamPaneBasic::CProjectParamPaneBasic(CWnd* pParent /*=NULL*/)
	: CDialog(CProjectParamPaneBasic::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProjectParamPaneBasic)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ProjectPtr = NULL;
	m_ParamActPtr = NULL;
	m_ProParameterPtr = NULL;	
	m_StopParamListBeSelected = false;
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneBasic::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProjectParamPaneBasic)	
	DDX_Control(pDX, PROBASIC_LANE_WIDTH_BTN, m_SetLaneWidthBtn);
	DDX_Control(pDX, PROBASIC_FOCUS_POS_BTN, m_SetFocusBtn);
	DDX_Control(pDX, PROBASIC_PARAM_EDIT, m_EditCtrl);
	DDX_Control(pDX, PROBASIC_PARAM_BTN, m_SetFolderBtn);
	DDX_Control(pDX, PROBASIC_PARAM_COMBO, m_ComboxCtrl);	
	DDX_Control(pDX, PROBASIC_PARAM_LIST_WND, m_ParamListCtrl);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CProjectParamPaneBasic, CDialog)
	//{{AFX_MSG_MAP(CProjectParamPaneBasic)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_NOTIFY(LVN_ITEMCHANGED, PROBASIC_PARAM_LIST_WND, OnItemchangedParamListWnd)
	ON_NOTIFY(NM_DBLCLK, PROBASIC_PARAM_LIST_WND, OnDblclkParamListWnd)
	ON_EN_KILLFOCUS(PROBASIC_PARAM_EDIT, OnKillfocusParamEdit)
	ON_CBN_SELCHANGE(PROBASIC_PARAM_COMBO, OnSelchangeParamCombo)
	ON_CBN_KILLFOCUS(PROBASIC_PARAM_COMBO, OnKillfocusParamCombo)
	ON_BN_CLICKED(PROBASIC_PARAM_BTN, OnParamBtn)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(PROBASIC_FOCUS_POS_BTN, OnFocusPosBtn)
	ON_BN_CLICKED(PROBASIC_LANE_WIDTH_BTN, OnLaneWidthBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CProjectParamPaneBasic message handlers
//-------------------------------------------------------------------------------------//
BOOL CProjectParamPaneBasic::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SwitchMultiLanguage();
	JetAPI::InitialListCtrl(m_ParamListCtrl);	
	BuildParamListWndHeader();
	if ( CWnd::IsWindowVisible() )
	{	BuildParamListWnd(); }
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneBasic::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here	
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneBasic::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if ( m_ParamListCtrl.GetSafeHwnd() == NULL ) { return; }
	CWnd *WndPtr = NULL;
	SIZE  WndSize={0};
	RECT  InfoRect={0};
	BOOL  bVisible = CWnd::IsWindowVisible();
	const int MarginX = 4;
	const int MarginY = 4;

	WndPtr = CWnd::GetDlgItem(PROBASIC_INFO_EDIT);
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

	if ( m_ParamListCtrl.GetSafeHwnd() != NULL )
	{
		RECT WndRect={0};		
		WndRect.left = MarginX;
		WndRect.right = cx-MarginX;
		WndRect.top = MarginY;
		WndRect.bottom = InfoRect.top-MarginY;
		m_ParamListCtrl.MoveWindow(&WndRect, FALSE);		
		if ( TRUE == bVisible )
		{	m_ParamListCtrl.Invalidate();	}
	}
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneBasic::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_PROJECT_PARAM_BASIC_PANE");
	//---------------------------------------------------------------------------------//
	WndID = IDD_PROJECT_PARAM_BASIC_PANE;
	WndKey = _T("IDD_PROJECT_PARAM_BASIC_PANE");
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
CString CProjectParamPaneBasic::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_PROJECT_PARAM_BASIC_PANE");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneBasic::SetProjectParameterPtr(CAOIProject *ProjectPtr, TProjectParameter *Ptr)
{
	this->m_ProjectPtr = ProjectPtr;
	this->m_ProParameterPtr = Ptr;
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneBasic::OnOK() 
{
	// TODO: Add extra validation here
	return;
	CDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneBasic::OnCancel() 
{
	// TODO: Add extra cleanup here
	return ;
	CDialog::OnCancel();
}
//-------------------------------------------------------------------------------------//
BOOL CProjectParamPaneBasic::PreTranslateMessage(MSG* pMsg) 
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
			return TRUE;
			break;
		}
		break;
	}
	return CDialog::PreTranslateMessage(pMsg);
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneBasic::OnItemchangedParamListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
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
	ExecItemchangedParamListWnd(m_ParamListCtrl, nItem);	
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneBasic::OnDblclkParamListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NMITEMACTIVATE *pNMListView = (NMITEMACTIVATE*)pNMHDR;	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) { return; }

	ExecDblclkParamListWnd(m_ParamListCtrl, nItem, nSubItem);
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneBasic::OnKillfocusParamEdit() 
{
	// TODO: Add your control notification handler code here
	if ( m_EditCtrl.IsWindowVisible() == FALSE ) { return; }
	ExecUpdateParamByEdit();
	m_EditCtrl.ShowWindow(SW_HIDE);	
	m_EditCtrl.SetWindowText(_T(""));
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneBasic::OnSelchangeParamCombo() 
{
	// TODO: Add your control notification handler code here
	if ( ExecUpdateParamByCombox() == false )
	{	return; }
	m_ComboxCtrl.ShowWindow(SW_HIDE);	
	JetAPI::ClearCombox(m_ComboxCtrl);	
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneBasic::OnKillfocusParamCombo() 
{
	// TODO: Add your control notification handler code here
	if ( m_ComboxCtrl.IsWindowVisible() == FALSE ) { return; }
	ExecUpdateParamByCombox();
	m_ComboxCtrl.ShowWindow(SW_HIDE);	
	JetAPI::ClearCombox(m_ComboxCtrl);	
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneBasic::OnParamBtn() 
{
	// TODO: Add your control notification handler code here
	HideCtrlBtn();
	CParamUni   *ParamPtr = GetActParamUni();
	if ( NULL == ParamPtr ) { return ; }	
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
	CThisListCtrl_24 *pListCtrl = (CThisListCtrl_24*)(ParamPtr->GetListCtrl());

	if ( NULL != pListCtrl )
	{
		const int ItemCount = pListCtrl->GetItemCount();
		pListCtrl->SetItemText(nItem, nSubItem, ItemText);
		pListCtrl->SetFocus();
	}
	return ;
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneBasic::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if ( TRUE == bShow )
	{	
		BuildParamListWnd();	
	}
}
//-------------------------------------------------------------------------------------//
CParamUni* CProjectParamPaneBasic::GetActParamUni()
{
	return m_ParamActPtr;
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneBasic::SetActParamUni(CParamUni *Ptr)
{
	m_ParamActPtr = Ptr;
}
//-------------------------------------------------------------------------------------//
bool CProjectParamPaneBasic::BuildParamList()
{
	CThisListCtrl_24 &ListCtrl = m_ParamListCtrl;
	m_StopParamListBeSelected = true;
	JetAPI::ClearListCtrl(ListCtrl, FALSE);
	m_StopParamListBeSelected = false;
	if ( NULL == m_ProParameterPtr ) { return true; }
	CAOIProject *ProjectPtr = m_ProjectPtr;
	if ( NULL == ProjectPtr ) { return true; }

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
	const size_t FrameCount = ProjectPtr->GetProjectFrameUniqueIDCount();
	const int nSubItem = 1;	
	CParamList &ParamList = m_ParamList;
	
	ParamList.clear();

	//專案機種名稱
	ParamUnit = CParamUni();
	str = _T("Module Name");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_MODULE_NAME;	
	ParamUnit.SetParamID((UINT)(ParamID));	
	ParamUnit.SetValue_STR(Ptr->m_ProjectModuleName.c_str());
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//專案版本
	ParamUnit = CParamUni();
	str = _T("Version");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_VERSION;	
	ParamUnit.SetParamID((UINT)(ParamID));	
	ParamUnit.SetValue_STR(Ptr->m_ProjectVersion.c_str());
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//專案產品正背面
	ParamUnit = CParamUni();
	str = _T("Panel Side");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_PANEL_SIDE;	
	ParamUnit.SetParamID((UINT)(ParamID));	
	strValue = AOIDataDefine.GetPanelSideModeText(PANEL_SIDE_TOP); ParamUnit.AddSelItem(PANEL_SIDE_TOP, strValue);
	strValue = AOIDataDefine.GetPanelSideModeText(PANEL_SIDE_BOTTOM); ParamUnit.AddSelItem(PANEL_SIDE_BOTTOM, strValue);
	strValue = AOIDataDefine.GetPanelSideModeText(PANEL_SIDE_HYBRID); ParamUnit.AddSelItem(PANEL_SIDE_HYBRID, strValue);	
	ParamUnit.SetValue_SEL(Ptr->m_ProjectPanelSideMode);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//專案工單編號
	ParamUnit = CParamUni();
	str = _T("Work Number");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_WORDK_NUMBER;	
	ParamUnit.SetParamID((UINT)(ParamID));	
	ParamUnit.SetValue_STR(Ptr->m_ProjectWorkNumber.c_str());
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//專案開檔碼	
	ParamUnit = CParamUni();
	str = _T("Open Code");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_OPEN_CODE;	
	ParamUnit.SetParamID((UINT)(ParamID));	
	ParamUnit.SetValue_STR(Ptr->m_ProjectOpenCode.c_str());
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);		

	//專案尺寸寬度-mm
	ParamUnit = CParamUni();
	str = _T("Test Size Width");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_TEST_SIZE_WIDTH;	
	ParamUnit.SetParamID((UINT)(ParamID));	
	ParamUnit.SetValue_DBL(Ptr->m_TestSizeWidth, 2, 0);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//專案尺寸高度-mm
	ParamUnit = CParamUni();
	str = _T("Test Size Height");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_TEST_SIZE_HEIGHT;	
	ParamUnit.SetParamID((UINT)(ParamID));	
	ParamUnit.SetValue_DBL(Ptr->m_TestSizeHeight, 2, 0);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//焦點偏差-um
	ParamUnit = CParamUni();
	str = _T("Focus Offset");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_FOCUS_OFFSET;
	ParamUnit.SetParamID((UINT)(ParamID));	
	ParamUnit.SetValue_DBL(Ptr->m_ProjectFocusOffset, 0);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamUnit.SetBtnWndPtr(&m_SetFocusBtn);	
	ParamList.push_back(ParamUnit);

	//專案軌道寬度-mm
	ParamUnit = CParamUni();
	str = _T("Lane Width");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_LANE_WIDTH;	
	ParamUnit.SetParamID((UINT)(ParamID));	
	ParamUnit.SetValue_DBL(Ptr->m_ProjectLaneWidth, 2);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamUnit.SetBtnWndPtr(&m_SetLaneWidthBtn);	
	ParamList.push_back(ParamUnit);

	//專案X板確認比例
	ParamUnit = CParamUni();
	str = _T("X-Board Check Ratio");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_XBOARD_CHECK_RATIO;	
	ParamUnit.SetParamID((UINT)(ParamID));	
	ParamUnit.SetValue_DBL(Ptr->m_ProjectXBoardCheckRatio, 4, 0, 100);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);	
	ParamList.push_back(ParamUnit);

	//高度轉灰階模式	
	ParamUnit = CParamUni();
	str = _T("Space to Gray Ratio Mode");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_SPACE_TO_GRAY_RATIO_MODE;	
	ParamUnit.SetParamID((UINT)(ParamID));	

	intValue = 0;
	const size_t RatioCount = AOIDataDefine.GetProjectSpaceToGrayRatioModeCount();
	for ( i=0; i<RatioCount; i++ )
	{
		intValue += 5;
		strValue.Format(_T("%d"), intValue);		
		ParamUnit.AddSelItem(intValue, strValue);
	}	
	ParamUnit.SetValue_SEL(Ptr->m_SpaceToGrayRatioMode);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);	
	ParamList.push_back(ParamUnit);
	
	//DLP-LED顏色
	ParamUnit = CParamUni();
	str = _T("3D LED Color Mode");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_DLP_LED_COLOR;	
	ParamUnit.SetParamID((UINT)(ParamID));
	strValue = AOIDataDefine.GetColorText_Red(); ParamUnit.AddSelItem(DLP_LED_COLOR_RED, strValue);
	strValue = AOIDataDefine.GetColorText_Green(); ParamUnit.AddSelItem(DLP_LED_COLOR_GREEN, strValue);
	strValue = AOIDataDefine.GetColorText_Blue(); ParamUnit.AddSelItem(DLP_LED_COLOR_BLUE, strValue);
	strValue = AOIDataDefine.GetColorText_White(); ParamUnit.AddSelItem(DLP_LED_COLOR_WHITE, strValue);	
	ParamUnit.SetValue_SEL(Ptr->m_ProjectDlpLedColor);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);	
	ParamList.push_back(ParamUnit);

	//視野寬度使用比例
	ParamUnit = CParamUni();
	str = _T("Field Width Size Mode");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_FIELD_SIZE_MODE_W;	
	ParamUnit.SetParamID((UINT)(ParamID));
	ParamUnit.AddSelItem(FIELD_SIZE_010, _T(" 10 %"));
	ParamUnit.AddSelItem(FIELD_SIZE_020, _T(" 20 %"));
	ParamUnit.AddSelItem(FIELD_SIZE_030, _T(" 30 %"));
	ParamUnit.AddSelItem(FIELD_SIZE_040, _T(" 40 %"));
	ParamUnit.AddSelItem(FIELD_SIZE_050, _T(" 50 %"));
	ParamUnit.AddSelItem(FIELD_SIZE_060, _T(" 60 %"));
	ParamUnit.AddSelItem(FIELD_SIZE_070, _T(" 70 %"));
	ParamUnit.AddSelItem(FIELD_SIZE_080, _T(" 80 %"));
	ParamUnit.AddSelItem(FIELD_SIZE_090, _T(" 90 %"));
	ParamUnit.AddSelItem(FIELD_SIZE_100, _T("100 %"));
	ParamUnit.AddSelItem(FIELD_SIZE_DOT, _T("Dot"));	
	ParamUnit.SetValue_SEL(Ptr->m_ProjectFieldSizeModeW);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);	
	ParamList.push_back(ParamUnit);

	//視野長度使用比例
	ParamUnit = CParamUni();
	str = _T("Field Height Size Mode");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_FIELD_SIZE_MODE_H;
	ParamUnit.SetParamID((UINT)(ParamID));
	ParamUnit.AddSelItem(FIELD_SIZE_010, _T(" 10 %"));
	ParamUnit.AddSelItem(FIELD_SIZE_020, _T(" 20 %"));
	ParamUnit.AddSelItem(FIELD_SIZE_030, _T(" 30 %"));
	ParamUnit.AddSelItem(FIELD_SIZE_040, _T(" 40 %"));
	ParamUnit.AddSelItem(FIELD_SIZE_050, _T(" 50 %"));
	ParamUnit.AddSelItem(FIELD_SIZE_060, _T(" 60 %"));
	ParamUnit.AddSelItem(FIELD_SIZE_070, _T(" 70 %"));
	ParamUnit.AddSelItem(FIELD_SIZE_080, _T(" 80 %"));
	ParamUnit.AddSelItem(FIELD_SIZE_090, _T(" 90 %"));
	ParamUnit.AddSelItem(FIELD_SIZE_100, _T("100 %"));
	ParamUnit.AddSelItem(FIELD_SIZE_DOT, _T("Dot"));	
	ParamUnit.SetValue_SEL(Ptr->m_ProjectFieldSizeModeH);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);	
	ParamList.push_back(ParamUnit);

	//整板定位點跳過檢測數量
	ParamUnit = CParamUni();
	str = _T("Panel Fd NG Skip Count");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_PANEL_FD_NG_SKIP_COUNT;
	ParamUnit.SetParamID((UINT)(ParamID));	
	for ( i=0; i<PANEL_MAX_FD_COUNT; i++ )
	{	strValue.Format(_T("%d"), i+1);	ParamUnit.AddSelItem(i+1, strValue);	}
	ParamUnit.SetValue_SEL(Ptr->m_PanelFdNGSkipCount);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//整板定位點異常處理模式
	ParamUnit = CParamUni();
	str = _T("Panel Fd NG Handle Mode");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_PANEL_FD_NG_HANDLE_MODE;
	ParamUnit.SetParamID((UINT)(ParamID));	
	strValue = AOIDataDefine.GetFdNGHandleModeText(FD_NG_HANDLE_NONE);	ParamUnit.AddSelItem(FD_NG_HANDLE_NONE, strValue);	
	strValue = AOIDataDefine.GetFdNGHandleModeText(FD_NG_HANDLE_PASS);	ParamUnit.AddSelItem(FD_NG_HANDLE_PASS, strValue);
	strValue = AOIDataDefine.GetFdNGHandleModeText(FD_NG_HANDLE_STOP);	ParamUnit.AddSelItem(FD_NG_HANDLE_STOP, strValue);	
	strValue = AOIDataDefine.GetFdNGHandleModeText(FD_NG_HANDLE_XBOARD);	ParamUnit.AddSelItem(FD_NG_HANDLE_XBOARD, strValue);	
	ParamUnit.SetValue_SEL(Ptr->m_PanelFdNGHandleMode);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//單板定位點跳過檢測數量
	ParamUnit = CParamUni();
	str = _T("Board Fd NG Skip Count");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_BOARD_FD_NG_SKIP_COUNT;
	ParamUnit.SetParamID((UINT)(ParamID));	
	for ( i=0; i<BOARD_MAX_FD_COUNT; i++ )
	{	strValue.Format(_T("%d"), i+1);	ParamUnit.AddSelItem(i+1, strValue);	}
	ParamUnit.SetValue_SEL(Ptr->m_BoardFdNGSkipCount);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//單板定位點異常處理模式
	ParamUnit = CParamUni();
	str = _T("Board Fd NG Handle Mode");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_BOARD_FD_NG_HANDLE_MODE;
	ParamUnit.SetParamID((UINT)(ParamID));	
	strValue = AOIDataDefine.GetFdNGHandleModeText(FD_NG_HANDLE_NONE);	ParamUnit.AddSelItem(FD_NG_HANDLE_NONE, strValue);	
	strValue = AOIDataDefine.GetFdNGHandleModeText(FD_NG_HANDLE_XBOARD);	ParamUnit.AddSelItem(FD_NG_HANDLE_XBOARD, strValue);
	ParamUnit.SetValue_SEL(Ptr->m_BoardFdNGHandleMode);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);		

	//單板定位點取像模式
	ParamUnit = CParamUni();
	str = _T("Board Fd Grab Mode");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_BOARD_FD_GRAB_MODE;
	ParamUnit.SetParamID((UINT)(ParamID));	
	strValue = AOIDataDefine.GetBoardFdGrabModeText(BOARD_FD_GRAB_AFTER_PANEL);	ParamUnit.AddSelItem(BOARD_FD_GRAB_AFTER_PANEL, strValue);
	strValue = AOIDataDefine.GetBoardFdGrabModeText(BOARD_FD_GRAB_INSPECTING);	ParamUnit.AddSelItem(BOARD_FD_GRAB_INSPECTING, strValue);
	ParamUnit.SetValue_SEL(Ptr->m_BoardFdGrabMode);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//異常處理模式
	ParamUnit = CParamUni();
	str = _T("Defect Product Handle Mode");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_DEFECT_HANDLE_MODE;
	ParamUnit.SetParamID((UINT)(ParamID));	
	strValue = AOIDataDefine.GetDefectHandleModeText(DEFECT_HANDLE_PASS);	ParamUnit.AddSelItem(DEFECT_HANDLE_PASS, strValue);
	strValue = AOIDataDefine.GetDefectHandleModeText(DEFECT_HANDLE_STOP_ALARM);	ParamUnit.AddSelItem(DEFECT_HANDLE_STOP_ALARM, strValue);
	strValue = AOIDataDefine.GetDefectHandleModeText(DEFECT_HANDLE_NEXT_STOP);	ParamUnit.AddSelItem(DEFECT_HANDLE_NEXT_STOP, strValue);
	strValue = AOIDataDefine.GetDefectHandleModeText(DEFECT_HANDLE_WAIT_FOR_REPAIR);	ParamUnit.AddSelItem(DEFECT_HANDLE_WAIT_FOR_REPAIR, strValue);
	strValue = AOIDataDefine.GetDefectHandleModeText(DEFECT_HANDLE_CONTROL_CENTER);	ParamUnit.AddSelItem(DEFECT_HANDLE_CONTROL_CENTER, strValue);
	ParamUnit.SetValue_SEL(Ptr->m_DefectHandleMode);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);
	

	//PCB出板模式
	ParamUnit = CParamUni();
	str = _T("PCB Out Mode");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_PCB_OUT_MODE;
	ParamUnit.SetParamID((UINT)(ParamID));	
	strValue = AOIDataDefine.GetPCBOutModeText(PCB_OUT_NORMAL);		ParamUnit.AddSelItem(PCB_OUT_NORMAL, strValue);
	strValue = AOIDataDefine.GetPCBOutModeText(PCB_OUT_SIDE_OUT);	ParamUnit.AddSelItem(PCB_OUT_SIDE_OUT, strValue);
	strValue = AOIDataDefine.GetPCBOutModeText(PCB_OUT_WITH_IN);	ParamUnit.AddSelItem(PCB_OUT_WITH_IN, strValue);
	strValue = AOIDataDefine.GetPCBOutModeText(PCB_OUT_LANE_AUTO);	ParamUnit.AddSelItem(PCB_OUT_LANE_AUTO, strValue);	
	strValue = AOIDataDefine.GetPCBOutModeText(PCB_OUT_OK_OUT_NG_SIDE);	ParamUnit.AddSelItem(PCB_OUT_OK_OUT_NG_SIDE, strValue);		
	ParamUnit.SetValue_SEL(Ptr->m_PCBOutMode);
	strDescription =CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//軌道提前運轉功能	
	ParamUnit = CParamUni();
	str = _T("Conveyer Pre-Run");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_ENABLE_CONVEYER_PRE_RUN;
	ParamUnit.SetParamID((UINT)(ParamID));
	intValue = FN_DISABLE;	strValue = AOIDataDefine.GetEnableDisableText(intValue);	ParamUnit.AddSelItem(intValue, strValue);
	intValue = FN_ENABLE;	strValue = AOIDataDefine.GetEnableDisableText(intValue);	ParamUnit.AddSelItem(intValue, strValue);	
	ParamUnit.SetValue_SEL(Ptr->m_EnableConveyerPreRun);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//檢測區域配置模式	
	ParamUnit = CParamUni();
	str = _T("Inspection Field Build Mode");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_INSPECTION_FIELD_BUILD_MODE;
	ParamUnit.SetParamID((UINT)(ParamID));	
	strValue = AOIDataDefine.GetInspectionFieldBuildMode(FIELD_BUILD_MATRIX);	ParamUnit.AddSelItem(FIELD_BUILD_MATRIX, strValue);
	strValue = AOIDataDefine.GetInspectionFieldBuildMode(FIELD_BUILD_RANDOM_PANEL);	ParamUnit.AddSelItem(FIELD_BUILD_RANDOM_PANEL, strValue);
	strValue = AOIDataDefine.GetInspectionFieldBuildMode(FIELD_BUILD_RANDOM_BOARD);	ParamUnit.AddSelItem(FIELD_BUILD_RANDOM_BOARD, strValue);	
	//case FIELD_BUILD_RANDOM_PROJECT:
	ParamUnit.SetValue_SEL(Ptr->m_InspectionFieldBuildMode);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//檢測區域配置面積模式
	ParamUnit = CParamUni();
	str = _T("Inspection Field Build Area Mode");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_INSPECTION_FIELD_BUILD_AREA_MODE;
	ParamUnit.SetParamID((UINT)(ParamID));	
	strValue = AOIDataDefine.GetComponentText();	ParamUnit.AddSelItem(FIELD_BUILD_AREA_COMPONENT, strValue);
	strValue = AOIDataDefine.GetBoardText();	ParamUnit.AddSelItem(FIELD_BUILD_AREA_BOARD, strValue);	
	ParamUnit.SetValue_SEL(Ptr->m_InspectionFieldBuildAreaMode);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//區域分割模式
	ParamUnit = CParamUni();
	str = _T("Field Division Mode");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_FIELD_DIVISION_MODE;
	ParamUnit.SetParamID((UINT)(ParamID));		
	strValue = AOIDataDefine.GetFieldDivisionMode(FIELD_DIVISION_MASS_AREA);	ParamUnit.AddSelItem(FIELD_DIVISION_MASS_AREA, strValue);	
	strValue = AOIDataDefine.GetFieldDivisionMode(FIELD_DIVISION_DIAGONAL_LINE);	ParamUnit.AddSelItem(FIELD_DIVISION_DIAGONAL_LINE, strValue);	
	strValue = AOIDataDefine.GetFieldDivisionMode(FIELD_DIVISION_HORIZONTAL_LINE);	ParamUnit.AddSelItem(FIELD_DIVISION_HORIZONTAL_LINE, strValue);	
	strValue = AOIDataDefine.GetFieldDivisionMode(FIELD_DIVISION_VERTICAL_LINE);	ParamUnit.AddSelItem(FIELD_DIVISION_VERTICAL_LINE, strValue);	
	ParamUnit.SetValue_SEL(Ptr->m_FieldDivisionMode);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//區域分割單板定位點優先	
	ParamUnit = CParamUni();
	str = _T("Field Division Board Fd Fist");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_FIELD_DIVISION_BOARD_FD_FIRST;
	ParamUnit.SetParamID((UINT)(ParamID));
	AOIDataDefine.BuildEnableDisableParamUni(ParamUnit);	
	ParamUnit.SetValue_SEL(Ptr->m_FieldDivisionBoardFdFirst);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//檢測區域路徑模式
	ParamUnit = CParamUni();
	str = _T("Field Path Mode");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_FIELD_PATH_MODE;
	ParamUnit.SetParamID((UINT)(ParamID));	
	strValue = AOIDataDefine.GetFieldPathMode(FIELD_PATH_SPATH_HOR);	ParamUnit.AddSelItem(FIELD_PATH_SPATH_HOR, strValue);
	strValue = AOIDataDefine.GetFieldPathMode(FIELD_PATH_SPATH_VER);	ParamUnit.AddSelItem(FIELD_PATH_SPATH_VER, strValue);	
	ParamUnit.SetValue_SEL(Ptr->m_FieldPathMode);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//區域區間係數-影響路徑規劃的走法
	ParamUnit = CParamUni();
	str = _T("Field Section Factor");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_FIELD_SECTION_FACTOR;
	ParamUnit.SetParamID((UINT)(ParamID));		
	ParamUnit.SetValue_DBL(Ptr->m_FieldSectionFactor, 2, 0.001, 100);	
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//報廢板映射檔案模式	
	ParamUnit = CParamUni();
	str = _T("X-Board Mapping File Mode");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_XBOARD_MAPPING_FILE_MODE;
	ParamUnit.SetParamID((UINT)(ParamID));	
	strValue = AOIDataDefine.GetXBoardMappingFileModeText(XBOARD_MAPPING_FILE_DISABLE);	ParamUnit.AddSelItem(XBOARD_MAPPING_FILE_DISABLE, strValue);
	strValue = AOIDataDefine.GetXBoardMappingFileModeText(XBOARD_MAPPING_FILE_MES_COMM);	ParamUnit.AddSelItem(XBOARD_MAPPING_FILE_MES_COMM, strValue);		
	ParamUnit.SetValue_SEL(Ptr->m_XBoardMappingFileMode);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//報廢板映射檔案順序模式	
	ParamUnit = CParamUni();
	str = _T("X-Board Mapping File Trigger Time");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_XBOARD_MAPPING_FILE_FLOW;
	ParamUnit.SetParamID((UINT)(ParamID));
	strValue = AOIDataDefine.GetXBoardMappingFileFlowText(XBOARD_MAPPING_FILE_FLOW_DEFAULT);
	ParamUnit.AddSelItem(XBOARD_MAPPING_FILE_FLOW_DEFAULT, strValue);
	strValue = AOIDataDefine.GetXBoardMappingFileFlowText(XBOARD_MAPPING_FILE_FLOW_AFTER_BARCODE);
	ParamUnit.AddSelItem(XBOARD_MAPPING_FILE_FLOW_AFTER_BARCODE, strValue);
	ParamUnit.SetValue_SEL(Ptr->m_XBoardMappingFileFlow);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//報廢板MES Comm 條件檢測
	ParamUnit = CParamUni();
	str = _T("X-Board Mapping File Board Check");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_XBOARD_MAPPING_FILE_MES_CHECK;
	ParamUnit.SetParamID((UINT)(ParamID));
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);	ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);	ParamUnit.AddSelItem(FN_ENABLE, strValue);
	ParamUnit.SetValue_SEL(Ptr->m_XBoardMappingMESCheck);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//通用參數字串-01
	ParamUnit = CParamUni();
	str = _T("General String 1");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_GENERAL_STR_01;	
	ParamUnit.SetParamID((UINT)(ParamID));	
	ParamUnit.SetValue_STR(Ptr->m_ProjectGeneralParamStr_01.c_str());
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//通用參數字串-02	
	ParamUnit = CParamUni();
	str = _T("General String 2");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_GENERAL_STR_02;	
	ParamUnit.SetParamID((UINT)(ParamID));	
	ParamUnit.SetValue_STR(Ptr->m_ProjectGeneralParamStr_02.c_str());
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//通用參數字串-03
	ParamUnit = CParamUni();
	str = _T("General String 3");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_GENERAL_STR_03;	
	ParamUnit.SetParamID((UINT)(ParamID));	
	ParamUnit.SetValue_STR(Ptr->m_ProjectGeneralParamStr_03.c_str());
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//通用參數字串-04	
	ParamUnit = CParamUni();
	str = _T("General String 4");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_GENERAL_STR_04;	
	ParamUnit.SetParamID((UINT)(ParamID));	
	ParamUnit.SetValue_STR(Ptr->m_ProjectGeneralParamStr_04.c_str());
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectParamPaneBasic::BuildParamListWnd()
{
	SetActParamUni(NULL);
	CThisListCtrl_24 &ListCtrl = m_ParamListCtrl;	
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
	
	BuildParamList();

	const int ParamCount = (int)(m_ParamList.size());

	nItem=0;
	ListCtrl.SetRedraw(FALSE);
	m_StopParamListBeSelected = true;
	for ( i=0; i<ParamCount; i++ )
	{
		ParamPtr = &(m_ParamList[i]);
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
	m_StopParamListBeSelected = false;
	ListCtrl.SetRedraw(TRUE);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectParamPaneBasic::BuildParamListWndHeader()
{
	CString str;
	int   nCol = 0;
	int width  = 64;
	int width2 = 64;
	RECT      Rect;	
	const int Align = LVCFMT_LEFT;//LVCFMT_CENTER;LVCFMT_RIGHT, LVCFMT_LEFT
	CThisListCtrl_24 &ListCtrl = m_ParamListCtrl;

	ListCtrl.GetClientRect(&Rect);
	width = (Rect.right-Rect.left-8)/4;
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
	return true;
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneBasic::SetDescriptionText(const CParamUni *Ptr)
{
	UINT CtrlID = PROBASIC_INFO_EDIT;
	if ( NULL == Ptr )
	{
		CWnd::SetDlgItemText(CtrlID, _T(""));
		return ;
	}
	CWnd::SetDlgItemText(CtrlID, Ptr->GetDesction());
}
//-------------------------------------------------------------------------------------//
bool CProjectParamPaneBasic::ExecItemchangedParamListWnd(CThisListCtrl_24 &ListCtrl, int nItem)
{	
	const int    ItemCount = ListCtrl.GetItemCount();
	if ( nItem<0 || nItem>=ItemCount ) { return false; }

	const size_t ParamIndex = ListCtrl.GetItemData(nItem);
	const size_t ParamCount = m_ParamList.size();
	if ( ParamIndex >= ParamCount ) { return false; }	
	CParamUni *ParamPtr=&(m_ParamList[ParamIndex]);	
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
	{	HideCtrlBtn();	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectParamPaneBasic::ExecDblclkParamListWnd(CThisListCtrl_24 &ListCtrl, int nItem, int nSubItem)
{	
	const int    ItemCount = ListCtrl.GetItemCount();
	if ( nItem<0 || nItem>=ItemCount ) { return false; }
	if ( nSubItem < SETTING_COL ) { return true; }

	const size_t ParamIndex = ListCtrl.GetItemData(nItem);
	const size_t ParamCount = m_ParamList.size();
	if ( ParamIndex >= ParamCount ) { return false; }		
	
	size_t          i=0;
	int             nSelIdx=0;
	int             nValue=0;
	CRect           ItemRect;
	RECT            CtrlRect={0};	
	CString         ItemText;	
	const int       Offset = 2;
	CParamUni      *ParamPtr=&(m_ParamList[ParamIndex]);		
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
	HideCtrlBtn();
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
bool CProjectParamPaneBasic::HideCtrlBtn()
{
	m_SetFocusBtn.ShowWindow(SW_HIDE);
	m_SetFolderBtn.ShowWindow(SW_HIDE);
	m_SetLaneWidthBtn.ShowWindow(SW_HIDE);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectParamPaneBasic::ExecReleaseParamCtrl()
{	
	HideCtrlBtn();
	m_EditCtrl.ShowWindow(SW_HIDE);	
	m_ComboxCtrl.ShowWindow(SW_HIDE);	
	m_EditCtrl.SetWindowText(_T(""));
	m_ParamListCtrl.SetFocus();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectParamPaneBasic::ExecUpdateParamByEdit()
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

	if ( CheckNeedToVerifyJsonString(ParamID) == true )
	{
		if ( CheckJsonStringValid(ItemText) == false )
		{
			ItemText = ParamPtr->GetParamText();
			m_EditCtrl.SetWindowText(ItemText);
			return false;
		}
	}

	if ( CAOIProject::SetProjectParameterStringByID(ParamID, *m_ProParameterPtr, ItemText) == false )
	{	return false; }
	
	CThisListCtrl_24 *pListCtrl = (CThisListCtrl_24*)(ParamPtr->GetListCtrl());
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
bool CProjectParamPaneBasic::ExecUpdateParamByCombox()
{
	CParamUni   *ParamPtr = GetActParamUni();
	if ( NULL == ParamPtr ) { return true; }	
	PARAM_DATA_TYPE  DataType = ParamPtr->GetDataType();
	if ( PARAM_DATA_SEL != DataType ) { return true; }
	SetActParamUni(NULL);

	CString ItemText;	
	PROJECT_PARAM_ID ParamID = (PROJECT_PARAM_ID)(ParamPtr->GetParamID());
	const int nCurSel = m_ComboxCtrl.GetCurSel();
	if ( nCurSel < 0 ) { return true; }
	const int Param = (int)(m_ComboxCtrl.GetItemData(nCurSel));
	if ( Param == ParamPtr->GetSelParam() ) { return false; }	
	ItemText.Format(_T("%d"),Param);	
	if ( ParamPtr->SetNewValue_SEL(Param) == false )
	{	return false; }
	if ( CAOIProject::SetProjectParameterStringByID(ParamID, *m_ProParameterPtr, ItemText) == false )
	{	return false; }

	ItemText = ParamPtr->GetParamText();
	CThisListCtrl_24 *pListCtrl = (CThisListCtrl_24*)(ParamPtr->GetListCtrl());
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
bool CProjectParamPaneBasic::CheckJsonStringValid(LPCTSTR Text)//確認符合Json字串
{
	if ( AOIDataCollect.VerifyJsonString(Text) == false )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return false;
	}
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CProjectParamPaneBasic::CheckNeedToVerifyJsonString(PROJECT_PARAM_ID ParamID)//確認是否需要確認Json字串
{
	const bool bVerify=AOIDataCollect.CheckProjectParamNeedToVerifyJsonString(ParamID);
	return bVerify;
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneBasic::OnFocusPosBtn() 
{
	// TODO: Add your control notification handler code here
	HideCtrlBtn();
	if ( NULL == m_ProParameterPtr ) { return; }
	CParamUni *ParamPtr = GetActParamUni();
	if ( NULL == ParamPtr ) { return; }
	CWnd      *BtnWndPtr = ParamPtr->GetBtnWndPtr();
	if ( NULL == BtnWndPtr ) { return; }

	CString str, str1;
	double PosX=0;
	double PosY=0;
	double PosZ=0;	
	MotionCtrlPtr->GetCurrentPos(PosX, PosY, PosZ);
	str = _T("Do you want to apply current z-Pos be project focus position?");
	str = LoadMultiLanguageString(str, str);
	str1.Format(_T("%s [Z:%.0f]"), str, PosZ);
	if ( JetAPI::ShowMessageBox(str1, MB_YESNO) == IDNO ) 
	{	return ; }
	
	CString  ItemText = ParamPtr->GetParamText();
	LANE_ID  LaneID = AOIDataCollect.GetActiveLaneID();
	double FocusOffsetZ = AOIDataCollect.CalcProjectFocusOffset(LaneID);
	PROJECT_PARAM_ID ParamID = (PROJECT_PARAM_ID)(ParamPtr->GetParamID());	
	
	ItemText.Format(_T("%.0f"), FocusOffsetZ);
	if ( CAOIProject::SetProjectParameterStringByID(ParamID, *m_ProParameterPtr, ItemText) == false )
	{	return ; }
	ParamPtr->SetNewValue(ItemText);
	SetActParamUni(NULL);
	const int nItem = ParamPtr->GetItemIndex();
	const int nSubItem = ParamPtr->GetSubItemIndex();	
	CThisListCtrl_24 *pListCtrl = (CThisListCtrl_24*)(ParamPtr->GetListCtrl());
	if ( NULL != pListCtrl )
	{
		const int ItemCount = pListCtrl->GetItemCount();
		pListCtrl->SetItemText(nItem, nSubItem, ItemText);
		pListCtrl->SetFocus();
	}	
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneBasic::OnLaneWidthBtn() 
{
	// TODO: Add your control notification handler code here
	HideCtrlBtn();
	if ( NULL == m_ProParameterPtr ) { return; }
	CParamUni *ParamPtr = GetActParamUni();
	if ( NULL == ParamPtr ) { return; }
	CWnd      *BtnWndPtr = ParamPtr->GetBtnWndPtr();
	if ( NULL == BtnWndPtr ) { return; }

	CString str, str1;
	CString LaneText;
	double  LaneWidth=0;
	LANE_ID LaneID = AOIDataCollect.GetActiveLaneID();
	LaneText = AOIDataDefine.GetLaneIDText(LaneID);
	LaneWidth = PlcCtrlPtr->ReadLaneAdjustCurrentPos(LaneID);
	str = _T("Do you want to apply current lane width be project lane width?");
	str = LoadMultiLanguageString(str, str);
	str1.Format(_T("%s [%s Width:%.2f mm]"), str, LaneText, LaneWidth);
	if ( JetAPI::ShowMessageBox(str1, MB_YESNO) == IDNO ) 
	{	return ; }

	CString ItemText = ParamPtr->GetParamText();
	PROJECT_PARAM_ID ParamID = (PROJECT_PARAM_ID)(ParamPtr->GetParamID());
	ItemText.Format(_T("%.2f"), LaneWidth);
	if ( CAOIProject::SetProjectParameterStringByID(ParamID, *m_ProParameterPtr, ItemText) == false )
	{	return ; }
	ParamPtr->SetNewValue(ItemText);
	SetActParamUni(NULL);
	const int nItem = ParamPtr->GetItemIndex();
	const int nSubItem = ParamPtr->GetSubItemIndex();	
	CThisListCtrl_24 *pListCtrl = (CThisListCtrl_24*)(ParamPtr->GetListCtrl());
	if ( NULL != pListCtrl )
	{
		const int ItemCount = pListCtrl->GetItemCount();
		pListCtrl->SetItemText(nItem, nSubItem, ItemText);
		pListCtrl->SetFocus();
	}
}
//-------------------------------------------------------------------------------------//