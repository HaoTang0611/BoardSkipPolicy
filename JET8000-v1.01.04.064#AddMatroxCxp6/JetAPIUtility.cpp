#include "stdafx.h"
#include "JetAPIUtility.h"
//-----------------------------------------------------------------------------//
#include <initguid.h>
#include <Setupapi.h>
#include "OpenCV_Def.h"
//-----------------------------------------------------------------------------//
#pragma comment(lib, "setupapi")
//-----------------------------------------------------------------------------//
#define  CHECK_FILE_SYNC_NAME  _T("SYNC")
//-----------------------------------------------------------------------------//
// The following define is from ntddser.h in the DDK. It is also needed for serial port enumeration.
#ifndef GUID_CLASS_COMPORT
DEFINE_GUID(GUID_CLASS_COMPORT, 0x86e0d1e0L, 0x8089, 0x11d0, 0x9c, 0xe4, 0x08, 0x00, 0x3e, 0x30, 0x1f, 0x73);
#endif//GUID_CLASS_COMPORT
//-----------------------------------------------------------------------------//
namespace JetAPI
{
	bool gStopCopyFunc=false;
//-----------------------------------------------------------------------------//
bool OpenFolderDialog(CWnd *pWnd, CString &Folder)
{
	AOIDataCollect.AddWaitUserInputCount();
	const bool bSucc=OpenFolderDialogFn(pWnd, Folder);
	AOIDataCollect.ReleaseWaitUserInputCount();
	if ( AOIDataCollect.CheckWaitUserInputCountZero() )
	{	AOIDataCollect.RegistUserLastInputTickCount();	}	
	return bSucc;
}
//-----------------------------------------------------------------------------//
bool OpenFolderDialogFn(CWnd *pWnd, CString &Folder)
{
	::SetCurrentDirectory(Folder);
	
	BROWSEINFO   browse;   
	TCHAR        FolderBuffer[MAX_JET_PATH]=_T("");

	::_tcscpy(FolderBuffer, Folder);
	::ZeroMemory(&browse,sizeof(browse)); 

	if ( pWnd == NULL )
	{	browse.hwndOwner   =   NULL;	}
	else
	{	browse.hwndOwner   =   pWnd->GetSafeHwnd();   }	

	browse.pszDisplayName  =  FolderBuffer;
	browse.lpszTitle       =   _T("");   
	browse.ulFlags         =   BIF_RETURNONLYFSDIRS;	
	browse.ulFlags         =   BIF_RETURNONLYFSDIRS|BIF_NEWDIALOGSTYLE;//BIF_USENEWUI;//|BIF_STATUSTEXT;//For Test
	browse.lpfn = BrowseCallbackProc;
	
	LPITEMIDLIST   lpItem   =   SHBrowseForFolder(&browse);
	if ( lpItem   ==   NULL)   { return false; }
    
	if ( SHGetPathFromIDList(lpItem,FolderBuffer) == false)  
	{ 	
		GlobalFree(lpItem);
		return false; 
	}	
	GlobalFree(lpItem);	

	if ( ::_tcslen(FolderBuffer) == 3 )
	{
		if ( FolderBuffer[2] == _T('\\') ) 
		{	FolderBuffer[2] = _T('\0');	}
	}
	Folder = FolderBuffer;
	return true;
}
//-----------------------------------------------------------------------------//
BOOL CALLBACK SetChildWndFont(HWND hWnd, LPARAM lParam) 
{
	if ( NULL == hWnd ) { return TRUE; }
	BOOL bRedraw=TRUE;	
	::SendMessage(hWnd, WM_SETFONT, (WPARAM)lParam, bRedraw);
	return TRUE;
}
//-----------------------------------------------------------------------------//
int  CALLBACK BrowseCallbackProc(HWND hwnd,UINT uMsg,LPARAM lp, LPARAM pData)
{
	TCHAR szDir[MAX_JET_PATH];
	switch ( uMsg ) 
	{
	case BFFM_INITIALIZED: 
		{
			if ( GetCurrentDirectory(MAX_JET_PATH,szDir) ) 
			{		
				SendMessage(hwnd,BFFM_SETSELECTION,TRUE,(LPARAM)szDir);
			}
		break;
		}
	case BFFM_SELCHANGED: 
		{
			// Set the status window to the currently selected path.
			if ( SHGetPathFromIDList((LPITEMIDLIST) lp ,szDir) ) 
			{
				SendMessage(hwnd,BFFM_SETSTATUSTEXT,0,(LPARAM)szDir);
			}
		break;
		}
	default:
		break;
	}
	return 0;	
}
//-----------------------------------------------------------------------------//
bool OpenFileDialog(CString &Filename, BOOL bOpen, LPCTSTR lpszDefExt,	LPCTSTR lpszFileName, DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd, DWORD dwSize, BOOL bVistaStyle)
{
	TCHAR   filename[MAX_JET_PATH]=_T("");
	CFileDialog dialog(bOpen, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd, dwSize, bVistaStyle);
	if ( TRUE==bOpen && Filename.GetLength()>0 )
	{
		::_tcscpy(filename, Filename);
		dialog.m_ofn.lpstrFile  = filename;	
	}	
	if ( dialog.DoModal() == IDCANCEL )
	{	return false; }
	Filename = dialog.GetPathName();
	return true;
}
//-----------------------------------------------------------------------------//
HANDLE CreateEvent(LPSECURITY_ATTRIBUTES lpEventAttributes,BOOL bManualReset, BOOL bInitialState, LPCTSTR lpName)
{		
	CString str;
	CString ExtraName;
	CString EventName;
	HANDLE  Handle=NULL;		
#ifndef OFFLINE_VERSION
	ExtraName = _T("-Online");
#else
	ExtraName = _T("-Offline");
#endif//OFFLINE_VERSION

	SYSTEMTIME LocalTime;
	SYSTEMTIME LocalTime2;
	::GetLocalTime(&LocalTime);
	ExtraName.Format(_T("%.4d%.2d%.2d%.2d%.2d%.2d#%.3d"), LocalTime.wYear, LocalTime.wMonth, LocalTime.wDay, LocalTime.wHour, LocalTime.wMinute, LocalTime.wSecond, LocalTime.wMilliseconds);
	EventName = lpName+ExtraName;	
	Handle = ::CreateEvent(lpEventAttributes, bManualReset, bInitialState, EventName);
	if ( NULL == Handle ) 
	{ 
		str.Format(_T("Error, The Event [%s] create fault"), EventName);
		JetAPI::ShowMessageBox(str);
		return NULL; 
	}

	DWORD Res = ::GetLastError();
	if ( Res == ERROR_ALREADY_EXISTS )
	{	
		str.Format(_T("Error, The Event [%s] already exist"), EventName);
		JetAPI::ShowMessageBox(str);
	}
	else
	{	Res = Res;	}
	//延遲1個ms, 避免連續創見時時間重疊
	DWORD Cnt=0;
	while ( true )
	{
		::GetLocalTime(&LocalTime2);
		if ( LocalTime2.wMilliseconds != LocalTime.wMilliseconds ||
			 LocalTime2.wSecond != LocalTime.wSecond ||
			 LocalTime2.wMinute != LocalTime.wMinute ||
			 LocalTime2.wHour != LocalTime.wHour ||
			 LocalTime2.wDay != LocalTime.wDay ||
			 LocalTime2.wMonth != LocalTime.wMonth ||
			 LocalTime2.wYear != LocalTime.wYear 
			)
		{	break; }
		Cnt ++;
	};
	return Handle;
}
//-----------------------------------------------------------------------------//
bool IsIniSection(LPCSTR pText)
{
	if ( NULL == pText ) { return false; }
	if ( '[' != pText[0] ) { return false; }
	const size_t len=::strlen(pText);
	if ( ']' != pText[len-1] ) { return false; }
	return true;
}
//-----------------------------------------------------------------------------//
bool IsIniSection(LPCWSTR pText)
{
	if ( NULL == pText ) { return false; }
	if ( L'[' != pText[0] ) { return false; }
	const size_t len=::wcslen(pText);	
	if ( L']' != pText[len-1] ) { return false; }
	return true;
}
//-----------------------------------------------------------------------------//
bool SaveINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pString, LPCTSTR pfilename, CString &Error)
{
	if ( ::WritePrivateProfileString(pSection, pKeyName, pString, pfilename) == false )
	{
		//Error.Format(_T("Error, WritePrivateProfileString Fault"));
		Error.Format(_T("Error, WritePrivateProfileString Fault\nFilename:%s\nSection:%s\nKey:%s\nString:%s"), pfilename, pSection, pKeyName, pString);
		return false;
	}
	//Error.Format(_T("Error, WritePrivateProfileString Fault\nFilename:%s\nSection:%s\nKey:%s\nString:%s"), pfilename, pSection, pKeyName, pString);
	return true;
}
//-----------------------------------------------------------------------------//
bool LoadINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pDefault, TCHAR *pString, int StringSize, LPCTSTR pfilename, bool IsCheckLens, CString &Error)
{
	int Res = 0;
	if ( IsCheckLens == true )
	{
		CString tempDefault = _T("..++--");	
		Res = ::GetPrivateProfileString(pSection, pKeyName, tempDefault, pString, StringSize, pfilename);
		if ( pString == tempDefault ) 
		{	
			Error.Format(_T("Error, GetPrivateProfileString Fault\nFilename:%s\nSection:%s\nKey:%s\nDefault:%s"), pfilename, pSection, pKeyName, pDefault);
			return false;	
		}
	}
	::GetPrivateProfileString(pSection, pKeyName, pDefault, pString, StringSize, pfilename);
	return true;
}
//-----------------------------------------------------------------------------//
bool IsEventInt(int val)//是否偶數
{
	//return (val%2)==0;
	return (val&1)==0;
}
//-----------------------------------------------------------------------------//
bool IsEventSize(size_t val)//是否偶數
{	
	//return (val%2)==0;
	return (val&1)==0;
}
//-----------------------------------------------------------------------------//
bool IsEventUInt(unsigned int val)//是否偶數	
{	
	//return (val%2)==0;
	return (val&1)==0;
}
//-----------------------------------------------------------------------------//
void Swap(int &v1, int &v2)//參數對調
{
	int tmp = v1;
	v1 = v2;
	v2 = tmp;
}
//-----------------------------------------------------------------------------//
void Swap(bool &v1, bool &v2)//參數對調
{
	bool tmp=v1;
	v1 = v2;
	v2 = tmp;
}
//-----------------------------------------------------------------------------//
void Swap(char &v1, char &v2)//參數對調
{
	char tmp = v1;
	v1 = v2;
	v2 = tmp;
}
//-----------------------------------------------------------------------------//
void Swap(float &v1, float &v2)//參數對調
{
	float tmp = v1;
	v1 = v2;
	v2 = tmp;
}
//-----------------------------------------------------------------------------//
void Swap(double &v1, double &v2)//參數對調
{
	double tmp = v1;
	v1 = v2;
	v2 = tmp;
}
//-----------------------------------------------------------------------------//
void Swap(wchar_t &v1, wchar_t &v2)//參數對調	
{
	wchar_t tmp = v1;
	v1 = v2;
	v2 = tmp;
}
//-----------------------------------------------------------------------------//
void Swap(unsigned int &v1, unsigned int &v2)//參數對調
{
	unsigned int tmp = v1;
	v1 = v2;
	v2 = tmp;
}
//-----------------------------------------------------------------------------//
void Swap(CString &v1, CString &v2)//參數對調
{
	CString tmp = v1;
	v1 = v2;
	v2 = tmp;
}
//-----------------------------------------------------------------------------//
void Swap(std::string &v1, std::string &v2)//參數對調
{
	std::string tmp = v1;
	v1 = v2;
	v2 = tmp;
}
//-----------------------------------------------------------------------------//
void Swap(std::wstring &v1, std::wstring &v2)//參數對調
{
	std::wstring tmp = v1;
	v1 = v2;
	v2 = tmp;
}
//-----------------------------------------------------------------------------//
void Swap(unsigned char *&v1, unsigned char *&v2)//參數對調
{
	unsigned char *tmp = v1;
	v1 = v2;
	v2 = tmp;
}
//-----------------------------------------------------------------------------//
bool CmpPoint_X_ASC(const POINT &v1, const POINT &v2)//比較函式-點X-遞增
{
	return (v1.x < v2.x);
}
//-----------------------------------------------------------------------------//
bool CmpPoint_X_DESC(const POINT &v1, const POINT &v2)//比較函式-點X-遞減
{
	return (v1.x > v2.x);
}
//-----------------------------------------------------------------------------//
bool CmpPoint_Y_ASC(const POINT &v1, const POINT &v2)//比較函式-點Y-遞增
{
	return (v1.y < v2.y);
}
//-----------------------------------------------------------------------------//
bool CmpPoint_Y_DESC(const POINT &v1, const POINT &v2)//比較函式-點Y-遞減
{
	return (v1.y > v2.y);
}
//-----------------------------------------------------------------------------//
bool CmpPoint2D_X(const TPOINT2D &v1, const TPOINT2D &v2)//比較函式-點X
{
	return (v1.x < v2.x);
}
//-----------------------------------------------------------------------------//
bool CmpPoint2D_Y(const TPOINT2D &v1, const TPOINT2D &v2)//比較函式-點Y
{
	return (v1.y < v2.y);
}
//-----------------------------------------------------------------------------//
bool CmpPoint3D_X(const TPOINT3D &v1, const TPOINT3D &v2)//比較函式-點X
{
	return (v1.x < v2.x);
}
//-----------------------------------------------------------------------------//
bool CmpPoint3D_Y(const TPOINT3D &v1, const TPOINT3D &v2)//比較函式-點Y
{
	return (v1.y < v2.y);
}
//-----------------------------------------------------------------------------//
bool CmpPoint3D_Z(const TPOINT3D &v1, const TPOINT3D &v2)//比較函式-點Z
{
	return (v1.z < v2.z);
}
//-----------------------------------------------------------------------------//
bool CmpRect_L(const RECT &v1, const RECT &v2)//比較函式-區域左
{
	return (v1.left < v2.left);
}
//-----------------------------------------------------------------------------//
bool CmpRect_T(const RECT &v1, const RECT &v2)//比較函式-區域上
{
	return (v1.top < v2.top);
}
//-----------------------------------------------------------------------------//
bool CmpRect_R(const RECT &v1, const RECT &v2)//比較函式-區域右
{
	return (v1.right < v2.right);
}
//-----------------------------------------------------------------------------//
bool CmpRect_B(const RECT &v1, const RECT &v2)//比較函式-區域下
{
	return (v1.bottom < v2.bottom);
}
//-----------------------------------------------------------------------------//
bool CmpRegion_MinX(const TREGION4D &v1, const TREGION4D &v2)//比較函式-區域小X
{
	return (v1.minX < v2.minX);
}
//-----------------------------------------------------------------------------//
bool CmpRegion_MinY(const TREGION4D &v1, const TREGION4D &v2)//比較函式-區域小Y
{
	return (v1.minY < v2.minY);
}
//-----------------------------------------------------------------------------//
bool CmpRegion_MaxX(const TREGION4D &v1, const TREGION4D &v2)//比較函式-區域大X
{
	return (v1.maxX < v2.maxX);
}
//-----------------------------------------------------------------------------//
bool CmpRegion_MaxY(const TREGION4D &v1, const TREGION4D &v2)//比較函式-區域大Y
{
	return (v1.maxY < v2.maxY);
}
//-----------------------------------------------------------------------------//
bool CheckLocked_INT(int &flag, int lockID, CRITICAL_SECTION &cs)//確認是否鎖住
{
	::EnterCriticalSection(&cs);
	if ( flag == lockID)
	{
		::LeaveCriticalSection(&cs);
		return true;
	}
	flag = lockID;
	::LeaveCriticalSection(&cs);
	return false;
}
//-----------------------------------------------------------------------------//
bool CheckLocked_BOOL(bool &locked, CRITICAL_SECTION &cs)//確認是否鎖住
{
	::EnterCriticalSection(&cs);
	if ( true == locked )
	{
		::LeaveCriticalSection(&cs);
		return true;
	}
	locked = true;
	::LeaveCriticalSection(&cs);
	return false;
}
//-----------------------------------------------------------------------------//
double Unit_MMtoUM(double mm)//Unit mm to um
{
	return (mm*1000.0);
}
//-----------------------------------------------------------------------------//
double Unit_UmtoMM(double um)//Unit um to um
{
	return (um/1000.0);
}
//-----------------------------------------------------------------------------//
int    ToInt(double val)//浮點數轉整數
{
	//return Floor(val);
	return Round(val);
}
//-----------------------------------------------------------------------------//
int    Round(double val)//四捨五入
{
	return std::round(val);	
#ifndef OPENCV_DISABLE
	return cvRound(val);//有小數誤差
#else
	return (int)(val+0.5);
#endif
}
//-----------------------------------------------------------------------------//
int    Ceil(double val)//無條件進入(2.1=>3)
{
	return std::ceil(val);
#ifndef OPENCV_DISABLE
	return cvCeil(val);
#else
	return ::ceil(val);
#endif
}
//-----------------------------------------------------------------------------//
int    Floor(double val)//無條件捨去(2.9=>2)
{
	return std::floor(val);
#ifndef OPENCV_DISABLE
	return cvFloor(val);
#else
	return ::floor(val);
#endif
}
//-----------------------------------------------------------------------------//
void AbsPoint(TPOINT2F &pt)//絕對值點座標
{
	pt.x = ::abs(pt.x);
	pt.y = ::abs(pt.y);	
}
//-----------------------------------------------------------------------------//
void AbsPoint(TPOINT2D &pt)//絕對值點座標
{
	pt.x = ::abs(pt.x);
	pt.y = ::abs(pt.y);	
}
//-----------------------------------------------------------------------------//
void AbsPoint(TPOINT3I &pt)//絕對值點座標
{
	pt.x = ::abs(pt.x);
	pt.y = ::abs(pt.y);
	pt.z = ::abs(pt.z);
}
//-----------------------------------------------------------------------------//
void AbsPoint(TPOINT3F &pt)//絕對值點座標
{
	pt.x = ::abs(pt.x);
	pt.y = ::abs(pt.y);
	pt.z = ::abs(pt.z);
}
//-----------------------------------------------------------------------------//
void AbsPoint(TPOINT3D &pt)//絕對值點座標
{
	pt.x = ::abs(pt.x);
	pt.y = ::abs(pt.y);
	pt.z = ::abs(pt.z);
}
//-----------------------------------------------------------------------------//
double CalcCos(double R, double L, double H)//餘弦定理
{
	double a = R;
	double b = L;
	double c = H;
	if ( fabs(a)<0.00001 || fabs(b)<0.00001 ) { return 0.0; }

	double t=((a*a)+(b*b)-(c*c))/(2*a*b);
	double r=::acos(t)*RAD_TO_DEG_DBL;
	return r;
}
//-----------------------------------------------------------------------------//
double CalcAngle2D(const TPOINT2D &Pt1, const TPOINT2D &Pt2)//計算2點角度
{
	double R = Pt2.x-Pt1.x;
	double H = Pt2.y-Pt1.y;
	return ::atan2(H, R)*RAD_TO_DEG_DBL;
}
//-----------------------------------------------------------------------------//
double CalcAngle3D(const TPOINT3D &Pt1, const TPOINT3D &Pt2)//計算2點角度
{
	double Rx = Pt2.x-Pt1.x;
	double Ry = Pt2.y-Pt1.y;
	double R = CalcDistance(Rx, Ry);
	double H = Pt2.z-Pt1.z;
	double L = CalcDistance(R, H);
	return ::atan2(H, R)*RAD_TO_DEG_DBL;	
}
//-----------------------------------------------------------------------------//
double  CalcDistance(double X, double Y)//計算距離
{
	return sqrt((X*X)+(Y*Y));
}
//-----------------------------------------------------------------------------//
double CalcDistance(const TPOINT2D &P1, const TPOINT2D &P2)//計算距離
{
	const double dx=P1.x-P2.x;
	const double dy=P1.y-P2.y;
	return sqrt((dx*dx)+(dy*dy));
}
//-----------------------------------------------------------------------------//
double  AdjustValue(double val, double divide)//調整數據為某個倍數
{
	double val_new = val;
	const double ratio = 100;
	const int nval = (int)(val*ratio);
	const int ndiv = (int)(divide*ratio);
	if ( ndiv == 0 ) { return val; }
	int nmulti = nval/ndiv;//倍數
	int nred   = nval%ndiv;//餘數
	if ( nmulti==0 || 0!=nred )
	{	nmulti += 1; }
	val_new = nmulti*ndiv;
	val_new /= ratio;
	return val_new;
}
//-----------------------------------------------------------------------------//
COLORREF  DivideColor(COLORREF clr, int nDivide)//顏色除半
{
	int R=GetRValue(clr);
	int G=GetGValue(clr);
	int B=GetBValue(clr);
	if ( nDivide > 1 )
	{
		R /= nDivide;
		G /= nDivide;
		B /= nDivide;
	}
	return RGB(R, G, B);
}
//-----------------------------------------------------------------------------//
bool GetTime(CString &Time,const CTime &NowTime)
{	
    //CString year, month, day, hour, min, sec;	
	//if ( JetAPI::GetTime(year, month, day, hour, min, sec, NowTime) == false ) { return false; }	
	//Time=year+month+day+hour+min+sec;
	Time = NowTime.Format(_T("%Y%m%d%H%M%S"));
	return true;
}
//-----------------------------------------------------------------------------//
bool GetTime(const char strDateTime[], CTime &NowTime)//取得時間
{	//20190115162831
	const size_t Len = ::strlen(strDateTime);
	if ( 14 != Len ) { return false; }

	size_t idx=0;
	char strYear[8]="0000";
	char strMonth[8]="00";
	char strDay[8]="00";
	char strHour[8]="00";
	char strMin[8]="00";
	char strSec[8]="00";

	strYear[0] = strDateTime[idx++];
	strYear[1] = strDateTime[idx++];
	strYear[2] = strDateTime[idx++];
	strYear[3] = strDateTime[idx++];
	strYear[4] = '\0';

	strMonth[0] = strDateTime[idx++];
	strMonth[1] = strDateTime[idx++];
	strMonth[2] = '\0';

	strDay[0] = strDateTime[idx++];
	strDay[1] = strDateTime[idx++];
	strDay[2] = '\0';

	strHour[0] = strDateTime[idx++];
	strHour[1] = strDateTime[idx++];
	strHour[2] = '\0';

	strMin[0] = strDateTime[idx++];
	strMin[1] = strDateTime[idx++];
	strMin[2] = '\0';

	strSec[0] = strDateTime[idx++];
	strSec[1] = strDateTime[idx++];
	strSec[2] = '\0';

	const int nYear = ::atoi(strYear);
	const int nMonth = ::atoi(strMonth);
	const int nDay = ::atoi(strDay);
	const int nHour = ::atoi(strHour);
	const int nMin = ::atoi(strMin);
	const int nSec = ::atoi(strSec);

	NowTime = CTime(nYear, nMonth, nDay, nHour, nMin, nSec);
	return true;
}
//-----------------------------------------------------------------------------//
bool GetDateTime(LPCTSTR DateTime, CString &Date, CString &Time)//取得日期時間
{
	const size_t Len = ::_tcslen(DateTime);
	if ( 14 != Len ) { return false; }
	Date.SetString(DateTime, 8);
	Time.SetString(&(DateTime[8]), 6);
	return true;
}
//-----------------------------------------------------------------------------//
bool GetTimeSpan(int nTotalMin, int &nDays, int &nHours, int &nMins)//取得時間跨距
{
	const int nDayMins = 1440;//1 day =1440 min
	const int nHourMins = 60;//1 hour=60 mins

	int nRemainHourMins = nTotalMin;//remain min for hours
	if ( nRemainHourMins < nDayMins )
	{	nDays = 0;	}
	else
	{			
		nDays = nTotalMin/nDayMins;//How many days
		nRemainHourMins = nTotalMin%nDayMins;//remain min for hours
	}

	int nRemainMins = nRemainHourMins;//remain min for mins
	if ( nRemainHourMins < nHourMins )
	{	nHours = 0;	}
	else
	{
		nHours = nRemainHourMins/nHourMins;//How many hours	
		nRemainMins = nRemainHourMins%nHourMins;//remain min for mins
	}

	nMins = nRemainMins;//How many mins	
	return true;
}
//-----------------------------------------------------------------------------//
bool GetTimeSpan(int nTotalSec, int &nDays, int &nHours, int &nMins, int &nSecs)//取得時間跨距
{	
	const int nDaySecs = 86400;//1 day =86400 secs
	const int nHourSecs = 3600;//1 hour=3600 secs
	const int nMinSecs = 60;//1 hour=60 secs

	int nRemainHourSecs = nTotalSec;//remain sec for hours
	if ( nRemainHourSecs < nDaySecs )
	{	nDays = 0;	}
	else
	{			
		nDays = nTotalSec/nDaySecs;//How many days
		nRemainHourSecs = nTotalSec%nDaySecs;//remain sec for hours
	}

	int nRemainMinSecs = nRemainHourSecs;//remain sec for mins
	if ( nRemainMinSecs < nHourSecs )
	{	nHours = 0;	}
	else
	{
		nHours = nRemainHourSecs/nHourSecs;//How many hours	
		nRemainMinSecs = nRemainHourSecs%nHourSecs;//remain sec for mins
	}

	int nRemainSecs = nRemainMinSecs;//remain sec
	if ( nRemainSecs < nMinSecs )
	{	nMins = 0;	}
	else
	{
		nMins = nRemainMinSecs/nMinSecs;//How many mins	
		nRemainSecs = nRemainMinSecs%nMinSecs;//remain sec for mins
	}
	
	nSecs = nRemainSecs;//How many sec
	return true;
}
//-----------------------------------------------------------------------------//
bool FormatTime(FORMAT_TIME Mode, const CTime &NowTime, CString &Time)//格式化時間
{
	switch ( Mode )
	{
	case FORMAT_TIME_01:
		Time = NowTime.Format(_T("%Y/%m/%d %H:%M:%S"));
		break;
	case FORMAT_TIME_HH_MM:
		Time = NowTime.Format(_T("%H:%M"));
		break;
	default:
		Time = NowTime.Format(_T("%Y%m%d%H%M%S"));
		break;
	}
	return true;
}
//-----------------------------------------------------------------------------//
bool GetTime(CString &year, CString &month, CString &day, CString &hour, CString &min, CString &sec,const CTime &NowTime)
{
	year.Format(_T("%i"), NowTime.GetYear());
	if ( NowTime.GetMonth()<10 )
	{	month.Format(_T("0%i"), NowTime.GetMonth());}
	else
	{	month.Format(_T("%i"), NowTime.GetMonth());}

	if ( NowTime.GetDay()<10 )
	{	day.Format(_T("0%i"), NowTime.GetDay());}
	else
	{	day.Format(_T("%i"), NowTime.GetDay());}

	if ( NowTime.GetHour()<10 )
	{	hour.Format(_T("0%i"), NowTime.GetHour());}
	else
	{	hour.Format(_T("%i"), NowTime.GetHour());}

	if ( NowTime.GetMinute()<10 )
	{	min.Format(_T("0%i"), NowTime.GetMinute());}
	else
	{	min.Format(_T("%i"), NowTime.GetMinute());}

	if ( NowTime.GetSecond()<10 )
	{	sec.Format(_T("0%i"), NowTime.GetSecond());}
	else
	{	sec.Format(_T("%i"), NowTime.GetSecond());}
	return true;
}
//-----------------------------------------------------------------------------//
bool FileTimeToTimet(const FILETIME &ft, time_t &time)//時間結構轉換
{
	ULARGE_INTEGER ull;
	ull.LowPart = ft.dwLowDateTime;
	ull.HighPart = ft.dwHighDateTime;
	time = (ull.QuadPart/10000000ULL)-(11644473600ULL);
	return true;
}
//-----------------------------------------------------------------------------//
bool TimetToFileTime(const time_t &time, FILETIME &ft)//時間結構轉換
{
	ULARGE_INTEGER ull;	
	ull.QuadPart = (time+11644473600ULL)*10000000ULL;	
	ft.dwLowDateTime = ull.LowPart;
	ft.dwHighDateTime = ull.HighPart;
	return true;
}
//-----------------------------------------------------------------------------//
bool TmToSystemTime(const tm &time, SYSTEMTIME &st)//時間結構轉換
{
	st.wYear = time.tm_year+1900;
	st.wMonth = time.tm_mon+1;
	st.wDay = time.tm_mday;
	st.wHour = time.tm_hour;
	st.wMinute = time.tm_min;
	st.wSecond = time.tm_sec;
	st.wMilliseconds = 0;	

	st.wDayOfWeek = time.tm_wday;
	return true;
}
//-----------------------------------------------------------------------------//
bool SystemTimeToTm(const SYSTEMTIME &st, tm &time)//時間結構轉換
{
	time.tm_year = st.wYear - 1900;
	time.tm_mon = st.wMonth - 1;
	time.tm_mday = st.wDay;
	time.tm_hour = st.wHour;
	time.tm_min = st.wMinute;
	time.tm_sec = st.wSecond;
	time.tm_isdst = -1;
	time.tm_wday = st.wDayOfWeek;
	return true;
}
//-----------------------------------------------------------------------------//
bool SetFuncTimeEnd(LARGE_INTEGER &End)//設定函式結束時間
{
	QueryPerformanceCounter(&End);	
	return true;
}
//-----------------------------------------------------------------------------//
bool SetFuncTimeStart(LARGE_INTEGER &Start)//設定函式起始時間	
{
	QueryPerformanceCounter(&Start);	
	return true;
}
//-----------------------------------------------------------------------------//
double CalcFuncTimeSpent(LARGE_INTEGER &Start, LARGE_INTEGER &End)//計算函式經過時間
{
	return (End.QuadPart - Start.QuadPart)*1000.0/AOIDataCollect.m_SystemFreq.QuadPart;	
}
//-----------------------------------------------------------------------------//
bool ExtractComponentBaseName(LPCTSTR str, CString &Name)//取零件基本名稱
{
	TCHAR Buffer[MAX_JET_PATH]=_T("");
	if ( ExtractComponentBaseName(str, Buffer, MAX_JET_PATH) == false )
	{	return false; }
	Name = Buffer;
	return true;
}
//-----------------------------------------------------------------------------//
bool ExtractComponentBaseName(LPCTSTR str, LPTSTR pName, size_t buffersize)//取零件基本名稱
{
#ifndef _UNICODE
	return ExtractComponentBaseNameA(str, pName, buffersize);
#else
	return ExtractComponentBaseNameW(str, pName, buffersize);
#endif//_UNICODE
	return false;
}
//-----------------------------------------------------------------------------//
bool ExtractComponentBaseNameA(LPCSTR str, LPSTR pName, size_t buffersize)//取零件基本名稱
{
	if ( NULL == str ) { return false; }
	size_t i=0, j=0;
	const size_t len = ::strlen(str);
	const size_t len2 = len-1;
	if ( len >= buffersize ) { return false; }	
	::memcpy(pName, str, sizeof(char)*(len+1));
	for ( i=0; i<len; i++ )
	{
		if ( '_' == pName[len-i-1] )
		{
			for ( j=len-i; j<len; j++ )
			{
				if ( pName[j]<'0' || pName[j]>'9' )
				{	break; }
			}
			if ( j == len )
			{	pName[len-i-1] = '\0';	}
			break;
		}
	}
	return true;
}
//-----------------------------------------------------------------------------//
bool ExtractComponentBaseNameW(LPCWSTR str, LPWSTR pName, size_t buffersize)//取零件基本名稱
{
	if ( NULL == str ) { return false; }
	size_t i=0, j=0;
	const size_t len = ::wcslen(str);
	if ( len >= buffersize ) { return false; }	
	::memcpy(pName, str, sizeof(wchar_t)*(len+1));
	for ( i=0; i<len; i++ )
	{
		if ( L'_' == pName[len-i-1] )
		{
			for ( j=len-i; j<len; j++ )
			{
				if ( pName[j]<L'0' || pName[j]>L'9' )
				{	break; }
			}
			if ( j == len )
			{	pName[len-i-1] = L'\0';	}			
			break;
		}
	}
	return true;
}
//-----------------------------------------------------------------------------//
bool ExtractComponentBaseNameIndex(LPCTSTR str, CString &Name, size_t &Idx)//取零件基本名稱與引數
{
	size_t BufferIdx=0;
	TCHAR Buffer[MAX_JET_PATH]=_T("");
	if ( ExtractComponentBaseNameIndex(str, Buffer, BufferIdx, MAX_JET_PATH) == false )
	{	return false; }
	Name = Buffer;	
	Idx = BufferIdx;
	return true;
}
//-----------------------------------------------------------------------------//
bool ExtractComponentBaseNameIndex(LPCTSTR str, LPTSTR pName, size_t &Idx, size_t buffersize)//取零件基本名稱與引數
{
#ifndef _UNICODE
	return ExtractComponentBaseNameIndexA(str, pName, Idx, buffersize);
#else
	return ExtractComponentBaseNameIndexW(str, pName, Idx, buffersize);
#endif//_UNICODE
	return false;	
}
//-----------------------------------------------------------------------------//
bool ExtractComponentBaseNameIndexA(LPCSTR str, LPSTR pName, size_t &Idx, size_t buffersize)//取零件基本名稱與引數
{
	if ( NULL == str ) { return false; }
	size_t i=0, j=0;
	const size_t len = ::strlen(str);
	const size_t len2 = len-1;
	if ( len >= buffersize ) { return false; }	
	Idx = 0;
	::memcpy(pName, str, sizeof(char)*(len+1));
	for ( i=0; i<len; i++ )
	{
		if ( '_' == pName[len-i-1] )
		{
			for ( j=len-i; j<len; j++ )
			{
				if ( pName[j]<'0' || pName[j]>'9' )
				{	break; }
			}
			if ( j == len )
			{	
				pName[len-i-1] = '\0';	

				char strIdx[64]="";
				const size_t IdxStart=len-i;				
				for ( size_t k=IdxStart; k<j; k++ )
				{	strIdx[k-IdxStart] = pName[k];	}
				strIdx[j-IdxStart] = '\0';
				Idx = ::atoi(strIdx);
			}
			break;
		}
	}	
	return true;
}
//-----------------------------------------------------------------------------//
bool ExtractComponentBaseNameIndexW(LPCWSTR str, LPWSTR pName, size_t &Idx, size_t buffersize)//取零件基本名稱與引數
{
	if ( NULL == str ) { return false; }
	size_t i=0, j=0;	
	const size_t len = ::wcslen(str);
	if ( len >= buffersize ) { return false; }	
	Idx = 0;
	::memcpy(pName, str, sizeof(wchar_t)*(len+1));
	for ( i=0; i<len; i++ )
	{
		if ( L'_' == pName[len-i-1] )
		{
			for ( j=len-i; j<len; j++ )
			{
				if ( pName[j]<L'0' || pName[j]>L'9' )
				{	break; }
			}
			if ( j == len )
			{	
				pName[len-i-1] = L'\0';	

				wchar_t strIdx[64]=L"";
				const size_t IdxStart=len-i;				
				for ( size_t k=IdxStart; k<j; k++ )
				{	strIdx[k-IdxStart] = pName[k];	}
				strIdx[j-IdxStart] = L'\0';
				Idx = ::_wtoi(strIdx);
			}
			break;
		}
	}
	return true;	
}
//-----------------------------------------------------------------------------//

bool ExtractFolder(LPCTSTR pfilename, CString &Folder)
{	
	TCHAR Buffer[MAX_JET_PATH]=_T("");
	if ( ExtractFolder(pfilename, Buffer, MAX_JET_PATH) == false )
	{	return false; }
	Folder = Buffer;
	return true;
}
//-----------------------------------------------------------------------------//
bool ExtractFolder(LPCTSTR pfilename, LPTSTR pFolder, size_t buffersize)
{
#ifndef _UNICODE
	return ExtractFolderA(pfilename, pFolder, buffersize);
#else
	return ExtractFolderW(pfilename, pFolder, buffersize);
#endif//_UNICODE
	return false;
}
//-----------------------------------------------------------------------------//
bool ExtractFolderA(LPCSTR pfilename, LPSTR pFolder, size_t buffersize)
{
	const size_t len = ::strlen(pfilename);
	if ( buffersize <= len ) 
	{	return false;	}

	size_t i=0;
	const char ch ='\\';
	::strcpy(pFolder, pfilename);
	for ( i=len; i!=-1; i-- )
	{
		if ( ch == pFolder[i] )
		{
			pFolder[i] = '\0';
			return true;
		}
	}
	pFolder[0] = '\0';	
	return false;
}
//-----------------------------------------------------------------------------//
bool ExtractFolderW(LPCWSTR pfilename, LPWSTR pFolder, size_t buffersize)
{
	const size_t len = ::wcslen(pfilename);
	if ( buffersize <= len ) 
	{	return false;	}

	size_t i=0;
	const wchar_t ch =L'\\';
	::wcscpy(pFolder, pfilename);
	for ( i=len; i!=-1; i-- )
	{
		if ( ch == pFolder[i] )
		{
			pFolder[i] = L'\0';
			return true;
		}
	}
	pFolder[0] = L'\0';	
	return false;
}
//-----------------------------------------------------------------------------//
bool ExtractExtendFileName(LPCTSTR pfilename, CString &ExtName)//取出副檔名
{
	TCHAR Buffer[MAX_JET_PATH]=_T("");
	if ( ExtractExtendFileName(pfilename, Buffer, MAX_JET_PATH) == false )
	{	return false; }
	ExtName = Buffer;
	return true;
}
//-----------------------------------------------------------------------------//
bool ExtractExtendFileName(LPCTSTR pfilename, LPTSTR pExtFileName, size_t buffersize)//取出副檔名
{
#ifndef _UNICODE
	return ExtractExtendFileNameA(pfilename, pExtFileName, buffersize);
#else
	return ExtractExtendFileNameW(pfilename, pExtFileName, buffersize);
#endif//_UNICODE
	return false;
}
//-----------------------------------------------------------------------------//
bool ExtractExtendFileNameA(LPCSTR pfilename, LPSTR pExtFileName, size_t buffersize)//取出副檔名
{
	pExtFileName[0] = '\0';
	const size_t len = ::strlen(pfilename);
	const char ch='.';
	size_t i=0;	
	for ( i=len; i!=-1; i-- )
	{
		if ( pfilename[i] == ch )
		{	break;}
	}
	if ( -1 == i ) 
	{	return false;	}

	if ( buffersize <= (len-i) ) 
	{	return false;	}

	size_t j=0;
	for ( j=i+1; j<len; j++ )
	{	pExtFileName[j-i-1] = pfilename[j];	}
	pExtFileName[j-i-1] = '\0';
	return true;
}
//-----------------------------------------------------------------------------//
bool ExtractExtendFileNameW(LPCWSTR pfilename, LPWSTR pExtFileName, size_t buffersize)//取出副檔名
{
	pExtFileName[0] = L'\0';
	const size_t len = ::wcslen(pfilename);
	const wchar_t ch = L'.';
	size_t i=0;	
	for ( i=len; i!=-1; i-- )
	{
		if ( pfilename[i] == ch )
		{	break;}
	}
	if ( -1 == i ) 
	{	return false;	}

	if ( buffersize <= (len-i) ) 
	{	return false;	}

	size_t j=0;
	for ( j=i+1; j<len; j++ )
	{	pExtFileName[j-i-1] = pfilename[j];	}
	pExtFileName[j-i-1] = L'\0';
	return true;
}
//-----------------------------------------------------------------------------//
bool ExtractMainFileName(LPCTSTR pfilename, CString &MainFileName)//取出主檔名	
{	
	//MainFileName = pfilename;
	//const int pos = MainFileName.ReverseFind(_T('.'));
	//if ( pos == 0 )
	//{	return true;	}
	//MainFileName = MainFileName.Left(pos);
	//return true;
	//*/

	TCHAR Buffer[MAX_JET_PATH]=_T("");
	if ( ExtractMainFileName(pfilename, Buffer, MAX_JET_PATH) == false )
	{	return false; }
	MainFileName = Buffer;
	return true;
}
//-----------------------------------------------------------------------------//
bool ExtractMainFileName(LPCTSTR pfilename, LPTSTR pMainFileName, size_t buffersize)//取出主檔名	
{
#ifndef _UNICODE
	return ExtractMainFileNameA(pfilename, pMainFileName, buffersize);
#else
	return ExtractMainFileNameW(pfilename, pMainFileName, buffersize);
#endif//_UNICODE
	return false;
}
//-----------------------------------------------------------------------------//
bool ExtractMainFileNameA(LPCSTR pfilename, LPSTR pMainFileName, size_t buffersize)//取出主檔名	
{	
	const size_t len = ::strlen(pfilename);
	if ( buffersize <= len ) 
	{	return false;	}
	const char ch='.';
	const char ch2='\\';
	size_t i=0;
	::strcpy(pMainFileName, pfilename);
	for ( i=len; i!=-1; i-- )
	{
		if ( ch2 == pMainFileName[i] )
		{	return true; }

		if ( ch == pMainFileName[i] )
		{
			pMainFileName[i] = '\0';
			return true;
		}
	}
	//沒有副檔名時
	return true;
}
//-----------------------------------------------------------------------------//
bool ExtractMainFileNameW(LPCWSTR pfilename, LPWSTR pMainFileName, size_t buffersize)//取出主檔名	
{	
	const size_t len = ::wcslen(pfilename);
	if ( buffersize <= len ) 
	{	return false;	}
	const wchar_t ch=L'.';
	const wchar_t ch2=L'\\';
	size_t i=0;
	::wcscpy(pMainFileName, pfilename);
	for ( i=len; i!=-1; i-- )
	{
		if ( ch2 == pMainFileName[i] )
		{	return true; }

		if ( ch == pMainFileName[i] )
		{
			pMainFileName[i] = L'\0';
			return true;
		}
	}
	//沒有副檔名時
	return true;
}
//-----------------------------------------------------------------------------//
bool ExtractMainFileNameNoPath(LPCTSTR pfilename, CString &MainFileName)//取出主檔名, 但不含路徑
{
	TCHAR Buffer[MAX_JET_PATH]=_T("");
	if ( ExtractMainFileNameNoPath(pfilename, Buffer, MAX_JET_PATH) == false )
	{	return false; }
	MainFileName = Buffer;
	return true;
}
//-----------------------------------------------------------------------------//
bool ExtractMainFileNameNoPath(LPCTSTR pfilename, LPTSTR pMainFileName, size_t buffersize)//取出主檔名, 但不含路徑
{
#ifndef _UNICODE
	return ExtractMainFileNameNoPathA(pfilename, pMainFileName, buffersize);
#else
	return ExtractMainFileNameNoPathW(pfilename, pMainFileName, buffersize);
#endif//_UNICODE
	return false;
}
//-----------------------------------------------------------------------------//
bool ExtractMainFileNameNoPathA(LPCSTR pfilename, LPSTR pMainFileName, size_t buffersize)//取出主檔名, 但不含路徑
{	
	char TempStr[MAX_JET_PATH] = "";	
	if ( JetAPI::ExtractMainFileNameA(pfilename, TempStr, MAX_JET_PATH) == false ) 
	{	return false;	}

	const char ch = '\\';
	size_t i=0;	
	const size_t len = ::strlen(TempStr);
	size_t StartI = -1;
	for ( i=len; i!=-1; i-- )
	{
		if ( ch == TempStr[i] )
		{	
			StartI = i;	
			break;
		}
	}
	if ( -1 == StartI ) 
	{	return false;	}

	for ( i=StartI+1; i<len; i++ )
	{	pMainFileName[i-StartI-1] = TempStr[i];	}
	pMainFileName[i-StartI-1] = '\0';	
	return true;
}
//-----------------------------------------------------------------------------//
bool ExtractMainFileNameNoPathW(LPCWSTR pfilename, LPWSTR pMainFileName, size_t buffersize)//取出主檔名, 但不含路徑
{
	wchar_t TempStr[MAX_JET_PATH] = L"";	
	if ( JetAPI::ExtractMainFileNameW(pfilename, TempStr, MAX_JET_PATH) == false ) 
	{	return false;	}

	const wchar_t ch = L'\\';
	size_t i=0;	
	const size_t len = ::wcslen(TempStr);
	size_t StartI = -1;
	for ( i=len; i!=-1; i-- )
	{
		if ( ch == TempStr[i] )
		{	
			StartI = i;	
			break;
		}
	}
	if ( -1 == StartI ) 
	{	return false;	}

	for ( i=StartI+1; i<len; i++ )
	{	pMainFileName[i-StartI-1] = TempStr[i];	}
	pMainFileName[i-StartI-1] = L'\0';	
	return true;
}
//-----------------------------------------------------------------------------//
bool  ExtractFileNameNoPath(LPCTSTR pfilename, CString &MainFileName)//取出檔名, 但不含路徑
{
	TCHAR Buffer[MAX_JET_PATH]=_T("");
	if ( ExtractFileNameNoPath(pfilename, Buffer, MAX_JET_PATH) == false )
	{	return false; }
	MainFileName = Buffer;
	return true;
}
//-----------------------------------------------------------------------------//
bool  ExtractFileNameNoPath(LPCTSTR pfilename, LPTSTR pMainFileName, size_t buffersize)//取出檔名, 但不含路徑
{
#ifndef _UNICODE
	return ExtractFileNameNoPathA(pfilename, pMainFileName, buffersize);
#else
	return ExtractFileNameNoPathW(pfilename, pMainFileName, buffersize);
#endif//_UNICODE
	return false;
}
//-----------------------------------------------------------------------------//
bool  ExtractFileNameNoPathA(LPCSTR pfilename, LPSTR pMainFileName, size_t buffersize)//取出檔名, 但不含路徑
{
	const size_t len = ::strlen(pfilename);
	if ( buffersize <= len ) 
	{	return false;	}

	char TempStr[MAX_JET_PATH] = "";	
	const char ch='\\';
	size_t i=0;
	size_t StartI = -1;
	::strcpy(TempStr, pfilename);
	for ( i=len; i!=-1; i-- )
	{
		if ( ch == TempStr[i] )
		{	
			StartI = i;	
			break;
		}
	}
	if ( -1 == StartI ) 
	{	return false;	}

	for ( i=StartI+1; i<len; i++ )
	{	pMainFileName[i-StartI-1] = TempStr[i];	}
	pMainFileName[i-StartI-1] = '\0';		
	return true;
}
//-----------------------------------------------------------------------------//
bool  ExtractFileNameNoPathW(LPCWSTR pfilename, LPWSTR pMainFileName, size_t buffersize)//取出檔名, 但不含路徑
{
	const size_t len = ::wcslen(pfilename);
	if ( buffersize <= len ) 
	{	return false;	}

	wchar_t TempStr[MAX_JET_PATH] = L"";	
	const wchar_t ch = L'\\';
	size_t i=0;
	size_t StartI = -1;
	::wcscpy(TempStr, pfilename);
	for ( i=len; i!=-1; i-- )
	{
		if ( ch == TempStr[i] )
		{	
			StartI = i;	
			break;
		}
	}
	if ( -1 == StartI ) 
	{	return false;	}

	for ( i=StartI+1; i<len; i++ )
	{	pMainFileName[i-StartI-1] = TempStr[i];	}
	pMainFileName[i-StartI-1] = L'\0';		
	return true;
}
//-----------------------------------------------------------------------------//
bool ExtractLastPath(LPCTSTR pfilename, CString &LastPath)//取出上一層檔名
{
	TCHAR Buffer[MAX_JET_PATH]=_T("");
	if ( ExtractLastPath(pfilename, Buffer, MAX_JET_PATH) == false )
	{	return false; }
	LastPath = Buffer;
	return true;
}
//-----------------------------------------------------------------------------//
bool ExtractLastPath(LPCTSTR pfilename, LPTSTR pLastPath, size_t buffersize)//取出上一層檔名
{
#ifndef _UNICODE
	return ExtractLastPathA(pfilename, pLastPath, buffersize);
#else
	return ExtractLastPathW(pfilename, pLastPath, buffersize);
#endif//_UNICODE
	return false;
}
//-----------------------------------------------------------------------------//
bool ExtractLastPathA(LPCSTR pfilename, LPSTR pLastPath, size_t buffersize)//取出上一層檔名
{
	const size_t len = ::strlen(pfilename);
	if ( len >= (buffersize-1) ) { return false; }

	size_t i=0;
	::strcpy(pLastPath, pfilename);
	for ( i=len; i!=-1; i-- )
	{
		if ( '\\' == pLastPath[i] ) 
		{
			pLastPath[i] = '\0';
			break;
		}
	}
	return true;
}
//-----------------------------------------------------------------------------//
bool ExtractLastPathW(LPCWSTR pfilename, LPWSTR pLastPath, size_t buffersize)//取出上一層檔名
{
	const size_t len = ::wcslen(pfilename);
	if ( len >= (buffersize-1) ) { return false; }

	size_t i=0;
	::wcscpy(pLastPath, pfilename);
	for ( i=len; i!=-1; i-- )
	{
		if ( L'\\' == pLastPath[i] ) 
		{
			pLastPath[i] = L'\0';
			break;
		}
	}
	return true;
}
//-----------------------------------------------------------------------------//
bool ExtractTopFolder(LPCTSTR pPath, CString &TopFolder)//取出上層資料夾 
{
	TCHAR Buffer[MAX_JET_PATH]=_T("");
	if ( ExtractTopFolder(pPath, Buffer, MAX_JET_PATH) == false )
	{	return false; }
	TopFolder = Buffer;
	return true;	
}
//-----------------------------------------------------------------------------//
bool ExtractTopFolder(LPCTSTR pPath, LPTSTR pTopFolder, size_t buffersize)//取出上層資料夾 
{
#ifndef _UNICODE
	return ExtractTopFolderA(pPath, pTopFolder, buffersize);
#else
	return ExtractTopFolderW(pPath, pTopFolder, buffersize);
#endif//_UNICODE
	return false;
}
//-----------------------------------------------------------------------------//
bool ExtractTopFolderA(LPCSTR pPath, LPSTR pTopFolder, size_t buffersize)//取出上層資料夾 
{
	const size_t len = ::strlen(pPath);
	if ( len >= (buffersize-1) ) { return false; }

	size_t i=0, j=0;
	size_t StartI=-1;	
	pTopFolder[0]='\0';
	for ( i=len; i!=-1; i-- )
	{
		if ( '\\' == pPath[i] ) 
		{
			StartI = i;
			break;
		}
	}
	if ( -1 == StartI )
	{	return false; }

	j=0;
	for ( i=StartI+1; i<len; i++ )
	{	pTopFolder[j++]=pPath[i];	}
	pTopFolder[j++] = '\0';
	return true;
}
//-----------------------------------------------------------------------------//
bool ExtractTopFolderW(LPCWSTR pPath, LPWSTR pTopFolder, size_t buffersize)//取出上層資料夾 
{
	const size_t len = ::wcslen(pPath);
	if ( len >= (buffersize-1) ) { return false; }

	size_t i=0, j=0;
	size_t StartI=-1;	
	pTopFolder[0]=L'\0';
	for ( i=len; i!=-1; i-- )
	{
		if ( L'\\' == pPath[i] ) 
		{
			StartI = i;
			break;
		}
	}
	if ( -1 == StartI )
	{	return false; }

	j=0;
	for ( i=StartI+1; i<len; i++ )
	{	pTopFolder[j++]=pPath[i];	}
	pTopFolder[j++] = L'\0';
	return true;
}
//-----------------------------------------------------------------------------//
bool ExtractShortFilename(LPCTSTR pFolder, LPCTSTR pfilename, CString &ShortName)//取出不含底部資料夾的短檔名 
{
	TCHAR Buffer[MAX_JET_PATH]=_T("");
	if ( ExtractShortFilename(pFolder, pfilename, Buffer, MAX_JET_PATH) == false )
	{	return false; }
	ShortName = Buffer;	
	return true;
}
//-----------------------------------------------------------------------------//
bool ExtractShortFilename(LPCTSTR pFolder, LPCTSTR pfilename, LPTSTR pShortName, size_t buffersize)//取出不含底部資料夾的短檔名 
{
#ifndef _UNICODE
	return ExtractShortFilenameA(pFolder, pfilename, pShortName, buffersize);
#else
	return ExtractShortFilenameW(pFolder, pfilename, pShortName, buffersize);
#endif//_UNICODE
	return false;	
}
//-----------------------------------------------------------------------------//
bool ExtractShortFilenameA(LPCSTR pFolder, LPCSTR pfilename, LPSTR pShortName, size_t buffersize)//取出不含底部資料夾的短檔名 
{
	if ( NULL==pFolder || NULL==pfilename || NULL==pShortName ) { return false; }

	size_t       i=0, j=0;
	const size_t TmpSize=MAX_JET_PATH;
	char         Folder[TmpSize]="";
	char         filename[TmpSize]="";
	const size_t FolderLen = ::strlen(pFolder);
	const size_t FilenameLen = ::strlen(pfilename);
	if ( TmpSize < FilenameLen ) { return false; }
	if ( FilenameLen <= FolderLen ) { return false; }
	if ( buffersize < FilenameLen ) { return false; }	

	::strcpy(Folder, pFolder);
	::strcpy(filename, pfilename);
	::_strupr(Folder);
	::_strupr(filename);
	
	for ( i=0; i<FolderLen; i++ )
	{
		if ( Folder[i] != filename[i] )
		{	return false; }
	}

	j=0;
	for ( i=FolderLen+1; i<FilenameLen; i++ )
	{	pShortName[j++] = pfilename[i];	}
	pShortName[j++] = '\0';
	return true;
}
//-----------------------------------------------------------------------------//
bool ExtractShortFilenameW(LPCWSTR pFolder, LPCWSTR pfilename, LPWSTR pShortName, size_t buffersize)//取出不含底部資料夾的短檔名 
{
	if ( NULL==pFolder || NULL==pfilename || NULL==pShortName ) { return false; }

	size_t       i=0, j=0;
	const size_t TmpSize=MAX_JET_PATH;
	wchar_t      Folder[TmpSize]=L"";
	wchar_t      filename[TmpSize]=L"";
	const size_t FolderLen = ::wcslen(pFolder);
	const size_t FilenameLen = ::wcslen(pfilename);
	if ( TmpSize < FilenameLen ) { return false; }
	if ( FilenameLen <= FolderLen ) { return false; }
	if ( buffersize < FilenameLen ) { return false; }	

	::wcscpy(Folder, pFolder);
	::wcscpy(filename, pfilename);
	::wcsupr(Folder);
	::wcsupr(filename);
	
	for ( i=0; i<FolderLen; i++ )
	{
		if ( Folder[i] != filename[i] )
		{	return false; }
	}

	j=0;
	for ( i=FolderLen+1; i<FilenameLen; i++ )
	{	pShortName[j++] = pfilename[i];	}
	pShortName[j++] = L'\0';
	return true;
}
//-----------------------------------------------------------------------------//
bool  ExtractIPAddress(LPCSTR str, BYTE &nField0, BYTE &nField1, BYTE &nField2, BYTE &nField3)//從字串取得IP網址
{
	if ( NULL == str ) { return false; }

	int   i=0, j=0, Cnt=0;
	char  Buffer[MAX_JET_PATH]="";
	const char   ch = '.';
	const size_t len = ::strlen(str);
	for ( i=0; i<len; i++ )
	{
		if ( ch == str[i] )
		{
			Buffer[j] = '\0';
			j = 0;

			switch ( Cnt ) 
			{
			case 0:	nField0 = ::atoi(Buffer);	break;
			case 1:	nField1 = ::atoi(Buffer);	break;
			case 2:	nField2 = ::atoi(Buffer);	break;
			case 3:	nField3 = ::atoi(Buffer);	break;
			}
			Cnt ++;
			continue;
		}
		Buffer[j] = str[i];
		j ++;
	}
	Buffer[j] = '\0';
	if ( Cnt < 3 ) { return false; }
	nField3 = ::atoi(Buffer);
	Cnt ++;
	return true;
}
//-----------------------------------------------------------------------------//
bool  ExtractIPAddress(LPCWSTR str, BYTE &nField0, BYTE &nField1, BYTE &nField2, BYTE &nField3)//從字串取得IP網址
{
	int      i=0, j=0, Cnt=0;
	wchar_t  Buffer[MAX_JET_PATH]=L"";
	const wchar_t   ch = L'.';
	const size_t len = ::wcslen(str);
	for ( i=0; i<len; i++ )
	{
		if ( ch == str[i] )
		{
			Buffer[j] = L'\0';
			j = 0;

			switch ( Cnt ) 
			{
			case 0:	nField0 = ::_wtoi(Buffer);	break;
			case 1:	nField1 = ::_wtoi(Buffer);	break;
			case 2:	nField2 = ::_wtoi(Buffer);	break;
			case 3:	nField3 = ::_wtoi(Buffer);	break;
			}
			Cnt ++;
			continue;
		}
		Buffer[j] = str[i];
		j ++;
	}
	Buffer[j] = L'\0';
	if ( Cnt < 3 ) { return false; }
	nField3 = ::_wtoi(Buffer);
	Cnt ++;
	return true;
}
//-----------------------------------------------------------------------------//
BOOL ClearListBox(CListBox &ListBox)//清除列表盒元件內容
{
	if ( ListBox.GetSafeHwnd() == NULL ) { return TRUE; }

	int       i=0;
	const int Count=ListBox.GetCount();
	for ( i=0; i<Count; i++ )
	{	ListBox.DeleteString(Count-i-1);	}	
	return TRUE;
}
//-----------------------------------------------------------------------------//
int GetEnsureVisibleIndex(int ItemIndex, int ItemCount)//取得確保顯示的項目引數
{
	const int MaxIndex = ItemCount-1;
	if ( MaxIndex < ItemIndex )
	{	return MaxIndex; }

	const int DifCount = MaxIndex-ItemIndex;
	const int ExtraIndex = 3;
	if ( DifCount > ExtraIndex )
	{	return ItemIndex+ExtraIndex; }
	return MaxIndex;	
}
//-----------------------------------------------------------------------------//
BOOL InitialListCtrl(CListCtrl &ListCtrl)
{
	if ( ListCtrl.GetSafeHwnd() == NULL ) { return FALSE; }

	DWORD dwStyle = (DWORD)::SendMessage(ListCtrl.GetSafeHwnd(),LVM_GETEXTENDEDLISTVIEWSTYLE,0,0);
    dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_GRIDLINES;	
	//dwStyle |= LVS_EX_CHECKBOXES;
    ::SendMessage(ListCtrl.GetSafeHwnd(),LVM_SETEXTENDEDLISTVIEWSTYLE,0,dwStyle);	

	//ListCtrl.ModifyStyle(0, LVS_SHOWSELALWAYS);	//ModifyStyle(Remove, Add)
	return TRUE;
}
//-----------------------------------------------------------------------------//
BOOL ClearListCtrl(CListCtrl &ListCtrl, BOOL ClearHeader)//清除列表控制元件內容
{
	if ( ListCtrl.GetSafeHwnd() == NULL ) { return FALSE; }
	
	ListCtrl.DeleteAllItems();	
	if ( TRUE == ClearHeader )
	{	ClearListCtrlHeaderList(ListCtrl);	}	
	return TRUE;
}
//-----------------------------------------------------------------------------//
BOOL ClearListCtrlHeaderList(CListCtrl &ListCtrl)//移除列表物件的標頭
{
	if ( ListCtrl.GetSafeHwnd() == NULL ) { return FALSE; }
	CHeaderCtrl *HeaderPtr = ListCtrl.GetHeaderCtrl();
	if ( NULL == HeaderPtr ) { return TRUE; }	
	int i=0;
	int nCount = HeaderPtr->GetItemCount();
	for ( i=nCount-1; i>=0; i-- )
	{	ListCtrl.DeleteColumn(i); }
	return TRUE;	
}
//-----------------------------------------------------------------------------//
BOOL  ClearListCtrl(CJETListCtrl &ListCtrl, BOOL ClearHeader)//清除列表控制元件內容
{
	if ( ListCtrl.GetSafeHwnd() == NULL ) { return FALSE; }
	
	ListCtrl.DeleteAllItems();	
	if ( TRUE == ClearHeader )
	{	ClearListCtrlHeaderList(ListCtrl);	}	
	return TRUE;
}
//-----------------------------------------------------------------------------//
BOOL ClearListCtrl(CJETMFCListCtrl &ListCtrl, BOOL ClearHeader)//清除列表控制元件內容				
{
	if ( ListCtrl.GetSafeHwnd() == NULL ) { return FALSE; }
	
	ListCtrl.DeleteAllItems();	
	if ( TRUE == ClearHeader )
	{	ClearListCtrlHeaderList(ListCtrl);	}	
	return TRUE;
}
//-----------------------------------------------------------------------------//
int GetListCtrlItemByData(CListCtrl &ListCtrl, DWORD_PTR Data)//列表物件
{
	if ( ListCtrl.GetSafeHwnd() == NULL ) { return -1; }	
	int i=0;
	int nCount = ListCtrl.GetItemCount();
	for ( i=0; i<nCount; i++ )
	{
		if ( ListCtrl.GetItemData(i) != Data)
		{	continue; }
		return i;
	}	
	return -1;
}
//-----------------------------------------------------------------------------//
bool SaveListCtrl(LPCTSTR pfilename, CListCtrl &ListCtrl)//儲存列表控制元件內容
{
	CHeaderCtrl *pHeaderCtrl = ListCtrl.GetHeaderCtrl();
	if ( NULL == pHeaderCtrl ) { return false; }
	const int ColCount = pHeaderCtrl->GetItemCount();
	if ( 0 == ColCount ) { return false; }
	const int ItemCount = ListCtrl.GetItemCount();

	TCHAR   TMode[32] = _T("");
	_tcscpy(TMode, _T("w+"));
	JetAPI::ModifyOpenFileMode_Write(TMode);
	FILE *pfile = ::_tfopen(pfilename, TMode);
	if ( NULL == pfile )
	{	return false;	}
	
	HDITEM hdItem;
	bool bFirst = true;
	const int szTextBuffer=256;
	TCHAR TextBuffer[szTextBuffer];	
	::memset(&hdItem, 0x00, sizeof(hdItem));
	::memset(TextBuffer, 0x00, sizeof(TextBuffer));

	hdItem.mask = HDI_TEXT;
	hdItem.pszText = TextBuffer;
	hdItem.cchTextMax = szTextBuffer;
	for ( int i=0; i<ColCount; i++ )
	{
		if ( pHeaderCtrl->GetItem(i, &hdItem) == FALSE ) { continue; }
		if ( bFirst )
		{	::_ftprintf(pfile, _T("%s"), hdItem.pszText);	}
		else
		{	::_ftprintf(pfile, _T(", %s"), hdItem.pszText);	}
		bFirst = false;
	}
	::_ftprintf(pfile, _T("\n"));

	for ( int j=0; j<ItemCount; j++ )
	{
		bFirst = true;
		for ( int i=0; i<ColCount; i++ )
		{
			CString ItemText = ListCtrl.GetItemText(j, i);
			if ( bFirst )
			{	::_ftprintf(pfile, _T("%s"), ItemText);	}
			else
			{	::_ftprintf(pfile, _T(", %s"), ItemText);	}
			bFirst = false;
		}
		::_ftprintf(pfile, _T("\n"));		
	}
	::fclose(pfile);
	return true;
}
//-----------------------------------------------------------------------------//
#if FRAME_STYLE_TYPE != FRAME_STYLE_MFC
BOOL InitialListCtrl(CMFCListCtrl &ListCtrl)//列表物件的初始化
{
	if ( ListCtrl.GetSafeHwnd() == NULL ) { return FALSE; }
	DWORD dwStyle = (DWORD)::SendMessage(ListCtrl.GetSafeHwnd(),LVM_GETEXTENDEDLISTVIEWSTYLE,0,0);
    dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_GRIDLINES;
	//dwStyle |= LVS_EX_CHECKBOXES;
    ::SendMessage(ListCtrl.GetSafeHwnd(),LVM_SETEXTENDEDLISTVIEWSTYLE,0,dwStyle);	

	ListCtrl.ModifyStyle(0, LVS_SHOWSELALWAYS);	//ModifyStyle(Remove, Add)
	return TRUE;
}
//-----------------------------------------------------------------------------//
BOOL ClearListCtrl(CMFCListCtrl &ListCtrl, BOOL ClearHeader)//清除列表控制元件內容
{
	if ( ListCtrl.GetSafeHwnd() == NULL ) { return FALSE; }	
	ListCtrl.DeleteAllItems();	
	if ( TRUE == ClearHeader )
	{	ClearListCtrlHeaderList(ListCtrl);	}	
	return TRUE;
}
//-----------------------------------------------------------------------------//
BOOL ClearListCtrlHeaderList(CMFCListCtrl &ListCtrl)//移除列表物件的標頭
{
	if ( ListCtrl.GetSafeHwnd() == NULL ) { return FALSE; }
	CMFCHeaderCtrl &HeaderCtrl = ListCtrl.GetHeaderCtrl();	
	int i=0;
	int nCount = HeaderCtrl.GetItemCount();
	for ( i=nCount-1; i>=0; i-- )
	{
		ListCtrl.DeleteColumn(i);
		//HeaderCtrl.DeleteItem(i); 
	}	
	return TRUE;
}
//-----------------------------------------------------------------------------//
int GetListCtrlItemByData(CMFCListCtrl &ListCtrl, DWORD_PTR Data)//列表物件
{
	if ( ListCtrl.GetSafeHwnd() == NULL ) { return -1; }	
	int i=0;
	int nCount = ListCtrl.GetItemCount();
	for ( i=0; i<nCount; i++ )
	{
		if ( ListCtrl.GetItemData(i) != Data)
		{	continue; }
		return i;
	}	
	return -1;
}
//-----------------------------------------------------------------------------//
#endif//FRAME_STYLE_TYPE
bool IsFolderExist(LPCTSTR  Folder)
{
	if ( NULL == Folder ) { return false; }

	size_t len = _tcslen(Folder);	
	CString filename;
	filename.Format(_T("%s\\a.txt"), Folder);	
	FILE *pfile = NULL;
	pfile = ::_tfopen(filename, _T("w+"));
	if ( pfile == NULL ) { return false; }
	::fclose(pfile);	 
	::DeleteFile(filename);	
	return true;
}
//----------------------------------------------------------------------------//
bool IsDiskDrive(LPCTSTR Folder)//是否磁碟機
{
	if ( NULL == Folder ) { return false; }
	const size_t len = _tcslen(Folder);
	if ( len > 2 ) { return false; }

	if ( Folder[0]<'a' || Folder[0]>'z' )
	{	return true; }
	if ( Folder[0]<'A' || Folder[0]>'Z' )
	{	return true; }
	return false;
}
//----------------------------------------------------------------------------//
bool EnumRS232(std::vector<int> &List)//列舉RS232
{
	// Create a device information set that will be the container for 
	// the device interfaces.
	GUID *guidDev = (GUID*) &GUID_CLASS_COMPORT;
	HDEVINFO hDevInfo = INVALID_HANDLE_VALUE;
	SP_DEVICE_INTERFACE_DETAIL_DATA *pDetData=NULL;

	List.clear();
	hDevInfo = SetupDiGetClassDevs(guidDev, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE );
	if ( INVALID_HANDLE_VALUE == hDevInfo) 
	{	return false;	}//GetLastError

	// Enumerate the serial ports
	CString str, str2;
	BOOL bOk = TRUE;
	DWORD err = 0;
	WCHAR desc[256] = {0};
	WCHAR fname[256] = {0};	
	WCHAR fclass[256] = {0};	
	WCHAR locinfo[256] = {0};	
	SP_DEVICE_INTERFACE_DATA ifcData;
	DWORD dwDetDataSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA) + 256;
	pDetData = (SP_DEVICE_INTERFACE_DETAIL_DATA*)new char[dwDetDataSize];
	if ( NULL == pDetData)
	{	return false;	}

	// This is required, according to the documentation. Yes,
	// it's weird.
	ifcData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
	pDetData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
	for (DWORD ii=0; bOk; ii++) 
	{
		bOk = SetupDiEnumDeviceInterfaces(hDevInfo,NULL, guidDev, ii, &ifcData);
		err = GetLastError();
		if ( ERROR_NO_MORE_ITEMS == err )
		{	break; }
		if ( FALSE == bOk )//GetLastError
		{				
			if ( NULL != pDetData)			
			{	delete [] (char*)pDetData;	}

			if ( INVALID_HANDLE_VALUE != hDevInfo )			
			{	SetupDiDestroyDeviceInfoList(hDevInfo);	}
			return false;
		}
		
		// Got a device. Get the details.
		SP_DEVINFO_DATA devdata = {sizeof(SP_DEVINFO_DATA)};
		bOk = SetupDiGetDeviceInterfaceDetail(hDevInfo, &ifcData, pDetData, dwDetDataSize, NULL, &devdata);
		if ( FALSE == bOk )
		{
			DWORD err = GetLastError();
			//if (err != ERROR_NO_MORE_ITEMS) 
			if ( NULL != pDetData)			
			{	delete [] (char*)pDetData;	}

			if ( INVALID_HANDLE_VALUE != hDevInfo )			
			{	SetupDiDestroyDeviceInfoList(hDevInfo);	}
			return false;
		}
		
		// Got a path to the device. Try to get some more info.
		::memset(desc, 0x00, sizeof(desc));
		::memset(fname, 0x00, sizeof(fname));
		::memset(fclass, 0x00, sizeof(fclass));
		::memset(locinfo, 0x00, sizeof(locinfo));
		
		BOOL bUsbDevice = FALSE;
		BOOL bSuccess = SetupDiGetDeviceRegistryProperty(hDevInfo, &devdata, SPDRP_FRIENDLYNAME, NULL, (PBYTE)fname, sizeof(fname), NULL);
		SetupDiGetDeviceRegistryProperty(hDevInfo, &devdata, SPDRP_CLASS, NULL, (PBYTE)fclass, sizeof(fclass), NULL);
		bSuccess = bSuccess && SetupDiGetDeviceRegistryProperty(hDevInfo, &devdata, SPDRP_DEVICEDESC, NULL,	(PBYTE)desc, sizeof(desc), NULL);	
		if (SetupDiGetDeviceRegistryProperty(hDevInfo, &devdata, SPDRP_LOCATION_INFORMATION, NULL,(PBYTE)locinfo, sizeof(locinfo), NULL))
		{
			// Just check the first three characters to determine
			// if the port is connected to the USB bus. This isn't
			// an infallible method; it would be better to use the
			// BUS GUID. Currently, Windows doesn't let you query
			// that though (SPDRP_BUSTYPEGUID seems to exist in
			// documentation only).
			bUsbDevice = (wcsncmp(locinfo,L"USB", 3)==0);
		}
		if (bSuccess)
		{	
			//printf("FriendlyName = %S/r/n",fname);
			//printf("Port Desc = %S/r/n",desc);

			str = fname;
			str.MakeLower();
			int Begin=str.Find(_T("(com"));
			int End=str.Find(_T(")"), Begin);			
			if ( -1==Begin || -1==End )
			{	str2= _T("");	}
			else
			{
				int Start=Begin+4;
				if ( End > Start )
				{	str2 = str.Mid(Start, End-Start); }
				else
				{	str2= _T(""); }
			}
			if ( str2.GetLength() > 0 )
			{	List.push_back(::_ttoi(str2));	}			
		}		
	}		

	if ( NULL != pDetData)			
	{	delete [] (char*)pDetData;	}
	if ( INVALID_HANDLE_VALUE != hDevInfo )			
	{	SetupDiDestroyDeviceInfoList(hDevInfo);	}	
	return true;
}
int FindUsbComPort(CString keyword, int &port)
{
	keyword.MakeLower();
	GUID *guidDev = (GUID*)&GUID_CLASS_COMPORT;
	HDEVINFO hDevInfo = INVALID_HANDLE_VALUE;
	SP_DEVICE_INTERFACE_DETAIL_DATA *pDetData = NULL;

	hDevInfo = SetupDiGetClassDevs(guidDev, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
	if (INVALID_HANDLE_VALUE == hDevInfo)
	{
		return false;
	}//GetLastError

	 // Enumerate the serial ports
	CString str, str2;
	BOOL bOk = TRUE;
	DWORD err = 0;
	WCHAR desc[256] = { 0 };
	WCHAR fname[256] = { 0 };
	WCHAR fclass[256] = { 0 };
	WCHAR locinfo[256] = { 0 };
	SP_DEVICE_INTERFACE_DATA ifcData;
	DWORD dwDetDataSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA) + 256;
	pDetData = (SP_DEVICE_INTERFACE_DETAIL_DATA*)new char[dwDetDataSize];
	if (NULL == pDetData)
	{
		return false;
	}

	// This is required, according to the documentation. Yes,
	// it's weird.
	ifcData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
	pDetData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
	for (DWORD ii = 0; bOk; ii++)
	{
		bOk = SetupDiEnumDeviceInterfaces(hDevInfo, NULL, guidDev, ii, &ifcData);
		err = GetLastError();
		if (ERROR_NO_MORE_ITEMS == err)
		{
			break;
		}
		if (FALSE == bOk)//GetLastError
		{
			if (NULL != pDetData)
			{
				delete[](char*)pDetData;
			}

			if (INVALID_HANDLE_VALUE != hDevInfo)
			{
				SetupDiDestroyDeviceInfoList(hDevInfo);
			}
			return false;
		}

		// Got a device. Get the details.
		SP_DEVINFO_DATA devdata = { sizeof(SP_DEVINFO_DATA) };
		bOk = SetupDiGetDeviceInterfaceDetail(hDevInfo, &ifcData, pDetData, dwDetDataSize, NULL, &devdata);
		if (FALSE == bOk)
		{
			DWORD err = GetLastError();
			//if (err != ERROR_NO_MORE_ITEMS) 
			if (NULL != pDetData)
			{
				delete[](char*)pDetData;
			}

			if (INVALID_HANDLE_VALUE != hDevInfo)
			{
				SetupDiDestroyDeviceInfoList(hDevInfo);
			}
			return false;
		}

		// Got a path to the device. Try to get some more info.
		::memset(desc, 0x00, sizeof(desc));
		::memset(fname, 0x00, sizeof(fname));
		::memset(fclass, 0x00, sizeof(fclass));
		::memset(locinfo, 0x00, sizeof(locinfo));

		BOOL bUsbDevice = FALSE;
		BOOL bSuccess = SetupDiGetDeviceRegistryProperty(hDevInfo, &devdata, SPDRP_FRIENDLYNAME, NULL, (PBYTE)fname, sizeof(fname), NULL);
		SetupDiGetDeviceRegistryProperty(hDevInfo, &devdata, SPDRP_CLASS, NULL, (PBYTE)fclass, sizeof(fclass), NULL);
		bSuccess = bSuccess && SetupDiGetDeviceRegistryProperty(hDevInfo, &devdata, SPDRP_DEVICEDESC, NULL, (PBYTE)desc, sizeof(desc), NULL);
		if (SetupDiGetDeviceRegistryProperty(hDevInfo, &devdata, SPDRP_LOCATION_INFORMATION, NULL, (PBYTE)locinfo, sizeof(locinfo), NULL))
		{
			// Just check the first three characters to determine
			// if the port is connected to the USB bus. This isn't
			// an infallible method; it would be better to use the
			// BUS GUID. Currently, Windows doesn't let you query
			// that though (SPDRP_BUSTYPEGUID seems to exist in
			// documentation only).
			bUsbDevice = (wcsncmp(locinfo, L"USB", 3) == 0);
		}
		if (bSuccess)
		{
			//printf("FriendlyName = %S/r/n",fname);
			//printf("Port Desc = %S/r/n",desc);

			str = fname;
			str.MakeLower();
			int Findkeyword = str.Find(keyword);
			if (Findkeyword < 0) { continue; }
			int Begin = str.Find(_T("(com"));
			int End = str.Find(_T(")"), Begin);
			if (-1 == Begin || -1 == End)
			{
				str2 = _T("");
			}
			else
			{
				int Start = Begin + 4;
				if (End > Start)
				{
					str2 = str.Mid(Start, End - Start);
				}
				else
				{
					str2 = _T("");
				}
			}
			if (str2.GetLength() > 0)
			{
				port = ::_ttoi(str2);
				return true;
			}
		}
	}

	if (NULL != pDetData)
	{
		delete[](char*)pDetData;
	}
	if (INVALID_HANDLE_VALUE != hDevInfo)
	{
		SetupDiDestroyDeviceInfoList(hDevInfo);
	}
	return false;
}
//----------------------------------------------------------------------------//
bool GetStopCopyFunc()//取得是否停止複製
{
	return gStopCopyFunc;
}
//----------------------------------------------------------------------------//
void SetStopCopyFunc(bool bStop)//設定是否停止複製
{
	gStopCopyFunc = bStop;
}
//----------------------------------------------------------------------------//
bool IsFileExist(LPCTSTR FileName)//檔案是否存在
{
	if ( ::_tcslen(FileName)<1 ) 
	{	return false;	}

	FILE *pfile = ::_tfopen(FileName, _T("r"));
	if ( pfile == NULL ) 
	{	return false;	}
	::fclose(pfile);
	return true;
}
//----------------------------------------------------------------------------//
bool RemoveFolder(LPCTSTR Folder)//移除資料夾
{
	bool IsOK = JetAPI::ClearFolder(Folder);
	if ( IsOK == false ) { return false; }	
	/*
	const bool bCheckTempFolder=false;
	if ( true == bCheckTempFolder )
	{
		CString str;
		CString TempFolder = AOIDataCollect.GetSystemParameter().m_AOIBaseTempDirectory;
		if ( TempFolder.CompareNoCase(Folder) == 0 )
		{
			str.Format(_T("Remove Temp Folder [%s]"), Folder);
			JetAPI::ShowMessageBox(str);	
		}
	}
	*/
	bool IsDelete = (bool)(::RemoveDirectory(Folder));
	return true;
}
//----------------------------------------------------------------------------//
bool CreateFolder(LPCTSTR Folder)//強迫建立資料夾
{
	int Level = 0;
	CString SubString;	
	CString LastFolder;
	CString CurrentFolder;
	bool bRootFolder=true;
	const size_t Len=_tcslen(Folder);
	if ( 0 == Len )
	{	return false; }	
	while ( true )
	{
		if ( ::AfxExtractSubString(SubString, Folder, Level, _T('\\')) == FALSE )
		{
			if ( true == bRootFolder )
			{	return false; }
			break; 
		}

		if ( SubString.GetLength() == 0 ) 
		{
			Level ++;
			CurrentFolder += CString(_T("\\"));
			LastFolder = CurrentFolder;
			continue;
		}

		if ( true == bRootFolder )
		{	
			bRootFolder = false;			
			CurrentFolder += SubString;			
		}
		else
		{		
			CurrentFolder.Format(_T("%s\\%s"), LastFolder, SubString);
			::CreateDirectory(CurrentFolder, NULL);
		}
		LastFolder = CurrentFolder;
		Level ++;
	}
	if ( CurrentFolder.CompareNoCase(Folder) != 0 )
	{	return false; }
	if ( JetAPI::IsFolderExist(CurrentFolder) == false ) 
	{	return false; }
	return true;
}
//----------------------------------------------------------------------------//
bool ClearFolder(LPCTSTR Folder)//清除資料夾內容
{
	if ( NULL == Folder ) 
	{	return true; }
	const size_t FolderLen = ::_tcslen(Folder);
	if ( FolderLen == 0  ) 
	{	return true; }

	DWORD   Res = 0;
	CString Dir = _T("");
	CString SearchName = _T("");	
	CString FileName;
	bool IsFolder = false;	
	Dir.Format(_T("%s\\"), Folder);	
	FileName.Format(_T("%s%s"), Dir, _T("*.*") );

	HANDLE handle ;
	WIN32_FIND_DATA FindFileData;
	handle  = FindFirstFile(FileName, &FindFileData);
	if (handle  == INVALID_HANDLE_VALUE) 
	{	return true;	}

	do
	{
		SearchName.Format(_T("%s"), FindFileData.cFileName);
		if ( SearchName == _T('.') || SearchName == _T("..") ) { continue; }		
		FileName.Format(_T("%s%s"), Dir, FindFileData.cFileName);
		//IsDelete = JetAPI::IsFolderExist(FileName);
		Res = FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY;
		if ( 0 != Res ) 
		{	IsFolder = true; }
		else
		{	IsFolder = false; }
		
		if ( true == IsFolder )
		{  JetAPI::RemoveFolder(FileName); }
		else
		{  ::DeleteFile(FileName); }

	}while(::FindNextFile(handle, &FindFileData));

	FindClose(handle);
	return true;
}
//----------------------------------------------------------------------------//
bool CreateSyncFile(LPCTSTR Filename)//建立同步檔案
{
	if ( NULL == Filename ) { return false; }
	CString SyncName;
	CString TempName;
	CString MainName;		
	if ( ExtractMainFileName(Filename, MainName) == false )
	{	return false; }	
	TempName.Format(_T("%s.%s"), MainName, _T("SynTmp"));
	SyncName.Format(_T("%s.%s"), MainName, CHECK_FILE_SYNC_NAME);	
	::DeleteFile(SyncName);	
	if ( CreateTempFile(TempName, NULL) == false )
	{	return false; }
	if ( ::MoveFile(TempName, SyncName) == FALSE )
	{	return false; }
	return true;
}
//----------------------------------------------------------------------------//
bool GetSyncFilename(LPCTSTR Filename, CString &Sync)//取得同步檔名
{	
	CString MainName;
	if ( ExtractMainFileName(Filename, MainName) == false )
	{	return false; }		
	Sync.Format(_T("%s.%s"), MainName, CHECK_FILE_SYNC_NAME);
	return true;
}
//----------------------------------------------------------------------------//
bool CreateTempFile(LPCTSTR Filename, LPCTSTR Content)//建立暫存檔案
{
	if ( NULL == Filename ) { return false; }
	FILE *pFile = ::_tfopen(Filename, _T("w+"));
	if ( NULL == pFile )
	{	return false;	}
	if ( NULL != Content )
	{
		::_ftprintf(pFile, _T("%s\n"), Content);
	}
	::fclose(pFile);
	pFile = NULL;
	return true;
}
//----------------------------------------------------------------------------//
bool ClearFolder_rmdir(LPCTSTR Folder)//清除資料夾內容	
{
	if ( NULL == Folder ) 
	{	return true; }
	const size_t FolderLen = ::_tcslen(Folder);
	if ( FolderLen == 0  ) 
	{	return true; }

	const size_t len = MAX_JET_PATH;	
	TCHAR Param[len] = _T("");	
	//_stprintf(Param, _T("rmdir %s /S/Q"), Folder);
	//注意要加上/c, 並且路徑要加上""
	_stprintf(Param, _T("/c rmdir \"%s\" /s/q"), Folder);
	if( SystemCmdLine(_T("cmd.exe"), Param, SW_HIDE, 300000) == false )
	{	return false; }	
	return true;
}
//----------------------------------------------------------------------------//
bool GetFileAccessTime(LPCTSTR FileName, time_t &time)//取得檔案存取日期時間
{
	if ( NULL == FileName ) { return false; }	
	struct _stat64 buf;	
	::memset(&buf, 0x00, sizeof(buf));
	const int Res = _tstati64(FileName, &buf);
	if ( 0 != Res )
	{	return false;	}
	time = buf.st_atime;
	return true;
}
//----------------------------------------------------------------------------//
bool GetFileCreatedTime(LPCTSTR FileName, time_t &time)//取得檔案建立日期時間
{
	if ( NULL == FileName ) { return false; }	
	struct _stat64 buf;	
	::memset(&buf, 0x00, sizeof(buf));
	const int Res = _tstati64(FileName, &buf);
	if ( 0 != Res )
	{	return false;	}
	time = buf.st_ctime;
	return true;
}
//----------------------------------------------------------------------------//
bool GetFileModifedTime(LPCTSTR FileName, time_t &time)//取得檔案修改日期時間
{
	if ( NULL == FileName ) { return false; }	
	struct _stat64 buf;	
	::memset(&buf, 0x00, sizeof(buf));
	const int Res = _tstati64(FileName, &buf);
	if ( 0 != Res )
	{	return false;	}
	time = buf.st_mtime;
	return true;
}
//----------------------------------------------------------------------------//
bool SystemCmdLine(LPCTSTR Func, LPCTSTR Param, int nShow, DWORD dwTimeout)//執行系統函式
{
	if ( NULL==Func ) { return false; }
	CWinApp *App = ::AfxGetApp();
	
	SHELLEXECUTEINFO ShExecInfo = {0};
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = Func;
	ShExecInfo.lpParameters = Param;
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = nShow;	
	ShExecInfo.hInstApp = NULL;
	if ( ShellExecuteEx(&ShExecInfo) == FALSE )
	{	return false; }
	DWORD Ret = WaitForSingleObject(ShExecInfo.hProcess,dwTimeout);
	if ( WAIT_TIMEOUT == Ret ) 
	{	return false; }
	return true;
}
//----------------------------------------------------------------------------//
bool ClearFolderFiles(LPCTSTR Path, LPCTSTR FilterExt)//清除資料夾中特定副檔名的檔案
{
	const size_t ExtLen = 32;
	const size_t Len = _tcslen(Path);
	if( Len < 2 || Len >= ExtLen)
	{	return false;	}
	
	CString SrcExt = FilterExt;
	TCHAR   DstExt[ExtLen]=_T("");
	CString SearchName = _T("");	
	CString FileName;
	bool IsDir = false;
	FileName.Format(_T("%s\\*.*"), Path);

	DWORD  Res=0;
	HANDLE handle ;
	WIN32_FIND_DATA FindFileData;
	handle  = FindFirstFile(FileName, &FindFileData);
	if (handle  == INVALID_HANDLE_VALUE) 
	{	return true;	}

	do
	{
		SearchName.Format(_T("%s"), FindFileData.cFileName);
		if ( SearchName == _T('.') || SearchName == _T("..") ) { continue; }

		FileName.Format(_T("%s\\%s"), Path, SearchName);		
		//IsDir = JetAPI::IsFolderExist(FileName);
		Res = FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY;
		if ( 0 != Res ) 
		{	IsDir = true; }
		else
		{	IsDir = false; }

		if ( IsDir == true )	//資料夾
		{	JetAPI::ClearFolderFiles(FileName, FilterExt);	}
		else //檔案
		{ 				
			JetAPI::ExtractExtendFileName(FileName, DstExt, ExtLen);			
			if ( SrcExt.CompareNoCase(DstExt) == 0 ) 
			{	::DeleteFile(FileName);	}
		}
	}while(::FindNextFile(handle, &FindFileData));
	FindClose(handle );	
	return true;
}
//----------------------------------------------------------------------------//
bool DeleteFileList(const std::vector<CString> &FileList)//清除檔案列表
{
	size_t i=0;
	const size_t FileCount = FileList.size();
	for ( i=0; i<FileCount; i++ )
	{	::DeleteFile(FileList[i]);	}
	return true;
}
//----------------------------------------------------------------------------//
bool DeleteFileList(LPCTSTR Folder, const std::vector<CString> &FileList)//清除檔案列表
{
	size_t i=0;
	CString Filename;
	const size_t FileCount = FileList.size();
	for ( i=0; i<FileCount; i++ )
	{
		Filename.Format(_T("%s\\%s"), Folder, FileList[i]);
		::DeleteFile(Filename);	
	}
	return true;
}
//--------------------------------------------------------------------------------//
bool KeepLatestFolder(LPCTSTR Folder, size_t KeepCount)//保留最新幾個資料夾	
{
	std::vector<CString> FolderList;
	if ( ListFolder(Folder, FolderList) == false ) 
	{	return false; }

	const size_t FolderCount = FolderList.size();
	if ( KeepCount >= FolderCount ) { return true; }

	size_t  i=0;
	CString RemoveFolder;
	const size_t RemoveCount = FolderCount-KeepCount;
	std::sort(FolderList.begin(), FolderList.end());
	for ( i=0; i<RemoveCount; i++ )
	{	
		RemoveFolder.Format(_T("%s\\%s"), Folder, FolderList[i]);
		JetAPI::RemoveFolder(RemoveFolder);
	}
	return true;
}
//----------------------------------------------------------------------------//
bool ListFolder(LPCTSTR Folder, std::vector<CString> &FolderList)//列表資料夾
{
	if ( NULL == Folder ) { return false; }
	CString BaseFolder = Folder;
	if ( BaseFolder.GetLength() == 0 ) { return false; }

	DWORD   Res=0;
	CString FileName;
	CString SearchName = _T("");	
	FileName.Format(_T("%s\\*.*"), Folder);

	HANDLE handle ;
	WIN32_FIND_DATA FindFileData;
	handle  = FindFirstFile(FileName, &FindFileData);
	if (handle == INVALID_HANDLE_VALUE) 
	{	return true;	}

	do
	{
		Res = FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY;
		if ( 0 == Res ) { continue; }

		SearchName.Format(_T("%s"), FindFileData.cFileName);
		if ( SearchName == _T('.') || SearchName == _T("..") ) { continue; }
		FolderList.push_back(FindFileData.cFileName);
	} while(::FindNextFile(handle, &FindFileData));
	FindClose(handle );	
	return true;
}
//----------------------------------------------------------------------------//
bool CopyFiles(LPCTSTR SrcPath, LPCTSTR DestPath, LPCTSTR ExtName)//複製特定副檔名	
{
	CString SrcS = SrcPath;
	CString DesS = DestPath;

	SrcS.MakeUpper();
	DesS.MakeUpper();
	if ( DesS == SrcS ) { return true; }

	::CreateDirectory(DestPath, NULL);
	
	CString SrcFile;
	CString DesFile;
	CString FileName =_T("");
	FileName.Format(_T("%s\\*.%s"), SrcPath, ExtName);

	DWORD Res=0;
	HANDLE handle;
	WIN32_FIND_DATA FindFileData;
	handle  = FindFirstFile(FileName, &FindFileData);
	if (handle  == INVALID_HANDLE_VALUE) 
	{	return true;	}

	do
	{
		Res = FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY;
		if ( NULL != Res ) { continue; }

		SrcFile.Format(_T("%s\\%s"), SrcPath, FindFileData.cFileName);		
		DesFile.Format(_T("%s\\%s"), DestPath, FindFileData.cFileName);
		::DeleteFile(DesFile);
		::CopyFile(SrcFile, DesFile, FALSE);
	}while(::FindNextFile(handle, &FindFileData));
	FindClose(handle );	
	return true;
}
//----------------------------------------------------------------------------//
bool ListFilesInFolder(LPCTSTR Folder, LPCTSTR ExtName, std::vector<CString> &FileList)//尋找資料夾內的檔案
{
	if ( ListFilesInFolder(Folder, _T("*"), ExtName, FileList) == false )
	{	return false; }
	return true;	
}
//----------------------------------------------------------------------------//
bool ListFilesInFolder(LPCTSTR Folder, LPCTSTR ExtName, std::vector<WIN32_FIND_DATA> &FileList)//尋找資料夾內的檔案
{
	if ( ListFilesInFolder(Folder, _T("*"), ExtName, FileList) == false )
	{	return false; }
	return true;	
}
//----------------------------------------------------------------------------//
bool ListFilesInFolder(LPCTSTR Folder, LPCTSTR MainName, LPCTSTR ExtName, std::vector<CString> &FileList)//尋找資料夾內的檔案
{
	if ( NULL == Folder ) { return false; }	
	
	CString FileName =_T("");	
	CString SearchName =_T("");	
	CString strExtName = _T("");
	CString strMainName = _T("");	
	if ( NULL==MainName || 0==_tcslen(MainName) )
	{	strMainName = _T("*");	}
	else
	{	strMainName = MainName; }
	if ( NULL==ExtName || 0==_tcslen(ExtName) )
	{	strExtName = _T("*");	}
	else
	{	strExtName = ExtName; }

	DWORD  Res=0;
	HANDLE handle;
	WIN32_FIND_DATA FindFileData;
	SearchName.Format(_T("%s\\%s.%s"), Folder, strMainName, strExtName);
	handle  = FindFirstFile(SearchName, &FindFileData);
	if (handle == INVALID_HANDLE_VALUE) 
	{	return true;	}

	do
	{
		FileName = FindFileData.cFileName;
		if ( FileName == _T('.') || FileName == _T("..")) { continue; }
		Res = FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY;
		if ( NULL != Res ) { continue; }		
		FileList.push_back(FileName);
	} while(::FindNextFile(handle, &FindFileData));
	FindClose(handle );	
	return true;	
}
//----------------------------------------------------------------------------//
bool ListFilesInFolder(LPCTSTR Folder, LPCTSTR MainName, LPCTSTR ExtName, std::vector<WIN32_FIND_DATA> &FileList)//尋找資料夾內的檔案
{
	if ( NULL == Folder ) { return false; }	
	
	CString FileName =_T("");	
	CString SearchName =_T("");	
	CString strExtName = _T("");
	CString strMainName = _T("");	
	if ( NULL==MainName || 0==_tcslen(MainName) )
	{	strMainName = _T("*");	}
	else
	{	strMainName = MainName; }
	if ( NULL==ExtName || 0==_tcslen(ExtName) )
	{	strExtName = _T("*");	}
	else
	{	strExtName = ExtName; }

	DWORD  Res=0;
	HANDLE handle;
	WIN32_FIND_DATA FindFileData;
	SearchName.Format(_T("%s\\%s.%s"), Folder, strMainName, strExtName);
	handle  = FindFirstFile(SearchName, &FindFileData);
	if (handle == INVALID_HANDLE_VALUE) 
	{	return true;	}

	do
	{
		FileName = FindFileData.cFileName;
		if ( FileName == _T('.') || FileName == _T("..")) { continue; }
		Res = FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY;
		if ( NULL != Res ) { continue; }
		FileList.push_back(FindFileData);		
	} while(::FindNextFile(handle, &FindFileData));
	FindClose(handle );		
	return true;	
}
//----------------------------------------------------------------------------//
bool ListFilesByFileName(LPCTSTR Folder, const std::vector<CString> &NameList, std::vector<WIN32_FIND_DATA> &FileList)//將檔案名稱列表轉成檔案列表
{
	size_t       i=0;
	CString      PathName;
	DWORD        Res=0;
	HANDLE       handle=NULL;
	WIN32_FIND_DATA FindFileData;
	const size_t NameCount = NameList.size();

	for ( i=0; i<NameCount; i++ )
	{
		if ( NULL == Folder )
		{	PathName = NameList[i]; }
		else
		{	PathName.Format(_T("%s\\%s"), Folder, NameList[i]); }
		handle  = FindFirstFile(PathName, &FindFileData);
		if ( handle == INVALID_HANDLE_VALUE ) { continue; }
		FindClose(handle);	
		::_tcscpy(FindFileData.cFileName, NameList[i]);
		FileList.push_back(FindFileData);
	}
	return true;
}
//----------------------------------------------------------------------------//
bool CopyFolderAToFolderB(LPCTSTR SrcPath, LPCTSTR DesPath, bool IsDeleteSrc, bool IsClearDest, LPCTSTR ExtName, int CurrentLevel, int MaxLevel, bool bChkStop)//拷貝整個資料夾
{
	bool IsFilter = false;
	const size_t ExtLen = 32;	
	TCHAR   Ext[16]=_T("");
	CString FilterExt=ExtName;
	if( _tcslen(ExtName) >= 3)
	{ 
		FilterExt.MakeLower();
		IsFilter = true; 
	}
	CString SrcDir = SrcPath;
	CString DesDir = DesPath;
	CString SearchName = _T("");	
	CString FileName;
	CString DesFileName;
	bool IsDir = false;
	const size_t SrcLen = ::_tcslen(SrcPath);
	if ( SrcLen < 2 ) 
	{	return false; }

	if ( SrcDir.CompareNoCase(DesDir) == 0 ) 
	{	return true;	}	

	SrcDir.Format(_T("%s\\"), SrcPath);
	DesDir.Format(_T("%s\\"), DesPath);
	FileName.Format(_T("%s%s"), SrcDir, _T("*.*") );
	::CreateDirectory(DesPath, NULL);

	if( IsClearDest == true ) 
	{	JetAPI::ClearFolder(DesPath); }	

	DWORD Res=0;
	HANDLE handle ;
	WIN32_FIND_DATA FindFileData;
	handle  = FindFirstFile(FileName, &FindFileData);
	if (handle == INVALID_HANDLE_VALUE) 
	{	return true;	}
	
	const int NextLevel = CurrentLevel+1;	
	do
	{
		if ( true==bChkStop && true==GetStopCopyFunc() )
		{	break; }
		SearchName.Format(_T("%s"), FindFileData.cFileName);
		if ( SearchName == _T('.') || SearchName == _T("..")) { continue; }
		FileName.Format(_T("%s%s"), SrcDir, FindFileData.cFileName);
		Res = FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY;
		//IsDir = JetAPI::IsFolderExist(FileName);
		if ( NULL != Res ) 
		{	IsDir = true; }
		else
		{	IsDir = false; }
		DesFileName.Format(_T("%s%s"), DesDir, FindFileData.cFileName);

		if ( true == IsDir)	//資料夾
		{ 
			if ( MaxLevel >= 0 )//Kai
			{
				if ( NextLevel >= MaxLevel )
				{	continue; }
			}
			::CreateDirectory(DesFileName, NULL);
			if ( JetAPI::CopyFolderAToFolderB(FileName, DesFileName, IsDeleteSrc, IsClearDest, FilterExt, NextLevel, MaxLevel, bChkStop) == false )
			{	
				FindClose(handle);
				return false; 
			}
		}
		else //檔案
		{ 
			if( true == IsFilter )
			{
				JetAPI::ExtractExtendFileName(FileName, Ext, ExtLen);
				if ( FilterExt.CompareNoCase(Ext) != 0 )
				{ ::CopyFile(FileName, DesFileName, NULL); }
			}
			else
			{	::CopyFile(FileName, DesFileName, NULL); }			
			if ( true == IsDeleteSrc )
			{ ::DeleteFile(FileName); }
		}
	}while(::FindNextFile(handle, &FindFileData));
	FindClose(handle);
	
	if ( true == IsDeleteSrc )
	{	JetAPI::RemoveFolder(SrcPath); }
	return true;
}
//----------------------------------------------------------------------------//
bool  XCopyFolderAToFolderB(LPCTSTR SrcPath, LPCTSTR DesPath, bool IsDeleteSrc, bool IsClearDest, LPCTSTR ExtName, int CurrentLevel, int MaxLevel)//拷貝整個資料夾	
{
	bool IsFilter = false;
	const size_t ExtLen = 32;	
	TCHAR   Ext[16]=_T("");
	CString FilterExt=ExtName;
	if( _tcslen(ExtName) >= 3)
	{ 
		FilterExt.MakeLower();
		IsFilter = true; 
	}
	CString SrcDir = SrcPath;
	CString DesDir = DesPath;
	CString SearchName = _T("");		
	bool IsDir = false;
	const size_t SrcLen = ::_tcslen(SrcPath);
	if ( SrcLen < 2 ) 
	{	return false; }

	if ( SrcDir.CompareNoCase(DesDir) == 0 ) 
	{	return true;	}	

	SrcDir = SrcPath;
	DesDir = DesPath;	
	::CreateDirectory(DesPath, NULL);

	if( IsClearDest == true ) 
	{	JetAPI::ClearFolder(DesPath); }	
	
	CString Param;
	//_stprintf(TempStrT, _T("xcopy \"%s\" \"%s\" /s/c/i/r/q/k/y"), SrcDir, DesDir);
	//_stprintf(TempStrT, _T("xcopy %s %s /s/c/i/r/q/k/y"), SrcDir, DesDir);	
	//_stprintf(Param, _T("%s %s /s/c/i/r/q/k/y"), SrcDir, DesDir);	
	Param.Format(_T("%s %s /s/c/i/r/q/k/y"), SrcDir, DesDir);	
	HCURSOR OldCursor = ::SetCursor(::LoadCursor(NULL, IDC_WAIT));	
	//int ret = ::_tsystem(TempStrT);		
	if ( JetAPI::SystemCmdLine(_T("xcopy"), Param, SW_HIDE, 300000) == false ) 
	{
		::SetCursor(OldCursor);
		return false; 
	}
	::SetCursor(OldCursor);	
	if ( true == IsDeleteSrc )
	{	JetAPI::RemoveFolder(SrcPath); }
	return true;
}
//----------------------------------------------------------------------------//
bool  RoboCopyFolderAToFolderB(LPCTSTR SrcPath, LPCTSTR DesPath, bool IsDeleteSrc, bool IsClearDest, LPCTSTR ExtName, int CurrentLevel, int MaxLevel)//拷貝整個資料夾	
{
	bool IsFilter = false;
	const size_t ExtLen = 32;	
	TCHAR   Ext[16]=_T("");
	CString FilterExt=ExtName;
	if( _tcslen(ExtName) >= 3)
	{ 
		FilterExt.MakeLower();
		IsFilter = true; 
	}
	CString SrcDir = SrcPath;
	CString DesDir = DesPath;
	CString SearchName = _T("");		
	bool IsDir = false;
	const size_t SrcLen = ::_tcslen(SrcPath);
	if ( SrcLen < 2 ) 
	{	return false; }

	if ( SrcDir.CompareNoCase(DesDir) == 0 ) 
	{	return true;	}	

	SrcDir = SrcPath;
	DesDir = DesPath;	
	::CreateDirectory(DesPath, NULL);

	if( IsClearDest == true ) 
	{	JetAPI::ClearFolder(DesPath); }	
	
	CString   Param;
	const int CPUCoreNumber = AOIDataCollect.GetComputerCPUCoreNumber();
	const int ExecThreadCnt = MAX(1, CPUCoreNumber-1);	
	//NS:No size, //NC:No Class, //NFL:No File List, //NDL: No Dir List, //NP: No Progress	
	//_stprintf(Param, _T("%s %s /E /NS /NC /NFL /NDL /MT:%d"), SrcDir, DesDir, ExecThreadCnt);		
	Param.Format(_T("%s %s /E /NS /NC /NFL /NDL /MT:%d"), SrcDir, DesDir, ExecThreadCnt);		
	HCURSOR OldCursor = ::SetCursor(::LoadCursor(NULL, IDC_WAIT));
	if ( JetAPI::SystemCmdLine(_T("robocopy"), Param, SW_HIDE, 300000) == false ) 
	{
		::SetCursor(OldCursor);
		return false; 
	}
	::SetCursor(OldCursor);
	if ( true == IsDeleteSrc )
	{	JetAPI::RemoveFolder(SrcPath); }
	return true;
}
//----------------------------------------------------------------------------//
bool HugeCopyFolderAToFolderB(COPY_HUGE_FILES_MODE Mode, LPCTSTR SrcPath, LPCTSTR DesPath, bool IsDeleteSrc, bool IsClearDest, LPCTSTR ExtName, int CurrentLevel, int MaxLevel)//拷貝整個資料夾	
{
	bool IsOK = true;
	switch ( Mode )
	{
	case COPY_HUGE_FILES_XCOPY:		
		IsOK = JetAPI::XCopyFolderAToFolderB(SrcPath, DesPath, IsDeleteSrc, IsClearDest, ExtName, CurrentLevel, MaxLevel);
		break;
	case COPY_HUGE_FILES_ROBOCOPY:		
		IsOK = JetAPI::RoboCopyFolderAToFolderB(SrcPath, DesPath, IsDeleteSrc, IsClearDest, ExtName, CurrentLevel, MaxLevel);
		break;			
	default:
	case COPY_HUGE_FILES_COPY:		
		IsOK = JetAPI::CopyFolderAToFolderB(SrcPath, DesPath, IsDeleteSrc, IsClearDest, ExtName, CurrentLevel, MaxLevel);
		break;
	}
	return IsOK;
}
//----------------------------------------------------------------------------//
int GetImageChannels(IMAGE_SIZE BitCount)
{
	if (8 == BitCount) { return 1; }
	if (24 == BitCount) { return 3; }
	return 0;
}
//----------------------------------------------------------------------------//
int GetBMPImagePixelsPerLine(int ImageW, int Align)//取得影像每條所需的位元組
{
	assert((Align & (Align - 1)) == 0); // Align is a power of 2
    return (ImageW + Align-1) & -Align;
}
//----------------------------------------------------------------------------//
int GetBMPImagePixelsPerLine(int ImageW, int BitCount, int Align)//取得影像每條所需的位元組
{
	const int ByteCount = (BitCount+7)/8;
	const int TotalWidth = ByteCount*ImageW;	
	return GetBMPImagePixelsPerLine(TotalWidth, Align);
}
//----------------------------------------------------------------------------//
unsigned int GetBMPImagePixelsPerLine(unsigned int ImageW, int Align)//取得BMP影像每條所需的位元組
{
	assert((Align & (Align - 1)) == 0); // Align is a power of 2
    return (ImageW + Align-1) & -Align;

	//if ( Align < 1 ) { Align = 1; }
	//IMAGE_SIZE BytePerLine=((ImageW+Align-1)/Align);	
	//return BytePerLine*Align;
}
//----------------------------------------------------------------------------//
unsigned int GetBMPImagePixelsPerLine(unsigned int ImageW, unsigned int BitCount, int Align)//取得BMP影像每條所需的位元組
{
	const unsigned int ByteCount = (BitCount+7)/8;
	const unsigned int TotalWidth = ByteCount*ImageW;	
	return GetBMPImagePixelsPerLine(TotalWidth, Align);
}
//----------------------------------------------------------------------------//
bool UpdateSubWndFont(HWND hWnd, HGDIOBJ hFont)
{
	if ( NULL==hWnd || NULL==hFont ) { return true; }			
	::SendMessage(hWnd, WM_SETFONT, (WPARAM)hFont, TRUE);		
	if ( EnumChildWindows(hWnd, SetChildWndFont, (LPARAM)hFont) == FALSE )
	{	return false; }
	return true;
}
//----------------------------------------------------------------------------//
bool CreateWndFont(CFont &rFont, int nAdd, LPCTSTR Face)
{
	LOGFONT lf;		
	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);
	afxGlobalData.fontRegular.GetLogFont(&lf);
	afxGlobalData.GetNonClientMetrics(info);
	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;
	_tcscpy(lf.lfFaceName, info.lfMenuFont.lfFaceName);	
	//nAdd = 0;	
	if ( lf.lfHeight < 0 )
	{	lf.lfHeight -= nAdd; }
	else
	{	lf.lfHeight += nAdd; }
	::DeleteObject(rFont.Detach());
	rFont.CreateFontIndirect(&lf);
	return true;
}
//----------------------------------------------------------------------------//
void EnableCloseButton(HWND hWnd, BOOL bEnable)//啟用視窗右上方關閉按鈕(系統選單)
{
	if ( NULL == hWnd ) { return; }
	HMENU hMenu = ::GetSystemMenu(hWnd, FALSE);
	if ( NULL == hMenu ) { return ; }
	if ( TRUE == bEnable )
	{	::EnableMenuItem(hMenu, SC_CLOSE, MF_BYCOMMAND|MF_ENABLED);	}
	else
	{	::EnableMenuItem(hMenu, SC_CLOSE, MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);	}	 
}
//----------------------------------------------------------------------------//
void EnableMinimizeButton(HWND hWnd, BOOL bEnable)//啟用視窗右上方最小按鈕(系統選單)
{
	if ( NULL == hWnd ) { return; }
	LONG WndStyle = ::GetWindowLongW(hWnd, GWL_STYLE);
	if ( TRUE == bEnable )
	{	WndStyle |= WS_MINIMIZEBOX;	}
	else
	{	WndStyle &= ~WS_MINIMIZEBOX; }
	SetWindowLong(hWnd, GWL_STYLE,WndStyle);
}
//----------------------------------------------------------------------------//
void EnableMaximizeButton(HWND hWnd, BOOL bEnable)//啟用視窗右上方最大按鈕(系統選單)
{
	if ( NULL == hWnd ) { return; }
	LONG WndStyle = ::GetWindowLongW(hWnd, GWL_STYLE);
	if ( TRUE == bEnable )
	{	WndStyle |= WS_MAXIMIZEBOX;	}
	else
	{	WndStyle &= ~WS_MAXIMIZEBOX; }
	SetWindowLong(hWnd, GWL_STYLE,WndStyle);
}
//----------------------------------------------------------------------------//
void ClearCombox(CComboBox &Combox)
{
	int i=0;
	const int NItems = Combox.GetCount();
	for ( i=NItems-1; i>=0; i-- )
	{	Combox.DeleteString(i);	}
	Combox.RedrawWindow();
}
//----------------------------------------------------------------------------//
CString  GetComboxCurSelText(const CComboBox &Combox)
{
	CString Text;
	int CurSel = Combox.GetCurSel();
	if ( CurSel < 0 ) { return Text; }
	Combox.GetLBText(CurSel, Text);
	return Text;
}
//----------------------------------------------------------------------------//
DWORD_PTR  GetComboxCurSelData(const CComboBox &Combox)
{
	DWORD_PTR ID = -1;
	int CurSel = Combox.GetCurSel();
	if ( CurSel < 0 ) { return ID; }
	ID = (DWORD_PTR)Combox.GetItemData(CurSel);
	return ID;
}
//----------------------------------------------------------------------------//
void SetComboxCurSel(CComboBox &Combox, const DWORD AppendID)
{
	int i=0;
	DWORD_PTR ID = 0;	
	int nAppendID=(int)(AppendID);
	DWORD_PTR AppendID2 = AppendID;
	const int NItems = Combox.GetCount();	
	if ( nAppendID < 0 ) 
	{	AppendID2 = nAppendID;	}	
	for ( i=0; i<NItems; i++ )
	{
		ID = Combox.GetItemData(i);
		if ( ID == AppendID2 ) 
		{
			Combox.SetCurSel(i);
			break;
		}
	}
	if ( i == NItems )
	{	Combox.SetCurSel(-1);	}
	return;
}
//----------------------------------------------------------------------------//
void SetComboxCurSelPtr(CComboBox &Combox, const DWORD_PTR AppendID)
{	
	int i=0;
	DWORD_PTR ID = 0;		
	const int NItems = Combox.GetCount();		
	for ( i=0; i<NItems; i++ )
	{
		ID = Combox.GetItemData(i);
		if ( ID == AppendID ) 
		{
			Combox.SetCurSel(i);
			break;
		}
	}
	if ( i == NItems )
	{	Combox.SetCurSel(-1);	}
	return;
}
//----------------------------------------------------------------------------//
bool CallExecApp(HWND hWnd, LPCTSTR lpOperation, LPCTSTR lpFile, LPCTSTR lpParameters, LPCTSTR lpDirectory, int nShowCmd, bool bWait)//呼叫外部執行檔
{
	if ( false == bWait )
	{	
		::ShellExecute(hWnd, lpOperation, lpFile, lpParameters, lpDirectory, nShowCmd);	
	}
	else
	{
		BOOL Ret=TRUE;
		SHELLEXECUTEINFO ShExecInfo;		
		::memset(&ShExecInfo, 0x00, sizeof(ShExecInfo));
		ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
		ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;//重點
		ShExecInfo.hwnd = hWnd;
		ShExecInfo.lpVerb = lpOperation;
		ShExecInfo.lpFile = lpFile;
		ShExecInfo.lpParameters = lpParameters;
		ShExecInfo.lpDirectory = lpDirectory;
		ShExecInfo.nShow = nShowCmd;
		ShExecInfo.hInstApp = NULL;
		if ( FALSE == ::ShellExecuteEx(&ShExecInfo) )
		{	return false;	}		
		::WaitForSingleObject(ShExecInfo.hProcess, INFINITE);//等待該軟體關閉
		::CloseHandle(ShExecInfo.hProcess);		
	}
	return true;
}
//-----------------------------------------------------------------------------//
bool LoadBomFile(LPCTSTR pfilename, const TLoadParam_BOM &LoadParam, std::vector<TComponentNode> &NodeList, CString &ErrorString)//讀入BOM
{
	const bool bUnicode = LoadParam.bUnicode;
	if ( NULL == pfilename ) { return false; }
	NodeList.clear();
	//--------------------------------------------------------------------//
	FILE *pfile = OpenReadFile(pfilename, bUnicode);	
	if ( pfile == NULL )
	{		
		ErrorString.Format(_T("Open File Fault (%s)"), pfilename);
		return false;
	}
	//--------------------------------------------------------------------//
	size_t Pos=0;
	bool   bReadSucc=true;
	const int StartLine = LoadParam.nStartLine;
	const size_t textlinesize = 1024;
	//--------------------------------------------------------------------//
	char TextFilter = '\"';
	std::vector<char> DelimList={','};
	char Textline[textlinesize] = "";
	std::string trimChar=" ";
	std::string StringLine, SubString, Str;	
	std::vector<std::string> SubStringList;
	std::vector<std::string> SubStringList2;
	
	wchar_t wTextFilter = L'\"';
	std::vector<wchar_t> wDelimList={L','};
	wchar_t wTextline[textlinesize] = L"";
	std::wstring wtrimChar=L" ";
	std::wstring wStringLine, wSubString, wStr;		
	std::vector<std::wstring> wSubStringList;	
	std::vector<std::wstring> wSubStringList2;		
	//--------------------------------------------------------------------//
	size_t textlen=0;
	size_t DataLine=0;	
	size_t SubStringCount=0;	
	size_t SubStringCount2=0;	
	//--------------------------------------------------------------------//
	const size_t ItemIndex = LoadParam.nItemIdx;
	const size_t UsageIndex = LoadParam.nUsageIdx;
	const size_t PartNumberIndex = LoadParam.nPNIdx;
	const size_t DescriptionIndex = LoadParam.nDescIdx;
	const size_t LocationIndex = LoadParam.nLocationIdx;
	//--------------------------------------------------------------------//
	int       ItemID=0;
	size_t    szUsage=0;
	const int Item_NewIndex=0;	//新一列, 新料號與新零件
	const int Item_MoreComponent=1;//同料號+新零件
	const int Item_NewPartNumber=2;//新料號+同零件
	//--------------------------------------------------------------------//	
	std::vector<CString> LastComponentList;	
	CString Item, PartNumber, Description, Usage, Temp, String;
	//--------------------------------------------------------------------//
	//Item,H.HP/N,Description,,,Usage,Location
	while(!feof(pfile) )
	{ 
		DataLine++;
		
		if ( false == bUnicode )
		{
			if( fgets(Textline,textlinesize,pfile) ==NULL ) 
			{	continue;	}
			StringLine = Textline;
		}
		else
		{
			if( fgetws(wTextline,textlinesize,pfile) ==NULL ) 
			{	continue;	}			
			wStringLine = wTextline;
		}
		//未達到使用者要求的開始列數
		if ( DataLine < StartLine ) { continue; }

		if ( false == bUnicode )
		{	textlen=StringLine.length(); }
		else
		{	textlen=wStringLine.length(); }
		if ( textlen == 0 ) //enter鍵
		{	
			DataLine ++;			
			continue;	
		}
		
		Pos=0;		
		SubStringList.clear();
		wSubStringList.clear();
		if ( false == bUnicode )
		{	bReadSucc = JetAPI::ListSubString(StringLine, DelimList, TextFilter, SubStringList);	}
		else
		{	bReadSucc = JetAPI::ListSubString(wStringLine, wDelimList, wTextFilter, wSubStringList);	}
		if ( false == bReadSucc )
		{
			::fclose(pfile); pfile=NULL;
			return false;
		}				
		
		if ( false == bUnicode )
		{	SubStringCount = SubStringList.size();	}
		else
		{	SubStringCount = wSubStringList.size();	}		
		if ( 0 == SubStringCount ) { continue; }		

		ItemID = Item_NewIndex;		
		for ( size_t j=0; j<SubStringCount; j++ )
		{
			if ( false == bUnicode )
			{	
				Str = SubStringList[j];	
				Str = JetAPI::Trim(Str, trimChar);
				String = CString(Str.c_str());
			}
			else
			{	
				wStr = wSubStringList[j];	
				wStr = JetAPI::Trim(wStr, wtrimChar);
				String = CString(wStr.c_str());
			}
			
			if ( ItemIndex == j )//Item
			{	
				Item = String;	
				if ( Item == _T("#") )//新料號+同零件
				{	ItemID = Item_NewPartNumber;	}
				else
				{
					if ( Item.GetLength() == 0 )//同料號+新零件
					{	ItemID = Item_MoreComponent;	}
					else//新一列, 新料號與新零件
					{	ItemID = Item_NewIndex;	}
				}
				switch ( ItemID )
				{
				case Item_NewIndex:
					Usage = _T("");
					PartNumber = _T("");
					Description = _T("");
					if ( szUsage != LastComponentList.size() )
					{	szUsage = 0;	}
					szUsage = 0;
					LastComponentList.clear();	
					break;
				case Item_MoreComponent:
					ItemID = Item_MoreComponent;
					break;
				case Item_NewPartNumber:
					PartNumber = _T("");
					Description = _T("");
					break;
				}
			}
			else if ( PartNumberIndex == j )//H.HP/N-PartNumber
			{
				if ( Item_MoreComponent != ItemID )
				{	PartNumber = String;	}							
			}
			else if ( DescriptionIndex == j )//Description-會友逗號的可能性, 需要拿掉
			{	
				if ( Item_MoreComponent != ItemID )
				{	Description = String;	}
			}
			else if ( UsageIndex == j )
			{	
				if ( String.GetLength() > 0 )
				{	
					Usage = String;	
					szUsage = ::_ttoi(String);
				}					
			}
			else if ( j == LocationIndex )
			{
				if ( String.GetLength() > 0 )
				{
					if ( false == bUnicode )
					{
						SubStringList2.clear();
						if ( JetAPI::CheckPackedString(Str) == false )
						{	SubStringList2.push_back(Str);	}
						else
						{
							JetAPI::RemovePackedString(Str);							
							bReadSucc = JetAPI::ListSubString(Str, DelimList, TextFilter, SubStringList2);
						}
						SubStringCount2 = SubStringList2.size();
					}
					else
					{
						wSubStringList2.clear();
						if ( JetAPI::CheckPackedString(wStr) == false )
						{	wSubStringList2.push_back(wStr);	}
						else
						{
							JetAPI::RemovePackedString(wStr);							
							bReadSucc = JetAPI::ListSubString(wStr, wDelimList, wTextFilter, wSubStringList2);	
						}
						SubStringCount2 = wSubStringList2.size();
					}					

					for ( size_t k=0; k<SubStringCount2; k++ )
					{						
						if ( false == bUnicode )
						{	
							Str = SubStringList2[k];	
							Str = JetAPI::Trim(Str, trimChar);							
							String = CString(Str.c_str());
						}
						else
						{	
							wStr = wSubStringList2[k];	
							wStr = JetAPI::Trim(wStr, wtrimChar);
							String = CString(wStr.c_str());
						}
						if ( String.GetLength() > 0 )
						{
							TComponentNode BomNode;
							BomNode.sComponentName = String;
							BomNode.sPartNumber = PartNumber;
							BomNode.bMainPartNumber = true;
							NodeList.push_back(BomNode);
							LastComponentList.push_back(String);
						}
					}
				}
			}
		}
		if ( Item_NewPartNumber == ItemID )
		{
			const size_t LastComponentCount=LastComponentList.size();
			if ( szUsage != LastComponentCount )
			{	szUsage = szUsage; }
			for ( size_t k=0; k<LastComponentCount; k++ )
			{
				TComponentNode BomNode;
				BomNode.sComponentName=LastComponentList[k];
				BomNode.sPartNumber = PartNumber;
				BomNode.bMainPartNumber = false;
				NodeList.push_back(BomNode);
			}
		}
		
	};
	::fclose(pfile); pfile=NULL;	


#ifdef _DEBUG
	bool bSave=false;
	CString strFolder;
	CString strFilename;
	strFolder = AOIDataCollect.GetAOITempDirectory();
	if ( true == bSave )
	{
		strFilename.Format(_T("%s\\%s"), strFolder, _T("VerifyMyBom.CSV"));
		pfile = ::_tfopen(strFilename, _T("w+"));
		if ( NULL != pfile )
		{
			size_t Count=0;
			const size_t NodeCount=NodeList.size();
			PartNumber = _T("");
			::_ftprintf(pfile, _T("index, PartNumber, Component, Count\n"));
			for ( size_t i=0; i<NodeCount; i++ )
			{
				const TComponentNode &rBomNode=NodeList[i];
				if ( PartNumber.CompareNoCase(rBomNode.sPartNumber) != 0 )
				{	Count = 1;	}
				else
				{	Count ++;	}
				::_ftprintf(pfile, _T("%d, %s, %s, %d\n"), i+1, rBomNode.sPartNumber, rBomNode.sComponentName, Count);
				PartNumber = rBomNode.sPartNumber;
			}
			::fclose(pfile); pfile = NULL;
		}
	}
#endif//_DEBUG
	return true;
}
//----------------------------------------------------------------------------//
bool PreLoadBomFile(LPCTSTR pfilename, const TLoadParam_BOM &LoadParam, size_t &Columns, size_t &Rows, CString &ErrorString)//預先讀入BOM
{
	//--------------------------------------------------------------------//
	if ( NULL == pfilename ) { return false; }	
	//--------------------------------------------------------------------//
	size_t DataLine = 0;
	std::string TextLine;	
	std::wstring wTextLine;
	std::vector<std::string> SubTextList;
	std::vector<std::wstring> wSubTextList;
	const size_t textlinesize = 1024;
	char textline[textlinesize] = "";	
	wchar_t wtextline[textlinesize] = L"";
	size_t TextLen=0;
	size_t NColumns = 0;		
	size_t SubTextLen=0;
	char TextFilter = '\"';
	wchar_t wTextFilter = L'\"';
	std::vector<char> DelimList={','};	
	std::vector<wchar_t> wDelimList={L','};
	const bool bUnicode = LoadParam.bUnicode;
	const int  nStartLine= LoadParam.nStartLine;
	FILE *pfile = OpenReadFile(pfilename, bUnicode);
	if ( pfile == NULL )
	{		
		ErrorString.Format(_T("Open File Fault (%s)"), pfilename);		
		return false;
	}
	//--------------------------------------------------------------------//	
	while(!feof(pfile) )
	{ 
		DataLine++;		
		if ( false == bUnicode )
		{
			if( fgets(textline,textlinesize,pfile) ==NULL ) 
			{	continue;	}			
		}
		else
		{
			if( fgetws(wtextline,textlinesize,pfile) ==NULL ) 
			{	continue;	}
		}		
		if ( DataLine < nStartLine ) { continue; }

		if ( false == bUnicode )
		{	TextLen=::strlen(textline);	}
		else
		{	TextLen=::wcslen(wtextline);	}
		
		if ( TextLen == 0 ) //enter鍵
		{	
			DataLine++;
			continue;
		}

		if ( false == bUnicode )
		{				
			TextLine=textline;			
			JetAPI::ListSubString(TextLine, DelimList, TextFilter, SubTextList);
			SubTextLen = SubTextList.size();
		}
		else
		{	
			wTextLine=wtextline;			
			JetAPI::ListSubString(wTextLine, wDelimList, wTextFilter, wSubTextList);
			SubTextLen = wSubTextList.size();
		}		
		
		//NColumns		
		if ( NColumns < SubTextLen ) 
		{	NColumns = SubTextLen; }
	}
	//--------------------------------------------------------------------//
	::fclose(pfile);
	pfile = NULL;
	//--------------------------------------------------------------------//
	Columns = NColumns+1;
	Rows = DataLine;	
	return true;
}
//----------------------------------------------------------------------------//
bool PreLoadCADXYFile(LPCTSTR pfilename, size_t StartLine, const std::vector<char> &Delimiters, size_t &Columns, size_t &Rows, CString &ErrorString)//預先讀入CADXY
{
	//--------------------------------------------------------------------//
	if ( NULL == pfilename ) { return false; }	
	//--------------------------------------------------------------------//
	size_t DataLine = 0;
	const size_t textlinesize = 1024;
	char textline[textlinesize] = "";
	size_t textlen=0;
	size_t i=0, j=0;
	const char SpecDelimiter = ' ';//空白字元是區間內的特殊字元	
	size_t NColumns = 0, CurrentNColumns=0;	
	size_t chidx=0;	
	const size_t NDelimiters = Delimiters.size();	
	FILE *pfile = JetAPI::OpenReadFile(pfilename, false);	
	if ( pfile == NULL )
	{		
		ErrorString.Format(_T("Open File Fault (%s)"), pfilename);		
		return false;
	}
	//--------------------------------------------------------------------//	
	while(!feof(pfile) )
	{ 
		DataLine++;		
		if( fgets(textline,textlinesize,pfile) ==NULL ) 
		{	continue;	}
		if ( DataLine < StartLine ) { continue; }

		textlen=::strlen(textline);
		if ( textlen == 0 ) //enter鍵
		{	
			DataLine++;
			continue;
		}
		//和分隔字元比較
		CurrentNColumns = 0;
		chidx = 0;		
		do
		{
			//------先確認一開始的字元是否為分隔字元-----------------//
			if ( CurrentNColumns == 0 )
			{
				for ( i=0; i<NDelimiters; i++ )
				{
					if ( textline[chidx] == Delimiters[i] )
					{	break;	}
				}
				if ( i == NDelimiters )
				{
					//如果一開始不是分隔字元					
					CurrentNColumns ++;
					//濾除分隔字元與資料間的空白字元
					for ( i=chidx; i<textlen; i++ )
					{
						if ( textline[i] != SpecDelimiter ) 
						{ break; }
					}
					chidx = i;
					continue;
				}
				else
				{
					chidx ++;
					continue;
				}
			}
			
			//尋找有分隔字元
			for ( i=0; i<NDelimiters; i++ )
			{				
				if ( textline[chidx] == Delimiters[i] )
				{	break;	}
			}
			if ( i != NDelimiters )
			{				
				//如果有找到分隔字元
				for ( i=chidx+1; i<textlen; i++ )
				{
					//濾除分隔字元與資料間的空白字元
					if ( textline[i] == SpecDelimiter ) 
					{ continue; }

					for ( j=0; j<NDelimiters; j++ )
					{
						if ( textline[i] == Delimiters[j] )
						{	break;		}
					}

					if ( j == NDelimiters )
					{	
						//如果有找到下一筆資料的話
						CurrentNColumns ++;
						chidx = i;
						break;
					}
					
				}
				if ( i == textlen )
				{
					chidx = i;
					break;
				}
			}
			else
			{
				//如果沒有找到分隔字元
				chidx ++;
			}
		} while ( chidx < textlen );		
		//NColumns		
		if ( NColumns < CurrentNColumns ) 
		{	NColumns = CurrentNColumns; }
	}
	//--------------------------------------------------------------------//
	::fclose(pfile);
	pfile = NULL;
	//--------------------------------------------------------------------//
	Columns = NColumns+1;
	Rows = DataLine;
	return true;
}
//----------------------------------------------------------------------------//
bool PreLoadCADXYFile(LPCTSTR pfilename, size_t StartLine, const std::vector<wchar_t> &Delimiters, size_t &Columns, size_t &Rows, CString &ErrorString)//預先讀入CADXY	
{
	//--------------------------------------------------------------------//
	if ( NULL == pfilename ) { return false; }	
	//--------------------------------------------------------------------//
	size_t DataLine = 0;
	const size_t textlinesize = 1024;
	wchar_t textline[textlinesize] = L"";
	size_t textlen=0;
	size_t i=0, j=0;
	const wchar_t SpecDelimiter = L' ';//空白字元是區間內的特殊字元	
	size_t NColumns = 0, CurrentNColumns=0;	
	size_t chidx=0;	
	const size_t NDelimiters = Delimiters.size();	
	FILE *pfile = JetAPI::OpenReadFile(pfilename, true);	
	if ( pfile == NULL )
	{		
		ErrorString.Format(_T("Open File Fault (%s)"), pfilename);		
		return false;
	}
	//--------------------------------------------------------------------//	
	while(!feof(pfile) )
	{ 
		DataLine++;		
		if( fgetws(textline,textlinesize,pfile) ==NULL ) 
		{	continue;	}
		if ( DataLine < StartLine ) { continue; }

		textlen=::wcslen(textline);
		if ( textlen == 0 ) //enter鍵
		{	
			DataLine++;
			continue;
		}
		//和分隔字元比較
		CurrentNColumns = 0;
		chidx = 0;		
		do
		{
			//------先確認一開始的字元是否為分隔字元-----------------//
			if ( CurrentNColumns == 0 )
			{
				for ( i=0; i<NDelimiters; i++ )
				{
					if ( textline[chidx] == Delimiters[i] )
					{	break;	}
				}
				if ( i == NDelimiters )
				{
					//如果一開始不是分隔字元					
					CurrentNColumns ++;
					//濾除分隔字元與資料間的空白字元
					for ( i=chidx; i<textlen; i++ )
					{
						if ( textline[i] != SpecDelimiter ) 
						{ break; }
					}
					chidx = i;
					continue;
				}
				else
				{
					chidx ++;
					continue;
				}
			}
			
			//尋找有分隔字元
			for ( i=0; i<NDelimiters; i++ )
			{				
				if ( textline[chidx] == Delimiters[i] )
				{	break;	}
			}
			if ( i != NDelimiters )
			{				
				//如果有找到分隔字元
				for ( i=chidx+1; i<textlen; i++ )
				{
					//濾除分隔字元與資料間的空白字元
					if ( textline[i] == SpecDelimiter ) 
					{ continue; }

					for ( j=0; j<NDelimiters; j++ )
					{
						if ( textline[i] == Delimiters[j] )
						{	break;		}
					}

					if ( j == NDelimiters )
					{	
						//如果有找到下一筆資料的話
						CurrentNColumns ++;
						chidx = i;
						break;
					}
					
				}
				if ( i == textlen )
				{
					chidx = i;
					break;
				}
			}
			else
			{
				//如果沒有找到分隔字元
				chidx ++;
			}
		} while ( chidx < textlen );		
		//NColumns		
		if ( NColumns < CurrentNColumns ) 
		{	NColumns = CurrentNColumns; }
	}
	//--------------------------------------------------------------------//
	::fclose(pfile);
	pfile = NULL;
	//--------------------------------------------------------------------//
	Columns = NColumns+1;
	Rows = DataLine;
	return true;
}
//----------------------------------------------------------------------------//
unsigned int  GetStringCharValue(LPCTSTR string)//取得字串的碼
{	
	size_t i=0;
	TCHAR  tch;
	unsigned int val=0;
	unsigned int val2=0;	
	const size_t Len = ::_tcslen(string);

	val = 0;
	for ( i=0; i<Len; i++ )
	{
		tch = string[i];
		val2 = (unsigned int)(tch);
		val += val2;
	}
	return val;
}
//----------------------------------------------------------------------------//
bool RemoveChar(char ch, std::string &String)//移除字元
{
	size_t i=0;	
	std::string Tmp;
	bool Removed=false;
	const size_t Len=String.length();
	Tmp.resize(Len+1);
	Tmp.clear();
	for ( i=0; i<Len; i++ )
	{
		if ( ch == String[i] )
		{
			Removed = true;
			continue; 
		}
		Tmp.push_back(String[i]);
	}
	if ( false == Removed )
	{	return true; }

	Tmp.push_back('\0');
	String = Tmp;
	return true;
}
//----------------------------------------------------------------------------//
bool RemoveChar(wchar_t ch, std::wstring &String)//移除字元
{
	size_t i=0;	
	std::wstring Tmp;
	bool Removed=false;
	const size_t Len=String.length();
	Tmp.resize(Len+1);
	Tmp.clear();
	for ( i=0; i<Len; i++ )
	{
		if ( ch == String[i] )
		{
			Removed = true;
			continue; 
		}
		Tmp.push_back(String[i]);
	}
	if ( false == Removed )
	{	return true; }

	Tmp.push_back('\0');
	String = Tmp;
	return true;
}
//----------------------------------------------------------------------------//
std::string Trim(const std::string &Str, const std::string &charList)
{
	std::string Tmp = Str;
    Tmp.erase( 0 , Tmp.find_first_not_of(charList.c_str()));
	Tmp.erase(Tmp.find_last_not_of(charList.c_str())+1);
    return Tmp;
}
//----------------------------------------------------------------------------//
std::string TrimLeft(const std::string &Str, const std::string &charList)
{
	std::string Tmp = Str;
    Tmp.erase( 0 , Tmp.find_first_not_of(charList.c_str()));
    return Tmp;
}
//----------------------------------------------------------------------------//
std::string TrimRight(const std::string &Str, const std::string &charList)
{
	std::string Tmp = Str;
    Tmp.erase(Tmp.find_last_not_of(charList.c_str())+1);
    return Tmp;
}
//----------------------------------------------------------------------------//
std::wstring Trim(const std::wstring &Str, const std::wstring &charList)
{
	std::wstring Tmp = Str;
    Tmp.erase( 0 , Tmp.find_first_not_of(charList.c_str()));
	Tmp.erase(Tmp.find_last_not_of(charList.c_str())+1);
    return Tmp;
}
//----------------------------------------------------------------------------//
std::wstring TrimLeft(const std::wstring &Str, const std::wstring &charList)
{
	std::wstring Tmp = Str;
    Tmp.erase( 0 , Tmp.find_first_not_of(charList.c_str()));
    return Tmp;
}
//----------------------------------------------------------------------------//
std::wstring TrimRight(const std::wstring &Str, const std::wstring &charList)
{
	std::wstring Tmp = Str;
    Tmp.erase(Tmp.find_last_not_of(charList.c_str())+1);
    return Tmp;
}
//----------------------------------------------------------------------------//
bool ReadNextString(const std::string &textline, size_t &Pos, std::string &String)//讀取字串
{
	size_t i=0;
	size_t Len=textline.length();
	size_t StartPos=Pos, EndPos=Pos;	
	if ( StartPos > Len )
	{	return false; }
	for ( i=Pos; i<textline.length(); i++ )
	{		
		if ( 0x0A != textline[i] )
		{	continue; }
		EndPos = i;
		break;
	}
	if ( EndPos == StartPos ) { EndPos = Len; }
	Pos = EndPos+1;
	if ( 0x0A == textline[EndPos] ) { EndPos--; }
	if ( 0x0D == textline[EndPos] ) { EndPos--; }
	EndPos ++;
	if ( EndPos > Len ) { EndPos = Len; }
	String.assign(textline.begin()+StartPos, textline.begin()+EndPos);
	return true;
}
//----------------------------------------------------------------------------//
bool ReadNextString(const std::wstring &textline, size_t &Pos, std::wstring &String)//讀取字串
{
	size_t i=0;
	size_t Len=textline.length();
	size_t StartPos=Pos, EndPos=Pos;	
	if ( StartPos > Len )
	{	return false; }
	for ( i=Pos; i<textline.length(); i++ )
	{		
		if ( 0x0A != textline[i] )
		{	continue; }
		EndPos = i;
		break;
	}
	if ( EndPos == StartPos ) { EndPos = Len; }
	Pos = EndPos+1;
	if ( 0x0A == textline[EndPos] ) { EndPos--; }
	if ( 0x0D == textline[EndPos] ) { EndPos--; }
	EndPos ++;
	if ( EndPos > Len ) { EndPos = Len; }
	String.assign(textline.begin()+StartPos, textline.begin()+EndPos);
	return true;
}
//----------------------------------------------------------------------------//
bool RemovePackedString(std::string &textline, char ch)//移除包裹的字串的包裹符號
{
	if ( CheckPackedString(textline, ch) == false ) { return true; }
	std::string str;	
	str.push_back(ch);
	textline = JetAPI::Trim(textline, str);
	return true;
}
//----------------------------------------------------------------------------//
bool RemovePackedString(std::wstring &textline, wchar_t ch)//移除包裹的字串的包裹符號
{
	if ( CheckPackedString(textline, ch) == false ) { return true; }
	std::wstring str;	
	str.push_back(ch);
	textline = JetAPI::Trim(textline, str);
	return true;
}
//----------------------------------------------------------------------------//
bool CheckPackedString(const std::string &textline, char ch)//確認是包裹的字串
{		
	if ( textline.empty() == true ) { return false; }	
	auto end=textline.end()-1;
	auto head=textline.begin();
	if ( ch != *end ) { return false; }
	if ( ch != *head ) { return false; }
	return true;
}
//----------------------------------------------------------------------------//
bool CheckPackedString(const std::wstring &textline, wchar_t ch)//確認是包裹的字串
{
	if ( textline.empty() == true ) { return false; }	
	auto end=textline.end()-1;
	auto head=textline.begin();
	if ( ch != *end ) { return false; }
	if ( ch != *head ) { return false; }
	return true;
}
//----------------------------------------------------------------------------//
bool CheckIsDelimiter(char ch, const std::vector<char> &list) //確認是否是分隔符號
{
	if ( std::find(list.begin(), list.end(), ch) != list.end() ) { return true; }
	return false;
}
//----------------------------------------------------------------------------//
bool CheckIsDelimiter(wchar_t ch, const std::vector<wchar_t> &list) //確認是否是分隔符號
{
	if ( std::find(list.begin(), list.end(), ch) != list.end() ) { return true; }
	return false;
}
//----------------------------------------------------------------------------//
bool ListSubString(const std::string &textline, char Delim, std::vector<std::string> &strList)
{
	size_t i=0;	
	size_t EndPos=0;
	size_t StartPos=0;
	bool   Header=false;
	std::string Str;
	const size_t Len=textline.length();	
	for ( i=0; i<Len; i++ )
	{		
		if ( Delim == textline[i] )
		{
			if ( false == Header ) { continue; }

			EndPos = i;
			Header = false;
			Str.assign(textline.begin()+StartPos, textline.begin()+EndPos);	
			strList.push_back(Str);
			StartPos = EndPos = i;
			continue; 
		}
		
		if ( false == Header )
		{
			Header = true; 
			StartPos = i;	
		}		
	}		
	if ( true == Header )
	{
		EndPos = Len;		
		Str.assign(textline.begin()+StartPos, textline.begin()+EndPos);	
		strList.push_back(Str);		
	}	
	return true;
}
//----------------------------------------------------------------------------//
bool ListSubString(const std::wstring &textline, wchar_t Delim, std::vector<std::wstring> &strList)
{
	size_t i=0;	
	size_t EndPos=0;
	size_t StartPos=0;
	bool   Header=false;
	std::wstring Str;
	const size_t Len=textline.length();	
	for ( i=0; i<Len; i++ )
	{		
		if ( Delim == textline[i] )
		{
			if ( false == Header ) { continue; }

			EndPos = i;
			Header = false;
			Str.assign(textline.begin()+StartPos, textline.begin()+EndPos);	
			strList.push_back(Str);
			StartPos = EndPos = i;
			continue; 
		}
		
		if ( false == Header )
		{
			Header = true; 
			StartPos = i;	
		}		
	}		
	if ( true == Header )
	{
		EndPos = Len;		
		Str.assign(textline.begin()+StartPos, textline.begin()+EndPos);	
		strList.push_back(Str);		
	}	
	return true;
}
//----------------------------------------------------------------------------//
bool ListSubString(const std::string &textline, const std::vector<char> &DelimList, char TextFilter, std::vector<std::string> &strList)//列出子字串
{
	size_t i=0;	
	size_t EndPos=0;
	size_t StartPos=0;		
	bool   Header=false;
	bool   EndPack=true;//包裹字串結束-偶數
	std::string Str;	
	const char EndLn = '\n';	
	const char PackCh = TextFilter;//包裹字串符號	
	const size_t Len=textline.length();		
	strList.clear();
	for ( i=0; i<Len; i++ )
	{	
		if ( CheckIsDelimiter(textline[i], DelimList)  && true==EndPack )
		{			
			if ( false == Header )
			{ 
				strList.push_back("");
				continue; 
			}

			EndPos = i;
			Header = false;
			Str.assign(textline.begin()+StartPos, textline.begin()+EndPos);				
			strList.push_back(Str);
			StartPos = EndPos = i;
			continue; 
		}
		
		if ( false == Header )
		{
			Header = true; 
			StartPos = i;			
		}		
		if ( PackCh == textline[i] )
		{	EndPack = !EndPack;	}
	}		
	if ( true == Header )
	{
		EndPos = Len;
		for ( i=Len-1; i!=0; i-- )
		{
			if ( EndLn != textline[i] )
			{	
				EndPos=i+1;
				break;; 
			}
		}		
		Str.assign(textline.begin()+StartPos, textline.begin()+EndPos);	
		strList.push_back(Str);		
	}		
	return true;
}
//----------------------------------------------------------------------------//
bool ListSubString(const std::wstring &textline, const std::vector<wchar_t> &DelimList, wchar_t TextFilter, std::vector<std::wstring> &strList)//列出子字串
{
	size_t i=0;	
	size_t EndPos=0;
	size_t StartPos=0;		
	bool   Header=false;
	bool   EndPack=true;//包裹字串結束-偶數
	std::wstring Str;	
	const wchar_t EndLn = L'\n';	
	const wchar_t PackCh = TextFilter;//包裹字串符號	
	const size_t Len=textline.length();		
	strList.clear();
	for ( i=0; i<Len; i++ )
	{	
		if ( CheckIsDelimiter(textline[i], DelimList)  && true==EndPack )
		{			
			if ( false == Header )
			{ 
				strList.push_back(L"");
				continue; 
			}

			EndPos = i;
			Header = false;
			Str.assign(textline.begin()+StartPos, textline.begin()+EndPos);				
			strList.push_back(Str);
			StartPos = EndPos = i;
			continue; 
		}
		
		if ( false == Header )
		{
			Header = true; 
			StartPos = i;			
		}		
		if ( PackCh == textline[i] )
		{	EndPack = !EndPack;	}
	}		
	if ( true == Header )
	{
		EndPos = Len;
		for ( i=Len-1; i!=0; i-- )
		{
			if ( EndLn != textline[i] )
			{	
				EndPos=i+1;
				break;; 
			}
		}		
		Str.assign(textline.begin()+StartPos, textline.begin()+EndPos);	
		strList.push_back(Str);		
	}			
	return true;
}
//----------------------------------------------------------------------------//
bool DecoderNPM_BadBlockText(const std::string &textline, size_t &nRow, std::string &ColCode)
{
	//BadBlockInfoC=010A, 
	//BadBlockInfo?=?表示第幾列(Row), A=1, B=2, C=3,...Z=256, 2A, 2B, 2C 每一列有256個Block
	//讀入後[->010A], 剔除最左側0[->10A], 再反轉字元[->A01], 再由16進位轉成2進位[A01=1010 0000 0001]
	std::vector<std::string> strList;
	nRow = 0;
	ColCode = "";
	if ( ListSubString(textline, '=', strList) == false ) { return false; }
	const size_t StrCount=strList.size();
	if ( 2 != StrCount ) { return false; }

	size_t i=0;
	size_t len=0;
	std::string Str;	
	int CharCnt=1, CharNum=1;	
	const size_t StartPos = 12;
	std::string Key = strList[0];
	const size_t KeyLen=Key.length();
	if ( KeyLen < StartPos ) { return false; }
	Str.assign(Key.begin()+StartPos, Key.begin()+KeyLen-1);	
	len = Str.length();
	if ( 0 == len ) { CharCnt = 1; }
	else { CharCnt = ::atoi(Str.c_str()); }
	Str.assign(Key.begin()+KeyLen-1, Key.begin()+KeyLen);	
	if ( Str[0] < 'a' ) { CharNum = Str[0]-'A'+1; }
	else { CharNum = Str[0]-'a'+1; }
	nRow = ((CharCnt-1)*26)+CharNum;

	bool bStart=false;
	char Bin[8]="";	
	std::string Tmp;
	std::string Val = strList[1];
	const size_t ValLen=Val.length();
	Str="";
	for ( i=0; i<ValLen; i++ )
	{
		const char ch=Val[i];
		if ( ' '==ch ) { continue; }
		if ( '0'!=ch ) { bStart=true; }
		if ( '0'==ch && false==bStart )
		{	continue;	}
		if ( HexToBin(ch, Bin) == false )
		{	continue; }
		if ( Str.length() == 0 )
		{	Str = Bin;	}
		else
		{
			Tmp = Str;
			Str = std::string(Bin)+Tmp;
		}		
	}
	ColCode = Str;
	return true;
}
//----------------------------------------------------------------------------//
bool DecoderNPM_BadBlockText(const std::wstring &textline, size_t &nRow, std::wstring &ColCode)
{
	//BadBlockInfoC=010A, 
	//BadBlockInfo?=?表示第幾列(Row), A=1, B=2, C=3,...Z=256, 2A, 2B, 2C 每一列有256個Block
	//讀入後[->010A], 剔除最左側0[->10A], 再反轉字元[->A01], 再由16進位轉成2進位[A01=1010 0000 0001]
	std::vector<std::wstring> strList;
	nRow = 0;
	ColCode = L"";
	if ( ListSubString(textline, L'=', strList) == false ) { return false; }
	const size_t StrCount=strList.size();
	if ( 2 != StrCount ) { return false; }

	size_t i=0;
	size_t len=0;
	std::wstring Str;	
	int CharCnt=1, CharNum=1;	
	const size_t StartPos = 12;
	std::wstring Key = strList[0];
	const size_t KeyLen=Key.length();
	if ( KeyLen < StartPos ) { return false; }
	Str.assign(Key.begin()+StartPos, Key.begin()+KeyLen-1);	
	len = Str.length();
	if ( 0 == len ) { CharCnt = 1; }
	else { CharCnt = ::_wtoi(Str.c_str()); }
	Str.assign(Key.begin()+KeyLen-1, Key.begin()+KeyLen);	
	if ( Str[0] > L'z' ) { CharNum = Str[0]-L'A'+1; }
	else { CharNum = Str[0]-L'a'+1; }
	nRow = ((CharCnt-1)*26)+CharNum;

	bool bStart=false;
	wchar_t Bin[8]=L"";	
	std::wstring Val = strList[1];
	const size_t ValLen=Val.length();
	Str=L"";
	for ( i=0; i<ValLen; i++ )
	{
		const char ch=Val[i];
		if ( L' '==ch ) { continue; }
		if ( L'0'!=ch ) { bStart=true; }
		if ( L'0'==ch && false==bStart )
		{	continue;	}
		if ( HexToBin(ch, Bin) == false )
		{	continue; }
		Str += std::wstring(Bin);
	}
	ColCode = Str;
	return true;
}
//----------------------------------------------------------------------------//
bool DecoderTextLine(const TCHAR textline[], TCHAR index[], TCHAR data[], TCHAR Delim)
{
	int i=0, j=0;
	int EndI=-1;
	bool GetFirst = false;
	const TCHAR  ch  = Delim;
	const int len = (int)(::_tcslen(textline));

	j = 0;
	data[0] = _T('\0');
	GetFirst = false;
	for ( i=0; i<len; i++ )
	{
		if ( GetFirst == false )
		{
			if ( textline[i] == _T(' ') )//跳過空白字元
			{	continue;	}
		}
		GetFirst = true;
		if ( textline[i] == ch )
		{
			index[j] = _T('\0');
			EndI = i+1;
			break;
		}
		index[j] = textline[i];
		j ++;
	}
	if ( i==len || EndI<0 ) 
	{	return false;	}

	//::AfxComparePath();
	//::AfxExtractSubString
	//::AfxFullPath
	//::AfxIsValidString  
	//::AfxTimeToFileTime();
	//::AfxExtractSubString(
	
	j = 0;
	int EndI2 = -1;
	GetFirst = false;
	for ( i=EndI; i<len; i++ )
	{
		if ( GetFirst == false )
		{
			if ( textline[i] == _T(' ') )//跳過空白字元
			{	continue;	}
		}		
		GetFirst = true;
		if ( textline[i]==ch || textline[i] == _T('\n') )//跳過換行字元
		{
			data[j] = _T('\0');
			EndI2 = i+1;
			break;
		}
		data[j] =  textline[i];
		j ++;
	}			
	
	if ( EndI2 < 0 ) 
	{
		if ( false == GetFirst )
		{	return false;  }
		data[j] = _T('\0');
		EndI2 = i;
		j ++;
	}
	return true;
}
//----------------------------------------------------------------------------//
bool DecoderTextLine(const TCHAR textline[], TCHAR index[], TCHAR data[], TCHAR data2[], TCHAR Delim)
{
	int i=0, j=0;	
	bool GetFirst = false;
	const TCHAR  ch  = Delim;
	const int len = (int)(::_tcslen(textline));

	data[0] = _T('\0');
	data2[0] = _T('\0');

	j = 0;	
	int EndI=-1;
	GetFirst = false;
	for ( i=0; i<len; i++ )
	{
		if ( GetFirst == false )
		{
			if ( textline[i] == _T(' ') )//跳過空白字元
			{	continue;	}
		}
		GetFirst = true;
		if ( textline[i] == ch )
		{
			index[j] = _T('\0');
			EndI = i+1;
			break;
		}
		index[j] = textline[i];
		j ++;
	}
	if ( i==len || EndI<0 ) 
	{	return false;	}

	j = 0;
	int EndI2 =-1;
	GetFirst = false;
	for ( i=EndI; i<len; i++ )
	{
		if ( GetFirst == false )
		{
			if ( textline[i] == _T(' ') )//跳過空白字元
			{	continue;	}
		}			
		GetFirst = true;
		if ( textline[i]==ch || textline[i] == _T('\n') )
		{
			data[j] = _T('\0');
			EndI2 = i+1;
			break;
		}
		data[j] =  textline[i];
		j ++;
	}				
	if ( i==len || EndI2<0 ) 
	{	return false; }

	j = 0;	
	int EndI3 = -1;
	GetFirst = false;
	for ( i=EndI2; i<len; i++ )
	{
		if ( GetFirst == false )
		{
			if ( textline[i] == _T(' ') )//跳過空白字元
			{	continue;	}
		}			
		GetFirst = true;
		if ( textline[i]==ch || textline[i] == _T('\n') )
		{
			data2[j] = _T('\0');
			EndI3 = i+1;
			break;
		}
		data2[j] = textline[i];
		j ++;
	}
	if ( EndI3 < 0 ) 
	{
		if ( false == GetFirst )
		{	return false;  }
		data2[j] = _T('\0');
		EndI3 = i;
		j ++;
	}
	return true;
}
//----------------------------------------------------------------------------//
bool DecoderTextLineA(const char textline[], char index[], char data[], char Delim)
{
	int i=0, j=0;
	int EndI=-1;
	bool GetFirst = false;
	const char ch  = Delim;
	const int len = (int)(::strlen(textline));

	j = 0;
	data[0] = ('\0');
	GetFirst = false;
	for ( i=0; i<len; i++ )
	{
		if ( GetFirst == false )
		{
			if ( textline[i] == (' ') )//跳過空白字元
			{	continue;	}
		}
		GetFirst = true;
		if ( textline[i] == ch )
		{
			index[j] = ('\0');
			EndI = i+1;
			break;
		}
		index[j] = textline[i];
		j ++;
	}
	if ( i==len || EndI<0 ) 
	{	return false;	}

	j = 0;
	int EndI2 = -1;
	GetFirst = false;
	for ( i=EndI; i<len; i++ )
	{
		if ( GetFirst == false )
		{
			if ( textline[i] == (' ') )//跳過空白字元
			{	continue;	}
		}									
		GetFirst = true;
		if ( textline[i]==ch || textline[i]==('\n') )
		{
			data[j] = ('\0');
			EndI2 = i+1;
			break;
		}
		data[j] =  textline[i];
		j ++;
	}
	if ( EndI2 < 0 ) 
	{
		if ( false == GetFirst )
		{	return false;  }
		data[j] = ('\0');
		EndI2 = i;
		j ++;
	}
	return true;
}
//----------------------------------------------------------------------------//
bool DecoderTextLineA(const char textline[], char index[], char data[], char data2[], char Delim)
{
	int i=0, j=0;	
	bool GetFirst = false;
	const char ch  = Delim;
	const int len = (int)(::strlen(textline));

	data[0] = ('\0');
	data2[0] = ('\0');

	j = 0;
	int EndI=-1;	
	GetFirst = false;
	for ( i=0; i<len; i++ )
	{
		if ( GetFirst == false )
		{
			if ( textline[i] == (' ') )//跳過空白字元
			{	continue;	}
		}
		GetFirst = true;
		if ( textline[i] == ch )
		{
			index[j] = ('\0');
			EndI = i+1;
			break;
		}
		index[j] = textline[i];
		j ++;
	}
	if ( i==len || EndI<0 ) 
	{	return false; }
	
	j = 0;
	int EndI2 = -1;
	GetFirst = false;
	for ( i=EndI; i<len; i++ )
	{
		if ( GetFirst == false )
		{
			if ( textline[i] == (' ') )//跳過空白字元
			{	continue;	}
		}		
		GetFirst = true;
		if ( textline[i]==ch || textline[i] == ('\n') )
		{
			index[j] = ('\0');
			EndI2 = i+1;
			break;
		}
		data[j] =  textline[i];
		j ++;
	}		
	if ( i==len || EndI2<0 ) 
	{	return false; }

	j = 0;
	int EndI3 = -1;
	GetFirst = false;
	for ( i=EndI2; i<len; i++ )
	{
		if ( GetFirst == false )
		{
			if ( textline[i] == (' ') )//跳過空白字元
			{	continue;	}
		}		
		GetFirst = true;
		if ( textline[i]==ch || textline[i] == ('\n') )
		{
			data2[j] = ('\0');
			EndI3 = i+1;
			break;
		}
		data2[j] =  textline[i];
		j ++;
	}		
	if ( EndI3 < 0 ) 
	{
		if ( false == GetFirst )
		{	return false;  }
		data2[j] = ('\0');
		EndI3 = i;
		j ++;
	}
	return true;
}
//----------------------------------------------------------------------------//
bool DecoderTextLineW(const wchar_t textline[], wchar_t index[], wchar_t data[], wchar_t Delim)
{
	int i=0, j=0;
	int EndI=-1;
	bool GetFirst = false;
	const wchar_t ch  = Delim;
	const int len = (int)(::wcslen(textline));

	j = 0;
	data[0] = L'\0';
	GetFirst = false;
	for ( i=0; i<len; i++ )
	{
		if ( GetFirst == false )
		{
			if ( textline[i] == L' ' )//跳過空白字元
			{	continue;	}
		}
		GetFirst = true;
		if ( textline[i] == ch )
		{
			index[j] = L'\0';
			EndI = i+1;
			break;
		}
		index[j] = textline[i];
		j ++;
	}
	if ( i==len || EndI<0 ) 
	{	return false;	}

	j = 0;
	int EndI2 = -1;
	GetFirst = false;
	for ( i=EndI; i<len; i++ )
	{
		if ( GetFirst == false )
		{
			if ( textline[i] == L' ' )//跳過空白字元
			{	continue;	}
		}							
		GetFirst = true;
		if ( textline[i]==ch || textline[i]==L'\n' )//跳過換行字元
		{
			data[j] = L'\0';
			EndI2 = i+1;
			break;
		}
		data[j] =  textline[i];
		j ++;
	}	
	if ( EndI2 < 0 ) 
	{
		if ( false == GetFirst )
		{	return false;  }
		data[j] = L'\0';
		EndI2 = i;
		j ++;
	}
	return true;
}
//----------------------------------------------------------------------------//
bool DecoderTextLineW(const wchar_t textline[], wchar_t index[], wchar_t data[], wchar_t data2[], wchar_t Delim)
{
	int i=0, j=0;	
	bool GetFirst = false;
	const wchar_t ch  = Delim;
	const int len = (int)(::wcslen(textline));
	data[0] = L'\0';
	data2[0] = L'\0';

	j = 0;
	int EndI=-1;
	GetFirst = false;
	for ( i=0; i<len; i++ )
	{
		if ( GetFirst == false )
		{
			if ( textline[i] == L' ' )//跳過空白字元
			{	continue;	}
		}
		GetFirst = true;
		if ( textline[i] == ch )
		{
			index[j] = L'\0';
			EndI = i+1;
			break;
		}
		index[j] = textline[i];
		j ++;
	}
	if ( i==len || EndI<0 ) 
	{	return false;	}

	j = 0;
	int EndI2=-1;
	GetFirst = false;
	for ( i=EndI; i<len; i++ )
	{
		if ( GetFirst == false )
		{
			if ( textline[i] == L' ' )//跳過空白字元
			{	continue;	}
		}
								
		GetFirst = true;
		if ( textline[i]==ch || textline[i]==L'\n' )
		{
			data[j] = L'\0';
			EndI2 = i+1;
			break;
		}
		data[j] =  textline[i];
		j ++;
	}		
	if ( i==len || EndI2<0 ) 
	{	return false;	}

	j = 0;
	int EndI3=-1;
	GetFirst = false;
	for ( i=EndI2; i<len; i++ )
	{
		if ( GetFirst == false )
		{
			if ( textline[i] == L' ' )//跳過空白字元
			{	continue;	}
		}
							
		GetFirst = true;
		if ( textline[i]==ch || textline[i]==L'\n' )
		{
			data2[j] = L'\0';
			EndI3 = i+1;
			break;
		}
		data2[j] =  textline[i];
		j ++;
	}		
	if ( EndI3 < 0 ) 
	{
		if ( false == GetFirst )
		{	return false;  }
		data2[j] = L'\0';
		EndI3 = i;
		j ++;
	}
	return true;
}
//----------------------------------------------------------------------------//
bool ExpandString_Increment(const std::string &src, int inc, std::string &dst, int base)//字串擴展-自動疊加
{
	size_t NumIdx=0;
	const int base36 = 36;//36進位
	const size_t len=src.length();
	for ( size_t i=len-1; i!=-1; i-- )
	{
		if ( isdigit(src[i]) != 0 ) { continue; }
		if ( base36 == base )
		{				
			if ( src[i]>='a' && src[i]<='z' )
			{	continue;	}
			else if ( src[i]>='A' && src[i]<='Z' )
			{	continue;	}						
		}
		NumIdx = i+1;
		break;
	}
	if ( len == NumIdx )
	{	return false;	}
	
	size_t MaxNumCount=9;
	if ( base36 == base )
	{	MaxNumCount = 5;	}
	const size_t NumCount=len-NumIdx;
	if ( NumCount > MaxNumCount )
	{	NumIdx = len-MaxNumCount;	}

	const size_t szBuffer=128;
	char Buffer[szBuffer]="";
	std::string NumStr=src.substr(NumIdx);
	const size_t SubLen=NumStr.length();
	if ( SubLen >= szBuffer )
	{	return false; }

	int Num=0;
	if ( base36 == base )
	{	Base36ToInt(NumStr.c_str(), Num);	}
	else
	{	Num = ::atoi(NumStr.c_str());		}
	Num += inc;
	bool bSucc=true;
	switch ( base )
	{
	case 10:	bSucc=IntToDec(Num, SubLen, Buffer);	break;
	default:	bSucc=IntToAny(Num, SubLen, Buffer, base); break;
	}
	if ( false == bSucc )
	{	return false; }	
	const size_t SubLen2=::strlen(Buffer);

	dst = src;	
	dst.replace(NumIdx, SubLen2, Buffer);
	return true;
}
//----------------------------------------------------------------------------//
bool ExpandString_Increment(const std::wstring &src, int inc, std::wstring &dst, int base)//字串擴展-自動疊加
{
	size_t NumIdx=0;
	const int base36 = 36;//36進位
	const size_t len=src.length();
	for ( size_t i=len-1; i!=-1; i-- )
	{
		if ( isdigit(src[i]) != 0 ) { continue; }
		if ( base36 == base )
		{				
			if ( src[i]>=L'a' && src[i]<=L'z' )
			{	continue;	}
			else if ( src[i]>=L'A' && src[i]<=L'Z' )
			{	continue;	}						
		}	
		NumIdx = i+1;
		break;
	}
	if ( len == NumIdx )
	{	return false;	}
	
	size_t MaxNumCount=9;
	if ( base36 == base )
	{	MaxNumCount = 5;	}
	const size_t NumCount=len-NumIdx;
	if ( NumCount > MaxNumCount )
	{	NumIdx = len-MaxNumCount;	}

	const size_t szBuffer=128;
	wchar_t Buffer[szBuffer]=L"";
	std::wstring NumStr=src.substr(NumIdx);
	const size_t SubLen=NumStr.length();
	if ( SubLen >= szBuffer )
	{	return false; }

	int Num=0;
	if ( base36 == base )
	{	Base36ToInt(NumStr.c_str(), Num);	}
	else
	{	Num=::_wtoi(NumStr.c_str());	}
	Num += inc;
	bool bSucc=true;
	switch ( base )
	{
	case 10:	bSucc=IntToDec(Num, SubLen, Buffer);	break;
	default:	bSucc=IntToAny(Num, SubLen, Buffer, base); break;
	}
	if ( false == bSucc )
	{	return false; }	
	const size_t SubLen2=::wcslen(Buffer);
	
	dst = src;
	dst.replace(NumIdx, SubLen2, Buffer);
	return true;
}
//----------------------------------------------------------------------------//
bool ExpandString_AddChar(const std::string &src, int nChar, int val, std::string &dst)//字串擴展-外加1字元
{
	const size_t szBuffer=128;	
	char Buffer[szBuffer]="";
	if ( IntToDec(val, nChar, Buffer) == false )
	{	return false; }		
	dst = src+std::string(Buffer);
	return true;
}
//----------------------------------------------------------------------------//
bool ExpandString_AddChar(const std::wstring &src, int nChar, int val, std::wstring &dst)//字串擴展-外加2字元
{
	const size_t szBuffer=128;	
	wchar_t Buffer[szBuffer]=L"";
	if ( IntToDec(val, nChar, Buffer) == false )
	{	return false; }	
	dst = src+std::wstring(Buffer);
	return true;
}
//----------------------------------------------------------------------------//
bool ExpandString_Replace(const std::string &src, int nChar, int val, std::string &dst)//字串擴展-取代1字元
{
	const size_t len=src.length();	
	if ( len < nChar ) 
	{	return false; }

	const size_t szBuffer=128;	
	char Buffer[szBuffer]="";
	if ( IntToDec(val, nChar, Buffer) == false )
	{	return false; }		
	dst = src;
	dst.replace(len-nChar, nChar, Buffer);
	return true;
}
//----------------------------------------------------------------------------//
bool ExpandString_Replace(const std::wstring &src, int nChar, int val, std::wstring &dst)//字串擴展-取代2字元
{
	const size_t len=src.length();	
	if ( len < nChar ) 
	{	return false; }

	const size_t szBuffer=128;	
	wchar_t Buffer[szBuffer]=L"";
	if ( IntToDec(val, nChar, Buffer) == false )
	{	return false; }	
	dst = src;
	dst.replace(len-nChar, nChar, Buffer);
	return true;
}
//----------------------------------------------------------------------------//
bool FindTextInString(LPCTSTR Text, LPCTSTR String)
{
#ifdef UNICODE	
	return FindTextInStringW(Text, String);
#else
	return FindTextInStringA(Text, String);
#endif//UNICODE
	return true;
}
//----------------------------------------------------------------------------//
bool FindTextInStringA(LPCSTR Text, LPCSTR String)
{
	if ( NULL == Text || NULL == String ) { return false; }

	size_t  i=0, j=0;
	const size_t szText = ::strlen(Text);
	const size_t szString = ::strlen(String);
	if ( szText > szString ) { return false; }

	char BufferText[256]="";
	char BufferString[256]="";

	::strcpy(BufferText, Text);
	::strcpy(BufferString, String);
	::_strupr(BufferText);
	::_strupr(BufferString);

	for ( i=0; i<szString-szText+1; i++ )
	{
		for ( j=0; j<szText; j++ )
		{
			if ( BufferText[j] == BufferString[i+j] ) { continue; }
			break;
		}
		if ( szText == j ) 
		{	return true; }
	}	
	return false;
}
//----------------------------------------------------------------------------//
bool FindTextInStringW(LPCWSTR Text, LPCWSTR String)
{
	if ( NULL == Text || NULL == String ) { return false; }
	size_t  i=0, j=0;
	const size_t szText = ::wcslen(Text);
	const size_t szString = ::wcslen(String);
	if ( szText > szString ) { return false; }

	wchar_t BufferText[256]=L"";
	wchar_t BufferString[256]=L"";

	::wcscpy(BufferText, Text);
	::wcscpy(BufferString, String);
	::_wcsupr(BufferText);
	::_wcsupr(BufferString);

	for ( i=0; i<szString-szText+1; i++ )
	{
		for ( j=0; j<szText; j++ )
		{				
			if ( BufferText[j] == BufferString[i+j] ) { continue; }
			break;
		}
		if ( szText == j ) 
		{	return true; }
	}	
	return false;
}
//----------------------------------------------------------------------------//
double MatchTwoString(LPCTSTR String1, LPCTSTR String2)
{
	double Score=0;
#ifdef UNICODE	
	Score = MatchTwoStringW(String1, String2);
#else
	Score = MatchTwoStringA(String1, String2);
#endif//UNICODE
	return Score;
}
//----------------------------------------------------------------------------//
double MatchTwoStringA(LPCSTR String1, LPCSTR String2)
{
	double Score=0;
	if ( NULL == String1 || NULL == String2 ) { return false; }
	
	const size_t szString1 = ::strlen(String1);
	const size_t szString2 = ::strlen(String2);
	const size_t szString = MIN(szString1, szString2);
	if ( 0 == szString )
	{	return Score; }

	char BufferStr1[256]="";
	char BufferStr2[256]="";

	::strcpy(BufferStr1, String1);
	::strcpy(BufferStr2, String2);
	::_strupr(BufferStr1);
	::_strupr(BufferStr2);
	
	size_t  i=0;
	int     Cnt=0;
	for ( i=0; i<szString; i++ )
	{
		if ( BufferStr1[i] != BufferStr2[i] ) { continue; }
		Cnt ++;		
	}	
	Score = Cnt*100.0/szString;
	return Score;
}
//----------------------------------------------------------------------------//
double MatchTwoStringW(LPCWSTR String1, LPCWSTR String2)
{
	double Score=0;
	if ( NULL == String1 || NULL == String2 ) { return false; }
	
	const size_t szString1 = ::wcslen(String1);
	const size_t szString2 = ::wcslen(String2);
	const size_t szString = MIN(szString1, szString2);
	if ( 0 == szString )
	{	return Score; }

	wchar_t BufferStr1[256]=L"";
	wchar_t BufferStr2[256]=L"";

	::wcscpy(BufferStr1, String1);
	::wcscpy(BufferStr2, String2);
	::_wcsupr(BufferStr1);
	::_wcsupr(BufferStr2);
	
	size_t  i=0;
	int     Cnt=0;
	for ( i=0; i<szString; i++ )
	{
		if ( BufferStr1[i] != BufferStr2[i] ) { continue; }
		Cnt ++;		
	}	
	Score = Cnt*100.0/szString;
	return Score;
}
//----------------------------------------------------------------------------//
FILE*  OpenReadFile(LPCTSTR pfilename, bool bUnicode)
{
	FILE *pfile = NULL;
	if ( false == bUnicode )
	{	pfile = ::_tfopen(pfilename, _T("r+")); }
	else
	{	pfile = ::_tfopen(pfilename, _T("r+, ccs=UNICODE")); }
	return pfile;
}
//----------------------------------------------------------------------------//
bool ModifyOpenFileMode_Read(TCHAR fileMode[])//取得寫檔的Unicode修飾詞
{
#if _MSC_VER >= VS_2008_NET
	::_tcscat(fileMode, _T(", ccs=UNICODE"));
#endif//_MSC_VER
	return true;
}
//----------------------------------------------------------------------------//
bool ModifyOpenFileMode_Write(TCHAR fileMode[])//取得讀檔的Unicode修飾詞	
{
#if _MSC_VER >= VS_2008_NET
	::_tcscat(fileMode, _T(", ccs=UTF-8"));//ccs=UTF-8, ccs=UTF-16LE
#endif//_MSC_VER
	return true;
}
//----------------------------------------------------------------------------//
bool string2lower(std::string &string)//字串轉小寫
{
	std::transform(string.begin(), string.end(), string.begin(), tolower);
	return true;
}
//----------------------------------------------------------------------------//
bool string2upper(std::string &string)//字串轉大寫		
{
	std::transform(string.begin(), string.end(), string.begin(), toupper);
	return true;
}
//----------------------------------------------------------------------------//
bool wstring2lower(std::wstring &string)//字串轉大寫
{
	std::transform(string.begin(), string.end(), string.begin(), towlower);
	return true;
}
//----------------------------------------------------------------------------//
bool wstring2upper(std::wstring &string)//字串轉小寫
{
	std::transform(string.begin(), string.end(), string.begin(), towupper);
	return true;
}
//----------------------------------------------------------------------------//
char* ReSizeList(size_t Need, std::vector<char> &List)//重設陣列大小
{	
	if ( Need < List.size() ) { return &(List[0]); }	
	size_t sz=MAX(List.size(), 32);
	size_t nTimes=(Need/sz)+1;
	sz *= nTimes;
	List.resize(sz);
	return &(List[0]);
}
//----------------------------------------------------------------------------//
wchar_t* ReSizeList(size_t Need, std::vector<wchar_t> &List)//重設陣列大小
{	
	if ( Need < List.size() ) { return &(List[0]); }	
	size_t sz=MAX(List.size(), 32);
	size_t nTimes=(Need/sz)+1;
	sz *= nTimes;
	List.resize(sz);
	return &(List[0]);
}
//----------------------------------------------------------------------------//
bool AddBackslash(char Buffer[], size_t BufferSize)//增加反斜線
{
	const size_t szTmp=1024;
	char BufferTmp[szTmp];
	const size_t Len=strlen(Buffer);
	if ( Len >= szTmp )
	{	return false; }

	size_t idx = 0;
	//::memset(whBufferTmp, 0x00, sizeof(whBufferTmp));
	::memcpy(BufferTmp, Buffer, sizeof(char)*(Len+1));	
	for ( size_t i=0; i<Len; i++ )
	{	
		if ( idx >= BufferSize )
		{	return false; }

		Buffer[idx] = BufferTmp[i];
		idx ++;
		if ( '\\'==BufferTmp[i] ) 
		{
			Buffer[idx] = BufferTmp[i];
			idx ++;
			continue; 
		}	
	}
	idx = idx;	
	return true;
}
//----------------------------------------------------------------------------//
bool AddBackslash(wchar_t Buffer[], size_t BufferSize)//增加反斜線
{
	const size_t szTmp=1024;
	wchar_t BufferTmp[szTmp];
	const size_t Len=wcslen(Buffer);
	if ( Len >= szTmp )
	{	return false; }

	size_t idx = 0;
	//::memset(whBufferTmp, 0x00, sizeof(whBufferTmp));
	::memcpy(BufferTmp, Buffer, sizeof(wchar_t)*(Len+1));	
	for ( size_t i=0; i<Len; i++ )
	{	
		if ( idx >= BufferSize )
		{	return false; }

		Buffer[idx] = BufferTmp[i];
		idx ++;
		if ( L'\\'==BufferTmp[i] ) 
		{
			Buffer[idx] = BufferTmp[i];
			idx ++;
			continue; 
		}	
	}
	idx = idx;
	return true;
}
//----------------------------------------------------------------------------//
int char2wstring(const char *String, std::wstring &result, UINT Code)//char to str::wstring
{
	result.clear();
	if (NULL == String) 
	{	return 0;	}
	//UINT Code = CP_ACP;//CP_UTF8
	int nNeedLen = MultiByteToWideChar(Code, 0, String, -1, NULL, 0);
	wchar_t *wsBuffer = new wchar_t[nNeedLen+1];
	if ( NULL == wsBuffer ) { return 0; }
	//int Len = MultiByteToWideChar(Code, MB_PRECOMPOSED, String, -1, wsBuffer, nNeedLen);
	int Len = MultiByteToWideChar(Code, 0, String, -1, wsBuffer, nNeedLen);
	result = wsBuffer;//result = std::wstring(wBuffer);
	delete[] wsBuffer; wsBuffer=NULL;
	return Len;
}
//----------------------------------------------------------------------------//
int wchar2string(const wchar_t *String, std::string &result, UINT Code)//wchar to str::string
{
	result.clear();
	if (NULL == String) 
	{	return 0;	}	
	//UINT Code = CP_ACP;//CP_UTF8;
	int nNeedLen = WideCharToMultiByte(Code, 0, String, -1, NULL, 0, NULL, false);//CP_UTF8

	char *sBuffer=new char[nNeedLen+1];
	if ( NULL == sBuffer )
	{	return 0; }

	//int Len = WideCharToMultiByte(Code, WC_COMPOSITECHECK, String, -1, sBuffer, nNeedLen, NULL, NULL);;
	int Len = WideCharToMultiByte(Code, 0, String, -1, sBuffer, nNeedLen, NULL, NULL);;
	sBuffer[Len] = '\0';
	result = sBuffer;//result = std::wstring(wBuffer);
	delete[] sBuffer; sBuffer=NULL;
	return Len;
}
//----------------------------------------------------------------------------//
int  wchar2char(const wchar_t *String, char whBuffer[], size_t BufferSize, UINT Code)//wchar_t to char
{
	if (NULL == String) 
	{	return 0;	}		
	int nNeedLen = WideCharToMultiByte(Code, 0, String, -1, NULL, 0, NULL, false);//CP_UTF8
	if ( nNeedLen > BufferSize )
	{	return 0; }
	//return WideCharToMultiByte(Code, WC_COMPOSITECHECK, String, -1, whBuffer, BufferSize, NULL, NULL);;
	return WideCharToMultiByte(Code, 0, String, -1, whBuffer, BufferSize, NULL, NULL);;
}
//----------------------------------------------------------------------------//
int char2wchar(const char *String, wchar_t whBuffer[], size_t BufferSize, UINT Code)//char to wchar_t
{
	if (NULL == String) 
	{	return 0;	}		
	int nNeedLen = MultiByteToWideChar(Code, 0, String, -1, NULL, 0);
	if ( nNeedLen > BufferSize )
	{	return 0; }
	//return MultiByteToWideChar(Code, MB_PRECOMPOSED, String, -1, whBuffer, nNeedLen);;
	return MultiByteToWideChar(Code, 0, String, -1, whBuffer, nNeedLen);;
}
//----------------------------------------------------------------------------//
bool TCHAR2string(LPCTSTR String, std::string &result, UINT Code)//TCHAR to std::string
{
	if ( NULL == String ) { return false; }
#ifdef UNICODE	
	JetAPI::wchar2string(String, result, Code);
#else
	result = String;	
#endif//UNICODE	
	return true;
}
//----------------------------------------------------------------------------//
bool TCHAR2wstring(LPCTSTR String, std::wstring &result, UINT Code)//TCHAR to std::wstring
{	
	if ( NULL == String ) { return false; }
#ifdef UNICODE	
	result = String;
#else
	JetAPI::char2wstring(String, result, Code);
#endif//UNICODE	
	return true;
}
//----------------------------------------------------------------------------//
bool TCHAR2char(LPCTSTR String, char chBuffer[], size_t BufferSize, UINT Code)//TCHAR to char	
{	
#ifdef UNICODE		
	int nNeedLen = WideCharToMultiByte(Code, 0, String, -1, NULL, 0, NULL, false);//CP_UTF8
	if ( nNeedLen > BufferSize )
	{	return false; }
	//::WideCharToMultiByte(Code, WC_COMPOSITECHECK, String, -1, chBuffer, nNeedLen, NULL, NULL);
	::WideCharToMultiByte(Code, 0, String, -1, chBuffer, nNeedLen, NULL, NULL);
#else
	::strcpy(chBuffer, String);
#endif//UNICODE
	return true;
}
//----------------------------------------------------------------------------//	
bool TCHAR2wchar(LPCTSTR String, wchar_t whBuffer[], size_t BufferSize, UINT Code)//TCHAR to wchar_t
{	
#ifdef UNICODE	
	::wcscpy(whBuffer, String);	
#else	
	int nNeedLen = MultiByteToWideChar(Code, 0, String, -1, NULL, 0);
	if ( nNeedLen > BufferSize )
	{	return false; }
	//MultiByteToWideChar(Code, MB_PRECOMPOSED, String, -1, whBuffer, nNeedLen);
	MultiByteToWideChar(Code, 0, String, -1, whBuffer, nNeedLen);
#endif//UNICODE
	return true;
}
//----------------------------------------------------------------------------//	
bool TCHARCopy(LPCTSTR TChar, char AChar[], size_t ACharSize, wchar_t WChar[], size_t WCharSize, UINT Code)//TCHAR to char and w_chart
{	
#ifdef UNICODE	
	::wcscpy(WChar, TChar);
	int nNeedLen = WideCharToMultiByte(Code, 0, TChar, -1, NULL, 0, NULL, false);//CP_UTF8
	if ( nNeedLen > ACharSize )
	{	return false; }
	//::WideCharToMultiByte(Code, WC_COMPOSITECHECK, TChar, -1, AChar, nNeedLen, NULL, NULL);
	::WideCharToMultiByte(Code, 0, TChar, -1, AChar, nNeedLen, NULL, NULL);
#else
	::strcpy(AChar, TChar);	
	int nNeedLen = MultiByteToWideChar(Code, 0, TChar, -1, NULL, 0);
	if ( nNeedLen > WCharSize )
	{	return false; }
	//::MultiByteToWideChar(Code, MB_PRECOMPOSED, TChar, -1, WChar, nNeedLen);
	::MultiByteToWideChar(Code, 0, TChar, -1, WChar, nNeedLen);
#endif//UNICODE	
	return true;
}
//----------------------------------------------------------------------------//	
bool AdjustTextA(LPCSTR ptext, char ReplaceCh, LPSTR pResult, size_t size)//將字元濾除
{
	size_t  idx=0;
	char TextFilter[32]={0};

	idx = 0;
	TextFilter[idx++] = '/';	
	TextFilter[idx++] = '\\';
	TextFilter[idx++] = '*';
	TextFilter[idx++] = '?';
	TextFilter[idx++] = '|';
	TextFilter[idx++] = '"';
	TextFilter[idx++] = ':';
	TextFilter[idx++] = '\n';
	TextFilter[idx++] = '\0';

	const size_t TextLen = ::strlen(ptext);
	const size_t FilterLen = ::strlen(TextFilter);
	const char SpecFilter = 0x0a;
	size_t i=0, j=0, k=0;

	k=0;
	pResult[0] = '\0';
	for ( i=0; i<TextLen; i++ )
	{
		if ( ptext[i] == SpecFilter ) 
		{ continue; }
		for ( j=0; j<FilterLen; j++ )
		{
			//判斷是否有相同字元
			if ( ptext[i] == TextFilter[j] )
			{	break;	}
		}

		//如果有相同字元的話, 則取代
		if ( j != FilterLen )
		{	pResult[k] = ReplaceCh;	}
		else
		{	pResult[k] = ptext[i]; }
		k ++;

		if ( k >= size )
		{	return false; }
	}
	pResult[k] = '\0';	
	return true;
}
//----------------------------------------------------------------------------//
bool AdjustTextW(LPCWSTR ptext, wchar_t ReplaceCh, LPWSTR pResult, size_t size)//將字元濾除
{
	size_t  idx=0;
	wchar_t TextFilter[32]={0};

	idx = 0;
	TextFilter[idx++] = L'/';	
	TextFilter[idx++] = L'\\';
	TextFilter[idx++] = L'*';
	TextFilter[idx++] = L'?';
	TextFilter[idx++] = L'|';
	TextFilter[idx++] = L'"';
	TextFilter[idx++] = L':';
	TextFilter[idx++] = L'\n';
	TextFilter[idx++] = L'\0';

	const size_t TextLen = ::wcslen(ptext);
	const size_t FilterLen = ::wcslen(TextFilter);	
	//const wchar_t SpecFilter = L(0x0a);
	const wchar_t SpecFilter = L' ';
	size_t i=0, j=0, k=0;
	
	k=0;
	pResult[0] = L'\0';
	for ( i=0; i<TextLen; i++ )
	{
		if ( ptext[i] == SpecFilter ) 
		{ continue; }
		for ( j=0; j<FilterLen; j++ )
		{
			//判斷是否有相同字元
			if ( ptext[i] == TextFilter[j] )
			{	break;	}
		}

		//如果有相同字元的話, 則取代
		if ( j != FilterLen )
		{	pResult[k] = ReplaceCh;	}
		else
		{	pResult[k] = ptext[i]; }
		k ++;

		if ( k >= size )
		{	return false; }
	}
	pResult[k] = L'\0';		
	return true;
}
//----------------------------------------------------------------------------//
bool AdjustTextS(LPCTSTR ptext, TCHAR ReplaceCh, CString &str)//將字元濾除
{
	const int szLen=256;
	TCHAR Buffer[szLen] = _T("");
	if ( AdjustTextT(ptext, ReplaceCh, Buffer, szLen) == false )
	{	return false; }
	str = Buffer;
	return true;
}
//----------------------------------------------------------------------------//
bool AdjustTextT(LPCTSTR ptext, TCHAR ReplaceCh, LPTSTR pResult, size_t size)//將字元濾除
{
	bool IsOK = true;
#ifdef UNICODE	
	IsOK = AdjustTextW(ptext, ReplaceCh, pResult, size);
#else
	IsOK = AdjustTextA(ptext, ReplaceCh, pResult, size);
#endif//UNICODE	
	return IsOK;
}
//----------------------------------------------------------------------------//	
bool FilerTextA(LPCSTR ptext, LPCSTR pTextFilter, LPSTR pResult, size_t size)//將字元濾除
{
	const size_t TextLen = ::strlen(ptext);
	const size_t FilterLen = ::strlen(pTextFilter);
	const char SpecFilter = 0x0a;
	size_t i=0, j=0, k=0;

	pResult[0] = '\0';
	for ( i=0; i<TextLen; i++ )
	{
		if ( ptext[i] == SpecFilter ) 
		{ continue; }
		for ( j=0; j<FilterLen; j++ )
		{
			//判斷是否有相同字元
			if ( ptext[i] == pTextFilter[j] )
			{	break;	}
		}

		if ( j != FilterLen )
		{ continue; }//如果有相同字元的話, 則繼續循環
		pResult[k] = ptext[i];
		k++;

		if ( k >= size )
		{	return false; }
	}
	pResult[k] = '\0';	
	return true;	
}
//----------------------------------------------------------------------------//
bool FilerTextW(LPCWSTR ptext, LPCWSTR pTextFilter, LPWSTR pResult, size_t size)//將字元濾除
{	
	const size_t TextLen = ::wcslen(ptext);
	const size_t FilterLen = ::wcslen(pTextFilter);
	const wchar_t SpecFilter = L' ';
	size_t i=0, j=0, k=0;
	pResult[0] = L'\0';
	for ( i=0; i<TextLen; i++ )
	{
		if ( ptext[i] == SpecFilter ) 
		{ continue; }
		for ( j=0; j<FilterLen; j++ )
		{
			//判斷是否有相同字元
			if ( ptext[i] == pTextFilter[j] )
			{	break;	}
		}

		if ( j != FilterLen )
		{ continue; }//如果有相同字元的話, 則繼續循環
		pResult[k] = ptext[i];
		k++;

		if ( k >= size )
		{	return false; }
	}
	pResult[k] = L'\0';		
	return true;
}
//----------------------------------------------------------------------------//
bool FilerTextS(LPCTSTR ptext, LPCTSTR pTextFilter, CString &str)//將字元濾除
{
	const int szLen=256;
	TCHAR Buffer[szLen] = _T("");
	if ( FilerTextT(ptext, pTextFilter, Buffer, szLen) == false )
	{	return false; }
	str = Buffer;
	return true;
}
//----------------------------------------------------------------------------//
bool FilerTextT(LPCTSTR ptext, LPCTSTR pTextFilter, LPTSTR pResult, size_t size)//將字元濾除
{
	bool IsOK = true;
#ifdef UNICODE	
	IsOK = FilerTextW(ptext, pTextFilter, pResult, size);
#else
	IsOK = FilerTextA(ptext, pTextFilter, pResult, size);
#endif//UNICODE	
	return IsOK;
}
//----------------------------------------------------------------------------//
bool  CheckJSONStringA(LPCSTR ptext)//確認JSON字串
{
	bool bRet=false;	
	std::string  str;	
	std::wstring wstr;	
	rapidjson::WDocument Doc;
	rapidjson::CJsonCtrl JSonCtrl;

	str  = "{\"Key\":\""+std::string(ptext)+std::string("\"}");
	if ( char2wstring(str.c_str(), wstr) == 0 )
	{	return false; }

	//initial
	Doc.SetObject();
	JSonCtrl.Set(&Doc);	
	return JSonCtrl.SetBuffer(wstr, Doc);
}
//----------------------------------------------------------------------------//
bool  CheckJSONStringW(LPCWSTR ptext)//確認JSON字串
{
	bool bRet=false;	
	std::wstring wstr;	
	rapidjson::WDocument Doc;
	rapidjson::CJsonCtrl JSonCtrl;

	wstr = L"{\"Key\":\""+std::wstring(ptext)+std::wstring(L"\"}");
	//initial
	Doc.SetObject();
	JSonCtrl.Set(&Doc);	
	return JSonCtrl.SetBuffer(wstr, Doc);
}
//----------------------------------------------------------------------------//
bool  CheckJSONStringT(LPCTSTR ptext)//確認JSON字串
{
#ifndef UNICODE
	return CheckJSONStringA(ptext);
#else
	return CheckJSONStringW(ptext);
#endif//UNICODE
	return true;
}
//----------------------------------------------------------------------------//
bool FilterJSONStringA(LPSTR ptext, char ch)//濾除JSON字串
{
	if ( NULL == ptext ) { return false; }
	size_t i=0;
	const size_t len=::strlen(ptext);
	for ( i=0; i<len; i++ )
	{
		if ( ptext[i] >= 0x20 ) { continue; }
		if ( 0x00 == ptext[i] ) { continue; }		
		ptext[i] = ch;
	}
	return true;
}
//----------------------------------------------------------------------------//
bool FilterJSONStringW(LPWSTR ptext, wchar_t ch)//濾除JSON字串
{
	if ( NULL == ptext ) { return false; }
	size_t i=0;
	const size_t len=::wcslen(ptext);
	for ( i=0; i<len; i++ )
	{
		if ( ptext[i] >= 0x20 ) { continue; }
		if ( 0x00 == ptext[i] ) { continue; }		
		ptext[i] = ch;
	}	
	return true;
}
//----------------------------------------------------------------------------//
bool FilterJSONStringT(LPTSTR ptext, TCHAR ch)//濾除JSON字串
{
#ifndef UNICODE
	return FilterJSONStringA(ptext, ch);
#else
	return FilterJSONStringW(ptext, ch);
#endif//UNICODE
	return true;
}
//----------------------------------------------------------------------------//
bool FilterJSONStringA(std::string &text, char ch)//濾除JSON字串
{
	size_t i=0;
	const size_t len=text.length();
	for ( i=0; i<len; i++ )
	{
		if ( text[i] >= 0x20 ) { continue; }
		if ( 0x00 == text[i] ) { continue; }		
		text[i] = ch;
	}	
	return true;
}
//----------------------------------------------------------------------------//
bool FilterJSONStringW(std::wstring &text, wchar_t ch)//濾除JSON字串
{
	size_t i=0;
	const size_t len=text.length();
	for ( i=0; i<len; i++ )
	{
		if ( text[i] >= 0x20 ) { continue; }
		if ( 0x00 == text[i] ) { continue; }		
		text[i] = ch;
	}	
	return true;
}
//----------------------------------------------------------------------------//
CString GetSpcBarcodeInternalCode()//取得SPC條碼內碼
{
#ifdef ODM_BRAND_VERSION
	CString Code;
	Code.Format(_T("@%s#"), AOI3D_VENDOR);
	return Code;
#endif//ODM_BRAND_VERSION
	return CString(_T("@JET#"));
}
//----------------------------------------------------------------------------//
CString AddSpcBarcodeInternalCode(LPCTSTR Barcode)//加入SPC條碼內碼
{
	CString str=Barcode;	
	if ( NULL == Barcode ) { return str; }	
	CString strCode=GetSpcBarcodeInternalCode();
	str.Format(_T("%s%s"), Barcode, strCode);
	return str;
}
//----------------------------------------------------------------------------//
CString AddKeyToErrorString(LPCTSTR Error, LPCTSTR Key)//在錯誤字串加入關鍵字0
{	
	CString Str=Error;
	if ( NULL == Key )
	{	return Str; }
	const size_t KeyLen=_tcslen(Key);
	if ( 0 == KeyLen )
	{	return Str; }
	if ( Str.Right(KeyLen).CompareNoCase(Key) == 0 )	
	{	return Str; }
	Str.Format(_T("%s %s"), Error, Key);
	return Str;
}
//----------------------------------------------------------------------------//
bool AddSubString(CString &FullStr, LPCTSTR SubStr, LPCTSTR Delimiter)//增加子字串至字串內
{
	if ( FullStr.GetLength() == 0 )
	{	FullStr = SubStr; }
	else
	{	FullStr += CString(Delimiter) + CString(SubStr);	}
	return true;
}
//----------------------------------------------------------------------------//
bool GetTwoLinesCrossPoint(double m1, double b1, double m2, double b2, double &x, double &y)//取得線對線的交點
{
	if ( m1 == m2 ) 
	{ 
		x = 0;
		y = 0;
		return false;
	}
	
	double dm = m1-m2;
	y = ( (m1*b2) - (m2*b1) )/dm;
	x = (b2-b1)/dm;
	return true;
}
//-----------------------------------------------------------------------------//
bool PointsToRect(const POINT &pt1, const POINT &pt2, RECT &Rect)//兩點合成一個矩形
{
	Rect.left   = __min(pt1.x, pt2.x);
	Rect.right  = __max(pt1.x, pt2.x);

	Rect.top    = __min(pt1.y, pt2.y);
	Rect.bottom = __max(pt1.y, pt2.y);
	return true;
}
//----------------------------------------------------------------------------//
bool PointsToRect(const TPOINT2D &pt1, const TPOINT2D &pt2, RECT &Rect)//兩點合成一個矩形
{
	Rect.left   = __min(pt1.x, pt2.x);
	Rect.right  = __max(pt1.x, pt2.x);

	Rect.top    = __min(pt1.y, pt2.y);
	Rect.bottom = __max(pt1.y, pt2.y);
	return true;
}
//----------------------------------------------------------------------------//
bool PointsToRect(const TPOINT2D &pt1, const TPOINT2D &pt2, TRECT4D &Rect)//兩點合成一個矩形
{
	Rect.left   = __min(pt1.x, pt2.x);
	Rect.right  = __max(pt1.x, pt2.x);

	Rect.top    = __min(pt1.y, pt2.y);
	Rect.bottom = __max(pt1.y, pt2.y);
	return true;
}
//----------------------------------------------------------------------------//
bool CornerPtToRect(const POINT CornerPt[], RECT &Rect)//四個端點合成一個矩形
{
	Rect.right  = Rect.left = CornerPt[0].x;
	Rect.bottom = Rect.top  = CornerPt[0].y;

	Rect.left   = __min(Rect.left, CornerPt[1].x);
	Rect.right  = __max(Rect.right, CornerPt[1].x);
	Rect.top    = __min(Rect.top, CornerPt[1].y);
	Rect.bottom = __max(Rect.bottom, CornerPt[1].y);

	Rect.left   = __min(Rect.left, CornerPt[2].x);
	Rect.right  = __max(Rect.right, CornerPt[2].x);
	Rect.top    = __min(Rect.top, CornerPt[2].y);
	Rect.bottom = __max(Rect.bottom, CornerPt[2].y);

	Rect.left   = __min(Rect.left, CornerPt[3].x);
	Rect.right  = __max(Rect.right, CornerPt[3].x);
	Rect.top    = __min(Rect.top, CornerPt[3].y);
	Rect.bottom = __max(Rect.bottom, CornerPt[3].y);
	return true;
}
//----------------------------------------------------------------------------//
bool CornerPtToRect(const TPOINT2D CornerPt[], RECT &Rect)//四個端點合成一個矩形	
{
	POINT CornerPt2[4];
	for ( int i=0; i<4; i++ )
	{	Point2DToPoint(CornerPt[i], CornerPt2[i]);	}
	return CornerPtToRect(CornerPt2, Rect);
}
//----------------------------------------------------------------------------//
bool CornerXYToRect(const double CornerX[], const double CornerY[], RECT &Rect)//四個端點合成一個矩形
{
	Rect.right  = Rect.left = (LONG)(CornerX[0]);
	Rect.bottom = Rect.top  = (LONG)(CornerY[0]);

	Rect.left   = (LONG)(__min(Rect.left, CornerX[1]));
	Rect.right  = (LONG)(__max(Rect.right, CornerX[1]));
	Rect.top    = (LONG)(__min(Rect.top, CornerY[1]));
	Rect.bottom = (LONG)(__max(Rect.bottom, CornerY[1]));

	Rect.left   = (LONG)(__min(Rect.left, CornerX[2]));
	Rect.right  = (LONG)(__max(Rect.right, CornerX[2]));
	Rect.top    = (LONG)(__min(Rect.top, CornerY[2]));
	Rect.bottom = (LONG)(__max(Rect.bottom, CornerY[2]));

	Rect.left   = (LONG)(__min(Rect.left, CornerX[3]));
	Rect.right  = (LONG)(__max(Rect.right, CornerX[3]));
	Rect.top    = (LONG)(__min(Rect.top, CornerY[3]));
	Rect.bottom = (LONG)(__max(Rect.bottom, CornerY[3]));
	return true;
}
//----------------------------------------------------------------------------//
bool CornerPtToRegion(const TPOINT2D CornerPt[], TREGION4D &Region)//四個端點合成一個矩形	
{
	Region.maxX = Region.minX = CornerPt[0].x;
	Region.maxY = Region.minY  = CornerPt[0].y;

	Region.minX  = __min(Region.minX, CornerPt[1].x);
	Region.maxX  = __max(Region.maxX, CornerPt[1].x);
	Region.minY  = __min(Region.minY, CornerPt[1].y);
	Region.maxY  = __max(Region.maxY, CornerPt[1].y);

	Region.minX  = __min(Region.minX, CornerPt[2].x);
	Region.maxX  = __max(Region.maxX, CornerPt[2].x);
	Region.minY  = __min(Region.minY, CornerPt[2].y);
	Region.maxY  = __max(Region.maxY, CornerPt[2].y);

	Region.minX  = __min(Region.minX, CornerPt[3].x);
	Region.maxX  = __max(Region.maxX, CornerPt[3].x);
	Region.minY  = __min(Region.minY, CornerPt[3].y);
	Region.maxY  = __max(Region.maxY, CornerPt[3].y);
	return true;
}
//----------------------------------------------------------------------------//
bool CornerXYToRegion(const double CornerX[], const double CornerY[], TREGION4D &Region)//四個端點合成一個矩形	
{
	Region.maxX = Region.minX = CornerX[0];
	Region.maxY = Region.minY = CornerY[0];

	Region.minX  = __min(Region.minX, CornerX[1]);
	Region.maxX  = __max(Region.maxX, CornerX[1]);
	Region.minY  = __min(Region.minY, CornerY[1]);
	Region.maxY  = __max(Region.maxY, CornerY[1]);

	Region.minX  = __min(Region.minX, CornerX[2]);
	Region.maxX  = __max(Region.maxX, CornerX[2]);
	Region.minY  = __min(Region.minY, CornerY[2]);
	Region.maxY  = __max(Region.maxY, CornerY[2]);

	Region.minX  = __min(Region.minX, CornerX[3]);
	Region.maxX  = __max(Region.maxX, CornerX[3]);
	Region.minY  = __min(Region.minY, CornerY[3]);
	Region.maxY  = __max(Region.maxY, CornerY[3]);
	return true;
}
//----------------------------------------------------------------------------//
bool CornerPt2DToCornerPt(const TPOINT2D dCornerPt[], POINT nCornerPt[])//四個端點資料轉換
{
	nCornerPt[0].x = JetAPI::ToInt(dCornerPt[0].x);	nCornerPt[0].y = JetAPI::ToInt(dCornerPt[0].y);
	nCornerPt[1].x = JetAPI::ToInt(dCornerPt[1].x);	nCornerPt[1].y = JetAPI::ToInt(dCornerPt[1].y);
	nCornerPt[2].x = JetAPI::ToInt(dCornerPt[2].x);	nCornerPt[2].y = JetAPI::ToInt(dCornerPt[2].y);
	nCornerPt[3].x = JetAPI::ToInt(dCornerPt[3].x);	nCornerPt[3].y = JetAPI::ToInt(dCornerPt[3].y);	
	return true;
}
//----------------------------------------------------------------------------//
void GetSystemLastError(CString &Str)//取得系統最新一次錯誤訊息
{
	LPVOID lpMsgBuf;
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
	);
	// Process any inserts in lpMsgBuf.
	// ...
	// Display the string.
	Str.Format(_T("%s"), lpMsgBuf);
	// Free the buffer.
	LocalFree( lpMsgBuf );
}
//----------------------------------------------------------------------------//
bool SendDebugString(LPCSTR lpszText)//送出DebugString
{		
#ifdef _SEND_DEBUG_STRING
	if ( NULL == lpszText ) { return false; }
	const int size = 512;
	const int len=::strlen(lpszText);
	#ifndef _UNICODE
		char lpszText2[size+1];
		if (size > len )
		{	::sprintf(lpszText2, "%s\n", lpszText); }
		else
		{
			::memcpy(lpszText2, lpszText, sizeof(char)*size); 
			lpszText2[size-1] = '\0';
		}
		OutputDebugString(lpszText2);
	#else
		wchar_t lpszText2[size+1];
		const int len2 = MIN(len, size);
		JetAPI::char2wchar(lpszText, lpszText2, size);
		lpszText2[len2]=L'\n';
		lpszText2[len2+1]=L'\0';		
		OutputDebugString(lpszText2);
	#endif//_UNICODE
#endif//_SEND_DEBUG_STRING
	return true;
}
//----------------------------------------------------------------------------//
bool SendDebugString(LPCWSTR lpszText)//送出DebugString
{		
#ifdef _SEND_DEBUG_STRING
	if ( NULL == lpszText ) { return false; }	
	const int size = 512;
	const int len=::wcslen(lpszText);
	#ifndef _UNICODE
		char lpszText2[size+1];
		const int len2 = MIN(len, size);
		JetAPI::wchar2char(lpszText, lpszText2, size);
		lpszText2[len2]='\n';
		lpszText2[len2+1]='\0';		
		OutputDebugString(lpszText2);
	#else
		wchar_t lpszText2[size+1];
		if (size > len )
		{	::swprintf(lpszText2, L"%s\n", lpszText); }
		else
		{	
			::memcpy(lpszText2, lpszText, sizeof(wchar_t)*size); 
			lpszText2[size-1] = L'\0';
		}
		OutputDebugString(lpszText2);
	#endif//_UNICODE
#endif//_SEND_DEBUG_STRING
	return true;
}
//----------------------------------------------------------------------------//
bool SendDebugString(std::string &str)//送出DebugString
{	
	return SendDebugString(str.c_str());
	/*
#ifdef _SEND_DEBUG_STRING
	if ( NULL == str.c_str() ) { return false; }
	#ifndef _UNICODE
		OutputDebugString(str.c_str());
	#else
		wchar_t lpszText2[512];
		JetAPI::char2wchar(str.c_str(), lpszText2, 512);
		OutputDebugString(lpszText2);	
	#endif//_UNICODE
#endif//_SEND_DEBUG_STRING
*/
	return true;	
}
//----------------------------------------------------------------------------//
bool SendDebugString(std::wstring &str)//送出DebugString
{
	return SendDebugString(str.c_str());
	/*
#ifdef _SEND_DEBUG_STRING
	if ( NULL == str.c_str() ) { return false; }
	#ifndef _UNICODE
		char lpszText2[512];
		JetAPI::wchar2char(lpszText, lpszText2, 512);
		OutputDebugString(lpszText2);
	#else
		OutputDebugString(str.c_str());
	#endif//_UNICODE
#endif//_SEND_DEBUG_STRING
*/
	return true;
}
//----------------------------------------------------------------------------//
int  ShowMessageBox(LPCSTR lpszText, UINT nType, UINT nIDHelp)//char
{
	AOIDataCollect.AddWaitUserInputCount();
	const int Ret=ShowMessageBoxFn(lpszText, nType, nIDHelp);
	AOIDataCollect.ReleaseWaitUserInputCount();
	if ( AOIDataCollect.CheckWaitUserInputCountZero() )
	{	AOIDataCollect.RegistUserLastInputTickCount();	}	
	return Ret;
}
//----------------------------------------------------------------------------//
int  ShowMessageBoxFn(LPCSTR lpszText, UINT nType, UINT nIDHelp)//char	
{
#ifndef _UNICODE
	return ::AfxMessageBox(lpszText, nType, nIDHelp);
#else
	return ::AfxMessageBox(CString(lpszText), nType, nIDHelp);
#endif
	return IDOK;
}
//----------------------------------------------------------------------------//
int  ShowMessageBox(LPCWSTR lpszText, UINT nType, UINT nIDHelp)//wchar_t
{
	AOIDataCollect.AddWaitUserInputCount();
	const int Ret=ShowMessageBoxFn(lpszText, nType, nIDHelp);
	AOIDataCollect.ReleaseWaitUserInputCount();
	if ( AOIDataCollect.CheckWaitUserInputCountZero() )
	{	AOIDataCollect.RegistUserLastInputTickCount();	}	
	return Ret;
}
//----------------------------------------------------------------------------//
int  ShowMessageBoxFn(LPCWSTR lpszText, UINT nType, UINT nIDHelp)//wchar_t
{
#ifndef _UNICODE
	return ::AfxMessageBox(CString(lpszText), nType, nIDHelp);
#else
	return ::AfxMessageBox(lpszText, nType, nIDHelp);
#endif
	return IDOK;	
}
//----------------------------------------------------------------------------//
int ShowMessageBox(const std::string &str, UINT nType, UINT nIDHelp)//char
{
	return ShowMessageBox(str.c_str(), nType, nIDHelp);
}
//----------------------------------------------------------------------------//
int ShowMessageBox(const std::wstring &wstr, UINT nType, UINT nIDHelp)//wchar_t
{
	return ShowMessageBox(wstr.c_str(), nType, nIDHelp);
}
//----------------------------------------------------------------------------//
bool Base36ToInt(LPCSTR str, int &Value)
{
	if ( NULL == str ) { return false; }
	const size_t len=::strlen(str);
	Value = 0;
	int Order=1;
	for ( size_t i=0; i<len; i++ )
	{
		int tmp=0;
		bool bContinue=true;
		const char ch=str[len-i-1];
		if ( ch>='0' && ch<='9' )
		{	tmp = ch-'0';	}
		else if ( ch>='a' && ch<='z' )
		{	tmp = ch-'a'+10;	}
		else if ( ch>='A' && ch<='Z' )
		{	tmp = ch-'A'+10;	}
		else
		{	continue; }
		Value += tmp*Order;
		Order *= 36;
	}
	return true;
}
//-----------------------------------------------------------------------------//
bool Base36ToInt(LPCWSTR str, int &Value)
{
	if ( NULL == str ) { return false; }
	const size_t len=::wcslen(str);
	Value = 0;
	int Order=1;
	for ( size_t i=0; i<len; i++ )
	{
		int tmp=0;
		bool bContinue=true;
		const wchar_t ch=str[len-i-1];
		if ( ch>=L'0' && ch<=L'9' )
		{	tmp = (ch-L'0');	}
		else if ( ch>=L'a' && ch<=L'z' )
		{	tmp = (ch-L'a')+10;	}
		else if ( ch>=L'A' && ch<=L'Z' )
		{	tmp = (ch-L'A')+10;	}
		else
		{	continue; }
		Value += tmp*Order;
		Order *= 36;
	}
	return true;
}
//-----------------------------------------------------------------------------//
bool ASCII_To_Int(LPCSTR Data,int *D_i,int Len_i)
{	
	for(int i=0;i<=Len_i/*8*/;i++)
	{
		if(*(Data+i)>='0' && *(Data+i)<='9')
		{ *(D_i+i)=*(Data+i)-0x30; }
		else
		{
			switch(*(Data+i))
			{
			case 'a':
			case 'A':
				*(D_i+i)=10;
				break;
			case 'b':
			case 'B':
				*(D_i+i)=11;
				break;
			case 'c':
			case 'C':
				*(D_i+i)=12;
				break;
			case 'd':
			case 'D':
				*(D_i+i)=13;
				break;
			case 'e':
			case 'E':
				*(D_i+i)=14;
				break;
			case 'f':
			case 'F':
				*(D_i+i)=15;
				break;
			}
		}
	}
	return true;
}
//-----------------------------------------------------------------------------//
bool ASCII_To_Int(LPCWSTR Data,int *D_i,int Len_i)
{	  
	for(int i=0;i<=Len_i/*8*/;i++)
	{
		if(*(Data+i)>=L'0' && *(Data+i)<=L'9')
		{ *(D_i+i)=*(Data+i)-L'0'; }
		else
		{
			switch(*(Data+i))
			{
			case L'a':
			case L'A':
				*(D_i+i)=10;
				break;
			case L'b':
			case L'B':
				*(D_i+i)=11;
				break;
			case L'c':
			case L'C':
				*(D_i+i)=12;
				break;
			case L'd':
			case L'D':
				*(D_i+i)=13;
				break;
			case L'e':
			case L'E':
				*(D_i+i)=14;
				break;
			case L'f':
			case L'F':
				*(D_i+i)=15;
				break;
			}
		}
	}
	return true;
}
//-----------------------------------------------------------------------------//
bool HexToInt(char c, BYTE &Value)
{
	bool IsOK = true;
	switch ( c )
	{
	case '0':	Value = 0;		break;		
	case '1':	Value = 1;		break;		
	case '2':	Value = 2;		break;
	case '3':	Value = 3;		break;
	case '4':	Value = 4;		break;
	case '5':	Value = 5;		break;
	case '6':	Value = 6;		break;
	case '7':	Value = 7;		break;
	case '8':	Value = 8;		break;
	case '9':	Value = 9;		break;
	case 'a':
	case 'A':	Value = 10;		break;
	case 'b':
	case 'B':	Value = 11;		break;
	case 'c':
	case 'C':	Value = 12;		break;
	case 'd':
	case 'D':	Value = 13;		break;
	case 'e':
	case 'E':	Value = 14;		break;
	case 'f':
	case 'F':	Value = 15;		break;
	default:	IsOK = false;	break;
	}
	return IsOK;
}
//-----------------------------------------------------------------------------//
bool HexToInt(wchar_t c, BYTE &Value)
{
	bool IsOK = true;
	switch ( c )
	{
	case L'0':	Value = 0;		break;		
	case L'1':	Value = 1;		break;		
	case L'2':	Value = 2;		break;
	case L'3':	Value = 3;		break;
	case L'4':	Value = 4;		break;
	case L'5':	Value = 5;		break;
	case L'6':	Value = 6;		break;
	case L'7':	Value = 7;		break;
	case L'8':	Value = 8;		break;
	case L'9':	Value = 9;		break;
	case L'a':
	case L'A':	Value = 10;		break;
	case L'b':
	case L'B':	Value = 11;		break;
	case L'c':
	case L'C':	Value = 12;		break;
	case L'd':
	case L'D':	Value = 13;		break;
	case L'e':
	case L'E':	Value = 14;		break;
	case L'f':
	case L'F':	Value = 15;		break;
	default:	IsOK =  false;	break;
	}
	return IsOK;
}
//-----------------------------------------------------------------------------//
void HexToInt(LPCSTR str, const int Length, int &Value)
{
	Value = 0;
	if( Length <= 0 )  { return; }

	int *D_i = new int[Length];
	::memset(D_i, 0, sizeof(int)*Length);
	JetAPI::ASCII_To_Int(str, D_i, Length);

	int i=0, j=0;
	int TempD = 0;
	for( i=0 ; i<Length ; i++ )
	{
		TempD = D_i[Length-1-i];

		for( j=0 ; j<i ; j++ )
		{
			TempD = TempD*16;
		}
		Value += TempD; 		
	}
	delete []D_i; D_i=NULL;
}
//-----------------------------------------------------------------------------//
void HexToInt(LPCWSTR str, const int Length, int &Value)
{
	Value = 0;
	if( Length <= 0 )  { return; }

	int *D_i = new int[Length];
	::memset(D_i, 0, sizeof(int)*Length);
	JetAPI::ASCII_To_Int(str, D_i, Length);

	int i=0, j=0;
	int TempD = 0;
	for( i=0 ; i<Length ; i++ )
	{
		TempD = D_i[Length-1-i];

		for( j=0 ; j<i ; j++ )
		{
			TempD = TempD*16;
		}
		Value += TempD; 		
	}
	delete []D_i; D_i=NULL;	
}
//-----------------------------------------------------------------------------//
bool HexToBin(char c, char Bin[])
{
	bool bException=false;
	switch ( c )
	{
	case '0': strcpy(Bin, "0000"); break;
	case '1': strcpy(Bin, "0001"); break;
	case '2': strcpy(Bin, "0010"); break;
	case '3': strcpy(Bin, "0011"); break;
	case '4': strcpy(Bin, "0100"); break;
	case '5': strcpy(Bin, "0101"); break;
	case '6': strcpy(Bin, "0110"); break;
	case '7': strcpy(Bin, "0111"); break;
	case '8': strcpy(Bin, "1000"); break;
	case '9': strcpy(Bin, "1001"); break;
	case 'a':
	case 'A': strcpy(Bin, "1010"); break;
	case 'b':
	case 'B': strcpy(Bin, "1011"); break;
	case 'c':
	case 'C': strcpy(Bin, "1100"); break;
	case 'd':
	case 'D': strcpy(Bin, "1101"); break;
	case 'e':
	case 'E': strcpy(Bin, "1110"); break;
	case 'f':
	case 'F': strcpy(Bin, "1111"); break;
	default:
		bException = true;
		break;
	}
	if ( true == bException )
	{	return false; }
	return true;
}
//-----------------------------------------------------------------------------//
bool HexToBin(wchar_t c, wchar_t Bin[])
{	
	bool bException=false;
	switch ( c )
	{
	case '0': wcscpy(Bin, L"0000"); break;
	case '1': wcscpy(Bin, L"0001"); break;
	case '2': wcscpy(Bin, L"0010"); break;
	case '3': wcscpy(Bin, L"0011"); break;
	case '4': wcscpy(Bin, L"0100"); break;
	case '5': wcscpy(Bin, L"0101"); break;
	case '6': wcscpy(Bin, L"0110"); break;
	case '7': wcscpy(Bin, L"0111"); break;
	case '8': wcscpy(Bin, L"1000"); break;
	case '9': wcscpy(Bin, L"1001"); break;
	case 'a':
	case 'A': wcscpy(Bin, L"1010"); break;
	case 'b':
	case 'B': wcscpy(Bin, L"1011"); break;
	case 'c':
	case 'C': wcscpy(Bin, L"1100"); break;
	case 'd':
	case 'D': wcscpy(Bin, L"1101"); break;
	case 'e':
	case 'E': wcscpy(Bin, L"1110"); break;
	case 'f':
	case 'F': wcscpy(Bin, L"1111"); break;
	default:
		bException = true;
		break;
	}
	if ( true == bException )
	{	return false; }
	return true;
}
//-----------------------------------------------------------------------------//
bool IntToHex(int Value, int Digits, char str[])
{
	bool IsOK = true;
	switch(Digits)
	{
	case 1: ::sprintf(str, "%.1X",Value);	break;
	case 2: ::sprintf(str, "%.2X",Value);	break;
	case 3: ::sprintf(str, "%.3X",Value);	break;
	case 4: ::sprintf(str, "%.4X",Value);	break;
	case 5: ::sprintf(str, "%.5X",Value);	break;
	case 6: ::sprintf(str, "%.6X",Value);	break;
	case 7: ::sprintf(str, "%.7X",Value);	break;
	case 8: ::sprintf(str, "%.8X",Value);	break;
	case 9: ::sprintf(str, "%.9X",Value);	break;
	case 10: ::sprintf(str,"%.10X",Value);	break;
	case 11: ::sprintf(str,"%.11X",Value);	break;
	case 12: ::sprintf(str,"%.12X",Value);	break;
	case 13: ::sprintf(str,"%.13X",Value);	break;
	case 14: ::sprintf(str,"%.14X",Value);	break;
	case 15: ::sprintf(str,"%.15X",Value);	break;
	case 16: ::sprintf(str,"%.16X",Value);	break;	
	default:	IsOK = false; break;
	}
	return IsOK;
}
//----------------------------------------------------------------------------//
bool IntToHex(int Value, int Digits, wchar_t str[])
{
	bool IsOK = true;
	switch(Digits)
	{
	case 1: ::swprintf(str, L"%.1X",Value);	break;
	case 2: ::swprintf(str, L"%.2X",Value);	break;
	case 3: ::swprintf(str, L"%.3X",Value);	break;
	case 4: ::swprintf(str, L"%.4X",Value);	break;
	case 5: ::swprintf(str, L"%.5X",Value);	break;
	case 6: ::swprintf(str, L"%.6X",Value);	break;
	case 7: ::swprintf(str, L"%.7X",Value);	break;
	case 8: ::swprintf(str, L"%.8X",Value);	break;
	case 9: ::swprintf(str, L"%.9X",Value);	break;
	case 10: ::swprintf(str,L"%.10X",Value);	break;
	case 11: ::swprintf(str,L"%.11X",Value);	break;
	case 12: ::swprintf(str,L"%.12X",Value);	break;
	case 13: ::swprintf(str,L"%.13X",Value);	break;
	case 14: ::swprintf(str,L"%.14X",Value);	break;
	case 15: ::swprintf(str,L"%.15X",Value);	break;
	case 16: ::swprintf(str,L"%.16X",Value);	break;	
	default:	IsOK = false; break;
	}
	return IsOK;
}
//----------------------------------------------------------------------------//
bool IntToDec(int Value, int Digits, char str[])//Decima-10進位
{
	bool IsOK = true;
	switch(Digits)
	{
	case 0: ::sprintf(str, "%d",Value);	break;
	case 1: ::sprintf(str, "%01d",Value);	break;
	case 2: ::sprintf(str, "%02d",Value);	break;
	case 3: ::sprintf(str, "%03d",Value);	break;
	case 4: ::sprintf(str, "%04d",Value);	break;
	case 5: ::sprintf(str, "%05d",Value);	break;
	case 6: ::sprintf(str, "%06d",Value);	break;
	case 7: ::sprintf(str, "%07d",Value);	break;
	case 8: ::sprintf(str, "%08d",Value);	break;
	case 9: ::sprintf(str, "%09d",Value);	break;
	case 10: ::sprintf(str,"%010d",Value);	break;
	case 11: ::sprintf(str,"%011d",Value);	break;
	case 12: ::sprintf(str,"%012d",Value);	break;
	case 13: ::sprintf(str,"%013d",Value);	break;
	case 14: ::sprintf(str,"%014d",Value);	break;
	case 15: ::sprintf(str,"%015d",Value);	break;
	case 16: ::sprintf(str,"%016d",Value);	break;	
	case 17: ::sprintf(str,"%017d",Value);	break;	
	case 18: ::sprintf(str,"%018d",Value);	break;	
	case 19: ::sprintf(str,"%019d",Value);	break;	
	case 20: ::sprintf(str,"%020d",Value);	break;	
	case 21: ::sprintf(str,"%021d",Value);	break;	
	case 22: ::sprintf(str,"%022d",Value);	break;	
	case 23: ::sprintf(str,"%023d",Value);	break;	
	case 24: ::sprintf(str,"%024d",Value);	break;	
	case 25: ::sprintf(str,"%025d",Value);	break;	
	case 26: ::sprintf(str,"%026d",Value);	break;	
	case 27: ::sprintf(str,"%027d",Value);	break;	
	case 28: ::sprintf(str,"%028d",Value);	break;	
	case 29: ::sprintf(str,"%029d",Value);	break;	
	case 30: ::sprintf(str,"%030d",Value);	break;	
	case 31: ::sprintf(str,"%031d",Value);	break;	
	case 32: ::sprintf(str,"%032d",Value);	break;	
	default:	IsOK = false; break;
	}
	return IsOK;
}
//----------------------------------------------------------------------------//
bool IntToDec(int Value, int Digits, wchar_t str[])//Decima-10進位
{
	bool IsOK = true;
	switch(Digits)
	{
	case 0: ::swprintf(str, L"%d",Value);	break;
	case 1: ::swprintf(str, L"%01d",Value);	break;
	case 2: ::swprintf(str, L"%02d",Value);	break;
	case 3: ::swprintf(str, L"%03d",Value);	break;
	case 4: ::swprintf(str, L"%04d",Value);	break;
	case 5: ::swprintf(str, L"%05d",Value);	break;
	case 6: ::swprintf(str, L"%06d",Value);	break;
	case 7: ::swprintf(str, L"%07d",Value);	break;
	case 8: ::swprintf(str, L"%08d",Value);	break;
	case 9: ::swprintf(str, L"%09d",Value);	break;
	case 10: ::swprintf(str,L"%010d",Value);	break;
	case 11: ::swprintf(str,L"%011d",Value);	break;
	case 12: ::swprintf(str,L"%012d",Value);	break;
	case 13: ::swprintf(str,L"%013d",Value);	break;
	case 14: ::swprintf(str,L"%014d",Value);	break;
	case 15: ::swprintf(str,L"%015d",Value);	break;
	case 16: ::swprintf(str,L"%016d",Value);	break;	
	case 17: ::swprintf(str,L"%017d",Value);	break;	
	case 18: ::swprintf(str,L"%018d",Value);	break;	
	case 19: ::swprintf(str,L"%019d",Value);	break;	
	case 20: ::swprintf(str,L"%020d",Value);	break;	
	case 21: ::swprintf(str,L"%021d",Value);	break;	
	case 22: ::swprintf(str,L"%022d",Value);	break;	
	case 23: ::swprintf(str,L"%023d",Value);	break;	
	case 24: ::swprintf(str,L"%024d",Value);	break;	
	case 25: ::swprintf(str,L"%025d",Value);	break;	
	case 26: ::swprintf(str,L"%026d",Value);	break;	
	case 27: ::swprintf(str,L"%027d",Value);	break;	
	case 28: ::swprintf(str,L"%028d",Value);	break;	
	case 29: ::swprintf(str,L"%029d",Value);	break;	
	case 30: ::swprintf(str,L"%030d",Value);	break;	
	case 31: ::swprintf(str,L"%031d",Value);	break;	
	case 32: ::swprintf(str,L"%032d",Value);	break;	
	default:	IsOK = false; break;
	}
	return IsOK;
}
//----------------------------------------------------------------------------//
bool IntToBin(int Value, int Digits, char str[])
{
	_itoa(Value, str, 2);
	if ( Digits >= 64 ) { return false; }

	char Buffer[64]="";
	const size_t uDigits = Digits;
	while ( ::strlen(str) < uDigits )
	{
		::strcpy(Buffer, str);
		::sprintf(str, "0%s", Buffer);
	};
	return true;
}
//----------------------------------------------------------------------------//
bool IntToBin(int Value, int Digits, wchar_t str[])
{
	_itow(Value, str, 2);
	if ( Digits >= 64 ) { return false; }

	wchar_t Buffer[64]=L"";
	const size_t uDigits = Digits;
	while ( ::wcslen(str) < uDigits )
	{
		::wcscpy(Buffer, str);
		::swprintf(str, L"0%s", Buffer);
	};
	return true;
}
//----------------------------------------------------------------------------//
bool IntToAny(int Value, int Digits, char str[], int base)//2~36進位
{	
	int n = Value;
	unsigned int idx=0;
	const int radix=base;
    do
	{
		int t=n%radix;
		if(t>=0&&t<=9)
		{	str[idx] += (t+'0');	}
        else
		{	str[idx] += (t-10+'A');	}
		idx ++;
        n /= radix;		
    }while(n!=0);
	for ( int i=idx; i<Digits; i++ )
	{	str[i] = '0';	}
	const int len=idx>Digits ? idx:Digits;
	const int len2=len/2;
	for ( int i=0; i<len2; i++ )
	{	Swap(str[i], str[len-i-1]);	}	
    return true;	
}
//----------------------------------------------------------------------------//
bool IntToAny(int Value, int Digits, wchar_t str[], int base)//2~36進位
{
	int n = Value;
	unsigned int idx=0;
	const int radix=base;
    do
	{
		int t=n%radix;
		if(t>=0&&t<=9)
		{	str[idx] += (t+L'0');	}
        else
		{	str[idx] += (t-10+L'A');	}
		idx ++;
        n /= radix;		
    }while(n!=0);
	for ( int i=idx; i<Digits; i++ )
	{	str[i] = L'0';	}
	const int len=idx>Digits ? idx:Digits;
	const int len2=len/2;
	for ( int i=0; i<len2; i++ )
	{	Swap(str[i], str[len-i-1]);	}	
    return true;		
}
//----------------------------------------------------------------------------//
bool CheckPtInCtrlWnd(CWnd *ParentWnd, POINT Pt, UINT CtrlID, POINT *Pt2)//確認點到該控制項
{
	if ( NULL == ParentWnd ) { return false; }
	CWnd *pWnd = ParentWnd->GetDlgItem(CtrlID);
	if ( (NULL==pWnd) || (NULL==pWnd->GetSafeHwnd()) )
	{	return false; }
	
	//ParentWnd->MapWindowPoints(pWnd, &Pt, 1);	
	ParentWnd->ClientToScreen(&Pt);
	pWnd->ScreenToClient(&Pt);
	if ( NULL != Pt2 )
	{	*Pt2 = Pt; }
	RECT Rect={0};
	pWnd->GetClientRect(&Rect);
	if ( ::PtInRect(&Rect, Pt) == FALSE )
	{	return false; }	
	return true;
}
//----------------------------------------------------------------------------//
CWnd* FocusCtrlWnd(CWnd *ParentWnd, UINT CtrlID)//焦點控制項
{
	if ( NULL == ParentWnd ) { return NULL; }
	CWnd  *pWnd = ParentWnd->GetDlgItem(CtrlID);
	if ( NULL==pWnd || NULL==pWnd->GetSafeHwnd() )
	{	return NULL; }	
	return pWnd->SetFocus();
}
//----------------------------------------------------------------------------//
bool ShowCtrlWnd(CWnd *ParentWnd, UINT CtrlID, BOOL bShow)//顯示控制項
{
	if ( NULL == ParentWnd ) { return false; }
	CWnd  *pWnd = ParentWnd->GetDlgItem(CtrlID);
	if ( NULL==pWnd || NULL==pWnd->GetSafeHwnd() )
	{	return false; }
	if ( TRUE == bShow )
	{	pWnd->ShowWindow(SW_SHOW);	}
	else
	{	pWnd->ShowWindow(SW_HIDE);	}	 
	return true;
}
//----------------------------------------------------------------------------//
bool EnableCtrlWnd(CWnd *ParentWnd, UINT CtrlID, BOOL bEnable)//啟用控制項
{
	if ( NULL == ParentWnd ) { return false; }
	CWnd  *pWnd = ParentWnd->GetDlgItem(CtrlID);
	if ( NULL==pWnd || NULL==pWnd->GetSafeHwnd() )
	{	return false; }
	pWnd->EnableWindow(bEnable); 
	return true;
}
//----------------------------------------------------------------------------//
bool EnableEditWnd(CWnd *ParentWnd, UINT CtrlID, BOOL bEnable)//啟用編輯控制項
{
	if ( NULL == ParentWnd ) { return false; }	
	CEdit *pEdit = (CEdit*)ParentWnd->GetDlgItem(CtrlID);
	if ( NULL==pEdit || NULL==pEdit->GetSafeHwnd() )
	{	return false; }

	//注意MFC類別的實作是使用視窗訊息傳遞, 因此跟是否為CEdit類別無關
	if ( TRUE == bEnable )
	{	pEdit->SetReadOnly(FALSE); }
	else
	{	pEdit->SetReadOnly(TRUE); }
	return true;
}
//----------------------------------------------------------------------------//
bool CheckRadioWnd(CWnd *ParentWnd, UINT CtrlID, UINT ActCtrlID)//啟用Radio控制項
{
	if ( NULL == ParentWnd ) { return false; }
	if ( CtrlID == ActCtrlID ) 
	{	ParentWnd->CheckDlgButton(CtrlID, TRUE); }
	else
	{	ParentWnd->CheckDlgButton(CtrlID, FALSE); }
	return true;
}
//----------------------------------------------------------------------------//
bool MoveCtrlWnd(CWnd *ParentWnd, UINT CtrlID, const RECT &Rect)//移動控制項
{
	if ( NULL == ParentWnd ) { return false; }	
	CWnd *pWnd = (CWnd*)ParentWnd->GetDlgItem(CtrlID);
	if ( NULL==pWnd || NULL==pWnd->GetSafeHwnd() )
	{	return false; }
	pWnd->MoveWindow(&Rect);
	return true;
}
//----------------------------------------------------------------------------//
bool MoveCtrlWnd(CWnd *ParentWnd, UINT CtrlID, const POINT &Offset)//移動控制項
{
	if ( NULL == ParentWnd ) { return false; }	
	CWnd *pWnd = (CWnd*)ParentWnd->GetDlgItem(CtrlID);
	if ( NULL==pWnd || NULL==pWnd->GetSafeHwnd() )
	{	return false; }
	
	RECT  WndRect={0};
	pWnd->GetWindowRect(&WndRect);
	ParentWnd->ScreenToClient(&WndRect);
	::OffsetRect(&WndRect, Offset.x, Offset.y);		
	pWnd->MoveWindow(&WndRect);
	return true;
}
//----------------------------------------------------------------------------//
bool GetDCUnityTransform(XFORM &xForm)//取得DC的座標-單位矩陣
{
	xForm.eM11 = (FLOAT) 1.0; 
    xForm.eM12 = (FLOAT) 0.0; 
    xForm.eM21 = (FLOAT) 0.0; 
    xForm.eM22 = (FLOAT) 1.0; 
    xForm.eDx  = (FLOAT) 0.0; 
    xForm.eDy  = (FLOAT) 0.0; 
	return true;
}
//----------------------------------------------------------------------------//
bool RotateDCTransform(double Angle, double X, double Y, XFORM &xForm)//旋轉DC的座標矩陣
{
	const double AngleRadius = Angle*DEG_TO_RAD_DBL;
	const FLOAT COS = (FLOAT)(::cos(AngleRadius));  
	const FLOAT SIN = (FLOAT)(::sin(AngleRadius));  

	xForm.eM11 = (FLOAT) (COS); 
	xForm.eM12 = (FLOAT) (SIN);
	xForm.eM21 = (FLOAT) (-SIN);
	xForm.eM22 = (FLOAT) (COS);
	xForm.eDx  = (FLOAT) (X); 
	xForm.eDy  = (FLOAT) (Y); 		
	return true;
}
//----------------------------------------------------------------------------//
bool Size2DToSize(const TSIZE2D &Size, SIZE &Sz)//尺寸資料轉換
{
	Sz.cx = JetAPI::ToInt(Size.cx);
	Sz.cy = JetAPI::ToInt(Size.cy);
	return true;
}
//----------------------------------------------------------------------------//
bool Point2DToPoint(const TPOINT2D &Point, POINT &Pt)//點資料轉換
{
	Pt.x = JetAPI::ToInt(Point.x);
	Pt.y = JetAPI::ToInt(Point.y);
	return true;
}
//----------------------------------------------------------------------------//
bool PointsSize(double *PtX, double *PtY, int NPts, double &CW, double &CH)//點群尺寸範圍
{
	if ( 0 == NPts ) { return false; }
	if ( NULL==PtX || NULL==PtY ) { return false; }

	int i=0;
	double MinX=0;
	double MaxX=0;
	double MinY=0;
	double MaxY=0;

	CW  = 0;
	CH = 0;
	MinX = MaxX = PtX[0];
	MinY = MaxY = PtY[0];

	for ( i=1; i<NPts; i++ )
	{
		if ( MinX > PtX[i] ) { MinX = PtX[i]; }
		if ( MinY > PtY[i] ) { MinY = PtY[i]; }

		if ( MaxX < PtX[i] ) { MaxX = PtX[i]; }
		if ( MaxY < PtY[i] ) { MaxY = PtY[i]; }
	}

	CW = (MaxX-MinX);
	CH = (MaxY-MinY);
	return true;
}
//----------------------------------------------------------------------------//
bool PointsCenter(const TPOINT2D *PTList, int NPTs, TPOINT2D &Cp)//點群中間座標	
{
	if ( 0 == NPTs ) { return false; }
	if ( NULL==PTList ) { return false; }

	int i=0;
	TREGION4D Rgn;
	Cp.x = 0;
	Cp.y = 0;
	Rgn.minX = Rgn.maxX = PTList[0].x;
	Rgn.minY = Rgn.maxY = PTList[0].y;
	for ( i=1; i<NPTs; i++ )
	{
		if ( Rgn.minX > PTList[i].x ) { Rgn.minX = PTList[i].x; }
		if ( Rgn.minY > PTList[i].y ) { Rgn.minY = PTList[i].y; }

		if ( Rgn.maxX < PTList[i].x ) { Rgn.maxX = PTList[i].x; }
		if ( Rgn.maxY < PTList[i].y ) { Rgn.maxY = PTList[i].y; }
	}

	Cp.x = (Rgn.maxX+Rgn.minX)/2;
	Cp.y = (Rgn.maxY+Rgn.minY)/2;
	return true;
}
//----------------------------------------------------------------------------//
bool PointsCenter(double *PtX, double *PtY, int NPts, double &CX, double &CY)//點群中間座標
{
	if ( 0 == NPts ) { return false; }
	if ( NULL==PtX || NULL==PtY ) { return false; }

	int i=0;
	double MinX=0;
	double MaxX=0;
	double MinY=0;
	double MaxY=0;

	CX  = 0;
	CY = 0;
	MinX = MaxX = PtX[0];
	MinY = MaxY = PtY[0];

	for ( i=1; i<NPts; i++ )
	{
		if ( MinX > PtX[i] ) { MinX = PtX[i]; }
		if ( MinY > PtY[i] ) { MinY = PtY[i]; }

		if ( MaxX < PtX[i] ) { MaxX = PtX[i]; }
		if ( MaxY < PtY[i] ) { MaxY = PtY[i]; }
	}

	CX = (MaxX+MinX)/2;
	CY = (MaxY+MinY)/2;
	return true;
}
//----------------------------------------------------------------------------//
void PointsToRect(const POINT *PTList, int NPTs, RECT &Rect)//將四個點合成一個矩形
{
	if ( NPTs == 0 ) { return; }
	if ( PTList == NULL ) { return; }
	int i=0;
	Rect.left   = PTList[0].x;
	Rect.top    = PTList[0].y;
	Rect.right  = PTList[0].x;
	Rect.bottom = PTList[0].y;

	for ( i=1; i<NPTs; i++ )
	{
		if ( Rect.left>PTList[i].x ) { Rect.left=PTList[i].x; }
		if ( Rect.top>PTList[i].y ) { Rect.top=PTList[i].y; }
		if ( Rect.right<PTList[i].x ) { Rect.right=PTList[i].x; }
		if ( Rect.bottom<PTList[i].y ) { Rect.bottom=PTList[i].y; }
	}	
}
//-----------------------------------------------------------------------------//
void PointsToRect(const TPOINT2D *PTList, int NPTs, TRECT4D &Rect)//將四個點合成一個矩形
{
	if ( NPTs == 0 ) { return; }
	if ( PTList == NULL ) { return; }
	int i=0;
	Rect.left   = PTList[0].x;
	Rect.top    = PTList[0].y;
	Rect.right  = PTList[0].x;
	Rect.bottom = PTList[0].y;

	for ( i=1; i<NPTs; i++ )
	{
		if ( Rect.left>PTList[i].x ) { Rect.left=PTList[i].x; }
		if ( Rect.top>PTList[i].y ) { Rect.top=PTList[i].y; }
		if ( Rect.right<PTList[i].x ) { Rect.right=PTList[i].x; }
		if ( Rect.bottom<PTList[i].y ) { Rect.bottom=PTList[i].y; }
	}	
}
//-----------------------------------------------------------------------------//
void PointListToRect(const std::vector<POINT> &PtList, RECT &Rect)//將點合成一個矩形	
{	
	const size_t Count=PtList.size();
	if ( 0 == Count ) { return; }

	size_t i=0;
	Rect.left   = PtList[0].x;
	Rect.top    = PtList[0].y;
	Rect.right  = PtList[0].x;
	Rect.bottom = PtList[0].y;
	for ( i=1; i<Count; i++ )
	{
		if ( Rect.left>PtList[i].x ) { Rect.left=PtList[i].x; }
		if ( Rect.top>PtList[i].y ) { Rect.top=PtList[i].y; }
		if ( Rect.right<PtList[i].x ) { Rect.right=PtList[i].x; }
		if ( Rect.bottom<PtList[i].y ) { Rect.bottom=PtList[i].y; }
	}	
	return;
}
//-----------------------------------------------------------------------------//
void PointListToRect(const std::vector<TPOINT2D> &PtList, TRECT4D &Rect)//將點合成一個矩形	
{
	const size_t Count=PtList.size();
	if ( 0 == Count ) { return; }

	size_t i=0;
	Rect.left   = PtList[0].x;
	Rect.top    = PtList[0].y;
	Rect.right  = PtList[0].x;
	Rect.bottom = PtList[0].y;
	for ( i=1; i<Count; i++ )
	{
		if ( Rect.left>PtList[i].x ) { Rect.left=PtList[i].x; }
		if ( Rect.top>PtList[i].y ) { Rect.top=PtList[i].y; }
		if ( Rect.right<PtList[i].x ) { Rect.right=PtList[i].x; }
		if ( Rect.bottom<PtList[i].y ) { Rect.bottom=PtList[i].y; }
	}	
	return;
}
//-----------------------------------------------------------------------------//
void    PointsToRegion(const TPOINT2D &Pt1, const TPOINT2D &Pt2, TREGION4D &Region)//將2個點合成一個矩形
{
	Region.minX = MIN(Pt1.x, Pt2.x);
	Region.minY = MIN(Pt1.y, Pt2.y);
	Region.maxX = MAX(Pt1.x, Pt2.x);
	Region.maxY = MAX(Pt1.y, Pt2.y);
}
//-----------------------------------------------------------------------------//
void    PointsToRegion(const TPOINT2D *PTList, int NPTs, TREGION4D &Region)//將四個點合成一個矩形
{
	if ( NPTs == 0 ) { return; }
	if ( PTList == NULL ) { return; }
	int i=0;
	Region.minX = PTList[0].x;
	Region.minY = PTList[0].y;
	Region.maxX = PTList[0].x;
	Region.maxY = PTList[0].y;

	for ( i=1; i<NPTs; i++ )
	{
		if ( Region.minX>PTList[i].x ) { Region.minX=PTList[i].x; }
		if ( Region.minY>PTList[i].y ) { Region.minY=PTList[i].y; }
		if ( Region.maxX<PTList[i].x ) { Region.maxX=PTList[i].x; }
		if ( Region.maxY<PTList[i].y ) { Region.maxY=PTList[i].y; }
	}
}
//-----------------------------------------------------------------------------//
void    PointsToRegion(const TPOINT3D *PTList, int NPTs, TREGION4D &Region)//將四個點合成一個矩形
{
	if ( NPTs == 0 ) { return; }
	if ( PTList == NULL ) { return; }
	int i=0;
	Region.minX = PTList[0].x;
	Region.minY = PTList[0].y;
	Region.maxX = PTList[0].x;
	Region.maxY = PTList[0].y;

	for ( i=1; i<NPTs; i++ )
	{
		if ( Region.minX>PTList[i].x ) { Region.minX=PTList[i].x; }
		if ( Region.minY>PTList[i].y ) { Region.minY=PTList[i].y; }
		if ( Region.maxX<PTList[i].x ) { Region.maxX=PTList[i].x; }
		if ( Region.maxY<PTList[i].y ) { Region.maxY=PTList[i].y; }
	}
}
//-----------------------------------------------------------------------------//
bool GetRectSize(const RECT &Rect, SIZE &Size)
{
	Size.cx = Rect.right-Rect.left;
	Size.cy = Rect.bottom-Rect.top;
	return true;
}
//-----------------------------------------------------------------------------//
bool  GetRectSize(const RECT &Rect, int &nW, int &nH)
{
	nW = Rect.right-Rect.left;
	nH = Rect.bottom-Rect.top;
	return true;
}
//-----------------------------------------------------------------------------//
bool GetRectCenterPos(const RECT &Rect, POINT &Cp)
{
	Cp.x = (Rect.left+Rect.right)/2;
	Cp.y = (Rect.top+Rect.bottom)/2;
	return true;
}
//-----------------------------------------------------------------------------//
bool GetRectCenterPos(const RECT &Rect, int &CpX, int &CpY)
{
	CpX = (Rect.left+Rect.right)/2;
	CpY = (Rect.top+Rect.bottom)/2;
	return true;
}
//-----------------------------------------------------------------------------//
bool    RectInRect(const RECT &Rect, const RECT &rcMatrix)
{
	if ( Rect.left < rcMatrix.left ) { return false; }
	if ( Rect.top  < rcMatrix.top ) { return false; }
	if ( Rect.right > rcMatrix.right ) { return false; }
	if ( Rect.bottom > rcMatrix.bottom ) { return false; }
	return true;
}
//-----------------------------------------------------------------------------//
bool PtInRect(const TPOINT2D &pt, const TRECT4D &Rect)
{
	if ( pt.x < Rect.left ) { return false; }
	if ( pt.y < Rect.top  ) { return false; }
	if ( pt.x > Rect.right  ) { return false; }
	if ( pt.y > Rect.bottom  ) { return false; }
	return true;
}
//-----------------------------------------------------------------------------//
bool PtInRegion(const TPOINT2D &pt, const TREGION4D &Region)
{
	if ( pt.x < Region.minX ) { return false; }
	if ( pt.y < Region.minY  ) { return false; }
	if ( pt.x > Region.maxX  ) { return false; }
	if ( pt.y > Region.maxY  ) { return false; }
	return true;
}
//-----------------------------------------------------------------------------//
HWND GetAncestorHWnd(HWND hWnd)
{
	HWND AncestorHWnd = hWnd;
	while ( ::GetParent(AncestorHWnd) != NULL )
	{	AncestorHWnd = ::GetParent(AncestorHWnd); }
	return AncestorHWnd;
}
//-----------------------------------------------------------------------------//
bool SendFrameWndMessage(CWnd *pWnd, UINT message, WPARAM wParam, LPARAM lParam)//發送訊息
{
	if ( NULL==pWnd || NULL==pWnd->GetSafeHwnd() ) { return false; }
	CWnd *ParentWnd = pWnd->GetParentFrame();
	if ( NULL==ParentWnd || NULL==ParentWnd->GetSafeHwnd() ) { return false; }
	if ( ::SendMessage(ParentWnd->GetSafeHwnd(), message, wParam, lParam) == FALSE )
	{	return false; }
	return true;
}
//----------------------------------------------------------------------------//
bool PostFrameWndMessage(CWnd *pWnd, UINT message, WPARAM wParam, LPARAM lParam)//發送訊息
{
	if ( NULL==pWnd || NULL==pWnd->GetSafeHwnd() ) { return false; }
	CWnd *ParentWnd = pWnd->GetParentFrame();
	if ( NULL==ParentWnd || NULL==ParentWnd->GetSafeHwnd() ) { return false; }
	if ( ::PostMessage(ParentWnd->GetSafeHwnd(), message, wParam, lParam) == FALSE )
	{	return false; }
	return true;
}
//----------------------------------------------------------------------------//
void SleepTime(DWORD dwMilliseconds, bool bTickCheck)//暫停
{
	if ( 0 == dwMilliseconds ) { return ; }
	if ( false == bTickCheck )
	{	::Sleep(dwMilliseconds);	}
	else
	{
		DWORD TickCntd = 0;
		DWORD TickCnt2 = 0;
		DWORD TickCnt1 = ::GetTickCount();
		while ( true )
		{
			TickCnt2 = ::GetTickCount();
			TickCntd = TickCnt2-TickCnt1;
			if ( TickCntd < dwMilliseconds )
			{	continue; }
			break;
		};
	}
	return;
}
//----------------------------------------------------------------------------//
void RemoveWndBusy(HWND hWnd)//移除視窗忙碌
{	
	return;
	//以下做法都會有問題
	MSG  msg;
	BOOL bRet=FALSE;
	bRet = GetInputState();
	if ( FALSE == bRet )
	{	return; }
	//JetAPI::RemoveMessage(hWnd, WM_PAINT, WM_PAINT);	
	JetAPI::RemoveMessage(hWnd, WM_KEYFIRST, WM_KEYLAST);	
	JetAPI::RemoveMessage(hWnd, WM_MOUSEFIRST , WM_MOUSELAST);	
	return; 
	while ( (bRet=GetMessage(&msg, hWnd, 0, 0)) !=0 )	
	{
		if ( -1 == bRet )
		{	break; }
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		break;
	};
	return;
	JetAPI::RemoveMessage(hWnd, WM_MOVE, WM_SIZE);	
	//JetAPI::RemoveMessage(hWnd, WM_NCPAINT, WM_NCPAINT);		
	JetAPI::RemoveMessage(hWnd, WM_KEYFIRST, WM_KEYLAST);	
	JetAPI::RemoveMessage(hWnd, WM_MOUSEFIRST , WM_MOUSELAST);	
	return ;
}
//----------------------------------------------------------------------------//
int RemoveMessage(HWND hWnd, UINT MsgFirst, UINT MsgEnd)//清除特定視窗的訊息(從佇列清除)
{
	MSG  msg;
	int  Count=0;
	BOOL bGetMsg=TRUE;	
	while ( true )
	{
		bGetMsg = ::PeekMessage(&msg, hWnd, MsgFirst, MsgEnd, PM_REMOVE);
		if ( FALSE == bGetMsg )
		{	break; }
		else
		{	Count ++;	}
	};

	if ( Count > 0 )
	{	Count = Count; }
	return Count;
}
//----------------------------------------------------------------------------//
void SleepMessage(DWORD dwMilliseconds, BOOL SkipMsg, HWND hWnd)//訊息暫停, 可以移除訊息的暫停
{
	if ( TRUE == SkipMsg )
	{
		MSG    msg;
		DWORD  dwT1=0, dwT2=0, dwT=0;
		dwT2 = dwT1 = ::GetTickCount();
		while ( true )
		{
			dwT2 = ::GetTickCount();
			dwT = dwT2-dwT1;
			if ( dwT >= dwMilliseconds )
			{	break; }
			::PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE);
		};
	}
	else
	{	::Sleep(dwMilliseconds);	}
}
//----------------------------------------------------------------------------//
bool BoundaryRect(const RECT &Boundary, RECT &Rect)//調整Rect範圍
{
	if ( Rect.left < Boundary.left ) { Rect.left = Boundary.left; }
	if ( Rect.top < Boundary.top ) { Rect.top = Boundary.top; }
	if ( Rect.right > Boundary.right ) { Rect.right = Boundary.right; }
	if ( Rect.bottom > Boundary.bottom ) { Rect.bottom = Boundary.bottom; }
	return true;
}
//----------------------------------------------------------------------------//
bool  SizeToRect(unsigned int RectW, unsigned int RectH, RECT &Rect)//尺寸轉程Rect
{
	Rect.left = Rect.top = 0;
	Rect.right = RectW;
	Rect.bottom = RectH;
	return true;
}
//----------------------------------------------------------------------------//
bool BoundaryRect(unsigned int RectW, unsigned int RectH, RECT &Rect)//調整Rect範圍
{
	if ( Rect.left < 0 ) { Rect.left = 0; }
	if ( Rect.top < 0 ) { Rect.top = 0; }
	if ( Rect.right < 0 ) { Rect.right = 0; }
	if ( Rect.bottom < 0 ) { Rect.bottom = 0; }
	if ( Rect.left > RectW ) { Rect.left = RectW; }
	if ( Rect.right > RectW ) { Rect.right = RectW; }
	if ( Rect.top > RectH ) { Rect.top = RectH; }
	if ( Rect.bottom > RectH ) { Rect.bottom = RectH; }
	return true;
}
//----------------------------------------------------------------------------//
bool AdjustRect(const RECT RectSrc, RECT &RectDst)//調整區域重新判斷大小值
{
	RectDst.left = MIN(RectSrc.left, RectSrc.right);
	RectDst.right = MAX(RectSrc.left, RectSrc.right);
	RectDst.top = MIN(RectSrc.top, RectSrc.bottom);
	RectDst.bottom = MAX(RectSrc.top, RectSrc.bottom);
	return true;
}
//----------------------------------------------------------------------------//
bool  AdjustRect(const TRECT4D RectSrc, TRECT4D &RectDst)//調整區域重新判斷大小值
{
	RectDst.left = MIN(RectSrc.left, RectSrc.right);
	RectDst.right = MAX(RectSrc.left, RectSrc.right);
	RectDst.top = MIN(RectSrc.top, RectSrc.bottom);
	RectDst.bottom = MAX(RectSrc.top, RectSrc.bottom);
	return true;
}
//----------------------------------------------------------------------------//
bool CheckPickSelectMode(const TREGION4D &Rgn, double Range)//確認是否為點選模式
{
	//以25um為單位
	if ( (Rgn.maxX-Rgn.minX) > Range ) { return false; }
	if ( (Rgn.maxY-Rgn.minY) > Range ) { return false; }
	return true;	
}
//----------------------------------------------------------------------------//
bool AdjustRegion(const TREGION4D RgnSrc, TREGION4D &RgnDst)//調整區域重新判斷大小值
{	
	RgnDst.minX = MIN(RgnSrc.minX, RgnSrc.maxX);
	RgnDst.maxX = MAX(RgnSrc.minX, RgnSrc.maxX);
	RgnDst.minY = MIN(RgnSrc.minY, RgnSrc.maxY);
	RgnDst.maxY = MAX(RgnSrc.minY, RgnSrc.maxY);
	return true;
}
//----------------------------------------------------------------------------//
bool BoundaryRegion(const TREGION4D &BoundaryRgn, TREGION4D &Rgn)//局限區域範圍	
{
	if ( Rgn.minX < BoundaryRgn.minX ) { Rgn.minX = BoundaryRgn.minX; }
	if ( Rgn.minY < BoundaryRgn.minY ) { Rgn.minY = BoundaryRgn.minY; }
	if ( Rgn.maxX > BoundaryRgn.maxX ) { Rgn.maxX = BoundaryRgn.maxX; }
	if ( Rgn.maxY > BoundaryRgn.maxY ) { Rgn.maxY = BoundaryRgn.maxY; }
	return true;
}
//----------------------------------------------------------------------------//
bool Rect4DToRect(const TRECT4D &dRect, RECT &Rect)//將區域轉成Rect
{
	Rect.left   = JetAPI::ToInt(dRect.left);
	Rect.right  = JetAPI::ToInt(dRect.right);
	Rect.top    = JetAPI::ToInt(dRect.top);
	Rect.bottom = JetAPI::ToInt(dRect.bottom);
	return true;
}
//----------------------------------------------------------------------------//
bool Region4DToRect(const TREGION4D &Region, RECT &Rect, bool Recheck)//將區域轉成Rect
{
	if ( true == Recheck )
	{
		Rect.left   = JetAPI::ToInt(MIN(Region.minX, Region.maxX));
		Rect.right  = JetAPI::ToInt(MAX(Region.minX, Region.maxX));
		Rect.top    = JetAPI::ToInt(MIN(Region.minY, Region.maxY));
		Rect.bottom = JetAPI::ToInt(MAX(Region.minY, Region.maxY));
	}
	else
	{
		Rect.left   = JetAPI::ToInt(Region.minX);
		Rect.right  = JetAPI::ToInt(Region.maxX);
		Rect.top    = JetAPI::ToInt(Region.minY);
		Rect.bottom = JetAPI::ToInt(Region.maxY);
	}
	return true;
}
//----------------------------------------------------------------------------//
bool AdjustRectByAlignW(RECT &Rect, int nAlign)//調整Rect寬度至N倍數
{
	//nAlign = 100;
	int RectW = Rect.right-Rect.left;
	int TempW = RectW;
	int TempI = (TempW+nAlign-1);	
	TempI = JetAPI::Floor(TempI/nAlign);
	TempW = TempI*nAlign;
	int OffsetX=(TempW-RectW)/2;
	if ( Rect.left > OffsetX )
	{	Rect.left -= OffsetX; }
	Rect.right = Rect.left+TempW;
	return true;
}
//----------------------------------------------------------------------------//
bool RectToCornerPt(const RECT &Rect, TPOINT2D CornerPt[])//將區域轉成4端點
{
	CornerPt[0].x = Rect.left;	CornerPt[0].y = Rect.top;
	CornerPt[1].x = Rect.right;	CornerPt[1].y = Rect.top;
	CornerPt[2].x = Rect.right;	CornerPt[2].y = Rect.bottom;
	CornerPt[3].x = Rect.left;	CornerPt[3].y = Rect.bottom;
	return true;
}
//----------------------------------------------------------------------------//
bool Rect4DToCornerPt(const TRECT4D &dRect, TPOINT2D CornerPt[])//將區域轉成4端點
{
	CornerPt[0].x = dRect.left;		CornerPt[0].y = dRect.top;
	CornerPt[1].x = dRect.right;	CornerPt[1].y = dRect.top;
	CornerPt[2].x = dRect.right;	CornerPt[2].y = dRect.bottom;
	CornerPt[3].x = dRect.left;		CornerPt[3].y = dRect.bottom;
	return true;
}
//----------------------------------------------------------------------------//
bool CheckCornerInRect(const POINT Corner[], const RECT &Rect)//確認4端點在區域內
{
	if ( Corner[0].x<Rect.left || Corner[0].y<Rect.top || Corner[0].x>Rect.right || Corner[0].y>Rect.bottom ) { return false; }
	if ( Corner[1].x<Rect.left || Corner[1].y<Rect.top || Corner[1].x>Rect.right || Corner[1].y>Rect.bottom ) { return false; }
	if ( Corner[2].x<Rect.left || Corner[2].y<Rect.top || Corner[2].x>Rect.right || Corner[2].y>Rect.bottom ) { return false; }
	if ( Corner[3].x<Rect.left || Corner[3].y<Rect.top || Corner[3].x>Rect.right || Corner[3].y>Rect.bottom ) { return false; }		
	return true;
}
//----------------------------------------------------------------------------//
bool CheckCornerInRect(const TPOINT2D Corner[], const RECT &Rect)//確認4端點在區域內
{
	if ( Corner[0].x<Rect.left || Corner[0].y<Rect.top || Corner[0].x>Rect.right || Corner[0].y>Rect.bottom ) { return false; }
	if ( Corner[1].x<Rect.left || Corner[1].y<Rect.top || Corner[1].x>Rect.right || Corner[1].y>Rect.bottom ) { return false; }
	if ( Corner[2].x<Rect.left || Corner[2].y<Rect.top || Corner[2].x>Rect.right || Corner[2].y>Rect.bottom ) { return false; }
	if ( Corner[3].x<Rect.left || Corner[3].y<Rect.top || Corner[3].x>Rect.right || Corner[3].y>Rect.bottom ) { return false; }		
	return true;
}
//----------------------------------------------------------------------------//
bool ScaleCornerPosSize(double sx, double sy, TPOINT2D CornerPos[])//將4端點尺寸縮放	
{
	const double CpX=(CornerPos[0].x+CornerPos[1].x+CornerPos[2].x+CornerPos[3].x)/4;
	const double CpY=(CornerPos[0].y+CornerPos[1].y+CornerPos[2].y+CornerPos[3].y)/4;
	CornerPos[0].x = ((CornerPos[0].x-CpX)*sx)+CpX;
	CornerPos[0].y = ((CornerPos[0].y-CpY)*sy)+CpY;
	CornerPos[1].x = ((CornerPos[1].x-CpX)*sx)+CpX;
	CornerPos[1].y = ((CornerPos[1].y-CpY)*sy)+CpY;
	CornerPos[2].x = ((CornerPos[2].x-CpX)*sx)+CpX;
	CornerPos[2].y = ((CornerPos[2].y-CpY)*sy)+CpY;
	CornerPos[3].x = ((CornerPos[3].x-CpX)*sx)+CpX;
	CornerPos[3].y = ((CornerPos[3].y-CpY)*sy)+CpY;
	return true;
}
//----------------------------------------------------------------------------//
bool ScaleRect(const RECT &Rect, double sx, double sy, int Mode, RECT &rt)//將區域縮放	
{
	int CpX = (Rect.left+Rect.right)/2;
	int CpY = (Rect.top+Rect.bottom)/2;
	int SizeW = (int)((Rect.right-Rect.left)*sx);
	int SizeH = (int)((Rect.bottom-Rect.top)*sy);
	if ( SizeW < 2 )
	{	SizeW = Rect.right-Rect.left; }
	if ( SizeH < 2 )
	{	SizeH = Rect.bottom-Rect.top; }

	switch ( Mode )
	{
	case SCALE_REGION_BY_SIDE_MIN_X:
		rt.left   = Rect.left;
		rt.right  = rt.left+SizeW;
		rt.top    = CpY-(SizeH/2);
		rt.bottom = CpY+(SizeH/2);
		break;
	case SCALE_REGION_BY_SIDE_MIN_Y:
		rt.top    = Rect.top;
		rt.bottom = rt.top+SizeH;
		rt.left   = CpX-(SizeW*0.5);
		rt.right  = CpX+(SizeW*0.5);
		break;
	case SCALE_REGION_BY_SIDE_MAX_X:
		rt.right  = Rect.right;
		rt.left   = rt.right-SizeW;
		rt.top    = CpY-(SizeH/2);
		rt.bottom = CpY+(SizeH/2);
		break;
	case SCALE_REGION_BY_SIDE_MAX_Y:
		rt.bottom = Rect.bottom;
		rt.top    = rt.bottom-SizeH;
		rt.left   = CpX-(SizeW/2);
		rt.right  = CpX+(SizeW/2);
		break;
	case SCALE_REGION_BY_CORNER_LT:
		rt.left   = Rect.left;
		rt.top    = Rect.top;
		rt.right  = rt.left+SizeW;
		rt.bottom = rt.top+SizeH;
		break;
	case SCALE_REGION_BY_CORNER_RT:
		rt.right  = Rect.right;
		rt.top    = Rect.top;
		rt.left   = rt.right-SizeW;
		rt.bottom = rt.top+SizeH;
		break;
	case SCALE_REGION_BY_CORNER_LB:
		rt.left   = Rect.left;
		rt.bottom = Rect.bottom;
		rt.right  = rt.left+SizeW;
		rt.top    = rt.bottom-SizeH;
		break;
	case SCALE_REGION_BY_CORNER_RB:
		rt.right  = Rect.right;
		rt.bottom = Rect.bottom;
		rt.left   = rt.right-SizeW;
		rt.top    = rt.bottom-SizeH;
		break;
	case SCALE_REGION_BY_CENTER:
	default:
		rt.left   = CpX-(SizeW/2);
		rt.top    = CpY-(SizeH/2);
		rt.right  = CpX+(SizeW/2);
		rt.bottom = CpY+(SizeH/2);
		break;
	}	
	return true;
}
//----------------------------------------------------------------------------//
bool ScaleRegion(const TREGION4D &Region, double sx, double sy, int Mode, TREGION4D &Rgn)//將區域縮放
{
	double CpX = Region.GetCpX();
	double CpY = Region.GetCpY();
	double SizeW = Region.GetWidth()*sx;
	double SizeH = Region.GetHeight()*sy;
	if ( SizeW < 2 )
	{	SizeW = Region.GetWidth(); }
	if ( SizeH < 2 )
	{	SizeH = Region.GetHeight(); }

	switch ( Mode )
	{
	case SCALE_REGION_BY_SIDE_MIN_X:
		Rgn.minX = Region.minX;
		Rgn.maxX = Rgn.minX+SizeW;
		Rgn.minY = CpY-(SizeH*0.5);
		Rgn.maxY = CpY+(SizeH*0.5);
		break;
	case SCALE_REGION_BY_SIDE_MIN_Y:
		Rgn.minY = Region.minY;
		Rgn.maxY = Rgn.minY+SizeH;
		Rgn.minX = CpX-(SizeW*0.5);
		Rgn.maxX = CpX+(SizeW*0.5);
		break;
	case SCALE_REGION_BY_SIDE_MAX_X:
		Rgn.maxX = Region.maxX;
		Rgn.minX = Rgn.maxX-SizeW;
		Rgn.minY = CpY-(SizeH*0.5);
		Rgn.maxY = CpY+(SizeH*0.5);
		break;
	case SCALE_REGION_BY_SIDE_MAX_Y:
		Rgn.maxY = Region.maxY;
		Rgn.minY = Rgn.maxY-SizeH;
		Rgn.minX = CpX-(SizeW*0.5);
		Rgn.maxX = CpX+(SizeW*0.5);
		break;
	case SCALE_REGION_BY_CORNER_LT:
		Rgn.minX = Region.minX;
		Rgn.minY = Region.minY;
		Rgn.maxX = Rgn.minX+SizeW;
		Rgn.maxY = Rgn.minY+SizeH;
		break;
	case SCALE_REGION_BY_CORNER_RT:
		Rgn.maxX = Region.maxX;
		Rgn.minY = Region.minY;
		Rgn.minX = Rgn.maxX-SizeW;
		Rgn.maxY = Rgn.minY+SizeH;
		break;
	case SCALE_REGION_BY_CORNER_LB:
		Rgn.minX = Region.minX;
		Rgn.maxY = Region.maxY;
		Rgn.maxX = Rgn.minX+SizeW;
		Rgn.minY = Rgn.maxY-SizeH;
		break;
	case SCALE_REGION_BY_CORNER_RB:
		Rgn.maxX = Region.maxX;
		Rgn.maxY = Region.maxY;
		Rgn.minX = Rgn.maxX-SizeW;
		Rgn.minY = Rgn.maxY-SizeH;
		break;
	case SCALE_REGION_BY_CENTER:
	default:
		Rgn.minX = CpX-(SizeW*0.5);
		Rgn.minY = CpY-(SizeH*0.5);
		Rgn.maxX = CpX+(SizeW*0.5);
		Rgn.maxY = CpY+(SizeH*0.5);
		break;
	}	
	return true;
}
//----------------------------------------------------------------------------//
bool CheckImageRoi(int ImageW, int ImageH, const RECT &RoiRect)//確認影像區域
{
	if ( RoiRect.left < 0 ) { return false; }
	if ( RoiRect.top < 0 ) { return false; }
	if ( RoiRect.right > ImageW ) { return false; }
	if ( RoiRect.bottom > ImageH ) { return false; }
	return true;
}
//----------------------------------------------------------------------------//
bool CheckImageRoi(unsigned int ImageW, unsigned int ImageH, const RECT &RoiRect)//確認影像區域
{
	if ( RoiRect.left < 0 ) { return false; }
	if ( RoiRect.top < 0 ) { return false; }
	if ( RoiRect.right > (int)(ImageW) ) { return false; }
	if ( RoiRect.bottom > (int)(ImageH) ) { return false; }
	if ( RoiRect.right == RoiRect.left ) { return false; }
	if ( RoiRect.bottom == RoiRect.top ) { return false; }
	return true;
}
//----------------------------------------------------------------------------//
void UnionRect(const RECT &Rect1, const RECT &Rect2, RECT &Rect)//兩個RECT的包含
{
	Rect.left   = MIN(Rect1.left, Rect2.left);
	Rect.top    = MIN(Rect1.top, Rect2.top);
	Rect.right  = MAX(Rect1.right, Rect2.right);
	Rect.bottom = MAX(Rect1.bottom, Rect2.bottom);
}
//----------------------------------------------------------------------------//
void IntersectRect(const RECT &Rect1, const RECT &Rect2, RECT &Rect)//兩個RECT的交合
{
	Rect.left   = MAX(Rect1.left, Rect2.left);
	Rect.top    = MAX(Rect1.top, Rect2.top);
	Rect.right  = MIN(Rect1.right, Rect2.right);
	Rect.bottom = MIN(Rect1.bottom, Rect2.bottom);
}
//----------------------------------------------------------------------------//
void UnionRegion(const TREGION4D &Rgn1, const TREGION4D &Rgn2, TREGION4D &Rgn)//兩個Region的包含
{
	Rgn.minX = MIN(Rgn1.minX, Rgn2.minX);
	Rgn.minY = MIN(Rgn1.minY, Rgn2.minY);
	Rgn.maxX = MAX(Rgn1.maxX, Rgn2.maxX);
	Rgn.maxY = MAX(Rgn1.maxY, Rgn2.maxY);
}
//----------------------------------------------------------------------------//
void IntersectRegion(const TREGION4D &Rgn1, const TREGION4D &Rgn2, TREGION4D &Rgn)//兩個Region的交合
{
	Rgn.minX = MAX(Rgn1.minX, Rgn2.minX);
	Rgn.minY = MAX(Rgn1.minY, Rgn2.minY);
	Rgn.maxX = MIN(Rgn1.maxX, Rgn2.maxX);
	Rgn.maxY = MIN(Rgn1.maxY, Rgn2.maxY);
}
//----------------------------------------------------------------------------//
void ExcludeRegion(const TREGION4D &Region, const TREGION4D &ExcRgn, BOX_TOWARD Toward, TREGION4D &Rgn)//Region剔除ExcRgn
{
	switch ( Toward )
	{
	case BOX_TOWARD_UP:
		Rgn.maxY = MIN(Region.maxY, ExcRgn.minY);
		break;
	case BOX_TOWARD_LEFT:
		Rgn.minX = MAX(Region.minX, ExcRgn.maxX);
		break;
	case BOX_TOWARD_DOWN:
		Rgn.minY = MAX(Region.minY, ExcRgn.maxY);
		break;
	case BOX_TOWARD_RIGHT:
		Rgn.maxX = MIN(Region.maxX, ExcRgn.minX);
		break;
	default:
		break;
	}
	return;
}
//----------------------------------------------------------------------------//
bool CheckPtInRegion(double Px, double Py, const TREGION4D &Rgn)//確認點在區域內
{
	if ( Px<Rgn.minX || Px>Rgn.maxX || Py<Rgn.minY || Py>Rgn.maxY ) { return false; }
	return true;
}
//----------------------------------------------------------------------------//
bool CheckRectInSize(const RECT &ObjRect, int SizeW, int SizeH)//確認區域在尺寸內	
{
	if ( ObjRect.left<0 || ObjRect.top<0 ) { return false; }
	if ( ObjRect.right>SizeW || ObjRect.bottom>SizeH ) { return false; }
	return true;
}
//----------------------------------------------------------------------------//
bool CheckRectInRect(const RECT &ObjRect, const RECT &BoundryRect, bool bEntireIn)//確認區域在區域內	
{
	if ( true == bEntireIn )
	{
		if ( ObjRect.right>BoundryRect.right || ObjRect.left<BoundryRect.left || 
			ObjRect.bottom>BoundryRect.bottom || ObjRect.top<BoundryRect.top )
		{	return false; }
		return true;
	}
	if ( ObjRect.right<BoundryRect.left || ObjRect.left>BoundryRect.right || 
		ObjRect.bottom<BoundryRect.top || ObjRect.top>BoundryRect.bottom )
	{	return false; }
	return true;
}
//----------------------------------------------------------------------------//
bool CheckRgnInRegion(const TREGION4D &ObjRgn, const TREGION4D &BoundryRgn, bool bEntireIn)//確認區域在區域內
{	
	if ( true == bEntireIn )
	{
		if ( ObjRgn.maxX>BoundryRgn.maxX || ObjRgn.minX<BoundryRgn.minX || 
			 ObjRgn.maxY>BoundryRgn.maxY || ObjRgn.minY<BoundryRgn.minY )
		{	return false; }
		return true;
	}
	if ( ObjRgn.maxX<BoundryRgn.minX || ObjRgn.minX>BoundryRgn.maxX || 
		 ObjRgn.maxY<BoundryRgn.minY || ObjRgn.minY>BoundryRgn.maxY )
	{	return false; }
	return true;
	/*
	if ( SelRgn.maxX<ObjRgn.minX || SelRgn.minX>ObjRgn.maxX || 
		 SelRgn.maxY<ObjRgn.minY || SelRgn.minY>ObjRgn.maxY )
	{	return false; }
	*/
	return true;
}
//----------------------------------------------------------------------------//
bool CalcRegionRect(const TREGION4D &BoundaryRgn, const RECT &BoundaryRect, const TREGION4D &Rgn, RECT &Rect, bool InverY)//將區域鏡射
{
	if ( Rgn.minX < BoundaryRgn.minX ) { return false; }
	if ( Rgn.minY < BoundaryRgn.minY ) { return false; }
	if ( Rgn.maxX > BoundaryRgn.maxX ) { return false; }
	if ( Rgn.maxY > BoundaryRgn.maxY ) { return false; }

	double       BoundaryScaleX = BoundaryRect.right-BoundaryRect.left;
	double       BoundaryScaleY = BoundaryRect.bottom-BoundaryRect.top;
	const double BoundaryRgnCpX = BoundaryRgn.GetCpX();
	const double BoundaryRgnCpY = BoundaryRgn.GetCpY();
	const double BoundaryRectCpX = (BoundaryRect.left+BoundaryRect.right)*0.5;
	const double BoundaryRectCpY = (BoundaryRect.top+BoundaryRect.bottom)*0.5;

	BoundaryScaleX /= BoundaryRgn.GetWidth();
	BoundaryScaleY /= BoundaryRgn.GetHeight();
	double RectMinY = 0;
	double RectMaxY = 0;
	double RectMinX = ((Rgn.minX-BoundaryRgnCpX)*BoundaryScaleX)+BoundaryRectCpX;	
	double RectMaxX = ((Rgn.maxX-BoundaryRgnCpX)*BoundaryScaleX)+BoundaryRectCpX;	
	if ( true == InverY )
	{
		RectMinY = BoundaryRectCpY-((Rgn.minY-BoundaryRgnCpY)*BoundaryScaleY);
		RectMaxY = BoundaryRectCpY-((Rgn.maxY-BoundaryRgnCpY)*BoundaryScaleY);
	}
	else
	{
		RectMinY = ((Rgn.minY-BoundaryRgnCpY)*BoundaryScaleY)+BoundaryRectCpY;
		RectMaxY = ((Rgn.maxY-BoundaryRgnCpY)*BoundaryScaleY)+BoundaryRectCpY;
	}

	int nMinX = JetAPI::ToInt(RectMinX);
	int nMinY = JetAPI::ToInt(RectMinY);
	int nMaxX = JetAPI::ToInt(RectMaxX);
	int nMaxY = JetAPI::ToInt(RectMaxY);
	Rect.left   = MIN(nMinX, nMaxX);
	Rect.top    = MIN(nMinY, nMaxY);
	Rect.right  = MAX(nMinX, nMaxX);
	Rect.bottom = MAX(nMinY, nMaxY);
	return true;
}
//----------------------------------------------------------------------------//
bool MapRegionToRect(const TREGION4D &Rgn1, const TREGION4D &Rgn2, const RECT &Rect1, RECT &Rect2)//區域映射
{
	double RatioX = (Rect1.right-Rect1.left)/(Rgn1.maxX-Rgn1.minX);
	double RatioY = (Rect1.bottom-Rect1.top)/(Rgn1.maxY-Rgn1.minY);

	Rect2.left  = (int)(Rect1.left+((Rgn2.minX-Rgn1.minX)*RatioX+0.5));
	Rect2.right = (int)(Rect1.left+((Rgn2.maxX-Rgn1.minX)*RatioX+0.5));
	Rect2.top   = (int)(Rect1.bottom-((Rgn2.maxY-Rgn1.minY)*RatioY-0.5));
	Rect2.bottom = (int)(Rect1.bottom-((Rgn2.minY-Rgn1.minY)*RatioY-0.5));

	if ( Rect2.top > Rect1.bottom ||
		 Rect2.bottom < Rect1.top ||
		 Rect2.left > Rect1.right ||
		 Rect2.right < Rect1.left )
	{
		Rect2.top = Rect2.bottom = 0;
		Rect2.left = Rect2.right = 0;
		return false;
	}
	
	if ( Rect2.left < Rect1.left ) { Rect2.left = Rect1.left; }
	if ( Rect2.top < Rect1.top ) { Rect2.top = Rect1.top; }
	if ( Rect2.right > Rect1.right ) { Rect2.right = Rect1.right; }
	if ( Rect2.bottom > Rect1.bottom ) { Rect2.bottom = Rect1.bottom; }
	return true;
}
//----------------------------------------------------------------------------//
bool MapRectToRegion(const RECT &Rect1, const RECT &Rect2, const TREGION4D &Rgn1, TREGION4D &Rgn2)//區域映射
{
	double RatioX = (Rgn1.maxX-Rgn1.minX)/(Rect1.right-Rect1.left);
	double RatioY = (Rgn1.maxY-Rgn1.minY)/(Rect1.bottom-Rect1.top);

	Rgn2.minX  = (Rgn1.minX+((Rect2.left-Rect1.left)*RatioX));
	Rgn2.maxX = (Rgn1.minX+((Rect2.right-Rect1.left)*RatioX));
	Rgn2.minY   = (Rgn1.maxY-((Rect2.bottom-Rect1.top)*RatioY));
	Rgn2.maxY = (Rgn1.maxY-((Rect2.top-Rect1.top)*RatioY));

	if ( Rgn2.minY > Rgn1.maxY ||
		 Rgn2.maxY < Rgn1.minY ||
		 Rgn2.minX > Rgn1.maxX ||
		 Rgn2.maxX < Rgn1.minX )
	{
		Rgn2.minY = Rgn2.maxY = 0;
		Rgn2.minX = Rgn2.maxX = 0;
		return false;
	}

	if ( Rgn2.minX < Rgn1.minX ) { Rgn2.minX = Rgn1.minX; }
	if ( Rgn2.minY < Rgn1.minY ) { Rgn2.minY = Rgn1.minY; }
	if ( Rgn2.maxX > Rgn1.maxX ) { Rgn2.maxX = Rgn1.maxX; }
	if ( Rgn2.maxY > Rgn1.maxY ) { Rgn2.maxY = Rgn1.maxY; }
	return true;
}
//----------------------------------------------------------------------------//
int GetTowardSize(double Scale)
{
	const int BaseSize = 10;//7
	const int NewSize = (int)(BaseSize/Scale);
	if ( NewSize < 2 ) { return 2; }
//	if ( NewSize > 8 ) { return 8; }
	return NewSize; 
}
//----------------------------------------------------------------------------//
int GetEditLineSize(double Scale, int Level)
{	
	int BaseSize = 4;//1.5;
	int NewSize = (int)(BaseSize/Scale);
	/*
	switch ( Level )
	{
	case 1:
		BaseSize = 2;
		NewSize = (int)(BaseSize/Scale);
		if ( NewSize < 3 ) { return 3; }
		if ( NewSize > 6 ) { return 6; }	
		break;
	case 2:
		BaseSize = 4;
		NewSize = (int)(BaseSize/Scale);
		if ( NewSize < 3 ) { return 3; }
		if ( NewSize > 10 ) { return 10; }	
		break;
	case 3:
		BaseSize = 6;
		NewSize = (int)(BaseSize/Scale);
		if ( NewSize < 3 ) { return 3; }
		if ( NewSize > 15 ) { return 15; }	
		break;
	}	
	*/	
	NewSize = GetEditCheckSize(Scale, Level);
	NewSize = NewSize/Scale;
	if ( NewSize < 1 ) { NewSize = 1; }
	return NewSize; 
}
//----------------------------------------------------------------------------//
int GetEditCheckSize(double Scale, int Level)//取得編輯確認的尺寸
{
	int BaseSize = 10;
	int NewSize = (int)(BaseSize/Scale);
	switch ( Level )
	{
	case 1:
		BaseSize = 6;//5;
		//NewSize = (int)(BaseSize*);
		//NewSize = (int)(BaseSize/Scale);
		NewSize = (int)(BaseSize*Scale);		
		if ( NewSize < 2 ) { return 2; }
		if ( NewSize > 12 ) { return 12; }	
		break;
	case 2:
		BaseSize = 8;
		//NewSize = (int)(BaseSize/Scale);
		NewSize = (int)(BaseSize*Scale);
		if ( NewSize < 2 ) { return 2; }
		if ( NewSize > 16 ) { return 16; }	
		break;
	case 3:
		BaseSize = 10;
		//NewSize = (int)(BaseSize/Scale);
		NewSize = (int)(BaseSize*Scale);
		if ( NewSize < 2 ) { return 2; }
		if ( NewSize > 20 ) { return 20; }	
		break;
	}	
	return NewSize; 
}
//----------------------------------------------------------------------------//
double  CalcBlobRatio(double BlobW, double BlobH, BOX_TOWARD Toward)
{
	double BlobAspectRatio = 1.0;
	if ( BOX_TOWARD_UP==Toward || BOX_TOWARD_DOWN==Toward )
	{	BlobAspectRatio = BlobW/BlobH;	}
	else
	{	BlobAspectRatio = BlobH/BlobW;	}		 
	return BlobAspectRatio;
}
//----------------------------------------------------------------------------//
int GetRandomValue()//取得隨機變數
{
	srand((unsigned)time( NULL ));
	return rand(); 
}
//----------------------------------------------------------------------------//
int GetRandomValue(int Mod)//取得隨機變數-餘數
{
	int val = GetRandomValue();
	return val%Mod;
}
//----------------------------------------------------------------------------//
int GetRandomValue(int Seed, int Mod)//取得隨機變數-餘數
{
	srand(Seed);
	return rand()%Mod; 	
}
//----------------------------------------------------------------------------//
double GetRandomValue(double Min, double Max)//取得隨機變數
{
	int val = GetRandomValue();
	double res = (val*(Max-Min)/(RAND_MAX+1.0))+Min;
	return res;
}
//----------------------------------------------------------------------------//
int   GetAngleLabel(double dAngle)//取得角度象限
{
	double Angle = JetAPI::RotateAngle(dAngle, 0);		
	if ( (Angle>45.0) && (Angle<135) ) 
	{	return 90; }
	if ( (Angle>=135.0) && (Angle<=225) ) 
	{	return 180; }
	if ( (Angle>225.0) && (Angle<315) ) 
	{	return 270; }
	else
	{	return 0; }
	
	
	//20180115
	/*
	if ( (Angle>=45.0) && (Angle<135.0) ) 
	{	return 90; }
	if ( (Angle>=135.0) && (Angle<225.0) ) 
	{	return 180; }
	if ( (Angle>=225.0) && (Angle<315.0) ) 
	{	return 270; }	
	*/
	return 0;
}
//----------------------------------------------------------------------------//
double AdjustRotationAngle(double Angle)//調整旋轉角度
{
	//餘數為+/-除數, 會有小於零的狀況
	double fAngle = ::fmod(Angle, 360.0);//角度, 注意回傳區間為-360 ~ 360
	if ( fAngle < 0.0 ) { fAngle += 360.0; }
	return fAngle;
}
//----------------------------------------------------------------------------//
bool  CheckIsExceptionAngle(double Angle)
{	
	Angle = AdjustRotationAngle(Angle);//角度, 注意回傳區間為-360 ~ 360
	if ( Angle < 0.1 ) { return false; }//0??
	if ( Angle> 89.9 && Angle <  90.1 ) { return false; }
	if ( Angle>179.9 && Angle < 180.1 ) { return false; }
	if ( Angle>269.9 && Angle < 270.1 ) { return false; }
	if ( Angle>359.9 && Angle < 360.1 ) { return false; }	
	return true; 
}
//-----------------------------------------------------------------------------//
double  MapCadAngleToImageAngle(double Angle)//將Cad的角度轉成圖像角度
{
	return 360.0-Angle;
}
//-----------------------------------------------------------------------------//
double  MapCadAngleToStageAngle(double Angle)//將Cad的角度轉成機台角度
{
	double NewAngle = Angle;
	const bool SignX = AOIDataCollect.GetStageSignPositiveX();
	const bool SignY = AOIDataCollect.GetStageSignPositiveY();

	if ( true == SignX )
	{
		if ( true == SignY ) 
		{	NewAngle = Angle;	}
		else
		{	NewAngle = 360.0-Angle;	}
	}
	else
	{
		if ( true == SignY ) 
		{	NewAngle = 180.0-Angle;	}
		else
		{	NewAngle = 180.0+Angle;	}
	}
	return NewAngle;	
}
//-----------------------------------------------------------------------------//
void RotateSize(double rAngle, SIZE &size)//尺寸旋轉
{
	LONG tempL = 0;
	const int AngleLabel = JetAPI::GetAngleLabel(rAngle);	
	switch ( AngleLabel )
	{
	case 90:		
	case 270:
		tempL = size.cx;
		size.cx = size.cy;
		size.cy = tempL;
		break;
	}
}
//-----------------------------------------------------------------------------//
void  RotateSize(double rAngle, TSIZE2D &size)//尺寸旋轉
{
	double tempD = 0;
	const int AngleLabel = JetAPI::GetAngleLabel(rAngle);	
	switch ( AngleLabel )
	{
	case 90:		
	case 270:
		tempD = size.cx;
		size.cx = size.cy;
		size.cy = tempD;
		break;
	}
}
//----------------------------------------------------------------------------//
void  RotateSize(double rAngle, double &cx, double &cy)//尺寸旋轉	
{
	double tempD = 0;
	const int AngleLabel = JetAPI::GetAngleLabel(rAngle);	
	switch ( AngleLabel )
	{
	case 90:		
	case 270:
		tempD = cx;
		cx = cy;
		cy = tempD;
		break;
	}
}
//----------------------------------------------------------------------------//
double  RotateAngle(double rAngle, double dAngle)
{
	double NewAngle = dAngle + rAngle;	
	NewAngle = AdjustRotationAngle(NewAngle);
	return NewAngle;
}
//-----------------------------------------------------------------------------//
void  RotatePos(double rAngle, double CPX, double CPY, TPOINT2D &Pos)//座標旋轉
{
	double DX = Pos.x-CPX;
	double DY = Pos.y-CPY;
	const double AngleR = rAngle*DEG_TO_RAD_DBL;//換算成徑度;
	const double COS = ::cos(AngleR);
	const double SIN = ::sin(AngleR);
	Pos.x = (DX*COS)-(DY*SIN)+CPX;
	Pos.y = (DX*SIN)+(DY*COS)+CPY;
}
//----------------------------------------------------------------------------//
void RotateRect(double rAngle, int W, int H, RECT &Rect)//區域旋轉
{	
	RECT      Rect2=Rect;	
	const int AngleLabel = JetAPI::GetAngleLabel(rAngle);	
	switch ( AngleLabel )
	{
	case 0:
		break;
	case 90:
		Rect.left = MIN(Rect2.top, Rect2.bottom);
		Rect.top  = MIN(Rect2.left, Rect2.right);
		Rect.right = MAX(Rect2.top, Rect2.bottom);
		Rect.bottom  = MAX(Rect2.left, Rect2.right);
		break;
	case 180:
		Rect.left = MIN(W-Rect2.left, W-Rect2.right);
		Rect.right = MAX(W-Rect2.left, W-Rect2.right);
		
		Rect.top  = MIN(H-Rect2.top, H-Rect2.bottom);		
		Rect.bottom  = MAX(H-Rect2.top, H-Rect2.bottom);
		break;
	case 270:
		Rect.left = MIN(Rect2.top, Rect2.bottom);		
		Rect.right = MAX(Rect2.top, Rect2.bottom);
		Rect.top  = MIN(W-Rect2.left, W-Rect2.right);
		Rect.bottom  = MAX(W-Rect2.left, W-Rect2.right);
		break;
	}
}
//----------------------------------------------------------------------------//
void  RotatePos(double rAngle, double CPX, double CPY, double &px, double &py)//座標旋轉
{
	double DX = px-CPX;
	double DY = py-CPY;
	const double AngleR = rAngle*DEG_TO_RAD_DBL;//換算成徑度;
	const double COS = ::cos(AngleR);
	const double SIN = ::sin(AngleR);
	px = (DX*COS)-(DY*SIN)+CPX;
	py = (DX*SIN)+(DY*COS)+CPY;
}
//----------------------------------------------------------------------------//
void RotatePosList(double Angle, double CPX, double CPY, std::vector<POINT> &PtList)//旋轉點列表
{
	size_t   i=0;
	TPOINT2D Point, Point2;		
	const size_t PtCnt=PtList.size();
	const double COS=::cos(Angle*DEG_TO_RAD_DBL);
	const double SIN=::sin(Angle*DEG_TO_RAD_DBL);	
	for ( i=0; i<PtCnt; i++ )
	{
		Point = PtList[i];
		Point.x -= CPX;
		Point.y -= CPY;			
		Point2.x = (Point.x*COS-Point.y*SIN);
		Point2.y = (Point.x*SIN+Point.y*COS);
		PtList[i].x = (int)(Point2.x+CPX);
		PtList[i].y = (int)(Point2.y+CPY);
	}
	return;
}
//----------------------------------------------------------------------------//
void RotatePosList(double Angle, double CPX, double CPY, std::vector<TPOINT2D> &PtList)//旋轉點列表
{
	size_t   i=0;
	TPOINT2D Point, Point2;		
	const size_t PtCnt=PtList.size();
	const double COS=::cos(Angle*DEG_TO_RAD_DBL);
	const double SIN=::sin(Angle*DEG_TO_RAD_DBL);	
	for ( i=0; i<PtCnt; i++ )
	{
		Point = PtList[i];
		Point.x -= CPX;
		Point.y -= CPY;			
		Point2.x = (Point.x*COS-Point.y*SIN);
		Point2.y = (Point.x*SIN+Point.y*COS);
		PtList[i].x = (Point2.x+CPX);
		PtList[i].y = (Point2.y+CPY);
	}
	return;
}
//----------------------------------------------------------------------------//
void RotatePos(double pX, double pY, double CpX, double CpY, double AngleRad, double &X, double &Y)//座標旋轉
{
	double dX = pX-CpX;
	double dY = pY-CpY;
	double dL = ::sqrt((dX*dX)+(dY*dY));
	double PosAngleRad=::atan2(dY, dX)+AngleRad;
	X = dL*cos(PosAngleRad)+CpX;
	Y = dL*sin(PosAngleRad)+CpY;
	return ;
}
//----------------------------------------------------------------------------//
void RotateRegion(double Angle, double CPX, double CPY, const TREGION4D &rgnSrc, TREGION4D &rgnDst)//區域旋轉
{	
	double CornerPosX[4];
	double CornerPosY[4];
	double dCornerPosX[4];
	double dCornerPosY[4];
	const double AngleR = Angle*DEG_TO_RAD_DBL;//換算成徑度;
	const double COS = ::cos(AngleR);
	const double SIN = ::sin(AngleR);

	dCornerPosX[0] = rgnSrc.minX - CPX;
	dCornerPosY[0] = rgnSrc.minY - CPY;
	dCornerPosX[1] = rgnSrc.maxX - CPX;
	dCornerPosY[1] = rgnSrc.minY - CPY;
	dCornerPosX[2] = rgnSrc.maxX - CPX;
	dCornerPosY[2] = rgnSrc.maxY - CPY;
	dCornerPosX[3] = rgnSrc.minX - CPX;
	dCornerPosY[3] = rgnSrc.maxY - CPY;	

	CornerPosX[0] = (dCornerPosX[0]*COS)-(dCornerPosY[0]*SIN)+CPX;
	CornerPosY[0] = (dCornerPosX[0]*SIN)+(dCornerPosY[0]*COS)+CPY;
	CornerPosX[1] = (dCornerPosX[1]*COS)-(dCornerPosY[1]*SIN)+CPX;
	CornerPosY[1] = (dCornerPosX[1]*SIN)+(dCornerPosY[1]*COS)+CPY;
	CornerPosX[2] = (dCornerPosX[2]*COS)-(dCornerPosY[2]*SIN)+CPX;
	CornerPosY[2] = (dCornerPosX[2]*SIN)+(dCornerPosY[2]*COS)+CPY;
	CornerPosX[3] = (dCornerPosX[3]*COS)-(dCornerPosY[3]*SIN)+CPX;
	CornerPosY[3] = (dCornerPosX[3]*SIN)+(dCornerPosY[3]*COS)+CPY;

	rgnDst.maxX = rgnDst.minX = CornerPosX[0];
	rgnDst.maxY = rgnDst.minY = CornerPosY[0];
	if ( rgnDst.minX > CornerPosX[1] ) { rgnDst.minX = CornerPosX[1]; }
	if ( rgnDst.minY > CornerPosY[1] ) { rgnDst.minY = CornerPosY[1]; }
	if ( rgnDst.maxX < CornerPosX[1] ) { rgnDst.maxX = CornerPosX[1]; }
	if ( rgnDst.maxY < CornerPosY[1] ) { rgnDst.maxY = CornerPosY[1]; }

	if ( rgnDst.minX > CornerPosX[2] ) { rgnDst.minX = CornerPosX[2]; }
	if ( rgnDst.minY > CornerPosY[2] ) { rgnDst.minY = CornerPosY[2]; }
	if ( rgnDst.maxX < CornerPosX[2] ) { rgnDst.maxX = CornerPosX[2]; }
	if ( rgnDst.maxY < CornerPosY[2] ) { rgnDst.maxY = CornerPosY[2]; }

	if ( rgnDst.minX > CornerPosX[3] ) { rgnDst.minX = CornerPosX[3]; }
	if ( rgnDst.minY > CornerPosY[3] ) { rgnDst.minY = CornerPosY[3]; }
	if ( rgnDst.maxX < CornerPosX[3] ) { rgnDst.maxX = CornerPosX[3]; }
	if ( rgnDst.maxY < CornerPosY[3] ) { rgnDst.maxY = CornerPosY[3]; }
	return;
}
//----------------------------------------------------------------------------//
void RotateCornerPos(double Angle, TPOINT2D CornerPos[])//角落座標旋轉
{
	TPOINT2D Cp;
	JetAPI::PointsCenter(CornerPos, 4, Cp);
	RotateCornerPos(Angle, Cp.x, Cp.y, CornerPos);
	return;
}
//----------------------------------------------------------------------------//
void RotateCornerPos(double Angle, double CPX, double CPY, TPOINT2D CornerPos[])//角落座標旋轉
{
	double dCornerPosX[4];
	double dCornerPosY[4];
	const double AngleR = Angle*DEG_TO_RAD_DBL;//換算成徑度;
	const double COS = ::cos(AngleR);
	const double SIN = ::sin(AngleR);

	dCornerPosX[0] = CornerPos[0].x - CPX;
	dCornerPosY[0] = CornerPos[0].y - CPY;
	dCornerPosX[1] = CornerPos[1].x - CPX;
	dCornerPosY[1] = CornerPos[1].y - CPY;
	dCornerPosX[2] = CornerPos[2].x - CPX;
	dCornerPosY[2] = CornerPos[2].y - CPY;
	dCornerPosX[3] = CornerPos[3].x - CPX;
	dCornerPosY[3] = CornerPos[3].y - CPY;
	
	CornerPos[0].x = (dCornerPosX[0]*COS)-(dCornerPosY[0]*SIN)+CPX;
	CornerPos[0].y = (dCornerPosX[0]*SIN)+(dCornerPosY[0]*COS)+CPY;
	CornerPos[1].x = (dCornerPosX[1]*COS)-(dCornerPosY[1]*SIN)+CPX;
	CornerPos[1].y = (dCornerPosX[1]*SIN)+(dCornerPosY[1]*COS)+CPY;
	CornerPos[2].x = (dCornerPosX[2]*COS)-(dCornerPosY[2]*SIN)+CPX;
	CornerPos[2].y = (dCornerPosX[2]*SIN)+(dCornerPosY[2]*COS)+CPY;
	CornerPos[3].x = (dCornerPosX[3]*COS)-(dCornerPosY[3]*SIN)+CPX;
	CornerPos[3].y = (dCornerPosX[3]*SIN)+(dCornerPosY[3]*COS)+CPY;
	return ;
}
//----------------------------------------------------------------------------//
void RotateCornerPos(double Angle, double CPX, double CPY, double CornerPosX[], double CornerPosY[])//角落座標旋轉
{
	double dCornerPosX[4];
	double dCornerPosY[4];
	const double AngleR = Angle*DEG_TO_RAD_DBL;//換算成徑度;
	const double COS = ::cos(AngleR);
	const double SIN = ::sin(AngleR);

	dCornerPosX[0] = CornerPosX[0] - CPX;
	dCornerPosY[0] = CornerPosY[0] - CPY;
	dCornerPosX[1] = CornerPosX[1] - CPX;
	dCornerPosY[1] = CornerPosY[1] - CPY;
	dCornerPosX[2] = CornerPosX[2] - CPX;
	dCornerPosY[2] = CornerPosY[2] - CPY;
	dCornerPosX[3] = CornerPosX[3] - CPX;
	dCornerPosY[3] = CornerPosY[3] - CPY;

	CornerPosX[0] = (dCornerPosX[0]*COS)-(dCornerPosY[0]*SIN)+CPX;
	CornerPosY[0] = (dCornerPosX[0]*SIN)+(dCornerPosY[0]*COS)+CPY;
	CornerPosX[1] = (dCornerPosX[1]*COS)-(dCornerPosY[1]*SIN)+CPX;
	CornerPosY[1] = (dCornerPosX[1]*SIN)+(dCornerPosY[1]*COS)+CPY;
	CornerPosX[2] = (dCornerPosX[2]*COS)-(dCornerPosY[2]*SIN)+CPX;
	CornerPosY[2] = (dCornerPosX[2]*SIN)+(dCornerPosY[2]*COS)+CPY;
	CornerPosX[3] = (dCornerPosX[3]*COS)-(dCornerPosY[3]*SIN)+CPX;
	CornerPosY[3] = (dCornerPosX[3]*SIN)+(dCornerPosY[3]*COS)+CPY;
	return ;
}
//----------------------------------------------------------------------------//
DWORD  RotateSideMode(double Angle, DWORD SideMode)
{
	if ( 0 == SideMode ) { return SideMode; }
	BOX_TOWARD TowardUp=BOX_TOWARD_NULL, TowardLeft=BOX_TOWARD_NULL, TowardDown=BOX_TOWARD_NULL, TowardRight=BOX_TOWARD_NULL;
	if ( 0 != (SideMode&BOX_TOWARD_UP) ) { TowardUp=JetAPI::RotateToward(Angle, BOX_TOWARD_UP); }
	if ( 0 != (SideMode&BOX_TOWARD_LEFT) ) { TowardLeft=JetAPI::RotateToward(Angle, BOX_TOWARD_LEFT); }
	if ( 0 != (SideMode&BOX_TOWARD_DOWN) ) { TowardDown=JetAPI::RotateToward(Angle, BOX_TOWARD_DOWN); }
	if ( 0 != (SideMode&BOX_TOWARD_RIGHT) ) { TowardRight=JetAPI::RotateToward(Angle, BOX_TOWARD_RIGHT); }
	if ( BOX_TOWARD_NULL==TowardUp || BOX_TOWARD_NULL==TowardLeft ||BOX_TOWARD_NULL==TowardDown ||BOX_TOWARD_NULL==TowardRight )
	{
		SideMode = 0;
		SideMode = TowardUp|TowardLeft|TowardDown|TowardRight;		
	}
	return SideMode;
}
//----------------------------------------------------------------------------//
BOX_TOWARD  RotateToward(double Angle, BOX_TOWARD Toward)
{
	BOX_TOWARD NewToward = Toward;
	const int AngleLabel = JetAPI::GetAngleLabel(Angle);
	switch ( AngleLabel )
	{
	case 90:
		switch ( Toward )
		{
		case BOX_TOWARD_LEFT:
			NewToward = BOX_TOWARD_DOWN;
			break;
		case BOX_TOWARD_DOWN:
			NewToward = BOX_TOWARD_RIGHT;
			break;
		case BOX_TOWARD_RIGHT:
			NewToward = BOX_TOWARD_UP;
			break;
		case BOX_TOWARD_UP:
			NewToward = BOX_TOWARD_LEFT;
			break;		
		}
		break;
	case 180:		
		switch ( Toward )
		{
		case BOX_TOWARD_LEFT:
			NewToward = BOX_TOWARD_RIGHT;
			break;
		case BOX_TOWARD_DOWN:
			NewToward = BOX_TOWARD_UP;
			break;
		case BOX_TOWARD_RIGHT:
			NewToward = BOX_TOWARD_LEFT;
			break;
		case BOX_TOWARD_UP:
			NewToward = BOX_TOWARD_DOWN;
			break;		
		}
		break;
	case 270:
		switch ( Toward )
		{
		case BOX_TOWARD_LEFT:
			NewToward = BOX_TOWARD_UP;
			break;
		case BOX_TOWARD_UP:
			NewToward = BOX_TOWARD_RIGHT;
			break;
		case BOX_TOWARD_RIGHT:
			NewToward = BOX_TOWARD_DOWN;
			break;
		case BOX_TOWARD_DOWN:
			NewToward = BOX_TOWARD_LEFT;
			break;		
		}
		break;
	default:
		break;
	}
	return NewToward;
}
//----------------------------------------------------------------------------//
BOARD_ORIENTATION_MODE     RotateBoardOrientationMode(BOARD_ORIENTATION_MODE Mode, double dAngle)//單板方向旋轉
{
	BOARD_ORIENTATION_MODE NewMode=Mode;
	const int nAngle = JetAPI::GetAngleLabel(dAngle);
	switch ( nAngle )
	{
	case 90:
		switch ( NewMode )
		{
		case BOARD_ORIENTATION_030:	NewMode = BOARD_ORIENTATION_120;	break;
		case BOARD_ORIENTATION_060: NewMode = BOARD_ORIENTATION_150;	break;			
		case BOARD_ORIENTATION_120: NewMode = BOARD_ORIENTATION_210;	break;
		case BOARD_ORIENTATION_150: NewMode = BOARD_ORIENTATION_240;	break;
		case BOARD_ORIENTATION_210: NewMode = BOARD_ORIENTATION_300;	break;
		case BOARD_ORIENTATION_240: NewMode = BOARD_ORIENTATION_330;	break;
		case BOARD_ORIENTATION_300: NewMode = BOARD_ORIENTATION_030;	break;
		case BOARD_ORIENTATION_330:	NewMode = BOARD_ORIENTATION_060;	break;			
		}
		break;
	case 180:
		switch ( NewMode )
		{
		case BOARD_ORIENTATION_030:	NewMode = BOARD_ORIENTATION_210;	break;
		case BOARD_ORIENTATION_060: NewMode = BOARD_ORIENTATION_240;	break;			
		case BOARD_ORIENTATION_120: NewMode = BOARD_ORIENTATION_300;	break;
		case BOARD_ORIENTATION_150: NewMode = BOARD_ORIENTATION_330;	break;
		case BOARD_ORIENTATION_210: NewMode = BOARD_ORIENTATION_030;	break;
		case BOARD_ORIENTATION_240: NewMode = BOARD_ORIENTATION_060;	break;
		case BOARD_ORIENTATION_300: NewMode = BOARD_ORIENTATION_120;	break;
		case BOARD_ORIENTATION_330:	NewMode = BOARD_ORIENTATION_150;	break;			
		}
		break;
	case 270:
		switch ( NewMode )
		{
		case BOARD_ORIENTATION_030:	NewMode = BOARD_ORIENTATION_300;	break;
		case BOARD_ORIENTATION_060: NewMode = BOARD_ORIENTATION_330;	break;			
		case BOARD_ORIENTATION_120: NewMode = BOARD_ORIENTATION_030;	break;
		case BOARD_ORIENTATION_150: NewMode = BOARD_ORIENTATION_060;	break;
		case BOARD_ORIENTATION_210: NewMode = BOARD_ORIENTATION_120;	break;
		case BOARD_ORIENTATION_240: NewMode = BOARD_ORIENTATION_150;	break;
		case BOARD_ORIENTATION_300: NewMode = BOARD_ORIENTATION_210;	break;
		case BOARD_ORIENTATION_330:	NewMode = BOARD_ORIENTATION_240;	break;			
		}
		break;
	case 0:
	default:
		break;
	}
	return NewMode;
}
//----------------------------------------------------------------------------//
bool ExtractOutline(const std::vector<POINT> &PtList, std::vector<POINT> &Outline)//萃取出輪廓線
{
	int    Max=0;
	int    Min=0;
	bool   MinRepeated=false;
	bool   MaxRepeated=false;
	POINT  Point;
	size_t i=0, j=0;
	size_t Count=0;
	std::vector<POINT>  TmpList=PtList;

	//Sort By X
	std::sort(TmpList.begin(), TmpList.end(), JetAPI::CmpPoint2D_X);
	Outline.clear();
	Count = TmpList.size();
	for ( i=0; i<Count; i++ )
	{
		Min=Max=TmpList[i].y;
		Point.x = TmpList[i].x;
		for ( j=i+1; j<Count; j++ )
		{
			if ( TmpList[j].x != TmpList[i].x ) 
			{
				i=j-1;
				break;
			}
			if ( Min > TmpList[j].y ) { Min = TmpList[j].y; }
			if ( Max < TmpList[j].y ) { Max = TmpList[j].y; }
		}
		Point.y = Min;
		Outline.push_back(Point);
		Point.y = Max;
		Outline.push_back(Point);
	}

	//Sort By Y	
	std::sort(TmpList.begin(), TmpList.end(), JetAPI::CmpPoint2D_Y);	
	Count = TmpList.size();
	const size_t CountT=Outline.size();
	for ( i=0; i<Count; i++ )
	{
		Min=Max=TmpList[i].x;
		Point.y = TmpList[i].y;
		for ( j=i+1; j<Count; j++ )
		{
			if ( TmpList[j].y != TmpList[i].y ) 
			{
				i=j-1;
				break;
			}
			if ( Min > TmpList[j].x ) { Min = TmpList[j].x; }
			if ( Max < TmpList[j].x ) { Max = TmpList[j].x; }
		}

		MaxRepeated = MinRepeated = false;		
		for ( j=0; j<CountT; j++ )
		{			
			if ( Point.y!=Outline[j].y ) { continue; }
			if ( Min == Outline[j].x )
			{	MinRepeated = true; }
			if ( Max == Outline[j].x )
			{	MaxRepeated = true; }
			if ( Point.x!=Outline[j].x ) { continue; }			
			if ( false==MinRepeated || false==MaxRepeated ) 
			{	continue; }
			break;
		}
		if ( false == MinRepeated )
		{
			Point.x = Min;			
			Outline.push_back(Point); 
		}
		if ( false == MaxRepeated )
		{
			Point.x = Max;			
			Outline.push_back(Point); 
		}
	}
	return true;
}
//----------------------------------------------------------------------------//
void MoveRect(const RECT &rectSrc, const TPOINT2D &MovePt, RECT &rectDst)
{
	const int Ox=(int)(MovePt.x);
	const int Oy=(int)(MovePt.y);
	rectDst.left = rectSrc.left+Ox;
	rectDst.top = rectSrc.top+Oy;
	rectDst.right = rectSrc.right+Ox;
	rectDst.bottom = rectSrc.bottom+Oy;
}
//----------------------------------------------------------------------------//
void MoveRegion(const TREGION4D &rgnSrc, const TPOINT2D &MovePt, TREGION4D &rgnDst)
{
	rgnDst.minX = rgnSrc.minX+MovePt.x;
	rgnDst.minY = rgnSrc.minY+MovePt.y;
	rgnDst.maxX = rgnSrc.maxX+MovePt.x;
	rgnDst.maxY = rgnSrc.maxY+MovePt.y;
	return;
}
//----------------------------------------------------------------------------//
void MoveCornerPts(const TPOINT2D CornerPts[4], const TPOINT2D &MovePt, TPOINT2D CornerPtsDst[4])
{
	CornerPtsDst[0].x = CornerPts[0].x+MovePt.x;
	CornerPtsDst[0].y = CornerPts[0].y+MovePt.y;
	CornerPtsDst[1].x = CornerPts[1].x+MovePt.x;
	CornerPtsDst[1].y = CornerPts[1].y+MovePt.y;
	CornerPtsDst[2].x = CornerPts[2].x+MovePt.x;
	CornerPtsDst[2].y = CornerPts[2].y+MovePt.y;
	CornerPtsDst[3].x = CornerPts[3].x+MovePt.x;
	CornerPtsDst[3].y = CornerPts[3].y+MovePt.y;	
	return;
}
//----------------------------------------------------------------------------//
double  MirrorXAxisAngle(double Angle)
{
	double NewAngle = 360-Angle;
	return JetAPI::RotateAngle(NewAngle, 0);
}
//-----------------------------------------------------------------------------//
double  MirrorYAxisAngle(double Angle)
{
	double NewAngle = 180-Angle;
	return JetAPI::RotateAngle(NewAngle, 0);
}
//-----------------------------------------------------------------------------//
void  MirrorXAxisToward(BOX_TOWARD &Toward)//朝向鏡射-X軸	
{
	switch ( Toward )
	{	
	case BOX_TOWARD_DOWN:
		Toward = BOX_TOWARD_UP;
		break;	
	case BOX_TOWARD_UP:
		Toward = BOX_TOWARD_DOWN;
		break;
	}
}
//-----------------------------------------------------------------------------//
void MirrorYAxisToward(BOX_TOWARD &Toward)//朝向鏡射-Y軸
{
	switch ( Toward )
	{
	case BOX_TOWARD_LEFT:
		Toward = BOX_TOWARD_RIGHT;
		break;	
	case BOX_TOWARD_RIGHT:
		Toward = BOX_TOWARD_LEFT;
		break;	
	}	
}
//-----------------------------------------------------------------------------//
void MirrorXAxisPos(double CPY, double &PosY)//座標鏡射-X軸
{
	PosY = CPY-(PosY-CPY);	
}
//-----------------------------------------------------------------------------//
void MirrorYAxisPos(double CPX, double &PosX)//座標鏡射-Y軸
{
	PosX = CPX-(PosX-CPX);
}
//-----------------------------------------------------------------------------//
void    MirrorXAxisPos(double CPY, TPOINT2D &Pos)//座標鏡射-X軸
{
	Pos.y = CPY-(Pos.y-CPY);	
}
//-----------------------------------------------------------------------------//
void    MirrorYAxisPos(double CPX, TPOINT2D &Pos)//座標鏡射-Y軸
{
	Pos.x = CPX-(Pos.x-CPX);
}
//-----------------------------------------------------------------------------//
void    MirrorXAxisRegion(double CPY, TREGION4D &Region)//區域鏡射-X軸
{
	double OMinY = Region.minY-CPY;	
	double OMaxY = Region.maxY-CPY;

	if ( -OMinY > -OMaxY )
	{
		Region.maxY = -OMinY+CPY;
		Region.minY = -OMaxY+CPY;
	}
	else
	{
		Region.maxY = -OMaxY+CPY;
		Region.minY = -OMinY+CPY;
	}
}
//-----------------------------------------------------------------------------//
void    MirrorYAxisRegion(double CPX, TREGION4D &Region)//區域鏡射-Y軸
{
	double OMinX = Region.minX-CPX;	
	double OMaxX = Region.maxX-CPX;	

	if ( -OMinX > -OMaxX )
	{
		Region.maxX = -OMinX+CPX;
		Region.minX = -OMaxX+CPX;
	}
	else
	{
		Region.maxX = -OMaxX+CPX;
		Region.minX = -OMinX+CPX;
	}
}
//-----------------------------------------------------------------------------//
void  MirrorPos(BOX_TOWARD &T1, BOX_TOWARD &T2, double &dPx, double &dPy)
{
	if ( T1 == T2 ) { return; }
	const double dPx2 = dPx;
	const double dPy2 = dPy;
	switch ( T1 )
	{
	case BOX_TOWARD_UP:
		switch ( T2 )
		{		
		case BOX_TOWARD_LEFT:
			dPx = dPy = 0;
			break;
		case BOX_TOWARD_DOWN:
			dPy = -dPy;
			break;
		case BOX_TOWARD_RIGHT:
			dPx = dPy = 0;
			break;
		}
		break;
	case BOX_TOWARD_LEFT:
		switch ( T2 )
		{
		case BOX_TOWARD_UP:
			dPx = dPy = 0;
			break;		
		case BOX_TOWARD_DOWN:
			dPx = dPy = 0;
			break;
		case BOX_TOWARD_RIGHT:			
			dPx = -dPx;
			break;
		}
		break;
	case BOX_TOWARD_DOWN:
		switch ( T2 )
		{
		case BOX_TOWARD_UP:
			dPy = -dPy;
			break;
		case BOX_TOWARD_LEFT:
			dPx = dPy = 0;
			break;		
		case BOX_TOWARD_RIGHT:
			dPx = dPy = 0;
			break;
		}
		break;
	case BOX_TOWARD_RIGHT:
		switch ( T2 )
		{
		case BOX_TOWARD_UP:
			dPx = dPy = 0;
			break;
		case BOX_TOWARD_LEFT:			
			dPx = -dPx;
			break;
		case BOX_TOWARD_DOWN:
			dPx = dPy = 0;
			break;		
		}
		break;
	}
	return;
}
//-----------------------------------------------------------------------------//
void    MirrorXAxisRegion(double CPY, double &MinX, double &MinY, double &MaxX, double &MaxY)//區域鏡射-X軸
{
	double OMinY = MinY-CPY;	
	double OMaxY = MaxY-CPY;

	if ( -OMinY > -OMaxY )
	{
		MaxY = -OMinY+CPY;
		MinY = -OMaxY+CPY;
	}
	else
	{
		MaxY = -OMaxY+CPY;
		MinY = -OMinY+CPY;
	}
}
//-----------------------------------------------------------------------------//
void    MirrorYAxisRegion(double CPX, double &MinX, double &MinY, double &MaxX, double &MaxY)//區域鏡射-Y軸
{
	double OMinX = MinX-CPX;	
	double OMaxX = MaxX-CPX;	

	if ( -OMinX > -OMaxX )
	{
		MaxX = -OMinX+CPX;
		MinX = -OMaxX+CPX;
	}
	else
	{
		MaxX = -OMaxX+CPX;
		MinX = -OMinX+CPX;
	}
}
//-----------------------------------------------------------------------------//
BOARD_ORIENTATION_MODE MirrorXAxisBoardOrientationMode(BOARD_ORIENTATION_MODE OrientationMode)//單板方向鏡射-Y軸
{
	switch ( OrientationMode )
	{
	case BOARD_ORIENTATION_030: OrientationMode = BOARD_ORIENTATION_330; break;
	case BOARD_ORIENTATION_060: OrientationMode = BOARD_ORIENTATION_300; break;
	case BOARD_ORIENTATION_120: OrientationMode = BOARD_ORIENTATION_240; break;
	case BOARD_ORIENTATION_150: OrientationMode = BOARD_ORIENTATION_210; break;
	case BOARD_ORIENTATION_210: OrientationMode = BOARD_ORIENTATION_150; break;
	case BOARD_ORIENTATION_240: OrientationMode = BOARD_ORIENTATION_120; break;
	case BOARD_ORIENTATION_300: OrientationMode = BOARD_ORIENTATION_060; break;
	case BOARD_ORIENTATION_330: OrientationMode = BOARD_ORIENTATION_030; break;	
	}
	return OrientationMode;
}
//-----------------------------------------------------------------------------//
BOARD_ORIENTATION_MODE MirrorYAxisBoardOrientationMode(BOARD_ORIENTATION_MODE OrientationMode)//單板方向鏡射-Y軸
{	
	switch ( OrientationMode )
	{
	case BOARD_ORIENTATION_030: OrientationMode = BOARD_ORIENTATION_150; break;
	case BOARD_ORIENTATION_060: OrientationMode = BOARD_ORIENTATION_120; break;
	case BOARD_ORIENTATION_120: OrientationMode = BOARD_ORIENTATION_060; break;
	case BOARD_ORIENTATION_150: OrientationMode = BOARD_ORIENTATION_030; break;
	case BOARD_ORIENTATION_210: OrientationMode = BOARD_ORIENTATION_330; break;
	case BOARD_ORIENTATION_240: OrientationMode = BOARD_ORIENTATION_300; break;
	case BOARD_ORIENTATION_300: OrientationMode = BOARD_ORIENTATION_240; break;
	case BOARD_ORIENTATION_330: OrientationMode = BOARD_ORIENTATION_210; break;	
	}
	return OrientationMode;
}
//-----------------------------------------------------------------------------//
void CalcBoardOrientationOffset(BOARD_ORIENTATION_MODE RefMode, BOARD_ORIENTATION_MODE NewMode, double &Angle, bool &MirrorXAxis, bool &MirrorYAxis)
{
	Angle = 0;	
	MirrorXAxis = false;
	MirrorYAxis = false;
	if ( RefMode == NewMode )
	{	return;	}
	switch ( RefMode )
	{
	case BOARD_ORIENTATION_030:
		switch ( NewMode )
		{
		case BOARD_ORIENTATION_030:
			//Self
			break;
		case BOARD_ORIENTATION_060:
			Angle = 90;			
			MirrorYAxis = true;
			break;
		case BOARD_ORIENTATION_120:
			Angle = 90;			
			break;
		case BOARD_ORIENTATION_150:			
			MirrorYAxis = true;
			break;
		case BOARD_ORIENTATION_210:
			Angle = 180;			
			break;
		case BOARD_ORIENTATION_240:
			Angle = 270;			
			MirrorYAxis = true;
			break;
		case BOARD_ORIENTATION_300:
			Angle = 270;			
			break;
		case BOARD_ORIENTATION_330:
			MirrorXAxis = true;
			break;	
		}
		break;
	case BOARD_ORIENTATION_060:
		switch ( NewMode )
		{
		case BOARD_ORIENTATION_030:
			Angle = 270;
			MirrorXAxis = true;
			break;
		case BOARD_ORIENTATION_060:
			//Self
			break;
		case BOARD_ORIENTATION_120:
			MirrorYAxis = true;
			break;
		case BOARD_ORIENTATION_150:
			Angle =  90;
			break;
		case BOARD_ORIENTATION_210:
			Angle =  90;
			MirrorXAxis = true;
			break;
		case BOARD_ORIENTATION_240:
			Angle = 180;
			break;
		case BOARD_ORIENTATION_300:
			MirrorXAxis = true;
			break;
		case BOARD_ORIENTATION_330:
			Angle = 270;
			break;	
		}
		break;
	case BOARD_ORIENTATION_120:
		switch ( NewMode )
		{
		case BOARD_ORIENTATION_030:
			Angle = 270;
			break;
		case BOARD_ORIENTATION_060:	
			MirrorYAxis = true;
			break;
		case BOARD_ORIENTATION_120:
			//Self
			break;
		case BOARD_ORIENTATION_150:
			Angle = 90;	
			MirrorXAxis = true;
			break;
		case BOARD_ORIENTATION_210:	
			Angle = 90;	
			break;
		case BOARD_ORIENTATION_240:
			MirrorXAxis = true;
			break;
		case BOARD_ORIENTATION_300:			
			Angle = 180;
			break;
		case BOARD_ORIENTATION_330:
			Angle = 270;
			MirrorXAxis = true;
			break;	
		}
		break;
	case BOARD_ORIENTATION_150:
		switch ( NewMode )
		{
		case BOARD_ORIENTATION_030:
			MirrorYAxis = true;
			break;
		case BOARD_ORIENTATION_060:
			Angle = 270;
			break;
		case BOARD_ORIENTATION_120:		
			Angle = 270;
			MirrorYAxis = true;
			break;
		case BOARD_ORIENTATION_150:
			//Self
			break;
		case BOARD_ORIENTATION_210:	
			MirrorXAxis = true;
			break;
		case BOARD_ORIENTATION_240:
			Angle = 90;
			break;
		case BOARD_ORIENTATION_300:			
			Angle = 90;
			MirrorYAxis = true;
			break;
		case BOARD_ORIENTATION_330:
			Angle = 180;
			break;	
		}
		break;
	case BOARD_ORIENTATION_210:
		switch ( NewMode )
		{
		case BOARD_ORIENTATION_030:
			Angle = 180;
			break;
		case BOARD_ORIENTATION_060:			
			Angle = 270;
			MirrorYAxis = true;
			break;
		case BOARD_ORIENTATION_120:	
			Angle = 270;
			break;
		case BOARD_ORIENTATION_150:
			MirrorXAxis = true;
			break;
		case BOARD_ORIENTATION_210:
			//Self
			break;
		case BOARD_ORIENTATION_240:
			Angle = 90;
			MirrorYAxis = true;
			break;
		case BOARD_ORIENTATION_300:
			Angle = 90;
			break;
		case BOARD_ORIENTATION_330:
			Angle = 180;
			MirrorXAxis = true;
			break;	
		}
		break;
	case BOARD_ORIENTATION_240:
		switch ( NewMode )
		{
		case BOARD_ORIENTATION_030:
			Angle = 90;
			MirrorXAxis = true;
			break;
		case BOARD_ORIENTATION_060:
			Angle = 180;
			break;
		case BOARD_ORIENTATION_120:	
			Angle = 180;
			MirrorYAxis = true;
			break;
		case BOARD_ORIENTATION_150:
			Angle = 270;
			break;
		case BOARD_ORIENTATION_210:
			Angle = 270;
			MirrorXAxis = true;
			break;
		case BOARD_ORIENTATION_240:
			//Self
			break;
		case BOARD_ORIENTATION_300:
			MirrorYAxis = true;
			break;
		case BOARD_ORIENTATION_330:
			Angle = 90;
			break;	
		}
		break;
	case BOARD_ORIENTATION_300:
		switch ( NewMode )
		{
		case BOARD_ORIENTATION_030:
			Angle = 90;
			break;
		case BOARD_ORIENTATION_060:
			Angle = 180;
			MirrorYAxis = true;
			break;
		case BOARD_ORIENTATION_120:
			Angle = 180;
			break;
		case BOARD_ORIENTATION_150:
			Angle = 270;
			MirrorXAxis = true;
			break;
		case BOARD_ORIENTATION_210:
			Angle = 270;
			break;
		case BOARD_ORIENTATION_240:
			MirrorYAxis = true;
			break;
		case BOARD_ORIENTATION_300:	
			//Self
			break;
		case BOARD_ORIENTATION_330:
			Angle = 90;
			MirrorXAxis = true;
			break;	
		}
		break;
	case BOARD_ORIENTATION_330:
		switch ( NewMode )
		{
		case BOARD_ORIENTATION_030:
			MirrorXAxis = true;
			break;
		case BOARD_ORIENTATION_060:
			Angle = 90;
			break;
		case BOARD_ORIENTATION_120:
			Angle = 90;
			MirrorYAxis = true;
			break;
		case BOARD_ORIENTATION_150:
			Angle = 180;
			break;
		case BOARD_ORIENTATION_210:
			Angle = 180;
			MirrorXAxis = true;
			break;
		case BOARD_ORIENTATION_240:
			Angle = 270;
			break;
		case BOARD_ORIENTATION_300:
			Angle = 270;
			MirrorYAxis = true;
			break;
		case BOARD_ORIENTATION_330:
			//Self
			break;	
		}
		break;
	}
	//JetAPI::RotatePos(Angle, 0, 0, NewPosX, NewPosY);		
	//if ( true == MirrorXAxis )
	//{	JetAPI::MirrorXAxisPos(0, NewPosY);	}
	//if ( true == MirrorYAxis )
	//{	JetAPI::MirrorYAxisPos(0, NewPosX);	}	
	return ;
}
//-----------------------------------------------------------------------------//
bool    CheckIsPressVRKey(UINT VK)//確認是否按下特定鍵盤
{
	switch ( VK )
	{
	case VK_CONTROL:
		VK = VK;
		break;
	case VK_SHIFT:
		VK = VK;
		break;
	}
	if ( ::GetKeyState(VK) < 0 ) { return true; }
	return false;
}
//-----------------------------------------------------------------------------//
void  UpdateCursor(CURSOR_POS_MODE Mode)
{
	CWinApp *AppPtr = ::AfxGetApp();
	if ( NULL == AppPtr ) { return; }
	
	HCURSOR hCursor=NULL;
	switch ( Mode )
	{
	case CURSOR_POS_LEFT:
	case CURSOR_POS_RIGHT:
		hCursor = AppPtr->LoadStandardCursor(IDC_SIZEWE);
		break;
	case CURSOR_POS_TOP:
	case CURSOR_POS_BOTTOM:
		hCursor = AppPtr->LoadStandardCursor(IDC_SIZENS);
		break;
	case CURSOR_POS_LEFT_TOP:
	case CURSOR_POS_RIGHT_BOTTOM:
		hCursor = AppPtr->LoadStandardCursor(IDC_SIZENWSE);		
		break;
	case CURSOR_POS_RIGHT_TOP:
	case CURSOR_POS_LEFT_BOTTOM:
		hCursor = AppPtr->LoadStandardCursor(IDC_SIZENESW);
		break;
	case CURSOR_POS_INNER:
		hCursor = AppPtr->LoadStandardCursor(IDC_SIZEALL);
		break;
	case CURSOR_POS_HAND:
		hCursor = AppPtr->LoadStandardCursor(IDC_HAND);
		break;
	default:
		hCursor = AppPtr->LoadStandardCursor(IDC_ARROW);
		break;	
	}
	if ( NULL == hCursor ) { return; }
	::SetCursor(hCursor);
}
//-----------------------------------------------------------------------------//
CURSOR_POS_MODE MapCadCursorPosModeToImageCursorPosMode(CURSOR_POS_MODE CursorMode, bool SignX, bool SignY)//將Cad的鼠標方向改成影像的鼠標方向
{
	CURSOR_POS_MODE NewCursorode = CursorMode;
	switch ( NewCursorode )//Cad的上下與影像的上下相反
	{
	case CURSOR_POS_TOP:	        NewCursorode = CURSOR_POS_BOTTOM; break;
	case CURSOR_POS_BOTTOM:	        NewCursorode = CURSOR_POS_TOP; break;		
	case CURSOR_POS_LEFT_TOP:	    NewCursorode = CURSOR_POS_LEFT_BOTTOM; break;
	case CURSOR_POS_LEFT_BOTTOM:	NewCursorode = CURSOR_POS_LEFT_TOP; break;
	case CURSOR_POS_RIGHT_TOP:	    NewCursorode = CURSOR_POS_RIGHT_BOTTOM; break;
	case CURSOR_POS_RIGHT_BOTTOM:	NewCursorode = CURSOR_POS_RIGHT_TOP; break;
	default:
		break;
	}
	return NewCursorode;
}
//-----------------------------------------------------------------------------//
CURSOR_POS_MODE MapStageCursorPosModeToCadCursorPosMode(CURSOR_POS_MODE CursorMode, bool SignX, bool SignY)//將機台的鼠標方向改成Cad的鼠標方向
{
	CURSOR_POS_MODE NewCursorode = CursorMode;
	if ( false == SignX )
	{
		switch ( NewCursorode )
		{
		case CURSOR_POS_LEFT:	        NewCursorode = CURSOR_POS_RIGHT; break;
		case CURSOR_POS_RIGHT:	        NewCursorode = CURSOR_POS_LEFT; break;		
		case CURSOR_POS_LEFT_TOP:	    NewCursorode = CURSOR_POS_RIGHT_TOP; break;
		case CURSOR_POS_LEFT_BOTTOM:	NewCursorode = CURSOR_POS_RIGHT_BOTTOM; break;
		case CURSOR_POS_RIGHT_TOP:	    NewCursorode = CURSOR_POS_LEFT_TOP; break;
		case CURSOR_POS_RIGHT_BOTTOM:	NewCursorode = CURSOR_POS_LEFT_BOTTOM; break;
		default:
			break;
		}
	}
	if ( false == SignY )
	{
		switch ( NewCursorode )
		{
		case CURSOR_POS_TOP:	        NewCursorode = CURSOR_POS_BOTTOM; break;
		case CURSOR_POS_BOTTOM:	        NewCursorode = CURSOR_POS_TOP; break;		
		case CURSOR_POS_LEFT_TOP:	    NewCursorode = CURSOR_POS_LEFT_BOTTOM; break;
		case CURSOR_POS_LEFT_BOTTOM:	NewCursorode = CURSOR_POS_LEFT_TOP; break;
		case CURSOR_POS_RIGHT_TOP:	    NewCursorode = CURSOR_POS_RIGHT_BOTTOM; break;
		case CURSOR_POS_RIGHT_BOTTOM:	NewCursorode = CURSOR_POS_RIGHT_TOP; break;
		default:
			break;
		}
	}
	return NewCursorode;
}
//-----------------------------------------------------------------------------//
CURSOR_POS_MODE MapStageCursorPosModeToImageCursorPosMode(CURSOR_POS_MODE CursorMode, bool SignX, bool SignY)//將機台的鼠標方向改成影像的鼠標方向
{
	CURSOR_POS_MODE NewCursorode = CursorMode;
	if ( false == SignX )//是指與Cad相反, 而非與影像相反
	{
		switch ( NewCursorode )
		{
		case CURSOR_POS_LEFT:	        NewCursorode = CURSOR_POS_RIGHT; break;
		case CURSOR_POS_RIGHT:	        NewCursorode = CURSOR_POS_LEFT; break;		
		case CURSOR_POS_LEFT_TOP:	    NewCursorode = CURSOR_POS_RIGHT_TOP; break;
		case CURSOR_POS_LEFT_BOTTOM:	NewCursorode = CURSOR_POS_RIGHT_BOTTOM; break;
		case CURSOR_POS_RIGHT_TOP:	    NewCursorode = CURSOR_POS_LEFT_TOP; break;
		case CURSOR_POS_RIGHT_BOTTOM:	NewCursorode = CURSOR_POS_LEFT_BOTTOM; break;
		default:
			break;
		}
	}
	if ( true == SignY )//是指與Cad相反, 而非與影像相反
	{
		switch ( NewCursorode )
		{
		case CURSOR_POS_TOP:	        NewCursorode = CURSOR_POS_BOTTOM; break;
		case CURSOR_POS_BOTTOM:	        NewCursorode = CURSOR_POS_TOP; break;		
		case CURSOR_POS_LEFT_TOP:	    NewCursorode = CURSOR_POS_LEFT_BOTTOM; break;
		case CURSOR_POS_LEFT_BOTTOM:	NewCursorode = CURSOR_POS_LEFT_TOP; break;
		case CURSOR_POS_RIGHT_TOP:	    NewCursorode = CURSOR_POS_RIGHT_BOTTOM; break;
		case CURSOR_POS_RIGHT_BOTTOM:	NewCursorode = CURSOR_POS_RIGHT_TOP; break;
		default:
			break;
		}
	}
	return NewCursorode;
}
//-----------------------------------------------------------------------------//
CURSOR_POS_MODE CheckCursorPosMode(const RECT &BoxRect, const SIZE &szGrid, const POINT &point, int ChkMode)//確認鼠標座標模式
{	
	CURSOR_POS_MODE Mode = CURSOR_POS_NONE;		
	const int OffSetX  = szGrid.cx;
	const int OffSetY  = szGrid.cy;
	const int CPX = (BoxRect.left+BoxRect.right)/2;
	const int CPY = (BoxRect.top+BoxRect.bottom)/2;
	RECT RectOuter, RectInner, Rect;

	RectOuter.left   = BoxRect.left-OffSetX;
	RectOuter.right  = BoxRect.right+OffSetX;
	RectOuter.top    = BoxRect.top-OffSetY;
	RectOuter.bottom = BoxRect.bottom+OffSetY;
	RectOuter.bottom += 1;

	if ( ::PtInRect(&RectOuter, point) == FALSE )
	{	return CURSOR_POS_NONE;	}

	RectInner.left   = BoxRect.left+OffSetX;
	RectInner.right  = BoxRect.right-OffSetX;
	RectInner.top    = BoxRect.top+OffSetY;
	RectInner.bottom = BoxRect.bottom-OffSetY;
	RectInner.top    += 1;

	//at Inner	
	if ( ::PtInRect(&RectInner, point) == TRUE )
	{
		if ( CHECK_CURSOR_MODE_FILL == ChkMode )
		{	return CURSOR_POS_INNER;	 }
		else
		{	return CURSOR_POS_NONE;	 }
	}	

	//Top Only
	Rect.top = RectOuter.top;
	Rect.bottom = RectInner.top;
	if ( CHECK_CURSOR_MODE_FILL == ChkMode )
	{
		Rect.left = RectInner.left;
		Rect.right = RectInner.right;
	}
	else
	{
		Rect.left = CPX-OffSetX;
		Rect.right = CPX+OffSetX;
	}
	if ( ::PtInRect(&Rect, point) == TRUE )
	{	return CURSOR_POS_TOP;	}

	//Bottom Only
	Rect.top = RectInner.bottom;
	Rect.bottom = RectOuter.bottom;
	if ( CHECK_CURSOR_MODE_FILL == ChkMode )
	{
		Rect.left = RectInner.left;
		Rect.right = RectInner.right;
	}
	else
	{
		Rect.left = CPX-OffSetX;
		Rect.right = CPX+OffSetX;
	}
	if ( ::PtInRect(&Rect, point) == TRUE )
	{	return CURSOR_POS_BOTTOM;	}

	//Left Only
	Rect.left = RectOuter.left;
	Rect.right = RectInner.left;
	if ( CHECK_CURSOR_MODE_FILL == ChkMode )
	{
		Rect.top = RectInner.top;
		Rect.bottom = RectInner.bottom;
	}
	else
	{
		Rect.top = CPY-OffSetY;
		Rect.bottom = CPY+OffSetY;
		Rect.bottom += 1;
	}
	if ( ::PtInRect(&Rect, point) == TRUE )
	{	return CURSOR_POS_LEFT;	}

	//Right Only
	Rect.left = RectInner.right;
	Rect.right = RectOuter.right;
	if ( CHECK_CURSOR_MODE_FILL == ChkMode )
	{
		Rect.top = RectInner.top;
		Rect.bottom = RectInner.bottom;
	}
	else
	{
		Rect.top = CPY-OffSetY;
		Rect.bottom = CPY+OffSetY;
		Rect.bottom += 1;
	}
	if ( ::PtInRect(&Rect, point) == TRUE )
	{	return CURSOR_POS_RIGHT;	}	

	//Left Top
	Rect.left = RectOuter.left;
	Rect.right = RectInner.left;
	Rect.top = RectOuter.top;
	Rect.bottom = RectInner.top;
	if ( ::PtInRect(&Rect, point) == TRUE )
	{	return CURSOR_POS_LEFT_TOP;	}

	//Left Bottom
	Rect.left = RectOuter.left;
	Rect.right = RectInner.left;
	Rect.top = RectInner.bottom;
	Rect.bottom = RectOuter.bottom;
	if ( ::PtInRect(&Rect, point) == TRUE )
	{	return CURSOR_POS_LEFT_BOTTOM;	}

	//Right Top
	Rect.left = RectInner.right;
	Rect.right = RectOuter.right;
	Rect.top = RectOuter.top;
	Rect.bottom = RectInner.top;
	if ( ::PtInRect(&Rect, point) == TRUE )
	{	return CURSOR_POS_RIGHT_TOP;	}

	//Right Bottom
	Rect.left = RectInner.right;
	Rect.right = RectOuter.right;
	Rect.top = RectInner.bottom;
	Rect.bottom = RectOuter.bottom;
	if ( ::PtInRect(&Rect, point) == TRUE )
	{	return CURSOR_POS_RIGHT_BOTTOM;	}
	
	return CURSOR_POS_INNER;	
}
//-----------------------------------------------------------------------------//
bool    CalcModifySizeRegion(CURSOR_POS_MODE CursorMode, const bool DoubleEdit, double dPx, double dPy, TREGION4D &dRgn)
{
	switch ( CursorMode )
	{
	case CURSOR_POS_LEFT:
		dRgn.minX = dPx;
		if ( true == DoubleEdit )
		{	dRgn.maxX = -dRgn.minX;	}
		break;
	case CURSOR_POS_RIGHT:
		dRgn.maxX = dPx;
		if ( true == DoubleEdit )
		{	dRgn.minX = -dRgn.maxX;	}
		break;
	case CURSOR_POS_TOP:
		dRgn.maxY = dPy;
		if ( true == DoubleEdit )
		{	dRgn.minY = -dRgn.maxY;	}
		break;
	case CURSOR_POS_BOTTOM:
		dRgn.minY = dPy;
		if ( true == DoubleEdit )
		{	dRgn.maxY = -dRgn.minY;	}
		break;
	case CURSOR_POS_LEFT_TOP:
		dRgn.minX = dPx;
		dRgn.maxY = dPy;
		if ( true == DoubleEdit )
		{	
			dRgn.maxX = -dRgn.minX;	
			dRgn.minY = -dRgn.maxY;
		}
		break;
	case CURSOR_POS_LEFT_BOTTOM:
		dRgn.minX = dPx;
		dRgn.minY = dPy;
		if ( true == DoubleEdit )
		{	
			dRgn.maxX = -dRgn.minX;	
			dRgn.maxY = -dRgn.minY;
		}
		break;
	case CURSOR_POS_RIGHT_TOP:
		dRgn.maxX = dPx;
		dRgn.maxY = dPy;
		if ( true == DoubleEdit )
		{	
			dRgn.minX = -dRgn.maxX;	
			dRgn.minY = -dRgn.maxY;
		}
		break;
	case CURSOR_POS_RIGHT_BOTTOM:
		dRgn.maxX = dPx;
		dRgn.minY = dPy;
		if ( true == DoubleEdit )
		{	
			dRgn.minX = -dRgn.maxX;	
			dRgn.maxY = -dRgn.minY;
		}
		break;
	}
	return true;
}
//-----------------------------------------------------------------------------//
BOOL InitialTreeCtrl(CTreeCtrl &TreeCtrl)//初始化樹狀圖控制元件
{
	if ( TreeCtrl.GetSafeHwnd() == NULL ) { return NULL; }

	//DWORD dwStyle = TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS |TVS_EDITLABELS | TVS_SHOWSELALWAYS | TVS_FULLROWSELECT;
	DWORD dwStyle = TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS | TVS_FULLROWSELECT;
	TreeCtrl.ModifyStyle(NULL, dwStyle);	
	return TRUE;
}
//-----------------------------------------------------------------------------//
HTREEITEM  GetTreeSelectedItem(CTreeCtrl &TreeCtrl)//取得現在滑鼠下的節點
{	
	if ( TreeCtrl.GetSafeHwnd() == NULL ) { return NULL; }

	UINT flag=0;
	POINT point;
	::GetCursorPos(&point);	
	TreeCtrl.ScreenToClient(&point);
	HTREEITEM hItem = TreeCtrl.HitTest(point, &flag);	
	if ( hItem == NULL ) { return NULL; }
	int Item = TVHT_ONITEM;
	int ItemLabel = TVHT_ONITEMLABEL;
	int ItemIcon = TVHT_ONITEMICON;
	if ( (ItemLabel & flag)!=0 || (ItemIcon & flag)!=0  )
	{	return hItem;	}
	return NULL; 
}
//--------------------------------------------------------------------------------//
int GetTreeNodeLevel(CTreeCtrl &TreeCtrl, HTREEITEM hItem)//取得節點所存在的階層, 0為最上層, 1為第二層, 2為第三層
{
	//由目前往上找, 找到NULL 表示找完
	HTREEITEM hItemParent=TreeCtrl.GetParentItem(hItem);
	int Level = 0;
	while ( hItemParent != NULL )
	{
		hItemParent = TreeCtrl.GetParentItem(hItemParent);
		Level ++;
	};	
	return Level;
}
//--------------------------------------------------------------------------------//
bool InitialUUID(UUID &uuid)//UUID的初始化
{
	::memset(&uuid, 0x00, sizeof(uuid));
	return true;
}
//--------------------------------------------------------------------------------//
bool UUIDFromStringA(LPCSTR Text, UUID &uuid)
{
	UCHAR *pszUuid = (UCHAR*)(Text); 
	if ( UuidFromStringA(pszUuid, &uuid) != RPC_S_OK )
	{	return false; }	
	return true;
}
//--------------------------------------------------------------------------------//
bool UUIDFromStringW(LPCWSTR Text, UUID &uuid)
{	
	USHORT *pszUuid = (USHORT*)(Text); 
	if ( UuidFromStringW(pszUuid, &uuid) != RPC_S_OK )
	{	return false; }		
	return true;
}
//--------------------------------------------------------------------------------//
bool UUIDToString(const UUID &uuid, CString &Text)
{	
#ifndef _UNICODE
	char uuidStr[MAX_JET_PATH]="";		
	if ( UUIDToStringA(uuid, uuidStr) == false )
	{	return false; }		
	Text = uuidStr;	
#else
	wchar_t uuidWStr[MAX_JET_PATH]=L"";		
	if ( UUIDToStringW(uuid, uuidWStr) == false )
	{	return false; }	
	Text = uuidWStr;	
#endif//_UNICODE
	return true;
}
//--------------------------------------------------------------------------------//
bool UUIDToStringA(const UUID &uuid, char Text[])
{
	UCHAR *pszUuid = NULL; 
	if ( UuidToStringA(&uuid, &pszUuid) != RPC_S_OK )
	{	return false; }
	::sprintf(Text, "%s", pszUuid);
	::_strupr(Text);
	RpcStringFreeA(&pszUuid);
	return true;
}
//--------------------------------------------------------------------------------//
bool UUIDToStringW(const UUID &uuid, wchar_t Text[])
{
	USHORT *pszUuid = NULL; 
	if ( UuidToStringW(&uuid, &pszUuid) != RPC_S_OK )
	{	return false; }
	::swprintf(Text, L"%s", pszUuid);
	::_wcsupr(Text);
	RpcStringFreeW(&pszUuid);
	return true;
}
//--------------------------------------------------------------------------------//
void ClearCastParam(TCastParam &CastParam)
{
	if ( NULL != CastParam.PtrA1 ) 
	{	JetMemory.free_func(CastParam.PtrA1); }
	if ( NULL != CastParam.PtrA2 ) 
	{	JetMemory.free_func(CastParam.PtrA2); }
	if ( NULL != CastParam.PtrA3 ) 
	{	JetMemory.free_func(CastParam.PtrA3); }
	if ( NULL != CastParam.PtrA4 ) 
	{	JetMemory.free_func(CastParam.PtrA4); }
	if ( NULL != CastParam.PtrA5 ) 
	{	JetMemory.free_func(CastParam.PtrA5); }

	if ( NULL != CastParam.PtrB1 ) 
	{	JetMemory.free_func(CastParam.PtrB1); }
	if ( NULL != CastParam.PtrB2 ) 
	{	JetMemory.free_func(CastParam.PtrB2); }
	if ( NULL != CastParam.PtrB3 ) 
	{	JetMemory.free_func(CastParam.PtrB3); }
	if ( NULL != CastParam.PtrB4 ) 
	{	JetMemory.free_func(CastParam.PtrB4); }
	if ( NULL != CastParam.PtrB5 ) 
	{	JetMemory.free_func(CastParam.PtrB5); }
	if ( NULL != CastParam.PtrB6 ) 
	{	JetMemory.free_func(CastParam.PtrB6); }

	if ( NULL != CastParam.PtrC1 ) 
	{	JetMemory.free_func(CastParam.PtrC1); }
	if ( NULL != CastParam.PtrC2 ) 
	{	JetMemory.free_func(CastParam.PtrC2); }
	if ( NULL != CastParam.PtrC3 ) 
	{	JetMemory.free_func(CastParam.PtrC3); }
	if ( NULL != CastParam.PtrC4 ) 
	{	JetMemory.free_func(CastParam.PtrC4); }
	if ( NULL != CastParam.PtrC5 ) 
	{	JetMemory.free_func(CastParam.PtrC5); }

	if ( NULL != CastParam.PtrD1 ) 
	{	JetMemory.free_func(CastParam.PtrD1); }
	if ( NULL != CastParam.PtrD2 ) 
	{	JetMemory.free_func(CastParam.PtrD2); }
	if ( NULL != CastParam.PtrD3 ) 
	{	JetMemory.free_func(CastParam.PtrD3); }
	if ( NULL != CastParam.PtrD4 ) 
	{	JetMemory.free_func(CastParam.PtrD4); }
	if ( NULL != CastParam.PtrD5 ) 
	{	JetMemory.free_func(CastParam.PtrD5); }
	if ( NULL != CastParam.PtrD6 ) 
	{	JetMemory.free_func(CastParam.PtrD6); }

	if ( NULL != CastParam.PtrMask ) 
	{	JetMemory.free_func(CastParam.PtrMask); }	
	if ( NULL != CastParam.PtrSpace ) 
	{	JetMemory.free_func(CastParam.PtrSpace); }	

	CastParam.PerA = 0;
	CastParam.PerB = 0;
	CastParam.ImageW = 0;
	CastParam.ImageH = 0;
	CastParam.ImageStep = 0;
	CastParam.ImageCount = 0;

	CastParam.PtrA1 = NULL;
	CastParam.PtrA2 = NULL;
	CastParam.PtrA3 = NULL;
	CastParam.PtrA4 = NULL;
	CastParam.PtrA5 = NULL;
	CastParam.PtrB1 = NULL;
	CastParam.PtrB2 = NULL;
	CastParam.PtrB3 = NULL;
	CastParam.PtrB4 = NULL;
	CastParam.PtrB5 = NULL;

	CastParam.PtrC1 = NULL;
	CastParam.PtrC2 = NULL;
	CastParam.PtrC3 = NULL;
	CastParam.PtrC4 = NULL;
	CastParam.PtrC5 = NULL;
	CastParam.PtrD1 = NULL;
	CastParam.PtrD2 = NULL;
	CastParam.PtrD3 = NULL;
	CastParam.PtrD4 = NULL;
	CastParam.PtrD5 = NULL;

	CastParam.PtrMask = NULL;	
	CastParam.PtrSpace = NULL;	

	CastParam.Gamma = 1.0;
	CastParam.ZeroPhasePtr = NULL;
	CastParam.HeightFactorPtr = NULL;	
	return;
}
//--------------------------------------------------------------------------------//
void ClearUniFrame(TUNI_FRAME &UniFrame)
{
	JetMemory.free_func(UniFrame.ImagePtr);
	JetMemory.free_func(UniFrame.MaskPtr);
	JetMemory.free_func(UniFrame.PhasePtr);
	JetMemory.free_func(UniFrame.SpacePtr);
	JetMemory.free_func(UniFrame.RawMaskPtr);
	JetMemory.free_func(UniFrame.RawSpacePtr);
}
//--------------------------------------------------------------------------------//
void InitialUniFrame(TUNI_FRAME &UniFrame)
{
	UniFrame.FrameUniqueID = FRAME_UNIQUE_ID_NULL;
	UniFrame.ImageW = 1024;
	UniFrame.ImageH = 1024;
	UniFrame.ImageStep = 1024;
	UniFrame.BitCount = 8;		
	UniFrame.ImagePtr = NULL;
	UniFrame.MaskPtr = NULL;
	UniFrame.PhasePtr = NULL;
	UniFrame.SpacePtr = NULL;		
	UniFrame.RawMaskPtr = NULL;
	UniFrame.RawSpacePtr = NULL;		
}
//--------------------------------------------------------------------------------//
void ClearUniFrameList(std::vector<TUNI_FRAME> &UniFrameList)
{
	size_t i=0;
	const size_t UniFrameCount = UniFrameList.size();
	for ( i=0; i<UniFrameCount; i++ )
	{	ClearUniFrame(UniFrameList[i]);	}
	UniFrameList.clear();
}
//--------------------------------------------------------------------------------//
void ClearUniFrameList(TUNI_FRAME *UniFrameListPtr, size_t Count)
{
	size_t i=0;	
	for ( i=0; i<Count; i++ )
	{
		JetMemory.free_func(UniFrameListPtr[i].ImagePtr);
		JetMemory.free_func(UniFrameListPtr[i].MaskPtr);
		JetMemory.free_func(UniFrameListPtr[i].PhasePtr);
		JetMemory.free_func(UniFrameListPtr[i].SpacePtr);
		JetMemory.free_func(UniFrameListPtr[i].RawMaskPtr);
		JetMemory.free_func(UniFrameListPtr[i].RawSpacePtr);
	}	
}
//--------------------------------------------------------------------------------//
void InitialUniFrameList(TUNI_FRAME *UniFrameListPtr, size_t Count)
{
	size_t i=0;	
	for ( i=0; i<Count; i++ )
	{	InitialUniFrame(UniFrameListPtr[i]);	}
}
//--------------------------------------------------------------------------------//
bool CloneUniFrameList(const char *fnName, const std::vector<TUNI_FRAME> &UniFrameListSrc, std::vector<TUNI_FRAME> &UniFrameListDst)
{
	bool   IsOK=true;
	size_t i=0;
	size_t BufferSize=0;
	size_t UniFrameCount;
	char   varName[64]="";
	TUNI_FRAME UniFrame;
	MASK_PTR   MaskPtr=NULL;
	MASK_PTR   MaskPtrNew=NULL;
	SPACE_PTR  SpacePtr=NULL;
	SPACE_PTR  SpacePtrNew=NULL;
	PHASE_PTR  PhasePtr=NULL;
	PHASE_PTR  PhasePtrNew=NULL;
	IMAGE_PTR  ImagePtr=NULL;
	IMAGE_PTR  ImagePtrNew=NULL;
	MASK_PTR   RawMaskPtr=NULL;
	MASK_PTR   RawMaskPtrNew=NULL;
	SPACE_PTR  RawSpacePtr=NULL;
	SPACE_PTR  RawSpacePtrNew=NULL;
	IMAGE_SIZE ImageW=0, ImageH=0, ImageStep=0, BitCount=0;
	IMAGE_SIZE ImageWOrg=0, ImageHOrg=0, ImageStepOrg=0, BitCountOrg=0;		

	IsOK=true;
	JetAPI::ClearUniFrameList(UniFrameListDst);	
	UniFrameCount = UniFrameListSrc.size();
	for ( i=0; i<UniFrameCount; i++ )
	{
		ImageW    = UniFrameListSrc[i].ImageW;
		ImageH    = UniFrameListSrc[i].ImageH;
		ImageStep = UniFrameListSrc[i].ImageStep;
		BitCount  = UniFrameListSrc[i].BitCount;

		ImagePtr    = UniFrameListSrc[i].ImagePtr;
		MaskPtr     = UniFrameListSrc[i].MaskPtr;		
		SpacePtr    = UniFrameListSrc[i].SpacePtr;
		PhasePtr    = UniFrameListSrc[i].PhasePtr;
		BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
		
		ImagePtrNew = NULL;
		MaskPtrNew = NULL;
		SpacePtrNew = NULL;
		PhasePtrNew = NULL;
		RawMaskPtrNew=NULL;
		RawSpacePtrNew=NULL;

		if ( NULL != ImagePtr ) 
		{
			::sprintf(varName, "%s#%d", "ImagePtr", i+1);
			if ( JetMemory.alloc_func(BufferSize, ImagePtrNew, fnName, varName) == false ) 
			{	
				IsOK=false;
				JetMemory.free_func(ImagePtrNew);
				JetMemory.free_func(MaskPtrNew);
				JetMemory.free_func(SpacePtrNew);
				JetMemory.free_func(PhasePtrNew);
				JetMemory.free_func(RawMaskPtrNew);
				JetMemory.free_func(RawSpacePtrNew);
				break;
			}
			::memcpy(ImagePtrNew, ImagePtr, sizeof(IMAGE_DATA)*BufferSize);			
		}

		if ( NULL != MaskPtr ) 
		{	
			::sprintf(varName, "%s#%d", "MaskPtr", i+1);
			if ( JetMemory.alloc_func(BufferSize, MaskPtrNew, fnName, varName) == false ) 
			{
				IsOK=false;
				JetMemory.free_func(ImagePtrNew);
				JetMemory.free_func(MaskPtrNew);
				JetMemory.free_func(SpacePtrNew);
				JetMemory.free_func(PhasePtrNew);
				JetMemory.free_func(RawMaskPtrNew);
				JetMemory.free_func(RawSpacePtrNew);
				break;
			}
			::memcpy(MaskPtrNew, MaskPtr, sizeof(MASK_DATA)*BufferSize);
		}

		if ( NULL != SpacePtr ) 
		{	
			::sprintf(varName, "%s#%d", "SpacePtr", i+1);
			if ( JetMemory.alloc_func(BufferSize, SpacePtrNew, fnName, varName) == false ) 
			{
				IsOK=false;
				JetMemory.free_func(ImagePtrNew);
				JetMemory.free_func(MaskPtrNew);
				JetMemory.free_func(SpacePtrNew);
				JetMemory.free_func(PhasePtrNew);
				JetMemory.free_func(RawMaskPtrNew);
				JetMemory.free_func(RawSpacePtrNew);
				break;
			}
			::memcpy(SpacePtrNew, SpacePtr, sizeof(SPACE_DATA)*BufferSize);
		}

		if ( NULL != PhasePtr )
		{
			::sprintf(varName, "%s#%d", "PhasePtr", i+1);
			if ( JetMemory.alloc_func(BufferSize, PhasePtrNew, fnName, varName) == false ) 
			{
				IsOK=false;
				JetMemory.free_func(ImagePtrNew);
				JetMemory.free_func(MaskPtrNew);
				JetMemory.free_func(SpacePtrNew);
				JetMemory.free_func(PhasePtrNew);
				JetMemory.free_func(RawMaskPtrNew);
				JetMemory.free_func(RawSpacePtrNew);
				break;
			}
			::memcpy(PhasePtrNew, PhasePtr, sizeof(PHASE_DATA)*BufferSize);
		}

		if ( NULL != RawMaskPtr ) 
		{	
			::sprintf(varName, "%s#%d", "RawMaskPtr", i+1);
			if ( JetMemory.alloc_func(BufferSize, RawMaskPtrNew, fnName, varName) == false ) 
			{
				IsOK=false;
				JetMemory.free_func(ImagePtrNew);
				JetMemory.free_func(MaskPtrNew);
				JetMemory.free_func(SpacePtrNew);
				JetMemory.free_func(PhasePtrNew);
				JetMemory.free_func(RawMaskPtrNew);
				JetMemory.free_func(RawSpacePtrNew);
				break;
			}
			::memcpy(RawMaskPtrNew, RawMaskPtr, sizeof(MASK_DATA)*BufferSize);
		}

		if ( NULL != RawSpacePtr ) 
		{	
			::sprintf(varName, "%s#%d", "RawSpacePtr", i+1);
			if ( JetMemory.alloc_func(BufferSize, RawSpacePtrNew, fnName, varName) == false ) 
			{
				IsOK=false;
				JetMemory.free_func(ImagePtrNew);
				JetMemory.free_func(MaskPtrNew);
				JetMemory.free_func(SpacePtrNew);
				JetMemory.free_func(PhasePtrNew);
				JetMemory.free_func(RawMaskPtrNew);
				JetMemory.free_func(RawSpacePtrNew);
				break;
			}
			::memcpy(RawSpacePtrNew, RawSpacePtr, sizeof(SPACE_DATA)*BufferSize);
		}
		
		UniFrame.ImageW = ImageW;
		UniFrame.ImageH = ImageH;
		UniFrame.ImageStep = ImageStep;
		UniFrame.BitCount = BitCount;
		UniFrame.ImagePtr = ImagePtrNew;
		UniFrame.MaskPtr = MaskPtrNew;
		UniFrame.SpacePtr = SpacePtrNew;
		UniFrame.PhasePtr = PhasePtrNew;
		UniFrame.RawMaskPtr = RawMaskPtrNew;
		UniFrame.RawSpacePtr = RawSpacePtrNew;
		UniFrameListDst.push_back(UniFrame);
	}

	if ( false == IsOK )
	{	JetAPI::ClearUniFrameList(UniFrameListDst);	}
	return IsOK;	
}
//--------------------------------------------------------------------------------//
DWORD BitMask_Add(DWORD val, DWORD msk)//位元遮罩相加
{
	return val|msk;
}
//--------------------------------------------------------------------------------//
DWORD BitMask_Remove(DWORD val, DWORD msk)//位元遮罩相加
{
	DWORD NotMsk = ~msk;
	return val&NotMsk;
}
//--------------------------------------------------------------------------------//
bool BitMask_Check(DWORD val, DWORD Msk)//遮罩比較
{
	return (Msk & val) ? true : false;

	DWORD Res = val&Msk;
	if ( 0 == Res ) { return false; }
	return true;
}
//--------------------------------------------------------------------------------//
bool CheckMoved(double x1, double y1, double x2, double y2)//確認移動過
{
	if ( ::fabs(x1-x2) > 3 ) { return true; }
	if ( ::fabs(y1-y2) > 3 ) { return true; }
	return false;
}
//--------------------------------------------------------------------------------//
int StrToInt(LPCSTR str)//字串轉整數
{
	return ::atoi(str);
}
//--------------------------------------------------------------------------------//
double StrToDbl(LPCSTR str)//字串轉浮點數
{
	return ::atof(str);
}
//--------------------------------------------------------------------------------//
int StrToInt(LPCWSTR  str)//字串轉整數
{
	return _wtoi(str);
}
//--------------------------------------------------------------------------------//
double StrToDbl(LPCWSTR  str)//字串轉浮點數
{
	return _wtof(str);
}
//--------------------------------------------------------------------------------//
bool TimeDelay_Sleep(DWORD dwMilliseconds)//時間延遲-使用Sleep
{
	::Sleep(dwMilliseconds);
	return true;
}
//--------------------------------------------------------------------------------//
bool TimeDelay_TickCount(DWORD dwMilliseconds)//時間延遲-使用TickCount
{
	DWORD CntD = 0;
	DWORD Cnt2 = 0;
	DWORD Cnt1 = ::GetTickCount();
	while ( true ) 
	{
		Cnt2 = ::GetTickCount();
		CntD = Cnt2-Cnt1;
		if ( CntD < dwMilliseconds ) 
		{	continue; }
		return true;
	};
	return true;
}
//--------------------------------------------------------------------------------//
CString SearchOtherTestFolder(const std::vector<CString> &FolderList, LPCTSTR refDateTime, bool bNext)
{
	if ( NULL == refDateTime ) { return _T(""); }

	size_t i=0, j=0;
	CString      Folder;
	CString      FolderTest;
	const size_t FolderCount = FolderList.size();

	if ( false == bNext )//往前找
	{
		for ( i=0; i<FolderCount; i++ )
		{
			j = FolderCount-i-1;
			FolderTest = FolderList[j];
			if ( FolderTest < refDateTime ) { return FolderTest; }
		}
		if ( i == FolderCount )
		{	return FolderList[0]; }
	}
	else//往後找
	{
		for ( i=0; i<FolderCount; i++ )
		{
			FolderTest = FolderList[i];
			if ( FolderTest > refDateTime ) { return FolderTest; }
		}
		if ( i == FolderCount )
		{	return FolderList[FolderCount-1]; }
	}	
	return _T("");
}
//-----------------------------------------------------------------------------//
void SwapFunc(float &v1, float &v2)
{
	float tmp=v1;
	v1 = v2;
	v2 = tmp;
}
//-----------------------------------------------------------------------------//
int  QuickSort_Partition(std::vector<float> &List, int front, int end)//快速排序-分割
{
	int   i = front-1;	
	float Pivot = List[end];    
    for (int j=front; j<end; j++)
	{
        if (List[j] < Pivot)
		{
            i++;
            SwapFunc(List[i], List[j]);
        }
    }
    i++;
    SwapFunc(List[i], List[end]);
    return i;
}
//-----------------------------------------------------------------------------//
bool QuickSort_Recursion(std::vector<float> &List, int front, int end)//快速排序-遞迴
{
	if (front < end) 
	{
        int Pivot = QuickSort_Partition(List, front, end);
        QuickSort_Recursion(List, front, Pivot-1);
        QuickSort_Recursion(List, Pivot+1, end);
    }
	return true;
}
//-----------------------------------------------------------------------------//
bool QuickSort_Iterative(std::vector<float> &List, int front, int end)//快速排序-疊代
{
	int h = end;
	int l = front;
	const int Size = h-l + 1;	

	// Create an auxiliary stack     
	std::vector<int> stack(Size+1);
  
    // initialize top of stack 
    int top = -1; 
  
    // push initial values of l and h to stack 
    stack[++top] = l; 
    stack[++top] = h; 
  
    // Keep popping from stack while is not empty 
    while (top >= 0) 
	{ 
        // Pop h and l 
        h = stack[top--]; 
        l = stack[top--]; 
  
        // Set pivot element at its correct position 
        // in sorted array 
        int p = QuickSort_Partition(List, l, h); 
  
        // If there are elements on left side of pivot, 
        // then push left side to stack 
        if ( (p-1) > l)
		{ 
            stack[++top] = l; 
            stack[++top] = p - 1; 
        } 

        // If there are elements on right side of pivot, 
        // then push right side to stack 
        if ( (p+1) < h) 
		{ 
            stack[++top] = p + 1; 
            stack[++top] = h; 
        } 
    } ;
	return true;
}
//-----------------------------------------------------------------------------//
bool MergeSort_Merge(std::vector<float> &List, int front, int mid, int end)//合併排序-合併
{
	// 利用 std::vector 的constructor, 	
	std::vector<float> LeftSub(List.begin()+front, List.begin()+mid+1),// 把array[front]~array[mid]放進 LeftSub[]
                     RightSub(List.begin()+mid+1, List.begin()+end+1);// 把array[mid+1]~array[end]放進 RightSub[]		
	LeftSub.push_back(FLT_MAX);// 在LeftSub[]尾端加入值為 Max 的元素	
	RightSub.push_back(FLT_MAX);// 在RightSub[]尾端加入值為 Max 的元素

	int   i=0;    
    int idxLeft = 0, idxRight = 0;
    for (i=front; i<=end; i++) 
	{
        if (LeftSub[idxLeft] <= RightSub[idxRight] ) 
		{
            List[i] = LeftSub[idxLeft];
            idxLeft++;
        }
        else
		{
            List[i] = RightSub[idxRight];
            idxRight++;
        }
    }
	return true;
}
//-----------------------------------------------------------------------------//
bool MergeSort_Recursion(std::vector<float> &List, int front, int end)//合併排序-遞迴
{
	if (front < end)  // 表示目前的矩陣範圍是有效的
	{                   
        int mid = (front+end)/2;         // mid即是將矩陣對半分的index
		if ( MergeSort_Recursion(List, front, mid) == false )// 繼續divide矩陣的前半段subarray
		{	return false; }        
        if ( MergeSort_Recursion(List, mid+1, end) == false ) // 繼續divide矩陣的後半段subarray
		{	return false; }
        if ( MergeSort_Merge(List, front, mid, end) == false )// 將兩個subarray做比較, 並合併出排序後的矩陣
		{	return false; }
    }
	return true;
}
//-----------------------------------------------------------------------------//
bool MergeSort_Iterative(std::vector<float> &List, int count)//合併排序-疊代
{
	int curr_size;  // For current size of subarrays to be merged 
                   // curr_size varies from 1 to n/2 
	int left_start; // For picking starting index of left subarray 
                   // to be merged 
  
   // Merge subarrays in bottom up manner.  First merge subarrays of 
   // size 1 to create sorted subarrays of size 2, then merge subarrays 
   // of size 2 to create sorted subarrays of size 4, and so on. 
   for (curr_size=1; curr_size<=count-1; curr_size = 2*curr_size) 
   { 
       // Pick starting point of different subarrays of current size 
       for (left_start=0; left_start<count-1; left_start += 2*curr_size) 
       { 
           // Find ending point of left subarray. mid+1 is starting  
           // point of right 
           int mid = MIN(left_start + curr_size - 1, count-1); 
  
           int right_end = MIN(left_start + 2*curr_size - 1, count-1); 
  
           // Merge Subarrays arr[left_start...mid] & arr[mid+1...right_end] 
           if ( MergeSort_Merge(List, left_start, mid, right_end) == false )
		   {	return false; }
       } 
   } 
   return true;
}
//-----------------------------------------------------------------------------//
bool CheckListSorted(const std::vector<float> &List)//確認已經排序過
{
	size_t     i=0;
	const size_t Count = List.size();
	if ( 0==Count || 1==Count ) 
	{	return true; }
	for ( i=0; i<Count-1; i++ )
	{
		if ( List[i] > List[i+1] ) 
		{	return false; }
	}
	return true;
}
//-----------------------------------------------------------------------------//
bool SortList_Insertion(std::vector<float> &List)
{
	int   i=0,j=0;
	float key=0.0f;
	const int Count=(int)(List.size());	
	for (i=1; i <Count; i++) 
	{
		j = i-1;
		key = List[i];		
		while (key < List[j] && j >= 0) 
		{
			List[j+1] = List[j];
			j--;
		}
		List[j+1] = key;
	}
	return true;
}
//-----------------------------------------------------------------------------//
bool SortList_Selection(std::vector<float> &List)
{
	float   Temp=0.0f;
	size_t  i=0, j=0, idx;
	const size_t Count = List.size();
	if ( 0 == Count )
	{	return true; }

	for ( i=0; i<Count-1; i++ )
	{
		idx = i;
		for ( j=i+1; j<Count; j++ )
		{
			if ( List[idx] > List[j] )
			{	idx = j;	}
		}		
		Temp = List[i];
		List[i] = List[idx];
		List[idx] = Temp;	 
	}
	return true;
}
//-----------------------------------------------------------------------------//
bool SortList_QuickSort(std::vector<float> &List)//快速排序法
{
	const int Count = (int)(List.size());
	//return QuickSort_Recursion(List, 0, Count-1);
	return QuickSort_Iterative(List, 0, Count-1);
}
//-----------------------------------------------------------------------------//
bool SortList_MergeSort(std::vector<float> &List)//合併排序法
{
	const int Count = (int)(List.size());
	//return MergeSort_Recursion(List, 0, Count-1);
	return MergeSort_Iterative(List, Count);	
}
//-----------------------------------------------------------------------------//
bool Calc2LinePoint(const TLINE2D &L1, const TLINE2D &L2, TPOINT2D &P)//求得2線交點
{
	const double Precision=0.0001;
	const double a1=L1.a, b1=L1.b, c1=L1.c;
	const double a2=L2.a, b2=L2.b, c2=L2.c;
	const double a1a2=a1*a2, b1a2=b1*a2, c1a2=c1*a2;
	const double a2a1=a2*a1, b2a1=b2*a1, c2a1=c2*a1;
	const double b1a2b2a1=b1a2-b2a1;
	const double c1a2c2a1=c1a2-c2a1;
	P.x = P.y = 0;
	if ( fabs(b1a2b2a1) < Precision )
	{	return true; }

	P.y = c1a2c2a1/b1a2b2a1;	
	if ( fabs(a1) > Precision )
	{	P.x = (c1-(b1*P.y))/a1; }
	else if ( fabs(a2) > Precision )
	{	P.x = (c2-(b2*P.y))/a2; }		
	return true;
}
//-----------------------------------------------------------------------------//
bool Calc2PointLine(const TPOINT2D &P1, const TPOINT2D &P2, TLINE2D &Line)//求得2點的直線(ax+by=c)
{	
	const double dx=P2.x-P1.x;
	const double dy=P2.y-P1.y;		
	const double Precision=0.0001;
	if ( fabs(dx)<Precision )//ax=c
	{
		Line.a = 1.0;
		Line.b = 0.0;
		Line.c = P1.x;
	}
	else if ( fabs(dy)<Precision )//by=c
	{
		Line.a = 0.0;
		Line.b = 1.0;
		Line.c = P1.y;
	}
	else
	{
		Line.a = dy/dx;
		Line.b = -1.0;
		Line.c = -1*((-Line.a*P1.x)+P1.y);
		if ( Line.a < 0.0 )
		{
			Line.a *= -1;
			Line.b *= -1;
			Line.c *= -1;
		}
	}	
	return true;
}
//-----------------------------------------------------------------------------//
bool CalcOrthogonalLine(const TPOINT2D &P, const TLINE2D &L1, TLINE2D &Line)//求得正交直線(ax+by=c)
{	
	const double Precision=0.0001;
	if ( fabs(L1.a) < Precision )//by=c=>ax
	{
		Line.a = 1.0;
		Line.b = 0.0;
		Line.c = P.x;
	}
	else if ( fabs(L1.b) < Precision )//ax=c
	{
		Line.a = 0.0;
		Line.b = 1.0;
		Line.c = P.y;
	}
	else//ax+by+c=0
	{
		Line.a =  L1.b;
		Line.b = -L1.a;
		Line.c = (Line.a*P.x+Line.b*P.y);
		if ( Line.a < 0.0 )
		{
			Line.a *= -1;
			Line.b *= -1;
			Line.c *= -1;
		}
	}
	return true;
}
//-----------------------------------------------------------------------------//
bool CalcFittingLine(const std::vector<TPOINT2D> &PtList, double &nX, double &nY)//線段擬合
{	/*輸入一組座標值, 根據最小二乘法計算直線方程 y=kx+b, 先返回斜率k ,再返回截距b*/
#ifndef OPENCV_DISABLE
	try 
	{
		//Ax=b的形式, 將A, b寫成矩陣的形式
		int   i=0;
		const int PtCnt=(int)(PtList.size());
		cv::Mat A(PtCnt, 2, CV_32F);
		cv::Mat b(PtCnt, 1, CV_32F);

		//初始化矩陣A
		for (i=0; i<PtCnt; i++  )	
		{	
			A.at<float>(i, 0) = PtList[i].x; 
			A.at<float>(i, 1) = 1;
		}
		//  cout <<矩陣A：<< endl << A << endl;

		//確認矩陣A是否為奇異矩陣-不能用
		double detVal=cv::determinant(A);
		if ( fabs(detVal) < 0.001 )
		{	return false; }

		//初始化矩陣b 
		for (i=0; i<PtCnt; i++  )
		{	b.at<float>(i, 0) = PtList[i].y; }
		//  cout <<"矩陣b"<< endl << b << endl;

		//根據線性代數知識, A'* A * x = A' * b 求得的矩陣 x 即為最優解
		//解 x = (A' * A)^-1 * A' * b
		cv::Mat x = (A.t()*A).inv()*A.t()*b;
		nX = x.at<float>(0, 0);
		nY = x.at<float>(1, 0);
		return true;
	}
	catch ( cv::Exception& e )
	{	
		CString str;
		const char* msg_e = e.what();
		str = msg_e;
		return false;
	}
#endif//OPENCV_DISABLE
	return false;
}
//-----------------------------------------------------------------------------//
bool CalcFittingPlane(const std::vector<TPOINT3D> &PtList, double &nX, double &nY, double &nZ)//平面擬合
{	/*輸入一組座標值, 根據最小二乘法計算平面方程分別返回 a ,b, c 的值[aX+bY-Z   c = 0]*/
#ifndef OPENCV_DISABLE	
	try 
	{
		//Ax = 0的形式, 將A, b 寫成矩陣的形式
		int   i=0;
		int PtCnt=(int)(PtList.size());
		if ( PtCnt < 3 ) { return false; }		
		if ( PtCnt > 3 ) 
		{ 
		//	PtCnt = 3; 		
		}

		cv::Mat A(PtCnt, 3, CV_32F);
		cv::Mat b(PtCnt, 1, CV_32F);
		//  cout <<"原始點為:"<< point << endl;
		//初始化矩陣A	
		for (i=0; i<PtCnt; i++  )
		{	
			A.at<float>(i, 0) = PtList[i].x; 
			A.at<float>(i, 1) = PtList[i].y;
			A.at<float>(i, 2) = 1;
		}
		//  cout << "矩陣A:" << endl << A << endl;

		//確認矩陣A是否為奇異矩陣-不能用
		//double detVal=cv::determinant(A);
		//if ( fabs(detVal) < 0.001 )
		//{	return false; }
	
		//初始化矩陣b 
		for (i=0; i<PtCnt; i++  )
		{	b.at<float>(i, 0) = PtList[i].z; }
		//  cout << "矩陣b:" << endl << b << endl;

		//根據線性代數知識，A'* A * x = A' * b 求得的矩陣 x 即為最優解
		//解 x = (A' * A)^-1 * A' * b
		//cv::Mat x = -((A.t()*A).inv()*A.t()*b);
		//cv::Mat x = (A.t()*A).inv()*A.t()*b;
		cv::Mat x = ((A.t()*A).inv()*A.t()*b);
	
		nX = x.at<float>(0, 0);
		nY = x.at<float>(1, 0);
		nZ = x.at<float>(2, 0);
		return true;
	}
	catch ( cv::Exception& e )
	{	
		CString str;
		const char* msg_e = e.what();  			
		str = msg_e;
		return false;
	}
#endif//OPENCV_DISABLE
	return false;
}
//-----------------------------------------------------------------------------//
bool BilinearInterpolation(const std::vector<TPOINT3D> &PtList, double PosX, double PosY, double &PosZ)//雙向內插法
{
	const size_t PtCount=PtList.size();
	if ( PtCount < 2 ) { return false; }
	const double X1=PosX-PtList[0].x;
	const double Y1=PosY-PtList[0].y;
	const double X2=PtList[1].x-PosX;
	const double Y2=PtList[1].y-PosY;
	const double X21=PtList[1].x-PtList[0].x;
	const double Y21=PtList[1].y-PtList[0].y;
	const double XZ1=(X2*PtList[0].z)/X21;
	const double XZ2=(X1*PtList[1].z)/X21;
	const double YZ1=(Y2*PtList[0].z)/Y21;
	const double YZ2=(Y1*PtList[1].z)/Y21;
	const double XZ=XZ1+XZ2;
	const double YZ=YZ1+YZ2;
	PosZ = (XZ+YZ)/2.0;	
	return true;
}
//-----------------------------------------------------------------------------//
}//End JetAPI Scope

