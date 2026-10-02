// SystemFolderPane.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "SystemFolderPane.h"
//-------------------------------------------------------------------------------------//
#include "InputListWnd.h"
#include "FilenameSyntaxWnd.h"
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
// CSystemFolderPane dialog
//-------------------------------------------------------------------------------------//
CSystemFolderPane::CSystemFolderPane(CWnd* pParent /*=NULL*/)
	: CDialog(CSystemFolderPane::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSystemFolderPane)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ParamActPtr = NULL;
	m_SysParameterPtr = NULL;	
	m_StopParamListBeSelected = false;
}
//-------------------------------------------------------------------------------------//
void CSystemFolderPane::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSystemFolderPane)
	DDX_Control(pDX, SYSFOLDER_PARAM_EDIT, m_EditCtrl);
	DDX_Control(pDX, SYSFOLDER_FOLDER_BTN, m_FoderBtn);
	DDX_Control(pDX, SYSFOLDER_LIBRARY_BTN, m_LibraryBtn);
	DDX_Control(pDX, SYSFOLDER_FILENAME_BTN, m_FilenameBtn);
	DDX_Control(pDX, SYSFOLDER_PARAM_COMBO, m_ComboxCtrl);	
	DDX_Control(pDX, SYSFOLDER_PARAM_LIST_WND, m_FolderListCtrl);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CSystemFolderPane, CDialog)
	//{{AFX_MSG_MAP(CSystemFolderPane)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_EN_KILLFOCUS(SYSFOLDER_PARAM_EDIT, OnKillfocusParamEdit)
	ON_NOTIFY(LVN_ITEMCHANGED, SYSFOLDER_PARAM_LIST_WND, OnItemchangedParamListWnd)
	ON_NOTIFY(NM_DBLCLK, SYSFOLDER_PARAM_LIST_WND, OnDblclkParamListWnd)
	ON_CBN_SELCHANGE(SYSFOLDER_PARAM_COMBO, OnSelchangeParamCombo)
	ON_CBN_KILLFOCUS(SYSFOLDER_PARAM_COMBO, OnKillfocusParamCombo)
	ON_BN_CLICKED(SYSFOLDER_FOLDER_BTN, OnFolderBtn)
	ON_BN_CLICKED(SYSFOLDER_LIBRARY_BTN, OnLibraryBtn)
	ON_BN_CLICKED(SYSFOLDER_FILENAME_BTN, OnFilenameBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CSystemFolderPane message handlers
//-------------------------------------------------------------------------------------//
BOOL CSystemFolderPane::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SwitchMultiLanguage();
	JetAPI::InitialListCtrl(m_FolderListCtrl);	
	//CWnd::ShowWindow(SW_SHOWNORMAL);
	BuildParamListWndHeader();
	if ( CWnd::IsWindowVisible() )
	{	BuildParamListWnd(); }
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CSystemFolderPane::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CSystemFolderPane::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if ( m_FolderListCtrl.GetSafeHwnd() == NULL ) { return; }
	CWnd *WndPtr = NULL;
	SIZE  WndSize={0};
	RECT  InfoRect={0};
	BOOL  bVisible = CWnd::IsWindowVisible();
	const int MarginX = 4;
	const int MarginY = 4;

	WndPtr = CWnd::GetDlgItem(SYSFOLDER_INFO_EDIT);
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

	if ( m_FolderListCtrl.GetSafeHwnd() != NULL )
	{
		RECT WndRect={0};		
		WndRect.left = MarginX;
		WndRect.right = cx-MarginX;
		WndRect.top = MarginY;
		WndRect.bottom = InfoRect.top-MarginY;
		m_FolderListCtrl.MoveWindow(&WndRect, FALSE);
		if ( TRUE == bVisible )
		{	m_FolderListCtrl.Invalidate();	}
	}
}
//-------------------------------------------------------------------------------------//
void CSystemFolderPane::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if ( TRUE == bShow )
	{	
		BuildParamListWnd();	
	}
}
//-------------------------------------------------------------------------------------//
BOOL CSystemFolderPane::PreTranslateMessage(MSG* pMsg) 
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
LRESULT CSystemFolderPane::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void CSystemFolderPane::OnOK() 
{
	// TODO: Add extra validation here
	return ;
	CDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CSystemFolderPane::OnCancel() 
{
	// TODO: Add extra cleanup here
	return ;
	CDialog::OnCancel();
}
//-------------------------------------------------------------------------------------//
void CSystemFolderPane::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_SYSTEM_FOLDER_PANE");
	//---------------------------------------------------------------------------------//
	WndID = IDD_SYSTEM_FOLDER_PANE;
	WndKey = _T("IDD_SYSTEM_FOLDER_PANE");
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
CString CSystemFolderPane::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_SYSTEM_FOLDER_PANE");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
void CSystemFolderPane::SetSystemParameterPtr(TSystemParameter *Ptr)
{
	m_SysParameterPtr = Ptr;
}
//-------------------------------------------------------------------------------------//
void CSystemFolderPane::OnItemchangedParamListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
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
	ExecItemchangedParamListWnd(m_FolderListCtrl, nItem);	
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CSystemFolderPane::OnDblclkParamListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NMITEMACTIVATE *pNMListView = (NMITEMACTIVATE*)pNMHDR;	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) { return; }

	ExecDblclkParamListWnd(m_FolderListCtrl, nItem, nSubItem);
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CSystemFolderPane::OnKillfocusParamEdit() 
{
	// TODO: Add your control notification handler code here
	if ( m_EditCtrl.IsWindowVisible() == FALSE ) { return; }
	ExecUpdateParamByEdit();
	m_EditCtrl.ShowWindow(SW_HIDE);	
	m_EditCtrl.SetWindowText(_T(""));
}
//-------------------------------------------------------------------------------------//
void CSystemFolderPane::OnSelchangeParamCombo() 
{
	// TODO: Add your control notification handler code here
	if ( ExecUpdateParamByCombox() == false )
	{	return; }
	m_ComboxCtrl.ShowWindow(SW_HIDE);	
	JetAPI::ClearCombox(m_ComboxCtrl);
}
//-------------------------------------------------------------------------------------//
void CSystemFolderPane::OnKillfocusParamCombo() 
{
	// TODO: Add your control notification handler code here
	if ( m_ComboxCtrl.IsWindowVisible() == FALSE ) { return; }
	ExecUpdateParamByCombox();
	m_ComboxCtrl.ShowWindow(SW_HIDE);	
	JetAPI::ClearCombox(m_ComboxCtrl);	
}
//-------------------------------------------------------------------------------------//
void CSystemFolderPane::OnFolderBtn() 
{
	// TODO: Add your control notification handler code here
	ExecReleaseCtrlWnd();
	CParamUni   *ParamPtr = GetActParamUni();
	if ( NULL == ParamPtr ) { return ; }
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
	CThisListCtrl_33 *pListCtrl = (CThisListCtrl_33*)(ParamPtr->GetListCtrl());

	if ( NULL != pListCtrl )
	{
		const int ItemCount = pListCtrl->GetItemCount();
		pListCtrl->SetItemText(nItem, nSubItem, ItemText);
		pListCtrl->SetFocus();
	}
	return ;
}
//-------------------------------------------------------------------------------------//
CParamUni* CSystemFolderPane::GetActParamUni()
{
	return m_ParamActPtr;
}
//-------------------------------------------------------------------------------------//
void CSystemFolderPane::SetActParamUni(CParamUni *Ptr)
{
	m_ParamActPtr = Ptr;
}
//-------------------------------------------------------------------------------------//
bool CSystemFolderPane::BuildParamList()
{
	CThisListCtrl_33 &ListCtrl = m_FolderListCtrl;
	m_StopParamListBeSelected = true;
	JetAPI::ClearListCtrl(ListCtrl, FALSE);
	m_StopParamListBeSelected = false;
	if ( NULL == m_SysParameterPtr ) { return true; }

	CString   str;
	CString   strValue;	
	CString   strCaption;	
	int       nItem=0;
	CParamUni    ParamUnit;	
	SYSTEM_PARAM_ID SysParam;
	TSystemParameter *Ptr = m_SysParameterPtr;
	const int nSubItem = 1;	
	CParamList &ParamList = m_ParamList;

	ParamList.clear();	

	//路徑
	ParamUnit = CParamUni();
	str = _T("Log Folder");
	strCaption = LoadMultiLanguageString(str, str);
	SysParam = SYSTEM_AOI_FOLDER_LOG;
	ParamUnit.SetParamID((UINT)(SysParam));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_STR(Ptr->m_AOILogDirectory);
	ParamUnit.SetReStart(true);
	ParamUnit.SetDesction(strCaption);
	ParamUnit.SetBtnWndPtr(&m_FoderBtn);
	ParamList.push_back(ParamUnit);

	//專案資料夾	
	ParamUnit = CParamUni();
	str = _T("Project Folder");
	strCaption = LoadMultiLanguageString(str, str);
	SysParam = SYSTEM_AOI_FOLDER_PROJECT;
	ParamUnit.SetParamID((UINT)(SysParam));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_STR(Ptr->m_AOIProjectFolder);
	ParamUnit.SetDesction(strCaption);
	ParamUnit.SetBtnWndPtr(&m_FoderBtn);
	ParamList.push_back(ParamUnit);
	
	//結果資料夾	
	ParamUnit = CParamUni();
	str = _T("Result Folder");
	strCaption = LoadMultiLanguageString(str, str);
	SysParam = SYSTEM_AOI_FOLDER_RESULT;
	ParamUnit.SetParamID((UINT)(SysParam));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_STR(Ptr->m_AOIResultFolder);
	ParamUnit.SetDesction(strCaption);
	ParamUnit.SetBtnWndPtr(&m_FoderBtn);
	ParamList.push_back(ParamUnit);

	//資料庫資料夾	
	ParamUnit = CParamUni();
	str = _T("Server Folder");
	strCaption = LoadMultiLanguageString(str, str);
	SysParam = SYSTEM_AOI_FOLDER_SERVER;
	ParamUnit.SetParamID((UINT)(SysParam));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_STR(Ptr->m_AOIServerFolder);
	ParamUnit.SetDesction(strCaption);
	ParamUnit.SetBtnWndPtr(&m_FoderBtn);
	ParamList.push_back(ParamUnit);	

	//統計數據資料夾
	ParamUnit = CParamUni();
	str = _T("Static Data Folder");
	strCaption = LoadMultiLanguageString(str, str);
	SysParam = SYSTEM_AOI_FOLDER_STATIC_DATA;
	ParamUnit.SetParamID((UINT)(SysParam));
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_STR(Ptr->m_AOIStaticDataFolder);
	ParamUnit.SetDesction(strCaption);
	ParamUnit.SetBtnWndPtr(&m_FoderBtn);
	ParamList.push_back(ParamUnit);

	//文字報告資料夾
	ParamUnit = CParamUni();
	str = _T("Text Report Folder");
	strCaption = LoadMultiLanguageString(str, str);
	SysParam = SYSTEM_AOI_FOLDER_TEXT_REPORT;
	ParamUnit.SetParamID((UINT)(SysParam));
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_STR(Ptr->m_AOITextReportFolder);
	ParamUnit.SetDesction(strCaption);
	ParamUnit.SetBtnWndPtr(&m_FoderBtn);	
	ParamList.push_back(ParamUnit);

	//線上調機資料夾
	ParamUnit = CParamUni();
	str = _T("Online Tuning Folder");
	strCaption = LoadMultiLanguageString(str, str);
	SysParam = SYSTEM_AOI_FOLDER_ONLINE_TUNING;
	ParamUnit.SetParamID((UINT)(SysParam));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_STR(Ptr->m_OnlineTuningFolder);
	ParamUnit.SetDesction(strCaption);
	ParamUnit.SetBtnWndPtr(&m_FoderBtn);
	ParamList.push_back(ParamUnit);

	//線上條碼資料夾	
	ParamUnit = CParamUni();
	str = _T("Online Barcode Folder");
	strCaption = LoadMultiLanguageString(str, str);
	SysParam = SYSTEM_AOI_FOLDER_ONLINE_BARCODE;
	ParamUnit.SetParamID((UINT)(SysParam));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_STR(Ptr->m_OnlineBarcodeFolder);
	ParamUnit.SetDesction(strCaption);
	ParamUnit.SetBtnWndPtr(&m_FoderBtn);
	ParamList.push_back(ParamUnit);

	//線上離線編程資料夾	
	ParamUnit = CParamUni();
	str = _T("Online Offline Folder");
	strCaption = LoadMultiLanguageString(str, str);
	SysParam = SYSTEM_AOI_FOLDER_ONLINE_OFFLINE;
	ParamUnit.SetParamID((UINT)(SysParam));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_STR(Ptr->m_OnlineOfflineFolder);
	ParamUnit.SetDesction(strCaption);
	ParamUnit.SetBtnWndPtr(&m_FoderBtn);
	ParamList.push_back(ParamUnit);

	//專案檢測底圖資料夾
	ParamUnit = CParamUni();
	str = _T("Project Test Map Folder");
	strCaption = LoadMultiLanguageString(str, str);
	SysParam = SYSTEM_AOI_FOLDER_PROJECT_TEST_MAP;
	ParamUnit.SetParamID((UINT)(SysParam));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_STR(Ptr->m_ProjectTestMapFolder);
	ParamUnit.SetDesction(strCaption);
	ParamUnit.SetBtnWndPtr(&m_FoderBtn);
	ParamUnit.SetBtnWndPtr2(&m_FilenameBtn);	
	ParamList.push_back(ParamUnit);	

	//專案檢測底圖資料夾-暫存
	ParamUnit = CParamUni();
	str = _T("Project Test Map Temp Folder");
	strCaption = LoadMultiLanguageString(str, str);
	SysParam = SYSTEM_AOI_FOLDER_PROJECT_TEST_MAP_TEMP;
	ParamUnit.SetParamID((UINT)(SysParam));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_STR(Ptr->m_ProjectTestMapFolderTemp);
	ParamUnit.SetDesction(strCaption);
	ParamUnit.SetBtnWndPtr(&m_FoderBtn);
	//ParamUnit.SetBtnWndPtr2(&m_FilenameBtn);	
	ParamList.push_back(ParamUnit);	

	//專案檢測底圖資料夾-備份
	ParamUnit = CParamUni();
	str = _T("Project Test Map Backup Folder");
	strCaption = LoadMultiLanguageString(str, str);
	SysParam = SYSTEM_AOI_FOLDER_PROJECT_TEST_MAP_BACKUP;
	ParamUnit.SetParamID((UINT)(SysParam));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_STR(Ptr->m_ProjectTestMapFolderBackup);
	ParamUnit.SetDesction(strCaption);
	ParamUnit.SetBtnWndPtr(&m_FoderBtn);
	//ParamUnit.SetBtnWndPtr2(&m_FilenameBtn);	
	ParamList.push_back(ParamUnit);	

	//專案原圖資料夾	
	ParamUnit = CParamUni();
	str = _T("Project Raw Folder");
	strCaption = LoadMultiLanguageString(str, str);
	SysParam = SYSTEM_AOI_FOLDER_PROJECT_RAW;
	ParamUnit.SetParamID((UINT)(SysParam));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_STR(Ptr->m_ProjectRawFolder);
	ParamUnit.SetDesction(strCaption);
	ParamUnit.SetBtnWndPtr(&m_FoderBtn);	
	ParamList.push_back(ParamUnit);	

	//專案除錯資料夾
	ParamUnit = CParamUni();
	str = _T("Project Debug Folder");
	strCaption = LoadMultiLanguageString(str, str);
	SysParam = SYSTEM_AOI_FOLDER_PROJECT_DEBUG;
	ParamUnit.SetParamID((UINT)(SysParam));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_STR(Ptr->m_ProjectDebugFolder);
	ParamUnit.SetDesction(strCaption);
	ParamUnit.SetBtnWndPtr(&m_FoderBtn);
	ParamList.push_back(ParamUnit);	

	//監控狀態資料夾	
	ParamUnit = CParamUni();
	str = _T("Monitor Status Folder");
	strCaption = LoadMultiLanguageString(str, str);
	SysParam = SYSTEM_AOI_FOLDER_MONITOR_STATUS;
	ParamUnit.SetParamID((UINT)(SysParam));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_STR(Ptr->m_MonitorStatusFolder);
	ParamUnit.SetDesction(strCaption);
	ParamUnit.SetBtnWndPtr(&m_FoderBtn);
	ParamList.push_back(ParamUnit);	

	//客戶訊息資料夾
	ParamUnit = CParamUni();
	str = _T("Customer Log Folder");
	strCaption = LoadMultiLanguageString(str, str);
	SysParam = SYSTEM_AOI_FOLDER_CUSTOMER_LOG;
	ParamUnit.SetParamID((UINT)(SysParam));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_STR(Ptr->m_CustomerLogFolder);
	ParamUnit.SetReStart(true);
	ParamUnit.SetDesction(strCaption);
	ParamUnit.SetBtnWndPtr(&m_FoderBtn);		
	ParamList.push_back(ParamUnit);		

	//客戶報告資料夾
	ParamUnit = CParamUni();
	str = _T("Customer Report Folder");
	strCaption = LoadMultiLanguageString(str, str);
	SysParam = SYSTEM_AOI_FOLDER_CUSTOMER_REPORT;
	ParamUnit.SetParamID((UINT)(SysParam));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_STR(Ptr->m_CustomerReportFolder);
	ParamUnit.SetDesction(strCaption);
	ParamUnit.SetBtnWndPtr(&m_FoderBtn);
	//ParamUnit.SetBtnWndPtr2(&m_FilenameBtn);	
	ParamList.push_back(ParamUnit);		

	//條碼檔案資料夾-A軌
	ParamUnit = CParamUni();
	str = _T("Barcode File Folder LA");
	strCaption = LoadMultiLanguageString(str, str);
	SysParam = SYSTEM_AOI_FOLDER_BARCODE_FILE_LA;
	ParamUnit.SetParamID((UINT)(SysParam));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_STR(Ptr->m_BarcodeFileFolder_LA);
	ParamUnit.SetDesction(strCaption);
	ParamUnit.SetBtnWndPtr(&m_FoderBtn);
	ParamList.push_back(ParamUnit);
		
	//條碼檔案資料夾-B軌
	ParamUnit = CParamUni();
	str = _T("Barcode File Folder LB");
	strCaption = LoadMultiLanguageString(str, str);
	SysParam = SYSTEM_AOI_FOLDER_BARCODE_FILE_LB;
	ParamUnit.SetParamID((UINT)(SysParam));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_STR(Ptr->m_BarcodeFileFolder_LB);
	ParamUnit.SetDesction(strCaption);
	ParamUnit.SetBtnWndPtr(&m_FoderBtn);
	ParamList.push_back(ParamUnit);

	//資料庫本機
	ParamUnit = CParamUni();
	str = _T("Host Library");
	strCaption = LoadMultiLanguageString(str, str);
	SysParam = SYSTEM_AOI_LIBRARY_HOST;
	ParamUnit.SetParamID((UINT)(SysParam));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_STR(Ptr->m_LibraryHost);
	ParamUnit.SetDesction(strCaption);
	ParamUnit.SetBtnWndPtr(&m_LibraryBtn);
	ParamList.push_back(ParamUnit);	

	//資料庫遠端
	/*
	ParamUnit = CParamUni();
	str = _T("Remote Library");
	strCaption = LoadMultiLanguageString(str, str);
	SysParam = SYSTEM_AOI_LIBRARY_REMOTE;
	ParamUnit.SetParamID((UINT)(SysParam));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_STR(Ptr->m_LibraryRemote);
	ParamUnit.SetDesction(strCaption);
	ParamUnit.SetBtnWndPtr(&m_LibraryBtn);
	ParamList.push_back(ParamUnit);
	*/	
	
	//AI檔案輸出資料夾
	ParamUnit = CParamUni();
	str = _T("AI File Folder");
	strCaption = LoadMultiLanguageString(str, str);
	SysParam = SYSTEM_AOI_FOLDER_AI_FILE_EXPORT;
	ParamUnit.SetParamID((UINT)(SysParam));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_STR(Ptr->m_AIFileExportFolder);
	ParamUnit.SetDesction(strCaption);
	ParamUnit.SetBtnWndPtr(&m_FoderBtn);
	ParamList.push_back(ParamUnit);

	//AI影像輸出資料夾	
	ParamUnit = CParamUni();
	str = _T("AI Image Folder");
	strCaption = LoadMultiLanguageString(str, str);
	SysParam = SYSTEM_AOI_FOLDER_AI_IMAGE_EXPORT;
	ParamUnit.SetParamID((UINT)(SysParam));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_STR(Ptr->m_AIImageExportFolder);
	ParamUnit.SetDesction(strCaption);
	ParamUnit.SetBtnWndPtr(&m_FoderBtn);
	ParamList.push_back(ParamUnit);

	//檢測追蹤資料夾	
	ParamUnit = CParamUni();	
	str = _T("Test Track Folder");
	strCaption = LoadMultiLanguageString(str, str);
	SysParam = SYSTEM_AOI_FOLDER_TEST_TRACK;
	ParamUnit.SetParamID((UINT)(SysParam));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_STR(Ptr->m_AOITestTrackFolder);
	ParamUnit.SetDesction(strCaption);
	ParamUnit.SetBtnWndPtr(&m_FoderBtn);	
	ParamList.push_back(ParamUnit);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CSystemFolderPane::BuildParamListWnd()
{
	SetActParamUni(NULL);
	CThisListCtrl_33 &ListCtrl = m_FolderListCtrl;	
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
	const int     nSubItem3 = 3;
	COLORREF      ReStartColor=0x0000BF;

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

		if ( ParamPtr->GetBtnWndPtr2() != NULL )
		{	ListCtrl.SetItemText(nItem, nSubItem3, _T("---"));	}
		if ( ParamPtr->GetReStart() == true )
		{	ListCtrl.SetItemTextColor(nItem, nSubItem1, ReStartColor);	}

		nItem ++;
	}	
	m_StopParamListBeSelected = false;
	ListCtrl.SetRedraw(TRUE);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CSystemFolderPane::BuildParamListWndHeader()
{
	CString str;
	int   nCol = 0;
	int width  = 64;
	int width2 = 64;
	RECT      Rect;	
	const int Align = LVCFMT_LEFT;//LVCFMT_CENTER;LVCFMT_RIGHT	
	CThisListCtrl_33 &ListCtrl = m_FolderListCtrl;

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

	width2 = width*4;
	str = _T("Information");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(nCol, str, Align, width2);
	nCol ++;

	width2 = width;
	str = _T("Filename");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(nCol, str, LVCFMT_CENTER, width2);
	nCol ++;
	return true;
}
//-------------------------------------------------------------------------------------//
void CSystemFolderPane::SetDescriptionText(const CParamUni *Ptr)
{
	UINT CtrlID = SYSFOLDER_INFO_EDIT;
	if ( NULL == Ptr )
	{
		CWnd::SetDlgItemText(CtrlID, _T(""));
		return ;
	}
	CWnd::SetDlgItemText(CtrlID, Ptr->GetDesction());
}
//-------------------------------------------------------------------------------------//
bool CSystemFolderPane::ExecItemchangedParamListWnd(CThisListCtrl_33 &ListCtrl, int nItem)
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
	{	ExecReleaseCtrlWnd();	}	

	//Filename Btn
	BtnWndPtr = ParamPtr->GetBtnWndPtr2();
	if ( NULL!=BtnWndPtr && BtnWndPtr->GetSafeHwnd()!=NULL) 	
	{
		CRect ItemRect;
		if ( ListCtrl.GetSubItemRect(nItem, nSubItem+1, LVIR_BOUNDS, ItemRect) == TRUE )
		{	
			SIZE BtnSize={0};
			RECT BtnRect={0};
			RECT CtrlRect = ItemRect;
			ListCtrl.ClientToScreen(&CtrlRect);
			this->ScreenToClient(&CtrlRect);
			BtnWndPtr->GetWindowRect(&BtnRect);
			JetAPI::GetRectSize(BtnRect, BtnSize);
			BtnRect = CtrlRect;			
			//BtnRect.left = BtnRect.right-BtnSize.cx;
			BtnWndPtr->MoveWindow(&BtnRect, FALSE);			
			BtnWndPtr->ShowWindow(SW_SHOW);			
			BtnWndPtr->BringWindowToTop();
			ListCtrl.UpdateWindow();
			BtnWndPtr->Invalidate();
			SetActParamUni(ParamPtr);			
		}		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CSystemFolderPane::ExecDblclkParamListWnd(CThisListCtrl_33 &ListCtrl, int nItem, int nSubItem)
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
	const size_t     SelItemCount = ParamPtr->GetSelItemCount();
	if ( ListCtrl.GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, ItemRect) == FALSE )
	{	return false; }
	ItemText = ListCtrl.GetItemText(nItem, nSubItem);

	CtrlRect = ItemRect;
	ListCtrl.ClientToScreen(&CtrlRect);
	this->ScreenToClient(&CtrlRect);
	::OffsetRect(&CtrlRect, 0, -2);
	ExecReleaseCtrlWnd();
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
bool CSystemFolderPane::ExecUpdateParamByEdit()
{	
	CParamUni   *ParamPtr = GetActParamUni();
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
	
	CThisListCtrl_33 *pListCtrl = (CThisListCtrl_33*)(ParamPtr->GetListCtrl());
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
bool CSystemFolderPane::ExecUpdateParamByCombox()
{
	CParamUni   *ParamPtr = GetActParamUni();
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

	ParamPtr->SetNewValue_SEL(Param);	
	ItemText = ParamPtr->GetParamText();
	CThisListCtrl_33 *pListCtrl = (CThisListCtrl_33*)(ParamPtr->GetListCtrl());
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
bool CSystemFolderPane::ExecReleaseCtrlWnd()
{
	m_FoderBtn.ShowWindow(SW_HIDE);
	m_LibraryBtn.ShowWindow(SW_HIDE);	
	m_FilenameBtn.ShowWindow(SW_HIDE);	
	m_ComboxCtrl.ShowWindow(SW_HIDE);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CSystemFolderPane::ExecReleaseParamCtrl()
{
	ExecReleaseCtrlWnd();
	m_EditCtrl.ShowWindow(SW_HIDE);	
	m_EditCtrl.SetWindowText(_T(""));
	m_FolderListCtrl.SetFocus();
	return true;
}
//-------------------------------------------------------------------------------------//
void CSystemFolderPane::OnLibraryBtn() 
{
	// TODO: Add your control notification handler code here
	ExecReleaseCtrlWnd();
	CParamUni   *ParamPtr = GetActParamUni();
	if ( NULL == ParamPtr ) { return ; }
	CWnd      *BtnWndPtr = ParamPtr->GetBtnWndPtr();
	if ( NULL == BtnWndPtr ) { return; }

	CString ItemText = ParamPtr->GetParamText();
	SYSTEM_PARAM_ID SysParam = (SYSTEM_PARAM_ID)(ParamPtr->GetParamID());		
	
	CString Filename = ItemText;
	TCHAR   filename[MAX_JET_PATH]=_T("");
	TCHAR szFilters[]=_T("Library Files (*.LIB)|*.LIB|All Files (*.*)|*.*||");
	CFileDialog dialog(FALSE, _T("LIB"), _T("*.LIB"), OFN_FILEMUSTEXIST, szFilters, this);
	::_tcscpy(filename, Filename);	
	dialog.m_ofn.lpstrFile  = filename;	
	if ( dialog.DoModal() == IDCANCEL ) 
	{	return ; }

	ItemText = dialog.GetPathName();	
	//if ( JetAPI::OpenFolderDialog(this, ItemText) == false ) { return; }
	if ( CAOIDataCollect::SetSystemParameterStringByID(SysParam, *m_SysParameterPtr, ItemText) == false )
	{	return ; }
	ParamPtr->SetNewValue(ItemText);
	SetActParamUni(NULL);

	const int nItem = ParamPtr->GetItemIndex();
	const int nSubItem = ParamPtr->GetSubItemIndex();	
	CThisListCtrl_33 *pListCtrl = (CThisListCtrl_33*)(ParamPtr->GetListCtrl());

	if ( NULL != pListCtrl )
	{
		const int ItemCount = pListCtrl->GetItemCount();
		pListCtrl->SetItemText(nItem, nSubItem, ItemText);
		pListCtrl->SetFocus();
	}
	return ;
}
//-------------------------------------------------------------------------------------//
void CSystemFolderPane::OnFilenameBtn() 
{
	// TODO: Add your control notification handler code here
	ExecReleaseCtrlWnd();
	CParamUni   *ParamPtr = GetActParamUni();
	if ( NULL == ParamPtr ) { return ; }
	CWnd      *BtnWndPtr = ParamPtr->GetBtnWndPtr2();
	if ( NULL == BtnWndPtr ) { return; }

	bool              bValidParam=false;
	CFilenameSyntax    FilenameSyntax;	
	CString ItemText = ParamPtr->GetParamText();
	SYSTEM_PARAM_ID SysParam = (SYSTEM_PARAM_ID)(ParamPtr->GetParamID());

	int       SaveMapScope=0;
	const int SaveMapScope_Board=1;
	const int SaveMapScope_Panel=2;
	const int SaveMapScope_Project=3;
	if ( SYSTEM_AOI_FOLDER_PROJECT_TEST_MAP==SysParam || SYSTEM_AOI_FOLDER_PROJECT_TEST_MAP_TEMP==SysParam || SYSTEM_AOI_FOLDER_PROJECT_TEST_MAP_BACKUP==SysParam )
	{	
		TListNode       Node;	
		CInputListWnd   EnumWnd;		
		DWORD_PTR       OldIndex=0;	
		std::vector<TListNode> NodelList;
		CString         strCaption, strLabel;
		Node.Data=SaveMapScope_Project;	Node.Text=AOIDataDefine.GetProjectText();	NodelList.push_back(Node);
		Node.Data=SaveMapScope_Panel;	Node.Text=AOIDataDefine.GetPanelText();	NodelList.push_back(Node);
		Node.Data=SaveMapScope_Board;	Node.Text=AOIDataDefine.GetBoardText();	NodelList.push_back(Node);		
	
		strCaption = _T("Save Map Scope");
		strLabel = _T("Map");
		EnumWnd.SetParam1(strCaption, strLabel, OldIndex, NodelList);
		if ( EnumWnd.GetSelIndex1() < 0 ) 
		{	EnumWnd.SetSelIndex1(0); }
		if ( EnumWnd.DoModal() == IDCANCEL )
		{	return ; }

		bValidParam = true;
		SaveMapScope = (int)(EnumWnd.GetSelData());	
		if ( SaveMapScope_Board == SaveMapScope )
		{	FilenameSyntax=AOIDataCollect.GetFilenameSyntax_BoardMap();	}
		if ( SaveMapScope_Panel == SaveMapScope )
		{	FilenameSyntax=AOIDataCollect.GetFilenameSyntax_PanelMap();	}
		if ( SaveMapScope_Project == SaveMapScope )
		{	FilenameSyntax=AOIDataCollect.GetFilenameSyntax_ProjectMap();	}
	}
	if ( SYSTEM_AOI_FOLDER_CUSTOMER_REPORT == SysParam )
	{
		bValidParam = true;
		FilenameSyntax=AOIDataCollect.GetFilenameSyntax_CustomerReport();
	}		
	if ( false == bValidParam )
	{	return; }
	
	CFilenameSyntaxWnd FilenameSyntaxWnd;
	CAOIProject *ProjectPtr=AOIDataCollect.GetActiveProject();
	CAOIPanel   *PanelPtr=AOIDataCollect.GetActivePanel();
	CAOIBoard   *BoardPtr=AOIDataCollect.GetActiveBoard();
	FilenameSyntax.SetProjectPtr(ProjectPtr);
	FilenameSyntax.SetPanelPtr(PanelPtr);
	FilenameSyntax.SetBoardPtr(BoardPtr);
	FilenameSyntaxWnd.SetFilenameSyntax(FilenameSyntax);
	if ( FilenameSyntaxWnd.DoModal() != IDOK )
	{	return; }

	SetActParamUni(NULL);
	FilenameSyntax = FilenameSyntaxWnd.GetFilenameSyntax();
	if ( SYSTEM_AOI_FOLDER_PROJECT_TEST_MAP==SysParam || SYSTEM_AOI_FOLDER_PROJECT_TEST_MAP_TEMP==SysParam || SYSTEM_AOI_FOLDER_PROJECT_TEST_MAP_BACKUP==SysParam )
	{
		if ( SaveMapScope_Board == SaveMapScope )
		{	AOIDataCollect.SetFilenameSyntax_BoardMap(FilenameSyntax);	}
		if ( SaveMapScope_Panel == SaveMapScope )
		{	AOIDataCollect.SetFilenameSyntax_PanelMap(FilenameSyntax);	}
		if ( SaveMapScope_Project == SaveMapScope )
		{	AOIDataCollect.SetFilenameSyntax_ProjectMap(FilenameSyntax);	}
	}
	if ( SYSTEM_AOI_FOLDER_CUSTOMER_REPORT == SysParam )
	{	AOIDataCollect.SetFilenameSyntax_CustomerReport(FilenameSyntax);	}	
	return ;
}
//-------------------------------------------------------------------------------------//