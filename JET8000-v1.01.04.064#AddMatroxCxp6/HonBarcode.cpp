// HonBarcode.cpp: implementation of the CHonBarcode class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "HonBarcode.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
#ifdef HON_BARCODE_USE
#include "..\\JET8000_Library\\HoneywellSwiftDecoder\\Include\\sds_api.h"
#ifdef _X64
	#define VOID_POINTER_TO_INT                     (int)(long long)
	#define VOID_POINTER_TO_UNSIGNED_INT   (unsigned int)(long long)
	#define INT_TO_VOID_POINTER                  (void *)(long long)
	#define VOID_POINTER_CONTENT                       * (long long *)
	//#pragma comment(lib,"libid_FlexRelease_x64.lib")
	#pragma comment(lib,"..\\JET8000_Library\\HoneywellSwiftDecoder\\Lib\\libid_FlexRelease_x64.lib")	
#else
	#define VOID_POINTER_TO_INT                     (int)
	#define VOID_POINTER_TO_UNSIGNED_INT   (unsigned int)
	#define INT_TO_VOID_POINTER                  (void *)
	#define VOID_POINTER_CONTENT                       * (int *)
	//#pragma comment(lib,"libid_FlexRelease_x86.lib")
#pragma comment(lib,"..\\JET8000_Library\\HoneywellSwiftDecoder\\Lib\\libid_FlexRelease_x86.lib")	
	#error Error, Honeywell SDK is not support x86
#endif//_X64
#endif//HON_BARCODE_USE
//-------------------------------------------------------------------------------------//
CHonBarcode   HoneywellBarcodeSdk;
//-------------------------------------------------------------------------------------//
void CHonBarcode::SD_CB_Result(int Handle)
{	
	HoneywellBarcodeSdk.HoneywellSwiftDecoder_ReadResult();	
	return;
}
//-------------------------------------------------------------------------------------//
/*
int CHonBarcode::m_HandleHon = 0;
bool CHonBarcode::m_bLicenseHon=false;
CString CHonBarcode::m_ErrorString=_T("");
CString CHonBarcode::m_LicenseFilename=_T("Response.bin");
CRITICAL_SECTION CHonBarcode::m_csHon;//同步機制-關鍵區間
*/
//-------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
void CHonBarcode::InitialHonBarcodeLock()//初始化Honeywell條碼關鍵區間
{	
	::InitializeCriticalSection(&m_csHon);
}
//-------------------------------------------------------------------------------------//
void CHonBarcode::DeleteHonBarcodeLock()//刪除Honeywell條碼關鍵區間
{
	::DeleteCriticalSection(&m_csHon);
}
//-------------------------------------------------------------------------------------//
void CHonBarcode::LockHonBarcode()//進入Honeywell條碼關鍵區間
{
	::EnterCriticalSection(&m_csHon);	
}
//-------------------------------------------------------------------------------------//
void CHonBarcode::UnlockHonBarcode()//離開Honeywell條碼關鍵區間
{
	::LeaveCriticalSection(&m_csHon);	
}
//-------------------------------------------------------------------------------------//
bool CHonBarcode::CheckHonLicenseKey()//確認Honeywell授權通過
{		
	m_bLicenseHon = false;		
#ifdef HON_BARCODE_USE
	int ActivationResult = 0;	
	//char * StoragePath = "./";   	
	char StoragePath[] = AOI3D_MAIN_FOLDER_A;//"C:\\JETAOI3D";
	const bool OfflineLicense=true;
	const char * LicenseKey = "siot-2018-8959-jmurp-06152022";
	//const char * LicenseKey = "trial-siot-jette-zchea-06012022";//Test License
	if ( false == OfflineLicense )
	{	ActivationResult = SD_RemoteEntitlementActivation(LicenseKey, StoragePath); }
	else
	{
		const size_t StrLen=256;
		//char *BinFilename="Response.bin";
		char BinFilename[StrLen]="Response.bin";		
		if ( m_LicenseFilename.GetLength() > 0 )
		{	
			if ( JetAPI::TCHAR2char(m_LicenseFilename, BinFilename, StrLen) == false )
			{	::strcpy(BinFilename, "Response.bin");	}
		}
		const std::string filename=std::string(StoragePath)+std::string("\\")+std::string(BinFilename);
		CString Filename(filename.c_str());
		if ( JetAPI::IsFileExist(Filename) == false )
		{
			m_ErrorString.Format(_T("Error, No License File(%s"), Filename);
			return m_bLicenseHon;
		}
		ActivationResult = SD_ConsumeLicenseResponse(LicenseKey, StoragePath, BinFilename);	
	}	
	if ( 1 == ActivationResult )
	{	m_bLicenseHon = true;	}		
	else
	{	m_ErrorString=_T("Error, No License");	}
#endif//HON_BARCODE_USE
	return m_bLicenseHon;
}
//-------------------------------------------------------------------------------------//
bool CHonBarcode::GetHonLibraryVersion(CString &str)//取得Honeywell條碼函式庫版本
{
	if ( CheckSwiftDecoderHandle() == false ) { return false; }
	int Major=0, Minor=0, Build=0;		
#ifdef HON_BARCODE_USE
	if ( HoneywellSwiftDecoder_Read(SD_PROP_VERSION_MAJOR, &Major) == false ) { return false; }
	if ( HoneywellSwiftDecoder_Read(SD_PROP_VERSION_MINOR, &Minor) == false ) { return false; }
	if ( HoneywellSwiftDecoder_Read(SD_PROP_VERSION_BUILD, &Build) == false ) { return false; }
#endif//HON_BARCODE_USE
	str.Format(_T("SwiftDecoder v%02d.%02d.%02d"), Major, Minor, Build);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CHonBarcode::SaveINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pString, LPCTSTR pfilename, CString &Error)
{
	if ( JetAPI::SaveINIData(pSection, pKeyName, pString, pfilename, Error) == false ) 
	{	return false;	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CHonBarcode::LoadINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pDefault, TCHAR *pString, int StringSize, LPCTSTR pfilename, bool IsCheckLens, CString &Error)	
{
	if ( JetAPI::LoadINIData(pSection, pKeyName, pDefault, pString, StringSize, pfilename, IsCheckLens, Error) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CHonBarcode::SaveHonIniFile()//儲存Honeywell參數檔案
{		
	CString IniErr;
	CString filename;
	CString Section;
	CString KeyName;
	CString String;

	filename.Format(_T("%s\\%s"), AOIDataCollect.GetAOIDirectory(), _T("BarcodeDecoder.INI"));
	Section = _T("Honeywell SwiftDecoder Decoder");
	
	KeyName = _T("License Filename");
	String.Format(_T("%s"), m_LicenseFilename);
	SaveINIData(Section, KeyName, String, filename, IniErr);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CHonBarcode::LoadHonIniFile()//載入Honeywell參數檔案
{
	int nVal=0;
	CString IniErr;
	CString filename;
	CString Section;
	CString KeyName;
	CString Default;
	const size_t StringSize = MAX_JET_PATH;
	TCHAR   String[StringSize] = _T("");

	filename.Format(_T("%s\\%s"), AOIDataCollect.GetAOIDirectory(), _T("BarcodeDecoder.INI"));
	Section = _T("Honeywell SwiftDecoder Decoder");

	KeyName = _T("License Filename");
	Default.Format(_T("%s"), m_LicenseFilename);
	if ( LoadINIData(Section, KeyName, Default, String, StringSize, filename, false, IniErr) == false )
	{	return false; }
	m_LicenseFilename = String;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CHonBarcode::CheckSwiftDecoderHandle()
{
	if ( 0 == m_HandleHon )
	{
		m_ErrorString=_T("Error, No Honeywell SwiftDecoder Handle");
		return false;
	}
	return true;
}
//--------------------------------------------------------------------------------------------//
bool CHonBarcode::CheckSwiftDecoderLicense()
{
	if ( false == m_bLicenseHon )
	{	m_ErrorString=_T("Error, Honeywell SwiftDecoder No License Key");	}
	return m_bLicenseHon;
}
//--------------------------------------------------------------------------------------------//
bool CHonBarcode::HoneywellSwiftDecoder_Create()
{
#ifdef HON_BARCODE_USE
	if ( HoneywellSwiftDecoder_Destroy() == false ) { return false; }	
	m_HandleHon = (int)SD_Create();
	if ( 0 == m_HandleHon )
	{
		m_ErrorString.Format(_T("Error, Honeywell SwiftDecoder SD_Create failed, Error = %d\n"), (int)SD_GetLastError());
		return false;
	} 
#endif//HON_BARCODE_USE
	return true;
}
//--------------------------------------------------------------------------------------------//
bool CHonBarcode::HoneywellSwiftDecoder_Destroy()
{
#ifdef HON_BARCODE_USE
	if ( CheckSwiftDecoderLicense() == false ) { return false; }
	if ( CheckSwiftDecoderHandle() == false ) { return true; }
	int Ret=SD_Destroy(m_HandleHon);	 
	m_HandleHon = 0;
#endif//HON_BARCODE_USE
	return true;
}
//--------------------------------------------------------------------------------------------//
bool CHonBarcode::HoneywellSwiftDecoder_Read(int Key, void *Val)
{
#ifdef HON_BARCODE_USE
	if ( CheckSwiftDecoderLicense() == false ) { return false; }
	if ( CheckSwiftDecoderHandle() == false ) { return false; }	
	const int Ret=SD_Get(m_HandleHon, Key, Val);
	if ( 1 != Ret )
	{
		m_ErrorString.Format(_T("Error, Honeywell SwiftDecoder SD_Get[%d] failed, Error = %d\n"), Key, (int)SD_GetLastError());
		return false;
	}
#endif//HON_BARCODE_USE
	return true;
}
//--------------------------------------------------------------------------------------------//
bool CHonBarcode::HoneywellSwiftDecoder_Write(int Key, void *Val)
{
#ifdef HON_BARCODE_USE
	if ( CheckSwiftDecoderLicense() == false ) { return false; }
	if ( CheckSwiftDecoderHandle() == false ) { return false; }	
	const int Ret=SD_Set(m_HandleHon, Key, Val);
	if ( 1 != Ret )
	{
		m_ErrorString.Format(_T("Error, Honeywell SwiftDecoder SD_Set[%d] failed, Error = %d\n"), Key, (int)SD_GetLastError());
		return false;
	}	
#endif//HON_BARCODE_USE	
	return true;
}
//--------------------------------------------------------------------------------------------//
bool CHonBarcode::HoneywellSwiftDecoder_ReadResult()
{
#ifdef HON_BARCODE_USE	
	const size_t BufferSize=sizeof(m_HonBarcodeResult);
	int Symbology=0, SymbologyEx=0, Modifier=0, Length=0;		
	if ( HoneywellSwiftDecoder_Read(SD_PROP_RESULT_SYMBOLOGY, &Symbology) == false ) { return false; }
	if ( HoneywellSwiftDecoder_Read(SD_PROP_RESULT_SYMBOLOGY_EX, &SymbologyEx) == false ) { return false; }
	if ( HoneywellSwiftDecoder_Read(SD_PROP_RESULT_MODIFIER, &Modifier) == false ) { return false; }
	if ( HoneywellSwiftDecoder_Read(SD_PROP_RESULT_LENGTH, &Length) == false ) { return false; }
	if ( Length >= BufferSize )
	{	return false;	}
	SetHonBarcodeLength(Length);
	if ( HoneywellSwiftDecoder_Read(SD_PROP_RESULT_STRING, m_HonBarcodeResult) == false ) { return false; }		
	//Here are some more result properties that can be read back
	//#define SD_PROP_RESULT_LINKAGE               0x40007005
	//#define SD_PROP_RESULT_BOUNDS                0x70007001
	//#define SD_PROP_RESULT_CENTER                0x70007002
	//#define SD_PROP_RESULT_QRPOSITION            0x40007008
	//#define SD_PROP_RESULT_QRTOTAL               0x40007009
	//#define SD_PROP_RESULT_QRPARITY              0x40007010
	//#define SD_PROP_RESULT_QUALITY               0x40007011
	//#define SD_PROP_RESULT_QUALITY2              0x40007012
	
#endif//HON_BARCODE_USE	
	return true;
}
//--------------------------------------------------------------------------------------------//
CHonBarcode::CHonBarcode()
{	
	m_HandleHon = 0;
	m_bLicenseHon = false;
	m_HonCheckSum = false;
	m_HonBarcodeLength = 0;
	m_LicenseFilename = _T("Response.bin");
	::memset(m_HonBarcodeResult, 0x00, sizeof(m_HonBarcodeResult));
}
//-------------------------------------------------------------------------------------//
CHonBarcode::~CHonBarcode()
{	
	HoneywellSwiftDecoder_Destroy();
}
//-------------------------------------------------------------------------------------//
LPCTSTR CHonBarcode::GetErrorString()
{
	return m_ErrorString;
}
//--------------------------------------------------------------------------------------------//
bool CHonBarcode::EnableAllDecode(bool bEnabled)
{
#ifdef HON_BARCODE_USE
	if ( CheckSwiftDecoderLicense() == false ) { return false; }
	if ( CheckSwiftDecoderHandle() == false ) { return false; }
	std::vector<int> DecodeList;
	//DecodeList.push_back(SD_PROP_AZ_ENABLED);	
	DecodeList.push_back(SD_PROP_CB_ENABLED);
	DecodeList.push_back(SD_PROP_C128_ENABLED);
	DecodeList.push_back(SD_PROP_C39_ENABLED);
	//DecodeList.push_back(SD_PROP_TRIOPTIC_ENABLED);
	DecodeList.push_back(SD_PROP_DM_ENABLED);
	DecodeList.push_back(SD_PROP_I25_ENABLED);
	//DecodeList.push_back(SD_PROP_MC_ENABLED);
	//DecodeList.push_back(SD_PROP_PDF_ENABLED);
	//DecodeList.push_back(SD_PROP_MICROPDF_ENABLED);
	//DecodeList.push_back(SD_PROP_POSTAL_ENABLED);
	DecodeList.push_back(SD_PROP_QR_ENABLED);
	//DecodeList.push_back(SD_PROP_UPC_ENABLED);
	DecodeList.push_back(SD_PROP_C93_ENABLED);
	//DecodeList.push_back(SD_PROP_RSS_ENABLED);
	//DecodeList.push_back(SD_PROP_CC_ENABLED);
	//DecodeList.push_back(SD_PROP_S25_2SS_ENABLED);
	//DecodeList.push_back(SD_PROP_S25_3SS_ENABLED);
	//DecodeList.push_back(SD_PROP_MSIP_ENABLED);
	//DecodeList.push_back(SD_PROP_PHARMA_ENABLED);
	DecodeList.push_back(SD_PROP_C11_ENABLED);
	//DecodeList.push_back(SD_PROP_M25_ENABLED);
	//DecodeList.push_back(SD_PROP_TP_ENABLED);
	//DecodeList.push_back(SD_PROP_NEC25_ENABLED);
	//DecodeList.push_back(SD_PROP_OCR_ENABLED);
	//DecodeList.push_back(SD_PROP_VER_LINEAR_ENABLED);
	//DecodeList.push_back(SD_PROP_HK25_ENABLED);
	//DecodeList.push_back(SD_PROP_VER_POSTAL_ENABLED);
	//DecodeList.push_back(SD_PROP_DPM_ENABLED);
	//DecodeList.push_back(SD_PROP_VER_PDF_ENABLED);
	//DecodeList.push_back(SD_PROP_VER_2D_ENABLED);
	//DecodeList.push_back(SD_PROP_KP_ENABLED);
	//DecodeList.push_back(SD_PROP_HX_ENABLED);
	//DecodeList.push_back(SD_PROP_LC_ENABLED);
	//DecodeList.push_back(SD_PROP_GM_ENABLED);
	//DecodeList.push_back(SD_PROP_DOTCODE_ENABLED);
	//DecodeList.push_back(1234567890);
	const int nEnabled = (int)(bEnabled);
	const size_t Count=DecodeList.size();
	for ( size_t i=0; i<Count; i++ )
	{
		int Decode=DecodeList[i];
		if ( HoneywellSwiftDecoder_Write(Decode, INT_TO_VOID_POINTER(nEnabled)) == false ) { return false; }
	}	
#endif//HON_BARCODE_USE
	return true;
}
//--------------------------------------------------------------------------------------------//
bool CHonBarcode::ExecDecodeImage_Hon(int ImageW, int ImageH, int ImageStep, unsigned char *pImage, char* BarcodeText, size_t BarcodeTextLen)
{	
#ifdef HON_BARCODE_USE
	if ( CheckSwiftDecoderLicense() == false ) { return false; }
	if ( CheckSwiftDecoderHandle() == false ) { return false; }
	
	SetHonBarcodeLength(0);
	::memset(m_HonBarcodeResult, 0x00, sizeof(m_HonBarcodeResult));
	if ( HoneywellSwiftDecoder_Write(SD_PROP_CALLBACK_RESULT, (void *)CHonBarcode::SD_CB_Result) == false ) { return false; }
	if ( HoneywellSwiftDecoder_Write(SD_PROP_IMAGE_WIDTH, INT_TO_VOID_POINTER(ImageStep)) == false ) { return false; }
	if ( HoneywellSwiftDecoder_Write(SD_PROP_IMAGE_HEIGHT, INT_TO_VOID_POINTER(ImageH)) == false ) { return false; }
	if ( HoneywellSwiftDecoder_Write(SD_PROP_IMAGE_LINE_DELTA, INT_TO_VOID_POINTER(ImageStep)) == false ) { return false; }
	if ( HoneywellSwiftDecoder_Write(SD_PROP_IMAGE_POINTER, (void*)(pImage)) == false ) { return false; }	

	const int Ret=(int)SD_Decode(m_HandleHon);//會呼叫外部的SD_CB_Result函式
	if ( 0 == Ret )
	{	
		m_ErrorString = _T("Error, Honeywell SwiftDecoder Decode Fault");
		return false;
	}
	const size_t ResultLength=GetHonBarcodeLength();
	if ( 0 == ResultLength )	
	{	return true; }
	if ( ResultLength >= BarcodeTextLen )
	{
		const size_t CopyLen=BarcodeTextLen-1;
		::memcpy(BarcodeText, m_HonBarcodeResult, sizeof(char)*(CopyLen));
		BarcodeText[CopyLen] = '\0';
		return true; 
	}
	::strcpy(BarcodeText, m_HonBarcodeResult);
#endif//HON_BARCODE_USE
	return true;	
}
//--------------------------------------------------------------------------------------------//
bool CHonBarcode::ExecDecode1D_Hon(int ImageW, int ImageH, int ImageStep, unsigned char *pImage, char* BarcodeText, size_t BarcodeTextLen)//讀取
{	
#ifdef HON_BARCODE_USE
	if ( CheckSwiftDecoderLicense() == false ) { return false; }
	if ( CheckSwiftDecoderHandle() == false ) { return false; }
	if ( EnableAllDecode(false) == false ) { return false; }
	//Switch 1-D Barcode Decoder
	int nCheckSum = 1;
	const int nEnabled = 1;				
	if ( true == m_HonCheckSum ) { nCheckSum = 1; }
	else { nCheckSum = 0; }
	if ( HoneywellSwiftDecoder_Write(SD_PROP_CB_ENABLED, INT_TO_VOID_POINTER(nEnabled)) == false ) { return false; }//Code-Bar
	if ( HoneywellSwiftDecoder_Write(SD_PROP_CB_CHECKSUM, INT_TO_VOID_POINTER(nCheckSum)) == false ) { return false; }//Code-Bar-Check Sum

	if ( HoneywellSwiftDecoder_Write(SD_PROP_C128_ENABLED, INT_TO_VOID_POINTER(nEnabled)) == false ) { return false; }//Code-128

	if ( HoneywellSwiftDecoder_Write(SD_PROP_C39_ENABLED, INT_TO_VOID_POINTER(nEnabled)) == false ) { return false; }//Code-39
	if ( HoneywellSwiftDecoder_Write(SD_PROP_C39_CHECKSUM, INT_TO_VOID_POINTER(nCheckSum)) == false ) { return false; }//Code-39-Check Sum	

	if ( HoneywellSwiftDecoder_Write(SD_PROP_I25_ENABLED, INT_TO_VOID_POINTER(nEnabled)) == false ) { return false; }//I 2 of 5 enabled	
	if ( HoneywellSwiftDecoder_Write(SD_PROP_I25_CHECKSUM, INT_TO_VOID_POINTER(nCheckSum)) == false ) { return false; }//I 2 of 5-Check Sum

	if ( HoneywellSwiftDecoder_Write(SD_PROP_C11_ENABLED, INT_TO_VOID_POINTER(nEnabled)) == false ) { return false; }//Code-11 enabled	
	if ( HoneywellSwiftDecoder_Write(SD_PROP_C11_CHECKSUM, INT_TO_VOID_POINTER(nCheckSum)) == false ) { return false; }//Code-11-Check Sum

	//if ( HoneywellSwiftDecoder_Write(SD_PROP_M25_ENABLED, INT_TO_VOID_POINTER(nEnabled)) == false ) { return false; }//M25 enabled	
	//if ( HoneywellSwiftDecoder_Write(SD_PROP_M25_CHECKSUM, INT_TO_VOID_POINTER(nCheckSum)) == false ) { return false; }//M25-Check Sum	

	//if ( HoneywellSwiftDecoder_Write(SD_PROP_NEC25_ENABLED, INT_TO_VOID_POINTER(nEnabled)) == false ) { return false; }//NEC25 enabled	
	//if ( HoneywellSwiftDecoder_Write(SD_PROP_NEC25_CHECKSUM, INT_TO_VOID_POINTER(nCheckSum)) == false ) { return false; }//NEC25-Check Sum	

	//if ( HoneywellSwiftDecoder_Write(SD_PROP_IMAGE_WIDTH, INT_TO_VOID_POINTER(ImageW)) == false ) { return false; }
	//if ( HoneywellSwiftDecoder_Write(SD_PROP_IMAGE_HEIGHT, INT_TO_VOID_POINTER(ImageH)) == false ) { return false; }
	//if ( HoneywellSwiftDecoder_Write(SD_PROP_IMAGE_LINE_DELTA, INT_TO_VOID_POINTER(ImageStep)) == false ) { return false; }
	//if ( HoneywellSwiftDecoder_Write(SD_PROP_IMAGE_POINTER, (void*)(pImage)) == false ) { return false; }

	if ( ExecDecodeImage_Hon(ImageW, ImageH, ImageStep, pImage, BarcodeText, BarcodeTextLen) == false )
	{
		m_ErrorString = _T("Error, Honeywell SwiftDecoder Decode 1D-Barcode Fault");
		return false;
	}	   
#endif//HON_BARCODE_USE
	return true;
}
//--------------------------------------------------------------------------------------------//
bool CHonBarcode::ExecDecodeQRCode_Hon(int ImageW, int ImageH, int ImageStep, unsigned char *pImage, char* BarcodeText, size_t BarcodeTextLen)
{	
#ifdef HON_BARCODE_USE
	if ( CheckSwiftDecoderLicense() == false ) { return false; }
	if ( CheckSwiftDecoderHandle() == false ) { return false; }
	if ( EnableAllDecode(false) == false ) { return false; }
	//Switch QRCode Decoder	
	const int nEnabled = 1;
	if ( HoneywellSwiftDecoder_Write(SD_PROP_QR_ENABLED, INT_TO_VOID_POINTER(nEnabled)) == false ) { return false; }//QRCode	

	if ( ExecDecodeImage_Hon(ImageW, ImageH, ImageStep, pImage, BarcodeText, BarcodeTextLen) == false )
	{
		m_ErrorString = _T("Error, Honeywell SwiftDecoder Decode QRCode Fault");
		return false;
	}
#endif//HON_BARCODE_USE
	return true;
}
//--------------------------------------------------------------------------------------------//
bool CHonBarcode::ExecDecodeDataMatrix_Hon(int ImageW, int ImageH, int ImageStep, unsigned char *pImage, char* BarcodeText, size_t BarcodeTextLen)
{
#ifdef HON_BARCODE_USE
	if ( CheckSwiftDecoderLicense() == false ) { return false; }
	if ( CheckSwiftDecoderHandle() == false ) { return false; }
	if ( EnableAllDecode(false) == false ) { return false; }
	//Switch DataMatrix Decoder	
	const int nEnabled = 1;
	if ( HoneywellSwiftDecoder_Write(SD_PROP_DM_ENABLED, INT_TO_VOID_POINTER(nEnabled)) == false ) { return false; }//DataMatrix

	if ( ExecDecodeImage_Hon(ImageW, ImageH, ImageStep, pImage, BarcodeText, BarcodeTextLen) == false )
	{
		m_ErrorString = _T("Error, Honeywell SwiftDecoder Decode DataMatrix Fault");
		return false;
	}
#endif//HON_BARCODE_USE
	return true;	
}
//--------------------------------------------------------------------------------------------//
