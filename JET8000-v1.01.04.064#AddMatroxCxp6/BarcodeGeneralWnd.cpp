// BarcodeGeneralWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "BarcodeGeneralWnd.h"
//-------------------------------------------------------------------------------------//
#include "InputBoxWnd.h"
#include "InputListWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CBarcodeGeneralWnd dialog
//-------------------------------------------------------------------------------------//
CBarcodeGeneralWnd::CBarcodeGeneralWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CBarcodeGeneralWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBarcodeGeneralWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_BarcodePtr = NULL;
}
//-------------------------------------------------------------------------------------//
void CBarcodeGeneralWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBarcodeGeneralWnd)
	DDX_Control(pDX, BARGEN_PORT_COMBO, m_PortCombox);
	DDX_Control(pDX, BARGEN_BAUD_COMBO, m_BaudCombox);
	DDX_Control(pDX, BARGEN_PARITY_COMBO, m_ParityCombox);
	DDX_Control(pDX, BARGEN_STOP_BITS_COMBO, m_StopBitsCombox);	
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CBarcodeGeneralWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CBarcodeGeneralWnd)
	ON_WM_DESTROY()
	ON_BN_CLICKED(BARGEN_CONNECT_CHK, OnConnectChk)
	ON_BN_CLICKED(BARGEN_START_BTN, OnStartBtn)
	ON_BN_CLICKED(BARGEN_START_CLEAR_BTN, OnStartClearBtn)
	ON_BN_CLICKED(BARGEN_START_ADD_CHAR_BTN, OnStartAddCharBtn)
	ON_BN_CLICKED(BARGEN_START_ADD_STRING_BTN, OnStartAddStringBtn)
	ON_BN_CLICKED(BARGEN_STOP_BTN, OnStopBtn)
	ON_BN_CLICKED(BARGEN_STOP_CLEAR_BTN, OnStopClearBtn)
	ON_BN_CLICKED(BARGEN_STOP_ADD_CHAR_BTN, OnStopAddCharBtn)
	ON_BN_CLICKED(BARGEN_STOP_ADD_STRING_BTN, OnStopAddStringBtn)
	ON_BN_CLICKED(BARGEN_READ_BTN, OnReadBtn)
	ON_BN_CLICKED(BARGEN_CODE_SEPARATOR_CLEAR, OnCodeSeparatorClearBtn)
	ON_BN_CLICKED(BARGEN_CODE_SEPARATOR_ADD_CHAR_BTN, OnCodeSeparatorAddCharBtn)
	ON_BN_CLICKED(BARGEN_CODE_SEPARATOR_ADD_STRING_BTN, OnCodeSeparatorAddStringBtn)
	ON_BN_CLICKED(BARGEN_CODE_PREFIX_CLEAR, OnCodePrefixClearBtn)
	ON_BN_CLICKED(BARGEN_CODE_PREFIX_ADD_CHAR_BTN, OnCodePrefixAddCharBtn)
	ON_BN_CLICKED(BARGEN_CODE_PREFIX_ADD_STRING_BTN, OnCodePrefixAddStringBtn)
	ON_BN_CLICKED(BARGEN_CODE_SUFFIX_CLEAR, OnCodeSuffixClearBtn)
	ON_BN_CLICKED(BARGEN_CODE_SUFFIX_ADD_CHAR_BTN, OnCodeSuffixAddCharBtn)
	ON_BN_CLICKED(BARGEN_CODE_SUFFIX_ADD_STRING_BTN, OnCodeSuffixAddStringBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CBarcodeGeneralWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CBarcodeGeneralWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here	
	SwitchMultiLanguage();	
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	AOIDataDefine.BuildRS232PortCombox(m_PortCombox);
	AOIDataDefine.BuildRS232BaudCombox(m_BaudCombox);
	AOIDataDefine.BuildRS232ParityCombox(m_ParityCombox);
	AOIDataDefine.BuildRS232StopBitsCombox(m_StopBitsCombox);	

	CBarcode_General *BarcodePtr=GetBarcodePtr();
	UpdateToBarcodeDeviceUI(BarcodePtr);
	UpdateToBarcodeDeviceUIState(BarcodePtr);	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CBarcodeGeneralWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CBarcodeGeneralWnd::SetBarcodePtr(CBarcode_General *BarcodePtr)
{
	m_BarcodePtr = BarcodePtr;
}
//-------------------------------------------------------------------------------------//
CBarcode_General* CBarcodeGeneralWnd::GetBarcodePtr()
{
	return m_BarcodePtr;
}
//-------------------------------------------------------------------------------------//
bool CBarcodeGeneralWnd::CheckBarcodePtr(CBarcode_General *Ptr)
{
	if ( NULL == Ptr )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
void CBarcodeGeneralWnd::SwitchMultiLanguage()
{
	//---------------------------------------------------------------------------------//	
	SetMultiLanauage(LoadIDAndName(IDD_BARCODE_GENERAL_WND), false);
	//---------------------------------------------------------------------------------//	
	SetMultiLanauage(LoadIDAndName(IDOK));
	SetMultiLanauage(LoadIDAndName(IDCANCEL));
	//---------------------------------------------------------------------------------//	
	SetMultiLanauage(LoadIDAndName(BARGEN_CONNECT_CHK));
	SetMultiLanauage(LoadIDAndName(BARGEN_PORT_LABEL));
	SetMultiLanauage(LoadIDAndName(BARGEN_PARITY_LABEL));
	SetMultiLanauage(LoadIDAndName(BARGEN_STOP_BITS_LABEL));
	SetMultiLanauage(LoadIDAndName(BARGEN_BAUD_LABEL));	
	//---------------------------------------------------------------------------------//
	SetMultiLanauage(LoadIDAndName(BARGEN_START_LABEL));
	SetMultiLanauage(LoadIDAndName(BARGEN_START_BTN));
	SetMultiLanauage(LoadIDAndName(BARGEN_START_CLEAR_BTN));
	SetMultiLanauage(LoadIDAndName(BARGEN_START_ADD_CHAR_BTN));
	SetMultiLanauage(LoadIDAndName(BARGEN_START_ADD_STRING_BTN));

	SetMultiLanauage(LoadIDAndName(BARGEN_STOP_LABEL));
	SetMultiLanauage(LoadIDAndName(BARGEN_STOP_BTN));
	SetMultiLanauage(LoadIDAndName(BARGEN_STOP_CLEAR_BTN));
	SetMultiLanauage(LoadIDAndName(BARGEN_STOP_ADD_CHAR_BTN));
	SetMultiLanauage(LoadIDAndName(BARGEN_STOP_ADD_STRING_BTN));

	SetMultiLanauage(LoadIDAndName(BARGEN_READ_BTN));

	SetMultiLanauage(LoadIDAndName(BARGEN_CODE_GROUP));
	SetMultiLanauage(LoadIDAndName(BARGEN_CODE_SEPARATOR_LABEL));	
	SetMultiLanauage(LoadIDAndName(BARGEN_CODE_SEPARATOR_CLEAR));
	SetMultiLanauage(LoadIDAndName(BARGEN_CODE_SEPARATOR_ADD_CHAR_BTN));
	SetMultiLanauage(LoadIDAndName(BARGEN_CODE_SEPARATOR_ADD_STRING_BTN));

	SetMultiLanauage(LoadIDAndName(BARGEN_CODE_PREFIX_LABEL));	
	SetMultiLanauage(LoadIDAndName(BARGEN_CODE_PREFIX_CLEAR));
	SetMultiLanauage(LoadIDAndName(BARGEN_CODE_PREFIX_ADD_CHAR_BTN));
	SetMultiLanauage(LoadIDAndName(BARGEN_CODE_PREFIX_ADD_STRING_BTN));

	SetMultiLanauage(LoadIDAndName(BARGEN_CODE_SUFFIX_LABEL));	
	SetMultiLanauage(LoadIDAndName(BARGEN_CODE_SUFFIX_CLEAR));
	SetMultiLanauage(LoadIDAndName(BARGEN_CODE_SUFFIX_ADD_CHAR_BTN));
	SetMultiLanauage(LoadIDAndName(BARGEN_CODE_SUFFIX_ADD_STRING_BTN));
	//---------------------------------------------------------------------------------//	
}
//-------------------------------------------------------------------------------------//
bool CBarcodeGeneralWnd::SetMultiLanauage(UINT ID, LPCTSTR Text, bool bCtrlID)
{
	LPCTSTR Section=_T("IDD_BARCODE_GENERAL_WND");		
	if ( AOIDataCollect.SwitchMultiLanguageWnd(this, Section, ID, Text, bCtrlID) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
CString CBarcodeGeneralWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_BARCODE_GENERAL_WND");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
void CBarcodeGeneralWnd::OnOK() 
{
	// TODO: Add extra validation here	
	const bool bSave=true;
	CBarcode_General *BarcodePtr=GetBarcodePtr();
	if ( UpdateToBarcodeDevice(BarcodePtr, bSave) == false ) { return; }	
	CBaseDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CBarcodeGeneralWnd::OnConnectChk() 
{
	// TODO: Add your control notification handler code here
	CBarcode_General *BarcodePtr=GetBarcodePtr();
	if ( CheckBarcodePtr(BarcodePtr) == false ) { return; }
	BOOL bChk=CWnd::IsDlgButtonChecked(BARGEN_CONNECT_CHK);
	if ( FALSE == bChk )
	{	BarcodePtr->Disconnected();	}
	else
	{	
		CString str;
		const bool bSave=true;	
		UpdateToBarcodeDevice(BarcodePtr, bSave);
		const bool bSucc=BarcodePtr->ConnectToDevice();
		if ( true == bSucc )
		{	str = AOIDataDefine.GetFinishText();	}
		else
		{	str = BarcodePtr->GetErrorString(); }
		JetAPI::ShowMessageBox(str);
	}
	UpdateToBarcodeDeviceUIState(BarcodePtr);	
}
//-------------------------------------------------------------------------------------//
bool CBarcodeGeneralWnd::UpdateToBarcodeDeviceUI(CBarcode_General *BarcodePtr)
{
	if ( CheckBarcodePtr(BarcodePtr) == false ) { return false; }

	CString str;
	const int nPort=::_ttoi(BarcodePtr->GetBarcodeDevicePort());
	JetAPI::SetComboxCurSel(m_PortCombox, nPort);
	JetAPI::SetComboxCurSel(m_BaudCombox, BarcodePtr->GetBarcodeDeviceBaudRate());
	JetAPI::SetComboxCurSel(m_ParityCombox, BarcodePtr->GetBarcodeDeviceParity());
	JetAPI::SetComboxCurSel(m_StopBitsCombox, BarcodePtr->GetBarcodeDeviceStopBits());

	CWnd::CheckDlgButton(BARGEN_CONNECT_CHK, BarcodePtr->CheckConnected());

	str = BarcodePtr->GetHexTriggerOn();
	SetBarcodeStartEditText(str);
		
	str = BarcodePtr->GetHexTriggerOff();
	SetBarcodeStopEditText(str);

	str = BarcodePtr->GetHexCodeSeparator();
	SetBarcodeSeparatorEditText(str);

	str = BarcodePtr->GetHexCodePrefix();
	SetBarcodeCodePrefixEditText(str);

	str = BarcodePtr->GetHexCodeSuffix();
	SetBarcodeCodeSuffixEditText(str);

	SetBarcodeReadEditText(_T(""));	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcodeGeneralWnd::UpdateToBarcodeDeviceUIState(CBarcode_General *BarcodePtr)
{
	if ( CheckBarcodePtr(BarcodePtr) == false ) { return false; }
	BOOL bEnable=TRUE;
	const bool bConnected=BarcodePtr->CheckConnected();
	if ( false == bConnected )
	{	bEnable=TRUE; }
	else
	{	bEnable=FALSE; }
	JetAPI::EnableCtrlWnd(this, BARGEN_PORT_COMBO, bEnable);
	JetAPI::EnableCtrlWnd(this, BARGEN_PARITY_COMBO, bEnable);
	JetAPI::EnableCtrlWnd(this, BARGEN_STOP_BITS_COMBO, bEnable);
	JetAPI::EnableCtrlWnd(this, BARGEN_BAUD_COMBO, bEnable);

	bEnable = bConnected;
	JetAPI::EnableCtrlWnd(this, BARGEN_START_BTN, bEnable);
	JetAPI::EnableCtrlWnd(this, BARGEN_START_CLEAR_BTN, bEnable);
	JetAPI::EnableCtrlWnd(this, BARGEN_START_ADD_CHAR_BTN, bEnable);
	JetAPI::EnableCtrlWnd(this, BARGEN_START_ADD_STRING_BTN, bEnable);

	JetAPI::EnableCtrlWnd(this, BARGEN_STOP_BTN, bEnable);
	JetAPI::EnableCtrlWnd(this, BARGEN_STOP_CLEAR_BTN, bEnable);
	JetAPI::EnableCtrlWnd(this, BARGEN_STOP_ADD_CHAR_BTN, bEnable);
	JetAPI::EnableCtrlWnd(this, BARGEN_STOP_ADD_STRING_BTN, bEnable);

	JetAPI::EnableCtrlWnd(this, BARGEN_READ_BTN, bEnable);

	JetAPI::EnableCtrlWnd(this, BARGEN_CODE_SEPARATOR_CLEAR, bEnable);
	JetAPI::EnableCtrlWnd(this, BARGEN_CODE_SEPARATOR_ADD_CHAR_BTN, bEnable);
	JetAPI::EnableCtrlWnd(this, BARGEN_CODE_SEPARATOR_ADD_STRING_BTN, bEnable);

	JetAPI::EnableCtrlWnd(this, BARGEN_CODE_PREFIX_CLEAR, bEnable);
	JetAPI::EnableCtrlWnd(this, BARGEN_CODE_PREFIX_ADD_CHAR_BTN, bEnable);
	JetAPI::EnableCtrlWnd(this, BARGEN_CODE_PREFIX_ADD_STRING_BTN, bEnable);

	JetAPI::EnableCtrlWnd(this, BARGEN_CODE_SUFFIX_CLEAR, bEnable);
	JetAPI::EnableCtrlWnd(this, BARGEN_CODE_SUFFIX_ADD_CHAR_BTN, bEnable);
	JetAPI::EnableCtrlWnd(this, BARGEN_CODE_SUFFIX_ADD_STRING_BTN, bEnable);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcodeGeneralWnd::UpdateToBarcodeDevice(CBarcode_General *BarcodePtr, bool bSave)
{
	if ( CheckBarcodePtr(BarcodePtr) == false ) { return false; }
	CString str;
	std::string stdstr;
	const int nBaud=JetAPI::GetComboxCurSelData(m_BaudCombox);
	const int nParity=JetAPI::GetComboxCurSelData(m_ParityCombox);
	const int nStopBits=JetAPI::GetComboxCurSelData(m_StopBitsCombox);	

	BarcodePtr->SetBarcodeDeviceBaudRate(nBaud);
	BarcodePtr->SetBarcodeDeviceParity(nParity);
	BarcodePtr->SetBarcodeDeviceStopBits(nStopBits);

	CWnd::GetDlgItemText(BARGEN_START_EDIT, str);
	JetAPI::TCHAR2string(str, stdstr);
	BarcodePtr->SetHexTriggerOn(stdstr.c_str());
	
	CWnd::GetDlgItemText(BARGEN_STOP_EDIT, str);		
	JetAPI::TCHAR2string(str, stdstr);
	BarcodePtr->SetHexTriggerOff(stdstr.c_str());

	CWnd::GetDlgItemText(BARGEN_CODE_SEPARATOR_EDIT, str);		
	JetAPI::TCHAR2string(str, stdstr);
	BarcodePtr->SetHexCodeSeparator(stdstr.c_str());

	CWnd::GetDlgItemText(BARGEN_CODE_PREFIX_EDIT, str);		
	JetAPI::TCHAR2string(str, stdstr);
	BarcodePtr->SetHexCodePrefix(stdstr.c_str());

	CWnd::GetDlgItemText(BARGEN_CODE_SUFFIX_EDIT, str);		
	JetAPI::TCHAR2string(str, stdstr);
	BarcodePtr->SetHexCodeSuffix(stdstr.c_str());

	SetBarcodeReadEditText(_T(""));	

	if ( true == bSave )
	{	BarcodePtr->SaveBarcodeINIFile();	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcodeGeneralWnd::SetBarcodeStartEditText(LPCTSTR Text)
{
	CWnd::SetDlgItemText(BARGEN_START_EDIT, Text);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcodeGeneralWnd::SetBarcodeStopEditText(LPCTSTR Text)
{
	CWnd::SetDlgItemText(BARGEN_STOP_EDIT, Text);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcodeGeneralWnd::SetBarcodeReadEditText(LPCTSTR Text)
{
	CWnd::SetDlgItemText(BARGEN_READ_EDIT, Text);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcodeGeneralWnd::SetBarcodeSeparatorEditText(LPCTSTR Text)
{
	CWnd::SetDlgItemText(BARGEN_CODE_SEPARATOR_EDIT, Text);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcodeGeneralWnd::SetBarcodeCodePrefixEditText(LPCTSTR Text)
{
	CWnd::SetDlgItemText(BARGEN_CODE_PREFIX_EDIT, Text);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcodeGeneralWnd::SetBarcodeCodeSuffixEditText(LPCTSTR Text)
{
	CWnd::SetDlgItemText(BARGEN_CODE_SUFFIX_EDIT, Text);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcodeGeneralWnd::ExecAddChar(LPCTSTR Fnuc, UINT CtrlID)
{
	CString         str;
	TListNode       Node;	
	CInputListWnd   EnumWnd;
	DWORD_PTR       OldIndex=0;
	CString         strCaption, strLabel;	
	std::vector<TListNode> NodelList;
	const std::map<int, std::string> &AsciiTable=AOIDataCollect.GetAsciiTable();
	
	for ( int i=1; i<256; i++ )
	{
		Node.Data = i;
		str.Format(_T("%c"), (char)(i));		
		auto iter = AsciiTable.find(i);
		if ( iter != AsciiTable.end() )
		{	str = iter->second.c_str();	}
		int len=str.GetLength();
		switch ( len )
		{
		case 0: str += "    "; break;
		case 1: str += "   "; break;
		case 2: str += "  "; break;
		case 3: str += " "; break;
		}
		Node.Text.Format(_T("%s, [0x%02X] = [%03d]"), str, i, i);
		//Node.Text.Format(_T("[0x%02X] = [%03d] = [%s]"), i, i, str);
		NodelList.push_back(Node);		
	}	
	
	strLabel = _T("Hex ASCII");
	strCaption = CString(Fnuc)+CString(_T(" Hex ASCII"));
	EnumWnd.SetParam1(strCaption, strLabel, OldIndex, NodelList);
	if ( EnumWnd.GetSelIndex1() < 0 ) 
	{	EnumWnd.SetSelIndex1(0); }
	if ( EnumWnd.DoModal() == IDCANCEL )
	{	return false; }
	
	CString Text, Sel;	
	const int Data=EnumWnd.GetSelData();
	CWnd::GetDlgItemText(CtrlID, Text);
	Sel.Format(_T("%02X"), Data);
	str = Text+Sel;
	CWnd::SetDlgItemText(CtrlID, str);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcodeGeneralWnd::ExecAddString(LPCTSTR Fnuc, UINT CtrlID)
{
	CString      str;
	CInputBoxWnd InputBox;
	CString strCaption, strLabel, strValue;	
	strLabel = _T("String");
	strCaption = CString(Fnuc)+CString(_T(" String"));
	InputBox.SetParam1(strCaption, strLabel, strValue);
	if ( InputBox.DoModal() == IDCANCEL ) 
	{	return false;	}

	int nVal=0;	
	strValue = InputBox.m_DataEdit1;		
	if ( 0 == strValue.GetLength() )
	{	return false; }

	std::string sstr;
	JetAPI::TCHAR2string(strValue, sstr);
	const size_t len=sstr.length();

	CString Text, Sel;	
	CWnd::GetDlgItemText(CtrlID, Text);
	for ( size_t i=0; i<len; i++ )
	{
		nVal = (unsigned char)(sstr[i]);
		Sel.Format(_T("%02X"), nVal);
		str = Text+Sel;
		Text= str;
	}
	CWnd::SetDlgItemText(CtrlID, str);	
	return true;
}
//-------------------------------------------------------------------------------------//
void CBarcodeGeneralWnd::OnStartBtn() 
{
	// TODO: Add your control notification handler code here
	CString str;
	CBarcode_General *BarcodePtr=GetBarcodePtr();
	if ( UpdateToBarcodeDevice(BarcodePtr) == false ) { return; }		
	if ( BarcodePtr->StartToRead() == false )
	{
		str = BarcodePtr->GetErrorString();
		JetAPI::ShowMessageBox(str);
	}
}
//-------------------------------------------------------------------------------------//
void CBarcodeGeneralWnd::OnStartClearBtn()
{
	// TODO: Add your control notification handler code here	
	CBarcode_General *BarcodePtr=GetBarcodePtr();
	if ( CheckBarcodePtr(BarcodePtr) == false ) { return ; }	
	BarcodePtr->SetHexTriggerOn("");
	SetBarcodeStartEditText(_T(""));	
	return;
}
//-------------------------------------------------------------------------------------//		
void CBarcodeGeneralWnd::OnStartAddCharBtn()
{
	// TODO: Add your control notification handler code here	
	CBarcode_General *BarcodePtr=GetBarcodePtr();
	if ( CheckBarcodePtr(BarcodePtr) == false ) { return ; }		
	
	const UINT CtrlID=BARGEN_START_EDIT;
	if ( ExecAddChar(_T("Trigger On"), CtrlID) == false )
	{	return; }

	CString str;
	std::string sBuf;
	CWnd::GetDlgItemText(CtrlID, str);	
	JetAPI::TCHAR2string(str, sBuf);
	BarcodePtr->SetHexTriggerOn(sBuf.c_str());
	return ;	
}
//-------------------------------------------------------------------------------------//
void CBarcodeGeneralWnd::OnStartAddStringBtn()
{
	// TODO: Add your control notification handler code here	
	CBarcode_General *BarcodePtr=GetBarcodePtr();
	if ( CheckBarcodePtr(BarcodePtr) == false ) { return ; }		
	
	const UINT CtrlID=BARGEN_START_EDIT;	
	if ( ExecAddString(_T("Trigger On"),CtrlID ) == false )
	{	return; }

	CString str;
	std::string sBuf;
	CWnd::GetDlgItemText(CtrlID, str);
	JetAPI::TCHAR2string(str, sBuf);
	BarcodePtr->SetHexTriggerOn(sBuf.c_str());
	return ;	
}
//-------------------------------------------------------------------------------------//
void CBarcodeGeneralWnd::OnStopBtn() 
{
	// TODO: Add your control notification handler code here
	CString str;
	CBarcode_General *BarcodePtr=GetBarcodePtr();
	if ( UpdateToBarcodeDevice(BarcodePtr) == false ) { return; }
	if ( BarcodePtr->EndReading() == false )
	{
		str = BarcodePtr->GetErrorString();
		JetAPI::ShowMessageBox(str);
	}
}
//-------------------------------------------------------------------------------------//
void CBarcodeGeneralWnd::OnStopClearBtn()
{
	// TODO: Add your control notification handler code here
	CBarcode_General *BarcodePtr=GetBarcodePtr();
	if ( CheckBarcodePtr(BarcodePtr) == false ) { return ; }	
	BarcodePtr->SetHexTriggerOff("");
	SetBarcodeStopEditText(_T(""));	
	return;
}
//-------------------------------------------------------------------------------------//	
void CBarcodeGeneralWnd::OnStopAddCharBtn()
{
	// TODO: Add your control notification handler code here	
	CBarcode_General *BarcodePtr=GetBarcodePtr();
	if ( CheckBarcodePtr(BarcodePtr) == false ) { return ; }

	const UINT CtrlID=BARGEN_STOP_EDIT;
	if ( ExecAddChar(_T("Trigger Off"), CtrlID) == false )
	{	return; }

	CString str;
	std::string sBuf;
	CWnd::GetDlgItemText(CtrlID, str);	
	JetAPI::TCHAR2string(str, sBuf);
	BarcodePtr->SetHexTriggerOff(sBuf.c_str());
	return ;
}
//-------------------------------------------------------------------------------------//
void CBarcodeGeneralWnd::OnStopAddStringBtn()
{
	// TODO: Add your control notification handler code here
	CBarcode_General *BarcodePtr=GetBarcodePtr();
	if ( CheckBarcodePtr(BarcodePtr) == false ) { return ; }		
	
	const UINT CtrlID=BARGEN_STOP_EDIT;	
	if ( ExecAddString(_T("Trigger Off"),CtrlID ) == false )
	{	return; }

	CString str;
	std::string sBuf;
	CWnd::GetDlgItemText(CtrlID, str);
	JetAPI::TCHAR2string(str, sBuf);
	BarcodePtr->SetHexTriggerOff(sBuf.c_str());
	return ;	
}
//-------------------------------------------------------------------------------------//
void CBarcodeGeneralWnd::OnReadBtn()
{
	// TODO: Add your control notification handler code here
	CString str;
	CBarcode_General *BarcodePtr=GetBarcodePtr();
	if ( UpdateToBarcodeDevice(BarcodePtr) == false ) { return; }
	if ( BarcodePtr->RetrieveCode() == false )
	{
		str = BarcodePtr->GetErrorString();
		JetAPI::ShowMessageBox(str);
	}
	else
	{		
		str = BarcodePtr->GetResultBuffer();
		CWnd::SetDlgItemText(BARGEN_READ_EDIT, str);
	}
}
//-------------------------------------------------------------------------------------//
void CBarcodeGeneralWnd::OnCodeSeparatorClearBtn()
{
	// TODO: Add your control notification handler code here
	CBarcode_General *BarcodePtr=GetBarcodePtr();
	if ( CheckBarcodePtr(BarcodePtr) == false ) { return ; }	
	BarcodePtr->SetHexCodeSeparator("");
	SetBarcodeSeparatorEditText(_T(""));	
	return;
}
//-------------------------------------------------------------------------------------//
void CBarcodeGeneralWnd::OnCodeSeparatorAddCharBtn()
{
	// TODO: Add your control notification handler code here
	CBarcode_General *BarcodePtr=GetBarcodePtr();
	if ( CheckBarcodePtr(BarcodePtr) == false ) { return ; }		
	
	const UINT CtrlID=BARGEN_CODE_SEPARATOR_EDIT;
	if ( ExecAddChar(_T("Separator"), CtrlID) == false )
	{	return; }

	CString str;
	std::string sBuf;
	CWnd::GetDlgItemText(CtrlID, str);	
	JetAPI::TCHAR2string(str, sBuf);
	BarcodePtr->SetHexCodeSeparator(sBuf.c_str());
	return ;	
}
//-------------------------------------------------------------------------------------//
void CBarcodeGeneralWnd::OnCodeSeparatorAddStringBtn()
{
	// TODO: Add your control notification handler code here
	CBarcode_General *BarcodePtr=GetBarcodePtr();
	if ( CheckBarcodePtr(BarcodePtr) == false ) { return ; }		
	
	const UINT CtrlID=BARGEN_CODE_SEPARATOR_EDIT;	
	if ( ExecAddString(_T("Separator"),CtrlID ) == false )
	{	return; }

	CString str;
	std::string sBuf;
	CWnd::GetDlgItemText(CtrlID, str);
	JetAPI::TCHAR2string(str, sBuf);
	BarcodePtr->SetHexCodeSeparator(sBuf.c_str());
	return ;
}
//-------------------------------------------------------------------------------------//
void CBarcodeGeneralWnd::OnCodePrefixClearBtn()
{
	// TODO: Add your control notification handler code here
	CBarcode_General *BarcodePtr=GetBarcodePtr();
	if ( CheckBarcodePtr(BarcodePtr) == false ) { return ; }	
	BarcodePtr->SetHexCodePrefix("");
	SetBarcodeCodePrefixEditText(_T(""));	
	return;
}
//-------------------------------------------------------------------------------------//
void CBarcodeGeneralWnd::OnCodePrefixAddCharBtn()
{
	// TODO: Add your control notification handler code here
	CBarcode_General *BarcodePtr=GetBarcodePtr();
	if ( CheckBarcodePtr(BarcodePtr) == false ) { return ; }		
	
	const UINT CtrlID=BARGEN_CODE_PREFIX_EDIT;
	if ( ExecAddChar(_T("Prefix"), CtrlID) == false )
	{	return; }

	CString str;
	std::string sBuf;
	CWnd::GetDlgItemText(CtrlID, str);	
	JetAPI::TCHAR2string(str, sBuf);
	BarcodePtr->SetHexCodePrefix(sBuf.c_str());
	return ;	
}
//-------------------------------------------------------------------------------------//
void CBarcodeGeneralWnd::OnCodePrefixAddStringBtn()
{
	// TODO: Add your control notification handler code here
	CBarcode_General *BarcodePtr=GetBarcodePtr();
	if ( CheckBarcodePtr(BarcodePtr) == false ) { return ; }		
	
	const UINT CtrlID=BARGEN_CODE_PREFIX_EDIT;	
	if ( ExecAddString(_T("Prefix"),CtrlID ) == false )
	{	return; }

	CString str;
	std::string sBuf;
	CWnd::GetDlgItemText(CtrlID, str);
	JetAPI::TCHAR2string(str, sBuf);
	BarcodePtr->SetHexCodePrefix(sBuf.c_str());
	return ;
}
//-------------------------------------------------------------------------------------//
void CBarcodeGeneralWnd::OnCodeSuffixClearBtn()
{
	// TODO: Add your control notification handler code here
	CBarcode_General *BarcodePtr=GetBarcodePtr();
	if ( CheckBarcodePtr(BarcodePtr) == false ) { return ; }	
	BarcodePtr->SetHexCodeSuffix("");
	SetBarcodeCodeSuffixEditText(_T(""));	
	return;
}
//-------------------------------------------------------------------------------------//
void CBarcodeGeneralWnd::OnCodeSuffixAddCharBtn()
{
	// TODO: Add your control notification handler code here
	CBarcode_General *BarcodePtr=GetBarcodePtr();
	if ( CheckBarcodePtr(BarcodePtr) == false ) { return ; }		
	
	const UINT CtrlID=BARGEN_CODE_SUFFIX_EDIT;
	if ( ExecAddChar(_T("Suffix"), CtrlID) == false )
	{	return; }

	CString str;
	std::string sBuf;
	CWnd::GetDlgItemText(CtrlID, str);	
	JetAPI::TCHAR2string(str, sBuf);
	BarcodePtr->SetHexCodePrefix(sBuf.c_str());
	return ;
}
//-------------------------------------------------------------------------------------//
void CBarcodeGeneralWnd::OnCodeSuffixAddStringBtn()
{
	// TODO: Add your control notification handler code here
	CBarcode_General *BarcodePtr=GetBarcodePtr();
	if ( CheckBarcodePtr(BarcodePtr) == false ) { return ; }		
	
	const UINT CtrlID=BARGEN_CODE_SUFFIX_EDIT;	
	if ( ExecAddString(_T("Suffix"),CtrlID ) == false )
	{	return; }

	CString str;
	std::string sBuf;
	CWnd::GetDlgItemText(CtrlID, str);
	JetAPI::TCHAR2string(str, sBuf);
	BarcodePtr->SetHexCodePrefix(sBuf.c_str());
	return ;
}
//-------------------------------------------------------------------------------------//