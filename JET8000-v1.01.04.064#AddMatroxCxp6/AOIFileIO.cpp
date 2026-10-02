// AOIFileIO.cpp: implementation of the CAOIFileIO class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "AOIFileIO.h"
//-------------------------------------------------------------------------------------//
#include "JetMemory.h"
//-------------------------------------------------------------------------------------//
//#include "AOIObjManager.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
const bool   EnableFileReadMemory = true;
const bool   EnableFileWriteMemory = true;
const size_t FileBufferSize=1024*1024*8;//1MBx8
//-------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
CAOIFileIO::CAOIFileIO()
{
	m_MapPos = 0;
	m_MapSize = 0;
	m_MapCount = 0;
	m_MapBufA = NULL;
	m_MapBufW = NULL;
	m_MapAllocByPoolA = false;
	m_MapAllocByPoolW = false;

	size_t sizeChunkINT    = sizeof(m_BinChunkINT);
	size_t sizeChunkDBL    = sizeof(m_BinChunkDBL);
	size_t sizeChunkINT64  = sizeof(m_BinChunkINT64);	
	size_t sizeChunkSTR016 = sizeof(m_BinChunkSTR016);
	size_t sizeChunkSTR032 = sizeof(m_BinChunkSTR032);
	size_t sizeChunkSTR064 = sizeof(m_BinChunkSTR064);
	size_t sizeChunkSTR128 = sizeof(m_BinChunkSTR128);
	size_t sizeChunkSTR256 = sizeof(m_BinChunkSTR256);
	size_t sizeChunkSTR512 = sizeof(m_BinChunkSTR512);

	
	m_FileMode = FILE_MODE_DUMMY;//FILE_MODE_TXT, FILE_MODE_BINARY, FILE_MODE_UNICODE
	m_FileTarget = FILE_TARGET_PROJECT;
	m_ChunkType = CHUNK_TYPE_INT;
	m_FileIOMode = FILE_IO_MODE_NONE;
	m_FileReadMode = FILE_READ_HARD_DISK;
	m_FileWriteMode = FILE_WRITE_HARD_DISK;
	m_LoadWStr = false;
	m_SaveWStr = false;

	::memset(&m_BinChunkINT, 0x00, sizeof(m_BinChunkINT));
	::memset(&m_BinChunkDBL, 0x00, sizeof(m_BinChunkDBL));
	::memset(&m_BinChunkINT64, 0x00, sizeof(sizeChunkINT64));	
	::memset(&m_BinChunkSTR016, 0x00, sizeof(m_BinChunkSTR016));
	::memset(&m_BinChunkSTR032, 0x00, sizeof(m_BinChunkSTR032));
	::memset(&m_BinChunkSTR064, 0x00, sizeof(m_BinChunkSTR064));
	::memset(&m_BinChunkSTR128, 0x00, sizeof(m_BinChunkSTR128));
	::memset(&m_BinChunkSTR256, 0x00, sizeof(m_BinChunkSTR256));
	::memset(&m_BinChunkSTR512, 0x00, sizeof(m_BinChunkSTR512));

	::memset(&m_BinChunkWSTR016, 0x00, sizeof(m_BinChunkWSTR016));
	::memset(&m_BinChunkWSTR032, 0x00, sizeof(m_BinChunkWSTR032));
	::memset(&m_BinChunkWSTR064, 0x00, sizeof(m_BinChunkWSTR064));
	::memset(&m_BinChunkWSTR128, 0x00, sizeof(m_BinChunkWSTR128));
	::memset(&m_BinChunkWSTR256, 0x00, sizeof(m_BinChunkWSTR256));
	::memset(&m_BinChunkWSTR512, 0x00, sizeof(m_BinChunkWSTR512));	

	m_TextSize = sizeof(m_TextLine);//2048;
	::memset(m_TextLine, 0x00, sizeof(m_TextLine));
	::memset(m_IndexLine, 0x00, sizeof(m_IndexLine));
	::memset(m_DataLine, 0x00, sizeof(m_DataLine));	

	m_wTextSize = sizeof(m_wTextLine);//2048;
	::memset(m_wTextLine, 0x00, sizeof(m_wTextLine));
	::memset(m_wIndexLine, 0x00, sizeof(m_wIndexLine));
	::memset(m_wDataLine, 0x00, sizeof(m_wDataLine));	


	m_RowCount = 0;
	m_TotalNodes = 0;
	m_NNodeCount = 0;	

	m_TotalModelObjs = 0;
	m_TotalComponents = 0;
	m_FilePtr = NULL;
}
//-------------------------------------------------------------------------------------//
CAOIFileIO::~CAOIFileIO()
{
	CloseFile();	
	ReleaseMapBuf();

	if ( m_ProgressWnd.GetSafeHwnd() != NULL )
	{	m_ProgressWnd.DestroyWindow(); }	
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::CreateProgressWnd()
{
	if ( m_ProgressWnd.GetSafeHwnd() != NULL ) { return true; }

	RECT MainRect = {0};
	CFrameWnd *pMainWnd = (CFrameWnd*)::AfxGetMainWnd();	
	if ( NULL == pMainWnd )
	{	return true; }

	CControlBar *pStatusBar = pMainWnd->GetControlBar(AFX_IDW_STATUS_BAR);	
	if ( pStatusBar != NULL && pStatusBar->GetSafeHwnd() != NULL )
	{
		pStatusBar->GetWindowRect(&MainRect);
		pMainWnd->ScreenToClient(&MainRect);
	}
	else
	{
		pMainWnd->GetClientRect(&MainRect);
		MainRect.top = MainRect.bottom - 24;
	}

	m_ProgressWnd.Create(WS_CHILD|WS_VISIBLE, MainRect, pMainWnd, NULL); 
	m_ProgressWnd.SetRange32(0, 100);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::DestroyProgressWnd()
{
	if ( m_ProgressWnd.GetSafeHwnd() == NULL ) { return true; }
	m_ProgressWnd.DestroyWindow();
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIFileIO::SetFnName(LPCTSTR str)
{
	m_FnName = str;
}
//-------------------------------------------------------------------------------------//
inline void CAOIFileIO::SetErrorString(LPCTSTR str)
{
	m_ErrorString = str;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIFileIO::GetErrorString()
{
	return m_ErrorString;
}
//-------------------------------------------------------------------------------------//
inline void CAOIFileIO::SetFileMode(FILE_MODE Mode)
{
	m_FileMode = Mode;
}
//-------------------------------------------------------------------------------------//
inline void CAOIFileIO::SetFileIOMode(FILE_IO_MODE Mode)
{
	m_FileIOMode = Mode;
}
//-------------------------------------------------------------------------------------//
inline void CAOIFileIO::SetFileReadMode(FILE_READ_MODE Mode)
{
	m_FileReadMode = Mode;
}
//-------------------------------------------------------------------------------------//
inline void CAOIFileIO::SetFileWriteMode(FILE_WRITE_MODE Mode)
{
	m_FileWriteMode = Mode;
}
//-------------------------------------------------------------------------------------//
inline void CAOIFileIO::SetLoadWStr(bool val)
{
	m_LoadWStr = val;
}
//-------------------------------------------------------------------------------------//
inline void CAOIFileIO::SetSaveWStr(bool val)
{
	m_SaveWStr = val;
}
//-------------------------------------------------------------------------------------//
inline bool CAOIFileIO::CheckFileMode(FILE_MODE Mode)
{
	if ( Mode != m_FileMode )
	{
		SetErrorString(_T("File Mode Exception"));
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CAOIFileIO::CheckFileIOMode(FILE_IO_MODE Mode)//確定檔案存取模式
{
	if ( Mode != m_FileIOMode )
	{
		CAOIFileIO::SetErrorString(_T("File IO Mode Exception"));
		JetAPI::ShowMessageBox(m_ErrorString);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CAOIFileIO::CheckMapBufA()//確認緩存記憶體
{
	if ( NULL == m_MapBufA )
	{
		CAOIFileIO::SetErrorString(_T("File Map Buffer Exception"));
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CAOIFileIO::CheckMapBufW()//確認緩存記憶體
{
	if ( NULL == m_MapBufW )
	{
		CAOIFileIO::SetErrorString(_T("File Map Buffer Exception"));
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CAOIFileIO::CheckTxtPrintf(int val)//確認Txt存檔結果
{
	if ( val > 0 ) { return true; }
	return false;
}
//-------------------------------------------------------------------------------------//
inline bool CAOIFileIO::CheckUtfPrintf(int val)//確認Utf存檔結果
{
	if ( val > 0 ) { return true; }
	return false;
}
//-------------------------------------------------------------------------------------//
inline bool CAOIFileIO::CheckTxtBufPrintf(int val)//確認Txt緩存結果
{
	if ( val > 0 ) { return true; }
	return false;
}
//-------------------------------------------------------------------------------------//
inline bool CAOIFileIO::CheckUtfBufPrintf(int val)//確認Utf緩存結果
{
	if ( val > 0 ) { return true; }
	return false;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::SaveBinChunk_BOL(FILE *pfile, int index, bool value)
{
#ifdef _DEBUG
	if ( CAOIFileIO::CheckFileMode(FILE_MODE_BINARY) == false ) { return false; }
#endif//_DEBUG

//	m_FileIOError.SetFnName(_T("CAOIFileIO::SaveBinChunk_BOL"));
	size_t Res = 0;
	m_ChunkType = CHUNK_TYPE_INT;
	Res = ::fwrite(&m_ChunkType, sizeof(m_ChunkType), 1, pfile);
	if ( Res != 1 ) { return false; }

	m_BinChunkINT.index = index;
	if ( false == value )
	{	m_BinChunkINT.value = 0; }
	else
	{	m_BinChunkINT.value = 1; }
	Res = ::fwrite(&m_BinChunkINT, sizeof(m_BinChunkINT), 1, pfile);
	if ( Res != 1 ) { return false; }

	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::SaveBinChunk_INT(FILE *pfile, int index, int value)
{
#ifdef _DEBUG
	if ( CAOIFileIO::CheckFileMode(FILE_MODE_BINARY) == false ) { return false; }
#endif//_DEBUG
//	m_FileIOError.SetFnName(_T("CAOIFileIO::SaveBinChunk_INT"));

	size_t Res = 0;
	m_ChunkType = CHUNK_TYPE_INT;
	Res = ::fwrite(&m_ChunkType, sizeof(m_ChunkType), 1, pfile);
	if ( Res != 1 ) { return false; }

	m_BinChunkINT.index = index;
	m_BinChunkINT.value = value;
	Res = ::fwrite(&m_BinChunkINT, sizeof(m_BinChunkINT), 1, pfile);
	if ( Res != 1 ) { return false; }

	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::SaveBinChunk_DBL(FILE *pfile, int index, double value)
{
#ifdef _DEBUG
	if ( CAOIFileIO::CheckFileMode(FILE_MODE_BINARY) == false ) { return false; }
#endif//_DEBUG
//	m_FileIOError.SetFnName(_T("CAOIFileIO::SaveBinChunk_DBL"));

	size_t Res = 0;
	m_ChunkType = CHUNK_TYPE_DBL;
	Res = ::fwrite(&m_ChunkType, sizeof(m_ChunkType), 1, pfile);
	if ( Res != 1 ) { return false; }

	m_BinChunkDBL.index = index;
	m_BinChunkDBL.value = value;
	Res = ::fwrite(&m_BinChunkDBL, sizeof(m_BinChunkDBL), 1, pfile);
	if ( Res != 1 ) { return false; }
	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::SaveBinChunk_STR(FILE *pfile, int index, const char *value)
{
#ifdef _DEBUG
	if ( CAOIFileIO::CheckFileMode(FILE_MODE_BINARY) == false ) { return false; }
#endif//_DEBUG
//	m_FileIOError.SetFnName(_T("CAOIFileIO::SaveBinChunk_STR"));

	bool IsOK = true;
	const size_t Len = (::strlen(value)+1)*sizeof(char);
	const size_t SizeStr016 = sizeof(m_BinChunkSTR016.value);
	const size_t SizeStr032 = sizeof(m_BinChunkSTR032.value);
	const size_t SizeStr064 = sizeof(m_BinChunkSTR064.value);
	const size_t SizeStr128 = sizeof(m_BinChunkSTR128.value);
	const size_t SizeStr256 = sizeof(m_BinChunkSTR256.value);
	const size_t SizeStr512 = sizeof(m_BinChunkSTR512.value);

	if ( Len < SizeStr016 )
	{	IsOK = CAOIFileIO::SaveBinChunk_STR_016(pfile, index, value);	}
	else if ( Len < SizeStr032 )
	{	IsOK = CAOIFileIO::SaveBinChunk_STR_032(pfile, index, value);	}
	else if ( Len < SizeStr064 )
	{	IsOK = CAOIFileIO::SaveBinChunk_STR_064(pfile, index, value);	}
	else if ( Len < SizeStr128 )
	{	IsOK = CAOIFileIO::SaveBinChunk_STR_128(pfile, index, value);	}
	else if ( Len < SizeStr256 )
	{	IsOK = CAOIFileIO::SaveBinChunk_STR_256(pfile, index, value);	}
	else if ( Len < SizeStr512 )
	{	IsOK = CAOIFileIO::SaveBinChunk_STR_512(pfile, index, value);	}
	else
	{
		IsOK = false;
		CString str;
		str.Format(_T("Error, String length out of limit(%d)"), Len);
		CAOIFileIO::SetErrorString(str);
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::SaveBinChunk_INT64(FILE *pfile, int index, __int64 value)
{
#ifdef _DEBUG
	if ( CAOIFileIO::CheckFileMode(FILE_MODE_BINARY) == false ) { return false; }
#endif//_DEBUG
//	m_FileIOError.SetFnName(_T("CAOIFileIO::SaveBinChunk_INT64"));

	size_t Res = 0;
	m_ChunkType = CHUNK_TYPE_INT_64;
	Res = ::fwrite(&m_ChunkType, sizeof(m_ChunkType), 1, pfile);
	if ( Res != 1 ) { return false; }

	m_BinChunkINT64.index = index;
	m_BinChunkINT64.value = value;
	Res = ::fwrite(&m_BinChunkINT64, sizeof(m_BinChunkINT64), 1, pfile);
	if ( Res != 1 ) { return false; }

	return true;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::SaveBinChunk_STR_016(FILE *pfile, int index, const char *value)
{
//	m_FileIOError.SetFnName(_T("CAOIFileIO::SaveBinChunk_STR_016"));

	size_t Res = 0;
	m_ChunkType = CHUNK_TYPE_STR_016;
	Res = ::fwrite(&m_ChunkType, sizeof(m_ChunkType), 1, pfile);
	if ( Res != 1 ) { return false; }

	::memset(&m_BinChunkSTR016, 0x00, sizeof(m_BinChunkSTR016));
	m_BinChunkSTR016.index = index;

	size_t size = sizeof(m_BinChunkSTR016.value);
	size_t len  = ::strlen(value)+1;	
//#ifdef  UNICODE
//	if ( len > size )
//	{	len = size; }
//	::wcstombs(m_BinChunkSTR016.value, value, len);
//#else
	if ( len > size )
	{	::memcpy(m_BinChunkSTR016.value, value, size);  }
	else
	{	::strcpy(m_BinChunkSTR016.value, value); }
//#endif
	Res = ::fwrite(&m_BinChunkSTR016, sizeof(m_BinChunkSTR016), 1, pfile);
	if ( Res != 1 ) { return false; }

	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::SaveBinChunk_STR_032(FILE *pfile, int index, const char *value)
{
//	m_FileIOError.SetFnName(_T("CAOIFileIO::SaveBinChunk_STR_032"));

	size_t Res = 0;
	m_ChunkType = CHUNK_TYPE_STR_032;
	Res = ::fwrite(&m_ChunkType, sizeof(m_ChunkType), 1, pfile);
	if ( Res != 1 ) { return false; }

	::memset(&m_BinChunkSTR032, 0x00, sizeof(m_BinChunkSTR032));
	m_BinChunkSTR032.index = index;

	size_t size = sizeof(m_BinChunkSTR032.value);
	size_t len  = ::strlen(value)+1;	
//#ifdef  UNICODE
//	if ( len > size )
//	{	len = size; }
//	::wcstombs(m_BinChunkSTR032.value, value, len);
//#else
	if ( len > size )
	{	::memcpy(m_BinChunkSTR032.value, value, size);  }
	else
	{	::strcpy(m_BinChunkSTR032.value, value); }
//#endif
	Res = ::fwrite(&m_BinChunkSTR032, sizeof(m_BinChunkSTR032), 1, pfile);
	if ( Res != 1 ) { return false; }

	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::SaveBinChunk_STR_064(FILE *pfile, int index, const char *value)
{
//	m_FileIOError.SetFnName(_T("CAOIFileIO::SaveBinChunk_STR_064"));

	size_t Res = 0;
	m_ChunkType = CHUNK_TYPE_STR_064;
	Res = ::fwrite(&m_ChunkType, sizeof(m_ChunkType), 1, pfile);
	if ( Res != 1 ) { return false; }

	::memset(&m_BinChunkSTR064, 0x00, sizeof(m_BinChunkSTR064));
	m_BinChunkSTR064.index = index;

	size_t size = sizeof(m_BinChunkSTR064.value);
	size_t len  = ::strlen(value)+1;	
//#ifdef  UNICODE
//	if ( len > size )
//	{	len = size; }
//	::wcstombs(m_BinChunkSTR064.value, value, len);
//#else
	if ( len > size )
	{	::memcpy(m_BinChunkSTR064.value, value, size);  }
	else
	{	::strcpy(m_BinChunkSTR064.value, value); }
//#endif
	Res = ::fwrite(&m_BinChunkSTR064, sizeof(m_BinChunkSTR064), 1, pfile);
	if ( Res != 1 ) { return false; }

	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::SaveBinChunk_STR_128(FILE *pfile, int index, const char *value)
{
//	m_FileIOError.SetFnName(_T("CAOIFileIO::SaveBinChunk_STR_128"));

	size_t Res = 0;
	m_ChunkType = CHUNK_TYPE_STR_128;
	Res = ::fwrite(&m_ChunkType, sizeof(m_ChunkType), 1, pfile);
	if ( Res != 1 ) { return false; }

	::memset(&m_BinChunkSTR128, 0x00, sizeof(m_BinChunkSTR128));
	m_BinChunkSTR128.index = index;

	size_t size = sizeof(m_BinChunkSTR128.value);
	size_t len  = ::strlen(value)+1;	
//#ifdef  UNICODE
//	if ( len > size )
//	{	len = size; }
//	::wcstombs(m_BinChunkSTR128.value, value, len);
//#else
	if ( len > size )
	{	::memcpy(m_BinChunkSTR128.value, value, size);  }
	else
	{	::strcpy(m_BinChunkSTR128.value, value); }
//#endif
	Res = ::fwrite(&m_BinChunkSTR128, sizeof(m_BinChunkSTR128), 1, pfile);
	if ( Res != 1 ) { return false; }

	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::SaveBinChunk_STR_256(FILE *pfile, int index, const char *value)
{
//	m_FileIOError.SetFnName(_T("CAOIFileIO::SaveBinChunk_STR_256"));

	size_t Res = 0;
	m_ChunkType = CHUNK_TYPE_STR_256;
	Res = ::fwrite(&m_ChunkType, sizeof(m_ChunkType), 1, pfile);
	if ( Res != 1 ) { return false; }

	::memset(&m_BinChunkSTR256, 0x00, sizeof(m_BinChunkSTR256));
	m_BinChunkSTR256.index = index;

	size_t size = sizeof(m_BinChunkSTR256.value);
	size_t len  = ::strlen(value)+1;	
//#ifdef  UNICODE
//	if ( len > size )
//	{	len = size; }
//	::wcstombs(m_BinChunkSTR256.value, value, len);
//#else
	if ( len > size )
	{	::memcpy(m_BinChunkSTR256.value, value, size);  }
	else
	{	::strcpy(m_BinChunkSTR256.value, value); }
//#endif
	Res = ::fwrite(&m_BinChunkSTR256, sizeof(m_BinChunkSTR256), 1, pfile);
	if ( Res != 1 ) { return false; }

	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::SaveBinChunk_STR_512(FILE *pfile, int index, const char *value)
{
//	m_FileIOError.SetFnName(_T("CAOIFileIO::SaveBinChunk_STR_512"));

	size_t Res = 0;
	m_ChunkType = CHUNK_TYPE_STR_512;
	Res = ::fwrite(&m_ChunkType, sizeof(m_ChunkType), 1, pfile);
	if ( Res != 1 ) { return false; }

	::memset(&m_BinChunkSTR512, 0x00, sizeof(m_BinChunkSTR512));
	m_BinChunkSTR512.index = index;

	size_t size = sizeof(m_BinChunkSTR512.value);
	size_t len  = ::strlen(value)+1;	
//#ifdef  UNICODE
//	if ( len > size )
//	{	len = size; }
//	::wcstombs(m_BinChunkSTR512.value, value, len);
//#else
	if ( len > size )
	{	::memcpy(m_BinChunkSTR512.value, value, size);  }
	else
	{	::strcpy(m_BinChunkSTR512.value, value); }
//#endif
	Res = ::fwrite(&m_BinChunkSTR512, sizeof(m_BinChunkSTR512), 1, pfile);
	if ( Res != 1 ) { return false; }

	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::SaveBinChunk_STR(FILE *pfile, int index, const wchar_t *value)
{
#ifdef _DEBUG
	if ( CAOIFileIO::CheckFileMode(FILE_MODE_BINARY) == false ) { return false; }
#endif//_DEBUG
//	m_FileIOError.SetFnName(_T("CAOIFileIO::SaveBinChunk_STR"));

	bool IsOK = true;
	const size_t Len = (::wcslen(value)+1)*sizeof(wchar_t);
	const size_t SizeStr016 = sizeof(m_BinChunkWSTR016.value);
	const size_t SizeStr032 = sizeof(m_BinChunkWSTR032.value);
	const size_t SizeStr064 = sizeof(m_BinChunkWSTR064.value);
	const size_t SizeStr128 = sizeof(m_BinChunkWSTR128.value);
	const size_t SizeStr256 = sizeof(m_BinChunkWSTR256.value);
	const size_t SizeStr512 = sizeof(m_BinChunkWSTR512.value);

	if ( Len < SizeStr016 )
	{	IsOK = CAOIFileIO::SaveBinChunk_WSTR_016(pfile, index, value);	}
	else if ( Len < SizeStr032 )
	{	IsOK = CAOIFileIO::SaveBinChunk_WSTR_032(pfile, index, value);	}
	else if ( Len < SizeStr064 )
	{	IsOK = CAOIFileIO::SaveBinChunk_WSTR_064(pfile, index, value);	}
	else if ( Len < SizeStr128 )
	{	IsOK = CAOIFileIO::SaveBinChunk_WSTR_128(pfile, index, value);	}
	else if ( Len < SizeStr256 )
	{	IsOK = CAOIFileIO::SaveBinChunk_WSTR_256(pfile, index, value);	}
	else if ( Len < SizeStr512 )
	{	IsOK = CAOIFileIO::SaveBinChunk_WSTR_512(pfile, index, value);	}
	else
	{
		IsOK = false;
		CString str;
		str.Format(_T("Error, String length out of limit(%d)"), Len);
		CAOIFileIO::SetErrorString(str);
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::SaveBinChunk_WSTR_016(FILE *pfile, int index, const wchar_t *value)
{
//	m_FileIOError.SetFnName(_T("CAOIFileIO::SaveBinChunk_WSTR_016"));

	size_t Res = 0;
	m_ChunkType = CHUNK_TYPE_WSTR_016;
	Res = ::fwrite(&m_ChunkType, sizeof(m_ChunkType), 1, pfile);
	if ( Res != 1 ) { return false; }

	::memset(&m_BinChunkWSTR016, 0x00, sizeof(m_BinChunkWSTR016));
	m_BinChunkWSTR032.index = index;

	size_t size = sizeof(m_BinChunkWSTR016.value);
	size_t len  = ::wcslen(value)+1;	
//#ifdef  UNICODE
//	if ( len > size )
//	{	len = size; }
//	::wcstombs(m_BinChunkWSTR016.value, value, len);
//#else
	if ( len > size )
	{	::memcpy(m_BinChunkWSTR016.value, value, size);  }
	else
	{	::wcscpy(m_BinChunkWSTR016.value, value); }
//#endif
	Res = ::fwrite(&m_BinChunkWSTR016, sizeof(m_BinChunkWSTR016), 1, pfile);
	if ( Res != 1 ) { return false; }

	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::SaveBinChunk_WSTR_032(FILE *pfile, int index, const wchar_t *value)
{
//	m_FileIOError.SetFnName(_T("CAOIFileIO::SaveBinChunk_WSTR_032"));

	size_t Res = 0;
	m_ChunkType = CHUNK_TYPE_WSTR_032;
	Res = ::fwrite(&m_ChunkType, sizeof(m_ChunkType), 1, pfile);
	if ( Res != 1 ) { return false; }

	::memset(&m_BinChunkWSTR032, 0x00, sizeof(m_BinChunkWSTR032));
	m_BinChunkWSTR032.index = index;

	size_t size = sizeof(m_BinChunkWSTR032.value);
	size_t len  = ::wcslen(value)+1;	
//#ifdef  UNICODE
//	if ( len > size )
//	{	len = size; }
//	::wcstombs(m_BinChunkWSTR032.value, value, len);
//#else
	if ( len > size )
	{	::memcpy(m_BinChunkWSTR032.value, value, size);  }
	else
	{	::wcscpy(m_BinChunkWSTR032.value, value); }
//#endif
	Res = ::fwrite(&m_BinChunkWSTR032, sizeof(m_BinChunkWSTR032), 1, pfile);
	if ( Res != 1 ) { return false; }

	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::SaveBinChunk_WSTR_064(FILE *pfile, int index, const wchar_t *value)
{
//	m_FileIOError.SetFnName(_T("CAOIFileIO::SaveBinChunk_WSTR_064"));

	size_t Res = 0;
	m_ChunkType = CHUNK_TYPE_WSTR_064;
	Res = ::fwrite(&m_ChunkType, sizeof(m_ChunkType), 1, pfile);
	if ( Res != 1 ) { return false; }

	::memset(&m_BinChunkWSTR064, 0x00, sizeof(m_BinChunkWSTR064));
	m_BinChunkWSTR064.index = index;

	size_t size = sizeof(m_BinChunkWSTR064.value);
	size_t len  = ::wcslen(value)+1;	
//#ifdef  UNICODE
//	if ( len > size )
//	{	len = size; }
//	::wcstombs(m_BinChunkWSTR064.value, value, len);
//#else
	if ( len > size )
	{	::memcpy(m_BinChunkWSTR064.value, value, size);  }
	else
	{	::wcscpy(m_BinChunkWSTR064.value, value); }
//#endif
	Res = ::fwrite(&m_BinChunkWSTR064, sizeof(m_BinChunkWSTR064), 1, pfile);
	if ( Res != 1 ) { return false; }

	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::SaveBinChunk_WSTR_128(FILE *pfile, int index, const wchar_t *value)
{
//	m_FileIOError.SetFnName(_T("CAOIFileIO::SaveBinChunk_WSTR_128"));

	size_t Res = 0;
	m_ChunkType = CHUNK_TYPE_WSTR_128;
	Res = ::fwrite(&m_ChunkType, sizeof(m_ChunkType), 1, pfile);
	if ( Res != 1 ) { return false; }

	::memset(&m_BinChunkWSTR128, 0x00, sizeof(m_BinChunkWSTR128));
	m_BinChunkWSTR128.index = index;

	size_t size = sizeof(m_BinChunkWSTR128.value);
	size_t len  = ::wcslen(value)+1;	
//#ifdef  UNICODE
//	if ( len > size )
//	{	len = size; }
//	::wcstombs(m_BinChunkWSTR128.value, value, len);
//#else
	if ( len > size )
	{	::memcpy(m_BinChunkWSTR128.value, value, size);  }
	else
	{	::wcscpy(m_BinChunkWSTR128.value, value); }
//#endif
	Res = ::fwrite(&m_BinChunkWSTR128, sizeof(m_BinChunkWSTR128), 1, pfile);
	if ( Res != 1 ) { return false; }

	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::SaveBinChunk_WSTR_256(FILE *pfile, int index, const wchar_t *value)
{
//	m_FileIOError.SetFnName(_T("CAOIFileIO::SaveBinChunk_WSTR_256"));

	size_t Res = 0;
	m_ChunkType = CHUNK_TYPE_WSTR_256;
	Res = ::fwrite(&m_ChunkType, sizeof(m_ChunkType), 1, pfile);
	if ( Res != 1 ) { return false; }

	::memset(&m_BinChunkWSTR256, 0x00, sizeof(m_BinChunkWSTR256));
	m_BinChunkWSTR256.index = index;

	size_t size = sizeof(m_BinChunkWSTR256.value);
	size_t len  = ::wcslen(value)+1;	
//#ifdef  UNICODE
//	if ( len > size )
//	{	len = size; }
//	::wcstombs(m_BinChunkWSTR256.value, value, len);
//#else
	if ( len > size )
	{	::memcpy(m_BinChunkWSTR256.value, value, size);  }
	else
	{	::wcscpy(m_BinChunkWSTR256.value, value); }
//#endif
	Res = ::fwrite(&m_BinChunkWSTR256, sizeof(m_BinChunkWSTR256), 1, pfile);
	if ( Res != 1 ) { return false; }

	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::SaveBinChunk_WSTR_512(FILE *pfile, int index, const wchar_t *value)
{
//	m_FileIOError.SetFnName(_T("CAOIFileIO::SaveBinChunk_WSTR_512"));

	size_t Res = 0;
	m_ChunkType = CHUNK_TYPE_WSTR_512;
	Res = ::fwrite(&m_ChunkType, sizeof(m_ChunkType), 1, pfile);
	if ( Res != 1 ) { return false; }

	::memset(&m_BinChunkWSTR512, 0x00, sizeof(m_BinChunkWSTR512));
	m_BinChunkWSTR512.index = index;

	size_t size = sizeof(m_BinChunkWSTR512.value);
	size_t len  = ::wcslen(value)+1;	
//#ifdef  UNICODE
//	if ( len > size )
//	{	len = size; }
//	::wcstombs(m_BinChunkWSTR512.value, value, len);
//#else
	if ( len > size )
	{	::memcpy(m_BinChunkWSTR512.value, value, size);  }
	else
	{	::wcscpy(m_BinChunkWSTR512.value, value); }
//#endif
	Res = ::fwrite(&m_BinChunkWSTR512, sizeof(m_BinChunkWSTR512), 1, pfile);
	if ( Res != 1 ) { return false; }

	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::LoadBinChunk(FILE *pfile, int &index, int &n32, double &f64, char *sValue, wchar_t *wValue, __int64 &n64)
{
#ifdef _DEBUG
	if ( CheckFileMode(FILE_MODE_BINARY) == false ) { return false; }
#endif//_DEBUG
//	m_FileIOError.SetFnName(_T("CAOIFileIO::LoadBinChunk"));
	size_t Res = ::fread(&m_ChunkType, sizeof(m_ChunkType), 1, pfile);
	if ( Res != 1 ) 
	{ 	
		CAOIFileIO::SetErrorString(_T("Error, Read Chunk Type Fault"));
		return false; 
	}
	__int64 nValue64=0;
	switch ( m_ChunkType )
	{
	case CHUNK_TYPE_INT:
		if ( LoadBinChunk_INT(pfile, index, n32) == false )
		{	return false; }
		break;
	case CHUNK_TYPE_INT_64:
		if ( LoadBinChunk_INT64(pfile, index, n64) == false )
		{	return false; }
		break;
	case CHUNK_TYPE_DBL:
		if ( LoadBinChunk_DBL(pfile, index, f64) == false )
		{	return false; }
		break;
	case CHUNK_TYPE_STR_016:
		if ( LoadBinChunk_STR_016(pfile, index, sValue) == false )
		{	return false; }
		break;
	case CHUNK_TYPE_WSTR_016:
		if ( LoadBinChunk_WSTR_016(pfile, index, wValue) == false )
		{	return false; }
		break;
	case CHUNK_TYPE_STR_032:
		if ( LoadBinChunk_STR_032(pfile, index, sValue) == false )
		{	return false; }
		break;
	case CHUNK_TYPE_WSTR_032:
		if ( CAOIFileIO::LoadBinChunk_WSTR_032(pfile, index, wValue) == false )
		{	return false; }
		break;
	case CHUNK_TYPE_STR_064:
		if ( LoadBinChunk_STR_064(pfile, index, sValue) == false )
		{	return false; }
		break;
	case CHUNK_TYPE_WSTR_064:
		if ( LoadBinChunk_WSTR_064(pfile, index, wValue) == false )
		{	return false; }
		break;
	case CHUNK_TYPE_STR_128:
		if ( LoadBinChunk_STR_128(pfile, index, sValue) == false )
		{	return false; }
		break;
	case CHUNK_TYPE_WSTR_128:
		if ( LoadBinChunk_WSTR_128(pfile, index, wValue) == false )
		{	return false; }
		break;
	case CHUNK_TYPE_STR_256:
		if ( CAOIFileIO::LoadBinChunk_STR_256(pfile, index, sValue) == false )
		{	return false; }
		break;
	case CHUNK_TYPE_WSTR_256:
		if ( LoadBinChunk_WSTR_256(pfile, index, wValue) == false )
		{	return false; }
		break;
	case CHUNK_TYPE_STR_512:
		if ( LoadBinChunk_STR_512(pfile, index, sValue) == false )
		{	return false; }
		break;
	case CHUNK_TYPE_WSTR_512:
		if ( LoadBinChunk_WSTR_512(pfile, index, wValue) == false )
		{	return false; }
		break;
	default:
		{
			CString str;
			str.Format(_T("Error No Defined Chunk Type (%d)"), m_ChunkType);
			CAOIFileIO::SetErrorString(str);
		}
		return false;
		break;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CAOIFileIO::LoadBinChunk_INT(FILE *pfile, int &index, int &value)
{
//	m_FileIOError.SetFnName(_T("CAOIFileIO::LoadBinChunk_INT"));

	size_t Res = 0;
	Res = ::fread(&m_BinChunkINT, sizeof(m_BinChunkINT), 1, pfile);
	if ( Res != 1 ) 
	{	return false;	}
	SetLoadWStr(false);
	index = m_BinChunkINT.index;
	value = m_BinChunkINT.value;
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CAOIFileIO::LoadBinChunk_DBL(FILE *pfile, int &index, double &value)
{
//	m_FileIOError.SetFnName(_T("CAOIFileIO::LoadBinChunk_DBL"));

	size_t Res = 0;
	Res = ::fread(&m_BinChunkDBL, sizeof(m_BinChunkDBL), 1, pfile);
	if ( Res != 1 ) { return false; }
	SetLoadWStr(false);
	index = m_BinChunkDBL.index;
	value = m_BinChunkDBL.value;
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CAOIFileIO::LoadBinChunk_INT64(FILE *pfile, int &index, __int64 &value)
{
//	m_FileIOError.SetFnName(_T("CAOIFileIO::LoadBinChunk_INT64"));

	size_t Res = 0;
	Res = ::fread(&m_BinChunkINT64, sizeof(m_BinChunkINT64), 1, pfile);
	if ( Res != 1 ) 
	{	return false;	}
	SetLoadWStr(false);
	index = m_BinChunkINT64.index;
	value = m_BinChunkINT64.value;	
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CAOIFileIO::LoadBinChunk_STR_016(FILE *pfile, int &index, char *value)
{
//	m_FileIOError.SetFnName(_T("CAOIFileIO::LoadBinChunk_STR_016"));

	size_t Res = 0;
	Res = ::fread(&m_BinChunkSTR016, sizeof(m_BinChunkSTR016), 1, pfile);
	if ( Res != 1 ) { return false; }
	SetLoadWStr(false);
	index = m_BinChunkSTR016.index;

//#ifdef  UNICODE
//	size_t len  = ::strlen(m_BinChunkSTR016.value)+1;
//	::mbstowcs(value, m_BinChunkSTR016.value, size);	
//#else
	::strcpy(value, m_BinChunkSTR016.value);
//#endif
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CAOIFileIO::LoadBinChunk_STR_032(FILE *pfile, int &index, char *value)
{
//	m_FileIOError.SetFnName(_T("CAOIFileIO::LoadBinChunk_STR_032"));

	size_t Res = 0;
	Res = ::fread(&m_BinChunkSTR032, sizeof(m_BinChunkSTR032), 1, pfile);
	if ( Res != 1 ) { return false; }
	SetLoadWStr(false);
	index = m_BinChunkSTR032.index;

//#ifdef  UNICODE
//	size_t len  = ::strlen(m_BinChunkSTR032.value)+1;
//	::mbstowcs(value, m_BinChunkSTR032.value, size);	
//#else
	::strcpy(value, m_BinChunkSTR032.value);
//#endif
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CAOIFileIO::LoadBinChunk_STR_064(FILE *pfile, int &index, char *value)
{
//	m_FileIOError.SetFnName(_T("CAOIFileIO::LoadBinChunk_STR_064"));

	size_t Res = 0;
	Res = ::fread(&m_BinChunkSTR064, sizeof(m_BinChunkSTR064), 1, pfile);
	if ( Res != 1 ) { return false; }
	SetLoadWStr(false);
	index = m_BinChunkSTR064.index;

//#ifdef  UNICODE
//	size_t len  = ::strlen(m_BinChunkSTR064.value)+1;
//	::mbstowcs(value, m_BinChunkSTR064.value, size);	
//#else
	::strcpy(value, m_BinChunkSTR064.value);
//#endif
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CAOIFileIO::LoadBinChunk_STR_128(FILE *pfile, int &index, char *value)
{
//	m_FileIOError.SetFnName(_T("CAOIFileIO::LoadBinChunk_STR_128"));

	size_t Res = 0;
	Res = ::fread(&m_BinChunkSTR128, sizeof(m_BinChunkSTR128), 1, pfile);
	if ( Res != 1 ) { return false; }
	SetLoadWStr(false);
	index = m_BinChunkSTR128.index;

//#ifdef  UNICODE
//	size_t len  = ::strlen(m_BinChunkSTR128.value)+1;
//	::mbstowcs(value, m_BinChunkSTR128.value, size);	
//#else
	::strcpy(value, m_BinChunkSTR128.value);
//#endif
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CAOIFileIO::LoadBinChunk_STR_256(FILE *pfile, int &index, char *value)
{
//	m_FileIOError.SetFnName(_T("CAOIFileIO::LoadBinChunk_STR_256"));

	size_t Res = 0;
	Res = ::fread(&m_BinChunkSTR256, sizeof(m_BinChunkSTR256), 1, pfile);
	if ( Res != 1 ) { return false; }
	SetLoadWStr(false);
	index = m_BinChunkSTR256.index;

//#ifdef  UNICODE
//	size_t len  = ::strlen(m_BinChunkSTR256.value)+1;
//	::mbstowcs(value, m_BinChunkSTR256.value, size);	
//#else
	::strcpy(value, m_BinChunkSTR256.value);
//#endif
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CAOIFileIO::LoadBinChunk_STR_512(FILE *pfile, int &index, char *value)
{
//	m_FileIOError.SetFnName(_T("CAOIFileIO::LoadBinChunk_STR_512"));

	size_t Res = 0;
	Res = ::fread(&m_BinChunkSTR512, sizeof(m_BinChunkSTR512), 1, pfile);
	if ( Res != 1 ) { return false; }
	SetLoadWStr(false);
	index = m_BinChunkSTR512.index;

//#ifdef  UNICODE
//	size_t len  = ::strlen(m_BinChunkSTR512.value)+1;
//	::mbstowcs(value, m_BinChunkSTR512.value, size);	
//#else
	::strcpy(value, m_BinChunkSTR512.value);
//#endif
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CAOIFileIO::LoadBinChunk_WSTR_016(FILE *pfile, int &index, wchar_t *value)
{
//	m_FileIOError.SetFnName(_T("CAOIFileIO::LoadBinChunk_WSTR_016"));

	size_t Res = 0;
	Res = ::fread(&m_BinChunkWSTR016, sizeof(m_BinChunkWSTR016), 1, pfile);
	if ( Res != 1 ) { return false; }
	SetLoadWStr(true);
	index = m_BinChunkWSTR016.index;

//#ifdef  UNICODE
//	size_t len  = ::strlen(m_BinChunkWSTR016.value)+1;
//	::mbstowcs(value, m_BinChunkWSTR016.value, size);	
//#else
	::wcscpy(value, m_BinChunkWSTR016.value);
//#endif
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CAOIFileIO::LoadBinChunk_WSTR_032(FILE *pfile, int &index, wchar_t *value)
{
//	m_FileIOError.SetFnName(_T("CAOIFileIO::LoadBinChunk_WSTR_032"));

	size_t Res = 0;
	Res = ::fread(&m_BinChunkWSTR032, sizeof(m_BinChunkWSTR032), 1, pfile);
	if ( Res != 1 ) { return false; }
	SetLoadWStr(true);
	index = m_BinChunkWSTR032.index;

//#ifdef  UNICODE
//	size_t len  = ::strlen(m_BinChunkWSTR032.value)+1;
//	::mbstowcs(value, m_BinChunkWSTR032.value, size);	
//#else
	::wcscpy(value, m_BinChunkWSTR032.value);
//#endif
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CAOIFileIO::LoadBinChunk_WSTR_064(FILE *pfile, int &index, wchar_t *value)
{
//	m_FileIOError.SetFnName(_T("CAOIFileIO::LoadBinChunk_WSTR_064"));

	size_t Res = 0;
	Res = ::fread(&m_BinChunkWSTR064, sizeof(m_BinChunkWSTR064), 1, pfile);
	if ( Res != 1 ) { return false; }
	SetLoadWStr(true);
	index = m_BinChunkWSTR064.index;

//#ifdef  UNICODE
//	size_t len  = ::strlen(m_BinChunkWSTR064.value)+1;
//	::mbstowcs(value, m_BinChunkWSTR064.value, size);	
//#else
	::wcscpy(value, m_BinChunkWSTR064.value);
//#endif
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CAOIFileIO::LoadBinChunk_WSTR_128(FILE *pfile, int &index, wchar_t *value)
{
//	m_FileIOError.SetFnName(_T("CAOIFileIO::LoadBinChunk_WSTR_128"));

	size_t Res = 0;
	Res = ::fread(&m_BinChunkWSTR128, sizeof(m_BinChunkWSTR128), 1, pfile);
	if ( Res != 1 ) { return false; }
	SetLoadWStr(true);
	index = m_BinChunkWSTR128.index;

//#ifdef  UNICODE
//	size_t len  = ::strlen(m_BinChunkWSTR128.value)+1;
//	::mbstowcs(value, m_BinChunkWSTR128.value, size);	
//#else
	::wcscpy(value, m_BinChunkWSTR128.value);
//#endif
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CAOIFileIO::LoadBinChunk_WSTR_256(FILE *pfile, int &index, wchar_t *value)
{
//	m_FileIOError.SetFnName(_T("CAOIFileIO::LoadBinChunk_WSTR_256"));

	size_t Res = 0;
	Res = ::fread(&m_BinChunkWSTR256, sizeof(m_BinChunkWSTR256), 1, pfile);
	if ( Res != 1 ) { return false; }
	SetLoadWStr(true);
	index = m_BinChunkWSTR256.index;

//#ifdef  UNICODE
//	size_t len  = ::strlen(m_BinChunkWSTR256.value)+1;
//	::mbstowcs(value, m_BinChunkWSTR256.value, size);	
//#else
	::wcscpy(value, m_BinChunkWSTR256.value);
//#endif
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CAOIFileIO::LoadBinChunk_WSTR_512(FILE *pfile, int &index, wchar_t *value)
{
//	m_FileIOError.SetFnName(_T("CAOIFileIO::LoadBinChunk_WSTR_512"));

	size_t Res = 0;
	Res = ::fread(&m_BinChunkWSTR512, sizeof(m_BinChunkWSTR512), 1, pfile);
	if ( Res != 1 ) { return false; }
	SetLoadWStr(true);
	index = m_BinChunkWSTR512.index;

//#ifdef  UNICODE
//	size_t len  = ::strlen(m_BinChunkWSTR512.value)+1;
//	::mbstowcs(value, m_BinChunkWSTR512.value, size);	
//#else
	::wcscpy(value, m_BinChunkWSTR512.value);
//#endif
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CAOIFileIO::SaveTxtChunk_BOL(FILE *pfile, FILE_IO_ID index, bool value)
{	
#ifdef _DEBUG
	if ( CAOIFileIO::CheckFileMode(FILE_MODE_TXT) == false ) { return false; }
#endif//_DEBUG

	int nVal = 0;
	if ( true == value ) { nVal = 1; }
	else { nVal = 0; }
	int Res = ::fprintf(pfile, "%d, %d\n", index, nVal);
	if ( CheckTxtPrintf(Res) == false )
	{
		CString str;
		str.Format(_T("Error, Save (%d, %d) Fault"), index, nVal);
		CAOIFileIO::SetErrorString(str);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CAOIFileIO::SaveTxtChunk_INT(FILE *pfile, FILE_IO_ID index, int value)
{
#ifdef _DEBUG
	if ( CAOIFileIO::CheckFileMode(FILE_MODE_TXT) == false ) { return false; }
#endif//_DEBUG

	int Res = ::fprintf(pfile, "%d, %d\n", index, value);
	if ( CheckTxtPrintf(Res) == false )
	{
		CString str;
		str.Format(_T("Error, Save (%d, %d) Fault"), index, value);
		CAOIFileIO::SetErrorString(str);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CAOIFileIO::SaveTxtChunk_DBL(FILE *pfile, FILE_IO_ID index, double value)
{
#ifdef _DEBUG
	if ( CAOIFileIO::CheckFileMode(FILE_MODE_TXT) == false ) { return false; }
#endif//_DEBUG

	int Res = ::fprintf(pfile, "%d, %.16f\n", index, value);
	if ( CheckTxtPrintf(Res) == false )
	{
		CString str;
		str.Format(_T("Error, Save (%d, %.16f) Fault"), index, value);
		CAOIFileIO::SetErrorString(str);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CAOIFileIO::SaveTxtChunk_STR(FILE *pfile, FILE_IO_ID index, const char *value)
{
#ifdef _DEBUG
	if ( CAOIFileIO::CheckFileMode(FILE_MODE_TXT) == false ) { return false; }
#endif//_DEBUG

	int Res = ::fprintf(pfile, "%d, %s\n", index, value);
	if ( CheckTxtPrintf(Res) == false )
	{
		CString str;
		CString strVal=value;
		str.Format(_T("Error, Save (%d, %s) Fault"), index, value);
		CAOIFileIO::SetErrorString(str);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CAOIFileIO::SaveTxtChunk_STR(FILE *pfile, FILE_IO_ID index, const wchar_t *value)
{
#ifdef _DEBUG
	if ( CAOIFileIO::CheckFileMode(FILE_MODE_TXT) == false ) { return false; }
#endif//_DEBUG

	int Res = ::fwprintf(pfile, L"%d, %s\n", index, value);
	if ( CheckUtfPrintf(Res) == false )
	{
		CString str;
		CString strVal=value;
		str.Format(_T("Error, Save (%d, %s) Fault"), index, strVal);
		CAOIFileIO::SetErrorString(str);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CAOIFileIO::SaveTxtChunk_INT64(FILE *pfile, FILE_IO_ID index, __int64 value)
{
#ifdef _DEBUG
	if ( CAOIFileIO::CheckFileMode(FILE_MODE_TXT) == false ) { return false; }
#endif//_DEBUG

	int Res = ::fprintf(pfile, "%d, %I64d\n", index, value);
	if ( CheckTxtPrintf(Res) == false )
	{
		CString str;
		str.Format(_T("Error, Save (%d, %I64d) Fault"), index, value);
		CAOIFileIO::SetErrorString(str);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CAOIFileIO::SaveUtfChunk_BOL(FILE *pfile, FILE_IO_ID index, bool value)
{
#ifdef _DEBUG
	if ( CAOIFileIO::CheckFileMode(FILE_MODE_UNICODE) == false ) { return false; }
#endif//_DEBUG
	int nVal = 0;
	if ( true == value ) { nVal = 1; }
	else { nVal = 0; }
	int Res = ::fwprintf(pfile, L"%d, %d\n", index, nVal);
	if ( CheckUtfPrintf(Res) == false )
	{
		CString str;		
		str.Format(_T("Error, Save (%d, %d) Fault"), index, nVal);
		CAOIFileIO::SetErrorString(str);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CAOIFileIO::SaveUtfChunk_INT(FILE *pfile, FILE_IO_ID index, int value)
{
#ifdef _DEBUG
	if ( CAOIFileIO::CheckFileMode(FILE_MODE_UNICODE) == false ) { return false; }
#endif//_DEBUG

	int Res = ::fwprintf(pfile, L"%d, %d\n", index, value);
	if ( CheckUtfPrintf(Res) == false )
	{
		CString str;		
		str.Format(_T("Error, Save (%d, %d) Fault"), index, value);
		CAOIFileIO::SetErrorString(str);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CAOIFileIO::SaveUtfChunk_DBL(FILE *pfile, FILE_IO_ID index, double value)
{
#ifdef _DEBUG
	if ( CAOIFileIO::CheckFileMode(FILE_MODE_UNICODE) == false ) { return false; }
#endif//_DEBUG

	int Res = ::fwprintf(pfile, L"%d, %.16f\n", index, value);
	if ( CheckUtfPrintf(Res) == false )
	{
		CString str;		
		str.Format(_T("Error, Save (%d, %.16f) Fault"), index, value);
		CAOIFileIO::SetErrorString(str);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CAOIFileIO::SaveUtfChunk_STR(FILE *pfile, FILE_IO_ID index, const char *value)
{
#ifdef _DEBUG
	if ( CAOIFileIO::CheckFileMode(FILE_MODE_UNICODE) == false ) { return false; }
#endif//_DEBUG

	int Res = ::fprintf(pfile, "%d, %s\n", index, value);
	if ( CheckTxtPrintf(Res) == false )
	{
		CString str;
		CString strVal=value;
		str.Format(_T("Error, Save (%d, %s) Fault"), index, strVal);
		CAOIFileIO::SetErrorString(str);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CAOIFileIO::SaveUtfChunk_STR(FILE *pfile, FILE_IO_ID index, const wchar_t *value)
{
#ifdef _DEBUG
	if ( CAOIFileIO::CheckFileMode(FILE_MODE_UNICODE) == false ) { return false; }
#endif//_DEBUG

	int Res = ::fwprintf(pfile, L"%d, %s\n", index, value);
	if ( CheckUtfPrintf(Res) == false )
	{
		CString str;
		CString strVal=value;
		str.Format(_T("Error, Save (%d, %s) Fault"), index, strVal);
		CAOIFileIO::SetErrorString(str);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CAOIFileIO::SaveUtfChunk_INT64(FILE *pfile, FILE_IO_ID index, __int64 value)
{
#ifdef _DEBUG
	if ( CAOIFileIO::CheckFileMode(FILE_MODE_UNICODE) == false ) { return false; }
#endif//_DEBUG

	int Res = ::fwprintf(pfile, L"%d, %I64d\n", index, value);
	if ( CheckUtfPrintf(Res) == false )
	{
		CString str;		
		str.Format(_T("Error, Save (%d, %I64d) Fault"), index, value);
		CAOIFileIO::SetErrorString(str);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::FlushMapBufA(FILE *pfile)
{	
#ifdef _DEBUG
	if ( CAOIFileIO::CheckMapBufA() == false ) { return false; }	
#endif//_DEBUG		
	int Res = ::fwrite(m_MapBufA, sizeof(m_MapBufA[0]), m_MapPos, pfile);
	m_MapCount += m_MapPos;	
	if ( CheckTxtPrintf(Res) == false )
	{
		CString str;		
		str = _T("Error, Flush BufferA to File Fault");
		CAOIFileIO::SetErrorString(str);
		return false;
	}
	if ( Res != m_MapPos )
	{
		CString str;		
		str = _T("Error, Flush BufferA to File Fault");
		CAOIFileIO::SetErrorString(str);
		return false;
	}
	::memset(m_MapBufA, 0x00, sizeof(m_MapBufA[0])*m_MapPos);
	m_MapPos = 0;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::AddMapBufA(FILE *pfile, const char *Buf, size_t Size)//加入內存緩衝
{
#ifdef _DEBUG
	if ( CAOIFileIO::CheckMapBufA() == false ) { return false; }	
#endif//_DEBUG
	size_t EndPos=m_MapPos+Size;
	if ( EndPos >= m_MapSize )
	{
		if ( FlushMapBufA(pfile) == false )
		{	return false; }		
	}	
	::memcpy(&(m_MapBufA[m_MapPos]), Buf, sizeof(char)*Size);
	m_MapPos += Size;	
	m_MapCount += Size;	
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CAOIFileIO::CheckFlushMapBufW()
{	
	if ( (m_MapSize-m_MapPos) > 1024 ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::FlushMapBufW(FILE *pfile)
{	
#ifdef _DEBUG
	if ( CAOIFileIO::CheckMapBufW() == false ) { return false; }	
#endif//_DEBUG	
	int Res = ::fwrite(m_MapBufW, sizeof(m_MapBufW[0]), m_MapPos, pfile);		
	m_MapCount += m_MapPos;	
	if ( CheckUtfPrintf(Res) == false )
	{
		CString str;		
		str = _T("Error, Flush BufferW to File Fault");
		CAOIFileIO::SetErrorString(str);
		return false;
	}
	if ( Res != m_MapPos )
	{
		CString str;		
		str = _T("Error, Flush BufferW to File Fault");
		CAOIFileIO::SetErrorString(str);
		return false;
	}
	::memset(m_MapBufW, 0x00, sizeof(m_MapBufW[0])*m_MapPos);
	m_MapPos = 0;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::AddMapBufW(FILE *pfile, const wchar_t *Buf, size_t Size)//加入內存緩衝
{
#ifdef _DEBUG
	if ( CAOIFileIO::CheckMapBufW() == false ) { return false; }	
#endif//_DEBUG
	size_t EndPos=m_MapPos+Size;
	if ( EndPos >= m_MapSize )
	{		
		if ( FlushMapBufW(pfile) == false )
		{	return false; }		
	}	
	::memcpy(&(m_MapBufW[m_MapPos]), Buf, sizeof(wchar_t)*Size);
	m_MapPos += Size;		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::SaveTxtBufChunk_BOL(FILE *pfile, FILE_IO_ID index, bool value)
{
#ifdef _DEBUG
	if ( CAOIFileIO::CheckFileMode(FILE_MODE_TXT) == false ) { return false; }
#endif//_DEBUG

	int nVal = 0;
	if ( true == value ) { nVal = 1; }
	else { nVal = 0; }
	int Res = ::sprintf(m_TextLine, "%d, %d\n", index, nVal);
	if ( CheckTxtBufPrintf(Res) == false )
	{
		CString str;
		str.Format(_T("Error, Save (%d, %d) Fault"), index, nVal);
		CAOIFileIO::SetErrorString(str);
		return false;
	}	
	if ( AddMapBufA(pfile, m_TextLine, Res) == false )
	{	return false; }	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::SaveTxtBufChunk_INT(FILE *pfile, FILE_IO_ID index, int value)
{
#ifdef _DEBUG
	if ( CAOIFileIO::CheckFileMode(FILE_MODE_TXT) == false ) { return false; }
#endif//_DEBUG

	int Res = ::sprintf(m_TextLine, "%d, %d\n", index, value);
	if ( CheckTxtBufPrintf(Res) == false )
	{
		CString str;
		str.Format(_T("Error, Save (%d, %d) Fault"), index, value);
		CAOIFileIO::SetErrorString(str);
		return false;
	}	
	if ( AddMapBufA(pfile, m_TextLine, Res) == false )
	{	return false; }		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::SaveTxtBufChunk_DBL(FILE *pfile, FILE_IO_ID index, double value)
{
#ifdef _DEBUG
	if ( CAOIFileIO::CheckFileMode(FILE_MODE_TXT) == false ) { return false; }
#endif//_DEBUG

	int Res = ::sprintf(m_TextLine, "%d, %.16f\n", index, value);
	if ( CheckTxtBufPrintf(Res) == false )
	{
		CString str;
		str.Format(_T("Error, Save (%d, %.16f) Fault"), index, value);
		CAOIFileIO::SetErrorString(str);
		return false;
	}	
	if ( AddMapBufA(pfile, m_TextLine, Res) == false )
	{	return false; }		
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::SaveTxtBufChunk_STR(FILE *pfile, FILE_IO_ID index, const char *value)
{
#ifdef _DEBUG
	if ( CAOIFileIO::CheckFileMode(FILE_MODE_TXT) == false ) { return false; }
#endif//_DEBUG

	int Res = ::sprintf(m_TextLine, "%d, %s\n", index, value);
	if ( CheckTxtBufPrintf(Res) == false )
	{
		CString str;
		str.Format(_T("Error, Save (%d, %s) Fault"), index, value);
		CAOIFileIO::SetErrorString(str);
		return false;
	}	
	if ( AddMapBufA(pfile, m_TextLine, Res) == false )
	{	return false; }			
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::SaveTxtBufChunk_STR(FILE *pfile, FILE_IO_ID index, const wchar_t *value)
{	
	{
		CString str;
		str.Format(_T("Error, Save (%d, %s) Fault"), index, value);
		CAOIFileIO::SetErrorString(str);
	}
	return false;

#ifdef _DEBUG
	if ( CAOIFileIO::CheckFileMode(FILE_MODE_TXT) == false ) { return false; }
#endif//_DEBUG

	int Res = ::sprintf(m_TextLine, "%d, %s\n", index, value);
	if ( CheckTxtBufPrintf(Res) == false )
	{
		CString str;
		str.Format(_T("Error, Save (%d, %s) Fault"), index, value);
		CAOIFileIO::SetErrorString(str);
		return false;
	}	
	if ( AddMapBufA(pfile, m_TextLine, Res) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::SaveTxtBufChunk_INT64(FILE *pfile, FILE_IO_ID index, __int64 value)
{
#ifdef _DEBUG
	if ( CAOIFileIO::CheckFileMode(FILE_MODE_TXT) == false ) { return false; }
#endif//_DEBUG

	int Res = ::sprintf(m_TextLine, "%d, %I64d\n", index, value);
	if ( CheckTxtBufPrintf(Res) == false )
	{
		CString str;
		str.Format(_T("Error, Save (%d, %I64d) Fault"), index, value);
		CAOIFileIO::SetErrorString(str);
		return false;
	}	
	if ( AddMapBufA(pfile, m_TextLine, Res) == false )
	{	return false; }		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::SaveUtfBufChunk_BOL(FILE *pfile, FILE_IO_ID index, bool value)
{
#ifdef _DEBUG
	if ( CAOIFileIO::CheckFileMode(FILE_MODE_UNICODE) == false ) { return false; }
#endif//_DEBUG
	int nVal = 0;
	if ( true == value ) { nVal = 1; }
	else { nVal = 0; }	
	int Res = ::swprintf(m_wTextLine, L"%d, %d\n", index, nVal);
	if ( CheckUtfBufPrintf(Res) == false )
	{
		CString str;		
		str.Format(_T("Error, Save (%d, %d) Fault"), index, nVal);
		CAOIFileIO::SetErrorString(str);
		return false;
	}
	if ( AddMapBufW(pfile, m_wTextLine, Res) == false )
	{	return false; }	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::SaveUtfBufChunk_INT(FILE *pfile, FILE_IO_ID index, int value)
{
#ifdef _DEBUG
	if ( CAOIFileIO::CheckFileMode(FILE_MODE_UNICODE) == false ) { return false; }
#endif//_DEBUG
	int Res = ::swprintf(m_wTextLine, L"%d, %d\n", index, value);
	if ( CheckUtfBufPrintf(Res) == false )
	{
		CString str;		
		str.Format(_T("Error, Save (%d, %d) Fault"), index, value);
		CAOIFileIO::SetErrorString(str);
		return false;
	}	
	if ( AddMapBufW(pfile, m_wTextLine, Res) == false )
	{	return false; }
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::SaveUtfBufChunk_DBL(FILE *pfile, FILE_IO_ID index, double value)
{
#ifdef _DEBUG
	if ( CAOIFileIO::CheckFileMode(FILE_MODE_UNICODE) == false ) { return false; }
#endif//_DEBUG
	int Res = ::swprintf(m_wTextLine, L"%d, %.16f\n", index, value);
	if ( CheckUtfBufPrintf(Res) == false )
	{
		CString str;		
		str.Format(_T("Error, Save (%d, %.16f) Fault"), index, value);
		CAOIFileIO::SetErrorString(str);
		return false;
	}	
	if ( AddMapBufW(pfile, m_wTextLine, Res) == false )
	{	return false; }
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::SaveUtfBufChunk_STR(FILE *pfile, FILE_IO_ID index, const char *value)
{
	{
		CString str;
		str.Format(_T("Error, Save (%d, %s) Fault"), index, value);
		CAOIFileIO::SetErrorString(str);
	}
	return false;

#ifdef _DEBUG
	if ( CAOIFileIO::CheckFileMode(FILE_MODE_UNICODE) == false ) { return false; }
#endif//_DEBUG
	int Res = ::swprintf(m_wTextLine, L"%d, %s\n", index, value);		
	if ( CheckUtfBufPrintf(Res) == false )
	{
		CString str;		
		str.Format(_T("Error, Save (%d, %s) Fault"), index, value);
		CAOIFileIO::SetErrorString(str);
		return false;
	}
	if ( AddMapBufW(pfile, m_wTextLine, Res) == false )
	{	return false; }	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::SaveUtfBufChunk_STR(FILE *pfile, FILE_IO_ID index, const wchar_t *value)
{
#ifdef _DEBUG
	if ( CAOIFileIO::CheckFileMode(FILE_MODE_UNICODE) == false ) { return false; }
#endif//_DEBUG
	int Res = ::swprintf(m_wTextLine, L"%d, %s\n", index, value);
	if ( CheckUtfBufPrintf(Res) == false )
	{
		CString str;		
		str.Format(_T("Error, Save (%d, %s) Fault"), index, value);
		CAOIFileIO::SetErrorString(str);
		return false;
	}
	if ( AddMapBufW(pfile, m_wTextLine, Res) == false )
	{	return false; }	
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::SaveUtfBufChunk_INT64(FILE *pfile, FILE_IO_ID index, __int64 value)
{
#ifdef _DEBUG
	if ( CAOIFileIO::CheckFileMode(FILE_MODE_UNICODE) == false ) { return false; }
#endif//_DEBUG
	int Res = ::swprintf(m_wTextLine, L"%d, %I64d\n", index, value);
	if ( CheckUtfBufPrintf(Res) == false )
	{
		CString str;		
		str.Format(_T("Error, Save (%d, %I64d) Fault"), index, value);
		CAOIFileIO::SetErrorString(str);
		return false;
	}	
	if ( AddMapBufW(pfile, m_wTextLine, Res) == false )
	{	return false; }		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::CheckFileEnd()//確認檔案尾點
{
	if ( FILE_READ_HARD_DISK == m_FileReadMode )
	{
		if ( ::feof(m_FilePtr) == 0 ) { return false; }
	}
	else
	{
		if ( m_MapPos < m_MapCount ) { return false; }
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::CheckFileMode()//確認檔案模式
{
	bool IsOK = false;
	switch ( m_FileMode )
	{
	case FILE_MODE_TXT:			
	case FILE_MODE_BINARY:
	case FILE_MODE_UNICODE:
		IsOK = true;
		break;
	default:
		CAOIFileIO::SetErrorString(_T("File Mode Exception"));
		break;
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::CheckFileOpened()//確認檔案開?
{
	if ( NULL == m_FilePtr ) 
	{
		CAOIFileIO::SetErrorString(_T("File Not Opened"));
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::CloseFile()//關閉檔案
{
	if ( NULL == m_FilePtr ) { return true; }

	if ( FILE_IO_MODE_SAVE == m_FileIOMode )
	{
		if ( FILE_WRITE_MEMORY == m_FileWriteMode )
		{
			if ( m_MapPos > 0 ) 
			{
				if ( NULL != m_MapBufA )
				{	FlushMapBufA(m_FilePtr); }
				if ( NULL != m_MapBufW )
				{	FlushMapBufW(m_FilePtr); }		
			}
		}
	}
	::fclose(m_FilePtr); m_FilePtr = NULL;
	SetFileIOMode(FILE_IO_MODE_NONE);	
	return true; 
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::OpenSaveFile(LPCTSTR filename, FILE_MODE FileMode)//開啟儲存檔案
{	
	SetFnName(_T("CAOIFileIO::OpenSaveFile"));
	bool IsOK = false;
	SetFileMode(FileMode);
	SetFileWriteMode(FILE_WRITE_HARD_DISK);
	switch ( m_FileMode )
	{
	case FILE_MODE_BINARY:
		SetSaveWStr(false);
		IsOK = OpenSaveFile_BIN(filename);
		break;
	case FILE_MODE_UNICODE:
		SetSaveWStr(true);
		IsOK = OpenSaveFile_UTF(filename);
		break;
	default:
		SetSaveWStr(false);
		IsOK = OpenSaveFile_TXT(filename);
		break;	
	}
	if ( false == IsOK )
	{
		SetFileIOMode(FILE_IO_MODE_NONE);		
		return false;
	}
	SetFileIOMode(FILE_IO_MODE_SAVE);
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::OpenSaveFile_TXT(LPCTSTR filename)//開啟儲存檔案
{
	SetFnName(_T("CAOIFileIO::OpenSaveFile_TXT"));
	CString str;
	FILE *pfile = NULL;
	pfile = ::_tfopen(filename, _T("w+"));
	if ( pfile == NULL )
	{
		str.Format(_T("Error, Save Project Fault (%s)"), filename);
		CAOIFileIO::SetErrorString(str);
		return false;
	}	
	m_FilePtr = pfile;	
	if ( true == EnableFileWriteMemory )
	{
		const size_t BufSize=FileBufferSize;	
		if ( CreateMapBufA(BufSize) == true )
		{
			m_MapCount = 0;
			SetFileWriteMode(FILE_WRITE_MEMORY);	
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::OpenSaveFile_BIN(LPCTSTR filename)//開啟儲存檔案
{
	SetFnName(_T("CAOIFileIO::OpenSaveFile_BIN"));
	CString str;
	FILE *pfile = NULL;
	pfile = ::_tfopen(filename, _T("wb"));
	if ( pfile == NULL )
	{
		str.Format(_T("Error, Save Project Fault (%s)"), filename);
		CAOIFileIO::SetErrorString(str);
		return false;
	}	
	m_FilePtr = pfile;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::OpenSaveFile_UTF(LPCTSTR filename)//開啟儲存檔案
{
	SetFnName(_T("CAOIFileIO::OpenSaveFile_UTF"));
	CString str;
	FILE *pfile = NULL;
	pfile = ::_tfopen(filename, _T("w+, ccs=UTF-8"));//UTF-8, UTF-16LE
	if ( pfile == NULL )
	{
		str.Format(_T("Error, Save Project Fault (%s)"), filename);
		CAOIFileIO::SetErrorString(str);
		return false;
	}		
	m_FilePtr = pfile;	
	if ( true == EnableFileWriteMemory )
	{
		const size_t BufSize=FileBufferSize;	
		if ( CreateMapBufW(BufSize) == true )
		{
			m_MapCount = 0;
			SetFileWriteMode(FILE_WRITE_MEMORY);	
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::OpenLoadFile(LPCTSTR filename, FILE_MODE FileMode)//開啟載入檔案
{
	SetFnName(_T("CAOIFileIO::OpenLoadFile"));
	bool IsOK = false;
	m_RowCount = 0;
	SetFileMode(FileMode);
	SetFileReadMode(FILE_READ_HARD_DISK);
	switch ( m_FileMode )
	{
	case FILE_MODE_BINARY:
		IsOK = OpenLoadFile_BIN(filename);
		break;
	case FILE_MODE_UNICODE:
		IsOK = OpenLoadFile_UTF(filename);
		break;
	default:
		IsOK = OpenLoadFile_TXT(filename);
		break;
	}
	if ( false == IsOK )
	{
		SetFileIOMode(FILE_IO_MODE_NONE);		
		return false;
	}
	SetFileIOMode(FILE_IO_MODE_LOAD);	
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::OpenLoadFile_TXT(LPCTSTR filename)//開啟載入檔案	
{
	SetFnName(_T("CAOIFileIO::OpenLoadFile_TXT"));
	CString str;
	FILE *pfile = NULL;
	pfile = ::_tfopen(filename, _T("r"));
	if ( pfile == NULL )
	{
		str.Format(_T("Error, Load Project Fault (%s)"), filename);
		CAOIFileIO::SetErrorString(str);
		return false;
	}		
	CAOIFileIO::m_FilePtr = pfile;
	if ( true == EnableFileReadMemory )
	{
		if ( CopyFileToMapBufA() == true )
		{	SetFileReadMode(FILE_READ_MEMORY); }
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::OpenLoadFile_BIN(LPCTSTR filename)//開啟載入檔案	
{
	SetFnName(_T("CAOIFileIO::OpenLoadFile_BIN"));
	CString str;
	FILE *pfile = NULL;
	pfile = ::_tfopen(filename, _T("rb"));
	if ( pfile == NULL )
	{
		str.Format(_T("Error, Load Project Fault (%s)"), filename);
		CAOIFileIO::SetErrorString(str);
		return false;
	}		
	CAOIFileIO::m_FilePtr = pfile;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::OpenLoadFile_UTF(LPCTSTR filename)//開啟載入檔案	
{
	SetFnName(_T("CAOIFileIO::OpenLoadFile_UTF"));
	CString str;
	FILE *pfile = NULL;
	pfile = ::_tfopen(filename, _T("r, ccs=UNICODE"));
	if ( pfile == NULL )
	{
		str.Format(_T("Error, Load Project Fault (%s)"), filename);
		CAOIFileIO::SetErrorString(str);
		return false;
	}	
	CAOIFileIO::m_FilePtr = pfile;
	if ( true == EnableFileReadMemory )
	{
		if ( CopyFileToMapBufW() == true )
		{	SetFileReadMode(FILE_READ_MEMORY); }
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::ReleaseMapBuf()
{
	if ( NULL != m_MapBufA )
	{
		if ( true == m_MapAllocByPoolA )
		{	JetMemory.free_func(m_MapBufA);	}
		else
		{	delete[] m_MapBufA;  }
		m_MapAllocByPoolA = false;
	}
	if ( NULL != m_MapBufW )
	{
		if ( true == m_MapAllocByPoolW )
		{	JetMemory.free_func(m_MapBufW);	}
		else
		{	delete[] m_MapBufW;	}
		m_MapAllocByPoolW = false;
	}
	m_MapPos = -1;	
	m_MapSize = 0;
	m_MapCount = 0;	
	m_MapBufA = NULL;
	m_MapBufW = NULL;		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::CreateMapBufA(size_t sz)
{
	ReleaseMapBuf();
	char *Buf = NULL;
	size_t MemCnt=JetMemory.get_memory_size_count();
	size_t MemSize=JetMemory.get_memory_size(MemCnt-1)/sizeof(char);
	if ( sz > MemSize )
	{	Buf = new char[sz]; }
	else
	{
		m_MapAllocByPoolA = true;
		if ( JetMemory.alloc_func(sz, Buf, "CAOIFileIO::CreateMapBufA", "Buf") == false )
		{	return false; }
	}
	if ( NULL == Buf ) 
	{	return false;	}
	::memset(Buf, 0x00, sizeof(char)*sz);

	m_MapPos =  0;	
	m_MapSize = sz;
	m_MapCount= sz;	
	m_MapBufA = Buf;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::CreateMapBufW(size_t sz)
{	
	ReleaseMapBuf();
	wchar_t *Buf = NULL;
	size_t MemCnt=JetMemory.get_memory_size_count();
	size_t MemSize=JetMemory.get_memory_size(MemCnt-1)/sizeof(wchar_t);
	if ( sz > MemSize )
	{	Buf = new wchar_t[sz]; }
	else
	{	
		m_MapAllocByPoolW = true;
		if ( JetMemory.alloc_func(sz, Buf, "CAOIFileIO::CreateMapBufW", "Buf") == false )
		{	return false; }
	}
	if ( NULL == Buf ) 
	{	return false;	}
	::memset(Buf, 0x00, sizeof(wchar_t)*sz);

	m_MapPos =  0;	
	m_MapSize = sz;
	m_MapCount= sz;	
	m_MapBufW = Buf;
	return true;
}
//-------------------------------------------------------------------------------------//
size_t CAOIFileIO::CalcFileSize()
{
	FILE *pfile = m_FilePtr;
	if ( NULL == pfile ) { return 0; }
#ifndef _X64
	long   FileSize=0;
	long   StartPos=0, EndPos=0;		
	StartPos = ::ftell(pfile);
	::fseek(pfile, 0, SEEK_END);
	EndPos = ::ftell(pfile);
	::fseek(pfile, StartPos, SEEK_SET);	
#else
	__int64    FileSize=0;
	__int64    StartPos=0, EndPos=0;		
	StartPos = ::_ftelli64(pfile);
	::_fseeki64 (pfile, 0, SEEK_END);
	EndPos = ::_ftelli64(pfile);
	::_fseeki64 (pfile, StartPos, SEEK_SET);	
#endif//_X64	
	FileSize = EndPos-StartPos;
	return FileSize;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::CopyFileToMapBufA()
{
	FILE *pfile = m_FilePtr;
	if ( NULL == pfile ) { return false; }
	const size_t FileSize = CalcFileSize();
	if ( 0 == FileSize ) { return false; }	
	if ( CreateMapBufA(FileSize) == false ) { return false; }	
	m_MapCount = ::fread(m_MapBufA, sizeof(m_MapBufA[0]), FileSize, pfile);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::CopyFileToMapBufW()
{
	FILE *pfile = m_FilePtr;
	if ( NULL == pfile ) { return false; }	
	const size_t FileSize = CalcFileSize();
	if ( 0 == FileSize ) { return false; }	
	if ( CreateMapBufW(FileSize) == false ) { return false; }	
	m_MapCount = ::fread(m_MapBufW, sizeof(m_MapBufW[0]), FileSize, pfile);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::mgets(char *Buffer, int len, size_t &BufPos)
{
	char *pBuf = (char*)m_MapBufA;
	if ( NULL == pBuf ) { return false; }

	size_t i=0, j=0;
	const size_t BufSize=m_MapCount;	
	for ( i=BufPos; i<BufSize; i++ )
	{
		Buffer[j++] = pBuf[i];
		if ( '\n' == pBuf[i] )
		{	break; }		
		if ( j == len )
		{	return false; }
	}
	Buffer[j] = L'\0';
	BufPos = i+1;
	if ( BufPos > BufSize )
	{	BufPos = BufSize; }	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::mgetws(wchar_t *Buffer, int len, size_t &BufPos)
{
	wchar_t *pBuf = (wchar_t*)m_MapBufW;
	if ( NULL == pBuf ) { return false; }

	bool bSucc=true;
	size_t i=0, j=0;
	const size_t BufSize=m_MapCount;	
	for ( i=BufPos; i<BufSize; i++ )
	{
		Buffer[j++] = pBuf[i];
		if ( L'\n' == pBuf[i] )
		{	break; }		
		if ( j == len )
		{
			bSucc = false;
			break;
		}
	}
	Buffer[j] = L'\0';
	BufPos = i+1;
	if ( BufPos > BufSize )
	{	BufPos = BufSize; }	
	return bSucc;	
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::SaveChunk_BOL(FILE_IO_ID index, bool value)
{	
#ifdef _DEBUG
	if ( CheckFileIOMode(FILE_IO_MODE_SAVE) == false )
	{	return false; }
#endif

	if ( FILE_MODE_BINARY == m_FileMode )
	{	return SaveBinChunk_BOL(m_FilePtr, index, value);	}

	if ( FILE_MODE_UNICODE == m_FileMode )
	{
		if ( FILE_WRITE_MEMORY == m_FileWriteMode )
		{	return SaveUtfBufChunk_BOL(m_FilePtr, index, value);	}
		else
		{	return SaveUtfChunk_BOL(m_FilePtr, index, value);	 }
	}

	if ( FILE_WRITE_MEMORY == m_FileWriteMode )
	{	return SaveTxtBufChunk_BOL(m_FilePtr, index, value); }
	return SaveTxtChunk_BOL(m_FilePtr, index, value);
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::SaveChunk_INT(FILE_IO_ID index, int value)
{
#ifdef _DEBUG
	if ( CheckFileIOMode(FILE_IO_MODE_SAVE) == false )
	{	return false; }
#endif

	if ( FILE_MODE_BINARY == m_FileMode )
	{	return SaveBinChunk_INT(m_FilePtr, index, value);	}

	if ( FILE_MODE_UNICODE == m_FileMode )
	{
		if ( FILE_WRITE_MEMORY == m_FileWriteMode )
		{	return SaveUtfBufChunk_INT(m_FilePtr, index, value);	}
		else
		{	return SaveUtfChunk_INT(m_FilePtr, index, value);	}
	}
	
	if ( FILE_WRITE_MEMORY == m_FileWriteMode )
	{	return SaveTxtBufChunk_INT(m_FilePtr, index, value); }
	return SaveTxtChunk_INT(m_FilePtr, index, value);	
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::SaveChunk_DBL(FILE_IO_ID index, double value)
{
#ifdef _DEBUG
	if ( CheckFileIOMode(FILE_IO_MODE_SAVE) == false )
	{	return false; }
#endif

	if ( FILE_MODE_BINARY == m_FileMode )
	{	return SaveBinChunk_DBL(m_FilePtr, index, value);	}

	if ( FILE_MODE_UNICODE == m_FileMode )
	{
		if ( FILE_WRITE_MEMORY == m_FileWriteMode )
		{	return SaveUtfBufChunk_DBL(m_FilePtr, index, value);	}
		else
		{	return SaveUtfChunk_DBL(m_FilePtr, index, value);	}
	}
	
	if ( FILE_WRITE_MEMORY == m_FileWriteMode )
	{	return SaveTxtBufChunk_DBL(m_FilePtr, index, value); }
	return SaveTxtChunk_DBL(m_FilePtr, index, value);
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::SaveChunk_STR(FILE_IO_ID index, const char *value)
{
#ifdef _DEBUG
	if ( CheckFileIOMode(FILE_IO_MODE_SAVE) == false )
	{	return false; }
#endif

	if ( FILE_MODE_BINARY == m_FileMode )
	{	return SaveBinChunk_STR(m_FilePtr, index, value);	}

	if ( FILE_MODE_UNICODE == m_FileMode )
	{
		if ( FILE_WRITE_MEMORY == m_FileWriteMode )
		{	return SaveUtfBufChunk_STR(m_FilePtr, index, value);	}
		else
		{	return SaveUtfChunk_STR(m_FilePtr, index, value);	}
	}	

	if ( FILE_WRITE_MEMORY == m_FileWriteMode )
	{	return SaveTxtBufChunk_STR(m_FilePtr, index, value); }
	return SaveTxtChunk_STR(m_FilePtr, index, value);	
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::SaveChunk_STR(FILE_IO_ID index, const wchar_t *value)
{
#ifdef _DEBUG
	if ( CheckFileIOMode(FILE_IO_MODE_SAVE) == false )
	{	return false; }
#endif

	if ( FILE_MODE_BINARY == m_FileMode )
	{	return SaveBinChunk_STR(m_FilePtr, index, value);	}

	if ( FILE_MODE_UNICODE == m_FileMode )
	{
		if ( FILE_WRITE_MEMORY == m_FileWriteMode )
		{	return SaveUtfBufChunk_STR(m_FilePtr, index, value);	}
		else
		{	return SaveUtfChunk_STR(m_FilePtr, index, value);	}
	}		
	
	if ( FILE_WRITE_MEMORY == m_FileWriteMode )
	{	return SaveTxtBufChunk_STR(m_FilePtr, index, value); }
	return SaveTxtChunk_STR(m_FilePtr, index, value);	
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::SaveChunk_INT64(FILE_IO_ID index, __int64 value)
{
#ifdef _DEBUG
	if ( CheckFileIOMode(FILE_IO_MODE_SAVE) == false )
	{	return false; }
#endif

	if ( FILE_MODE_BINARY == m_FileMode )
	{	return SaveBinChunk_INT64(m_FilePtr, index, value);	}

	if ( FILE_MODE_UNICODE == m_FileMode )
	{
		if ( FILE_WRITE_MEMORY == m_FileWriteMode )
		{	return SaveUtfBufChunk_INT64(m_FilePtr, index, value);	 }
		else
		{	return SaveUtfChunk_INT64(m_FilePtr, index, value);	 }
	}
	
	if ( FILE_WRITE_MEMORY == m_FileWriteMode )
	{	return SaveTxtBufChunk_INT64(m_FilePtr, index, value); }
	return SaveTxtChunk_INT64(m_FilePtr, index, value);	
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::LoadChunk(int &index)
{	
#ifdef _DEBUG
	if ( CheckFileIOMode(FILE_IO_MODE_LOAD) == false )
	{	return false; }
#endif

	FILE *pfile = m_FilePtr;
	if ( FILE_MODE_BINARY == m_FileMode )
	{	
		if ( LoadBinChunk(pfile, index, m_Data_INT, m_Data_DBL,  m_DataLine, m_wDataLine, m_Data_INT64) == false )
		{	return false; }
	}

	//Txt
	if ( FILE_MODE_UNICODE == m_FileMode )
	{
		SetLoadWStr(true);
		if ( FILE_READ_MEMORY == m_FileReadMode )
		{
			if ( mgetws(m_wTextLine, m_wTextSize, m_MapPos) == false )
			{	return false; }
		}
		else
		{
			if( ::fgetws(m_wTextLine, m_wTextSize, pfile) ==NULL ) 
			{	return false; }
		}		
		if ( JetAPI::DecoderTextLineW(m_wTextLine, m_wIndexLine, m_wDataLine) == false )
		{	return false; }
		index = ::_wtoi(m_wIndexLine);
	}
	else
	{
		SetLoadWStr(false);
		if ( FILE_READ_MEMORY == m_FileReadMode )
		{
			if ( mgets(m_TextLine, m_TextSize, m_MapPos) == false )
			{	return false; }
		}
		else
		{
			if( ::fgets(m_TextLine, m_TextSize, pfile) ==NULL ) 
			{	return false; }
		}		
		if ( JetAPI::DecoderTextLineA(m_TextLine, m_IndexLine, m_DataLine) == false )
		{	return false; }
		index = ::atoi(m_IndexLine);
	}
	m_RowCount ++;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::GetData_BOL()
{
	if ( FILE_MODE_BINARY == m_FileMode )
	{
		if ( 0 == m_Data_INT ) { return false; }
		return true;
	}
	if ( FILE_MODE_UNICODE == m_FileMode )
	{
		if ( L'0' == m_wDataLine[0] ) { return false; }
		return true;
	}
	//Txt	
	if ( '0' == m_DataLine[0] ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
int CAOIFileIO::GetData_INT()
{
	if ( FILE_MODE_BINARY == m_FileMode )
	{	return m_Data_INT;	}

	if ( FILE_MODE_UNICODE == m_FileMode )
	{
		m_Data_INT = ::_wtoi(m_wDataLine);			
		return m_Data_INT;
	}
	//Txt
	m_Data_INT = ::atoi(m_DataLine);	
	return m_Data_INT;
}
//-------------------------------------------------------------------------------------//
float CAOIFileIO::GetData_FLT()
{
	return static_cast<float>(GetData_DBL());
}
//-------------------------------------------------------------------------------------//
double CAOIFileIO::GetData_DBL()
{
	if ( FILE_MODE_BINARY == m_FileMode )
	{	return m_Data_DBL;	}

	if ( FILE_MODE_UNICODE == m_FileMode )
	{
		m_Data_DBL = ::_wtof(m_wDataLine);			
		return m_Data_DBL;
	}
	//Txt
	m_Data_DBL = ::atof(m_DataLine);	
	return m_Data_DBL;
}
//-------------------------------------------------------------------------------------//
char* CAOIFileIO::GetData_STR()
{
	return m_DataLine;
}
//-------------------------------------------------------------------------------------//
wchar_t* CAOIFileIO::GetData_WSTR()
{
	return m_wDataLine;
}
//-------------------------------------------------------------------------------------//
__int64 CAOIFileIO::GetData_INT64()
{
	if ( FILE_MODE_BINARY == m_FileMode )
	{	return m_Data_INT64;	}

	if ( FILE_MODE_UNICODE == m_FileMode )
	{		
		m_Data_INT64 = ::_wtoi64(m_wDataLine);			
		return m_Data_INT64;
	}
	//Txt
	m_Data_INT64 = ::_atoi64(m_DataLine);	
	return m_Data_INT64;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::ReadIntListFile(std::vector<int> &List)
{
#ifdef _DEBUG
	if ( CheckFileMode() == false ) { return false; }
	if ( CheckFileOpened() == false ) { return false; }	
#endif//_DEBUG
	int    index = 0;	
	SetFnName(_T("CAOIFileIO::ReadIntListFile"));
	while ( CheckFileEnd()==false )
	{ 		
		if ( LoadChunk(index) == false ) 		
		{	continue; }		
		
		switch ( index )
		{
		case FILE_IO_INT_LIST_START://整數列表參數-起點
			List.clear();
			break;
		case FILE_IO_INT_LIST_END://整數列表參數-終點			
			return true;
			break;
		case FILE_IO_INT_LIST_VALUE://整數列表參數-數值			
			List.push_back(GetData_INT());
			break;		
		default:
			break;
		}
	};
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::WriteIntListFile(const std::vector<int> &List)
{
#ifdef _DEBUG
	if ( CheckFileMode() == false ) { return false; }
	if ( CheckFileOpened() == false ) { return false; }	
#endif//_DEBUG

	SetFnName(_T("CAOIFileIO::WriteIntListFile"));
	//----------------------------------------------------------------------------------------//
	size_t       i=0;
	const size_t Count = List.size();
	if ( SaveChunk_INT(FILE_IO_INT_LIST_START, 0) == false ) { return false; }

	for ( i=0; i<Count; i++ )
	{
		if ( SaveChunk_INT(FILE_IO_INT_LIST_VALUE, List[i]) == false )
		{	return false; }
	}
	if ( SaveChunk_INT(FILE_IO_INT_LIST_END, 0) == false ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::ReadBoolListFile(std::vector<bool> &List)
{
#ifdef _DEBUG
	if ( CheckFileMode() == false ) { return false; }
	if ( CheckFileOpened() == false ) { return false; }	
#endif//_DEBUG
	int    index = 0;	
	SetFnName(_T("CAOIFileIO::ReadBoolListFile"));
	while ( CheckFileEnd()==false )
	{ 		
		if ( LoadChunk(index) == false ) 		
		{	continue; }		
		
		switch ( index )
		{
		case FILE_IO_BOL_LIST_START://布林列表參數-起點		
			List.clear();
			break;
		case FILE_IO_BOL_LIST_END://布林列表參數-終點			
			return true;
			break;
		case FILE_IO_BOL_LIST_VALUE://布林列表參數-數值			
			List.push_back(GetData_BOL());
			break;		
		default:
			break;
		}
	};
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::WriteBoolListFile(const std::vector<bool> &List)
{
#ifdef _DEBUG
	if ( CheckFileMode() == false ) { return false; }
	if ( CheckFileOpened() == false ) { return false; }	
#endif//_DEBUG

	SetFnName(_T("CAOIFileIO::WriteBoolListFile"));
	//----------------------------------------------------------------------------------------//
	size_t       i=0;
	const size_t Count = List.size();
	if ( SaveChunk_INT(FILE_IO_BOL_LIST_START, 0) == false ) { return false; }

	for ( i=0; i<Count; i++ )
	{
		if ( SaveChunk_BOL(FILE_IO_BOL_LIST_VALUE, List[i]) == false )
		{	return false; }
	}
	if ( SaveChunk_INT(FILE_IO_BOL_LIST_END, 0) == false ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::ReadDoubleListFile(std::vector<double> &List)
{
#ifdef _DEBUG
	if ( CheckFileMode() == false ) { return false; }
	if ( CheckFileOpened() == false ) { return false; }	
#endif//_DEBUG
	int    index = 0;	
	SetFnName(_T("CAOIFileIO::ReadDoubleListFile"));
	while ( CheckFileEnd()==false )
	{ 		
		if ( LoadChunk(index) == false ) 		
		{	continue; }		
		
		switch ( index )
		{
		case FILE_IO_DBL_LIST_START://浮點數列表參數-起點
			List.clear();
			break;
		case FILE_IO_DBL_LIST_END://浮點數列表參數-終點			
			return true;
			break;
		case FILE_IO_DBL_LIST_VALUE://浮點數列表參數-數值			
			List.push_back(GetData_DBL());
			break;		
		default:
			break;
		}
	};
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::WriteDoubleListFile(const std::vector<double> &List)
{
#ifdef _DEBUG
	if ( CheckFileMode() == false ) { return false; }
	if ( CheckFileOpened() == false ) { return false; }	
#endif//_DEBUG

	SetFnName(_T("CAOIFileIO::WriteBoolListFile"));
	//----------------------------------------------------------------------------------------//
	size_t       i=0;
	const size_t Count = List.size();
	if ( SaveChunk_INT(FILE_IO_DBL_LIST_START, 0) == false ) { return false; }

	for ( i=0; i<Count; i++ )
	{
		if ( SaveChunk_DBL(FILE_IO_DBL_LIST_VALUE, List[i]) == false )
		{	return false; }
	}
	if ( SaveChunk_INT(FILE_IO_DBL_LIST_END, 0) == false ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::ReadDoubleArrayFile(int Count, double List[])
{
#ifdef _DEBUG
	if ( CheckFileMode() == false ) { return false; }
	if ( CheckFileOpened() == false ) { return false; }	
#endif//_DEBUG
	int    idx=0;
	int    index = 0;		
	SetFnName(_T("CAOIFileIO::ReadDoubleArrayFile"));
	while ( CheckFileEnd()==false )
	{ 		
		if ( LoadChunk(index) == false ) 		
		{	continue; }		
		
		switch ( index )
		{
		case FILE_IO_DBL_LIST_START://浮點數列表參數-起點
			idx = 0;			
			break;
		case FILE_IO_DBL_LIST_END://浮點數列表參數-終點			
			return true;
			break;
		case FILE_IO_DBL_LIST_VALUE://浮點數列表參數-數值
			if ( idx < Count )
			{	List[idx++] = GetData_DBL();	}
			break;		
		default:
			break;
		}
	};
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::WriteDoubleArrayFile(int Count, const double List[])
{
#ifdef _DEBUG
	if ( CheckFileMode() == false ) { return false; }
	if ( CheckFileOpened() == false ) { return false; }	
#endif//_DEBUG

	SetFnName(_T("CAOIFileIO::WriteDoubleArrayFile"));
	//----------------------------------------------------------------------------------------//
	size_t       i=0;	
	if ( SaveChunk_INT(FILE_IO_DBL_LIST_START, 0) == false ) { return false; }

	for ( i=0; i<Count; i++ )
	{
		if ( SaveChunk_DBL(FILE_IO_DBL_LIST_VALUE, List[i]) == false )
		{	return false; }
	}
	if ( SaveChunk_INT(FILE_IO_DBL_LIST_END, 0) == false ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::ReadStrListFile(std::vector<std::string> &List)
{
#ifdef _DEBUG
	if ( CheckFileMode() == false ) { return false; }
	if ( CheckFileOpened() == false ) { return false; }	
#endif//_DEBUG
	int    index = 0;	
	SetFnName(_T("CAOIFileIO::ReadStrListFile"));
	while ( CheckFileEnd()==false )
	{ 		
		if ( LoadChunk(index) == false ) 		
		{	continue; }		
		
		switch ( index )
		{
		case FILE_IO_STR_LIST_START://字串列表參數-起點
			List.clear();
			break;
		case FILE_IO_STR_LIST_END://字串列表參數-終點			
			return true;
			break;
		case FILE_IO_STR_LIST_VALUE://字串列表參數-數值			
			List.push_back(GetData_STR());
			break;		
		default:
			break;
		}
	};
	return true;		
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::WriteStrListFile(const std::vector<std::string> &List)
{
#ifdef _DEBUG
	if ( CheckFileMode() == false ) { return false; }
	if ( CheckFileOpened() == false ) { return false; }	
#endif//_DEBUG

	SetFnName(_T("CAOIFileIO::WriteStrListFile"));
	//----------------------------------------------------------------------------------------//
	size_t       i=0;
	const size_t Count = List.size();
	if ( SaveChunk_INT(FILE_IO_STR_LIST_START, 0) == false ) { return false; }

	for ( i=0; i<Count; i++ )
	{
		if ( SaveChunk_STR(FILE_IO_STR_LIST_VALUE, List[i].c_str()) == false )
		{	return false; }		
	}
	if ( SaveChunk_INT(FILE_IO_STR_LIST_END, 0) == false ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::ReadStrListFile(std::vector<std::wstring> &List)
{
#ifdef _DEBUG
	if ( CheckFileMode() == false ) { return false; }
	if ( CheckFileOpened() == false ) { return false; }	
#endif//_DEBUG
	int    index = 0;	
	SetFnName(_T("CAOIFileIO::ReadStrListFile"));
	while ( CheckFileEnd()==false )
	{ 		
		if ( LoadChunk(index) == false ) 		
		{	continue; }		
		
		switch ( index )
		{
		case FILE_IO_STR_LIST_START://字串列表參數-起點
			List.clear();
			break;
		case FILE_IO_STR_LIST_END://字串列表參數-終點			
			return true;
			break;
		case FILE_IO_STR_LIST_VALUE://字串列表參數-數值			
			List.push_back(GetData_WSTR());
			break;		
		default:
			break;
		}
	};
	return true;		
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::WriteStrListFile(const std::vector<std::wstring> &List)
{
#ifdef _DEBUG
	if ( CheckFileMode() == false ) { return false; }
	if ( CheckFileOpened() == false ) { return false; }	
#endif//_DEBUG

	SetFnName(_T("CAOIFileIO::WriteStrListFile"));
	//----------------------------------------------------------------------------------------//
	size_t       i=0;
	const size_t Count = List.size();
	if ( SaveChunk_INT(FILE_IO_STR_LIST_START, 0) == false ) { return false; }

	for ( i=0; i<Count; i++ )
	{
		if ( SaveChunk_STR(FILE_IO_STR_LIST_VALUE, List[i].c_str()) == false )
		{	return false; }		
	}
	if ( SaveChunk_INT(FILE_IO_STR_LIST_END, 0) == false ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::ReadPointFile(POINT &Pt)
{
#ifdef _DEBUG
	if ( CheckFileMode() == false ) { return false; }
	if ( CheckFileOpened() == false ) { return false; }	
#endif//_DEBUG

	int    index = 0;
	SetFnName(_T("CAOIFileIO::ReadPointFile"));
	while ( CheckFileEnd()==false )
	{ 		
		if ( LoadChunk(index) == false ) 		
		{	continue; }		
		
		switch ( index )
		{
		case FILE_IO_POINT_START://點參數-起點			
			break;
		case FILE_IO_POINT_END://點參數-終點			
			return true;
			break;
		case FILE_IO_POINT_X://點參數-X
			Pt.x = GetData_INT();
			break;
		case FILE_IO_POINT_Y://點參數-Y	
			Pt.y = GetData_INT();			
			break;
		default:
			break;
		}
	};	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::WritePointFile(const POINT &Pt)
{
#ifdef _DEBUG
	if ( CheckFileMode() == false ) { return false; }
	if ( CheckFileOpened() == false ) { return false; }	
#endif//_DEBUG

	SetFnName(_T("CAOIFileIO::WritePointFile"));
	//----------------------------------------------------------------------------------------//
	if ( SaveChunk_INT(FILE_IO_POINT_START, 0) == false ) { return false; }
	if ( SaveChunk_INT(FILE_IO_POINT_X, Pt.x) == false ) { return false; }
	if ( SaveChunk_INT(FILE_IO_POINT_Y, Pt.y) == false ) { return false; }
	if ( SaveChunk_INT(FILE_IO_POINT_END, 0) == false ) { return false; }	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::ReadPointListFile(std::vector<POINT> &PtList)
{
#ifdef _DEBUG
	if ( CheckFileMode() == false ) { return false; }
	if ( CheckFileOpened() == false ) { return false; }	
#endif//_DEBUG

	POINT  Pt;
	int    index = 0;	
	SetFnName(_T("CAOIFileIO::ReadPointListFile"));
	while ( CheckFileEnd()==false )
	{ 		
		if ( LoadChunk(index) == false ) 		
		{	continue; }		
		
		switch ( index )
		{
		case FILE_IO_POINT_SECTION_START://點參數-起點			
			break;
		case FILE_IO_POINT_SECTION_END://點參數-終點			
			return true;
			break;
		case FILE_IO_POINT_START://點參數-起點
			if ( ReadPointFile(Pt) == false )
			{	return false; }
			PtList.push_back(Pt);
			break;		
		default:
			break;
		}
	};	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::WritePointListFile(const std::vector<POINT> &PtList)
{
#ifdef _DEBUG
	if ( CheckFileMode() == false ) { return false; }
	if ( CheckFileOpened() == false ) { return false; }	
#endif//_DEBUG

	SetFnName(_T("CAOIFileIO::WritePointListFile"));
	//----------------------------------------------------------------------------------------//
	size_t       i=0;
	const size_t Count = PtList.size();
	if ( SaveChunk_INT(FILE_IO_POINT_SECTION_START, 0) == false ) { return false; }

	for ( i=0; i<Count; i++ )
	{
		if ( WritePointFile(PtList[i]) == false ) 
		{	return false; }
	}
	if ( SaveChunk_INT(FILE_IO_POINT_SECTION_END, 0) == false ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::ReadRectFile(RECT &Rect)
{
#ifdef _DEBUG
	if ( CheckFileMode() == false ) { return false; }
	if ( CheckFileOpened() == false ) { return false; }	
#endif//_DEBUG

	int    index = 0;
	SetFnName(_T("CAOIFileIO::ReadRectFile"));
	while ( CheckFileEnd()==false )
	{ 		
		if ( LoadChunk(index) == false ) 		
		{	continue; }		
		
		switch ( index )
		{
		case FILE_IO_RECT_START://矩形參數-起點			
			break;
		case FILE_IO_RECT_END://矩形參數-終點			
			return true;
			break;
		case FILE_IO_RECT_LEFT://矩形參數-左
			Rect.left = GetData_INT();
			break;
		case FILE_IO_RECT_TOP://矩形參數-上	
			Rect.top = GetData_INT();			
			break;
		case FILE_IO_RECT_RIGHT://矩形參數-右	
			Rect.right = GetData_INT();			
			break;
		case FILE_IO_RECT_BOTTOM://矩形參數-下	
			Rect.bottom = GetData_INT();			
			break;
		default:
			break;
		}
	};	
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::WriteRectFile(const RECT &Rect)
{
#ifdef _DEBUG
	if ( CheckFileMode() == false ) { return false; }
	if ( CheckFileOpened() == false ) { return false; }	
#endif//_DEBUG

	SetFnName(_T("CAOIFileIO::WriteRectFile"));
	//----------------------------------------------------------------------------------------//
	if ( SaveChunk_INT(FILE_IO_RECT_START, 0) == false ) { return false; }
	if ( SaveChunk_INT(FILE_IO_RECT_LEFT, Rect.left) == false ) { return false; }
	if ( SaveChunk_INT(FILE_IO_RECT_TOP, Rect.top) == false ) { return false; }
	if ( SaveChunk_INT(FILE_IO_RECT_RIGHT, Rect.right) == false ) { return false; }
	if ( SaveChunk_INT(FILE_IO_RECT_BOTTOM, Rect.bottom) == false ) { return false; }
	if ( SaveChunk_INT(FILE_IO_RECT_END, 0) == false ) { return false; }	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::ReadRectListFile(std::vector<RECT> &RectList)
{
#ifdef _DEBUG
	if ( CheckFileMode() == false ) { return false; }
	if ( CheckFileOpened() == false ) { return false; }	
#endif//_DEBUG

	RECT   Rect;
	int    index = 0;	
	SetFnName(_T("CAOIFileIO::ReadRectListFile"));
	while ( CheckFileEnd()==false )
	{ 		
		if ( LoadChunk(index) == false ) 		
		{	continue; }		
		
		switch ( index )
		{
		case FILE_IO_RECT_SECTION_START://矩形參數-起點			
			break;
		case FILE_IO_RECT_SECTION_END://矩形參數-終點			
			return true;
			break;
		case FILE_IO_RECT_START://矩形參數-起點
			if ( ReadRectFile(Rect) == false )
			{	return false; }
			RectList.push_back(Rect);
			break;		
		default:
			break;
		}
	};		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::WriteRectListFile(const std::vector<RECT> &RectList)
{
#ifdef _DEBUG
	if ( CheckFileMode() == false ) { return false; }
	if ( CheckFileOpened() == false ) { return false; }	
#endif//_DEBUG

	SetFnName(_T("CAOIFileIO::WriteRectListFile"));
	//----------------------------------------------------------------------------------------//
	size_t       i=0;
	const size_t Count = RectList.size();
	if ( SaveChunk_INT(FILE_IO_RECT_SECTION_START, 0) == false ) { return false; }

	for ( i=0; i<Count; i++ )
	{
		if ( WriteRectFile(RectList[i]) == false ) 
		{	return false; }
	}
	if ( SaveChunk_INT(FILE_IO_RECT_SECTION_END, 0) == false ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::ReadVersionCodeFile(TVersionCode &VersionCode)
{
#ifdef _DEBUG
	if ( CheckFileMode() == false ) { return false; }
	if ( CheckFileOpened() == false ) { return false; }	
#endif//_DEBUG

	int    index = 0;
	SetFnName(_T("CAOIFileIO::ReadVersionCodeFile"));
	while ( CheckFileEnd()==false )
	{ 		
		if ( LoadChunk(index) == false ) 		
		{	continue; }		
		
		switch ( index )
		{
		case FILE_IO_VERSION_CODE_START://版本號參數-起點
			VersionCode = TVersionCode();
			break;
		case FILE_IO_VERSION_CODE_END://版本號參數-終點			
			return true;
			break;
		case FILE_IO_VERSION_CODE_NAME://版本號參數-名稱
			if ( GetLoadWStr() == true )
			{	VersionCode.wsCodeName = GetData_WSTR(); }
			else
			{	JetAPI::char2wstring(GetData_STR(), VersionCode.wsCodeName);	}
			break;
		case FILE_IO_VERSION_CODE_ENABLED://版本號參數-啟用
			VersionCode.bEnabled = GetData_BOL();
			break;
		default:
			break;
		}
	};	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::WriteVersionCodeFile(const TVersionCode &VersionCode)
{
#ifdef _DEBUG
	if ( CheckFileMode() == false ) { return false; }
	if ( CheckFileOpened() == false ) { return false; }	
#endif//_DEBUG

	SetFnName(_T("CAOIFileIO::WriteVersionCodeFile"));
	//----------------------------------------------------------------------------------------//
	if ( SaveChunk_INT(FILE_IO_VERSION_CODE_START, 0) == false ) { return false; }
	if ( SaveChunk_STR(FILE_IO_VERSION_CODE_NAME, VersionCode.wsCodeName.c_str()) == false ) { return false; }
	if ( SaveChunk_BOL(FILE_IO_VERSION_CODE_ENABLED, VersionCode.bEnabled) == false ) { return false; }	
	if ( SaveChunk_INT(FILE_IO_VERSION_CODE_END, 0) == false ) { return false; }	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::ReadVersionCodeListFile(std::vector<TVersionCode> &VersionList)
{
#ifdef _DEBUG
	if ( CheckFileMode() == false ) { return false; }
	if ( CheckFileOpened() == false ) { return false; }	
#endif//_DEBUG
	
	int    index = 0;	
	TVersionCode   VersionCode;
	SetFnName(_T("CAOIFileIO::ReadVersionCodeListFile"));
	while ( CheckFileEnd()==false )
	{ 		
		if ( LoadChunk(index) == false ) 		
		{	continue; }		
		
		switch ( index )
		{
		case FILE_IO_VERSION_CODE_SECTION_START://版本號參數-起點
			VersionList.clear();
			break;
		case FILE_IO_VERSION_CODE_SECTION_END://版本號參數-終點			
			return true;
			break;
		case FILE_IO_VERSION_CODE_START://版本號參數-起點
			VersionCode = TVersionCode();
			if ( ReadVersionCodeFile(VersionCode) == false )
			{	return false; }
			VersionList.push_back(VersionCode);
			break;		
		default:
			break;
		}
	};		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::WriteVersionCodeListFile(const std::vector<TVersionCode> &VersionList)
{
#ifdef _DEBUG
	if ( CheckFileMode() == false ) { return false; }
	if ( CheckFileOpened() == false ) { return false; }	
#endif//_DEBUG

	SetFnName(_T("CAOIFileIO::WriteVersionCodeListFile"));
	//----------------------------------------------------------------------------------------//
	size_t       i=0;
	const size_t Count = VersionList.size();
	if ( SaveChunk_INT(FILE_IO_VERSION_CODE_SECTION_START, 0) == false ) { return false; }

	for ( i=0; i<Count; i++ )
	{
		if ( WriteVersionCodeFile(VersionList[i]) == false ) 
		{	return false; }
	}
	if ( SaveChunk_INT(FILE_IO_VERSION_CODE_SECTION_END, 0) == false ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::ReadVersionParamFile(TVersionParam &VersionParam)
{
#ifdef _DEBUG
	if ( CheckFileMode() == false ) { return false; }
	if ( CheckFileOpened() == false ) { return false; }	
#endif//_DEBUG

	int    index = 0;
	SetFnName(_T("CAOIFileIO::ReadVersionParamFile"));
	while ( CheckFileEnd()==false )
	{ 		
		if ( LoadChunk(index) == false ) 		
		{	continue; }		
		
		switch ( index )
		{
		case FILE_IO_VERSION_PARAM_START://版本參數-起點
			VersionParam = TVersionParam();
			break;
		case FILE_IO_VERSION_PARAM_END://版本參數-終點			
			return true;
			break;		
		case FILE_IO_VERSION_PARAM_BYPASSED://版本號參數-不檢測
			VersionParam.bBypassed = GetData_BOL();
			break;
		case FILE_IO_VERSION_PARAM_BYPASSED_3D://版本號參數-不檢測3D
			VersionParam.bBypassed3D = GetData_BOL();
			break;
		case FILE_IO_VERSION_PARAM_X_BOARD_UNIT://版本號參數-報廢件
			VersionParam.bXBoardUnit = GetData_BOL();
			break;
		case FILE_IO_VERSION_PARAM_MODEL_NAME://版本參數-模組名稱			
			if ( GetLoadWStr()==true )
			{	VersionParam.wsModelName = GetData_WSTR();	}
			else
			{	JetAPI::char2wstring(GetData_STR(), VersionParam.wsModelName);	}
			break;
		case FILE_IO_VERSION_PARAM_PART_NUMBER://版本參數-料號名稱
			if ( GetLoadWStr()==true )
			{	VersionParam.wsPartNumber = GetData_WSTR();	}
			else
			{	JetAPI::char2wstring(GetData_STR(), VersionParam.wsPartNumber);	}
			break;
		default:
			break;
		}
	};	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::WriteVersionParamFile(const TVersionParam &VersionParam)
{
#ifdef _DEBUG
	if ( CheckFileMode() == false ) { return false; }
	if ( CheckFileOpened() == false ) { return false; }	
#endif//_DEBUG

	SetFnName(_T("CAOIFileIO::WriteVersionParamFile"));
	//----------------------------------------------------------------------------------------//
	if ( SaveChunk_INT(FILE_IO_VERSION_PARAM_START, 0) == false ) { return false; }	
	if ( SaveChunk_BOL(FILE_IO_VERSION_PARAM_BYPASSED, VersionParam.bBypassed) == false ) { return false; }	
	if ( SaveChunk_BOL(FILE_IO_VERSION_PARAM_BYPASSED_3D, VersionParam.bBypassed3D) == false ) { return false; }	
	if ( SaveChunk_BOL(FILE_IO_VERSION_PARAM_X_BOARD_UNIT, VersionParam.bXBoardUnit) == false ) { return false; }	
	if ( SaveChunk_STR(FILE_IO_VERSION_PARAM_MODEL_NAME, VersionParam.wsModelName.c_str()) == false ) { return false; } 
	if ( SaveChunk_STR(FILE_IO_VERSION_PARAM_PART_NUMBER, VersionParam.wsPartNumber.c_str()) == false ) { return false; }
	if ( SaveChunk_INT(FILE_IO_VERSION_PARAM_END, 0) == false ) { return false; }	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::ReadVersionParamListFile(std::vector<TVersionParam> &VersionList)
{
#ifdef _DEBUG
	if ( CheckFileMode() == false ) { return false; }
	if ( CheckFileOpened() == false ) { return false; }	
#endif//_DEBUG
	
	int    index = 0;	
	TVersionParam   VersionParam;
	SetFnName(_T("CAOIFileIO::ReadVersionParamListFile"));
	while ( CheckFileEnd()==false )
	{ 		
		if ( LoadChunk(index) == false ) 		
		{	continue; }		
		
		switch ( index )
		{
		case FILE_IO_VERSION_PARAM_SECTION_START://版本參數區間-起點
			VersionList.clear();
			break;
		case FILE_IO_VERSION_PARAM_SECTION_END://版本參數區間-終點			
			return true;
			break;
		case FILE_IO_VERSION_PARAM_START://版本參數-起點
			VersionParam = TVersionParam();
			if ( ReadVersionParamFile(VersionParam) == false )
			{	return false; }
			VersionList.push_back(VersionParam);
			break;		
		default:
			break;
		}
	};
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::WriteVersionParamListFile(const std::vector<TVersionParam> &VersionList)
{
#ifdef _DEBUG
	if ( CheckFileMode() == false ) { return false; }
	if ( CheckFileOpened() == false ) { return false; }	
#endif//_DEBUG

	SetFnName(_T("CAOIFileIO::WriteVersionParamListFile"));
	//----------------------------------------------------------------------------------------//
	size_t       i=0;
	const size_t Count = VersionList.size();
	if ( SaveChunk_INT(FILE_IO_VERSION_PARAM_SECTION_START, 0) == false ) { return false; }

	for ( i=0; i<Count; i++ )
	{
		if ( WriteVersionParamFile(VersionList[i]) == false ) 
		{	return false; }
	}
	if ( SaveChunk_INT(FILE_IO_VERSION_PARAM_SECTION_END, 0) == false ) { return false; }	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::ReadSpaceBasePlaneParamFile_II(TBasePlaneParam &BasePlaneParam)
{
#ifdef _DEBUG
	if ( CheckFileMode() == false ) { return false; }
	if ( CheckFileOpened() == false ) { return false; }	
#endif//_DEBUG

	int    index = 0;
	SetFnName(_T("CAOIFileIO::ReadSpaceBasePlaneParamFile_II"));
	BasePlaneParam.BasePlaneProcType = BASE_PLANE_PROC_TYPE_1;
	BasePlaneParam.BaePlaneAutRgnMaxGap= 2000;
	BasePlaneParam.BasePlaneAutRgnMode = BASE_PLANE_AUTO_REGION_DISABLE;
	while ( CheckFileEnd()==false )
	{ 		
		if ( LoadChunk(index) == false ) 		
		{	continue; }		
		
		switch ( index )
		{
		case FILE_IO_3D_BASE_PLANE_END_II://3D基面參數-終點
			return true;
			break;		

		case FILE_IO_3D_BASE_PLANE_BASE_PROC_TYPE_II://3D基面參數-基準程序樣式				
			BasePlaneParam.BasePlaneProcType = (BASE_PLANE_PROC_TYPE)(GetData_INT());
			break;
		case FILE_IO_3D_BASE_PLANE_CALC_BASE_MODE_II://3D基面參數-計算基準模式		
			BasePlaneParam.CalcBasePlaneMode = (CALC_BASE_PLANE_MODE)(GetData_INT());
			break;		
		case FILE_IO_3D_BASE_PLANE_MAX_TILT_ANGLE_II://3D基面參數-最多傾斜角		
			BasePlaneParam.MaxTiltAngle = GetData_DBL();
			break;			
		case FILE_IO_3D_BASE_PLANE_SYS_NOISE_RANGE_II://3D基面參數-系統雜訊範圍		
			BasePlaneParam.SystemNoiseRange = GetData_DBL();
			break;
		case FILE_IO_3D_BASE_PLANE_UPPER_RATIO_II://3D基面參數-上範圍比例		
			BasePlaneParam.UpperRatio = GetData_DBL();
			break;
		case FILE_IO_3D_BASE_PLANE_LOWER_RATIO_II://3D基面參數-下範圍比例		
			BasePlaneParam.LowerRatio = GetData_DBL();
			break;
		case FILE_IO_3D_BASE_PLANE_OFFSET_Z_II://3D基面參數-最後Z軸偏差量		
			BasePlaneParam.OffsetZ = GetData_DBL();
			break;
		case FILE_IO_3D_BASE_PLANE_PLANE_RATIO_LSL_II://3D基面參數-平面滿足最小比例		
			BasePlaneParam.PlaneRatioLSL = GetData_DBL();
			if ( BasePlaneParam.PlaneRatioLSL < 0 ) { BasePlaneParam.PlaneRatioLSL = 0; }
			if ( BasePlaneParam.PlaneRatioLSL > 100 ) { BasePlaneParam.PlaneRatioLSL = 100; }
			break;
			
		case FILE_IO_3D_BASE_PLANE_PLANE_RATIO_USL_II://3D基面參數-平面不可超過比例		
			BasePlaneParam.PlaneRatioUSL = GetData_DBL();
			if ( BasePlaneParam.PlaneRatioUSL < 0 ) { BasePlaneParam.PlaneRatioUSL = 0; }
			if ( BasePlaneParam.PlaneRatioUSL > 100 ) { BasePlaneParam.PlaneRatioUSL = 100; }
			break;
		case FILE_IO_3D_BASE_PLANE_RANGE_RATIO_MIN_II://3D基面參數-範圍比例下限		
			BasePlaneParam.RangeRatioMin = GetData_DBL();
			if ( BasePlaneParam.RangeRatioMin < 0 ) { BasePlaneParam.RangeRatioMin = 0; }
			if ( BasePlaneParam.RangeRatioMin > 100 ) { BasePlaneParam.RangeRatioMin = 100; }
			break;
		case FILE_IO_3D_BASE_PLANE_RANGE_RATIO_MAX_II://3D基面參數-範圍比例上限		
			BasePlaneParam.RangeRatioMax = GetData_DBL();
			if ( BasePlaneParam.RangeRatioMax < 0 ) { BasePlaneParam.RangeRatioMax = 0; }
			if ( BasePlaneParam.RangeRatioMax > 100 ) { BasePlaneParam.RangeRatioMax = 100; }
			break;
		case FILE_IO_3D_BASE_PLANE_OVER_HIGH_FILTER_II://3D基面參數-過高剔除		
			BasePlaneParam.OverHighFilter = GetData_DBL();
			break;
		case FILE_IO_3D_BASE_PLANE_OVER_LOW_FILTER_II://3D基面參數-過低剔除		
			BasePlaneParam.OverLowFilter = GetData_DBL();
			break;
		case FILE_IO_3D_BASE_PLANE_USE_SIDE_MODE_II://3D基面參數-四邊篩選		
			BasePlaneParam.UseSideMode = GetData_INT();
			break;		
		case FILE_IO_3D_BASE_PLANE_CLIP_ROTATED_OUTER://3D基面參數-切除斜角度外圍
			BasePlaneParam.RotatedClip = GetData_BOL();
			break;
		case FILE_IO_3D_BASE_PLANE_TOWARD_MODE://3D基面參數-朝向模式
			BasePlaneParam.BasePlaneToward = (BASE_PLANE_TOWARD_MODE)(GetData_INT());
			break;
		case FILE_IO_3D_BASE_PLANE_USE_INNER_MODE://3D基面參數-使用內部
			BasePlaneParam.UseInnerMode = GetData_BOL();
			break;
		case FILE_IO_3D_BASE_PLANE_AUTO_REGION_MODE://3D基面參數-自動區域模式			
			BasePlaneParam.BasePlaneAutRgnMode = (BASE_PLANE_AUTO_REGION_MODE)(GetData_INT());
			break;

		case FILE_IO_3D_BASE_PLANE_2DMASK_BASE_ENABLE://3D基面參數-基準面2D遮罩-啟用
			BasePlaneParam.BasePlane2DMaskEnabled = GetData_BOL();
			break;
		case FILE_IO_3D_BASE_PLANE_2DMASK_FRAME_UNIQUE_ID://3D基面參數-基準面2D遮罩-唯一碼
			BasePlaneParam.BasePlane2DMaskFrameUniqueID = (unsigned int)(GetData_INT());
			break;
		case FILE_IO_3D_BASE_PLANE_2DMASK_COLOR_GRAOUP_INDEX://3D基面參數-基準面2D遮罩-專案顏色序號
			BasePlaneParam.BasePlane2DMaskGroupLinkIndex = GetData_INT();
			break;

		case FILE_IO_3D_BASE_PLANE_AUTO_REGION_MAX_GAP://3D基面參數-自動選區最大差距
			BasePlaneParam.BaePlaneAutRgnMaxGap = GetData_DBL();			
			break;

		case FILE_IO_3D_BASE_PLANE_BODY_OUTSIDE_WIDTH://3D基面參數-本體外圍寬度-um
			BasePlaneParam.BodyOutsideW = GetData_INT();
			break;
		case FILE_IO_3D_BASE_PLANE_BODY_OUTSIDE_HEIGHT://3D基面參數-本體外圍高度-um
			BasePlaneParam.BodyOutsideH = GetData_INT();
			break;
		case FILE_IO_3D_BASE_PLANE_BODY_OUTSIDE_ENABLED://3D基面參數-本體外圍啟用
			BasePlaneParam.BodyOutsideMode = (BASE_PLANE_BODY_OUTSIDE_MODE)(GetData_INT());	
			break;

		case FILE_IO_3D_BASE_PLANE_FILTER_MODE_II://3D基面參數-濾波模式		
			BasePlaneParam.FilterMode = GetData_INT();
			break;
		case FILE_IO_3D_BASE_PLANE_FILTER_SIZE_II://3D基面參數-濾波尺寸		
			BasePlaneParam.FilterKerSize = GetData_INT();
			break;
		case FILE_IO_3D_BASE_PLANE_FILTER_INTER_CNT_II://3D基面參數-濾波疊代次數		
			BasePlaneParam.FilterIterCount = GetData_INT();
			break;
		case FILE_IO_3D_BASE_PLANE_FILTER_PITCH_II://3D基面參數-濾波間距		
			BasePlaneParam.FilterPitch = GetData_INT();
			break;
		case FILE_IO_3D_BASE_PLANE_FILTER_USE_SIZE_II://3D基面參數-濾波使用尺寸		
			BasePlaneParam.FilterUseSize = GetData_INT();
			break;

		case FILE_IO_3D_BASE_PLANE_2D_FILTER_MODE_II://3D基面參數-2D濾波模式		
			BasePlaneParam.FilterMode2D = GetData_INT();
			break;
		case FILE_IO_3D_BASE_PLANE_2D_FILTER_SIZE_II://3D基面參數-2D濾波尺寸		
			BasePlaneParam.FilterKerSize2D = GetData_INT();
			break;

		default:
			break;
		}		
	};	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::WriteSpaceBasePlaneParamFile_II(const TBasePlaneParam &BasePlaneParam)
{
#ifdef _DEBUG
	if ( CheckFileMode() == false ) { return false; }
	if ( CheckFileOpened() == false ) { return false; }	
#endif//_DEBUG

	SetFnName(_T("CAOIFileIO::WriteSpaceBasePlaneParamFile_II"));
	//----------------------------------------------------------------------------------------//	
	//基準面
	if ( SaveChunk_INT(FILE_IO_3D_BASE_PLANE_START_II, 0) == false ) { return false; }

	if ( SaveChunk_INT(FILE_IO_3D_BASE_PLANE_BASE_PROC_TYPE_II, BasePlaneParam.BasePlaneProcType) == false ) { return false; }
	if ( SaveChunk_INT(FILE_IO_3D_BASE_PLANE_CALC_BASE_MODE_II, BasePlaneParam.CalcBasePlaneMode) == false ) { return false; }
	if ( SaveChunk_DBL(FILE_IO_3D_BASE_PLANE_MAX_TILT_ANGLE_II, BasePlaneParam.MaxTiltAngle) == false ) { return false; }
	if ( SaveChunk_DBL(FILE_IO_3D_BASE_PLANE_SYS_NOISE_RANGE_II, BasePlaneParam.SystemNoiseRange) == false ) { return false; }
	if ( SaveChunk_DBL(FILE_IO_3D_BASE_PLANE_UPPER_RATIO_II, BasePlaneParam.UpperRatio) == false ) { return false; }
	if ( SaveChunk_DBL(FILE_IO_3D_BASE_PLANE_LOWER_RATIO_II, BasePlaneParam.LowerRatio) == false ) { return false; }
	if ( SaveChunk_DBL(FILE_IO_3D_BASE_PLANE_OFFSET_Z_II, BasePlaneParam.OffsetZ) == false ) { return false; }
	if ( SaveChunk_DBL(FILE_IO_3D_BASE_PLANE_PLANE_RATIO_LSL_II, BasePlaneParam.PlaneRatioLSL) == false ) { return false; }
	if ( SaveChunk_DBL(FILE_IO_3D_BASE_PLANE_PLANE_RATIO_USL_II, BasePlaneParam.PlaneRatioUSL) == false ) { return false; }
	if ( SaveChunk_DBL(FILE_IO_3D_BASE_PLANE_RANGE_RATIO_MIN_II, BasePlaneParam.RangeRatioMin) == false ) { return false; }
	if ( SaveChunk_DBL(FILE_IO_3D_BASE_PLANE_RANGE_RATIO_MAX_II, BasePlaneParam.RangeRatioMax) == false ) { return false; }
	if ( SaveChunk_DBL(FILE_IO_3D_BASE_PLANE_OVER_HIGH_FILTER_II, BasePlaneParam.OverHighFilter) == false ) { return false; }	
	if ( SaveChunk_DBL(FILE_IO_3D_BASE_PLANE_OVER_LOW_FILTER_II, BasePlaneParam.OverLowFilter) == false ) { return false; }
	if ( SaveChunk_INT(FILE_IO_3D_BASE_PLANE_USE_SIDE_MODE_II, BasePlaneParam.UseSideMode) == false ) { return false; }	  
	if ( SaveChunk_BOL(FILE_IO_3D_BASE_PLANE_CLIP_ROTATED_OUTER, BasePlaneParam.RotatedClip) == false ) { return false; }	  	
	if ( SaveChunk_INT(FILE_IO_3D_BASE_PLANE_TOWARD_MODE, BasePlaneParam.BasePlaneToward) == false ) { return false; }
	if ( SaveChunk_BOL(FILE_IO_3D_BASE_PLANE_USE_INNER_MODE, BasePlaneParam.UseInnerMode) == false ) { return false; }	
	if ( SaveChunk_INT(FILE_IO_3D_BASE_PLANE_AUTO_REGION_MODE, BasePlaneParam.BasePlaneAutRgnMode) == false ) { return false; }	

	if ( SaveChunk_BOL(FILE_IO_3D_BASE_PLANE_2DMASK_BASE_ENABLE, BasePlaneParam.BasePlane2DMaskEnabled) == false ) { return false; }
	if ( SaveChunk_INT(FILE_IO_3D_BASE_PLANE_2DMASK_FRAME_UNIQUE_ID, BasePlaneParam.BasePlane2DMaskFrameUniqueID) == false ) { return false; }
	if ( SaveChunk_INT(FILE_IO_3D_BASE_PLANE_2DMASK_COLOR_GRAOUP_INDEX, BasePlaneParam.BasePlane2DMaskGroupLinkIndex) == false ) { return false; }

	if ( SaveChunk_DBL(FILE_IO_3D_BASE_PLANE_AUTO_REGION_MAX_GAP, BasePlaneParam.BaePlaneAutRgnMaxGap) == false ) { return false; }		

	if ( SaveChunk_INT(FILE_IO_3D_BASE_PLANE_BODY_OUTSIDE_WIDTH, BasePlaneParam.BodyOutsideW) == false ) { return false; }
	if ( SaveChunk_INT(FILE_IO_3D_BASE_PLANE_BODY_OUTSIDE_HEIGHT, BasePlaneParam.BodyOutsideH) == false ) { return false; }
	if ( SaveChunk_INT(FILE_IO_3D_BASE_PLANE_BODY_OUTSIDE_ENABLED, BasePlaneParam.BodyOutsideMode) == false ) { return false; }

	if ( SaveChunk_INT(FILE_IO_3D_BASE_PLANE_FILTER_MODE_II, BasePlaneParam.FilterMode) == false ) { return false; }
	if ( SaveChunk_INT(FILE_IO_3D_BASE_PLANE_FILTER_SIZE_II, BasePlaneParam.FilterKerSize) == false ) { return false; }
	if ( SaveChunk_INT(FILE_IO_3D_BASE_PLANE_FILTER_INTER_CNT_II, BasePlaneParam.FilterIterCount) == false ) { return false; }	
	if ( SaveChunk_INT(FILE_IO_3D_BASE_PLANE_FILTER_PITCH_II, BasePlaneParam.FilterPitch) == false ) { return false; }	
	if ( SaveChunk_INT(FILE_IO_3D_BASE_PLANE_FILTER_USE_SIZE_II, BasePlaneParam.FilterUseSize) == false ) { return false; }
	
	if ( SaveChunk_INT(FILE_IO_3D_BASE_PLANE_2D_FILTER_MODE_II, BasePlaneParam.FilterMode2D) == false ) { return false; }
	if ( SaveChunk_INT(FILE_IO_3D_BASE_PLANE_2D_FILTER_SIZE_II, BasePlaneParam.FilterKerSize2D) == false ) { return false; }

	if ( SaveChunk_INT(FILE_IO_3D_BASE_PLANE_END_II, 0) == false ) { return false; }	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::ReadSpaceNoiseFilterParamFile(TNoiseFilterParam &NoiseFilterParam)
{
#ifdef _DEBUG
	if ( CheckFileMode() == false ) { return false; }
	if ( CheckFileOpened() == false ) { return false; }	
#endif//_DEBUG

	int    index = 0;
	SetFnName(_T("CAOIFileIO::ReadSpaceNoiseFilterParamFile"));
	NoiseFilterParam.BasePlaneParam.BasePlaneProcType = BASE_PLANE_PROC_TYPE_1;
	while ( CheckFileEnd()==false )
	{ 		
		if ( LoadChunk(index) == false ) 		
		{	continue; }		
		
		switch ( index )
		{
		case FILE_IO_3D_NOISE_FILTER_START://3D過濾參數-起點			
			if ( ReadSpaceNoiseFilterParamFile_I(NoiseFilterParam) == false )
			{	return false; }
			return true;
			break;
		case FILE_IO_3D_BASE_PLANE_START_II://3D基面參數-起點
			if ( ReadSpaceBasePlaneParamFile_II(NoiseFilterParam.BasePlaneParam) == false )
			{	return false; }
			break;
		case FILE_IO_3D_NOISE_FILTER_START_II://3D過濾參數-起點
			if ( ReadSpaceNoiseFilterParamFile_II(NoiseFilterParam) == false )
			{	return false; }
			return true;
			break;		
		default:
			break;
		}		
	};	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::ReadSpaceNoiseFilterParamFile_I(TNoiseFilterParam &NoiseFilterParam)
{
#ifdef _DEBUG
	if ( CheckFileMode() == false ) { return false; }
	if ( CheckFileOpened() == false ) { return false; }	
#endif//_DEBUG

	int    index = 0;
	SetFnName(_T("CAOIFileIO::ReadSpaceNoiseFilterParamFile_I"));
	NoiseFilterParam.BasePlaneParam.BasePlaneProcType = BASE_PLANE_PROC_TYPE_1;
	while ( CheckFileEnd()==false )
	{ 		
		if ( LoadChunk(index) == false ) 		
		{	continue; }		
		
		switch ( index )
		{		
		case FILE_IO_3D_NOISE_FILTER_END://過濾參數-終點			
			return true;
			break;

		case FILE_IO_3D_NOISE_FILTER_BASE_BASE_PROC_TYPE://3D基面參數-基準程序樣式		
			NoiseFilterParam.BasePlaneParam.BasePlaneProcType = (BASE_PLANE_PROC_TYPE)(GetData_INT());
			break;
		case FILE_IO_3D_NOISE_FILTER_BASE_CALC_BASE_MODE://3D基面參數-計算基準模式
			NoiseFilterParam.BasePlaneParam.CalcBasePlaneMode = (CALC_BASE_PLANE_MODE)(GetData_INT());
			break;
		case FILE_IO_3D_NOISE_FILTER_BASE_MAX_TILT_ANGLE://3D基面參數-最多傾斜角
			NoiseFilterParam.BasePlaneParam.MaxTiltAngle = GetData_DBL();
			break;
		case FILE_IO_3D_NOISE_FILTER_BASE_SYS_NOISE_RANGE://3D基面參數-系統雜訊範圍
			NoiseFilterParam.BasePlaneParam.SystemNoiseRange = GetData_DBL();
			break;
		case FILE_IO_3D_NOISE_FILTER_BASE_UPPER_RATIO://3D基面參數-上範圍比例
			NoiseFilterParam.BasePlaneParam.UpperRatio = GetData_DBL();
			break;
		case FILE_IO_3D_NOISE_FILTER_BASE_LOWER_RATIO://3D基面參數-下範圍比例
			NoiseFilterParam.BasePlaneParam.LowerRatio = GetData_DBL();
			break;
		case FILE_IO_3D_NOISE_FILTER_BASE_OFFSET_Z://3D基面參數-最後Z軸偏差量
			NoiseFilterParam.BasePlaneParam.OffsetZ = GetData_DBL();
			break;
		case FILE_IO_3D_NOISE_FILTER_BASE_PLANE_RATIO_LSL://3D基面參數-平面滿足最小比例
			NoiseFilterParam.BasePlaneParam.PlaneRatioLSL = GetData_DBL();
			if ( NoiseFilterParam.BasePlaneParam.PlaneRatioLSL < 0 ) { NoiseFilterParam.BasePlaneParam.PlaneRatioLSL = 0; }
			if ( NoiseFilterParam.BasePlaneParam.PlaneRatioLSL > 100 ) { NoiseFilterParam.BasePlaneParam.PlaneRatioLSL = 100; }
			break;
		case FILE_IO_3D_NOISE_FILTER_BASE_PLANE_RATIO_USL://3D基面參數-平面不可超過比例
			NoiseFilterParam.BasePlaneParam.PlaneRatioUSL = GetData_DBL();
			if ( NoiseFilterParam.BasePlaneParam.PlaneRatioUSL < 0 ) { NoiseFilterParam.BasePlaneParam.PlaneRatioUSL = 0; }
			if ( NoiseFilterParam.BasePlaneParam.PlaneRatioUSL > 100 ) { NoiseFilterParam.BasePlaneParam.PlaneRatioUSL = 100; }
			break;
		case FILE_IO_3D_NOISE_FILTER_BASE_RANGE_RATIO_MIN://3D基面參數-範圍比例下限
			NoiseFilterParam.BasePlaneParam.RangeRatioMin = GetData_DBL();
			if ( NoiseFilterParam.BasePlaneParam.RangeRatioMin < 0 ) { NoiseFilterParam.BasePlaneParam.RangeRatioMin = 0; }
			if ( NoiseFilterParam.BasePlaneParam.RangeRatioMin > 100 ) { NoiseFilterParam.BasePlaneParam.RangeRatioMin = 100; }
			break;
		case FILE_IO_3D_NOISE_FILTER_BASE_RANGE_RATIO_MAX://3D基面參數-範圍比例上限
			NoiseFilterParam.BasePlaneParam.RangeRatioMax = GetData_DBL();
			if ( NoiseFilterParam.BasePlaneParam.RangeRatioMax < 0 ) { NoiseFilterParam.BasePlaneParam.RangeRatioMax = 0; }
			if ( NoiseFilterParam.BasePlaneParam.RangeRatioMax > 100 ) { NoiseFilterParam.BasePlaneParam.RangeRatioMax = 100; }
			break;		
		case FILE_IO_3D_NOISE_FILTER_BASE_FILTER_MODE://3D基面參數-濾波模式
			NoiseFilterParam.BasePlaneParam.FilterMode = GetData_INT();
			break;
		case FILE_IO_3D_NOISE_FILTER_BASE_FILTER_SIZE://3D基面參數-濾波尺寸
			NoiseFilterParam.BasePlaneParam.FilterKerSize = GetData_INT();
			break;
		case FILE_IO_3D_NOISE_FILTER_BASE_FILTER_INTER_CNT://3D基面參數-濾波疊代次數
			NoiseFilterParam.BasePlaneParam.FilterIterCount = GetData_INT();
			break;
		case FILE_IO_3D_NOISE_FILTER_BASE_FILTER_PITCH://3D基面參數-濾波間距
			NoiseFilterParam.BasePlaneParam.FilterPitch = GetData_INT();
			break;
		case FILE_IO_3D_NOISE_FILTER_BASE_FILTER_USE_SIZE://3D基面參數-濾波使用尺寸
			NoiseFilterParam.BasePlaneParam.FilterUseSize = GetData_INT();
			break;

		case FILE_IO_3D_NOISE_FILTER_BASE_OVER_HIGH_FILTER://3D基面參數-過高剔除
			NoiseFilterParam.BasePlaneParam.OverHighFilter = GetData_DBL();
			break;
		case FILE_IO_3D_NOISE_FILTER_BASE_OVER_LOW_FILTER://3D基面參數-過低剔除
			NoiseFilterParam.BasePlaneParam.OverLowFilter = GetData_DBL();
			break;
		case FILE_IO_3D_NOISE_FILTER_BASE_USE_SIDE_MODE://3D基面參數-四邊篩選
			NoiseFilterParam.BasePlaneParam.UseSideMode = GetData_INT();
			break;		
		
		case FILE_IO_3D_NOISE_FILTER_DATA_LINK_INDEX://3D過濾參數-資料-連接引數
			NoiseFilterParam.DataFilterIndex = GetData_INT();
			break;
		case FILE_IO_3D_NOISE_FILTER_DATA_INFO_TEXT://3D過濾參數-資料-資訊文字
			if ( GetLoadWStr()==true )
			{	NoiseFilterParam.DataFilterInfoText = GetData_WSTR();	}
			else
			{	NoiseFilterParam.DataFilterInfoText = GetData_STR();	}
			break;

		case FILE_IO_3D_NOISE_FILTER_DATA_VOID_EXPAND_ENABLE://過濾參數-資料-無效點外擴啟用
			NoiseFilterParam.DataVoidExpandEnabled = GetData_BOL();
			break;		
		case FILE_IO_3D_NOISE_FILTER_DATA_VOID_EXPAND_SIZE://過濾參數-資料-無效點外擴尺寸
			NoiseFilterParam.DataVoidExpandSize = GetData_INT();
			break;

		case FILE_IO_3D_NOISE_FILTER_DATA_FIRST_FILTER_MODE://3D過濾參數-資料-首次濾波模式
			NoiseFilterParam.DataFirstFilterMode = GetData_INT();
			break;		
		case FILE_IO_3D_NOISE_FILTER_DATA_FIRST_KERNEL_SIZE://3D過濾參數-資料-首次濾波尺寸
			NoiseFilterParam.DataFirstFilterKerSize = GetData_INT();
			break;		
		case FILE_IO_3D_NOISE_FILTER_DATA_FIRST_USE_SIZE://3D過濾參數-資料-首次使用尺寸
			NoiseFilterParam.DataFirstFilterUseSize = GetData_INT();
			break;		
		case FILE_IO_3D_NOISE_FILTER_DATA_FIRST_FILTER_PITCH://3D過濾參數-資料-首次濾波步長
			NoiseFilterParam.DataFirstFilterPitch = GetData_INT();
			break;		
			NoiseFilterParam.DataFirstFilterSearchOn = GetData_BOL();
			break;
		
		case FILE_IO_3D_NOISE_FILTER_DATA_HEIGHT_VAR_MODE://過濾參數-資料-高度變異過濾模式
			NoiseFilterParam.DataHeightFTMode = GetData_INT();
			break;
		case FILE_IO_3D_NOISE_FILTER_DATA_HEIGHT_VAR_RANGE://過濾參數-資料-高度變異過濾範圍
			NoiseFilterParam.DataHeightFTRange = GetData_DBL();
			break;
		case FILE_IO_3D_NOISE_FILTER_DATA_HEIGHT_VAR_CHK_SIZE://3D過濾參數-資料-高度變異確認尺寸
			NoiseFilterParam.DataHeightFTChkSize = GetData_INT();
			break;
		case FILE_IO_3D_NOISE_FILTER_DATA_HEIGHT_VAR_USE_SIZE://過濾參數-資料-高度變異使用尺寸
			NoiseFilterParam.DataHeightFTUseSize = GetData_INT();
			break;
		case FILE_IO_3D_NOISE_FILTER_DATA_HEIGHT_VAR_KER_SIZE://過濾參數-資料-高度變異過濾尺寸
			NoiseFilterParam.DataHeightFTKerSize = GetData_INT();
			break;
		case FILE_IO_3D_NOISE_FILTER_DATA_HEIGHT_VAR_REPEAT://3D過濾參數-資料-高度變異過濾次數
			NoiseFilterParam.DataHeightFTRepeatCnt = GetData_INT();
			break;
		case FILE_IO_3D_NOISE_FILTER_DATA_HEIGHT_VAR_PITCH://3D過濾參數-資料-高度變異過濾步長
			NoiseFilterParam.DataHeightFTPitch = GetData_INT();
			break;

		case FILE_IO_3D_NOISE_FILTER_DATA_OVER_LOW_MODE://過濾參數-資料-過低過濾模式
			NoiseFilterParam.DataOverLowFTMode = GetData_INT();
			break;
		case FILE_IO_3D_NOISE_FILTER_DATA_OVER_LOW_RANGE://過濾參數-資料-過低過濾範圍
			NoiseFilterParam.DataOverLowFTRange = GetData_DBL();
			NoiseFilterParam.DataOverLowFTLimit = NoiseFilterParam.DataOverLowFTRange;
			break;
		case FILE_IO_3D_NOISE_FILTER_DATA_OVER_LOW_LIMIT://3D過濾參數-資料-過低極限範圍
			NoiseFilterParam.DataOverLowFTLimit = GetData_DBL();
			break;
		case FILE_IO_3D_NOISE_FILTER_DATA_OVER_LOW_KER_SIZE://3D過濾參數-資料-過低濾波尺寸
			NoiseFilterParam.DataOverLowFTKerSize = GetData_INT();
			break;
		case FILE_IO_3D_NOISE_FILTER_DATA_OVER_LOW_USE_SIZE://3D過濾參數-資料-過低使用尺寸
			NoiseFilterParam.DataOverLowFTUseSize = GetData_INT();
			break;

		case FILE_IO_3D_NOISE_FILTER_DATA_VOID_RECONTRUCT_ENABLE://過濾參數-資料-無效點重建啟用
			NoiseFilterParam.DataVoidReContructed = GetData_BOL();
			break;
		case FILE_IO_3D_NOISE_FILTER_DATA_VOID_RECONTRUCT_EXT_RANGE://過濾參數-資料-無效點重建尺寸
			NoiseFilterParam.DataVoidReContructedExtSize = GetData_INT();
			break;
		
		case FILE_IO_3D_NOISE_FILTER_DATA_FINAL_FILTER_MODE://過濾參數-資料-後製濾波啟用
			NoiseFilterParam.DataFinalFilterMode = GetData_INT();
			break;
		case FILE_IO_3D_NOISE_FILTER_DATA_FINAL_KERNEL_SIZE://過濾參數-資料-後製濾波尺寸
			NoiseFilterParam.DataFinalFilterKerSize = GetData_INT();			
			break;
		case FILE_IO_3D_NOISE_FILTER_DATA_FINAL_USE_SIZE://3D過濾參數-資料-後製使用尺寸
			NoiseFilterParam.DataFinalFilterUseSize = GetData_INT();
			break;
		case FILE_IO_3D_NOISE_FILTER_DATA_FINAL_PITCH://3D過濾參數-資料-後製濾波步長
			NoiseFilterParam.DataFinalFilterPitch = GetData_INT();
			break;
		
		case FILE_IO_3D_NOISE_FILTER_DATA_FINAL_FILTER_MODE2://過濾參數-資料-後製濾波啟用-2
			NoiseFilterParam.DataFinalFilterMode2 = GetData_INT();
			break;
		case FILE_IO_3D_NOISE_FILTER_DATA_FINAL_KERNEL_SIZE2://過濾參數-資料-後製濾波尺寸-2
			NoiseFilterParam.DataFinalFilterKerSize2 = GetData_INT();			
			break;
		case FILE_IO_3D_NOISE_FILTER_DATA_FINAL_USE_SIZE2://3D過濾參數-資料-後製使用尺寸-2
			NoiseFilterParam.DataFinalFilterUseSize2 = GetData_INT();
			break;
		case FILE_IO_3D_NOISE_FILTER_DATA_FINAL_PITCH2://3D過濾參數-資料-後製濾波步長-2		
			NoiseFilterParam.DataFinalFilterPitch2 = GetData_INT();
			break;
		default:
			break;
		}		
	};	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::WriteSpaceNoistFilterParamFile(const TNoiseFilterParam &NoiseFilterParam)
{
#ifdef _DEBUG
	if ( CheckFileMode() == false ) { return false; }
	if ( CheckFileOpened() == false ) { return false; }	
#endif//_DEBUG

	SetFnName(_T("CAOIFileIO::WriteSpaceNoistFilterParamFile"));
	//----------------------------------------------------------------------------------------//	
	//基準面
	if ( WriteSpaceBasePlaneParamFile_II(NoiseFilterParam.BasePlaneParam) == false ) { return false; }	
	
	//3D資料	
	if ( WriteSpaceNoistFilterParamFile_II(NoiseFilterParam) == false ) { return false; }	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::ReadSpaceNoiseFilterParamFile_II(TNoiseFilterParam &NoiseFilterParam)
{

#ifdef _DEBUG
	if ( CheckFileMode() == false ) { return false; }
	if ( CheckFileOpened() == false ) { return false; }	
#endif//_DEBUG

	int    index = 0;
	SetFnName(_T("CAOIFileIO::ReadSpaceNoiseFilterParamFile_II"));	
	while ( CheckFileEnd()==false )
	{ 		
		if ( LoadChunk(index) == false ) 		
		{	continue; }		
		
		switch ( index )
		{
		case FILE_IO_3D_NOISE_FILTER_END_II://3D過濾參數-終點
			return true;
			break;		
		case FILE_IO_3D_NOISE_FILTER_LINK_INDEX_II://3D過濾參數-連接引數		
			NoiseFilterParam.DataFilterIndex = GetData_INT();
			break;
		case FILE_IO_3D_NOISE_FILTER_INFO_TEXT_II://3D過濾參數-資訊文字
			if ( GetLoadWStr()==true )
			{	NoiseFilterParam.DataFilterInfoText = GetData_WSTR();	}
			else
			{	NoiseFilterParam.DataFilterInfoText = GetData_STR();	}
			break;

		case FILE_IO_3D_NOISE_FILTER_HEIGHT_CORRECT_II://3D過濾參數-高度校正
			NoiseFilterParam.DataCorrectMode = (HEIGHT_DATA_CORRECT_MODE)(GetData_INT());
			break;

		case FILE_IO_3D_NOISE_FILTER_VOID_EXPAND_ENABLE_II://3D過濾參數-無效點外擴啟用
			NoiseFilterParam.DataVoidExpandEnabled = GetData_BOL();
			break;
		case FILE_IO_3D_NOISE_FILTER_VOID_EXPAND_SIZE_II://3D過濾參數-無效點外擴尺寸
			NoiseFilterParam.DataVoidExpandSize = GetData_INT();
			break;

		case FILE_IO_3D_NOISE_FILTER_FIRST_FILTER_MODE_II://3D過濾參數-首次濾波模式
			NoiseFilterParam.DataFirstFilterMode = GetData_INT();
			break;
		case FILE_IO_3D_NOISE_FILTER_FIRST_KERNEL_SIZE_II://3D過濾參數-首次濾波尺寸
			NoiseFilterParam.DataFirstFilterKerSize = GetData_INT();
			break;
		case FILE_IO_3D_NOISE_FILTER_FIRST_USE_SIZE_II://3D過濾參數-首次使用尺寸
			NoiseFilterParam.DataFirstFilterUseSize = GetData_INT();
			break;
		case FILE_IO_3D_NOISE_FILTER_FIRST_FILTER_PITCH_II://3D過濾參數-首次濾波步長
			NoiseFilterParam.DataFirstFilterPitch = GetData_INT();
			break;
		case FILE_IO_3D_NOISE_FILTER_FIRST_SEARCHON_II://3D過濾參數-搜尋相似高度啟用		
			NoiseFilterParam.DataFirstFilterSearchOn = GetData_BOL();
			break;
		case FILE_IO_3D_NOISE_FILTER_FIRST_ALPHAF_II://3D過濾參數-搜尋相似高度
			NoiseFilterParam.DataFirstFilterAlphaF = GetData_INT();
			break;
		case FILE_IO_3D_NOISE_FILTER_FIRST_ALPHAS_II://3D過濾參數-使用相似高度
			NoiseFilterParam.DataFirstFilterAlphaS = GetData_INT();
			break;
		case FILE_IO_3D_NOISE_FILTER_FIRST_ALPHAM_II://3D過濾參數-使用自身高度
			NoiseFilterParam.DataFirstFilterAlphaM = GetData_INT();
			break;
		case FILE_IO_3D_NOISE_FILTER_FIRST_ALPHAI_II://3D過濾參數-使用自身灰階
			NoiseFilterParam.DataFirstFilterAlphaI = GetData_INT();
			break;
		case FILE_IO_3D_NOISE_FILTER_FIRST_OUTLIER_II://3D過濾參數-雜訊下限值
			NoiseFilterParam.DataFirstFilterThresdhold_Outlier = GetData_INT();
			break;

		case FILE_IO_3D_NOISE_FILTER_HEIGHT_VAR_MODE_II://3D過濾參數-高度變異過濾模式
			NoiseFilterParam.DataHeightFTMode = GetData_INT();
			break;
		case FILE_IO_3D_NOISE_FILTER_HEIGHT_VAR_RANGE_II://3D過濾參數-高度變異過濾範圍
			NoiseFilterParam.DataHeightFTRange = GetData_DBL();
			break;
		case FILE_IO_3D_NOISE_FILTER_HEIGHT_VAR_CHK_SIZE_II://3D過濾參數-高度變異確認尺寸
			NoiseFilterParam.DataHeightFTChkSize = GetData_INT();
			break;
		case FILE_IO_3D_NOISE_FILTER_HEIGHT_VAR_USE_SIZE_II://3D過濾參數-高度變異使用尺寸
			NoiseFilterParam.DataHeightFTUseSize = GetData_INT();
			break;
		case FILE_IO_3D_NOISE_FILTER_HEIGHT_VAR_KER_SIZE_II://3D過濾參數-高度變異過濾尺寸
			NoiseFilterParam.DataHeightFTKerSize = GetData_INT();
			break;
		case FILE_IO_3D_NOISE_FILTER_HEIGHT_VAR_REPEAT_II://3D過濾參數-高度變異過濾次數
			NoiseFilterParam.DataHeightFTRepeatCnt = GetData_INT();
			break;
		case FILE_IO_3D_NOISE_FILTER_HEIGHT_VAR_PITCH_II://3D過濾參數-高度變異過濾步長
			NoiseFilterParam.DataHeightFTPitch = GetData_INT();
			break;
			
		case FILE_IO_3D_NOISE_FILTER_OVER_LOW_MODE_II://3D過濾參數-過低過濾模式
			NoiseFilterParam.DataOverLowFTMode = GetData_INT();
			break;		
		case FILE_IO_3D_NOISE_FILTER_OVER_LOW_RANGE_II://3D過濾參數-過低過濾範圍
			NoiseFilterParam.DataOverLowFTRange = GetData_DBL();
			NoiseFilterParam.DataOverLowFTLimit = NoiseFilterParam.DataOverLowFTRange;
			break;
		case FILE_IO_3D_NOISE_FILTER_OVER_LOW_LIMIT_II://3D過濾參數-過低極限範圍
			NoiseFilterParam.DataOverLowFTLimit = GetData_DBL();
			break;		
		case FILE_IO_3D_NOISE_FILTER_OVER_LOW_KER_SIZE_II://3D過濾參數-過低濾波尺寸
			NoiseFilterParam.DataOverLowFTKerSize = GetData_INT();
			break;
		case FILE_IO_3D_NOISE_FILTER_OVER_LOW_USE_SIZE_II://3D過濾參數-過低使用尺寸
			NoiseFilterParam.DataOverLowFTUseSize = GetData_INT();
			break;

		case FILE_IO_3D_NOISE_FILTER_VOID_RECONTRUCT_ENABLE_II://3D過濾參數-無效點重建啟用
			NoiseFilterParam.DataVoidReContructed = GetData_BOL();
			break;
		case FILE_IO_3D_NOISE_FILTER_VOID_RECONTRUCT_EXT_RANGE_II://3D過濾參數-無效點重建尺寸
			NoiseFilterParam.DataVoidReContructedExtSize = GetData_INT();
			break;

		case FILE_IO_3D_NOISE_FILTER_FINAL_FILTER_MODE_II://3D過濾參數-後製濾波模式
			NoiseFilterParam.DataFinalFilterMode = GetData_INT();
			break;		
		case FILE_IO_3D_NOISE_FILTER_FINAL_KERNEL_SIZE_II://3D過濾參數-後製濾波尺寸
			NoiseFilterParam.DataFinalFilterKerSize = GetData_INT();			
			break;
		case FILE_IO_3D_NOISE_FILTER_FINAL_USE_SIZE_II://3D過濾參數-後製使用尺寸
			NoiseFilterParam.DataFinalFilterUseSize = GetData_INT();
			break;
		case FILE_IO_3D_NOISE_FILTER_FINAL_PITCH_II://3D過濾參數-後製濾波步長
			NoiseFilterParam.DataFinalFilterPitch = GetData_INT();
			break;		
		case FILE_IO_3D_NOISE_FILTER_FINAL_SEARCHON_II://3D過濾參數-搜尋相似高度啟用
			NoiseFilterParam.DataFinalFilterSearchOn = GetData_BOL();
			break;
		case FILE_IO_3D_NOISE_FILTER_FINAL_ALPHAF_II://3D過濾參數-搜尋相似高度
			NoiseFilterParam.DataFinalFilterAlphaF = GetData_INT();
			break;
		case FILE_IO_3D_NOISE_FILTER_FINAL_ALPHAS_II://3D過濾參數-使用相似高度
			NoiseFilterParam.DataFinalFilterAlphaS = GetData_INT();
			break;
		case FILE_IO_3D_NOISE_FILTER_FINAL_ALPHAM_II://3D過濾參數-使用自身高度
			NoiseFilterParam.DataFinalFilterAlphaM = GetData_INT();
			break;
		case FILE_IO_3D_NOISE_FILTER_FINAL_ALPHAI_II://3D過濾參數-搜尋自身灰階
			NoiseFilterParam.DataFinalFilterAlphaI = GetData_INT();
			break;
		case FILE_IO_3D_NOISE_FILTER_FINAL_OUTLIER_II://3D過濾參數-雜訊下限值
			NoiseFilterParam.DataFinalFilterThresdhold_Outlier = GetData_INT();
			break;

		case FILE_IO_3D_NOISE_FILTER_FINAL_FILTER_MODE2_II://3D過濾參數-後製濾波模式-2
			NoiseFilterParam.DataFinalFilterMode2 = GetData_INT();
			break;	
		case FILE_IO_3D_NOISE_FILTER_FINAL_KERNEL_SIZE2_II://3D過濾參數-後製濾波尺寸-2
			NoiseFilterParam.DataFinalFilterKerSize2 = GetData_INT();			
			break;
		case FILE_IO_3D_NOISE_FILTER_FINAL_USE_SIZE2_II://3D過濾參數-後製使用尺寸-2
			NoiseFilterParam.DataFinalFilterUseSize2 = GetData_INT();
			break;
		case FILE_IO_3D_NOISE_FILTER_FINAL_PITCH2_II://3D過濾參數-後製濾波步長-2
			NoiseFilterParam.DataFinalFilterPitch2 = GetData_INT();
			break;
		case FILE_IO_3D_NOISE_FILTER_FINAL_SEARCHON2_II://3D過濾參數-搜尋相似高度啟用
			NoiseFilterParam.DataFinalFilterSearchOn = GetData_BOL();
			break;
		case FILE_IO_3D_NOISE_FILTER_FINAL_ALPHAF2_II://3D過濾參數-搜尋相似高度
			NoiseFilterParam.DataFinalFilterAlphaF = GetData_INT();
			break;
		case FILE_IO_3D_NOISE_FILTER_FINAL_ALPHAS2_II://3D過濾參數-使用相似高度
			NoiseFilterParam.DataFinalFilterAlphaS = GetData_INT();
			break;
		case FILE_IO_3D_NOISE_FILTER_FINAL_ALPHAM2_II://3D過濾參數-使用自身高度
			NoiseFilterParam.DataFinalFilterAlphaM = GetData_INT();
			break;
		case FILE_IO_3D_NOISE_FILTER_FINAL_ALPHAI2_II://3D過濾參數-搜尋自身灰階
			NoiseFilterParam.DataFinalFilterAlphaI = GetData_INT();
			break;
		case FILE_IO_3D_NOISE_FILTER_FINAL_OUTLIER2_II://3D過濾參數-雜訊下限值
			NoiseFilterParam.DataFinalFilterThresdhold_Outlier = GetData_INT();
			break;
		default:
			break;
		}		
	};		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::WriteSpaceNoistFilterParamFile_II(const TNoiseFilterParam &NoiseFilterParam)
{
#ifdef _DEBUG
	if ( CheckFileMode() == false ) { return false; }
	if ( CheckFileOpened() == false ) { return false; }	
#endif//_DEBUG

	SetFnName(_T("CAOIFileIO::WriteSpaceNoistFilterParamFile_II"));
	//----------------------------------------------------------------------------------------//		
	//3D資料	
	if ( SaveChunk_INT(FILE_IO_3D_NOISE_FILTER_START_II, 0) == false ) { return false; }
	
	if ( SaveChunk_INT(FILE_IO_3D_NOISE_FILTER_LINK_INDEX_II, NoiseFilterParam.DataFilterIndex) == false ) { return false; }
	if ( SaveChunk_STR(FILE_IO_3D_NOISE_FILTER_INFO_TEXT_II, NoiseFilterParam.DataFilterInfoText) == false ) { return false; }

	if ( SaveChunk_INT(FILE_IO_3D_NOISE_FILTER_HEIGHT_CORRECT_II, NoiseFilterParam.DataCorrectMode) == false ) { return false; }		

	if ( SaveChunk_BOL(FILE_IO_3D_NOISE_FILTER_VOID_EXPAND_ENABLE_II, NoiseFilterParam.DataVoidExpandEnabled) == false ) { return false; }
	if ( SaveChunk_INT(FILE_IO_3D_NOISE_FILTER_VOID_EXPAND_SIZE_II, NoiseFilterParam.DataVoidExpandSize) == false ) { return false; }	

	if ( SaveChunk_INT(FILE_IO_3D_NOISE_FILTER_FIRST_FILTER_MODE_II, NoiseFilterParam.DataFirstFilterMode) == false ) { return false; }
	if ( SaveChunk_INT(FILE_IO_3D_NOISE_FILTER_FIRST_KERNEL_SIZE_II, NoiseFilterParam.DataFirstFilterKerSize) == false ) { return false; }
	if ( SaveChunk_INT(FILE_IO_3D_NOISE_FILTER_FIRST_USE_SIZE_II, NoiseFilterParam.DataFirstFilterUseSize) == false ) { return false; }	
	if ( SaveChunk_INT(FILE_IO_3D_NOISE_FILTER_FIRST_FILTER_PITCH_II, NoiseFilterParam.DataFirstFilterPitch) == false ) { return false; }
	if ( SaveChunk_INT(FILE_IO_3D_NOISE_FILTER_FIRST_SEARCHON_II, NoiseFilterParam.DataFirstFilterSearchOn) == false) { return false; }
	if ( SaveChunk_INT(FILE_IO_3D_NOISE_FILTER_FIRST_ALPHAF_II, NoiseFilterParam.DataFirstFilterAlphaF) == false) { return false; }
	if ( SaveChunk_INT(FILE_IO_3D_NOISE_FILTER_FIRST_ALPHAS_II, NoiseFilterParam.DataFirstFilterAlphaS) == false) { return false; }
	if ( SaveChunk_INT(FILE_IO_3D_NOISE_FILTER_FIRST_ALPHAM_II, NoiseFilterParam.DataFirstFilterAlphaM) == false) { return false; }
	if ( SaveChunk_INT(FILE_IO_3D_NOISE_FILTER_FIRST_ALPHAI_II, NoiseFilterParam.DataFirstFilterAlphaI) == false) { return false; }
	if ( SaveChunk_INT(FILE_IO_3D_NOISE_FILTER_FIRST_OUTLIER_II, NoiseFilterParam.DataFirstFilterThresdhold_Outlier) == false) { return false; }

	if ( SaveChunk_INT(FILE_IO_3D_NOISE_FILTER_HEIGHT_VAR_MODE_II, NoiseFilterParam.DataHeightFTMode) == false ) { return false; }
	if ( SaveChunk_DBL(FILE_IO_3D_NOISE_FILTER_HEIGHT_VAR_RANGE_II, NoiseFilterParam.DataHeightFTRange) == false ) { return false; }
	if ( SaveChunk_INT(FILE_IO_3D_NOISE_FILTER_HEIGHT_VAR_CHK_SIZE_II, NoiseFilterParam.DataHeightFTChkSize) == false ) { return false; }
	if ( SaveChunk_INT(FILE_IO_3D_NOISE_FILTER_HEIGHT_VAR_USE_SIZE_II, NoiseFilterParam.DataHeightFTUseSize) == false ) { return false; }
	if ( SaveChunk_INT(FILE_IO_3D_NOISE_FILTER_HEIGHT_VAR_KER_SIZE_II, NoiseFilterParam.DataHeightFTKerSize) == false ) { return false; }	
	if ( SaveChunk_INT(FILE_IO_3D_NOISE_FILTER_HEIGHT_VAR_REPEAT_II, NoiseFilterParam.DataHeightFTRepeatCnt) == false ) { return false; }
	if ( SaveChunk_INT(FILE_IO_3D_NOISE_FILTER_HEIGHT_VAR_PITCH_II, NoiseFilterParam.DataHeightFTPitch) == false ) { return false; }	
	
	if ( SaveChunk_INT(FILE_IO_3D_NOISE_FILTER_OVER_LOW_MODE_II, NoiseFilterParam.DataOverLowFTMode) == false ) { return false; }
	if ( SaveChunk_DBL(FILE_IO_3D_NOISE_FILTER_OVER_LOW_RANGE_II, NoiseFilterParam.DataOverLowFTRange) == false ) { return false; }
	if ( SaveChunk_DBL(FILE_IO_3D_NOISE_FILTER_OVER_LOW_LIMIT_II, NoiseFilterParam.DataOverLowFTLimit) == false ) { return false; }
	if ( SaveChunk_INT(FILE_IO_3D_NOISE_FILTER_OVER_LOW_KER_SIZE_II, NoiseFilterParam.DataOverLowFTKerSize) == false ) { return false; }
	if ( SaveChunk_INT(FILE_IO_3D_NOISE_FILTER_OVER_LOW_USE_SIZE_II, NoiseFilterParam.DataOverLowFTUseSize) == false ) { return false; }

	if ( SaveChunk_BOL(FILE_IO_3D_NOISE_FILTER_VOID_RECONTRUCT_ENABLE_II, NoiseFilterParam.DataVoidReContructed) == false ) { return false; }
	if ( SaveChunk_INT(FILE_IO_3D_NOISE_FILTER_VOID_RECONTRUCT_EXT_RANGE_II, NoiseFilterParam.DataVoidReContructedExtSize) == false ) { return false; }

	if ( SaveChunk_INT(FILE_IO_3D_NOISE_FILTER_FINAL_FILTER_MODE_II, NoiseFilterParam.DataFinalFilterMode) == false ) { return false; }
	if ( SaveChunk_INT(FILE_IO_3D_NOISE_FILTER_FINAL_KERNEL_SIZE_II, NoiseFilterParam.DataFinalFilterKerSize) == false ) { return false; }
	if ( SaveChunk_INT(FILE_IO_3D_NOISE_FILTER_FINAL_USE_SIZE_II, NoiseFilterParam.DataFinalFilterUseSize) == false ) { return false; }	
	if ( SaveChunk_INT(FILE_IO_3D_NOISE_FILTER_FINAL_PITCH_II, NoiseFilterParam.DataFinalFilterPitch) == false ) { return false; }
	if ( SaveChunk_INT(FILE_IO_3D_NOISE_FILTER_FINAL_SEARCHON_II, NoiseFilterParam.DataFinalFilterSearchOn) == false) { return false; }
	if ( SaveChunk_INT(FILE_IO_3D_NOISE_FILTER_FINAL_ALPHAF_II, NoiseFilterParam.DataFinalFilterAlphaF) == false) { return false; }
	if ( SaveChunk_INT(FILE_IO_3D_NOISE_FILTER_FINAL_ALPHAS_II, NoiseFilterParam.DataFinalFilterAlphaS) == false) { return false; }
	if ( SaveChunk_INT(FILE_IO_3D_NOISE_FILTER_FINAL_ALPHAM_II, NoiseFilterParam.DataFinalFilterAlphaM) == false) { return false; }
	if ( SaveChunk_INT(FILE_IO_3D_NOISE_FILTER_FINAL_ALPHAI_II, NoiseFilterParam.DataFinalFilterAlphaI) == false) { return false; }
	if ( SaveChunk_INT(FILE_IO_3D_NOISE_FILTER_FINAL_OUTLIER_II, NoiseFilterParam.DataFinalFilterThresdhold_Outlier) == false) { return false; }

	if ( SaveChunk_INT(FILE_IO_3D_NOISE_FILTER_FINAL_FILTER_MODE2_II, NoiseFilterParam.DataFinalFilterMode2) == false ) { return false; }
	if ( SaveChunk_INT(FILE_IO_3D_NOISE_FILTER_FINAL_KERNEL_SIZE2_II, NoiseFilterParam.DataFinalFilterKerSize2) == false ) { return false; }
	if ( SaveChunk_INT(FILE_IO_3D_NOISE_FILTER_FINAL_USE_SIZE2_II, NoiseFilterParam.DataFinalFilterUseSize2) == false ) { return false; }	
	if ( SaveChunk_INT(FILE_IO_3D_NOISE_FILTER_FINAL_PITCH2_II, NoiseFilterParam.DataFinalFilterPitch2) == false ) { return false; }
	if ( SaveChunk_INT(FILE_IO_3D_NOISE_FILTER_FINAL_SEARCHON2_II, NoiseFilterParam.DataFinalFilterSearchOn2) == false) { return false; }
	if ( SaveChunk_INT(FILE_IO_3D_NOISE_FILTER_FINAL_ALPHAF2_II, NoiseFilterParam.DataFinalFilterAlphaF2) == false) { return false; }
	if ( SaveChunk_INT(FILE_IO_3D_NOISE_FILTER_FINAL_ALPHAS2_II, NoiseFilterParam.DataFinalFilterAlphaS2) == false) { return false; }
	if ( SaveChunk_INT(FILE_IO_3D_NOISE_FILTER_FINAL_ALPHAM2_II, NoiseFilterParam.DataFinalFilterAlphaM2) == false) { return false; }
	if ( SaveChunk_INT(FILE_IO_3D_NOISE_FILTER_FINAL_ALPHAI2_II, NoiseFilterParam.DataFinalFilterAlphaI2) == false) { return false; }
	if ( SaveChunk_INT(FILE_IO_3D_NOISE_FILTER_FINAL_OUTLIER2_II, NoiseFilterParam.DataFinalFilterThresdhold_Outlier2) == false) { return false; }
	
	if ( SaveChunk_INT(FILE_IO_3D_NOISE_FILTER_END_II, 0) == false ) { return false; }
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::ReadXYDotNodeCaliFile(int &CountX, int &CountY, std::vector<TDotNode> &List)//讀取XY校正檔案
{
#ifdef _DEBUG
	if ( CheckFileMode() == false ) { return false; }
	if ( CheckFileOpened() == false ) { return false; }
#endif//_DEBUG

	int      index=0;
	TDotNode DotNode;
	SetFnName(_T("CAOIFileIO::ReadXYDotNodeCaliFile"));
	//----------------------------------------------------------------------------------------//
	List.clear();
	while ( CheckFileEnd()==false )
	{ 		
		if ( LoadChunk(index) == false )		
		{	continue; }

		switch ( index )
		{
		case FILE_IO_START:
			break;
		case FILE_IO_END:
			return true;
			break;
		case FILE_IO_DOT_NODE_SECTION_BEGIN:
			break;
		case FILE_IO_DOT_NODE_SECTION_END:			
			break;
		case FILE_IO_DOT_NODE_COUNT_X:
			CountX = GetData_INT();
			break;
		case FILE_IO_DOT_NODE_COUNT_Y:
			CountY = GetData_INT();
			break;
		case FILE_IO_DOT_NODE_BEGIN:
			DotNode = TDotNode();
			break;
		case FILE_IO_DOT_NODE_END:
			List.push_back(DotNode);
			break;
		case FILE_IO_DOT_NODE_INDEX_X:
			DotNode.nIndexX = GetData_INT();
			break;
		case FILE_IO_DOT_NODE_INDEX_Y:
			DotNode.nIndexY = GetData_INT();
			break;
		case FILE_IO_DOT_NODE_CAD_POS_X:
			DotNode.dPosX = GetData_DBL();
			break;
		case FILE_IO_DOT_NODE_CAD_POS_Y:
			DotNode.dPosY = GetData_DBL();
			break;
		case FILE_IO_DOT_NODE_STAGE_POS_X:
			DotNode.dCaliPosX = GetData_DBL();
			break;
		case FILE_IO_DOT_NODE_STAGE_POS_Y:
			DotNode.dCaliPosY = GetData_DBL();
			break;			
		case FILE_IO_DOT_NODE_OFFSET_X:
			DotNode.dOffsetX = GetData_DBL();
			break;
		case FILE_IO_DOT_NODE_OFFSET_Y:
			DotNode.dOffsetY = GetData_DBL();
			break;			

		case FILE_IO_DOT_NODE_RECT_LEFT:
			DotNode.rcDot.left = GetData_INT();
			break;
		case FILE_IO_DOT_NODE_RECT_TOP:
			DotNode.rcDot.top = GetData_INT();
			break;
		case FILE_IO_DOT_NODE_RECT_RIGHT:
			DotNode.rcDot.right = GetData_INT();
			break;
		case FILE_IO_DOT_NODE_RECT_BOTTOM:
			DotNode.rcDot.bottom = GetData_INT();
			break;			

		case FILE_IO_DOT_NODE_RESULT_ID:
			DotNode.eResultID = (RESULT_ID)(GetData_INT());
			break;
		case FILE_IO_DOT_NODE_SHAPE_MODE:
			DotNode.eShapeMode = (BOX_SHAPE_MODE)(GetData_INT());
			break;
		default:
#ifdef _DEBUG
			index = index;
#endif//_DEBUG
			break;
		}
	};	
	return true;	
	
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::WriteXYDotNodeCaliFile(int CountX, int CountY, const std::vector<TDotNode> &List)//寫入XY校正檔案
{
#ifdef _DEBUG
	if ( CheckFileMode() == false ) { return false; }
	if ( CheckFileOpened() == false ) { return false; }
#endif//_DEBUG

	size_t i=0;
	TDotNode     DotNode;
	const size_t DotNodeCount=List.size();
	SetFnName(_T("CAOIFileIO::WriteXYDotNodeCaliFile"));
	//----------------------------------------------------------------------------------------//
	if ( SaveChunk_INT(FILE_IO_DOT_NODE_SECTION_BEGIN, 0) == false ) { return false; }

	if ( SaveChunk_INT(FILE_IO_DOT_NODE_COUNT_X, CountX) == false ) { return false; }
	if ( SaveChunk_INT(FILE_IO_DOT_NODE_COUNT_Y, CountY) == false ) { return false; }
	for ( i=0; i<DotNodeCount; i++ )
	{
		DotNode = List[i];

		if ( SaveChunk_INT(FILE_IO_DOT_NODE_BEGIN, 0) == false ) { return false; }

		if ( SaveChunk_INT(FILE_IO_DOT_NODE_INDEX_X, DotNode.nIndexX) == false ) { return false; }
		if ( SaveChunk_INT(FILE_IO_DOT_NODE_INDEX_Y, DotNode.nIndexY) == false ) { return false; }

		if ( SaveChunk_DBL(FILE_IO_DOT_NODE_CAD_POS_X, DotNode.dPosX) == false ) { return false; }
		if ( SaveChunk_DBL(FILE_IO_DOT_NODE_CAD_POS_Y, DotNode.dPosY) == false ) { return false; }

		if ( SaveChunk_DBL(FILE_IO_DOT_NODE_STAGE_POS_X, DotNode.dCaliPosX) == false ) { return false; }
		if ( SaveChunk_DBL(FILE_IO_DOT_NODE_STAGE_POS_Y, DotNode.dCaliPosY) == false ) { return false; }

		if ( SaveChunk_DBL(FILE_IO_DOT_NODE_OFFSET_X, DotNode.dOffsetX) == false ) { return false; }
		if ( SaveChunk_DBL(FILE_IO_DOT_NODE_OFFSET_Y, DotNode.dOffsetY) == false ) { return false; }

		if ( SaveChunk_INT(FILE_IO_DOT_NODE_RECT_LEFT, DotNode.rcDot.left) == false ) { return false; }
		if ( SaveChunk_INT(FILE_IO_DOT_NODE_RECT_TOP, DotNode.rcDot.top) == false ) { return false; }
		if ( SaveChunk_INT(FILE_IO_DOT_NODE_RECT_RIGHT, DotNode.rcDot.right) == false ) { return false; }
		if ( SaveChunk_INT(FILE_IO_DOT_NODE_RECT_BOTTOM, DotNode.rcDot.bottom) == false ) { return false; }

		if ( SaveChunk_INT(FILE_IO_DOT_NODE_RESULT_ID, DotNode.eResultID) == false ) { return false; }
		if ( SaveChunk_INT(FILE_IO_DOT_NODE_SHAPE_MODE, DotNode.eShapeMode) == false ) { return false; }

		if ( SaveChunk_INT(FILE_IO_DOT_NODE_END, 0) == false ) { return false; }
	}
	if ( SaveChunk_INT(FILE_IO_DOT_NODE_SECTION_END, 0) == false ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::ReadPhaseFactorTableFile(TPhaseFactorTable &GridTable)//讀取相位高度比例參數檔案
{
#ifdef _DEBUG
	if ( CheckFileMode() == false ) { return false; }
	if ( CheckFileOpened() == false ) { return false; }
#endif//_DEBUG

	int      index=0;		
	double   dValue=0;
	double   TargetH=0;
	int      TargetNo=0;
	double   HeightBase=2700;//3000;
	bool     bReadTargetNo=false;
	TPhaseFactorGrid Grid;	
	std::vector<TPhaseFactorGrid> &List=GridTable.GridList;
	SetFnName(_T("CAOIFileIO::ReadPhaseFactorTableFile"));
	//----------------------------------------------------------------------------------------//
	List.clear();
	while ( CheckFileEnd()==false )
	{ 		
		if ( LoadChunk(index) == false )		
		{	continue; }

		switch ( index )
		{
		case FILE_IO_START:
			break;
		case FILE_IO_END:
			return true;
			break;
		case FILE_IO_PHASE_FACTOR_SECTION_BEGIN:
			break;
		case FILE_IO_PHASE_FACTOR_SECTION_END:			
			break;
		case FILE_IO_PHASE_FACTOR_COUNT_X:
			GridTable.nCols = GetData_INT();
			break;
		case FILE_IO_PHASE_FACTOR_COUNT_Y:
			GridTable.nRows = GetData_INT();
			break;
		case FILE_IO_PHASE_FACTOR_TARGET_HEIGHT:
			TargetH = GetData_DBL();
			GridTable.dHeight = TargetH;
			break;		
		case FILE_IO_PHASE_FACTOR_TARGET_NO:
			bReadTargetNo=true;
			TargetNo = GetData_INT();
			GridTable.nTargetNo = TargetNo;			
			break;
		case FILE_IO_PHASE_FACTOR_BEGIN:
			Grid = TPhaseFactorGrid();
			Grid.m_TargetNo = TargetNo;
			Grid.m_Height = TargetH;
			Grid.m_HeightOffset = TargetH;			
			break;
		case FILE_IO_PHASE_FACTOR_END:
			//HeightBase = 3000;
			//Grid.m_HeightBase = HeightBase;
			//Grid.m_HeightTarget = HeightBase+TargetH;			

			Grid.m_PhaseBaseCalc = Grid.m_PhaseBase;
			Grid.m_PhaseTargetCalc = Grid.m_PhaseTarget;
			Grid.m_HeightBaseCalc = Grid.m_HeightBase;
			Grid.m_HeightTargetCalc = Grid.m_HeightTarget;
			Grid.m_PhaseOffset = Grid.m_PhaseTarget-Grid.m_PhaseBase;			
			Grid.m_Factor = Grid.m_HeightOffset/Grid.m_PhaseOffset;
			List.push_back(Grid);
			break;
		case FILE_IO_PHASE_FACTOR_INDEX_X:
			Grid.m_IdxX = GetData_INT();
			break;
		case FILE_IO_PHASE_FACTOR_INDEX_Y:
			Grid.m_IdxY = GetData_INT();
			break;
		case FILE_IO_PHASE_FACTOR_TARGET_NO_TEMP:
			Grid.m_TargetNo = GetData_INT();			
			if ( false == bReadTargetNo )
			{
				bReadTargetNo = true;
				TargetNo = GetData_INT();
				GridTable.nTargetNo = TargetNo;					
			}
			break;
		case FILE_IO_PHASE_FACTOR_STAGE_POS_X:
			Grid.m_PosX = GetData_DBL();
			break;
		case FILE_IO_PHASE_FACTOR_STAGE_POS_Y:
			Grid.m_PosY = GetData_DBL();
			break;
		case FILE_IO_PHASE_FACTOR_STAGE_POS_Z:
			Grid.m_PosZ = GetData_DBL();
			break;
		case FILE_IO_PHASE_FACTOR_IMAGE_POS_X:
			Grid.m_ImgX = GetData_DBL();
			break;
		case FILE_IO_PHASE_FACTOR_IMAGE_POS_Y:
			Grid.m_ImgY = GetData_DBL();
			break;			
		case FILE_IO_PHASE_FACTOR_IMAGE_POS_Z:
			Grid.m_ImgZ = GetData_DBL();
			break;
		case FILE_IO_PHASE_FACTOR_PHASE_OFFSET:
			dValue = GetData_DBL();
			Grid.m_Phase = dValue;
			Grid.m_PhaseOffset = dValue;
			break;		
		case FILE_IO_PHASE_FACTOR_HEIGHT_OFFSET:
			dValue = GetData_DBL();
			Grid.m_Height = dValue;
			Grid.m_HeightOffset = dValue;
			break;

		case FILE_IO_PHASE_FACTOR_PHASE_BASE:
			Grid.m_PhaseBase = GetData_DBL();
			break;
		case FILE_IO_PHASE_FACTOR_HEIGHT_BASE:
			Grid.m_HeightBase = GetData_DBL();			
			break;
		case FILE_IO_PHASE_FACTOR_PHASE_TARGET:
			Grid.m_PhaseTarget = GetData_DBL();
			break;
		case FILE_IO_PHASE_FACTOR_HEIGHT_TARGET:
			Grid.m_HeightTarget = GetData_DBL();
			break;			

		case FILE_IO_PHASE_FACTOR_ROI_RECT_LEFT:
			Grid.m_RectLevel.left = GetData_INT();
			break;
		case FILE_IO_PHASE_FACTOR_ROI_RECT_TOP:
			Grid.m_RectLevel.top = GetData_INT();
			break;
		case FILE_IO_PHASE_FACTOR_ROI_RECT_RIGHT:
			Grid.m_RectLevel.right = GetData_INT();
			break;
		case FILE_IO_PHASE_FACTOR_ROI_RECT_BOTTOM:
			Grid.m_RectLevel.bottom = GetData_INT();
			break;

		case FILE_IO_PHASE_FACTOR_OBJ_RECT_LEFT:
			Grid.m_RectTarget.left = GetData_INT();
			break;
		case FILE_IO_PHASE_FACTOR_OBJ_RECT_TOP:
			Grid.m_RectTarget.top = GetData_INT();
			break;
		case FILE_IO_PHASE_FACTOR_OBJ_RECT_RIGHT:
			Grid.m_RectTarget.right = GetData_INT();
			break;
		case FILE_IO_PHASE_FACTOR_OBJ_RECT_BOTTOM:
			Grid.m_RectTarget.bottom = GetData_INT();
			break;
		
		default:
#ifdef _DEBUG
			index = index;
#endif//_DEBUG
			break;
		}
	};	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::WritePhaseFactorTableFile(const TPhaseFactorTable &GridTable)//寫入相位高度比例參數檔案
{
#ifdef _DEBUG
	if ( CheckFileMode() == false ) { return false; }
	if ( CheckFileOpened() == false ) { return false; }
#endif//_DEBUG

	size_t i=0;
	TPhaseFactorGrid Grid;
	const std::vector<TPhaseFactorGrid> &List=GridTable.GridList;
	const size_t GridCount=List.size();
	SetFnName(_T("CAOIFileIO::WritePhaseFactorTableFile"));
	//----------------------------------------------------------------------------------------//
	if ( SaveChunk_INT(FILE_IO_PHASE_FACTOR_SECTION_BEGIN, 0) == false ) { return false; }

	if ( SaveChunk_INT(FILE_IO_PHASE_FACTOR_COUNT_X, GridTable.nCols) == false ) { return false; }
	if ( SaveChunk_INT(FILE_IO_PHASE_FACTOR_COUNT_Y, GridTable.nRows) == false ) { return false; }
	if ( SaveChunk_DBL(FILE_IO_PHASE_FACTOR_TARGET_HEIGHT, GridTable.dHeight) == false ) { return false; }
	if ( SaveChunk_INT(FILE_IO_PHASE_FACTOR_TARGET_NO, GridTable.nTargetNo) == false ) { return false; }	

	for ( i=0; i<GridCount; i++ )
	{
		Grid = List[i];

		if ( SaveChunk_INT(FILE_IO_PHASE_FACTOR_BEGIN, 0) == false ) { return false; }

		if ( SaveChunk_INT(FILE_IO_PHASE_FACTOR_INDEX_X, Grid.m_IdxX) == false ) { return false; }
		if ( SaveChunk_INT(FILE_IO_PHASE_FACTOR_INDEX_Y, Grid.m_IdxY) == false ) { return false; }
		
		if ( SaveChunk_DBL(FILE_IO_PHASE_FACTOR_STAGE_POS_X, Grid.m_PosX) == false ) { return false; }
		if ( SaveChunk_DBL(FILE_IO_PHASE_FACTOR_STAGE_POS_Y, Grid.m_PosY) == false ) { return false; }
		if ( SaveChunk_DBL(FILE_IO_PHASE_FACTOR_STAGE_POS_Z, Grid.m_PosZ) == false ) { return false; }

		if ( SaveChunk_DBL(FILE_IO_PHASE_FACTOR_IMAGE_POS_X, Grid.m_ImgX) == false ) { return false; }
		if ( SaveChunk_DBL(FILE_IO_PHASE_FACTOR_IMAGE_POS_Y, Grid.m_ImgY) == false ) { return false; }
		if ( SaveChunk_DBL(FILE_IO_PHASE_FACTOR_IMAGE_POS_Z, Grid.m_ImgZ) == false ) { return false; }

		if ( SaveChunk_DBL(FILE_IO_PHASE_FACTOR_PHASE_OFFSET, Grid.m_PhaseOffset) == false ) { return false; }		
		if ( SaveChunk_DBL(FILE_IO_PHASE_FACTOR_HEIGHT_OFFSET, Grid.m_HeightOffset) == false ) { return false; }		

		if ( SaveChunk_DBL(FILE_IO_PHASE_FACTOR_PHASE_BASE, Grid.m_PhaseBase) == false ) { return false; }
		if ( SaveChunk_DBL(FILE_IO_PHASE_FACTOR_HEIGHT_BASE, Grid.m_HeightBase) == false ) { return false; }
		if ( SaveChunk_DBL(FILE_IO_PHASE_FACTOR_PHASE_TARGET, Grid.m_PhaseTarget) == false ) { return false; }
		if ( SaveChunk_DBL(FILE_IO_PHASE_FACTOR_HEIGHT_TARGET, Grid.m_HeightTarget) == false ) { return false; }

		if ( SaveChunk_INT(FILE_IO_PHASE_FACTOR_ROI_RECT_LEFT, Grid.m_RectLevel.left) == false ) { return false; }
		if ( SaveChunk_INT(FILE_IO_PHASE_FACTOR_ROI_RECT_TOP, Grid.m_RectLevel.top) == false ) { return false; }
		if ( SaveChunk_INT(FILE_IO_PHASE_FACTOR_ROI_RECT_RIGHT, Grid.m_RectLevel.right) == false ) { return false; }
		if ( SaveChunk_INT(FILE_IO_PHASE_FACTOR_ROI_RECT_BOTTOM, Grid.m_RectLevel.bottom) == false ) { return false; }
		
		if ( SaveChunk_INT(FILE_IO_PHASE_FACTOR_OBJ_RECT_LEFT, Grid.m_RectTarget.left) == false ) { return false; }
		if ( SaveChunk_INT(FILE_IO_PHASE_FACTOR_OBJ_RECT_TOP, Grid.m_RectTarget.top) == false ) { return false; }
		if ( SaveChunk_INT(FILE_IO_PHASE_FACTOR_OBJ_RECT_RIGHT, Grid.m_RectTarget.right) == false ) { return false; }
		if ( SaveChunk_INT(FILE_IO_PHASE_FACTOR_OBJ_RECT_BOTTOM, Grid.m_RectTarget.bottom) == false ) { return false; }

		if ( SaveChunk_INT(FILE_IO_PHASE_FACTOR_END, 0) == false ) { return false; }
	}
	if ( SaveChunk_INT(FILE_IO_PHASE_FACTOR_SECTION_END, 0) == false ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::ReadGroundEquationFile(CJetGroundEquation &GroundEquation)//讀取底面方程式參數檔案
{
#ifdef _DEBUG
	if ( CheckFileMode() == false ) { return false; }
	if ( CheckFileOpened() == false ) { return false; }
#endif//_DEBUG

	int      index=0;		
	double   dValue=0;
	double   *T=GroundEquation.GetGroundParamList();
	SetFnName(_T("CAOIFileIO::ReadGroundEquationFile"));
	//----------------------------------------------------------------------------------------//	
	while ( CheckFileEnd()==false )
	{ 		
		if ( LoadChunk(index) == false )		
		{	continue; }

		switch ( index )
		{
		case FILE_IO_GROUND_EQUATION_START://基準面方程式參數-起點
			GroundEquation.ClearGroundParam();
			break;
		case FILE_IO_GROUND_EQUATION_END://基準面方程式參數-終點
			return true;
			break;
		case FILE_IO_GROUND_EQUATION_MODE://基準面方程式參數-模式
			GroundEquation.SetGroundEquationMode((GROUND_EQUATION_MODE)(GetData_INT()));
			break;
		case FILE_IO_GROUND_EQUATION_PARAM://基準面方程式參數-參數列表
			if ( ReadDoubleArrayFile(GROUND_EQUATION_PARAM_COUNT, T) == false )
			{	return false;	}
			break;
		default:
			break;
		}
	};	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::WriteGroundEquationFile(const CJetGroundEquation &GroundEquation)//讀取底面方程式參數檔案
{
#ifdef _DEBUG
	if ( CheckFileMode() == false ) { return false; }
	if ( CheckFileOpened() == false ) { return false; }
#endif//_DEBUG

	SetFnName(_T("CAOIFileIO::WriteGroundEquationFile"));	
	const double *T = GroundEquation.GetGroundParamList();
	const int Count = GroundEquation.GetGroundParamCount();
	//----------------------------------------------------------------------------------------//
	if ( SaveChunk_INT(FILE_IO_GROUND_EQUATION_START, 0) == false ) { return false; }

	if ( SaveChunk_INT(FILE_IO_GROUND_EQUATION_MODE, GroundEquation.GetGroundEquationMode()) == false ) { return false; }
	if ( SaveChunk_INT(FILE_IO_GROUND_EQUATION_PARAM, 0) == false ) { return false; }
	if ( WriteDoubleArrayFile(Count, T) == false )
	{	return false; }
	if ( SaveChunk_INT(FILE_IO_GROUND_EQUATION_END, 0) == false ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::ReadGrrSigmaItemFile(TGrrSigmaItem &GrrSigmaItem)//讀取Grr標準差項目檔案
{
#ifdef _DEBUG
	if ( CheckFileMode() == false ) { return false; }
	if ( CheckFileOpened() == false ) { return false; }
#endif//_DEBUG
	int      index=0;		
	SetFnName(_T("CAOIFileIO::ReadSigmaItemFile"));
	//----------------------------------------------------------------------------------------//	
	while ( CheckFileEnd()==false )
	{ 		
		if ( LoadChunk(index) == false )		
		{	continue; }

		switch ( index )
		{
		case FILE_IO_GRR_SIGMA_ITEM_START://Grr標準差項目-起點
			//GrrSigmaItem = tagSigmaItem();
			GrrSigmaItem.Count = 0;
			GrrSigmaItem.Barcode = L"";			
			break;
		case FILE_IO_GRR_SIGMA_ITEM_END://標準差項目-終點
			return true;
			break;
		case FILE_IO_GRR_SIGMA_ITEM_COUNT://Grr標準差項目-數量
			GrrSigmaItem.Count = GetData_INT();			
			break;
		case FILE_IO_GRR_SIGMA_ITEM_BARCODE://Grr標準差項目-條碼
			if ( GetLoadWStr() == false )
			{	GrrSigmaItem.Barcode = GetData_STR();	}
			else
			{	GrrSigmaItem.Barcode = GetData_WSTR();	}			
			break;
		case FILE_IO_GRR_SIGMA_ITEM_SIGMA_ITEM_NODE://Grr標準差項目-標準差項目
			if ( ReadSigmaItemFile(GrrSigmaItem.SigmaItem) == false )
			{	return false; }
			break;		
		default:
			break;
		}
	};	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::WriteGrrSigmaItemFile(const TGrrSigmaItem &GrrSigmaItem)//讀取Grr標準差項目檔案
{
#ifdef _DEBUG
	if ( CheckFileMode() == false ) { return false; }
	if ( CheckFileOpened() == false ) { return false; }
#endif//_DEBUG
	if ( SaveChunk_INT(FILE_IO_GRR_SIGMA_ITEM_START, 0) == false ) { return false; }	
	if ( SaveChunk_INT(FILE_IO_GRR_SIGMA_ITEM_COUNT, GrrSigmaItem.Count) == false ) { return false; }				
	if ( SaveChunk_STR(FILE_IO_GRR_SIGMA_ITEM_BARCODE, GrrSigmaItem.Barcode) == false ) { return false; }
	if ( SaveChunk_INT(FILE_IO_GRR_SIGMA_ITEM_SIGMA_ITEM_NODE, 0) == false ) { return false; }		
	if ( WriteSigmaItemFile(GrrSigmaItem.SigmaItem) == false ) { return false; }
	
	if ( SaveChunk_INT(FILE_IO_GRR_SIGMA_ITEM_END, 0) == false ) { return false; }	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::ReadSigmaItemFile(TSigmaItem &SigmaItem)//讀取標準差項目檔案
{
#ifdef _DEBUG
	if ( CheckFileMode() == false ) { return false; }
	if ( CheckFileOpened() == false ) { return false; }
#endif//_DEBUG

	int      index=0;		
	SetFnName(_T("CAOIFileIO::ReadSigmaItemFile"));
	//----------------------------------------------------------------------------------------//	
	while ( CheckFileEnd()==false )
	{ 		
		if ( LoadChunk(index) == false )		
		{	continue; }

		switch ( index )
		{
		case FILE_IO_SIGMA_ITEM_START://標準差項目-起點
			//SigmaItem = tagSigmaItem();
			break;
		case FILE_IO_SIGMA_ITEM_END://標準差項目-終點
			return true;
			break;
		case FILE_IO_SIGMA_ITEM_NODE_OFFSET_X://標準差項目-節點-偏移X
			if ( ReadSigmaTempFile(SigmaItem.OffsetX) == false )
			{	return false; }
			break;
		case FILE_IO_SIGMA_ITEM_NODE_OFFSET_Y://標準差項目-節點-偏移Y
			if ( ReadSigmaTempFile(SigmaItem.OffsetY) == false )
			{	return false; }
			break;
		case FILE_IO_SIGMA_ITEM_NODE_SKEW_ANGLE://標準差項目-節點-偏移角度
			if ( ReadSigmaTempFile(SigmaItem.SkewAngle) == false )
			{	return false; }
			break;
		case FILE_IO_SIGMA_ITEM_NODE_BODY_HEIGHT://標準差項目-節點-本體高度
			if ( ReadSigmaTempFile(SigmaItem.BodyHeight) == false )
			{	return false; }
			break;
		default:
			break;
		}
	};	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::WriteSigmaItemFile(const TSigmaItem &SigmaItem)//讀取標準差項目檔案
{
#ifdef _DEBUG
	if ( CheckFileMode() == false ) { return false; }
	if ( CheckFileOpened() == false ) { return false; }
#endif//_DEBUG

	SetFnName(_T("CAOIFileIO::WriteSigmaItemFile"));		
	if ( SaveChunk_INT(FILE_IO_SIGMA_ITEM_START, 0) == false ) { return false; } 
	if ( SaveChunk_INT(FILE_IO_SIGMA_ITEM_NODE_OFFSET_X, 0) == false ) { return false; } 
	if ( WriteSigmaTempFile(SigmaItem.OffsetX) == false ) { return false; }
	if ( SaveChunk_INT(FILE_IO_SIGMA_ITEM_NODE_OFFSET_Y, 0) == false ) { return false; } 
	if ( WriteSigmaTempFile(SigmaItem.OffsetY) == false ) { return false; }
	if ( SaveChunk_INT(FILE_IO_SIGMA_ITEM_NODE_SKEW_ANGLE, 0) == false ) { return false; } 
	if ( WriteSigmaTempFile(SigmaItem.SkewAngle) == false ) { return false; }
	if ( SaveChunk_INT(FILE_IO_SIGMA_ITEM_NODE_BODY_HEIGHT, 0) == false ) { return false; } 
	if ( WriteSigmaTempFile(SigmaItem.BodyHeight) == false ) { return false; }
	if ( SaveChunk_INT(FILE_IO_SIGMA_ITEM_END, 0) == false ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::ReadSigmaTempFile(TSigmaTemp &SigmaTemp)//讀取標準差參數檔案
{
#ifdef _DEBUG
	if ( CheckFileMode() == false ) { return false; }
	if ( CheckFileOpened() == false ) { return false; }
#endif//_DEBUG

	int      index=0;		
	SetFnName(_T("CAOIFileIO::ReadSigmaTempFile"));
	//----------------------------------------------------------------------------------------//	
	while ( CheckFileEnd()==false )
	{ 		
		if ( LoadChunk(index) == false )		
		{	continue; }

		switch ( index )
		{
		case FILE_IO_SIGMA_TEMP_START://標準差參數-起點
			SigmaTemp = tagSigmaTemp();
			break;
		case FILE_IO_SIGMA_TEMP_END://標準差參數-終點
			return true;
			break;		
		case FILE_IO_SIGMA_TEMP_MAX://標準差參數-最大值
			SigmaTemp.Max = GetData_DBL();
			break;
		case FILE_IO_SIGMA_TEMP_MIN://標準差參數-最小值
			SigmaTemp.Min = GetData_DBL();
			break;
		case FILE_IO_SIGMA_TEMP_SUM://標準差參數-總和
			SigmaTemp.Sum = GetData_DBL();
			break;
		case FILE_IO_SIGMA_TEMP_SUM_SQRD://標準差參數-平方總和
			SigmaTemp.SumSqrd = GetData_DBL();
			break;
		case FILE_IO_SIGMA_TEMP_COUNT://標準差參數-數量
			SigmaTemp.Count = GetData_INT();
			break;
		default:
			break;
		}
	};	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFileIO::WriteSigmaTempFile(const TSigmaTemp &SigmaTemp)//讀取標準差參數檔案
{
#ifdef _DEBUG
	if ( CheckFileMode() == false ) { return false; }
	if ( CheckFileOpened() == false ) { return false; }
#endif//_DEBUG

	SetFnName(_T("CAOIFileIO::WriteSigmaTempFile"));	
	if ( SaveChunk_INT(FILE_IO_SIGMA_TEMP_START, 0) == false ) { return false; }		
	if ( SaveChunk_DBL(FILE_IO_SIGMA_TEMP_MAX, SigmaTemp.Max) == false ) { return false; }
	if ( SaveChunk_DBL(FILE_IO_SIGMA_TEMP_MIN, SigmaTemp.Min) == false ) { return false; }
	if ( SaveChunk_DBL(FILE_IO_SIGMA_TEMP_SUM, SigmaTemp.Sum) == false ) { return false; }
	if ( SaveChunk_DBL(FILE_IO_SIGMA_TEMP_SUM_SQRD, SigmaTemp.SumSqrd) == false ) { return false; }
	if ( SaveChunk_INT(FILE_IO_SIGMA_TEMP_COUNT, SigmaTemp.Count) == false ) { return false; }
	if ( SaveChunk_INT(FILE_IO_SIGMA_TEMP_END, 0) == false ) { return false; }	
	return true;
}
//-------------------------------------------------------------------------------------//