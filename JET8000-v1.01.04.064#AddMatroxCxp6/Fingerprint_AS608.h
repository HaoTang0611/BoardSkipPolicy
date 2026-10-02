#pragma once
#include "Fingerprint_Base.h"
class Fingerprint_AS608 : public Fingerprint_Base {
public:
	Fingerprint_AS608();
	~Fingerprint_AS608() ;
	//---------------------------------------------------------------------------------//
	bool                    InitialFPSDevice()	override;
	//---------------------------------------------------------------------------------//
	bool                    ConnectDevice()		override;
	bool                    Disconnected()		override;
	bool                    CheckConnected()	override;
	bool                    CheckFPSDeviceExisted() override;
	bool                    DelFingerprintInDB(std::vector<TUserNode> UserList) override;
	bool                    WaitForDataInQuene();
	//---------------------------------------------------------------------------------//
	bool                    RecordFingerprint(LPVOID pParam) override;
	bool                    MatchFingerprint(LPVOID pParam)  override;
	bool                    FindFingerprint(LPVOID pParam)   override;
	//---------------------------------------------------------------------------------//
	bool                    GetBiometricIdentity(BYTE* pBuffer, DWORD &nBufferSize) override;
	bool                    GetFeatureIdentity(BYTE* pBuffer, DWORD &nBufferSize);
	//---------------------------------------------------------------------------------//
	bool					ExecuteRecordFingerprint(LPVOID pParam);	//簡易註冊，讀取一次指紋
	bool					ExecuteRecordFingerprint_V2(LPVOID pParam);	//嚴格註冊，讀取兩次指紋並合併特徵
	bool					ExecuteMatchFingerprint(LPVOID pParam);		//指紋驗證
	bool					ExecuteFindFingerprint(LPVOID pParam);		//指紋尋找
	bool					ExecuteFindFingerprintLoop(LPVOID pParam);	//指紋尋找-持續
	//---------------------------------------------------------------------------------//
	FPS_DEVICE              GetDeviceType()  const  { return FPS_DEVICE_AS608; }
	
	
	//---------------------------------------------------------------------------------//
private:
	CJetSerial              m_RS232COM;						//RS232	
	int                     m_BaudRate;
	bool                    m_DeviceConnected;
	CString                 m_DevicePort;
	//---------------------------------------------------------------------------------//
	//特徵資料大小為 128*6 = 768，開發文件中描述Buffer大小是512，多出來的256目前猜測是包頭包尾。
	static constexpr int    m_FeatureSize = 768;
	BYTE                    m_Feature[m_FeatureSize];
	//---------------------------------------------------------------------------------//
	void                    SetDeviceConnected(bool val) { m_DeviceConnected = val; }
	void                    SetDevicePort(LPCTSTR val) { m_DevicePort = val; }
	LPCTSTR                 GetDevicePort() const { return m_DevicePort; }
	void					SetCOMPort(unsigned int nPort);	//設定COM Port
	//---------------------------------------------------------------------------------//
	//發送命令與接受回應
	bool                    SendCommand(AS608_COMMAND command, AS608_BUFFER_NUMBER buffer = AS608_BUFFER_NUMBER_1);
	AS608_STATUS			ReadResponse(AS608_COMMAND command);
	AS608_STATUS            ReadResponse(unsigned int &size, BYTE& flag, BYTE* data);
	//---------------------------------------------------------------------------------//
	//現行程式，提取來自UPCHAR封包的資料，儲存資料大小128*6
	bool					ReadData(unsigned int &size, BYTE& flag, BYTE* data);
	bool					SendData(BYTE* data);
};
