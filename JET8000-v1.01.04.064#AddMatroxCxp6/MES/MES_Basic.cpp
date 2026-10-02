// MES_Basic.cpp: implementation of the CMES_Basic class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "MES_Basic.h"
//-------------------------------------------------------------------------------------//
#include "MES_ITS.h"
#include "MES_MTS.h"
#include "MES_IPS.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
#ifndef MES_DISABLE
CMES_Basic MES_OBJ;
//-------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
CMES_Basic::CMES_Basic()
{
	::InitializeCriticalSection(&m_csMESProc);
	PreInitMES();
	InitialMES();
}
//-------------------------------------------------------------------------------------//
CMES_Basic::~CMES_Basic()
{
	ReleaseImp();	
	::DeleteCriticalSection(&m_csMESProc);
}
//-------------------------------------------------------------------------------------//
void CMES_Basic::PreInitMES()
{
	_Imp = NULL;
	CreateMesImpToContact(MES_CONTACT_TYPE);
}
//-------------------------------------------------------------------------------------//
void CMES_Basic::InitialMES()
{	
}
//-------------------------------------------------------------------------------------//
void CMES_Basic::CloneMES(const CMES_Basic &other)
{	
}
//-------------------------------------------------------------------------------------//
bool CMES_Basic::CheckImp()
{
	if ( NULL == _Imp )
	{
		m_ErrorString=_T("Error, CMES_BASIC _Imp is NULL");
		SetMESExceptionCode(AOI_EXCEPTION_MES_CREATE);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_Basic::ReleaseImp()//釋放執行類別指標
{
	if ( NULL == _Imp ) { return true; }
	delete _Imp;
	_Imp = NULL;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_Basic::CreateImpCls(MES_IMP_CLS Cls)//建立執行類別指標
{
	try 
	{	
		switch ( Cls )
		{
	#ifndef ITS_DISABLE
		case MES_IMP_CLS_ITS: _Imp=new CMES_ITS(); break;
	#endif//ITS_DISABLE

	#ifndef MTS_DISABLE
		case MES_IMP_CLS_MTS: _Imp=new CMES_MTS(); break;
	#endif//MTS_DISABLE

	#ifndef IPS_DISABLE
		case MES_IMP_CLS_IPS: _Imp=new CMES_IPS(); break;
	#endif//IPS_DISABLE

		default:
			_Imp = NULL;
			break;
		}		
	}
	catch (...)//無法測試是否可以正常攔截到例外訊息
	{
		CString ClsName;
		switch ( Cls )
		{	
		case MES_IMP_CLS_ITS: ClsName=_T("CMES_ITS"); break;
		case MES_IMP_CLS_MTS: ClsName=_T("CMES_MTS"); break;
		case MES_IMP_CLS_IPS: ClsName=_T("CMES_IPS"); break;
		default:
			ClsName.Format(_T("Undefine[%d]"), Cls);
			break;
		}
		m_ErrorString.Format(_T("Error, CMES_Basic::CreateImpCls Fault [%s]"), ClsName);
		SetMESExceptionCode(AOI_EXCEPTION_MES_CREATE);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
MES_IMP_CLS CMES_Basic::MapMesImpCls(int nClass) const//映射執行類別
{
	MES_IMP_CLS Cls;
	switch ( nClass )
	{
	case MES_CONTACT_IPS: Cls=MES_IMP_CLS_IPS;	break;
	case MES_CONTACT_MTS: Cls=MES_IMP_CLS_MTS;	break;		
	case MES_CONTACT_IBS: Cls=MES_IMP_CLS_ITS;	break;
	default:
		Cls = MES_IMP_CLS_NONE;
		break;
	}
	return Cls;
}
//-------------------------------------------------------------------------------------//
void CMES_Basic::SetErrorString(LPCTSTR Str)
{
	m_ErrorString = Str;
}
//-------------------------------------------------------------------------------------//
void CMES_Basic::LockMESProc()//鎖住MES執行緒同步化
{
	::EnterCriticalSection(&m_csMESProc);
}
//-------------------------------------------------------------------------------------//
void CMES_Basic::UnlockMESProc()//釋放MES執行緒同步化
{
	::LeaveCriticalSection(&m_csMESProc);
}
//-------------------------------------------------------------------------------------//
MES_IMP_CLS CMES_Basic::GetMesImpCls()
{
	if ( CheckImp() == false ) { return MES_IMP_CLS_NONE;}
	return _Imp->GetMesImpCls();
}
//-------------------------------------------------------------------------------------//
bool CMES_Basic::CreateMesImpToContact(int nContact)//建立MES-To對接軟體
{	
	const MES_IMP_CLS eCls=GetMesImpCls();
	const MES_IMP_CLS nCls=MapMesImpCls(nContact);
	if ( nCls == eCls ) { return true; }

	ReleaseImp();
	if ( CreateImpCls(nCls) == false )
	{	return false; }
	if ( CreateMESProcThread() == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
void CMES_Basic::SetMESExceptionCode(DWORD Code, LPCTSTR Err)
{
	CString str = (NULL!=Err) ? Err:m_ErrorString;
	AOIExceptionCodeCtrl.SetAOIExceptionCode_MES(Code, str);
}
//-------------------------------------------------------------------------------------//
void CMES_Basic::SetMESExceptionCode_FileRead(LPCTSTR Err)//設定系統錯誤代碼-MES-檔案讀取
{
	CString str = (NULL!=Err) ? Err:m_ErrorString;	
	AOIExceptionCodeCtrl.SetAOIExceptionCode_FileRead(str);
	//AOIExceptionCodeCtrl.SetAOIExceptionCode_MES(AOI_EXCEPTION_MES_FILE_READ, str);	
	return;
}
//-------------------------------------------------------------------------------------//
void CMES_Basic::SetMESExceptionCode_FileWrite(LPCTSTR Err)//設定系統錯誤代碼-MES-檔案寫入
{
	CString str = (NULL!=Err) ? Err:m_ErrorString;
	AOIExceptionCodeCtrl.SetAOIExceptionCode_FileWrite(str);
	//AOIExceptionCodeCtrl.SetAOIExceptionCode_MES(AOI_EXCEPTION_MES_FILE_WRITE, str);	
	return;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CMES_Basic::GetErrorString()//取得錯誤訊息
{
	if ( CheckImp() == true )
	{	m_ErrorString = _Imp->GetErrorString();		}
	m_ErrorStringOut=JetAPI::AddKeyToErrorString(m_ErrorString, _T("[MES]"));
	AOIExceptionCodeCtrl.SetAOIExceptionCode_MES_Others(m_ErrorStringOut);
	return m_ErrorStringOut;
}
//-------------------------------------------------------------------------------------//
DWORD CMES_Basic::GetMesCommTimeoutMS()//取得溝通愈時ms
{
	if ( CheckImp() == false ) { return 0; }
	return _Imp->GetMesCommTimeoutMS();
}
//-------------------------------------------------------------------------------------//
void CMES_Basic::SetMesCommTimeoutMS(DWORD val)//設定溝通愈時ms
{
	if ( CheckImp() == false ) { return ; }
	return _Imp->SetMesCommTimeoutMS(val);	
}
//-------------------------------------------------------------------------------------//
bool CMES_Basic::GetMesRemotCtrlOn()//取得MES遠端控制
{
	if ( CheckImp() == false ) { return false; }
	return _Imp->GetMesRemotCtrlOn();	
}
//-------------------------------------------------------------------------------------//
void CMES_Basic::ResetMesEqpCtrlStateMode()//復歸MES設備連線模式//None
{
	if ( CheckImp() == false ) { return ; }
	_Imp->ResetMesEqpCtrlStateMode();	
	return;
}
//-------------------------------------------------------------------------------------//
MES_EQP_CTRL_STATE_MODE CMES_Basic::GetMesEqpCtrlStateMode()//取得MES設備連線模式//Online, Offline, Local
{
	if ( CheckImp() == false ) { return MES_EQP_CTRL_STATE_NONE; }
	return _Imp->GetMesEqpCtrlStateMode();	
}
//-------------------------------------------------------------------------------------//
bool CMES_Basic::CreateMESProcThread()//建立MES執行執行緒
{
	if ( CheckImp() == false ) { return false; }	
	return _Imp->CreateMESProcThread();	
}
//-------------------------------------------------------------------------------------//
bool CMES_Basic::DeleteMESProcThread()//刪除MES執行執行緒
{
	if ( CheckImp() == false ) { return false; }	
	return _Imp->DeleteMESProcThread();	
}
//-------------------------------------------------------------------------------------//
CITSLinker& CMES_Basic::GetMESLinker()//取得MES連線參考
{
	if ( CheckImp() == false ) { return m_MESLinkerTemp; }
	return _Imp->GetMESLinker();
}
//-------------------------------------------------------------------------------------//
bool CMES_Basic::GetMESConnected()//是否已經連線
{
	if ( CheckImp() == false ) { return false; }	
	return _Imp->GetMESConnected();
}
//-------------------------------------------------------------------------------------//
bool CMES_Basic::ConnectMESLinker()
{
	if ( CheckImp() == false ) { return false; }	
	if ( _Imp->ConnectMESLinker() == false )
	{			
		SetErrorString(_Imp->GetErrorString());
		SetMESExceptionCode(AOI_EXCEPTION_MES_CONNECT);	
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_Basic::DisconnectMESLinker()//停止連線到MES連結軟體
{
	if ( CheckImp() == false ) { return false; }
	if ( _Imp->DisconnectMESLinker() == false )
	{
		SetErrorString(_Imp->GetErrorString());
		SetMESExceptionCode(AOI_EXCEPTION_MES_DISCONNECT);	
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMES_Basic::ExecMESComm_CheckMESReady()//執行MES溝通-確認MES就緒
{
	bool IsOK=true;	
	if ( CheckImp() == false ) { return false; }
	LockMESProc();
	IsOK=_Imp->ExecMESComm_CheckMESReady();
	if ( false == IsOK )
	{
		SetErrorString(_Imp->GetErrorString());
		SetMESExceptionCode(AOI_EXCEPTION_MES_GET_MES_READY); 
	}
	UnlockMESProc();
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CMES_Basic::ExecMESComm_SetSystemParam()//執行MES溝通-設定系統參數
{
	bool IsOK=true;	
	if ( CheckImp() == false ) { return false; }
	LockMESProc();
	IsOK=_Imp->ExecMESComm_SetSystemParam();
	if ( false == IsOK )
	{
		SetErrorString(_Imp->GetErrorString());
		SetMESExceptionCode(AOI_EXCEPTION_MES_SET_SYSTEM_PARAM); 
	}
	UnlockMESProc();
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CMES_Basic::ExecMESComm_SetProjectParam(CAOIProject *ProjectPtr)//執行MES溝通-設定專案參數
{
	bool IsOK=true;
	if ( CheckImp() == false ) { return false; }	
	LockMESProc();
	IsOK=_Imp->ExecMESComm_SetProjectParam(ProjectPtr);
	if ( false == IsOK )
	{
		SetErrorString(_Imp->GetErrorString());
		SetMESExceptionCode(AOI_EXCEPTION_MES_SET_PROJECT_PARAM); 
	}
	UnlockMESProc();
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CMES_Basic::ExecMESComm_SetProjectLoadFinished(CAOIProject *ProjectPtr, bool bSucc, LPCTSTR Err)//執行MES溝通-專案載入完畢
{
	bool IsOK=true;
	if ( CheckImp() == false ) { return false; }	
	LockMESProc();
	IsOK=_Imp->ExecMESComm_SetProjectLoadFinished(ProjectPtr, bSucc, Err);
	if ( false == IsOK )
	{
		SetErrorString(_Imp->GetErrorString());
		SetMESExceptionCode(AOI_EXCEPTION_MES_SET_FUNC); 
	}
	UnlockMESProc();
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CMES_Basic::ExecMESComm_UploadProjectFile(LPCTSTR filename)//執行MES溝通-上傳專案檔案
{
	bool IsOK=true;
	if ( CheckImp() == false ) { return false; }	
	LockMESProc();
	IsOK=_Imp->ExecMESComm_UploadProjectFile(filename);	
	if ( false == IsOK )
	{
		SetErrorString(_Imp->GetErrorString());
		SetMESExceptionCode(AOI_EXCEPTION_MES_UPLOAD_FUNC); 
	}
	UnlockMESProc();
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CMES_Basic::ExecMESComm_DownloadProjectFile(LPCTSTR filename)//執行MES溝通-下載專案檔案	
{
	bool IsOK=true;
	if ( CheckImp() == false ) { return false; }	
	LockMESProc();
	IsOK=_Imp->ExecMESComm_DownloadProjectFile(filename);	
	if ( false == IsOK )
	{
		SetErrorString(_Imp->GetErrorString());
		SetMESExceptionCode(AOI_EXCEPTION_MES_DOWNLOAD_FUNC); 
	}
	UnlockMESProc();
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CMES_Basic::ExecMESComm_SetMachineStatus()//執行MES溝通-設定機台狀態
{
	bool IsOK=true;
	if ( CheckImp() == false ) { return false; }
	LockMESProc();
	IsOK=_Imp->ExecMESComm_SetMachineStatus();
	if ( false == IsOK )
	{
		SetErrorString(_Imp->GetErrorString());
		SetMESExceptionCode(AOI_EXCEPTION_MES_SET_MACHINE_STATUS); 
	}
	UnlockMESProc();
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CMES_Basic::ExecMESComm_SetMachineStatus(ONLINE_STATE_MODE Status)//執行MES溝通-設定機台狀態	
{
	bool IsOK=true;
	if ( CheckImp() == false ) { return false; }
	LockMESProc();
	IsOK=_Imp->ExecMESComm_SetMachineStatus(Status);
	if ( false == IsOK )
	{
		SetErrorString(_Imp->GetErrorString());
		SetMESExceptionCode(AOI_EXCEPTION_MES_SET_MACHINE_STATUS); 
	}
	UnlockMESProc();
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CMES_Basic::ExecMESComm_ProcessID(int ProcessID)//執行MES溝通-程序運作
{
	bool IsOK=true;
	if ( CheckImp() == false ) { return false; }
	LockMESProc();
	IsOK=_Imp->ExecMESComm_ProcessID(ProcessID);
	if ( false == IsOK )
	{
		SetErrorString(_Imp->GetErrorString());
		SetMESExceptionCode(AOI_EXCEPTION_MES_SET_PROCESS_ID); 
	}
	UnlockMESProc();
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CMES_Basic::ExecMESComm_SetAOIExceptionCode(LPCTSTR ErrStr)//執行MES溝通-系統異常碼
{
	bool IsOK=true;	
	if ( CheckImp() == false ) { return false; }	
	LockMESProc();
	IsOK=_Imp->ExecMESComm_SetAOIExceptionCode(ErrStr);	
	if ( false == IsOK )
	{
		SetErrorString(_Imp->GetErrorString());
		SetMESExceptionCode(AOI_EXCEPTION_MES_SET_FUNC); 
	}
	UnlockMESProc();	
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CMES_Basic::ExecMESComm_SetUserLogin_out(bool bLogin)//執行MES溝通-設定使用者登入
{
	bool IsOK=true;
	if ( CheckImp() == false ) { return false; }
	LockMESProc();
	IsOK=_Imp->ExecMESComm_SetUserLogin_out(bLogin);
	if ( false == IsOK )
	{
		SetErrorString(_Imp->GetErrorString());
		SetMESExceptionCode(AOI_EXCEPTION_MES_LOGOUT_LOGIN); 
	}
	UnlockMESProc();
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CMES_Basic::ExecMESComm_SetControlStateMode(MES_EQP_CTRL_STATE_MODE Mode)//執行MES溝通-設定控制狀態模式
{
	bool IsOK=true;	
	if ( CheckImp() == false ) { return false; }	
	LockMESProc();
	IsOK=_Imp->ExecMESComm_SetControlStateMode(Mode);	
	if ( false == IsOK )
	{
		SetErrorString(_Imp->GetErrorString());
		SetMESExceptionCode(AOI_EXCEPTION_MES_SET_CONTROL_STATE); 
	}
	UnlockMESProc();	
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CMES_Basic::ExecMESComm_CheckBarcode(CAOIProject *ProjectPtr)//執行MES溝通-確認條碼
{
	bool IsOK=true;
	if ( CheckImp() == false ) { return false; }
	LockMESProc();
	IsOK=_Imp->ExecMESComm_CheckBarcode(ProjectPtr);
	if ( false == IsOK )
	{
		SetErrorString(_Imp->GetErrorString());
		SetMESExceptionCode(AOI_EXCEPTION_MES_BARCODE_CHECK); 
	}
	UnlockMESProc();
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CMES_Basic::ExecMESComm_AskBarcode(CAOIProject *ProjectPtr)//執行MES溝通-詢問條碼
{
	bool IsOK=true;
	if ( CheckImp() == false ) { return false; }
	LockMESProc();
	IsOK=_Imp->ExecMESComm_AskBarcode(ProjectPtr);
	if ( false == IsOK )
	{
		SetErrorString(_Imp->GetErrorString());
		SetMESExceptionCode(AOI_EXCEPTION_MES_BARCODE_ASK); 
	}
	UnlockMESProc();
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CMES_Basic::ExecMESComm_BoardMapping(CAOIProject *ProjectPtr)//執行MES溝通-單板映射
{
	bool IsOK=true;
	if ( CheckImp() == false ) { return false; }
	LockMESProc();
	IsOK=_Imp->ExecMESComm_BoardMapping(ProjectPtr);	
	if ( false == IsOK )
	{
		SetErrorString(_Imp->GetErrorString());
		SetMESExceptionCode(AOI_EXCEPTION_MES_BOARD_MAPPING); 
	}
	UnlockMESProc();
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CMES_Basic::ExecMESComm_UnCheckTestFile(LANE_ID LaneID, int &Count)//執行MES溝通-未判定檢測檔案數
{
	bool IsOK=true;
	if ( CheckImp() == false ) { return false; }
	LockMESProc();	
	IsOK=_Imp->ExecMESComm_UnCheckTestFile(LaneID, Count);
	if ( false == IsOK )
	{
		SetErrorString(_Imp->GetErrorString());
		SetMESExceptionCode(AOI_EXCEPTION_MES_UNCHECK_TEST_FILE); 
	}
	UnlockMESProc();
	return IsOK;
}
//-------------------------------------------------------------------------------------//
size_t CMES_Basic::GetMESRecvNodeCount()
{
	if ( CheckImp() == false ) { return 0; }	
	return _Imp->GetMESRecvNodeCount();
	return 0;
}
//-------------------------------------------------------------------------------------//
bool CMES_Basic::ProcesMESRecvNodeList(bool bThread)//處理收到MES的訊息列表
{
	bool IsOK=true;
	if ( CheckImp() == false ) { return false; }	
	LockMESProc();
	IsOK=_Imp->ProcesMESRecvNodeList(bThread);
	if ( false == IsOK )
	{
		SetErrorString(_Imp->GetErrorString());
		SetMESExceptionCode(AOI_EXCEPTION_MES_EXEC_FUNC); 
	}
	UnlockMESProc();
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CMES_Basic::SendToMESNode(const char *strBuf, bool bAck, int nPPID, DWORD AckTime)//送資料給MES
{
	bool IsOK=true;
	if ( CheckImp() == false ) { return false; }
	LockMESProc();
	IsOK=_Imp->SendToMESNode(strBuf, bAck, nPPID, AckTime);
	if ( false == IsOK )
	{
		SetErrorString(_Imp->GetErrorString());
		SetMESExceptionCode(AOI_EXCEPTION_MES_SEND); 
	}
	UnlockMESProc();
	return IsOK;
}
//-------------------------------------------------------------------------------------//
void CMES_Basic::ClearProjectOpenList()
{
	if ( CheckImp() == false ) { return ; }
	LockMESProc();
	_Imp->ClearProjectOpenList();	
	UnlockMESProc();
	return ;
}
//-------------------------------------------------------------------------------------//
void CMES_Basic::CloneProjectOpenList(std::vector<TMES_ProjectOpen> &List)
{
	if ( CheckImp() == false ) { return ; }
	LockMESProc();
	_Imp->CloneProjectOpenList(List);
	UnlockMESProc();
	return ;
}
//-------------------------------------------------------------------------------------//
#endif//MES_DISABLE