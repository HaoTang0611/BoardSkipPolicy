// ITSCommWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "ITSCommWnd.h"
//-------------------------------------------------------------------------------------//
#include "JsonCtrl.h"
#include "InputListWnd.h"
#include "MES\\MES_Class.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
#define ITS_COMM_TIMER_SHOW_MSG       100
//-------------------------------------------------------------------------------------//
CITSCommWnd ITSCommWnd;
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CITSCommWnd dialog
//-------------------------------------------------------------------------------------//
CITSCommWnd::CITSCommWnd(CWnd* pParent /*=NULL*/)
	: CDialog(CITSCommWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CITSCommWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ConvertCode = CP_UTF8;
	m_TickCountRecv = 0;	
	m_TickCountSend = 0;
}
//-------------------------------------------------------------------------------------//
void CITSCommWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CITSCommWnd)
	DDX_Control(pDX, ITS_SEND_LIST_BOX, m_ClientSendListBox);	
	DDX_Control(pDX, ITS_RECV_LIST_BOX, m_ClientRecvListBox);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CITSCommWnd, CDialog)
	//{{AFX_MSG_MAP(CITSCommWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_GETMINMAXINFO()
	ON_WM_TIMER()
	ON_BN_CLICKED(ITS_CONNECT_BTN, OnConnectBtn)
	ON_BN_CLICKED(ITS_DISCONNECT_BTN, OnDisconnectBtn)
	ON_BN_CLICKED(ITS_SEND_MSG_BTN, OnSendMsgBtn)
	ON_BN_CLICKED(ITS_SEND_FILE_BTN, OnSendFileBtn)
	ON_BN_CLICKED(ITS_PROJECT_UPLOAD_BTN, OnProjectUploadBtn)
	ON_BN_CLICKED(ITS_PROJECT_DOWNLOAD_BTN, OnProjectDownloadBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CITSCommWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CITSCommWnd::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SwitchMultiLanguage();

	HWND hWnd = GetSafeHwnd();
	TSystemParameter &SysParam = AOIDataCollect.GetSystemParameter();
	CString IPAddress = SysParam.m_ITSSocketIPAddress;
	CIPAddressCtrl *IPAddCtrl = (CIPAddressCtrl*)(CWnd::GetDlgItem(ITS_IP_ADDRESS_CTRL));	
	if ( NULL != IPAddCtrl )
	{	
		BYTE Field0=0, Filed1=0, Filed2=0, Field3=0;
		if ( JetAPI::ExtractIPAddress(IPAddress, Field0, Filed1, Filed2, Field3) == false ) 
		{	IPAddCtrl->SetAddress(127, 0, 0, 1);  }
		else		
		{	IPAddCtrl->SetAddress(Field0, Filed1, Filed2, Field3);  }
	}
	//m_ITSCommunicationMode=ITS_COMMUNICATION_SOCKET;//ITS通訊模式
	
	CWnd::SetDlgItemInt(ITS_PORT_EDIT, SysParam.m_ITSSocketIPPort);
	CWnd::SetDlgItemInt(ITS_RABBITMQ_PORT_EDIT, SysParam.m_RabbitMQServerPort);
	CWnd::SetDlgItemText(ITS_RABBITMQ_ADDRESS_EDIT, SysParam.m_RabbitMQServerAddress);
	CWnd::SetDlgItemText(ITS_RABBITMQ_USERNAME_EDIT, SysParam.m_RabbitMQUserName);
	CWnd::SetDlgItemText(ITS_RABBITMQ_PASSWORD_EDIT, SysParam.m_RabbitMQPassword);
	CWnd::SetDlgItemText(ITS_RABBITMQ_QUEUE_RECV_EDIT, SysParam.m_ITSRabbitMQRecvQueueName);
	CWnd::SetDlgItemText(ITS_RABBITMQ_QUEUE_SEND_EDIT, SysParam.m_ITSRabbitMQSendQueueName);

	CWnd::CheckDlgButton(ITS_ENABLE_SOCKET_CHK, SysParam.m_ITSCommunicationEnabled);	

	CWnd::SetDlgItemText(ITS_FILECHECKER_SEND_EDIT, SysParam.m_ITSFileFolderSend);
	CWnd::SetDlgItemText(ITS_FILECHECKER_RECV_EDIT, SysParam.m_ITSFileFolderRecv);		

	SetMesCallback(true);
	UpdateCommunicationUI();
	UpdateSystemSocketListToUI();

	AOIDataCollect.CreateUIWndFont(hWnd, m_Font);
	//AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());//會有問題, 待查
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CITSCommWnd::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	SetMesCallback(false);
}
//-------------------------------------------------------------------------------------//
void CITSCommWnd::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if ( CWnd::GetSafeHwnd() == NULL ) { return; }

	CWnd *WndPtr = NULL;
	const int GapY=4;
	WndPtr = CWnd::GetDlgItem(ITS_SEND_GROUP);
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

	WndPtr = CWnd::GetDlgItem(ITS_RECV_GROUP);
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
void CITSCommWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here	
	bool Used=true;
	if ( TRUE == bShow )
	{	Used = true;	}
	else
	{	Used = false;	}
	SetMesCallback(Used);
	if ( true==Used )
	{
		if ( AOIDataCollect.OperateLevelMesShowContext() == false )
		{
			ClearListBox(m_ClientRecvListBox);
			ClearListBox(m_ClientSendListBox);
		}
	}
}
//-------------------------------------------------------------------------------------//
void CITSCommWnd::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnGetMinMaxInfo(lpMMI);
	lpMMI->ptMinTrackSize.x = 1024;
	lpMMI->ptMinTrackSize.y =  600;
}
//-------------------------------------------------------------------------------------//
void CITSCommWnd::OnTimer(UINT_PTR nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default	
	switch ( nIDEvent )
	{
	case ITS_COMM_TIMER_SHOW_MSG:
		CWnd::KillTimer(nIDEvent);
		CWnd::PostMessage(WM_COMMAND, IDCANCEL, NULL);
		break;
	}
	CDialog::OnTimer(nIDEvent);
}
//-------------------------------------------------------------------------------------//
BOOL CITSCommWnd::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::PreTranslateMessage(pMsg);
}
//-------------------------------------------------------------------------------------//
LRESULT CITSCommWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch ( message )
	{
	case MSG_SOCKET_WND:
		switch ( wParam )
		{
		case SOCKET_CLIENT_RECV_TEXT:
			if ( AOIDataCollect.OperateLevelMesShowContext() == true )
			{	
				std::string  sBuf;
				std::wstring wsBuf;
				char *pStr = NULL;
				bool bTickCount=false;
				DWORD &rTickCount=m_TickCountRecv;
				if ( NULL != lParam )
				{	char *pStr = (LPSTR)lParam; }
				else
				{	
				#ifndef MES_DISABLE
					CITSLinker &MESLinker=MES_OBJ.GetMESLinker();
					if ( rTickCount < MESLinker.GetTickCountRecv() )
					{
						bTickCount=true;
						MESLinker.LockBufferRecv();						
						sBuf = MESLinker.GetNewBufferRecv();						
						MESLinker.UnlockBufferRecv();
						if ( sBuf.length() > 0 )
						{	pStr = const_cast<char*>(sBuf.c_str()); }
					}
				#endif//MES_DISABLE
				}	
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
					if ( bTickCount )
					{	rTickCount = GetTickCount();	}
				}
			}
			break;
		case SOCKET_CLIENT_SEND_TEXT:
			if ( AOIDataCollect.OperateLevelMesShowContext() == true )
			{
				std::string  sBuf;
				std::wstring wsBuf;
				char *pStr = NULL;
				bool bTickCount=false;
				DWORD &rTickCount=m_TickCountSend;
				if ( NULL != lParam )
				{	pStr = (LPSTR)lParam;	}
				else
				{
				#ifndef MES_DISABLE
					CITSLinker &MESLinker=MES_OBJ.GetMESLinker();
					if ( rTickCount < MESLinker.GetTickCountSend() )
					{
						bTickCount=true;
						MESLinker.LockBufferSend();						
						sBuf = MESLinker.GetNewBufferSend();						
						MESLinker.UnlockBufferSend();
						if ( sBuf.length() > 0 )
						{	pStr = const_cast<char*>(sBuf.c_str());	}
					}
				#endif//MES_DISABLE
				}
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
					if ( bTickCount )
					{	rTickCount = GetTickCount();	}
				}
			}
			break;
		}
		break;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void CITSCommWnd::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_ITS_COMM_WND");
	//---------------------------------------------------------------------------------//
	WndID = IDD_ITS_COMM_WND;
	WndKey = _T("IDD_ITS_COMM_WND");
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
	WndID = ITS_PORT_LABEL;
	WndKey = _T("ITS_PORT_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ITS_IP_ADDRESS_LABEL;
	WndKey = _T("ITS_IP_ADDRESS_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ITS_ENABLE_SOCKET_CHK;
	WndKey = _T("ITS_ENABLE_SOCKET_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	
	WndID = ITS_CONNECT_BTN;
	WndKey = _T("ITS_CONNECT_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	
	WndID = ITS_RABBITMQ_PORT_LABEL;
	WndKey = _T("ITS_RABBITMQ_PORT_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ITS_RABBITMQ_ADDRESS_LABEL;
	WndKey = _T("ITS_RABBITMQ_ADDRESS_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ITS_RABBITMQ_USERNAME_LABEL;
	WndKey = _T("ITS_RABBITMQ_USERNAME_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ITS_RABBITMQ_PASSWORD_LABEL;
	WndKey = _T("ITS_RABBITMQ_PASSWORD_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ITS_RABBITMQ_QUEUE_RECV_LABEL;
	WndKey = _T("ITS_RABBITMQ_QUEUE_RECV_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ITS_RABBITMQ_QUEUE_SEND_LABEL;
	WndKey = _T("ITS_RABBITMQ_QUEUE_SEND_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = ITS_DISCONNECT_BTN;
	WndKey = _T("ITS_DISCONNECT_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ITS_SEND_MSG_BTN;
	WndKey = _T("ITS_SEND_MSG_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ITS_SEND_FILE_BTN;
	WndKey = _T("ITS_SEND_FILE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ITS_INFO_LABEL;
	WndKey = _T("ITS_INFO_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = ITS_SYSTEM_RECV_INFO_LABEL;
	WndKey = _T("ITS_SYSTEM_RECV_INFO_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//
	WndID = ITS_SEND_GROUP;
	WndKey = _T("ITS_SEND_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ITS_RECV_GROUP;
	WndKey = _T("ITS_RECV_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = ITS_PROJECT_UPLOAD_BTN;
	WndKey = _T("ITS_PROJECT_UPLOAD_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ITS_PROJECT_DOWNLOAD_BTN;
	WndKey = _T("ITS_PROJECT_DOWNLOAD_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
}
//-------------------------------------------------------------------------------------//
CString CITSCommWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_ITS_COMM_WND");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
bool CITSCommWnd::SetShowTimer(DWORD val)//設定顯示時間	
{
	CWnd::SetTimer(ITS_COMM_TIMER_SHOW_MSG, val, NULL);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CITSCommWnd::SetMessage(CString &str)
{
	const BOOL bClear=TRUE;
	CListBox &ListBox=m_ClientRecvListBox;
	FillListBox(bClear, str, ListBox);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CITSCommWnd::ToggleConnected(bool bConnected)
{
	if ( true == bConnected )
	{
		JetAPI::EnableCtrlWnd(this, ITS_CONNECT_BTN, FALSE);
		JetAPI::EnableCtrlWnd(this, ITS_DISCONNECT_BTN, TRUE);
	}
	else
	{
		JetAPI::EnableCtrlWnd(this, ITS_CONNECT_BTN, TRUE);
		JetAPI::EnableCtrlWnd(this, ITS_DISCONNECT_BTN, FALSE);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CITSCommWnd::ClearListBox(CListBox &ListBox)
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
bool CITSCommWnd::FillListBox(BOOL bClear, CString &str, CListBox &ListBox)
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
void CITSCommWnd::ExecConnectBtn_ITS()
{
	CString str;
	TSystemParameter &SysParam = AOIDataCollect.GetSystemParameter();	
#ifndef MES_DISABLE
	CITSLinker &ITSLinker = MES_OBJ.GetMESLinker();
	if ( ITS_COMMUNICATION_RABBIT_MQ == SysParam.m_ITSCommunicationMode ) 
	{
		char  strIP[64]="";
		char  strUserName[64]="";
		char  strPassword[64]="";
		char  strRecvName[64]="";
		char  strSendName[64]="";
		const int nPort = CWnd::GetDlgItemInt(ITS_RABBITMQ_PORT_EDIT);	
		CWnd::GetDlgItemText(ITS_RABBITMQ_ADDRESS_EDIT, str);	JetAPI::TCHAR2char(str, strIP, 64);
		CWnd::GetDlgItemText(ITS_RABBITMQ_USERNAME_EDIT, str);	JetAPI::TCHAR2char(str, strUserName, 64);
		CWnd::GetDlgItemText(ITS_RABBITMQ_PASSWORD_EDIT, str);	JetAPI::TCHAR2char(str, strPassword, 64);
		CWnd::GetDlgItemText(ITS_RABBITMQ_QUEUE_RECV_EDIT, str);	JetAPI::TCHAR2char(str, strRecvName, 64);
		CWnd::GetDlgItemText(ITS_RABBITMQ_QUEUE_SEND_EDIT, str);	JetAPI::TCHAR2char(str, strSendName, 64);

		if ( ITSLinker.ConnectITS_Rabbit(strIP, nPort, strUserName, strPassword, strRecvName, strSendName) == false )
		{
			str = ITSLinker.GetErrorString();
			JetAPI::ShowMessageBox(str);
			return; 
		}			

		SysParam.m_RabbitMQServerPort = nPort;
		SysParam.m_RabbitMQServerAddress = strIP;
		SysParam.m_RabbitMQUserName = strUserName;
		SysParam.m_RabbitMQPassword = strPassword;
		SysParam.m_ITSRabbitMQRecvQueueName = strRecvName;
		SysParam.m_ITSRabbitMQSendQueueName = strSendName;		
	}
	else if ( ITS_COMMUNICATION_FILE_CHECKER == SysParam.m_ITSCommunicationMode ) 
	{
		CString strFolderTemp;
		CString strFolderSend;
		CString strFolderRecv;
		CString strBackupFolderSend;
		CString strBackupFolderRecv;
		bool    bBackup=SysParam.m_ITSFileBackupEnabled;
		bool    bUseSync=SysParam.m_ITSFileUseSyncFileEnabled;
		CString FolderTemp=AOIDataCollect.GetAOITempDirectory();
		CString strLogFolder=AOIDataCollect.GetAOILogDirectory();
		CWnd::GetDlgItemText(ITS_FILECHECKER_SEND_EDIT, strFolderSend);
		CWnd::GetDlgItemText(ITS_FILECHECKER_RECV_EDIT, strFolderRecv);
		strFolderTemp.Format(_T("%s\\%s"), FolderTemp, _T("ITSFileTemp"));
		strBackupFolderSend.Format(_T("%s\\ITSFileBackup"), strLogFolder);
		strBackupFolderRecv.Format(_T("%s\\ITSFileBackup"), strLogFolder);
		::CreateDirectory(strFolderTemp, NULL); ::Sleep(0);
		::CreateDirectory(strBackupFolderSend, NULL); ::Sleep(0);
		::CreateDirectory(strBackupFolderRecv, NULL); ::Sleep(0);
		if ( ITSLinker.ConnectITS_File(strFolderSend, strFolderRecv, strFolderTemp, bBackup, bUseSync, strBackupFolderSend, strBackupFolderRecv) == false )
		{
			str = ITSLinker.GetErrorString();
			JetAPI::ShowMessageBox(str);
			return; 
		}
		SysParam.m_ITSFileFolderSend = strFolderSend;
		SysParam.m_ITSFileFolderRecv = strFolderRecv;
	}
	else
	{
		char  strIP[64]="";
		const int nPort = CWnd::GetDlgItemInt(ITS_PORT_EDIT);	
		CIPAddressCtrl *IPAddCtrl = (CIPAddressCtrl*)(CWnd::GetDlgItem(ITS_IP_ADDRESS_CTRL));
		BYTE field1=0, field2=0, field3=0, field4=0;
		IPAddCtrl->GetAddress(field1, field2, field3, field4);
		::sprintf(strIP, "%d.%d.%d.%d", field1, field2, field3, field4);
		if ( ITSLinker.ConnectITS_Socket(strIP, nPort) == false )
		{
			str = ITSLinker.GetErrorString();
			JetAPI::ShowMessageBox(str);
			return; 
		}	
		SysParam.m_ITSSocketIPPort = nPort;
		SysParam.m_ITSSocketIPAddress = strIP;
	}
	

	ToggleConnected(true);	
	//將初步連線的文字給予清除	
	::Sleep(1000);
	std::vector<std::string> RecvList;		
	ITSLinker.CloneITSRecvMsgList(RecvList, true);
	JetAPI::ShowMessageBox(_T("Connect OK"));
#endif//MES_DISABLE
	return;
}
//-------------------------------------------------------------------------------------//
void CITSCommWnd::ExecConnectBtn_IPS()
{
	CString str;
	TSystemParameter &SysParam = AOIDataCollect.GetSystemParameter();	
#ifndef MES_DISABLE
	CITSLinker &IPSLinker = MES_OBJ.GetMESLinker();
	if ( ITS_COMMUNICATION_RABBIT_MQ == SysParam.m_ITSCommunicationMode ) 
	{
		char  strIP[64]="";
		char  strUserName[64]="";
		char  strPassword[64]="";
		char  strRecvName[64]="";
		char  strSendName[64]="";
		const int nPort = CWnd::GetDlgItemInt(ITS_RABBITMQ_PORT_EDIT);	
		CWnd::GetDlgItemText(ITS_RABBITMQ_ADDRESS_EDIT, str);	JetAPI::TCHAR2char(str, strIP, 64);
		CWnd::GetDlgItemText(ITS_RABBITMQ_USERNAME_EDIT, str);	JetAPI::TCHAR2char(str, strUserName, 64);
		CWnd::GetDlgItemText(ITS_RABBITMQ_PASSWORD_EDIT, str);	JetAPI::TCHAR2char(str, strPassword, 64);
		CWnd::GetDlgItemText(ITS_RABBITMQ_QUEUE_RECV_EDIT, str);	JetAPI::TCHAR2char(str, strRecvName, 64);
		CWnd::GetDlgItemText(ITS_RABBITMQ_QUEUE_SEND_EDIT, str);	JetAPI::TCHAR2char(str, strSendName, 64);

		if ( IPSLinker.ConnectITS_Rabbit(strIP, nPort, strUserName, strPassword, strRecvName, strSendName) == false )
		{
			str = IPSLinker.GetErrorString();
			JetAPI::ShowMessageBox(str);
			return; 
		}			

		SysParam.m_RabbitMQServerPort = nPort;
		SysParam.m_RabbitMQServerAddress = strIP;
		SysParam.m_RabbitMQUserName = strUserName;
		SysParam.m_RabbitMQPassword = strPassword;
		SysParam.m_ITSRabbitMQRecvQueueName = strRecvName;
		SysParam.m_ITSRabbitMQSendQueueName = strSendName;		
	}
	else if ( ITS_COMMUNICATION_FILE_CHECKER == SysParam.m_ITSCommunicationMode ) 
	{
		CString strFolderTemp;
		CString strFolderSend;
		CString strFolderRecv;
		CString strBackupFolderSend;
		CString strBackupFolderRecv;
		bool    bBackup=SysParam.m_ITSFileBackupEnabled;
		bool    bUseSync=SysParam.m_ITSFileUseSyncFileEnabled;
		CString FolderTemp=AOIDataCollect.GetAOITempDirectory();
		CString strLogFolder=AOIDataCollect.GetAOILogDirectory();
		CWnd::GetDlgItemText(ITS_FILECHECKER_SEND_EDIT, strFolderSend);
		CWnd::GetDlgItemText(ITS_FILECHECKER_RECV_EDIT, strFolderRecv);
		strFolderTemp.Format(_T("%s\\%s"), FolderTemp, _T("ITSFileTemp"));
		strBackupFolderSend.Format(_T("%s\\ITSFileBackup"), strLogFolder);
		strBackupFolderRecv.Format(_T("%s\\ITSFileBackup"), strLogFolder);
		::CreateDirectory(strFolderTemp, NULL); ::Sleep(0);
		::CreateDirectory(strBackupFolderSend, NULL); ::Sleep(0);
		::CreateDirectory(strBackupFolderRecv, NULL); ::Sleep(0);
		if ( IPSLinker.ConnectITS_File(strFolderSend, strFolderRecv, strFolderTemp, bBackup, bUseSync, strBackupFolderSend, strBackupFolderRecv) == false )
		{
			str = IPSLinker.GetErrorString();
			JetAPI::ShowMessageBox(str);
			return; 
		}
		SysParam.m_ITSFileFolderSend = strFolderSend;
		SysParam.m_ITSFileFolderRecv = strFolderRecv;
	}
	else
	{
		char  strIP[64]="";
		const int nPort = CWnd::GetDlgItemInt(ITS_PORT_EDIT);	
		CIPAddressCtrl *IPAddCtrl = (CIPAddressCtrl*)(CWnd::GetDlgItem(ITS_IP_ADDRESS_CTRL));
		BYTE field1=0, field2=0, field3=0, field4=0;
		IPAddCtrl->GetAddress(field1, field2, field3, field4);
		::sprintf(strIP, "%d.%d.%d.%d", field1, field2, field3, field4);
		if ( IPSLinker.ConnectITS_Socket(strIP, nPort) == false )
		{
			str = IPSLinker.GetErrorString();
			JetAPI::ShowMessageBox(str);
			return; 
		}	
		SysParam.m_ITSSocketIPPort = nPort;
		SysParam.m_ITSSocketIPAddress = strIP;
	}
	

	ToggleConnected(true);	
	//將初步連線的文字給予清除	
	::Sleep(1000);
	std::vector<std::string> RecvList;		
	IPSLinker.CloneITSRecvMsgList(RecvList, true);
	JetAPI::ShowMessageBox(_T("Connect OK"));
#endif//MES_DISABLE
	return;
}
//-------------------------------------------------------------------------------------//
void CITSCommWnd::OnConnectBtn() 
{
	// TODO: Add your control notification handler code here
#ifndef MES_DISABLE
	MES_IMP_CLS MesImpCls=MES_OBJ.GetMesImpCls();
	if ( MES_IMP_CLS_ITS == MesImpCls )
	{	ExecConnectBtn_ITS();	}
	if ( MES_IMP_CLS_IPS == MesImpCls )
	{	ExecConnectBtn_IPS();	}	
#endif//MES_DISABLE
}
//-------------------------------------------------------------------------------------//
void CITSCommWnd::OnDisconnectBtn() 
{
	// TODO: Add your control notification handler code here
#ifndef MES_DISABLE
	CITSLinker &MESLinker = MES_OBJ.GetMESLinker();
	MESLinker.DisconnectITS();
#endif//MES_DISABLE
	ToggleConnected(false);	
}
//-------------------------------------------------------------------------------------//
void CITSCommWnd::OnSendMsgBtn() 
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
	CWnd::SetDlgItemText(ITS_INFO_EDIT, str);

	UpdateSystemSocketListToUI();
}
//-------------------------------------------------------------------------------------//
void CITSCommWnd::OnSendFileBtn() 
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
	CWnd::SetDlgItemText(ITS_INFO_EDIT, str);

	UpdateSystemSocketListToUI();
}
//-------------------------------------------------------------------------------------//
bool CITSCommWnd::SetMesCallback(bool Used)
{
#ifndef MES_DISABLE	
	HWND hWnd=GetSafeHwnd();
	CITSLinker &MESLinker=MES_OBJ.GetMESLinker();
	if ( false == Used )
	{	MESLinker.SetITSHWnd(NULL, MSG_SOCKET_WND, SOCKET_CLIENT_RECV_TEXT, SOCKET_CLIENT_SEND_TEXT);	}
	else
	{	MESLinker.SetITSHWnd(hWnd, MSG_SOCKET_WND, SOCKET_CLIENT_RECV_TEXT, SOCKET_CLIENT_SEND_TEXT);	}
	bool bConnected = MESLinker.GetITSConnected();
	ToggleConnected(bConnected);	
#endif//MES_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CITSCommWnd::UpdateCommunicationUI()//更新連線介面
{
	BOOL bEnable = TRUE;
	BOOL bEnableRabbit = FALSE;
	BOOL bEnableSocket = FALSE;
	BOOL bEnableFile   = FALSE;
	TSystemParameter &SysParam = AOIDataCollect.GetSystemParameter();
	if ( ITS_COMMUNICATION_RABBIT_MQ == SysParam.m_ITSCommunicationMode )
	{	bEnableRabbit = TRUE;	}
	else if ( ITS_COMMUNICATION_FILE_CHECKER == SysParam.m_ITSCommunicationMode ) 
	{	bEnableFile = TRUE;	}
	else
	{	bEnableSocket = TRUE;	}	

	bEnable = bEnableSocket;
	JetAPI::EnableEditWnd(this, ITS_PORT_EDIT, bEnable);
	JetAPI::EnableCtrlWnd(this, ITS_IP_ADDRESS_CTRL, bEnable);

	bEnable = bEnableRabbit;
	JetAPI::EnableEditWnd(this, ITS_RABBITMQ_PORT_EDIT, bEnable);
	JetAPI::EnableEditWnd(this, ITS_RABBITMQ_ADDRESS_EDIT, bEnable);
	JetAPI::EnableEditWnd(this, ITS_RABBITMQ_USERNAME_EDIT, bEnable);
	JetAPI::EnableEditWnd(this, ITS_RABBITMQ_PASSWORD_EDIT, bEnable);
	JetAPI::EnableEditWnd(this, ITS_RABBITMQ_QUEUE_RECV_EDIT, bEnable);
	JetAPI::EnableEditWnd(this, ITS_RABBITMQ_QUEUE_SEND_EDIT, bEnable);	

	bEnable = bEnableFile;
	JetAPI::EnableEditWnd(this, ITS_FILECHECKER_SEND_EDIT, bEnable);
	JetAPI::EnableEditWnd(this, ITS_FILECHECKER_RECV_EDIT, bEnable);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CITSCommWnd::UpdateSystemSocketListToUI()
{
	CString str;
	const size_t RecvNodeCount = AOIDataCollect.GetMESRecvNodeCount();	
	
	str.Format(_T("%d"), RecvNodeCount);
	CWnd::SetDlgItemText(ITS_SYSTEM_RECV_INFO_EDIT, str);
	return true;
}
//-------------------------------------------------------------------------------------//
void CITSCommWnd::OnProjectUploadBtn()
{
	// TODO: Add your control notification handler code here
#ifndef MES_DISABLE
	CString Filename;
	CWnd::GetDlgItemText(ITS_PROJECT_NAME_EDIT, Filename);
	if ( MES_OBJ.ExecMESComm_UploadProjectFile(Filename) == true )
	{	AOIDataCollect.SetMESComm_RemoteCtrlProject(Filename);	}
	else
	{	JetAPI::ShowMessageBox(MES_OBJ.GetErrorString());	}
#endif//MES_DISABLE
}
//-------------------------------------------------------------------------------------//
void CITSCommWnd::OnProjectDownloadBtn()
{
	// TODO: Add your control notification handler code here
#ifndef MES_DISABLE
	CString Filename;
	CWnd::GetDlgItemText(ITS_PROJECT_NAME_EDIT, Filename);
	if ( MES_OBJ.ExecMESComm_DownloadProjectFile(Filename) == true )
	{	AOIDataCollect.SetMESComm_RemoteCtrlProject(Filename);	}
	else
	{	JetAPI::ShowMessageBox(MES_OBJ.GetErrorString());	}
#endif//MES_DISABLE
}
//-------------------------------------------------------------------------------------//
void CITSCommWnd::OnOK() 
{
	// TODO: Add extra validation here	
	AOIDataCollect.SaveSystemParameter();
	CDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CITSCommWnd::OnCancel()
{
	SetMesCallback(false);
	CDialog::OnCancel();
}
//-------------------------------------------------------------------------------------//