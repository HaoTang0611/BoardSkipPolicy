// MES_Basic.h: interface for the CMES_Imp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MES_IMP_H__9907CC24_B865_42DD_A8D3_9F528A3D1D27__INCLUDED_)
#define AFX_MES_IMP_H__9907CC24_B865_42DD_A8D3_9F528A3D1D27__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
enum MES_IMP_CLS
{
	MES_IMP_CLS_NONE  = 0,//無使用
	MES_IMP_CLS_ITS   = 1,//DS+JOE的ITS軟體	
	MES_IMP_CLS_IPS   = 2,//宏基的IPS軟體
	MES_IMP_CLS_MTS   = 3,//開發預留的MTS軟體
	MES_IMP_CLS_RETURN
};
//-------------------------------------------------------------------------------------//
typedef struct tagMES_ProjectOpen
{
	CString    sFilename;
	int        nLaneID;
	int        nSide;
	int        nLaneClearType;

	tagMES_ProjectOpen()
	{
		sFilename = _T("");
		nLaneID = 0;
		nSide   = 0;
		nLaneClearType=0;
	}
} TMES_ProjectOpen, *PMES_ProjectOpen;
//-------------------------------------------------------------------------------------//
#ifndef MES_DISABLE
#include "ITSLinker.h"
//-------------------------------------------------------------------------------------//
class CMES_Imp  
{
protected:
	//---------------------------------------------------------------------------------//
	CRITICAL_SECTION           m_csImpProc;//同步化
	CString                    m_ErrorString;
	CITSLinker                 m_MESLinker;//連線元件
	DWORD                      m_MesCommTimeoutMS;//溝通愈時ms	
	bool                       m_FreezeMESFuncMode;//凍結MES函式
	bool                       m_MesRemotCtrlOn;//MES遠端控制
	MES_EQP_CTRL_STATE_MODE    m_MesEqpCtrlStateMode;//MES設備連線模式//Online, Offline, Local
	int                        m_AOIExceptionCodeLast;
	DWORD                      m_AOIExceptionCodeTickCount;
	//---------------------------------------------------------------------------------//	
	std::vector<TMES_ProjectOpen> m_ProjectOpenList;//專案開啟列表
	//---------------------------------------------------------------------------------//
	CMES_Imp(const CMES_Imp &other);
	CMES_Imp& operator=(const CMES_Imp &other);
	//---------------------------------------------------------------------------------//
	void                       PreInitMES();
	void                       InitialMES();
	void                       CloneMES(const CMES_Imp &other);
	//---------------------------------------------------------------------------------//
	bool                       ReturnNotImplement(LPCTSTR fnName);//回傳未完成函式
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	CMES_Imp();
	virtual ~CMES_Imp();
	//---------------------------------------------------------------------------------//	
	void                       LockImpProc();//鎖住MES執行緒同步化
	void                       UnlockImpProc();//釋放MES執行緒同步化
	//---------------------------------------------------------------------------------//	
	virtual MES_IMP_CLS        GetMesImpCls() const=0;//取得衍生類別的編號
	//---------------------------------------------------------------------------------//
	LPCTSTR                    GetErrorString() const;//取得錯誤訊息
	void                       SetErrorString(const char *String);//設定錯誤訊息
	void                       SetErrorString(const wchar_t *String);//設定錯誤訊息
	//---------------------------------------------------------------------------------//	
	DWORD                      GetMesCommTimeoutMS() const;//取得溝通愈時ms
	void                       SetMesCommTimeoutMS(DWORD val);//設定溝通愈時ms
	//---------------------------------------------------------------------------------//
	bool                       GetFreezeMESFuncMode() const;//取得凍結MES函式
	void                       SetFreezeMESFuncMode(bool val);//設定凍結MES函式
	bool                       WaitForFreezeMESFuncModeDone();//等待凍結MES函式結束
	//---------------------------------------------------------------------------------//
	bool                       GetMesRemotCtrlOn() const;//取得MES遠端控制
	void                       SetMesRemotCtrlOn(bool val);//設定MES遠端控制
	//---------------------------------------------------------------------------------//	
	void                       ResetMesEqpCtrlStateMode();//復歸MES設備連線模式//None
	MES_EQP_CTRL_STATE_MODE    GetMesEqpCtrlStateMode() const;//取得MES設備連線模式//Online, Offline, Local
	void                       SetMesEqpCtrlStateMode(MES_EQP_CTRL_STATE_MODE val);//設定MES設備連線模式//Online, Offline, Local
	//---------------------------------------------------------------------------------//
	LANE_ID                    GetActiveLaneID();
	CAOIProject*               GetActiveProject();
	//---------------------------------------------------------------------------------//
	TASK_STATE_MODE            GetOnlineTaskState() const;
	void                       SetOnlineTaskState(TASK_STATE_MODE val);
	//---------------------------------------------------------------------------------//
	int                        GetAOIExceptionCode() const;
	CString                    GetAOIExceptionText() const;
	//---------------------------------------------------------------------------------//
	void                       ResetAOIExceptionCode();
	bool                       CheckBypassAOIExceptionCode();//跳過重複異常碼
	//---------------------------------------------------------------------------------//
	bool                       CheckSystemReady(bool bChkStartLight, bool bAutoReset);//確認系統是否正常
	//---------------------------------------------------------------------------------//
	bool                       SendMainFrameWndMessage(UINT message, WPARAM wParam, LPARAM lParam);//送訊息給回傳主框架視窗 
	bool                       PostMainFrameWndMessage(UINT message, WPARAM wParam, LPARAM lParam);//送訊息給回傳主框架視窗
	//---------------------------------------------------------------------------------//
	virtual bool               CreateMESProcThread();//建立MES執行執行緒
	virtual bool               DeleteMESProcThread();//刪除MES執行執行緒
	//---------------------------------------------------------------------------------//
	CITSLinker&                GetMESLinker();//取得MES連線參考
	virtual bool               GetMESConnected();//是否已經連線
	virtual bool               ConnectMESLinker();//連線到MES連結軟體
	virtual bool               DisconnectMESLinker();//停止連線到MES連結軟體
	//---------------------------------------------------------------------------------//
	virtual bool               ExecMESComm_CheckMESReady();//執行MES溝通-確認MES就緒
	virtual bool               ExecMESComm_SetSystemParam();//執行MES溝通-設定系統參數
	virtual bool               ExecMESComm_SetProjectParam(CAOIProject *ProjectPtr);//執行MES溝通-設定專案參數
	virtual bool               ExecMESComm_SetProjectLoadFinished(CAOIProject *ProjectPtr, bool bSucc, LPCTSTR Err);//執行MES溝通-專案載入完畢
	virtual bool               ExecMESComm_UploadProjectFile(LPCTSTR filename);//執行MES溝通-上傳專案檔案
	virtual bool               ExecMESComm_DownloadProjectFile(LPCTSTR filename);//執行MES溝通-下載專案檔案
	virtual bool               ExecMESComm_SetMachineStatus();//執行MES溝通-設定機台狀態
	virtual bool               ExecMESComm_SetMachineStatus(ONLINE_STATE_MODE Status);//執行MES溝通-設定機台狀態		
	virtual bool               ExecMESComm_ProcessID(int ProcessID);//執行MES溝通-程序運作
	virtual bool               ExecMESComm_SetAOIExceptionCode(LPCTSTR ErrStr);//執行MES溝通-系統異常碼
	virtual bool               ExecMESComm_SetUserLogin_out(bool bLogin);//執行MES溝通-設定使用者登入	
	virtual bool               ExecMESComm_SetControlStateMode(MES_EQP_CTRL_STATE_MODE Mode);//執行MES溝通-設定控制狀態模式	
	virtual bool               ExecMESComm_CheckBarcode(CAOIProject *ProjectPtr);//執行MES溝通-確認條碼
	virtual bool               ExecMESComm_AskBarcode(CAOIProject *ProjectPtr);//執行MES溝通-詢問條碼
	virtual bool               ExecMESComm_BoardMapping(CAOIProject *ProjectPtr);//執行MES溝通-單板映射
	virtual bool               ExecMESComm_UnCheckTestFile(LANE_ID LaneID, int &Count);//執行MES溝通-未判定檢測檔案數
	//---------------------------------------------------------------------------------//
	virtual size_t             GetMESRecvNodeCount();	
	virtual bool               ProcesMESRecvNodeList(bool bThread);//處理收到MES的訊息列表
	//---------------------------------------------------------------------------------//
	virtual bool               SendToMESNode(const char *strBuf, bool bAck, int nPPID, DWORD AckTime);//送資料給MES
	//---------------------------------------------------------------------------------//
	CString                    GetMESMultiLanguage(LPCTSTR String);//取得多國語系
	CString                    GetMESMultiLanguage(LPCTSTR Key, LPCTSTR String);//取得多國語系
	//---------------------------------------------------------------------------------//
	bool                       DumpMESDoc(rapidjson::WDocument &Doc, const wchar_t *fnName);//匯出IPS文件
	//---------------------------------------------------------------------------------//	
	void                       ClearProjectOpenList();
	void                       AddProjectOpen(const TMES_ProjectOpen &val);
	void                       CloneProjectOpenList(std::vector<TMES_ProjectOpen> &List);
	//---------------------------------------------------------------------------------//	
};
//-------------------------------------------------------------------------------------//
#endif//MES_DISABLE
#endif // !defined(AFX_MES_IMP_H__9907CC24_B865_42DD_A8D3_9F528A3D1D27__INCLUDED_)
