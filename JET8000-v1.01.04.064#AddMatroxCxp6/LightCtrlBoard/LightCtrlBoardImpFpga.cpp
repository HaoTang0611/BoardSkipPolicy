// LightCtrlBoardFpga.cpp: implementation of the CLightCtrlBoardImpFpga class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "LightCtrlBoard.h"
#include "LightCtrlBoardImpFpga.h"
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
// Address(Write)
#define W_LOOPBACKA				0x01
#define W_LOOPBACKB				0x02
#define W_WRITE					0x03
#define W_MODE_SET				0x04
#define W_TABLE_SET				0x05
#define W_READ_RAM_ADD			0x06


#define W_DLP_ENABLE			0x09

// Address(Read)
#define R_LOOPBACKA				0x01
#define R_LOOPBACKB				0x02
#define R_RAM_DATA				0x06
#define R_RAM_ADDRESS			0x07
#define R_ERROR					0x08

#define R_WORKING_NOW			0x09
#define R_FPGA_OK				0x09

#define R_DLP_ERR_CH            0x0A

#define R_PC2FPGA_TRIGCOUNT		0x0B
#define R_FPGA2CCD_TRIGCOUNT	0x0C

#define R_FPGA2DLP_TRIGCOUNT	0x0D
//#define R_FPGA2DLP_TRIGCOUNT	0x0E
//#define R_FPGA2DLP_TRIGCOUNT	0x0F
//#define R_FPGA2DLP_TRIGCOUNT	0x10
//#define R_FPGA2DLP_TRIGCOUNT	0x11
//#define R_FPGA2DLP_TRIGCOUNT	0x12
//#define R_FPGA2DLP_TRIGCOUNT	0x13
//#define R_FPGA2DLP_TRIGCOUNT	0x14

#define R_DLP2FPGA_TRIGCOUNT	0x15
//#define R_DLP2FPGA_TRIGCOUNT	0x16
//#define R_DLP2FPGA_TRIGCOUNT	0x17
//#define R_DLP2FPGA_TRIGCOUNT	0x18
//#define R_DLP2FPGA_TRIGCOUNT	0x19
//#define R_DLP2FPGA_TRIGCOUNT	0x1A
//#define R_DLP2FPGA_TRIGCOUNT	0x1B
//#define R_DLP2FPGA_TRIGCOUNT	0x1C

#define R_DLP_TRIG_STEP         0x38
#define R_FPGA_VERSION			0x3F


//SET DATA
#define SET_DATA_FPGA_MODE					0x0000	//0000 0000 0000 0000 
#define SET_DATA_PC_WRITE_MODE				0x0004	//0000 0000 0000 0100
#define SET_DATA_PC_READ_MODE				0x0002	//0000 0000 0000 0010 
#define SET_DATA_PC_ADDRESS_ASSIGN_MODE		0x0006	//0000 0000 0000 0110 

#define FPGA_TABLE_SIZE		                17//10進制
#define FPGA_TABLE_MAX_COUNT_8DA1           54//表格上限//MAX_TABLE_COUNT
#define FPGA_TABLE_MAX_COUNT_8DA3           84//表格上限//MAX_TABLE_COUNT
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::CreateImp_Fpga()//建立_Imp指標	
{
	try
	{
		_Imp = new CLightCtrlBoardImpFpga();
		if ( NULL == _Imp )
		{
			m_ErrorString=_T("Error, CLightCtrlBoard CreateImp_Fpga Fault");
			return false;
		}
	}
	catch (...)
	{
		m_ErrorString=_T("Error, CLightCtrlBoard CreateImp_Fpga Fault");
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
CLightCtrlBoardImpFpga::CLightCtrlBoardImpFpga():CLightCtrlBoardImp()
{	
#ifndef LIGHT_CTRL_DISABLE
	::InitializeCriticalSection(&m_csUSB);
	m_usbBoardID = 2;
	SetTableMaxCount(54);
	LoadLightCtrlBoardINI();
	SaveLightCtrlBoardINI();	
#endif//LIGHT_CTRL_DISABLE
}
//-------------------------------------------------------------------------------------//
CLightCtrlBoardImpFpga::~CLightCtrlBoardImpFpga()
{
#ifndef LIGHT_CTRL_DISABLE
	CLightCtrlBoardImpFpga::DisConnect();	
	::DeleteCriticalSection(&m_csUSB);
#endif//LIGHT_CTRL_DISABLE		
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpFpga::UpdateSupportFuncByVersion()//依據版本號更新支援功能
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
	if ( true == bMoreThan64Table )
	{	SetTableMaxCount(FPGA_TABLE_MAX_COUNT_8DA3);	}
	else
	{	SetTableMaxCount(FPGA_TABLE_MAX_COUNT_8DA1); }
	SetSupportMoreThan64Table(bMoreThan64Table);
	SetSupportModifyTriggerFirstIndex(bModifyTriggerFirstIndex);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpFpga::Connect()
{
#ifndef LIGHT_CTRL_DISABLE	
	const int BoardID = GetUSBBoardID();
	char BoardName[JET_USB_TEXT_SIZE] = "";
	char BoardCode[JET_USB_TEXT_SIZE] = "";
	return Connect(BoardID, BoardName, BoardCode);
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpFpga::Connect(int BoardID, char BoardName[], char BoardCode[])
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
	SetTableRunCount(0);
	SetUSBBoardID(BoardID);
	ReadFPGAVersion(m_FPGAVersion);	
	//m_FullVersion.Format(_T("%s%s (Version:%s)"), strName, strCode, m_FPGAVersion);		
	m_FullVersion.Format(_T("%s#%s%s"), m_FPGAVersion, strName, strCode);	
	UpdateSupportFuncByVersion();
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpFpga::GetIsConnected()
{	
#ifndef LIGHT_CTRL_DISABLE		
	return m_USB.GetIsConnected();	
#endif//LIGHT_CTRL_DISABLE
	return false;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpFpga::CheckIsConnected()
{
	if ( GetIsConnected() == false )
	{
		m_ErrorString=_T("Error, Not Connected");
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpFpga::DisConnect()
{
#ifndef LIGHT_CTRL_DISABLE	
	if ( m_USB.GetIsConnected() == true )
	{	m_USB.DisConnectUSB();	}
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
void CLightCtrlBoardImpFpga::SetUSBBoardID(int val)
{
	m_usbBoardID = val;
}
//-------------------------------------------------------------------------------------//
int CLightCtrlBoardImpFpga::GetUSBBoardID() const
{
	return m_usbBoardID;
}
//-------------------------------------------------------------------------------------//
int CLightCtrlBoardImpFpga::GetCameraCount() const//取得相機數量
{
	return m_CameraCount;	
}
//-------------------------------------------------------------------------------------//
int CLightCtrlBoardImpFpga::GetDLPCastCount() const//取得DLP投光數量
{
	return m_DLPCastCount;	
}
//-------------------------------------------------------------------------------------//
int CLightCtrlBoardImpFpga::GetLEDChannelCount() const
{
	return m_LEDChannelCount;	
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpFpga::UpdateLightCtrlBoardType()//更新控制板USB命令文字
{
	//寫入USB控制命令的參數	
	char  CmdText[32]="";
	const LIGHT_CTRL_BOARD_TYPE LightCtrlBoardType = GetLightCtrlBoardType();//取得控制板樣式

	if ( CLightCtrlBoardImp::UpdateLightCtrlBoardType() == false )
	{	return false; }

	switch ( LightCtrlBoardType )
	{
	case LIGHT_CTRL_BOARD_3DA6:
		::strcpy(CmdText, "0A8A0A");
		m_USB.SetCmdTextReadData(CmdText);//讀取資料的命令

		::strcpy(CmdText, "0A4A0A");	
		m_USB.SetCmdTextWriteData(CmdText);//寫入資料的命令

		::strcpy(CmdText, "0B4B0B");
		m_USB.SetCmdTextReadAddress(CmdText);//讀取位址的命令

		::strcpy(CmdText, "0B4B0B");
		m_USB.SetCmdTextWriteAddress(CmdText);//寫入位址的命令
		break;

	case LIGHT_CTRL_BOARD_8DA1:	
		::strcpy(CmdText, "0A8A0A");	
		m_USB.SetCmdTextReadData(CmdText);//讀取資料的命令

		::strcpy(CmdText, "0A4A0A");	
		m_USB.SetCmdTextWriteData(CmdText);//寫入資料的命令

		::strcpy(CmdText, "0B5B0B");
		m_USB.SetCmdTextReadAddress(CmdText);//讀取位址的命令

		::strcpy(CmdText, "0B6B0B");
		m_USB.SetCmdTextWriteAddress(CmdText);//寫入位址的命令
		break;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpFpga::LoopBackTestAll()
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
bool CLightCtrlBoardImpFpga::LoopBackTest(int data, char DataWS[], char DataRA[], char DataRB[])
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
bool CLightCtrlBoardImpFpga::LoopBackTestA(int data, char DataWS[], char DataR[])
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
bool CLightCtrlBoardImpFpga::LoopBackTestB(int data, char DataWS[], char DataR[])
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
bool CLightCtrlBoardImpFpga::ExecUSBRead(char Address[], char Data[])
{	
#ifndef LIGHT_CTRL_DISABLE	
	bool IsOK = true;
	::EnterCriticalSection(&m_csUSB);
	IsOK = m_USB.USBRead(Address, Data);
	::LeaveCriticalSection(&m_csUSB);
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
bool CLightCtrlBoardImpFpga::ExecUSBWrite(char Address[], char Data[])
{
#ifndef LIGHT_CTRL_DISABLE
//	if ( m_USB.WriteData(Data) == false )
//	{	return false; }
	bool IsOK = true;
	::EnterCriticalSection(&m_csUSB);
	IsOK = m_USB.USBWrite(Address, Data);
	::LeaveCriticalSection(&m_csUSB);
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
bool CLightCtrlBoardImpFpga::ReadLightCtrlBorad(const std::string &Address, std::string &Data)
{
	char tData[JET_USB_TEXT_SIZE]="";
	char tAddress[JET_USB_TEXT_SIZE]="";	
	strcpy(tData, Data.c_str());
	strcpy(tAddress, Address.c_str());	
	if ( ExecUSBRead(tAddress, tData) == false )
	{	return false; }
	Data=tData;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpFpga::WriteLightCtrlBorad(const std::string &Address, const std::string &Data)
{
	char tData[JET_USB_TEXT_SIZE]="";
	char tAddress[JET_USB_TEXT_SIZE]="";	
	strcpy(tData, Data.c_str());
	strcpy(tAddress, Address.c_str());	
	return ExecUSBWrite(tAddress, tData);
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpFpga::SaveLightCtrlBoardINI()//寫參數至檔案
{
	int     i=0;
	bool    IsOK = true;
	CString FileName;
	CString KeyName;
	CString KeyString;
	CString Section;	
	const LIGHT_CTRL_BOARD_TYPE LightCtrlBoardType = GetLightCtrlBoardType();

	if ( CLightCtrlBoardImp::SaveLightCtrlBoardINI() == false )
	{	return false; }
	
	FileName = GetLCBIniFileName();
	Section = GetLCBIniSectionName(LightCtrlBoardType);	
	
	//USB編號
	KeyName.Format(_T("USB Board ID")); KeyString.Format(_T("%d"), m_usbBoardID);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }

	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpFpga::LoadLightCtrlBoardINI()//從檔案讀取參數
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
	LIGHT_CTRL_BOARD_TYPE LightCtrlBoardType;	

	if ( CLightCtrlBoardImp::LoadLightCtrlBoardINI() == false )
	{	return false; }

	FileName = GetLCBIniFileName();
	LightCtrlBoardType = GetLightCtrlBoardType();//讀完INI檔案會變更控制板版本
	Section = GetLCBIniSectionName(LightCtrlBoardType);	

	//USB編號
	KeyName.Format(_T("USB Board ID")); KeyString.Format(_T("%d"), m_usbBoardID);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	
		nValue = ::_ttoi(ReturnString); 		
		if ( nValue >= 0 ) 
		{	m_usbBoardID = nValue; }
	}	

	UpdateLightCtrlBoardType();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpFpga::ReadRAMData(char data[])
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
bool CLightCtrlBoardImpFpga::ReadRAMAddress(char data[])
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
bool CLightCtrlBoardImpFpga::CheckLightCtrlBoardError(bool &IsError, CString &ErrorStr)
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
	if ( DLPStep.GetLength() > 0 )
	{
		tmpErrStr.Format(_T("%s\n%s"), ErrorStr, DLPStep);
		ErrorStr = tmpErrStr;	
	}
		
	CString CountStr = _T("");
	if( GetTriggerCountText(CountStr) == false ) { return false; }
	tmpErrStr.Format(_T("%s\n%s\n"), ErrorStr, CountStr);
	ErrorStr = tmpErrStr;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpFpga::ReadErrorCode(int &Error)
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
bool CLightCtrlBoardImpFpga::ReadErrorString(CString &Error)
{
	Error = _T("");
#ifndef LIGHT_CTRL_DISABLE	
	const int StrSize = JET_USB_TEXT_SIZE;		
	char  DataRS[StrSize] = "";		
	char  AddressRS[StrSize] = "";	
	JetAPI::IntToHex(R_ERROR, 2, AddressRS);
	if ( ExecUSBRead(AddressRS, DataRS) == false ) 
	{	return false;	}
	Error = CString(DataRS);	
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpFpga::ReadDLPErrCH(CString &ErrDlp)
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
bool CLightCtrlBoardImpFpga::ReadDLPTrigStepInfo(CString &DLPTrigInfo)
{
	DLPTrigInfo = _T("");
#ifndef LIGHT_CTRL_DISABLE	
#ifndef PHASE_CTRL_DISABLE
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
#endif//PHASE_CTRL_DISABLE
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpFpga::DecodeErrorCodeText(int ErrorCode, CString &ErrorStr)
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
bool CLightCtrlBoardImpFpga::GetTriggerCountText(CString &TrigCount)
{	
#ifndef LIGHT_CTRL_DISABLE
	UINT cntPC_FPGA=0;
	UINT cntFPG_CCD=0;	

	if( GetPCtoFPGATrigCount(cntPC_FPGA) == false ) { return false; }	
	if( GetFPGAtoCCDTrigCount(cntFPG_CCD) == false ) { return false; }			
#ifndef PHASE_CTRL_DISABLE
	UINT cntFPG_DLP1=0, cntFPG_DLP2=0, cntFPG_DLP3=0, cntFPG_DLP4=0;
	UINT cntFPG_DLP5=0, cntFPG_DLP6=0, cntFPG_DLP7=0, cntFPG_DLP8=0;
	UINT cntDLP1_FPG=0, cntDLP2_FPG=0, cntDLP3_FPG=0, cntDLP4_FPG=0;
	UINT cntDLP5_FPG=0, cntDLP6_FPG=0, cntDLP7_FPG=0, cntDLP8_FPG=0;
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
#else
	TrigCount.Format(_T("PC_FPGA(%d), FPGA_CCD(%d)"), cntPC_FPGA, cntFPG_CCD); 
#endif//PHASE_CTRL_DISABLE
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpFpga::ReadFPGAVersion(CString &Version)
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
bool CLightCtrlBoardImpFpga::GetIsWorkingNow(bool &Working)
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
bool CLightCtrlBoardImpFpga::GetIsFPGAOK(bool &IsOK)
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
bool CLightCtrlBoardImpFpga::SetDLPEnable(UINT DLPChannel)
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
bool CLightCtrlBoardImpFpga::ReadBoardAllCount()
{
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpFpga::GetPCtoFPGATrigCount(UINT &count)
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
bool CLightCtrlBoardImpFpga::GetFPGAtoCCDTrigCount(UINT &count)
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
bool CLightCtrlBoardImpFpga::GetFPGAtoCCDTrigCount(UINT CameraID, UINT &count)
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
bool CLightCtrlBoardImpFpga::GetFPGAtoDLPTrigCount(UINT DLPChannel, UINT &count)
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
bool CLightCtrlBoardImpFpga::GetDLPtoFPGATrigCount(UINT DLPChannel, UINT &count)
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
bool CLightCtrlBoardImpFpga::TriggerStart()//開始觸發
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
bool CLightCtrlBoardImpFpga::ClearAllCount()//清除所有數量
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
bool CLightCtrlBoardImpFpga::ClearAll()//清除全部資料
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
bool CLightCtrlBoardImpFpga::SwitchToFPGA()
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
bool CLightCtrlBoardImpFpga::SwitchToRead()
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
bool CLightCtrlBoardImpFpga::SwitchToWrite()
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
bool CLightCtrlBoardImpFpga::SwitchToAssign()
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
bool CLightCtrlBoardImpFpga::SetMode_FPGA()//FPGA Run
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
bool CLightCtrlBoardImpFpga::SetMode_PCWrite()//寫入
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
bool CLightCtrlBoardImpFpga::SetMode_PCRead()//讀取
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
bool CLightCtrlBoardImpFpga::SetMode_PCAssign()//指定位址
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
bool CLightCtrlBoardImpFpga::WriteTableRunCount(int nTable)//Trigger後要跑幾張TABLE
{
#ifndef LIGHT_CTRL_DISABLE
	//	if ( this->SetMode_PCWrite() == false ) {	return false;	}
	const int MaxTableCount=GetTableMaxCount();
	if ( nTable<0 || nTable>=MaxTableCount )
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

	SetTableRunCount(nTable);
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpFpga::ReadTableRunCount(int &nTable)
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
bool CLightCtrlBoardImpFpga::SetCCDTrigEdge(int TrigEdge)//CCD Trigger Edge
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
bool CLightCtrlBoardImpFpga::EnableDLPChannel(int DLPMask)//新增加的DLP開啟功能
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
bool CLightCtrlBoardImpFpga::GetCCDTrigEdge(int &TrigEdge)
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
bool CLightCtrlBoardImpFpga::SetTriggerFirstIndex(UINT Index)//處發表格第1張引數
{
#ifndef LIGHT_CTRL_DISABLE	
	const int MaxTableCount=GetTableMaxCount();
	if ( Index<0 || Index>=MaxTableCount )
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
bool CLightCtrlBoardImpFpga::TableRAMAssign(UINT AssignAddress)//指定RAM位址
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
bool CLightCtrlBoardImpFpga::TableAssign(UINT TableIndex)//指定RAM位址至某個Table的起始位址
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
bool CLightCtrlBoardImpFpga::TableRAMAddressAdd()//RAM位址加1
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
bool CLightCtrlBoardImpFpga::TableSingleWrite(TLCB_TRIG_TABLE &Table)//寫入一個Table
{
#ifndef LIGHT_CTRL_DISABLE
	const int MaxTableCount=GetTableMaxCount();
	const LIGHT_CTRL_BOARD_TYPE LightCtrlBoardType = GetLightCtrlBoardType();

	//寫入一個Table
	if( Table.sTableID<0 || Table.sTableID>=MaxTableCount )
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
bool CLightCtrlBoardImpFpga::TableSingleRead(TLCB_TRIG_TABLE &Table)//讀出一個Table
{
#ifndef LIGHT_CTRL_DISABLE
	const int MaxTableCount=GetTableMaxCount();
	const LIGHT_CTRL_BOARD_TYPE LightCtrlBoardType = GetLightCtrlBoardType();

	//讀出一個Table	
	if( Table.sTableID<0 || Table.sTableID>=MaxTableCount )
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
bool CLightCtrlBoardImpFpga::TableListRead(std::vector<TLCB_TRIG_TABLE> &TableList, int nTable)
{
	TableList.clear();
#ifndef LIGHT_CTRL_DISABLE
	//return table index
	if ( ClearAllCount() == false ) 
	{	return false; }

	int             i=0;	
	TLCB_TRIG_TABLE tmpTable;	
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
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpFpga::TableListWrite(const std::vector<TLCB_TRIG_TABLE> &TableList)
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

	size_t  i=0;
	int     tmpData=0;
	DWORD   DLPEnableMask=0;	
	TLCB_TRIG_TABLE tmpTable;
	TLCB_TRIG_TABLE tmpTableA;
	TLCB_TRIG_TABLE tmpTableB;		
	std::vector<TLCB_TRIG_TABLE>  TableListTotal = TableList;
	const size_t nTable = TableList.size();

	//將後面給予填入空的表格
	DefaultTable(tmpTable);
	const size_t MaxTableCount = GetTableMaxCount();
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
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpFpga::TableReset(int nTable)
{
#ifndef LIGHT_CTRL_DISABLE
	if( nTable <= 0 )  { return true; }
	//return table index
	if( ClearAllCount() == false ) { return false; }

	int i=0;
	TLCB_TRIG_TABLE tmpTable;
	const int MaxTableCount=GetTableMaxCount();
	if ( nTable > MaxTableCount ) 
	{	nTable = MaxTableCount; }

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
int CLightCtrlBoardImpFpga::GetCurFPGAMode() const
{
	return this->m_FPGAMode;
}
//-------------------------------------------------------------------------------------//