// Barcode_Azurewave.cpp: implementation of the CBarcode_Azurewave class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
//#include "jet8000.h"
#include "Barcode_Azurewave.h"
//-------------------------------------------------------------------------------------//
#pragma warning (disable:4996)// _CRT_SECURE_NO_WARNINGS
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
CBarcode_Azurewave::CBarcode_Azurewave(const CBarcode_Azurewave &device) :CBarcode_Basic(device)
{
	PreInitBarcodeDevice_Azurewave();
	CloneBarcodeDevice_Azurewave(device);
}
//-------------------------------------------------------------------------------------//
void CBarcode_Azurewave::CloneBarcodeDevice_Azurewave(const CBarcode_Azurewave &device)
{
	return;
}
//-------------------------------------------------------------------------------------//
CBarcode_Azurewave& CBarcode_Azurewave::operator=(const CBarcode_Azurewave &device)
{
	if (this == &device) { return *this; }
	CBarcode_Basic::operator=(device);
	CloneBarcodeDevice_Azurewave(device);
	return *this;
}
//-------------------------------------------------------------------------------------//
void CBarcode_Azurewave::PreInitBarcodeDevice_Azurewave()
{
	BARCODE_DEVICE_TYPE DeviceType = BARCODE_DEVICE_OTHER_AZUREWAVE;
	CString ModelName = AOIDataDefine.GetBarcodeDeviceName(DeviceType);
	SetBarcodeDeviceType(DeviceType);//條碼機樣式
	SetBarcodeDeviceModel(ModelName);//條碼機型號
	return;
}
//-------------------------------------------------------------------------------------//
void CBarcode_Azurewave::InitialBarcodeDevice_Azurewave()
{
	return;
}
//-------------------------------------------------------------------------------------//
bool CBarcode_Azurewave::ConnectDevice_Azurewave()//連線
{
	int nBaud = 115200;
	CString   strPort = GetBarcodeDevicePort();
	const int nByteSize = 8;
	const int nParity = SERIES_PARITY_NONE;
	const int nStopBits = ONESTOPBIT;	
	const int nPort = JetAPI::StrToInt(strPort);	
	
	const int Baud[]={CBR_9600};	
	const int MaxBaud=sizeof(Baud)/sizeof(Baud[0]);	

	int i = 0;
	bool IsConnected = true;
	//bool IsConnected = false;
	CString str = _T("");
	CString strDeviceName;

	//斷線
	Disconnect_Azurewave();

	//載入參數檔案
	LoadBarcodeINIFile();
	SaveBarcodeINIFile();

	ClearResultBuffer();
	ClearResultBackup();
	m_ErrorString = _T("");
	strDeviceName = GetBarcodeDeviceFullName();
	for (i = 0; i < MaxBaud; i++)
	{
		if (this->m_RS232COM.IsOpened())
		{	this->m_RS232COM.Close();	}

		nBaud = Baud[i];
		if (m_RS232COM.Open(nPort, nBaud, nByteSize, nParity, nStopBits) == false)
		{
			IsConnected = false;
			m_ErrorString.Format(_T("%s::RS232 Connected Fault [Port::%d]"), strDeviceName, nPort);
			continue;
		}

		if (!m_RS232COM.IsOpened())
		{
			IsConnected = false;
			m_ErrorString.Format(_T("%s::RS232 Connected Fault [Port::%d]"), strDeviceName, nPort);
			continue;
		}

		//讀取條碼機回傳值
		IsConnected = true;
		SetBarcodeDeviceConnected(IsConnected);
		this->ClearBuffer();		
		break;
	}
	if (IsConnected == true)
	{	m_ErrorString = _T("");	}
	else
	{	this->m_RS232COM.Close();	}
	SetBarcodeDeviceConnected(IsConnected);
	return IsConnected;
}
//-------------------------------------------------------------------------------------//
bool CBarcode_Azurewave::Disconnect_Azurewave()//斷線
{
	if (this->m_RS232COM.IsOpened())
	{	this->m_RS232COM.Close();	}
	SetBarcodeDeviceConnected(false);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcode_Azurewave::StartToRead_Azurewave()//開始讀取
{
	const bool clrbuf=false;
	CBarcode_Basic::ClearResultBuffer();
	if (EndReading_Azurewave(clrbuf) == false)
	{	return false;	}	
	SetBarcodeDeviceActived(true);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcode_Azurewave::EndReading_Azurewave(bool clrbuf)//關閉讀取
{
	if ( true==clrbuf )
	{	ClearBuffer();	}
	SetBarcodeDeviceActived(false);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcode_Azurewave::WaitForDataInQuene_Azurewave()//等待有資料進來
{
	int WaitCount = 0;
	const int MaxWaitCounts = m_BarcodeDeviceWaitDataCount;
	while (m_RS232COM.ReadDataWaiting() == 0)
	{
		Sleep(m_BarcodeDeviceWaitDataDwellTime);
		WaitCount++;
		if (WaitCount > MaxWaitCounts)
		{
			return false;
		}
	};
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcode_Azurewave::RetrieveCode_Azurewave()//接收裝置內的條碼
{
	ClearResultBackup();
	CString strDeviceName = GetBarcodeDeviceFullName();
	const bool DeviceActived = GetBarcodeDeviceActived();
	if (false == DeviceActived)
	{
		m_ErrorString.Format(_T("%s::did not actived"), strDeviceName);
		return false;
	}

	if (WaitForDataInQuene_Azurewave() == false)
	{
		m_ErrorString.Format(_T("%s::wait for data too long"), strDeviceName);
		return false;
	}

	CString str = _T("");
	int i = 0;
	int nReads = 0;
	const size_t length = MAX_BARCODE_DEVICE_RECEIVE_SIZE;
	char Msg[length + 32] = "";
	char Buffer[length + 32] = "";

	memset(Msg, 0x00, sizeof(Msg));
	memset(Buffer, 0x00, sizeof(Buffer));	

	//等資料全部送出來
	if (m_BarcodeDeviceReadDataDelayTime > 0)
	{
		::Sleep(m_BarcodeDeviceReadDataDelayTime);
	}

	for (i = 0; i < 30; i++)
	{
		nReads = m_RS232COM.ReadData_Syn(Msg, length);
		if (nReads > 0)
		{
			break;
		}
		if (20 == i)
		{
			m_ErrorString.Format(_T("%s::read rs232 fault"), strDeviceName);
			return false;
		}
		Sleep(10);
	}

	if (ExtractBarcodeContent_Azurewave(Msg, Buffer, length) == false)	
	{
		m_ErrorString.Format(_T("%s::read content fault[%s]"), strDeviceName, Msg);
		return false;
	}
	::strcpy(Msg, Buffer);
	_strupr(Buffer);
	//if ( Buffer[0]=='N' && Buffer[1]=='R' && Buffer[2]==0x0D )//確認是否為No Read
	//{ 	
	//	::strcpy(Msg, "No Read");
	//	size_t lln  = ::strlen(Msg);
	//	Msg[lln] = m_SeparatorChar;
	//	Msg[lln+1] = '\0';
	//}

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
		//::sprintf(Buffer, "%s%s%s", GetResultBuffer(), m_SeparatorString, Msg);		
		::strcpy(Msg, Buffer);
	}
	CBarcode_Basic::SetResultBuffer(Msg);
	AnalysisResultBuffer();
	BackupResultBuffer();
	m_ErrorString = _T("");
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcode_Azurewave::ExtractBarcodeContent_Azurewave(const char *src, char *buffer, size_t bufferSize)//萃取條碼內容(去除Header、Termintory字串)
{	
	if ( NULL == src ) { return false; }
	//Filter profix, suffix
	const size_t len=::strlen(src);
	if ( 0 == len ) { return false; }	
	size_t pos1=0;
	size_t pos2=len-1;
	const char profix=0x02;//前綴
	const char suffix=0x03;//後綴
	if ( profix==src[pos1] ) { pos1++; }
	if ( suffix==src[pos2] ) { pos2--; }	
	const size_t len2=MIN(pos2-pos1+1, bufferSize-1);
	::memcpy(buffer, &src[pos1], sizeof(char)*len2);
	buffer[len2]='\0';
	//::strcpy(buffer, src);
	return true;
}
//-------------------------------------------------------------------------------------//
CBarcode_Azurewave::CBarcode_Azurewave()
{
	PreInitBarcodeDevice_Azurewave();
	InitialBarcodeDevice_Azurewave();
}
//-------------------------------------------------------------------------------------//
CBarcode_Azurewave::~CBarcode_Azurewave()
{

}
//-------------------------------------------------------------------------------------//
void CBarcode_Azurewave::SetCOMPort(unsigned int nPort)
{
	CString port;
	port.Format(_T("%d"), nPort);
	if (this->CheckConnected())
	{
		if (!this->Disconnected())
		{	
			this->SetBarcodeDevicePort(port);
			m_ErrorString.Format(_T("%s::RS232 Connected Fault [Port::%d]"), GetBarcodeDeviceFullName(), nPort);
		}
	}
	else
	{	this->SetBarcodeDevicePort(port);	}
	return ;
}
//-------------------------------------------------------------------------------------//
bool CBarcode_Azurewave::CheckConnected()//確認是否連線
{
	if (m_RS232COM.IsOpened() == false) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcode_Azurewave::ConnectToDevice()
{
	return ConnectDevice_Azurewave();
}
//-------------------------------------------------------------------------------------//
bool CBarcode_Azurewave::Disconnected()
{
	return Disconnect_Azurewave();
}
//-------------------------------------------------------------------------------------//
void CBarcode_Azurewave::ClearBuffer()
{
	char temp = ' ';
	if (CheckConnected() == false) { return; }
	while (this->m_RS232COM.ReadSingleChar(temp) != 0)
	{
	}
}
//-------------------------------------------------------------------------------------//
bool CBarcode_Azurewave::LoadBarcodeINIFile()
{
	if (CBarcode_Basic::LoadBarcodeINIFile() == false) { return false; }
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CBarcode_Azurewave::SaveBarcodeINIFile()
{
	if (CBarcode_Basic::SaveBarcodeINIFile() == false) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
CString CBarcode_Azurewave::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)//載入多國語系
{
	return CBarcode_Basic::LoadMultiLanguageString(KeyName, Default);
}
//-------------------------------------------------------------------------------------//
bool CBarcode_Azurewave::Initialize()
{
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CBarcode_Azurewave::StartToRead()
{
	if (StartToRead_Azurewave() == false)
	{	return false;	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcode_Azurewave::EndReading()
{
	const bool clrbuf=true;
	if (EndReading_Azurewave(clrbuf) == false)
	{	return false;	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcode_Azurewave::RetrieveCode()//接收裝置內的條碼
{
	if (RetrieveCode_Azurewave() == false)
	{	return false;	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcode_Azurewave::AnalysisResultBuffer()//分析結果字串
{	
	SetResultSubBuffer(0, GetResultBuffer());
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcode_Azurewave::CloneResultBuffer(char *Buffer, size_t BufferSize)//複製結果暫存區
{	
	const char  *ResultStr=GetResultBuffer();	
	const size_t ResultLen = ::strlen(ResultStr);	
	const size_t MinLen = MIN(ResultLen, BufferSize-1);
	::memcpy(Buffer, ResultStr, sizeof(char)*MinLen);
	Buffer[MinLen] = '\0';	
	return true;
}
//-------------------------------------------------------------------------------------//

