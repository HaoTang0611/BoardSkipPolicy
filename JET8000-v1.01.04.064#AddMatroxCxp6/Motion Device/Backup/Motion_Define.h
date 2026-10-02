#ifndef _MotionObjDef_H_
#define _MotionObjDef_H_
//-------------------------------------------------------------------------------------//
#define MOTION_PCI_M114GL             1
#define MOTION_PCE_M114GL             2
#define MOTION_LIB_MODULE             3

//#define MOTION_DERIVE_MODE               MOTION_PCI_M114GL
#define MOTION_DERIVE_MODE               MOTION_PCE_M114GL
//#define MOTION_DERIVE_MODE               MOTION_LIB_MODULE
//-------------------------------------------------------------------------------------//
#if MOTION_DERIVE_MODE == MOTION_PCI_M114GL 
	#define AXIS_X          0
	#define AXIS_Y          1
	#define AXIS_Z          2
	#define AXIS_X_SLAVE    3
#elif MOTION_DERIVE_MODE == MOTION_PCE_M114GL 
	#define AXIS_X          0
	#define AXIS_Y          1
	#define AXIS_Z          2
	#define AXIS_X_SLAVE    3
#elif MOTION_DERIVE_MODE == MOTION_LIB_MODULE
	#define AXIS_X          0
	#define AXIS_Y          1
	#define AXIS_Z          2
	#define AXIS_X_SLAVE    3
#else	
	#define AXIS_X          0
	#define AXIS_Y          1
	#define AXIS_Z          2	
#endif//MOTION_DERIVE_MODE
//-------------------------------------------------------------------------------------//
#ifdef DISABLE_Z_AXIS
	#define DISABLE_Z_AXIS  //關閉Z軸
#endif//DISABLE_Z_AXIS
//-------------------------------------------------------------------------------------//
#define TRIGGER_AXIS                        AXIS_X//X軸為主要的Trigger軸
#define TRIGGER_SUBAXIS                     AXIS_Y//Y軸為附屬移動軸
//-------------------------------------------------------------------------------------//
#define TRIGGER_CCD_BETWEEN_LED				1	//LED Trigger 與 CCD Trigger的間距時間 (ms) //chia031
//-------------------------------------------------------------------------------------//
const int MOTION_MIN_HOME_OFFSET = 5000;
//-------------------------------------------------------------------------------------//
enum MOTION_MOVING_MODE
{
	MOTION_MOVING_NORMAL  = 1,
	MOTION_MOVING_HOME    = 2,
	MOTION_MOVING_SCAN    = 3,
	MOTION_MOVING_GO_STOP = 4,
	MOTION_MOVING_PCB_IN  = 5,
	MOTION_MOVING_PCB_OUT = 6,
	MOTION_MOVING_RETURN
};
//-------------------------------------------------------------------------------------//
enum MOTION_TRIGGER_MODE
{
	MOTION_TRIGGER_NULL     = 0,
	MOTION_TRIGGER_FORWARD  = 1,
	MOTION_TRIGGER_BACKWARD = 2,
	MOTION_TRIGGER_RETURN
};
//-------------------------------------------------------------------------------------//
enum MOVE_CURVE_MODE
{
	MOVE_CURVE_T       = 1, //梯形曲線
	MOVE_CURVE_S       = 2,  //S型曲線
	MOVE_CURVE_RETURN
};
//-------------------------------------------------------------------------------------//
enum MOVE_COORDINATE_MODE
{
	MOVE_COORDINATE_ABS      = 1,//絕對移動
	MOVE_COORDINATE_INS      = 2, //相對移動
	MOVE_COORDINATE_RETURN
};
//---------------------------------------------------------------------------------------//
enum ACC_TIME_ADJUST_MODE//加減速度時間調整模式
{
	ACC_TIME_ADJUST_OFF         = 0,//關閉
	ACC_TIME_ADJUST_FIX_T       = 1,//固定時間(一律以使用者設定時間為主)
	ACC_TIME_ADJUST_MIN_T       = 2,//最小時間(低於最小時間就以最小時間為主)
	ACC_TIME_ADJUST_GAMMA       = 3,//Gamma修正(低於最小時間就以Gamma時間修正)
	ACC_TIME_ADJUST_RETURN
};
//---------------------------------------------------------------------------------------//
enum MOTION_PARAM_ID//MOTION_PARAM_
{
	MOTION_PARAM_BEGIN,	
	MOTION_PARAM_SAVE_MOTION_CARD_PARAM,//是否儲存運動卡參數
	MOTION_PARAM_SAVE_CURRENT_PROCESS,//是否儲存運動訊息	
	MOTION_PARAM_ACCELERATION_ADJUST, //加速度調整
	MOTION_PARAM_XY_CALI_ENABLE,      //是否XY座標補正	
	MOTION_PARAM_XY_CALI_EXTEND_RANGE,//XY座標補正外擴範圍	
	MOTION_PARAM_S_CURVE_VELOCITY_RATIO,//S-Curve速度比例 	
	MOTION_PARAM_SIGN_POSITIVE_CONVERT,//內部方向性轉換
	//---------------------------------------------------------------------------------//
	MOTION_PARAM_SIGN_POSITIVE_X,//X軸與Cad的正負方向
	MOTION_PARAM_SIGN_POSITIVE_Y,//Y軸與Cad的正負方向
	MOTION_PARAM_SIGN_POSITIVE_Z,//Z軸與Cad的正負方向
	//---------------------------------------------------------------------------------//	
	MOTION_PARAM_HOME_PRE_DIST_X,//X軸歸零前移動距離
	MOTION_PARAM_HOME_PRE_DIST_Y,//Y軸歸零前移動距離
	MOTION_PARAM_HOME_PRE_DIST_Z,//Z軸歸零前移動距離
	//---------------------------------------------------------------------------------//	
	MOTION_PARAM_HOME_ORG_OFFSET_X,//X軸歸零後的偏移值
	MOTION_PARAM_HOME_ORG_OFFSET_Y,//Y軸歸零後的偏移值
	MOTION_PARAM_HOME_ORG_OFFSET_Z,//Z軸歸零後的偏移值
	//---------------------------------------------------------------------------------//	
	MOTION_PARAM_START_VELOCITY,//起始速度  
	
	MOTION_PARAM_HOME_VELOCITY_X,//X軸的歸零速度
	MOTION_PARAM_HOME_VELOCITY_Y,//Y軸的歸零速度
	MOTION_PARAM_HOME_VELOCITY_Z,//Z軸的歸零速度

	MOTION_PARAM_MAX_VELOCITY_X,//X軸的一般移動最大速度, 
	MOTION_PARAM_MAX_VELOCITY_Y,//y軸的一般移動最大速度, 
	MOTION_PARAM_MAX_VELOCITY_Z,//z軸的一般移動最大速度, 

	MOTION_PARAM_BOARD_IN_VELOCITY_X,//X軸的移動至進板速度
	MOTION_PARAM_BOARD_IN_VELOCITY_Y,//y軸的移動至進板速度
	MOTION_PARAM_BOARD_IN_VELOCITY_Z,//z軸的移動至進板速度

	MOTION_PARAM_BOARD_OUT_VELOCITY_X,//X軸的移動至出板速度
	MOTION_PARAM_BOARD_OUT_VELOCITY_Y,//y軸的移動至出板速度
	MOTION_PARAM_BOARD_OUT_VELOCITY_Z,//z軸的移動至出板速度

	MOTION_PARAM_GRABBING_VELOCITY_X,//X軸的取像移動最大速度, 
	MOTION_PARAM_GRABBING_VELOCITY_Y,//y軸的取像移動最大速度, 
	MOTION_PARAM_GRABBING_VELOCITY_Z,//z軸的取像移動最大速度, 
	//---------------------------------------------------------------------------------//	
	MOTION_PARAM_STAGE_START_POS_X,//機台起始的位置X
	MOTION_PARAM_STAGE_START_POS_Y,//機台起始的位置Y
	MOTION_PARAM_STAGE_START_POS_Z,//機台起始的位置Z

	MOTION_PARAM_STAGE_LEAVE_POS_X,//機台離開的位置X
	MOTION_PARAM_STAGE_LEAVE_POS_Y,//機台離開的位置Y
	MOTION_PARAM_STAGE_LEAVE_POS_Z,//機台離開的位置Z

	MOTION_PARAM_BEFORE_PCB_IN_POS_X,//進板前座標-X
	MOTION_PARAM_BEFORE_PCB_IN_POS_Y,//進板前座標-Y
	MOTION_PARAM_BEFORE_PCB_IN_POS_Z,//進板前座標-Z

	MOTION_PARAM_PCB_STOP_POS_X_LA,//A軌道右停板的位置X
	MOTION_PARAM_PCB_STOP_POS_Y_LA,//A軌道右停板的位置Y
	MOTION_PARAM_PCB_STOP_POS_Z_LA,//A軌道右停板的位置Z

	MOTION_PARAM_PCB_STOP_RIGHT_POS_X_LB,//B軌道右停板的位置X
	MOTION_PARAM_PCB_STOP_RIGHT_POS_Y_LB,//B軌道右停板的位置Y
	MOTION_PARAM_PCB_STOP_RIGHT_POS_Z_LB,//B軌道右停板的位置Z

	MOTION_PARAM_PCB_STOP_LEFT_POS_X_LA,//A軌道左停板的位置X
	MOTION_PARAM_PCB_STOP_LEFT_POS_Y_LA,//A軌道左停板的位置Y
	MOTION_PARAM_PCB_STOP_LEFT_POS_Z_LA,//A軌道左停板的位置Z

	MOTION_PARAM_PCB_STOP_LEFT_POS_X_LB,//B軌道左停板的位置X
	MOTION_PARAM_PCB_STOP_LEFT_POS_Y_LB,//B軌道左停板的位置Y
	MOTION_PARAM_PCB_STOP_LEFT_POS_Z_LB,//B軌道左停板的位置Z

	MOTION_PARAM_LANE_LED_RIGHT_STOP_POS_X_LA,//A軌道LED右停板的位置X
	MOTION_PARAM_LANE_LED_RIGHT_STOP_POS_Y_LA,//A軌道LED右停板的位置Y
	MOTION_PARAM_LANE_LED_RIGHT_STOP_POS_Z_LA,//A軌道LED右停板的位置Z

	MOTION_PARAM_LANE_LED_RIGHT_SLOW_POS_X_LA,//A軌道LED右減速的位置X
	MOTION_PARAM_LANE_LED_RIGHT_SLOW_POS_Y_LA,//A軌道LED右減速的位置Y
	MOTION_PARAM_LANE_LED_RIGHT_SLOW_POS_Z_LA,//A軌道LED右減速的位置Z

	MOTION_PARAM_LANE_LED_RIGHT_STOP_POS_X_LB,//B軌道LED右停板的位置X
	MOTION_PARAM_LANE_LED_RIGHT_STOP_POS_Y_LB,//B軌道LED右停板的位置Y
	MOTION_PARAM_LANE_LED_RIGHT_STOP_POS_Z_LB,//B軌道LED右停板的位置Z

	MOTION_PARAM_LANE_LED_RIGHT_SLOW_POS_X_LB,//B軌道LED右減速的位置X
	MOTION_PARAM_LANE_LED_RIGHT_SLOW_POS_Y_LB,//B軌道LED右減速的位置Y
	MOTION_PARAM_LANE_LED_RIGHT_SLOW_POS_Z_LB,//B軌道LED右減速的位置Z

	MOTION_PARAM_LANE_LED_LEFT_STOP_POS_X_LA,//A軌道LED左停板的位置X
	MOTION_PARAM_LANE_LED_LEFT_STOP_POS_Y_LA,//A軌道LED左停板的位置Y
	MOTION_PARAM_LANE_LED_LEFT_STOP_POS_Z_LA,//A軌道LED左停板的位置Z

	MOTION_PARAM_LANE_LED_LEFT_SLOW_POS_X_LA,//A軌道LED左減速的位置X
	MOTION_PARAM_LANE_LED_LEFT_SLOW_POS_Y_LA,//A軌道LED左減速的位置Y
	MOTION_PARAM_LANE_LED_LEFT_SLOW_POS_Z_LA,//A軌道LED左減速的位置Z

	MOTION_PARAM_LANE_LED_LEFT_STOP_POS_X_LB,//B軌道LED左停板的位置X
	MOTION_PARAM_LANE_LED_LEFT_STOP_POS_Y_LB,//B軌道LED左停板的位置Y
	MOTION_PARAM_LANE_LED_LEFT_STOP_POS_Z_LB,//B軌道LED左停板的位置Z

	MOTION_PARAM_LANE_LED_LEFT_SLOW_POS_X_LB,//B軌道LED左減速的位置X
	MOTION_PARAM_LANE_LED_LEFT_SLOW_POS_Y_LB,//B軌道LED左減速的位置Y
	MOTION_PARAM_LANE_LED_LEFT_SLOW_POS_Z_LB,//B軌道LED左減速的位置Z
	//---------------------------------------------------------------------------------//	
	MOTION_PARAM_IN_POSITION_DELAY_TIME,//機台走定位後, 延遲時間	

	//Trigger Parameter	
	MOTION_PARAM_PRE_TRIGGER_OFFSET_DIST,//等速前的偏移位置
	MOTION_PARAM_POST_TRIGGER_OFFSET_DIST,//停止前的偏移位置
	
	MOTION_PARAM_TRIGGER_START_VELOCITY,
	MOTION_PARAM_TRIGGER_MAX_VELOCITY,//取回張數可以一致, 但是影像偶爾有問題
	MOTION_PARAM_TRIGGER_ACCEL_TIME,//Trigger軸加速度時間	
	MOTION_PARAM_TRIGGER_DECEL_TIME,//Trigger軸減速度時間	

	MOTION_PARAM_TRIGGER_FORWARD_OFFSET,//Trigger向前掃時, 和直接拍攝的Trigger差值, 用校正求得
	MOTION_PARAM_TRIGGER_BACKWARD_OFFSET,//Trigger向前掃時, 和直接拍攝的Trigger差值, 用校正求得
	MOTION_PARAM_TRIGGER_SUB_AXIS_OFFSET,//Y軸移動的間距	
	//---------------------------------------------------------------------------------//	
	//JOG
	MOTION_PARAM_JOG_START_VELOCITY,//自由移動時的初始速度
	MOTION_PARAM_JOG_MOVING_VELOCITY,//自由移動時的移動速度 	
	MOTION_PARAM_JOG_MAX_VELOCITY,//自由移動時的最大速度 	
	MOTION_PARAM_JOG_ACCEL_TIME,//自由移動時的加速度時間
	MOTION_PARAM_JOG_DECEL_TIME,//自由移動時的減速度時間

	MOTION_PARAM_JOG_MAX_VELOCITY_X,//X軸自由移動時的最大速度 	
	MOTION_PARAM_JOG_MAX_VELOCITY_Y,//Y軸自由移動時的最大速度 	
	MOTION_PARAM_JOG_MAX_VELOCITY_Z,//Z軸自由移動時的最大速度 	

	MOTION_PARAM_JOG_MOVING_VELOCITY_X,//X軸自由移動時的移動速度	
	MOTION_PARAM_JOG_MOVING_VELOCITY_Y,//Y軸自由移動時的移動速度	
	MOTION_PARAM_JOG_MOVING_VELOCITY_Z,//Z軸自由移動時的移動速度	
	//---------------------------------------------------------------------------------//	
	MOTION_PARAM_ACCEL_VALUE_X,//X軸加速度, 單位mm/s/s
	MOTION_PARAM_ACCEL_VALUE_Y,//Y軸加速度, 單位mm/s/s
	MOTION_PARAM_ACCEL_VALUE_Z,//Z軸加速度, 單位mm/s/s

	MOTION_PARAM_ACCEL_TIME_X,//X軸加速度時間, sec
	MOTION_PARAM_ACCEL_TIME_Y,//Y軸加速度時間, sec
	MOTION_PARAM_ACCEL_TIME_Z,//Z軸加速度時間, sec
	
	MOTION_PARAM_DECEL_TIME_X,//X軸減速度時間, sec
	MOTION_PARAM_DECEL_TIME_Y,//Y軸減速度時間, sec
	MOTION_PARAM_DECEL_TIME_Z,//Z軸減速度時間, sec

	MOTION_PARAM_ACCEL_MIN_TIME_X,//X軸加速度最小時間, sec
	MOTION_PARAM_ACCEL_MIN_TIME_Y,//Y軸加速度最小時間, sec
	MOTION_PARAM_ACCEL_MIN_TIME_Z,//Z軸加速度最小時間, sec

	MOTION_PARAM_MOVING_CURVE_MODE_X,//X軸移動曲線
	MOTION_PARAM_MOVING_CURVE_MODE_Y,//Y軸移動曲線
	MOTION_PARAM_MOVING_CURVE_MODE_Z,//Z軸移動曲線

	MOTION_PARAM_ACC_TIME_ADJUST_MODE_X,//X軸加減速度時間調整模式
	MOTION_PARAM_ACC_TIME_ADJUST_MODE_Y,//Y軸加減速度時間調整模式
	MOTION_PARAM_ACC_TIME_ADJUST_MODE_Z,//Z軸加減速度時間調整模式

	MOTION_PARAM_TRIANGLE_CORRECTION_X,//X軸三角波抑制
	MOTION_PARAM_TRIANGLE_CORRECTION_Y,//Y軸三角波抑制
	MOTION_PARAM_TRIANGLE_CORRECTION_Z,//Z軸三角波抑制		
	//---------------------------------------------------------------------------------//	
	MOTION_PARAM_TEST_TIME_DISTANCE_X,//X軸測驗時間的偏移量, um
	MOTION_PARAM_TEST_TIME_DISTANCE_Y,//Y軸測驗時間的偏移量, um
	MOTION_PARAM_TEST_TIME_DISTANCE_Z,//Z軸測驗時間的偏移量, um	
	//---------------------------------------------------------------------------------//	
	MOTION_PARAM_LIMIT_MAX_X,//X軸的最大範圍
	MOTION_PARAM_LIMIT_MAX_Y,//Y軸的最大範圍
	MOTION_PARAM_LIMIT_MAX_Z,//Z軸的最大範圍
	MOTION_PARAM_LIMIT_MIN_X,//X軸的最小範圍
	MOTION_PARAM_LIMIT_MIN_Y,//Y軸的最小範圍
	MOTION_PARAM_LIMIT_MIN_Z,//Z軸的最小範圍	
	MOTION_PARAM_LIMIT_MARGIN_X,//X軸的範圍內縮值
	MOTION_PARAM_LIMIT_MARGIN_Y,//Y軸的範圍內縮值
	MOTION_PARAM_LIMIT_MARGIN_Z,//Z軸的範圍內縮值
	
	MOTION_PARAM_ENABLE_SOFTWARE_LIMIT,//是否啟用運動系統內部的軟體極限
	//---------------------------------------------------------------------------------//	
	//for Aero Tech Soloist Controller	
	MOTION_PARAM_CONTROLLER_PORT_X,//X 控制器 Port
	MOTION_PARAM_CONTROLLER_PORT_Y,//Y 控制器 Port
	MOTION_PARAM_CONTROLLER_PORT_Z,//Z 控制器 Port
	
	MOTION_PARAM_CONTROLLER_IP_X,//X 控制器 IP
	MOTION_PARAM_CONTROLLER_IP_Y,//X 控制器 IP
	MOTION_PARAM_CONTROLLER_IP_Z,//X 控制器 IP
	MOTION_PARAM_END
};
//-------------------------------------------------------------------------------------//
//最佳化路徑
typedef struct tagScanPath
{
	//-------------------------------------------------------------------------//	
	int   m_StartPosX;           //此路徑下的移動的起始位置
	int   m_EndPosX;             //此路徑下的移動的結束位置
	int   m_CurrentPosY;         //現在的位置
	int   m_CurrentPathID;       //現在的位置的編號
	//-------------------------------------------------------------------------//	
	int   m_TriggerStartPosX;    //此路徑下的觸發的起始位置
	int   m_TriggerEndPosX;      //此路徑下的觸發的結束位置
	//-------------------------------------------------------------------------//	
	int   m_FOVStartIDX_C;         //此路徑下的中間相機起始影像張數
	int   m_FOVEndIDX_C;           //此路徑下的中間相機結束影像張數
	int   m_FOVStartIDX_A;         //此路徑下的斜角相機起始影像張數
	int   m_FOVEndIDX_A;           //此路徑下的斜角相機結束影像張數
	//-------------------------------------------------------------------------//	
	int   m_MotionTriggerStartIndex;//AeroTech Soloist的PSO Array參考表-起點
	int   m_MotionTriggerNElements;//AeroTech Soloist的PSO Array參考表-數量
	//-------------------------------------------------------------------------//	
	tagScanPath()
	{
		m_StartPosX = 0;           //此路徑下的移動的起始位置
		m_EndPosX = 0;             //此路徑下的移動的結束位置
		m_CurrentPosY = 0;         //現在的位置
		m_CurrentPathID = 0;       //現在的位置的編號
		m_TriggerStartPosX = 0;    //此路徑下的觸發的起始位置
		m_TriggerEndPosX = 0;      //此路徑下的觸發的結束位置	
		m_FOVStartIDX_C = 0;         //此路徑下的中間相機起始影像張數
		m_FOVEndIDX_C = 0;           //此路徑下的中間相機結束影像張數
		m_FOVStartIDX_A = 0;         //此路徑下的斜角相機起始影像張數
		m_FOVEndIDX_A = 0;           //此路徑下的斜角相機結束影像張數	
		m_MotionTriggerStartIndex = 0;//AeroTech Soloist的PSO Array參考表-起點
		m_MotionTriggerNElements = 0;//AeroTech Soloist的PSO Array參考表-數量
	}
	//-------------------------------------------------------------------------//	
} TScanPath, *PScanPath;
//-------------------------------------------------------------------------------------//
typedef struct tagMotionParameter
{
	int    m_SaveMotionCardParam;           //是否儲存運動卡參數
	int    m_SaveCurrentMotionProcess;      //是否儲存運動訊息
	int    m_AccelerationAdjust;            //加速度調整
	int    m_XYCaliEnable;                  //是否XY座標校正	
	double m_XYCaliExtendRange;             //XY座標補正外擴範圍
	double m_SCurveVelRatio;                //S-Curve速度比例 	
	int    m_SignPositiveConvert;           //內部方向性轉換
	//---------------------------------------------------------------------------------//	
	int    m_SignPositiveX;                 //X軸與Cad的正負方向
	int    m_SignPositiveY;                 //Y軸與Cad的正負方向
	int    m_SignPositiveZ;                 //Z軸與Cad的正負方向
	//---------------------------------------------------------------------------------//	
	double m_HomePreMoveDisX;               //X軸歸零前移動距離
	double m_HomePreMoveDisY;               //Y軸歸零前移動距離
	double m_HomePreMoveDisZ;               //Z軸歸零前移動距離
	//---------------------------------------------------------------------------------//	
	double m_HomeOrgOffsetX;                //X軸歸零後的偏移值
	double m_HomeOrgOffsetY;                //Y軸歸零後的偏移值
	double m_HomeOrgOffsetZ;	            //Z軸歸零後的偏移值
	//---------------------------------------------------------------------------------//	
	double m_StartVelocity;                  //起始速度  

	double m_HomeVelocityX;                  //X軸的歸零速度
	double m_HomeVelocityY;                  //Y軸的歸零速度
	double m_HomeVelocityZ;                  //Z軸的歸零速度

	double m_MaxVelocityX;                   //X軸的一般移動最大速度, 
	double m_MaxVelocityY;                   //y軸的一般移動最大速度, 
	double m_MaxVelocityZ;                   //z軸的一般移動最大速度, 

	double m_BoardInVelocityX;               //X軸的慢速度
	double m_BoardInVelocityY;               //y軸的慢速度
	double m_BoardInVelocityZ;               //z軸的慢速度

	double m_BoardOutVelocityX;              //X軸的慢速度
	double m_BoardOutVelocityY;              //y軸的慢速度
	double m_BoardOutVelocityZ;              //z軸的慢速度

	double m_GrabbingVelocityX;           //X軸的取像移動最大速度, 
	double m_GrabbingVelocityY;           //y軸的取像移動最大速度, 
	double m_GrabbingVelocityZ;           //z軸的取像移動最大速度, 
	//---------------------------------------------------------------------------------//	
	double m_StageStartPosX;                 //機台起始的位置X
	double m_StageStartPosY;                 //機台起始的位置Y
	double m_StageStartPosZ;                 //機台起始的位置Z

	double m_StageLeavePosX;                 //機台離開的位置X
	double m_StageLeavePosY;                 //機台離開的位置Y
	double m_StageLeavePosZ;                 //機台離開的位置Z

	double m_BeforePCBInPosX;                //進板前座標-X
	double m_BeforePCBInPosY;                //進板前座標-Y
	double m_BeforePCBInPosZ;                //進板前座標-Z

	double m_PCBStopRPosX_LA;                 //A軌道右停板的位置X
	double m_PCBStopRPosY_LA;                 //A軌道右停板的位置Y
	double m_PCBStopRPosZ_LA;                 //A軌道右停板的位置Z

	double m_PCBStopRPosX_LB;                 //B軌道右停板的位置X
	double m_PCBStopRPosY_LB;                 //B軌道右停板的位置Y
	double m_PCBStopRPosZ_LB;                 //B軌道右停板的位置Z

	double m_PCBStopLPosX_LA;                //A軌道左停板的位置X
	double m_PCBStopLPosY_LA;                //A軌道左停板的位置Y
	double m_PCBStopLPosZ_LA;                //A軌道左停板的位置Z

	double m_PCBStopLPosX_LB;                //B軌道左停板的位置X
	double m_PCBStopLPosY_LB;                //B軌道左停板的位置Y
	double m_PCBStopLPosZ_LB;                //B軌道左停板的位置Z

	double m_LaneLedStopRPosX_LA;            //A軌道LED右停板的位置X
	double m_LaneLedStopRPosY_LA;            //A軌道LED右停板的位置Y
	double m_LaneLedStopRPosZ_LA;            //A軌道LED右停板的位置Z

	double m_LaneLedSlowRPosX_LA;             //A軌道LED右減速的位置X
	double m_LaneLedSlowRPosY_LA;             //A軌道LED右減速的位置Y
	double m_LaneLedSlowRPosZ_LA;             //A軌道LED右減速的位置Z

	double m_LaneLedStopRPosX_LB;            //B軌道LED右停板的位置X
	double m_LaneLedStopRPosY_LB;            //B軌道LED右停板的位置Y
	double m_LaneLedStopRPosZ_LB;            //B軌道LED右停板的位置Z

	double m_LaneLedSlowRPosX_LB;            //B軌道LED右減速的位置X
	double m_LaneLedSlowRPosY_LB;            //B軌道LED右減速的位置Y
	double m_LaneLedSlowRPosZ_LB;            //B軌道LED右減速的位置Z

	double m_LaneLedStopLPosX_LA;            //A軌道LED左停板的位置X
	double m_LaneLedStopLPosY_LA;            //A軌道LED左停板的位置Y
	double m_LaneLedStopLPosZ_LA;            //A軌道LED左停板的位置Z

	double m_LaneLedSlowLPosX_LA;            //A軌道LED左減速的位置X
	double m_LaneLedSlowLPosY_LA;            //A軌道LED左減速的位置Y
	double m_LaneLedSlowLPosZ_LA;            //A軌道LED左減速的位置Z

	double m_LaneLedStopLPosX_LB;            //B軌道LED左停板的位置X
	double m_LaneLedStopLPosY_LB;            //B軌道LED左停板的位置Y
	double m_LaneLedStopLPosZ_LB;            //B軌道LED左停板的位置Z

	double m_LaneLedSlowLPosX_LB;            //B軌道LED左減速的位置X
	double m_LaneLedSlowLPosY_LB;            //B軌道LED左減速的位置Y
	double m_LaneLedSlowLPosZ_LB;            //B軌道LED左減速的位置Z	
	//---------------------------------------------------------------------------------//	
	DWORD  m_InPositionDelayTime;            //機台走定位後, 延遲時間	

	//Trigger Parameter
	double m_PreTriggerOffsetDis;           //等速前的偏移位置
	double m_PostTriggerOffsetDis;          //停止前的偏移位置
	double m_TriggerStartVelocity;
	double m_TriggerMaxVelocity;             //取回張數可以一致, 但是影像偶爾有問題
	double m_TriggerAccelerationTime;        //Trigger軸加速度時間
	double m_TriggerDecelerationTime;        //Trigger軸減速度時間	

	double m_TriggerForwardOffset;           //Trigger向前掃時, 和直接拍攝的Trigger差值, 用校正求得
	double m_TriggerBackwardOffset;          //Trigger向前掃時, 和直接拍攝的Trigger差值, 用校正求得
	double m_TriggerSubAxisOffset;           //Y軸移動的間距	
	//---------------------------------------------------------------------------------//	
	//JOG
	double m_JogStartVelocity;          //自由移動時的初始速度
	double m_JogMovingVelocity;         //自由移動時的移動速度 	
	double m_JogMaxVelocity;            //自由移動時的最大速度 	
	double m_JogAccelerationTime;       //自由移動時的加速度時間
	double m_JogDecelerationTime;       //自由移動時的減速度時間

	double m_JogMaxVelocityX;           //X軸自由移動時的最大速度 mm/s/s
	double m_JogMaxVelocityY;           //Y軸自由移動時的最大速度 mm/s/s
	double m_JogMaxVelocityZ;           //Z軸自由移動時的最大速度 mm/s/s

	double m_JogMovingVelocityX;        //X軸自由移動時的移動速度 mm/s/s
	double m_JogMovingVelocityY;        //Y軸自由移動時的移動速度 mm/s/s
	double m_JogMovingVelocityZ;        //Z軸自由移動時的移動速度 mm/s/s
	//---------------------------------------------------------------------------------//	
	double m_AccelerationValueX;//X軸加速度, 單位mm/s/s
	double m_AccelerationValueY;//Y軸加速度, 單位mm/s/s
	double m_AccelerationValueZ;//Z軸加速度, 單位mm/s/s

	double m_AccelerationTimeX;//X軸加速度時間, sec
	double m_AccelerationTimeY;//Y軸加速度時間, sec
	double m_AccelerationTimeZ;//Z軸加速度時間, sec

	double m_DecelerationTimeX;           //X軸減速度時間, sec
	double m_DecelerationTimeY;           //Y軸減速度時間, sec
	double m_DecelerationTimeZ;           //Z軸減速度時間, sec

	double m_AccelerationMinTimeX;//X軸加速度最小時間, sec
	double m_AccelerationMinTimeY;//Y軸加速度最小時間, sec
	double m_AccelerationMinTimeZ;//Z軸加速度最小時間, sec

	MOVE_CURVE_MODE    m_MovingCurveModeX;//X軸移動曲線
	MOVE_CURVE_MODE    m_MovingCurveModeY;//Y軸移動曲線
	MOVE_CURVE_MODE    m_MovingCurveModeZ;//Z軸移動曲線

	ACC_TIME_ADJUST_MODE m_AccTimeAdjustModeX;//X軸加減速度時間調整模式
	ACC_TIME_ADJUST_MODE m_AccTimeAdjustModeY;//Y軸加減速度時間調整模式
	ACC_TIME_ADJUST_MODE m_AccTimeAdjustModeZ;//Z軸加減速度時間調整模式

	bool   m_TriangleCorrectionX;//X軸三角波抑制
	bool   m_TriangleCorrectionY;//Y軸三角波抑制
	bool   m_TriangleCorrectionZ;//Z軸三角波抑制
	//---------------------------------------------------------------------------------//	
	double m_TestTimeDistanceX;//X軸測驗時間的偏移量, um
	double m_TestTimeDistanceY;//Y軸測驗時間的偏移量, um
	double m_TestTimeDistanceZ;//Z軸測驗時間的偏移量, um
	//---------------------------------------------------------------------------------//	
	double m_LimitMaxX; //X軸的最大範圍
	double m_LimitMaxY; //Y軸的最大範圍
	double m_LimitMaxZ; //Z軸的最大範圍
	double m_LimitMinX; //X軸的最小範圍
	double m_LimitMinY; //Y軸的最小範圍
	double m_LimitMinZ; //Z軸的最小範圍
	double m_LimitMarginX;//X軸的範圍內縮值
	double m_LimitMarginY;//Y軸的範圍內縮值
	double m_LimitMarginZ;//Z軸的範圍內縮值

	int    m_UsingMotionSoftwareLimit;//是否啟用運動系統內部的軟體極限
	//---------------------------------------------------------------------------------//	
	//for Aero Tech Soloist Controller	
	int   m_ControllerPortX;
	int   m_ControllerPortY;
	int   m_ControllerPortZ;	
	CString  m_ControllerIPX;
	CString  m_ControllerIPY;
	CString  m_ControllerIPZ;		
	//---------------------------------------------------------------------------------//	
	tagMotionParameter()
	{
		m_SaveMotionCardParam = FN_DISABLE;    //是否儲存運動卡參數
		m_SaveCurrentMotionProcess = FN_ENABLE;//是否儲存運動訊息
		m_AccelerationAdjust = FN_ENABLE;      //加速度調整
		m_XYCaliEnable = FN_DISABLE;         //是否XY座標補正
		m_XYCaliExtendRange = 20000;         //XY座標補正外擴範圍
		m_SCurveVelRatio = 65.0;             //S-Curve速度比例 
		m_SignPositiveConvert = FN_ENABLE;   //內部方向性轉換

		m_SignPositiveX = FN_ENABLE;         //X軸與Cad的正負向
		m_SignPositiveY = FN_ENABLE;         //Y軸與Cad的正負向
		m_SignPositiveZ = FN_ENABLE;         //Z軸與Cad的正負向

		m_HomePreMoveDisX = 100000;          //X軸歸零前移動距離
		m_HomePreMoveDisY = 100000;          //Y軸歸零前移動距離
		m_HomePreMoveDisZ = 5000;            //Z軸歸零前移動距離

		m_HomeOrgOffsetX = 50000;            //X軸歸零後的偏移值
		m_HomeOrgOffsetY = 50000;            //Y軸歸零後的偏移值
		m_HomeOrgOffsetZ = 5000;             //Z軸歸零後的偏移值
	//---------------------------------------------------------------------------------//	
		m_StartVelocity = 0;                 //起始速度  

		m_HomeVelocityX = 50000;             //X軸的歸零速度
		m_HomeVelocityY = 50000;             //Y軸的歸零速度
		m_HomeVelocityZ = 10000;             //Z軸的歸零速度

		m_MaxVelocityX  = 500000;            //X軸的一般移動最大速度, 
		m_MaxVelocityY  = 500000;            //Y軸的一般移動最大速度, 
		m_MaxVelocityZ  = 100000;            //Z軸的一般移動最大速度, 

		m_BoardInVelocityX = 200000;         //X軸的慢速度
		m_BoardInVelocityY = 200000;         //Y軸的慢速度
		m_BoardInVelocityZ = 100000;         //Z軸的慢速度

		m_BoardOutVelocityX = 500000;        //X軸的慢速度
		m_BoardOutVelocityY = 500000;        //Y軸的慢速度
		m_BoardOutVelocityZ = 100000;        //Z軸的慢速度

		m_GrabbingVelocityX = 500000;     //X軸的取像移動最大速度, 
		m_GrabbingVelocityY = 500000;     //y軸的取像移動最大速度, 
		m_GrabbingVelocityZ = 100000;     //z軸的取像移動最大速度, 
	//---------------------------------------------------------------------------------//
		m_InPositionDelayTime = 0;           //機台走定位後, 延遲時間(ms)

		//Trigger Parameter
		m_PreTriggerOffsetDis = 40000;       //等速前的偏移位置
		m_PostTriggerOffsetDis = 40000;      //停止前的偏移位置
		m_TriggerStartVelocity = 0;
		m_TriggerMaxVelocity = 400000;       //取回張數可以一致, 但是影像偶爾有問題
		m_TriggerAccelerationTime = 0.05;    //Trigger軸加速度時間 sec
		m_TriggerDecelerationTime = 0.05;    //Trigger軸減速度時間 sec		

		m_TriggerForwardOffset = 40;         //Trigger向前掃時, 和直接拍攝的Trigger差值, 用校正求得
		m_TriggerBackwardOffset = 40;        //Trigger向前掃時, 和直接拍攝的Trigger差值, 用校正求得
		//double m_TriggerSubAxisOffset;     //Y軸移動的間距	
	//---------------------------------------------------------------------------------//	
		//JOG
		m_JogStartVelocity = 0;          //自由移動時的初始速度
		m_JogMovingVelocity = 50000;     //自由移動時的移動速度 
		m_JogMaxVelocity    = 150000;    //自由移動時的最大速度 
		m_JogAccelerationTime = 0.15;    //自由移動時的加速度時間
		m_JogDecelerationTime = 0.15;    //自由移動時的減速度時間

		m_JogMaxVelocityX = 150000;     //X軸自由移動時的最大速度 mm/s/s
		m_JogMaxVelocityY = 150000;     //Y軸自由移動時的最大速度 mm/s/s
		m_JogMaxVelocityZ = 3000;       //Z軸自由移動時的最大速度 mm/s/s

		m_JogMovingVelocityX = 50000;   //X軸自由移動時的移動速度 mm/s/s
		m_JogMovingVelocityY = 50000;   //Y軸自由移動時的移動速度 mm/s/s
		m_JogMovingVelocityZ = 1000;    //Z軸自由移動時的移動速度 mm/s/s
	//---------------------------------------------------------------------------------//	
		m_AccelerationValueX = 2000000;        //X軸加速度, 單位um/s/s
		m_AccelerationValueY = 2000000;        //Y軸加速度, 單位um/s/s
		m_AccelerationValueZ = 2000000;        //Z軸加速度, 單位um/s/s

		m_AccelerationTimeX = 0.1;           //X軸加速度時間, sec
		m_AccelerationTimeY = 0.1;           //Y軸加速度時間, sec
		m_AccelerationTimeZ = 0.1;           //Z軸加速度時間, sec

		m_DecelerationTimeX = 0.1;           //X軸減速度時間, sec
		m_DecelerationTimeY = 0.1;           //Y軸減速度時間, sec
		m_DecelerationTimeZ = 0.1;           //Z軸減速度時間, sec

		m_AccelerationMinTimeX = 0.05;		//X軸加速度最小時間, sec
		m_AccelerationMinTimeY = 0.05;      //Y軸加速度最小時間, sec
		m_AccelerationMinTimeZ = 0.05;      //Z軸加速度最小時間, sec

		m_MovingCurveModeX = MOVE_CURVE_T;   //X軸移動曲線
		m_MovingCurveModeY = MOVE_CURVE_T;   //Y軸移動曲線
		m_MovingCurveModeZ = MOVE_CURVE_T;   //Z軸移動曲線

		m_AccTimeAdjustModeX = ACC_TIME_ADJUST_MIN_T;//X軸加減速度時間調整模式
		m_AccTimeAdjustModeY = ACC_TIME_ADJUST_MIN_T;//Y軸加減速度時間調整模式
		m_AccTimeAdjustModeZ = ACC_TIME_ADJUST_MIN_T;//Z軸加減速度時間調整模式

		m_TriangleCorrectionX = false;//X軸三角波抑制
		m_TriangleCorrectionY = false;//Y軸三角波抑制
		m_TriangleCorrectionZ = false;//Z軸三角波抑制
	//---------------------------------------------------------------------------------//	
		m_TestTimeDistanceX = 40000;//X軸測驗時間的偏移量, um
		m_TestTimeDistanceY = 40000;//Y軸測驗時間的偏移量, um
		m_TestTimeDistanceZ =  2000;//Z軸測驗時間的偏移量, um

		m_StageStartPosX = 0; //機台起始位置-X
		m_StageStartPosY = 0; //機台起始位置-Y
		m_StageStartPosZ = 0; //機台起始位置-Z

		m_StageLeavePosX = 0;//機台離開的位置X
		m_StageLeavePosY = 0;//機台離開的位置Y
		m_StageLeavePosZ = 0;//機台離開的位置Z

		m_BeforePCBInPosX = 0;//進板前座標-X
		m_BeforePCBInPosY = 0;//進板前座標-Y
		m_BeforePCBInPosZ = 0;//進板前座標-Z

		m_PCBStopRPosX_LA = 0; //A軌道右停板的位置X
		m_PCBStopRPosY_LA = 0; //A軌道右停板的位置Y
		m_PCBStopRPosZ_LA = 0; //A軌道右停板的位置Z

		m_PCBStopRPosX_LB = 0; //B軌道右停板的位置X
		m_PCBStopRPosY_LB = 0; //B軌道右停板的位置Y
		m_PCBStopRPosZ_LB = 0; //B軌道右停板的位置Z

		m_PCBStopLPosX_LA= 0; //A軌道左停板的位置X
		m_PCBStopLPosY_LA= 0; //A軌道左停板的位置Y
		m_PCBStopLPosZ_LA= 0; //A軌道左停板的位置Z

		m_PCBStopLPosX_LB= 0; //B軌道左停板的位置X
		m_PCBStopLPosY_LB= 0; //B軌道左停板的位置Y
		m_PCBStopLPosZ_LB= 0; //B軌道左停板的位置Z

		m_LaneLedStopRPosX_LA= 0; //A軌道LED右停板的位置X
		m_LaneLedStopRPosY_LA= 0; //A軌道LED右停板的位置Y
		m_LaneLedStopRPosZ_LA= 0; //A軌道LED右停板的位置Z

		m_LaneLedSlowRPosX_LA= 0; //A軌道LED右減速的位置X
		m_LaneLedSlowRPosY_LA= 0; //A軌道LED右減速的位置Y
		m_LaneLedSlowRPosZ_LA= 0; //A軌道LED右減速的位置Z

		m_LaneLedStopRPosX_LB= 0; //B軌道LED右停板的位置X
		m_LaneLedStopRPosY_LB= 0; //B軌道LED右停板的位置Y
		m_LaneLedStopRPosZ_LB= 0; //B軌道LED右停板的位置Z

		m_LaneLedSlowRPosX_LB= 0; //B軌道LED右減速的位置X
		m_LaneLedSlowRPosY_LB= 0; //B軌道LED右減速的位置Y
		m_LaneLedSlowRPosZ_LB= 0; //B軌道LED右減速的位置Z

		m_LaneLedStopLPosX_LA= 0; //A軌道LED左停板的位置X
		m_LaneLedStopLPosY_LA= 0; //A軌道LED左停板的位置Y
		m_LaneLedStopLPosZ_LA= 0; //A軌道LED左停板的位置Z

		m_LaneLedSlowLPosX_LA= 0; //A軌道LED左減速的位置X
		m_LaneLedSlowLPosY_LA= 0; //A軌道LED左減速的位置Y
		m_LaneLedSlowLPosZ_LA= 0; //A軌道LED左減速的位置Z

		m_LaneLedStopLPosX_LB= 0; //B軌道LED左停板的位置X
		m_LaneLedStopLPosY_LB= 0; //B軌道LED左停板的位置Y
		m_LaneLedStopLPosZ_LB= 0; //B軌道LED左停板的位置Z

		m_LaneLedSlowLPosX_LB= 0; //B軌道LED左減速的位置X
		m_LaneLedSlowLPosY_LB= 0; //B軌道LED左減速的位置Y
		m_LaneLedSlowLPosZ_LB= 0; //B軌道LED左減速的位置Z		

		m_LimitMaxX =  10000000; //X軸的最大範圍
		m_LimitMaxY =  10000000; //Y軸的最大範圍
		m_LimitMaxZ =  10000000; //Z軸的最大範圍
		m_LimitMinX = -10000000; //X軸的最小範圍
		m_LimitMinY = -10000000; //Y軸的最小範圍
		m_LimitMinZ = -10000000; //Z軸的最小範圍

		m_LimitMarginX = 5000;//X軸的範圍內縮值-5mm
		m_LimitMarginY = 5000;//Y軸的範圍內縮值-5mm
		m_LimitMarginZ = 50;//Z軸的範圍內縮值50um
		m_UsingMotionSoftwareLimit = FN_ENABLE;//是否啟用運動系統內部的軟體極限
	//---------------------------------------------------------------------------------//	
		//for Aero Tech Soloist Controller
		m_ControllerPortX = 8000;
		m_ControllerPortY = 8000;
		m_ControllerPortZ = 8000;	
		m_ControllerIPX = _T("192.168.1.2");
		m_ControllerIPY = _T("192.168.1.3");
		m_ControllerIPZ = _T("192.168.1.4");			
	//---------------------------------------------------------------------------------//
	}
} TMotionParameter, *PMotionParameter;
//-------------------------------------------------------------------------------------//
#endif//Motion_Define