// Barcode_DataLogic_Matrix210NN.cpp: implementation of the CBarcode_DataLogic_Matrix210NN class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "jet8000.h"
#include "Barcode_DataLogic_Matrix210N.h"
#pragma warning (disable:4996)// _CRT_SECURE_NO_WARNINGS
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#define DATALOGIC_REPLAY_ESC              0x1B
#define DATALOGIC_REPLAY_CR               0x0D
#define DATALOGIC_REPLAY_LF               0x0A

//-------Protected Start----------//
CBarcode_DataLogic_Matrix210N::CBarcode_DataLogic_Matrix210N(const CBarcode_DataLogic_Matrix210N &device) :CBarcode_Basic(device)
{
	PreInitBarcodeDevice_Matrix210N();
	CloneBarcodeDevice_Matrix210N(device);
}
void CBarcode_DataLogic_Matrix210N::CloneBarcodeDevice_Matrix210N(const CBarcode_DataLogic_Matrix210N &device)
{
	m_ESCChar = device.m_ESCChar;
	::strcpy(m_HeaderString, device.m_HeaderString);
	::strcpy(m_TerminatorString, device.m_TerminatorString);
	::strcpy(m_DeviceNoRead, device.m_DeviceNoRead);
	::strcpy(m_TriggerOnString, device.m_TriggerOnString);
	::strcpy(m_TriggerOffString, device.m_TriggerOffString);
}
CBarcode_DataLogic_Matrix210N& CBarcode_DataLogic_Matrix210N::operator=(const CBarcode_DataLogic_Matrix210N &device)
{
	if (this == &device) { return *this; }
	CBarcode_Basic::operator=(device);
	CloneBarcodeDevice_Matrix210N(device);
	return *this;
}

#pragma region Initialize
void CBarcode_DataLogic_Matrix210N::PreInitBarcodeDevice_Matrix210N()
{
	BARCODE_DEVICE_TYPE DeviceType = BARCODE_DEVICE_DATALOGIC_MATRIX_210N;
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
void CBarcode_DataLogic_Matrix210N::InitialBarcodeDevice_Matrix210N()
{
	m_ESCChar = DATALOGIC_REPLAY_ESC;
	strcpy(m_SeparatorString, "+++");
	strcpy(m_HeaderString, "<S>");
	strcpy(m_TerminatorString, "<E>");
	strcpy(m_DeviceNoRead, "NOREAD");
	strcpy(m_TriggerOnString, "<ST>");
	strcpy(m_TriggerOffString, "<ET>");
}
#pragma endregion

#pragma region Connect/Disconnect RS232
bool CBarcode_DataLogic_Matrix210N::ConnectDevice_DataLogic_Matrix210N()//連線
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
	CString str = _T("");
	CString strDeviceName;

	//斷線
	Disconnect_DataLogic_Matrix210N();

	//載入參數檔案
	LoadBarcodeINIFile();
	SaveBarcodeINIFile();

	ClearResultBuffer();
	ClearResultBackup();
	m_ErrorString = _T("");
	strDeviceName = GetBarcodeDeviceFullName();
	for (i = 0; i < 7; i++)
	{
		if (this->m_RS232COM.IsOpened())
		{
			this->m_RS232COM.Close();
		}

		nBaud = Baud[i];
		if (m_RS232COM.Open(nPort, nBaud, nByteSize, SERIES_PARITY_NONE, 0) == false)
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

		if (SendData_DataLogic_Matrix210N("[A", NULL) == false)
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
bool CBarcode_DataLogic_Matrix210N::Disconnect_DataLogic_Matrix210N()//斷線
{
	if (this->m_RS232COM.IsOpened())
	{
		this->m_RS232COM.Close();
	}
	SetBarcodeDeviceConnected(false);
	return true;
}
#pragma endregion

#pragma region Loading Barcode Data
bool CBarcode_DataLogic_Matrix210N::StartToRead_DataLogic_Matrix210N()//開始讀取
{
	CBarcode_Basic::ClearResultBuffer();
	if (EndReading_DataLogic_Matrix210N() == false)
	{
		return false;
	}
	if (SendData_DataLogic_Matrix210N(m_TriggerOnString, NULL) == false)
	{
		return false;
	}
	SetBarcodeDeviceActived(true);
	return true;
}
bool CBarcode_DataLogic_Matrix210N::EndReading_DataLogic_Matrix210N()//關閉讀取
{
	ClearBuffer();
	CString str;
	bool IsOK = SendData_DataLogic_Matrix210N(m_TriggerOffString, NULL);
	SetBarcodeDeviceActived(false);
	//BarCode_CS::StopReading();	
	return IsOK;
}
bool CBarcode_DataLogic_Matrix210N::WaitForDataInQuene_DataLogic_Matrix210N()//等待有資料進來
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
bool CBarcode_DataLogic_Matrix210N::RetrieveCode_DataLogic_Matrix210N()//接收裝置內的條碼
{
	ClearResultBackup();
	CString strDeviceName = GetBarcodeDeviceFullName();
	const bool DeviceActived = GetBarcodeDeviceActived();
	if (false == DeviceActived)
	{
		m_ErrorString.Format(_T("%s::did not actived"), strDeviceName);
		return false;
	}

	if (WaitForDataInQuene_DataLogic_Matrix210N() == false)
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

	if (ExtractBarcodeContent_DataLogic_Matrix210N(Msg, Buffer, length) == false)	
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
		//::sprintf(Buffer, "%s%s", GetResultBuffer(), Msg);
		::sprintf(Buffer, "%s%s%s", GetResultBuffer(), m_SeparatorString, Msg);
		::strcpy(Msg, Buffer);
	}
	CBarcode_Basic::SetResultBuffer(Msg);
	AnalysisResultBuffer();
	BackupResultBuffer();
	m_ErrorString = _T("");
	return true;
}
bool CBarcode_DataLogic_Matrix210N::ExtractBarcodeContent_DataLogic_Matrix210N(const char *src, char *buffer, size_t bufferSize)//萃取條碼內容(去除Header、Termintory字串)
{	
	//return ExtractBarcodeContent_DataLogic_Matrix210N_I(src, buffer, bufferSize);
	//return ExtractBarcodeContent_DataLogic_Matrix210N_II(src, buffer, bufferSize);
	return ExtractBarcodeContent_DataLogic_Matrix210N_III(src, buffer, bufferSize);
}
bool CBarcode_DataLogic_Matrix210N::ExtractBarcodeContent_DataLogic_Matrix210N_I(const char *src, char *buffer, size_t bufferSize)//萃取條碼內容(去除Header、Termintory字串)
{
	if (NULL == src || NULL == buffer) { return false; }
	const size_t len = (::strlen(src));
	if (len >= bufferSize) { return false; }
	size_t idx_start = 0;
	size_t idx_end = 0;
	size_t i = 0, j = 0, k = 0, idx = 0;
	const size_t HeaderLen = ::strlen(m_HeaderString);
	const size_t TerminLen = ::strlen(m_TerminatorString);
	if (len <= HeaderLen || len <= TerminLen) { return false; }

#pragma region Find index(idx_start) of HeaderString
	idx_start = 0;
	for (i = 0; i < len - HeaderLen; i++)
	{
		k = 0;
		for (j = i; j < i + HeaderLen; j++)
		{
			if (src[j] != m_HeaderString[k++])
			{
				break;
			}
		}
		if (k != HeaderLen) { continue; }
		idx_start = i + HeaderLen;
		break;
	}
#pragma endregion

#pragma region Find index(idx_end) of TerminatorString
	idx_end = len;
	for (i = 0; i < len; i++)
	{
		k = 0;
		for (j = i; j < i + TerminLen; j++)
		{
			if (src[j] != m_TerminatorString[k++])
			{
				break;
			}
		}
		if (k != TerminLen) { continue; }
		else
		{
			idx_end = i;
		}
		
		if (k == TerminLen && idx_start < idx_end)
		{
			break;
		}
	}
#pragma endregion

	idx = 0;
	for (i = idx_start; i < idx_end; i++)
	{
		buffer[idx++] = src[i];
	}
	buffer[idx++] = '\0';
	return true;
}
bool CBarcode_DataLogic_Matrix210N::ExtractBarcodeContent_DataLogic_Matrix210N_II(const char *src, char *buffer, size_t bufferSize)//萃取條碼內容(去除Header、Termintory字串)
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

	idx = 0;
	for(idx_i = 0; idx_i < len; idx_i++)
	{
		#pragma region Find index(idx_start) of HeaderString
		idx_start = 0;
		for (i = idx_i; i < len - HeaderLen; i++)
		{
			k = 0;
			for (j = i; j < i + HeaderLen; j++)
			{
				if (src[j] != m_HeaderString[k++])
				{
					break;
				}
			}
			if (k != HeaderLen) { continue; }
			idx_start = i + HeaderLen;
			break;
		}
		#pragma endregion
		
		idx_i = idx_start;

		#pragma region Find index(idx_end) of TerminatorString
		idx_end = len;
		for (i = idx_i; i < len; i++)
		{
			k = 0;
			for (j = i; j < i + TerminLen; j++)
			{
				if (src[j] != m_TerminatorString[k++])
				{
					break;
				}
			}
			if (k != TerminLen) { continue; }
			else
			{
				idx_end = i;
			}
		
			if (k == TerminLen && idx_start < idx_end)
			{
				break;
			}
		}
		#pragma endregion

		idx_i = idx_end;
		
		for (i = idx_start; i < idx_end; i++)
		{
			buffer[idx++] = src[i];
		}

		if((len - idx_i) < (HeaderLen + TerminLen))	{ break; }
		for (i = 0; i < SeparatorLen; i++)
		{
			buffer[idx++] = m_SeparatorString[i];
		}
	}	
	buffer[idx++] = '\0';
	return true;
}
bool CBarcode_DataLogic_Matrix210N::ExtractBarcodeContent_DataLogic_Matrix210N_III(const char *src, char *buffer, size_t bufferSize)//萃取條碼內容(去除Header、Termintory字串以及CRLF)
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

	idx = 0;
	for (idx_i = 0; idx_i < len; idx_i++)
	{
		#pragma region Find index(idx_start) of HeaderString
		idx_start = 0;
		for (i = idx_i; i < len - HeaderLen; i++)
		{
			k = 0;
			for (j = i; j < i + HeaderLen; j++)
			{
				if (src[j] != m_HeaderString[k++])
				{
					break;
				}
			}
			if (k != HeaderLen) { continue; }
			idx_start = i + HeaderLen;
			break;
		}
#pragma endregion

		idx_i = idx_start;

		#pragma region Find index(idx_end) of TerminatorString
		idx_end = len;
		for (i = idx_i; i < len; i++)
		{
			k = 0;
			for (j = i; j < i + TerminLen; j++)
			{
				if (src[j] != m_TerminatorString[k++])
				{
					break;
				}
			}
			if (k != TerminLen) { continue; }
			else
			{
				idx_end = i;
			}

			if (k == TerminLen && idx_start < idx_end)
			{
				break;
			}
		}
#pragma endregion

		idx_i = idx_end;

		for (i = idx_start; i < idx_end; i++)
		{
			if (src[i] == '\r' || src[i] == '\n') {
				buffer[idx++] = '\0';		
			}
			else {
				buffer[idx++] = src[i];
			}
		}

		if ((len - idx_i) < (HeaderLen + TerminLen)) { break; }
		for (i = 0; i < SeparatorLen; i++)
		{
			buffer[idx++] = m_SeparatorString[i];
		}
	}
	buffer[idx++] = '\0';
	return true;
}
#pragma endregion

#pragma region Send Command/Set Param
bool CBarcode_DataLogic_Matrix210N::SetParam_DataLogic_Matrix210N(const char *buffer)//傳送資料
{
	if (SendData_DataLogic_Matrix210N(buffer, NULL) == false) { return false; }
	if (ExitSingleProgram() == false) { return false; }
	return true;
}
bool CBarcode_DataLogic_Matrix210N::SendData_DataLogic_Matrix210N(const char *buffer, const char* response)
{
	const size_t szBuffer = 128;
	char SendBuffer[szBuffer] = "";
	if (NULL == buffer) { return false; }
	::sprintf(SendBuffer, "%c%s", m_ESCChar, buffer);
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
		if (WaitForDataInQuene_DataLogic_Matrix210N() == false)
		{
			CString strDeviceName = GetBarcodeDeviceFullName();
			m_ErrorString.Format(_T("%s::wait for data too long"), strDeviceName);
			return false;
		}
		if (CheckResponse_DataLogic_Matrix210N(response) == false)
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
bool CBarcode_DataLogic_Matrix210N::CheckResponse_DataLogic_Matrix210N(const char *response)//確認回傳資料
{
	if (NULL == response) { return false; }
	//回傳碼為<ESC> ???? <CR><LF>
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
			if (TempChar != DATALOGIC_REPLAY_ESC) { continue; }
			Started = true;
		}
		Buffer[BuferIndex++] = TempChar;
		if (BuferIndex >= BufferSize) { break; }
		if (Started == true)
		{
			if (TempChar == DATALOGIC_REPLAY_LF) { break; }
		}
	};

	if (Started == false) { return false; }
	if (BuferIndex < 2) { return false; }

	if (Buffer[BuferIndex - 1] != DATALOGIC_REPLAY_LF) { return false; }
	if (Buffer[BuferIndex - 2] != DATALOGIC_REPLAY_CR) { return false; }

	int i = 0, j = 0;
	for (i = 1; i < BuferIndex - 2; i++)
	{
		Buffer2[j] = Buffer[i];
		j++;
	}
	Buffer2[j] = '\0';

	if (::strcmp(Buffer2, response) != 0)
	{
		return false;
	}
	return true;
}
bool CBarcode_DataLogic_Matrix210N::ChangeToProgramMode()//切換至參數設定模式
{
	char Buffer[128] = "";
	if (GetBarcodeDeviceConnected() == false) { return false; }
	::sprintf(Buffer, "%c%s", m_ESCChar, "[C");//Enter Host Mode
	SendData_DataLogic_Matrix210N(Buffer, "H");

	::sprintf(Buffer, "%c%s", m_ESCChar, "[B");//Enter Programming Mode
	SendData_DataLogic_Matrix210N(Buffer, "Q");
	return true;
}
bool CBarcode_DataLogic_Matrix210N::ExitProgramMode()//離開參數設定模式
{
	char Buffer[128] = "";
	if (GetBarcodeDeviceConnected() == false) { return false; }
	::sprintf(Buffer, "%c%s", m_ESCChar, "IA!");//Exit Programming Mode and Data Storag
	SendData_DataLogic_Matrix210N(Buffer, "K");

	::sprintf(Buffer, "%c%s", m_ESCChar, "[A");//Exit Host Mode
	SendData_DataLogic_Matrix210N(Buffer, "X");
	return true;
}
bool CBarcode_DataLogic_Matrix210N::ExitSingleProgram()//單一參數寫完
{
	char Buffer[128] = "";
	::sprintf(Buffer, "%c%s", m_ESCChar, "IA#");
	return SendData_DataLogic_Matrix210N(Buffer, "K");
}
#pragma endregion	
//-------Protected End----------//

//-------Public Start----------//
CBarcode_DataLogic_Matrix210N::CBarcode_DataLogic_Matrix210N()
{
	PreInitBarcodeDevice_Matrix210N();
	InitialBarcodeDevice_Matrix210N();
}
CBarcode_DataLogic_Matrix210N::~CBarcode_DataLogic_Matrix210N()
{

}

#pragma region Function of RS232
void CBarcode_DataLogic_Matrix210N::SetCOMPort(unsigned int nPort)
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
	{
		this->SetBarcodeDevicePort(port);
	}
}
bool CBarcode_DataLogic_Matrix210N::CheckConnected()//確認是否連線
{
	if (m_RS232COM.IsOpened() == false) { return false; }
	return true;
}
bool CBarcode_DataLogic_Matrix210N::ConnectToDevice()
{
	return ConnectDevice_DataLogic_Matrix210N();
}
bool CBarcode_DataLogic_Matrix210N::Disconnected()
{
	return Disconnect_DataLogic_Matrix210N();
}
void CBarcode_DataLogic_Matrix210N::ClearBuffer()
{
	char temp = ' ';
	if (CheckConnected() == false) { return; }
	while (this->m_RS232COM.ReadSingleChar(temp) != 0)
	{
	}
}
#pragma endregion

#pragma region Function of BarcodeReader
bool CBarcode_DataLogic_Matrix210N::LoadBarcodeINIFile()
{
	if (CBarcode_Basic::LoadBarcodeINIFile() == false) { return false; }

	const size_t textlen = 128;
	CString Filename;
	CString Section = _T("");
	CString KeyName = _T("");
	CString Default = _T("");
	TCHAR   String[textlen] = _T("");
	CString strDeviceName = GetBarcodeDeviceFullName();

	Filename = AOIDataCollect.GetSystemParamFilename();
	Section.Format(_T("%s"), strDeviceName);

	//分隔符號
	KeyName.Format(_T("Separator String"));
	Default = m_SeparatorString;
	if (JetAPI::LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true)
	{
		JetAPI::TCHAR2char(String, m_SeparatorString, sizeof(m_SeparatorString));
	}
	return true;
}
bool CBarcode_DataLogic_Matrix210N::SaveBarcodeINIFile()
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
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false)
	{	return false;	}
	return true;
}
CString CBarcode_DataLogic_Matrix210N::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)//載入多國語系
{
	return CBarcode_Basic::LoadMultiLanguageString(KeyName, Default);
}
bool CBarcode_DataLogic_Matrix210N::Initialize()
{
	bool IsOK = true;
	const size_t szBuffer = 128;
	char Buffer[szBuffer] = "";

	return true;
	//請使用條碼機軟體調教
	if (ChangeToProgramMode() == false) { return false; }
	//::sprintf(Buffer, "%c%s%s", m_ESCChar, "KF", m_DeviceNoRead);	//No Read message
	//IsOK = SetParam_DataLogic_Matrix210N(Buffer);
	ExitProgramMode();
	return true;
}
bool CBarcode_DataLogic_Matrix210N::StartToRead()
{
	if (StartToRead_DataLogic_Matrix210N() == false)
	{
		return false;
	}
	return true;
}
bool CBarcode_DataLogic_Matrix210N::EndReading()
{
	if (EndReading_DataLogic_Matrix210N() == false)
	{
		return false;
	}
	return true;
}
bool CBarcode_DataLogic_Matrix210N::RetrieveCode()//接收裝置內的條碼
{
	if (RetrieveCode_DataLogic_Matrix210N() == false)
	{
		return false;
	}
	return true;
}
bool CBarcode_DataLogic_Matrix210N::AnalysisResultBuffer()//分析結果字串
{
	int    SubCount = 0;
	bool   GetSubCode = false;
	size_t i = 0, j = 0, k = 0, idx = 0;
	const size_t BufferLen=1024;
	char SubBuffer[BufferLen+1] = "";
	char ContextBuffer[BufferLen+1] = "";
	const char tmpCR = 0x0D;	
	const char  *ResultStr=GetResultBuffer();	
	const size_t ResultLen = ::strlen(ResultStr);		
	const size_t SeparatorLen = ::strlen(m_SeparatorString);
	CString strDeviceName = GetBarcodeDeviceFullName();
	if ( ResultLen > BufferLen )
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
				if ( (ContextLen-i) > SeparatorLen )				
				{
					for (j = ContextLen - SeparatorLen; j < ContextLen; j++)
					{
						SubBuffer[idx++] = ContextBuffer[j];
					}
					i = j;
				}
				else
				{	i = ContextLen; }			
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

	/*
	SubCount=0;
	for ( i=0; i<ContextLen; i++ )
	{
	idx = 0;
	GetSubCode=false;
	::memset(SubBuffer, 0x00, sizeof(SubBuffer));
	if ( ContextLen > SeparatorLen )
	{
	for ( j=i; j<ContextLen-SeparatorLen; j++ )
	{
	s=0;
	for ( k=0; k<SeparatorLen; k++ )
	{
	if ( ContextBuffer[k+j] != m_SeparatorString[k] )
	{	break; }
	}
	if ( k < SeparatorLen )
	{	SubBuffer[idx++] = ContextBuffer[j];	}
	else
	{
	GetSubCode = true;
	i = j+SeparatorLen-1;//next loop begin will add one value
	break;
	}
	}
	if ( false == GetSubCode )
	{
	for ( j=ContextLen-SeparatorLen; j<ContextLen; j++ )
	{	SubBuffer[idx++] = ContextBuffer[j]; }
	i = j;
	}
	}
	else
	{
	for ( j=i; j<ContextLen; j++ )
	{	SubBuffer[idx++] = ContextBuffer[j];	}
	i = j;
	}
	SubBuffer[idx++] = '\0';
	SetResultSubBuffer(SubCount, SubBuffer);
	SubCount ++;
	}
	*/
	return true;
}
bool CBarcode_DataLogic_Matrix210N::CloneResultBuffer(char *Buffer, size_t BufferSize)//複製結果暫存區
{
	size_t i = 0, j = 0, k = 0;
	const char ShowSeparatorChar = '+';
	const size_t BufferLen=1024;
	char ContextBuffer[BufferLen+1] = "";
	const char  *ResultStr=GetResultBuffer();	
	const size_t ResultLen = ::strlen(ResultStr);		
	const size_t CopyLen = MIN(ResultLen, BufferLen);
	::memcpy(ContextBuffer, ResultStr, sizeof(char)*(CopyLen+1));
	const size_t ContextLen = ::strlen(ContextBuffer);
	//將內部分隔符號轉成顯示的分隔符號
	for (i = 0; i < ContextLen; i++)
	{
		//if ( ContextBuffer[i] == m_SeparatorChar )
		//{	ContextBuffer[i] = ShowSeparatorChar; }
	}

	if (ShowSeparatorChar == ContextBuffer[ContextLen - 1])
	{	ContextBuffer[ContextLen-1] = '\0';	}

	const size_t MinLen = MIN(ContextLen, BufferSize - 1);
	::memcpy(Buffer, ContextBuffer, sizeof(char)*MinLen);
	Buffer[MinLen] = '\0';
	return true;
}
#pragma endregion
//-------Public End----------//

