// JetZip.h: interface for the CJetZip class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JETZIP_H__D84CA857_43F2_4C79_BC0B_E0679C111700__INCLUDED_)
#define AFX_JETZIP_H__D84CA857_43F2_4C79_BC0B_E0679C111700__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef ZLIB_USE
//-------------------------------------------------------------------//
#define ZLIB_WINAPI//要加入此宣告, 否則無法連到ZLib.LIB
//-------------------------------------------------------------------//
#include <stdio.h>
#include <stdlib.h>
#include "..\\JET8000_Library\\Zlib\\include\\zlib.h"
/* Compression Level
	Z_NO_COMPRESSION   
	Z_BEST_SPEED      
	Z_BEST_COMPRESSION   
	Z_DEFAULT_COMPRESSION  (-1)
*/
//-------------------------------------------------------------------//
class CJetZip  
{
protected:
	//---------------------------------------------------------------------------------//
	static CRITICAL_SECTION    m_csJetZip;//同步機制-關鍵區間
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	static void                InitialJetZipLock(); //初始化JetZip的關鍵區間
	static void                DeleteJetZipLock();  //刪除JetZip的關鍵區間
	static void                LockJetZip();        //進入JetZip的關鍵區間
	static void                UnlockJetZip();      //離開JetZip的關鍵區間
	//---------------------------------------------------------------------------------//
protected:
	//------------------------------------------------------------------------------------------//
	CString                    m_ErrorString;
	int                        m_CompressionLevel;
	void                       PreInitial();
	void                       Release();
	//------------------------------------------------------------------------------------------//
/*
	void AddFileToZip(zipFile zf, const char* fileNameInZip, const char* srcFile); 
	void CollectFilesInDirToZip(zipFile zf, const CString& strPath, const CString& parentDir);
	//最終介面：從某個目錄創建zip檔  
	void CreateZipFromDir(const CString& dirName, const CString& zipFileName);  
*/
	//------------------------------------------------------------------------------------------//
	bool                       CheckData(const void *Ptr, size_t Size);
	size_t                     CalcBufferSize(IMAGE_SIZE PtrStep, IMAGE_SIZE PtrH) const;//計算記憶體大小
	bool                       CheckGraySize(IMAGE_SIZE PtrW, IMAGE_SIZE PtrH, IMAGE_SIZE PtrStep);
	bool                       CheckGrayImage(IMAGE_SIZE PtrW, IMAGE_SIZE PtrH, IMAGE_SIZE PtrStep, const void *Ptr);
	//------------------------------------------------------------------------------------------//
	bool                       SaveGrayPtr_GZip(LPCTSTR pfilename, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, unsigned int TypeSize, const void *Ptr);	
	bool                       LoadImageSize_GZip(gzFile fp, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount);
	//------------------------------------------------------------------------------------------//
public:
	//------------------------------------------------------------------------------------------//
	CJetZip();
	virtual ~CJetZip();
	//------------------------------------------------------------------------------------------//	
public:
	//------------------------------------------------------------------------------------------//
	LPCTSTR                    GetErrorString() const;
	void                       SetCompressionLevel(int Level);
	//------------------------------------------------------------------------------------------//
	bool                       DoJetZip3DWrite(LPCTSTR ZipPath,float *p3D,int DataW, int DataH);
	bool                       DoJetZip3DRead(LPCTSTR ZipPath,float *p3D,int DataW, int DataH);
	//------------------------------------------------------------------------------------------//
	bool                       SaveGZipBuffer(LPCTSTR pfilename, const void *Ptr, unsigned int TypeSize, unsigned int DataSize);
	bool                       LoadGZipBuffer(LPCTSTR pfilename, void *Ptr, unsigned int TypeSize, unsigned int DataSize);
	//------------------------------------------------------------------------------------------//
	bool                       SaveGZipGrayImage(LPCTSTR pfilename, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const int *Ptr);
	bool                       LoadGZipGrayImage(LPCTSTR pfilename, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, int *&Ptr);	

	bool                       SaveGZipGrayImage(LPCTSTR pfilename, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const float *Ptr);
	bool                       LoadGZipGrayImage(LPCTSTR pfilename, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, float *&Ptr);	

	bool                       SaveGZipGrayImage(LPCTSTR pfilename, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const short *Ptr);
	bool                       LoadGZipGrayImage(LPCTSTR pfilename, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, short *&Ptr);	

	bool                       SaveGZipGrayImage(LPCTSTR pfilename, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *Ptr);
	bool                       LoadGZipGrayImage(LPCTSTR pfilename, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, unsigned char *&Ptr);	

	bool                       SaveGZipGrayImage(LPCTSTR pfilename, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned short *Ptr);
	bool                       LoadGZipGrayImage(LPCTSTR pfilename, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, unsigned short *&Ptr);	
//	bool                       DoTestSaveFolder();
	//------------------------------------------------------------------------------------------//	
};
#endif//ZLIB_USE

#endif // !defined(AFX_JETZIP_H__D84CA857_43F2_4C79_BC0B_E0679C111700__INCLUDED_)
