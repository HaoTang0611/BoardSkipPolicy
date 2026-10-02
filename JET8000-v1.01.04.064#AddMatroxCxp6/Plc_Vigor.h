// Plc_Vigor.h: interface for the CPLC_Vigor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLCOBJ_VIGOR_H__80C038EF_3902_4E62_8267_1B3C0124F841__INCLUDED_)
#define AFX_PLCOBJ_VIGOR_H__80C038EF_3902_4E62_8267_1B3C0124F841__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include "Plc_Basic.h"
//-------------------------------------------------------------------------------------//
#if PLC_OBJ_MODE == PLC_OBJ_VIGOR
//---------------------------------------------------------------------------------//	
#include "JetSerial.h"   // RS232C application program
//---------------------------------------------------------------------------------//	
#define   AOI_MONITOR_RUN                        1
#define   AOI_MONITOR_STOP                       2
//---------------------------------------------------------------------------------//	
enum PLC_VIGOR_NODE_FNC_MODE
{
	PLC_VIGOR_NODE_FNC_NONE=0,//無效用
	PLC_VIGOR_NODE_FNC_LANE_A_DCODE,//Lane A D Code
	PLC_VIGOR_NODE_FNC_LANE_B_DCODE,//Lane B D Code
	PLC_VIGOR_NODE_FNC_PCB_IN_DIRECTION,//PCB In Direction
	PLC_VIGOR_NODE_FNC_LOCK_OTHER_MACHINE_LA,//Lock Other Machine LA
	PLC_VIGOR_NODE_FNC_LOCK_OTHER_MACHINE_LB,//Lock Other Machine LB
	PLC_VIGOR_NODE_FNC_CURRENT_AIR,//Current Air States
	PLC_VIGOR_NODE_FNC_CURRENT_CAP,//Current Cap States
	PLC_VIGOR_NODE_FNC_CURRENT_REAR_DOOR,//Current Rear Door States
	PLC_VIGOR_NODE_FNC_CURRENT_KEY_SWITCH,//Current Key Swithch States
	PLC_VIGOR_NODE_FNC_CURRENT_START_LIGHT,//Current Start Light States
	PLC_VIGOR_NODE_FNC_CURRENT_EMS,//Current EMS States
	PLC_VIGOR_NODE_FNC_KEEP_CAP,//Keep Cap States
	PLC_VIGOR_NODE_FNC_KEEP_REAR_DOOR,//Keep Rear Door States
	PLC_VIGOR_NODE_FNC_KEEP_AIR,//Keep Air States
	PLC_VIGOR_NODE_FNC_KEEP_EMS,//Keep EMS States
	PLC_VIGOR_NODE_FNC_XYZ_MOTOR_AC_POWER,//XYZ Motor AC Power
	PLC_VIGOR_NODE_FNC_SAFTY_PASS,//Safty Pass
	PLC_VIGOR_NODE_FNC_LIGHT_POWER//Light Power
};
//-------------------------------------------------------------------------------------//
class CPLC_Vigor : public CPLC_Basic  
{
	//---------------------------------------------------------------------------------//	
	DECLARE_DYNAMIC(CPLC_Vigor)
	//---------------------------------------------------------------------------------//	
private:
	//---------------------------------------------------------------------------------//	
	CJetSerial                 m_RS232;           // RS232C program
	char                       m_StageNumber;     // 
	int                        m_CommCheckCount;  
	//---------------------------------------------------------------------------------//		
	char m_SendBuffer[JET_PLC_BUFFER_SIZE];
	char m_ReceiveBuffer[JET_PLC_BUFFER_SIZE];
	//---------------------------------------------------------------------------------//
	bool                       m_ConveyorMotorForward_LA;
	bool                       m_ConveyorMotorSlowDown_LA;
	bool                       m_ConveyorMotorForward_LB;
	bool                       m_ConveyorMotorSlowDown_LB;
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//
	bool GetConveyorMotorForward_LA() const;
	void SetConveyorMotorForward_LA(bool val);
	bool GetConveyorMotorSlowDown_LA() const;
	void SetConveyorMotorSlowDown_LA(bool val);
	//---------------------------------------------------------------------------------//
	bool GetConveyorMotorForward_LB() const;
	void SetConveyorMotorForward_LB(bool val);
	bool GetConveyorMotorSlowDown_LB() const;
	void SetConveyorMotorSlowDown_LB(bool val);	
	//---------------------------------------------------------------------------------//
	void MakeWord(char hightbyte, char lowbyte, char &word);    // For PLC command      
	void GetBytes(char Value, char &HightByte, char &LowByte);  // check sum value
	bool GetUnitsInformation(const char* unit,int &DataAdress, int &point);  // adjust relay number is correct?
	bool CheckErrorNumber(int ErrorNumber);   // check retrun string of PLC
	bool ReadPLCVersion();
	bool ReadPLCSaftyBypass();
	bool ReadPLCDualLaneMode();
	bool ReadPLCDualTowerLight();
	bool ReadPLCStageAlarm();
	bool ReadPLCInspectionAlarm();
	bool ResetPLCFinishState();//復歸PLC完成訊號
	//---------------------------------------------------------------------------------//
	bool ReadSingle(const char* unit, bool &stats);
	bool WriteSingle(const char* unit, bool stats);
	//---------------------------------------------------------------------------------//
	bool ReadData(const char* StartUnit, bool Stats[], int counts, int &point);  // Read data from COM
	bool WriteData(const char* StartUnit, bool Stats[], int counts);
	//---------------------------------------------------------------------------------//
	bool DoReadDCode(int DN, char data[]);//Read D Conde, in 16-bit mode	
	bool DoWriteDCode(int DN, char data[]);//Write D Conde, in 16-bit mode
	//---------------------------------------------------------------------------------//
	bool WriteDCode(int DN, char data[]);//Write D Conde, in 16-bit mode
	bool ReadDCode(int DN, char data[]);//Read D Conde, in 16-bit mode		
	//---------------------------------------------------------------------------------//
	int  TransferDcodeToINT(const char *pDCode);
	void TransferINTToDcodeData(int value, char Data[], int DataCnt=4);
	//---------------------------------------------------------------------------------//
	int  GetCodeNumber(const char *CodeName);
	//---------------------------------------------------------------------------------//
	bool                       ReturnPLCNotSupportFunc(LPCTSTR fnName);//回傳PLC未支援函式
	//---------------------------------------------------------------------------------//
	bool                       ReadLaneFault_LA();//PLC-確認軌道異常-LA
	bool                       ReadLaneFault_LB();//PLC-確認軌道異常-LB
	//---------------------------------------------------------------------------------//
	bool                       WritePCBBackOut_LA(bool On);//PLC-寫入PCB退出板-LA
	bool                       WritePCBBackOut_LB(bool On);//PLC-寫入PCB退出板-LB
	//---------------------------------------------------------------------------------//
	bool                       WritePCBAutoOutIn_LA(bool On);//PLC-寫入PCB自動出進-LA
	bool                       WritePCBAutoOutIn_LB(bool On);//PLC-寫入PCB自動出進-LB	
	//---------------------------------------------------------------------------------//
	bool                       WritePCBAutoBackIn_LA(bool On);//PLC-寫入PCB自動退進-LA
	bool                       WritePCBAutoBackIn_LB(bool On);//PLC-寫入PCB自動退進-LB	
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	CPLC_Vigor();
	virtual ~CPLC_Vigor();
	//---------------------------------------------------------------------------------//	
	virtual bool               InitialPLC();
	virtual bool               PLC_ConnectTo(LPCTSTR Param);
	virtual bool               PLC_Disconnect();
	
	virtual bool               PLC_BuildPlcNodeList();
	virtual bool               PLC_ReadAllStats(bool bCheckThread);	
	virtual bool               PLC_ReadExecAlarm();//讀取執行異常
	virtual bool               PLC_ReadTowerLight(bool bCheckThread);//讀取塔燈
	virtual bool               PLC_ReadMachineSensor(bool bCheckThread);//讀取機台感測器	
	virtual bool               PLC_ReadConveryerSensor(bool bCheckThread);//讀取軌道感應器
	virtual bool               PLC_ReadLaneAdjustStatus(bool bCheckThread);//讀取軌道間距狀態
	virtual bool               PLC_ReadConveryerRunStatus(bool bCheckThread);//讀取軌道運轉狀態
	virtual bool               PLC_ReadConveryerSignalSend(bool bCheckThread);//讀取軌道訊號
	virtual bool               PLC_ReadPlcNodeList(bool bCheckThread);//PLC讀取PLC節點列表
	virtual bool               PLC_ReadNode(const char *Node, int &value);
	virtual bool               PLC_WriteNode(const char *Node, int value);
	virtual bool               PLC_WriteINIParameter();
	virtual bool               PLC_WriteINIParameterFn();
	virtual CString            LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);//載入多國語系
	virtual CString            GetPLCErrorCodeText(int ErrorCode);//取得PLC錯誤帶碼文字
	//---------------------------------------------------------------------------------//	
	//操作	
	virtual bool               TurnOnOffTargetCap(bool On);//PLC-開關塊規上蓋	

	virtual bool               WritePLCReset();
	virtual bool               WriteBypassSafty(bool On);//關閉安全檢測
	virtual bool               WriteDualLaneMode(bool On);//設定是否為雙軌道模式
	virtual bool               WriteDualTowerLight(bool On);//設定是否為雙塔燈模式

	virtual bool               WriteTowerLightMode(int Mode);//設定是否為塔燈模式
	virtual bool               WriteTowerLightState_Stop(int Value);//設定塔燈狀態-停止
	virtual bool               WriteTowerLightState_Stop(LANE_ID LaneID, int Value);//設定塔燈狀態-停止
	virtual bool               WriteTowerLightState_Inspection(int Value);//設定塔燈狀態-檢測中
	virtual bool               WriteTowerLightState_Bypass(int Value);//設定塔燈狀態-直通
	virtual bool               WriteTowerLightState_WaitLast(int Value);//設定塔燈狀態-等上站
	virtual bool               WriteTowerLightState_WaitNext(int Value);//設定塔燈狀態-等下站
	virtual bool               WriteTowerLightState_PCBIn(int Value);//設定塔燈狀態-進板
	virtual bool               WriteTowerLightState_PCBOut(int Value);//設定塔燈狀態-出板
	virtual bool               WriteTowerLightState_PCBBack(int Value);//設定塔燈狀態-退板

	//Lane A
	virtual bool               WriteOnInspection_LA(bool On);//設定檢測中-LA	
	virtual bool               TurnOnConveryerLED_LA(bool On);//開啟軌道LED-LA

	virtual bool               TurnOnStageAlarm_LA();//機台異常-LA
	virtual bool               TurnOffStageAlarm_LA();//機台異常-LA

	virtual bool               TurnOnInspectAlarm_LA();//檢測異常-LA
	virtual bool               TurnOffInspectAlarm_LA();//檢測異常-LA
	
	virtual bool               WriteConveryerClamp_LA(bool On);//PLC-是否夾板-LA	         
	virtual bool               WriteConveryerStopBar_LA(bool On);//PLC-停板器-LA	
	virtual int                ReadLanePLCErrorCode_LA();//讀取PLC的軌道錯誤代碼-LA

	virtual bool               ExecPCBIn_LA(bool bWait);//PLC-進板-LA
	virtual bool               WaitForPCBInFinish_LA();//PLC-等進板完成-LA
	virtual bool               CheckPCBInFinish_LA();//PLC-進板完成-LA
	virtual bool               CheckPCBInFault_LA();//PLC-進板失敗-LA

	virtual bool               ExecPCBIn2nd_LA(bool bWait);//PLC-第2段進板-LA
	virtual bool               WaitForPCBIn2ndFinish_LA();//PLC-第2段等進板完成-LA
	virtual bool               CheckPCBIn2ndFinish_LA();//PLC-第2段進板完成-LA
	virtual bool               CheckPCBIn2ndFault_LA();//PLC-第2段進板失敗-LA	

	virtual bool               ExecPCBIn3rd_LA(bool bWait);//PLC-第3段進板-LA
	virtual bool               WaitForPCBIn3rdFinish_LA();//PLC-第3段等進板完成-LA
	virtual bool               CheckPCBIn3rdFinish_LA();//PLC-第3段進板完成-LA
	virtual bool               CheckPCBIn3rdFault_LA();//PLC-第3段進板失敗-LA

	virtual bool               ExecPCBBack_LA(bool bWait);////PLC-退板-LA
	virtual bool               WaitForPCBBackFinish_LA();//PLC-等退板完成-LA
	virtual bool               CheckPCBBackFinish_LA();//PLC-退板完成-LA
	virtual bool               CheckPCBBackFault_LA();//PLC-退板失敗-LA

	virtual bool               ExecPCBBackOut_LA(bool bWait);//PLC-退出板-LA
	virtual bool               WaitForPCBBackOutFinish_LA();//PLC-等退出板完成-LA
	virtual bool               CheckPCBBackOutFinish_LA();//PLC-退出板完成-LA
	virtual bool               CheckPCBBackOutFault_LA();//PLC-退出板失敗-LA	

	virtual bool               ExecPCBOut_LA(bool bWait);//PLC-出板-LA
	virtual bool               WaitForPCBOutFinish_LA();//PLC-等出板完成-LA
	virtual bool               CheckPCBOutFinish_LA();//PLC-出板完成-LA
	virtual bool               CheckPCBOutFault_LA();//PLC-出板失敗-LA

	virtual bool               ExecPCBOutInside_LA(bool bWait);//PLC-出板至機台側邊-LA
	virtual bool               WaitForPCBOutInsideFinish_LA();//PLC-等待出板至機台側邊-LA
	virtual bool               CheckPCBOutInsideFinish_LA();//PLC-出板至機台側邊完成-LA
	virtual bool               CheckPCBOutInsideFault_LA();//PLC-出板至機台側邊失敗-LA

	virtual bool               ExecPCBOutIn_LA(bool bWait);//PLC-出板+進板-LA
	virtual bool               WaitForPCBOutInFinish_LA();//PLC-等待出板+進板-LA
	virtual bool               CheckPCBOutInFinish_LA();//PLC-出板+進板完成-LA
	virtual bool               CheckPCBOutInFault_LA();//PLC-出板+進板失敗-LA

	virtual bool               ExecPCBReIn_LA(bool bWait);//PLC-重新進板-LA
	virtual bool               CheckPCBReInFinish_LA();//PLC-重新進板完成-LA
	virtual bool               WaitForPCBReInFinish_LA();//PLC-等待重新進板完成-LA

	virtual bool               ExecPCBClear_LA(bool bWait);//PLC-清板板-LA
	virtual bool               CheckPCBClearFinish_LA();//PLC-確認清板完成-LA
	virtual bool               WaitForPCBClearFinish_LA();//PLC-等待清板完成-LA

	virtual bool               StopPCBAutoOutIn_LA(bool bWait);//PLC-停止自動進出板-LA
	virtual bool               ExecPCBAutoOutIn_LA(bool On, bool bWait);//PLC-自動出進板-LA
	virtual bool               WaitForPCBAutoOutInFinish_LA();//PLC-等待自動出進板-LA
	virtual bool               ReadPCBAutoOutInFinish_LA();//PLC-自動出進板完成-LA
	virtual bool               CheckPCBAutoOutInFinish_LA();//PLC-自動出進板完成-LA
	virtual bool               CheckPCBAutoOutInFault_LA();//PLC-自動出進板失敗-LA
	virtual bool               WritePCBAutoOutInFinish_LA(bool On);//PLC-寫入PCB自動出進完成-LA

	virtual bool               StopPCBAutoBackIn_LA(bool bWait);//PLC-停止自動退進板-LA
	virtual bool               ExecPCBAutoBackIn_LA(bool On, bool bWait);//PLC-自動退進板-LA
	virtual bool               WaitForPCBAutoBackInFinish_LA();//PLC-等待自動退進板-LA
	virtual bool               ReadPCBAutoBackInFinish_LA();//PLC-自動退進板完成-LA
	virtual bool               CheckPCBAutoBackInFinish_LA();//PLC-自動退進板完成-LA
	virtual bool               CheckPCBAutoBackInFault_LA();//PLC-自動退進板失敗-LA
	virtual bool               WritePCBAutoBackInFinish_LA(bool On);//PLC-寫入PCB自動退進完成-LA

	virtual bool               CheckConveryerStopRunning_LA();//PLC-確認軌道停止運轉-LA
	virtual bool               WaitForConveryerStopRunning_LA();//PLC-等待軌道停止運轉-LA
	virtual bool               ExecConveyorMotorRunning_LA(bool On, bool bPositive, bool bSlow);//PLC-執行軌道運轉-LA

	virtual bool               WriteSignalToLast_LA(bool On);//PLC-送訊號給上一站-LA
	virtual bool               WriteSignalToNext_LA(bool On);//PLC-送訊號給下一站-LA
	virtual bool               WriteOKSignalToNext_LA(bool On);//PLC-送OK訊號給下一站-LA
	virtual bool               WriteNGSignalToNext_LA(bool On);//PLC-送NG訊號給下一站-LA
	virtual bool               WriteLockSignal_LA(bool On);//PLC-送鎖住訊號-LA
	virtual bool               WriteConveyerSensorPower_LA(bool On, bool bForce);//PLC-關閉軌道感應器電源-LA
	virtual bool               TurnOffConveyerSensorPower_LA(bool On);//PLC-關閉軌道感應器電源-LA

	//Lane B
	virtual bool               WriteOnInspection_LB(bool On);//設定檢測中-LB		
	virtual bool               TurnOnConveryerLED_LB(bool On);//開啟軌道LED-LB

	virtual bool               TurnOnStageAlarm_LB();//機台異常-LB
	virtual bool               TurnOffStageAlarm_LB();//機台異常-LB

	virtual bool               TurnOnInspectAlarm_LB();//檢測異常-LB
	virtual bool               TurnOffInspectAlarm_LB();//檢測異常-LB	
	
	virtual bool               WriteConveryerClamp_LB(bool On);//PLC-是否夾板-LB
	virtual bool               WriteConveryerStopBar_LB(bool On);//PLC-停板器-LB	
	virtual int                ReadLanePLCErrorCode_LB();//讀取PLC的軌道錯誤代碼-LB

	virtual bool               ExecPCBIn_LB(bool bWait);//PLC-進板-LB
	virtual bool               WaitForPCBInFinish_LB();//PLC-等進板完成-LB
	virtual bool               CheckPCBInFinish_LB();//PLC-進板完成-LB
	virtual bool               CheckPCBInFault_LB();//PLC-進板失敗-LB	

	virtual bool               ExecPCBIn2nd_LB(bool bWait);//PLC-第2段進板-LB
	virtual bool               WaitForPCBIn2ndFinish_LB();//PLC-第2段等進板完成-LB
	virtual bool               CheckPCBIn2ndFinish_LB();//PLC-第2段進板完成-LB
	virtual bool               CheckPCBIn2ndFault_LB();//PLC-第2段進板失敗-LB

	virtual bool               ExecPCBIn3rd_LB(bool bWait);//PLC-第3段進板-LB
	virtual bool               WaitForPCBIn3rdFinish_LB();//PLC-第3段等進板完成-LB
	virtual bool               CheckPCBIn3rdFinish_LB();//PLC-第3段進板完成-LB
	virtual bool               CheckPCBIn3rdFault_LB();//PLC-第3段進板失敗-LB

	virtual bool               ExecPCBBack_LB(bool bWait);////PLC-退板-LB
	virtual bool               WaitForPCBBackFinish_LB();//PLC-等退板完成-LB
	virtual bool               CheckPCBBackFinish_LB();//PLC-退板完成-LB
	virtual bool               CheckPCBBackFault_LB();//PLC-退板失敗-LB

	virtual bool               ExecPCBBackOut_LB(bool bWait);//PLC-退出板-LB
	virtual bool               WaitForPCBBackOutFinish_LB();//PLC-等退出板完成-LB
	virtual bool               CheckPCBBackOutFinish_LB();//PLC-退出板完成-LB
	virtual bool               CheckPCBBackOutFault_LB();//PLC-退出板失敗-LB	

	virtual bool               ExecPCBOut_LB(bool bWait);//PLC-出板-LB
	virtual bool               WaitForPCBOutFinish_LB();//PLC-等出板完成-LB
	virtual bool               CheckPCBOutFinish_LB();//PLC-出板完成-LB
	virtual bool               CheckPCBOutFault_LB();//PLC-出板失敗-LB

	virtual bool               ExecPCBOutInside_LB(bool bWait);//PLC-出板至機台側邊-LB
	virtual bool               WaitForPCBOutInsideFinish_LB();//PLC-等待出板至機台側邊-LB
	virtual bool               CheckPCBOutInsideFinish_LB();//PLC-出板至機台側邊完成-LB
	virtual bool               CheckPCBOutInsideFault_LB();//PLC-出板至機台側邊失敗-LB

	virtual bool               ExecPCBOutIn_LB(bool bWait);//PLC-出板+進板-LB
	virtual bool               WaitForPCBOutInFinish_LB();//PLC-等待出板+進板-LB
	virtual bool               CheckPCBOutInFinish_LB();//PLC-出板+進板完成-LB
	virtual bool               CheckPCBOutInFault_LB();//PLC-出板+進板失敗-LB

	virtual bool               ExecPCBReIn_LB(bool bWait);//PLC-重新進板-LB
	virtual bool               CheckPCBReInFinish_LB();//PLC-重新進板完成-LB
	virtual bool               WaitForPCBReInFinish_LB();//PLC-等待重新進板完成-LB

	virtual bool               ExecPCBClear_LB(bool bWait);//PLC-清板板-LB
	virtual bool               CheckPCBClearFinish_LB();//PLC-確認清板完成-LB
	virtual bool               WaitForPCBClearFinish_LB();//PLC-等待清板完成-LB

	virtual bool               StopPCBAutoOutIn_LB(bool bWait);//PLC-停止自動進出板-LB
	virtual bool               ExecPCBAutoOutIn_LB(bool On, bool bWait);//PLC-自動出進板-LB
	virtual bool               WaitForPCBAutoOutInFinish_LB();//PLC-等待自動出進板-LB
	virtual bool               ReadPCBAutoOutInFinish_LB();//PLC-自動出進板完成-LB
	virtual bool               CheckPCBAutoOutInFinish_LB();//PLC-自動出進板完成-LB
	virtual bool               CheckPCBAutoOutInFault_LB();//PLC-自動出進板失敗-LB
	virtual bool               WritePCBAutoOutInFinish_LB(bool On);//PLC-寫入PCB自動出進完成-LB

	virtual bool               StopPCBAutoBackIn_LB(bool bWait);//PLC-停止自動退進板-LB
	virtual bool               ExecPCBAutoBackIn_LB(bool On, bool bWait);//PLC-自動退進板-LB
	virtual bool               WaitForPCBAutoBackInFinish_LB();//PLC-等待自動退進板-LB
	virtual bool               ReadPCBAutoBackInFinish_LB();//PLC-自動退進板完成-LB
	virtual bool               CheckPCBAutoBackInFinish_LB();//PLC-自動退進板完成-LB
	virtual bool               CheckPCBAutoBackInFault_LB();//PLC-自動退進板失敗-LB
	virtual bool               WritePCBAutoBackInFinish_LB(bool On);//PLC-寫入PCB自動退進完成-LB

	virtual bool               CheckConveryerStopRunning_LB();//PLC-確認軌道停止運轉-LB
	virtual bool               WaitForConveryerStopRunning_LB();//PLC-等待軌道停止運轉-LB
	virtual bool               ExecConveyorMotorRunning_LB(bool On, bool bPositive, bool bSlow);//PLC-執行軌道運轉-LB

	virtual bool               WriteSignalToLast_LB(bool On);//PLC-送訊號給上一站-LB
	virtual bool               WriteSignalToNext_LB(bool On);//PLC-送訊號給下一站-LB
	virtual bool               WriteOKSignalToNext_LB(bool On);//PLC-送OK訊號給下一站-LB
	virtual bool               WriteNGSignalToNext_LB(bool On);//PLC-送NG訊號給下一站-LB
	virtual bool               WriteLockSignal_LB(bool On);//PLC-送鎖住訊號-LB
	virtual bool               WriteConveyerSensorPower_LB(bool On, bool bForce);//PLC-關閉軌道感應器電源-LB
	virtual bool               TurnOffConveyerSensorPower_LB(bool On);//PLC-關閉軌道感應器電源-LB	
	//---------------------------------------------------------------------------------//	
	virtual bool               PushDownStartBtn();//按下啟動燈
	virtual bool               PushDownResetBtn();//按下復歸燈	
	virtual bool               PushDownStopBtn();	//按下停止燈
	//---------------------------------------------------------------------------------//	
	virtual bool               WriteLaneAdjustManual(bool On);//間距馬達-手動	
	virtual bool               WriteLaneAdjustFixed14Lane(bool On);//間距馬達-固定軌道模式	
	virtual bool               WriteLaneAdjustHomeTimeout(int time_ms);//間距馬達-歸零逾時
	virtual bool               WriteLaneAdjustMoveTimeout(int time_ms);//間距馬達-移動逾時

	virtual bool               InitialLaneAdjust_LA();//間距馬達-初始化
	virtual bool               WriteLaneAdjustCmdPos_LA(double Pos);//間距馬達-設定命令位置	
	virtual bool               WriteLaneAdjustToHome_LA(bool On);//間距馬達-歸零搜尋
	virtual bool               WriteLaneAdjustToMove_LA(bool On);//間距馬達-開始移動
	virtual bool               WriteLaneAdjustJogMove_LA(bool Dir, bool Stop);//間距馬達-搖桿移動
	virtual bool               WriteLaneAdjustLimitMaxPos_LA(double Pos);//間距馬達-設定最大位置
	virtual bool               WriteLaneAdjustJogSpeed_LA(double Speed);//間距馬達-設定Jog速度
	virtual bool               WriteLaneAdjustSkewPitch_LA(double Pitch);//間距馬達-設定移動距離		
	virtual bool               ResetLaneAdjustException_LA();//間距馬達-復歸異常	

	virtual bool               ReadLaneAdjustDisable_LA();//間距馬達-是否關閉	
	virtual bool               ReadLaneAdjustSensorORG_LA();//間距馬達-確認是否在原點
	virtual bool               ReadLaneAdjustSensorLimit_LA();//間距馬達-確認是否在極限	
	virtual bool               ReadLaneAdjustExecption_LA();//間距馬達-確認是否異常
	virtual bool               ReadLaneAdjustHomeDone_LA();//間距馬達-確認是否歸零完畢
	virtual bool               ReadLaneAdjustMoveDone_LA();//間距馬達-確認是否移動完畢
	virtual double             ReadLaneAdjustCurrentPos_LA();//間距馬達-取回目前位置	
	virtual int                ReadLaneAdjustPLCErrorCode_LA();//間距馬達-取回PLC的錯誤代碼
	virtual int                ReadLaneAdjustMotorErrorCode_LA();//間距馬達-取回馬達的錯誤代碼

	virtual bool               InitialLaneAdjust_LB();//間距馬達-初始化	
	virtual bool               WriteLaneAdjustCmdPos_LB(double Pos);//間距馬達-設定命令位置	
	virtual bool               WriteLaneAdjustToHome_LB(bool On);//間距馬達-歸零搜尋
	virtual bool               WriteLaneAdjustToMove_LB(bool On);//間距馬達-開始移動
	virtual bool               WriteLaneAdjustJogMove_LB(bool Dir, bool Stop);//間距馬達-搖桿移動
	virtual bool               WriteLaneAdjustLimitMaxPos_LB(double Pos);//間距馬達-設定最大位置
	virtual bool               WriteLaneAdjustJogSpeed_LB(double Speed);//間距馬達-設定Jog速度
	virtual bool               WriteLaneAdjustSkewPitch_LB(double Pitch);//間距馬達-設定移動距離		
	virtual bool               ResetLaneAdjustException_LB();//間距馬達-復歸異常	

	virtual bool               ReadLaneAdjustDisable_LB();//間距馬達-是否關閉	
	virtual bool               ReadLaneAdjustSensorORG_LB();//間距馬達-確認是否在原點
	virtual bool               ReadLaneAdjustSensorLimit_LB();//間距馬達-確認是否在極限	
	virtual bool               ReadLaneAdjustExecption_LB();//間距馬達-確認是否異常
	virtual bool               ReadLaneAdjustHomeDone_LB();//間距馬達-確認是否歸零完畢
	virtual bool               ReadLaneAdjustMoveDone_LB();//間距馬達-確認是否移動完畢
	virtual double             ReadLaneAdjustCurrentPos_LB();//間距馬達-取回目前位置
	virtual int                ReadLaneAdjustPLCErrorCode_LB();//間距馬達-取回PLC的錯誤代碼
	virtual int                ReadLaneAdjustMotorErrorCode_LB();//間距馬達-取回馬達的錯誤代碼	
	//---------------------------------------------------------------------------------//
};
//-------------------------------------------------------------------------------------//
extern CPLC_Vigor  PLC_Vigor;
//-------------------------------------------------------------------------------------//
#endif//PLC_OBJ_MODE
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_PLCOBJ_VIGOR_H__80C038EF_3902_4E62_8267_1B3C0124F841__INCLUDED_)
