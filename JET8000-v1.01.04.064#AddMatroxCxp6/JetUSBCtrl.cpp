#include "stdafx.h"
#include "JetUSBCtrl.h"
//---------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------//
CJetUSBCtrl::CJetUSBCtrl()
{
	PreInitialUSB();
}
//-------------------------------------------------------------------//
CJetUSBCtrl::~CJetUSBCtrl()
{
	ReleaseUSBControl();
}
//-------------------------------------------------------------------//
LPCTSTR CJetUSBCtrl::GetErrorString()
{
	return m_ErrorString;
}
//------------------------------------------------------------------//
void CJetUSBCtrl::PreInitialUSB()
{
	m_hDevice = NULL;
	m_IsConnected = false;
	m_SaveLogMsg = false;
	m_BoardID = -1;	

	::memset(m_CmdTextReadData, 0x00, sizeof(m_CmdTextReadData));
	::memset(m_CmdTextWriteData, 0x00, sizeof(m_CmdTextWriteData));
	::memset(m_CmdTextReadAddress, 0x00, sizeof(m_CmdTextReadAddress));
	::memset(m_CmdTextWriteAddress, 0x00, sizeof(m_CmdTextWriteAddress));	

	SetCmdTextReadData("0A8A0A");
	SetCmdTextWriteData("0A4A0A");
	SetCmdTextReadAddress("0B4B0B");
	SetCmdTextWriteAddress("0B4B0B");		

	this->m_bOverLapped = TRUE;
	this->m_USBTimeOut = 1000;	
	this->m_pWriteOvLap = NULL;
	this->m_pReadOvLap = NULL;
	QueryPerformanceFrequency(&m_SystemFreq);

	CString Folder = AOIDataCollect.GetAOILogDirectory();
	this->m_USBDebugFile.Format(_T("%s\\%s"), Folder, _T("USBLogFile.TXT"));
	::DeleteFile(this->m_USBDebugFile);

	this->SaveUSBLogMessage(_T("CJetUSBCtrl::PreInitialUSB"));
}
//-------------------------------------------------------------------//
inline bool CJetUSBCtrl::CheckInit()
{	
	if ( this->m_hDevice == NULL || m_hDevice==INVALID_HANDLE_VALUE )
	{
		this->m_ErrorString = _T("Error, USB Exception (m_hDevice == NULL)");
		return false;
	}
	if ( this->m_IsConnected == false ) 
	{ 
		this->m_ErrorString = _T("Error, USB Exception (m_IsConnected == false)");
		return false;
	}
	if ( m_BoardID == -1 )
	{
		this->m_ErrorString = _T("Error, USB Exception (m_BoardID == -1)");
		return false;
	}
	return true;
}
//-------------------------------------------------------------------//
inline bool CJetUSBCtrl::CheckDeviceHandle(HANDLE hDevice)
{
	if ( hDevice == NULL || hDevice==INVALID_HANDLE_VALUE )
	{
		this->m_ErrorString = _T("Error, USB Exception (hDevice == NULL)");
		return false;
	}
	return true;
}
//-------------------------------------------------------------------//
void CJetUSBCtrl::ReleaseUSBControl()
{
	DisConnectUSB();
	this->SaveUSBLogMessage(_T("CJetUSBCtrl::ReleaseUSBControl"));
}
//-------------------------------------------------------------------//
bool CJetUSBCtrl::WriteAddress(char Address[], const int AddressMode)
{
	if ( this->CheckInit() == false ) { return false; }
	
	BOOL  bResult = FALSE;
	int   TryCount = 0; 

	::_strupr(Address);	
	int A_i[8]={0};	
	char Add_cr[8] = {0};
	char Comm_cr[8] = {0};
	char TempStr[JET_USB_TEXT_SIZE]={0};
	char WriteComm[JET_USB_TEXT_SIZE] = "";	
	if( AddressMode == ADDRESS_R )
	{	
		if ( GetReadAddressCmd(WriteComm) == false )
		{	return false; }
	}
	else
	{
		if ( GetWriteAddressCmd(WriteComm) == false )
		{	return false; }
	}
	sprintf(TempStr, "%s", Address); 
	ASCII_to_Int_Fn(TempStr, A_i, 2);
	Add_cr[0]=A_i[0]*16+A_i[1];
	
	if( AddressMode == ADDRESS_R )
	{ 
		Add_cr[0] = Add_cr[0] & 0x3f;
		Add_cr[0] = Add_cr[0] | 0x80;
	}
	else
	{ 
		Add_cr[0] = Add_cr[0] & 0x3f;
		Add_cr[0] = Add_cr[0] | 0x40;
	}

	TryCount = 0; 	
	do
	{
		bResult = this->USB_WRITE_Fn(Add_cr,2,0);//PE1 => pipe=0
		if ( bResult == TRUE )
		{	break; }

		TryCount++;
		if ( TryCount >= MAX_USB_RESET_COUNT )
		{	return false;	}

	#ifdef ENABLE_USB_DEVICE_RESET
		if ( DoDeviceReset() == false )
		{	return false;	}
	#endif

	} while (true);
	
	sprintf(TempStr, "%s", WriteComm); 
	ASCII_to_Int_Fn(TempStr, A_i, 6);
	Comm_cr[0]=A_i[4]*16+A_i[5];
	Comm_cr[1]=A_i[2]*16+A_i[3];
	Comm_cr[2]=A_i[0]*16+A_i[1];

	TryCount = 0; 
	do
	{
		bResult = USB_WRITE_Fn(Comm_cr,3,3);//PE6 => pipe=3
		if ( bResult == TRUE )
		{	break; }

		TryCount++;
		if ( TryCount >= MAX_USB_RESET_COUNT )
		{	return false;	}

	#ifdef ENABLE_USB_DEVICE_RESET
		if ( DoDeviceReset() == false )
		{	return false;	}
	#endif

	} while ( true );
	return true;
}
//-------------------------------------------------------------------//
bool CJetUSBCtrl::ReadData(char DataS[])
{
	if ( this->CheckInit() == false ) { return false; }
	
	BOOL  bResult = FALSE;
	int TryCount = 0; 

	int A_i[8]={0};
	char TempStr[16]={0};
	char Comm_cr[8] = {0};
	char ReadData[4] = {0};
	
	char  ReadStr[JET_USB_TEXT_SIZE] = "";
	char  ShowData[JET_USB_TEXT_SIZE] = "";	
	char  BufferStr[JET_USB_TEXT_SIZE] = "";
	char  ReadCommand[JET_USB_TEXT_SIZE] = "";
	if ( GetReadDadaCmd(ReadCommand) == false )
	{	return false; }
	sprintf(TempStr, "%s", ReadCommand); 
	ASCII_to_Int_Fn(TempStr, A_i, 6);
	Comm_cr[0]=A_i[4]*16+A_i[5];
	Comm_cr[1]=A_i[2]*16+A_i[3];
	Comm_cr[2]=A_i[0]*16+A_i[1];

	TryCount = 0;
	do
	{
		bResult = USB_WRITE_Fn(Comm_cr,3,3);//PE6 => pipe=3
		if ( bResult == TRUE )
		{
			bResult = USB_READ_Fn(ReadData,2,1);//PE1 IN => pipe=1
			if ( bResult == TRUE )
			{	break; }
		}

		TryCount++;
		if ( TryCount >= MAX_USB_RESET_COUNT )
		{	return false;	}

	#ifdef ENABLE_USB_DEVICE_RESET
		if ( DoDeviceReset() == false )
		{	return false;	}		
	#endif

	} while ( true );

	IntToHex(ReadData[1]&0xFF,2, ReadStr);
	::strcpy(BufferStr, ShowData);
	::sprintf(ShowData, "%s%s", BufferStr, ReadStr);	
	IntToHex(ReadData[0]&0xFF,2, ReadStr);
	::strcpy(BufferStr, ShowData);
	::sprintf(ShowData, "%s%s", BufferStr, ReadStr);

	::strcpy(DataS, ShowData);
	return true;
}
//-------------------------------------------------------------------//
bool CJetUSBCtrl::Delay(DWORD Delayus)
{
	//Delayus = 100000;
	if ( 0 == Delayus ) { return true; }

	double time=0.0;
	LARGE_INTEGER Cnt1, Cnt2;	
	QueryPerformanceCounter(&Cnt1);
	while ( true )
	{
		QueryPerformanceCounter(&Cnt2);	
		time = (Cnt2.QuadPart - Cnt1.QuadPart)*1000000.0/m_SystemFreq.QuadPart;
		if ( time > Delayus )
		{	break; }
	};
	return true;
}
//-------------------------------------------------------------------//
bool CJetUSBCtrl::GetReadDadaCmd(char Cmd[])
{	
	strcpy(Cmd, m_CmdTextReadData);	
	return true;
}
//-------------------------------------------------------------------//
bool CJetUSBCtrl::GetWriteDadaCmd(char Cmd[])
{	
	strcpy(Cmd, m_CmdTextWriteData);
	return true;
}
//-------------------------------------------------------------------//
bool CJetUSBCtrl::GetReadAddressCmd(char Cmd[])
{	
	strcpy(Cmd, m_CmdTextReadAddress);
	return true;
}
//-------------------------------------------------------------------//
bool CJetUSBCtrl::GetWriteAddressCmd(char Cmd[])
{	
	strcpy(Cmd, m_CmdTextWriteAddress);
	return true;
}
//-------------------------------------------------------------------//
bool CJetUSBCtrl::WriteData(char DataS[])
{
	if ( this->CheckInit() == false ) { return false; }

	BOOL  bResult = FALSE;
	int TryCount = 0;
	
	::_strupr(DataS);
	int A_i[8]={0};	
	char Comm_cr[8] = {0};
	char Data_cr[8] = {0};
	char TempStr[JET_USB_TEXT_SIZE]={0};	

	if ( CJetUSBCtrl::CheckData(DataS, 4) == false )
	{	return false; }	

	sprintf(TempStr, "%s", DataS);
	ASCII_to_Int_Fn(TempStr, A_i, 4);
	Data_cr[0]=A_i[2]*16+A_i[3];
	Data_cr[1]=A_i[0]*16+A_i[1];

	TryCount = 0;
	do 
	{
		bResult = USB_WRITE_Fn(Data_cr,2,0);//PE1 OUT => pipe=0
		if ( bResult == TRUE )
		{	break; }

		TryCount++;
		if ( TryCount >= MAX_USB_RESET_COUNT )
		{	return false;	}

	#ifdef ENABLE_USB_DEVICE_RESET
		if ( DoDeviceReset() == false )
		{	return false;	}
	#endif

	} while ( true );

	char WriteCommand[JET_USB_TEXT_SIZE] = "";
	if ( GetWriteDadaCmd(WriteCommand) == false )
	{	return false; }
	sprintf(TempStr, "%s", WriteCommand); 
	ASCII_to_Int_Fn(TempStr, A_i, 6);
	Comm_cr[0]=A_i[4]*16+A_i[5];
	Comm_cr[1]=A_i[2]*16+A_i[3];
	Comm_cr[2]=A_i[0]*16+A_i[1];

	TryCount = 0;
	do 
	{
		bResult = USB_WRITE_Fn(Comm_cr, 3, 3);//PE6 => pipe=3
		if ( bResult == TRUE )
		{	break; }

		TryCount++;
		if ( TryCount >= MAX_USB_RESET_COUNT )
		{	return false;	}

	#ifdef ENABLE_USB_DEVICE_RESET
		if ( DoDeviceReset() == false)
		{	return false;	}
	#endif

	} while ( true );
	return true;
}
//-------------------------------------------------------------------//
bool CJetUSBCtrl::USBWrite(char Address[], char DataS[])
{
	if ( CJetUSBCtrl::CheckInit() == false )
	{	return false; }

	if ( this->CheckData(Address, JET_USB_CTRL_ADDRESS_SIZE) == false )
	{	return false;	}

	if ( this->CheckData(DataS, JET_USB_CTRL_DATA_SIZE) == false )
	{	return false;	}

	//Write Data	
	if( WriteData(DataS) == false ) 
	{	return false;	}

	//write Address 
	if( WriteAddress(Address, ADDRESS_W) == false ) 
	{	return false;	}
	
	return true;
}
//-------------------------------------------------------------------//
bool CJetUSBCtrl::USBRead(char Address[], char DataS[], const int status)
{
	if ( CJetUSBCtrl::CheckInit() == false )
	{	return false; }	

	if ( this->CheckData(Address, JET_USB_CTRL_ADDRESS_SIZE) == false )
	{	return false;	}

	//write Address 	
	if ( WriteAddress(Address, ADDRESS_R) == false )
	{	return false;	}

	//Read Data	
	if ( ReadData(DataS) == false )
	{	return false;	}
	return true;
}
//---------------------------------------------------------------------------//
bool CJetUSBCtrl::TestReadGetID(int DeviceIndex, char strBoardID[])
{
	//開啟裝置
	if ( OpenDriver(DeviceIndex) == false )
	{	return false;}

	//	
	char  Address[JET_USB_TEXT_SIZE] = "00";	
	char  BufferStr[JET_USB_TEXT_SIZE] = "";
	char  ReadCommand[JET_USB_TEXT_SIZE] = "3F7F3F";

	if ( CJetUSBCtrl::CheckData(Address, 2) == false )
	{	return false; }	

	char SendMsg[128]="";
	char ShowData[128]="";	
	char Code_cr[4]={0};
	char Comm[16]={0};
	char Comm_Lo_cr[2]={0};
	char Comm_Hi_cr[2]={0};
	char Comm_cr[8] = {0};
	int A_i[8]={0};

	::sprintf(SendMsg, "%s%s", Address, ReadCommand);	
	::sprintf(Comm, "%s", SendMsg); 
	ASCII_to_Int_Fn(Comm, A_i, 8);

	//Code
	Comm_cr[0]=A_i[6]*16+A_i[7];
	Comm_cr[1]=A_i[4]*16+A_i[5];
	Comm_cr[2]=A_i[2]*16+A_i[3];

	//Address
	Comm_cr[3]=A_i[0]*16+A_i[1];

	char ReadData[4]={0};
	char TempStr[128] = "";
	if ( USB_WRITE_Fn(Comm_cr,3,3) == FALSE ) //PE6 => pipe=3
	{	return false; }

	if ( USB_READ_Fn(ReadData,2,1) == FALSE ) //PE1 IN => pipe=1
	{	return false; }

	IntToHex(ReadData[1]&0xFF,2, TempStr);
	::strcpy(BufferStr, ShowData);
	::sprintf(ShowData, "%s%s", BufferStr, TempStr);			
	IntToHex(ReadData[0]&0xFF,2, TempStr);
	::strcpy(BufferStr, ShowData);
	::sprintf(ShowData, "%s%s", BufferStr, TempStr);
	::strcpy(strBoardID, ShowData);	
	return true;
}
//---------------------------------------------------------------------------//
void CJetUSBCtrl::IntToHex(int Value, int Digits, char str[])
{	
	JetAPI::IntToHex(Value, Digits, str);
}
//---------------------------------------------------------------------------//
void CJetUSBCtrl::ASCII_to_Int_Fn(char *Data,int *D_i,int Len_i)
{
	JetAPI::ASCII_To_Int(Data, D_i, Len_i);
}
//---------------------------------------------------------------------------//
void CJetUSBCtrl::DisConnectUSB()
{
	m_IsConnected = false;
	if (m_hDevice)
	{
		CloseHandle(m_hDevice);	m_hDevice=NULL;
	}

	this->ReleaseOverLapped();
}
//---------------------------------------------------------------------------//
bool CJetUSBCtrl::ConnectUSB(char BoardName[], char BoardCode[])//fang 1011212
{
	if( m_BoardID == -1 )
	{
		m_ErrorString = _T("Error, USB BoardID = -1!");
		return false;
	}	

	CString err = "";
	size_t BoardCodeLen = 0;
	int DeviceIndex = 0;
	char Slot[128] = "";
	char Buffer[128] = "";
	char BoardID[128] = "";	

	int i =0;
	int DeviceSearchTime = 5;	

	this->CreateOverLapped();

	for(i=0;i<DeviceSearchTime;i++)
	{
		if( OpenDriver(i)== true )
		{			
			m_IsConnected = true;
			TestReadGetID(i, Buffer);
			BoardCodeLen = ::strlen(Buffer);
			if ( BoardCodeLen < 2 ) { continue; }

			Slot[0] = Buffer[0];
			Slot[1] = Buffer[1];
			Slot[2] = '\0';

			BoardID[0] = Buffer[BoardCodeLen-2];
			BoardID[1] = Buffer[BoardCodeLen-1];
			BoardID[2] = '\0';
			if( atoi(BoardID) == m_BoardID )
			{
				m_DeviceIndex = i;
				m_IsConnected = true;
				::strcpy(BoardCode, BoardID);
				return true;
			}
			else
			{
				if( m_hDevice )	{ CloseHandle(m_hDevice);m_hDevice = NULL; }
				m_IsConnected = false;
			}
		}
		else
		{
			if( m_hDevice )	{ CloseHandle(m_hDevice);m_hDevice = NULL; }
			m_IsConnected = false;
		}
	}	
	m_ErrorString = _T("Error, USB Can't Find Device in correct BoardID! ");
	return false;
}
//-------------------------------------------------------------------//
bool CJetUSBCtrl::OpenDriver(int DeviceIndex)
{
	m_IsConnected = false;
	if ( m_hDevice )	
	{ 
		CloseHandle(m_hDevice);
		m_hDevice=NULL;	
	}

	CString err = "";
	BOOL bResult = FALSE;
	HANDLE hDevice =0;
	SP_DEVINFO_DATA devInfoData;
	SP_DEVICE_INTERFACE_DATA  devInterfaceData;
	PSP_INTERFACE_DEVICE_DETAIL_DATA functionClassDeviceData;
	ULONG requiredLength = 0;
	//	int deviceNumber = 0; // Can be other values if more than 1 device connected to driver

	HDEVINFO hwDeviceInfo = SetupDiGetClassDevs( 
		(LPGUID)&CYUSBDRV_GUID,
		NULL,
		NULL,
		DIGCF_PRESENT|DIGCF_INTERFACEDEVICE
		);

	if ( hwDeviceInfo == INVALID_HANDLE_VALUE )
	{
		JetAPI::GetSystemLastError(this->m_ErrorString);
		return m_IsConnected;
	}

	devInterfaceData.cbSize = sizeof(devInterfaceData);
	bResult = SetupDiEnumDeviceInterfaces(hwDeviceInfo, 0, (LPGUID) &CYUSBDRV_GUID, DeviceIndex, &devInterfaceData);
	if ( bResult == FALSE ) 
	{
		JetAPI::GetSystemLastError(this->m_ErrorString);
		return m_IsConnected;
	}

	SetupDiGetInterfaceDeviceDetail ( hwDeviceInfo, &devInterfaceData, NULL, 0,	&requiredLength, NULL);
	ULONG predictedLength = requiredLength;
	functionClassDeviceData = (PSP_INTERFACE_DEVICE_DETAIL_DATA) malloc(predictedLength);
	functionClassDeviceData->cbSize = sizeof (SP_INTERFACE_DEVICE_DETAIL_DATA);
	devInfoData.cbSize = sizeof(devInfoData);

	bResult = SetupDiGetInterfaceDeviceDetail (hwDeviceInfo, &devInterfaceData, functionClassDeviceData, predictedLength, &requiredLength, &devInfoData);
	if ( bResult == FALSE ) 
	{
		free(functionClassDeviceData);
		JetAPI::GetSystemLastError(this->m_ErrorString);
		return m_IsConnected;
	}

	hDevice = CreateFile (functionClassDeviceData->DevicePath,
		GENERIC_WRITE | GENERIC_READ,
		FILE_SHARE_WRITE | FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_OVERLAPPED,
		NULL);

	if ( hDevice!=NULL && hDevice!=INVALID_HANDLE_VALUE )
	{
		m_hDevice = hDevice;
		m_IsConnected = true;
	}

	free(functionClassDeviceData);
	SetupDiDestroyDeviceInfoList(hwDeviceInfo);	
	return m_IsConnected;
}
//---------------------------------------------------------------------------
bool CJetUSBCtrl::CheckUSBDevice()
{
	ULONG USB_Version = 0;
	ULONG USB_DIVersion = 0;
	UCHAR USB_EndPts = 0;
	LONG  USB_TransferSize=0;
	char USB_Name[JET_USB_TEXT_SIZE] = "";
	char USB_FridName[JET_USB_TEXT_SIZE] = "";
	if ( this->USB_GetDeviceName(this->m_hDevice, USB_Name) == false )
	{	return false; }	
	if ( this->USB_GetDeviceName(this->m_hDevice, USB_Name) == false )
	{	return false; }
	if ( this->USB_GetDriverVersion(this->m_hDevice, USB_Version) == false )
	{	return false; }
	if ( this->USB_GetFriendlyName(this->m_hDevice, USB_FridName) == false )
	{	return false; }
	if ( this->USB_GetNumberPoints(this->m_hDevice, USB_EndPts) == false )
	{	return false; }
	if ( this->USB_GetTransferSize(this->m_hDevice, 0x01, USB_TransferSize) == false )
	{	return false; }
	if ( this->USB_GetTransferSize(this->m_hDevice, 0x81, USB_TransferSize) == false )
	{	return false; }
	if ( this->USB_GetTransferSize(this->m_hDevice, 0x02, USB_TransferSize) == false )
	{	return false; }
	if ( this->USB_GetTransferSize(this->m_hDevice, 0x06, USB_TransferSize) == false )
	{	return false; }
	if ( this->USB_GetUSBDIVersion(this->m_hDevice, USB_DIVersion) == false )
	{	return false; }
	return true;
}
//---------------------------------------------------------------------------
bool CJetUSBCtrl::USB_WRITE_Fn_Direct(char *Bufer_cr,int len_i,int pipeNum)
{
	if ( this->CheckInit() == false ) { return FALSE; }

	UCHAR Address = 0x00;
	if ( this->PipeNumToAddress(pipeNum, Address) == false )
	{	return false; }

	bool bResult = this->USB_IO_Control_Direct(m_hDevice, Address, Bufer_cr, len_i, m_pWriteOvLap);
	return bResult;
}
//---------------------------------------------------------------------------
bool CJetUSBCtrl::USB_WRITE_Fn_Transfer(char *Bufer_cr,int len_i,int pipeNum)
{
	if ( this->CheckInit() == false ) { return FALSE; }

	UCHAR Address = 0x00;
	if ( this->PipeNumToAddress(pipeNum, Address) == false )
	{	return false; }

	bool bResult = this->USB_IO_Control_Transfer(m_hDevice, Address, Bufer_cr, len_i, m_pWriteOvLap);
	return bResult;
}
//---------------------------------------------------------------------------
bool CJetUSBCtrl::USB_WRITE_Fn(char *Bufer_cr,int len_i,int pipeNum)
{	
	bool bResult = false;
	bResult = USB_WRITE_Fn_Direct(Bufer_cr, len_i, pipeNum);
	//bResult = USB_WRITE_Fn_Transfer(Bufer_cr, len_i, pipeNum);	
	return bResult;
}
//---------------------------------------------------------------------------//
bool CJetUSBCtrl::USB_READ_Fn_Direct(char* Bufer_cr,int len_i,int pipeNum)
{
	if ( this->CheckInit() == false ) { return FALSE; }

	UCHAR Address = 0x00;
	if ( this->PipeNumToAddress(pipeNum, Address) == false )
	{	return false; }
	
	bool bResult = this->USB_IO_Control_Direct(m_hDevice, Address, Bufer_cr, len_i, m_pReadOvLap);
	return bResult;
}
//---------------------------------------------------------------------------//
bool CJetUSBCtrl::USB_READ_Fn_Transfer(char* Bufer_cr,int len_i,int pipeNum)
{
	if ( this->CheckInit() == false ) { return false; }

	UCHAR Address = 0x00;
	if ( this->PipeNumToAddress(pipeNum, Address) == false )
	{	return false; }
	
	bool bResult = this->USB_IO_Control_Transfer(m_hDevice, Address, Bufer_cr, len_i, m_pReadOvLap);
	return bResult;
}
//---------------------------------------------------------------------------//
bool CJetUSBCtrl::USB_READ_Fn(char* Bufer_cr,int len_i,int pipeNum)
{
	bool bResult = false;
	bResult = this->USB_READ_Fn_Direct(Bufer_cr, len_i, pipeNum);	
	//bResult = this->USB_READ_Fn_Transfer(Bufer_cr, len_i, pipeNum);		
	return bResult;
}
//---------------------------------------------------------------------------//
bool CJetUSBCtrl::GetIsConnected()
{
	return m_IsConnected;
}
//---------------------------------------------------------------------------//
bool CJetUSBCtrl::DoDeviceReset()
{
	CString str;
	str.Format(_T("%s"), _T("Error, CJetUSBCtrl::DoDeviceReset"));	
	AOIDataCollect.ShowMessage(0,  MSG_MB_OK, MSG_MB_ICONSTOP, MSG_MB_DEFBUTTON1, str, true, false);	
	this->SaveUSBLogMessage(str);

	if ( m_hDevice )
	{
		this->USB_ResetParentPort(m_hDevice);
		CloseHandle(m_hDevice);m_hDevice = NULL;
	}
	if ( OpenDriver(m_DeviceIndex) == false )
	{	return false;	}
	return true;
}
//---------------------------------------------------------------------------//
void CJetUSBCtrl::SetBoardID(int BoardID)
{
	m_BoardID = BoardID;
}
//---------------------------------------------------------------------------//
int CJetUSBCtrl::GetBoardID()
{
	return m_BoardID;
}
//---------------------------------------------------------------------------//
inline bool CJetUSBCtrl::CheckData(char data[], size_t len)
{
	char BufferStr[JET_USB_TEXT_SIZE] = "";
	while ( ::strlen(data) < len )	
	{
		::strcpy(BufferStr, data);
		::sprintf(data, "0%s", BufferStr);
	};

	if ( ::strlen(data) != len )
	{
		CString str = data;
		switch ( len )
		{
		case JET_USB_CTRL_ADDRESS_SIZE:
			m_ErrorString.Format(_T("Error, Check Address fault. [%s]"), str);
			break;
		case JET_USB_CTRL_DATA_SIZE:
			m_ErrorString.Format(_T("Error, Check Data fault. [%s]"), str);
			break;
		case JET_USB_CTRL_ADDRESS_BIT_SIZE:
			m_ErrorString.Format(_T("Error, Check Address Bit fault. [%s]"), str);
			break;
		case JET_USB_CTRL_DATA_BIT_SIZE:
			m_ErrorString.Format(_T("Error, Check Data Bit fault. [%s]"), str);
			break;
		default:
			m_ErrorString.Format(_T("Error, Address or Data fault. [%s]"), str);
			break;
		}		
		return false;
	}
	return true;
}
//---------------------------------------------------------------------------//
void CJetUSBCtrl::SetCmdTextReadData(char Cmd[])//設定電腦讀取資料命令
{
	::strcpy(m_CmdTextReadData, Cmd);	
}
//---------------------------------------------------------------------------//
void CJetUSBCtrl::SetCmdTextWriteData(char Cmd[])//設定電腦寫入資料命令
{
	::strcpy(m_CmdTextWriteData, Cmd);
}
//---------------------------------------------------------------------------//
void CJetUSBCtrl::SetCmdTextReadAddress(char Cmd[])//設定電腦讀取位址命令
{
	::strcpy(m_CmdTextReadAddress, Cmd);
}
//---------------------------------------------------------------------------//
void CJetUSBCtrl::SetCmdTextWriteAddress(char Cmd[])//設定電腦寫入位址命令
{
	::strcpy(m_CmdTextWriteAddress, Cmd);
}
//---------------------------------------------------------------------------//
bool CJetUSBCtrl::PipeNumToAddress(int PipeNum, UCHAR &Address)
{
	switch ( PipeNum )
	{
	case 0:	Address = 0x01;
		break;
	case 1:	Address = 0x81;
		break;
	case 2:	Address = 0x02;
		break;
	case 3:	Address = 0x06;	
		break;
	default:
		return false;
	}
	return true;
}
//---------------------------------------------------------------------------//
bool CJetUSBCtrl::ReleaseOverLapped()
{
	if ( this->m_pWriteOvLap != NULL )
	{
		if ( this->m_pWriteOvLap->hEvent != NULL )
		{	
			::CloseHandle(m_pWriteOvLap->hEvent); 
			m_pWriteOvLap->hEvent = NULL;
		}
		delete m_pWriteOvLap; m_pWriteOvLap=NULL;
	}
	
	if ( m_pReadOvLap != NULL )
	{
		if ( m_pReadOvLap->hEvent != NULL )
		{
			::CloseHandle(m_pReadOvLap->hEvent); 
			m_pReadOvLap->hEvent = NULL;
		}
		delete m_pReadOvLap; m_pReadOvLap=NULL;
	}
	return true;
}
//---------------------------------------------------------------------------//
bool CJetUSBCtrl::CreateOverLapped()
{
	this->ReleaseOverLapped();
	if ( this->m_bOverLapped == FALSE ) { return true; }

	m_pWriteOvLap = new OVERLAPPED;
	m_pReadOvLap = new OVERLAPPED;

	if ( m_pWriteOvLap==NULL || m_pReadOvLap==NULL )
	{
		delete m_pWriteOvLap; m_pWriteOvLap=NULL;
		delete m_pReadOvLap; m_pReadOvLap=NULL;
		return false;
	}
	::memset(m_pWriteOvLap, 0x00, sizeof(OVERLAPPED));
	::memset(m_pReadOvLap, 0x00, sizeof(OVERLAPPED));

	m_pWriteOvLap->hEvent = ::CreateEventA(NULL, false, false, "CYUSB_Write");
	m_pReadOvLap->hEvent  = ::CreateEventA(NULL, false, false, "CYUSB_Read");
	return true;
}
//---------------------------------------------------------------------------//
bool CJetUSBCtrl::USB_ResetParentPort(HANDLE hDevice)
{
	if ( CJetUSBCtrl::CheckDeviceHandle(hDevice) == false )
	{	return false; }

	DWORD dwBytes;
	BOOL bResult=FALSE;
	bResult = DeviceIoControl(
		hDevice, 
		IOCTL_ADAPT_RESET_PARENT_PORT,
		NULL, 0,
		NULL, 0,
		&dwBytes, NULL
		);
	if ( FALSE == bResult )
	{	
		JetAPI::GetSystemLastError(this->m_ErrorString);	
		return false;
	}
	return true;
}
//---------------------------------------------------------------------------//
bool CJetUSBCtrl::USB_AbortPipe(HANDLE hDevice, UCHAR Address)
{
	if ( CJetUSBCtrl::CheckDeviceHandle(hDevice) == false )
	{	return false; }

	DWORD dwBytes = 0;
	BOOL bResult=FALSE;
	bResult = DeviceIoControl(
		hDevice, 
		IOCTL_ADAPT_ABORT_PIPE,
       &Address, sizeof (UCHAR),
        NULL, 0,
       &dwBytes, NULL
	   );
	if ( FALSE == bResult )
	{	
		JetAPI::GetSystemLastError(this->m_ErrorString);	
		return false;
	}
	return true;
}
//---------------------------------------------------------------------------//
bool CJetUSBCtrl::USB_CyclePort(HANDLE hDevice)
{
	if ( CJetUSBCtrl::CheckDeviceHandle(hDevice) == false )
	{	return false; }

	DWORD dwBytes = 0;
	BOOL bResult=FALSE;
	bResult = DeviceIoControl(
		hDevice, 
		IOCTL_ADAPT_CYCLE_PORT,
        NULL, 0,
        NULL, 0,
       &dwBytes, NULL
	   );
	if ( FALSE == bResult )
	{	
		JetAPI::GetSystemLastError(this->m_ErrorString);	
		return false;
	}
	return true;
}
//---------------------------------------------------------------------------//
bool CJetUSBCtrl::USB_GetDeviceName(HANDLE hDevice, char Name[])
{
	if ( CJetUSBCtrl::CheckDeviceHandle(hDevice) == false )
	{	return false; }	

	DWORD dwBytes = 0;
	BOOL bResult=FALSE;
	const ULONG len = 256;
	UCHAR buf[len] = {0};

	bResult = DeviceIoControl(
		hDevice, 
		IOCTL_ADAPT_GET_DEVICE_NAME,
        buf, len,
        buf, len,
       &dwBytes, NULL
	   );	
	if ( bResult == FALSE )
	{	
		JetAPI::GetSystemLastError(this->m_ErrorString);	
		return false;
	}
	::strcpy(Name, (const char*)buf);	
	return true;
}
//---------------------------------------------------------------------------//
bool CJetUSBCtrl::USB_GetDriverVersion(HANDLE hDevice, ULONG &version)
{
	if ( CJetUSBCtrl::CheckDeviceHandle(hDevice) == false )
	{	return false; }

	DWORD dwBytes = 0;
	BOOL bResult=FALSE;	
	bResult = DeviceIoControl(
		hDevice, 
		IOCTL_ADAPT_GET_DRIVER_VERSION,
		&version, sizeof(version),
		&version, sizeof(version),
		&dwBytes, NULL
		);
	if ( bResult == FALSE )
	{	
		JetAPI::GetSystemLastError(this->m_ErrorString);	
		return false;
	}
	return true;
}
//---------------------------------------------------------------------------//
bool CJetUSBCtrl::USB_GetFriendlyName(HANDLE hDevice, char Name[])
{
	if ( CJetUSBCtrl::CheckDeviceHandle(hDevice) == false )
	{	return false; }

	DWORD dwBytes = 0;
	BOOL bResult=FALSE;	
	const ULONG len = 256;
	UCHAR buf[len] = {0};

	bResult = DeviceIoControl(
		hDevice, 
		IOCTL_ADAPT_GET_FRIENDLY_NAME,
        buf, len,
        buf, len,
       &dwBytes, NULL
	   );
	if ( bResult == FALSE )
	{	
		JetAPI::GetSystemLastError(this->m_ErrorString);	
		return false;
	}	
	::strcpy(Name, (const char*)buf);	
	return true;
}
//---------------------------------------------------------------------------//
bool CJetUSBCtrl::USB_GetNumberPoints(HANDLE hDevice, UCHAR &endPts)
{
	if ( CJetUSBCtrl::CheckDeviceHandle(hDevice) == false )
	{	return false; }	

	DWORD dwBytes = 0;
	BOOL bResult=FALSE;	

	bResult = DeviceIoControl(
		hDevice, 
		IOCTL_ADAPT_GET_NUMBER_ENDPOINTS,
		NULL, 0,
		&endPts, sizeof (endPts),
		&dwBytes, NULL
		);
	if ( bResult == FALSE )
	{	
		JetAPI::GetSystemLastError(this->m_ErrorString);	
		return false;
	}
	return true;
}
//---------------------------------------------------------------------------//
bool CJetUSBCtrl::USB_GetTransferSize(HANDLE hDevice, UCHAR Address, LONG &transferSz)
{
	if ( CJetUSBCtrl::CheckDeviceHandle(hDevice) == false )
	{	return false; }		

	DWORD BytesXfered;
	BOOL bResult=FALSE;	
	SET_TRANSFER_SIZE_INFO SetTransferInfo;

	SetTransferInfo.EndpointAddress = Address;
	bResult = DeviceIoControl(
		hDevice,  
		IOCTL_ADAPT_GET_TRANSFER_SIZE,
       &SetTransferInfo, sizeof (SET_TRANSFER_SIZE_INFO),
       &SetTransferInfo, sizeof (SET_TRANSFER_SIZE_INFO),
       &BytesXfered,  NULL
	   );

	transferSz = SetTransferInfo.TransferSize;
	if ( bResult == FALSE )
	{	
		JetAPI::GetSystemLastError(this->m_ErrorString);	
		return false;
	}
	return true;
}
//---------------------------------------------------------------------------//
bool  CJetUSBCtrl::USB_GetUSBDIVersion(HANDLE hDevice, ULONG &version)
{
	if ( CJetUSBCtrl::CheckDeviceHandle(hDevice) == false )
	{	return false; }		

	DWORD dwBytes = 0;
	BOOL bResult=FALSE;	
	
	bResult = DeviceIoControl(
		hDevice, 
		IOCTL_ADAPT_GET_USBDI_VERSION,
		&version, sizeof(version),
		&version, sizeof(version),
		&dwBytes, NULL
		);
	if ( bResult == FALSE )
	{	
		JetAPI::GetSystemLastError(this->m_ErrorString);	
		return false;
	}
	return true;
}
//---------------------------------------------------------------------------//
bool CJetUSBCtrl::USB_IO_Control_Direct(HANDLE hDevice, UCHAR Address, char *Bufer_cr, int len_i, OVERLAPPED *ov)
{
	if ( CJetUSBCtrl::CheckDeviceHandle(hDevice) == false )
	{	return false; }			
	
	BOOL bResult=FALSE;
	LONG bufLen = len_i;	

	const int iXmitBufSize = sizeof (SINGLE_TRANSFER);
	PUCHAR pXmitBuf[iXmitBufSize]={0};
	ZeroMemory (pXmitBuf, iXmitBufSize);

	PSINGLE_TRANSFER pTransfer = (PSINGLE_TRANSFER) pXmitBuf;
	pTransfer->ucEndpointAddress = Address;	
	pTransfer->IsoPacketLength = 0;
	pTransfer->BufferOffset = 0;
	pTransfer->BufferLength = 0;	
	DWORD dwReturnBytes;

	bResult = DeviceIoControl(
		hDevice,
		IOCTL_ADAPT_SEND_NON_EP0_DIRECT,
		pXmitBuf, iXmitBufSize,
		Bufer_cr, bufLen,
		&dwReturnBytes, ov
		);	

	DWORD Res = WAIT_OBJECT_0;	
	if ( ov != NULL )
	{
		Res = ::WaitForSingleObject(ov->hEvent, m_USBTimeOut);		
		if ( Res == WAIT_OBJECT_0 ) 
		{	bResult = TRUE;	}
	}
	
	if ( bResult == FALSE ) 
	{
		JetAPI::GetSystemLastError(m_ErrorString);		
		if ( Res == WAIT_TIMEOUT )
		{	this->USB_AbortPipe(hDevice, Address);	}
		return false;
	}

	return true;
}
//---------------------------------------------------------------------------//
bool CJetUSBCtrl::USB_IO_Control_Transfer(HANDLE hDevice, UCHAR Address, char *Bufer_cr, int len_i, OVERLAPPED *ov)
{
	if ( CJetUSBCtrl::CheckDeviceHandle(hDevice) == false )
	{	return false; }		

	BOOL bResult=FALSE;
	int bufLen = len_i;
	const int MaxBufSize = 4096;
	const int iXmitBufSize = sizeof (SINGLE_TRANSFER)+bufLen;
	if ( iXmitBufSize > MaxBufSize ) 
	{	return FALSE; }

	UCHAR pXmitBuf[MaxBufSize];
	ZeroMemory (pXmitBuf, iXmitBufSize);

	PSINGLE_TRANSFER pTransfer = (PSINGLE_TRANSFER) pXmitBuf;	
	UCHAR *ptr = pXmitBuf + sizeof(SINGLE_TRANSFER);

	pTransfer->ucEndpointAddress = Address;
	pTransfer->reserved = 0;	
	pTransfer->IsoPacketLength = 0;
	pTransfer->BufferOffset = sizeof(SINGLE_TRANSFER);
	pTransfer->BufferLength = bufLen;

	::memcpy(ptr, Bufer_cr, sizeof(char)*len_i);

	DWORD dwReturnBytes;
	DWORD dwReturnDataBytes;	
	bResult = DeviceIoControl(
		hDevice, 
		IOCTL_ADAPT_SEND_NON_EP0_TRANSFER,
		pXmitBuf, iXmitBufSize,
		pXmitBuf, iXmitBufSize,
		&dwReturnBytes, ov);

	DWORD Res = WAIT_OBJECT_0;	
	if ( ov != NULL )
	{
		Res = ::WaitForSingleObject(ov->hEvent, m_USBTimeOut);
		if ( Res == WAIT_OBJECT_0 )
		{	bResult = TRUE;	}	
	}	
	
	if ( bResult == FALSE )
	{
		JetAPI::GetSystemLastError(m_ErrorString);		
		if ( Res == WAIT_TIMEOUT )//before delete buffer, should abort the address.
		{	this->USB_AbortPipe(hDevice, Address);	}
		return false;
	}

	// Copy data into  buf
	ptr = pXmitBuf + sizeof(SINGLE_TRANSFER);
	if ( dwReturnBytes > 0 ) 
	{
		dwReturnDataBytes = dwReturnBytes-sizeof(SINGLE_TRANSFER);
		if ( dwReturnDataBytes > 0 ) 
		{	
			if ( dwReturnDataBytes > len_i ) 
			{	dwReturnDataBytes = len_i; }				
		}
	}
	else
	{	dwReturnDataBytes = len_i;	}
	memcpy(Bufer_cr, ptr, dwReturnDataBytes);			
	return TRUE;	
}
//---------------------------------------------------------------------------//
void CJetUSBCtrl::SetSaveLogMsg(bool Save)
{
	m_SaveLogMsg = Save;
}
//---------------------------------------------------------------------------//
bool CJetUSBCtrl::SaveUSBLogMessage(LPCTSTR Info)
{	
	if ( false == m_SaveLogMsg ) { return true; }

	FILE *pfile = NULL;
	TCHAR   TMode[32] = _T("");
	_tcscpy(TMode, _T("a+"));
	JetAPI::ModifyOpenFileMode_Write(TMode);
	pfile = ::_tfopen(this->m_USBDebugFile, TMode);
	if ( pfile == NULL ) { return false; }

	CString tp;
	JetAPI::GetTime(tp,CTime::GetCurrentTime());	
	//::_ftprintf(pfile, _T("%s\n"), Info);
	_ftprintf(pfile, _T("%s/%s/%s,%s:%s:%s %s\n"),tp.Mid(0,4),tp.Mid(4,2),tp.Mid(6,2),tp.Mid(8,2),tp.Mid(10,2),tp.Mid(12,2), Info);			

	::fclose(pfile);
	pfile = NULL;
	return true;
}
//---------------------------------------------------------------------------//