// Barcode_General.cpp: implementation of the CBarcode_General class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
//#include "jet8000.h"
#include "Barcode_General.h"
//-------------------------------------------------------------------------------------//
#pragma warning (disable:4996)// _CRT_SECURE_NO_WARNINGS
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
CBarcode_General::CBarcode_General(const CBarcode_General &device) :CBarcode_Basic(device)
{
	PreInitBarcodeDevice_General();
	CloneBarcodeDevice_General(device);
}
//-------------------------------------------------------------------------------------//
void CBarcode_General::CloneBarcodeDevice_General(const CBarcode_General &device)
{
	return;
}
//-------------------------------------------------------------------------------------//
CBarcode_General& CBarcode_General::operator=(const CBarcode_General &device)
{
	if (this == &device) { return *this; }
	CBarcode_Basic::operator=(device);
	CloneBarcodeDevice_General(device);
	return *this;
}
//-------------------------------------------------------------------------------------//
void CBarcode_General::PreInitBarcodeDevice_General()
{
	CString DeviceName;
	BARCODE_DEVICE_TYPE DeviceType = BARCODE_DEVICE_GENERAL_GROUP;
	CString ModelName = AOIDataDefine.GetBarcodeDeviceName(DeviceType);
	DeviceName.Format(_T("%s_%02d"), ModelName, m_DeviceID);	
	SetBarcodeDeviceType(DeviceType);//條碼機樣式
	SetBarcodeDeviceModel(DeviceName);//條碼機型號
	return;
}
//-------------------------------------------------------------------------------------//
void CBarcode_General::InitialBarcodeDevice_General()
{	
	m_HexTriggerOn = "<ST>";
	m_HexTriggerOff = "<ET>";
	LoadBarcodeINIFile();
	return;
}
//-------------------------------------------------------------------------------------//
bool CBarcode_General::ConnectDevice_General()//連線
{	
	CString   strPort = GetBarcodeDevicePort();

	const int nByteSize = 8;	
	const int nPort = JetAPI::StrToInt(strPort);	
	const int nBaud = GetBarcodeDeviceBaudRate();
	const int nParity = GetBarcodeDeviceParity();//SERIES_PARITY_NONE, SERIES_PARITY_ODD, SERIES_PARITY_EVEN, SERIES_PARITY_MASK, SERIES_PARITY_SPACE
	const int nStopBits = GetBarcodeDeviceStopBits();//SERIES_STOPBITS_10, SERIES_STOPBITS_15, SERIES_STOPBITS_20	
	//CBR_110, CBR_300, CBR_600, CBR_1200, CBR_2400, CBR_4800, CBR_9600, CBR_14400, CBR_19200, CBR_38400, CBR_56000, CBR_57600, CBR_115200, CBR_128000, CBR_256000
	//const int Baud[]={CBR_9600};
	//const int MaxBaud=sizeof(Baud)/sizeof(Baud[0]);		

	int i = 0;
	bool IsConnected = true;
	//bool IsConnected = false;
	CString str = _T("");
	CString strDeviceName;

	//斷線
	Disconnect_General();

	//載入參數檔案
	LoadBarcodeINIFile();
	SaveBarcodeINIFile();

	ClearResultBuffer();
	ClearResultBackup();
	m_ErrorString = _T("");
	strDeviceName = GetBarcodeDeviceFullName();
	
	if (this->m_RS232COM.IsOpened())
	{	this->m_RS232COM.Close();	}
	
	if (m_RS232COM.Open(nPort, nBaud, nByteSize, nParity, nStopBits) == false)
	{
		IsConnected = false;
		m_ErrorString.Format(_T("%s::RS232 Connected Fault [Port::%d]"), strDeviceName, nPort);
		return false;
	}

	if (!m_RS232COM.IsOpened())
	{
		IsConnected = false;
		m_ErrorString.Format(_T("%s::RS232 Connected Fault [Port::%d]"), strDeviceName, nPort);
		return false;
	}

	//讀取條碼機回傳值
	IsConnected = true;
	SetBarcodeDeviceConnected(IsConnected);
	this->ClearBuffer();		
	
	if (IsConnected == true)
	{	m_ErrorString = _T("");	}
	else
	{	this->m_RS232COM.Close();	}
	SetBarcodeDeviceConnected(IsConnected);
	return IsConnected;
}
//-------------------------------------------------------------------------------------//
bool CBarcode_General::Disconnect_General()//斷線
{
	if (this->m_RS232COM.IsOpened())
	{	this->m_RS232COM.Close();	}
	SetBarcodeDeviceConnected(false);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcode_General::StartToRead_General()//開始讀取
{
	const bool clrbuf=true;
	CBarcode_Basic::ClearResultBuffer();
	if (EndReading_General(clrbuf) == false)
	{	return false;	}	
	if (SendData_General(m_HexTriggerOn.c_str(), NULL) == false)
	{	return false;	}	
	SetBarcodeDeviceActived(true);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcode_General::EndReading_General(bool clrbuf)//關閉讀取
{
	if ( true == clrbuf )
	{	ClearBuffer();	}
	bool IsOK = SendData_General(m_HexTriggerOff.c_str(), NULL);
	SetBarcodeDeviceActived(false);	
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CBarcode_General::WaitForDataInQuene_General()//等待有資料進來
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
bool CBarcode_General::RetrieveCode_General()//接收裝置內的條碼
{
	ClearResultBackup();
	CString strDeviceName = GetBarcodeDeviceFullName();
	const bool DeviceActived = GetBarcodeDeviceActived();
	if (false == DeviceActived)
	{
		m_ErrorString.Format(_T("%s::did not actived"), strDeviceName);
		return false;
	}

	if (WaitForDataInQuene_General() == false)
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

	if (ExtractBarcodeContent_General(Msg, Buffer, length) == false)	
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
bool CBarcode_General::ExtractBarcodeContent_General(const char *src, char *buffer, size_t bufferSize)//萃取條碼內容(去除Header、Termintory字串)
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
bool CBarcode_General::SendData_General(const char *buffer, const char* response)//傳送資料
{	
	if ( GetBarcodeDeviceConnected() == false )
	{
		CString strDeviceName = GetBarcodeDeviceFullName();
		m_ErrorString.Format(_T("%s::does not connect"), strDeviceName);
		return false;
	}

	size_t bufCnt=0;	
	char buf[32]="";
	if ( ConvertHexToString(buffer, buf, sizeof(buf), bufCnt) == false )
	{	return false; }

	int len = (int)(bufCnt);
	char *ptr=(char*)(buf);
	
	const int SendLen=m_RS232COM.SendData(ptr, len);
	if ( SendLen == 0 )	
	{
		CString str(buffer);
		CString strDeviceName = GetBarcodeDeviceFullName();
		m_ErrorString.Format(_T("%s::send data fault[%s]"), strDeviceName, str);		
		return false;
	}	

	if ( m_BarcodeDeviceCommDelayTime > 0 ) 
	{	::Sleep(m_BarcodeDeviceCommDelayTime);	}

	if ( NULL != response )
	{
		if ( WaitForDataInQuene_General() == false )
		{
			CString strDeviceName = GetBarcodeDeviceFullName();
			m_ErrorString.Format(_T("%s::wait for data too long"), strDeviceName);
			return false;
		}
		if ( CheckResponse_General(response) == false )
		{	
			CString strDeviceName = GetBarcodeDeviceFullName();
			m_ErrorString.Format(_T("%s::read reply data fault[In:%s, Out:%s]"), strDeviceName, buffer, response);
			return false; 
		}
		if ( m_BarcodeDeviceCommDelayTime > 0 ) 
		{	::Sleep(m_BarcodeDeviceCommDelayTime);	}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcode_General::ConvertHexToString(const char *buffer, char Buf[], size_t BufSize, size_t &Count)//建立傳送字串
{
	const size_t len=::strlen(buffer);
	const size_t len2=(len/2)*2;
	if ( len != len2 )
	{
		CString str=buffer;
		m_ErrorString.Format(_T("Error, Barcode send data string Fault(%s)"), str);
		return false;
	}
	if ( len2 > BufSize )
	{
		CString str=buffer;
		m_ErrorString.Format(_T("Error, Barcode send data string size out of limit(%s)"), str);
		return false;
	}

	size_t idx=0;
	char Tmp[32]="";	
	unsigned char ch=0;
	::memset(Tmp, 0x00, sizeof(Tmp));
	for ( size_t i=0; i<len2; i+=2 )
	{
		Tmp[0] = buffer[i];
		Tmp[1] = buffer[i+1];
		Tmp[2] = '\0';
		HexStringToByte(Tmp, ch);
		Buf[idx] = (char)(ch);
		idx ++;
	}
	Count = idx;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcode_General::CheckResponse_General(const char *response)//確認回傳資料
{	
	if ( NULL == response ) { return false; }
	//回傳碼為<ESC> ???? <CR><LF>
	char  TempChar = 0x00;	
	bool  Started = false;
	int   ContinueCount = 0;
	int   BuferIndex = 0;
	const int BufferSize = 1024;
	char Buffer[BufferSize]="";	
	char Buffer2[BufferSize]="";		

	Started = false;
	BuferIndex = 0;
	while ( true)
	{
		ContinueCount ++;
		if ( ContinueCount >= BufferSize ) { break; }

		this->m_RS232COM.ReadSingleChar(TempChar);	
		if ( TempChar == 0x00 ) { break; }
		if ( Started == false )
		{
			if ( TempChar != ASCII_ESC ) { continue; }
			Started = true;
		}		
		Buffer[BuferIndex++] = TempChar;		
		if ( BuferIndex >= BufferSize ) { break; }		
		if ( Started == true ) 
		{
			if ( TempChar == ASCII_LF ) { break; }
		}
	};

	if ( Started == false ) { return false; }
	if ( BuferIndex < 2 ) { return false; }

	if ( Buffer[BuferIndex-1] != ASCII_LF ) { return false; }
	if ( Buffer[BuferIndex-2] != ASCII_CR ) { return false; }	
	
	int i=0, j=0;
	for ( i=1; i<BuferIndex-2; i++ )
	{
		Buffer2[j] = Buffer[i];
		j++;
	}
	Buffer2[j] = '\0';	

	if ( ::strcmp(Buffer2, response) != 0 ) 
	{	return false; }	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcode_General::HexStringToByte(const char buf[], unsigned char &val)//16進位文字轉成位元祖
{	
	int tmp=0;
	::sscanf_s(buf, "%02X", &tmp);	
	val = (unsigned char)(tmp);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcode_General::ByteToHexString(unsigned char val, char buf[], size_t size)//位元祖轉成16進位的文字	
{
	const int tmp=(int)(val);
	::sprintf_s(buf, size, "%02X", tmp);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcode_General::ExtractSubCode(const char *Barcode, const char *Separator, std::vector<std::string> &CodeList)
{
	CodeList.clear();
	if ( NULL == Barcode ) { return false; }
	const size_t SepLen=::strlen(Separator);
	const size_t BarcodeLen=::strlen(Barcode);
	if ( NULL==Separator || 0==SepLen )
	{
		CodeList.push_back(std::string(Barcode));
		return true;
	}

	bool bMatch=true;
	size_t PosStart=0, PosEnd=0;
	for ( size_t i=0; i<BarcodeLen-SepLen+1; i++ )
	{		
		bMatch=true;
		for ( size_t j=0; j<SepLen; j++ )
		{	
			if ( Barcode[i+j]!=Separator[j] )
			{	
				bMatch=false;
				break; 
			}
		}
		if ( bMatch == false )
		{	continue; }
		PosEnd = i;		
		if ( PosEnd < PosStart )
		{	return false; }
		std::string Code;
		const size_t Len=PosEnd-PosStart;
		Code.assign(&Barcode[PosStart], Len);
		CodeList.push_back(Code);
		PosStart = PosEnd+SepLen;		
		i = PosStart-1;
	}
	if ( false==bMatch )
	{
		PosEnd = BarcodeLen;
		if ( PosEnd < PosStart )
		{	return false; }
		std::string Code;
		const size_t Len=PosEnd-PosStart;
		Code.assign(&Barcode[PosStart], Len);
		CodeList.push_back(Code);		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcode_General::RemoveOtherWords(const char *Code, const char *Prefix, const char *Suffix, std::string &NewCode)
{
	if ( NULL == Code ) { return false; }
	
	const size_t CodeLen=::strlen(Code);
	const size_t PreLen=::strlen(Prefix);
	const size_t SufLen=::strlen(Suffix);
	if ( PreLen>CodeLen || SufLen>CodeLen )
	{	return false; }

	size_t PosStart=0;
	size_t PosEnd=CodeLen;
	if ( NULL==Prefix || 0==PreLen )
	{	PosStart=0; }
	else
	{
		for ( size_t i=0; i<CodeLen-PreLen+1; i++ )
		{
			bool bMatch=true;
			for ( size_t j=0; j<PreLen; j++ )
			{
				if ( Code[i+j]!=Prefix[j] )
				{	
					bMatch=false;
					break; 
				}
			}
			if ( false == bMatch ) { continue; }
			PosStart = i+PreLen;
			break;
		}
	}
	if ( NULL==Suffix || 0==SufLen )
	{	PosEnd=CodeLen; }
	else
	{
		for ( size_t i=PosStart; i<CodeLen-SufLen+1; i++ )
		{
			bool bMatch=true;
			for ( size_t j=0; j<SufLen; j++ )
			{
				if ( Code[i+j]!=Suffix[j] )
				{	
					bMatch=false;
					break; 
				}
			}
			if ( false == bMatch ) { continue; }
			PosEnd = i;
			break;
		}
	}
	if ( PosEnd < PosStart )
	{	return false; }

	const size_t Len=PosEnd-PosStart;
	NewCode.assign(&Code[PosStart], Len);
	return true;
}
//-------------------------------------------------------------------------------------//
CBarcode_General::CBarcode_General(int DevicdID):m_DeviceID(DevicdID)
{
	PreInitBarcodeDevice_General();
	InitialBarcodeDevice_General();
}
//-------------------------------------------------------------------------------------//
CBarcode_General::~CBarcode_General()
{

}
//-------------------------------------------------------------------------------------//
void CBarcode_General::SetCOMPort(unsigned int nPort)
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
bool CBarcode_General::CheckConnected()//確認是否連線
{
	if (m_RS232COM.IsOpened() == false) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcode_General::ConnectToDevice()
{
	return ConnectDevice_General();
}
//-------------------------------------------------------------------------------------//
bool CBarcode_General::Disconnected()
{
	return Disconnect_General();
}
//-------------------------------------------------------------------------------------//
void CBarcode_General::ClearBuffer()
{
	char temp = ' ';
	if (CheckConnected() == false) { return; }
	while (this->m_RS232COM.ReadSingleChar(temp) != 0)
	{
	}
}
//-------------------------------------------------------------------------------------//
bool CBarcode_General::LoadBarcodeINIFile()
{
	if (CBarcode_Basic::LoadBarcodeINIFile() == false) { return false; }

	const size_t textlen = 128;
	CString Filename;
	CString Section = _T("");	
	CString KeyName = _T("");
	CString Default = _T("");
	TCHAR   String[textlen]=_T("");		
	CString strDeviceName = GetBarcodeDeviceFullName();
	
	Filename = AOIDataCollect.GetSystemParamFilename();	
	Section.Format(_T("%s"), strDeviceName);	
	
	//RS232 Parity
	KeyName.Format(_T("RS232 Parity"));	
	Default.Format(_T("%d"), GetBarcodeDeviceParity());
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	SetBarcodeDeviceParity(::_ttoi(String));	}

	//RS232 Stop Bits
	KeyName.Format(_T("RS232 Stop Bits"));	
	Default.Format(_T("%d"), GetBarcodeDeviceStopBits());
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	SetBarcodeDeviceStopBits(::_ttoi(String));	}
	
	//RS232 Baud Rate
	KeyName.Format(_T("RS232 Baud Rate"));	
	Default.Format(_T("%d"), GetBarcodeDeviceBaudRate());
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	SetBarcodeDeviceBaudRate(::_ttoi(String));	}	

	KeyName.Format(_T("Trigger On (hex)"));
	Default = m_HexTriggerOn.c_str();
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	JetAPI::TCHAR2string(String, m_HexTriggerOn);	}

	KeyName.Format(_T("Trigger Off (hex)"));
	Default = m_HexTriggerOff.c_str();
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	JetAPI::TCHAR2string(String, m_HexTriggerOff);	}

	KeyName.Format(_T("Code Peffix (hex)"));
	Default = m_HexCodePrefix.c_str();
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	JetAPI::TCHAR2string(String, m_HexCodePrefix);	}

	KeyName.Format(_T("Code Suffix (hex)"));
	Default = m_HexCodeSuffix.c_str();
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	JetAPI::TCHAR2string(String, m_HexCodeSuffix);	}

	KeyName.Format(_T("Code Separator (hex)"));
	Default = m_HexCodeSeparator.c_str();
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	JetAPI::TCHAR2string(String, m_HexCodeSeparator);	}
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CBarcode_General::SaveBarcodeINIFile()
{
	if (CBarcode_Basic::SaveBarcodeINIFile() == false) { return false; }

	CString Filename;
	CString Section = _T("");	
	CString KeyName = _T("");
	CString String = _T("");
	CString strDeviceName = GetBarcodeDeviceFullName();
	
	Filename = AOIDataCollect.GetSystemParamFilename();
	Section.Format(_T("%s"), strDeviceName);

	//RS232 Parity
	KeyName.Format(_T("RS232 Parity"));	
	String.Format(_T("%d"), GetBarcodeDeviceParity());
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}	

	//RS232 Stop Bits
	KeyName.Format(_T("RS232 Stop Bits"));	
	String.Format(_T("%d"), GetBarcodeDeviceStopBits());
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}		
	
	//RS232 Baud Rate
	KeyName.Format(_T("RS232 Baud Rate"));	
	String.Format(_T("%d"), GetBarcodeDeviceBaudRate());
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}	

	KeyName.Format(_T("Trigger On (hex)"));
	String = m_HexTriggerOn.c_str();
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	KeyName.Format(_T("Trigger Off (hex)"));
	String = m_HexTriggerOff.c_str();
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}	

	KeyName.Format(_T("Code Peffix (hex)"));
	String = m_HexCodePrefix.c_str();
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}	

	KeyName.Format(_T("Code Suffix (hex)"));
	String = m_HexCodeSuffix.c_str();
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}	

	KeyName.Format(_T("Code Separator (hex)"));
	String = m_HexCodeSeparator.c_str();
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}	
	return true;
}
//-------------------------------------------------------------------------------------//
CString CBarcode_General::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)//載入多國語系
{
	return CBarcode_Basic::LoadMultiLanguageString(KeyName, Default);
}
//-------------------------------------------------------------------------------------//
bool CBarcode_General::Initialize()
{
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CBarcode_General::StartToRead()
{
	if (StartToRead_General() == false)
	{	return false;	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcode_General::EndReading()
{
	const bool clrbuf=true;
	if (EndReading_General(clrbuf) == false)
	{	return false;	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcode_General::RetrieveCode()//接收裝置內的條碼
{
	if (RetrieveCode_General() == false)
	{	return false;	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcode_General::AnalysisResultBuffer()//分析結果字串
{	
	size_t PrefixCount=0;
	size_t SuffixCount=0;
	size_t SeparatorCount=0;
	char CodePrefix[32]="";
	char CodeSuffix[32]="";
	char CodeSeparator[32]="";	
	std::vector<std::string> CodeList;	
	ConvertHexToString(m_HexCodePrefix.c_str(), CodePrefix, sizeof(CodePrefix), PrefixCount);
	ConvertHexToString(m_HexCodeSuffix.c_str(), CodeSuffix, sizeof(CodeSuffix), SuffixCount);
	ConvertHexToString(m_HexCodeSeparator.c_str(), CodeSeparator, sizeof(CodeSeparator), SeparatorCount);

	ExtractSubCode(GetResultBuffer(), CodeSeparator, CodeList);

	const size_t CodeCount=CodeList.size();
	const size_t CodeCountUsed=MIN(CodeCount, BARCODE_SUB_COUNT);
	for ( size_t i=0; i<CodeCountUsed; i++ )
	{
		std::string Code2;
		std::string Code=GetResultBuffer();
		RemoveOtherWords(Code.c_str(), CodePrefix, CodeSuffix, Code2);	
		SetResultSubBuffer(i, Code2.c_str());
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcode_General::CloneResultBuffer(char *Buffer, size_t BufferSize)//複製結果暫存區
{	
	const char  *ResultStr=GetResultBuffer();	
	const size_t ResultLen = ::strlen(ResultStr);	
	const size_t MinLen = MIN(ResultLen, BufferSize-1);
	::memcpy(Buffer, ResultStr, sizeof(char)*MinLen);
	Buffer[MinLen] = '\0';	
	return true;
}
//-------------------------------------------------------------------------------------//
const char* CBarcode_General::GetHexTriggerOn() const
{
	return m_HexTriggerOn.c_str();
}
//-------------------------------------------------------------------------------------//
void CBarcode_General::SetHexTriggerOn(const char *val)
{
	m_HexTriggerOn = val;
}
//-------------------------------------------------------------------------------------//
const char* CBarcode_General::GetHexTriggerOff() const
{
	return m_HexTriggerOff.c_str();
}
//-------------------------------------------------------------------------------------//
void CBarcode_General::SetHexTriggerOff(const char *val)
{
	m_HexTriggerOff = val;
}
//-------------------------------------------------------------------------------------//
const char* CBarcode_General::GetHexCodePrefix() const
{
	return m_HexCodePrefix.c_str();
}
//-------------------------------------------------------------------------------------//
void CBarcode_General::SetHexCodePrefix(const char *val)
{
	m_HexCodePrefix = val;
}
//-------------------------------------------------------------------------------------//
const char* CBarcode_General::GetHexCodeSuffix() const
{
	return m_HexCodeSuffix.c_str();
}
//-------------------------------------------------------------------------------------//
void CBarcode_General::SetHexCodeSuffix(const char *val)
{
	m_HexCodeSuffix = val;
}
//-------------------------------------------------------------------------------------//
const char* CBarcode_General::GetHexCodeSeparator() const
{
	return m_HexCodeSeparator.c_str();
}
//-------------------------------------------------------------------------------------//
void CBarcode_General::SetHexCodeSeparator(const char *val)
{
	m_HexCodeSeparator = val;
}
//-------------------------------------------------------------------------------------//