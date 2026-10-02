#pragma once
//#include "FingerprintDefine.h"
class Fingerprint_Base {
public:
	virtual ~Fingerprint_Base() = default;
	
	virtual bool            ConnectDevice()	 = 0;
	virtual bool            Disconnected() = 0;
	virtual bool            CheckConnected()  = 0;
	virtual bool            CheckFPSDeviceExisted() = 0;
	virtual bool            InitialFPSDevice() = 0;
	virtual bool            RecordFingerprint(LPVOID pParam) = 0;
	virtual bool            MatchFingerprint(LPVOID pParam) = 0;
	virtual bool            FindFingerprint(LPVOID pParam) = 0;
	virtual bool            GetBiometricIdentity(BYTE* pBuffer, DWORD &nBufferSize) = 0;
	virtual bool            DelFingerprintInDB(std::vector<TUserNode> UserList) = 0;
	//---------------------------------------------------------------------------------//
	virtual FPS_DEVICE    	GetDeviceType() const = 0;
	//---------------------------------------------------------------------------------//
	CString	                GetErrorString() const;
	void				    SetErrorString(CString str);

	CString					m_ErrorString;
	CString                 m_DeviceName;
	//---------------------------------------------------------------------------------//
	int                     m_FPSDeviceWaitDataCount;		//等待時間
	DWORD                   m_FPSDeviceWaitDataDwellTime;	//等待資料的延遲時間-ms
	DWORD                   m_FPSDeviceReadDataDelayTime;	//讀取資料前延遲時間-ms
	int                     m_FPSDeviceBaudRate;			//鮑率
	//---------------------------------------------------------------------------------//
	bool                    LoadINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pDefault, TCHAR *pString, int StringSize, LPCTSTR pfilename, bool IsCheckLens, CString &Error);
	bool                    SaveINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pString, LPCTSTR pfilename, CString &Error);
	bool                    LoadFPSINIFile();
	bool                    SaveFPSINIFile();
	//---------------------------------------------------------------------------------//
};
