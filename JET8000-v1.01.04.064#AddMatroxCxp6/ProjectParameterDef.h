#ifndef _PROJECT_PARAMETER_DEF_H
#define _PROJECT_PARAMETER_DEF_H
//-------------------------------------------------------------------------------------//
#include "WndDefectItem.h"
//-------------------------------------------------------------------------------------//
#define   COPY_PROJECT_FOLDER_NONE              0x00000000//全部資料夾不複製
#define   COPY_PROJECT_FOLDER_FIDUCIAL          0x00000001//複製定位點資料夾
#define   COPY_PROJECT_FOLDER_LIBRARY           0x00000002//複製資料庫資料夾
#define   COPY_PROJECT_FOLDER_PART_LIBRARY      0x00000004//複製零件資料庫資料夾
#define   COPY_PROJECT_FOLDER_FIELD_MARK        0x00000008//複製區域標記資料夾
#define   COPY_PROJECT_FOLDER_OFFLINE           0x00000010//複製離線編輯資料夾
#define   COPY_PROJECT_FOLDER_SYSTEM            0x00000020//複製系統參數資料夾
#define   COPY_PROJECT_FOLDER_OPER_LOG          0x00000040//複製操作訊息資料夾
#define   COPY_PROJECT_FOLDER_ALL               0xFFFFFFFF//複製區域標記資料夾
#define   COPY_PROJECT_FOLDER_NO_OFFLINE        COPY_PROJECT_FOLDER_ALL-COPY_PROJECT_FOLDER_OFFLINE//複製離線編輯資料夾
//-------------------------------------------------------------------------------------//
#define   PROJECT_SPC_COMPONENT_FOLDER          _T("Components")//專案SPC零件圖資料夾名稱
//-------------------------------------------------------------------------------------//
enum PROJECT_PARAM_ID//PROJECT_PARAM_
{
	PROJECT_PARAM_BEGIN,
	
	PROJECT_PARAM_MODULE_NAME, //專案機種名稱
	PROJECT_PARAM_VERSION,//專案版本
	PROJECT_PARAM_PANEL_SIDE,//專案產品正背面	
	PROJECT_PARAM_WORDK_NUMBER,//專案工單號碼
	PROJECT_PARAM_OPEN_CODE,//專案開檔碼	

	PROJECT_PARAM_TEST_SIZE_WIDTH,//檢測尺寸寬度
	PROJECT_PARAM_TEST_SIZE_HEIGHT,//檢測尺寸高度

	PROJECT_PARAM_FOCUS_OFFSET,//專案焦距偏差-um	
	PROJECT_PARAM_LANE_WIDTH,//專案軌道寬度-mm	
	PROJECT_PARAM_XBOARD_CHECK_RATIO,//專案X板確認比例
	PROJECT_PARAM_SPACE_TO_GRAY_RATIO_MODE,//高度轉灰階模式	
	PROJECT_PARAM_DLP_LED_COLOR,//DLP-LED顏色
	PROJECT_PARAM_FIELD_SIZE_MODE_W,//區域尺寸模式-寬度
	PROJECT_PARAM_FIELD_SIZE_MODE_H,//區域尺寸模式-長度

	PROJECT_PARAM_TEST_PROJECT_SCALE_MODE,//專案底圖縮小比例

	PROJECT_PARAM_SAVE_TEST_PROJECT_MAP,//啟用儲存專案底圖	
	PROJECT_PARAM_SAVE_TEST_PROJECT_MAP_01,//啟用儲存專案底圖-01
	PROJECT_PARAM_SAVE_TEST_PROJECT_MAP_02,//啟用儲存專案底圖-02
	PROJECT_PARAM_SAVE_TEST_PROJECT_MAP_03,//啟用儲存專案底圖-03
	PROJECT_PARAM_SAVE_TEST_PROJECT_MAP_04,//啟用儲存專案底圖-04
	PROJECT_PARAM_SAVE_TEST_PROJECT_MAP_05,//啟用儲存專案底圖-05
	PROJECT_PARAM_SAVE_TEST_PROJECT_MAP_06,//啟用儲存專案底圖-06
	PROJECT_PARAM_SAVE_TEST_PROJECT_MAP_07,//啟用儲存專案底圖-07
	PROJECT_PARAM_SAVE_TEST_PROJECT_MAP_08,//啟用儲存專案底圖-08	
	PROJECT_PARAM_SAVE_TEST_PROJECT_MAP_TO_REPAIR,//啟用儲存專案檢測底圖至維修站	

	PROJECT_PARAM_ONLINE_TUNING_MODE,//在線調機模式
	PROJECT_PARAM_SAVE_MODEL_IMAGE_MODE,//儲存模組圖片模式
	PROJECT_PARAM_SAVE_FIELD_IMAGE_MODE,//儲存區域圖片模式
	PROJECT_PARAM_SAVE_MODEL_IMAGE_MODE_AI,//儲存模組圖片模式-AI	
	PROJECT_PARAM_SAVE_MODEL_IMAGE_ON_OFF_AI,//儲存模組圖片開關-AI	
	PROJECT_PARAM_SAVE_MODEL_IMAGE_3D_FILE_AI,//儲存模組圖片3D檔案-AI
	PROJECT_PARAM_SAVE_OFFLINE_IMAGE_SCOPE,//儲存調機影像範疇
	PROJECT_PARAM_SAVE_STATIC_DATA,//儲存統計資料	
	PROJECT_PARAM_SAVE_COMPONENT_WND_LIST_MODE,//儲存零件所有檢測框模式	
	PROJECT_PARAM_SAVE_SPC_HEADER_JSON_BEFORE_INSPECTION,//檢測前儲存JSON頭檔


	PROJECT_PARAM_PANEL_FD_NG_SKIP_COUNT,//整板定位點跳過檢測數量
	PROJECT_PARAM_PANEL_FD_NG_HANDLE_MODE,//整板定位點異常處理模式
	PROJECT_PARAM_BOARD_FD_NG_SKIP_COUNT,//單板定位點跳過檢測數量
	PROJECT_PARAM_BOARD_FD_NG_HANDLE_MODE,//單板定位點異常處理模式
	PROJECT_PARAM_BOARD_FD_GRAB_MODE,//單板定位點取像模式	

	PROJECT_PARAM_DEFECT_HANDLE_MODE,//檢出瑕疵處理模式	
	PROJECT_PARAM_PCB_OUT_MODE,//PCB出板模式	
	PROJECT_PARAM_ENABLE_CONVEYER_PRE_RUN,//軌道提前運轉功能
	PROJECT_PARAM_INSPECTION_FIELD_BUILD_MODE,//檢測區域配置模式
	PROJECT_PARAM_INSPECTION_FIELD_BUILD_AREA_MODE,//檢測區域配置面積模式
	PROJECT_PARAM_FIELD_DIVISION_MODE,//檢測區域分割模式
	PROJECT_PARAM_FIELD_DIVISION_BOARD_FD_FIRST,//區域分割單板定位點優先	
	PROJECT_PARAM_FIELD_PATH_MODE,//檢測區域路徑模式
	PROJECT_PARAM_FIELD_SECTION_FACTOR,//區域區間係數-影響路徑規劃的走法	
	PROJECT_PARAM_PROJECT_LINK_SERVER_MODE,//專案連線伺服器模式	
	PROJECT_PARAM_SERVER_LIBRARY_GROUP,//專案伺服器資料庫群組	
	PROJECT_PARAM_XBOARD_MAPPING_FILE_MODE,//報廢板映射檔案模式
	PROJECT_PARAM_XBOARD_MAPPING_FILE_FLOW,//報廢板映射檔案順序模式
	PROJECT_PARAM_XBOARD_MAPPING_FILE_MES_CHECK,//報廢板MES Comm條件檢測
	
	PROJECT_PARAM_GENERAL_STR_01,//通用參數字串-01
	PROJECT_PARAM_GENERAL_STR_02,//通用參數字串-02
	PROJECT_PARAM_GENERAL_STR_03,//通用參數字串-03
	PROJECT_PARAM_GENERAL_STR_04,//通用參數字串-04

	PROJECT_PARAM_STATISTIC_DEFECT_FROM_MODE,//瑕疵來源模式
	PROJECT_PARAM_STATISTIC_BY_MODE,//統計依據時間模式
	PROJECT_PARAM_STATISTIC_BY_TIME_VALUE,//統計依據時間數量	
	PROJECT_PARAM_STATISTIC_BY_COUNT_VALUE,//統計依據次數數量	
	PROJECT_PARAM_ALARM_TEST_YIELD_MIN,//警報-檢測良率下限-%
	PROJECT_PARAM_ALARM_PANEL_YIELD_MIN,//警報-整板良率下限-%
	PROJECT_PARAM_ALARM_BOARD_YIELD_MIN,//警報-單板良率下限-%
	PROJECT_PARAM_ALARM_COMPONENT_YIELD_MIN,//警報-零件良率下限-%
	PROJECT_PARAM_ALARM_COMPONENT_DEFECT_RATE_MAX,//警報-零件單次瑕疵率上限-%	
	PROJECT_PARAM_ALARM_EACH_COMPONENT_TOTAL_NG_COUNT,//警報-瑕疵次數上限
	PROJECT_PARAM_ALARM_EACH_COMPONENT_CONTINUE_NG_COUNT,//警報-連續瑕疵次數
	PROJECT_PARAM_ALARM_LOCK_MODE,//警報鎖機模式
	
	PROJECT_PARAM_STATISTIC_DEFECT_FROM_MODE_ARS,//瑕疵來源模式_ARS
	PROJECT_PARAM_STATISTIC_BY_MODE_ARS,//統計依據時間模式_ARS
	PROJECT_PARAM_STATISTIC_BY_TIME_VALUE_ARS,//統計依據時間數量_ARS
	PROJECT_PARAM_STATISTIC_BY_COUNT_VALUE_ARS,//統計依據次數數量_ARS
	PROJECT_PARAM_ALARM_TEST_YIELD_MIN_ARS,//警報-檢測良率下限-%_ARS
	PROJECT_PARAM_ALARM_PANEL_YIELD_MIN_ARS,//警報-整板良率下限-%_ARS
	PROJECT_PARAM_ALARM_BOARD_YIELD_MIN_ARS,//警報-單板良率下限-%_ARS
	PROJECT_PARAM_ALARM_COMPONENT_YIELD_MIN_ARS,//警報-零件良率下限-%_ARS
	PROJECT_PARAM_ALARM_COMPONENT_DEFECT_RATE_MAX_ARS,//警報-零件單次瑕疵率上限-%_ARS	
	PROJECT_PARAM_ALARM_EACH_COMPONENT_TOTAL_NG_COUNT_ARS,//警報-瑕疵次數上限_ARS
	PROJECT_PARAM_ALARM_EACH_COMPONENT_CONTINUE_NG_COUNT_ARS,//警報-連續瑕疵次數_ARS

	PROJECT_PARAM_BARCODE_INPUT_TYPE,//條碼輸入模式
	PROJECT_PARAM_BARCODE_INPUT_FILE_ENABLE,//檔案讀取條碼模式啟用
	PROJECT_PARAM_BARCODE_INPUT_CAMERA_ENABLE,//相機輸入條碼啟用	
	PROJECT_PARAM_BARCODE_CAMERA_SAVE_IMAGE_ENABLE,//相機條碼存圖啟用	
	PROJECT_PARAM_BARCODE_NG_HANDLE_MODE,//條碼失敗處理模式
	PROJECT_PARAM_BARCODE_CAMERA_GRAB_MODE,//相機條碼讀取模式
	PROJECT_PARAM_BARCODE_DEVICE_GRAB_MODE,//條碼機讀取模式
	PROJECT_PARAM_BARCODE_HANDHELD_READ_MODE,//手持條碼機讀取模式
	PROJECT_PARAM_BARCODE_INPUT_FILE_DELAY_TIME,//讀取檔案模式延遲時間-ms
	PROJECT_PARAM_BARCODE_END_REMOVE_CHAR_COUNT,//條碼後端移除字元數
	PROJECT_PARAM_BARCODE_BEGIN_REMOVE_CHAR_COUNT,//條碼前端移除字元數
	PROJECT_PARAM_BARCODE_AUTO_EXPAND_MODE_PANEL,//條碼自動擴展模式-整板	
	PROJECT_PARAM_BARCODE_AUTO_EXPAND_MODE_BOARD,//條碼自動擴展模式-單板	
	PROJECT_PARAM_BARCODE_VERIFY_MODE,//條碼驗證模式
	PROJECT_PARAM_BARCODE_RETRIEVE_MODE,//條碼取回模式

	//檢測拋件
	PROJECT_PARAM_DROP_OUT_PART_ENABLE,//啟用
	PROJECT_PARAM_DROP_OUT_PART_SAVE_IMAGE,//存圖
	PROJECT_PARAM_DROP_OUT_PART_FRAME_UNIQUE_ID,//影像唯一碼
	PROJECT_PARAM_DROP_OUT_PART_MATCH_USE_SCALE,//使用縮放匹配
	PROJECT_PARAM_DROP_OUT_PART_OVER_LOW,//過低定義
	PROJECT_PARAM_DROP_OUT_PART_OVER_HIGH,//過高定義	
	PROJECT_PARAM_DROP_OUT_PART_DARK_LEVEL,//暗部定義
	PROJECT_PARAM_DROP_OUT_PART_LIGHT_LEVEL,//過亮定義
	PROJECT_PARAM_DROP_OUT_PART_TOLERANCE,//公差	
	PROJECT_PARAM_DROP_OUT_PART_SMOOTH_SIZE,//平滑過濾		
	PROJECT_PARAM_DROP_OUT_PART_EDGE_REMOVE,//邊線移除	
	PROJECT_PARAM_DROP_OUT_PART_DILATE_SIZE,//膨脹尺寸
	PROJECT_PARAM_DROP_OUT_PART_GAUSSIAN_SIZE,//高斯過濾
	PROJECT_PARAM_DROP_OUT_PART_FILTER_OPEN_SIZE,//開運算
	PROJECT_PARAM_DROP_OUT_PART_FILTER_CLOSE_SIZE,//閉運算
	PROJECT_PARAM_DROP_OUT_PART_CALC_SIZE_W,//樣板尺寸寬度
	PROJECT_PARAM_DROP_OUT_PART_CALC_SIZE_H,//樣板尺寸長度
	PROJECT_PARAM_DROP_OUT_PART_MIN_SIZE_W,//最小尺寸寬度
	PROJECT_PARAM_DROP_OUT_PART_MIN_SIZE_H,//最小尺寸長度
	PROJECT_PARAM_DROP_OUT_PART_MAX_SIZE_R,//最大尺寸比例
	PROJECT_PARAM_DROP_OUT_PART_BASE_PLANE_PARAM,//基準面設定
	PROJECT_PARAM_DROP_OUT_PART_SPACE_NOISE_FILTER,//空間雜訊過濾處理		
	PROJECT_PARAM_DROP_OUT_PART_XBOARD_EXCLUDED,//報廢板忽略

	//檢測刮傷
	PROJECT_PARAM_SCRATCH_PART_ENABLE,//啟用
	PROJECT_PARAM_SCRATCH_PART_SAVE_IMAGE,//存圖
	PROJECT_PARAM_SCRATCH_PART_FRAME_UNIQUE_ID,//影像唯一碼
	PROJECT_PARAM_SCRATCH_PART_MATCH_USE_SCALE,//使用縮放匹配
	PROJECT_PARAM_SCRATCH_PART_CALC_SIZE_W,//計算尺寸寬度
	PROJECT_PARAM_SCRATCH_PART_CALC_SIZE_H,//計算尺寸長度
	PROJECT_PARAM_SCRATCH_PART_COLOR_EXPAND,//顏色外擴		
	PROJECT_PARAM_SCRATCH_PART_FILTER_OPEN_SIZE,//開運算
	PROJECT_PARAM_SCRATCH_PART_FILTER_CLOSE_SIZE,//閉運算
	PROJECT_PARAM_SCRATCH_PART_MIN_SIZE_W,//最小尺寸寬度	
	PROJECT_PARAM_SCRATCH_PART_MIN_SIZE_H,//最小尺寸長度
	PROJECT_PARAM_SCRATCH_PART_SMOOTH_SIZE,//檢測刮傷-高斯平滑過濾
	PROJECT_PARAM_SCRATCH_PART_EDGE_THRESHOLD,//檢測刮傷-目標邊緣閾值
	PROJECT_PARAM_SCRATCH_PART_MIN_PIXELS,//檢測刮傷-刮傷最小像素 
	PROJECT_PARAM_SCRATCH_PART_MIN_SIZE_D, //檢測刮傷-刮傷最小對角距離
	PROJECT_PARAM_SCRATCH_PART_MIN_GRAYSCALE, //檢測刮傷-灰階下界
	PROJECT_PARAM_SCRATCH_PART_MAX_GRAYSCALE, //檢測刮傷-灰階上界	

	PROJECT_PARAM_END
};
//-------------------------------------------------------------------------------------//
typedef struct tagProjectParameter
{
	//Region Image
	std::wstring               m_ProjectModuleName;//專案機種名稱
	std::wstring               m_ProjectVersion;//專案版本
	std::wstring               m_ProjectWorkNumber;//專案工單號碼
	std::wstring               m_ProjectOpenCode;//專案開檔碼	
	PANEL_SIDE_MODE            m_ProjectPanelSideMode;//專案產品正背面
	double                     m_RegionMapGain;//檢測範圍影像增益
	double                     m_TestSizeWidth;//檢測尺寸寬度mm
	double                     m_TestSizeHeight;//檢測尺寸高度mm
	double                     m_ProjectFocusOffset;//專案焦距偏差值	
	double                     m_ProjectLaneWidth;//專案軌道寬度
	double                     m_ProjectXBoardCheckRatio;//專案X板確認比例
	int                        m_SpaceToGrayRatioMode;//高度轉灰階比值
	int                        m_ProjectDlpLedColor;//專案DLP-LED顏色
	FIELD_SIZE_MODE            m_ProjectFieldSizeModeW;//區域尺寸模式-寬度
	FIELD_SIZE_MODE            m_ProjectFieldSizeModeH;//區域尺寸模式-長度
	
	SAVE_TEST_MAP_MODE         m_SaveProjectTestMap;//啟用儲存專案檢測底圖
	int                        m_SaveProjectTestMap_01;//啟用儲存專案檢測底圖-01
	int                        m_SaveProjectTestMap_02;//啟用儲存專案檢測底圖-02
	int                        m_SaveProjectTestMap_03;//啟用儲存專案檢測底圖-03
	int                        m_SaveProjectTestMap_04;//啟用儲存專案檢測底圖-04
	int                        m_SaveProjectTestMap_05;//啟用儲存專案檢測底圖-05
	int                        m_SaveProjectTestMap_06;//啟用儲存專案檢測底圖-06
	int                        m_SaveProjectTestMap_07;//啟用儲存專案檢測底圖-07
	int                        m_SaveProjectTestMap_08;//啟用儲存專案檢測底圖-08
	int                        m_SaveProjectTestMapToRepair;//啟用儲存專案檢測底圖至維修站
	int                        m_ProjectTestMapScaleMode;//儲存專案檢測底圖縮圖比例
	SAVE_TEST_IMAGE_MODE       m_OnlineTuningMode;//在線調機模式
	SAVE_TEST_IMAGE_MODE       m_SaveModelImageMode;//儲存模組圖片模式
	SAVE_TEST_IMAGE_MODE       m_SaveFieldImageMode;//儲存區域圖片模式
	SAVE_TEST_IMAGE_MODE       m_SaveModelImageMode_AI;//儲存模組圖片模式-AI
	int                        m_SaveModelImageOnOff_AI;//儲存模組圖片開關-AI
	int                        m_SaveModelImage3DFile_AI;//儲存模組圖片3D檔案[Z3D]

	OFFLINE_IMAGE_SCOPE        m_SaveOfflineImageScope;//儲存離線影像範疇
	int                        m_SaveStaticData;//啟用儲存統計資料
	SAVE_TEST_DATA_MODE        m_SaveComponentWndListMode;//儲存零件所有檢測框模式
	bool                       m_SaveSPCHeaderJSONBeforeInspection;//檢測前儲存JSON頭檔

	PCB_OUT_MODE               m_PCBOutMode;//PCB出板模式	
	int                        m_PanelFdNGSkipCount;//整板定位點跳過檢測數量	 
	FD_NG_HANDLE_MODE          m_PanelFdNGHandleMode;//整板定位點異常處理模式
	int                        m_BoardFdNGSkipCount;//單板定位點跳過檢測數量
	FD_NG_HANDLE_MODE          m_BoardFdNGHandleMode;//單板定位點異常處理模式	
	BOARD_FD_GRAB_MODE         m_BoardFdGrabMode;//單板定位點取像模式		
	CWndDefectItem             m_DefectTestItem;//瑕疵檢測項目模式
	CWndDefectItem             m_DefectAlarmItem;//瑕疵警報項目模式
	CWndDefectItem             m_DefectEnableItem;//瑕疵啟用項目模式
	DEFECT_HANDLE_MODE         m_DefectHandleMode;//檢出異常處理模式	
	int                        m_EnableConveyerPreRun;//軌道提前運轉功能
	FIELD_BUILD_MODE           m_InspectionFieldBuildMode;//檢測區域配置模式
	FIELD_BUILD_AREA_MODE      m_InspectionFieldBuildAreaMode;//檢測區域配置面積模式
	FIELD_DIVISION_MODE        m_FieldDivisionMode;//區域分割模式
	int                        m_FieldDivisionBoardFdFirst;//區域分割單板定位點優先
	FIELD_PATH_MODE            m_FieldPathMode;//區域路徑模式
	double                     m_FieldSectionFactor;//區域區間係數-影響路徑規劃的走法		
	PROJECT_LINK_SERVER_MODE   m_ProjectLinkServerMode;//專案連線伺服器模式
	std::wstring               m_ProjectServerLibraryGroup;//專案伺服器資料庫群組	
	XBOARD_MAPPING_FILE_MODE   m_XBoardMappingFileMode;//報廢板映射檔案模式
	XBOARD_MAPPING_FILE_FLOW   m_XBoardMappingFileFlow;//報廢板映射檔案順序模式//Alan
	bool                       m_XBoardMappingMESCheck;//報廢板MES Comm條件檢測//Alan

	std::wstring               m_ProjectGeneralParamStr_01;//專案通用參數字串-01
	std::wstring               m_ProjectGeneralParamStr_02;//專案通用參數字串-02
	std::wstring               m_ProjectGeneralParamStr_03;//專案通用參數字串-03
	std::wstring               m_ProjectGeneralParamStr_04;//專案通用參數字串-04

	DEFECT_FROM_MODE           m_StatisticDefectFromMode;//瑕疵來源模式
	STATISTIC_BY_MODE          m_StatisticByMode;//統計依據模式
	int                        m_StatisticByTimeValue;//統計依據時間數量-分鐘
	int                        m_StatisticByCountValue;//統計依據次數數量-次數	
	double                     m_AlaramTestYieldMin;//警報-檢測良率下限-%
	double                     m_AlaramPanelYieldMin;//警報-整板良率下限-%
	double                     m_AlaramBoardYieldMin;//警報-單板良率下限-%
	double                     m_AlaramComponentYieldMin;//警報-零件良率下限-%
	double                     m_AlaramComponentDefectRateMax;//警報-零件單次瑕疵率上限-%
	int                        m_AlaramEachComponentTotalNGCount;//警報-每個零件瑕疵數上限
	int                        m_AlaramEachComponentContinueNGCount;//警報-每個零件連續瑕疵次數上限
	ALARM_LOCK_MODE            m_AlarmLockMode;//警報鎖住模式

	DEFECT_FROM_MODE           m_StatisticDefectFromMode_ARS;//瑕疵來源模式-ARS
	STATISTIC_BY_MODE          m_StatisticByMode_ARS;//統計依據模式_ARS
	int                        m_StatisticByTimeValue_ARS;//統計依據時間數量-分鐘_ARS
	int                        m_StatisticByCountValue_ARS;//統計依據次數數量-次數_ARS
	double                     m_AlaramTestYieldMin_ARS;//警報-檢測良率下限-%_ARS
	double                     m_AlaramPanelYieldMin_ARS;//警報-整板良率下限-%_ARS
	double                     m_AlaramBoardYieldMin_ARS;//警報-單板良率下限-%_ARS
	double                     m_AlaramComponentYieldMin_ARS;//警報-零件良率下限-%_ARS
	double                     m_AlaramComponentDefectRateMax_ARS;//警報-零件單次瑕疵率上限-%_ARS
	int                        m_AlaramEachComponentTotalNGCount_ARS;//警報-每個零件瑕疵數上限_ARS
	int                        m_AlaramEachComponentContinueNGCount_ARS;//警報-每個零件連續瑕疵次數上限_ARS

	//條碼設定
	int                        m_BarcodeInputFileEnabled;//檔案讀取條碼模式啟用
	int                        m_BarcodeInputCameraEnabled;//相機讀取條碼模式啟用
	int                        m_BarcodeCameraSaveImageEnabled;//相機條碼存圖啟用
	BARCODE_INPUT_TYPE         m_BarcodeInputType;//條碼輸入樣式
	BARCODE_NG_HANDLE_MODE     m_BarcodeNGHandleMode;//條碼失敗處理模式
	BARCODE_CAMERA_GRAB_MODE   m_BarcodeCameraGrabMode;//相機條碼讀取模式
	BARCODE_DEVICE_GRAB_MODE   m_BarcodeDeviceGrabMode;//固定條碼機讀取模式
	BARCODE_HANDHELD_READ_MODE m_BarcodeHandHeldReadMode;//手持條碼機讀取模式
	int                        m_BarcodeInputFileDelayTime;//相機讀取檔案模式延遲時間-ms
	int                        m_BarcodeEndRemoveCharCount;//條碼後端移除字元數
	int                        m_BarcodeBeginRemoveCharCount;//條碼前端移除字元數	
	BARCODE_AUTO_EXPAND_MODE   m_BarcodeAutoExpandMode_Panel;//條碼自動擴展模式-整板
	BARCODE_AUTO_EXPAND_MODE   m_BarcodeAutoExpandMode_Board;//條碼自動擴展模式-單板
	int                        m_BarcodeVerifyMode;//條碼驗證模式
	int                        m_BarcodeRetrieveMode;//條碼查詢模式

	//版本代碼
	int                        m_VersionCodeActiveIndex;//版本代碼啟用編號

	//檢測拋件
	int                        m_DropOutPartEnable;//啟用
	int                        m_DropOutPartSaveImage;//存圖
	unsigned int               m_DropOutPartFrameIndex;//影像唯一碼
	unsigned int               m_DropOutPartFrameUniqueID;//影像唯一碼
	int                        m_DropOutPartMatchUseScale;//縮放匹配啟用	
	int                        m_DropOutPartOverLow;//過低定義
	int                        m_DropOutPartOverHigh;//過高定義
	int                        m_DropOutPartDarkLevel;//暗部定義
	int                        m_DropOutPartLightLevel;//過亮定義	
	int                        m_DropOutPartTolerance;//公差	
	int                        m_DropOutPartSmoothSize;//平滑過濾
	int                        m_DropOutPartEdgeRemove;//邊線移除
	int                        m_DropOutPartDilateSize;//膨脹尺寸
	int                        m_DropOutPartGaussian;//高斯過濾
	int                        m_DropOutPartFilterOpenSize;//開運算尺寸
	int                        m_DropOutPartFilterCloseSize;//閉運算尺寸
	int                        m_DropOutPartCalcSizeW;//樣板尺寸寬度
	int                        m_DropOutPartCalcSizeH;//樣板尺寸長度	
	double                     m_DropOutPartMinSizeW;//最小尺寸寬度
	double                     m_DropOutPartMinSizeH;//最小尺寸長度
	double                     m_DropOutPartMaxSizeR;//最大尺寸比例	
	TNoiseFilterParam          m_DropOutPartSpaceNoiseFilter;//空間雜訊過濾處理
	bool                       m_DropOutPartXBoardExcluded;//報廢板排除

	//檢測刮傷
	int                        m_ScratchPartEnable;//啟用
	int                        m_ScratchPartSaveImage;//存圖
	unsigned int               m_ScratchPartFrameIndex;//影像唯一碼
	unsigned int               m_ScratchPartFrameUniqueID;//影像唯一碼
	int                        m_ScratchPartMatchUseScale;//縮放匹配啟用	
	double                     m_ScratchPartCalcSizeW;//計算尺寸寬度-um
	double                     m_ScratchPartCalcSizeH;//計算尺寸長度-um	
	int                        m_ScratchPartColorExpand;//顏色外擴
	int                        m_ScratchPartFilterOpenSize;//開運算
	int                        m_ScratchPartFilterCloseSize;//閉運算
	double                     m_ScratchPartMinSizeW;//最小尺寸寬度
	double                     m_ScratchPartMinSizeH;//最小尺寸長度
	int                        m_ScratchPartSmoothSize;//高斯平滑過濾
	int                        m_ScratchPartEdgeThreshold;//目標邊緣閾值
	int                        m_ScratchPartMinPixels;//刮傷最小像素 
	double                     m_ScratchPartMinSizeD;//最小對角距離
	int                        m_ScratchPartMinGrayscale;//灰階下界
	int                        m_ScratchPartMaxGrayscale;//灰階上界
	//檢測尺寸		
	unsigned int               m_PartDimensionFrameIndex;//影像唯一碼
	unsigned int               m_PartDimensionFrameUniqueID;//影像唯一碼	
	int                        m_PartDimensionOverLow;//過低定義
	int                        m_PartDimensionOverHigh;//過高定義		
	int                        m_PartDimensionDilateSize;//膨脹尺寸	
	int                        m_PartDimensionFilterOpenSize;//開運算尺寸
	int                        m_PartDimensionFilterCloseSize;//閉運算尺寸	
	double                     m_PartDimensionMinSizeW;//最小尺寸寬度
	double                     m_PartDimensionMinSizeH;//最小尺寸長度
	double                     m_PartDimensionMaxSizeR;//最大尺寸比例	
	TNoiseFilterParam          m_PartDimensionSpaceNoiseFilter;//空間雜訊過濾處理
	tagProjectParameter()
	{	
		m_ProjectModuleName=L"Module";
		m_ProjectVersion=L"Version";
		m_ProjectWorkNumber=L"Work Number";
		m_ProjectOpenCode=L"";		
		m_ProjectPanelSideMode=PANEL_SIDE_TOP;
		m_ProjectFieldSizeModeW=FIELD_SIZE_100;//區域尺寸模式-寬度
		m_ProjectFieldSizeModeH=FIELD_SIZE_100;//區域尺寸模式-長度
		m_RegionMapGain = 2.0;
		m_TestSizeWidth = 0;
		m_TestSizeHeight = 0;
		m_ProjectFocusOffset = 0;
		m_ProjectLaneWidth = 100;
		m_ProjectXBoardCheckRatio = 100.0;//專案X板確認比例
		m_SpaceToGrayRatioMode=50;		
		m_ProjectDlpLedColor = 1;

		m_SaveProjectTestMap = SAVE_TEST_MAP_DISABLE;
		m_SaveProjectTestMap_01 = FN_ENABLE;
		m_SaveProjectTestMap_02 = FN_ENABLE;
		m_SaveProjectTestMap_03 = FN_ENABLE;
		m_SaveProjectTestMap_04 = FN_ENABLE;
		m_SaveProjectTestMap_05 = FN_ENABLE;
		m_SaveProjectTestMap_06 = FN_ENABLE;
		m_SaveProjectTestMap_07 = FN_ENABLE;
		m_SaveProjectTestMap_08 = FN_ENABLE;
		m_SaveProjectTestMapToRepair = FN_DISABLE;
		m_ProjectTestMapScaleMode = 4;//儲存專案檢測底圖縮圖比例

		m_OnlineTuningMode = SAVE_TEST_IMAGE_DEFECT;//在線調機模式
		m_SaveModelImageMode = SAVE_TEST_IMAGE_DEFECT;//儲存模組圖片模式
		m_SaveFieldImageMode = SAVE_TEST_IMAGE_DISABLE;//儲存區域圖片模式
		m_SaveModelImageMode_AI = SAVE_TEST_IMAGE_DISABLE;//儲存模組圖片模式-AI		
		m_SaveModelImageOnOff_AI = 0xFFFF;//儲存模組圖片開關-AI	
		m_SaveModelImage3DFile_AI = FN_ENABLE;

		m_SaveOfflineImageScope = OFFLINE_IMAGE_FOV;
		m_SaveStaticData = FN_DISABLE;//啟用儲存統計資料
		m_SaveComponentWndListMode = SAVE_TEST_DATA_DISABLE;
		m_SaveSPCHeaderJSONBeforeInspection = false;

		m_PCBOutMode = PCB_OUT_NORMAL;
		m_PanelFdNGSkipCount = 2;
		m_PanelFdNGHandleMode = FD_NG_HANDLE_STOP;
		m_BoardFdNGSkipCount = 2;
		m_BoardFdNGHandleMode = FD_NG_HANDLE_NONE;
		m_BoardFdGrabMode = BOARD_FD_GRAB_AFTER_PANEL;
		m_DefectTestItem.SetAll(1);
		m_DefectAlarmItem.SetAll(0);
		m_DefectEnableItem.SetAll(1);
		m_DefectHandleMode = DEFECT_HANDLE_NEXT_STOP;
		m_BarcodeInputFileEnabled = FN_DISABLE;
		m_BarcodeInputCameraEnabled = FN_ENABLE;
		m_BarcodeCameraSaveImageEnabled = FN_DISABLE;
		m_BarcodeInputType = BARCODE_INPUT_DISABLED;
		m_BarcodeNGHandleMode = BARCODE_NG_HANDLE_ALARM;
		m_BarcodeCameraGrabMode = BARCODE_CAMERA_GRAB_INSPECTING;
		m_BarcodeDeviceGrabMode = BARCODE_DEVICE_GRAB_WHILE_PCB_IN;
		m_BarcodeHandHeldReadMode = BARCODE_HANDHELD_READ_PROJECT;
		m_BarcodeInputFileDelayTime = 50;
		m_BarcodeEndRemoveCharCount= 0;
		m_BarcodeBeginRemoveCharCount = 0;
		m_BarcodeAutoExpandMode_Panel = BARCODE_AUTO_EXPAND_DISABLE;
		m_BarcodeAutoExpandMode_Board = BARCODE_AUTO_EXPAND_DISABLE;
		m_BarcodeVerifyMode = FN_DISABLE;
		m_BarcodeRetrieveMode = FN_DISABLE;
		m_VersionCodeActiveIndex = PROJECT_VERSION_CODE_BASE_INDEX;//版本代碼啟用編號
		m_EnableConveyerPreRun = FN_DISABLE;
		m_InspectionFieldBuildMode = FIELD_BUILD_RANDOM_PANEL;//檢測區域配置模式
		m_InspectionFieldBuildAreaMode = FIELD_BUILD_AREA_COMPONENT;//檢測區域配置面積模式
		m_FieldDivisionMode = FIELD_DIVISION_DIAGONAL_LINE;//區域分割模式
		m_FieldDivisionBoardFdFirst = FN_ENABLE;
		m_FieldPathMode = FIELD_PATH_SPATH_HOR;//區域路徑模式
		m_FieldSectionFactor = 0.5;//區域區間係數-影響路徑規劃的走法
		m_ProjectLinkServerMode = PROJECT_LINK_SERVER_DISABLE;//伺服器資料庫模式
		m_ProjectServerLibraryGroup=L"Library";
		m_XBoardMappingFileMode = XBOARD_MAPPING_FILE_DISABLE;
		m_XBoardMappingFileFlow = XBOARD_MAPPING_FILE_FLOW_DEFAULT;
		m_XBoardMappingMESCheck = FN_DISABLE;
		
		m_ProjectGeneralParamStr_01 = L"";
		m_ProjectGeneralParamStr_02 = L"";
		m_ProjectGeneralParamStr_03 = L"";
		m_ProjectGeneralParamStr_04 = L"";

		m_StatisticDefectFromMode = DEFECT_FROM_NONE;
		m_StatisticByMode = STATISTIC_BY_COUNT;
		m_StatisticByTimeValue = 0;
		m_StatisticByCountValue = 0;
		m_AlaramTestYieldMin = 0.0;
		m_AlaramPanelYieldMin = 0.0;
		m_AlaramBoardYieldMin = 0.0;
		m_AlaramComponentYieldMin = 0.0;
		m_AlaramComponentDefectRateMax = 100.0;
		m_AlaramEachComponentTotalNGCount = 0;
		m_AlaramEachComponentContinueNGCount = 0;
		m_AlarmLockMode = ALARM_LOCK_AOI;

		m_StatisticDefectFromMode_ARS = DEFECT_FROM_ARS;//瑕疵來源模式-ARS
		m_StatisticByMode_ARS = STATISTIC_BY_COUNT;//統計依據模式_ARS
		m_StatisticByTimeValue_ARS = 0;//統計依據時間數量-分鐘_ARS
		m_StatisticByCountValue_ARS = 0;//統計依據次數數量-次數_ARS
		m_AlaramTestYieldMin_ARS = 0.0;//警報-檢測良率下限-%_ARS
		m_AlaramPanelYieldMin_ARS = 0.0;//警報-整板良率下限-%_ARS
		m_AlaramBoardYieldMin_ARS = 0.0;//警報-單板良率下限-%_ARS
		m_AlaramComponentYieldMin_ARS = 0.0;//警報-零件良率下限-%_ARS
		m_AlaramComponentDefectRateMax_ARS = 100.0;//警報-零件單次瑕疵率上限-%_ARS
		m_AlaramEachComponentTotalNGCount_ARS = 0;//警報-每個零件瑕疵數上限_ARS
		m_AlaramEachComponentContinueNGCount_ARS = 0;//警報-每個零件連續瑕疵次數上限_ARS

		//檢測拋件
		m_DropOutPartEnable = FN_DISABLE;//啟用
		m_DropOutPartSaveImage = FN_DISABLE;//存圖
		m_DropOutPartFrameIndex = 0;
		m_DropOutPartFrameUniqueID = FRAME_UNIQUE_ID_LOW;//影像唯一碼
		m_DropOutPartMatchUseScale = FN_DISABLE;//縮放匹配啟用		
		m_DropOutPartOverLow = 150;//過低定義
		m_DropOutPartOverHigh = 2000;//過高定義
		m_DropOutPartDarkLevel = 30;//暗部定義
		m_DropOutPartLightLevel = 200;//過亮定義
		m_DropOutPartTolerance = 60;//公差
		m_DropOutPartSmoothSize = 0;//平滑過濾				
		m_DropOutPartEdgeRemove = FN_ENABLE;//邊線移除
		m_DropOutPartDilateSize = 3;//膨脹尺寸
		m_DropOutPartGaussian = 0;//高斯過濾
		m_DropOutPartFilterOpenSize = 5;//開運算
		m_DropOutPartFilterCloseSize = 5;//閉運算
		m_DropOutPartCalcSizeW = 640;//樣板尺寸寬度
		m_DropOutPartCalcSizeH = 640;//樣板尺寸長度
		m_DropOutPartMinSizeW = 300;//最小尺寸寬度
		m_DropOutPartMinSizeH = 300;//最小尺寸長度
		m_DropOutPartMaxSizeR = 6.0;//最大尺寸比例
		m_DropOutPartSpaceNoiseFilter=TNoiseFilterParam();//空間雜訊過濾處理
		m_DropOutPartXBoardExcluded = FN_DISABLE;//報廢板排除

		//檢測刮傷
		m_ScratchPartEnable = FN_DISABLE;//啟用
		m_ScratchPartSaveImage = FN_DISABLE;//存圖
		m_ScratchPartFrameIndex = 0;//影像唯一碼
		m_ScratchPartFrameUniqueID = FRAME_UNIQUE_ID_RGB;//影像唯一碼
		m_ScratchPartMatchUseScale = FN_DISABLE;//縮放匹配啟用		
		m_ScratchPartCalcSizeW = 500;//計算尺寸寬度-um
		m_ScratchPartCalcSizeH = 500;//計算尺寸長度-um	
		m_ScratchPartColorExpand = 10;//顏色外擴
		m_ScratchPartFilterOpenSize = 2;//開運算
		m_ScratchPartFilterCloseSize = 2;//閉運算
		m_ScratchPartMinSizeW = 100;//最小尺寸寬度
		m_ScratchPartMinSizeH = 100;//最小尺寸長度
		m_ScratchPartSmoothSize = 2;//高斯平滑過濾
		m_ScratchPartEdgeThreshold = 8;//目標邊緣閾值
		m_ScratchPartMinPixels = 0;//目標最小像素
		m_ScratchPartMinSizeD = 100;//最小對角距離
		m_ScratchPartMinGrayscale = 0;//灰階下界
		m_ScratchPartMaxGrayscale = 255;//灰階上界

		//檢測尺寸		
		m_PartDimensionFrameIndex = 0;//影像唯一碼
		m_PartDimensionFrameUniqueID = FRAME_UNIQUE_ID_DLP;//影像唯一碼	
		m_PartDimensionOverLow = 150;//過低定義
		m_PartDimensionOverHigh = 5000;//過高定義		
		m_PartDimensionDilateSize = 3;//膨脹尺寸		
		m_PartDimensionFilterOpenSize = 5;//開運算尺寸
		m_PartDimensionFilterCloseSize= 5;//閉運算尺寸	
		m_PartDimensionMinSizeW = 300;//最小尺寸寬度
		m_PartDimensionMinSizeH = 300;//最小尺寸長度
		m_PartDimensionMaxSizeR = 6.0;//最大尺寸比例	
		m_PartDimensionSpaceNoiseFilter=TNoiseFilterParam();//空間雜訊過濾處理
	}
} TProjectParameter, *PProjectParameter;
//-------------------------------------------------------------------------------------//

#endif//_PROJECT_PARAMETER_DEF_H