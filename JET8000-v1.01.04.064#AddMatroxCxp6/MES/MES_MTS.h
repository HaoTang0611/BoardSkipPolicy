// MES_MTS.h: interface for the CMES_MTS class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MES_MTS_H__C771C11C_38E0_409F_88D8_26FFC1584F1E__INCLUDED_)
#define AFX_MES_MTS_H__C771C11C_38E0_409F_88D8_26FFC1584F1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
//#include "MES_Basic.h"
//-------------------------------------------------------------------------------------//
#ifndef MES_DISABLE
#ifndef MTS_DISABLE
#include "MES_Imp.h"
#include "MES_MTS_Define.h"
//-------------------------------------------------------------------------------//
class CMES_MTS : public CMES_Imp
{
private:
	//---------------------------------------------------------------------------------//
	int                        m_MsgPPID;	
	//---------------------------------------------------------------------------------//
	CString                    m_MTSShareFolder;//共用資料夾
	std::string                m_MTSSendBuffer;
	std::vector<TITSCommNode>  m_MTSRecvNodeList;
	//---------------------------------------------------------------------------------//
	THREAD_STATE_MODE          m_MTSProcThreadState;//MTS執行緒狀態
	THREAD_COMMAND_MODE        m_MTSProcThreadCmd;//MTS執行緒命令		
	//---------------------------------------------------------------------------------//		
protected:
	//---------------------------------------------------------------------------------//
	CMES_MTS(const CMES_MTS &other);
	CMES_MTS& operator=(const CMES_MTS &other);
	//---------------------------------------------------------------------------------//
	void                       PreInitMTS();
	void                       InitialMTS();
	void                       CloneMTS(const CMES_MTS &other);
	//---------------------------------------------------------------------------------//
	void                       LockMTSProc();//鎖住MTS執行緒同步化
	void                       UnlockMTSProc();//釋放MTS執行緒同步化
	//---------------------------------------------------------------------------------//
	bool                       ConnectMTSLinker();//連線到MTS連結軟體
	bool                       DisconnectMTSLinker();//停止連線到MTS連結軟體
	//---------------------------------------------------------------------------------//
	int                        GetMTSStatusID_Ack(int StatusID);//取得MTS的回應訊息
	bool                       CheckMTSStatusID_Ack(int StatusID);//確認MTS的回應訊息
	//--------------------------------------------------------------------------//
	size_t                     GetMTSRecvNodeCount();	
	bool                       RecvNewMTSRecvNodeList();//接收新的MTS訊息列表
	bool                       AddMTSRecvNode(const std::string &sBuff);
	bool                       DecoderMTSPacket(const std::string &sBuff, TITSCommNode &sNode);
	bool                       GetMTSRecvNode(size_t index, bool bCheck, TITSCommNode &sNode);	
	//--------------------------------------------------------------------------//
	bool                       ProcesMTSRecvNodeList(bool bThread);//處理收到MTS的訊息列表
	bool                       ProcesMTSRecvNode(bool bThread, TITSCommNode &sNode, bool &bBreak, bool &bRemove);//處理收到MTS的訊息
	//--------------------------------------------------------------------------//
	//MTS軟體
	bool                       ExecMTSComm_MesSetParam(bool bThread, TITSCommNode &sNode, bool &bBreak, bool &bRemove);	
	bool                       ExecMTSComm_SetSystemParam();//執行MTS溝通-設定系統參數
	bool                       ExecMTSComm_SetProjectParam();//執行MTS溝通-設定專案參數
	bool                       ExecMTSComm_SetMachineStatus();//執行MTS溝通-設定機台狀態
	bool                       ExecMTSComm_SetMachineStatus(ONLINE_STATE_MODE Status);//執行MTS溝通-設定機台狀態	
	bool                       ExecMTSComm_ProcessID(int ProcessID);//執行MTS溝通-程序運作
	//--------------------------------------------------------------------------//
	int                        GetMTSFreePPID();//取得MTS可用的PPID
	bool                       BuildMTSDoc(int nStatus, bool bAck, int AckTime, int &nPPID, rapidjson::WDocument &Doc);//建立MTS文檔
	bool                       BuildMTSDoc_SetSystemParam(bool bAck, int AckTime, int &nPPID, std::wstring& wsBuf);//建立系統資訊文檔
	bool                       BuildMTSDoc_SetProjectParam(bool bAck, int AckTime, int &nPPID, std::wstring& wsBuf);//建立基本資訊文檔
	bool                       BuildMTSDoc_SetMachineStatus(ONLINE_STATE_MODE Status, bool bAck, int AckTime, int &nPPID, std::wstring& wsBuf);//建立機台運作文檔	
	bool                       BuildMTSDoc_ProcessID(int ProcessID, bool bAck, int AckTime, int &nPPID, std::wstring& wsBuf);//建立程序運作文檔
	bool                       SendToMTSNode(const char *strBuf, bool bAck, int nPPID, DWORD AckTime);//送資料給MTS
	bool                       WaitForMTSResponse(int nPPID, DWORD AckTime);//等待MTS回傳資料
	//--------------------------------------------------------------------------//	
	CString                    GetMTSStatusText(int StatusID);
	int                        MapMTSComm_ProcessID(int ProcessID);//映射ProcessID
	//--------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	CMES_MTS();
	virtual ~CMES_MTS();
	//---------------------------------------------------------------------------------//
	MES_IMP_CLS                GetMesImpCls() const { return MES_IMP_CLS_MTS; }
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
	//MTS執行
	bool                       ExecMTSProcFn();                      //執行MTS執行執行緒
	bool                       CreateMTSProcThread();                //建立MTS執行執行緒
	bool                       DeleteMTSProcThread();                //刪除MTS執行執行緒
	bool                       StopMTSProcThread(bool WaitOn);      //開始MTS執行執行緒
	bool                       StartMTSProcThread(bool WaitOn);      //開始MTS執行執行緒	
	bool                       WaitForMTSProcThreadIdle();           //等待MTS執行執行緒閒置
	bool                       WaitForMTSProcThreadStop();           //等待MTS執行執行緒停止
	bool                       WaitForMTSProcThreadStart();          //等待MTS執行執行緒開始
	bool                       WaitForMTSProcThreadFinish();         //等待MTS執行執行緒結束
	bool                       CheckMTSProcThreadState(THREAD_STATE_MODE State);//確認MTS執行執行緒狀態
	void                       SetMTSProcThreadState(THREAD_STATE_MODE State);//設定MTS執行執行緒狀態
	THREAD_STATE_MODE          GetMTSProcThreadState();              //取得MTS執行執行緒狀態
	void                       SetMTSProcThreadCmd(THREAD_COMMAND_MODE Cmd); //設定MTS執行執行緒命令
	THREAD_COMMAND_MODE        GetMTSProcThreadCmd();	               //取得MTS執行執行緒命令
	//---------------------------------------------------------------------------------//	
};
//-------------------------------------------------------------------------------------//
extern CMES_MTS MES_MTS;
//-------------------------------------------------------------------------------------//
#endif//MTS_DISABLE
#endif//MES_DISABLE
#endif // !defined(AFX_MES_MTS_H__C771C11C_38E0_409F_88D8_26FFC1584F1E__INCLUDED_)
