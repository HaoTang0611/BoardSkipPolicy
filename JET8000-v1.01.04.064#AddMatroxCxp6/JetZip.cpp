// JetZip.cpp: implementation of the CJetZip class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "JetZip.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#ifdef ZLIB_USE
//-------------------------------------------------------------------------------------//
CRITICAL_SECTION CJetZip::m_csJetZip;//同步機制-關鍵區間
//-------------------------------------------------------------------------------------//
void CJetZip::InitialJetZipLock() //初始化JetZip的關鍵區間
{
	::InitializeCriticalSection(&m_csJetZip);
}
//-------------------------------------------------------------------------------------//
void CJetZip::DeleteJetZipLock()  //刪除JetZip的關鍵區間
{
	::DeleteCriticalSection(&m_csJetZip);
}
//-------------------------------------------------------------------------------------//
void CJetZip::LockJetZip()//進入JetZip的關鍵區間
{
	return;
	::EnterCriticalSection(&m_csJetZip);
}
//-------------------------------------------------------------------------------------//
void CJetZip::UnlockJetZip()//離開JetZip的關鍵區間
{
	return;
	::LeaveCriticalSection(&m_csJetZip);
}
//-------------------------------------------------------------------------------------//
CJetZip::CJetZip()
{
	PreInitial();
}
//-------------------------------------------------------------------------------------//
CJetZip::~CJetZip()
{
	Release();
}
//-------------------------------------------------------------------------------------//
bool CJetZip::DoJetZip3DWrite(LPCTSTR ZipPath,float *p3D,int DataW, int DataH)
{
	CString strFilePath = "";
	//存檔路徑
	strFilePath = ZipPath;

	LPVOID lpDataAddress = NULL;
	DWORD dwDataSize = 0, dwFileLengthToWrite = 0;

	//要壓縮的資料
	lpDataAddress = p3D; 
	dwDataSize = DataW * DataH * sizeof(float);

	//因為壓縮函數的輸出緩衝必須比輸入大0.1%+12然後一個DWORD用來保存壓縮前的大小
	//解壓縮的時候用，當然還可以保存更多的信息，這裡用不到
//	dwFileLengthToWrite = (double)dwDataSize*1.001 + 12 + sizeof(DWORD);

	dwFileLengthToWrite = compressBound (dwDataSize);
	dwFileLengthToWrite += sizeof(DWORD);//存原始檔案大小



	HANDLE hFileToWrite = NULL;
	HANDLE hMapFileToWrite = NULL;
	LPVOID lpMapAddressToWrite = NULL;
	//以下是創建一個文件，用來保存壓縮後的文件
	hFileToWrite = CreateFile(strFilePath, // demoFile.rar
		GENERIC_WRITE|GENERIC_READ, // open for writing
		0, // do not share
		NULL, // no security
		CREATE_ALWAYS, // overwrite existing
		FILE_ATTRIBUTE_NORMAL , // normal file
		NULL); // no attr. template
	
	if (hFileToWrite == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hMapFileToWrite);
		m_ErrorString = _T("Could not open file to write");
		return false;
	}

	hMapFileToWrite = CreateFileMapping(hFileToWrite, // Current file handle.
		NULL, // Default security.
		PAGE_READWRITE, // Read/write permission.
		0, // Max. object size.
		dwFileLengthToWrite, // Size of hFile.
		_T("ZipTestMappingObjectForWrite")); // Name of mapping object.
	
	if (hMapFileToWrite == NULL)
	{
		CloseHandle(hMapFileToWrite);
		SetFilePointer(hFileToWrite,dwFileLengthToWrite + sizeof(DWORD) ,NULL,FILE_BEGIN);
		SetEndOfFile(hFileToWrite);
		CloseHandle(hFileToWrite);
		m_ErrorString = _T("Could not create file mapping object for write");
		return false;
	}

	lpMapAddressToWrite = MapViewOfFile(hMapFileToWrite, // Handle to mapping object.
		FILE_MAP_WRITE, // Read/write permission
		0, // Max. object size.
		0, // Size of hFile.
		0); // Map entire file.
	
	if (lpMapAddressToWrite == NULL)
	{
		UnmapViewOfFile(lpMapAddressToWrite);
		CloseHandle(hMapFileToWrite);
		SetFilePointer(hFileToWrite,dwFileLengthToWrite + sizeof(DWORD) ,NULL,FILE_BEGIN);
		SetEndOfFile(hFileToWrite);
		CloseHandle(hFileToWrite);
		m_ErrorString = _T("Could not map view of file");
		return false;
	}

	//這裡是將壓縮前的大小保存在文件的第一個DWORD裡面
	LPVOID pBuf = lpMapAddressToWrite;
	(*(DWORD*)pBuf) = dwDataSize;
	pBuf = (DWORD*)pBuf + 1;

	//////////////////////////////////////////
	int err;

	err = compress2((Bytef*)pBuf,&dwFileLengthToWrite, (Bytef*)lpDataAddress, dwDataSize,m_CompressionLevel);
	if (err != Z_OK) 
	{
		m_ErrorString = _T("compress fault!");
		UnmapViewOfFile(lpMapAddressToWrite);
		CloseHandle(hMapFileToWrite);
		SetFilePointer(hFileToWrite,dwFileLengthToWrite + sizeof(DWORD) ,NULL,FILE_BEGIN);
		SetEndOfFile(hFileToWrite);
		CloseHandle(hFileToWrite);	
		return false;
	}

	UnmapViewOfFile(lpMapAddressToWrite);
	CloseHandle(hMapFileToWrite);
	//這裡將文件大小重新配置一下
	SetFilePointer(hFileToWrite,dwFileLengthToWrite + sizeof(DWORD) ,NULL,FILE_BEGIN);
	//SetFilePointer(hFileToWrite,dwFileLengthToWrite ,NULL,FILE_BEGIN);
	SetEndOfFile(hFileToWrite);
	CloseHandle(hFileToWrite);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetZip::DoJetZip3DRead(LPCTSTR ZipPath,float *p3D,int DataW, int DataH)
{	
	if( p3D == NULL )
	{
		m_ErrorString.Format(_T("p3D == NULL!"));
		return false;
	}
	int BufferSize = DataW * DataH * sizeof(float);


	CString strFilePath = "";
	//讀檔路徑
	strFilePath = ZipPath;

	LPVOID lpDataAddress = NULL;
	DWORD dwDataSize = 0, dwFileLength = 0;

	lpDataAddress = p3D;

	HANDLE hFile = NULL;
	HANDLE hMapFile = NULL;	
	LPVOID lpMapAddress = NULL;
	
	//打開要進行解壓縮的文件
	hFile = CreateFile(strFilePath, // file name
		GENERIC_READ, // open for reading
		FILE_SHARE_READ, // share for reading
		NULL, // no security
		OPEN_EXISTING, // existing file only
		FILE_ATTRIBUTE_NORMAL, // normal file
		NULL); // no attr. template
	
	if (hFile == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);
		this->m_ErrorString = _T("Could not open file to read"); // process error
		return false;
	}
	
	//創建一個文件映射
	hMapFile = CreateFileMapping(hFile, // Current file handle.
		NULL, // Default security.
		PAGE_READONLY, // Read/write permission.
		0, // Max. object size.
		0, // Size of hFile.
		_T("ZipTestMappingObjectForRead")); // Name of mapping object.
	
	if (hMapFile == NULL)
	{
		CloseHandle(hMapFile);
		CloseHandle(hFile);
		this->m_ErrorString = _T("Could not create file mapping object");
		return false;
	}

	//創建一個文件映射的視圖用來作為source
	lpMapAddress = MapViewOfFile(hMapFile, // Handle to mapping object.
		FILE_MAP_READ, // Read/write permission
		0, // Max. object size.
		0, // Size of hFile.
		0); // Map entire file.
	
	if (lpMapAddress == NULL)
	{
		UnmapViewOfFile(lpMapAddress);
		CloseHandle(hMapFile);
		CloseHandle(hFile);
		this->m_ErrorString = _T("Could not map view of file");
		return false;
	}

	//////////////////////////////////////////////////////////////////////////////////
	dwFileLength = GetFileSize(hFile, NULL) - sizeof(DWORD);

	dwDataSize = (*(DWORD*)lpMapAddress);
	
	LPVOID pSourceBuf = lpMapAddress;
	pSourceBuf = (DWORD*)pSourceBuf + 1;

	if( BufferSize != (int)dwDataSize )
	{
		UnmapViewOfFile(lpMapAddress);
		CloseHandle(hMapFile);
		CloseHandle(hFile);
		m_ErrorString.Format(_T("dwDataSize = %d , BufferSize = %d"),dwDataSize,BufferSize);
		return false;
	}


	int err = 0;
	
	uncompress((Bytef*)lpDataAddress,&dwDataSize, (Bytef*)pSourceBuf, dwFileLength);
	if (err != Z_OK) 
	{
		UnmapViewOfFile(lpMapAddress);
		CloseHandle(hMapFile);
		CloseHandle(hFile);
		this->m_ErrorString = _T("uncompress fault!");
		return false;
	}	

	if( BufferSize != (int)dwDataSize )
	{
		UnmapViewOfFile(lpMapAddress);
		CloseHandle(hMapFile);
		CloseHandle(hFile);
		m_ErrorString.Format(_T("dwDataSize = %d , BufferSize = %d"),dwDataSize,BufferSize);
		return false;
	}
	UnmapViewOfFile(lpMapAddress);
	CloseHandle(hMapFile);
	CloseHandle(hFile);
	return true;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CJetZip::GetErrorString() const
{
	return this->m_ErrorString;
}
//-------------------------------------------------------------------------------------//
void CJetZip::PreInitial()
{
	m_ErrorString = _T("");
	m_CompressionLevel = Z_BEST_SPEED;
}
//-------------------------------------------------------------------------------------//
void CJetZip::Release()
{
}
//-------------------------------------------------------------------------------------//
void CJetZip::SetCompressionLevel(int Level)
{
	m_CompressionLevel = Level;
}
//-------------------------------------------------------------------------------------//
inline bool CJetZip::CheckData(const void *Ptr, size_t Size)
{
	if ( 0 == Size )
	{
		this->m_ErrorString.Format(_T("Error, Data Size Exception (%d)"), Size);
		return false;
	}
	if ( NULL == Ptr ) 
	{
		this->m_ErrorString.Format(_T("Error, Data Ptr Exception (NULL)"));
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
size_t CJetZip::CalcBufferSize(IMAGE_SIZE PtrStep, IMAGE_SIZE PtrH) const//計算記憶體大小
{
	return (size_t)(PtrStep)*(size_t)(PtrH);
}
//-------------------------------------------------------------------------------------//
inline bool CJetZip::CheckGraySize(IMAGE_SIZE PtrW, IMAGE_SIZE PtrH, IMAGE_SIZE PtrStep)
{
	if ( 0==PtrW || 0==PtrH || PtrStep<PtrW )
	{
		this->m_ErrorString.Format(_T("Error, Gray Image Szie Exception (W:%d, H:%d, Step:%d)"), PtrW,PtrH, PtrStep);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CJetZip::CheckGrayImage(IMAGE_SIZE PtrW, IMAGE_SIZE PtrH, IMAGE_SIZE PtrStep, const void *Ptr)
{
	if ( CJetZip::CheckGraySize(PtrW, PtrH, PtrStep) == false )
	{	return false; }	
	if ( NULL == Ptr )
	{
		this->m_ErrorString.Format(_T("Error, Image Ptr Exception (NULL)"));
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetZip::SaveGZipBuffer(LPCTSTR pfilename, const void *Ptr, unsigned int TypeSize, unsigned int DataSize)
{
	if ( CJetZip::CheckData(Ptr, DataSize) == false )
	{	return false; }

	const unsigned int chBufferSize = MAX_JET_PATH;
	char filename[chBufferSize]="";
	const unsigned int BufferSize = DataSize*TypeSize;	
	JetAPI::TCHAR2char(pfilename, filename, chBufferSize);

	void *pBuf = (void*)Ptr;	
	gzFile fp=NULL;
	fp = gzopen(filename, "wb1");
	if( fp == NULL)
	{
		m_ErrorString.Format(_T("Error, GZ Open Error! (%s)"), pfilename);
		return false;
	}	
	size_t FileWriteSize = 0;
	FileWriteSize = gzwrite(fp, pBuf, BufferSize);
	if( FileWriteSize==0 || FileWriteSize!=BufferSize )
	{
		m_ErrorString.Format(_T("Error, GZ Write Error! FileWriteSize = %d ,BufferSize = %d"), FileWriteSize, BufferSize);
		gzclose(fp);
		return false;
	}
	gzclose(fp);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetZip::LoadGZipBuffer(LPCTSTR pfilename, void *Ptr, unsigned int TypeSize, unsigned int DataSize)
{
	if ( CJetZip::CheckData(Ptr, DataSize) == false )
	{	return false; }	

	const unsigned int chBufferSize = MAX_JET_PATH;
	char filename[chBufferSize]="";
	const unsigned int BufferSize = DataSize*TypeSize;	
	JetAPI::TCHAR2char(pfilename, filename, chBufferSize);

	void *pBuf = Ptr;	
	gzFile fp=NULL;	
	fp = gzopen(filename,"rb");
	if( fp == NULL)
	{
		m_ErrorString.Format(_T("Error, GZ Open Error! (%s)"), pfilename);
		return false;
	}

	size_t FileReadSize = 0;
	FileReadSize = gzread(fp, pBuf, BufferSize);
	if( FileReadSize==-1 || FileReadSize!=BufferSize )
	{
		m_ErrorString.Format(_T("eRROR, GZ Read Error! FileReadSize = %d ,BufferSize = %d"), FileReadSize, BufferSize);
		gzclose(fp);
		return false;
	}
	gzclose(fp);
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CJetZip::SaveGrayPtr_GZip(LPCTSTR pfilename, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, unsigned int TypeSize, const void *Ptr)
{
	if ( CJetZip::CheckGrayImage(ImageW, ImageH, ImageStep, Ptr) == false )	
	{	return false; }	

	const unsigned int chBufferSize = MAX_JET_PATH;
	char filename[chBufferSize]="";
	const size_t ImageSize = CalcBufferSize(ImageStep, ImageH);
	const size_t BufferSize = ImageSize*TypeSize;	
	JetAPI::TCHAR2char(pfilename, filename, chBufferSize);

	void *pBuf = (void*)Ptr;
	gzFile fp = NULL;
	size_t FileWriteSize = 0;

	fp = gzopen(filename, "wb");
	if( fp == NULL)
	{
		m_ErrorString.Format(_T("Error, GZ Open Error! (%s)"), pfilename);
		return false;
	}	
	
	//Save Image Width
	FileWriteSize = gzwrite(fp, &ImageW, sizeof(ImageW));	
	if( FileWriteSize==0 || FileWriteSize!=sizeof(ImageW) )
	{
		m_ErrorString.Format(_T("Error, GZ Write Error (Width:%d)"), ImageW);
		gzclose(fp);
		return false;
	}
	//Save Image Height
	FileWriteSize = gzwrite(fp, &ImageH, sizeof(ImageH));	
	if( FileWriteSize==0 || FileWriteSize!=sizeof(ImageH) )
	{
		m_ErrorString.Format(_T("Error, GZ Write Error (Height:%d)"), ImageH);
		gzclose(fp);
		return false;
	}
	//Save Image Step
	FileWriteSize = gzwrite(fp, &ImageStep, sizeof(ImageStep));	
	if( FileWriteSize==0 || FileWriteSize!=sizeof(ImageStep) )
	{
		m_ErrorString.Format(_T("Error, GZ Write Error (Step:%d)"), ImageStep);
		gzclose(fp);
		return false;
	}
	//Save Image BitCount	
	FileWriteSize = gzwrite(fp, &BitCount, sizeof(BitCount));	
	if( FileWriteSize==0 || FileWriteSize!=sizeof(BitCount) )
	{
		m_ErrorString.Format(_T("Error, GZ Write Error (BitCount:%d)"), BitCount);
		gzclose(fp);
		return false;
	}

	FileWriteSize = gzwrite(fp,pBuf, BufferSize);	
	if( FileWriteSize==0 || FileWriteSize!=BufferSize )
	{
		m_ErrorString.Format(_T("Error, GZ Write Error! FileWriteSize = %d ,BufferSize = %d"), FileWriteSize, BufferSize);
		gzclose(fp);
		return false;
	}
	gzclose(fp);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetZip::LoadImageSize_GZip(gzFile fp, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount)
{
	size_t FileReadSize = 0;
	if ( fp == NULL)
	{
		m_ErrorString.Format(_T("Error, GZ File Exception (NULL)"));
		return false;
	}

	//Read Image Width
	FileReadSize = gzread(fp, &ImageW, sizeof(ImageW));
	if( FileReadSize==-1 || FileReadSize!=sizeof(ImageW) )
	{
		m_ErrorString.Format(_T("Error, GZ Read Error! (Width:%d)"), ImageW);		
		return false;
	}
	//Read Image Height
	FileReadSize = gzread(fp, &ImageH, sizeof(ImageH));
	if( FileReadSize==-1 || FileReadSize!=sizeof(ImageH) )
	{
		m_ErrorString.Format(_T("Error, GZ Read Error! (Height:%d)"), ImageH);		
		return false;
	}
	//Read Image Step
	FileReadSize = gzread(fp, &ImageStep, sizeof(ImageStep));
	if( FileReadSize==-1 || FileReadSize!=sizeof(ImageStep) )
	{
		m_ErrorString.Format(_T("Error, GZ Read Error! (Step:%d)"), ImageStep);		
		return false;
	}
	//Read Image BitCount
	FileReadSize = gzread(fp, &BitCount, sizeof(BitCount));
	if( FileReadSize==-1 || FileReadSize!=sizeof(BitCount) )
	{
		m_ErrorString.Format(_T("Error, GZ Read Error! (BitCount:%d)"), BitCount);		
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetZip::SaveGZipGrayImage(LPCTSTR pfilename, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const int *Ptr)
{
	return CJetZip::SaveGrayPtr_GZip(pfilename, ImageW, ImageH, ImageStep, BitCount, sizeof(int), Ptr);	
}
//-------------------------------------------------------------------------------------//
bool CJetZip::LoadGZipGrayImage(LPCTSTR pfilename, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, int *&Ptr)
{
	char fnName[] = "CJetZip::LoadGZipGrayImage-Int";
	const unsigned int chBufferSize = MAX_JET_PATH;
	char filename[chBufferSize]="";	
	JetAPI::TCHAR2char(pfilename, filename, chBufferSize);

	JetMemory.free_func(Ptr);

	gzFile fp=NULL;
	size_t FileReadSize = 0;
	fp = gzopen(filename,"rb");
	if( fp == NULL)
	{
		m_ErrorString.Format(_T("Error, GZ Open Error! (%s)"), pfilename);
		return false;
	}
	if ( CJetZip::LoadImageSize_GZip(fp, ImageW, ImageH, ImageStep, BitCount) == false )
	{
		gzclose(fp);
		return false;
	}
	if ( CJetZip::CheckGraySize(ImageW, ImageH, ImageStep) == false )
	{
		gzclose(fp);
		return false;
	}
	const size_t PtrSize = CalcBufferSize(ImageStep, ImageH);
	const size_t BufferSize = PtrSize*sizeof(int);	
	if ( JetMemory.alloc_func(PtrSize, Ptr, fnName, "Ptr") == false )
	{
		this->m_ErrorString = JetMemory.GetErrorString();
		gzclose(fp);
		return false;
	}

	void *pBuf = Ptr;
	FileReadSize = gzread(fp,pBuf, BufferSize);
	if( FileReadSize==-1 || FileReadSize!=BufferSize )
	{
		JetMemory.free_func(Ptr);
		m_ErrorString.Format(_T("Error, GZ Read Error! FileReadSize = %d ,BufferSize = %d"), FileReadSize, BufferSize);
		gzclose(fp);
		return false;
	}	
	gzclose(fp);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetZip::SaveGZipGrayImage(LPCTSTR pfilename, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const float *Ptr)
{
	return CJetZip::SaveGrayPtr_GZip(pfilename, ImageW, ImageH, ImageStep, BitCount, sizeof(float), Ptr);	
}
//-------------------------------------------------------------------------------------//
bool CJetZip::LoadGZipGrayImage(LPCTSTR pfilename, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, float *&Ptr)
{
	char fnName[] = "CJetZip::LoadGZipGrayImage-Float";
	const unsigned int chBufferSize = MAX_JET_PATH;
	char filename[chBufferSize]="";	
	JetAPI::TCHAR2char(pfilename, filename, chBufferSize);

	JetMemory.free_func(Ptr);

	gzFile fp=NULL;
	size_t FileReadSize = 0;
	fp = gzopen(filename,"rb");
	if( fp == NULL)
	{
		m_ErrorString.Format(_T("Error, GZ Open Error! (%s)"), pfilename);
		return false;
	}
	if ( CJetZip::LoadImageSize_GZip(fp, ImageW, ImageH, ImageStep, BitCount) == false )
	{
		gzclose(fp);
		return false;
	}
	if ( CJetZip::CheckGraySize(ImageW, ImageH, ImageStep) == false )
	{
		gzclose(fp);
		return false;
	}
	const size_t PtrSize = CalcBufferSize(ImageStep, ImageH);
	const size_t BufferSize = PtrSize*sizeof(float);	
	if ( JetMemory.alloc_func(PtrSize, Ptr, fnName, "Ptr") == false )
	{
		this->m_ErrorString = JetMemory.GetErrorString();
		gzclose(fp);
		return false;
	}

	void *pBuf = Ptr;
	FileReadSize = gzread(fp,pBuf, BufferSize);
	if( FileReadSize==-1 || FileReadSize!=BufferSize )
	{
		JetMemory.free_func(Ptr);
		m_ErrorString.Format(_T("Error, GZ Read Error! FileReadSize = %d ,BufferSize = %d"), FileReadSize, BufferSize);
		gzclose(fp);
		return false;
	}	
	gzclose(fp);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetZip::SaveGZipGrayImage(LPCTSTR pfilename, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const short *Ptr)
{
	return CJetZip::SaveGrayPtr_GZip(pfilename, ImageW, ImageH, ImageStep, BitCount, sizeof(short), Ptr);	
}
//-------------------------------------------------------------------------------------//
bool CJetZip::LoadGZipGrayImage(LPCTSTR pfilename, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, short *&Ptr)
{
	const char fnName[] = "CJetZip::LoadGZipGrayImage-SHRT";
	const unsigned int chBufferSize = MAX_JET_PATH;
	char filename[chBufferSize]="";	
	JetAPI::TCHAR2char(pfilename, filename, chBufferSize);

	JetMemory.free_func(Ptr);

	gzFile fp=NULL;
	size_t FileReadSize = 0;
	fp = gzopen(filename,"rb");
	if( fp == NULL)
	{
		m_ErrorString.Format(_T("Error, GZ Open Error! (%s)"), pfilename);
		return false;
	}
	if ( CJetZip::LoadImageSize_GZip(fp, ImageW, ImageH, ImageStep, BitCount) == false )
	{
		gzclose(fp);
		return false;
	}
	if ( CJetZip::CheckGraySize(ImageW, ImageH, ImageStep) == false )
	{
		gzclose(fp);
		return false;
	}
	const size_t PtrSize = CalcBufferSize(ImageStep, ImageH);
	const size_t BufferSize = PtrSize*sizeof(short);	
	if ( JetMemory.alloc_func(PtrSize, Ptr, fnName, "Ptr") == false )
	{
		this->m_ErrorString = JetMemory.GetErrorString();
		gzclose(fp);
		return false;
	}

	void *pBuf = Ptr;
	FileReadSize = gzread(fp,pBuf, BufferSize);
	if( FileReadSize==-1 || FileReadSize!=BufferSize )
	{
		JetMemory.free_func(Ptr);
		m_ErrorString.Format(_T("Error, GZ Read Error! FileReadSize = %d ,BufferSize = %d"), FileReadSize, BufferSize);
		gzclose(fp);
		return false;
	}	
	gzclose(fp);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetZip::SaveGZipGrayImage(LPCTSTR pfilename, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *Ptr)
{
	return CJetZip::SaveGrayPtr_GZip(pfilename, ImageW, ImageH, ImageStep, BitCount, sizeof(unsigned char), Ptr);	
}
//-------------------------------------------------------------------------------------//
bool CJetZip::LoadGZipGrayImage(LPCTSTR pfilename, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, unsigned char *&Ptr)	
{
	const char fnName[] = "CJetZip::LoadGZipGrayImage-UCHAR";
	const unsigned int chBufferSize = MAX_JET_PATH;
	char filename[chBufferSize]="";	
	JetAPI::TCHAR2char(pfilename, filename, chBufferSize);

	JetMemory.free_func(Ptr);

	gzFile fp=NULL;
	size_t FileReadSize = 0;
	fp = gzopen(filename,"rb");
	if( fp == NULL)
	{
		m_ErrorString.Format(_T("Error, GZ Open Error! (%s)"), pfilename);
		return false;
	}
	if ( CJetZip::LoadImageSize_GZip(fp, ImageW, ImageH, ImageStep, BitCount) == false )
	{
		gzclose(fp);
		return false;
	}
	if ( CJetZip::CheckGraySize(ImageW, ImageH, ImageStep) == false )
	{
		gzclose(fp);
		return false;
	}
	const size_t PtrSize = CalcBufferSize(ImageStep, ImageH);
	const size_t BufferSize = PtrSize*sizeof(unsigned char);	
	if ( JetMemory.alloc_func(PtrSize, Ptr, fnName, "Ptr") == false )
	{
		this->m_ErrorString = JetMemory.GetErrorString();
		gzclose(fp);
		return false;
	}

	void *pBuf = Ptr;
	FileReadSize = gzread(fp,pBuf, BufferSize);
	if( FileReadSize==-1 || FileReadSize!=BufferSize )
	{
		JetMemory.free_func(Ptr);
		m_ErrorString.Format(_T("Error, GZ Read Error! FileReadSize = %d ,BufferSize = %d"), FileReadSize, BufferSize);
		gzclose(fp);
		return false;
	}	
	gzclose(fp);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetZip::SaveGZipGrayImage(LPCTSTR pfilename, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned short *Ptr)
{
	return CJetZip::SaveGrayPtr_GZip(pfilename, ImageW, ImageH, ImageStep, BitCount, sizeof(unsigned short), Ptr);	
}
//-------------------------------------------------------------------------------------//
bool CJetZip::LoadGZipGrayImage(LPCTSTR pfilename, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, unsigned short *&Ptr)
{
	const char fnName[] = "CJetZip::LoadGZipGrayImage-USHRT";
	const unsigned int chBufferSize = MAX_JET_PATH;
	char filename[chBufferSize]="";	
	JetAPI::TCHAR2char(pfilename, filename, chBufferSize);

	JetMemory.free_func(Ptr);

	gzFile fp=NULL;
	size_t FileReadSize = 0;
	fp = gzopen(filename,"rb");
	if( fp == NULL)
	{
		m_ErrorString.Format(_T("Error, GZ Open Error! (%s)"), pfilename);
		return false;
	}
	if ( CJetZip::LoadImageSize_GZip(fp, ImageW, ImageH, ImageStep, BitCount) == false )
	{
		gzclose(fp);
		return false;
	}
	if ( CJetZip::CheckGraySize(ImageW, ImageH, ImageStep) == false )
	{
		gzclose(fp);
		return false;
	}
	const size_t PtrSize = CalcBufferSize(ImageStep, ImageH);
	const size_t BufferSize = PtrSize*sizeof(unsigned short);	
	if ( JetMemory.alloc_func(PtrSize, Ptr, fnName, "Ptr") == false )
	{
		this->m_ErrorString = JetMemory.GetErrorString();
		gzclose(fp);
		return false;
	}

	void *pBuf = Ptr;
	FileReadSize = gzread(fp,pBuf, BufferSize);
	if( FileReadSize==-1 || FileReadSize!=BufferSize )
	{
		JetMemory.free_func(Ptr);
		m_ErrorString.Format(_T("Error, GZ Read Error! FileReadSize = %d ,BufferSize = %d"), FileReadSize, BufferSize);
		gzclose(fp);
		return false;
	}	
	gzclose(fp);	
	return true;
}
//-------------------------------------------------------------------------------------//

/*
bool CJetZip::DoTestSaveFolder()
{
	CString dirName = "D:\\TestWrite";
	CString zipFileName = "kerker.zip";
	
	
	CreateZipFromDir(dirName, zipFileName);
	return true;
}
//-------------------------------------------------------------------------------------//
void CJetZip::AddFileToZip(zipFile zf, const char* fileNameInZip, const char* srcFile)  
{  
	//將檔添加到zip檔中，注意如果原始檔案srcFile為空則添加空目錄  
	//fileNameInZip: 在zip檔中的檔案名，包含相對路徑  
	FILE* srcfp = NULL;  
	  
	//初始化寫入zip的檔資訊  
	zip_fileinfo zi;  
	zi.tmz_date.tm_sec = zi.tmz_date.tm_min = zi.tmz_date.tm_hour =  
	zi.tmz_date.tm_mday = zi.tmz_date.tm_mon = zi.tmz_date.tm_year = 0;  
	zi.dosDate = 0;  
	zi.internal_fa = 0;  
	zi.external_fa = 0;  
	  
	//如果srcFile為空，加入空目錄  
	char new_file_name[MAX_JET_PATH];  
	memset(new_file_name, 0, sizeof(new_file_name));  
	strcat(new_file_name, fileNameInZip);  
	if (srcFile == NULL)  
	{  
		strcat(new_file_name, "/");  
	}  
	    
	//在zip文件中創建新文件  
	zipOpenNewFileInZip(zf, new_file_name, &zi, NULL, 0, NULL, 0, NULL, Z_DEFLATED, Z_BEST_SPEED);  
	
	if (srcFile != NULL)  
	{  
		//打開原始檔案  
		HANDLE hFile = NULL;
		HANDLE hMapFile = NULL;	
		LPVOID lpMapAddress = NULL;
		
		hFile = CreateFile(srcFile, // file name
			GENERIC_READ, // open for reading
			FILE_SHARE_READ, // share for reading
			NULL, // no security
			OPEN_EXISTING, // existing file only
			FILE_ATTRIBUTE_NORMAL, // normal file
			NULL); // no attr. template
		
		if (hFile == INVALID_HANDLE_VALUE)
		{
			AfxMessageBox("Could not open file to read"); // process error
			return;
		}
		
		//創建一個文件映射
		hMapFile = CreateFileMapping(hFile, // Current file handle.
			NULL, // Default security.
			PAGE_READONLY, // Read/write permission.
			0, // Max. object size.
			0, // Size of hFile.
			"ZipTestMappingObjectForRead"); // Name of mapping object.
		
		if (hMapFile == NULL)
		{
			AfxMessageBox("Could not create file mapping object");
			return;
		}
		
		//創建一個文件映射的視圖用來作為source
		lpMapAddress = MapViewOfFile(hMapFile, // Handle to mapping object.
			FILE_MAP_READ, // Read/write permission
			0, // Max. object size.
			0, // Size of hFile.
			0); // Map entire file.
		
		if (lpMapAddress == NULL)
		{
			AfxMessageBox("Could not map view of file");
			return;
		}
		DWORD dwFileLength = 0;

		dwFileLength = GetFileSize(hFile, NULL);

		zipWriteInFileInZip(zf, lpMapAddress, dwFileLength);  

		UnmapViewOfFile(lpMapAddress);
		CloseHandle(hMapFile);
		CloseHandle(hFile);

	}  
	  
	//關閉zip文件  
	zipCloseFileInZip(zf);  
}  
//-------------------------------------------------------------------------------------//
void CJetZip::CollectFilesInDirToZip(zipFile zf, const CString& strPath, const CString& parentDir)  
{ 
	//遞迴添加子目錄到zip檔  
	//USES_CONVERSION; //for W2CA  
	    
	CString strRelativePath;  
	CFileFind finder;   
	BOOL bWorking = finder.FindFile(strPath + ("\\*.*"));  
	while(bWorking)   
	{   
		bWorking = finder.FindNextFile();   
	    if(finder.IsDots())  
	    continue;   
	        
		if(parentDir == (""))  
		{
			strRelativePath = finder.GetFileName();  
		}
		else
		{
			strRelativePath = parentDir + ("\\") + finder.GetFileName(); //生成在zip檔中的相對路徑  
		}
	    if(finder.IsDirectory())  
		{  
			AddFileToZip(zf, strRelativePath, NULL); //在zip檔中生成目錄結構  
			CollectFilesInDirToZip(zf, finder.GetFilePath(), strRelativePath); //遞迴收集子目錄檔  
			continue;  
		}  
	        
			AddFileToZip(zf, strRelativePath, finder.GetFilePath()); //將文件添加到zip文件中  
	}  
}  
//-------------------------------------------------------------------------------------//
void CJetZip::CreateZipFromDir(const CString& dirName, const CString& zipFileName)  
{
	//最終介面：從某個目錄創建zip檔  
	//USES_CONVERSION; //使用W2CA轉換unicode字元集  
	zipFile newZipFile = zipOpen(zipFileName, APPEND_STATUS_CREATE); //創建zip文件  
	if (newZipFile == NULL)  
	{  
		::AfxMessageBox(("無法創建zip文件!"));  
		return;  
	}  

	/*CString strZipFile = "";
	strZipFile.Format("%s\\%s",dirName,zipFileName);
	strZipFile.ReleaseBuffer();
    TCHAR szDrive[_MAX_DRIVE],szDir[_MAX_DIR], szName[_MAX_FNAME], szExt[_MAX_EXT];

	bool a = false;

	_tsplitpath(strZipFile,szDrive,szDir,szName, szExt);  
//	strZipFile = "D:\\TestWrite\\222";
	//CFile::Remove(strZipFile);
//	a = RemoveDirectory(strZipFile);

	CollectFilesInDirToZip(newZipFile, dirName, ("GG"));  
	zipClose(newZipFile, NULL); //關閉zip文件  
} */	
//-------------------------------------------------------------------------------------//
#endif//ZLIB_USE