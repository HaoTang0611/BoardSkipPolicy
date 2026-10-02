// DtkBarcode.cpp: implementation of the CDtkBarcode class.
//
//////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "DtkBarcode.h"
//--------------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//--------------------------------------------------------------------------------------------//
unsigned long CDtkBarcode::m_nRef=0;//實體數量
unsigned long CDtkBarcode::m_nUniqueIDCount=0;//唯一碼數量
#ifdef DTK_BARCODE_CRITICAL_SECTION_USE
CRITICAL_SECTION CDtkBarcode::m_csDTK;//同步機制-關鍵區間
#endif//DTK_BARCODE_CRITICAL_SECTION_USE

CString CDtkBarcode::m_DTKVersion;
int CDtkBarcode::m_DTKScanInterval = 4;
int CDtkBarcode::m_DTKQuietZoneSize = QZ_Small;
int CDtkBarcode::m_DTKThresholdMode = 3;
int CDtkBarcode::m_DTKThresholdValue = 128;
int CDtkBarcode::m_DTKThresholdCount = 32;
int CDtkBarcode::m_DTKThresholdStep = 8;
//--------------------------------------------------------------------------------------------//
HMODULE hDTKBarcodeModule=NULL;
typedef HRESULT (STDAPICALLTYPE* DTK_CreateBarcodeReader)(IBarcodeReader**);
typedef HRESULT (STDAPICALLTYPE* DTK_DestroyBarcodeReader)(IBarcodeReader*);

DTK_CreateBarcodeReader  DTK_fCreateBarcodeReader=NULL;
DTK_DestroyBarcodeReader DTK_fDestroyBarcodeReader=NULL;
//--------------------------------------------------------------------------------------------//
//CDtkBarcode JetBarcode_DTK;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------------//
void CDtkBarcode::InitialDTKBarcodeLock()//初始化DTK條碼關鍵區間
{
#ifdef DTK_BARCODE_CRITICAL_SECTION_USE
	::InitializeCriticalSection(&m_csDTK);
#endif//DTK_BARCODE_CRITICAL_SECTION_USE
}
//--------------------------------------------------------------------------------------------//
void CDtkBarcode::DeleteDTKBarcodeLock()//刪除DTK條碼關鍵區間
{
#ifdef DTK_BARCODE_CRITICAL_SECTION_USE
	::DeleteCriticalSection(&m_csDTK);
#endif//DTK_BARCODE_CRITICAL_SECTION_USE
}
//--------------------------------------------------------------------------------------------//
void CDtkBarcode::LockDTKBarcode()//進入DTK條碼關鍵區間
{
#ifdef DTK_BARCODE_CRITICAL_SECTION_USE
	::EnterCriticalSection(&m_csDTK);	
#endif//DTK_BARCODE_CRITICAL_SECTION_USE
}
//--------------------------------------------------------------------------------------------//
void CDtkBarcode::UnlockDTKBarcode()//離開DTK條碼關鍵區間
{
#ifdef DTK_BARCODE_CRITICAL_SECTION_USE
	::LeaveCriticalSection(&m_csDTK);	
#endif//DTK_BARCODE_CRITICAL_SECTION_USE
}
//--------------------------------------------------------------------------------------------//
bool CDtkBarcode::LoadDTKLibrary(CString &DTKErr)
{	
	//CString DTKErr = _T("");
	CString LibFile = _T("");
	CString strFunName = _T("");
	char    FunName[MAX_JET_PATH] = "";
	if ( 0 != CDtkBarcode::m_nRef )
	{	JetAPI::ShowMessageBox(_T("Error, DTK Barcode Not Release All"));	}

#ifdef DTK_BARCODE_USE
	#ifdef JET_DONGLE_USE
		char ERR[1024]="";
		R4_Lib R4;
		int code=R4.R4GetFunction(JET_DONGLE_DTK,ERR);
		if ( code != 0 ) 
		{
			m_DTKErr.Format("Cannot Get JET Dongle (%s)", ERR);		
			return false; 
		}
	#endif//JET_DONGLE_USE
	
#ifndef _X64
	#ifdef _DEBUG
		LibFile.Format(_T("%s\\Runtime_x32\\%s"), AOIDataCollect.GetAOIDirectory(), _T("DTKBarReader.dll"));
	#else
		LibFile.Format(_T("%s\\Runtime_x32\\%s"), AOIDataCollect.GetAOIDirectory(), _T("DTKBarReader_x86.dll"));		
	#endif
#else
	LibFile.Format(_T("%s\\Runtime_x64\\%s"), AOIDataCollect.GetAOIDirectory(), _T("DTKBarReader_x64.dll"));		
#endif
	
	hDTKBarcodeModule = LoadLibrary(LibFile); 
	if ( hDTKBarcodeModule==NULL )	
	{
		DTKErr.Format(_T("Cannot load %s"), LibFile);		
		return false;
	}

	strcpy(FunName, ("CreateBarcodeReader"));
	DTK_fCreateBarcodeReader = (DTK_CreateBarcodeReader)GetProcAddress(hDTKBarcodeModule, FunName);
	if ( DTK_fCreateBarcodeReader == NULL )
	{
		strFunName = FunName;
		DTKErr.Format(_T("Cannot load DTK function%s"), strFunName);		
		return false;
	}

	strcpy(FunName, ("DestroyBarcodeReader"));	
	DTK_fDestroyBarcodeReader = (DTK_DestroyBarcodeReader)GetProcAddress(hDTKBarcodeModule, FunName); 
	if ( DTK_fDestroyBarcodeReader == NULL )
	{
		strFunName = FunName;
		DTKErr.Format(_T("Cannot load DTK function%s"), strFunName);		
		return false;
	}

	IBarcodeReader *DTKReader=NULL;
	if (DTK_fCreateBarcodeReader(&DTKReader) != 0)
	{
		DTKErr = _T("Cannot Create DTK Barcode Reader");	
		return false;
	}
	
	ILicManager *licenseManager=NULL;
	DTKReader->get_LicenseManager(&licenseManager);
	if ( licenseManager == NULL )
	{
		DTK_fDestroyBarcodeReader(DTKReader);
		CDtkBarcode::FreeDTKLibrary();
		DTKErr = _T("Cannot get DTK get_LicenseManager");		
		return false;
	}

	HRESULT hr = NULL;
	hr = licenseManager->AddLicenseKey(L"PBLVZJUXJQFJHY1UG8CA");
	if ( FAILED(hr) ) 
	{
		DTK_fDestroyBarcodeReader(DTKReader);
		CDtkBarcode::FreeDTKLibrary();
		DTKErr = _T("Cannot add DTK AddLicenseKey - 1");		
		return false;
	}
	hr = licenseManager->AddLicenseKey(L"EK2DXDCVJRVB5US9C6R7");
	if ( FAILED(hr) ) 
	{
		DTK_fDestroyBarcodeReader(DTKReader);
		CDtkBarcode::FreeDTKLibrary();
		DTKErr = _T("Cannot add DTK AddLicenseKey - 2");		
		return false;
	}

#ifndef _DEBUG	
	BSTR bstrVersion = NULL;
	DTKReader->get_Version(&bstrVersion);
	CString strVersion = bstrVersion;
	SysFreeString(bstrVersion);	
	m_DTKVersion.Format(_T("DTK-v%s"), strVersion);
#else
	m_DTKVersion.Format(_T("DTK-v%s"), _T("Debug"));
#endif//_DEBUG	

	DTK_fDestroyBarcodeReader(DTKReader);
#endif//DTK_BARCODE_USE
	return true;
}
//--------------------------------------------------------------------------------------------//
bool CDtkBarcode::FreeDTKLibrary()
{
#ifdef DTK_BARCODE_USE
	if ( 0 != CDtkBarcode::m_nRef )
	{	JetAPI::ShowMessageBox(_T("Error, DTK Barcode Not Release All"));	}

	CDtkBarcode::LockDTKBarcode();
	m_DTKVersion = _T("");
	DTK_fCreateBarcodeReader  = NULL;		
	DTK_fDestroyBarcodeReader = NULL;
	if ( hDTKBarcodeModule != NULL )
	{
		::FreeLibrary(hDTKBarcodeModule);
		hDTKBarcodeModule = NULL;
	}	
	CDtkBarcode::UnlockDTKBarcode();
#endif//DTK_BARCODE_USE
	return true;
}
//--------------------------------------------------------------------------------------------//
bool CDtkBarcode::GetDTKLibraryVersion(CString &str)//取得DTK條碼函式庫版本
{
	str = m_DTKVersion;	
	return true;
}
//--------------------------------------------------------------------------------------------//
bool CDtkBarcode::SaveINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pString, LPCTSTR pfilename, CString &Error)
{
	if ( JetAPI::SaveINIData(pSection, pKeyName, pString, pfilename, Error) == false ) 
	{	return false;	}	
	return true;
}
//--------------------------------------------------------------------------------------------//
bool CDtkBarcode::LoadINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pDefault, TCHAR *pString, int StringSize, LPCTSTR pfilename, bool IsCheckLens, CString &Error)	
{
	if ( JetAPI::LoadINIData(pSection, pKeyName, pDefault, pString, StringSize, pfilename, IsCheckLens, Error) == false )
	{	return false; }
	return true;
}
//--------------------------------------------------------------------------------------------//
bool CDtkBarcode::SaveDTKIniFile()//儲存DTK參數檔案
{
	CString DTKErr;
	CString filename;
	CString Section;
	CString KeyName;
	CString String;

	filename.Format(_T("%s\\%s"), AOIDataCollect.GetAOIDirectory(), _T("BarcodeDecoder.INI"));
	Section = _T("DTK Decoder");
	
	KeyName = _T("Scan Interval");
	String.Format(_T("%d"), m_DTKScanInterval);
	SaveINIData(Section, KeyName, String, filename, DTKErr);

	KeyName = _T("Quiet Zone Size");
	String.Format(_T("%d"), m_DTKQuietZoneSize);
	SaveINIData(Section, KeyName, String, filename, DTKErr);

	KeyName = _T("Threashold Mode");
	String.Format(_T("%d"), m_DTKThresholdMode);
	SaveINIData(Section, KeyName, String, filename, DTKErr);

	KeyName = _T("Threashold Value");
	String.Format(_T("%d"), m_DTKThresholdValue);
	SaveINIData(Section, KeyName, String, filename, DTKErr);

	KeyName = _T("Threashold Count");
	String.Format(_T("%d"), m_DTKThresholdCount);
	SaveINIData(Section, KeyName, String, filename, DTKErr);

	KeyName = _T("Threashold Step");
	String.Format(_T("%d"), m_DTKThresholdStep);
	SaveINIData(Section, KeyName, String, filename, DTKErr);
	return true;
}
//--------------------------------------------------------------------------------------------//
bool CDtkBarcode::LoadDTKIniFile()//載入DTK參數檔案
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
	Section = _T("DTK Decoder");

	KeyName = _T("Scan Interval");
	Default.Format(_T("%d"), m_DTKScanInterval);
	if ( LoadINIData(Section, KeyName, Default, String, StringSize, filename, false, DTKErr) == false )
	{	return false; }
	m_DTKScanInterval = ::_ttoi(String);
	if ( m_DTKScanInterval < 0 ) { m_DTKScanInterval = 0; }
	if ( m_DTKScanInterval > 20 ) { m_DTKScanInterval = 20; }

	KeyName = _T("Quiet Zone Size");
	Default.Format(_T("%d"), m_DTKQuietZoneSize);
	if ( LoadINIData(Section, KeyName, Default, String, StringSize, filename, false, DTKErr) == false )
	{	return false; }
	nVal = ::_ttoi(String);
	switch ( nVal )
	{
	case QZ_Large:
	case QZ_Normal:
	case QZ_ExtraSmall:
		m_DTKQuietZoneSize=nVal;
		break;
	default:
	case QZ_Small:
		m_DTKQuietZoneSize=QZ_Small;
		break;
	}

	KeyName = _T("Threashold Mode");
	Default.Format(_T("%d"), m_DTKThresholdMode);
	if ( LoadINIData(Section, KeyName, Default, String, StringSize, filename, false, DTKErr) == false )
	{	return false; }
	m_DTKThresholdMode = ::_ttoi(String);

	KeyName = _T("Threashold Value");
	Default.Format(_T("%d"), m_DTKThresholdValue);
	if ( LoadINIData(Section, KeyName, Default, String, StringSize, filename, false, DTKErr) == false )
	{	return false; }
	m_DTKThresholdValue = ::_ttoi(String);

	KeyName = _T("Threashold Count");
	Default.Format(_T("%d"), m_DTKThresholdCount);
	if ( LoadINIData(Section, KeyName, Default, String, StringSize, filename, false, DTKErr) == false )
	{	return false; }
	m_DTKThresholdCount = ::_ttoi(String);

	KeyName = _T("Threashold Step");
	Default.Format(_T("%d"), m_DTKThresholdStep);
	if ( LoadINIData(Section, KeyName, Default, String, StringSize, filename, false, DTKErr) == false )
	{	return false; }
	m_DTKThresholdStep = ::_ttoi(String);

	return true;
}
//--------------------------------------------------------------------------------------------//
CDtkBarcode::CDtkBarcode()
{
	::InterlockedIncrement(&m_nRef);
	m_UniqueID = ::InterlockedIncrement(&m_nUniqueIDCount);	
	this->m_DTKReader = NULL;
	this->m_DTKCheckSum = false;
	this->m_DTKBarcodeDir = 0xFF;

	this->m_DTKImageW = 0;
	this->m_DTKImageH = 0;
	this->m_DTKImagePtr = NULL;
	
	m_DTKRecognitionTimeout = 0;
	//this->LoadDTKLibrary();	
	CreateDtkBarcodeReader(m_DTKReader);//2.5~3.1 ms
}
//--------------------------------------------------------------------------------------------//
CDtkBarcode::~CDtkBarcode()
{
	::InterlockedDecrement(&m_nRef);
	DestroyDtkBarcodeReader(m_DTKReader);
	//this->FreeDTKLibrary();	
}
//--------------------------------------------------------------------------------------------//
bool CDtkBarcode::CheckDtkBarcodeLibraryFunc()
{
	if ( NULL == DTK_fCreateBarcodeReader )
	{
		m_DTKErr=_T("Error, DTK_fCreateBarcodeReader is NULL");
		return false; 
	}
	if ( NULL == DTK_fDestroyBarcodeReader )
	{
		m_DTKErr=_T("Error, DTK_fDestroyBarcodeReader is NULL");
		return false; 
	}
	return true;
}
//--------------------------------------------------------------------------------------------//
bool CDtkBarcode::CheckDtkBarcodeReader()//確認條碼初始化
{
	if ( NULL == m_DTKReader )
	{
		this->m_DTKErr = _T("Error, DTK Barcode Reader not Initialized");
		return false; 
	}
	return true;
}
//--------------------------------------------------------------------------------------------//
bool CDtkBarcode::GetDTKErrorMSG(CString &str)
{
//	CDtkBarcode::LockDTKBarcode();
	if (m_DTKReader != NULL)
	{
		BSTR errorText;
		m_DTKReader->GetLastErrorText(&errorText);	
		str = errorText;
		SysFreeString(errorText);
	}
//	CDtkBarcode::UnlockDTKBarcode();
	return true;
}
//--------------------------------------------------------------------------------------------//
bool CDtkBarcode::CreateDtkBarcodeReader(IBarcodeReader *&Reader)
{
#ifdef DTK_BARCODE_USE
	CString DTKErr;
	if ( CheckDtkBarcodeLibraryFunc() == false )
	{	return false; }
	if (DTK_fCreateBarcodeReader(&Reader) != 0)
	{
		m_DTKErr = _T("Cannot Create DTK Barcode Reader");		
		return false;	
	}
#endif//DTK_BARCODE_USE
	return true;
}
//--------------------------------------------------------------------------------------------//
bool CDtkBarcode::DestroyDtkBarcodeReader(IBarcodeReader *&Reader)
{
#ifdef DTK_BARCODE_USE
	CString DTKErr;
	if ( NULL == Reader ) { return true; }
	if ( CheckDtkBarcodeLibraryFunc() == false )
	{	return false; }	
	DTK_fDestroyBarcodeReader(Reader);	Reader = NULL;	
#endif//DTK_BARCODE_USE
	return true;
}
//--------------------------------------------------------------------------------------------//
LPCTSTR CDtkBarcode::GetErrorString()
{
	return this->m_DTKErr;
}
//--------------------------------------------------------------------------------------------//
unsigned long CDtkBarcode::GetDtkBarcodeUniqueID() const
{	
	return m_UniqueID;
}
//--------------------------------------------------------------------------------------------//
void CDtkBarcode::SetLoadImage_DTK(LPCTSTR pfilename)
{
	this->m_DTKImageFileName = pfilename;
}
//--------------------------------------------------------------------------------------------//
CString CDtkBarcode::BuildLoadImage_DTK(LPCTSTR Folder, LPCTSTR Name) const
{
	CString str;
	const unsigned long nID=(m_UniqueID%1024)+1;
	str.Format(_T("%s\\%s[%04u].PNG"), Folder, Name, nID);
	return str;
}
//--------------------------------------------------------------------------------------------//
bool CDtkBarcode::ExecDecode1D_DTK(char *Buffer, size_t BufferSize)
{
	if ( CheckDtkBarcodeReader() == false ) 
	{	return false;	}

	CString str;
	HRESULT hr = NULL;
	BarcodeTypeEnum types = BT_Unknown;
	BarcodeOrientationEnum orientation = BO_Unknown;

	types = BT_Unknown;
	//types = (BarcodeTypeEnum)(types | BT_Code11);
	types = (BarcodeTypeEnum)(types | BT_Code39);
	types = (BarcodeTypeEnum)(types | BT_Code39Extended);
	types = (BarcodeTypeEnum)(types | BT_Code93);
	types = (BarcodeTypeEnum)(types | BT_Code128);
	types = (BarcodeTypeEnum)(types | BT_Codabar);
	types = (BarcodeTypeEnum)(types | BT_Inter2of5);
	//types = (BarcodeTypeEnum)(types | BT_PatchCode);
	types = (BarcodeTypeEnum)(types | BT_EAN13);
	types = (BarcodeTypeEnum)(types | BT_EAN8);
	types = (BarcodeTypeEnum)(types | BT_UPCE);
	types = (BarcodeTypeEnum)(types | BT_UPCA);
	//types = (BarcodeTypeEnum)(types | BT_Plus2);
	//types = (BarcodeTypeEnum)(types | BT_Plus5);

	this->m_DTKReader->put_BarcodeTypes(types);

	// Barcode Orientaion
	orientation = BO_Unknown;
	if ( (m_DTKBarcodeDir&BO_TopToBottom) != 0 ) { orientation = (BarcodeOrientationEnum)(orientation | BO_TopToBottom); }
	if ( (m_DTKBarcodeDir&BO_BottomToTop) != 0 ) { orientation = (BarcodeOrientationEnum)(orientation | BO_BottomToTop); }
	if ( (m_DTKBarcodeDir&BO_LeftToRight) != 0 ) { orientation = (BarcodeOrientationEnum)(orientation | BO_LeftToRight); }
	if ( (m_DTKBarcodeDir&BO_RightToLeft) != 0 ) { orientation = (BarcodeOrientationEnum)(orientation | BO_RightToLeft); }
	this->m_DTKReader->put_BarcodeOrientation(orientation);
	
	// Optional checksum
	this->m_DTKReader->put_Code11Checksum(m_DTKCheckSum);
	this->m_DTKReader->put_Code39Checksum(m_DTKCheckSum);
	this->m_DTKReader->put_Code93Checksum(m_DTKCheckSum);
	this->m_DTKReader->put_I2of5Checksum(m_DTKCheckSum);
	
	// Settings
	this->m_DTKReader->put_BarcodesToRead(1);//期許多少個條碼數
	this->m_DTKReader->put_ScanInterval(m_DTKScanInterval);
	this->m_DTKReader->put_ScanPage(0);	
	
	this->m_DTKReader->put_QuietZoneSize((QuietZoneSizeEnum)(m_DTKQuietZoneSize));
	this->m_DTKReader->put_PDFReadingType(PDF_Images);//PDF_Images, PDF_Render	
	
	//this->m_DTKReader->put_ThresholdMode(TM_Automatic);//TM_Automatic, TM_Fixed, TM_Multiple	
	switch ( m_DTKThresholdMode )
	{
	case 0x01:	this->m_DTKReader->put_ThresholdMode(TM_Automatic);	break;
	case 0x02:	this->m_DTKReader->put_ThresholdMode(TM_Fixed);		break;
	case 0x03:	this->m_DTKReader->put_ThresholdMode(TM_Multiple);	break;
	case 0x04:	this->m_DTKReader->put_ThresholdMode(TM_Adaptive);	break;
	default:	this->m_DTKReader->put_ThresholdMode(TM_Automatic);	break;
	}	
	this->m_DTKReader->put_Threshold(m_DTKThresholdValue);
	this->m_DTKReader->put_ThresholdCount(m_DTKThresholdCount);
	this->m_DTKReader->put_ThresholdStep(m_DTKThresholdStep);	
#ifndef _DEBUG
	this->m_DTKReader->put_RecognitionTimeout(m_DTKRecognitionTimeout);	
#endif//_DEBUG

	// Preprocessing
	this->m_DTKReader->put_ImageInvert(0);
	this->m_DTKReader->put_ImageDespeckle(0);
	this->m_DTKReader->put_ImageDilate(0);
	this->m_DTKReader->put_ImageErode(0);
	this->m_DTKReader->put_ImageSharp(0);	

	int method = 1;
	mouse_event(MOUSEEVENTF_MOVE, 0, 0, 0, 0 );//avoid DTK change to ServerMode
	/*

		m_grabberPtr->execute<Euresys::RemoteModule>("AcquisitionStop");
		m_grabberPtr->stop();
	
	*/
try
{
	if  ( this->m_DTKImagePtr == NULL ) // read barcodes directly from file
	{
		WCHAR fileName[MAX_JET_PATH];
	#ifdef UNICODE
		::wcscpy(fileName, m_DTKImageFileName);		
	#else		
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, m_DTKImageFileName, -1, fileName, MAX_JET_PATH);		
	#endif
		hr = this->m_DTKReader->ReadFromFile(fileName);
		if ( 0==m_DTKRecognitionTimeout && FAILED(hr) ) 
		{
			this->GetDTKErrorMSG(this->m_DTKErr);
			return false;
		}
	}
	else
	{
		long buffer = (long)m_DTKImagePtr;
		hr = this->m_DTKReader->ReadFromBuffer(buffer, m_DTKImageW, m_DTKImageH, m_DTKImageWAligned, 8);
		if ( FAILED(hr) ) 
		{
			this->GetDTKErrorMSG(this->m_DTKErr);
			return false;
		}
	}
	IBarcodeCollection *barcodes=NULL;
	hr = this->m_DTKReader->get_Barcodes(&barcodes);
	if ( FAILED(hr) ) 
	{
		this->GetDTKErrorMSG(this->m_DTKErr);
		return false;
	}

	LONG barcodesCount;
	hr = barcodes->get_Count(&barcodesCount);
	if ( FAILED(hr) ) 
	{
		this->GetDTKErrorMSG(this->m_DTKErr);
		return false;
	}
	if ( 0 == barcodesCount )
	{
		::strcpy(Buffer, "");
		barcodes->Release();
		return true;
	}

	int BarcodeID = 0;
	IBarcode *bar=NULL;
	barcodes->get_Item(BarcodeID, &bar);

	// Barcode string
	BSTR barcodeString;
	bar->get_BarcodeString(&barcodeString);
	str = barcodeString;
	const int len = str.GetLength();		
	JetAPI::TCHAR2char(str, Buffer, BufferSize);

	SysFreeString(barcodeString);
	bar->Release();		
	barcodes->Release();
	return true;
}
catch(const std::exception &e)
{	
	m_DTKErr = e.what();	
	return false;
}
	return true;
}
//--------------------------------------------------------------------------------------------//
bool CDtkBarcode::ExecDecodeQRCode_DTK(char *Buffer, size_t BufferSize)
{
	if ( CheckDtkBarcodeReader() == false ) 
	{	return false;	}	
	BarcodeTypeEnum types = BT_Unknown;	

	types = BT_Unknown;
//	types = (BarcodeTypeEnum)(types | BT_PDF417);
//	types = (BarcodeTypeEnum)(types | BT_DataMatrix);
	types = (BarcodeTypeEnum)(types | BT_QRCode);
	types = (BarcodeTypeEnum)(types | BT_MicroQRCode);

	this->m_DTKReader->put_BarcodeTypes(types);

	if ( ExecDecode2D_DTK(Buffer, BufferSize) == false )
	{	return false; }
	return true;
}
//--------------------------------------------------------------------------------------------//
bool CDtkBarcode::ExecDecodeDataMatrix_DTK(char *Buffer, size_t BufferSize)
{
	if ( CheckDtkBarcodeReader() == false ) 
	{	return false;	}	
	BarcodeTypeEnum types = BT_Unknown;	

	types = BT_Unknown;
//	types = (BarcodeTypeEnum)(types | BT_PDF417);
	types = (BarcodeTypeEnum)(types | BT_DataMatrix);
//	types = (BarcodeTypeEnum)(types | BT_QRCode);
//	types = (BarcodeTypeEnum)(types | BT_MicroQRCode);

	this->m_DTKReader->put_BarcodeTypes(types);

	if ( ExecDecode2D_DTK(Buffer, BufferSize) == false )
	{	return false; }
	return true;	
}
//--------------------------------------------------------------------------------------------//
bool CDtkBarcode::ExecDecode2D_DTK(char *Buffer, size_t BufferSize)
{
	CString str;
	HRESULT hr = NULL;	
	BarcodeOrientationEnum orientation = BO_Unknown;

	// Barcode Orientaion
	orientation = BO_Unknown;
	orientation = (BarcodeOrientationEnum)(orientation | BO_TopToBottom);
	orientation = (BarcodeOrientationEnum)(orientation | BO_BottomToTop);
	orientation = (BarcodeOrientationEnum)(orientation | BO_LeftToRight);
	orientation = (BarcodeOrientationEnum)(orientation | BO_RightToLeft);	
	this->m_DTKReader->put_BarcodeOrientation(orientation);
	
	// Optional checksum
	this->m_DTKReader->put_Code11Checksum(m_DTKCheckSum);
	this->m_DTKReader->put_Code39Checksum(m_DTKCheckSum);
	this->m_DTKReader->put_Code93Checksum(m_DTKCheckSum);
	this->m_DTKReader->put_I2of5Checksum(m_DTKCheckSum);
	
	// Settings
	this->m_DTKReader->put_BarcodesToRead(1);//期許多少個條碼數
	this->m_DTKReader->put_ScanInterval(1);
	this->m_DTKReader->put_ScanPage(0);
	this->m_DTKReader->put_Threshold(128);
	
	this->m_DTKReader->put_QuietZoneSize(QZ_Normal);//QZ_Large, QZ_Normal, QZ_Small, QZ_ExtraSmall, QZ_Small(省時間)
	this->m_DTKReader->put_PDFReadingType(PDF_Images);//PDF_Images, PDF_Render	

	//this->m_DTKReader->put_ThresholdMode(TM_Automatic);//TM_Automatic, TM_Fixed, TM_Multiple
	switch ( m_DTKThresholdMode )
	{
	case 0x01:	this->m_DTKReader->put_ThresholdMode(TM_Automatic);	break;
	case 0x02:	this->m_DTKReader->put_ThresholdMode(TM_Fixed);		break;
	case 0x03:	this->m_DTKReader->put_ThresholdMode(TM_Multiple);	break;
	case 0x04:	this->m_DTKReader->put_ThresholdMode(TM_Adaptive);	break;
	default:	this->m_DTKReader->put_ThresholdMode(TM_Automatic);	break;
	}	
	this->m_DTKReader->put_Threshold(m_DTKThresholdValue);
	this->m_DTKReader->put_ThresholdCount(m_DTKThresholdCount);
	this->m_DTKReader->put_ThresholdStep(m_DTKThresholdStep);	
#ifndef _DEBUG
	this->m_DTKReader->put_RecognitionTimeout(m_DTKRecognitionTimeout);	
#endif//_DEBUG

	// Preprocessing
	this->m_DTKReader->put_ImageInvert(0);
	this->m_DTKReader->put_ImageDespeckle(0);
	this->m_DTKReader->put_ImageDilate(0);
	this->m_DTKReader->put_ImageErode(0);
	this->m_DTKReader->put_ImageSharp(0);	

	int method = 1;	
	mouse_event(MOUSEEVENTF_MOVE, 0, 0, 0, 0 );//avoid DTK change to ServerMode
try
{
	if  ( this->m_DTKImagePtr == NULL ) // read barcodes directly from file
	{
		WCHAR fileName[MAX_JET_PATH];
	#ifdef UNICODE
		::wcscpy(fileName, m_DTKImageFileName);		
	#else		
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, m_DTKImageFileName, -1, fileName, MAX_JET_PATH);		
	#endif		
		hr = this->m_DTKReader->ReadFromFile(fileName);
		if ( 0==m_DTKRecognitionTimeout && FAILED(hr) ) 
		{
			this->GetDTKErrorMSG(this->m_DTKErr);
			return false;
		}
	}
	else
	{
		long buffer = (long)m_DTKImagePtr;
		hr = this->m_DTKReader->ReadFromBuffer(buffer, m_DTKImageW, m_DTKImageH, m_DTKImageWAligned, 8);
		if ( FAILED(hr) ) 
		{
			this->GetDTKErrorMSG(this->m_DTKErr);
			return false;
		}
	}
	
	IBarcodeCollection *barcodes=NULL;
	hr = this->m_DTKReader->get_Barcodes(&barcodes);
	if ( FAILED(hr) ) 
	{
		this->GetDTKErrorMSG(this->m_DTKErr);
		return false;
	}

	LONG barcodesCount;
	hr = barcodes->get_Count(&barcodesCount);
	if ( FAILED(hr) ) 
	{
		this->GetDTKErrorMSG(this->m_DTKErr);
		return false;
	}
	if ( 0 == barcodesCount )
	{	
		::strcpy(Buffer, "");
		barcodes->Release();
		return true;		
	}

	int BarcodeID = 0;
	IBarcode *bar=NULL;
	barcodes->get_Item(BarcodeID, &bar);

	// Barcode string
	BSTR barcodeString;
	bar->get_BarcodeString(&barcodeString);

	str = barcodeString;
	const int len = str.GetLength();		
	JetAPI::TCHAR2char(str, Buffer, BufferSize);

	SysFreeString(barcodeString);
	bar->Release();	
	barcodes->Release();	
}
catch(const std::exception &e)
{	
	m_DTKErr = e.what();	
	return false;
}	
	return true;	
}
//--------------------------------------------------------------------------------------------//
void CDtkBarcode::SetImageBits_DTK(int ImageW, int ImageH, const unsigned char *pImage, int ImageW2)
{
	this->m_DTKImageW = ImageW;
	this->m_DTKImageH = ImageH;
	this->m_DTKImageWAligned = ImageW2;
	this->m_DTKImagePtr = (unsigned char*)pImage;
}
//--------------------------------------------------------------------------------------------//
void CDtkBarcode::FreeImageBits_DTK()
{
	this->m_DTKImageW = 0;
	this->m_DTKImageH = 0;
	this->m_DTKImagePtr = NULL;
	this->m_DTKImageWAligned = 0;
}
//--------------------------------------------------------------------------------------------//