#pragma once
#include "Fingerprint_Base.h"

#ifndef WINBIO_DISABLE
#include <winbio.h>
#pragma comment(lib, "winbio.lib")
class Fingerprint_PQIFPS_Reader : public Fingerprint_Base {
public:
	Fingerprint_PQIFPS_Reader();
	~Fingerprint_PQIFPS_Reader();
	//---------------------------------------------------------------------------------//
	bool                    InitialFPSDevice()	override;
	//---------------------------------------------------------------------------------//
	bool                    ConnectDevice()		override;
	bool                    Disconnected()		override;
	bool                    CheckConnected()	override;
	bool                    CheckFPSDeviceExisted() override;
	bool                    WaitForDataInQuene();
	//---------------------------------------------------------------------------------//
	bool                    RecordFingerprint(LPVOID pParam) override;
	bool                    MatchFingerprint(LPVOID pParam)  override;
	bool                    FindFingerprint(LPVOID pParam)   override;
	bool                    DelFingerprintInDB(std::vector<TUserNode> UserList) override;
	//---------------------------------------------------------------------------------//
	bool                    GetBiometricIdentity(BYTE* pBuffer, DWORD &nBufferSize) override;
	//---------------------------------------------------------------------------------//
	FPS_DEVICE              GetDeviceType() const { return FPS_DEVICE_PQIFPS_READER; }

private:
	CWnd                   *m_pParentWnd;
	HWND                    m_hParentWnd;
	USER_FINGERPRINT_MODE   m_UserFingerprintMode;

	WINBIO_SESSION_HANDLE   m_WinbioSession;
	WINBIO_UNIT_ID	        m_WinbioUnitId;
	PWINBIO_ASYNC_RESULT    m_WinbioAsyncConfig;
	WINBIO_BIOMETRIC_SUBTYPE m_WinbioSubtype;
	WINBIO_OPERATION_TYPE   m_WinbioOperationType;
	WINBIO_OPERATION_TYPE   m_WinbioNextOperationType; //  callback 決定的下一個狀態
	SIZE_T                  m_WinbioRecordCount;
	SIZE_T                  m_WinbioRecordTimes;
	bool                    m_WinbioFinished;
	bool                    m_WinbioStateChanged;      //  callback 是否已更新下一個狀態
	bool                    m_WinbioCancelRequested;   //  記錄是否取消
	DWORD                   m_NextOperationTick;       //  用於取代 callback 內的 Sleep

	LPTSTR		            ConvertErrorCodeToString(__in HRESULT ErrorCode);
	LPTSTR		            ConvertRejectDetailToString(WINBIO_REJECT_DETAIL RejectDetail);
	LPTSTR                  GetWinBioOperationTypeName(WINBIO_OPERATION_TYPE Type);
	LPTSTR                  GetWinBioOperationHint(WINBIO_OPERATION_TYPE Type);
	CString                 LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	bool                    GetAvailableSubtype(WINBIO_UNIT_ID unitId, WINBIO_BIOMETRIC_SUBTYPE &Subtype);
	bool                    GetAvailableSubtype(PWINBIO_BIOMETRIC_SUBTYPE SubTypeList, SIZE_T SubTypeCount, WINBIO_BIOMETRIC_SUBTYPE &Subtype);
	HRESULT                 GetSystemPoolIdentity(PWINBIO_IDENTITY Identity);
	static void CALLBACK    WinBioCallback(__in PWINBIO_ASYNC_RESULT AsyncResult);
	void                    OnAsyncCompletion(PWINBIO_ASYNC_RESULT AsyncResult);
	void                    OnAsyncCompletionRecord(PWINBIO_ASYNC_RESULT AsyncResult);
	void                    OnAsyncCompletionFind(PWINBIO_ASYNC_RESULT AsyncResult);
	bool                    PostMessageStringToParentWnd(UINT Msg, WPARAM wParam, CString str);

	//---------------------------------------------------------------------------------//
	//  狀態機執行函式：Record/Find 負責執行目前狀態
	HRESULT                 ExecuteCurrentRecordState();
	HRESULT                 ExecuteCurrentFindState();
	void                    SetNextOperation(WINBIO_OPERATION_TYPE NextOperation, DWORD DelayMs = 0);
	void                    NotifyOperationUi(WINBIO_OPERATION_TYPE WinBioOperation);
};
#endif // Winbio_disable


