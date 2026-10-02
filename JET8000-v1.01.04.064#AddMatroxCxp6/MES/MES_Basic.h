// MES_Basic.h: interface for the CMES_Basic class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MES_BASIC_H__9907CC24_B865_42DD_A8D3_9F528A3D1D27__INCLUDED_)
#define AFX_MES_BASIC_H__9907CC24_B865_42DD_A8D3_9F528A3D1D27__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include "MES_Imp.h"
//-------------------------------------------------------------------------------------//
#ifndef MES_DISABLE
//-------------------------------------------------------------------------------------//
class CMES_Basic  
{
protected:
	//---------------------------------------------------------------------------------//
	CRITICAL_SECTION           m_csMESProc;//同步化
	CString                    m_ErrorString;
	CString                    m_ErrorStringOut;
	CITSLinker                 m_MESLinkerTemp;//連線元件	
	//---------------------------------------------------------------------------------//
	CMES_Basic(const CMES_Basic &other);
	CMES_Basic& operator=(const CMES_Basic &other);
	//---------------------------------------------------------------------------------//
	void                       PreInitMES();
	void                       InitialMES();
	void                       CloneMES(const CMES_Basic &other);
	//---------------------------------------------------------------------------------//
	CMES_Imp                  *_Imp;//實際執行類別指標
	bool                       CheckImp();//確認執行類別指標	
	bool                       ReleaseImp();//釋放執行類別指標
	bool                       CreateImpCls(MES_IMP_CLS Cls);//建立執行類別指標
	MES_IMP_CLS                MapMesImpCls(int nClass) const;//映射執行類別
	//---------------------------------------------------------------------------------//
	void                       SetErrorString(LPCTSTR Str);
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	CMES_Basic();
	virtual ~CMES_Basic();
	//---------------------------------------------------------------------------------//	
	void                       LockMESProc();//鎖住MES執行緒同步化
	void                       UnlockMESProc();//釋放MES執行緒同步化
	//---------------------------------------------------------------------------------//	
	MES_IMP_CLS                GetMesImpCls();//取得MES的實現指標類別
	bool                       CreateMesImpToContact(int nContact);//建立MES-To對接軟體
	//---------------------------------------------------------------------------------//
	void                       SetMESExceptionCode(DWORD Code, LPCTSTR Err=NULL);
	void                       SetMESExceptionCode_FileRead(LPCTSTR Err=NULL);//設定系統錯誤代碼-MES-檔案讀取
	void                       SetMESExceptionCode_FileWrite(LPCTSTR Err=NULL);//設定系統錯誤代碼-MES-檔案寫入
	//---------------------------------------------------------------------------------//
	LPCTSTR                    GetErrorString();//取得錯誤訊息
	//---------------------------------------------------------------------------------//	
	DWORD                      GetMesCommTimeoutMS();//取得溝通愈時ms
	void                       SetMesCommTimeoutMS(DWORD val);//設定溝通愈時ms
	//---------------------------------------------------------------------------------//	
	bool                       GetMesRemotCtrlOn();//取得MES遠端控制		
	void                       ResetMesEqpCtrlStateMode();//復歸MES設備連線模式//None
	MES_EQP_CTRL_STATE_MODE    GetMesEqpCtrlStateMode();//取得MES設備連線模式//Online, Offline, Local	
	//---------------------------------------------------------------------------------//	
	bool                       CreateMESProcThread();//建立MES執行執行緒
	bool                       DeleteMESProcThread();//刪除MES執行執行緒
	//---------------------------------------------------------------------------------//
	CITSLinker&                GetMESLinker();//取得MES連線參考
	bool                       GetMESConnected();//是否已經連線
	bool                       ConnectMESLinker();//連線到MES連結軟體
	bool                       DisconnectMESLinker();//停止連線到MES連結軟體
	//---------------------------------------------------------------------------------//
	bool                       ExecMESComm_CheckMESReady();//執行MES溝通-確認MES就緒
	bool                       ExecMESComm_SetSystemParam();//執行MES溝通-設定系統參數
	bool                       ExecMESComm_SetProjectParam(CAOIProject *ProjectPtr);//執行MES溝通-設定專案參數
	bool                       ExecMESComm_SetProjectLoadFinished(CAOIProject *ProjectPtr, bool bSucc, LPCTSTR Err);//執行MES溝通-專案載入完畢
	bool                       ExecMESComm_UploadProjectFile(LPCTSTR filename);//執行MES溝通-上傳專案檔案
	bool                       ExecMESComm_DownloadProjectFile(LPCTSTR filename);//執行MES溝通-下載專案檔案	
	bool                       ExecMESComm_SetMachineStatus();//執行MES溝通-設定機台狀態
	bool                       ExecMESComm_SetMachineStatus(ONLINE_STATE_MODE Status);//執行MES溝通-設定機台狀態		
	bool                       ExecMESComm_ProcessID(int ProcessID);//執行MES溝通-程序運作
	bool                       ExecMESComm_SetAOIExceptionCode(LPCTSTR ErrStr);//執行MES溝通-系統異常碼
	bool                       ExecMESComm_SetUserLogin_out(bool bLogin);//執行MES溝通-設定使用者登入	
	bool                       ExecMESComm_SetControlStateMode(MES_EQP_CTRL_STATE_MODE Mode);//執行MES溝通-設定控制狀態模式	
	bool                       ExecMESComm_CheckBarcode(CAOIProject *ProjectPtr);//執行MES溝通-確認條碼
	bool                       ExecMESComm_AskBarcode(CAOIProject *ProjectPtr);//執行MES溝通-詢問條碼
	bool                       ExecMESComm_BoardMapping(CAOIProject *ProjectPtr);//執行MES溝通-單板映射
	bool                       ExecMESComm_UnCheckTestFile(LANE_ID LaneID, int &Count);//執行MES溝通-未判定檢測檔案數
	//---------------------------------------------------------------------------------//
	size_t                     GetMESRecvNodeCount();	
	bool                       ProcesMESRecvNodeList(bool bThread);//處理收到MES的訊息列表
	//---------------------------------------------------------------------------------//
	bool                       SendToMESNode(const char *strBuf, bool bAck, int nPPID, DWORD AckTime);//送資料給MES
	//---------------------------------------------------------------------------------//		
	void                       ClearProjectOpenList();	
	void                       CloneProjectOpenList(std::vector<TMES_ProjectOpen> &List);
	//---------------------------------------------------------------------------------//
};
//-------------------------------------------------------------------------------------//
extern CMES_Basic MES_OBJ;
//-------------------------------------------------------------------------------------//
#endif//MES_DISABLE
#endif // !defined(AFX_MES_BASIC_H__9907CC24_B865_42DD_A8D3_9F528A3D1D27__INCLUDED_)
