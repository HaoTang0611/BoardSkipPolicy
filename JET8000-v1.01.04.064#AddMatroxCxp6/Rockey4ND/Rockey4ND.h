#ifndef _Rockey4ND_H_
#define _Rockey4ND_H_
//-------------------------------------------------------------------------------------//
//#include "Rockey4ND_Define.h"
//-------------------------------------------------------------------------------------//
//#define ROCKEY4_DEVELOPER//開發版本
//-------------------------------------------------------------------------------------//
#define ROCKEY4_MAX_COUNT                      32//最多Rockey4的數量
#define ROCKEY4_INVALID                        0xFFFF//無效的Rockey4-Handle
#define ROCKEY4_BUFFER_SIZE                    1024
//-------------------------------------------------------------------------------------//
#define  CURRENT_TIME_GLOBAL                   1//全球時區
#define  CURRENT_TIME_LOCAL                    2//當地時區
#define  CURRENT_TIME_MODE                     CURRENT_TIME_LOCAL   
//#define  CURRENT_TIME_MODE                     CURRENT_TIME_GLOBAL   
//-------------------------------------------------------------------------------------//
#define  ROCKEY4_TIMER_BY_DATE                 1
#define  ROCKEY4_TIMER_BY_HOUR                 2
#define  ROCKEY4_TIMER_BY_DAY                  3
//-------------------------------------------------------------------------------------//
#define  ROCKEY4_ALG_ID_READ_MODULE              0//演算法編號-讀取比較模組
#define  ROCKEY4_ALG_ID_CALCULATE_1            124//演算法編號-顯示計算1內部參數
//-------------------------------------------------------------------------------------//
#ifndef _WIN64
	#pragma comment(lib,"..\\JET8000_Library\\Ry4S\\Ry4S.lib")	
#else
	#pragma comment(lib,"..\\JET8000_Library\\Ry4S\\Ry4S_X64.lib")
#endif//_WIN64
#include "..\\JET8000_Library\\Ry4S\\ry4s.h"
//-------------------------------------------------------------------------------------//
class CRockey4ND
{
private:
	//---------------------------------------------------------------------------------//
	int                        m_HardIndex;//Active Rockey4 Index	
	WORD                       m_Handle[ROCKEY4_MAX_COUNT];//Rockey4的Handle	
	DWORD                      m_HardNumber[ROCKEY4_MAX_COUNT];//Rockey4的Hard Number
	WORD                       m_RetCode;//Rockey4 Return Code;
	WORD                       m_Param[4];//Rockey4 param(p1,p2,p3,p4);
	DWORD                      m_LParam[2];//Rockey4 long param(lp1,lp2);
	DWORD                      m_Rockey4Ver;	
	char                       m_DeviceError[256];//ErrCode Form Rockey4 API;
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//		
	char                       m_ErrorString[256];//Total ErrorCode
	//---------------------------------------------------------------------------------//		
	CRockey4ND(const CRockey4ND &Rockey4);
	CRockey4ND& operator=(const CRockey4ND &Rockey4);
	//---------------------------------------------------------------------------------//		
	void                       PreInitRockey4ND();
	void                       InitialRockey4ND();	
	void                       CloneRockey4ND(const CRockey4ND &Rockey4);
	//---------------------------------------------------------------------------------//	
	bool                       CloseHID(WORD HID);	
	bool                       CheckRockey4Type(DWORD Type);
	bool                       CheckRockey4NDRetCode(WORD RetCode);
	//---------------------------------------------------------------------------------//
	bool                       SetHardIndex(int val);
	int                        GetHardIndex() const;
	//---------------------------------------------------------------------------------//
	void                       SetHardNumber(DWORD lp1);
	//---------------------------------------------------------------------------------//
	void                       SetLParam(DWORD LP1=0x00, DWORD LP2=0x00);
	void                       GetLParam(DWORD &LP1, DWORD &LP2) const;
	//---------------------------------------------------------------------------------//	
	void                       SetParam(WORD P1=0x00, WORD P2=0x00, WORD P3=0x00, WORD P4=0x00);
	void                       GetParam(WORD &P1, WORD &P2, WORD &P3, WORD &P4) const;
	//---------------------------------------------------------------------------------//
	WORD                       ExecRockeyFnc(int fnID, unsigned char Buffer[]);
	//---------------------------------------------------------------------------------//	
	bool                       SupportDevelopVersion();
	//---------------------------------------------------------------------------------//
	
public:
	//---------------------------------------------------------------------------------//	
	CRockey4ND();
	~CRockey4ND();
	//---------------------------------------------------------------------------------//	
	WORD                       GetRetCode() const;
	//---------------------------------------------------------------------------------//	
	const char*                GetErrorString() const;	
	//---------------------------------------------------------------------------------//	
	virtual bool               OpenRockey4();
	bool                       CloseRockey4();	
	//---------------------------------------------------------------------------------//
	bool                       CheckHIDValid(WORD HID);
	bool                       GetHardNumber(DWORD &HNum, WORD HID) const;
	//---------------------------------------------------------------------------------//
	bool                       ReadUserID(DWORD &val, WORD HID=0);
	bool                       WriteUserID(DWORD val, WORD HID=0);	
	//---------------------------------------------------------------------------------//
	bool                       CreateRandom(int &v1, WORD HID=0);
	bool                       CreateRandom(int &v1, int &v2, WORD HID=0);
	bool                       CreateRandom(int &v1, int &v2, int &v3, WORD HID=0);
	bool                       CreateRandom(int &v1, int &v2, int &v3, int &v4, WORD HID=0);
	//---------------------------------------------------------------------------------//
	bool                       GetSeedCode(int val, int &v1, int &v2, int &v3, int &v4, WORD HID=0);
	//---------------------------------------------------------------------------------//
	bool                       ReadDataMemory(int Pos, WORD &Data, WORD HID=0);	
	bool                       ReadDataMemory(int Pos, DWORD &Data, WORD HID=0);	
	bool                       ReadDataMemory(int Pos, unsigned char &Data, WORD HID=0);	
	bool                       ReadDataMemory(int Pos, int Len, char Data[], WORD HID=0);
	//---------------------------------------------------------------------------------//
	bool                       WriteDataMemory(int Pos, WORD Data, WORD HID=0);	
	bool                       WriteDataMemory(int Pos, DWORD Data, WORD HID=0);
	bool                       WriteDataMemory(int Pos, unsigned char Data, WORD HID=0);
	bool                       WriteDataMemory(int Pos, const char Data[], WORD HID=0);	
	bool                       WriteDataMemory(int Pos, int Len, const unsigned char Data[], WORD HID=0);	
	//---------------------------------------------------------------------------------//	
	bool                       DecreaseModule(int ID, WORD HID=0);//ModuleID(0~63), AutoDecrease(0,1)
	bool                       SetModule(int ID, int Content, int AutoDecrease, WORD HID=0);//ModuleID(0~63), AutoDecrease(0,1)
	bool                       CheckModule(int ID, int &Content, int &AutoDecrease, WORD HID=0);//ModuleID(0~63), AutoDecrease(0,1)	
	bool                       ReadModuleContent(int ID, int &Content, WORD HID=0);////ModuleID(0~63)
	//---------------------------------------------------------------------------------//
	bool                       WriteArithmetic(int AlgID, const char AlgFunc[], WORD HID=0);//寫入演算法公式
	bool                       WriteArithmetic_ReadModelContent(WORD HID=0);//寫入演算法公式-讀取模組內容 
	//---------------------------------------------------------------------------------//
	bool                       CalculateBySeed(int AlgID, int Seed, int &v1, int &v2, int &v3, int &v4, WORD HID=0);
	bool                       CalculateByModule(int AlgID, int ModuleID, int &v1, int &v2, int &v3, int &v4, WORD HID=0);	
	bool                       CalculateByModuleList(int AlgID, int FirstModuleID, int &v1, int &v2, int &v3, int &v4, WORD HID=0);	
	//---------------------------------------------------------------------------------//	
	bool                       SetDesKey(int DESMode, const char Key[], WORD HID=0);//設定DES的金鑰, DES:0, 3DES:1
	bool                       EncryptDes(int DESMode, int len, const char Data[], char Encrypt[], WORD HID=0);//以DES加密, DES:0, 3DES:1, len:8n
	bool                       DecryptDes(int DESMode, int len, const char Data[], char Decrypt[], WORD HID=0);//以DES解密, DES:0, 3DES:1, len:8n	
	//---------------------------------------------------------------------------------//	
	bool                       SetRsaKeyN(const char Key[], WORD HID=0);//設定RSA的金鑰-N
	bool                       SetRsaKeyD(const char Key[], WORD HID=0);//設定RSA的金鑰-D
	bool                       EncryptRsa(int KeyMode, int len, int PadMode, const char Data[], char Encrypt[], WORD HID=0);//以RSA加密
	bool                       DecryptRsa(int KeyMode, int len, int PadMode, const char Data[], char Decrypt[], WORD HID=0);//以RSA解密
	//---------------------------------------------------------------------------------//	
	bool                       SetCountEx(int ID, DWORD Count, int Decrease, WORD HID=0);//設定次數
	bool                       GetCountEx(int ID, DWORD &Count, int &Decrease, WORD HID=0);//取得次數
	//---------------------------------------------------------------------------------//			
	bool                       GetElapsedTime(const SYSTEMTIME &time, DWORD &Ret, WORD HID=0);//相對於2006/01/01-00::00::00	
	bool                       GetElapsedTime(DWORD Year, WORD Month, WORD Day, WORD Hour, WORD Minute, DWORD &Ret, WORD HID=0);//相對於2006/01/01-00::00::00
	bool                       SetTimerEx(int ID, int Mode, SYSTEMTIME time, DWORD Hours, DWORD Days, WORD HID=0);//設定計時器, Mode:1:Date, 2:Hour, 3:Day
	bool                       GetTimerEx(int ID, int &Mode, SYSTEMTIME &time, DWORD &Hours, DWORD &Days, WORD HID=0);//取得計時器, Mode:1:Date, 2:Hour, 3:Day
	bool                       AdjustTimerEx(int ID, SYSTEMTIME &time, WORD HID=0);//同步計時器
	//---------------------------------------------------------------------------------//	
	DWORD                      GetRockey4Ver() const;
	bool                       ReadRockey4Ver(WORD HID=0);//讀取版本	
	//---------------------------------------------------------------------------------//	
	bool                       GetBaseDateTime(SYSTEMTIME &CurTime);//取得時間系起點
	bool                       GetCurrentDateTime(SYSTEMTIME &CurTime);		
	bool                       CalcElapsedTime_Day(const SYSTEMTIME &t1, const SYSTEMTIME &t2, DWORD &dwDay);//以日為主
	bool                       CalcElapsedTime_Hour(const SYSTEMTIME &t1, const SYSTEMTIME &t2, DWORD &dwHour);//以小時為主
	bool                       CalcElapsedTime_Minute(const SYSTEMTIME &t1, const SYSTEMTIME &t2, DWORD &dwMinutes);//以分鐘為主	
	//---------------------------------------------------------------------------------//	
	bool                       CalcLastDateByDay(const SYSTEMTIME &CurTime, DWORD Days, SYSTEMTIME &NextTime);//計算之前日期-天數
	bool                       CalcLastDateByHour(const SYSTEMTIME &CurTime, DWORD Hours, SYSTEMTIME &NextTime);//計算之前日期-小時
	bool                       CalcLastDateByMinute(const SYSTEMTIME &CurTime, DWORD Minutes, SYSTEMTIME &NextTime);//計算之前日期-分鐘
	bool                       CalcLastDateBySecond(const SYSTEMTIME &CurTime, DWORD Seconds, SYSTEMTIME &NextTime);//計算之前日期-秒數
	//---------------------------------------------------------------------------------//	
	bool                       CalcNextDateByDay(const SYSTEMTIME &CurTime, DWORD Days, SYSTEMTIME &NextTime);//計算之後日期-天數
	bool                       CalcNextDateByHour(const SYSTEMTIME &CurTime, DWORD Hours, SYSTEMTIME &NextTime);//計算之後日期-小時
	bool                       CalcNextDateByMinute(const SYSTEMTIME &CurTime, DWORD Minutes, SYSTEMTIME &NextTime);//計算之後日期-分鐘
	bool                       CalcNextDateBySecond(const SYSTEMTIME &CurTime, DWORD Seconds, SYSTEMTIME &NextTime);//計算之後日期-秒數
	//---------------------------------------------------------------------------------//	
};
#endif//_Rockey4ND_H_
