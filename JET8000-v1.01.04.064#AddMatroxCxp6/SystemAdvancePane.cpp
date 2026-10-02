// SystemAdvancePane.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "SystemAdvancePane.h"
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
// CSystemAdvancePane dialog
//-------------------------------------------------------------------------------------//
CSystemAdvancePane::CSystemAdvancePane(CWnd* pParent /*=NULL*/)
	: CDialog(CSystemAdvancePane::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSystemAdvancePane)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ParamActPtr = NULL;
	m_SysParameterPtr = NULL;	
	m_StopParamListBeSelected = false;
}
//-------------------------------------------------------------------------------------//
void CSystemAdvancePane::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSystemAdvancePane)
	DDX_Control(pDX, SYSADVANCE_PARAM_EDIT, m_EditCtrl);
	DDX_Control(pDX, SYSADVANCE_PARAM_BTN, m_BtnCtrl);
	DDX_Control(pDX, SYSADVANCE_PARAM_COMBO, m_ComboxCtrl);	
	DDX_Control(pDX, SYSADVANCE_PARAM_LIST_WND, m_ParamListCtrl);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CSystemAdvancePane, CDialog)
	//{{AFX_MSG_MAP(CSystemAdvancePane)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(LVN_ITEMCHANGED, SYSADVANCE_PARAM_LIST_WND, OnItemchangedParamListWnd)
	ON_NOTIFY(NM_DBLCLK, SYSADVANCE_PARAM_LIST_WND, OnDblclkParamListWnd)
	ON_EN_KILLFOCUS(SYSADVANCE_PARAM_EDIT, OnKillfocusParamEdit)
	ON_CBN_KILLFOCUS(SYSADVANCE_PARAM_COMBO, OnKillfocusParamCombo)
	ON_CBN_SELCHANGE(SYSADVANCE_PARAM_COMBO, OnSelchangeParamCombo)
	ON_BN_CLICKED(SYSADVANCE_PARAM_BTN, OnParamBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CSystemAdvancePane message handlers
//-------------------------------------------------------------------------------------//
BOOL CSystemAdvancePane::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SwitchMultiLanguage();
	JetAPI::InitialListCtrl(m_ParamListCtrl);	
	CWnd::ShowWindow(SW_SHOWNORMAL);
	BuildParamListWndHeader();
	BuildParamListWnd();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CSystemAdvancePane::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CSystemAdvancePane::OnSize(UINT nType, int cx, int cy) 
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

	WndPtr = CWnd::GetDlgItem(SYSADVANCE_INFO_EDIT);
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
void CSystemAdvancePane::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if ( TRUE == bShow )
	{	
		BuildParamListWnd();	
	}
}
//-------------------------------------------------------------------------------------//
void CSystemAdvancePane::OnOK() 
{
	// TODO: Add extra validation here
	return ;
	CDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CSystemAdvancePane::OnCancel() 
{
	// TODO: Add extra cleanup here
	return ;
	CDialog::OnCancel();
}
//-------------------------------------------------------------------------------------//
void CSystemAdvancePane::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_SYSTEM_ADVANCE_PANE");
	//---------------------------------------------------------------------------------//
	WndID = IDD_SYSTEM_ADVANCE_PANE;
	WndKey = _T("IDD_SYSTEM_ADVANCE_PANE");
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
CString CSystemAdvancePane::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_SYSTEM_ADVANCE_PANE");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
void CSystemAdvancePane::SetSystemParameterPtr(TSystemParameter *Ptr)
{
	m_SysParameterPtr = Ptr;
}
//-------------------------------------------------------------------------------------//
CParamUni* CSystemAdvancePane::GetActParamUni()
{
	return m_ParamActPtr;
}
//-------------------------------------------------------------------------------------//
void CSystemAdvancePane::SetActParamUni(CParamUni *Ptr)
{
	m_ParamActPtr = Ptr;
}
//-------------------------------------------------------------------------------------//
bool CSystemAdvancePane::BuildParamList()
{	
	CThisListCtrl_29 &ListCtrl = m_ParamListCtrl;
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
	USER_LEVEL_MODE  UserLevelMode = AOIDataCollect.GetCurrentUserLevel();//使用者權限			
	const int ComputerCPUCoreNumber = AOIDataCollect.GetComputerCPUCoreNumber();	
	CParamList &ParamList = m_ParamList;

	ParamList.clear();

	//設備機種樣式	
	ParamUnit = CParamUni();
	str = _T("Machine Model Type");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_MACHINE_MODEL_TYPE;
	ParamUnit.SetParamID((UINT)(ParamID));
#ifndef ODM_BRAND_VERSION
	ParamUnit.AddSelItem(MACHINE_MODEL_6500, _T("JET6500"));
	ParamUnit.AddSelItem(MACHINE_MODEL_8000, AOI3D_APP_NAME);
#else
	ParamUnit.AddSelItem(MACHINE_MODEL_8000, AOI3D_APP_NAME);
#endif//ODM_BRAND_VERSION
	ParamUnit.SetValue_SEL(Ptr->m_MachineModelType);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//設備相機方向
	ParamUnit = CParamUni();
	str = _T("Machine Camera Side");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_MACHINE_CAMERA_SIDE;
	ParamUnit.SetParamID((UINT)(ParamID));
	ParamUnit.AddSelItem(MACHINE_CAMERA_TOP, _T("Top"));
	ParamUnit.AddSelItem(MACHINE_CAMERA_BOT, _T("Bot"));
	ParamUnit.SetValue_SEL(Ptr->m_MachineCameraSide);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);		

	//自動重測次數上限
	ParamUnit = CParamUni();
	str = _T("Auto Retry Max Count");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_AUTO_RETRY_MAX_COUNT;
	ParamUnit.SetParamID((UINT)(ParamID));
	for ( i=0; i<=100; i++ )
	{
		if ( FN_DISABLE == i ) 
		{	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);	}
		else
		{	strValue.Format(_T("%d"), i);	}
		ParamUnit.AddSelItem(i, strValue);		
	}
	ParamUnit.SetValue_SEL(Ptr->m_AutoRetryMaxCount);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);		
	
	//自動設定DLP樣板	
	ParamUnit = CParamUni();
	str = _T("Auto Setup 3D Pattern");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_AUTO_SETUP_DLP_PATTERN;
	ParamUnit.SetParamID((UINT)(ParamID));
	AOIDataDefine.BuildEnableDisableParamUni(ParamUnit);	
	ParamUnit.SetValue_SEL(Ptr->m_AutoSetupDlpPattern);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//提前載入專案離線圖檔	
	ParamUnit = CParamUni();
	str = _T("Pre Load Project Offline Image");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_PRE_LOAD_PROJECT_OFFLINE_IMAGE;
	ParamUnit.SetParamID((UINT)(ParamID));
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);	ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);	ParamUnit.AddSelItem(FN_ENABLE, strValue);
	ParamUnit.SetValue_SEL(Ptr->m_PreLoadProjectOfflineImage);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//自動釋放區域影像
	ParamUnit = CParamUni();
	str = _T("Auto Release Field Frame");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_AUTO_RELEASE_FIELD_FRAME_BUFFER;
	ParamUnit.SetParamID((UINT)(ParamID));
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);	ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);	ParamUnit.AddSelItem(FN_ENABLE, strValue);
	ParamUnit.SetValue_SEL(Ptr->m_AutoReleaseFieldFrameBuffer);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//自動釋放離線編程區域影像
	ParamUnit = CParamUni();
	str = _T("Auto Release Offline Field Frame");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_AUTO_RELEASE_OFFLINE_FIELD_FRAME_BUFFER;
	ParamUnit.SetParamID((UINT)(ParamID));
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);	ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);	ParamUnit.AddSelItem(FN_ENABLE, strValue);
	ParamUnit.SetValue_SEL(Ptr->m_AutoReleaseOfflineFieldFrameBuffer);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	
	
	//編輯線尺寸的層級
	ParamUnit = CParamUni();
	str = _T("Edit Line Size Level");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_EDIT_LINE_SIZE_LEVEL;
	ParamUnit.SetParamID((UINT)(ParamID));	
	strValue.Format(_T("%d"), 1);	ParamUnit.AddSelItem(1, strValue);
	strValue.Format(_T("%d"), 2);	ParamUnit.AddSelItem(2, strValue);
	strValue.Format(_T("%d"), 3);	ParamUnit.AddSelItem(3, strValue);
	ParamUnit.SetValue_SEL(Ptr->m_EditLineSizeLevel);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//線上調機保留最久時間-分鐘 	
	ParamUnit = CParamUni();
	str = _T("Online Tuning Keep Max Time");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_ONLINE_TUNING_KEEP_MAX_TIME;
	ParamUnit.SetParamID((UINT)(ParamID));	
	ParamUnit.SetValue_INT(Ptr->m_OnlineTuningKeepMaxTime);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);		
	
	//線上調機儲存最多數量-片數 
	ParamUnit = CParamUni();
	str = _T("Online Tuning Saved Max Count");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_ONLINE_TUNING_SAVED_MAX_COUNT;
	ParamUnit.SetParamID((UINT)(ParamID));	
	ParamUnit.SetValue_INT(Ptr->m_OnlineTuningSavedMaxCount, 0);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//檢測結束顯示結果列表
	ParamUnit = CParamUni();
	str = _T("Inspection Finish Show Result List");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_INSPECTION_FINISH_SHOW_RESULT_LIST;
	ParamUnit.SetParamID((UINT)(ParamID));	
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);		ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);		ParamUnit.AddSelItem(FN_ENABLE, strValue);	
	ParamUnit.SetValue_SEL(Ptr->m_InspectionFinishShowResultList);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//case SYSTEM_MODEL_DEFAULT_WND_LEVEL://模組預設檢測框等級		
	//可切換至專案3D畫面
	ParamUnit = CParamUni();
	str = _T("Switch Project 3D Frame");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_SWITCH_PROJECT_3D_FRAME;
	ParamUnit.SetParamID((UINT)(ParamID));	
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);		ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);		ParamUnit.AddSelItem(FN_ENABLE, strValue);	
	ParamUnit.SetValue_SEL(Ptr->m_SwitchProject3DFrame);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//自動切換至檢測框3D畫面	
	ParamUnit = CParamUni();
	str = _T("Auto Switch Wnd 3D Frame");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_AUTO_SWITCH_WND_3D_FRAME_MODE;
	ParamUnit.SetParamID((UINT)(ParamID));	
	strValue = AOIDataDefine.GetAutoSwitchWnd3DFrameModeText(AUTO_SWITCH_WND_3D_FRAME_DISABLE);		ParamUnit.AddSelItem(AUTO_SWITCH_WND_3D_FRAME_DISABLE, strValue);
	strValue = AOIDataDefine.GetAutoSwitchWnd3DFrameModeText(AUTO_SWITCH_WND_3D_FRAME_ENABLE);		ParamUnit.AddSelItem(AUTO_SWITCH_WND_3D_FRAME_ENABLE, strValue);
	strValue = AOIDataDefine.GetAutoSwitchWnd3DFrameModeText(AUTO_SWITCH_WND_3D_FRAME_BY_SIZE);		ParamUnit.AddSelItem(AUTO_SWITCH_WND_3D_FRAME_BY_SIZE, strValue);
	strValue = AOIDataDefine.GetAutoSwitchWnd3DFrameModeText(AUTO_SWITCH_WND_3D_FRAME_BY_TYPE);		ParamUnit.AddSelItem(AUTO_SWITCH_WND_3D_FRAME_BY_TYPE, strValue);	
	strValue = AOIDataDefine.GetAutoSwitchWnd3DFrameModeText(AUTO_SWITCH_WND_3D_FRAME_BY_GROUP_CHANGE);		ParamUnit.AddSelItem(AUTO_SWITCH_WND_3D_FRAME_BY_GROUP_CHANGE, strValue);	
	ParamUnit.SetValue_SEL(Ptr->m_AutoSwitchWnd3DFrameMode);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//自動切換至檢測框3D畫面尺寸上限-um		
	ParamUnit = CParamUni();
	str = _T("Auto Switch Wnd 3D Frame Size Limit");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_AUTO_SWITCH_WND_3D_FRAME_SIZE_LIMIT;
	ParamUnit.SetParamID((UINT)(ParamID));		
	ParamUnit.SetValue_INT(Ptr->m_AutoSwitchWnd3DFrameSizeLimit, 0);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//顯示零件-整板零件
	ParamUnit = CParamUni();
	str = _T("Show Full-Map Components");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_SHOW_COMPONENT_FULL_MAP;
	ParamUnit.SetParamID((UINT)(ParamID));	
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);		ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);		ParamUnit.AddSelItem(FN_ENABLE, strValue);	
	ParamUnit.SetValue_SEL(Ptr->m_ShowComponentFullMap);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	
	
	//僅顯示瑕疵零件(線上畫面)
	ParamUnit = CParamUni();
	str = _T("Show Defect Only Components");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_SHOW_COMPONENT_DEFECT_ONLY;
	ParamUnit.SetParamID((UINT)(ParamID));	
	AOIDataDefine.BuildEnableDisableParamUni(ParamUnit);	
	ParamUnit.SetValue_SEL(Ptr->m_ShowComponentDefectOnly);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//拼圖參數-填補尺寸	
	ParamUnit = CParamUni();
	str = _T("Stitch Image Padding Size");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_STITCH_IMAGE_PADDING_SIZE;
	ParamUnit.SetParamID((UINT)(ParamID));		
	ParamUnit.SetValue_INT(Ptr->m_StitchImagePaddingSize, 0);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//模組預設SOT樣式	
	ParamUnit = CParamUni();
	str = _T("Model Default Transistor Type");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_MODEL_DEFAULT_TRANSISTOR_TYPE;
	ParamUnit.SetParamID((UINT)(ParamID));	
	strValue = _T("Electrode");		ParamUnit.AddSelItem(MODEL_TYPE_TRANSISTOR, strValue);
	strValue = _T("Lead");		ParamUnit.AddSelItem(MODEL_TYPE_LEAD_TRANSISTOR, strValue);	
	ParamUnit.SetValue_SEL(Ptr->m_ModelDefaultTransistorType);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//專案使用本機資料夾
	ParamUnit = CParamUni();
	str = _T("Project Local Folder");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_PROJECT_LOCAL_FOLDER_ENABLED;
	ParamUnit.SetParamID((UINT)(ParamID));	
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);		ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);		ParamUnit.AddSelItem(FN_ENABLE, strValue);	
	ParamUnit.SetValue_SEL(Ptr->m_ProjectLocalFolderEnabled);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//部分複製專案資料庫
	ParamUnit = CParamUni();
	str = _T("Partial Copy Project Library");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_PARTIAL_COPY_PROJECT_LIBRARY;
	ParamUnit.SetParamID((UINT)(ParamID));	
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);		ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);		ParamUnit.AddSelItem(FN_ENABLE, strValue);	
	ParamUnit.SetValue_SEL(Ptr->m_PartialCopyProjectLibrary);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//自動重整模組底圖檔案	
	ParamUnit = CParamUni();
	str = _T("Auto Arrange Model Bk Image Files");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_AUTO_ARRANGE_MODEL_BK_IMAGE_FILES;
	ParamUnit.SetParamID((UINT)(ParamID));	
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);		ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);		ParamUnit.AddSelItem(FN_ENABLE, strValue);	
	ParamUnit.SetValue_SEL(Ptr->m_AutoArrangeModelBkImageFiles);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//自動複製SPC零件圖檔
	ParamUnit = CParamUni();
	str = _T("Auto Copy Spc Component Image Files");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_AUTO_COPY_SPC_COMPONENT_IMAGE_FILES;
	ParamUnit.SetParamID((UINT)(ParamID));	
	AOIDataDefine.BuildEnableDisableParamUni(ParamUnit);	
	ParamUnit.SetValue_SEL(Ptr->m_AutoCopySpcComponentImageFiles);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//自動跳過3D影像	
	ParamUnit = CParamUni();
	str = _T("Auto Bypass Grab 3D Frame");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_AUTO_BYPASS_GRAB_3D_FRAME;
	ParamUnit.SetParamID((UINT)(ParamID));	
	AOIDataDefine.BuildEnableDisableParamUni(ParamUnit);	
	ParamUnit.SetValue_SEL(Ptr->m_AutoBypassGrab3DFrame);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//確認專案定位點狀態
	ParamUnit = CParamUni();
	str = _T("Check Project Fd Ready");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_CHECK_PROJECT_FD_READY;
	ParamUnit.SetParamID((UINT)(ParamID));	
	AOIDataDefine.BuildEnableDisableParamUni(ParamUnit);	
	ParamUnit.SetValue_SEL(Ptr->m_CheckProjectFdReady);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//鎖住模組本體的位置	
	ParamUnit = CParamUni();
	str = _T("Lock Model Body Position");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_LOCK_MODEL_BODY_POSITION;
	ParamUnit.SetParamID((UINT)(ParamID));	
	AOIDataDefine.BuildEnableDisableParamUni(ParamUnit);	
	ParamUnit.SetValue_SEL(Ptr->m_LockModelBodyPosition);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);
	
	//最多未判定檢測檔案數 
	ParamUnit = CParamUni();
	str = _T("Max Uncheck Test File Count");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_MAX_UNCHECK_TEST_FILE_COUNT;
	ParamUnit.SetParamID((UINT)(ParamID));		
	ParamUnit.SetValue_INT(Ptr->m_MaxUncheckTestFileCount, 0);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//啟用零件條碼確認	
	ParamUnit = CParamUni();
	str = _T("Confirm Component Barcode");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_CONFIRM_COMPONENT_BARCODE_ENABLED;
	ParamUnit.SetParamID((UINT)(ParamID));	
	AOIDataDefine.BuildEnableDisableParamUni(ParamUnit);	
	ParamUnit.SetValue_SEL(Ptr->m_ConfirmComponentBarcodeEnabled);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);
	
	//儲存JPEG的質量(001~100)
	ParamUnit = CParamUni();
	str = _T("Save JPEG Quality");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_SAVE_JPEG_QUALITY;
	ParamUnit.SetParamID((UINT)(ParamID));		
	ParamUnit.SetValue_INT(Ptr->m_SaveJpegQuality, 1, 100);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//啟用螢幕鎖住
	ParamUnit = CParamUni();
	str = _T("Enable Lock Screen");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_LOCK_SCREEN_ENABLED;
	ParamUnit.SetParamID((UINT)(ParamID));	
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);		ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);		ParamUnit.AddSelItem(FN_ENABLE, strValue);	
	ParamUnit.SetValue_SEL(Ptr->m_LockScreenEnabled);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//鎖住螢幕鍵號	
	ParamUnit = CParamUni();
	str = _T("Lock Screen Key");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_LOCK_SCREEN_KEY_ID;
	ParamUnit.SetParamID((UINT)(ParamID));	
	for ( i='A'; i<='Z'; i++ )
	{		
		strValue.Format(_T("%c"), (TCHAR)(i));		
		ParamUnit.AddSelItem(i, strValue);		
	}
	ParamUnit.SetValue_SEL(Ptr->m_LockScreenKeyID);
	//ParamUnit.SetValue_SEL((int)('B'));
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	
	
	//在線調機-軟體條碼
	ParamUnit = CParamUni();
	str = _T("Enable Online Tuning Barcode");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_ONLINE_TUNING_ENABLE_BARCODE;
	ParamUnit.SetParamID((UINT)(ParamID));	
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);		ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);		ParamUnit.AddSelItem(FN_ENABLE, strValue);	
	ParamUnit.SetValue_SEL(Ptr->m_OnlineTuningEnableBarcode);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//是否顯示PLC安全檢知設定介面
	ParamUnit = CParamUni();
	str = _T("Show PLC Safty Setting UI");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_SHOW_PLC_SAFTY_SETTING_UI;
	ParamUnit.SetParamID((UINT)(ParamID));	
	AOIDataDefine.BuildEnableDisableParamUni(ParamUnit);	
	ParamUnit.SetValue_SEL(Ptr->m_ShowPlcSaftySettingUI);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//是否顯示演算法OffsetL的參數 	
	ParamUnit = CParamUni();
	str = _T("Show Alg Offset-L Parameter");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_SHOW_ALG_OFFSET_L_PARAM;
	ParamUnit.SetParamID((UINT)(ParamID));	
	AOIDataDefine.BuildEnableDisableParamUni(ParamUnit);	
	ParamUnit.SetValue_SEL(Ptr->m_ShowAlgOffsetLParam);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//是否顯示演算法OffsetA的參數
	ParamUnit = CParamUni();
	str = _T("Show Alg Offset-A Parameter");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_SHOW_ALG_OFFSET_A_PARAM;
	ParamUnit.SetParamID((UINT)(ParamID));	
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);		ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);		ParamUnit.AddSelItem(FN_ENABLE, strValue);	
	ParamUnit.SetValue_SEL(Ptr->m_ShowAlgOffsetAParam);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//是否顯示演算法亮度比例的比例參數	
	ParamUnit = CParamUni();
	str = _T("Show Alg Bright-Ratio Scale Param");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_SHOW_ALG_BRIGHT_RATIO_SCALE_PARAM;
	ParamUnit.SetParamID((UINT)(ParamID));	
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);		ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);		ParamUnit.AddSelItem(FN_ENABLE, strValue);	
	ParamUnit.SetValue_SEL(Ptr->m_ShowAlgBrightRatioScaleParam);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//是否顯示模組屬性參數	
	ParamUnit = CParamUni();
	str = _T("Show Model Property Parameter");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_SHOW_MODEL_PROPERTY_PARAM;
	ParamUnit.SetParamID((UINT)(ParamID));	
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);		ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);		ParamUnit.AddSelItem(FN_ENABLE, strValue);	
	ParamUnit.SetValue_SEL(Ptr->m_ShowModelPropertyParam);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//計算焦距平滑尺寸
	ParamUnit = CParamUni();
	str = _T("Calc Focus Image Smooth Size");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_CALC_FOCUS_SMOOTH_SIZE;
	ParamUnit.SetParamID((UINT)(ParamID));		
	ParamUnit.SetValue_INT(Ptr->m_CalcFocusSmoothSize, 0, 256);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//計算焦距模式
	ParamUnit = CParamUni();
	str = _T("Calc Focus Image Mode");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_CALC_FOCUS_MODE;
	ParamUnit.SetParamID((UINT)(ParamID));	
	ParamUnit.AddSelItem(CALC_FOCUS_AMPLITUDE, _T("Amplitude"));	
	ParamUnit.AddSelItem(CALC_FOCUS_VARIANCE, _T("Variance"));	
	ParamUnit.AddSelItem(CALC_FOCUS_SUM_MODULES_DIFFERENCE, _T("SMD"));	
	ParamUnit.AddSelItem(CALC_FOCUS_SQUARED_GRADIENT, _T("Squard Gradient"));	
	ParamUnit.AddSelItem(CALC_FOCUS_TENENGRAD, _T("Tenengrad"));
	ParamUnit.AddSelItem(CALC_FOCUS_LAPLACIAN, _T("Laplacian"));
	ParamUnit.AddSelItem(CALC_FOCUS_PIXEL_DIFFERENCE, _T("Pixel Difference"));		
	ParamUnit.SetValue_SEL(Ptr->m_CalcFocusMode);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	
	
	//UI視窗字型增加大小
	ParamUnit = CParamUni();
	str = _T("Wnd Font Add Size");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_UI_WND_FONT_ADD_SIZE;
	ParamUnit.SetParamID((UINT)(ParamID));	
	for ( i=0; i<=4; i+=2 )
	{
		strValue.Format(_T("+%d"), i);
		if ( 0 == i )
		{	strValue = AOIDataDefine.GetDisableText();	}
		ParamUnit.AddSelItem(i, strValue);
	}
	ParamUnit.SetValue_SEL(Ptr->m_UIWndFontAddSize);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//m_UIDockWndSlideSteps;//UI駐停視窗滑動步長
	//SYSTEM_UI_DOCK_WND_SLIDE_STEPS://UI駐停視窗滑動步長	

	//UI啟用PCB出板按鈕	
	ParamUnit = CParamUni();
	str = _T("Enable PCB Out Button");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_UI_ENABLE_PCB_OUT_BUTTON;
	ParamUnit.SetParamID((UINT)(ParamID));	
	AOIDataDefine.BuildEnableDisableParamUni(ParamUnit);	
	ParamUnit.SetValue_SEL(Ptr->m_UIEnablePCBOutButton);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//SYSTEM_COPY_HUGE_FILES_MODE://複製大量檔案模式

	//連續貼上模式
	ParamUnit = CParamUni();
	str = _T("Continue Paste Mode");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_CONTINUE_PASTE_MODE;
	ParamUnit.SetParamID((UINT)(ParamID));	
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);		ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);		ParamUnit.AddSelItem(FN_ENABLE, strValue);	
	ParamUnit.SetValue_SEL(Ptr->m_ContinuePasteMode);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//啟用Resin高度對齊-軍達3D對位	
	ParamUnit = CParamUni();
	str = _T("Resign Height Align");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_RESIN_HEIGHT_ALIGN_ENABLED;
	ParamUnit.SetParamID((UINT)(ParamID));	
	AOIDataDefine.BuildEnableDisableParamUni(ParamUnit);	
	ParamUnit.SetValue_SEL(Ptr->m_ResinHeightAlignEnabled);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//啟用模組影像Cad偏移補償um	
	ParamUnit = CParamUni();
	str = _T("Moel Image Cad Offset");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_MODEL_IMAGE_CAD_OFFSET_ENABLED;
	ParamUnit.SetParamID((UINT)(ParamID));	
	AOIDataDefine.BuildEnableDisableParamUni(ParamUnit);	
	ParamUnit.SetValue_SEL(Ptr->m_ModelImageCadOffsetEnabled);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//啟用Honeywell Swift Decoder
	ParamUnit = CParamUni();
	str = _T("Honeywell Swift Decoder");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_HONEYWELL_SWIFT_DECODER_ENABLED;
	ParamUnit.SetParamID((UINT)(ParamID));	
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);		ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);		ParamUnit.AddSelItem(FN_ENABLE, strValue);	
	ParamUnit.SetValue_SEL(Ptr->m_HoneywellSwiftDecoderEnabled);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//外部複製檔案啟用
	ParamUnit = CParamUni();
	str = _T("External Copy File Enabled");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_EXTERNAL_COPY_FILE_ENABLED;
	ParamUnit.SetParamID((UINT)(ParamID));	
	AOIDataDefine.BuildEnableDisableParamUni(ParamUnit);	
	ParamUnit.SetValue_SEL(Ptr->m_ExternalCopyFileEnabled);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//外部複製檔案軟體名稱	
	ParamUnit = CParamUni();
	str = _T("External Copy File App Name");
	strCaption = LoadMultiLanguageString(str, str);
	ParamID = SYSTEM_EXTERNAL_COPY_FILE_APP_NAME;
	ParamUnit.SetParamID((UINT)(ParamID));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_STR(Ptr->m_ExternalCopyFileAppName);
	ParamUnit.SetDesction(strCaption);
	ParamUnit.SetBtnWndPtr(&m_BtnCtrl);	
	ParamList.push_back(ParamUnit);	

	//外部複製檔案輸出資料夾	
	ParamUnit = CParamUni();
	str = _T("External Copy File Send Folder");
	strCaption = LoadMultiLanguageString(str, str);
	ParamID = SYSTEM_EXTERNAL_COPY_FILE_SEND_FOLDER;
	ParamUnit.SetParamID((UINT)(ParamID));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_STR(Ptr->m_ExternalCopyFileSendFolder);
	ParamUnit.SetDesction(strCaption);
	ParamUnit.SetBtnWndPtr(&m_BtnCtrl);	
	ParamList.push_back(ParamUnit);	

	//Cpk圖表啟用
	ParamUnit = CParamUni();
	str = _T("CPK Chart Enabled");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_CPK_CHART_ENABLED;
	ParamUnit.SetParamID((UINT)(ParamID));	
	AOIDataDefine.BuildEnableDisableParamUni(ParamUnit);	
	ParamUnit.SetValue_SEL(Ptr->m_CpkChartEnabled);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//Wnd 檢測框跟隨旋轉
	ParamUnit = CParamUni();
	str = _T("Enable Wnd Rotation Followed");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_WND_ROTATION_FOLLOWED;
	ParamUnit.SetParamID((UINT)(ParamID));
	AOIDataDefine.BuildEnableDisableParamUni(ParamUnit);
	ParamUnit.SetValue_SEL(Ptr->m_WndRotationFollowed);
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
bool CSystemAdvancePane::BuildParamListWnd()
{
	SetActParamUni(NULL);
	CThisListCtrl_29 &ListCtrl = m_ParamListCtrl;	
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
bool CSystemAdvancePane::BuildParamListWndHeader()
{
	CString str;
	int   nCol = 0;
	int width  = 64;
	int width2 = 64;
	RECT      Rect;	
	const int Align = LVCFMT_LEFT;//LVCFMT_CENTER;LVCFMT_RIGHT, LVCFMT_LEFT
	CThisListCtrl_29 &ListCtrl = m_ParamListCtrl;

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
void CSystemAdvancePane::OnItemchangedParamListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
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
void CSystemAdvancePane::OnDblclkParamListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
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
void CSystemAdvancePane::SetDescriptionText(const CParamUni *Ptr)
{
	UINT CtrlID = SYSADVANCE_INFO_EDIT;
	if ( NULL == Ptr )
	{
		CWnd::SetDlgItemText(CtrlID, _T(""));
		return ;
	}
	CWnd::SetDlgItemText(CtrlID, Ptr->GetDesction());
}
//-------------------------------------------------------------------------------------//
bool CSystemAdvancePane::ExecItemchangedParamListWnd(CThisListCtrl_29 &ListCtrl, int nItem)
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
bool CSystemAdvancePane::ExecDblclkParamListWnd(CThisListCtrl_29 &ListCtrl, int nItem, int nSubItem)
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
void CSystemAdvancePane::OnKillfocusParamEdit() 
{
	// TODO: Add your control notification handler code here
	if ( m_EditCtrl.IsWindowVisible() == FALSE ) { return; }
	ExecUpdateParamByEdit();
	m_EditCtrl.ShowWindow(SW_HIDE);	
	m_EditCtrl.SetWindowText(_T(""));
}
//-------------------------------------------------------------------------------------//
void CSystemAdvancePane::OnKillfocusParamCombo() 
{
	// TODO: Add your control notification handler code here
	if ( m_ComboxCtrl.IsWindowVisible() == FALSE ) { return; }
	ExecUpdateParamByCombox();
	m_ComboxCtrl.ShowWindow(SW_HIDE);	
	JetAPI::ClearCombox(m_ComboxCtrl);	
}
//-------------------------------------------------------------------------------------//
void CSystemAdvancePane::OnSelchangeParamCombo() 
{
	// TODO: Add your control notification handler code here
	if ( ExecUpdateParamByCombox() == false )
	{	return; }
	m_ComboxCtrl.ShowWindow(SW_HIDE);	
	JetAPI::ClearCombox(m_ComboxCtrl);	
}
//-------------------------------------------------------------------------------------//
void CSystemAdvancePane::OnParamBtn() 
{
	// TODO: Add your control notification handler code here
	m_BtnCtrl.ShowWindow(SW_HIDE);
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
	CThisListCtrl_29 *pListCtrl = (CThisListCtrl_29*)(ParamPtr->GetListCtrl());

	if ( NULL != pListCtrl )
	{
		const int ItemCount = pListCtrl->GetItemCount();
		pListCtrl->SetItemText(nItem, nSubItem, ItemText);
		pListCtrl->SetFocus();
	}
}
//-------------------------------------------------------------------------------------//
bool CSystemAdvancePane::ExecUpdateParamByEdit()
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
	
	CThisListCtrl_29 *pListCtrl = (CThisListCtrl_29*)(ParamPtr->GetListCtrl());
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
bool CSystemAdvancePane::ExecReleaseParamCtrl()
{
	m_BtnCtrl.ShowWindow(SW_HIDE);
	m_EditCtrl.ShowWindow(SW_HIDE);	
	m_ComboxCtrl.ShowWindow(SW_HIDE);

	m_EditCtrl.SetWindowText(_T(""));
	m_ParamListCtrl.SetFocus();
	return true;
}
//-------------------------------------------------------------------------------------//
BOOL CSystemAdvancePane::PreTranslateMessage(MSG* pMsg) 
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
bool CSystemAdvancePane::ExecUpdateParamByCombox()
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
	CThisListCtrl_29 *pListCtrl = (CThisListCtrl_29*)(ParamPtr->GetListCtrl());
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
LRESULT CSystemAdvancePane::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class	
	return CDialog::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
