#ifndef _Jet3DAOIDefine_H_
#define _Jet3DAOIDefine_H_
//-------------------------------------------------------------------------------------//
//#include <windef.h>
//#include <WinUser.h>
//#include <afxstr.h>
//啟用狀態
//-------------------------------------------------------------------------------------//
const int FN_DISABLE = 0;
const int FN_ENABLE  = 1;
//-------------------------------------------------------------------------------------//
#define   MAX_JET_PATH                     MAX_PATH
//-------------------------------------------------------------------------------------//
#define   MACHINE_TYPE_JET8000             30
//-------------------------------------------------------------------------------------//
#define   INVALID_INDEX        -1//無效的引數或者序號
#define   INVAlID_FLOAT        FLT_MAX//無效的浮點數
#define   INVALID_DOUBLE       DBL_MAX//無效的浮點數
//-------------------------------------------------------------------------------------//
#define   BARCODE_SIZE          128//條碼容量尺寸
//-------------------------------------------------------------------------------------//
#define   MAX_PROJECT_COUNT      8//最多專案數上限
//-------------------------------------------------------------------------------------//
#define   PANEL_MAX_FD_COUNT     4//整板最多定位點上限
#define   BOARD_MAX_FD_COUNT     4//單板最多定位點上限
//-------------------------------------------------------------------------------------//
#define   MAX_SYSTEM_COLOR_GROUP_COUNT    8//最多系統顏色上限
//-------------------------------------------------------------------------------------//
#define   MAX_SYSTEM_BASE_PLANE_PARAM_COUNT     8//最多系統基準面參數數量
#define   MAX_SYSTEM_NOISE_FILTER_PARAM_COUNT  12//最多系統空間雜訊過濾數量
//-------------------------------------------------------------------------------------//
#define   EXT_NAME_MASK                _T("MSK")//遮罩圖的附檔名
#define   EXT_NAME_SPACE               _T("Z3D")//空間圖的附檔名
//-------------------------------------------------------------------------------------//
#define   JET8000_INI_FILE             AOI3D_APP_CAT(".INI")//_T("JET8000.INI")
#define   IMAGE_CONFIG_INI_FILE        _T("ImageConfig.INI")
#define   SPACE_BASE_PLANE_INI_FILE    _T("SpaceBasePlane.INI")
#define   SPACE_NOISE_FILTER_INI_FILE  _T("SpaceNoiseFilter.INI")
#define   MOTION_XY_CALI_FILE          _T("MotionXYCali.PRM")
//-------------------------------------------------------------------------------------//
#define   PROJECT_REPORT_TEXT          _T("ProjectReport.TXT")
//-------------------------------------------------------------------------------------//
#define   PROJECT_VERSION_CODE_SIZE         32//專案版本號數量上限 
#define   PROJECT_VERSION_CODE_BASE_INDEX    0//專案版本號基本引數
//-------------------------------------------------------------------------------------//
//FLT:://取小數點7位數
//DBL:://取小數點15位數
//徑度
#define PI_RAD		            3.141592653589793                                
#define PERIOD_RAD              6.283185307179586//PERIOD_RAD=2*PI_RAD;
#define PERIOD_RAD_FLT          6.283185f//PERIOD_RAD=2*PI_RAD;

//角度
#define PI_DEG		            180.0
#define PERIOD_DEG              360.0//PERIOD_DEG=2*PI_DEG;
#define PERIOD_DEG_FLT         360.0f//PERIOD_DEG=2*PI_DEG;

//徑度轉角度                    
#define RAD_TO_DEG_DBL          57.295779513082321//PI_DEG/PI_RAD
#define RAD_TO_DEG_FLT          57.295780f        //PI_DEG/PI_RAD

//角度轉徑度                    
#define DEG_TO_RAD_DBL          0.017453292519943 //PI_RAD/180.0
#define DEG_TO_RAD_FLT          0.0174533f        //PI_RAD/180.0f
//-------------------------------------------------------------------------------------//
const int OBJECT_STATE_NULL     = 0x00000000;//清除狀態
const int OBJECT_STATE_SELECTED = 0x00000001;//選取狀態
const int OBJECT_STATE_DELETED  = 0x00000002;//刪除狀態
const int OBJECT_STATE_ENABLED  = 0x00000004;//啟用狀態
const int OBJECT_STATE_MAXIMUM  = 0x00000008;//最大狀態
const int OBJECT_STATE_MINIMUM  = 0x00000010;//最小狀態
//-------------------------------------------------------------------------------------//
#define TREE_CTRL_UPDATE_NULL     0//更新樹狀圖-不更新
#define TREE_CTRL_UPDATE_STATE    1//更新樹狀圖-狀態
#define TREE_CTRL_UPDATE_TEXT     2//更新樹狀圖-文字
#define TREE_CTRL_UPDATE_ALL      0xFFFFFF//更新樹狀圖-全部
//-------------------------------------------------------------------------------------//
#define MULTI_TOWER_LIGHT_SINGLE       1//單塔燈
#define MULTI_TOWER_LIGHT_DUAL         2//雙塔燈
//-------------------------------------------------------------------------------------//
#define LAST_STATION_LINE_MODE_2       2//前站兩線式, 提前送訊號
#define LAST_STATION_LINE_MODE_2_4     3//前站兩線式, 不提前送訊號
#define LAST_STATION_LINE_MODE_4       4//前站四線式
//-------------------------------------------------------------------------------------//
#define BOM_LIST_HEADER_NULL            0
#define BOM_LIST_HEADER_ITEM            1
#define BOM_LIST_HEADER_PART_NUMBER     2
#define BOM_LIST_HEADER_DESCRIPTION     3
#define BOM_LIST_HEADER_USAGE           4
#define BOM_LIST_HEADER_LOCATION        5
//-------------------------------------------------------------------------------------//
#define CADXY_LIST_HEADER_NULL          0
#define CADXY_LIST_HEADER_MODEL_NAME    1
#define CADXY_LIST_HEADER_COMPONENT     2
#define CADXY_LIST_HEADER_POSITIONX     3
#define CADXY_LIST_HEADER_POSITIONY     4
#define CADXY_LIST_HEADER_ANGLE         5
#define CADXY_LIST_HEADER_PART_NUMBER   6
#define CADXY_LIST_HEADER_BOARD_ID      7
#define CADXY_LIST_HEADER_NOZZLE        8
//-------------------------------------------------------------------------------------//
#define GRR_SIGMA_ITEM_MAX_COUNT        6
//-------------------------------------------------------------------------------------//
enum TB_SYSTEM_ID//上下系統編號
{
	TB_SYSTEM_NONE  = 0x00,//未定義
	TB_SYSTEM_TOP   = 0x01,//第1系統
	TB_SYSTEM_BOT   = 0x02,//第2系統
	TB_SYSTEM_RETURN
};
//-------------------------------------------------------------------------------------//
enum COPY_HUGE_FILES_MODE//複製大量檔案模式
{
	COPY_HUGE_FILES_COPY      = 1,
	COPY_HUGE_FILES_XCOPY     = 2,
	COPY_HUGE_FILES_ROBOCOPY  = 3,
	COPY_HUGE_FILES_RETURN 
};
//-------------------------------------------------------------------------------------//
enum THREAD_ID//執行緒編號
{
	THREAD_00 = 0,//None
	THREAD_01 = 1,
	THREAD_02 = 2,
	THREAD_03 = 3,
	THREAD_04 = 4,
	THREAD_05 = 5,
	THREAD_06 = 6,
	THREAD_07 = 7,
	THREAD_08 = 8,
	THREAD_09 = 9,
	THREAD_10 = 10,
	THREAD_11 = 11,
	THREAD_12 = 12,

	THREAD_RETURN
};
const unsigned int MAX_THREAD_COUNT = 16;
//-------------------------------------------------------------------------------------//
enum USER_LEVEL_MODE
{
	USER_LEVEL_SIGN_OUT   =   0,//未登入
	USER_LEVEL_OPERATOR   =   1,//作業員
	USER_LEVEL_ENGINEER   =  11,//工程師
	USER_LEVEL_SUPERVISOR =  21,//管理者
	USER_LEVEL_JET_FAE    = 101,//捷智-工程師
	USER_LEVEL_JET_SENIOR = 102,//捷智-資深工程師
	USER_LEVEL_JET_RD     = 201,//捷智-研發員
	USER_LEVEL_RETURN
};
//-------------------------------------------------------------------------------------//
enum USER_LOGIN_MODE//使用者登入模式
{
	USER_LOGIN_DISABLE    =   0,//不登入
	USER_LOGIN_ENGINEER   =   1,//工程師
	USER_LOGIN_OPERATOR   =   2,//作業員	
	USER_LOGIN_RETURN//管理者		
};
//-------------------------------------------------------------------------------------//
enum USER_LOGIN_OPTIONS//使用者登入選項
{
	USER_LOGIN_OPTIONS_PASSWORD = 0,//密碼
	USER_LOGIN_OPTIONS_FINGERPRINT = 1,//指紋
	//USER_LOGIN_OPTIONS_MULTI = 2,//兩者	
	USER_LOGIN_OPTIONS_FINGERPRINT_ONLY = 2,//只能指紋
	USER_LOGIN_OPTIONS_RETURN
};
//-------------------------------------------------------------------------------------//
//排序模式
#define    SORT_ASCEND                 1
#define    SORT_DESCEND               -1
//-------------------------------------------------------------------------------------//
//確認使用者錯誤碼
#define    CHECK_USER_ERROR_CODE_OK                        0//讀檔失敗
#define    CHECK_USER_ERROR_CODE_READ_FILE_FAULT           1//讀檔失敗
#define    CHECK_USER_ERROR_CODE_NO_USER_NAME              2//無使用者
#define    CHECK_USER_ERROR_CODE_WRONG_PASSWORD            3//密碼錯誤
#define    CHECK_USER_ERROR_CODE_LOWER_LEVEL               4//權限不足
//-------------------------------------------------------------------------------------//
enum MONITOR_STATUS_MODE
{
	MONITOR_STATUS_CLOSE          = 0,//軟體關閉
	MONITOR_STATUS_RUN            = 1,//檢測中
	MONITOR_STATUS_STOP           = 2,//停機中
	MONITOR_STATUS_ERROR          = 3,//異常中
	MONITOR_STATUS_WAIT_FOR_LAST  = 4,//等待前站送板訊號
	MONITOR_STATUS_WAIT_FOR_NEXT  = 5,//等待後站出板訊號	
	MONITOR_STATUS_RETURN
};
//-------------------------------------------------------------------------------------//
enum MACHINE_CAMERA_SIDE//設備相機方向
{
	MACHINE_CAMERA_TOP = 1,//上方相機
	MACHINE_CAMERA_BOT = 2,//下方相機

	MACHINE_CAMERA_RETURN
};
//-------------------------------------------------------------------------------------//
enum MACHINE_MODEL_TYPE//設備機種樣式
{
	MACHINE_MODEL_6500      = 6500,
	MACHINE_MODEL_8000      = 8000,
	MACHINE_MODEL_7500_TBII = 750022,//7500+2(TB)+II(2)

	MACHINE_MODEL_RETURN
};
//-------------------------------------------------------------------------------------//
const unsigned int MAX_CAMERA_COUNT = 9;
enum CAMERA_ID//相機編號
{
	CAMERA_ID_0 = 0,
	CAMERA_ID_1 = 1,
	CAMERA_ID_2 = 2,
	CAMERA_ID_3 = 3,
	CAMERA_ID_4 = 4,
	CAMERA_ID_5 = 5,
	CAMERA_ID_6 = 6,
	CAMERA_ID_7 = 7,
	CAMERA_ID_8 = 8,

	CAMERA_ID_RETURN
};
const CAMERA_ID PRIMARY_CAMERA_ID = CAMERA_ID_1;
//-------------------------------------------------------------------------------------//
enum LIGHT_MODE//2D燈源模式
{
	LIGHT_MODE_NONE =  0,
	LIGHT_MODE_A    =  1,
	LIGHT_MODE_B    =  2,
	LIGHT_MODE_C    =  3,
	LIGHT_MODE_D    =  4,
	LIGHT_MODE_E    =  5,
	LIGHT_MODE_F    =  6,
	LIGHT_MODE_G    =  7,
	LIGHT_MODE_H    =  8,//LED_CHANNEL_08
	LIGHT_MODE_3D   = 32
};
const LIGHT_MODE DEFAULT_LIGHT_MODE = LIGHT_MODE_A;
//-------------------------------------------------------------------------------------//
enum LANE_ID//軌道編號
{
	LANE_ID_NULL = 0,
	LANE_ID_A    = 1,
	LANE_ID_B    = 2,

	LANE_ID_BOTH,
	LANE_ID_RETURN
};
//-------------------------------------------------------------------------------------//
enum DISTRICT_ID//分區模式
{
	DISTRICT_ID_NULL    = 0,
	DISTRICT_ID_A       = 1,
	DISTRICT_ID_B       = 2,
	DISTRICT_ID_RETURN
};
//-------------------------------------------------------------------------------------//
enum LANE_WORK_MODE//軌道運轉模式
{
	LANE_WORK_DISABLE = 0,//不使用
	LANE_WORK_RUN     = 1,//正常運轉
	LANE_WORK_BYPASS  = 2,//輸送帶模式
	LANE_WORK_RETURN
};
//-------------------------------------------------------------------------------------//
enum MULTI_LANE_MODE//多軌道模式
{
	MULTI_LANE_OFF = 0,//無軌道
	MULTI_LANE_1   = 1,
	MULTI_LANE_2   = 2,

	MULTI_LANE_RETURN
};
//-------------------------------------------------------------------------------------//
enum FUNC_EXEC_MODE//函式執行模式
{
	FUNC_EXEC_OFF      = 0,//關閉
	FUNC_EXEC_AUTO     = 1,//自動
	FUNC_EXEC_ASK      = 2,//詢問
	FUNC_EXEC_RETURN
};
//-------------------------------------------------------------------------------------//
enum ONLINE_INPUT_TIMING//在線輸入時機
{
	ONLINE_INPUT_DISABLE          = 0,//關閉
	ONLINE_INPUT_BEFORE_SIGN_IN   = 1,//登入前
	ONLINE_INPUT_RETURN
};
//-------------------------------------------------------------------------------------//
enum MULTI_PROJECT_TEST_ORDER_MODE//多專案檢測次序模式
{
	MULTI_PROJECT_TEST_ORDER_NONE               = 0,//多專案不切換
	MULTI_PROJECT_TEST_ORDER_BY_MARK            = 1,//依照專案特徵
	MULTI_PROJECT_TEST_ORDER_BY_TURN            = 2,//依照輪流切換
	MULTI_PROJECT_TEST_ORDER_ONE_CYCLE_A_B      = 3,//依照一次檢測切換, A->B
	MULTI_PROJECT_TEST_ORDER_ONE_CYCLE_B_A      = 4,//依照一次檢測切換, B->A
	MULTI_PROJECT_TEST_ORDER_RETURN
};
//-------------------------------------------------------------------------------------//
enum ONLINE_CALIBRATION_MODE//線上校正模式
{
	ONLINE_CALIBRATION_OFF      = 0,//關閉
	ONLINE_CALIBRATION_AUTO     = 1,//自動
	ONLINE_CALIBRATION_ASK      = 2,//詢問
	ONLINE_CALIBRATION_RETURN
};
//-------------------------------------------------------------------------------------//
enum LIGHT_3D_CAST_ID//3D投光編號
{
	LIGHT_3D_CAST_00 = 0,
	LIGHT_3D_CAST_01 = 1, 
	LIGHT_3D_CAST_02 = 2,
	LIGHT_3D_CAST_03 = 3, 
	LIGHT_3D_CAST_04 = 4,
	LIGHT_3D_CAST_05 = 5, 
	LIGHT_3D_CAST_06 = 6,
	LIGHT_3D_CAST_07 = 7, 
	LIGHT_3D_CAST_08 = 8,

	LIGHT_3D_CAST_RETURN
};
//-------------------------------------------------------------------------------------//
enum MULTI_BOARD_CTRL_MODE
{
	MULTI_BOARD_CTRL_DISABLE = 0,
	MULTI_BOARD_CTRL_BOARD   = 1,
	MULTI_BOARD_CTRL_PANEL   = 2,
	MULTI_BOARD_CTRL_RETURN
};
//-------------------------------------------------------------------------------------//
enum AOI_CUSTOMER_ID//客戶編號
{
	AOI_CUSTOMER_ID_JET_TWN           = 0,//捷智-台灣	
	AOI_CUSTOMER_ID_PEGATRON_TWN      = 1,//和碩-台灣廠
	AOI_CUSTOMER_ID_KINPO_YUEYANG     = 2,//金寶-岳陽廠
	AOI_CUSTOMER_ID_FOXCONN_LONGHUA   = 3,//富士康-龍華廠
	AOI_CUSTOMER_ID_RETURN 
};
//-------------------------------------------------------------------------------------//
enum BARCODE_SPREAD_MODE//條碼擴散模式
{
	BARCODE_SPREAD_OFF     = 0,//關閉
	BARCODE_SPREAD_LOCAL   = 1,//局部
	BARCODE_SPREAD_ALL     = 2,//全部
	BARCODE_SPREAD_RETURN
};
//-------------------------------------------------------------------------------------//
enum BARCODE_BELONG_MODE//條碼屬於模式
{
	BARCODE_BELONG_NONE    = 0,
	BARCODE_BELONG_PROJECT = 1,
	BARCODE_BELONG_PANEL   = 2,
	BARCODE_BELONG_BOARD   = 3,
	BARCODE_BELONG_TRAY    = 4,	//載具
	BARCODE_BELONG_COVER   = 5, //蓋板
	BARCODE_BELONG_RETURN
};
//-------------------------------------------------------------------------------------//
enum WND_MESSAGE_MODE//視窗訊息傳送模式
{
	WND_MESSAGE_SEND                 = 1,//SEND
	WND_MESSAGE_POST                 = 2,//POST
	WND_MESSAGE_RETURN
};
//-------------------------------------------------------------------------------------//
enum WND_MESSAGE_FROM_ID//視窗訊息來源編號
{
	WND_MESSAGE_FROM_NORMAL           = 1,//一般視窗訊息
	WND_MESSAGE_FROM_MES              = 2, //MES發送視窗訊息
	WND_MESSAGE_FROM_RETURN
};
//-------------------------------------------------------------------------------------//
enum LED_CURRENT_CALI_MODE//LED電流校正模式
{
	LED_CURRENT_CALI_DISABLE = 0,//不校正
	LED_CURRENT_CALI_AVERAGE = 1,//平均電流
	LED_CURRENT_CALI_BALANCE = 2,//平衡紅綠藍
	LED_CURRENT_CALI_RETURN
};
//-------------------------------------------------------------------------------------//
typedef struct tagFuncTickCount//函數時間戳記
{
	DWORD dwEnd;
	DWORD dwStart;	
	tagFuncTickCount()
	{
		dwEnd=0;
		dwStart=0;
	}

	void SetEnd()
	{	dwEnd=GetTickCount();	}

	void SetStart()
	{	dwEnd=dwStart=GetTickCount();	}	

	DWORD GetFuncTime() const
	{	return dwEnd-dwStart; }
} TFuncTickCount, *PFuncTickCount;
//-------------------------------------------------------------------------------------//
typedef struct tagLEDTable
{
	unsigned int  PowerValue;//LED燈源強度
	unsigned int  OnOffState;//LED開關狀態	
	tagLEDTable()
	{
		PowerValue = 0;
		OnOffState = 0;		
	}
} TLEDTable, *PLEDTable;
//-------------------------------------------------------------------------------------//
typedef struct tagDLPTable
{	
	unsigned int OnOffState;//DLP開關狀態	
	unsigned int TriggerCount;//DLP觸發次數
	unsigned int PhasePatMode;//DLP樣板模式
	tagDLPTable()
	{
		OnOffState = 0;		
		TriggerCount = 0;
		PhasePatMode = 0;
	}
} TDLPTable, *PDLPTable;
//-------------------------------------------------------------------------------------//
#define DLP_CAST_COUNT       4
#define LED_CHANNEL_COUNT   12
//-------------------------------------------------------------------------------------//
enum LIGHT_TYPE
{	
	LIGHT_LED=1,
	LIGHT_DLP=2
};
//-------------------------------------------------------------------------------------//
typedef struct tagLightTable
{
	LIGHT_TYPE   LightType;//LED, DLP

	//About LED
	unsigned int LEDTurnOnTimeus;//LED開啟時間-us	
	LED_CURRENT_CALI_MODE LEDCurrCaliMode;//電流校正模式
	TLEDTable    LEDChannel[LED_CHANNEL_COUNT];	

	//About DLP
	unsigned int DLPTurnOnTimeus;//DLP開啟時間-us	
	TDLPTable    DLPCast[DLP_CAST_COUNT];

	tagLightTable()
	{		
		LightType = LIGHT_LED;
		LEDTurnOnTimeus = 1000;		
		LEDCurrCaliMode = LED_CURRENT_CALI_AVERAGE;

		::memset(LEDChannel, 0x00, sizeof(LEDChannel));
		::memset(DLPCast, 0x00, sizeof(DLPCast));

		DLPTurnOnTimeus = 0;		
	}
} TLightTable, *PLightTable;
//-------------------------------------------------------------------------------------//
enum SLICE_FUNC_MODE//Slice 功能模式
{
	SLICE_FUNC_2D_IMAGE_GRAY            =   1,//2D影像-灰階

	SLICE_FUNC_3D_4STEP_1EXP_A			=  11,//3D影像-4步1相位曝光1-A
	SLICE_FUNC_3D_4STEP_4STEP_1EXP		=  12,//3D影像-4+4步2相位曝光1
	SLICE_FUNC_3D_4STEP_2STEP_1EXP		=  13,//3D影像-4+2步2相位曝光1
	SLICE_FUNC_3D_2STEP_2STEP_1EXP		=  14,//3D影像-2+2步2相位曝光1-Joe
	SLICE_FUNC_3D_4STEP_5GC_1EXP		=  15,//3D影像-4步+5GC2相位曝光1
	SLICE_FUNC_3D_4STEP_6GC_1EXP		=  16,//3D影像-4步+6GC2相位曝光1
	SLICE_FUNC_3D_4STEP_4GC_1EXP		=  17,//3D影像-4步+4GC2相位曝光1

	SLICE_FUNC_3D_4STEP_4STEP_2LIGHT	= 112,//3D影像-4+4步2相位打光2
	SLICE_FUNC_3D_4STEP_5GC_2LIGHT		= 115,//3D影像-4步+5GC2相位打光2
	SLICE_FUNC_3D_4STEP_6GC_2LIGHT		= 116,//3D影像-4步+6GC2相位打光2
	SLICE_FUNC_3D_4STEP_4GC_2LIGHT		= 117,//3D影像-4步+4GC2相位打光2

	SLICE_FUNC_3D_4STEP_2EXP_A			= 211,//3D影像-4步1相位曝光2-A
	SLICE_FUNC_3D_4STEP_4STEP_2EXP		= 212,//3D影像-4+4步2相位曝光2
	SLICE_FUNC_3D_4STEP_2STEP_2EXP		= 213,//3D影像-4+2步2相位曝光2
	SLICE_FUNC_3D_4STEP_5GC_2EXP		= 215,//3D影像-4步+5GC2相位曝光2	
	SLICE_FUNC_3D_4STEP_6GC_2EXP		= 216,//3D影像-4步+5GC2相位曝光2
	SLICE_FUNC_3D_4STEP_4GC_2EXP		= 217 //3D影像-4步+4GC2相位曝光2
};
//-------------------------------------------------------------------------------------//
const int SLICE_UNIQUE_ID_NULL        =   0;
const int SLICE_UNIQUE_ID_COLOR_R     =   1;
const int SLICE_UNIQUE_ID_COLOR_G     =   2;
const int SLICE_UNIQUE_ID_COLOR_B     =   3;
const int SLICE_UNIQUE_ID_RGB_R       =   4;
const int SLICE_UNIQUE_ID_RGB_G       =   5;
const int SLICE_UNIQUE_ID_RGB_B       =   6;
const int SLICE_UNIQUE_ID_TOP_R       =   7;
const int SLICE_UNIQUE_ID_TOP_G       =   8;
const int SLICE_UNIQUE_ID_TOP_B       =   9;
const int SLICE_UNIQUE_ID_LOW_R       =  10;
const int SLICE_UNIQUE_ID_LOW_G       =  11;
const int SLICE_UNIQUE_ID_LOW_B       =  12;
const int SLICE_UNIQUE_ID_DLP         =  99;
const int SLICE_UNIQUE_ID_DEFAULT     =   1;
const int SLICE_UNIQUE_ID_USER_DEFINED= 100;
const int SLICE_UNIQUE_ID_MAX         = 1023;//最大值
const int SLICE_UNIQUE_ID_COUNT       = 1024;//數量

typedef struct tagSliceParam
{	
	unsigned int SliceUniqueID;//識別碼
	CAMERA_ID    SliceCameraID;//相機編號
	int          SliceCameraFrames;//相機取像數量
	unsigned int SliceCameraExpTimeus;//相機曝光時間
	unsigned int SliceCameraDelayTimeus;//相機延遲時間
	unsigned int SliceCameraGrabFrameTimeus;//相機每取像1張數所需時間
	TLightTable  SliceLightTable;//燈源組態
	unsigned int SliceTargetGray;//校正的灰階目標
	double       SliceGainValue;//亮度增益比例	
	CString      SliceName;//名稱
	int          SliceEnabled;//是否啟用	
	int          SliceVerifyTolerance;//驗證誤差
	SLICE_FUNC_MODE SliceFuncMode;//功能模式
	unsigned int SliceNextGrabBtTimeus;//下次取像間隔時間(Bt=Between)

	tagSliceParam()
	{
		//_tcscpy(SliceName, _T("None"));
		SliceName = _T("None");
		SliceUniqueID = SLICE_UNIQUE_ID_NULL;
		SliceCameraID = PRIMARY_CAMERA_ID;
		SliceCameraFrames = 1;
		SliceCameraExpTimeus = 1000;//us
		SliceCameraDelayTimeus = 0;
		SliceCameraGrabFrameTimeus = 1000;//us
		SliceTargetGray = 64;
		SliceGainValue = 1.0;
		SliceEnabled = FN_ENABLE;
		SliceVerifyTolerance = 10;
		SliceFuncMode = SLICE_FUNC_2D_IMAGE_GRAY;
		SliceNextGrabBtTimeus = 0;//us
	}
} TSliceParam, *PSliceParam;
//-------------------------------------------------------------------------------------//
enum FRAME_TYPE
{
	FRAME_NULL   = 0,//無定義
	FRAME_GRAY   = 1,//灰階影像
	FRAME_BAYER  = 2,//Bayer影像
	FRAME_COLOR  = 3,//彩色影像
	FRAME_SPACE  = 4 //空間影像-3D
};
//-------------------------------------------------------------------------------------//
const int FRAME_MAX_COUNT              =   8;
const int FRAME_UNIQUE_ID_NULL         =   0;
const int FRAME_UNIQUE_ID_FD           =   1;//501;
const int FRAME_UNIQUE_ID_COLOR        =   2;//502;
const int FRAME_UNIQUE_ID_RGB          =   3;//503;
const int FRAME_UNIQUE_ID_TOP          =   4;//504;
const int FRAME_UNIQUE_ID_LOW          =   5;//505;
const int FRAME_UNIQUE_ID_DLP          =   6;//506;
const int FRAME_UNIQUE_ID_DEFAULT      =   FRAME_UNIQUE_ID_RGB;
const int FRAME_UNIQUE_ID_USER_DEFINED = 100;//600;
const int FRAME_UNIQUE_ID_MAX          = 255;//最大值
const int FRAME_UNIQUE_ID_COUNT        = 256;//數量
const int GUIDEIMAGE_UNIQUE_ID         = FRAME_UNIQUE_ID_LOW;
//-------------------------------------------------------------------------------------//
typedef struct tagFrameParam
{
	unsigned int      FrameUniqueID;//識別碼	
	FRAME_TYPE        FrameType;//樣式
	unsigned int      FrameSliceID1;//對應的Slice ID-1
	unsigned int      FrameSliceID2;//對應的Slice ID-2
	unsigned int      FrameSliceID3;//對應的Slice ID-3

	unsigned int      FrameSliceIdx1;//對應的Slice Idx-1
	unsigned int      FrameSliceIdx2;//對應的Slice Idx-2
	unsigned int      FrameSliceIdx3;//對應的Slice Idx-3
	unsigned int      FrameImageCount;//影像張數
	
	double            FrameSaturationRed;//飽和調整-紅色//20190219
	double            FrameSaturationGreen;//飽和調整-綠色//20190219
	double            FrameSaturationBlue;//飽和調整-藍色//20190219
	CString           FrameName;

	tagFrameParam()
	{
		FrameUniqueID = FRAME_UNIQUE_ID_NULL;
		//_tcscpy(FrameName, _T(""));		
		FrameName = _T("");
		FrameType  = FRAME_GRAY;
		FrameSliceID1   = SLICE_UNIQUE_ID_NULL;
		FrameSliceID2   = SLICE_UNIQUE_ID_NULL;
		FrameSliceID3   = SLICE_UNIQUE_ID_NULL;

		FrameSliceIdx1  = -1;
		FrameSliceIdx2  = -1;
		FrameSliceIdx3  = -1;
		FrameImageCount = 0;

		FrameSaturationRed = 1.0;//飽和調整-紅色
		FrameSaturationGreen = 1.0;//飽和調整-綠色
		FrameSaturationBlue = 1.0;//飽和調整-藍色
	}
	bool CompareFrameParam(const tagFrameParam &Param)
	{
		if ( FrameUniqueID != Param.FrameUniqueID ) { return false; }
		if ( FrameName != Param.FrameName ) { return false; }
		if ( FrameType != Param.FrameType ) { return false; }
		if ( FrameSliceID1 != Param.FrameSliceID1 ) { return false; }
		if ( FrameSliceID2 != Param.FrameSliceID2 ) { return false; }
		if ( FrameSliceID3 != Param.FrameSliceID3 ) { return false; }
		if ( FrameSliceIdx1 != Param.FrameSliceIdx1 ) { return false; }
		if ( FrameSliceIdx2 != Param.FrameSliceIdx2 ) { return false; }
		if ( FrameSliceIdx3 != Param.FrameSliceIdx3 ) { return false; }
		if ( FrameImageCount != Param.FrameImageCount ) { return false; }
		if ( FrameSaturationRed != Param.FrameSaturationRed ) { return false; }
		if ( FrameSaturationGreen != Param.FrameSaturationGreen ) { return false; }
		if ( FrameSaturationBlue != Param.FrameSaturationBlue ) { return false; }
		return true;
	}

	CString GetFrameGridName(bool ShowUniID=true) const
	{
		CString strValue;		
		if ( false == ShowUniID )
		{	strValue.Format(_T("%s"), FrameName);	}
		else
		{	strValue.Format(_T("%s[%d]"), FrameName, FrameUniqueID);	}		
		return strValue;
	}
} TFrameParam, *PFrameParam;
//-------------------------------------------------------------------------------------//
enum MULTI_LANGUAGE_MODE//多國語系模式
{
	MULTI_LANGUAGE_ENGLISH      = 1,//英文版
	MULTI_LANGUAGE_CHINESE_TRAD = 2,//中文繁體版
	MULTI_LANGUAGE_CHINESE_SIMP = 3,//中文簡體版	
	MULTI_LANGUAGE_LOCAL        = 4 //當地版
};
//-------------------------------------------------------------------------------------//
enum SYSTEM_INITIAL_STEP//系統初始化步驟
{
	SYSTEM_INITIAL_CAMERA,
	SYSTEM_INITIAL_MOTION_XYZ,
	SYSTEM_INITIAL_PLC,
	SYSTEM_INITIAL_LIGHT_CTRL,
	SYSTEM_INITIAL_PHASE_CTRL,
	SYSTEM_INITIAL_IMAGE_LIB_EURESYS, 
	SYSTEM_INITIAL_IMAGE_LIB_MIM_LIB,
	SYSTEM_INITIAL_IMAGE_LIB_DTK_LIB,
	SYSTEM_INITIAL_IMAGE_LIB_HON_LIB,
	SYSTEM_INITIAL_ITS_COMM,	
	SYSTEM_INITIAL_AI_SERVER,	
	SYSTEM_INITIAL_BARCODE_DEVICE,
	SYSTEM_INITIAL_RETURN
};
//-------------------------------------------------------------------------------------//
enum NEW_PROJECT_MODE//新專案模式
{
	NEW_PROJECT_ONLINE  = 1,//新專案-在線
	NEW_PROJECT_OFFLINE = 2,//新專案-離線編程
	NEW_PROJECT_RETURN
};
//-------------------------------------------------------------------------------------//
enum OFFLINE_VERSION_MODE//離線版本模式
{
	OFFLINE_VERSION_NORMAL         = 0,//一般
	OFFLINE_VERSION_HOST_TUNING    = 1,//本機調適
	OFFLINE_VERSION_REMOTE_TUNING  = 2,//遠端調適
	OFFLINE_VERSION_RETURN
};
//-------------------------------------------------------------------------------------//
enum TASK_MODE//任務模式
{
	TASK_NONE=0,          //任務-無
	TASK_PROJECT_MAP,     //任務-專案底圖
	TASK_ALIGN_PROJECT,   //任務-對齊專案
	TASK_EXPORT_OFFLINE,  //任務-匯出離線專案
	TASK_TUNING_PROJECT,  //任務-檢測調適專案
	TASK_TUNING_OFFLINE,  //任務-檢測離線專案
	TASK_INSPECT_PROJECT, //任務-檢測檢測專案
	TASK_EXPORT_COMPONENT, //任務-輸出零件圖
	TASK_LANE_BYPASS,      //任務-軌道流片模式
	TASK_MODE_RETURN
};
//-------------------------------------------------------------------------------------//
enum TASK_STATE_MODE//任務狀態模式
{
	TASK_STATE_NONE=0,  //任務狀態-無
	TASK_STATE_RUNNING, //任務狀態-執行中
	TASK_STATE_TO_STOP, //任務狀態-取消執行
	TASK_STATE_TO_ABORT,//任務狀態-中止執行 
	TASK_STATE_IDLE     //任務狀態-閒置中
};
//-------------------------------------------------------------------------------------//
enum LANE_STATE_MODE//任務狀態模式
{
	LANE_STATE_NONE = 0,  //任務狀態-無
	LANE_STATE_RUNNING,   //任務狀態-執行中
	LANE_STATE_PAUSE,     //任務狀態-暫停
	LANE_STATE_BYPASS,    //任務狀態-直通
};
//-------------------------------------------------------------------------------------//
enum SIMPLE_JOB_MODE//簡單工作模式
{
	SIMPLE_JOB_NULL               = 0,//停止
	SIMPLE_JOB_MOVE_TO_DISTRICT_B = 1,//移動至區塊B
	SIMPLE_JOB_RETURN
};
//-------------------------------------------------------------------------------------//
enum INSPECTION_STATE_MODE//檢測狀態
{
	INSPECTION_STATE_NONE      = 1,//停止檢測
	INSPECTION_STATE_FD_PANEL  = 2,//整板定位點
	INSPECTION_STATE_FD_BOARD  = 3,//單板定位點
	INSPECTION_STATE_BARCOD    = 4,//軟體條碼
	INSPECTION_STATE_PROJECT   = 5,//產品檢測
};
//-------------------------------------------------------------------------------------//
enum ONLINE_FROM_MODE
{
	ONLINE_FROM_ONLINE_THREAD   = 1,
	ONLINE_FROM_CONVEYER_THREAD = 2,
	ONLINE_FROM_RETURN
};
//-------------------------------------------------------------------------------------//
enum ONLINE_STATE_MODE//線上檢測狀態
{	
    ONLINE_STATE_INSPECTION_STOP = 0,//停止檢測
	ONLINE_STATE_PCB_READY,          //PCB就緒
	ONLINE_STATE_INPUT_BARCODE,      //輸入條碼
	ONLINE_STATE_PROJECT_MAP,       //專案底圖
	ONLINE_STATE_PROJECT_MARK,      //專案標記	
	ONLINE_STATE_PROJECT_OPEN_CODE, //開啟專案條碼
	ONLINE_STATE_PROJECT_RELOAD,//專案重載
	ONLINE_STATE_PROJECT_RELOAD_SERVER,//專案重載伺服器
	ONLINE_STATE_PROJECT_SWITCH_BY_TURN,//專案切換-輪流
	ONLINE_STATE_PROJECT_SWITCH_BY_TURN_ONE_CYCLE_RESET,//專案切換-重設輪流一次檢測
	ONLINE_STATE_INSPECTION_START,//開始檢測	
	ONLINE_STATE_INSPECTION_WAITING,//等待檢測
    ONLINE_STATE_INSPECT_FD_PANEL,//定位整板
    ONLINE_STATE_INSPECT_FD_BOARD,//定位單板
	ONLINE_STATE_INSPECT_BARCODE,//檢測條碼
    ONLINE_STATE_INSPECT_PROJECT,//檢測專案
	ONLINE_STATE_STATICS_PROJECT,//統計專案
	ONLINE_STATE_INSPECTION_FINISH,//檢測結束
    ONLINE_STATE_WAIT_FOR_LAST_STATION,//等待上一站訊號
    ONLINE_STATE_WAIT_FOR_NEXT_STATION,//等待下一站訊號	
	ONLINE_STATE_WAIT_FOR_PCB_REMOVED,//等待板子移走
	ONLINE_STATE_WAIT_FOR_REPAIR_VERIFY,//等待維修站確認
    ONLINE_STATE_PCB_IN_START,//進板開始
    ONLINE_STATE_PCB_IN_CHECKING,//進板確認
    ONLINE_STATE_PCB_IN_FINISH,//進板結束
    ONLINE_STATE_PCB_OUT_START,//出板開始
    ONLINE_STATE_PCB_OUT_CHECKING,//出板確認
    ONLINE_STATE_PCB_OUT_FINISH,//出板結束
	ONLINE_STATE_PCB_OUT_INSIDE_START,//停側邊開始
    ONLINE_STATE_PCB_OUT_INSIDE_CHECKING,//停側邊確認
    ONLINE_STATE_PCB_OUT_INSIDE_FINISH,//停側邊結束
    ONLINE_STATE_PCB_BACK_START,//退板開始
    ONLINE_STATE_PCB_BACK_CHECKING,//退板確認
    ONLINE_STATE_PCB_BACK_FINISH,//退板結束
	ONLINE_STATE_PCB_BACK_OUT_START,//退出板開始
	ONLINE_STATE_PCB_BACK_OUT_CHECKING,//退出板確認
	ONLINE_STATE_PCB_BACK_OUT_FINISH,//退出板結束
	ONLINE_STATE_PCB_OUT_IN_START,//出板帶進板開始
    ONLINE_STATE_PCB_OUT_IN_CHECKING,//出板帶進板確認
    ONLINE_STATE_PCB_OUT_IN_FINISH,//出板帶進板結束	
	ONLINE_STATE_PCB_AUTO_RUN_START,//自動進出板開始
	ONLINE_STATE_PCB_AUTO_RUN_CHECKING,//自動進出板確認
	ONLINE_STATE_PCB_AUTO_RUN_FINISH,//自動進出板完成
	ONLINE_STATE_PCB_DUAL_RUN_START,//雙軌進出板開始//20230425
	ONLINE_STATE_PCB_DUAL_RUN_CHECKING,//雙軌進出板確認//20230425
	ONLINE_STATE_PCB_DUAL_RUN_FINISH,//雙軌進出板完成//20230425
	ONLINE_STATE_PCB_INSPECTION_PAUSE,//雙軌-使用者暫停單軌
	
	ONLINE_STATE_AUTO_CALIBRATION_XYZ_HOME,//自動校正-XYZ歸零
	ONLINE_STATE_AUTO_CALIBRATION_2D_CURRENT,//自動校正-2D電流
	ONLINE_STATE_AUTO_CALIBRATION_3D_CURRENT,//自動校正-3D電流
	ONLINE_STATE_AUTO_CALIBRATION_3D_ZERO_PLANE,//自動校正-3D相平面
	ONLINE_STATE_AUTO_CALIBRATION_3D_HEIGHT_FACTOR,//自動校正-3D高度比例

	ONLINE_STATE_APP_OPEN  =10000,//軟體起來
	ONLINE_STATE_APP_CLOSE,       //軟體關閉
	ONLINE_STATE_RETURN  //最末一碼
};
//-------------------------------------------------------------------------------------//
enum THREAD_STATE_MODE//執行緒狀態模式
{
	THREAD_STATE_NONE = 0,//剛初始化完成
	THREAD_STATE_IDLE = 1,//閒置中
	THREAD_STATE_RUNNING = 2,//執行緒收到執行，正在執行中，用來告知系統是否正在執行
	THREAD_STATE_FINISH  = 3,//呼叫要停止執行緒
	THREAD_STATE_EXCEPTION =9//異常狀態
};
//-------------------------------------------------------------------------------------//
enum THREAD_COMMAND_MODE//執行緒命令模式
{
	THREAD_COMMAND_TO_NONE = 0,//呼叫要離開執行緒
	THREAD_COMMAND_TO_EXIT = 1,//呼叫要離開執行緒
	THREAD_COMMAND_TO_IDLE = 2,//呼叫要停止執行緒
	THREAD_COMMAND_TO_RUN  = 3,//呼叫要執行執行緒	
	THREAD_COMMAND_WAIT_EXCEPTION = 9//等待異常確認
};
//-------------------------------------------------------------------------------------//
enum CONNECTED_BUFFER_TYPE//連接輸送帶樣式
{
	CONNECTED_BUFFER_FIXED   = 1,//固定式輸送帶
	CONNECTED_BUFFER_MOVABLE = 2,//移動式輸送帶-移載機
	CONNECTED_BUFFER_RETURN
};
//-------------------------------------------------------------------------------------//
enum PROJECT_TASK_MODE//專案模式
{
	PROJECT_TASK_NORMAL      = 1,//專案列表
	PROJECT_TASK_OPEN_BARCODE = 2,//軟體開專案
	PROJECT_TASK_RETURN
};
//-------------------------------------------------------------------------------------//
enum BOARD_SIDE_MODE//單板正背面
{
	BOARD_SIDE_NONE=0,
	BOARD_SIDE_TOP=1,
	BOARD_SIDE_BOT=2,
	BOARD_SIDE_RETURN
};
//-------------------------------------------------------------------------------------//
enum BOARD_ORIENTATION_MODE//單板的方向性
{	
	BOARD_ORIENTATION_030     =  30, //預設單板方向
	BOARD_ORIENTATION_060     =  60, //030+Mirror
	BOARD_ORIENTATION_120     = 120, //030+090
	BOARD_ORIENTATION_150     = 150, //
	BOARD_ORIENTATION_210     = 210, //030+180
	BOARD_ORIENTATION_240     = 240,
	BOARD_ORIENTATION_300     = 300, //030+270
	BOARD_ORIENTATION_330     = 330,
	BOARD_ORIENTATION_RETURN  //最末一碼
};
//-------------------------------------------------------------------------------------//
enum IMAGE_DISPLAY_MODE//影像顯示模式
{
	IMAGE_DISPLAY_GRAY  = 1,
	IMAGE_DISPLAY_BAYER = 2,
	IMAGE_DISPLAY_COLOR = 3,
	IMAGE_DISPLAY_RETURN  //最末一碼
};
//-------------------------------------------------------------------------------------//
enum IMAGE_DISPLAY_ENHANCE_MODE//影像顯示強化模式
{
	IMAGE_DISPLAY_ENHANCE_NONE            = 0,
	IMAGE_DISPLAY_ENHANCE_GAIN            = 1,
	IMAGE_DISPLAY_ENHANCE_GAMMA           = 2,
	IMAGE_DISPLAY_ENHANCE_SHARPNESS_GAIN  = 3,
	IMAGE_DISPLAY_ENHANCE_SHARPNESS_GAMMA = 4,
	IMAGE_DISPLAY_ENHANCE_LOCAL_GAMMA     = 5,
	IMAGE_DISPLAY_ENHANCE_RETURN //最末一碼
};
//-------------------------------------------------------------------------------------//
enum IMAGE_DEBAYER_MODE//顏色還原模式
{
	IMAGE_DEBAYER_CAMERA_API = 1,
	IMAGE_DEBAYER_OPEN_CV    = 2,
	IMAGE_DEBAYER_RAW_COLOR  = 3,
	IMAGE_DEBAYER_RAW_MONO   = 4,
	IMAGE_DEBAYER_RETURN    //最末一碼
};
//-------------------------------------------------------------------------------------//
enum IMAGE_SRC_MODE//影像來源模式
{
	IMAGE_SRC_GRAY         = 0,//影像來源-灰階
	IMAGE_SRC_COLOR        = 1,//影像來源-彩色
	IMAGE_SRC_RED          = 2,//影像來源-紅色
	IMAGE_SRC_GREEN        = 3,//影像來源-綠色
	IMAGE_SRC_BLUE         = 4,//影像來源-藍色
	IMAGE_SRC_LIGHTNESS    = 5,//影像來源-亮色 
	IMAGE_SRC_SYNTHESIS    = 6,//影像來源-合成
	IMAGE_SRC_DARKNESS     = 7,//影像來源-暗色 
	IMAGE_SRC_SATURATION   = 8,//影像來源-飽和
	IMAGE_SRC_RED_RATIO    = 9,//影像來源-紅色比例
	IMAGE_SRC_GREEN_RATIO  =10,//影像來源-綠色比例
	IMAGE_SRC_BLUE_RATIO   =11,//影像來源-藍色比例	
	IMAGE_SRC_MAX_GRN_BLU  =12,//影像來源-最亮的綠藍
	IMAGE_SRC_RETURN           //最末一碼  
};
//-------------------------------------------------------------------------------------//
enum EDGE_ENHANCE_MODE//邊緣強化
{
	EDGE_ENHANCE_DISABLE   = 0,
	EDGE_ENHANCE_SOBEL     = 1,
	EDGE_ENHANCE_DARK_TOP  = 2,//上黑	
	EDGE_ENHANCE_DARK_LEFT = 3,//左黑
	EDGE_ENHANCE_DARK_BOT  = 4,//下黑
	EDGE_ENHANCE_DARK_RIGHT= 5,//右黑
	EDGE_ENHANCE_RETURN
};
//-------------------------------------------------------------------------------------//
enum MASK_FUNC_MODE///遮罩功能模式
{
	MASK_FUNC_CALC     = 1,//計算區
	MASK_FUNC_ERASE    = 2,//忽略區
	MASK_FUNC_RETURN	
};
//-------------------------------------------------------------------------------------//
enum BINARY_MODE///二值化模式
{
	BINARY_DISABLE                = 0,//二值化-關閉
	BINARY_COLOR_FILTER           = 1,//二值化-彩色過濾
	BINARY_FIXED_THRESHOLD        = 2,//二值化-固定閥值
	BINARY_DYNAMIC_THRESHOLD      = 3,//二值化-動態閥值
	BINARY_RELATIVE_AVE_THRESHOLD = 4,//二值化-相對閥值	
	BINARY_ADAPTIVE_THRESHOLD     = 5,//二值化-適應閥值		
	BINARY_RETURN                     //最末一碼 
};
//-------------------------------------------------------------------------------------//
enum NOISE_FILTER_MODE///雜訊過濾模式
{
	NOISE_FILTER_DISABLE       = 0,//雜訊過濾-關閉
	NOISE_FILTER_SMOOTH        = 1,//雜訊過濾-平均濾波
	NOISE_FILTER_MEDIAN        = 2,//雜訊過濾-中值濾波
	NOISE_FILTER_OPEN          = 3,//雜訊過濾-開運算
	NOISE_FILTER_CLOSE         = 4,//雜訊過濾-閉運算
	NOISE_FILTER_3LEVEL        = 5,//雜訊過濾-3分等濾波
	NOISE_FILTER_MEDIAN2       = 6,//雜訊過濾-中值濾波-2
	NOISE_FILTER_PYRAMID_MEDIAN = 7,//雜訊過濾-金字塔中值濾波
	NOISE_FILTER_CONTENTAWARE  = 8,//雜訊過濾-Content Aware濾波
	NOISE_FILTER_FAST_MEDIAN   = 9,//雜訊過濾-AVX Median filter
	NOISE_FILTER_FAST_AVERAGE  = 10,//雜訊過濾-AVX Average filter
	NOISE_FILTER_EROSION        =11,//雜訊過濾-侵蝕
	NOISE_FILTER_DILATION       =12,//雜訊過濾-膨脹
	NOISE_FILTER_GRADIENT       =13,//雜訊過濾-Gradient
	NOISE_FILTER_RETURN            //最末一碼
};
//-------------------------------------------------------------------------------------//
enum FIELD_PATH_MODE
{
	FIELD_PATH_SPATH_HOR  = 1,  //水平配置
	FIELD_PATH_SPATH_VER  = 2,  //垂直配置
	FIELD_PATH_SPATH_USER = 3,  //手動配置
	FIELD_PATH_SPATH_RETURN
};
//----------------------------------------------------------------------------------//
enum FIELD_BUILD_MODE//區域建立模式
{
	FIELD_BUILD_NONE          = 0,//未定義
	FIELD_BUILD_MATRIX        = 1,//掃描-等間距
	FIELD_BUILD_RANDOM_PANEL  = 2,//走停-任意移動, Project, Panel, Board, Component, 分成4種, 後面再寫
	FIELD_BUILD_RANDOM_BOARD  = 3,//走停-各自單板配置
	FIELD_BUILD_RANDOM_PROJECT= 4,//走停-全部混在一起
	FIELD_BUILD_RETURN            //最末一碼 
};
//-------------------------------------------------------------------------------------//
enum FIELD_BUILD_AREA_MODE//區域建立面積模式
{
	FIELD_BUILD_AREA_NONE       = 0,//未定義
	FIELD_BUILD_AREA_COMPONENT  = 1,//零件
	FIELD_BUILD_AREA_BOARD      = 2,//單板
	FIELD_BUILD_AREA_RETURN     //最末一碼 
};
//-------------------------------------------------------------------------------------//
enum FIELD_DIVISION_MODE//區域分割模式
{
	FIELD_DIVISION_MASS_AREA       = 1,//最大面積
	FIELD_DIVISION_DIAGONAL_LINE   = 2,//對角線
	FIELD_DIVISION_HORIZONTAL_LINE = 3,//水平線
	FIELD_DIVISION_VERTICAL_LINE   = 4,//垂直線
	FIELD_DIVISION_RETURN
};
//----------------------------------------------------------------------------------//
enum FIELD_SIZE_MODE//區域尺寸模式
{
     FIELD_SIZE_NONE    =   0,
     FIELD_SIZE_010     =  10,
     FIELD_SIZE_020     =  20,
     FIELD_SIZE_030     =  30,
     FIELD_SIZE_040     =  40,
     FIELD_SIZE_050     =  50,
     FIELD_SIZE_060     =  60,
     FIELD_SIZE_070     =  70,
     FIELD_SIZE_080     =  80,
     FIELD_SIZE_090     =  90,
     FIELD_SIZE_100     = 100,
     FIELD_SIZE_DOT     = 199,//校正用
     FIELD_SIZE_RETURN
};
//-------------------------------------------------------------------------------------//
enum AUTO_SWITCH_WND_3D_FRAME_MODE//自動切換3D畫面模式
{
	AUTO_SWITCH_WND_3D_FRAME_DISABLE = 0,//關閉
	AUTO_SWITCH_WND_3D_FRAME_ENABLE  = 1,//啟用
	AUTO_SWITCH_WND_3D_FRAME_BY_SIZE = 2,//依據尺寸
	AUTO_SWITCH_WND_3D_FRAME_BY_TYPE = 3,//依據樣式
	AUTO_SWITCH_WND_3D_FRAME_BY_GROUP_CHANGE = 4,//依據群組切換
	AUTO_SWITCH_WND_3D_RETURN
};
//-------------------------------------------------------------------------------------//
enum OFFLINE_FILE_MODE//離線檔案模式
{
	OFFLINE_FILE_DISABLE    = 0, //未定義
	OFFLINE_FILE_PROGRAM    = 1, //編程
	OFFLINE_FILE_INSPECTION = 2, //檢測
	OFFLINE_FILE_RETURN          //最末一碼 
};
//-------------------------------------------------------------------------------------//
enum HEIGHT_DATA_CORRECT_MODE//高度校正模式
{
	HEIGHT_DATA_CORRECT_NONE         = 0,//關閉
	HEIGHT_DATA_CORRECT_ENABLE       = 1,//啟用
	HEIGHT_DATA_CORRECT_RETURN           //最末一碼 
};
//-------------------------------------------------------------------------------------//
enum BASE_PLANE_PROC_TYPE//基準面程序樣式
{
	BASE_PLANE_PROC_TYPE_1    = 1,
	BASE_PLANE_PROC_TYPE_2    = 2,
	BASE_PLANE_PROC_TYPE_RETURN
};
//-------------------------------------------------------------------------------------//
enum BASE_PLANE_TOWARD_MODE//基準面朝向方式
{
	BASE_PLANE_TOWARD_ANY       = 1,//朝向任意
	BASE_PLANE_TOWARD_VERTICAL  = 2,//朝向垂直
	BASE_PLANE_TOWARD_RETURN
};
//-------------------------------------------------------------------------------------//
enum CALC_BASE_PLANE_MODE//計算基準面方式
{
	CALC_BASE_PLANE_DISABLE     =  0, //關閉基準面
	CALC_BASE_PLANE_AVERAGE     =  1, //平均值
	CALC_BASE_PLANE_CORNER      =  2, //四個端點
	CALC_BASE_PLANE_ISO_DATA    =  3, //Iso data
	CALC_BASE_PLANE_OTSU        =  4, //OTSU
	CALC_BASE_PLANE_CORNER_ONLY =  5, //只有四個端點
	CALC_BASE_PLANE_SURROUND    =  6, //外圍
	CALC_BASE_PLANE_AUTO_LOWER  =  7, //自動找最低	
	CALC_BASE_PLANE_PANEL       =  8, //參考整板
	CALC_BASE_PLANE_LOCAL       =  9, //參考局部平面
	CALC_BASE_PLANE_RETURN           //最末一碼 
};
//-------------------------------------------------------------------------------------//
enum BASE_PLANE_EQUATION_MODE//基準面方程式模式
{
	BASE_PLANE_EQUATION_NONE    = 0,//0階-無
	BASE_PLANE_EQUATION_PLANE   = 1,//1階-平面
	BASE_PLANE_EQUATION_CURVE   = 2,//2階-曲面
	BASE_PLANE_EQUATION_RETURN
};
//-------------------------------------------------------------------------------------//
enum BASE_PLANE_AUTO_REGION_MODE//基準面自動選區模式
{
	BASE_PLANE_AUTO_REGION_DISABLE = 0,//關閉
	BASE_PLANE_AUTO_REGION_GROUP   = 1,//群組比較
	BASE_PLANE_AUTO_REGION_LOWEST  = 2,//最低高度
	BASE_PLANE_AUTO_REGION_RETURN
};
//-------------------------------------------------------------------------------------//
enum BASE_PLANE_BODY_OUTSIDE_MODE//基準面本體外圍模式
{
	BASE_PLANE_BODY_OUTSIDE_DISABLE    = 0,//關閉
	BASE_PLANE_BODY_OUTSIDE_BODY       = 1,//本體
	BASE_PLANE_BODY_OUTSIDE_BODY_LAND  = 2,//本體+特徵框
	BASE_PLANE_BODY_OUTSIDE_RETURN
};
//-------------------------------------------------------------------------------------//
enum ITS_COMMUNICATION_MODE//與ITS通訊模式
{
	ITS_COMMUNICATION_SOCKET       = 1,
	ITS_COMMUNICATION_RABBIT_MQ    = 2,
	ITS_COMMUNICATION_FILE_CHECKER = 3,
	ITS_COMMUNICATION_RETURN
};
//-------------------------------------------------------------------------------------//
enum MOVE_TO_EVENT_MODE //移動至模式, 
{
	MOVE_TO_EVENT_SEL_CHANGE = 1,//選取時
	MOVE_TO_EVENT_DBCLICK    = 2,//雙擊時
	MOVE_TO_EVENT_RETURN         //最末一碼 
};
//-------------------------------------------------------------------------------------//
enum RESULT_ID//結果狀態
{
	RESULT_ID_NONE           = 0,//尚未檢測
	RESULT_ID_OK             = 1,//結果良品
	RESULT_ID_NG             = 2,//結果瑕疵
	RESULT_ID_BYPASS         = 3,//結果不檢測-指定
	RESULT_ID_EXCEPTION      = 4,//結果異常
	RESULT_ID_SKIP           = 5,//結果不檢測-自動
	RESULT_ID_RETURN             //最末一碼 
};
//-------------------------------------------------------------------------------------//
enum TEST_RESULT_ID//專案結果
{
	TEST_RESULT_NONE    = 0,//未檢測 
	TEST_RESULT_OK      = 1,//良品
	TEST_RESULT_NG      = 2,//不良品
	TEST_RESULT_FD      = 3,//定位點異常
	TEST_RESULT_RETURN      //最末一碼 
};
//-------------------------------------------------------------------------------------//
enum SPC_TOWARD//SPC的朝向編號
{
	SPC_TOWARD_NONE = 0,
	SPC_TOWARD_TOP,
	SPC_TOWARD_BOT,
	SPC_TOWARD_LEFT,
	SPC_TOWARD_RIGHT
};
//-------------------------------------------------------------------------------------//
enum SPC_RESULT_ID//SPC的結果編號
{	//0:No Test  1:Skip 2:Bypass 3:Sys-Ok 4:Sys-Ng 5:Op-Ok 6:Op-Ng 7:Fd-Ng 
	SPC_RESULT_ID_NONE     = 0,
	SPC_RESULT_ID_SKIP     = 1,
	SPC_RESULT_ID_BYPASS   = 2,
	SPC_RESULT_ID_TEST_OK  = 3,
	SPC_RESULT_ID_TEST_NG  = 4,
	SPC_RESULT_ID_CHECK_OK = 5,
	SPC_RESULT_ID_CHECK_NG = 6,
	SPC_RESULT_ID_FD_NG    = 7,
	SPC_RESULT_ID_RETURN,
};
//-------------------------------------------------------------------------------------//
enum FD_NG_HANDLE_MODE//定位點瑕疵處理模式
{	
	FD_NG_HANDLE_NONE           = 0,//無作動
	FD_NG_HANDLE_PASS           = 1,//直通
	FD_NG_HANDLE_STOP           = 2,//停機警報	
	FD_NG_HANDLE_XBOARD         = 3,//報廢板
	FD_NG_HANDLE_RETURN         //最末一碼 
};
//-------------------------------------------------------------------------------------//
enum BOARD_FD_GRAB_MODE//單板定位點的取像模式
{
	BOARD_FD_GRAB_AFTER_PANEL   = 1,//整板後
	BOARD_FD_GRAB_INSPECTING    = 2,//檢測中
	BOARD_FD_GRAB_RETURN            //最末一碼 
};
//-------------------------------------------------------------------------------------//
enum STATISTIC_BY_MODE//統計依據模式
{
	STATISTIC_BY_NONE    = 0,//關閉
	STATISTIC_BY_TIME    = 1,//依據時間
	STATISTIC_BY_COUNT   = 2,//依據數量
	STATISTIC_BY_RETURN      //最末碼
};
//-------------------------------------------------------------------------------------//
enum DEFECT_HANDLE_MODE//檢出瑕疵處理模式
{
	DEFECT_HANDLE_PASS            = 1,//通過, 燒機模式
	DEFECT_HANDLE_STOP_ALARM      = 2,//停機警報
	DEFECT_HANDLE_NEXT_STOP       = 3,//停於下一站
	DEFECT_HANDLE_WAIT_FOR_REPAIR = 4,//等人員判定
	DEFECT_HANDLE_CONTROL_CENTER  = 5,//中控中心
	DEFECT_HANDLE_RETURN             //最末一碼 
};
//-------------------------------------------------------------------------------------//
enum BARCODE_DECODER_TYPE//條碼解碼樣式
{
	BARCODE_DECODER_OFF  = 0,  //關閉
	BARCODE_DECODER_EVS  = 1,  //Open-eVision
	BARCODE_DECODER_DTK  = 2,  //DTK
	BARCODE_DECODER_HON  = 3,  //Honeywell SwiftDecoder
	BARCODE_DECODER_RETURN
};
//-------------------------------------------------------------------------------------//
enum BARCODE_INPUT_TYPE//條碼輸入樣式
{
	BARCODE_INPUT_DISABLED  = 0,  //關閉
	BARCODE_INPUT_DEVICE    = 1,  //外接條碼機  
	BARCODE_INPUT_HANDHELD  = 2,  //手持條碼機	
	BARCODE_INPUT_RETURN          //最末一碼 
};
//-------------------------------------------------------------------------------------//
enum BARCODE_NG_HANDLE_MODE//條碼失敗處理模式
{
	BARCODE_NG_HANDLE_PASS     = 1,//直通
	BARCODE_NG_HANDLE_ALARM    = 2,//警報
	BARCODE_NG_HANDLE_INPUT    = 3,//輸入
	BARCODE_NG_HANDLE_RETURN       //最末一碼 
};
//-------------------------------------------------------------------------------------//
enum BARCODE_CAMERA_GRAB_MODE//相機條碼讀取時機
{
	BARCODE_CAMERA_GRAB_AFTER_FD      = 1,//定位點後
	BARCODE_CAMERA_GRAB_INSPECTING    = 2,//檢測中
	BARCODE_CAMERA_GRAB_RETURN            //最末一碼 
};
//-------------------------------------------------------------------------------------//
enum BARCODE_CAMERA_TEST_STATE//相機條碼檢測狀態
{
	BARCODE_CAMERA_TEST_STATE_ALL     = 0,//全條碼檢測
	BARCODE_CAMERA_TEST_STATE_LAST    = 1,//上次條碼檢測
	BARCODE_CAMERA_TEST_STATE_OTHERS  = 2,//其餘條碼檢測
	BARCODE_CAMERA_TEST_STATE_RETURN
};
//-------------------------------------------------------------------------------------//
enum BARCODE_DEVICE_GRAB_MODE//條碼機讀取時機
{
	BARCODE_DEVICE_GRAB_BEFORE_PCB_IN  = 1,//進板前
	BARCODE_DEVICE_GRAB_WHILE_PCB_IN   = 2,//進板中
	BARCODE_DEVICE_GRAB_AFTER_PCB_IN   = 3,//進板後
	BARCODE_DEVICE_GRAB_BEFORE_INSPECT = 4,//檢測前
	BARCODE_DEVICE_GRAB_RETURN             //最末一碼 
};
//-------------------------------------------------------------------------------------//
enum BARCODE_HANDHELD_READ_MODE//手持條碼機讀取模式
{
	BARCODE_HANDHELD_READ_MANUAL   = 0,//手動讀取
	BARCODE_HANDHELD_READ_PROJECT  = 1,//依照專案讀取
	BARCODE_HANDHELD_READ_PANEL    = 2,//依照整板讀取
	BARCODE_HANDHELD_READ_BOARD    = 3,//依照單板讀取
	BARCODE_HANDHELD_READ_RETURN       //最末一碼 
};
//-------------------------------------------------------------------------------------//
enum BARCODE_AUTO_EXPAND_MODE//條碼自動擴展模式
{
	BARCODE_AUTO_EXPAND_DISABLE          =  0,//關閉
	BARCODE_AUTO_EXPAND_INCREMENT        =  1,//自動疊加
	BARCODE_AUTO_EXPAND_ADD_CHAR_1       = 11,//額外加1字元1
	BARCODE_AUTO_EXPAND_ADD_CHAR_2       = 12,//額外加2字元01
	BARCODE_AUTO_EXPAND_REPLACE_01       = 21,//取代末1字元1
	BARCODE_AUTO_EXPAND_REPLACE_02       = 22,//取代末2字元01
	BARCODE_AUTO_EXPAND_INCREMENT_BASE36 =101,//自動疊加-36進位
	BARCODE_AUTO_EXPAND_RETURN
};
//-------------------------------------------------------------------------------------//
enum PCB_OUT_DIRECTION//出板方向
{
	PCB_OUT_DIR_FORWARD      = 1,//正向
	PCB_OUT_DIR_BACKWARD     = 2,//反向
	PCB_OUT_DIR_BACKWARD_OUT = 3,//反向出板
	PCB_OUT_DIR_RETURN       //最末一碼 
};
//-------------------------------------------------------------------------------------//
enum PCB_OUT_MODE//出板方向
{
	PCB_OUT_NORMAL           = 1,//預設
	PCB_OUT_SIDE_OUT         = 2,//兩段式, 機台內側, 再出板
	PCB_OUT_WITH_IN          = 3,//出板帶進板
	PCB_OUT_LANE_AUTO        = 4,//軌道自行運作
	PCB_OUT_OK_OUT_NG_SIDE   = 5,//OK出板, NG停內側
	PCB_OUT_RETURN        //最末一碼 
};
//-------------------------------------------------------------------------------------//
enum PANEL_SIDE_MODE//板面模式
{
	PANEL_SIDE_TOP      = 1,//上面
	PANEL_SIDE_BOTTOM   = 2,//下面
	PANEL_SIDE_HYBRID   = 3,//混合
	PANEL_SIDE_RETURN   //最末一碼 
};
//-------------------------------------------------------------------------------------//
enum OPEN_PROJECT_MODE//開啟專模式
{
	OPEN_PROJECT_FILE  = 1,//依檔案開啟
	OPEN_PROJECT_CODE  = 2,//依刷碼開啟
	OPEN_PROJECT_RETURN
}; 
//-------------------------------------------------------------------------------------//
enum VERIFY_PROJECT_MODE//驗證專案模式
{
	VERIFY_PROJECT_DISABLE    =   0,//關閉
	VERIFY_PROJECT_FILENAME   =   1,//檔名確認
	VERIFY_PROJECT_RETURN
};
//-------------------------------------------------------------------------------------//
enum ONLINE_OPEN_PROJECT_MODE//線上開啟專模式
{
	ONLINE_OPEN_PROJECT_DISABLE          = 0,//關閉
	ONLINE_OPEN_PROJECT_BARCODE_DEVICE   = 1,//外接條碼機
	ONLINE_OPEN_PROJECT_BARCODE_HANDHELD = 2,//手持條碼機
	ONLINE_OPEN_PROJECT_BARCODE_CAMERA   = 3,//相機條碼	
	ONLINE_OPEN_PROJECT_RETURN
}; 
//-------------------------------------------------------------------------------------//
enum PROJECT_LINK_SERVER_MODE//專案連線伺服器模式
{
	PROJECT_LINK_SERVER_DISABLE          = 0,//關閉
	PROJECT_LINK_SERVER_ENABLE_ALL       = 1,//全啟用
	PROJECT_LINK_SERVER_ENABLE_ALL_ASK   = 2,//全啟用-詢問

	PROJECT_LINK_SERVER_PROJECT_ONLY     = 3,//同專案
	PROJECT_LINK_SERVER_PROJECT_ONLY_ASK = 4,//同專案-詢問
	PROJECT_LINK_SERVER_RETURN
};
//-------------------------------------------------------------------------------------//
enum SAVE_TEST_MAP_MODE//儲存檢測底圖模式
{
	SAVE_TEST_MAP_DISABLE        = 0, //不儲存
	SAVE_TEST_MAP_ENB_PROG       = 1, //全圖(0x01)	
	SAVE_TEST_MAP_ENB_PANEL      = 2, //整板(0x02)
	SAVE_TEST_MAP_ENB_PROG_PANEL = 3, //全圖+整板(0x02)	
	SAVE_TEST_MAP_ENB_BOARD      = 4, //單板(0x04)
	SAVE_TEST_MAP_ENB_PROG_BOARD = 5, //全圖+單板(0x01+0x04)
	SAVE_TEST_MAP_RETURN
};
//-------------------------------------------------------------------------------------//
enum OFFLINE_IMAGE_SCOPE//離線影像範疇
{
	OFFLINE_IMAGE_FOV    = 1,//視野影像
	OFFLINE_IMAGE_PART   = 2,//部分影像
	OFFLINE_IMAGE_RETURN 
};
//-------------------------------------------------------------------------------------//
enum SAVE_TEST_IMAGE_MODE//儲存檢測圖片模式
{
	SAVE_TEST_IMAGE_DISABLE   = 0,//不儲存	
	SAVE_TEST_IMAGE_DEFECT    = 1,//瑕疵儲存
	SAVE_TEST_IMAGE_EVERYONE  = 2,//每個儲存
	SAVE_TEST_IMAGE_RETURN
};
//-------------------------------------------------------------------------------------//
enum SAVE_TEXT_FILENAME_MODE//儲存文字檔案名稱模式
{
	SAVE_TEXT_FILENAME_DATETIME             = 1,
	SAVE_TEXT_FILENAME_BARCODE_DATETIME     = 2,
	SAVE_TEXT_FILENAME_RETURN
};
//-------------------------------------------------------------------------------------//
enum SAVE_SPC_FILE_MODE//儲存SPC檔案模式
{
	SAVE_SPC_FILE_JSON_VRS     = 1,
	SAVE_SPC_FILE_JSON_RSM     = 2,
	SAVE_SPC_FILE_RETURN
};
//-------------------------------------------------------------------------------------//
enum SAVE_SPC_OTHER_FILE_MODE//儲存SPC其餘檔案模式
{
	SAVE_SPC_OTHER_FILE_OFF        = 0,//不儲存
	SAVE_SPC_OTHER_FILE_AUTO       = 1,//自動儲存
	SAVE_SPC_OTHER_FILE_ASK        = 2,//詢問儲存
	SAVE_SPC_OTHER_FILE_RETURN
};
//-------------------------------------------------------------------------------------//
enum SAVE_SPC_PART_IMAGE_MODE//儲存SPC零件圖檔模式
{
	SAVE_SPC_PART_IMAGE_DISABLE       = 0,//不儲存
	SAVE_SPC_PART_IMAGE_EVERYONE      = 1,//每個輸出	

	SAVE_SPC_PART_IMAGE_SELECTED      = 2,//僅輸出選取到的
	SAVE_SPC_PART_IMAGE_SELECTED_ADD  = 3,//僅輸出選取到的-擴增	
	SAVE_SPC_PART_IMAGE_SELECTED_COPY = 4,//僅輸出選取到的-複製到其他
	SAVE_SPC_PART_IMAGE_RETURN
};
//-------------------------------------------------------------------------------------//
enum SAVE_TEST_DATA_MODE//儲存檢測資料模式
{
	SAVE_TEST_DATA_DISABLE   = 0,//不儲存
	SAVE_TEST_DATA_ENABLE    = 1,//儲存
	SAVE_TEST_DATA_DEFECT    = 2,//瑕疵
	SAVE_TEST_DATA_RETURN
};
//-------------------------------------------------------------------------------------//
enum SAVE_TEST_WND_LIST_MODE//儲存檢測框列表模式
{
	SAVE_TEST_WND_LIST_DISABLE   = 0,//不儲存
	SAVE_TEST_WND_LIST_ENABLE    = 1,//儲存
	SAVE_TEST_WND_LIST_DEFECT    = 2,//瑕疵
	SAVE_TEST_WND_LIST_RETURN
};
//-------------------------------------------------------------------------------------//
enum ONLINE_FROMVIEW_MODE
{
	ONLINE_FROMVIEW_ONE      = 1,
	ONLINE_FROMVIEW_DUAL     = 2,
	ONLINE_FROMVIEW_RETURN
};
//-------------------------------------------------------------------------------------//
enum BOX_TOWARD//框的朝向
{
	BOX_TOWARD_NULL   =   0,//未定義
	BOX_TOWARD_UP     =   1,//朝上0x01
	BOX_TOWARD_LEFT   =   2,//朝左0x02
	BOX_TOWARD_DOWN   =   4,//朝下0x04
	BOX_TOWARD_RIGHT  =   8,//朝右0x08	
	BOX_TOWARD_RETURN     //最末一碼 
};
//-------------------------------------------------------------------------------------//
enum BOX_SHAPE_MODE//框的形狀
{
	BOX_SHAPE_RECTANGLE       = 1, //矩形-直角
	BOX_SHAPE_ROUND_RECT      = 2, //矩形-圓角
	BOX_SHAPE_ELLIPSE         = 3, //橢圓形
	BOX_SHAPE_CAPSULE         = 4, //膠囊形	
	BOX_SHAPE_BULLET	      = 5, //子彈形	
	BOX_SHAPE_HALF_ROUND_RECT = 6, //矩形-半圓角
	BOX_SHAPE_T_SHAPE         = 7, //T形
	BOX_SHAPE_RETURN          //最末一碼 
};
//-------------------------------------------------------------------------------------//
enum WND_LOGIC_TYPE //檢測框邏輯範圍
{
	WND_LOGIC_NONE      = 0,//無邏輯
	WND_LOGIC_GROUP_ID  = 1,//同群組編號
	WND_LOGIC_DEFECT_ID = 2,//同瑕疵代碼
	WND_LOGIC_RETURN        //最末一碼 
};
//-------------------------------------------------------------------------------------//
enum WND_CONSTRAIN_MODE//檢測框局限模式
{
	WND_CONSTRAIN_DISABLE	      =  0,//關閉
	WND_CONSTRAIN_PAD_RGN_MOVE    = 11,//焊盤範圍內-移動 
	WND_CONSTRAIN_PAD_RGN_X_MOVE  = 12,//焊盤範圍內-X-移動 
	WND_CONSTRAIN_PAD_RGN_Y_MOVE  = 13,//焊盤範圍內-Y-移動 

	WND_CONSTRAIN_PAD_RGN_SCALE   = 15,//焊盤範圍內-縮放
	WND_CONSTRAIN_PAD_RGN_X_SCALE = 16,//焊盤範圍內-X-縮放
	WND_CONSTRAIN_PAD_RGN_Y_SCALE = 17,//焊盤範圍內-Y-縮放

	WND_CONSTRAIN_RETURN//最末碼
};
//-------------------------------------------------------------------------------------//
enum MES_EQP_CTRL_STATE_MODE//MES機台控制狀態模式
{
	MES_EQP_CTRL_STATE_NONE     = 0,//未設定
	MES_EQP_CTRL_STATE_OFFLINE  = 1,//離線
	MES_EQP_CTRL_STATE_LOCAL    = 2,//上線-本地
	MES_EQP_CTRL_STATE_REMOTE   = 3,//上線-遠程
};
//-------------------------------------------------------------------------------------//
enum MES_STATAUS_ID
{
	MES_STATAUS_NONE                        =  0,

	MES_STATAUS_MES_SET_CMD                 =   1,//MES Set Parameters
	MES_STATAUS_MES_SET_RES                 =   2,//MES Set Parameters Response
	MES_STATAUS_MES_GET_CMD                 =   3,//MES Set GetParameters
	MES_STATAUS_MES_GET_RES                 =   4,//MES GetParameters Response

	MES_STATAUS_AOI_SET_CMD                 =  33,//AOI Set Parameters
	MES_STATAUS_AOI_SET_RES                 =  34,//AOI Set Parameters Response
	MES_STATAUS_AOI_GET_CMD                 =  35,//AOI Get Parameters
	MES_STATAUS_AOI_GET_RES                 =  36,//AOI Get Parameters Response

	MES_STATAUS_VRS_SET_CMD                 =  37,//VRS Set Parameters
	MES_STATAUS_VRS_SET_RES                 =  38,//VRS Set Parameters Response
	MES_STATAUS_VRS_GET_CMD                 =  39,//VRS Get Parameters
	MES_STATAUS_VRS_GET_RES                 =  40,//VRS Get Parameters Response

	MES_STATAUS_AOI_READY_TO_LOAD_CMD       =  65,//AOI ReadyToLoad
	MES_STATAUS_AOI_READY_TO_LOAD_RES       =  66,//AOI ReadyToLoad Response

	MES_STATAUS_AOI_LOAd_COMPLETE_CMD       =  67,//AOI Load Complete 
	MES_STATAUS_AOI_LOAd_COMPLETE_RES       =  68,//AOI Load Complete  Response

	MES_STATAUS_AOI_START_INSPECTION_CMD    =  69,//AOI Start Inspection
	MES_STATAUS_AOI_START_INSPECTION_RES    =  70,//AOI Start Inspection Response

	MES_STATAUS_AOI_INSPECTION_COMPLETE_CMD =  71,//AOI Inspection Complete
	MES_STATAUS_AOI_INSPECTION_COMPLETE_RES =  72,//AOI Inspection Complete Response

	MES_STATAUS_AOI_READY_TO_UNLOAD_CMD     =  73,//AOI ReadyToUnload
	MES_STATAUS_AOI_READY_TO_UNLOAD_RES     =  74,//AOI ReadyToUnload Response

	MES_STATAUS_AOI_UNLOAd_COMPLETE_CMD     =  75,//AOI Unload Complete 
	MES_STATAUS_AOI_UNLOAd_COMPLETE_RES     =  76,//AOI Unload Complete  Response	

	MES_STATAUS_AOI_LOGIN_OUT_CMD           =  81,//AOI Login/out Parameters
	MES_STATAUS_AOI_LOGIN_OUT_RES           =  82,//AOI Login/out Parameters Response

	MES_STATAUS_AOI_CHECK_BARCODE_CMD       =  83,//AOI Check Barcode
	MES_STATAUS_AOI_CHECK_BARCODE_RES       =  84,//AOI Check Barcode Response

	MES_STATAUS_AOI_INSPECTION_STOP_CMD     =  85,//AOI Inspection Stop
	MES_STATAUS_AOI_INSPECTION_STOP_RES     =  86,//AOI Inspection Stop Response
	
	MES_STATAUS_AOI_PARAM_CHANGE_CMD        =  87,//AOI Param Changed
	MES_STATAUS_AOI_PROJECT_OPEN            =  88,//AOI Project Open
	MES_STATAUS_AOI_PROJECT_LOAD_FINISH     =  89,//AOI Project Load Finish
	MES_STATAUS_AOI_ALARM_CLEAR             =  90,//AOI Alarm Clear	
	MES_STATAUS_AOI_EDIT_MODE               =  91,//AOI Edit Mode
	MES_STATAUS_AOI_ONLINE_TEST             =  92,//AOI Online Test

	MES_STATAUS_VRS_UPLOAD_SFC_CMD          =  160,//VRS Upload SFC
	MES_STATAUS_VRS_UPLOAD_SFC_RES          =  161,//VRS Upload SFC Response

	MES_STATAUS_RETURN
};
//-------------------------------------------------------------------------------------//
enum MES_CMD_ID
{
	MES_CMD_TO_STOP        = 0,//停止運轉
	MES_CMD_TO_RUN         = 1,//開始運轉	
	MES_CMD_TO_BYPASS      = 2,//開始流片
	MES_CMD_RETURN
};
//-------------------------------------------------------------------------------------//
enum CALC_FOCUS_MODE//計算焦距模式
{
	CALC_FOCUS_AMPLITUDE                =    1,//平均相對平均差異[|g(x,y)-u|/N]
	CALC_FOCUS_VARIANCE                 =    2,//平均相對平均差異平方[(g(x,y)-u)^2/(N*N)]
	CALC_FOCUS_SUM_MODULES_DIFFERENCE   =    3,//相對鄰近差值[|g(x,y)-g(x+1,y)|+|g(x,y)-g(x,y+1)|]
	CALC_FOCUS_SQUARED_GRADIENT         =    4,//垂直差異平方和[g(x,y+1)-g(x,y)]^2
	CALC_FOCUS_TENENGRAD                =   11,//先Sobel後再計算
	CALC_FOCUS_LAPLACIAN                =   51,//先Laplacian後再計算
	CALC_FOCUS_PIXEL_DIFFERENCE         =  101,//相對平均值比較
	CALC_FOCUS_RETURN
};
//-------------------------------------------------------------------------------------//
enum CAD_FILE_CONTENT_MODE
{
	CAD_FILE_CONTENT_NORMAL          = 1,//一般
	CAD_FILE_CONTENT_FIDUCIAL        = 2,//帶有定位點
	CAD_FILE_CONTENT_RETURN
};
//-------------------------------------------------------------------------------------//
//模組遮罩
#define MODEL_MASK_NONE               0x00000000
#define MODEL_MASK_PAD                0x00000001//焊盤遮罩
#define MODEL_MASK_BODY               0x00000010//本體遮罩
#define MODEL_MASK_BODY_NO_LEAD       0x00000020//本體去腳遮罩
#define MODEL_MASK_LEAD               0x00000100//引腳電極遮罩
#define MODEL_MASK_LEAD_TIP           0x00000200//引腳前端遮罩
#define MODEL_MASK_LEAD_SHOULDER      0x00000400//引腳根部遮罩
//-------------------------------------------------------------------------------------//
#define  MAX_WND_DEFECT_ID_COUNT      36//最多檢測框瑕疵編號數量
enum WND_DEFECT_ID
{
	WND_DEFECT_NONE              =     0,//無定義	

	WND_DEFECT_PAD_ALIGN         =     1,//焊盤定位
	WND_DEFECT_PART_ALIGN		 =     2,//本體定位
	WND_DEFECT_PAD_ADJUST        =     3,//焊盤調整
	WND_DEFECT_LEAD_ADJUST       =     4,//管腳調整

	WND_DEFECT_CLASS_CHECK       =    98,//類別確認
	WND_DEFECT_BASE_VALUE        =    99,//基準數值

	WND_DEFECT_BODY_MISSING      =   101,//缺件
	WND_DEFECT_BODY_OFFSET       =   102,//偏移
	WND_DEFECT_BODY_TILT         =   103,//本體傾斜
	WND_DEFECT_BODY_POLARITY     =   104,//極反
	WND_DEFECT_BODY_TURNOVER     =   105,//反件
	WND_DEFECT_BODY_MOUNT        =   106,//錯件-裝貼
	WND_DEFECT_BODY_WRONG_CODE   =   107,//錯件-條碼
	WND_DEFECT_BODY_WRONG_TEXT   =   108,//錯件-文字
	WND_DEFECT_BODY_TOMBSTONE    =   109,//立碑
	WND_DEFECT_BODY_BILLBOARD    =   110,//側立
	WND_DEFECT_BODY_DAMAGED      =   111,//破損

	WND_DEFECT_SOLDER_POOR       =   201,//焊錫不足
	WND_DEFECT_SOLDER_OPEN       =   202,//焊錫空焊
	WND_DEFECT_SOLDER_PAD_EXPOSED=   203,//焊錫沒有-漏銅
	WND_DEFECT_SOLDER_BRIDGE     =   204,//焊錫短路
	WND_DEFECT_SOLDER_BEAD       =   205,//焊錫錫珠
	WND_DEFECT_SOLDER_EXCESS     =   206,//焊錫過量

	WND_DEFECT_LEAD_LIFTED       =   301,//引腳翹起
	WND_DEFECT_LEAD_BENDED       =   302,//引腳彎曲
	WND_DEFECT_LEAD_PROTRUDED    =   303,//引腳凸出

	WND_DEFECT_PAD_SCRATCH       =   401,//焊盤刮傷
	WND_DEFECT_FOREIGN_BODY      =   402,//異物

	WND_DEFECT_USER_DEFINE_01    =   501,//自定義-01
	WND_DEFECT_USER_DEFINE_02    =   502,//自定義-02
	WND_DEFECT_USER_DEFINE_03    =   503,//自定義-03
	WND_DEFECT_USER_DEFINE_04    =   504,//自定義-04
	WND_DEFECT_USER_DEFINE_05    =   505,//自定義-05
	WND_DEFECT_USER_DEFINE_06    =   506,//自定義-06
	WND_DEFECT_USER_DEFINE_07    =   507,//自定義-07
	WND_DEFECT_USER_DEFINE_08    =   508,//自定義-08	
	WND_DEFECT_USER_DEFINE_09    =   509,//自定義-09
	WND_DEFECT_USER_DEFINE_10    =   510,//自定義-10	

	WND_DEFECT_RETURN                    //最末一碼 
};
//-------------------------------------------------------------------------------------//
enum ALARM_LOCK_MODE //警報鎖住模式
{
	ALARM_LOCK_NONE    = 0,//不鎖住-無使用
	ALARM_LOCK_AOI     = 1,//鎖住機台
	ALARM_LOCK_ARS     = 2,//鎖住維修站
	ALARM_LOCK_RETURN      //最末一碼 
};
//-------------------------------------------------------------------------------------//
enum DEFECT_FROM_MODE //瑕疵判定來源
{
	DEFECT_FROM_NONE    = 0,
	DEFECT_FROM_AOI,
	DEFECT_FROM_ARS,	
	DEFECT_FROM_RETURN      //最末一碼 
};
//-------------------------------------------------------------------------------------//
enum TOP10_SCOPE  //前十大不良資料範疇
{
	TOP10_SCOPE_NONE=0,
	TOP10_SCOPE_MODEL,
	TOP10_SCOPE_PART_NUMBER,
	TOP10_SCOPE_COMPONENT,
	TOP10_SCOPE_RETURN   //最末一碼 
};
//-------------------------------------------------------------------------------------//
enum YIELDING_SCOPE //良率資料範疇
{
	YIELDING_SCOPE_NONE=0,
	YIELDING_SCOPE_TEST,
	YIELDING_SCOPE_PANEL,
	YIELDING_SCOPE_BOARD,
	YIELDING_SCOPE_COMPONENT,
	YIELDING_SCOPE_RETURN   //最末一碼 
};
//-------------------------------------------------------------------------------------//
enum DEFECT_PARAM_FROM_MODE//瑕疵參數來源模式
{
	DEFECT_PARAM_FROM_DISABLE    =  0,//關閉
	DEFECT_PARAM_FROM_PROJECT    =  1,//專案參數
	DEFECT_PARAM_FROM_COMPONENT  =  2,//零件設定
	DEFECT_PARAM_FROM_RETURN
};
//-------------------------------------------------------------------------------------//
enum CPK_FROM_MODE//Cpk來源模式
{
	CPK_FROM_OFFSET_X   = 1,//偏移量-X
	CPK_FROM_OFFSET_Y   = 2,//偏移量-Y
	CPK_FROM_SKEW_ANGLE = 3,//偏移角度
	CPK_FROM_RETURN
};
//-------------------------------------------------------------------------------------//
enum SWITCH_MODEL_ITEM_MODE//切換個模組項目模式
{
	SWITCH_MODEL_ITEM_ANY,
	SWITCH_MODEL_ITEM_SET,
	SWITCH_MODEL_ITEM_UNSET,
	SWITCH_MODEL_ITEM_RETURN
};
//-------------------------------------------------------------------------------------//
enum JET_MATCH_LIB_TYPE
{
	JET_MATCH_LIB_NONE = 0,
	JET_MATCH_LIB_EVS  = 1,
	JET_MATCH_LIB_MIM  = 2,
	JET_MATCH_LIB_RETURN  //最末一碼 
};
//-------------------------------------------------------------------------------------//
#define DATA_NF_DISABLE        0//關閉過濾
#define DATA_NF_AVERAGE        1//平均過濾
#define DATA_NF_MEDIAN         2//中值過濾
#define DATA_NF_3LEVEL         3//3分等過濾
#define DATA_NF_MEDIAN_2       4//中值過濾2
#define DATA_NF_PYRAMID_MEDIAN 5//金字塔中值濾波
#define DATA_NF_CONTENTAWARE   6//Content Aware
#define DATA_NF_FAST_MEDIAN    7//AVX-Median filter
#define DATA_NF_FAST_AVERAGE   8//AVX-Average filter
//-------------------------------------------------------------------------------------//
typedef struct tagBasePlaneParam
{	
	int                   BasePlaneIndex;//資料列表引數
	CString               BasePlaneInfoText;//訊息文字

	bool                  BasePlane2DMaskEnabled;//是否啟用
	unsigned int          BasePlane2DMaskFrameIndex;//影像序號
	unsigned int          BasePlane2DMaskFrameUniqueID;//影像唯一碼
	int                   BasePlane2DMaskGroupLinkIndex;//彩色過濾的連動編號

	BASE_PLANE_PROC_TYPE  BasePlaneProcType;//基準面程序樣式	
	CALC_BASE_PLANE_MODE  CalcBasePlaneMode;//計算基準面方式
	BASE_PLANE_TOWARD_MODE BasePlaneToward;//基準面朝向方式
	BASE_PLANE_EQUATION_MODE  BasePlaneEqMode;//基準面方程式模式, 平面, 曲面
	double                      BaePlaneAutRgnMaxGap;//基準面自動選區上限	
	BASE_PLANE_AUTO_REGION_MODE BasePlaneAutRgnMode;//基準面自動選區模式
	int                   BasePlaneXYPitch;//基準面XY計算步長
	DWORD                 UseSideMode;//使用位置的旗標
	bool                  UseInnerMode;//使用內部模式
	double                MaxTiltAngle;//最多傾斜角
	double                SystemNoiseRange;//系統雜訊範圍
	double                UpperRatio;//上範圍比例
	double                LowerRatio;//下範圍比例
	double                OffsetZ;//最後Z軸偏差量
	double                NormalX;//法向量-X
	double                NormalY;//法向量-Y
	double                NormalZ;//法向量-Z
	CJetGroundEquation    GroundEquation;//基準面方程式
	double                PanelBasePlane;//整板基準面
	double                PlaneRatioLSL;//滿足平面的比例下限
	double                PlaneRatioUSL;//滿足平面的比例上限
	double                RangeRatioMin;//範圍比例低平面
	double                RangeRatioMax;//範圍比例高平面
	CJetGroundEquation    LocalGroundEquation;//局部基準面方程式

	int                   FilterMode;//濾波器模式
	int                   FilterPitch;//濾波器間距
	int                   FilterKerSize;//濾波器遮罩尺寸
	int                   FilterUseSize;//濾波器使用尺寸
	int                   FilterIterCount;//濾波器疊代次數

	double                OverHighFilter;//過高剔除
	double                OverLowFilter;//過低剔除
	double                SpentTime;//花費時間

	bool                  RotatedClip;//斜角度切除
	double                RotatedAngle;//旋轉角度
	int                   RotatedSizeW;//旋轉後寬度
	int                   RotatedSizeH;//旋轉後長度

	int                   FilterMode2D;//濾波器模式-2D
	int                   FilterKerSize2D;//濾波器遮罩尺寸-2D

	RECT                  BodyRect;//本體區域
	int                   BodyOutsideW;//本體外圍寬度-um
	int                   BodyOutsideH;//本體外圍長度-um
	int                   BodyOutsideWPxl;//本體外圍寬度-pxl
	int                   BodyOutsideHPxl;//本體外圍長度-pxl
	BASE_PLANE_BODY_OUTSIDE_MODE BodyOutsideMode;//本體外圍模式

	tagBasePlaneParam()
	{
		BasePlaneIndex = -1;
		BasePlaneInfoText = _T("");

		BasePlane2DMaskEnabled = false;
		BasePlane2DMaskFrameIndex = 0;
		BasePlane2DMaskFrameUniqueID = 0;
		BasePlane2DMaskGroupLinkIndex = 0;

		BasePlaneProcType =  BASE_PLANE_PROC_TYPE_1;
		CalcBasePlaneMode =  CALC_BASE_PLANE_AUTO_LOWER;//CALC_BASE_PLANE_SURROUND;//CALC_BASE_PLANE_CORNER_ONLY, CALC_BASE_PLANE_AUTO_LOWER
		BasePlaneToward   =  BASE_PLANE_TOWARD_ANY;
		BasePlaneEqMode   =  BASE_PLANE_EQUATION_PLANE;
		BaePlaneAutRgnMaxGap=2000;
		BasePlaneAutRgnMode= BASE_PLANE_AUTO_REGION_LOWEST;
		BasePlaneXYPitch  =  1;
		UseSideMode       =  0xFF;
		UseInnerMode      =  false;
		MaxTiltAngle      =  4.0;//degree
		SystemNoiseRange  =  120;
		UpperRatio        =  1.0;
		LowerRatio        =  1.0;
		OffsetZ           =  0.0;
		NormalX           =  0.0;
		NormalY           =  0.0;
		NormalZ           =  0.0;
		PanelBasePlane    =  0.0;
		PlaneRatioLSL     =  0.0;
		PlaneRatioUSL     =  100.0;
		RangeRatioMin     =  0.0;
		RangeRatioMax     = 50.0;		

		FilterMode = DATA_NF_DISABLE;//濾波器模式
		FilterPitch = 1;//濾波器間距
		FilterKerSize = 3;//濾波器遮罩尺寸
		FilterUseSize = 0;//濾波器使用尺寸
		FilterIterCount = 1;//濾波器疊代次數

		OverHighFilter = 40000;//20 mm
		OverLowFilter = -5000;//-5 mm
		SpentTime = 0.0;//花費時間

		RotatedClip = false;
		RotatedAngle = 0.0;//旋轉角度
		RotatedSizeW = 0;//旋轉後寬度
		RotatedSizeH = 0;//旋轉後長度

		FilterMode2D = NOISE_FILTER_CLOSE;//濾波器模式-2D
		FilterKerSize2D = 9;//濾波器遮罩尺寸-2D

		BodyOutsideMode = BASE_PLANE_BODY_OUTSIDE_DISABLE;
		BodyOutsideW = 150;
		BodyOutsideH = 150;
		BodyOutsideWPxl = 20;
		BodyOutsideHPxl = 20;
		BodyRect.left = BodyRect.right = 0;
		BodyRect.top = BodyRect.bottom = 0;
	}

	void InitialBasePlaneParam()
	{
		NormalX           =  0.0;
		NormalY           =  0.0;
		NormalZ           =  0.0;
		GroundEquation.ClearGroundParam();
		LocalGroundEquation.ClearGroundParam();
	}

	void CopyBasePlaneGroundEquationFrom(const tagBasePlaneParam &BasePlaneParam)
	{
		NormalX = BasePlaneParam.NormalX;
		NormalY = BasePlaneParam.NormalY;
		NormalZ = BasePlaneParam.NormalZ;
		GroundEquation = BasePlaneParam.GroundEquation;
		BasePlaneEqMode = BasePlaneParam.BasePlaneEqMode;		
		LocalGroundEquation = BasePlaneParam.LocalGroundEquation;		
	}

	bool CheckUseBodyRect() const
	{
		if ( BASE_PLANE_BODY_OUTSIDE_DISABLE==BodyOutsideMode || 0==(BodyRect.right-BodyRect.left) || 0==(BodyRect.bottom-BodyRect.top) )
		{	return false; }
		return true;
	}
	void SetBasePlaneBodyRect(const RECT &Rect, int dX=0, int dY=0)
	{
		BodyRect = Rect;		
		InflateRect(&BodyRect, dX, dY);
	}

} TBasePlaneParam, *PBasePlaneParam;
//-------------------------------------------------------------------------------------//
typedef struct tagNoiseFilterParam
{
	//for Level Plane - 基準面用
	TBasePlaneParam       BasePlaneParam;//計算基準面方式		

	//for Space Data - 空間資料用
	int                   DataFilterIndex;//資料列表引數
	CString               DataFilterInfoText;

	HEIGHT_DATA_CORRECT_MODE DataCorrectMode;//資料修正模式

	int                   DataVoidExpandSize;//無效點外擴尺寸	
	bool                  DataVoidExpandEnabled;//無效點外擴啟用
	int                   DataVoidExpandIterCount;//無效點外擴疊代次數
	double                DataVoidExpandSpentTime;//花費時間		

	int                   DataFirstFilterMode;//首次濾波器啟用
	int                   DataFirstFilterPitch;//首次濾波器步進
	int                   DataFirstFilterKerSize;//首次濾波器尺寸
	int                   DataFirstFilterUseSize;//首次濾波器使用尺寸	
	int					  DataFirstFilterAlphaF;//Content Aware濾波器尋找相似高度之標準差
	int					  DataFirstFilterAlphaS;//Content Aware濾波器("鄰域"與"相似"高度)之標準差
	int					  DataFirstFilterAlphaM;//Content Aware濾波器("鄰域"與"中間"高度)之標準差
	int					  DataFirstFilterAlphaI;//Content Aware濾波器挑選影像(Gray)相似之標準差
	int					  DataFirstFilterThresdhold_Outlier;//Content Aware濾波器Outlier的閥值(0 - 1)
	bool				  DataFirstFilterSearchOn;//Content Aware濾波器尋找相似高度之功能,關閉後可加速運算(差異不大)
	double                DataFirstFilterSpentTime;//花費時間
	
	int                   DataOverLowFTMode;//過低濾除模式
	double                DataOverLowFTRange;//過低濾除範圍
	double                DataOverLowFTLimit;//過低濾除極限
	int                   DataOverLowFTKerSize;//高度濾波器尺寸	
	int                   DataOverLowFTUseSize;//高度濾波器使用尺寸	
	double                DataOverLowSpentTime;//花費時間

	int                   DataHeightFTMode;//高度異常濾除模式
	double                DataHeightFTRange;//高度異常濾除範圍	
	double                DataHeightFTSpentTime;//花費時間
	int                   DataHeightFTPitch;//高度異常確認器步進
	int                   DataHeightFTChkSize;//高度異常確認器尺寸	
	int                   DataHeightFTKerSize;//高度異常濾波器尺寸	
	int                   DataHeightFTUseSize;//高度異常濾波器使用尺寸	
	int                   DataHeightFTRepeatCnt;//高度異常濾波器重複次數

	bool                  DataVoidReContructed;//雜訊資料重建
	int                   DataVoidReContructedExtSize;//雜訊資料外擴尺寸
	double                DataVoidRecontructedSpentTime;//花費時間
	
	int                   DataFinalFilterMode;//最終濾波器啟用
	int                   DataFinalFilterPitch;//最終濾波器步進
	int                   DataFinalFilterKerSize;//最終濾波器尺寸
	int                   DataFinalFilterUseSize;//最終濾波器使用尺寸
	int					  DataFinalFilterAlphaF;//Content Aware濾波器尋找相似高度之標準差
	int					  DataFinalFilterAlphaS;//Content Aware濾波器("鄰域"與"相似"高度)之標準差
	int					  DataFinalFilterAlphaM;//Content Aware濾波器("鄰域"與"中間"高度)之標準差
	int					  DataFinalFilterAlphaI;//Content Aware濾波器挑選影像(Gray)相似之標準差
	int					  DataFinalFilterThresdhold_Outlier;//Content Aware濾波器Outlier的閥值(0 - 1)
	bool				  DataFinalFilterSearchOn;//Content Aware濾波器尋找相似高度之功能,關閉後可加速運算(差異不大)
	double                DataFinalFilterSpentTime;//花費時間

	int                   DataFinalFilterMode2;//最終濾波器啟用-2
	int                   DataFinalFilterPitch2;//最終濾波器步進-2
	int                   DataFinalFilterKerSize2;//最終濾波器尺寸-2
	int                   DataFinalFilterUseSize2;//最終濾波器使用尺寸-2
	int					  DataFinalFilterAlphaF2;//Content Aware濾波器尋找相似高度之標準差
	int					  DataFinalFilterAlphaS2;//Content Aware濾波器("鄰域"與"相似"高度)之標準差
	int					  DataFinalFilterAlphaM2;//Content Aware濾波器("鄰域"與"中間"高度)之標準差
	int					  DataFinalFilterAlphaI2;//Content Aware濾波器挑選影像(Gray)相似之標準差
	int					  DataFinalFilterThresdhold_Outlier2;//Content Aware濾波器Outlier的閥值(0 - 1)
	bool				  DataFinalFilterSearchOn2;//Content Aware濾波器尋找相似高度之功能,關閉後可加速運算(差異不大)
	double                DataFinalFilterSpentTime2;//花費時間

	tagNoiseFilterParam()
	{	
		BasePlaneParam = TBasePlaneParam();			

		DataFilterIndex = -1;
		DataFilterInfoText = _T("");

		DataCorrectMode = HEIGHT_DATA_CORRECT_NONE;

		DataVoidExpandSize = 10;//雜訊遮罩外擴尺寸
		DataVoidExpandEnabled = true;//雜訊遮罩外擴啟用
		DataVoidExpandIterCount = 1;
		DataVoidExpandSpentTime = 0.0;//花費時間		

		DataFirstFilterMode=DATA_NF_MEDIAN;//首次濾波器濾波器啟用
		DataFirstFilterPitch=2;//首次濾波器步進
		DataFirstFilterKerSize=9;//首次濾波器尺寸
		DataFirstFilterUseSize=1;//首次濾波器使用尺寸		
		DataFirstFilterAlphaF=450;//Content Aware濾波器尋找相似高度之標準差
		DataFirstFilterAlphaS=200;//Content Aware濾波器("鄰域"與"相似"高度)之標準差
		DataFirstFilterAlphaM=200;//Content Aware濾波器("鄰域"與"中間"高度)之標準差
		DataFirstFilterAlphaI=18;//Content Aware濾波器挑選影像(Gray)相似之標準差
		DataFirstFilterThresdhold_Outlier=1;//Content Aware濾波器Outlier的閥值(0 - 100)
		DataFirstFilterSearchOn=true;//Content Aware濾波器尋找相似高度之功能,關閉後可加速運算(差異不大)
		DataFirstFilterSpentTime=0;//花費時間

		DataOverLowFTMode = DATA_NF_MEDIAN;//過低濾除啟用
		DataOverLowFTRange = -200;//過低濾除範圍
		DataOverLowFTLimit = -500;//過低濾除極限
		DataOverLowFTKerSize = 13;//過低濾波器尺寸	
		DataOverLowFTUseSize = 1;//過低濾波器使用尺寸	
		DataOverLowSpentTime = 0.0;//花費時間

		DataHeightFTMode = DATA_NF_AVERAGE;//高度濾除模式
		DataHeightFTRange = 200;//高度濾除範圍		
		DataHeightFTSpentTime = 0.0;//花費時間		
		DataHeightFTPitch = 4;
		DataHeightFTChkSize   = 5;
		DataHeightFTKerSize   = 5;		
		DataHeightFTUseSize   = 1;
		DataHeightFTRepeatCnt = 1;

		DataVoidReContructed = true;//雜訊資料重建
		DataVoidReContructedExtSize = 5;//雜訊資料外擴尺寸
		DataVoidRecontructedSpentTime = 0.0;//花費時間
		
		DataFinalFilterMode = DATA_NF_DISABLE;
		DataFinalFilterPitch = 4;//平滑滑移步進
		DataFinalFilterKerSize = 3;
		DataFinalFilterUseSize = 0;
		DataFinalFilterAlphaF = 450;//Content Aware濾波器尋找相似高度之標準差
		DataFinalFilterAlphaS = 200;//Content Aware濾波器("鄰域"與"相似"高度)之標準差
		DataFinalFilterAlphaM = 200;//Content Aware濾波器("鄰域"與"中間"高度)之標準差
		DataFinalFilterAlphaI = 18;//Content Aware濾波器挑選影像(Gray)相似之標準差
		DataFinalFilterThresdhold_Outlier = 1;//Content Aware濾波器Outlier的閥值(0 - 100)
		DataFinalFilterSearchOn = true;//Content Aware濾波器尋找相似高度之功能,關閉後可加速運算(差異不大)
		DataFinalFilterSpentTime = 0.0;//花費時間

		DataFinalFilterMode2 = DATA_NF_DISABLE;//最終濾波器啟用-2
		DataFinalFilterPitch2 = 1;//最終濾波器步進-2
		DataFinalFilterKerSize2 = 0;//最終濾波器尺寸-2
		DataFinalFilterUseSize2 = 0;//最終濾波器使用尺寸-2
		DataFinalFilterAlphaF2 = 450;//Content Aware濾波器尋找相似高度之標準差
		DataFinalFilterAlphaS2 = 200;//Content Aware濾波器("鄰域"與"相似"高度)之標準差
		DataFinalFilterAlphaM2 = 200;//Content Aware濾波器("鄰域"與"中間"高度)之標準差
		DataFinalFilterAlphaI2 = 18;//Content Aware濾波器挑選影像(Gray)相似之標準差
		DataFinalFilterThresdhold_Outlier2 = 1;//Content Aware濾波器Outlier的閥值(0 - 100)
		DataFinalFilterSearchOn2 = true;//Content Aware濾波器尋找相似高度之功能,關閉後可加速運算(差異不大)
		DataFinalFilterSpentTime2 = 0.0;//花費時間
	}	
} TNoiseFilterParam, *PNoiseFilterParam;
//-------------------------------------------------------------------------------------//
typedef struct tagImageFilterParam
{		
	bool                  SkipValidPixel;//跳過有效值

	int                   FilterMode;//濾波器模式
	int                   FilterSize;//濾波器尺寸
	int                   FilterPitch;//濾波器步長
	int                   FilterShift;//濾波器起點	
	int                   FilterUseSize;//濾波器使用尺寸
	int                   FilterTolCount;//濾波器公差數量
	float                 FilterTolerance;//濾波器公差範圍	
	int                   FilterIterCount;//濾波器重複次數

#pragma region Content Aware Filter, Joe 20191118
	float				  FilterAlphaF;//Content Aware濾波器尋找相似高度之標準差
	float				  FilterAlphaS;//Content Aware濾波器("鄰域"與"相似"高度)之標準差
	float				  FilterAlphaM;//Content Aware濾波器("鄰域"與"中間"高度)之標準差
	float				  FilterAlphaI;//Content Aware濾波器挑選影像(Gray)相似之標準差
	float				  FilterThresdhold_Outlier;//Content Aware濾波器Outlier的閥值(0 - 1)
	bool				  FilterSearchOn;//Content Aware濾波器尋找相似高度之功能,關閉後可加速運算(差異不大)
#pragma endregion

	tagImageFilterParam()
	{
		SkipValidPixel = false;

		FilterMode = 0;//濾波器模式
		FilterSize = 3;//濾波器尺寸
		FilterPitch = 0;//濾波器步長
		FilterShift = 0;//濾波器起點	
		FilterUseSize = 0;//濾波器使用尺寸
		FilterTolCount = 4;//濾波器公差數量
		FilterTolerance = 50;//濾波器公差範圍	
		FilterIterCount = 1;//濾波器重複次數
		
		FilterAlphaF = 450;//Content Aware濾波器尋找相似高度之標準差
		FilterAlphaS = 200;//Content Aware濾波器("鄰域"與"相似"高度)之標準差
		FilterAlphaM = 200;//Content Aware濾波器("鄰域"與"中間"高度)之標準差
		FilterAlphaI = 18;//Content Aware濾波器挑選影像(Gray)相似之標準差
		FilterThresdhold_Outlier = 1;//Content Aware濾波器Outlier的閥值(0 - 100)
		FilterSearchOn = true;//Content Aware濾波器尋找相似高度之功能,關閉後可加速運算(差異不大)
	}

	int CalcFilterShift(int nStep, int nTotal)
	{
		FilterShift=0;
		if ( FilterPitch > nTotal )
		{	FilterShift = (nStep*FilterPitch/nTotal)%FilterPitch; }
		else
		{	FilterShift = nStep%FilterPitch;	}
		return FilterShift;
	}
} TImageFilterParam, *PImageFilterParam;
//-------------------------------------------------------------------------------------//
typedef struct tagDataModelParam//資料模型參數
{		
	int            PosX;//pixel
	int            PosY;//pixel
	int            SizeX;//pixel
	int            SizeY;//pixel
	int            DataLevel;//資料等級
	double         SizeZ;//um
	double         ShapeParam;
	double         ShapeParam2;
	BOX_SHAPE_MODE ShapeMode;//1:Rect, 2:Circle	

	tagDataModelParam()
	{	
		PosX = 0;
		PosY = 0;
		SizeX = 0;
		SizeY = 0;
		SizeZ = 0;
		DataLevel = 3;
		ShapeParam = 0;
		ShapeParam2 = 0;
		ShapeMode = BOX_SHAPE_RECTANGLE;		
	}
} TDataModelParam, *PDataModelParam;
//-------------------------------------------------------------------------------------//
enum MSG_MB_TYPE//訊息盒樣式
{
	MSG_MB_ABORTRETRYIGNORE = MB_ABORTRETRYIGNORE,
	MSG_MB_OK = MB_OK,	
	MSG_MB_OKCANCEL = MB_OKCANCEL,
	MSG_MB_RETRYCANCEL = MB_RETRYCANCEL,
	MSG_MB_YESNO = MB_YESNO,
	MSG_MB_YESNOCANCEL = MB_YESNOCANCEL	
};
enum MSG_MB_ICON_TYPE//訊息盒圖式樣式
{
	MSG_MB_NULL = 0x000000,
	MSG_MB_ICONEXCLAMATION = MB_ICONEXCLAMATION,
	MSG_MB_ICONINFORMATION = MB_ICONINFORMATION,	
	MSG_MB_ICONQUESTION = MB_ICONQUESTION,
	MSG_MB_ICONSTOP = MB_ICONSTOP	
};
enum MSG_MB_BTN//訊息盒按鈕樣式
{
	MSG_MB_DEFBUTTON1 = MB_DEFBUTTON1,
	MSG_MB_DEFBUTTON2 = MB_DEFBUTTON2,
	MSG_MB_DEFBUTTON3 = MB_DEFBUTTON3
};

const int MSG_FILTER_NONE     = 0x00000000;//未定義訊息
const int MSG_FILTER_SYSTEM   = 0x00000001;//系統訊息
const int MSG_FILTER_OPERATE  = 0x00000002;//操作訊息
const int MSG_FILTER_CAMERA   = 0x00000010;//相機訊息
const int MSG_FILTER_MOTION   = 0x00000020;//軸控訊息
const int MSG_FILTER_PLC      = 0x00000040;//PLC訊息
const int MSG_FILTER_LIGHT2D  = 0x00000100;//燈盤控制-2D
const int MSG_FILTER_LIGHT3D  = 0x00000200;//燈盤控制-3D

const int MSG_LEVEL_NONE    = 0;//訊息程度-未定義
const int MSG_LEVEL_HIGH    = 1;//訊息程度-高
const int MSG_LEVEL_MIDDLE  = 2;//訊息程度-中
const int MSG_LEVEL_LOW     = 3;//訊息程度-低
//-------------------------------------------------------------------------------------//
enum CURSOR_POS_MODE//鼠標位置模式
{
	CURSOR_POS_NONE            =  0,
	CURSOR_POS_INNER           =  1,
	CURSOR_POS_LEFT            =  2,
	CURSOR_POS_RIGHT           =  3,
	CURSOR_POS_TOP             =  4,
	CURSOR_POS_BOTTOM          =  5,
	CURSOR_POS_LEFT_TOP        =  6,
	CURSOR_POS_LEFT_BOTTOM     =  7,
	CURSOR_POS_RIGHT_TOP       =  8,
	CURSOR_POS_RIGHT_BOTTOM    =  9,
	CURSOR_POS_HAND            = 10
};
//-------------------------------------------------------------------------------------//
enum PHASE_UNWRAP_MODE//相位還原模式
{
	PHASE_UNWRAP_DISABLE   = 0,
	PHASE_UNWRAP_PHASE     = 1,
	PHASE_UNWRAP_GOLDSTEIN = 2
};
//-------------------------------------------------------------------------------------//
enum XBOARD_MAPPING_FILE_MODE//報廢板映射檔案模式
{
	XBOARD_MAPPING_FILE_DISABLE              =  0,//不使用
	XBOARD_MAPPING_FILE_MES_COMM             =  1,//MES通訊	
	XBOARD_MAPPING_FILE_RETURN
};
//-------------------------------------------------------------------------------------//
enum XBOARD_MAPPING_FILE_FLOW//報廢板映射順序模式
{
	XBOARD_MAPPING_FILE_FLOW_DEFAULT = 0,//預設
	XBOARD_MAPPING_FILE_FLOW_AFTER_BARCODE = 1,//在BARCODE檢查後運行
	XBOARD_MAPPING_RUN_RETURN
};
//-------------------------------------------------------------------------------------//
#define FOV_REGION_MARGIN_INNER     2000//視野範圍外部預留範圍-4//1mm
#define FOV_REGION_MARGIN_OUTER     1000//視野範圍外部預留範圍-2//1mm
//-------------------------------------------------------------------------------------//
#define FOV_RESOLUTION_020         20//2.0um
#define FOV_RESOLUTION_025         25//2.5um
#define FOV_RESOLUTION_030         30//3.0um
#define FOV_RESOLUTION_050         50//5.0um
#define FOV_RESOLUTION_055         55//5.5um
#define FOV_RESOLUTION_060         60//6.0um
#define FOV_RESOLUTION_065         65//6.5um
#define FOV_RESOLUTION_070         70//7.0um
#define FOV_RESOLUTION_072         72//7.2um
#define FOV_RESOLUTION_075         75//7.5um
#define FOV_RESOLUTION_078         78//7.8um
#define FOV_RESOLUTION_080         80//8.0um
#define FOV_RESOLUTION_100        100//10.0um
#define FOV_RESOLUTION_110        110//11.0um
#define FOV_RESOLUTION_120        120//12.0um
#define FOV_RESOLUTION_130        130//13.0um
#define FOV_RESOLUTION_150        150//15.0um
#define FOV_RESOLUTION_160        160//16.0um
#define FOV_RESOLUTION_180        180//18.0um
#define FOV_RESOLUTION_200        200//20.0um
//-------------------------------------------------------------------------------------//
#define DBL_PRECISION              0.0001//倍精度判定數值 
//-------------------------------------------------------------------------------------//
typedef struct _POINT2F
{
	float x,y;			
	_POINT2F(float fx=0, float fy=0):x(fx),y(fy)
	{	}
	_POINT2F(const POINT &pt):x(pt.x),y(pt.y)
	{}
} TPOINT2F, *PPOINT2F;

typedef struct _POINT2D
{
	double x,y;			
	_POINT2D(double fx=0, double fy=0):x(fx),y(fy)
	{	}
	_POINT2D(const POINT &pt):x(pt.x),y(pt.y)
	{}
	_POINT2D(const _POINT2F &pt):x(pt.x),y(pt.y)
	{}
} TPOINT2D, *PPOINT2D;

typedef struct _LINE2D//ax+by=c
{
	double a,b,c;
	_LINE2D(double a=0, double b=0, double c=0):a(a),b(b),c(c)
	{	}
} TLINE2D, *PLINE2D;

typedef struct _POINT3I
{
	int x,y,z;			
	_POINT3I():x(0),y(0),z(0)
	{	}
	_POINT3I(const POINT &pt):x(pt.x),y(pt.y),z(0)
	{	}
} TPOINT3I, *PPOINT3I;

typedef struct _POINT3F
{
	float x,y,z;			
	_POINT3F():x(0.0f),y(0.0f),z(0.0f)
	{	}
	_POINT3F(const _POINT2F &pt):x(pt.x),y(pt.y),z(0.0f)
	{	}
} TPOINT3F, *PPOINT3F;

typedef struct _POINT3D
{
	double x,y,z;			
	_POINT3D(double X=0, double Y=0, double Z=0):x(X),y(Y),z(Z)
	{	}	
	_POINT3D(const _POINT2D &pt):x(pt.x),y(pt.y),z(0.0f)
	{	}
} TPOINT3D, *PPOINT3D;

typedef struct _POINT4D
{
	double x,y,u,v;			
	_POINT4D():x(0.0),y(0.0),u(0.0),v(0.0)
	{	}
	_POINT4D(const _POINT2D &pt):x(pt.x),y(pt.y),u(0.0),v(0.0)
	{	}
	_POINT4D(const _POINT3D &pt):x(pt.x),y(pt.y),u(pt.z),v(0.0)
	{	}
} TPOINT4D, *PPOINT4D;


typedef struct _RECT4UI
{
	unsigned int left,top,right,bottom;
	_RECT4UI():left(0),top(0),right(0),bottom(0)
	{	}
	_RECT4UI(const RECT &rect):left(rect.left),top(rect.top),right(rect.right),bottom(rect.bottom)
	{	}
} TRECT4UI, *PRECT4UI;

typedef struct _RECT4D
{
	double left,top,right,bottom;
	_RECT4D():left(0),top(0),right(0),bottom(0)
	{	}
	_RECT4D(const RECT &rect):left(rect.left),top(rect.top),right(rect.right),bottom(rect.bottom)
	{	}
	double GetCpX() const { return (left+right)*0.5; }
	double GetCpY() const { return (top+bottom)*0.5; }
	double GetWidth() const { return (right-left); }
	double GetHeight() const { return (bottom-top); }
	double GetSizeX() const { return (right-left); }
	double GetSizeY() const { return (bottom-top); }
	void   Limit(bool bInvert)
	{
		if ( false == bInvert )
		{
			top = left = -DBL_MAX;
			bottom = right =  DBL_MAX;
		}
		else
		{
			top = left =  DBL_MAX;
			bottom = right = -DBL_MAX;
		}
	}	
	void SetRect(_POINT2D Pt[], size_t Count)
	{
		if ( 0 == Count ) { return; }
		left = right = Pt[0].x;
		top = bottom = Pt[0].y;
		for ( size_t i=1; i<Count; i++ )
		{
			if ( left > Pt[i].x ) { left = Pt[i].x; }
			if ( top > Pt[i].y ) { top = Pt[i].y; }
			if ( right < Pt[i].x ) { right = Pt[i].x; }
			if ( bottom < Pt[i].y ) { bottom = Pt[i].y; }			
		}
	}
	void SetRect(_POINT3D Pt[], size_t Count)
	{
		if ( 0 == Count ) { return; }
		left = right = Pt[0].x;
		top = bottom = Pt[0].y;
		for ( size_t i=1; i<Count; i++ )
		{
			if ( left > Pt[i].x ) { left = Pt[i].x; }
			if ( top > Pt[i].y ) { top = Pt[i].y; }
			if ( right < Pt[i].x ) { right = Pt[i].x; }
			if ( bottom < Pt[i].y ) { bottom = Pt[i].y; }			
		}
	}
	void SetRect(double X[], double Y[], size_t Count)
	{
		if ( 0 == Count ) { return; }
		left = right = X[0];
		top = bottom = Y[0];
		for ( size_t i=1; i<Count; i++ )
		{
			if ( left > X[i] ) { left = X[i]; }
			if ( top > Y[i] ) { top = Y[i]; }
			if ( right < X[i] ) { right = X[i]; }
			if ( bottom < Y[i] ) { bottom = Y[i]; }			
		}
	}
	void SetRect(const std::vector<POINT> &PtList)
	{
		const size_t Count=PtList.size();
		if ( 0 == Count ) { return; }
		left = right = PtList[0].x;
		top = bottom = PtList[0].y;
		for ( size_t i=1; i<Count; i++ )
		{
			if ( left > PtList[i].x ) { left = PtList[i].x; }
			if ( top > PtList[i].y ) { top = PtList[i].y; }
			if ( right < PtList[i].x ) { right = PtList[i].x; }
			if ( bottom < PtList[i].y ) { bottom = PtList[i].y; }			
		}
	}
	void SetRect(const std::vector<_POINT2D> &PtList)
	{
		const size_t Count=PtList.size();
		if ( 0 == Count ) { return; }
		left = right = PtList[0].x;
		top = bottom = PtList[0].y;
		for ( size_t i=1; i<Count; i++ )
		{
			if ( left > PtList[i].x ) { left = PtList[i].x; }
			if ( top > PtList[i].y ) { top = PtList[i].y; }
			if ( right < PtList[i].x ) { right = PtList[i].x; }
			if ( bottom < PtList[i].y ) { bottom = PtList[i].y; }			
		}
	}
	void SetRect(const std::vector<_POINT3D> &PtList)
	{
		const size_t Count=PtList.size();
		if ( 0 == Count ) { return; }
		left = right = PtList[0].x;
		top = bottom = PtList[0].y;
		for ( size_t i=1; i<Count; i++ )
		{
			if ( left > PtList[i].x ) { left = PtList[i].x; }
			if ( top > PtList[i].y ) { top = PtList[i].y; }
			if ( right < PtList[i].x ) { right = PtList[i].x; }
			if ( bottom < PtList[i].y ) { bottom = PtList[i].y; }			
		}
	}

	void   Move(double x, double y) 
	{    
		left   += x;
		top    += y;
		right  += x;
		bottom += y;
	}
	void   Move(const TPOINT2D &Pt) 
	{    
		left   += Pt.x;
		top    += Pt.y;
		right  += Pt.x;
		bottom += Pt.y;
	}
	void   Move(const TPOINT3D &Pt) 
	{    
		left   += Pt.x;
		top    += Pt.y;
		right  += Pt.x;
		bottom += Pt.y;
	}
	bool CheckPtInside(double x, double y)
	{
		if ( x < left ) { return false; }
		if ( y < top  ) { return false; }
		if ( x > right ) { return false; }
		if ( y > bottom  ) { return false; }
		return true;
	}
	bool CheckPtInside(const TPOINT2D &Pt)
	{
		if ( Pt.x < left ) { return false; }
		if ( Pt.y < top  ) { return false; }
		if ( Pt.x > right ) { return false; }
		if ( Pt.y > bottom  ) { return false; }
		return true;
	}
	bool CheckPtInside(const TPOINT3D &Pt)
	{
		if ( Pt.x < left ) { return false; }
		if ( Pt.y < top  ) { return false; }
		if ( Pt.x > right ) { return false; }
		if ( Pt.y > bottom  ) { return false; }
		return true;
	}
	void Expand(const POINT &Pt)
	{
		if ( left > Pt.x ) { left = Pt.x; }
		if ( top > Pt.y ) { top  = Pt.y; }
		if ( right < Pt.x ) { right = Pt.x; }
		if ( bottom < Pt.y ) { bottom = Pt.y; }
	}
	void Expand(const TPOINT2D &Pt)
	{
		if ( left > Pt.x ) { left = Pt.x; }
		if ( top > Pt.y ) { top  = Pt.y; }
		if ( right < Pt.x ) { right = Pt.x; }
		if ( bottom < Pt.y ) { bottom = Pt.y; }
	}
	void Expand(const _RECT4D &Rect)
	{
		if ( left > Rect.left ) { left = Rect.left; }
		if ( top > Rect.top ) { top = Rect.top; }
		if ( right < Rect.right ) { right = Rect.right; }
		if ( bottom < Rect.bottom ) { bottom = Rect.bottom; }
	}
} TRECT4D, *PRECT4D;

typedef struct _RECT6I
{
	int  left,top,right,bottom, maxZ, minZ;
	_RECT6I():left(0),top(0),right(0),bottom(0), maxZ(0), minZ(0)
	{	}
	_RECT6I(const RECT &rect):left(rect.left),top(rect.top),right(rect.right),bottom(rect.bottom), maxZ(0), minZ(0)
	{	}
	double GetCpX() const { return (left+right)*0.5; }
	double GetCpY() const { return (top+bottom)*0.5; }
	int GetWidth() const { return (right-left); }
	int GetHeight() const { return (bottom-top); }
	int GetSizeX() const { return (right-left); }
	int GetSizeY() const { return (bottom-top); }
	int GetMaxZ() const { return maxZ; }
	int GetMinZ() const { return minZ; }
	int GetRangeZ() const { return maxZ-minZ; }	
} TRECT6I, *PRECT6I;

typedef struct _SIZE2D
{
	double cx,cy;
	_SIZE2D(double x=0.0, double y=0.0):cx(x),cy(y)
	{	}
	void SetSize(double w, double h)
	{
		cx = w;
		cy = h;
	}
	void Expand(double w, double h)
	{
		if ( cx < w ) { cx = w; }
		if ( cy < h ) { cy = h; }
	}	
} TSIZE2D, *PSIZE2D;

typedef struct _SIZE3D
{
	double cx,cy,cz;
	_SIZE3D():cx(0.0),cy(0.0),cz(0.0)
	{	}
	_SIZE3D(const _SIZE2D &sz):cx(sz.cx),cy(sz.cy),cz(0.0)
	{	}
	void SetSize(double w, double l, double h)
	{
		cx = w;
		cy = l;
		cz = h;
	}
	void Expand(double w, double l, double h)
	{
		if ( cx < w ) { cx = w; }
		if ( cy < l ) { cy = l; }
		if ( cz < h ) { cz = h; }
	}
} TSIZE3D, *PSIZE3D;

typedef struct _REGION4D
{
	double minX,minY,maxX,maxY;
	_REGION4D():minX(0.0),minY(0.0),maxX(0.0),maxY(0.0)
	{	}
	_REGION4D(const RECT &rect):minX(rect.left),minY(rect.top),maxX(rect.right),maxY(rect.bottom)
	{	}
	_REGION4D(const TRECT4D &rect):minX(rect.left),minY(rect.top),maxX(rect.right),maxY(rect.bottom)
	{	}
	double GetCpX() const { return (minX+maxX)*0.5; }
	double GetCpY() const { return (minY+maxY)*0.5; }
	double GetWidth() const { return (maxX-minX); }
	double GetHeight() const { return (maxY-minY); }	
	double GetArea() const { return (maxX-minX)*(maxY-minY); }
	double GetSizeX() const { return (maxX-minX); }
	double GetSizeY() const { return (maxY-minY); }
	double GetSize() const { return (maxX-minX)*(maxY-minY); }
	void   Limit(bool bInvert)
	{
		if ( false == bInvert )
		{
			minY = minX = -DBL_MAX;
			maxY = maxX =  DBL_MAX;
		}
		else
		{
			minY = minX =  DBL_MAX;
			maxY = maxX = -DBL_MAX;
		}
	}
	void   Move(double x, double y) 
	{    
		minX += x;
		minY += y;
		maxX += x;
		maxY += y;
	}
	void   Move(const TPOINT2D &Pt) 
	{    
		minX += Pt.x;
		minY += Pt.y;
		maxX += Pt.x;
		maxY += Pt.y;
	}
	void   Move(const TPOINT3D &Pt) 
	{    
		minX += Pt.x;
		minY += Pt.y;
		maxX += Pt.x;
		maxY += Pt.y;
	}
	void   MoveTo(double x, double y) 
	{    
		double W2 = (maxX-minX)*0.5;
		double H2 = (maxY-minY)*0.5;
		minX = x-W2;
		minY = y-H2;
		maxX = x+W2;
		maxY = y+H2;
	}
	void   MoveToX(double x) 
	{    
		double W2 = (maxX-minX)*0.5;		
		minX = x-W2;		
		maxX = x+W2;		
	}
	void   MoveToY(double y) 
	{    
		double H2 = (maxY-minY)*0.5;		
		minY = y-H2;		
		maxY = y+H2;
	}
	void Modify(const _REGION4D &dRgn)
	{
		minX += dRgn.minX;
		minY += dRgn.minY;
		maxX += dRgn.maxX;
		maxY += dRgn.maxY;
	}
	bool CheckPtInside(double x, double y)
	{
		if ( x < minX ) { return false; }
		if ( y < minY ) { return false; }
		if ( x > maxX ) { return false; }
		if ( y > maxY ) { return false; }
		return true;
	}
	bool CheckPtInside(const TPOINT2D &Pt)
	{
		if ( Pt.x < minX ) { return false; }
		if ( Pt.y < minY ) { return false; }
		if ( Pt.x > maxX ) { return false; }
		if ( Pt.y > maxY ) { return false; }
		return true;
	}
	bool CheckPtInside(const TPOINT3D &Pt)
	{
		if ( Pt.x < minX ) { return false; }
		if ( Pt.y < minY  ) { return false; }
		if ( Pt.x > maxX ) { return false; }
		if ( Pt.y > maxY  ) { return false; }
		return true;
	}
	void SetSize(double W, double H)
	{
		double X=GetCpX();
		double Y=GetCpY();
		minX = X-(W*0.5);
		minY = Y-(H*0.5);
		maxX = X+(W*0.5);
		maxY = Y+(H*0.5);
	}
	void SetRgn(const RECT &Rect)
	{
		if ( Rect.left < Rect.right )
		{	minX = Rect.left;	maxX = Rect.right;	}
		else
		{	minX = Rect.right;	maxX = Rect.left;	}

		if ( Rect.top < Rect.bottom )
		{	minY = Rect.top;	maxY = Rect.bottom;	}
		else
		{	minY = Rect.bottom;	maxY = Rect.top;	}
	}
	void SetRgn(const TRECT4D &Rect)
	{
		if ( Rect.left < Rect.right )
		{	minX = Rect.left;	maxX = Rect.right;	}
		else
		{	minX = Rect.right;	maxX = Rect.left;	}

		if ( Rect.top < Rect.bottom )
		{	minY = Rect.top;	maxY = Rect.bottom;	}
		else
		{	minY = Rect.bottom;	maxY = Rect.top;	}
	}
	void SetRgn(double X, double Y, double W, double H)
	{
		minX = X-(W*0.5);
		minY = Y-(H*0.5);
		maxX = X+(W*0.5);
		maxY = Y+(H*0.5);
	}	
	void SetRgn(_POINT2D Pt[], size_t Count)
	{
		if ( 0 == Count ) { return; }
		minX = maxX = Pt[0].x;
		minY = maxY = Pt[0].y;
		for ( size_t i=1; i<Count; i++ )
		{
			if ( minX > Pt[i].x ) { minX = Pt[i].x; }
			if ( minY > Pt[i].y ) { minY = Pt[i].y; }
			if ( maxX < Pt[i].x ) { maxX = Pt[i].x; }
			if ( maxY < Pt[i].y ) { maxY = Pt[i].y; }			
		}
	}
	void SetRgn(_POINT3D Pt[], size_t Count)
	{
		if ( 0 == Count ) { return; }
		minX = maxX = Pt[0].x;
		minY = maxY = Pt[0].y;
		for ( size_t i=1; i<Count; i++ )
		{
			if ( minX > Pt[i].x ) { minX = Pt[i].x; }
			if ( minY > Pt[i].y ) { minY = Pt[i].y; }
			if ( maxX < Pt[i].x ) { maxX = Pt[i].x; }
			if ( maxY < Pt[i].y ) { maxY = Pt[i].y; }			
		}
	}
	void SetRgn(double X[], double Y[], size_t Count)
	{
		if ( 0 == Count ) { return; }
		minX = maxX = X[0];
		minY = maxY = Y[0];
		for ( size_t i=1; i<Count; i++ )
		{
			if ( minX > X[i] ) { minX = X[i]; }
			if ( minY > Y[i] ) { minY = Y[i]; }
			if ( maxX < X[i] ) { maxX = X[i]; }
			if ( maxY < Y[i] ) { maxY = Y[i]; }			
		}
	}
	void SetRgn(const std::vector<POINT> &PtList)
	{		
		const size_t Count = PtList.size();
		if ( 0 == Count ) { return; }
		minX = maxX = PtList[0].x;
		minY = maxY = PtList[0].y;
		for ( size_t i=1; i<Count; i++ )
		{
			if ( minX > PtList[i].x ) { minX = PtList[i].x; }
			if ( minY > PtList[i].y ) { minY = PtList[i].y; }
			if ( maxX < PtList[i].x ) { maxX = PtList[i].x; }
			if ( maxY < PtList[i].y ) { maxY = PtList[i].y; }			
		}
	}
	void SetRgn(const std::vector<_POINT2D> &PtList)
	{		
		const size_t Count = PtList.size();
		if ( 0 == Count ) { return; }
		minX = maxX = PtList[0].x;
		minY = maxY = PtList[0].y;
		for ( size_t i=1; i<Count; i++ )
		{
			if ( minX > PtList[i].x ) { minX = PtList[i].x; }
			if ( minY > PtList[i].y ) { minY = PtList[i].y; }
			if ( maxX < PtList[i].x ) { maxX = PtList[i].x; }
			if ( maxY < PtList[i].y ) { maxY = PtList[i].y; }			
		}
	}
	void SetRgn(const std::vector<_POINT3D> &PtList)
	{		
		const size_t Count = PtList.size();
		if ( 0 == Count ) { return; }
		minX = maxX = PtList[0].x;
		minY = maxY = PtList[0].y;
		for ( size_t i=1; i<Count; i++ )
		{
			if ( minX > PtList[i].x ) { minX = PtList[i].x; }
			if ( minY > PtList[i].y ) { minY = PtList[i].y; }
			if ( maxX < PtList[i].x ) { maxX = PtList[i].x; }
			if ( maxY < PtList[i].y ) { maxY = PtList[i].y; }			
		}
	}

	void Expand(const POINT &Pt)
	{
		if ( minX > Pt.x ) { minX = Pt.x; }
		if ( minY > Pt.y ) { minY = Pt.y; }
		if ( maxX < Pt.x ) { maxX = Pt.x; }
		if ( maxY < Pt.y ) { maxY = Pt.y; }
	}
	void Expand(const TPOINT2D &Pt)
	{
		if ( minX > Pt.x ) { minX = Pt.x; }
		if ( minY > Pt.y ) { minY = Pt.y; }
		if ( maxX < Pt.x ) { maxX = Pt.x; }
		if ( maxY < Pt.y ) { maxY = Pt.y; }
	}

	void Expand(const _REGION4D &Rgn)
	{
		if ( minX > Rgn.minX ) { minX = Rgn.minX; }
		if ( minY > Rgn.minY ) { minY = Rgn.minY; }
		if ( maxX < Rgn.maxX ) { maxX = Rgn.maxX; }
		if ( maxY < Rgn.maxY ) { maxY = Rgn.maxY; }
	}

	void Spin(double Angle)
	{
		double CpX = GetCpX();
		double CpY = GetCpY();
		double Width = GetWidth();
		double Height= GetHeight();

		double Err_000 = fabs(Angle-0.00);
		double Err_090 = fabs(Angle-090.00);
		double Err_180 = fabs(Angle-180.00);
		double Err_270 = fabs(Angle-270.00);
		double Err_360 = fabs(Angle-360.00);
		if ( Err_000<0.0001 || Err_360<0.0001 || Err_180<0.0001 )
		{	return ; }

		if ( Err_090<0.0001 || Err_270<0.0001 )
		{	
			SetRgn(CpX, CpY, Height, Width);
			return ; 
		}
		return;
	}

	void GetCornerPts(TPOINT2D CornerPts[4])
	{
		const double RgnCpx = GetCpX();
		const double RgnCpy = GetCpY();
		const double RgnW = GetWidth();
		const double RgnH = GetHeight();
		CornerPts[0].x = RgnCpx-(RgnW/2);
		CornerPts[0].y = RgnCpy-(RgnH/2);
		CornerPts[1].x = RgnCpx+(RgnW/2);
		CornerPts[1].y = RgnCpy-(RgnH/2);
		CornerPts[2].x = RgnCpx+(RgnW/2);
		CornerPts[2].y = RgnCpy+(RgnH/2);
		CornerPts[3].x = RgnCpx-(RgnW/2);
		CornerPts[3].y = RgnCpy+(RgnH/2);
	}
} TREGION4D, *PREGION4D;

typedef struct _REGION6D
{
	double minX,minY,maxX,maxY,minZ,maxZ;
	_REGION6D():minX(0.0),minY(0.0),minZ(0.0),maxX(0.0),maxY(0.0),maxZ(0.0)
	{	}
	_REGION6D(const TREGION4D& rgn):minX(rgn.minX),minY(rgn.minY),minZ(0.0),maxX(rgn.maxX),maxY(rgn.maxY),maxZ(0.0)
	{	}
	double GetCpX() const { return (minX+maxX)*0.5; }
	double GetCpY() const { return (minY+maxY)*0.5; }
	double GetCpZ() const { return (minZ+maxZ)*0.5; }
	double GetSizeX() const { return (maxX-minX); }
	double GetSizeY() const { return (maxY-minY); }
	double GetSizeZ() const { return (maxZ-minZ); }
} TREGION6D, *PREGION6D;

typedef struct _PIXEL_GRY
{
	int x,y;
	int gray;
	_PIXEL_GRY():x(0),y(0),gray(0)
	{	}		
} TPIXEL_GRY, *PPIXEL_GRY;

typedef struct _PIXEL_RGB
{
	int x,y;
	int r,g,b;
	_PIXEL_RGB():x(0),y(0),r(0),g(0),b(0)
	{	}		
} TPIXEL_RGB, *PPIXEL_RGB;
//-------------------------------------------------------------------------------------//
typedef struct _2D_CURRENT
{
	int    Current;
	float  Gray;
	float  Red;
	float  Green;
	float  Blue;
	float  Gain;
	_2D_CURRENT():Current(0),Gray(0),Red(0),Green(0),Blue(0),Gain(1)
	{	}
	_2D_CURRENT(int cur, float gray):Current(cur),Gray(gray),Red(0),Green(0),Blue(0),Gain(1)
	{	}
	_2D_CURRENT(int cur, float r, float g, float b):Current(cur),Gray(0),Red(r),Green(g),Blue(b),Gain(1)
	{	}
} T2D_CURRENT, *P2D_CURRENT;
//-------------------------------------------------------------------------------------//
typedef struct tagImageStat
{		
	int        m_Max;//最大值
	int        m_Min;//最小值
	double     m_Ave;//平均值
	double     m_Std;//標準差
	double     m_Cst;//對比度//(I80-I20)/(I80+I20)
	double     m_Ratio;//差值比例-外部使用	
	DWORD      m_State;//狀態

	RECT       m_Rect;
	tagImageStat()
	{
		m_Ave = 0;//平均值
		m_Max = 0;//最大值
		m_Min = 0;//最小值
		m_Std = 0;//標準差
		m_Cst = 0;//對比度//(I80-I20)/(I80+I20)		
		m_Ratio = 0;//差值比例-外部使用	
		m_State = OBJECT_STATE_NULL;
		m_Rect.left=m_Rect.right=m_Rect.top=m_Rect.bottom=0;
	}
} TImageStat, *PImageStat;
//-------------------------------------------------------------------------------------//
typedef struct tagResultCnt//結果次數
{	
	size_t OK;//OK次數
	size_t NG;//NG次數
	size_t None;//None次數
	size_t Skip;//Skip次數
	size_t Bypass;//Bypass次數
	size_t Exception;//Exception次數
	size_t Total;//全部
	tagResultCnt()
	{
		OK = 0;
		NG = 0;
		None = 0;
		Skip = 0;
		Bypass = 0;
		Exception = 0;
		Total = 0;
	}
	double CalcYielding() const
	{
		double Yielding=0.0;
		if ( 0 == Total ) { return Yielding; }
		Yielding = 100.0*(Total-NG)/Total;
		return Yielding;
	}

	void ResetCount(size_t Cnt=0)
	{
		OK = Cnt;
		NG = Cnt;
		None = Cnt;
		Skip = Cnt;
		Bypass = Cnt;
		Exception = Cnt;
		Total = Cnt;
	}

	void AddCount(RESULT_ID ResultID )
	{
		switch ( ResultID )
		{		
		case RESULT_ID_OK:     OK ++;	break;
		case RESULT_ID_NG:     NG ++;	break;
		case RESULT_ID_NONE:   None ++;	break;
		case RESULT_ID_SKIP:   Skip ++;	break;		
		case RESULT_ID_BYPASS: Bypass ++;	break;
		case RESULT_ID_EXCEPTION: Exception ++; break;
		}
		Total ++;
	}

	void AddCount(const tagResultCnt &Count )
	{
		OK += Count.OK;
		NG += Count.NG;
		None += Count.None;
		Skip += Count.Skip;
		Bypass += Count.Bypass;
		Exception += Count.Exception;
		Total += Count.Total;
	}

	RESULT_ID CheckResultID(RESULT_ID DefaultID=RESULT_ID_OK) const
	{
		if ( Exception > 0 ) { return RESULT_ID_EXCEPTION; }
		if ( NG > 0 ) { return RESULT_ID_NG; }
		if ( OK > 0 ) { return RESULT_ID_OK; }
		if ( Skip > 0 ) { return RESULT_ID_SKIP; }
		if ( Bypass > 0 ) { return RESULT_ID_BYPASS; }
		return DefaultID;//RESULT_ID_NONE
	}
} TResultCnt, *PResultCnt;
//-------------------------------------------------------------------------------------//
typedef struct tagTestTime
{
	double     tFd;//取定位點花費時間
	double     tGrab;//取像花費時間
	double     tCalc;//剩餘計算時間
	double     tTest;//檢測花費時間	
	double     tCycle;//檢測循環時間(檢測+進出板)
	tagTestTime()
	{
		tFd = 0;
		tGrab = 0;
		tCalc = 0;
		tTest = 0;
		tCycle = 0;
	}
} TTestTime, *PTestTime;
//-------------------------------------------------------------------------------------//
typedef struct tagSigmaTemp//標準差暫存用
{
	double            Max;//最大值
	double            Min;//最小值
	double            Sum;//總和
	double            SumSqrd;//平方總和
	int               Count;//數量
	tagSigmaTemp()
	{
		Max = 0;
		Min = 0;
		Sum = 0;
		SumSqrd = 0;
		Count = 0;
	}

	void AddValue(double val)
	{
		Sum += val;
		SumSqrd += (val)*(val);
		if ( 0 == Count )
		{	Max = Min = val;	}
		else
		{
			if ( Max < val ) { Max = val; }
			if ( Min > val ) { Min = val; }
		}
		Count ++;
	}

	int GetCount() const 
	{	return Count; }
	double GetMax() const 
	{	return Max; }
	double GetMin() const 
	{	return Min; }	
	double CalcMean() const
	{
		if ( 0 == Count ) { return 0; }
		return Sum/Count;
	}

	double CalcSigma() const
	{
		double Mean=Sum/Count;
		double Variance=(SumSqrd/Count)-(Mean*Mean);
		double Sigma=::sqrt(Variance);
		return Sigma;
	}
} TSigmaTemp, *PSigmaTemp;
//-------------------------------------------------------------------------------------//
typedef struct tagSigmaItem//標準差項目
{
	TSigmaTemp        OffsetX;
	TSigmaTemp        OffsetY;
	TSigmaTemp        SkewAngle;	
	TSigmaTemp        BodyHeight;	
} TSigmaItem, *PSigmaItem;
//-------------------------------------------------------------------------------------//
typedef struct tagGrrSigmaItem//GRR使用的SigmaItem
{
	unsigned int      Count;
	CString           Barcode;
	TSigmaItem        SigmaItem;
	tagGrrSigmaItem()
	{	Count = 0;	}
} TGrrSigmaItem, *PGrrSigmaItem;
//-------------------------------------------------------------------------------------//
typedef struct tagCpkTemp//Cpk暫存用
{
	float              Ave;      //平均值
	float              Min;      //最小值
	float              Max;      //最大值
	float              Sigma;    //標準差	
	float              USL;      //規格上限
	float              LSL;      //規格下限 
	float              Cpk;      //Cpk	
	float              Bin;      //Bin大小
	std::vector<float> BinList;  //Bin列表
	tagCpkTemp()
	{
		Ave = 0;
		Min = 0;
		Max = 0;
		Sigma = 0;
		USL = 0;
		LSL = 0;
		Cpk = 0;		
		Bin = 0.0f;		
	}
} TCpkTemp, *PCpkTemp;
//-------------------------------------------------------------------------------------//
typedef struct tagCpkItem//Cpk項目
{	
	CTime              DateTime;
	CString            Barcode;
	TEST_RESULT_ID     ResultID;
	TCpkTemp           OffsetX;
	TCpkTemp           OffsetY;
	TCpkTemp           SkewAngle;
	TCpkTemp           BodyHeight;
	tagCpkItem()
	{
		ResultID = TEST_RESULT_NONE;
	}
} TCpkItem, *PCpkItem;
//-------------------------------------------------------------------------------------//
typedef struct tagTestResult
{
	CTime           sDateTimeS;//檢測開始日期-Start
	CTime           sDateTimeE;//檢測開始日期-End
	TTestTime       sTestTime;//檢測花費時間
	TResultCnt      sTest;//檢測
	TResultCnt      sPanel;//整板
	TResultCnt      sBoard;//單板
	TResultCnt      sComponent;//零件
	LANE_ID         sLaneID;
	TEST_RESULT_ID  sResultID;
	tagTestResult()
	{	
		sDateTimeS = CTime::GetCurrentTime();
		sDateTimeE = sDateTimeS;
		sLaneID = LANE_ID_A;
		sResultID = TEST_RESULT_NONE;
	}
} TTestResult, *PTestResult;
//-------------------------------------------------------------------------------------//
typedef struct tagTop10Node
{
	CString    NodeName;//節點名稱
	size_t     StatisticsIndex;
	size_t     DefectCountAOI;//設備檢出瑕疵次數
	size_t     DefectCountARS;//人員判定瑕疵次數
	tagTop10Node()
	{
		NodeName = _T("");
		StatisticsIndex = -1;
		DefectCountAOI = 0;
		DefectCountARS = 0;
	}
} TTop10Node, *PTop10Node;
//-------------------------------------------------------------------------------------//
typedef struct tagAliasNode
{
	std::wstring  wsPartNumber; //料號名稱
	std::wstring  wsModelName;  //模組名稱	 
} TAliasNode, *PAliasNode;
//-------------------------------------------------------------------------------------//
typedef struct tagUserNode
{
	wchar_t          wUserName[128];//使用者名稱
	wchar_t          wPassword[128];//使用者名稱
	USER_LEVEL_MODE  eUserLevel;//使用者權限
	bool             wFingerEnable;
	BYTE             wFinger[1024];
	tagUserNode()
	{
		memset(wUserName, 0x00, sizeof(wUserName));
		memset(wPassword, 0x00, sizeof(wPassword));
		memset(wFinger,	  0x00, sizeof(wFinger));
		eUserLevel = USER_LEVEL_SIGN_OUT;		
		wFingerEnable = false;
	};
} TUserNode, *PUserNode;
//-------------------------------------------------------------------------------------//
typedef struct tagObjPos//物件位置
{
	UINT           ObjType;
	int            UniqueID;//唯一碼
	unsigned int   FdIndex;//定位點引數
	unsigned int   PanelIndex;//整板引數
	unsigned int   BoardIndex;//單板引數
	unsigned int   ModelIndex;//模組引數
	unsigned int   BarcodeIndex;//條碼引數
	unsigned int   ComponentIndex;//零件引數

	void          *FdPtr;
	void          *PanelPtr;
	void          *BoardPtr;	
	void          *ModelPtr;
	void          *BarcodePtr;
	void          *ComponentPtr;

	double         CadPosX;//Cad座標-X
	double         CadPosY;//Cad座標-Y
	double         CadPosZ;//Cad座標-Z
	double         StagePosX;//機台座標-X
	double         StagePosY;//機台座標-Y
	double         StagePosZ;//機台座標-Z

	double         BasePlane;//基準面
	RESULT_ID      ResultID;//結果編號
	tagObjPos()
	{
		ObjType  = NULL;
		UniqueID = -1;
		FdIndex  = -1;
		PanelIndex = -1;
		BoardIndex = -1;
		ModelIndex = -1;
		BarcodeIndex = -1;
		ComponentIndex = -1;

		FdPtr = NULL;
		PanelPtr = NULL;
		BoardPtr = NULL;	
		ModelPtr = NULL;
		BarcodePtr = NULL;
		ComponentPtr = NULL;

		CadPosX = 0.0;
		CadPosY = 0.0;
		CadPosZ = 0.0;
		StagePosX = 0.0;
		StagePosY = 0.0;
		StagePosZ = 0.0;

		BasePlane = 0.0;
		ResultID = RESULT_ID_NONE;
	}
} TObjPos, *PObjPos;
//-------------------------------------------------------------------------------------//
typedef struct tagBarcodeInfo
{
	void         *pPanel;
	void         *pBoard;
	unsigned int  nPanelIndex;
	unsigned int  nBoardIndex;
	std::wstring  wsBarcode;

	tagBarcodeInfo()
	{
		pPanel = NULL;
		pBoard = NULL;
		nPanelIndex = -1;
		nBoardIndex = -1;
		wsBarcode.clear();		
	}
} TBarcodeInfo, *PBarcodeInfo;
//-------------------------------------------------------------------------------------//
typedef struct tagBarcodeDevice
{
	int                    nDeviceID;
	CString                sDevicePort;//裝置連接埠
	BARCODE_DEVICE_TYPE    eDevieType;//裝置樣式
	tagBarcodeDevice()
	{
		nDeviceID = 0;
		sDevicePort = _T("1");
		eDevieType = BARCODE_DEVICE_NULL;
	}
} TBarcodeDevice, *PBarcodeDevice;
//-------------------------------------------------------------------------------------//
typedef struct tagProjectOpenCode
{
	std::wstring     wsOpenCode;
	std::wstring     wsProjectName;	
} TProjectOpenCode, *PProjectOpenCode;
//-------------------------------------------------------------------------------------//
typedef struct tagModelUpdateToGroupParam
{
	bool       bUpdateAll;
	bool       bUpdateParam;
	bool       bUpdateBinary;
	bool       bUpdateWndSize;
	bool       bUpdateAddOne;
	bool       bUpdateDelete;
	bool       bKeepSpec;
	tagModelUpdateToGroupParam()
	{
		bUpdateAll = false;
		bUpdateParam = true;
		bUpdateBinary = false;
		bUpdateWndSize = false;
		bUpdateAddOne = false;
		bUpdateDelete = false;
		bKeepSpec = false;
	}
} TModelUpdateToGroupParam, *PModelUpdateToGroupParam;
//-------------------------------------------------------------------------------------//
typedef struct tagDotNode
{
	int    nIndexX;
	int    nIndexY;

	double dPosX;//理論座標
	double dPosY;//理論座標
	double dAlignedX;//對齊座標-After Fd
	double dAlignedY;//對齊座標-After Fd
	double dCaliPosX;//修正座標
	double dCaliPosY;//修正座標
	double dOffsetX;//偏移值
	double dOffsetY;//偏移值

	RECT   rcDot;//圖像位置
	RESULT_ID eResultID;//結果狀態	
	BOX_SHAPE_MODE eShapeMode;//外形 

	tagDotNode()
	{
		nIndexX = -1;
		nIndexY = -1;
		dPosX = 0;
		dPosY = 0;
		dAlignedX = 0;
		dAlignedY = 0;
		dCaliPosX = 0;
		dCaliPosY = 0;
		dOffsetX = 0;
		dOffsetY = 0;

		rcDot.left = 0;
		rcDot.top = 0;
		rcDot.right = 0;
		rcDot.bottom = 0;

		eResultID=RESULT_ID_NONE;
		eShapeMode=BOX_SHAPE_ELLIPSE;
	};
} TDotNode, *PDotNode;
//-------------------------------------------------------------------------------------//
//相位高度校正
#define   MAX_HEIGHT_TARGET_COUNT      10//最多幾個高度塊規
#define   HEIGHT_FACTOR_PARAM_COUNT     8//高度係數參數數量
#define   HEIGHT_FACTOR_PHASE           0
#define   HEIGHT_FACTOR_PHASE_BASE      1
#define   HEIGHT_FACTOR_PHASE_TARGET    2
#define   HEIGHT_FACTOR_PHASE_OFFSET    3
//-------------------------------------------------------------------------------------//
typedef struct tagPhaseFactorGrid
{		
	int                m_TargetNo;
	RECT               m_RectLevel;
	RECT               m_RectTarget;	
	unsigned int       m_IdxX, m_IdxY;
	double             m_PosX, m_PosY, m_PosZ;
	double             m_ImgX, m_ImgY, m_ImgZ;	
	double             m_Factor;
	double             m_Phase;
	double             m_Height;
	double             m_FactorAve;//比例平均	
	double             m_PhaseBase;//基準面相對相平面的相位值
	double             m_PhaseTarget;//階高塊相對相平面的相位值	
	double             m_PhaseOffset;//階高塊相對基準面的相位差-Target-Base
	double             m_HeightBase;//基準面相對相平面的高度值
	double             m_HeightTarget;//階高塊相對相平面的高度值
	double             m_HeightOffset;//階高塊相對基準面的高度差-Target-Base
	double             m_PhaseBaseCalc;//基準面相對相平面的相位值-計算用
	double             m_PhaseTargetCalc;//階高塊相對相平面的相位值-計算用
	double             m_HeightBaseCalc;//基準面相對相平面的高度值-計算用
	double             m_HeightTargetCalc;//階高塊相對相平面的高度值-計算用
	tagPhaseFactorGrid()
	{	
		m_TargetNo = 0;
		m_RectLevel.left = m_RectLevel.right = m_RectLevel.top = m_RectLevel.bottom = 0;
		m_RectTarget.left = m_RectTarget.right = m_RectTarget.top = m_RectTarget.bottom = 0;
		m_IdxX = m_IdxY = 0;
		m_PosX = m_PosY = m_PosZ = 0;
		m_ImgX = m_ImgY = m_ImgZ = 0;		
		m_Phase = 0.0;
		m_Factor = 1.00;
		m_Height = 0.0;
		m_FactorAve = 0.0;
		m_PhaseBase = 0.0;
		m_PhaseTarget = 0.0;
		m_PhaseOffset = 0.0;
		m_HeightBase = 0.0;
		m_HeightTarget = 0.0;		
		m_HeightOffset = 0.0;

		m_PhaseBaseCalc = 0.0;
		m_PhaseTargetCalc = 0.0;
		m_HeightBaseCalc = 0.0;
		m_HeightTargetCalc = 0.0;
	}
} TPhaseFactorGrid, *PPhaseFactorGrid;
//-------------------------------------------------------------------------------------//
typedef struct tagPhaseFactorTable
{
	int        nRows;
	int        nCols;	
	double     dHeight;
	int        nTargetNo;
	std::vector<TPhaseFactorGrid> GridList;

	tagPhaseFactorTable()
	{
		nRows   = 0;
		nCols   = 0;
		dHeight = 0;
		nTargetNo = 0;
	}
} TPhaseFactorTable, *PPhaseFactorTable;
//-------------------------------------------------------------------------------------//
//機台XY座標校正
typedef struct tagXYCali
{
	double PosX1;//座標-X-理論值-1
	double PosY1;//座標-Y-理論值-1
	double CaliX1;//座標-X-修正值-1
	double CaliY1;//座標-Y-修正值-1

	double PosX2;//座標-X-理論值-2
	double PosY2;//座標-Y-理論值-2
	double CaliX2;//座標-X-修正值-2
	double CaliY2;//座標-Y-修正值-2

	double PosX3;//座標-X-理論值-3
	double PosY3;//座標-Y-理論值-3
	double CaliX3;//座標-X-修正值-3
	double CaliY3;//座標-Y-修正值-3

	double PosX4;//座標-X-理論值-4
	double PosY4;//座標-Y-理論值-4
	double CaliX4;//座標-X-修正值-4
	double CaliY4;//座標-Y-修正值-4

	//加速比較用
	double PosXMin;
	double PosXMax;
	double PosYMin;
	double PosYMax;

	double CaliXMin;
	double CaliXMax;
	double CaliYMin;
	double CaliYMax;

	//座標轉換
	double P2C_M11;
	double P2C_M12;
	double P2C_M13;
	double P2C_M21;
	double P2C_M22;
	double P2C_M23;
	double P2C_M31;
	double P2C_M32;
	double P2C_M33;

	double C2P_M11;
	double C2P_M12;
	double C2P_M13;
	double C2P_M21;
	double C2P_M22;
	double C2P_M23;
	double C2P_M31;
	double C2P_M32;
	double C2P_M33;
	tagXYCali()
	{
		PosX1 = PosX2 = PosX3 = PosX4 = 0;
		PosY1 = PosY2 = PosY3 = PosY4 = 0;
		CaliX1 = CaliX2 = CaliX3 = CaliX4 = 0;
		CaliY1 = CaliY2 = CaliY3 = CaliY4 = 0;
		PosXMin = PosXMax = PosYMin = PosYMax = 0;
		CaliXMin = CaliXMax = CaliYMin = CaliYMax = 0;

		//跟機台方向有關嗎?
		P2C_M11 = 1; P2C_M12 = 0; P2C_M13 = 0;
		P2C_M21 = 0; P2C_M22 = 1; P2C_M23 = 0;
		P2C_M31 = 0; P2C_M32 = 0; P2C_M33 = 1;

		C2P_M11 = 1; C2P_M12 = 0; C2P_M13 = 0;
		C2P_M21 = 0; C2P_M22 = 1; C2P_M23 = 0;
		C2P_M31 = 0; C2P_M32 = 0; C2P_M33 = 1;
	};
} TXYCali, *PXYCali;
//-------------------------------------------------------------------------------//
typedef struct tagITSCommNode//ITS Communication Node
{
	char         cIdent[32];   //識別碼
	int          nVer;         //版本碼
	int          nDir;         //訊息方向
	int          nStatus;      //狀態碼
	int          nPPID;        //資料唯一碼
	int          nAck;         //是否等待回傳
	int          nTimeout;     //逾時時間(ms)
	int          nErrorCode;   //錯誤碼
	std::wstring wsErrorCode;  //錯誤內容
	int          nChkCount;    //AOI確認次數-超過就不處理
	std::string  sRawData;     //原始資料 

	tagITSCommNode()
	{
		::strcpy(cIdent, AOI3D_VENDOR_A);//"JET"
		nVer = 0;
		nDir = 0;
		nStatus = 0;
		nPPID = 0;
		nAck = 0;
		nTimeout = 0;
		nErrorCode = 0;
		nChkCount = 0;
		wsErrorCode.clear();
		sRawData.clear();
	}
} TITSCommNode, *PITSCommNode;
//-------------------------------------------------------------------------------//
typedef struct tagRepairResultNode//維修站結果節點
{
	char            sDateTime[16];//檢測時間
	LANE_ID         eLaneID;//軌道編號
	RESULT_ID       eResultID;//檢測結果
	tagRepairResultNode()
	{
		strcpy(sDateTime, "19000101010101");
		eLaneID = LANE_ID_A;
		eResultID = RESULT_ID_NONE; 
	}
} TRepairResultNode, *PRepairResultNode;
//-------------------------------------------------------------------------------//
typedef struct tagOnlineProcParam//線上檢測函式參數
{		
	void              *pProject;//專案指標//20230425
	bool               bExitLoop;//離開迴圈-軌道自動運轉使用
	TASK_MODE          eTaskMode;//任務模式
	LANE_ID            eLaneID;//軌道編號
	LANE_ID            eLaneIDNext;//軌道編號
	PCB_OUT_MODE       ePCBOutMode;//出板模式
	ONLINE_FROM_MODE   eFromMode;//呼叫來源		
	MULTI_LANE_MODE    eMultiLaneMode;//多軌道模式
	LANE_WORK_MODE     eLaneWorkModeLA;//A軌道模式
	LANE_WORK_MODE     eLaneWorkModeLB;//B軌道模式
	bool               bBypassLastSignal;//不管下一站訊號
	bool               bBypassNextSignal;//不管下一站訊號	
	PCB_OUT_DIRECTION  ePCBOutDirection;//出板方向	
	ONLINE_STATE_MODE  eOnlineStateOld;//狀態模式
	ONLINE_STATE_MODE  eOnlineStateNew;//狀態模式
	DWORD              dwSleepTime;//延遲時間
	int                nLastStationLineMode;//與上一站連線方式, 2線式或4線式
	WND_MESSAGE_MODE   eWndMessageMode;//視窗訊息模式
	bool               bConveyerPreRunMode;//軌道預跑模式

	tagOnlineProcParam()
	{	
		pProject = NULL;
		bExitLoop = false;
		eTaskMode  = TASK_NONE;
		eLaneID    = LANE_ID_NULL;
		eLaneIDNext= LANE_ID_NULL;
		bBypassLastSignal = false;
		bBypassNextSignal = false;
		ePCBOutMode= PCB_OUT_NORMAL;		
		eMultiLaneMode = MULTI_LANE_1;
		eLaneWorkModeLA = LANE_WORK_RUN;
		eLaneWorkModeLB = LANE_WORK_DISABLE;		
		eFromMode  = ONLINE_FROM_ONLINE_THREAD;		
		ePCBOutDirection = PCB_OUT_DIR_FORWARD;
		eOnlineStateOld = ONLINE_STATE_INSPECTION_STOP;		
		eOnlineStateNew = ONLINE_STATE_INSPECTION_STOP;
		dwSleepTime = 0;
		nLastStationLineMode = 4;
		eWndMessageMode = WND_MESSAGE_SEND;
		bConveyerPreRunMode = false;
	}
} TOnlineProcParam, *POnlineProcParam;
//-------------------------------------------------------------------------------//
typedef struct tagRemoteParam
{	
	size_t     nIndex;	
	bool       bActived;//是否主要
	bool       bDeleted;//是否刪除
	CString    sRemotePCName;//遠端電腦名稱
	CString    sProjectFolder;//專案資料夾
	CString    sTuningFolder;//調機資料夾	
	tagRemoteParam()
	{	
		nIndex = -1;
		bActived = false;
		bDeleted = false;
		sRemotePCName = _T("");
		sProjectFolder = _T("");
		sTuningFolder = _T("");		
	}
} TRemoteParam, *PRemoteParam;
//-------------------------------------------------------------------------------//
typedef struct tagPartBarcode//零件條碼
{
	std::wstring   wsName;
	std::wstring   wsCode;
} TPartBarcode, *PPartBarcode;
//-------------------------------------------------------------------------------//
typedef struct tagVersionCode//專案版本號
{
	bool           bEnabled;
	std::wstring   wsCodeName;
	tagVersionCode()
	{
		bEnabled = false;
		wsCodeName = L"";
	}
} TVersionCode, *PVersionCode;
//-------------------------------------------------------------------------------//
typedef struct tagVersionParam//零件版本參數
{
	bool           bBypassed;  //不檢測
	bool           bBypassed3D;//不檢測3D
	bool           bXBoardUnit;//報廢件
	std::wstring   wsModelName;//模組名稱
	std::wstring   wsPartNumber;//料號名稱
	tagVersionParam()
	{
		bBypassed = false;		
		bBypassed3D = false;
		bXBoardUnit = false;
		wsModelName = L"";
		wsPartNumber = L"";
	}
} TVersionParam, *PVersionParam;
//-------------------------------------------------------------------------------//
typedef struct tagNPM_APC_Result
{
	int        nSTS;
	double     dGapX;
	double     dGapY;
	double     dGapA;	

	double     dSX;//Size X
	double     dSY;//Size Y
	double     dST;//Size Tilt
	int        nNgCode;
	tagNPM_APC_Result()
	{
		nSTS = 0;
		dGapX = 0.0;
		dGapY = 0.0;
		dGapA = 0.0;

		dSX = 0.0;
		dSY = 0.0;
		dST = 0.0;
		nNgCode = 0;		
	}
} TNPM_APC_Result, *PNPM_APC_Result;
//-------------------------------------------------------------------------------//
typedef struct tagLoadParam_BOM//載入BOM的所需參數
{
	bool          bUnicode;//是否Unicode檔案
	int           nStartLine;//起始列數
	int           nItemIdx;//項目編號
	int           nPNIdx;//料號編號
	int           nDescIdx;//描述編號//非必要
	int           nUsageIdx;//使用數編號
	int           nLocationIdx;//上件名編號
	tagLoadParam_BOM()
	{
		bUnicode = false;
		nStartLine = 0;
		nItemIdx = -1;
		nPNIdx = -1;
		nDescIdx = -1;
		nUsageIdx = -1;
		nLocationIdx = -1;
	}
} TLoadParam_BOM, *PLoadParam_BOM;
//-------------------------------------------------------------------------------//
typedef struct tagComponentNode//Component Node
{
	CString           sBoardName;//單板名
	CString           sModelName;//模組名
	CString           sPartNumber;//料號名	
	CString           sNozzleName;//吸嘴名	
	CString           sComponentName;//零件名
	double            fAngle;//零件角度
	double            fCadPosX;//座標-X
	double            fCadPosY;//座標-Y	
	bool              bMainPartNumber;//主要料號

	void             *pPanel;//整板指標 
	void             *pBoard;//單板指標 
	void             *pCompoennt;//零件指標 

	DWORD             dwModelType;//模組樣式
	DWORD             dwResultID;//結果編號

	bool              bFocused;//焦點
	bool              bSelected;//選取

	int               nTempInt;	
	tagComponentNode()
	{
		sBoardName = _T("");
		sModelName = _T("Model");
		sPartNumber = _T("PartNumber");
		sNozzleName = _T("Unset");
		sComponentName = _T("NewPart");
		fAngle = 0.0f;
		fCadPosX = 0.0f;
		fCadPosY = 0.0f;
		bMainPartNumber=true;

		pPanel = NULL;
		pBoard = NULL;
		pCompoennt = NULL;

		dwModelType = 0;
		dwResultID = 0;

		bFocused = false;
		bSelected = false;

		nTempInt = 0;
	}
} TComponentNode, *PComponentNode;
//-------------------------------------------------------------------------------//
typedef struct tagCopyFileNode//複製檔案
{
	CString     SrcFile;//來源檔案名或路徑名
	CString     DstFile;//目的檔案名或路徑名
	bool        bDelSrc;//刪除來源
	bool        bFileMode;//檔案或路徑	
	tagCopyFileNode()
	{
		SrcFile = _T("");
		DstFile = _T("");
		bDelSrc = false;
		bFileMode = true;
	}
} TCopyFileNode, *PCopyFileNode;
//-------------------------------------------------------------------------------------//
typedef CMap<CString, LPCTSTR, std::vector<CString>, std::vector<CString>&> HASI_FieldMap;
enum HASI_AOI_STAGE {
	//AOI 檢查階段
	HASI_AOI_STAGE_NONE,
	HASI_AOI_STAGE_PRE,
	HASI_AOI_STAGE_POST,
};
enum HASI_STATE_MODE {
	//這個狀態是由韓華傳遞要求而改變的，為了測試，新增HASI_STATE_MODE_TEST
	//Note*: Points for which information cannot be confirmed, such as when a .sco file is not
	//       received or is received but does not contain point data, are processed as NSCO.
	//.sco: SPI offset file
	HASI_STATE_MODE_STOP,	//Do not send .omr file [inspect result file]
	//Using in AOIB , pre 
	HASI_STATE_MODE_SC,		//Note*
	HASI_STATE_MODE_AC,		//Ignore .sco file processing
	HASI_STATE_MODE_SCAC,	//Note*
	//Using in SAOI 
	HASI_STATE_MODE_RUN,	
	HASI_STATE_MODE_TEST,  
	HASI_STATE_MODE_RETURN
};
enum HASI_STATE_REQUEST {
	HASI_STATE_REQUEST_JOB = 5,           //to AOI
	HASI_STATE_REQUEST_MODE_TRANSFER = 13,//to AOI
	HASI_STATE_REQUEST_M2M_USAGE = 14     //from AOI
};
enum HASI_SPI_OFFSET_POINT {
	HASI_SPI_OFFSET_POINT_POINTID,
	HASI_SPI_OFFSET_POINT_REF,
	HASI_SPI_OFFSET_POINT_PART,
	HASI_SPI_OFFSET_POINT_DELTAX,
	HASI_SPI_OFFSET_POINT_DELTAY,
	HASI_SPI_OFFSET_POINT_DELTAT
};
typedef struct tagHASIPCBSerial
{
	const std::vector<CString> CheckField = {
		_T("SERIALNO"),_T("BARCODE"),_T("DATE"),_T("TIME"),
		_T("REGNUMBER"),_T("MODIFIED"),
		_T("LANETYPE")
	};
	CString                    m_PCBSerialNo;		//PCB Serial 序號
	CString                    m_PCBSerialBarcode;	//PCB Serial 條碼
	CString                    m_PCBSerialDate;		//PCB Serial 日期
	CString                    m_PCBSerialTime;		//PCB Serial 時間
	CString                    m_PCBSerialRegNo;	//PCB Serial 日期
	CString                    m_PCBSerialModified;	//PCB Serial 修改日期
	CString                    m_PCBSerialLaneType;	//PCB Serial 
	CString                    m_ErrorString;
	tagHASIPCBSerial()
	{
		m_PCBSerialNo = _T("");
		m_PCBSerialBarcode = _T("");
		m_PCBSerialDate = _T("");
		m_PCBSerialTime = _T("");
		m_PCBSerialRegNo = _T("");
		m_PCBSerialModified = _T("");
		m_PCBSerialLaneType = _T("");
		m_ErrorString = _T("");
	}
	bool HASI_InitSerialFile(std::vector<CString> Field, std::vector<CString> Data) {
		if (CheckField.size() != Field.size()) {
			m_ErrorString = _T("Serial file format error\n Field Size is incorrect.");
			return false;
		}
		if (Field.size() != Data.size()) {
			m_ErrorString = _T("Serial file format error\n Data Size is incorrect.");
			return false;
		}
		for (int i = 0; i < CheckField.size(); i++) {
			if (CheckField.at(i) != Field.at(i)) {
				m_ErrorString = _T("Serial file format error\nField format is incorrect.");
				return false;
			}
			switch (i)
			{
			case(0):
				m_PCBSerialNo = Data.at(i);
				break;
			case(1):
				m_PCBSerialBarcode = Data.at(i);
				break;
			case(2):
				m_PCBSerialDate = Data.at(i);
				break;
			case(3):
				m_PCBSerialTime = Data.at(i);
				break;
			case(4):
				m_PCBSerialRegNo = Data.at(i);
				break;
			case(5):
				m_PCBSerialModified = Data.at(i);
				break;
			case(6):
				m_PCBSerialLaneType = Data.at(i);
				break;
			default:
				break;
			}
		}
		return true;
	}
	void HASI_ClearSerialFile() {
		m_PCBSerialNo = _T("");
		m_PCBSerialBarcode = _T("");
		m_PCBSerialDate = _T("");
		m_PCBSerialTime = _T("");
		m_PCBSerialRegNo = _T("");
		m_PCBSerialModified = _T("");
		m_PCBSerialLaneType = _T("");
		m_ErrorString = _T("");
	};
} tHASIPCBSerial, *pHASIPCBSerial;
#endif