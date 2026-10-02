// EvsBarcodeQRCode.cpp: implementation of the CEvsBarcodeQRCode class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "EvsBarcodeQRCode.h"
//-------------------------------------------------------------------------------------//
#include "EVisionLibDef.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
#define EVISION_QRCODE_TIMEOUT     200000
//-------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
CEvsBarcodeQRCode::CEvsBarcodeQRCode()
{
	CEvsBarcodeQRCode::PreInitQRCode();
	CEvsBarcodeQRCode::InitialQRCode();
}
//-------------------------------------------------------------------------------------//
CEvsBarcodeQRCode::CEvsBarcodeQRCode(const CEvsBarcodeQRCode &QRCode)
{
	CEvsBarcodeQRCode::PreInitQRCode();
	CEvsBarcodeQRCode::CloneQRCode(QRCode);
}
//-------------------------------------------------------------------------------------//
CEvsBarcodeQRCode::~CEvsBarcodeQRCode()
{

}
//-------------------------------------------------------------------------------------//
CEvsBarcodeQRCode& CEvsBarcodeQRCode::operator=(const CEvsBarcodeQRCode &QRCode)
{
	if ( this == &QRCode ) { return *this; }
	CEvsBarcodeQRCode::CloneQRCode(QRCode);
	return *this;
}
//-------------------------------------------------------------------------------------//
void CEvsBarcodeQRCode::PreInitQRCode()
{
	m_ErrorCode = 0;
	m_EvsDecodeTimeout = EVISION_QRCODE_TIMEOUT;
	m_EvsDecodeTimeoutDefault = EVISION_QRCODE_TIMEOUT;
	::memset(m_ErrorString, 0x00, sizeof(m_ErrorString));
	m_ErrorStringLen = sizeof(m_ErrorString)/sizeof(m_ErrorString[0]);
}
//-------------------------------------------------------------------------------------//
void CEvsBarcodeQRCode::InitialQRCode()
{
	LoadQRCodeIniFile();
	SaveQRCodeIniFile();
}
//-------------------------------------------------------------------------------------//
void CEvsBarcodeQRCode::CloneQRCode(const CEvsBarcodeQRCode &QRCode)
{
	this->m_ErrorCode = QRCode.m_ErrorCode;
	m_ErrorStringLen = QRCode.m_ErrorStringLen;
	::strcpy(m_ErrorString, QRCode.m_ErrorString);
	m_EvsDecodeTimeout = QRCode.m_EvsDecodeTimeout;
	m_EvsDecodeTimeoutDefault = QRCode.m_EvsDecodeTimeoutDefault;
	//this->m_DataMatrixReader = QRCode.m_DataMatrixReader;
}
//-------------------------------------------------------------------------------------//
const char* CEvsBarcodeQRCode::GetErrorString() const
{
	return this->m_ErrorString;
}
//-------------------------------------------------------------------------------------//
bool CEvsBarcodeQRCode::ReturnNoSupportQRCode()
{
	::strcpy(m_ErrorString, "Error, Not Support EVision QRCode");
	return false;
}
//-------------------------------------------------------------------------------------//
bool CEvsBarcodeQRCode::SaveINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pString, LPCTSTR pfilename, CString &Error)
{
	if ( JetAPI::SaveINIData(pSection, pKeyName, pString, pfilename, Error) == false ) 
	{
		JetAPI::TCHAR2char(Error, m_ErrorString, m_ErrorStringLen);
		return false;	
	}	
	return true;
}
//--------------------------------------------------------------------------------------------//
bool CEvsBarcodeQRCode::LoadINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pDefault, TCHAR *pString, int StringSize, LPCTSTR pfilename, bool IsCheckLens, CString &Error)	
{
	if ( JetAPI::LoadINIData(pSection, pKeyName, pDefault, pString, StringSize, pfilename, IsCheckLens, Error) == false )
	{
		JetAPI::TCHAR2char(Error, m_ErrorString, m_ErrorStringLen);
		return false; 
	}
	return true;
}
//--------------------------------------------------------------------------------------------//
bool CEvsBarcodeQRCode::SaveQRCodeIniFile()//纗QRCode把计郎
{
	CString EvsErr;
	CString filename;
	CString Section;
	CString KeyName;
	CString String;		
	
	filename.Format(_T("%s\\%s"), AOIDataCollect.GetAOIDirectory(), _T("BarcodeDecoder.INI"));
	Section = _T("EvsBarcode QRCode");
	
	KeyName = _T("Default Timeout");
	String.Format(_T("%d"), m_EvsDecodeTimeoutDefault);
	SaveINIData(Section, KeyName, String, filename, EvsErr);
	return true;
}
//--------------------------------------------------------------------------------------------//
bool CEvsBarcodeQRCode::LoadQRCodeIniFile()//更QRCode把计郎	
{
	int nVal=0;	
	CString DTKErr;
	CString filename;
	CString Section;
	CString KeyName;
	CString Default;	
	const size_t StringSize = MAX_JET_PATH;
	TCHAR   String[StringSize] = _T("");	

	filename.Format(_T("%s\\%s"), AOIDataCollect.GetAOIDirectory(), _T("BarcodeDecoder.INI"));
	Section = _T("EvsBarcode QRCode");

	KeyName = _T("Default Timeout");
	Default.Format(_T("%d"), m_EvsDecodeTimeoutDefault);
	if ( LoadINIData(Section, KeyName, Default, String, StringSize, filename, false, DTKErr) == false )
	{	return false; }
	m_EvsDecodeTimeoutDefault = ::_ttoi(String);
	m_EvsDecodeTimeoutDefault = MAX(1000, m_EvsDecodeTimeoutDefault);
	return true;
}
//--------------------------------------------------------------------------------------------//
bool CEvsBarcodeQRCode::GetIsEVisionError()
{
#ifdef EVISION_QRCODE_USE	
	#if EVISION_MODE == EVISION_MODE_EVISION
		if (EGetError( ) != E_OK)
		{
			sprintf(this->m_ErrorString, "%s", EGetErrorText());
			EOk();
			return true;
		}
	#elif EVISION_MODE == EVISION_MODE_OPEN_EVISION
		if ( this->m_ErrorCode != EError_Ok )
		{	return true;	}		
	#endif	
	return false;
#endif//EVISION_QRCODE_USE	
	return false;
}
//-------------------------------------------------------------------------------------//
bool CEvsBarcodeQRCode::Read(CEvsRoiBW8 &Roi, char* BarcodeText, size_t BarcodeTextLen)
{	
#ifdef EVISION_QRCODE_USE		
	#if EVISION_MODE == EVISION_MODE_EVISION
		return ReturnNoSupportQRCode();		
	#elif EVISION_MODE == EVISION_MODE_OPEN_EVISION
		try
		{				
			this->m_ErrorCode = EError_Ok;
			int Timeout = m_EvsDecodeTimeout*1000;
			EQRCodeReader         QRCodeReader;
			std::vector<EQRCode>  QRCodeList;
			std::vector<char>     strAll;

			if ( 0 == Timeout ) { Timeout = m_EvsDecodeTimeoutDefault; }
			QRCodeReader.SetTimeOut(Timeout);
			QRCodeReader.SetSearchField(*(Roi.GetRoiBW8Ptr()));
			QRCodeList = QRCodeReader.Read();
			const size_t QRCodeCount=QRCodeList.size();
			if ( 0 == QRCodeCount )
			{
				::strcpy(BarcodeText, "");
				::sprintf(this->m_ErrorString, "Error, QRCode Count Exception (%d)", QRCodeCount);
				return true;	
			}			
			for ( size_t i=0; i<QRCodeCount; i++ )
			{
				std::vector<unsigned char> strBarcode;
				EQRCode &QRCodeRef = QRCodeList[i];
			#if EVISION_MODE_OPEN_EVISION_VERSION >= EVISION_MODE_OPEN_EVISION_23_12_0_18439
				const EQRCodeDecodedStream &QRCodeDecodedStreamRef = QRCodeRef.GetDecodedStream();
			#else
				EQRCodeDecodedStream &QRCodeDecodedStreamRef = QRCodeRef.GetDecodedStream();
			#endif//EVISION_MODE_OPEN_EVISION_VERSION
				std::vector<EQRCodeDecodedStreamPart> DecodedStreamList=QRCodeDecodedStreamRef.GetDecodedStreamParts();
				const size_t DecodedCount=DecodedStreamList.size();
				for ( size_t j=0; j<DecodedCount; j++ )
				{
					EQRCodeDecodedStreamPart &StreamPartRef=DecodedStreamList[j];
					strBarcode = StreamPartRef.GetDecodedData();
					const size_t strBarcodeCount=strBarcode.size();
					for ( size_t k=0; k<strBarcodeCount; k++ )
					{	strAll.push_back(strBarcode[k]);	}
				}				
			}
			strAll.push_back('\0');
			char *Ptr=&strAll[0];
			std::string str = Ptr;
			const size_t len = str.length();
			if ( len >= BarcodeTextLen )
			{
				::memcpy(BarcodeText, str.c_str(), sizeof(char)*(BarcodeTextLen-1));
				BarcodeText[BarcodeTextLen-1] = '\0';
			}
			else
			{	::strcpy(BarcodeText, str.c_str()); }
			return true;
		}
		catch(EException exc)
		{	
			m_ErrorCode = exc.GetError();
			sprintf(this->m_ErrorString, "%s", exc.What().c_str());
			return false;
		}	
	#endif//EVISION_MODE
#endif//EVISION_QRCODE_USE
	return false;
}
//-------------------------------------------------------------------------------------//
bool CEvsBarcodeQRCode::Read(int ImageW, int ImageH, int ImageStep, unsigned char* pImage, char* BarcodeText, size_t BarcodeTextLen)
{
#ifdef EVISION_QRCODE_USE
	CEvsRoiBW8   RoiBW8;
	CEvsImageBW8 ImageBW8;
	if ( ImageBW8.SetImagePtr(pImage, ImageW, ImageH, ImageStep, true) == false ) 
	{	return false; }

	RoiBW8.Attach(&ImageBW8);
	RoiBW8.SetPlacement(0, 0, ImageW, ImageH);
	if ( Read(RoiBW8, BarcodeText, BarcodeTextLen) == false )
	{
		RoiBW8.Detach();
		return false; 
	}
	RoiBW8.Detach();
	return true;
#endif//EVISION_QRCODE_USE
	return false;
}
//-------------------------------------------------------------------------------------//