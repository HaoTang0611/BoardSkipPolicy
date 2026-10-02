#ifndef _JetAPIUtility_H_
#define _JetAPIUtility_H_
//-------------------------------------------------------------------------------------//
#include <math.h>
#include <vector>
#include <algorithm>
#include "JETListCtrl.h"
#include "JETMFCListCtrl.h"
//-------------------------------------------------------------------------------------//
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#define CHECK_CURSOR_MODE_FILL                1//確認鼠標模式-內部為主
#define CHECK_CURSOR_MODE_FRAME               2//確認鼠標模式-框線為主
//-------------------------------------------------------------------------------------//
//縮放區域模式(L:MinX, R:MaxX, T:MinY, B:MaxY)
#define SCALE_REGION_BY_CENTER                 1
#define SCALE_REGION_BY_SIDE_MIN_X            11
#define SCALE_REGION_BY_SIDE_MIN_Y            12
#define SCALE_REGION_BY_SIDE_MAX_X            13
#define SCALE_REGION_BY_SIDE_MAX_Y            14
#define SCALE_REGION_BY_CORNER_LT             21
#define SCALE_REGION_BY_CORNER_RT             22
#define SCALE_REGION_BY_CORNER_LB             23
#define SCALE_REGION_BY_CORNER_RB             24
//-------------------------------------------------------------------------------------//
enum FORMAT_TIME
{
	FORMAT_TIME_01,//YYYY/MM/DD hh:mm:ss	
	FORMAT_TIME_RAW,//YYYYMMDDhhmmss
	FORMAT_TIME_HH_MM,//hh:mm
	FORMAT_TIME_RETURN
};
//-------------------------------------------------------------------------------------//
namespace JetAPI
{
	//---------------------------------------------------------------------------------//	
	bool OpenFolderDialog(CWnd *pWnd, CString &Folder);
	bool OpenFolderDialogFn(CWnd *pWnd, CString &Folder);
	int  CALLBACK BrowseCallbackProc(HWND hwnd,UINT uMsg,LPARAM lp, LPARAM pData);	
	bool OpenFileDialog(CString &Filename, BOOL bOpen, LPCTSTR lpszDefExt=NULL,	LPCTSTR lpszFileName=NULL, DWORD dwFlags=OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, LPCTSTR lpszFilter=NULL, CWnd* pParentWnd=NULL, DWORD dwSize=0, BOOL bVistaStyle=TRUE);
	//---------------------------------------------------------------------------------//
	//System Event
	HANDLE CreateEvent(LPSECURITY_ATTRIBUTES lpEventAttributes,BOOL bManualReset, BOOL bInitialState, LPCTSTR lpName);
	//---------------------------------------------------------------------------------//
	//About INI File
	bool                       IsIniSection(LPCSTR pText);
	bool                       IsIniSection(LPCWSTR pText);
	bool                       SaveINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pString, LPCTSTR pfilename, CString &Error);
	bool                       LoadINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pDefault, TCHAR *pString, int StringSize, LPCTSTR pfilename, bool IsCheckLens, CString &Error);	
	//---------------------------------------------------------------------------------//
	bool                       IsEventInt(int val);//是否偶數
	bool                       IsEventSize(size_t val);//是否偶數
	bool                       IsEventUInt(unsigned int val);//是否偶數	
	//---------------------------------------------------------------------------------//
	void                       Swap(int &v1, int &v2);//參數對調
	void                       Swap(bool &v1, bool &v2);//參數對調
	void                       Swap(char &v1, char &v2);//參數對調
	void                       Swap(float &v1, float &v2);//參數對調
	void                       Swap(double &v1, double &v2);//參數對調
	void                       Swap(wchar_t &v1, wchar_t &v2);//參數對調	
	void                       Swap(unsigned int &v1, unsigned int &v2);//參數對調
	void                       Swap(CString &v1, CString &v2);//參數對調
	void                       Swap(std::string &v1, std::string &v2);//參數對調
	void                       Swap(std::wstring &v1, std::wstring &v2);//參數對調
	void                       Swap(unsigned char *&v1, unsigned char *&v2);//參數對調
	//---------------------------------------------------------------------------------//	
	bool                       CmpPoint_X_ASC(const POINT &v1, const POINT &v2);//比較函式-點X-遞增
	bool                       CmpPoint_X_DESC(const POINT &v1, const POINT &v2);//比較函式-點X-遞減
	bool                       CmpPoint_Y_ASC(const POINT &v1, const POINT &v2);//比較函式-點Y-遞增
	bool                       CmpPoint_Y_DESC(const POINT &v1, const POINT &v2);//比較函式-點Y-遞減
	//---------------------------------------------------------------------------------//
	bool                       CmpPoint2D_X(const TPOINT2D &v1, const TPOINT2D &v2);//比較函式-點X
	bool                       CmpPoint2D_Y(const TPOINT2D &v1, const TPOINT2D &v2);//比較函式-點Y
	//---------------------------------------------------------------------------------//
	bool                       CmpPoint3D_X(const TPOINT3D &v1, const TPOINT3D &v2);//比較函式-點X
	bool                       CmpPoint3D_Y(const TPOINT3D &v1, const TPOINT3D &v2);//比較函式-點Y
	bool                       CmpPoint3D_Z(const TPOINT3D &v1, const TPOINT3D &v2);//比較函式-點Y
	//---------------------------------------------------------------------------------//
	bool                       CmpRect_L(const RECT &v1, const RECT &v2);//比較函式-區域左
	bool                       CmpRect_T(const RECT &v1, const RECT &v2);//比較函式-區域上
	bool                       CmpRect_R(const RECT &v1, const RECT &v2);//比較函式-區域右
	bool                       CmpRect_B(const RECT &v1, const RECT &v2);//比較函式-區域下
	//---------------------------------------------------------------------------------//
	bool                       CmpRegion_MinX(const TREGION4D &v1, const TREGION4D &v2);//比較函式-區域小X
	bool                       CmpRegion_MinY(const TREGION4D &v1, const TREGION4D &v2);//比較函式-區域小Y
	bool                       CmpRegion_MaxX(const TREGION4D &v1, const TREGION4D &v2);//比較函式-區域大X
	bool                       CmpRegion_MaxY(const TREGION4D &v1, const TREGION4D &v2);//比較函式-區域大Y
	//---------------------------------------------------------------------------------//
	//About Critical Section
	bool                       CheckLocked_INT(int &flag, int lockID, CRITICAL_SECTION &cs);//確認是否鎖住
	bool                       CheckLocked_BOOL(bool &locked, CRITICAL_SECTION &cs);//確認是否鎖住
	//---------------------------------------------------------------------------------//	
	//About Unit
	double                     Unit_MMtoUM(double mm);//Unit mm to um
	double                     Unit_UmtoMM(double um);//Unit um to um	
	//---------------------------------------------------------------------------------//	
	int                        ToInt(double val);//浮點數轉整數
	int                        Round(double val);//四捨五入
	int                        Ceil(double val);//無條件進入(2.1=>3)
	int                        Floor(double val);//無條件捨去(2.9=>2)
	void                       AbsPoint(TPOINT2F &pt);//絕對值點座標
	void                       AbsPoint(TPOINT2D &pt);//絕對值點座標
	void                       AbsPoint(TPOINT3I &pt);//絕對值點座標
	void                       AbsPoint(TPOINT3F &pt);//絕對值點座標
	void                       AbsPoint(TPOINT3D &pt);//絕對值點座標
	double                     CalcCos(double R, double L, double H);//餘弦定理
	double                     CalcAngle2D(const TPOINT2D &Pt1, const TPOINT2D &Pt2);//計算2點角度
	double                     CalcAngle3D(const TPOINT3D &Pt1, const TPOINT3D &Pt2);//計算2點角度
	double                     CalcDistance(double X, double Y);//計算距離
	double                     CalcDistance(const TPOINT2D &P1, const TPOINT2D &P2);//計算距離
	double                     AdjustValue(double val, double divide);//調整數據為某個倍數
	//---------------------------------------------------------------------------------//
	COLORREF                   DivideColor(COLORREF clr, int nDivide);//顏色除半
	//---------------------------------------------------------------------------------//
	bool                       GetTime(CString &Time, const CTime &NowTime);//取得時間
	bool                       GetTime(const char strDateTime[], CTime &NowTime);//取得時間
	bool                       GetDateTime(LPCTSTR DateTime, CString &Date, CString &Time);//取得日期時間
	bool                       GetTimeSpan(int nTotalMin, int &nDays, int &nHours, int &nMins);//取得時間跨距
	bool                       GetTimeSpan(int nTotalSec, int &nDays, int &nHours, int &nMins, int &nSecs);//取得時間跨距	
	bool                       FormatTime(FORMAT_TIME Mode, const CTime &NowTime, CString &Time);//格式化時間
	bool                       GetTime(CString &year, CString &month, CString &day, CString &hour, CString &min, CString &sec,const CTime &NowTime);//取得時間	
	bool                       FileTimeToTimet(const FILETIME &ft, time_t &time);//時間結構轉換
	bool                       TimetToFileTime(const time_t &time, FILETIME &ft);//時間結構轉換
	bool                       TmToSystemTime(const tm &time, SYSTEMTIME &st);//時間結構轉換
	bool                       SystemTimeToTm(const SYSTEMTIME &st, tm &time);//時間結構轉換
	//---------------------------------------------------------------------------------//			
	bool                       SetFuncTimeEnd(LARGE_INTEGER &End);//設定函式結束時間
	bool                       SetFuncTimeStart(LARGE_INTEGER &Start);//設定函式起始時間	
	double                     CalcFuncTimeSpent(LARGE_INTEGER &Start, LARGE_INTEGER &End);//計算函式經過時間
	//---------------------------------------------------------------------------------//	
	bool                       EnumRS232(std::vector<int> &List);//列舉RS232
	int						   FindUsbComPort(CString keyword, int &port);
	//---------------------------------------------------------------------------------//
	bool                       GetStopCopyFunc();//取得是否停止複製
	void                       SetStopCopyFunc(bool bStop);//設定是否停止複製
	//---------------------------------------------------------------------------------//
	bool                       IsFileExist(LPCTSTR FileName);//檔案是否存在	
	bool                       IsFolderExist(LPCTSTR Folder);//資料夾是否存在	
	bool                       IsDiskDrive(LPCTSTR Folder);//是否磁碟機
	bool                       RemoveFolder(LPCTSTR Folder);//移除資料夾
	bool                       CreateFolder(LPCTSTR Folder);//強迫建立資料夾
	bool                       ClearFolder(LPCTSTR Folder);//清除資料夾內容		
	bool                       CreateSyncFile(LPCTSTR Filename);//建立同步檔案
	bool                       GetSyncFilename(LPCTSTR Filename, CString &Sync);//取得同步檔名
	bool                       CreateTempFile(LPCTSTR Filename, LPCTSTR Content);//建立暫存檔案	
	bool                       ClearFolder_rmdir(LPCTSTR Folder);//清除資料夾內容	
	bool                       GetFileAccessTime(LPCTSTR FileName, time_t &time);//取得檔案存取日期時間
	bool                       GetFileCreatedTime(LPCTSTR FileName, time_t &time);//取得檔案建立日期時間
	bool                       GetFileModifedTime(LPCTSTR FileName, time_t &time);//取得檔案修改日期時間	
	bool                       SystemCmdLine(LPCTSTR Func, LPCTSTR Param, int nShow, DWORD dwTimeout=INFINITE);//執行系統函式
	bool                       ClearFolderFiles(LPCTSTR Path, LPCTSTR FilterExt);//清除資料夾中特定副檔名的檔案
	bool                       DeleteFileList(const std::vector<CString> &FileList);//清除檔案列表
	bool                       DeleteFileList(LPCTSTR Folder, const std::vector<CString> &FileList);//清除檔案列表
	bool                       KeepLatestFolder(LPCTSTR Folder, size_t KeepCount);//保留最新幾個資料夾	
	bool                       ListFolder(LPCTSTR Folder, std::vector<CString> &FolderList);//列表資料夾		
	bool                       CopyFiles(LPCTSTR SrcPath, LPCTSTR DestPath, LPCTSTR ExtName);//複製特定副檔名	
	bool                       ListFilesInFolder(LPCTSTR Folder, LPCTSTR ExtName, std::vector<CString> &FileList);//尋找資料夾內的檔案
	bool                       ListFilesInFolder(LPCTSTR Folder, LPCTSTR ExtName, std::vector<WIN32_FIND_DATA> &FileList);//尋找資料夾內的檔案
	bool                       ListFilesInFolder(LPCTSTR Folder, LPCTSTR MainName, LPCTSTR ExtName, std::vector<CString> &FileList);//尋找資料夾內的檔案
	bool                       ListFilesInFolder(LPCTSTR Folder, LPCTSTR MainName, LPCTSTR ExtName, std::vector<WIN32_FIND_DATA> &FileList);//尋找資料夾內的檔案
	bool                       ListFilesByFileName(LPCTSTR Folder, const std::vector<CString> &NameList, std::vector<WIN32_FIND_DATA> &FileList);//將檔案名稱列表轉成檔案列表
	bool                       CopyFolderAToFolderB(LPCTSTR SrcPath, LPCTSTR DesPath, bool IsDeleteSrc, bool IsClearDest, LPCTSTR ExtName, int CurrentLevel, int MaxLevel, bool bChkStop=false);//拷貝整個資料夾
	bool                       XCopyFolderAToFolderB(LPCTSTR SrcPath, LPCTSTR DesPath, bool IsDeleteSrc, bool IsClearDest, LPCTSTR ExtName, int CurrentLevel, int MaxLevel);//拷貝整個資料夾	
	bool                       RoboCopyFolderAToFolderB(LPCTSTR SrcPath, LPCTSTR DesPath, bool IsDeleteSrc, bool IsClearDest, LPCTSTR ExtName, int CurrentLevel, int MaxLevel);//拷貝整個資料夾	
	bool                       HugeCopyFolderAToFolderB(COPY_HUGE_FILES_MODE Mode, LPCTSTR SrcPath, LPCTSTR DesPath, bool IsDeleteSrc, bool IsClearDest, LPCTSTR ExtName, int CurrentLevel, int MaxLevel);//拷貝整個資料夾	
	//---------------------------------------------------------------------------------//	
	//About Image
	int                        GetImageChannels(IMAGE_SIZE BitCount);
	//---------------------------------------------------------------------------------//	
	int                        GetBMPImagePixelsPerLine(int ImageW, int Align);//取得影像每條所需的位元組
	int                        GetBMPImagePixelsPerLine(int ImageW, int BitCount, int Align);//取得影像每條所需的位元組

	unsigned int               GetBMPImagePixelsPerLine(unsigned int ImageW, int Align);//取得影像每條所需的位元組
	unsigned int               GetBMPImagePixelsPerLine(unsigned int ImageW, unsigned int BitCount, int Align);//取得影像每條所需的位元組
	//---------------------------------------------------------------------------------//
	//About CWnd Font
	bool                       UpdateSubWndFont(HWND hWnd, HGDIOBJ hFont);
	bool                       CreateWndFont(CFont &rFont, int nAdd, LPCTSTR Face=_T(""));		
	//---------------------------------------------------------------------------------//
	//About CWnd
	void                       EnableCloseButton(HWND hWnd, BOOL bEnable);//啟用視窗右上方關閉按鈕(系統選單)
	void                       EnableMinimizeButton(HWND hWnd, BOOL bEnable);//啟用視窗右上方最小按鈕(系統選單)
	void                       EnableMaximizeButton(HWND hWnd, BOOL bEnable);//啟用視窗右上方最大按鈕(系統選單)
	//---------------------------------------------------------------------------------//
	//About Combox 
	void                       ClearCombox(CComboBox &Combox);
	CString                    GetComboxCurSelText(const CComboBox &Combox);
	DWORD_PTR                  GetComboxCurSelData(const CComboBox &Combox);
	void                       SetComboxCurSel(CComboBox &Combox, const DWORD AppendID);
	void                       SetComboxCurSelPtr(CComboBox &Combox, const DWORD_PTR AppendID);		
	//-----------------------------------------------------------------------------//	
	bool                       CallExecApp(HWND hWnd, LPCTSTR lpOperation, LPCTSTR lpFile, LPCTSTR lpParameters, LPCTSTR lpDirectory, int nShowCmd, bool bWait=false);//呼叫外部執行檔
	//-----------------------------------------------------------------------------//	
	//Aboud Bom File
	bool                       LoadBomFile(LPCTSTR pfilename, const TLoadParam_BOM &LoadParam, std::vector<TComponentNode> &NodeList, CString &ErrorString);//讀入BOM
	bool                       PreLoadBomFile(LPCTSTR pfilename, const TLoadParam_BOM &LoadParam, size_t &Columns, size_t &Rows, CString &ErrorString);//預先讀入BOM	
	//-----------------------------------------------------------------------------//		
	//About CADXY File
	bool                       PreLoadCADXYFile(LPCTSTR pfilename, size_t StartLine, const std::vector<char> &Delimiters, size_t &Columns, size_t &Rows, CString &ErrorString);//預先讀入CADXY	
	bool                       PreLoadCADXYFile(LPCTSTR pfilename, size_t StartLine, const std::vector<wchar_t> &Delimiters, size_t &Columns, size_t &Rows, CString &ErrorString);//預先讀入CADXY	
	//-----------------------------------------------------------------------------//	
	bool                       ExtractComponentBaseName(LPCTSTR str, CString &Name);//取零件基本名稱
	bool                       ExtractComponentBaseName(LPCTSTR str, LPTSTR pName, size_t buffersize);//取零件基本名稱
	bool                       ExtractComponentBaseNameA(LPCSTR str, LPSTR pName, size_t buffersize);//取零件基本名稱
	bool                       ExtractComponentBaseNameW(LPCWSTR str, LPWSTR pName, size_t buffersize);//取零件基本名稱
	//---------------------------------------------------------------------------------//	
	bool                       ExtractComponentBaseNameIndex(LPCTSTR str, CString &Name, size_t &Idx);//取零件基本名稱與引數
	bool                       ExtractComponentBaseNameIndex(LPCTSTR str, LPTSTR pName, size_t &Idx, size_t buffersize);//取零件基本名稱與引數
	bool                       ExtractComponentBaseNameIndexA(LPCSTR str, LPSTR pName, size_t &Idx, size_t buffersize);//取零件基本名稱與引數
	bool                       ExtractComponentBaseNameIndexW(LPCWSTR str, LPWSTR pName, size_t &Idx, size_t buffersize);//取零件基本名稱與引數
	//---------------------------------------------------------------------------------//	
	bool                       ExtractFolder(LPCTSTR pfilename, CString &Folder);
	bool                       ExtractFolder(LPCTSTR pfilename, LPTSTR pFolder, size_t buffersize);
	bool                       ExtractFolderA(LPCSTR pfilename, LPSTR pFolder, size_t buffersize);
	bool                       ExtractFolderW(LPCWSTR pfilename, LPWSTR pFolder, size_t buffersize);

	bool                       ExtractExtendFileName(LPCTSTR pfilename, CString &ExtName);//取出副檔名
	bool                       ExtractExtendFileName(LPCTSTR pfilename, LPTSTR pExtFileName, size_t buffersize);//取出副檔名
	bool                       ExtractExtendFileNameA(LPCSTR pfilename, LPSTR pExtFileName, size_t buffersize);//取出副檔名
	bool                       ExtractExtendFileNameW(LPCWSTR pfilename, LPWSTR pExtFileName, size_t buffersize);//取出副檔名

	bool                       ExtractMainFileName(LPCTSTR pfilename, CString &MainFileName);//取出主檔名	
	bool                       ExtractMainFileName(LPCTSTR pfilename, LPTSTR pMainFileName, size_t buffersize);//取出主檔名	
	bool                       ExtractMainFileNameA(LPCSTR pfilename, LPSTR pMainFileName, size_t buffersize);//取出主檔名	
	bool                       ExtractMainFileNameW(LPCWSTR pfilename, LPWSTR pMainFileName, size_t buffersize);//取出主檔名	

	bool                       ExtractMainFileNameNoPath(LPCTSTR pfilename, CString &MainFileName);//取出主檔名, 但不含路徑
	bool                       ExtractMainFileNameNoPath(LPCTSTR pfilename, LPTSTR pMainFileName, size_t buffersize);//取出主檔名, 但不含路徑
	bool                       ExtractMainFileNameNoPathA(LPCSTR pfilename, LPSTR pMainFileName, size_t buffersize);//取出主檔名, 但不含路徑
	bool                       ExtractMainFileNameNoPathW(LPCWSTR pfilename, LPWSTR pMainFileName, size_t buffersize);//取出主檔名, 但不含路徑

	bool                       ExtractFileNameNoPath(LPCTSTR pfilename, CString &MainFileName);//取出檔名, 但不含路徑
	bool                       ExtractFileNameNoPath(LPCTSTR pfilename, LPTSTR pMainFileName, size_t buffersize);//取出檔名, 但不含路徑
	bool                       ExtractFileNameNoPathA(LPCSTR pfilename, LPSTR pMainFileName, size_t buffersize);//取出檔名, 但不含路徑
	bool                       ExtractFileNameNoPathW(LPCWSTR pfilename, LPWSTR pMainFileName, size_t buffersize);//取出檔名, 但不含路徑

	bool                       ExtractLastPath(LPCTSTR pfilename, CString &LastPath);//取出上一層路徑 
	bool                       ExtractLastPath(LPCTSTR pfilename, LPTSTR pLastPath, size_t buffersize);//取出上一層路徑
	bool                       ExtractLastPathA(LPCSTR pfilename, LPSTR pLastPath, size_t buffersize);//取出上一層路徑
	bool                       ExtractLastPathW(LPCWSTR pfilename, LPWSTR pLastPath, size_t buffersize);//取出上一層路徑
	
	bool                       ExtractTopFolder(LPCTSTR pPath, CString &TopFolder);//取出上層資料夾 
	bool                       ExtractTopFolder(LPCTSTR pPath, LPTSTR pTopFolder, size_t buffersize);//取出上層資料夾 
	bool                       ExtractTopFolderA(LPCSTR pPath, LPSTR pTopFolder, size_t buffersize);//取出上層資料夾 
	bool                       ExtractTopFolderW(LPCWSTR pPath, LPWSTR pTopFolder, size_t buffersize);//取出上層資料夾  

	bool                       ExtractShortFilename(LPCTSTR pFolder, LPCTSTR pfilename, CString &ShortName);//取出不含底部資料夾的短檔名 
	bool                       ExtractShortFilename(LPCTSTR pFolder, LPCTSTR pfilename, LPTSTR pShortName, size_t buffersize);//取出不含底部資料夾的短檔名 
	bool                       ExtractShortFilenameA(LPCSTR pFolder, LPCSTR pfilename, LPSTR pShortName, size_t buffersize);//取出不含底部資料夾的短檔名 
	bool                       ExtractShortFilenameW(LPCWSTR pFolder, LPCWSTR pfilename, LPWSTR pShortName, size_t buffersize);//取出不含底部資料夾的短檔名 

	bool                       ExtractIPAddress(LPCSTR str, BYTE &nField0, BYTE &nField1, BYTE &nField2, BYTE &nField3);//從字串取得IP網址
	bool                       ExtractIPAddress(LPCWSTR str, BYTE &nField0, BYTE &nField1, BYTE &nField2, BYTE &nField3);//從字串取得IP網址
	//---------------------------------------------------------------------------------//		
	unsigned int               GetStringCharValue(LPCTSTR string);//取得字串的碼
	//---------------------------------------------------------------------------------//	
	bool                       RemoveChar(char ch, std::string &String);//移除字元
	bool                       RemoveChar(wchar_t ch, std::wstring &String);//移除字元
	//-----------------------------------------------------------------------------//
	std::string                Trim(const std::string &Str, const std::string &charList);	
	std::string                TrimLeft(const std::string &Str, const std::string &charList);	
	std::string                TrimRight(const std::string &Str, const std::string &charList);
	//-----------------------------------------------------------------------------//
	std::wstring               Trim(const std::wstring &Str, const std::wstring &charList);	
	std::wstring               TrimLeft(const std::wstring &Str, const std::wstring &charList);
	std::wstring               TrimRight(const std::wstring &Str, const std::wstring &charList);
	//-----------------------------------------------------------------------------//
	bool                       ReadNextString(const std::string &textline, size_t &Pos, std::string &String);//讀取字串
	bool                       ReadNextString(const std::wstring &textline, size_t &Pos, std::wstring &String);//讀取字串
	//-----------------------------------------------------------------------------//
	bool                       RemovePackedString(std::string &textline, char ch='\"');//移除包裹的字串的包裹符號
	bool                       RemovePackedString(std::wstring &textline, wchar_t ch=L'\"');//移除包裹的字串的包裹符號
	//-----------------------------------------------------------------------------//
	bool                       CheckPackedString(const std::string &textline, char ch='\"');//確認是包裹的字串
	bool                       CheckPackedString(const std::wstring &textline, wchar_t ch=L'\"');//確認是包裹的字串
	//-----------------------------------------------------------------------------//	
	bool                       CheckIsDelimiter(char ch, const std::vector<char> &DelimList); //確認是否是分隔符號
	bool                       CheckIsDelimiter(wchar_t ch, const std::vector<wchar_t> &DelimList); //確認是否是分隔符號
	//-----------------------------------------------------------------------------//	
	bool                       ListSubString(const std::string &textline, char Delim, std::vector<std::string> &strList);//列出子字串
	bool                       ListSubString(const std::wstring &textline, wchar_t Delim, std::vector<std::wstring> &strList);//列出子字串
	//-----------------------------------------------------------------------------//
	bool                       ListSubString(const std::string &textline, const std::vector<char> &DelimList, char TextFilter, std::vector<std::string> &strList);//列出子字串
	bool                       ListSubString(const std::wstring &textline, const std::vector<wchar_t> &DelimList, wchar_t TextFilter, std::vector<std::wstring> &strList);//列出子字串
	//-----------------------------------------------------------------------------//	
	bool                       DecoderNPM_BadBlockText(const std::string &textline, size_t &nRow, std::string &ColCode);
	bool                       DecoderNPM_BadBlockText(const std::wstring &textline, size_t &nRow, std::wstring &ColCode);
	//-----------------------------------------------------------------------------//
	bool                       DecoderTextLine(const TCHAR textline[], TCHAR index[], TCHAR data[], TCHAR Delim=_T(','));
	bool                       DecoderTextLine(const TCHAR textline[], TCHAR index[], TCHAR data[], TCHAR data2[], TCHAR Delim=_T(','));
	bool                       DecoderTextLineA(const char textline[], char index[], char data[], char Delim=',');
	bool                       DecoderTextLineA(const char textline[], char index[], char data[], char data2[], char Delim=',');
	bool                       DecoderTextLineW(const wchar_t textline[], wchar_t index[], wchar_t data[], wchar_t Delim=L',');
	bool                       DecoderTextLineW(const wchar_t textline[], wchar_t index[], wchar_t data[], wchar_t data2[], wchar_t Delim=L',');
	//-----------------------------------------------------------------------------//
	//字串擴展
	bool                       ExpandString_Increment(const std::string &src, int inc, std::string &dst, int base);//字串擴展-自動疊加
	bool                       ExpandString_Increment(const std::wstring &src, int inc, std::wstring &dst, int base);//字串擴展-自動疊加
	bool                       ExpandString_AddChar(const std::string &src, int nChar, int val, std::string &dst);//字串擴展-外加1字元
	bool                       ExpandString_AddChar(const std::wstring &src, int nChar, int val, std::wstring &dst);//字串擴展-外加2字元
	bool                       ExpandString_Replace(const std::string &src, int nChar, int val, std::string &dst);//字串擴展-取代1字元
	bool                       ExpandString_Replace(const std::wstring &src, int nChar, int val, std::wstring &dst);//字串擴展-取代2字元
	//文字比較
	bool                       FindTextInString(LPCTSTR Text, LPCTSTR String); 
	bool                       FindTextInStringA(LPCSTR Text, LPCSTR String); 
	bool                       FindTextInStringW(LPCWSTR Text, LPCWSTR String);

	double                     MatchTwoString(LPCTSTR String1, LPCTSTR String2); 
	double                     MatchTwoStringA(LPCSTR String1, LPCSTR String2); 
	double                     MatchTwoStringW(LPCWSTR String1, LPCWSTR String2); 
	//-----------------------------------------------------------------------------//
	FILE*                      OpenReadFile(LPCTSTR pfilename, bool bUnicode=false);
	//-----------------------------------------------------------------------------//
	bool                       ModifyOpenFileMode_Read(TCHAR fileMode[]);//取得寫檔的Unicode修飾詞
	bool                       ModifyOpenFileMode_Write(TCHAR fileMode[]);//取得讀檔的Unicode修飾詞	
	//-----------------------------------------------------------------------------//
	bool                       string2lower(std::string &string);//字串轉小寫
	bool                       string2upper(std::string &string);//字串轉大寫		
	bool                       wstring2lower(std::wstring &string);//字串轉大寫
	bool                       wstring2upper(std::wstring &string);//字串轉小寫
	//-----------------------------------------------------------------------------//
	char*                      ReSizeList(size_t Need, std::vector<char> &List);//重設陣列大小
	wchar_t*                   ReSizeList(size_t Need, std::vector<wchar_t> &List);//重設陣列大小
	//-----------------------------------------------------------------------------//
	bool                       AddBackslash(char Buffer[], size_t BufferSize);//增加反斜線
	bool                       AddBackslash(wchar_t whBuffer[], size_t BufferSize);//增加反斜線
	//-----------------------------------------------------------------------------//
	int                        char2wstring(const char *String, std::wstring &result, UINT Code=CP_ACP);//char to str::wstring
	int                        wchar2string(const wchar_t *String, std::string &result, UINT Code=CP_ACP);//wchar to str::string
	int                        wchar2char(const wchar_t *String, char whBuffer[], size_t BufferSize, UINT Code=CP_ACP);//wchar_t to char
	int                        char2wchar(const char *String, wchar_t whBuffer[], size_t BufferSize, UINT Code=CP_ACP);//char to wchar_t		
	bool                       TCHAR2string(LPCTSTR String, std::string &result, UINT Code=CP_ACP);//TCHAR to std::string
	bool                       TCHAR2wstring(LPCTSTR String, std::wstring &result, UINT Code=CP_ACP);//TCHAR to std::wstring
	bool                       TCHAR2char(LPCTSTR String, char chBuffer[], size_t BufferSize, UINT Code=CP_ACP);//TCHAR to char	
	bool                       TCHAR2wchar(LPCTSTR String, wchar_t whBuffer[], size_t BufferSize, UINT Code=CP_ACP);//TCHAR to wchar_t	
	bool                       TCHARCopy(LPCTSTR TChar, char AChar[], size_t ACharSize, wchar_t WChar[], size_t WCharSize, UINT Code=CP_ACP);//TCHAR to char and w_chart
	//-----------------------------------------------------------------------------//
	bool                       AdjustTextA(LPCSTR ptext, char ReplaceCh, LPSTR pResult, size_t size);//將字元濾除
	bool                       AdjustTextW(LPCWSTR ptext, wchar_t ReplaceCh, LPWSTR pResult, size_t size);//將字元濾除
	bool                       AdjustTextS(LPCTSTR ptext, TCHAR ReplaceCh, CString &str);//將字元濾除
	bool                       AdjustTextT(LPCTSTR ptext, TCHAR ReplaceCh, LPTSTR pResult, size_t size);//將字元濾除
	//-----------------------------------------------------------------------------//		
	bool                       FilerTextA(LPCSTR ptext, LPCSTR pTextFilter, LPSTR pResult, size_t size);//將字元濾除
	bool                       FilerTextW(LPCWSTR ptext, LPCWSTR pTextFilter, LPWSTR pResult, size_t size);//將字元濾除
	bool                       FilerTextS(LPCTSTR ptext, LPCTSTR pTextFilter, CString &str);//將字元濾除
	bool                       FilerTextT(LPCTSTR ptext, LPCTSTR pTextFilter, LPTSTR pResult, size_t size);//將字元濾除	
	//-----------------------------------------------------------------------------//
	bool                       CheckJSONStringA(LPCSTR ptext);//確認JSON字串
	bool                       CheckJSONStringW(LPCWSTR ptext);//確認JSON字串
	bool                       CheckJSONStringT(LPCTSTR ptext);//確認JSON字串
	//-----------------------------------------------------------------------------//	
	bool                       FilterJSONStringA(LPSTR ptext, char ch='_');//濾除JSON字串
	bool                       FilterJSONStringW(LPWSTR ptext, wchar_t ch=L'_');//濾除JSON字串
	bool                       FilterJSONStringT(LPTSTR ptext, TCHAR ch=_T('_'));//濾除JSON字串
	bool                       FilterJSONStringA(std::string &text, char ch='_');//濾除JSON字串
	bool                       FilterJSONStringW(std::wstring &text, wchar_t ch=L'_');//濾除JSON字串
	//-----------------------------------------------------------------------------//	
	CString                    GetSpcBarcodeInternalCode();//取得SPC條碼內碼
	CString                    AddSpcBarcodeInternalCode(LPCTSTR Barcode);//加入SPC條碼內碼
	//-----------------------------------------------------------------------------//	
	CString                    AddKeyToErrorString(LPCTSTR Error, LPCTSTR Key);//在錯誤字串加入關鍵字0
	bool                       AddSubString(CString &FullStr, LPCTSTR SubStr, LPCTSTR Delimiter);//增加子字串至字串內
	//-----------------------------------------------------------------------------//
	void                       GetSystemLastError(CString &Str);//取得系統最新一次錯誤訊息
	//-----------------------------------------------------------------------------//
	bool                       SendDebugString(LPCSTR lpszText);//送出DebugString
	bool                       SendDebugString(LPCWSTR lpszText);//送出DebugString
	bool                       SendDebugString(std::string &str);//送出DebugString
	bool                       SendDebugString(std::wstring &str);//送出DebugString
	//-----------------------------------------------------------------------------//
	int                        ShowMessageBox(LPCSTR lpszText, UINT nType = MB_OK, UINT nIDHelp = 0);//char	
	int                        ShowMessageBoxFn(LPCSTR lpszText, UINT nType = MB_OK, UINT nIDHelp = 0);//char	
	int                        ShowMessageBox(LPCWSTR lpszText, UINT nType = MB_OK, UINT nIDHelp = 0);//wchar_t
	int                        ShowMessageBoxFn(LPCWSTR lpszText, UINT nType = MB_OK, UINT nIDHelp = 0);//wchar_t
	int                        ShowMessageBox(const std::string &str, UINT nType = MB_OK, UINT nIDHelp = 0);//char
	int                        ShowMessageBox(const std::wstring &wstr, UINT nType = MB_OK, UINT nIDHelp = 0);//wchar_t
	//-----------------------------------------------------------------------------//	
	bool                       Base36ToInt(LPCSTR str, int &Value);		
	bool                       Base36ToInt(LPCWSTR str, int &Value);		
	//-----------------------------------------------------------------------------//	
	bool                       ASCII_To_Int(LPCSTR  Data,int *D_i,int Len_i);
	bool                       ASCII_To_Int(LPCWSTR Data,int *D_i,int Len_i);
	bool                       HexToInt(char c, BYTE &Value);
	void                       HexToInt(LPCSTR str, const int Length, int &Value);	
	bool                       HexToInt(wchar_t c, BYTE &Value);
	void                       HexToInt(LPCWSTR str, const int Length, int &Value);	
	bool                       HexToBin(char c, char Bin[]);
	bool                       HexToBin(wchar_t c, wchar_t Bin[]);
	bool                       IntToHex(int Value, int Digits, char str[]);
	bool                       IntToHex(int Value, int Digits, wchar_t str[]);
	bool                       IntToDec(int Value, int Digits, char str[]);//Decima-10進位
	bool                       IntToDec(int Value, int Digits, wchar_t str[]);//Decima-10進位
	bool                       IntToBin(int Value, int Digits, char str[]);
	bool                       IntToBin(int Value, int Digits, wchar_t str[]);
	bool                       IntToAny(int Value, int Digits, char str[], int base);//2~36進位
	bool                       IntToAny(int Value, int Digits, wchar_t str[], int base);//2~36進位
	//-----------------------------------------------------------------------------//
	bool                       CheckPtInCtrlWnd(CWnd *ParentWnd, POINT Pt, UINT CtrlID, POINT *Pt2=NULL);//確認點到該控制項
	CWnd*                      FocusCtrlWnd(CWnd *ParentWnd, UINT CtrlID);//焦點控制項
	bool                       ShowCtrlWnd(CWnd *ParentWnd, UINT CtrlID, BOOL bShow);//顯示控制項	
	bool                       EnableCtrlWnd(CWnd *ParentWnd, UINT CtrlID, BOOL bEnable);//啟用控制項
	bool                       EnableEditWnd(CWnd *ParentWnd, UINT CtrlID, BOOL bEnable);//啟用編輯控制項
	bool                       CheckRadioWnd(CWnd *ParentWnd, UINT CtrlID, UINT ActCtrlID);//啟用Radio控制項
	bool                       MoveCtrlWnd(CWnd *ParentWnd, UINT CtrlID, const RECT &Rect);//移動控制項
	bool                       MoveCtrlWnd(CWnd *ParentWnd, UINT CtrlID, const POINT &Offset);//移動控制項
	//-----------------------------------------------------------------------------//
	HWND                       GetAncestorHWnd(HWND hWnd);
	bool                       SendFrameWndMessage(CWnd *pWnd, UINT message, WPARAM wParam, LPARAM lParam);//發送訊息
	bool                       PostFrameWndMessage(CWnd *pWnd, UINT message, WPARAM wParam, LPARAM lParam);//發送訊息
	//-----------------------------------------------------------------------------//	
	void                       SleepTime(DWORD dwMilliseconds, bool bTickCheck);//暫停
	//-----------------------------------------------------------------------------//	
	void                       RemoveWndBusy(HWND hWnd);//移除視窗忙碌
	int                        RemoveMessage(HWND hWnd, UINT MsgFirst, UINT MsgEnd);//清除特定視窗的訊息(從佇列清除)	
	void                       SleepMessage(DWORD dwMilliseconds, BOOL SkipMsg, HWND hWnd);//訊息暫停, 可以移除訊息的暫停	
	//-----------------------------------------------------------------------------//
	BOOL                       ClearListBox(CListBox &ListBox);//清除列表盒元件內容			
	//-----------------------------------------------------------------------------//
	int                        GetEnsureVisibleIndex(int ItemIndex, int ItemCount);//取得確保顯示的項目引數
	BOOL                       InitialListCtrl(CListCtrl &ListCtrl);//初始化列表控制元件	
	BOOL                       ClearListCtrl(CListCtrl &ListCtrl, BOOL ClearHeader);//清除列表控制元件內容		
	BOOL                       ClearListCtrl(CJETListCtrl &ListCtrl, BOOL ClearHeader);//清除列表控制元件內容			
	BOOL                       ClearListCtrl(CJETMFCListCtrl &ListCtrl, BOOL ClearHeader);//清除列表控制元件內容				
	BOOL                       ClearListCtrlHeaderList(CListCtrl &ListCtrl);//移除列表物件的標頭	
	int                        GetListCtrlItemByData(CListCtrl &ListCtrl, DWORD_PTR Data);//列表物件
	bool                       SaveListCtrl(LPCTSTR pfilename, CListCtrl &ListCtrl);//儲存列表控制元件內容	
#if FRAME_STYLE_TYPE != FRAME_STYLE_MFC
	BOOL                       ClearListCtrlHeaderList(CMFCListCtrl &ListCtrl);//移除列表物件的標頭
	BOOL                       ClearListCtrl(CMFCListCtrl &ListCtrl, BOOL ClearHeader);//清除列表控制元件內容		
	BOOL                       InitialListCtrl(CMFCListCtrl &ListCtrl);//列表物件的初始化
	int                        GetListCtrlItemByData(CMFCListCtrl &ListCtrl, DWORD_PTR Data);//列表物件
#endif//FRAME_STYLE_TYPE
	//-----------------------------------------------------------------------------//
	BOOL                       InitialTreeCtrl(CTreeCtrl &TreeCtrl);//初始化樹狀圖控制元件
	HTREEITEM                  GetTreeSelectedItem(CTreeCtrl &TreeCtrl);
	int                        GetTreeNodeLevel(CTreeCtrl &TreeCtrl, HTREEITEM hItem);
	//-----------------------------------------------------------------------------//
	bool                       GetTwoLinesCrossPoint(double m1, double b1, double m2, double b2, double &x, double &y);//取得線對線的交點
	//-----------------------------------------------------------------------------//
	bool                       GetDCUnityTransform(XFORM &xForm);//取得DC的座標矩陣-單位矩陣
	bool                       RotateDCTransform(double Angle, double X, double Y, XFORM &xForm);//旋轉DC的座標矩陣
	//-----------------------------------------------------------------------------//
	bool                       Size2DToSize(const TSIZE2D &Size, SIZE &Sz);//尺寸資料轉換	
	bool                       Point2DToPoint(const TPOINT2D &Point, POINT &Pt);//點資料轉換		
	//-----------------------------------------------------------------------------//
	bool                       PointsSize(double *PtX, double *PtY, int NPts, double &CW, double &CH);//點群尺寸範圍
	bool                       PointsCenter(const TPOINT2D *PTList, int NPTs, TPOINT2D &Cp);//點群中間座標	
	bool                       PointsCenter(double *PtX, double *PtY, int NPts, double &CX, double &CY);//點群中間座標				

	bool                       CornerPtToRect(const POINT CornerPt[], RECT &Rect);//四個端點合成一個矩形	
	bool                       CornerPtToRect(const TPOINT2D CornerPt[], RECT &Rect);//四個端點合成一個矩形	
	bool                       CornerXYToRect(const double CornerX[], const double CornerY[], RECT &Rect);//四個端點合成一個矩形
	bool                       CornerPtToRegion(const TPOINT2D CornerPt[], TREGION4D &Region);//四個端點合成一個矩形	
	bool                       CornerXYToRegion(const double CornerX[], const double CornerY[], TREGION4D &Region);//四個端點合成一個矩形

	bool                       CornerPt2DToCornerPt(const TPOINT2D dCornerPt[], POINT nCornerPt[]);//四個端點資料轉換

	bool                       PointsToRect(const POINT &pt1, const POINT &pt2, RECT &Rect);//兩點合成一個矩形
	bool                       PointsToRect(const TPOINT2D &pt1, const TPOINT2D &pt2, RECT &Rect);//兩點合成一個矩形
	bool                       PointsToRect(const TPOINT2D &pt1, const TPOINT2D &pt2, TRECT4D &Rect);//兩點合成一個矩形
	void                       PointsToRect(const POINT *PTList, int NPTs, RECT &Rect);//將四個點合成一個矩形	
	void                       PointsToRect(const TPOINT2D *PTList, int NPTs, TRECT4D &Rect);//將四個點合成一個矩形		
	void                       PointListToRect(const std::vector<POINT> &PtList, RECT &Rect);//將點合成一個矩形	
	void                       PointListToRect(const std::vector<TPOINT2D> &PtList, TRECT4D &Rect);//將點合成一個矩形	
	void                       PointsToRegion(const TPOINT2D *PTList, int NPTs, TREGION4D &Region);//將四個點合成一個矩形
	void                       PointsToRegion(const TPOINT3D *PTList, int NPTs, TREGION4D &Region);//將四個點合成一個矩形
	void                       PointsToRegion(const TPOINT2D &Pt1, const TPOINT2D &Pt2, TREGION4D &Region);//將2個點合成一個矩形
	//-----------------------------------------------------------------------------//	
	bool                       GetRectSize(const RECT &Rect, SIZE &Size);
	bool                       GetRectSize(const RECT &Rect, int &nW, int &nH);
	bool                       GetRectCenterPos(const RECT &Rect, POINT &Cp);
	bool                       GetRectCenterPos(const RECT &Rect, int &CpX, int &CpY);
	bool                       RectInRect(const RECT &Rect, const RECT &rcMatrix);
	bool                       PtInRect(const TPOINT2D &pt, const TRECT4D &Rect);
	bool                       PtInRegion(const TPOINT2D &pt, const TREGION4D &Region);	
	//-----------------------------------------------------------------------------//	
	bool                       BoundaryRect(const RECT &Boundary, RECT &Rect);//調整Rect範圍	
	bool                       SizeToRect(unsigned int RectW, unsigned int RectH, RECT &Rect);//尺寸轉程Rect
	bool                       BoundaryRect(unsigned int RectW, unsigned int RectH, RECT &Rect);//調整Rect範圍	

	bool                       AdjustRect(const RECT RectSrc, RECT &RectDst);//調整區域重新判斷大小值
	bool                       AdjustRect(const TRECT4D RectSrc, TRECT4D &RectDst);//調整區域重新判斷大小值
	bool                       CheckPickSelectMode(const TREGION4D &Rgn, double Range);//確認是否為點選模式		
	bool                       AdjustRegion(const TREGION4D RgnSrc, TREGION4D &RgnDst);//調整區域重新判斷大小值
	bool                       BoundaryRegion(const TREGION4D &BoundaryRgn, TREGION4D &Rgn);//局限區域範圍		
	bool                       Rect4DToRect(const TRECT4D &dRect, RECT &Rect);//將區域轉成Rect	
	bool                       Region4DToRect(const TREGION4D &Region, RECT &Rect, bool Recheck);//將區域轉成Rect	
	bool                       AdjustRectByAlignW(RECT &Rect, int nAlign);//調整Rect寬度至N倍數
	bool                       RectToCornerPt(const RECT &Rect, TPOINT2D CornerPt[]);//將區域轉成4端點
	bool                       Rect4DToCornerPt(const TRECT4D &dRect, TPOINT2D CornerPt[]);//將區域轉成4端點
	bool                       CheckCornerInRect(const POINT Corner[], const RECT &Rect);//確認4端點在區域內
	bool                       CheckCornerInRect(const TPOINT2D Corner[], const RECT &Rect);//確認4端點在區域內

	bool                       ScaleCornerPosSize(double sx, double sy, TPOINT2D CornerPos[]);//將4端點尺寸縮放
	bool                       ScaleRect(const RECT &Rect, double sx, double sy, int Mode, RECT &rt);//將區域縮放	
	bool                       ScaleRegion(const TREGION4D &Region, double sx, double sy, int Mode, TREGION4D &Rgn);//將區域縮放	
	bool                       CheckImageRoi(int ImageW, int ImageH, const RECT &RoiRect);//確認影像區域
	bool                       CheckImageRoi(unsigned int ImageW, unsigned int ImageH, const RECT &RoiRect);//確認影像區域
	void                       UnionRect(const RECT &Rect1, const RECT &Rect2, RECT &Rect);//兩個RECT的集合
	void                       IntersectRect(const RECT &Rect1, const RECT &Rect2, RECT &Rect);//兩個RECT的交合
	void                       UnionRegion(const TREGION4D &Rgn1, const TREGION4D &Rgn2, TREGION4D &Rgn);//兩個Region的集合
	void                       IntersectRegion(const TREGION4D &Rgn1, const TREGION4D &Rgn2, TREGION4D &Rgn);//兩個Region的交合
	void                       ExcludeRegion(const TREGION4D &Region, const TREGION4D &ExcRgn, BOX_TOWARD Toward, TREGION4D &Rgn);//Region剔除ExcRgn
	bool                       CheckPtInRegion(double Px, double Py, const TREGION4D &Rgn);//確認點在區域內	
	bool                       CheckRectInSize(const RECT &ObjRect, int SizeW, int SizeH);//確認區域在尺寸內	
	bool                       CheckRectInRect(const RECT &ObjRect, const RECT &BoundryRect, bool bEntireIn);//確認區域在區域內	
	bool                       CheckRgnInRegion(const TREGION4D &ObjRgn, const TREGION4D &BoundryRgn, bool bEntireIn);//確認區域在區域內	
	bool                       MapRegionToRect(const TREGION4D &Rgn1, const TREGION4D &Rgn2, const RECT &Rect1, RECT &Rect2);//區域映射
	bool                       MapRectToRegion(const RECT &Rect1, const RECT &Rect2, const TREGION4D &Rgn1, TREGION4D &Rgn2);//區域映射
	bool                       CalcRegionRect(const TREGION4D &BoundaryRgn, const RECT &BoundaryRect, const TREGION4D &Rgn, RECT &Rect, bool InverY);//將區域鏡射		
	//-----------------------------------------------------------------------------//
	int                        GetTowardSize(double Scale);//取得朝向尺寸
	int                        GetEditLineSize(double Scale, int Level);//取得編輯線的尺寸
	int                        GetEditCheckSize(double Scale, int Level);//取得編輯確認的尺寸
	double                     CalcBlobRatio(double BlobW, double BlobH, BOX_TOWARD Toward);		
	//-----------------------------------------------------------------------------//
	
	//-----------------------------------------------------------------------------//
	int                        GetRandomValue();//取得隨機變數
	int                        GetRandomValue(int Mod);//取得隨機變數-餘數
	int                        GetRandomValue(int Seed, int Mod);//取得隨機變數-餘數
	double                     GetRandomValue(double Min, double Max);//取得隨機變數
	int                        GetAngleLabel(double dAngle);//取得角度象限	
	double                     AdjustRotationAngle(double Angle);//調整旋轉角度
	bool                       CheckIsExceptionAngle(double Angle);//判斷是否為斜角度
	double                     MapCadAngleToImageAngle(double Angle);//將Cad的角度轉成圖像角度
	double                     MapCadAngleToStageAngle(double Angle);//將Cad的角度轉成機台角度	
	void                       RotateSize(double rAngle, SIZE &size);//尺寸旋轉
	void                       RotateSize(double rAngle, TSIZE2D &size);//尺寸旋轉	
	void                       RotateSize(double rAngle, double &cx, double &cy);//尺寸旋轉		
	double                     RotateAngle(double rAngle, double dAngle);//角度旋轉	
	void                       RotateRect(double rAngle, int W, int H, RECT &Rect);//區域旋轉
	void                       RotatePos(double rAngle, double CPX, double CPY, TPOINT2D &Pos);//座標旋轉
	void                       RotatePos(double rAngle, double CPX, double CPY, double &px, double &py);//座標旋轉	
	void                       RotatePosList(double Angle, double CPX, double CPY, std::vector<POINT> &PtList);//旋轉點列表
	void                       RotatePosList(double Angle, double CPX, double CPY, std::vector<TPOINT2D> &PtList);//旋轉點列表
	void                       RotatePos(double pX, double pY, double CpX, double CpY, double AngleRad, double &X, double &Y);//座標旋轉
	void                       RotateRegion(double Angle, double CPX, double CPY, const TREGION4D &rgnSrc, TREGION4D &rgnDst);//區域旋轉	
	void                       RotateCornerPos(double Angle, TPOINT2D CornerPos[]);//角落座標旋轉
	void                       RotateCornerPos(double Angle, double CPX, double CPY, TPOINT2D CornerPos[]);//角落座標旋轉
	void                       RotateCornerPos(double Angle, double CPX, double CPY, double CornerPosX[], double CornerPosY[]);//角落座標旋轉	
	DWORD                      RotateSideMode(double Angle, DWORD SideMode);
	BOX_TOWARD                 RotateToward(double Angle, BOX_TOWARD Toward);
	BOARD_ORIENTATION_MODE     RotateBoardOrientationMode(BOARD_ORIENTATION_MODE Mode, double dAngle);//單板方向旋轉
	//-----------------------------------------------------------------------------//	
	bool                       ExtractOutline(const std::vector<POINT> &PtList, std::vector<POINT> &Outline);//萃取出輪廓線
	//-----------------------------------------------------------------------------//	
	void                       MoveRect(const RECT &rectSrc, const TPOINT2D &MovePt, RECT &rectDst);
	void                       MoveRegion(const TREGION4D &rgnSrc, const TPOINT2D &MovePt, TREGION4D &rgnDst);
	void                       MoveCornerPts(const TPOINT2D CornerPts[4], const TPOINT2D &MovePt, TPOINT2D CornerPtsDst[4]);
	//-----------------------------------------------------------------------------//	
	double                     MirrorXAxisAngle(double Angle);//角度鏡射-X軸
	double                     MirrorYAxisAngle(double Angle);//角度鏡射-Y軸
	void                       MirrorXAxisToward(BOX_TOWARD &Toward);//朝向鏡射-X軸	
	void                       MirrorYAxisToward(BOX_TOWARD &Toward);//朝向鏡射-Y軸
	void                       MirrorXAxisPos(double CPY, double &PosY);//座標鏡射-X軸
	void                       MirrorYAxisPos(double CPX, double &PosX);//座標鏡射-Y軸
	void                       MirrorXAxisPos(double CPY, TPOINT2D &Pos);//座標鏡射-X軸
	void                       MirrorYAxisPos(double CPX, TPOINT2D &Pos);//座標鏡射-Y軸	
	void                       MirrorXAxisRegion(double CPY, TREGION4D &Region);//區域鏡射-X軸
	void                       MirrorYAxisRegion(double CPX, TREGION4D &Region);//區域鏡射-Y軸	
	void                       MirrorPos(BOX_TOWARD &T1, BOX_TOWARD &T2, double &dPx, double &dPy);	
	void                       MirrorXAxisRegion(double CPY, double &MinX, double &MinY, double &MaxX, double &MaxY);//區域鏡射-X軸
	void                       MirrorYAxisRegion(double CPX, double &MinX, double &MinY, double &MaxX, double &MaxY);//區域鏡射-Y軸		
	BOARD_ORIENTATION_MODE     MirrorXAxisBoardOrientationMode(BOARD_ORIENTATION_MODE OrientationMode);//單板方向鏡射-Y軸
	BOARD_ORIENTATION_MODE     MirrorYAxisBoardOrientationMode(BOARD_ORIENTATION_MODE OrientationMode);//單板方向鏡射-Y軸
	//-----------------------------------------------------------------------------//
	void                       CalcBoardOrientationOffset(BOARD_ORIENTATION_MODE RefMode, BOARD_ORIENTATION_MODE NewMode, double &Angle, bool &MirrorXAxis, bool &MirrorYAxis);
	//-----------------------------------------------------------------------------//
	bool                       CheckIsPressVRKey(UINT VK);//確認是否按下特定鍵盤
	void                       UpdateCursor(CURSOR_POS_MODE Mode);
	CURSOR_POS_MODE            MapCadCursorPosModeToImageCursorPosMode(CURSOR_POS_MODE CursorMode, bool SignX, bool SignY);//將Cad的鼠標方向改成影像的鼠標方向
	CURSOR_POS_MODE            MapStageCursorPosModeToCadCursorPosMode(CURSOR_POS_MODE CursorMode, bool SignX, bool SignY);//將機台的鼠標方向改成Cad的鼠標方向
	CURSOR_POS_MODE            MapStageCursorPosModeToImageCursorPosMode(CURSOR_POS_MODE CursorMode, bool SignX, bool SignY);//將機台的鼠標方向改成影像的鼠標方向
	CURSOR_POS_MODE            CheckCursorPosMode(const RECT &BoxRect, const SIZE &szGrid, const POINT &point, int ChkMode=CHECK_CURSOR_MODE_FRAME);//確認鼠標座標模式	
	bool                       CalcModifySizeRegion(CURSOR_POS_MODE CursorMode, const bool DoubleEdit, double dPx, double dPy, TREGION4D &dRgn);	
	//-----------------------------------------------------------------------------//
	bool                       InitialUUID(UUID &uuid);//UUID的初始化	
	bool                       UUIDFromStringA(LPCSTR Text, UUID &uuid);
	bool                       UUIDFromStringW(LPCWSTR Text, UUID &uuid);
	bool                       UUIDToString(const UUID &uuid, CString &Text);	
	bool                       UUIDToStringA(const UUID &uuid, char Text[]);
	bool                       UUIDToStringW(const UUID &uuid, wchar_t Text[]);
	//-----------------------------------------------------------------------------//	
	void                       ClearCastParam(TCastParam &CastParam);
	//-----------------------------------------------------------------------------//
	void                       ClearUniFrame(TUNI_FRAME &UniFrame);
	void                       InitialUniFrame(TUNI_FRAME &UniFrame);	
	void                       ClearUniFrameList(std::vector<TUNI_FRAME> &UniFrameList);	
	void                       ClearUniFrameList(TUNI_FRAME *UniFrameListPtr, size_t Count);
	void                       InitialUniFrameList(TUNI_FRAME *UniFrameListPtr, size_t Count);
	bool                       CloneUniFrameList(const char *fnName, const std::vector<TUNI_FRAME> &UniFrameListSrc, std::vector<TUNI_FRAME> &UniFrameListDst);
	//-----------------------------------------------------------------------------//	
	DWORD                      BitMask_Add(DWORD val, DWORD msk);//位元遮罩相加
	DWORD                      BitMask_Remove(DWORD val, DWORD msk);//位元遮罩相加
	bool                       BitMask_Check(DWORD val, DWORD Msk);//遮罩比較
	//-----------------------------------------------------------------------------//
	bool                       CheckMoved(double x1, double y1, double x2, double y2);//確認移動過
	//-----------------------------------------------------------------------------//
	int                        StrToInt(LPCSTR str);//字串轉整數
	double                     StrToDbl(LPCSTR str);//字串轉浮點數
	int                        StrToInt(LPCWSTR  str);//字串轉整數
	double                     StrToDbl(LPCWSTR  str);//字串轉浮點數
	//-----------------------------------------------------------------------------//
	bool                       TimeDelay_Sleep(DWORD dwMilliseconds);//時間延遲-使用Sleep
	bool                       TimeDelay_TickCount(DWORD dwMilliseconds);//時間延遲-使用TickCount
	//-----------------------------------------------------------------------------//	
	CString                    SearchOtherTestFolder(const std::vector<CString> &FolderList, LPCTSTR refDateTime, bool bNext);//尋找其他檢測資料夾
	//-----------------------------------------------------------------------------//
	void                       SwapFunc(float &v1, float &v2);
	int                        QuickSort_Partition(std::vector<float> &List, int front, int end);//快速排序-分割
	bool                       QuickSort_Recursion(std::vector<float> &List, int front, int end);//快速排序-遞迴
	bool                       QuickSort_Iterative(std::vector<float> &List, int front, int end);//快速排序-疊代

	bool                       MergeSort_Merge(std::vector<float> &List, int front, int mid, int end);//合併排序-合併
	bool                       MergeSort_Recursion(std::vector<float> &List, int front, int end);//合併排序-遞迴
	bool                       MergeSort_Iterative(std::vector<float> &List, int count);//合併排序-疊代

	bool                       CheckListSorted(const std::vector<float> &List);//確認已經排序過
	bool                       SortList_Insertion(std::vector<float> &List);//數量少時較快(數量<16)
	bool                       SortList_Selection(std::vector<float> &List);//數量少時較快(數量<4)
	bool                       SortList_QuickSort(std::vector<float> &List);//快速排序法(數量>500)
	bool                       SortList_MergeSort(std::vector<float> &List);//合併排序法(數量>500)
	//-----------------------------------------------------------------------------//	
	bool                       Calc2LinePoint(const TLINE2D &L1, const TLINE2D &L2, TPOINT2D &P);//求得2線交點
	bool                       Calc2PointLine(const TPOINT2D &P1, const TPOINT2D &P2, TLINE2D &Line);//求得2點的直線(ax+by=c)
	bool                       CalcOrthogonalLine(const TPOINT2D &P, const TLINE2D &L1, TLINE2D &Line);//求得正交直線(ax+by=c)	
	//-----------------------------------------------------------------------------//	
	bool                       CalcFittingLine(const std::vector<TPOINT2D> &PtList, double &nX, double &nY);//線段擬合
	bool                       CalcFittingPlane(const std::vector<TPOINT3D> &PtList, double &nX, double &nY, double &nZ);//平面擬合	
	bool                       BilinearInterpolation(const std::vector<TPOINT3D> &PtList, double PosX, double PosY, double &PosZ);//雙向內插法
	//-----------------------------------------------------------------------------//
}
//-------------------------------------------------------------------------------------//
#endif//JetAPIUtility