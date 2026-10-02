// SystemBasicPane.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "SystemBasicPane.h"
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
// CSystemBasicPane dialog
//-------------------------------------------------------------------------------------//
CSystemBasicPane::CSystemBasicPane(CWnd* pParent /*=NULL*/)
	: CDialog(CSystemBasicPane::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSystemBasicPane)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ParamActPtr = NULL;
	m_SysParameterPtr = NULL;	
	m_StopParamListBeSelected = false;
}
//-------------------------------------------------------------------------------------//
void CSystemBasicPane::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSystemBasicPane)
	DDX_Control(pDX, SYSBASIC_PARAM_EDIT, m_EditCtrl);
	DDX_Control(pDX, SYSBASIC_PARAM_BTN, m_BtnCtrl);
	DDX_Control(pDX, SYSBASIC_PARAM_COMBO, m_ComboxCtrl);	
	DDX_Control(pDX, SYSBASIC_PARAM_LIST_WND, m_ParamListCtrl);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CSystemBasicPane, CDialog)
	//{{AFX_MSG_MAP(CSystemBasicPane)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(NM_DBLCLK, SYSBASIC_PARAM_LIST_WND, OnDblclkParamListWnd)
	ON_NOTIFY(NM_KILLFOCUS, SYSBASIC_PARAM_LIST_WND, OnKillfocusParamListWnd)
	ON_NOTIFY(LVN_ITEMCHANGED, SYSBASIC_PARAM_LIST_WND, OnItemchangedParamListWnd)
	ON_EN_KILLFOCUS(SYSBASIC_PARAM_EDIT, OnKillfocusParamEdit)
	ON_CBN_SELCHANGE(SYSBASIC_PARAM_COMBO, OnSelchangeParamCombo)
	ON_CBN_KILLFOCUS(SYSBASIC_PARAM_COMBO, OnKillfocusParamCombo)
	ON_BN_CLICKED(SYSBASIC_PARAM_BTN, OnParamBtn)		
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CSystemBasicPane message handlers
//-------------------------------------------------------------------------------------//
BOOL CSystemBasicPane::OnInitDialog() 
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
void CSystemBasicPane::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CSystemBasicPane::OnSize(UINT nType, int cx, int cy) 
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

	WndPtr = CWnd::GetDlgItem(SYSBASIC_INFO_EDIT);
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
void CSystemBasicPane::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if ( TRUE == bShow )
	{	
		BuildParamListWnd();	
	}
}
//-------------------------------------------------------------------------------------//
void CSystemBasicPane::OnOK() 
{
	// TODO: Add extra validation here
	return ;
	CDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CSystemBasicPane::OnCancel() 
{
	// TODO: Add extra cleanup here
	return ;
	CDialog::OnCancel();
}
//-------------------------------------------------------------------------------------//
void CSystemBasicPane::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_SYSTEM_BASIC_PANE");
	//---------------------------------------------------------------------------------//
	WndID = IDD_SYSTEM_BASIC_PANE;
	WndKey = _T("IDD_SYSTEM_BASIC_PANE");
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
CString CSystemBasicPane::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_SYSTEM_BASIC_PANE");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
void CSystemBasicPane::SetSystemParameterPtr(TSystemParameter *Ptr)
{
	m_SysParameterPtr = Ptr;
}
//-------------------------------------------------------------------------------------//
CParamUni* CSystemBasicPane::GetActParamUni()
{
	return m_ParamActPtr;
}
//-------------------------------------------------------------------------------------//
void CSystemBasicPane::SetActParamUni(CParamUni *Ptr)
{
	m_ParamActPtr = Ptr;
}
//-------------------------------------------------------------------------------------//
bool CSystemBasicPane::BuildParamList()
{
	CThisListCtrl_30 &ListCtrl = m_ParamListCtrl;
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
	CString strTime = AOIDataDefine.GetTimeText();
	CString strBarcode = AOIDataDefine.GetBarcodeText();
	const int nSubItem = 1;	
	const int ComputerCPUCoreNumber = AOIDataCollect.GetComputerCPUCoreNumber();
	CParamList &ParamList = m_ParamList;

	ParamList.clear();

	//本機廠區
	ParamUnit = CParamUni();
	str = _T("Location");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_MACHINE_LOCATION;	
	ParamUnit.SetParamID((UINT)(ParamID));	
	ParamUnit.SetValue_STR(Ptr->m_MachineLocation);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//本機棟別
	ParamUnit = CParamUni();
	str = _T("Building");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_MACHINE_BUILDING;
	ParamUnit.SetParamID((UINT)(ParamID));		
	ParamUnit.SetValue_STR(Ptr->m_MachineBuilding);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//本機樓層
	ParamUnit = CParamUni();
	str = _T("Floor");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_MACHINE_FLOOR;
	ParamUnit.SetParamID((UINT)(ParamID));		
	ParamUnit.SetValue_STR(Ptr->m_MachineFloor);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);
	
	//本機車間
	ParamUnit = CParamUni();
	str = _T("Room Name");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_MACHINE_ROOM;
	ParamUnit.SetParamID((UINT)(ParamID));		
	ParamUnit.SetValue_STR(Ptr->m_MachineRoom);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//本機線別
	ParamUnit = CParamUni();
	str = _T("Line Name");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_MACHINE_LINE;
	ParamUnit.SetParamID((UINT)(ParamID));		
	ParamUnit.SetValue_STR(Ptr->m_MachineLine);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//本機線別-B軌
	ParamUnit = CParamUni();
	str = _T("Line Name LB");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_MACHINE_LINE_LB;
	ParamUnit.SetParamID((UINT)(ParamID));		
	ParamUnit.SetValue_STR(Ptr->m_MachineLine_LB);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//本機站別
	ParamUnit = CParamUni();
	str = _T("Station");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_MACHINE_STATION;
	ParamUnit.SetParamID((UINT)(ParamID));		
	ParamUnit.SetValue_STR(Ptr->m_MachineStation);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);
	
	//本機站別-B軌
	ParamUnit = CParamUni();
	str = _T("Station LB");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_MACHINE_STATION_LB;
	ParamUnit.SetParamID((UINT)(ParamID));		
	ParamUnit.SetValue_STR(Ptr->m_MachineStation_LB);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//本機序號
	ParamUnit = CParamUni();
	str = _T("Series Number");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_MACHINE_SN;
	ParamUnit.SetParamID((UINT)(ParamID));		
	ParamUnit.SetValue_STR(Ptr->m_MachineSN);
	if ( AOIDataCollect.UserLevel_MachineSN() == false )
	{	ParamUnit.SetReadOnly(true); }
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//機台型號
	ParamUnit = CParamUni();
	str = _T("Machine Name");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_MACHINE_NAME;
	ParamUnit.SetParamID((UINT)(ParamID));		
	ParamUnit.SetValue_STR(Ptr->m_MachineName);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//機台廠商	
	ParamUnit = CParamUni();
	str = _T("Machine Vendor");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_MACHINE_VENDOR;
	ParamUnit.SetParamID((UINT)(ParamID));		
	ParamUnit.SetValue_STR(Ptr->m_MachineVendor);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//機台別名
	ParamUnit = CParamUni();
	str = _T("Machine Alias");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_MACHINE_ALIAS;
	ParamUnit.SetParamID((UINT)(ParamID));		
	ParamUnit.SetValue_STR(Ptr->m_MachineAlias);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

#ifndef ODM_BRAND_VERSION
	//客戶編號	
	ParamUnit = CParamUni();
	str = _T("Customer ID");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_AOI_CUSTOMER_ID;
	ParamUnit.SetParamID((UINT)(ParamID));
	strValue = AOIDataDefine.GetAOICustomerIDText(AOI_CUSTOMER_ID_JET_TWN);			ParamUnit.AddSelItem(AOI_CUSTOMER_ID_JET_TWN, strValue);
	strValue = AOIDataDefine.GetAOICustomerIDText(AOI_CUSTOMER_ID_PEGATRON_TWN);	ParamUnit.AddSelItem(AOI_CUSTOMER_ID_PEGATRON_TWN, strValue);
	strValue = AOIDataDefine.GetAOICustomerIDText(AOI_CUSTOMER_ID_KINPO_YUEYANG);	ParamUnit.AddSelItem(AOI_CUSTOMER_ID_KINPO_YUEYANG, strValue);
	strValue = AOIDataDefine.GetAOICustomerIDText(AOI_CUSTOMER_ID_FOXCONN_LONGHUA);	ParamUnit.AddSelItem(AOI_CUSTOMER_ID_FOXCONN_LONGHUA, strValue);	
	ParamUnit.SetValue_SEL(Ptr->m_AOICustomerID);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	
#endif//ODM_BRAND_VERSION

	//電腦IP
	ParamUnit = CParamUni();
	str = _T("Host PC IP");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_IP_HOST_COMPUTER;
	ParamUnit.SetParamID((UINT)(ParamID));		
	ParamUnit.SetValue_STR(Ptr->m_IPHostComputer);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	
	
	//多國語系
	MULTI_LANGUAGE_MODE MultiLanguageMode;
	ParamUnit = CParamUni();
	str = _T("Language");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_MULTI_LANGUAGE_MODE;
	ParamUnit.SetParamID((UINT)(ParamID));		
	MultiLanguageMode = MULTI_LANGUAGE_ENGLISH;
	strValue = AOIDataDefine.GetMultiLanguageModeText(MultiLanguageMode);
	ParamUnit.AddSelItem(MultiLanguageMode, strValue);
	MultiLanguageMode = MULTI_LANGUAGE_CHINESE_TRAD;
	strValue = AOIDataDefine.GetMultiLanguageModeText(MultiLanguageMode);
	ParamUnit.AddSelItem(MultiLanguageMode, strValue);
	MultiLanguageMode = MULTI_LANGUAGE_CHINESE_SIMP;
	strValue = AOIDataDefine.GetMultiLanguageModeText(MultiLanguageMode);
	ParamUnit.AddSelItem(MultiLanguageMode, strValue);	
	MultiLanguageMode = MULTI_LANGUAGE_LOCAL;
	strValue = AOIDataDefine.GetMultiLanguageModeText(MultiLanguageMode);
	ParamUnit.AddSelItem(MultiLanguageMode, strValue);
	ParamUnit.SetValue_SEL(Ptr->m_MultiLanguageMode);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	
	
	//是否儲存現在狀態訊息
	ParamUnit = CParamUni();
	str = _T("Save Project Text Report");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_SAVE_PROJECT_REPORT_TEXT;
	ParamUnit.SetParamID((UINT)(ParamID));
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);	ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);	ParamUnit.AddSelItem(FN_ENABLE, strValue);
	ParamUnit.SetValue_SEL(Ptr->m_SaveProjectReportText);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//儲存專案文字檔報告檔名
	ParamUnit = CParamUni();	
	str = _T("Project Text Report Filename Mode");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_SAVE_PROJECT_REPORT_TEXT_FILENAME_MODE;
	ParamUnit.SetParamID((UINT)(ParamID));
	strValue.Format(_T("%s_%s"), strBarcode, strTime);
	ParamUnit.AddSelItem(SAVE_TEXT_FILENAME_DATETIME, strTime);
	ParamUnit.AddSelItem(SAVE_TEXT_FILENAME_BARCODE_DATETIME, strValue);	
	ParamUnit.SetValue_SEL(Ptr->m_SaveProjectReportTextFilenameMode);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//儲存客戶報告模式
	ParamUnit = CParamUni();
	str = _T("Save Customer Report File");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_SAVE_CUSTOMER_REPORT_FILE;
	ParamUnit.SetParamID((UINT)(ParamID));
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);	ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);	ParamUnit.AddSelItem(FN_ENABLE, strValue);
	ParamUnit.SetValue_SEL(Ptr->m_SaveCustomerReportFile);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//儲存專案SPC檔案模式	
	ParamUnit = CParamUni();
	str = _T("Save Project SPC File Mode");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_SAVE_PROJECT_SPC_FILE_MODE;
	ParamUnit.SetParamID((UINT)(ParamID));
	//ParamUnit.AddSelItem(SAVE_SPC_FILE_JSON_VRS, _T("VRS"));//Remove It For IPS
	ParamUnit.AddSelItem(SAVE_SPC_FILE_JSON_RSM, _T("RSM"));	
	ParamUnit.SetValue_SEL(Ptr->m_SaveProjectSpcFileMode);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//儲存專案SPC資料庫模式	
	ParamUnit = CParamUni();
	str = _T("Save Project SPC Library Mode");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_SAVE_PROJECT_SPC_LIBRARY_MODE;
	ParamUnit.SetParamID((UINT)(ParamID));
	ParamUnit.AddSelItem(SAVE_SPC_OTHER_FILE_OFF, _T("Off"));
	ParamUnit.AddSelItem(SAVE_SPC_OTHER_FILE_AUTO, _T("Auto"));	
	ParamUnit.AddSelItem(SAVE_SPC_OTHER_FILE_ASK, _T("Ask"));	
	ParamUnit.SetValue_SEL(Ptr->m_SaveProjectSpcLibraryMode);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//多段檢測啟用
	ParamUnit = CParamUni();
	str = _T("Enable Multi District Mode");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_MULTI_DISTRICT_MODE_ENABLED;
	ParamUnit.SetParamID((UINT)(ParamID));
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);	ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);	ParamUnit.AddSelItem(FN_ENABLE, strValue);
	ParamUnit.SetValue_SEL(Ptr->m_MultiDistrictModeEnabled);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//是否儲存專案檢測框數據檔案	
	ParamUnit = CParamUni();
	str = _T("Save Project Wnd Reading Report");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_SAVE_PROJECT_REPORT_WND_READING;
	ParamUnit.SetParamID((UINT)(ParamID));
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);	ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);	ParamUnit.AddSelItem(FN_ENABLE, strValue);
	ParamUnit.SetValue_SEL(Ptr->m_SaveProjectReportWndReading);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//與上一站連線模式
	ParamUnit = CParamUni();
	str = _T("Last Station Line Mode");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_CONNECT_LAST_STATION_MODE;
	ParamUnit.SetParamID((UINT)(ParamID));
	intValue = LAST_STATION_LINE_MODE_2;	strValue.Format(_T("%d"), intValue);	ParamUnit.AddSelItem(intValue, strValue);
	intValue = LAST_STATION_LINE_MODE_2_4;	strValue.Format(_T("%d"), intValue);	ParamUnit.AddSelItem(intValue, strValue);	
	intValue = LAST_STATION_LINE_MODE_4;	strValue.Format(_T("%d"), intValue);	ParamUnit.AddSelItem(intValue, strValue);		
	ParamUnit.SetValue_SEL(Ptr->m_LastStationLineMode);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//A軌道運轉模式
	ParamUnit = CParamUni();
	str = _T("Lane Work Mode Lane A");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_LANE_WORK_MODEL_LA;
	ParamUnit.SetParamID((UINT)(ParamID));
	strValue = AOIDataDefine.GetLaneWorkModeText(LANE_WORK_DISABLE);	ParamUnit.AddSelItem(LANE_WORK_DISABLE, strValue);
	strValue = AOIDataDefine.GetLaneWorkModeText(LANE_WORK_RUN);	ParamUnit.AddSelItem(LANE_WORK_RUN, strValue);
	strValue = AOIDataDefine.GetLaneWorkModeText(LANE_WORK_BYPASS);	ParamUnit.AddSelItem(LANE_WORK_BYPASS, strValue);	
	ParamUnit.SetValue_SEL(Ptr->m_LaneWorkMode_LA);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);
	
	//B軌道運轉模式
	ParamUnit = CParamUni();
	str = _T("Lane Work Mode Lane B");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_LANE_WORK_MODEL_LB;
	ParamUnit.SetParamID((UINT)(ParamID));
	strValue = AOIDataDefine.GetLaneWorkModeText(LANE_WORK_DISABLE);	ParamUnit.AddSelItem(LANE_WORK_DISABLE, strValue);
	strValue = AOIDataDefine.GetLaneWorkModeText(LANE_WORK_RUN);	ParamUnit.AddSelItem(LANE_WORK_RUN, strValue);
	strValue = AOIDataDefine.GetLaneWorkModeText(LANE_WORK_BYPASS);	ParamUnit.AddSelItem(LANE_WORK_BYPASS, strValue);	
	ParamUnit.SetValue_SEL(Ptr->m_LaneWorkMode_LB);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//線上檢測介面模式	
	ParamUnit = CParamUni();
	str = _T("Online Form View Mode");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_ONLINE_FORM_VIEW_MODE;
	ParamUnit.SetParamID((UINT)(ParamID));
	ParamUnit.AddSelItem(ONLINE_FROMVIEW_ONE, _T("Normal"));
	ParamUnit.AddSelItem(ONLINE_FROMVIEW_DUAL, _T("Dual Lane"));
	ParamUnit.SetValue_SEL(Ptr->m_OnlineFormViewMode);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//雙塔燈模式	
	ParamUnit = CParamUni();
	str = _T("Multi-Tower Light");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_MULTI_TOWER_LIGHT_MODE;
	ParamUnit.SetParamID((UINT)(ParamID));
	intValue = 1;	strValue.Format(_T("%d"), intValue);	ParamUnit.AddSelItem(intValue, strValue);
	intValue = 2;	strValue.Format(_T("%d"), intValue);	ParamUnit.AddSelItem(intValue, strValue);	
	ParamUnit.SetValue_SEL(Ptr->m_MultiTowerLight);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//確認PCB板移走次數
	ParamUnit = CParamUni();
	str = _T("Check PCB Removed Count");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_CHECK_PCB_REMOVED_COUNT;
	ParamUnit.SetParamID((UINT)(ParamID));	
	ParamUnit.SetValue_INT(Ptr->m_CheckPCBRemovedCount, 1, 100);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	
	
	//下一站連接輸送帶樣式
	ParamUnit = CParamUni();
	str = _T("Next Buffer Type");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_NEXT_CONNECTED_BUFFER_TYPE;
	ParamUnit.SetParamID((UINT)(ParamID));
	strValue = AOIDataDefine.GetConnectedBufferTypeText(CONNECTED_BUFFER_FIXED);	ParamUnit.AddSelItem(CONNECTED_BUFFER_FIXED, strValue);
	strValue = AOIDataDefine.GetConnectedBufferTypeText(CONNECTED_BUFFER_MOVABLE);	ParamUnit.AddSelItem(CONNECTED_BUFFER_MOVABLE, strValue);	
	ParamUnit.SetValue_SEL(Ptr->m_NextConnectedBufferType);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//多專案檢測次序模式
	ParamUnit = CParamUni();
	str = _T("Multi Project Test Order");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_MULTI_PROJECT_TEST_ORDER_MODE;
	ParamUnit.SetParamID((UINT)(ParamID));
	strValue = AOIDataDefine.GetMultiProjectTestOrderText(MULTI_PROJECT_TEST_ORDER_BY_MARK);	ParamUnit.AddSelItem(MULTI_PROJECT_TEST_ORDER_BY_MARK, strValue);
	//strValue = AOIDataDefine.GetMultiProjectTestOrderText(MULTI_PROJECT_TEST_ORDER_BY_TURN);	ParamUnit.AddSelItem(MULTI_PROJECT_TEST_ORDER_BY_TURN, strValue);
	strValue = AOIDataDefine.GetMultiProjectTestOrderText(MULTI_PROJECT_TEST_ORDER_ONE_CYCLE_A_B);	ParamUnit.AddSelItem(MULTI_PROJECT_TEST_ORDER_ONE_CYCLE_A_B, strValue);	
	strValue = AOIDataDefine.GetMultiProjectTestOrderText(MULTI_PROJECT_TEST_ORDER_ONE_CYCLE_B_A);	ParamUnit.AddSelItem(MULTI_PROJECT_TEST_ORDER_ONE_CYCLE_B_A, strValue);
	ParamUnit.SetValue_SEL(Ptr->m_MultiProjectTestOrderMode);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//進板前移動相機頭	
	ParamUnit = CParamUni();
	str = _T("Move Camera Before PCB-In");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_MOVE_CAMERA_BEFORE_PCB_IN;
	ParamUnit.SetParamID((UINT)(ParamID));
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);	ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);	ParamUnit.AddSelItem(FN_ENABLE, strValue);		
	ParamUnit.SetValue_SEL(Ptr->m_MoveCameraBeforePCBIn);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//進板使用相機影像模式
	ParamUnit = CParamUni();
	str = _T("PCB-In Use Camera Image Mode");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_PCB_IN_USE_CAMERA_IMAGE_MODE;
	ParamUnit.SetParamID((UINT)(ParamID));
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);	ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);	ParamUnit.AddSelItem(FN_ENABLE, strValue);		
	ParamUnit.SetValue_SEL(Ptr->m_PCBInUseCameraImageMode);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//檢測前夾板模式	
	ParamUnit = CParamUni();
	str = _T("Clamp PCB Before Test Mode");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_CLAMP_PCB_BEFORE_TEST_MODE;
	ParamUnit.SetParamID((UINT)(ParamID));	
	strValue = AOIDataDefine.GetFuncExecModeText(FUNC_EXEC_OFF);		ParamUnit.AddSelItem(FUNC_EXEC_OFF, strValue);
	strValue = AOIDataDefine.GetFuncExecModeText(FUNC_EXEC_AUTO);		ParamUnit.AddSelItem(FUNC_EXEC_AUTO, strValue);
	strValue = AOIDataDefine.GetFuncExecModeText(FUNC_EXEC_ASK);		ParamUnit.AddSelItem(FUNC_EXEC_ASK, strValue);	
	ParamUnit.SetValue_SEL(Ptr->m_ClampPcbBeforeTestMode);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//定位點延遲時間
	ParamUnit = CParamUni();
	str = _T("Grab Fiducial Delay Time");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_GRAB_FIDUCIAL_DELAY_TIME;
	ParamUnit.SetParamID((UINT)(ParamID));	
	ParamUnit.SetValue_DBL(Ptr->m_GrabFiducialDelayTime_ms, 0);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	
	
	//PCB出板方向
	ParamUnit = CParamUni();
	str = _T("PCB Out Direction");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_PCB_OUT_DIRECTION;
	ParamUnit.SetParamID((UINT)(ParamID));	
	strValue = AOIDataDefine.GetPCBOutDirectionText(PCB_OUT_DIR_FORWARD);	ParamUnit.AddSelItem(PCB_OUT_DIR_FORWARD, strValue);
	strValue = AOIDataDefine.GetPCBOutDirectionText(PCB_OUT_DIR_BACKWARD);	ParamUnit.AddSelItem(PCB_OUT_DIR_BACKWARD, strValue);	
	strValue = AOIDataDefine.GetPCBOutDirectionText(PCB_OUT_DIR_BACKWARD_OUT);	ParamUnit.AddSelItem(PCB_OUT_DIR_BACKWARD_OUT, strValue);
	ParamUnit.SetValue_SEL(Ptr->m_PCBOutDirection);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);
	
	//忽略上一站訊號-回板使用
	ParamUnit = CParamUni();
	str = _T("Bypass Last Signal");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_BYPASS_LAST_SIGNAL;
	ParamUnit.SetParamID((UINT)(ParamID));	
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);	ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);	ParamUnit.AddSelItem(FN_ENABLE, strValue);		
	ParamUnit.SetValue_SEL(Ptr->m_BypassLastSignal);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);
	
	//忽略下一站訊號-回板使用	
	ParamUnit = CParamUni();
	str = _T("Bypass Next Signal");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_BYPASS_NEXT_SIGNAL;
	ParamUnit.SetParamID((UINT)(ParamID));	
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);	ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);	ParamUnit.AddSelItem(FN_ENABLE, strValue);		
	ParamUnit.SetValue_SEL(Ptr->m_BypassNextSignal);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//PCB OK/NG訊號延遲時間-ms
	ParamUnit = CParamUni();
	str = _T("PCB OK NG Signal Delay Time");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_PCB_OK_NG_SIGNAL_DELAY_TIME;
	ParamUnit.SetParamID((UINT)(ParamID));		
	ParamUnit.SetValue_INT(Ptr->m_PCBOKNGSignalDelayTime, 0);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//使用者登入使用
	ParamUnit = CParamUni();
	str = _T("Enable User Login");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_USER_LOGIN_ENABLED;
	ParamUnit.SetParamID((UINT)(ParamID));	
	strValue = AOIDataDefine.GetUserLoginModeText(USER_LOGIN_DISABLE);		ParamUnit.AddSelItem(USER_LOGIN_DISABLE, strValue);
	strValue = AOIDataDefine.GetUserLoginModeText(USER_LOGIN_OPERATOR);		ParamUnit.AddSelItem(USER_LOGIN_OPERATOR, strValue);	
	strValue = AOIDataDefine.GetUserLoginModeText(USER_LOGIN_ENGINEER);		ParamUnit.AddSelItem(USER_LOGIN_ENGINEER, strValue);	
	ParamUnit.SetValue_SEL(Ptr->m_UserLoginMode);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//使用者登入選項
	ParamUnit = CParamUni();
	str = _T("User Login Options");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_USER_LOGIN_OPTIONS;
	ParamUnit.SetParamID((UINT)(ParamID));
	strValue = AOIDataDefine.GetUserLoginOptionsText(USER_LOGIN_OPTIONS_PASSWORD);		ParamUnit.AddSelItem(USER_LOGIN_OPTIONS_PASSWORD, strValue);
	strValue = AOIDataDefine.GetUserLoginOptionsText(USER_LOGIN_OPTIONS_FINGERPRINT);	ParamUnit.AddSelItem(USER_LOGIN_OPTIONS_FINGERPRINT, strValue);
	strValue = AOIDataDefine.GetUserLoginOptionsText(USER_LOGIN_OPTIONS_FINGERPRINT_ONLY);	ParamUnit.AddSelItem(USER_LOGIN_OPTIONS_FINGERPRINT_ONLY, strValue);
	ParamUnit.SetValue_SEL(Ptr->m_UserLoginOptions);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//開啟專案模式
	ParamUnit = CParamUni();
	str = _T("Open Project Mode");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_OPEN_PROJECT_MODE;
	ParamUnit.SetParamID((UINT)(ParamID));	
	strValue = AOIDataDefine.GetOpenProjectModeText(OPEN_PROJECT_FILE);		ParamUnit.AddSelItem(OPEN_PROJECT_FILE, strValue);
	strValue = AOIDataDefine.GetOpenProjectModeText(OPEN_PROJECT_CODE);		ParamUnit.AddSelItem(OPEN_PROJECT_CODE, strValue);	
	ParamUnit.SetValue_SEL(Ptr->m_OpenProjectMode);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);
	
	//開啟專案底圖編號
	ParamUnit = CParamUni();
	str = _T("Open Project Map Index");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_OPEN_PROJECT_MAP_INDEX;
	ParamUnit.SetParamID((UINT)(ParamID));	
	for ( i=0; i<FRAME_MAX_COUNT; i++ )
	{	
		strValue .Format(_T("%d"), i+1);
		ParamUnit.AddSelItem(i, strValue);	
	}	
	ParamUnit.SetValue_SEL(Ptr->m_OpenProjectMapIndex);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	
	
	//驗證專案模式	
	ParamUnit = CParamUni();
	str = _T("Verify Project Mode");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_VERIFY_PROJECT_MODE;
	ParamUnit.SetParamID((UINT)(ParamID));	
	strValue = AOIDataDefine.GetVerifyProjectModeText(VERIFY_PROJECT_DISABLE);		ParamUnit.AddSelItem(VERIFY_PROJECT_DISABLE, strValue);
	strValue = AOIDataDefine.GetVerifyProjectModeText(VERIFY_PROJECT_FILENAME);		ParamUnit.AddSelItem(VERIFY_PROJECT_FILENAME, strValue);	
	ParamUnit.SetValue_SEL(Ptr->m_VerifyProjectMode);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);
	
	//驗證專案的檔案
	ParamUnit = CParamUni();
	str = _T("Verify Project Filename");
	strCaption = LoadMultiLanguageString(str, str);
	ParamID = SYSTEM_VERIFY_PROJECT_FILENAME;
	ParamUnit.SetParamID((UINT)(ParamID));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_STR(Ptr->m_VerifyProjectFilename);
	ParamUnit.SetDesction(strCaption);	
	ParamUnit.SetBtnWndPtr(&m_BtnCtrl);
	ParamList.push_back(ParamUnit);
	
	//模組名稱使用料號 
	ParamUnit = CParamUni();
	str = _T("Model Name Use Part Number");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_MODEL_NAME_USE_PART_NUMBER;
	ParamUnit.SetParamID((UINT)(ParamID));	
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);	ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);	ParamUnit.AddSelItem(FN_ENABLE, strValue);		
	ParamUnit.SetValue_SEL(Ptr->m_ModelNameUsePartNumber);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

#ifdef ONLINE_OPEN_PROJECT_USE
	//線上開專案模式		
	ParamUnit = CParamUni();
	str = _T("Online Open Project Mode");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_ONLINE_OPEN_PROJECT_MODE;
	ParamUnit.SetParamID((UINT)(ParamID));	
	strValue = AOIDataDefine.GetOnlineOpenProjectModeText(ONLINE_OPEN_PROJECT_DISABLE);			ParamUnit.AddSelItem(ONLINE_OPEN_PROJECT_DISABLE, strValue);
	strValue = AOIDataDefine.GetOnlineOpenProjectModeText(ONLINE_OPEN_PROJECT_BARCODE_DEVICE);	ParamUnit.AddSelItem(ONLINE_OPEN_PROJECT_BARCODE_DEVICE, strValue);	
	strValue = AOIDataDefine.GetOnlineOpenProjectModeText(ONLINE_OPEN_PROJECT_BARCODE_HANDHELD);	ParamUnit.AddSelItem(ONLINE_OPEN_PROJECT_BARCODE_HANDHELD, strValue);	
	ParamUnit.SetValue_SEL(Ptr->m_OnlineOpenProjectMode);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);
	
	//線上開專案-相機條碼
	ParamUnit = CParamUni();
	str = _T("Online Open Project Camera Barcode");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_ONLINE_OPEN_PROJECT_CAMERA_BARCODE;
	ParamUnit.SetParamID((UINT)(ParamID));	
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);		ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);		ParamUnit.AddSelItem(FN_ENABLE, strValue);	
	ParamUnit.SetValue_SEL(Ptr->m_OnlineOpenProjectCameraBarcode);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//線上開專案外接條碼取像模式	
	ParamUnit = CParamUni();
	str = _T("Online Open Project Barcode Device Grab Mode");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_ONLINE_OPEN_PROJECT_BARCODE_DEVICE_GRAB_MODE;
	ParamUnit.SetParamID((UINT)(ParamID));	
	strValue = AOIDataDefine.GetBarcodeDeviceGrabModeText(BARCODE_DEVICE_GRAB_BEFORE_PCB_IN);	ParamUnit.AddSelItem(BARCODE_DEVICE_GRAB_BEFORE_PCB_IN, strValue);
	strValue = AOIDataDefine.GetBarcodeDeviceGrabModeText(BARCODE_DEVICE_GRAB_WHILE_PCB_IN);	ParamUnit.AddSelItem(BARCODE_DEVICE_GRAB_WHILE_PCB_IN, strValue);	
	strValue = AOIDataDefine.GetBarcodeDeviceGrabModeText(BARCODE_DEVICE_GRAB_AFTER_PCB_IN);	ParamUnit.AddSelItem(BARCODE_DEVICE_GRAB_AFTER_PCB_IN, strValue);	
	strValue = AOIDataDefine.GetBarcodeDeviceGrabModeText(BARCODE_DEVICE_GRAB_BEFORE_INSPECT);	ParamUnit.AddSelItem(BARCODE_DEVICE_GRAB_BEFORE_INSPECT, strValue);		
	ParamUnit.SetValue_SEL(Ptr->m_OnlineOpenProjectBarcodeDeviceGrabMode);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);
#endif//ONLINE_OPEN_PROJECT_USE

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
bool CSystemBasicPane::BuildParamListWnd()
{	
	SetActParamUni(NULL);
	CThisListCtrl_30 &ListCtrl = m_ParamListCtrl;	
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
bool CSystemBasicPane::BuildParamListWndHeader()
{
	CString str;
	int   nCol = 0;
	int width  = 64;
	int width2 = 64;
	RECT      Rect;	
	const int Align = LVCFMT_LEFT;//LVCFMT_CENTER;LVCFMT_RIGHT, LVCFMT_LEFT
	CThisListCtrl_30 &ListCtrl = m_ParamListCtrl;

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
void CSystemBasicPane::OnDblclkParamListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
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
void CSystemBasicPane::OnKillfocusParamListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CSystemBasicPane::OnItemchangedParamListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
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
void CSystemBasicPane::OnKillfocusParamEdit() 
{
	// TODO: Add your control notification handler code here
	if ( m_EditCtrl.IsWindowVisible() == FALSE ) { return; }
	ExecUpdateParamByEdit();
	m_EditCtrl.ShowWindow(SW_HIDE);	
	m_EditCtrl.SetWindowText(_T(""));
}
//-------------------------------------------------------------------------------------//
void CSystemBasicPane::OnSelchangeParamCombo() 
{
	// TODO: Add your control notification handler code here
	if ( ExecUpdateParamByCombox() == false )
	{	return; }
	m_ComboxCtrl.ShowWindow(SW_HIDE);	
	JetAPI::ClearCombox(m_ComboxCtrl);	
}
//-------------------------------------------------------------------------------------//
void CSystemBasicPane::OnKillfocusParamCombo() 
{
	// TODO: Add your control notification handler code here
	if ( m_ComboxCtrl.IsWindowVisible() == FALSE ) { return; }
	ExecUpdateParamByCombox();
	m_ComboxCtrl.ShowWindow(SW_HIDE);	
	JetAPI::ClearCombox(m_ComboxCtrl);	
}
//-------------------------------------------------------------------------------------//
void CSystemBasicPane::OnParamBtn() 
{
	// TODO: Add your control notification handler code here
	m_BtnCtrl.ShowWindow(SW_HIDE);
	CParamUni *ParamPtr = GetActParamUni();
	if ( NULL == ParamPtr ) { return; }	
	CWnd      *BtnWndPtr = ParamPtr->GetBtnWndPtr();
	if ( NULL == BtnWndPtr ) { return; }

	int BtnMode = 0;
	const int FolderMode   = 1;
	const int FilenameMode = 2;
	CString ItemText = ParamPtr->GetParamText();
	SYSTEM_PARAM_ID SysParam = (SYSTEM_PARAM_ID)(ParamPtr->GetParamID());	

	DWORD dwFlags=0;
	BOOL  bOpenMode=TRUE;
	CString szFilters, szDefExt, szFileName;	
	switch ( SysParam )
	{
	default: BtnMode = FolderMode;	break;
	case SYSTEM_VERIFY_PROJECT_FILENAME:
		BtnMode = FilenameMode;
		bOpenMode=TRUE;
		szDefExt = _T("INI");
		szFileName = _T("*.INI");
		szFilters = _T("INI Files (*.INI)|*.INI|All Files (*.*)|*.*||");
		dwFlags = OFN_FILEMUSTEXIST;
		break;
	}
	if ( FolderMode == BtnMode )
	{
		if ( JetAPI::OpenFolderDialog(this, ItemText) == false ) { return; }
	}
	if ( FilenameMode == BtnMode )
	{		
		if ( JetAPI::OpenFileDialog(ItemText, bOpenMode, szDefExt, szFileName, dwFlags, szFilters, this) == false )
		{	return ; }
	}
	if ( CAOIDataCollect::SetSystemParameterStringByID(SysParam, *m_SysParameterPtr, ItemText) == false )
	{	return ; }
	ParamPtr->SetNewValue(ItemText);
	SetActParamUni(NULL);
	
	const int nItem = ParamPtr->GetItemIndex();
	const int nSubItem = ParamPtr->GetSubItemIndex();	
	CThisListCtrl_30 *pListCtrl = (CThisListCtrl_30*)(ParamPtr->GetListCtrl());

	if ( NULL != pListCtrl )
	{
		const int ItemCount = pListCtrl->GetItemCount();
		pListCtrl->SetItemText(nItem, nSubItem, ItemText);
		pListCtrl->SetFocus();
	}
	return ;
}
//-------------------------------------------------------------------------------------//
void CSystemBasicPane::SetDescriptionText(const CParamUni *Ptr)
{
	UINT CtrlID = SYSBASIC_INFO_EDIT;
	if ( NULL == Ptr )
	{
		CWnd::SetDlgItemText(CtrlID, _T(""));
		return ;
	}
	CWnd::SetDlgItemText(CtrlID, Ptr->GetDesction());
}
//-------------------------------------------------------------------------------------//
bool CSystemBasicPane::ExecItemchangedParamListWnd(CThisListCtrl_30 &ListCtrl, int nItem)
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
	{	m_BtnCtrl.ShowWindow(SW_HIDE);	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CSystemBasicPane::ExecDblclkParamListWnd(CThisListCtrl_30 &ListCtrl, int nItem, int nSubItem)
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
bool CSystemBasicPane::ExecUpdateParamByEdit()
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

	CThisListCtrl_30 *pListCtrl = (CThisListCtrl_30*)(ParamPtr->GetListCtrl());
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
bool CSystemBasicPane::ExecUpdateParamByCombox()
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
	CThisListCtrl_30 *pListCtrl = (CThisListCtrl_30*)(ParamPtr->GetListCtrl());
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
bool CSystemBasicPane::ExecReleaseParamCtrl()
{
	m_BtnCtrl.ShowWindow(SW_HIDE);
	m_EditCtrl.ShowWindow(SW_HIDE);	
	m_ComboxCtrl.ShowWindow(SW_HIDE);

	m_EditCtrl.SetWindowText(_T(""));
	m_ParamListCtrl.SetFocus();
	return true;
}
//-------------------------------------------------------------------------------------//
BOOL CSystemBasicPane::PreTranslateMessage(MSG* pMsg) 
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
LRESULT CSystemBasicPane::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class	
	return CDialog::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
