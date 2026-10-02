// FingerprintWnd.cpp : 實作檔
//

#include "stdafx.h"
#include "jet8000.h"
#include "FingerprintDevice.h"
#include "FingerprintWnd.h"
#include "afxdialogex.h"


// CFingerprintWnd 對話方塊

IMPLEMENT_DYNAMIC(CFingerprintWnd, CDialog)
//-------------------------------------------------------------------------------------//
CFingerprintWnd::CFingerprintWnd(CWnd* pParent /*=NULL*/)
	: CDialog(CFingerprintWnd::IDD, pParent)
{
	this->m_LEDGreen.LoadBitmap(IDB_LED_MEDIAN_GREEN);
	this->m_LEDRed.LoadBitmap(IDB_LED_MEDIAN_RED);
	this->m_LEDYellow.LoadBitmap(IDB_LED_MEDIAN_YELLOW);
	this->m_LEDGray.LoadBitmap(IDB_LED_MEDIAN_GRAY);

	m_FingerFeature = new BYTE[m_FingerFeatureSize];
	memset(m_FingerFeature, 0x00, sizeof(BYTE)*m_FingerFeatureSize);

	m_FPSDevicePtr = NULL;
	m_FPSDeviceType = FPS_DEVICE_PQIFPS_READER;
}
//-------------------------------------------------------------------------------------//
CFingerprintWnd::~CFingerprintWnd()
{
	delete[] m_FingerFeature; m_FingerFeature = NULL;
}
//-------------------------------------------------------------------------------------//
void CFingerprintWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, FINGERPRINT_STATE_IMG, m_FingerPrintRegisterImg);
	DDX_Control(pDX, FINGERPRINT_TITLE_TEXT, m_CommandLabelStatic1);
	DDX_Control(pDX, FINGERPRINT_STATE_TEXT, m_StatusLabelStatic1); 
	DDX_Control(pDX, FINGERPRINT_PORT_COMBO_BOX, m_PortCombox);
}
//-------------------------------------------------------------------------------------//
BOOL CFingerprintWnd::OnInitDialog()
{
	CDialog::OnInitDialog();
	CString str;
	SwitchMultiLanguage();
	m_CancelEvent = false;
	m_ThreadIsClose = false;
	m_ReturnPasswordMode = false;
	str = LoadMultiLanguageString(m_WndText, m_WndText);
	if (false == LoadFPSINIFile()) { return FALSE; }
	if (false == SaveFPSINIFile()) { return FALSE; }
	switch (m_FingerPrintMode)
	{
		case(USER_FINGERPRINT_MODE_ENROLL):	m_WndText = "Finger Enroll (Best with thumb)";	break;
		case(USER_FINGERPRINT_MODE_MATCH):	m_WndText = "Finger Match";						break;
		case(USER_FINGERPRINT_MODE_FIND):	m_WndText = "Finger Login";						break;
		default:	break;
	}

	CWnd::SetWindowText(str);
	m_CommandLabelStatic1.SetWindowText(L"");
	m_StatusLabelStatic1.SetWindowText(L"");
	m_PortCombox.EnableWindow(TRUE);

	m_FPSDevicePtr = FingerprintDevice::CreateDevice(m_FPSDeviceType);
	if (NULL == m_FPSDevicePtr) { 
		m_ThreadIsClose = true; 
		return 0;
	}

	if(false == m_FPSDevicePtr->CheckFPSDeviceExisted()){
		AOIDataDefine.BuildRS232PortCombox(m_PortCombox);
		str = L"Fingerprint sensor connection failed.";
		str = LoadMultiLanguageString(str, str);
		m_CommandLabelStatic1.SetWindowText(str);
		str = L"Please select the proper port.";
		str = LoadMultiLanguageString(str, str);
		m_StatusLabelStatic1.SetWindowText(str);
		m_ThreadIsClose = true;
	}
	if (m_FingerPrintMode != USER_FINGERPRINT_MODE_ENROLL) {
		str = L"Password";
		m_PortCombox.AddString(str);
		m_PortCombox.SetItemData(m_PortCombox.GetCount(), 100);
	}
	if (false == m_ThreadIsClose) { AfxBeginThread(ExecuteThread, this); }
	return 0;
}
//-------------------------------------------------------------------------------------//
BOOL CFingerprintWnd::PreTranslateMessage(MSG * pMsg)
{
	switch (pMsg->message)
	{
	//---------------------------------------------------------------------------------//
	case(WM_KEYDOWN)://按下鍵盤
		switch (pMsg->wParam)
		{
		case(VK_RETURN)://攔截按下Enter，避免觸發OnOK離開視窗
			return true;
			break;
		default:
			break;
		}
		break;
	//---------------------------------------------------------------------------------//
	default:
		break;
	}
	return CDialog::PreTranslateMessage(pMsg);
}
//-------------------------------------------------------------------------------------//
void CFingerprintWnd::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section = _T("IDD_USER_FINGERPRINT_WND");
	//---------------------------------------------------------------------------------//
	WndID = IDD_USER_FINGERPRINT_WND;
	WndKey = _T("IDD_USER_FINGERPRINT_WND");
	this->GetWindowText(LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
	this->SetWindowText(NewLabelText);
	//---------------------------------------------------------------------------------//
	//WndID = IDOK;
	//NewLabelText = AOIDataDefine.GetWndOKText();
	//this->SetDlgItemText(WndID, NewLabelText);

	WndID = IDCANCEL;
	NewLabelText = AOIDataDefine.GetWndCancelText();
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//
}
//-------------------------------------------------------------------------------------//
CString CFingerprintWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section = _T("IDD_USER_FINGERPRINT_WND");
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
bool CFingerprintWnd::LoadINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pDefault, TCHAR * pString, int StringSize, LPCTSTR pfilename, bool IsCheckLens, CString & Error)
{
	if (JetAPI::LoadINIData(pSection, pKeyName, pDefault, pString, StringSize, pfilename, IsCheckLens, Error) == false)
	{	return false;	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CFingerprintWnd::SaveINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pString, LPCTSTR pfilename, CString & Error)
{
	if (JetAPI::SaveINIData(pSection, pKeyName, pString, pfilename, Error) == false)
	{	return false;	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CFingerprintWnd::LoadFPSINIFile()
{
	const size_t textlen = 128;
	CString Filename;
	CString Section = _T("FingerPrintDevice");
	CString KeyName = _T("");
	CString Default = _T("");
	TCHAR   String[textlen] = _T("");
	Filename = AOIDataDefine.GetFingerPrintConfigFilename();

	KeyName.Format(_T("FPS Device Type"));
	Default.Format(_T("%d"), m_FPSDeviceType);

	if (LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true)
	{	m_FPSDeviceType = (FPS_DEVICE)(JetAPI::StrToInt(String));	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CFingerprintWnd::SaveFPSINIFile()
{
	const size_t textlen = 128;
	CString Filename;
	CString Section = _T("FingerPrintDevice");
	CString KeyName = _T("");
	CString Default = _T("");
	CString String  = _T("");
	Filename = AOIDataDefine.GetFingerPrintConfigFilename();

	KeyName.Format(_T("FPS Device Type"));
	String.Format(_T("%d"), m_FPSDeviceType);

	if (SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false)
	{	return false;	}
	return true;
}
//-------------------------------------------------------------------------------------//
UINT CFingerprintWnd::ExecuteThread(LPVOID pParam)
{
	CFingerprintWnd* pThis = (CFingerprintWnd*)pParam;
	std::shared_ptr<Fingerprint_Base> FPSDevicePtr = pThis->m_FPSDevicePtr;
	pThis->m_CancelEvent = false;
	//---------------------------------------------------------------------------------//
	//連接
	//Fingerprint.SetCOMPort(pThis->nPort);
	pThis->PostMessage(MSG_FINGERPRINT_WND, WPARAM_FINGERPRINT_SHOW_STATE, LPARAM_FINGERPRINT_CONNECTING);
	if (false == FPSDevicePtr->ConnectDevice()) {
		pThis->m_ErrorString.Format(L"Can not connect to Fingerprint Device");
		pThis->PostMessage(MSG_FINGERPRINT_WND, WPARAM_FINGERPRINT_SHOW_STATE, LPARAM_FINGERPRINT_CONNECT_FAIL);
		pThis->m_ThreadIsClose = true;
		return 0;
	}
	else
		pThis->PostMessage(MSG_FINGERPRINT_WND, WPARAM_FINGERPRINT_SHOW_STATE, LPARAM_FINGERPRINT_WAITING);
	//---------------------------------------------------------------------------------//
	switch (pThis->m_FingerPrintMode)
	{
	case(USER_FINGERPRINT_MODE_MATCH):
		if (true == FPSDevicePtr->MatchFingerprint(pParam)) {
			pThis->PostMessage(MSG_FINGERPRINT_WND, WPARAM_FINGERPRINT_SHOW_STATE, LPARAM_FINGERPRINT_FINISH);
		}
		else {
			pThis->m_ErrorString = FPSDevicePtr->GetErrorString();
			pThis->PostMessage(MSG_FINGERPRINT_WND, WPARAM_FINGERPRINT_SHOW_STATE, LPARAM_FINGERPRINT_ERROR);
		}
		break;
	case(USER_FINGERPRINT_MODE_ENROLL):
		if (true == FPSDevicePtr->RecordFingerprint(pParam)) {
			DWORD FeatureSize = 0;
			FPSDevicePtr->GetBiometricIdentity(pThis->m_FingerFeature, FeatureSize);
			pThis->PostMessage(MSG_FINGERPRINT_WND, WPARAM_FINGERPRINT_SHOW_STATE, LPARAM_FINGERPRINT_FINISH);
		}
		else {
			pThis->m_ErrorString = FPSDevicePtr->GetErrorString();
			pThis->PostMessage(MSG_FINGERPRINT_WND, WPARAM_FINGERPRINT_SHOW_STATE, LPARAM_FINGERPRINT_ERROR);
		}
		break;
	case(USER_FINGERPRINT_MODE_FIND):
		if (true == FPSDevicePtr->FindFingerprint(pParam)) {
			pThis->PostMessage(MSG_FINGERPRINT_WND, WPARAM_FINGERPRINT_SHOW_STATE, LPARAM_FINGERPRINT_FINISH);
		}
		else {
			pThis->m_ErrorString = FPSDevicePtr->GetErrorString();
			pThis->PostMessage(MSG_FINGERPRINT_WND, WPARAM_FINGERPRINT_SHOW_STATE, LPARAM_FINGERPRINT_ERROR);
		}
		break;
	default:
		break;
	}
	pThis->m_ThreadIsClose = true;
	return 0;
}
//-------------------------------------------------------------------------------------//
LRESULT CFingerprintWnd::OnGetMessage(WPARAM wParam, LPARAM lParam)
{
	CString str;
	CString* pStr;
	CString CommandText, StatusText;
	if (wParam == WPARAM_FINGERPRINT_SHOW_OPERATION_STRING) {
		pStr = reinterpret_cast<CString*>(lParam);
		m_CommandLabelStatic1.SetWindowText(*pStr);
		delete pStr;
		return 0;
	}
	else if (wParam == WPARAM_FINGERPRINT_SHOW_STATE_STRING|| wParam == WPARAM_FINGERPRINT_SHOW_REJECT_STRING) {
		pStr = reinterpret_cast<CString*>(lParam);
		m_StatusLabelStatic1.SetWindowText(*pStr);
		delete pStr;
		return 0;
	}
	CommandText = AOIDataDefine.GetAS608CommandText((AS608_COMMAND)wParam);
	StatusText = AOIDataDefine.GetAS608StatusText((AS608_STATUS)lParam);
	CommandText = LoadMultiLanguageString(CommandText, CommandText);
	StatusText = LoadMultiLanguageString(StatusText, StatusText);

	if ((AS608_STATUS)lParam == AS608_STATUS_PORT_INVALID) {
		str = L"Port do not Response, please try others";

		str = LoadMultiLanguageString(str, str);
		m_CommandLabelStatic1.SetWindowText(str);

		str = L"Please select the proper port.";
		str = LoadMultiLanguageString(str, str);
		m_StatusLabelStatic1.SetWindowText(str);
		m_CancelEvent = true;

		AOIDataDefine.BuildRS232PortCombox(m_PortCombox);
		if (m_FingerPrintMode != USER_FINGERPRINT_MODE_ENROLL) {
			str = L"Password";
			int idx = m_PortCombox.GetCount();
			m_PortCombox.AddString(str);
			m_PortCombox.SetItemData(idx, 100);
		}

		m_FingerPrintRegisterImg.SetBitmap(m_LEDGray);
		while (!m_ThreadIsClose) {
			//確保Thread關閉
			Sleep(10);
		}
		return 0;
	}
	str.Format(L"%s: %s ", CommandText, StatusText);
	switch (wParam) {
	// wParam放AS608_COMMAND， lParam放AS608_STATUS
	//---------------------------------------------------------------------------------//
	case(WPARAM_FINGERPRINT_SHOW_STATE)://AS608_COMMAND沒有0，用來放其他例外項
		switch (lParam) {
		case(LPARAM_FINGERPRINT_CONNECT_FAIL)://裝置連接失敗
			m_FingerPrintRegisterImg.SetBitmap(m_LEDGray);
			str = L"Fingerprint sensor connection failed.";
			str = LoadMultiLanguageString(str, str);
			m_CommandLabelStatic1.SetWindowText(str);
			//str = LoadMultiLanguageString(m_ErrorString, m_ErrorString);
			//m_StatusLabelStatic1.SetWindowText(str);
			AOIDataDefine.BuildRS232PortCombox(m_PortCombox);
			if (m_FingerPrintMode != USER_FINGERPRINT_MODE_ENROLL) {
				str = L"Password";
				int idx = m_PortCombox.GetCount();
				m_PortCombox.AddString(str);
				m_PortCombox.SetItemData(idx, 100);
			}
			break;
		case(LPARAM_FINGERPRINT_CONNECTING)://裝置連接中
			str = L"Fingerprint sensor connecting...";
			m_CommandLabelStatic1.SetWindowText(str);
			break;
		case(LPARAM_FINGERPRINT_WAITING): m_FingerPrintRegisterImg.SetBitmap(m_LEDYellow); break;//裝置連接成功，尚未完成指令
		case(LPARAM_FINGERPRINT_FINISH)://完成，自動關閉
			m_FingerPrintRegisterImg.SetBitmap(m_LEDGreen);
			while (!m_ThreadIsClose) {
			//確保Thread關閉
				Sleep(10);
			}
			Sleep(500);
			EndDialog(IDOK);
			break;
		case(LPARAM_FINGERPRINT_EXIST)://
			m_ErrorString = _T("Fingerprint Existed");
			str = LoadMultiLanguageString(m_ErrorString, m_ErrorString);
			JetAPI::ShowMessageBox(m_ErrorString);
			//m_CommandLabelStatic1.SetWindowText(str);
			//str = L"";
			//m_StatusLabelStatic1.SetWindowText(str);
			break;
		case(LPARAM_FINGERPRINT_ERROR):
			JetAPI::ShowMessageBox(m_ErrorString);
			m_FingerPrintRegisterImg.SetBitmap(m_LEDGray);
			str = L"Fingerprint sensor connection failed.";
			str = LoadMultiLanguageString(str, str);
			m_CommandLabelStatic1.SetWindowText(str);
			//str = LoadMultiLanguageString(m_ErrorString, m_ErrorString);
			//m_StatusLabelStatic1.SetWindowText(str);
			m_StatusLabelStatic1.SetWindowText(_T(""));
			AOIDataDefine.BuildRS232PortCombox(m_PortCombox);
			if (m_FingerPrintMode != USER_FINGERPRINT_MODE_ENROLL) {
				str = L"Password";
				int idx = m_PortCombox.GetCount();
				m_PortCombox.AddString(str);
				m_PortCombox.SetItemData(idx, 100);
			}
			break;
		default:
			break;
		}
		break;
	//---------------------------------------------------------------------------------//
	case(AS608_COMMAND_GETIMAGE)://只有這一項會要求使用者動作，其他都是處理過程
		str.Format(L"%s ", StatusText);
		m_CommandLabelStatic1.SetWindowText(str);
		str.Format(L"%s ", CommandText);
		m_StatusLabelStatic1.SetWindowText(str);
		break;
	case(AS608_COMMAND_UPCHAR)://從AS608上傳至電腦
		switch (lParam) {
		case(AS608_STATUS_OK):
			m_FingerPrintRegisterImg.SetBitmap(m_LEDGreen);
			break;
		}
		str.Format(L"%s: %s ", CommandText, StatusText);
		m_StatusLabelStatic1.SetWindowText(str);
		break;
	case(AS608_COMMAND_MATCH)://驗證是否是同一指紋
		switch (lParam) {
		case(AS608_STATUS_OK):
			str = L"Successed";
			str.Format(L"%s: %s ", CommandText, str);
			str = LoadMultiLanguageString(str, str);
			break;
		default:
			str = L"";
			str.Format(L"%s: %s ", CommandText, str);
			str = LoadMultiLanguageString(str, str);
		}
		str.Format(L"%s: %s ", CommandText, StatusText);
		m_StatusLabelStatic1.SetWindowText(str);
		break;
	default:
		str.Format(L"%s: %s ", CommandText, StatusText);
		m_StatusLabelStatic1.SetWindowText(str);
		break;
	}
	return 0;
}
//-------------------------------------------------------------------------------------//
void CFingerprintWnd::OnCancel()
{
	//傳遞取消事件
	m_CancelEvent = true;
	while (!m_ThreadIsClose) {
		//等待Thread被關閉
		Sleep(10);
	}
	CDialog::OnCancel();
}

//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CFingerprintWnd, CDialog)
	ON_MESSAGE(MSG_FINGERPRINT_WND, &CFingerprintWnd::OnGetMessage)
	ON_CBN_SELCHANGE(FINGERPRINT_PORT_COMBO_BOX, &CFingerprintWnd::OnCbnSelchangePortComboBox)
END_MESSAGE_MAP()

//-------------------------------------------------------------------------------------//
// CFingerprintWnd 訊息處理常式
void CFingerprintWnd::SetFingerPrintMode(USER_FINGERPRINT_MODE bMode)
{
	m_FingerPrintMode = bMode;
}
//-------------------------------------------------------------------------------------//
void CFingerprintWnd::SetLoginFingerPrint(BYTE * FingerFeature)
{
	memcpy(m_FingerFeature, FingerFeature, m_FingerFeatureSize);
}
//-------------------------------------------------------------------------------------//
void CFingerprintWnd::GetEnrollFingerPrint(BYTE* FingerFeature)
{
	memcpy(FingerFeature, m_FingerFeature, m_FingerFeatureSize);
}
//-------------------------------------------------------------------------------------//
bool CFingerprintWnd::CheckWndIsCancel()
{
	return m_CancelEvent;
}
//-------------------------------------------------------------------------------------//
bool CFingerprintWnd::DeleteFingerprintInDB(std::vector<TUserNode> UserList)
{
	if (UserList.size() == 0) { return true; }
	if (false == LoadFPSINIFile()) { return false; }
	if (m_FPSDeviceType == FPS_DEVICE_AS608) { return true; }
	std::shared_ptr<Fingerprint_Base> FPSDevicePtr = FingerprintDevice::CreateDevice(m_FPSDeviceType);
	//if (false == FPSDevicePtr->ConnectDevice()) { return false; }
	if (false == FPSDevicePtr->DelFingerprintInDB(UserList)) { return false; }
	

	return true;
}
//-------------------------------------------------------------------------------------//

void CFingerprintWnd::OnCbnSelchangePortComboBox()
{
	CString	str;
	CString PortText = JetAPI::GetComboxCurSelText(m_PortCombox);
	if (PortText == L"Password") {
		m_ReturnPasswordMode = true;
		m_CancelEvent = true;
		while (false == m_ThreadIsClose) {
			//確保Thread關閉
			Sleep(10);
		}
		EndDialog(IDOK);
		return;
	}
	else if ((PortText == L"")) {
		return;
	}
	int ComboxIndex = JetAPI::GetComboxCurSelData(m_PortCombox);
	//str.Format(L"Do you want to set port to [%s] ?", PortText);
	str = L"Do you want to set port to ";
	str = LoadMultiLanguageString(str, str);
	str.Format(L"%s [%s] ?", str, PortText);

	DWORD Res = JetAPI::ShowMessageBox(str, MB_YESNO);
	if (Res == IDNO) { return; }

	str = L"Waiting for response";
	str = LoadMultiLanguageString(str, str);
	m_CommandLabelStatic1.SetWindowText(str);
	m_StatusLabelStatic1.SetWindowText(L"");
	m_ThreadIsClose = false;

	int Length = PortText.GetLength();
	int Begin = PortText.Find(L"COM") + 3;
	str = PortText.Right(Length-Begin);
	nPort = ::_ttoi(str);
	JetAPI::SetComboxCurSel(m_PortCombox, ComboxIndex);
	//m_PortCombox.EnableWindow(false);
	AfxBeginThread(ExecuteThread, this);
	return;
}
//-------------------------------------------------------------------------------------//