//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "AOIDataCollect.h"
//-------------------------------------------------------------------------------------//
#include "JetLoadDll.h"
//-------------------------------------------------------------------------------------//
USER_LEVEL_MODE CAOIDataCollect::LoadUserLevel(LPCTSTR sLevel)//載入使用者權限
{
	USER_LEVEL_MODE eLevel=USER_LEVEL_SIGN_OUT;
	if ( NULL == sLevel ) { return eLevel; }
	const int nLevel=::_ttoi(sLevel);	
	switch ( nLevel )
	{
	case USER_LEVEL_SIGN_OUT:
	case USER_LEVEL_OPERATOR:
	case USER_LEVEL_ENGINEER:
	case USER_LEVEL_SUPERVISOR:
	case USER_LEVEL_JET_FAE:
	case USER_LEVEL_JET_SENIOR:
	case USER_LEVEL_JET_RD:
		eLevel = (USER_LEVEL_MODE)(nLevel);
		break;
	default:
		eLevel = USER_LEVEL_ENGINEER;
		break;
	}
	return eLevel;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataCollect::ObtainSystemParameterDescText(SYSTEM_PARAM_ID ParamID)//取得系統參數的說明文字
{	
	CString String;	
	CString KeyName;	
	CString Default;
	switch ( ParamID )
	{
	case SYSTEM_AOI_FOLDER_HOST://系統資料夾
		KeyName = _T("SYSTEM_AOI_FOLDER_HOST");
		break;
	case SYSTEM_AOI_FOLDER_TEMP://暫存的資料夾	
		KeyName = _T("SYSTEM_AOI_FOLDER_TEMP");
		break;
	case SYSTEM_AOI_FOLDER_LOG://系統訊息資料夾		
		KeyName = _T("SYSTEM_AOI_FOLDER_LOG");
		break;
	case SYSTEM_AOI_FOLDER_PROJECT://專案資料夾	
		KeyName = _T("SYSTEM_AOI_FOLDER_PROJECT");	
		break;
	case SYSTEM_AOI_FOLDER_RESULT://結果資料夾
		KeyName = _T("SYSTEM_AOI_FOLDER_RESULT");
		break;	
	case SYSTEM_AOI_FOLDER_SERVER://伺服器資料夾
		KeyName = _T("SYSTEM_AOI_FOLDER_SERVER");
		break;
	case SYSTEM_AOI_FOLDER_STATIC_DATA://統計數據資料夾
		KeyName = _T("SYSTEM_AOI_FOLDER_STATIC_DATA");
		break;
	case SYSTEM_AOI_FOLDER_TEXT_REPORT://文字報告資料夾
		KeyName = _T("SYSTEM_AOI_FOLDER_TEXT_REPORT");
		break;
	case SYSTEM_AOI_FOLDER_ONLINE_TUNING://線上調機資料夾
		KeyName = _T("SYSTEM_AOI_FOLDER_ONLINE_TUNING");
		break;
	case SYSTEM_AOI_FOLDER_ONLINE_BARCODE://線上條碼資料夾
		KeyName = _T("SYSTEM_AOI_FOLDER_ONLINE_BARCODE");
		break;
	case SYSTEM_AOI_FOLDER_ONLINE_OFFLINE://線上離線編程資料夾	
		KeyName = _T("SYSTEM_AOI_FOLDER_ONLINE_OFFLINE");
		break;
	case SYSTEM_AOI_FOLDER_PROJECT_TEST_MAP://專案檢測底圖資料夾
		KeyName = _T("SYSTEM_AOI_FOLDER_PROJECT_TEST_MAP");
		break;
	case SYSTEM_AOI_FOLDER_PROJECT_TEST_MAP_TEMP://專案檢測底圖資料夾-暫存
		KeyName = _T("SYSTEM_AOI_FOLDER_PROJECT_TEST_MAP_TEMP");
		break;
	case SYSTEM_AOI_FOLDER_PROJECT_TEST_MAP_BACKUP://專案檢測底圖資料夾-備份
		KeyName = _T("SYSTEM_AOI_FOLDER_PROJECT_TEST_MAP_BACKUP");
		break;
	case SYSTEM_AOI_FOLDER_PROJECT_RAW://專案原圖資料夾
		KeyName = _T("SYSTEM_AOI_FOLDER_PROJECT_RAW");
		break;
	case SYSTEM_AOI_FOLDER_PROJECT_DEBUG://專案除錯資料夾
		KeyName = _T("SYSTEM_AOI_FOLDER_PROJECT_DEBUG");
		break;
	case SYSTEM_AOI_FOLDER_MONITOR_STATUS://監控狀態資料夾
		KeyName = _T("SYSTEM_AOI_FOLDER_MONITOR_STATUS");
		break;
	case SYSTEM_AOI_FOLDER_CUSTOMER_LOG://客戶訊息資料夾
		KeyName = _T("SYSTEM_AOI_FOLDER_CUSTOMER_LOG");
		break;
	case SYSTEM_AOI_FOLDER_CUSTOMER_REPORT://客戶報告資料夾	
		KeyName = _T("SYSTEM_AOI_FOLDER_CUSTOMER_REPORT");
		break;
	case SYSTEM_AOI_FOLDER_BARCODE_FILE_LA://條碼檔案資料夾-A軌
		KeyName = _T("SYSTEM_AOI_FOLDER_BARCODE_FILE_LA");
		break;
	case SYSTEM_AOI_FOLDER_BARCODE_FILE_LB://條碼檔案資料夾-B軌	
		KeyName = _T("SYSTEM_AOI_FOLDER_BARCODE_FILE_LB");
		break;
	case SYSTEM_AOI_FOLDER_AI_FILE_EXPORT://AI檔案輸出資料夾
		KeyName = _T("SYSTEM_AOI_FOLDER_AI_FILE_EXPORT");
		break;
	case SYSTEM_AOI_FOLDER_AI_IMAGE_EXPORT://AI影像輸出資料夾
		KeyName = _T("SYSTEM_AOI_FOLDER_AI_IMAGE_EXPORT");
		break;	
	case SYSTEM_AOI_FOLDER_TEST_TRACK://檢測追蹤資料夾
		KeyName = _T("SYSTEM_AOI_FOLDER_TEST_TRACK");
		break;
	case SYSTEM_AOI_LIBRARY_HOST://資料庫-本機
		KeyName = _T("SYSTEM_AOI_LIBRARY_HOST");
		break;
	case SYSTEM_AOI_LIBRARY_REMOTE://資料庫-遠端
		KeyName = _T("SYSTEM_AOI_LIBRARY_REMOTE");
		break;	
	case SYSTEM_IP_HOST_COMPUTER://本機網路網址
		KeyName = _T("SYSTEM_IP_HOST_COMPUTER");		
		break;
	case SYSTEM_MACHINE_LOCATION://本機廠區
		KeyName = _T("SYSTEM_MACHINE_LOCATION");
		break;
	case SYSTEM_MACHINE_BUILDING://本機棟別
		KeyName = _T("SYSTEM_MACHINE_BUILDING");
		break;
	case SYSTEM_MACHINE_FLOOR://本機樓層
		KeyName = _T("SYSTEM_MACHINE_FLOOR");
		break;
	case SYSTEM_MACHINE_ROOM://本機車間
		KeyName = _T("SYSTEM_MACHINE_ROOM");
		break;
	case SYSTEM_MACHINE_LINE://本機線別
		KeyName = _T("SYSTEM_MACHINE_LINE");
		break;
	case SYSTEM_MACHINE_LINE_LB://本機線別-B軌
		KeyName = _T("SYSTEM_MACHINE_LINE_LB");
		break;
	case SYSTEM_MACHINE_STATION://本機站別
		KeyName = _T("SYSTEM_MACHINE_STATION");
		break;	
	case SYSTEM_MACHINE_STATION_LB://本機站別-B軌
		KeyName = _T("SYSTEM_MACHINE_STATION_LB");
		break;
	case SYSTEM_MACHINE_SN://本機序號
		KeyName = _T("SYSTEM_MACHINE_SN");
		break;
	case SYSTEM_MACHINE_NAME://機台型號
		KeyName = _T("SYSTEM_MACHINE_NAME");
		break;
	case SYSTEM_MACHINE_VENDOR://機台廠商
		KeyName = _T("SYSTEM_MACHINE_VENDOR");
		break;
	case SYSTEM_MACHINE_ALIAS://機台別名
		KeyName = _T("SYSTEM_MACHINE_ALIAS");
		break;
	case SYSTEM_MACHINE_MES_NAME://MES登入名稱
		KeyName = _T("SYSTEM_MACHINE_MES_NAME");
		break;
	case SYSTEM_MACHINE_MES_PASSWORD://MES登入密碼
		KeyName = _T("SYSTEM_MACHINE_MES_PASSWORD");
		break;
	case SYSTEM_MACHINE_MES_DEVICE://MES登入裝置
		KeyName = _T("SYSTEM_MACHINE_MES_DEVICE");
		break;
	case SYSTEM_MACHINE_MES_DEVICE_2://MES登入裝置-2
		KeyName = _T("SYSTEM_MACHINE_MES_DEVICE_2");
		break;
	case SYSTEM_MACHINE_MES_CODE_NAME://MES-裝置代號
		KeyName = _T("SYSTEM_MACHINE_MES_CODE_NAME");
		break;

	case SYSTEM_AOI_CUSTOMER_ID://客戶編號
		KeyName = _T("SYSTEM_AOI_CUSTOMER_ID");
		break;
	case SYSTEM_CPU_MAX_COUNT_USED://Cpu使用數量
		KeyName = _T("SYSTEM_CPU_MAX_COUNT_USED");
		break;
	case SYSTEM_THREAD_CNT_SLICE_FILL://多執行緒數量-畫面填圖
		KeyName = _T("SYSTEM_THREAD_CNT_SLICE_FILL");
		break;
	case SYSTEM_THREAD_CNT_FRAME_MERGE://多執行緒數量-影像合併
		KeyName = _T("SYSTEM_THREAD_CNT_FRAME_MERGE");
		break;
	case SYSTEM_THREAD_CNT_FIELD_MERGE://多執行緒數量-區域合併
		KeyName = _T("SYSTEM_THREAD_CNT_FIELD_MERGE");
		break;
	case SYSTEM_THREAD_CNT_FRAME_LOAD://多執行緒數量-影像載入
		KeyName = _T("SYSTEM_THREAD_CNT_FRAME_LOAD");
		break;
	case SYSTEM_THREAD_CNT_REGION_CALC://多執行緒數量-區域計算
		KeyName = _T("SYSTEM_THREAD_CNT_REGION_CALC");
		break;
	case SYSTEM_THREAD_CNT_PROC_IDLE://多執行緒數量-閒置核心	
		KeyName = _T("SYSTEM_THREAD_CNT_PROC_IDLE");
		break;
	case SYSTEM_THREAD_CNT_GRAB_IDLE://多執行緒數量-取像閒置核心	
		KeyName = _T("SYSTEM_THREAD_CNT_GRAB_IDLE");
		break;
	case SYSTEM_OPEN_MP_CNT_GENERAL://OpenMP核心數量-一般操作
		KeyName = _T("SYSTEM_OPEN_MP_CNT_GENERAL");
		break;
	case SYSTEM_OPEN_MP_CNT_INSPECTION://OpenMP核心數量-檢測中	
		KeyName = _T("SYSTEM_OPEN_MP_CNT_INSPECTION");
		break;
	case SYSTEM_OPEN_MP_CHK_SIZE_INSPECTION://OpenMP確認尺寸-檢測中
		KeyName = _T("SYSTEM_OPEN_MP_CHK_SIZE_INSPECTION");
		break;
	case SYSTEM_SAVE_MSG_MOVING_TIME://是否移動時間狀態訊息
		KeyName = _T("SYSTEM_SAVE_MSG_MOVING_TIME");
		break;
	case SYSTEM_BACKUP_MSG_MOVING_TIME://是否備份移動時間狀態訊息	
		KeyName = _T("SYSTEM_BACKUP_MSG_MOVING_TIME");
		break;
	case SYSTEM_SAVE_MSG_CURRENT_PROCESS://是否儲存現在狀態訊息
		KeyName = _T("SYSTEM_SAVE_MSG_CURRENT_PROCESS");
		break;	
	case SYSTEM_BACKUP_MSG_CURRENT_PROCESS://是否備份現在狀態訊息
		KeyName = _T("SYSTEM_BACKUP_MSG_CURRENT_PROCESS");
		break;
	case SYSTEM_SAVE_MSG_MACHINE_MONITOR://是否儲存機台監控訊息
		KeyName = _T("SYSTEM_SAVE_MSG_MACHINE_MONITOR");
		break;
	case SYSTEM_SAVE_MSG_MACHINE_MONITOR_WEB://是否儲存機台監控訊息-網頁用
		KeyName = _T("SYSTEM_SAVE_MSG_MACHINE_MONITOR_WEB");
		break;
	case SYSTEM_SAVE_CAMERA_ADD_RING_BUFFER_LOG://是否儲存相機增加循環資料訊息
		KeyName = _T("SYSTEM_SAVE_CAMERA_ADD_RING_BUFFER_LOG");
		break;
	case SYSTEM_SHOW_MEMORY_LEAK_MESSAGE://顯示記憶體未釋放訊息
		KeyName = _T("SYSTEM_SHOW_MEMORY_LEAK_MESSAGE");
		break;
	case SYSTEM_SEND_DEBUG_VIEW_STRING://是否送至DebugView視窗
		KeyName = _T("SYSTEM_SEND_DEBUG_VIEW_STRING");
		break;
	case SYSTEM_SAVE_SLICE_FILL_THREAD_LOG://是否儲存影像填滿執行緒訊息
		KeyName = _T("SYSTEM_SAVE_SLICE_FILL_THREAD_LOG");
		break;
	case SYSTEM_SAVE_FRAME_MERGE_THREAD_LOG://是否儲存影像合併執行緒訊息
		KeyName = _T("SYSTEM_SAVE_FRAME_MERGE_THREAD_LOG");
		break;
	case SYSTEM_SAVE_FIELD_MERGE_THREAD_LOG://是否儲存區域合併執行緒訊息
		KeyName = _T("SYSTEM_SAVE_FIELD_MERGE_THREAD_LOG");
		break;
	case SYSTEM_SAVE_FRAME_LOAD_THREAD_LOG://是否儲存影像載入執行緒訊息
		KeyName = _T("SYSTEM_SAVE_FRAME_LOAD_THREAD_LOG");
		break;
	case SYSTEM_SAVE_REGION_CALC_THREAD_LOG://是否儲存區域計算執行緒訊息
		KeyName = _T("SYSTEM_SAVE_REGION_CALC_THREAD_LOG");
		break;
	case SYSTEM_SAVE_SQUENCE_THREAD_LOG://是否儲存系列執行緒訊息
		KeyName = _T("SYSTEM_SAVE_SQUENCE_THREAD_LOG");
		break;
	case SYSTEM_SAVE_FIELD_FRAME_RELEASE_THREAD_LOG://是否儲存視野影像釋放執行緒訊息
		KeyName = _T("SYSTEM_SAVE_FIELD_FRAME_RELEASE_THREAD_LOG");
		break;
	case SYSTEM_SAVE_ONLINE_INSPECTION_THREAD_LOG://是否儲存線上檢測執行緒訊息
		KeyName = _T("SYSTEM_SAVE_ONLINE_INSPECTION_THREAD_LOG");
		break;
	case SYSTEM_SAVE_REMOVE_FOLDER_THREAD_LOG://是否儲存刪除資料夾執行緒訊息
		KeyName = _T("SYSTEM_SAVE_REMOVE_FOLDER_THREAD_LOG");
		break;
	case SYSTEM_SAVE_CONVEYER_PRE_RUN_THREAD_LOG://是否儲存軌道自動運轉執行緒訊息
		KeyName = _T("SYSTEM_SAVE_CONVEYER_PRE_RUN_THREAD_LOG");
		break;
	case SYSTEM_SAVE_ITS_PROC_THREAD_LOG://是否儲存ITS運作執行緒訊息
		KeyName = _T("SYSTEM_SAVE_ITS_PROC_THREAD_LOG");
		break;
	case SYSTEM_SAVE_LOAD_REPAIR_FILE_THREAD_LOG://是否儲存載入維修站檔案執行緒訊息
		KeyName = _T("SYSTEM_SAVE_LOAD_REPAIR_FILE_THREAD_LOG");
		break;
	case SYSTEM_SAVE_REPAIR_RESULT_SIGNAL_THREAD_LOG://是否儲存維修站訊息執行緒訊息
		KeyName = _T("SYSTEM_SAVE_REPAIR_RESULT_SIGNAL_THREAD_LOG");
		break;
	case SYSTEM_SAVE_SIMPLE_JOB_THREAD_LOG://是否儲存簡易作業執行緒訊息
		KeyName = _T("SYSTEM_SAVE_SIMPLE_JOB_THREAD_LOG");
		break;
	case SYSTEM_SAVE_HASI_MONITOR_THREAD_LOG://是否儲存HASI執行緒訊息
		KeyName = _T("SYSTEM_SAVE_HASI_MONITOR_THREAD_LOG");
		break;
	case SYSTEM_SAVE_TEST_OBJECT_FINISH_LOG://是否儲存檢測物件結束訊息
		KeyName = _T("SYSTEM_SAVE_TEST_OBJECT_FINISH_LOG");
		break;
	case SYSTEM_SAVE_UI_DRAW_FUNC_LOG://是否儲存介面重繪函式訊息
		KeyName = _T("SYSTEM_SAVE_UI_DRAW_FUNC_LOG");
		break;		
	case SYSTEM_SAVE_USER_OPERATION_LOG://是否儲存使用者操作訊息
		KeyName = _T("SYSTEM_SAVE_USER_OPERATION_LOG");
		break;
	case SYSTEM_SAVE_TEST_TRACK_FILE: //是否儲存檢測追蹤檔案
		KeyName = _T("SYSTEM_SAVE_TEST_TRACK_FILE");
		break;
	case SYSTEM_SAVE_PROJECT_REPORT_TEXT://是否專案文字檔報告
		KeyName = _T("SYSTEM_SAVE_PROJECT_REPORT_TEXT");
		break;	
	case SYSTEM_SAVE_PROJECT_REPORT_TEXT_FILENAME_MODE://儲存專案文字檔報告檔名
		KeyName = _T("SYSTEM_SAVE_PROJECT_REPORT_TEXT_FILENAME_MODE");
		break;
	case SYSTEM_SAVE_CUSTOMER_REPORT_FILE://是否儲存客戶報告模式
		KeyName = _T("SYSTEM_SAVE_CUSTOMER_REPORT_FILE");
		break;
	case SYSTEM_SAVE_PROJECT_SPC_FILE_MODE://儲存專案SPC檔案模式	
		KeyName = _T("SYSTEM_SAVE_PROJECT_SPC_FILE_MODE");
		break;
	case SYSTEM_SAVE_PROJECT_SPC_LIBRARY_MODE://儲存專案SPC資料庫模式
		KeyName = _T("SYSTEM_SAVE_PROJECT_SPC_LIBRARY_MODE");
		break;
	case SYSTEM_SAVE_PROJECT_REPORT_WND_READING://是否儲存專案檢測框數據檔案	
		KeyName = _T("SYSTEM_SAVE_PROJECT_REPORT_WND_READING");
		break;
	case SYSTEM_PRE_LOAD_PROJECT_OFFLINE_IMAGE://提前載入專案離線圖檔
		KeyName = _T("SYSTEM_PRE_LOAD_PROJECT_OFFLINE_IMAGE");
		break;
	case SYSTEM_AUTO_RELEASE_FIELD_FRAME_BUFFER://自動釋放區域影像
		KeyName = _T("SYSTEM_AUTO_RELEASE_FIELD_FRAME_BUFFER");
		break;
	case SYSTEM_AUTO_RELEASE_OFFLINE_FIELD_FRAME_BUFFER://自動釋放離線編程區域影像
		KeyName = _T("SYSTEM_AUTO_RELEASE_OFFLINE_FIELD_FRAME_BUFFER");
		break;	
	case SYSTEM_MACHINE_MODEL_TYPE://設備機種樣式
		KeyName = _T("SYSTEM_MACHINE_MODEL_TYPE");
		break;
	case SYSTEM_MACHINE_CAMERA_SIDE://設備相機方向
		KeyName = _T("SYSTEM_MACHINE_CAMERA_SIDE");
		break;	
	case SYSTEM_MULTI_LANGUAGE_MODE://多國語系版本
		KeyName = _T("SYSTEM_MULTI_LANGUAGE_MODE");
		break;	
	case SYSTEM_CAMERA_DEBAYER_MODE://影像還元彩色模式
		KeyName = _T("SYSTEM_CAMERA_DEBAYER_MODE");
		break;
	case SYSTEM_DC_STRECTCH_BLT_MODE://繪圖模式
		KeyName = _T("SYSTEM_DC_STRECTCH_BLT_MODE");
		break;	
	case SYSTEM_IMAGE_DISPLAY_MODE://影像顯示模式
		KeyName = _T("SYSTEM_IMAGE_DISPLAY_MODE");
		break;
	case SYSTEM_IMAGE_DISPLAY_ENHANCE_MODE://影像顯示強化模式
		KeyName = _T("SYSTEM_IMAGE_DISPLAY_ENHANCE_MODE");
		break;
	case SYSTEM_IMAGE_DISPLAY_GAIN://影像顯示的Gain
		KeyName = _T("SYSTEM_IMAGE_DISPLAY_GAIN");
		break;
	case SYSTEM_IMAGE_DISPLAY_GAMMA://影像顯示的Gamma
		KeyName = _T("SYSTEM_IMAGE_DISPLAY_GAMMA");
		break;
	case SYSTEM_IMAGE_DISPLAY_SHARPNESS_RADIUS://影像顯示的銳利化的半徑
		KeyName = _T("SYSTEM_IMAGE_DISPLAY_SHARPNESS_RADIUS");
		break;
	case SYSTEM_IMAGE_DISPLAY_SHARPNESS_AMOUNT://影像顯示的銳利化的總量
		KeyName = _T("SYSTEM_IMAGE_DISPLAY_SHARPNESS_AMOUNT");
		break;
	case SYSTEM_IMAGE_DISPLAY_SHARPNESS_THRESHOLD://影像顯示的銳利化的閥值
		KeyName = _T("SYSTEM_IMAGE_DISPLAY_SHARPNESS_THRESHOLD");
		break;
	case SYSTEM_IMAGE_DISPLAY_LOCAL_GAMMA_CALC_SIZE://影像顯示的局部Gamma的計算範圍
		KeyName = _T("SYSTEM_IMAGE_DISPLAY_LOCAL_GAMMA_CALC_SIZE");
		break;
	case SYSTEM_IMAGE_DISPLAY_LOCAL_GAMMA_SCALE_VAL://影像顯示的局部Gamma的縮放比例
		KeyName = _T("SYSTEM_IMAGE_DISPLAY_LOCAL_GAMMA_SCALE_VAL");
		break;
	case SYSTEM_IMAGE_DISPLAY_MAX_ZOOM_SCALE://影像顯示最大縮放比例	
		KeyName = _T("SYSTEM_IMAGE_DISPLAY_MAX_ZOOM_SCALE");
		break;
	case SYSTEM_ONLINE_FORM_VIEW_MODE://線上檢測介面模式	
		KeyName = _T("SYSTEM_ONLINE_FORM_VIEW_MODE");
		break;
	case SYSTEM_CALC_FOCUS_SMOOTH_SIZE://計算焦距平滑尺寸
		KeyName = _T("SYSTEM_CALC_FOCUS_SMOOTH_SIZE");
		break;
	case SYSTEM_CALC_FOCUS_MODE://計算焦距模式
		KeyName = _T("SYSTEM_CALC_FOCUS_MODE");
		break;
	case SYSTEM_LIBRARY_MODE_MATCH://影像匹配函式庫樣式
		KeyName = _T("SYSTEM_LIBRARY_MODE_MATCH");
		break;
	case SYSTEM_HONEYWELL_SWIFT_DECODER_ENABLED://Honeywell SwiftDecoder啟用
		KeyName = _T("SYSTEM_HONEYWELL_SWIFT_DECODER_ENABLED");
		break;
	case SYSTEM_DONGLE_WARNING_REMAINING_DAYS://硬體鎖警告剩餘天數
		KeyName = _T("SYSTEM_DONGLE_WARNING_REMAINING_DAYS");
		break;
	case SYSTEM_DONGLE_WARNING_REMAINING_COUNT://硬體鎖警告剩餘次數
		KeyName = _T("SYSTEM_DONGLE_WARNING_REMAINING_COUNT");
		break;
	case SYSTEM_CONNECT_LAST_STATION_MODE://與上一站連線方式
		KeyName = _T("SYSTEM_CONNECT_LAST_STATION_MODE");
		break;	
	case SYSTEM_LANE_WORK_MODEL_LA://A軌道運轉模式
		KeyName = _T("SYSTEM_LANE_WORK_MODEL_LA");
		break;
	case SYSTEM_LANE_WORK_MODEL_LB://B軌道運轉模式
		KeyName = _T("SYSTEM_LANE_WORK_MODEL_LB");
		break;
	case SYSTEM_MULTI_TOWER_LIGHT_MODE://多塔燈模式
		KeyName = _T("SYSTEM_MULTI_TOWER_LIGHT_MODE");
		break;
	case SYSTEM_CHECK_PCB_REMOVED_COUNT://確認PCB板移走次數
		KeyName = _T("SYSTEM_CHECK_PCB_REMOVED_COUNT");
		break;
	case SYSTEM_NEXT_CONNECTED_BUFFER_TYPE://確認PCB下一站連接輸送帶樣式
		KeyName = _T("SYSTEM_NEXT_CONNECTED_BUFFER_TYPE");
		break;
	case SYSTEM_MULTI_PROJECT_TEST_ORDER_MODE://多專案檢測次序模式		
		KeyName = _T("SYSTEM_MULTI_PROJECT_TEST_ORDER_MODE");
		break;
	case SYSTEM_ONLINE_INPUT_PROJECT_WORK_NUMBER://在線輸入-專案工單號碼
		KeyName = _T("SYSTEM_ONLINE_INPUT_PROJECT_WORK_NUMBER");
		break;
	case SYSTEM_ONLINE_AUTO_CALIBRATION_DLP_LED_COLOR://在線自動校正DLP-LED-顏色
		KeyName = _T("SYSTEM_ONLINE_AUTO_CALIBRATION_DLP_LED_COLOR");
		break;
	case SYSTEM_ONLINE_AUTO_CALIBRATION_CAP_DELAY_TIME://在線自動校正上蓋延遲時間-ms
		KeyName = _T("SYSTEM_ONLINE_AUTO_CALIBRATION_CAP_DELAY_TIME");
		break;
	case SYSTEM_ONLINE_AUTO_CALIBRATION_MODE_XYZ_HOME://在線自動校正模式-XYZ歸零
		KeyName = _T("SYSTEM_ONLINE_AUTO_CALIBRATION_MODE_XYZ_HOME");
		break;
	case SYSTEM_ONLINE_AUTO_CALIBRATION_PERIOD_XYZ_HOME://在線自動校正週期-XYZ歸零-小時
		KeyName = _T("SYSTEM_ONLINE_AUTO_CALIBRATION_PERIOD_XYZ_HOME");
		break;
	case SYSTEM_ONLINE_AUTO_CALIBRATION_MODE_2D_CURRENT://在線自動校正模式-2D電流
		KeyName = _T("SYSTEM_ONLINE_AUTO_CALIBRATION_MODE_2D_CURRENT");
		break;
	case SYSTEM_ONLINE_AUTO_CALIBRATION_PERIOD_2D_CURRENT://在線自動校正週期-2D電流-小時
		KeyName = _T("SYSTEM_ONLINE_AUTO_CALIBRATION_PERIOD_2D_CURRENT");
		break;
	case SYSTEM_ONLINE_AUTO_CALIBRATION_PERIOD_3D_CURRENT://在線自動校正週期-3D電流-小時
		KeyName = _T("SYSTEM_ONLINE_AUTO_CALIBRATION_PERIOD_3D_CURRENT");
		break;
	case SYSTEM_ONLINE_AUTO_CALIBRATION_MODE_3D_ZERO_PLANE://在線自動校正模式-3D相平面
		KeyName = _T("SYSTEM_ONLINE_AUTO_CALIBRATION_MODE_3D_ZERO_PLANE");
		break;
	case SYSTEM_ONLINE_AUTO_CALIBRATION_PERIOD_3D_ZERO_PLANE://在線自動校正週期-3D相平面-小時
		KeyName = _T("SYSTEM_ONLINE_AUTO_CALIBRATION_PERIOD_3D_ZERO_PLANE");
		break;
	case SYSTEM_ONLINE_AUTO_CALIBRATION_PERIOD_3D_HEIGHT_FACTOR://在線自動校正週期-3D高度比例-小時
		KeyName = _T("SYSTEM_ONLINE_AUTO_CALIBRATION_PERIOD_3D_HEIGHT_FACTOR");
		break;
	case SYSTEM_ONLINE_AUTO_STOP_BY_IDLE_TIME://在線自動停機-閒置時間-分鐘
		KeyName = _T("SYSTEM_ONLINE_AUTO_STOP_BY_IDLE_TIME");
		break;
	case SYSTEM_ONLINE_AUTO_STOP_BY_SPEC_TIME_1://在線自動停機-特定時間-時時分分秒秒
		KeyName = _T("SYSTEM_ONLINE_AUTO_STOP_BY_SPEC_TIME_1");
		break;
	case SYSTEM_ONLINE_AUTO_STOP_BY_SPEC_TIME_2://在線自動停機-特定時間-時時分分秒秒
		KeyName = _T("SYSTEM_ONLINE_AUTO_STOP_BY_SPEC_TIME_2");
		break;
	case SYSTEM_ONLINE_AUTO_STOP_BY_SPEC_TIME_3://在線自動停機-特定時間-時時分分秒秒
		KeyName = _T("SYSTEM_ONLINE_AUTO_STOP_BY_SPEC_TIME_3");
		break;
	case SYSTEM_AUTO_SWITCH_TO_ONILINEVIEW_TIME://自動切換線上畫面-秒	
		KeyName = _T("SYSTEM_AUTO_SWITCH_TO_ONILINEVIEW_TIME");
		break;
	case SYSTEM_ONLINE_SHOW_PROJECT_TEST_MAP://在線顯示專案檢測底圖
		KeyName = _T("SYSTEM_ONLINE_SHOW_PROJECT_TEST_MAP");
		break;
	case SYSTEM_AUTO_SWITCH_TO_ONILINE_REMOTE_CTRL_TIME://自動切換線上遠端控制時間-毫秒
		KeyName = _T("SYSTEM_AUTO_SWITCH_TO_ONILINE_REMOTE_CTRL_TIME");
		break;
	case SYSTEM_AUTO_SWITCH_TO_ONILINE_REMOTE_CTRL_MODE://自動切換線上遠端控制模式
		KeyName = _T("SYSTEM_AUTO_SWITCH_TO_ONILINE_REMOTE_CTRL_MODE");
		break;
	case SYSTEM_PHASE_PERIOD_1://第1個相位的週期
		KeyName = _T("SYSTEM_PHASE_PERIOD_1");
		break;
	case SYSTEM_PHASE_PERIOD_2://第2個相位的週期
		KeyName = _T("SYSTEM_PHASE_PERIOD_2");
		break;
	case SYSTEM_PHASE_PERIOD_3://第3個相位的週期
		KeyName = _T("SYSTEM_PHASE_PERIOD_3");
		break;
	case SYSTEM_SEPARATE_DLP_2EXP_TABLE://分離DLP2次曝光表格
		KeyName = _T("SYSTEM_SEPARATE_DLP_2EXP_TABLE");
		break;
	case SYSTEM_PHASE_CONVERT_HEIGHT_MODE://相位轉換高度模式
		KeyName = _T("SYSTEM_PHASE_CONVERT_HEIGHT_MODE");
		break;
	case SYSTEM_PHASE_NOISE_DEFINE://相位雜訊定義啟用
		KeyName = _T("SYSTEM_PHASE_NOISE_DEFINE");
		break;
	case SYSTEM_PHASE_NOISE_DEFINE_MODE://相位雜訊定義模式
		KeyName = _T("SYSTEM_PHASE_NOISE_DEFINE_MODE");
		break;
	case SYSTEM_PHASE_NOISE_EXTEND_VOID://相位遮罩-無效點外擴
		KeyName = _T("SYSTEM_PHASE_NOISE_EXTEND_VOID");
		break;
	case SYSTEM_PHASE_NOISE_LOW_CONTRAST://相位遮罩-低對比
		KeyName = _T("SYSTEM_PHASE_NOISE_LOW_CONTRAST");
		break;	
	case SYSTEM_PHASE_NOISE_LOW_POTENTIAL://相位遮罩-低潛力
		KeyName = _T("SYSTEM_PHASE_NOISE_LOW_POTENTIAL");
		break;
	case SYSTEM_PHASE_NOISE_OVER_SATURATED://相位遮罩-過亮度
		KeyName = _T("SYSTEM_PHASE_NOISE_OVER_SATURATED");
		break;	
	case SYSTEM_PHASE_NOISE_LOW_CONTRAST_B://相位遮罩-低對比
		KeyName = _T("SYSTEM_PHASE_NOISE_LOW_CONTRAST_B");
		break;
	case SYSTEM_PHASE_NOISE_LOW_POTENTIAL_B://相位遮罩-低潛力	
		KeyName = _T("SYSTEM_PHASE_NOISE_LOW_POTENTIAL_B");
		break;
	case SYSTEM_PHASE_NOISE_OVER_SATURATED_B://相位遮罩-過亮度	
		KeyName = _T("SYSTEM_PHASE_NOISE_OVER_SATURATED_B");
		break;
	case SYSTEM_PHASE_NOISE_LOW_CONTRAST_C://相位遮罩-低對比
		KeyName = _T("SYSTEM_PHASE_NOISE_LOW_CONTRAST_C");
		break;
	case SYSTEM_PHASE_NOISE_LOW_POTENTIAL_C://相位遮罩-低潛力	
		KeyName = _T("SYSTEM_PHASE_NOISE_LOW_POTENTIAL_C");
		break;
	case SYSTEM_PHASE_NOISE_OVER_SATURATED_C://相位遮罩-過亮度	
		KeyName = _T("SYSTEM_PHASE_NOISE_OVER_SATURATED_C");
		break;	
	case SYSTEM_PHASE_NOISE_SMOOTH_FILTER://相位遮罩-平滑處理
		KeyName = _T("SYSTEM_PHASE_NOISE_SMOOTH_FILTER");
		break;
	case SYSTEM_PHASE_NOISE_LOW_CONTRAST_COLOR://相位遮罩-低對比-顏色
		KeyName = _T("SYSTEM_PHASE_NOISE_LOW_CONTRAST_COLOR");
		break;	
	case SYSTEM_PHASE_NOISE_LOW_POTENTIAL_COLOR://相位遮罩-潛力-顏色
		KeyName = _T("SYSTEM_PHASE_NOISE_LOW_POTENTIAL_COLOR");
		break;
	case SYSTEM_PHASE_NOISE_OVER_SATURATED_COLOR://相位遮罩-過亮度-顏色
		KeyName = _T("SYSTEM_PHASE_NOISE_OVER_SATURATED_COLOR");
		break;	
	case SYSTEM_PHASE_NOISE_EXTEND_VOID_COLOR://相位遮罩-無效點外擴-顏色
		KeyName = _T("SYSTEM_PHASE_NOISE_EXTEND_VOID_COLOR");
		break;	
	case SYSTEM_SPACE_NOISE_HEIGHT_UNEXPECTED_COLOR://相位遮罩-高度異常-顏色
		KeyName = _T("SYSTEM_SPACE_NOISE_HEIGHT_UNEXPECTED_COLOR");
		break;
	case SYSTEM_SPACE_NOISE_HEIGHT_OVER_LOW_COLOR://相位遮罩-過低異常-顏色
		KeyName = _T("SYSTEM_SPACE_NOISE_HEIGHT_OVER_LOW_COLOR");
		break;
	case SYSTEM_SPACE_VALID_BEST_COLOR://相位遮罩-可靠高度-顏色	
		KeyName = _T("SYSTEM_SPACE_VALID_BEST_COLOR");
		break;

	case SYSTEM_3D_OBJECT_DRAW_SCALE_X://3D物件顯示比例-X		
		KeyName = _T("SYSTEM_3D_OBJECT_DRAW_SCALE_X");
		break;
	case SYSTEM_3D_OBJECT_DRAW_SCALE_Y://3D物件顯示比例-Y
		KeyName = _T("SYSTEM_3D_OBJECT_DRAW_SCALE_Y");
		break;
	case SYSTEM_3D_OBJECT_DRAW_SCALE_Z://3D物件顯示比例-Z
		KeyName = _T("SYSTEM_3D_OBJECT_DRAW_SCALE_Z");
		break;

	case SYSTEM_PANEL_COLOR_1://整板的顏色-1
		KeyName = _T("SYSTEM_PANEL_COLOR_1");
		break;
	case SYSTEM_PANEL_COLOR_2://整板的顏色-2
		KeyName = _T("SYSTEM_PANEL_COLOR_2");
		break;
	case SYSTEM_PANEL_TEXT_COLOR://整板文字顏色	
		KeyName = _T("SYSTEM_PANEL_TEXT_COLOR");
		break;
	case SYSTEM_PANEL_SELECTED_COLOR://整板選取到的顏色
		KeyName = _T("SYSTEM_PANEL_SELECTED_COLOR");
		break;
	case SYSTEM_BOARD_COLOR_1://單板的顏色-1
		KeyName = _T("SYSTEM_BOARD_COLOR_1");
		break;
	case SYSTEM_BOARD_COLOR_2://單板的顏色-2	
		KeyName = _T("SYSTEM_BOARD_COLOR_2");
		break;
	case SYSTEM_BOARD_TEXT_COLOR://單板文字顏色	
		KeyName = _T("SYSTEM_BOARD_TEXT_COLOR");
		break;
	case SYSTEM_BOARD_SELECTED_COLOR://單板選取到的顏色
		KeyName = _T("SYSTEM_BOARD_SELECTED_COLOR");
		break;

	case SYSTEM_FD_COLOR_1://定位點的顏色-1
		KeyName = _T("SYSTEM_FD_COLOR_1");
		break;
	case SYSTEM_FD_COLOR_2://定位點的顏色-2		
		KeyName = _T("SYSTEM_FD_COLOR_2");
		break;
	case SYSTEM_FD_TEXT_COLOR://定位點文字顏色
		KeyName = _T("SYSTEM_FD_TEXT_COLOR");
		break;
	case SYSTEM_FD_SELECTED_COLOR://定位點選取到的顏色
		KeyName = _T("SYSTEM_FD_SELECTED_COLOR");
		break;

	case SYSTEM_BARCODE_COLOR_1://條碼的顏色-1
		KeyName = _T("SYSTEM_BARCODE_COLOR_1");
		break;
	case SYSTEM_BARCODE_COLOR_2://條碼的顏色-2	
		KeyName = _T("SYSTEM_BARCODE_COLOR_2");
		break;
	case SYSTEM_BARCODE_TEXT_COLOR://條碼文字顏色
		KeyName = _T("SYSTEM_BARCODE_TEXT_COLOR");
		break;
	case SYSTEM_BARCODE_SELECTED_COLOR://條碼選取到的顏
		KeyName = _T("SYSTEM_BARCODE_SELECTED_COLOR");
		break;

	case SYSTEM_COMPONENT_COLOR_1://零件的顏色-1
		KeyName = _T("SYSTEM_COMPONENT_COLOR_1");
		break;
	case SYSTEM_COMPONENT_COLOR_2://零件的顏色-2
		KeyName = _T("SYSTEM_COMPONENT_COLOR_2");
		break;
	case SYSTEM_COMPONENT_TEXT_COLOR://零件文字顏色
		KeyName = _T("SYSTEM_COMPONENT_TEXT_COLOR");
		break;
	case SYSTEM_COMPONENT_SELECTED_COLOR://零件選取到的顏色
		KeyName = _T("SYSTEM_COMPONENT_SELECTED_COLOR");
		break;
	case SYSTEM_DISTRICT_COLOR_1://分段的顏色-1
		KeyName = _T("SYSTEM_DISTRICT_COLOR_1");
		break;
	case SYSTEM_DISTRICT_COLOR_2://分段的顏色-2
		KeyName = _T("SYSTEM_DISTRICT_COLOR_2");
		break;
	case SYSTEM_INSPECTED_RESULT_OK_COLOR://檢測OK顏色
		KeyName = _T("SYSTEM_INSPECTED_RESULT_OK_COLOR");
		break;
	case SYSTEM_INSPECTED_RESULT_NG_COLOR://檢測NG顏色
		KeyName = _T("SYSTEM_INSPECTED_RESULT_NG_COLOR");
		break;
	case SYSTEM_INSPECTED_RESULT_SKIP_COLOR://檢測Skip顏色
		KeyName = _T("SYSTEM_INSPECTED_RESULT_SKIP_COLOR");
		break;
	case SYSTEM_INSPECTED_RESULT_BYPASS_COLOR://檢測Bypass顏色
		KeyName = _T("SYSTEM_INSPECTED_RESULT_BYPASS_COLOR");
		break;
	case SYSTEM_INSPECTED_RESULT_UNTEST_COLOR://檢測UnTest顏色
		KeyName = _T("SYSTEM_INSPECTED_RESULT_UNTEST_COLOR");
		break;
	case SYSTEM_INSPECTED_RESULT_WARNING_COLOR://檢測Warning顏色
		KeyName = _T("SYSTEM_INSPECTED_RESULT_WARNING_COLOR");
		break;
	case SYSTEM_INSPECTED_RESULT_EXCEPTION_COLOR://檢測Exception顏色
		KeyName = _T("SYSTEM_INSPECTED_RESULT_EXCEPTION_COLOR");
		break;
	case SYSTEM_SPACE_NOISE_SINGLE_CAST_LOW_LIMIT://高度雜訊單投光高度最低極限-um
		KeyName = _T("SYSTEM_SPACE_NOISE_SINGLE_CAST_LOW_LIMIT");
		break;
	case SYSTEM_SPACE_NOISE_MULTI_CAST_PATCH_SIZE://高度雜訊多投光合併Patch尺寸
		KeyName = _T("SYSTEM_SPACE_NOISE_MULTI_CAST_PATCH_SIZE");
		break;
	case SYSTEM_SPACE_NOISE_MULTI_CAST_MERGE_MODE://高度雜訊多投光合併模式
		KeyName = _T("SYSTEM_SPACE_NOISE_MULTI_CAST_MERGE_MODE");
		break;
	case SYSTEM_SPACE_NOISE_MULTI_CAST_MERGE_BEST_MODE://高度雜訊多投光合併最可靠模式
		KeyName = _T("SYSTEM_SPACE_NOISE_MULTI_CAST_MERGE_BEST_MODE");
		break;
	case SYSTEM_SPACE_NOISE_MULTI_INTENSITY_MERGE_MODE://高度雜訊多亮度合併模式-Mean, MaxB
		KeyName = _T("SYSTEM_SPACE_NOISE_MULTI_INTENSITY_MERGE_MODE");
		break;
	case SYSTEM_SPACE_NOISE_MULTI_CAST_MIN_VALID_COUNT://高度雜訊多投光最少有效值數	
		KeyName = _T("SYSTEM_SPACE_NOISE_MULTI_CAST_MIN_VALID_COUNT");
		break;
	case SYSTEM_SPACE_NOISE_MULTI_CAST_MAX_DIFFERENCE://高度雜訊多投光高度最大差值-um
		KeyName = _T("SYSTEM_SPACE_NOISE_MULTI_CAST_MAX_DIFFERENCE");
		break;		
	case SYSTEM_SPACE_NOISE_MULTI_CAST_LIMIT_DIFFERENCE://高度雜訊多投光高度極限差值-um	
		KeyName = _T("SYSTEM_SPACE_NOISE_MULTI_CAST_LIMIT_DIFFERENCE");
		break;
	case SYSTEM_SPACE_NOISE_MULTI_CAST_VALID_BEST_RATIO://高度雜訊多投光高度最好比例-um	
		KeyName = _T("SYSTEM_SPACE_NOISE_MULTI_CAST_VALID_BEST_RATIO");
		break;
	case SYSTEM_SPACE_NOISE_MULTI_CAST_VALID_DIFFERENCE://高度雜訊多投光高度有效差值-um
		KeyName = _T("SYSTEM_SPACE_NOISE_MULTI_CAST_VALID_DIFFERENCE");
		break;
	case SYSTEM_SPACE_NOISE_MULTI_CAST_OPPOSITE_MAX_GRAY://高度雜訊多投光合併對邊灰階上限-gray	
		KeyName = _T("SYSTEM_SPACE_NOISE_MULTI_CAST_OPPOSITE_MAX_GRAY");
		break;
	case SYSTEM_SPACE_NOISE_CAST_FILTER_MODE://投光後濾波模式
		KeyName = _T("SYSTEM_SPACE_NOISE_CAST_FILTER_MODE");
		break;
	case SYSTEM_SPACE_NOISE_CAST_MEDIAN_FILTER_SIZE://高度雜訊的中值濾波尺寸
		KeyName = _T("SYSTEM_SPACE_NOISE_CAST_MEDIAN_FILTER_SIZE");
		break;
	case SYSTEM_SPACE_NOISE_CAST_MEDIAN_FILTER_USE_SIZE://高度雜訊的中值濾波使用尺寸
		KeyName = _T("SYSTEM_SPACE_NOISE_CAST_MEDIAN_FILTER_USE_SIZE");
		break;
	case SYSTEM_SPACE_NOISE_DATA_VOID_EXPAND_SIZE://空間雜訊無效點外擴尺寸-piexel	
		KeyName = _T("SYSTEM_SPACE_NOISE_DATA_VOID_EXPAND_SIZE");
		break;
	case SYSTEM_SPACE_NOISE_DATA_VOID_EXPAND_ENABLED://空間雜訊無效點外擴啟用
		KeyName = _T("SYSTEM_SPACE_NOISE_DATA_VOID_EXPAND_ENABLED");
		break;
	case SYSTEM_SPACE_NOISE_FIRST_FILTER_MODE://空間雜訊首次濾波模式
		KeyName = _T("SYSTEM_SPACE_NOISE_FIRST_FILTER_MODE");
		break;
	case SYSTEM_SPACE_NOISE_FIRST_FILTER_PITCH://空間雜訊首次濾波步長
		KeyName = _T("SYSTEM_SPACE_NOISE_FIRST_FILTER_PITCH");
		break;
	case SYSTEM_SPACE_NOISE_FIRST_KER_SIZE://空間雜訊首次濾波尺寸
		KeyName = _T("SYSTEM_SPACE_NOISE_FIRST_KER_SIZE");
		break;
	case SYSTEM_SPACE_NOISE_FIRST_USE_SIZE://空間雜訊首次使用尺寸
		KeyName = _T("SYSTEM_SPACE_NOISE_FIRST_USE_SIZE");
		break;	
	case SYSTEM_SPACE_NOISE_OVER_LOW_MODE://高度雜訊過低模式
		KeyName = _T("SYSTEM_SPACE_NOISE_OVER_LOW_MODE");
		break;
	case SYSTEM_SPACE_NOISE_OVER_LOW_RANGE://高度雜訊過低高度-um
		KeyName = _T("SYSTEM_SPACE_NOISE_OVER_LOW_RANGE");
		break;		
	case SYSTEM_SPACE_NOISE_OVER_LOW_LIMIT://高度雜訊過低極限-um	
		KeyName = _T("SYSTEM_SPACE_NOISE_OVER_LOW_LIMIT");
		break;
	case SYSTEM_SPACE_NOISE_OVER_LOW_KER_SIZE://高度雜訊過低模式
		KeyName = _T("SYSTEM_SPACE_NOISE_OVER_LOW_KER_SIZE");
		break;
	case SYSTEM_SPACE_NOISE_OVER_LOW_USE_SIZE://高度雜訊過低使用尺寸
		KeyName = _T("SYSTEM_SPACE_NOISE_OVER_LOW_USE_SIZE");
		break;
	case SYSTEM_SPACE_NOISE_HEIGHT_ABNORMAL_MODE://高度雜訊高度異常模式
		KeyName = _T("SYSTEM_SPACE_NOISE_HEIGHT_ABNORMAL_MODE");
		break;
	case SYSTEM_SPACE_NOISE_HEIGHT_ABNORMAL_PITCH://高度雜訊高度異常步長
		KeyName = _T("SYSTEM_SPACE_NOISE_HEIGHT_ABNORMAL_PITCH");
		break;
	case SYSTEM_SPACE_NOISE_HEIGHT_ABNORMAL_RANGE://高度雜訊高度異常範圍-um		
		KeyName = _T("SYSTEM_SPACE_NOISE_HEIGHT_ABNORMAL_RANGE");
		break;	
	case SYSTEM_SPACE_NOISE_HEIGHT_ABNORMAL_CHK_SIZE://高度雜訊高度異常確認尺寸
		KeyName = _T("SYSTEM_SPACE_NOISE_HEIGHT_ABNORMAL_CHK_SIZE");
		break;
	case SYSTEM_SPACE_NOISE_HEIGHT_ABNORMAL_KER_SIZE://高度雜訊高度異常濾波尺寸
		KeyName = _T("SYSTEM_SPACE_NOISE_HEIGHT_ABNORMAL_KER_SIZE");
		break;
	case SYSTEM_SPACE_NOISE_HEIGHT_ABNORMAL_USE_SIZE://高度雜訊高度異常使用尺寸
		KeyName = _T("SYSTEM_SPACE_NOISE_HEIGHT_ABNORMAL_USE_SIZE");
		break;	
	case SYSTEM_SPACE_NOISE_HEIGHT_ABNORMAL_REPEAT_CNT://高度雜訊高度異常重複次數
		KeyName = _T("SYSTEM_SPACE_NOISE_HEIGHT_ABNORMAL_REPEAT_CNT");
		break;
	case SYSTEM_SPACE_NOISE_RECONTRUCTED_EXT_SIZE://空間雜訊重建外擴尺寸
		KeyName = _T("SYSTEM_SPACE_NOISE_RECONTRUCTED_EXT_SIZE");
		break;
	case SYSTEM_SPACE_NOISE_RECONTRUCTED_ENABLED://空間雜訊重建外擴啟用	
		KeyName = _T("SYSTEM_SPACE_NOISE_RECONTRUCTED_ENABLED");
		break;	
	case SYSTEM_SPACE_NOISE_FINAL_FILTER_MODE://空間雜訊最後濾波模式
		KeyName = _T("SYSTEM_SPACE_NOISE_FINAL_FILTER_MODE");
		break;	
	case SYSTEM_SPACE_NOISE_FINAL_FILTER_PITCH://空間雜訊最後濾波步長
		KeyName = _T("SYSTEM_SPACE_NOISE_FINAL_FILTER_PITCH");
		break;
	case SYSTEM_SPACE_NOISE_FINAL_KER_SIZE://空間雜訊最後濾波尺寸
		KeyName = _T("SYSTEM_SPACE_NOISE_FINAL_KER_SIZE");
		break;
	case SYSTEM_SPACE_NOISE_FINAL_USE_SIZE://空間雜訊最後使用尺寸
		KeyName = _T("SYSTEM_SPACE_NOISE_FINAL_USE_SIZE");
		break;
	case SYSTEM_SPACE_NOISE_FINAL_FILTER_MODE2://空間雜訊最後濾波模式-2
		KeyName = _T("SYSTEM_SPACE_NOISE_FINAL_FILTER_MODE2");
		break;
	case SYSTEM_SPACE_NOISE_FINAL_FILTER_PITCH2://空間雜訊最後濾波步長-2
		KeyName = _T("SYSTEM_SPACE_NOISE_FINAL_FILTER_PITCH2");
		break;
	case SYSTEM_SPACE_NOISE_FINAL_KER_SIZE2://空間雜訊最後濾波尺寸-2
		KeyName = _T("SYSTEM_SPACE_NOISE_FINAL_KER_SIZE2");
		break;
	case SYSTEM_SPACE_NOISE_FINAL_USE_SIZE2://空間雜訊最後使用尺寸-2
		KeyName = _T("SYSTEM_SPACE_NOISE_FINAL_USE_SIZE2");
		break;
	case SYSTEM_SPACE_VERIFY_MULTI_CAST_GAP_RATIO://高度驗證-多投光差距比例上限-%
		KeyName = _T("SYSTEM_SPACE_VERIFY_MULTI_CAST_GAP_RATIO");
		break;
	case SYSTEM_SPACE_VERIFY_MULTI_CAST_GAP_THRESHOLD://高度驗證-多投光差距閥值-um	
		KeyName = _T("SYSTEM_SPACE_VERIFY_MULTI_CAST_GAP_THRESHOLD");
		break;
	case SYSTEM_CUDA_FUNCTION_ENABLED://Cuda函式啟用
		KeyName = _T("SYSTEM_CUDA_FUNCTION_ENABLED");
		break;
	case SYSTEM_CUDA_NUMBER_BLOCK://Cuda Block數量
		KeyName = _T("SYSTEM_CUDA_NUMBER_BLOCK");
		break;	
	case SYSTEM_CUDA_NUMBER_THREAD://Cuda Thread數量
		KeyName = _T("SYSTEM_CUDA_NUMBER_THREAD");
		break;	
	case SYSTEM_AUTO_RETRY_MAX_COUNT://自動重測次數上限
		KeyName = _T("SYSTEM_AUTO_RETRY_MAX_COUNT");
		break;
	case SYSTEM_AUTO_SETUP_DLP_PATTERN://自動設定DLP樣板	
		KeyName = _T("SYSTEM_AUTO_SETUP_DLP_PATTERN");
		break;
	case SYSTEM_RESOLUTION_MODE://解析度模式
		KeyName = _T("SYSTEM_RESOLUTION_MODE");
		break;		
	case SYSTEM_RESOLUTION_SHOW_SCALE://解析度顯示比例
		KeyName = _T("SYSTEM_RESOLUTION_SHOW_SCALE");
		break;
	case SYSTEM_MOVE_CAMERA_BEFORE_PCB_IN://進板前移動相機頭
		KeyName = _T("SYSTEM_MOVE_CAMERA_BEFORE_PCB_IN");
		break;
	case SYSTEM_PCB_IN_USE_CAMERA_IMAGE_MODE://進板使用相機影像模式
		KeyName = _T("SYSTEM_PCB_IN_USE_CAMERA_IMAGE_MODE");
		break;
	case SYSTEM_CLAMP_PCB_BEFORE_TEST_MODE://檢測前夾板模式	
		KeyName = _T("SYSTEM_CLAMP_PCB_BEFORE_TEST_MODE");
		break;
	case SYSTEM_GRAB_FIDUCIAL_DELAY_TIME://解取定位點影像延遲時間
		KeyName = _T("SYSTEM_GRAB_FIDUCIAL_DELAY_TIME");
		break;	
	case SYSTEM_PCB_OUT_DIRECTION://PCB出板方向
		KeyName = _T("SYSTEM_PCB_OUT_DIRECTION");
		break;
	case SYSTEM_BYPASS_LAST_SIGNAL://忽略上一站訊號-回板使用
		KeyName = _T("SYSTEM_BYPASS_LAST_SIGNAL");
		break;
	case SYSTEM_BYPASS_NEXT_SIGNAL://忽略下一站訊號-回板使用	
		KeyName = _T("SYSTEM_BYPASS_NEXT_SIGNAL");
		break;
	case SYSTEM_PCB_OK_NG_SIGNAL_DELAY_TIME://PCB OK/NG訊號延遲時間-ms
		KeyName = _T("SYSTEM_PCB_OK_NG_SIGNAL_DELAY_TIME");
		break;
	case SYSTEM_EDIT_LINE_SIZE_LEVEL://編輯線尺寸的層級
		KeyName = _T("SYSTEM_EDIT_LINE_SIZE_LEVEL");
		break;	
	case SYSTEM_ONLINE_TUNING_KEEP_MAX_TIME://線上調機保留最久時間-分鐘 	
		KeyName = _T("SYSTEM_ONLINE_TUNING_KEEP_MAX_TIME");
		break;
	case SYSTEM_ONLINE_TUNING_SAVED_MAX_COUNT://線上調機儲存最多數量-片數 
		KeyName = _T("SYSTEM_ONLINE_TUNING_SAVED_MAX_COUNT");
		break;
	case SYSTEM_USER_LOGIN_ENABLED://使用者登入使用
		KeyName = _T("SYSTEM_USER_LOGIN_ENABLED");
		break;	
	case SYSTEM_USER_LOGIN_OPTIONS://使用者登入選項
		KeyName = _T("SYSTEM_USER_LOGIN_OPTIONS");
		break;
	case SYSTEM_OPEN_PROJECT_MODE://開啟專案模式
		KeyName = _T("SYSTEM_OPEN_PROJECT_MODE");		
		break;
	case SYSTEM_OPEN_PROJECT_MAP_INDEX://開啟專案底圖編號
		KeyName = _T("SYSTEM_OPEN_PROJECT_MAP_INDEX");
		break;	
	case SYSTEM_VERIFY_PROJECT_MODE://驗證專案模式
		KeyName = _T("SYSTEM_VERIFY_PROJECT_MODE");
		break;
	case SYSTEM_VERIFY_PROJECT_FILENAME://驗證專案的檔名
		KeyName = _T("SYSTEM_VERIFY_PROJECT_FILENAME");
		break;
	case SYSTEM_MODEL_NAME_USE_PART_NUMBER://模組名稱使用料號 
		KeyName = _T("SYSTEM_MODEL_NAME_USE_PART_NUMBER");
		break;
	case SYSTEM_ONLINE_OPEN_PROJECT_MODE://線上開專案模式
		KeyName = _T("SYSTEM_ONLINE_OPEN_PROJECT_MODE");		
		break;
	case SYSTEM_ONLINE_OPEN_PROJECT_CAMERA_BARCODE://線上開專案-相機條碼
		KeyName = _T("SYSTEM_ONLINE_OPEN_PROJECT_CAMERA_BARCODE");
		break;
	case SYSTEM_ONLINE_OPEN_PROJECT_BARCODE_DEVICE_GRAB_MODE://線上開專案外接條碼取像模式
		KeyName = _T("SYSTEM_ONLINE_OPEN_PROJECT_BARCODE_DEVICE_GRAB_MODE");		
		break;
	case SYSTEM_MODEL_UNSET_COLOR://模組未設定顏色
		KeyName = _T("SYSTEM_MODEL_UNSET_COLOR");
		break;
	case SYSTEM_BINARY_MASK_COLOR_RGB://2值化遮罩-顏色-3色燈
		KeyName = _T("SYSTEM_BINARY_MASK_COLOR_RGB");
		break;
	case SYSTEM_BINARY_MASK_COLOR_DEFAULT://2值化遮罩-顏色-預設
		KeyName = _T("SYSTEM_BINARY_MASK_COLOR_DEFAULT");
		break;
	case SYSTEM_BINARY_MASK_COLOR_ALPHA://2值化遮罩-顏色-透明度	
		KeyName = _T("SYSTEM_BINARY_MASK_COLOR_ALPHA");
		break;
	case SYSTEM_INSPECTION_FINISH_SHOW_RESULT_LIST://檢測結束顯示結果列表 
		KeyName = _T("SYSTEM_INSPECTION_FINISH_SHOW_RESULT_LIST");
		break;
	case SYSTEM_MODEL_DEFAULT_WND_LEVEL://模組預設檢測框等級
		KeyName = _T("SYSTEM_MODEL_DEFAULT_WND_LEVEL");
		break;
	case SYSTEM_SWITCH_PROJECT_3D_FRAME://可切換至專案3D畫面
		KeyName = _T("SYSTEM_SWITCH_PROJECT_3D_FRAME");
		break;	
	case SYSTEM_AUTO_SWITCH_WND_3D_FRAME_MODE://自動切換至檢測框3D畫面模式
		KeyName = _T("SYSTEM_AUTO_SWITCH_WND_3D_FRAME_MODE");
		break;
	case SYSTEM_AUTO_SWITCH_WND_3D_FRAME_SIZE_LIMIT://自動切換至檢測框3D畫面尺寸上限-um	
		KeyName = _T("SYSTEM_AUTO_SWITCH_WND_3D_FRAME_SIZE_LIMIT");
		break;
	case SYSTEM_SHOW_COMPONENT_FULL_MAP://顯示零件-整板零件
		KeyName = _T("SYSTEM_SHOW_COMPONENT_FULL_MAP");
		break;
	case SYSTEM_SHOW_COMPONENT_DEFECT_ONLY://僅顯示瑕疵零件(線上畫面)	
		KeyName = _T("SYSTEM_SHOW_COMPONENT_DEFECT_ONLY");
		break;
	case SYSTEM_SHOW_DEBUG_FORM_VIEW://顯示除錯頁面 
		KeyName = _T("SYSTEM_SHOW_DEBUG_FORM_VIEW");
		break;
	case SYSTEM_LEVEL_FILTER_SHIFT_ENABLED://分等濾波起點偏移啟用
		KeyName = _T("SYSTEM_LEVEL_FILTER_SHIFT_ENABLED");
		break;
	case SYSTEM_MEDIAN_FILTER_SHIFT_ENABLED://中值濾波起點偏移啟用
		KeyName = _T("SYSTEM_MEDIAN_FILTER_SHIFT_ENABLED");
		break;
	case SYSTEM_SMOOTH_FILTER_SHIFT_ENABLED://平滑濾波起點偏移啟用	
		KeyName = _T("SYSTEM_SMOOTH_FILTER_SHIFT_ENABLED");
		break;
	case SYSTEM_PYRAMID_MEDIAN_FILTER_SHIFT_ENABLED://金字塔中值濾波起點偏移啟用
		KeyName = _T("SYSTEM_PYRAMID_MEDIAN_FILTER_SHIFT_ENABLED");
		break;
	case SYSTEM_LEVEL_FILTER_PITCH_TOLERANCE://分等濾波步長可接受誤差
		KeyName = _T("SYSTEM_LEVEL_FILTER_PITCH_TOLERANCE");
		break;
	case SYSTEM_MEDIAN_FILTER_PITCH_TOLERANCE://中值濾波步長可接受誤差
		KeyName = _T("SYSTEM_MEDIAN_FILTER_PITCH_TOLERANCE");
		break;
	case SYSTEM_SMOOTH_FILTER_PITCH_TOLERANCE://平滑濾波步長可接受誤差
		KeyName = _T("SYSTEM_SMOOTH_FILTER_PITCH_TOLERANCE");
		break;
	case SYSTEM_PYRAMID_MEDIAN_FILTER_PITCH_TOLERANCE://金字塔中值濾波步長可接受誤差
		KeyName = _T("SYSTEM_PYRAMID_MEDIAN_FILTER_PITCH_TOLERANCE");
		break;
	case SYSTEM_GRR_OFFSET_RANDOM_VALUE://Grr偏移隨機補償值
		KeyName = _T("SYSTEM_GRR_OFFSET_RANDOM_VALUE");
		break;
	case SYSTEM_GRR_AVERAGE_RESET_ENABLED://Grr平均值復歸-啟用-依據條碼比較
		KeyName = _T("SYSTEM_GRR_AVERAGE_RESET_ENABLED");
		break;
	case SYSTEM_GRR_AVERAGE_RESET_MATCH_RATIO://Grr平均值復歸-匹配比例
		KeyName = _T("SYSTEM_GRR_AVERAGE_RESET_MATCH_RATIO");
		break;
	case SYSTEM_GRR_SKEW_AVERAGE_ENB_VAL://Grr偏移角度平均啟用角度
		KeyName = _T("SYSTEM_GRR_SKEW_AVERAGE_ENB_VAL");
		break;
	case SYSTEM_GRR_SKEW_AVERAGE_START_GAP://Grr偏移角度平均開始差距-角度	
		KeyName = _T("SYSTEM_GRR_SKEW_AVERAGE_START_GAP");
		break;
	case SYSTEM_GRR_SKEW_AVERAGE_WEIGHTING://Grr偏移角度平均權重 
		KeyName = _T("SYSTEM_GRR_SKEW_AVERAGE_WEIGHTING");
		break;
	case SYSTEM_GRR_OFFSET_AVERAGE_ENB_PXL://Grr偏移平均啟用像素
		KeyName = _T("SYSTEM_GRR_OFFSET_AVERAGE_ENB_PXL");
		break;
	case SYSTEM_GRR_OFFSET_AVERAGE_START_GAP://Grr偏移平均開始差距-微米	
		KeyName = _T("SYSTEM_GRR_OFFSET_AVERAGE_START_GAP");
		break;
	case SYSTEM_GRR_OFFSET_AVERAGE_WEIGHTING://Grr偏移平均權重
		KeyName = _T("SYSTEM_GRR_OFFSET_AVERAGE_WEIGHTING");
		break;
	case SYSTEM_GRR_HEIGHT_AVERAGE_ENB_VAL://Grr偏移高度平均啟用高度
		KeyName = _T("SYSTEM_GRR_HEIGHT_AVERAGE_ENB_VAL");
		break;
	case SYSTEM_GRR_HEIGHT_AVERAGE_START_GAP://Grr偏移高度平均開始差距-微米
		KeyName = _T("SYSTEM_GRR_HEIGHT_AVERAGE_START_GAP");
		break;
	case SYSTEM_GRR_HEIGHT_AVERAGE_WEIGHTING://Grr偏移高度平均權重 
		KeyName = _T("SYSTEM_GRR_HEIGHT_AVERAGE_WEIGHTING");
		break;
	case SYSTEM_LOCK_SCREEN_ENABLED://啟用鎖住螢幕
		KeyName = _T("SYSTEM_LOCK_SCREEN_ENABLED");
		break;
	case SYSTEM_LOCK_SCREEN_KEY_ID://鎖住螢幕鍵號
		KeyName = _T("SYSTEM_LOCK_SCREEN_KEY_ID");
		break;
	case SYSTEM_MULTI_DISTRICT_MODE_ENABLED://多段檢測啟用
		KeyName = _T("SYSTEM_MULTI_DISTRICT_MODE_ENABLED");
		break;
	case SYSTEM_ONLINE_TUNING_ENABLE_BARCODE://在線調機-軟體條碼
		KeyName = _T("SYSTEM_ONLINE_TUNING_ENABLE_BARCODE");
		break;
	case SYSTEM_SHOW_PLC_SAFTY_SETTING_UI://是否顯示PLC安全檢知設定介面
		KeyName = _T("SYSTEM_SHOW_PLC_SAFTY_SETTING_UI");
		break;
	case SYSTEM_SHOW_ALG_OFFSET_L_PARAM://是否顯示演算法OffsetL的參數 	
		KeyName = _T("SYSTEM_SHOW_ALG_OFFSET_L_PARAM");
		break;
	case SYSTEM_SHOW_ALG_OFFSET_A_PARAM://是否顯示演算法OffsetA的參數
		KeyName = _T("SYSTEM_SHOW_ALG_OFFSET_A_PARAM");
		break;
	case SYSTEM_SHOW_ALG_BRIGHT_RATIO_SCALE_PARAM://是否顯示演算法亮度比例的比例參數
		KeyName = _T("SYSTEM_SHOW_ALG_BRIGHT_RATIO_SCALE_PARAM");
		break;
	case SYSTEM_SHOW_MODEL_PROPERTY_PARAM://是否顯示模組屬性參數
		KeyName = _T("SYSTEM_SHOW_MODEL_PROPERTY_PARAM");
		break;
	case SYSTEM_CONTINUE_PASTE_MODE://連續貼上模式
		KeyName = _T("SYSTEM_CONTINUE_PASTE_MODE");
		break;
	case SYSTEM_STITCH_IMAGE_PADDING_SIZE://拼圖參數-填補尺寸
		KeyName = _T("SYSTEM_STITCH_IMAGE_PADDING_SIZE");
		break;
	case SYSTEM_RESIN_HEIGHT_ALIGN_ENABLED://啟用Resin高度對齊-軍達3D對位
		KeyName = _T("SYSTEM_RESIN_HEIGHT_ALIGN_ENABLED");
		break;
	case SYSTEM_MODEL_IMAGE_CAD_OFFSET_ENABLED://啟用模組影像Cad偏移補償um
		KeyName = _T("SYSTEM_MODEL_IMAGE_CAD_OFFSET_ENABLED");
		break;
	case SYSTEM_MODEL_DEFAULT_TRANSISTOR_TYPE://模組預設SOT樣式
		KeyName = _T("SYSTEM_MODEL_DEFAULT_TRANSISTOR_TYPE");
		break;
	case SYSTEM_PROJECT_LOCAL_FOLDER_ENABLED://專案使用本機資料夾
		KeyName = _T("SYSTEM_PROJECT_LOCAL_FOLDER_ENABLED");
		break;
	case SYSTEM_PARTIAL_COPY_PROJECT_LIBRARY://部分複製專案資料庫	
		KeyName = _T("SYSTEM_PARTIAL_COPY_PROJECT_LIBRARY");
		break;
	case SYSTEM_AUTO_ARRANGE_MODEL_BK_IMAGE_FILES://自動重整模組底圖檔案
		KeyName = _T("SYSTEM_AUTO_ARRANGE_MODEL_BK_IMAGE_FILES");
		break;
	case SYSTEM_AUTO_COPY_SPC_COMPONENT_IMAGE_FILES://自動複製SPC零件圖檔
		KeyName = _T("SYSTEM_AUTO_COPY_SPC_COMPONENT_IMAGE_FILES");
		break;
	case SYSTEM_AUTO_BYPASS_GRAB_3D_FRAME://自動跳過3D影像	
		KeyName = _T("SYSTEM_AUTO_BYPASS_GRAB_3D_FRAME");
		break;
	case SYSTEM_CHECK_PROJECT_FD_READY://確認專案定位點狀態
		KeyName = _T("SYSTEM_CHECK_PROJECT_FD_READY");
		break;
	case SYSTEM_LOCK_MODEL_BODY_POSITION://鎖住模組本體的位置
		KeyName = _T("SYSTEM_LOCK_MODEL_BODY_POSITION");
		break;	
	case SYSTEM_MAX_UNCHECK_TEST_FILE_COUNT://最多未判定檢測檔案數 
		KeyName = _T("SYSTEM_MAX_UNCHECK_TEST_FILE_COUNT");
		break;
	case SYSTEM_CONFIRM_COMPONENT_BARCODE_ENABLED://啟用零件條碼確認	
		KeyName = _T("SYSTEM_CONFIRM_COMPONENT_BARCODE_ENABLED");
		break;
	case SYSTEM_SAVE_JPEG_QUALITY://儲存JPEG的質量(001~100)
		KeyName = _T("SYSTEM_SAVE_JPEG_QUALITY");
		break;
	case SYSTEM_COPY_HUGE_FILES_MODE://複製大量檔案模式
		KeyName = _T("SYSTEM_COPY_HUGE_FILES_MODE");
		break;	

	case SYSTEM_OFFLINE_VERSION_MODE://離線版本模式
		KeyName = _T("SYSTEM_OFFLINE_VERSION_MODE");
		break;
	case SYSTEM_USE_PROJECT_SYSTEM_PARAM_MODE://使用專案系統參數模式
		KeyName = _T("SYSTEM_USE_PROJECT_SYSTEM_PARAM_MODE");
		break;

	case SYSTEM_UI_WND_FONT_ADD_SIZE://UI視窗字型增加大小
		KeyName = _T("SYSTEM_UI_WND_FONT_ADD_SIZE");
		break;
	case SYSTEM_UI_DOCK_WND_SLIDE_STEPS://UI駐停視窗滑動步長
		KeyName = _T("SYSTEM_UI_DOCK_WND_SLIDE_STEPS");
		break;		
	case SYSTEM_UI_ENABLE_PCB_OUT_BUTTON://UI啟用PCB出板按鈕
		KeyName = _T("SYSTEM_UI_ENABLE_PCB_OUT_BUTTON");
		break;

	case SYSTEM_RABBITMQ_SERVER_PORT://RabbitMQ伺服器Port
		KeyName = _T("SYSTEM_RABBITMQ_SERVER_PORT");
		break;
	case SYSTEM_RABBITMQ_IP_ADDRESS://RabbitMQ通訊網址
		KeyName = _T("SYSTEM_RABBITMQ_IP_ADDRESS");
		break;
	case SYSTEM_RABBITMQ_USER_NAME://RabbitMQ登錄名稱
		KeyName = _T("SYSTEM_RABBITMQ_USER_NAME");
		break;
	case SYSTEM_RABBITMQ_PASSWORD://RabbitMQ登錄密碼
		KeyName = _T("SYSTEM_RABBITMQ_PASSWORD");
		break;

	case SYSTEM_ITS_FILENAME://ITS軟體名稱
		KeyName = _T("SYSTEM_ITS_FILENAME");
		break;
	case SYSTEM_ITS_COMMUNICATION_MODE://ITS通訊模式
		KeyName = _T("SYSTEM_ITS_COMMUNICATION_MODE");
		break;
	case SYSTEM_ITS_SOCKET_IP_PORT://ITS網路Port
		KeyName = _T("SYSTEM_ITS_SOCKET_IP_PORT");
		break;
	case SYSTEM_ITS_SOCKET_IP_ADDRESS://ITS網路網址
		KeyName = _T("SYSTEM_ITS_SOCKET_IP_ADDRESS");
		break;
	case SYSTEM_ITS_RABBITMQ_QUEUE_NAME_RECV://ITS訊息佇列接收名
		KeyName = _T("SYSTEM_ITS_RABBITMQ_QUEUE_NAME_RECV");
		break;
	case SYSTEM_ITS_RABBITMQ_QUEUE_NAME_SEND://ITS訊息佇列傳送名
		KeyName = _T("SYSTEM_ITS_RABBITMQ_QUEUE_NAME_SEND");
		break;
	case SYSTEM_ITS_CONTACT_SOFTWARE://ITS-對接軟體
		KeyName = _T("SYSTEM_ITS_CONTACT_SOFTWARE");
		break;
	case SYSTEM_ITS_COMMUNICATION_ENABLED://連線至ITS啟用
		KeyName = _T("SYSTEM_ITS_COMMUNICATION_ENABLED");
		break;
	case SYSTEM_ITS_COMMUNICATION_TIMEOUT_MS://與ITS溝通逾時(ms)	
		KeyName = _T("SYSTEM_ITS_COMMUNICATION_TIMEOUT_MS");
		break;	
	case SYSTEM_ITS_FILE_FOLDER_SEND://ITS檔案資料夾-傳送
		KeyName = _T("SYSTEM_ITS_FILE_FOLDER_SEND");
		break;
	case SYSTEM_ITS_FILE_FOLDER_RECV://ITS檔案資料夾-接收
		KeyName = _T("SYSTEM_ITS_FILE_FOLDER_RECV");
		break;
	case SYSTEM_ITS_FILE_BACKUP_ENABLED://ITS檔案資料夾-備份	
		KeyName = _T("SYSTEM_ITS_FILE_BACKUP_ENABLED");
		break;
	case SYSTEM_ITS_FILE_USE_SYNC_FILE_ENABLED://ITS檔案資料夾-同步檔案
		KeyName = _T("SYSTEM_ITS_FILE_USE_SYNC_FILE_ENABLED");
		break;	
	case SYSTEM_ITS_SET_SECS_GEM_ENABLED://ITS使用設定SECS/GEM
		KeyName = _T("SYSTEM_ITS_SET_SECS_GEM_ENABLED");
		break;
	case SYSTEM_ITS_SECS_GEM_REMOTE_LOCAL: //ITS使用SECS/GEM-Remote Local
		KeyName = _T("SYSTEM_ITS_SECS_GEM_REMOTE_LOCAL");
		break;
	case SYSTEM_ITS_SET_SYSTEM_PARAM_ENABLED://ITS使用設定系統參數
		KeyName = _T("SYSTEM_ITS_SET_SYSTEM_PARAM_ENABLED");
		break;
	case SYSTEM_ITS_SET_PROJECT_PARAM_ENABLED://ITS使用設定專案參數
		KeyName = _T("SYSTEM_ITS_SET_PROJECT_PARAM_ENABLED");
		break;
	case SYSTEM_ITS_SET_MACHINE_STATUS_ENABLED://ITS使用設定機台狀態
		KeyName = _T("SYSTEM_ITS_SET_MACHINE_STATUS_ENABLED");
		break;
	case SYSTEM_ITS_SET_APP_OPEN_CLOSE_ENABLED://ITS使用設定軟體開關
		KeyName = _T("SYSTEM_ITS_SET_APP_OPEN_CLOSE_ENABLED");
		break;
	case SYSTEM_ITS_SET_PROCESS_ID_ENABLED://ITS使用設定程序編號
		KeyName = _T("SYSTEM_ITS_SET_PROCESS_ID_ENABLED");
		break;
	case SYSTEM_ITS_SET_USER_LOGIN_OUT_ENABLED://ITS使用設定使用者登入登出
		KeyName = _T("SYSTEM_ITS_SET_USER_LOGIN_OUT_ENABLED");
		break;	
	case SYSTEM_PROG_DEFAULT_SPACE_TO_GRAY_RATIO_MODE://專案預設高度轉灰階比例 
		KeyName = _T("SYSTEM_PROG_DEFAULT_SPACE_TO_GRAY_RATIO_MODE");
		break;
	case SYSTEM_PROG_DEFAULT_SPACE_BASE_PLANE_INDEX://專案預設空間基準面編號 
		KeyName = _T("SYSTEM_PROG_DEFAULT_SPACE_BASE_PLANE_INDEX");
		break;
	case SYSTEM_PROG_DEFAULT_SPACE_NOISE_FILTER_INDEX://專案預設空間雜訊過濾編號
		KeyName = _T("SYSTEM_PROG_DEFAULT_SPACE_NOISE_FILTER_INDEX");
		break;
	case SYSTEM_PROG_DEFAULT_ENABLE_CONVEYER_PRE_RUN://專案預設軌道提前運轉功能
		KeyName = _T("SYSTEM_PROG_DEFAULT_ENABLE_CONVEYER_PRE_RUN");
		break;
	case SYSTEM_PROG_DEFAULT_FD_NG_HANDLE_MODE://專案預設定位點異常處理模式
		KeyName = _T("SYSTEM_PROG_DEFAULT_FD_NG_HANDLE_MODE");
		break;
	case SYSTEM_PROG_DEFAULT_BOARD_FD_GRAB_MODE://專案預設單板定位點取像模式
		KeyName = _T("SYSTEM_PROG_DEFAULT_BOARD_FD_GRAB_MODE");
		break;
	case SYSTEM_PROG_DEFAULT_BDEFECT_HANDLE_MODE://專案預設檢出異常處理模式
		KeyName = _T("SYSTEM_PROG_DEFAULT_BDEFECT_HANDLE_MODE");
		break;
	case SYSTEM_PROG_DEFAULT_PCB_OUT_MODE://專案預設PCB出板模式
		KeyName = _T("SYSTEM_PROG_DEFAULT_PCB_OUT_MODE");
		break;
	case SYSTEM_PROG_DEFAULT_PROJECT_SAVE_TEST_MAP://專案預設儲存檢測底圖模式
		KeyName = _T("SYSTEM_PROG_DEFAULT_PROJECT_SAVE_TEST_MAP");
		break;
	case SYSTEM_PROG_DEFAULT_PROJECT_LINK_SERVER_MODE://專案預設連線伺服器模式	
		KeyName = _T("SYSTEM_PROG_DEFAULT_PROJECT_LINK_SERVER_MODE");
		break;
	case SYSTEM_PROG_DEFAULT_SAVE_OFFLINE_IMAGE_FILES://專案預設儲存離線圖檔
		KeyName = _T("SYSTEM_PROG_DEFAULT_SAVE_OFFLINE_IMAGE_FILES");
		break;
	case SYSTEM_PROG_DEFAULT_BARCODE_VERIFY_MODE://專案預設條碼驗證模式
		KeyName = _T("SYSTEM_PROG_DEFAULT_BARCODE_VERIFY_MODE");
		break;
	case SYSTEM_PROG_DEFAULT_BARCODE_RETRIEVE_MODE://專案預設條碼查詢模式
		KeyName = _T("SYSTEM_PROG_DEFAULT_BARCODE_RETRIEVE_MODE");
		break;

	case SYSTEM_OPERATE_LEVEL_PROJECT_OPEN://操作等級-專案開啟
		KeyName = _T("SYSTEM_OPERATE_LEVEL_PROJECT_OPEN");
		break;
	case SYSTEM_OPERATE_LEVEL_PROJECT_SAVE://操作等級-專案儲存
		KeyName = _T("SYSTEM_OPERATE_LEVEL_PROJECT_SAVE");
		break;
	case SYSTEM_OPERATE_LEVEL_PROJECT_PARAM://操作等級-專案參數
		KeyName = _T("SYSTEM_OPERATE_LEVEL_PROJECT_PARAM");
		break;
	case SYSTEM_OPERATE_LEVEL_ONLINE_RUN://操作等級-線上運行
		KeyName = _T("SYSTEM_OPERATE_LEVEL_ONLINE_RUN");
		break;
	case SYSTEM_OPERATE_LEVEL_ONLINE_BYPASS://操作等級-線上直通
		KeyName = _T("SYSTEM_OPERATE_LEVEL_ONLINE_BYPASS");
		break;
	case SYSTEM_OPERATE_LEVEL_ONLINE_STOP://操作等級-線上停止
		KeyName = _T("SYSTEM_OPERATE_LEVEL_ONLINE_STOP");
		break;
	case SYSTEM_OPERATE_LEVEL_ONLINE_SAVE_IMAGE://操作等級-線上存圖
		KeyName = _T("SYSTEM_OPERATE_LEVEL_ONLINE_SAVE_IMAGE");
		break;
	case SYSTEM_OPERATE_LEVEL_ONLINE_UNLOCK://操作等級-線上解鎖
		KeyName = _T("SYSTEM_OPERATE_LEVEL_ONLINE_UNLOCK");
		break;	
	case SYSTEM_OPERATE_LEVEL_EDIT_FUNC_ADD://操作等級-編輯功能-新增
		KeyName = _T("SYSTEM_OPERATE_LEVEL_EDIT_FUNC_ADD");
		break;
	case SYSTEM_OPERATE_LEVEL_EDIT_FUNC_DEL://操作等級-編輯功能-刪除
		KeyName = _T("SYSTEM_OPERATE_LEVEL_EDIT_FUNC_DEL");
		break;
	case SYSTEM_OPERATE_LEVEL_EDIT_FUNC_BYPASS://操作等級-編輯功能-不檢測
		KeyName = _T("SYSTEM_OPERATE_LEVEL_EDIT_FUNC_BYPASS");
		break;	
	case SYSTEM_OPERATE_LEVEL_MES_CTRL_STATE_OFFLINE://操作等級-MES相關-控制狀態-離線
		KeyName = _T("SYSTEM_OPERATE_LEVEL_MES_CTRL_STATE_OFFLINE");
		break;
	case SYSTEM_OPERATE_LEVEL_MES_CTRL_STATE_LOCAL://操作等級-MES相關-控制狀態-本地上線
		KeyName = _T("SYSTEM_OPERATE_LEVEL_MES_CTRL_STATE_LOCAL");
		break;
	case SYSTEM_OPERATE_LEVEL_MES_CTRL_STATE_REMOTE://操作等級-MES相關-控制狀態-遠端上線
		KeyName = _T("SYSTEM_OPERATE_LEVEL_MES_CTRL_STATE_REMOTE");
		break;
	case SYSTEM_OPERATE_LEVEL_MES_SHOW_CONTEXT://操作等級-MES相關-顯示內容
		KeyName = _T("SYSTEM_OPERATE_LEVEL_MES_SHOW_CONTEXT");
		break;

	case SYSTEM_M2M_NPM_BARCODE_ENABLE://啟用NPM-條碼
		KeyName = _T("SYSTEM_M2M_NPM_BARCODE_ENABLE");
		break;
	case SYSTEM_M2M_NPM_APC_FF1_ENABLE://啟用NPM APC-FF1
		KeyName = _T("SYSTEM_M2M_NPM_APC_FF1_ENABLE");
		break;
	case SYSTEM_M2M_NPM_APC_FF2_ENABLE://啟用NPM APC-FF2
		KeyName = _T("SYSTEM_M2M_NPM_APC_FF2_ENABLE");
		break;
	case SYSTEM_M2M_NPM_APC_MFB_ENABLE://啟用NPM APC-MFB
		KeyName = _T("SYSTEM_M2M_NPM_APC_MFB_ENABLE");
		break;
	case SYSTEM_M2M_NPM_LANE_NAME_LA://NPM軌道名稱-A軌
		KeyName = _T("SYSTEM_M2M_NPM_LANE_NAME_LA");
		break;
	case SYSTEM_M2M_NPM_LANE_NAME_LB://NPM軌道名稱-B軌
		KeyName = _T("SYSTEM_M2M_NPM_LANE_NAME_LB");
		break;
	case SYSTEM_M2M_NPM_INPUT_SHARE_FOLDER_LA://NPM-輸入共享資料夾-A軌
		KeyName = _T("SYSTEM_M2M_NPM_INPUT_SHARE_FOLDER_LA");
		break;
	case SYSTEM_M2M_NPM_INPUT_SHARE_FOLDER_LB://NPM-輸入共享資料夾-B軌
		KeyName = _T("SYSTEM_M2M_NPM_INPUT_SHARE_FOLDER_LB");
		break;
	case SYSTEM_M2M_NPM_OUTPUT_SHARE_FOLDER_LA://NPM-輸出共享資料夾-A軌
		KeyName = _T("SYSTEM_M2M_NPM_OUTPUT_SHARE_FOLDER_LA");
		break;
	case SYSTEM_M2M_NPM_OUTPUT_SHARE_FOLDER_LB://NPM-輸出共享資料夾-B軌
		KeyName = _T("SYSTEM_M2M_NPM_OUTPUT_SHARE_FOLDER_LB");
		break;

	case SYSTEM_M2M_HASI_ENABLE://啟用M2M HAS I (Hanwha AOI Solution MAOI_SAOI)
		KeyName = _T("SYSTEM_M2M_HASI_ENABLE");
		break;
	case SYSTEM_M2M_HASI_SHARE_FOLDER://HAS I 共享資料夾
		KeyName = _T("SYSTEM_M2M_HASI_SHARE_FOLDER");
		break;
	case SYSTEM_M2M_HASI_SERIALFILE_ENABLE://啟用 HAS I - SerialFile
		KeyName = _T("SYSTEM_M2M_HASI_SERIALFILE_ENABLE");
		break;
	case SYSTEM_M2M_HASI_SERIALFILE_QUEUESIZE://HAS I - SerialFile - Size of Serial Queue
		KeyName = _T("SYSTEM_M2M_HASI_SERIALFILE_QUEUESIZE");
		break;
	case SYSTEM_M2M_HASI_SERIALFILE_DWELLTIME:// 等待資料的延遲時間 - ms
		KeyName = _T("SYSTEM_M2M_HASI_SERIALFILE_DWELLTIME");
		break;
	case SYSTEM_M2M_HASI_SPIOFFSETFILE_ENABLE://啟用 HAS I - SPIOffsetFile
		KeyName = _T("SYSTEM_M2M_HASI_SPIOFFSETFILE_ENABLE");
		break;
	case SYSTEM_M2M_HASI_PNP_ENABLE://啟用 HAS I - PNP
		KeyName = _T("SYSTEM_M2M_HASI_PNP_ENABLE");
		break;
	case SYSTEM_M2M_HASI_AOI_STAGE://產線上 AOI 的檢查階段
		KeyName = _T("SYSTEM_M2M_HASI_AOI_STAGE");
		break;
	case SYSTEM_M2M_HASI_STATE_MODE://HASI 狀態
		KeyName = _T("SYSTEM_M2M_HASI_STATE_MODE");
		break;

	case SYSTEM_AI_MODEL_SERVER_ENABLE://AI模型伺服器啟用
		KeyName = _T("SYSTEM_AI_MODEL_SERVER_ENABLE");
		break;
	case SYSTEM_AI_MODEL_SERVER_TIMEOUT://AI模型伺服器逾時
		KeyName = _T("SYSTEM_AI_MODEL_SERVER_TIMEOUT");
		break;
	case SYSTEM_AI_MODEL_SERVER_FILENAME://AI模型伺服器檔名
		KeyName = _T("SYSTEM_AI_MODEL_SERVER_FILENAME");
		break;
	case SYSTEM_AI_MODEL_FILE_FOLDER_SEND://AI模型檔案資料夾-傳送
		KeyName = _T("SYSTEM_AI_MODEL_FILE_FOLDER_SEND");
		break;
	case SYSTEM_AI_MODEL_FILE_FOLDER_RECV://AI模型檔案資料夾-接收
		KeyName = _T("SYSTEM_AI_MODEL_FILE_FOLDER_RECV");
		break;
	case SYSTEM_AI_MODEL_LABEL_MIN_CLUSTER_DISTANCE://AI模型分類最小叢集距離-um
		KeyName = _T("SYSTEM_AI_MODEL_LABEL_MIN_CLUSTER_DISTANCE");
		break;

	case SYSTEM_EXTERNAL_COPY_FILE_ENABLED://外部複製檔案啟用
		KeyName = _T("SYSTEM_EXTERNAL_COPY_FILE_ENABLED");
		break;
	case SYSTEM_EXTERNAL_COPY_FILE_APP_NAME://外部複製檔案軟體名稱
		KeyName = _T("SYSTEM_EXTERNAL_COPY_FILE_APP_NAME");
		break;
	case SYSTEM_EXTERNAL_COPY_FILE_SEND_FOLDER://外部複製檔案輸出資料夾
		KeyName = _T("SYSTEM_EXTERNAL_COPY_FILE_SEND_FOLDER");
		break;

	case SYSTEM_CPK_CHART_ENABLED://Cpk圖表啟用
		KeyName = _T("SYSTEM_CPK_CHART_ENABLED");
		break;
	case SYSTEM_WND_ROTATION_FOLLOWED://檢測框跟隨旋轉
		KeyName = _T("SYSTEM_WND_ROTATION_FOLLOWED");
		break;

	default:		
		KeyName = _T("UNDEFINE_PARAM");
		break;
	}
	KeyName = KeyName + _T("_DESCRIPTION");
	AOIDataCollect.GetUILanguageString(_T("SYSTEM_PARAMETER"), KeyName, Default, String);	
	return String;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ObtainSystemParameterKeyName(SYSTEM_PARAM_ID ParamID, CString &Section, CString &KeyName)//取得系統參數的KeyName
{	
	bool IsOK = true;
	CString Err;
	switch ( ParamID )
	{
	case SYSTEM_AOI_FOLDER_HOST://系統資料夾
		Section = _T("System Parameter");
		KeyName = _T("AOI Directory");
		break;
	case SYSTEM_AOI_FOLDER_TEMP://暫存的資料夾
		Section = _T("System Parameter");
	#ifndef OFFLINE_VERSION
		KeyName = _T("AOI Temp Directory");
	#else
		KeyName = _T("AOI Temp Directory Offline");
	#endif//OFFLINE_VERSION
		break;
	case SYSTEM_AOI_FOLDER_LOG://系統訊息資料夾
		Section = _T("System Parameter");
	#ifndef OFFLINE_VERSION
		KeyName = _T("AOI Log Directory");
	#else
		KeyName = _T("AOI Log Directory Offline");
	#endif//OFFLINE_VERSION
		break;
	case SYSTEM_AOI_FOLDER_PROJECT://專案資料夾	
		Section = _T("System Parameter");
	#ifndef OFFLINE_VERSION
		KeyName = _T("AOI Project Directory");
	#else
		KeyName = _T("AOI Project Directory Offline");
	#endif//OFFLINE_VERSION
		break;
	case SYSTEM_AOI_FOLDER_RESULT://結果資料夾
		Section = _T("System Parameter");
	#ifndef OFFLINE_VERSION
		KeyName = _T("AOI Result Directory");
	#else
		KeyName = _T("AOI Result Directory Offline");
	#endif//OFFLINE_VERSION
		break;
	case SYSTEM_AOI_FOLDER_SERVER://伺服器資料夾
		Section = _T("System Parameter");
		KeyName = _T("AOI Server Directory");
		break;
	case SYSTEM_AOI_FOLDER_STATIC_DATA:
		Section = _T("System Parameter");
		KeyName = _T("AOI Static Data Directory");
		break;
	case SYSTEM_AOI_FOLDER_TEXT_REPORT://文字報告資料夾
		Section = _T("System Parameter");
		KeyName = _T("AOI Text Report Directory");
		break;
	case SYSTEM_AOI_FOLDER_ONLINE_TUNING://線上調機資料夾
		Section = _T("System Parameter");
	#ifndef OFFLINE_VERSION
		KeyName = _T("AOI Online Tuning Directory");
	#else
		KeyName = _T("AOI Online Tuning Directory Offline");
	#endif//OFFLINE_VERSION
		break;
	case SYSTEM_AOI_FOLDER_ONLINE_BARCODE://線上條碼資料夾
		Section = _T("System Parameter");
		KeyName = _T("AOI Online Barcode Directory");
		break;
	case SYSTEM_AOI_FOLDER_ONLINE_OFFLINE://線上離線編程資料夾	
		Section = _T("System Parameter");
	#ifndef OFFLINE_VERSION
		KeyName = _T("AOI Online Offline Directory");
	#else
		KeyName = _T("AOI Online Offline Directory Offline");
	#endif//OFFLINE_VERSION
		break;
	case SYSTEM_AOI_FOLDER_PROJECT_TEST_MAP://專案檢測底圖資料夾
		Section = _T("System Parameter");
		KeyName = _T("AOI Project Test Map Directory");
		break;
	case SYSTEM_AOI_FOLDER_PROJECT_TEST_MAP_TEMP://專案檢測底圖資料夾-暫存
		Section = _T("System Parameter");
		KeyName = _T("AOI Project Test Map Temp Directory");
		break;
	case SYSTEM_AOI_FOLDER_PROJECT_TEST_MAP_BACKUP://專案檢測底圖資料夾-備份
		Section = _T("System Parameter");
		KeyName = _T("AOI Project Test Map Backup Directory");
		break;
	case SYSTEM_AOI_FOLDER_PROJECT_RAW://專案原圖資料夾
		Section = _T("System Parameter");
		KeyName = _T("AOI Project Raw Directory");
		break;
	case SYSTEM_AOI_FOLDER_PROJECT_DEBUG://專案除錯資料夾
		Section = _T("System Parameter");
		KeyName = _T("AOI Project Debug Directory");
		break;
	case SYSTEM_AOI_FOLDER_MONITOR_STATUS://監控狀態資料夾
		Section = _T("System Parameter");
		KeyName = _T("AOI Monitor Status Directory");
		break;
	case SYSTEM_AOI_FOLDER_CUSTOMER_LOG://客戶訊息資料夾
		Section = _T("System Parameter");
		KeyName = _T("AOI Customer Log Directory");
		break;
	case SYSTEM_AOI_FOLDER_CUSTOMER_REPORT://客戶報告資料夾			
		Section = _T("System Parameter");
		KeyName = _T("AOI Customer Report Directory");
		break;
	case SYSTEM_AOI_FOLDER_BARCODE_FILE_LA://條碼檔案資料夾-A軌
		Section = _T("System Parameter");
		KeyName = _T("AOI Barcode File Directory LA");
		break;
	case SYSTEM_AOI_FOLDER_BARCODE_FILE_LB://條碼檔案資料夾-B軌	
		Section = _T("System Parameter");
		KeyName = _T("AOI Barcode File Directory LB");
		break;
	case SYSTEM_AOI_FOLDER_AI_FILE_EXPORT://AI檔案輸出資料夾
		Section = _T("System Parameter");
		KeyName = _T("AOI AI File Directory");
		break;
	case SYSTEM_AOI_FOLDER_AI_IMAGE_EXPORT://AI影像輸出資料夾
		Section = _T("System Parameter");
		KeyName = _T("AOI AI Image Directory");
		break;
	case SYSTEM_AOI_FOLDER_TEST_TRACK://檢測追蹤資料夾
		Section = _T("System Parameter");
		KeyName = _T("AOI Test Track Directory");
		break;
	case SYSTEM_AOI_LIBRARY_HOST://資料庫-本機
		Section = _T("System Parameter");
		KeyName = _T("AOI Host Library");
		break;
	case SYSTEM_AOI_LIBRARY_REMOTE://資料庫-遠端
		Section = _T("System Parameter");
		KeyName = _T("AOI Remote Library");
		break;	
	case SYSTEM_IP_HOST_COMPUTER://本機網路網址
		Section = _T("System Parameter");
		KeyName = _T("AOI Host Computer IP");
		break;
	case SYSTEM_MACHINE_LOCATION://本機廠區
		Section = _T("System Parameter");
		KeyName = _T("Machine Location");
		break;
	case SYSTEM_MACHINE_BUILDING://本機棟別
		Section = _T("System Parameter");
		KeyName = _T("Machine Building");
		break;
	case SYSTEM_MACHINE_FLOOR://本機樓層
		Section = _T("System Parameter");
		KeyName = _T("Machine Floor");
		break;
	case SYSTEM_MACHINE_ROOM://本機車間
		Section = _T("System Parameter");
		KeyName = _T("Machine Room Name");
		break;
	case SYSTEM_MACHINE_LINE://本機線別
		Section = _T("System Parameter");
		KeyName = _T("Machine Line Name");
		break;
	case SYSTEM_MACHINE_LINE_LB://本機線別-B軌
		Section = _T("System Parameter");
		KeyName = _T("Machine Line Name LB");
		break;
	case SYSTEM_MACHINE_STATION://本機站別
		Section = _T("System Parameter");
		KeyName = _T("Machine Station Name");
		break;	
	case SYSTEM_MACHINE_STATION_LB://本機站別-B軌
		Section = _T("System Parameter");
		KeyName = _T("Machine Station Name LB");
		break;
	case SYSTEM_MACHINE_SN://本機序號
		Section = _T("System Parameter");
		KeyName = _T("Machine Series Number");
		break;
	case SYSTEM_MACHINE_NAME://機台型號
		Section = _T("System Parameter");
		KeyName = _T("Machine Name");
		break;
	case SYSTEM_MACHINE_VENDOR://機台廠商
		Section = _T("System Parameter");
		KeyName = _T("Machine Vendor");
		break;
	case SYSTEM_MACHINE_ALIAS://機台別名
		Section = _T("System Parameter");
		KeyName = _T("Machine Alias");
		break;
	case SYSTEM_MACHINE_MES_NAME://MES登入名稱
		Section = _T("System Parameter");
		KeyName = _T("Machine MES Name");
		break;
	case SYSTEM_MACHINE_MES_PASSWORD://MES登入密碼
		Section = _T("System Parameter");
		KeyName = _T("Machine MES Password");
		break;
	case SYSTEM_MACHINE_MES_DEVICE://MES登入裝置
		Section = _T("System Parameter");
		KeyName = _T("Machine MES Device");
		break;
	case SYSTEM_MACHINE_MES_DEVICE_2://MES登入裝置-2
		Section = _T("System Parameter");
		KeyName = _T("Machine MES Device 2");
		break;
	case SYSTEM_MACHINE_MES_CODE_NAME://MES-裝置代號
		Section = _T("System Parameter");
		KeyName = _T("Machine MES Code Name");
		break;

	case SYSTEM_AOI_CUSTOMER_ID://客戶編號
		Section = _T("System Parameter");
		KeyName = _T("AOI Customer ID");
		break;
	case SYSTEM_CPU_MAX_COUNT_USED://Cpu最多使用數量
		Section = _T("System Parameter");
		KeyName = _T("CPU Max Count Used");
		break;
	case SYSTEM_THREAD_CNT_SLICE_FILL://多執行緒數量-畫面填圖	
		Section = _T("System Parameter");
		KeyName = _T("Multi Thread Count Silice Fill");
		break;
	case SYSTEM_THREAD_CNT_FRAME_MERGE://多執行緒數量-影像合併	
		Section = _T("System Parameter");
		KeyName = _T("Multi Thread Count Frame Merge");
		break;
	case SYSTEM_THREAD_CNT_FIELD_MERGE://多執行緒數量-區域合併
		Section = _T("System Parameter");
		KeyName = _T("Multi Thread Count Field Merge");
		break;
	case SYSTEM_THREAD_CNT_FRAME_LOAD://多執行緒數量-影像載入	
		Section = _T("System Parameter");
		KeyName = _T("Multi Thread Count Frame Load");
		break;
	case SYSTEM_THREAD_CNT_REGION_CALC://多執行緒數量-區域計算
		Section = _T("System Parameter");
	#ifndef OFFLINE_VERSION
		KeyName = _T("Multi Thread Count Region Calc");
	#else
		KeyName = _T("Multi Thread Count Region Calc Offline");
	#endif//OFFLINE_VERSION
		break;
	case SYSTEM_THREAD_CNT_PROC_IDLE://多執行緒數量-閒置核心	
		Section = _T("System Parameter");	
	#ifndef OFFLINE_VERSION
		KeyName = _T("Multi Thread Process Idle");	
	#else
		KeyName = _T("Multi Thread Process Idle Offline");	
	#endif//OFFLINE_VERSION
		break;
	case SYSTEM_THREAD_CNT_GRAB_IDLE://多執行緒數量-取像閒置核心	
		Section = _T("System Parameter");	
	#ifndef OFFLINE_VERSION
		KeyName = _T("Multi Thread Grabbing Idle");	
	#else
		KeyName = _T("Multi Thread Grabbing Idle Offline");	
	#endif//OFFLINE_VERSION
		break;
	case SYSTEM_OPEN_MP_CNT_GENERAL://OpenMP核心數量-一般操作
		Section = _T("System Parameter");
	#ifndef OFFLINE_VERSION
		KeyName = _T("Open MP Count General");
	#else
		KeyName = _T("Open MP Count General Offline");
	#endif//OFFLINE_VERSION
		break;
	case SYSTEM_OPEN_MP_CNT_INSPECTION://OpenMP核心數量-檢測中	
		Section = _T("System Parameter");
	#ifndef OFFLINE_VERSION
		KeyName = _T("Open MP Count Inspection");
	#else
		KeyName = _T("Open MP Count Inspection Offline");
	#endif//OFFLINE_VERSION
		break;
	case SYSTEM_OPEN_MP_CHK_SIZE_INSPECTION://OpenMP確認尺寸-檢測中
		Section = _T("System Parameter");	
		KeyName = _T("Open MP Check Size Inspection");
		break;
	case SYSTEM_SAVE_MSG_MOVING_TIME://是否移動時間狀態訊息
		Section = _T("System Parameter");
		KeyName = _T("Save Moving Time Message");
		break;
	case SYSTEM_BACKUP_MSG_MOVING_TIME://是否備份移動時間狀態訊息	
		Section = _T("System Parameter");
		KeyName = _T("Backup Moving Time Message");
		break;
	case SYSTEM_SAVE_MSG_CURRENT_PROCESS://是否儲存現在狀態訊息
		Section = _T("System Parameter");
		KeyName = _T("Save Current Process Message");
		break;	
	case SYSTEM_BACKUP_MSG_CURRENT_PROCESS://是否備份現在狀態訊息
		Section = _T("System Parameter");
		KeyName = _T("Backup Current Process Message");
		break;
	case SYSTEM_SAVE_MSG_MACHINE_MONITOR://是否儲存機台監控訊息
		Section = _T("System Parameter");
		KeyName = _T("Save Machine Monitor Message");
		break;
	case SYSTEM_SAVE_MSG_MACHINE_MONITOR_WEB://是否儲存機台監控訊息-網頁用
		Section = _T("System Parameter");
		KeyName = _T("Save Machine Monitor Message for WEB");
		break;
	case SYSTEM_SAVE_CAMERA_ADD_RING_BUFFER_LOG://是否儲存相機增加循環資料訊息
		Section = _T("System Parameter");
		KeyName = _T("Save Camera Add Ring Buffer Log");
		break;
	case SYSTEM_SHOW_MEMORY_LEAK_MESSAGE://顯示記憶體未釋放訊息
		Section = _T("System Parameter");
		KeyName = _T("Show Memory Leak Message");
		break;
	case SYSTEM_SEND_DEBUG_VIEW_STRING://是否送至DebugView視窗
		Section = _T("System Parameter");
		KeyName = _T("Send Debug View String");
		break;
	case SYSTEM_SAVE_SLICE_FILL_THREAD_LOG://是否儲存影像填滿執行緒訊息
		Section = _T("System Parameter");
		KeyName = _T("Save Slice Fill Thread Log");
		break;
	case SYSTEM_SAVE_FRAME_MERGE_THREAD_LOG://是否儲存影像合併執行緒訊息
		Section = _T("System Parameter");
		KeyName = _T("Save Frame Merge Thread Log");
		break;
	case SYSTEM_SAVE_FIELD_MERGE_THREAD_LOG://是否儲存區域合併執行緒訊息
		Section = _T("System Parameter");
		KeyName = _T("Save Field Merge Thread Log");
		break;
	case SYSTEM_SAVE_FRAME_LOAD_THREAD_LOG://是否儲存影像載入執行緒訊息
		Section = _T("System Parameter");
		KeyName = _T("Save Frame Load Thread Log");
		break;
	case SYSTEM_SAVE_REGION_CALC_THREAD_LOG://是否儲存區域計算執行緒訊息
		Section = _T("System Parameter");
		KeyName = _T("Save Region Calc Thread Log");
		break;
	case SYSTEM_SAVE_SQUENCE_THREAD_LOG://是否儲存系列執行緒訊息
		Section = _T("System Parameter");
		KeyName = _T("Save Squence Thread Log");
		break;
	case SYSTEM_SAVE_FIELD_FRAME_RELEASE_THREAD_LOG://是否儲存視野影像釋放執行緒訊息
		Section = _T("System Parameter");
		KeyName = _T("Save Field Frame Release Thread Log");
		break;
	case SYSTEM_SAVE_ONLINE_INSPECTION_THREAD_LOG://是否儲存線上檢測執行緒訊息
		Section = _T("System Parameter");
		KeyName = _T("Save Online Inspection Thread Log");
		break;
	case SYSTEM_SAVE_REMOVE_FOLDER_THREAD_LOG://是否儲存刪除資料夾執行緒訊息
		Section = _T("System Parameter");
		KeyName = _T("Save Remove Folder Thread Log");
		break;
	case SYSTEM_SAVE_CONVEYER_PRE_RUN_THREAD_LOG://是否儲存軌道自動運轉執行緒訊息
		Section = _T("System Parameter");
		KeyName = _T("Save Conveyer Pre-Run Thread Log");
		break;
	case SYSTEM_SAVE_ITS_PROC_THREAD_LOG://是否儲存ITS運作執行緒訊息
		Section = _T("System Parameter");
		KeyName = _T("Save ITS Proc Thread Log");
		break;
	case SYSTEM_SAVE_LOAD_REPAIR_FILE_THREAD_LOG://是否儲存載入維修站檔案執行緒訊息
		Section = _T("System Parameter");
		KeyName = _T("Save Load Repair File Thread Log");
		break;
	case SYSTEM_SAVE_REPAIR_RESULT_SIGNAL_THREAD_LOG://是否儲存維修站訊息執行緒訊息
		Section = _T("System Parameter");
		KeyName = _T("Save Repair Result Signal Thread Log");
		break;
	case SYSTEM_SAVE_SIMPLE_JOB_THREAD_LOG://是否儲存簡易作業執行緒訊息
		Section = _T("System Parameter");
		KeyName = _T("Save Simple Job Thread Log");
		break;
	case SYSTEM_SAVE_HASI_MONITOR_THREAD_LOG://是否儲存HASI執行緒訊息
		Section = _T("System Parameter");
		KeyName = _T("Save HASI Monitor Thread Log");
		break;
	case SYSTEM_SAVE_TEST_OBJECT_FINISH_LOG://是否儲存檢測物件結束訊息
		Section = _T("System Parameter");
		KeyName = _T("Save Test Object Finish Log");
		break;
	case SYSTEM_SAVE_UI_DRAW_FUNC_LOG://是否儲存介面重繪函式訊息
		Section = _T("System Parameter");
		KeyName = _T("Save UI Draw Func Log");
		break;	
	case SYSTEM_SAVE_USER_OPERATION_LOG://是否儲存使用者操作訊息
		Section = _T("System Parameter");
		KeyName = _T("Save User Operation Log");
		break;
	case SYSTEM_SAVE_TEST_TRACK_FILE: //是否儲存檢測追蹤檔案
		Section = _T("System Parameter");
		KeyName = _T("Save Test Track File");
		break;
	case SYSTEM_SAVE_PROJECT_REPORT_TEXT://是否專案文字檔報告
		Section = _T("System Parameter");
		KeyName = _T("Save Project Text Report");
		break;
	case SYSTEM_SAVE_PROJECT_REPORT_TEXT_FILENAME_MODE://儲存專案文字檔報告檔名
		Section = _T("System Parameter");
		KeyName = _T("Save Project Text Report Filename Mode");
		break;
	case SYSTEM_SAVE_CUSTOMER_REPORT_FILE://是否儲存客戶報告模式
		Section = _T("System Parameter");
		KeyName = _T("Save Customer Report File");
		break;
	case SYSTEM_SAVE_PROJECT_SPC_FILE_MODE://儲存專案SPC檔案模式	
		Section = _T("System Parameter");
		KeyName = _T("Save Project SPC File Mode");
		break;
	case SYSTEM_SAVE_PROJECT_SPC_LIBRARY_MODE://儲存專案SPC資料庫模式
		Section = _T("System Parameter");
		KeyName = _T("Save Project SPC Library Mode");
		break;
	case SYSTEM_SAVE_PROJECT_REPORT_WND_READING://是否儲存專案檢測框數據檔案	
		Section = _T("System Parameter");
		KeyName = _T("Save Project Wnd Reading Report");
		break;
	case SYSTEM_PRE_LOAD_PROJECT_OFFLINE_IMAGE://提前載入專案離線圖檔
		Section = _T("System Parameter");
		KeyName = _T("Pre Load Project Offline Image");
		break;
	case SYSTEM_AUTO_RELEASE_FIELD_FRAME_BUFFER://自動釋放區域影像
		Section = _T("System Parameter");
		KeyName = _T("Auto Release Field Frame Buffer");
		break;
	case SYSTEM_AUTO_RELEASE_OFFLINE_FIELD_FRAME_BUFFER://自動釋放離線編程區域影像
		Section = _T("System Parameter");
		KeyName = _T("Auto Release Offline Field Frame Buffer");
		break;	
	case SYSTEM_MACHINE_MODEL_TYPE://設備機種樣式
		Section = _T("System Parameter");
		KeyName = _T("Machine Model Type");
		break;
	case SYSTEM_MACHINE_CAMERA_SIDE://設備相機方向
		Section = _T("System Parameter");
		KeyName = _T("Machine Camera Side");
		break;	
	case SYSTEM_MULTI_LANGUAGE_MODE://多國語系版本
		Section = _T("System Parameter");
		KeyName = _T("Multi Language Mode");
		break;	
	case SYSTEM_CAMERA_DEBAYER_MODE://影像還元彩色模式
		Section = _T("System Parameter");
		KeyName = _T("Camera Image Debayer Mode");
		break;
	case SYSTEM_DC_STRECTCH_BLT_MODE://繪圖模式
		Section = _T("System Parameter");
		KeyName = _T("DC Stretch Blt Mode");
		break;
	case SYSTEM_IMAGE_DISPLAY_MODE://影像顯示模式
		Section = _T("System Parameter");
		KeyName = _T("Image Display Mode");
		break;
	case SYSTEM_IMAGE_DISPLAY_ENHANCE_MODE://影像顯示強化模式
		Section = _T("System Parameter");
		KeyName = _T("Image Display Enhance Mode");
		break;
	case SYSTEM_IMAGE_DISPLAY_GAIN://影像顯示的Gain
		Section = _T("System Parameter");
		KeyName = _T("Image Display Gain");
		break;
	case SYSTEM_IMAGE_DISPLAY_GAMMA://影像顯示的Gamma
		Section = _T("System Parameter");
		KeyName = _T("Image Display Gamma");
		break;
	case SYSTEM_IMAGE_DISPLAY_SHARPNESS_RADIUS://影像顯示的銳利化的半徑
		Section = _T("System Parameter");
		KeyName = _T("Image Display Sharpness Radius");
		break;
	case SYSTEM_IMAGE_DISPLAY_SHARPNESS_AMOUNT://影像顯示的銳利化的總量
		Section = _T("System Parameter");
		KeyName = _T("Image Display Sharpness Amount");
		break;
	case SYSTEM_IMAGE_DISPLAY_SHARPNESS_THRESHOLD://影像顯示的銳利化的閥值
		Section = _T("System Parameter");
		KeyName = _T("Image Display Sharpness Threshold");
		break;
	case SYSTEM_IMAGE_DISPLAY_LOCAL_GAMMA_CALC_SIZE://影像顯示的局部Gamma的計算範圍		
		Section = _T("System Parameter");
		KeyName = _T("Image Display Local Gamma Calc Size");
		break;
	case SYSTEM_IMAGE_DISPLAY_LOCAL_GAMMA_SCALE_VAL://影像顯示的局部Gamma的縮放比例
		Section = _T("System Parameter");
		KeyName = _T("Image Display Local Gamma Scale Value");
		break;
	case SYSTEM_IMAGE_DISPLAY_MAX_ZOOM_SCALE://影像顯示最大縮放比例	
		Section = _T("System Parameter");
		KeyName = _T("Image Display Max Zoom Scale");
		break;
	case SYSTEM_ONLINE_FORM_VIEW_MODE://線上檢測介面模式	
		Section = _T("System Parameter");
	#ifndef OFFLINE_VERSION
		KeyName = _T("Online Form View Mode");
	#else
		KeyName = _T("Online Form View Mode Offline");
	#endif//OFFLINE_VERSION
		break;
	case SYSTEM_CALC_FOCUS_SMOOTH_SIZE://計算焦距平滑尺寸
		Section = _T("System Parameter");
		KeyName = _T("Calc Focus Smooth Size");
		break;
	case SYSTEM_CALC_FOCUS_MODE://計算焦距模式
		Section = _T("System Parameter");
		KeyName = _T("Calc Focus Mode");
		break;
	case SYSTEM_LIBRARY_MODE_MATCH://影像匹配函式庫樣式
		Section = _T("System Parameter");
		KeyName = _T("Match Library Type");
		break;
	case SYSTEM_HONEYWELL_SWIFT_DECODER_ENABLED://Honeywell SwiftDecoder啟用
		Section = _T("System Parameter");
		KeyName = _T("Enable Honeywell Swift Decoder");
		break;
	case SYSTEM_DONGLE_WARNING_REMAINING_DAYS://硬體鎖警告剩餘天數
		Section = _T("System Parameter");
		KeyName = _T("Dongle Warning Remaining Days");
		break;
	case SYSTEM_DONGLE_WARNING_REMAINING_COUNT://硬體鎖警告剩餘次數
		Section = _T("System Parameter");
		KeyName = _T("Dongle Warning Remaining Count");
		break;
	case SYSTEM_CONNECT_LAST_STATION_MODE://與上一站連線方式
		Section = _T("System Parameter");
		KeyName = _T("Last Station Line Mode");
		break;	
	case SYSTEM_LANE_WORK_MODEL_LA://A軌道運轉模式
		Section = _T("System Parameter");
		KeyName = _T("SYSTEM_LANE_WORK_MODEL_LA");
		break;
	case SYSTEM_LANE_WORK_MODEL_LB://B軌道運轉模式
		Section = _T("System Parameter");
		KeyName = _T("SYSTEM_LANE_WORK_MODEL_LB");
		break;
	case SYSTEM_MULTI_TOWER_LIGHT_MODE://多塔燈模式
		Section = _T("System Parameter");
		KeyName = _T("Multi Tower Light Mode");
		break;
	case SYSTEM_CHECK_PCB_REMOVED_COUNT://確認PCB板移走次數
		Section = _T("System Parameter");
		KeyName = _T("Check PCB Removed Count");
		break;
	case SYSTEM_NEXT_CONNECTED_BUFFER_TYPE://確認PCB下一站連接輸送帶樣式
		Section = _T("System Parameter");
		KeyName = _T("Next Buffer Type");
		break;
	case SYSTEM_MULTI_PROJECT_TEST_ORDER_MODE://多專案檢測次序模式		
		Section = _T("System Parameter");
		KeyName = _T("Multi Project Test Order Mode");
		break;
	case SYSTEM_ONLINE_INPUT_PROJECT_WORK_NUMBER://在線輸入-專案工單號碼
		Section = _T("System Parameter");
		KeyName = _T("Online Input Project Work Number");
		break;
	case SYSTEM_ONLINE_AUTO_CALIBRATION_DLP_LED_COLOR://在線自動校正DLP-LED-顏色
		Section = _T("System Parameter");
		KeyName = _T("Online Auto Calibration DLP LED Color");
		break;
	case SYSTEM_ONLINE_AUTO_CALIBRATION_CAP_DELAY_TIME://在線自動校正上蓋延遲時間-ms
		Section = _T("System Parameter");
		KeyName = _T("Online Auto Calibration Target Cap Delay Time");
		break;
	case SYSTEM_ONLINE_AUTO_CALIBRATION_MODE_XYZ_HOME://在線自動校正模式-XYZ歸零
		Section = _T("System Parameter");
		KeyName = _T("Online Auto Calibration Mode XYZ Home");
		break;
	case SYSTEM_ONLINE_AUTO_CALIBRATION_PERIOD_XYZ_HOME://在線自動校正週期-XYZ歸零-小時
		Section = _T("System Parameter");
		KeyName = _T("Online Auto Calibration Period XYZ Home");
		break;
	case SYSTEM_ONLINE_AUTO_CALIBRATION_MODE_2D_CURRENT://在線自動校正模式-2D電流
		Section = _T("System Parameter");
		KeyName = _T("Online Auto Calibration Mode 2D Current");
		break;
	case SYSTEM_ONLINE_AUTO_CALIBRATION_PERIOD_2D_CURRENT://在線自動校正週期-2D電流-小時
		Section = _T("System Parameter");
		KeyName = _T("Online Auto Calibration Period 2D Current");
		break;
	case SYSTEM_ONLINE_AUTO_CALIBRATION_PERIOD_3D_CURRENT://在線自動校正週期-3D電流-小時
		Section = _T("System Parameter");
		KeyName = _T("Online Auto Calibration Period 3D Current");
		break;
	case SYSTEM_ONLINE_AUTO_CALIBRATION_MODE_3D_ZERO_PLANE://在線自動校正模式-3D相平面
		Section = _T("System Parameter");
		KeyName = _T("Online Auto Calibration Mode 3D Zero Plane");
		break;
	case SYSTEM_ONLINE_AUTO_CALIBRATION_PERIOD_3D_ZERO_PLANE://在線自動校正週期-3D相平面-小時
		Section = _T("System Parameter");
		KeyName = _T("Online Auto Calibration Period 3D Zero Plane");
		break;
	case SYSTEM_ONLINE_AUTO_CALIBRATION_PERIOD_3D_HEIGHT_FACTOR://在線自動校正週期-3D高度比例-小時
		Section = _T("System Parameter");
		KeyName = _T("Online Auto Calibration Period 3D Height Factor");
		break;
	case SYSTEM_ONLINE_AUTO_STOP_BY_IDLE_TIME://在線自動停機-閒置時間-分鐘
		Section = _T("System Parameter");
		KeyName = _T("Online Auto Stop By Idle Time");
		break;
	case SYSTEM_ONLINE_AUTO_STOP_BY_SPEC_TIME_1://在線自動停機-特定時間-時時分分秒秒
		Section = _T("System Parameter");
		KeyName = _T("Online Auto Stop By Spec Time 1");
		break;
	case SYSTEM_ONLINE_AUTO_STOP_BY_SPEC_TIME_2://在線自動停機-特定時間-時時分分秒秒
		Section = _T("System Parameter");
		KeyName = _T("Online Auto Stop By Spec Time 2");
		break;
	case SYSTEM_ONLINE_AUTO_STOP_BY_SPEC_TIME_3://在線自動停機-特定時間-時時分分秒秒
		Section = _T("System Parameter");
		KeyName = _T("Online Auto Stop By Spec Time 3");
		break;
	case SYSTEM_AUTO_SWITCH_TO_ONILINEVIEW_TIME://自動切換線上畫面-秒	
		Section = _T("System Parameter");
		KeyName = _T("Auto Switch To OnlineView Time");		
		break;
	case SYSTEM_ONLINE_SHOW_PROJECT_TEST_MAP://在線顯示專案檢測底圖
		Section = _T("System Parameter");
		KeyName = _T("Online Show Project Test Map");		
		break;
	case SYSTEM_AUTO_SWITCH_TO_ONILINE_REMOTE_CTRL_TIME://自動切換線上遠端控制時間-毫秒
		Section = _T("System Parameter");
		KeyName = _T("Auto Switch To Online Remote Ctrl Time");
		break;
	case SYSTEM_AUTO_SWITCH_TO_ONILINE_REMOTE_CTRL_MODE://自動切換線上遠端控制模式
		Section = _T("System Parameter");
		KeyName = _T("Auto Switch To Online Remote Ctrl Mode");
		break;
	case SYSTEM_PHASE_PERIOD_1://第1個相位的週期
		Section = _T("System Parameter");
		KeyName = _T("Phase Period 1");
		break;
	case SYSTEM_PHASE_PERIOD_2://第2個相位的週期
		Section = _T("System Parameter");
		KeyName = _T("Phase Period 2");
		break;
	case SYSTEM_PHASE_PERIOD_3://第3個相位的週期
		Section = _T("System Parameter");
		KeyName = _T("Phase Period 3");	
		break;
	case SYSTEM_SEPARATE_DLP_2EXP_TABLE://分離DLP2次曝光表格
		Section = _T("System Parameter");
		KeyName = _T("Separate DLP 2-Exposure Table");	
		break;
	case SYSTEM_PHASE_CONVERT_HEIGHT_MODE://相位轉換高度模式
		Section = _T("System Parameter");
		KeyName = _T("Phase Convert Height Mode");	
		break;
	case SYSTEM_PHASE_NOISE_DEFINE://相位雜訊定義啟用
		Section = _T("System Parameter");
		KeyName = _T("Phase Noise Define");
		break;
	case SYSTEM_PHASE_NOISE_DEFINE_MODE://相位雜訊定義模式
		Section = _T("System Parameter");
		KeyName = _T("Phase Noise Define Mode");
		break;
	case SYSTEM_PHASE_NOISE_EXTEND_VOID://相位遮罩-無效點外擴
		Section = _T("System Parameter");
		KeyName = _T("Phase Noise Extend Void");
		break;
	case SYSTEM_PHASE_NOISE_LOW_CONTRAST://相位遮罩-低對比
		Section = _T("System Parameter");
		KeyName = _T("Phase Noise Low Contrast");
		break;	
	case SYSTEM_PHASE_NOISE_LOW_POTENTIAL://相位遮罩-低潛力
		Section = _T("System Parameter");
		KeyName = _T("Phase Noise Low Potential");
		break;
	case SYSTEM_PHASE_NOISE_OVER_SATURATED://相位遮罩-過亮度
		Section = _T("System Parameter");
		KeyName = _T("Phase Noise Over Saturated");
		break;		
	case SYSTEM_PHASE_NOISE_LOW_CONTRAST_B://相位遮罩-低對比
		Section = _T("System Parameter");
		KeyName = _T("Phase Noise Low Contrast Pattern B");
		break;
	case SYSTEM_PHASE_NOISE_LOW_POTENTIAL_B://相位遮罩-低潛力	
		Section = _T("System Parameter");
		KeyName = _T("Phase Noise Low Potential Pattern B");
		break;
	case SYSTEM_PHASE_NOISE_OVER_SATURATED_B://相位遮罩-過亮度	
		Section = _T("System Parameter");
		KeyName = _T("Phase Noise Over Saturated Pattern B");
		break;
	case SYSTEM_PHASE_NOISE_LOW_CONTRAST_C://相位遮罩-低對比
		Section = _T("System Parameter");
		KeyName = _T("Phase Noise Low Contrast Pattern C");
		break;
	case SYSTEM_PHASE_NOISE_LOW_POTENTIAL_C://相位遮罩-低潛力	
		Section = _T("System Parameter");
		KeyName = _T("Phase Noise Low Potential Pattern C");
		break;
	case SYSTEM_PHASE_NOISE_OVER_SATURATED_C://相位遮罩-過亮度	
		Section = _T("System Parameter");
		KeyName = _T("Phase Noise Over Saturated Pattern C");
		break;
	case SYSTEM_PHASE_NOISE_SMOOTH_FILTER://相位遮罩-平滑處理
		Section = _T("System Parameter");
		KeyName = _T("Phase Noise Smooth Filter");
		break;
	case SYSTEM_PHASE_NOISE_LOW_CONTRAST_COLOR://相位遮罩-低對比-顏色
		Section = _T("System Parameter");
		KeyName = _T("Phase Noise Low Contrast Color");
		break;			
	case SYSTEM_PHASE_NOISE_LOW_POTENTIAL_COLOR://相位遮罩-潛力-顏色
		Section = _T("System Parameter");
		KeyName = _T("Phase Noise Low Potential Color");
		break;	
	case SYSTEM_PHASE_NOISE_OVER_SATURATED_COLOR://相位遮罩-過亮度-顏色
		Section = _T("System Parameter");
		KeyName = _T("Phase Noise Over Saturated Color");
		break;	
	case SYSTEM_PHASE_NOISE_EXTEND_VOID_COLOR://相位遮罩-無效點外擴-顏色
		Section = _T("System Parameter");
		KeyName = _T("Phase Noise Extend Void Color");
		break;	
	case SYSTEM_SPACE_NOISE_HEIGHT_UNEXPECTED_COLOR://相位遮罩-高度異常-顏色
		Section = _T("System Parameter");
		KeyName = _T("Space Noise Height Unexpected Color");
		break;	
	case SYSTEM_SPACE_NOISE_HEIGHT_OVER_LOW_COLOR://相位遮罩-過低異常-顏色
		Section = _T("System Parameter");
		KeyName = _T("Space Noise Height Over Low Color");
		break;
	case SYSTEM_SPACE_VALID_BEST_COLOR://相位遮罩-可靠高度-顏色	
		Section = _T("System Parameter");
		KeyName = _T("Space Best Valid Color");
		break;

	case SYSTEM_3D_OBJECT_DRAW_SCALE_X://3D物件顯示比例-X		
		Section = _T("System Parameter");
		KeyName = _T("3D Object Draw Scale X");
		break;
	case SYSTEM_3D_OBJECT_DRAW_SCALE_Y://3D物件顯示比例-Y
		Section = _T("System Parameter");
		KeyName = _T("3D Object Draw Scale Y");
		break;
	case SYSTEM_3D_OBJECT_DRAW_SCALE_Z://3D物件顯示比例-Z
		Section = _T("System Parameter");
		KeyName = _T("3D Object Draw Scale Z");
		break;

	case SYSTEM_PANEL_COLOR_1://整板的顏色-1
		Section = _T("System Parameter");
		KeyName = _T("Panel Color 1");
		break;
	case SYSTEM_PANEL_COLOR_2://整板的顏色-2	
		Section = _T("System Parameter");
		KeyName = _T("Panel Color 2");
		break;
	case SYSTEM_PANEL_TEXT_COLOR://整板文字顏色	
		Section = _T("System Parameter");
		KeyName = _T("Panel Text Color");
		break;
	case SYSTEM_PANEL_SELECTED_COLOR://整板選取到的顏色
		Section = _T("System Parameter");
		KeyName = _T("Panel Selected Color");
		break;
	case SYSTEM_BOARD_COLOR_1://單板的顏色-1
		Section = _T("System Parameter");
		KeyName = _T("Board Color 1");
		break;
	case SYSTEM_BOARD_COLOR_2://單板的顏色-2	
		Section = _T("System Parameter");
		KeyName = _T("Board Color 2");
		break;
	case SYSTEM_BOARD_TEXT_COLOR://單板文字顏色	
		Section = _T("System Parameter");
		KeyName = _T("Board Text Color");
		break;
	case SYSTEM_BOARD_SELECTED_COLOR://單板選取到的顏色
		Section = _T("System Parameter");
		KeyName = _T("Board Selectd Color");
		break;

	case SYSTEM_FD_COLOR_1://定位點的顏色-1
		Section = _T("System Parameter");
		KeyName = _T("Fd Color 1");
		break;
	case SYSTEM_FD_COLOR_2://定位點的顏色-2		
		Section = _T("System Parameter");
		KeyName = _T("Fd Color 2");
		break;
	case SYSTEM_FD_TEXT_COLOR://定位點文字顏色
		Section = _T("System Parameter");
		KeyName = _T("Fd Text Color");
		break;
	case SYSTEM_FD_SELECTED_COLOR://定位點選取到的顏色
		Section = _T("System Parameter");
		KeyName = _T("Fd Selected Color");
		break;

	case SYSTEM_BARCODE_COLOR_1://條碼的顏色-1
		Section = _T("System Parameter");
		KeyName = _T("Barcode Color 1");
		break;
	case SYSTEM_BARCODE_COLOR_2://條碼的顏色-2		
		Section = _T("System Parameter");
		KeyName = _T("Barcode Color 2");
		break;
	case SYSTEM_BARCODE_TEXT_COLOR://條碼文字顏色
		Section = _T("System Parameter");
		KeyName = _T("Barcode Text Color");
		break;
	case SYSTEM_BARCODE_SELECTED_COLOR://條碼選取到的顏
		Section = _T("System Parameter");
		KeyName = _T("Barcode Selected Color");
		break;
	case SYSTEM_COMPONENT_COLOR_1://零件的顏色
		Section = _T("System Parameter");
		KeyName = _T("Component Color 1");
		break;
	case SYSTEM_COMPONENT_COLOR_2://零件的顏色-2
		Section = _T("System Parameter");
		KeyName = _T("Component Color 2");
		break;
	case SYSTEM_COMPONENT_TEXT_COLOR://零件文字顏色
		Section = _T("System Parameter");
		KeyName = _T("Component Text Color");
		break;
	case SYSTEM_COMPONENT_SELECTED_COLOR://零件選取到的顏色
		Section = _T("System Parameter");
		KeyName = _T("Component Selected Color");
		break;
	case SYSTEM_DISTRICT_COLOR_1://分段的顏色-1
		Section = _T("System Parameter");
		KeyName = _T("District Color 1");
		break;
	case SYSTEM_DISTRICT_COLOR_2://分段的顏色-2
		Section = _T("System Parameter");
		KeyName = _T("District Color 2");
		break;
	case SYSTEM_INSPECTED_RESULT_OK_COLOR://檢測OK顏色
		Section = _T("System Parameter");
		KeyName = _T("Inspected Result OK Color");
		break;
	case SYSTEM_INSPECTED_RESULT_NG_COLOR://檢測NG顏色
		Section = _T("System Parameter");
		KeyName = _T("Inspected Result NG Color");
		break;
	case SYSTEM_INSPECTED_RESULT_SKIP_COLOR://檢測Skip顏色
		Section = _T("System Parameter");
		KeyName = _T("Inspected Result Skip Color");
		break;
	case SYSTEM_INSPECTED_RESULT_BYPASS_COLOR://檢測Bypass顏色
		Section = _T("System Parameter");
		KeyName = _T("Inspected Result Bypass Color");
		break;
	case SYSTEM_INSPECTED_RESULT_UNTEST_COLOR://檢測UnTest顏色
		Section = _T("System Parameter");
		KeyName = _T("Inspected Result Un-Test Color");
		break;
	case SYSTEM_INSPECTED_RESULT_WARNING_COLOR://檢測Warning顏色		
		Section = _T("System Parameter");
		KeyName = _T("Inspected Result Warning Color");
		break;
	case SYSTEM_INSPECTED_RESULT_EXCEPTION_COLOR://檢測Exception顏色
		Section = _T("System Parameter");
		KeyName = _T("Inspected Result Exception Color");
		break;
	case SYSTEM_SPACE_NOISE_SINGLE_CAST_LOW_LIMIT://高度雜訊單投光高度最低極限-um
		Section = _T("System Parameter");
		KeyName = _T("Space Noise Single Cast Low Limit (um)");
		break;
	case SYSTEM_SPACE_NOISE_MULTI_CAST_PATCH_SIZE://高度雜訊多投光合併Patch尺寸
		Section = _T("System Parameter");
		KeyName = _T("Space Noise Multi Cast Patch Size");
		break;
	case SYSTEM_SPACE_NOISE_MULTI_CAST_MERGE_MODE://高度雜訊多投光合併模式
		Section = _T("System Parameter");
		KeyName = _T("Space Noise Multi Cast Merge Mode");
		break;
	case SYSTEM_SPACE_NOISE_MULTI_CAST_MERGE_BEST_MODE://高度雜訊多投光合併最可靠模式
		Section = _T("System Parameter");
		KeyName = _T("Space Noise Multi Cast Merge Best Mode");
		break;
	case SYSTEM_SPACE_NOISE_MULTI_INTENSITY_MERGE_MODE://高度雜訊多亮度合併模式-Mean, MaxB
		Section = _T("System Parameter");
		KeyName = _T("Space Noise Multi Intensity Merge Mode");
		break;
	case SYSTEM_SPACE_NOISE_MULTI_CAST_MIN_VALID_COUNT://高度雜訊多投光最少有效值數	
		Section = _T("System Parameter");
		KeyName = _T("Space Noise Multi Cast Min Valid Count");
		break;
	case SYSTEM_SPACE_NOISE_MULTI_CAST_MAX_DIFFERENCE://高度雜訊多投光高度最大差值-um	
		Section = _T("System Parameter");
		KeyName = _T("Space Noise Multi Cast Max Difference (um)");
		break;	
	case SYSTEM_SPACE_NOISE_MULTI_CAST_LIMIT_DIFFERENCE://高度雜訊多投光高度極限差值-um	
		Section = _T("System Parameter");
		KeyName = _T("Space Noise Multi Cast Limit Difference (um)");
		break;
	case SYSTEM_SPACE_NOISE_MULTI_CAST_VALID_BEST_RATIO://高度雜訊多投光高度最好比例-um	
		Section = _T("System Parameter");
		KeyName = _T("Space Noise Multi Cast Valid Best Ratio");
		break;
	case SYSTEM_SPACE_NOISE_MULTI_CAST_VALID_DIFFERENCE://高度雜訊多投光高度有效差值-um
		Section = _T("System Parameter");
		KeyName = _T("Space Noise Multi Cast Valid Difference (um)");
		break;
	case SYSTEM_SPACE_NOISE_MULTI_CAST_OPPOSITE_MAX_GRAY://高度雜訊多投光合併對邊灰階上限-gray	
		Section = _T("System Parameter");
		KeyName = _T("Space Noise Multi Cast Opposite Max Gray");
		break;
	case SYSTEM_SPACE_NOISE_CAST_FILTER_MODE://投光濾波後模式
		Section = _T("System Parameter");
		KeyName = _T("Space Noise Cast Filter Mode");
		break;
	case SYSTEM_SPACE_NOISE_CAST_MEDIAN_FILTER_SIZE://高度雜訊的中值濾波尺寸
		Section = _T("System Parameter");
		KeyName = _T("Space Noise Median Filter Size");
		break;
	case SYSTEM_SPACE_NOISE_CAST_MEDIAN_FILTER_USE_SIZE://高度雜訊的中值濾波使用尺寸
		Section = _T("System Parameter");
		KeyName = _T("Space Noise Median Filter Use Size");
		break;
	case SYSTEM_SPACE_NOISE_DATA_VOID_EXPAND_SIZE://空間雜訊無效點外擴尺寸-piexel	
		Section = _T("System Parameter");
		KeyName = _T("Space Noise Data Void Expand Size");
		break;
	case SYSTEM_SPACE_NOISE_DATA_VOID_EXPAND_ENABLED://空間雜訊無效點外擴啟用
		Section = _T("System Parameter");
		KeyName = _T("Space Noise Data Void Expand Enabled");
		break;
	case SYSTEM_SPACE_NOISE_FIRST_FILTER_MODE://空間雜訊首次濾波模式
		Section = _T("System Parameter");
		KeyName = _T("Space Noise First Filter Mode");
		break;
	case SYSTEM_SPACE_NOISE_FIRST_FILTER_PITCH://空間雜訊首次濾波步長
		Section = _T("System Parameter");
		KeyName = _T("Space Noise First Filter Pitch");
		break;
	case SYSTEM_SPACE_NOISE_FIRST_KER_SIZE://空間雜訊首次濾波尺寸
		Section = _T("System Parameter");
		KeyName = _T("Space Noise First Filter Size");
		break;
	case SYSTEM_SPACE_NOISE_FIRST_USE_SIZE://空間雜訊首次使用尺寸
		Section = _T("System Parameter");
		KeyName = _T("Space Noise First Use Size");
		break;
	case SYSTEM_SPACE_NOISE_OVER_LOW_MODE://高度雜訊過低模式
		Section = _T("System Parameter");
		KeyName = _T("Space Noise Over Lower Filter Mode");
		break;	
	case SYSTEM_SPACE_NOISE_OVER_LOW_RANGE://高度雜訊過低高度-um
		Section = _T("System Parameter");
		KeyName = _T("Space Noise Over Lower Range (um)");
		break;	
	case SYSTEM_SPACE_NOISE_OVER_LOW_LIMIT://高度雜訊過低極限-um	
		Section = _T("System Parameter");
		KeyName = _T("Space Noise Over Lower Limit (um)");
		break;	
	case SYSTEM_SPACE_NOISE_OVER_LOW_KER_SIZE://高度雜訊過低濾波尺寸
		Section = _T("System Parameter");
		KeyName = _T("Space Noise Over Lower Filter Size");
		break;
	case SYSTEM_SPACE_NOISE_OVER_LOW_USE_SIZE://高度雜訊過低使用尺寸
		Section = _T("System Parameter");
		KeyName = _T("Space Noise Over Lower Use Size");
		break;
	case SYSTEM_SPACE_NOISE_HEIGHT_ABNORMAL_MODE://高度雜訊高度異常模式
		Section = _T("System Parameter");
		KeyName = _T("Space Noise Height Abnormal Mode");
		break;
	case SYSTEM_SPACE_NOISE_HEIGHT_ABNORMAL_PITCH://高度雜訊高度異常步長
		Section = _T("System Parameter");
		KeyName = _T("Space Noise Height Abnormal Pitch");
		break;
	case SYSTEM_SPACE_NOISE_HEIGHT_ABNORMAL_RANGE://高度雜訊高度異常範圍-um		
		Section = _T("System Parameter");
		KeyName = _T("Space Noise Height Abnormal Range");
		break;
	case SYSTEM_SPACE_NOISE_HEIGHT_ABNORMAL_CHK_SIZE://高度雜訊高度異常確認尺寸
		Section = _T("System Parameter");
		KeyName = _T("Space Noise Height Abnormal Check Size");
		break;
	case SYSTEM_SPACE_NOISE_HEIGHT_ABNORMAL_KER_SIZE://高度雜訊高度異常濾波尺寸
		Section = _T("System Parameter");
		KeyName = _T("Space Noise Height Abnormal Filter Size");
		break;
	case SYSTEM_SPACE_NOISE_HEIGHT_ABNORMAL_USE_SIZE://高度雜訊高度異常使用尺寸
		Section = _T("System Parameter");
		KeyName = _T("Space Noise Height Abnormal Use Size");
		break;	
	case SYSTEM_SPACE_NOISE_HEIGHT_ABNORMAL_REPEAT_CNT://高度雜訊高度異常重複次數	
		Section = _T("System Parameter");
		KeyName = _T("Space Noise Height Abnormal Repeat Count");
		break;
	case SYSTEM_SPACE_NOISE_RECONTRUCTED_EXT_SIZE://空間雜訊重建外擴尺寸
		Section = _T("System Parameter");
		KeyName = _T("Space Noise Re-Contructed Extend Size");
		break;
	case SYSTEM_SPACE_NOISE_RECONTRUCTED_ENABLED://空間雜訊重建外擴啟用	
		Section = _T("System Parameter");
		KeyName = _T("Space Noise Re-Contructed Enabled");
		break;	
	case SYSTEM_SPACE_NOISE_FINAL_KER_SIZE://空間雜訊最後濾波尺寸
		Section = _T("System Parameter");
		KeyName = _T("Space Noise Final Filter Size");
		break;
	case SYSTEM_SPACE_NOISE_FINAL_USE_SIZE://空間雜訊最後使用尺寸
		Section = _T("System Parameter");
		KeyName = _T("Space Noise Final Use Size");
		break;
	case SYSTEM_SPACE_NOISE_FINAL_FILTER_MODE://空間雜訊最後濾波模式
		Section = _T("System Parameter");
		KeyName = _T("Space Noise Final Filter Mode");
		break;
	case SYSTEM_SPACE_NOISE_FINAL_FILTER_PITCH://空間雜訊最後濾波步長
		Section = _T("System Parameter");
		KeyName = _T("Space Noise Final Filter Pitch");
		break;
	case SYSTEM_SPACE_NOISE_FINAL_FILTER_MODE2://空間雜訊最後濾波模式-2
		Section = _T("System Parameter");
		KeyName = _T("Space Noise Final Filter Mode 2");
		break;
	case SYSTEM_SPACE_NOISE_FINAL_FILTER_PITCH2://空間雜訊最後濾波步長-2
		Section = _T("System Parameter");
		KeyName = _T("Space Noise Final Filter Pitch 2");
		break;
	case SYSTEM_SPACE_NOISE_FINAL_KER_SIZE2://空間雜訊最後濾波尺寸-2
		Section = _T("System Parameter");
		KeyName = _T("Space Noise Final Filter Size 2");
		break;
	case SYSTEM_SPACE_NOISE_FINAL_USE_SIZE2://空間雜訊最後使用尺寸-2
		Section = _T("System Parameter");
		KeyName = _T("Space Noise Final Use Size 2");
		break;
	case SYSTEM_SPACE_VERIFY_MULTI_CAST_GAP_RATIO://高度驗證-多投光差距比例上限-%
		Section = _T("System Parameter");
		KeyName = _T("Space Verify Multi Cast Gap Ratio");
		break;
	case SYSTEM_SPACE_VERIFY_MULTI_CAST_GAP_THRESHOLD://高度驗證-多投光差距閥值-um	
		Section = _T("System Parameter");
		KeyName = _T("Space Verify Multi Cast Gap Threshold");
		break;
	case SYSTEM_CUDA_FUNCTION_ENABLED://Cuda函式啟用
		Section = _T("System Parameter");
		KeyName = _T("Cuda Function Enabled");
		break;
	case SYSTEM_CUDA_NUMBER_BLOCK://Cuda Block數量
		Section = _T("System Parameter");
		KeyName = _T("Cuda Block Number");
		break;	
	case SYSTEM_CUDA_NUMBER_THREAD://Cuda Thread數量
		Section = _T("System Parameter");
		KeyName = _T("Cuda Thread Number");
		break;	
	case SYSTEM_AUTO_RETRY_MAX_COUNT://自動重測次數上限
		Section = _T("System Parameter");
		KeyName = _T("Auto Retry Max Count");
		break;
	case SYSTEM_AUTO_SETUP_DLP_PATTERN://自動設定DLP樣板	
		Section = _T("System Parameter");
		KeyName = _T("Auto Setup DLP Pattern");
		break;
	case SYSTEM_RESOLUTION_MODE://解析度模式
		Section = _T("System Parameter");
		KeyName = _T("Image Resolution Mode");
		break;		
	case SYSTEM_RESOLUTION_SHOW_SCALE://解析度顯示比例
		Section = _T("System Parameter");
		KeyName = _T("Image Resolution Show Scale");
		break;
	case SYSTEM_MOVE_CAMERA_BEFORE_PCB_IN://進板前移動相機頭
		Section = _T("System Parameter");
		KeyName = _T("Move Camera Before PCB-In");
		break;
	case SYSTEM_PCB_IN_USE_CAMERA_IMAGE_MODE://進板使用相機影像模式
		Section = _T("System Parameter");
		KeyName = _T("PCB-In Use Camera Image Mode");
		break;
	case SYSTEM_CLAMP_PCB_BEFORE_TEST_MODE://檢測前夾板模式	
		Section = _T("System Parameter");
		KeyName = _T("Clamp PCB Before Test Mode");
		break;
	case SYSTEM_GRAB_FIDUCIAL_DELAY_TIME://解取定位點影像延遲時間
		Section = _T("System Parameter");
		KeyName = _T("Grab Fiducial Delay Time");
		break;	
	case SYSTEM_PCB_OUT_DIRECTION://PCB出板方向
		Section = _T("System Parameter");
		KeyName = _T("PCB Out Direction");
		break;
	case SYSTEM_BYPASS_LAST_SIGNAL://忽略上一站訊號-回板使用
		Section = _T("System Parameter");
		KeyName = _T("Bypass Last Signal");
		break;
	case SYSTEM_BYPASS_NEXT_SIGNAL://忽略下一站訊號-回板使用	
		Section = _T("System Parameter");
		KeyName = _T("Bypass Next Signal");
		break;
	case SYSTEM_PCB_OK_NG_SIGNAL_DELAY_TIME://PCB OK/NG訊號延遲時間-ms
		Section = _T("System Parameter");
		KeyName = _T("PCB OK NG Signal Delay Time");
		break;
	case SYSTEM_EDIT_LINE_SIZE_LEVEL://編輯線尺寸的層級
		Section = _T("System Parameter");
		KeyName = _T("Edit Line Size Level");
		break;	
	case SYSTEM_ONLINE_TUNING_KEEP_MAX_TIME://線上調機保留最久時間-分鐘 	
		Section = _T("System Parameter");
		KeyName = _T("Online Tuning Keep Max Time");		
		break;
	case SYSTEM_ONLINE_TUNING_SAVED_MAX_COUNT://線上調機儲存最多數量-片數 
		Section = _T("System Parameter");
		KeyName = _T("Online Tuning Saved Max Count");		
		break;
	case SYSTEM_USER_LOGIN_ENABLED://使用者登入使用
		Section = _T("System Parameter");
		KeyName = _T("Enable User Login");		
		break;	
	case SYSTEM_USER_LOGIN_OPTIONS: //使用者登入選項
		Section = _T("System Parameter");
		KeyName = _T("User Login Options");
		break;
	case SYSTEM_OPEN_PROJECT_MODE://開啟專案模式
		Section = _T("System Parameter");
		KeyName = _T("Open Project Mode");		
		break;
	case SYSTEM_OPEN_PROJECT_MAP_INDEX://開啟專案底圖編號
		Section = _T("System Parameter");
		KeyName = _T("Open Project Map Index");				
		break;	
	case SYSTEM_VERIFY_PROJECT_MODE://驗證專案模式
		Section = _T("System Parameter");
		KeyName = _T("Verify Project Mode");		
		break;
	case SYSTEM_VERIFY_PROJECT_FILENAME://驗證專案的檔名
		Section = _T("System Parameter");
		KeyName = _T("Verify Project Filename");		
		break;
	case SYSTEM_MODEL_NAME_USE_PART_NUMBER://模組名稱使用料號 
		Section = _T("System Parameter");
		KeyName = _T("Model Name Use Part Number");		
		break;
	case SYSTEM_ONLINE_OPEN_PROJECT_MODE://線上開專案模式
		Section = _T("System Parameter");
		KeyName = _T("Online Open Project Mode");		
		break;
	case SYSTEM_ONLINE_OPEN_PROJECT_CAMERA_BARCODE://線上開專案-相機條碼
		Section = _T("System Parameter");
		KeyName = _T("Online Open Project Enable Camera Barcode");		
		break;
	case SYSTEM_ONLINE_OPEN_PROJECT_BARCODE_DEVICE_GRAB_MODE://線上開專案外接條碼取像模式
		Section = _T("System Parameter");
		KeyName = _T("Online Open Project Barcode Device Grab Mode");		
		break;
	case SYSTEM_MODEL_UNSET_COLOR://模組未設定顏色
		Section = _T("System Parameter");
		KeyName = _T("Model Unset Color");
		break;
	case SYSTEM_BINARY_MASK_COLOR_RGB://2值化遮罩-顏色-3色燈
		Section = _T("System Parameter");
		KeyName = _T("Binary Mask Color RGB");
		break;
	case SYSTEM_BINARY_MASK_COLOR_DEFAULT://2值化遮罩-顏色-預設
		Section = _T("System Parameter");
		KeyName = _T("Binary Mask Color Default");
		break;	
	case SYSTEM_BINARY_MASK_COLOR_ALPHA://2值化遮罩-顏色-透明度			
		Section = _T("System Parameter");
		KeyName = _T("Binary Mask Color Alpha");
		break;
	case SYSTEM_INSPECTION_FINISH_SHOW_RESULT_LIST://檢測結束顯示結果列表
		Section = _T("System Parameter");
		KeyName = _T("Inspection Finish Show Result List");
		break;
	case SYSTEM_MODEL_DEFAULT_WND_LEVEL://模組預設檢測框等級
		Section = _T("System Parameter");
		KeyName = _T("Model Default Wnd Level");
		break;
	case SYSTEM_SWITCH_PROJECT_3D_FRAME://可切換至專案3D畫面
		Section = _T("System Parameter");
		KeyName = _T("Switch Project 3D Frame");
		break;	
	case SYSTEM_AUTO_SWITCH_WND_3D_FRAME_MODE://自動切換至檢測框3D畫面模式
		Section = _T("System Parameter");
		KeyName = _T("Auto Switch Wnd 3D Frame Mode");
		break;
	case SYSTEM_AUTO_SWITCH_WND_3D_FRAME_SIZE_LIMIT://自動切換至檢測框3D畫面尺寸上限-um	
		Section = _T("System Parameter");
		KeyName = _T("Auto Switch Wnd 3D Frame Size Limit");
		break;
	case SYSTEM_SHOW_COMPONENT_FULL_MAP://顯示零件-整板零件
		Section = _T("System Parameter");
		KeyName = _T("Show Full-Map Components");
		break;
	case SYSTEM_SHOW_COMPONENT_DEFECT_ONLY://僅顯示瑕疵零件(線上畫面)	
		Section = _T("System Parameter");
		KeyName = _T("Show Defect Only Components");
		break;
	case SYSTEM_SHOW_DEBUG_FORM_VIEW://顯示除錯頁面 
		Section = _T("System Parameter");
		KeyName = _T("Show Debug Form View");
		break;
	case SYSTEM_LEVEL_FILTER_SHIFT_ENABLED://分等濾波起點偏移啟用
		Section = _T("System Parameter");
		KeyName = _T("Level Filter Shift Enabled");
		break;
	case SYSTEM_MEDIAN_FILTER_SHIFT_ENABLED://中值濾波起點偏移啟用
		Section = _T("System Parameter");
		KeyName = _T("Median Filter Shift Enabled");
		break;
	case SYSTEM_SMOOTH_FILTER_SHIFT_ENABLED://平滑濾波起點偏移啟用	
		Section = _T("System Parameter");
		KeyName = _T("Smooth Filter Shift Enabled");
		break;
	case SYSTEM_PYRAMID_MEDIAN_FILTER_SHIFT_ENABLED://金字塔中值濾波起點偏移啟用
		Section = _T("System Parameter");
		KeyName = _T("Pyramid Median Filter Shift Enabled");
		break;
	case SYSTEM_LEVEL_FILTER_PITCH_TOLERANCE://分等濾波步長可接受誤差
		Section = _T("System Parameter");
		KeyName = _T("Level Filter Pitch Tolerance");
		break;
	case SYSTEM_MEDIAN_FILTER_PITCH_TOLERANCE://中值濾波步長可接受誤差
		Section = _T("System Parameter");
		KeyName = _T("Median Filter Pitch Tolerance");
		break;
	case SYSTEM_SMOOTH_FILTER_PITCH_TOLERANCE://平滑濾波步長可接受誤差
		Section = _T("System Parameter");
		KeyName = _T("Smooth Filter Pitch Tolerance");
		break;
	case SYSTEM_PYRAMID_MEDIAN_FILTER_PITCH_TOLERANCE://金字塔中值濾波步長可接受誤差
		Section = _T("System Parameter");
		KeyName = _T("Pyramid Median Filter Pitch Tolerance");
		break;
	case SYSTEM_GRR_OFFSET_RANDOM_VALUE://Grr偏移隨機補償值
		Section = _T("System Parameter");
		KeyName = _T("GRR Offset Random Value");
		break;
	case SYSTEM_GRR_AVERAGE_RESET_ENABLED://Grr平均值復歸-啟用-依據條碼比較
		Section = _T("System Parameter");
		KeyName = _T("GRR Average Reset Enabled");
		break;
	case SYSTEM_GRR_AVERAGE_RESET_MATCH_RATIO://Grr平均值復歸-匹配比例
		Section = _T("System Parameter");
		KeyName = _T("GRR Average Reset Match Ratio");
		break;
	case SYSTEM_GRR_SKEW_AVERAGE_ENB_VAL://Grr偏移角度平均啟用角度
		Section = _T("System Parameter");
		KeyName = _T("GRR Skew Average Enable Angle");
		break;
	case SYSTEM_GRR_SKEW_AVERAGE_START_GAP://Grr偏移角度平均開始差距-角度	
		Section = _T("System Parameter");
		KeyName = _T("GRR Skew Average Start Gap");
		break;
	case SYSTEM_GRR_SKEW_AVERAGE_WEIGHTING://Grr偏移角度平均權重 
		Section = _T("System Parameter");
		KeyName = _T("GRR Skew Average Weighting");
		break;
	case SYSTEM_GRR_OFFSET_AVERAGE_ENB_PXL://Grr偏移平均啟用像素
		Section = _T("System Parameter");
		KeyName = _T("GRR Offset Average Enable Pixel");
		break;
	case SYSTEM_GRR_OFFSET_AVERAGE_START_GAP://Grr偏移平均開始差距-微米	
		Section = _T("System Parameter");
		KeyName = _T("GRR Offset Average Start Gap");
		break;
	case SYSTEM_GRR_OFFSET_AVERAGE_WEIGHTING://Grr偏移平均權重
		Section = _T("System Parameter");
		KeyName = _T("GRR Offset Average Weighting");
		break;
	case SYSTEM_GRR_HEIGHT_AVERAGE_ENB_VAL://Grr偏移高度平均啟用高度
		Section = _T("System Parameter");
		KeyName = _T("GRR Height Average Enable Value");
		break;
	case SYSTEM_GRR_HEIGHT_AVERAGE_START_GAP://Grr偏移高度平均開始差距-微米
		Section = _T("System Parameter");
		KeyName = _T("GRR Height Average Start Gap");
		break;
	case SYSTEM_GRR_HEIGHT_AVERAGE_WEIGHTING://Grr偏移高度平均權重 
		Section = _T("System Parameter");
		KeyName = _T("GRR Height Average Weighting");
		break;
	case SYSTEM_LOCK_SCREEN_ENABLED://啟用鎖住螢幕
		Section = _T("System Parameter");
		KeyName = _T("Enable Lock Screen Func");
		break;
	case SYSTEM_LOCK_SCREEN_KEY_ID://鎖住螢幕鍵號
		Section = _T("System Parameter");
		KeyName = _T("Lock Screen Key Number");
		break;
	case SYSTEM_MULTI_DISTRICT_MODE_ENABLED://多段檢測啟用
		Section = _T("System Parameter");
		KeyName = _T("Enable Multi District Mode");
		break;
		break;
	case SYSTEM_ONLINE_TUNING_ENABLE_BARCODE://在線調機-軟體條碼
		Section = _T("System Parameter");
		KeyName = _T("Enable Online Tuning Barcode");
		break;
	case SYSTEM_SHOW_PLC_SAFTY_SETTING_UI://是否顯示PLC安全檢知設定介面
		Section = _T("System Parameter");
		KeyName = _T("Show PLC Safty Setting UI");
		break;
	case SYSTEM_SHOW_ALG_OFFSET_L_PARAM://是否顯示演算法OffsetL的參數 	
		Section = _T("System Parameter");
		KeyName = _T("Show Alg Offset-L Parameter");
		break;
	case SYSTEM_SHOW_ALG_OFFSET_A_PARAM://是否顯示演算法OffsetA的參數
		Section = _T("System Parameter");
		KeyName = _T("Show Alg Offset-A Parameter");
		break;
	case SYSTEM_SHOW_ALG_BRIGHT_RATIO_SCALE_PARAM://是否顯示演算法亮度比例的比例參數
		Section = _T("System Parameter");
		KeyName = _T("Show Alg Bright-Ratio Scale Parameter");
		break;
	case SYSTEM_SHOW_MODEL_PROPERTY_PARAM://是否顯示模組屬性參數
		Section = _T("System Parameter");
		KeyName = _T("Show Model Property Parameter");
		break;	
	case SYSTEM_CONTINUE_PASTE_MODE://連續貼上模式
		Section = _T("System Parameter");
		KeyName = _T("Continue Paste Mode");
		break;
	case SYSTEM_STITCH_IMAGE_PADDING_SIZE://拼圖參數-填補尺寸
		Section = _T("System Parameter");
		KeyName = _T("Stitch Image Padding Size");
		break;
	case SYSTEM_RESIN_HEIGHT_ALIGN_ENABLED://啟用Resin高度對齊-軍達3D對位
		Section = _T("System Parameter");
		KeyName = _T("Resin Height Align Enabled");
		break;
	case SYSTEM_MODEL_IMAGE_CAD_OFFSET_ENABLED://啟用模組影像Cad偏移補償um
		Section = _T("System Parameter");
		KeyName = _T("Model Image Cad Offset Enabled");
		break;
	case SYSTEM_MODEL_DEFAULT_TRANSISTOR_TYPE://模組預設SOT樣式
		Section = _T("System Parameter");
		KeyName = _T("Model Default Transistor Type");
		break;
	case SYSTEM_PROJECT_LOCAL_FOLDER_ENABLED://專案使用本機資料夾
		Section = _T("System Parameter");
		KeyName = _T("Project Local Folder Enabled");
		break;
	case SYSTEM_PARTIAL_COPY_PROJECT_LIBRARY://部分複製專案資料庫	
		Section = _T("System Parameter");
		KeyName = _T("Partial Copy Project Library");
		break;
	case SYSTEM_AUTO_ARRANGE_MODEL_BK_IMAGE_FILES://自動重整模組底圖檔案
		Section = _T("System Parameter");
		KeyName = _T("Auto Arrange Model Bk Image Files");
		break;
	case SYSTEM_AUTO_COPY_SPC_COMPONENT_IMAGE_FILES://自動複製SPC零件圖檔
		Section = _T("System Parameter");
		KeyName = _T("Auto Copy Spc Component Image Files");
		break;
	case SYSTEM_AUTO_BYPASS_GRAB_3D_FRAME://自動跳過3D影像
		Section = _T("System Parameter");
		KeyName = _T("Auto Bypass Grab 3D Frame");
		break;
	case SYSTEM_CHECK_PROJECT_FD_READY://確認專案定位點狀態
		Section = _T("System Parameter");
		KeyName = _T("Check Project Fd Ready");
		break;
	case SYSTEM_LOCK_MODEL_BODY_POSITION://鎖住模組本體的位置
		Section = _T("System Parameter");
		KeyName = _T("Lock Model Body Position");
		break;	
	case SYSTEM_MAX_UNCHECK_TEST_FILE_COUNT://最多未判定檢測檔案數 
		Section = _T("System Parameter");
		KeyName = _T("Max Uncheck Test File Count");
		break;
	case SYSTEM_CONFIRM_COMPONENT_BARCODE_ENABLED://啟用零件條碼確認	
		Section = _T("System Parameter");
		KeyName = _T("Confirm Component Barcode Enabled");
		break;
	case SYSTEM_SAVE_JPEG_QUALITY://儲存JPEG的質量(001~100)
		Section = _T("System Parameter");
		KeyName = _T("Save JPEG Quality");
		break;
	case SYSTEM_COPY_HUGE_FILES_MODE://複製大量檔案模式
		Section = _T("System Parameter");
	#ifndef OFFLINE_VERSION
		KeyName = _T("Copy Huge Files Mode");
	#else
		KeyName = _T("Copy Huge Files Mode Offline");
	#endif//OFFLINE_VERSION
		break;

	case SYSTEM_OFFLINE_VERSION_MODE://離線版本模式
		Section = _T("Offline Parameter");
		KeyName = _T("Offline Version Mode");		
		break;
	case SYSTEM_USE_PROJECT_SYSTEM_PARAM_MODE://使用專案系統參數模式	
		Section = _T("Offline Parameter");
		KeyName = _T("Use Project System Parameter");	
		break;

	case SYSTEM_UI_WND_FONT_ADD_SIZE://UI視窗字型增加大小
		Section = _T("User Interface Parameter");
		KeyName = _T("UI Wnd Font Add Size");
		break;
	case SYSTEM_UI_DOCK_WND_SLIDE_STEPS://UI駐停視窗滑動步長	
		Section = _T("User Interface Parameter");
		KeyName = _T("UI Dock Wnd Slide Steps");
		break;	
	case SYSTEM_UI_ENABLE_PCB_OUT_BUTTON://UI啟用PCB出板按鈕
		Section = _T("User Interface Parameter");
		KeyName = _T("UI Enable PCB Out Button");
		break;

	case SYSTEM_RABBITMQ_SERVER_PORT://RabbitMQ伺服器Port
		Section = _T("RabbitMQ Parameter");
		KeyName = _T(" RabbitMQ Server Port");
		break;
	case SYSTEM_RABBITMQ_IP_ADDRESS://RabbitMQ通訊網址
		Section = _T("RabbitMQ Parameter");
		KeyName = _T(" RabbitMQ IP Address");
		break;
	case SYSTEM_RABBITMQ_USER_NAME://RabbitMQ登錄名稱
		Section = _T("RabbitMQ Parameter");
		KeyName = _T(" RabbitMQ User Name");
		break;
	case SYSTEM_RABBITMQ_PASSWORD://RabbitMQ登錄密碼
		Section = _T("RabbitMQ Parameter");
		KeyName = _T(" RabbitMQ Password");
		break;

	case SYSTEM_ITS_FILENAME://ITS軟體名稱
		Section = _T("System Parameter");
		KeyName = _T(" ITS Filename");
		break;
	case SYSTEM_ITS_COMMUNICATION_MODE://ITS通訊模式
		Section = _T("System Parameter");
		KeyName = _T(" ITS Communication Mode");
		break;
	case SYSTEM_ITS_SOCKET_IP_PORT://ITS網路Port
		Section = _T("System Parameter");
		KeyName = _T(" ITS Socket IP Port");
		break;
	case SYSTEM_ITS_SOCKET_IP_ADDRESS://ITS網路網址
		Section = _T("System Parameter");
		KeyName = _T(" ITS Socket IP Address");
		break;
	case SYSTEM_ITS_RABBITMQ_QUEUE_NAME_RECV://ITS訊息佇列接收名
		Section = _T("System Parameter");
		KeyName = _T(" ITS RabbitMQ Recv Queue Name");
		break;
	case SYSTEM_ITS_RABBITMQ_QUEUE_NAME_SEND://ITS訊息佇列傳送名
		Section = _T("System Parameter");
		KeyName = _T(" ITS RabbitMQ Send Queue Name");
		break;
	case SYSTEM_ITS_CONTACT_SOFTWARE://ITS-對接軟體
		Section = _T("System Parameter");
		KeyName = _T(" ITS Contact Software");
		break;
	case SYSTEM_ITS_COMMUNICATION_ENABLED://網路溝通啟用
		Section = _T("System Parameter");
		KeyName = _T(" Enable ITS Communication");
		break;
	case SYSTEM_ITS_COMMUNICATION_TIMEOUT_MS://與ITS溝通逾時(ms)	
		Section = _T("System Parameter");
		KeyName = _T(" ITS Communication Timeout ms");
		break;	
	case SYSTEM_ITS_FILE_FOLDER_SEND://ITS檔案資料夾-傳送
		Section = _T("System Parameter");
		KeyName = _T("ITS File Checker Send Folder");
		break;
	case SYSTEM_ITS_FILE_FOLDER_RECV://ITS檔案資料夾-接收
		Section = _T("System Parameter");
		KeyName = _T("ITS File Checker Recv Folder");
		break;
	case SYSTEM_ITS_FILE_BACKUP_ENABLED://ITS檔案資料夾-備份	
		Section = _T("System Parameter");
		KeyName = _T(" Enable ITS File Backup");
		break;
	case SYSTEM_ITS_FILE_USE_SYNC_FILE_ENABLED://ITS檔案資料夾-同步檔案
		Section = _T("System Parameter");
		KeyName = _T(" Enable ITS File Sync File");
		break;	
	case SYSTEM_ITS_SET_SECS_GEM_ENABLED://ITS使用設定SECS/GEM
		Section = _T("System Parameter");
		KeyName = _T("Enable ITS Set SECS/GEM");
		break;
	case SYSTEM_ITS_SECS_GEM_REMOTE_LOCAL: //ITS使用SECS/GEM-Remote Local
		Section = _T("System Parameter");		
		KeyName = _T("Enable ITS SECS/GEM Remote Local");
		break;
	case SYSTEM_ITS_SET_SYSTEM_PARAM_ENABLED://ITS使用設定系統參數
		Section = _T("System Parameter");
		KeyName = _T("Enable ITS Set System Param");
		break;
	case SYSTEM_ITS_SET_PROJECT_PARAM_ENABLED://ITS使用設定專案參數
		Section = _T("System Parameter");
		KeyName = _T("Enable ITS Set Project Param");
		break;
	case SYSTEM_ITS_SET_MACHINE_STATUS_ENABLED://ITS使用設定機台狀態
		Section = _T("System Parameter");
		KeyName = _T("Enable ITS Set Machine Status");
		break;
	case SYSTEM_ITS_SET_APP_OPEN_CLOSE_ENABLED://ITS使用設定軟體開關
		Section = _T("System Parameter");
		KeyName = _T("Enable ITS Set App Open Close");
		break;
	case SYSTEM_ITS_SET_PROCESS_ID_ENABLED://ITS使用設定程序編號
		Section = _T("System Parameter");
		KeyName = _T("Enable ITS Set Process ID");
		break;
	case SYSTEM_ITS_SET_USER_LOGIN_OUT_ENABLED://ITS使用設定使用者登入登出
		Section = _T("System Parameter");
		KeyName = _T("Enable ITS Set User Login-out");
		break;	
	case SYSTEM_PROG_DEFAULT_SPACE_TO_GRAY_RATIO_MODE://專案預設高度轉灰階比例 
		Section = _T("Project Parameter");
		KeyName = _T("Space To Gray Ratio Mode");
		break;
	case SYSTEM_PROG_DEFAULT_SPACE_BASE_PLANE_INDEX://專案預設空間基準面編號 
		Section = _T("Project Parameter");
		KeyName = _T("Space Base Plane Index");
		break;
	case SYSTEM_PROG_DEFAULT_SPACE_NOISE_FILTER_INDEX://專案預設空間雜訊過濾編號
		Section = _T("Project Parameter");
		KeyName = _T("Space Noise Filter Index");
		break;
	case SYSTEM_PROG_DEFAULT_ENABLE_CONVEYER_PRE_RUN://專案預設軌道提前運轉功能
		Section = _T("Project Parameter");
		KeyName = _T("Enable Conveyer Pre-Run");
		break;
	case SYSTEM_PROG_DEFAULT_FD_NG_HANDLE_MODE://專案預設定位點異常處理模式
		Section = _T("Project Parameter");
		KeyName = _T("Fiducial NG Handle Mode");
		break;
	case SYSTEM_PROG_DEFAULT_BOARD_FD_GRAB_MODE://專案預設單板定位點取像模式
		Section = _T("Project Parameter");
		KeyName = _T("Board Fd Grab Mode");
		break;
	case SYSTEM_PROG_DEFAULT_BDEFECT_HANDLE_MODE://專案預設檢出異常處理模式
		Section = _T("Project Parameter");
		KeyName = _T("Defect Product Handle Mode");
		break;
	case SYSTEM_PROG_DEFAULT_PCB_OUT_MODE://專案預設PCB出板模式
		Section = _T("Project Parameter");
		KeyName = _T("PCB Out Mode");
		break;
	case SYSTEM_PROG_DEFAULT_PROJECT_SAVE_TEST_MAP://專案預設儲存檢測底圖模式
		Section = _T("Project Parameter");
		KeyName = _T("Project Save Test Map");
		break;
	case SYSTEM_PROG_DEFAULT_PROJECT_LINK_SERVER_MODE://專案預設連線伺服器模式	
		Section = _T("Project Parameter");
		KeyName = _T("Project Link Server Mode");
		break;
	case SYSTEM_PROG_DEFAULT_SAVE_OFFLINE_IMAGE_FILES://專案預設儲存離線圖檔
		Section = _T("Project Parameter");
		KeyName = _T("Project Save Offline Image Files");
		break;
	case SYSTEM_PROG_DEFAULT_BARCODE_VERIFY_MODE://專案預設條碼驗證模式
		Section = _T("Project Parameter");
		KeyName = _T("Barcode Verify Mode");
		break;
	case SYSTEM_PROG_DEFAULT_BARCODE_RETRIEVE_MODE://專案預設條碼查詢模式
		Section = _T("Project Parameter");
		KeyName = _T("Barcode Retrieve Mode");
		break;

	case SYSTEM_OPERATE_LEVEL_PROJECT_OPEN://操作等級-專案開啟
		Section = _T("Operate Level");
		KeyName = _T("Operate Level Project Open");
		break;
	case SYSTEM_OPERATE_LEVEL_PROJECT_SAVE://操作等級-專案儲存
		Section = _T("Operate Level");
		KeyName = _T("Operate Level Project Save");
		break;
	case SYSTEM_OPERATE_LEVEL_PROJECT_PARAM://操作等級-專案參數
		Section = _T("Operate Level");
		KeyName = _T("Operate Level Project Param");
		break;
	case SYSTEM_OPERATE_LEVEL_ONLINE_RUN://操作等級-線上運行
		Section = _T("Operate Level");
		KeyName = _T("Operate Level Online Run");
		break;
	case SYSTEM_OPERATE_LEVEL_ONLINE_BYPASS://操作等級-線上直通
		Section = _T("Operate Level");
		KeyName = _T("Operate Level Online Bypass");
		break;
	case SYSTEM_OPERATE_LEVEL_ONLINE_STOP://操作等級-線上停止
		Section = _T("Operate Level");
		KeyName = _T("Operate Level Online Stop");
		break;
	case SYSTEM_OPERATE_LEVEL_ONLINE_SAVE_IMAGE://操作等級-線上存圖
		Section = _T("Operate Level");
		KeyName = _T("Operate Level Online Save Image");
		break;
	case SYSTEM_OPERATE_LEVEL_ONLINE_UNLOCK://操作等級-線上解鎖
		Section = _T("Operate Level");
		KeyName = _T("Operate Level Online Unlock");
		break;	
	case SYSTEM_OPERATE_LEVEL_EDIT_FUNC_ADD://操作等級-編輯功能-新增
		Section = _T("Operate Level");
		KeyName = _T("Operate Level Edit Func Add");
		break;
	case SYSTEM_OPERATE_LEVEL_EDIT_FUNC_DEL://操作等級-編輯功能-刪除
		Section = _T("Operate Level");
		KeyName = _T("Operate Level Edit Func Delete");
		break;
	case SYSTEM_OPERATE_LEVEL_EDIT_FUNC_BYPASS://操作等級-編輯功能-不檢測
		Section = _T("Operate Level");
		KeyName = _T("Operate Level Edit Func Bypass");
		break;	
	case SYSTEM_OPERATE_LEVEL_MES_CTRL_STATE_OFFLINE://操作等級-MES相關-控制狀態-離線
		Section = _T("Operate Level");
		KeyName = _T("Operate Level MES Comm Ctrl State Offline");
		break;
	case SYSTEM_OPERATE_LEVEL_MES_CTRL_STATE_LOCAL://操作等級-MES相關-控制狀態-本地上線
		Section = _T("Operate Level");
		KeyName = _T("Operate Level MES Comm Ctrl State Local");
		break;
	case SYSTEM_OPERATE_LEVEL_MES_CTRL_STATE_REMOTE://操作等級-MES相關-控制狀態-遠端上線
		Section = _T("Operate Level");
		KeyName = _T("Operate Level MES Comm Ctrl State Remote");
		break;
	case SYSTEM_OPERATE_LEVEL_MES_SHOW_CONTEXT://操作等級-MES相關-顯示內容
		Section = _T("Operate Level");
		KeyName = _T("Operate Level MES Comm Show Context");
		break;

	case SYSTEM_M2M_NPM_BARCODE_ENABLE://啟用NPM-條碼
		Section = _T("M2M NPM Parameter");
		KeyName = _T("M2M NPM Barcode Enable");
		break;
	case SYSTEM_M2M_NPM_APC_FF1_ENABLE://啟用NPM APC-FF1
		Section = _T("M2M NPM Parameter");
		KeyName = _T("M2M NPM APC FF1 Enable");
		break;
	case SYSTEM_M2M_NPM_APC_FF2_ENABLE://啟用NPM APC-FF2
		Section = _T("M2M NPM Parameter");
		KeyName = _T("M2M NPM APC FF2 Enable");
		break;
	case SYSTEM_M2M_NPM_APC_MFB_ENABLE://啟用NPM APC-MFB
		Section = _T("M2M NPM Parameter");
		KeyName = _T("M2M NPM APC MFB Enable");
		break;
	case SYSTEM_M2M_NPM_LANE_NAME_LA://NPM軌道名稱-A軌
		Section = _T("M2M NPM Parameter");
		KeyName = _T("M2M NPM Lane Name LA");
		break;
	case SYSTEM_M2M_NPM_LANE_NAME_LB://NPM軌道名稱-B軌
		Section = _T("M2M NPM Parameter");
		KeyName = _T("M2M NPM Lane Name LB");
		break;
	case SYSTEM_M2M_NPM_INPUT_SHARE_FOLDER_LA://NPM-輸入共享資料夾-A軌
		Section = _T("M2M NPM Parameter");
		KeyName = _T("M2M NPM Input Share Folder LA");
		break;
	case SYSTEM_M2M_NPM_INPUT_SHARE_FOLDER_LB://NPM-輸入共享資料夾-B軌
		Section = _T("M2M NPM Parameter");
		KeyName = _T("M2M NPM Input Share Folder LB");
		break;
	case SYSTEM_M2M_NPM_OUTPUT_SHARE_FOLDER_LA://NPM-輸出共享資料夾-A軌
		Section = _T("M2M NPM Parameter");
		KeyName = _T("M2M NPM Output Share Folder LA");
		break;
	case SYSTEM_M2M_NPM_OUTPUT_SHARE_FOLDER_LB://NPM-輸出共享資料夾-B軌
		Section = _T("M2M NPM Parameter");
		KeyName = _T("M2M NPM Output Share Folder LB");
		break;

	case SYSTEM_M2M_HASI_ENABLE://啟用M2M HAS I (Hanwha AOI Solution MAOI_SAOI)
		Section = _T("M2M HAS I Parameter");
		KeyName = _T("M2M HAS I Enable");
		break;
	case SYSTEM_M2M_HASI_SHARE_FOLDER://HAS I 共享資料夾
		Section = _T("M2M HAS I Parameter");
		KeyName = _T("M2M HAS I Share Folder Hanwha");
		break;
	case SYSTEM_M2M_HASI_SERIALFILE_ENABLE://啟用 HAS I - SerialFile
		Section = _T("M2M HAS I Parameter");
		KeyName = _T("M2M HAS I Serial file Enable");
		break;
	case SYSTEM_M2M_HASI_SERIALFILE_QUEUESIZE://HAS I - SerialFile - Size of Serial Queue
		Section = _T("M2M HAS I Parameter");
		KeyName = _T("M2M HAS I Serial file Queue Size");
		break;
	case SYSTEM_M2M_HASI_SERIALFILE_DWELLTIME://等待資料的延遲時間-ms
		Section = _T("M2M HAS I Parameter");
		KeyName = _T("M2M HAS I Serialfile Dwell Time");
		break;
	case SYSTEM_M2M_HASI_SPIOFFSETFILE_ENABLE://啟用 HAS I - SPIOffsetFile
		Section = _T("M2M HAS I Parameter");
		KeyName = _T("M2M HAS I SPIOffsetFile Enable");
		break;
	case SYSTEM_M2M_HASI_PNP_ENABLE://啟用 HAS I - PNP
		Section = _T("M2M HAS I Parameter");
		KeyName = _T("M2M HAS I PnP Hanwha Enable");
		break;
	case SYSTEM_M2M_HASI_AOI_STAGE://產線上 AOI 的檢查階段
		Section = _T("M2M HAS I Parameter");
		KeyName = _T("M2M HAS I AOI Stage");
		break;
	case SYSTEM_M2M_HASI_STATE_MODE://產線上 AOI 的檢查階段
		Section = _T("M2M HAS I Parameter");
		KeyName = _T("M2M HAS I State mode");
		break;

	case SYSTEM_AI_MODEL_SERVER_ENABLE://AI模型伺服器啟用
		Section = _T("AI Model Parameter");
		KeyName = _T("AI Model Server Enable");
		break;
	case SYSTEM_AI_MODEL_SERVER_TIMEOUT://AI模型伺服器逾時
		Section = _T("AI Model Parameter");
		KeyName = _T("AI Model Server Timeout");
		break;
	case SYSTEM_AI_MODEL_SERVER_FILENAME://AI模型伺服器檔名
		Section = _T("AI Model Parameter");
		KeyName = _T("AI Model Server Filename");
		break;
	case SYSTEM_AI_MODEL_FILE_FOLDER_SEND://AI模型檔案資料夾-傳送
		Section = _T("AI Model Parameter");
		KeyName = _T("AI Model File Folder Send");
		break;
	case SYSTEM_AI_MODEL_FILE_FOLDER_RECV://AI模型檔案資料夾-接收
		Section = _T("AI Model Parameter");
		KeyName = _T("AI Model File Folder Recv");
		break;
	case SYSTEM_AI_MODEL_LABEL_MIN_CLUSTER_DISTANCE://AI模型分類最小叢集距離-um
		Section = _T("AI Model Parameter");
		KeyName = _T("AI Model Label Min Cluster Distance");
		break;

	case SYSTEM_EXTERNAL_COPY_FILE_ENABLED://外部複製檔案啟用
		Section = _T("System Parameter");
		KeyName = _T("External Copy File Enable");
		break;
	case SYSTEM_EXTERNAL_COPY_FILE_APP_NAME://外部複製檔案軟體名稱
		Section = _T("System Parameter");
		KeyName = _T("External Copy File App Name");
		break;
	case SYSTEM_EXTERNAL_COPY_FILE_SEND_FOLDER://外部複製檔案輸出資料夾
		Section = _T("System Parameter");
		KeyName = _T("External Copy File Send Folder");
		break;

	case SYSTEM_CPK_CHART_ENABLED://Cpk圖表啟用
		Section = _T("CPK Chart");
		KeyName = _T("CPK Char Enabled");
		break;

	case SYSTEM_WND_ROTATION_FOLLOWED://檢測框跟隨旋轉
		Section = _T("Wnd Rotation Followed");
		KeyName = _T("Wnd Rotation Followed Enabled");
		break;

	default:
		IsOK = false;
		Section = _T("System Parameter");
		KeyName = _T("AAAAAAAAAAAAAAa");
		Err.Format(_T("Error, No System Param Define [%d]"), ParamID);
		JetAPI::ShowMessageBox(Err);
		break;
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::SetSystemParameterStringByID(SYSTEM_PARAM_ID ParamID, TSystemParameter &SysParam, LPCTSTR String)//設定系統參數
{
	CString Err;	
	int     tempI=0;
	double  tempD=0;
	bool IsOK = true;	
	switch ( ParamID )
	{
	case SYSTEM_AOI_FOLDER_HOST://系統資料夾
		SysParam.m_AOIDirectory = String;
		break;
	case SYSTEM_AOI_FOLDER_TEMP://暫存的資料夾		
		SysParam.m_AOIBaseTempDirectory = String;
		break;
	case SYSTEM_AOI_FOLDER_LOG://系統訊息資料夾		
		SysParam.m_AOILogDirectory = String;
		break;
	case SYSTEM_AOI_FOLDER_PROJECT://專案資料夾	
		SysParam.m_AOIProjectFolder = String;		
		break;
	case SYSTEM_AOI_FOLDER_RESULT://結果資料夾
		SysParam.m_AOIResultFolder = String;		
		break;	
	case SYSTEM_AOI_FOLDER_SERVER://伺服器資料夾
		SysParam.m_AOIServerFolder = String;		
		break;
	case SYSTEM_AOI_FOLDER_STATIC_DATA:
		SysParam.m_AOIStaticDataFolder = String;
		break;
	case SYSTEM_AOI_FOLDER_TEXT_REPORT://文字報告資料夾
		SysParam.m_AOITextReportFolder = String;
		break;
	case SYSTEM_AOI_FOLDER_ONLINE_TUNING://線上調機資料夾
		SysParam.m_OnlineTuningFolder = String;		
		break;
	case SYSTEM_AOI_FOLDER_ONLINE_BARCODE://線上條碼資料夾
		SysParam.m_OnlineBarcodeFolder = String;		
		break;
	case SYSTEM_AOI_FOLDER_ONLINE_OFFLINE://線上離線編程資料夾	
		SysParam.m_OnlineOfflineFolder = String;		
		break;
	case SYSTEM_AOI_FOLDER_PROJECT_TEST_MAP://專案檢測底圖資料夾
		SysParam.m_ProjectTestMapFolder = String;
		break;
	case SYSTEM_AOI_FOLDER_PROJECT_TEST_MAP_TEMP://專案檢測底圖資料夾-暫存
		SysParam.m_ProjectTestMapFolderTemp = String;
		break;
	case SYSTEM_AOI_FOLDER_PROJECT_TEST_MAP_BACKUP://專案檢測底圖資料夾-備份
		SysParam.m_ProjectTestMapFolderBackup = String;
		break;
	case SYSTEM_AOI_FOLDER_PROJECT_RAW://專案原圖資料夾
		SysParam.m_ProjectRawFolder = String;
		break;
	case SYSTEM_AOI_FOLDER_PROJECT_DEBUG://專案除錯資料夾		
		SysParam.m_ProjectDebugFolder = String;
		break;
	case SYSTEM_AOI_FOLDER_MONITOR_STATUS://監控狀態資料夾
		SysParam.m_MonitorStatusFolder = String;
		break;
	case SYSTEM_AOI_FOLDER_CUSTOMER_LOG://客戶訊息資料夾
		SysParam.m_CustomerLogFolder = String;
		break;
	case SYSTEM_AOI_FOLDER_CUSTOMER_REPORT://客戶報告資料夾			
		SysParam.m_CustomerReportFolder = String;
		break;
	case SYSTEM_AOI_FOLDER_BARCODE_FILE_LA://條碼檔案資料夾-A軌
		SysParam.m_BarcodeFileFolder_LA = String;
		break;
	case SYSTEM_AOI_FOLDER_BARCODE_FILE_LB://條碼檔案資料夾-B軌	
		SysParam.m_BarcodeFileFolder_LB = String;
		break;		
	case SYSTEM_AOI_FOLDER_AI_FILE_EXPORT://AI檔案輸出資料夾
		SysParam.m_AIFileExportFolder = String;
		break;
	case SYSTEM_AOI_FOLDER_AI_IMAGE_EXPORT://AI影像輸出資料夾
		SysParam.m_AIImageExportFolder = String;
		break;
	case SYSTEM_AOI_FOLDER_TEST_TRACK://檢測追蹤資料夾
		SysParam.m_AOITestTrackFolder = String;
		break;
	case SYSTEM_AOI_LIBRARY_HOST://資料庫-本機
		SysParam.m_LibraryHost = String;		
		break;
	case SYSTEM_AOI_LIBRARY_REMOTE://資料庫-遠端
		SysParam.m_LibraryRemote = String;		
		break;	
	case SYSTEM_IP_HOST_COMPUTER://本機網路網址	
		SysParam.m_IPHostComputer = String;
		break;
	case SYSTEM_MACHINE_LOCATION://本機廠區		
		SysParam.m_MachineLocation = String;
		break;
	case SYSTEM_MACHINE_BUILDING://本機棟別		
		SysParam.m_MachineBuilding = String;
		break;
	case SYSTEM_MACHINE_FLOOR://本機樓層	
		SysParam.m_MachineFloor = String;
		break;
	case SYSTEM_MACHINE_ROOM://本機車間
		SysParam.m_MachineRoom = String;
		break;
	case SYSTEM_MACHINE_LINE://本機線別
		SysParam.m_MachineLine = String;
		break;
	case SYSTEM_MACHINE_LINE_LB://本機線別-B軌
		SysParam.m_MachineLine_LB = String;
		break;
	case SYSTEM_MACHINE_STATION://本機站別		
		SysParam.m_MachineStation = String;
		break;
	case SYSTEM_MACHINE_STATION_LB://本機站別-B軌
		SysParam.m_MachineStation_LB = String;
		break;
	case SYSTEM_MACHINE_SN://本機序號
		SysParam.m_MachineSN = String;
		break;
	case SYSTEM_MACHINE_NAME://機台型號
		SysParam.m_MachineName = String;
		break;		
	case SYSTEM_MACHINE_VENDOR://機台廠商
		SysParam.m_MachineVendor = String;
		break;
	case SYSTEM_MACHINE_ALIAS://機台別名
		SysParam.m_MachineAlias = String;
		break;
	case SYSTEM_MACHINE_MES_NAME://MES登入名稱
		SysParam.m_MachineMES_Name = String;
		break;
	case SYSTEM_MACHINE_MES_PASSWORD://MES登入密碼
		SysParam.m_MachineMES_Password = String;
		break;
	case SYSTEM_MACHINE_MES_DEVICE://MES登入裝置
		SysParam.m_MachineMES_Device = String;
		break;
	case SYSTEM_MACHINE_MES_DEVICE_2://MES登入裝置-2
		SysParam.m_MachineMES_Device2 = String;
		break;
	case SYSTEM_MACHINE_MES_CODE_NAME://MES-裝置代號
		SysParam.m_MachineMES_CodeName = String;
		break;

	case SYSTEM_AOI_CUSTOMER_ID://客戶編號
		tempI = ::_ttoi(String); 
		switch ( tempI )
		{
		case AOI_CUSTOMER_ID_PEGATRON_TWN:
		case AOI_CUSTOMER_ID_KINPO_YUEYANG:
		case AOI_CUSTOMER_ID_FOXCONN_LONGHUA:		
			SysParam.m_AOICustomerID = (AOI_CUSTOMER_ID)(tempI);;
			break;
		default:
		case AOI_CUSTOMER_ID_JET_TWN:
			SysParam.m_AOICustomerID = AOI_CUSTOMER_ID_JET_TWN;
			break;
		}
		break;		
	case SYSTEM_CPU_MAX_COUNT_USED://Cpu最多使用數量
		tempI = ::_ttoi(String); 
		if ( tempI >= 0 )		
		{	SysParam.m_CpuMaxCountUsed = tempI; }		
		break;
	case SYSTEM_THREAD_CNT_SLICE_FILL://多執行緒數量-畫面填圖
		tempI = ::_ttoi(String); 
		if ( tempI < 0 ) 
		{	SysParam.m_MTCount_SliceFill = 1; }
		else if ( tempI > MAX_THREAD_COUNT_SLICE_FILL )
		{	SysParam.m_MTCount_SliceFill = MAX_THREAD_COUNT_SLICE_FILL; }
		else
		{	SysParam.m_MTCount_SliceFill = (tempI); }
		break;
	case SYSTEM_THREAD_CNT_FRAME_MERGE://多執行緒數量-影像合併
		tempI = ::_ttoi(String); 
		if ( tempI < 0 ) 
		{	SysParam.m_MTCount_FrameMerge = 1; }
		else if ( tempI > MAX_THREAD_COUNT_FRAME_MERGE )
		{	SysParam.m_MTCount_FrameMerge = MAX_THREAD_COUNT_FRAME_MERGE; }
		else
		{	SysParam.m_MTCount_FrameMerge = (tempI); }
		break;
	case SYSTEM_THREAD_CNT_FIELD_MERGE://多執行緒數量-區域合併
		tempI = ::_ttoi(String); 
		if ( tempI < 0 ) 
		{	SysParam.m_MTCount_FieldMerge = 1; }
		else if ( tempI > MAX_THREAD_COUNT_FIELD_MERGE )
		{	SysParam.m_MTCount_FieldMerge = MAX_THREAD_COUNT_FIELD_MERGE; }
		else
		{	SysParam.m_MTCount_FieldMerge = (tempI); }
		break;
	case SYSTEM_THREAD_CNT_FRAME_LOAD://多執行緒數量-影像載入
		tempI = ::_ttoi(String); 
		if ( tempI < 0 ) 
		{	SysParam.m_MTCount_FrameLoad = 1; }
		else if ( tempI > MAX_THREAD_COUNT_FRAME_LOAD )
		{	SysParam.m_MTCount_FrameLoad = MAX_THREAD_COUNT_FRAME_LOAD; }
		else
		{	SysParam.m_MTCount_FrameLoad = (tempI); }
		break;
	case SYSTEM_THREAD_CNT_REGION_CALC://多執行緒數量-區域計算
		tempI = ::_ttoi(String); 
		if ( tempI < 0 ) 
		{	SysParam.m_MTCount_RegionCalc = 1; }
		else if ( tempI > MAX_THREAD_COUNT_REGION_CALC )
		{	SysParam.m_MTCount_RegionCalc = MAX_THREAD_COUNT_REGION_CALC; }
		else
		{	SysParam.m_MTCount_RegionCalc = (tempI); }
		break;
	case SYSTEM_THREAD_CNT_PROC_IDLE://多執行緒數量-閒置核心	
		tempI = ::_ttoi(String); 
		if ( tempI < 0 ) 
		{	SysParam.m_MTCount_ProcIdle = 0; }
		else if ( tempI > MAX_THREAD_COUNT_PROC_IDLE )
		{	SysParam.m_MTCount_ProcIdle = MAX_THREAD_COUNT_PROC_IDLE; }
		else
		{	SysParam.m_MTCount_ProcIdle = (tempI); }
		break;
	case SYSTEM_THREAD_CNT_GRAB_IDLE://多執行緒數量-取像閒置核心	
		tempI = ::_ttoi(String); 
		if ( tempI < 0 ) 
		{	SysParam.m_MTCount_GrabIdle = 0; }
		else if ( tempI > MAX_THREAD_COUNT_GRAB_IDLE )
		{	SysParam.m_MTCount_GrabIdle = MAX_THREAD_COUNT_GRAB_IDLE; }
		else
		{	SysParam.m_MTCount_GrabIdle = (tempI); }
		break;
	case SYSTEM_OPEN_MP_CNT_GENERAL://OpenMP核心數量-一般操作
		tempI = ::_ttoi(String); 
		if ( tempI<0 || tempI>MAX_OPEN_MP_COUNT ) 
		{	SysParam.m_OpenMPCount_General = 0; }
		else 
		{	SysParam.m_OpenMPCount_General = (tempI); }
		break;	
	case SYSTEM_OPEN_MP_CNT_INSPECTION://OpenMP核心數量-檢測中	
		tempI = ::_ttoi(String); 
		if ( tempI<0 || tempI>MAX_OPEN_MP_COUNT ) 
		{	SysParam.m_OpenMPCount_Inspection = 0; }
		else
		{	SysParam.m_OpenMPCount_Inspection = (tempI); }
		break;
	case SYSTEM_OPEN_MP_CHK_SIZE_INSPECTION://OpenMP確認尺寸-檢測中
		tempI = ::_ttoi(String); 
		if ( tempI < 10000 ) 
		{	SysParam.m_OpenMPCheckSize_Inspection = 10000; }
		else
		{	SysParam.m_OpenMPCheckSize_Inspection = (tempI); }
		break;
	case SYSTEM_SAVE_MSG_MOVING_TIME://是否移動時間狀態訊息
		SysParam.m_SaveMovingTimeMessage = ::_ttoi(String);
		break;
	case SYSTEM_BACKUP_MSG_MOVING_TIME://是否備份移動時間狀態訊息	
		SysParam.m_BackupMovingTimeMessage = ::_ttoi(String);
		break;
	case SYSTEM_SAVE_MSG_CURRENT_PROCESS://是否儲存現在狀態訊息
		SysParam.m_SaveCurrentProcessMessage = ::_ttoi(String);
		break;	
	case SYSTEM_BACKUP_MSG_CURRENT_PROCESS://是否備份現在狀態訊息
		SysParam.m_BackupCurrentProcessMessage = ::_ttoi(String);
		break;
	case SYSTEM_SAVE_MSG_MACHINE_MONITOR://是否儲存機台監控訊息
		SysParam.m_SaveMachineMonitorMessage = ::_ttoi(String);
		break;
	case SYSTEM_SAVE_MSG_MACHINE_MONITOR_WEB://是否儲存機台監控訊息-網頁用
		SysParam.m_SaveMachineMonitorMessageWeb = ::_ttoi(String);
		break;
	case SYSTEM_SAVE_CAMERA_ADD_RING_BUFFER_LOG://是否儲存相機增加循環資料訊息
		SysParam.m_SaveCameraAddRingBufferLog = ::_ttoi(String);
		break;
	case SYSTEM_SHOW_MEMORY_LEAK_MESSAGE://顯示記憶體未釋放訊息
		SysParam.m_ShowMemoryLeakMessage = ::_ttoi(String);
		break;		
	case SYSTEM_SEND_DEBUG_VIEW_STRING://是否送至DebugView視窗
		SysParam.m_SendDebugViewString = ::_ttoi(String);
		break;
	case SYSTEM_SAVE_SLICE_FILL_THREAD_LOG://是否儲存影像填滿執行緒訊息
		SysParam.m_SaveSliceFillThreadLog = ::_ttoi(String);
		break;
	case SYSTEM_SAVE_FRAME_MERGE_THREAD_LOG://是否儲存影像合併執行緒訊息
		SysParam.m_SaveFrameMergeThreadLog = ::_ttoi(String);
		break;
	case SYSTEM_SAVE_FIELD_MERGE_THREAD_LOG://是否儲存區域合併執行緒訊息
		SysParam.m_SaveFieldMergeThreadLog = ::_ttoi(String);
		break;
	case SYSTEM_SAVE_FRAME_LOAD_THREAD_LOG://是否儲存影像載入執行緒訊息
		SysParam.m_SaveFrameLoadThreadLog = ::_ttoi(String);
		break;
	case SYSTEM_SAVE_REGION_CALC_THREAD_LOG://是否儲存區域計算執行緒訊息
		SysParam.m_SaveRegionCalcThreadLog = ::_ttoi(String);
		break;
	case SYSTEM_SAVE_SQUENCE_THREAD_LOG://是否儲存系列執行緒訊息
		SysParam.m_SaveSequenceThreadLog = ::_ttoi(String);
		break;
	case SYSTEM_SAVE_FIELD_FRAME_RELEASE_THREAD_LOG://是否儲存視野影像釋放執行緒訊息
		SysParam.m_SaveFieldFrameReleaseThreadLog = ::_ttoi(String);
		break;
	case SYSTEM_SAVE_ONLINE_INSPECTION_THREAD_LOG://是否儲存線上檢測執行緒訊息	
		SysParam.m_SaveOnlineInspectionThreadLog = ::_ttoi(String);
		break;
	case SYSTEM_SAVE_REMOVE_FOLDER_THREAD_LOG://是否儲存刪除資料夾執行緒訊息
		SysParam.m_SaveRemoveFolderThreadLog = ::_ttoi(String);
		break;
	case SYSTEM_SAVE_CONVEYER_PRE_RUN_THREAD_LOG://是否儲存軌道自動運轉執行緒訊息
		SysParam.m_SaveConveyerPreRunThreadLog = ::_ttoi(String);
		break;
	case SYSTEM_SAVE_ITS_PROC_THREAD_LOG://是否儲存ITS運作執行緒訊息
		SysParam.m_SaveITSProcThreadLog = ::_ttoi(String);
		break;
	case SYSTEM_SAVE_LOAD_REPAIR_FILE_THREAD_LOG://是否儲存載入維修站檔案執行緒訊息
		SysParam.m_SaveLoadRepairFileThreadLog = ::_ttoi(String);
		break;
	case SYSTEM_SAVE_REPAIR_RESULT_SIGNAL_THREAD_LOG://是否儲存維修站訊息執行緒訊息
		SysParam.m_SaveRepairResultSignalThreadLog = ::_ttoi(String);
		break;
	case SYSTEM_SAVE_SIMPLE_JOB_THREAD_LOG://是否儲存簡易作業執行緒訊息
		SysParam.m_SaveSimpleJobThreadLog = ::_ttoi(String);
		break;
	case SYSTEM_SAVE_HASI_MONITOR_THREAD_LOG://是否儲存HASI執行緒訊息
		SysParam.m_SaveHASIMonitorThreadLog = ::_ttoi(String);
		break;
	case SYSTEM_SAVE_TEST_OBJECT_FINISH_LOG://是否儲存檢測物件結束訊息
		SysParam.m_SaveTestObjectFinishLog = ::_ttoi(String);
		break;
	case SYSTEM_SAVE_UI_DRAW_FUNC_LOG://是否儲存介面重繪函式訊息
		SysParam.m_SaveUIDrawFuncLog = ::_ttoi(String);
		break;	
	case SYSTEM_SAVE_USER_OPERATION_LOG://是否儲存使用者操作訊息
		SysParam.m_SaveUserOperationLog = ::_ttoi(String);
		break;
	case SYSTEM_SAVE_TEST_TRACK_FILE: //是否儲存檢測追蹤檔案
		SysParam.m_SaveTestTrackFile = ::_ttoi(String);
		break;
	case SYSTEM_SAVE_PROJECT_REPORT_TEXT://是否專案文字檔報告
		SysParam.m_SaveProjectReportText = ::_ttoi(String);
		break;
	case SYSTEM_SAVE_PROJECT_REPORT_TEXT_FILENAME_MODE://儲存專案文字檔報告檔名
		SysParam.m_SaveProjectReportTextFilenameMode = (SAVE_TEXT_FILENAME_MODE)(::_ttoi(String));
		break;
	case SYSTEM_SAVE_CUSTOMER_REPORT_FILE://是否儲存客戶報告模式
		tempI = ::_ttoi(String);
		switch ( tempI )
		{
		case FN_ENABLE:
		case FN_DISABLE:
			SysParam.m_SaveCustomerReportFile = tempI;
			break;
		}
		break;
	case SYSTEM_SAVE_PROJECT_SPC_FILE_MODE://儲存專案SPC檔案模式
		tempI = ::_ttoi(String);
		switch ( tempI )
		{
		case SAVE_SPC_FILE_JSON_VRS:
		case SAVE_SPC_FILE_JSON_RSM:
			SysParam.m_SaveProjectSpcFileMode = (SAVE_SPC_FILE_MODE)(tempI);
			break;
		}
		break;
	case SYSTEM_SAVE_PROJECT_SPC_LIBRARY_MODE://儲存專案SPC資料庫模式
		tempI = ::_ttoi(String);
		switch ( tempI )
		{		
		case SAVE_SPC_OTHER_FILE_AUTO:
		case SAVE_SPC_OTHER_FILE_ASK:
			SysParam.m_SaveProjectSpcLibraryMode = (SAVE_SPC_OTHER_FILE_MODE)(tempI);
			break;
		default:
		case SAVE_SPC_OTHER_FILE_OFF:
			SysParam.m_SaveProjectSpcLibraryMode = SAVE_SPC_OTHER_FILE_OFF;
			break;
		}
		break;		
	case SYSTEM_SAVE_PROJECT_REPORT_WND_READING://是否儲存專案檢測框數據檔案	
		SysParam.m_SaveProjectReportWndReading = ::_ttoi(String);
		break;		
	case SYSTEM_PRE_LOAD_PROJECT_OFFLINE_IMAGE://提前載入專案離線圖檔
		SysParam.m_PreLoadProjectOfflineImage = ::_ttoi(String);
		break;
	case SYSTEM_AUTO_RELEASE_FIELD_FRAME_BUFFER://自動釋放區域影像
		SysParam.m_AutoReleaseFieldFrameBuffer = ::_ttoi(String);
		break;
	case SYSTEM_AUTO_RELEASE_OFFLINE_FIELD_FRAME_BUFFER://自動釋放離線編程區域影像		
		SysParam.m_AutoReleaseOfflineFieldFrameBuffer = ::_ttoi(String);
		break;	
	case SYSTEM_MACHINE_MODEL_TYPE://設備機種樣式
		tempI = ::_ttoi(String);
		switch ( tempI )
		{
		case MACHINE_MODEL_6500:
		case MACHINE_MODEL_8000:
		case MACHINE_MODEL_7500_TBII:
			SysParam.m_MachineModelType = (MACHINE_MODEL_TYPE)(tempI);
			break;
		default:	IsOK = false;	break;
		}
		break;
	case SYSTEM_MACHINE_CAMERA_SIDE://設備相機方向
		tempI = ::_ttoi(String);
		switch ( tempI )
		{
		case MACHINE_CAMERA_TOP:
		case MACHINE_CAMERA_BOT:		
			SysParam.m_MachineCameraSide = (MACHINE_CAMERA_SIDE)(tempI);
			break;
		default:	IsOK = false;	break;
		}
		break;	
	case SYSTEM_MULTI_LANGUAGE_MODE://多國語系版本		
		tempI = ::_ttoi(String);
		switch ( tempI )
		{
		case MULTI_LANGUAGE_ENGLISH:
		case MULTI_LANGUAGE_CHINESE_TRAD:
		case MULTI_LANGUAGE_CHINESE_SIMP:
		case MULTI_LANGUAGE_LOCAL:
			SysParam.m_MultiLanguageMode = (MULTI_LANGUAGE_MODE)(tempI);
			break;
		default:	IsOK = false;	break;
		}
		break;	
	case SYSTEM_DC_STRECTCH_BLT_MODE://繪圖模式
		SysParam.m_StretchBltMode = ::_ttoi(String);
		break;
	case SYSTEM_CAMERA_DEBAYER_MODE://影像還元彩色模式
		tempI = ::_ttoi(String);
		switch ( tempI )
		{
		case IMAGE_DEBAYER_CAMERA_API:
		case IMAGE_DEBAYER_OPEN_CV:
		case IMAGE_DEBAYER_RAW_COLOR:
		case IMAGE_DEBAYER_RAW_MONO:
			SysParam.m_DebayerMode = (IMAGE_DEBAYER_MODE)(tempI);
			break;
		default:	IsOK = false;	break;
		}		
		break;	
	case SYSTEM_IMAGE_DISPLAY_MODE://影像顯示模式
		tempI = ::_ttoi(String);
		switch ( tempI )
		{
		case IMAGE_DISPLAY_GRAY:
		case IMAGE_DISPLAY_BAYER:
		case IMAGE_DISPLAY_COLOR:		
			SysParam.m_ImageDisplayMode = (IMAGE_DISPLAY_MODE)(tempI);
			break;
		default:	IsOK = false;	break;
		}
		break;
	case SYSTEM_IMAGE_DISPLAY_ENHANCE_MODE://影像顯示強化模式
		tempI = ::_ttoi(String);
		switch ( tempI )
		{
		case IMAGE_DISPLAY_ENHANCE_NONE:
		case IMAGE_DISPLAY_ENHANCE_GAIN:
		case IMAGE_DISPLAY_ENHANCE_GAMMA:
		case IMAGE_DISPLAY_ENHANCE_SHARPNESS_GAIN:
		case IMAGE_DISPLAY_ENHANCE_SHARPNESS_GAMMA:
		case IMAGE_DISPLAY_ENHANCE_LOCAL_GAMMA:
			SysParam.m_ImageDisplayEnhanceMode = (IMAGE_DISPLAY_ENHANCE_MODE)(tempI);
			break;
		default:	IsOK = false;	break;
		}
		break;
	case SYSTEM_IMAGE_DISPLAY_GAIN://影像顯示的Gain
		tempD = ::_tcstod(String, NULL);
		if ( tempD < 0.00 || tempD > 255.0 )
		{	IsOK = false;	}
		else
		{	SysParam.m_ImageDisplayGain = tempD; }
		break;
	case SYSTEM_IMAGE_DISPLAY_GAMMA://影像顯示的Gamma
		tempD = ::_tcstod(String, NULL);
		if ( tempD < 0.00 || tempD > 100 )
		{	IsOK = false;	}
		else
		{	SysParam.m_ImageDisplayGamma = tempD; }
		break;		
	case SYSTEM_IMAGE_DISPLAY_SHARPNESS_RADIUS://影像顯示的銳利化的半徑
		tempI = ::_ttoi(String);
		if ( tempI < 0 ) 
		{	IsOK = false; }
		else
		{	SysParam.m_ImageDisplaySharpnessRadius = tempI; }
		break;
	case SYSTEM_IMAGE_DISPLAY_SHARPNESS_AMOUNT://影像顯示的銳利化的總量
		tempI = ::_ttoi(String);
		if ( tempI < 0 ) 
		{	IsOK = false; }
		else
		{	SysParam.m_ImageDisplaySharpnessAmount = tempI; }
		break;
	case SYSTEM_IMAGE_DISPLAY_SHARPNESS_THRESHOLD://影像顯示的銳利化的閥值
		tempI = ::_ttoi(String);
		if ( tempI<0 || tempI>255 ) 
		{	IsOK = false; }
		else
		{	SysParam.m_ImageDisplaySharpnessThreshold = tempI; }
		break;		
	case SYSTEM_IMAGE_DISPLAY_LOCAL_GAMMA_CALC_SIZE://影像顯示的局部Gamma的計算範圍
		tempI = ::_ttoi(String);
		if ( tempI<1 || tempI>4000 ) 
		{	IsOK = false; }
		else
		{	SysParam.m_ImageDisplayLocalGammaCalcSize = tempI; }
		break;
	case SYSTEM_IMAGE_DISPLAY_LOCAL_GAMMA_SCALE_VAL://影像顯示的局部Gamma的縮放比例
		tempD = ::_ttof(String);
		if ( tempD<0 ) 
		{	IsOK = false; }
		else
		{	SysParam.m_ImageDisplayLocalGammaScaleVal = tempD; }
		break;
	case SYSTEM_IMAGE_DISPLAY_MAX_ZOOM_SCALE://影像顯示最大縮放比例	
		SysParam.m_ImageDisplayMaxZoomScale = MAX(0.0, ::_ttof(String));
		break;		
	case SYSTEM_ONLINE_FORM_VIEW_MODE://線上檢測介面模式	
		tempI = ::_ttoi(String);
		switch ( tempI )
		{
		case ONLINE_FROMVIEW_ONE:
		case ONLINE_FROMVIEW_DUAL:
			SysParam.m_OnlineFormViewMode = (ONLINE_FROMVIEW_MODE)(tempI);
			break;
		}
		break;	
	case SYSTEM_CALC_FOCUS_SMOOTH_SIZE://計算焦距平滑尺寸
		tempI = ::_ttoi(String);
		if ( tempI<0 || tempI>255 ) 
		{	IsOK = false; }
		else
		{	SysParam.m_CalcFocusSmoothSize = tempI; }
		break;
	case SYSTEM_CALC_FOCUS_MODE://計算焦距模式
		tempI = ::_ttoi(String);
		switch ( tempI )
		{
		case CALC_FOCUS_AMPLITUDE:
		case CALC_FOCUS_VARIANCE:		
		case CALC_FOCUS_SUM_MODULES_DIFFERENCE:		
		case CALC_FOCUS_SQUARED_GRADIENT:		
		case CALC_FOCUS_TENENGRAD:
		case CALC_FOCUS_LAPLACIAN:
		case CALC_FOCUS_PIXEL_DIFFERENCE:
			SysParam.m_CalcFocusMode = (CALC_FOCUS_MODE)(tempI);
			break;
		default:	IsOK = false;	break;
		}
		break;		
	case SYSTEM_LIBRARY_MODE_MATCH://影像匹配函式庫樣式
		tempI = ::_ttoi(String);
		switch ( tempI )
		{
		case JET_MATCH_LIB_EVS:
		case JET_MATCH_LIB_MIM:		
			SysParam.m_MatchLibType = (JET_MATCH_LIB_TYPE)(tempI);
			break;
		default:	IsOK = false;	break;
		}
		break;
	case SYSTEM_HONEYWELL_SWIFT_DECODER_ENABLED://Honeywell SwiftDecoder啟用
		tempI = ::_ttoi(String);
		switch ( tempI )
		{
		case FN_ENABLE:
		case FN_DISABLE:		
			SysParam.m_HoneywellSwiftDecoderEnabled = (tempI);
			break;
		default:	IsOK = false;	break;
		}
		break;
	case SYSTEM_DONGLE_WARNING_REMAINING_DAYS://硬體鎖警告剩餘天數
		tempI = ::_ttoi(String);
		if ( tempI > 0 )
		{	SysParam.m_DongleWarningRemainingDays = tempI; }
		else
		{	IsOK = false; }
		break;
	case SYSTEM_DONGLE_WARNING_REMAINING_COUNT://硬體鎖警告剩餘次數
		tempI = ::_ttoi(String);
		if ( tempI > 0 )
		{	SysParam.m_DongleWarningRemainingCount = tempI; }
		else
		{	IsOK = false; }
		break;
	case SYSTEM_CONNECT_LAST_STATION_MODE://與上一站連線方式
		tempI = ::_ttoi(String);		
		switch ( tempI )
		{
		case LAST_STATION_LINE_MODE_2://前站兩線式, 提前送訊號
		case LAST_STATION_LINE_MODE_2_4://前站兩線式, 不提前送訊號
		case LAST_STATION_LINE_MODE_4://前站四線式
			SysParam.m_LastStationLineMode = tempI;
			break;
		default:	IsOK = false;	break;
		}
		break;	
	case SYSTEM_LANE_WORK_MODEL_LA://A軌道運轉模式
		tempI = ::_ttoi(String);
		switch ( tempI )
		{
		case LANE_WORK_DISABLE:
		case LANE_WORK_RUN:
		case LANE_WORK_BYPASS:
			SysParam.m_LaneWorkMode_LA = (LANE_WORK_MODE)(tempI);
			break;
		default:	IsOK = false;	break;
		}
		break;
	case SYSTEM_LANE_WORK_MODEL_LB://B軌道運轉模式
		tempI = ::_ttoi(String);
		switch ( tempI )
		{
		case LANE_WORK_DISABLE:
		case LANE_WORK_RUN:
		case LANE_WORK_BYPASS:
			SysParam.m_LaneWorkMode_LB = (LANE_WORK_MODE)(tempI);
			break;
		default:	IsOK = false;	break;
		}
		break;	
	case SYSTEM_MULTI_TOWER_LIGHT_MODE://多塔燈模式		
		tempI = ::_ttoi(String);	
		switch ( tempI )
		{
		case MULTI_TOWER_LIGHT_SINGLE:
		case MULTI_TOWER_LIGHT_DUAL:
			SysParam.m_MultiTowerLight = tempI;
			break;
		default:	IsOK = false;	break;
		}
		break;		
	case SYSTEM_CHECK_PCB_REMOVED_COUNT://確認PCB板移走次數
		tempI = ::_ttoi(String);	
		if ( tempI < 0 ) { SysParam.m_CheckPCBRemovedCount = 1; }
		else if ( tempI > 100 ) { SysParam.m_CheckPCBRemovedCount = 100; }
		else { SysParam.m_CheckPCBRemovedCount = tempI;  }
		break;
	case SYSTEM_NEXT_CONNECTED_BUFFER_TYPE://確認PCB下一站連接輸送帶樣式
		tempI = ::_ttoi(String);	
		switch ( tempI )
		{
		case CONNECTED_BUFFER_FIXED:
		case CONNECTED_BUFFER_MOVABLE:
			SysParam.m_NextConnectedBufferType = (CONNECTED_BUFFER_TYPE)(tempI);
			break;
		default:	IsOK = false;	break;
		}
		break;		
	case SYSTEM_MULTI_PROJECT_TEST_ORDER_MODE://多專案檢測次序模式		
		tempI = ::_ttoi(String);	
		switch ( tempI )
		{
		case MULTI_PROJECT_TEST_ORDER_BY_MARK:
		case MULTI_PROJECT_TEST_ORDER_BY_TURN:
		case MULTI_PROJECT_TEST_ORDER_ONE_CYCLE_A_B:
		case MULTI_PROJECT_TEST_ORDER_ONE_CYCLE_B_A:
			SysParam.m_MultiProjectTestOrderMode = (MULTI_PROJECT_TEST_ORDER_MODE)(tempI);
			break;
		default:	IsOK = false;	break;
		}
		break;
	case SYSTEM_ONLINE_INPUT_PROJECT_WORK_NUMBER://在線輸入-專案工單號碼
		tempI = ::_ttoi(String);	
		switch ( tempI )
		{
		case ONLINE_INPUT_DISABLE:
		case ONLINE_INPUT_BEFORE_SIGN_IN:		
			SysParam.m_OnlineInputProjectWorkNumber = (ONLINE_INPUT_TIMING)(tempI);
			break;
		default:	IsOK = false;	break;
		}
		break;
	case SYSTEM_ONLINE_AUTO_CALIBRATION_DLP_LED_COLOR://在線自動校正DLP-LED-顏色
		tempI = ::_ttoi(String);	
		switch ( tempI )
		{
		case DLP_LED_COLOR_RED:
		case DLP_LED_COLOR_GREEN:
		case DLP_LED_COLOR_BLUE:
		case DLP_LED_COLOR_WHITE:
			SysParam.m_OnlineAutoCalibrationDlpLedColor = tempI;
			break;
		default:	IsOK = false;	break;
		}
		break;
	case SYSTEM_ONLINE_AUTO_CALIBRATION_CAP_DELAY_TIME://在線自動校正上蓋延遲時間-ms
		tempI = ::_ttoi(String);	
		if ( tempI < 0 ) { IsOK = false; }
		else
		{	SysParam.m_OnlineAutoCalibrationCapDelayTime = (DWORD)(tempI);	}
		break;
	case SYSTEM_ONLINE_AUTO_CALIBRATION_MODE_XYZ_HOME://在線自動校正模式-XYZ歸零
		tempI = ::_ttoi(String);	
		switch ( tempI )
		{
		case FUNC_EXEC_AUTO:
		case FUNC_EXEC_ASK:
			SysParam.m_OnlineAutoCalibrationMode_XYZHome = (FUNC_EXEC_MODE)(tempI);
			break;
		default:
		case FUNC_EXEC_OFF:
			SysParam.m_OnlineAutoCalibrationMode_XYZHome = FUNC_EXEC_OFF;
			break;
		}		
		break;
	case SYSTEM_ONLINE_AUTO_CALIBRATION_PERIOD_XYZ_HOME://在線自動校正週期-XYZ歸零-小時
		tempI = ::_ttoi(String);	
		if ( tempI < 0 ) { tempI = 0; }
		SysParam.m_OnlineAutoCalibrationPeriod_XYZHome = tempI;
		break;
	case SYSTEM_ONLINE_AUTO_CALIBRATION_MODE_2D_CURRENT://在線自動校正模式-2D電流
		tempI = ::_ttoi(String);	
		switch ( tempI )
		{
		case FUNC_EXEC_AUTO:
		case FUNC_EXEC_ASK:
			SysParam.m_OnlineAutoCalibrationMode_2DCurrent = (FUNC_EXEC_MODE)(tempI);
			break;
		default:
		case FUNC_EXEC_OFF:
			SysParam.m_OnlineAutoCalibrationMode_2DCurrent = FUNC_EXEC_OFF;
			break;
		}
		break;
	case SYSTEM_ONLINE_AUTO_CALIBRATION_PERIOD_2D_CURRENT://在線自動校正週期-2D電流-小時
		tempI = ::_ttoi(String);	
		if ( tempI < 0 ) { tempI = 0; }
		SysParam.m_OnlineAutoCalibrationPeriod_2DCurrent = tempI;
		break;
	case SYSTEM_ONLINE_AUTO_CALIBRATION_PERIOD_3D_CURRENT://在線自動校正週期-3D電流-小時
		tempI = ::_ttoi(String);	
		if ( tempI < 0 ) { tempI = 0; }
		SysParam.m_OnlineAutoCalibrationPeriod_3DCurrent = tempI;
		break;
	case SYSTEM_ONLINE_AUTO_CALIBRATION_MODE_3D_ZERO_PLANE://在線自動校正模式-3D相平面
		tempI = ::_ttoi(String);	
		switch ( tempI )
		{
		case FUNC_EXEC_AUTO:
		case FUNC_EXEC_ASK:
			SysParam.m_OnlineAutoCalibrationMode_3DZeroPlane = (FUNC_EXEC_MODE)(tempI);
			break;
		default:
		case FUNC_EXEC_OFF:
			SysParam.m_OnlineAutoCalibrationMode_3DZeroPlane = FUNC_EXEC_OFF;
			break;
		}
		break;
	case SYSTEM_ONLINE_AUTO_CALIBRATION_PERIOD_3D_ZERO_PLANE://在線自動校正週期-3D相平面-小時
		tempI = ::_ttoi(String);	
		if ( tempI < 0 ) { tempI = 0; }
		SysParam.m_OnlineAutoCalibrationPeriod_3DZeroPlane = tempI;
		break;
	case SYSTEM_ONLINE_AUTO_CALIBRATION_PERIOD_3D_HEIGHT_FACTOR://在線自動校正週期-3D高度比例-小時
		tempI = ::_ttoi(String);	
		if ( tempI < 0 ) { tempI = 0; }
		SysParam.m_OnlineAutoCalibrationPeriod_3DHeightFactor = tempI;
		break;
	case SYSTEM_ONLINE_AUTO_STOP_BY_IDLE_TIME://在線自動停機-閒置時間-分鐘
		tempI = ::_ttoi(String);	
		if ( tempI < 0 ) { tempI = 0; }
		SysParam.m_OnlineAutoStopByIdleTime = tempI;
		break;
	case SYSTEM_ONLINE_AUTO_STOP_BY_SPEC_TIME_1://在線自動停機-特定時間-時時分分秒秒
		SysParam.m_OnlineAutoStopBySpecTime1 = ::_ttoi64(String);
		break;
	case SYSTEM_ONLINE_AUTO_STOP_BY_SPEC_TIME_2://在線自動停機-特定時間-時時分分秒秒
		SysParam.m_OnlineAutoStopBySpecTime2 = ::_ttoi64(String);
		break;
	case SYSTEM_ONLINE_AUTO_STOP_BY_SPEC_TIME_3://在線自動停機-特定時間-時時分分秒秒
		SysParam.m_OnlineAutoStopBySpecTime3 = ::_ttoi64(String);
		break;
	case SYSTEM_AUTO_SWITCH_TO_ONILINEVIEW_TIME://自動切換線上畫面-秒	
		tempI = ::_ttoi(String); 
		if ( tempI < 0 ) { tempI = 0; }
		SysParam.m_AutoSwitchToOnlineViewTime = tempI;
		break;
	case SYSTEM_ONLINE_SHOW_PROJECT_TEST_MAP://在線顯示專案檢測底圖
		tempI = ::_ttoi(String);		
		switch ( tempI )
		{
		case FN_ENABLE:	SysParam.m_OnlineShowProjectTestMap = (tempI);	break;
		default:
		case FN_DISABLE: SysParam.m_OnlineShowProjectTestMap = FN_DISABLE; break;
		}
		break;	
	case SYSTEM_AUTO_SWITCH_TO_ONILINE_REMOTE_CTRL_TIME://自動切換線上遠端控制時間-毫秒
		tempI = ::_ttoi(String); 
		if ( tempI < 0 ) { tempI = 0; }
		SysParam.m_AutoSwitchToOnlineRemoteCtrlTime = tempI;
		break;
	case SYSTEM_AUTO_SWITCH_TO_ONILINE_REMOTE_CTRL_MODE://自動切換線上遠端控制模式
		tempI = ::_ttoi(String);		
		switch ( tempI )
		{
		case MES_EQP_CTRL_STATE_OFFLINE:
		case MES_EQP_CTRL_STATE_LOCAL:
		case MES_EQP_CTRL_STATE_REMOTE:
			SysParam.m_AutoSwitchToOnlineRemoteCtrlMode = (MES_EQP_CTRL_STATE_MODE)(tempI);	break;
		default:
		case MES_EQP_CTRL_STATE_NONE: SysParam.m_AutoSwitchToOnlineRemoteCtrlMode = MES_EQP_CTRL_STATE_NONE; break;
		}
		break;
	case SYSTEM_PHASE_PERIOD_1://第1個相位的週期
		SysParam.m_PhasePeriod1 = ::_tcstod(String, NULL);
		break;
	case SYSTEM_PHASE_PERIOD_2://第2個相位的週期
		SysParam.m_PhasePeriod2 = ::_tcstod(String, NULL);
		break;
	case SYSTEM_PHASE_PERIOD_3://第3個相位的週期
		SysParam.m_PhasePeriod3 = ::_tcstod(String, NULL);
		break;	
	case SYSTEM_SEPARATE_DLP_2EXP_TABLE://分離DLP2次曝光表格
		SysParam.m_SeparateDLP2ExpTable = ::_ttoi(String);	
		break;	
	case SYSTEM_PHASE_CONVERT_HEIGHT_MODE://相位轉換高度模式
		SysParam.m_PhaseConvertHeightMode = ::_ttoi(String);	
		switch ( SysParam.m_PhaseConvertHeightMode )
		{
		case PHASE_CONVERT_HEIGHT_SCALE:
		case PHASE_CONVERT_HEIGHT_MAPPING_FUNC_3:
		case PHASE_CONVERT_HEIGHT_MAPPING_FUNC_4:
		case PHASE_CONVERT_HEIGHT_MAPPING_FUNC_5:
		case PHASE_CONVERT_HEIGHT_MAPPING_FUNC_6:
		case PHASE_CONVERT_HEIGHT_MAPPING_FUNC_7:
			break;
		default:
			SysParam.m_PhaseConvertHeightMode = PHASE_CONVERT_HEIGHT_SCALE;
			break;
		}		
		break;
	case SYSTEM_PHASE_NOISE_DEFINE://相位雜訊定義啟用
		SysParam.m_PhaseNoiseDefine = ::_ttoi(String);
		break;
	case SYSTEM_PHASE_NOISE_DEFINE_MODE://相位雜訊定義模式
		SysParam.m_PhaseNoiseDefineMode = ::_ttoi(String);
		break;
	case SYSTEM_PHASE_NOISE_EXTEND_VOID://相位遮罩-無效點外擴
		tempI = ::_ttoi(String);		
		if ( tempI<0 || tempI>255 )
		{	IsOK = false;	}
		else
		{	SysParam.m_PhaseNoiseExtendVoid = tempI; }		
		break;
	case SYSTEM_PHASE_NOISE_LOW_CONTRAST://相位遮罩-低對比
		tempI = ::_ttoi(String);
		if ( tempI<0 || tempI>255 )
		{	IsOK = false;	}
		else
		{	SysParam.m_PhaseNoiseLowContrastA = tempI; }
		break;	
	case SYSTEM_PHASE_NOISE_LOW_POTENTIAL://相位遮罩-低潛力
		tempI = ::_ttoi(String);
		if ( tempI<0 || tempI>255 )
		{	IsOK = false;	}
		else
		{	SysParam.m_PhaseNoiseLowPotentialA = tempI; }		
		break;
	case SYSTEM_PHASE_NOISE_OVER_SATURATED://相位遮罩-過亮度	
		tempI = ::_ttoi(String);
		if ( tempI<0 || tempI>255 )
		{	IsOK = false;	}
		else
		{	SysParam.m_PhaseNoiseOverSaturatedA = tempI; }		
		break;	
	case SYSTEM_PHASE_NOISE_LOW_CONTRAST_B://相位遮罩-低對比
		tempI = ::_ttoi(String);
		if ( tempI<0 || tempI>255 )
		{	IsOK = false;	}
		else
		{	SysParam.m_PhaseNoiseLowContrastB = tempI; }
		break;
	case SYSTEM_PHASE_NOISE_LOW_POTENTIAL_B://相位遮罩-低潛力	
		tempI = ::_ttoi(String);
		if ( tempI<0 || tempI>255 )
		{	IsOK = false;	}
		else
		{	SysParam.m_PhaseNoiseLowPotentialB = tempI; }		
		break;
	case SYSTEM_PHASE_NOISE_OVER_SATURATED_B://相位遮罩-過亮度	
		tempI = ::_ttoi(String);
		if ( tempI<0 || tempI>255 )
		{	IsOK = false;	}
		else
		{	SysParam.m_PhaseNoiseOverSaturatedB = tempI; }	
		break;
	case SYSTEM_PHASE_NOISE_LOW_CONTRAST_C://相位遮罩-低對比
		tempI = ::_ttoi(String);
		if ( tempI<0 || tempI>255 )
		{	IsOK = false;	}
		else
		{	SysParam.m_PhaseNoiseLowContrastC = tempI; }
		break;
	case SYSTEM_PHASE_NOISE_LOW_POTENTIAL_C://相位遮罩-低潛力	
		tempI = ::_ttoi(String);
		if ( tempI<0 || tempI>255 )
		{	IsOK = false;	}
		else
		{	SysParam.m_PhaseNoiseLowPotentialC = tempI; }		
		break;
	case SYSTEM_PHASE_NOISE_OVER_SATURATED_C://相位遮罩-過亮度	
		tempI = ::_ttoi(String);
		if ( tempI<0 || tempI>255 )
		{	IsOK = false;	}
		else
		{	SysParam.m_PhaseNoiseOverSaturatedC = tempI; }	
		break;		
	case SYSTEM_PHASE_NOISE_SMOOTH_FILTER://相位遮罩-平滑處理
		tempI = ::_ttoi(String);
		if ( tempI < 0 )
		{	IsOK = false;	}
		else
		{	SysParam.m_PhaseNoiseSmoothFilter = tempI; }		
		break;
	case SYSTEM_PHASE_NOISE_LOW_CONTRAST_COLOR://相位遮罩-低對比-顏色
		SysParam.m_PhaseNoiseLowContrastColor = (COLORREF)::_ttoi(String);
		break;		
	case SYSTEM_PHASE_NOISE_LOW_POTENTIAL_COLOR://相位遮罩-潛力-顏色
		SysParam.m_PhaseNoiseLowPotentialColor = (COLORREF)::_ttoi(String);
		break;		
	case SYSTEM_PHASE_NOISE_OVER_SATURATED_COLOR://相位遮罩-過亮度-顏色
		SysParam.m_PhaseNoiseOverSaturatedColor = (COLORREF)::_ttoi(String);
		break;		
	case SYSTEM_PHASE_NOISE_EXTEND_VOID_COLOR://相位遮罩-無效點外擴-顏色
		SysParam.m_PhaseNoiseExtendVoidColor = (COLORREF)::_ttoi(String);		
		break;	
	case SYSTEM_SPACE_NOISE_HEIGHT_UNEXPECTED_COLOR://相位遮罩-高度異常-顏色
		SysParam.m_SpaceNoiseHeightUnexpectedColor = (COLORREF)::_ttoi(String);
		break;	
	case SYSTEM_SPACE_NOISE_HEIGHT_OVER_LOW_COLOR://相位遮罩-過低異常-顏色
		SysParam.m_SpaceNoiseHeightOverLowColor = (COLORREF)::_ttoi(String);
		break;	
	case SYSTEM_SPACE_VALID_BEST_COLOR://相位遮罩-可靠高度-顏色	
		SysParam.m_SpaceBestValidColor = (COLORREF)::_ttoi(String);
		break;

	case SYSTEM_3D_OBJECT_DRAW_SCALE_X://3D物件顯示比例-X
		SysParam.m_3DObjectDrawScaleX = ::_ttof(String);
		break;
	case SYSTEM_3D_OBJECT_DRAW_SCALE_Y://3D物件顯示比例-Y
		SysParam.m_3DObjectDrawScaleY = ::_ttof(String);
		break;
	case SYSTEM_3D_OBJECT_DRAW_SCALE_Z://3D物件顯示比例-Z
		SysParam.m_3DObjectDrawScaleZ = ::_ttof(String);
		break;

	case SYSTEM_PANEL_COLOR_1://整板的顏色-1
		SysParam.m_PanelColor1 = (COLORREF)::_ttoi(String);
		break;
	case SYSTEM_PANEL_COLOR_2://整板的顏色-2	
		SysParam.m_PanelColor2 = (COLORREF)::_ttoi(String);
		break;
	case SYSTEM_PANEL_TEXT_COLOR://整板文字顏色	
		SysParam.m_PanelTextColor = (COLORREF)::_ttoi(String);
		break;
	case SYSTEM_PANEL_SELECTED_COLOR://整板選取到的顏色
		SysParam.m_PanelSelectedColor = (COLORREF)::_ttoi(String);
		break;
	case SYSTEM_BOARD_COLOR_1://單板的顏色-1
		SysParam.m_BoardColor1 = (COLORREF)::_ttoi(String);
		break;
	case SYSTEM_BOARD_COLOR_2://單板的顏色-2	
		SysParam.m_BoardColor2 = (COLORREF)::_ttoi(String);
		break;
	case SYSTEM_BOARD_TEXT_COLOR://單板文字顏色	
		SysParam.m_BoardTextColor = (COLORREF)::_ttoi(String);
		break;
	case SYSTEM_BOARD_SELECTED_COLOR://單板選取到的顏色
		SysParam.m_BoardSelectedColor = (COLORREF)::_ttoi(String);
		break;
	case SYSTEM_FD_COLOR_1://定位點的顏色-1
		SysParam.m_FdColor1 = (COLORREF)::_ttoi(String);
		break;
	case SYSTEM_FD_COLOR_2://定位點的顏色-2		
		SysParam.m_FdColor2 = (COLORREF)::_ttoi(String);
		break;
	case SYSTEM_FD_TEXT_COLOR://定位點文字顏色
		SysParam.m_FdTextColor = (COLORREF)::_ttoi(String);
		break;
	case SYSTEM_FD_SELECTED_COLOR://定位點選取到的顏色
		SysParam.m_FdSelectedColor = (COLORREF)::_ttoi(String);
		break;
	case SYSTEM_BARCODE_COLOR_1://條碼的顏色-1
		SysParam.m_BarcodeColor1 = (COLORREF)::_ttoi(String);
		break;
	case SYSTEM_BARCODE_COLOR_2://條碼的顏色-2		
		SysParam.m_BarcodeColor2 = (COLORREF)::_ttoi(String);
		break;
	case SYSTEM_BARCODE_TEXT_COLOR://條碼文字顏色
		SysParam.m_BarcodeTextColor = (COLORREF)::_ttoi(String);
		break;
	case SYSTEM_BARCODE_SELECTED_COLOR://條碼選取到的顏
		SysParam.m_BarcodeSelectedColor = (COLORREF)::_ttoi(String);
		break;
	case SYSTEM_COMPONENT_COLOR_1://零件的顏色-1
		SysParam.m_ComponentColor1 = (COLORREF)::_ttoi(String);
		break;
	case SYSTEM_COMPONENT_COLOR_2://零件的顏色-2
		SysParam.m_ComponentColor2 = (COLORREF)::_ttoi(String);
		break;		
	case SYSTEM_COMPONENT_TEXT_COLOR://零件文字顏色
		SysParam.m_ComponentTextColor = (COLORREF)::_ttoi(String);
		break;
	case SYSTEM_COMPONENT_SELECTED_COLOR://零件選取到的顏色
		SysParam.m_ComponentSelectedColor = (COLORREF)::_ttoi(String);
		break;
	case SYSTEM_DISTRICT_COLOR_1://分段的顏色-1
		SysParam.m_DistrictColor1 = (COLORREF)::_ttoi(String);
		break;
	case SYSTEM_DISTRICT_COLOR_2://分段的顏色-2
		SysParam.m_DistrictColor2 = (COLORREF)::_ttoi(String);
		break;
	case SYSTEM_INSPECTED_RESULT_OK_COLOR://檢測OK顏色
		SysParam.m_InspectedResultOKColor = (COLORREF)::_ttoi(String);
		break;
	case SYSTEM_INSPECTED_RESULT_NG_COLOR://檢測NG顏色
		SysParam.m_InspectedResultNGColor = (COLORREF)::_ttoi(String);
		break;
	case SYSTEM_INSPECTED_RESULT_SKIP_COLOR://檢測Skip顏色
		SysParam.m_InspectedResultSkipColor = (COLORREF)::_ttoi(String);
		break;
	case SYSTEM_INSPECTED_RESULT_BYPASS_COLOR://檢測Bypass顏色
		SysParam.m_InspectedResultBypassColor = (COLORREF)::_ttoi(String);
		break;
	case SYSTEM_INSPECTED_RESULT_UNTEST_COLOR://檢測UnTest顏色
		SysParam.m_InspectedResultUnTestColor = (COLORREF)::_ttoi(String);
		break;
	case SYSTEM_INSPECTED_RESULT_WARNING_COLOR://檢測Warning顏色		
		SysParam.m_InspectedResultWarningColor = (COLORREF)::_ttoi(String);
		break;
	case SYSTEM_INSPECTED_RESULT_EXCEPTION_COLOR://檢測Exception顏色
		SysParam.m_InspectedResultExceptionColor = (COLORREF)::_ttoi(String);
		break;

	case SYSTEM_SPACE_NOISE_SINGLE_CAST_LOW_LIMIT://高度雜訊單投光高度最低極限-um
		SysParam.m_SpaceNoiseSingleCastLowLimit = ::_tcstod(String, NULL);
		break;		
	case SYSTEM_SPACE_NOISE_MULTI_CAST_PATCH_SIZE://高度雜訊多投光合併Patch尺寸
		SysParam.m_SpaceNoiseMultiCastPatchSize = ::_ttoi(String);
		break;
	case SYSTEM_SPACE_NOISE_MULTI_CAST_MERGE_MODE://高度雜訊多投光合併模式
		SysParam.m_SpaceNoiseMultiCastMergeMode = ::_ttoi(String);
		break;
	case SYSTEM_SPACE_NOISE_MULTI_CAST_MERGE_BEST_MODE://高度雜訊多投光合併最可靠模式
		SysParam.m_SpaceNoiseMultiCastMergeBestMode = ::_ttoi(String);
		break;
	case SYSTEM_SPACE_NOISE_MULTI_INTENSITY_MERGE_MODE://高度雜訊多亮度合併模式-Mean, MaxB
		SysParam.m_SpaceNoiseMultiIntensityMergeMode = ::_ttoi(String);
		break;
	case SYSTEM_SPACE_NOISE_MULTI_CAST_MIN_VALID_COUNT://高度雜訊多投光最少有效值數	
		SysParam.m_SpaceNoiseMultiCastMinValidCount = ::_ttoi(String);
		break;	
	case SYSTEM_SPACE_NOISE_MULTI_CAST_MAX_DIFFERENCE://高度雜訊多投光高度最大差值-um
		SysParam.m_SpaceNoiseMultiCastMaxDifference = ::_tcstod(String, NULL);
		break;
	case SYSTEM_SPACE_NOISE_MULTI_CAST_LIMIT_DIFFERENCE://高度雜訊多投光高度極限差值-um	
		SysParam.m_SpaceNoiseMultiCastLimitDifference = ::_tcstod(String, NULL);
		break;
	case SYSTEM_SPACE_NOISE_MULTI_CAST_VALID_BEST_RATIO://高度雜訊多投光高度最好比例-um	
		SysParam.m_SpaceNoiseMultiCastValidBestRatio = ::_tcstod(String, NULL);
		if ( SysParam.m_SpaceNoiseMultiCastValidBestRatio > 100 ) { SysParam.m_SpaceNoiseMultiCastValidBestRatio = 100; }
		if ( SysParam.m_SpaceNoiseMultiCastValidBestRatio < 0 ) { SysParam.m_SpaceNoiseMultiCastValidBestRatio = 0.0; }
		break;
	case SYSTEM_SPACE_NOISE_MULTI_CAST_VALID_DIFFERENCE://高度雜訊多投光高度有效差值-um
		SysParam.m_SpaceNoiseMultiCastValidDifference = ::_tcstod(String, NULL);
		break;
	case SYSTEM_SPACE_NOISE_MULTI_CAST_OPPOSITE_MAX_GRAY://高度雜訊多投光合併對邊灰階上限-gray	
		SysParam.m_SpaceNoiseMultiCastOppositeMaxGray = ::_ttoi(String);
		break;	
	case SYSTEM_SPACE_NOISE_CAST_FILTER_MODE://投光後濾波模式
		SysParam.m_SpaceNoiseCastFilterMode = ::_ttoi(String);
		break;
	case SYSTEM_SPACE_NOISE_CAST_MEDIAN_FILTER_SIZE://高度雜訊的中值濾波尺寸
		SysParam.m_SpaceNoiseCastMedianFilterSize = ::_ttoi(String);
		break;
	case SYSTEM_SPACE_NOISE_CAST_MEDIAN_FILTER_USE_SIZE://高度雜訊的中值濾波使用尺寸
		SysParam.m_SpaceNoiseCastMedianFilterUseSize = ::_ttoi(String);
		break;	
	case SYSTEM_SPACE_NOISE_DATA_VOID_EXPAND_SIZE://空間雜訊無效點外擴尺寸-piexel	
		SysParam.m_SpaceNoiseDataVoidExpandSize = ::_ttoi(String);
		break;		
	case SYSTEM_SPACE_NOISE_DATA_VOID_EXPAND_ENABLED://空間雜訊無效點外擴啟用
		SysParam.m_SpaceNoiseDataVoidExpandEnabed  = ::_ttoi(String);		
		break;
	case SYSTEM_SPACE_NOISE_FIRST_FILTER_MODE://空間雜訊首次濾波模式
		SysParam.m_SpaceNoiseFirstFilterMode  = ::_ttoi(String);		
		break;	
	case SYSTEM_SPACE_NOISE_FIRST_FILTER_PITCH://空間雜訊首次濾波步長
		SysParam.m_SpaceNoiseFirstFilterPitch  = ::_ttoi(String);		
		break;		
	case SYSTEM_SPACE_NOISE_FIRST_KER_SIZE://空間雜訊首次濾波尺寸
		SysParam.m_SpaceNoiseFirstKerSize  = ::_ttoi(String);		
		break;	
	case SYSTEM_SPACE_NOISE_FIRST_USE_SIZE://空間雜訊首次使用尺寸
		SysParam.m_SpaceNoiseFirstUseSize  = ::_ttoi(String);		
		break;
	case SYSTEM_SPACE_NOISE_OVER_LOW_MODE://高度雜訊過低模式
		SysParam.m_SpaceNoiseOverLowerMode = ::_ttoi(String);		
		break;	
	case SYSTEM_SPACE_NOISE_OVER_LOW_RANGE://高度雜訊過低高度-um
		SysParam.m_SpaceNoiseOverLowerRange = ::_ttoi(String);	
		break;
	case SYSTEM_SPACE_NOISE_OVER_LOW_LIMIT://高度雜訊過低極限-um			
		SysParam.m_SpaceNoiseOverLowerLimit = ::_ttoi(String);	
		break;
	case SYSTEM_SPACE_NOISE_OVER_LOW_KER_SIZE://高度雜訊過低濾波尺寸
		SysParam.m_SpaceNoiseOverLowerKerSize = ::_ttoi(String);		
		break;	
	case SYSTEM_SPACE_NOISE_OVER_LOW_USE_SIZE://高度雜訊過低使用尺寸
		SysParam.m_SpaceNoiseOverLowerUseSize = ::_ttoi(String);		
		break;	
	case SYSTEM_SPACE_NOISE_HEIGHT_ABNORMAL_MODE://高度雜訊高度異常模式
		SysParam.m_SpaceNoiseHeightAbnormalMode  = ::_ttoi(String);		
		break;
	case SYSTEM_SPACE_NOISE_HEIGHT_ABNORMAL_PITCH://高度雜訊高度異常步長
		SysParam.m_SpaceNoiseHeightAbnormalPitch  = ::_ttoi(String);		
		break;	
	case SYSTEM_SPACE_NOISE_HEIGHT_ABNORMAL_RANGE://高度雜訊高度異常範圍-um		
		SysParam.m_SpaceNoiseHeightAbnormalRange = ::_ttoi(String);	
		break;		
	case SYSTEM_SPACE_NOISE_HEIGHT_ABNORMAL_CHK_SIZE://高度雜訊高度異常確認尺寸
		SysParam.m_SpaceNoiseHeightAbnormalChkSize = ::_ttoi(String);	
		break;
	case SYSTEM_SPACE_NOISE_HEIGHT_ABNORMAL_KER_SIZE://高度雜訊高度異常濾波尺寸
		SysParam.m_SpaceNoiseHeightAbnormalKerSize = ::_ttoi(String);	
		break;
	case SYSTEM_SPACE_NOISE_HEIGHT_ABNORMAL_USE_SIZE://高度雜訊高度異常使用尺寸
		SysParam.m_SpaceNoiseHeightAbnormalUseSize = ::_ttoi(String);	
		break;	
	case SYSTEM_SPACE_NOISE_HEIGHT_ABNORMAL_REPEAT_CNT://高度雜訊高度異常重複次數	
		SysParam.m_SpaceNoiseHeightAbnormalRepeatCnt = ::_ttoi(String);	
		break;		
	case SYSTEM_SPACE_NOISE_RECONTRUCTED_EXT_SIZE://空間雜訊重建外擴尺寸
		SysParam.m_SpaceNoiseDataVoidReContructedExtSize = ::_ttoi(String);		
		break;
	case SYSTEM_SPACE_NOISE_RECONTRUCTED_ENABLED://空間雜訊重建外擴啟用	
		SysParam.m_SpaceNoiseDataVoidReContructedEnabled = ::_ttoi(String);		
		break;
	case SYSTEM_SPACE_NOISE_FINAL_KER_SIZE://空間雜訊最後濾波尺寸
		SysParam.m_SpaceNoiseFinalKerSize = ::_ttoi(String);		
		break;
	case SYSTEM_SPACE_NOISE_FINAL_USE_SIZE://空間雜訊最後使用尺寸
		SysParam.m_SpaceNoiseFinalUseSize = ::_ttoi(String);			
		break;
	case SYSTEM_SPACE_NOISE_FINAL_FILTER_MODE://空間雜訊最後濾波模式
		SysParam.m_SpaceNoiseFinalFilterMode = ::_ttoi(String);		
		break;
	case SYSTEM_SPACE_NOISE_FINAL_FILTER_PITCH://空間雜訊最後濾波步長
		SysParam.m_SpaceNoiseFinalPitch = ::_ttoi(String);		
		break;	
	case SYSTEM_SPACE_NOISE_FINAL_FILTER_MODE2://空間雜訊最後濾波模式-2
		SysParam.m_SpaceNoiseFinalFilterMode2 = ::_ttoi(String);		
		break;
	case SYSTEM_SPACE_NOISE_FINAL_FILTER_PITCH2://空間雜訊最後濾波步長-2
		SysParam.m_SpaceNoiseFinalPitch2 = ::_ttoi(String);		
		break;	
	case SYSTEM_SPACE_NOISE_FINAL_KER_SIZE2://空間雜訊最後濾波尺寸-2
		SysParam.m_SpaceNoiseFinalKerSize2 = ::_ttoi(String);		
		break;
	case SYSTEM_SPACE_NOISE_FINAL_USE_SIZE2://空間雜訊最後使用尺寸-2
		SysParam.m_SpaceNoiseFinalUseSize2 = ::_ttoi(String);		
		break;
	case SYSTEM_SPACE_VERIFY_MULTI_CAST_GAP_RATIO://高度驗證-多投光差距比例上限-%
		SysParam.m_SpaceVerifyMultiCastGapRatio = ::_ttof(String);		
		break;
	case SYSTEM_SPACE_VERIFY_MULTI_CAST_GAP_THRESHOLD://高度驗證-多投光差距閥值-um	
		SysParam.m_SpaceVerifyMultiCastGapThreshold = ::_ttof(String);		
		break;

	case SYSTEM_CUDA_FUNCTION_ENABLED://Cuda函式啟用
		tempI = ::_ttoi(String); 
		switch ( tempI )
		{
		case FN_ENABLE:
		case FN_DISABLE:
			SysParam.m_CudaFnEnabled = tempI;
			break;
		default:	IsOK = false;	break;
		}
	#ifndef CUDA_USE			
		SysParam.m_CudaFnEnabled = FN_DISABLE;				
	#endif//CUDA_USE	
		break;
	case SYSTEM_CUDA_NUMBER_BLOCK://Cuda Block數量
		tempI = ::_ttoi(String); 
		if ( tempI < 0 ) 
		{	IsOK = false; }
		else
		{	SysParam.m_CudaBlockNumber = tempI; }
		break;	
	case SYSTEM_CUDA_NUMBER_THREAD://Cuda Thread數量
		tempI = ::_ttoi(String); 
		if ( tempI < 0 ) 
		{	IsOK = false; }
		else
		{	SysParam.m_CudaThreadNumber = tempI; }		
		break;
	case SYSTEM_AUTO_RETRY_MAX_COUNT://自動重測次數上限
		SysParam.m_AutoRetryMaxCount = ::_ttoi(String);
		break;
	case SYSTEM_AUTO_SETUP_DLP_PATTERN://自動設定DLP樣板	
		SysParam.m_AutoSetupDlpPattern = ::_ttoi(String);
		break;
	case SYSTEM_RESOLUTION_MODE://解析度模式
		SysParam.m_ResolutionModeTmp = ::_ttoi(String);		
		break;		
	case SYSTEM_RESOLUTION_SHOW_SCALE://解析度顯示比例		
		tempD = ::_ttof(String); 
		if ( tempD > 0 )
		{	SysParam.m_ResolutionShowScale = tempD; }
		break;
	case SYSTEM_MOVE_CAMERA_BEFORE_PCB_IN://進板前移動相機頭
		tempI = ::_ttoi(String); 
		switch ( tempI )
		{
		case FN_ENABLE:
		case FN_DISABLE:
			SysParam.m_MoveCameraBeforePCBIn = tempI;
			break;
		default:	IsOK = false;	break;
		}		
		break;
	case SYSTEM_PCB_IN_USE_CAMERA_IMAGE_MODE://進板使用相機影像模式
		tempI = ::_ttoi(String); 
		switch ( tempI )
		{
		case FN_ENABLE:
		case FN_DISABLE:
			SysParam.m_PCBInUseCameraImageMode = tempI;
			break;
		default:	IsOK = false;	break;
		}
		break;
	case SYSTEM_CLAMP_PCB_BEFORE_TEST_MODE://檢測前夾板模式	
		tempI = ::_ttoi(String); 
		switch ( tempI )
		{
		case FUNC_EXEC_OFF:
		case FUNC_EXEC_AUTO:
		case FUNC_EXEC_ASK:
			SysParam.m_ClampPcbBeforeTestMode = (FUNC_EXEC_MODE)(tempI);
			break;
		default:	IsOK = false;	break;
		}		
		break;		
	case SYSTEM_GRAB_FIDUCIAL_DELAY_TIME://解取定位點影像延遲時間
		tempI = ::_ttoi(String); 
		if ( tempI < 0 ) 
		{	IsOK = false; }
		else
		{	SysParam.m_GrabFiducialDelayTime_ms = tempI; }
		break;	
	case SYSTEM_PCB_OUT_DIRECTION://PCB出板方向
		tempI = ::_ttoi(String); 
		switch ( tempI )
		{
		case PCB_OUT_DIR_FORWARD:
		case PCB_OUT_DIR_BACKWARD:		
		case PCB_OUT_DIR_BACKWARD_OUT:
			SysParam.m_PCBOutDirection = (PCB_OUT_DIRECTION)(tempI);
			break;
		default:	IsOK = false;	break;
		}
		break;
	case SYSTEM_BYPASS_LAST_SIGNAL://忽略上一站訊號-回板使用
		tempI = ::_ttoi(String); 
		switch ( tempI )
		{
		case FN_ENABLE:
		case FN_DISABLE:
			SysParam.m_BypassLastSignal = tempI;
			break;
		default:	IsOK = false;	break;
		}
		break;
	case SYSTEM_BYPASS_NEXT_SIGNAL://忽略下一站訊號-回板使用	
		tempI = ::_ttoi(String); 
		switch ( tempI )
		{
		case FN_ENABLE:
		case FN_DISABLE:
			SysParam.m_BypassNextSignal = tempI;
			break;
		default:	IsOK = false;	break;
		}
		break;
	case SYSTEM_PCB_OK_NG_SIGNAL_DELAY_TIME://PCB OK/NG訊號延遲時間-ms
		tempI = ::_ttoi(String);
		if ( tempI >= 0 )
		{	SysParam.m_PCBOKNGSignalDelayTime = tempI; }
		else
		{	IsOK = false; }
		break;
	case SYSTEM_EDIT_LINE_SIZE_LEVEL://編輯線尺寸的層級
		tempI = ::_ttoi(String); 
		switch ( tempI )
		{
		case 1:
		case 2:				
		case 3:		
			SysParam.m_EditLineSizeLevel = (tempI);
			break;
		default:	IsOK = false;	break;
		}
		break;	
	case SYSTEM_ONLINE_TUNING_KEEP_MAX_TIME://線上調機保留最久時間-分鐘 	
		tempI = ::_ttoi(String); 
		if ( tempI > 0 ) 
		{	SysParam.m_OnlineTuningKeepMaxTime = tempI;	}
		else 
		{	IsOK = false; }
		break;
	case SYSTEM_ONLINE_TUNING_SAVED_MAX_COUNT://線上調機儲存最多數量-片數 
		tempI = ::_ttoi(String); 
		if ( tempI >= 0 ) 
		{	SysParam.m_OnlineTuningSavedMaxCount = tempI;	}
		else 
		{	IsOK = false; }
		break;
	case SYSTEM_USER_LOGIN_ENABLED://使用者登入使用
		tempI = ::_ttoi(String); 
		switch ( tempI )
		{		
		case USER_LOGIN_DISABLE:
		case USER_LOGIN_OPERATOR:
		case USER_LOGIN_ENGINEER:
			SysParam.m_UserLoginMode = (USER_LOGIN_MODE)(tempI);
			break;
		}
		break;			
	case SYSTEM_USER_LOGIN_OPTIONS://使用者登入選項
		tempI = ::_ttoi(String);
		switch (tempI)
		{
		case USER_LOGIN_OPTIONS_PASSWORD:
		case USER_LOGIN_OPTIONS_FINGERPRINT:
		case USER_LOGIN_OPTIONS_FINGERPRINT_ONLY:
			SysParam.m_UserLoginOptions = (USER_LOGIN_OPTIONS)(tempI);
			break;
		}
		break;
	case SYSTEM_OPEN_PROJECT_MODE://開啟專案模式
		tempI = ::_ttoi(String); 
		switch ( tempI )
		{		
		case OPEN_PROJECT_FILE:
		case OPEN_PROJECT_CODE:
			SysParam.m_OpenProjectMode = (OPEN_PROJECT_MODE)(tempI);
			break;
		}
		break;
	case SYSTEM_OPEN_PROJECT_MAP_INDEX://開啟專案底圖編號
		tempI = ::_ttoi(String); 
		if ( tempI >=0 && tempI<FRAME_MAX_COUNT )
		{	SysParam.m_OpenProjectMapIndex = tempI;	}		
		break;
	case SYSTEM_VERIFY_PROJECT_MODE://驗證專案模式
		tempI = ::_ttoi(String); 
		switch ( tempI )
		{		
		case VERIFY_PROJECT_DISABLE:
		case VERIFY_PROJECT_FILENAME:
			SysParam.m_VerifyProjectMode = (VERIFY_PROJECT_MODE)(tempI);
			break;
		}
		break;
	case SYSTEM_VERIFY_PROJECT_FILENAME://驗證專案的檔名
		SysParam.m_VerifyProjectFilename = String;
		break;
	case SYSTEM_MODEL_NAME_USE_PART_NUMBER://模組名稱使用料號 
		tempI = ::_ttoi(String); 
		switch ( tempI )
		{		
		case FN_ENABLE:
		case FN_DISABLE:
			SysParam.m_ModelNameUsePartNumber = (tempI);
			break;
		}
		break;
	case SYSTEM_ONLINE_OPEN_PROJECT_MODE://線上開專案模式
		tempI = ::_ttoi(String); 
		switch ( tempI )
		{		
		case ONLINE_OPEN_PROJECT_DISABLE:
		case ONLINE_OPEN_PROJECT_BARCODE_DEVICE:
		case ONLINE_OPEN_PROJECT_BARCODE_HANDHELD:
		#ifndef ONLINE_OPEN_PROJECT_USE
			tempI = ONLINE_OPEN_PROJECT_DISABLE;
		#endif//ONLINE_OPEN_PROJECT_USE
			SysParam.m_OnlineOpenProjectMode = (ONLINE_OPEN_PROJECT_MODE)(tempI);
			break;
		}
		break;
	case SYSTEM_ONLINE_OPEN_PROJECT_CAMERA_BARCODE://線上開專案-相機條碼
		tempI = ::_ttoi(String); 
		switch ( tempI )
		{		
		case FN_ENABLE:
		case FN_DISABLE:
		#ifndef ONLINE_OPEN_PROJECT_USE
			tempI = FN_DISABLE;
		#endif//ONLINE_OPEN_PROJECT_USE
			SysParam.m_OnlineOpenProjectCameraBarcode = (tempI);
			break;
		}
		break;
	case SYSTEM_ONLINE_OPEN_PROJECT_BARCODE_DEVICE_GRAB_MODE://線上開專案外接條碼取像模式
		tempI = ::_ttoi(String); 
		switch ( tempI )
		{		
		case BARCODE_DEVICE_GRAB_BEFORE_PCB_IN:
		case BARCODE_DEVICE_GRAB_WHILE_PCB_IN:
		case BARCODE_DEVICE_GRAB_AFTER_PCB_IN:
		case BARCODE_DEVICE_GRAB_BEFORE_INSPECT:
			SysParam.m_OnlineOpenProjectBarcodeDeviceGrabMode = (BARCODE_DEVICE_GRAB_MODE)(tempI);
			break;
		}
		break;		
	case SYSTEM_MODEL_UNSET_COLOR://模組未設定顏色
		SysParam.m_ModelUnsetColor = (COLORREF)::_ttoi(String);
		break;
	case SYSTEM_BINARY_MASK_COLOR_RGB://2值化遮罩-顏色-3色燈
		SysParam.m_BinaryMaskColorRGB = (COLORREF)::_ttoi(String);
		break;
	case SYSTEM_BINARY_MASK_COLOR_DEFAULT://2值化遮罩-顏色-預設		
		SysParam.m_BinaryMaskColorDefault = (COLORREF)::_ttoi(String);		
		break;
	case SYSTEM_BINARY_MASK_COLOR_ALPHA://2值化遮罩-顏色-透明度			
		SysParam.m_BinaryMaskColorAlpha = ::_ttoi(String);		
		break;		
	case SYSTEM_INSPECTION_FINISH_SHOW_RESULT_LIST://檢測結束顯示結果列表
		tempI = ::_ttoi(String); 
		switch ( tempI )
		{		
		case FN_ENABLE:
		case FN_DISABLE:
			SysParam.m_InspectionFinishShowResultList = tempI;
			break;
		}
		break;
	case SYSTEM_MODEL_DEFAULT_WND_LEVEL://模組預設檢測框等級
		SysParam.m_ModelDefaultWndLevel = ::_ttoi(String); 		
		break;
	case SYSTEM_SWITCH_PROJECT_3D_FRAME://可切換至專案3D畫面
		tempI = ::_ttoi(String); 
		switch ( tempI )
		{		
		case FN_ENABLE:
		case FN_DISABLE:
			SysParam.m_SwitchProject3DFrame = tempI;
			break;
		}
		break;	
	case SYSTEM_AUTO_SWITCH_WND_3D_FRAME_MODE://自動切換至檢測框3D畫面模式模式
		tempI = ::_ttoi(String); 
		switch ( tempI )
		{		
		case AUTO_SWITCH_WND_3D_FRAME_DISABLE:
		case AUTO_SWITCH_WND_3D_FRAME_ENABLE:
		case AUTO_SWITCH_WND_3D_FRAME_BY_SIZE:
		case AUTO_SWITCH_WND_3D_FRAME_BY_TYPE:
		case AUTO_SWITCH_WND_3D_FRAME_BY_GROUP_CHANGE:
			SysParam.m_AutoSwitchWnd3DFrameMode = (AUTO_SWITCH_WND_3D_FRAME_MODE)(tempI);
			break;
		}
		break;		
	case SYSTEM_AUTO_SWITCH_WND_3D_FRAME_SIZE_LIMIT://自動切換至檢測框3D畫面尺寸上限-um	
		tempI = ::_ttoi(String); 
		if ( tempI > 0 ) 
		{	SysParam.m_AutoSwitchWnd3DFrameSizeLimit = tempI;	}
		break;	
	case SYSTEM_SHOW_COMPONENT_FULL_MAP://顯示零件-整板零件
		tempI = ::_ttoi(String); 
		switch ( tempI )
		{		
		case FN_ENABLE:
		case FN_DISABLE:
			SysParam.m_ShowComponentFullMap = tempI;
			break;
		}
		break;	
	case SYSTEM_SHOW_COMPONENT_DEFECT_ONLY://僅顯示瑕疵零件(線上畫面)	
		tempI = ::_ttoi(String); 
		switch ( tempI )
		{		
		case FN_ENABLE:
		case FN_DISABLE:
			SysParam.m_ShowComponentDefectOnly = tempI;
			break;
		}
		break;
	case SYSTEM_SHOW_DEBUG_FORM_VIEW://顯示除錯頁面 
		tempI = ::_ttoi(String); 
		switch ( tempI )
		{		
		case FN_ENABLE:
		case FN_DISABLE:
			SysParam.m_ShowDebugFormView = tempI;
			break;
		}
		break;
	case SYSTEM_LEVEL_FILTER_SHIFT_ENABLED://分等濾波起點偏移啟用
		tempI = ::_ttoi(String); 
		switch ( tempI )
		{		
		case FN_ENABLE:
		case FN_DISABLE:
			SysParam.m_LevelFilterShiftEnabled = tempI;
			break;
		}
		break;
	case SYSTEM_MEDIAN_FILTER_SHIFT_ENABLED://中值濾波起點偏移啟用
		tempI = ::_ttoi(String); 
		switch ( tempI )
		{		
		case FN_ENABLE:
		case FN_DISABLE:
			SysParam.m_MedianFilterShiftEnabled = tempI;
			break;
		}
		break;	
	case SYSTEM_SMOOTH_FILTER_SHIFT_ENABLED://平滑濾波起點偏移啟用	
		tempI = ::_ttoi(String); 
		switch ( tempI )
		{		
		case FN_ENABLE:
		case FN_DISABLE:
			SysParam.m_SmoothFilterShiftEnabled = tempI;
			break;
		}
		break;	
	case SYSTEM_PYRAMID_MEDIAN_FILTER_SHIFT_ENABLED://金字塔中值濾波起點偏移啟用
		tempI = ::_ttoi(String);
		switch (tempI)
		{
		case FN_ENABLE:
		case FN_DISABLE:
			SysParam.m_PyramidMedianFilterShiftEnabled = tempI;
			break;
		}
		break;
	case SYSTEM_LEVEL_FILTER_PITCH_TOLERANCE://分等濾波步長可接受誤差
		SysParam.m_LevelFilterPitchTolerance = ::_ttoi(String); 
		break;
	case SYSTEM_MEDIAN_FILTER_PITCH_TOLERANCE://中值濾波步長可接受誤差
		SysParam.m_MedianFilterPitchTolerance = ::_ttoi(String); 
		break;
	case SYSTEM_SMOOTH_FILTER_PITCH_TOLERANCE://平滑濾波步長可接受誤差
		SysParam.m_SmoothFilterPitchTolerance = ::_ttoi(String); 
		break;	
	case SYSTEM_PYRAMID_MEDIAN_FILTER_PITCH_TOLERANCE://金字塔中值濾波步長可接受誤差
		SysParam.m_PyramidMedianFilterPitchTolerance = ::_ttoi(String);
		break;
	case SYSTEM_GRR_OFFSET_RANDOM_VALUE://Grr偏移隨機補償值-nm
		SysParam.m_GrrOffsetRandomValue = ::_ttoi(String); 
		break;	
	case SYSTEM_GRR_AVERAGE_RESET_ENABLED://Grr平均值復歸-啟用-依據條碼比較
		SysParam.m_GrrAverageResetEnabled = ::_ttoi(String); 
		break;
	case SYSTEM_GRR_AVERAGE_RESET_MATCH_RATIO://Grr平均值復歸-匹配比例
		SysParam.m_GrrAverageResetMatchRatio = ::_ttof(String); 
		break;		
	case SYSTEM_GRR_SKEW_AVERAGE_ENB_VAL://Grr偏移角度平均啟用角度
		SysParam.m_GrrSkewAverageEnbVal = MAX(0, ::_ttof(String)); 
		break;
	case SYSTEM_GRR_SKEW_AVERAGE_START_GAP://Grr偏移角度平均開始差距-角度	
		SysParam.m_GrrSkewAverageStartGap = MAX(0, ::_ttof(String)); 
		break;
	case SYSTEM_GRR_SKEW_AVERAGE_WEIGHTING://Grr偏移角度平均權重 
		SysParam.m_GrrSkewAverageWeighting = MAX(0, ::_ttoi(String)); 
		break;
	case SYSTEM_GRR_OFFSET_AVERAGE_ENB_PXL://Grr偏移平均啟用像素
		SysParam.m_GrrOffsetAverageEnbPxl = MAX(0, ::_ttoi(String)); 
		break;
	case SYSTEM_GRR_OFFSET_AVERAGE_START_GAP://Grr偏移平均開始差距-微米	
		SysParam.m_GrrOffsetAverageStartGap = MAX(0, ::_ttof(String)); 
		break;
	case SYSTEM_GRR_OFFSET_AVERAGE_WEIGHTING://Grr偏移平均權重
		SysParam.m_GrrOffsetAverageWeighting = MAX(0, ::_ttoi(String)); 
		break;	
	case SYSTEM_GRR_HEIGHT_AVERAGE_ENB_VAL://Grr偏移高度平均啟用高度
		SysParam.m_GrrHeightAverageEnbVal = MAX(0, ::_ttof(String)); 
		break;
	case SYSTEM_GRR_HEIGHT_AVERAGE_START_GAP://Grr偏移高度平均開始差距-微米
		SysParam.m_GrrHeightAverageStartGap = MAX(0, ::_ttof(String)); 
		break;
	case SYSTEM_GRR_HEIGHT_AVERAGE_WEIGHTING://Grr偏移高度平均權重 
		SysParam.m_GrrHeightAverageWeighting = MAX(0, ::_ttoi(String)); 
		break;
	case SYSTEM_LOCK_SCREEN_ENABLED://啟用鎖住螢幕
		SysParam.m_LockScreenEnabled = ::_ttoi(String); 
		break;	
	case SYSTEM_LOCK_SCREEN_KEY_ID://鎖住螢幕鍵號
		SysParam.m_LockScreenKeyID = ::_ttoi(String); 
		break;
	case SYSTEM_MULTI_DISTRICT_MODE_ENABLED://多段檢測啟用
		SysParam.m_MultiDistrictModeEnabled = ::_ttoi(String); 
		break;	
	case SYSTEM_ONLINE_TUNING_ENABLE_BARCODE://在線調機-軟體條碼
		SysParam.m_OnlineTuningEnableBarcode = ::_ttoi(String); 
		break;		
	case SYSTEM_SHOW_PLC_SAFTY_SETTING_UI://是否顯示PLC安全檢知設定介面
		SysParam.m_ShowPlcSaftySettingUI = ::_ttoi(String); 
		break;
	case SYSTEM_SHOW_ALG_OFFSET_L_PARAM://是否顯示演算法OffsetL的參數 	
		SysParam.m_ShowAlgOffsetLParam = ::_ttoi(String); 
		break;
	case SYSTEM_SHOW_ALG_OFFSET_A_PARAM://是否顯示演算法OffsetA的參數
		SysParam.m_ShowAlgOffsetAParam = ::_ttoi(String); 
		break;
	case SYSTEM_SHOW_ALG_BRIGHT_RATIO_SCALE_PARAM://是否顯示演算法亮度比例的比例參數
		SysParam.m_ShowAlgBrightRatioScaleParam = ::_ttoi(String); 
		break;
	case SYSTEM_SHOW_MODEL_PROPERTY_PARAM://是否顯示模組屬性參數
		SysParam.m_ShowModelPropertyParam = ::_ttoi(String); 
		break;	
	case SYSTEM_CONTINUE_PASTE_MODE://連續貼上模式
		SysParam.m_ContinuePasteMode = ::_ttoi(String); 
		break;
	case SYSTEM_STITCH_IMAGE_PADDING_SIZE://拼圖參數-填補尺寸
		SysParam.m_StitchImagePaddingSize = ::_ttoi(String); 
		break;
	case SYSTEM_RESIN_HEIGHT_ALIGN_ENABLED://啟用Resin高度對齊-軍達3D對位
		SysParam.m_ResinHeightAlignEnabled = ::_ttoi(String); 
		break;
	case SYSTEM_MODEL_IMAGE_CAD_OFFSET_ENABLED://啟用模組影像Cad偏移補償um
		SysParam.m_ModelImageCadOffsetEnabled = ::_ttoi(String); 
		break;
	case SYSTEM_MODEL_DEFAULT_TRANSISTOR_TYPE://模組預設SOT樣式
		tempI = ::_ttoi(String); 
		switch ( tempI )
		{
		case MODEL_TYPE_TRANSISTOR:
		case MODEL_TYPE_LEAD_TRANSISTOR:
			SysParam.m_ModelDefaultTransistorType = (MODEL_TYPE)(tempI); 
			break;
		}		
		break;	
	case SYSTEM_PROJECT_LOCAL_FOLDER_ENABLED://專案使用本機資料夾
		SysParam.m_ProjectLocalFolderEnabled = ::_ttoi(String);
		break;
	case SYSTEM_PARTIAL_COPY_PROJECT_LIBRARY://部分複製專案資料庫	
		SysParam.m_PartialCopyProjectLibrary = ::_ttoi(String);
		break;
	case SYSTEM_AUTO_ARRANGE_MODEL_BK_IMAGE_FILES://自動重整模組底圖檔案
		SysParam.m_AutoArrangeModelBkImageFiles = ::_ttoi(String);
		break;
	case SYSTEM_AUTO_COPY_SPC_COMPONENT_IMAGE_FILES://自動複製SPC零件圖檔
		SysParam.m_AutoCopySpcComponentImageFiles = ::_ttoi(String);
		break;
	case SYSTEM_AUTO_BYPASS_GRAB_3D_FRAME://自動跳過3D影像	
		SysParam.m_AutoBypassGrab3DFrame = ::_ttoi(String);
		break;
	case SYSTEM_CHECK_PROJECT_FD_READY://確認專案定位點狀態
		SysParam.m_CheckProjectFdReady = ::_ttoi(String);
		break;
	case SYSTEM_LOCK_MODEL_BODY_POSITION://鎖住模組本體的位置
		SysParam.m_LockModelBodyPosition = ::_ttoi(String);
		break;	
	case SYSTEM_MAX_UNCHECK_TEST_FILE_COUNT://最多未判定檢測檔案數 
		SysParam.m_MaxUncheckTestFileCount = ::_ttoi(String);
		break;	
	case SYSTEM_CONFIRM_COMPONENT_BARCODE_ENABLED://啟用零件條碼確認	
		SysParam.m_ConfirmComponentBarcodeEnabled = ::_ttoi(String);
		break;
	case SYSTEM_SAVE_JPEG_QUALITY://儲存JPEG的質量(001~100)
		SysParam.m_SaveJpegQuality = ::_ttoi(String);
		if ( SysParam.m_SaveJpegQuality < 1 ) { SysParam.m_SaveJpegQuality = 1; }
		else if ( SysParam.m_SaveJpegQuality > 100 ) { SysParam.m_SaveJpegQuality = 100; }
		break;		
	case SYSTEM_COPY_HUGE_FILES_MODE://複製大量檔案模式
		SysParam.m_CopyHugeFilesMode = (COPY_HUGE_FILES_MODE)(::_ttoi(String)); 
		break;

	case SYSTEM_OFFLINE_VERSION_MODE://離線版本模式
		tempI = ::_ttoi(String);
		switch ( tempI )
		{
		case OFFLINE_VERSION_NORMAL:
		case OFFLINE_VERSION_HOST_TUNING:
		case OFFLINE_VERSION_REMOTE_TUNING:
			SysParam.m_OfflineVersionMode = (OFFLINE_VERSION_MODE)(tempI); 
			break;
		}		
		break;	
	case SYSTEM_USE_PROJECT_SYSTEM_PARAM_MODE://使用專案系統參數模式	
		SysParam.m_UseProjectSystemParamMode = ::_ttoi(String); 
		break;
	
	case SYSTEM_UI_WND_FONT_ADD_SIZE://UI視窗字型增加大小
		tempI = ::_ttoi(String); 
		if ( tempI >= 0 )
		{	SysParam.m_UIWndFontAddSize = tempI;	}		
		break;
	case SYSTEM_UI_DOCK_WND_SLIDE_STEPS://UI駐停視窗滑動步長	
		SysParam.m_UIDockWndSlideSteps = ::_ttoi(String); 
		if ( SysParam.m_UIDockWndSlideSteps < 1 ) { SysParam.m_UIDockWndSlideSteps = 1; }
		break;
	case SYSTEM_UI_ENABLE_PCB_OUT_BUTTON://UI啟用PCB出板按鈕
		tempI = ::_ttoi(String); 
		if ( tempI >= 0 )
		{	SysParam.m_UIEnablePCBOutButton = tempI;	}		
		break;

	case SYSTEM_RABBITMQ_SERVER_PORT://RabbitMQ伺服器Port
		SysParam.m_RabbitMQServerPort = ::_ttoi(String);//5672
		break;
	case SYSTEM_RABBITMQ_IP_ADDRESS://RabbitMQ通訊網址
		SysParam.m_RabbitMQServerAddress= String;//"localhost"
		break;
	case SYSTEM_RABBITMQ_USER_NAME://RabbitMQ登錄名稱
		SysParam.m_RabbitMQUserName= String;//"aoiuser"
		break;
	case SYSTEM_RABBITMQ_PASSWORD://RabbitMQ登錄密碼
		SysParam.m_RabbitMQPassword= String;//"123456"
		break;

	case SYSTEM_ITS_FILENAME://ITS軟體名稱
		SysParam.m_ITSFilename = (String);
		break;
	case SYSTEM_ITS_COMMUNICATION_MODE://ITS通訊模式
		SysParam.m_ITSCommunicationMode = (ITS_COMMUNICATION_MODE)::_ttoi(String); 		
		break;
	case SYSTEM_ITS_SOCKET_IP_PORT://ITS網路Port
		SysParam.m_ITSSocketIPPort = ::_ttoi(String); 		
		break;
	case SYSTEM_ITS_SOCKET_IP_ADDRESS://ITS網路網址
		SysParam.m_ITSSocketIPAddress= String;
		break;
	case SYSTEM_ITS_RABBITMQ_QUEUE_NAME_RECV://ITS訊息佇列接收名
		SysParam.m_ITSRabbitMQRecvQueueName= String;//"ITS2AOI"
		break;
	case SYSTEM_ITS_RABBITMQ_QUEUE_NAME_SEND://ITS訊息佇列傳送名
		SysParam.m_ITSRabbitMQSendQueueName= String;//"AOI2ITS"
		break;
	case SYSTEM_ITS_CONTACT_SOFTWARE://ITS-對接軟體		
		SysParam.m_ITSContactSoftware = ::_ttoi(String);
		switch ( SysParam.m_ITSContactSoftware )
		{
	#ifndef ITS_DISABLE
		case MES_CONTACT_IBS:
	#endif//ITS_DISABLE
	#ifndef MTS_DISABLE
		case MES_CONTACT_MTS:
	#endif//MTS_DISABLE
	#ifndef IPS_DISABLE
		case MES_CONTACT_IPS:
	#endif//IPS_DISABLE
			break;
		default:
			SysParam.m_ITSContactSoftware=MES_CONTACT_TYPE;
			break;
		}
		break;
	case SYSTEM_ITS_COMMUNICATION_ENABLED://連線至ITS啟用
		SysParam.m_ITSCommunicationEnabled = ::_ttoi(String); 		
		break;
	case SYSTEM_ITS_COMMUNICATION_TIMEOUT_MS://與ITS溝通逾時(ms)	
		SysParam.m_ITSCommunicationTimeoutMS = ::_ttoi(String); 		
		break;	
	case SYSTEM_ITS_FILE_FOLDER_SEND://ITS檔案資料夾-傳送
		SysParam.m_ITSFileFolderSend= String;//"AOI2ITS"
		break;
	case SYSTEM_ITS_FILE_FOLDER_RECV://ITS檔案資料夾-接收
		SysParam.m_ITSFileFolderRecv= String;//"ITS2AOI"
		break;
	case SYSTEM_ITS_FILE_BACKUP_ENABLED://ITS檔案資料夾-備份	
		SysParam.m_ITSFileBackupEnabled = ::_ttoi(String); 		
		break;
	case SYSTEM_ITS_FILE_USE_SYNC_FILE_ENABLED://ITS檔案資料夾-同步檔案
		SysParam.m_ITSFileUseSyncFileEnabled = ::_ttoi(String); 		
		break;	
	case SYSTEM_ITS_SET_SECS_GEM_ENABLED://ITS使用設定SECS/GEM
		SysParam.m_ITSSetSecsGemEnabled = ::_ttoi(String);
		break;
	case SYSTEM_ITS_SECS_GEM_REMOTE_LOCAL: //ITS使用SECS/GEM-Remote Local
		SysParam.m_ITSSetSecsGemRemoteLocal = ::_ttoi(String);
		break;
	case SYSTEM_ITS_SET_SYSTEM_PARAM_ENABLED://ITS使用設定系統參數
		SysParam.m_ITSSetSystemParamEnabled = ::_ttoi(String);
		break;
	case SYSTEM_ITS_SET_PROJECT_PARAM_ENABLED://ITS使用設定專案參數
		SysParam.m_ITSSetProjectParamEnabled = ::_ttoi(String);
		break;
	case SYSTEM_ITS_SET_MACHINE_STATUS_ENABLED://ITS使用設定機台狀態
		SysParam.m_ITSSetMacineStatusEnabled = ::_ttoi(String);
		break;
	case SYSTEM_ITS_SET_APP_OPEN_CLOSE_ENABLED://ITS使用設定軟體開關
		SysParam.m_ITSSetAppOpnCloseEnabled = ::_ttoi(String);
		break;
	case SYSTEM_ITS_SET_PROCESS_ID_ENABLED://ITS使用設定程序編號
		SysParam.m_ITSSetProcessIDEnabled = ::_ttoi(String);
		break;
	case SYSTEM_ITS_SET_USER_LOGIN_OUT_ENABLED://ITS使用設定使用者登入登出
		SysParam.m_ITSSetUserLogin_outEnabled = ::_ttoi(String);
		break;	
	case SYSTEM_PROG_DEFAULT_SPACE_TO_GRAY_RATIO_MODE://專案預設高度轉灰階比例 
		tempI = ::_ttoi(String); 
		SysParam.m_DefaultSpaceToGrayRatioMode = tempI;
		break;
	case SYSTEM_PROG_DEFAULT_SPACE_BASE_PLANE_INDEX://專案預設空間基準面編號 
		tempI = ::_ttoi(String); 
		if ( tempI < 0 ) { tempI = -1; }
		else if ( tempI > MAX_SYSTEM_BASE_PLANE_PARAM_COUNT ) { tempI = MAX_SYSTEM_BASE_PLANE_PARAM_COUNT; }
		SysParam.m_DefaultSpaceBasePlaneIndex = tempI;
		break;
	case SYSTEM_PROG_DEFAULT_SPACE_NOISE_FILTER_INDEX://專案預設空間雜訊過濾編號
		tempI = ::_ttoi(String); 
		if ( tempI < 0 ) { tempI = -1; }
		else if ( tempI > MAX_SYSTEM_NOISE_FILTER_PARAM_COUNT ) { tempI = MAX_SYSTEM_NOISE_FILTER_PARAM_COUNT; }
		SysParam.m_DefaultSpaceNoiseFilterIndex = tempI;
		break;
	case SYSTEM_PROG_DEFAULT_ENABLE_CONVEYER_PRE_RUN://專案預設軌道提前運轉功能
		tempI = ::_ttoi(String); 
		switch ( tempI )
		{
		case FN_ENABLE:
		case FN_DISABLE:
			SysParam.m_DefaultEnableConveyerPreRun = tempI;
			break;
		default:	IsOK = false;	break;
		}		
		break;
	case SYSTEM_PROG_DEFAULT_FD_NG_HANDLE_MODE://專案預設定位點異常處理模式
		tempI = ::_ttoi(String); 
		switch ( tempI )
		{
		case FD_NG_HANDLE_PASS:
		case FD_NG_HANDLE_STOP:		
			SysParam.m_DefaultFdNGHandleMode = (FD_NG_HANDLE_MODE)(tempI);;
			break;
		default:	IsOK = false;	break;
		}		
		break;
	case SYSTEM_PROG_DEFAULT_BOARD_FD_GRAB_MODE://專案預設單板定位點取像模式
		tempI = ::_ttoi(String); 
		switch ( tempI )
		{
		case BOARD_FD_GRAB_AFTER_PANEL:
		case BOARD_FD_GRAB_INSPECTING:
			SysParam.m_DefaultBoardFdGrabMode = (BOARD_FD_GRAB_MODE)(tempI);;
			break;
		default:	IsOK = false;	break;
		}		
		break;		
	case SYSTEM_PROG_DEFAULT_BDEFECT_HANDLE_MODE://專案預設檢出異常處理模式
		tempI = ::_ttoi(String); 
		switch ( tempI )
		{
		case DEFECT_HANDLE_PASS:
		case DEFECT_HANDLE_STOP_ALARM:
		case DEFECT_HANDLE_NEXT_STOP:
		case DEFECT_HANDLE_WAIT_FOR_REPAIR:
		case DEFECT_HANDLE_CONTROL_CENTER:
			SysParam.m_DefaultDefectHandleMode = (DEFECT_HANDLE_MODE)(tempI);
			break;
		default:	IsOK = false;	break;
		}
		break;
	case SYSTEM_PROG_DEFAULT_PCB_OUT_MODE://專案預設PCB出板模式
		tempI = ::_ttoi(String); 
		switch ( tempI )
		{
		case PCB_OUT_NORMAL:
		case PCB_OUT_SIDE_OUT:
		case PCB_OUT_WITH_IN:
		case PCB_OUT_LANE_AUTO:
		case PCB_OUT_OK_OUT_NG_SIDE:
			SysParam.m_DefaultPCBOutMode = (PCB_OUT_MODE)(tempI);
			break;
		default:	IsOK = false;	break;
		}
		break;
	case SYSTEM_PROG_DEFAULT_PROJECT_SAVE_TEST_MAP://專案預設儲存檢測底圖模式
		tempI = ::_ttoi(String); 
		switch ( tempI )
		{
		case SAVE_TEST_MAP_DISABLE:
		case SAVE_TEST_MAP_ENB_PROG:
		case SAVE_TEST_MAP_ENB_PANEL:
		case SAVE_TEST_MAP_ENB_PROG_PANEL:
		case SAVE_TEST_MAP_ENB_BOARD:
		case SAVE_TEST_MAP_ENB_PROG_BOARD:
			SysParam.m_DefaultProjectSaveTestMap = (SAVE_TEST_MAP_MODE)(tempI);
			break;
		default:	IsOK = false;	break;
		}
		break;
	case SYSTEM_PROG_DEFAULT_PROJECT_LINK_SERVER_MODE://專案預設連線伺服器模式	
		tempI = ::_ttoi(String); 
		switch ( tempI )
		{
		case PROJECT_LINK_SERVER_DISABLE:
		case PROJECT_LINK_SERVER_ENABLE_ALL:
		case PROJECT_LINK_SERVER_PROJECT_ONLY:
		case PROJECT_LINK_SERVER_ENABLE_ALL_ASK:
		case PROJECT_LINK_SERVER_PROJECT_ONLY_ASK:
			SysParam.m_DefaultProjectLinkServerMode = (PROJECT_LINK_SERVER_MODE)(tempI);
			break;
		default:	IsOK = false;	break;
		}
		break;
	case SYSTEM_PROG_DEFAULT_SAVE_OFFLINE_IMAGE_FILES://專案預設儲存離線圖檔
		tempI = ::_ttoi(String); 
		switch ( tempI )
		{
		case FN_ENABLE:
		case FN_DISABLE:
			SysParam.m_DefaultProjectSaveOfflineImageFiles=(bool)(tempI);
			break;
		}
		break;
	case SYSTEM_PROG_DEFAULT_BARCODE_VERIFY_MODE://專案預設條碼驗證模式
		tempI = ::_ttoi(String); 
		switch ( tempI )
		{
		case FN_ENABLE:
		case FN_DISABLE:
			SysParam.m_DefaultBarcodeVerifyMode=tempI;
			break;
		}
		break;
	case SYSTEM_PROG_DEFAULT_BARCODE_RETRIEVE_MODE://專案預設條碼查詢模式
		tempI = ::_ttoi(String); 
		switch ( tempI )
		{
		case FN_ENABLE:
		case FN_DISABLE:
			SysParam.m_DefaultBarcodeRetrieveMode=tempI;
			break;
		}
		break;

	case SYSTEM_OPERATE_LEVEL_PROJECT_OPEN://操作等級-專案開啟
		SysParam.m_OperateLevelProjectOpen = LoadUserLevel(String);
		break;
	case SYSTEM_OPERATE_LEVEL_PROJECT_SAVE://操作等級-專案儲存
		SysParam.m_OperateLevelProjectSave = LoadUserLevel(String);
		break;
	case SYSTEM_OPERATE_LEVEL_PROJECT_PARAM://操作等級-專案參數
		SysParam.m_OperateLevelProjectParam = LoadUserLevel(String);
		break;
	case SYSTEM_OPERATE_LEVEL_ONLINE_RUN://操作等級-線上運行
		SysParam.m_OperateLevelOnlineRun = LoadUserLevel(String);
		break;
	case SYSTEM_OPERATE_LEVEL_ONLINE_BYPASS://操作等級-線上直通
		SysParam.m_OperateLevelOnlineBypass = LoadUserLevel(String);
		break;
	case SYSTEM_OPERATE_LEVEL_ONLINE_STOP://操作等級-線上停止
		SysParam.m_OperateLevelOnlineStop = LoadUserLevel(String);
		break;	
	case SYSTEM_OPERATE_LEVEL_ONLINE_SAVE_IMAGE://操作等級-線上存圖
		SysParam.m_OperateLevelOnlineSaveImage = LoadUserLevel(String);
		break;
	case SYSTEM_OPERATE_LEVEL_ONLINE_UNLOCK://操作等級-線上解鎖
		SysParam.m_OperateLevelOnlineUnlock = LoadUserLevel(String);
		break;	
	case SYSTEM_OPERATE_LEVEL_EDIT_FUNC_ADD://操作等級-編輯功能-新增
		SysParam.m_OperateLevelEditFuncAdd = LoadUserLevel(String);
		break;
	case SYSTEM_OPERATE_LEVEL_EDIT_FUNC_DEL://操作等級-編輯功能-刪除
		SysParam.m_OperateLevelEditFuncDel = LoadUserLevel(String);
		break;
	case SYSTEM_OPERATE_LEVEL_EDIT_FUNC_BYPASS://操作等級-編輯功能-不檢測
		SysParam.m_OperateLevelEditFuncBypass = LoadUserLevel(String);
		break;	
	case SYSTEM_OPERATE_LEVEL_MES_CTRL_STATE_OFFLINE://操作等級-MES相關-控制狀態-離線
		SysParam.m_OperateLevelMesCtrlState_Offline = LoadUserLevel(String);
		break;
	case SYSTEM_OPERATE_LEVEL_MES_CTRL_STATE_LOCAL://操作等級-MES相關-控制狀態-本地上線
		SysParam.m_OperateLevelMesCtrlState_Local = LoadUserLevel(String);
		break;
	case SYSTEM_OPERATE_LEVEL_MES_CTRL_STATE_REMOTE://操作等級-MES相關-控制狀態-遠端上線
		SysParam.m_OperateLevelMesCtrlState_Remote = LoadUserLevel(String);
		break;
	case SYSTEM_OPERATE_LEVEL_MES_SHOW_CONTEXT://操作等級-MES相關-顯示內容
		SysParam.m_OperateLevelMesShowContext = LoadUserLevel(String);
		break;

	case SYSTEM_M2M_NPM_BARCODE_ENABLE://啟用NPM-條碼
		tempI = ::_ttoi(String); 
		SysParam.m_M2M_NPM_Barcode_Enable = tempI;		
		break;		
	case SYSTEM_M2M_NPM_APC_FF1_ENABLE://啟用NPM APC-FF1
		tempI = ::_ttoi(String); 
		SysParam.m_M2M_NPM_APC_FF1_Enable = tempI;		
		break;
	case SYSTEM_M2M_NPM_APC_FF2_ENABLE://啟用NPM APC-FF2
		tempI = ::_ttoi(String); 
		SysParam.m_M2M_NPM_APC_FF2_Enable = tempI;		
		break;
	case SYSTEM_M2M_NPM_APC_MFB_ENABLE://啟用NPM APC-MFB
		tempI = ::_ttoi(String); 
		SysParam.m_M2M_NPM_APC_MFB_Enable = tempI;		
		break;
	case SYSTEM_M2M_NPM_LANE_NAME_LA://NPM軌道名稱-A軌
		SysParam.m_M2M_NPM_LaneName_LA = String;
		break;
	case SYSTEM_M2M_NPM_LANE_NAME_LB://NPM軌道名稱-B軌
		SysParam.m_M2M_NPM_LaneName_LB = String;
		break;
	case SYSTEM_M2M_NPM_INPUT_SHARE_FOLDER_LA://NPM-輸入共享資料夾-A軌		
		SysParam.m_M2M_NPM_InputShareFolder_LA = String;
		break;
	case SYSTEM_M2M_NPM_INPUT_SHARE_FOLDER_LB://NPM-輸入共享資料夾-B軌
		SysParam.m_M2M_NPM_InputShareFolder_LB = String;
		break;
	case SYSTEM_M2M_NPM_OUTPUT_SHARE_FOLDER_LA://NPM-輸出共享資料夾-A軌
		SysParam.m_M2M_NPM_OutputShareFolder_LA = String;		
		break;
	case SYSTEM_M2M_NPM_OUTPUT_SHARE_FOLDER_LB://NPM-輸出共享資料夾-B軌
		SysParam.m_M2M_NPM_OutputShareFolder_LB = String;
		break;

	case SYSTEM_M2M_HASI_ENABLE://啟用M2M HAS I (Hanwha AOI Solution MAOI_SAOI)
		tempI = ::_ttoi(String);
		SysParam.m_M2M_HASI_Enable = tempI;
		break;
	case SYSTEM_M2M_HASI_SHARE_FOLDER://HAS I 共享資料夾
		SysParam.m_M2M_HASI_ShareFolder = String;
		break;
	case SYSTEM_M2M_HASI_SERIALFILE_ENABLE://啟用 HAS I - SerialFile
		tempI = ::_ttoi(String);
		SysParam.m_M2M_HASI_SerialFile_Enable = tempI;
		break;
	case SYSTEM_M2M_HASI_SERIALFILE_QUEUESIZE://HAS I - SerialFile - Size of Serial Queue
		tempI = ::_ttoi(String);
		SysParam.m_M2M_HASI_SerialFile_QueueSize = tempI;
		break;
	case SYSTEM_M2M_HASI_SERIALFILE_DWELLTIME://等待資料的延遲時間-ms
		tempI = ::_ttoi(String);
		SysParam.m_M2M_HASI_SerialFile_DwellTime = tempI;
		break;
	case SYSTEM_M2M_HASI_SPIOFFSETFILE_ENABLE://啟用SPIOffsetFile-Hanwha
		tempI = ::_ttoi(String);
		SysParam.m_M2M_HASI_SPIOffsetFile_Enable = tempI;
		break;
	case SYSTEM_M2M_HASI_PNP_ENABLE://啟用 HAS I - PNP
		tempI = ::_ttoi(String);
		SysParam.m_M2M_HASI_PNP_Enable = tempI;
		break;
	case SYSTEM_M2M_HASI_AOI_STAGE://產線上 AOI 的檢查階段
		tempI = ::_ttoi(String);
		SysParam.m_M2M_HASI_AOI_Stage = (HASI_AOI_STAGE)tempI;
		break;
	case SYSTEM_M2M_HASI_STATE_MODE://HASI 狀態
		tempI = ::_ttoi(String);
		SysParam.m_M2M_HASI_StateMode = (HASI_STATE_MODE)tempI;
		break;

	case SYSTEM_AI_MODEL_SERVER_ENABLE://AI模型伺服器啟用
		SysParam.m_AiModelServerEnabled = ::_ttoi(String); 		
		break;
	case SYSTEM_AI_MODEL_SERVER_TIMEOUT://AI模型伺服器逾時
		SysParam.m_AiModelServerTimeout = ::_ttoi(String); 		
		break;		
	case SYSTEM_AI_MODEL_SERVER_FILENAME://AI模型伺服器檔名
		SysParam.m_AiModelServerFilename = String;
		break;
	case SYSTEM_AI_MODEL_FILE_FOLDER_SEND://AI模型檔案資料夾-傳送
		SysParam.m_AiModelFileFolderSend = String;
		break;
	case SYSTEM_AI_MODEL_FILE_FOLDER_RECV://AI模型檔案資料夾-接收
		SysParam.m_AiModelFileFolderRecv = String;
		break;
	case SYSTEM_AI_MODEL_LABEL_MIN_CLUSTER_DISTANCE://AI模型分類最小叢集距離-um
		tempD = ::_ttof(String); 		
		SysParam.m_AIModelLabelMinClusterDistance = tempD;
		break;

	case SYSTEM_EXTERNAL_COPY_FILE_ENABLED://外部複製檔案啟用
		SysParam.m_ExternalCopyFileEnabled = ::_ttoi(String);
		break;
	case SYSTEM_EXTERNAL_COPY_FILE_APP_NAME://外部複製檔案軟體名稱
		SysParam.m_ExternalCopyFileAppName = String;
		break;
	case SYSTEM_EXTERNAL_COPY_FILE_SEND_FOLDER://外部複製檔案輸出資料夾
		SysParam.m_ExternalCopyFileSendFolder = String;
		break;

	case SYSTEM_CPK_CHART_ENABLED://Cpk圖表啟用
		SysParam.m_CpkChartEnabled = ::_ttoi(String);
		break;
	case SYSTEM_WND_ROTATION_FOLLOWED://檢測框跟隨旋轉
		SysParam.m_WndRotationFollowed = ::_ttoi(String);
		break;

	default:	
		IsOK = false;
		Err.Format(_T("Error, No System Param Define [%d]"), ParamID);
		JetAPI::ShowMessageBox(Err);
		break;
	}
	/*
	CString                    m_AppVersion;//軟件版本
	PHASE_UNWRAP_MODE          m_UnwrappingMode;//是否使用unwrapping			
	
	int                        m_PhaseSmoothCudaMode;//
	int                        m_PhaseSmoothCudaMaskSize;
	
	*/
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::GetSystemParameterStringByID(SYSTEM_PARAM_ID ParamID, const TSystemParameter &SysParam, CString &String)//取得系統參數
{
	CString Err;	
	int     tempI=0;
	double  tempD=0;
	bool IsOK = true;	
	switch ( ParamID )
	{
	case SYSTEM_AOI_FOLDER_HOST://系統資料夾
		String = SysParam.m_AOIDirectory;		 
		break;
	case SYSTEM_AOI_FOLDER_TEMP://暫存的資料夾		
		String = SysParam.m_AOIBaseTempDirectory;		
		break;
	case SYSTEM_AOI_FOLDER_LOG://系統訊息資料夾		
		String = SysParam.m_AOILogDirectory;		
		break;
	case SYSTEM_AOI_FOLDER_PROJECT://專案資料夾	
		String = SysParam.m_AOIProjectFolder;
		break;
	case SYSTEM_AOI_FOLDER_RESULT://結果資料夾
		String = SysParam.m_AOIResultFolder;		
		break;	
	case SYSTEM_AOI_FOLDER_SERVER://伺服器資料夾
		String = SysParam.m_AOIServerFolder;
		break;
	case SYSTEM_AOI_FOLDER_STATIC_DATA://統計數據資料夾
		String = SysParam.m_AOIStaticDataFolder;
		break;
	case SYSTEM_AOI_FOLDER_TEXT_REPORT://文字報告資料夾
		String = SysParam.m_AOITextReportFolder;
		break;
	case SYSTEM_AOI_FOLDER_ONLINE_TUNING://線上調機資料夾
		String = SysParam.m_OnlineTuningFolder;		
		break;
	case SYSTEM_AOI_FOLDER_ONLINE_BARCODE://線上條碼資料夾
		String = SysParam.m_OnlineBarcodeFolder;		
		break;
	case SYSTEM_AOI_FOLDER_ONLINE_OFFLINE://線上離線編程資料夾	
		String = SysParam.m_OnlineOfflineFolder;
		break;
	case SYSTEM_AOI_FOLDER_PROJECT_TEST_MAP://專案檢測底圖資料夾
		String = SysParam.m_ProjectTestMapFolder;
		break;
	case SYSTEM_AOI_FOLDER_PROJECT_TEST_MAP_TEMP://專案檢測底圖資料夾-暫存
		String = SysParam.m_ProjectTestMapFolderTemp;
		break;
	case SYSTEM_AOI_FOLDER_PROJECT_TEST_MAP_BACKUP://專案檢測底圖資料夾-備份
		String = SysParam.m_ProjectTestMapFolderBackup;
		break;
	case SYSTEM_AOI_FOLDER_PROJECT_RAW://專案原圖資料夾
		String = SysParam.m_ProjectRawFolder;		
		break;
	case SYSTEM_AOI_FOLDER_PROJECT_DEBUG://專案除錯資料夾
		String = SysParam.m_ProjectDebugFolder;		
		break;
	case SYSTEM_AOI_FOLDER_MONITOR_STATUS://監控狀態資料夾
		String = SysParam.m_MonitorStatusFolder;		
		break;
	case SYSTEM_AOI_FOLDER_CUSTOMER_LOG://客戶訊息資料夾
		String = SysParam.m_CustomerLogFolder;
		break;
	case SYSTEM_AOI_FOLDER_CUSTOMER_REPORT://客戶報告資料夾			
		String = SysParam.m_CustomerReportFolder;		
		break;
	case SYSTEM_AOI_FOLDER_BARCODE_FILE_LA://條碼檔案資料夾-A軌
		String = SysParam.m_BarcodeFileFolder_LA;
		break;
	case SYSTEM_AOI_FOLDER_BARCODE_FILE_LB://條碼檔案資料夾-B軌	
		String = SysParam.m_BarcodeFileFolder_LB;
		break;
	case SYSTEM_AOI_FOLDER_AI_FILE_EXPORT://AI檔案輸出資料夾
		String = SysParam.m_AIFileExportFolder;
		break;
	case SYSTEM_AOI_FOLDER_AI_IMAGE_EXPORT://AI影像輸出資料夾
		String = SysParam.m_AIImageExportFolder;
		break;
	case SYSTEM_AOI_FOLDER_TEST_TRACK://檢測追蹤資料夾
		String = SysParam.m_AOITestTrackFolder;
		break;
	case SYSTEM_AOI_LIBRARY_HOST://資料庫-本機
		String = SysParam.m_LibraryHost;		
		break;
	case SYSTEM_AOI_LIBRARY_REMOTE://資料庫-遠端
		String = SysParam.m_LibraryRemote;
		break;	
	case SYSTEM_IP_HOST_COMPUTER://本機網路網址	
		String = SysParam.m_IPHostComputer;		
		break;
	case SYSTEM_MACHINE_LOCATION://本機廠區	
		String = SysParam.m_MachineLocation;		
		break;
	case SYSTEM_MACHINE_BUILDING://本機棟別
		String = SysParam.m_MachineBuilding;		
		break;
	case SYSTEM_MACHINE_FLOOR://本機樓層	
		String = SysParam.m_MachineFloor;		
		break;
	case SYSTEM_MACHINE_ROOM://本機車間
		String = SysParam.m_MachineRoom;		
		break;
	case SYSTEM_MACHINE_LINE://本機線別
		String = SysParam.m_MachineLine;		
		break;
	case SYSTEM_MACHINE_LINE_LB://本機線別-B軌
		String = SysParam.m_MachineLine_LB;			
		break;
	case SYSTEM_MACHINE_STATION://本機站別		
		String = SysParam.m_MachineStation;		
		break;	
	case SYSTEM_MACHINE_STATION_LB://本機站別-B軌
		String = SysParam.m_MachineStation_LB;		
		break;
	case SYSTEM_MACHINE_SN://本機序號
		String = SysParam.m_MachineSN;
		break;
	case SYSTEM_MACHINE_NAME://機台型號
		String = SysParam.m_MachineName;
		break;
	case SYSTEM_MACHINE_VENDOR://機台廠商
		String = SysParam.m_MachineVendor;
		break;
	case SYSTEM_MACHINE_ALIAS://機台別名
		String = SysParam.m_MachineAlias;
		break;
	case SYSTEM_MACHINE_MES_NAME://MES登入名稱
		String = SysParam.m_MachineMES_Name;
		break;
	case SYSTEM_MACHINE_MES_PASSWORD://MES登入密碼
		String = SysParam.m_MachineMES_Password;
		break;
	case SYSTEM_MACHINE_MES_DEVICE://MES登入裝置
		String = SysParam.m_MachineMES_Device;
		break;
	case SYSTEM_MACHINE_MES_DEVICE_2://MES登入裝置-2
		String = SysParam.m_MachineMES_Device2;
		break;
	case SYSTEM_MACHINE_MES_CODE_NAME://MES-裝置代號
		String = SysParam.m_MachineMES_CodeName;
		break;
		
	case SYSTEM_AOI_CUSTOMER_ID://客戶編號
		String.Format(_T("%d"), SysParam.m_AOICustomerID);		
		break;
	case SYSTEM_CPU_MAX_COUNT_USED://Cpu最多使用數量
		String.Format(_T("%d"), SysParam.m_CpuMaxCountUsed);		
		break;
	case SYSTEM_THREAD_CNT_SLICE_FILL://多執行緒數量-畫面填圖
		String.Format(_T("%d"), SysParam.m_MTCount_SliceFill);		
		break;
	case SYSTEM_THREAD_CNT_FRAME_MERGE://多執行緒數量-影像合併
		String.Format(_T("%d"), SysParam.m_MTCount_FrameMerge);		
		break;
	case SYSTEM_THREAD_CNT_FIELD_MERGE://多執行緒數量-區域合併
		String.Format(_T("%d"), SysParam.m_MTCount_FieldMerge);		
		break;
	case SYSTEM_THREAD_CNT_FRAME_LOAD://多執行緒數量-影像載入
		String.Format(_T("%d"), SysParam.m_MTCount_FrameLoad);		
		break;
	case SYSTEM_THREAD_CNT_REGION_CALC://多執行緒數量-區域計算
		String.Format(_T("%d"), SysParam.m_MTCount_RegionCalc);		
		break;
	case SYSTEM_THREAD_CNT_PROC_IDLE://多執行緒數量-閒置核心	
		String.Format(_T("%d"), SysParam.m_MTCount_ProcIdle);		
		break;
	case SYSTEM_THREAD_CNT_GRAB_IDLE://多執行緒數量-取像閒置核心	
		String.Format(_T("%d"), SysParam.m_MTCount_GrabIdle);		
		break;
	case SYSTEM_OPEN_MP_CNT_GENERAL://OpenMP核心數量-一般操作
		String.Format(_T("%d"), SysParam.m_OpenMPCount_General);		
		break;
	case SYSTEM_OPEN_MP_CNT_INSPECTION://OpenMP核心數量-檢測中	
		String.Format(_T("%d"), SysParam.m_OpenMPCount_Inspection);
		break;
	case SYSTEM_OPEN_MP_CHK_SIZE_INSPECTION://OpenMP確認尺寸-檢測中
		String.Format(_T("%d"), SysParam.m_OpenMPCheckSize_Inspection);
		break;
	case SYSTEM_SAVE_MSG_MOVING_TIME://是否移動時間狀態訊息
		String.Format(_T("%d"), SysParam.m_SaveMovingTimeMessage);		
		break;
	case SYSTEM_BACKUP_MSG_MOVING_TIME://是否備份移動時間狀態訊息	
		String.Format(_T("%d"), SysParam.m_BackupMovingTimeMessage);		
		break;
	case SYSTEM_SAVE_MSG_CURRENT_PROCESS://是否儲存現在狀態訊息
		String.Format(_T("%d"), SysParam.m_SaveCurrentProcessMessage);
		break;		
	case SYSTEM_BACKUP_MSG_CURRENT_PROCESS://是否備份現在狀態訊息
		String.Format(_T("%d"), SysParam.m_BackupCurrentProcessMessage);
		break;
	case SYSTEM_SAVE_MSG_MACHINE_MONITOR://是否儲存機台監控訊息
		String.Format(_T("%d"), SysParam.m_SaveMachineMonitorMessage);
		break;
	case SYSTEM_SAVE_MSG_MACHINE_MONITOR_WEB://是否儲存機台監控訊息-網頁用
		String.Format(_T("%d"), SysParam.m_SaveMachineMonitorMessageWeb);
		break;
	case SYSTEM_SAVE_CAMERA_ADD_RING_BUFFER_LOG://是否儲存相機增加循環資料訊息
		String.Format(_T("%d"), SysParam.m_SaveCameraAddRingBufferLog);
		break;
	case SYSTEM_SHOW_MEMORY_LEAK_MESSAGE://顯示記憶體未釋放訊息
		String.Format(_T("%d"), SysParam.m_ShowMemoryLeakMessage);		
		break;		
	case SYSTEM_SEND_DEBUG_VIEW_STRING://是否送至DebugView視窗
		String.Format(_T("%d"), SysParam.m_SendDebugViewString);
		break;
	case SYSTEM_SAVE_SLICE_FILL_THREAD_LOG://是否儲存影像填滿執行緒訊息
		String.Format(_T("%d"), SysParam.m_SaveSliceFillThreadLog);
		break;
	case SYSTEM_SAVE_FRAME_MERGE_THREAD_LOG://是否儲存影像合併執行緒訊息
		String.Format(_T("%d"), SysParam.m_SaveFrameMergeThreadLog);
		break;
	case SYSTEM_SAVE_FIELD_MERGE_THREAD_LOG://是否儲存區域合併執行緒訊息
		String.Format(_T("%d"), SysParam.m_SaveFieldMergeThreadLog);
		break;
	case SYSTEM_SAVE_FRAME_LOAD_THREAD_LOG://是否儲存影像載入執行緒訊息
		String.Format(_T("%d"), SysParam.m_SaveFrameLoadThreadLog);
		break;
	case SYSTEM_SAVE_REGION_CALC_THREAD_LOG://是否儲存區域計算執行緒訊息
		String.Format(_T("%d"), SysParam.m_SaveRegionCalcThreadLog);
		break;
	case SYSTEM_SAVE_SQUENCE_THREAD_LOG://是否儲存系列執行緒訊息
		String.Format(_T("%d"), SysParam.m_SaveSequenceThreadLog);
		break;
	case SYSTEM_SAVE_FIELD_FRAME_RELEASE_THREAD_LOG://是否儲存視野影像釋放執行緒訊息
		String.Format(_T("%d"), SysParam.m_SaveFieldFrameReleaseThreadLog);
		break;
	case SYSTEM_SAVE_ONLINE_INSPECTION_THREAD_LOG://是否儲存線上檢測執行緒訊息
		String.Format(_T("%d"), SysParam.m_SaveOnlineInspectionThreadLog);
		break;
	case SYSTEM_SAVE_REMOVE_FOLDER_THREAD_LOG://是否儲存刪除資料夾執行緒訊息
		String.Format(_T("%d"), SysParam.m_SaveRemoveFolderThreadLog);
		break;
	case SYSTEM_SAVE_CONVEYER_PRE_RUN_THREAD_LOG://是否儲存軌道自動運轉執行緒訊息
		String.Format(_T("%d"), SysParam.m_SaveConveyerPreRunThreadLog);
		break;
	case SYSTEM_SAVE_ITS_PROC_THREAD_LOG://是否儲存ITS運作執行緒訊息
		String.Format(_T("%d"), SysParam.m_SaveITSProcThreadLog);
		break;
	case SYSTEM_SAVE_LOAD_REPAIR_FILE_THREAD_LOG://是否儲存載入維修站檔案執行緒訊息
		String.Format(_T("%d"), SysParam.m_SaveLoadRepairFileThreadLog);
		break;
	case SYSTEM_SAVE_REPAIR_RESULT_SIGNAL_THREAD_LOG://是否儲存維修站訊息執行緒訊息
		String.Format(_T("%d"), SysParam.m_SaveRepairResultSignalThreadLog);
		break;
	case SYSTEM_SAVE_SIMPLE_JOB_THREAD_LOG://是否儲存簡易作業執行緒訊息
		String.Format(_T("%d"), SysParam.m_SaveSimpleJobThreadLog);
		break;
	case SYSTEM_SAVE_HASI_MONITOR_THREAD_LOG://是否儲存HASI執行緒訊息
		String.Format(_T("%d"), SysParam.m_SaveHASIMonitorThreadLog);
		break;
	case SYSTEM_SAVE_TEST_OBJECT_FINISH_LOG://是否儲存檢測物件結束訊息
		String.Format(_T("%d"), SysParam.m_SaveTestObjectFinishLog);
		break;
	case SYSTEM_SAVE_UI_DRAW_FUNC_LOG://是否儲存介面重繪函式訊息
		String.Format(_T("%d"), SysParam.m_SaveUIDrawFuncLog);
		break;	
	case SYSTEM_SAVE_USER_OPERATION_LOG://是否儲存使用者操作訊息
		String.Format(_T("%d"), SysParam.m_SaveUserOperationLog);
		break;
	case SYSTEM_SAVE_TEST_TRACK_FILE: //是否儲存檢測追蹤檔案
		String.Format(_T("%d"), SysParam.m_SaveTestTrackFile);
		break;
	case SYSTEM_SAVE_PROJECT_REPORT_TEXT://是否專案文字檔報告
		String.Format(_T("%d"), SysParam.m_SaveProjectReportText);
		break;
	case SYSTEM_SAVE_PROJECT_REPORT_TEXT_FILENAME_MODE://儲存專案文字檔報告檔名
		String.Format(_T("%d"), SysParam.m_SaveProjectReportTextFilenameMode);
		break;
	case SYSTEM_SAVE_CUSTOMER_REPORT_FILE://是否儲存客戶報告模式
		String.Format(_T("%d"), SysParam.m_SaveCustomerReportFile);				
		break;
	case SYSTEM_SAVE_PROJECT_SPC_FILE_MODE://儲存專案SPC檔案模式
		String.Format(_T("%d"), SysParam.m_SaveProjectSpcFileMode);
		break;
	case SYSTEM_SAVE_PROJECT_SPC_LIBRARY_MODE://儲存專案SPC資料庫模式
		String.Format(_T("%d"), SysParam.m_SaveProjectSpcLibraryMode);
		break;
	case SYSTEM_SAVE_PROJECT_REPORT_WND_READING://是否儲存專案檢測框數據檔案
		String.Format(_T("%d"), SysParam.m_SaveProjectReportWndReading);
		break;		
	case SYSTEM_PRE_LOAD_PROJECT_OFFLINE_IMAGE://提前載入專案離線圖檔
		String.Format(_T("%d"), SysParam.m_PreLoadProjectOfflineImage);
		break;
	case SYSTEM_AUTO_RELEASE_FIELD_FRAME_BUFFER://自動釋放區域影像
		String.Format(_T("%d"), SysParam.m_AutoReleaseFieldFrameBuffer);
		break;
	case SYSTEM_AUTO_RELEASE_OFFLINE_FIELD_FRAME_BUFFER://自動釋放離線編程區域影像		
		String.Format(_T("%d"), SysParam.m_AutoReleaseOfflineFieldFrameBuffer);
		break;
	case SYSTEM_MACHINE_MODEL_TYPE://設備機種樣式
		String.Format(_T("%d"), SysParam.m_MachineModelType);
		break;
	case SYSTEM_MACHINE_CAMERA_SIDE://設備相機方向
		String.Format(_T("%d"), SysParam.m_MachineCameraSide);
		break;	
	case SYSTEM_MULTI_LANGUAGE_MODE://多國語系版本
		String.Format(_T("%d"), SysParam.m_MultiLanguageMode);
		break;	
	case SYSTEM_CAMERA_DEBAYER_MODE://影像還元彩色模式
		String.Format(_T("%d"), SysParam.m_DebayerMode);		
		break;
	case SYSTEM_DC_STRECTCH_BLT_MODE://繪圖模式
		String.Format(_T("%d"), SysParam.m_StretchBltMode);		
		break;	
	case SYSTEM_IMAGE_DISPLAY_MODE://影像顯示模式
		String.Format(_T("%d"), SysParam.m_ImageDisplayMode);		
		break;
	case SYSTEM_IMAGE_DISPLAY_ENHANCE_MODE://影像顯示強化模式
		String.Format(_T("%d"), SysParam.m_ImageDisplayEnhanceMode);
		break;
	case SYSTEM_IMAGE_DISPLAY_GAIN://影像顯示的Gain
		String.Format(_T("%.16f"), SysParam.m_ImageDisplayGain);		
		break;
	case SYSTEM_IMAGE_DISPLAY_GAMMA://影像顯示的Gamma
		String.Format(_T("%.16f"), SysParam.m_ImageDisplayGamma);
		break;		
	case SYSTEM_IMAGE_DISPLAY_SHARPNESS_RADIUS://影像顯示的銳利化的半徑
		String.Format(_T("%d"), SysParam.m_ImageDisplaySharpnessRadius);
		break;
	case SYSTEM_IMAGE_DISPLAY_SHARPNESS_AMOUNT://影像顯示的銳利化的總量
		String.Format(_T("%d"), SysParam.m_ImageDisplaySharpnessAmount);
		break;
	case SYSTEM_IMAGE_DISPLAY_SHARPNESS_THRESHOLD://影像顯示的銳利化的閥值
		String.Format(_T("%d"), SysParam.m_ImageDisplaySharpnessThreshold);
		break;		
	case SYSTEM_IMAGE_DISPLAY_LOCAL_GAMMA_CALC_SIZE://影像顯示的局部Gamma的計算範圍		
		String.Format(_T("%d"), SysParam.m_ImageDisplayLocalGammaCalcSize);
		break;
	case SYSTEM_IMAGE_DISPLAY_LOCAL_GAMMA_SCALE_VAL://影像顯示的局部Gamma的縮放比例
		String.Format(_T("%.2f"), SysParam.m_ImageDisplayLocalGammaScaleVal);
		break;		
	case SYSTEM_IMAGE_DISPLAY_MAX_ZOOM_SCALE://影像顯示最大縮放比例	
		String.Format(_T("%.2f"), SysParam.m_ImageDisplayMaxZoomScale);
		break;
	case SYSTEM_ONLINE_FORM_VIEW_MODE://線上檢測介面模式	
		String.Format(_T("%d"), SysParam.m_OnlineFormViewMode);
		break;
	case SYSTEM_CALC_FOCUS_SMOOTH_SIZE://計算焦距平滑尺寸
		String.Format(_T("%d"), SysParam.m_CalcFocusSmoothSize);
		break;
	case SYSTEM_CALC_FOCUS_MODE://計算焦距模式
		String.Format(_T("%d"), SysParam.m_CalcFocusMode);
		break;		
	case SYSTEM_LIBRARY_MODE_MATCH://影像匹配函式庫樣式
		String.Format(_T("%d"), SysParam.m_MatchLibType);		
		break;
	case SYSTEM_HONEYWELL_SWIFT_DECODER_ENABLED://Honeywell SwiftDecoder啟用
		String.Format(_T("%d"), SysParam.m_HoneywellSwiftDecoderEnabled);		
		break;
	case SYSTEM_DONGLE_WARNING_REMAINING_DAYS://硬體鎖警告剩餘天數
		String.Format(_T("%d"), SysParam.m_DongleWarningRemainingDays);		
		break;
	case SYSTEM_DONGLE_WARNING_REMAINING_COUNT://硬體鎖警告剩餘次數
		String.Format(_T("%d"), SysParam.m_DongleWarningRemainingCount);		
		break;
	case SYSTEM_CONNECT_LAST_STATION_MODE://與上一站連線方式
		String.Format(_T("%d"), SysParam.m_LastStationLineMode);		
		break;	
	case SYSTEM_LANE_WORK_MODEL_LA://A軌道運轉模式
		String.Format(_T("%d"), SysParam.m_LaneWorkMode_LA);
		break;
	case SYSTEM_LANE_WORK_MODEL_LB://B軌道運轉模式
		String.Format(_T("%d"), SysParam.m_LaneWorkMode_LB);
		break;
	case SYSTEM_MULTI_TOWER_LIGHT_MODE://多塔燈模式		
		String.Format(_T("%d"), SysParam.m_MultiTowerLight);
		break;	
	case SYSTEM_CHECK_PCB_REMOVED_COUNT://確認PCB板移走次數
		String.Format(_T("%d"), SysParam.m_CheckPCBRemovedCount);
		break;
	case SYSTEM_NEXT_CONNECTED_BUFFER_TYPE://確認PCB下一站連接輸送帶樣式
		String.Format(_T("%d"), SysParam.m_NextConnectedBufferType);
		break;
	case SYSTEM_MULTI_PROJECT_TEST_ORDER_MODE://多專案檢測次序模式	
		String.Format(_T("%d"), SysParam.m_MultiProjectTestOrderMode);		
		break;
	case SYSTEM_ONLINE_INPUT_PROJECT_WORK_NUMBER://在線輸入-專案工單號碼
		String.Format(_T("%d"), SysParam.m_OnlineInputProjectWorkNumber);
		break;
	case SYSTEM_ONLINE_AUTO_CALIBRATION_DLP_LED_COLOR://在線自動校正DLP-LED-顏色
		String.Format(_T("%d"), SysParam.m_OnlineAutoCalibrationDlpLedColor);
		break;
	case SYSTEM_ONLINE_AUTO_CALIBRATION_CAP_DELAY_TIME://在線自動校正上蓋延遲時間-ms
		String.Format(_T("%d"), SysParam.m_OnlineAutoCalibrationCapDelayTime);
		break;
	case SYSTEM_ONLINE_AUTO_CALIBRATION_MODE_XYZ_HOME://在線自動校正模式-XYZ歸零
		String.Format(_T("%d"), SysParam.m_OnlineAutoCalibrationMode_XYZHome);
		break;
	case SYSTEM_ONLINE_AUTO_CALIBRATION_PERIOD_XYZ_HOME://在線自動校正週期-XYZ歸零-小時
		String.Format(_T("%d"), SysParam.m_OnlineAutoCalibrationPeriod_XYZHome);
		break;
	case SYSTEM_ONLINE_AUTO_CALIBRATION_MODE_2D_CURRENT://在線自動校正模式-2D電流
		String.Format(_T("%d"), SysParam.m_OnlineAutoCalibrationMode_2DCurrent);
		break;
	case SYSTEM_ONLINE_AUTO_CALIBRATION_PERIOD_2D_CURRENT://在線自動校正週期-2D電流-小時
		String.Format(_T("%d"), SysParam.m_OnlineAutoCalibrationPeriod_2DCurrent);
		break;
	case SYSTEM_ONLINE_AUTO_CALIBRATION_PERIOD_3D_CURRENT://在線自動校正週期-3D電流-小時
		String.Format(_T("%d"), SysParam.m_OnlineAutoCalibrationPeriod_3DCurrent);
		break;
	case SYSTEM_ONLINE_AUTO_CALIBRATION_MODE_3D_ZERO_PLANE://在線自動校正模式-3D相平面
		String.Format(_T("%d"), SysParam.m_OnlineAutoCalibrationMode_3DZeroPlane);
		break;
	case SYSTEM_ONLINE_AUTO_CALIBRATION_PERIOD_3D_ZERO_PLANE://在線自動校正週期-3D相平面-小時
		String.Format(_T("%d"), SysParam.m_OnlineAutoCalibrationPeriod_3DZeroPlane);
		break;
	case SYSTEM_ONLINE_AUTO_CALIBRATION_PERIOD_3D_HEIGHT_FACTOR://在線自動校正週期-3D高度比例-小時
		String.Format(_T("%d"), SysParam.m_OnlineAutoCalibrationPeriod_3DHeightFactor);
		break;
	case SYSTEM_ONLINE_AUTO_STOP_BY_IDLE_TIME://在線自動停機-閒置時間-分鐘
		String.Format(_T("%d"), SysParam.m_OnlineAutoStopByIdleTime);
		break;
	case SYSTEM_ONLINE_AUTO_STOP_BY_SPEC_TIME_1://在線自動停機-特定時間-時時分分秒秒
		String.Format(_T("%I64d"), SysParam.m_OnlineAutoStopBySpecTime1);
		break;
	case SYSTEM_ONLINE_AUTO_STOP_BY_SPEC_TIME_2://在線自動停機-特定時間-時時分分秒秒
		String.Format(_T("%I64d"), SysParam.m_OnlineAutoStopBySpecTime2);
		break;
	case SYSTEM_ONLINE_AUTO_STOP_BY_SPEC_TIME_3://在線自動停機-特定時間-時時分分秒秒
		String.Format(_T("%I64d"), SysParam.m_OnlineAutoStopBySpecTime3);
		break;
	case SYSTEM_AUTO_SWITCH_TO_ONILINEVIEW_TIME://自動切換線上畫面-秒	
		String.Format(_T("%d"), SysParam.m_AutoSwitchToOnlineViewTime);	
		break;
	case SYSTEM_ONLINE_SHOW_PROJECT_TEST_MAP://在線顯示專案檢測底圖
		String.Format(_T("%d"), SysParam.m_OnlineShowProjectTestMap);	
		break;
	case SYSTEM_AUTO_SWITCH_TO_ONILINE_REMOTE_CTRL_TIME://自動切換線上遠端控制時間-毫秒
		String.Format(_T("%d"), SysParam.m_AutoSwitchToOnlineRemoteCtrlTime);
		break;
	case SYSTEM_AUTO_SWITCH_TO_ONILINE_REMOTE_CTRL_MODE://自動切換線上遠端控制模式
		String.Format(_T("%d"), SysParam.m_AutoSwitchToOnlineRemoteCtrlMode);
		break;
	case SYSTEM_PHASE_PERIOD_1://第1個相位的週期
		String.Format(_T("%.16f"), SysParam.m_PhasePeriod1);		
		break;
	case SYSTEM_PHASE_PERIOD_2://第2個相位的週期
		String.Format(_T("%.16f"), SysParam.m_PhasePeriod2);
		break;
	case SYSTEM_PHASE_PERIOD_3://第3個相位的週期
		String.Format(_T("%.16f"), SysParam.m_PhasePeriod3);
		break;		
	case SYSTEM_SEPARATE_DLP_2EXP_TABLE://分離DLP2次曝光表格
		String.Format(_T("%d"), SysParam.m_SeparateDLP2ExpTable);
		break;
	case SYSTEM_PHASE_CONVERT_HEIGHT_MODE://相位轉換高度模式
		String.Format(_T("%d"), SysParam.m_PhaseConvertHeightMode);
		break;
	case SYSTEM_PHASE_NOISE_DEFINE://相位雜訊定義啟用
		String.Format(_T("%d"), SysParam.m_PhaseNoiseDefine);		
		break;
	case SYSTEM_PHASE_NOISE_DEFINE_MODE://相位雜訊定義模式
		String.Format(_T("%d"), SysParam.m_PhaseNoiseDefineMode);		
		break;
	case SYSTEM_PHASE_NOISE_EXTEND_VOID://相位遮罩-無效點外擴
		String.Format(_T("%d"), SysParam.m_PhaseNoiseExtendVoid);		
		break;	
	case SYSTEM_PHASE_NOISE_LOW_CONTRAST://相位遮罩-低對比
		String.Format(_T("%d"), SysParam.m_PhaseNoiseLowContrastA);		
		break;	
	case SYSTEM_PHASE_NOISE_LOW_POTENTIAL://相位遮罩-低潛力
		String.Format(_T("%d"), SysParam.m_PhaseNoiseLowPotentialA);
		break;
	case SYSTEM_PHASE_NOISE_OVER_SATURATED://相位遮罩-過亮度	
		String.Format(_T("%d"), SysParam.m_PhaseNoiseOverSaturatedA);
		break;		
	case SYSTEM_PHASE_NOISE_LOW_CONTRAST_B://相位遮罩-低對比		
		String.Format(_T("%d"), SysParam.m_PhaseNoiseLowContrastB);
		break;
	case SYSTEM_PHASE_NOISE_LOW_POTENTIAL_B://相位遮罩-低潛力	
		String.Format(_T("%d"), SysParam.m_PhaseNoiseLowPotentialB);
		break;
	case SYSTEM_PHASE_NOISE_OVER_SATURATED_B://相位遮罩-過亮度	
		String.Format(_T("%d"), SysParam.m_PhaseNoiseOverSaturatedB);
		break;
	case SYSTEM_PHASE_NOISE_LOW_CONTRAST_C://相位遮罩-低對比		
		String.Format(_T("%d"), SysParam.m_PhaseNoiseLowContrastC);
		break;
	case SYSTEM_PHASE_NOISE_LOW_POTENTIAL_C://相位遮罩-低潛力	
		String.Format(_T("%d"), SysParam.m_PhaseNoiseLowPotentialC);
		break;
	case SYSTEM_PHASE_NOISE_OVER_SATURATED_C://相位遮罩-過亮度	
		String.Format(_T("%d"), SysParam.m_PhaseNoiseOverSaturatedC);
		break;			
	case SYSTEM_PHASE_NOISE_SMOOTH_FILTER://相位遮罩-平滑處理
		String.Format(_T("%d"), SysParam.m_PhaseNoiseSmoothFilter);		
		break;
	case SYSTEM_PHASE_NOISE_LOW_CONTRAST_COLOR://相位遮罩-低對比-顏色
		String.Format(_T("%d"), SysParam.m_PhaseNoiseLowContrastColor);
		break;
	case SYSTEM_PHASE_NOISE_LOW_POTENTIAL_COLOR://相位遮罩-潛力-顏色
		String.Format(_T("%d"), SysParam.m_PhaseNoiseLowPotentialColor);		
		break;	
	case SYSTEM_PHASE_NOISE_OVER_SATURATED_COLOR://相位遮罩-過亮度-顏色
		String.Format(_T("%d"), SysParam.m_PhaseNoiseOverSaturatedColor);		
		break;	
	case SYSTEM_PHASE_NOISE_EXTEND_VOID_COLOR://相位遮罩-無效點外擴-顏色
		String.Format(_T("%d"), SysParam.m_PhaseNoiseExtendVoidColor);		
		break;	
	case SYSTEM_SPACE_NOISE_HEIGHT_UNEXPECTED_COLOR://相位遮罩-高度異常-顏色
		String.Format(_T("%d"), SysParam.m_SpaceNoiseHeightUnexpectedColor);		
		break;
	case SYSTEM_SPACE_NOISE_HEIGHT_OVER_LOW_COLOR://相位遮罩-過低異常-顏色
		String.Format(_T("%d"), SysParam.m_SpaceNoiseHeightOverLowColor);
		break;	
	case SYSTEM_SPACE_VALID_BEST_COLOR://相位遮罩-可靠高度-顏色	
		String.Format(_T("%d"), SysParam.m_SpaceBestValidColor);
		break;
		
	case SYSTEM_3D_OBJECT_DRAW_SCALE_X://3D物件顯示比例-X	
		String.Format(_T("%.4f"), SysParam.m_3DObjectDrawScaleX);
		break;
	case SYSTEM_3D_OBJECT_DRAW_SCALE_Y://3D物件顯示比例-Y
		String.Format(_T("%.4f"), SysParam.m_3DObjectDrawScaleY);
		break;
	case SYSTEM_3D_OBJECT_DRAW_SCALE_Z://3D物件顯示比例-Z
		String.Format(_T("%.4f"), SysParam.m_3DObjectDrawScaleZ);
		break;

	case SYSTEM_PANEL_COLOR_1://整板的顏色-1
		String.Format(_T("%d"), SysParam.m_PanelColor1);
		break;
	case SYSTEM_PANEL_COLOR_2://整板的顏色-2	
		String.Format(_T("%d"), SysParam.m_PanelColor2);
		break;
	case SYSTEM_PANEL_TEXT_COLOR://整板文字顏色	
		String.Format(_T("%d"), SysParam.m_PanelTextColor);
		break;
	case SYSTEM_PANEL_SELECTED_COLOR://整板選取到的顏色
		String.Format(_T("%d"), SysParam.m_PanelSelectedColor);
		break;
	case SYSTEM_BOARD_COLOR_1://單板的顏色-1
		String.Format(_T("%d"), SysParam.m_BoardColor1);
		break;
	case SYSTEM_BOARD_COLOR_2://單板的顏色-2	
		String.Format(_T("%d"), SysParam.m_BoardColor2);
		break;
	case SYSTEM_BOARD_TEXT_COLOR://單板文字顏色	
		String.Format(_T("%d"), SysParam.m_BoardTextColor);
		break;
	case SYSTEM_BOARD_SELECTED_COLOR://單板選取到的顏色
		String.Format(_T("%d"), SysParam.m_BoardSelectedColor);
		break;			
	case SYSTEM_FD_COLOR_1://定位點的顏色-1
		String.Format(_T("%d"), SysParam.m_FdColor1);
		break;
	case SYSTEM_FD_COLOR_2://定位點的顏色-2		
		String.Format(_T("%d"), SysParam.m_FdColor2);
		break;
	case SYSTEM_FD_TEXT_COLOR://定位點文字顏色
		String.Format(_T("%d"), SysParam.m_FdTextColor);
		break;
	case SYSTEM_FD_SELECTED_COLOR://定位點選取到的顏色
		String.Format(_T("%d"), SysParam.m_FdSelectedColor);
		break;
	case SYSTEM_BARCODE_COLOR_1://條碼的顏色-1
		String.Format(_T("%d"), SysParam.m_BarcodeColor1);
		break;
	case SYSTEM_BARCODE_COLOR_2://條碼的顏色-2		
		String.Format(_T("%d"), SysParam.m_BarcodeColor2);
		break;
	case SYSTEM_BARCODE_TEXT_COLOR://條碼文字顏色
		String.Format(_T("%d"), SysParam.m_BarcodeTextColor);
		break;
	case SYSTEM_BARCODE_SELECTED_COLOR://條碼選取到的顏
		String.Format(_T("%d"), SysParam.m_BarcodeSelectedColor);
		break;
	case SYSTEM_COMPONENT_COLOR_1://零件的顏色-1
		String.Format(_T("%d"), SysParam.m_ComponentColor1);
		break;
	case SYSTEM_COMPONENT_COLOR_2://零件的顏色-2
		String.Format(_T("%d"), SysParam.m_ComponentColor2);
		break;		
	case SYSTEM_COMPONENT_TEXT_COLOR://零件文字顏色
		String.Format(_T("%d"), SysParam.m_ComponentTextColor);
		break;
	case SYSTEM_COMPONENT_SELECTED_COLOR://零件選取到的顏色
		String.Format(_T("%d"), SysParam.m_ComponentSelectedColor);
		break;
	case SYSTEM_DISTRICT_COLOR_1://分段的顏色-1
		String.Format(_T("%d"), SysParam.m_DistrictColor1);
		break;
	case SYSTEM_DISTRICT_COLOR_2://分段的顏色-2
		String.Format(_T("%d"), SysParam.m_DistrictColor2);
		break;
	case SYSTEM_INSPECTED_RESULT_OK_COLOR://檢測OK顏色
		String.Format(_T("%d"), SysParam.m_InspectedResultOKColor);
		break;
	case SYSTEM_INSPECTED_RESULT_NG_COLOR://檢測NG顏色
		String.Format(_T("%d"), SysParam.m_InspectedResultNGColor);
		break;
	case SYSTEM_INSPECTED_RESULT_SKIP_COLOR://檢測Skip顏色
		String.Format(_T("%d"), SysParam.m_InspectedResultSkipColor);
		break;
	case SYSTEM_INSPECTED_RESULT_BYPASS_COLOR://檢測Bypass顏色
		String.Format(_T("%d"), SysParam.m_InspectedResultBypassColor);
		break;
	case SYSTEM_INSPECTED_RESULT_UNTEST_COLOR://檢測UnTest顏色
		String.Format(_T("%d"), SysParam.m_InspectedResultUnTestColor);
		break;
	case SYSTEM_INSPECTED_RESULT_WARNING_COLOR://檢測Warning顏色		
		String.Format(_T("%d"), SysParam.m_InspectedResultWarningColor);
		break;
	case SYSTEM_INSPECTED_RESULT_EXCEPTION_COLOR://檢測Exception顏色
		String.Format(_T("%d"), SysParam.m_InspectedResultExceptionColor);
		break;

	case SYSTEM_SPACE_NOISE_SINGLE_CAST_LOW_LIMIT://高度雜訊單投光高度最低極限-um
		String.Format(_T("%.16f"), SysParam.m_SpaceNoiseSingleCastLowLimit);		
		break;		
	case SYSTEM_SPACE_NOISE_MULTI_CAST_PATCH_SIZE://高度雜訊多投光合併Patch尺寸
		String.Format(_T("%d"), SysParam.m_SpaceNoiseMultiCastPatchSize);
		break;
	case SYSTEM_SPACE_NOISE_MULTI_CAST_MERGE_MODE://高度雜訊多投光合併模式
		String.Format(_T("%d"), SysParam.m_SpaceNoiseMultiCastMergeMode);
		break;
	case SYSTEM_SPACE_NOISE_MULTI_CAST_MERGE_BEST_MODE://高度雜訊多投光合併最可靠模式
		String.Format(_T("%d"), SysParam.m_SpaceNoiseMultiCastMergeBestMode);
		break;
	case SYSTEM_SPACE_NOISE_MULTI_INTENSITY_MERGE_MODE://高度雜訊多亮度合併模式-Mean, MaxB
		String.Format(_T("%d"), SysParam.m_SpaceNoiseMultiIntensityMergeMode);
		break;
	case SYSTEM_SPACE_NOISE_MULTI_CAST_MIN_VALID_COUNT://高度雜訊多投光最少有效值數	
		String.Format(_T("%d"), SysParam.m_SpaceNoiseMultiCastMinValidCount);
		break;
	case SYSTEM_SPACE_NOISE_MULTI_CAST_MAX_DIFFERENCE://高度雜訊多投光高度最大差值-um
		String.Format(_T("%.16f"), SysParam.m_SpaceNoiseMultiCastMaxDifference);		
		break;	
	case SYSTEM_SPACE_NOISE_MULTI_CAST_LIMIT_DIFFERENCE://高度雜訊多投光高度極限差值-um
		String.Format(_T("%.16f"), SysParam.m_SpaceNoiseMultiCastLimitDifference);		
		break;
	case SYSTEM_SPACE_NOISE_MULTI_CAST_VALID_BEST_RATIO://高度雜訊多投光高度最好比例-um	
		String.Format(_T("%.16f"), SysParam.m_SpaceNoiseMultiCastValidBestRatio);		
		break;
	case SYSTEM_SPACE_NOISE_MULTI_CAST_VALID_DIFFERENCE://高度雜訊多投光高度有效差值-um
		String.Format(_T("%.16f"), SysParam.m_SpaceNoiseMultiCastValidDifference);		
		break;
	case SYSTEM_SPACE_NOISE_MULTI_CAST_OPPOSITE_MAX_GRAY://高度雜訊多投光合併對邊灰階上限-gray	
		String.Format(_T("%d"), SysParam.m_SpaceNoiseMultiCastOppositeMaxGray);		
		break;	
	case SYSTEM_SPACE_NOISE_CAST_FILTER_MODE://投光後濾波模式
		String.Format(_T("%d"), SysParam.m_SpaceNoiseCastFilterMode);
		break;
	case SYSTEM_SPACE_NOISE_CAST_MEDIAN_FILTER_SIZE://高度雜訊的中值濾波尺寸
		String.Format(_T("%d"), SysParam.m_SpaceNoiseCastMedianFilterSize);		
		break;
	case SYSTEM_SPACE_NOISE_CAST_MEDIAN_FILTER_USE_SIZE://高度雜訊的中值濾波使用尺寸
		String.Format(_T("%d"), SysParam.m_SpaceNoiseCastMedianFilterUseSize);		
		break;	
	case SYSTEM_SPACE_NOISE_DATA_VOID_EXPAND_SIZE://空間雜訊無效點外擴尺寸-piexel	
		String.Format(_T("%d"), SysParam.m_SpaceNoiseDataVoidExpandSize);		
		break;		
	case SYSTEM_SPACE_NOISE_DATA_VOID_EXPAND_ENABLED://空間雜訊無效點外擴啟用
		String.Format(_T("%d"), SysParam.m_SpaceNoiseDataVoidExpandEnabed);		
		break;
	case SYSTEM_SPACE_NOISE_FIRST_FILTER_MODE://空間雜訊首次濾波模式
		String.Format(_T("%d"), SysParam.m_SpaceNoiseFirstFilterMode);		
		break;		
	case SYSTEM_SPACE_NOISE_FIRST_FILTER_PITCH://空間雜訊首次濾波步長
		String.Format(_T("%d"), SysParam.m_SpaceNoiseFirstFilterPitch);		
		break;	
	case SYSTEM_SPACE_NOISE_FIRST_KER_SIZE://空間雜訊首次濾波尺寸
		String.Format(_T("%d"), SysParam.m_SpaceNoiseFirstKerSize);		
		break;		
	case SYSTEM_SPACE_NOISE_FIRST_USE_SIZE://空間雜訊首次使用尺寸
		String.Format(_T("%d"), SysParam.m_SpaceNoiseFirstUseSize);		
		break;
	case SYSTEM_SPACE_NOISE_OVER_LOW_MODE://高度雜訊過低模式
		String.Format(_T("%d"), SysParam.m_SpaceNoiseOverLowerMode);		
		break;	
	case SYSTEM_SPACE_NOISE_OVER_LOW_RANGE://高度雜訊過低高度-um
		String.Format(_T("%d"), SysParam.m_SpaceNoiseOverLowerRange);				
		break;			
	case SYSTEM_SPACE_NOISE_OVER_LOW_LIMIT://高度雜訊過低極限-um			
		String.Format(_T("%d"), SysParam.m_SpaceNoiseOverLowerLimit);
		break;
	case SYSTEM_SPACE_NOISE_OVER_LOW_KER_SIZE://高度雜訊過低濾波尺寸
		String.Format(_T("%d"), SysParam.m_SpaceNoiseOverLowerKerSize);		
		break;
	case SYSTEM_SPACE_NOISE_OVER_LOW_USE_SIZE://高度雜訊過低使用尺寸
		String.Format(_T("%d"), SysParam.m_SpaceNoiseOverLowerUseSize);		
		break;
	case SYSTEM_SPACE_NOISE_HEIGHT_ABNORMAL_MODE://高度雜訊高度異常模式
		String.Format(_T("%d"), SysParam.m_SpaceNoiseHeightAbnormalMode);		
		break;
	case SYSTEM_SPACE_NOISE_HEIGHT_ABNORMAL_PITCH://高度雜訊高度異常步長
		String.Format(_T("%d"), SysParam.m_SpaceNoiseHeightAbnormalPitch);		
		break;	
	case SYSTEM_SPACE_NOISE_HEIGHT_ABNORMAL_RANGE://高度雜訊高度異常範圍-um		
		String.Format(_T("%d"), SysParam.m_SpaceNoiseHeightAbnormalRange);		
		break;		
	case SYSTEM_SPACE_NOISE_HEIGHT_ABNORMAL_CHK_SIZE://高度雜訊高度異常確認尺寸
		String.Format(_T("%d"), SysParam.m_SpaceNoiseHeightAbnormalChkSize);		
		break;	
	case SYSTEM_SPACE_NOISE_HEIGHT_ABNORMAL_KER_SIZE://高度雜訊高度異常濾波尺寸
		String.Format(_T("%d"), SysParam.m_SpaceNoiseHeightAbnormalKerSize);		
		break;
	case SYSTEM_SPACE_NOISE_HEIGHT_ABNORMAL_USE_SIZE://高度雜訊高度異常使用尺寸
		String.Format(_T("%d"), SysParam.m_SpaceNoiseHeightAbnormalUseSize);		
		break;
	case SYSTEM_SPACE_NOISE_HEIGHT_ABNORMAL_REPEAT_CNT://高度雜訊高度異常重複次數	
		String.Format(_T("%d"), SysParam.m_SpaceNoiseHeightAbnormalRepeatCnt);		
		break;	
	case SYSTEM_SPACE_NOISE_RECONTRUCTED_EXT_SIZE://空間雜訊重建外擴尺寸
		String.Format(_T("%d"), SysParam.m_SpaceNoiseDataVoidReContructedExtSize);
		break;
	case SYSTEM_SPACE_NOISE_RECONTRUCTED_ENABLED://空間雜訊重建外擴啟用	
		String.Format(_T("%d"), SysParam.m_SpaceNoiseDataVoidReContructedEnabled);
		break;	
	case SYSTEM_SPACE_NOISE_FINAL_KER_SIZE://空間雜訊最後濾波尺寸
		String.Format(_T("%d"), SysParam.m_SpaceNoiseFinalKerSize);
		break;
	case SYSTEM_SPACE_NOISE_FINAL_USE_SIZE://空間雜訊最後使用尺寸
		String.Format(_T("%d"), SysParam.m_SpaceNoiseFinalUseSize);
		break;
	case SYSTEM_SPACE_NOISE_FINAL_FILTER_MODE://空間雜訊最後濾波模式
		String.Format(_T("%d"), SysParam.m_SpaceNoiseFinalFilterMode);
		break;		
	case SYSTEM_SPACE_NOISE_FINAL_FILTER_PITCH://空間雜訊最後濾波步長
		String.Format(_T("%d"), SysParam.m_SpaceNoiseFinalPitch);
		break;
	case SYSTEM_SPACE_NOISE_FINAL_FILTER_MODE2://空間雜訊最後濾波模式-2		
		String.Format(_T("%d"), SysParam.m_SpaceNoiseFinalFilterMode2);
		break;
	case SYSTEM_SPACE_NOISE_FINAL_FILTER_PITCH2://空間雜訊最後濾波步長-2	
		String.Format(_T("%d"), SysParam.m_SpaceNoiseFinalPitch2);
		break;
	case SYSTEM_SPACE_NOISE_FINAL_KER_SIZE2://空間雜訊最後濾波尺寸-2		
		String.Format(_T("%d"), SysParam.m_SpaceNoiseFinalKerSize2);
		break;
	case SYSTEM_SPACE_NOISE_FINAL_USE_SIZE2://空間雜訊最後使用尺寸-2		
		String.Format(_T("%d"), SysParam.m_SpaceNoiseFinalUseSize2);
		break;
	case SYSTEM_SPACE_VERIFY_MULTI_CAST_GAP_RATIO://高度驗證-多投光差距比例上限-%
		String.Format(_T("%.2f"), SysParam.m_SpaceVerifyMultiCastGapRatio);
		break;
	case SYSTEM_SPACE_VERIFY_MULTI_CAST_GAP_THRESHOLD://高度驗證-多投光差距閥值-um	
		String.Format(_T("%.2f"), SysParam.m_SpaceVerifyMultiCastGapThreshold);
		break;

	case SYSTEM_CUDA_FUNCTION_ENABLED://Cuda函式啟用
		String.Format(_T("%d"), SysParam.m_CudaFnEnabled);		
		break;
	case SYSTEM_CUDA_NUMBER_BLOCK://Cuda Block數量
		String.Format(_T("%d"), SysParam.m_CudaBlockNumber);
		break;	
	case SYSTEM_CUDA_NUMBER_THREAD://Cuda Thread數量
		String.Format(_T("%d"), SysParam.m_CudaThreadNumber);		
		break;			
	case SYSTEM_AUTO_RETRY_MAX_COUNT://自動重測次數上限
		String.Format(_T("%d"), SysParam.m_AutoRetryMaxCount);		
		break;
	case SYSTEM_AUTO_SETUP_DLP_PATTERN://自動設定DLP樣板	
		String.Format(_T("%d"), SysParam.m_AutoSetupDlpPattern);		
		break;
	case SYSTEM_RESOLUTION_MODE://解析度模式
		String.Format(_T("%d"), SysParam.m_ResolutionModeTmp);		
		break;
	case SYSTEM_RESOLUTION_SHOW_SCALE://解析度顯示比例
		String.Format(_T("%.2f"), SysParam.m_ResolutionShowScale);		
		break;
	case SYSTEM_MOVE_CAMERA_BEFORE_PCB_IN://進板前移動相機頭		
		String.Format(_T("%d"), SysParam.m_MoveCameraBeforePCBIn);		
		break;
	case SYSTEM_PCB_IN_USE_CAMERA_IMAGE_MODE://進板使用相機影像模式
		String.Format(_T("%d"), SysParam.m_PCBInUseCameraImageMode);
		break;
	case SYSTEM_CLAMP_PCB_BEFORE_TEST_MODE://檢測前夾板模式	
		String.Format(_T("%d"), SysParam.m_ClampPcbBeforeTestMode);
		break;
	case SYSTEM_GRAB_FIDUCIAL_DELAY_TIME://解取定位點影像延遲時間
		String.Format(_T("%d"), SysParam.m_GrabFiducialDelayTime_ms);		
		break;	
	case SYSTEM_PCB_OUT_DIRECTION://PCB出板方向
		String.Format(_T("%d"), SysParam.m_PCBOutDirection);	
		break;	
	case SYSTEM_BYPASS_LAST_SIGNAL://忽略上一站訊號-回板使用
		String.Format(_T("%d"), SysParam.m_BypassLastSignal);
		break;
	case SYSTEM_BYPASS_NEXT_SIGNAL://忽略下一站訊號-回板使用	
		String.Format(_T("%d"), SysParam.m_BypassNextSignal);		
		break;	
	case SYSTEM_PCB_OK_NG_SIGNAL_DELAY_TIME://PCB OK/NG訊號延遲時間-ms
		String.Format(_T("%d"), SysParam.m_PCBOKNGSignalDelayTime);		
		break;
	case SYSTEM_EDIT_LINE_SIZE_LEVEL://編輯線尺寸的層級
		String.Format(_T("%d"), SysParam.m_EditLineSizeLevel);	
		break;	
	case SYSTEM_ONLINE_TUNING_KEEP_MAX_TIME://線上調機保留最久時間-分鐘 	
		String.Format(_T("%d"), SysParam.m_OnlineTuningKeepMaxTime);	
		break;
	case SYSTEM_ONLINE_TUNING_SAVED_MAX_COUNT://線上調機儲存最多數量-片數 
		String.Format(_T("%d"), SysParam.m_OnlineTuningSavedMaxCount);	
		break;
	case SYSTEM_USER_LOGIN_ENABLED://使用者登入模式
		String.Format(_T("%d"), SysParam.m_UserLoginMode);	
		break;			
	case SYSTEM_USER_LOGIN_OPTIONS://使用者登入選項
		String.Format(_T("%d"), SysParam.m_UserLoginOptions);
		break;
	case SYSTEM_OPEN_PROJECT_MODE://開啟專案模式
		String.Format(_T("%d"), SysParam.m_OpenProjectMode);	
		break;
	case SYSTEM_OPEN_PROJECT_MAP_INDEX://開啟專案底圖編號
		String.Format(_T("%d"), SysParam.m_OpenProjectMapIndex);	
		break;	
	case SYSTEM_VERIFY_PROJECT_MODE://驗證專案模式
		String.Format(_T("%d"), SysParam.m_VerifyProjectMode);	
		break;
	case SYSTEM_VERIFY_PROJECT_FILENAME://驗證專案的檔名
		String.Format(_T("%s"), SysParam.m_VerifyProjectFilename);	
		break;
	case SYSTEM_MODEL_NAME_USE_PART_NUMBER://模組名稱使用料號 
		String.Format(_T("%d"), SysParam.m_ModelNameUsePartNumber);	
		break;
	case SYSTEM_ONLINE_OPEN_PROJECT_MODE://線上開專案模式
		String.Format(_T("%d"), SysParam.m_OnlineOpenProjectMode);	
		break;
	case SYSTEM_ONLINE_OPEN_PROJECT_CAMERA_BARCODE://線上開專案-相機條碼
		String.Format(_T("%d"), SysParam.m_OnlineOpenProjectCameraBarcode);	
		break;
	case SYSTEM_ONLINE_OPEN_PROJECT_BARCODE_DEVICE_GRAB_MODE://線上開專案外接條碼取像模式
		String.Format(_T("%d"), SysParam.m_OnlineOpenProjectBarcodeDeviceGrabMode);	
		break;
	case SYSTEM_MODEL_UNSET_COLOR://模組未設定顏色
		String.Format(_T("%d"), SysParam.m_ModelUnsetColor);
		break;
	case SYSTEM_BINARY_MASK_COLOR_RGB://2值化遮罩-顏色-3色燈
		String.Format(_T("%d"), SysParam.m_BinaryMaskColorRGB);
		break;
	case SYSTEM_BINARY_MASK_COLOR_DEFAULT://2值化遮罩-顏色-預設		
		String.Format(_T("%d"), SysParam.m_BinaryMaskColorDefault);
		break;
	case SYSTEM_BINARY_MASK_COLOR_ALPHA://2值化遮罩-顏色-透明度		
		String.Format(_T("%d"), SysParam.m_BinaryMaskColorAlpha);
		break;		
	case SYSTEM_INSPECTION_FINISH_SHOW_RESULT_LIST://檢測結束顯示結果列表
		String.Format(_T("%d"), SysParam.m_InspectionFinishShowResultList);		
		break;
	case SYSTEM_MODEL_DEFAULT_WND_LEVEL://模組預設檢測框等級
		String.Format(_T("%d"), SysParam.m_ModelDefaultWndLevel);
		break;
	case SYSTEM_SWITCH_PROJECT_3D_FRAME://可切換至專案3D畫面
		String.Format(_T("%d"), SysParam.m_SwitchProject3DFrame);
		break;	
	case SYSTEM_AUTO_SWITCH_WND_3D_FRAME_MODE://自動切換至檢測框3D畫面模式模式
		String.Format(_T("%d"), SysParam.m_AutoSwitchWnd3DFrameMode);
		break;
	case SYSTEM_AUTO_SWITCH_WND_3D_FRAME_SIZE_LIMIT://自動切換至檢測框3D畫面尺寸上限-um	
		String.Format(_T("%d"), SysParam.m_AutoSwitchWnd3DFrameSizeLimit);
		break;	
	case SYSTEM_SHOW_COMPONENT_FULL_MAP://顯示零件-整板零件
		String.Format(_T("%d"), SysParam.m_ShowComponentFullMap);		
		break;	
	case SYSTEM_SHOW_COMPONENT_DEFECT_ONLY://僅顯示瑕疵零件(線上畫面)
		String.Format(_T("%d"), SysParam.m_ShowComponentDefectOnly);
		break;
	case SYSTEM_SHOW_DEBUG_FORM_VIEW://顯示除錯頁面 
		String.Format(_T("%d"), SysParam.m_ShowDebugFormView);		
		break;
	case SYSTEM_LEVEL_FILTER_SHIFT_ENABLED://分等濾波起點偏移啟用
		String.Format(_T("%d"), SysParam.m_LevelFilterShiftEnabled);
		break;
	case SYSTEM_MEDIAN_FILTER_SHIFT_ENABLED://中值濾波起點偏移啟用
		String.Format(_T("%d"), SysParam.m_MedianFilterShiftEnabled);
		break;	
	case SYSTEM_SMOOTH_FILTER_SHIFT_ENABLED://平滑濾波起點偏移啟用	
		String.Format(_T("%d"), SysParam.m_SmoothFilterShiftEnabled);		
		break;
	case SYSTEM_PYRAMID_MEDIAN_FILTER_SHIFT_ENABLED://金字塔中值起點偏移啟用
		String.Format(_T("%d"), SysParam.m_PyramidMedianFilterShiftEnabled);
		break;
	case SYSTEM_LEVEL_FILTER_PITCH_TOLERANCE://分等濾波步長可接受誤差
		String.Format(_T("%d"), SysParam.m_LevelFilterPitchTolerance);
		break;
	case SYSTEM_MEDIAN_FILTER_PITCH_TOLERANCE://中值濾波步長可接受誤差
		String.Format(_T("%d"), SysParam.m_MedianFilterPitchTolerance);
		break;	
	case SYSTEM_SMOOTH_FILTER_PITCH_TOLERANCE://平滑濾波步長可接受誤差
		String.Format(_T("%d"), SysParam.m_SmoothFilterPitchTolerance);		
		break;	
	case SYSTEM_PYRAMID_MEDIAN_FILTER_PITCH_TOLERANCE://金字塔中值濾波步長可接受誤差
		String.Format(_T("%d"), SysParam.m_PyramidMedianFilterPitchTolerance);
		break;
	case SYSTEM_GRR_OFFSET_RANDOM_VALUE://Grr偏移隨機補償值-nm
		String.Format(_T("%d"), SysParam.m_GrrOffsetRandomValue);		
		break;		
	case SYSTEM_GRR_AVERAGE_RESET_ENABLED://Grr平均值復歸-啟用-依據條碼比較
		String.Format(_T("%d"), SysParam.m_GrrAverageResetEnabled);		
		break;
	case SYSTEM_GRR_AVERAGE_RESET_MATCH_RATIO://Grr平均值復歸-匹配比例
		String.Format(_T("%.3f"), SysParam.m_GrrAverageResetMatchRatio);		
		break;
	case SYSTEM_GRR_SKEW_AVERAGE_ENB_VAL://Grr偏移角度平均啟用角度
		String.Format(_T("%.3f"), SysParam.m_GrrSkewAverageEnbVal);		
		break;
	case SYSTEM_GRR_SKEW_AVERAGE_START_GAP://Grr偏移角度平均開始差距-角度	
		String.Format(_T("%.3f"), SysParam.m_GrrSkewAverageStartGap);		
		break;
	case SYSTEM_GRR_SKEW_AVERAGE_WEIGHTING://Grr偏移角度平均權重 
		String.Format(_T("%d"), SysParam.m_GrrSkewAverageWeighting);		
		break;	
	case SYSTEM_GRR_OFFSET_AVERAGE_ENB_PXL://Grr偏移平均啟用像素
		String.Format(_T("%d"), SysParam.m_GrrOffsetAverageEnbPxl);		
		break;
	case SYSTEM_GRR_OFFSET_AVERAGE_START_GAP://Grr偏移平均開始差距-微米	
		String.Format(_T("%.3f"), SysParam.m_GrrOffsetAverageStartGap);
		break;
	case SYSTEM_GRR_OFFSET_AVERAGE_WEIGHTING://Grr偏移平均權重
		String.Format(_T("%d"), SysParam.m_GrrOffsetAverageWeighting);		
		break;		
	case SYSTEM_GRR_HEIGHT_AVERAGE_ENB_VAL://Grr偏移高度平均啟用高度
		String.Format(_T("%.3f"), SysParam.m_GrrHeightAverageEnbVal);		
		break;
	case SYSTEM_GRR_HEIGHT_AVERAGE_START_GAP://Grr偏移高度平均開始差距-微米
		String.Format(_T("%.3f"), SysParam.m_GrrHeightAverageStartGap);
		break;
	case SYSTEM_GRR_HEIGHT_AVERAGE_WEIGHTING://Grr偏移高度平均權重 
		String.Format(_T("%d"), SysParam.m_GrrHeightAverageWeighting);	
		break;
	case SYSTEM_LOCK_SCREEN_ENABLED://啟用鎖住螢幕
		String.Format(_T("%d"), SysParam.m_LockScreenEnabled);		
		break;
	case SYSTEM_LOCK_SCREEN_KEY_ID://鎖住螢幕鍵號
		String.Format(_T("%d"), SysParam.m_LockScreenKeyID);		
		break;
	case SYSTEM_MULTI_DISTRICT_MODE_ENABLED://多段檢測啟用
		String.Format(_T("%d"), SysParam.m_MultiDistrictModeEnabled);		
		break;		
	case SYSTEM_ONLINE_TUNING_ENABLE_BARCODE://在線調機-軟體條碼
		String.Format(_T("%d"), SysParam.m_OnlineTuningEnableBarcode);		
		break;		
	case SYSTEM_SHOW_PLC_SAFTY_SETTING_UI://是否顯示PLC安全檢知設定介面
		String.Format(_T("%d"), SysParam.m_ShowPlcSaftySettingUI);
		break;
	case SYSTEM_SHOW_ALG_OFFSET_L_PARAM://是否顯示演算法OffsetL的參數 	
		String.Format(_T("%d"), SysParam.m_ShowAlgOffsetLParam);
		break;
	case SYSTEM_SHOW_ALG_OFFSET_A_PARAM://是否顯示演算法OffsetA的參數
		String.Format(_T("%d"), SysParam.m_ShowAlgOffsetAParam);
		break;
	case SYSTEM_SHOW_ALG_BRIGHT_RATIO_SCALE_PARAM://是否顯示演算法亮度比例的比例參數
		String.Format(_T("%d"), SysParam.m_ShowAlgBrightRatioScaleParam);
		break;
	case SYSTEM_SHOW_MODEL_PROPERTY_PARAM://是否顯示模組屬性參數
		String.Format(_T("%d"), SysParam.m_ShowModelPropertyParam);		
		break;		
	case SYSTEM_CONTINUE_PASTE_MODE://連續貼上模式
		String.Format(_T("%d"), SysParam.m_ContinuePasteMode);
		break;
	case SYSTEM_STITCH_IMAGE_PADDING_SIZE://拼圖參數-填補尺寸
		String.Format(_T("%d"), SysParam.m_StitchImagePaddingSize);
		break;
	case SYSTEM_RESIN_HEIGHT_ALIGN_ENABLED://啟用Resin高度對齊-軍達3D對位
		String.Format(_T("%d"), SysParam.m_ResinHeightAlignEnabled);
		break;
	case SYSTEM_MODEL_IMAGE_CAD_OFFSET_ENABLED://啟用模組影像Cad偏移補償um
		String.Format(_T("%d"), SysParam.m_ModelImageCadOffsetEnabled);
		break;
	case SYSTEM_MODEL_DEFAULT_TRANSISTOR_TYPE://模組預設SOT樣式
		String.Format(_T("%d"), SysParam.m_ModelDefaultTransistorType);
		break;
	case SYSTEM_PROJECT_LOCAL_FOLDER_ENABLED://專案使用本機資料夾
		String.Format(_T("%d"), SysParam.m_ProjectLocalFolderEnabled);		
		break;
	case SYSTEM_PARTIAL_COPY_PROJECT_LIBRARY://部分複製專案資料庫	
		String.Format(_T("%d"), SysParam.m_PartialCopyProjectLibrary);		
		break;
	case SYSTEM_AUTO_ARRANGE_MODEL_BK_IMAGE_FILES://自動重整模組底圖檔案
		String.Format(_T("%d"), SysParam.m_AutoArrangeModelBkImageFiles);		
		break;
	case SYSTEM_AUTO_COPY_SPC_COMPONENT_IMAGE_FILES://自動複製SPC零件圖檔
		String.Format(_T("%d"), SysParam.m_AutoCopySpcComponentImageFiles);
		break;
	case SYSTEM_AUTO_BYPASS_GRAB_3D_FRAME://自動跳過3D影像	
		String.Format(_T("%d"), SysParam.m_AutoBypassGrab3DFrame);		
		break;
	case SYSTEM_CHECK_PROJECT_FD_READY://確認專案定位點狀態
		String.Format(_T("%d"), SysParam.m_CheckProjectFdReady);
		break;
	case SYSTEM_LOCK_MODEL_BODY_POSITION://鎖住模組本體的位置
		String.Format(_T("%d"), SysParam.m_LockModelBodyPosition);
		break;	
	case SYSTEM_MAX_UNCHECK_TEST_FILE_COUNT://最多未判定檢測檔案數 
		String.Format(_T("%d"), SysParam.m_MaxUncheckTestFileCount);
		break;
	case SYSTEM_CONFIRM_COMPONENT_BARCODE_ENABLED://啟用零件條碼確認	
		String.Format(_T("%d"), SysParam.m_ConfirmComponentBarcodeEnabled);
		break;
	case SYSTEM_SAVE_JPEG_QUALITY://儲存JPEG的質量(001~100)
		String.Format(_T("%d"), SysParam.m_SaveJpegQuality);
		break;
	case SYSTEM_COPY_HUGE_FILES_MODE://複製大量檔案模式
		String.Format(_T("%d"), SysParam.m_CopyHugeFilesMode);		
		break;
	case SYSTEM_OFFLINE_VERSION_MODE://離線版本模式
		String.Format(_T("%d"), SysParam.m_OfflineVersionMode);		
		break;	
	case SYSTEM_USE_PROJECT_SYSTEM_PARAM_MODE://使用專案系統參數模式		
		String.Format(_T("%d"), SysParam.m_UseProjectSystemParamMode);		
		break;

	case SYSTEM_UI_WND_FONT_ADD_SIZE://UI視窗字型增加大小
		String.Format(_T("%d"), SysParam.m_UIWndFontAddSize);
		break;
	case SYSTEM_UI_DOCK_WND_SLIDE_STEPS://UI駐停視窗滑動步長	
		String.Format(_T("%d"), SysParam.m_UIDockWndSlideSteps);		
		break;
	case SYSTEM_UI_ENABLE_PCB_OUT_BUTTON://UI啟用PCB出板按鈕
		String.Format(_T("%d"), SysParam.m_UIEnablePCBOutButton);
		break;
	
	case SYSTEM_RABBITMQ_SERVER_PORT://RabbitMQ伺服器Port
		String.Format(_T("%d"), SysParam.m_RabbitMQServerPort);
		break;
	case SYSTEM_RABBITMQ_IP_ADDRESS://RabbitMQ通訊網址
		String.Format(_T("%s"), SysParam.m_RabbitMQServerAddress);
		break;
	case SYSTEM_RABBITMQ_USER_NAME://RabbitMQ登錄名稱
		String.Format(_T("%s"), SysParam.m_RabbitMQUserName);
		break;
	case SYSTEM_RABBITMQ_PASSWORD://RabbitMQ登錄密碼
		String.Format(_T("%s"), SysParam.m_RabbitMQPassword);
		break;

	case SYSTEM_ITS_FILENAME://ITS軟體名稱
		String = SysParam.m_ITSFilename;
		break;
	case SYSTEM_ITS_COMMUNICATION_MODE://ITS通訊模式
		String.Format(_T("%d"), SysParam.m_ITSCommunicationMode);
		break;
	case SYSTEM_ITS_SOCKET_IP_PORT://ITS網路Port
		String.Format(_T("%d"), SysParam.m_ITSSocketIPPort);
		break;
	case SYSTEM_ITS_SOCKET_IP_ADDRESS://ITS網路網址
		String.Format(_T("%s"), SysParam.m_ITSSocketIPAddress);
		break;
	case SYSTEM_ITS_RABBITMQ_QUEUE_NAME_RECV://ITS訊息佇列接收名
		String.Format(_T("%s"), SysParam.m_ITSRabbitMQRecvQueueName);
		break;
	case SYSTEM_ITS_RABBITMQ_QUEUE_NAME_SEND://ITS訊息佇列傳送名
		String.Format(_T("%s"), SysParam.m_ITSRabbitMQSendQueueName);
		break;		
	case SYSTEM_ITS_CONTACT_SOFTWARE://ITS-對接軟體			
		String.Format(_T("%d"), SysParam.m_ITSContactSoftware);
		break;
	case SYSTEM_ITS_COMMUNICATION_ENABLED://連線至ITS啟用
		String.Format(_T("%d"), SysParam.m_ITSCommunicationEnabled);		
		break;		
	case SYSTEM_ITS_COMMUNICATION_TIMEOUT_MS://與ITS溝通逾時(ms)	
		String.Format(_T("%d"), SysParam.m_ITSCommunicationTimeoutMS);		
		break;	
	case SYSTEM_ITS_FILE_FOLDER_SEND://ITS檔案資料夾-傳送
		String.Format(_T("%s"), SysParam.m_ITSFileFolderSend);
		break;
	case SYSTEM_ITS_FILE_FOLDER_RECV://ITS檔案資料夾-接收
		String.Format(_T("%s"), SysParam.m_ITSFileFolderRecv);
		break;
	case SYSTEM_ITS_FILE_BACKUP_ENABLED://ITS檔案資料夾-備份	
		String.Format(_T("%d"), SysParam.m_ITSFileBackupEnabled);
		break;
	case SYSTEM_ITS_FILE_USE_SYNC_FILE_ENABLED://ITS檔案資料夾-同步檔案
		String.Format(_T("%d"), SysParam.m_ITSFileUseSyncFileEnabled);
		break;	
	case SYSTEM_ITS_SET_SECS_GEM_ENABLED://ITS使用設定SECS/GEM
		String.Format(_T("%d"), SysParam.m_ITSSetSecsGemEnabled);		
		break;
	case SYSTEM_ITS_SECS_GEM_REMOTE_LOCAL: //ITS使用SECS/GEM-Remote Local
		String.Format(_T("%d"), SysParam.m_ITSSetSecsGemRemoteLocal);
		break;
	case SYSTEM_ITS_SET_SYSTEM_PARAM_ENABLED://ITS使用設定系統參數
		String.Format(_T("%d"), SysParam.m_ITSSetSystemParamEnabled);
		break;
	case SYSTEM_ITS_SET_PROJECT_PARAM_ENABLED://ITS使用設定專案參數
		String.Format(_T("%d"), SysParam.m_ITSSetProjectParamEnabled);
		break;
	case SYSTEM_ITS_SET_MACHINE_STATUS_ENABLED://ITS使用設定機台狀態
		String.Format(_T("%d"), SysParam.m_ITSSetMacineStatusEnabled);
		break;
	case SYSTEM_ITS_SET_APP_OPEN_CLOSE_ENABLED://ITS使用設定軟體開關
		String.Format(_T("%d"), SysParam.m_ITSSetAppOpnCloseEnabled);
		break;
	case SYSTEM_ITS_SET_PROCESS_ID_ENABLED://ITS使用設定程序編號
		String.Format(_T("%d"), SysParam.m_ITSSetProcessIDEnabled);
		break;
	case SYSTEM_ITS_SET_USER_LOGIN_OUT_ENABLED://ITS使用設定使用者登入登出
		String.Format(_T("%d"), SysParam.m_ITSSetUserLogin_outEnabled);
		break;	
	case SYSTEM_PROG_DEFAULT_SPACE_TO_GRAY_RATIO_MODE://專案預設高度轉灰階比例 
		String.Format(_T("%d"), SysParam.m_DefaultSpaceToGrayRatioMode);		
		break;
	case SYSTEM_PROG_DEFAULT_SPACE_BASE_PLANE_INDEX://專案預設空間基準面編號 
		String.Format(_T("%d"), SysParam.m_DefaultSpaceBasePlaneIndex);
		break;
	case SYSTEM_PROG_DEFAULT_SPACE_NOISE_FILTER_INDEX://專案預設空間雜訊過濾編號
		String.Format(_T("%d"), SysParam.m_DefaultSpaceNoiseFilterIndex);
		break;
	case SYSTEM_PROG_DEFAULT_ENABLE_CONVEYER_PRE_RUN://專案預設軌道提前運轉功能
		String.Format(_T("%d"), SysParam.m_DefaultEnableConveyerPreRun);		
		break;
	case SYSTEM_PROG_DEFAULT_FD_NG_HANDLE_MODE://專案預設定位點異常處理模式
		String.Format(_T("%d"), SysParam.m_DefaultFdNGHandleMode);		
		break;
	case SYSTEM_PROG_DEFAULT_BOARD_FD_GRAB_MODE://專案預設單板定位點取像模式
		String.Format(_T("%d"), SysParam.m_DefaultBoardFdGrabMode);		
		break;
	case SYSTEM_PROG_DEFAULT_BDEFECT_HANDLE_MODE://專案預設檢出異常處理模式
		String.Format(_T("%d"), SysParam.m_DefaultDefectHandleMode);	
		break;
	case SYSTEM_PROG_DEFAULT_PCB_OUT_MODE://專案預設PCB出板模式
		String.Format(_T("%d"), SysParam.m_DefaultPCBOutMode);	
		break;
	case SYSTEM_PROG_DEFAULT_PROJECT_SAVE_TEST_MAP://專案預設儲存檢測底圖模式
		String.Format(_T("%d"), SysParam.m_DefaultProjectSaveTestMap);
		break;
	case SYSTEM_PROG_DEFAULT_PROJECT_LINK_SERVER_MODE://專案預設連線伺服器模式	
		String.Format(_T("%d"), SysParam.m_DefaultProjectLinkServerMode);			
		break;
	case SYSTEM_PROG_DEFAULT_SAVE_OFFLINE_IMAGE_FILES://專案預設儲存離線圖檔
		String.Format(_T("%d"), SysParam.m_DefaultProjectSaveOfflineImageFiles);	
		break;
	case SYSTEM_PROG_DEFAULT_BARCODE_VERIFY_MODE://專案預設條碼驗證模式
		String.Format(_T("%d"), SysParam.m_DefaultBarcodeVerifyMode);
		break;
	case SYSTEM_PROG_DEFAULT_BARCODE_RETRIEVE_MODE://專案預設條碼查詢模式
		String.Format(_T("%d"), SysParam.m_DefaultBarcodeRetrieveMode);
		break;

	case SYSTEM_OPERATE_LEVEL_PROJECT_OPEN://操作等級-專案開啟
		String.Format(_T("%d"), SysParam.m_OperateLevelProjectOpen);
		break;
	case SYSTEM_OPERATE_LEVEL_PROJECT_SAVE://操作等級-專案儲存
		String.Format(_T("%d"), SysParam.m_OperateLevelProjectSave);
		break;
	case SYSTEM_OPERATE_LEVEL_PROJECT_PARAM://操作等級-專案參數
		String.Format(_T("%d"), SysParam.m_OperateLevelProjectParam);
		break;
	case SYSTEM_OPERATE_LEVEL_ONLINE_RUN://操作等級-線上運行
		String.Format(_T("%d"), SysParam.m_OperateLevelOnlineRun);	
		break;
	case SYSTEM_OPERATE_LEVEL_ONLINE_BYPASS://操作等級-線上直通
		String.Format(_T("%d"), SysParam.m_OperateLevelOnlineBypass);	
		break;
	case SYSTEM_OPERATE_LEVEL_ONLINE_STOP://操作等級-線上停止
		String.Format(_T("%d"), SysParam.m_OperateLevelOnlineStop);	
		break;
	case SYSTEM_OPERATE_LEVEL_ONLINE_SAVE_IMAGE://操作等級-線上存圖
		String.Format(_T("%d"), SysParam.m_OperateLevelOnlineSaveImage);
		break;
	case SYSTEM_OPERATE_LEVEL_ONLINE_UNLOCK://操作等級-線上解鎖
		String.Format(_T("%d"), SysParam.m_OperateLevelOnlineUnlock);
		break;	
	case SYSTEM_OPERATE_LEVEL_EDIT_FUNC_ADD://操作等級-編輯功能-新增
		String.Format(_T("%d"), SysParam.m_OperateLevelEditFuncAdd);
		break;
	case SYSTEM_OPERATE_LEVEL_EDIT_FUNC_DEL://操作等級-編輯功能-刪除
		String.Format(_T("%d"), SysParam.m_OperateLevelEditFuncDel);
		break;
	case SYSTEM_OPERATE_LEVEL_EDIT_FUNC_BYPASS://操作等級-編輯功能-不檢測
		String.Format(_T("%d"), SysParam.m_OperateLevelEditFuncBypass);
		break;	
	case SYSTEM_OPERATE_LEVEL_MES_CTRL_STATE_OFFLINE://操作等級-MES相關-控制狀態-離線
		String.Format(_T("%d"), SysParam.m_OperateLevelMesCtrlState_Offline);
		break;
	case SYSTEM_OPERATE_LEVEL_MES_CTRL_STATE_LOCAL://操作等級-MES相關-控制狀態-本地上線
		String.Format(_T("%d"), SysParam.m_OperateLevelMesCtrlState_Local);
		break;
	case SYSTEM_OPERATE_LEVEL_MES_CTRL_STATE_REMOTE://操作等級-MES相關-控制狀態-遠端上線
		String.Format(_T("%d"), SysParam.m_OperateLevelMesCtrlState_Remote);
		break;
	case SYSTEM_OPERATE_LEVEL_MES_SHOW_CONTEXT://操作等級-MES相關-顯示內容
		String.Format(_T("%d"), SysParam.m_OperateLevelMesShowContext);
		break;

	case SYSTEM_M2M_NPM_BARCODE_ENABLE://啟用NPM-條碼
		String.Format(_T("%d"), SysParam.m_M2M_NPM_Barcode_Enable);
		break;
	case SYSTEM_M2M_NPM_APC_FF1_ENABLE://啟用NPM APC-FF1
		String.Format(_T("%d"), SysParam.m_M2M_NPM_APC_FF1_Enable);
		break;
	case SYSTEM_M2M_NPM_APC_FF2_ENABLE://啟用NPM APC-FF2
		String.Format(_T("%d"), SysParam.m_M2M_NPM_APC_FF2_Enable);
		break;
	case SYSTEM_M2M_NPM_APC_MFB_ENABLE://啟用NPM APC-MFB
		String.Format(_T("%d"), SysParam.m_M2M_NPM_APC_MFB_Enable);		
		break;
	case SYSTEM_M2M_NPM_LANE_NAME_LA://NPM軌道名稱-A軌
		String.Format(_T("%s"), SysParam.m_M2M_NPM_LaneName_LA);
		break;
	case SYSTEM_M2M_NPM_LANE_NAME_LB://NPM軌道名稱-B軌
		String.Format(_T("%s"), SysParam.m_M2M_NPM_LaneName_LB);
		break;
	case SYSTEM_M2M_NPM_INPUT_SHARE_FOLDER_LA://NPM-輸入共享資料夾-A軌
		String.Format(_T("%s"), SysParam.m_M2M_NPM_InputShareFolder_LA);
		break;
	case SYSTEM_M2M_NPM_INPUT_SHARE_FOLDER_LB://NPM-輸入共享資料夾-B軌
		String.Format(_T("%s"), SysParam.m_M2M_NPM_InputShareFolder_LB);
		break;
	case SYSTEM_M2M_NPM_OUTPUT_SHARE_FOLDER_LA://NPM-輸出共享資料夾-A軌
		String.Format(_T("%s"), SysParam.m_M2M_NPM_OutputShareFolder_LA);		
		break;
	case SYSTEM_M2M_NPM_OUTPUT_SHARE_FOLDER_LB://NPM-輸出共享資料夾-B軌
		String.Format(_T("%s"), SysParam.m_M2M_NPM_OutputShareFolder_LB);
		break;

	case SYSTEM_M2M_HASI_ENABLE://啟用M2M HAS I (Hanwha AOI Solution MAOI_SAOI)
		String.Format(_T("%d"), SysParam.m_M2M_HASI_Enable);
		break;
	case SYSTEM_M2M_HASI_SHARE_FOLDER://HAS I 共享資料夾
		String.Format(_T("%s"), SysParam.m_M2M_HASI_ShareFolder);
		break;
	case SYSTEM_M2M_HASI_SERIALFILE_ENABLE://啟用 HAS I - SerialFile
		String.Format(_T("%d"), SysParam.m_M2M_HASI_SerialFile_Enable);
		break;
	case SYSTEM_M2M_HASI_SERIALFILE_QUEUESIZE://HAS I - SerialFile - Size of Serial Queue
		String.Format(_T("%d"), SysParam.m_M2M_HASI_SerialFile_QueueSize);
		break;
	case SYSTEM_M2M_HASI_SERIALFILE_DWELLTIME://等待資料的延遲時間-ms
		String.Format(_T("%d"), SysParam.m_M2M_HASI_SerialFile_DwellTime);
		break;
	case SYSTEM_M2M_HASI_SPIOFFSETFILE_ENABLE://啟用 HAS I - SPIOffsetFile
		String.Format(_T("%d"), SysParam.m_M2M_HASI_SPIOffsetFile_Enable);
		break;
	case SYSTEM_M2M_HASI_PNP_ENABLE://啟用 HAS I - PNP
		String.Format(_T("%d"), SysParam.m_M2M_HASI_PNP_Enable);
		break;
	case SYSTEM_M2M_HASI_AOI_STAGE://產線上 AOI 的檢查階段
		String.Format(_T("%d"), SysParam.m_M2M_HASI_AOI_Stage);
		break;
	case SYSTEM_M2M_HASI_STATE_MODE://HASI 狀態
		String.Format(_T("%d"), SysParam.m_M2M_HASI_StateMode);
		break;

	case SYSTEM_AI_MODEL_SERVER_ENABLE://AI模型伺服器啟用
		String.Format(_T("%d"), SysParam.m_AiModelServerEnabled);		
		break;
	case SYSTEM_AI_MODEL_SERVER_TIMEOUT://AI模型伺服器逾時
		String.Format(_T("%d"), SysParam.m_AiModelServerTimeout);		
		break;		
	case SYSTEM_AI_MODEL_SERVER_FILENAME://AI模型伺服器檔名
		String.Format(_T("%s"), SysParam.m_AiModelServerFilename);
		break;
	case SYSTEM_AI_MODEL_FILE_FOLDER_SEND://AI模型檔案資料夾-傳送
		String.Format(_T("%s"), SysParam.m_AiModelFileFolderSend);
		break;
	case SYSTEM_AI_MODEL_FILE_FOLDER_RECV://AI模型檔案資料夾-接收
		String.Format(_T("%s"), SysParam.m_AiModelFileFolderRecv);
		break;
	case SYSTEM_AI_MODEL_LABEL_MIN_CLUSTER_DISTANCE://AI模型分類最小叢集距離-um
		String.Format(_T("%.4f"), SysParam.m_AIModelLabelMinClusterDistance);
		break;

	case SYSTEM_EXTERNAL_COPY_FILE_ENABLED://外部複製檔案啟用
		String.Format(_T("%d"), SysParam.m_ExternalCopyFileEnabled);
		break;
	case SYSTEM_EXTERNAL_COPY_FILE_APP_NAME://外部複製檔案軟體名稱
		String = SysParam.m_ExternalCopyFileAppName;
		break;
	case SYSTEM_EXTERNAL_COPY_FILE_SEND_FOLDER://外部複製檔案輸出資料夾
		String = SysParam.m_ExternalCopyFileSendFolder;
		break;

	case SYSTEM_CPK_CHART_ENABLED://Cpk圖表啟用
		String.Format(_T("%d"), SysParam.m_CpkChartEnabled);
		break;
	case SYSTEM_WND_ROTATION_FOLLOWED://檢測框跟隨旋轉
		String.Format(_T("%d"), SysParam.m_WndRotationFollowed);
		break;

	default:	
		IsOK = false;
		Err.Format(_T("Error, No System Param Define [%d]"), ParamID);
		JetAPI::ShowMessageBox(Err);
		break;
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::SaveSystemParameter()//儲存系統參數
{	
	CString Filename;
	Filename = GetSystemParamFilename();
	if ( SaveSystemParamFile(Filename, m_SystemParameter) == false )
	{	return false;	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::SaveSystemParamFile(LPCTSTR filename, const TSystemParameter &Param)//儲存系統參數		
{
	if ( SaveSystemParamFileFn(filename, Param) == false )
	{
		SetSystemExceptionCode_FileWrite();
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::SaveSystemParamFileFn(LPCTSTR filename, const TSystemParameter &Param)//儲存系統參數
{
	if ( NULL == filename ) 
	{
		m_ErrorString = _T("Error, SaveSystemParamFile No Filename");
		return false;
	}
	int i=0;	
	CString str;
	CString Section;
	CString KeyName;
	CString String;
	CString FileName;	
	SYSTEM_PARAM_ID SysParam;
	
	FileName = filename;
	Section = _T("System Parameter");

	/*
	//Basic
	//CString                    m_AppVersion;//軟件版本

	//Multi-Threading
	SYSTEM_THREAD_CNT_FRAME_MERGE,//多執行緒數量-影像合併	
	SYSTEM_THREAD_CNT_FRAME_LOAD,//多執行緒數量-影像載入

	//Message
	
	//Phase相位 SYSTEM_PHASE_  SYSTEM_PHASE_NOISE_ 
	//Setting SYSTEM_	
	SYSTEM_CAMERA_DEBAYER_MODE,//影像還元彩色模式
	SYSTEM_CONNECT_LAST_STATION_MODE,//與上一站連線方式		
	SYSTEM_MOVE_CAMERA_BEFORE_PCB_IN,//進板前移動相機頭
	SYSTEM_CLAMP_PCB_BEFORE_TEST_MODE,//檢測前夾板模式	
	SYSTEM_GRAB_FIDUCIAL_DELAY_TIME,//解取定位點影像延遲時間	
	*/
	//系統資料夾
	SysParam = SYSTEM_AOI_FOLDER_HOST;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }

	//暫存的資料夾
	SysParam = SYSTEM_AOI_FOLDER_TEMP;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }
	
	//系統訊息資料夾
	SysParam = SYSTEM_AOI_FOLDER_LOG;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }

	//專案資料夾	
	SysParam = SYSTEM_AOI_FOLDER_PROJECT;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }

	//結果資料夾
	SysParam = SYSTEM_AOI_FOLDER_RESULT;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }

	//資料庫-伺服器資料夾		
	SysParam = SYSTEM_AOI_FOLDER_SERVER;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }

	//統計數據資料夾
	SysParam = SYSTEM_AOI_FOLDER_STATIC_DATA;
	if (SaveSystemParamNode(SysParam, Param, FileName) == false)
	{	return false; }

	//文字報告資料夾
	SysParam = SYSTEM_AOI_FOLDER_TEXT_REPORT;
	if (SaveSystemParamNode(SysParam, Param, FileName) == false)
	{	return false; }

	//線上調機資料夾
	SysParam = SYSTEM_AOI_FOLDER_ONLINE_TUNING;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	

	//線上條碼資料夾
	SysParam = SYSTEM_AOI_FOLDER_ONLINE_BARCODE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	

	//線上離線編程資料夾	
	SysParam = SYSTEM_AOI_FOLDER_ONLINE_OFFLINE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }		

	//專案檢測底圖資料夾
	SysParam = SYSTEM_AOI_FOLDER_PROJECT_TEST_MAP;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }		

	//專案檢測底圖資料夾-暫存
	SysParam = SYSTEM_AOI_FOLDER_PROJECT_TEST_MAP_TEMP;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }

	//專案檢測底圖資料夾-備份
	SysParam = SYSTEM_AOI_FOLDER_PROJECT_TEST_MAP_BACKUP;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }

	//專案原圖資料夾
	SysParam = SYSTEM_AOI_FOLDER_PROJECT_RAW;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	

	//專案除錯資料夾
	SysParam = SYSTEM_AOI_FOLDER_PROJECT_DEBUG;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	

	//監控狀態資料夾
	SysParam = SYSTEM_AOI_FOLDER_MONITOR_STATUS;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	

	//客戶訊息資料夾
	SysParam = SYSTEM_AOI_FOLDER_CUSTOMER_LOG;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	

	//客戶報告資料夾			
	SysParam = SYSTEM_AOI_FOLDER_CUSTOMER_REPORT;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	
		
	//條碼檔案資料夾-A軌		
	SysParam = SYSTEM_AOI_FOLDER_BARCODE_FILE_LA;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	

	//條碼檔案資料夾-B軌	
	SysParam = SYSTEM_AOI_FOLDER_BARCODE_FILE_LB;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	

	//AI檔案輸出資料夾
	SysParam = SYSTEM_AOI_FOLDER_AI_FILE_EXPORT;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	

	//AI影像輸出資料夾		
	SysParam = SYSTEM_AOI_FOLDER_AI_IMAGE_EXPORT;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	

	//檢測追蹤資料夾
	SysParam = SYSTEM_AOI_FOLDER_TEST_TRACK;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	

	//資料庫-本機
	SysParam = SYSTEM_AOI_LIBRARY_HOST;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }

	//資料庫-遠端
	SysParam = SYSTEM_AOI_LIBRARY_REMOTE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }

	//是否儲存移動時間
	SysParam = SYSTEM_SAVE_MSG_MOVING_TIME;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }

	//是否備份移動時間狀態訊息	
	SysParam = SYSTEM_BACKUP_MSG_MOVING_TIME;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }

	//是否儲存現在狀態訊息	
	SysParam = SYSTEM_SAVE_MSG_CURRENT_PROCESS;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }

	//是否備份現在狀態訊息
	SysParam = SYSTEM_BACKUP_MSG_CURRENT_PROCESS;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }

	//是否儲存機台監控訊息
	SysParam = SYSTEM_SAVE_MSG_MACHINE_MONITOR;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }

	//是否儲存機台監控訊息-網頁用
	SysParam = SYSTEM_SAVE_MSG_MACHINE_MONITOR_WEB;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }
		
	//是否儲存相機增加循環資料訊息
	SysParam = SYSTEM_SAVE_CAMERA_ADD_RING_BUFFER_LOG;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	

	//是否專案文字檔報告
	SysParam = SYSTEM_SAVE_PROJECT_REPORT_TEXT;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	

	//儲存專案文字檔報告檔名
	SysParam = SYSTEM_SAVE_PROJECT_REPORT_TEXT_FILENAME_MODE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }

	//是否儲存客戶報告
	SysParam = SYSTEM_SAVE_CUSTOMER_REPORT_FILE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	

	//儲存專案SPC檔案模式
	SysParam = SYSTEM_SAVE_PROJECT_SPC_FILE_MODE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }
		
	//儲存專案SPC資料庫模式		
	SysParam = SYSTEM_SAVE_PROJECT_SPC_LIBRARY_MODE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }

	//是否儲存專案檢測框數據檔案	
	SysParam = SYSTEM_SAVE_PROJECT_REPORT_WND_READING;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }

	//顯示記憶體未釋放訊息
	SysParam = SYSTEM_SHOW_MEMORY_LEAK_MESSAGE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	
	
	//是否送至DebugView視窗
	SysParam = SYSTEM_SEND_DEBUG_VIEW_STRING;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }

	//是否儲存影像填滿執行緒訊息
	SysParam = SYSTEM_SAVE_SLICE_FILL_THREAD_LOG;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }

	//是否儲存影像合併執行緒訊息
	SysParam = SYSTEM_SAVE_FRAME_MERGE_THREAD_LOG;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }

	//是否儲存區域合併執行緒訊息
	SysParam = SYSTEM_SAVE_FIELD_MERGE_THREAD_LOG;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }

	//是否儲存影像載入執行緒訊息
	SysParam = SYSTEM_SAVE_FRAME_LOAD_THREAD_LOG;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }

	//是否儲存區域計算執行緒訊息
	SysParam = SYSTEM_SAVE_REGION_CALC_THREAD_LOG;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }

	//是否儲存系列執行緒訊息
	SysParam = SYSTEM_SAVE_SQUENCE_THREAD_LOG;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }

	//是否儲存視野影像釋放執行緒訊息
	SysParam = SYSTEM_SAVE_FIELD_FRAME_RELEASE_THREAD_LOG;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }

	//是否儲存線上檢測執行緒訊息
	SysParam = SYSTEM_SAVE_ONLINE_INSPECTION_THREAD_LOG;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }

	//是否儲存刪除資料夾執行緒訊息
	SysParam = SYSTEM_SAVE_REMOVE_FOLDER_THREAD_LOG;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }

	//是否儲存軌道自動運轉執行緒訊息
	SysParam = SYSTEM_SAVE_CONVEYER_PRE_RUN_THREAD_LOG;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }

	//是否儲存ITS運作執行緒訊息
	SysParam = SYSTEM_SAVE_ITS_PROC_THREAD_LOG;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }

	//是否儲存載入維修站檔案執行緒訊息	
	SysParam = SYSTEM_SAVE_LOAD_REPAIR_FILE_THREAD_LOG;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }

	//是否儲存維修站訊息執行緒訊息	
	SysParam = SYSTEM_SAVE_REPAIR_RESULT_SIGNAL_THREAD_LOG;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }

	//是否儲存簡易作業執行緒訊息
	SysParam = SYSTEM_SAVE_SIMPLE_JOB_THREAD_LOG;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	
	
	//是否儲存HASI監視執行緒訊息
	SysParam = SYSTEM_SAVE_HASI_MONITOR_THREAD_LOG;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	

	//是否儲存檢測物件結束訊息
	SysParam = SYSTEM_SAVE_TEST_OBJECT_FINISH_LOG;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	
		
	//是否儲存介面重繪函式訊息
	SysParam = SYSTEM_SAVE_UI_DRAW_FUNC_LOG;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	

	//是否儲存使用者操作訊息
	SysParam = SYSTEM_SAVE_USER_OPERATION_LOG;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	

	//是否儲存檢測追蹤檔案
	SysParam = SYSTEM_SAVE_TEST_TRACK_FILE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	

	//提前載入專案離線圖檔
	SysParam = SYSTEM_PRE_LOAD_PROJECT_OFFLINE_IMAGE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }

	//自動釋放區域影像
	SysParam = SYSTEM_AUTO_RELEASE_FIELD_FRAME_BUFFER;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }

	//自動釋放離線編程區域影像				
	SysParam = SYSTEM_AUTO_RELEASE_OFFLINE_FIELD_FRAME_BUFFER;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }

	//本機網路網址	
	SysParam = SYSTEM_IP_HOST_COMPUTER;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }

	//機台廠區
	SysParam = SYSTEM_MACHINE_LOCATION;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }

	//機台棟別
	SysParam = SYSTEM_MACHINE_BUILDING;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }
		 
	//機台樓層
	SysParam = SYSTEM_MACHINE_FLOOR;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }
	
	//機台車間
	SysParam = SYSTEM_MACHINE_ROOM;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }

	//機台線名
	SysParam = SYSTEM_MACHINE_LINE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }
	
	//本機線別-B軌
	SysParam = SYSTEM_MACHINE_LINE_LB;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }

	//機台站名
	SysParam = SYSTEM_MACHINE_STATION;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	 

	//本機站別-B軌
	SysParam = SYSTEM_MACHINE_STATION_LB;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	 

	//本機序號
	SysParam = SYSTEM_MACHINE_SN;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	 		
	
	//機台型號
	SysParam = SYSTEM_MACHINE_NAME;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	
	
	//機台廠商		
	SysParam = SYSTEM_MACHINE_VENDOR;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	

	//機台別名
	SysParam = SYSTEM_MACHINE_ALIAS;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }		

	//MES登入名稱
	SysParam = SYSTEM_MACHINE_MES_NAME;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	

	//MES登入密碼
	SysParam = SYSTEM_MACHINE_MES_PASSWORD;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	

	//MES登入裝置
	SysParam = SYSTEM_MACHINE_MES_DEVICE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }		

	//MES登入裝置-2		
	SysParam = SYSTEM_MACHINE_MES_DEVICE_2;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }		

	//MES-裝置代號
	SysParam = SYSTEM_MACHINE_MES_CODE_NAME;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }		

	//客戶編號		
	SysParam = SYSTEM_AOI_CUSTOMER_ID;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	

	//設備機種樣式
	SysParam = SYSTEM_MACHINE_MODEL_TYPE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }

	//設備相機方向
	SysParam = SYSTEM_MACHINE_CAMERA_SIDE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }

	//多國語系版本
	SysParam = SYSTEM_MULTI_LANGUAGE_MODE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }

	//繪圖模式 
	SysParam = SYSTEM_DC_STRECTCH_BLT_MODE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }

	//影像還元彩色模式 
	SysParam = SYSTEM_CAMERA_DEBAYER_MODE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	
	
	//使用相位還原法
	//KeyName = _T("Phase Unwrapping Mode");
	//if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	//{	return false; }

	//影像顯示模式
	SysParam = SYSTEM_IMAGE_DISPLAY_MODE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }

	//影像顯示強化模式
	SysParam = SYSTEM_IMAGE_DISPLAY_ENHANCE_MODE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }

	//影像顯示Gain
	SysParam = SYSTEM_IMAGE_DISPLAY_GAIN;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	

	//影像顯示Gamma
	SysParam = SYSTEM_IMAGE_DISPLAY_GAMMA;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	
	
	//影像顯示的銳利化的半徑		
	SysParam = SYSTEM_IMAGE_DISPLAY_SHARPNESS_RADIUS;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	

	//影像顯示的銳利化的總量	
	SysParam = SYSTEM_IMAGE_DISPLAY_SHARPNESS_AMOUNT;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	

	//影像顯示的銳利化的閥值
	SysParam = SYSTEM_IMAGE_DISPLAY_SHARPNESS_THRESHOLD;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	

	//影像顯示的局部Gamma的計算範圍			
	SysParam = SYSTEM_IMAGE_DISPLAY_LOCAL_GAMMA_CALC_SIZE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	

	//影像顯示的局部Gamma的縮放比例
	SysParam = SYSTEM_IMAGE_DISPLAY_LOCAL_GAMMA_SCALE_VAL;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }		

	//影像顯示最大縮放比例	
	SysParam = SYSTEM_IMAGE_DISPLAY_MAX_ZOOM_SCALE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }		

	//線上檢測介面模式	
	SysParam = SYSTEM_ONLINE_FORM_VIEW_MODE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	

	//計算焦距平滑尺寸		
	SysParam = SYSTEM_CALC_FOCUS_SMOOTH_SIZE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	

	//計算焦距模式
	SysParam = SYSTEM_CALC_FOCUS_MODE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }		

	//自動重測次數上限
	SysParam = SYSTEM_AUTO_RETRY_MAX_COUNT;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }		

	//自動設定DLP樣板	
	SysParam = SYSTEM_AUTO_SETUP_DLP_PATTERN;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }		

	//解析度模式
	SysParam = SYSTEM_RESOLUTION_MODE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }		
	
	//解析度顯示比例
	SysParam = SYSTEM_RESOLUTION_SHOW_SCALE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }		
		
	//Cpu最多使用數量
	SysParam = SYSTEM_CPU_MAX_COUNT_USED;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }		

	//多執行緒數量-畫面填圖
	SysParam = SYSTEM_THREAD_CNT_SLICE_FILL;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }		

	//多執行緒數量-影像合併
	SysParam = SYSTEM_THREAD_CNT_FRAME_MERGE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }		 

	//多執行緒數量-區域合併		
	SysParam = SYSTEM_THREAD_CNT_FIELD_MERGE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	

	//多執行緒數量-影像載入
	SysParam = SYSTEM_THREAD_CNT_FRAME_LOAD;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }

	//多執行緒數量-區域計算 
	SysParam = SYSTEM_THREAD_CNT_REGION_CALC;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	

	//多執行緒數量-閒置核心	
	SysParam = SYSTEM_THREAD_CNT_PROC_IDLE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	

	//多執行緒數量-取像閒置核心			
	SysParam = SYSTEM_THREAD_CNT_GRAB_IDLE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	

	//OpenMP核心數量-一般操作
	SysParam = SYSTEM_OPEN_MP_CNT_GENERAL;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }		

	//OpenMP核心數量-檢測中	
	SysParam = SYSTEM_OPEN_MP_CNT_INSPECTION;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }		

	//OpenMP確認尺寸-檢測中		
	SysParam = SYSTEM_OPEN_MP_CHK_SIZE_INSPECTION;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }		

	//相位週期-1 
	SysParam = SYSTEM_PHASE_PERIOD_1;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }

	//相位週期-2
	SysParam = SYSTEM_PHASE_PERIOD_2;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }

	//相位週期-3
	SysParam = SYSTEM_PHASE_PERIOD_3;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }

	//分離DLP2次曝光表格
	SysParam = SYSTEM_SEPARATE_DLP_2EXP_TABLE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }

	//相位轉換高度模式		
	SysParam = SYSTEM_PHASE_CONVERT_HEIGHT_MODE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }

	//相位遮罩-無效點外擴
	SysParam = SYSTEM_PHASE_NOISE_EXTEND_VOID;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }

	//相位遮罩-低對比 
	SysParam = SYSTEM_PHASE_NOISE_LOW_CONTRAST;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }

	//相位遮罩-低潛能 
	SysParam = SYSTEM_PHASE_NOISE_LOW_POTENTIAL;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }

	//相位遮罩-過亮度
	SysParam = SYSTEM_PHASE_NOISE_OVER_SATURATED;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	
	
	//相位遮罩-低對比-B
	SysParam = SYSTEM_PHASE_NOISE_LOW_CONTRAST_B;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }

	//相位遮罩-低潛能-B
	SysParam = SYSTEM_PHASE_NOISE_LOW_POTENTIAL_B;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }

	//相位遮罩-過亮度-B
	SysParam = SYSTEM_PHASE_NOISE_OVER_SATURATED_B;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	

	//相位遮罩-低對比-C
	SysParam = SYSTEM_PHASE_NOISE_LOW_CONTRAST_C;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }

	//相位遮罩-低潛能-C
	SysParam = SYSTEM_PHASE_NOISE_LOW_POTENTIAL_C;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }

	//相位遮罩-過亮度-C
	SysParam = SYSTEM_PHASE_NOISE_OVER_SATURATED_C;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	
	
	//相位遮罩-平滑處理
	SysParam = SYSTEM_PHASE_NOISE_SMOOTH_FILTER;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	

	//相位遮罩-低對比-顏色
	SysParam = SYSTEM_PHASE_NOISE_LOW_CONTRAST_COLOR;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }

	//相位遮罩-低潛力-顏色
	SysParam = SYSTEM_PHASE_NOISE_LOW_POTENTIAL_COLOR;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }
		
	//相位遮罩-過亮度-顏色
	SysParam = SYSTEM_PHASE_NOISE_OVER_SATURATED_COLOR;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }
	
	//相位遮罩-無效點外擴-顏色
	SysParam = SYSTEM_PHASE_NOISE_EXTEND_VOID_COLOR;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }

	//相位遮罩-高度異常-顏色
	SysParam = SYSTEM_SPACE_NOISE_HEIGHT_UNEXPECTED_COLOR;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }

	//相位遮罩-過低異常-顏色
	SysParam = SYSTEM_SPACE_NOISE_HEIGHT_OVER_LOW_COLOR;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	

	//相位遮罩-可靠高度-顏色			
	SysParam = SYSTEM_SPACE_VALID_BEST_COLOR;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	
	
	//3D物件顯示比例-X				
	SysParam = SYSTEM_3D_OBJECT_DRAW_SCALE_X;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	

	//3D物件顯示比例-Y		
	SysParam = SYSTEM_3D_OBJECT_DRAW_SCALE_Y;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	

	//3D物件顯示比例-Z		
	SysParam = SYSTEM_3D_OBJECT_DRAW_SCALE_Z;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	

	//整板的顏色-1		
	SysParam = SYSTEM_PANEL_COLOR_1;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	

	//整板的顏色-2		
	SysParam = SYSTEM_PANEL_COLOR_2;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	

	//整板文字顏色		
	SysParam = SYSTEM_PANEL_TEXT_COLOR;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	

	//整板選取到的顏色	
	SysParam = SYSTEM_PANEL_SELECTED_COLOR;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	

	//單板的顏色-1		
	SysParam = SYSTEM_BOARD_COLOR_1;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	
	
	//單板的顏色-2
	SysParam = SYSTEM_BOARD_COLOR_2;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	

	//單板文字顏色	
	SysParam = SYSTEM_BOARD_TEXT_COLOR;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	
		
	//單板選取到的顏色
	SysParam = SYSTEM_BOARD_SELECTED_COLOR;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	
		
	//定位點的顏色-1
	SysParam = SYSTEM_FD_COLOR_1;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	

	//定位點的顏色-2		
	SysParam = SYSTEM_FD_COLOR_2;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	

	//定位點文字顏色
	SysParam = SYSTEM_FD_TEXT_COLOR;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	

	//定位點選取到的顏色		
	SysParam = SYSTEM_FD_SELECTED_COLOR;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	

	//條碼的顏色-1
	SysParam = SYSTEM_BARCODE_COLOR_1;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	

	//條碼的顏色-2		
	SysParam = SYSTEM_BARCODE_COLOR_2;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	

	//條碼文字顏色
	SysParam = SYSTEM_BARCODE_TEXT_COLOR;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	

	//條碼選取到的顏
	SysParam = SYSTEM_BARCODE_SELECTED_COLOR;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }		

	//零件的顏色-1
	SysParam = SYSTEM_COMPONENT_COLOR_1;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	

	//零件的顏色-2
	SysParam = SYSTEM_COMPONENT_COLOR_2;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }			

	//零件文字顏色
	SysParam = SYSTEM_COMPONENT_TEXT_COLOR;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	

	//零件選取到的顏色
	SysParam = SYSTEM_COMPONENT_SELECTED_COLOR;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	

	//分段的顏色-1
	SysParam = SYSTEM_DISTRICT_COLOR_1;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	

	//分段的顏色-2
	SysParam = SYSTEM_DISTRICT_COLOR_2;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }		

	//檢測OK顏色		
	SysParam = SYSTEM_INSPECTED_RESULT_OK_COLOR;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	

	//檢測NG顏色		
	SysParam = SYSTEM_INSPECTED_RESULT_NG_COLOR;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	

	//檢測Skip顏色		
	SysParam = SYSTEM_INSPECTED_RESULT_SKIP_COLOR;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	

	//檢測Bypass顏色		
	SysParam = SYSTEM_INSPECTED_RESULT_BYPASS_COLOR;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	

	//檢測UnTest顏色		
	SysParam = SYSTEM_INSPECTED_RESULT_UNTEST_COLOR;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false )	
	{	return false; }	

	//檢測Warning顏色				
	SysParam = SYSTEM_INSPECTED_RESULT_WARNING_COLOR;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//檢測Exception顏色
	SysParam = SYSTEM_INSPECTED_RESULT_EXCEPTION_COLOR;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	
	
	//相位雜訊定義
	SysParam = SYSTEM_PHASE_NOISE_DEFINE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//相位雜訊定義模式
	SysParam = SYSTEM_PHASE_NOISE_DEFINE_MODE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//高度雜訊單投光高度最低極限-um
	SysParam = SYSTEM_SPACE_NOISE_SINGLE_CAST_LOW_LIMIT;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//高度雜訊多投光合併Patch尺寸		
	SysParam = SYSTEM_SPACE_NOISE_MULTI_CAST_PATCH_SIZE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//高度雜訊多投光合併模式 
	SysParam = SYSTEM_SPACE_NOISE_MULTI_CAST_MERGE_MODE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//高度雜訊多投光合併最可靠模式
	SysParam = SYSTEM_SPACE_NOISE_MULTI_CAST_MERGE_BEST_MODE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//高度雜訊多亮度合併模式-Mean, MaxB
	SysParam = SYSTEM_SPACE_NOISE_MULTI_INTENSITY_MERGE_MODE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//高度雜訊多投光最少有效值數 
	SysParam = SYSTEM_SPACE_NOISE_MULTI_CAST_MIN_VALID_COUNT;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }		

	//高度雜訊多投光高度最大差值 
	SysParam = SYSTEM_SPACE_NOISE_MULTI_CAST_MAX_DIFFERENCE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }		

	//高度雜訊多投光高度極限差值-um
	SysParam = SYSTEM_SPACE_NOISE_MULTI_CAST_LIMIT_DIFFERENCE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }			

	//高度雜訊多投光高度最好比例-um	
	SysParam = SYSTEM_SPACE_NOISE_MULTI_CAST_VALID_BEST_RATIO;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }			

	//高度雜訊多投光高度有效差值-um
	SysParam = SYSTEM_SPACE_NOISE_MULTI_CAST_VALID_DIFFERENCE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }			

	//高度雜訊多投光合併對邊灰階上限-gray	
	SysParam = SYSTEM_SPACE_NOISE_MULTI_CAST_OPPOSITE_MAX_GRAY;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }			
		
	//投光後濾波模式
	SysParam = SYSTEM_SPACE_NOISE_CAST_FILTER_MODE;
	if (SaveSystemParamNode(SysParam, Param, FileName) == false)
	{
		return false;
	}

	//高度雜訊的中值濾波尺寸
	SysParam = SYSTEM_SPACE_NOISE_CAST_MEDIAN_FILTER_SIZE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }
	
	//高度雜訊的中值濾波使用尺寸
	SysParam = SYSTEM_SPACE_NOISE_CAST_MEDIAN_FILTER_USE_SIZE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//空間雜訊無效點外擴尺寸-piexel	
	SysParam = SYSTEM_SPACE_NOISE_DATA_VOID_EXPAND_SIZE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }		
	
	//空間雜訊無效點外擴啟用
	SysParam = SYSTEM_SPACE_NOISE_DATA_VOID_EXPAND_ENABLED;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//空間雜訊首次濾波模式
	SysParam = SYSTEM_SPACE_NOISE_FIRST_FILTER_MODE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	
	
	//空間雜訊首次濾波步長
	SysParam = SYSTEM_SPACE_NOISE_FIRST_FILTER_PITCH;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	
	
	//空間雜訊首次濾波尺寸
	SysParam = SYSTEM_SPACE_NOISE_FIRST_KER_SIZE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	
	
	//空間雜訊首次使用尺寸
	SysParam = SYSTEM_SPACE_NOISE_FIRST_USE_SIZE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//高度雜訊過低啟用	
	SysParam = SYSTEM_SPACE_NOISE_OVER_LOW_MODE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//高度雜訊過低高度-um 
	SysParam = SYSTEM_SPACE_NOISE_OVER_LOW_RANGE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//高度雜訊過低極限-um	
	SysParam = SYSTEM_SPACE_NOISE_OVER_LOW_LIMIT;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//高度雜訊過低濾波尺寸	
	SysParam = SYSTEM_SPACE_NOISE_OVER_LOW_KER_SIZE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//高度雜訊過低使用尺寸
	SysParam = SYSTEM_SPACE_NOISE_OVER_LOW_USE_SIZE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//高度雜訊高度異常模式
	SysParam = SYSTEM_SPACE_NOISE_HEIGHT_ABNORMAL_MODE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }		

	//高度雜訊高度異常步長
	SysParam = SYSTEM_SPACE_NOISE_HEIGHT_ABNORMAL_PITCH;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }		
	
	//高度雜訊高度異常範圍-um		
	SysParam = SYSTEM_SPACE_NOISE_HEIGHT_ABNORMAL_RANGE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }		
	
	//高度雜訊高度異常確認尺寸
	SysParam = SYSTEM_SPACE_NOISE_HEIGHT_ABNORMAL_CHK_SIZE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//高度雜訊高度異常濾波尺寸
	SysParam = SYSTEM_SPACE_NOISE_HEIGHT_ABNORMAL_KER_SIZE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//高度雜訊高度異常使用尺寸
	SysParam = SYSTEM_SPACE_NOISE_HEIGHT_ABNORMAL_USE_SIZE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }		
	
	//高度雜訊高度異常重複次數
	SysParam = SYSTEM_SPACE_NOISE_HEIGHT_ABNORMAL_REPEAT_CNT;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	
	
	//空間雜訊重建外擴尺寸
	SysParam = SYSTEM_SPACE_NOISE_RECONTRUCTED_EXT_SIZE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	
	
	//空間雜訊重建外擴啟用	
	SysParam = SYSTEM_SPACE_NOISE_RECONTRUCTED_ENABLED;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	
	
	//空間雜訊最後濾波尺寸
	SysParam = SYSTEM_SPACE_NOISE_FINAL_KER_SIZE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//空間雜訊最後遮罩尺寸
	SysParam = SYSTEM_SPACE_NOISE_FINAL_USE_SIZE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//空間雜訊最後平滑啟用	
	SysParam = SYSTEM_SPACE_NOISE_FINAL_FILTER_MODE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	
	
	//空間雜訊最後濾波步長
	SysParam = SYSTEM_SPACE_NOISE_FINAL_FILTER_PITCH;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	
	
	//空間雜訊最後濾波尺寸-2
	SysParam = SYSTEM_SPACE_NOISE_FINAL_KER_SIZE2;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//空間雜訊最後遮罩尺寸-2
	SysParam = SYSTEM_SPACE_NOISE_FINAL_USE_SIZE2;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//空間雜訊最後平滑啟用-2
	SysParam = SYSTEM_SPACE_NOISE_FINAL_FILTER_MODE2;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//空間雜訊最後濾波步長-2
	SysParam = SYSTEM_SPACE_NOISE_FINAL_FILTER_PITCH2;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//高度驗證-多投光差距比例上限-%
	SysParam = SYSTEM_SPACE_VERIFY_MULTI_CAST_GAP_RATIO;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//高度驗證-多投光差距閥值-um
	SysParam = SYSTEM_SPACE_VERIFY_MULTI_CAST_GAP_THRESHOLD;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//啟用Cuda函式 
	SysParam = SYSTEM_CUDA_FUNCTION_ENABLED;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//Cude Block數量
	SysParam = SYSTEM_CUDA_NUMBER_BLOCK;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//Cude Thread數量 
	SysParam = SYSTEM_CUDA_NUMBER_THREAD;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//影像匹配函式庫樣式 
	SysParam = SYSTEM_LIBRARY_MODE_MATCH;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }			

	//Honeywell SwiftDecoder啟用		
	SysParam = SYSTEM_HONEYWELL_SWIFT_DECODER_ENABLED;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//硬體鎖警告剩餘天數		
	SysParam = SYSTEM_DONGLE_WARNING_REMAINING_DAYS;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//硬體鎖警告剩餘次數
	SysParam = SYSTEM_DONGLE_WARNING_REMAINING_COUNT;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }		

	//與上一站連線方式
	SysParam = SYSTEM_CONNECT_LAST_STATION_MODE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//A軌道運轉模式
	SysParam = SYSTEM_LANE_WORK_MODEL_LA;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//B軌道運轉模式
	SysParam = SYSTEM_LANE_WORK_MODEL_LB;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//雙塔燈模式
	SysParam = SYSTEM_MULTI_TOWER_LIGHT_MODE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//確認PCB板移走次數
	SysParam = SYSTEM_CHECK_PCB_REMOVED_COUNT;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//確認PCB下一站連接輸送帶樣式		
	SysParam = SYSTEM_NEXT_CONNECTED_BUFFER_TYPE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//多專案檢測次序模式		
	SysParam = SYSTEM_MULTI_PROJECT_TEST_ORDER_MODE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//在線輸入-專案工單號碼
	SysParam = SYSTEM_ONLINE_INPUT_PROJECT_WORK_NUMBER;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//在線自動校正DLP-LED-顏色
	SysParam = SYSTEM_ONLINE_AUTO_CALIBRATION_DLP_LED_COLOR;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//在線自動校正上蓋延遲時間-ms
	SysParam = SYSTEM_ONLINE_AUTO_CALIBRATION_CAP_DELAY_TIME;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//在線自動校正模式-XYZ歸零
	SysParam = SYSTEM_ONLINE_AUTO_CALIBRATION_MODE_XYZ_HOME;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//在線自動校正週期-XYZ歸零-小時
	SysParam = SYSTEM_ONLINE_AUTO_CALIBRATION_PERIOD_XYZ_HOME;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//在線自動校正模式-2D電流
	SysParam = SYSTEM_ONLINE_AUTO_CALIBRATION_MODE_2D_CURRENT;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//在線自動校正週期-2D電流-小時		
	SysParam = SYSTEM_ONLINE_AUTO_CALIBRATION_PERIOD_2D_CURRENT;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//在線自動校正週期-3D電流-小時		
	SysParam = SYSTEM_ONLINE_AUTO_CALIBRATION_PERIOD_3D_CURRENT;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//在線自動校正模式-3D相平面		
	SysParam = SYSTEM_ONLINE_AUTO_CALIBRATION_MODE_3D_ZERO_PLANE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//在線自動校正週期-3D相平面-小時		
	SysParam = SYSTEM_ONLINE_AUTO_CALIBRATION_PERIOD_3D_ZERO_PLANE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//在線自動校正週期-3D高度比例-小時		
	SysParam = SYSTEM_ONLINE_AUTO_CALIBRATION_PERIOD_3D_HEIGHT_FACTOR;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//在線自動停機-閒置時間-分鐘		
	SysParam = SYSTEM_ONLINE_AUTO_STOP_BY_IDLE_TIME;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//在線自動停機-特定時間-時時分分秒秒		
	SysParam = SYSTEM_ONLINE_AUTO_STOP_BY_SPEC_TIME_1;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//在線自動停機-特定時間-時時分分秒秒		
	SysParam = SYSTEM_ONLINE_AUTO_STOP_BY_SPEC_TIME_2;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//在線自動停機-特定時間-時時分分秒秒
	SysParam = SYSTEM_ONLINE_AUTO_STOP_BY_SPEC_TIME_3;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//自動切換線上畫面-秒
	SysParam = SYSTEM_AUTO_SWITCH_TO_ONILINEVIEW_TIME;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//在線顯示專案檢測底圖
	SysParam = SYSTEM_ONLINE_SHOW_PROJECT_TEST_MAP;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//自動切換線上遠端控制時間-毫秒
	SysParam = SYSTEM_AUTO_SWITCH_TO_ONILINE_REMOTE_CTRL_TIME;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//自動切換線上遠端控制模式
	SysParam = SYSTEM_AUTO_SWITCH_TO_ONILINE_REMOTE_CTRL_MODE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//進板前移動相機頭
	SysParam = SYSTEM_MOVE_CAMERA_BEFORE_PCB_IN;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//進板使用相機影像模式
	SysParam = SYSTEM_PCB_IN_USE_CAMERA_IMAGE_MODE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//檢測前夾板模式			
	SysParam = SYSTEM_CLAMP_PCB_BEFORE_TEST_MODE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//擷取定位點影像延遲時間-ms
	SysParam = SYSTEM_GRAB_FIDUCIAL_DELAY_TIME;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//PCB出板方向
	SysParam = SYSTEM_PCB_OUT_DIRECTION;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	SysParam = SYSTEM_EDIT_LINE_SIZE_LEVEL;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//忽略上一站訊號-回板使用
	SysParam = SYSTEM_BYPASS_LAST_SIGNAL;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//忽略下一站訊號-回板使用
	SysParam = SYSTEM_BYPASS_NEXT_SIGNAL;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	
	
	//PCB OK/NG訊號延遲時間-ms
	SysParam = SYSTEM_PCB_OK_NG_SIGNAL_DELAY_TIME;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//編輯線尺寸的層級
	SysParam = SYSTEM_EDIT_LINE_SIZE_LEVEL;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//線上調機保留最久時間-分鐘 	
	SysParam = SYSTEM_ONLINE_TUNING_KEEP_MAX_TIME;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//線上調機儲存最多數量-片數 
	SysParam = SYSTEM_ONLINE_TUNING_SAVED_MAX_COUNT;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//使用者登入使用
	SysParam = SYSTEM_USER_LOGIN_ENABLED;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	
	
	//使用者登入選項
	SysParam = SYSTEM_USER_LOGIN_OPTIONS;
	if (SaveSystemParamNode(SysParam, Param, FileName) == false)
	{	return false;	}

	//開啟專案模式
	SysParam = SYSTEM_OPEN_PROJECT_MODE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	
	
	//開啟專案底圖編號
	SysParam = SYSTEM_OPEN_PROJECT_MAP_INDEX;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//驗證專案模式		
	SysParam = SYSTEM_VERIFY_PROJECT_MODE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//驗證專案的檔名
	SysParam = SYSTEM_VERIFY_PROJECT_FILENAME;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//模組名稱使用料號 		
	SysParam = SYSTEM_MODEL_NAME_USE_PART_NUMBER;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//線上開專案模式		
	SysParam = SYSTEM_ONLINE_OPEN_PROJECT_MODE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//線上開專案-相機條碼
	SysParam = SYSTEM_ONLINE_OPEN_PROJECT_CAMERA_BARCODE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//線上開專案外接條碼取像模式
	SysParam = SYSTEM_ONLINE_OPEN_PROJECT_BARCODE_DEVICE_GRAB_MODE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }		

	//模組未設定顏色
	SysParam = SYSTEM_MODEL_UNSET_COLOR;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//2值化遮罩-顏色-3色燈
	SysParam = SYSTEM_BINARY_MASK_COLOR_RGB;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//2值化遮罩-顏色-預設
	SysParam = SYSTEM_BINARY_MASK_COLOR_DEFAULT;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }		

	//2值化遮罩-顏色-透明度			
	SysParam = SYSTEM_BINARY_MASK_COLOR_ALPHA;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }		

	//檢測結束顯示結果列表
	SysParam = SYSTEM_INSPECTION_FINISH_SHOW_RESULT_LIST;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//模組預設檢測框等級
	SysParam = SYSTEM_MODEL_DEFAULT_WND_LEVEL;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }		

	//可切換至專案3D畫面
	SysParam = SYSTEM_SWITCH_PROJECT_3D_FRAME;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//自動切換至檢測框3D畫面
	SysParam = SYSTEM_AUTO_SWITCH_WND_3D_FRAME_MODE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	
	
	//自動切換至檢測框3D畫面尺寸上限-um	
	SysParam = SYSTEM_AUTO_SWITCH_WND_3D_FRAME_SIZE_LIMIT;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	
		
	//顯示零件-整板零件
	SysParam = SYSTEM_SHOW_COMPONENT_FULL_MAP;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }		

	//僅顯示瑕疵零件(線上畫面)	
	SysParam = SYSTEM_SHOW_COMPONENT_DEFECT_ONLY;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//顯示除錯頁面
	SysParam = SYSTEM_SHOW_DEBUG_FORM_VIEW;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }		
	
	//分等濾波起點偏移啟用
	SysParam = SYSTEM_LEVEL_FILTER_SHIFT_ENABLED;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }		

	//中值濾波起點偏移啟用
	SysParam = SYSTEM_MEDIAN_FILTER_SHIFT_ENABLED;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }		

	//平滑濾波起點偏移啟用
	SysParam = SYSTEM_SMOOTH_FILTER_SHIFT_ENABLED;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//金字塔中值濾波起點偏移啟用		
	SysParam = SYSTEM_PYRAMID_MEDIAN_FILTER_SHIFT_ENABLED;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//分等濾波步長可接受誤差
	SysParam = SYSTEM_LEVEL_FILTER_PITCH_TOLERANCE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }		

	//中值濾波步長可接受誤差
	SysParam = SYSTEM_MEDIAN_FILTER_PITCH_TOLERANCE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }		

	//平滑濾波步長可接受誤差
	SysParam = SYSTEM_SMOOTH_FILTER_PITCH_TOLERANCE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }			

	//金字塔中值濾波步長可接受誤差
	SysParam = SYSTEM_PYRAMID_MEDIAN_FILTER_PITCH_TOLERANCE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }			

	//Grr偏移隨機補償值
	SysParam = SYSTEM_GRR_OFFSET_RANDOM_VALUE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }			

	//Grr平均值復歸-啟用-依據條碼比較
	SysParam = SYSTEM_GRR_AVERAGE_RESET_ENABLED;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//Grr平均值復歸-匹配比例
	SysParam = SYSTEM_GRR_AVERAGE_RESET_MATCH_RATIO;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//Grr偏移角度平均啟用角度		
	SysParam = SYSTEM_GRR_SKEW_AVERAGE_ENB_VAL;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }		

	//Grr偏移角度平均開始差距-角度	
	SysParam = SYSTEM_GRR_SKEW_AVERAGE_START_GAP;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }		

	//Grr偏移角度平均權重 
	SysParam = SYSTEM_GRR_SKEW_AVERAGE_WEIGHTING;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }		

	//Grr偏移平均啟用像素
	SysParam = SYSTEM_GRR_OFFSET_AVERAGE_ENB_PXL;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }			

	//Grr偏移平均開始差距-微米	
	SysParam = SYSTEM_GRR_OFFSET_AVERAGE_START_GAP;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }		

	//Grr偏移平均權重
	SysParam = SYSTEM_GRR_OFFSET_AVERAGE_WEIGHTING;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }		

	//Grr偏移高度平均啟用高度	
	SysParam = SYSTEM_GRR_HEIGHT_AVERAGE_ENB_VAL;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//Grr偏移高度平均開始差距-微米		
	SysParam = SYSTEM_GRR_HEIGHT_AVERAGE_START_GAP;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//Grr偏移高度平均權重 
	SysParam = SYSTEM_GRR_HEIGHT_AVERAGE_WEIGHTING;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//啟用鎖住螢幕		
	SysParam = SYSTEM_LOCK_SCREEN_ENABLED;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//鎖住螢幕鍵號
	SysParam = SYSTEM_LOCK_SCREEN_KEY_ID;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//多段檢測啟用
	SysParam = SYSTEM_MULTI_DISTRICT_MODE_ENABLED;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//在線調機-軟體條碼
	SysParam = SYSTEM_ONLINE_TUNING_ENABLE_BARCODE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//是否顯示PLC安全檢知設定介面
	SysParam = SYSTEM_SHOW_PLC_SAFTY_SETTING_UI;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//是否顯示演算法OffsetL的參數 	
	SysParam = SYSTEM_SHOW_ALG_OFFSET_L_PARAM;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//是否顯示演算法OffsetA的參數
	SysParam = SYSTEM_SHOW_ALG_OFFSET_A_PARAM;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//是否顯示演算法亮度比例的比例參數
	SysParam = SYSTEM_SHOW_ALG_BRIGHT_RATIO_SCALE_PARAM;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//是否顯示模組屬性參數
	SysParam = SYSTEM_SHOW_MODEL_PROPERTY_PARAM;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//連續貼上模式
	SysParam = SYSTEM_CONTINUE_PASTE_MODE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//拼圖參數-填補尺寸
	SysParam = SYSTEM_STITCH_IMAGE_PADDING_SIZE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//啟用Resin高度對齊-軍達3D對位
	SysParam = SYSTEM_RESIN_HEIGHT_ALIGN_ENABLED;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//啟用模組影像Cad偏移補償um
	SysParam = SYSTEM_MODEL_IMAGE_CAD_OFFSET_ENABLED;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//模組預設SOT樣式
	SysParam = SYSTEM_MODEL_DEFAULT_TRANSISTOR_TYPE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//專案使用本機資料夾
	SysParam = SYSTEM_PROJECT_LOCAL_FOLDER_ENABLED;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//部分複製專案資料庫	
	SysParam = SYSTEM_PARTIAL_COPY_PROJECT_LIBRARY;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }
		
	//自動重整模組底圖檔案
	SysParam = SYSTEM_AUTO_ARRANGE_MODEL_BK_IMAGE_FILES;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }
		
	//自動複製SPC零件圖檔
	SysParam = SYSTEM_AUTO_COPY_SPC_COMPONENT_IMAGE_FILES;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//自動跳過3D影像	
	SysParam = SYSTEM_AUTO_BYPASS_GRAB_3D_FRAME;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//確認專案定位點狀態
	SysParam = SYSTEM_CHECK_PROJECT_FD_READY;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//鎖住模組本體的位置
	SysParam = SYSTEM_LOCK_MODEL_BODY_POSITION;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//最多未判定檢測檔案數 
	SysParam = SYSTEM_MAX_UNCHECK_TEST_FILE_COUNT;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//啟用零件條碼確認	
	SysParam = SYSTEM_CONFIRM_COMPONENT_BARCODE_ENABLED;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//儲存JPEG的質量(001~100)
	SysParam = SYSTEM_SAVE_JPEG_QUALITY;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//複製大量檔案模式
	SysParam = SYSTEM_COPY_HUGE_FILES_MODE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//離線版本模式
	SysParam = SYSTEM_OFFLINE_VERSION_MODE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//使用專案系統參數模式		
	SysParam = SYSTEM_USE_PROJECT_SYSTEM_PARAM_MODE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }
		
	//UI視窗字型增加大小
	SysParam = SYSTEM_UI_WND_FONT_ADD_SIZE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//UI駐停視窗滑動步長			
	SysParam = SYSTEM_UI_DOCK_WND_SLIDE_STEPS;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//UI啟用PCB出板按鈕
	SysParam = SYSTEM_UI_ENABLE_PCB_OUT_BUTTON;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//RabbitMQ伺服器Port
	SysParam = SYSTEM_RABBITMQ_SERVER_PORT;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	
	
	//RabbitMQ通訊網址		
	SysParam = SYSTEM_RABBITMQ_IP_ADDRESS;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//RabbitMQ登錄名稱		
	SysParam = SYSTEM_RABBITMQ_USER_NAME;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//RabbitMQ登錄密碼
	SysParam = SYSTEM_RABBITMQ_PASSWORD;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	
		
	//ITS軟體名稱
	SysParam = SYSTEM_ITS_FILENAME;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//ITS通訊模式
	SysParam = SYSTEM_ITS_COMMUNICATION_MODE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//ITS網路Port
	SysParam = SYSTEM_ITS_SOCKET_IP_PORT;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//ITS網路網址
	SysParam = SYSTEM_ITS_SOCKET_IP_ADDRESS;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//ITS訊息佇列接收名		
	SysParam = SYSTEM_ITS_RABBITMQ_QUEUE_NAME_RECV;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//ITS訊息佇列傳送名
	SysParam = SYSTEM_ITS_RABBITMQ_QUEUE_NAME_SEND;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }
	
	//ITS-對接軟體
	SysParam = SYSTEM_ITS_CONTACT_SOFTWARE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//連線至ITS啟用
	SysParam = SYSTEM_ITS_COMMUNICATION_ENABLED;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//與ITS溝通逾時(ms)			
	SysParam = SYSTEM_ITS_COMMUNICATION_TIMEOUT_MS;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//ITS檔案資料夾-傳送		
	SysParam = SYSTEM_ITS_FILE_FOLDER_SEND;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//ITS檔案資料夾-接收		
	SysParam = SYSTEM_ITS_FILE_FOLDER_RECV;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//ITS檔案資料夾-備份			
	SysParam = SYSTEM_ITS_FILE_BACKUP_ENABLED;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//ITS檔案資料夾-同步檔案
	SysParam = SYSTEM_ITS_FILE_USE_SYNC_FILE_ENABLED;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//ITS使用設定SECS/GEM
	SysParam = SYSTEM_ITS_SET_SECS_GEM_ENABLED;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//ITS使用SECS/GEM-Remote Local
	SysParam = SYSTEM_ITS_SECS_GEM_REMOTE_LOCAL;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//ITS使用設定系統參數
	SysParam = SYSTEM_ITS_SET_SYSTEM_PARAM_ENABLED;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }			

	//ITS使用設定專案參數
	SysParam = SYSTEM_ITS_SET_PROJECT_PARAM_ENABLED;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }			

	//ITS使用設定機台狀態
	SysParam = SYSTEM_ITS_SET_MACHINE_STATUS_ENABLED;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }		

	//ITS使用設定軟體開關
	SysParam = SYSTEM_ITS_SET_APP_OPEN_CLOSE_ENABLED;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }		

	//ITS使用設定程序編號
	SysParam = SYSTEM_ITS_SET_PROCESS_ID_ENABLED;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }			

	//ITS使用設定使用者登入登出
	SysParam = SYSTEM_ITS_SET_USER_LOGIN_OUT_ENABLED;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//專案預設高度轉灰階比例 
	SysParam = SYSTEM_PROG_DEFAULT_SPACE_TO_GRAY_RATIO_MODE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//專案預設空間基準面編號 	
	SysParam = SYSTEM_PROG_DEFAULT_SPACE_BASE_PLANE_INDEX;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//專案預設空間雜訊過濾編號		
	SysParam = SYSTEM_PROG_DEFAULT_SPACE_NOISE_FILTER_INDEX;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//專案預設軌道提前運轉功能
	SysParam = SYSTEM_PROG_DEFAULT_ENABLE_CONVEYER_PRE_RUN;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }
	
	//專案預設定位點異常處理模式
	SysParam = SYSTEM_PROG_DEFAULT_FD_NG_HANDLE_MODE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//專案預設單板定位點取像模式
	SysParam = SYSTEM_PROG_DEFAULT_BOARD_FD_GRAB_MODE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	
		
	//專案預設檢出異常處理模式
	SysParam = SYSTEM_PROG_DEFAULT_BDEFECT_HANDLE_MODE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//專案預設PCB出板模式
	SysParam = SYSTEM_PROG_DEFAULT_PCB_OUT_MODE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//專案預設儲存檢測底圖模式	
	SysParam = SYSTEM_PROG_DEFAULT_PROJECT_SAVE_TEST_MAP;	
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//專案預設連線伺服器模式	
	SysParam = SYSTEM_PROG_DEFAULT_PROJECT_LINK_SERVER_MODE;	
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//專案預設儲存離線圖檔
	SysParam = SYSTEM_PROG_DEFAULT_SAVE_OFFLINE_IMAGE_FILES;	
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//專案預設條碼驗證模式		
	SysParam = SYSTEM_PROG_DEFAULT_BARCODE_VERIFY_MODE;	
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//專案預設條碼查詢模式
	SysParam = SYSTEM_PROG_DEFAULT_BARCODE_RETRIEVE_MODE;	
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//操作等級-專案開啟
	SysParam = SYSTEM_OPERATE_LEVEL_PROJECT_OPEN;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//操作等級-專案開啟
	SysParam = SYSTEM_OPERATE_LEVEL_PROJECT_OPEN;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//操作等級-專案儲存
	SysParam = SYSTEM_OPERATE_LEVEL_PROJECT_SAVE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//操作等級-專案參數
	SysParam = SYSTEM_OPERATE_LEVEL_PROJECT_PARAM;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//操作等級-線上運行
	SysParam = SYSTEM_OPERATE_LEVEL_ONLINE_RUN;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//操作等級-線上直通
	SysParam = SYSTEM_OPERATE_LEVEL_ONLINE_BYPASS;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//操作等級-線上停止
	SysParam = SYSTEM_OPERATE_LEVEL_ONLINE_STOP;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//操作等級-線上存圖
	SysParam = SYSTEM_OPERATE_LEVEL_ONLINE_SAVE_IMAGE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//操作等級-線上解鎖		
	SysParam = SYSTEM_OPERATE_LEVEL_ONLINE_UNLOCK;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//操作等級-編輯功能-新增		
	SysParam = SYSTEM_OPERATE_LEVEL_EDIT_FUNC_ADD;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//操作等級-編輯功能-刪除
	SysParam = SYSTEM_OPERATE_LEVEL_EDIT_FUNC_DEL;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//操作等級-編輯功能-不檢測
	SysParam = SYSTEM_OPERATE_LEVEL_EDIT_FUNC_BYPASS;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//操作等級-MES相關-控制狀態-離線		
	SysParam = SYSTEM_OPERATE_LEVEL_MES_CTRL_STATE_OFFLINE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//操作等級-MES相關-控制狀態-本地上線
	SysParam = SYSTEM_OPERATE_LEVEL_MES_CTRL_STATE_LOCAL;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//操作等級-MES相關-控制狀態-遠端上線
	SysParam = SYSTEM_OPERATE_LEVEL_MES_CTRL_STATE_REMOTE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//操作等級-MES相關-顯示內容
	SysParam = SYSTEM_OPERATE_LEVEL_MES_SHOW_CONTEXT;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }	

	//啟用NPM-條碼
	SysParam = SYSTEM_M2M_NPM_BARCODE_ENABLE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//啟用NPM APC-FF1
	SysParam = SYSTEM_M2M_NPM_APC_FF1_ENABLE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//啟用NPM APC-FF2
	SysParam = SYSTEM_M2M_NPM_APC_FF2_ENABLE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//啟用NPM APC-MFB
	SysParam = SYSTEM_M2M_NPM_APC_MFB_ENABLE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//NPM軌道名稱-A軌
	SysParam = SYSTEM_M2M_NPM_LANE_NAME_LA;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//NPM軌道名稱-B軌
	SysParam = SYSTEM_M2M_NPM_LANE_NAME_LB;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//NPM-輸入共享資料夾-A軌
	SysParam = SYSTEM_M2M_NPM_INPUT_SHARE_FOLDER_LA;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//NPM-輸入共享資料夾-B軌
	SysParam = SYSTEM_M2M_NPM_INPUT_SHARE_FOLDER_LB;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//NPM-輸出共享資料夾-A軌		
	SysParam = SYSTEM_M2M_NPM_OUTPUT_SHARE_FOLDER_LA;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//NPM-輸出共享資料夾-B軌		
	SysParam = SYSTEM_M2M_NPM_OUTPUT_SHARE_FOLDER_LB;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//啟用M2M HAS I (Hanwha AOI Solution MAOI_SAOI)
	SysParam = SYSTEM_M2M_HASI_ENABLE;
	if (SaveSystemParamNode(SysParam, Param, FileName) == false)
	{	return false; }

	//HAS I 共享資料夾
	SysParam = SYSTEM_M2M_HASI_SHARE_FOLDER;
	if (SaveSystemParamNode(SysParam, Param, FileName) == false)
	{	return false; }

	//啟用 HAS I - SerialFile
	SysParam = SYSTEM_M2M_HASI_SERIALFILE_ENABLE;
	if (SaveSystemParamNode(SysParam, Param, FileName) == false)
	{	return false; }

	//HAS I - SerialFile - Size of Serial Queue
	SysParam = SYSTEM_M2M_HASI_SERIALFILE_QUEUESIZE;
	if (SaveSystemParamNode(SysParam, Param, FileName) == false)
	{	return false; }

	//等待資料的延遲時間-ms
	SysParam = SYSTEM_M2M_HASI_SERIALFILE_DWELLTIME;
	if (SaveSystemParamNode(SysParam, Param, FileName) == false)
	{	return false; }

	//啟用 HAS I - SPIOffsetFile
	SysParam = SYSTEM_M2M_HASI_SPIOFFSETFILE_ENABLE;
	if (SaveSystemParamNode(SysParam, Param, FileName) == false)
	{	return false; }

	//產線上 AOI 的檢查階段
	SysParam = SYSTEM_M2M_HASI_AOI_STAGE;
	if (SaveSystemParamNode(SysParam, Param, FileName) == false)
	{	return false; }

	//啟用 HAS I - PNP
	SysParam = SYSTEM_M2M_HASI_PNP_ENABLE;
	if (SaveSystemParamNode(SysParam, Param, FileName) == false)
	{	return false; }

	//AI模型伺服器啟用
	SysParam = SYSTEM_AI_MODEL_SERVER_ENABLE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//AI模型伺服器逾時
	SysParam = SYSTEM_AI_MODEL_SERVER_TIMEOUT;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//AI模型伺服器檔名
	SysParam = SYSTEM_AI_MODEL_SERVER_FILENAME;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//AI模型檔案資料夾-傳送
	SysParam = SYSTEM_AI_MODEL_FILE_FOLDER_SEND;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//AI模型檔案資料夾-接收
	SysParam = SYSTEM_AI_MODEL_FILE_FOLDER_RECV;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//AI模型分類最小叢集距離-um
	SysParam = SYSTEM_AI_MODEL_LABEL_MIN_CLUSTER_DISTANCE;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//外部複製檔案啟用
	SysParam = SYSTEM_EXTERNAL_COPY_FILE_ENABLED;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//外部複製檔案軟體名稱
	SysParam = SYSTEM_EXTERNAL_COPY_FILE_APP_NAME;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//外部複製檔案輸出資料夾
	SysParam = SYSTEM_EXTERNAL_COPY_FILE_SEND_FOLDER;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//Cpk圖表啟用
	SysParam = SYSTEM_CPK_CHART_ENABLED;
	if ( SaveSystemParamNode(SysParam, Param, FileName) == false ) 	
	{	return false; }

	//檢測框跟隨旋轉
	SysParam = SYSTEM_WND_ROTATION_FOLLOWED;
	if (SaveSystemParamNode(SysParam, Param, FileName) == false)
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::SaveSystemParamNode(SYSTEM_PARAM_ID SysParam, const TSystemParameter &Param, LPCTSTR filename)//儲存系統參數
{	
	CString Section = _T("");	
	CString KeyName = _T("");
	CString String  = _T("");
	ObtainSystemParameterKeyName(SysParam, Section, KeyName);
	GetSystemParameterStringByID(SysParam, Param, String);
	if ( SaveINIData(Section, KeyName, String, filename, m_ErrorString) == false )
	{	return false; }	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::LoadSystemParameter(bool bCreateTempFolder)//載入系統參數
{	
	CString Filename;
	Filename = GetSystemParamFilename();
	if ( LoadSystemParamFile(Filename, m_SystemParameter) == false )
	{	return false; }

	CString String;
	String = GetAOIDirectory();
	JetAPI::TCHARCopy(String, m_AOIDirectoryA, sizeof(m_AOIDirectoryA), m_AOIDirectoryW, sizeof(m_AOIDirectoryW));
	String = GetAOILogDirectory();
	JetAPI::TCHARCopy(String, m_AOILogDirectoryA, sizeof(m_AOILogDirectoryA), m_AOILogDirectoryW, sizeof(m_AOILogDirectoryW));

	if ( true == bCreateTempFolder )
	{	CreateAOITempDirectory();	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::LoadSystemParamFile(LPCTSTR filename, TSystemParameter &Param)//載入系統參數
{
	if ( LoadSystemParamFileFn(filename, Param) == false )
	{
		SetSystemExceptionCode_FileRead();
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::LoadSystemParamFileFn(LPCTSTR filename, TSystemParameter &Param)//載入系統參數
{
	if ( NULL == filename ) 
	{
		m_ErrorString = _T("Error, LoadSystemParamFile No Filename");
		return false;
	}

	const size_t textlen = 128;
	int     tempI=0;
	double  tempD=0.0;
	CString str;
	CString FileName;
	CString Section = _T("");	
	CString KeyName = _T("");
	CString Default = _T("");
	SYSTEM_PARAM_ID SysParam;
	TCHAR   String[textlen]=_T("");	

	FileName = filename;
	Section = _T("System Parameter");		
	//::_tcstod(String, NULL);	

	//系統資料夾
	//SysParam = SYSTEM_AOI_FOLDER_HOST;	
	//if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	//{	SetSystemParameterStringByID(SysParam, Param, String);	}		
	
	//暫存的資料夾
	SysParam = SYSTEM_AOI_FOLDER_TEMP;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String);	}

	//系統訊息資料夾
	SysParam = SYSTEM_AOI_FOLDER_LOG;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{
		Param.m_CustomerLogFolder = String;
		SetSystemParameterStringByID(SysParam, Param, String);	
	}

	//專案資料夾	
	SysParam = SYSTEM_AOI_FOLDER_PROJECT;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String);	}

	//結果資料夾
	SysParam = SYSTEM_AOI_FOLDER_RESULT;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String);	}
	
	//伺服器資料夾
	SysParam = SYSTEM_AOI_FOLDER_SERVER;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String);	}

	//統計數據
	SysParam = SYSTEM_AOI_FOLDER_STATIC_DATA;
	if (LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true)
	{	SetSystemParameterStringByID(SysParam, Param, String);	}

	//文字報告資料夾
	SysParam = SYSTEM_AOI_FOLDER_TEXT_REPORT;
	if (LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true)
	{	SetSystemParameterStringByID(SysParam, Param, String);	}

	//線上調機資料夾
	SysParam = SYSTEM_AOI_FOLDER_ONLINE_TUNING;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String);	}

	//線上條碼資料夾
	SysParam = SYSTEM_AOI_FOLDER_ONLINE_BARCODE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String);	}

	//線上離線編程資料夾	
	SysParam = SYSTEM_AOI_FOLDER_ONLINE_OFFLINE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String);	}

	//專案檢測底圖資料夾
	SysParam = SYSTEM_AOI_FOLDER_PROJECT_TEST_MAP;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String);	}

	//專案檢測底圖資料夾-暫存
	SysParam = SYSTEM_AOI_FOLDER_PROJECT_TEST_MAP_TEMP;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String);	}

	//專案檢測底圖資料夾-備份
	SysParam = SYSTEM_AOI_FOLDER_PROJECT_TEST_MAP_BACKUP;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String);	}

	//專案原圖資料夾
	SysParam = SYSTEM_AOI_FOLDER_PROJECT_RAW;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String);	}

	//專案除錯資料夾
	SysParam = SYSTEM_AOI_FOLDER_PROJECT_DEBUG;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String);	}

	//監控狀態資料夾
	SysParam = SYSTEM_AOI_FOLDER_MONITOR_STATUS;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String);	}

	//客戶訊息資料夾
	SysParam = SYSTEM_AOI_FOLDER_CUSTOMER_LOG;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String);	}

	//客戶報告資料夾		
	SysParam = SYSTEM_AOI_FOLDER_CUSTOMER_REPORT;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String);	}

	//條碼檔案資料夾-A軌	
	SysParam = SYSTEM_AOI_FOLDER_BARCODE_FILE_LA;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String);	}

	//條碼檔案資料夾-B軌	
	SysParam = SYSTEM_AOI_FOLDER_BARCODE_FILE_LB;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String);	}

	//AI檔案輸出資料夾
	SysParam = SYSTEM_AOI_FOLDER_AI_FILE_EXPORT;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String);	}

	//AI影像輸出資料夾
	SysParam = SYSTEM_AOI_FOLDER_AI_IMAGE_EXPORT;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String);	}

	//檢測追蹤資料夾
	SysParam = SYSTEM_AOI_FOLDER_TEST_TRACK;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String);	}

	//資料庫-本機
	SysParam = SYSTEM_AOI_LIBRARY_HOST;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String);	}

	//資料庫-遠端
	SysParam = SYSTEM_AOI_LIBRARY_REMOTE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String);	}

	//是否儲存移動時間
	SysParam = SYSTEM_SAVE_MSG_MOVING_TIME;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//是否備份移動時間狀態訊息	
	SysParam = SYSTEM_BACKUP_MSG_MOVING_TIME;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//是否儲存現在狀態訊息
	SysParam = SYSTEM_SAVE_MSG_CURRENT_PROCESS;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }
	
	//是否備份現在狀態訊息
	SysParam = SYSTEM_BACKUP_MSG_CURRENT_PROCESS;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//是否儲存機台監控訊息
	SysParam = SYSTEM_SAVE_MSG_MACHINE_MONITOR;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//是否儲存機台監控訊息-網頁用
	SysParam = SYSTEM_SAVE_MSG_MACHINE_MONITOR_WEB;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//是否儲存相機增加循環資料訊息
	SysParam = SYSTEM_SAVE_CAMERA_ADD_RING_BUFFER_LOG;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//是否專案文字檔報告
	SysParam = SYSTEM_SAVE_PROJECT_REPORT_TEXT;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//儲存專案文字檔報告檔名
	SysParam = SYSTEM_SAVE_PROJECT_REPORT_TEXT_FILENAME_MODE;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//是否儲存客戶報告
	SysParam = SYSTEM_SAVE_CUSTOMER_REPORT_FILE;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//儲存專案SPC檔案模式
	SysParam = SYSTEM_SAVE_PROJECT_SPC_FILE_MODE;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//儲存專案SPC資料庫模式
	SysParam = SYSTEM_SAVE_PROJECT_SPC_LIBRARY_MODE;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//是否儲存專案檢測框數據檔案	
	SysParam = SYSTEM_SAVE_PROJECT_REPORT_WND_READING;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//顯示記憶體未釋放訊息
	SysParam = SYSTEM_SHOW_MEMORY_LEAK_MESSAGE;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//是否送至DebugView視窗	
	SysParam = SYSTEM_SEND_DEBUG_VIEW_STRING;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//是否儲存影像填滿執行緒訊息
	SysParam = SYSTEM_SAVE_SLICE_FILL_THREAD_LOG;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//是否儲存影像合併執行緒訊息
	SysParam = SYSTEM_SAVE_FRAME_MERGE_THREAD_LOG;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//是否儲存區域合併執行緒訊息
	SysParam = SYSTEM_SAVE_FIELD_MERGE_THREAD_LOG;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//是否儲存影像載入執行緒訊息
	SysParam = SYSTEM_SAVE_FRAME_LOAD_THREAD_LOG;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//是否儲存區域計算執行緒訊息
	SysParam = SYSTEM_SAVE_REGION_CALC_THREAD_LOG;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//是否儲存系列執行緒訊息
	SysParam = SYSTEM_SAVE_SQUENCE_THREAD_LOG;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//是否儲存視野影像釋放執行緒訊息
	SysParam = SYSTEM_SAVE_FIELD_FRAME_RELEASE_THREAD_LOG;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//是否儲存線上檢測執行緒訊息
	SysParam = SYSTEM_SAVE_ONLINE_INSPECTION_THREAD_LOG;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//是否儲存刪除資料夾執行緒訊息
	SysParam = SYSTEM_SAVE_REMOVE_FOLDER_THREAD_LOG;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//是否儲存軌道自動運轉執行緒訊息
	SysParam = SYSTEM_SAVE_CONVEYER_PRE_RUN_THREAD_LOG;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//是否儲存ITS運作執行緒訊息
	SysParam = SYSTEM_SAVE_ITS_PROC_THREAD_LOG;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//是否儲存載入維修站檔案執行緒訊息	
	SysParam = SYSTEM_SAVE_LOAD_REPAIR_FILE_THREAD_LOG;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//是否儲存維修站訊息執行緒訊息	
	SysParam = SYSTEM_SAVE_REPAIR_RESULT_SIGNAL_THREAD_LOG;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//是否儲存簡易作業執行緒訊息
	SysParam = SYSTEM_SAVE_SIMPLE_JOB_THREAD_LOG;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }
	
	//是否儲存HASI監視執行緒訊息
	SysParam = SYSTEM_SAVE_HASI_MONITOR_THREAD_LOG;
	if (LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true)
	{	SetSystemParameterStringByID(SysParam, Param, String);	}

	//是否儲存檢測物件結束訊息
	SysParam = SYSTEM_SAVE_TEST_OBJECT_FINISH_LOG;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//是否儲存介面重繪函式訊息
	SysParam = SYSTEM_SAVE_UI_DRAW_FUNC_LOG;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//是否儲存使用者操作訊息
	SysParam = SYSTEM_SAVE_USER_OPERATION_LOG;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//是否儲存檢測追蹤檔案
	SysParam = SYSTEM_SAVE_TEST_TRACK_FILE;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//提前載入專案離線圖檔
	SysParam = SYSTEM_PRE_LOAD_PROJECT_OFFLINE_IMAGE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//自動釋放區域影像
	SysParam = SYSTEM_AUTO_RELEASE_FIELD_FRAME_BUFFER;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//自動釋放離線編程區域影像		
	SysParam = SYSTEM_AUTO_RELEASE_OFFLINE_FIELD_FRAME_BUFFER;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//本機網路網址
	SysParam = SYSTEM_IP_HOST_COMPUTER;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }
	
	//機台廠區
	SysParam = SYSTEM_MACHINE_LOCATION;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//機台棟別
	SysParam = SYSTEM_MACHINE_BUILDING;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }
		 
	//機台樓層
	SysParam = SYSTEM_MACHINE_FLOOR;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }
	
	//機台車間
	SysParam = SYSTEM_MACHINE_ROOM;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }
		 
	//機台線名
	SysParam = SYSTEM_MACHINE_LINE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }
	Param.m_MachineLine_LB = Param.m_MachineLine;

	//本機線別-B軌
	SysParam = SYSTEM_MACHINE_LINE_LB;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//機台站名
	SysParam = SYSTEM_MACHINE_STATION;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//本機站別-B軌
	SysParam = SYSTEM_MACHINE_STATION_LB;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }
	Param.m_MachineStation_LB = Param.m_MachineStation;

	//本機序號
	SysParam = SYSTEM_MACHINE_SN;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//機台型號
	SysParam = SYSTEM_MACHINE_NAME;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//機台廠商		
	SysParam = SYSTEM_MACHINE_VENDOR;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//機台別名
	SysParam = SYSTEM_MACHINE_ALIAS;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//MES登入名稱
	SysParam = SYSTEM_MACHINE_MES_NAME;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//MES登入密碼
	SysParam = SYSTEM_MACHINE_MES_PASSWORD;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//MES登入裝置
	SysParam = SYSTEM_MACHINE_MES_DEVICE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	
	
	//MES登入裝置-2
	SysParam = SYSTEM_MACHINE_MES_DEVICE_2;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//MES-裝置代號
	SysParam = SYSTEM_MACHINE_MES_CODE_NAME;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//客戶編號
	SysParam = SYSTEM_AOI_CUSTOMER_ID;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//設備機種樣式
	SysParam = SYSTEM_MACHINE_MODEL_TYPE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//設備相機方向
	SysParam = SYSTEM_MACHINE_CAMERA_SIDE;
	if (LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true)
	{	SetSystemParameterStringByID(SysParam, Param, String);	}

	//多國語系版本 
	SysParam = SYSTEM_MULTI_LANGUAGE_MODE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//繪圖模式 
	SysParam = SYSTEM_DC_STRECTCH_BLT_MODE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//影像還元彩色模式 
	SysParam = SYSTEM_CAMERA_DEBAYER_MODE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	
	
	//使用相位還原法
	KeyName = _T("Phase Unwrapping Mode");
	Default.Format(_T("%d"), Param.m_UnwrappingMode);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	Param.m_UnwrappingMode = (PHASE_UNWRAP_MODE)::_ttoi(String); }

	//影像顯示模式
	SysParam = SYSTEM_IMAGE_DISPLAY_MODE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//影像顯示強化模式 
	SysParam = SYSTEM_IMAGE_DISPLAY_ENHANCE_MODE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//影像顯示Gain 
	SysParam = SYSTEM_IMAGE_DISPLAY_GAIN;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }
	
	//影像顯示Gamma 
	SysParam = SYSTEM_IMAGE_DISPLAY_GAMMA;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }
	
	//影像顯示的銳利化的半徑	
	SysParam = SYSTEM_IMAGE_DISPLAY_SHARPNESS_RADIUS;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//影像顯示的銳利化的總量		
	SysParam = SYSTEM_IMAGE_DISPLAY_SHARPNESS_AMOUNT;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//影像顯示的銳利化的閥值
	SysParam = SYSTEM_IMAGE_DISPLAY_SHARPNESS_THRESHOLD;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//影像顯示的局部Gamma的計算範圍				
	SysParam = SYSTEM_IMAGE_DISPLAY_LOCAL_GAMMA_CALC_SIZE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//影像顯示的局部Gamma的縮放比例
	SysParam = SYSTEM_IMAGE_DISPLAY_LOCAL_GAMMA_SCALE_VAL;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//影像顯示最大縮放比例	
	SysParam = SYSTEM_IMAGE_DISPLAY_MAX_ZOOM_SCALE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//線上檢測介面模式	
	SysParam = SYSTEM_ONLINE_FORM_VIEW_MODE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//計算焦距平滑尺寸		
	SysParam = SYSTEM_CALC_FOCUS_SMOOTH_SIZE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//計算焦距模式
	SysParam = SYSTEM_CALC_FOCUS_MODE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//自動重測次數上限
	SysParam = SYSTEM_AUTO_RETRY_MAX_COUNT;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//自動設定DLP樣板	
	SysParam = SYSTEM_AUTO_SETUP_DLP_PATTERN;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//解析度模式
	SysParam = SYSTEM_RESOLUTION_MODE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//解析度顯示比例
	SysParam = SYSTEM_RESOLUTION_SHOW_SCALE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//Cpu最多使用數量
	SysParam = SYSTEM_CPU_MAX_COUNT_USED;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//多執行緒數量-畫面填圖 
	SysParam = SYSTEM_THREAD_CNT_SLICE_FILL;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	
	
	//多執行緒數量-影像合併
	SysParam = SYSTEM_THREAD_CNT_FRAME_MERGE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }
	Param.m_MTCount_FieldMerge=Param.m_MTCount_FrameMerge;

	//多執行緒數量-區域合併
	SysParam = SYSTEM_THREAD_CNT_FIELD_MERGE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//多執行緒數量-影像載入
	SysParam = SYSTEM_THREAD_CNT_FRAME_LOAD;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//多執行緒數量-區域計算 
	SysParam = SYSTEM_THREAD_CNT_REGION_CALC;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//多執行緒數量-閒置核心	
	SysParam = SYSTEM_THREAD_CNT_PROC_IDLE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//多執行緒數量-取像閒置核心	
	SysParam = SYSTEM_THREAD_CNT_GRAB_IDLE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//OpenMP核心數量-一般操作
	SysParam = SYSTEM_OPEN_MP_CNT_GENERAL;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//OpenMP核心數量-檢測中	
	SysParam = SYSTEM_OPEN_MP_CNT_INSPECTION;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//OpenMP確認尺寸-檢測中
	SysParam = SYSTEM_OPEN_MP_CHK_SIZE_INSPECTION;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//相位週期-1 
	SysParam = SYSTEM_PHASE_PERIOD_1;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//相位週期-2
	SysParam = SYSTEM_PHASE_PERIOD_2;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//相位週期-3
	SysParam = SYSTEM_PHASE_PERIOD_3;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//分離DLP2次曝光表格
	SysParam = SYSTEM_SEPARATE_DLP_2EXP_TABLE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//相位轉換高度模式
	SysParam = SYSTEM_PHASE_CONVERT_HEIGHT_MODE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//相位遮罩-無效點外擴
	SysParam = SYSTEM_PHASE_NOISE_EXTEND_VOID;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//相位遮罩-低對比 
	SysParam = SYSTEM_PHASE_NOISE_LOW_CONTRAST;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//相位遮罩-低潛能 
	SysParam = SYSTEM_PHASE_NOISE_LOW_POTENTIAL;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//相位遮罩-過亮度 
	SysParam = SYSTEM_PHASE_NOISE_OVER_SATURATED;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }
	
	Param.m_PhaseNoiseLowContrastB = Param.m_PhaseNoiseLowContrastA;//相位遮罩-低對比
	Param.m_PhaseNoiseLowPotentialB = Param.m_PhaseNoiseLowPotentialA;//相位遮罩-潛力
	Param.m_PhaseNoiseOverSaturatedB = Param.m_PhaseNoiseOverSaturatedA;//相位遮罩-過亮度
	Param.m_PhaseNoiseLowContrastC = Param.m_PhaseNoiseLowContrastA;//相位遮罩-低對比
	Param.m_PhaseNoiseLowPotentialC = Param.m_PhaseNoiseLowPotentialA;//相位遮罩-潛力
	Param.m_PhaseNoiseOverSaturatedC = Param.m_PhaseNoiseOverSaturatedA;//相位遮罩-過亮度
	
	//相位遮罩-低對比-B
	SysParam = SYSTEM_PHASE_NOISE_LOW_CONTRAST_B;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//相位遮罩-低潛能-B 
	SysParam = SYSTEM_PHASE_NOISE_LOW_POTENTIAL_B;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//相位遮罩-過亮度-B
	SysParam = SYSTEM_PHASE_NOISE_OVER_SATURATED_B;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//相位遮罩-低對比-C
	SysParam = SYSTEM_PHASE_NOISE_LOW_CONTRAST_C;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//相位遮罩-低潛能-C 
	SysParam = SYSTEM_PHASE_NOISE_LOW_POTENTIAL_C;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//相位遮罩-過亮度-C
	SysParam = SYSTEM_PHASE_NOISE_OVER_SATURATED_C;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//相位遮罩-平滑處理
	SysParam = SYSTEM_PHASE_NOISE_SMOOTH_FILTER;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//相位遮罩-低對比-顏色
	SysParam = SYSTEM_PHASE_NOISE_LOW_CONTRAST_COLOR;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//相位遮罩-低潛力-顏色
	SysParam = SYSTEM_PHASE_NOISE_LOW_POTENTIAL_COLOR;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	
	
	//相位遮罩-過亮度-顏色
	SysParam = SYSTEM_PHASE_NOISE_OVER_SATURATED_COLOR;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	
	
	//相位遮罩-無效點外擴-顏色
	SysParam = SYSTEM_PHASE_NOISE_EXTEND_VOID_COLOR;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//相位遮罩-高度異常-顏色
	SysParam = SYSTEM_SPACE_NOISE_HEIGHT_UNEXPECTED_COLOR;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	
	
	//相位遮罩-過低異常-顏色
	SysParam = SYSTEM_SPACE_NOISE_HEIGHT_OVER_LOW_COLOR;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	
	
	//相位遮罩-可靠高度-顏色	
	SysParam = SYSTEM_SPACE_VALID_BEST_COLOR;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }
		
	//3D物件顯示比例-X
	SysParam = SYSTEM_3D_OBJECT_DRAW_SCALE_X;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//3D物件顯示比例-Y		
	SysParam = SYSTEM_3D_OBJECT_DRAW_SCALE_Y;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//3D物件顯示比例-Z
	SysParam = SYSTEM_3D_OBJECT_DRAW_SCALE_Z;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//整板的顏色-1		
	SysParam = SYSTEM_PANEL_COLOR_1;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//整板的顏色-2		
	SysParam = SYSTEM_PANEL_COLOR_2;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//整板文字顏色		
	SysParam = SYSTEM_PANEL_TEXT_COLOR;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//整板選取到的顏色
	SysParam = SYSTEM_PANEL_SELECTED_COLOR;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//單板的顏色-1		
	SysParam = SYSTEM_BOARD_COLOR_1;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }
	
	//單板的顏色-2
	SysParam = SYSTEM_BOARD_COLOR_2;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//單板文字顏色	
	SysParam = SYSTEM_BOARD_TEXT_COLOR;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//單板選取到的顏色
	SysParam = SYSTEM_BOARD_SELECTED_COLOR;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//定位點的顏色-1
	SysParam = SYSTEM_FD_COLOR_1;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//定位點的顏色-2		
	SysParam = SYSTEM_FD_COLOR_2;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//定位點文字顏色
	SysParam = SYSTEM_FD_TEXT_COLOR;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//定位點選取到的顏色
	SysParam = SYSTEM_FD_SELECTED_COLOR;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//條碼的顏色-1
	SysParam = SYSTEM_BARCODE_COLOR_1;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//條碼的顏色-2		
	SysParam = SYSTEM_BARCODE_COLOR_2;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//條碼文字顏色
	SysParam = SYSTEM_BARCODE_TEXT_COLOR;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//條碼選取到的顏
	SysParam = SYSTEM_BARCODE_SELECTED_COLOR;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//零件的顏色-1
	SysParam = SYSTEM_COMPONENT_COLOR_1;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//零件的顏色-2
	SysParam = SYSTEM_COMPONENT_COLOR_2;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//零件文字顏色
	SysParam = SYSTEM_COMPONENT_TEXT_COLOR;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//零件選取到的顏色
	SysParam = SYSTEM_COMPONENT_SELECTED_COLOR;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//分段的顏色-1
	SysParam = SYSTEM_DISTRICT_COLOR_1;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }
		
	//分段的顏色-2
	SysParam = SYSTEM_DISTRICT_COLOR_2;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//檢測OK顏色		
	SysParam = SYSTEM_INSPECTED_RESULT_OK_COLOR;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//檢測NG顏色		
	SysParam = SYSTEM_INSPECTED_RESULT_NG_COLOR;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//檢測Skip顏色		
	SysParam = SYSTEM_INSPECTED_RESULT_SKIP_COLOR;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//檢測Bypass顏色		
	SysParam = SYSTEM_INSPECTED_RESULT_BYPASS_COLOR;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//檢測UnTest顏色		
	SysParam = SYSTEM_INSPECTED_RESULT_UNTEST_COLOR;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//檢測Warning顏色		
	SysParam = SYSTEM_INSPECTED_RESULT_WARNING_COLOR;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//檢測Exception顏色
	SysParam = SYSTEM_INSPECTED_RESULT_EXCEPTION_COLOR;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//相位雜訊定義 
	SysParam = SYSTEM_PHASE_NOISE_DEFINE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//相位雜訊定義模式
	SysParam = SYSTEM_PHASE_NOISE_DEFINE_MODE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//高度雜訊單投光高度最低極限-um
	SysParam = SYSTEM_SPACE_NOISE_SINGLE_CAST_LOW_LIMIT;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//高度雜訊多投光合併Patch尺寸
	SysParam = SYSTEM_SPACE_NOISE_MULTI_CAST_PATCH_SIZE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//高度雜訊多投光合併模式 
	SysParam = SYSTEM_SPACE_NOISE_MULTI_CAST_MERGE_MODE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//高度雜訊多投光合併最可靠模式
	SysParam = SYSTEM_SPACE_NOISE_MULTI_CAST_MERGE_BEST_MODE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//高度雜訊多亮度合併模式-Mean, MaxB
	SysParam = SYSTEM_SPACE_NOISE_MULTI_INTENSITY_MERGE_MODE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//高度雜訊多投光最少有效值數 
	SysParam = SYSTEM_SPACE_NOISE_MULTI_CAST_MIN_VALID_COUNT;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//高度雜訊多投光高度最大差值 
	SysParam = SYSTEM_SPACE_NOISE_MULTI_CAST_MAX_DIFFERENCE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	
	
	//高度雜訊多投光高度極限差值-um
	SysParam = SYSTEM_SPACE_NOISE_MULTI_CAST_LIMIT_DIFFERENCE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//高度雜訊多投光高度最好比例-um	
	SysParam = SYSTEM_SPACE_NOISE_MULTI_CAST_VALID_BEST_RATIO;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//高度雜訊多投光高度有效差值-um
	SysParam = SYSTEM_SPACE_NOISE_MULTI_CAST_VALID_DIFFERENCE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//高度雜訊多投光合併對邊灰階上限-gray	
	SysParam = SYSTEM_SPACE_NOISE_MULTI_CAST_OPPOSITE_MAX_GRAY;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	
	
	//投光後濾波模式
	SysParam = SYSTEM_SPACE_NOISE_CAST_FILTER_MODE;
	if (LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true)
	{
		SetSystemParameterStringByID(SysParam, Param, String);
	}

	//高度雜訊的中值濾波尺寸
	SysParam = SYSTEM_SPACE_NOISE_CAST_MEDIAN_FILTER_SIZE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }		

	//高度雜訊的中值濾波使用尺寸
	SysParam = SYSTEM_SPACE_NOISE_CAST_MEDIAN_FILTER_USE_SIZE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }		

	//空間雜訊無效點外擴尺寸-piexel	
	SysParam = SYSTEM_SPACE_NOISE_DATA_VOID_EXPAND_SIZE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	
	
	//空間雜訊無效點外擴啟用
	SysParam = SYSTEM_SPACE_NOISE_DATA_VOID_EXPAND_ENABLED;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	
	
	//空間雜訊首次濾波模式
	SysParam = SYSTEM_SPACE_NOISE_FIRST_FILTER_MODE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }		

	//空間雜訊首次濾波步長
	SysParam = SYSTEM_SPACE_NOISE_FIRST_FILTER_PITCH;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }		

	//空間雜訊首次濾波尺寸
	SysParam = SYSTEM_SPACE_NOISE_FIRST_KER_SIZE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//空間雜訊首次使用尺寸	
	SysParam = SYSTEM_SPACE_NOISE_FIRST_USE_SIZE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//高度雜訊過低模式
	SysParam = SYSTEM_SPACE_NOISE_OVER_LOW_MODE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//高度雜訊過低高度-um 
	SysParam = SYSTEM_SPACE_NOISE_OVER_LOW_RANGE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//高度雜訊過低極限-um	
	SysParam = SYSTEM_SPACE_NOISE_OVER_LOW_LIMIT;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//高度雜訊過低濾波尺寸
	SysParam = SYSTEM_SPACE_NOISE_OVER_LOW_KER_SIZE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//高度雜訊過低使用尺寸
	SysParam = SYSTEM_SPACE_NOISE_OVER_LOW_USE_SIZE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//高度雜訊高度異常模式
	SysParam = SYSTEM_SPACE_NOISE_HEIGHT_ABNORMAL_MODE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//高度雜訊高度異常步長
	SysParam = SYSTEM_SPACE_NOISE_HEIGHT_ABNORMAL_PITCH;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	
	
	//高度雜訊高度異常範圍-um		
	SysParam = SYSTEM_SPACE_NOISE_HEIGHT_ABNORMAL_RANGE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//高度雜訊高度異常確認尺寸
	SysParam = SYSTEM_SPACE_NOISE_HEIGHT_ABNORMAL_CHK_SIZE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//高度雜訊高度異常濾波尺寸
	SysParam = SYSTEM_SPACE_NOISE_HEIGHT_ABNORMAL_KER_SIZE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//高度雜訊高度異常使用尺寸
	SysParam = SYSTEM_SPACE_NOISE_HEIGHT_ABNORMAL_USE_SIZE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	
	
	//高度雜訊高度異常重複次數
	SysParam = SYSTEM_SPACE_NOISE_HEIGHT_ABNORMAL_REPEAT_CNT;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	
	
	//空間雜訊重建外擴尺寸
	SysParam = SYSTEM_SPACE_NOISE_RECONTRUCTED_EXT_SIZE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	
	
	//空間雜訊重建外擴啟用	
	SysParam = SYSTEM_SPACE_NOISE_RECONTRUCTED_ENABLED;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//空間雜訊最後濾波尺寸
	SysParam = SYSTEM_SPACE_NOISE_FINAL_KER_SIZE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//空間雜訊最後遮罩尺寸
	SysParam = SYSTEM_SPACE_NOISE_FINAL_USE_SIZE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//空間雜訊最後平滑啟用	
	SysParam = SYSTEM_SPACE_NOISE_FINAL_FILTER_MODE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//空間雜訊最後濾波步長
	SysParam = SYSTEM_SPACE_NOISE_FINAL_FILTER_PITCH;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	
	
	//空間雜訊最後濾波尺寸-2
	SysParam = SYSTEM_SPACE_NOISE_FINAL_KER_SIZE2;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//空間雜訊最後遮罩尺寸-2
	SysParam = SYSTEM_SPACE_NOISE_FINAL_USE_SIZE2;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//空間雜訊最後平滑啟用-2
	SysParam = SYSTEM_SPACE_NOISE_FINAL_FILTER_MODE2;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//空間雜訊最後濾波步長-2
	SysParam = SYSTEM_SPACE_NOISE_FINAL_FILTER_PITCH2;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//高度驗證-多投光差距比例上限-%		
	SysParam = SYSTEM_SPACE_VERIFY_MULTI_CAST_GAP_RATIO;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//高度驗證-多投光差距閥值-um
	SysParam = SYSTEM_SPACE_VERIFY_MULTI_CAST_GAP_THRESHOLD;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//啟用Cuda函式 
	SysParam = SYSTEM_CUDA_FUNCTION_ENABLED;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//Cude Block數量  
	SysParam = SYSTEM_CUDA_NUMBER_BLOCK;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//Cude Thread數量
	SysParam = SYSTEM_CUDA_NUMBER_THREAD;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//影像匹配函式庫樣式 
	SysParam = SYSTEM_LIBRARY_MODE_MATCH;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//Honeywell SwiftDecoder啟用
	SysParam = SYSTEM_HONEYWELL_SWIFT_DECODER_ENABLED;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//硬體鎖警告剩餘天數		
	SysParam = SYSTEM_DONGLE_WARNING_REMAINING_DAYS;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//硬體鎖警告剩餘次數
	SysParam = SYSTEM_DONGLE_WARNING_REMAINING_COUNT;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//與上一站連線方式
	SysParam = SYSTEM_CONNECT_LAST_STATION_MODE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//A軌道運轉模式
	SysParam = SYSTEM_LANE_WORK_MODEL_LA;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//B軌道運轉模式
	SysParam = SYSTEM_LANE_WORK_MODEL_LB;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//雙塔燈模式 
	SysParam = SYSTEM_MULTI_TOWER_LIGHT_MODE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	
	
	//確認PCB板移走次數
	SysParam = SYSTEM_CHECK_PCB_REMOVED_COUNT;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//確認PCB下一站連接輸送帶樣式
	SysParam = SYSTEM_NEXT_CONNECTED_BUFFER_TYPE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//多專案檢測次序模式		
	SysParam = SYSTEM_MULTI_PROJECT_TEST_ORDER_MODE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//在線輸入-專案工單號碼
	SysParam = SYSTEM_ONLINE_INPUT_PROJECT_WORK_NUMBER;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//在線自動校正DLP-LED-顏色
	SysParam = SYSTEM_ONLINE_AUTO_CALIBRATION_DLP_LED_COLOR;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//在線自動校正上蓋延遲時間-ms
	SysParam = SYSTEM_ONLINE_AUTO_CALIBRATION_CAP_DELAY_TIME;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//在線自動校正模式-XYZ歸零
	SysParam = SYSTEM_ONLINE_AUTO_CALIBRATION_MODE_XYZ_HOME;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//在線自動校正週期-XYZ歸零-小時
	SysParam = SYSTEM_ONLINE_AUTO_CALIBRATION_PERIOD_XYZ_HOME;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//在線自動校正模式-2D電流
	SysParam = SYSTEM_ONLINE_AUTO_CALIBRATION_MODE_2D_CURRENT;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//在線自動校正週期-2D電流-小時
	SysParam = SYSTEM_ONLINE_AUTO_CALIBRATION_PERIOD_2D_CURRENT;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//在線自動校正週期-3D電流-小時
	SysParam = SYSTEM_ONLINE_AUTO_CALIBRATION_PERIOD_3D_CURRENT;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//在線自動校正模式-3D相平面
	SysParam = SYSTEM_ONLINE_AUTO_CALIBRATION_MODE_3D_ZERO_PLANE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//在線自動校正週期-3D相平面-小時
	SysParam = SYSTEM_ONLINE_AUTO_CALIBRATION_PERIOD_3D_ZERO_PLANE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//在線自動校正週期-3D高度比例-小時
	SysParam = SYSTEM_ONLINE_AUTO_CALIBRATION_PERIOD_3D_HEIGHT_FACTOR;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//在線自動停機-閒置時間-分鐘		
	SysParam = SYSTEM_ONLINE_AUTO_STOP_BY_IDLE_TIME;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//在線自動停機-特定時間-時時分分秒秒		
	SysParam = SYSTEM_ONLINE_AUTO_STOP_BY_SPEC_TIME_1;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//在線自動停機-特定時間-時時分分秒秒		
	SysParam = SYSTEM_ONLINE_AUTO_STOP_BY_SPEC_TIME_2;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//在線自動停機-特定時間-時時分分秒秒
	SysParam = SYSTEM_ONLINE_AUTO_STOP_BY_SPEC_TIME_3;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//自動切換線上畫面-秒
	SysParam = SYSTEM_AUTO_SWITCH_TO_ONILINEVIEW_TIME;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//在線顯示專案檢測底圖
	SysParam = SYSTEM_ONLINE_SHOW_PROJECT_TEST_MAP;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//自動切換線上遠端控制時間-毫秒
	SysParam = SYSTEM_AUTO_SWITCH_TO_ONILINE_REMOTE_CTRL_TIME;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//自動切換線上遠端控制模式
	SysParam = SYSTEM_AUTO_SWITCH_TO_ONILINE_REMOTE_CTRL_MODE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//進板前移動相機頭
	SysParam = SYSTEM_MOVE_CAMERA_BEFORE_PCB_IN;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//進板使用相機影像模式
	SysParam = SYSTEM_PCB_IN_USE_CAMERA_IMAGE_MODE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//檢測前夾板模式	
	SysParam = SYSTEM_CLAMP_PCB_BEFORE_TEST_MODE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//擷取定位點影像延遲時間-ms
	SysParam = SYSTEM_GRAB_FIDUCIAL_DELAY_TIME;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//PCB出板方向
	SysParam = SYSTEM_PCB_OUT_DIRECTION;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	
	
	//忽略上一站訊號-回板使用		
	SysParam = SYSTEM_BYPASS_LAST_SIGNAL;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	
		
	//忽略下一站訊號-回板使用			
	SysParam = SYSTEM_BYPASS_NEXT_SIGNAL;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//PCB OK/NG訊號延遲時間-ms
	SysParam = SYSTEM_PCB_OK_NG_SIGNAL_DELAY_TIME;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//編輯線尺寸的層級	
	SysParam = SYSTEM_EDIT_LINE_SIZE_LEVEL;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//線上調機保留最久時間-分鐘 	
	SysParam = SYSTEM_ONLINE_TUNING_KEEP_MAX_TIME;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//線上調機儲存最多數量-片數 
	SysParam = SYSTEM_ONLINE_TUNING_SAVED_MAX_COUNT;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//使用者登入使用
	SysParam = SYSTEM_USER_LOGIN_ENABLED;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//使用者登入選項
	SysParam = SYSTEM_USER_LOGIN_OPTIONS;
	if (LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true)
	{	SetSystemParameterStringByID(SysParam, Param, String);	}

	//開啟專案模式
	SysParam = SYSTEM_OPEN_PROJECT_MODE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }
	
	//開啟專案底圖編號
	SysParam = SYSTEM_OPEN_PROJECT_MAP_INDEX;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//驗證專案模式
	SysParam = SYSTEM_VERIFY_PROJECT_MODE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//驗證專案的檔名
	SysParam = SYSTEM_VERIFY_PROJECT_FILENAME;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//模組名稱使用料號 
	SysParam = SYSTEM_MODEL_NAME_USE_PART_NUMBER;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//線上開專案模式		
	SysParam = SYSTEM_ONLINE_OPEN_PROJECT_MODE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//線上開專案-相機條碼
	SysParam = SYSTEM_ONLINE_OPEN_PROJECT_CAMERA_BARCODE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//線上開專案外接條碼取像模式
	SysParam = SYSTEM_ONLINE_OPEN_PROJECT_BARCODE_DEVICE_GRAB_MODE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//模組未設定顏色
	SysParam = SYSTEM_MODEL_UNSET_COLOR;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//2值化遮罩-顏色-3色燈
	SysParam = SYSTEM_BINARY_MASK_COLOR_RGB;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//2值化遮罩-顏色-預設
	SysParam = SYSTEM_BINARY_MASK_COLOR_DEFAULT;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//2值化遮罩-顏色-透明度			
	SysParam = SYSTEM_BINARY_MASK_COLOR_ALPHA;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//檢測結束顯示結果列表
	SysParam = SYSTEM_INSPECTION_FINISH_SHOW_RESULT_LIST;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//模組預設檢測框等級
	SysParam = SYSTEM_MODEL_DEFAULT_WND_LEVEL;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//可切換至專案3D畫面
	SysParam = SYSTEM_SWITCH_PROJECT_3D_FRAME;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	
	
	//自動切換至檢測框3D畫面
	SysParam = SYSTEM_AUTO_SWITCH_WND_3D_FRAME_MODE;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//自動切換至檢測框3D畫面尺寸上限-um	
	SysParam = SYSTEM_AUTO_SWITCH_WND_3D_FRAME_SIZE_LIMIT;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//顯示零件-整板零件
	SysParam = SYSTEM_SHOW_COMPONENT_FULL_MAP;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//僅顯示瑕疵零件(線上畫面)	
	SysParam = SYSTEM_SHOW_COMPONENT_DEFECT_ONLY;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//顯示除錯頁面
	SysParam = SYSTEM_SHOW_DEBUG_FORM_VIEW;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	
	
	//分等濾波起點偏移啟用
	SysParam = SYSTEM_LEVEL_FILTER_SHIFT_ENABLED;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//中值濾波起點偏移啟用
	SysParam = SYSTEM_MEDIAN_FILTER_SHIFT_ENABLED;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//平滑濾波起點偏移啟用
	SysParam = SYSTEM_SMOOTH_FILTER_SHIFT_ENABLED;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//金字塔中值濾波起點偏移啟用
	SysParam = SYSTEM_PYRAMID_MEDIAN_FILTER_SHIFT_ENABLED;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//分等濾波步長可接受誤差
	SysParam = SYSTEM_LEVEL_FILTER_PITCH_TOLERANCE;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//中值濾波步長可接受誤差
	SysParam = SYSTEM_MEDIAN_FILTER_PITCH_TOLERANCE;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//平滑濾波步長可接受誤差
	SysParam = SYSTEM_SMOOTH_FILTER_PITCH_TOLERANCE;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//金字塔中值濾波步長可接受誤差
	SysParam = SYSTEM_PYRAMID_MEDIAN_FILTER_PITCH_TOLERANCE;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//Grr偏移隨機補償值
	SysParam = SYSTEM_GRR_OFFSET_RANDOM_VALUE;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//Grr平均值復歸-啟用-依據條碼比較
	SysParam = SYSTEM_GRR_AVERAGE_RESET_ENABLED;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//Grr平均值復歸-匹配比例
	SysParam = SYSTEM_GRR_AVERAGE_RESET_MATCH_RATIO;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//Grr偏移角度平均啟用角度		
	SysParam = SYSTEM_GRR_SKEW_AVERAGE_ENB_VAL;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//Grr偏移角度平均開始差距-角度	
	SysParam = SYSTEM_GRR_SKEW_AVERAGE_START_GAP;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//Grr偏移角度平均權重 
	SysParam = SYSTEM_GRR_SKEW_AVERAGE_WEIGHTING;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//Grr偏移平均啟用像素
	SysParam = SYSTEM_GRR_OFFSET_AVERAGE_ENB_PXL;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//Grr偏移平均開始差距-微米	
	SysParam = SYSTEM_GRR_OFFSET_AVERAGE_START_GAP;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//Grr偏移平均權重
	SysParam = SYSTEM_GRR_OFFSET_AVERAGE_WEIGHTING;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//Grr偏移高度平均啟用高度
	SysParam = SYSTEM_GRR_HEIGHT_AVERAGE_ENB_VAL;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//Grr偏移高度平均開始差距-微米		
	SysParam = SYSTEM_GRR_HEIGHT_AVERAGE_START_GAP;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//Grr偏移高度平均權重 
	SysParam = SYSTEM_GRR_HEIGHT_AVERAGE_WEIGHTING;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//啟用鎖住螢幕
	SysParam = SYSTEM_LOCK_SCREEN_ENABLED;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//鎖住螢幕鍵號
	SysParam = SYSTEM_LOCK_SCREEN_KEY_ID;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//多段檢測啟用
	SysParam = SYSTEM_MULTI_DISTRICT_MODE_ENABLED;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//在線調機-軟體條碼
	SysParam = SYSTEM_ONLINE_TUNING_ENABLE_BARCODE;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//是否顯示PLC安全檢知設定介面
	SysParam = SYSTEM_SHOW_PLC_SAFTY_SETTING_UI;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//是否顯示演算法OffsetL的參數 	
	SysParam = SYSTEM_SHOW_ALG_OFFSET_L_PARAM;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//是否顯示演算法OffsetA的參數
	SysParam = SYSTEM_SHOW_ALG_OFFSET_A_PARAM;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//是否顯示演算法亮度比例的比例參數
	SysParam = SYSTEM_SHOW_ALG_BRIGHT_RATIO_SCALE_PARAM;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//是否顯示模組屬性參數
	SysParam = SYSTEM_SHOW_MODEL_PROPERTY_PARAM;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//連續貼上模式
	SysParam = SYSTEM_CONTINUE_PASTE_MODE;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//拼圖參數-填補尺寸
	SysParam = SYSTEM_STITCH_IMAGE_PADDING_SIZE;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//啟用Resin高度對齊-軍達3D對位
	SysParam = SYSTEM_RESIN_HEIGHT_ALIGN_ENABLED;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//啟用模組影像Cad偏移補償um
	SysParam = SYSTEM_MODEL_IMAGE_CAD_OFFSET_ENABLED;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//模組預設SOT樣式
	SysParam = SYSTEM_MODEL_DEFAULT_TRANSISTOR_TYPE;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//專案使用本機資料夾
	SysParam = SYSTEM_PROJECT_LOCAL_FOLDER_ENABLED;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//部分複製專案資料庫	
	SysParam = SYSTEM_PARTIAL_COPY_PROJECT_LIBRARY;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//自動重整模組底圖檔案
	SysParam = SYSTEM_AUTO_ARRANGE_MODEL_BK_IMAGE_FILES;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//自動複製SPC零件圖檔
	SysParam = SYSTEM_AUTO_COPY_SPC_COMPONENT_IMAGE_FILES;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//自動跳過3D影像	
	SysParam = SYSTEM_AUTO_BYPASS_GRAB_3D_FRAME;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//確認專案定位點狀態
	SysParam = SYSTEM_CHECK_PROJECT_FD_READY;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//鎖住模組本體的位置
	SysParam = SYSTEM_LOCK_MODEL_BODY_POSITION;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//最多未判定檢測檔案數 
	SysParam = SYSTEM_MAX_UNCHECK_TEST_FILE_COUNT;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//啟用零件條碼確認	
	SysParam = SYSTEM_CONFIRM_COMPONENT_BARCODE_ENABLED;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//儲存JPEG的質量(001~100)
	SysParam = SYSTEM_SAVE_JPEG_QUALITY;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//複製大量檔案模式
	SysParam = SYSTEM_COPY_HUGE_FILES_MODE;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//離線版本模式
	SysParam = SYSTEM_OFFLINE_VERSION_MODE;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//使用專案系統參數模式
	SysParam = SYSTEM_USE_PROJECT_SYSTEM_PARAM_MODE;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//UI視窗字型增加大小
	SysParam = SYSTEM_UI_WND_FONT_ADD_SIZE;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//UI駐停視窗滑動步長	
	SysParam = SYSTEM_UI_DOCK_WND_SLIDE_STEPS;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//UI啟用PCB出板按鈕
	SysParam = SYSTEM_UI_ENABLE_PCB_OUT_BUTTON;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//RabbitMQ伺服器Port		
	SysParam = SYSTEM_RABBITMQ_SERVER_PORT;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//RabbitMQ通訊網址		
	SysParam = SYSTEM_RABBITMQ_IP_ADDRESS;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//RabbitMQ登錄名稱		
	SysParam = SYSTEM_RABBITMQ_USER_NAME;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//RabbitMQ登錄密碼
	SysParam = SYSTEM_RABBITMQ_PASSWORD;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//ITS軟體名稱
	SysParam = SYSTEM_ITS_FILENAME;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//ITS通訊模式
	SysParam = SYSTEM_ITS_COMMUNICATION_MODE;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//ITS網路Port
	SysParam = SYSTEM_ITS_SOCKET_IP_PORT;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//ITS網路網址
	SysParam = SYSTEM_ITS_SOCKET_IP_ADDRESS;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//ITS訊息佇列接收名		
	SysParam = SYSTEM_ITS_RABBITMQ_QUEUE_NAME_RECV;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//ITS訊息佇列傳送名
	SysParam = SYSTEM_ITS_RABBITMQ_QUEUE_NAME_SEND;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//ITS-對接軟體
	SysParam = SYSTEM_ITS_CONTACT_SOFTWARE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//連線至ITS啟用
	SysParam = SYSTEM_ITS_COMMUNICATION_ENABLED;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//與ITS溝通逾時(ms)	
	SysParam = SYSTEM_ITS_COMMUNICATION_TIMEOUT_MS;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//ITS檔案資料夾-傳送		
	SysParam = SYSTEM_ITS_FILE_FOLDER_SEND;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//ITS檔案資料夾-接收
	SysParam = SYSTEM_ITS_FILE_FOLDER_RECV;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//ITS檔案資料夾-備份	
	SysParam = SYSTEM_ITS_FILE_BACKUP_ENABLED;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//ITS檔案資料夾-同步檔案
	SysParam = SYSTEM_ITS_FILE_USE_SYNC_FILE_ENABLED;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//ITS使用設定SECS/GEM		
	SysParam = SYSTEM_ITS_SET_SECS_GEM_ENABLED;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//ITS使用SECS/GEM-Remote Local
	SysParam = SYSTEM_ITS_SECS_GEM_REMOTE_LOCAL;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//ITS使用設定系統參數
	SysParam = SYSTEM_ITS_SET_SYSTEM_PARAM_ENABLED;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//ITS使用設定專案參數
	SysParam = SYSTEM_ITS_SET_PROJECT_PARAM_ENABLED;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//ITS使用設定機台狀態
	SysParam = SYSTEM_ITS_SET_MACHINE_STATUS_ENABLED;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//ITS使用設定軟體開關
	SysParam = SYSTEM_ITS_SET_APP_OPEN_CLOSE_ENABLED;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//ITS使用設定程序編號
	SysParam = SYSTEM_ITS_SET_PROCESS_ID_ENABLED;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//ITS使用設定使用者登入登出
	SysParam = SYSTEM_ITS_SET_USER_LOGIN_OUT_ENABLED;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//專案預設高度轉灰階比例 
	SysParam = SYSTEM_PROG_DEFAULT_SPACE_TO_GRAY_RATIO_MODE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//專案預設空間基準面編號 	
	SysParam = SYSTEM_PROG_DEFAULT_SPACE_BASE_PLANE_INDEX;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//專案預設空間雜訊過濾編號
	SysParam = SYSTEM_PROG_DEFAULT_SPACE_NOISE_FILTER_INDEX;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//專案預設軌道提前運轉功能
	SysParam = SYSTEM_PROG_DEFAULT_ENABLE_CONVEYER_PRE_RUN;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }	

	//專案預設定位點異常處理模式
	SysParam = SYSTEM_PROG_DEFAULT_FD_NG_HANDLE_MODE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//專案預設單板定位點取像模式
	SysParam = SYSTEM_PROG_DEFAULT_BOARD_FD_GRAB_MODE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//專案預設檢出異常處理模式
	SysParam = SYSTEM_PROG_DEFAULT_BDEFECT_HANDLE_MODE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//專案預設PCB出板模式
	SysParam = SYSTEM_PROG_DEFAULT_PCB_OUT_MODE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//專案預設儲存檢測底圖模式	
	SysParam = SYSTEM_PROG_DEFAULT_PROJECT_SAVE_TEST_MAP;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//專案預設連線伺服器模式
	SysParam = SYSTEM_PROG_DEFAULT_PROJECT_LINK_SERVER_MODE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//專案預設儲存離線圖檔
	SysParam = SYSTEM_PROG_DEFAULT_SAVE_OFFLINE_IMAGE_FILES;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//專案預設條碼驗證模式		
	SysParam = SYSTEM_PROG_DEFAULT_BARCODE_VERIFY_MODE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//專案預設條碼查詢模式
	SysParam = SYSTEM_PROG_DEFAULT_BARCODE_RETRIEVE_MODE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//操作等級-專案開啟
	SysParam = SYSTEM_OPERATE_LEVEL_PROJECT_OPEN;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//操作等級-專案儲存	
	SysParam = SYSTEM_OPERATE_LEVEL_PROJECT_SAVE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//操作等級-專案參數
	SysParam = SYSTEM_OPERATE_LEVEL_PROJECT_PARAM;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//操作等級-線上運行
	SysParam = SYSTEM_OPERATE_LEVEL_ONLINE_RUN;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//操作等級-線上直通
	SysParam = SYSTEM_OPERATE_LEVEL_ONLINE_BYPASS;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//操作等級-線上停止
	SysParam = SYSTEM_OPERATE_LEVEL_ONLINE_STOP;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//操作等級-線上存圖
	SysParam = SYSTEM_OPERATE_LEVEL_ONLINE_SAVE_IMAGE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//操作等級-線上解鎖
	SysParam = SYSTEM_OPERATE_LEVEL_ONLINE_UNLOCK;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//操作等級-編輯功能-新增
	SysParam = SYSTEM_OPERATE_LEVEL_EDIT_FUNC_ADD;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//操作等級-編輯功能-刪除
	SysParam = SYSTEM_OPERATE_LEVEL_EDIT_FUNC_DEL;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//操作等級-編輯功能-不檢測
	SysParam = SYSTEM_OPERATE_LEVEL_EDIT_FUNC_BYPASS;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//操作等級-MES相關-控制狀態-離線		
	SysParam = SYSTEM_OPERATE_LEVEL_MES_CTRL_STATE_OFFLINE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//操作等級-MES相關-控制狀態-本地上線
	SysParam = SYSTEM_OPERATE_LEVEL_MES_CTRL_STATE_LOCAL;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//操作等級-MES相關-控制狀態-遠端上線
	SysParam = SYSTEM_OPERATE_LEVEL_MES_CTRL_STATE_REMOTE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//操作等級-MES相關-顯示內容
	SysParam = SYSTEM_OPERATE_LEVEL_MES_SHOW_CONTEXT;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//啟用NPM-條碼
	SysParam = SYSTEM_M2M_NPM_BARCODE_ENABLE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//啟用NPM APC-FF1		
	SysParam = SYSTEM_M2M_NPM_APC_FF1_ENABLE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//啟用NPM APC-FF2
	SysParam = SYSTEM_M2M_NPM_APC_FF2_ENABLE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//啟用NPM APC-MFB
	SysParam = SYSTEM_M2M_NPM_APC_MFB_ENABLE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//NPM軌道名稱-A軌	
	SysParam = SYSTEM_M2M_NPM_LANE_NAME_LA;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//NPM軌道名稱-B軌	
	SysParam = SYSTEM_M2M_NPM_LANE_NAME_LB;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//NPM-輸入共享資料夾-A軌
	SysParam = SYSTEM_M2M_NPM_INPUT_SHARE_FOLDER_LA;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//NPM-輸入共享資料夾-B軌
	SysParam = SYSTEM_M2M_NPM_INPUT_SHARE_FOLDER_LB;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//NPM-輸出共享資料夾-A軌		
	SysParam = SYSTEM_M2M_NPM_OUTPUT_SHARE_FOLDER_LA;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//NPM-輸出共享資料夾-B軌		
	SysParam = SYSTEM_M2M_NPM_OUTPUT_SHARE_FOLDER_LB;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//啟用M2M HAS I (Hanwha AOI Solution MAOI_SAOI)
	SysParam = SYSTEM_M2M_HASI_ENABLE;
	if (LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true)
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//HAS I 共享資料夾	
	SysParam = SYSTEM_M2M_HASI_SHARE_FOLDER;
	if (LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true)
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//啟用 HAS I - SerialFile
	SysParam = SYSTEM_M2M_HASI_SERIALFILE_ENABLE;
	if (LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true)
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//HAS I - SerialFile - Size of Serial Queue
	SysParam = SYSTEM_M2M_HASI_SERIALFILE_QUEUESIZE;
	if (LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true)
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//等待資料的延遲時間-ms
	SysParam = SYSTEM_M2M_HASI_SERIALFILE_DWELLTIME;
	if (LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true)
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//啟用 HAS I - SPIOffsetFile
	SysParam = SYSTEM_M2M_HASI_SPIOFFSETFILE_ENABLE;
	if (LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true)
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//產線上 AOI 的檢查階段
	SysParam = SYSTEM_M2M_HASI_AOI_STAGE;
	if (LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true)
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//啟用 HAS I - PNP
	SysParam = SYSTEM_M2M_HASI_PNP_ENABLE;
	if (LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true)
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//AI模型伺服器啟用
	SysParam = SYSTEM_AI_MODEL_SERVER_ENABLE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//AI模型伺服器逾時
	SysParam = SYSTEM_AI_MODEL_SERVER_TIMEOUT;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//AI模型伺服器檔名
	SysParam = SYSTEM_AI_MODEL_SERVER_FILENAME;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//AI模型檔案資料夾-傳送
	SysParam = SYSTEM_AI_MODEL_FILE_FOLDER_SEND;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//AI模型檔案資料夾-接收
	SysParam = SYSTEM_AI_MODEL_FILE_FOLDER_RECV;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//AI模型分類最小叢集距離-um
	SysParam = SYSTEM_AI_MODEL_LABEL_MIN_CLUSTER_DISTANCE;
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//外部複製檔案啟用
	SysParam = SYSTEM_EXTERNAL_COPY_FILE_ENABLED;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//外部複製檔案軟體名稱
	SysParam = SYSTEM_EXTERNAL_COPY_FILE_APP_NAME;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//外部複製檔案輸出資料夾
	SysParam = SYSTEM_EXTERNAL_COPY_FILE_SEND_FOLDER;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//Cpk圖表啟用
	SysParam = SYSTEM_CPK_CHART_ENABLED;	
	if ( LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true )
	{	SetSystemParameterStringByID(SysParam, Param, String); }

	//檢測框跟隨旋轉
	SysParam = SYSTEM_WND_ROTATION_FOLLOWED;
	if (LoadSystemParamNode(SysParam, Param, String, textlen, FileName) == true)
	{	SetSystemParameterStringByID(SysParam, Param, String); }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::LoadSystemParamNode(SYSTEM_PARAM_ID SysParam, const TSystemParameter &Param, TCHAR String[], int textlen, LPCTSTR filename)//載入系統參數
{		
	CString Section;
	CString KeyName;
	CString Default;
	ObtainSystemParameterKeyName(SysParam, Section, KeyName);
	GetSystemParameterStringByID(SysParam, Param, Default);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, filename, false, m_ErrorString) == false ) 	
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::SaveCalibrationParameter()//儲存校正參數
{	
	CString Filename;
	Filename = GetSystemParamFilename();
	if ( SaveCalibrationParamFile(Filename, m_CalibrationParameter) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::SaveCalibrationParamFile(LPCTSTR filename, const TCalibrationParameter &CaliParam)//儲存校正參數
{
	if ( SaveCalibrationParamFileFn(filename, CaliParam) == false )
	{
		SetSystemExceptionCode_FileWrite();
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::SaveCalibrationParamFileFn(LPCTSTR filename, const TCalibrationParameter &CaliParam)//儲存校正參數
{
#ifndef OFFLINE_VERSION
	if ( NULL == filename ) 
	{
		m_ErrorString = _T("Error, SaveCalibrationParamFile No Filename");
		return false;
	}

	int i=0;
	CString str;
	CString Section;
	CString KeyName;
	CString String;
	CString FileName;	
	const double ResolutionValue = GetFovResolutionValueFn(CaliParam.m_ResolutionMode);//取得視野解析度-理論解析度

	FileName = filename;
	Section = GetCalibrationSectionName();

	//格線規座標-X
	KeyName = _T("Target Grid Position X");
	String.Format(_T("%.4f"), CaliParam.m_TargetGridPosX);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }

	//格線規座標-Y
	KeyName = _T("Target Grid Position Y");
	String.Format(_T("%.4f"), CaliParam.m_TargetGridPosY);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }

	//格線規座標-Z
	KeyName = _T("Target Grid Position Z");
	String.Format(_T("%.4f"), CaliParam.m_TargetGridPosZ);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }

	//格線塊位置-曝光時間us
	KeyName = _T("Target Grid Exposure Time");
	String.Format(_T("%d"), CaliParam.m_TargetGridExpTime_us);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }

	//方形規座標-X
	KeyName = _T("Target Rectangle Position X");
	String.Format(_T("%.4f"), CaliParam.m_TargetRectPosX);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }

	//方形規座標-Y
	KeyName = _T("Target Rectangle Position Y");
	String.Format(_T("%.4f"), CaliParam.m_TargetRectPosY);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }

	//方形規座標-Z
	KeyName = _T("Target Rectangle Position Z");
	String.Format(_T("%.4f"), CaliParam.m_TargetRectPosZ);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }

	//格線塊位置-曝光時間us
	KeyName = _T("Target Rectangle Exposure Time");
	String.Format(_T("%d"), CaliParam.m_TargetRectExpTime_us);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }

	//白色規座標-X
	KeyName = _T("Target White Position X");
	String.Format(_T("%.4f"), CaliParam.m_TargetWhitePosX);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }

	//白色規座標-Y
	KeyName = _T("Target White Position Y");
	String.Format(_T("%.4f"), CaliParam.m_TargetWhitePosY);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }

	//白色規座標-Z
	KeyName = _T("Target White Position Z");
	String.Format(_T("%.4f"), CaliParam.m_TargetWhitePosZ);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }

	//格線塊位置-曝光時間us
	KeyName = _T("Target White Exposure Time");
	String.Format(_T("%d"), CaliParam.m_TargetWhiteExpTime_us);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }

	//高度規座標-X
	KeyName = _T("Target Height Position X");
	String.Format(_T("%.4f"), CaliParam.m_TargetHeightPosX);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }

	//高度規座標-Y
	KeyName = _T("Target Height Position Y");
	String.Format(_T("%.4f"), CaliParam.m_TargetHeightPosY);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }

	//高度規座標-Z
	KeyName = _T("Target Height Position Z");
	String.Format(_T("%.4f"), CaliParam.m_TargetHeightPosZ);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }	

	//格線塊位置-曝光時間us
	KeyName = _T("Target Height Exposure Time");
	String.Format(_T("%d"), CaliParam.m_TargetHeightExpTime_us);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }	

	//相平面偏差位置-Z
	KeyName = _T("Phase Zero Plane Offset Z Value");
	String.Format(_T("%.4f"), CaliParam.m_PhaseZeroPlaneOffsetPosZ);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }	
	
	//3D投光電流校正灰階值
	KeyName = _T("3D Cast Current Gray Value");
	String.Format(_T("%d"), CaliParam.m_3DCastCurrentGray);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }	

	//3D投光電流校正間距
	KeyName = _T("3D Cast Current Pitch");
	String.Format(_T("%d"), CaliParam.m_3DCastCurrentPitch);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }	

	//3D投光電流校正曝光時間us
	KeyName = _T("3D Cast Current Exposure Time us");
	String.Format(_T("%d"), CaliParam.m_3DCastCurrentExpTime_us);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }	

	//3D投光的安裝角度
	KeyName = _T("3D Cast Mount Angle");
	String.Format(_T("%.4f"), CaliParam.m_3DCastMountAngle);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }	

	//3D投光的高度校正網格數量-欄數
	KeyName = _T("3D Cast Height Factor Grid Cols");
	String.Format(_T("%d"), CaliParam.m_3DCastHeightFactorGridCols);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }	

	//3D投光的高度校正網格數量-列數
	KeyName = _T("3D Cast Height Factor Grid Rows");
	String.Format(_T("%d"), CaliParam.m_3DCastHeightFactorGridRows);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }	

	//3D塊規高度值
	KeyName = _T("Target Hegiht Thick Value");
	String.Format(_T("%.4f"), CaliParam.m_TargetHeightThickValue);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }	
	
	//相位參數視野的間距
	KeyName = _T("Phase Height Factor FOV Z-Pitch");
	String.Format(_T("%.4f"), CaliParam.m_PhaseFactorFovPitch);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }

	//相位參數視野的範圍
	KeyName = _T("Phase Height Factor FOV Z-Range");
	String.Format(_T("%.4f"), CaliParam.m_PhaseFactorFovRange);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }
	
	//啟用相平面濾波
	KeyName = _T("Enable Base Phase Correct");
	String.Format(_T("%d"), CaliParam.m_EnableBasePhaseCorrect);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }
	
	//基本相平面濾波次數
	KeyName = _T("Base Phase Correct Times");
	String.Format(_T("%d"), CaliParam.m_BasePhaseCorrectTimes);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }

	//啟用高度係數修正
	KeyName = _T("Enable Height Factor Correct");
	String.Format(_T("%d"), CaliParam.m_EnableHeightFactorCorrect);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }		

	//高度係數修正次數
	KeyName = _T("Height Factor Correct Times");
	String.Format(_T("%d"), CaliParam.m_HeightFactorCorrectTimes);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }	

	//高度係數校正使用FOV模式	 
	KeyName = _T("Height Factor Calibrate With FOV");
	String.Format(_T("%d"), CaliParam.m_HeightFactorCalibrateWithFOV);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }	

	//影像解析度
	KeyName = _T("Image Resolution Mode");
	String.Format(_T("%d"), CaliParam.m_ResolutionMode);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }	

	//視野寬度
	KeyName = _T("FOV Width");
	String.Format(_T("%.4f"), CaliParam.m_FOVWidth_um);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }	
	
	//視野寬度
	KeyName = _T("FOV Height");
	String.Format(_T("%.4f"), CaliParam.m_FOVHeight_um);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }	
	
	//視野寬度
	KeyName.Format(_T("FOV Width for %.2f um"), ResolutionValue);
	String.Format(_T("%.4f"), CaliParam.m_FOVWidth_um);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }	

	//視野寬度	
	KeyName.Format(_T("FOV Height for %.2f um"), ResolutionValue);
	String.Format(_T("%.4f"), CaliParam.m_FOVHeight_um);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }	

	//曝光時間
	KeyName = _T("Exposure Time");
	String.Format(_T("%d"), CaliParam.m_ExposureTime_us);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }		

	//定位點曝光時間
	KeyName = _T("Fd Exposure Time");
	String.Format(_T("%d"), CaliParam.m_FdExposureTime_us);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }		
	
	//樣板曝光時間
	KeyName = _T("DLP Exposure Time");
	String.Format(_T("%d"), CaliParam.m_DLPExposureTime_us);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }	

	//樣板曝光時間 2
	KeyName = _T("DLP Exposure Time 2");
	String.Format(_T("%d"), CaliParam.m_DLPExposureTime2_us);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }		
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::LoadCalibrationParameter()//載入校正參數
{
	CString Filename;
	Filename = GetSystemParamFilename();
	//支援舊版本	
	int OldResolutionMode = m_SystemParameter.m_ResolutionModeTmp;
	m_CalibrationParameter.m_ResolutionMode = OldResolutionMode;
	m_CalibrationParameter.m_ResolutionMode_Offline = OldResolutionMode;
	if ( LoadCalibrationParamFile(Filename, m_CalibrationParameter) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::LoadCalibrationParamFile(LPCTSTR filename, TCalibrationParameter &CaliParam)//載入校正參數
{
	if ( LoadCalibrationParamFileFn(filename, CaliParam) == false )
	{
		SetSystemExceptionCode_FileRead();
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::LoadCalibrationParamFileFn(LPCTSTR filename, TCalibrationParameter &CaliParam)//載入校正參數
{
	if ( NULL == filename ) 
	{
		m_ErrorString = _T("Error, LoadCalibrationParamFile No Filename");
		return false;
	}

	const size_t textlen = 128;
	CString str;
	CString FileName;
	CString Section = _T("");	
	CString KeyName = _T("");
	CString Default = _T("");
	TCHAR   String[textlen]=_T("");	
	
	FileName = filename;
	Section = GetCalibrationSectionName();
	//::_tcstod(String, NULL);	

	//格線規座標-X
	KeyName = _T("Target Grid Position X");
	Default.Format(_T("%.4f"), CaliParam.m_TargetGridPosX);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	CaliParam.m_TargetGridPosX = ::_tcstod(String, NULL); }

	//格線規座標-Y
	KeyName = _T("Target Grid Position Y");
	Default.Format(_T("%.4f"), CaliParam.m_TargetGridPosY);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	CaliParam.m_TargetGridPosY = ::_tcstod(String, NULL); }

	//格線規座標-Z
	KeyName = _T("Target Grid Position Z");
	Default.Format(_T("%.4f"), CaliParam.m_TargetGridPosZ);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	CaliParam.m_TargetGridPosZ = ::_tcstod(String, NULL); }

	//格線塊位置-曝光時間us
	KeyName = _T("Target Grid Exposure Time");
	Default.Format(_T("%d"), CaliParam.m_TargetGridExpTime_us);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	CaliParam.m_TargetGridExpTime_us = ::_ttoi(String); }

	//方形規座標-X
	KeyName = _T("Target Rectangle Position X");
	Default.Format(_T("%.4f"), CaliParam.m_TargetRectPosX);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	CaliParam.m_TargetRectPosX = ::_tcstod(String, NULL); }

	//方形規座標-Y
	KeyName = _T("Target Rectangle Position Y");
	Default.Format(_T("%.4f"), CaliParam.m_TargetRectPosY);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	CaliParam.m_TargetRectPosY = ::_tcstod(String, NULL); }

	//方形規座標-Z
	KeyName = _T("Target Rectangle Position Z");
	Default.Format(_T("%.4f"), CaliParam.m_TargetRectPosZ);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	CaliParam.m_TargetRectPosZ = ::_tcstod(String, NULL); }

	//格線塊位置-曝光時間us
	KeyName = _T("Target Rectangle Exposure Time");
	Default.Format(_T("%d"), CaliParam.m_TargetRectExpTime_us);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	CaliParam.m_TargetRectExpTime_us = ::_ttoi(String); }

	//白色規座標-X
	KeyName = _T("Target White Position X");
	Default.Format(_T("%.4f"), CaliParam.m_TargetWhitePosX);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	CaliParam.m_TargetWhitePosX = ::_tcstod(String, NULL); }

	//白色規座標-Y
	KeyName = _T("Target White Position Y");
	Default.Format(_T("%.4f"), CaliParam.m_TargetWhitePosY);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	CaliParam.m_TargetWhitePosY = ::_tcstod(String, NULL); }

	//白色規座標-Z
	KeyName = _T("Target White Position Z");
	Default.Format(_T("%.4f"), CaliParam.m_TargetWhitePosZ);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	CaliParam.m_TargetWhitePosZ = ::_tcstod(String, NULL); }

	//格線塊位置-曝光時間us
	KeyName = _T("Target White Exposure Time");
	Default.Format(_T("%d"), CaliParam.m_TargetWhiteExpTime_us);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	CaliParam.m_TargetWhiteExpTime_us = ::_ttoi(String); }

	//高度規座標-X
	KeyName = _T("Target Height Position X");
	Default.Format(_T("%.4f"), CaliParam.m_TargetHeightPosX);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	CaliParam.m_TargetHeightPosX = ::_tcstod(String, NULL); }

	//高度規座標-Y
	KeyName = _T("Target Height Position Y");
	Default.Format(_T("%.4f"), CaliParam.m_TargetHeightPosY);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	CaliParam.m_TargetHeightPosY = ::_tcstod(String, NULL); }

	//高度規座標-Z
	KeyName = _T("Target Height Position Z");
	Default.Format(_T("%.4f"), CaliParam.m_TargetHeightPosZ);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	CaliParam.m_TargetHeightPosZ = ::_tcstod(String, NULL); }

	//格線塊位置-曝光時間us
	KeyName = _T("Target Height Exposure Time");
	Default.Format(_T("%d"), CaliParam.m_TargetHeightExpTime_us);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	CaliParam.m_TargetHeightExpTime_us = ::_ttoi(String); }
	
	//相平面偏差位置-Z
	KeyName = _T("Phase Zero Plane Offset Z Value");
	Default.Format(_T("%.4f"), CaliParam.m_PhaseZeroPlaneOffsetPosZ);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	CaliParam.m_PhaseZeroPlaneOffsetPosZ = ::_tcstod(String, NULL); }

	//3D投光電流校正灰階值
	KeyName = _T("3D Cast Current Gray Value");
	Default.Format(_T("%d"), CaliParam.m_3DCastCurrentGray);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	CaliParam.m_3DCastCurrentGray = ::_ttoi(String); }	

	//3D投光電流校正間距
	KeyName = _T("3D Cast Current Pitch");
	Default.Format(_T("%d"), CaliParam.m_3DCastCurrentPitch);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	
		CaliParam.m_3DCastCurrentPitch = ::_ttoi(String); 
		if ( CaliParam.m_3DCastCurrentPitch < 1 ) 
		{	CaliParam.m_3DCastCurrentPitch = 1; }
	}

	//3D投光電流校正曝光時間us
	KeyName = _T("3D Cast Current Exposure Time us");
	Default.Format(_T("%d"), CaliParam.m_3DCastCurrentExpTime_us);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	CaliParam.m_3DCastCurrentExpTime_us = ::_ttoi(String); }	

	//3D投光的安裝角度
	KeyName = _T("3D Cast Mount Angle");
	Default.Format(_T("%.4f"), CaliParam.m_3DCastMountAngle);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	CaliParam.m_3DCastMountAngle = ::_tcstod(String, NULL); }

	//3D投光的高度校正網格數量-欄數
	KeyName = _T("3D Cast Height Factor Grid Cols");
	Default.Format(_T("%d"), CaliParam.m_3DCastHeightFactorGridCols);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	CaliParam.m_3DCastHeightFactorGridCols = ::_ttoi(String); }

	//3D投光的高度校正網格數量-列數
	KeyName = _T("3D Cast Height Factor Grid Rows");
	Default.Format(_T("%d"), CaliParam.m_3DCastHeightFactorGridRows);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	CaliParam.m_3DCastHeightFactorGridRows = ::_ttoi(String); }

	//3D塊規高度值
	KeyName = _T("Target Hegiht Thick Value");
	Default.Format(_T("%.4f"), CaliParam.m_TargetHeightThickValue);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	CaliParam.m_TargetHeightThickValue = ::_tcstod(String, NULL); }
	
	KeyName = _T("Phase Height Factor FOV Z-Pitch");
	Default.Format(_T("%.4f"), CaliParam.m_PhaseFactorFovPitch);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	CaliParam.m_PhaseFactorFovPitch = ::_tcstod(String, NULL); }

	KeyName = _T("Phase Height Factor FOV Z-Range");
	Default.Format(_T("%.4f"), CaliParam.m_PhaseFactorFovRange);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	CaliParam.m_PhaseFactorFovRange = ::_tcstod(String, NULL); }

	//啟用相平面濾波
	KeyName = _T("Enable Base Phase Correct");
	Default.Format(_T("%d"), CaliParam.m_EnableBasePhaseCorrect);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	CaliParam.m_EnableBasePhaseCorrect = ::_ttoi(String); }
	
	//基本相平面濾波次數
	KeyName = _T("Base Phase Correct Times");	
	Default.Format(_T("%d"), CaliParam.m_BasePhaseCorrectTimes);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	CaliParam.m_BasePhaseCorrectTimes = ::_ttoi(String); }

	//啟用高度係數修正
	KeyName = _T("Enable Height Factor Correct");
	Default.Format(_T("%d"), CaliParam.m_EnableHeightFactorCorrect);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	CaliParam.m_EnableHeightFactorCorrect = ::_ttoi(String); }

	//高度係數修正次數
	KeyName = _T("Height Factor Correct Times");
	Default.Format(_T("%d"), CaliParam.m_HeightFactorCorrectTimes);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	CaliParam.m_HeightFactorCorrectTimes = ::_ttoi(String); }

	//高度係數校正使用FOV模式	 
	KeyName = _T("Height Factor Calibrate With FOV");
	Default.Format(_T("%d"), CaliParam.m_HeightFactorCalibrateWithFOV);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	CaliParam.m_HeightFactorCalibrateWithFOV = ::_ttoi(String); }

	//影像解析度
	KeyName = _T("Image Resolution Mode");
	Default.Format(_T("%d"), CaliParam.m_ResolutionMode);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	CaliParam.m_ResolutionMode = ::_ttoi(String); }
#ifdef OFFLINE_VERSION
	CaliParam.m_ResolutionMode_Offline = CaliParam.m_ResolutionMode;
#endif//OFFLINE_VERSION
	const double ResolutionValue = GetFovResolutionValueFn(CaliParam.m_ResolutionMode);//取得視野解析度-理論解析度

	//視野寬度
	KeyName = _T("FOV Width");
	Default.Format(_T("%.4f"), CaliParam.m_FOVWidth_um);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	CaliParam.m_FOVWidth_um = ::_tcstod(String, NULL); }

	//視野寬度
	KeyName = _T("FOV Height");
	Default.Format(_T("%.4f"), CaliParam.m_FOVHeight_um);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	CaliParam.m_FOVHeight_um = ::_tcstod(String, NULL); }
	
	//視野寬度
	KeyName.Format(_T("FOV Width for %.2f um"), ResolutionValue);
	Default.Format(_T("%.4f"), CaliParam.m_FOVWidth_um);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	CaliParam.m_FOVWidth_um = ::_tcstod(String, NULL); }	

	//視野寬度	
	KeyName.Format(_T("FOV Height for %.2f um"), ResolutionValue);
	Default.Format(_T("%.4f"), CaliParam.m_FOVHeight_um);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	CaliParam.m_FOVHeight_um = ::_tcstod(String, NULL); }

	//曝光時間
	KeyName = _T("Exposure Time");
	Default.Format(_T("%d"), CaliParam.m_ExposureTime_us);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	CaliParam.m_ExposureTime_us = ::_ttoi(String); }

	//定位點曝光時間
	KeyName = _T("Fd Exposure Time");
	Default.Format(_T("%d"), CaliParam.m_FdExposureTime_us);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	CaliParam.m_FdExposureTime_us = ::_ttoi(String); }

	//樣板曝光時間
	KeyName = _T("DLP Exposure Time");
	Default.Format(_T("%d"), CaliParam.m_DLPExposureTime_us);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	CaliParam.m_DLPExposureTime_us = ::_ttoi(String); }
	CaliParam.m_DLPExposureTime2_us = CaliParam.m_DLPExposureTime_us;

	//樣板曝光時間 2
	KeyName = _T("DLP Exposure Time 2");
	Default.Format(_T("%d"), CaliParam.m_DLPExposureTime2_us);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	CaliParam.m_DLPExposureTime2_us = ::_ttoi(String); }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::SaveSystemImageParameter()//儲存影像參數
{
	CString Filename;
	Filename = GetSystemParamFilename();
	if ( SaveSystemImageParamFile(Filename, m_ImageParameter) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::SaveSystemImageParamFile(LPCTSTR filename, const TImageParameter &ImageParam)//儲存影像參數
{
	if ( SaveSystemImageParamFileFn(filename, ImageParam) == false )
	{
		SetSystemExceptionCode_FileWrite();
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::SaveSystemImageParamFileFn(LPCTSTR filename, const TImageParameter &ImageParam)//儲存影像參數
{
	if ( NULL == filename ) 
	{
		m_ErrorString = _T("Error, SaveSystemImageParamFile No Filename");
		return false;
	}

	int i=0;
	CString str;
	CString Section;
	CString KeyName;
	CString String;
	CString FileName;		
	
	FileName = filename;
	Section = _T("Image Parameter");	

	//影像縮放最小值
	KeyName = _T("Image Zoom Min");
	String.Format(_T("%.4f"), ImageParam.m_ImageZoomMin);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }

	//影像縮放最大值
	KeyName = _T("Image Zoom Max");
	String.Format(_T("%.4f"), ImageParam.m_ImageZoomMax);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }

	//專案影像最大寬度
	KeyName = _T("Project Image Max Width");
	String.Format(_T("%d"), ImageParam.m_ProjectImageMaxWidth);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }

	//專案影像最大高度
	KeyName = _T("Project Image Max Height");
	String.Format(_T("%d"), ImageParam.m_ProjectImageMaxHeight);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::LoadSystemImageParameter()//載入影像參數
{
	CString Filename;
	Filename = GetSystemParamFilename();
	if ( LoadSystemImageParamFile(Filename, m_ImageParameter) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::LoadSystemImageParamFile(LPCTSTR filename, TImageParameter &ImageParam)//載入影像參數
{
	if ( LoadSystemImageParamFileFn(filename, ImageParam) == false )
	{
		SetSystemExceptionCode_FileRead();
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::LoadSystemImageParamFileFn(LPCTSTR filename, TImageParameter &ImageParam)//載入影像參數
{
	if ( NULL == filename ) 
	{
		m_ErrorString = _T("Error, LoadSystemImageParamFile No Filename");
		return false;
	}

	const size_t textlen = 128;
	CString str;
	CString FileName;
	CString Section = _T("");	
	CString KeyName = _T("");
	CString Default = _T("");
	TCHAR   String[textlen]=_T("");		

	FileName = filename;
	Section = _T("Image Parameter");	
	//::_tcstod(String, NULL);	

	//影像縮放最小值
	KeyName = _T("Image Zoom Min");
	Default.Format(_T("%.4f"), ImageParam.m_ImageZoomMin);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	ImageParam.m_ImageZoomMin = ::_tcstod(String, NULL); }

	//影像縮放最大值
	KeyName = _T("Image Zoom Max");
	Default.Format(_T("%.4f"), ImageParam.m_ImageZoomMax);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	ImageParam.m_ImageZoomMax = ::_tcstod(String, NULL); }

	//專案影像最大寬度
	KeyName = _T("Project Image Max Width");
	Default.Format(_T("%d"), ImageParam.m_ProjectImageMaxWidth);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	ImageParam.m_ProjectImageMaxWidth = ::_ttoi(String); }	

	//專案影像最大高度
	KeyName = _T("Project Image Max Height");
	Default.Format(_T("%d"), ImageParam.m_ProjectImageMaxHeight);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	ImageParam.m_ProjectImageMaxHeight = ::_ttoi(String); }		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::SaveSystemBarcodeParameter()//儲存條碼機參數
{	
	CString Filename;
	Filename = GetSystemParamFilename();
	if ( SaveSystemBarcodeParamFile(Filename, m_BarcodeParam, m_BarcodeDeviceIDList_LA, m_BarcodeDeviceIDList_LB)==false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::SaveSystemBarcodeParamFile(LPCTSTR filename, const TBarcodeDevice BarcodeParamList[], int IDList_LA[], int IDList_LB[])//儲存條碼機參數
{
	if ( SaveSystemBarcodeParamFileFn(filename, BarcodeParamList, IDList_LA, IDList_LB) == false )
	{
		SetBarcodeDeviceExceptionCode_FileWrite();
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::SaveSystemBarcodeParamFileFn(LPCTSTR filename, const TBarcodeDevice BarcodeParamList[], int IDList_LA[], int IDList_LB[])//儲存條碼機參數
{
	if ( NULL == filename ) 
	{
		m_ErrorString = _T("Error, SaveSystemBarcodeParamFile No Filename");
		return false;
	}

	size_t  i=0;	
	CString str;
	CString Section;
	CString KeyName;
	CString String;
	CString FileName;
	TBarcodeDevice BarcodeDevice;
	const size_t MaxDeviceCount = MAX_BARCODE_DEVICE_COUNT;		
	
	FileName = filename;
	for ( i=0; i<MaxDeviceCount; i++ )
	{
		BarcodeDevice = BarcodeParamList[i];
		Section.Format(_T("%s %d"), _T("Barcode Device"), BarcodeDevice.nDeviceID);
		
		//條碼機樣式
		KeyName = _T("Device Type");
		String.Format(_T("%d"), BarcodeDevice.eDevieType);
		if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
		{	return false; }

		//連接埠		
		KeyName = _T("Connection Port");
		String = BarcodeDevice.sDevicePort;
		if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
		{	return false; }
	}

	Section = _T("Barcode Device ID Lane A");
	for ( i=0; i<MaxDeviceCount; i++ )
	{			
		KeyName.Format(_T("Lane A Barcode Device %d"), i+1);
		String.Format(_T("%d"), IDList_LA[i]);
		if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
		{	return false; }
	}

	Section = _T("Barcode Device ID Lane B");
	for ( i=0; i<MaxDeviceCount; i++ )
	{			
		KeyName.Format(_T("Lane B Barcode Device %d"), i+1);
		String.Format(_T("%d"), IDList_LB[i]);
		if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
		{	return false; }
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::LoadSystemBarcodeParameter()//載入條碼機參數
{
	CString Filename;
	Filename = GetSystemParamFilename();
	if ( LoadSystemBarcodeParamFile(Filename, m_BarcodeParam, m_BarcodeDeviceIDList_LA, m_BarcodeDeviceIDList_LB)==false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::LoadSystemBarcodeParamFile(LPCTSTR filename, TBarcodeDevice BarcodeParamList[], int IDList_LA[], int IDList_LB[])//載入條碼機參數
{
	if ( LoadSystemBarcodeParamFileFn(filename, BarcodeParamList, IDList_LA, IDList_LB) == false )
	{
		SetBarcodeDeviceExceptionCode_FileRead();
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::LoadSystemBarcodeParamFileFn(LPCTSTR filename, TBarcodeDevice BarcodeParamList[], int IDList_LA[], int IDList_LB[])//載入條碼機參數
{	
	if ( NULL == filename ) 
	{
		m_ErrorString = _T("Error, LoadSystemBarcodeParamFile No Filename");
		return false;
	}

	size_t  i=0;
	int     tempI=0;
	double  tempD=0.0;
	const size_t textlen = 128;
	CString str;
	CString FileName;
	CString Section = _T("");	
	CString KeyName = _T("");
	CString Default = _T("");	
	TCHAR   String[textlen]=_T("");		
	const size_t MaxDeviceCount = MAX_BARCODE_DEVICE_COUNT;		

	FileName = filename;
	for ( i=0; i<MaxDeviceCount; i++ )
	{
		TBarcodeDevice &BarcodeDevice = BarcodeParamList[i];

		switch ( i )
		{
		case 0: BarcodeDevice.nDeviceID = BARCODE_DEVICE_ID_01;	break;
		case 1: BarcodeDevice.nDeviceID = BARCODE_DEVICE_ID_02;	break;
		case 2: BarcodeDevice.nDeviceID = BARCODE_DEVICE_ID_03;	break;
		case 3: BarcodeDevice.nDeviceID = BARCODE_DEVICE_ID_04;	break;
		case 4: BarcodeDevice.nDeviceID = BARCODE_DEVICE_ID_05;	break;
		case 5: BarcodeDevice.nDeviceID = BARCODE_DEVICE_ID_06;	break;
		case 6: BarcodeDevice.nDeviceID = BARCODE_DEVICE_ID_07;	break;
		case 7: BarcodeDevice.nDeviceID = BARCODE_DEVICE_ID_08;	break;
		default:	BarcodeDevice.nDeviceID = 0; break;
		}
		if ( 0 == BarcodeDevice.nDeviceID ) { continue; }

		Section.Format(_T("%s %d"), _T("Barcode Device"), BarcodeDevice.nDeviceID);
		
		//條碼機樣式
		KeyName = _T("Device Type");
		Default.Format(_T("%d"), BarcodeDevice.eDevieType);
		if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
		{
			tempI = JetAPI::StrToInt(String);
			switch ( tempI )
			{
			case BARCODE_DEVICE_HONEYWELL_3310GHD:
			case BARCODE_DEVICE_DATALOGIC_MATRIX_210:
			case BARCODE_DEVICE_DATALOGIC_MATRIX_210N:
			case BARCODE_DEVICE_KEYENCE_SR2000:
			case BARCODE_DEVICE_KEYENCE_SR751:
			case BARCODE_DEVICE_OTHER_AZUREWAVE:
			case BARCODE_DEVICE_GENERAL_DEVICE_01:
			case BARCODE_DEVICE_GENERAL_DEVICE_02:
			case BARCODE_DEVICE_GENERAL_DEVICE_03:
			case BARCODE_DEVICE_GENERAL_DEVICE_04:
			case BARCODE_DEVICE_GENERAL_DEVICE_05:
			case BARCODE_DEVICE_GENERAL_DEVICE_06:
			case BARCODE_DEVICE_GENERAL_DEVICE_07:
			case BARCODE_DEVICE_GENERAL_DEVICE_08:
				BarcodeDevice.eDevieType = (BARCODE_DEVICE_TYPE)(tempI);
				break;
			case BARCODE_DEVICE_NULL:
			default:
				BarcodeDevice.eDevieType = BARCODE_DEVICE_NULL;
				break;
			}			
		}

		//連接埠		
		KeyName = _T("Connection Port");
		Default = BarcodeDevice.sDevicePort;
		if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
		{	BarcodeDevice.sDevicePort = String;	}
	}

	Section = _T("Barcode Device ID Lane A");
	for ( i=0; i<MaxDeviceCount; i++ )
	{			
		KeyName.Format(_T("Lane A Barcode Device %d"), i+1);
		Default.Format(_T("%d"), IDList_LA[i]);
		if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
		{	IDList_LA[i] = JetAPI::StrToInt(String);	}
	}

	Section = _T("Barcode Device ID Lane B");
	for ( i=0; i<MaxDeviceCount; i++ )
	{			
		KeyName.Format(_T("Lane B Barcode Device %d"), i+1);
		Default.Format(_T("%d"), IDList_LB[i]);
		if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
		{	IDList_LB[i] = JetAPI::StrToInt(String);	}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::SaveModelDefaultWndParam()
{		
	bool bSucc=true;
	if ( SaveModelDefaultWndParam(MDW_VERSION_1) == false )
	{	bSucc = false; }
	if ( SaveModelDefaultWndParam(MDW_VERSION_2) == false )
	{	bSucc = false; }
	return bSucc;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::SaveModelDefaultWndParam(MDW_VERSION eVersion)
{	
	int   i=0;
	CString          GroupName;
	CHIP_SIZE_MODE   ChipSizeLevel;
	const int nLevel = 2;	
	MODEL_TYPE               ModelType;
	TMODEL_DEFAULT_WND_PARAM ModelParam;

	if ( MDW_VERSION_1 == eVersion)
	{	return true;	}

	//被動元件	
	ModelType = MODEL_TYPE_CHIP;
	ChipSizeLevel = CHIP_SIZE_OTHERS;
	LoadModelDefaultWndParam(ModelType, ChipSizeLevel, GroupName, ModelParam, eVersion);
	SaveModelDefaultWndParam(ModelParam, eVersion);

	for ( i=1; i<CHIP_SIZE_OTHERS; i++ )
	{
		ChipSizeLevel = (CHIP_SIZE_MODE)(i);
		
		//被動元件-電容
		ModelType = MODEL_TYPE_CHIP_C;
		LoadModelDefaultWndParam(ModelType, ChipSizeLevel, GroupName, ModelParam, eVersion);
		SaveModelDefaultWndParam(ModelParam, eVersion);
		//被動元件-電阻
		ModelType = MODEL_TYPE_CHIP_R;
		LoadModelDefaultWndParam(ModelType, ChipSizeLevel, GroupName, ModelParam, eVersion);
		SaveModelDefaultWndParam(ModelParam, eVersion);
		//被動元件-電感
		ModelType = MODEL_TYPE_CHIP_L;
		LoadModelDefaultWndParam(ModelType, ChipSizeLevel, GroupName, ModelParam, eVersion);
		SaveModelDefaultWndParam(ModelParam, eVersion);
	}
	ChipSizeLevel = CHIP_SIZE_OTHERS;

	std::vector<MODEL_TYPE> ModelTypeList;
	CAOIModel::GetModelTypeList(ModelTypeList);	
	const size_t ModelTypeCount=ModelTypeList.size();
	for ( size_t i=0; i<ModelTypeCount; i++ )
	{
		ModelType = ModelTypeList[i];
		if ( MODEL_TYPE_NULL == ModelType ) { continue; }
		if ( CAOIModel::CheckModelTypUseChipSizeMode(ModelType) == true ) { continue; }
		LoadModelDefaultWndParam(ModelType, ChipSizeLevel, GroupName, ModelParam, eVersion);
		SaveModelDefaultWndParam(ModelParam, eVersion);
	}		
	return true;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataCollect::GetModelDefaultWndParamSection(MODEL_TYPE ModelType, CHIP_SIZE_MODE ChipSizeMode)
{	
	CString Section;	
	CString ChipSizeText;
	CString ModelTypeText;
	char    TypeText[64]="";
	CAOIModel::GetModelTypeText(ModelType, TypeText);
	ModelTypeText = TypeText;
	switch ( ModelType )
	{
	case MODEL_TYPE_CHIP:	
	case MODEL_TYPE_CHIP_LED:
	case MODEL_TYPE_MELF:		
		ChipSizeText = CAOIModel::GetModelChipSizeModeText(ChipSizeMode);		
		Section.Format(_T("%s %s"), ModelTypeText, ChipSizeText);		
		Section = ModelTypeText;
		break;		
	case MODEL_TYPE_CHIP_C:
	case MODEL_TYPE_CHIP_R:
	case MODEL_TYPE_CHIP_L:		
		ChipSizeText = CAOIModel::GetModelChipSizeModeText(ChipSizeMode);				
		switch ( ChipSizeMode )
		{
		case CHIP_SIZE_008_004:
		case CHIP_SIZE_030_015_METRIC:
		case CHIP_SIZE_010_005:
		case CHIP_SIZE_020_010:
		case CHIP_SIZE_040_020:
		case CHIP_SIZE_060_030:
		case CHIP_SIZE_080_050:
		case CHIP_SIZE_120_060:
		case CHIP_SIZE_120_100:
		case CHIP_SIZE_180_120:
		case CHIP_SIZE_200_100:
		case CHIP_SIZE_250_120:		
			Section.Format(_T("%s%s"), ModelTypeText, ChipSizeText);
			break;
		default:
		case CHIP_SIZE_NONE:
		case CHIP_SIZE_OTHERS:
			Section.Format(_T("%s %s"), ModelTypeText, ChipSizeText);		
			break;
		}
		break;
	default:
		Section = ModelTypeText;
		break;
	}	
	return Section;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::SaveModelDefaultWndParam(const TMODEL_DEFAULT_WND_PARAM &ModelParam, MDW_VERSION eVersion)
{
	if ( SaveModelDefaultWndParamFn(ModelParam, eVersion) == false )
	{
		SetSystemExceptionCode_FileWrite();
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::SaveModelDefaultWndParamFn(const TMODEL_DEFAULT_WND_PARAM &ModelParam, MDW_VERSION eVersion)	
{
	int i=0;	
	CString str;
	CString Section;
	CString KeyName;
	CString String;
	CString FileName;
	CString ChipSizeText = _T("");
	const size_t GroupNameLen = ModelParam.sGroupName.GetLength();
	
	str = GetAOIDefaultModelDirectory();	
	switch ( eVersion )
	{
	case MDW_VERSION_2:
		FileName.Format(_T("%s\\%s"), str, _T("ModelDefault_v2.ini"));
		Section = GetModelDefaultWndParamSection(ModelParam.eModelType, ModelParam.eChipSizeMode);
		break;
	default:
	case MDW_VERSION_1:
		Section = ModelParam.sGroupName;
		FileName.Format(_T("%s\\%s"), str, _T("ModelDefault.ini"));		
		break;
	}	
	if ( 0 == Section.GetLength() )
	{	return false; }
	//::_tcstod(String, NULL);		

	//Model Type
	KeyName = _T("Model Type");
	String.Format(_T("%d"), ModelParam.eModelType);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }

	//Group Name
	KeyName = _T("Group Name");
	String.Format(_T("%s"), ModelParam.sGroupName);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }

	//Chip Size Level
	KeyName = _T("Chip Size Mode");
	String.Format(_T("%d"), ModelParam.eChipSizeMode);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }

	//Use Default Model
	KeyName = _T("Use Default Model");
	String.Format(_T("%d"), ModelParam.bUseDefaultModel);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }

	//Pad Align
	KeyName = _T("Pad Align Enabled");
	String.Format(_T("%d"), ModelParam.bPadAlign);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }
	KeyName = _T("Pad Align Extend Range");
	String.Format(_T("%.2f"), ModelParam.dPadAlignExtendRange);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }

	//Pad Adjust
	KeyName = _T("Pad Adjust Enabled");
	String.Format(_T("%d"), ModelParam.bPadAdjust);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }
	KeyName = _T("Pad Adjust Extend Range");
	String.Format(_T("%.2f"), ModelParam.dPadAdjustExtendRange);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }

	//Part Align
	KeyName = _T("Part Align Enabled");
	String.Format(_T("%d"), ModelParam.bPartAlign);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }	
	KeyName = _T("Part Align Height Tolerance");
	String.Format(_T("%.2f"), ModelParam.dPartAlignHeightTolerance);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }
	KeyName = _T("Part Align Skew Limit");
	String.Format(_T("%.2f"), ModelParam.dPartAlignSkewLimit);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }
	KeyName = _T("Part Align Offset Limit");
	String.Format(_T("%.2f"), ModelParam.dPartAlignOffsetLimit);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }
	KeyName = _T("Part Align Extend Range");
	String.Format(_T("%.2f"), ModelParam.dPartAlignExtendRange);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }	
	//WND_RGN_LINK_MODE ePartAlignLinkMode;	

	KeyName = _T("Part Align Mode");
	String.Format(_T("%d"), ModelParam.nPartAlignMode);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }	

	//Lead Adjust
	KeyName = _T("Lead Adjust Enabled");
	String.Format(_T("%d"), ModelParam.bLeadAdjust);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }
	KeyName = _T("Lead Adjust Extend Range");
	String.Format(_T("%.2f"), ModelParam.dLeadAdjustExtendRange);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }

	//Body Missing
	KeyName = _T("Body Missing Enabled");
	String.Format(_T("%d"), ModelParam.bBodyMissing);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }

	KeyName = _T("Body Missing Mode");
	String.Format(_T("%d"), ModelParam.nBodyMissingMode);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }
	KeyName = _T("Body Missing Height Tolerance");
	String.Format(_T("%.2f"), ModelParam.dBodyMissingHeightTolerance);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }	

	//Body Tilt
	KeyName = _T("Body Tilt Enabled");
	String.Format(_T("%d"), ModelParam.bBodyTilt);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }
	KeyName = _T("Body Tilt Height Tolerance");
	String.Format(_T("%.2f"), ModelParam.dBodyTiltHeightTolerance);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }		
	//int          nBodyTiltNum;

	//Body Mount
	KeyName = _T("Body Mount Enabled");
	String.Format(_T("%d"), ModelParam.bBodyMount);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }
	//int          nBodyMountNum;	

	//Polarity Mount
	KeyName = _T("Polarity Enabled");
	String.Format(_T("%d"), ModelParam.bPolarity);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }
	KeyName = _T("Polarity Extend Range");
	String.Format(_T("%.2f"), ModelParam.dPolarityExtendRange);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }

	//Text Wrong
	KeyName = _T("Text Wrong Enabled");
	String.Format(_T("%d"), ModelParam.bTextWrong);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }

	//Body Damaged
	KeyName = _T("Body Damaged Enabled");
	String.Format(_T("%d"), ModelParam.bBodyDamaged);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }

	//Lead Lifted
	KeyName = _T("Lead Lifted Enabled");
	String.Format(_T("%d"), ModelParam.bLeadLifted);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }

	//Lead Bended
	KeyName = _T("Lead Bended Enabled");
	String.Format(_T("%d"), ModelParam.bLeadBended);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }	

	//Solder Open
	KeyName = _T("Solder Open Enabled");
	String.Format(_T("%d"), ModelParam.bSolderOpen);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }	
	KeyName = _T("Solder Open Use Side Wnd");
	String.Format(_T("%d"), ModelParam.bSolderOpenUseSideWnd);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }
	KeyName = _T("Solder Open Bright Ratio Ratio USL");
	String.Format(_T("%.2f"), ModelParam.dSolderOpenBrRatioUSL);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }	

	//Solder Poor
	KeyName = _T("Solder Poor Enabled");
	String.Format(_T("%d"), ModelParam.bSolderPoor);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }		
	KeyName = _T("Solder Poor Use Side Wnd");
	String.Format(_T("%d"), ModelParam.bSolderPoorUseSideWnd);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }	
	KeyName = _T("Solder Poor Bright Ratio Ratio USL");
	String.Format(_T("%.2f"), ModelParam.dSolderPoorBrRatioUSL);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }	

	//Pad Scratch
	KeyName = _T("Pad Scratch Enabled");
	String.Format(_T("%d"), ModelParam.bPadScratch);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }	

	//Bridge
	KeyName = _T("Bridge Enabled");
	String.Format(_T("%d"), ModelParam.bBridge);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }
	KeyName = _T("Bridge Use 2D");
	String.Format(_T("%d"), ModelParam.bBridgeUse2D);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }
	KeyName = _T("Bridge Use 3D");
	String.Format(_T("%d"), ModelParam.bBridgeUse3D);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }
	KeyName = _T("Bridge Extend Range");
	String.Format(_T("%.2f"), ModelParam.dBridgeExtendRange);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }	
	KeyName = _T("Bridge Fixed Threshold Low 2D");
	String.Format(_T("%d"), ModelParam.nBridgeFixThresholdL2D);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }	
	KeyName = _T("Bridge Fixed Threshold Low 3D");
	String.Format(_T("%d"), ModelParam.nBridgeFixThresholdL3D);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }	

	//Foreign Body
	KeyName = _T("Foreign Body Enabled");
	String.Format(_T("%d"), ModelParam.bForeignBody);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }

	//Others
	KeyName = _T("Default Model All Defect Enabled");
	String.Format(_T("%d"), ModelParam.bDefaultModelAllDefect);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::LoadModelDefaultWndParam(MODEL_TYPE ModelType, CHIP_SIZE_MODE ChipSizeMode, LPCTSTR GroupName, TMODEL_DEFAULT_WND_PARAM &ModelParam, MDW_VERSION eVersion)
{
	if ( LoadModelDefaultWndParamFn(ModelType, ChipSizeMode, GroupName, ModelParam, eVersion) == false )
	{
		SetSystemExceptionCode_FileRead();
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::LoadModelDefaultWndParamFn(MODEL_TYPE ModelType, CHIP_SIZE_MODE ChipSizeMode, LPCTSTR GroupName, TMODEL_DEFAULT_WND_PARAM &ModelParam, MDW_VERSION eVersion)
{	
	const size_t textlen = 128;	
	CString str;
	CString FileName;
	CString Section = _T("");	
	CString KeyName = _T("");
	CString Default = _T("");
	CString ChipSizeText = _T("");
	TCHAR   String[textlen]=_T("");
	const size_t GroupNameLen = _tcslen(GroupName);	
	
	str = GetAOIDefaultModelDirectory();		
	switch ( eVersion )
	{
	case MDW_VERSION_2:
		FileName.Format(_T("%s\\%s"), str, _T("ModelDefault_v2.ini"));
		Section = GetModelDefaultWndParamSection(ModelType, ChipSizeMode);		
		break;
	default:
	case MDW_VERSION_1:
		Section = GroupName;
		FileName.Format(_T("%s\\%s"), str, _T("ModelDefault.ini"));
		break;
	}
	if ( 0 == Section.GetLength() )
	{	return false; }

	//::_tcstod(String, NULL);	
	//並非所有參數都儲存在檔案, 所以還是要先取得預設參數
	AOIDataDefine.GetModelDefaultWndParam(ModelType, ChipSizeMode, ModelParam);	
	
	ModelParam.sGroupName = GroupName;
	ModelParam.eModelType = ModelType;
	ModelParam.eChipSizeMode = ChipSizeMode;

	//Model Type
	KeyName = _T("Model Type");
	Default.Format(_T("%d"), ModelParam.eModelType);
	LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString);

	//Group Name
	KeyName = _T("Group Name");
	Default.Format(_T("%s"), ModelParam.sGroupName);
	LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString);

	//Chip Size Level
	KeyName = _T("Chip Size Mode");
	Default.Format(_T("%d"), ModelParam.eChipSizeMode);
	LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString);

	KeyName = _T("Use Default Model");
	Default.Format(_T("%d"), ModelParam.bUseDefaultModel);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	ModelParam.bUseDefaultModel = (bool)(::_ttoi(String));	}

	//Pad Align
	KeyName = _T("Pad Align Enabled");
	Default.Format(_T("%d"), ModelParam.bPadAlign);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	ModelParam.bPadAlign = (bool)(::_ttoi(String));	}
	KeyName = _T("Pad Align Extend Range");
	Default.Format(_T("%.2f"), ModelParam.dPadAlignExtendRange);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	ModelParam.dPadAlignExtendRange = ::_tcstod(String, NULL);	}	

	//Pad Adjust
	KeyName = _T("Pad Adjust Enabled");
	Default.Format(_T("%d"), ModelParam.bPadAdjust);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	ModelParam.bPadAdjust = (bool)(::_ttoi(String));	}
	KeyName = _T("Pad Adjust Extend Range");
	Default.Format(_T("%.2f"), ModelParam.dPadAdjustExtendRange);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	ModelParam.dPadAdjustExtendRange = ::_tcstod(String, NULL);	}	

	//Part Align
	KeyName = _T("Part Align Enabled");
	Default.Format(_T("%d"), ModelParam.bPartAlign);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	ModelParam.bPartAlign = (bool)(::_ttoi(String));	}
	KeyName = _T("Part Align Height Tolerance");
	Default.Format(_T("%.2f"), ModelParam.dPartAlignHeightTolerance);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	ModelParam.dPartAlignHeightTolerance = ::_tcstod(String, NULL);	}
	KeyName = _T("Part Align Skew Limit");
	Default.Format(_T("%.2f"), ModelParam.dPartAlignSkewLimit);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	ModelParam.dPartAlignSkewLimit = ::_tcstod(String, NULL);	}	
	KeyName = _T("Part Align Offset Limit");
	Default.Format(_T("%.2f"), ModelParam.dPartAlignOffsetLimit);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	ModelParam.dPartAlignOffsetLimit = ::_tcstod(String, NULL);	}	
	KeyName = _T("Part Align Extend Range");
	Default.Format(_T("%.2f"), ModelParam.dPartAlignExtendRange);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	ModelParam.dPartAlignExtendRange = ::_tcstod(String, NULL);	}

	KeyName = _T("Part Align Mode");
	Default.Format(_T("%d"), ModelParam.nPartAlignMode);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	ModelParam.nPartAlignMode = ::_ttoi(String);	}
	//WND_RGN_LINK_MODE ePartAlignLinkMode;	

	//Lead Adjust
	KeyName = _T("Lead Adjust Enabled");
	Default.Format(_T("%d"), ModelParam.bLeadAdjust);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	ModelParam.bLeadAdjust = (bool)(::_ttoi(String));	}
	KeyName = _T("Lead Adjust Extend Range");
	Default.Format(_T("%.2f"), ModelParam.dLeadAdjustExtendRange);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	ModelParam.dLeadAdjustExtendRange = ::_tcstod(String, NULL);	}	

	//Body Missing
	KeyName = _T("Body Missing Enabled");
	Default.Format(_T("%d"), ModelParam.bBodyMissing);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	ModelParam.bBodyMissing = (bool)(::_ttoi(String));	}

	KeyName = _T("Body Missing Mode");
	Default.Format(_T("%d"), ModelParam.nBodyMissingMode);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	ModelParam.nBodyMissingMode = (::_ttoi(String));	}	
	KeyName = _T("Body Missing Height Tolerance");
	Default.Format(_T("%.2f"), ModelParam.dBodyMissingHeightTolerance);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	ModelParam.dBodyMissingHeightTolerance = (::_ttof(String));	}	

	//Body Tilt
	KeyName = _T("Body Tilt Enabled");
	Default.Format(_T("%d"), ModelParam.bBodyTilt);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	ModelParam.bBodyTilt = (bool)(::_ttoi(String));	}

	KeyName = _T("Body Tilt Height Tolerance");
	Default.Format(_T("%.2f"), ModelParam.dBodyTiltHeightTolerance);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	ModelParam.dBodyTiltHeightTolerance = (::_ttof(String));	}
	//int          nBodyTiltNum;

	//Body Mount
	KeyName = _T("Body Mount Enabled");
	Default.Format(_T("%d"), ModelParam.bBodyMount);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	ModelParam.bBodyMount = (bool)(::_ttoi(String));	}
	//int          nBodyMountNum;	

	//Polarity Mount
	KeyName = _T("Polarity Enabled");
	Default.Format(_T("%d"), ModelParam.bPolarity);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	ModelParam.bPolarity = (bool)(::_ttoi(String));	}
	KeyName = _T("Polarity Extend Range");
	Default.Format(_T("%.2f"), ModelParam.dPolarityExtendRange);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	ModelParam.dPolarityExtendRange = ::_tcstod(String, NULL);	}	

	//Text Wrong
	KeyName = _T("Text Wrong Enabled");
	Default.Format(_T("%d"), ModelParam.bTextWrong);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	ModelParam.bTextWrong = (bool)(::_ttoi(String));	}

	//Body Damaged
	KeyName = _T("Body Damaged Enabled");
	Default.Format(_T("%d"), ModelParam.bBodyDamaged);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	ModelParam.bBodyDamaged = (bool)(::_ttoi(String));	}

	//Lead Lifted
	KeyName = _T("Lead Lifted Enabled");
	Default.Format(_T("%d"), ModelParam.bLeadLifted);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	ModelParam.bLeadLifted = (bool)(::_ttoi(String));	}

	//Lead Bended
	KeyName = _T("Lead Bended Enabled");
	Default.Format(_T("%d"), ModelParam.bLeadBended);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	ModelParam.bLeadBended = (bool)(::_ttoi(String));	}

	//Solder Open
	KeyName = _T("Solder Open Enabled");
	Default.Format(_T("%d"), ModelParam.bSolderOpen);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	ModelParam.bSolderOpen = (bool)(::_ttoi(String));	}	
	KeyName = _T("Solder Open Use Side Wnd");
	Default.Format(_T("%d"), ModelParam.bSolderOpenUseSideWnd);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	ModelParam.bSolderOpenUseSideWnd = (bool)(::_ttoi(String));	}
	KeyName = _T("Solder Open Bright Ratio Ratio USL");
	Default.Format(_T("%.2f"), ModelParam.dSolderOpenBrRatioUSL);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	ModelParam.dSolderOpenBrRatioUSL = (::_ttof(String));	}	

	//Solder Poor
	KeyName = _T("Solder Poor Enabled");
	Default.Format(_T("%d"), ModelParam.bSolderPoor);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	ModelParam.bSolderPoor = (bool)(::_ttoi(String));	}
	KeyName = _T("Solder Poor Use Side Wnd");
	Default.Format(_T("%d"), ModelParam.bSolderPoorUseSideWnd);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	ModelParam.bSolderPoorUseSideWnd = (bool)(::_ttoi(String));	}	
	KeyName = _T("Solder Poor Bright Ratio Ratio USL");
	Default.Format(_T("%.2f"), ModelParam.dSolderPoorBrRatioUSL);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	ModelParam.dSolderPoorBrRatioUSL = (::_ttof(String));	}	

	//Pad Scratch
	KeyName = _T("Pad Scratch Enabled");
	Default.Format(_T("%d"), ModelParam.bPadScratch);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	ModelParam.bPadScratch = (bool)(::_ttoi(String));	}

	//Bridge
	KeyName = _T("Bridge Enabled");
	Default.Format(_T("%d"), ModelParam.bBridge);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	ModelParam.bBridge = (bool)(::_ttoi(String));	}
	KeyName = _T("Bridge Use 2D");
	Default.Format(_T("%d"), ModelParam.bBridgeUse2D);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	ModelParam.bBridgeUse2D = (bool)(::_ttoi(String));	}
	KeyName = _T("Bridge Use 3D");
	Default.Format(_T("%d"), ModelParam.bBridgeUse3D);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	ModelParam.bBridgeUse3D = (bool)(::_ttoi(String));	}
	KeyName = _T("Bridge Extend Range");
	Default.Format(_T("%.2f"), ModelParam.dBridgeExtendRange);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	ModelParam.dBridgeExtendRange = ::_ttof(String);	}
	KeyName = _T("Bridge Fixed Threshold Low 2D");
	Default.Format(_T("%d"), ModelParam.nBridgeFixThresholdL2D);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	ModelParam.nBridgeFixThresholdL2D = ::_ttoi(String);	}
	KeyName = _T("Bridge Fixed Threshold Low 3D");
	Default.Format(_T("%d"), ModelParam.nBridgeFixThresholdL3D);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	ModelParam.nBridgeFixThresholdL3D = ::_ttoi(String);	}

	//Foreign Body
	KeyName = _T("Foreign Body Enabled");
	Default.Format(_T("%d"), ModelParam.bForeignBody);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	ModelParam.bForeignBody = (bool)(::_ttoi(String));	}

	//Others
	KeyName = _T("Default Model All Defect Enabled");
	Default.Format(_T("%d"), ModelParam.bDefaultModelAllDefect);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	ModelParam.bDefaultModelAllDefect = (bool)(::_ttoi(String));	}
	return true;
}
//-------------------------------------------------------------------------------------//
MODEL_TYPE CAOIDataCollect::GetModelDefaultTransistorType() const//取得模組預設的SOT樣式
{
	return GetSystemParameter().m_ModelDefaultTransistorType;	
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::BuildRemoteParamList()//建立遠端調機參數列表
{	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::SaveSystemRemoteParameter()//儲存遠端調機參數列表
{	
	CString FileName;	
	CString TempName;	
	CString ShortName = _T("RemoteTuning.INI");
	CString SysFolder = GetAOIDirectory();
	CString TempFolder = GetAOITempDirectory();	
	TempName.Format(_T("%s\\%s"), TempFolder, ShortName);	
	FileName.Format(_T("%s\\%s"), SysFolder, ShortName);
	if ( SaveSystemRemoteParamFile(TempName, m_SystemRemoteParamList)==false )
	{
		DeleteFile(TempName);
		return false; 
	}	
	DeleteFile(FileName);
	::CopyFile(TempName, FileName, FALSE);
	DeleteFile(TempName);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::LoadSystemRemoteParameter()//載入遠端調機參數列表
{
	CString FileName;	
	CString TempName;	
	CString ShortName = _T("RemoteTuning.INI");
	CString SysFolder = GetAOIDirectory();
	CString TempFolder = GetAOITempDirectory();	
	TempName.Format(_T("%s\\%s"), TempFolder, ShortName);	
	FileName.Format(_T("%s\\%s"), SysFolder, ShortName);
	if ( LoadSystemRemoteParamFile(FileName, m_SystemRemoteParamList)==false )
	{	return false; }		
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIDataCollect::GetSystemRemoteParamList(std::vector<TRemoteParam> &ParamList) const//取得遠端調機參數列表
{
	ParamList = m_SystemRemoteParamList;
}
//-------------------------------------------------------------------------------------//
void CAOIDataCollect::SetSystemRemoteParamList(const std::vector<TRemoteParam> &ParamList)//設定遠端調機參數列表
{
	m_SystemRemoteParamList = ParamList;
}
//-------------------------------------------------------------------------------------//
size_t CAOIDataCollect::GetRemoteParamActivedIndex(const std::vector<TRemoteParam> &ParamList)//由唯一碼取得引數
{
	size_t i = 0;
	size_t index = -1;	
	const size_t ParamCount = ParamList.size();
	for ( i=0; i<ParamCount; i++ )
	{
		if ( true == ParamList[i].bDeleted ) { continue; }
		if ( false == ParamList[i].bActived ) { continue; }
		index = i;
		break;
	}
	return index;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::SetRemoteParamActived(size_t index, std::vector<TRemoteParam> &ParamList)//設定哪個參數為主要狀態
{
	size_t i = 0;	
	const size_t ParamCount = ParamList.size();
	for ( i=0; i<ParamCount; i++ )
	{	ParamList[i].bActived = false; }

	if ( index < ParamCount )
	{	ParamList[index].bActived = true;	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIDataCollect::ArrangeRemoteParamList(const std::vector<TRemoteParam> &SrcList, std::vector<TRemoteParam> &DstList)
{
	size_t       i=0;
	TRemoteParam RemoteParam;
	const size_t SrcCount=SrcList.size();
	DstList.clear();
	for ( i=0; i<SrcCount; i++ )
	{
		if ( true == SrcList[i].bDeleted ) { continue; }
		RemoteParam = SrcList[i];
		RemoteParam.nIndex = DstList.size();
		DstList.push_back(RemoteParam);
	}
	return;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::LoadSystemRemoteParamFile(LPCTSTR filename, std::vector<TRemoteParam> &ParamList)//儲存遠端調機參數
{
	if ( LoadSystemRemoteParamFileFn(filename, ParamList) == false )
	{
		SetSystemExceptionCode_FileRead();
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::LoadSystemRemoteParamFileFn(LPCTSTR filename, std::vector<TRemoteParam> &ParamList)//儲存遠端調機參數
{
	if ( NULL == filename )
	{
		m_ErrorString = _T("Error, LoadSystemRemoteParamFile No Filename");
		return false;	
	}

	size_t  i=0;
	int     tempI=0;
	double  tempD=0.0;
	TRemoteParam RemoteParam;
	size_t  MaxRemoteCount = 0;
	const size_t textlen = 128;
	CString str;
	CString FileName;
	CString Section = _T("");	
	CString KeyName = _T("");
	CString Default = _T("");	
	TCHAR   String[textlen]=_T("");	
	
	FileName = filename;
	Section = _T("General");
	KeyName = _T("Remote Count");
	Default.Format(_T("%u"), MaxRemoteCount);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	MaxRemoteCount = ::_ttoi(String);	}

	ParamList.clear();
	for ( i=0; i<MaxRemoteCount; i++ )
	{
		RemoteParam = TRemoteParam();
		Section.Format(_T("%s %u"), _T("Remote Param"), i+1);

		//遠端電腦名稱
		KeyName = _T("Remote PC Name");
		Default.Format(_T("%s"), RemoteParam.sRemotePCName);
		if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
		{	RemoteParam.sRemotePCName = String;	}

		//專案資料夾
		KeyName = _T("Project Folder");
		Default.Format(_T("%s"), RemoteParam.sProjectFolder);
		if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
		{	RemoteParam.sProjectFolder = String;	}

		//調機資料夾		
		KeyName = _T("Tuning Folder");
		Default.Format(_T("%s"), RemoteParam.sTuningFolder);
		if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
		{	RemoteParam.sTuningFolder = String;	}
		
		//啟用中
		KeyName = _T("Active");
		Default.Format(_T("%d"), RemoteParam.bActived);
		if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
		{	RemoteParam.bActived = (bool)(::_ttoi(String));	}

		RemoteParam.nIndex = i;
		ParamList.push_back(RemoteParam);
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::SaveSystemRemoteParamFile(LPCTSTR filename, const std::vector<TRemoteParam> &ParamList)//載入遠端調機參數
{
	if ( SaveSystemRemoteParamFileFn(filename, ParamList) == false )
	{
		SetSystemExceptionCode_FileWrite();
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::SaveSystemRemoteParamFileFn(LPCTSTR filename, const std::vector<TRemoteParam> &ParamList)//載入遠端調機參數
{
	if ( NULL == filename )
	{
		m_ErrorString = _T("Error, SaveSystemRemoteParamFile No Filename");
		return false;	
	}

	size_t  i=0;	
	CString str;
	CString Section;
	CString KeyName;
	CString String;
	CString FileName;
	TRemoteParam RemoteParam;
	const size_t MaxRemoteCount = ParamList.size();

	FileName = filename;
	Section = _T("General");
	KeyName = _T("Remote Count");
	String.Format(_T("%u"), MaxRemoteCount);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
	{	return false; }

	for ( i=0; i<MaxRemoteCount; i++ )
	{
		RemoteParam = ParamList[i];
		Section.Format(_T("%s %u"), _T("Remote Param"), i+1);
		
		//遠端電腦名稱
		KeyName = _T("Remote PC Name");
		String = RemoteParam.sRemotePCName;
		if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
		{	return false; }

		//專案資料夾
		KeyName = _T("Project Folder");		
		String = RemoteParam.sProjectFolder;
		if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
		{	return false; }

		//調機資料夾		
		KeyName = _T("Tuning Folder");
		String = RemoteParam.sTuningFolder;
		if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
		{	return false; }

		//啟用中
		KeyName = _T("Active");
		String.Format(_T("%d"), RemoteParam.bActived);
		if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false )
		{	return false; }
	}
	return true;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataCollect::GetPhaseFactorFovZFilename(LIGHT_3D_CAST_ID CastID, int Index, bool bTemp)//取得相位高度FOV相位檔名
{
	CString Filename;
	CString Root;	
	CString Folder;		
	if ( false == bTemp )
	{	Root = GetAOIDirectory();	}
	else	
	{	Root = GetAOITempDirectory();	}	
	Folder.Format(_T("%s\\%s"), Root, _T("Calibration"));	
	Filename.Format(_T("%s\\HeightFactorFov_ID%02d_%02d.BIN"), Folder, CastID, Index);	
	return Filename;
}
//-------------------------------------------------------------------------------------//
double CAOIDataCollect::Calc2ParamVecotr(const std::vector<double> &List1, const std::vector<double> &List2)
{
	double    tempD=0;
	double    tempSum=0;
	const int List1Cnt=(int)(List1.size());
	const int List2Cnt=(int)(List2.size());
	const int MinCnt=MIN(List1Cnt, List2Cnt);
	for ( int i=0; i<MinCnt; i++ )
	{
		tempD = List1[i]*List2[i];
		tempSum += tempD;
	}
	return tempSum;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::GetHeightFactorXYParam(int ParamCount, const TPhaseFactorGrid &Grid, std::vector<double> &ParamList)//取得TPhaseFactorGrid計算相位參數值
{
	const int ParamSize = (int)(ParamList.size());
	if ( ParamSize < ParamCount ) { return false; }

	double C = 1.0;
	double X = Grid.m_ImgX;
	double Y = Grid.m_ImgY;	
	switch ( ParamCount )
	{
	case 1:
		ParamList[0] = C;
		break;
	case 3:
		ParamList[0] = C;
		ParamList[1] = X;//u
		ParamList[2] = Y;//v
		break;
	case 6:
		ParamList[0]  = C;
		ParamList[1]  = X;//u
		ParamList[2]  = Y;//v
		ParamList[3]  = X*Y;//uv				
		ParamList[4]  = X*X;//uu
		ParamList[5]  = Y*Y;//vv
		break;
	case 7://3rd
		ParamList[0]  = C;//
		ParamList[1]  = X;//u-x
		ParamList[2]  = Y;//v-y
		ParamList[3]  = X*X;//uu
		ParamList[4]  = Y*Y;//vv
		ParamList[5]  = X*X*X;//uuu
		ParamList[6]  = Y*Y*Y;//vvv
		break;
	case 10://3rd
		ParamList[0]  = C;//
		ParamList[1]  = X;//u-x
		ParamList[2]  = Y;//v-y
		ParamList[3]  = X*Y;//uv
		ParamList[4]  = X*X;//uu
		ParamList[5]  = Y*Y;//vv

		ParamList[6]  = X*X*Y;//uuv				
		ParamList[7]  = X*Y*Y;//uvv
		ParamList[8]  = X*X*X;//uuu
		ParamList[9]  = Y*Y*Y;//vvv
		break;	
	case 9://4th
		ParamList[0]  = C;//
		ParamList[1]  = X;//u-x
		ParamList[2]  = Y;//v-y
		ParamList[3]  = X*X;//uu
		ParamList[4]  = Y*Y;//vv
		ParamList[5]  = X*X*X;//uuu
		ParamList[6]  = Y*Y*Y;//vvv
		ParamList[7]  = X*X*X*X;//uuuu
		ParamList[8]  = Y*Y*Y*Y;//vvvu
		break;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::GetHeightFactorPhaseParam(int Mode, int ParamCount, const TPhaseFactorGrid &Grid, std::vector<double> &ParamList)//取得TPhaseFactorGrid計算高度參數值
{
	const int ParamSize = (int)(ParamList.size());
	if ( ParamSize < ParamCount ) { return false; }

	double C = 1.0;
	double Phase = Grid.m_Phase;
	switch ( Mode )
	{
	case HEIGHT_FACTOR_PHASE_BASE:	Phase = Grid.m_PhaseBaseCalc;	break;
	case HEIGHT_FACTOR_PHASE_TARGET:	Phase = Grid.m_PhaseTargetCalc;	break;
	case HEIGHT_FACTOR_PHASE_OFFSET:	Phase = Grid.m_PhaseOffset;	break;
	default:
		Phase = Grid.m_Phase;
		break;
	}	

	switch ( ParamCount )
	{
	case 1:
		ParamList[0]  = C;//
		break;
	case 2://a0~a1, 1階函式
		ParamList[0]  = C;//
		ParamList[1]  = Phase;;//p-hase			
		break;
	case 3://a0~a2, 2階函式
		ParamList[0]  = C;//			
		ParamList[1]  = Phase;;//p-hase
		ParamList[2]  = Phase*Phase;//pp
		break;
	case 4://a0~a3, 3階函式
		ParamList[0]  = C;//			
		ParamList[1]  = Phase;;//p-hase
		ParamList[2]  = Phase*Phase;//pp
		ParamList[3]  = Phase*Phase*Phase;//ppp
		break;
	case 5://a0~a4, 4階函式
		ParamList[0]  = C;//			
		ParamList[1]  = Phase;;//p-hase
		ParamList[2]  = Phase*Phase;//pp
		ParamList[3]  = Phase*Phase*Phase;//ppp
		ParamList[4]  = Phase*Phase*Phase*Phase;//pppp
		break;		
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::CalcPhaseFactorMappingFunc(const std::vector<TPhaseFactorGrid> &List, std::vector<double> &KList)//計算相位高度比例參數-由相位轉成高度
{
	int   i=0,j=0,k=0,idx=0;
	double ImgX=0, ImgY=0;
	double AveErr=0.0, MaxErr=0.0;
	double Phase=0, Height=0, Factor=1.0;
	//const int DotCoountH=3;//Hor
	//const int DotCoountV=3;//Ver
	//const int DotHeightCount=2;//高度校正值
	//const int MaxHeightParamCount=5;//a0~a4, 2階-建議使用		
	const int MaxHeightParamCount=10;//a0~a9, 2階
	//const int MaxHeightParamCount=11;//a0~a10, 3階-建議使用
	//const int MaxHeightParamCount=13;//a0~a12, 3階
	//const int MaxHeightParamCount=20;//a0~a19, 3階
	const int TotalMaxDotSize = 325;//TotalMaxDotCounts
	const int TotalMaxDotCounts = (int)(List.size());	
	//const int TotalMaxDotCounts = DotCoountH*DotCoountV*DotHeightCount;
	const int TotalMatrixElements = MaxHeightParamCount*MaxHeightParamCount;
	int NRows = MaxHeightParamCount;
	int NCols = MaxHeightParamCount;

	double tempD=0;	
	const double C = 1.0;
	std::vector<double> MatrixElem1(TotalMatrixElements);
	std::vector<double> MatrixElem2(MaxHeightParamCount);	
	std::fill(MatrixElem1.begin(), MatrixElem1.end(), 0);
	std::fill(MatrixElem2.begin(), MatrixElem2.end(), 0);	

	//double T[TotalMaxDotSize][MaxHeightParamCount]={0};	
	//double TT[TotalMaxDotSize][MaxHeightParamCount][MaxHeightParamCount]={0};	
	
	std::vector<double> t(MaxHeightParamCount);
	std::vector<double> Step(TotalMaxDotCounts);
	std::vector<double> Error(TotalMaxDotCounts);
	std::vector<double> pResult(MaxHeightParamCount);
	std::fill(t.begin(), t.end(), 0);
	std::fill(Step.begin(), Step.end(), 0);
	std::fill(Error.begin(), Error.end(), 0);
	std::fill(pResult.begin(), pResult.end(), 0);	
	
	//::memset(MatrixElem1, 0x00, sizeof(double)*TotalMatrixElements);
	//::memset(MatrixElem2, 0x00, sizeof(double)*MaxHeightParamCount);	

	TPhaseFactorGrid PhaseFactorGrid;
	for ( i=0; i<TotalMaxDotCounts; i++ )
	{
		PhaseFactorGrid = List[i];

		ImgX = PhaseFactorGrid.m_ImgX;
		ImgY = PhaseFactorGrid.m_ImgY;
		Phase= PhaseFactorGrid.m_Phase;
		Height=PhaseFactorGrid.m_Height;

		switch ( MaxHeightParamCount )
		{		
		case 5:
			t[0]  = C;//
			t[1]  = ImgX;//u-x
			t[2]  = ImgY;//v-y
			t[3]  = Phase;;//p-hase			
			t[4]  = Phase*Phase;//pp						
		
			t[0]  = C;//c				
			t[1]  = Phase;//p-phase			
			t[2]  = ImgX*Phase;//up
			t[3]  = ImgY*Phase;//vp				
			t[4]  = Phase*Phase;//pp				
			break;		
		case 10:
			t[0]  = C;//
			t[1]  = ImgX;//u-x
			t[2]  = ImgY;//v-y
			t[3]  = Phase;;//p-hase
			t[4]  = ImgX*ImgY;//uv
			t[5]  = ImgX*Phase;//up
			t[6]  = ImgY*Phase;//vp
			t[7]  = ImgX*ImgX;//uu
			t[8]  = ImgY*ImgY;//vv
			t[9]  = Phase*Phase;//pp
			break;
		case 11:
			t[0]  = C;//				
			t[1]  = Phase;;//p-hase
				
			t[2]  = ImgX*Phase;//up
			t[3]  = ImgY*Phase;//vp				
			t[4]  = Phase*Phase;//pp

			t[5] = ImgX*ImgY*Phase;//uvp
			t[6] = ImgX*Phase*ImgX;//uup
			t[7] = ImgY*Phase*ImgY;//vvp
			t[8] = ImgX*Phase*Phase;//upp
			t[9] = ImgY*Phase*Phase;//vpp				
			t[10] = Phase*Phase*Phase;//ppp
			break;
		case 13:
			t[0]  = C;//
			t[1]  = ImgX;//u-x
			t[2]  = ImgY;//v-y
			t[3]  = Phase;;//p-hase
			t[4]  = ImgX*ImgY;//uv
			t[5]  = ImgX*Phase;//up
			t[6]  = ImgY*Phase;//vp
			t[7]  = ImgX*ImgX;//uu
			t[8]  = ImgY*ImgY;//vv
			t[9]  = Phase*Phase;//pp
			t[10] = ImgX*ImgX*ImgX;//uuu
			t[11] = ImgY*ImgY*ImgY;//vvv
			t[12] = Phase*Phase*Phase;//ppp
			break;
		case 20:
			t[0]  = C;//
			t[1]  = ImgX;//u-x
			t[2]  = ImgY;//v-y
			t[3]  = Phase;;//p-hase

			t[4]  = ImgX*ImgY;//uv
			t[5]  = ImgX*Phase;//up
			t[6]  = ImgY*Phase;//vp
			t[7]  = ImgX*ImgX;//uu
			t[8]  = ImgY*ImgY;//vv
			t[9]  = Phase*Phase;//pp

			t[10] = ImgX*ImgY*ImgX;//uuv
			t[11] = ImgX*ImgY*ImgY;//uvv
			t[12] = ImgX*ImgY*Phase;//uvp
			t[13] = ImgX*Phase*ImgX;//uup
			t[14] = ImgY*Phase*ImgY;//vvp
			t[15] = ImgX*Phase*Phase;//upp
			t[16] = ImgY*Phase*Phase;//vpp
			t[17] = ImgX*ImgX*ImgX;//uuu
			t[18] = ImgY*ImgY*ImgY;//vvv
			t[19] = Phase*Phase*Phase;//ppp
			break;
		}
		

		for ( j=0; j<MaxHeightParamCount; j++ )
		{
			//T[i][j] = t[j];
			tempD = Height*t[j];
			MatrixElem2[j] += tempD;
			//for ( k=0; k<MaxHeightParamCount; k++ )
			//{	TT[i][j][k] = t[j]*t[k];	}
		}		

		idx = 0;
		for ( j=0; j<MaxHeightParamCount; j++ )
		{
			for ( k=0; k<MaxHeightParamCount; k++ )
			{
				tempD = t[j]*t[k];
				MatrixElem1[idx] += tempD;
				idx ++;
			}			
		}
		idx = idx;
	}	
	
	try
	{
		//[mat1][A]=[mat2]
		//[mat1]^-1[mat1][A]=[mat1]^-1[mat2]
		//[I][A]=[mat1]^-1[mat2]
		//[A]=[mat3][mat2]
		//mat1.SetMatrixSize(NRows, NCols, MatrixElem1);
		//mat3=mat1.Inversion();
		//mat2.SetMatrixSize(NRows, 1, MatrixElem2);
		//mat4 = mat1*mat3;//測試反矩陣可用
		//mat5 = mat3*mat2;//求解
		//cv::Mat mat1(cv::Size(NRows,NCols), CV_64FC1, MatrixElem1);
		//cv::Mat mat2(cv::Size(NRows,1), CV_64FC1, MatrixElem2);	
		cv::Mat mat1(NRows, NCols, CV_64FC1, MatrixElem1.data());
		cv::Mat mat2(NRows, 1, CV_64FC1, MatrixElem2.data());	
		cv::Mat mat3=mat1.inv();	
		//測試反矩陣可用
		cv::Mat mat4=mat1*mat3;	
		//求解
		cv::Mat mat5 = mat3*mat2;
		//將資料放進去結果陣列中			
		for ( i=0; i<MaxHeightParamCount; i++ )
		{	
			pResult[i] = mat5.ptr<double>(0)[i];//.GetElement(0, i);	}
		}
		KList = pResult;

		ImageAPI.WriteMatData(_T("R:\\Matrix1.csv"), mat1);
		ImageAPI.WriteMatData(_T("R:\\Matrix2.csv"), mat2);
		ImageAPI.WriteMatData(_T("R:\\Matrix3.csv"), mat3);
		ImageAPI.WriteMatData(_T("R:\\Matrix4.csv"), mat4);
		ImageAPI.WriteMatData(_T("R:\\Matrix5.csv"), mat5);

		//驗證結果		
		for ( i=0; i<TotalMaxDotCounts; i++ )
		{
			PhaseFactorGrid = List[i];

			Factor = 1.0;
			ImgX = PhaseFactorGrid.m_ImgX;
			ImgY = PhaseFactorGrid.m_ImgY;
			Phase= PhaseFactorGrid.m_Phase;
			Height=PhaseFactorGrid.m_Height;			

			//Phase = 0;
			//Height = 0;
			switch ( MaxHeightParamCount )
			{			
			case 5:
				t[0]  = C;//
				t[1]  = ImgX;//u-x
				t[2]  = ImgY;//v-y
				t[3]  = Phase;;//p-hase
				t[4]  = Phase*Phase;//pp
				
				t[0]  = C;//c				
				t[1]  = Phase;//p-phase				
				t[2]  = ImgX*Phase;//up
				t[3]  = ImgY*Phase;//vp				
				t[4]  = Phase*Phase;//pp				
				break;			
			case 10:
				t[0]  = C;//c
				t[1]  = ImgX;//u-x
				t[2]  = ImgY;//v-y
				t[3]  = Phase;;//p-phase
				t[4]  = ImgX*ImgY;//uv
				t[5]  = ImgX*Phase;//up
				t[6]  = ImgY*Phase;//vp
				t[7]  = ImgX*ImgX;//uu
				t[8]  = ImgY*ImgY;//vv
				t[9]  = Phase*Phase;//pp
				break;
			case 13:
				t[0]  = C;//c
				t[1]  = ImgX;//u-x
				t[2]  = ImgY;//v-y
				t[3]  = Phase;;//p-phase
				t[4]  = ImgX*ImgY;//uv
				t[5]  = ImgX*Phase;//up
				t[6]  = ImgY*Phase;//vp
				t[7]  = ImgX*ImgX;//uu
				t[8]  = ImgY*ImgY;//vv
				t[9]  = Phase*Phase;//pp
				t[10] = ImgX*ImgX*ImgX;//uuu
				t[11] = ImgY*ImgY*ImgY;//vvv
				t[12] = Phase*Phase*Phase;//ppp
				break;
			case 11:
				t[0]  = C;//				
				t[1]  = Phase;;//p-hase
				
				t[2]  = ImgX*Phase;//up
				t[3]  = ImgY*Phase;//vp				
				t[4]  = Phase*Phase;//pp

				t[5] = ImgX*ImgY*Phase;//uvp
				t[6] = ImgX*Phase*ImgX;//uup
				t[7] = ImgY*Phase*ImgY;//vvp
				t[8] = ImgX*Phase*Phase;//upp
				t[9] = ImgY*Phase*Phase;//vpp				
				t[10] = Phase*Phase*Phase;//ppp
				break;
			case 20:
				t[0]  = C;//
				t[1]  = ImgX;//u-x
				t[2]  = ImgY;//v-y
				t[3]  = Phase;;//p-hase

				t[4]  = ImgX*ImgY;//uv
				t[5]  = ImgX*Phase;//up
				t[6]  = ImgY*Phase;//vp
				t[7]  = ImgX*ImgX;//uu
				t[8]  = ImgY*ImgY;//vv
				t[9]  = Phase*Phase;//pp

				t[10] = ImgX*ImgY*ImgX;//uuv
				t[11] = ImgX*ImgY*ImgY;//uvv
				t[12] = ImgX*ImgY*Phase;//uvp
				t[13] = ImgX*Phase*ImgX;//uup
				t[14] = ImgY*Phase*ImgY;//vvp
				t[15] = ImgX*Phase*Phase;//upp
				t[16] = ImgY*Phase*Phase;//vpp
				t[17] = ImgX*ImgX*ImgX;//uuu
				t[18] = ImgY*ImgY*ImgY;//vvv
				t[19] = Phase*Phase*Phase;//ppp
				break;
			}

			Step[i] = 0;
			Error[i] = 0;
			tempD = 0;
			for ( j=0; j<MaxHeightParamCount; j++ )
			{
				tempD = pResult[j]*t[j];
				Step[i] += tempD;
			}
			Step[i] *= Factor;
			Error[i] = fabs(Step[i]-Height);
			AveErr += Error[i];
			if ( MaxErr < Error[i] ) { MaxErr = Error[i]; }
			idx = idx;
		}
		AveErr /= TotalMaxDotCounts;
		idx = idx;
	}
	catch ( cv::Exception& e )
	{			
		const char* msg_e = e.what();  			
		m_ErrorString = msg_e;
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::CalcPhaseFactorZMappingFunc(int Mode, const std::vector<TPhaseFactorGrid> &List, std::vector<double> &KList)//計算相位高度比例參數-由相位轉成高度
{	//同1個XY位置上不同高度的映射函式(輸入同XY下不同的相位值, 輸入對應高度值)
	int   i=0,j=0,k=0,idx=0;	
	double AveErr=0.0, MaxErr=0.0;
	double Target=0, Factor=1.0;	
	const int MaxHeightParamCount = Mode;	
	const int TotalMaxDotCounts = (int)(List.size());		
	const int TotalMatrixElements = MaxHeightParamCount*MaxHeightParamCount;
	int NRows = MaxHeightParamCount;
	int NCols = MaxHeightParamCount;

	double tempD=0;	
	const double C = 1.0;
	std::vector<double> t(MaxHeightParamCount);
	std::vector<double> Step(TotalMaxDotCounts);
	std::vector<double> Error(TotalMaxDotCounts);
	std::vector<double> pResult(MaxHeightParamCount);
	std::vector<double> MatrixElem1(TotalMatrixElements);
	std::vector<double> MatrixElem2(MaxHeightParamCount);		

	std::fill(t.begin(), t.end(), 0);
	std::fill(Step.begin(), Step.end(), 0);
	std::fill(Error.begin(), Error.end(), 0);
	std::fill(pResult.begin(), pResult.end(), 0);	
	std::fill(MatrixElem1.begin(), MatrixElem1.end(), 0);
	std::fill(MatrixElem2.begin(), MatrixElem2.end(), 0);	

	TPhaseFactorGrid PhaseFactorGrid;
	for ( i=0; i<TotalMaxDotCounts; i++ )
	{
		PhaseFactorGrid = List[i];
		Target = PhaseFactorGrid.m_Height;
		GetHeightFactorPhaseParam(HEIGHT_FACTOR_PHASE, MaxHeightParamCount, PhaseFactorGrid, t);
		for ( j=0; j<MaxHeightParamCount; j++ )
		{
			//T[i][j] = t[j];
			tempD = Target*t[j];
			MatrixElem2[j] += tempD;
			//for ( k=0; k<MaxHeightParamCount; k++ )
			//{	TT[i][j][k] = t[j]*t[k];	}
		}		

		idx = 0;
		for ( j=0; j<MaxHeightParamCount; j++ )
		{
			for ( k=0; k<MaxHeightParamCount; k++ )
			{
				tempD = t[j]*t[k];
				MatrixElem1[idx] += tempD;
				idx ++;
			}			
		}
		idx = idx;
	}	
	
	try
	{
		//[mat1][A]=[mat2]
		//[mat1]^-1[mat1][A]=[mat1]^-1[mat2]
		//[I][A]=[mat1]^-1[mat2]
		//[A]=[mat3][mat2]
		//mat1.SetMatrixSize(NRows, NCols, MatrixElem1);
		//mat3=mat1.Inversion();
		//mat2.SetMatrixSize(NRows, 1, MatrixElem2);
		//mat4 = mat1*mat3;//測試反矩陣可用
		//mat5 = mat3*mat2;//求解
		//cv::Mat mat1(cv::Size(NRows,NCols), CV_64FC1, MatrixElem1);
		//cv::Mat mat2(cv::Size(NRows,1), CV_64FC1, MatrixElem2);	
		cv::Mat mat1(NRows, NCols, CV_64FC1, MatrixElem1.data());
		cv::Mat mat2(NRows, 1, CV_64FC1, MatrixElem2.data());	
		cv::Mat mat3=mat1.inv();	
		//測試反矩陣可用
		cv::Mat mat4=mat1*mat3;	
		//求解
		cv::Mat mat5 = mat3*mat2;
		//將資料放進去結果陣列中			
		for ( i=0; i<MaxHeightParamCount; i++ )
		{	
			pResult[i] = mat5.ptr<double>(0)[i];//.GetElement(0, i);	}
		}
		KList = pResult;		

		//驗證結果		
		for ( i=0; i<TotalMaxDotCounts; i++ )
		{
			Factor = 1.0;
			Step[i] = 0;
			Error[i] = 0;
			PhaseFactorGrid = List[i];
			Target=PhaseFactorGrid.m_Height;
			GetHeightFactorPhaseParam(HEIGHT_FACTOR_PHASE, MaxHeightParamCount, PhaseFactorGrid, t);
			Step[i] = Calc2ParamVecotr(pResult, t);			
			Step[i] *= Factor;
			Error[i] = fabs(Step[i]-Target);
			AveErr += Error[i];
			if ( MaxErr < Error[i] ) { MaxErr = Error[i]; }
			idx = idx;
		}
		AveErr /= TotalMaxDotCounts;
		idx = idx;
	}
	catch ( cv::Exception& e )
	{			
		const char* msg_e = e.what();  			
		m_ErrorString = msg_e;
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::CalcPhaseFactorXYKMappingFunc(int Mode, const std::vector<TPhaseFactorGrid> &List, std::vector<double> &KList)//計算相位高度比例參數-由XY計算出K值
{	//同1個高度下不同XY的K值映射函式(輸入XY, 輸出對應K值-在同高度下)
	int   i=0,j=0,k=0,idx=0;	
	double Target=0.0;
	double AveErr=0.0, MaxErr=0.0;
	double Phase=0, Height=0, Factor=1.0;	
	const int MaxHeightParamCount = Mode;	
	const int TotalMaxDotCounts = (int)(List.size());		
	const int TotalMatrixElements = MaxHeightParamCount*MaxHeightParamCount;
	int NRows = MaxHeightParamCount;
	int NCols = MaxHeightParamCount;

	double tempD=0;	
	const double C = 1.0;
	std::vector<double> MatrixElem1(TotalMatrixElements);
	std::vector<double> MatrixElem2(MaxHeightParamCount);		
	std::vector<double> t(MaxHeightParamCount);
	std::vector<double> Step(TotalMaxDotCounts);
	std::vector<double> Error(TotalMaxDotCounts);
	std::vector<double> pResult(MaxHeightParamCount);

	//Initialize
	std::fill(t.begin(), t.end(), 0);
	std::fill(Step.begin(), Step.end(), 0);
	std::fill(Error.begin(), Error.end(), 0);
	std::fill(pResult.begin(), pResult.end(), 0);	
	std::fill(MatrixElem1.begin(), MatrixElem1.end(), 0);
	std::fill(MatrixElem2.begin(), MatrixElem2.end(), 0);	
	//::memset(MatrixElem1, 0x00, sizeof(double)*TotalMatrixElements);
	//::memset(MatrixElem2, 0x00, sizeof(double)*MaxHeightParamCount);	

	TPhaseFactorGrid PhaseFactorGrid;
	for ( i=0; i<TotalMaxDotCounts; i++ )
	{
		PhaseFactorGrid = List[i];		
		Phase= PhaseFactorGrid.m_Phase;
		Height=PhaseFactorGrid.m_Height;
		if ( Phase < 0.1 ) { continue; }
		Target = Height/Phase;		
		GetHeightFactorXYParam(MaxHeightParamCount, PhaseFactorGrid, t);

		for ( j=0; j<MaxHeightParamCount; j++ )
		{
			tempD = Target*t[j];
			MatrixElem2[j] += tempD;			
		}		

		idx = 0;
		for ( j=0; j<MaxHeightParamCount; j++ )
		{
			for ( k=0; k<MaxHeightParamCount; k++ )
			{
				tempD = t[j]*t[k];
				MatrixElem1[idx] += tempD;
				idx ++;
			}			
		}
		idx = idx;
	}	
	
	try
	{
		//[mat1][A]=[mat2]
		//[mat1]^-1[mat1][A]=[mat1]^-1[mat2]
		//[I][A]=[mat1]^-1[mat2]
		//[A]=[mat3][mat2]		
		//cv::Mat mat1(cv::Size(NRows,NCols), CV_64FC1, MatrixElem1);//這樣行列數量換顛倒
		//cv::Mat mat2(cv::Size(NRows,1), CV_64FC1, MatrixElem2);//這樣行列數量換顛倒
		cv::Mat mat1(NRows, NCols, CV_64FC1, MatrixElem1.data());
		cv::Mat mat2(NRows, 1, CV_64FC1, MatrixElem2.data());	
		cv::Mat mat3=mat1.inv();//反矩陣			
		cv::Mat mat4=mat1*mat3;	//測試反矩陣可用		
		cv::Mat mat5 = mat3*mat2;//求解
		//將資料放進去結果陣列中			
		for ( i=0; i<MaxHeightParamCount; i++ )
		{	
			pResult[i] = mat5.ptr<double>(0)[i];//.GetElement(0, i);	}
		}
		KList = pResult;		
		//驗證結果		
		for ( i=0; i<TotalMaxDotCounts; i++ )
		{
			Factor = 1.0;	
			Step[i] = 0;
			Error[i] = 0;
			PhaseFactorGrid = List[i];
			Phase= PhaseFactorGrid.m_Phase;
			Height=PhaseFactorGrid.m_Height;									
			if ( Phase < 0.1 ) { continue; }
			Factor = Phase;
			GetHeightFactorXYParam(MaxHeightParamCount, PhaseFactorGrid, t);
			Step[i] = Calc2ParamVecotr(pResult, t);			
			Step[i] *= Factor;
			Error[i] = fabs(Step[i]-Height);
			AveErr += Error[i];
			if ( MaxErr < Error[i] ) { MaxErr = Error[i]; }
			idx = idx;
		}
		AveErr /= TotalMaxDotCounts;
		idx = idx;
	}
	catch ( cv::Exception& e )
	{			
		const char* msg_e = e.what();  			
		m_ErrorString = msg_e;
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::CalcPhaseFactorXYPhaseMappingFunc(int Mode, const std::vector<TPhaseFactorGrid> &List, std::vector<double> &KList)//計算相位高度比例參數-由XY計算出相位值
{	//同1個高度下不同XY的相位映射函式(輸入XY, 輸出對應相位-在同高度下)
	int   i=0,j=0,k=0,idx=0;	
	double AveErr=0.0, MaxErr=0.0;
	double Target=0, Factor=1.0;		
	const int MaxHeightParamCount=Mode;
	const int TotalMaxDotCounts = (int)(List.size());		
	const int TotalMatrixElements = MaxHeightParamCount*MaxHeightParamCount;
	int NRows = MaxHeightParamCount;
	int NCols = MaxHeightParamCount;

	double tempD=0;	
	const double C = 1.0;	
	std::vector<double> t(MaxHeightParamCount);
	std::vector<double> Step(TotalMaxDotCounts);
	std::vector<double> Error(TotalMaxDotCounts);
	std::vector<double> pResult(MaxHeightParamCount);
	std::vector<double> MatrixElem1(TotalMatrixElements);
	std::vector<double> MatrixElem2(MaxHeightParamCount);	

	std::fill(t.begin(), t.end(), 0);
	std::fill(Step.begin(), Step.end(), 0);
	std::fill(Error.begin(), Error.end(), 0);
	std::fill(pResult.begin(), pResult.end(), 0);	
	std::fill(MatrixElem1.begin(), MatrixElem1.end(), 0);
	std::fill(MatrixElem2.begin(), MatrixElem2.end(), 0);	
	
	TPhaseFactorGrid PhaseFactorGrid;
	for ( i=0; i<TotalMaxDotCounts; i++ )
	{
		PhaseFactorGrid = List[i];
		Target= PhaseFactorGrid.m_Phase;		
		GetHeightFactorXYParam(MaxHeightParamCount, PhaseFactorGrid, t);		
		for ( j=0; j<MaxHeightParamCount; j++ )
		{
			//T[i][j] = t[j];
			tempD = Target*t[j];
			MatrixElem2[j] += tempD;
			//for ( k=0; k<MaxHeightParamCount; k++ )
			//{	TT[i][j][k] = t[j]*t[k];	}
		}		

		idx = 0;
		for ( j=0; j<MaxHeightParamCount; j++ )
		{
			for ( k=0; k<MaxHeightParamCount; k++ )
			{
				tempD = t[j]*t[k];
				MatrixElem1[idx] += tempD;
				idx ++;
			}			
		}
		idx = idx;
	}	
	
	try
	{
		//[mat1][A]=[mat2]
		//[mat1]^-1[mat1][A]=[mat1]^-1[mat2]
		//[I][A]=[mat1]^-1[mat2]
		//[A]=[mat3][mat2]
		//mat1.SetMatrixSize(NRows, NCols, MatrixElem1);
		//mat3=mat1.Inversion();
		//mat2.SetMatrixSize(NRows, 1, MatrixElem2);
		//mat4 = mat1*mat3;//測試反矩陣可用
		//mat5 = mat3*mat2;//求解
		//cv::Mat mat1(cv::Size(NRows,NCols), CV_64FC1, MatrixElem1);
		//cv::Mat mat2(cv::Size(NRows,1), CV_64FC1, MatrixElem2);	
		cv::Mat mat1(NRows, NCols, CV_64FC1, MatrixElem1.data());
		cv::Mat mat2(NRows, 1, CV_64FC1, MatrixElem2.data());	
		cv::Mat mat3=mat1.inv();	
		//測試反矩陣可用
		cv::Mat mat4=mat1*mat3;	
		//求解
		cv::Mat mat5 = mat3*mat2;
		//將資料放進去結果陣列中			
		for ( i=0; i<MaxHeightParamCount; i++ )
		{	
			pResult[i] = mat5.ptr<double>(0)[i];//.GetElement(0, i);	}
		}
		KList = pResult;		
		//驗證結果		
		for ( i=0; i<TotalMaxDotCounts; i++ )
		{
			Factor = 1.0;
			Step[i] = 0;
			Error[i] = 0;
			PhaseFactorGrid = List[i];
			Target= PhaseFactorGrid.m_Phase;			
			GetHeightFactorXYParam(MaxHeightParamCount, PhaseFactorGrid, t);
			Step[i] = Calc2ParamVecotr(pResult, t);			
			Step[i] *= Factor;
			Error[i] = fabs(Step[i]-Target);
			AveErr += Error[i];
			if ( MaxErr < Error[i] ) { MaxErr = Error[i]; }
			idx = idx;
		}
		AveErr /= TotalMaxDotCounts;
		idx = idx;
	}
	catch ( cv::Exception& e )
	{			
		const char* msg_e = e.what();  			
		m_ErrorString = msg_e;
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataCollect::GetPhaseFactorFolder(bool bTemp)//取得相位高度比例資料夾
{	
	CString Root;	
	CString Folder;		
	if ( false == bTemp )
	{	Root = GetAOIDirectory();	}
	else	
	{	Root = GetAOITempDirectory();	}	
	Folder.Format(_T("%s\\%s"), Root, _T("Calibration"));		
	return Folder;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataCollect::GetPhaseFactorFilename(LIGHT_3D_CAST_ID CastID, double TargetHeight, bool bTemp)//取得相位高度比例參數檔案	
{
	CString Filename;	
	CString MainName;
	CString Folder = GetPhaseFactorFolder(bTemp);
	const int nTargetHeight = (int)(TargetHeight+0.1);	
#ifdef TB_SYSTEM_ONLY_BOT
	MainName = _T("HeightFactorBot");	
#else	
	MainName = _T("HeightFactor");
#endif//TB_SYSTEM_ONLY_BOT
	Filename.Format(_T("%s\\%s_ID%02d_%02d.DAT"), Folder, MainName, CastID, nTargetHeight);	
	//Filename.Format(_T("%s\\HeightFactor_ID%02d_%02d.DAT"), Folder, CastID, nTargetHeight);	
	return Filename;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::LoadPhaseFactorTableFile(LPCTSTR pfilename, TPhaseFactorTable &GridTable)//載入相位高度比例參數檔案
{
	if ( LoadPhaseFactorTableFileFn(pfilename, GridTable) == false )
	{
		SetSystemExceptionCode_FileRead();
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::LoadPhaseFactorTableFileFn(LPCTSTR pfilename, TPhaseFactorTable &GridTable)//載入相位高度比例參數檔案
{
	if ( NULL == pfilename )  { return false; }

	HCURSOR hCursor = ::AfxGetApp()->LoadStandardCursor(IDC_WAIT);
	HCURSOR hOldCursor = ::SetCursor(hCursor);

	CString      str;
	bool         IsOK=true;
	CAOIFileIO   FileIO;
	CString      FileName = pfilename;

	FileIO.CreateProgressWnd();	
	FileIO.SetFileName(FileName);	
	FileIO.SetFileTarget(FILE_TARGET_OTHERS);	
	//----------------------------------------------------------------------------------------//		
	IsOK = FileIO.OpenLoadFile(FileName);	
	if ( false == IsOK )
	{	
		m_ErrorString = FileIO.GetErrorString();
		FileIO.DestroyProgressWnd();
		::SetCursor(hOldCursor);		
		return false;
	}
	IsOK = FileIO.ReadPhaseFactorTableFile(GridTable);
	if ( false == IsOK )
	{
		m_ErrorString = FileIO.GetErrorString();
		FileIO.CloseFile();
		FileIO.DestroyProgressWnd();
		::SetCursor(hOldCursor);
		return false;
	}
	FileIO.CloseFile();
	FileIO.DestroyProgressWnd();		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::SavePhaseFactorTableFile(LPCTSTR pfilename, const TPhaseFactorTable &GridTable)//儲存相位高度比例參數檔案		
{
	if ( SavePhaseFactorTableFileFn(pfilename, GridTable) == false )
	{
		SetSystemExceptionCode_FileWrite();
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::SavePhaseFactorTableFileFn(LPCTSTR pfilename, const TPhaseFactorTable &GridTable)//儲存相位高度比例參數檔案			
{
	CString      str;	
	CAOIFileIO   FileIO;
	bool         IsOK=true;
	CString      filename = pfilename;
	CString      FileNameDst = pfilename;

	HCURSOR hCursor = ::AfxGetApp()->LoadStandardCursor(IDC_WAIT);
	HCURSOR hOldCursor = ::SetCursor(hCursor);

	FileIO.CreateProgressWnd();
	FileIO.SetFileName(FileNameDst);
	FileIO.SetFileTarget(FILE_TARGET_OTHERS);		

	IsOK = FileIO.OpenSaveFile(filename);	
	if ( false == IsOK )
	{
		m_ErrorString = FileIO.GetErrorString();
		FileIO.DestroyProgressWnd();
		::SetCursor(hOldCursor);
		return false;
	}	

	if ( FileIO.SaveChunk_INT(FILE_IO_START, 0) == false ) { return false; }
	IsOK = FileIO.WritePhaseFactorTableFile(GridTable);		
	if ( false == IsOK )
	{
		m_ErrorString = FileIO.GetErrorString();
		FileIO.CloseFile();
		FileIO.DestroyProgressWnd();
		::SetCursor(hOldCursor);
		return false;
	}
	if ( FileIO.SaveChunk_INT(FILE_IO_END, 0) == false ) { return false; }
	FileIO.CloseFile();
	FileIO.DestroyProgressWnd();	
	return true;	
}
//-------------------------------------------------------------------------------------//
CString CAOIDataCollect::GetDotNodeCaliFilename(LANE_ID LaneID)//取得DotNode參數檔案
{	
	CString Folder = GetAOIDirectory();
	CString Filename=GetDotNodeCaliFilename(Folder, LaneID);	
	return Filename;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataCollect::GetDotNodeCaliFilename(LPCTSTR Folder, LANE_ID LaneID)//取得DotNode參數檔案	
{
	CString Filename;	
	switch ( LaneID )
	{
	case LANE_ID_B:
		Filename.Format(_T("%s\\%s"), Folder, _T("XYDotNodeList_LB.DAT"));
		break;
	default:
	case LANE_ID_A:
		Filename.Format(_T("%s\\%s"), Folder, _T("XYDotNodeList_LA.DAT"));
		break;
	}
	return Filename;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::MoveXYDotNodeList(double OffsetX, double OffsetY, std::vector<TDotNode> &List)//移動XY校正檔案
{
	size_t i = 0;	
	const size_t DotNodeCount = List.size();
	for (i = 0; i<DotNodeCount; i++)
	{
		TDotNode &DotNode = List[i];
		DotNode.dPosX += OffsetX;
		DotNode.dPosY += OffsetY;

		DotNode.dCaliPosX += OffsetX;
		DotNode.dCaliPosY += OffsetY;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::LoadXYDotNodeCaliFile(LPCTSTR pfilename, int &CountX, int &CountY, std::vector<TDotNode> &List)//載入XY校正檔案
{
	if ( LoadXYDotNodeCaliFileFn(pfilename, CountX, CountY, List) == false )
	{
		SetSystemExceptionCode_FileRead();
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::LoadXYDotNodeCaliFileFn(LPCTSTR pfilename, int &CountX, int &CountY, std::vector<TDotNode> &List)//載入XY校正檔案
{
	if ( NULL == pfilename )  { return false; }

	HCURSOR hCursor = ::AfxGetApp()->LoadStandardCursor(IDC_WAIT);
	HCURSOR hOldCursor = ::SetCursor(hCursor);

	CString      str;
	bool         IsOK=true;
	CAOIFileIO   FileIO;
	CString      FileName = pfilename;

	FileIO.CreateProgressWnd();	
	FileIO.SetFileName(FileName);	
	FileIO.SetFileTarget(FILE_TARGET_OTHERS);	
	//----------------------------------------------------------------------------------------//		
	IsOK = FileIO.OpenLoadFile(FileName);	
	if ( false == IsOK )
	{	
		m_ErrorString = FileIO.GetErrorString();
		FileIO.DestroyProgressWnd();
		::SetCursor(hOldCursor);		
		return false;
	}
	IsOK = FileIO.ReadXYDotNodeCaliFile(CountX, CountY, List);
	if ( false == IsOK )
	{
		m_ErrorString = FileIO.GetErrorString();
		FileIO.CloseFile();
		FileIO.DestroyProgressWnd();
		::SetCursor(hOldCursor);
		return false;
	}
	FileIO.CloseFile();
	FileIO.DestroyProgressWnd();		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::SaveXYDotNodeCaliFile(LPCTSTR pfilename, int CountX, int CountY, const std::vector<TDotNode> &List)//儲存XY校正檔案
{
	if ( SaveXYDotNodeCaliFileFn(pfilename, CountX, CountY, List) == false )
	{
		SetSystemExceptionCode_FileWrite();
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::SaveXYDotNodeCaliFileFn(LPCTSTR pfilename, int CountX, int CountY, const std::vector<TDotNode> &List)//儲存XY校正檔案
{
	CString      str;	
	CAOIFileIO   FileIO;
	bool         IsOK=true;
	CString      filename = pfilename;
	CString      FileNameDst = pfilename;

	HCURSOR hCursor = ::AfxGetApp()->LoadStandardCursor(IDC_WAIT);
	HCURSOR hOldCursor = ::SetCursor(hCursor);

	FileIO.CreateProgressWnd();
	FileIO.SetFileName(FileNameDst);
	FileIO.SetFileTarget(FILE_TARGET_OTHERS);		

	IsOK = FileIO.OpenSaveFile(filename);	
	if ( false == IsOK )
	{
		m_ErrorString = FileIO.GetErrorString();
		FileIO.DestroyProgressWnd();
		::SetCursor(hOldCursor);
		return false;
	}	

	if ( FileIO.SaveChunk_INT(FILE_IO_START, 0) == false ) { return false; }
	IsOK = FileIO.WriteXYDotNodeCaliFile(CountX, CountY, List);		
	if ( false == IsOK )
	{
		m_ErrorString = FileIO.GetErrorString();
		FileIO.CloseFile();
		FileIO.DestroyProgressWnd();
		::SetCursor(hOldCursor);
		return false;
	}
	if ( FileIO.SaveChunk_INT(FILE_IO_END, 0) == false ) { return false; }
	FileIO.CloseFile();
	FileIO.DestroyProgressWnd();	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ConvertXYDotNodeToXYCali(int CountX, int CountY, const std::vector<TDotNode> &DotList, std::vector<TXYCali> &XYCaliList)//轉換XY節點至XY校正
{
	int          NPos=0;
	int          index=0;
	int          i=0, j=0;
	TXYCali      XYCali, Cali;
	const int    DOT_CORNER_1 = 0;
	const int    DOT_CORNER_2 = 1;
	const int    DOT_CORNER_3 = 2;
	const int    DOT_CORNER_4 = 3;
	const int    DOT_CORNER_TOTAL = 4;
	TDotNode     DotNode[DOT_CORNER_TOTAL];
	const int    DotNodeCount = (int)(DotList.size());
	/*
	const double PitchX = GetDotPitchX();
	const double PitchY = GetDotPitchY();		
	const double CellRangeXMin = fabs(PitchX)*0.25;
	const double CellRangeYMin = fabs(PitchY)*0.25;
	const double CellRangeXMax = fabs(PitchX)*1.25;
	const double CellRangeYMax = fabs(PitchY)*1.25;		
	const bool  bSignX = GetStageSignPositiveX();
	const bool  bSignY = GetStageSignPositiveY();
	*/
	for ( j=0; j<CountY-1; j++ )
	{
		for ( i=0; i<CountX-1; i++ )
		{
			NPos = 0;		
			MotionCtrlPtr->InitMotionXYCali(XYCali);

			NPos = 1;
			index = (j*CountX)+i;
			if ( index >= DotNodeCount ) { return false; }
			DotNode[DOT_CORNER_1] = DotList[index];
			index = (j*CountX)+i+1;
			if ( index >= DotNodeCount ) { return false; }
			DotNode[DOT_CORNER_2] = DotList[index];
			index = ((j+1)*CountX)+i;
			if ( index >= DotNodeCount ) { return false; }
			DotNode[DOT_CORNER_3] = DotList[index];
			index = ((j+1)*CountX)+i+1;
			if ( index >= DotNodeCount ) { return false; }
			DotNode[DOT_CORNER_4] = DotList[index];			

			//使用最接近的校正座標, 而非理論座標+偏差值, 否則會有新的偏差值
			XYCali.PosX1 = DotNode[DOT_CORNER_1].dPosX;
			XYCali.PosY1 = DotNode[DOT_CORNER_1].dPosY;
			XYCali.CaliX1 = DotNode[DOT_CORNER_1].dCaliPosX;
			XYCali.CaliY1 = DotNode[DOT_CORNER_1].dCaliPosY;

			XYCali.PosX2 = DotNode[DOT_CORNER_2].dPosX;
			XYCali.PosY2 = DotNode[DOT_CORNER_2].dPosY;
			XYCali.CaliX2 = DotNode[DOT_CORNER_2].dCaliPosX;
			XYCali.CaliY2 = DotNode[DOT_CORNER_2].dCaliPosY;

			XYCali.PosX3 = DotNode[DOT_CORNER_3].dPosX;
			XYCali.PosY3 = DotNode[DOT_CORNER_3].dPosY;
			XYCali.CaliX3 = DotNode[DOT_CORNER_3].dCaliPosX;
			XYCali.CaliY3 = DotNode[DOT_CORNER_3].dCaliPosY;

			XYCali.PosX4 = DotNode[DOT_CORNER_4].dPosX;
			XYCali.PosY4 = DotNode[DOT_CORNER_4].dPosY;
			XYCali.CaliX4 = DotNode[DOT_CORNER_4].dCaliPosX;
			XYCali.CaliY4 = DotNode[DOT_CORNER_4].dCaliPosY;

			MotionCtrlPtr->RearrangeMotionXYCali(XYCali);
			MotionCtrlPtr->CalcMotionXYCaliLimit(XYCali);
			MotionCtrlPtr->CalcMotionXYCaliTransform(XYCali);
			Cali = XYCali;
			XYCaliList.push_back(XYCali);
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::RefineXYDotNodeListByLane(const std::vector<TDotNode> &DotList_LA, const std::vector<TDotNode> &DotList_LB, std::vector<TDotNode> &DotListA, std::vector<TDotNode> &DotListB)//提煉AB軌道的XY節點
{
	//取整個AB軌道校正範圍, 然後各取一半為AB軌道的校正區域
	size_t       i=0;
	bool         bFirst=true;
	TREGION4D    DotRgn;
	TREGION4D    DotRgn_LA;
	TREGION4D    DotRgn_LB;
	TDotNode    *DotNodePtr=NULL;
	const size_t DotCount_LA = DotList_LA.size();
	const size_t DotCount_LB = DotList_LB.size();

	if ( 0==DotCount_LA || 0==DotCount_LB )
	{	
		DotListA = DotList_LA;
		DotListB = DotList_LB;		
		return true;
	}
	DotListA.clear();
	DotListB.clear();

	//Lane A Region
	bFirst=true;
	for ( i=0; i<DotCount_LA; i++ )
	{
		DotNodePtr = (TDotNode*)&(DotList_LA[i]);
		if ( NULL == DotNodePtr ) { continue; }
		if ( true == bFirst ) 
		{
			bFirst = false;
			DotRgn_LA.minX = DotRgn_LA.maxX = DotNodePtr->dPosX;
			DotRgn_LA.minY = DotRgn_LA.maxY = DotNodePtr->dPosY;
			continue;
		}
		if ( DotRgn_LA.minX > DotNodePtr->dPosX ) { DotRgn_LA.minX = DotNodePtr->dPosX; }
		else if ( DotRgn_LA.maxX < DotNodePtr->dPosX ) { DotRgn_LA.maxX = DotNodePtr->dPosX; }
		if ( DotRgn_LA.minY > DotNodePtr->dPosY ) { DotRgn_LA.minY = DotNodePtr->dPosY; }
		else if ( DotRgn_LA.maxY < DotNodePtr->dPosY ) { DotRgn_LA.maxY = DotNodePtr->dPosY; }
	}

	//Lane B Region
	bFirst = true;
	for ( i=0; i<DotCount_LB; i++ )
	{
		DotNodePtr = (TDotNode*)&(DotList_LB[i]);
		if ( NULL == DotNodePtr ) { continue; }
		if ( true == bFirst ) 
		{
			bFirst = false;
			DotRgn_LB.minX = DotRgn_LB.maxX = DotNodePtr->dPosX;
			DotRgn_LB.minY = DotRgn_LB.maxY = DotNodePtr->dPosY;
			continue;
		}
		if ( DotRgn_LB.minX > DotNodePtr->dPosX ) { DotRgn_LB.minX = DotNodePtr->dPosX; }
		else if ( DotRgn_LB.maxX < DotNodePtr->dPosX ) { DotRgn_LB.maxX = DotNodePtr->dPosX; }
		if ( DotRgn_LB.minY > DotNodePtr->dPosY ) { DotRgn_LB.minY = DotNodePtr->dPosY; }
		else if ( DotRgn_LB.maxY < DotNodePtr->dPosY ) { DotRgn_LB.maxY = DotNodePtr->dPosY; }
	}

	DotRgn = DotRgn_LA;
	DotRgn.Expand(DotRgn_LB);

	const double MinLineY=DotRgn.GetCpY();

	//分別將軌道AB值放入	
	for ( i=0; i<DotCount_LA; i++ )
	{
		DotNodePtr = (TDotNode*)&(DotList_LA[i]);
		if ( NULL == DotNodePtr ) { continue; }
		if ( DotNodePtr->dPosY > MinLineY ) 
		{	continue;	}
		DotListA.push_back(DotList_LA[i]);
	}

	for ( i=0; i<DotCount_LB; i++ )
	{
		DotNodePtr = (TDotNode*)&(DotList_LB[i]);
		if ( NULL == DotNodePtr ) { continue; }
		if ( DotNodePtr->dPosY < MinLineY ) 
		{	continue;	}
		DotListB.push_back(DotList_LB[i]);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::SaveSystemFilenameSyntax()//儲存檔名語句
{
	bool IsOK = true;
	CString Err, Tmp;
	CString ErrString;
	CString Filename;
	CString Section;
	CString Folder=GetAOIDirectory();
	Filename.Format(_T("%s\\%s"), Folder, _T("FilenameSyntax.INI"));

	SetErrorString(_T(""));
	//單板底圖
	Section = _T("Board Map");	
	if ( SaveSystemFilenameSyntax(Filename, Section, m_FilenameSyntax_BoardMap, ErrString) == false )
	{	IsOK = false; }	

	//整板底圖
	Section = _T("Panel Map");
	if ( SaveSystemFilenameSyntax(Filename, Section, m_FilenameSyntax_PanelMap, ErrString) == false )
	{	IsOK = false; }

	//專案底圖
	Section = _T("Project Map");
	if ( SaveSystemFilenameSyntax(Filename, Section, m_FilenameSyntax_ProjectMap, ErrString) == false )
	{	IsOK = false; }

	//客戶報告
	Section = _T("Customer Report");
	if ( SaveSystemFilenameSyntax(Filename, Section, m_FilenameSyntax_CustomerReport, ErrString) == false )
	{	IsOK = false; }

	if ( false == IsOK )
	{	
		Err.Format(_T("Error, %s"), ErrString);
		SetErrorString(Err);	
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::LoadSystemFilenameSyntax()//載入檔名語句
{
	bool IsOK = true;
	CString Err, Tmp;
	CString ErrString;
	CString Filename;
	CString Section;
	CString Folder=GetAOIDirectory();
	Filename.Format(_T("%s\\%s"), Folder, _T("FilenameSyntax.INI"));

	SetErrorString(_T(""));
	//單板底圖	
	Section = _T("Board Map");
	if ( LoadSystemFilenameSyntax(Filename, Section, m_FilenameSyntax_BoardMap, ErrString) == false )
	{	IsOK = false; }

	//整板底圖
	Section = _T("Panel Map");
	if ( LoadSystemFilenameSyntax(Filename, Section, m_FilenameSyntax_PanelMap, ErrString) == false )
	{	IsOK = false; }

	//專案底圖
	Section = _T("Project Map");
	if ( LoadSystemFilenameSyntax(Filename, Section, m_FilenameSyntax_ProjectMap, ErrString) == false )
	{	IsOK = false; }

	//客戶報告
	Section = _T("Customer Report");
	if ( LoadSystemFilenameSyntax(Filename, Section, m_FilenameSyntax_CustomerReport, ErrString) == false )
	{	IsOK = false; }

	if ( false == IsOK )
	{
		Err.Format(_T("Error, %s"), ErrString);
		SetErrorString(Err);	
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::SaveSystemFilenameSyntax(LPCTSTR Filename, LPCTSTR Section, CFilenameSyntax &Syntax, CString &ErrString)//儲存檔名語句
{
	const bool IsOK=Syntax.SaveFilenameSyntaxIni(Filename, Section);
	if ( false == IsOK )
	{
		CString Err;
		Err.Format(_T("Save %s Filename Syntax Fault[%s]"), Section, Syntax.GetErrorString());
		if ( ErrString.GetLength() == 0 ) { ErrString = Err; }
		else { ErrString += CString(_T("\n"))+Err;	}
		SetSystemExceptionCode_FileWrite(Err);
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::LoadSystemFilenameSyntax(LPCTSTR Filename, LPCTSTR Section, CFilenameSyntax &Syntax, CString &ErrString)//載入檔名語句
{
	Syntax.SetFilenameSyntaxTag(Section);
	const bool IsOK = Syntax.LoadFilenameSyntaxIni(Filename, Section);
	if ( false == IsOK )
	{	
		CString Err;
		Err.Format(_T("Load %s Filename Syntax Fault[%s]"), Section, Syntax.GetErrorString());
		if ( ErrString.GetLength() == 0 ) { ErrString = Err; }
		else { ErrString += CString(_T("\n"))+Err;	}
		SetSystemExceptionCode_FileRead(Err);
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
CFilenameSyntax& CAOIDataCollect::GetFilenameSyntax_BoardMap()  //單板底圖檔名語句
{
	return m_FilenameSyntax_BoardMap;
}
//-------------------------------------------------------------------------------------//
void CAOIDataCollect::SetFilenameSyntax_BoardMap(CFilenameSyntax &Syntax)//單板底圖檔名語句
{
	m_FilenameSyntax_BoardMap = Syntax;
}
//-------------------------------------------------------------------------------------//
CFilenameSyntax& CAOIDataCollect::GetFilenameSyntax_PanelMap()  //整板底圖檔名語句
{
	return m_FilenameSyntax_PanelMap;
}
//-------------------------------------------------------------------------------------//
void CAOIDataCollect::SetFilenameSyntax_PanelMap(CFilenameSyntax &Syntax)//整板底圖檔名語句
{
	m_FilenameSyntax_PanelMap = Syntax;
}
//-------------------------------------------------------------------------------------//
CFilenameSyntax& CAOIDataCollect::GetFilenameSyntax_ProjectMap()//專案底圖檔名語句
{
	return m_FilenameSyntax_ProjectMap;
}
//-------------------------------------------------------------------------------------//
void CAOIDataCollect::SetFilenameSyntax_ProjectMap(CFilenameSyntax &Syntax)//專案底圖檔名語句
{
	m_FilenameSyntax_ProjectMap = Syntax;
}
//-------------------------------------------------------------------------------------//
CFilenameSyntax& CAOIDataCollect::GetFilenameSyntax_CustomerReport()  //客戶報告檔名語句
{
	return m_FilenameSyntax_CustomerReport;
}
//-------------------------------------------------------------------------------------//
void CAOIDataCollect::SetFilenameSyntax_CustomerReport(CFilenameSyntax &Syntax)//客戶報告檔名語句
{
	m_FilenameSyntax_CustomerReport = Syntax;
}
//-------------------------------------------------------------------------------------//