// ProjectParamPaneBarcode.cpp : implementation file
//

#include "stdafx.h"
#include "jet8000.h"
#include "ProjectParamPaneBarcode.h"
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
// CProjectParamPaneBarcode dialog
//-------------------------------------------------------------------------------------//
CProjectParamPaneBarcode::CProjectParamPaneBarcode(CWnd* pParent /*=NULL*/)
	: CDialog(CProjectParamPaneBarcode::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProjectParamPaneBarcode)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ProjectPtr = NULL;
	m_ParamActPtr = NULL;
	m_ProParameterPtr = NULL;	
	m_StopParamListBeSelected = false;
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneBarcode::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProjectParamPaneBarcode)
	DDX_Control(pDX, PROBARCODE_PARAM_EDIT, m_EditCtrl);	
	DDX_Control(pDX, PROBARCODE_PARAM_COMBO, m_ComboxCtrl);	
	DDX_Control(pDX, PROBARCODE_PARAM_LIST_WND, m_ParamListCtrl);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CProjectParamPaneBarcode, CDialog)
	//{{AFX_MSG_MAP(CProjectParamPaneBarcode)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(LVN_ITEMCHANGED, PROBARCODE_PARAM_LIST_WND, OnItemchangedParamListWnd)
	ON_NOTIFY(NM_DBLCLK, PROBARCODE_PARAM_LIST_WND, OnDblclkParamListWnd)
	ON_EN_KILLFOCUS(PROBARCODE_PARAM_EDIT, OnKillfocusParamEdit)
	ON_CBN_SELCHANGE(PROBARCODE_PARAM_COMBO, OnSelchangeParamCombo)
	ON_CBN_KILLFOCUS(PROBARCODE_PARAM_COMBO, OnKillfocusParamCombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CProjectParamPaneBarcode message handlers
//-------------------------------------------------------------------------------------//
BOOL CProjectParamPaneBarcode::OnInitDialog() 
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
void CProjectParamPaneBarcode::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneBarcode::OnSize(UINT nType, int cx, int cy) 
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

	WndPtr = CWnd::GetDlgItem(PROBARCODE_INFO_EDIT);
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
void CProjectParamPaneBarcode::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if ( TRUE == bShow )
	{	
		BuildParamListWnd();	
	}
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneBarcode::OnOK() 
{
	// TODO: Add extra validation here
	return;
	CDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneBarcode::OnCancel() 
{
	// TODO: Add extra cleanup here
	return;
	CDialog::OnCancel();
}
//-------------------------------------------------------------------------------------//
BOOL CProjectParamPaneBarcode::PreTranslateMessage(MSG* pMsg) 
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
LRESULT CProjectParamPaneBarcode::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneBarcode::OnItemchangedParamListWnd(NMHDR* pNMHDR, LRESULT* pResult)
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
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneBarcode::OnDblclkParamListWnd(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMITEMACTIVATE *pNMListView = (NMITEMACTIVATE*)pNMHDR;	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) { return; }

	ExecDblclkParamListWnd(m_ParamListCtrl, nItem, nSubItem);
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneBarcode::OnKillfocusParamEdit()
{
	if ( m_EditCtrl.IsWindowVisible() == FALSE ) { return; }
	ExecUpdateParamByEdit();
	m_EditCtrl.ShowWindow(SW_HIDE);	
	m_EditCtrl.SetWindowText(_T(""));
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneBarcode::OnSelchangeParamCombo()
{
	if ( ExecUpdateParamByCombox() == false )
	{	return; }
	m_ComboxCtrl.ShowWindow(SW_HIDE);	
	JetAPI::ClearCombox(m_ComboxCtrl);	
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneBarcode::OnKillfocusParamCombo()
{
	if ( m_ComboxCtrl.IsWindowVisible() == FALSE ) { return; }
	ExecUpdateParamByCombox();
	m_ComboxCtrl.ShowWindow(SW_HIDE);	
	JetAPI::ClearCombox(m_ComboxCtrl);	
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneBarcode::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_PROJECT_PARAM_BARCODE_PANE");
	//---------------------------------------------------------------------------------//
	WndID = IDD_PROJECT_PARAM_BARCODE_PANE;
	WndKey = _T("IDD_PROJECT_PARAM_BARCODE_PANE");
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
CString CProjectParamPaneBarcode::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_PROJECT_PARAM_BARCODE_PANE");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneBarcode::SetProjectParameterPtr(CAOIProject *ProjectPtr, TProjectParameter *Ptr)
{
	this->m_ProjectPtr = ProjectPtr;
	this->m_ProParameterPtr = Ptr;
}
//-------------------------------------------------------------------------------------//
CParamUni* CProjectParamPaneBarcode::GetActParamUni()
{
	return m_ParamActPtr;
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneBarcode::SetActParamUni(CParamUni *Ptr)
{
	m_ParamActPtr = Ptr;
}
//-------------------------------------------------------------------------------------//
bool CProjectParamPaneBarcode::BuildParamList()
{
	CThisListCtrl_23 &ListCtrl = m_ParamListCtrl;
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

	//條碼讀取模式
	ParamUnit = CParamUni();
	str = _T("Barcode Input Type");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_BARCODE_INPUT_TYPE;
	ParamUnit.SetParamID((UINT)(ParamID));
	strValue = AOIDataDefine.GetBarcodeInputTypeText(BARCODE_INPUT_DISABLED);	ParamUnit.AddSelItem(BARCODE_INPUT_DISABLED, strValue);
	strValue = AOIDataDefine.GetBarcodeInputTypeText(BARCODE_INPUT_DEVICE);		ParamUnit.AddSelItem(BARCODE_INPUT_DEVICE, strValue);
	strValue = AOIDataDefine.GetBarcodeInputTypeText(BARCODE_INPUT_HANDHELD);	ParamUnit.AddSelItem(BARCODE_INPUT_HANDHELD, strValue);	
	ParamUnit.SetValue_SEL(Ptr->m_BarcodeInputType);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//檔案讀取條碼模式啟用	
	ParamUnit = CParamUni();
	str = _T("Barcode Input File Enable");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_BARCODE_INPUT_FILE_ENABLE;
	ParamUnit.SetParamID((UINT)(ParamID));
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);	ParamUnit.AddSelItem(FN_ENABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);	ParamUnit.AddSelItem(FN_DISABLE, strValue);	
	ParamUnit.SetValue_SEL(Ptr->m_BarcodeInputFileEnabled);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//相機輸入條碼啟用	
	ParamUnit = CParamUni();
	str = _T("Barcode Input Camera Enable");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_BARCODE_INPUT_CAMERA_ENABLE;
	ParamUnit.SetParamID((UINT)(ParamID));
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);	ParamUnit.AddSelItem(FN_ENABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);	ParamUnit.AddSelItem(FN_DISABLE, strValue);	
	ParamUnit.SetValue_SEL(Ptr->m_BarcodeInputCameraEnabled);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//條碼失敗處理模式
	ParamUnit = CParamUni();
	str = _T("Barcode NG Handle Mode");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_BARCODE_NG_HANDLE_MODE;
	ParamUnit.SetParamID((UINT)(ParamID));
	strValue = AOIDataDefine.GetBarcodeNGHandleModeText(BARCODE_NG_HANDLE_PASS);	ParamUnit.AddSelItem(BARCODE_NG_HANDLE_PASS, strValue);
	strValue = AOIDataDefine.GetBarcodeNGHandleModeText(BARCODE_NG_HANDLE_ALARM);	ParamUnit.AddSelItem(BARCODE_NG_HANDLE_ALARM, strValue);
	//strValue = AOIDataDefine.GetBarcodeNGHandleModeText(BARCODE_NG_HANDLE_INPUT);	ParamUnit.AddSelItem(BARCODE_NG_HANDLE_INPUT, strValue);
	ParamUnit.SetValue_SEL(Ptr->m_BarcodeNGHandleMode);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//相機條碼讀取模式
	ParamUnit = CParamUni();
	str = _T("Barcode Camera Grab Mode");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_BARCODE_CAMERA_GRAB_MODE;
	ParamUnit.SetParamID((UINT)(ParamID));
	strValue = AOIDataDefine.GetBarcodeCameraGrabModeText(BARCODE_CAMERA_GRAB_AFTER_FD);	ParamUnit.AddSelItem(BARCODE_CAMERA_GRAB_AFTER_FD, strValue);
	strValue = AOIDataDefine.GetBarcodeCameraGrabModeText(BARCODE_CAMERA_GRAB_INSPECTING);	ParamUnit.AddSelItem(BARCODE_CAMERA_GRAB_INSPECTING, strValue);	
	ParamUnit.SetValue_SEL(Ptr->m_BarcodeCameraGrabMode);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//相機條碼存圖啟用	
	ParamUnit = CParamUni();
	str = _T("Barcode Camera Save Image");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_BARCODE_CAMERA_SAVE_IMAGE_ENABLE;
	ParamUnit.SetParamID((UINT)(ParamID));
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);	ParamUnit.AddSelItem(FN_ENABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);	ParamUnit.AddSelItem(FN_DISABLE, strValue);	
	ParamUnit.SetValue_SEL(Ptr->m_BarcodeCameraSaveImageEnabled);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//條碼機讀取模式
	ParamUnit = CParamUni();
	str = _T("Barcode Device Grab Mode");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_BARCODE_DEVICE_GRAB_MODE;
	ParamUnit.SetParamID((UINT)(ParamID));
	strValue = AOIDataDefine.GetBarcodeDeviceGrabModeText(BARCODE_DEVICE_GRAB_BEFORE_PCB_IN);	ParamUnit.AddSelItem(BARCODE_DEVICE_GRAB_BEFORE_PCB_IN, strValue);
	strValue = AOIDataDefine.GetBarcodeDeviceGrabModeText(BARCODE_DEVICE_GRAB_WHILE_PCB_IN);	ParamUnit.AddSelItem(BARCODE_DEVICE_GRAB_WHILE_PCB_IN, strValue);	
	strValue = AOIDataDefine.GetBarcodeDeviceGrabModeText(BARCODE_DEVICE_GRAB_AFTER_PCB_IN);	ParamUnit.AddSelItem(BARCODE_DEVICE_GRAB_AFTER_PCB_IN, strValue);	
	strValue = AOIDataDefine.GetBarcodeDeviceGrabModeText(BARCODE_DEVICE_GRAB_BEFORE_INSPECT);	ParamUnit.AddSelItem(BARCODE_DEVICE_GRAB_BEFORE_INSPECT, strValue);		
	ParamUnit.SetValue_SEL(Ptr->m_BarcodeDeviceGrabMode);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//手持條碼機讀取模式
	ParamUnit = CParamUni();
	str = _T("Barcode Handheld Read Mode");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_BARCODE_HANDHELD_READ_MODE;
	ParamUnit.SetParamID((UINT)(ParamID));
	strValue = AOIDataDefine.GetBarcodeHandHeldReadModeText(BARCODE_HANDHELD_READ_MANUAL);	ParamUnit.AddSelItem(BARCODE_HANDHELD_READ_MANUAL, strValue);
	strValue = AOIDataDefine.GetBarcodeHandHeldReadModeText(BARCODE_HANDHELD_READ_PROJECT);	ParamUnit.AddSelItem(BARCODE_HANDHELD_READ_PROJECT, strValue);	
	strValue = AOIDataDefine.GetBarcodeHandHeldReadModeText(BARCODE_HANDHELD_READ_PANEL);	ParamUnit.AddSelItem(BARCODE_HANDHELD_READ_PANEL, strValue);	
	strValue = AOIDataDefine.GetBarcodeHandHeldReadModeText(BARCODE_HANDHELD_READ_BOARD);	ParamUnit.AddSelItem(BARCODE_HANDHELD_READ_BOARD, strValue);
	ParamUnit.SetValue_SEL(Ptr->m_BarcodeHandHeldReadMode);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);		

	//讀取檔案模式延遲時間-ms
	ParamUnit = CParamUni();
	str = _T("Barcode Input File Delay Time");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_BARCODE_INPUT_FILE_DELAY_TIME;	
	ParamUnit.SetParamID((UINT)(ParamID));	
	ParamUnit.SetValue_INT(Ptr->m_BarcodeInputFileDelayTime, 0);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);			

	//條碼前端移除字元數
	ParamUnit = CParamUni();
	str = _T("Barcode Begin Remove Char Count");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_BARCODE_BEGIN_REMOVE_CHAR_COUNT;	
	ParamUnit.SetParamID((UINT)(ParamID));	
	ParamUnit.SetValue_INT(Ptr->m_BarcodeBeginRemoveCharCount, 0);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);		

	//條碼後端移除字元數	
	ParamUnit = CParamUni();
	str = _T("Barcode End Remove Char Count");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_BARCODE_END_REMOVE_CHAR_COUNT;	
	ParamUnit.SetParamID((UINT)(ParamID));	
	ParamUnit.SetValue_INT(Ptr->m_BarcodeEndRemoveCharCount, 0);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//條碼自動擴展模式-單板
	ParamUnit = CParamUni();
	str = _T("Barcode Auto Expand Mode (Panel)");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_BARCODE_AUTO_EXPAND_MODE_PANEL;
	ParamUnit.SetParamID((UINT)(ParamID));
	AOIDataDefine.BuildBarcodeAutoExpandModeParamUni(ParamUnit);
	ParamUnit.SetValue_SEL(Ptr->m_BarcodeAutoExpandMode_Panel);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//條碼自動擴展模式-單板
	ParamUnit = CParamUni();
	str = _T("Barcode Auto Expand Mode (Board)");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_BARCODE_AUTO_EXPAND_MODE_BOARD;
	ParamUnit.SetParamID((UINT)(ParamID));
	AOIDataDefine.BuildBarcodeAutoExpandModeParamUni(ParamUnit);	
	ParamUnit.SetValue_SEL(Ptr->m_BarcodeAutoExpandMode_Board);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//條碼驗證模式
	ParamUnit = CParamUni();
	str = _T("Barcode Verify Mode");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_BARCODE_VERIFY_MODE;
	ParamUnit.SetParamID((UINT)(ParamID));
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);	ParamUnit.AddSelItem(FN_ENABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);	ParamUnit.AddSelItem(FN_DISABLE, strValue);	
	ParamUnit.SetValue_SEL(Ptr->m_BarcodeVerifyMode);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//條碼取回模式
	ParamUnit = CParamUni();
	str = _T("Barcode Retrieve Mode");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_BARCODE_RETRIEVE_MODE;
	ParamUnit.SetParamID((UINT)(ParamID));
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);	ParamUnit.AddSelItem(FN_ENABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);	ParamUnit.AddSelItem(FN_DISABLE, strValue);	
	ParamUnit.SetValue_SEL(Ptr->m_BarcodeRetrieveMode);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	//尚未完成
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectParamPaneBarcode::BuildParamListWnd()
{
	SetActParamUni(NULL);
	CThisListCtrl_23 &ListCtrl = m_ParamListCtrl;	
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
bool CProjectParamPaneBarcode::BuildParamListWndHeader()
{
	CString str;
	int   nCol = 0;
	int width  = 64;
	int width2 = 64;
	RECT      Rect;	
	const int Align = LVCFMT_LEFT;//LVCFMT_CENTER;LVCFMT_RIGHT, LVCFMT_LEFT
	CThisListCtrl_23 &ListCtrl = m_ParamListCtrl;

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
void CProjectParamPaneBarcode::SetDescriptionText(const CParamUni *Ptr)
{
	UINT CtrlID = PROBARCODE_INFO_EDIT;
	if ( NULL == Ptr )
	{
		CWnd::SetDlgItemText(CtrlID, _T(""));
		return ;
	}
	CWnd::SetDlgItemText(CtrlID, Ptr->GetDesction());
}
//-------------------------------------------------------------------------------------//
bool CProjectParamPaneBarcode::ExecItemchangedParamListWnd(CThisListCtrl_23 &ListCtrl, int nItem)
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
bool CProjectParamPaneBarcode::ExecDblclkParamListWnd(CThisListCtrl_23 &ListCtrl, int nItem, int nSubItem)
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
bool CProjectParamPaneBarcode::HideCtrlBtn()
{	
	//m_SetFolderBtn.ShowWindow(SW_HIDE);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectParamPaneBarcode::ExecReleaseParamCtrl()
{	
	HideCtrlBtn();
	m_EditCtrl.ShowWindow(SW_HIDE);	
	m_ComboxCtrl.ShowWindow(SW_HIDE);	
	m_EditCtrl.SetWindowText(_T(""));
	m_ParamListCtrl.SetFocus();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectParamPaneBarcode::ExecUpdateParamByEdit()
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
	
	CThisListCtrl_23 *pListCtrl = (CThisListCtrl_23*)(ParamPtr->GetListCtrl());
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
bool CProjectParamPaneBarcode::ExecUpdateParamByCombox()
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
	CThisListCtrl_23 *pListCtrl = (CThisListCtrl_23*)(ParamPtr->GetListCtrl());
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