// MES_IPS.h: interface for the CMES_IPS class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MES_IPS_H__0DA9E560_23BF_465B_8F91_A391E929E9FC__INCLUDED_)
#define AFX_MES_IPS_H__0DA9E560_23BF_465B_8F91_A391E929E9FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#ifndef MES_DISABLE
#ifndef IPS_DISABLE
#include "MES_Imp.h"
#include "MES_IPS_Define.h"
//-------------------------------------------------------------------------------//
class CMES_IPS : public CMES_Imp
{
private:
	//---------------------------------------------------------------------------------//			
	int                        m_MsgPPID;
	std::string                m_IPSSendBuffer;
	std::vector<TIPSCommNode>  m_IPSRecvNodeList;
	//---------------------------------------------------------------------------------//
	THREAD_STATE_MODE          m_IPSProcThreadState;//IPS執行緒狀態
	THREAD_COMMAND_MODE        m_IPSProcThreadCmd;//IPS執行緒命令		
	//---------------------------------------------------------------------------------//		
protected:
	//---------------------------------------------------------------------------------//
	CMES_IPS(const CMES_IPS &other);
	CMES_IPS& operator=(const CMES_IPS &other);
	//---------------------------------------------------------------------------------//
	void                       PreInitIPS();
	void                       InitialIPS();
	void                       CloneIPS(const CMES_IPS &other);
	//---------------------------------------------------------------------------------//
	bool                       CheckProjectPtr(CAOIProject *ProjectPtr);//確認專案指標
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//	
	void                       LockIPSProc();//鎖住IPS執行緒同步化
	void                       UnlockIPSProc();//釋放IPS執行緒同步化
	//---------------------------------------------------------------------------------//		
	CITSLinker&                GetIPSLinker();//取得IPS連線參考
	bool                       ConnectIPSLinker();//連線到IPS連結軟體
	bool                       DisconnectIPSLinker();//停止連線到IPS連結軟體
	//---------------------------------------------------------------------------------//
	int                        GetIPSStatusID_Ack(int StatusID);//取得IPS的回應訊息
	bool                       CheckIPSStatusID_Ack(const TIPSCommNode &Node);//確認IPS的回應訊息
	bool                       CheckIPSStatusCode_RemoteControl(const TIPSCommNode &Node);//確認IPS的遠端控制
	//---------------------------------------------------------------------------------//	
	size_t                     GetIPSRecvNodeCount();	
	bool                       RecvNewIPSRecvNodeList();//接收新的IPS訊息列表
	bool                       AddIPSRecvNode(const std::string &sBuff);
	bool                       DecoderIPSPacket(const std::string &sBuff, TIPSCommNode &sNode);
	bool                       GetIPSRecvNode(size_t index, bool bCheck, TIPSCommNode &sNode);	
	//---------------------------------------------------------------------------------//	
	bool                       ProcesIPSRecvNodeList(bool bThread);//處理收到IPS的訊息列表
	bool                       ProcesIPSRecvNode(bool bThread, TIPSCommNode &sNode, bool &bBreak, bool &bRemove);//處理收到IPS的訊息
	//---------------------------------------------------------------------------------//	
	//IPS軟體	
	bool                       ExecIPSComm_MesSetParam(bool bThread, TIPSCommNode &sNode, bool &bBreak, bool &bRemove);	
	bool                       ExecIPSComm_MesOpenProject(bool bThread, TIPSCommNode &sNode, bool &bBreak, bool &bRemove);	
	bool                       ExecIPSComm_MesListProject(bool bThread, TIPSCommNode &sNode, bool &bBreak, bool &bRemove);
	bool                       ExecIPSComm_MesRemoteControl(bool bThread, TIPSCommNode &sNode, bool &bBreak, bool &bRemove);
	bool                       ExecIPSComm_SetSystemParam();//執行IPS溝通-設定系統參數
	bool                       ExecIPSComm_SetProjectParam(CAOIProject *ProjectPtr);//執行IPS溝通-設定專案參數
	bool                       ExecIPSComm_SetMachineStatus();//執行IPS溝通-設定機台狀態
	bool                       ExecIPSComm_SetMachineStatus(ONLINE_STATE_MODE Status);//執行IPS溝通-設定機台狀態	
	bool                       ExecIPSComm_ProcessID(int ProcessID);//執行IPS溝通-程序運作
	bool                       ExecIPSComm_SetAOIExceptionCode(LPCTSTR ErrStr);//執行MES溝通-系統異常碼
	bool                       ExecIPSComm_SetUserLogin_out(bool bLogin);//執行IPS溝通-設定使用者登入	
	bool                       ExecIPSComm_SetRemoteControlMode(bool bOnline);//執行IPS溝通-遠端控制模式		
	bool                       ExecIPSComm_CheckBarcode(CAOIProject *ProjectPtr);//執行IPS溝通-確認條碼
	bool                       ExecIPSComm_SetProjectList();//執行IPS溝通-條列專案
	bool                       ExecIPSComm_SetProjectLoad(CAOIProject *ProjectPtr, bool Success);//執行IPS溝通-專案載入
	//---------------------------------------------------------------------------------//	
	int                        GetIPSFreePPID();//取得IPS可用的PPID
	bool                       CheckIPSAlwaysAckMode();//確認總是回傳模式	
	bool                       BuildIPSDoc_BarcodeList(CAOIProject *ProjectPtr, rapidjson::WDocument &Doc, rapidjson::WValue &object);//建立IPS文檔
	bool                       BuildIPSDoc(int nStatus, bool bAck, int AckTime, int &nPPID, rapidjson::WDocument &Doc);//建立IPS文檔	
	bool                       BuildIPSDoc_Ack(int nStatus, bool bAck, int AckTime, int ResultCode, const wchar_t *ResultStr, rapidjson::WDocument &Doc);//建立IPS文檔	
	bool                       BuildIPSDoc_SetSystemParam(bool bAck, int AckTime, int &nPPID, std::wstring& wsBuf);//建立系統資訊文檔
	bool                       BuildIPSDoc_SetProjectParam(bool bAck, int AckTime, int &nPPID, CAOIProject *ProjectPtr, std::wstring& wsBuf);//建立基本資訊文檔
	bool                       BuildIPSDoc_SetMachineStatus(ONLINE_STATE_MODE Status, bool bAck, int AckTime, int &nPPID, std::wstring& wsBuf);//建立機台運作文檔	
	bool                       BuildIPSDoc_ProcessID(int ProcessID, bool bAck, int AckTime, int &nPPID, std::wstring& wsBuf);//建立程序運作文檔
	bool                       BuildIPSDoc_SetAOIExceptionCode(bool bAck, int AckTime, int &nPPID, std::wstring& wsBuf);//執行MES溝通-系統異常碼
	bool                       BuildIPSDoc_SetLogin_out(bool bAck, int AckTime, int &nPPID, bool Login, std::wstring& wsBuf);//建立登入參數文檔
	bool                       BuildIPSDoc_SetRemotControlMode(bool bAck, int AckTime, int &nPPID, bool bOnline, std::wstring& wsBuf);//建立遠端控制文檔	
	bool                       BuildIPSDoc_CheckBarcode(bool bAck, int AckTime, int &nPPID, CAOIProject *ProjectPtr, std::wstring& wsBuf);//建立確認條碼文檔
	bool                       BuildIPSDoc_SetProjectList(bool bAck, int AckTime, int &nPPID, std::wstring& wsBuf);//建立條列專案
	bool                       BuildIPSDoc_SetProjectLoad(bool bAck, int AckTime, int &nPPID, CAOIProject *ProjectPtr, bool Success, std::wstring& wsBuf);//建立專案載入文檔
	bool                       SendToIPSNode(const char *strBuf, bool bAck, int nPPID, DWORD AckTime, bool bChkFrz=true, LPCTSTR filename=NULL);//送資料給IPS
	bool                       WaitForIPSResponse(int nPPID, DWORD AckTime, LPCTSTR Filename);//等待IPS回傳資料
	//---------------------------------------------------------------------------------//	
	CString                    GetIPSStatusText(int StatusID);
	int                        MapIPSComm_ProcessID(int ProcessID);//映射ProcessID
	//---------------------------------------------------------------------------------//		
	bool                       GetAckFilename(CString &Filename);//取回Ack回傳的檔名	
	CString                    ExtractFilenameDateTime(LPCTSTR Filename);//萃取檔案名稱上的日期時間
	//---------------------------------------------------------------------------------//
	CString                    GetIPS_RemoteControlErrorText(int ErrorCode);//取得遠端控制錯誤訊息
	void                       GetIPS_RemoteControlErrorText(int ErrorCode, std::wstring &ErrorStr);//取得遠端控制錯誤訊息
	//---------------------------------------------------------------------------------//	
public:
	//---------------------------------------------------------------------------------//	
	CMES_IPS();
	virtual ~CMES_IPS();
	//---------------------------------------------------------------------------------//	
	MES_IMP_CLS                GetMesImpCls() const { return MES_IMP_CLS_IPS; }
	//---------------------------------------------------------------------------------//	
	bool                       CreateMESProcThread();//建立MES執行執行緒
	bool                       DeleteMESProcThread();//刪除MES執行執行緒
	//---------------------------------------------------------------------------------//
	bool                       GetMESConnected();//是否已經連線
	bool                       ConnectMESLinker();//連線到MES連結軟體
	bool                       DisconnectMESLinker();//停止連線到MES連結軟體
	//---------------------------------------------------------------------------------//
	bool                       ExecMESComm_SetSystemParam();//執行MES溝通-設定系統參數
	bool                       ExecMESComm_SetProjectParam(CAOIProject *ProjectPtr);//執行MES溝通-設定專案參數
	bool                       ExecMESComm_SetMachineStatus();//執行MES溝通-設定機台狀態
	bool                       ExecMESComm_SetMachineStatus(ONLINE_STATE_MODE Status);//執行MES溝通-設定機台狀態		
	bool                       ExecMESComm_ProcessID(int ProcessID);//執行MES溝通-程序運作
	bool                       ExecMESComm_SetAOIExceptionCode(LPCTSTR ErrStr);//執行MES溝通-系統異常碼
	bool                       ExecMESComm_SetUserLogin_out(bool bLogin);//執行MES溝通-設定使用者登入	
	bool                       ExecMESComm_SetRemoteControlMode(bool bOnline);//執行MES溝通-遠端控制模式		
	bool                       ExecMESComm_CheckBarcode(CAOIProject *ProjectPtr);//執行MES溝通-確認條碼
	bool                       ExecMESComm_SetProjectList();//執行MES溝通-條列專案
	bool                       ExecMESComm_SetProjectLoad(CAOIProject *ProjectPtr, bool Success);//執行MES溝通-專案載入
	//---------------------------------------------------------------------------------//
	size_t                     GetMESRecvNodeCount();	
	bool                       ProcesMESRecvNodeList(bool bThread);//處理收到MES的訊息列表
	//---------------------------------------------------------------------------------//
	bool                       SendToMESNode(const char *strBuf, bool bAck, int nPPID, DWORD AckTime);//送資料給MES
	//---------------------------------------------------------------------------------//
	//IPS執行
	bool                       ExecIPSProcFn();                      //執行IPS執行執行緒
	bool                       CreateIPSProcThread();                //建立IPS執行執行緒
	bool                       DeleteIPSProcThread();                //刪除IPS執行執行緒
	bool                       StopIPSProcThread(bool WaitOn);      //開始IPS執行執行緒
	bool                       StartIPSProcThread(bool WaitOn);      //開始IPS執行執行緒	
	bool                       WaitForIPSProcThreadIdle();           //等待IPS執行執行緒閒置
	bool                       WaitForIPSProcThreadStop();           //等待IPS執行執行緒停止
	bool                       WaitForIPSProcThreadStart();          //等待IPS執行執行緒開始
	bool                       WaitForIPSProcThreadFinish();         //等待IPS執行執行緒結束
	bool                       CheckIPSProcThreadState(THREAD_STATE_MODE State);//確認IPS執行執行緒狀態
	void                       SetIPSProcThreadState(THREAD_STATE_MODE State);//設定IPS執行執行緒狀態
	THREAD_STATE_MODE          GetIPSProcThreadState();              //取得IPS執行執行緒狀態
	void                       SetIPSProcThreadCmd(THREAD_COMMAND_MODE Cmd); //設定IPS執行執行緒命令
	THREAD_COMMAND_MODE        GetIPSProcThreadCmd();	               //取得IPS執行執行緒命令
	//---------------------------------------------------------------------------------//
};
//-------------------------------------------------------------------------------------//
#endif//IPS_DISABLE
#endif//MES_DISABLE
#endif // !defined(AFX_MES_IPS_H__0DA9E560_23BF_465B_8F91_A391E929E9FC__INCLUDED_)
