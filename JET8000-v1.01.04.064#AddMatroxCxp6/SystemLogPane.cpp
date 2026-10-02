// SystemLogPane.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "SystemLogPane.h"
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
// CSystemLogPane dialog
//-------------------------------------------------------------------------------------//
CSystemLogPane::CSystemLogPane(CWnd* pParent /*=NULL*/)
	: CDialog(CSystemLogPane::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSystemLogPane)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ParamActPtr = NULL;
	m_SysParameterPtr = NULL;	
	m_StopParamListBeSelected = false;
}
//-------------------------------------------------------------------------------------//
void CSystemLogPane::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSystemLogPane)
	DDX_Control(pDX, SYSLOG_PARAM_EDIT, m_EditCtrl);
	DDX_Control(pDX, SYSLOG_PARAM_BTN, m_BtnCtrl);
	DDX_Control(pDX, SYSLOG_PARAM_COMBO, m_ComboxCtrl);	
	DDX_Control(pDX, SYSLOG_PARAM_LIST_WND, m_ParamListCtrl);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CSystemLogPane, CDialog)
	//{{AFX_MSG_MAP(CSystemLogPane)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(LVN_ITEMCHANGED, SYSLOG_PARAM_LIST_WND, OnItemchangedParamListWnd)
	ON_NOTIFY(NM_DBLCLK, SYSLOG_PARAM_LIST_WND, OnDblclkParamListWnd)
	ON_EN_KILLFOCUS(SYSLOG_PARAM_EDIT, OnKillfocusParamEdit)
	ON_CBN_KILLFOCUS(SYSLOG_PARAM_COMBO, OnKillfocusParamCombo)
	ON_CBN_SELCHANGE(SYSLOG_PARAM_COMBO, OnSelchangeParamCombo)
	ON_BN_CLICKED(SYSLOG_PARAM_BTN, OnParamBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CSystemLogPane message handlers
//-------------------------------------------------------------------------------------//
BOOL CSystemLogPane::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SwitchMultiLanguage();
	JetAPI::InitialListCtrl(m_ParamListCtrl);	
	//CWnd::ShowWindow(SW_SHOWNORMAL);
	BuildParamListWndHeader();
	if ( CWnd::IsWindowVisible() )
	{	BuildParamListWnd(); }
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CSystemLogPane::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CSystemLogPane::OnSize(UINT nType, int cx, int cy) 
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

	WndPtr = CWnd::GetDlgItem(SYSLOG_INFO_EDIT);
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
void CSystemLogPane::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if ( TRUE == bShow )
	{	
		BuildParamListWnd();	
	}
}
//-------------------------------------------------------------------------------------//
void CSystemLogPane::OnOK() 
{
	// TODO: Add extra validation here
	return ;
	CDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CSystemLogPane::OnCancel() 
{
	// TODO: Add extra cleanup here
	return ;
	CDialog::OnCancel();
}
//-------------------------------------------------------------------------------------//
BOOL CSystemLogPane::PreTranslateMessage(MSG* pMsg) 
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
LRESULT CSystemLogPane::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void CSystemLogPane::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_SYSTEM_LOG_PANE");
	//---------------------------------------------------------------------------------//
	WndID = IDD_SYSTEM_LOG_PANE;
	WndKey = _T("IDD_SYSTEM_LOG_PANE");
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
CString CSystemLogPane::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_SYSTEM_LOG_PANE");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
void CSystemLogPane::SetSystemParameterPtr(TSystemParameter *Ptr)
{
	m_SysParameterPtr = Ptr;
}
//-------------------------------------------------------------------------------------//
CParamUni* CSystemLogPane::GetActParamUni()
{
	return m_ParamActPtr;
}
//-------------------------------------------------------------------------------------//
void CSystemLogPane::SetActParamUni(CParamUni *Ptr)
{
	m_ParamActPtr = Ptr;
}
//-------------------------------------------------------------------------------------//
bool CSystemLogPane::BuildParamList()
{
	CThisListCtrl_36 &ListCtrl = m_ParamListCtrl;
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
	const int ComputerCPUCoreNumber = AOIDataCollect.GetComputerCPUCoreNumber();
	CParamList &ParamList = m_ParamList;

	ParamList.clear();

	//是否儲存移動時間
	ParamUnit = CParamUni();
	str = _T("Save Moving Time Message");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_SAVE_MSG_MOVING_TIME;
	ParamUnit.SetParamID((UINT)(ParamID));
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);	ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);	ParamUnit.AddSelItem(FN_ENABLE, strValue);
	ParamUnit.SetValue_SEL(Ptr->m_SaveMovingTimeMessage);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//是否備份移動時間狀態訊息		
	ParamUnit = CParamUni();
	str = _T("Backup Moving Time Message");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_BACKUP_MSG_MOVING_TIME;
	ParamUnit.SetParamID((UINT)(ParamID));
	AOIDataDefine.BuildEnableDisableParamUni(ParamUnit);	
	ParamUnit.SetValue_SEL(Ptr->m_BackupMovingTimeMessage);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//是否儲存現在狀態訊息
	ParamUnit = CParamUni();
	str = _T("Save Current Process Message");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_SAVE_MSG_CURRENT_PROCESS;
	ParamUnit.SetParamID((UINT)(ParamID));
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);	ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);	ParamUnit.AddSelItem(FN_ENABLE, strValue);
	ParamUnit.SetValue_SEL(Ptr->m_SaveCurrentProcessMessage);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//是否備份現在狀態訊息
	ParamUnit = CParamUni();	
	str = _T("Backup Current Process Message");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_BACKUP_MSG_CURRENT_PROCESS;
	ParamUnit.SetParamID((UINT)(ParamID));
	AOIDataDefine.BuildEnableDisableParamUni(ParamUnit);	
	ParamUnit.SetValue_SEL(Ptr->m_BackupCurrentProcessMessage);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//是否儲存機台監控訊息
	ParamUnit = CParamUni();
	str = _T("Save Machine Monitor Message");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_SAVE_MSG_MACHINE_MONITOR;
	ParamUnit.SetParamID((UINT)(ParamID));
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);	ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);	ParamUnit.AddSelItem(FN_ENABLE, strValue);
	ParamUnit.SetValue_SEL(Ptr->m_SaveMachineMonitorMessage);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//是否儲存機台監控訊息-網頁用	
	ParamUnit = CParamUni();
	str = _T("Save Machine Monitor Message for Web");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_SAVE_MSG_MACHINE_MONITOR_WEB;
	ParamUnit.SetParamID((UINT)(ParamID));
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);	ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);	ParamUnit.AddSelItem(FN_ENABLE, strValue);
	ParamUnit.SetValue_SEL(Ptr->m_SaveMachineMonitorMessageWeb);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//是否儲存相機增加循環資料訊息	
	ParamUnit = CParamUni();
	str = _T("Save Camera Add Ring Buffer Log");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_SAVE_CAMERA_ADD_RING_BUFFER_LOG;
	ParamUnit.SetParamID((UINT)(ParamID));
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);	ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);	ParamUnit.AddSelItem(FN_ENABLE, strValue);
	ParamUnit.SetValue_SEL(Ptr->m_SaveCameraAddRingBufferLog);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//顯示記憶體未釋放訊息
	ParamUnit = CParamUni();
	str = _T("Show Memory Leak Message");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_SHOW_MEMORY_LEAK_MESSAGE;
	ParamUnit.SetParamID((UINT)(ParamID));
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);	ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);	ParamUnit.AddSelItem(FN_ENABLE, strValue);
	ParamUnit.SetValue_SEL(Ptr->m_ShowMemoryLeakMessage);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//是否送至DebugView視窗	
	ParamUnit = CParamUni();
	str = _T("Using Debug View Wnd");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_SEND_DEBUG_VIEW_STRING;
	ParamUnit.SetParamID((UINT)(ParamID));
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);	ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);	ParamUnit.AddSelItem(FN_ENABLE, strValue);
	ParamUnit.SetValue_SEL(Ptr->m_SendDebugViewString);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//是否儲存影像填滿執行緒訊息
	ParamUnit = CParamUni();
	str = _T("Save Slice Fill Thread Log");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_SAVE_SLICE_FILL_THREAD_LOG;
	ParamUnit.SetParamID((UINT)(ParamID));
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);	ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);	ParamUnit.AddSelItem(FN_ENABLE, strValue);
	ParamUnit.SetValue_SEL(Ptr->m_SaveSliceFillThreadLog);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//是否儲存影像合併執行緒訊息
	ParamUnit = CParamUni();
	str = _T("Save Frame Merge Thread Log");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_SAVE_FRAME_MERGE_THREAD_LOG;
	ParamUnit.SetParamID((UINT)(ParamID));
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);	ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);	ParamUnit.AddSelItem(FN_ENABLE, strValue);
	ParamUnit.SetValue_SEL(Ptr->m_SaveFrameMergeThreadLog);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//是否儲存區域合併執行緒訊息
	ParamUnit = CParamUni();
	str = _T("Save Field Merge Thread Log");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_SAVE_FIELD_MERGE_THREAD_LOG;
	ParamUnit.SetParamID((UINT)(ParamID));
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);	ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);	ParamUnit.AddSelItem(FN_ENABLE, strValue);
	ParamUnit.SetValue_SEL(Ptr->m_SaveFieldMergeThreadLog);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//是否儲存影像載入執行緒訊息
	ParamUnit = CParamUni();
	str = _T("Save Frame Load Thread Log");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_SAVE_FRAME_LOAD_THREAD_LOG;
	ParamUnit.SetParamID((UINT)(ParamID));
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);	ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);	ParamUnit.AddSelItem(FN_ENABLE, strValue);
	ParamUnit.SetValue_SEL(Ptr->m_SaveFrameLoadThreadLog);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);		

	//是否儲存區域計算執行緒訊息
	ParamUnit = CParamUni();
	str = _T("Save Region Calc Thread Log");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_SAVE_REGION_CALC_THREAD_LOG;
	ParamUnit.SetParamID((UINT)(ParamID));
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);	ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);	ParamUnit.AddSelItem(FN_ENABLE, strValue);
	ParamUnit.SetValue_SEL(Ptr->m_SaveRegionCalcThreadLog);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//是否儲存系列執行緒訊息
	ParamUnit = CParamUni();
	str = _T("Save Squence Thread Log");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_SAVE_SQUENCE_THREAD_LOG;
	ParamUnit.SetParamID((UINT)(ParamID));
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);	ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);	ParamUnit.AddSelItem(FN_ENABLE, strValue);
	ParamUnit.SetValue_SEL(Ptr->m_SaveSequenceThreadLog);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);
	
	//是否儲存視野影像釋放執行緒訊息
	ParamUnit = CParamUni();
	str = _T("Save Field Frame Release Thread Log");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_SAVE_FIELD_FRAME_RELEASE_THREAD_LOG;
	ParamUnit.SetParamID((UINT)(ParamID));
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);	ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);	ParamUnit.AddSelItem(FN_ENABLE, strValue);
	ParamUnit.SetValue_SEL(Ptr->m_SaveFieldFrameReleaseThreadLog);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//是否儲存線上檢測執行緒訊息
	ParamUnit = CParamUni();
	str = _T("Save Online Inspection Thread Log");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_SAVE_ONLINE_INSPECTION_THREAD_LOG;
	ParamUnit.SetParamID((UINT)(ParamID));
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);	ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);	ParamUnit.AddSelItem(FN_ENABLE, strValue);
	ParamUnit.SetValue_SEL(Ptr->m_SaveOnlineInspectionThreadLog);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//是否儲存刪除資料夾執行緒訊息
	ParamUnit = CParamUni();
	str = _T("Save Remove Folder Thread Log");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_SAVE_REMOVE_FOLDER_THREAD_LOG;
	ParamUnit.SetParamID((UINT)(ParamID));
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);	ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);	ParamUnit.AddSelItem(FN_ENABLE, strValue);
	ParamUnit.SetValue_SEL(Ptr->m_SaveRemoveFolderThreadLog);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//是否儲存軌道自動運轉執行緒訊息
	ParamUnit = CParamUni();
	str = _T("Save Conveyer Pre-Run Thread Log");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_SAVE_CONVEYER_PRE_RUN_THREAD_LOG;
	ParamUnit.SetParamID((UINT)(ParamID));
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);	ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);	ParamUnit.AddSelItem(FN_ENABLE, strValue);
	ParamUnit.SetValue_SEL(Ptr->m_SaveConveyerPreRunThreadLog);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);				

	//是否儲存ITS運作執行緒訊息
	ParamUnit = CParamUni();
	str = _T("Save ITS Proc Thread Log");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_SAVE_ITS_PROC_THREAD_LOG;
	ParamUnit.SetParamID((UINT)(ParamID));
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);	ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);	ParamUnit.AddSelItem(FN_ENABLE, strValue);
	ParamUnit.SetValue_SEL(Ptr->m_SaveITSProcThreadLog);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//是否儲存載入維修站檔案執行緒訊息	
	ParamUnit = CParamUni();
	str = _T("Save Load Repair File Thread Log");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_SAVE_LOAD_REPAIR_FILE_THREAD_LOG;
	ParamUnit.SetParamID((UINT)(ParamID));
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);	ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);	ParamUnit.AddSelItem(FN_ENABLE, strValue);
	ParamUnit.SetValue_SEL(Ptr->m_SaveLoadRepairFileThreadLog);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);				

	//是否儲存維修站訊息執行緒訊息	
	ParamUnit = CParamUni();
	str = _T("Save Repair Result Signal Thread Log");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_SAVE_REPAIR_RESULT_SIGNAL_THREAD_LOG;
	ParamUnit.SetParamID((UINT)(ParamID));
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);	ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);	ParamUnit.AddSelItem(FN_ENABLE, strValue);
	ParamUnit.SetValue_SEL(Ptr->m_SaveRepairResultSignalThreadLog);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//是否儲存簡易作業執行緒訊息
	ParamUnit = CParamUni();
	str = _T("Save Simple Job Thread Log");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_SAVE_SIMPLE_JOB_THREAD_LOG;
	ParamUnit.SetParamID((UINT)(ParamID));
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);	ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);	ParamUnit.AddSelItem(FN_ENABLE, strValue);
	ParamUnit.SetValue_SEL(Ptr->m_SaveSimpleJobThreadLog);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//是否儲存HASI監視執行緒訊息
	ParamUnit = CParamUni();
	str = _T("Save HASI Monitor Thread Log");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_SAVE_HASI_MONITOR_THREAD_LOG;
	ParamUnit.SetParamID((UINT)(ParamID));
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);	ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);	ParamUnit.AddSelItem(FN_ENABLE, strValue);
	ParamUnit.SetValue_SEL(Ptr->m_SaveHASIMonitorThreadLog);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//是否儲存檢測物件結束訊息	
	ParamUnit = CParamUni();
	str = _T("Save Test Object Finish Log");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_SAVE_TEST_OBJECT_FINISH_LOG;
	ParamUnit.SetParamID((UINT)(ParamID));
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);	ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);	ParamUnit.AddSelItem(FN_ENABLE, strValue);
	ParamUnit.SetValue_SEL(Ptr->m_SaveTestObjectFinishLog);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);
	
	//是否儲存介面重繪函式訊息
	ParamUnit = CParamUni();
	str = _T("Save UI Draw Func Log");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_SAVE_UI_DRAW_FUNC_LOG;
	ParamUnit.SetParamID((UINT)(ParamID));
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);	ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);	ParamUnit.AddSelItem(FN_ENABLE, strValue);
	ParamUnit.SetValue_SEL(Ptr->m_SaveUIDrawFuncLog);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//是否儲存使用者操作訊息
	ParamUnit = CParamUni();
	str = _T("Save User Operation Log");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_SAVE_USER_OPERATION_LOG;
	ParamUnit.SetParamID((UINT)(ParamID));
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);	ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);	ParamUnit.AddSelItem(FN_ENABLE, strValue);
	ParamUnit.SetValue_SEL(Ptr->m_SaveUserOperationLog);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//是否儲存檢測追蹤檔案	
	ParamUnit = CParamUni();
	str = _T("Save Test Track File");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_SAVE_TEST_TRACK_FILE;
	ParamUnit.SetParamID((UINT)(ParamID));
	AOIDataDefine.BuildEnableDisableParamUni(ParamUnit);	
	ParamUnit.SetValue_SEL(Ptr->m_SaveTestTrackFile);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CSystemLogPane::BuildParamListWnd()
{
	SetActParamUni(NULL);
	CThisListCtrl_36 &ListCtrl = m_ParamListCtrl;	
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
bool CSystemLogPane::BuildParamListWndHeader()
{
	CString str;
	int   nCol = 0;
	int width  = 64;
	int width2 = 64;
	RECT      Rect;	
	const int Align = LVCFMT_LEFT;//LVCFMT_CENTER;LVCFMT_RIGHT, LVCFMT_LEFT
	CThisListCtrl_36 &ListCtrl = m_ParamListCtrl;

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
void CSystemLogPane::OnItemchangedParamListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
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
void CSystemLogPane::OnDblclkParamListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
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
void CSystemLogPane::SetDescriptionText(const CParamUni *Ptr)
{
	UINT CtrlID = SYSLOG_INFO_EDIT;
	if ( NULL == Ptr )
	{
		CWnd::SetDlgItemText(CtrlID, _T(""));
		return ;
	}
	CWnd::SetDlgItemText(CtrlID, Ptr->GetDesction());
}
//-------------------------------------------------------------------------------------//
bool CSystemLogPane::ExecItemchangedParamListWnd(CThisListCtrl_36 &ListCtrl, int nItem)
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
bool CSystemLogPane::ExecDblclkParamListWnd(CThisListCtrl_36 &ListCtrl, int nItem, int nSubItem)
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
void CSystemLogPane::OnKillfocusParamEdit() 
{
	// TODO: Add your control notification handler code here
	if ( m_EditCtrl.IsWindowVisible() == FALSE ) { return; }
	ExecUpdateParamByEdit();
	m_EditCtrl.ShowWindow(SW_HIDE);	
	m_EditCtrl.SetWindowText(_T(""));
}
//-------------------------------------------------------------------------------------//
void CSystemLogPane::OnKillfocusParamCombo() 
{
	// TODO: Add your control notification handler code here
	if ( m_ComboxCtrl.IsWindowVisible() == FALSE ) { return; }
	ExecUpdateParamByCombox();
	m_ComboxCtrl.ShowWindow(SW_HIDE);	
	JetAPI::ClearCombox(m_ComboxCtrl);	
}
//-------------------------------------------------------------------------------------//
void CSystemLogPane::OnSelchangeParamCombo() 
{
	// TODO: Add your control notification handler code here
	if ( ExecUpdateParamByCombox() == false )
	{	return; }
	m_ComboxCtrl.ShowWindow(SW_HIDE);	
	JetAPI::ClearCombox(m_ComboxCtrl);	
}
//-------------------------------------------------------------------------------------//
void CSystemLogPane::OnParamBtn() 
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
	CThisListCtrl_36 *pListCtrl = (CThisListCtrl_36*)(ParamPtr->GetListCtrl());

	if ( NULL != pListCtrl )
	{
		const int ItemCount = pListCtrl->GetItemCount();
		pListCtrl->SetItemText(nItem, nSubItem, ItemText);
		pListCtrl->SetFocus();
	}
}
//-------------------------------------------------------------------------------------//
bool CSystemLogPane::ExecUpdateParamByEdit()
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
	
	CThisListCtrl_36 *pListCtrl = (CThisListCtrl_36*)(ParamPtr->GetListCtrl());
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
bool CSystemLogPane::ExecReleaseParamCtrl()
{
	m_BtnCtrl.ShowWindow(SW_HIDE);
	m_EditCtrl.ShowWindow(SW_HIDE);	
	m_ComboxCtrl.ShowWindow(SW_HIDE);

	m_EditCtrl.SetWindowText(_T(""));
	m_ParamListCtrl.SetFocus();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CSystemLogPane::ExecUpdateParamByCombox()
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
	CThisListCtrl_36 *pListCtrl = (CThisListCtrl_36*)(ParamPtr->GetListCtrl());
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