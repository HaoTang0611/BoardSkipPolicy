#include "stdafx.h"
#include "jet8000.h"
#include "JetSerial.h"
#include "FingerprintDefine.h"
#include "Fingerprint_AS608.h"
#include "FingerprintWnd.h"
#include <vector>
#include <numeric>
using namespace std;
Fingerprint_AS608::Fingerprint_AS608()
{
	InitialFPSDevice();
}
//-------------------------------------------------------------------------------------//
Fingerprint_AS608::~Fingerprint_AS608()
{
	Disconnected();
}
//-------------------------------------------------------------------------------------//
bool Fingerprint_AS608::InitialFPSDevice()
{
	m_DeviceName = _T("AS608");
	m_FPSDeviceWaitDataCount = 50;//等待次數
	m_FPSDeviceWaitDataDwellTime = 50;//等待資料的延遲時間-ms
	m_FPSDeviceReadDataDelayTime = 50;//讀取資料前延遲時間-ms
	m_FPSDeviceBaudRate = 57600;
	LoadFPSINIFile();
	SaveFPSINIFile();
	return true;
}
//-------------------------------------------------------------------------------------//
bool Fingerprint_AS608::ConnectDevice()
{
	int nBaud = 115200;
	CString   strPort = GetDevicePort();
	const int nByteSize = 8;
	const int nParity = SERIES_PARITY_NONE;
	const int nStopBits = ONESTOPBIT;
	const int nPort = JetAPI::StrToInt(strPort);
	const int nINIBaud = m_FPSDeviceBaudRate;
	const int Baud[] = { nINIBaud, CBR_9600, CBR_14400, CBR_19200, CBR_38400, CBR_57600, CBR_115200};
	const int MaxBaud = sizeof(Baud) / sizeof(Baud[0]);
	int i = 0;

	bool IsConnected = false;
	CString str = _T("");
	CString strDeviceName;

	//斷線
	Disconnected();

	for (i = 0; i < MaxBaud; i++)
	{
		if (this->m_RS232COM.IsOpened())
		{ this->m_RS232COM.Close(); }
		nBaud = Baud[i];
		if (nINIBaud == nBaud && i>0) { continue; }
		
		if (m_RS232COM.Open(nPort, nBaud, nByteSize, nParity, nStopBits) == false)
		{
			IsConnected = false;
			m_ErrorString.Format(_T("%s::RS232 Connected Fault [Port::%d]"), strDeviceName, nPort);
			continue;
		}
		if (!m_RS232COM.IsOpened())
		{
			IsConnected = false;
			m_ErrorString.Format(_T("%s::RS232 Connected Fault [Port::%d]"), strDeviceName, nPort);
			continue;
		}
		if (false == SendCommand(AS608_COMMAND_READSYSPARA, AS608_BUFFER_NUMBER_2)) { return false; }
		AS608_STATUS status = (AS608_STATUS)ReadResponse(AS608_COMMAND_READSYSPARA);
		if (status == AS608_STATUS_PORT_INVALID) { continue; }
		m_FPSDeviceBaudRate = nBaud;

		SaveFPSINIFile();
		IsConnected = true;
		SetDeviceConnected(IsConnected);
		break;
	}
	if (IsConnected == true){	m_ErrorString = _T("");	}
	else { this->m_RS232COM.Close(); }

	SetDeviceConnected(IsConnected);
	return IsConnected;
}
//-------------------------------------------------------------------------------------//
bool Fingerprint_AS608::Disconnected()
{
	if (this->m_RS232COM.IsOpened())
	{	this->m_RS232COM.Close();	}
	SetDeviceConnected(false);
	return true;
}
//-------------------------------------------------------------------------------------//
bool Fingerprint_AS608::CheckConnected()
{
	if (m_RS232COM.IsOpened() == false) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool Fingerprint_AS608::CheckFPSDeviceExisted()
{
	int nPort;
#ifdef _DEBUG_FindUsb
	if (false == JetAPI::FindUsbComPort(_T("CP210xX"), nPort)) {
#else
	if (false == JetAPI::FindUsbComPort(_T("CP210x"), nPort)) {
#endif // _DEBUG_FindUsb
		return false;
	}
	SetCOMPort(nPort);
	return true;
}
//-------------------------------------------------------------------------------------//
bool Fingerprint_AS608::DelFingerprintInDB(std::vector<TUserNode> UserList)
{
	return true;
}
//-------------------------------------------------------------------------------------//
bool Fingerprint_AS608::WaitForDataInQuene()
{
	int WaitCount = 0;
	const int MaxWaitCounts = m_FPSDeviceWaitDataCount;
	while (m_RS232COM.ReadDataWaiting() == 0)
	{
		Sleep(m_FPSDeviceWaitDataDwellTime);
		WaitCount++;
		if (WaitCount > MaxWaitCounts)
		{
			m_ErrorString.Format(_T("%s::wait for data too long"), m_DeviceName);
			return false;
		}
	};
	return true;
}
//-------------------------------------------------------------------------------------//
bool Fingerprint_AS608::RecordFingerprint(LPVOID pParam)
{
	return ExecuteRecordFingerprint_V2(pParam);
}
//-------------------------------------------------------------------------------------//
bool Fingerprint_AS608::MatchFingerprint(LPVOID pParam)
{
	return ExecuteMatchFingerprint(pParam);
}
//-------------------------------------------------------------------------------------//
bool Fingerprint_AS608::FindFingerprint(LPVOID pParam)
{
	return ExecuteFindFingerprintLoop(pParam);
}
//-------------------------------------------------------------------------------------//
bool Fingerprint_AS608::GetBiometricIdentity(BYTE * pBuffer, DWORD &nBufferSize)
{
	return GetFeatureIdentity(pBuffer, nBufferSize);
}
//-------------------------------------------------------------------------------------//
bool Fingerprint_AS608::GetFeatureIdentity(BYTE * pBuffer, DWORD &nBufferSize)
{
	nBufferSize = m_FeatureSize;
	memcpy(pBuffer, m_Feature, nBufferSize);
	return true;
}
//-------------------------------------------------------------------------------------//
bool Fingerprint_AS608::ExecuteRecordFingerprint(LPVOID pParam)
{
	CFingerprintWnd* pThis = (CFingerprintWnd*)pParam;
	AS608_STATUS status;
	bool isread = false;
	while (!isread && false == pThis->CheckWndIsCancel()) {
		Sleep(50);
		// 呼叫AS608採集指紋
		if (false == SendCommand(AS608_COMMAND_GETIMAGE, AS608_BUFFER_NUMBER_1)) { return false; }
		status = (AS608_STATUS)ReadResponse(AS608_COMMAND_GETIMAGE);
		pThis->PostMessage(MSG_FINGERPRINT_WND, AS608_COMMAND_GETIMAGE, status);
		if (AS608_STATUS_OK != status) { continue; }
		// 呼叫AS608生成指紋特徵
		if (false == SendCommand(AS608_COMMAND_GENCHAR, AS608_BUFFER_NUMBER_1)) { return false; }
		status = (AS608_STATUS)ReadResponse(AS608_COMMAND_GENCHAR);
		//pThis->PostMessage(MSG_FINGERPRINT_WND, AS608_COMMAND_GENCHAR, status);
		if (AS608_STATUS_OK != status) { continue; }

		// 呼叫AS608上傳指紋特徵
		//if (AS608_STATUS_OK != ReadResponse(AS608_COMMAND_GENCHAR)) { continue; }
		if (false == SendCommand(AS608_COMMAND_UPCHAR, AS608_BUFFER_NUMBER_1)) { return false; }
		status = (AS608_STATUS)ReadResponse(AS608_COMMAND_UPCHAR);
		pThis->PostMessage(MSG_FINGERPRINT_WND, AS608_COMMAND_UPCHAR, status);
		isread = (AS608_STATUS_OK == status);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool Fingerprint_AS608::ExecuteRecordFingerprint_V2(LPVOID pParam)
{
	// ExecuteRecordFingerprint 嚴格版，採集兩次指紋

	CFingerprintWnd* pThis = (CFingerprintWnd*)pParam;
	bool isread = false;
	AS608_STATUS status;
	AS608_COMMAND Command = AS608_COMMAND_GETIMAGE;
	AS608_BUFFER_NUMBER CharBuffer = AS608_BUFFER_NUMBER_1;
	FINGERPRINT_RECORD_PROCESS ProcessState = FINGERPRINT_RECORD_PROCESS_RECORD;

	while (!isread && false == pThis->CheckWndIsCancel()) {
		Sleep(10);
		//發送指令並接收回傳
		if (false == SendCommand(Command, CharBuffer)) { return false; }
		status = (AS608_STATUS)ReadResponse(Command);
		pThis->PostMessage(MSG_FINGERPRINT_WND, Command, status);
		if (AS608_STATUS_PORT_INVALID == status) { return false; }
		//如果正確執行，改變ProcessState、Command與CharBuffer。
		//完整流程	收集指紋->生成特徵->提醒暫時移開手指->
		//			收集指紋->生成特徵->合併特徵->上傳
		if (AS608_STATUS_OK == status) {
			if (FINGERPRINT_RECORD_PROCESS_MERGE == ProcessState) {
				if (true == ExecuteFindFingerprint(pParam)) {
					pThis->PostMessage(MSG_FINGERPRINT_WND, WPARAM_FINGERPRINT_SHOW_STATE, LPARAM_FINGERPRINT_EXIST);
					CharBuffer = AS608_BUFFER_NUMBER_1;
					ProcessState = FINGERPRINT_RECORD_PROCESS_RECORD;
					Command = AS608_COMMAND_GETIMAGE;
					Sleep(100);
				}
			}
			switch (ProcessState)
			{
			case(FINGERPRINT_RECORD_PROCESS_RECORD):	//收集指紋
				ProcessState = FINGERPRINT_RECORD_PROCESS_GENCHAR;
				Command = AS608_COMMAND_GENCHAR;
				break;
			case(FINGERPRINT_RECORD_PROCESS_GENCHAR):	//生成特徵
				if (AS608_BUFFER_NUMBER_1 == CharBuffer) {
					ProcessState = FINGERPRINT_RECORD_PROCESS_MOVEOUT;
					Command = AS608_COMMAND_GETIMAGE;
				}
				else {
					ProcessState = FINGERPRINT_RECORD_PROCESS_MERGE;
					Command = AS608_COMMAND_REGMODEL;
				}
				break;
			case(FINGERPRINT_RECORD_PROCESS_MOVEOUT):	//提醒暫時移開手指
				pThis->PostMessage(MSG_FINGERPRINT_WND, AS608_COMMAND_GETIMAGE, AS608_STATUS_NO_MOVE);
				break;
			case(FINGERPRINT_RECORD_PROCESS_MERGE):		//合併特徵
				ProcessState = FINGERPRINT_RECORD_PROCESS_UPCHAR;
				Command = AS608_COMMAND_UPCHAR;
				break;
			case(FINGERPRINT_RECORD_PROCESS_UPCHAR):	//上傳
				isread = true;
				break;
			default:
				break;
			}
		}
		else if (ProcessState == FINGERPRINT_RECORD_PROCESS_MOVEOUT) {
			//特殊:	在 -提醒暫時移開手指- 狀態下，同樣是 -收集指紋-，
			//		但要求回傳訊息，不能是OK
			ProcessState = FINGERPRINT_RECORD_PROCESS_RECORD;
			CharBuffer = AS608_BUFFER_NUMBER_2;
			Sleep(100);
		}
		else {
			if (ProcessState>FINGERPRINT_RECORD_PROCESS_MOVEOUT)
				CharBuffer = AS608_BUFFER_NUMBER_1;
			//執行失敗:	重新回到 -收集指紋- 
			ProcessState = FINGERPRINT_RECORD_PROCESS_RECORD;
			Command = AS608_COMMAND_GETIMAGE;
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool Fingerprint_AS608::ExecuteMatchFingerprint(LPVOID pParam)
{
	CFingerprintWnd* pThis = (CFingerprintWnd*)pParam;
	AS608_STATUS status;
	BYTE UserFingerFeature[768];
	pThis->GetEnrollFingerPrint(UserFingerFeature);
	bool isOK = false;

	// 呼叫AS608準備接收指紋
	if (false == SendCommand(AS608_COMMAND_DOWNCHAR, AS608_BUFFER_NUMBER_2)) { return false; }
	status = (AS608_STATUS)ReadResponse(AS608_COMMAND_DOWNCHAR);
	pThis->PostMessage(MSG_FINGERPRINT_WND, AS608_COMMAND_DOWNCHAR, status);

	if (AS608_STATUS_OK != status) { return false; }
	// 傳送指紋特徵
	//if (false == SendPacket(UserFingerFeature)) { return false; }
	if (false == SendData(UserFingerFeature)) { return false; } 

	// 將上傳給AS608的特徵下載，比對是否有掉封包
	if (false == SendCommand(AS608_COMMAND_UPCHAR, AS608_BUFFER_NUMBER_2)) { return false; }
	status = (AS608_STATUS)ReadResponse(AS608_COMMAND_UPCHAR);
	if (status == AS608_STATUS_OK) {
		for (size_t i = 0; i < m_FeatureSize; i++) {
			if (UserFingerFeature[i] != m_Feature[i]) {
				m_ErrorString = L"Fingerprint from AS608 is different to user list, please try again later.\n It cause by lost packet or fingerprint feature is broken.";
				return false;
			}
		}
	}

	while (!isOK && false == pThis->CheckWndIsCancel()) {
		Sleep(10);
		// 呼叫AS608採集指紋
		if (false == SendCommand(AS608_COMMAND_GETIMAGE, AS608_BUFFER_NUMBER_1)) { return false; }
		status = (AS608_STATUS)ReadResponse(AS608_COMMAND_GETIMAGE);
		pThis->PostMessage(MSG_FINGERPRINT_WND, AS608_COMMAND_GETIMAGE, status);
		if (AS608_STATUS_OK != status) { continue; }
		// 呼叫AS608生成指紋特徵
		if (false == SendCommand(AS608_COMMAND_GENCHAR, AS608_BUFFER_NUMBER_1)) { return false; }
		status = (AS608_STATUS)ReadResponse(AS608_COMMAND_GENCHAR);
		pThis->PostMessage(MSG_FINGERPRINT_WND, AS608_COMMAND_GENCHAR, status);
		if (AS608_STATUS_OK != status) { continue; }

		// 呼叫AS608比對指紋特徵
		if (false == SendCommand(AS608_COMMAND_MATCH, AS608_BUFFER_NUMBER_1)) { return false; }
		status = (AS608_STATUS)ReadResponse(AS608_COMMAND_MATCH);
		pThis->PostMessage(MSG_FINGERPRINT_WND, AS608_COMMAND_MATCH, status);
		isOK = (status == AS608_STATUS_OK);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool Fingerprint_AS608::ExecuteFindFingerprint(LPVOID pParam)
{
	std::vector<TUserNode> UserList;
	TUserNode UserNode;
	AS608_STATUS status;

	CFingerprintWnd* pThis = (CFingerprintWnd*)pParam;
	UserList = pThis->GetUserList();
	if (UserList.empty()) {
		CString filename = AOIDataCollect.GetUserFilename();
		AOIDataCollect.ReadUserFile(filename, UserList);
	}

	//loop比對指紋
	for (int i = 0; i < UserList.size(); i++) {
		UserNode = UserList[i];
		if (false == UserNode.wFingerEnable) { continue; }
		// 呼叫AS608準備接收指紋
		Sleep(10);
		if (false == SendCommand(AS608_COMMAND_DOWNCHAR, AS608_BUFFER_NUMBER_1)) { return false; }
		status = (AS608_STATUS)ReadResponse(AS608_COMMAND_DOWNCHAR);
		pThis->PostMessage(MSG_FINGERPRINT_WND, AS608_COMMAND_DOWNCHAR, status);
		if (AS608_STATUS_OK != status) { return false; }
		// 傳送指紋特徵
		if (false == SendData(UserNode.wFinger)) { return false; }
		// 呼叫AS608比對指紋特徵
		if (false == SendCommand(AS608_COMMAND_MATCH, AS608_BUFFER_NUMBER_1)) { return false; }
		status = (AS608_STATUS)ReadResponse(AS608_COMMAND_MATCH);
		pThis->PostMessage(MSG_FINGERPRINT_WND, AS608_COMMAND_MATCH, status);
		if (AS608_STATUS_OK != status) { continue; }
		pThis->SetFingerPrintUser(UserNode);
		return true;
	}
	return false;
}
//-------------------------------------------------------------------------------------//
bool Fingerprint_AS608::ExecuteFindFingerprintLoop(LPVOID pParam)
{
	CFingerprintWnd* pThis = (CFingerprintWnd*)pParam;
	bool isOK = false;
	AS608_STATUS status;
	while (!isOK && false == pThis->CheckWndIsCancel()) {
		Sleep(50);
		// 呼叫AS608採集指紋
		if (false == SendCommand(AS608_COMMAND_GETIMAGE)) { return false; }

		status = (AS608_STATUS)ReadResponse(AS608_COMMAND_GETIMAGE);
		pThis->PostMessage(MSG_FINGERPRINT_WND, AS608_COMMAND_GETIMAGE, status);
		if (AS608_STATUS_OK != status) { continue; }
		// 呼叫AS608生成指紋特徵
		if (false == SendCommand(AS608_COMMAND_GENCHAR, AS608_BUFFER_NUMBER_2)) { return false; }
		status = (AS608_STATUS)ReadResponse(AS608_COMMAND_GENCHAR);
		pThis->PostMessage(MSG_FINGERPRINT_WND, AS608_COMMAND_GENCHAR, status);
		if (AS608_STATUS_OK != status) { continue; }
		isOK = ExecuteFindFingerprint(pThis);
	}
	if (true == pThis->CheckWndIsCancel()) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
void Fingerprint_AS608::SetCOMPort(unsigned int nPort)
{
	CString port;
	port.Format(_T("%d"), nPort);
	if (true == this->CheckConnected()) {
		if (false == this->Disconnected()) {
			this->SetDevicePort(port);
			m_ErrorString.Format(_T("Fingerprint::RS232 Connected Fault [Port::%d]"), nPort);
		}
	}
	else { this->SetDevicePort(port); }
	return;
}
//-------------------------------------------------------------------------------------//
bool Fingerprint_AS608::SendCommand(AS608_COMMAND command, AS608_BUFFER_NUMBER buffer)
{
	//	指令碼
	//	2bytes:	0xEF, 0x01,					//封包標頭 固定
	//	4bytes:	0xFF, 0xFF, 0xFF, 0xFF,		//芯片地址
	//	1bytes:	0x01,						//封包標誌 01 命令,02數據 08最後數據包
	// 以上都固定，除非修改芯片地址
	//	2bytes: 0x00, 0x03,					//封包長度 從這一項後的bytes數
	//	1bytes: 0x01,						//指令碼 : AS608_COMMAND 
	//  ?bytes:								//其他，根據指令碼的要求
	//	2bytes: 0x00, 0x05					//校驗和 為 封包標誌(包含)之後的累加
	vector<BYTE> v_CommandCode = { 0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x01 };
	BYTE CommandCode[16];
	memset(CommandCode, 0x00, sizeof(CommandCode));
	int length, CodeSum = 0;
	switch (command)
	{
	case(AS608_COMMAND_GETIMAGE):
	case(AS608_COMMAND_MATCH):
	case(AS608_COMMAND_REGMODEL):
	case(AS608_COMMAND_READSYSPARA):
		length = 3;
		break;
	case(AS608_COMMAND_GENCHAR):
	case(AS608_COMMAND_UPCHAR):
	case(AS608_COMMAND_DOWNCHAR):
		length = 4;
		break;
	default:
		break;
	}
	//-------------------------------------------------------------//
	// 封包長度
	v_CommandCode.push_back((length >> 8) & 0xFF);
	v_CommandCode.push_back(length & 0xFF);
	//-------------------------------------------------------------//
	// 指令碼
	v_CommandCode.push_back(command);
	//-------------------------------------------------------------//
	// 其他封包
	switch (command)
	{
	case(AS608_COMMAND_GENCHAR):
	case(AS608_COMMAND_UPCHAR):
	case(AS608_COMMAND_DOWNCHAR):
		v_CommandCode.push_back(buffer);
		break;
	default:
		break;
	}
	//-------------------------------------------------------------//
	// 校驗和
	for (int i = 6; i < v_CommandCode.size(); i++) {
		CodeSum += v_CommandCode[i];
	}
	v_CommandCode.push_back((CodeSum >> 8) & 0xFF);
	v_CommandCode.push_back(CodeSum & 0xFF);
	//-------------------------------------------------------------//
	copy(v_CommandCode.begin(), v_CommandCode.end(), CommandCode);
	if (m_RS232COM.SendData((char*)CommandCode, v_CommandCode.size()) == 0)
	{
		m_ErrorString = _T("FingerPrint::send data fault");
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
AS608_STATUS Fingerprint_AS608::ReadResponse(AS608_COMMAND command)
{
	int i = 0;
	AS608_STATUS status;
	unsigned int size, TotalSize = 0;
	BYTE flag = 0;
	BYTE data[256];
	memset(data, 0x00, sizeof(data));
	CString hexStr;

	if (false == WaitForDataInQuene()) { return AS608_STATUS_PORT_INVALID; }
	switch (command)
	{
	case(AS608_COMMAND_GETIMAGE):
	case(AS608_COMMAND_GENCHAR):
	case(AS608_COMMAND_DOWNCHAR):
	case(AS608_COMMAND_MATCH):
	case(AS608_COMMAND_REGMODEL):
	case(AS608_COMMAND_READSYSPARA):
		return ReadResponse(size, flag, data);
	case(AS608_COMMAND_UPCHAR):
		status = (AS608_STATUS)ReadResponse(size, flag, data);
		if (status != AS608_STATUS_OK) { return status; }
		//if (false == ReadPacket(m_FeaturePacket)) { return AS608_STATUS_FRAME_ERROR; }
		if (false == ReadData(size, flag, data)) { return AS608_STATUS_FRAME_ERROR; }
		return status;
	default:
		break;
	}
}
//-------------------------------------------------------------------------------------//
AS608_STATUS Fingerprint_AS608::ReadResponse(unsigned int &size, BYTE& flag, BYTE* data)
{
	//	指令碼
	//	2bytes:	0xEF, 0x01,			 		//封包標頭 固定
	//	4bytes:	0xFF, 0xFF, 0xFF, 0xFF,		//芯片地址
	//	1bytes:	0x02,						//封包標誌 01:命令,02:數據包且會有後續包,07:,08:最後數據包
	// 以上都固定，除非修改芯片地址
	//	2bytes: 0x00, 0x03,					//封包長度 從這一項後的bytes數
	//	0-1bytes: 0x00,						//確認碼 : AS608_STATUS ，數據包沒有這一項
	//  ?bytes:								//其他，根據指令碼的要求
	//	2bytes: 0x00, 0x04					//校驗和 為 封包標誌(包含)之後的累加
	const int length = 9;//先讀取封包長度
	int ReadedLength = 0;
	BYTE Msg[length];

	memset(Msg, 0x00, sizeof(Msg));
	int nReads;
	for (int i = 0; i < 30; i++)
	{
		nReads = m_RS232COM.ReadData_Syn(Msg + ReadedLength, length - ReadedLength);
		ReadedLength += nReads;
		if (ReadedLength == length) { break; }
		if (20 == i)
		{
			m_ErrorString = _T("AS608:[Packet length] No Response");
			return AS608_STATUS_NO_FRAME;
		}
		Sleep(10);
	}
	flag = Msg[6]; //封包標誌
	size = (Msg[length - 2] << 8) + Msg[length - 1];
	ReadedLength = 0;
	for (int i = 0; i < 30; i++)
	{
		nReads = m_RS232COM.ReadData_Syn(data + ReadedLength, size - ReadedLength);
		ReadedLength += nReads;
		if (ReadedLength == size) { break; }
		if (20 == i)
		{
			m_ErrorString = _T("AS608:[AS608 Responese] No Response");
			//return AS608_STATUS_PORT_INVALID;
			return AS608_STATUS_NO_FRAME;
		}
		Sleep(10);
	}
	// 校驗和驗證
	int CodeSum = accumulate(data, data + size - 2, 0);//size-2 去掉校驗碼
	CodeSum += flag;
	CodeSum += Msg[length - 2] + Msg[length - 1];
	int ResponseCodeSum = (data[size - 2] << 8) + data[size - 1];
	//int ResponseCodeSum = data[size - 2] * 256 + data[size - 1];
	//if (CodeSum != ResponseCodeSum)return AS608_STATUS_FRAME_ERROR;
	if (CodeSum != ResponseCodeSum) {
		m_ErrorString = _T("AS608: Verification code error");
		return AS608_STATUS_FRAME_ERROR;
	}
	return (AS608_STATUS)data[0];
}
//-------------------------------------------------------------------------------------//
bool Fingerprint_AS608::ReadData(unsigned int & size, BYTE & flag, BYTE * data)
{
	const int length = 9;//先讀取封包長度
	BYTE Msg[length];
	BYTE temp[256];
	memset(Msg, 0x00, sizeof(Msg));
	memset(temp, 0x00, sizeof(temp));
	memset(m_Feature, 0x00, sizeof(temp));

	int nReads,CodeSum,Position=0;
	flag = 0;

	int ReadedLength;
	while (flag != AS608_FLAG_DATA_END) {
		Sleep(20);//
		ReadedLength = 0;
		//讀取包頭到資料長度
		for (int i = 0; i < 30; i++)
		{
			nReads = m_RS232COM.ReadData_Syn(Msg + ReadedLength, length - ReadedLength);
			ReadedLength += nReads;
			if (ReadedLength == length) { break; }
			if (20 == i) { return false; }
			Sleep(10);
		}
		flag = Msg[6]; //封包標誌
		size = (Msg[length - 2] << 8) + Msg[length - 1];
		//size = Msg[length - 2] * 256 + Msg[length - 1];
		//讀取資料
		ReadedLength = 0;
		for (int i = 0; i < 30; i++)
		{
			nReads = m_RS232COM.ReadData_Syn(temp + ReadedLength, size - ReadedLength);
			ReadedLength += nReads;
			if (ReadedLength == size) { break; }
			if (20 == i) { return false; }
			Sleep(10);
		}
		//校驗和驗證
		CodeSum = accumulate(temp, temp + size - 2, 0);//size-2 去掉校驗碼
		CodeSum += flag;
		CodeSum += Msg[length - 2] + Msg[length - 1];
		int ResponseCodeSum = (temp[size - 2] << 8) | temp[size - 1];
		if (CodeSum != ResponseCodeSum)return false;
		//驗證成功
		memcpy(m_Feature + Position, temp, size-2);
		Position += size - 2;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool Fingerprint_AS608::SendData(BYTE * data)
{
	//	資料封包
	//	2bytes:	0xEF, 0x01,					//封包標頭 固定
	//	4bytes:	0xFF, 0xFF, 0xFF, 0xFF,		//芯片地址
	//	1bytes:	0x02,						//封包標誌 01 命令,02數據 08最後數據包
	// 以上都固定，除非修改芯片地址
	//	2bytes: 0x00, 0x82,					//封包長度 從這一項後的bytes數
	//  128bytes:	.....					//資料
	//	2bytes: 0x00, 0x05					//校驗和 為 封包標誌(包含)之後的累加
	const int HeaderSize = 9;
	const int DataSize = 128;
	const int TotalDataSize = 768;
	const int TotalPacketSize = 139;

	BYTE Header[HeaderSize] = { 0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x02, 0x00, 0x82 };
	BYTE Packet[TotalPacketSize];
	char buffer[TotalPacketSize];

	int length, CodeSum = 0;
	//unsigned int CodeSum2;
	memcpy(Packet, Header, HeaderSize);
	for (int i = 0; i < TotalDataSize; i += DataSize) {
		if (i == TotalDataSize - DataSize) {
			//封包旗幟改成 08 最後數據包
			Packet[6] = 0x08;
		}
		memcpy(Packet + HeaderSize, data + i, DataSize);
		//校驗和
		CodeSum = accumulate(Packet + 6, Packet + TotalPacketSize - 2, 0);

		Packet[TotalPacketSize - 2] = (CodeSum >> 8) & 0xff;
		Packet[TotalPacketSize - 1] = CodeSum & 0xff;

		memcpy(buffer, Packet, TotalPacketSize);
		length = m_RS232COM.SendData(buffer, TotalPacketSize);
		if (length != TotalPacketSize) return false;
		Sleep(10);
	}
	return true;
}
//-------------------------------------------------------------------------------------//