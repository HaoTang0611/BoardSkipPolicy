#ifndef _SYSTEM_PARAMETER_DEF_H_
#define _SYSTEM_PARAMETER_DEF_H_
//-------------------------------------------------------------------------------------//
enum SYSTEM_PARAM_ID
{
	SYSTEM_PARAM_BEGIN,
	//Folder
	SYSTEM_AOI_FOLDER_HOST,//系統資料夾
	SYSTEM_AOI_FOLDER_TEMP,//暫存的資料夾
	SYSTEM_AOI_FOLDER_LOG, //系統訊息資料夾
	SYSTEM_AOI_FOLDER_PROJECT, //專案資料夾
	SYSTEM_AOI_FOLDER_RESULT,  //結果資料夾	
	SYSTEM_AOI_FOLDER_SERVER,  //伺服器資料夾
	SYSTEM_AOI_FOLDER_STATIC_DATA,//統計數據資料夾
	SYSTEM_AOI_FOLDER_TEXT_REPORT,//文字報告資料夾
	SYSTEM_AOI_FOLDER_ONLINE_TUNING,//線上調機資料夾
	SYSTEM_AOI_FOLDER_ONLINE_BARCODE,//線上條碼資料夾	
	SYSTEM_AOI_FOLDER_ONLINE_OFFLINE,//線上離線編程資料夾	
	SYSTEM_AOI_FOLDER_PROJECT_TEST_MAP,//專案檢測底圖資料夾
	SYSTEM_AOI_FOLDER_PROJECT_TEST_MAP_TEMP,//專案檢測底圖資料夾-暫存
	SYSTEM_AOI_FOLDER_PROJECT_TEST_MAP_BACKUP,//專案檢測底圖資料夾-備份
	SYSTEM_AOI_FOLDER_PROJECT_RAW,//專案原圖資料夾	
	SYSTEM_AOI_FOLDER_PROJECT_DEBUG,//專案除錯資料夾	
	SYSTEM_AOI_FOLDER_MONITOR_STATUS,//監控狀態資料夾		
	SYSTEM_AOI_FOLDER_CUSTOMER_LOG,//客戶訊息資料夾
	SYSTEM_AOI_FOLDER_CUSTOMER_REPORT,//客戶報告資料夾		
	SYSTEM_AOI_FOLDER_BARCODE_FILE_LA,//條碼檔案資料夾-A軌
	SYSTEM_AOI_FOLDER_BARCODE_FILE_LB,//條碼檔案資料夾-B軌	
	SYSTEM_AOI_FOLDER_AI_FILE_EXPORT,//AI檔案輸出資料夾
	SYSTEM_AOI_FOLDER_AI_IMAGE_EXPORT,//AI影像輸出資料夾	
	SYSTEM_AOI_FOLDER_TEST_TRACK,//檢測追蹤資料夾	

	SYSTEM_AOI_LIBRARY_HOST,   //資料庫-本機
	SYSTEM_AOI_LIBRARY_REMOTE, //資料庫-遠端	

	//Basic
	//CString                    m_AppVersion;//軟件版本
	SYSTEM_IP_HOST_COMPUTER,//本機網路網址	

	SYSTEM_MACHINE_LOCATION,//本機廠區
	SYSTEM_MACHINE_BUILDING,//本機棟別
	SYSTEM_MACHINE_FLOOR,//本機樓層
	SYSTEM_MACHINE_ROOM,//本機車間
	SYSTEM_MACHINE_LINE,//本機線別
	SYSTEM_MACHINE_LINE_LB,//本機線別-B軌
	SYSTEM_MACHINE_STATION,//本機站別
	SYSTEM_MACHINE_STATION_LB,//本機站別-B軌
	SYSTEM_MACHINE_SN,//本機序號
	SYSTEM_MACHINE_NAME,//機台型號
	SYSTEM_MACHINE_VENDOR,//機台廠商
	SYSTEM_MACHINE_ALIAS,//機台別名

	SYSTEM_MACHINE_MES_NAME,//MES登入名稱
	SYSTEM_MACHINE_MES_PASSWORD,//MES登入密碼
	SYSTEM_MACHINE_MES_DEVICE,//MES登入裝置
	SYSTEM_MACHINE_MES_DEVICE_2,//MES登入裝置-2
	SYSTEM_MACHINE_MES_CODE_NAME,//MES-設備代號

	SYSTEM_AOI_CUSTOMER_ID,//客戶編號	

	//Multi-Threading
	SYSTEM_CPU_MAX_COUNT_USED,//Cpu最多使用數量
	SYSTEM_THREAD_CNT_SLICE_FILL,//多執行緒數量-畫面填圖	
	SYSTEM_THREAD_CNT_FRAME_MERGE,//多執行緒數量-影像合併	
	SYSTEM_THREAD_CNT_FIELD_MERGE,//多執行緒數量-區域合併
	SYSTEM_THREAD_CNT_FRAME_LOAD,//多執行緒數量-影像載入	
	SYSTEM_THREAD_CNT_REGION_CALC,//多執行緒數量-區域計算
	SYSTEM_THREAD_CNT_PROC_IDLE,//多執行緒數量-閒置核心	
	SYSTEM_THREAD_CNT_GRAB_IDLE,//多執行緒數量-取像閒置核心	

	//Open MP
	SYSTEM_OPEN_MP_CNT_GENERAL,//OpenMP核心數量-一般操作
	SYSTEM_OPEN_MP_CNT_INSPECTION,//OpenMP核心數量-檢測中	
	SYSTEM_OPEN_MP_CHK_SIZE_INSPECTION,//OpenMP確認尺寸-檢測中

	//Library
	SYSTEM_LIBRARY_MODE_MATCH,//影像匹配函式庫樣式
	SYSTEM_HONEYWELL_SWIFT_DECODER_ENABLED,//Honeywell SwiftDecoder啟用	

	SYSTEM_DONGLE_WARNING_REMAINING_DAYS,//硬體鎖警告剩餘天數
	SYSTEM_DONGLE_WARNING_REMAINING_COUNT,//硬體鎖警告剩餘次數	

	//Display
	SYSTEM_DC_STRECTCH_BLT_MODE,//繪圖模式	
	SYSTEM_IMAGE_DISPLAY_MODE,//影像顯示模式
	SYSTEM_IMAGE_DISPLAY_ENHANCE_MODE,//影像顯示強化模式
	SYSTEM_IMAGE_DISPLAY_GAIN,//影像顯示的Gain
	SYSTEM_IMAGE_DISPLAY_GAMMA,//影像顯示的Gamma	
	SYSTEM_IMAGE_DISPLAY_SHARPNESS_RADIUS,//影像顯示的銳利化的半徑
	SYSTEM_IMAGE_DISPLAY_SHARPNESS_AMOUNT,//影像顯示的銳利化的總量
	SYSTEM_IMAGE_DISPLAY_SHARPNESS_THRESHOLD,//影像顯示的銳利化的閥值
	SYSTEM_IMAGE_DISPLAY_LOCAL_GAMMA_CALC_SIZE,//影像顯示的局部Gamma的計算範圍
	SYSTEM_IMAGE_DISPLAY_LOCAL_GAMMA_SCALE_VAL,//影像顯示的局部Gamma的縮放比例
	SYSTEM_IMAGE_DISPLAY_MAX_ZOOM_SCALE,//影像顯示最大縮放比例	
	SYSTEM_ONLINE_FORM_VIEW_MODE,//線上檢測介面模式	
	
	SYSTEM_CALC_FOCUS_SMOOTH_SIZE,//計算焦距平滑尺寸
	SYSTEM_CALC_FOCUS_MODE,//計算焦距模式

	//Message
	SYSTEM_SAVE_MSG_MOVING_TIME,    //是否移動時間狀態訊息	
	SYSTEM_BACKUP_MSG_MOVING_TIME,  //是否備份移動時間狀態訊息		
	SYSTEM_SAVE_MSG_CURRENT_PROCESS,//是否儲存現在狀態訊息	
	SYSTEM_BACKUP_MSG_CURRENT_PROCESS,//是否備份現在狀態訊息
	SYSTEM_SAVE_MSG_MACHINE_MONITOR,//是否儲存機台監控訊息
	SYSTEM_SAVE_MSG_MACHINE_MONITOR_WEB,//是否儲存機台監控訊息-網頁用
	SYSTEM_SAVE_CAMERA_ADD_RING_BUFFER_LOG,//是否儲存相機增加循環資料訊息	
	SYSTEM_SEND_DEBUG_VIEW_STRING,//是否送至DebugView視窗
	SYSTEM_SAVE_SLICE_FILL_THREAD_LOG,//是否儲存影像填滿執行緒訊息
	SYSTEM_SAVE_FRAME_MERGE_THREAD_LOG,//是否儲存影像合併執行緒訊息
	SYSTEM_SAVE_FIELD_MERGE_THREAD_LOG,//是否儲存區域合併執行緒訊息
	SYSTEM_SAVE_FRAME_LOAD_THREAD_LOG,//是否儲存影像載入執行緒訊息
	SYSTEM_SAVE_REGION_CALC_THREAD_LOG,//是否儲存區域計算執行緒訊息
	SYSTEM_SAVE_SQUENCE_THREAD_LOG,//是否儲存系列執行緒訊息
	SYSTEM_SAVE_FIELD_FRAME_RELEASE_THREAD_LOG,//是否儲存視野影像釋放執行緒訊息
	SYSTEM_SAVE_ONLINE_INSPECTION_THREAD_LOG,//是否儲存線上檢測執行緒訊息
	SYSTEM_SAVE_REMOVE_FOLDER_THREAD_LOG,//是否儲存刪除資料夾執行緒訊息
	SYSTEM_SAVE_CONVEYER_PRE_RUN_THREAD_LOG,//是否儲存軌道自動運轉執行緒訊息
	SYSTEM_SAVE_ITS_PROC_THREAD_LOG,//是否儲存ITS運作執行緒訊息
	SYSTEM_SAVE_LOAD_REPAIR_FILE_THREAD_LOG,//是否儲存載入維修站檔案執行緒訊息
	SYSTEM_SAVE_REPAIR_RESULT_SIGNAL_THREAD_LOG,//是否儲存維修站訊息執行緒訊息
	SYSTEM_SAVE_SIMPLE_JOB_THREAD_LOG,//是否儲存簡易作業執行緒訊息
	SYSTEM_SAVE_HASI_MONITOR_THREAD_LOG,//是否儲存HASI監視執行緒訊息
	SYSTEM_SAVE_TEST_OBJECT_FINISH_LOG,//是否儲存檢測物件結束訊息	
	SYSTEM_SAVE_UI_DRAW_FUNC_LOG,//是否儲存介面重繪函式訊息		
	SYSTEM_SAVE_USER_OPERATION_LOG,//是否儲存使用者操作訊息
	SYSTEM_SAVE_TEST_TRACK_FILE,//是否儲存檢測追蹤檔案
	SYSTEM_SHOW_MEMORY_LEAK_MESSAGE,//顯示記憶體未釋放訊息

	//File	
	SYSTEM_SAVE_PROJECT_REPORT_TEXT,    //是否專案文字檔報告		
	SYSTEM_SAVE_PROJECT_REPORT_TEXT_FILENAME_MODE,//儲存專案文字檔報告檔名 
	SYSTEM_SAVE_CUSTOMER_REPORT_FILE,   //是否儲存客戶報告模式
	SYSTEM_SAVE_PROJECT_REPORT_WND_READING,    //是否儲存專案檢測框數據檔案		

	SYSTEM_PRE_LOAD_PROJECT_OFFLINE_IMAGE,//提前載入專案離線圖檔	
	SYSTEM_AUTO_RELEASE_FIELD_FRAME_BUFFER,//自動釋放區域影像		
	SYSTEM_AUTO_RELEASE_OFFLINE_FIELD_FRAME_BUFFER,//自動釋放離線編程區域影像

	//Phase相位 SYSTEM_PHASE_  SYSTEM_PHASE_NOISE_ 
	SYSTEM_PHASE_PERIOD_1,//第1個相位的週期
	SYSTEM_PHASE_PERIOD_2,//第2個相位的週期
	SYSTEM_PHASE_PERIOD_3,//第3個相位的週期
	SYSTEM_SEPARATE_DLP_2EXP_TABLE,//分離DLP2次曝光表格
	SYSTEM_PHASE_CONVERT_HEIGHT_MODE, //相位轉換高度模式		
	SYSTEM_PHASE_NOISE_DEFINE,//相位雜訊定義啟用
	SYSTEM_PHASE_NOISE_DEFINE_MODE,//相位雜訊定義模式
	SYSTEM_PHASE_NOISE_EXTEND_VOID,//相位遮罩-無效點外擴
	SYSTEM_PHASE_NOISE_LOW_CONTRAST,//相位遮罩-低對比
	SYSTEM_PHASE_NOISE_LOW_POTENTIAL,//相位遮罩-低潛力	
	SYSTEM_PHASE_NOISE_OVER_SATURATED,//相位遮罩-過亮度	

	SYSTEM_PHASE_NOISE_LOW_CONTRAST_B,//相位遮罩-低對比
	SYSTEM_PHASE_NOISE_LOW_POTENTIAL_B,//相位遮罩-低潛力	
	SYSTEM_PHASE_NOISE_OVER_SATURATED_B,//相位遮罩-過亮度	
	SYSTEM_PHASE_NOISE_LOW_CONTRAST_C,//相位遮罩-低對比
	SYSTEM_PHASE_NOISE_LOW_POTENTIAL_C,//相位遮罩-低潛力	
	SYSTEM_PHASE_NOISE_OVER_SATURATED_C,//相位遮罩-過亮度	

	SYSTEM_PHASE_NOISE_SMOOTH_FILTER,//相位遮罩-平滑處理
	SYSTEM_PHASE_NOISE_LOW_CONTRAST_COLOR,//相位遮罩-低對比-顏色	
	SYSTEM_PHASE_NOISE_LOW_POTENTIAL_COLOR,//相位遮罩-潛力-顏色
	SYSTEM_PHASE_NOISE_OVER_SATURATED_COLOR,//相位遮罩-過亮度-顏色		
	SYSTEM_PHASE_NOISE_EXTEND_VOID_COLOR,//相位遮罩-無效點外擴-顏色
	SYSTEM_SPACE_NOISE_HEIGHT_UNEXPECTED_COLOR,//相位遮罩-高度異常-顏色
	SYSTEM_SPACE_NOISE_HEIGHT_OVER_LOW_COLOR,//相位遮罩-過低異常-顏色
	SYSTEM_SPACE_VALID_BEST_COLOR,//相位遮罩-可靠高度-顏色	
	
	SYSTEM_3D_OBJECT_DRAW_SCALE_X,//3D物件顯示比例-X		
	SYSTEM_3D_OBJECT_DRAW_SCALE_Y,//3D物件顯示比例-Y
	SYSTEM_3D_OBJECT_DRAW_SCALE_Z,//3D物件顯示比例-Z

	SYSTEM_PANEL_COLOR_1,//整板的顏色-1
	SYSTEM_PANEL_COLOR_2,//整板的顏色-2	
	SYSTEM_PANEL_TEXT_COLOR,//整板文字顏色	
	SYSTEM_PANEL_SELECTED_COLOR,//整板選取到的顏色
	SYSTEM_BOARD_COLOR_1,//單板的顏色-1
	SYSTEM_BOARD_COLOR_2,//單板的顏色-2	
	SYSTEM_BOARD_TEXT_COLOR,//單板文字顏色	
	SYSTEM_BOARD_SELECTED_COLOR,//單板選取到的顏色
	SYSTEM_FD_COLOR_1,//定位點的顏色-1
	SYSTEM_FD_COLOR_2,//定位點的顏色-2		
	SYSTEM_FD_TEXT_COLOR,//定位點文字顏色
	SYSTEM_FD_SELECTED_COLOR,//定位點選取到的顏色
	SYSTEM_BARCODE_COLOR_1,//條碼的顏色-1
	SYSTEM_BARCODE_COLOR_2,//條碼的顏色-2		
	SYSTEM_BARCODE_TEXT_COLOR,//條碼文字顏色
	SYSTEM_BARCODE_SELECTED_COLOR,//條碼選取到的顏色
	SYSTEM_COMPONENT_COLOR_1,//零件的顏色-1
	SYSTEM_COMPONENT_COLOR_2,//零件的顏色-2		
	SYSTEM_COMPONENT_TEXT_COLOR,//零件文字顏色
	SYSTEM_COMPONENT_SELECTED_COLOR,//零件選取到的顏色
	SYSTEM_DISTRICT_COLOR_1,//分段的顏色-1
	SYSTEM_DISTRICT_COLOR_2,//分段的顏色-2
	SYSTEM_INSPECTED_RESULT_OK_COLOR,//檢測OK顏色
	SYSTEM_INSPECTED_RESULT_NG_COLOR,//檢測NG顏色
	SYSTEM_INSPECTED_RESULT_SKIP_COLOR,//檢測Skip顏色
	SYSTEM_INSPECTED_RESULT_BYPASS_COLOR,//檢測Bypass顏色
	SYSTEM_INSPECTED_RESULT_UNTEST_COLOR,//檢測UnTest顏色
	SYSTEM_INSPECTED_RESULT_WARNING_COLOR,//檢測Warning顏色		
	SYSTEM_INSPECTED_RESULT_EXCEPTION_COLOR,//檢測Exception顏色
	
	//空間高度	
	SYSTEM_SPACE_NOISE_SINGLE_CAST_LOW_LIMIT,//高度雜訊單投光高度最低極限-um		
	SYSTEM_SPACE_NOISE_MULTI_CAST_PATCH_SIZE,//高度雜訊多投光合併Patch尺寸
	SYSTEM_SPACE_NOISE_MULTI_CAST_MERGE_MODE,//高度雜訊多投光合併模式
	SYSTEM_SPACE_NOISE_MULTI_CAST_MERGE_BEST_MODE,//高度雜訊多投光合併最可靠模式
	SYSTEM_SPACE_NOISE_MULTI_INTENSITY_MERGE_MODE,//高度雜訊多亮度合併模式-Mean, MaxB
	SYSTEM_SPACE_NOISE_MULTI_CAST_MIN_VALID_COUNT,//高度雜訊多投光最少有效值數	
	SYSTEM_SPACE_NOISE_MULTI_CAST_MAX_DIFFERENCE,//高度雜訊多投光高度最大差值-um	
	SYSTEM_SPACE_NOISE_MULTI_CAST_LIMIT_DIFFERENCE,//高度雜訊多投光高度極限差值-um		
	SYSTEM_SPACE_NOISE_MULTI_CAST_VALID_BEST_RATIO,//高度雜訊多投光高度最好比例-um	
	SYSTEM_SPACE_NOISE_MULTI_CAST_VALID_DIFFERENCE,//高度雜訊多投光高度有效差值-um
	SYSTEM_SPACE_NOISE_MULTI_CAST_OPPOSITE_MAX_GRAY,//高度雜訊多投光合併對邊灰階上限-gray	

	SYSTEM_SPACE_NOISE_CAST_FILTER_MODE,//投光後濾波模式//20240904-Joe
	SYSTEM_SPACE_NOISE_CAST_MEDIAN_FILTER_SIZE,//高度雜訊的中值濾波尺寸
	SYSTEM_SPACE_NOISE_CAST_MEDIAN_FILTER_USE_SIZE,//高度雜訊的中值濾波使用尺寸	

	SYSTEM_SPACE_NOISE_DATA_VOID_EXPAND_SIZE,//空間雜訊無效點外擴尺寸-piexel	
	SYSTEM_SPACE_NOISE_DATA_VOID_EXPAND_ENABLED,//空間雜訊無效點外擴啟用
	SYSTEM_SPACE_NOISE_FIRST_FILTER_MODE,//空間雜訊首次濾波模式
	SYSTEM_SPACE_NOISE_FIRST_FILTER_PITCH,//空間雜訊首次濾波步長	
	SYSTEM_SPACE_NOISE_FIRST_KER_SIZE,//空間雜訊首次濾波尺寸
	SYSTEM_SPACE_NOISE_FIRST_USE_SIZE,//空間雜訊首次使用尺寸
	SYSTEM_SPACE_NOISE_OVER_LOW_MODE,//高度雜訊過低模式
	SYSTEM_SPACE_NOISE_OVER_LOW_RANGE,//高度雜訊過低高度-um	
	SYSTEM_SPACE_NOISE_OVER_LOW_LIMIT,//高度雜訊過低極限-um	
	SYSTEM_SPACE_NOISE_OVER_LOW_KER_SIZE,//高度雜訊過低濾波尺寸
	SYSTEM_SPACE_NOISE_OVER_LOW_USE_SIZE,//高度雜訊過低使用尺寸
	SYSTEM_SPACE_NOISE_HEIGHT_ABNORMAL_MODE,//高度雜訊高度異常模式
	SYSTEM_SPACE_NOISE_HEIGHT_ABNORMAL_PITCH,//高度雜訊高度異常步長
	SYSTEM_SPACE_NOISE_HEIGHT_ABNORMAL_RANGE,//高度雜訊高度異常範圍-um			
	SYSTEM_SPACE_NOISE_HEIGHT_ABNORMAL_CHK_SIZE,//高度雜訊高度異常確認尺寸
	SYSTEM_SPACE_NOISE_HEIGHT_ABNORMAL_KER_SIZE,//高度雜訊高度異常濾波尺寸
	SYSTEM_SPACE_NOISE_HEIGHT_ABNORMAL_USE_SIZE,//高度雜訊高度異常使用尺寸	
	SYSTEM_SPACE_NOISE_HEIGHT_ABNORMAL_REPEAT_CNT,//高度雜訊高度異常重複次數		
	SYSTEM_SPACE_NOISE_RECONTRUCTED_EXT_SIZE,//空間雜訊重建外擴尺寸
	SYSTEM_SPACE_NOISE_RECONTRUCTED_ENABLED,//空間雜訊重建外擴啟用		
	SYSTEM_SPACE_NOISE_FINAL_FILTER_MODE,//空間雜訊最後濾波模式
	SYSTEM_SPACE_NOISE_FINAL_FILTER_PITCH,//空間雜訊最後濾波步長
	SYSTEM_SPACE_NOISE_FINAL_KER_SIZE,//空間雜訊最後濾波尺寸
	SYSTEM_SPACE_NOISE_FINAL_USE_SIZE,//空間雜訊最後使用尺寸
	SYSTEM_SPACE_NOISE_FINAL_FILTER_MODE2,//空間雜訊最後濾波模式-2
	SYSTEM_SPACE_NOISE_FINAL_FILTER_PITCH2,//空間雜訊最後濾波步長-2
	SYSTEM_SPACE_NOISE_FINAL_KER_SIZE2,//空間雜訊最後濾波尺寸-2
	SYSTEM_SPACE_NOISE_FINAL_USE_SIZE2,//空間雜訊最後使用尺寸-2

	SYSTEM_SPACE_VERIFY_MULTI_CAST_GAP_RATIO,//高度驗證-多投光差距比例上限-%
	SYSTEM_SPACE_VERIFY_MULTI_CAST_GAP_THRESHOLD,//高度驗證-多投光差距閥值-um		

	//Cuda SYSTEM_CUDA_
	SYSTEM_CUDA_FUNCTION_ENABLED,//Cuda函式啟用
	SYSTEM_CUDA_NUMBER_BLOCK,    //Cuda Block數量
	SYSTEM_CUDA_NUMBER_THREAD,    //Cuda Thread數量	

	//Setting SYSTEM_	
	SYSTEM_MACHINE_MODEL_TYPE,//設備機種樣式
	SYSTEM_MACHINE_CAMERA_SIDE,//設備相機方向	
	SYSTEM_MULTI_LANGUAGE_MODE,//多國語系版本
	SYSTEM_CAMERA_DEBAYER_MODE,//影像還元彩色模式	
	SYSTEM_CONNECT_LAST_STATION_MODE,//與上一站連線方式		
	SYSTEM_LANE_WORK_MODEL_LA,//A軌道運轉模式
	SYSTEM_LANE_WORK_MODEL_LB,//B軌道運轉模式

	SYSTEM_MULTI_TOWER_LIGHT_MODE,//多塔燈模式	
	SYSTEM_CHECK_PCB_REMOVED_COUNT,//確認PCB板移走次數
	SYSTEM_NEXT_CONNECTED_BUFFER_TYPE,//確認PCB下一站連接輸送帶樣式		
	SYSTEM_MULTI_PROJECT_TEST_ORDER_MODE,//多專案檢測次序模式

	SYSTEM_ONLINE_INPUT_PROJECT_WORK_NUMBER,//在線輸入-專案工單號碼

	SYSTEM_ONLINE_AUTO_CALIBRATION_DLP_LED_COLOR,//在線自動校正DLP-LED-顏色
	SYSTEM_ONLINE_AUTO_CALIBRATION_CAP_DELAY_TIME,//在線自動校正上蓋延遲時間-ms
	SYSTEM_ONLINE_AUTO_CALIBRATION_MODE_XYZ_HOME,//在線自動校正模式-XYZ歸零
	SYSTEM_ONLINE_AUTO_CALIBRATION_PERIOD_XYZ_HOME,//在線自動校正週期-XYZ歸零-小時
	SYSTEM_ONLINE_AUTO_CALIBRATION_MODE_2D_CURRENT,//在線自動校正模式-2D電流
	SYSTEM_ONLINE_AUTO_CALIBRATION_PERIOD_2D_CURRENT,//在線自動校正週期-2D電流-小時
	SYSTEM_ONLINE_AUTO_CALIBRATION_PERIOD_3D_CURRENT,//在線自動校正週期-3D電流-小時		
	SYSTEM_ONLINE_AUTO_CALIBRATION_MODE_3D_ZERO_PLANE,//在線自動校正模式-3D相平面
	SYSTEM_ONLINE_AUTO_CALIBRATION_PERIOD_3D_ZERO_PLANE,//在線自動校正週期-3D相平面-小時
	SYSTEM_ONLINE_AUTO_CALIBRATION_PERIOD_3D_HEIGHT_FACTOR,//在線自動校正週期-3D高度比例-小時

	SYSTEM_ONLINE_AUTO_STOP_BY_IDLE_TIME,//在線自動停機-閒置時間-分鐘
	SYSTEM_ONLINE_AUTO_STOP_BY_SPEC_TIME_1,//在線自動停機-特定時間-時時分分秒秒
	SYSTEM_ONLINE_AUTO_STOP_BY_SPEC_TIME_2,//在線自動停機-特定時間-時時分分秒秒
	SYSTEM_ONLINE_AUTO_STOP_BY_SPEC_TIME_3,//在線自動停機-特定時間-時時分分秒秒	
	SYSTEM_AUTO_SWITCH_TO_ONILINEVIEW_TIME,      //自動切換線上畫面-秒	
	SYSTEM_ONLINE_SHOW_PROJECT_TEST_MAP,      //在線顯示專案檢測底圖
	SYSTEM_AUTO_SWITCH_TO_ONILINE_REMOTE_CTRL_TIME,//自動切換線上遠端控制時間-毫秒
	SYSTEM_AUTO_SWITCH_TO_ONILINE_REMOTE_CTRL_MODE,//自動切換線上遠端控制模式

	SYSTEM_AUTO_RETRY_MAX_COUNT,//自動重測次數上限
	SYSTEM_AUTO_SETUP_DLP_PATTERN,//自動設定DLP樣板		

	SYSTEM_RESOLUTION_MODE,//解析度模式
	SYSTEM_RESOLUTION_SHOW_SCALE,//解析度顯示比例
	SYSTEM_MOVE_CAMERA_BEFORE_PCB_IN,//進板前移動相機頭
	SYSTEM_PCB_IN_USE_CAMERA_IMAGE_MODE,//進板使用相機影像模式
	SYSTEM_CLAMP_PCB_BEFORE_TEST_MODE,//檢測前夾板模式	
	SYSTEM_GRAB_FIDUCIAL_DELAY_TIME,//解取定位點影像延遲時間
	
	SYSTEM_PCB_OUT_DIRECTION,                  //PCB出板方向
	SYSTEM_BYPASS_LAST_SIGNAL,                 //忽略上一站訊號-回板使用
	SYSTEM_BYPASS_NEXT_SIGNAL,                 //忽略下一站訊號-回板使用	
	SYSTEM_PCB_OK_NG_SIGNAL_DELAY_TIME,        //PCB OK/NG訊號延遲時間-ms

	/*
	int                        m_MemoryKeepMode;//記憶體維持模式
	PHASE_UNWRAP_MODE          m_UnwrappingMode;//是否使用unwrapping	
	*/	
	SYSTEM_EDIT_LINE_SIZE_LEVEL,                 //編輯線尺寸的層級	
	SYSTEM_ONLINE_TUNING_KEEP_MAX_TIME,          //線上調機保留最久時間-分鐘 	
	SYSTEM_ONLINE_TUNING_SAVED_MAX_COUNT,        //線上調機儲存最多數量-片數 
	SYSTEM_USER_LOGIN_ENABLED,                   //使用者登入使用		
	SYSTEM_USER_LOGIN_OPTIONS,                   //使用者登入選項	

	SYSTEM_OPEN_PROJECT_MODE,                    //開啟專案模式
	SYSTEM_OPEN_PROJECT_MAP_INDEX,               //開啟專案底圖編號	

	SYSTEM_VERIFY_PROJECT_MODE,                  //驗證專案模式
	SYSTEM_VERIFY_PROJECT_FILENAME,              //驗證專案的檔名

	SYSTEM_MODEL_NAME_USE_PART_NUMBER,           //模組名稱使用料號 		

	SYSTEM_ONLINE_OPEN_PROJECT_MODE,             //線上開專案模式		
	SYSTEM_ONLINE_OPEN_PROJECT_CAMERA_BARCODE,   //線上開專案-相機條碼		
	SYSTEM_ONLINE_OPEN_PROJECT_BARCODE_DEVICE_GRAB_MODE,//線上開專案外接條碼取像模式

	SYSTEM_SAVE_PROJECT_SPC_FILE_MODE,           //儲存專案SPC檔案模式		
	SYSTEM_SAVE_PROJECT_SPC_LIBRARY_MODE,        //儲存專案SPC資料庫模式	

	SYSTEM_MODEL_UNSET_COLOR,                    //模組未設定顏色
	SYSTEM_BINARY_MASK_COLOR_RGB,                //2值化遮罩-顏色-3色燈
	SYSTEM_BINARY_MASK_COLOR_DEFAULT,            //2值化遮罩-顏色-預設
	SYSTEM_BINARY_MASK_COLOR_ALPHA,              //2值化遮罩-顏色-透明度		
	SYSTEM_INSPECTION_FINISH_SHOW_RESULT_LIST,   //檢測結束顯示結果列表 
	SYSTEM_MODEL_DEFAULT_WND_LEVEL,              //模組預設檢測框等級
	SYSTEM_SWITCH_PROJECT_3D_FRAME,              //可切換至專案3D畫面	
	SYSTEM_AUTO_SWITCH_WND_3D_FRAME_MODE,        //自動切換至檢測框3D畫面模式
	SYSTEM_AUTO_SWITCH_WND_3D_FRAME_SIZE_LIMIT,  //自動切換至檢測框3D畫面尺寸上限-um		
	SYSTEM_SHOW_COMPONENT_FULL_MAP,              //顯示零件-整板零件
	SYSTEM_SHOW_COMPONENT_DEFECT_ONLY,           //僅顯示瑕疵零件(線上畫面)
	SYSTEM_SHOW_DEBUG_FORM_VIEW,                 //顯示除錯頁面 			
	SYSTEM_LEVEL_FILTER_SHIFT_ENABLED,           //分等濾波起點偏移啟用
	SYSTEM_MEDIAN_FILTER_SHIFT_ENABLED,          //中值濾波起點偏移啟用
	SYSTEM_SMOOTH_FILTER_SHIFT_ENABLED,          //平滑濾波起點偏移啟用
	SYSTEM_PYRAMID_MEDIAN_FILTER_SHIFT_ENABLED,  //金字塔中值濾波起點偏移啟用
	SYSTEM_LEVEL_FILTER_PITCH_TOLERANCE,         //分等濾波步長可接受誤差
	SYSTEM_MEDIAN_FILTER_PITCH_TOLERANCE,        //中值濾波步長可接受誤差
	SYSTEM_SMOOTH_FILTER_PITCH_TOLERANCE,        //平滑濾波步長可接受誤差	
	SYSTEM_PYRAMID_MEDIAN_FILTER_PITCH_TOLERANCE,//金字塔中值濾波步長可接受誤差
	SYSTEM_GRR_OFFSET_RANDOM_VALUE,              //Grr偏移隨機補償值
	SYSTEM_GRR_AVERAGE_RESET_ENABLED,            //Grr平均值復歸-啟用-依據條碼比較
	SYSTEM_GRR_AVERAGE_RESET_MATCH_RATIO,        //Grr平均值復歸-匹配比例	
	SYSTEM_GRR_SKEW_AVERAGE_ENB_VAL,             //Grr偏移角度平均啟用角度
	SYSTEM_GRR_SKEW_AVERAGE_START_GAP,           //Grr偏移角度平均開始差距-角度
	SYSTEM_GRR_SKEW_AVERAGE_WEIGHTING,           //Grr偏移角度平均權重 
	SYSTEM_GRR_OFFSET_AVERAGE_ENB_PXL,           //Grr偏移平均啟用像素 
	SYSTEM_GRR_OFFSET_AVERAGE_START_GAP,         //Grr偏移平均開始差距-微米	
	SYSTEM_GRR_OFFSET_AVERAGE_WEIGHTING,         //Grr偏移平均權重 
	SYSTEM_GRR_HEIGHT_AVERAGE_ENB_VAL,           //Grr偏移高度平均啟用高度
	SYSTEM_GRR_HEIGHT_AVERAGE_START_GAP,         //Grr偏移高度平均開始差距-微米
	SYSTEM_GRR_HEIGHT_AVERAGE_WEIGHTING,         //Grr偏移高度平均權重 
	SYSTEM_LOCK_SCREEN_ENABLED,                  //鎖住螢幕啟用
	SYSTEM_LOCK_SCREEN_KEY_ID,                   //鎖住螢幕鍵號
	SYSTEM_ONLINE_TUNING_ENABLE_BARCODE,         //在線調機-軟體條碼
	SYSTEM_MULTI_DISTRICT_MODE_ENABLED,          //多段檢測啟用		
	SYSTEM_SHOW_PLC_SAFTY_SETTING_UI,            //是否顯示PLC安全檢知設定介面
	SYSTEM_SHOW_ALG_OFFSET_L_PARAM,              //是否顯示演算法OffsetL的參數 
	SYSTEM_SHOW_ALG_OFFSET_A_PARAM,              //是否顯示演算法OffsetA的參數 	
	SYSTEM_SHOW_ALG_BRIGHT_RATIO_SCALE_PARAM,    //是否顯示演算法亮度比例的比例參數		
	SYSTEM_SHOW_MODEL_PROPERTY_PARAM,            //是否顯示模組屬性參數	
	SYSTEM_COPY_HUGE_FILES_MODE,                 //複製大量檔案模式		
	SYSTEM_CONTINUE_PASTE_MODE,                  //連續貼上模式
	SYSTEM_STITCH_IMAGE_PADDING_SIZE,            //拼圖參數-填補尺寸
	SYSTEM_RESIN_HEIGHT_ALIGN_ENABLED,           //啟用Resin高度對齊-軍達3D對位	
	SYSTEM_MODEL_IMAGE_CAD_OFFSET_ENABLED,       //啟用模組影像Cad偏移補償um
	SYSTEM_MODEL_DEFAULT_TRANSISTOR_TYPE,        //模組預設SOT樣式		
	SYSTEM_PROJECT_LOCAL_FOLDER_ENABLED,         //專案使用本機資料夾		
	SYSTEM_PARTIAL_COPY_PROJECT_LIBRARY,         //部分複製專案資料庫		
	SYSTEM_AUTO_ARRANGE_MODEL_BK_IMAGE_FILES,    //自動重整模組底圖檔案
	SYSTEM_AUTO_COPY_SPC_COMPONENT_IMAGE_FILES,  //自動複製SPC零件圖檔 
	SYSTEM_AUTO_BYPASS_GRAB_3D_FRAME,            //自動跳過3D影像
	SYSTEM_CHECK_PROJECT_FD_READY,               //確認專案定位點狀態
	SYSTEM_LOCK_MODEL_BODY_POSITION,             //鎖住模組本體的位置	
	SYSTEM_MAX_UNCHECK_TEST_FILE_COUNT,          //最多未判定檢測檔案數 
	SYSTEM_CONFIRM_COMPONENT_BARCODE_ENABLED,    //啟用零件條碼確認
	SYSTEM_SAVE_JPEG_QUALITY,                    //儲存JPEG的質量(001~100) 

	SYSTEM_OFFLINE_VERSION_MODE,                 //離線版本模式				
	SYSTEM_USE_PROJECT_SYSTEM_PARAM_MODE,        //使用專案系統參數模式

	SYSTEM_UI_WND_FONT_ADD_SIZE,                 //UI視窗字型增加大小
	SYSTEM_UI_DOCK_WND_SLIDE_STEPS,              //UI駐停視窗滑動步長 
	SYSTEM_UI_ENABLE_PCB_OUT_BUTTON,             //UI啟用PCB出板按鈕 
		
	SYSTEM_RABBITMQ_SERVER_PORT,                 //RabbitMQ伺服器Port
	SYSTEM_RABBITMQ_IP_ADDRESS,                  //RabbitMQ伺服器網址
	SYSTEM_RABBITMQ_USER_NAME,                   //RabbitMQ登錄名稱
	SYSTEM_RABBITMQ_PASSWORD,                    //RabbitMQ登錄密碼

	SYSTEM_ITS_FILENAME,                         //ITS軟體名稱
	SYSTEM_ITS_COMMUNICATION_MODE,               //ITS通訊模式
	SYSTEM_ITS_SOCKET_IP_PORT,                   //ITS網路Port
	SYSTEM_ITS_SOCKET_IP_ADDRESS,                //ITS網路網址
	SYSTEM_ITS_RABBITMQ_QUEUE_NAME_RECV,         //ITS訊息佇列接收名
	SYSTEM_ITS_RABBITMQ_QUEUE_NAME_SEND,         //ITS訊息佇列傳送名
	SYSTEM_ITS_CONTACT_SOFTWARE,                 //ITS-對接軟體
	SYSTEM_ITS_COMMUNICATION_ENABLED,            //連線至ITS啟用
	SYSTEM_ITS_COMMUNICATION_TIMEOUT_MS,         //與ITS溝通逾時(ms)	
	SYSTEM_ITS_FILE_FOLDER_SEND,                 //ITS檔案資料夾-傳送
	SYSTEM_ITS_FILE_FOLDER_RECV,                 //ITS檔案資料夾-接收
	SYSTEM_ITS_FILE_BACKUP_ENABLED,              //ITS檔案資料夾-備份
	SYSTEM_ITS_FILE_USE_SYNC_FILE_ENABLED,       //ITS檔案資料夾-同步檔案	
	SYSTEM_ITS_SET_SECS_GEM_ENABLED,             //ITS使用設定SECS/GEM
	SYSTEM_ITS_SECS_GEM_REMOTE_LOCAL,            //ITS使用SECS/GEM-Remote Local
	SYSTEM_ITS_SET_SYSTEM_PARAM_ENABLED,         //ITS使用設定系統參數
	SYSTEM_ITS_SET_PROJECT_PARAM_ENABLED,        //ITS使用設定專案參數
	SYSTEM_ITS_SET_MACHINE_STATUS_ENABLED,       //ITS使用設定機台狀態
	SYSTEM_ITS_SET_APP_OPEN_CLOSE_ENABLED,       //ITS使用設定軟體開關
	SYSTEM_ITS_SET_PROCESS_ID_ENABLED,           //ITS使用設定程序編號
	SYSTEM_ITS_SET_USER_LOGIN_OUT_ENABLED,       //ITS使用設定使用者登入登出	
		
	SYSTEM_PROG_DEFAULT_SPACE_TO_GRAY_RATIO_MODE,//專案預設高度轉灰階比例 
	SYSTEM_PROG_DEFAULT_SPACE_BASE_PLANE_INDEX,//專案預設空間基準面編號 
	SYSTEM_PROG_DEFAULT_SPACE_NOISE_FILTER_INDEX,//專案預設空間雜訊過濾編號
	SYSTEM_PROG_DEFAULT_ENABLE_CONVEYER_PRE_RUN, //專案預設軌道提前運轉功能
	SYSTEM_PROG_DEFAULT_FD_NG_HANDLE_MODE,       //專案預設定位點異常處理模式
	SYSTEM_PROG_DEFAULT_BOARD_FD_GRAB_MODE,      //專案預設單板定位點取像模式
	SYSTEM_PROG_DEFAULT_BDEFECT_HANDLE_MODE,     //專案預設檢出異常處理模式
	SYSTEM_PROG_DEFAULT_PCB_OUT_MODE,            //專案預設PCB出板模式
	SYSTEM_PROG_DEFAULT_PROJECT_SAVE_TEST_MAP,   //專案預設儲存檢測底圖模式
	SYSTEM_PROG_DEFAULT_PROJECT_LINK_SERVER_MODE,//專案預設連線伺服器模式
	SYSTEM_PROG_DEFAULT_SAVE_OFFLINE_IMAGE_FILES,//專案預設儲存離線圖檔
	SYSTEM_PROG_DEFAULT_BARCODE_VERIFY_MODE,     //專案預設條碼驗證模式
	SYSTEM_PROG_DEFAULT_BARCODE_RETRIEVE_MODE,   //專案預設條碼查詢模式

	SYSTEM_OPERATE_LEVEL_PROJECT_OPEN,            //操作等級-專案開啟
	SYSTEM_OPERATE_LEVEL_PROJECT_SAVE,            //操作等級-專案儲存
	SYSTEM_OPERATE_LEVEL_PROJECT_PARAM,           //操作等級-專案參數
	SYSTEM_OPERATE_LEVEL_ONLINE_RUN,              //操作等級-線上運行
	SYSTEM_OPERATE_LEVEL_ONLINE_BYPASS,           //操作等級-線上直通
	SYSTEM_OPERATE_LEVEL_ONLINE_STOP,             //操作等級-線上停止
	SYSTEM_OPERATE_LEVEL_ONLINE_SAVE_IMAGE,       //操作等級-線上存圖
	SYSTEM_OPERATE_LEVEL_ONLINE_UNLOCK,           //操作等級-線上解鎖		
	SYSTEM_OPERATE_LEVEL_EDIT_FUNC_ADD,           //操作等級-編輯功能-新增
	SYSTEM_OPERATE_LEVEL_EDIT_FUNC_DEL,           //操作等級-編輯功能-刪除
	SYSTEM_OPERATE_LEVEL_EDIT_FUNC_BYPASS,        //操作等級-編輯功能-不檢測	
	SYSTEM_OPERATE_LEVEL_MES_CTRL_STATE_OFFLINE,  //操作等級-MES相關-控制狀態-離線
	SYSTEM_OPERATE_LEVEL_MES_CTRL_STATE_LOCAL,    //操作等級-MES相關-控制狀態-本地上線
	SYSTEM_OPERATE_LEVEL_MES_CTRL_STATE_REMOTE,   //操作等級-MES相關-控制狀態-遠端上線
	SYSTEM_OPERATE_LEVEL_MES_SHOW_CONTEXT,        //操作等級-MES相關-顯示內容

	SYSTEM_M2M_NPM_BARCODE_ENABLE,                  //啟用NPM-條碼
	SYSTEM_M2M_NPM_APC_FF1_ENABLE,                  //啟用NPM APC-FF1
	SYSTEM_M2M_NPM_APC_FF2_ENABLE,                  //啟用NPM APC-FF2
	SYSTEM_M2M_NPM_APC_MFB_ENABLE,                  //啟用NPM APC-MFB
	SYSTEM_M2M_NPM_LANE_NAME_LA,                    //NPM軌道名稱-A軌
	SYSTEM_M2M_NPM_LANE_NAME_LB,                    //NPM軌道名稱-B軌		
	SYSTEM_M2M_NPM_INPUT_SHARE_FOLDER_LA,           //NPM-輸入共享資料夾-A軌
	SYSTEM_M2M_NPM_INPUT_SHARE_FOLDER_LB,           //NPM-輸入共享資料夾-B軌
	SYSTEM_M2M_NPM_OUTPUT_SHARE_FOLDER_LA,          //NPM-輸出共享資料夾-A軌
	SYSTEM_M2M_NPM_OUTPUT_SHARE_FOLDER_LB,          //NPM-輸出共享資料夾-B軌

	SYSTEM_M2M_HASI_ENABLE,                         //啟用 HAS I (Hanwha AOI Solution)
	SYSTEM_M2M_HASI_SHARE_FOLDER,					//HAS I 共享資料夾
	SYSTEM_M2M_HASI_SERIALFILE_ENABLE,			    //啟用 HAS I - SerialFile
	SYSTEM_M2M_HASI_SERIALFILE_QUEUESIZE,			//SerialFile- Size of Serial Queue
	SYSTEM_M2M_HASI_SERIALFILE_DWELLTIME,			//SerialFile- 等待資料的延遲時間-ms
	SYSTEM_M2M_HASI_SPIOFFSETFILE_ENABLE,           //啟用 HAS I - SPIOffsetFile
	SYSTEM_M2M_HASI_PNP_ENABLE,                     //啟用 HAS I - PNP File
	SYSTEM_M2M_HASI_AOI_STAGE,						//設定 HAS I 開啟爐前/爐後功能
	SYSTEM_M2M_HASI_STATE_MODE,

	SYSTEM_AI_MODEL_SERVER_ENABLE,                  //AI模型伺服器啟用
	SYSTEM_AI_MODEL_SERVER_TIMEOUT,                 //AI模型伺服器逾時		
	SYSTEM_AI_MODEL_SERVER_FILENAME,                //AI模型伺服器檔名
	SYSTEM_AI_MODEL_FILE_FOLDER_SEND,               //AI模型檔案資料夾-傳送
	SYSTEM_AI_MODEL_FILE_FOLDER_RECV,               //AI模型檔案資料夾-接收
	SYSTEM_AI_MODEL_LABEL_MIN_CLUSTER_DISTANCE,     //AI模型分類最小叢集距離-um

	SYSTEM_EXTERNAL_COPY_FILE_ENABLED,              //外部複製檔案啟用
	SYSTEM_EXTERNAL_COPY_FILE_APP_NAME,             //外部複製檔案軟體名稱
	SYSTEM_EXTERNAL_COPY_FILE_SEND_FOLDER,          //外部複製檔案輸出資料夾	

	SYSTEM_CPK_CHART_ENABLED,                       //Cpk圖表啟用
	SYSTEM_WND_ROTATION_FOLLOWED,                   //檢測框跟隨旋轉//Alan

	SYSTEM_PARAM_END
};
//-------------------------------------------------------------------------------------//
typedef struct tagSystemParameter
{
	CString                    m_AOIDirectory;//系統資料夾
	CString                    m_AOIBaseTempDirectory;//暫存的資料夾
	CString                    m_AOILogDirectory;//系統訊息資料夾	
	CString                    m_AOIProjectFolder;//專案資料夾
	CString                    m_AOIResultFolder;//結果資料夾			
	CString                    m_AOIServerFolder;//伺服器資料夾
	CString                    m_AOIStaticDataFolder;//統計資料資料夾	
	CString                    m_AOITextReportFolder;//文字報告資料夾
	CString                    m_OnlineTuningFolder;//線上調機資料夾
	CString                    m_OnlineBarcodeFolder;//線上條碼資料夾
	CString                    m_OnlineOfflineFolder;//線上離線編程資料夾	
	CString                    m_ProjectTestMapFolder;//專案檢測底圖資料夾	
	CString                    m_ProjectTestMapFolderTemp;//專案檢測底圖資料夾-暫存
	CString                    m_ProjectTestMapFolderBackup;//專案檢測底圖資料夾-備份
	CString                    m_ProjectRawFolder;//專案原圖資料夾
	CString                    m_ProjectDebugFolder;//專案除錯資料夾		
	CString                    m_MonitorStatusFolder;//監控狀態資料夾		
	CString                    m_CustomerLogFolder;//客戶訊息資料夾	
	CString                    m_CustomerReportFolder;//客戶報告資料夾
	CString                    m_BarcodeFileFolder_LA;//條碼檔案資料夾-A軌
	CString                    m_BarcodeFileFolder_LB;//條碼檔案資料夾-B軌
	CString                    m_AIFileExportFolder;//AI檔案輸出資料夾
	CString                    m_AIImageExportFolder;//AI影像輸出資料夾
	CString                    m_AOITestTrackFolder;//檢測追蹤資料夾

	CString                    m_LibraryHost;//本機資料庫
	CString                    m_LibraryRemote;//遠端資料庫		
	
	int                        m_SaveMovingTimeMessage;//是否移動時間狀態訊息
	int                        m_BackupMovingTimeMessage;//是否備份移動時間狀態訊息
	int                        m_SaveCurrentProcessMessage;//是否儲存現在狀態訊息
	int                        m_BackupCurrentProcessMessage;//是否備份現在狀態訊息
	int                        m_SaveMachineMonitorMessage;//是否儲存機台監控訊息
	int                        m_SaveMachineMonitorMessageWeb;//是否儲存機台監控訊息-網頁版
	int                        m_SaveCameraAddRingBufferLog;//是否儲存相機增加循環資料訊息
	int                        m_ShowMemoryLeakMessage;//顯示記憶體未釋放訊息
	int                        m_SendDebugViewString;//是否送至DebugView視窗
	int                        m_SaveSliceFillThreadLog;//是否儲存影像填滿執行緒訊息
	int                        m_SaveFrameMergeThreadLog;//是否儲存影像合併執行緒訊息
	int                        m_SaveFieldMergeThreadLog;//是否儲存區域合併執行緒訊息	
	int                        m_SaveFrameLoadThreadLog;//是否儲存影像載入執行緒訊息
	int                        m_SaveRegionCalcThreadLog;//是否儲存區域計算執行緒訊息
	int                        m_SaveSequenceThreadLog;//是否儲存系列執行緒訊息
	int                        m_SaveFieldFrameReleaseThreadLog;//是否儲存視野影像釋放執行緒訊息
	int                        m_SaveOnlineInspectionThreadLog;//是否儲存線上檢測執行緒訊息
	int                        m_SaveRemoveFolderThreadLog;//是否儲存刪除資料夾執行緒訊息
	int                        m_SaveConveyerPreRunThreadLog;//是否儲存軌道自動運轉執行緒訊息
	int                        m_SaveITSProcThreadLog;//是否儲存ITS運作執行緒訊息
	int                        m_SaveLoadRepairFileThreadLog;//是否儲存載入維修站檔案執行緒訊息
	int                        m_SaveRepairResultSignalThreadLog;//是否儲存維修站訊息執行緒訊息
	int                        m_SaveSimpleJobThreadLog;//是否儲存簡易作業執行緒訊息	
	int                        m_SaveHASIMonitorThreadLog;//是否儲存HASI檢測執行緒訊息
	int                        m_SaveTestObjectFinishLog;//是否儲存檢測物件結束訊息
	int                        m_SaveUIDrawFuncLog;//是否儲存介面重繪函式訊息
	int                        m_SaveUserOperationLog;//是否儲存使用者操作訊息
	int                        m_SaveTestTrackFile;//是否儲存檢測足跡檔案

	int                        m_SaveProjectReportText;//是否儲存專案文字檔報告
	SAVE_TEXT_FILENAME_MODE    m_SaveProjectReportTextFilenameMode;//儲存專案文字檔報告檔名
	int                        m_SaveCustomerReportFile;//是否儲存客戶報告模式
	int                        m_SaveProjectReportWndReading;//是否儲存專案檢測框數據檔案	
	SAVE_SPC_FILE_MODE         m_SaveProjectSpcFileMode;//專案儲存SPC檔案模式
	SAVE_SPC_OTHER_FILE_MODE   m_SaveProjectSpcLibraryMode;//專案儲存SPC資料庫模式
	
	int                        m_PreLoadProjectOfflineImage;//提前載入專案離線圖檔
	int                        m_AutoReleaseFieldFrameBuffer;//自動釋放區域影像	
	int                        m_AutoReleaseOfflineFieldFrameBuffer;//自動釋放離線編程區域影像

	CString                    m_AppVersion;//軟件版本
	CString                    m_IPHostComputer;//本機網路網址	

	CString                    m_MachineLocation;//本機廠區
	CString                    m_MachineBuilding;//本機棟別
	CString                    m_MachineFloor;//本機樓層
	CString                    m_MachineRoom;//本機車間
	CString                    m_MachineLine;//本機線別
	CString                    m_MachineLine_LB;//本機線別-B軌
	CString                    m_MachineStation;//本機站別	
	CString                    m_MachineStation_LB;//本機站別-B軌	
	CString                    m_MachineSN;//本機序號	
	CString                    m_MachineName;//機台型號
	CString                    m_MachineVendor;//機台廠商
	CString                    m_MachineAlias;//機台別名

	CString                    m_MachineMES_Name;//MES登入名稱
	CString                    m_MachineMES_Password;//MES登入密碼
	CString                    m_MachineMES_Device;//MES登入裝置
	CString                    m_MachineMES_Device2;//MES登入裝置-2
	CString                    m_MachineMES_CodeName;//MES裝置代號

	AOI_CUSTOMER_ID            m_AOICustomerID;//客戶編號	
	
	MACHINE_MODEL_TYPE         m_MachineModelType;//設備機種樣式
	MACHINE_CAMERA_SIDE        m_MachineCameraSide;//設備相機方向	
	MULTI_LANGUAGE_MODE        m_MultiLanguageMode;//多國語系版本
	int                        m_StretchBltMode;//繪圖模式
	IMAGE_DEBAYER_MODE         m_DebayerMode;//影像還元彩色模式			
	

	PHASE_UNWRAP_MODE          m_UnwrappingMode;//是否使用unwrapping
	IMAGE_DISPLAY_MODE         m_ImageDisplayMode;//影像顯示模式
	IMAGE_DISPLAY_ENHANCE_MODE m_ImageDisplayEnhanceMode;//影像顯示強化模式
	double                     m_ImageDisplayGain;//影像顯示的Gain
	double                     m_ImageDisplayGamma;//影像顯示的Gamma	
	int                        m_ImageDisplaySharpnessRadius;//影像顯示的銳利化的半徑
	int                        m_ImageDisplaySharpnessAmount;//影像顯示的銳利化的總量-%
	int                        m_ImageDisplaySharpnessThreshold;//影像顯示的銳利化的閥值
	int                        m_ImageDisplayLocalGammaCalcSize;//影像顯示的局部Gamma的計算範圍
	double                     m_ImageDisplayLocalGammaScaleVal;//影像顯示的局部Gamma的縮放比例
	double                     m_ImageDisplayMaxZoomScale;//影像顯示最大縮放比例
	ONLINE_FROMVIEW_MODE       m_OnlineFormViewMode;//線上檢測介面模式	

	int                        m_AutoRetryMaxCount;//自動重測次數上限	
	int                        m_AutoSetupDlpPattern;//自動設定DLP樣板

	int                        m_ResolutionModeTmp;//解析度模式	
	double                     m_ResolutionShowScale;//解析度顯示比例	
	
	int                        m_CpuMaxCountUsed;//Cpu最多使用數量
	int                        m_MTCount_SliceFill;//多執行緒數量-畫面填圖	
	int                        m_MTCount_FrameMerge;//多執行緒數量-影像合併
	int                        m_MTCount_FieldMerge;//多執行緒數量-區域合併
	int                        m_MTCount_FrameLoad;//多執行緒數量-影像載入
	int                        m_MTCount_RegionCalc;//多執行緒數量-區域計算	
	int                        m_MTCount_ProcIdle;//多執行緒數量-閒置核心
	int                        m_MTCount_GrabIdle;//多執行緒數量-取像閒置

	int                        m_OpenMPCount_General;//OpenMP核心數量-一般操作
	int                        m_OpenMPCount_Inspection;//OpenMP核心數量-檢測中
	int                        m_OpenMPCheckSize_Inspection;//OpenMP確認尺寸-檢測中
	
	double                     m_PhasePeriod1;//第1個相位的週期
	double                     m_PhasePeriod2;//第2個相位的週期
	double                     m_PhasePeriod3;//第3個相位的週期
	int                        m_SeparateDLP2ExpTable;//分離DLP2次曝光表格
	int                        m_PhaseConvertHeightMode;//相位轉換高度模式
	int                        m_PhaseNoiseDefine;	
	int                        m_PhaseNoiseDefineMode;//相位雜訊定義模式
	int                        m_PhaseNoiseLowContrastA;//相位遮罩-低對比
	int                        m_PhaseNoiseLowPotentialA;//相位遮罩-潛力
	int                        m_PhaseNoiseOverSaturatedA;//相位遮罩-過亮度
	int                        m_PhaseNoiseLowContrastB;//相位遮罩-低對比
	int                        m_PhaseNoiseLowPotentialB;//相位遮罩-潛力
	int                        m_PhaseNoiseOverSaturatedB;//相位遮罩-過亮度
	int                        m_PhaseNoiseLowContrastC;//相位遮罩-低對比
	int                        m_PhaseNoiseLowPotentialC;//相位遮罩-潛力
	int                        m_PhaseNoiseOverSaturatedC;//相位遮罩-過亮度
	int                        m_PhaseNoiseExtendVoid;//相位遮罩-無效點外擴
	int                        m_PhaseNoiseSmoothFilter;//相位遮罩-平滑處理
	
	COLORREF                   m_PhaseNoiseLowContrastColor;//相位遮罩-低對比-顏色
	COLORREF                   m_PhaseNoiseLowPotentialColor;//相位遮罩-潛力-顏色
	COLORREF                   m_PhaseNoiseOverSaturatedColor;//相位遮罩-過亮度-顏色		
	COLORREF                   m_PhaseNoiseExtendVoidColor;//相位遮罩-無效點外擴-顏色
	COLORREF                   m_SpaceNoiseHeightUnexpectedColor;//相位遮罩-高度異常-顏色	
	COLORREF                   m_SpaceNoiseHeightOverLowColor;//相位遮罩-過低異常-顏色				
	COLORREF                   m_SpaceBestValidColor;//相位遮罩-可靠高度-顏色	
	
	double                     m_3DObjectDrawScaleX;//3D物件縮放比例-X
	double                     m_3DObjectDrawScaleY;//3D物件縮放比例-Y
	double                     m_3DObjectDrawScaleZ;//3D物件縮放比例-Z

	COLORREF                   m_PanelColor1;//整板的顏色-1
	COLORREF                   m_PanelColor2;//整板的顏色-2
	COLORREF                   m_PanelTextColor;//整板文字顏色
	COLORREF                   m_PanelSelectedColor;//整板選取到的顏色
	COLORREF                   m_BoardColor1;//單板的顏色-1
	COLORREF                   m_BoardColor2;//單板的顏色-2
	COLORREF                   m_BoardTextColor;//單板文字顏色
	COLORREF                   m_BoardSelectedColor;//單板選取到的顏色
	COLORREF                   m_FdColor1;//定位點的顏色-1
	COLORREF                   m_FdColor2;//定位點的顏色-2
	COLORREF                   m_FdTextColor;//定位點文字顏色
	COLORREF                   m_FdSelectedColor;//定位點選取到的顏色
	COLORREF                   m_BarcodeColor1;//條碼的顏色-1
	COLORREF                   m_BarcodeColor2;//條碼的顏色-2
	COLORREF                   m_BarcodeTextColor;//條碼文字顏色
	COLORREF                   m_BarcodeSelectedColor;//條碼選取到的顏色
	COLORREF                   m_ComponentColor1;//零件的顏色-1
	COLORREF                   m_ComponentColor2;//零件的顏色-2
	COLORREF                   m_ComponentTextColor;//零件文字顏色
	COLORREF                   m_ComponentSelectedColor;//零件選取到的顏色
	COLORREF                   m_DistrictColor1;//分段的顏色-1
	COLORREF                   m_DistrictColor2;//分段的顏色-2
	COLORREF                   m_InspectedResultOKColor;//檢測OK顏色
	COLORREF                   m_InspectedResultNGColor;//檢測NG顏色
	COLORREF                   m_InspectedResultSkipColor;//檢測Skip顏色
	COLORREF                   m_InspectedResultBypassColor;//檢測Bypass顏色
	COLORREF                   m_InspectedResultUnTestColor;//檢測UnTest顏色
	COLORREF                   m_InspectedResultWarningColor;//檢測Warning顏色
	COLORREF                   m_InspectedResultExceptionColor;//檢測Exception顏色

	double                     m_SpaceNoiseSingleCastLowLimit;//高度雜訊單投光高度最低極限-um	
	int                        m_SpaceNoiseMultiCastPatchSize;//高度雜訊多投光合併Patch尺寸
	int                        m_SpaceNoiseMultiCastMergeMode;//高度雜訊多投光合併模式-MASS, Lower
	int                        m_SpaceNoiseMultiCastMergeBestMode;//高度雜訊多投光合併最可靠模式-2,3
	int                        m_SpaceNoiseMultiIntensityMergeMode;//高度雜訊多亮度合併模式-Mean, MaxB
	int                        m_SpaceNoiseMultiCastMinValidCount;//高度雜訊多投光最少有效值數
	double                     m_SpaceNoiseMultiCastMaxDifference;//高度雜訊多投光高度最大差值-um
	double                     m_SpaceNoiseMultiCastLimitDifference;//高度雜訊多投光高度極限差值-um	
	double                     m_SpaceNoiseMultiCastValidBestRatio;//高度雜訊多投光高度最好比例-um	
	double                     m_SpaceNoiseMultiCastValidDifference;//高度雜訊多投光高度有效差值-um	
	int                        m_SpaceNoiseMultiCastOppositeMaxGray;//高度雜訊多投光合併對邊灰階上限-gray

	int                        m_SpaceNoiseCastFilterMode;//投光後濾波模式//20240904-Joe
	int                        m_SpaceNoiseCastMedianFilterSize;//高度雜訊的中值濾波尺寸
	int                        m_SpaceNoiseCastMedianFilterUseSize;//高度雜訊的中值濾波使用尺寸

	int                        m_SpaceMergeRecursionMode;//空間合併遞迴模式
	int                        m_SpaceMergeRecursionMaxCount;//空間合併遞迴最多次數
	int                        m_SpaceMergeRecursionKernelSize;//空間合併遞迴鄰居尺寸	
	int                        m_SpaceMergeRecursionMaskSize;//空間合併遞迴鄰居忽略尺寸	

	int                        m_SpaceNoiseDataVoidExpandSize;//空間雜訊無效點外擴尺寸-piexel
	int                        m_SpaceNoiseDataVoidExpandEnabed;//空間雜訊無效點外擴啟用	
	int                        m_SpaceNoiseFirstFilterMode;//空間雜訊首次濾波模式
	int                        m_SpaceNoiseFirstFilterPitch;//空間雜訊首次濾波步長
	int                        m_SpaceNoiseFirstKerSize;//空間雜訊首次平滑尺寸
	int                        m_SpaceNoiseFirstUseSize;//空間雜訊首次使用尺寸
	int						   m_SpaceNoiseFirstFilterAlphaF;//Content Aware濾波器尋找相似高度之標準差
	int						   m_SpaceNoiseFirstFilterAlphaS;//Content Aware濾波器("鄰域"與"相似"高度)之標準差
	int						   m_SpaceNoiseFirstFilterAlphaM;//Content Aware濾波器("鄰域"與"中間"高度)之標準差
	int						   m_SpaceNoiseFirstFilterAlphaI;//Content Aware濾波器挑選影像(Gray)相似之標準差
	int					       m_SpaceNoiseFirstFilterThresdhold_Outlier;//Content Aware濾波器Outlier的閥值(0 - 1)
	bool				       m_SpaceNoiseFirstFilterSearchOn;//Content Aware濾波器尋找相似高度之功能,關閉後可加速運算(差異不大)
	int                        m_SpaceNoiseOverLowerMode;//高度雜訊過低模式
	int                        m_SpaceNoiseOverLowerRange;//高度雜訊過低高度-um		
	int                        m_SpaceNoiseOverLowerLimit;//高度雜訊過低極限-um		
	int                        m_SpaceNoiseOverLowerKerSize;//高度雜訊過低濾波尺寸
	int                        m_SpaceNoiseOverLowerUseSize;//高度雜訊過低使用尺寸
	int                        m_SpaceNoiseHeightAbnormalMode;//高度雜訊高度異常模式
	int                        m_SpaceNoiseHeightAbnormalPitch;//高度雜訊高度異常步長
	int                        m_SpaceNoiseHeightAbnormalRange;//高度雜訊高度異常範圍-um		
	int                        m_SpaceNoiseHeightAbnormalChkSize;//高度雜訊高度異常確認尺寸
	int                        m_SpaceNoiseHeightAbnormalKerSize;//高度雜訊高度異常濾波尺寸
	int                        m_SpaceNoiseHeightAbnormalUseSize;//高度雜訊高度異常使用尺寸	
	int                        m_SpaceNoiseHeightAbnormalRepeatCnt;//高度雜訊高度異常重複次數
	int                        m_SpaceNoiseDataVoidReContructedExtSize;//空間雜訊重建外擴尺寸
	int                        m_SpaceNoiseDataVoidReContructedEnabled;//空間雜訊重建外擴啟用		
	int                        m_SpaceNoiseFinalFilterMode;//空間雜訊最後濾波模式
	int                        m_SpaceNoiseFinalPitch;//空間雜訊最後濾波步長
	int                        m_SpaceNoiseFinalKerSize;//空間雜訊最後平滑尺寸
	int                        m_SpaceNoiseFinalUseSize;//空間雜訊最後使用尺寸
	int						   m_SpaceNoiseFinalFilterAlphaF;//Content Aware濾波器尋找相似高度之標準差
	int						   m_SpaceNoiseFinalFilterAlphaS;//Content Aware濾波器("鄰域"與"相似"高度)之標準差
	int						   m_SpaceNoiseFinalFilterAlphaM;//Content Aware濾波器("鄰域"與"中間"高度)之標準差
	int						   m_SpaceNoiseFinalFilterAlphaI;//Content Aware濾波器挑選影像(Gray)相似之標準差
	int					       m_SpaceNoiseFinalFilterThresdhold_Outlier;//Content Aware濾波器Outlier的閥值(0 - 1)
	bool				       m_SpaceNoiseFinalFilterSearchOn;//Content Aware濾波器尋找相似高度之功能,關閉後可加速運算(差異不大)
	int                        m_SpaceNoiseFinalFilterMode2;//空間雜訊最後濾波模式-2
	int                        m_SpaceNoiseFinalPitch2;//空間雜訊最後濾波步長-2
	int                        m_SpaceNoiseFinalKerSize2;//空間雜訊最後平滑尺寸-2
	int                        m_SpaceNoiseFinalUseSize2;//空間雜訊最後使用尺寸-2
	int						   m_SpaceNoiseFinalFilterAlphaF2;//Content Aware濾波器尋找相似高度之標準差
	int						   m_SpaceNoiseFinalFilterAlphaS2;//Content Aware濾波器("鄰域"與"相似"高度)之標準差
	int						   m_SpaceNoiseFinalFilterAlphaM2;//Content Aware濾波器("鄰域"與"中間"高度)之標準差
	int						   m_SpaceNoiseFinalFilterAlphaI2;//Content Aware濾波器挑選影像(Gray)相似之標準差
	int					       m_SpaceNoiseFinalFilterThresdhold_Outlier2;//Content Aware濾波器Outlier的閥值(0 - 1)
	bool				       m_SpaceNoiseFinalFilterSearchOn2;//Content Aware濾波器尋找相似高度之功能,關閉後可加速運算(差異不大)

	double                     m_SpaceVerifyMultiCastGapRatio;//高度驗證-多投光差距比例上限-%
	double                     m_SpaceVerifyMultiCastGapThreshold;//高度驗證-多投光差距閥值-um

	int                        m_PhaseSmoothCudaMode;//
	int                        m_PhaseSmoothCudaMaskSize;

	int                        m_CudaFnEnabled;//Cuda函式啟用
	int                        m_CudaBlockNumber;//Cuda Block數量
	int                        m_CudaThreadNumber;//Cuda Thread數量

	int                        m_CalcFocusSmoothSize;//計算焦距平滑處理
	CALC_FOCUS_MODE            m_CalcFocusMode;//計算焦距模式

	int                        m_MemoryKeepMode;//記憶體維持模式
	JET_MATCH_LIB_TYPE         m_MatchLibType;  //影像匹配函式庫樣式	
	int                        m_HoneywellSwiftDecoderEnabled;//Honeywell SwiftDecoder啟用

	int                        m_DongleWarningRemainingDays;//硬體鎖警告剩餘天數
	int                        m_DongleWarningRemainingCount;//硬體鎖警告剩餘次數

	int                        m_LastStationLineMode;//與上一站連線方式	
	LANE_WORK_MODE             m_LaneWorkMode_LA;//A軌道運轉模式
	LANE_WORK_MODE             m_LaneWorkMode_LB;//B軌道運轉模式
	int                        m_MultiTowerLight;//雙塔燈模式
	int                        m_CheckPCBRemovedCount;//確認PCB移除次數
	CONNECTED_BUFFER_TYPE      m_NextConnectedBufferType;//下一站連接輸送帶樣式
	MULTI_PROJECT_TEST_ORDER_MODE  m_MultiProjectTestOrderMode;//多專案檢測次序模式

	ONLINE_INPUT_TIMING        m_OnlineInputProjectWorkNumber;//在線輸入專案工單號碼

	int                        m_OnlineAutoCalibrationDlpLedColor;//在線自動校正DLP-LED顏色
	DWORD                      m_OnlineAutoCalibrationCapDelayTime;//在線自動校正上蓋延遲時間-ms
	FUNC_EXEC_MODE             m_OnlineAutoCalibrationMode_XYZHome;//在線自動校正模式-XYZ歸零
	int                        m_OnlineAutoCalibrationPeriod_XYZHome;//在線自動校正週期-XYZ歸零-小時	
	FUNC_EXEC_MODE             m_OnlineAutoCalibrationMode_2DCurrent;//在線自動校正模式-2D電流
	int                        m_OnlineAutoCalibrationPeriod_2DCurrent;//在線自動校正週期-2D電流-小時
	int                        m_OnlineAutoCalibrationPeriod_3DCurrent;//在線自動校正週期-3D電流-小時
	FUNC_EXEC_MODE             m_OnlineAutoCalibrationMode_3DZeroPlane;//在線自動校正模式-3D相平面
	int                        m_OnlineAutoCalibrationPeriod_3DZeroPlane;//在線自動校正週期-3D相平面-小時	
	int                        m_OnlineAutoCalibrationPeriod_3DHeightFactor;//在線自動校正週期-3D高度比例-小時

	int                        m_OnlineAutoStopByIdleTime;//在線自動停機-閒置時間-分鐘
	__int64                    m_OnlineAutoStopBySpecTime1;//在線自動停機-特定時間-時時分分秒秒
	__int64                    m_OnlineAutoStopBySpecTime2;//在線自動停機-特定時間-時時分分秒秒
	__int64                    m_OnlineAutoStopBySpecTime3;//在線自動停機-特定時間-時時分分秒秒
	int                        m_AutoSwitchToOnlineViewTime;//自動切換線上畫面-秒	
	int                        m_OnlineShowProjectTestMap;//在線顯示專案檢測底圖
	int                        m_AutoSwitchToOnlineRemoteCtrlTime;//自動切換線上遠端控制時間-毫秒	
	MES_EQP_CTRL_STATE_MODE    m_AutoSwitchToOnlineRemoteCtrlMode;//自動切換線上遠端控制模式

	int                        m_MoveCameraBeforePCBIn;//進板前移動相機頭
	int                        m_PCBInUseCameraImageMode;//進板使用相機影像模式
	FUNC_EXEC_MODE             m_ClampPcbBeforeTestMode;//檢測前夾板模式
	DWORD                      m_GrabFiducialDelayTime_ms;//解取定位點影像延遲時間
	PCB_OUT_DIRECTION          m_PCBOutDirection;//PCB出板方向
	int                        m_BypassLastSignal;//忽略上一站訊號-回板使用
	int                        m_BypassNextSignal;//忽略下一站訊號-回板使用
	int                        m_PCBOKNGSignalDelayTime;;//PCB OK/NG訊號延遲時間-ms
	int                        m_EditLineSizeLevel;//編輯線尺寸的層級	
	int                        m_OnlineTuningKeepMaxTime;//線上調機保留最久時間-分鐘
	int                        m_OnlineTuningSavedMaxCount;//線上調機儲存最多數量-片數
	USER_LOGIN_MODE            m_UserLoginMode;//使用者登入模式	
	USER_LOGIN_OPTIONS         m_UserLoginOptions;//使用者登入選項

	OPEN_PROJECT_MODE          m_OpenProjectMode;//開專案模式
	int                        m_OpenProjectMapIndex;//開專案底圖編號		

	VERIFY_PROJECT_MODE        m_VerifyProjectMode;//驗證專案模式
	CString                    m_VerifyProjectFilename;//驗證專案的檔名

	int                        m_ModelNameUsePartNumber;//模組名稱使用料號

	ONLINE_OPEN_PROJECT_MODE   m_OnlineOpenProjectMode;//線上開專案模式
	int                        m_OnlineOpenProjectCameraBarcode;//線上開專案模式-相機條碼
	BARCODE_DEVICE_GRAB_MODE   m_OnlineOpenProjectBarcodeDeviceGrabMode;//線上開專案外接條碼取像模式

	COLORREF                   m_ModelUnsetColor;//模組未設定顏色
	COLORREF                   m_BinaryMaskColorRGB;//2值化遮罩顏色-3色燈
	COLORREF                   m_BinaryMaskColorDefault;//2值化遮罩顏色-預設	
	int                        m_BinaryMaskColorAlpha;//2值化遮罩顏色-透明度
	int                        m_InspectionFinishShowResultList;//檢測結束顯示結果列表
	int                        m_ModelDefaultWndLevel;//模組預設檢測框等級;//
	int                        m_SwitchProject3DFrame;//可切換至專案3D畫面	
	AUTO_SWITCH_WND_3D_FRAME_MODE m_AutoSwitchWnd3DFrameMode;//自動切換至檢測框3D畫面模式
	int                        m_AutoSwitchWnd3DFrameSizeLimit;//自動切換至檢測框3D畫面尺寸上限-um
	int                        m_ShowComponentFullMap;//顯示零件-整板零件
	int                        m_ShowComponentDefectOnly;//僅顯示瑕疵零件(線上畫面)
	int                        m_ShowDebugFormView;//顯示除錯頁面
	int                        m_LevelFilterShiftEnabled;//分等濾波起點偏移啟用
	int                        m_MedianFilterShiftEnabled;//中值濾波起點偏移啟用
	int                        m_SmoothFilterShiftEnabled;//平滑濾波起點偏移啟用
	int                        m_PyramidMedianFilterShiftEnabled;//金字塔中值濾波起點偏移啟用
	int                        m_LevelFilterPitchTolerance;//分等濾波步長可接受誤差
	int                        m_MedianFilterPitchTolerance;//中值濾波步長可接受誤差
	int                        m_SmoothFilterPitchTolerance;//平滑濾波步長可接受誤差
	int                        m_PyramidMedianFilterPitchTolerance;//金字塔中值濾波步長可接受誤差
	int                        m_GrrOffsetRandomValue;//Grr偏移隨機補償值-nm	
	int                        m_GrrAverageResetEnabled;//Grr平均值復歸-啟用-依據條碼比較
	float                      m_GrrAverageResetMatchRatio;//Grr平均值復歸-匹配比例
	float                      m_GrrSkewAverageEnbVal;//Grr偏移平均啟用數值-角度
	float                      m_GrrSkewAverageStartGap;//Grr偏移角平均開始差距-角度
	int                        m_GrrSkewAverageWeighting;//Grr偏移角度權重
	int                        m_GrrOffsetAverageEnbPxl;//Grr偏移平均啟用像素
	float                      m_GrrOffsetAverageStartGap;//Grr偏移平均開始差距-微米 
	int                        m_GrrOffsetAverageWeighting;//Grr偏移平均權重
	float                      m_GrrHeightAverageEnbVal;//Grr偏移高度啟用數值-微米
	float                      m_GrrHeightAverageStartGap;//Grr偏移高度開始差距-微米 
	int                        m_GrrHeightAverageWeighting;//Grr偏移高度權重
	int                        m_LockScreenEnabled;//啟用鎖住螢幕
	int                        m_LockScreenKeyID;//鎖住螢幕鍵號
	int                        m_MultiDistrictModeEnabled;//多段檢測啟用
	int                        m_OnlineTuningEnableBarcode;//在線調機-軟體條碼
	int                        m_ShowPlcSaftySettingUI;//是否顯示PLC安全檢知設定介面
	int                        m_ShowAlgOffsetLParam;//是否顯示演算法OffsetL的參數 
	int                        m_ShowAlgOffsetAParam;//是否顯示演算法OffsetA的參數
	int                        m_ShowAlgBrightRatioScaleParam;//是否顯示演算法亮度比例的比例參數
	int                        m_ShowModelPropertyParam;//是否顯示模組屬性參數
	int                        m_ContinuePasteMode;//連續貼上模式	
	int                        m_StitchImagePaddingSize;//拼圖參數-填補尺寸
	int                        m_ResinHeightAlignEnabled;//啟用Resin高度對齊-軍達3D對位
	int                        m_ModelImageCadOffsetEnabled;//啟用模組影像Cad偏移補償um
	MODEL_TYPE                 m_ModelDefaultTransistorType;//模組預設SOT樣式
	COPY_HUGE_FILES_MODE       m_CopyHugeFilesMode;//複製大量檔案模式	
	int                        m_ProjectLocalFolderEnabled;//專案使用本機資料夾
	int                        m_PartialCopyProjectLibrary;//部分複製專案資料庫
	int                        m_AutoArrangeModelBkImageFiles;//自動重整模組底圖檔案
	int                        m_AutoCopySpcComponentImageFiles;//自動複製SPC零件圖檔
	int                        m_AutoBypassGrab3DFrame;//自動跳過取3D影像
	int                        m_CheckProjectFdReady;//確認專案定位點狀態	
	int                        m_LockModelBodyPosition;//鎖住模組本體的位置	
	int                        m_MaxUncheckTestFileCount;//最多未判定檢測檔案數
	int                        m_ConfirmComponentBarcodeEnabled;//啟用零件條碼確認
	int                        m_SaveJpegQuality;//儲存JPEG的質量(001~100)

	//Offline
	OFFLINE_VERSION_MODE       m_OfflineVersionMode;//離線版本模式
	int                        m_UseProjectSystemParamMode;//使用專案系統參數模式	


	int                        m_UIWndFontAddSize;//UI視窗字型增加大小
	int                        m_UIDockWndSlideSteps;//UI駐停視窗滑動步長 
	int                        m_UIEnablePCBOutButton;;//UI啟用PCB出板按鈕 
	
	int                        m_RabbitMQServerPort;//RabbitMQ伺服器Port
	CString                    m_RabbitMQServerAddress;//RabbitMQ伺服器位址
	CString                    m_RabbitMQUserName;//RabbitMQ登錄名稱
	CString                    m_RabbitMQPassword;//RabbitMQ登錄密碼

	CString                    m_ITSFilename;//ITS軟體名稱
	ITS_COMMUNICATION_MODE     m_ITSCommunicationMode;//ITS通訊模式
	int                        m_ITSSocketIPPort;//ITS網路Port
	CString                    m_ITSSocketIPAddress;//ITS網路位址	
	CString                    m_ITSRabbitMQRecvQueueName;//ITS訊息佇列接收名
	CString                    m_ITSRabbitMQSendQueueName;//ITS訊息佇列傳送名
	int                        m_ITSContactSoftware;//ITS-對接軟體
	int                        m_ITSCommunicationEnabled;//連線至ITS啟用	
	int                        m_ITSCommunicationTimeoutMS;//與ITS溝通逾時(ms)	
	CString                    m_ITSFileFolderSend;//ITS檔案資料夾-傳送
	CString                    m_ITSFileFolderRecv;//ITS檔案資料夾-接收
	bool                       m_ITSFileBackupEnabled;//ITS檔案資料夾-備份	
	bool                       m_ITSFileUseSyncFileEnabled;//ITS檔案資料夾-同步檔案	
	int                        m_ITSSetSecsGemEnabled;//ITS使用設定SECS/GEM
	int                        m_ITSSetSecsGemRemoteLocal;//ITS使用SECS/GEM-Remote Local
	int                        m_ITSSetSystemParamEnabled;//ITS使用設定系統參數
	int                        m_ITSSetProjectParamEnabled;//ITS使用設定專案參數
	int                        m_ITSSetMacineStatusEnabled;//ITS使用設定機台狀態
	int                        m_ITSSetAppOpnCloseEnabled;//ITS使用設定軟體開關
	int                        m_ITSSetProcessIDEnabled;//ITS使用執行程序編號
	int                        m_ITSSetUserLogin_outEnabled;//ITS使用設定使用者登入登出	

	int                        m_DefaultSpaceToGrayRatioMode;//專案預設高度轉灰階比例
	int                        m_DefaultSpaceBasePlaneIndex;//專案預設空間基準面編號
	int                        m_DefaultSpaceNoiseFilterIndex;//專案預設空間雜訊過濾編號
	int                        m_DefaultEnableConveyerPreRun;//專案預設軌道提前運轉功能
	FD_NG_HANDLE_MODE          m_DefaultFdNGHandleMode;//專案預設定位點異常處理模式
	BOARD_FD_GRAB_MODE         m_DefaultBoardFdGrabMode;//專案預設單板定位點取像模式
	DEFECT_HANDLE_MODE         m_DefaultDefectHandleMode;//專案預設檢出異常處理模式
	PCB_OUT_MODE               m_DefaultPCBOutMode;//專案預設PCB出板模式
	SAVE_TEST_MAP_MODE         m_DefaultProjectSaveTestMap;//專案預設儲存檢測底圖模式
	PROJECT_LINK_SERVER_MODE   m_DefaultProjectLinkServerMode;//專案預設連線伺服器模式	
	bool                       m_DefaultProjectSaveOfflineImageFiles;//專案預設儲存離線圖檔
	int                        m_DefaultBarcodeVerifyMode;//專案預設條碼驗證模式
	int                        m_DefaultBarcodeRetrieveMode;//專案預設條碼查詢模式

	USER_LEVEL_MODE            m_OperateLevelProjectOpen;//操作等級-專案開啟
	USER_LEVEL_MODE            m_OperateLevelProjectSave;//操作等級-專案儲存
	USER_LEVEL_MODE            m_OperateLevelProjectParam;//操作等級-專案參數
	USER_LEVEL_MODE            m_OperateLevelOnlineRun;//操作等級-線上運行
	USER_LEVEL_MODE            m_OperateLevelOnlineBypass;//操作等級-線上直通
	USER_LEVEL_MODE            m_OperateLevelOnlineStop;//操作等級-線上停止
	USER_LEVEL_MODE            m_OperateLevelOnlineSaveImage;//操作等級-線上存圖
	USER_LEVEL_MODE            m_OperateLevelOnlineUnlock;//操作等級-線上解鎖	
	USER_LEVEL_MODE            m_OperateLevelEditFuncAdd;//操作等級-編輯功能-新增
	USER_LEVEL_MODE            m_OperateLevelEditFuncDel;//操作等級-編輯功能-刪除
	USER_LEVEL_MODE            m_OperateLevelEditFuncBypass;//操作等級-編輯功能-不檢測	
	USER_LEVEL_MODE            m_OperateLevelMesCtrlState_Offline;//操作等級-MES相關-控制狀態-離線
	USER_LEVEL_MODE            m_OperateLevelMesCtrlState_Local;//操作等級-MES相關-控制狀態-本地上線
	USER_LEVEL_MODE            m_OperateLevelMesCtrlState_Remote;//操作等級-MES相關-控制狀態-遠端上線
	USER_LEVEL_MODE            m_OperateLevelMesShowContext;//操作等級-MES相關-顯示內容

	//M2M Machine to Machine
	int                        m_M2M_NPM_Barcode_Enable;//啟用NPM-條碼
	int                        m_M2M_NPM_APC_FF1_Enable;//啟用NPM APC-FF1
	int                        m_M2M_NPM_APC_FF2_Enable;//啟用NPM APC-FF2
	int                        m_M2M_NPM_APC_MFB_Enable;//啟用NPM APC-MFB
	CString                    m_M2M_NPM_LaneName_LA;//NPM軌道名稱-A軌
	CString                    m_M2M_NPM_LaneName_LB;//NPM軌道名稱-B軌
	CString                    m_M2M_NPM_InputShareFolder_LA;//NPM-輸入共享資料夾-A軌
	CString                    m_M2M_NPM_InputShareFolder_LB;//NPM-輸入共享資料夾-B軌
	CString                    m_M2M_NPM_OutputShareFolder_LA;//NPM-輸出共享資料夾-A軌
	CString                    m_M2M_NPM_OutputShareFolder_LB;//NPM-輸出共享資料夾-B軌

	//M2M Machine to Machine - HAS I (Hanwha AOI Solution MAOI_SAOI)
	bool                       m_M2M_HASI_Enable;//啟用M2M HAS I
	CString                    m_M2M_HASI_ShareFolder;//HAS I 共享資料夾
	int                        m_M2M_HASI_SerialFile_Enable;//啟用 HAS I - SerialFile
	int                        m_M2M_HASI_SerialFile_QueueSize;//HAS I - SerialFile - Size of Serial Queue
	DWORD                      m_M2M_HASI_SerialFile_DwellTime;//等待資料的延遲時間-ms
	int                        m_M2M_HASI_SPIOffsetFile_Enable;//啟用 HAS I - SPIOffsetFile
	int                        m_M2M_HASI_PNP_Enable;//啟用 HAS I - PNP
	HASI_AOI_STAGE             m_M2M_HASI_AOI_Stage;//產線上 AOI 的檢查階段
	HASI_STATE_MODE            m_M2M_HASI_StateMode;//Hanwha傳遞的狀態

	//AI Model
	int                        m_AiModelServerEnabled;//AI模型伺服器啟用
	DWORD                      m_AiModelServerTimeout;//AI模型伺服器逾時
	CString                    m_AiModelServerFilename;//AI模型伺服器檔名
	CString                    m_AiModelFileFolderSend;//AI模型檔案資料夾-傳送
	CString                    m_AiModelFileFolderRecv;//AI模型檔案資料夾-接收
	double                     m_AIModelLabelMinClusterDistance;//AI模型分類最小叢集距離-um

	//External Copy File
	int                        m_ExternalCopyFileEnabled;//外部複製檔案啟用	
	CString                    m_ExternalCopyFileAppName;//外部複製檔案軟體名稱
	CString                    m_ExternalCopyFileSendFolder;//外部複製檔案輸出資料夾

	//CPK Chart
	int                        m_CpkChartEnabled;//Cpk圖表啟用
	bool                       m_WndRotationFollowed;//檢測框跟隨旋轉//Alan

	tagSystemParameter()
	{
		m_AOIDirectory = AOI3D_MAIN_FOLDER;//_T("C:\\JETAOI3D");
		m_AOILogDirectory = AOI3D_FOLDER("\\Log");//_T("C:\\JETAOI3D\\Log");			
		m_AOIBaseTempDirectory = AOI3D_FOLDER("\\Temp");//_T("C:\\JETAOI3D\\Temp");	
		m_AOIProjectFolder = _T("D:\\AOI3DProject");//專案資料夾
		m_AOIResultFolder  = _T("D:\\AOI3DResult");//結果資料夾
		m_AOIServerFolder = _T("D:\\AOI3DServer");//資料庫資料夾
		m_AOIStaticDataFolder = _T("D:\\AOI3DStaticFile");//統計資料資料夾
		m_AOITextReportFolder = _T("D:\\AOI3DTextReport");
		m_OnlineTuningFolder = _T("D:\\AOI3DTuning");//線上調機資料夾
		m_OnlineBarcodeFolder = _T("D:\\AOI3DBarcode");//線上條碼資料夾
		m_OnlineOfflineFolder = _T("D:\\AOI3DOffline");//線上離線編程資料夾
		m_ProjectTestMapFolder = _T("D:\\AOI3DTestMap");//專案檢測底圖資料夾
		m_ProjectTestMapFolderTemp = AOI3D_FOLDER("\\TempMap");//_T("C:\\JETAOI3D\\TempMap");//專案檢測底圖資料夾-暫存		
		m_ProjectTestMapFolderBackup = AOI3D_FOLDER("\\TestMap");//_T("C:\\JETAOI3D\\TestMap");//專案檢測底圖資料夾-備份
		m_ProjectRawFolder = _T("D:\\AOI3DRaw");//專案原圖資料夾	
		m_ProjectDebugFolder = _T("D:\\AOI3DDebug");//專案除錯資料夾	
		m_MonitorStatusFolder = _T("D:\\AOI3DMonitor");//監控狀態資料夾	
		m_CustomerLogFolder = AOI3D_FOLDER("\\Log");//_T("C:\\JETAOI3D\\Log");//客戶訊息資料夾
		m_CustomerReportFolder = _T("D:\\AOI3DCustomer");//客戶報告資料夾
		m_BarcodeFileFolder_LA = _T("D:\\AOI3DBarcodeFileA");//條碼檔案資料夾-A軌
		m_BarcodeFileFolder_LB = _T("D:\\AOI3DBarcodeFileB");//條碼檔案資料夾-B軌		
		m_AIFileExportFolder = _T("D:\\AOI3D\\AIFileFolder");//AI檔案輸出資料夾
		m_AIImageExportFolder = _T("D:\\AOI3D\\AIImageFolder");//AI影像輸出資料夾
		m_AOITestTrackFolder = _T("D:\\AOI3D\\TestTrackFolder");

		m_LibraryHost = _T("D:\\AOI3DLibrary\\Library.LIB");//資料庫本機
		m_LibraryRemote = _T("D:\\AOI3DLibrary\\Library.LIB");//資料庫遠端		
		
		m_SaveMovingTimeMessage = FN_DISABLE;
		m_BackupMovingTimeMessage = FN_DISABLE;
		m_SaveCurrentProcessMessage = FN_DISABLE;	
		m_BackupCurrentProcessMessage = FN_DISABLE;	
		m_SaveMachineMonitorMessage = FN_DISABLE;
		m_SaveMachineMonitorMessageWeb = FN_DISABLE;
		m_SaveCameraAddRingBufferLog = FN_DISABLE;
		m_ShowMemoryLeakMessage = FN_DISABLE;
		m_SendDebugViewString = FN_DISABLE;
		m_SaveSliceFillThreadLog = FN_DISABLE;//是否儲存影像填滿執行緒訊息
		m_SaveFrameMergeThreadLog = FN_DISABLE;//是否儲存影像合併執行緒訊息
		m_SaveFieldMergeThreadLog = FN_DISABLE;//是否儲存區域合併執行緒訊息	
		m_SaveFrameLoadThreadLog = FN_DISABLE;//是否儲存影像載入執行緒訊息
		m_SaveRegionCalcThreadLog = FN_DISABLE;//是否儲存區域計算執行緒訊息
		m_SaveSequenceThreadLog = FN_DISABLE;//是否儲存系列執行緒訊息
		m_SaveFieldFrameReleaseThreadLog = FN_DISABLE;//是否儲存視野影像釋放執行緒訊息
		m_SaveOnlineInspectionThreadLog = FN_DISABLE;//是否儲存線上檢測執行緒訊息
		m_SaveRemoveFolderThreadLog = FN_DISABLE;//是否儲存刪除資料夾執行緒訊息
		m_SaveConveyerPreRunThreadLog = FN_DISABLE;//是否儲存軌道自動運轉執行緒訊息
		m_SaveITSProcThreadLog = FN_DISABLE;//是否儲存ITS運作執行緒訊息
		m_SaveLoadRepairFileThreadLog = FN_DISABLE;//是否儲存載入維修站檔案執行緒訊息
		m_SaveRepairResultSignalThreadLog = FN_DISABLE;//是否儲存維修站訊息執行緒訊息
		m_SaveSimpleJobThreadLog = FN_DISABLE;//是否儲存簡易作業執行緒訊息
		m_SaveHASIMonitorThreadLog = FN_DISABLE;//是否儲存HASI監視執行緒訊息
		m_SaveTestObjectFinishLog = FN_DISABLE;//是否儲存檢測物件結束訊息		
		m_SaveUIDrawFuncLog = FN_DISABLE;//是否儲存介面重繪函式訊息
		m_SaveUserOperationLog = FN_DISABLE;//是否儲存使用者操作訊息
		m_SaveTestTrackFile = FN_DISABLE;
		m_SaveProjectReportText = FN_DISABLE;
		m_SaveProjectReportTextFilenameMode = SAVE_TEXT_FILENAME_DATETIME;
		m_SaveCustomerReportFile = FN_DISABLE;		
		m_SaveProjectReportWndReading = FN_DISABLE;		

		m_PreLoadProjectOfflineImage = FN_ENABLE;
		m_AutoReleaseFieldFrameBuffer = FN_DISABLE;//自動釋放區域影像		
		m_AutoReleaseOfflineFieldFrameBuffer = FN_DISABLE;//自動釋放離線編程區域影像		

		m_AppVersion = _T("0.0.0.0");
		m_IPHostComputer = _T("192.168.1.100");

		m_MachineLocation = _T("Location");//本機廠區
		m_MachineBuilding = _T("A");//機台棟別
		m_MachineFloor    = _T("1");//機台樓層
		m_MachineRoom     = _T("1");//機台車間
		m_MachineLine     = _T("Line 1");//機台線名
		m_MachineLine_LB  = _T("Line 1");//機台線名-B軌
		m_MachineStation  = _T("Post AOI");//機台站名	
		m_MachineStation_LB= _T("Post AOI");//機台站名-B軌
		m_MachineSN       = _T("SN123456789");//本機序號
		m_MachineName     = AOI3D_APP_NAME;//_T("JET8000");//機台型號
		m_MachineVendor   = AOI3D_VENDOR;//_T("JET");//機台廠商
		m_MachineAlias    = AOI3D_APP_NAME;//_T("JET8000");//機台別名

		m_MachineMES_Name = _T("MES_Name");//MES登入名稱
		m_MachineMES_Password = _T("MES_PWD");//MES登入密碼
		m_MachineMES_Device = _T("Device");//MES登入裝置
		m_MachineMES_Device2= _T("Device2");//MES登入裝置-2
		m_MachineMES_CodeName= _T("TSP_JET8000");//MES-裝置代號
		
		m_AOICustomerID   = AOI_CUSTOMER_ID_JET_TWN;//客戶編號

		m_MachineModelType = MACHINE_MODEL_8000;//設備機種樣式
		m_MachineCameraSide = MACHINE_CAMERA_TOP;//設備相機方向
		m_MultiLanguageMode = MULTI_LANGUAGE_ENGLISH;
		m_StretchBltMode = HALFTONE;
		m_DebayerMode    = IMAGE_DEBAYER_CAMERA_API;				
		
		m_UnwrappingMode = PHASE_UNWRAP_PHASE;
		m_ImageDisplayMode = IMAGE_DISPLAY_COLOR;
		m_ImageDisplayEnhanceMode = IMAGE_DISPLAY_ENHANCE_GAIN;
		m_ImageDisplayGain = 4.0;
		m_ImageDisplayGamma = 1.0;
		m_ImageDisplaySharpnessRadius = 3;//影像顯示的銳利化的半徑
		m_ImageDisplaySharpnessAmount = 200;//影像顯示的銳利化的總量-%
		m_ImageDisplaySharpnessThreshold = 8;//影像顯示的銳利化的閥值
		m_ImageDisplayLocalGammaCalcSize = 31;//影像顯示的局部Gamma的計算範圍
		m_ImageDisplayLocalGammaScaleVal = 1.0;//影像顯示的局部Gamma的縮放比例
		m_ImageDisplayMaxZoomScale = 0.0;
		m_OnlineFormViewMode = ONLINE_FROMVIEW_ONE;//線上檢測介面模式

		m_AutoRetryMaxCount = 0;//自動重測次數上限
		m_AutoSetupDlpPattern = FN_ENABLE;
		m_ResolutionModeTmp = 50;		
		m_ResolutionShowScale = 100.0;

		m_CpuMaxCountUsed = 0;
		m_MTCount_SliceFill = 1;//多執行緒數量-畫面填圖	    
		m_MTCount_FrameMerge = 1;//多執行緒數量-影像合併
		m_MTCount_FieldMerge = 1;
		m_MTCount_FrameLoad = 1;//多執行緒數量-影像載入
		m_MTCount_RegionCalc = 1;//多執行緒數量-區域計算
		m_MTCount_ProcIdle = 1;//多執行緒數量-閒置核心
		m_MTCount_GrabIdle = 4;

		m_OpenMPCount_General = 8;//OpenMP核心數量
		m_OpenMPCount_Inspection = 2;//OpenMP核心數量-檢測中
		m_OpenMPCheckSize_Inspection = 1000*1000;//OpenMP確認尺寸-檢測中		
		
		m_PhasePeriod1 = 16;//第1個相位的週期
		m_PhasePeriod2 = 128.0;//第2個相位的週期
		m_PhasePeriod3 = 160.0;//第3個相位的週期
		m_SeparateDLP2ExpTable = FN_DISABLE;
		m_PhaseConvertHeightMode = PHASE_CONVERT_HEIGHT_SCALE;//相位轉換高度模式

		m_PhaseNoiseLowContrastA = 0;//相位遮罩-低對比
		m_PhaseNoiseLowPotentialA = 0;//相位遮罩-潛力-目前先用LowContrast來取代		
		m_PhaseNoiseOverSaturatedA = 255;//相位遮罩-過亮度
		m_PhaseNoiseLowPotentialB = 0;//相位遮罩-潛力-目前先用LowContrast來取代
		m_PhaseNoiseLowContrastB = 0;//相位遮罩-低對比
		m_PhaseNoiseOverSaturatedB = 255;//相位遮罩-過亮度
		m_PhaseNoiseLowPotentialC = 0;//相位遮罩-潛力-目前先用LowContrast來取代
		m_PhaseNoiseLowContrastC = 0;//相位遮罩-低對比
		m_PhaseNoiseOverSaturatedC = 255;//相位遮罩-過亮度

		m_PhaseNoiseExtendVoid = 0;//相位遮罩-無效點外擴
		m_PhaseNoiseSmoothFilter = 0;//相位遮罩-平滑處理
		m_PhaseNoiseLowContrastColor = 0xFFFF00;//相位遮罩-低對比-顏色
		m_PhaseNoiseLowPotentialColor = 0x00FFFF;//相位遮罩-潛力-顏色
		m_PhaseNoiseOverSaturatedColor = 0x0000FF;//相位遮罩-過亮度-顏色	
		m_PhaseNoiseExtendVoidColor = 0xFF00FF;//相位遮罩-無效點外擴-顏色
		m_SpaceNoiseHeightUnexpectedColor = 0x00FF00;//相位遮罩-高度異常-顏色		
		m_SpaceNoiseHeightOverLowColor = 0x008080;//相位遮罩-過低異常-顏色	
		m_SpaceBestValidColor = 0xFFFFFF;//相位遮罩-可靠高度-顏色

		m_3DObjectDrawScaleX = 1.0;//3D物件縮放比例-X
		m_3DObjectDrawScaleY = 1.0;//3D物件縮放比例-Y
		m_3DObjectDrawScaleZ = 1.0;//3D物件縮放比例-Z

		m_PanelColor1 = 0x40B0B0;//整板的顏色-1
		m_PanelColor2 = 0xB0B040;//整板的顏色-2
		m_PanelTextColor = 0x2200A0;//整板文字顏色
		m_PanelSelectedColor = 0xFF00FF;//整板選取到的顏色
		m_BoardColor1 = 0x40B0B0;//單板的顏色-1
		m_BoardColor2 = 0xB0B040;//單板的顏色-2
		m_BoardTextColor = 0xFFFFFF;//單板文字顏色
		m_BoardSelectedColor = 0xFF00FF;//單板選取到的顏色
		m_FdColor1 = 0xFF00FF;//定位點的顏色-1
		m_FdColor2 = 0xFFFF00;//定位點的顏色-2
		m_FdTextColor = 0x2200A0;//定位點文字顏色
		m_FdSelectedColor = 0xFFFFFF;//定位點選取到的顏色
		m_BarcodeColor1 = 0x00FFFF;//條碼的顏色-1
		m_BarcodeColor2 = 0xFF00FF;//條碼的顏色-2
		m_BarcodeTextColor = 0x2200A0;//條碼文字顏色
		m_BarcodeSelectedColor = 0xFFFFFF;//條碼選取到的顏色
		m_ComponentColor1 = 0x40B0B0;//零件的顏色-1
		m_ComponentColor2 = 0xB0B040;//零件的顏色-2
		m_ComponentTextColor = 0x2200A0;//零件文字顏色
		m_ComponentSelectedColor = 0xFFFFFF;//零件選取到的顏色
		m_DistrictColor1 = 0x9D8420;//分段的顏色-1
		m_DistrictColor2 = 0xB0E4EF;//分段的顏色-2
		m_InspectedResultOKColor = 0x00FF00;//檢測OK顏色
		m_InspectedResultNGColor = 0x0000FF;//檢測NG顏色
		m_InspectedResultSkipColor = 0xFF0000;//檢測Skip顏色
		m_InspectedResultBypassColor = 0xFF0000;//檢測Bypass顏色
		m_InspectedResultUnTestColor = 0x808080;//檢測UnTest顏色
		m_InspectedResultWarningColor = 0x207FFF;//檢測Warning顏色
		m_InspectedResultExceptionColor = 0x0000FF;//檢測Exception顏色

		m_PhaseSmoothCudaMode = FN_DISABLE;
		m_PhaseSmoothCudaMaskSize = 256;
		m_PhaseNoiseDefine = 0x0FFFFFFF;
		m_PhaseNoiseDefineMode = PHASE_NOISE_DEF_MODE_1;
		m_SpaceNoiseSingleCastLowLimit = 2000;//um
		m_SpaceNoiseMultiCastPatchSize = 0;//高度雜訊多投光合併Patch尺寸
		m_SpaceNoiseMultiCastMergeMode = MULTI_CAST_MERGE_MODE_MASS;//高度雜訊多投光合併模式-MASS, Lower
		m_SpaceNoiseMultiCastMergeBestMode = MULTI_CAST_MERGE_BEST_MODE_02;
		m_SpaceNoiseMultiIntensityMergeMode = MULTI_INTENSITY_MERGE_MODE_MEAN;
		m_SpaceNoiseMultiCastMinValidCount = 1;//高度雜訊多投光最少有效值數
		m_SpaceNoiseMultiCastMaxDifference = 500;//um
		m_SpaceNoiseMultiCastLimitDifference = 2000;//um
		m_SpaceNoiseMultiCastValidBestRatio = 100;//高度雜訊多投光高度最好比例-um	
		m_SpaceNoiseMultiCastValidDifference = 200;//um
		m_SpaceNoiseMultiCastOppositeMaxGray = 0;//高度雜訊多投光合併對邊灰階上限-gray

		m_SpaceNoiseCastFilterMode = CAST_SPACE_FILTER_MODE_DEFAULT;//投光後濾波模式
		m_SpaceNoiseCastMedianFilterSize  = 0 ;//高度雜訊的中值濾波尺寸
		m_SpaceNoiseCastMedianFilterUseSize = 1;//高度雜訊的中值濾波使用尺寸

		m_SpaceMergeRecursionMode = FN_DISABLE;//空間合併遞迴模式
		m_SpaceMergeRecursionMaxCount = 3;//空間合併遞迴最多次數
		m_SpaceMergeRecursionKernelSize = 5;//空間合併遞迴鄰居尺寸
		m_SpaceMergeRecursionMaskSize = 0;//空間合併遞迴鄰居忽略尺寸	

		m_SpaceNoiseDataVoidExpandSize = 5;//空間雜訊無效點外擴尺寸-piexel
		m_SpaceNoiseDataVoidExpandEnabed = FN_ENABLE;//空間雜訊無效點外擴啟用
		m_SpaceNoiseFirstFilterMode=DATA_NF_MEDIAN;//空間雜訊首次濾波模式
		m_SpaceNoiseFirstFilterPitch = 4;//空間雜訊首次濾波步長
		m_SpaceNoiseFirstKerSize=9;//空間雜訊首次平滑尺寸
		m_SpaceNoiseFirstUseSize=1;//空間雜訊首次使用尺寸
		m_SpaceNoiseFirstFilterSearchOn = true;//搜尋相似高度啟用
		m_SpaceNoiseFirstFilterAlphaF = 450;//搜尋相似高度範圍
		m_SpaceNoiseFirstFilterAlphaS = 200;//使用相似高度範圍
		m_SpaceNoiseFirstFilterAlphaM = 200;//使用自身高度範圍
		m_SpaceNoiseFirstFilterAlphaI = 18;//使用自身灰階範圍
		m_SpaceNoiseFirstFilterThresdhold_Outlier = 5;//雜訊下限值
		m_SpaceNoiseOverLowerMode = DATA_NF_MEDIAN;//高度雜訊過低啟用
		m_SpaceNoiseOverLowerRange = -200;//um
		m_SpaceNoiseOverLowerLimit = -500;//um	
		m_SpaceNoiseOverLowerKerSize = 31;//高度雜訊過低濾波尺寸
		m_SpaceNoiseOverLowerUseSize = 1;//高度雜訊過低使用尺寸
		m_SpaceNoiseHeightAbnormalMode = DATA_NF_MEDIAN;//高度雜訊高度異常啟用
		m_SpaceNoiseHeightAbnormalPitch = 4;//高度雜訊高度異常步長
		m_SpaceNoiseHeightAbnormalRange = 500;//高度雜訊高度異常範圍-um	
		m_SpaceNoiseHeightAbnormalChkSize = 21;//高度雜訊高度異常確認尺寸
		m_SpaceNoiseHeightAbnormalKerSize = 13;//高度雜訊高度異常濾波尺寸
		m_SpaceNoiseHeightAbnormalUseSize = 1;//高度雜訊高度異常使用尺寸		
		m_SpaceNoiseHeightAbnormalRepeatCnt = 1;//高度雜訊高度異常重複次數
		m_SpaceNoiseDataVoidReContructedExtSize = 5;//空間雜訊重建外擴尺寸
		m_SpaceNoiseDataVoidReContructedEnabled = FN_ENABLE;//空間雜訊重建外擴啟用			
		m_SpaceNoiseFinalFilterMode = DATA_NF_AVERAGE;//空間雜訊最後濾波模式
		m_SpaceNoiseFinalPitch = 4;//空間雜訊最後濾波步長
		m_SpaceNoiseFinalKerSize = 5;//空間雜訊最後平滑尺寸
		m_SpaceNoiseFinalUseSize = 0;//空間雜訊最後使用尺寸
		m_SpaceNoiseFinalFilterSearchOn = true;//搜尋相似高度啟用
		m_SpaceNoiseFinalFilterAlphaF = 450;//搜尋相似高度範圍
		m_SpaceNoiseFinalFilterAlphaS = 200;//使用相似高度範圍
		m_SpaceNoiseFinalFilterAlphaM = 200;//使用自身高度範圍
		m_SpaceNoiseFinalFilterAlphaI = 18;//使用自身灰階範圍
		m_SpaceNoiseFinalFilterThresdhold_Outlier = 1;//雜訊下限值
		m_SpaceNoiseFinalFilterMode2 = DATA_NF_DISABLE;//空間雜訊最後濾波模式-2
		m_SpaceNoiseFinalPitch2 = 1;//空間雜訊最後濾波步長-2
		m_SpaceNoiseFinalKerSize2 = 0;//空間雜訊最後平滑尺寸-2
		m_SpaceNoiseFinalUseSize2 = 0;//空間雜訊最後使用尺寸-2
		m_SpaceNoiseFinalFilterSearchOn2 = true;//搜尋相似高度啟用2
		m_SpaceNoiseFinalFilterAlphaF2 = 450;//搜尋相似高度範圍2
		m_SpaceNoiseFinalFilterAlphaS2 = 200;//使用相似高度範圍2
		m_SpaceNoiseFinalFilterAlphaM2 = 200;//使用自身高度範圍2
		m_SpaceNoiseFinalFilterAlphaI2 = 18;//使用自身灰階範圍2
		m_SpaceNoiseFinalFilterThresdhold_Outlier2 = 1;//雜訊下限值2
		
		m_SpaceVerifyMultiCastGapRatio = 10;//高度驗證-多投光差距比例上限-%
		m_SpaceVerifyMultiCastGapThreshold = 50;//高度驗證-多投光差距閥值-um

		m_CudaFnEnabled = FN_ENABLE;
		m_CudaBlockNumber = 986;
		m_CudaThreadNumber = 512;

		m_CalcFocusSmoothSize = 5;//計算焦距平滑處理
		m_CalcFocusMode = CALC_FOCUS_VARIANCE;//計算焦距模式

		m_MemoryKeepMode = FN_ENABLE;
		m_MatchLibType = JET_MATCH_LIB_MIM;
		m_HoneywellSwiftDecoderEnabled = FN_DISABLE;

		m_DongleWarningRemainingDays = 30;//硬體鎖警告剩餘天數
		m_DongleWarningRemainingCount = 1000;//硬體鎖警告剩餘次數

		m_LastStationLineMode = LAST_STATION_LINE_MODE_2;//與上一站連線方式		
		m_LaneWorkMode_LA = LANE_WORK_RUN;//A軌道運轉模式
		m_LaneWorkMode_LB = LANE_WORK_DISABLE;//B軌道運轉模式
		m_MultiTowerLight = MULTI_TOWER_LIGHT_SINGLE;//雙塔燈模式
		m_CheckPCBRemovedCount = 5;//確認PCB板移走次數
		m_NextConnectedBufferType = CONNECTED_BUFFER_FIXED;//下一站連接輸送帶樣式
		m_MultiProjectTestOrderMode = MULTI_PROJECT_TEST_ORDER_BY_MARK;//多專案檢測次序模式

		m_OnlineInputProjectWorkNumber = ONLINE_INPUT_DISABLE;//在線輸入專案工單號碼

		m_OnlineAutoCalibrationDlpLedColor = 4;//在線自動校正DLP-LED顏色(4-Blue)
		m_OnlineAutoCalibrationCapDelayTime = 2000;//在線自動校正上蓋延遲時間-ms
		m_OnlineAutoCalibrationMode_XYZHome = FUNC_EXEC_OFF;//在線自動校正模式-XYZ歸零
		m_OnlineAutoCalibrationPeriod_XYZHome = 0;//在線自動校正週期-XYZ歸零-小時	
		m_OnlineAutoCalibrationMode_2DCurrent=FUNC_EXEC_OFF;//在線自動校正模式-2D電流
		m_OnlineAutoCalibrationPeriod_2DCurrent = 0;//在線自動校正週期-2D電流-小時
		m_OnlineAutoCalibrationPeriod_3DCurrent = 0;//在線自動校正週期-3D電流-小時
		m_OnlineAutoCalibrationMode_3DZeroPlane=FUNC_EXEC_OFF;//在線自動校正模式-3D相平面
		m_OnlineAutoCalibrationPeriod_3DZeroPlane = 0;//在線自動校正週期-3D相平面-小時
		m_OnlineAutoCalibrationPeriod_3DHeightFactor = 0;//在線自動校正週期-3D高度比例-小時

		m_OnlineAutoStopByIdleTime = 0;//在線自動停機-閒置時間-分鐘
		m_OnlineAutoStopBySpecTime1= 0;//在線自動停機-特定時間-時時分分秒秒
		m_OnlineAutoStopBySpecTime2= 0;//在線自動停機-特定時間-時時分分秒秒
		m_OnlineAutoStopBySpecTime3= 0;//在線自動停機-特定時間-時時分分秒秒
		m_AutoSwitchToOnlineViewTime = 0;//自動切換線上畫面-秒	
		m_OnlineShowProjectTestMap = FN_DISABLE;//在線顯示專案檢測底圖
		m_AutoSwitchToOnlineRemoteCtrlTime = 0;
		m_AutoSwitchToOnlineRemoteCtrlMode = MES_EQP_CTRL_STATE_NONE;

		m_MoveCameraBeforePCBIn = FN_DISABLE;
		m_PCBInUseCameraImageMode = FN_DISABLE;
		m_ClampPcbBeforeTestMode = FUNC_EXEC_OFF;
		m_GrabFiducialDelayTime_ms = 200;
		m_PCBOutDirection = PCB_OUT_DIR_FORWARD;//PCB出板方向
		m_BypassLastSignal = FN_DISABLE;//忽略上一站訊號-回板使用
		m_BypassNextSignal = FN_DISABLE;//忽略下一站訊號-回板使用
		m_PCBOKNGSignalDelayTime = 0;//PCB OK/NG訊號延遲時間-ms
		m_EditLineSizeLevel = 2;//編輯線尺寸的層級		
		m_OnlineTuningKeepMaxTime = 10;//線上調機保留最久時間-分鐘
		m_OnlineTuningSavedMaxCount = 0;
		m_UserLoginMode = USER_LOGIN_DISABLE;		
		m_UserLoginOptions = USER_LOGIN_OPTIONS_PASSWORD;
		m_OpenProjectMode = OPEN_PROJECT_FILE;//開專案模式
		m_OpenProjectMapIndex = 1;//開專案底圖編號		

		m_VerifyProjectMode = VERIFY_PROJECT_DISABLE;//驗證專案模式
		m_VerifyProjectFilename = _T("");//驗證專案的檔名

		m_ModelNameUsePartNumber = FN_ENABLE;//模組名稱使用料號

		m_OnlineOpenProjectMode = ONLINE_OPEN_PROJECT_DISABLE;//線上開專案模式
		m_OnlineOpenProjectCameraBarcode = FN_DISABLE;//線上開專案-相機條碼
		m_OnlineOpenProjectBarcodeDeviceGrabMode = BARCODE_DEVICE_GRAB_BEFORE_PCB_IN;//線上開專案外接條碼取像模式

		m_SaveProjectSpcFileMode = SAVE_SPC_FILE_JSON_VRS;//專案儲存SPC檔案模式
		m_SaveProjectSpcLibraryMode = SAVE_SPC_OTHER_FILE_OFF;//專案儲存SPC資料庫模式

		m_ModelUnsetColor = 0xC08000;//模組未設定顏色
		m_BinaryMaskColorRGB = 0xFFFFFF;//2值化遮罩顏色-3色燈
		m_BinaryMaskColorDefault = 0x00FFFF;//2值化遮罩顏色-預設
		m_BinaryMaskColorAlpha = 0x00;//2值化遮罩顏色-透明度

		m_InspectionFinishShowResultList = FN_ENABLE;//檢測結束顯示結果列表
		m_ModelDefaultWndLevel = 1;//模組預設檢測框等級;//
		m_SwitchProject3DFrame = FN_ENABLE;//可切換至專案3D畫面		
		m_AutoSwitchWnd3DFrameMode = AUTO_SWITCH_WND_3D_FRAME_BY_GROUP_CHANGE;//自動切換至檢測框3D畫面模式
		m_AutoSwitchWnd3DFrameSizeLimit = 1000;//自動切換至檢測框3D畫面尺寸上限-um
		m_ShowComponentFullMap = FN_DISABLE;//顯示零件-整板零件
		m_ShowComponentDefectOnly = FN_DISABLE;
		m_ShowDebugFormView = FN_DISABLE;//顯示除錯頁面
		m_LevelFilterShiftEnabled = FN_ENABLE;//分等濾波起點偏移啟用
		m_MedianFilterShiftEnabled = FN_ENABLE;//中值濾波起點偏移啟用
		m_SmoothFilterShiftEnabled = FN_ENABLE;//平滑濾波起點偏移啟用
		m_PyramidMedianFilterShiftEnabled = FN_ENABLE;//金字塔中值濾波起點偏移啟用
		m_LevelFilterPitchTolerance = 30;//分等濾波步長可接受誤差
		m_MedianFilterPitchTolerance = 30;//中值濾波可接受誤差		
		m_SmoothFilterPitchTolerance = 30;//平滑濾波步長可接受誤差
		m_PyramidMedianFilterPitchTolerance = 30;//金字塔中值濾波可接受誤差		

		m_GrrOffsetRandomValue = 0;//Grr偏移隨機補償值-nm
		m_GrrAverageResetEnabled = FN_DISABLE;
		m_GrrAverageResetMatchRatio = 50.0f;
		m_GrrSkewAverageEnbVal = 0.0f;
		m_GrrSkewAverageStartGap = 0.0f;
		m_GrrSkewAverageWeighting = 50;
		m_GrrOffsetAverageEnbPxl = 0;
		m_GrrOffsetAverageStartGap = 0.0f; 
		m_GrrOffsetAverageWeighting = 50;
		m_GrrHeightAverageEnbVal = 0.0f;
		m_GrrHeightAverageStartGap = 0.0f;
		m_GrrHeightAverageWeighting = 50;
		m_LockScreenEnabled = FN_DISABLE;//啟用鎖住螢幕
		m_LockScreenKeyID = 'Z';//鎖住螢幕鍵號
		m_MultiDistrictModeEnabled = FN_DISABLE;//多段檢測啟用
		m_OnlineTuningEnableBarcode = FN_DISABLE;//在線調機-軟體條碼
		m_ShowPlcSaftySettingUI = FN_ENABLE;//是否顯示PLC安全檢知設定介面
		m_ShowAlgOffsetLParam = FN_DISABLE;
		m_ShowAlgOffsetAParam = FN_DISABLE;//是否顯示演算法OffsetA的參數
		m_ShowAlgBrightRatioScaleParam = FN_DISABLE;//是否顯示演算法亮度比例的比例參數
		m_ShowModelPropertyParam = FN_DISABLE;//是否顯示模組屬性參數
		m_ContinuePasteMode = FN_DISABLE;//連續貼上模式
		m_StitchImagePaddingSize = 2;//拼圖參數-填補尺寸
		m_ResinHeightAlignEnabled = FN_DISABLE;//啟用Resin高度對齊-軍達3D對位
		m_ModelImageCadOffsetEnabled = FN_ENABLE;//啟用模組影像Cad偏移補償um
		m_ModelDefaultTransistorType = MODEL_TYPE_TRANSISTOR;//模組預設SOT樣式
		m_ProjectLocalFolderEnabled = FN_ENABLE;//專案使用本機資料夾
		m_PartialCopyProjectLibrary = FN_DISABLE;//部分複製專案資料庫
		m_AutoArrangeModelBkImageFiles = FN_DISABLE;//自動重整模組底圖檔案
		m_AutoCopySpcComponentImageFiles = FN_DISABLE;
		m_AutoBypassGrab3DFrame = FN_DISABLE;//自動跳過取3D影像
		m_CheckProjectFdReady = FN_ENABLE;
		m_LockModelBodyPosition = FN_DISABLE;		
		m_MaxUncheckTestFileCount = 0;
		m_ConfirmComponentBarcodeEnabled = FN_DISABLE;
		m_SaveJpegQuality = 90;
	#ifndef OFFLINE_VERSION
		m_CopyHugeFilesMode = COPY_HUGE_FILES_ROBOCOPY;//複製大量檔案模式
	#else
		m_CopyHugeFilesMode = COPY_HUGE_FILES_COPY;//複製大量檔案模式
	#endif//OFFLINE_VERSION
		m_OfflineVersionMode = OFFLINE_VERSION_NORMAL;//離線版本模式		
		m_UseProjectSystemParamMode = FN_DISABLE;//使用專案系統參數模式		

		m_UIWndFontAddSize = 0;//UI視窗字型增加大小
		m_UIDockWndSlideSteps = 2;//UI駐停視窗滑動步長 
		m_UIEnablePCBOutButton = FN_ENABLE;

		m_RabbitMQServerPort = 5672;//RabbitMQ伺服器Port		
		m_RabbitMQServerAddress = _T("localhost");//RabbitMQ伺服器位址		
		m_RabbitMQUserName = "aoiuser";//RabbitMQ登錄名稱
		m_RabbitMQPassword = "123456";//ITS訊息佇列密碼

		m_ITSFilename = AOI3D_FOLDER("\\JetITS.exe");//_T("C:\\JETAOI3D\\JetITS.exe");//ITS軟體名稱
		m_ITSCommunicationMode=ITS_COMMUNICATION_SOCKET;//ITS通訊模式
		m_ITSSocketIPPort = 8192;//ITS網路Port
		m_ITSSocketIPAddress = "127.0.0.1";//ITS網路位址
		m_ITSRabbitMQRecvQueueName = _T("ITS2AOI");//ITS訊息佇列接收名
		m_ITSRabbitMQSendQueueName = _T("AOI2ITS");//ITS訊息佇列傳送名
		m_ITSContactSoftware = MES_CONTACT_TYPE;//ITS-對接軟體		
		m_ITSCommunicationEnabled = FN_DISABLE;//連線至ITS啟用	
		m_ITSCommunicationTimeoutMS = 10000;//與ITS溝通逾時(ms)		
		m_ITSFileFolderSend = _T("D:\\AOI3DITS\\AOI2ITS");//ITS檔案資料夾-傳送
		m_ITSFileFolderRecv = _T("D:\\AOI3DITS\\ITS2AOI");//ITS檔案資料夾-接收
		m_ITSFileBackupEnabled = false;//ITS檔案資料夾-備份	
		m_ITSFileUseSyncFileEnabled = false;//ITS檔案資料夾-同步檔案		
		m_ITSSetSecsGemEnabled = FN_DISABLE;//ITS使用設定SECS/GEM
		m_ITSSetSecsGemRemoteLocal = FN_DISABLE;
		m_ITSSetSystemParamEnabled = FN_ENABLE;//ITS使用設定系統參數
		m_ITSSetProjectParamEnabled = FN_ENABLE;//ITS使用設定專案參數
		m_ITSSetMacineStatusEnabled = FN_ENABLE;//ITS使用設定機台狀態
		m_ITSSetAppOpnCloseEnabled = FN_ENABLE;//ITS使用設定軟體開關
		m_ITSSetProcessIDEnabled = FN_ENABLE;//ITS使用執行程序編號
		m_ITSSetUserLogin_outEnabled = FN_ENABLE;//ITS使用設定使用者登入登出		

		m_DefaultSpaceToGrayRatioMode = 50;//專案預設高度轉灰階比例
		m_DefaultSpaceBasePlaneIndex = -1;//專案預設空間基準面編號
		m_DefaultSpaceNoiseFilterIndex = -1;//專案預設空間雜訊過濾編號
		m_DefaultEnableConveyerPreRun = FN_DISABLE;//專案預設軌道提前運轉功能
		m_DefaultFdNGHandleMode = FD_NG_HANDLE_STOP;//專案預設定位點異常處理模式
		m_DefaultBoardFdGrabMode = BOARD_FD_GRAB_AFTER_PANEL;//專案預設單板定位點取像模式
		m_DefaultDefectHandleMode = DEFECT_HANDLE_NEXT_STOP;//專案預設檢出異常處理模式
		m_DefaultPCBOutMode = PCB_OUT_NORMAL;//專案預設PCB出板模式
		m_DefaultProjectSaveTestMap = SAVE_TEST_MAP_DISABLE;
		m_DefaultProjectLinkServerMode = PROJECT_LINK_SERVER_DISABLE;//專案預設連線伺服器模式		
		m_DefaultProjectSaveOfflineImageFiles = true;//專案預設儲存離線圖檔
		m_DefaultBarcodeVerifyMode = FN_DISABLE;//專案預設條碼驗證模式
		m_DefaultBarcodeRetrieveMode = FN_DISABLE;//專案預設條碼查詢模式

		m_OperateLevelProjectOpen = USER_LEVEL_SIGN_OUT;//操作等級-專案開啟
		m_OperateLevelProjectSave = USER_LEVEL_SIGN_OUT;//操作等級-專案儲存
		m_OperateLevelProjectParam = USER_LEVEL_SIGN_OUT;//操作等級-專案參數
		m_OperateLevelOnlineRun = USER_LEVEL_SIGN_OUT;//操作等級-線上運行
		m_OperateLevelOnlineBypass = USER_LEVEL_SIGN_OUT;//操作等級-線上直通
		m_OperateLevelOnlineStop = USER_LEVEL_SIGN_OUT;//操作等級-線上停止
		m_OperateLevelOnlineSaveImage = USER_LEVEL_ENGINEER;//操作等級-線上存圖
		m_OperateLevelOnlineUnlock = USER_LEVEL_ENGINEER;//操作等級-線上解鎖		
		m_OperateLevelEditFuncAdd = USER_LEVEL_ENGINEER;//操作等級-編輯功能-新增
		m_OperateLevelEditFuncDel = USER_LEVEL_ENGINEER;//操作等級-編輯功能-刪除
		m_OperateLevelEditFuncBypass = USER_LEVEL_ENGINEER;//操作等級-編輯功能-不檢測		
		m_OperateLevelMesCtrlState_Offline = USER_LEVEL_ENGINEER;//操作等級-MES相關-離線
		m_OperateLevelMesCtrlState_Local = USER_LEVEL_ENGINEER;//操作等級-MES相關-本地上線
		m_OperateLevelMesCtrlState_Remote = USER_LEVEL_ENGINEER;//操作等級-MES相關-遠端上線		
		m_OperateLevelMesShowContext = USER_LEVEL_JET_FAE;//操作等級-MES相關-顯示內容

		m_M2M_NPM_Barcode_Enable = FN_DISABLE;//啟用NPM-條碼
		m_M2M_NPM_APC_FF1_Enable = FN_DISABLE;//啟用NPM APC-FF1
		m_M2M_NPM_APC_FF2_Enable = FN_DISABLE;//啟用NPM APC-FF2
		m_M2M_NPM_APC_MFB_Enable = FN_DISABLE;//啟用NPM APC-MFB
		m_M2M_NPM_LaneName_LA = _T("L1");//NPM軌道名稱-A軌
		m_M2M_NPM_LaneName_LB = _T("L2");//NPM軌道名稱-B軌
		m_M2M_NPM_InputShareFolder_LA = AOI3D_FOLDER("\\M2M");//_T("C:\\JETAOI3D\\M2M");//NPM-輸入共享資料夾-A軌
		m_M2M_NPM_InputShareFolder_LB = AOI3D_FOLDER("\\M2M");//_T("C:\\JETAOI3D\\M2M");//NPM-輸入共享資料夾-B軌
		m_M2M_NPM_OutputShareFolder_LA = AOI3D_FOLDER("\\M2M");//_T("C:\\JETAOI3D\\M2M");//NPM-輸出共享資料夾-A軌
		m_M2M_NPM_OutputShareFolder_LB = AOI3D_FOLDER("\\M2M");//_T("C:\\JETAOI3D\\M2M");//NPM-輸出共享資料夾-B軌

		m_M2M_HASI_Enable = false;
		m_M2M_HASI_ShareFolder = "D:\\HAS";
		m_M2M_HASI_SerialFile_Enable = true;//啟用SerialFile-Hanwha
		m_M2M_HASI_SerialFile_QueueSize = 1;//SerialFile-Size of Serial Queue-Hanwha
		m_M2M_HASI_SerialFile_DwellTime = 3000;//等待資料的延遲時間-ms
		m_M2M_HASI_SPIOffsetFile_Enable = false;//啟用SPIOffsetFile-Hanwha
		m_M2M_HASI_AOI_Stage = HASI_AOI_STAGE_POST;//產線上 AOI 的檢查階段
		m_M2M_HASI_PNP_Enable = false;//啟用PNP-Hanwha
		m_M2M_HASI_StateMode = HASI_STATE_MODE_STOP;

		m_AiModelServerEnabled = FN_DISABLE;//AI模型伺服器啟用
		m_AiModelServerTimeout = 10000;//AI模型伺服器逾時
		m_AiModelServerFilename = _T("");//AI模型伺服器檔名
		m_AiModelFileFolderSend = AOI3D_FOLDER("\\AiModel\\AOI2AI");//_T("C:\\JETAOI3D\\AiModel\\AOI2AI");//AI模型檔案資料夾-傳送
		m_AiModelFileFolderRecv = AOI3D_FOLDER("\\AiModel\\AI12AOI");//_T("C:\\JETAOI3D\\AiModel\\AI12AOI");//AI模型檔案資料夾-接收
		m_AIModelLabelMinClusterDistance = 80.0;//AI模型分類最小叢集距離-um

		m_ExternalCopyFileEnabled = FN_DISABLE;
		m_ExternalCopyFileAppName=_T("C:\\FileCopy\\FileCopy.exe");//外部複製檔案軟體名稱
		m_ExternalCopyFileSendFolder=_T("C:\\FileCopy\\Files");//外部複製檔案輸出資料夾

		m_CpkChartEnabled = FN_DISABLE;//Cpk圖表啟用
		m_WndRotationFollowed = FN_DISABLE;//檢測框跟隨旋轉
	}
} TSystemParameter, *PSystemParameter;
//-------------------------------------------------------------------------------------//
typedef struct tagCalibrationParameter
{
	//---------------------------------------------------------------------------------//
	double                     m_TargetGridPosX;//格線塊位置-X
	double                     m_TargetGridPosY;//格線塊位置-Y
	double                     m_TargetGridPosZ;//格線塊位置-Z
	int                        m_TargetGridExpTime_us;//格線塊位置-曝光時間us
	double                     m_TargetRectPosX;//方形規位置-X
	double                     m_TargetRectPosY;//方形規位置-Y
	double                     m_TargetRectPosZ;//方形規位置-Z
	int                        m_TargetRectExpTime_us;//格線塊位置-曝光時間us
	double                     m_TargetWhitePosX;//白色規位置-X
	double                     m_TargetWhitePosY;//白色規位置-Y
	double                     m_TargetWhitePosZ;//白色規位置-Z
	int                        m_TargetWhiteExpTime_us;//格線塊位置-曝光時間us
	double                     m_TargetHeightPosX;//高度規位置-X
	double                     m_TargetHeightPosY;//高度規位置-Y
	double                     m_TargetHeightPosZ;//高度規位置-Z
	int                        m_TargetHeightExpTime_us;//格線塊位置-曝光時間us
	double                     m_PhaseZeroPlaneOffsetPosZ;//相平面偏差位置-Z	
	int                        m_3DCastCurrentGray;//3D投光電流校正灰階值
	int                        m_3DCastCurrentPitch;//3D投光電流校正間距
	int                        m_3DCastCurrentExpTime_us;//3D投光電流校正曝光時間us
	double                     m_3DCastMountAngle;//3D投光的安裝角度
	int                        m_3DCastHeightFactorGridCols;//3D投光的高度校正網格數量-X
	int                        m_3DCastHeightFactorGridRows;//3D投光的高度校正網格數量-Y
	double                     m_PhaseFactorFovPitch;//相位參數視野的間距
	double                     m_PhaseFactorFovRange;//相位參數視野的範圍
	double                     m_TargetHeightThickValue;//塊規高度值	
	int                        m_EnableBasePhaseCorrect;//啟用基本相平面修正
	int                        m_BasePhaseCorrectTimes;//基本相平面修正階數//v1.01.03.234
	int                        m_EnableHeightFactorCorrect;//啟用高度係數修正
	int                        m_HeightFactorCorrectTimes;//高度係數修正階數//v1.01.04.001
	int                        m_HeightFactorCalibrateWithFOV;//高度係數校正使用FOV模式
	//---------------------------------------------------------------------------------//
	int                        m_ResolutionMode;//解析度模式
	int                        m_ResolutionMode_Offline;//解析度模式		
	//---------------------------------------------------------------------------------//
	double                     m_FOVWidth_um;//FOV寬度 
	double                     m_FOVHeight_um;//FOV高度 
	int                        m_ExposureTime_us;//曝光時間	
	int                        m_FdExposureTime_us;//定位點曝光時間	
	int                        m_DLPExposureTime_us;//樣板曝光時間	
	int                        m_DLPExposureTime2_us;//樣板曝光時間	
	//---------------------------------------------------------------------------------//
	tagCalibrationParameter()
	{
		m_TargetGridPosX = 0;
		m_TargetGridPosY = 0;
		m_TargetGridPosZ = 0;
		m_TargetGridExpTime_us = 3000;
		m_TargetRectPosX = 0;
		m_TargetRectPosY = 0;
		m_TargetRectPosZ = 0;
		m_TargetRectExpTime_us = 3000;
		m_TargetWhitePosX = 0;
		m_TargetWhitePosY = 0;
		m_TargetWhitePosZ = 0;
		m_TargetWhiteExpTime_us = 3000;
		m_TargetHeightPosX = 0;
		m_TargetHeightPosY = 0;
		m_TargetHeightPosZ = 0;
		m_TargetHeightExpTime_us = 3000;
		m_PhaseZeroPlaneOffsetPosZ = 3000;
		m_3DCastCurrentGray = 180;
		m_3DCastCurrentPitch = 1;
		m_3DCastCurrentExpTime_us = 3000;
		m_3DCastMountAngle = 30.0;
		m_3DCastHeightFactorGridCols = 9;
		m_3DCastHeightFactorGridRows = 9;
		m_TargetHeightThickValue = 3000;
		m_PhaseFactorFovPitch = 2000;
		m_PhaseFactorFovRange = 11000;
		m_EnableBasePhaseCorrect = FN_DISABLE;
		m_BasePhaseCorrectTimes = 3;
		m_EnableHeightFactorCorrect = FN_DISABLE;
		m_HeightFactorCorrectTimes = 3;
		m_HeightFactorCalibrateWithFOV = FN_DISABLE;

		m_ResolutionMode = 50;
		m_ResolutionMode_Offline = m_ResolutionMode;
		
		m_FOVWidth_um = 1000;
		m_FOVHeight_um = 1000;
		m_ExposureTime_us = 3000;
		m_FdExposureTime_us = 6000;
		m_DLPExposureTime_us = 6000;
		m_DLPExposureTime2_us = 6000;
	}
	//---------------------------------------------------------------------------------//
} TCalibrationParameter,*PCalibrationParameter;
//-------------------------------------------------------------------------------------//
typedef struct tagImageParameter
{
	double                        m_ImageZoomMin;//影像縮放最小值
	double                        m_ImageZoomMax;//影像縮放最大值
	int                           m_ProjectImageMaxWidth;//專案影像最大寬度
	int                           m_ProjectImageMaxHeight;//專案影像最大高度
	tagImageParameter()
	{
		m_ImageZoomMin =  0.20;//影像縮放最小值
		m_ImageZoomMax = 10.00;//影像縮放最大值
		m_ProjectImageMaxWidth = 1920;//專案影像最大寬度
		m_ProjectImageMaxHeight = 1920;//專案影像最大高度
	}
} TImageParameter, *PImageParameter;
//-------------------------------------------------------------------------------------//
#endif//_SYSTEM_PARAMETER_DEF_H_
