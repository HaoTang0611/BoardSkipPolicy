// DtkBarcode.h: interface for the CDtkBarcode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DTKBARCODE_H__307B4174_B58F_455D_BE92_87FF2136830D__INCLUDED_)
#define AFX_DTKBARCODE_H__307B4174_B58F_455D_BE92_87FF2136830D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
//#define DTK_BARCODE_CRITICAL_SECTION_USE
//-------------------------------------------------------------------------------------//
#include "..\\JET8000_Library\\DTK\\Include\\DTKBarReader.h"
//-------------------------------------------------------------------------------------//
class CDtkBarcode  
{
protected:
	//---------------------------------------------------------------------------------//
	static unsigned long       m_nRef;//實體數量
	static unsigned long       m_nUniqueIDCount;//唯一碼數量
#ifdef DTK_BARCODE_CRITICAL_SECTION_USE
	static CRITICAL_SECTION    m_csDTK;//同步機制-關鍵區間
#endif//DTK_BARCODE_CRITICAL_SECTION_USE	
	//---------------------------------------------------------------------------------//
	static CString             m_DTKVersion;
	static int                 m_DTKScanInterval;
	static int                 m_DTKQuietZoneSize;//QZ_Large, QZ_Normal, QZ_Small, QZ_ExtraSmall//加速:Smal
	static int                 m_DTKThresholdMode;
	static int                 m_DTKThresholdValue;
	static int                 m_DTKThresholdCount;
	static int                 m_DTKThresholdStep;	
	//---------------------------------------------------------------------------------//	
	static bool                SaveINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pString, LPCTSTR pfilename, CString &Error);
	static bool                LoadINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pDefault, TCHAR *pString, int StringSize, LPCTSTR pfilename, bool IsCheckLens, CString &Error);	
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	static void                InitialDTKBarcodeLock(); //初始化DTK條碼關鍵區間
	static void                DeleteDTKBarcodeLock();  //刪除DTK條碼關鍵區間
	static void                LockDTKBarcode();        //進入DTK條碼關鍵區間
	static void                UnlockDTKBarcode();      //離開DTK條碼關鍵區間
	//---------------------------------------------------------------------------------//	
	static bool                LoadDTKLibrary(CString &DTKErr);//載入DTK條碼函式庫
	static bool                FreeDTKLibrary();//釋放DTK條碼函式庫	
	static bool                GetDTKLibraryVersion(CString &str);//取得DTK條碼函式庫版本
	//---------------------------------------------------------------------------------//
	static bool                SaveDTKIniFile();//儲存DTK參數檔案
	static bool                LoadDTKIniFile();//載入DTK參數檔案	
	//---------------------------------------------------------------------------------//
private:
	//---------------------------------------------------------------------------------//
	CString                    m_DTKErr;	
	unsigned long              m_UniqueID;
	IBarcodeReader            *m_DTKReader;
	//---------------------------------------------------------------------------------//
	CString                    m_DTKImageFileName;
	bool                       m_DTKCheckSum;
	int                        m_DTKBarcodeDir;
	//---------------------------------------------------------------------------------//
	int                        m_DTKImageW;
	int                        m_DTKImageH;
	int                        m_DTKImageWAligned;
	unsigned char             *m_DTKImagePtr;
	//---------------------------------------------------------------------------------//	
	int                        m_DTKRecognitionTimeout;
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//	
	CDtkBarcode(const CDtkBarcode &barcode);
	//---------------------------------------------------------------------------------//
	CDtkBarcode& operator=(const CDtkBarcode &barcode);
	//---------------------------------------------------------------------------------//
	bool CheckDtkBarcodeLibraryFunc();
	bool CheckDtkBarcodeReader();//確認DTK條碼初始化
	bool GetDTKErrorMSG(CString &str);	
	bool CreateDtkBarcodeReader(IBarcodeReader *&Reader);
	bool DestroyDtkBarcodeReader(IBarcodeReader *&Reader);	
	bool ExecDecode2D_DTK(char *Buffer, size_t BufferSize);
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	CDtkBarcode();
	virtual ~CDtkBarcode();
	//---------------------------------------------------------------------------------//	
	LPCTSTR GetErrorString();
	//---------------------------------------------------------------------------------//
	unsigned long GetDtkBarcodeUniqueID() const;	
	//---------------------------------------------------------------------------------//
	void SetLoadImage_DTK(LPCTSTR pfilename);
	CString BuildLoadImage_DTK(LPCTSTR Folder, LPCTSTR Name) const;	
	void SetVerifyChecksum_DTK(bool Check) { m_DTKCheckSum = Check; }
	void SetBarcodeDirection_DTK(int Dir) { m_DTKBarcodeDir = Dir; }
	//---------------------------------------------------------------------------------//	
	void SetRecognitionTimeout_DTK(int val) { m_DTKRecognitionTimeout = val; }
	//---------------------------------------------------------------------------------//
	bool ExecDecode1D_DTK(char *Buffer, size_t BufferSize);
	bool ExecDecodeQRCode_DTK(char *Buffer, size_t BufferSize);
	bool ExecDecodeDataMatrix_DTK(char *Buffer, size_t BufferSize);
	//---------------------------------------------------------------------------------//
	void SetImageBits_DTK(int ImageW, int ImageH, const unsigned char *pImage, int ImageW2);
	void FreeImageBits_DTK();
	//---------------------------------------------------------------------------------//	
};
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_DTKBARCODE_H__307B4174_B58F_455D_BE92_87FF2136830D__INCLUDED_)
