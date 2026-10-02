// Barcode_Honeywell_3310GHD.cpp: implementation of the CBarcode_Honeywell_3310GHD class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "Barcode_Honeywell_3310GHD.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
CBarcode_Honeywell_3310GHD::CBarcode_Honeywell_3310GHD()
{
	PreInitBarcodeDevice_3310GHD();
	InitialBarcodeDevice_3310GHD();
}
//-------------------------------------------------------------------------------------//
CBarcode_Honeywell_3310GHD::CBarcode_Honeywell_3310GHD(const CBarcode_Honeywell_3310GHD &device):CBarcode_Basic(device)
{
	PreInitBarcodeDevice_3310GHD();
	CloneBarcodeDevice_3310GHD(device);
}
//-------------------------------------------------------------------------------------//
CBarcode_Honeywell_3310GHD::~CBarcode_Honeywell_3310GHD()
{
	Disconnect_Honeywell_3310GHD();
}
//-------------------------------------------------------------------------------------//
CBarcode_Honeywell_3310GHD& CBarcode_Honeywell_3310GHD::operator=(const CBarcode_Honeywell_3310GHD &device)
{
	if ( this == &device ) { return *this; }
	CBarcode_Basic::operator=(device);
	CloneBarcodeDevice_3310GHD(device);
	return *this;
}
//-------------------------------------------------------------------------------------//
void CBarcode_Honeywell_3310GHD::PreInitBarcodeDevice_3310GHD()
{
	BARCODE_DEVICE_TYPE DeviceType = BARCODE_DEVICE_HONEYWELL_3310GHD;
	CString ModelName = AOIDataDefine.GetBarcodeDeviceName(DeviceType);
	SetBarcodeDeviceType(DeviceType);//條碼機樣式
	SetBarcodeDeviceModel(ModelName);//條碼機型號		
	::memset(m_DeviceNoRead, 0x00, sizeof(m_DeviceNoRead));
}
//-------------------------------------------------------------------------------------//
void CBarcode_Honeywell_3310GHD::InitialBarcodeDevice_3310GHD()
{	
	m_StartCommand = 0x16;	//$
	m_EndCommand = 0x0D;    //<CR>
	m_SeparatorChar = 0x0d;
	strcpy(m_DeviceNoRead, "NR");
	m_TriggerChar = 0x54;
	m_TriggerOffChar = 0x55;
}
//-------------------------------------------------------------------------------------//
void CBarcode_Honeywell_3310GHD::CloneBarcodeDevice_3310GHD(const CBarcode_Honeywell_3310GHD &device)
{
	m_StartCommand = device.m_StartCommand;	//$
	m_EndCommand = device.m_EndCommand;    //<CR>	
	m_SeparatorChar = device.m_SeparatorChar;	
	::strcpy(m_DeviceNoRead, device.m_DeviceNoRead);	
	m_TriggerChar = device.m_TriggerChar;
	m_TriggerOffChar = device.m_TriggerOffChar;	
}
//-------------------------------------------------------------------------------------//
bool CBarcode_Honeywell_3310GHD::LoadBarcodeINIFile()
{
	if ( CBarcode_Basic::LoadBarcodeINIFile() == false ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcode_Honeywell_3310GHD::SaveBarcodeINIFile()
{
	if ( CBarcode_Basic::SaveBarcodeINIFile() == false ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
CString CBarcode_Honeywell_3310GHD::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)//載入多國語系
{
	return CBarcode_Basic::LoadMultiLanguageString(KeyName, Default);
}
//-------------------------------------------------------------------------------------//
bool CBarcode_Honeywell_3310GHD::ConnectDevice_Honeywell_3310GHD()//連線
{		
	int nBaud = 115200;
	CString   strPort = GetBarcodeDevicePort();
	const int nByteSize = 8;
	const int nParity = SERIES_PARITY_NONE;
	const int nStopBits = 0;//SERIES_STOPBITS_15;	
	const int nPort = JetAPI::StrToInt(strPort);

	int Baud[7];
	Baud[6] = 2400;
	Baud[5] = 4800;
	Baud[4] = 9600;
	Baud[3] = 19200;
	Baud[2] = 38400;
	Baud[1] = 57600;
	Baud[0] = 115200;

	int i = 0; 
	bool IsConnected = true;
	//bool IsConnected = false;
	CString str=_T("");
	CString strDeviceName;
	
	//斷線
	Disconnect_Honeywell_3310GHD();

	//載入參數檔案
	LoadBarcodeINIFile();
	SaveBarcodeINIFile();

	ClearResultBuffer();	
	ClearResultBackup();
	m_ErrorString = _T("");		
	strDeviceName = GetBarcodeDeviceFullName();
	for( i=0 ; i<7 ; i++ )
	{
		if ( this->m_RS232COM.IsOpened() )
		{ this->m_RS232COM.Close(); }

		nBaud = Baud[i];
		if ( m_RS232COM.Open(nPort, nBaud, nByteSize, SERIES_PARITY_NONE, 0) == false )
		{
			IsConnected = false;
			m_ErrorString.Format(_T("%s::RS232 Connected Fault [Port::%d]"), strDeviceName, nPort);
			continue; 
		}
		
		if( !m_RS232COM.IsOpened() ) 
		{ 
			IsConnected = false;
			m_ErrorString.Format(_T("%s::RS232 Connected Fault [Port::%d]"), strDeviceName, nPort);
			continue; 
		}

		//讀取條碼機回傳值
		IsConnected = true;
		SetBarcodeDeviceConnected(IsConnected);	
		this->ClearBuffer();
		if ( IsConnected == false )
		{
			if ( this->m_RS232COM.IsOpened() )
			{ this->m_RS232COM.Close(); }

			m_ErrorString.Format(_T("%s::RS232 Connected Fault [Port::%d]"), strDeviceName, nPort);
			continue;
		}
		break;
	}	
	if ( IsConnected == true )
	{	m_ErrorString = _T("");	}
	else
	{	this->m_RS232COM.Close(); }
	SetBarcodeDeviceConnected(IsConnected);	
	return IsConnected;	
}
//-------------------------------------------------------------------------------------//
bool CBarcode_Honeywell_3310GHD::Disconnect_Honeywell_3310GHD()//斷線
{
	if ( this->m_RS232COM.IsOpened() )
	{	this->m_RS232COM.Close();	}
	SetBarcodeDeviceConnected(false);
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CBarcode_Honeywell_3310GHD::ConnectToDevice()
{
	return ConnectDevice_Honeywell_3310GHD();
}
//-------------------------------------------------------------------------------------//
bool CBarcode_Honeywell_3310GHD::Disconnected()
{	
	return Disconnect_Honeywell_3310GHD();
}
//-------------------------------------------------------------------------------------//
bool CBarcode_Honeywell_3310GHD::CheckConnected()//確認是否連線
{
	if ( m_RS232COM.IsOpened() == false ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
void CBarcode_Honeywell_3310GHD::ClearBuffer()
{
	char temp=' ';	
	if ( CheckConnected() == false ) { return ; }
	while(this->m_RS232COM.ReadSingleChar(temp) != 0)
	{}
}
//-------------------------------------------------------------------------------------//
bool CBarcode_Honeywell_3310GHD::StartToRead_Honeywell_3310GHD()//開始讀取
{	
	const size_t szBuffer=128;
	char Buffer[szBuffer] = "";

	ClearBuffer();	
	CBarcode_Basic::ClearResultBuffer();		
	SetBarcodeDeviceActived(false);
	::sprintf(Buffer, "%c%c%c", m_StartCommand, m_TriggerChar, m_EndCommand);
	if ( SendData_Honeywell_3310GHD(Buffer) == false )	
	{	return false; }
	SetBarcodeDeviceActived(true);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcode_Honeywell_3310GHD::EndReading_Honeywell_3310GHD()//關閉讀取
{	
	const size_t szBuffer=128;
	char Buffer[szBuffer] = "";

	ClearBuffer();
	CString str;
	::sprintf(Buffer, "%c%c%c", m_StartCommand, m_TriggerOffChar, m_EndCommand);
	bool IsOK = SendData_Honeywell_3310GHD(Buffer);
	SetBarcodeDeviceActived(false);
	//BarCode_CS::StopReading();	
	return IsOK;	
}
//-------------------------------------------------------------------------------------//
bool CBarcode_Honeywell_3310GHD::WaitForDataInQuene_Honeywell_3310GHD()//等待有資料進來
{
	int WaitCount = 0;
	const int MaxWaitCounts = m_BarcodeDeviceWaitDataCount;
	while( m_RS232COM.ReadDataWaiting()==0 )
	{
		Sleep(m_BarcodeDeviceWaitDataDwellTime);
		WaitCount++;
		if ( WaitCount > MaxWaitCounts )
		{	return false;	}
	};
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcode_Honeywell_3310GHD::RetrieveCode_Honeywell_3310GHD()//接收裝置內的條碼
{
	ClearResultBackup();
	CString strDeviceName = GetBarcodeDeviceFullName();
	const bool DeviceActived = GetBarcodeDeviceActived();	
	if( false == DeviceActived )
	{
		m_ErrorString.Format(_T("%s::did not actived"), strDeviceName);
		return false;
	}

	if ( WaitForDataInQuene_Honeywell_3310GHD() == false )
	{	
		m_ErrorString.Format(_T("%s::wait for data too long"), strDeviceName);
		return false; 
	}

	CString str = _T("");		
	int nReads=0;
	int i=0;
	const size_t length = MAX_BARCODE_DEVICE_RECEIVE_SIZE;
	char Msg[length+32]="";
	char Buffer[length+32]="";
	char SymbolNR[16] = "NR";

	memset(Msg, 0x00, sizeof(Msg));
	memset(Buffer, 0x00, sizeof(Buffer));		

	//等資料全部送出來
	if ( m_BarcodeDeviceReadDataDelayTime > 0 ) 
	{	::Sleep(m_BarcodeDeviceReadDataDelayTime); }

	for ( i=0; i<30; i++ )
	{
		nReads = m_RS232COM.ReadData_Syn(Msg, length);
		if ( nReads > 0 ) 
		{	break; }
		if (  20==i ) 
		{
			m_ErrorString.Format(_T("%s::read rs232 fault"), strDeviceName);
			return false; 
		}		
		Sleep(10);
	}
	::strcpy(Buffer, Msg);
	_strupr(Buffer);
	if ( Buffer[0]=='N' && Buffer[1]=='R' && Buffer[2]==0x0D )//確認是否為No Read
	{ 	
		::strcpy(Msg, "No Read");
		size_t lln  = ::strlen(Msg);
		Msg[lln] = m_SeparatorChar;
		Msg[lln+1] = '\0';
	}

	const size_t MsgLen= ::strlen(Msg);
	const size_t BeforBufferLen = ::strlen(GetResultBuffer());	
	const size_t AfterBufferLen=MsgLen+BeforBufferLen;
	if ( AfterBufferLen >= length )
	{
		m_ErrorString.Format(_T("%s::read content over-flow[%d]"), strDeviceName, AfterBufferLen);
		return false;
	}
	if (BeforBufferLen > 0) //確認是否有讀到之前的
	{
		::sprintf(Buffer, "%s%s", GetResultBuffer(), Msg);
		::strcpy(Msg, Buffer);
	}

	CBarcode_Basic::SetResultBuffer(Msg);		
	AnalysisResultBuffer();
	BackupResultBuffer();
	m_ErrorString = _T("");
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CBarcode_Honeywell_3310GHD::SendData_Honeywell_3310GHD(const char *buffer)
{
	if ( NULL == buffer ) { return false; }
	const int len = (int)(::strlen(buffer));
	if ( GetBarcodeDeviceConnected() == false )
	{
		CString strDeviceName = GetBarcodeDeviceFullName();
		m_ErrorString.Format(_T("%s::does not connect"), strDeviceName);
		return false;
	}

	if ( m_RS232COM.SendData(buffer, len) == 0 )
	{
		CString str(buffer);
		CString strDeviceName = GetBarcodeDeviceFullName();
		m_ErrorString.Format(_T("%s::send data fault[%s]"), strDeviceName, str);		
		return false;
	}	

	if ( m_BarcodeDeviceCommDelayTime > 0 ) 
	{	::Sleep(m_BarcodeDeviceCommDelayTime);	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcode_Honeywell_3310GHD::Initialize()
{
	const size_t szBuffer=128;
	char Buffer[szBuffer] = "";

	::sprintf(Buffer, "%c%s%c", m_StartCommand, "CSNRM01", m_EndCommand);//操作模式 0: one shot, 1: Continue, 2: Phase Mode	
	if( SendData_Honeywell_3310GHD(Buffer) == false ) { return false; }//設定參數

	::sprintf(Buffer, "%c%s%c", m_StartCommand, "CSTON3C53543E00000000000000000000000000000000", m_EndCommand);	//設定Trigger ON 字元 <ST>
	if( SendData_Honeywell_3310GHD(Buffer) == false ) { return false; }//設定參數	

	::sprintf(Buffer, "%c%s%c", m_StartCommand, "CSTOF3C45543E00000000000000000000000000000000", m_EndCommand);	//設定Trigger OFF 字元 <ET>
	if( SendData_Honeywell_3310GHD(Buffer) == false ) { return false; }//設定參數

	::sprintf(Buffer, "%c%s%c", m_StartCommand, "CLFSU2B00000000000000000000000000000000000000", m_EndCommand);	//設定末碼字元
	if( SendData_Honeywell_3310GHD(Buffer) == false ) { return false; }//設定參數

	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcode_Honeywell_3310GHD::StartToRead()
{
	if ( StartToRead_Honeywell_3310GHD() == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcode_Honeywell_3310GHD::EndReading()
{
	if ( EndReading_Honeywell_3310GHD() == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcode_Honeywell_3310GHD::RetrieveCode()//接收裝置內的條碼
{
	if ( RetrieveCode_Honeywell_3310GHD() == false )
	{	return false; }
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CBarcode_Honeywell_3310GHD::AnalysisResultBuffer()//分析結果字串
{	
	int    SubCount=0;
	bool   GetSubCode=false;
	size_t i=0, j=0, k=0;
	const size_t BufferLen=1024;
	char SubBuffer[BufferLen+1]="";
	char ContextBuffer[BufferLen+1]="";
	const char tmpCR = 0x0D;
	const char  *ResultStr=GetResultBuffer();
	const size_t ResultLen = ::strlen(ResultStr);		
	CString strDeviceName = GetBarcodeDeviceFullName();	
	if ( ResultLen > BufferLen )
	{
		m_ErrorString.Format(_T("%s::Barcode Len is bigger than buffer size [%d/%d]"), strDeviceName, ResultLen, BufferLen);
		return false;
	}	
	SubCount=0;
	strcpy(ContextBuffer, GetResultBuffer());
	const size_t ContextLen = ::strlen(ContextBuffer);
	for ( i=0; i<ContextLen; i++ )
	{
		k = 0;
		GetSubCode=false;
		::memset(SubBuffer, 0x00, sizeof(SubBuffer));
		for ( j=i; j<ContextLen; j++ )
		{
			if ( ContextBuffer[j] == m_SeparatorChar )
			{	break; }
			SubBuffer[k++] = ContextBuffer[j];
		}
		SubBuffer[k++] = '\0';
		SetResultSubBuffer(SubCount, SubBuffer);		
		SubCount ++;
		i = j;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcode_Honeywell_3310GHD::CloneResultBuffer(char *Buffer, size_t BufferSize)//複製結果暫存區
{
	size_t i=0, j=0, k=0;	
	const char ShowSeparatorChar='+';
	const size_t BufferLen=1024;
	char ContextBuffer[BufferLen+1]="";
	const char  *ResultStr=GetResultBuffer();	
	const size_t ResultLen = ::strlen(ResultStr);		
	const size_t CopyLen = MIN(ResultLen, BufferLen);
	::memcpy(ContextBuffer, ResultStr, sizeof(char)*(CopyLen+1));	
	const size_t ContextLen = ::strlen(ContextBuffer);	
	//將內部分隔符號轉成顯示的分隔符號
	for ( i=0; i<ContextLen; i++ )
	{
		if ( ContextBuffer[i] == m_SeparatorChar )
		{	ContextBuffer[i] = ShowSeparatorChar; }
	}
	
	if ( ShowSeparatorChar == ContextBuffer[ContextLen-1] )
	{	ContextBuffer[ContextLen-1] = '\0';	}

	const size_t MinLen = MIN(ContextLen, BufferSize-1);
	::memcpy(Buffer, ContextBuffer, sizeof(char)*MinLen);
	Buffer[MinLen] = '\0';
	return true;
}
//-------------------------------------------------------------------------------------//
