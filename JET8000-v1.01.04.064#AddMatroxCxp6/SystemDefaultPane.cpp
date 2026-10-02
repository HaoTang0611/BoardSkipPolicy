// SystemDefaultPane.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "SystemDefaultPane.h"
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
// CSystemDefaultPane dialog
//-------------------------------------------------------------------------------------//
CSystemDefaultPane::CSystemDefaultPane(CWnd* pParent /*=NULL*/)
	: CDialog(CSystemDefaultPane::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSystemDefaultPane)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ParamActPtr = NULL;
	m_SysParameterPtr = NULL;	
	m_StopParamListBeSelected = false;
}
//-------------------------------------------------------------------------------------//
void CSystemDefaultPane::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSystemDefaultPane)
	DDX_Control(pDX, SYSDEFAULT_PARAM_EDIT, m_EditCtrl);
	DDX_Control(pDX, SYSDEFAULT_FOLDER_BTN, m_BtnFolder);
	DDX_Control(pDX, SYSDEFAULT_PARAM_COMBO, m_ComboxCtrl);	
	DDX_Control(pDX, SYSDEFAULT_PARAM_LIST_WND, m_ParamListCtrl);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CSystemDefaultPane, CDialog)
	//{{AFX_MSG_MAP(CSystemDefaultPane)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(LVN_ITEMCHANGED, SYSDEFAULT_PARAM_LIST_WND, OnItemchangedParamListWnd)
	ON_NOTIFY(NM_DBLCLK, SYSDEFAULT_PARAM_LIST_WND, OnDblclkParamListWnd)
	ON_EN_KILLFOCUS(SYSDEFAULT_PARAM_EDIT, OnKillfocusParamEdit)
	ON_CBN_SELCHANGE(SYSDEFAULT_PARAM_COMBO, OnSelchangeParamCombo)
	ON_CBN_KILLFOCUS(SYSDEFAULT_PARAM_COMBO, OnKillfocusParamCombo)
	ON_BN_CLICKED(SYSDEFAULT_FOLDER_BTN, OnFolderBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CSystemDefaultPane message handlers
//-------------------------------------------------------------------------------------//
BOOL CSystemDefaultPane::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SwitchMultiLanguage();
	JetAPI::InitialListCtrl(m_ParamListCtrl);	
	//CWnd::ShowWindow(SW_SHOWNORMAL);
	BuildParamListWndHeader();
	if ( CWnd::IsWindowVisible() )
	{	BuildParamListWnd();	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CSystemDefaultPane::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CSystemDefaultPane::OnSize(UINT nType, int cx, int cy) 
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

	WndPtr = CWnd::GetDlgItem(SYSDEFAULT_INFO_EDIT);
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
void CSystemDefaultPane::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if ( TRUE == bShow )
	{	
		BuildParamListWnd();	
	}
}
//-------------------------------------------------------------------------------------//
void CSystemDefaultPane::OnOK() 
{
	// TODO: Add extra validation here
	return;
	CDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CSystemDefaultPane::OnCancel() 
{
	// TODO: Add extra cleanup here
	return;
	CDialog::OnCancel();
}
//-------------------------------------------------------------------------------------//
BOOL CSystemDefaultPane::PreTranslateMessage(MSG* pMsg) 
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
LRESULT CSystemDefaultPane::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void CSystemDefaultPane::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_SYSTEM_DEFAULT_PANE");
	//---------------------------------------------------------------------------------//
	WndID = IDD_SYSTEM_DEFAULT_PANE;
	WndKey = _T("IDD_SYSTEM_DEFAULT_PANE");
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
CString CSystemDefaultPane::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_SYSTEM_DEFAULT_PANE");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
void CSystemDefaultPane::SetSystemParameterPtr(TSystemParameter *Ptr)
{
	m_SysParameterPtr = Ptr;
}
//-------------------------------------------------------------------------------------//
CParamUni* CSystemDefaultPane::GetActParamUni()
{
	return m_ParamActPtr;
}
//-------------------------------------------------------------------------------------//
void CSystemDefaultPane::SetActParamUni(CParamUni *Ptr)
{
	m_ParamActPtr = Ptr;
}
//-------------------------------------------------------------------------------------//
bool CSystemDefaultPane::BuildParamList()
{
	CThisListCtrl_32 &ListCtrl = m_ParamListCtrl;
	m_StopParamListBeSelected = true;
	JetAPI::ClearListCtrl(ListCtrl, FALSE);
	m_StopParamListBeSelected = false;
	if ( NULL == m_SysParameterPtr ) { return true; }

	int       i=0;
	int       intValue=0;
	CString   str;
	CString   strValue;	
	CString   strCaption;	
	CString   strDescription;
	int       nItem=0;
	CParamUni    ParamUnit;	
	SYSTEM_PARAM_ID ParamID;
	TSystemParameter *Ptr = m_SysParameterPtr;
	const int nSubItem = 1;		
	CParamList &ParamList = m_ParamList;

	ParamList.clear();
	
	//專案預設高度轉灰階比例	
	ParamUnit = CParamUni();
	str = _T("Space to Gray Ratio Mode");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_PROG_DEFAULT_SPACE_TO_GRAY_RATIO_MODE;	
	ParamUnit.SetParamID((UINT)(ParamID));	

	intValue = 0;
	const size_t RatioCount = AOIDataDefine.GetProjectSpaceToGrayRatioModeCount();
	for ( i=0; i<RatioCount; i++ )
	{
		intValue += 5;
		strValue.Format(_T("%d"), intValue);		
		ParamUnit.AddSelItem(intValue, strValue);
	}	
	ParamUnit.SetValue_SEL(Ptr->m_DefaultSpaceToGrayRatioMode);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);	
	ParamList.push_back(ParamUnit);

	//專案預設空間基準面編號	
	ParamUnit = CParamUni();
	str = _T("Space Base Plane Index");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_PROG_DEFAULT_SPACE_BASE_PLANE_INDEX;	
	ParamUnit.SetParamID((UINT)(ParamID));
	for ( i=0; i<= MAX_SYSTEM_BASE_PLANE_PARAM_COUNT; i++ )
	{
		intValue = i-1;
		if ( 0 == i )
		{	strValue.Format(_T("[%d] %s"), i, AOIDataDefine.GetDisableText()); }					
		else
		{				
			TBasePlaneParam BasePlaneParam;
			AOIDataCollect.GetSystemBasePlaneParam(intValue, BasePlaneParam);
			strValue.Format(_T("[%d] %s"), i, BasePlaneParam.BasePlaneInfoText);
		}
		ParamUnit.AddSelItem(intValue, strValue);	
	}	
	ParamUnit.SetValue_SEL(Ptr->m_DefaultSpaceBasePlaneIndex);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);	
	ParamList.push_back(ParamUnit);


	//專案預設空間雜訊過濾編號	
	ParamUnit = CParamUni();
	str = _T("Space Noise Filter Index");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_PROG_DEFAULT_SPACE_NOISE_FILTER_INDEX;	
	ParamUnit.SetParamID((UINT)(ParamID));
	for ( i=0; i<= MAX_SYSTEM_NOISE_FILTER_PARAM_COUNT; i++ )
	{
		intValue = i-1;
		if ( 0 == i )
		{	strValue.Format(_T("[%d] %s"), i, AOIDataDefine.GetDisableText());	}
		else
		{				
			TNoiseFilterParam NoiseFilterParam;
			AOIDataCollect.GetSystemNoiseFilterParam(intValue, NoiseFilterParam);			
			strValue.Format(_T("[%d] %s"), i, NoiseFilterParam.DataFilterInfoText);
		}
		ParamUnit.AddSelItem(intValue, strValue);	
	}	
	ParamUnit.SetValue_SEL(Ptr->m_DefaultSpaceNoiseFilterIndex);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);	
	ParamList.push_back(ParamUnit);

	//專案預設軌道提前運轉功能	
	ParamUnit = CParamUni();
	str = _T("Conveyer Pre-Run");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_PROG_DEFAULT_ENABLE_CONVEYER_PRE_RUN;
	ParamUnit.SetParamID((UINT)(ParamID));
	intValue = FN_DISABLE;		
	strValue = AOIDataDefine.GetEnableDisableText(intValue);
	ParamUnit.AddSelItem(intValue, strValue);
	intValue = FN_ENABLE;
	strValue = AOIDataDefine.GetEnableDisableText(intValue);
	ParamUnit.AddSelItem(intValue, strValue);	
	ParamUnit.SetValue_SEL(Ptr->m_DefaultEnableConveyerPreRun);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//專案預設定位點異常處理模式
	ParamUnit = CParamUni();
	str = _T("Fd NG Handle Mode");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_PROG_DEFAULT_FD_NG_HANDLE_MODE;
	ParamUnit.SetParamID((UINT)(ParamID));	
	strValue = AOIDataDefine.GetFdNGHandleModeText(FD_NG_HANDLE_PASS);	ParamUnit.AddSelItem(FD_NG_HANDLE_PASS, strValue);
	strValue = AOIDataDefine.GetFdNGHandleModeText(FD_NG_HANDLE_STOP);	ParamUnit.AddSelItem(FD_NG_HANDLE_STOP, strValue);	
	ParamUnit.SetValue_SEL(Ptr->m_DefaultFdNGHandleMode);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//專案預設單板定位點取像模式
	ParamUnit = CParamUni();
	str = _T("Board Fd Grab Mode");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_PROG_DEFAULT_BOARD_FD_GRAB_MODE;
	ParamUnit.SetParamID((UINT)(ParamID));	
	strValue = AOIDataDefine.GetBoardFdGrabModeText(BOARD_FD_GRAB_AFTER_PANEL);	ParamUnit.AddSelItem(BOARD_FD_GRAB_AFTER_PANEL, strValue);
	strValue = AOIDataDefine.GetBoardFdGrabModeText(BOARD_FD_GRAB_INSPECTING);	ParamUnit.AddSelItem(BOARD_FD_GRAB_INSPECTING, strValue);
	ParamUnit.SetValue_SEL(Ptr->m_DefaultBoardFdGrabMode);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//專案預設檢出異常處理模式
	ParamUnit = CParamUni();
	str = _T("Defect Product Handle Mode");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_PROG_DEFAULT_BDEFECT_HANDLE_MODE;
	ParamUnit.SetParamID((UINT)(ParamID));	
	strValue = AOIDataDefine.GetDefectHandleModeText(DEFECT_HANDLE_PASS);	ParamUnit.AddSelItem(DEFECT_HANDLE_PASS, strValue);
	strValue = AOIDataDefine.GetDefectHandleModeText(DEFECT_HANDLE_STOP_ALARM);	ParamUnit.AddSelItem(DEFECT_HANDLE_STOP_ALARM, strValue);
	strValue = AOIDataDefine.GetDefectHandleModeText(DEFECT_HANDLE_NEXT_STOP);	ParamUnit.AddSelItem(DEFECT_HANDLE_NEXT_STOP, strValue);
	strValue = AOIDataDefine.GetDefectHandleModeText(DEFECT_HANDLE_WAIT_FOR_REPAIR);	ParamUnit.AddSelItem(DEFECT_HANDLE_WAIT_FOR_REPAIR, strValue);
	strValue = AOIDataDefine.GetDefectHandleModeText(DEFECT_HANDLE_CONTROL_CENTER);	ParamUnit.AddSelItem(DEFECT_HANDLE_CONTROL_CENTER, strValue);
	ParamUnit.SetValue_SEL(Ptr->m_DefaultDefectHandleMode);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);
	
	//專案預設PCB出板模式
	ParamUnit = CParamUni();
	str = _T("PCB Out Mode");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_PROG_DEFAULT_PCB_OUT_MODE;
	ParamUnit.SetParamID((UINT)(ParamID));	
	strValue = AOIDataDefine.GetPCBOutModeText(PCB_OUT_NORMAL);		ParamUnit.AddSelItem(PCB_OUT_NORMAL, strValue);
	strValue = AOIDataDefine.GetPCBOutModeText(PCB_OUT_SIDE_OUT);	ParamUnit.AddSelItem(PCB_OUT_SIDE_OUT, strValue);
	strValue = AOIDataDefine.GetPCBOutModeText(PCB_OUT_WITH_IN);	ParamUnit.AddSelItem(PCB_OUT_WITH_IN, strValue);
	strValue = AOIDataDefine.GetPCBOutModeText(PCB_OUT_LANE_AUTO);	ParamUnit.AddSelItem(PCB_OUT_LANE_AUTO, strValue);	
	strValue = AOIDataDefine.GetPCBOutModeText(PCB_OUT_OK_OUT_NG_SIDE);	ParamUnit.AddSelItem(PCB_OUT_OK_OUT_NG_SIDE, strValue);		
	ParamUnit.SetValue_SEL(Ptr->m_DefaultPCBOutMode);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//專案預設儲存檢測底圖模式	
	ParamUnit = CParamUni();
	str = _T("Project Save Test Map");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_PROG_DEFAULT_PROJECT_SAVE_TEST_MAP;
	ParamUnit.SetParamID((UINT)(ParamID));	
	strValue = AOIDataDefine.GetSaveTestMapModeText(SAVE_TEST_MAP_DISABLE);	ParamUnit.AddSelItem(SAVE_TEST_MAP_DISABLE, strValue);
	strValue = AOIDataDefine.GetSaveTestMapModeText(SAVE_TEST_MAP_ENB_PROG);	ParamUnit.AddSelItem(SAVE_TEST_MAP_ENB_PROG, strValue);
	strValue = AOIDataDefine.GetSaveTestMapModeText(SAVE_TEST_MAP_ENB_PANEL);	ParamUnit.AddSelItem(SAVE_TEST_MAP_ENB_PANEL, strValue);
	strValue = AOIDataDefine.GetSaveTestMapModeText(SAVE_TEST_MAP_ENB_BOARD);	ParamUnit.AddSelItem(SAVE_TEST_MAP_ENB_BOARD, strValue);	
	strValue = AOIDataDefine.GetSaveTestMapModeText(SAVE_TEST_MAP_ENB_PROG_PANEL);	ParamUnit.AddSelItem(SAVE_TEST_MAP_ENB_PROG_PANEL, strValue);
	strValue = AOIDataDefine.GetSaveTestMapModeText(SAVE_TEST_MAP_ENB_PROG_BOARD);	ParamUnit.AddSelItem(SAVE_TEST_MAP_ENB_PROG_BOARD, strValue);	
	ParamUnit.SetValue_SEL(Ptr->m_DefaultProjectSaveTestMap);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//專案預設連線伺服器模式	
	ParamUnit = CParamUni();
	str = _T("Project Link Server Mode");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_PROG_DEFAULT_PROJECT_LINK_SERVER_MODE;
	ParamUnit.SetParamID((UINT)(ParamID));	
	strValue = AOIDataDefine.GetProjectLinkServerModeText(PROJECT_LINK_SERVER_DISABLE);		ParamUnit.AddSelItem(PROJECT_LINK_SERVER_DISABLE, strValue);
	strValue = AOIDataDefine.GetProjectLinkServerModeText(PROJECT_LINK_SERVER_ENABLE_ALL);		ParamUnit.AddSelItem(PROJECT_LINK_SERVER_ENABLE_ALL, strValue);
	strValue = AOIDataDefine.GetProjectLinkServerModeText(PROJECT_LINK_SERVER_PROJECT_ONLY);	ParamUnit.AddSelItem(PROJECT_LINK_SERVER_PROJECT_ONLY, strValue);
	strValue = AOIDataDefine.GetProjectLinkServerModeText(PROJECT_LINK_SERVER_ENABLE_ALL_ASK);		ParamUnit.AddSelItem(PROJECT_LINK_SERVER_ENABLE_ALL_ASK, strValue);
	strValue = AOIDataDefine.GetProjectLinkServerModeText(PROJECT_LINK_SERVER_PROJECT_ONLY_ASK);		ParamUnit.AddSelItem(PROJECT_LINK_SERVER_PROJECT_ONLY_ASK, strValue);	
	ParamUnit.SetValue_SEL(Ptr->m_DefaultProjectLinkServerMode);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	
	
	//專案預設儲存離線圖檔
	ParamUnit = CParamUni();
	str = _T("Save Offline Image Files");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_PROG_DEFAULT_SAVE_OFFLINE_IMAGE_FILES;
	ParamUnit.SetParamID((UINT)(ParamID));	
	intValue = FN_DISABLE;	strValue = AOIDataDefine.GetEnableDisableText(intValue);	ParamUnit.AddSelItem(intValue, strValue);
	intValue = FN_ENABLE;	strValue = AOIDataDefine.GetEnableDisableText(intValue);	ParamUnit.AddSelItem(intValue, strValue);	
	ParamUnit.SetValue_SEL(Ptr->m_DefaultProjectSaveOfflineImageFiles);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//專案預設條碼驗證模式
	ParamUnit = CParamUni();
	str = _T("Barcode Verify Mode");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_PROG_DEFAULT_BARCODE_VERIFY_MODE;
	ParamUnit.SetParamID((UINT)(ParamID));	
	AOIDataDefine.BuildEnableDisableParamUni(ParamUnit);	
	ParamUnit.SetValue_SEL(Ptr->m_DefaultBarcodeVerifyMode);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//專案預設條碼查詢模式	
	ParamUnit = CParamUni();
	str = _T("Barcode Retrieve Mode");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_PROG_DEFAULT_BARCODE_RETRIEVE_MODE;
	ParamUnit.SetParamID((UINT)(ParamID));	
	AOIDataDefine.BuildEnableDisableParamUni(ParamUnit);
	ParamUnit.SetValue_SEL(Ptr->m_DefaultBarcodeRetrieveMode);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	
	/*
	//路徑
	ParamUnit = CParamUni();
	str = _T("Log Folder");
	strCaption = LoadMultiLanguageString(str, str);
	ParamID = SYSTEM_AOI_FOLDER_LOG;
	ParamUnit.SetParamID((UINT)(ParamID));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_STR(Ptr->m_AOILogDirectory);
	ParamUnit.SetDesction(strCaption);	
	ParamUnit.SetBtnWndPtr(&m_BtnCtrl);
	ParamList.push_back(ParamUnit);
	*/
	return true;
}
//-------------------------------------------------------------------------------------//
bool CSystemDefaultPane::BuildParamListWnd()
{
	SetActParamUni(NULL);
	CThisListCtrl_32 &ListCtrl = m_ParamListCtrl;	
	m_StopParamListBeSelected = true;
	JetAPI::ClearListCtrl(ListCtrl, FALSE);
	m_StopParamListBeSelected = false;
	if ( NULL == m_SysParameterPtr ) { return true; }

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
bool CSystemDefaultPane::BuildParamListWndHeader()
{
	CString str;
	int   nCol = 0;
	int width  = 64;
	int width2 = 64;
	RECT      Rect;	
	const int Align = LVCFMT_LEFT;//LVCFMT_CENTER;LVCFMT_RIGHT, LVCFMT_LEFT
	CThisListCtrl_32 &ListCtrl = m_ParamListCtrl;

	ListCtrl.GetClientRect(&Rect);
	width = (Rect.right-Rect.left-8)/8;
	width2 = 48;
	str = AOIDataDefine.GetIndexText();	
	ListCtrl.InsertColumn(nCol, str, Align, width2);
	nCol ++;

	width2 = width*3;
	str = _T("Item");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(nCol, str, Align, width2);
	nCol ++;

	width2 = width*5;
	str = _T("Information");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(nCol, str, Align, width2);
	nCol ++;
	return true;
}
//-------------------------------------------------------------------------------------//
void CSystemDefaultPane::OnItemchangedParamListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
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
	ExecItemchangedParamListWnd(m_ParamListCtrl, nItem);	
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CSystemDefaultPane::OnDblclkParamListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
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
void CSystemDefaultPane::OnKillfocusParamEdit() 
{
	// TODO: Add your control notification handler code here
	if ( m_EditCtrl.IsWindowVisible() == FALSE ) { return; }
	ExecUpdateParamByEdit();
	m_EditCtrl.ShowWindow(SW_HIDE);	
	m_EditCtrl.SetWindowText(_T(""));
}
//-------------------------------------------------------------------------------------//
void CSystemDefaultPane::OnSelchangeParamCombo() 
{
	// TODO: Add your control notification handler code here
	if ( ExecUpdateParamByCombox() == false )
	{	return; }
	m_ComboxCtrl.ShowWindow(SW_HIDE);	
	JetAPI::ClearCombox(m_ComboxCtrl);	
}
//-------------------------------------------------------------------------------------//
void CSystemDefaultPane::OnKillfocusParamCombo() 
{
	// TODO: Add your control notification handler code here
	if ( m_ComboxCtrl.IsWindowVisible() == FALSE ) { return; }
	ExecUpdateParamByCombox();
	m_ComboxCtrl.ShowWindow(SW_HIDE);	
	JetAPI::ClearCombox(m_ComboxCtrl);	
}
//-------------------------------------------------------------------------------------//
void CSystemDefaultPane::OnFolderBtn() 
{
	// TODO: Add your control notification handler code here
	m_BtnFolder.ShowWindow(SW_HIDE);
	CParamUni *ParamPtr = GetActParamUni();
	if ( NULL == ParamPtr ) { return; }	
	CWnd      *BtnWndPtr = ParamPtr->GetBtnWndPtr();
	if ( NULL == BtnWndPtr ) { return; }

	CString ItemText = ParamPtr->GetParamText();
	SYSTEM_PARAM_ID SysParam = (SYSTEM_PARAM_ID)(ParamPtr->GetParamID());	

	if ( JetAPI::OpenFolderDialog(this, ItemText) == false ) { return; }
	if ( CAOIDataCollect::SetSystemParameterStringByID(SysParam, *m_SysParameterPtr, ItemText) == false )
	{	return ; }
	ParamPtr->SetNewValue(ItemText);
	SetActParamUni(NULL);
	
	const int nItem = ParamPtr->GetItemIndex();
	const int nSubItem = ParamPtr->GetSubItemIndex();	
	CThisListCtrl_32 *pListCtrl = (CThisListCtrl_32*)(ParamPtr->GetListCtrl());

	if ( NULL != pListCtrl )
	{
		const int ItemCount = pListCtrl->GetItemCount();
		pListCtrl->SetItemText(nItem, nSubItem, ItemText);
		pListCtrl->SetFocus();
	}
	return ;
}
//-------------------------------------------------------------------------------------//
void CSystemDefaultPane::SetDescriptionText(const CParamUni *Ptr)
{
	UINT CtrlID = SYSDEFAULT_INFO_EDIT;
	if ( NULL == Ptr )
	{
		CWnd::SetDlgItemText(CtrlID, _T(""));
		return ;
	}
	CWnd::SetDlgItemText(CtrlID, Ptr->GetDesction());
}
//-------------------------------------------------------------------------------------//
bool CSystemDefaultPane::ExecItemchangedParamListWnd(CThisListCtrl_32 &ListCtrl, int nItem)
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
	{	m_BtnFolder.ShowWindow(SW_HIDE);	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CSystemDefaultPane::ExecDblclkParamListWnd(CThisListCtrl_32 &ListCtrl, int nItem, int nSubItem)
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
	m_BtnFolder.ShowWindow(SW_HIDE);
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
bool CSystemDefaultPane::ExecUpdateParamByEdit()
{
	CParamUni *ParamPtr = GetActParamUni();
	if ( NULL == ParamPtr ) { return true; }	
	PARAM_DATA_TYPE  DataType = ParamPtr->GetDataType();	
	if ( PARAM_DATA_SEL == DataType ) { return true; }
	SetActParamUni(NULL);

	CString ItemText;	
	SYSTEM_PARAM_ID SysParam = (SYSTEM_PARAM_ID)(ParamPtr->GetParamID());	
	m_EditCtrl.GetWindowText(ItemText);
	if ( ParamPtr->SetNewValue(ItemText) == false )
	{		
		ItemText = ParamPtr->GetParamText();
		m_EditCtrl.SetWindowText(ItemText);
		return false;
	}
	if ( CAOIDataCollect::SetSystemParameterStringByID(SysParam, *m_SysParameterPtr, ItemText) == false )
	{	return false; }

	CThisListCtrl_32 *pListCtrl = (CThisListCtrl_32*)(ParamPtr->GetListCtrl());
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
bool CSystemDefaultPane::ExecUpdateParamByCombox()
{
	CParamUni *ParamPtr = GetActParamUni();
	if ( NULL == ParamPtr ) { return true; }	
	PARAM_DATA_TYPE  DataType = ParamPtr->GetDataType();
	if ( PARAM_DATA_SEL != DataType ) { return true; }
	SetActParamUni(NULL);

	CString ItemText;	
	SYSTEM_PARAM_ID SysParam = (SYSTEM_PARAM_ID)(ParamPtr->GetParamID());
	const int nCurSel = m_ComboxCtrl.GetCurSel();
	if ( nCurSel < 0 ) { return true; }
	const int Param = (int)(m_ComboxCtrl.GetItemData(nCurSel));
	if ( Param == ParamPtr->GetSelParam() ) { return false; }
	ItemText.Format(_T("%d"),Param);	
	if ( ParamPtr->SetNewValue_SEL(Param) == false )
	{	return false; }
	if ( CAOIDataCollect::SetSystemParameterStringByID(SysParam, *m_SysParameterPtr, ItemText) == false )
	{	return false; }

	ItemText = ParamPtr->GetParamText();
	CThisListCtrl_32 *pListCtrl = (CThisListCtrl_32*)(ParamPtr->GetListCtrl());
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
bool CSystemDefaultPane::ExecReleaseParamCtrl()
{	
	m_EditCtrl.ShowWindow(SW_HIDE);	
	m_BtnFolder.ShowWindow(SW_HIDE);
	m_ComboxCtrl.ShowWindow(SW_HIDE);

	m_EditCtrl.SetWindowText(_T(""));
	m_ParamListCtrl.SetFocus();
	return true;
}
//-------------------------------------------------------------------------------------//