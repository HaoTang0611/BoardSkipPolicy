// MES_ITS.h: interface for the CMES_ITS class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MES_ITS_H__F700B4AF_0A79_4B04_A4D3_5709E701B77E__INCLUDED_)
#define AFX_MES_ITS_H__F700B4AF_0A79_4B04_A4D3_5709E701B77E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
//#include "MES_Basic.h"
//#include "ITSLinker.h"
//-------------------------------------------------------------------------------------//
#ifndef MES_DISABLE
#ifndef ITS_DISABLE
#include "MES_Imp.h"
#include "MES_ITS_Define.h"
//-------------------------------------------------------------------------------//
class CMES_ITS : public CMES_Imp  
{
private:
	//---------------------------------------------------------------------------------//			
	int                        m_MsgPPID;
	std::string                m_ITSSendBuffer;
	std::vector<TITSCommNode>  m_ITSRecvNodeList;
	//---------------------------------------------------------------------------------//
	THREAD_STATE_MODE          m_ITSProcThreadState;//ITS執行緒狀態
	THREAD_COMMAND_MODE        m_ITSProcThreadCmd;//ITS執行緒命令		
	//---------------------------------------------------------------------------------//		
protected:
	//---------------------------------------------------------------------------------//
	CMES_ITS(const CMES_ITS &other);
	CMES_ITS& operator=(const CMES_ITS &other);
	//---------------------------------------------------------------------------------//
	void                       PreInitITS();
	void                       InitialITS();
	void                       CloneITS(const CMES_ITS &other);
	//---------------------------------------------------------------------------------//
	bool                       CheckProjectPtr(CAOIProject *ProjectPtr);//確認專案指標
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//
	void                       LockITSProc();//鎖住ITS執行緒同步化
	void                       UnlockITSProc();//釋放ITS執行緒同步化
	//---------------------------------------------------------------------------------//	
	CITSLinker&                GetITSLinker();//取得ITS連線參考
	bool                       ConnectITSLinker();//連線到ITS連結軟體
	bool                       DisconnectITSLinker();//停止連線到ITS連結軟體
	//---------------------------------------------------------------------------------//
	int                        GetITSStatusID_Ack(int StatusID);//取得ITS的回應訊息
	bool                       CheckITSStatusID_Ack(int StatusID);//確認ITS的回應訊息
	//--------------------------------------------------------------------------//
	size_t                     GetITSRecvNodeCount();	
	bool                       RecvNewITSRecvNodeList();//接收新的ITS訊息列表
	bool                       AddITSRecvNode(const std::string &sBuff);
	bool                       DecoderITSPacket(const std::string &sBuff, TITSCommNode &sNode);
	bool                       GetITSRecvNode(size_t index, bool bCheck, TITSCommNode &sNode);	
	//--------------------------------------------------------------------------//
	bool                       ProcesITSRecvNodeList(bool bThread);//處理收到ITS的訊息列表
	bool                       ProcesITSRecvNode(bool bThread, TITSCommNode &sNode, bool &bBreak, bool &bRemove);//處理收到ITS的訊息
	//--------------------------------------------------------------------------//
	//ITS軟體
	bool                       ExecITSComm_MesSetParam(bool bThread, TITSCommNode &sNode, bool &bBreak, bool &bRemove);	
	bool                       ExecITSComm_SetSystemParam();//執行ITS溝通-設定系統參數
	bool                       ExecITSComm_SetProjectParam(CAOIProject *ProjectPtr);//執行ITS溝通-設定專案參數
	bool                       ExecITSComm_SetMachineStatus();//執行ITS溝通-設定機台狀態
	bool                       ExecITSComm_SetMachineStatus(ONLINE_STATE_MODE Status);//執行ITS溝通-設定機台狀態	
	bool                       ExecITSComm_ProcessID(int ProcessID);//執行ITS溝通-程序運作
	bool                       ExecITSComm_SetUserLogin_out(bool bLogin);//執行ITS溝通-設定使用者登入	
	bool                       ExecITSComm_CheckBarcode(CAOIProject *ProjectPtr);//執行ITS溝通-確認條碼
	//--------------------------------------------------------------------------//
	int                        GetITSFreePPID();//取得ITS可用的PPID
	bool                       CheckITSAlwaysAckMode();//確認總是回傳模式
	bool                       BuildITSDoc(int nStatus, bool bAck, int AckTime, int &nPPID, rapidjson::WDocument &Doc);//建立ITS文檔
	bool                       BuildITSDoc_SetSystemParam(bool bAck, int AckTime, int &nPPID, std::wstring& wsBuf);//建立系統資訊文檔
	bool                       BuildITSDoc_SetProjectParam(bool bAck, int AckTime, int &nPPID, CAOIProject *ProjectPtr, std::wstring& wsBuf);//建立基本資訊文檔
	bool                       BuildITSDoc_SetMachineStatus(ONLINE_STATE_MODE Status, bool bAck, int AckTime, int &nPPID, std::wstring& wsBuf);//建立機台運作文檔	
	bool                       BuildITSDoc_ProcessID(int ProcessID, bool bAck, int AckTime, int &nPPID, std::wstring& wsBuf);//建立程序運作文檔
	bool                       BuildITSDoc_SetLogin_out(bool bAck, int AckTime, int &nPPID, bool Login, std::wstring& wsBuf);//建立登入參數文檔
	bool                       BuildITSDoc_CheckBarcode(bool bAck, int AckTime, int &nPPID, CAOIProject *ProjectPtr, std::wstring& wsBuf);//建立確認條碼文檔
	bool                       SendToITSNode(const char *strBuf, bool bAck, int nPPID, DWORD AckTime);//送資料給ITS
	bool                       WaitForITSResponse(int nPPID, DWORD AckTime);//等待ITS回傳資料
	//--------------------------------------------------------------------------//	
	CString                    GetITSStatusText(int StatusID);
	int                        MapITSComm_ProcessID(int ProcessID);//映射ProcessID
	//--------------------------------------------------------------------------//	
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	CMES_ITS();
	virtual ~CMES_ITS();
	//---------------------------------------------------------------------------------//	
	MES_IMP_CLS                GetMesImpCls() const { return MES_IMP_CLS_ITS; }
	//---------------------------------------------------------------------------------//	
	bool                       CreateMESProcThread();//建立MES執行執行緒
	bool                       DeleteMESProcThread();//刪除MES執行執行緒
	//---------------------------------------------------------------------------------//
	bool                       GetMESConnected();//是否已經連線
	bool                       ConnectMESLinker();//連線到MES連結軟體
	bool                       DisconnectMESLinker();//停止連線到MES連結軟體
	//---------------------------------------------------------------------------------//
	bool                       ExecMESComm_CheckMESReady();//執行MES溝通-確認MES就緒
	bool                       ExecMESComm_SetSystemParam();//執行MES溝通-設定系統參數
	bool                       ExecMESComm_SetProjectParam(CAOIProject *ProjectPtr);//執行MES溝通-設定專案參數
	bool                       ExecMESComm_SetProjectLoadFinished(CAOIProject *ProjectPtr, bool bSucc, LPCTSTR Err);//執行MES溝通-專案載入完畢
	bool                       ExecMESComm_SetMachineStatus();//執行MES溝通-設定機台狀態
	bool                       ExecMESComm_SetMachineStatus(ONLINE_STATE_MODE Status);//執行MES溝通-設定機台狀態		
	bool                       ExecMESComm_ProcessID(int ProcessID);//執行MES溝通-程序運作
	bool                       ExecMESComm_SetAOIExceptionCode(LPCTSTR ErrStr);//執行MES溝通-系統異常碼
	bool                       ExecMESComm_SetUserLogin_out(bool bLogin);//執行MES溝通-設定使用者登入		
	bool                       ExecMESComm_CheckBarcode(CAOIProject *ProjectPtr);//執行MES溝通-確認條碼
	//---------------------------------------------------------------------------------//
	size_t                     GetMESRecvNodeCount();	
	bool                       ProcesMESRecvNodeList(bool bThread);//處理收到MES的訊息列表
	//---------------------------------------------------------------------------------//
	bool                       SendToMESNode(const char *strBuf, bool bAck, int nPPID, DWORD AckTime);//送資料給MES
	//---------------------------------------------------------------------------------//
	//ITS執行
	bool                       ExecITSProcFn();                      //執行ITS執行執行緒
	bool                       CreateITSProcThread();                //建立ITS執行執行緒
	bool                       DeleteITSProcThread();                //刪除ITS執行執行緒
	bool                       StopITSProcThread(bool WaitOn);      //開始ITS執行執行緒
	bool                       StartITSProcThread(bool WaitOn);      //開始ITS執行執行緒	
	bool                       WaitForITSProcThreadIdle();           //等待ITS執行執行緒閒置
	bool                       WaitForITSProcThreadStop();           //等待ITS執行執行緒停止
	bool                       WaitForITSProcThreadStart();          //等待ITS執行執行緒開始
	bool                       WaitForITSProcThreadFinish();         //等待ITS執行執行緒結束
	bool                       CheckITSProcThreadState(THREAD_STATE_MODE State);//確認ITS執行執行緒狀態
	void                       SetITSProcThreadState(THREAD_STATE_MODE State);//設定ITS執行執行緒狀態
	THREAD_STATE_MODE          GetITSProcThreadState();              //取得ITS執行執行緒狀態
	void                       SetITSProcThreadCmd(THREAD_COMMAND_MODE Cmd); //設定ITS執行執行緒命令
	THREAD_COMMAND_MODE        GetITSProcThreadCmd();	               //取得ITS執行執行緒命令
	//---------------------------------------------------------------------------------//	
};
//-------------------------------------------------------------------------------------//
#endif//ITS_DISABLE
#endif//MES_DISABLE
#endif // !defined(AFX_MES_ITS_H__F700B4AF_0A79_4B04_A4D3_5709E701B77E__INCLUDED_)
