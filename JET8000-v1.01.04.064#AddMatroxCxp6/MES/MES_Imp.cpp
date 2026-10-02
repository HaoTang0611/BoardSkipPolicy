// MES_Imp.cpp: implementation of the CMES_Imp class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "MES_Imp.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
#ifndef MES_DISABLE
//-------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
CMES_Imp::CMES_Imp()
{
	::InitializeCriticalSection(&m_csImpProc);
	PreInitMES();
	InitialMES();
}
//-------------------------------------------------------------------------------------//
CMES_Imp::~CMES_Imp()
{
	::DeleteCriticalSection(&m_csImpProc);
}
//-------------------------------------------------------------------------------------//
void CMES_Imp::PreInitMES()
{	
}
//-------------------------------------------------------------------------------------//
void CMES_Imp::InitialMES()
{
	m_MesCommTimeoutMS = 10000;
	m_FreezeMESFuncMode = false;
	m_MesRemotCtrlOn = false;
	m_MesEqpCtrlStateMode = MES_EQP_CTRL_STATE_NONE;
	ResetAOIExceptionCode();	
}
//-------------------------------------------------------------------------------------//
void CMES_Imp::CloneMES(const CMES_Imp &other)
{
	m_MesCommTimeoutMS = other.m_MesCommTimeoutMS;
	m_FreezeMESFuncMode = other.m_FreezeMESFuncMode;
	m_MesRemotCtrlOn = other.m_MesRemotCtrlOn;
	m_MesEqpCtrlStateMode = other.m_MesEqpCtrlStateMode;	
}
//-------------------------------------------------------------------------------------//
bool CMES_Imp::ReturnNotImplement(LPCTSTR fnName)//回傳未完成函式
{
	m_ErrorString.Format(_T("Error, the Func[%s] not Implement"), fnName);
	return false;
}
//-------------------------------------------------------------------------------------//
void CMES_Imp::LockImpProc()//鎖住MES執行緒同步化
{
	::EnterCriticalSection(&m_csImpProc);
}
//-------------------------------------------------------------------------------------//
void CMES_Imp::UnlockImpProc()//釋放MES執行緒同步化
{
	::LeaveCriticalSection(&m_csImpProc);
}
//-------------------------------------------------------------------------------------//
LPCTSTR CMES_Imp::GetErrorString() const//取得錯誤訊息
{
	return m_ErrorString;
}
//-------------------------------------------------------------------------------------//
void CMES_Imp::SetErrorString(const char *String)//設定錯誤訊息
{
	m_ErrorString = String;
}
//----------------------------------------------------------------------------//
void CMES_Imp::SetErrorString(const wchar_t *String)//設定錯誤訊息
{
	m_ErrorString = String;
}
//----------------------------------------------------------------------------//
DWORD CMES_Imp::GetMesCommTimeoutMS() const//取得溝通愈時ms
{
	return m_MesCommTimeoutMS;
}
//----------------------------------------------------------------------------//
void CMES_Imp::SetMesCommTimeoutMS(DWORD val)//設定溝通愈時ms
{
	m_MesCommTimeoutMS = val;
}
//----------------------------------------------------------------------------//
bool CMES_Imp::GetFreezeMESFuncMode() const//取得凍結MES函式
{
	return m_FreezeMESFuncMode;
}
//----------------------------------------------------------------------------//
void CMES_Imp::SetFreezeMESFuncMode(bool val)//設定凍結MES函式
{
	m_FreezeMESFuncMode = val;
	m_MESLinker.SetFreezeComm(val);
}
//----------------------------------------------------------------------------//
bool CMES_Imp::WaitForFreezeMESFuncModeDone()//等待凍結MES函式結束
{
	int Cnt=0;
	int MaxCnt=100;
	while ( true )
	{
		if ( GetFreezeMESFuncMode() == false )
		{	break; }
		Cnt ++;
		if ( Cnt > MaxCnt )
		{	
			//m_ErrorString = GetMESMultiLanguage(str);
			m_ErrorString = _T("Error, IPS WaitForFreezeMESFuncModeDone Finish too long");
			return false;
		}
		::Sleep(10);
	};
	if ( Cnt > 0 )
	{	Cnt = Cnt; }
	return true;
}
//----------------------------------------------------------------------------//
bool CMES_Imp::GetMesRemotCtrlOn() const//取得MES遠端控制
{
	return m_MesRemotCtrlOn;
}
//----------------------------------------------------------------------------//
void CMES_Imp::SetMesRemotCtrlOn(bool val)//設定MES遠端控制
{
	m_MesRemotCtrlOn = val;
}
//----------------------------------------------------------------------------//
void CMES_Imp::ResetMesEqpCtrlStateMode()//復歸MES設備連線模式//None
{
	SetMesEqpCtrlStateMode(MES_EQP_CTRL_STATE_NONE);
}
//----------------------------------------------------------------------------//
MES_EQP_CTRL_STATE_MODE CMES_Imp::GetMesEqpCtrlStateMode() const//取得MES設備連線模式//Online, Offline, Local
{
	return m_MesEqpCtrlStateMode;
}
//----------------------------------------------------------------------------//
void CMES_Imp::SetMesEqpCtrlStateMode(MES_EQP_CTRL_STATE_MODE val)//設定MES設備連線模式//Online, Offline, Local
{
	m_MesEqpCtrlStateMode = val;
}
//----------------------------------------------------------------------------//
LANE_ID CMES_Imp::GetActiveLaneID()
{
	return AOIDataCollect.GetActiveLaneID();
}
//----------------------------------------------------------------------------//
CAOIProject*  CMES_Imp::GetActiveProject()
{
	CAOIProject *Ptr = AOIDataCollect.GetActiveProject();
	return Ptr;
}
//-------------------------------------------------------------------------------------//
TASK_STATE_MODE CMES_Imp::GetOnlineTaskState() const
{
	return AOIDataCollect.GetOnlineTaskState();
}
//-------------------------------------------------------------------------------------//
void CMES_Imp::SetOnlineTaskState(TASK_STATE_MODE val)
{
	AOIDataCollect.SetOnlineTaskState(val);
}
//-------------------------------------------------------------------------------------//
int CMES_Imp::GetAOIExceptionCode() const
{
	return AOIExceptionCodeCtrl.GetAOILastExceptionCode();
}
//-------------------------------------------------------------------------------------//
CString CMES_Imp::GetAOIExceptionText() const
{	
	return CString(AOIExceptionCodeCtrl.GetAOILastExceptionText());
}
//-------------------------------------------------------------------------------------//
void CMES_Imp::ResetAOIExceptionCode()
{
	m_AOIExceptionCodeLast = 0;
	m_AOIExceptionCodeTickCount = 0;
	return;
}
//-------------------------------------------------------------------------------------//
bool CMES_Imp::CheckBypassAOIExceptionCode()//跳過重複異常碼
{
#ifndef AOI_EXCEPTION_CODE_USE
	return false;
#endif//AOI_EXCEPTION_CODE_USE
	DWORD TickCountGap = GetTickCount()-m_AOIExceptionCodeTickCount;	
	if ( TickCountGap > 30000 )
	{	return false; }
	const int AOIExceptionCode = GetAOIExceptionCode();
	if ( AOIExceptionCode != m_AOIExceptionCodeLast )
	{	return false;	}

	CString str;
	str.Format(_T("CMES_Imp::CheckBypassAOIExceptionCode[%d]"), AOIExceptionCode);
	AOIDataCollect.SaveMESTimeMsg(str);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_Imp::CheckSystemReady(bool bChkStartLight, bool bAutoReset)//確認系統是否正常
{
	if ( AOIDataCollect.CheckSystemReady(bChkStartLight, bAutoReset) == false )
	{	
		m_ErrorString = AOIDataCollect.GetErrorString(); 
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_Imp::SendMainFrameWndMessage(UINT message, WPARAM wParam, LPARAM lParam)//送訊息給回傳主框架視窗 
{
	AOIDataCollect.SetWndMessageFromID(WND_MESSAGE_FROM_MES);
	if ( AOIDataCollect.SendMainFrameWndMessage(message, wParam, lParam) == false )
	{
		m_ErrorString = AOIDataCollect.GetErrorString(); 
		return false;
	}
	AOIDataCollect.SendMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_RESET_MSG_FROM_ID, NULL);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_Imp::PostMainFrameWndMessage(UINT message, WPARAM wParam, LPARAM lParam)//送訊息給回傳主框架視窗
{
	AOIDataCollect.SetWndMessageFromID(WND_MESSAGE_FROM_MES);
	if ( AOIDataCollect.PostMainFrameWndMessage(message, wParam, lParam) == false )
	{
		m_ErrorString = AOIDataCollect.GetErrorString(); 
		return false;
	}
	AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_RESET_MSG_FROM_ID, NULL);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_Imp::CreateMESProcThread()//建立MES執行執行緒
{
	return ReturnNotImplement(_T("CMES_Imp::CreateMESProcThread"));
}
//-------------------------------------------------------------------------------------//
bool CMES_Imp::DeleteMESProcThread()//刪除MES執行執行緒
{
	return ReturnNotImplement(_T("CMES_Imp::DeleteMESProcThread"));
}
//-------------------------------------------------------------------------------------//
CITSLinker& CMES_Imp::GetMESLinker()//取得MES連線參考
{
	return m_MESLinker;
}
//-------------------------------------------------------------------------------------//
bool CMES_Imp::GetMESConnected()//是否已經連線
{
	return ReturnNotImplement(_T("CMES_Imp::GetMESConnected"));
}
//-------------------------------------------------------------------------------------//
bool CMES_Imp::ConnectMESLinker()
{
	return ReturnNotImplement(_T("CMES_Imp::ConnectMESLinker"));
}
//-------------------------------------------------------------------------------------//
bool CMES_Imp::DisconnectMESLinker()//停止連線到MES連結軟體
{
	return ReturnNotImplement(_T("CMES_Imp::DisconnectMESLinker"));
}
//-------------------------------------------------------------------------------------//
bool CMES_Imp::ExecMESComm_CheckMESReady()//執行MES溝通-確認MES就緒
{
	return ReturnNotImplement(_T("CMES_Imp::ExecMESComm_CheckMESReady"));
}
//-------------------------------------------------------------------------------------//
bool CMES_Imp::ExecMESComm_SetSystemParam()//執行MES溝通-設定系統參數
{
	return ReturnNotImplement(_T("CMES_Imp::ExecMESComm_SetSystemParam"));
}
//-------------------------------------------------------------------------------------//
bool CMES_Imp::ExecMESComm_SetProjectParam(CAOIProject *ProjectPtr)//執行MES溝通-設定專案參數
{
	return ReturnNotImplement(_T("CMES_Imp::ExecMESComm_SetProjectParam"));
}
//-------------------------------------------------------------------------------------//
bool CMES_Imp::ExecMESComm_SetProjectLoadFinished(CAOIProject *ProjectPtr, bool bSucc, LPCTSTR Err)//執行MES溝通-專案載入完畢
{
	return ReturnNotImplement(_T("CMES_Imp::ExecMESComm_SetProjectLoadFinished"));
}
//-------------------------------------------------------------------------------------//
bool CMES_Imp::ExecMESComm_UploadProjectFile(LPCTSTR filename)//執行MES溝通-上傳專案檔案
{
	return ReturnNotImplement(_T("CMES_Imp::ExecMESComm_UploadProjectFile"));
}
//-------------------------------------------------------------------------------------//
bool CMES_Imp::ExecMESComm_DownloadProjectFile(LPCTSTR filename)//執行MES溝通-下載專案檔案
{
	return ReturnNotImplement(_T("CMES_Imp::ExecMESComm_DownloadProjectFile"));
}
//-------------------------------------------------------------------------------------//
bool CMES_Imp::ExecMESComm_SetMachineStatus()//執行MES溝通-設定機台狀態
{
	return ReturnNotImplement(_T("CMES_Imp::ExecMESComm_SetMachineStatus"));
}
//-------------------------------------------------------------------------------------//
bool CMES_Imp::ExecMESComm_SetMachineStatus(ONLINE_STATE_MODE Status)//執行MES溝通-設定機台狀態	
{
	return ReturnNotImplement(_T("CMES_Imp::ExecMESComm_SetMachineStatus"));	
}
//-------------------------------------------------------------------------------------//
bool CMES_Imp::ExecMESComm_ProcessID(int ProcessID)//執行MES溝通-程序運作
{
	return ReturnNotImplement(_T("CMES_Imp::ExecMESComm_ProcessID"));
}
//-------------------------------------------------------------------------------------//
bool CMES_Imp::ExecMESComm_SetAOIExceptionCode(LPCTSTR ErrStr)//執行MES溝通-系統異常碼
{
	return ReturnNotImplement(_T("CMES_Imp::ExecMESComm_SetAOIExceptionCode"));
}
//-------------------------------------------------------------------------------------//
bool CMES_Imp::ExecMESComm_SetUserLogin_out(bool bLogin)//執行MES溝通-設定使用者登入
{
	return ReturnNotImplement(_T("CMES_Imp::ExecMESComm_SetUserLogin_out"));
}
//-------------------------------------------------------------------------------------//
bool CMES_Imp::ExecMESComm_SetControlStateMode(MES_EQP_CTRL_STATE_MODE Mode)//執行MES溝通-設定控制狀態模式
{
	return ReturnNotImplement(_T("CMES_Imp::ExecMESComm_SetControlStateMode"));
}
//-------------------------------------------------------------------------------------//
bool CMES_Imp::ExecMESComm_CheckBarcode(CAOIProject *ProjectPtr)//執行MES溝通-確認條碼
{
	return ReturnNotImplement(_T("CMES_Imp::ExecMESComm_CheckBarcode"));
}
//-------------------------------------------------------------------------------------//
bool CMES_Imp::ExecMESComm_AskBarcode(CAOIProject *ProjectPtr)//執行MES溝通-詢問條碼
{
	return ReturnNotImplement(_T("CMES_Imp::ExecMESComm_AskBarcode"));
}
//-------------------------------------------------------------------------------------//
bool CMES_Imp::ExecMESComm_BoardMapping(CAOIProject *ProjectPtr)//執行MES溝通-單板映射
{
	return ReturnNotImplement(_T("CMES_Imp::ExecMESComm_BoardMapping"));
}
//-------------------------------------------------------------------------------------//
bool CMES_Imp::ExecMESComm_UnCheckTestFile(LANE_ID LaneID, int &Count)//執行MES溝通-未判定檢測檔案數
{
	Count = 0;
	return ReturnNotImplement(_T("CMES_Imp::ExecMESComm_UnCheckTestFile"));
}
//-------------------------------------------------------------------------------------//
size_t CMES_Imp::GetMESRecvNodeCount()
{
	return 0;
}
//-------------------------------------------------------------------------------------//
bool CMES_Imp::ProcesMESRecvNodeList(bool bThread)//處理收到MES的訊息列表
{
	return ReturnNotImplement(_T("CMES_Imp::ProcesMESRecvNodeList"));
}
//-------------------------------------------------------------------------------------//
bool CMES_Imp::SendToMESNode(const char *strBuf, bool bAck, int nPPID, DWORD AckTime)//送資料給MES
{
	return ReturnNotImplement(_T("CMES_Imp::ProcesMESRecvNodeList"));
}
//-------------------------------------------------------------------------------------//
CString CMES_Imp::GetMESMultiLanguage(LPCTSTR String)//取得多國語系
{
	if ( NULL == String ) { return m_ErrorString; }
	return GetMESMultiLanguage(String, String);	
}
//-------------------------------------------------------------------------------------//
CString CMES_Imp::GetMESMultiLanguage(LPCTSTR Key, LPCTSTR String)//取得多國語系
{
	if ( NULL == Key ) { return m_ErrorString; }
	const TCHAR Section[]=_T("MES_BASIC");
	CString strKey=Key;
	CString strDefault=String;
	CString strNew=String;
	AOIDataCollect.GetUILanguageString(Section, strKey, strDefault, strNew);	
	return strNew;
}
//-------------------------------------------------------------------------------------//
bool CMES_Imp::DumpMESDoc(rapidjson::WDocument &Doc, const wchar_t *fnName)//匯出IPS文件
{
	if ( NULL == fnName ) { return true; }
#ifdef _DEBUG
	std::wstring wsPath;	
	rapidjson::CJsonCtrl JSonCtrl;
	std::wstring wsFolder = AOIDataCollect.GetAOITempDirectoryW();
	wsPath = wsFolder + std::wstring(L"\\") + std::wstring(fnName) + std::wstring(L".JSON");		
	JSonCtrl.SaveFile(wsPath.c_str(), Doc);
#endif//_DEBUG
	return true;
}
//-------------------------------------------------------------------------------------//
void CMES_Imp::ClearProjectOpenList()
{
	LockImpProc();
	m_ProjectOpenList.clear();
	UnlockImpProc();
}
//-------------------------------------------------------------------------------------//
void CMES_Imp::AddProjectOpen(const TMES_ProjectOpen &val)
{
	LockImpProc();
	m_ProjectOpenList.push_back(val);
	UnlockImpProc();
}
//-------------------------------------------------------------------------------------//
void CMES_Imp::CloneProjectOpenList(std::vector<TMES_ProjectOpen> &List)
{
	LockImpProc();
	List = m_ProjectOpenList;
	UnlockImpProc();
}
//-------------------------------------------------------------------------------------//
#endif//MES_DISABLE