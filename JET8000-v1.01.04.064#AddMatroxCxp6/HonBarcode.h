// HonBarcode.h: interface for the CHonBarcode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HONBARCODE_H__307B4174_B58F_455D_BE92_87FF2136830D__INCLUDED_)
#define AFX_HONBARCODE_H__307B4174_B58F_455D_BE92_87FF2136830D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#define HON_API               //static
//-------------------------------------------------------------------------------------//
class CHonBarcode //Honeywell SwiftDecoder
{
protected:
	//---------------------------------------------------------------------------------//
	HON_API int                 m_HandleHon;//Honeywell-Handle
	HON_API bool                m_bLicenseHon;//是否授權過
	HON_API CRITICAL_SECTION    m_csHon;//同步機制-關鍵區間	
	HON_API CString             m_ErrorString;
	HON_API CString             m_LicenseFilename;//授權檔名
	//---------------------------------------------------------------------------------//		
	HON_API bool                SaveINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pString, LPCTSTR pfilename, CString &Error);
	HON_API bool                LoadINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pDefault, TCHAR *pString, int StringSize, LPCTSTR pfilename, bool IsCheckLens, CString &Error);	
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	static void                 SD_CB_Result(int Handle);
	//---------------------------------------------------------------------------------//
	HON_API void                InitialHonBarcodeLock(); //初始化Honeywell條碼關鍵區間
	HON_API void                DeleteHonBarcodeLock();  //刪除Honeywell條碼關鍵區間
	HON_API void                LockHonBarcode();        //進入Honeywell條碼關鍵區間
	HON_API void                UnlockHonBarcode();      //離開Honeywell條碼關鍵區間
	//---------------------------------------------------------------------------------//
	HON_API bool                CheckHonLicenseKey();//確認Honeywell授權通過		
	HON_API bool                GetHonLibraryVersion(CString &str);//取得Honeywell條碼函式庫版本
	//---------------------------------------------------------------------------------//
	HON_API bool                SaveHonIniFile();//儲存Honeywell參數檔案
	HON_API bool                LoadHonIniFile();//載入Honeywell參數檔案	
	//---------------------------------------------------------------------------------//	
	HON_API LPCTSTR             GetErrorString();	
	HON_API bool                CheckSwiftDecoderHandle();
	HON_API bool                CheckSwiftDecoderLicense();
	HON_API bool                HoneywellSwiftDecoder_Create();
	HON_API bool                HoneywellSwiftDecoder_Destroy();
	HON_API bool                HoneywellSwiftDecoder_Read(int Key, void *Val);
	HON_API bool                HoneywellSwiftDecoder_Write(int Key, void *Val);
	HON_API bool                HoneywellSwiftDecoder_ReadResult();
	//---------------------------------------------------------------------------------//	
private:
	//---------------------------------------------------------------------------------//
	bool                       m_HonCheckSum;		
	size_t                     m_HonBarcodeLength;
	char                       m_HonBarcodeResult[256];
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//	
	CHonBarcode(const CHonBarcode &barcode);
	//---------------------------------------------------------------------------------//
	CHonBarcode& operator=(const CHonBarcode &barcode);
	//---------------------------------------------------------------------------------//
	bool EnableAllDecode(bool bEnabled);
	bool ExecDecodeImage_Hon(int ImageW, int ImageH, int ImageStep, unsigned char *pImage, char* BarcodeText, size_t BarcodeTextLen);
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	CHonBarcode();
	virtual ~CHonBarcode();
	//---------------------------------------------------------------------------------//		
	void SetVerifyChecksum_Hon(bool Check) { m_HonCheckSum = Check; }	
	//---------------------------------------------------------------------------------//		
	size_t GetHonBarcodeLength() const { return m_HonBarcodeLength; }
	void SetHonBarcodeLength(size_t val) { m_HonBarcodeLength = val; }
	//---------------------------------------------------------------------------------//
	bool ExecDecode1D_Hon(int ImageW, int ImageH, int ImageStep, unsigned char *pImage, char* BarcodeText, size_t BarcodeTextLen);//讀取
	bool ExecDecodeQRCode_Hon(int ImageW, int ImageH, int ImageStep, unsigned char *pImage, char* BarcodeText, size_t BarcodeTextLen);
	bool ExecDecodeDataMatrix_Hon(int ImageW, int ImageH, int ImageStep, unsigned char *pImage, char* BarcodeText, size_t BarcodeTextLen);
	//---------------------------------------------------------------------------------//
};
//-------------------------------------------------------------------------------------//
extern CHonBarcode   HoneywellBarcodeSdk;
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_HONBARCODE_H__307B4174_B58F_455D_BE92_87FF2136830D__INCLUDED_)
