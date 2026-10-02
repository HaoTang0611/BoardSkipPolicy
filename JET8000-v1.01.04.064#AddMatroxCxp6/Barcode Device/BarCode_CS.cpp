// BarCode_CS.cpp: implementation of the BarCode_CS class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BarCode_CS.h"
#include <process.h>
#include "BarcodeTypeDef.h"

//----------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//----------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------//

const std::string g_kDefIniFilePath = "D:\\Barcodes.INI";

const std::string g_pCodeName[] ={"Code39", "Code128",
									"BC412", "Interleaved 2 of 5",
									"Codabar", "UPC/EAN",
									"Code93", "Pharmacode",
									"Data Matrix", "QR Code",
									"Micro QR Code", "Aztec Code",
									"PDF417", "Micro PDF417"
								};

const int g_pCodeId[] = {BARCODE_CODE_CODE39, BARCODE_CODE_CODE128,
							BARCODE_CODE_BC412, BARCODE_CODE_INTERLEAVED_2OF5,
							BARCODE_CODE_CODABAR, BARCODE_CODE_UPC_EAN,
							BARCODE_CODE_CODE93, BARCODE_CODE_PHARMACODE,
							BARCODE_CODE_DATAMATRIX, BARCODE_CODE_QRCODE,
							BARCODE_CODE_MICRO_QRCODE, BARCODE_CODE_AZTEC_CODE,
							BARCODE_CODE_PDF417, BARCODE_CODE_MICRO_PDF417
							};

enum BarcodeDecoderType
{
	//Support BarCode - 1維部分
	BDT_CODE_39,
	BDT_CODE_128,
	BDT_BC412,
	BDT_INTERLEAVED_2OF5,
	BDT_CODABAR,
	BDT_UPC_EAN,
	BDT_CODE_93,
	BDT_CODE_PHARMACODE,
	//Support BarCode - 2維部分
	BDT_DATA_MATRIX,
	BDT_QR_CODE,
	BDT_MICRO_QR_CODE,
	BDT_AZTEC_CODE,
	BDT_PDF417,
	BDT_MICRO_PDF417,
	BDT_TOTAL
};

namespace Barcode_API
{
	static int GetDecodeSerialId(int DecType)
	{
		for (int j=sizeof(g_pCodeId)/sizeof(int);j--;)
		{
			if (g_pCodeId[j] == DecType)
				return j;
		}
		return -1;
	}

	const std::string GetBarcodeReaderName(const int BarcodeType)
	{
		std::string BarcodeName;
		switch ( BarcodeType )
		{
		case BARCODE_TYPE_NULL:       BarcodeName = "No-Set Barcode Type"; break;
		case BARCODE_SICK_442:        BarcodeName = "Sick CLV 442/432";	break;
		case BARCODE_MICROSCAN_MS3:   BarcodeName = "Microscan MS3";break;
		case BARCODE_SICK_ICR840:     BarcodeName = "Sick ICR 840"; break;
		case BARCODE_DATALOGIC_M1000: BarcodeName = "DataLogic M1000"; break;
		case BARCODE_MICROSCAN_MINI:  BarcodeName = "Microscan MINI";  break;
		case BARCODE_MICROSCAN_MINI3: BarcodeName = "Microscan MINI3"; break;
		case BARCODE_MICROSCAN_MINI_VELOCITY: BarcodeName = "Microscan MINI Velocity"; break;
		case BARCODE_MICROSCAN_MINI_HAWK: BarcodeName = "Microscan MINI Hawk";	break;		
		case BARCODE_DATALOGIC_MATRIX_200: BarcodeName = "DataLogic Marix 200";	break;	
		case BARCODE_DATALOGIC_MATRIX_210: BarcodeName = "DataLogic Marix 210";	break;	
		case BARCODE_DATALOGIC_GFS4400: BarcodeName = "DataLogic GFS4400";	break; //Barcode DataLogic GFS4400 Dean 20150126
		case BARCODE_HONEYWELL_3310GHD: BarcodeName = "Honeywell 3310GHD";	break;//Barcode Honeywell 3310GHD Dean
		case BARCODE_HONEYWELL_1900: BarcodeName = "Honeywell 1900";	break;//Barcode Honeywell 1900 Dean
		default:                      BarcodeName = "Exception !!!";   break;
		}	
		return BarcodeName;
	}

	const std::string GetCodeName(const int CodeID)
	{
		std::string Name = "No Define";
		
		switch ( CodeID )
		{
		case BARCODE_CODE_CODE39:	Name = "Code39";	break;
		case BARCODE_CODE_CODE128:  Name = "Code128";	break;
		case BARCODE_CODE_BC412:    Name = "BC412";		break;
		case BARCODE_CODE_INTERLEAVED_2OF5:
			Name = "Interleaved 2 of 5";
			break;
		case BARCODE_CODE_CODABAR: Name = "Codabar";	break;
		case BARCODE_CODE_UPC_EAN: Name = "UPC/EAN";    break;
		case BARCODE_CODE_CODE93:  Name = "Code93";		break;
		case BARCODE_CODE_PHARMACODE: Name = "Pharmacode"; break;
		case BARCODE_CODE_DATAMATRIX: Name = "Data Matrix";	break;
		case BARCODE_CODE_QRCODE:     Name = "QR Code";     break;
		case BARCODE_CODE_MICRO_QRCODE: Name = "Micro QR Code";	break;
		case BARCODE_CODE_AZTEC_CODE:   Name = "Aztec Code";	break;
		case BARCODE_CODE_PDF417:       Name = "PDF417";		break;
		case BARCODE_CODE_MICRO_PDF417: Name = "Micro PDF417";	break;
		}
		return Name;
	}

	const std::string wstring2string(const std::wstring &rStr)
	{
		char strBuff[3072];
		auto utf8Size = WideCharToMultiByte(CP_UTF8, 0, rStr.c_str(), -1, NULL, 0, NULL, false);	//求size
		WideCharToMultiByte(CP_UTF8, 0, rStr.c_str(), -1, strBuff, utf8Size, NULL, false);//將寬字元字串寫入字串
	
		return std::string(strBuff);	
	}

	const std::wstring string2wstring(const std::string &rStr)
	{	
		int slength = (int)rStr.length() + 1;
		int len = MultiByteToWideChar(CP_ACP, 0, rStr.c_str(), slength, 0, 0); 
		std::wstring r(len, L'\0');
		MultiByteToWideChar(CP_ACP, 0, rStr.c_str(), slength, &r[0], len);
		return r;	
	}
}

//----------------------------------------------------------------------------//
BarCode_CS::BarCode_CS()
{		
	this->m_INIPath = g_kDefIniFilePath;
	this->m_MinCodes = 1;
	this->m_MaxCodes = 1;
	this->m_NCodes   = 1;
	this->m_CodeLength = -1;
	
	this->m_BarcodeID = BIDS_ID_01;
	
	this->m_IsInitialSuccessed = false;
	this->m_IsSupportCalibration = false;
	this->m_BarcodeType = BARCODE_TYPE_NULL;
	this->m_SectionName = "Barcode_CS";
	
	//Support BarCode
	m_SupportDecode.resize(BDT_TOTAL, false);
	m_EnableDecode.resize(BDT_TOTAL, BARCODE_CODE_DISABLE);

	//
	::InitializeCriticalSection(&m_BarcodeCriticalSection);
	this->ClearResultBuffer();	
	this->SetIsExternalTrigger(false);
	ClearErrorMsg();
}
//----------------------------------------------------------------------------//
BarCode_CS::~BarCode_CS()
{
	::DeleteCriticalSection(&m_BarcodeCriticalSection);
	TRACE0("BarCode_CS::~BarCode_CS\n");
}
//----------------------------------------------------------------------------//
void BarCode_CS::Show()
{
	
}
//----------------------------------------------------------------------------//
bool BarCode_CS::GetCode_N(int N, char* str, int BufferSize)
{
	if ( ( N >= BarCode_CS::m_NCodes) || ( N < 0 ) )
	{
		SetErrorMsg("Codes Id out of Range");
		return false;
	}	

	std::string ResultBuffer(BarCode_CS::GetResultBuffer() );

	if ( 0 == ResultBuffer.length() )
	{ 
		SetErrorMsg("Have not data! ");
		return false;
	}

	std::vector<std::string> vTempS(BarCode_CS::m_NCodes, "");

	//separate multi-data
	int nId=0;
	while (1)
	{		
		size_t EP = ResultBuffer.find(SEPARATOR);
		vTempS[nId++] = ResultBuffer.substr(0, EP);
		if (EP == std::string::npos)
		{	
			break;
		}
		ResultBuffer = ResultBuffer.substr(EP + strlen(SEPARATOR) );
	}
	
	if ( static_cast<unsigned int>(BufferSize) <= vTempS[N].length() )
	{	
		SetErrorMsg(" Barcode Out of Buffer size(%d)", vTempS[N].length() );
		return false;	
	}

	strcpy(str, vTempS[N].c_str() );

	return true;
}

//----------------------------------------------------------------------------//
bool BarCode_CS::GetCode_N(std::vector<std::string> &rDataList)
{
	std::string strResultBuffer(m_ResultBuffer);

	if ( 0 == strResultBuffer.length() )
	{ 
		SetErrorMsg("Have not data! ");
		return false;
	}

	rDataList.clear();
	rDataList.reserve(BarCode_CS::m_NCodes + 1);

	//separate multi-data	
	while (1)
	{		
		size_t EP = strResultBuffer.find(SEPARATOR);
		rDataList.push_back(strResultBuffer.substr(0, EP) );
		if (EP == std::string::npos)
		{	
			break;
		}
		strResultBuffer = strResultBuffer.substr(EP + strlen(SEPARATOR) );
	}

	for (size_t j=rDataList.size();j--;)
	{
		if (0 == rDataList[j].compare("NOREAD") )
			rDataList.erase(rDataList.begin() + j);
	}

	return true;
}

//----------------------------------------------------------------------------//
bool BarCode_CS::DoBarcodeReadFn()
{	
	const int kMaxCounts = 15;
	for (int i=0; i < kMaxCounts; i++ )	
	{
		if ( this->ReadData() == true ) 
		{
			return true;
		}
		::Sleep(5);
	}

	return false;	
}
//----------------------------------------------------------------------------//
void BarCode_CS::SetBarcodeID(int BarcodeID)
{
	this->m_BarcodeID = BarcodeID;

	//
	char str[128];
	sprintf_s(str, "%s_%02d", m_SectionName.c_str(), m_BarcodeID);
	m_SectionName = std::string(str);
}
//----------------------------------------------------------------------------//
int BarCode_CS::GetBarcodeID()
{
	return this->m_BarcodeID;
}
//----------------------------------------------------------------------------//
void BarCode_CS::SetIsExternalTrigger(bool Is)
{
	::EnterCriticalSection(&m_BarcodeCriticalSection);
	this->m_IsExternalTrigger = Is;
	::LeaveCriticalSection(&m_BarcodeCriticalSection);	
}
//----------------------------------------------------------------------------//
bool BarCode_CS::GetIsExternalTrigger()
{
	return this->m_IsExternalTrigger;
}
//----------------------------------------------------------------------------//
bool BarCode_CS::GetIsInitialSuccessed()
{
	return this->m_IsInitialSuccessed;
}
//----------------------------------------------------------------------------//
const std::string &BarCode_CS::GetErrMsg()
{
	char str[512];
	sprintf(str, "%s, ( BarcodeID: %d)", m_ErrMsg.c_str(), m_BarcodeID );
	return (m_FullErrMsg = str);
}
//----------------------------------------------------------------------------//
bool BarCode_CS::GetIsSupportCalibration() const
{
	return this->m_IsSupportCalibration;
}
//----------------------------------------------------------------------------//
int BarCode_CS::GetBarcodeType() const
{
	return this->m_BarcodeType;
}
//----------------------------------------------------------------------------//
int BarCode_CS::GetNMultiCodes()
{
	return this->m_NCodes;
}
//----------------------------------------------------------------------------//
bool  BarCode_CS::IsSupportDecode(int SId)
{
	return m_SupportDecode[SId];
}
//----------------------------------------------------------------------------//
bool BarCode_CS::IsEnableDecode(int SId)
{
	return ( (BARCODE_CODE_ENABLE == m_EnableDecode[SId]) ? true : false );
}
//----------------------------------------------------------------------------//
bool BarCode_CS::EnableDecode(int SId)
{
	if ( IsSupportDecode(SId) == false )
	{
		SetErrorMsg("Error, No Supoort Decode (Type = %s)", GetCodeName(SId).c_str() );
		return false;
	}

	m_EnableDecode[SId] = BARCODE_CODE_ENABLE;

	return true;
}

//----------------------------------------------------------------------------//
void BarCode_CS::DisableDecode(int SId)
{
	m_EnableDecode[SId] = BARCODE_CODE_DISABLE;
}

//----------------------------------------------------------------------------//
const std::string BarCode_CS::GetCodeName(int SId)
{
	if (SId < sizeof(g_pCodeId)/sizeof(int) )
	{
		return (g_pCodeName[SId] );
	}

	return "No Define";
}

//----------------------------------------------------------------------------//
int BarCode_CS::GetDecodeSerialId(int DecType)
{
	for (int j=sizeof(g_pCodeId)/sizeof(int);j--;)
	{
		if (g_pCodeId[j] == DecType)
			return j;
	}
	return -1;
}

//----------------------------------------------------------------------------//
bool BarCode_CS::GetDecoder(int CodeID)
{
	int SerialId = GetDecodeSerialId(CodeID);
	return ( (SerialId < 0) ? false : (IsSupportDecode(SerialId) ) );
}
//----------------------------------------------------------------------------//
bool BarCode_CS::SetDecoder(int CodeID, bool TF)
{
	int SerialId = GetDecodeSerialId(CodeID);
	if (SerialId < 0)
		return false;

	m_SupportDecode[SerialId] = TF;
	return true;
}
//----------------------------------------------------------------------------//
bool  BarCode_CS::GetIsEnableCode(int CodeID)
{
	int SerialId = GetDecodeSerialId(CodeID);

	return (SerialId < 0) ? false : IsEnableDecode(SerialId);
}
//----------------------------------------------------------------------------//
bool  BarCode_CS::EnableCode(int CodeID)
{
	int SerialId = GetDecodeSerialId(CodeID);
	return ( (SerialId < 0) ? false : (EnableDecode(SerialId) ) );	
}
//----------------------------------------------------------------------------//
void BarCode_CS::DisableCode(int CodeID)
{	
	int SerialId = GetDecodeSerialId(CodeID);
	if ( 0 <= SerialId)
		this->DisableDecode(SerialId);	
}
//----------------------------------------------------------------------------//
void BarCode_CS::ClearResultBuffer()
{
	::EnterCriticalSection(&m_BarcodeCriticalSection);
	this->m_ResultBuffer[0]='\0';
	::LeaveCriticalSection(&m_BarcodeCriticalSection);
}
//----------------------------------------------------------------------------//
const char* BarCode_CS::GetResultBuffer() const
{
	return this->m_ResultBuffer;
}
//----------------------------------------------------------------------------//
void BarCode_CS::SetResultBuffer(const char* String)
{
	const int Len = (int)(::strlen(String));
	const int MaxLen = sizeof(this->m_ResultBuffer)/sizeof(this->m_ResultBuffer[0])-1;

	::EnterCriticalSection(&m_BarcodeCriticalSection);
	if ( Len<MaxLen )
	{	::strcpy(this->m_ResultBuffer, String);	}
	else
	{
		::memcpy(this->m_ResultBuffer, String, sizeof(char)*MaxLen);
		this->m_ResultBuffer[MaxLen]='\0';
	}	
	::LeaveCriticalSection(&m_BarcodeCriticalSection);
}
//----------------------------------------------------------------------------//
int BarCode_CS::GetCodeLength() const
{
	return this->m_CodeLength;
}

void BarCode_CS::SetTimeout(int TimeoutMs)
{
	m_Timeout = TimeoutMs;
}

int BarCode_CS::GetTimeout()
{
	return m_Timeout;
}

void BarCode_CS::SetINIFilePath(const std::string &rFilePath)
{
	this->m_INIPath = rFilePath;
}

const std::string &BarCode_CS::GetINIFilePath() const
{
	return m_INIPath;
}

void BarCode_CS::SetInitializeFlag(bool Enable)
{
	this->m_IsInitialSuccessed = Enable;
}

void BarCode_CS::SetErrorMsg(char *format, ...)
{
	char strCmd[128];

	int nBytes = vsprintf(strCmd, format, ( (char *)&format)+sizeof(char *) );

	m_ErrMsg = strCmd;
}

void BarCode_CS::ClearErrorMsg()
{
	m_ErrMsg = "";
}

//---------------------------------------------------------------------------------------------------//
bool BarCode_CS::FindStringDrop(std::string &rSrcDec, const std::string &rObject, bool Foreward)
{
	size_t pos = 0;

	if (Foreward)
		pos = rSrcDec.find(rObject);
	else
		pos = rSrcDec.rfind(rObject);

	if (pos == std::string::npos)
	{
		//waining for not found the object string
		return false;
	}

	//drop string
	if (Foreward)		
		rSrcDec = rSrcDec.substr(0, pos);
	else
		rSrcDec = rSrcDec.substr(pos + rObject.length());

	return true;
}
//---------------------------------------------------------------------------------------------------//