#include "stdafx.h"
#include "Barcode_Keyence_SR751.h"

CBarcode_Keyence_SR751::CBarcode_Keyence_SR751()
{
	PreInitBarcodeDevice_Keyence_SR751();
	InitialBarcodeDevice_Keyence_SR751();
}
CBarcode_Keyence_SR751::~CBarcode_Keyence_SR751()
{
}

//-------Protected Start----------//
CBarcode_Keyence_SR751::CBarcode_Keyence_SR751(const CBarcode_Keyence_SR751 &device) :CBarcode_Basic(device)
{
	PreInitBarcodeDevice_Keyence_SR751();
	CloneBarcodeDevice_Keyence_SR751(device);
}
void CBarcode_Keyence_SR751::CloneBarcodeDevice_Keyence_SR751(const CBarcode_Keyence_SR751 &device)
{
	::strcpy(m_HeaderString, device.m_HeaderString);
	::strcpy(m_TerminatorString, device.m_TerminatorString);
	::strcpy(m_DeviceNoRead, device.m_DeviceNoRead);
	::strcpy(m_TriggerOnString, device.m_TriggerOnString);
	::strcpy(m_TriggerOffString, device.m_TriggerOffString);
}
CBarcode_Keyence_SR751& CBarcode_Keyence_SR751::operator=(const CBarcode_Keyence_SR751 &device)
{
	if (this == &device) { return *this; }
	CBarcode_Basic::operator=(device);
	CloneBarcodeDevice_Keyence_SR751(device);
	return *this;
}

//-------Initialize----------//
void CBarcode_Keyence_SR751::PreInitBarcodeDevice_Keyence_SR751()
{
	BARCODE_DEVICE_TYPE DeviceType = BARCODE_DEVICE_KEYENCE_SR751;
	CString ModelName = AOIDataDefine.GetBarcodeDeviceName(DeviceType);
	SetBarcodeDeviceType(DeviceType);//條碼機樣式
	SetBarcodeDeviceModel(ModelName);//條碼機型號		

	::memset(m_SeparatorString, 0x00, sizeof(m_SeparatorString));
	::memset(m_HeaderString, 0x00, sizeof(m_HeaderString));
	::memset(m_TerminatorString, 0x00, sizeof(m_TerminatorString));
	::memset(m_TriggerOnString, 0x00, sizeof(m_TriggerOnString));
	::memset(m_TriggerOffString, 0x00, sizeof(m_TriggerOffString));
	::memset(m_DeviceNoRead, 0x00, sizeof(m_DeviceNoRead));
}
void CBarcode_Keyence_SR751::InitialBarcodeDevice_Keyence_SR751()
{
	m_ESCChar = ESC;
	strcpy(m_SeparatorString, ",");
	m_HeaderString[0] = STX;
	m_TerminatorString[0] = ETX;
	strcpy(m_DeviceNoRead, "ERROR");
	strcpy(m_TriggerOnString, "LON");//LON
	strcpy(m_TriggerOffString, "LOFF");//LOFF
}

//-------Connect----------//
bool CBarcode_Keyence_SR751::ConnectDevice_Keyence_SR751()
{
#pragma region keyence_SR751 出貨前設定
	int nBaud = 115200;			// 9600, 19200, 38400, 57600 or 115200	
	const int nByteSize = 8;				// 7 or 8
	const int nParity = EVENPARITY;			// NOPARITY(0), ODDPRITY(1), or EVENPARITY(2)
	const int nStopBits = ONESTOPBIT;		// ONESTOPBIT(1) or TWOSTOPBITS(2)
#pragma endregion
	CString   strPort = GetBarcodeDevicePort();
	const int nPort = JetAPI::StrToInt(strPort);

	int i = 0;
	bool IsConnected = true;
	//bool IsConnected = false;
	CString str = _T("");
	CString strDeviceName;

	//斷線
	Disconnect_Keyence_SR751();

	//載入參數檔案
	LoadBarcodeINIFile();
	SaveBarcodeINIFile();

	ClearResultBuffer();
	ClearResultBackup();
	m_ErrorString = _T("");
	strDeviceName = GetBarcodeDeviceFullName();
	for (i = 0; i < BAUDRATE_COUNT; i++)
	{
		if (this->m_RS232COM.IsOpened())
		{
			this->m_RS232COM.Close();
		}

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
		if (IsConnected == false)
		{
			if (this->m_RS232COM.IsOpened())
			{
				this->m_RS232COM.Close();
			}

			m_ErrorString.Format(_T("%s::RS232 Connected Fault [Port::%d]"), strDeviceName, nPort);
			continue;
		}
		if (SendData_Keyence_SR751("BCLR", "OK", 2) == false)//
		{
			IsConnected = false;
			SetBarcodeDeviceConnected(IsConnected);
			continue;
		}
		if (SendData_Keyence_SR751("WP,101,0", "OK,WP", 5) == false)//電平:0; 單觸發:1
		{
			IsConnected = false;
			SetBarcodeDeviceConnected(IsConnected);
			continue;
		}
		//if (SendData_Keyence_SR751("WP,102,2550", "OK,WP", 5) == false)//單觸發同步化之量測時間(單位:10ms)
		//{
		//	IsConnected = false;
		//	SetBarcodeDeviceConnected(IsConnected);
		//	continue;
		//}
		if (SendData_Keyence_SR751("WP,200,1", "OK,WP", 5) == false)//讀取模式, 單個:0、多個:1、突發:2、腳本:3
		{
			IsConnected = false;
			SetBarcodeDeviceConnected(IsConnected);
			continue;
		}
		if (SendData_Keyence_SR751("WP,202,255", "OK,WP", 5) == false)//多重讀取防止重複條碼(單位:100ms)
		{
			IsConnected = false;
			SetBarcodeDeviceConnected(IsConnected);
			continue;
		}
		if (SendData_Keyence_SR751("WP,213,1", "OK,WP", 5) == false)//多重讀取防止重複條碼(單位:100ms)
		{
			IsConnected = false;
			SetBarcodeDeviceConnected(IsConnected);
			continue;
		}
		break;
	}
	if (IsConnected == true)
	{
		m_ErrorString = _T("");
	}
	else
	{
		this->m_RS232COM.Close();
	}
	SetBarcodeDeviceConnected(IsConnected);
	return IsConnected;
}
bool CBarcode_Keyence_SR751::Disconnect_Keyence_SR751()
{
	if (this->m_RS232COM.IsOpened())
	{
		this->m_RS232COM.Close();
	}
	SetBarcodeDeviceConnected(false);
	return true;
}

bool CBarcode_Keyence_SR751::StartToRead_Keyence_SR751()
{
	CBarcode_Basic::ClearResultBuffer();
	if (EndReading_Keyence_SR751() == false)
	{
		return false;
	}
	if (SendData_Keyence_SR751(m_TriggerOnString, NULL) == false)
	{
		return false;
	}
	SetBarcodeDeviceActived(true);
	return true;
}
bool CBarcode_Keyence_SR751::EndReading_Keyence_SR751()
{
	ClearBuffer();
	CString str;
	bool IsOK = SendData_Keyence_SR751(m_TriggerOffString, NULL);
	SetBarcodeDeviceActived(false);
	return IsOK;
}
bool CBarcode_Keyence_SR751::WaitForDataInQuene_Keyence_SR751()
{
	int WaitCount = 0;
	const int MaxWaitCounts = m_BarcodeDeviceWaitDataCount;
	while (m_RS232COM.ReadDataWaiting() == 0 && WaitCount++ < MaxWaitCounts)
	{
		Sleep(m_BarcodeDeviceWaitDataDwellTime);
	};
	if (WaitCount > MaxWaitCounts) {
		return false;
	}
	else {
		return true;
	}
}
bool CBarcode_Keyence_SR751::RetrieveCode_Keyence_SR751()
{
	ClearResultBackup();
	CString strDeviceName = GetBarcodeDeviceFullName();
	const bool DeviceActived = GetBarcodeDeviceActived();
	if (false == DeviceActived)
	{
		m_ErrorString.Format(_T("%s::did not actived"), strDeviceName);
		return false;
	}

	if (WaitForDataInQuene_Keyence_SR751() == false)
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

	if (ExtractBarcodeContent_Keyence_SR751(Msg, Buffer, length) == false)
	{
		m_ErrorString.Format(_T("%s::read content fault[%s]"), strDeviceName, Msg);
		return false;
	}
	::strcpy(Msg, Buffer);
	_strupr(Buffer);

	const size_t BeforBufferLen = ::strlen(GetResultBuffer());
	if (BeforBufferLen > 0) //確認是否有讀到之前的
	{
		::sprintf(Buffer, "%s%s%s", GetResultBuffer(), m_SeparatorString, Msg);
		::strcpy(Msg, Buffer);
	}
	CBarcode_Basic::SetResultBuffer(Msg);
	AnalysisResultBuffer();
	BackupResultBuffer();
	m_ErrorString = _T("");
	return true;
}
bool CBarcode_Keyence_SR751::ExtractBarcodeContent_Keyence_SR751(const char *src, char *buffer, size_t bufferSize)//萃取條碼內容(去除Header、Termintory字串)
{
	if (NULL == src || NULL == buffer) { return false; }
	const size_t len = (::strlen(src));
	if (len >= bufferSize) { return false; }
	size_t idx_start = 0;
	size_t idx_end = 0;
	size_t i = 0, j = 0, k = 0, idx = 0, idx_i = 0;
	const size_t HeaderLen = ::strlen(m_HeaderString);
	const size_t TerminLen = ::strlen(m_TerminatorString);
	const size_t SeparatorLen = ::strlen(m_SeparatorString);
	if (len <= HeaderLen || len <= TerminLen) { return false; }
	std::string s = src;
	idx_start = s.find_last_of(m_TerminatorString) + TerminLen;
	idx_end = s.find_last_of("\r");
	if (idx_end == -1) {
		return false;
	}

	idx = 0;
	for (i = idx_start; i < idx_end; i++)
	{
		buffer[idx++] = src[i] == '\r' ? '\0' : src[i];
	}
	//for (i = 0; i < SeparatorLen; i++)
	//{
	//	buffer[idx++] = m_SeparatorString[i];
	//}	
	buffer[idx++] = '\0';
	return true;
}

bool CBarcode_Keyence_SR751::SendData_Keyence_SR751(const char *buffer, const char* response, int length)
{
	const size_t szBuffer = 128;
	char SendBuffer[szBuffer] = "";
	if (NULL == buffer) { return false; }
	::sprintf(SendBuffer, "%s%s%s", m_HeaderString, buffer, m_TerminatorString);
	const int len = (int)(::strlen(SendBuffer));
	if (GetBarcodeDeviceConnected() == false)
	{
		CString strDeviceName = GetBarcodeDeviceFullName();
		m_ErrorString.Format(_T("%s::does not connect"), strDeviceName);
		return false;
	}

	if (m_RS232COM.SendData(SendBuffer, len) == 0)
	{
		CString str(buffer);
		CString strDeviceName = GetBarcodeDeviceFullName();
		m_ErrorString.Format(_T("%s::send data fault[%s]"), strDeviceName, str);
		return false;
	}

	if (m_BarcodeDeviceCommDelayTime > 0)
	{
		::Sleep(m_BarcodeDeviceCommDelayTime);
	}

	if (NULL != response)
	{
		if (WaitForDataInQuene_Keyence_SR751() == false)
		{
			CString strDeviceName = GetBarcodeDeviceFullName();
			m_ErrorString.Format(_T("%s::wait for data too long"), strDeviceName);
			return false;
		}
		if (CheckResponse_Keyence_SR751(response, length) == false)
		{
			CString strDeviceName = GetBarcodeDeviceFullName();
			m_ErrorString.Format(_T("%s::read reply data fault[In:%s, Out:%s]"), strDeviceName, buffer, response);
			return false;
		}
		if (m_BarcodeDeviceCommDelayTime > 0)
		{
			::Sleep(m_BarcodeDeviceCommDelayTime);
		}
	}
	return true;
}
bool CBarcode_Keyence_SR751::CheckResponse_Keyence_SR751(const char *response, int length)//確認回傳資料
{
	if (NULL == response) { return false; }
	char  TempChar = 0x00;
	bool  Started = false;
	int   ContinueCount = 0;
	int   BuferIndex = 0;
	const int BufferSize = 1024;
	char Buffer[BufferSize] = "";
	char Buffer2[BufferSize] = "";

	Started = false;
	BuferIndex = 0;
	while (true)
	{
		ContinueCount++;
		if (ContinueCount >= BufferSize) { break; }

		this->m_RS232COM.ReadSingleChar(TempChar);
		if (TempChar == 0x00) { break; }
		if (Started == false)
		{
			if (TempChar != STX) { continue; }
			Started = true;
		}
		Buffer[BuferIndex++] = TempChar;
		if (BuferIndex >= BufferSize) { break; }
		if (Started == true)
		{
			if (TempChar == ETX) { break; }
		}
	};

	if (Started == false) { return false; }
	if (BuferIndex < 2) { return false; }

	int i = 0, j = 0;
	for (i = 1; i < BuferIndex - 1; i++)
	{
		Buffer2[j] = Buffer[i];
		j++;
	}
	Buffer2[j] = '\0';

	if (::strncmp(Buffer2, response, length) != 0)
	{
		return false;
	}
	return true;
}

bool CBarcode_Keyence_SR751::CheckConnected()//確認是否連線
{
	if (m_RS232COM.IsOpened() == false) { return false; }
	return true;
}
bool CBarcode_Keyence_SR751::ConnectToDevice()
{
	return ConnectDevice_Keyence_SR751();
}
bool CBarcode_Keyence_SR751::Disconnected()
{
	return Disconnect_Keyence_SR751();
}
void CBarcode_Keyence_SR751::ClearBuffer()
{
	char temp = ' ';
	if (CheckConnected() == false) { return; }
	while (this->m_RS232COM.ReadSingleChar(temp) != 0)
	{
	}
}

bool CBarcode_Keyence_SR751::LoadBarcodeINIFile()
{
	if (CBarcode_Basic::LoadBarcodeINIFile() == false) { return false; }

	const size_t textlen = 128;
	CString Filename;
	CString Section = _T("");
	CString KeyName = _T("");
	CString Default = _T("");
	TCHAR   String[textlen] = _T("");
	CString strDeviceName = GetBarcodeDeviceFullName();

	//Filename = AOIDataCollect.GetSystemParamFilename();
	Section.Format(_T("%s"), strDeviceName);

	//分隔符號
	KeyName.Format(_T("Separator String"));
	Default = m_SeparatorString;
	if (LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true)
	{	JetAPI::TCHAR2char(String, m_SeparatorString, sizeof(m_SeparatorString));	}
	return true;
}
bool CBarcode_Keyence_SR751::SaveBarcodeINIFile()
{
	if (CBarcode_Basic::SaveBarcodeINIFile() == false) { return false; }

	CString Filename;
	CString Section = _T("");
	CString KeyName = _T("");
	CString String = _T("");
	CString strDeviceName = GetBarcodeDeviceFullName();

	Filename = AOIDataCollect.GetSystemParamFilename();
	Section.Format(_T("%s"), strDeviceName);

	//分隔符號
	KeyName.Format(_T("Separator String"));
	String = m_SeparatorString;
	if (SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false)
	{	return false;	}
	return true;
}
CString CBarcode_Keyence_SR751::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)//載入多國語系
{
	return CBarcode_Basic::LoadMultiLanguageString(KeyName, Default);
}
bool CBarcode_Keyence_SR751::Initialize()
{
	InitialBarcodeDevice_Keyence_SR751();
	return true;
}
bool CBarcode_Keyence_SR751::StartToRead()
{
	if (StartToRead_Keyence_SR751() == false)
	{
		return false;
	}
	return true;
}
bool CBarcode_Keyence_SR751::EndReading()
{
	if (EndReading_Keyence_SR751() == false)
	{
		return false;
	}
	return true;
}
bool CBarcode_Keyence_SR751::RetrieveCode()//接收裝置內的條碼
{
	if (RetrieveCode_Keyence_SR751() == false)
	{
		return false;
	}
	return true;
}
bool CBarcode_Keyence_SR751::AnalysisResultBuffer()//分析結果字串
{
	int    SubCount = 0;
	bool   GetSubCode = false;
	size_t i = 0, j = 0, k = 0, idx = 0;
	const size_t BufferLen = 1024;
	char SubBuffer[BufferLen + 1] = "";
	char ContextBuffer[BufferLen + 1] = "";
	const char tmpCR = 0x0D;
	const char  *ResultStr = GetResultBuffer();
	const size_t ResultLen = ::strlen(ResultStr);
	const size_t SeparatorLen = ::strlen(m_SeparatorString);
	CString strDeviceName = GetBarcodeDeviceFullName();
	if (ResultLen > BufferLen)
	{
		m_ErrorString.Format(_T("%s::Barcode Len is bigger than buffer size [%d/%d]"), strDeviceName, ResultLen, BufferLen);
		return false;
	}
	SubCount = 0;
	strcpy(ContextBuffer, GetResultBuffer());
	const size_t ContextLen = ::strlen(ContextBuffer);
	if (ContextLen > SeparatorLen)
	{
		for (i = 0; i < ContextLen; i++)
		{
			idx = 0;
			GetSubCode = false;
			::memset(SubBuffer, 0x00, sizeof(SubBuffer));
			for (j = i; j < ContextLen - SeparatorLen; j++)
			{
				for (k = 0; k < SeparatorLen; k++)
				{
					if (ContextBuffer[k + j] != m_SeparatorString[k])
					{
						break;
					}
				}
				if (k < SeparatorLen)
				{
					SubBuffer[idx++] = ContextBuffer[j];
				}
				else
				{
					GetSubCode = true;
					i = j + SeparatorLen - 1;//next loop begin will add one value
					break;
				}
			}
			if (false == GetSubCode)
			{
				if ((ContextLen - i) > SeparatorLen)
				{
					for (j = ContextLen - SeparatorLen; j < ContextLen; j++)
					{
						SubBuffer[idx++] = ContextBuffer[j];
					}
					i = j;
				}
				else
				{
					i = ContextLen;
				}
			}
			SubBuffer[idx++] = '\0';
			SetResultSubBuffer(SubCount, SubBuffer);
			SubCount++;
		}
	}
	else
	{
		for (j = i; j < ContextLen; j++)
		{
			SubBuffer[idx++] = ContextBuffer[j];
		}
		SubBuffer[idx++] = '\0';
		SetResultSubBuffer(SubCount, SubBuffer);
		SubCount++;
	}
	return true;
}
bool CBarcode_Keyence_SR751::CloneResultBuffer(char *Buffer, size_t BufferSize)//複製結果暫存區
{
	size_t i = 0, j = 0, k = 0;
	const char ShowSeparatorChar = '+';
	const size_t BufferLen = 1024;
	char ContextBuffer[BufferLen + 1] = "";
	const char  *ResultStr = GetResultBuffer();
	const size_t ResultLen = ::strlen(ResultStr);
	const size_t CopyLen = MIN(ResultLen, BufferLen);
	::memcpy(ContextBuffer, ResultStr, sizeof(char)*(CopyLen + 1));
	const size_t ContextLen = ::strlen(ContextBuffer);
	//將內部分隔符號轉成顯示的分隔符號
	for (i = 0; i < ContextLen; i++)
	{
		//if ( ContextBuffer[i] == m_SeparatorChar )
		//{	ContextBuffer[i] = ShowSeparatorChar; }
	}

	if (ShowSeparatorChar == ContextBuffer[ContextLen - 1])
	{
		ContextBuffer[ContextLen - 1] = '\0';
	}

	const size_t MinLen = MIN(ContextLen, BufferSize - 1);
	::memcpy(Buffer, ContextBuffer, sizeof(char)*MinLen);
	Buffer[MinLen] = '\0';
	return true;
}