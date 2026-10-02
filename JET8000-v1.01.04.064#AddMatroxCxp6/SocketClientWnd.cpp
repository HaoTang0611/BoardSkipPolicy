// SocketClientWnd.cpp : implementation file
//

#include "stdafx.h"
#include "jet8000.h"
#include "SocketClientWnd.h"
//-------------------------------------------------------------------------------------//
#include "JsonCtrl.h"
#include "InputListWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CSocketClientWnd dialog
//-------------------------------------------------------------------------------------//
CSocketClientWnd::CSocketClientWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CSocketClientWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSocketClientWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ConvertCode = CP_UTF8;
}
//-------------------------------------------------------------------------------------//
void CSocketClientWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSocketClientWnd)
	DDX_Control(pDX, SCW_SEND_LIST_BOX, m_ClientSendListBox);	
	DDX_Control(pDX, SCW_RECV_LIST_BOX, m_ClientRecvListBox);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CSocketClientWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CSocketClientWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_GETMINMAXINFO()
	ON_BN_CLICKED(SCW_CONNECT_BTN, OnConnectBtn)
	ON_BN_CLICKED(SCW_DISCONNECT_BTN, OnDisconnectBtn)
	ON_BN_CLICKED(SCW_SEND_MSG_BTN, OnSendMsgBtn)	
	ON_BN_CLICKED(SCW_SEND_FILE_BTN, OnSendFileBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CSocketClientWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CSocketClientWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SwitchMultiLanguage();

	HWND hWnd = GetSafeHwnd();
	TSystemParameter &SysParam = AOIDataCollect.GetSystemParameter();
	CString IPAddress = SysParam.m_ITSSocketIPAddress;
	CIPAddressCtrl *IPAddCtrl = (CIPAddressCtrl*)(CWnd::GetDlgItem(SCW_IP_ADDRESS_CTRL));	
	if ( NULL != IPAddCtrl )
	{
		BYTE Field0=0, Filed1=0, Filed2=0, Field3=0;
		if ( JetAPI::ExtractIPAddress(IPAddress, Field0, Filed1, Filed2, Field3) == false ) 
		{	IPAddCtrl->SetAddress(127, 0, 0, 1);  }
		else		
		{	IPAddCtrl->SetAddress(Field0, Filed1, Filed2, Field3);  }
	}
	CWnd::SetDlgItemInt(SCW_PORT_EDIT, SysParam.m_ITSSocketIPPort);
	
	CWnd::CheckDlgButton(SCW_ENABLE_SOCKET_CHK, SysParam.m_ITSCommunicationEnabled);
	m_JetSocketClient.SetHWnd(hWnd, MSG_SOCKET_WND, SOCKET_CLIENT_RECV_TEXT, SOCKET_CLIENT_SEND_TEXT);	
	bool bConnected = m_JetSocketClient.GetConnected();
	ToggleConnected(bConnected);	
	
	UpdateSystemSocketListToUI();
	AOIDataCollect.CreateUIWndFont(hWnd, m_Font);
	//AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());//會有問題, 待查	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CSocketClientWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	m_JetSocketClient.SetHWnd(NULL, MSG_SOCKET_WND, SOCKET_CLIENT_RECV_TEXT, SOCKET_CLIENT_SEND_TEXT);	
}
//-------------------------------------------------------------------------------------//
void CSocketClientWnd::OnSize(UINT nType, int cx, int cy) 
{
	CBaseDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if ( CWnd::GetSafeHwnd() == NULL ) { return; }

	CWnd *WndPtr = NULL;
	const int GapY=4;
	WndPtr = CWnd::GetDlgItem(SCW_SEND_GROUP);
	if ( NULL!=WndPtr && WndPtr->GetSafeHwnd()!=NULL )
	{
		RECT WndRect={0,0,0,0};
		WndPtr->GetWindowRect(&WndRect);
		CWnd::ScreenToClient(&WndRect);
		WndRect.bottom = cy-GapY;
		WndPtr->MoveWindow(&WndRect);
	}

	WndPtr = &m_ClientSendListBox;
	if ( WndPtr->GetSafeHwnd() != NULL )
	{
		RECT WndRect={0,0,0,0};
		WndPtr->GetWindowRect(&WndRect);
		CWnd::ScreenToClient(&WndRect);
		WndRect.bottom = cy-GapY-GapY;
		WndPtr->MoveWindow(&WndRect);
	}

	WndPtr = CWnd::GetDlgItem(SCW_RECV_GROUP);
	if ( NULL!=WndPtr && WndPtr->GetSafeHwnd()!=NULL )
	{
		RECT WndRect={0,0,0,0};
		WndPtr->GetWindowRect(&WndRect);
		CWnd::ScreenToClient(&WndRect);
		WndRect.bottom = cy-GapY;
		WndPtr->MoveWindow(&WndRect);
	}

	WndPtr = &m_ClientRecvListBox;
	if ( WndPtr->GetSafeHwnd() != NULL )
	{
		RECT WndRect={0,0,0,0};
		WndPtr->GetWindowRect(&WndRect);
		CWnd::ScreenToClient(&WndRect);
		WndRect.bottom = cy-GapY-GapY;
		WndPtr->MoveWindow(&WndRect);
	}
}
//-------------------------------------------------------------------------------------//
void CSocketClientWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CBaseDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CSocketClientWnd::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default
	
	CBaseDialog::OnGetMinMaxInfo(lpMMI);
	lpMMI->ptMinTrackSize.x = 1024;
	lpMMI->ptMinTrackSize.y =  600;
}
//-------------------------------------------------------------------------------------//
BOOL CSocketClientWnd::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CBaseDialog::PreTranslateMessage(pMsg);
}
//-------------------------------------------------------------------------------------//
LRESULT CSocketClientWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch ( message )
	{
	case MSG_SOCKET_WND:
		switch ( wParam )
		{
		case SOCKET_CLIENT_RECV_TEXT:
			{					
				std::wstring wsBuf;
				char *pStr = (LPSTR)lParam;				
				if ( JetAPI::char2wstring(pStr, wsBuf, m_ConvertCode) != 0 ) 
				{
					CString s = wsBuf.c_str();					
					BOOL bChk = TRUE;//Use JSON
					if ( TRUE == bChk)
					{
						std::wstring strWBuf;
						rapidjson::WDocument Doc;
						rapidjson::CJsonCtrl JSonCtrl;						
						Doc.SetObject();
						JSonCtrl.Set(&Doc);	
						JSonCtrl.SetBuffer(wsBuf, Doc);
						JSonCtrl.GetBufferIncSpace(strWBuf, Doc);
						s = strWBuf.c_str();
					}
					FillListBox(bChk, s, m_ClientRecvListBox);					
				}
			}
			break;
		case SOCKET_CLIENT_SEND_TEXT:
			{					
				std::wstring wsBuf;
				char *pStr = (LPSTR)lParam;
				if ( JetAPI::char2wstring(pStr, wsBuf, m_ConvertCode) != 0 ) 
				{
					CString s = wsBuf.c_str();					
					BOOL bChk = TRUE;
					if ( TRUE == bChk)
					{
						std::wstring strWBuf;
						rapidjson::WDocument Doc;
						rapidjson::CJsonCtrl JSonCtrl;						
						Doc.SetObject();
						JSonCtrl.Set(&Doc);	
						JSonCtrl.SetBuffer(wsBuf, Doc);
						JSonCtrl.GetBufferIncSpace(strWBuf, Doc);
						s = strWBuf.c_str();
					}
					FillListBox(bChk, s, m_ClientSendListBox);					
				}
			}
			break;
		}
		break;
	}
	return CBaseDialog::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void CSocketClientWnd::OnConnectBtn() 
{
	// TODO: Add your control notification handler code here
	char  strIP[32]="";
	const int nPort = CWnd::GetDlgItemInt(SCW_PORT_EDIT);
	CIPAddressCtrl *IPAddCtrl = (CIPAddressCtrl*)(CWnd::GetDlgItem(SCW_IP_ADDRESS_CTRL));
	BYTE field1=0, field2=0, field3=0, field4=0;
	IPAddCtrl->GetAddress(field1, field2, field3, field4);
	::sprintf(strIP, "%d.%d.%d.%d", field1, field2, field3, field4);
	if ( m_JetSocketClient.Connect(strIP, nPort) == false )
	{	return; }	

	ToggleConnected(true);	
	//將初步連線的文字給予清除	
	::Sleep(1000);
	std::vector<std::string> RecvList;		
	m_JetSocketClient.CloneRecvMsgList(RecvList, true);
	JetAPI::ShowMessageBox(_T("Connect OK"));
}
//-------------------------------------------------------------------------------------//
void CSocketClientWnd::OnDisconnectBtn() 
{
	// TODO: Add your control notification handler code here
	m_JetSocketClient.CloseClient();
	ToggleConnected(false);	
}
//-------------------------------------------------------------------------------------//
void CSocketClientWnd::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_SOCKET_CLIENT_WND");
	//---------------------------------------------------------------------------------//
	WndID = IDD_SOCKET_CLIENT_WND;
	WndKey = _T("IDD_SOCKET_CLIENT_WND");
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
	WndID = SCW_PORT_LABEL;
	WndKey = _T("SCW_PORT_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SCW_IP_ADDRESS_LABEL;
	WndKey = _T("SCW_IP_ADDRESS_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SCW_ENABLE_SOCKET_CHK;
	WndKey = _T("SCW_ENABLE_SOCKET_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	
	WndID = SCW_CONNECT_BTN;
	WndKey = _T("SCW_CONNECT_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SCW_DISCONNECT_BTN;
	WndKey = _T("SCW_DISCONNECT_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SCW_SEND_MSG_BTN;
	WndKey = _T("SCW_SEND_MSG_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SCW_SEND_FILE_BTN;
	WndKey = _T("SCW_SEND_FILE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SCW_INFO_LABEL;
	WndKey = _T("SCW_INFO_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = SCW_SYSTEM_RECV_INFO_LABEL;
	WndKey = _T("SCW_SYSTEM_RECV_INFO_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//
	WndID = SCW_SEND_GROUP;
	WndKey = _T("SCW_SEND_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SCW_RECV_GROUP;
	WndKey = _T("SCW_RECV_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
}
//-------------------------------------------------------------------------------------//
CString CSocketClientWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_SOCKET_CLIENT_WND");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
bool CSocketClientWnd::ToggleConnected(bool bConnected)
{
	if ( true == bConnected )
	{
		JetAPI::EnableCtrlWnd(this, SCW_CONNECT_BTN, FALSE);
		JetAPI::EnableCtrlWnd(this, SCW_DISCONNECT_BTN, TRUE);
	}
	else
	{
		JetAPI::EnableCtrlWnd(this, SCW_CONNECT_BTN, TRUE);
		JetAPI::EnableCtrlWnd(this, SCW_DISCONNECT_BTN, FALSE);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CSocketClientWnd::ClearListBox(CListBox &ListBox)
{
	if ( ListBox.GetSafeHwnd() == NULL ) { return true; }
	int   i=0;
	const int Count = ListBox.GetCount();

	ListBox.SetRedraw(FALSE);
	for ( i=0; i<Count; i++ )
	{	ListBox.DeleteString(Count-i-1);	}
	ListBox.SetRedraw(TRUE);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CSocketClientWnd::FillListBox(BOOL bClear, CString &str, CListBox &ListBox)
{
	if ( TRUE == bClear )
	{
		if ( ClearListBox(ListBox) == false ) { return false; }	
	}
	else
	{
		if ( ListBox.GetSafeHwnd() == NULL ) { return false; }
	}
	int     Cnt=0;
	CString strItem;
	const int len = str.GetLength();

	ListBox.SetRedraw(FALSE);
	while ( true )
	{
		if ( AfxExtractSubString(strItem, str, Cnt, _T('\n')) == FALSE )		
		{	break; }
		if ( strItem.GetLength() == 0 ) 
		{ 
			Cnt ++;
			continue; 
		}
		ListBox.AddString(strItem);
		Cnt ++;
	}
	ListBox.SetRedraw(TRUE);
	return true;	
}
//-------------------------------------------------------------------------------------//
void CSocketClientWnd::OnSendMsgBtn() 
{
	// TODO: Add your control notification handler code here
	CString         str;
	POINT           Point;
	CString         strLabel;
	CString         strCaption;
	TListNode       Node;
	MES_STATAUS_ID  eStatusID;	
	CInputListWnd   EnumWnd;
	std::vector<TListNode> NodelList;		
	CAOIProject    *ProjectPtr = AOIDataCollect.GetActiveProject();
	
	strLabel = _T("Command");
	//strLabel = LoadMultiLanguageString(strLabel, strLabel);
	strCaption = _T("Test Sockect Command");
	//strCaption = LoadMultiLanguageString(strCaption, strCaption);		
	
	eStatusID = MES_STATAUS_AOI_SET_CMD;
	Node.Data = eStatusID;
	Node.Text = AOIDataDefine.GetMESStatusText(eStatusID);
	NodelList.push_back(Node);		

	eStatusID = MES_STATAUS_AOI_READY_TO_LOAD_CMD;
	Node.Data = eStatusID;
	Node.Text = AOIDataDefine.GetMESStatusText(eStatusID);
	NodelList.push_back(Node);

	eStatusID = MES_STATAUS_AOI_LOAd_COMPLETE_CMD;
	Node.Data = eStatusID;
	Node.Text = AOIDataDefine.GetMESStatusText(eStatusID);
	NodelList.push_back(Node);

	eStatusID = MES_STATAUS_AOI_START_INSPECTION_CMD;
	Node.Data = eStatusID;
	Node.Text = AOIDataDefine.GetMESStatusText(eStatusID);
	NodelList.push_back(Node);

	eStatusID = MES_STATAUS_AOI_INSPECTION_COMPLETE_CMD;
	Node.Data = eStatusID;
	Node.Text = AOIDataDefine.GetMESStatusText(eStatusID);
	NodelList.push_back(Node);

	eStatusID = MES_STATAUS_AOI_READY_TO_UNLOAD_CMD;
	Node.Data = eStatusID;
	Node.Text = AOIDataDefine.GetMESStatusText(eStatusID);
	NodelList.push_back(Node);

	eStatusID = MES_STATAUS_AOI_UNLOAd_COMPLETE_CMD;
	Node.Data = eStatusID;
	Node.Text = AOIDataDefine.GetMESStatusText(eStatusID);
	NodelList.push_back(Node);	
	
	eStatusID = MES_STATAUS_AOI_INSPECTION_STOP_CMD;
	Node.Data = eStatusID;
	Node.Text = AOIDataDefine.GetMESStatusText(eStatusID);
	NodelList.push_back(Node);	

	::GetCursorPos(&Point);
	EnumWnd.SetWndPos(Point);	
	EnumWnd.SetParam1(strCaption, strLabel, MES_STATAUS_AOI_READY_TO_LOAD_CMD, NodelList);	
	if ( EnumWnd.DoModal() == IDCANCEL )
	{	return ; }			
	
	bool bIsOK = true;
	LARGE_INTEGER  fnStart, fnEnd;

	ClearListBox(m_ClientRecvListBox);
	JetAPI::SetFuncTimeStart(fnStart);
	int nProcessID = EnumWnd.GetSelData();
	if ( nProcessID == MES_STATAUS_AOI_SET_CMD )
	{
		if ( NULL != ProjectPtr ) 
		{	bIsOK = AOIDataCollect.ExecMESComm_SetProjectParam(); }
		else
		{	bIsOK = AOIDataCollect.ExecMESComm_SetSystemParam(); }
	}
	else
	{	bIsOK = AOIDataCollect.ExecMESComm_ProcessID(nProcessID);	}
	JetAPI::SetFuncTimeStart(fnEnd);

	if ( false == bIsOK ) 
	{
		str = AOIDataCollect.GetErrorString();
		JetAPI::ShowMessageBox(str);
	}
	double SpentTime = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);
	str.Format(_T("Spent Time = %.3f ms"), SpentTime);
	CWnd::SetDlgItemText(SCW_INFO_EDIT, str);

	UpdateSystemSocketListToUI();
}
//-------------------------------------------------------------------------------------//
bool CSocketClientWnd::UpdateSystemSocketListToUI()
{
	CString str;
	const size_t RecvNodeCount = AOIDataCollect.GetMESRecvNodeCount();	
	
	str.Format(_T("%d"), RecvNodeCount);
	CWnd::SetDlgItemText(SCW_SYSTEM_RECV_INFO_EDIT, str);
	return true;
}
//-------------------------------------------------------------------------------------//
void CSocketClientWnd::OnSendFileBtn() 
{
	// TODO: Add your control notification handler code here
	TCHAR szFilters[]=_T("JSON Files (*.JSON)|*.JSON|All Files (*.*)|*.*||");
	CFileDialog dialog (TRUE, _T("JSON"), _T("*.JSON"), OFN_FILEMUSTEXIST, szFilters);
	if ( dialog.DoModal() == IDCANCEL )
	{	return ; }
	m_JSONFilename = dialog.GetPathName();	
	const int len = m_JSONFilename.GetLength();
	if ( 0 == len ) { return; }

	CString str;
	int bret=false;	
	rapidjson::WDocument Doc;
	rapidjson::CJsonCtrl JSonCtrl;

	ClearListBox(m_ClientRecvListBox);
	//initial
	Doc.SetObject();
	JSonCtrl.Set(&Doc);	
	
	std::wstring wsFilename = (LPCWSTR)(m_JSONFilename);	
	bret = JSonCtrl.OpenFile(wsFilename.c_str(), Doc);
	if ( false == bret )
	{
		str.Format(_T("Error, Load JSON File Fault[%s]"), m_JSONFilename);
		JetAPI::ShowMessageBox(str);
		return ;
	}


	bool bRet=false;
	bool bAck=false;
	DWORD  dwTimeout=1000;
	int nPPID=0;
	rapidjson::CGMItr itr;
	bRet=JSonCtrl.FindMember(L"PPID", itr);
	if ( true == bRet ) 
	{
		if ( itr->value.IsInt() == true ) 
		{	nPPID = itr->value.GetInt(); }
	}
	bRet=JSonCtrl.FindMember(L"ACK", itr);
	if ( true == bRet ) 
	{
		if ( itr->value.IsInt() == true ) 
		{	bAck = itr->value.GetInt(); }
	}	

	bRet=JSonCtrl.FindMember(L"Timeout", itr);
	if ( true == bRet ) 
	{
		if ( itr->value.IsInt() == true ) 
		{	dwTimeout = itr->value.GetInt(); }
	}	
	std::string  sBuf;
	std::wstring wsBuf;
	JSonCtrl.GetBuffer(wsBuf, Doc);
	JetAPI::wchar2string(wsBuf.c_str(), sBuf, m_ConvertCode);
	
	LARGE_INTEGER  fnStart, fnEnd;
	JetAPI::SetFuncTimeStart(fnStart);
	if ( AOIDataCollect.SendToMESNode(sBuf.c_str(), bAck, nPPID, dwTimeout) == false )
	{	
		str = AOIDataCollect.GetErrorString(); 
		JetAPI::ShowMessageBox(str);
	}

	JetAPI::SetFuncTimeStart(fnEnd);
	double SpentTime = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);
	str.Format(_T("Spent Time = %.3f ms"), SpentTime);
	CWnd::SetDlgItemText(SCW_INFO_EDIT, str);

	UpdateSystemSocketListToUI();
}
//-------------------------------------------------------------------------------------//