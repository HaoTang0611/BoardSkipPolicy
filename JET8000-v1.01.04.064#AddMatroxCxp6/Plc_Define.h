#ifndef _PLC_DEFINE_H_
#define _PLC_DEFINE_H_
//-------------------------------------------------------------------------------------//
#define   PLC_OBJ_MODE               PLC_OBJ_VIGOR
//-------------------------------------------------------------------------------------//
#define   PLC_TOWER_LIGHT_NORMAL                 0
#define   PLC_TOWER_LIGHT_USER_DEFINE            2
//---------------------------------------------------------------------------------//	
#define   PLC_TOWER_LIGHT_TURN_ON_RED            1//塔燈-亮紅燈
#define   PLC_TOWER_LIGHT_TURN_ON_YELLOW         2//塔燈-亮黃燈
#define   PLC_TOWER_LIGHT_TURN_ON_GREEN          3//塔燈-亮綠燈
#define   PLC_TOWER_LIGHT_FLASH_RED              4//塔燈-閃紅燈
#define   PLC_TOWER_LIGHT_FLASH_YELLOW           5//塔燈-閃黃燈
#define   PLC_TOWER_LIGHT_FLASH_GREEN            6//塔燈-閃綠燈
#define   PLC_TOWER_LIGHT_TURN_ON_YELLOW_GREEN   7//塔燈-亮黃綠燈
//---------------------------------------------------------------------------------//	
#define   JET_PLC_BUFFER_SIZE          1024
//---------------------------------------------------------------------------------//	
#define   JET_PLC_LANE_ADJUST_LIMIT_MIN         50//自動調整軌道規格最短距離
//---------------------------------------------------------------------------------//	
#define   PLC_PCB_AUTO_RUN_STOP                   0//PLC記錄的自動運轉模式
#define   PLC_PCB_AUTO_RUN_OUT_IN                 1//PLC記錄的自動運轉模式
#define   PLC_PCB_AUTO_RUN_BACK_IN                2//PLC記錄的自動運轉模式
//---------------------------------------------------------------------------------//
//0.初始  1.進板等待中  2.進板中  3.出板等待中  4.出板中  5.出板完成
#define   PLC_CONVERYER_STATUS_STOP                    0  
#define   PLC_CONVERYER_STATUS_PCB_IN_WAITING          1
#define   PLC_CONVERYER_STATUS_PCB_IN_RUNNING          2
//#define   PLC_CONVERYER_STATUS_PCB_IN_FINISH         3//PLC_CONVERYER_STATUS_STOP
#define   PLC_CONVERYER_STATUS_PCB_OUT_WAITING         3
#define   PLC_CONVERYER_STATUS_PCB_OUT_RUNNING         4
#define   PLC_CONVERYER_STATUS_PCB_OUT_FINISH          5
//#define   PLC_CONVERYER_STATUS_PCB_BACK_WAITING        3
#define   PLC_CONVERYER_STATUS_PCB_BACK_RUNNING        6
//#define   PLC_CONVERYER_STATUS_PCB_BACK_FINISH         5
//---------------------------------------------------------------------------------//
enum PLC_PARAM_ID
{
	//Kernel參數
	PLC_PARAM_COMM_DELAY_TIME,//PLC通訊延遲時間	
	PLC_PARAM_CHECK_PCB_INSIDE_BEFORE_PCB_IN,//進板前確認機台有無板子

	//PLC一般參數
	PLC_PARAM_PCB_IN_OUT_TIMEOUT_LA,//PCB進出板逾時-10s-A軌 
	PLC_PARAM_PCB_IN_OUT_TIMEOUT_LB,//PCB進出板逾時-10s-B軌
	PLC_PARAM_AIR_LOST_CHECK_TIME,//氣壓不足監控時間-0.5s
	PLC_PARAM_PCB_STOP_DELAY_TIME,//定位Sensor感應後持續運轉延遲時間-0.1sec
	PLC_PARAM_CLAMP_ON_OFF_DELAY_TIME,//維修模式時-汽缸夾鬆板愈時-1s
	PLC_PARAM_PCB_OUT_WITH_IN_DELAY_TIME_LA,//PCB出板帶進板延遲時間-0.1s-A軌
	PLC_PARAM_PCB_OUT_WITH_IN_DELAY_TIME_LB,//PCB出板帶進板延遲時間-0.1s-A軌
	PLC_PARAM_PCB_OUT_DELAY_TIME_LA,//PCB出板後延遲時間-0.1s-A軌
	PLC_PARAM_PCB_OUT_DELAY_TIME_LB,//PCB出板後延遲時間-0.1s-A軌
	PLC_PARAM_PCB_CLEAR_TIMEOUT_LA,//PCB板清除時間-時間單位:1=100ms
	PLC_PARAM_PCB_CLEAR_TIMEOUT_LB,//PCB板清除時間-時間單位:1=100ms
	PLC_PARAM_TURN_ON_LAST_SIGNAL_DELAY_TIME,//機台向上一站要板持續時間:1=100ms
	PLC_PARAM_CHECK_PCB_DOUBLE_IN_PITCH_TIME,//入料檢知頻寬設定(2個板間隔時間):1=100ms
	PLC_PARAM_CHECK_PCB_DOUBLE_IN_BOARD_TIME,//入料檢知時間設定(單板最長時間):1=100ms
	PLC_PARAM_PCB_IN_AUTO_OFF_STOP_BAR_LA,//PCB進出自動縮停止塊-A軌
	PLC_PARAM_PCB_IN_AUTO_OFF_STOP_BAR_LB,//PCB進出自動縮停止塊-B軌
	PLC_PARAM_PCB_AUTO_RUN_WITH_SIDE_STOP_LA,//PCB自動進出板帶停板邊-A軌
	PLC_PARAM_PCB_AUTO_RUN_WITH_SIDE_STOP_LB,//PCB自動進出板帶停板邊-B軌	
	PLC_PARAM_TURN_OFF_LANE_SENSOR_POWER_LA,//關閉軌道感測器電源-A軌
	PLC_PARAM_TURN_OFF_LANE_SENSOR_POWER_LB,//關閉軌道感測器電源-B軌
	PLC_PARAM_PCB_OUT_WITH_CLEAR_OK_NG_SIGNAL,//PCB出板時清除OK, NG訊號	
	PLC_PARAM_ENABLE_CHECK_PCB_DOUBLE_IN,//啟用確認PCB重複進板
	PLC_PARAM_ENABLE_CHECK_PCB_BARGE_IN,//啟用確認檢測時PCB板闖入	
	PLC_PARAM_ENABLE_FAN_ALARM,         //啟用風扇警報
	PLC_PARAM_ENABLE_OPEN_DOOR_STOP_POWER,   //啟用開門斷電
	PLC_PARAM_ENABLE_STOPPER_SENSOR_LA,   //啟用擋板塊感應器-A軌
	PLC_PARAM_ENABLE_STOPPER_SENSOR_LB,   //啟用擋板塊感應器-B軌

	//軌道間距參數
	PLC_PARAM_LANE_ADJUST_HOME_TIMEOUT,//自動調整軌道歸零逾時
	PLC_PARAM_LANE_ADJUST_MOVE_TIMEOUT,//自動調整軌道移動逾時

	PLC_PARAM_LANE_ADJUST_SKEW_PITCH,//自動調整軌道螺紋間距

	PLC_PARAM_LANE_ADJUST_CURRENT_POS_LA,//自動調整軌道A目前位置
	PLC_PARAM_LANE_ADJUST_CURRENT_POS_LB,//自動調整軌道A目前位置

	PLC_PARAM_LANE_ADJUST_MOVE_TO_POS_LA,//自動調整軌道A移動位置
	PLC_PARAM_LANE_ADJUST_MOVE_TO_POS_LB,//自動調整軌道B移動位置

	PLC_PARAM_LANE_ADJUST_LIMIT_MAX_LA,//自動調整軌道A最大範圍
	PLC_PARAM_LANE_ADJUST_LIMIT_MAX_LB,//自動調整軌道B最大範圍

	PLC_PARAM_LANE_ADJUST_LIMIT_MIN_LA,//自動調整軌道A最大範圍
	PLC_PARAM_LANE_ADJUST_LIMIT_MIN_LB,//自動調整軌道B最大範圍

	PLC_PARAM_LANE_ADJUST_MOVE_SPEED_LA,//自動調整軌道A速度
	PLC_PARAM_LANE_ADJUST_JOG_SPEED_LA,//自動調整軌道A速度

	PLC_PARAM_LANE_ADJUST_MOVE_SPEED_LB,//自動調整軌道B速度
	PLC_PARAM_LANE_ADJUST_JOG_SPEED_LB,//自動調整軌道B速度

	PLC_PARAM_LANE_ADJUST_HOME_POS_LA,//自動調整軌道A速度
	PLC_PARAM_LANE_ADJUST_HOME_POS_LB,//自動調整軌道B速度

	PLC_PARAM_RETURN
};

typedef struct tagPLCParameter
{
	int                m_PLCCommDelayTime;//PLC通訊延遲時間
	int                m_CheckPCBInsideBeforePCBIn;//進板前確認機台有無板子

	//PLC Device
	int                m_PCBInOutTimeout_LA;//進出板逾時-10s
	int                m_PCBInOutTimeout_LB;//進出板逾時-10s
	int                m_AirLostCheckTime;  //氣壓不足逾時-0.5s
	int                m_PCBStopDelayTime;  //碰觸定位Sensor後皮帶持續運轉多久時間	
	int                m_ClampOnOffDelayTime; //夾鬆板延遲時間-0.5s	
	int                m_PCBOutWithInDelayTime_LA;//出板帶進板延遲時間-1s
	int                m_PCBOutWithInDelayTime_LB;//出板帶進板延遲時間-1s
	int                m_PCBOutDelayTime_LA;//出板後板延遲時間-1s
	int                m_PCBOutDelayTime_LB;//出板後板延遲時間-1s
	int                m_PCBClearTimeout_LA;//PCB板清除時間-時間單位:1=100ms
	int                m_PCBClearTimeout_LB;//PCB板清除時間-時間單位:1=100ms
	int                m_TurnOnLastSignalDelayTime;//機台向上一站要板持續時間:1=100ms
	int                m_CheckPCBDoubleInPitchTime;//入料檢知頻寬設定(2個板間隔時間):1=100ms
	int                m_CheckPCBDoubleInBoardTime;//入料檢知時間設定(單板最長時間):1=100ms
	int                m_PCBInAutoOffStopBar_LA;//PCB進出自動縮停止塊-A軌
	int                m_PCBInAutoOffStopBar_LB;//PCB進出自動縮停止塊-B軌	
	int                m_PCBAutoRunWithSideStop_LA;//PCB自動進出板帶停板邊-A軌	
	int                m_PCBAutoRunWithSideStop_LB;//PCB自動進出板帶停板邊-B軌	
	int                m_TurnOffConveyerSensorPower_LA;//關閉軌道感測器電源-A軌	
	int                m_TurnOffConveyerSensorPower_LB;//關閉軌道感測器電源-B軌		
	int                m_PCBOutWithClearOKNGSignal;//PCB出板時清除OK, NG訊號	
	int                m_EnableCheckPCBDoubleIn;//啟用確認PCB重複進板
	int                m_EnableCheckPCBBargeIn;//啟用確認檢測時PCB板闖入
	int                m_EnabledFanAlarm;//啟用風扇警報
	int                m_EnabledOpenDoorStopPower;//啟用開門斷電
	int                m_EnabledStopperSensor_LA;//啟用擋板塊感應器-A軌
	int                m_EnabledStopperSensor_LB;//啟用擋板塊感應器-B軌

	tagPLCParameter()
	{
		m_PLCCommDelayTime = 0;//PLC通訊延遲時間
		m_CheckPCBInsideBeforePCBIn = FN_DISABLE;//進板前確認機台有無板子
		m_PCBInOutTimeout_LA = 100;//進出板愈時-10s
		m_PCBInOutTimeout_LB = 100;//進出板愈時-10s	
		m_AirLostCheckTime = 5;   //氣壓不足逾時-0.5s
		m_PCBStopDelayTime = 10;   //碰觸定位Sensor後皮帶持續運轉多久時間	
		m_ClampOnOffDelayTime = 5;//維修模式時-汽缸送氣逾時-0.5s		
		m_PCBOutWithInDelayTime_LA = 100;//出板帶進板延遲時間-1s	
		m_PCBOutWithInDelayTime_LB = 100;//出板帶進板延遲時間-1s	
		m_PCBOutDelayTime_LA =  0;//出板後板延遲時間-1s
		m_PCBOutDelayTime_LB =  0;//出板後板延遲時間-1s
		m_PCBClearTimeout_LA = 30;//PCB板清除時間-時間單位:1=100ms
		m_PCBClearTimeout_LB = 30;//PCB板清除時間-時間單位:1=100ms
		m_TurnOnLastSignalDelayTime = 10;//機台向上一站要板持續時間:1=100ms
		m_CheckPCBDoubleInPitchTime = 2;//入料檢知頻寬設定(2個板間隔時間):1=100ms
		m_CheckPCBDoubleInBoardTime = 40;//入料檢知時間設定(單板最長時間):1=100ms
		m_PCBInAutoOffStopBar_LA = FN_ENABLE;//PCB進出自動縮停止塊-A軌
		m_PCBInAutoOffStopBar_LB = FN_ENABLE;//PCB進出自動縮停止塊-B軌
		m_PCBAutoRunWithSideStop_LA = FN_DISABLE;//PCB自動進出板帶停板邊-A軌	
		m_PCBAutoRunWithSideStop_LB = FN_DISABLE;//PCB自動進出板帶停板邊-B軌	
		m_TurnOffConveyerSensorPower_LA = FN_DISABLE;//關閉軌道感測器電源-A軌	
		m_TurnOffConveyerSensorPower_LB = FN_DISABLE;//關閉軌道感測器電源-B軌	
		m_PCBOutWithClearOKNGSignal = FN_DISABLE;//PCB出板時清除OK, NG訊號		
		m_EnableCheckPCBDoubleIn = FN_DISABLE;//啟用確認PCB重複進板		
		m_EnableCheckPCBBargeIn = FN_DISABLE;//啟用確認檢測時PCB板闖入
		m_EnabledFanAlarm = FN_DISABLE;
		m_EnabledOpenDoorStopPower = FN_DISABLE;
		m_EnabledStopperSensor_LA = FN_DISABLE;
		m_EnabledStopperSensor_LB = FN_DISABLE;
	}

} TPLCParameter, *PPLCParameter;
/*
//---------------------------------------------------------------------------------//	
//雙軌的愈時時間設定
//---------------------------------------------------------------------------------//	
#define   JET_PLC_LANE_ADJUST_HOME_TIMEOUT    7020//自動調整軌道歸零逾時
#define   JET_PLC_LANE_ADJUST_MOVE_TIMEOUT    7021//自動調整軌道移動逾時

#define   JET_PLC_LANE_ADJUST_SKEW_PITCH_LA   7308//自動調整軌道螺紋間距
#define   JET_PLC_LANE_ADJUST_SKEW_PITCH_LB   7328//自動調整軌道螺紋間距

#define   JET_PLC_LANE_ADJUST_CURRENT_POS_LA  7300//自動調整軌道A目前位置
#define   JET_PLC_LANE_ADJUST_CURRENT_POS_LB  7320//自動調整軌道A目前位置

#define   JET_PLC_LANE_ADJUST_MOVE_TO_POS_LA  3112//自動調整軌道A移動位置
#define   JET_PLC_LANE_ADJUST_MOVE_TO_POS_LB  3212//自動調整軌道B移動位置

#define   JET_PLC_LANE_ADJUST_LIMIT_MAX_LA    7310//自動調整軌道A最大範圍
#define   JET_PLC_LANE_ADJUST_LIMIT_MAX_LB    7330//自動調整軌道B最大範圍

#define   JET_PLC_LANE_ADJUST_LIMIT_MIN_LA    7312//自動調整軌道A最大範圍
#define   JET_PLC_LANE_ADJUST_LIMIT_MIN_LB    7332//自動調整軌道B最大範圍

#define   JET_PLC_LANE_ADJUST_MOVE_SPEED_LA   7304//自動調整軌道A速度
#define   JET_PLC_LANE_ADJUST_JOG_SPEED_LA    7314//自動調整軌道A速度

#define   JET_PLC_LANE_ADJUST_MOVE_SPEED_LB   7324//自動調整軌道B速度
#define   JET_PLC_LANE_ADJUST_JOG_SPEED_LB    7334//自動調整軌道B速度

#define   JET_PLC_LANE_ADJUST_HOME_POS_LA     7306//自動調整軌道A速度
#define   JET_PLC_LANE_ADJUST_HOME_POS_LB     7326//自動調整軌道B速度
*/
//---------------------------------------------------------------------------------//
typedef struct _PlcNode
{	
	CString            m_Name;//名稱
	char               m_Address[32];//位址
	int                m_Value;//狀態
	int                m_FnCode;//功能編號(NULL則為客戶額外增加)
	bool               m_Seleted;//可選取到
	bool               m_Deleted;//刪除
	bool               m_ToRead;//去讀取

	_PlcNode()
	{
		m_Name = _T("New");//名稱
		m_Address[0] = '\0';//位址
		m_Value = 0;//狀態
		m_FnCode = 0;//功能編號(NULL則為客戶額外增加)
		m_Seleted = false;//選取到
		m_Deleted = false;//刪除
		m_ToRead = false;//去讀取
	}

	void SetAddress(LPCTSTR Address)
	{
	#ifdef _UNICODE
		::wcstombs(m_Address, Address, sizeof(m_Address));
	#else
		::strcpy(m_Address, Address);
	#endif
	}
} TPlcNode, *PPlcNode;
//-------------------------------------------------------------------------------------//

#endif//_PLC_DEFINE_H_