// LightCtrlBoard.cpp: implementation of the CLightCtrlBoard class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "LightCtrlBoard.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
//Light Ctrl Board Update List
//8DA1:初版
//8DA2:支援12個通道, 8個相機
//8DA3:支援超過64個表格
//8DA4:支援指定觸發時表格的起點
//-------------------------------------------------------------------------------------//
CLightCtrlBoard LightCtrlBoard;
//-------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::BuildTableTypeCombox(CComboBox &Combox)//建立表格樣式
{
	CString str;
	int     idx=0;	
	int     Param=0;

	JetAPI::ClearCombox(Combox);

	str = _T("LED");
	Param=TABLE_TYPE_LED;
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;

	str = _T("3D");
	Param=TABLE_TYPE_DLP;
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::BuildTable3DCastCombox(CComboBox &Combox)//建立3D投光編號
{
	CString str;
	int     idx=0;	
	int     Param=0;

	JetAPI::ClearCombox(Combox);

	int i=0;
	const int Max3DCast = 8;
	for ( i=0; i<=Max3DCast; i++ )
	{
		if ( 0 == i )
		{	str = _T("None"); }
		else
		{	str.Format(_T("%d"), i); }
		Param = i;
		Combox.InsertString(-1, str);
		Combox.SetItemData(idx, Param);
		idx ++;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
int CLightCtrlBoard::GetTableDLPChannelMask(int DLPChannel)//取得表格內DLP引數的遮罩碼
{
	int Mask = 0;
	switch ( DLPChannel ) 
	{
	case 1:	Mask = 0x01;	break;
	case 2:	Mask = 0x02;	break;
	case 3:	Mask = 0x04;	break;
	case 4:	Mask = 0x08;	break;
	case 5:	Mask = 0x10;	break;
	case 6:	Mask = 0x20;	break;
	case 7:	Mask = 0x40;	break;
	case 8:	Mask = 0x80;	break;
	}
	return Mask;
}
//-------------------------------------------------------------------------------------//
CLightCtrlBoard::CLightCtrlBoard()
{	
#ifndef LIGHT_CTRL_DISABLE
	int   i=0;
	::InitializeCriticalSection(&m_csLightCtrlBoard);
	m_usbBoardID = 2;
	m_FPGAMode = FPGA_MODE_UNDEFINED;	
	m_FullVersion = _T("");
	m_FPGAVersion = _T("");	
	
	m_CameraCount = 8;
	m_DLPCastCount = 8;		
	m_LEDChannelCount = 12;
	m_TableRunCount = 0;	
	m_DLPTableStartIndex=-1;
	m_DLPMultiTableEnabled = true;
	m_SupportMoreThan64Table = false;
	m_SupportModifyTriggerFirstIndex = false;
	m_TableRecheckEnabled = FN_DISABLE;//表格重複確認模式
	m_DLPCallbackTimeus = 65535;
	m_DLPPulseWidthTimeus = 20;//DLP脈波寬度
	m_TableMinBetweenTimeus = 50;//us
	m_PreCheckTableListEnabled = FN_DISABLE;
	m_LightCtrlBoardType = LIGHT_CTRL_BOARD_8DA1;

	//DLP的編號映射
	const int DLPCount = DLP_CHANNEL_COUNT;
	for ( i=0; i<DLPCount; i++ )
	{	m_DLPChannelMap[i] = i;	}	
	LoadLightCtrlBoardINI();
	SaveLightCtrlBoardINI();	
#endif//LIGHT_CTRL_DISABLE
}
//-------------------------------------------------------------------------------------//
CLightCtrlBoard::~CLightCtrlBoard()
{
#ifndef LIGHT_CTRL_DISABLE
	CLightCtrlBoard::DisConnect();
	::DeleteCriticalSection(&m_csLightCtrlBoard);
#endif//LIGHT_CTRL_DISABLE		
}
//-------------------------------------------------------------------------------------//
LPCTSTR  CLightCtrlBoard::GetErrorString()
{
	return m_ErrorString;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::UpdateSupportFuncByVersion()//依據版本號更新支援功能
{
	TCHAR str8DA1[]=_T("8DA1");
	TCHAR str8DA2[]=_T("8DA2");
	TCHAR str8DA3[]=_T("8DA3");
	TCHAR str8DA4[]=_T("8DA4");
	CString Version = m_FPGAVersion;
	bool bMoreThan64Table=false;
	bool bModifyTriggerFirstIndex = false;

	if ( Version.CompareNoCase(str8DA1) == 0 )
	{	
		bMoreThan64Table = false;
		bModifyTriggerFirstIndex = false;
	}
	else if ( Version.CompareNoCase(str8DA2) == 0 )
	{
		bMoreThan64Table = false;
		bModifyTriggerFirstIndex = false;
	}
	else if ( Version.CompareNoCase(str8DA3) == 0 )
	{
		bMoreThan64Table = true;
		bModifyTriggerFirstIndex = false;
	}
	else if ( Version.CompareNoCase(str8DA4) == 0 )
	{
		bMoreThan64Table = true;
		bModifyTriggerFirstIndex = true;
	}
	else //其餘更新的版本
	{
		bMoreThan64Table = true;
		bModifyTriggerFirstIndex = true;
	}
	SetSupportMoreThan64Table(bMoreThan64Table);
	SetSupportModifyTriggerFirstIndex(bModifyTriggerFirstIndex);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::Connect(int BoardID)
{
#ifndef LIGHT_CTRL_DISABLE	
	char BoardName[JET_USB_TEXT_SIZE] = "";
	char BoardCode[JET_USB_TEXT_SIZE] = "";
	return Connect(BoardID, BoardName, BoardCode);
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::Connect(int BoardID, char BoardName[], char BoardCode[])
{	
#ifndef LIGHT_CTRL_DISABLE		
	this->m_USB.SetBoardID(BoardID);
	if ( this->m_USB.ConnectUSB(BoardName, BoardCode) == false )
	{
		this->m_ErrorString = m_USB.GetErrorString();
		return false; 
	}
	if ( this->m_USB.CheckUSBDevice() == false )
	{
		this->m_ErrorString = m_USB.GetErrorString();
		return false; 
	}	
	CString strName = BoardName;
	CString strCode = BoardCode;
	SetUSBBoardID(BoardID);
	ReadFPGAVersion(m_FPGAVersion);	
	//m_FullVersion.Format(_T("%s%s (Version:%s)"), strName, strCode, m_FPGAVersion);		
	m_FullVersion.Format(_T("%s#%s%s"), m_FPGAVersion, strName, strCode);	
	UpdateSupportFuncByVersion();
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::GetIsConnected()
{	
#ifndef LIGHT_CTRL_DISABLE		
	return m_USB.GetIsConnected();	
#endif//LIGHT_CTRL_DISABLE
	return false;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::DisConnect()
{
#ifndef LIGHT_CTRL_DISABLE	
	if ( m_USB.GetIsConnected() == true )
	{	m_USB.DisConnectUSB();	}
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
void CLightCtrlBoard::SetUSBBoardID(int val)
{
	m_usbBoardID = val;
}
//-------------------------------------------------------------------------------------//
int CLightCtrlBoard::GetUSBBoardID() const
{
	return m_usbBoardID;
}
//-------------------------------------------------------------------------------------//
int CLightCtrlBoard::GetCameraCount() const//取得相機數量
{
	return m_CameraCount;	
}
//-------------------------------------------------------------------------------------//
int CLightCtrlBoard::GetDLPCastCount() const//取得DLP投光數量
{
	return m_DLPCastCount;	
}
//-------------------------------------------------------------------------------------//
int CLightCtrlBoard::GetLEDChannelCount() const
{
	return m_LEDChannelCount;	
}
//-------------------------------------------------------------------------------------//
LIGHT_CTRL_BOARD_TYPE CLightCtrlBoard::GetLightCtrlBoardType() const
{
	return m_LightCtrlBoardType;
}
//-------------------------------------------------------------------------------------//
CString CLightCtrlBoard::GetLightCtrlBoardTypeText() const//取得控制板樣式文字
{
	CString Text;
	switch ( m_LightCtrlBoardType )
	{
	case LIGHT_CTRL_BOARD_NULL: Text=_T("NULL"); break;
	case LIGHT_CTRL_BOARD_3DA6: Text=_T("3DA6"); break;
	case LIGHT_CTRL_BOARD_8DA1: Text=_T("8DA1"); break;
	default: 
		Text = _T("Undefined");
		break;
	}
	return Text;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::UpdateLightCtrlBoardType()//更新控制板USB命令文字
{
	//寫入USB控制命令的參數	
	char  CmdText[32]="";
	const LIGHT_CTRL_BOARD_TYPE LightCtrlBoardType = GetLightCtrlBoardType();//取得控制板樣式
	
	switch ( LightCtrlBoardType )
	{
	case LIGHT_CTRL_BOARD_8DA1:
		m_CameraCount = 8;
		m_DLPCastCount = 8;		
		m_LEDChannelCount = 12;

		::strcpy(CmdText, "0A8A0A");	
		m_USB.SetCmdTextReadData(CmdText);//讀取資料的命令

		::strcpy(CmdText, "0A4A0A");	
		m_USB.SetCmdTextWriteData(CmdText);//寫入資料的命令

		::strcpy(CmdText, "0B5B0B");
		m_USB.SetCmdTextReadAddress(CmdText);//讀取位址的命令

		::strcpy(CmdText, "0B6B0B");
		m_USB.SetCmdTextWriteAddress(CmdText);//寫入位址的命令
		break;

	default://LIGHT_CTRL_BOARD_3DA6
		m_CameraCount = 0;
		m_DLPCastCount = 8;		
		m_LEDChannelCount = 8;

		::strcpy(CmdText, "0A8A0A");
		m_USB.SetCmdTextReadData(CmdText);//讀取資料的命令

		::strcpy(CmdText, "0A4A0A");	
		m_USB.SetCmdTextWriteData(CmdText);//寫入資料的命令

		::strcpy(CmdText, "0B4B0B");
		m_USB.SetCmdTextReadAddress(CmdText);//讀取位址的命令

		::strcpy(CmdText, "0B4B0B");
		m_USB.SetCmdTextWriteAddress(CmdText);//寫入位址的命令
		break;
	}

	if ( m_DLPCastCount > DLP_CAST_COUNT )
	{	m_DLPCastCount = DLP_CAST_COUNT; }
	if ( m_LEDChannelCount > LED_CHANNEL_COUNT )
	{	m_LEDChannelCount = LED_CHANNEL_COUNT; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::LoopBackTestAll()
{	
#ifndef LIGHT_CTRL_DISABLE
	const int StrSize = JET_USB_TEXT_SIZE;
	char  DataWS[StrSize] = "";
	char  DataRS[StrSize] = "";	
	char  AddressWS[StrSize] = "";
	char  AddressRS[StrSize] = "";
	int DataW = 0;

	int NGs = 0;	
	int i=0;
	int Res=0;

	CString filename;
	FILE *pfile = NULL;
	filename.Format(_T("%s\\LoopBackText.txt"), AOIDataCollect.GetAOITempDirectory());
	pfile = ::_tfopen(filename, _T("w+"));
	if ( pfile == NULL )
	{		
		this->m_ErrorString.Format(_T("Error, Open file fault(%s)"), filename);
		return false;
	}

	::fprintf(pfile, "%s, %s, %s, %s\n", "Address_W", "Data_W", "Address_R", "Data_R");

	NGs = 0;

	JetAPI::IntToHex(W_LOOPBACKA, 2, AddressWS);		
	JetAPI::IntToHex(R_LOOPBACKA, 2, AddressRS);
	for ( i=0; i<0xFFFF; i++ )
	{
		DataW = i;
		JetAPI::IntToHex(DataW, 4, DataWS);
		if ( ExecUSBWrite(AddressWS, DataWS) == false )
		{
			::fclose(pfile); pfile=NULL;
			return false; 
		}
		if ( ExecUSBRead(AddressRS, DataRS) == false )
		{ 
			::fclose(pfile); pfile=NULL;
			return false; 
		}
		
		//if ( DataWS == DataRS ) { continue; }	
		Res = ::strcmp(DataWS, DataRS);
		if ( 0 == Res ) { continue; }

		::fprintf(pfile, "%s, %s, %s, %s\n", AddressWS, DataWS, AddressRS, DataRS);
		NGs ++;
	}

	JetAPI::IntToHex(W_LOOPBACKB, 2, AddressWS);		
	JetAPI::IntToHex(R_LOOPBACKB, 2, AddressRS);
	for ( i=0; i<0xFFFF; i++ )
	{
		DataW = i;
		JetAPI::IntToHex(DataW, 4, DataWS);
		if ( ExecUSBWrite(AddressWS, DataWS) == false )
		{
			::fclose(pfile); pfile=NULL;
			return false; 
		}
		if ( ExecUSBRead(AddressRS, DataRS) == false )
		{ 
			::fclose(pfile); pfile=NULL;
			return false; 
		}

		//if ( DataWS == DataRS ) { continue; }
		Res = ::strcmp(DataWS, DataRS);
		if ( 0 == Res ) { continue; }

		::fprintf(pfile, "%s, %s, %s, %s\n", AddressWS, DataWS, AddressRS, DataRS);
		NGs ++;
	}
	::fclose(pfile); pfile=NULL;

	if ( NGs > 0 ) 
	{
		::ShellExecute(NULL, _T("open"), filename, NULL, NULL, SW_SHOW);		
		return false;
	}
	return true;
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::LoopBackTest(int data, char DataWS[], char DataRA[], char DataRB[])
{
#ifndef LIGHT_CTRL_DISABLE
	const int StrSize = JET_USB_TEXT_SIZE;	
	//char  DataWS[StrSize] = "";
	char  DataRS[StrSize] = "";	
	char  AddressWS[StrSize] = "";
	char  AddressRS[StrSize] = "";
	int DataW = 0;

	int NGs = 0;	
	int i=0;

	NGs = 0;

	JetAPI::IntToHex(W_LOOPBACKA, 2, AddressWS);		
	JetAPI::IntToHex(R_LOOPBACKA, 2, AddressRS);
	
	DataW = data;
	JetAPI::IntToHex(DataW, 4, DataWS);				
	
	if ( ExecUSBWrite(AddressWS, DataWS) == false )
	{	return false;	}	
	
	if ( ExecUSBRead(AddressRS, DataRS) == false )
	{	return false;	}
		
	//DataR1 = DataRS;	
	::strcpy(DataRA, DataRS);

	JetAPI::IntToHex(W_LOOPBACKB, 2, AddressWS);		
	JetAPI::IntToHex(R_LOOPBACKB, 2, AddressRS);

	if ( ExecUSBWrite(AddressWS, DataWS) == false )
	{	return false;	}	

	if ( ExecUSBRead(AddressRS, DataRS) == false )
	{ 	return false;	}

	//DataR2 = DataRS;
	::strcpy(DataRB, DataRS);
		
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::LoopBackTestA(int data, char DataWS[], char DataR[])
{
#ifndef LIGHT_CTRL_DISABLE
	const int StrSize = JET_USB_TEXT_SIZE;	
	//char  DataWS[StrSize] = "";
	char  DataRS[StrSize] = "";	
	char  AddressWS[StrSize] = "";
	char  AddressRS[StrSize] = "";
	
	int DataW = 0;
	int NGs = 0;	
	int i=0;
	NGs = 0;

	JetAPI::IntToHex(W_LOOPBACKA, 2, AddressWS);		
	JetAPI::IntToHex(R_LOOPBACKA, 2, AddressRS);
	
	DataW = data;
	JetAPI::IntToHex(DataW, 4, DataWS);	
	if ( ExecUSBWrite(AddressWS, DataWS) == false )
	{	return false;	}				

	if ( ExecUSBRead(AddressRS, DataRS) == false )
	{	return false;	}
		
	//DataR = DataRS;	
	::strcpy(DataR, DataRS);
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::LoopBackTestB(int data, char DataWS[], char DataR[])
{
#ifndef LIGHT_CTRL_DISABLE
	const int StrSize = JET_USB_TEXT_SIZE;	
	//char  DataWS[StrSize] = "";
	char  DataRS[StrSize] = "";	
	char  AddressWS[StrSize] = "";
	char  AddressRS[StrSize] = "";
	int DataW = 0;
	int NGs = 0;	
	int i=0;
	NGs = 0;

	JetAPI::IntToHex(W_LOOPBACKB, 2, AddressWS);		
	JetAPI::IntToHex(R_LOOPBACKB, 2, AddressRS);
	DataW = data;
	JetAPI::IntToHex(DataW, 4, DataWS);		

	if ( ExecUSBWrite(AddressWS, DataWS) == false )
	{	return false;	}		

	if ( ExecUSBRead(AddressRS, DataRS) == false )
	{ 	return false;	}

	//DataR = DataRS;
	::strcpy(DataR, DataRS);
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::ExecUSBRead(char Address[], char Data[])
{	
#ifndef LIGHT_CTRL_DISABLE	
	bool IsOK = true;
	::EnterCriticalSection(&m_csLightCtrlBoard);
	IsOK = m_USB.USBRead(Address, Data);
	::LeaveCriticalSection(&m_csLightCtrlBoard);
	if ( false == IsOK )
	{
		CString Err = m_USB.GetErrorString();
		this->m_ErrorString.Format(_T("%s"), Err);
		return false;	
	}	
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::ExecUSBWrite(char Address[], char Data[])
{
#ifndef LIGHT_CTRL_DISABLE
//	if ( m_USB.WriteData(Data) == false )
//	{	return false; }
	bool IsOK = true;
	::EnterCriticalSection(&m_csLightCtrlBoard);
	IsOK = m_USB.USBWrite(Address, Data);
	::LeaveCriticalSection(&m_csLightCtrlBoard);
	if ( false == IsOK )
	{
		CString Err = m_USB.GetErrorString();
		this->m_ErrorString.Format(_T("%s"), Err);
		return false;	
	}	
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::SaveLightCtrlBoardINI()//寫參數至檔案
{
	int     i=0;
	bool    IsOK = true;	
	CString FileName;
	CString KeyName;
	CString KeyString;
	CString Section;
	const LIGHT_CTRL_BOARD_TYPE LightCtrlBoardType = GetLightCtrlBoardType();

	FileName.Format(_T("%s\\%s"), AOIDataCollect.GetAOIDirectory(), _T("LightCtrlBoard.ini"));	

	Section = _T("Light Ctrl Board");
	KeyName.Format(_T("Light Ctrl Board Type")); KeyString.Format(_T("%d"), LightCtrlBoardType);
	if ( JetAPI::SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }

	switch ( LightCtrlBoardType )
	{
	case LIGHT_CTRL_BOARD_3DA6:	Section = _T("3DA6"); break;
	case LIGHT_CTRL_BOARD_8DA1:	Section = _T("8DA1"); break;
	default: Section=_T(""); break;
	}
	if ( Section.GetLength() == 0 )
	{ return true; }
	
	//USB編號
	KeyName.Format(_T("USB Board ID")); KeyString.Format(_T("%d"), m_usbBoardID);
	if ( JetAPI::SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }

	//表格重複確認模式
	KeyName.Format(_T("Table Recheck")); KeyString.Format(_T("%d"), m_TableRecheckEnabled);
	if ( JetAPI::SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }

	KeyName.Format(_T("DLP Enable Multi Table")); KeyString.Format(_T("%d"), m_DLPMultiTableEnabled);
	if ( JetAPI::SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	KeyName.Format(_T("DLP Callback Time")); KeyString.Format(_T("%d"), m_DLPCallbackTimeus);
	if ( JetAPI::SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	KeyName.Format(_T("DLP Pulse Width Time")); KeyString.Format(_T("%d"), m_DLPPulseWidthTimeus);
	if ( JetAPI::SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }

	KeyName.Format(_T("Table Min Between Time")); KeyString.Format(_T("%d"), m_TableMinBetweenTimeus);
	if ( JetAPI::SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	KeyName.Format(_T("Table Pre-Check Enabled")); KeyString.Format(_T("%d"), m_PreCheckTableListEnabled);
	if ( JetAPI::SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	for ( i=0; i<DLP_CHANNEL_COUNT; i++ )
	{
		KeyName.Format(_T("DLP Channel %d"), i); KeyString.Format(_T("%d"), m_DLPChannelMap[i]);
		if ( JetAPI::SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
		{	IsOK = false; }	
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::LoadLightCtrlBoardINI()//從檔案讀取參數
{
	int     i=0;
	int     nValue=0;
	CString FileName;
	CString TempStr;
	CString KeyName;
	CString KeyString;
	CString Section;	
	const size_t StringSize = 128;
	TCHAR ReturnString[StringSize];
	LIGHT_CTRL_BOARD_TYPE LightCtrlBoardType = GetLightCtrlBoardType();

	FileName.Format(_T("%s\\%s"), AOIDataCollect.GetAOIDirectory(), _T("LightCtrlBoard.ini"));

	Section = _T("Light Ctrl Board");
	KeyName.Format(_T("Light Ctrl Board Type")); KeyString.Format(_T("%d"), LightCtrlBoardType);
	if ( JetAPI::LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	
		nValue = ::_ttoi(ReturnString); 
		switch ( nValue )
		{
		case LIGHT_CTRL_BOARD_NULL:
		case LIGHT_CTRL_BOARD_3DA6:
		case LIGHT_CTRL_BOARD_8DA1:
			LightCtrlBoardType = (LIGHT_CTRL_BOARD_TYPE)nValue;
			m_LightCtrlBoardType = LightCtrlBoardType;
			break;
		}		
	}

	switch ( LightCtrlBoardType )
	{
	case LIGHT_CTRL_BOARD_3DA6:	Section = _T("3DA6"); break;
	case LIGHT_CTRL_BOARD_8DA1:	Section = _T("8DA1"); break;
	default: Section=_T(""); break;
	}
	if ( Section.GetLength() == 0 )
	{ return true; }

	//USB編號
	KeyName.Format(_T("USB Board ID")); KeyString.Format(_T("%d"), m_usbBoardID);
	if ( JetAPI::LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	
		nValue = ::_ttoi(ReturnString); 		
		if ( nValue >= 0 ) 
		{	m_usbBoardID = nValue; }
	}	

	//表格重複確認模式
	KeyName.Format(_T("Table Recheck")); KeyString.Format(_T("%d"), m_TableRecheckEnabled);
	if ( JetAPI::LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	
		nValue = ::_ttoi(ReturnString); 
		switch ( nValue )
		{
		case FN_ENABLE:
		case FN_DISABLE:
			m_TableRecheckEnabled = nValue;
			break;
		}		
	}

	KeyName.Format(_T("DLP Enable Multi Table")); KeyString.Format(_T("%d"), m_DLPMultiTableEnabled);	
	if ( JetAPI::LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	
		nValue = ::_ttoi(ReturnString); 
		if ( 0 == nValue ) { m_DLPMultiTableEnabled = false; }
		else { m_DLPMultiTableEnabled = true; }
	}

	KeyName.Format(_T("DLP Callback Time")); KeyString.Format(_T("%d"),m_DLPCallbackTimeus);
	if ( JetAPI::LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	m_DLPCallbackTimeus = ::_ttoi(ReturnString); }

	KeyName.Format(_T("DLP Pulse Width Time")); KeyString.Format(_T("%d"), m_DLPPulseWidthTimeus);
	if ( JetAPI::LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	m_DLPPulseWidthTimeus = ::_ttoi(ReturnString); }

	KeyName.Format(_T("Table Min Between Time")); KeyString.Format(_T("%d"),m_TableMinBetweenTimeus);
	if ( JetAPI::LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	m_TableMinBetweenTimeus = ::_ttoi(ReturnString); }

	KeyName.Format(_T("Table Pre-Check Enabled")); KeyString.Format(_T("%d"), m_PreCheckTableListEnabled);
	if ( JetAPI::LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	m_PreCheckTableListEnabled = ::_ttoi(ReturnString); }

	for ( i=0; i<DLP_CHANNEL_COUNT; i++ )
	{
		KeyName.Format(_T("DLP Channel %d"), i); KeyString.Format(_T("%d"), m_DLPChannelMap[i]);
		if ( JetAPI::LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
		{	m_DLPChannelMap[i] = ::_ttoi(ReturnString); }		
	}

	UpdateLightCtrlBoardType();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::ReadRAMData(char data[])
{
#ifndef LIGHT_CTRL_DISABLE
	const int StrSize = JET_USB_TEXT_SIZE;		
	char  DataRS[StrSize] = "";		
	char  AddressRS[StrSize] = "";
	
	JetAPI::IntToHex(R_RAM_DATA, 2, AddressRS);
	if ( ExecUSBRead(AddressRS, DataRS) == false )
	{	return false;	}

	//data = DataRS;
	::strcpy(data, DataRS);
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::ReadRAMAddress(char data[])
{
#ifndef LIGHT_CTRL_DISABLE
	const int StrSize = JET_USB_TEXT_SIZE;		
	char  DataRS[StrSize] = "";		
	char  AddressRS[StrSize] = "";
	
	JetAPI::IntToHex(R_RAM_ADDRESS, 2, AddressRS);
	if ( ExecUSBRead(AddressRS, DataRS) == false )
	{	return false;	}

	//data = DataRS;
	::strcpy(data, DataRS);
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::CheckLightCtrlBoardError(bool &IsError, CString &ErrorStr)
{
	IsError = false;
	ErrorStr = _T("");
	int ErrValue = 0;
	if( ReadErrorCode(ErrValue) == false ) { return false; }
	
	CString tmpErrStr = _T("");
	if( (ErrValue&0xff) > 0 )
	{
		DecodeErrorCodeText(ErrValue, ErrorStr);
		IsError = true;
	}

	if( IsError == true )
	{
		CString ErrDLP = _T("");
		if( ReadDLPErrCH(ErrDLP) == false ) { return false; }
		tmpErrStr.Format(_T("%s\n%s"), ErrorStr, ErrDLP);
		ErrorStr = tmpErrStr;
	}	
		
	CString DLPStep = _T("");
	if( ReadDLPTrigStepInfo(DLPStep) == false ) { return false; }
	tmpErrStr.Format(_T("%s\n%s"), ErrorStr, DLPStep);
	ErrorStr = tmpErrStr;	
		
	CString CountStr = _T("");
	if( GetTriggerCountText(CountStr) == false ) { return false; }
	tmpErrStr.Format(_T("%s\n%s\n"), ErrorStr, CountStr);
	ErrorStr = tmpErrStr;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::ReadErrorCode(int &Error)
{
	Error = 0;
#ifndef LIGHT_CTRL_DISABLE	
	const int StrSize = JET_USB_TEXT_SIZE;		
	char  DataRS[StrSize] = "";		
	char  AddressRS[StrSize] = "";	
	JetAPI::IntToHex(R_ERROR, 2, AddressRS);
	if ( ExecUSBRead(AddressRS, DataRS) == false ) 
	{	return false;	}

	const int Len = (int)::strlen(DataRS);
	JetAPI::HexToInt(DataRS, Len, Error);
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::ReadDLPErrCH(CString &ErrDlp)
{
	ErrDlp = _T("");
#ifndef LIGHT_CTRL_DISABLE	
	//	if ( this->SwitchToRead() == false ) {	return false;	}
	const int StrSize = JET_USB_TEXT_SIZE;		
	char  DataRS[StrSize] = "";		
	char  AddressRS[StrSize] = "";	
	JetAPI::IntToHex(R_DLP_ERR_CH, 2, AddressRS);
	if ( ExecUSBRead(AddressRS, DataRS) == false ) 
	{	return false;	}

	int value = 0;
	const int len = (int)(::strlen(DataRS));
	JetAPI::HexToInt(DataRS, len, value);	
	//9~16bit ( 1~8 DLP )
	CString tmpErr = _T("");
	if(value&0x0100) { tmpErr+=_T("01,");}
	if(value&0x0200) { tmpErr+=_T("02,");}
	if(value&0x0400) { tmpErr+=_T("03,");}
	if(value&0x0800) { tmpErr+=_T("04,");}
	if(value&0x1000) { tmpErr+=_T("05,");}
	if(value&0x2000) { tmpErr+=_T("06,");}
	if(value&0x4000) { tmpErr+=_T("07,");}
	if(value&0x8000) { tmpErr+=_T("08,");}
	if( tmpErr.GetLength() > 0  )
	{	
		ErrDlp.Format(_T("ErrDLP:%s"), tmpErr);
		ErrDlp.SetAt(ErrDlp.GetLength()-1, _T(' '));
	}
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::ReadDLPTrigStepInfo(CString &DLPTrigInfo)
{
	DLPTrigInfo = _T("");
#ifndef LIGHT_CTRL_DISABLE	
	const int StrSize = JET_USB_TEXT_SIZE;		
	char  DataRS[StrSize] = "";		
	char  AddressRS[StrSize] = "";	
	CString DLPStep = _T("DLP Trig Step: 8->1\n");
	JetAPI::IntToHex(R_DLP_TRIG_STEP, 2, AddressRS);	
	if ( ExecUSBRead(AddressRS, DataRS) == false ) 
	{	return false;	}

	int value = 0;
	const int len = (int)(::strlen(DataRS));
	JetAPI::HexToInt(DataRS, len, value);	
	//1~8bit ( 1~8 DLP Trig Step )
	if(value&0x0080) { DLPStep+=_T("1");} else { DLPStep+=_T("0");}
	if(value&0x0040) { DLPStep+=_T("1");} else { DLPStep+=_T("0");}
	if(value&0x0020) { DLPStep+=_T("1");} else { DLPStep+=_T("0");}
	if(value&0x0010) { DLPStep+=_T("1");} else { DLPStep+=_T("0");}
	if(value&0x0008) { DLPStep+=_T("1");} else { DLPStep+=_T("0");}
	if(value&0x0004) { DLPStep+=_T("1");} else { DLPStep+=_T("0");}
	if(value&0x0002) { DLPStep+=_T("1");} else { DLPStep+=_T("0");}
	if(value&0x0001) { DLPStep+=_T("1");} else { DLPStep+=_T("0");}
	DLPStep+=_T("   DLP Step");
	DLPStep+=_T("\n");
	//9~16bit ( 1~8 DLP Trig CallBack)
	if(value&0x8000) { DLPStep+=_T("1");} else { DLPStep+=_T("0");}
	if(value&0x4000) { DLPStep+=_T("1");} else { DLPStep+=_T("0");}
	if(value&0x2000) { DLPStep+=_T("1");} else { DLPStep+=_T("0");}
	if(value&0x1000) { DLPStep+=_T("1");} else { DLPStep+=_T("0");}
	if(value&0x0800) { DLPStep+=_T("1");} else { DLPStep+=_T("0");}
	if(value&0x0400) { DLPStep+=_T("1");} else { DLPStep+=_T("0");}
	if(value&0x0200) { DLPStep+=_T("1");} else { DLPStep+=_T("0");}
	if(value&0x0100) { DLPStep+=_T("1");} else { DLPStep+=_T("0");}
	DLPStep+=_T("   Trig CallBack");
	
	DLPTrigInfo = DLPStep;	
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::DecodeErrorCodeText(int ErrorCode, CString &ErrorStr)
{
	ErrorStr = _T("Ready");
	if( ErrorCode == 0 ) { return true; }

	ErrorStr = _T("");	
	CString ErrorText = _T("");
	CString ErrorCodeBitStr = _T("0000000000000000");
	if( ErrorCode & 0x0001 ) { ErrorCodeBitStr.SetAt(15, '1'); ErrorText+=_T("(01)SET_ERROR\n"); }
	if( ErrorCode & 0x0002 ) { ErrorCodeBitStr.SetAt(14, '1'); ErrorText+=_T("(02)NOT RESET\n"); }
	if( ErrorCode & 0x0004 ) { ErrorCodeBitStr.SetAt(13, '1'); ErrorText+=_T("(03)ERR_DLP_TRIG\n"); }
	if( ErrorCode & 0x0008 ) { ErrorCodeBitStr.SetAt(12, '1'); ErrorText+=_T("(04)ERR_LED_TYPE\n"); }
	if( ErrorCode & 0x0010 ) { ErrorCodeBitStr.SetAt(11, '1'); ErrorText+=_T("(05)ERR_DLP OVERTIME\n"); }
	if( ErrorCode & 0x0020 ) { ErrorCodeBitStr.SetAt(10, '1'); ErrorText+=_T("(06)ERR FPGA RAM\n"); }
	if( ErrorCode & 0x0040 ) { ErrorCodeBitStr.SetAt(9, '1'); ErrorText+=_T("(07)RESERVE\n"); }
	if( ErrorCode & 0x0080 ) { ErrorCodeBitStr.SetAt(8, '1'); ErrorText+=_T("(08)RESERVE\n"); }

	bool PowerBoardErr = false;
	CString PowerErrorText = _T("");
	if( ErrorCode & 0x0100 ) { ErrorCodeBitStr.SetAt(7, '1'); PowerErrorText+=_T("(09)POWER BOARD\n"); }
	if( ErrorCode & 0x0200 ) { ErrorCodeBitStr.SetAt(6, '1'); PowerErrorText+=_T("(10)POWER BOARD\n"); }
	if( ErrorCode & 0x0400 ) { ErrorCodeBitStr.SetAt(5, '1'); PowerErrorText+=_T("(11)POWER BOARD\n"); }
	if( ErrorCode & 0x0800 ) { ErrorCodeBitStr.SetAt(4, '1'); PowerErrorText+=_T("(12)POWER BOARD\n"); }
	if( ErrorCode & 0x1000 ) { ErrorCodeBitStr.SetAt(3, '1'); PowerErrorText+=_T("(13)POWER BOARD\n"); }
	if( ErrorCode & 0x2000 ) { ErrorCodeBitStr.SetAt(2, '1'); PowerErrorText+=_T("(14)POWER BOARD\n"); }
	if( ErrorCode & 0x4000 ) { ErrorCodeBitStr.SetAt(1, '1'); PowerErrorText+=_T("(15)POWER BOARD\n"); }
	if( ErrorCode & 0x8000 ) { ErrorCodeBitStr.SetAt(0, '1'); PowerErrorText+=_T("(16)POWER BOARD\n"); }

	if( PowerBoardErr == true )
	{ ErrorStr.Format(_T("(%s) %s\nPowerCH ERROR:%s"), ErrorCodeBitStr, ErrorText, PowerErrorText); }
	else
	{ ErrorStr.Format(_T("(%s) %s"), ErrorCodeBitStr, ErrorText); }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::GetTriggerCountText(CString &TrigCount)
{	
#ifndef LIGHT_CTRL_DISABLE
	UINT cntPC_FPGA=0;
	UINT cntFPG_CCD=0;
	UINT cntFPG_DLP1=0, cntFPG_DLP2=0, cntFPG_DLP3=0, cntFPG_DLP4=0;
	UINT cntFPG_DLP5=0, cntFPG_DLP6=0, cntFPG_DLP7=0, cntFPG_DLP8=0;
	UINT cntDLP1_FPG=0, cntDLP2_FPG=0, cntDLP3_FPG=0, cntDLP4_FPG=0;
	UINT cntDLP5_FPG=0, cntDLP6_FPG=0, cntDLP7_FPG=0, cntDLP8_FPG=0;

	if( GetPCtoFPGATrigCount(cntPC_FPGA) == false ) { return false; }	
	if( GetFPGAtoCCDTrigCount(cntFPG_CCD) == false ) { return false; }			

	if( GetFPGAtoDLPTrigCount(DLP_CHANNEL_1, cntFPG_DLP1) == false ) { return false; }
	if( GetFPGAtoDLPTrigCount(DLP_CHANNEL_2, cntFPG_DLP2) == false ) { return false; }
	if( GetFPGAtoDLPTrigCount(DLP_CHANNEL_3, cntFPG_DLP3) == false ) { return false; }
	if( GetFPGAtoDLPTrigCount(DLP_CHANNEL_4, cntFPG_DLP4) == false ) { return false; }
	if( GetFPGAtoDLPTrigCount(DLP_CHANNEL_5, cntFPG_DLP5) == false ) { return false; }
	if( GetFPGAtoDLPTrigCount(DLP_CHANNEL_6, cntFPG_DLP6) == false ) { return false; }
	if( GetFPGAtoDLPTrigCount(DLP_CHANNEL_7, cntFPG_DLP7) == false ) { return false; }
	if( GetFPGAtoDLPTrigCount(DLP_CHANNEL_8, cntFPG_DLP8) == false ) { return false; }

	if( GetDLPtoFPGATrigCount(DLP_CHANNEL_1, cntDLP1_FPG) == false ) { return false; }
	if( GetDLPtoFPGATrigCount(DLP_CHANNEL_2, cntDLP2_FPG) == false ) { return false; }
	if( GetDLPtoFPGATrigCount(DLP_CHANNEL_3, cntDLP3_FPG) == false ) { return false; }
	if( GetDLPtoFPGATrigCount(DLP_CHANNEL_4, cntDLP4_FPG) == false ) { return false; }
	if( GetDLPtoFPGATrigCount(DLP_CHANNEL_5, cntDLP5_FPG) == false ) { return false; }
	if( GetDLPtoFPGATrigCount(DLP_CHANNEL_6, cntDLP6_FPG) == false ) { return false; }
	if( GetDLPtoFPGATrigCount(DLP_CHANNEL_7, cntDLP7_FPG) == false ) { return false; }
	if( GetDLPtoFPGATrigCount(DLP_CHANNEL_8, cntDLP8_FPG) == false ) { return false; }	

	TrigCount.Format(_T("PC_FPGA(%d), FPGA_CCD(%d)\nFPGA_DLP(%d, %d, %d, %d, %d, %d, %d, %d)\nDLP_FPGA(%d, %d, %d, %d, %d, %d, %d, %d)"), 
		cntPC_FPGA, cntFPG_CCD, 
		cntFPG_DLP1, cntFPG_DLP2, cntFPG_DLP3, cntFPG_DLP4, cntFPG_DLP5, cntFPG_DLP6, cntFPG_DLP7, cntFPG_DLP8, 
		cntDLP1_FPG, cntDLP2_FPG, cntDLP3_FPG, cntDLP4_FPG, cntDLP5_FPG, cntDLP6_FPG, cntDLP7_FPG, cntDLP8_FPG); 
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::ReadFPGAVersion(CString &Version)
{	
#ifndef LIGHT_CTRL_DISABLE
	const int StrSize = JET_USB_TEXT_SIZE;		
	char  DataRS[StrSize] = "";		
	if ( ExecUSBRead("3F", DataRS) == false ) { return false; }
	Version = DataRS;
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CLightCtrlBoard::GetFPGAVersion() const
{
	return m_FPGAVersion;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CLightCtrlBoard::GetFullVersion() const
{
	return m_FullVersion;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::GetIsWorkingNow(bool &Working)
{
#ifndef LIGHT_CTRL_DISABLE
	int value = 0;	
	const int StrSize = JET_USB_TEXT_SIZE;		
	char  DataRS[StrSize] = "";		
	if ( ExecUSBRead("09", DataRS) == false ) 
	{	return false; }
	const int Len = (int)::strlen(DataRS);
	JetAPI::HexToInt(DataRS, Len, value);
	if ( value&0x01 ) 
	{	Working = true; }
	else
	{	Working = false; }
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::GetIsFPGAOK(bool &IsOK)
{
#ifndef LIGHT_CTRL_DISABLE
	int value = 0;	
	const int StrSize = JET_USB_TEXT_SIZE;		
	char  DataRS[StrSize] = "";		
	if ( ExecUSBRead("09", DataRS) == false ) 
	{	return false; }
	const int Len = (int)::strlen(DataRS);
	JetAPI::HexToInt(DataRS, Len, value);
	if ( value&0x02 ) 
	{	IsOK = true; }
	else
	{	IsOK = false; }
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::GetPCtoFPGATrigCount(UINT &count)
{
#ifndef LIGHT_CTRL_DISABLE
	int value = 0;
	const int StrSize = JET_USB_TEXT_SIZE;		
	char  DataRS[StrSize] = "";
	if ( ExecUSBRead("0B", DataRS) == false ) 
	{	return false; }
	const int Len = (int)::strlen(DataRS);
	JetAPI::HexToInt(DataRS, Len, value);
	count = value;
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::GetFPGAtoCCDTrigCount(UINT &count)
{
#ifndef LIGHT_CTRL_DISABLE
	int value = 0;
	const int StrSize = JET_USB_TEXT_SIZE;		
	char  DataRS[StrSize] = "";
	if ( ExecUSBRead("0C", DataRS) == false ) 
	{	return false; }
	const int Len = (int)::strlen(DataRS);
	JetAPI::HexToInt(DataRS, Len, value);
	count = value;
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::GetFPGAtoCCDTrigCount(UINT CameraID, UINT &count)
{
#ifndef LIGHT_CTRL_DISABLE
	int value = 0;	
	bool  bUsed=false;
	const int StrSize = JET_USB_TEXT_SIZE;		
	char  DataRS[StrSize] = "";
	char  AddressRS[StrSize] = "";
	const LIGHT_CTRL_BOARD_TYPE LightCtrlBoardType = GetLightCtrlBoardType();

	if ( LIGHT_CTRL_BOARD_3DA6 == LightCtrlBoardType )
	{
		bUsed = true;
		switch ( CameraID )
		{
		case CAMERA_ID_1:	::strcpy(AddressRS, "0C"); break;
		default:
			count = 0;
			bUsed=false;		
			break;
		}	
	}
	if ( LIGHT_CTRL_BOARD_8DA1 == LightCtrlBoardType )
	{
		bUsed = true;
		switch ( CameraID )
		{
		case CAMERA_ID_1:	::strcpy(AddressRS, "0C"); break;	
		case CAMERA_ID_2:	::strcpy(AddressRS, "1D"); break;
		case CAMERA_ID_3:	::strcpy(AddressRS, "1E"); break;
		case CAMERA_ID_4:	::strcpy(AddressRS, "1F"); break;
		case CAMERA_ID_5:	::strcpy(AddressRS, "20"); break;
		case CAMERA_ID_6:	::strcpy(AddressRS, "21"); break;
		case CAMERA_ID_7:	::strcpy(AddressRS, "22"); break;
		case CAMERA_ID_8:	::strcpy(AddressRS, "23"); break;	
		default:
			count = 0;
			bUsed=false;		
			break;
		}	
	}	
	if ( false == bUsed )
	{	return true; }

	if ( ExecUSBRead(AddressRS, DataRS) == false ) 
	{	return false; }
	const int Len = (int)::strlen(DataRS);
	JetAPI::HexToInt(DataRS, Len, value);
	count = value;
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::SetDLPEnable(UINT DLPChannel)
{
	//等同於EnableDLPChannel
	return true;
#ifndef LIGHT_CTRL_DISABLE
	int   DataW = 0;
	const int StrSize = JET_USB_TEXT_SIZE;		
	char  DataWS[StrSize] = "";
	char  Address[StrSize] = "09";	

	DataW = 0;
	DataW |= 0x01;//DLP-01
	DataW |= 0x02;//DLP-02
	DataW |= 0x04;//DLP-03
	DataW |= 0x08;//DLP-04
	DataW |= 0x10;//DLP-05
	DataW |= 0x20;//DLP-06
	DataW |= 0x40;//DLP-07
	DataW |= 0x80;//DLP-08
	JetAPI::IntToHex(DataW, 4, DataWS);		
	if (  ExecUSBWrite(Address, DataWS) == false ) 
	{	return false; }	
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::GetFPGAtoDLPTrigCount(UINT DLPChannel, UINT &count)
{
#ifndef LIGHT_CTRL_DISABLE
	int value = 0;
	const int StrSize = JET_USB_TEXT_SIZE;		
	char  DataRS[StrSize] = "";
	char  Address[StrSize] = "";	
	switch ( DLPChannel )
	{
	case DLP_CHANNEL_1: ::strcpy(Address, "0D"); break;
	case DLP_CHANNEL_2: ::strcpy(Address, "0E"); break;
	case DLP_CHANNEL_3: ::strcpy(Address, "0F"); break;
	case DLP_CHANNEL_4: ::strcpy(Address, "10"); break;
	case DLP_CHANNEL_5: ::strcpy(Address, "11"); break;
	case DLP_CHANNEL_6: ::strcpy(Address, "12"); break;
	case DLP_CHANNEL_7: ::strcpy(Address, "13"); break;
	case DLP_CHANNEL_8: ::strcpy(Address, "14"); break;
	default:	
		m_ErrorString.Format(_T("Error, DLP CH Error!(%d)"), DLPChannel); 
		return false;
	}
	if ( ExecUSBRead(Address, DataRS) == false ) 
	{	return false; }
	const int Len = (int)::strlen(DataRS);
	JetAPI::HexToInt(DataRS, Len, value);
	count = value;
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::GetDLPtoFPGATrigCount(UINT DLPChannel, UINT &count)
{
#ifndef LIGHT_CTRL_DISABLE
	int value = 0;
	const int StrSize = JET_USB_TEXT_SIZE;		
	char  DataRS[StrSize] = "";
	char  Address[StrSize] = "";	
	switch ( DLPChannel )
	{
	case DLP_CHANNEL_1: ::strcpy(Address, "15"); break;
	case DLP_CHANNEL_2: ::strcpy(Address, "16"); break;
	case DLP_CHANNEL_3: ::strcpy(Address, "17"); break;
	case DLP_CHANNEL_4: ::strcpy(Address, "18"); break;
	case DLP_CHANNEL_5: ::strcpy(Address, "19"); break;
	case DLP_CHANNEL_6: ::strcpy(Address, "1A"); break;
	case DLP_CHANNEL_7: ::strcpy(Address, "1B"); break;
	case DLP_CHANNEL_8: ::strcpy(Address, "1C"); break;
	default:	
		m_ErrorString.Format(_T("Error, DLP CH Error!(%d)"), DLPChannel); 
		return false;
	}
	if ( ExecUSBRead(Address, DataRS) == false ) 
	{	return false; }
	const int Len = (int)::strlen(DataRS);
	JetAPI::HexToInt(DataRS, Len, value);
	count = value;
	return true;
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::TriggerStart()//TriggerStart
{
#ifndef LIGHT_CTRL_DISABLE
	//	if ( this->SetMode_PCWrite() == false ) {	return false;	}
	const int StrSize = JET_USB_TEXT_SIZE;		
	char  DataS[StrSize] = "";
	char  Address[StrSize] = "03";	
	UINT adds = W_WRITE;
	UINT data = 0;	

	UINT Tempdata = 0x0480;		//0000 0100 0000 0000	//0x0400
	UINT Ack = 0x0200;			//0000 0010 0000 0000	//0x0200	
	data = Tempdata; 
	JetAPI::IntToHex(adds, JET_USB_CTRL_ADDRESS_SIZE, Address);
	JetAPI::IntToHex(data, JET_USB_CTRL_DATA_SIZE, DataS);	
	if( ExecUSBWrite(Address, DataS) == false ) 
	{	return false; }

	//ACK L->H
	data += Ack;
	JetAPI::IntToHex( data, JET_USB_CTRL_DATA_SIZE, DataS);	
	if( ExecUSBWrite(Address, DataS) == false ) 
	{	return false; }
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::ClearAllCount()//ClearAllCount
{
#ifndef LIGHT_CTRL_DISABLE
	//	if ( this->SetMode_PCWrite() == false ) {	return false;	}
	const int StrSize = JET_USB_TEXT_SIZE;		
	char  DataS[StrSize] = "";
	char  Address[StrSize] = "03";	
	UINT adds = W_WRITE;
	UINT data = 0;	

	UINT Tempdata = 0x1080;		//0001 0000 0000 0000	//0x1000
	UINT Ack = 0x0800;			//0000 1000 0000 0000	//0x0800	
	data = Tempdata; 
	JetAPI::IntToHex( adds, JET_USB_CTRL_ADDRESS_SIZE, Address);
	JetAPI::IntToHex( data, JET_USB_CTRL_DATA_SIZE, DataS);	
	if( ExecUSBWrite(Address, DataS) == false ) 
	{	return false; }

	//ACK L->H
	data += Ack;
	JetAPI::IntToHex( data, JET_USB_CTRL_DATA_SIZE, DataS);	
	if( ExecUSBWrite(Address, DataS) == false ) 
	{	return false; }	
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::ClearAll()//ClearAll
{
#ifndef LIGHT_CTRL_DISABLE
	//if ( this->SetMode_PCWrite() == false ) {	return false;	}
	const int StrSize = JET_USB_TEXT_SIZE;		
	char  DataS[StrSize] = "";
	char  Address[StrSize] = "03";	
	UINT adds = W_WRITE;
	UINT data = 0;	

	UINT Tempdata = 0x8080;		//1000 0000 0000 0000	//0x4000
	UINT Ack = 0x2000;			//0010 0000 0000 0000	//0x2000	
	data = Tempdata; 
	JetAPI::IntToHex( adds, JET_USB_CTRL_ADDRESS_SIZE, Address);
	JetAPI::IntToHex( data, JET_USB_CTRL_DATA_SIZE, DataS);	
	if( ExecUSBWrite(Address, DataS) == false ) 
	{	return false; }

	//ACK L->H
	data += Ack;
	JetAPI::IntToHex( data, JET_USB_CTRL_DATA_SIZE, DataS);	
	if( ExecUSBWrite(Address, DataS) == false ) 
	{	return false; }	
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::SwitchToFPGA()
{
#ifndef LIGHT_CTRL_DISABLE
	if( FPGA_MODE_FPGA == m_FPGAMode ) { return true; }
	if( SetMode_FPGA() == false ) 
	{	return false; }
	return true;
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::SwitchToRead()
{
#ifndef LIGHT_CTRL_DISABLE
	if( FPGA_MODE_READ == m_FPGAMode ) { return true; }
	if( SetMode_PCRead() == false ) 
	{	return false; }
	return true;
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::SwitchToWrite()
{
#ifndef LIGHT_CTRL_DISABLE
	if( FPGA_MODE_WRITE == m_FPGAMode ) { return true; }
	if( SetMode_PCWrite() == false ) 
	{	return false; }
	return true;
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::SwitchToAssign()
{
#ifndef LIGHT_CTRL_DISABLE
	if( FPGA_MODE_ASSIGN == m_FPGAMode ) { return true; }
	if( SetMode_PCAssign() == false ) 
	{	return false; }
	return true;
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::SetMode_FPGA()//FPGA Run
{
#ifndef LIGHT_CTRL_DISABLE
	const int StrSize = JET_USB_TEXT_SIZE;		
	char  DataS[StrSize] = "";
	char  Address[StrSize] = "04";
	UINT adds = W_MODE_SET;
	UINT data = SET_DATA_FPGA_MODE;	
	JetAPI::IntToHex( adds, JET_USB_CTRL_ADDRESS_SIZE, Address);
	JetAPI::IntToHex( data, JET_USB_CTRL_DATA_SIZE, DataS);
	if( ExecUSBWrite(Address, DataS) == false ) 
	{	return false; }
	m_FPGAMode = FPGA_MODE_FPGA;
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::SetMode_PCWrite()//寫入
{
#ifndef LIGHT_CTRL_DISABLE
	const int StrSize = JET_USB_TEXT_SIZE;		
	char  DataS[StrSize] = "";
	char  Address[StrSize] = "04";		
	UINT adds = W_MODE_SET;
	UINT data = SET_DATA_PC_WRITE_MODE;	
	JetAPI::IntToHex( adds, JET_USB_CTRL_ADDRESS_SIZE, Address);
	JetAPI::IntToHex( data, JET_USB_CTRL_DATA_SIZE, DataS);
	if( ExecUSBWrite(Address, DataS) == false ) 
	{	return false; }
	m_FPGAMode = FPGA_MODE_WRITE;
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::SetMode_PCRead()//讀取
{
#ifndef LIGHT_CTRL_DISABLE
	const int StrSize = JET_USB_TEXT_SIZE;		
	char  DataS[StrSize] = "";
	char  Address[StrSize] = "04";		
	UINT adds = W_MODE_SET;
	UINT data = SET_DATA_PC_READ_MODE;	
	JetAPI::IntToHex( adds, JET_USB_CTRL_ADDRESS_SIZE, Address);
	JetAPI::IntToHex( data, JET_USB_CTRL_DATA_SIZE, DataS);
	if( ExecUSBWrite(Address, DataS) == false ) 
	{	return false; }
	m_FPGAMode = FPGA_MODE_READ;
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::SetMode_PCAssign()//指定位址
{
#ifndef LIGHT_CTRL_DISABLE
	const int StrSize = JET_USB_TEXT_SIZE;		
	char  DataS[StrSize] = "";
	char  Address[StrSize] = "04";	
	UINT adds = W_MODE_SET;
	UINT data = SET_DATA_PC_ADDRESS_ASSIGN_MODE;	
	JetAPI::IntToHex( adds, JET_USB_CTRL_ADDRESS_SIZE, Address);
	JetAPI::IntToHex( data, JET_USB_CTRL_DATA_SIZE, DataS);
	if( ExecUSBWrite(Address, DataS) == false ) 
	{	return false; }
	m_FPGAMode = FPGA_MODE_ASSIGN;
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::SetTableRunCount(int nTable)//Trigger後要跑幾張TABLE
{
#ifndef LIGHT_CTRL_DISABLE
	//	if ( this->SetMode_PCWrite() == false ) {	return false;	}
	if ( nTable<0 || nTable>=MAX_TABLE_COUNT )
	{
		this->m_ErrorString.Format(_T("Error, Table Count Error!(%d)"), nTable);
		return false; 		
	}
	const int StrSize = JET_USB_TEXT_SIZE;		
	char  DataS[StrSize] = "";
	char  Address[StrSize] = "04";
	UINT adds = W_MODE_SET;
	UINT mode = SET_DATA_PC_WRITE_MODE;	
	UINT data = 0;	
	UINT Tempdata = 0x1f80;		//0001 1111 1000 0000	//0x1f80	
	UINT Ack = 0x40;			//0000 0000 0100 0000	//0x40	
	m_TableRunCount = nTable;

	//for 0A Address		
	//const bool bUse0A=true;
	const bool bSupportMoreThan64Table = GetSupportMoreThan64Table();
	if ( true == bSupportMoreThan64Table )//for Over 64 Tables
	{		
		adds = 0x0A;
		Tempdata = 0x01ff;	//0000 0001 1111 1111	//0xff80	
		Ack = 0x40;			//0000 0000 0100 0000	//0x40	
		strcpy(Address, "0A");
		data = (nTable)&Tempdata;		
		Tempdata = 0;//mode;
		//送給FPGA的bit順序是相反的
		if( data&0x0001 ) { Tempdata+=0x0100; }//1
		if( data&0x0002 ) { Tempdata+=0x0080; }//2
		if( data&0x0004 ) { Tempdata+=0x0040; }//3
		if( data&0x0008 ) { Tempdata+=0x0020; }//4
		if( data&0x0010 ) { Tempdata+=0x0010; }//5
		if( data&0x0020 ) { Tempdata+=0x0008; }//6
		if( data&0x0040 ) { Tempdata+=0x0004; }//7
		if( data&0x0080 ) { Tempdata+=0x0002; }//8	
		if( data&0x0100 ) { Tempdata+=0x0001; }//9
		data = Tempdata;
		JetAPI::IntToHex( adds, JET_USB_CTRL_ADDRESS_SIZE, Address);
		JetAPI::IntToHex( data, JET_USB_CTRL_DATA_SIZE, DataS);	
		if( ExecUSBWrite(Address, DataS) == false ) 
		{	return false; }	
	}	
/*
	Tempdata = 0x1f80;	//0001 1111 1000 0000	//0x1f80
	Ack = 0x40;			//0000 0000 0100 0000	//0x40		
	data = (nTable<<7)&Tempdata;		
	data += mode;
	JetAPI::IntToHex( adds, JET_USB_CTRL_ADDRESS_SIZE, Address);
	JetAPI::IntToHex( data, JET_USB_CTRL_DATA_SIZE, DataS);	
	if( ExecUSBWrite(Address, DataS) == false ) 
	{	return false; }
*/

	//for 04 Address
	adds = W_MODE_SET;
	strcpy(Address, "04");	
	Tempdata = 0x1f80;	//0001 1111 1000 0000	//0x1f80	
	Ack = 0x40;			//0000 0000 0100 0000	//0x40		
	data = (nTable<<7)&Tempdata;		
	Tempdata = mode;
	//送給FPGA的bit順序是相反的
	if( data&0x0080 ) { Tempdata+=0x1000; }
	if( data&0x0100 ) { Tempdata+=0x0800; }
	if( data&0x0200 ) { Tempdata+=0x0400; }
	if( data&0x0400 ) { Tempdata+=0x0200; }
	if( data&0x0800 ) { Tempdata+=0x0100; }
	if( data&0x1000 ) { Tempdata+=0x0080; }
	data = Tempdata;
	JetAPI::IntToHex( adds, JET_USB_CTRL_ADDRESS_SIZE, Address);
	JetAPI::IntToHex( data, JET_USB_CTRL_DATA_SIZE, DataS);	
	if( ExecUSBWrite(Address, DataS) == false ) 
	{	return false; }	

	//ACK L->H
	data += Ack;
	JetAPI::IntToHex( data, JET_USB_CTRL_DATA_SIZE, DataS);	
	if( ExecUSBWrite(Address, DataS) == false ) 
	{	return false; }	
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::SetCCDTrigEdge(int TrigEdge)//CCD Trigger Edge
{
#ifndef LIGHT_CTRL_DISABLE
//	if ( this->SetMode_PCWrite() == false ) {	return false;	}
	const int StrSize = JET_USB_TEXT_SIZE;		
	char  DataS[StrSize] = "";
	char  Address[StrSize] = "04";
	UINT adds = W_MODE_SET;
	UINT mode = SET_DATA_PC_WRITE_MODE;	
	UINT data = 0;	

	UINT Tempdata = 0xc000;		//1100 0000 0000 0000	//0xc000
	UINT Ack = 0x2000;			//0010 0000 0000 0000	//0x2000	
	if( TrigEdge == CCD_TRIG_EDGE_H )  
	{ data = Tempdata; }	//11  H Trigger
	else 
	{ data = 0; }			//00  L Trigger	
	data += mode;
	JetAPI::IntToHex( adds, JET_USB_CTRL_ADDRESS_SIZE, Address);
	JetAPI::IntToHex( data, JET_USB_CTRL_DATA_SIZE, DataS);	
	if( ExecUSBWrite(Address, DataS) == false ) 
	{	return false; }

	//ACK L->H
	data += Ack;
	JetAPI::IntToHex( data, JET_USB_CTRL_DATA_SIZE, DataS);	
	if( ExecUSBWrite(Address, DataS) == false ) 
	{	return false; }	
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::EnableDLPChannel(int DLPMask)//新增加的DLP開啟功能
{
#ifndef LIGHT_CTRL_DISABLE
//	if ( this->SetMode_PCWrite() == false ) {	return false;	}
	const int StrSize = JET_USB_TEXT_SIZE;		
	char  DataS[StrSize] = "";
	char  Address[StrSize] = "09";
	JetAPI::IntToHex(DLPMask, JET_USB_CTRL_DATA_SIZE, DataS);	
	if( ExecUSBWrite(Address, DataS) == false ) 
	{	return false; }
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::GetTableRunCount(int &nTable)
{
#ifndef LIGHT_CTRL_DISABLE
	int Value = 0;
	const int StrSize = JET_USB_TEXT_SIZE;		
	char  DataRS[StrSize] = "";			
	if ( ExecUSBRead("04", DataRS) == false ) 
	{	return false; }	//讀取
	const int Len = (int)::strlen(DataRS);
	JetAPI::HexToInt(DataRS, Len, Value);
	int tempValue = 0;
	//送給FPGA的bit順序是相反的
	if( Value&0x0080 ) { tempValue+=0x1000; }
	if( Value&0x0100 ) { tempValue+=0x0800; }
	if( Value&0x0200 ) { tempValue+=0x0400; }
	if( Value&0x0400 ) { tempValue+=0x0200; }
	if( Value&0x0800 ) { tempValue+=0x0100; }
	if( Value&0x1000 ) { tempValue+=0x0080; }
	Value = (tempValue>>7)&0x3f;
	nTable = Value;

//	int tempValue = Value>>7;
//	Value = tempValue&0x3f;
//	nTable = Value;
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::GetCCDTrigEdge(int &TrigEdge)
{
#ifndef LIGHT_CTRL_DISABLE	
	int Value = 0;
	const int StrSize = JET_USB_TEXT_SIZE;		
	char  DataRS[StrSize] = "";			
	if ( ExecUSBRead("04", DataRS) == false ) //讀取
	{	return false; }	
	const int Len = (int)::strlen(DataRS);
	JetAPI::HexToInt(DataRS, Len, Value);
	if( Value & 0xc000 ) { TrigEdge = CCD_TRIG_EDGE_H; }
	else				 { TrigEdge = CCD_TRIG_EDGE_L; }
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::SetTriggerFirstIndex(UINT Index)//處發表格第1張引數
{
#ifndef LIGHT_CTRL_DISABLE	
	if ( Index<0 || Index>=MAX_TABLE_COUNT )
	{	return false; }
	const int StrSize = JET_USB_TEXT_SIZE;		
	char Data[StrSize] = "";
	char Address[StrSize] = "07";

	const int TableSize = FPGA_TABLE_SIZE;
	const int TableAddress = Index*TableSize;//0a17(10進制), 0x11(16進制)	
	JetAPI::IntToHex(TableAddress, 4, Data);
	if( ExecUSBWrite(Address, Data) == false ) 
	{	return false; }
	::Sleep(10);
	if( SwitchToAssign() == false )	
	{	return false;	}	
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
int  CLightCtrlBoard::GetDLPTableStartIndex() const
{
	return m_DLPTableStartIndex;
}
//-------------------------------------------------------------------------------------//
void CLightCtrlBoard::SetDLPTableStartIndex(int val)
{
	m_DLPTableStartIndex = val;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::GetDLPInteralTrigger() const
{
	bool InteralTrig = true;
#ifndef LIGHT_CTRL_DISABLE
	const LIGHT_CTRL_BOARD_TYPE LightCtrlBoardType = GetLightCtrlBoardType();
	switch ( LightCtrlBoardType )
	{
	case LIGHT_CTRL_BOARD_3DA6: InteralTrig = false; break;
	case LIGHT_CTRL_BOARD_8DA1: InteralTrig = false; break;
	default: InteralTrig = true; break;
	}	
#else
	InteralTrig = true;
#endif//LIGHT_CTRL_DISABLE
	return InteralTrig;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::GetDLPMultiTableEnabled() const
{ 
	return m_DLPMultiTableEnabled; 
}
//-------------------------------------------------------------------------------------//
void CLightCtrlBoard::SetDLPMultiTableEnabled(bool val)
{	
	m_DLPMultiTableEnabled = val; 
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::GetSupportMoreThan64Table() const
{
	return m_SupportMoreThan64Table;
}
//-------------------------------------------------------------------------------------//
void CLightCtrlBoard::SetSupportMoreThan64Table(bool val)
{
	m_SupportMoreThan64Table = val;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::GetSupportModifyTriggerFirstIndex() const
{ 	
	return m_SupportModifyTriggerFirstIndex; 
}
//-------------------------------------------------------------------------------------//
void CLightCtrlBoard::SetSupportModifyTriggerFirstIndex(bool val)
{ 
	m_SupportModifyTriggerFirstIndex = val; 
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::TableRAMAssign(UINT AssignAddress)//指定RAM位址
{
#ifndef LIGHT_CTRL_DISABLE
	//指定RAM位址
	if( this->SetMode_PCAssign() == false ) 
	{	return false; }

	const int StrSize = JET_USB_TEXT_SIZE;		
	char  DataS[StrSize] = "";
	char  Address[StrSize] = "07";	
	int data = AssignAddress;	
	//if( data > 1023 ) //For 8DA2
	if( data > 8191 ) //For 8DA3, Un-Check
	{ 
		this->m_ErrorString.Format(_T("Error, RAM Address out of range!(%d)"), data);
		return false; 
	}
	JetAPI::IntToHex( data, JET_USB_CTRL_DATA_SIZE, DataS);	
	if( ExecUSBWrite(Address, DataS) == false ) 
	{	return false; }
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::TableAssign(UINT TableIndex)//指定RAM位址至某個Table的起始位址
{
#ifndef LIGHT_CTRL_DISABLE
	//14:舊表格大小, 17:新表格大小//20200109
	//指定RAM位址至某個Table的起始位址

	//1~14，15~28, ......
	//因為使用WRITE_RAM時會自動加1，所以指定位址時要先減1。 所以變成 0, 14, 28, ...

	//8DA2
	//1~17，17~34, ......
	//因為使用WRITE_RAM時會自動加1，所以指定位址時要先減1。 所以變成 0, 17, 34, ...
	int TableSize = FPGA_TABLE_SIZE;
	int Address = TableIndex*TableSize;
	return TableRAMAssign(Address);
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::TableRAMAddressAdd()//RAM位址加1
{
#ifndef LIGHT_CTRL_DISABLE
	//RAM位址加1，不需送data
	const int StrSize = JET_USB_TEXT_SIZE;		
	char  DataS[StrSize] = "";
	char  Address[StrSize] = "06";		
	JetAPI::IntToHex( W_READ_RAM_ADD, JET_USB_CTRL_ADDRESS_SIZE, Address);
	JetAPI::IntToHex( 0, JET_USB_CTRL_DATA_SIZE, DataS);	
	if( ExecUSBWrite(Address, DataS) == false ) 
	{	return false; }	
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::TableSingleWrite(TLCB_TRIG_TABLE &Table)//寫入一個Table
{
#ifndef LIGHT_CTRL_DISABLE
	const LIGHT_CTRL_BOARD_TYPE LightCtrlBoardType = GetLightCtrlBoardType();

	//寫入一個Table
	if( Table.sTableID<0 || Table.sTableID>=MAX_TABLE_COUNT )
	{
		this->m_ErrorString.Format(_T("Error, Table ID Error!(%d)"), Table.sTableID);
		return false; 
	}	
	if( TableAssign(Table.sTableID) == false ) 
	{	return false; }

	//if( SetMode_PCWrite() == false ) 
	if ( SwitchToWrite() == false )
	{	return false; }	

	UINT data = 0;
	UINT tmpData = 0;
	const int StrSize = JET_USB_TEXT_SIZE;		
	char  DataS[StrSize] = "";
	char  Address[StrSize] = "05";	
	JetAPI::IntToHex(W_TABLE_SET, 2, Address);	

//(1)
//	BYTE sTableType;		//(bit15~13)	//TABLE_TYPE_LED、TABLE_TYPE_DLP
//	BYTE sDLPTrigOutNumber;	//(bit12~8)		//DLP觸發數量
//	BYTE sDLPActiveChannel;	//(bit7~0)		//8個DLP每個Table最多只能啟動一個DLP
	data = 0;
	if( TABLE_TYPE_LED == Table.sTableType )
	{ tmpData = 1; }
	else
	{ tmpData = 2; }
	data = tmpData<<13;

	tmpData = Table.sDLPTrigOutNumber;
	data += (tmpData<<8);

	tmpData = Table.sDLPActiveChannel;
	if ( tmpData>0 && tmpData<DLP_CHANNEL_COUNT )
	{	tmpData = m_DLPChannelMap[tmpData];	}
	switch(tmpData)
	{
	case 1:	tmpData = 0x01; break;
	case 2:	tmpData = 0x02; break;
	case 3:	tmpData = 0x04; break;
	case 4:	tmpData = 0x08; break;
	case 5:	tmpData = 0x10; break;
	case 6:	tmpData = 0x20; break;
	case 7:	tmpData = 0x40; break;
	case 8:	tmpData = 0x80; break;
	default: tmpData = 0;	break;
	}
	data += (tmpData);	
	JetAPI::IntToHex( data, JET_USB_CTRL_DATA_SIZE, DataS);	
	if( ExecUSBWrite(Address, DataS) == false ) 
	{	return false; }

//(2)(3)
//	UINT sNextTableTime;	//(2)(bit15~0) (3)(bit31~16)	//Table間距時間, 第一個Table要設0
	//(bit15~0)
	data = Table.sNextTableTime&0xffff;		
	JetAPI::IntToHex( data, JET_USB_CTRL_DATA_SIZE, DataS);	
	if( ExecUSBWrite(Address, DataS) == false ) 
	{	return false; }
	//(bit31~16)
	data = Table.sNextTableTime&0xffff0000;
	data = data>>16;
	JetAPI::IntToHex( data, JET_USB_CTRL_DATA_SIZE, DataS);	
	if( ExecUSBWrite(Address, DataS) == false ) 
	{	return false; }

//(4)(5)
//	UINT sLEDTableTotalTime;	//(4)(bit15~0) (5)(bit31~16)	//LED Table總時間 (CCD延遲時間+CCD曝光時間)
	//(bit15~0)
	data = Table.sLEDTableTotalTime&0xffff;	
	JetAPI::IntToHex( data, JET_USB_CTRL_DATA_SIZE, DataS);	
	if( ExecUSBWrite(Address, DataS) == false ) 
	{	return false; }
	//(bit31~16)
	data = Table.sLEDTableTotalTime&0xffff0000;
	data = data>>16;
	JetAPI::IntToHex( data, JET_USB_CTRL_DATA_SIZE, DataS);	
	if( ExecUSBWrite(Address, DataS) == false ) 
	{	return false; }

//(6)
//	UINT sCCDDelayTime;			//燈亮至相機觸發的時間(us)	//DLP Type時，CCD觸發訊號的延遲時間(us)
	data = Table.sCCDDelayTime;
	JetAPI::IntToHex( data, JET_USB_CTRL_DATA_SIZE, DataS);	
	if( ExecUSBWrite(Address, DataS) == false ) 
	{	return false; }

//(7)
//	TLCB_LED_ITEM sPWM1;			//(bit7~0)	 bit7=>ON/OFF, 6~0=>Power(%)
//	TLCB_LED_ITEM sPWM2;			//(bit15~8)	 bit15=>ON/OFF, 14~8=>Power(%)
	//(bit7~0)
	TLCB_LED_ITEM *tmpPWM = &(Table.sPWM1);
	tmpData = tmpPWM->sPower&0x7f;
	if( tmpPWM->sIsON == true )	{ tmpData += 0x80; }
	data = tmpData;
	//(bit15~8)
	tmpPWM = &(Table.sPWM2);
	tmpData = tmpPWM->sPower&0x7f;
	if( tmpPWM->sIsON == true )	{ tmpData += 0x80; }
	data += (tmpData<<8);
	JetAPI::IntToHex( data, JET_USB_CTRL_DATA_SIZE, DataS);	
	if( ExecUSBWrite(Address, DataS) == false ) 
	{	return false; }

//(8)
//	TLCB_LED_ITEM sPWM3;			//(bit7~0)
//	TLCB_LED_ITEM sPWM4;			//(bit15~8)
	//(bit7~0)
	tmpPWM = &(Table.sPWM3);
	tmpData = tmpPWM->sPower&0x7f;
	if( tmpPWM->sIsON == true )	{ tmpData += 0x80; }
	data = tmpData;
	//(bit15~8)
	tmpPWM = &(Table.sPWM4);
	tmpData = tmpPWM->sPower&0x7f;
	if( tmpPWM->sIsON == true )	{ tmpData += 0x80; }
	data += (tmpData<<8);
	JetAPI::IntToHex( data, JET_USB_CTRL_DATA_SIZE, DataS);	
	if( ExecUSBWrite(Address, DataS) == false ) 
	{	return false; }

//(9)
//	TLCB_LED_ITEM sPWM5;			//(bit7~0)
//	TLCB_LED_ITEM sPWM6;			//(bit15~8)
	//(bit7~0)
	tmpPWM = &(Table.sPWM5);
	tmpData = tmpPWM->sPower&0x7f;
	if( tmpPWM->sIsON == true )	{ tmpData += 0x80; }
	data = tmpData;
	//(bit15~8)
	tmpPWM = &(Table.sPWM6);
	tmpData = tmpPWM->sPower&0x7f;
	if( tmpPWM->sIsON == true )	{ tmpData += 0x80; }
	data += (tmpData<<8);
	JetAPI::IntToHex( data, JET_USB_CTRL_DATA_SIZE, DataS);	
	if( ExecUSBWrite(Address, DataS) == false ) 
	{	return false; }

//(10)
//	TLCB_LED_ITEM sPWM7;			//(bit7~0)
//	TLCB_LED_ITEM sPWM8;			//(bit15~8)
	//(bit7~0)
	tmpPWM = &(Table.sPWM7);
	tmpData = tmpPWM->sPower&0x7f;
	if( tmpPWM->sIsON == true )	{ tmpData += 0x80; }
	data = tmpData;
	//(bit15~8)
	tmpPWM = &(Table.sPWM8);
	tmpData = tmpPWM->sPower&0x7f;
	if( tmpPWM->sIsON == true )	{ tmpData += 0x80; }
	data += (tmpData<<8);
	JetAPI::IntToHex( data, JET_USB_CTRL_DATA_SIZE, DataS);	
	if( ExecUSBWrite(Address, DataS) == false ) 
	{	return false; }

	if ( LIGHT_CTRL_BOARD_8DA1 == LightCtrlBoardType )
	{
	//(11-v2)
	//	TLCB_LED_ITEM sPWM9;			//(bit7~0)
	//	TLCB_LED_ITEM sPWM10;			//(bit15~8)
		//(bit7~0)
		tmpPWM = &(Table.sPWM9);
		tmpData = tmpPWM->sPower&0x7f;
		if( tmpPWM->sIsON == true )	{ tmpData += 0x80; }
		data = tmpData;
		//(bit15~8)
		tmpPWM = &(Table.sPWM10);
		tmpData = tmpPWM->sPower&0x7f;
		if( tmpPWM->sIsON == true )	{ tmpData += 0x80; }
		data += (tmpData<<8);
		JetAPI::IntToHex( data, JET_USB_CTRL_DATA_SIZE, DataS);	
		if( ExecUSBWrite(Address, DataS) == false ) 
		{	return false; }

	//(12-v2)
	//	TLCB_LED_ITEM sPWM11;			//(bit7~0)
	//	TLCB_LED_ITEM sPWM12;			//(bit15~8)
		//(bit7~0)
		tmpPWM = &(Table.sPWM11);
		tmpData = tmpPWM->sPower&0x7f;
		if( tmpPWM->sIsON == true )	{ tmpData += 0x80; }
		data = tmpData;
		//(bit15~8)
		tmpPWM = &(Table.sPWM12);
		tmpData = tmpPWM->sPower&0x7f;
		if( tmpPWM->sIsON == true )	{ tmpData += 0x80; }
		data += (tmpData<<8);
		JetAPI::IntToHex( data, JET_USB_CTRL_DATA_SIZE, DataS);	
		if( ExecUSBWrite(Address, DataS) == false ) 
		{	return false; }

		//(13-v2)
		data = Table.sCameraEnable;
		JetAPI::IntToHex( data, JET_USB_CTRL_DATA_SIZE, DataS);	
		if( ExecUSBWrite(Address, DataS) == false ) 
		{	return false; }
	}
	
//(11-v1), (14-v2)
//	UINT sDLPPulseTime;			//DLP Pulse Width的時間(us)
	data = Table.sDLPPulseTime;
	JetAPI::IntToHex( data, JET_USB_CTRL_DATA_SIZE, DataS);	
	if( ExecUSBWrite(Address, DataS) == false ) 
	{	return false; }

//(12-v1)(13-v1), (15-v2)(16-v2)
//	UINT sDLPCallbackTime;			//(12)(bit15~0) (13)(bit31~16)	//等待DLP Callback的時間(us)，超時會出現異常
	//(bit15~0)
	data = Table.sDLPCallbackTime&0xffff;	
	JetAPI::IntToHex( data, JET_USB_CTRL_DATA_SIZE, DataS);	
	if( ExecUSBWrite(Address, DataS) == false ) 
	{	return false; }

	//(bit31~16)
	data = Table.sDLPCallbackTime&0xffff0000;
	data = data>>16;
	JetAPI::IntToHex( data, JET_USB_CTRL_DATA_SIZE, DataS);	
	if( ExecUSBWrite(Address, DataS) == false ) 
	{	return false; }

//(14-v1), (17-v2)
//	UINT sDLPCCDExpTime;		//DLP的CCD曝光時間(us)
	data = Table.sDLPCCDExpTime;
	JetAPI::IntToHex( data, JET_USB_CTRL_DATA_SIZE, DataS);	
	if( ExecUSBWrite(Address, DataS) == false ) 
	{	return false; }
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::TableSingleRead(TLCB_TRIG_TABLE &Table)//讀出一個Table
{
#ifndef LIGHT_CTRL_DISABLE
	const LIGHT_CTRL_BOARD_TYPE LightCtrlBoardType = GetLightCtrlBoardType();

	//讀出一個Table	
	if( Table.sTableID<0 || Table.sTableID>=MAX_TABLE_COUNT )
	{
		this->m_ErrorString.Format(_T("Error, Table ID Error!(%d)"), Table.sTableID);
		return false; 
	}	
	//Address移至要讀取的Table
	if( TableAssign(Table.sTableID) == false ) 
	{	return false; }	

	//切換至讀取模式
	//if( SetMode_PCRead() == false ) 
	if ( SwitchToRead() == false )
	{	return false; }	
	
	const int StrSize = JET_USB_TEXT_SIZE;		
	char  DataRS[StrSize] = "";
	char  EmpData[StrSize] = "00";
	char  Address[StrSize] = "06";	
	int Len = 0;
	int Value = 0;
	UINT TempI = 0;

//(1)
//	BYTE sTableType;		//(bit15~13)	//TABLE_TYPE_LED、TABLE_TYPE_DLP
//	BYTE sDLPTrigOutNumber;	//(bit12~8)		//DLP觸發數量
//	BYTE sDLPActiveChannel;	//(bit7~0)		//8個DLP每個Table最多只能啟動一個DLP
	if( ExecUSBWrite(Address, EmpData) == false ) //Address +1
	{	return false; }	
	if ( ExecUSBRead(Address, DataRS) == false ) //讀取
	{	return false; }	
	Len = (int)::strlen(DataRS);
	JetAPI::HexToInt(DataRS, Len, Value);
	TempI = (Value&0xe000)>>13;
	if( TempI == TABLE_TYPE_LED )		{ Table.sTableType = TABLE_TYPE_LED; }
	else if( TempI == TABLE_TYPE_DLP )	{ Table.sTableType = TABLE_TYPE_DLP; }
	else								{ Table.sTableType = TABLE_TYPE_LED; }

	TempI = (Value&0x1f00)>>8;
	Table.sDLPTrigOutNumber = TempI;
	
	TempI = (Value&0x00ff);
	if( TempI&0x01 )		{ Table.sDLPActiveChannel = 1; }
	else if( TempI&0x02 )	{ Table.sDLPActiveChannel = 2; }
	else if( TempI&0x04 )	{ Table.sDLPActiveChannel = 3; }
	else if( TempI&0x08 )	{ Table.sDLPActiveChannel = 4; }
	else if( TempI&0x10 )	{ Table.sDLPActiveChannel = 5; }
	else if( TempI&0x20 )	{ Table.sDLPActiveChannel = 6; }
	else if( TempI&0x40 )	{ Table.sDLPActiveChannel = 7; }
	else if( TempI&0x80 )	{ Table.sDLPActiveChannel = 8; }
	else					{ Table.sDLPActiveChannel = 0; }	
		
//(2)(3)	//Table間距時間, 第一個Table要設0
	if( ExecUSBWrite(Address, EmpData) == false ) 
	{	return false; }	//Address +1
	if ( ExecUSBRead(Address, DataRS) == false ) 
	{	return false; }	//讀取
	Len = (int)::strlen(DataRS);
	JetAPI::HexToInt(DataRS, Len, Value);
	TempI = Value;
	
	if( ExecUSBWrite(Address, EmpData) == false ) 
	{	return false; }
	if ( ExecUSBRead(Address, DataRS) == false ) 
	{	return false; }
	Len = (int)::strlen(DataRS);
	JetAPI::HexToInt(DataRS, Len, Value);
	TempI += (Value<<16);
	Table.sNextTableTime = TempI;
		
//(4)(5)
//	UINT sLEDTableTotalTime;	//(4)(bit15~0) (5)(bit31~16)	//LED Table總時間 (CCD延遲時間+CCD曝光時間)
	if( ExecUSBWrite(Address, EmpData) == false ) 
	{	return false; }
	if ( ExecUSBRead(Address, DataRS) == false ) 
	{	return false; }
	Len = (int)::strlen(DataRS);
	JetAPI::HexToInt(DataRS, Len, Value);
	TempI = Value;
	
	if( ExecUSBWrite(Address, EmpData) == false ) 
	{	return false; }
	if ( ExecUSBRead(Address, DataRS) == false ) 
	{	return false; }
	Len = (int)::strlen(DataRS);
	JetAPI::HexToInt(DataRS, Len, Value);
	TempI += (Value<<16);
	Table.sLEDTableTotalTime = TempI;
		
//(6)
//	UINT sCCDDelayTime;			//燈亮至相機觸發的時間(us)	//DLP Type時，CCD觸發訊號的延遲時間(us)
	if( ExecUSBWrite(Address, EmpData) == false ) 
	{	return false; }
	if ( ExecUSBRead(Address, DataRS) == false ) 
	{	return false; }
	Len = (int)::strlen(DataRS);
	JetAPI::HexToInt(DataRS, Len, Value);
	Table.sCCDDelayTime = Value;
		
//(7)
//	TLCB_LED_ITEM sPWM1;			//(bit7~0)	 bit7=>ON/OFF, 6~0=>Power(%)
//	TLCB_LED_ITEM sPWM2;			//(bit15~8)	 bit15=>ON/OFF, 14~8=>Power(%)
	TLCB_LED_ITEM *tmpPWM = NULL;
	if( ExecUSBWrite(Address, EmpData) == false ) 
	{	return false; }
	if ( ExecUSBRead(Address, DataRS) == false ) 
	{	return false; }
	Len = (int)::strlen(DataRS);
	JetAPI::HexToInt(DataRS, Len, Value);
	tmpPWM = &(Table.sPWM1);
	TempI = Value&0x00ff;
	if( TempI&0x80 ) { tmpPWM->sIsON = true; } else { tmpPWM->sIsON = false; }
	tmpPWM->sPower = (TempI&0x7f);
	
	tmpPWM = &(Table.sPWM2);
	TempI = (Value&0xff00)>>8;
	if( TempI&0x80 ) { tmpPWM->sIsON = true; } else { tmpPWM->sIsON = false; }
	tmpPWM->sPower = (TempI&0x7f);
		
//(8)
	if( ExecUSBWrite(Address, EmpData) == false ) 
	{	return false; }
	if ( ExecUSBRead(Address, DataRS) == false ) 
	{	return false; }
	Len = (int)::strlen(DataRS);
	JetAPI::HexToInt(DataRS, Len, Value);
	tmpPWM = &(Table.sPWM3);
	TempI = Value&0x00ff;
	if( TempI&0x80 ) { tmpPWM->sIsON = true; } else { tmpPWM->sIsON = false; }
	tmpPWM->sPower = (TempI&0x7f);
	
	tmpPWM = &(Table.sPWM4);
	TempI = (Value&0xff00)>>8;
	if( TempI&0x80 ) { tmpPWM->sIsON = true; } else { tmpPWM->sIsON = false; }
	tmpPWM->sPower = (TempI&0x7f);
		
//(9)
	if( ExecUSBWrite(Address, EmpData) == false ) 
	{	return false; }
	if ( ExecUSBRead(Address, DataRS) == false ) 
	{	return false; }
	Len = (int)::strlen(DataRS);
	JetAPI::HexToInt(DataRS, Len, Value);
	tmpPWM = &(Table.sPWM5);
	TempI = Value&0x00ff;
	if( TempI&0x80 ) { tmpPWM->sIsON = true; } else { tmpPWM->sIsON = false; }
	tmpPWM->sPower = (TempI&0x7f);
	
	tmpPWM = &(Table.sPWM6);
	TempI = (Value&0xff00)>>8;
	if( TempI&0x80 ) { tmpPWM->sIsON = true; } else { tmpPWM->sIsON = false; }
	tmpPWM->sPower = (TempI&0x7f);
		
//(10)
	if( ExecUSBWrite(Address, EmpData) == false ) 
	{	return false; }
	if ( ExecUSBRead(Address, DataRS) == false ) 
	{	return false; }
	Len = (int)::strlen(DataRS);
	JetAPI::HexToInt(DataRS, Len, Value);
	tmpPWM = &(Table.sPWM7);
	TempI = Value&0x00ff;
	if( TempI&0x80 ) { tmpPWM->sIsON = true; } else { tmpPWM->sIsON = false; }
	tmpPWM->sPower = (TempI&0x7f);
	
	tmpPWM = &(Table.sPWM8);
	TempI = (Value&0xff00)>>8;
	if( TempI&0x80 ) { tmpPWM->sIsON = true; } else { tmpPWM->sIsON = false; }
	tmpPWM->sPower = (TempI&0x7f);
		
	if ( LIGHT_CTRL_BOARD_8DA1 == LightCtrlBoardType )
	{
	//(11-v2)
		if( ExecUSBWrite(Address, EmpData) == false ) 
		{	return false; }
		if ( ExecUSBRead(Address, DataRS) == false ) 
		{	return false; }
		Len = (int)::strlen(DataRS);
		JetAPI::HexToInt(DataRS, Len, Value);
		tmpPWM = &(Table.sPWM9);
		TempI = Value&0x00ff;
		if( TempI&0x80 ) { tmpPWM->sIsON = true; } else { tmpPWM->sIsON = false; }
		tmpPWM->sPower = (TempI&0x7f);
	
		tmpPWM = &(Table.sPWM10);
		TempI = (Value&0xff00)>>8;
		if( TempI&0x80 ) { tmpPWM->sIsON = true; } else { tmpPWM->sIsON = false; }
		tmpPWM->sPower = (TempI&0x7f);

	//(12-v2)
		if( ExecUSBWrite(Address, EmpData) == false ) 
		{	return false; }
		if ( ExecUSBRead(Address, DataRS) == false ) 
		{	return false; }
		Len = (int)::strlen(DataRS);
		JetAPI::HexToInt(DataRS, Len, Value);
		tmpPWM = &(Table.sPWM11);
		TempI = Value&0x00ff;
		if( TempI&0x80 ) { tmpPWM->sIsON = true; } else { tmpPWM->sIsON = false; }
		tmpPWM->sPower = (TempI&0x7f);
	
		tmpPWM = &(Table.sPWM12);
		TempI = (Value&0xff00)>>8;
		if( TempI&0x80 ) { tmpPWM->sIsON = true; } else { tmpPWM->sIsON = false; }
		tmpPWM->sPower = (TempI&0x7f);

	//(13-v2)
		//	UINT sCameraEnable;			//相機啟用通道
		if( ExecUSBWrite(Address, EmpData) == false ) 
		{	return false; }
		if ( ExecUSBRead(Address, DataRS) == false ) 
		{	return false; }
		Len = (int)::strlen(DataRS);
		JetAPI::HexToInt(DataRS, Len, Value);
		Table.sCameraEnable = Value;	
	}

//(11-v1), (14-v2)
//	UINT sDLPPulseTime;			//DLP Pulse Width的時間(us)
	if( ExecUSBWrite(Address, EmpData) == false ) 
	{	return false; }
	if ( ExecUSBRead(Address, DataRS) == false ) 
	{	return false; }
	Len = (int)::strlen(DataRS);
	JetAPI::HexToInt(DataRS, Len, Value);
	Table.sDLPPulseTime = Value;
		
//(12-v1)(13-v1), (15-v2)(16-v2)
//	UINT sDLPCallbackTime;			//(12)(bit15~0) (13)(bit31~16)	//等待DLP Callback的時間(us)，超時會出現異常
	if( ExecUSBWrite(Address, EmpData) == false ) 
	{	return false; }
	if ( ExecUSBRead(Address, DataRS) == false ) 
	{	return false; }
	Len = (int)::strlen(DataRS);
	JetAPI::HexToInt(DataRS, Len, Value);
	TempI = Value;
	
	if( ExecUSBWrite(Address, EmpData) == false ) 
	{	return false; }
	if ( ExecUSBRead(Address, DataRS) == false ) 
	{	return false; }
	Len = (int)::strlen(DataRS);
	JetAPI::HexToInt(DataRS, Len, Value);
	TempI += (Value<<16);
	Table.sDLPCallbackTime = TempI;
		
//(14-v1), (17-v2)
//	UINT sDLPCCDExpTime;		//DLP的CCD曝光時間(us)
	if( ExecUSBWrite(Address, EmpData) == false ) 
	{	return false; }
	if ( ExecUSBRead(Address, DataRS) == false ) 
	{	return false; }
	Len = (int)::strlen(DataRS);
	JetAPI::HexToInt(DataRS, Len, Value);
	Table.sDLPCCDExpTime = Value;

#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::TableCompare(const TLCB_TRIG_TABLE &Table1, const TLCB_TRIG_TABLE &Table2)//比較2個Table
{
	const LIGHT_CTRL_BOARD_TYPE LightCtrlBoardType = GetLightCtrlBoardType();

	if ( Table1.sTableID != Table2.sTableID ) { return false; }

	if ( Table1.sTableType != Table2.sTableType ) { return false; }
	if ( Table1.sDLPTrigOutNumber != Table2.sDLPTrigOutNumber ) { return false; }
	if ( Table1.sDLPActiveChannel != Table2.sDLPActiveChannel ) { return false; }
	if ( Table1.sDLPPhasePatMode != Table2.sDLPPhasePatMode ) { return false; }

	if ( Table1.sNextTableTime != Table2.sNextTableTime ) { return false; }

	if ( Table1.sLEDTableTotalTime != Table2.sLEDTableTotalTime ) { return false; }


	if ( Table1.sCCDDelayTime != Table2.sCCDDelayTime ) { return false; }

	if ( Table1.sPWM1.sIsON != Table2.sPWM1.sIsON ) { return false; }
	if ( Table1.sPWM1.sPower != Table2.sPWM1.sPower ) { return false; }
	if ( Table1.sPWM2.sIsON != Table2.sPWM2.sIsON ) { return false; }
	if ( Table1.sPWM2.sPower != Table2.sPWM2.sPower ) { return false; }
	if ( Table1.sPWM3.sIsON != Table2.sPWM3.sIsON ) { return false; }
	if ( Table1.sPWM3.sPower != Table2.sPWM3.sPower ) { return false; }
	if ( Table1.sPWM4.sIsON != Table2.sPWM4.sIsON ) { return false; }
	if ( Table1.sPWM4.sPower != Table2.sPWM4.sPower ) { return false; }
	if ( Table1.sPWM5.sIsON != Table2.sPWM5.sIsON ) { return false; }
	if ( Table1.sPWM5.sPower != Table2.sPWM5.sPower ) { return false; }
	if ( Table1.sPWM6.sIsON != Table2.sPWM6.sIsON ) { return false; }
	if ( Table1.sPWM6.sPower != Table2.sPWM6.sPower ) { return false; }
	if ( Table1.sPWM7.sIsON != Table2.sPWM7.sIsON ) { return false; }
	if ( Table1.sPWM7.sPower != Table2.sPWM7.sPower ) { return false; }
	if ( Table1.sPWM8.sIsON != Table2.sPWM8.sIsON ) { return false; }
	if ( Table1.sPWM8.sPower != Table2.sPWM8.sPower ) { return false; }

	if ( LIGHT_CTRL_BOARD_8DA1 == LightCtrlBoardType )
	{
		if ( Table1.sPWM9.sIsON != Table2.sPWM9.sIsON ) { return false; }
		if ( Table1.sPWM9.sPower != Table2.sPWM9.sPower ) { return false; }
		if ( Table1.sPWM10.sIsON != Table2.sPWM10.sIsON ) { return false; }
		if ( Table1.sPWM10.sPower != Table2.sPWM10.sPower ) { return false; }
		if ( Table1.sPWM11.sIsON != Table2.sPWM11.sIsON ) { return false; }
		if ( Table1.sPWM11.sPower != Table2.sPWM11.sPower ) { return false; }
		if ( Table1.sPWM12.sIsON != Table2.sPWM12.sIsON ) { return false; }
		if ( Table1.sPWM12.sPower != Table2.sPWM12.sPower ) { return false; }

		if ( Table1.sCameraEnable != Table2.sCameraEnable ) { return false; }	
	}
	
	if ( Table1.sDLPPulseTime != Table2.sDLPPulseTime ) { return false; }

	if ( Table1.sDLPCallbackTime != Table2.sDLPCallbackTime ) { return false; }
	if ( Table1.sDLPCCDExpTime != Table2.sDLPCCDExpTime ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::TableListRead(std::vector<TLCB_TRIG_TABLE> &TableList, int nTable)
{
	TableList.clear();
#ifndef LIGHT_CTRL_DISABLE
	//return table index
	if ( ClearAllCount() == false ) 
	{	return false; }

	int             i=0;
	CString         str;
	double          fnTime=0;
	LARGE_INTEGER   fnEnd;
	LARGE_INTEGER   fnStart;
	TLCB_TRIG_TABLE tmpTable;

	str.Format(_T("CLightCtrlBoard::TableListRead[%d] Start"), nTable);
	AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);
	JetAPI::SetFuncTimeStart(fnStart);
	for( i=0 ; i<nTable ; i++ )
	{
		this->DefaultTable(tmpTable);
		tmpTable.sTableID = i;
		if ( TableSingleRead(tmpTable) == false ) 
		{	return false; }
		TableList.push_back(tmpTable);
	}
	//return table index
	if ( ClearAllCount() == false ) 
	{	return false; }

	JetAPI::SetFuncTimeEnd(fnEnd);
	fnTime = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);//計算函式經過時間
	str.Format(_T("CLightCtrlBoard::TableListRead[%d] Time=%.3f ms"), nTable, fnTime);
	AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::TableListWrite(const std::vector<TLCB_TRIG_TABLE> &TableList)
{
#ifndef LIGHT_CTRL_DISABLE
	const int PreCheckTableList = GetPreCheckTableListEnabled();
	//return table index
	if ( ClearAllCount() == false ) 
	{	return false; }	
	
	if ( FN_ENABLE == PreCheckTableList )
	{
		if ( TableListCompare(TableList) == true ) 
		{	return true; }
	}

	CString str;
	size_t  i=0;
	int     tmpData=0;
	DWORD   DLPEnableMask=0;	
	TLCB_TRIG_TABLE tmpTable;
	TLCB_TRIG_TABLE tmpTableA;
	TLCB_TRIG_TABLE tmpTableB;	
	double          fnTime=0;
	LARGE_INTEGER   fnEnd;
	LARGE_INTEGER   fnStart;
	std::vector<TLCB_TRIG_TABLE>  TableListTotal = TableList;
	const size_t nTable = TableList.size();		

	str.Format(_T("CLightCtrlBoard::TableListWrite[%d] Start"), nTable);
	AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);
	JetAPI::SetFuncTimeStart(fnStart);
	//將後面給予填入空的表格
	DefaultTable(tmpTable);
	const size_t MaxTableCount = MAX_TABLE_COUNT;
	for ( i=nTable; i<MaxTableCount; i++ )
	{
		tmpTable.sTableID = i;
		TableListTotal.push_back(tmpTable);
	}
	
	const size_t NTableWrite=nTable;//MaxTableCount	//只寫入要的資料
	for ( i=0; i<NTableWrite; i++)
	{
		//tmpTable = TableList[i];
		tmpTable = TableListTotal[i];
		tmpTable.sTableID = i;
		if ( TableSingleWrite(tmpTable) == false ) 
		{	return false; }		

		if ( TABLE_TYPE_DLP == tmpTable.sTableType )
		{
			tmpData = tmpTable.sDLPActiveChannel;
			if ( tmpData>0 && tmpData<DLP_CHANNEL_COUNT )
			{	tmpData = m_DLPChannelMap[tmpData];	}
			switch ( tmpData )
			{
			case 1:	DLPEnableMask |= 0x01;	break;
			case 2:	DLPEnableMask |= 0x02;	break;
			case 3:	DLPEnableMask |= 0x04;	break;
			case 4:	DLPEnableMask |= 0x08;	break;
			case 5:	DLPEnableMask |= 0x10;	break;
			case 6:	DLPEnableMask |= 0x20;	break;
			case 7:	DLPEnableMask |= 0x40;	break;
			case 8:	DLPEnableMask |= 0x80;	break;
			}
		}
		else
		{	i = i; }
	}

	m_LCBTableList = TableList;
	//sDLPActiveChannel	
	SetDLPEnable(0);

	if ( EnableDLPChannel(DLPEnableMask) == false )
	{	return false;	}	

	//return table index
	if ( ClearAllCount() == false ) 
	{	return false; }

	JetAPI::SetFuncTimeEnd(fnEnd);
	fnTime = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);//計算函式經過時間
	str.Format(_T("CLightCtrlBoard::TableListWrite[%d] Time=%.3f ms"), NTableWrite, fnTime);
	AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::TableListCompare(const std::vector<TLCB_TRIG_TABLE> &TableList)
{	
	const size_t TableCount1 = TableList.size();
	const size_t TableCount2 = m_LCBTableList.size();
	if ( TableCount1 != TableCount2 ) { return false; }

	CString         str;
	size_t          i=0;
	TLCB_TRIG_TABLE Table1;
	TLCB_TRIG_TABLE Table2;
	double          fnTime=0;
	LARGE_INTEGER   fnEnd;
	LARGE_INTEGER   fnStart;

	str.Format(_T("CLightCtrlBoard::TableListCompare[%d] Start"), TableCount1);
	AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);
	JetAPI::SetFuncTimeStart(fnStart);

	for ( i=0; i<TableCount2; i++ )
	{
		Table1 = TableList[i];
		Table2 = m_LCBTableList[i];

		if ( TableCompare(Table1, Table2) == false )
		{	return false; }
	}

	JetAPI::SetFuncTimeEnd(fnEnd);
	fnTime = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);//計算函式經過時間
	str.Format(_T("CLightCtrlBoard::TableListCompare[%d] Time=%.3f ms"), TableCount1, fnTime);
	AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::ReMapTableList_ReadToWrite(const std::vector<TLCB_TRIG_TABLE> &TableListRead, std::vector<TLCB_TRIG_TABLE> &TableListWrite)
{	//因為DLP的Channel是查表的所以要反查回去
	int       j=0;
	size_t    i=0;
	int       DLPChannel=0;
	TLCB_TRIG_TABLE Table;
	const size_t ReadTableCount = TableListRead.size();

	TableListWrite.clear();
	for ( i=0; i<ReadTableCount; i++ )
	{
		Table = (TableListRead[i]);
		if ( Table.sDLPActiveChannel > 0 ) 
		{ 
			DLPChannel = Table.sDLPActiveChannel;
			for ( j=1; j<DLP_CHANNEL_COUNT; j++ )
			{
				if ( DLPChannel == m_DLPChannelMap[j] ) 
				{	
					DLPChannel = j;
					break; 
				}
			}
			Table.sDLPActiveChannel = DLPChannel;
		}
		TableListWrite.push_back(Table);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::TableReset(int nTable)
{
#ifndef LIGHT_CTRL_DISABLE
	if( nTable <= 0 )  { return true; }
	//return table index
	if( ClearAllCount() == false ) { return false; }

	int i=0;
	TLCB_TRIG_TABLE tmpTable;
	if ( nTable > MAX_TABLE_COUNT ) 
	{	nTable = MAX_TABLE_COUNT; }

	for( i=0 ; i<nTable ; i++ )
	{
		this->DefaultTable(tmpTable);
		tmpTable.sTableID = i;
		if( TableSingleWrite(tmpTable) == false ) 
		{	return false; }
	}
	
	SetDLPEnable(0);
	EnableDLPChannel(0);
	ClearLCBTableList();

	//return table index
	if( ClearAllCount() == false ) { return false; }
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::ClearLCBTableList()//清空內部的列表
{
	m_LCBTableList.clear();
	return true;
}
//-------------------------------------------------------------------------------------//
int CLightCtrlBoard::GetCurFPGAMode()
{
	return this->m_FPGAMode;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::DefaultTable(TLCB_TRIG_TABLE &Table)
{
	TLCB_LED_ITEM DefaultPWM;
	DefaultPWM.sIsON = false;
	DefaultPWM.sPower = 0;

	Table.sTableID = -1;
//(1)
	Table.sTableType = TABLE_TYPE_LED;	//TABLE_TYPE_LED、TABLE_TYPE_DLP
	Table.sDLPTrigOutNumber = 0;			//DLP觸發數量
	Table.sDLPActiveChannel = 0;			//8個DLP每個Table最多只能啟動一個DLP	(DLP_CHANNEL_1~8)
	Table.sDLPPhasePatMode = 0;

//(2)(3)
	Table.sNextTableTime = 0;				//Table間距時間, 第一個Table要設0

//(4)(5)
	Table.sLEDTableTotalTime=0;		//LED Table總時間 (CCD延遲時間+CCD曝光時間)

//(6)
	Table.sCCDDelayTime=0;				//燈亮至相機觸發的時間(us)	//DLP Type時，CCD觸發訊號的延遲時間(us)

//(7)
	Table.sPWM1=DefaultPWM;				//(bit7~0)	 bit7=>ON/OFF, 6~0=>Power(%)
	Table.sPWM2=DefaultPWM;				//(bit15~8)	 bit15=>ON/OFF, 14~8=>Power(%)
//(8)
	Table.sPWM3=DefaultPWM;				//(bit7~0)
	Table.sPWM4=DefaultPWM;				//(bit15~8)
//(9)
	Table.sPWM5=DefaultPWM;				//(bit7~0)
	Table.sPWM6=DefaultPWM;				//(bit15~8)
//(10)
	Table.sPWM7=DefaultPWM;				//(bit7~0)
	Table.sPWM8=DefaultPWM;				//(bit15~8)

//(11-v2)
	Table.sPWM9=DefaultPWM;				//(bit7~0)
	Table.sPWM10=DefaultPWM;			//(bit15~8)
//(12-v2)
	Table.sPWM11=DefaultPWM;			//(bit7~0)
	Table.sPWM12=DefaultPWM;			//(bit15~8)
//(13-v2)
	Table.sCameraEnable=0x01;			    //(bit7~0)

//(11-v1), (14-v2)
	Table.sDLPPulseTime=GetDLPPulseWidthTimeus();			//DLP Pulse Width的時間(us)-20
//(12-v1)(13-v1), (15-v1)(16-v1)
	Table.sDLPCallbackTime=65535;	//(12)(bit15~0) (13)(bit31~16)	//等待DLP Callback的時間(us)，超時會出現異常 - 20000
//(14-v1), (17-v1)
	Table.sDLPCCDExpTime=6000;		//DLP的CCD曝光時間(us)-6000
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::GetTableInfoText(TLCB_TRIG_TABLE &Table, CString &Info)
{
	const LIGHT_CTRL_BOARD_TYPE LightCtrlBoardType = GetLightCtrlBoardType();

	Info = _T("");
	CString tmpStr = _T("");
	tmpStr.Format(_T("ID%d"), Table.sTableID);
	Info += tmpStr;
//Type
	int TableType = Table.sTableType;
	if( TableType == TABLE_TYPE_LED )		{ tmpStr = _T("LED"); }
	else if( TableType == TABLE_TYPE_DLP )	{ tmpStr = _T("DLP"); }
	else									{ tmpStr = _T("???"); return false; }
	Info += _T(" , ");
	Info += tmpStr;

//CCD Delay
	tmpStr.Format(_T("CCD Delay(%5d)"), Table.sCCDDelayTime);	
	Info += _T(" , ");
	Info += tmpStr;
//CCD Exp
	if( TableType == TABLE_TYPE_LED )
	{ tmpStr.Format(_T("Exp(%5d)"), Table.sLEDTableTotalTime-Table.sCCDDelayTime); }
	else if( TableType == TABLE_TYPE_DLP ) 
	{ tmpStr.Format(_T("Exp(%5d)"), Table.sDLPCCDExpTime); }
	Info += _T(" , ");
	Info += tmpStr;
//Next T
	tmpStr.Format(_T("NextT(%5d)"), Table.sNextTableTime);
	Info += _T(" , ");
	Info += tmpStr;

	tmpStr = _T("");
	if( TableType == TABLE_TYPE_LED )
	{
		Info += _T(" , PWM ");

		int nLED = 0;
		if( Table.sPWM1.sIsON == true ) { tmpStr.Format(_T(",01(%2d)"), Table.sPWM1.sPower); Info += tmpStr; nLED++; }
		if( Table.sPWM2.sIsON == true ) { tmpStr.Format(_T(",02(%2d)"), Table.sPWM2.sPower); Info += tmpStr; nLED++; }
		if( Table.sPWM3.sIsON == true ) { tmpStr.Format(_T(",03(%2d)"), Table.sPWM3.sPower); Info += tmpStr; nLED++; }
		if( Table.sPWM4.sIsON == true ) { tmpStr.Format(_T(",04(%2d)"), Table.sPWM4.sPower); Info += tmpStr; nLED++; }
		if( Table.sPWM5.sIsON == true ) { tmpStr.Format(_T(",05(%2d)"), Table.sPWM5.sPower); Info += tmpStr; nLED++; }
		if( Table.sPWM6.sIsON == true ) { tmpStr.Format(_T(",06(%2d)"), Table.sPWM6.sPower); Info += tmpStr; nLED++; }
		if( Table.sPWM7.sIsON == true ) { tmpStr.Format(_T(",07(%2d)"), Table.sPWM7.sPower); Info += tmpStr; nLED++; }
		if( Table.sPWM8.sIsON == true ) { tmpStr.Format(_T(",08(%2d)"), Table.sPWM8.sPower); Info += tmpStr; nLED++; }

		if ( LIGHT_CTRL_BOARD_8DA1 == LightCtrlBoardType )
		{
			if( Table.sPWM9.sIsON == true ) { tmpStr.Format(_T(",09(%2d)"), Table.sPWM9.sPower); Info += tmpStr; nLED++; }
			if( Table.sPWM10.sIsON == true ) { tmpStr.Format(_T(",10(%2d)"), Table.sPWM10.sPower); Info += tmpStr; nLED++; }
			if( Table.sPWM11.sIsON == true ) { tmpStr.Format(_T(",11(%2d)"), Table.sPWM11.sPower); Info += tmpStr; nLED++; }
			if( Table.sPWM12.sIsON == true ) { tmpStr.Format(_T(",12(%2d)"), Table.sPWM12.sPower); Info += tmpStr; nLED++; }
		}	

		if( nLED == 0 )
		{ Info += _T("EMPTY"); }
	}
	else
	{
//DLP CH
		tmpStr.Format(_T("CH(%02d)"), Table.sDLPActiveChannel);
		Info += _T(" , ");
		Info += tmpStr;
//DLP nTrig.
		tmpStr.Format(_T("nTrig.(%d)"), Table.sDLPTrigOutNumber);
		Info += _T(" , ");
		Info += tmpStr;
//DLP Time
		tmpStr.Format(_T("DLPT(%5d)"), Table.sDLPPulseTime);
		Info += _T(" , ");
		Info += tmpStr;
//Over Time
		tmpStr.Format(_T("OverT(%5d)"), Table.sDLPCallbackTime);
		Info += _T(" , ");
		Info += tmpStr;		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::CheckTableListEqually(const std::vector<TLCB_TRIG_TABLE> &TableListIn, const std::vector<TLCB_TRIG_TABLE> &TableListOut, std::vector<int> &NGTableList)//確認兩個表格是否相等
{
	int  i=0;
	CString ErrorStr;
	TLCB_TRIG_TABLE tmpTableIn;
	TLCB_TRIG_TABLE tmpTableOut;
	const int TableCountIn = (int)(TableListIn.size());
	const int TableCountOut = (int)(TableListOut.size());
	NGTableList.clear();
	//if ( TableCountIn != TableCountOut )
	//{
	//	m_ErrorString.Format(_T("Error, In/Out Table Count Exception (%d, %d)"), TableCountIn, TableCountOut);
	//	return false;
	//}
	const int TableCount = MIN(TableCountIn, TableCountOut);
	for ( i=0; i<TableCount; i++ )
	{
		tmpTableIn = TableListIn[i];
		tmpTableOut = TableListOut[i];
		if ( CheckTableEqually(i, tmpTableIn, tmpTableOut) == false )
		{
			if ( 0 == NGTableList.size() )
			{	ErrorStr = GetErrorString();	}
			NGTableList.push_back(i); 
		}			
	}	
	const size_t NGTableCount = NGTableList.size();
	if ( NGTableCount > 0 ) 
	{	
		m_ErrorString = ErrorStr;
		return false; 
	}

	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::CheckTableEqually(size_t idx, const TLCB_TRIG_TABLE &TableIn, const TLCB_TRIG_TABLE &TableOut)//確認兩個表格是否相等
{
	const LIGHT_CTRL_BOARD_TYPE LightCtrlBoardType = GetLightCtrlBoardType();

	idx = idx+1;
	if ( TableIn.sTableID != TableOut.sTableID ) 
	{ 
		m_ErrorString.Format(_T("Error, In/Out Table[%d] ID Exception (%d, %d)"), idx, TableIn.sTableID, TableOut.sTableID);
		return false; 
	}
	if ( TableIn.sTableType != TableOut.sTableType )
	{ 
		m_ErrorString.Format(_T("Error, In/Out Table[%d] Type Exception (%d, %d)"), idx, TableIn.sTableType, TableOut.sTableType);
		return false; 
	}
	if ( TableIn.sDLPTrigOutNumber != TableOut.sDLPTrigOutNumber )
	{ 
		m_ErrorString.Format(_T("Error, In/Out Table[%d] DLPTrigOutNumber Exception (%d, %d)"), idx, TableIn.sDLPTrigOutNumber, TableOut.sDLPTrigOutNumber);
		return false; 
	}
	
	int DLPChannelA = TableIn.sDLPActiveChannel;
	if ( DLPChannelA < DLP_CHANNEL_COUNT )
	{	DLPChannelA = m_DLPChannelMap[DLPChannelA];	}//因為寫入的DLP編號會跳號
	if ( DLPChannelA != TableOut.sDLPActiveChannel )
	{ 
		m_ErrorString.Format(_T("Error, In/Out Table[%d] DLPActiveChannel Exception (%d, %d)"), idx, DLPChannelA, TableOut.sDLPActiveChannel);
		return false; 
	}
	//if ( TableIn.sDLPPhasePatMode != TableOut.sDLPPhasePatMode )//並非寫入CtrlBoard內
	//{ 
	//	m_ErrorString.Format(_T("Error, In/Out Table[%d] DLPPhasePatMode Exception (%d, %d)"), idx, TableIn.sDLPPhasePatMode, TableOut.sDLPPhasePatMode);
	//	return false; 
	//}
	if ( TableIn.sNextTableTime != TableOut.sNextTableTime )
	{ 
		m_ErrorString.Format(_T("Error, In/Out Table[%d] NextTableTime Exception (%d, %d)"), idx, TableIn.sNextTableTime, TableOut.sNextTableTime);
		return false; 
	}
	if ( TableIn.sLEDTableTotalTime != TableOut.sLEDTableTotalTime )
	{ 
		m_ErrorString.Format(_T("Error, In/Out Table[%d] LEDTableTotalTime Exception (%d, %d)"), idx, TableIn.sLEDTableTotalTime, TableOut.sLEDTableTotalTime);
		return false; 
	}
	if ( TableIn.sCCDDelayTime != TableOut.sCCDDelayTime )
	{ 
		m_ErrorString.Format(_T("Error, In/Out Table[%d] CCDDelayTime Exception (%d, %d)"), idx, TableIn.sCCDDelayTime, TableOut.sCCDDelayTime);
		return false; 
	}

	if ( TableIn.sPWM1.sIsON != TableOut.sPWM1.sIsON )
	{ 
		m_ErrorString.Format(_T("Error, In/Out Table[%d] PWM1.sIsON Exception (%d, %d)"), idx, TableIn.sPWM1.sIsON, TableOut.sPWM1.sIsON);
		return false; 
	}
	if ( TableIn.sPWM1.sPower != TableOut.sPWM1.sPower )
	{ 
		m_ErrorString.Format(_T("Error, In/Out Table[%d] PWM1.sPower Exception (%d, %d)"), idx, TableIn.sPWM1.sPower, TableOut.sPWM1.sPower);
		return false; 
	}
	
	if ( TableIn.sPWM2.sIsON != TableOut.sPWM2.sIsON )
	{ 
		m_ErrorString.Format(_T("Error, In/Out Table[%d] PWM2.sIsON Exception (%d, %d)"), idx, TableIn.sPWM2.sIsON, TableOut.sPWM2.sIsON);
		return false; 
	}
	if ( TableIn.sPWM2.sPower != TableOut.sPWM2.sPower )
	{ 
		m_ErrorString.Format(_T("Error, In/Out Table[%d] PWM2.sPower Exception (%d, %d)"), idx, TableIn.sPWM2.sPower, TableOut.sPWM2.sPower);
		return false; 
	}

	if ( TableIn.sPWM3.sIsON != TableOut.sPWM3.sIsON )
	{ 
		m_ErrorString.Format(_T("Error, In/Out Table[%d] PWM3.sIsON Exception (%d, %d)"), idx, TableIn.sPWM3.sIsON, TableOut.sPWM3.sIsON);
		return false; 
	}
	if ( TableIn.sPWM3.sPower != TableOut.sPWM3.sPower )
	{ 
		m_ErrorString.Format(_T("Error, In/Out Table[%d] PWM3.sPower Exception (%d, %d)"), idx, TableIn.sPWM3.sPower, TableOut.sPWM3.sPower);
		return false; 
	}

	if ( TableIn.sPWM4.sIsON != TableOut.sPWM4.sIsON )
	{ 
		m_ErrorString.Format(_T("Error, In/Out Table[%d] PWM4.sIsON Exception (%d, %d)"), idx, TableIn.sPWM4.sIsON, TableOut.sPWM4.sIsON);
		return false; 
	}
	if ( TableIn.sPWM4.sPower != TableOut.sPWM4.sPower )
	{ 
		m_ErrorString.Format(_T("Error, In/Out Table[%d] PWM4.sPower Exception (%d, %d)"), idx, TableIn.sPWM4.sPower, TableOut.sPWM4.sPower);
		return false; 
	}

	if ( TableIn.sPWM5.sIsON != TableOut.sPWM5.sIsON )
	{ 
		m_ErrorString.Format(_T("Error, In/Out Table[%d] PWM5.sIsON Exception (%d, %d)"), idx, TableIn.sPWM5.sIsON, TableOut.sPWM5.sIsON);
		return false; 
	}
	if ( TableIn.sPWM5.sPower != TableOut.sPWM5.sPower )
	{ 
		m_ErrorString.Format(_T("Error, In/Out Table[%d] PWM5.sPower Exception (%d, %d)"), idx, TableIn.sPWM5.sPower, TableOut.sPWM5.sPower);
		return false; 
	}

	if ( TableIn.sPWM6.sIsON != TableOut.sPWM6.sIsON )
	{ 
		m_ErrorString.Format(_T("Error, In/Out Table[%d] PWM6.sIsON Exception (%d, %d)"), idx, TableIn.sPWM6.sIsON, TableOut.sPWM6.sIsON);
		return false; 
	}
	if ( TableIn.sPWM6.sPower != TableOut.sPWM6.sPower )
	{ 
		m_ErrorString.Format(_T("Error, In/Out Table[%d] PWM6.sPower Exception (%d, %d)"), idx, TableIn.sPWM6.sPower, TableOut.sPWM6.sPower);
		return false; 
	}

	if ( TableIn.sPWM7.sIsON != TableOut.sPWM7.sIsON )
	{ 
		m_ErrorString.Format(_T("Error, In/Out Table[%d] PWM7.sIsON Exception (%d, %d)"), idx, TableIn.sPWM7.sIsON, TableOut.sPWM7.sIsON);
		return false; 
	}
	if ( TableIn.sPWM7.sPower != TableOut.sPWM7.sPower )
	{ 
		m_ErrorString.Format(_T("Error, In/Out Table[%d] PWM7.sPower Exception (%d, %d)"), idx, TableIn.sPWM7.sPower, TableOut.sPWM7.sPower);
		return false; 
	}

	if ( TableIn.sPWM8.sIsON != TableOut.sPWM8.sIsON )
	{ 
		m_ErrorString.Format(_T("Error, In/Out Table[%d] PWM8.sIsON Exception (%d, %d)"), idx, TableIn.sPWM8.sIsON, TableOut.sPWM8.sIsON);
		return false; 
	}
	if ( TableIn.sPWM8.sPower != TableOut.sPWM8.sPower )
	{ 
		m_ErrorString.Format(_T("Error, In/Out Table[%d] PWM8.sPower Exception (%d, %d)"), idx, TableIn.sPWM8.sPower, TableOut.sPWM8.sPower);
		return false; 
	}

	if ( LIGHT_CTRL_BOARD_8DA1 == LightCtrlBoardType )
	{
		if ( TableIn.sPWM9.sIsON != TableOut.sPWM9.sIsON )
		{ 
			m_ErrorString.Format(_T("Error, In/Out Table[%d] sPWM9.sIsON Exception (%d, %d)"), idx, TableIn.sPWM9.sIsON, TableOut.sPWM9.sIsON);
			return false; 
		}
		if ( TableIn.sPWM9.sPower != TableOut.sPWM9.sPower )
		{ 
			m_ErrorString.Format(_T("Error, In/Out Table[%d] sPWM9.sPower Exception (%d, %d)"), idx, TableIn.sPWM9.sPower, TableOut.sPWM9.sPower);
			return false; 
		}

		if ( TableIn.sPWM10.sIsON != TableOut.sPWM10.sIsON )
		{ 
			m_ErrorString.Format(_T("Error, In/Out Table[%d] sPWM10.sIsON Exception (%d, %d)"), idx, TableIn.sPWM10.sIsON, TableOut.sPWM10.sIsON);
			return false; 
		}
		if ( TableIn.sPWM10.sPower != TableOut.sPWM10.sPower )
		{ 
			m_ErrorString.Format(_T("Error, In/Out Table[%d] sPWM10.sPower Exception (%d, %d)"), idx, TableIn.sPWM10.sPower, TableOut.sPWM10.sPower);
			return false; 
		}

		if ( TableIn.sPWM11.sIsON != TableOut.sPWM11.sIsON )
		{ 
			m_ErrorString.Format(_T("Error, In/Out Table[%d] sPWM11.sIsON Exception (%d, %d)"), idx, TableIn.sPWM11.sIsON, TableOut.sPWM11.sIsON);
			return false; 
		}
		if ( TableIn.sPWM11.sPower != TableOut.sPWM11.sPower )
		{ 
			m_ErrorString.Format(_T("Error, In/Out Table[%d] sPWM11.sPower Exception (%d, %d)"), idx, TableIn.sPWM11.sPower, TableOut.sPWM11.sPower);
			return false; 
		}

		if ( TableIn.sPWM12.sIsON != TableOut.sPWM12.sIsON )
		{ 
			m_ErrorString.Format(_T("Error, In/Out Table[%d] sPWM12.sIsON Exception (%d, %d)"), idx, TableIn.sPWM12.sIsON, TableOut.sPWM12.sIsON);
			return false; 
		}
		if ( TableIn.sPWM12.sPower != TableOut.sPWM12.sPower )
		{ 
			m_ErrorString.Format(_T("Error, In/Out Table[%d] sPWM12.sPower Exception (%d, %d)"), idx, TableIn.sPWM12.sPower, TableOut.sPWM12.sPower);
			return false; 
		}

		if ( TableIn.sCameraEnable != TableOut.sCameraEnable )
		{ 
			m_ErrorString.Format(_T("Error, In/Out Table[%d] sCameraEnable Exception (%d, %d)"), idx, TableIn.sCameraEnable, TableOut.sCameraEnable);
			return false; 
		}
	}

	if ( TableIn.sDLPPulseTime != TableOut.sDLPPulseTime )
	{ 
		m_ErrorString.Format(_T("Error, In/Out Table[%d] DLPPulseTime Exception (%d, %d)"), idx, TableIn.sDLPPulseTime, TableOut.sDLPPulseTime);
		return false; 
	}
	if ( TableIn.sDLPCallbackTime != TableOut.sDLPCallbackTime )
	{ 
		m_ErrorString.Format(_T("Error, In/Out Table[%d] DLPCallbackTime Exception (%d, %d)"), idx, TableIn.sDLPCallbackTime, TableOut.sDLPCallbackTime);
		return false; 
	}
	if ( TableIn.sDLPCCDExpTime != TableOut.sDLPCCDExpTime )
	{ 
		m_ErrorString.Format(_T("Error, In/Out Table[%d] DLPCCDExpTime Exception (%d, %d)"), idx, TableIn.sDLPCCDExpTime, TableOut.sDLPCCDExpTime);
		return false; 
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::WriteTableListToFile(LPCTSTR filename, std::vector<TLCB_TRIG_TABLE> &TableList, std::vector<int> &NGTableList)//將表格列表寫至檔案中
{
	bool   IsNGTable=false;
	unsigned int i=0, j=0;
	FILE *pfile = NULL;
	char  CameraEnable[16]="";
	TLCB_TRIG_TABLE tmpTable;	
	const unsigned int TableCount = (int)(TableList.size());
	const unsigned int NGTableCount = (int)(NGTableList.size());
	const LIGHT_CTRL_BOARD_TYPE LightCtrlBoardType = GetLightCtrlBoardType();

	pfile = ::_tfopen(filename, _T("w+"));
	if ( NULL == pfile ) 
	{
		m_ErrorString.Format(_T("Error, Open File to Write Fault(%s)"), filename);
		return false;
	}
	for ( i=0; i<TableCount; i++ )
	{
		IsNGTable=false;
		for ( j=0; j<NGTableCount; j++ )
		{
			if ( i == NGTableList[j] )
			{	
				IsNGTable=true;
				break;
			}
		}

		tmpTable = TableList[i];	
		if ( false == IsNGTable )
		{	::fprintf(pfile, "[Table %d]\n", i+1); }
		else
		{	::fprintf(pfile, "[Table %d -- NG]\n", i+1); }

		::fprintf(pfile, "TableID:%d\n", tmpTable.sTableID);
		::fprintf(pfile, "TableType:%d\n", tmpTable.sTableType);
		::fprintf(pfile, "DLPTrigOutNumber:%d\n", tmpTable.sDLPTrigOutNumber);
		::fprintf(pfile, "DLPActiveChannel:%d\n", tmpTable.sDLPActiveChannel);

		::fprintf(pfile, "NextTableTime:%d\n", tmpTable.sNextTableTime);
		::fprintf(pfile, "LEDTableTotalTime:%d\n", tmpTable.sLEDTableTotalTime);
		::fprintf(pfile, "CCDDelayTime:%d\n", tmpTable.sCCDDelayTime);

		::fprintf(pfile, "LED PWN1.IsON:%d\n", tmpTable.sPWM1.sIsON);
		::fprintf(pfile, "LED PWN1.Power:%d\n", tmpTable.sPWM1.sPower);
		::fprintf(pfile, "LED PWN2.IsON:%d\n", tmpTable.sPWM2.sIsON);
		::fprintf(pfile, "LED PWN2.Power:%d\n", tmpTable.sPWM2.sPower);
		::fprintf(pfile, "LED PWN3.IsON:%d\n", tmpTable.sPWM3.sIsON);
		::fprintf(pfile, "LED PWN3.Power:%d\n", tmpTable.sPWM3.sPower);
		::fprintf(pfile, "LED PWN4.IsON:%d\n", tmpTable.sPWM4.sIsON);
		::fprintf(pfile, "LED PWN4.Power:%d\n", tmpTable.sPWM4.sPower);
		::fprintf(pfile, "LED PWN5.IsON:%d\n", tmpTable.sPWM5.sIsON);
		::fprintf(pfile, "LED PWN5.Power:%d\n", tmpTable.sPWM5.sPower);
		::fprintf(pfile, "LED PWN6.IsON:%d\n", tmpTable.sPWM6.sIsON);
		::fprintf(pfile, "LED PWN6.Power:%d\n", tmpTable.sPWM6.sPower);
		::fprintf(pfile, "LED PWN7.IsON:%d\n", tmpTable.sPWM7.sIsON);
		::fprintf(pfile, "LED PWN7.Power:%d\n", tmpTable.sPWM7.sPower);
		::fprintf(pfile, "LED PWN8.IsON:%d\n", tmpTable.sPWM8.sIsON);
		::fprintf(pfile, "LED PWN8.Power:%d\n", tmpTable.sPWM8.sPower);

		if ( LIGHT_CTRL_BOARD_8DA1 == LightCtrlBoardType )
		{
			::fprintf(pfile, "LED sPWM9.IsON:%d\n", tmpTable.sPWM9.sIsON);
			::fprintf(pfile, "LED sPWM9.Power:%d\n", tmpTable.sPWM9.sPower);
			::fprintf(pfile, "LED sPWM10.IsON:%d\n", tmpTable.sPWM10.sIsON);
			::fprintf(pfile, "LED sPWM10.Power:%d\n", tmpTable.sPWM10.sPower);
			::fprintf(pfile, "LED sPWM11.IsON:%d\n", tmpTable.sPWM11.sIsON);
			::fprintf(pfile, "LED sPWM11.Power:%d\n", tmpTable.sPWM11.sPower);
			::fprintf(pfile, "LED sPWM12.IsON:%d\n", tmpTable.sPWM12.sIsON);
			::fprintf(pfile, "LED sPWM12.Power:%d\n", tmpTable.sPWM12.sPower);

			JetAPI::IntToBin(tmpTable.sCameraEnable, 8, CameraEnable);
			::fprintf(pfile, "LED Camera Enable:%s\n", CameraEnable);
		}	

		::fprintf(pfile, "DLPPulseTime:%d\n", tmpTable.sDLPPulseTime);
		::fprintf(pfile, "DLPCallbackTime:%d\n", tmpTable.sDLPCallbackTime);
		::fprintf(pfile, "DLPCCDExpTime:%d\n", tmpTable.sDLPCCDExpTime);		
	}
	::fclose(pfile); pfile=NULL;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::BuildLCBTableFromSliceParamList(const std::vector<TSliceParam> &SliceParamList)//由SliceParm列表來建立燈盤表格列表
{
#ifndef LIGHT_CTRL_DISABLE	
	int i=0;
	int DLPIndex = -1;
	int DLPChannel=0;
	int DLPChannelMask = 0;
	int DLPMask = 0;//0x01|0x02|0x04|0x08;
	int DLPPeriod_us = 0;//us
	int DLPExposure_us = 0;//us
	int DLPLEDColor = DLP_LED_COLOR_WHITE;
	TLCB_TRIG_TABLE LCBTable;
	std::vector<TLCB_TRIG_TABLE> TableList;
	const bool bForce = false;
	const bool bSleep = false;
	const int  PaddingTime_us = CLight3DCtrl::GetDLPPeriodPaddTime_us();//us
	const int  ExpPaddingTime_us = CLight3DCtrl::GetDLPExposurePaddTime_us();//us	
	const bool DLPMultiTable = GetDLPMultiTableEnabled();
	const bool DLPInternalTrigger = GetDLPInteralTrigger();
	LIGHT_3D_CLS_PTR PhasePtr = NULL;
	LIGHT_3D_CAST_ID Light3DID = LIGHT_3D_CAST_00;
	
	SetDLPTableStartIndex(-1);
	if( ClearAll() == false )	
	{	return false;	}
	if ( AOIDataCollect.ConvertSliceParamListToLCBTableList(SliceParamList, TableList, DLPMultiTable) == false )
	{
		m_ErrorString = AOIDataCollect.GetErrorString();
		return false;
	}	
	
	int   DLPStartIndex = -1;
	const int TriggerTableCount = (int)(TableList.size());
	for ( i=0; i<TriggerTableCount; i++ )
	{
		LCBTable = TableList[i];
		if ( LIGHT_DLP != LCBTable.sTableType )	{	continue; }
		if ( -1 == DLPStartIndex )
		{	DLPStartIndex = (int)(i); }
		DLPChannelMask = CLightCtrlBoard::GetTableDLPChannelMask(LCBTable.sDLPActiveChannel);
		if ( 0 == (DLPMask&DLPChannelMask) ) 
		{	DLPMask |= DLPChannelMask;	}
		else//開啟過了
		{	continue;	}

		Light3DID = CLight3DCtrl::GetLight3DCastIDByDLPChannel(LCBTable.sDLPActiveChannel);		
		PhasePtr = Light3DCtrl.GetLight3DCastPtr(Light3DID);
		if ( NULL == PhasePtr ) 
		{
			m_ErrorString = Light3DCtrl.GetErrorString();
			return false;
		}		
		DLPIndex = LCBTable.sDLPActiveChannel-1;		
		const int NFrames = LCBTable.sDLPTrigOutNumber;
		const int PhasePatMode = LCBTable.sDLPPhasePatMode;
		//const int DLPCameraExpTime = CameraExposureTime_us;
		const int DLPCameraExpTime = LCBTable.sDLPCCDExpTime;
		if ( DLP_PATTERN_SEQUENCE_DEBUG == PhasePatMode )	
		{
			m_ErrorString.Format(_T("Error, Phase Pattern Sequence Exception"));
			return false;
		}
		const int DLPExposureMinTime = CLight3DCtrl::GetDLPExposureMinTime(DLPInternalTrigger, DLPMultiTable, NFrames);
		DLPExposure_us = DLPCameraExpTime+ExpPaddingTime_us;
		DLPExposure_us = MAX(DLPExposure_us, DLPExposureMinTime);
		DLPPeriod_us = MAX(DLPPeriod_us, DLPExposure_us+PaddingTime_us);
		DLPPeriod_us = PhasePtr->GetDLPParam().m_PeriodTime_us;
		DLPExposure_us = PhasePtr->GetDLPParam().m_ExposureTime_us;		
		DLPLEDColor = PhasePtr->GetDLPParam().m_LEDColor;
		if ( DLPCameraExpTime < DLPExposure_us )
		{
			m_ErrorString.Format(_T("Error, Camera Exp. Time[%d] < DLP Exp. Time[%d]"), DLPCameraExpTime, DLPExposure_us);
			return false;
		}
		if ( PhasePtr->ExecDLPPatBuildSendValidate(PhasePatMode, DLPInternalTrigger, DLPMultiTable, DLPPeriod_us, DLPExposure_us, DLPLEDColor, bSleep, bForce) == false )		
		{
			m_ErrorString = PhasePtr->GetErrorString();
			return false;
		}
		
		//Run
		if ( PhasePtr->ExecDLPPattern_Run()  == false )
		{
			m_ErrorString = PhasePtr->GetErrorString();
			return false;
		}					
	}	
	SetDLPTableStartIndex(DLPStartIndex);
	if ( false == bSleep )//如果DLP的設定沒有延遲時間的話
	{	JetAPI::TimeDelay_TickCount(50);	}
	
	if ( TableListWrite(TableList) == false )
	{	return false;	}	

	const int CheckTable = GetTableRecheckEnabled();
	if ( FN_ENABLE == CheckTable )
	{
		::Sleep(50);		
		std::vector<int>             NGTableList;
		std::vector<TLCB_TRIG_TABLE> TableListOut;
		std::vector<TLCB_TRIG_TABLE> TableListIn=TableList;
		const size_t TableCountIn = TableListIn.size();
		if ( TableListRead(TableListOut, TableCountIn) == false ) 
		{	return false;	}
		if ( CheckTableListEqually(TableListIn, TableListOut, NGTableList) == false )
		{
			CString tmpfile;
			tmpfile.Format(_T("%s\\LightCtrlTableFault.TXT"), AOIDataCollect.GetAOITempDirectory());
			WriteTableListToFile(tmpfile, TableListOut, NGTableList);
			::ShellExecute(NULL, _T("open"), tmpfile, NULL, NULL, SW_SHOW);			
			return false; 
		}
	}

	if ( SetTableRunCount(TriggerTableCount) == false )
	{	return false;	}		
	
	if( SetMode_FPGA() == false )	
	{	return false;	}
#endif//LIGHT_CTRL_DISABLE	
	return true;
}
//-------------------------------------------------------------------------------------//