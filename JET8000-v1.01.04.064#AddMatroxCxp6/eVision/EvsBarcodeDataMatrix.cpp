// EvsBarcodeDataMatrix.cpp: implementation of the CEvsBarcodeDataMatrix class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "EvsBarcodeDataMatrix.h"
//-------------------------------------------------------------------------------------//
#include "EVisionLibDef.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
#define EVISION_DATA_MATRIX_READER_1    1
#define EVISION_DATA_MATRIX_READER_2    2
//-------------------------------------------------------------------------------------//
#define EVISION_DATA_MATRIX_TIMEOUT     200000
//-------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
CEvsBarcodeDataMatrix::CEvsBarcodeDataMatrix()
{
	CEvsBarcodeDataMatrix::PreInitDataMatrix();
	CEvsBarcodeDataMatrix::InitialDataMatrix();
}
//-------------------------------------------------------------------------------------//
CEvsBarcodeDataMatrix::CEvsBarcodeDataMatrix(const CEvsBarcodeDataMatrix &DataMatrix)
{
	CEvsBarcodeDataMatrix::PreInitDataMatrix();
	CEvsBarcodeDataMatrix::CloneDataMatrix(DataMatrix);
}
//-------------------------------------------------------------------------------------//
CEvsBarcodeDataMatrix::~CEvsBarcodeDataMatrix()
{	
}
//-------------------------------------------------------------------------------------//
CEvsBarcodeDataMatrix& CEvsBarcodeDataMatrix::operator=(const CEvsBarcodeDataMatrix &DataMatrix)
{
	if ( this == &DataMatrix ) { return *this; }
	CEvsBarcodeDataMatrix::CloneDataMatrix(DataMatrix);
	return *this;
}
//-------------------------------------------------------------------------------------//
void CEvsBarcodeDataMatrix::PreInitDataMatrix()
{
	m_ErrorCode = 0;
	m_EvsDecodeReader = EVISION_DATA_MATRIX_READER_2;
	m_EvsDecodeTimeout = EVISION_DATA_MATRIX_TIMEOUT;
	m_EvsDecodeTimeoutDefault = EVISION_DATA_MATRIX_TIMEOUT;
	::memset(m_ErrorString, 0x00, sizeof(m_ErrorString));
	m_ErrorStringLen = sizeof(m_ErrorString)/sizeof(m_ErrorString[0]);
}
//-------------------------------------------------------------------------------------//
void CEvsBarcodeDataMatrix::InitialDataMatrix()
{
	LoadDataMatrixIniFile();
	SaveDataMatrixIniFile();
}
//-------------------------------------------------------------------------------------//
void CEvsBarcodeDataMatrix::CloneDataMatrix(const CEvsBarcodeDataMatrix &DataMatrix)
{
	this->m_ErrorCode = DataMatrix.m_ErrorCode;
	m_ErrorStringLen = DataMatrix.m_ErrorStringLen;
	::strcpy(m_ErrorString, DataMatrix.m_ErrorString);
	m_EvsDecodeReader = DataMatrix.m_EvsDecodeReader;
	m_EvsDecodeTimeout = DataMatrix.m_EvsDecodeTimeout;
	m_EvsDecodeTimeoutDefault = DataMatrix.m_EvsDecodeTimeoutDefault;
	//this->m_DataMatrixReader = DataMatrix.m_DataMatrixReader;
}
//-------------------------------------------------------------------------------------//
const char* CEvsBarcodeDataMatrix::GetErrorString() const
{
	return this->m_ErrorString;
}
//-------------------------------------------------------------------------------------//
bool CEvsBarcodeDataMatrix::SaveINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pString, LPCTSTR pfilename, CString &Error)
{
	if ( JetAPI::SaveINIData(pSection, pKeyName, pString, pfilename, Error) == false ) 
	{
		JetAPI::TCHAR2char(Error, m_ErrorString, m_ErrorStringLen);
		return false;	
	}	
	return true;
}
//--------------------------------------------------------------------------------------------//
bool CEvsBarcodeDataMatrix::LoadINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pDefault, TCHAR *pString, int StringSize, LPCTSTR pfilename, bool IsCheckLens, CString &Error)	
{
	if ( JetAPI::LoadINIData(pSection, pKeyName, pDefault, pString, StringSize, pfilename, IsCheckLens, Error) == false )
	{
		JetAPI::TCHAR2char(Error, m_ErrorString, m_ErrorStringLen);
		return false; 
	}
	return true;
}
//--------------------------------------------------------------------------------------------//
bool CEvsBarcodeDataMatrix::SaveDataMatrixIniFile()//纗DataMatrix把计郎
{
	CString EvsErr;
	CString filename;
	CString Section;
	CString KeyName;
	CString String;		

	filename.Format(_T("%s\\%s"), AOIDataCollect.GetAOIDirectory(), _T("BarcodeDecoder.INI"));	
	Section = _T("EvsBarcode DataMatrix");
	
	KeyName = _T("Decode Reader");
	String.Format(_T("%d"), m_EvsDecodeReader);
	SaveINIData(Section, KeyName, String, filename, EvsErr);

	KeyName = _T("Default Timeout");
	String.Format(_T("%d"), m_EvsDecodeTimeoutDefault);
	SaveINIData(Section, KeyName, String, filename, EvsErr);
	return true;
}
//--------------------------------------------------------------------------------------------//
bool CEvsBarcodeDataMatrix::LoadDataMatrixIniFile()//更DataMatrix把计郎	
{
	int nVal=0;	
	CString EvsErr;
	CString filename;
	CString Section;
	CString KeyName;
	CString Default;	
	const size_t StringSize = MAX_JET_PATH;
	TCHAR   String[StringSize] = _T("");	

	filename.Format(_T("%s\\%s"), AOIDataCollect.GetAOIDirectory(), _T("BarcodeDecoder.INI"));
	Section = _T("EvsBarcode DataMatrix");

	KeyName = _T("Decode Reader");
	Default.Format(_T("%d"), m_EvsDecodeReader);
	if ( LoadINIData(Section, KeyName, Default, String, StringSize, filename, false, EvsErr) == false )
	{	return false;	}
	nVal = ::_ttoi(String);	
	switch ( nVal )
	{
	case EVISION_DATA_MATRIX_READER_1:
	case EVISION_DATA_MATRIX_READER_2:
		m_EvsDecodeReader = nVal;
		break;
	}	

	KeyName = _T("Default Timeout");
	Default.Format(_T("%d"), m_EvsDecodeTimeoutDefault);
	if ( LoadINIData(Section, KeyName, Default, String, StringSize, filename, false, EvsErr) == false )
	{	return false;	}
	m_EvsDecodeTimeoutDefault = ::_ttoi(String);
	m_EvsDecodeTimeoutDefault = MAX(1000, m_EvsDecodeTimeoutDefault);
	return true;
}
//--------------------------------------------------------------------------------------------//
bool CEvsBarcodeDataMatrix::GetIsEVisionError()
{
#ifdef EVISION_DATA_MATRIX_USE	
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
#endif//EVISION_DATA_MATRIX_USE	
	return false;
}
//-------------------------------------------------------------------------------------//
int CEvsBarcodeDataMatrix::GetDecodeReader() const
{
#if EVISION_MODE_OPEN_EVISION_VERSION < EVISION_MODE_OPEN_EVISION_22_12_2_15123
	return EVISION_DATA_MATRIX_READER_1;
#endif//EVISION_MODE_OPEN_EVISION_VERSION
	return m_EvsDecodeReader;
}
//-------------------------------------------------------------------------------------//
bool CEvsBarcodeDataMatrix::Reader1(CEvsRoiBW8 &Roi, char* BarcodeText, size_t BarcodeTextLen)
{	
#ifdef EVISION_DATA_MATRIX_USE		
	#if EVISION_MODE == EVISION_MODE_EVISION
		try
		{	
			int Timeout = m_EvsDecodeTimeout*1000;
			Euresys::eVision::MatrixCode       DataMatrixCode;
			Euresys::eVision::MatrixCodeReader DataMatrixReader;			

			if ( 0 == Timeout ) { Timeout = m_EvsDecodeTimeoutDefault; }
			DataMatrixReader.SetTimeOut(Timeout);
			DataMatrixCode = DataMatrixReader.Read(*(Roi.GetRoiBW8Ptr()));			
			const size_t len = DataMatrixCode.DecodedString.length();
			if ( this->GetIsEVisionError() == true )
			{	return false; }
			if ( len == 0  )
			{		
				::strcpy(BarcodeText, "");
				::sprintf(this->m_ErrorString, "Error, Length of EMatrixCode DecodedString Exception (%d)", len);
				return true;	
			}

			if ( len >= BarcodeTextLen )
			{
				::memcpy(BarcodeText, DataMatrixCode.DecodedString.c_str(), sizeof(char)*(BarcodeTextLen-1));
				BarcodeText[BarcodeTextLen-1] = '\0';
			}
			else
			{	::strcpy(BarcodeText, DataMatrixCode.DecodedString.c_str()); }
			return true;
		}
		catch ( Euresys::eVision::Exception exc )
		{	
			::sprintf(this->m_ErrorString, "Error, Software Barcode MatrixCode Exception (%s)", exc.What());
			EOk();	
			return false;
		}
	#elif EVISION_MODE == EVISION_MODE_OPEN_EVISION
		try
		{				
			this->m_ErrorCode = EError_Ok;
			int Timeout = m_EvsDecodeTimeout*1000;
			EMatrixCode       DataMatrixCode;
			EMatrixCodeReader DataMatrixReader;			

			if ( 0 == Timeout ) { Timeout = m_EvsDecodeTimeoutDefault; }
			DataMatrixReader.SetTimeOut(Timeout);
			DataMatrixCode = DataMatrixReader.Read(*(Roi.GetRoiBW8Ptr()));
			std::string str = DataMatrixCode.GetDecodedString();
			const size_t len = (size_t)(str.length());
			if ( len == 0  )
			{		
				::strcpy(BarcodeText, "");
				::sprintf(this->m_ErrorString, "Error, Length of EMatrixCode DecodedString Exception (%d)", len);
				return true;	
			}

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
#endif//EVISION_DATA_MATRIX_USE
	return false;
}
//-------------------------------------------------------------------------------------//
bool CEvsBarcodeDataMatrix::Reader2(CEvsRoiBW8 &Roi, char* BarcodeText, size_t BarcodeTextLen)
{
#ifdef EVISION_DATA_MATRIX_USE		
#if EVISION_MODE == EVISION_MODE_OPEN_EVISION
#if EVISION_MODE_OPEN_EVISION_VERSION >= EVISION_MODE_OPEN_EVISION_22_12_2_15123//EVISION_MODE_OPEN_EVISION_23_12_0_18439
	try
	{		
		this->m_ErrorCode = EError_Ok;
		int Timeout = m_EvsDecodeTimeout*1000;
		std::vector<EasyMatrixCode2::EMatrixCode> DataMatrixCode; // EasyMatrixCode2::EMatrixCode instances
		EasyMatrixCode2::EMatrixCodeReader DataMatrixReader; // EasyMatrixCode2::EMatrixCodeReader instance	
		const int TimeoutBefore = DataMatrixReader.GetTimeOut();//-1
		if ( 0 == Timeout ) { Timeout = m_EvsDecodeTimeoutDefault; }			
		DataMatrixReader.SetTimeOut(Timeout);//us
		DataMatrixCode = DataMatrixReader.Read(*(Roi.GetRoiBW8Ptr()));		
	
		std::string str;			
		const size_t MatrixCodeCount=DataMatrixCode.size();
		if ( MatrixCodeCount > 0 )
		{
			EasyMatrixCode2::EMatrixCode &Code=DataMatrixCode[0];
			str = Code.GetDecodedString();
		}	
		const size_t len = (size_t)(str.length());
		if ( len == 0  )
		{		
			::strcpy(BarcodeText, "");
			::sprintf(this->m_ErrorString, "Error, Length of EMatrixCode2 DecodedString Exception (%d)", len);
			return true;	
		}

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
#endif//EVISION_MODE_OPEN_EVISION_VERSION
#endif//EVISION_MODE
#endif//EVISION_DATA_MATRIX_USE
	return false;
}
//-------------------------------------------------------------------------------------//
bool CEvsBarcodeDataMatrix::Read(int ImageW, int ImageH, int ImageStep, unsigned char* pImage, char* BarcodeText, size_t BarcodeTextLen)
{
#ifdef EVISION_DATA_MATRIX_USE
	CEvsRoiBW8   RoiBW8;
	CEvsImageBW8 ImageBW8;
	const bool bByFile=false;//EVISION_MODE_OPEN_EVISION_23_12_0_18439
	if ( ImageBW8.SetImagePtr(pImage, ImageW, ImageH, ImageStep, true, bByFile) == false ) 
	{	return false; }

	RoiBW8.Attach(&ImageBW8);
	RoiBW8.SetPlacement(0, 0, ImageW, ImageH);
	
	bool bSucc = true;
	const int DecodeReader=GetDecodeReader();
	switch ( DecodeReader )
	{
	case EVISION_DATA_MATRIX_READER_2:	bSucc = Reader2(RoiBW8, BarcodeText, BarcodeTextLen);	break;
	default:
	case EVISION_DATA_MATRIX_READER_1:	bSucc = Reader1(RoiBW8, BarcodeText, BarcodeTextLen);	break;
	}	
	RoiBW8.Detach();
	return bSucc;
#endif//EVISION_DATA_MATRIX_USE
	return false;
}
//-------------------------------------------------------------------------------------//