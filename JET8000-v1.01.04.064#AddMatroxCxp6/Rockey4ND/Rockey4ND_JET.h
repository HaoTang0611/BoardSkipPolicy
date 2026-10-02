#ifndef _Rockey4ND_JET_H_
#define _Rockey4ND_JET_H_
//-------------------------------------------------------------------------------------//
#include <vector>
#include "Rockey4ND.h"
//-------------------------------------------------------------------------------------//
#define HID_ALL                                0xFFFF//全部掃描
//-------------------------------------------------------------------------------------//
//#define ROECKEY4_TIMER_ENABLE//啟用Rocket4的計時器
//-------------------------------------------------------------------------------------//
#define ROCKEY4_BIT_01                         0x0001
#define ROCKEY4_BIT_02                         0x0002
#define ROCKEY4_BIT_03                         0x0004
#define ROCKEY4_BIT_04                         0x0008
#define ROCKEY4_BIT_05                         0x0010
#define ROCKEY4_BIT_06                         0x0020
#define ROCKEY4_BIT_07                         0x0040
#define ROCKEY4_BIT_08                         0x0080
#define ROCKEY4_BIT_09                         0x0100
#define ROCKEY4_BIT_10                         0x0200
#define ROCKEY4_BIT_11                         0x0400
#define ROCKEY4_BIT_12                         0x0800
#define ROCKEY4_BIT_13                         0x1000
#define ROCKEY4_BIT_14                         0x2000
#define ROCKEY4_BIT_15                         0x4000
#define ROCKEY4_BIT_16                         0x8000
//-------------------------------------------------------------------------------------//
//JET軟體樣式
enum JET_APP_ID
{
	JET_APP_NONE    = 0,	
	JET_APP_JET6500 = 2,
	JET_APP_JET7000 = 4,
	JET_APP_JET8000 = 6,

	JET_APP_JET_ARS = 10,
	JET_APP_JET_MIC = 12,
	JET_APP_JET_SFC = 14,

	JET_APP_JET_VRS = 20,
	JET_APP_JET_MSC = 22,

	JET_APP_END     = 30
};
#define JET_APP_COUNT         32
//-------------------------------------------------------------------------------------//
typedef struct tagAppItem
{
	int     nAppID;
	bool    bEnabled;	
} TAppItem, *PAppItem;
//---------------------------------------------------------------------------------//
typedef struct tagFuncItem
{
	DWORD     FuncID;
	bool      bOnOff;
	CString   sText;
} TFuncItem, *PFuncItem;
//---------------------------------------------------------------------------------//
class CRockey4ND_JET:public CRockey4ND
{
private:
	//---------------------------------------------------------------------------------//
	int                        m_DongleVer;
	JET_APP_ID                 m_AppID;
	//---------------------------------------------------------------------------------//
	char                       m_AppFolder[256];
	std::vector<TFuncItem>     m_FuncItemList;
	//---------------------------------------------------------------------------------//
	DWORD                      m_RemainingCount[JET_APP_COUNT];//剩餘次數
	//---------------------------------------------------------------------------------//
	DWORD                      m_ElapsedTime_Build[JET_APP_COUNT];//創建時間
	DWORD                      m_ElapsedTime_Check[JET_APP_COUNT];//確認時間
	DWORD                      m_ElapsedTime_Expired[JET_APP_COUNT];//逾期時間
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//
	CRockey4ND_JET(const CRockey4ND_JET &Rockey4);
	CRockey4ND_JET& operator=(const CRockey4ND_JET &Rockey4);
	//---------------------------------------------------------------------------------//
	void                       PreInitRockey4ND_JET();
	void                       InitialRockey4ND_JET();	
	void                       CloneRockey4ND_JET(const CRockey4ND_JET &Rockey4);
	//---------------------------------------------------------------------------------//		
	bool                       CheckUserID_JET();	
	//---------------------------------------------------------------------------------//	
	bool                       ReadJETTimerEnabled_All(int AppID, bool &bEnabled);//讀取JET計時器使用 
	bool                       ReadJETTimerEnabled(int AppID, bool &bEnabled, WORD HID=HID_ALL);//讀取JET計時器使用 
	//---------------------------------------------------------------------------------//
	bool                       ReadJETTimerValid_All(int AppID, bool &bValid);//讀取JET計時器有效-無逾期
	bool                       ReadJETTimerValid(int AppID, bool &bValid, WORD HID=HID_ALL);//讀取JET計時器有效-無逾期	
	//---------------------------------------------------------------------------------//
	bool                       ReadJETCountEnabled_All(int AppID, bool &bEnabled);//讀取JET計次器使用 
	bool                       ReadJETCountEnabled(int AppID, bool &bEnabled, WORD HID=HID_ALL);//讀取JET計次器使用 
	//---------------------------------------------------------------------------------//
	bool                       ReadJETCountValid_All(int AppID, bool &bValid);//讀取JET計次器有效-次數符合
	bool                       ReadJETCountValid(int AppID, bool &bValid, WORD HID=HID_ALL);//讀取JET計次器有效-次數符合
	//---------------------------------------------------------------------------------//	
public:
	//---------------------------------------------------------------------------------//
	CRockey4ND_JET();
	~CRockey4ND_JET();
	//---------------------------------------------------------------------------------//
	virtual bool               OpenRockey4();
	//---------------------------------------------------------------------------------//
	int                        GetDongleVer() const;
	//---------------------------------------------------------------------------------//
	JET_APP_ID                 GetJETAppID() const;	
	void                       SetJETAppID(JET_APP_ID AppID);		
	bool                       GetAppIDText(JET_APP_ID AppID, char* Text);	
	//---------------------------------------------------------------------------------//
	const char*                GetAppFolder() const;
	void                       SetAppFolder(const char* Folder);	
	//---------------------------------------------------------------------------------//	
	bool                       LoadJETAppINI(int AppID);
	bool                       SaveJETAppINI(int AppID);
	//---------------------------------------------------------------------------------//
	void                       ClearFuncList();
	size_t                     GetFuncCount() const;
	void                       AddFuncText(LPCTSTR FuncText);
	LPCTSTR                    GetFuncText(size_t idx, bool bChk);
	bool                       GetFuncItem(size_t idx, TFuncItem &Item);
	bool                       SetFuncItem(size_t idx, const TFuncItem &Item);
	bool                       EnableFunctItemList(bool bEnable);
	//---------------------------------------------------------------------------------//
	unsigned char              GetBit(WORD val);//取得位元數
	WORD                       GetAppMemorySize(bool bPublic);//取得軟體記憶體尺寸
	WORD                       GetAppBuildTimePos(int AppID);//取得軟體的創建日期時間
	WORD                       GetAppCheckTimePos(int AppID);//取得軟體的確認日期時間
	WORD                       GetAppExpiredTimePos(int AppID);//取得軟體的逾時日期時間
	WORD                       GetAppFuncIDPos(int AppID, int FuncID);//取得軟體的功能編號位置
	WORD                       GetAppCustomerIDPos(int AppID);//取得軟體的客戶編號位置
	WORD                       GetAppMemoryStartPos(bool bPublic);//取得軟體記憶體起始點
	WORD                       GetAppMemoryStartPos(int AppID, bool bPublic);//取得軟體記憶體起始點
	//---------------------------------------------------------------------------------//
	bool                       CheckByte(unsigned char Byte, bool Bit[]);//確認位元組
	//---------------------------------------------------------------------------------//
	DWORD                      GetRemainingCount(int AppID);//取得剩餘次數	
	bool                       CheckAppRemainingCount(int AppID, DWORD Count);//確認剩餘次數是否大於...
	//---------------------------------------------------------------------------------//	
	bool                       GetAppBuildTime(int AppID, SYSTEMTIME &time);//創建時間
	bool                       GetAppCheckTime(int AppID, SYSTEMTIME &time);//確認時間
	bool                       GetAppExpiredTime(int AppID, SYSTEMTIME &time);//逾期時間
	//---------------------------------------------------------------------------------//
	bool                       CheckAppRemainingDay(int AppID, DWORD Days);//確認剩餘天數是否大於
	//---------------------------------------------------------------------------------//
	bool                       EnableJETDongle_All();//啟用JET硬體鎖 	
	bool                       EnableJETDongle(WORD HID=HID_ALL);//啟用JET硬體鎖 		
	//---------------------------------------------------------------------------------//
	bool                       CheckAppID(int AppID);
	bool                       CheckAppValid(bool bValid);
	bool                       CheckAppTimerValid(bool bValid);
	bool                       CheckAppCountValid(bool bValid);
	//---------------------------------------------------------------------------------//	
	bool                       CheckJETAppValid_All(int AppID, bool &bValid);//確認JET有效用
	bool                       CheckJETAppValid(int AppID, bool &bValid, WORD HID=HID_ALL);//確認JET有效用
	//---------------------------------------------------------------------------------//		
	bool                       ListJETAppValid(std::vector<TAppItem> &AppList, WORD HID=HID_ALL);
	//---------------------------------------------------------------------------------//	
	bool                       CheckJETTimerValid_All(int AppID, bool &bValid);//確認JET計時器有效-無逾期
	bool                       CheckJETTimerValid(int AppID, bool &bEnable, bool &bValid, WORD HID=HID_ALL);//確認JET計時器有效-無逾期
	//---------------------------------------------------------------------------------//
	bool                       CheckJETCountValid_All(int AppID, bool &bValid);//確認JET計次器有效-次數符合
	bool                       CheckJETCountValid(int AppID, bool &bEnable, bool &bValid, WORD HID=HID_ALL);//確認JET計次器有效-次數符合
	//---------------------------------------------------------------------------------//
	bool                       ReadAppElapsedTimeBuild(int AppID, DWORD &ElapsedTime, WORD HID=0);
	bool                       ReadAppElapsedTimeCheck(int AppID, DWORD &ElapsedTime, WORD HID=0);
	bool                       ReadAppElapsedTimeExpired(int AppID, DWORD &ElapsedTime, WORD HID=0);	
	bool                       CompareTimeValid(const SYSTEMTIME &CurTime, const SYSTEMTIME &ExpTime, int Precision);//比較時間
	//---------------------------------------------------------------------------------//	
	bool                       ReadAppDateTime_All(int AppID, SYSTEMTIME &tBuild, SYSTEMTIME &tCheck, SYSTEMTIME &tExp);
	bool                       ReadAppDateTime(int AppID, SYSTEMTIME &tBuild, SYSTEMTIME &tCheck, SYSTEMTIME &tExp, WORD HID=HID_ALL);
	//---------------------------------------------------------------------------------//	
	bool                       RemoveJETApp_All(int AppID);//移除JET軟體
	bool                       RemoveJETApp(int AppID, WORD HID=HID_ALL);//移除JET軟體
	//---------------------------------------------------------------------------------//
	bool                       EnableJETApp_All(int AppID, DWORD Timer, DWORD Count);//啟用JET軟體
	bool                       EnableJETApp(int AppID, DWORD Timer, DWORD Count, WORD HID=HID_ALL);//啟用JET軟體
	//---------------------------------------------------------------------------------//
	bool                       ResetJETAppMemory_All(int AppID);//清除JET軟體記憶體區塊
	bool                       ResetJETAppMemory(int AppID, WORD HID=HID_ALL);//清除JET軟體記憶體區塊
	//---------------------------------------------------------------------------------//	
	bool                       EnableJETAllFunc_All(int AppID);//啟用所有特殊功能
	bool                       EnableJETAllFunc(int AppID, WORD HID=HID_ALL);//啟用所有特殊功能
	//---------------------------------------------------------------------------------//
	bool                       DisableJETAllFunc_All(int AppID);//關閉所有特殊功能
	bool                       DisableJETAllFunc(int AppID, WORD HID=HID_ALL);//關閉所有特殊功能
	//---------------------------------------------------------------------------------//	
	bool                       ReadJETAllFunc_All(int AppID, int&Count, bool Bit[]);//讀取所有特殊功能
	bool                       ReadJETAllFunc(int AppID, int&Count, bool Bit[], WORD HID=HID_ALL);//讀取所有特殊功能
	//---------------------------------------------------------------------------------//
	bool                       WriteJETAllFunc_All(int AppID, int Count, bool Bit[]);//寫入所有特殊功能
	bool                       WriteJETAllFunc(int AppID, int Count, bool Bit[], WORD HID=HID_ALL);//寫入所有特殊功能
	bool                       BitListToBuffer(int Count, const bool Bit[], int &Len, unsigned char Buffer[]);	
	//---------------------------------------------------------------------------------//
	bool                       ReadJETAllFunc_All(int AppID, std::vector<bool> &BitList);//讀取所有特殊功能
	bool                       ReadJETAllFunc(int AppID, std::vector<bool> &BitList, WORD HID=HID_ALL);//讀取所有特殊功能
	//---------------------------------------------------------------------------------//	
	bool                       WriteJETAllFunc_All(int AppID, std::vector<bool> &BitList);//寫入所有特殊功能
	bool                       WriteJETAllFunc(int AppID, std::vector<bool> &BitList, WORD HID=HID_ALL);//寫入所有特殊功能
	bool                       BitListToBuffer(const std::vector<bool> &BitList, int &Len, unsigned char Buffer[]);	
	//---------------------------------------------------------------------------------//	
	bool                       ReadJETAllFuncBuffer(int AppID, WORD &Len, unsigned char Data[], WORD HID=0);//讀取所有特殊功能
	//---------------------------------------------------------------------------------//	
	bool                       WriteJETAllFuncBuffer(int AppID, WORD Len, const unsigned char Data[], WORD HID=0);//讀取所有特殊功能
	//---------------------------------------------------------------------------------//
	bool                       EnableJETFuncID_All(int AppID, DWORD FuncID);//啟用特殊功能		
	bool                       EnableJETFuncID(int AppID, DWORD FuncID, WORD HID=HID_ALL);//啟用特殊功能		
	//---------------------------------------------------------------------------------//
	bool                       DisableJETFuncID_All(int AppID, DWORD FuncID);//關閉特殊功能		
	bool                       DisableJETFuncID(int AppID, DWORD FuncID, WORD HID=HID_ALL);//關閉特殊功能		
	//---------------------------------------------------------------------------------//
	bool                       CheckJETFuncID_All(int AppID, DWORD FuncID, bool &bEnabled);//確認特殊功能		
	bool                       CheckJETFuncID(int AppID, DWORD FuncID, bool &bEnabled, WORD HID=HID_ALL);//確認特殊功能		
	//---------------------------------------------------------------------------------//	
	bool                       WriteJETCustomerID_All(int AppID, WORD CustomerID);//寫入客戶編號
	bool                       WriteJETCustomerID(int AppID, WORD CustomerID, WORD HID=HID_ALL);//寫入客戶編號
	//---------------------------------------------------------------------------------//		
	bool                       ReadJETCustomerID(int AppID, WORD &CustomerID, WORD HID=0);//讀取客戶編號
	//---------------------------------------------------------------------------------//	
	bool                       ReadJETAppFuncItemList(int AppID, WORD HID=HID_ALL);//更新特殊功能列表
	bool                       WreadJETAppFuncItemList(int AppID, WORD HID=HID_ALL);//寫入軟體特殊功能列表
	//---------------------------------------------------------------------------------//	
};
//-------------------------------------------------------------------------------------//
#ifdef ROCKEY4ND_USE
extern CRockey4ND_JET JET_Rockey4ND;
#endif//ROCKEY4ND_USE
//-------------------------------------------------------------------------------------//
#endif//_Rockey4ND_JET_H_
