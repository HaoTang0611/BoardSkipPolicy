#pragma once
#include "FingerprintDefine.h"
class CFingerprint{
public:
	CFingerprint();
	virtual ~CFingerprint();
private:
//---------------------------------------------------------------------------------//
	CJetSerial              m_RS232COM;			//RS232	
	bool                    m_DeviceConnected;	//裝置連線中
	CString                 m_DevicePort;
	CString                 m_ErrorString;		
	CString                 m_DeviceName;		//裝置名稱
	int                     m_FPSDeviceWaitDataCount; //等待時間
	DWORD                   m_FPSDeviceWaitDataDwellTime;//等待資料的延遲時間-ms
	DWORD                   m_FPSDeviceReadDataDelayTime;//讀取資料前延遲時間-ms
	int                     m_FPSDeviceBaudRate;
//---------------------------------------------------------------------------------//
	//特徵資料大小為 128*6 = 768，開發文件中描述Buffer大小是512，多出來的256目前猜測是包頭包尾。
	const int				m_FeatureSize = 768;//834
	BYTE					m_Feature[768];		//特徵資料
	BYTE					m_FeaturePacket[834];//deprecated，完整封包
//---------------------------------------------------------------------------------//
	//開發過程中棄用的方法
	CString					m_FeatureText;//特徵文字
//---------------------------------------------------------------------------------//
	// 字串定義，移至 AOIDataDefine
	//CString					m_AS608CommandText_GetImage;
	//CString					m_AS608CommandText_GenChar;
	//CString					m_AS608CommandText_Match;
	//CString					m_AS608CommandText_RegMode;
	//CString					m_AS608CommandText_UpChar;
	//CString					m_AS608CommandText_DownChar;

	//CString					m_AS608StatusText_OK;
	//CString					m_AS608StatusText_Error;
	//CString					m_AS608StatusText_NOFingerprint;
	//CString					m_AS608StatusText_InputError;
	//CString					m_AS608StatusText_ImageTooDry;
	//CString					m_AS608StatusText_ImageTooWet;
	//CString					m_AS608StatusText_ImageTooClutter;
	//CString					m_AS608StatusText_ImageTooFewFeature;
	//CString					m_AS608StatusText_NotMatch;
	//CString					m_AS608StatusText_NoMove;

public:
	void					SetCOMPort(unsigned int nPort);	//設定COM Port
	bool                    ConnectDevice();				//連線
//---------------------------------------------------------------------------------//	
	bool					ExecuteRecordFingerprint(LPVOID pParam);	//簡易註冊，讀取一次指紋
	bool					ExecuteRecordFingerprint_V2(LPVOID pParam);	//嚴格註冊，讀取兩次指紋並合併特徵
	bool					ExecuteMatchFingerprint(LPVOID pParam);		//指紋驗證
	bool					ExecuteFindFingerprint(LPVOID pParam);		//指紋尋找
	bool					ExecuteFindFingerprintLoop(LPVOID pParam);	//指紋尋找-持續
//---------------------------------------------------------------------------------//	
	LPCTSTR                 GetErrorString() { return m_ErrorString; }; //取得錯誤文字
	BYTE*					GetAS608FeaturePtr();			//取得特徵資料
//---------------------------------------------------------------------------------//
	//deprecated
	BYTE*					GetAS608FeaturePacketPtr();		//取得完整封包資料
	CString					GetAs608FeatureText();			//取得特徵文字，
//---------------------------------------------------------------------------------//
	//移至 AOIDataDefine
	//bool					InitialDefine();
	//CString				GetAS608StatusText(AS608_STATUS status);
	//CString				GetAS608CommandText(AS608_COMMAND command);
	//CString				GetAS608StatusText(int status);
	//CString				GetAS608CommandText(int command);

private:
//---------------------------------------------------------------------------------//
	//參考自其他使用到Serial的程式碼
	void                    InitialFPSDevice();
	bool                    LoadINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pDefault, TCHAR *pString, int StringSize, LPCTSTR pfilename, bool IsCheckLens, CString &Error);
	bool                    SaveINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pString, LPCTSTR pfilename, CString &Error);
	bool                    LoadFPSINIFile();
	bool                    SaveFPSINIFile();
	void                    SetFingerprintDeviceConnected(bool val) { m_DeviceConnected = val; }
	void                    SetDeviceConnected(bool val) { m_DeviceConnected = val; }
	void                    SetDevicePort(LPCTSTR val) { m_DevicePort = val; }
	LPCTSTR                 GetDevicePort() const { return m_DevicePort; }
	bool					CheckConnected();	//確認是否連線
	bool					Disconnected();		//斷線
	bool                    WaitForDataInQuene();
//---------------------------------------------------------------------------------//
	//發送命令與接受回應
	char					ReadResponse(AS608_COMMAND command);
	char					ReadResponse(unsigned int &size, BYTE& flag, BYTE* data = NULL);
	bool					SendCommand(AS608_COMMAND command, AS608_BUFFER_NUMBER buffer = AS608_BUFFER_NUMBER_1);
//---------------------------------------------------------------------------------//
	//現行程式，提取來自UPCHAR封包的資料，儲存資料大小128*6
	bool					ReadData(unsigned int &size, BYTE& flag, BYTE* data);
	bool					SendData(BYTE* data);
//---------------------------------------------------------------------------------//
	//deprecated，以下程式碼直接將UPCHAR獲取的封包原封不動的儲存與發送，儲存資料大小139*6
	//如果SendData與ReadData問題太多，可以換成用這邊的
	bool					ReadPacket(BYTE* data);//接收封包
	bool					SendPacket(char* data);//傳送封包
	bool					SendPacket(BYTE* data);//傳送封包
};