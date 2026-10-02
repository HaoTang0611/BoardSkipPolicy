#include "stdafx.h"
#include "Rockey4ND.h"
//-------------------------------------------------------------------------------------//
#pragma warning( disable : 4996 )//_CRT_SECURE_NO_WARNINGS
//---------------------------------------------------------------------------------//
#define  TYPE_ROCKEY4			1
#define  TYPE_ROCKEY4P			2
#define  TYPE_ROCKEYUSB			3
#define  TYPE_ROCKEYUSBP		4
#define  TYPE_ROCKEYNET			5
#define  TYPE_ROCKEYUSBNET		6
#define  TYPE_ROCKEY4SMARTNET   10
#define  TYPE_ROCKEY4S          16
//---------------------------------------------------------------------------------//
CRockey4ND::CRockey4ND()
{
	PreInitRockey4ND();
	InitialRockey4ND();
}
//-------------------------------------------------------------------------------------//
CRockey4ND::~CRockey4ND()
{
	CloseRockey4();
}
//-------------------------------------------------------------------------------------//
void CRockey4ND::PreInitRockey4ND()
{
	m_HardIndex=0;
	m_Rockey4Ver = 0;
	m_RetCode = 0;
	::memset(m_Handle, 0xFF, sizeof(m_Handle));	
	::memset(m_HardNumber, 0x00, sizeof(m_HardNumber));
	::memset(m_ErrorString, 0x00, sizeof(m_ErrorString));
	::memset(m_DeviceError, 0x00, sizeof(m_DeviceError));	
}
//-------------------------------------------------------------------------------------//
void CRockey4ND::InitialRockey4ND()
{
	::memset(m_Param, 0x00, sizeof(m_Param));
	::memset(m_LParam, 0x00, sizeof(m_LParam));	
}
//-------------------------------------------------------------------------------------//
void CRockey4ND::CloneRockey4ND(const CRockey4ND &Rockey4)
{
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND::CloseHID(WORD HID)
{
	WORD retcode=0;	
	BYTE Buffer[ROCKEY4_BUFFER_SIZE];		
	SetHardIndex(HID);
	retcode = ExecRockeyFnc(RY_CLOSE, Buffer);
	m_HardNumber[HID]=0;
	m_Handle[HID] = ROCKEY4_INVALID;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND::CheckHIDValid(WORD HID)
{
	if ( HID > ROCKEY4_MAX_COUNT ) { return false; }
	if ( ROCKEY4_INVALID == m_Handle[HID] ) { return false; }
	if ( 0 == m_HardNumber[HID] ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND::GetHardNumber(DWORD &HNum, WORD HID) const
{
	if ( HID > ROCKEY4_MAX_COUNT ) { return false; }
	if ( ROCKEY4_INVALID == m_Handle[HID] ) { return false; }
	HNum = m_HardNumber[HID];
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND::CheckRockey4Type(DWORD Type)
{
	char TypeText[64];
	char &TextBuffer = TypeText[0];
	switch ( Type )
	{
	case TYPE_ROCKEY4:	::sprintf(&TextBuffer, "%s", "Rockey4 Standard Parallel Port");	break;
	case TYPE_ROCKEY4P:	::sprintf(&TextBuffer, "%s", "Rockey4 Plus Parallel Port");	break;
	case TYPE_ROCKEYUSB:	::sprintf(&TextBuffer, "%s", "Rockey4 Standard USB Port");	break;
	case TYPE_ROCKEYUSBP:	::sprintf(&TextBuffer, "%s", "Rockey4 Plus USB Port");	break;
	case TYPE_ROCKEYNET:	::sprintf(&TextBuffer, "%s", "Rockey4 Net Parallel Port");	break;
	case TYPE_ROCKEYUSBNET:	::sprintf(&TextBuffer, "%s", "Rockey4 Net USB Port");	break;
	case TYPE_ROCKEY4SMARTNET:	::sprintf(&TextBuffer, "%s", "Net Rockey4Smart USB Port");	break;
	case TYPE_ROCKEY4S:	::sprintf(&TextBuffer, "%s", "Rockey4Smart USB Port");	break;
	default:
		::sprintf(&TextBuffer, "Type[Undefined:%d]", Type);
		break;
	}	
	//if ( TYPE_ROCKEY4S != Type ) 
	//{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND::CheckRockey4NDRetCode(WORD RetCode)
{
	//if ( ERR_SUCCESS == RetCode )
	//{	return true; }
	char &TextBuffer = m_DeviceError[0];
	switch ( RetCode )
	{
	case ERR_SUCCESS:	::sprintf(&TextBuffer, "Rockey4[%s]", "ERR_SUCCESS");	break;
	case ERR_NO_ROCKEY:	::sprintf(&TextBuffer, "Rockey4[%s]", "ERR_NO_ROCKEY");	break;	
	case ERR_INVALID_PASSWORD:	::sprintf(&TextBuffer, "Rockey4[%s]", "ERR_INVALID_PASSWORD");	break;	
	case ERR_INVALID_PASSWORD_OR_ID:	::sprintf(&TextBuffer, "Rockey4[%s]", "ERR_INVALID_PASSWORD_OR_ID");	break;	
	case ERR_SETID:	::sprintf(&TextBuffer, "Rockey4[%s]", "ERR_SETID");	break;	
	case ERR_INVALID_ADDR_OR_SIZE:	::sprintf(&TextBuffer, "Rockey4[%s]", "ERR_INVALID_ADDR_OR_SIZE");	break;	
	case ERR_UNKNOWN_COMMAND:	::sprintf(&TextBuffer, "Rockey4[%s]", "ERR_UNKNOWN_COMMAND");	break;	
	case ERR_NOTBELEVEL3:	::sprintf(&TextBuffer, "Rockey4[%s]", "ERR_NOTBELEVEL3");	break;	
	case ERR_READ:	::sprintf(&TextBuffer, "Rockey4[%s]", "ERR_READ");	break;	
	case ERR_WRITE:	::sprintf(&TextBuffer, "Rockey4[%s]", "ERR_WRITE");	break;	
	case ERR_RANDOM:	::sprintf(&TextBuffer, "Rockey4[%s]", "ERR_RANDOM");	break;	
	case ERR_SEED:	::sprintf(&TextBuffer, "Rockey4[%s]", "ERR_SEED");	break;	
	case ERR_CALCULATE:	::sprintf(&TextBuffer, "Rockey4[%s]", "ERR_CALCULATE");	break;	
	case ERR_NO_OPEN:	::sprintf(&TextBuffer, "Rockey4[%s]", "ERR_NO_OPEN");	break;	
	case ERR_OPEN_OVERFLOW:	::sprintf(&TextBuffer, "Rockey4[%s]", "ERR_OPEN_OVERFLOW");	break;	
	case ERR_NOMORE:	::sprintf(&TextBuffer, "Rockey4[%s]", "ERR_NOMORE");	break;	
	case ERR_NEED_FIND:	::sprintf(&TextBuffer, "Rockey4[%s]", "ERR_NEED_FIND");	break;	
	case ERR_DECREASE:	::sprintf(&TextBuffer, "Rockey4[%s]", "ERR_DECREASE");	break;
	case ERR_AR_BADCOMMAND:	::sprintf(&TextBuffer, "Rockey4[%s]", "ERR_AR_BADCOMMAND");	break;
	case ERR_AR_UNKNOWN_OPCODE:	::sprintf(&TextBuffer, "Rockey4[%s]", "ERR_AR_UNKNOWN_OPCODE");	break;
	case ERR_AR_WRONGBEGIN:	::sprintf(&TextBuffer, "Rockey4[%s]", "ERR_AR_WRONGBEGIN");	break;
	case ERR_AR_WRONG_END:	::sprintf(&TextBuffer, "Rockey4[%s]", "ERR_AR_WRONG_END");	break;
	case ERR_AR_VALUEOVERFLOW:	::sprintf(&TextBuffer, "Rockey4[%s]", "ERR_AR_VALUEOVERFLOW");	break;
	case ERR_TOOMUCHTHREAD:	::sprintf(&TextBuffer, "Rockey4[%s]", "ERR_TOOMUCHTHREAD");	break;
	case ERR_INVALID_RY4S:	::sprintf(&TextBuffer, "Rockey4[%s]", "ERR_INVALID_RY4S");	break;	
	case ERR_INVALID_PARAMETER:	::sprintf(&TextBuffer, "Rockey4[%s]", "ERR_INVALID_PARAMETER");	break;
	case ERR_INVALID_TIMEVALUE:	::sprintf(&TextBuffer, "Rockey4[%s]", "ERR_INVALID_TIMEVALUE");	break;

	case ERR_SET_DES_KEY:	::sprintf(&TextBuffer, "Rockey4[%s]", "ERR_SET_DES_KEY");	break;
	case ERR_DES_ENCRYPT:	::sprintf(&TextBuffer, "Rockey4[%s]", "ERR_DES_ENCRYPT");	break;
	case ERR_DES_DECRYPT:	::sprintf(&TextBuffer, "Rockey4[%s]", "ERR_DES_DECRYPT");	break;
	case ERR_SET_RSAKEY_N:	::sprintf(&TextBuffer, "Rockey4[%s]", "ERR_SET_RSAKEY_N");	break;
	case ERR_SET_RSAKEY_D:	::sprintf(&TextBuffer, "Rockey4[%s]", "ERR_SET_RSAKEY_D");	break;
	case ERR_RSA_ENCRYPT:	::sprintf(&TextBuffer, "Rockey4[%s]", "ERR_RSA_ENCRYPT");	break;
	case ERR_RSA_DECRYPT:	::sprintf(&TextBuffer, "Rockey4[%s]", "ERR_RSA_DECRYPT");	break;
	case ERR_INVALID_LENGTH:	::sprintf(&TextBuffer, "Rockey4[%s]", "ERR_INVALID_LENGTH");	break;		

	case R4SERR_JUST_TIMER:	::sprintf(&TextBuffer, "Rockey4[%s]", "R4SERR_JUST_TIMER");	break;
	case R4SERR_NO_SUCH_DEVICE:	::sprintf(&TextBuffer, "Rockey4[%s]", "R4SERR_NO_SUCH_DEVICE");	break;
	case R4SERR_NOT_OPENED_DEVICE:	::sprintf(&TextBuffer, "Rockey4[%s]", "R4SERR_NOT_OPENED_DEVICE");	break;
	case R4SERR_WRONG_UID:	::sprintf(&TextBuffer, "Rockey4[%s]", "R4SERR_WRONG_UID");	break;
	case R4SERR_INDEX:	::sprintf(&TextBuffer, "Rockey4[%s]", "R4SERR_INDEX");	break;
	case R4SERR_TOO_LONG_SEED:	::sprintf(&TextBuffer, "Rockey4[%s]", "R4SERR_TOO_LONG_SEED");	break;
	case R4SERR_WRITE_PROTECT:	::sprintf(&TextBuffer, "Rockey4[%s]", "R4SERR_WRITE_PROTECT");	break;
	case R4SERR_OPEN_DEVICE:	::sprintf(&TextBuffer, "Rockey4[%s]", "R4SERR_OPEN_DEVICE");	break;
	case R4SERR_READ_REPORT:	::sprintf(&TextBuffer, "Rockey4[%s]", "R4SERR_READ_REPORT");	break;
	case R4SERR_WRITE_REPORT:	::sprintf(&TextBuffer, "Rockey4[%s]", "R4SERR_WRITE_REPORT");	break;
	case R4SERR_SETUP_DI_GET_DEVICE_INTERFACE_DETAIL:	::sprintf(&TextBuffer, "Rockey4[%s]", "R4SERR_SETUP_DI_GET_DEVICE_INTERFACE_DETAIL");	break;
	case R4SERR_GET_ATTRIBUTES:	::sprintf(&TextBuffer, "Rockey4[%s]", "R4SERR_GET_ATTRIBUTES");	break;
	case R4SERR_GET_PREPARSED_DATA:	::sprintf(&TextBuffer, "Rockey4[%s]", "R4SERR_GET_PREPARSED_DATA");	break;
	case R4SERR_GETCAPS:	::sprintf(&TextBuffer, "Rockey4[%s]", "R4SERR_GETCAPS");	break;
	case R4SERR_FREE_PREPARSED_DATA:	::sprintf(&TextBuffer, "Rockey4[%s]", "R4SERR_FREE_PREPARSED_DATA");	break;
	case R4SERR_FLUSH_QUEUE:	::sprintf(&TextBuffer, "Rockey4[%s]", "R4SERR_FLUSH_QUEUE");	break;
	case R4SERR_SETUP_DI_CLASS_DEVS:	::sprintf(&TextBuffer, "Rockey4[%s]", "R4SERR_SETUP_DI_CLASS_DEVS");	break;
	case R4SERR_GET_SERIAL:	::sprintf(&TextBuffer, "Rockey4[%s]", "R4SERR_GET_SERIAL");	break;
	case R4SERR_GET_PRODUCT_STRING:	::sprintf(&TextBuffer, "Rockey4[%s]", "R4SERR_GET_PRODUCT_STRING");	break;
	case R4SERR_TOO_LONG_DEVICE_DETAIL:	::sprintf(&TextBuffer, "Rockey4[%s]", "R4SERR_TOO_LONG_DEVICE_DETAIL");	break;
	case R4SERR_UNKNOWN_DEVICE:	::sprintf(&TextBuffer, "Rockey4[%s]", "R4SERR_UNKNOWN_DEVICE");	break;
	case R4SERR_VERIFY:	::sprintf(&TextBuffer, "Rockey4[%s]", "R4SERR_VERIFY");	break;
	case R4SERR_ERASE:	::sprintf(&TextBuffer, "Rockey4[%s]", "R4SERR_ERASE");	break;
	case RY4SERR_INVALID_RY4S:	::sprintf(&TextBuffer, "Rockey4[%s]", "RY4SERR_INVALID_RY4S");	break;
	case R4SERR_UNKNOWN_ERROR:	::sprintf(&TextBuffer, "Rockey4[%s]", "R4SERR_UNKNOWN_ERROR");	break;

	case ERR_UPFILE_CRC:	::sprintf(&TextBuffer, "Rockey4[%s]", "ERR_UPFILE_CRC");	break;
	case ERR_UPFILE_UID:	::sprintf(&TextBuffer, "Rockey4[%s]", "ERR_UPFILE_UID");	break;
	case ERR_UPFILE_HID:	::sprintf(&TextBuffer, "Rockey4[%s]", "ERR_UPFILE_HID");	break;
	case ERR_UPFILE_MOD:	::sprintf(&TextBuffer, "Rockey4[%s]", "ERR_UPFILE_MOD");	break;
	case ERR_UPFILE_DATE:	::sprintf(&TextBuffer, "Rockey4[%s]", "ERR_UPFILE_DATE");	break;
	case ERR_UPDATE:	::sprintf(&TextBuffer, "Rockey4[%s]", "ERR_UPDATE");	break;
	case ERR_UPDATE_OVERFLOW:	::sprintf(&TextBuffer, "Rockey4[%s]", "ERR_UPDATE_OVERFLOW");	break;
	case ERR_UPDATE_UNKNOWN_CMD:	::sprintf(&TextBuffer, "Rockey4[%s]", "ERR_UPDATE_UNKNOWN_CMD");	break;
	case ERR_UPDATE_INVALID:	::sprintf(&TextBuffer, "Rockey4[%s]", "ERR_UPDATE_INVALID");	break;	

	case ERR_ACTIVATION:	::sprintf(&TextBuffer, "Rockey4[%s]", "ERR_ACTIVATION");	break;
	case ERROR_PASSWORD:	::sprintf(&TextBuffer, "Rockey4[%s]", "ERROR_PASSWORD");	break;
	case ERROR_DESCENDING_IS_NOT_ALLOWED:	::sprintf(&TextBuffer, "Rockey4[%s]", "ERROR_DESCENDING_IS_NOT_ALLOWED");	break;
	case ERROR_WITHOUT_DUE_AUTHORITY:	::sprintf(&TextBuffer, "Rockey4[%s]", "ERROR_WITHOUT_DUE_AUTHORITY");	break;
	case ERROR_COUNT_MODULE_ISZERO:	::sprintf(&TextBuffer, "Rockey4[%s]", "ERROR_COUNT_MODULE_ISZERO");	break;
	case ERROR_CHANGE_PWD:	::sprintf(&TextBuffer, "Rockey4[%s]", "ERROR_CHANGE_PWD");	break;
	case ERR_INDEX:	::sprintf(&TextBuffer, "Rockey4[%s]", "ERR_INDEX");	break;
	case ERR_AUTH_TYPE:	::sprintf(&TextBuffer, "Rockey4[%s]", "ERR_AUTH_TYPE");	break;
	case ERR_AUTH_VIRGIN:	::sprintf(&TextBuffer, "Rockey4[%s]", "ERR_AUTH_VIRGIN");	break;
	case ERR_AUTH_EXPIRED:	::sprintf(&TextBuffer, "Rockey4[%s]", "ERR_AUTH_EXPIRED");	break;
	case ERR_RSA:	::sprintf(&TextBuffer, "Rockey4[%s]", "ERR_RSA");	break;
	case R4S_ERR_UNKNOWN:	::sprintf(&TextBuffer, "Rockey4[%s]", "R4S_ERR_UNKNOWN");	break;
	case R4S_ERR_ADDR:	::sprintf(&TextBuffer, "Rockey4[%s]", "R4S_ERR_ADDR");	break;

	case ERR_RECEIVE_NULL:	::sprintf(&TextBuffer, "Rockey4[%s]", "ERR_RECEIVE_NULL");	break;
	case ERR_INVALID_BUFFER:	::sprintf(&TextBuffer, "Rockey4[%s]", "ERR_INVALID_BUFFER");	break;
	case ERR_UNKNOWN_SYSTEM:	::sprintf(&TextBuffer, "Rockey4[%s]", "ERR_UNKNOWN_SYSTEM");	break;
	case ERROR_UNINIT_TIME_UNIT:	::sprintf(&TextBuffer, "Rockey4[%s]", "ERROR_UNINIT_TIME_UNIT");	break;
	case ERR_UNKNOWN:	::sprintf(&TextBuffer, "Rockey4[%s]", "ERR_UNKNOWN");	break;
	default:
		::sprintf(&TextBuffer, "Rockey4[Undefined:%d]", RetCode);
		break;
	}
	if ( ERR_SUCCESS == RetCode ) { return true; }
	::sprintf(m_ErrorString, "Error, %s", &TextBuffer);
	return false;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND::SetHardIndex(int val)
{
	if ( val<0 || val>=ROCKEY4_MAX_COUNT ) 
	{
		::sprintf(m_ErrorString, "Error, HID Exception[%d]", val);
		return false; 
	}
	
	m_HardIndex = val;
	return true;
}
//-------------------------------------------------------------------------------------//
int CRockey4ND::GetHardIndex() const
{
	return m_HardIndex;
}
//-------------------------------------------------------------------------------------//
void CRockey4ND::SetHardNumber(DWORD lp1)
{
	m_HardNumber[m_HardIndex] = lp1;
}
//-------------------------------------------------------------------------------------//
void CRockey4ND::SetLParam(DWORD LP1, DWORD LP2)
{
	m_LParam[0] = LP1;
	m_LParam[1] = LP2;
}
//-------------------------------------------------------------------------------------//
void CRockey4ND::GetLParam(DWORD &LP1, DWORD &LP2) const
{
	LP1 = m_LParam[0];
	LP2 = m_LParam[1];
}
//-------------------------------------------------------------------------------------//
void CRockey4ND::SetParam(WORD P1, WORD P2, WORD P3, WORD P4)
{
	m_Param[0] = P1;
	m_Param[1] = P2;
	m_Param[2] = P3;
	m_Param[3] = P4;
}
//-------------------------------------------------------------------------------------//
void CRockey4ND::GetParam(WORD &P1, WORD &P2, WORD &P3, WORD &P4) const
{
	P1 = m_Param[0];
	P2 = m_Param[1];
	P3 = m_Param[2];
	P4 = m_Param[3];	
}
//-------------------------------------------------------------------------------------//
WORD CRockey4ND::ExecRockeyFnc(int fnID, unsigned char Buffer[])
{
	m_RetCode = Rockey(fnID, &m_Handle[m_HardIndex], &m_LParam[0], &m_LParam[1], &m_Param[0], &m_Param[1], &m_Param[2], &m_Param[3], Buffer);	
	return m_RetCode;	
}
//-------------------------------------------------------------------------------------//
WORD CRockey4ND::GetRetCode() const
{
	return m_RetCode;
}
//-------------------------------------------------------------------------------------//
const char* CRockey4ND::GetErrorString() const
{
	return m_ErrorString;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND::SupportDevelopVersion()
{
#ifdef ROCKEY4_DEVELOPER
	return true;
#endif//ROCKEY4_DEVELOPER
	::strcpy(m_ErrorString, "Error, the Function Only For Develop");	
	return false;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND::GetBaseDateTime(SYSTEMTIME &CurTime)
{
	CurTime.wYear = 2006;
	CurTime.wMonth = 1;
	CurTime.wDay   = 1;
	CurTime.wHour  = 0;
	CurTime.wMinute = 0;
	CurTime.wSecond = 0;
	CurTime.wMilliseconds = 0;
	CurTime.wDayOfWeek = 0;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND::GetCurrentDateTime(SYSTEMTIME &CurTime)
{
#if CURRENT_TIME_MODE == CURRENT_TIME_LOCAL
	::GetLocalTime(&CurTime);
#else
	::GetSystemTime(&CurTime);	
#endif//CURRENT_TIME_MODE	
	return true;
}
//-------------------------------------------------------------------------------------//
bool  CRockey4ND::CalcElapsedTime_Day(const SYSTEMTIME &t1, const SYSTEMTIME &t2, DWORD &dwDay)//以日為主
{
	FILETIME ft1;	
	FILETIME ft2;	
	ULARGE_INTEGER tmp1;	
	ULARGE_INTEGER tmp2;	
	ULARGE_INTEGER tmpD;	
	__int64 day_ns=(__int64)10000000*60*60*24; //1 day in 100 ns intervals

	SystemTimeToFileTime(&t1, &ft1);
	SystemTimeToFileTime(&t2, &ft2);
	tmp1.LowPart = ft1.dwLowDateTime;
	tmp1.HighPart = ft1.dwHighDateTime;
	tmp2.LowPart = ft2.dwLowDateTime;
	tmp2.HighPart = ft2.dwHighDateTime;
	tmpD.QuadPart = tmp2.QuadPart-tmp1.QuadPart;

	tmpD.QuadPart = tmpD.QuadPart/day_ns;
	dwDay = (DWORD)(tmpD.QuadPart&0xFFFFFFFF);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND::CalcElapsedTime_Hour(const SYSTEMTIME &t1, const SYSTEMTIME &t2, DWORD &dwHour)//以小時為主
{
	FILETIME ft1;	
	FILETIME ft2;	
	ULARGE_INTEGER tmp1;	
	ULARGE_INTEGER tmp2;	
	ULARGE_INTEGER tmpD;	
	__int64 hour_ns=(__int64)10000000*60*60; //1 hour in 100 ns intervals

	SystemTimeToFileTime(&t1, &ft1);
	SystemTimeToFileTime(&t2, &ft2);
	tmp1.LowPart = ft1.dwLowDateTime;
	tmp1.HighPart = ft1.dwHighDateTime;
	tmp2.LowPart = ft2.dwLowDateTime;
	tmp2.HighPart = ft2.dwHighDateTime;
	tmpD.QuadPart = tmp2.QuadPart-tmp1.QuadPart;

	tmpD.QuadPart = tmpD.QuadPart/(hour_ns);
	dwHour = (DWORD)(tmpD.QuadPart&0xFFFFFFFF);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND::CalcElapsedTime_Minute(const SYSTEMTIME &t1, const SYSTEMTIME &t2, DWORD &dwMinutes)//以分鐘為主
{
	FILETIME ft1;	
	FILETIME ft2;	
	ULARGE_INTEGER tmp1;	
	ULARGE_INTEGER tmp2;	
	ULARGE_INTEGER tmpD;	
	__int64 min_ns=(__int64)10000000*60; //1 minute in 100 ns intervals

	SystemTimeToFileTime(&t1, &ft1);
	SystemTimeToFileTime(&t2, &ft2);
	tmp1.LowPart = ft1.dwLowDateTime;
	tmp1.HighPart = ft1.dwHighDateTime;
	tmp2.LowPart = ft2.dwLowDateTime;
	tmp2.HighPart = ft2.dwHighDateTime;
	tmpD.QuadPart = tmp2.QuadPart-tmp1.QuadPart;

	tmpD.QuadPart = tmpD.QuadPart/(min_ns);
	dwMinutes = (DWORD)(tmpD.QuadPart&0xFFFFFFFF);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND::CalcLastDateByDay(const SYSTEMTIME &CurTime, DWORD Days, SYSTEMTIME &NextTime)//計算之前日期-天數
{
	FILETIME ft;	
	ULARGE_INTEGER tmp;	
	__int64 day_ns=(__int64)10000000*60*60*24; //1 day in 100 ns intervals

	SystemTimeToFileTime(&CurTime, &ft);
	tmp.LowPart = ft.dwLowDateTime;
	tmp.HighPart = ft.dwHighDateTime;

	tmp.QuadPart -= (Days*day_ns); // time/86400 time in to days 

	ft.dwLowDateTime = tmp.LowPart;
	ft.dwHighDateTime = tmp.HighPart;

	FileTimeToSystemTime(&ft, &NextTime);
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND::CalcLastDateByHour(const SYSTEMTIME &CurTime, DWORD Hours, SYSTEMTIME &NextTime)//計算之前日期-小時
{
	FILETIME ft;	
	ULARGE_INTEGER tmp;	
	__int64 day_ns=(__int64)10000000*60*60*24; //1 day in 100 ns intervals
	__int64 day_hour=(__int64)(day_ns/24);

	SystemTimeToFileTime(&CurTime, &ft);
	tmp.LowPart = ft.dwLowDateTime;
	tmp.HighPart = ft.dwHighDateTime;

	//tmp.QuadPart -= (Hours*day_ns/24); // time/24 time in to days 
	//tmp.QuadPart -= (Hours/24)*day_ns; // time/24 time in to days 
	tmp.QuadPart -= (Hours*day_hour); // time/24 time in to days 

	ft.dwLowDateTime = tmp.LowPart;
	ft.dwHighDateTime = tmp.HighPart;

	FileTimeToSystemTime(&ft, &NextTime);	
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND::CalcLastDateByMinute(const SYSTEMTIME &CurTime, DWORD Minutes, SYSTEMTIME &NextTime)//計算之前日期-分鐘
{
	FILETIME ft;	
	ULARGE_INTEGER tmp;	
	__int64 day_ns=(__int64)10000000*60*60*24; //1 day in 100 ns intervals
	__int64 day_min=(__int64)(day_ns/1440);

	SystemTimeToFileTime(&CurTime, &ft);
	tmp.LowPart = ft.dwLowDateTime;
	tmp.HighPart = ft.dwHighDateTime;

	//tmp.QuadPart -= (Minutes*day_ns/1440); // time/1440 time in to days 
	//tmp.QuadPart -= (Minutes/1440)*day_ns; // time/1440 time in to days 
	tmp.QuadPart -= (Minutes*day_min); // time/1440 time in to days 

	ft.dwLowDateTime = tmp.LowPart;
	ft.dwHighDateTime = tmp.HighPart;

	FileTimeToSystemTime(&ft, &NextTime);		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND::CalcLastDateBySecond(const SYSTEMTIME &CurTime, DWORD Seconds, SYSTEMTIME &NextTime)//計算之前日期-秒數
{
	FILETIME ft;	
	ULARGE_INTEGER tmp;	
	__int64 day_ns=(__int64)10000000*60*60*24; //1 day in 100 ns intervals
	__int64 day_sec=(__int64)(day_ns/86400);
	 

	SystemTimeToFileTime(&CurTime, &ft);
	tmp.LowPart = ft.dwLowDateTime;
	tmp.HighPart = ft.dwHighDateTime;

	//tmp.QuadPart -= (Seconds*day_ns/86400); // time/86400 time in to days 
	//tmp.QuadPart -= (Seconds/86400)*day_ns; // time/86400 time in to days 
	tmp.QuadPart -= (Seconds*day_sec); // time/86400 time in to days 

	ft.dwLowDateTime = tmp.LowPart;
	ft.dwHighDateTime = tmp.HighPart;

	FileTimeToSystemTime(&ft, &NextTime);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND::CalcNextDateByDay(const SYSTEMTIME &CurTime, DWORD Days, SYSTEMTIME &NextTime)
{
	FILETIME ft;	
	ULARGE_INTEGER tmp;	
	__int64 day_ns=(__int64)10000000*60*60*24; //1 day in 100 ns intervals

	SystemTimeToFileTime(&CurTime, &ft);
	tmp.LowPart = ft.dwLowDateTime;
	tmp.HighPart = ft.dwHighDateTime;

	tmp.QuadPart += (Days*day_ns); // time/86400 time in to days 

	ft.dwLowDateTime = tmp.LowPart;
	ft.dwHighDateTime = tmp.HighPart;

	FileTimeToSystemTime(&ft, &NextTime);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND::CalcNextDateByHour(const SYSTEMTIME &CurTime, DWORD Hours, SYSTEMTIME &NextTime)
{
	FILETIME ft;	
	ULARGE_INTEGER tmp;	
	__int64 day_ns=(__int64)10000000*60*60*24; //1 day in 100 ns intervals
	__int64 day_hour=(__int64)(day_ns/24);

	SystemTimeToFileTime(&CurTime, &ft);
	tmp.LowPart = ft.dwLowDateTime;
	tmp.HighPart = ft.dwHighDateTime;

	//tmp.QuadPart += (Hours*day_ns/24); // time/24 time in to days 
	//tmp.QuadPart += (Hours/24)*day_ns; // time/24 time in to days 
	tmp.QuadPart += (Hours*day_hour); // time/24 time in to days 

	ft.dwLowDateTime = tmp.LowPart;
	ft.dwHighDateTime = tmp.HighPart;

	FileTimeToSystemTime(&ft, &NextTime);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND::CalcNextDateByMinute(const SYSTEMTIME &CurTime, DWORD Minutes, SYSTEMTIME &NextTime)
{
	FILETIME ft;	
	ULARGE_INTEGER tmp;	
	__int64 day_ns=(__int64)10000000*60*60*24; //1 day in 100 ns intervals
	__int64 day_min=(__int64)(day_ns/1440);

	SystemTimeToFileTime(&CurTime, &ft);
	tmp.LowPart = ft.dwLowDateTime;
	tmp.HighPart = ft.dwHighDateTime;

	//tmp.QuadPart += (Minutes*day_ns/1440); // time/1440 time in to days 
	//tmp.QuadPart += (Minutes/1440)*day_ns; // time/1440 time in to days 
	tmp.QuadPart += (Minutes*day_min); // time/1440 time in to days 

	ft.dwLowDateTime = tmp.LowPart;
	ft.dwHighDateTime = tmp.HighPart;

	FileTimeToSystemTime(&ft, &NextTime);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND::CalcNextDateBySecond(const SYSTEMTIME &CurTime, DWORD Seconds, SYSTEMTIME &NextTime)
{
	FILETIME ft;	
	ULARGE_INTEGER tmp;	
	__int64 day_ns=(__int64)10000000*60*60*24; //1 day in 100 ns intervals
	__int64 day_sec=(__int64)(day_ns/86400);
	 

	SystemTimeToFileTime(&CurTime, &ft);
	tmp.LowPart = ft.dwLowDateTime;
	tmp.HighPart = ft.dwHighDateTime;

	//tmp.QuadPart += (Seconds*day_ns/86400); // time/86400 time in to days 
	//tmp.QuadPart += (Seconds/86400)*day_ns; // time/86400 time in to days 
	tmp.QuadPart += (Seconds*day_sec); // time/86400 time in to days 

	ft.dwLowDateTime = tmp.LowPart;
	ft.dwHighDateTime = tmp.HighPart;

	FileTimeToSystemTime(&ft, &NextTime);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND::OpenRockey4()
{	
	int  i=0;
	WORD HID=0;
	WORD retcode=0;
	DWORD lp1, lp2;
	BYTE Buffer[ROCKEY4_BUFFER_SIZE];	
	//P1=C44C P2=C8F8 P3=0799 P4=C43B
	SetParam(0xC44C, 0xC8F8, 0x0799, 0xC43B);

	CString str;
	for ( i=0; i<ROCKEY4_MAX_COUNT; i++ )
	{
		HID = (WORD)(i);
		SetHardIndex(HID);
	#ifndef ROCKEY4_DEVELOPER
		SetParam(0xA331, 0xA09C);//JET Code		
	#else
		SetParam(0xA331, 0xA09C, 0x530B, 0x9C93);//JET Code			
	#endif//ROCKEY4_DEVELOPER
		if ( 0 == i )
		{	
			retcode = ExecRockeyFnc(RY_FIND, Buffer);	
			if ( CheckRockey4NDRetCode(retcode) == false )
			{	return false; }
		}
		else
		{	
			SetLParam(lp1, lp2);
			retcode = ExecRockeyFnc(RY_FIND_NEXT, Buffer);	
			if ( ERR_NOMORE == retcode )
			{	break; }
			if ( ERR_SUCCESS != retcode )
			{
				CloseRockey4();
				return false;
			}
		}
		
		GetLParam(lp1, lp2);
		SetHardNumber(lp1);
		str.Format(_T("Find Rock: %08X"), lp1);			
		CheckRockey4Type(lp2);
		
		retcode = ExecRockeyFnc(RY_OPEN, Buffer);
		if ( CheckRockey4NDRetCode(retcode) == false )
		{
			CloseRockey4();
			return false;
		}
	#ifdef ROCKEY4_DEVELOPER
		if ( WriteArithmetic_ReadModelContent(HID) == false )
		{
			CloseRockey4();
			return false;
		}
	#endif//ROCKEY4_DEVELOPER
	}
	SetHardIndex(0);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND::CloseRockey4()
{
	int  i=0;	
	for ( i=ROCKEY4_MAX_COUNT-1; -1!=i; i-- )
	{
		if ( ROCKEY4_INVALID == m_Handle[i] ) { continue; }
		CloseHID(i);		
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND::ReadUserID(DWORD &val, WORD HID)
{
	WORD retcode=0;	
	DWORD lp1, lp2;
	BYTE Buffer[ROCKEY4_BUFFER_SIZE];			
	SetParam();
	SetLParam();
	if ( SetHardIndex(HID) == false ) { return false; }
	retcode = ExecRockeyFnc(RY_READ_USERID, Buffer);
	if ( CheckRockey4NDRetCode(retcode) == false )
	{	return false;	}	
	GetLParam(lp1, lp2);
	val = lp1;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND::WriteUserID(DWORD val, WORD HID)
{
	WORD retcode=0;
	BYTE Buffer[ROCKEY4_BUFFER_SIZE];			
	SetParam();
	SetLParam(val);
	if ( SetHardIndex(HID) == false ) { return false; }
	retcode = ExecRockeyFnc(RY_WRITE_USERID, Buffer);
	if ( CheckRockey4NDRetCode(retcode) == false )
	{	return false;	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND::CreateRandom(int &v1, WORD HID)
{
	int v2=0, v3=0, v4=0;
	if ( CreateRandom(v1, v2, v3, v4, HID) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND::CreateRandom(int &v1, int &v2, WORD HID)
{
	int v3=0, v4=0;
	if ( CreateRandom(v1, v2, v3, v4, HID) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND::CreateRandom(int &v1, int &v2, int &v3, WORD HID)
{
	int v4=0;
	if ( CreateRandom(v1, v2, v3, v4, HID) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND::CreateRandom(int &v1, int &v2, int &v3, int &v4, WORD HID)
{	
	WORD retcode=0;	
	WORD p1,p2,p3,p4;
	BYTE Buffer[ROCKEY4_BUFFER_SIZE];			
	SetParam();
	if ( SetHardIndex(HID) == false ) { return false; }
	retcode = ExecRockeyFnc(RY_RANDOM, Buffer);
	if ( CheckRockey4NDRetCode(retcode) == false )
	{	return false;	}
	GetParam(p1,p2,p3,p4);
	v1 = p1;	v2 = p2;	v3 = p3;	v4 = p4;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND::GetSeedCode(int val, int &v1, int &v2, int &v3, int &v4, WORD HID)
{	//可以利用Seed產出的4個數值來進行加密(new_val=val+v1+v2+v3+v4)與解密(old_val=val-v1-v2-v3-v4)
	WORD retcode=0;	
	WORD p1,p2,p3,p4;
	BYTE Buffer[ROCKEY4_BUFFER_SIZE];			
	SetParam();
	SetLParam(0, val);
	if ( SetHardIndex(HID) == false ) { return false; }
	retcode = ExecRockeyFnc(RY_SEED, Buffer);
	if ( CheckRockey4NDRetCode(retcode) == false )
	{	return false;	}
	GetParam(p1,p2,p3,p4);
	v1 = p1;	v2 = p2;	v3 = p3;	v4 = p4;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND::ReadDataMemory(int Pos, WORD &Data, WORD HID)
{
	WORD retcode=0;	
	BYTE Buffer[ROCKEY4_BUFFER_SIZE];			
	WORD Len = (WORD)(sizeof(WORD));
	SetParam((WORD)Pos, (WORD)Len);
	if ( SetHardIndex(HID) == false ) { return false; }
	retcode = ExecRockeyFnc(RY_READ_EX, Buffer);//RY_READ, RY_READ_EX
	if ( CheckRockey4NDRetCode(retcode) == false )
	{	return false;	}
	Buffer[Len]='\0';
	::memcpy(&Data, Buffer, Len);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND::ReadDataMemory(int Pos, DWORD &Data, WORD HID)
{
	WORD retcode=0;	
	BYTE Buffer[ROCKEY4_BUFFER_SIZE];			
	WORD Len = (WORD)(sizeof(DWORD));
	SetParam((WORD)Pos, (WORD)Len);
	if ( SetHardIndex(HID) == false ) { return false; }
	retcode = ExecRockeyFnc(RY_READ_EX, Buffer);//RY_READ, RY_READ_EX
	if ( CheckRockey4NDRetCode(retcode) == false )
	{	return false;	}
	Buffer[Len]='\0';
	::memcpy(&Data, Buffer, Len);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND::ReadDataMemory(int Pos, unsigned char &Data, WORD HID)
{
	WORD retcode=0;	
	BYTE Buffer[ROCKEY4_BUFFER_SIZE];			
	WORD Len = (WORD)(sizeof(unsigned char));
	SetParam((WORD)Pos, (WORD)Len);
	if ( SetHardIndex(HID) == false ) { return false; }
	retcode = ExecRockeyFnc(RY_READ_EX, Buffer);//RY_READ, RY_READ_EX
	if ( CheckRockey4NDRetCode(retcode) == false )
	{	return false;	}
	Buffer[Len]='\0';
	::memcpy(&Data, Buffer, Len);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND::ReadDataMemory(int Pos, int Len, char Data[], WORD HID)
{	
	WORD retcode=0;	
	BYTE Buffer[ROCKEY4_BUFFER_SIZE];			
	SetParam((WORD)Pos, (WORD)Len);
	if ( SetHardIndex(HID) == false ) { return false; }
	retcode = ExecRockeyFnc(RY_READ_EX, Buffer);//RY_READ, RY_READ_EX
	if ( CheckRockey4NDRetCode(retcode) == false )
	{	return false;	}
	Buffer[Len]='\0';
	::memcpy(Data, Buffer, Len+1);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND::WriteDataMemory(int Pos, WORD Data, WORD HID)
{
	WORD retcode=0;
	WORD p1=0, p2=0;
	BYTE Buffer[ROCKEY4_BUFFER_SIZE];		
	const WORD Len = (WORD)(sizeof(WORD));
	::memcpy((char*)Buffer, &Data, Len);	
	p1 = (WORD)(Pos);// Memory Position
	p2 = (WORD)(Len);// Data Length
	SetParam(p1, p2);
	if ( SetHardIndex(HID) == false ) { return false; }
	retcode = ExecRockeyFnc(RY_WRITE_EX, Buffer);//RY_WRITE, RY_WRITE_EX
	if ( CheckRockey4NDRetCode(retcode) == false )
	{	return false;	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND::WriteDataMemory(int Pos, DWORD Data, WORD HID)
{
	WORD retcode=0;
	WORD p1=0, p2=0;
	BYTE Buffer[ROCKEY4_BUFFER_SIZE];		
	const WORD Len = (WORD)(sizeof(DWORD));
	::memcpy((char*)Buffer, &Data, Len);	
	p1 = (WORD)(Pos);// Memory Position
	p2 = (WORD)(Len);// Data Length
	SetParam(p1, p2);
	if ( SetHardIndex(HID) == false ) { return false; }
	retcode = ExecRockeyFnc(RY_WRITE_EX, Buffer);//RY_WRITE, RY_WRITE_EX
	if ( CheckRockey4NDRetCode(retcode) == false )
	{	return false;	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND::WriteDataMemory(int Pos, unsigned char Data, WORD HID)
{
	WORD retcode=0;
	WORD p1=0, p2=0;
	BYTE Buffer[ROCKEY4_BUFFER_SIZE];		
	const WORD Len = (WORD)(sizeof(unsigned char));
	::memcpy((char*)Buffer, &Data, Len);	
	p1 = (WORD)(Pos);// Memory Position
	p2 = (WORD)(Len);// Data Length
	SetParam(p1, p2);
	if ( SetHardIndex(HID) == false ) { return false; }
	retcode = ExecRockeyFnc(RY_WRITE_EX, Buffer);//RY_WRITE, RY_WRITE_EX
	if ( CheckRockey4NDRetCode(retcode) == false )
	{	return false;	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND::WriteDataMemory(int Pos, const char Data[], WORD HID)
{	
	WORD retcode=0;
	WORD p1=0, p2=0;
	BYTE Buffer[ROCKEY4_BUFFER_SIZE];		
	strcpy((char*)Buffer, Data);	
	p1 = (WORD)(Pos);// Memory Position
	p2 = (WORD)(::strlen(Data));// Data Length
	SetParam(p1, p2);
	if ( SetHardIndex(HID) == false ) { return false; }
	retcode = ExecRockeyFnc(RY_WRITE_EX, Buffer);//RY_WRITE, RY_WRITE_EX
	if ( CheckRockey4NDRetCode(retcode) == false )
	{	return false;	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND::WriteDataMemory(int Pos, int Len, const unsigned char Data[], WORD HID)
{
	WORD retcode=0;
	WORD p1=0, p2=0;
	BYTE Buffer[ROCKEY4_BUFFER_SIZE];
	::memcpy(Buffer, Data, Len);
	p1 = (WORD)(Pos);// Memory Position
	p2 = (WORD)(Len);// Data Length
	SetParam(p1, p2);
	if ( SetHardIndex(HID) == false ) { return false; }
	retcode = ExecRockeyFnc(RY_WRITE_EX, Buffer);//RY_WRITE, RY_WRITE_EX
	if ( CheckRockey4NDRetCode(retcode) == false )
	{	return false;	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND::DecreaseModule(int ID, WORD HID)//ModuleID(0~63), AutoDecrease(0,1)
{
	WORD retcode=0;	
	BYTE Buffer[ROCKEY4_BUFFER_SIZE];	
	SetLParam();
	SetParam((WORD)(ID));
	if ( SetHardIndex(HID) == false ) { return false; }
	retcode = ExecRockeyFnc(RY_DECREASE, Buffer);
	if ( CheckRockey4NDRetCode(retcode) == false )
	{	return false;	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND::SetModule(int ID, int Content, int AutoDecrease, WORD HID)//ModuleID(0~63), AutoDecrease(0,1)
{	
	WORD retcode=0;	
	BYTE Buffer[ROCKEY4_BUFFER_SIZE];	
	SetLParam();
	SetParam((WORD)(ID), (WORD)(Content), (WORD)(AutoDecrease));
	if ( SetHardIndex(HID) == false ) { return false; }
	retcode = ExecRockeyFnc(RY_SET_MODULE, Buffer);
	if ( CheckRockey4NDRetCode(retcode) == false )
	{	return false;	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND::CheckModule(int ID, int &Content, int &AutoDecrease, WORD HID)//ModuleID(0~63), AutoDecrease(0,1)
{	//只能確認Module的狀態是可用, 無法取得數量
	WORD retcode=0;	
	WORD p1,p2,p3,p4;
	BYTE Buffer[ROCKEY4_BUFFER_SIZE];	
	SetLParam();
	SetParam((WORD)(ID));
	if ( SetHardIndex(HID) == false ) { return false; }
	retcode = ExecRockeyFnc(RY_CHECK_MODULE, Buffer);
	if ( CheckRockey4NDRetCode(retcode) == false )
	{	return false;	}	
	GetParam(p1,p2,p3,p4);
	Content=p2;
	AutoDecrease=p3;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND::ReadModuleContent(int ID, int &Content, WORD HID)////ModuleID(0~63)
{
	//::sprintf(m_ErrorString, "Error, Not Support Read Model Content");
	//return false;
	BYTE  Buffer[ROCKEY4_BUFFER_SIZE];
	int AlgID=0;
	int v1=0, v2=0, v3=0, v4=0;
	const size_t Len=sizeof(WORD);

#ifdef ROCKEY4_DEVELOPER
	AlgID=ROCKEY4_ALG_ID_CALCULATE_1;	
	int t1=0, t2=0, t3=0, t4=0;
	if ( 1 == HID )
	{	HID = HID; }
	if ( CRockey4ND::CalculateByModule(AlgID, ID, t1, t2, t3, t4, HID) == false )
	{	return false;	}
#endif//ROCKEY4_DEVELOPER	


	AlgID=ROCKEY4_ALG_ID_READ_MODULE;	
	::memcpy(Buffer, &m_HardNumber[HID], sizeof(m_HardNumber[HID]));
	::memcpy(&v2, &Buffer, Len);
	::memcpy(&v1, &(Buffer[Len]), Len);	
	v3 = 1;
	v4 = 0;
	if ( CRockey4ND::CalculateByModule(AlgID, ID, v1, v2, v3, v4, HID) == false )
	{	return false;	}
	if ( 0 != v1 || 0!=v2 )
	{
		::sprintf(m_ErrorString, "Error, Rockey4 Module Content Exception[%d]", HID);
		return false; 
	}
	Content = v3;

	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND::WriteArithmetic(int AlgID, const char AlgFunc[], WORD HID)
{
	WORD retcode=0;		
	BYTE Buffer[ROCKEY4_BUFFER_SIZE];	
	SetLParam();
	SetParam((WORD)(AlgID));
	if ( SetHardIndex(HID) == false ) { return false; }
	strcpy((char*)Buffer, AlgFunc);
	retcode = ExecRockeyFnc(RY_WRITE_ARITHMETIC, Buffer);
	if ( CheckRockey4NDRetCode(retcode) == false )
	{	return false;	}		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND::WriteArithmetic_ReadModelContent(WORD HID)//寫入演算法公式-讀取模組內容 
{
#ifdef ROCKEY4_DEVELOPER
	int     AlgID1=ROCKEY4_ALG_ID_READ_MODULE;
	char    Cmd1[]="A=A-E, B=B-F, C=C*G, D=D+H";//共佔用4個指令
	if ( CRockey4ND::WriteArithmetic(AlgID1, Cmd1, HID) == false )
	{	return false;	}	
	//return true;

	int     AlgID2=ROCKEY4_ALG_ID_CALCULATE_1;
	char    Cmd2[]="A=E|E, B=F|F, C=G|G, D=H|H";//共佔用4個指令
	if ( CRockey4ND::WriteArithmetic(AlgID2, Cmd2, HID) == false )
	{	return false;	}	
#else
	return SupportDevelopVersion();
#endif//ROCKEY4_DEVELOPER
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND::CalculateBySeed(int AlgID, int Seed, int &v1, int &v2, int &v3, int &v4, WORD HID)
{	//A=v1, B=v2, C=v3, D-v4, E=Seed-p1, F=Seed-p2, G=Seed-p3, H=Seed-p4
	WORD retcode=0;
	WORD p1,p2,p3,p4;
	BYTE Buffer[ROCKEY4_BUFFER_SIZE];	
	SetLParam((DWORD)AlgID, (DWORD)Seed);
	SetParam((WORD)v1, (WORD)v2, (WORD)v3, (WORD)v4);
	if ( SetHardIndex(HID) == false ) { return false; }
	retcode = ExecRockeyFnc(RY_CALCULATE2, Buffer);
	if ( CheckRockey4NDRetCode(retcode) == false )
	{	return false;	}
	GetParam(p1,p2,p3,p4);
	v1 = p1;	v2 = p2;	v3 = p3;	v4 = p4;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND::CalculateByModule(int AlgID, int ModuleID, int &v1, int &v2, int &v3, int &v4, WORD HID)
{	//A=v1, B=v2, C=v3, D-v4, E=Hi(HID), F=Lo(HID), G=MID, H=Random
	WORD retcode=0;
	WORD p1,p2,p3,p4;
	BYTE Buffer[ROCKEY4_BUFFER_SIZE];	
	SetLParam((DWORD)AlgID, (DWORD)ModuleID);
	SetParam((WORD)v1, (WORD)v2, (WORD)v3, (WORD)v4);
	if ( SetHardIndex(HID) == false ) { return false; }
	retcode = ExecRockeyFnc(RY_CALCULATE1, Buffer);
	if ( CheckRockey4NDRetCode(retcode) == false )
	{	return false;	}
	GetParam(p1,p2,p3,p4);
	v1 = p1;	v2 = p2;	v3 = p3;	v4 = p4;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND::CalculateByModuleList(int AlgID, int FirstModuleID, int &v1, int &v2, int &v3, int &v4, WORD HID)
{	//A=v1, B=v2, C=v3, D-v4, E=MID1_Content, F=MID2_Content, G=MID3_Content, H=MID4_Content
	WORD retcode=0;
	WORD p1,p2,p3,p4;
	BYTE Buffer[ROCKEY4_BUFFER_SIZE];	
	SetLParam((DWORD)AlgID, (DWORD)FirstModuleID);
	SetParam((WORD)v1, (WORD)v2, (WORD)v3, (WORD)v4);
	if ( SetHardIndex(HID) == false ) { return false; }
	retcode = ExecRockeyFnc(RY_CALCULATE3, Buffer);
	if ( CheckRockey4NDRetCode(retcode) == false )
	{	return false;	}
	GetParam(p1,p2,p3,p4);
	v1 = p1;	v2 = p2;	v3 = p3;	v4 = p4;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND::SetDesKey(int DESMode, const char Key[], WORD HID)//DES:0, 3DES:1
{
	WORD retcode=0;		
	BYTE Buffer[ROCKEY4_BUFFER_SIZE];	
	SetLParam();
	SetParam((WORD)(DESMode));
	if ( SetHardIndex(HID) == false ) { return false; }
	strcpy((char*)Buffer, Key);
	retcode = ExecRockeyFnc(RY_SET_DES_KEY, Buffer);
	if ( CheckRockey4NDRetCode(retcode) == false )
	{	return false;	}		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND::EncryptDes(int DESMode, int len, const char Data[], char Encrypt[], WORD HID)//DES:0, 3DES:1, len:8n
{
	WORD retcode=0;		
	BYTE Buffer[ROCKEY4_BUFFER_SIZE];	
	SetLParam();
	SetParam((WORD)(DESMode), (WORD)len);
	if ( SetHardIndex(HID) == false ) { return false; }
	strcpy((char*)Buffer, Data);
	::memset(Buffer, 0x00, sizeof(Buffer));
	retcode = ExecRockeyFnc(RY_DES_ENC, Buffer);
	if ( CheckRockey4NDRetCode(retcode) == false )
	{	return false;	}		
	::strcpy(Encrypt, (char*)Buffer);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND::DecryptDes(int DESMode, int len, const char Data[], char Decrypt[], WORD HID)//DES:0, 3DES:1, len:8n	
{
	WORD retcode=0;		
	BYTE Buffer[ROCKEY4_BUFFER_SIZE];	
	SetLParam();
	SetParam((WORD)(DESMode), (WORD)len);
	if ( SetHardIndex(HID) == false ) { return false; }
	strcpy((char*)Buffer, Data);
	::memset(Buffer, 0x00, sizeof(Buffer));
	retcode = ExecRockeyFnc(RY_DES_DEC, Buffer);
	if ( CheckRockey4NDRetCode(retcode) == false )
	{	return false;	}	
	::strcpy(Decrypt, (char*)Buffer);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND::SetRsaKeyN(const char Key[], WORD HID)//設定RSA的金鑰-N
{
	WORD retcode=0;		
	BYTE Buffer[ROCKEY4_BUFFER_SIZE];	
	SetLParam();
	SetParam();
	if ( SetHardIndex(HID) == false ) { return false; }
	strcpy((char*)Buffer, Key);
	retcode = ExecRockeyFnc(RY_SET_RSAKEY_N, Buffer);
	if ( CheckRockey4NDRetCode(retcode) == false )
	{	return false;	}		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND::SetRsaKeyD(const char Key[], WORD HID)//設定RSA的金鑰-D
{
	WORD retcode=0;		
	BYTE Buffer[ROCKEY4_BUFFER_SIZE];	
	SetLParam();
	SetParam();
	if ( SetHardIndex(HID) == false ) { return false; }
	strcpy((char*)Buffer, Key);
	retcode = ExecRockeyFnc(RY_SET_RSAKEY_D, Buffer);
	if ( CheckRockey4NDRetCode(retcode) == false )
	{	return false;	}		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND::EncryptRsa(int KeyMode, int len, int PadMode, const char Data[], char Encrypt[], WORD HID)//以RSA加密
{
	WORD retcode=0;		
	BYTE Buffer[ROCKEY4_BUFFER_SIZE];	
	SetLParam();
	SetParam((WORD)KeyMode, (WORD)len, (WORD)PadMode);
	if ( SetHardIndex(HID) == false ) { return false; }
	strcpy((char*)Buffer, Data);
	::memset(Buffer, 0x00, sizeof(Buffer));
	retcode = ExecRockeyFnc(RY_RSA_ENC, Buffer);
	if ( CheckRockey4NDRetCode(retcode) == false )
	{	return false;	}
	::strcpy(Encrypt, (char*)Buffer);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND::DecryptRsa(int KeyMode, int len, int PadMode, const char Data[], char Decrypt[], WORD HID)//以RSA解密
{
	WORD retcode=0;		
	BYTE Buffer[ROCKEY4_BUFFER_SIZE];	
	SetLParam();
	SetParam((WORD)KeyMode, (WORD)len, (WORD)PadMode);
	if ( SetHardIndex(HID) == false ) { return false; }
	strcpy((char*)Buffer, Data);
	::memset(Buffer, 0x00, sizeof(Buffer));
	retcode = ExecRockeyFnc(RY_RSA_DEC, Buffer);
	if ( CheckRockey4NDRetCode(retcode) == false )
	{	return false;	}			
	::strcpy(Decrypt, (char*)Buffer);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND::SetCountEx(int ID, DWORD Count, int Decrease, WORD HID)//設定次數
{	
	WORD retcode=0;		
	BYTE Buffer[ROCKEY4_BUFFER_SIZE];	
	SetLParam();
	SetParam((WORD)ID, (WORD)0, (WORD)Decrease);
	if ( SetHardIndex(HID) == false ) { return false; }
	memcpy(Buffer, &Count, sizeof(DWORD));   	
	retcode = ExecRockeyFnc(RY_SET_COUNTER_EX, Buffer);
	if ( CheckRockey4NDRetCode(retcode) == false )
	{	return false;	}		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND::GetCountEx(int ID, DWORD &Count, int &Decrease, WORD HID)//取得次數
{
	WORD retcode=0;
	WORD p1,p2,p3,p4;
	BYTE Buffer[ROCKEY4_BUFFER_SIZE];	
	SetLParam();
	SetParam((WORD)ID);
	if ( SetHardIndex(HID) == false ) { return false; }
	retcode = ExecRockeyFnc(RY_GET_COUNTER_EX, Buffer);
	if ( CheckRockey4NDRetCode(retcode) == false )
	{	return false;	}	
	GetParam(p1,p2,p3,p4);	
	Decrease = p3;
	memcpy(&Count, Buffer, sizeof(DWORD));   	
	return true;
}
//-------------------------------------------------------------------------------------//
bool  CRockey4ND::GetElapsedTime(const SYSTEMTIME &time, DWORD &Ret, WORD HID)//相對於2006/01/01-00::00::00
{
	return GetElapsedTime(time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, Ret, HID);
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND::GetElapsedTime(DWORD Year, WORD Month, WORD Day, WORD Hour, WORD Minute, DWORD &Ret, WORD HID)
{
	WORD retcode=0;
	DWORD lp1=0, lp2=0;
	BYTE Buffer[ROCKEY4_BUFFER_SIZE];	
	SetLParam(0, Year);
	SetParam((WORD)Month, (WORD)Day, (WORD)Hour, (WORD) Minute);
	if ( SetHardIndex(HID) == false ) { return false; }
	retcode = ExecRockeyFnc(RY_GET_TIME_DWORD, Buffer);
	if ( CheckRockey4NDRetCode(retcode) == false )
	{	return false;	}		
	GetLParam(lp1, lp2);
	Ret = lp1;
	//*lp1 = 以2006-1-1 0:0 計時的分鐘數	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND::SetTimerEx(int ID, int Mode, SYSTEMTIME time, DWORD Hours, DWORD Days, WORD HID)//設定計時器, Mode:1:Date, 2:Hour, 3:Day
{
	WORD retcode=0;
	BYTE Buffer[ROCKEY4_BUFFER_SIZE];	
	SetLParam();
	SetParam((WORD)ID, (WORD)0, (WORD)Mode);
	if ( SetHardIndex(HID) == false ) { return false; }
	switch ( Mode )
	{
	case ROCKEY4_TIMER_BY_DAY:
		memcpy(Buffer, &Days, sizeof(DWORD));	
		break;
	case ROCKEY4_TIMER_BY_HOUR:
		memcpy(Buffer, &Hours, sizeof(DWORD));	
		break;
	default:
	case ROCKEY4_TIMER_BY_DATE:
		memcpy(Buffer, &time, sizeof(SYSTEMTIME));	
		break;
	}	
	retcode = ExecRockeyFnc(RY_SET_TIMER_EX, Buffer);
	if ( CheckRockey4NDRetCode(retcode) == false )
	{	return false;	}		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND::GetTimerEx(int ID, int &Mode, SYSTEMTIME &time, DWORD &Hours, DWORD &Days, WORD HID)//取得計時器, Mode:1:Day, 2:Hour, 3:Day
{
	WORD retcode=0;
	WORD p1,p2,p3,p4;
	BYTE Buffer[ROCKEY4_BUFFER_SIZE];	
	SetLParam();
	SetParam((WORD)ID);
	if ( SetHardIndex(HID) == false ) { return false; }
	retcode = ExecRockeyFnc(RY_GET_TIMER_EX, Buffer);
	if ( CheckRockey4NDRetCode(retcode) == false )
	{	return false;	}		
	GetParam(p1,p2,p3,p4);	
	Mode = p3;
	switch ( Mode )
	{
	case ROCKEY4_TIMER_BY_HOUR:
		memset((void*)(&Hours), 0, sizeof(DWORD));   
		memcpy((void*)(&Hours), Buffer, sizeof(DWORD));   
		break;
	case ROCKEY4_TIMER_BY_DAY:
		memset((void*)(&Days), 0, sizeof(DWORD));   
		memcpy((void*)(&Days), Buffer, sizeof(DWORD));   
		break;
	default:
	case ROCKEY4_TIMER_BY_DATE:
		memset((void*)(&time), 0, sizeof(SYSTEMTIME));   
		memcpy((void*)(&time), Buffer, sizeof(SYSTEMTIME));   		
		break;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND::AdjustTimerEx(int ID, SYSTEMTIME &time, WORD HID)//同步計時器
{
	WORD retcode=0;		
	BYTE Buffer[ROCKEY4_BUFFER_SIZE];	
	SetLParam();
	SetParam((WORD)ID);
	if ( SetHardIndex(HID) == false ) { return false; }
	memcpy(Buffer, &time, sizeof(SYSTEMTIME));
	retcode = ExecRockeyFnc(RY_ADJUST_TIMER_EX, Buffer);
	if ( CheckRockey4NDRetCode(retcode) == false )
	{	return false;	}		
	return true;
}
//-------------------------------------------------------------------------------------//
DWORD CRockey4ND::GetRockey4Ver() const
{
	return m_Rockey4Ver;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND::ReadRockey4Ver(WORD HID)//讀取版本
{
	WORD retcode=0;		
	BYTE Buffer[ROCKEY4_BUFFER_SIZE];	
	SetLParam();
	SetParam();
	if ( SetHardIndex(HID) == false ) { return false; }
	retcode = ExecRockeyFnc(RY_VERSION, Buffer);
	if ( CheckRockey4NDRetCode(retcode) == false )
	{	return false;	}
	m_Rockey4Ver = m_LParam[1];
	return true;
}
//-------------------------------------------------------------------------------------//