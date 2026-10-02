// Plc_Basic.h: interface for the CPLC_Basic class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLC_BASIC_H__B7922FA4_F633_443D_AE48_B522285D7B7A__INCLUDED_)
#define AFX_PLC_BASIC_H__B7922FA4_F633_443D_AE48_B522285D7B7A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include "Plc_Define.h"
//-------------------------------------------------------------------------------------//
//PLC_OBJ_DISABLE
#define PLC_OBJ_VIGOR                 1
//函式命名定義
//Set/Get    對於內存變數設定與取回
//Write/Read 對於PLC裝置直接讀寫


class CPLC_Basic : public CObject  
{
	//---------------------------------------------------------------------------------//	
	DECLARE_DYNAMIC(CPLC_Basic)
	//---------------------------------------------------------------------------------//
	static CString             ObtainPLCParameterDescText(PLC_PARAM_ID ParamID);//取得PLC參數的說明文字		
	static bool                SetPLCParameterStringByID(PLC_PARAM_ID ParamID, TPLCParameter &Param, LPCTSTR String);//設定PLC參數
	static bool                GetPLCParameterStringByID(PLC_PARAM_ID ParamID, const TPLCParameter &Param, CString &String);//取得PLC參數
	//---------------------------------------------------------------------------------//	
	static bool                CheckPlcNodeAddress(const char *Address);
	static CString             FormTowerLightModelText(int Mode);//取得塔燈模式文字
	static CString             FormTowerLightStateText(int State);//取得塔燈狀態文字
	static bool                BuildTowerLightModeCombox(CComboBox &Combox);//建立塔燈模式列表視窗
	static bool                BuildTowerLightStateCombox(CComboBox &Combox);//建立塔燈狀態列表視窗
	//---------------------------------------------------------------------------------//		
private:
	//---------------------------------------------------------------------------------//	
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//			
	int                        m_PLCNodeCurrentIndex;	
	std::vector<TPlcNode>      m_PLCNodeList;
	//---------------------------------------------------------------------------------//				
	CString                    m_PLCVersion;
	int                        m_PLCVersionI;
	CString                    m_ErrorString;
	CString                    m_ErrorStringOut;//錯誤字串	
	bool                       m_PLCIsConnected;
	CString                    m_PLCConnectParam;
	CRITICAL_SECTION           m_csPLC;	
	TPLCParameter              m_PLCParameter;
	//---------------------------------------------------------------------------------//	
	THREAD_STATE_MODE          m_PLCPollingThreadState;
	THREAD_COMMAND_MODE        m_PLCPollingThreadCmd;
	int                        m_PLCPollingSleepTime;	
	bool                       m_PLCPollingLaneAdjustSensor;//追蹤軌道調整感測器
	//---------------------------------------------------------------------------------//	
	bool                       m_HardwareByPass_LA;
	bool                       m_HardwareByPass_LB;
	bool                       m_PLC_RightIn;//PLC右進料模式
	bool                       m_PLC_FanAlarm;//PLC風扇警報
	bool                       m_PLC_SafetyAlarm;//PLC安全警報
	bool                       m_PLC_SafetyAlarmEMS;//PLC安全警報-EMS
	bool                       m_DualLaneMode;//雙軌道模式
	bool                       m_DualTowerLight;//雙塔燈模式
	int                        m_LastStationLineMode;//上一站接線模式
	bool                       m_ConveryerSensorPower_LA;//PLC輸送帶感應器電源-A軌
	bool                       m_ConveryerSensorPower_LB;//PLC輸送帶感應器電源-B軌

	//輸送帶A軌的感測器
	bool                       m_ConveryerEnabled_LA;//軌道啟用
	bool                       m_ConveryerClamp_LA;
	bool                       m_ConveryerStopBar_LA;
	bool                       m_PLC_I_SensorPCBIn_LA;//進板感應器
	bool                       m_PLC_I_SensorPCBOut_LA;//出板感應器
	bool                       m_PLC_I_SensorPCBStop_LA;//停板感應器
	bool                       m_PLC_I_SensorPCBSlow_LA;//減速感應器
	bool                       m_PLC_I_SensorPCBStop2_LA;//停板感應器-2
	bool                       m_PLC_I_SignalFromLast_LA;//上一站訊號
	bool                       m_PLC_I_SignalFromNext_LA;//下一站訊號	
	bool                       m_PLC_O_SignalLockStation_LA;	
	bool                       m_PLC_O_SignalToLast_LA;
	bool                       m_PLC_O_SignalToNext_LA;	
	bool                       m_PLC_O_SignalToNextOK_LA;
	bool                       m_PLC_O_SignalToNextNG_LA;
	bool                       m_PLC_O_PCBBargeIn_LA;//PCB闖入
	int                        m_PLC_O_ConveryerStatus_LA;//PLC輸送帶狀態

	//輸送帶B軌的感測器	
	bool                       m_ConveryerEnabled_LB;//軌道啟用
	bool                       m_ConveryerClamp_LB;
	bool                       m_ConveryerStopBar_LB;
	bool                       m_PLC_I_SensorPCBIn_LB;//進板感應器
	bool                       m_PLC_I_SensorPCBOut_LB;//出板感應器
	bool                       m_PLC_I_SensorPCBStop_LB;//停板感應器
	bool                       m_PLC_I_SensorPCBSlow_LB;//減速感應器	
	bool                       m_PLC_I_SensorPCBStop2_LB;//停板感應器-2
	bool                       m_PLC_I_SignalFromLast_LB;
	bool                       m_PLC_I_SignalFromNext_LB;	
	bool                       m_PLC_O_SignalLockStation_LB;
	bool                       m_PLC_O_SignalToLast_LB;
	bool                       m_PLC_O_SignalToNext_LB;
	bool                       m_PLC_O_SignalToNextOK_LB;
	bool                       m_PLC_O_SignalToNextNG_LB;
	bool                       m_PLC_O_PCBBargeIn_LB;//PCB闖入
	int                        m_PLC_O_ConveryerStatus_LB;//PLC輸送帶狀態
	
	//現況訊號
	bool                       m_PLC_I_AirLost;
	bool                       m_PLC_I_FrontCapOpened;
	bool                       m_PLC_I_RearCapOpened;
	bool                       m_PLC_I_KeySwitchOff;
	bool                       m_PLC_I_EMSOn;
	bool                       m_PLC_I_BtnStop;//輸出-停止燈-紅燈	
	bool                       m_PLC_I_BtnStart;//輸入-啟動燈-綠燈
	bool                       m_PLC_I_BtnReset;//輸入-復歸燈-黃燈
	bool                       m_PLC_I_OverHeat;//輸入-過熱感應器
	bool                       m_PLC_I_FanAlarm;//輸入-風扇警報

	bool                       m_PLC_O_LightStop;//輸出-停止燈-紅燈
	bool                       m_PLC_O_LightStart;//輸出-啟動燈-綠燈	
	
	//電源開關
	bool                       m_PLC_O_PowerACMotor;	
	bool                       m_PLC_O_LightDay;//輸出-日光燈
	//安全檢測
	bool                       m_SaftyBypass;
	
	//異常
	bool                       m_PLCExecAlarm;
	bool                       m_StageAlarm_LA;
	bool                       m_InspectionAlarm_LA;
	bool                       m_StageAlarm_LB;
	bool                       m_InspectionAlarm_LB;

	//塔燈
	bool                       m_PLC_O_Buzzer_LA;
	bool                       m_PLC_O_LightTowerRed_LA;
	bool                       m_PLC_O_LightTowerYel_LA;
	bool                       m_PLC_O_LightTowerGrn_LA;

	bool                       m_PLC_O_Buzzer_LB;
	bool                       m_PLC_O_LightTowerRed_LB;
	bool                       m_PLC_O_LightTowerYel_LB;
	bool                       m_PLC_O_LightTowerGrn_LB;	
	//---------------------------------------------------------------------------------//	
	int                        m_TowerLightMode;//塔燈模式
	//塔燈狀態
	int                        m_TowerLightState_Stop;          //塔燈-停止中
	int                        m_TowerLightState_Stop_LA;       //塔燈-停止中
	int                        m_TowerLightState_Stop_LB;       //塔燈-停止中
	int                        m_TowerLightState_Inspection;    //塔燈-檢測中
	int                        m_TowerLightState_Bypass;        //塔燈-輸送帶模式
	int                        m_TowerLightState_WaitLast;      //塔燈-等待上一站
	int                        m_TowerLightState_WaitNext;      //塔燈-等待下一站
	int                        m_TowerLightState_PCBIn;         //塔燈-進板中
	int                        m_TowerLightState_PCBOut;        //塔燈-出板中
	int                        m_TowerLightState_PCBBack;       //塔燈-回板中	
	//---------------------------------------------------------------------------------//	
	bool                       m_PCBBackOut_LA;//是否退出板狀態
	bool                       m_PCBBackOut_LB;//是否退出板狀態
	//---------------------------------------------------------------------------------//
	int                        m_PCBAutoRunMode_LA;//自動進出板模式
	int                        m_PCBInCheckCount_LA;//進板確認次數
	int                        m_PCBOutCheckCount_LA;//出板確認次數
	int                        m_PCBBackCheckCount_LA;//回板確認次數
	int                        m_PCBOutInCheckCount_LA;//出板帶進板的確認次數
	bool                       m_PCBAutoRunPCBChaned_LA;//自動進出板PCB改變過

	int                        m_PCBAutoRunMode_LB;//自動進出板模式
	int                        m_PCBInCheckCount_LB;//進板確認次數
	int                        m_PCBOutCheckCount_LB;//出板確認次數
	int                        m_PCBBackCheckCount_LB;//回板確認次數
	int                        m_PCBOutInCheckCount_LB;//出板帶進板的確認次數
	bool                       m_PCBAutoRunPCBChaned_LB;//自動進出板PCB改變過

	DWORD	                   m_WaitingLastStationTimeStart_LA;	//等待前站訊號開始
	DWORD	                   m_WaitingLastStationTimeEnd_LA;	//等待前站訊號結束
	DWORD	                   m_WaitingNextStationTimeStart_LA;	//等待後站訊號開始
	DWORD	                   m_WaitingNextStationTimeEnd_LA;	//等待後站訊號結束
	DWORD	                   m_WaitingLastStationTimeStart_LB;	//等待前站訊號開始
	DWORD	                   m_WaitingLastStationTimeEnd_LB;	//等待前站訊號結束
	DWORD	                   m_WaitingNextStationTimeStart_LB;	//等待後站訊號開始
	DWORD	                   m_WaitingNextStationTimeEnd_LB;	//等待後站訊號結束
	
	DWORD                      m_PCBInTimeStart_LA;//進板時間開始
	DWORD                      m_PCBInTimeEnd_LA;//進板時間結束
	DWORD                      m_PCBOutTimeStart_LA;//出版時間開始
	DWORD                      m_PCBOutTimeEnd_LA;//出版時間結束
	DWORD                      m_PCBBackTimeStart_LA;//回板時間開始
	DWORD                      m_PCBBackTimeEnd_LA;//回板時間結束
	
	DWORD                      m_PCBInTimeStart_LB;//進板時間開始
	DWORD                      m_PCBInTimeEnd_LB;//進板時間結束
	DWORD                      m_PCBOutTimeStart_LB;//出版時間開始
	DWORD                      m_PCBOutTimeEnd_LB;//出版時間結束
	DWORD                      m_PCBBackTimeStart_LB;//回板時間開始
	DWORD                      m_PCBBackTimeEnd_LB;//回板時間結束
	//---------------------------------------------------------------------------------//	
	//軌道間距馬達			
	int                        m_LaneAdjust_Fixed14Lane;//軌道調整-14軌固定
	int                        m_LaneAdjust_HomeTimeout;//軌道調整-歸零逾時
	int                        m_LaneAdjust_MoveTimeout;//軌道調整-移動逾時
	
	bool                       m_LaneAdjust_Disable_LA;//軌道調整-關閉-消磁-LA
	double                     m_LaneAdjust_SkewPitch_LA;	//軌道調整-螺紋間距-LA
	bool                       m_LaneAdjust_SensorORG_LA;//軌道調整-歸零感應器-LA
	bool                       m_LaneAdjust_SensorLimit_LA;//軌道調整-極限感應器-LA	
	double                     m_LaneAdjust_LimitMax_LA;	//軌道調整-最大位置-LA
	double                     m_LaneAdjust_LimitMin_LA;	//軌道調整-最小位置-LA
	bool                       m_LaneAdjust_Homed_LA;	//軌道調整-是否歸零過-LA
	double                     m_LaneAdjust_MoveSpeed_LA; //軌道調整-移動速度-LA
	bool                       m_LaneAdjust_JogMoving_LA;//軌道調整-Jog移動中-LA
	double                     m_LaneAdjust_JogSlowSpeed_LA;//軌道調整-搖桿速度-慢-LA
	double                     m_LaneAdjust_JogFastSpeed_LA;//軌道調整-搖桿速度-快-LA
	double                     m_LaneAdjust_HomePos_LA;//軌道調整-歸零位置-LA
	double                     m_LaneAdjust_CurrentPos_LA;//軌道調整-現金位置-LA	

	bool                       m_LaneAdjust_Disable_LB;//軌道調整-關閉-消磁-LB
	double                     m_LaneAdjust_SkewPitch_LB;	//軌道調整-螺紋間距-LB
	bool                       m_LaneAdjust_SensorORG_LB;//軌道調整-歸零感應器-LB
	bool                       m_LaneAdjust_SensorLimit_LB;//軌道調整-極限感應器-LB
	double                     m_LaneAdjust_LimitMax_LB;  //軌道調整-最大位置-LB
	double                     m_LaneAdjust_LimitMin_LB;  //軌道調整-最小位置-LB
	bool                       m_LaneAdjust_Homed_LB;     //軌道調整-是否歸零過-LB
	double                     m_LaneAdjust_MoveSpeed_LB; //軌道調整-移動速度-LB
	bool                       m_LaneAdjust_JogMoving_LB;//軌道調整-Jog移動中-LA
	double                     m_LaneAdjust_JogSlowSpeed_LB;//軌道調整-搖桿速度-慢-LB
	double                     m_LaneAdjust_JogFastSpeed_LB;//軌道調整-搖桿速度-快-LB
	double                     m_LaneAdjust_HomePos_LB;     //軌道調整-歸零位置-LB
	double                     m_LaneAdjust_CurrentPos_LB;//軌道調整-現金位置-LB
	//---------------------------------------------------------------------------------//	
	CPLC_Basic(const CPLC_Basic &PLC);
	CPLC_Basic& operator=(const CPLC_Basic &PLC);	
	int                        CheckTowerLightState(int State);//確認塔燈設定狀態
	//---------------------------------------------------------------------------------//	
	bool                       ReturnPLCNotSupportFunc(LPCTSTR fnName);//回傳PLC未支援函式
	//---------------------------------------------------------------------------------//
	bool                       SaveINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pString, LPCTSTR pfilename, CString &Error);
	bool                       LoadINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pDefault, TCHAR *pString, int StringSize, LPCTSTR pfilename, bool IsCheckLens, CString &Error);	
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	CPLC_Basic();
	virtual ~CPLC_Basic();
	//---------------------------------------------------------------------------------//
	void                       LockPLC();
	void                       UnlockPLC();
	//---------------------------------------------------------------------------------//	
	void                       SetPLCExceptionCode_Param(LPCTSTR Err=NULL);//設定JET錯誤碼-參數異常
	void                       SetPLCExceptionCode_FileRead(LPCTSTR Err=NULL);//設定JET錯誤碼-檔案讀取
	void                       SetPLCExceptionCode_FileWrite(LPCTSTR Err=NULL);//設定JET錯誤碼-檔案寫入
	void                       SetPLCExceptionCode(DWORD Code, LPCTSTR Err=NULL);//設定JET錯誤碼
	//---------------------------------------------------------------------------------//
	bool                       GetPLCIsConnected() const;	
	bool                       CheckPLCIsConnected();	
	LPCTSTR                    GetPLCErrorString();
	LPCTSTR                    GetPLCVersion() const;
	//---------------------------------------------------------------------------------//
	void                       SetPLCConnectParam(LPCTSTR val);//設定PLC連線參數
	LPCTSTR                    GetPLCConnectParam() const;//取得PLC連線參數
	//---------------------------------------------------------------------------------//
	TPLCParameter&             GetPLCParameter();//取得PLC參數
	const TPLCParameter&       GetPLCParameter() const;//取得PLC參數
	void                       SetPLCParameter(const TPLCParameter &Param);//設定PLC參數	
	//---------------------------------------------------------------------------------//	
	void                       SetPLCPollingThreadState(THREAD_STATE_MODE State);
	THREAD_STATE_MODE          GetPLCPollingThreadState() const;
	void                       SetPLCPollingThreadCmd(THREAD_COMMAND_MODE Cmd);
	THREAD_COMMAND_MODE        GetPLCPollingThreadCmd() const;
	void                       SetPLCPollingSleepTime(int time);
	int                        GetPLCPollingSleepTime() const;
	//追蹤軌道調整感測器
	void                       SetPLCPollingLaneAdjustSensor(bool On);
	bool                       GetPLCPollingLaneAdjustSensor() const;	
	//---------------------------------------------------------------------------------//
	bool                       CreatePLCPollingThread();
	bool                       DeletePLCPollingThread();
	bool                       StartPLCPollingThread(); //開始監控PLC
	bool                       StopPLCPollingThread();  //停止監控PLC
	//---------------------------------------------------------------------------------//	
	virtual bool               PreInitPLC();
	virtual bool               SavePLCINIParameter();
	virtual bool               SavePLCINIParameterFn();
	virtual bool               LoadPLCINIParameter();
	virtual bool               LoadPLCINIParameterFn();

	virtual bool               SavePLCMovingTimeMsg(const char *pContext);//儲存PLC移動時間訊息
	virtual bool               SavePLCMovingTimeMsg(const wchar_t *pContext);//儲存PLC移動時間訊

	virtual bool               InitialPLC()=0;
	virtual bool               PLC_ConnectTo(LPCTSTR Param)=0;
	virtual bool               PLC_Disconnect()=0;		
	virtual bool               PLC_BuildPlcNodeList()=0;
	virtual bool               PLC_ReadAllStats(bool bCheckThread)=0;	
	virtual bool               PLC_ReadExecAlarm()=0;
	virtual bool               PLC_ReadTowerLight(bool bCheckThread)=0;//讀取塔燈
	virtual bool               PLC_ReadMachineSensor(bool bCheckThread)=0;//讀取機台感測器	
	virtual bool               PLC_ReadConveryerSensor(bool bCheckThread)=0;//讀取軌道感應器	
	virtual bool               PLC_ReadLaneAdjustStatus(bool bCheckThread)=0;//讀取軌道間距狀態
	virtual bool               PLC_ReadConveryerRunStatus(bool bCheckThread)=0;//讀取軌道運轉狀態
	virtual bool               PLC_ReadConveryerSignalSend(bool bCheckThread)=0;//讀取軌道訊號
	virtual bool               PLC_ReadPlcNodeList(bool bCheckThread)=0;//PLC讀取PLC節點列表
	virtual bool               PLC_ReadNode(const char *Node, int &value)=0;
	virtual bool               PLC_WriteNode(const char *Node, int value)=0;	
	virtual bool               PLC_WriteINIParameter()=0;
	virtual CString            LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);//載入多國語系
	virtual CString            GetPLCErrorCodeText(int ErrorCode)=0;//取得PLC錯誤帶碼文字	
	//---------------------------------------------------------------------------------//
	//操作	
	virtual bool               TurnOnOffTargetCap(bool On)=0;//PLC-開關塊規上蓋	

	virtual bool               WritePLCReset()=0;
	virtual bool               WriteBypassSafty(bool On)=0;//關閉安全檢測	
	virtual bool               WriteDualLaneMode(bool On)=0;//設定是否為雙軌道模式
	virtual bool               WriteDualTowerLight(bool On)=0;//設定是否為雙塔燈模式	

	virtual bool               WriteTowerLightMode(int Mode)=0;//設定是否為塔燈模式
	virtual bool               WriteTowerLightState_Stop(int Value)=0;//設定塔燈狀態-停止
	virtual bool               WriteTowerLightState_Stop(LANE_ID LaneID, int Value)=0;//設定塔燈狀態-停止
	virtual bool               WriteTowerLightState_Inspection(int Value)=0;//設定塔燈狀態-檢測中
	virtual bool               WriteTowerLightState_Bypass(int Value)=0;//設定塔燈狀態-直通
	virtual bool               WriteTowerLightState_WaitLast(int Value)=0;//設定塔燈狀態-等上站
	virtual bool               WriteTowerLightState_WaitNext(int Value)=0;//設定塔燈狀態-等下站
	virtual bool               WriteTowerLightState_PCBIn(int Value)=0;//設定塔燈狀態-進板
	virtual bool               WriteTowerLightState_PCBOut(int Value)=0;//設定塔燈狀態-出板
	virtual bool               WriteTowerLightState_PCBBack(int Value)=0;//設定塔燈狀態-退板

	virtual bool               UpdateTowerLightState_Stop();//更新塔燈狀態-停止
	virtual bool               UpdateTowerLightState_Stop(LANE_ID LaneID);//更新塔燈狀態-停止
	virtual bool               UpdateTowerLightState_WaitLast(LANE_ID LaneID);//更新塔燈狀態-等上站
	virtual bool               UpdateTowerLightState_WaitNext(LANE_ID LaneID);//更新塔燈狀態-等下站

	//Lane
	virtual bool               WriteOnInspection(LANE_ID LaneID, bool On);//檢測中	
	virtual bool               TurnOnConveryerLED(LANE_ID LaneID, bool On);//軌道LED燈

	virtual bool               TurnOnStageAlarm(LANE_ID LaneID);//PLC-設定機台異常
	virtual bool               TurnOffStageAlarm(LANE_ID LaneID);//PLC-關閉機台異常

	virtual bool               TurnOnInspectAlarm(LANE_ID LaneID);//PLC-設定檢測異常
	virtual bool               TurnOffInspectAlarm(LANE_ID LaneID);//PLC-關閉檢測異常
	
	virtual bool               WriteConveryerClamp(LANE_ID LaneID, bool On);//PLC-是否夾板
	virtual bool               WriteConveryerStopBar(LANE_ID LaneID, bool On);//PLC-停板器	

	virtual bool               ExecPCBIn(LANE_ID LaneID, bool bWait);//PLC-進板
	virtual bool               WaitForPCBInFinish(LANE_ID LaneID);//PLC-等進板完成
	virtual bool               CheckPCBInFinish(LANE_ID LaneID);//PLC-進板完成
	virtual bool               CheckPCBInFault(LANE_ID LaneID);//PLC-進板失敗	

	virtual bool               ExecPCBIn2nd(LANE_ID LaneID, bool bWait);//PLC-第2段進板
	virtual bool               WaitForPCBIn2ndFinish(LANE_ID LaneID);//PLC-等第2段進板完成
	virtual bool               CheckPCBIn2ndFinish(LANE_ID LaneID);//PLC-第2段進板完成
	virtual bool               CheckPCBIn2ndFault(LANE_ID LaneID);//PLC-第2段進板失敗	

	virtual bool               ExecPCBIn3rd(LANE_ID LaneID, bool bWait);//PLC-第3段進板
	virtual bool               WaitForPCBIn3rdFinish(LANE_ID LaneID);//PLC-等第3段進板完成
	virtual bool               CheckPCBIn3rdFinish(LANE_ID LaneID);//PLC-第3段進板完成
	virtual bool               CheckPCBIn3rdFault(LANE_ID LaneID);//PLC-第3段進板失敗	

	virtual bool               ExecPCBBack(LANE_ID LaneID, bool bWait);////PLC-退板
	virtual bool               WaitForPCBBackFinish(LANE_ID LaneID);//PLC-等退板完成
	virtual bool               CheckPCBBackFinish(LANE_ID LaneID);//PLC-退板完成
	virtual bool               CheckPCBBackFault(LANE_ID LaneID);//PLC-退板失敗

	virtual bool               ExecPCBBackOut(LANE_ID LaneID, bool bWait);//PLC-退出板
	virtual bool               WaitForPCBBackOutFinish(LANE_ID LaneID);//PLC-等退出板完成
	virtual bool               CheckPCBBackOutFinish(LANE_ID LaneID);//PLC-退出板完成
	virtual bool               CheckPCBBackOutFault(LANE_ID LaneID);//PLC-退出板失敗

	virtual bool               ExecPCBOut(LANE_ID LaneID, bool bWait);//PLC-出板
	virtual bool               WaitForPCBOutFinish(LANE_ID LaneID);//PLC-等出板完成
	virtual bool               CheckPCBOutFinish(LANE_ID LaneID);//PLC-出板完成
	virtual bool               CheckPCBOutFault(LANE_ID LaneID);//PLC-出板失敗

	virtual bool               ExecPCBOutInside(LANE_ID LaneID, bool bWait);//PLC-出板至機台側邊
	virtual bool               WaitForPCBOutInsideFinish(LANE_ID LaneID);//PLC-等待出板至機台側邊
	virtual bool               CheckPCBOutInsideFinish(LANE_ID LaneID);//PLC-出板至機台側邊完成
	virtual bool               CheckPCBOutInsideFault(LANE_ID LaneID);//PLC-出板至機台側邊失敗

	virtual bool               ExecPCBOutIn(LANE_ID LaneID, bool bWait);//PLC-出板+進板
	virtual bool               WaitForPCBOutInFinish(LANE_ID LaneID);//PLC-等待出板+進板
	virtual bool               CheckPCBOutInFinish(LANE_ID LaneID);//PLC-出板+進板完成
	virtual bool               CheckPCBOutInFault(LANE_ID LaneID);//PLC-出板+進板失敗

	virtual bool               ExecPCBReIn(LANE_ID LaneID, bool bWait);//PLC-重新進板
	virtual bool               WaitForPCBReInFinish(LANE_ID LaneID);//PLC-等待重新進板完成

	virtual bool               ExecPCBClear(LANE_ID LaneID, bool bWait);//PLC-清板
	virtual bool               WaitForPCBClearFinish(LANE_ID LaneID);//PLC-等待清板完成
	
	virtual int                GetPCBAutoRunMode(LANE_ID LaneID);//自動進出板模式
	virtual void               SetPCBAutoRunMode(LANE_ID LaneID, int Mode);//自動進出板模式
	virtual bool               GetPCBAutoRunPCBChaned(LANE_ID LaneID);//自動進出板PCB改變過
	virtual void               SetPCBAutoRunPCBChaned(LANE_ID LaneID, bool Changed);//自動進出板PCB改變過
	virtual bool               StopPCBAutoOutIn(LANE_ID LaneID, bool bWait);//PLC-停止自動出進板
	virtual bool               ExecPCBAutoOutIn(LANE_ID LaneID, bool On, bool bWait);//PLC-自動出進板
	virtual bool               WaitForPCBAutoOutInFinish(LANE_ID LaneID);//PLC-等待自動出進板
	virtual bool               ReadPCBAutoOutInFinish(LANE_ID LaneID);//PLC-自動出進板完成
	virtual bool               CheckPCBAutoOutInFinish(LANE_ID LaneID);//PLC-自動出進板完成
	virtual bool               CheckPCBAutoOutInFault(LANE_ID LaneID);//PLC-自動出進板失敗
	virtual bool               WritePCBAutoOutInFinish(LANE_ID LaneID, bool On);//PLC-寫入自動出進板完成
	
	virtual bool               StopPCBAutoBackIn(LANE_ID LaneID, bool bWait);//PLC-停止自動退進板
	virtual bool               ExecPCBAutoBackIn(LANE_ID LaneID, bool On, bool bWait);//PLC-自動退進板
	virtual bool               WaitForPCBAutoBackInFinish(LANE_ID LaneID);//PLC-等待自動退進板
	virtual bool               ReadPCBAutoBackInFinish(LANE_ID LaneID);//PLC-自動退進板完成
	virtual bool               CheckPCBAutoBackInFinish(LANE_ID LaneID);//PLC-自動退進板完成
	virtual bool               CheckPCBAutoBackInFault(LANE_ID LaneID);//PLC-自動退進板失敗
	virtual bool               WritePCBAutoBackInFinish(LANE_ID LaneID, bool On);//PLC-寫入自動退進板完成

	virtual bool               WaitForConveryerStopRunning(LANE_ID LaneID);//PLC-等待軌道停止運轉
	virtual bool               ExecConveyorMotorStop(LANE_ID LaneID);//PLC-執行軌道停止
	virtual bool               ExecConveyorMotorRunning(LANE_ID LaneID, bool On, bool bPositive, bool Slow);//PLC-執行軌道運轉	

	virtual bool               WriteSignalToLast(LANE_ID LaneID, bool On);//PLC-送訊號給上一站
	virtual bool               WriteSignalToNext(LANE_ID LaneID, bool On);//PLC-送訊號給下一站
	virtual bool               WriteOKSignalToNext(LANE_ID LaneID, bool On);//PLC-送OK訊號給下一站
	virtual bool               WriteNGSignalToNext(LANE_ID LaneID, bool On);//PLC-送NG訊號給下一站
	virtual bool               WriteLockSignal(LANE_ID LaneID, bool On);//PLC-送鎖住訊號
	virtual bool               TurnOffConveyerSensorPower(LANE_ID LaneID, bool On);//PLC-關閉軌道感應器電源

	//Lane A	
	virtual bool               WriteOnInspection_LA(bool On)=0;//設定檢測中-LA	
	virtual bool               TurnOnConveryerLED_LA(bool On)=0;//開啟軌道LED-LA

	virtual bool               TurnOnStageAlarm_LA()=0;
	virtual bool               TurnOffStageAlarm_LA()=0;	

	virtual bool               TurnOnInspectAlarm_LA()=0;
	virtual bool               TurnOffInspectAlarm_LA()=0;

	virtual bool               WriteConveryerClamp_LA(bool On)=0;//PLC-是否夾板-LA	
	virtual bool               WriteConveryerStopBar_LA(bool On)=0;//PLC-停板器-LA	
	virtual int                ReadLanePLCErrorCode_LA()=0;//讀取PLC的軌道錯誤代碼-LA

	virtual bool               ExecPCBIn_LA(bool bWait)=0;//PLC-進板-LA
	virtual bool               WaitForPCBInFinish_LA()=0;//PLC-等進板完成-LA
	virtual bool               CheckPCBInFinish_LA()=0;//PLC-進板完成-LA
	virtual bool               CheckPCBInFault_LA()=0;//PLC-進板失敗-LA	

	virtual bool               ExecPCBIn2nd_LA(bool bWait)=0;//PLC-第2段進板-LA
	virtual bool               WaitForPCBIn2ndFinish_LA()=0;//PLC-第2段等進板完成-LA
	virtual bool               CheckPCBIn2ndFinish_LA()=0;//PLC-第2段進板完成-LA
	virtual bool               CheckPCBIn2ndFault_LA()=0;//PLC-第2段進板失敗-LA	

	virtual bool               ExecPCBIn3rd_LA(bool bWait)=0;//PLC-第3段進板-LA
	virtual bool               WaitForPCBIn3rdFinish_LA()=0;//PLC-第3段等進板完成-LA
	virtual bool               CheckPCBIn3rdFinish_LA()=0;//PLC-第3段進板完成-LA
	virtual bool               CheckPCBIn3rdFault_LA()=0;//PLC-第3段進板失敗-LA	

	virtual bool               ExecPCBBack_LA(bool bWait)=0;////PLC-退板-LA
	virtual bool               WaitForPCBBackFinish_LA()=0;//PLC-等退板完成-LA
	virtual bool               CheckPCBBackFinish_LA()=0;//PLC-退板完成-LA
	virtual bool               CheckPCBBackFault_LA()=0;//PLC-退板失敗-LA

	virtual bool               ExecPCBBackOut_LA(bool bWait)=0;//PLC-退出板-LA
	virtual bool               WaitForPCBBackOutFinish_LA()=0;//PLC-等退出板完成-LA
	virtual bool               CheckPCBBackOutFinish_LA()=0;//PLC-退出板完成-LA
	virtual bool               CheckPCBBackOutFault_LA()=0;//PLC-退出板失敗-LA	

	virtual bool               ExecPCBOut_LA(bool bWait)=0;//PLC-出板-LA
	virtual bool               WaitForPCBOutFinish_LA()=0;//PLC-等出板完成-LA
	virtual bool               CheckPCBOutFinish_LA()=0;//PLC-出板完成-LA
	virtual bool               CheckPCBOutFault_LA()=0;//PLC-出板失敗-LA

	virtual bool               ExecPCBOutInside_LA(bool bWait)=0;//PLC-出板至機台側邊-LA
	virtual bool               WaitForPCBOutInsideFinish_LA()=0;//PLC-等待出板至機台側邊-LA
	virtual bool               CheckPCBOutInsideFinish_LA()=0;//PLC-出板至機台側邊完成-LA
	virtual bool               CheckPCBOutInsideFault_LA()=0;//PLC-出板至機台側邊失敗-LA

	virtual bool               ExecPCBOutIn_LA(bool bWait)=0;//PLC-出板+進板-LA
	virtual bool               WaitForPCBOutInFinish_LA()=0;//PLC-等待出板+進板-LA
	virtual bool               CheckPCBOutInFinish_LA()=0;//PLC-出板+進板完成-LA
	virtual bool               CheckPCBOutInFault_LA()=0;//PLC-出板+進板失敗-LA

	virtual bool               ExecPCBReIn_LA(bool bWait)=0;//PLC-重新進板-LA
	virtual bool               CheckPCBReInFinish_LA()=0;//PLC-重新進板完成-LA
	virtual bool               WaitForPCBReInFinish_LA()=0;//PLC-等待重新進板完成-LA

	virtual bool               ExecPCBClear_LA(bool bWait)=0;//PLC-清板-LA
	virtual bool               CheckPCBClearFinish_LA()=0;//PLC-確認清板完成-LA
	virtual bool               WaitForPCBClearFinish_LA()=0;//PLC-等待清板完成-LA

	virtual int                GetPCBAutoRunMode_LA();//自動進出板模式-LA	
	virtual bool               GetPCBAutoRunPCBChaned_LA() const;//自動進出板PCB改變過
	virtual void               SetPCBAutoRunPCBChaned_LA(bool val);//自動進出板PCB改變過
	virtual bool               StopPCBAutoOutIn_LA(bool bWait)=0;//PLC-停止自動進出板-LA
	virtual bool               ExecPCBAutoOutIn_LA(bool On, bool bWait)=0;//PLC-自動出進板-LA	
	virtual bool               WaitForPCBAutoOutInFinish_LA()=0;//PLC-等待自動出進板-LA
	virtual bool               ReadPCBAutoOutInFinish_LA()=0;//PLC-自動出進板完成-LA
	virtual bool               CheckPCBAutoOutInFinish_LA()=0;//PLC-自動出進板完成-LA
	virtual bool               CheckPCBAutoOutInFault_LA()=0;//PLC-自動出進板失敗-LA
	virtual bool               WritePCBAutoOutInFinish_LA(bool On)=0;//PLC-寫入PCB自動出進完成-LA	

	virtual bool               StopPCBAutoBackIn_LA(bool bWait)=0;//PLC-停止自動退進板-LA
	virtual bool               ExecPCBAutoBackIn_LA(bool On, bool bWait)=0;//PLC-自動退進板-LA
	virtual bool               WaitForPCBAutoBackInFinish_LA()=0;//PLC-等待自動退進板-LA
	virtual bool               ReadPCBAutoBackInFinish_LA()=0;//PLC-自動退進板完成-LA
	virtual bool               CheckPCBAutoBackInFinish_LA()=0;//PLC-自動退進板完成-LA
	virtual bool               CheckPCBAutoBackInFault_LA()=0;//PLC-自動退進板失敗-LA
	virtual bool               WritePCBAutoBackInFinish_LA(bool On)=0;//PLC-寫入PCB自動退進完成-LA

	virtual bool               CheckConveryerStopRunning_LA()=0;//PLC-確認軌道停止運轉-LA
	virtual bool               WaitForConveryerStopRunning_LA()=0;//PLC-等待軌道停止運轉-LA
	virtual bool               ExecConveyorMotorRunning_LA(bool On, bool bPositive, bool bSlow)=0;//PLC-執行軌道運轉-LA

	virtual bool               WriteSignalToLast_LA(bool On)=0;//PLC-送訊號給上一站-LA
	virtual bool               WriteSignalToNext_LA(bool On)=0;//PLC-送訊號給下一站-LA
	virtual bool               WriteOKSignalToNext_LA(bool On)=0;//PLC-送OK訊號給下一站-LA
	virtual bool               WriteNGSignalToNext_LA(bool On)=0;//PLC-送NG訊號給下一站-LA
	virtual bool               WriteLockSignal_LA(bool On)=0;//PLC-送鎖住訊號-LA
	virtual bool               WriteConveyerSensorPower_LA(bool On, bool bForce)=0;//PLC-關閉軌道感應器電源-LA
	virtual bool               TurnOffConveyerSensorPower_LA(bool On)=0;//PLC-關閉軌道感應器電源-LA

	//Lane B
	virtual bool               WriteOnInspection_LB(bool On)=0;//設定檢測中-LB	
	virtual bool               TurnOnConveryerLED_LB(bool On)=0;//開啟軌道LED-LB

	virtual bool               TurnOnStageAlarm_LB()=0;
	virtual bool               TurnOffStageAlarm_LB()=0;	

	virtual bool               TurnOnInspectAlarm_LB()=0;
	virtual bool               TurnOffInspectAlarm_LB()=0;	
	
	virtual bool               WriteConveryerClamp_LB(bool On)=0;//PLC-是否夾板-LB
	virtual bool               WriteConveryerStopBar_LB(bool On)=0;//PLC-停板器-LB	
	virtual int                ReadLanePLCErrorCode_LB()=0;//讀取PLC的軌道錯誤代碼-LB

	virtual bool               ExecPCBIn_LB(bool bWait)=0;//PLC-進板-LB
	virtual bool               WaitForPCBInFinish_LB()=0;//PLC-等進板完成-LB
	virtual bool               CheckPCBInFinish_LB()=0;//PLC-進板完成-LB
	virtual bool               CheckPCBInFault_LB()=0;//PLC-進板失敗-LB	

	virtual bool               ExecPCBIn2nd_LB(bool bWait)=0;//PLC-第2段進板-LB
	virtual bool               WaitForPCBIn2ndFinish_LB()=0;//PLC-第2段等進板完成-LB
	virtual bool               CheckPCBIn2ndFinish_LB()=0;//PLC-第2段進板完成-LB
	virtual bool               CheckPCBIn2ndFault_LB()=0;//PLC-第2段進板失敗-LB

	virtual bool               ExecPCBIn3rd_LB(bool bWait)=0;//PLC-第3段進板-LB
	virtual bool               WaitForPCBIn3rdFinish_LB()=0;//PLC-第3段等進板完成-LB
	virtual bool               CheckPCBIn3rdFinish_LB()=0;//PLC-第3段進板完成-LB
	virtual bool               CheckPCBIn3rdFault_LB()=0;//PLC-第3段進板失敗-LB

	virtual bool               ExecPCBBack_LB(bool bWait)=0;////PLC-退板-LB
	virtual bool               WaitForPCBBackFinish_LB()=0;//PLC-等退板完成-LB
	virtual bool               CheckPCBBackFinish_LB()=0;//PLC-退板完成-LB
	virtual bool               CheckPCBBackFault_LB()=0;//PLC-退板失敗-LB

	virtual bool               ExecPCBBackOut_LB(bool bWait)=0;//PLC-退出板-LB
	virtual bool               WaitForPCBBackOutFinish_LB()=0;//PLC-等退出板完成-LB
	virtual bool               CheckPCBBackOutFinish_LB()=0;//PLC-退出板完成-LB
	virtual bool               CheckPCBBackOutFault_LB()=0;//PLC-退出板失敗-LB	

	virtual bool               ExecPCBOut_LB(bool bWait)=0;//PLC-出板-LB
	virtual bool               WaitForPCBOutFinish_LB()=0;//PLC-等出板完成-LB
	virtual bool               CheckPCBOutFinish_LB()=0;//PLC-出板完成-LB
	virtual bool               CheckPCBOutFault_LB()=0;//PLC-出板失敗-LB

	virtual bool               ExecPCBOutInside_LB(bool bWait)=0;//PLC-出板至機台側邊-LB
	virtual bool               WaitForPCBOutInsideFinish_LB()=0;//PLC-等待出板至機台側邊-LB
	virtual bool               CheckPCBOutInsideFinish_LB()=0;//PLC-出板至機台側邊完成-LB
	virtual bool               CheckPCBOutInsideFault_LB()=0;//PLC-出板至機台側邊失敗-LB

	virtual bool               ExecPCBOutIn_LB(bool bWait)=0;//PLC-出板+進板-LB
	virtual bool               WaitForPCBOutInFinish_LB()=0;//PLC-等待出板+進板-LB
	virtual bool               CheckPCBOutInFinish_LB()=0;//PLC-出板+進板完成-LB
	virtual bool               CheckPCBOutInFault_LB()=0;//PLC-出板+進板失敗-LB

	virtual bool               ExecPCBReIn_LB(bool bWait)=0;//PLC-重新進板-LB
	virtual bool               CheckPCBReInFinish_LB()=0;//PLC-重新進板完成-LB
	virtual bool               WaitForPCBReInFinish_LB()=0;//PLC-等待重新進板完成-LB

	virtual bool               ExecPCBClear_LB(bool bWait)=0;//PLC-清板板-LB
	virtual bool               CheckPCBClearFinish_LB()=0;//PLC-確認清板完成-LB
	virtual bool               WaitForPCBClearFinish_LB()=0;//PLC-等待清板完成-LB

	virtual int                GetPCBAutoRunMode_LB();//自動進出板模式-LB	
	virtual bool               GetPCBAutoRunPCBChaned_LB() const;//自動進出板PCB改變過
	virtual void               SetPCBAutoRunPCBChaned_LB(bool val);//自動進出板PCB改變過
	virtual bool               StopPCBAutoOutIn_LB(bool bWait)=0;//PLC-停止自動進出板-LB
	virtual bool               ExecPCBAutoOutIn_LB(bool On, bool bWait)=0;//PLC-自動出進板-LB
	virtual bool               WaitForPCBAutoOutInFinish_LB()=0;//PLC-等待自動出進板-LB
	virtual bool               ReadPCBAutoOutInFinish_LB()=0;//PLC-自動出進板完成-LB
	virtual bool               CheckPCBAutoOutInFinish_LB()=0;//PLC-自動出進板完成-LB
	virtual bool               CheckPCBAutoOutInFault_LB()=0;//PLC-自動出進板失敗-LB
	virtual bool               WritePCBAutoOutInFinish_LB(bool On)=0;//PLC-寫入PCB自動出進完成-LB

	virtual bool               StopPCBAutoBackIn_LB(bool bWait)=0;//PLC-停止自動退進板-LB
	virtual bool               ExecPCBAutoBackIn_LB(bool On, bool bWait)=0;//PLC-自動退進板-LB
	virtual bool               WaitForPCBAutoBackInFinish_LB()=0;//PLC-等待自動退進板-LB
	virtual bool               ReadPCBAutoBackInFinish_LB()=0;//PLC-自動退進板完成-LB
	virtual bool               CheckPCBAutoBackInFinish_LB()=0;//PLC-自動退進板完成-LB
	virtual bool               CheckPCBAutoBackInFault_LB()=0;//PLC-自動退進板失敗-LB
	virtual bool               WritePCBAutoBackInFinish_LB(bool On)=0;//PLC-寫入PCB自動退進完成-LB

	virtual bool               CheckConveryerStopRunning_LB()=0;//PLC-確認軌道停止運轉-LB
	virtual bool               WaitForConveryerStopRunning_LB()=0;//PLC-等待軌道停止運轉-LB
	virtual bool               ExecConveyorMotorRunning_LB(bool On, bool bPositive, bool bSlow)=0;//PLC-執行軌道運轉-LB

	virtual bool               WriteSignalToLast_LB(bool On)=0;//PLC-送訊號給上一站-LB
	virtual bool               WriteSignalToNext_LB(bool On)=0;//PLC-送訊號給下一站-LB
	virtual bool               WriteOKSignalToNext_LB(bool On)=0;//PLC-送OK訊號給下一站-LB
	virtual bool               WriteNGSignalToNext_LB(bool On)=0;//PLC-送NG訊號給下一站-LB
	virtual bool               WriteLockSignal_LB(bool On)=0;//PLC-送鎖住訊號-LB
	virtual bool               WriteConveyerSensorPower_LB(bool On, bool bForce)=0;//PLC-關閉軌道感應器電源-LB
	virtual bool               TurnOffConveyerSensorPower_LB(bool On)=0;//PLC-關閉軌道感應器電源-LB	
	//---------------------------------------------------------------------------------//
	void                       ClearPlcNodeList();//PLC清除節點列表
	int                        GetPlcNodeCount() const;//PLC取得節點數量
	TPlcNode*                  GetPlcNodePtr(int index, bool bCheck);//PLC取得節點指標
	bool                       AddPlcNode(TPlcNode &Node);	//PLC新增節點	
	bool                       RemovePlcNode();//PLC刪除節點
	bool                       LoadExtraPlcNodeList();//PLC載入節點列表
	bool                       LoadExtraPlcNodeListFn();//PLC載入節點列表
	bool                       SaveExtraPlcNodeList();//PLC儲存節點列表	
	bool                       SaveExtraPlcNodeListFn();//PLC儲存節點列表	
	bool                       SetReadAllPlcNode();//設定讀取全部的PLC節點
	bool                       SetReadFnCodePlcNode();//設定讀取只有函式的PLC節點
	
	bool                       GetPlcNodeList(std::vector<TPlcNode> &PLCNodeList);//PLC取得節點列表	
	bool                       SetPlcNodeList(const std::vector<TPlcNode> &PLCNodeList);//PLC設定節點列表	
	//---------------------------------------------------------------------------------//			
	bool                       GetHardwareBypass_LA() const { return m_HardwareByPass_LA; }	
	bool                       GetHardwareBypass_LB() const { return m_HardwareByPass_LB; }	
	bool                       GetIsPCBRightInDirection() const { return m_PLC_RightIn; }	
	bool                       GetPLC_FanAlarm() const { return m_PLC_FanAlarm; }
	bool                       GetSafetyAlarm() const { return m_PLC_SafetyAlarm; }
	bool                       GetSafetyAlarmEMS() const { return m_PLC_SafetyAlarmEMS; }	
	//---------------------------------------------------------------------------------//
	//與上一站接線模式
	void                       SetLastStationLineMode(int val) { m_LastStationLineMode = val; }
	int                        GetLastStationLineMode() const { return m_LastStationLineMode; }
	//---------------------------------------------------------------------------------//			
	void                       SetPCBAutoRunMode_LA(int Mode);//自動進出板模式
	void                       SetPCBAutoRunMode_LB(int Mode);//自動進出板模式	
	//---------------------------------------------------------------------------------//
	void                       SetConveryerSensorPower_LA(bool val);//設定軌道感測器電源-A軌
	bool                       GetConveryerSensorPower_LA() const;//取得軌道感測器電源-A軌
	void                       SetConveryerSensorPower_LB(bool val);//設定軌道感測器電源-B軌
	bool                       GetConveryerSensorPower_LB() const;//取得軌道感測器電源-B軌
	//---------------------------------------------------------------------------------//
	//軌道運作時間
	DWORD                      GetPCBInTime(LANE_ID LaneID) const;
	DWORD                      GetPCBOutTime(LANE_ID LaneID) const;
	DWORD                      GetPCBBackTime(LANE_ID LaneID) const;
	
	//軌道A運作時間
	DWORD                      GetPCBInTime_LA() const;
	DWORD                      GetPCBOutTime_LA() const;
	DWORD                      GetPCBBackTime_LA() const;

	//軌道B運作時間
	DWORD                      GetPCBInTime_LB() const;
	DWORD                      GetPCBOutTime_LB() const;
	DWORD                      GetPCBBackTime_LB() const;
	//---------------------------------------------------------------------------------//
	//軌道的感測器
	bool                       CheckPCBInside(LANE_ID LaneID) const;
	bool                       GetConveryerClamp(LANE_ID LaneID) const;
	bool                       GetConveryerStopBar(LANE_ID LaneID) const;	
	bool                       GetConveryerSensorPCBIn(LANE_ID LaneID) const;
	bool                       GetConveryerSensorPCBOut(LANE_ID LaneID) const;
	bool                       GetConveryerSensorPCBStop(LANE_ID LaneID) const;
	bool                       GetConveryerSensorPCBSlow(LANE_ID LaneID) const;
	bool                       GetConveryerSensorPCBStop2(LANE_ID LaneID) const;
	bool                       GetLastStationSignal(LANE_ID LaneID) const;
	bool                       GetNextStationSignal(LANE_ID LaneID) const;
	bool                       GetLockStationSignal(LANE_ID LaneID) const;
	bool                       GetSendToLastStation(LANE_ID LaneID) const;
	bool                       GetSendToNextStation(LANE_ID LaneID) const;
	bool                       GetSendToNextStationOK(LANE_ID LaneID) const;
	bool                       GetSendToNextStationNG(LANE_ID LaneID) const;	
	int                        GetConveryerStatus(LANE_ID LaneID) const;
	bool                       GetConveryerStatusText(int Val, CString &Text) const;

	//軌道A軌的感測器	
	bool                       CheckPCBInside_LA() const;
	void                       SetConveryerEnabled_LA(bool val) { m_ConveryerEnabled_LA=val; }
	bool                       GetConveryerEnabled_LA() const { return m_ConveryerEnabled_LA; }

	bool                       GetConveryerClamp_LA() const { return m_ConveryerClamp_LA; }	
	bool                       GetConveryerStopBar_LA() const { return m_ConveryerStopBar_LA; }	
	bool                       GetConveryerSensorPCBIn_LA() const { return m_PLC_I_SensorPCBIn_LA; }	
	bool                       GetConveryerSensorPCBOut_LA() const { return m_PLC_I_SensorPCBOut_LA; }
	bool                       GetConveryerSensorPCBStop_LA() const { return m_PLC_I_SensorPCBStop_LA; }
	bool                       GetConveryerSensorPCBSlow_LA() const { return m_PLC_I_SensorPCBSlow_LA; }
	bool                       GetConveryerSensorPCBStop2_LA() const { return m_PLC_I_SensorPCBStop2_LA; }	
	bool                       GetLastStationSignal_LA() const { return m_PLC_I_SignalFromLast_LA; }
	bool                       GetNextStationSignal_LA() const { return m_PLC_I_SignalFromNext_LA; }
	bool                       GetLockStationSignal_LA() const { return m_PLC_O_SignalLockStation_LA; }
	bool                       GetSendToLastStation_LA() const { return m_PLC_O_SignalToLast_LA; }
	bool                       GetSendToNextStation_LA() const { return m_PLC_O_SignalToNext_LA; }
	bool                       GetSendToNextStationOK_LA() const { return m_PLC_O_SignalToNextOK_LA; }
	bool                       GetSendToNextStationNG_LA() const { return m_PLC_O_SignalToNextNG_LA; }
	bool                       GetPCBBargeIn_LA() const { return m_PLC_O_PCBBargeIn_LA; }	
	int                        GetConveryerStatus_LA() const { return m_PLC_O_ConveryerStatus_LA; }	

	//軌道B軌的感測器	
	bool                       CheckPCBInside_LB() const;
	void                       SetConveryerEnabled_LB(bool val) { m_ConveryerEnabled_LB=val; }
	bool                       GetConveryerEnabled_LB() const { return m_ConveryerEnabled_LB; }

	bool                       GetConveryerClamp_LB() const { return m_ConveryerClamp_LB; }	
	bool                       GetConveryerStopBar_LB() const { return m_ConveryerStopBar_LB; }	
	bool                       GetConveryerSensorPCBIn_LB() const { return m_PLC_I_SensorPCBIn_LB; }
	bool                       GetConveryerSensorPCBOut_LB() const { return m_PLC_I_SensorPCBOut_LB; }
	bool                       GetConveryerSensorPCBStop_LB() const { return m_PLC_I_SensorPCBStop_LB; }
	bool                       GetConveryerSensorPCBSlow_LB() const { return m_PLC_I_SensorPCBSlow_LB; }
	bool                       GetConveryerSensorPCBStop2_LB() const { return m_PLC_I_SensorPCBStop2_LB; }
	bool                       GetLastStationSignal_LB() const { return m_PLC_I_SignalFromLast_LB; }
	bool                       GetNextStationSignal_LB() const { return m_PLC_I_SignalFromNext_LB; }
	bool                       GetLockStationSignal_LB() const { return m_PLC_O_SignalLockStation_LB; }
	bool                       GetSendToLastStation_LB() const { return m_PLC_O_SignalToLast_LB; }
	bool                       GetSendToNextStation_LB() const { return m_PLC_O_SignalToNext_LB; }
	bool                       GetSendToNextStationOK_LB() const { return m_PLC_O_SignalToNextOK_LB; }
	bool                       GetSendToNextStationNG_LB() const { return m_PLC_O_SignalToNextNG_LB; }
	bool                       GetPCBBargeIn_LB() const { return m_PLC_O_PCBBargeIn_LB; }	
	int                        GetConveryerStatus_LB() const { return m_PLC_O_ConveryerStatus_LB; }

	//現況訊號	
	bool                       GetCurrentAirStats() const { return m_PLC_I_AirLost; }
	bool                       GetCurrentFrontCapStats() const { return m_PLC_I_FrontCapOpened; }
	bool                       GetCurrentRearCapStats() const { return m_PLC_I_RearCapOpened; }
	bool                       GetCurrentKeySwitchStats() const { return m_PLC_I_KeySwitchOff; }	
	bool                       GetCurrentRedLightStats() const { return m_PLC_O_LightStop; }
	bool                       GetCurrentGreenLightStats() const { return m_PLC_O_LightStart; }
	bool                       GetCurrentEMSStats() const { return m_PLC_I_EMSOn; }	
	bool                       GetBtnLightRed() const { return m_PLC_I_BtnStop; }	
	bool                       GetBtnLightGreen() const { return m_PLC_I_BtnStart; }
	bool                       GetBtnLightYellow() const { return m_PLC_I_BtnReset; }		
	bool                       GetCurrentOverHeat() const { return m_PLC_I_OverHeat; }
	bool                       GetCurrentFanAlarm() const { return m_PLC_I_FanAlarm; }

	//電源開關
	bool                       GetLinearMotorACPower() const { return m_PLC_O_PowerACMotor; }			
	bool                       GetDayLightPower() const { return m_PLC_O_LightDay; }
	
	//安全檢測	
	bool                       GetSaftyBypass() const { return m_SaftyBypass; }

	//異常
	bool                       CheckPLCExecAlarm();//確認PLC執行異常
	bool                       GetPLCExecAlarm() const { return m_PLCExecAlarm; }	
	bool                       GetStageAlarm_LA() const { return m_StageAlarm_LA; }
	bool                       GetInspectionAlarm_LA() const { return m_InspectionAlarm_LA; }
	bool                       GetStageAlarm_LB() const { return m_StageAlarm_LB; }
	bool                       GetInspectionAlarm_LB() const { return m_InspectionAlarm_LB; }

	//塔燈
	void                       SetDualTowerLight(bool val) { m_DualTowerLight=val; }
	bool                       GetDualTowerLight() const { return m_DualTowerLight; }

	bool                       GetBuzzer_LA() const { return m_PLC_O_Buzzer_LA; }
	bool                       GetTowerLightRed_LA() const { return m_PLC_O_LightTowerRed_LA; }
	bool                       GetTowerLightYellow_LA() const { return m_PLC_O_LightTowerYel_LA; }
	bool                       GetTowerLightGreen_LA() const { return m_PLC_O_LightTowerGrn_LA; }

	bool                       GetBuzzer_LB() const { return m_PLC_O_Buzzer_LB; }
	bool                       GetTowerLightRed_LB() const { return m_PLC_O_LightTowerRed_LB; }
	bool                       GetTowerLightYellow_LB() const { return m_PLC_O_LightTowerYel_LB; }
	bool                       GetTowerLightGreen_LB() const { return m_PLC_O_LightTowerGrn_LB;  }
	//---------------------------------------------------------------------------------//	
	bool                       IncrementPCBInCount(LANE_ID LaneID);//疊加進板確認次數
	bool                       IncrementPCBOutCount(LANE_ID LaneID);//疊加出板確認次數
	bool                       IncrementPCBBackCount(LANE_ID LaneID);//疊加回板確認次數
	bool                       IncrementPCBOutInCheckCount(LANE_ID LaneID);//疊加出板帶進板確認次數

	bool                       IncrementPCBInCount_LA();//疊加進板確認次數-LA
	bool                       IncrementPCBOutCount_LA();//疊加出板確認次數-LA
	bool                       IncrementPCBBackCount_LA();//疊加回板確認次數-LA
	bool                       IncrementPCBOutInCheckCount_LA();//疊加出板帶進板確認次數-LA

	bool                       IncrementPCBInCount_LB();//疊加進板確認次數-LB
	bool                       IncrementPCBOutCount_LB();//疊加出板確認次數-LB
	bool                       IncrementPCBBackCount_LB();//疊加回板確認次數-LB
	bool                       IncrementPCBOutInCheckCount_LB();//疊加出板帶進板確認次數-LB

	int                        GetPCBInOutTimeout(LANE_ID LaneID) const;
	int                        GetPCBInOutTimeout_LA() const;
	int                        GetPCBInOutTimeout_LB() const;	
	//---------------------------------------------------------------------------------//
	bool                       CheckPLCReady(bool bChkStartLight, bool bAutoReset);//確認PLC狀態OK
	//---------------------------------------------------------------------------------//
	virtual bool               PushDownStartBtn()=0;//按下啟動燈
	virtual bool               PushDownResetBtn()=0;//按下復歸燈	
	virtual bool               PushDownStopBtn()=0;	//按下停止燈
	//---------------------------------------------------------------------------------//
	//塔燈設定	
	int                        GetTowerLightState_Stop(LANE_ID LaneID) const;
	void                       SetTowerLightState_Stop(LANE_ID LaneID, int State);	
	void                       SetTowerLightMode(int State) { m_TowerLightMode=State;}
	int                        GetTowerLightMode() const { return m_TowerLightMode; }
	void                       SetTowerLightState_Stop(int State) { m_TowerLightState_Stop=State;}
	int                        GetTowerLightState_Stop() const { return m_TowerLightState_Stop; }
	void                       SetTowerLightState_Inspection(int State) { m_TowerLightState_Inspection=State;}
	int                        GetTowerLightState_Inspection() const { return m_TowerLightState_Inspection; }
	void                       SetTowerLightState_Bypass(int State) { m_TowerLightState_Bypass=State;}
	int                        GetTowerLightState_Bypass() const { return m_TowerLightState_Bypass; }
	void                       SetTowerLightState_WaitLast(int State) { m_TowerLightState_WaitLast=State;}
	int                        GetTowerLightState_WaitLast() const { return m_TowerLightState_WaitLast; }
	void                       SetTowerLightState_WaitNext(int State) { m_TowerLightState_WaitNext=State;}
	int                        GetTowerLightState_WaitNext() const { return m_TowerLightState_WaitNext; }
	void                       SetTowerLightState_PCBIn(int State) { m_TowerLightState_PCBIn=State;}
	int                        GetTowerLightState_PCBIn() const { return m_TowerLightState_PCBIn; }
	void                       SetTowerLightState_PCBOut(int State) { m_TowerLightState_PCBOut=State;}
	int                        GetTowerLightState_PCBOut() const { return m_TowerLightState_PCBOut; }
	void                       SetTowerLightState_PCBBack(int State) { m_TowerLightState_PCBBack=State;}
	int                        GetTowerLightState_PCBBack() const { return m_TowerLightState_PCBBack; }
	//---------------------------------------------------------------------------------//
	//軌道間距馬達
	virtual bool               ReadLaneAdjustHomeDone(LANE_ID LaneID);
	virtual bool               CheckLaneAdjustCanMove(LANE_ID LaneID);
	virtual bool               GetLaneAdjustCanMove(LANE_ID LaneID) const;
	virtual bool               GetLaneAdjustHomeDone(LANE_ID LaneID) const;		
	virtual bool               GetLaneAdjustCanMove_LA() const;//間距馬達-是否可以移動
	virtual bool               GetLaneAdjustCanMove_LB() const;//間距馬達-是否可以移動
	virtual bool               CheckLaneAdjustCanMove_LA();//間距馬達-是否可以移動
	virtual bool               CheckLaneAdjustCanMove_LB();//間距馬達-是否可以移動
	virtual bool               CheckLaneAdjustHomeDone_LA();//間距馬達-是否已經歸零
	virtual bool               CheckLaneAdjustHomeDone_LB();//間距馬達-是否已經歸零
	virtual bool               CheckLaneAdjustDisableBtn_LA();//間距馬達-是否消磁	
	virtual bool               CheckLaneAdjustDisableBtn_LB();//間距馬達-是否消磁	

	virtual bool               GetLaneAdjustFixed14Lane() const;//軌道調整-14軌固定
	virtual int                GetLaneAdjustHomeTimeout() const;//軌道調整-歸零逾時
	virtual int                GetLaneAdjustMoveTimeout() const;//軌道調整-歸零逾時		
	virtual bool               GetLaneAdjustDisableBtn_LA() const;//間距馬達-是否消磁	
	virtual bool               GetLaneAdjustDisableBtn_LB() const;//間距馬達-是否消磁	
	virtual bool               GetLaneAdjustDisableBtn(LANE_ID LaneID) const;//間距馬達-是否消磁	
	virtual double             GetLaneAdjustCurrentPos(LANE_ID LaneID) const;	
	virtual bool               SetLaneAdjustCurrentPos(LANE_ID LaneID, double Pos);	
	virtual bool               ExecLaneAdjustMoveToPos(LANE_ID LaneID, double Pos);
	virtual bool               ExecLaneAdjustHomeSearch(LANE_ID LaneID, bool Wait);	
	virtual bool               CheckLaneAdjustPCBInside(LANE_ID LaneID);//間距馬達-確認PCB板在內
	virtual double             ReadLaneAdjustCurrentPos(LANE_ID LaneID);//間距馬達-取回目前位置	

	virtual bool               WriteLaneAdjustManual(bool On)=0;//間距馬達-手動			
	virtual bool               WriteLaneAdjustFixed14Lane(bool On)=0;//間距馬達-固定軌道模式		
	virtual bool               WriteLaneAdjustHomeTimeout(int time)=0;//間距馬達-歸零逾時
	virtual bool               WriteLaneAdjustMoveTimeout(int time)=0;//間距馬達-移動逾時
	
	virtual bool               InitialLaneAdjust_LA()=0;//間距馬達-初始化		
	virtual bool               WriteLaneAdjustCmdPos_LA(double Pos)=0;//間距馬達-設定命令位置	
	virtual bool               WriteLaneAdjustToHome_LA(bool On)=0;//間距馬達-歸零搜尋
	virtual bool               WriteLaneAdjustToMove_LA(bool On)=0;//間距馬達-開始移動
	virtual bool               WriteLaneAdjustJogMove_LA(bool Dir, bool Stop)=0;//間距馬達-搖桿移動
	virtual bool               WriteLaneAdjustLimitMaxPos_LA(double Pos)=0;//間距馬達-設定最大位置
	virtual bool               WriteLaneAdjustJogSpeed_LA(double Speed)=0;//間距馬達-設定Jog速度
	virtual bool               WriteLaneAdjustSkewPitch_LA(double Pitch)=0;//間距馬達-設定移動距離		
	virtual bool               ResetLaneAdjustException_LA()=0;//間距馬達-復歸異常	
	
	virtual bool               ReadLaneAdjustDisable_LA()=0;//間距馬達-是否關閉	
	virtual bool               ReadLaneAdjustSensorORG_LA()=0;//間距馬達-確認是否在原點
	virtual bool               ReadLaneAdjustSensorLimit_LA()=0;//間距馬達-確認是否在極限	
	virtual bool               ReadLaneAdjustExecption_LA()=0;//間距馬達-確認是否異常
	virtual bool               ReadLaneAdjustHomeDone_LA()=0;//間距馬達-確認是否歸零完畢
	virtual bool               ReadLaneAdjustMoveDone_LA()=0;//間距馬達-確認是否移動完畢
	virtual double             ReadLaneAdjustCurrentPos_LA()=0;//間距馬達-取回目前位置
	virtual int                ReadLaneAdjustPLCErrorCode_LA()=0;//間距馬達-取回PLC的錯誤代碼
	virtual int                ReadLaneAdjustMotorErrorCode_LA()=0;//間距馬達-取回馬達的錯誤代碼	
	
	virtual bool               GetLaneAdjustHomeDone_LA() const;
	virtual double             GetLaneAdjustLimitMax_LA() const;
	virtual double             GetLaneAdjustLimitMin_LA() const;
	virtual double             GetLaneAdjustSkewPitch_LA() const;//間距馬達-設定移動距離
	virtual double             GetLaneAdjustCurrentPos_LA() const;
	virtual void               SetLaneAdjustCurrentPos_LA(double val);
	virtual double             GetLaneAdjustJogSlowSpeed_LA() const;
	virtual double             GetLaneAdjustJogFastSpeed_LA() const;
	virtual bool               GetLaneAdjustSensorORG_LA() const;
	virtual bool               GetLaneAdjustSensorLimit_LA() const;
	virtual bool               GetLaneAdjustJogMoving_LA() const;	
	virtual bool               CheckLaneAdjustPCBInside_LA();	
	virtual bool               ExecLaneAdjustHomeSearch_LA(bool Wait);//間距馬達-歸零搜尋
	virtual bool               ExecLaneAdjustMoveTo_LA(double Pos, bool Wait);//間距馬達-移動至
	virtual bool               ExecLaneAdjustJogMove_LA(bool Dir, bool Stop);//間距馬達-Jog移動

	virtual bool               InitialLaneAdjust_LB()=0;//間距馬達-初始化		
	virtual bool               WriteLaneAdjustCmdPos_LB(double Pos)=0;//間距馬達-設定命令位置	
	virtual bool               WriteLaneAdjustToHome_LB(bool On)=0;//間距馬達-歸零搜尋
	virtual bool               WriteLaneAdjustToMove_LB(bool On)=0;//間距馬達-開始移動
	virtual bool               WriteLaneAdjustJogMove_LB(bool Dir, bool Stop)=0;//間距馬達-搖桿移動
	virtual bool               WriteLaneAdjustLimitMaxPos_LB(double Pos)=0;//間距馬達-設定最大位置
	virtual bool               WriteLaneAdjustJogSpeed_LB(double Speed)=0;//間距馬達-設定Jog速度
	virtual bool               WriteLaneAdjustSkewPitch_LB(double Pitch)=0;//間距馬達-設定移動距離		
	virtual bool               ResetLaneAdjustException_LB()=0;//間距馬達-復歸異常
	
	virtual bool               ReadLaneAdjustDisable_LB()=0;//間距馬達-是否關閉	
	virtual bool               ReadLaneAdjustSensorORG_LB()=0;//間距馬達-確認是否在原點
	virtual bool               ReadLaneAdjustSensorLimit_LB()=0;//間距馬達-確認是否在極限	
	virtual bool               ReadLaneAdjustExecption_LB()=0;//間距馬達-確認是否異常
	virtual bool               ReadLaneAdjustHomeDone_LB()=0;//間距馬達-確認是否歸零完畢
	virtual bool               ReadLaneAdjustMoveDone_LB()=0;//間距馬達-確認是否移動完畢
	virtual double             ReadLaneAdjustCurrentPos_LB()=0;//間距馬達-取回目前位置
	virtual int                ReadLaneAdjustPLCErrorCode_LB()=0;//間距馬達-取回PLC的錯誤代碼
	virtual int                ReadLaneAdjustMotorErrorCode_LB()=0;//間距馬達-取回馬達的錯誤代碼
	
	virtual bool               GetLaneAdjustHomeDone_LB() const;
	virtual double             GetLaneAdjustLimitMax_LB() const;
	virtual double             GetLaneAdjustLimitMin_LB() const;
	virtual double             GetLaneAdjustSkewPitch_LB() const;//間距馬達-設定移動距離	
	virtual double             GetLaneAdjustCurrentPos_LB() const;
	virtual void               SetLaneAdjustCurrentPos_LB(double val);
	virtual double             GetLaneAdjustJogSlowSpeed_LB() const;
	virtual double             GetLaneAdjustJogFastSpeed_LB() const;	
	virtual bool               GetLaneAdjustSensorORG_LB() const;
	virtual bool               GetLaneAdjustSensorLimit_LB() const;
	virtual bool               GetLaneAdjustJogMoving_LB() const;	
	virtual bool               CheckLaneAdjustPCBInside_LB();
	virtual bool               ExecLaneAdjustHomeSearch_LB(bool Wait);//間距馬達-歸零搜尋
	virtual bool               ExecLaneAdjustMoveTo_LB(double Pos, bool Wait);//間距馬達-移動至
	virtual bool               ExecLaneAdjustJogMove_LB(bool Dir, bool Stop);//間距馬達-Jog移動
	//---------------------------------------------------------------------------------//
};
//-------------------------------------------------------------------------------------//
extern CPLC_Basic *PlcCtrlPtr;
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_PLC_BASIC_H__B7922FA4_F633_443D_AE48_B522285D7B7A__INCLUDED_)
