#include "stdafx.h"
#include "Rockey4ND_JET.h"
//-------------------------------------------------------------------------------------//
#pragma warning( disable : 4996 )//_CRT_SECURE_NO_WARNINGS
//-------------------------------------------------------------------------------------//
#ifdef ROCKEY4ND_USE
CRockey4ND_JET JET_Rockey4ND;
#endif//ROCKEY4ND_USE
//-------------------------------------------------------------------------------------//
#define BUILD_TIME_OFFSET                        0//創建日期的位置偏移值
#define EXPIRED_TIME_OFFSET                      4//逾時日期的位置偏移值
#define FUNCTION_ID_OFFSET                      20//功能編號的位置偏移值
//-------------------------------------------------------------------------------------//
#define CUSTOMER_ID_OFFSET                       0//客戶編號的位置偏移值
#define CHECK_TIME_OFFSET                        2//確認日期的位置偏移值
//-------------------------------------------------------------------------------------//
#define PUBLIC_MEMORY_ADDRESS_SIZE              14//公開區記憶體尺寸
#define PUBLIC_MEMORY_ADDRESS_START             52//公開區寫入起點
//-------------------------------------------------------------------------------------//
#define PRIVATE_MEMORY_ADDRESS_SIZE             46//私有區記憶體尺寸
#define PRIVATE_MEMORY_ADDRESS_START           528//私有區寫入起點
//-------------------------------------------------------------------------------------//
#define ROCKEY4_APP_ON                         ROCKEY4_BIT_01
#define ROCKEY4_TIMER_ON                       ROCKEY4_BIT_02
#define ROCKEY4_COUNT_ON                       ROCKEY4_BIT_03
//-------------------------------------------------------------------------------------//
#define CMP_TIME_PRECISION_YEAR                  1//時間比較精度-年
#define CMP_TIME_PRECISION_MONTH                 2//時間比較精度-月
#define CMP_TIME_PRECISION_DAY                   3//時間比較精度-日
#define CMP_TIME_PRECISION_HOUR                  4//時間比較精度-時
#define CMP_TIME_PRECISION_MINUTE                5//時間比較精度-分
#define CMP_TIME_PRECISION_SECOND                7//時間比較精度-秒
#define CMP_TIME_PRECISION_MIL_SEC               8//時間比較精度-毫秒
//-------------------------------------------------------------------------------------//
CRockey4ND_JET::CRockey4ND_JET()
{
	PreInitRockey4ND_JET();
	InitialRockey4ND_JET();	
}
//-------------------------------------------------------------------------------------//
CRockey4ND_JET::~CRockey4ND_JET()
{
}
//-------------------------------------------------------------------------------------//
void CRockey4ND_JET::PreInitRockey4ND_JET()
{
	m_DongleVer = 0;
	m_AppID = JET_APP_NONE;
	::memset(m_AppFolder, 0x00, sizeof(m_AppFolder));
	const size_t Len = sizeof(SYSTEMTIME);	
	::memset(m_RemainingCount, 0x00, sizeof(m_RemainingCount));
	::memset(m_ElapsedTime_Build, 0x00, sizeof(m_ElapsedTime_Build));
	::memset(m_ElapsedTime_Check, 0x00, sizeof(m_ElapsedTime_Check));
	::memset(m_ElapsedTime_Expired, 0x00, sizeof(m_ElapsedTime_Expired));	
}
//-------------------------------------------------------------------------------------//
void CRockey4ND_JET::InitialRockey4ND_JET()
{
}
//-------------------------------------------------------------------------------------//
void CRockey4ND_JET::CloneRockey4ND_JET(const CRockey4ND_JET &Rockey4)
{
}
//-------------------------------------------------------------------------------------//
void CRockey4ND_JET::SetJETAppID(JET_APP_ID AppID)
{
	m_AppID = AppID;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::GetAppIDText(JET_APP_ID AppID, char* Text)
{
	bool IsOK = true;
	switch ( AppID )
	{
	case JET_APP_NONE: strcpy(Text, "Test"); break;

	case JET_APP_JET6500: strcpy(Text, "JET6500"); break;
	case JET_APP_JET7000: strcpy(Text, "JET7000"); break;
	case JET_APP_JET8000: strcpy(Text, "JET8000"); break;

	case JET_APP_JET_ARS: strcpy(Text, "JetARS"); break;
	case JET_APP_JET_MIC: strcpy(Text, "JetMIC"); break;
	case JET_APP_JET_SFC: strcpy(Text, "JetSFC"); break;

	case JET_APP_JET_VRS: strcpy(Text, "JetVRS"); break;
	case JET_APP_JET_MSC: strcpy(Text, "JetMSC"); break;

	default:
	case JET_APP_END:
		IsOK = false;		
		::sprintf(Text, "AppID Exception[%d]", AppID);
		::sprintf(m_ErrorString, "Error, %s", Text);
		break;
	}
	return IsOK;	    
}
//-------------------------------------------------------------------------------------//
const char*  CRockey4ND_JET::GetAppFolder() const
{
	return m_AppFolder;
}
//-------------------------------------------------------------------------------------//
void CRockey4ND_JET::SetAppFolder(const char* Folder)
{
	if ( NULL == Folder ) { return; }
	::strcpy(m_AppFolder, Folder);	
}
//-------------------------------------------------------------------------------------//
int CRockey4ND_JET::GetDongleVer() const
{
	return m_DongleVer;
}
//-------------------------------------------------------------------------------------//
JET_APP_ID CRockey4ND_JET::GetJETAppID() const
{
	return m_AppID;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::LoadJETAppINI(int AppID)
{
	int     i=0;
	CString Section;
	CString KeyName;
	CString Default;
	CString Folder;	
	CString Filename;
	const DWORD szString = 128;
	char    AppText[szString]="";
	TCHAR   String[szString]=_T("");

	Folder = GetAppFolder();
	if ( Folder.GetLength() == 0 ) 
	{
		::sprintf(m_ErrorString, "Error, Not Set App Folder");
		return false; 
	}
	if ( GetAppIDText((JET_APP_ID)AppID, AppText) == false )
	{	return false;	}

	ClearFuncList();
	Section = AppText;
	Filename.Format(_T("%s\\%s.INI"), Folder, _T("Rockey4ND_JET"));
	KeyName = _T("Function Count");
	Default = _T("0");
	::GetPrivateProfileString(Section, KeyName, Default, String, szString, Filename);
	const int FuncCount = (int)(::_ttoi(String));
	for ( i=0; i<FuncCount; i++ )
	{
		KeyName.Format(_T("Func_%04d"), i);
		Default = _T("NONE");
		::GetPrivateProfileString(Section, KeyName, Default, String, szString, Filename);
		if ( Default.CompareNoCase(String) == 0 ) 
		{	AddFuncText(_T(""));	}
		else
		{	AddFuncText(String); }
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::SaveJETAppINI(int AppID)
{
	int     i=0;
	CString Section;
	CString KeyName;
	CString String;
	CString Folder;	
	CString Filename;
	const DWORD szString = 128;
	char    AppText[szString]="";	

	Folder = GetAppFolder();
	if ( Folder.GetLength() == 0 ) 
	{
		::sprintf(m_ErrorString, "Error, Not Set App Folder");
		return false; 
	}
	if ( GetAppIDText((JET_APP_ID)AppID, AppText) == false )
	{	return false;	}

	const int FuncCount=(int)(GetFuncCount());
	Section = AppText;
	Filename.Format(_T("%s\\%s.INI"), Folder, _T("Rockey4ND_JET"));
	KeyName = _T("Function Count");
	String.Format(_T("%d"), FuncCount);
	::WritePrivateProfileString(Section, KeyName, String, Filename);	
	if ( 0 == FuncCount )
	{
		KeyName.Format(_T("Func_%04d"), 0);
		String = _T("NONE");
		::WritePrivateProfileString(Section, KeyName, String, Filename);
	}
	else
	{
		for ( i=0; i<FuncCount; i++ )
		{
			KeyName.Format(_T("Func_%04d"), i);
			String = GetFuncText(i, false);
			::WritePrivateProfileString(Section, KeyName, String, Filename);
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CRockey4ND_JET::ClearFuncList()
{
	m_FuncItemList.clear();
}
//-------------------------------------------------------------------------------------//
size_t CRockey4ND_JET::GetFuncCount() const
{
	return m_FuncItemList.size();
}
//-------------------------------------------------------------------------------------//
void CRockey4ND_JET::AddFuncText(LPCTSTR FuncText)
{
	TFuncItem FuncItem;	
	FuncItem.bOnOff = false;
	FuncItem.sText = FuncText;
	FuncItem.FuncID = (int)(m_FuncItemList.size());
	m_FuncItemList.push_back(FuncItem);
}
//-------------------------------------------------------------------------------------//
LPCTSTR CRockey4ND_JET::GetFuncText(size_t idx, bool bChk)
{
	if ( bChk )
	{
		const size_t Count = m_FuncItemList.size();
		if ( idx >= Count ) 
		{	return NULL; }
	}
	return m_FuncItemList[idx].sText;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::GetFuncItem(size_t idx, TFuncItem &Item)
{
	const size_t Count = m_FuncItemList.size();
	if ( idx >= Count ) 
	{	return false; }
	Item = m_FuncItemList[idx];
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::SetFuncItem(size_t idx, const TFuncItem &Item)
{
	const size_t Count = m_FuncItemList.size();
	if ( idx >= Count ) 
	{	return false; }
	m_FuncItemList[idx]=Item;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::EnableFunctItemList(bool bEnable)
{
	int i=0;
	const size_t Count = m_FuncItemList.size();
	for ( i=0; i<Count; i++ )
	{	m_FuncItemList[i].bOnOff = bEnable;	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::CheckUserID_JET()
{
	int    i=0;
	WORD   HID=0;
	DWORD  UserID=0;
	int    Version=0;
	char   Buffer[32]="";
	const size_t Len=sizeof(DWORD);
	for ( i=0; i<ROCKEY4_MAX_COUNT; i++ )
	{
		HID = (WORD)(i);
		if ( CRockey4ND::CheckHIDValid(HID) == false ) { continue; }
		if ( CRockey4ND::ReadUserID(UserID, HID) == false )
		{
			CRockey4ND::CloseHID(HID);
			return false; 
		}
	
		::memcpy(Buffer, &UserID, Len);
		Buffer[Len] = '\0';
		if ( Buffer[0]!='J' || Buffer[1]!='E' || Buffer[2]!='T' ) 
		{
			CRockey4ND::CloseHID(HID);
			::sprintf(m_ErrorString, "Error, Read JET Dongle Fault[%s]", Buffer);
			return false; 
		}
		Version=Buffer[3]-'0';
		m_DongleVer = Version;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::OpenRockey4()
{
	if ( CRockey4ND::OpenRockey4() == false )
	{	return false; }
#ifndef ROCKEY4_DEVELOPER
	//Check UerID	
	if ( CheckUserID_JET() == false )
	{	return false; }
#endif//ROCKEY4_DEVELOPER

	WORD retcode=0;
	int AppID = GetJETAppID();	
	bool bValidApp=true;
	bool bValidTimer=true;
	bool bValidCount=true;
	bool bEnabledTimer=true;
	bool bEnabledCount=true;
	bool bEnabledTimerOne=false;
	bool bEnabledCountOne=false;
	DWORD ElapsedTimeBuild=0;
	DWORD ElapsedTimeCheck=0;
	DWORD ElapsedTimeExpired=0;
	SYSTEMTIME time;	
	GetCurrentDateTime(time);	

	int  i=0;
	WORD hid=0;
	bool bTempApp=false;
	bool bTempTimer=false;
	bool bTempCount=false;
	bool bTempTimerLoc=false;
	bool bTempCountLoc=false;
	m_ElapsedTime_Build[AppID] = 0xFFFFFFFF;
	m_ElapsedTime_Check[AppID] = 0;
	m_ElapsedTime_Expired[AppID] = 0;
	for ( i=0; i<ROCKEY4_MAX_COUNT; i++ )
	{
		hid = (WORD)(i);		
		if ( CheckHIDValid(hid) == false ) { continue; }

		if ( ReadAppElapsedTimeBuild(AppID, ElapsedTimeBuild, hid) == false )
		{	return false; }
		if ( ReadAppElapsedTimeCheck(AppID, ElapsedTimeCheck, hid) == false )
		{	return false; }
		if ( ReadAppElapsedTimeExpired(AppID, ElapsedTimeExpired, hid) == false )
		{	return false; }
		if ( m_ElapsedTime_Build[AppID] > ElapsedTimeBuild ) 
		{	m_ElapsedTime_Build[AppID] = ElapsedTimeBuild; }
		if ( m_ElapsedTime_Check[AppID] < ElapsedTimeCheck ) 
		{	m_ElapsedTime_Check[AppID] = ElapsedTimeCheck; }
		if ( m_ElapsedTime_Expired[AppID] < ElapsedTimeExpired ) 
		{	m_ElapsedTime_Expired[AppID] = ElapsedTimeExpired; }

		if ( false == bTempApp )
		{
			if ( CheckJETAppValid(AppID, bTempApp, hid) == false )
			{	return false;	}
			if ( true == bTempApp )
			{	bTempApp = true; }
		}

		bTempTimerLoc=bTempTimer;
		bTempCountLoc=bTempCount;
		if ( false == bTempTimer )
		{
			bEnabledTimerOne=true;
			if ( ReadJETTimerEnabled(AppID, bEnabledTimer, hid) == false ) 
			{	return false;	}
			if ( true == bEnabledTimer )
			{
				if ( ReadJETTimerValid(AppID, bValidTimer, hid) == false )
				{	return false;	}
				if ( true == bValidTimer )
				{	
					bTempTimer = true; 
					bTempTimerLoc = true;
				}
			}
			else
			{	bTempTimerLoc = true; }
		}
		
		if ( false == bTempCount )
		{
			bEnabledCountOne=true;
			if ( ReadJETCountEnabled(AppID, bEnabledCount, hid) == false ) 
			{	return false; }
			if ( true == bEnabledCount )
			{
				if ( ReadJETCountValid(AppID, bValidCount, hid) == false )
				{	return false;	}
				if ( true == bValidCount )
				{	
					bTempCount = true; 
					bTempCountLoc = true;
				}
			}			
			else
			{	bTempCountLoc = true; }
		}
		if ( CheckAppValid(bTempApp) == true )
		{
			if ( CheckAppTimerValid(bTempTimerLoc) == true )
			{
				if ( CheckAppCountValid(bTempCountLoc) == true )
				{	return true; }
			}
		}
		
	}
	/*
	if ( false==bEnabledTimerOne )
	{	bTempTimer = true; }
	if ( false==bEnabledCountOne )
	{	bTempCount = true; }
	*/
	bValidApp = bTempApp;
	bValidTimer = bTempTimer;
	bValidCount = bTempCount;	
	if ( CheckAppValid(bValidApp) == false )
	{	return false; }
	if ( CheckAppTimerValid(bValidTimer) == false )
	{	return false; }	
	if ( CheckAppCountValid(bValidCount) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
unsigned char CRockey4ND_JET::GetBit(WORD val)//取得位元數
{
	unsigned char Bit=0;
	switch ( val )
	{
	case 0:	Bit = ROCKEY4_BIT_01;	break;
	case 1:	Bit = ROCKEY4_BIT_02;	break;
	case 2:	Bit = ROCKEY4_BIT_03;	break;
	case 3:	Bit = ROCKEY4_BIT_04;	break;
	case 4:	Bit = ROCKEY4_BIT_05;	break;
	case 5:	Bit = ROCKEY4_BIT_06;	break;
	case 6:	Bit = ROCKEY4_BIT_07;	break;
	case 7:	Bit = ROCKEY4_BIT_08;	break;
	}
	return Bit;
	
}
//-------------------------------------------------------------------------------------//
WORD CRockey4ND_JET::GetAppMemorySize(bool bPublic)//取得軟體記憶體尺寸
{	
	if ( true == bPublic )
	{	return PUBLIC_MEMORY_ADDRESS_SIZE; }
	return PRIVATE_MEMORY_ADDRESS_SIZE;
}
//-------------------------------------------------------------------------------------//
WORD CRockey4ND_JET::GetAppBuildTimePos(int AppID)//取得軟體的創建日期時間
{
	WORD Offset= (WORD)(BUILD_TIME_OFFSET);
	WORD Start = GetAppMemoryStartPos(AppID, false);		
	return Start+Offset;
}
//-------------------------------------------------------------------------------------//
WORD CRockey4ND_JET::GetAppCheckTimePos(int AppID)//取得軟體的確認日期時間
{
	WORD Offset= (WORD)(CHECK_TIME_OFFSET);
	WORD Start = GetAppMemoryStartPos(AppID, true);		
	return Start+Offset;
}
//-------------------------------------------------------------------------------------//
WORD CRockey4ND_JET::GetAppExpiredTimePos(int AppID)//取得軟體的逾時日期時間
{
	WORD Offset= (WORD)(EXPIRED_TIME_OFFSET);
	WORD Start = GetAppMemoryStartPos(AppID, false);		
	return Start+Offset;
}
//-------------------------------------------------------------------------------------//
WORD CRockey4ND_JET::GetAppFuncIDPos(int AppID, int FuncID)//取得軟體的功能編號位置
{
	WORD Offset= (WORD)(FuncID/8);
	WORD Start = GetAppMemoryStartPos(AppID, false)+FUNCTION_ID_OFFSET;		
	return Start+Offset;
}
//-------------------------------------------------------------------------------------//
WORD CRockey4ND_JET::GetAppCustomerIDPos(int AppID)//取得軟體的客戶編號位置
{
	WORD Start = GetAppMemoryStartPos(AppID, true);
	return Start+CUSTOMER_ID_OFFSET;
}
//-------------------------------------------------------------------------------------//
WORD CRockey4ND_JET::GetAppMemoryStartPos(bool bPublic)//取得軟體記憶體起始點
{
	return GetAppMemoryStartPos(m_AppID, bPublic);
}
//-------------------------------------------------------------------------------------//
WORD CRockey4ND_JET::GetAppMemoryStartPos(int AppID, bool bPublic)//取得軟體記憶體起始點
{	
	if ( true == bPublic )
	{	return PUBLIC_MEMORY_ADDRESS_START+(PUBLIC_MEMORY_ADDRESS_SIZE*AppID); }
	return PRIVATE_MEMORY_ADDRESS_START+(PRIVATE_MEMORY_ADDRESS_SIZE*AppID);
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::CheckByte(unsigned char Byte, bool Bit[])//確認位元組
{
	Bit[0] = (bool)(Byte&ROCKEY4_BIT_01);
	Bit[1] = (bool)(Byte&ROCKEY4_BIT_02);
	Bit[2] = (bool)(Byte&ROCKEY4_BIT_03);
	Bit[3] = (bool)(Byte&ROCKEY4_BIT_04);
	Bit[4] = (bool)(Byte&ROCKEY4_BIT_05);
	Bit[5] = (bool)(Byte&ROCKEY4_BIT_06);
	Bit[6] = (bool)(Byte&ROCKEY4_BIT_07);
	Bit[7] = (bool)(Byte&ROCKEY4_BIT_08);
	return true;
}
//-------------------------------------------------------------------------------------//
DWORD CRockey4ND_JET::GetRemainingCount(int AppID)//取得剩餘次數
{
	if ( CheckAppID(AppID) == false ) { return 0; }

	return m_RemainingCount[AppID];
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::CheckAppRemainingCount(int AppID, DWORD Count)//確認剩餘次數是否大於...
{
	int RemainedCnt=GetRemainingCount(AppID);
	if ( Count > RemainedCnt )
	{
		::sprintf(m_ErrorString, "Error, Remaining Count[%u] less thean %u", RemainedCnt, Count);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::GetAppBuildTime(int AppID, SYSTEMTIME &time)//創建時間
{
	if ( CheckAppID(AppID) == false ) { return false; }

	SYSTEMTIME BaseTime;
	DWORD ElapsedTime = m_ElapsedTime_Build[AppID];
	GetBaseDateTime(BaseTime);
	if ( CalcNextDateByMinute(BaseTime, ElapsedTime, time) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::GetAppCheckTime(int AppID, SYSTEMTIME &time)//確認時間
{
	if ( CheckAppID(AppID) == false ) { return false; }

	SYSTEMTIME BaseTime;
	DWORD ElapsedTime = m_ElapsedTime_Check[AppID];
	GetBaseDateTime(BaseTime);
	if ( CalcNextDateByMinute(BaseTime, ElapsedTime, time) == false )
	{	return false; }
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::GetAppExpiredTime(int AppID, SYSTEMTIME &time)//逾期時間
{
	if ( CheckAppID(AppID) == false ) { return false; }

	SYSTEMTIME BaseTime;
	DWORD ElapsedTime = m_ElapsedTime_Expired[AppID];
	GetBaseDateTime(BaseTime);
	if ( CalcNextDateByMinute(BaseTime, ElapsedTime, time) == false )
	{	return false; }
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::CheckAppRemainingDay(int AppID, DWORD Days)//確認剩餘天數是否大於
{
	SYSTEMTIME NowTime;
	SYSTEMTIME ExpTime;
	const int CmpTimePrec=CMP_TIME_PRECISION_MIL_SEC;
	if ( GetCurrentDateTime(NowTime) == false )
	{	return false; }
	if ( GetAppExpiredTime(AppID, ExpTime) == false )
	{	return false; }
	if ( CompareTimeValid(NowTime, ExpTime, CmpTimePrec) == false )
	{	return CheckAppTimerValid(false);	}

	DWORD etDays=0;//目前到逾期的天數
	if ( CalcElapsedTime_Day(NowTime, ExpTime, etDays) == false )
	{	return false; }
	if ( etDays < Days )
	{
		::sprintf(m_ErrorString, "Error, Remaining Days[%u] less than %u", etDays, Days);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::EnableJETDongle_All()//啟用JET硬體鎖 	
{
#ifdef ROCKEY4_DEVELOPER		
	int  i=0;
	WORD hid=0;
	bool IsOK=true;
	for ( i=0; i<ROCKEY4_MAX_COUNT; i++ )
	{
		hid = (WORD)(i);
		if ( CheckHIDValid(hid) == false ) { continue; }
		if ( EnableJETDongle(hid) == false )
		{	IsOK = false; }
	}
	return IsOK;	
#else
	return SupportDevelopVersion();
#endif//ROCKEY4_DEVELOPER
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::EnableJETDongle(WORD HID)//啟用JET硬體鎖 
{
#ifdef ROCKEY4_DEVELOPER
	if ( HID_ALL == HID )
	{	return EnableJETDongle_All();	}	

	DWORD val=0;
	BYTE Buffer[32]="JET1";	
	const size_t Len = sizeof(DWORD);
	::memcpy(&val, Buffer, Len);
	if ( CRockey4ND::WriteUserID(val, HID) == false )
	{	return false; }
#else
	return SupportDevelopVersion();
#endif//ROCKEY4_DEVELOPER
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::CheckAppID(int AppID)
{
	if ( AppID<0 || AppID>JET_APP_COUNT )
	{
		::sprintf(m_ErrorString, "Error, the App ID Exception[%d]", AppID);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::CheckAppValid(bool bValid)
{
	if ( true == bValid ) { return true; }
	::sprintf(m_ErrorString, "Error, the App is not valided");
	return false;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::CheckAppTimerValid(bool bValid)
{
	if ( true == bValid ) { return true; }
	::sprintf(m_ErrorString, "Error, the Time is expired");
	return false;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::CheckAppCountValid(bool bValid)
{
	if ( true == bValid ) { return true; }
	::sprintf(m_ErrorString, "Error, the Remaining count is zero");
	return false;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::CheckJETAppValid_All(int AppID, bool &bValid)//確認JET有效用
{
	int  i=0;
	WORD hid=0;
	bool bTemp=true;
	bValid = false;
	for ( i=0; i<ROCKEY4_MAX_COUNT; i++ )
	{
		hid = (WORD)(i);
		if ( CheckHIDValid(hid) == false ) { continue; }
		if ( CheckJETAppValid(AppID, bTemp, hid) == false ) { return false; }		
		if ( true == bTemp ) 
		{	
			bValid = true;
			return true; 
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::CheckJETAppValid(int AppID, bool &bValid, WORD HID)//確認該模組有效用
{
	if ( HID_ALL == HID )
	{	return CheckJETAppValid_All(AppID, bValid); }

	int  Content = 0;	
	if ( CheckAppID(AppID) == false )
	{	return false; }
	if ( CRockey4ND::ReadModuleContent(AppID, Content, HID) == false )
	{	return false; }
	DWORD Res = Content&ROCKEY4_APP_ON;
	if ( 0 == Res ) { bValid = false; }
	else { bValid = true; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::ReadAppElapsedTimeBuild(int AppID, DWORD &ElapsedTime, WORD HID)
{
	DWORD val=0;
	int Pos = GetAppBuildTimePos(AppID);	
	if ( CRockey4ND::ReadDataMemory(Pos, val, HID) == false )
	{	return false; }
	ElapsedTime = val;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::ReadAppElapsedTimeCheck(int AppID, DWORD &ElapsedTime, WORD HID)
{
	DWORD val=0;
	int Pos = GetAppCheckTimePos(AppID);	
	if ( CRockey4ND::ReadDataMemory(Pos, val, HID) == false )
	{	return false; }
	ElapsedTime = val;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::ReadAppElapsedTimeExpired(int AppID, DWORD &ElapsedTime, WORD HID)
{
	DWORD val=0;
	int Pos = GetAppExpiredTimePos(AppID);	
	if ( CRockey4ND::ReadDataMemory(Pos, val, HID) == false )
	{	return false; }
	ElapsedTime = val;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::CompareTimeValid(const SYSTEMTIME &CurTime, const SYSTEMTIME &ExpTime, int Precision)//比較時間
{	
	if ( CurTime.wYear < ExpTime.wYear ) { return true; }
	if ( CurTime.wYear > ExpTime.wYear ) { return false; }	
	if ( Precision < CMP_TIME_PRECISION_YEAR ) { return true; }
	if ( CurTime.wMonth < ExpTime.wMonth ) { return true; }
	if ( CurTime.wMonth > ExpTime.wMonth ) { return false; }
	if ( Precision < CMP_TIME_PRECISION_MONTH ) { return true; }
	if ( CurTime.wDay < ExpTime.wDay ) { return true; }
	if ( CurTime.wDay > ExpTime.wDay ) { return false; }
	if ( Precision < CMP_TIME_PRECISION_DAY ) { return true; }
	if ( CurTime.wHour < ExpTime.wHour ) { return true; }
	if ( CurTime.wHour > ExpTime.wHour ) { return false; }
	if ( Precision < CMP_TIME_PRECISION_HOUR ) { return true; }
	if ( CurTime.wMinute < ExpTime.wMinute ) { return true; }
	if ( CurTime.wMinute > ExpTime.wMinute ) { return false; }
	if ( Precision < CMP_TIME_PRECISION_MINUTE ) { return true; }
	if ( CurTime.wSecond < ExpTime.wSecond ) { return true; }
	if ( CurTime.wSecond > ExpTime.wSecond ) { return false; }
	if ( Precision < CMP_TIME_PRECISION_SECOND ) { return true; }
	if ( CurTime.wMilliseconds < ExpTime.wMilliseconds ) { return true; }
	if ( CurTime.wMilliseconds > ExpTime.wMilliseconds ) { return false; }
	if ( Precision < CMP_TIME_PRECISION_MIL_SEC ) { return true; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::ListJETAppValid(std::vector<TAppItem> &AppList, WORD HID)
{
	int i=0;
	int AppID=0;
	bool  bValid=true;
	TAppItem  AppItem;
	const int AppCount=JET_APP_COUNT;
	AppList.clear();	
	for ( i=0; i<AppCount; i++ )
	{
		switch ( i )
		{
		case JET_APP_NONE:
		case JET_APP_JET6500:
		case JET_APP_JET7000:
		case JET_APP_JET8000:
		case JET_APP_JET_ARS:
		case JET_APP_JET_MIC:
		case JET_APP_JET_SFC:
		case JET_APP_JET_VRS:
		case JET_APP_JET_MSC:
			AppID = i;
			break;
		default:
		case JET_APP_END:
			AppID = JET_APP_END;
			break;
		}
		if ( JET_APP_END == AppID ) { continue; }
		AppItem.nAppID = AppID;		
		if ( CheckJETAppValid(AppID, bValid, HID) == false )
		{	return false; }
		AppItem.bEnabled = bValid;
		AppList.push_back(AppItem);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::ReadAppDateTime_All(int AppID, SYSTEMTIME &tBuild, SYSTEMTIME &tCheck, SYSTEMTIME &tExp)
{
	int  i=0;
	WORD hid=0;
	DWORD etBuild=0, etCheck=0, etExp=0;
	DWORD etBuildApp=0, etCheckApp=0, etExpApp=0;
	etBuildApp = 0xFFFFFFFF;
	for ( i=0; i<ROCKEY4_MAX_COUNT; i++ )
	{
		hid = (WORD)(i);
		if ( CheckHIDValid(hid) == false ) { continue; }
		if ( ReadAppElapsedTimeBuild(AppID, etBuild, hid) == false ) { return false; }
		if ( ReadAppElapsedTimeCheck(AppID, etCheck, hid) == false ) { return false; }
		if ( ReadAppElapsedTimeExpired(AppID, etExp, hid) == false ) { return false; }
		if ( etBuildApp > etBuild ) { etBuildApp = etBuild; }
		if ( etCheckApp < etCheck ) { etCheckApp = etCheck; }
		if ( etExpApp < etExp ) { etExpApp = etExp; }
	}
	if ( 0xFFFFFFFF == etBuildApp ) 
	{
		::sprintf(m_ErrorString, "Error, Read Build Time Fault");
		return false;
	}
	SYSTEMTIME BaseTime;
	GetBaseDateTime(BaseTime);
	CalcNextDateByMinute(BaseTime, etBuildApp, tBuild);
	CalcNextDateByMinute(BaseTime, etCheckApp, tCheck);
	CalcNextDateByMinute(BaseTime, etExpApp, tExp);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::ReadAppDateTime(int AppID, SYSTEMTIME &tBuild, SYSTEMTIME &tCheck, SYSTEMTIME &tExp, WORD HID)
{	
	if ( HID_ALL == HID )
	{	return ReadAppDateTime_All(AppID, tBuild, tCheck, tExp); }
	
	DWORD etBuild=0, etCheck=0, etExp=0;	
	if ( CRockey4ND_JET::ReadAppElapsedTimeBuild(AppID, etBuild, HID) == false )
	{	return false;	}
	if ( CRockey4ND_JET::ReadAppElapsedTimeCheck(AppID, etCheck, HID) == false )
	{	return false;	}
	if ( CRockey4ND_JET::ReadAppElapsedTimeExpired(AppID, etExp, HID) == false )
	{	return false;	}

	SYSTEMTIME BaseTime;
	GetBaseDateTime(BaseTime);
	CalcNextDateByMinute(BaseTime, etBuild, tBuild);
	CalcNextDateByMinute(BaseTime, etCheck, tCheck);
	CalcNextDateByMinute(BaseTime, etExp, tExp);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::ReadJETTimerValid_All(int AppID, bool &bValid)//確認JET計時器有效-無逾期
{
	int  i=0;
	WORD hid=0;
	bool bTemp=true;
	bValid = false;
	for ( i=0; i<ROCKEY4_MAX_COUNT; i++ )
	{
		hid = (WORD)(i);
		if ( CheckHIDValid(hid) == false ) { continue; }
		if ( ReadJETTimerValid(AppID, bTemp, hid) == false ) { return false; }		
		if ( true == bTemp ) 
		{	
			bValid = true;
			return true; 
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::ReadJETTimerValid(int AppID, bool &bValid, WORD HID)//讀取JET計時器有效-無逾期
{
	if ( HID_ALL == HID )
	{	return ReadJETTimerValid_All(AppID, bValid); }
	int Pos=0;
	int Mode=1;	
	WORD retcode=0;
	SYSTEMTIME time;
	SYSTEMTIME ExpTime;
	SYSTEMTIME BaseTime;
	DWORD Hours=0, Days=0;
	const int CmpTimePrec=CMP_TIME_PRECISION_HOUR;
	bValid = true;
	GetCurrentDateTime(time);
	GetBaseDateTime(BaseTime);
#ifdef ROECKEY4_TIMER_ENABLE
	if ( CRockey4ND::GetTimerEx(AppID, Mode, time, Hours, Days, HID) == false )
	{
		retcode = CRockey4ND::GetRetCode();
		if  ( ERR_AUTH_EXPIRED==retcode )
		{	bValid = false; }
		else if ( R4SERR_JUST_TIMER==retcode)			
		{	bValid = false; }
		else
		{	return false;  }
	}
	if ( true == bValid )
	{
		switch ( Mode )
		{	
		case ROCKEY4_TIMER_BY_HOUR:
			GetCurrentDateTime(time);
			CalcNextDateByHour(time, Hours, ExpTime);
			break;
		case ROCKEY4_TIMER_BY_DAY:
			ExpTime = time;
			break;
		default:
		case ROCKEY4_TIMER_BY_DATE:
			ExpTime = time;
			break;
		}	
		bValid = CompareTimeValid(time, ExpTime, CmpTimePrec);
}
#endif//ROECKEY4_TIMER_ENABLE

	DWORD etNow=0, etCheck=0, etExp=0;	
	if ( CRockey4ND::CalcElapsedTime_Minute(BaseTime, time, etNow) == false )
	{	return false;	}

	Pos = GetAppCheckTimePos(AppID);
	if ( CRockey4ND::ReadDataMemory(Pos, etCheck, HID) == false )
	{	return false;	}
	if ( etCheck > etNow )
	{
		bValid = false;
		::sprintf(m_ErrorString, "Error, Current Time is less than last Check Time");
		return false;
	}	
	if ( CRockey4ND::WriteDataMemory(Pos, etNow, HID) == false )
	{	return false; 	}

	Pos = GetAppExpiredTimePos(AppID);
	if ( CRockey4ND::ReadDataMemory(Pos, etExp, HID) == false )
	{	return false;	}
	if ( etExp < etNow )
	{	bValid = false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::CheckJETTimerValid_All(int AppID, bool &bValid)//確認JET計時器有效-無逾期
{
	int  i=0;
	WORD hid=0;
	bool bTemp=true;
	bool bEnable=true;
	bValid = false;
	for ( i=0; i<ROCKEY4_MAX_COUNT; i++ )
	{
		hid = (WORD)(i);
		if ( CheckHIDValid(hid) == false ) { continue; }
		if ( CheckJETTimerValid(AppID, bEnable, bTemp, hid) == false ) { return false; }		
		if ( true == bTemp ) 
		{	
			if ( true == bEnable )
			{
				bValid = true;
				return true; 
			}
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::CheckJETTimerValid(int AppID, bool &bEnable, bool &bValid, WORD HID)//確認JET計時器有效-無逾期	
{
	if ( HID_ALL == HID ) 
	{	return CheckJETTimerValid_All(AppID, bValid); }

	bEnable=true;
	if ( ReadJETTimerEnabled(AppID, bEnable, HID) == false )
	{	return false;	}

	if ( false == bEnable ) 
	{	
		bValid = true;
		return true;
	}	
	if ( ReadJETTimerValid(AppID, bValid, HID) == false )
	{	return false;	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::ReadJETCountValid_All(int AppID, bool &bValid)//讀取JET計次器有效-次數符合
{
	int  i=0;
	WORD hid=0;
	bool bTemp=true;
	bValid = false;
	for ( i=0; i<ROCKEY4_MAX_COUNT; i++ )
	{
		hid = (WORD)(i);
		if ( CheckHIDValid(hid) == false ) { continue; }
		if ( ReadJETCountValid(AppID, bTemp, hid) == false ) { return false; }		
		if ( true == bTemp ) 
		{	
			bValid = true;
			return true; 
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::ReadJETCountValid(int AppID, bool &bValid, WORD HID)//讀取JET計次器有效-次數符合
{
	if ( HID_ALL == HID ) 
	{	return ReadJETCountValid_All(AppID, bValid); }
	WORD   retcode=0;
	DWORD  Count = 0;
	int    Decrease = 0;
	if ( CRockey4ND::GetCountEx(AppID, Count, Decrease, HID) == false )
	{
		retcode = CRockey4ND::GetRetCode();
		if  ( ERROR_COUNT_MODULE_ISZERO == retcode )
		{	Count = 0;  }
		else
		{	return false; }
	}	
	if ( 0 == Count ) 
	{	bValid = false; }
	else
	{	bValid = true; }
	m_RemainingCount[AppID] = Count;
	return true;
}
//-------------------------------------------------------------------------------------//
bool  CRockey4ND_JET::CheckJETCountValid_All(int AppID, bool &bValid)//確認JET計次器有效-次數符合
{
	int  i=0;
	WORD hid=0;
	bool bTemp=true;
	bool bEnable=true;
	bValid = false;
	for ( i=0; i<ROCKEY4_MAX_COUNT; i++ )
	{
		hid = (WORD)(i);
		if ( CheckHIDValid(hid) == false ) { continue; }
		if ( CheckJETCountValid(AppID, bEnable, bTemp, hid) == false ) { return false; }		
		if ( true == bTemp ) 
		{	
			if ( true == bEnable )
			{
				bValid = true;
				return true; 
			}
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool  CRockey4ND_JET::CheckJETCountValid(int AppID, bool &bEnable, bool &bValid, WORD HID)//確認JET計次器有效-次數符合
{
	if ( HID_ALL == HID ) 
	{	return CheckJETCountValid_All(AppID, bValid); }

	bEnable = true;
	if ( ReadJETCountEnabled(AppID, bEnable, HID) == false )
	{	return false;	}
	if ( false == bEnable )
	{	
		bValid = true;	
		return true;
	}	
	if ( ReadJETCountValid(AppID, bValid, HID) == false )
	{	return false;	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::ReadJETTimerEnabled_All(int AppID, bool &bEnabled)//讀取JET計時器使用 
{
	int  i=0;
	WORD hid=0;
	bool bTemp=true;
	bEnabled = true;
	for ( i=0; i<ROCKEY4_MAX_COUNT; i++ )
	{
		hid = (WORD)(i);
		if ( CheckHIDValid(hid) == false ) { continue; }
		if ( ReadJETTimerEnabled(AppID, bTemp, hid) == false ) { return false; }		
		if ( false == bTemp ) 
		{	
			bEnabled = false;
			return true; 
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::ReadJETTimerEnabled(int AppID, bool &bEnabled, WORD HID)//讀取JET計時器使用 
{
	if ( HID_ALL == HID )
	{	return ReadJETTimerEnabled_All(AppID, bEnabled); }

	int  Content = 0;	
	if ( CRockey4ND::ReadModuleContent(AppID, Content, HID) == false )
	{	return false; }
	DWORD Res = Content&ROCKEY4_TIMER_ON;
	if ( 0 == Res ) { bEnabled=false; }
	else {	bEnabled = true; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::ReadJETCountEnabled_All(int AppID, bool &bEnabled)//讀取JET計次器使用 
{
	int  i=0;
	WORD hid=0;
	bool bTemp=true;
	bEnabled = true;
	for ( i=0; i<ROCKEY4_MAX_COUNT; i++ )
	{
		hid = (WORD)(i);
		if ( CheckHIDValid(hid) == false ) { continue; }
		if ( ReadJETCountEnabled(AppID, bTemp, hid) == false ) { return false; }		
		if ( false == bTemp ) 
		{	
			bEnabled = false;
			return true; 
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::ReadJETCountEnabled(int AppID, bool &bEnabled, WORD HID)//讀取JET計次器使用
{
	if ( HID_ALL == HID )
	{	return ReadJETCountEnabled_All(AppID, bEnabled); }

	int  Content = 0;	
	if ( CRockey4ND::ReadModuleContent(AppID, Content, HID) == false )
	{	return false; }
	DWORD Res = Content&ROCKEY4_COUNT_ON;
	if ( 0 == Res ) { bEnabled=false; }
	else {	bEnabled = true; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::RemoveJETApp_All(int AppID)//移除JET軟體
{
#ifdef ROCKEY4_DEVELOPER
	int  i=0;
	WORD hid=0;
	bool bTemp=true;
	for ( i=0; i<ROCKEY4_MAX_COUNT; i++ )
	{
		hid = (WORD)(i);
		if ( CheckHIDValid(hid) == false ) { continue; }
		if ( RemoveJETApp(AppID, hid) == false ) { return false; }
	}
	return true;
#else
	return SupportDevelopVersion();
#endif//ROCKEY4_DEVELOPER
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::RemoveJETApp(int AppID, WORD HID)//移除JET軟體
{
#ifdef ROCKEY4_DEVELOPER
	if ( HID_ALL == HID )
	{	return RemoveJETApp_All(AppID); }

	//Reset Memory First
	if ( ResetJETAppMemory(AppID, HID) == false )
	{	return false; }

	//Reset Timer
	DWORD Days=0;
	DWORD Hours=0;
	SYSTEMTIME time;
	int Mode = ROCKEY4_TIMER_BY_DATE;
	GetCurrentDateTime(time);
	if ( CRockey4ND::SetTimerEx(AppID, Mode, time, Hours, Days, HID) == false )
	{	return false; }

	//Reset Counter
	DWORD Count = 0;
	int Decrease = 0;
	if ( CRockey4ND::SetCountEx(AppID, Count, Decrease, HID) == false )
	{	return false; }

	int  Content = 0;	
	int  AutoDecrease = 0;
	if ( CRockey4ND::SetModule(AppID, Content, AutoDecrease, HID) == false )
	{	return false; }	
#else
	return SupportDevelopVersion();
#endif//ROCKEY4_DEVELOPER
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::EnableJETApp_All(int AppID, DWORD Timer, DWORD Count)//啟用JET軟體
{
#ifdef ROCKEY4_DEVELOPER
	int  i=0;
	WORD hid=0;
	bool bTemp=true;
	for ( i=0; i<ROCKEY4_MAX_COUNT; i++ )
	{
		hid = (WORD)(i);
		if ( CheckHIDValid(hid) == false ) { continue; }
		if ( EnableJETApp(AppID, Timer, Count, hid) == false ) { return false; }
	}
	return true;
#else
	return SupportDevelopVersion();
#endif//ROCKEY4_DEVELOPER
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::EnableJETApp(int AppID, DWORD Timer, DWORD Count, WORD HID)//啟用JET軟體
{
#ifdef ROCKEY4_DEVELOPER
	if ( HID_ALL == HID )
	{	return EnableJETApp_All(AppID, Timer, Count); }

	SYSTEMTIME time;
	SYSTEMTIME Basetime;
	int  Pos = 0;
	int  Content = 0;	
	int  AutoDecrease = 0;	
	DWORD ElapsedTime=0;
	DWORD ElapsedTime2=0;
	Content = ROCKEY4_APP_ON;
	if ( Timer > 0 )
	{	Content |= ROCKEY4_TIMER_ON; }
	if ( Count > 0 )
	{	Content |= ROCKEY4_COUNT_ON; }
	if ( CRockey4ND::SetModule(AppID, Content, AutoDecrease, HID) == false )
	{	return false; }	
	
	GetCurrentDateTime(time);
	GetBaseDateTime(Basetime);
	if ( CRockey4ND::CalcElapsedTime_Minute(Basetime, time, ElapsedTime) == false )
	{	return false; }
	Pos = GetAppBuildTimePos(AppID);
	if ( CRockey4ND::WriteDataMemory(Pos, ElapsedTime, HID) == false )
	{	return false; }
	Pos = GetAppCheckTimePos(AppID);
	if ( CRockey4ND::WriteDataMemory(Pos, ElapsedTime, HID) == false )
	{	return false; }

	if ( Timer > 0 )
	{
		SYSTEMTIME time, time2;
		DWORD Hours=0, Days = 0;
		int TimerMode=ROCKEY4_TIMER_BY_DAY;//ROCKEY4_TIMER_BY_HOUR;//ROCKEY4_TIMER_BY_DAY
		
		::memset(&time2, 0x00,sizeof(time2));
		switch ( TimerMode )
		{
		case ROCKEY4_TIMER_BY_DAY:	
			Days = Timer; 
			ElapsedTime2 = Timer*24*60;
			break;
		case ROCKEY4_TIMER_BY_HOUR:				
			Hours = Timer; 
			ElapsedTime2 = Timer*60;
			break;
		default:
			GetCurrentDateTime(time);
			CalcNextDateByDay(time, Timer, time2);
			//CalcNextDateByHour(time, Timer, time2);
			ElapsedTime2 = Timer*60;
			break;
		}	
	#ifdef ROECKEY4_TIMER_ENABLE
		if ( CRockey4ND::SetTimerEx(AppID, TimerMode, time2, Hours, Days, HID) == false )
		{	return false;	}
	#endif//ROECKEY4_TIMER_ENABLE
	}
	else//100 years 
	{	ElapsedTime2 = 100*365*24*60;}
	Pos = GetAppExpiredTimePos(AppID);	
	ElapsedTime2 += ElapsedTime;
	if ( CRockey4ND::WriteDataMemory(Pos, ElapsedTime2, HID) == false )
	{	return false; }

	if ( Count > 0 )
	{		
		if ( CRockey4ND::SetCountEx(AppID, Count, true, HID) == false )
		{	return false;	}
	}
#else
	return SupportDevelopVersion();
#endif//ROCKEY4_DEVELOPER
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::ResetJETAppMemory_All(int AppID)//清除JET軟體記憶體區塊
{
#ifdef ROCKEY4_DEVELOPER
	int  i=0;
	WORD hid=0;
	bool bTemp=true;
	for ( i=0; i<ROCKEY4_MAX_COUNT; i++ )
	{
		hid = (WORD)(i);
		if ( CheckHIDValid(hid) == false ) { continue; }
		if ( ResetJETAppMemory(AppID, hid) == false ) { return false; }
	}
	return true;
#else
	return SupportDevelopVersion();
#endif//ROCKEY4_DEVELOPER	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::ResetJETAppMemory(int AppID, WORD HID)//清除JET軟體記憶體區塊
{
#ifdef ROCKEY4_DEVELOPER
	if ( HID_ALL == HID ) 
	{	ResetJETAppMemory_All(AppID); }

	int Pos = 0;	
	int Len = 0;
	bool bPublic;
	BYTE  Buffer[ROCKEY4_BUFFER_SIZE];

	//For Pulbic
	bPublic = true;
	Len = GetAppMemorySize(bPublic);
	Pos = GetAppMemoryStartPos(AppID, bPublic);		
	::memset(Buffer, 0x00, Len);	
	if ( CRockey4ND::WriteDataMemory(Pos, Len, Buffer, HID) == false )
	{	return false; }	

	//For Private
	bPublic = false;
	Len = GetAppMemorySize(bPublic);
	Pos = GetAppMemoryStartPos(AppID, bPublic);		
	::memset(Buffer, 0x00, Len);	
	if ( CRockey4ND::WriteDataMemory(Pos, Len, Buffer, HID) == false )
	{	return false; }	
#else
	return SupportDevelopVersion();
#endif//ROCKEY4_DEVELOPER
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::EnableJETAllFunc_All(int AppID)//啟用所有特殊功能
{
#ifdef ROCKEY4_DEVELOPER
	int  i=0;
	WORD hid=0;
	bool bTemp=true;
	for ( i=0; i<ROCKEY4_MAX_COUNT; i++ )
	{
		hid = (WORD)(i);
		if ( CheckHIDValid(hid) == false ) { continue; }
		if ( EnableJETAllFunc(AppID, hid) == false ) { return false; }
	}
	return true;
#else
	return SupportDevelopVersion();
#endif//ROCKEY4_DEVELOPER	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::EnableJETAllFunc(int AppID, WORD HID)//啟用所有特殊功能
{
#ifdef ROCKEY4_DEVELOPER
	if ( HID_ALL == HID )
	{	return EnableJETAllFunc_All(AppID); }

	bool bValid=true;
	BYTE Buffer[ROCKEY4_BUFFER_SIZE];	
	int Pos = GetAppFuncIDPos(AppID, 0);	
	int PosEnd = GetAppMemoryStartPos(AppID+1, false);
	int Len = PosEnd-Pos;
	::memset(Buffer, 0xFF, Len);
	if ( CheckJETAppValid(AppID, bValid, HID) == false )
	{	return false; }	
	if ( CheckAppValid(bValid) == false )
	{	return false; }
	if ( CRockey4ND::WriteDataMemory(Pos, Len, Buffer, HID) == false )
	{	return false; }	
#else
	return SupportDevelopVersion();
#endif//ROCKEY4_DEVELOPER
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::DisableJETAllFunc_All(int AppID)//關閉所有特殊功能
{
#ifdef ROCKEY4_DEVELOPER
	int  i=0;
	WORD hid=0;
	bool bTemp=true;
	for ( i=0; i<ROCKEY4_MAX_COUNT; i++ )
	{
		hid = (WORD)(i);
		if ( CheckHIDValid(hid) == false ) { continue; }
		if ( DisableJETAllFunc(AppID, hid) == false ) { return false; }
	}
	return true;
#else
	return SupportDevelopVersion();
#endif//ROCKEY4_DEVELOPER	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::DisableJETAllFunc(int AppID, WORD HID)//關閉所有特殊功能
{
#ifdef ROCKEY4_DEVELOPER
	if ( HID_ALL == HID ) 
	{	return DisableJETAllFunc_All(AppID); }

	bool bValid=true;
	BYTE Buffer[ROCKEY4_BUFFER_SIZE];	
	int Pos = GetAppFuncIDPos(AppID, 0);	
	int PosEnd = GetAppMemoryStartPos(AppID+1, false);
	int Len = PosEnd-Pos;
	::memset(Buffer, 0x00, Len);
	if ( CheckJETAppValid(AppID, bValid, HID) == false )
	{	return false; }	
	if ( CheckAppValid(bValid) == false )
	{	return false; }
	if ( CRockey4ND::WriteDataMemory(Pos, Len, Buffer, HID) == false )
	{	return false; }	
#else
	return SupportDevelopVersion();
#endif//ROCKEY4_DEVELOPER
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::ReadJETAllFunc_All(int AppID, int&Count, bool Bit[])//讀取所有特殊功能
{
	int  i=0, j=0;
	WORD hid=0;
	int  nTempCnt=0;
	bool bTempBit[ROCKEY4_BUFFER_SIZE];
	Count = 0;
	for ( i=0; i<ROCKEY4_MAX_COUNT; i++ )
	{
		hid = (WORD)(i);
		if ( CheckHIDValid(hid) == false ) { continue; }
		if ( ReadJETAllFunc(AppID, nTempCnt, bTempBit, hid) == false ) { return false; }
		for ( j=0; j<nTempCnt; j++ )
		{	Bit[j] |= bTempBit[j];	}
		if ( Count < nTempCnt ) { Count = nTempCnt; }
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool  CRockey4ND_JET::ReadJETAllFunc(int AppID, int&Count, bool Bit[], WORD HID)//讀取所有特殊功能
{
	if ( HID_ALL == HID )
	{	return ReadJETAllFunc_All(AppID, Count, Bit); }

	int  i=0;
	WORD Len = 0;
	BYTE Buffer[ROCKEY4_BUFFER_SIZE];	
	if ( ReadJETAllFuncBuffer(AppID, Len, Buffer, HID) == false )
	{	return false;	}
	Count = Len*8;
	for ( i=0; i<Len; i++ )
	{	CheckByte(Buffer[i], &(Bit[i*8]));	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::WriteJETAllFunc_All(int AppID, int Count, bool Bit[])//寫入所有特殊功能
{
#ifdef ROCKEY4_DEVELOPER
	int  i=0;
	int  Len=0;
	WORD hid=0;
	BYTE Buffer[ROCKEY4_BUFFER_SIZE];	
	if ( BitListToBuffer(Count, Bit, Len, Buffer) == false ) 
	{	return false; }
	for ( i=0; i<ROCKEY4_MAX_COUNT; i++ )
	{
		hid = (WORD)(i);
		if ( CheckHIDValid(hid) == false ) { continue; }
		if ( WriteJETAllFuncBuffer(AppID, Len, Buffer, hid) == false ) { return false; }
	}
	return true;
#else
	return SupportDevelopVersion();
#endif//ROCKEY4_DEVELOPER	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::WriteJETAllFunc(int AppID, int Count, bool Bit[], WORD HID)//寫入所有特殊功能
{
#ifdef ROCKEY4_DEVELOPER
	if ( HID_ALL == HID )
	{	return WriteJETAllFunc_All(AppID, Count, Bit); }

	int  Len=0;
	bool bValid=true;	
	BYTE Buffer[ROCKEY4_BUFFER_SIZE];	
	int Pos = GetAppFuncIDPos(AppID, 0);	
	int PosEnd = GetAppMemoryStartPos(AppID+1, false);		
	if ( CheckJETAppValid(AppID, bValid, HID) == false )
	{	return false; }	
	if ( CheckAppValid(bValid) == false )
	{	return false; }
	if ( BitListToBuffer(Count, Bit, Len, Buffer) == false ) 
	{	return false; }
	if ( CRockey4ND::WriteDataMemory(Pos, Len, Buffer, HID) == false )
	{	return false; }	
	return true;
#else
	return SupportDevelopVersion();
#endif//ROCKEY4_DEVELOPER	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::BitListToBuffer(int Count, const bool Bit[], int &Len, unsigned char Buffer[])	
{
	int   i=0, j=0;
	unsigned char Byte=0;	
	const int Mod=Count%8;
	if ( 0 == Mod )
	{
		for ( i=0; i<Count; i+=8 )
		{
			j = i;
			Byte = 0;			
			if ( Bit[j++] )
			{	Byte |= ROCKEY4_BIT_01; }
			if ( Bit[j++] )
			{	Byte |= ROCKEY4_BIT_02; }			
			if ( Bit[j++] )
			{	Byte |= ROCKEY4_BIT_03; }
			if ( Bit[j++] )
			{	Byte |= ROCKEY4_BIT_04; }
			if ( Bit[j++] )
			{	Byte |= ROCKEY4_BIT_05; }
			if ( Bit[j++] )
			{	Byte |= ROCKEY4_BIT_06; }
			if ( Bit[j++] )
			{	Byte |= ROCKEY4_BIT_07; }
			if ( Bit[j++] )
			{	Byte |= ROCKEY4_BIT_08; }
			Buffer[Len++] = Byte;	
		}
	}
	else
	{	
		for ( i=0; i<Count; i+=8 )
		{
			j = i;
			Byte = 0;
			if ( j>Count ) 
			{	break; }
			if ( Bit[j++] )
			{	Byte |= ROCKEY4_BIT_01; }
			if ( j>Count ) 
			{	break; }
			if ( Bit[j++] )
			{	Byte |= ROCKEY4_BIT_02; }
			if ( j>Count ) 
			{	break; }
			if ( Bit[j++] )
			{	Byte |= ROCKEY4_BIT_03; }
			if ( j>Count ) 
			{	break; }
			if ( Bit[j++] )
			{	Byte |= ROCKEY4_BIT_04; }
			if ( j>Count ) 
			{	break; }
			if ( Bit[j++] )
			{	Byte |= ROCKEY4_BIT_05; }
			if ( j>Count ) 
			{	break; }
			if ( Bit[j++] )
			{	Byte |= ROCKEY4_BIT_06; }
			if ( j>Count ) 
			{	break; }
			if ( Bit[j++] )
			{	Byte |= ROCKEY4_BIT_07; }
			if ( j>Count ) 
			{	break; }
			if ( Bit[j++] )
			{	Byte |= ROCKEY4_BIT_08; }

			Buffer[Len++] = Byte;
		}
		Buffer[Len++] = Byte;	
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::ReadJETAllFunc_All(int AppID, std::vector<bool> &BitList)//讀取所有特殊功能
{
	int  i=0, j=0;	
	WORD hid=0;	
	size_t BitCnt=0;
	size_t TmpCnt=0;
	size_t UseCnt=0;
	std::vector<bool> TmpList;	
	BitList.clear();
	for ( i=0; i<ROCKEY4_MAX_COUNT; i++ )
	{
		hid = (WORD)(i);
		if ( CheckHIDValid(hid) == false ) { continue; }
		if ( ReadJETAllFunc(AppID, TmpList, hid) == false ) { return false; }
		BitCnt = BitList.size();
		TmpCnt = TmpList.size();
		if ( 0 == BitCnt )
		{	BitList = TmpList; }
		else
		{
			if ( TmpCnt < BitCnt ) { UseCnt = TmpCnt; }
			else { UseCnt = BitCnt; }			
			for ( j=0; j<UseCnt; j++ )
			{	
				if ( true == TmpList[j] ) 
				{	BitList[j] = true; }				
			}
			for ( j=UseCnt; j<TmpCnt; j++ )
			{	BitList.push_back(TmpList[j]); }
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::ReadJETAllFunc(int AppID, std::vector<bool> &BitList, WORD HID)//讀取所有特殊功能
{
	if ( HID_ALL == HID )
	{	return ReadJETAllFunc_All(AppID, BitList); }

	int  i=0;
	bool Bit[8];
	WORD Len = 0;
	BYTE Buffer[ROCKEY4_BUFFER_SIZE];	
	if ( ReadJETAllFuncBuffer(AppID, Len, Buffer, HID) == false )
	{	return false;	}	
	BitList.clear();
	for ( i=0; i<Len; i++ )
	{	
		if ( 0x00 != Buffer[i] )
		{	i = i; }

		CheckByte(Buffer[i], Bit);	
		BitList.push_back(Bit[0]);
		BitList.push_back(Bit[1]);
		BitList.push_back(Bit[2]);
		BitList.push_back(Bit[3]);
		BitList.push_back(Bit[4]);
		BitList.push_back(Bit[5]);
		BitList.push_back(Bit[6]);
		BitList.push_back(Bit[7]);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::WriteJETAllFunc_All(int AppID, std::vector<bool> &BitList)//寫入所有特殊功能
{
#ifdef ROCKEY4_DEVELOPER
	int  i=0;
	int  Len=0;
	WORD hid=0;
	BYTE Buffer[ROCKEY4_BUFFER_SIZE];	
	if ( BitListToBuffer(BitList, Len, Buffer) == false ) 
	{	return false; }
	for ( i=0; i<ROCKEY4_MAX_COUNT; i++ )
	{
		hid = (WORD)(i);
		if ( CheckHIDValid(hid) == false ) { continue; }
		if ( WriteJETAllFuncBuffer(AppID, Len, Buffer, hid) == false ) { return false; }
	}
	return true;
#else
	return SupportDevelopVersion();
#endif//ROCKEY4_DEVELOPER	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::WriteJETAllFunc(int AppID, std::vector<bool> &BitList, WORD HID)//寫入所有特殊功能
{
#ifdef ROCKEY4_DEVELOPER
	if ( HID_ALL == HID )
	{	return WriteJETAllFunc_All(AppID, BitList); }

	int  Len=0;
	bool bValid=true;	
	BYTE Buffer[ROCKEY4_BUFFER_SIZE];	
	int Pos = GetAppFuncIDPos(AppID, 0);	
	int PosEnd = GetAppMemoryStartPos(AppID+1, false);		
	if ( CheckJETAppValid(AppID, bValid, HID) == false )
	{	return false; }	
	if ( CheckAppValid(bValid) == false )
	{	return false; }
	if ( BitListToBuffer(BitList, Len, Buffer) == false ) 
	{	return false; }
	if ( (Pos+Len) > PosEnd )
	{
		::sprintf(m_ErrorString, "Error, Buffer size Exception(%d)", Len);
		return false; 
	}
	if ( CRockey4ND::WriteDataMemory(Pos, Len, Buffer, HID) == false )
	{	return false; }	
	return true;
#else
	return SupportDevelopVersion();
#endif//ROCKEY4_DEVELOPER	
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::BitListToBuffer(const std::vector<bool> &BitList, int &Len, unsigned char Buffer[])
{
	int   i=0, j=0;
	unsigned char Byte=0;
	const int Count = (int)(BitList.size());
	const int Mod=Count%8;
	if ( 0 == Mod )
	{
		for ( i=0; i<Count; i+=8 )
		{
			j = i;
			Byte = 0;			
			if ( BitList[j++] )
			{	Byte |= ROCKEY4_BIT_01; }
			if ( BitList[j++] )
			{	Byte |= ROCKEY4_BIT_02; }			
			if ( BitList[j++] )
			{	Byte |= ROCKEY4_BIT_03; }
			if ( BitList[j++] )
			{	Byte |= ROCKEY4_BIT_04; }
			if ( BitList[j++] )
			{	Byte |= ROCKEY4_BIT_05; }
			if ( BitList[j++] )
			{	Byte |= ROCKEY4_BIT_06; }
			if ( BitList[j++] )
			{	Byte |= ROCKEY4_BIT_07; }
			if ( BitList[j++] )
			{	Byte |= ROCKEY4_BIT_08; }
			Buffer[Len++] = Byte;	
		}
	}
	else
	{	
		for ( i=0; i<Count; i+=8 )
		{
			j = i;
			Byte = 0;
			if ( j>Count ) 
			{	break; }
			if ( BitList[j++] )
			{	Byte |= ROCKEY4_BIT_01; }
			if ( j>Count ) 
			{	break; }
			if ( BitList[j++] )
			{	Byte |= ROCKEY4_BIT_02; }
			if ( j>Count ) 
			{	break; }
			if ( BitList[j++] )
			{	Byte |= ROCKEY4_BIT_03; }
			if ( j>Count ) 
			{	break; }
			if ( BitList[j++] )
			{	Byte |= ROCKEY4_BIT_04; }
			if ( j>Count ) 
			{	break; }
			if ( BitList[j++] )
			{	Byte |= ROCKEY4_BIT_05; }
			if ( j>Count ) 
			{	break; }
			if ( BitList[j++] )
			{	Byte |= ROCKEY4_BIT_06; }
			if ( j>Count ) 
			{	break; }
			if ( BitList[j++] )
			{	Byte |= ROCKEY4_BIT_07; }
			if ( j>Count ) 
			{	break; }
			if ( BitList[j++] )
			{	Byte |= ROCKEY4_BIT_08; }

			Buffer[Len++] = Byte;
		}
		Buffer[Len++] = Byte;	
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::ReadJETAllFuncBuffer(int AppID, WORD &Len, unsigned char Data[], WORD HID)//讀取所有特殊功能
{
	bool bValid=true;
	BYTE Buffer[ROCKEY4_BUFFER_SIZE];	
	int Pos = GetAppFuncIDPos(AppID, 0);	
	int PosEnd = GetAppMemoryStartPos(AppID+1, false);
	Len = PosEnd-Pos;
	::memset(Buffer, 0x00, Len);
	if ( CheckJETAppValid(AppID, bValid, HID) == false )
	{	return false; }	
	if ( CheckAppValid(bValid) == false )
	{	return false; }
	if ( CRockey4ND::ReadDataMemory(Pos, Len, (char*)Buffer, HID) == false )
	{	return false; }	
	::memcpy(Data, Buffer, Len);
	Data[Len] = '\0';
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::WriteJETAllFuncBuffer(int AppID, WORD Len, const unsigned char Data[], WORD HID)//讀取所有特殊功能
{
#ifdef ROCKEY4_DEVELOPER
	bool bValid=true;	
	int Pos = GetAppFuncIDPos(AppID, 0);	
	int PosEnd = GetAppMemoryStartPos(AppID+1, false);		
	if ( CheckJETAppValid(AppID, bValid, HID) == false )
	{	return false; }	
	if ( CheckAppValid(bValid) == false )
	{	return false; }
	if ( CRockey4ND::WriteDataMemory(Pos, Len, Data, HID) == false )
	{	return false; }	
	return true;
#else
	return SupportDevelopVersion();
#endif//ROCKEY4_DEVELOPER	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::EnableJETFuncID_All(int AppID, DWORD FuncID)//啟用特殊功能		
{
#ifdef ROCKEY4_DEVELOPER
	int  i=0;
	WORD hid=0;
	bool bTemp=true;
	for ( i=0; i<ROCKEY4_MAX_COUNT; i++ )
	{
		hid = (WORD)(i);
		if ( CheckHIDValid(hid) == false ) { continue; }
		if ( EnableJETFuncID(AppID, FuncID, hid) == false ) { return false; }
	}
	return true;
#else
	return SupportDevelopVersion();
#endif//ROCKEY4_DEVELOPER	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::EnableJETFuncID(int AppID, DWORD FuncID, WORD HID)//啟用特殊功能
{
#ifdef ROCKEY4_DEVELOPER
	if ( HID_ALL == HID )
	{	return EnableJETFuncID_All(AppID, FuncID); }

	bool bValid=true;
	unsigned char val=0;
	DWORD Bit = GetBit(FuncID%8);
	int Pos = GetAppFuncIDPos(AppID, FuncID);	
	if ( CheckJETAppValid(AppID, bValid, HID) == false )
	{	return false; }
	if ( CheckAppValid(bValid) == false )
	{	return false; }
	if ( CRockey4ND::ReadDataMemory(Pos, val, HID) == false )
	{	return false; }
	val |= Bit;
	if ( CRockey4ND::WriteDataMemory(Pos, val, HID) == false )
	{	return false; }	
#else
	return SupportDevelopVersion();
#endif//ROCKEY4_DEVELOPER
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::DisableJETFuncID_All(int AppID, DWORD FuncID)//關閉特殊功能		
{
#ifdef ROCKEY4_DEVELOPER
	int  i=0;
	WORD hid=0;
	bool bTemp=true;
	for ( i=0; i<ROCKEY4_MAX_COUNT; i++ )
	{
		hid = (WORD)(i);
		if ( CheckHIDValid(hid) == false ) { continue; }
		if ( DisableJETFuncID(AppID, FuncID, hid) == false ) { return false; }
	}
	return true;
#else
	return SupportDevelopVersion();
#endif//ROCKEY4_DEVELOPER	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::DisableJETFuncID(int AppID, DWORD FuncID, WORD HID)//關閉特殊功能		
{
#ifdef ROCKEY4_DEVELOPER
	if ( HID_ALL == HID )
	{	return DisableJETFuncID_All(AppID, FuncID); }

	bool bValid=true;
	unsigned char val=0;
	DWORD Bit = GetBit(FuncID%8);
	int Pos = GetAppFuncIDPos(AppID, FuncID);	
	if ( CheckJETAppValid(AppID, bValid, HID) == false )
	{	return false; }
	if ( CheckAppValid(bValid) == false )
	{	return false; }
	if ( CRockey4ND::ReadDataMemory(Pos, val, HID) == false )
	{	return false; }
	Bit = ~Bit;
	val &= Bit;
	if ( CRockey4ND::WriteDataMemory(Pos, val, HID) == false )
	{	return false; }	
#else
	return SupportDevelopVersion();
#endif//ROCKEY4_DEVELOPER
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::CheckJETFuncID_All(int AppID, DWORD FuncID, bool &bEnabled)//確認特殊功能		
{
	int  i=0;
	WORD hid=0;
	bool bTemp=true;
	for ( i=0; i<ROCKEY4_MAX_COUNT; i++ )
	{
		hid = (WORD)(i);
		if ( CheckHIDValid(hid) == false ) { continue; }
		if ( CheckJETFuncID(AppID, FuncID, bTemp, hid) == false ) { return false; }		
		if ( true == bTemp ) 
		{	
			bEnabled = true;
			return true; 
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::CheckJETFuncID(int AppID, DWORD FuncID, bool &bEnabled, WORD HID)//確認特殊功能
{	
	if ( HID_ALL == HID )
	{	return CheckJETFuncID_All(AppID, FuncID, bEnabled);	}

	DWORD val=0;
	bool bValid=true;
	int Pos = GetAppFuncIDPos(AppID, FuncID);
	if ( CheckJETAppValid(AppID, bValid, HID) == false )
	{	return false; }
	if ( CheckAppValid(bValid) == false )
	{	return false; }
	if ( CRockey4ND::ReadDataMemory(Pos, val, HID) == false )
	{	return false; }
	DWORD Bit = GetBit(FuncID%8);
	DWORD Res=val&Bit;
	if ( 0 == Res ) 
	{	bEnabled = false; }
	else
	{	bEnabled = true; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::WriteJETCustomerID_All(int AppID, WORD CustomerID)//寫入客戶編號
{
#ifdef ROCKEY4_DEVELOPER
	int  i=0;
	WORD hid=0;
	bool bTemp=true;
	for ( i=0; i<ROCKEY4_MAX_COUNT; i++ )
	{
		hid = (WORD)(i);
		if ( CheckHIDValid(hid) == false ) { continue; }
		if ( WriteJETCustomerID(AppID, CustomerID, hid) == false ) { return false; }
	}
	return true;
#else
	return SupportDevelopVersion();
#endif//ROCKEY4_DEVELOPER	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::WriteJETCustomerID(int AppID, WORD CustomerID, WORD HID)//寫入客戶編號
{
#ifdef ROCKEY4_DEVELOPER
	if ( HID_ALL == HID )
	{	return WriteJETCustomerID_All(AppID, CustomerID);	}

	bool bValid=true;
	int Pos = GetAppCustomerIDPos(AppID);		
	if ( CheckJETAppValid(AppID, bValid, HID) == false )
	{	return false; }
	if ( CheckAppValid(bValid) == false )
	{	return false; }
	if ( CRockey4ND::WriteDataMemory(Pos, CustomerID, HID) == false )
	{	return false; }	
#else
	return SupportDevelopVersion();
#endif//ROCKEY4_DEVELOPER
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::ReadJETCustomerID(int AppID, WORD &CustomerID, WORD HID)//讀取客戶編號
{
	WORD val=0;
	bool bValid=true;
	int Pos = GetAppCustomerIDPos(AppID);
	if ( CheckJETAppValid(AppID, bValid, HID) == false )
	{	return false; }
	if ( CheckAppValid(bValid) == false )
	{	return false; }
	if ( CRockey4ND::ReadDataMemory(Pos, val, HID) == false )
	{	return false; }
	CustomerID = val;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::ReadJETAppFuncItemList(int AppID, WORD HID)//更新特殊功能列表
{
	std::vector<bool> BitList;
	if ( LoadJETAppINI(AppID) == false )
	{	return false; }
	if ( ReadJETAllFunc(AppID, BitList, HID) == false ) 
	{	return false; }

	int   i=0, j=0;
	TFuncItem FuncItem;
	const int FuncCount=GetFuncCount();
	const int BitCount=(int)(BitList.size());
	for ( i=0; i<FuncCount; i++ )
	{
		if ( GetFuncItem(i, FuncItem) == false )
		{	break; }
		if ( FuncItem.FuncID >= BitCount ) { continue; }
		FuncItem.bOnOff = BitList[FuncItem.FuncID];
		SetFuncItem(i, FuncItem);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRockey4ND_JET::WreadJETAppFuncItemList(int AppID, WORD HID)//寫入軟體特殊功能列表
{
	std::vector<bool> BitList;	
	if ( ReadJETAllFunc(AppID, BitList, HID) == false ) 
	{	return false; }

	int   i=0, j=0;
	TFuncItem FuncItem;
	const int FuncCount=GetFuncCount();
	const int BitCount=(int)(BitList.size());
	//其餘設定為0
	for ( i=0; i<BitCount; i++ )
	{	BitList[i] = false; }
	for ( i=0; i<FuncCount; i++ )
	{
		if ( GetFuncItem(i, FuncItem) == false )
		{	break; }
		if ( FuncItem.FuncID >= BitCount ) { continue; }
		BitList[FuncItem.FuncID] = FuncItem.bOnOff;		
	}
	if ( WriteJETAllFunc(AppID, BitList, HID) == false )
	{	return false; }

	if ( SaveJETAppINI(AppID) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//