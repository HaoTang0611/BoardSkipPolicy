// SystemAuthorizationPane.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "SystemAuthorizationPane.h"
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
// CSystemAuthorizationPane dialog
//-------------------------------------------------------------------------------------//
CSystemAuthorizationPane::CSystemAuthorizationPane(CWnd* pParent /*=NULL*/)
	: CDialog(CSystemAuthorizationPane::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSystemAuthorizationPane)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ParamActPtr = NULL;
	m_SysParameterPtr = NULL;	
	m_StopParamListBeSelected = false;
}
//-------------------------------------------------------------------------------------//
void CSystemAuthorizationPane::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSystemAuthorizationPane)
	DDX_Control(pDX, SYSAUTH_PARAM_EDIT, m_EditCtrl);
	DDX_Control(pDX, SYSAUTH_PARAM_BTN, m_BtnCtrl);	
	DDX_Control(pDX, SYSAUTH_PARAM_COMBO, m_ComboxCtrl);	
	DDX_Control(pDX, SYSAUTH_PARAM_LIST_WND, m_ParamListCtrl);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CSystemAuthorizationPane, CDialog)
	//{{AFX_MSG_MAP(CSystemAuthorizationPane)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(LVN_ITEMCHANGED, SYSAUTH_PARAM_LIST_WND, OnItemchangedParamListWnd)
	ON_NOTIFY(NM_DBLCLK, SYSAUTH_PARAM_LIST_WND, OnDblclkParamListWnd)
	ON_EN_KILLFOCUS(SYSAUTH_PARAM_EDIT, OnKillfocusParamEdit)
	ON_CBN_KILLFOCUS(SYSAUTH_PARAM_COMBO, OnKillfocusParamCombo)
	ON_CBN_SELCHANGE(SYSAUTH_PARAM_COMBO, OnSelchangeParamCombo)
	ON_BN_CLICKED(SYSAUTH_PARAM_BTN, OnParamBtn)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CSystemAuthorizationPane message handlers
//-------------------------------------------------------------------------------------//
BOOL CSystemAuthorizationPane::OnInitDialog() 
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
void CSystemAuthorizationPane::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CSystemAuthorizationPane::OnSize(UINT nType, int cx, int cy) 
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

	WndPtr = CWnd::GetDlgItem(SYSAUTH_INFO_EDIT);
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
void CSystemAuthorizationPane::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if ( TRUE == bShow )
	{	
		BuildParamListWnd();	
	}
}
//-------------------------------------------------------------------------------------//
BOOL CSystemAuthorizationPane::PreTranslateMessage(MSG* pMsg) 
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
LRESULT CSystemAuthorizationPane::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void CSystemAuthorizationPane::OnOK() 
{
	// TODO: Add extra validation here
	return;
	CDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CSystemAuthorizationPane::OnCancel() 
{
	// TODO: Add extra cleanup here
	return;
	CDialog::OnCancel();
}
//-------------------------------------------------------------------------------------//
void CSystemAuthorizationPane::SwitchMultiLanguage()
{
	//---------------------------------------------------------------------------------//	
	SetMultiLanauage(LoadIDAndName(IDD_SYSTEM_ONLINE_PANE), false);
	//---------------------------------------------------------------------------------//	
	SetMultiLanauage(LoadIDAndName(IDOK));
	SetMultiLanauage(LoadIDAndName(IDCANCEL));
	//---------------------------------------------------------------------------------//	
}
//-------------------------------------------------------------------------------------//
bool CSystemAuthorizationPane::SetMultiLanauage(UINT ID, LPCTSTR Text, bool bCtrlID)
{	
	LPCTSTR Section=_T("IDD_SYSTEM_AUTHORIZATION_PANE");		
	if ( AOIDataCollect.SwitchMultiLanguageWnd(this, Section, ID, Text, bCtrlID) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
CString CSystemAuthorizationPane::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_SYSTEM_AUTHORIZATION_PANE");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
void CSystemAuthorizationPane::SetSystemParameterPtr(TSystemParameter *Ptr)
{
	m_SysParameterPtr = Ptr;
}
//-------------------------------------------------------------------------------------//
CParamUni* CSystemAuthorizationPane::GetActParamUni()
{
	return m_ParamActPtr;
}
//-------------------------------------------------------------------------------------//
void CSystemAuthorizationPane::SetActParamUni(CParamUni *Ptr)
{
	m_ParamActPtr = Ptr;
}
//-------------------------------------------------------------------------------------//
CString CSystemAuthorizationPane::GetTimeString(__int64 Time) const
{
	return AOIDataDefine.GetOnlineAutoStopBySpecTimeText(Time);	
}
//-------------------------------------------------------------------------------------//
bool CSystemAuthorizationPane::BuildParamList()
{
	CThisListCtrl_56 &ListCtrl = m_ParamListCtrl;
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
	USER_LEVEL_MODE UserLevelMode = AOIDataCollect.GetCurrentUserLevel();	
	const int ComputerCPUCoreNumber = AOIDataCollect.GetComputerCPUCoreNumber();
	CParamList &ParamList = m_ParamList;

	ParamList.clear();

	//操作等級-專案開啟
	ParamUnit = CParamUni();
	str = _T("Project Open");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_OPERATE_LEVEL_PROJECT_OPEN;
	ParamUnit.SetParamID((UINT)(ParamID));
	SetParamUniLevel(ParamUnit, Ptr->m_OperateLevelProjectOpen, USER_LEVEL_SIGN_OUT);	
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//操作等級-專案儲存	
	ParamUnit = CParamUni();
	str = _T("Project Save");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_OPERATE_LEVEL_PROJECT_SAVE;
	ParamUnit.SetParamID((UINT)(ParamID));
	SetParamUniLevel(ParamUnit, Ptr->m_OperateLevelProjectSave, USER_LEVEL_SIGN_OUT);	
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//操作等級-專案參數
	ParamUnit = CParamUni();
	str = _T("Project Param");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_OPERATE_LEVEL_PROJECT_PARAM;
	ParamUnit.SetParamID((UINT)(ParamID));
	SetParamUniLevel(ParamUnit, Ptr->m_OperateLevelProjectParam, USER_LEVEL_SIGN_OUT);	
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//線上運行
	ParamUnit = CParamUni();
	str = _T("Online Run");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_OPERATE_LEVEL_ONLINE_RUN;
	ParamUnit.SetParamID((UINT)(ParamID));
	SetParamUniLevel(ParamUnit, Ptr->m_OperateLevelOnlineRun, USER_LEVEL_SIGN_OUT);	
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//線上直通
	ParamUnit = CParamUni();
	str = _T("Online Bypass");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_OPERATE_LEVEL_ONLINE_BYPASS;
	ParamUnit.SetParamID((UINT)(ParamID));
	SetParamUniLevel(ParamUnit, Ptr->m_OperateLevelOnlineBypass, USER_LEVEL_SIGN_OUT);	
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//線上停止
	ParamUnit = CParamUni();
	str = _T("Online Stop");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_OPERATE_LEVEL_ONLINE_STOP;
	ParamUnit.SetParamID((UINT)(ParamID));
	SetParamUniLevel(ParamUnit, Ptr->m_OperateLevelOnlineStop, USER_LEVEL_SIGN_OUT);	
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//線上存圖
	ParamUnit = CParamUni();
	str = _T("Online Save Image");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_OPERATE_LEVEL_ONLINE_SAVE_IMAGE;
	ParamUnit.SetParamID((UINT)(ParamID));
	SetParamUniLevel(ParamUnit, Ptr->m_OperateLevelOnlineSaveImage);	
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//線上解鎖	
	ParamUnit = CParamUni();
	str = _T("Online Unlock");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_OPERATE_LEVEL_ONLINE_UNLOCK;
	ParamUnit.SetParamID((UINT)(ParamID));
	SetParamUniLevel(ParamUnit, Ptr->m_OperateLevelOnlineUnlock);	
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//編輯功能-新增		
	ParamUnit = CParamUni();
	str = _T("Edit Func Add");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_OPERATE_LEVEL_EDIT_FUNC_ADD;
	ParamUnit.SetParamID((UINT)(ParamID));
	SetParamUniLevel(ParamUnit, Ptr->m_OperateLevelEditFuncAdd);	
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//編輯功能-刪除
	ParamUnit = CParamUni();
	str = _T("Edit Func Delete");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_OPERATE_LEVEL_EDIT_FUNC_DEL;
	ParamUnit.SetParamID((UINT)(ParamID));
	SetParamUniLevel(ParamUnit, Ptr->m_OperateLevelEditFuncDel);	
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//編輯功能-不檢測
	ParamUnit = CParamUni();
	str = _T("Edit Func Bypass");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_OPERATE_LEVEL_EDIT_FUNC_BYPASS;
	ParamUnit.SetParamID((UINT)(ParamID));
	SetParamUniLevel(ParamUnit, Ptr->m_OperateLevelEditFuncBypass);	
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);		

	//操作等級-MES相關-控制狀態-離線
	ParamUnit = CParamUni();
	str = _T("MES Comm Ctrl State [Offline]");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_OPERATE_LEVEL_MES_CTRL_STATE_OFFLINE;
	ParamUnit.SetParamID((UINT)(ParamID));
	SetParamUniLevel(ParamUnit, Ptr->m_OperateLevelMesCtrlState_Offline, USER_LEVEL_SIGN_OUT);	
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//操作等級-MES相關-控制狀態-本地連線
	ParamUnit = CParamUni();
	str = _T("MES Comm Ctrl State [Local]");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_OPERATE_LEVEL_MES_CTRL_STATE_LOCAL;
	ParamUnit.SetParamID((UINT)(ParamID));
	SetParamUniLevel(ParamUnit, Ptr->m_OperateLevelMesCtrlState_Local, USER_LEVEL_SIGN_OUT);	
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//操作等級-MES相關-控制狀態-遠端離線
	ParamUnit = CParamUni();
	str = _T("MES Comm Ctrl State [Remote]");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_OPERATE_LEVEL_MES_CTRL_STATE_REMOTE;
	ParamUnit.SetParamID((UINT)(ParamID));
	SetParamUniLevel(ParamUnit, Ptr->m_OperateLevelMesCtrlState_Remote, USER_LEVEL_SIGN_OUT);	
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//操作等級-MES相關-顯示內容
	ParamUnit = CParamUni();
	str = _T("MES Comm Show Context");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_OPERATE_LEVEL_MES_SHOW_CONTEXT;
	ParamUnit.SetParamID((UINT)(ParamID));
	SetParamUniLevel(ParamUnit, Ptr->m_OperateLevelMesShowContext, USER_LEVEL_SIGN_OUT);	
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CSystemAuthorizationPane::BuildParamListWnd()
{
	SetActParamUni(NULL);
	CThisListCtrl_56 &ListCtrl = m_ParamListCtrl;	
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
bool CSystemAuthorizationPane::BuildParamListWndHeader()
{
	CString str;
	int   nCol = 0;
	int width  = 64;
	int width2 = 64;
	RECT      Rect;	
	const int Align = LVCFMT_LEFT;//LVCFMT_CENTER;LVCFMT_RIGHT, LVCFMT_LEFT
	CThisListCtrl_56 &ListCtrl = m_ParamListCtrl;

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
bool CSystemAuthorizationPane::SetParamUniLevel(CParamUni &ParamUnit, USER_LEVEL_MODE Level, USER_LEVEL_MODE LowLevel)
{	
	USER_LEVEL_MODE eLevelParam=Level;	
	USER_LEVEL_MODE eLevelCurrent=AOIDataCollect.GetCurrentUserLevel();

	if ( eLevelParam > eLevelCurrent )//權限不足
	{	
		ParamUnit.SetReadOnly(true);	
		ParamUnit.SetValue_STR(AOIDataDefine.GetUserLevelModeText(eLevelParam));
		return true;
	}

	CString strValue;
	USER_LEVEL_MODE eLevel;
	eLevel = USER_LEVEL_SIGN_OUT;
	if ( eLevel>=LowLevel && eLevel<=eLevelCurrent )
	{	strValue = AOIDataDefine.GetUserLevelModeText(eLevel);		ParamUnit.AddSelItem(eLevel, strValue);	}

	eLevel = USER_LEVEL_OPERATOR;
	if ( eLevel>=LowLevel && eLevel<=eLevelCurrent )
	{	strValue = AOIDataDefine.GetUserLevelModeText(eLevel);		ParamUnit.AddSelItem(eLevel, strValue);	}

	eLevel = USER_LEVEL_ENGINEER;
	if ( eLevel>=LowLevel && eLevel<=eLevelCurrent )
	{	strValue = AOIDataDefine.GetUserLevelModeText(eLevel);		ParamUnit.AddSelItem(eLevel, strValue);	}

	eLevel = USER_LEVEL_SUPERVISOR;
	if ( eLevel>=LowLevel && eLevel<=eLevelCurrent )
	{	strValue = AOIDataDefine.GetUserLevelModeText(eLevel);		ParamUnit.AddSelItem(eLevel, strValue);	}

	eLevel = USER_LEVEL_JET_FAE;
	if ( eLevel>=LowLevel && eLevel<=eLevelCurrent )
	{	strValue = AOIDataDefine.GetUserLevelModeText(eLevel);		ParamUnit.AddSelItem(eLevel, strValue);	}

	eLevel = USER_LEVEL_JET_SENIOR;
	if ( eLevel>=LowLevel && eLevel<=eLevelCurrent )
	{	strValue = AOIDataDefine.GetUserLevelModeText(eLevel);		ParamUnit.AddSelItem(eLevel, strValue);	}

	eLevel = USER_LEVEL_JET_RD;
	if ( eLevel>=LowLevel && eLevel<=eLevelCurrent )
	{	strValue = AOIDataDefine.GetUserLevelModeText(eLevel);		ParamUnit.AddSelItem(eLevel, strValue);	}	

	ParamUnit.SetValue_SEL(eLevelParam);
	return true;
}
//-------------------------------------------------------------------------------------//
void CSystemAuthorizationPane::OnItemchangedParamListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
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
void CSystemAuthorizationPane::OnDblclkParamListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
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
void CSystemAuthorizationPane::SetDescriptionText(const CParamUni *Ptr)
{
	UINT CtrlID = SYSAUTH_INFO_EDIT;
	if ( NULL == Ptr )
	{
		CWnd::SetDlgItemText(CtrlID, _T(""));
		return ;
	}
	CWnd::SetDlgItemText(CtrlID, Ptr->GetDesction());
}
//-------------------------------------------------------------------------------------//
bool CSystemAuthorizationPane::ExecItemchangedParamListWnd(CThisListCtrl_56 &ListCtrl, int nItem)
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
	{	
		m_BtnCtrl.ShowWindow(SW_HIDE);			
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CSystemAuthorizationPane::ExecDblclkParamListWnd(CThisListCtrl_56 &ListCtrl, int nItem, int nSubItem)
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
void CSystemAuthorizationPane::OnKillfocusParamEdit() 
{
	// TODO: Add your control notification handler code here
	if ( m_EditCtrl.IsWindowVisible() == FALSE ) { return; }
	ExecUpdateParamByEdit();
	m_EditCtrl.ShowWindow(SW_HIDE);	
	m_EditCtrl.SetWindowText(_T(""));
}
//-------------------------------------------------------------------------------------//
void CSystemAuthorizationPane::OnKillfocusParamCombo() 
{
	// TODO: Add your control notification handler code here
	if ( m_ComboxCtrl.IsWindowVisible() == FALSE ) { return; }
	ExecUpdateParamByCombox();
	m_ComboxCtrl.ShowWindow(SW_HIDE);	
	JetAPI::ClearCombox(m_ComboxCtrl);	
}
//-------------------------------------------------------------------------------------//
void CSystemAuthorizationPane::OnSelchangeParamCombo() 
{
	// TODO: Add your control notification handler code here
	if ( ExecUpdateParamByCombox() == false )
	{	return; }
	m_ComboxCtrl.ShowWindow(SW_HIDE);	
	JetAPI::ClearCombox(m_ComboxCtrl);	
}
//-------------------------------------------------------------------------------------//
void CSystemAuthorizationPane::OnParamBtn() 
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
	CThisListCtrl_56 *pListCtrl = (CThisListCtrl_56*)(ParamPtr->GetListCtrl());

	if ( NULL != pListCtrl )
	{
		const int ItemCount = pListCtrl->GetItemCount();
		pListCtrl->SetItemText(nItem, nSubItem, ItemText);
		pListCtrl->SetFocus();
	}
}
//-------------------------------------------------------------------------------------//
bool CSystemAuthorizationPane::ExecUpdateParamByEdit()
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
	
	CThisListCtrl_56 *pListCtrl = (CThisListCtrl_56*)(ParamPtr->GetListCtrl());
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
bool CSystemAuthorizationPane::ExecReleaseParamCtrl()
{
	m_BtnCtrl.ShowWindow(SW_HIDE);	
	m_EditCtrl.ShowWindow(SW_HIDE);	
	m_ComboxCtrl.ShowWindow(SW_HIDE);	

	m_EditCtrl.SetWindowText(_T(""));
	m_ParamListCtrl.SetFocus();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CSystemAuthorizationPane::ExecUpdateParamByCombox()
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
	CThisListCtrl_56 *pListCtrl = (CThisListCtrl_56*)(ParamPtr->GetListCtrl());
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