#include "stdafx.h"
#include "jet8000.h"
#include "FingerprintDefine.h"

#ifndef WINBIO_DISABLE
#include "Fingerprint_PQIFPS_Reader.h"
#include "FingerprintWnd.h"
using namespace std;
//---------------------------------------------------------------------------------//
Fingerprint_PQIFPS_Reader::Fingerprint_PQIFPS_Reader()
{
	InitialFPSDevice();
	m_pParentWnd = NULL;
	m_hParentWnd = NULL;

}
//---------------------------------------------------------------------------------//
Fingerprint_PQIFPS_Reader::~Fingerprint_PQIFPS_Reader()
{
	Disconnected();
}
//---------------------------------------------------------------------------------//
bool Fingerprint_PQIFPS_Reader::InitialFPSDevice()
{
	m_DeviceName = _T("PQI Fingerprint Security Feader");
	m_FPSDeviceWaitDataCount = 50;//等待次數
	m_FPSDeviceWaitDataDwellTime = 50;//等待資料的延遲時間-ms
	m_FPSDeviceReadDataDelayTime = 50;//讀取資料前延遲時間-ms
									  //LoadFPSINIFile();
									  //SaveFPSINIFile();

	m_WinbioSession = NULL;
	m_WinbioUnitId = 0;
	m_WinbioAsyncConfig = NULL;
	m_WinbioOperationType = WINBIO_OPERATION_NONE;
	m_WinbioNextOperationType = WINBIO_OPERATION_NONE;
	m_WinbioStateChanged = false;
	m_WinbioFinished = false;
	m_WinbioCancelRequested = false;
	m_NextOperationTick = 0;
	m_ErrorString = _T("");
	return true;
}
//---------------------------------------------------------------------------------//
bool Fingerprint_PQIFPS_Reader::ConnectDevice()
{
	if (m_WinbioSession != NULL) { return true; }
	m_ErrorString = _T("");
	//HRESULT hr = WinBioOpenSession(
	//	WINBIO_TYPE_FINGERPRINT,
	//	WINBIO_POOL_SYSTEM,
	//	WINBIO_FLAG_DEFAULT,
	//	nullptr,	0,
	//	WINBIO_DB_DEFAULT,
	//	&m_WinbioSession
	//);
	HRESULT hr = WinBioAsyncOpenSession(
		WINBIO_TYPE_FINGERPRINT,      // Factor
		WINBIO_POOL_SYSTEM,           // PoolType
		WINBIO_FLAG_DEFAULT,          // Flags 
		NULL,                         // UnitArray
		0,                            // UnitCount
		WINBIO_DB_DEFAULT,            // DatabaseId
		WINBIO_ASYNC_NOTIFY_CALLBACK, // NotificationMethod (使用回呼函式)
		NULL,                         // TargetWindow (Callback 模式下此項無效，傳 NULL)
		0,                            // MessageCode (Callback 模式下此項無效，傳 0)
		WinBioCallback,               // CallbackRoutine (指向你的靜態回呼函式)
		this,                         // UserData (傳入類別指標，方便 Callback 內存取成員)
		FALSE,                        // AsynchronousOpen (設為 FALSE 讓 OpenSession 同步完成)
		&m_WinbioSession              // SessionHandle
	);
	if (FAILED(hr)) {
		m_WinbioSession = NULL;
		m_ErrorString = ConvertErrorCodeToString(hr);
		return false;
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool Fingerprint_PQIFPS_Reader::Disconnected()
{
	m_pParentWnd = NULL;
	if (m_WinbioSession == NULL) { return true; }
	HRESULT hr = WinBioCloseSession(m_WinbioSession);
	if (FAILED(hr))
	{
		m_ErrorString = ConvertErrorCodeToString(hr);
		return false;
	}
	//非同步，要等到callback 裡面完成close 才去清掉m_WinbioSession
	//m_WinbioSession = NULL;
	return true;
}
//---------------------------------------------------------------------------------//
bool Fingerprint_PQIFPS_Reader::CheckConnected()
{
	return true;
}
//---------------------------------------------------------------------------------//
bool Fingerprint_PQIFPS_Reader::CheckFPSDeviceExisted()
{
	HRESULT hr = S_OK;
	WINBIO_UNIT_SCHEMA* pUnits = NULL;
	SIZE_T cUnits = 0;
	hr = WinBioEnumBiometricUnits(
		WINBIO_TYPE_FINGERPRINT, // 列舉指紋辨識器
		&pUnits,
		&cUnits
	);
	if (SUCCEEDED(hr) && cUnits > 0) {
		m_WinbioUnitId = pUnits[0].UnitId;
		WinBioFree(pUnits);
		return true;
	}
	else if (FAILED(hr)) {
		m_ErrorString = ConvertErrorCodeToString(hr);
	}
	if (pUnits != NULL) {
		WinBioFree(pUnits);
	}
	return false;
}
//---------------------------------------------------------------------------------//
bool Fingerprint_PQIFPS_Reader::WaitForDataInQuene()
{
	return true;
}
//---------------------------------------------------------------------------------//
//  callback 只決定下一個狀態，真正執行 WinBio operation 由 RecordFingerprint 內的 while loop 觸發
bool Fingerprint_PQIFPS_Reader::RecordFingerprint(LPVOID pParam)
{
	if (pParam == NULL) { return false; }
	if (m_WinbioSession == NULL) { return false; }
	HRESULT hr = S_OK;
	CFingerprintWnd* pWnd = static_cast<CFingerprintWnd*>(pParam);


	m_pParentWnd = pWnd;
	m_hParentWnd = pWnd->GetSafeHwnd();
	m_UserFingerprintMode = USER_FINGERPRINT_MODE_ENROLL;
	m_WinbioRecordCount = 0;
	//m_WinbioRecordTimes = 10;//掃描次數 deprecated
	m_WinbioFinished = false;
	m_WinbioCancelRequested = false;
	m_WinbioStateChanged = false;
	m_WinbioOperationType = WINBIO_OPERATION_NONE;
	m_WinbioNextOperationType = WINBIO_OPERATION_NONE;
	m_NextOperationTick = 0;
	m_ErrorString = _T("");

	//m_WinbioOperationType = WINBIO_OPERATION_LOCATE_SENSOR;
	SetNextOperation(WINBIO_OPERATION_ENUM_ENROLLMENTS);

	while (m_WinbioFinished == false) {
		if (true == pWnd->CheckWndIsCancel()) {
			m_WinbioCancelRequested = true;
			m_ErrorString = _T("Canceled");
			Disconnected();
		}

		if (m_WinbioStateChanged == true) {
			if (GetTickCount() >= m_NextOperationTick) {
				m_WinbioOperationType = m_WinbioNextOperationType;
				m_WinbioStateChanged = false;

				if (m_WinbioOperationType == WINBIO_OPERATION_NONE) { break; }

				NotifyOperationUi(m_WinbioOperationType);

				hr = ExecuteCurrentRecordState();
				if (FAILED(hr)) {
					m_ErrorString = ConvertErrorCodeToString(hr);
					Disconnected();
				}
			}
		}

		if (m_WinbioOperationType == WINBIO_OPERATION_NONE && m_WinbioSession == NULL) { break; }
		Sleep(20);
	}
	m_pParentWnd = NULL;
	return m_WinbioFinished;
}
//---------------------------------------------------------------------------------//
bool Fingerprint_PQIFPS_Reader::MatchFingerprint(LPVOID pParam)
{
	return false;
}
//---------------------------------------------------------------------------------//
//  callback 只決定下一個狀態，真正執行 WinBio operation 由 FindFingerprint 內的 while loop 觸發
bool Fingerprint_PQIFPS_Reader::FindFingerprint(LPVOID pParam)
{
	if (pParam == NULL) { return false; }
	if (m_WinbioSession == NULL) { return false; }

	//CFingerprintWnd* pThis = (CFingerprintWnd*)pParam;
	CFingerprintWnd* pWnd = static_cast<CFingerprintWnd*>(pParam);
	HRESULT hr = S_OK;

	m_pParentWnd = pWnd;
	m_hParentWnd = pWnd->GetSafeHwnd();
	m_UserFingerprintMode = USER_FINGERPRINT_MODE_FIND;
	m_WinbioFinished = false;
	m_WinbioCancelRequested = false;
	m_WinbioStateChanged = false;
	m_WinbioOperationType = WINBIO_OPERATION_NONE;
	m_WinbioNextOperationType = WINBIO_OPERATION_NONE;
	m_NextOperationTick = 0;
	m_ErrorString = _T("");

	SetNextOperation(WINBIO_OPERATION_IDENTIFY);

	while (m_WinbioFinished == false) {
		if (true == pWnd->CheckWndIsCancel()) {
			m_WinbioCancelRequested = true;
			m_ErrorString = _T("Canceled");
			Disconnected();
			break;
		}

		if (m_WinbioStateChanged == true) {
			if (GetTickCount() >= m_NextOperationTick) {
				m_WinbioOperationType = m_WinbioNextOperationType;
				m_WinbioStateChanged = false;

				if (m_WinbioOperationType == WINBIO_OPERATION_NONE) { break; }

				NotifyOperationUi(m_WinbioOperationType);

				hr = ExecuteCurrentFindState();
				if (FAILED(hr)) {
					m_ErrorString = ConvertErrorCodeToString(hr);
					Disconnected();
				}
			}
		}

		if (m_WinbioOperationType == WINBIO_OPERATION_NONE && m_WinbioSession == NULL) { break; }
		Sleep(20);
	}
	if (m_WinbioFinished == false) { return false; }

	CString str = GetWinBioOperationTypeName(m_WinbioOperationType);
	str = LoadMultiLanguageString(str, str);
	PostMessageStringToParentWnd(MSG_FINGERPRINT_WND, WPARAM_FINGERPRINT_SHOW_OPERATION_STRING, str);
	hr = S_OK; str = ConvertErrorCodeToString(hr);
	PostMessageStringToParentWnd(MSG_FINGERPRINT_WND, WPARAM_FINGERPRINT_SHOW_STATE_STRING, str);

	std::vector<TUserNode> UserList;
	TUserNode UserNode;
	UserList = pWnd->GetUserList();
	if (UserList.empty()) {
		CString filename = AOIDataCollect.GetUserFilename();
		AOIDataCollect.ReadUserFile(filename, UserList);
	}

	//比對指紋
	bool bFind = false;
	for (int i = 0; i < UserList.size(); i++) {
		UserNode = UserList[i];
		if (false == UserNode.wFingerEnable) { continue; }
		if (UserNode.wFinger[0] != m_WinbioSubtype) { continue; }
		pWnd->SetFingerPrintUser(UserNode);
		bFind = true;
	}
	if (bFind == false) {
		m_ErrorString = _T("Can't Find the Fingerprint Owner.");
	}
	m_pParentWnd = NULL;
	return bFind;
}
//---------------------------------------------------------------------------------//
bool Fingerprint_PQIFPS_Reader::DelFingerprintInDB(std::vector<TUserNode> UserList)
{
	TUserNode UserNode;
	size_t i;

	HRESULT hr;
	WINBIO_IDENTITY identity = {};
	WINBIO_UNIT_SCHEMA* pUnits = NULL;
	WINBIO_BIOMETRIC_SUBTYPE SubType;
	SIZE_T cUnits = 0;

	hr = WinBioEnumBiometricUnits(
		WINBIO_TYPE_FINGERPRINT, // 列舉指紋辨識器
		&pUnits,
		&cUnits
	);
	if (FAILED(hr)) {
		m_ErrorString = ConvertErrorCodeToString(hr);
		return false;
	}
	m_WinbioUnitId = pUnits[0].UnitId;
	WinBioFree(pUnits);

	hr = WinBioOpenSession(
		WINBIO_TYPE_FINGERPRINT,
		WINBIO_POOL_SYSTEM,
		WINBIO_FLAG_DEFAULT,
		nullptr, 0,
		WINBIO_DB_DEFAULT,
		&m_WinbioSession
	);
	if (FAILED(hr)) {
		m_ErrorString = ConvertErrorCodeToString(hr);
		return false;
	}
	hr = GetSystemPoolIdentity(&identity);
	if (FAILED(hr)) {
		m_ErrorString = ConvertErrorCodeToString(hr);
		return false;
	}
	for (i = 0; i < UserList.size(); i++) {
		UserNode = UserList[i];
		if (UserNode.wFingerEnable == false) { continue; }
		SubType = UserNode.wFinger[0];
		hr = WinBioDeleteTemplate(
			m_WinbioSession,
			m_WinbioUnitId,
			&identity,
			SubType
		);
		if (FAILED(hr)) {
			m_ErrorString = ConvertErrorCodeToString(hr);
			return false;
		}
	}

	return true;
}
//---------------------------------------------------------------------------------//
bool Fingerprint_PQIFPS_Reader::GetBiometricIdentity(BYTE * pBuffer, DWORD & nBufferSize)
{
	nBufferSize = 1;
	pBuffer[0] = (BYTE)m_WinbioSubtype;
	return true;
}
//---------------------------------------------------------------------------------//
LPTSTR Fingerprint_PQIFPS_Reader::ConvertErrorCodeToString(HRESULT ErrorCode)
{
	TCHAR *messageBuffer = NULL;
	DWORD messageLength = 0;

	std::vector<TCHAR> systemPath;
	UINT systemPathSize = 0;
	systemPathSize = GetSystemWindowsDirectory(NULL, 0);
	systemPath.resize(systemPathSize);
	systemPathSize = GetSystemWindowsDirectory((LPTSTR)&systemPath[0], systemPathSize);

	CString libraryPath = &systemPath[0];
	libraryPath += _T("\\system32\\winbio.dll");

	HMODULE winbioLibrary = NULL;
	winbioLibrary = LoadLibraryEx(
		libraryPath.GetBuffer(),
		NULL,
		LOAD_LIBRARY_AS_DATAFILE |
		LOAD_LIBRARY_AS_IMAGE_RESOURCE
	);
	if (winbioLibrary != NULL)
	{
		messageLength = FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_HMODULE |
			FORMAT_MESSAGE_FROM_SYSTEM,
			winbioLibrary,
			ErrorCode,
			0,                      // LANGID
			(LPTSTR)&messageBuffer,
			0,                      // arg count
			NULL                    // arg array
		);
		if (messageLength > 0)
		{
			// success
			messageBuffer[messageLength] = _T('\0');
		}
		FreeLibrary(winbioLibrary);
		winbioLibrary = NULL;
	}

	if (messageBuffer == NULL)
	{
		messageLength = 100;    // "0x" + "%08x"
		messageBuffer = (TCHAR*)LocalAlloc(LPTR, (messageLength + 1) * sizeof(TCHAR));
		if (messageBuffer != NULL)
		{
			_stprintf_s(messageBuffer, messageLength, _T("0x%08x"), ErrorCode);
		}
	}

	// Caller must release buffer with LocalFree()
	return messageBuffer;
}
//---------------------------------------------------------------------------------//
LPTSTR Fingerprint_PQIFPS_Reader::ConvertRejectDetailToString(WINBIO_REJECT_DETAIL RejectDetail)
{
	const TCHAR* pszName = NULL;

	switch (RejectDetail) {
	case 0:                                   pszName = _T("Success"); break;
	case WINBIO_FP_TOO_HIGH:                  pszName = _T("Scan your fingerprint a little lower."); break;
	case WINBIO_FP_TOO_LOW:                   pszName = _T("Scan your fingerprint a little higher."); break;
	case WINBIO_FP_TOO_LEFT:                  pszName = _T("Scan your fingerprint more to the right."); break;
	case WINBIO_FP_TOO_RIGHT:                 pszName = _T("Scan your fingerprint more to the left."); break;
	case WINBIO_FP_TOO_FAST:                  pszName = _T("Scan your fingerprint more slowly."); break;
	case WINBIO_FP_TOO_SLOW:                  pszName = _T("Scan your fingerprint more quickly."); break;
	case WINBIO_FP_POOR_QUALITY:              pszName = _T("The quality of the fingerprint scan was not sufficient to make a match. Check to make sure the sensor is clean."); break;
	case WINBIO_FP_TOO_SKEWED:                pszName = _T("Hold your finger flat and straight when scanning your fingerprint."); break;
	case WINBIO_FP_TOO_SHORT:                 pszName = _T("Use a longer stroke when scanning your fingerprint."); break;
	case WINBIO_FP_MERGE_FAILURE:             pszName = _T("Unable to merge samples into a single enrollment. Try to repeat the enrollment procedure from the beginning."); break;
	default:                                  pszName = _T("Reason for failure couldn't be diagnosed."); break;
	}
	size_t len = (_tcslen(pszName) + 1) * sizeof(TCHAR);
	LPTSTR pszResult = (LPTSTR)LocalAlloc(LPTR, len);
	if (pszResult != NULL) {
		_tcscpy_s(pszResult, len / sizeof(TCHAR), pszName);
	}
	return pszResult;
}
//---------------------------------------------------------------------------------//
LPTSTR Fingerprint_PQIFPS_Reader::GetWinBioOperationTypeName(WINBIO_OPERATION_TYPE Type)
{
	const TCHAR* pszName = NULL;

	switch (Type) {
	case WINBIO_OPERATION_NONE:                 pszName = _T("None"); break;
	case WINBIO_OPERATION_OPEN:                 pszName = _T("Open Session"); break;
	case WINBIO_OPERATION_CLOSE:                pszName = _T("Close Session"); break;
	case WINBIO_OPERATION_VERIFY:               pszName = _T("Verify"); break;
	case WINBIO_OPERATION_IDENTIFY:             pszName = _T("Identify"); break;
	case WINBIO_OPERATION_LOCATE_SENSOR:        pszName = _T("Locate Sensor"); break;
	case WINBIO_OPERATION_ENROLL_BEGIN:         pszName = _T("Enroll Begin"); break;
	case WINBIO_OPERATION_ENROLL_CAPTURE:       pszName = _T("Enroll Capture"); break;
	case WINBIO_OPERATION_ENROLL_COMMIT:        pszName = _T("Enroll Commit"); break;
	case WINBIO_OPERATION_ENROLL_DISCARD:       pszName = _T("Enroll Discard"); break;
	case WINBIO_OPERATION_ENUM_ENROLLMENTS:     pszName = _T("Enum Enrollments"); break;
	case WINBIO_OPERATION_DELETE_TEMPLATE:      pszName = _T("Delete Enrollment"); break;
	case WINBIO_OPERATION_CAPTURE_SAMPLE:       pszName = _T("Capture Sample"); break;
	case WINBIO_OPERATION_GET_PROPERTY:         pszName = _T("Get Property"); break;
	case WINBIO_OPERATION_SET_PROPERTY:         pszName = _T("Set Property"); break;
	default:                                    pszName = _T("Unknown Operation"); break;
	}
	size_t len = (_tcslen(pszName) + 1) * sizeof(TCHAR);
	LPTSTR pszResult = (LPTSTR)LocalAlloc(LPTR, len);
	if (pszResult != NULL) {
		_tcscpy_s(pszResult, len / sizeof(TCHAR), pszName);
	}
	return pszResult;
}
//---------------------------------------------------------------------------------//
LPTSTR Fingerprint_PQIFPS_Reader::GetWinBioOperationHint(WINBIO_OPERATION_TYPE Type)
{
	const TCHAR* pszHint = NULL;

	switch (Type) {
	case WINBIO_OPERATION_IDENTIFY:        pszHint = _T("Please place your finger"); break;
	case WINBIO_OPERATION_LOCATE_SENSOR:   pszHint = _T("Please place your finger"); break;
	case WINBIO_OPERATION_ENROLL_BEGIN:    pszHint = _T("Starting enrollment process..."); break;
	case WINBIO_OPERATION_ENROLL_CAPTURE:  pszHint = _T("Please place your finger"); break;
	case WINBIO_OPERATION_ENROLL_COMMIT:   pszHint = _T("Saving fingerprint to the system database..."); break;
	case WINBIO_OPERATION_ENUM_ENROLLMENTS:pszHint = _T("Checking database..."); break;
	default:                               pszHint = _T("Undefined"); break;
	}

	size_t len = (_tcslen(pszHint) + 1) * sizeof(TCHAR);
	LPTSTR pszResult = (LPTSTR)LocalAlloc(LPTR, len);
	if (pszResult != NULL) {
		_tcscpy_s(pszResult, len / sizeof(TCHAR), pszHint);
	}
	return pszResult;
}
//---------------------------------------------------------------------------------//
CString Fingerprint_PQIFPS_Reader::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CFingerprintWnd* pWnd = (CFingerprintWnd*)m_pParentWnd;
	return pWnd->LoadMultiLanguageString(KeyName, Default);
}
//---------------------------------------------------------------------------------//
bool Fingerprint_PQIFPS_Reader::GetAvailableSubtype(WINBIO_UNIT_ID unitId, WINBIO_BIOMETRIC_SUBTYPE &Subtype)
{
	WINBIO_IDENTITY identity = {};
	PWINBIO_BIOMETRIC_SUBTYPE subTypeList = NULL;
	WINBIO_BIOMETRIC_SUBTYPE EnrolledSubType;
	size_t subTypeCount = 0;
	size_t i, j;
	HRESULT hr;
	hr = GetSystemPoolIdentity(&identity);
	if (!SUCCEEDED(hr)) {
		m_ErrorString = ConvertErrorCodeToString(hr);
		return false;
	}
	hr = WinBioEnumEnrollments(
		m_WinbioSession,
		unitId,
		&identity,
		NULL,   // 非同步模式下，這兩個參數必須為 NULL
		NULL    // 因為結果會傳到 Callback 的 AsyncResult 裡
				//&subTypeList,
				//&subTypeCount
	);
	if (!SUCCEEDED(hr)) {
		m_ErrorString = ConvertErrorCodeToString(hr);
		return false;
	}
	bool bUsed;
	for (i = 0x01; i <= WINBIO_ANSI_381_POS_LH_LITTLE_FINGER; i++) {
		Subtype = (WINBIO_BIOMETRIC_SUBTYPE)i;
		bUsed = false;
		for (j = 0; j < subTypeCount; j++) {
			EnrolledSubType = subTypeList[j];
			if (Subtype == EnrolledSubType) {
				bUsed = true;
				break;
			}
		}
		if (bUsed == false) { break; }
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool Fingerprint_PQIFPS_Reader::GetAvailableSubtype(PWINBIO_BIOMETRIC_SUBTYPE SubTypeList, SIZE_T SubTypeCount, WINBIO_BIOMETRIC_SUBTYPE &Subtype)
{

	size_t i, j;
	bool bUsed;
	WINBIO_BIOMETRIC_SUBTYPE EnrolledSubType;
	if (SubTypeList == NULL || SubTypeCount == 0) {
		Subtype = WINBIO_ANSI_381_POS_RH_THUMB;
		return true;
	}
	for (i = WINBIO_ANSI_381_POS_RH_THUMB; i <= WINBIO_ANSI_381_POS_LH_LITTLE_FINGER; i++) {
		//for (i = WINBIO_ANSI_381_POS_RH_THUMB; i <= WINBIO_ANSI_381_POS_RH_THUMB; i++) {// test fulled database
		Subtype = (WINBIO_BIOMETRIC_SUBTYPE)i;
		bUsed = false;
		for (j = 0; j < SubTypeCount; j++) {
			EnrolledSubType = SubTypeList[j];
			if (Subtype == EnrolledSubType) {
				bUsed = true;
				break;
			}
		}
		if (bUsed == false) { break; }
	}
	if (bUsed == true) { return false; }//所有指紋槽都被註冊
	return true;
}
//---------------------------------------------------------------------------------//
HRESULT Fingerprint_PQIFPS_Reader::GetSystemPoolIdentity(PWINBIO_IDENTITY Identity)
{
	HRESULT hr = S_OK;
	HANDLE tokenHandle = NULL;
	DWORD bytesReturned = 0;

	// 定義一個足夠大的 Buffer 來存放 Token User 資訊
	struct {
		TOKEN_USER tokenUser;
		BYTE buffer[SECURITY_MAX_SID_SIZE];
	} tokenInfoBuffer;

	ZeroMemory(Identity, sizeof(WINBIO_IDENTITY));
	Identity->Type = WINBIO_ID_TYPE_NULL;

	// 1. 打開當前進程的 Token
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_READ, &tokenHandle)) {
		return HRESULT_FROM_WIN32(GetLastError());
	}

	// 2. 取得使用者資訊 (SID)
	if (GetTokenInformation(tokenHandle, TokenUser, &tokenInfoBuffer, sizeof(tokenInfoBuffer), &bytesReturned)) {
		Identity->Type = WINBIO_ID_TYPE_SID;
		// 複製 SID 到 Identity 結構中
		Identity->Value.AccountSid.Size = GetLengthSid(tokenInfoBuffer.tokenUser.User.Sid);
		CopySid(
			SECURITY_MAX_SID_SIZE,
			Identity->Value.AccountSid.Data,
			tokenInfoBuffer.tokenUser.User.Sid
		);
	}
	else {
		hr = HRESULT_FROM_WIN32(GetLastError());
	}

	if (tokenHandle) CloseHandle(tokenHandle);
	return hr;
}
//---------------------------------------------------------------------------------//
void Fingerprint_PQIFPS_Reader::WinBioCallback(PWINBIO_ASYNC_RESULT AsyncResult)
{
	if (AsyncResult == NULL) return;
	CString str;
	HRESULT hr = AsyncResult->ApiStatus;
	WINBIO_OPERATION_TYPE WinBioOperation = AsyncResult->Operation;
	Fingerprint_PQIFPS_Reader* pThis = static_cast<Fingerprint_PQIFPS_Reader*>(AsyncResult->UserData);
	if (pThis == nullptr)
	{
		WinBioFree(AsyncResult);
		return;
	}
	if (hr == WINBIO_E_BAD_CAPTURE) {
		WINBIO_REJECT_DETAIL RejectDetail = 0;
		switch (WinBioOperation)
		{
		case WINBIO_OPERATION_IDENTIFY:
			RejectDetail = AsyncResult->Parameters.Identify.RejectDetail;
			str = pThis->ConvertRejectDetailToString(RejectDetail);
			str = pThis->LoadMultiLanguageString(str, str);
			break;
		case WINBIO_OPERATION_ENROLL_CAPTURE:
			RejectDetail = AsyncResult->Parameters.EnrollCapture.RejectDetail;
			str = pThis->ConvertRejectDetailToString(RejectDetail);
			str = pThis->LoadMultiLanguageString(str, str);
			break;
		default:
			break;
		}
	}
	else {
		str = pThis->ConvertErrorCodeToString(hr);
	}
	if (!str.IsEmpty()) {
		pThis->PostMessageStringToParentWnd(MSG_FINGERPRINT_WND, WPARAM_FINGERPRINT_SHOW_STATE_STRING, str);
	}
	pThis->OnAsyncCompletion(AsyncResult);

	WinBioFree(AsyncResult);
}
//---------------------------------------------------------------------------------//
//  callback 只負責決定下一個狀態，不在 callback 內直接呼叫 WinBio operation
void Fingerprint_PQIFPS_Reader::OnAsyncCompletion(PWINBIO_ASYNC_RESULT AsyncResult)
{
	if (AsyncResult == NULL) return;
	WINBIO_OPERATION_TYPE WinBioOperation = AsyncResult->Operation;
	Fingerprint_PQIFPS_Reader* pThis = static_cast<Fingerprint_PQIFPS_Reader*>(AsyncResult->UserData);
	if (pThis == nullptr) { return; }
	if (WinBioOperation == WINBIO_OPERATION_CLOSE)
	{
		pThis->m_WinbioOperationType = WINBIO_OPERATION_NONE;
		pThis->m_WinbioNextOperationType = WINBIO_OPERATION_NONE;
		pThis->m_WinbioStateChanged = false;
		pThis->m_WinbioSession = NULL;
		return;
	}

	switch (pThis->m_UserFingerprintMode)
	{
	case USER_FINGERPRINT_MODE_ENROLL:
		pThis->OnAsyncCompletionRecord(AsyncResult);
		break;
	case USER_FINGERPRINT_MODE_FIND:
		pThis->OnAsyncCompletionFind(AsyncResult);
		break;
	default:
		break;
	}
}
//---------------------------------------------------------------------------------//
//  callback 只決定下一個狀態，真正執行由 RecordFingerprint 內的 while loop 呼叫 ExecuteCurrentRecordState
void Fingerprint_PQIFPS_Reader::OnAsyncCompletionRecord(PWINBIO_ASYNC_RESULT AsyncResult)
{
	HRESULT hr = AsyncResult->ApiStatus;
	WINBIO_OPERATION_TYPE WinBioOperation = AsyncResult->Operation;
	Fingerprint_PQIFPS_Reader* pThis = static_cast<Fingerprint_PQIFPS_Reader*>(AsyncResult->UserData);
	if (pThis == nullptr) { return; }

	if (AsyncResult->UnitId != 0) {
		pThis->m_WinbioUnitId = AsyncResult->UnitId;
	}

	switch (AsyncResult->Operation)
	{
	case WINBIO_OPERATION_LOCATE_SENSOR:
		if (SUCCEEDED(hr)) {
			WinBioOperation = WINBIO_OPERATION_ENUM_ENROLLMENTS;
			pThis->SetNextOperation(WinBioOperation);
		}
		else {
			WinBioOperation = WINBIO_OPERATION_CLOSE;
			pThis->m_ErrorString = pThis->ConvertErrorCodeToString(hr);
			pThis->SetNextOperation(WinBioOperation);
		}
		break;
	case WINBIO_OPERATION_ENROLL_BEGIN:
		if (SUCCEEDED(hr)) {
			WinBioOperation = WINBIO_OPERATION_ENROLL_CAPTURE;
			pThis->SetNextOperation(WinBioOperation);
		}
		else {
			WinBioOperation = WINBIO_OPERATION_CLOSE;
			pThis->m_ErrorString = pThis->ConvertErrorCodeToString(hr);
			pThis->SetNextOperation(WinBioOperation);
		}
		break;
	case WINBIO_OPERATION_ENROLL_CAPTURE:
		if (hr == WINBIO_I_MORE_DATA) {
			pThis->m_WinbioRecordCount = pThis->m_WinbioRecordCount + 1;
			WinBioOperation = WINBIO_OPERATION_ENROLL_CAPTURE;
			pThis->SetNextOperation(WinBioOperation, 250);
		}
		else if (SUCCEEDED(hr)) {
			pThis->m_WinbioRecordCount = pThis->m_WinbioRecordCount + 1;
			WinBioOperation = WINBIO_OPERATION_ENROLL_COMMIT;
			pThis->SetNextOperation(WinBioOperation);
		}
		else if (hr == WINBIO_E_BAD_CAPTURE) {
			WinBioOperation = WINBIO_OPERATION_ENROLL_CAPTURE;
			pThis->SetNextOperation(WinBioOperation, 250);
		}
		else {
			WinBioOperation = WINBIO_OPERATION_CLOSE;
			pThis->m_ErrorString = pThis->ConvertErrorCodeToString(hr);
			pThis->SetNextOperation(WinBioOperation);
		}
		break;
	case WINBIO_OPERATION_ENROLL_COMMIT:
		if (SUCCEEDED(hr)) {
			pThis->m_WinbioFinished = true;
			pThis->m_WinbioOperationType = WINBIO_OPERATION_ENROLL_COMMIT;
			return;
		}
		else if (hr == WINBIO_E_DUPLICATE_TEMPLATE) {
			pThis->m_WinbioRecordCount = 0;
			WinBioOperation = WINBIO_OPERATION_ENROLL_BEGIN;
			pThis->SetNextOperation(WinBioOperation, 2000);
		}
		else {
			WinBioOperation = WINBIO_OPERATION_CLOSE;
			pThis->m_ErrorString = pThis->ConvertErrorCodeToString(hr);
			pThis->SetNextOperation(WinBioOperation);
		}
		break;
	case WINBIO_OPERATION_ENROLL_DISCARD:
		WinBioOperation = WINBIO_OPERATION_ENROLL_BEGIN;
		pThis->SetNextOperation(WinBioOperation);
		break;
	case WINBIO_OPERATION_ENUM_ENROLLMENTS:
		if (SUCCEEDED(hr) || hr == WINBIO_E_UNKNOWN_ID) {
			PWINBIO_BIOMETRIC_SUBTYPE subTypeList = AsyncResult->Parameters.EnumEnrollments.SubFactorArray;
			SIZE_T subTypeCount = AsyncResult->Parameters.EnumEnrollments.SubFactorCount;
			if (false == pThis->GetAvailableSubtype(subTypeList, subTypeCount, pThis->m_WinbioSubtype)) {
				WinBioOperation = WINBIO_OPERATION_CLOSE;
				pThis->m_ErrorString = pThis->ConvertErrorCodeToString(WINBIO_E_DATABASE_FULL);
				pThis->SetNextOperation(WinBioOperation);
				return;
			}
			//開始註冊
			WinBioOperation = WINBIO_OPERATION_ENROLL_BEGIN;
			pThis->SetNextOperation(WinBioOperation);
		}
		else {
			WinBioOperation = WINBIO_OPERATION_CLOSE;
			pThis->m_ErrorString = pThis->ConvertErrorCodeToString(hr);
			pThis->SetNextOperation(WinBioOperation);
		}
		break;
	}
}
//---------------------------------------------------------------------------------//
//  callback 只決定下一個狀態，真正執行由 FindFingerprint 內的 while loop 呼叫 ExecuteCurrentFindState
void Fingerprint_PQIFPS_Reader::OnAsyncCompletionFind(PWINBIO_ASYNC_RESULT AsyncResult)
{
	HRESULT hr = AsyncResult->ApiStatus;
	WINBIO_IDENTITY identity = {};
	WINBIO_OPERATION_TYPE WinBioOperation;
	Fingerprint_PQIFPS_Reader* pThis = static_cast<Fingerprint_PQIFPS_Reader*>(AsyncResult->UserData);
	if (pThis == nullptr) { return; }

	switch (AsyncResult->Operation)
	{
	case WINBIO_OPERATION_IDENTIFY:
		if (SUCCEEDED(hr)) {
			pThis->m_WinbioFinished = true;
			WinBioOperation = WINBIO_OPERATION_CLOSE;
			pThis->m_WinbioSubtype = AsyncResult->Parameters.Identify.SubFactor;
			pThis->m_WinbioOperationType = WinBioOperation;
			return;
		}
		else if (hr == WINBIO_E_BAD_CAPTURE || hr == WINBIO_E_UNKNOWN_ID|| hr == WINBIO_E_CANCELED) {
			WinBioOperation = WINBIO_OPERATION_IDENTIFY;
			pThis->SetNextOperation(WinBioOperation, 250);
		}
		else {
			WinBioOperation = WINBIO_OPERATION_CLOSE;
			pThis->m_ErrorString = pThis->ConvertErrorCodeToString(hr);
			pThis->SetNextOperation(WinBioOperation);
		}
		break;
	case WINBIO_OPERATION_LOCATE_SENSOR:
		if (SUCCEEDED(hr)) {
			WinBioOperation = WINBIO_OPERATION_IDENTIFY;
			pThis->SetNextOperation(WinBioOperation);
		}
		else {
			WinBioOperation = WINBIO_OPERATION_CLOSE;
			pThis->m_ErrorString = pThis->ConvertErrorCodeToString(hr);
			pThis->SetNextOperation(WinBioOperation);
		}
		break;
	}
}
//---------------------------------------------------------------------------------//
bool Fingerprint_PQIFPS_Reader::PostMessageStringToParentWnd(UINT Msg, WPARAM wParam, CString str)
{
	//OnMessage need delete the pointer
	if (m_hParentWnd == NULL) { return false; }
	if (!::IsWindow(m_hParentWnd)) { return false; }
	CString* pString = new CString(str);
	if (!::PostMessage(m_hParentWnd, Msg, wParam, (LPARAM)pString))
	{
		delete pString;
		return false;
	}
	return true;
}
//---------------------------------------------------------------------------------//
//  執行目前的註冊狀態
HRESULT Fingerprint_PQIFPS_Reader::ExecuteCurrentRecordState()
{
	HRESULT hr = S_OK;
	WINBIO_IDENTITY identity = {};

	switch (m_WinbioOperationType)
	{
	case WINBIO_OPERATION_ENUM_ENROLLMENTS:
		hr = GetSystemPoolIdentity(&identity);
		if (FAILED(hr)) { return hr; }
		hr = WinBioEnumEnrollments(m_WinbioSession, m_WinbioUnitId, &identity, NULL, NULL);
		return hr;

	case WINBIO_OPERATION_ENROLL_BEGIN:
		hr = WinBioEnrollBegin(m_WinbioSession, m_WinbioSubtype, m_WinbioUnitId);
		return hr;

	case WINBIO_OPERATION_ENROLL_CAPTURE:
		hr = WinBioEnrollCapture(m_WinbioSession, NULL);
		return hr;

	case WINBIO_OPERATION_ENROLL_COMMIT:
		hr = WinBioEnrollCommit(m_WinbioSession, NULL, NULL);
		return hr;

	case WINBIO_OPERATION_CLOSE:
		Disconnected();
		return S_OK;

	default:
		return E_FAIL;
	}
}
//---------------------------------------------------------------------------------//
//  執行目前的辨識狀態
HRESULT Fingerprint_PQIFPS_Reader::ExecuteCurrentFindState()
{
	HRESULT hr = S_OK;
	WINBIO_IDENTITY identity = {};

	switch (m_WinbioOperationType)
	{
	case WINBIO_OPERATION_IDENTIFY:
		hr = WinBioIdentify(m_WinbioSession, &m_WinbioUnitId, &identity, NULL, NULL);
		return hr;

	case WINBIO_OPERATION_LOCATE_SENSOR:
		hr = WinBioLocateSensor(m_WinbioSession, &m_WinbioUnitId);
		return hr;

	case WINBIO_OPERATION_CLOSE:
		Disconnected();
		return S_OK;

	default:
		return E_FAIL;
	}
}
//---------------------------------------------------------------------------------//
//  設定下一個狀態，delayMs 可用來取代 callback 內的 Sleep
void Fingerprint_PQIFPS_Reader::SetNextOperation(WINBIO_OPERATION_TYPE NextOperation, DWORD DelayMs)
{
	m_WinbioNextOperationType = NextOperation;
	m_WinbioStateChanged = true;
	m_NextOperationTick = GetTickCount() + DelayMs;
}
//---------------------------------------------------------------------------------//
//  UI 顯示目前要執行的狀態
void Fingerprint_PQIFPS_Reader::NotifyOperationUi(WINBIO_OPERATION_TYPE WinBioOperation)
{
	CString str = GetWinBioOperationTypeName(WinBioOperation);
	str = LoadMultiLanguageString(str, str);
	if (WinBioOperation == WINBIO_OPERATION_ENROLL_CAPTURE) {
		str.Format(_T("%s[%d]"), str, m_WinbioRecordCount);
	}
	PostMessageStringToParentWnd(MSG_FINGERPRINT_WND, WPARAM_FINGERPRINT_SHOW_OPERATION_STRING, str);
	str = GetWinBioOperationHint(WinBioOperation);
	str = LoadMultiLanguageString(str, str);
	PostMessageStringToParentWnd(MSG_FINGERPRINT_WND, WPARAM_FINGERPRINT_SHOW_STATE_STRING, str);
}
//---------------------------------------------------------------------------------//

#endif
