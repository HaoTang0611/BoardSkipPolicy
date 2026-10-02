#ifndef _JET8000MSGDef_H_
#define _JET8000MSGDef_H_

#pragma warning (disable:4819)
//----------------------------------------------------------------------------//
//JET8000MSGDef.h
//JET8000所自訂的視窗訊息
//初寫日期::2016/07/01
//撰寫者::廖振凱(Nathan)
//財產所屬::捷智科技
//版本::1.00
//------------------------------------------------------------------------------------------//
//Ribbon 引數 意義
#define RIBBON_INDEX_ONLINE_MAIN_VIEW   1
#define RIBBON_INDEX_EDIT_MAIN_VIEW     2
#define RIBBON_INDEX_EDIT_MODEL_VIEW    3
#define RIBBON_INDEX_EDIT_BARCODE_VIEW  4
#define RIBBON_INDEX_EDIT_MARK_VIEW     5
#define RIBBON_INDEX_EDIT_FD_VIEW       6
#define RIBBON_INDEX_EDIT_SYSTEM_VIEW   7
#define RIBBON_INDEX_EDIT_DEBUG_VIEW    8
//------------------------------------------------------------------------------------------//
#define        MSG_MODE_NONE                              0//不處理
#define        MSG_MODE_BUILD                             1//重建
#define        MSG_MODE_UPDATE                            2//更新
//------------------------------------------------------------------------------------------//
//CAMERA CALLBACK MSG
const UINT     MSG_CAMERA_CALLBACK                        = WM_USER+10;
const UINT     MSG_CAMERA_REGRAB_IMAGE                    = WM_USER+11;//重新取像

const WPARAM   WPARAM_CAMERA_1_CALLBACK                   =  1; 
const WPARAM   WPARAM_CAMERA_2_CALLBACK                   =  2;
const WPARAM   WPARAM_CAMERA_3_CALLBACK                   =  3;
const WPARAM   WPARAM_CAMERA_4_CALLBACK                   =  4;
const WPARAM   WPARAM_CAMERA_5_CALLBACK                   =  5;

const LPARAM   LPARAM_CAMERA_FRAME_CALLBACK               =  1;
const LPARAM   LPARAM_CAMERA_BYPASS_CALLBACK              =  2;
//------------------------------------------------------------------------------------------//
const UINT     MSG_MOTION_CALLBACK                        = WM_USER+21;
const WPARAM   WPARAM_MOTION_FORWARD_FINISH_CALLBACK      = 1;//正向運動(機構)行程結束
const WPARAM   WPARAM_MOTION_BACKWARD_FINISH_CALLBACK     = 2;//逆向運動(機構)行程結束
const WPARAM   WPARAM_MOTION_PROCESS_FINISH_CALLBACK      = 4;//運動(機構)行程結束
//------------------------------------------------------------------------------------------//
const UINT     MSG_LOCK_SCREEN_HOOK                       = WM_USER+29;//系統鎖住螢幕訊息
//------------------------------------------------------------------------------------------//
const UINT     MSG_INSPECTION_CALLBACK                    = WM_USER+31;//檢測訊息
const WPARAM   WPARAM_INSPECTION_PROJECT_MARK             = 1;//檢測狀態-專案特徵
const WPARAM   WPARAM_INSPECTION_PROJECT_INITIAL          = 2;//檢測狀態-專案初始化
const WPARAM   WPARAM_INSPECTION_PANEL_FD                 = 3;//檢測狀態-整板定位點
const WPARAM   WPARAM_INSPECTION_BOARD_FD                 = 4;//檢測狀態-單板定位點
const WPARAM   WPARAM_INSPECTION_BARCODE                  = 5;//檢測狀態-條碼檢測
const WPARAM   WPARAM_INSPECTION_PROJECT_RESET            = 6;//檢測狀態-專案覆歸
const WPARAM   WPARAM_INSPECTION_PROJECT_TEST             = 7;//檢測狀態-專案檢測
const WPARAM   WPARAM_INSPECTION_PROJECT_ANALYSIS         = 8;//檢測狀態-專案分析
const WPARAM   WPARAM_INSPECTION_FINISH                   = 9;//檢測狀態-檢測結束
const WPARAM   WPARAM_INSPECTION_ONLINE_FINISH            =10;//檢測狀態-線上檢測
const WPARAM   WPARAM_INSPECTION_RE_TUNING                =11;//檢測狀態-重新調機
const WPARAM   WPARAM_INSPECTION_RE_INSPECT               =12;//檢測狀態-重新檢測
//------------------------------------------------------------------------------------------//
const UINT     MSG_SYSTEM_EXCEPTION_CALLBACK              = WM_USER+41;//影像或機構異常狀況發生
const UINT     MSG_SYSTEM_AUTO_RETRY                      = WM_USER+42;//系統重新復歸
const LPARAM   LPARAM_SYSTEM_EXCEPTION_CAMERA             = 0x000000001;//系統異常-相機
const LPARAM   LPARAM_SYSTEM_EXCEPTION_MOTION             = 0x000000002;//系統異常-XYZ運動
const LPARAM   LPARAM_SYSTEM_EXCEPTION_PLC                = 0x000000004;//系統異常-PLC
const LPARAM   LPARAM_SYSTEM_EXCEPTION_LIGHT              = 0x000000008;//系統異常-燈盤控制
const LPARAM   LPARAM_SYSTEM_EXCEPTION_INSPECTION         = 0x000000010;//系統異常-檢測中
//------------------------------------------------------------------------------------------//
const UINT     MSG_SYSTEM_CALIBRATION_CALLBACK             = WM_USER+51;//系統校正訊息
const WPARAM   WPARAM_GRAB_NEXT_DOT_TARGET                 = 1;//重新取像
const WPARAM   WPARAM_GRAB_NEXT_CALIBRATION                = 2;//重新取像
//------------------------------------------------------------------------------------------//
const UINT     MSG_CALIBRATION_ALIGN_WND                   = WM_USER+52;//送至校正對位視窗
const WPARAM   WPARAM_SAVE_SYSTEM_PARAM                    = 1;//儲存系統參數
//------------------------------------------------------------------------------------------//
const UINT     MSG_CALIBRATION_STAGE_WND                   = WM_USER+53;//送至校正機台視窗
//------------------------------------------------------------------------------------------//
const UINT     MSG_SELF_WND_EXTRA_MESSAGE                  = WM_USER+60;//本身視窗額外訊息
const WPARAM   WPARAM_FIRST_UI_CALLBACK                    = 1;//更新專案機台座標
//------------------------------------------------------------------------------------------//
const UINT     MSG_IMAGE_WND_DRAW_NEXT                     = WM_USER+61;//在控制項繪圖後

const UINT     MSG_IMAGE_WND_NOTIFY_EVENT                  = WM_USER+62;//繪圖視窗通知事件
const WPARAM   WPARAM_MODIFY_STAGE_POS                     = 1;//更新專案機台座標
const WPARAM   WPARAM_LBUTTON_DOWN                         = 11;//左鍵按下
const WPARAM   WPARAM_LBUTTON_UP                           = 12;//左鍵放開
const WPARAM   WPARAM_LBUTTON_DBCLICK                      = 13;//左鍵雙擊
const WPARAM   WPARAM_RBUTTON_DOWN                         = 21;//右鍵按下
const WPARAM   WPARAM_RBUTTON_UP                           = 22;//右鍵放開
const WPARAM   WPARAM_RBUTTON_DBCLICK                      = 23;//左鍵雙擊
const WPARAM   WPARAM_MOUSE_MOVE                           = 31;//滑鼠移動
const WPARAM   WPARAM_MOUSE_WHEEL                          = 32;//滾輪移動
const WPARAM   WPARAM_CONTEXT_MENU                         = 41;//蹦跳選單
//------------------------------------------------------------------------------------------//
const UINT     MSG_TREE_WND_MOVE_TO_ACTIVE_OBJ             = WM_USER+71;//移動至選到的物件
const WPARAM   WPARAM_MOVE_TO_TEACH_STAGE_POS              = 1;//移動至教導時機台
const WPARAM   WPARAM_MOVE_TO_CURRENT_STAGE_POS            = 2;//移動至現今時機台
//------------------------------------------------------------------------------------------//
const UINT     MSG_COLOR_FILTER_WND                        = WM_USER+81;//彩色過濾
const WPARAM   WPARAM_UPDATE_COLOR_FILTER                  = 1;//更新彩色過濾參數
const WPARAM   WPARAM_UPDATE_COLOR_FILTER_BTN_UP           = 2;//更新彩色過濾參數-按鈕放開
const WPARAM   WPARAM_UPDATE_COLOR_FILTER_LEFT_BTN_UP      = 3;//更新彩色過濾參數-左按鈕放開
const WPARAM   WPARAM_UPDATE_COLOR_FILTER_RIGHT_BTN_UP     = 4;//更新彩色過濾參數-右按鈕放開
const WPARAM   WPARAM_UPDATE_WND_COLOR                     = 11;//更新檢測框顏色
//------------------------------------------------------------------------------------------//
const UINT     MSG_OPEN_GL_WND                             = WM_USER+82;//OpenGL視窗
const WPARAM   WPARAM_UPDATE_CLIP_PLANE                    = 1;//OpenGL視窗-變更了切割平面
//------------------------------------------------------------------------------------------//
const UINT     MSG_SOCKET_WND                              = WM_USER+83;//Socket視窗
const WPARAM   SOCKET_CLIENT_RECV_TEXT                     = 10;//Socket客戶-收到訊息
const WPARAM   SOCKET_CLIENT_SEND_TEXT                     = 11;//Socket客戶-發送訊息
const WPARAM   SOCKET_SERVER_RECV_TEXT                     = 20;//Socket伺服器-收到訊息
const WPARAM   SOCKET_SERVER_SEND_TEXT                     = 21;//Socket伺服器-發送訊息
//------------------------------------------------------------------------------------------//
const UINT     MSG_MAIN_FRAME_MESSAGE                      = WM_USER+100;//主框架訊息
const WPARAM   WPARAM_INITIAL_MAIN_FRAME                   =   1;//初始化應用程式框架
const WPARAM   WPARAM_RESET_MSG_FROM_ID                    =   2;//復原視窗來源編號
const WPARAM   WPARAM_SWITCH_USER                          =   3;//切換使用者

const WPARAM   WPARAM_PROJECT_NEW                          = 101;//專案新開
const WPARAM   WPARAM_PROJECT_OPEN                         = 102;//專案開啓
const WPARAM   WPARAM_PROJECT_CLOSE                        = 103;//專案關閉
const WPARAM   WPARAM_PROJECT_SWITCH                       = 104;//專案切換
const WPARAM   WPARAM_PROJECT_UPDATE                       = 105;//專案更新
const WPARAM   WPARAM_PROJECT_UPDATE_FD_ALIGN              = 106;//專案更新-定位點
const WPARAM   WPARAM_PROJECT_PART_SELECTED                = 107;//專案更新的選取元件
const WPARAM   WPARAM_PROJECT_PART_DELETED                 = 108;//移除刪除的元件
const WPARAM   WPARAM_PROJECT_SWITCH_MARK                  = 109;//專案切換標記
const WPARAM   WPARAM_PROJECT_MODIFY_MAP                   = 110;//專案變更底圖
const WPARAM   WPARAM_PROJECT_SWITCH_MAP                   = 111;//專案切換底圖
const WPARAM   WPARAM_PROJECT_SWITCH_LANE                  = 112;//專案切換軌道
const WPARAM   WPARAM_PROJECT_CLOSE_ACTIVE_OBJ             = 113;//專案關閉選取的物件

const WPARAM   WPARAM_RESET_CALLBACK_HWND                  = 201;//重設回傳視窗至主畫面
const WPARAM   WPARAM_CALC_CURRENT_FOV_POSITION            = 202;//計算現今位置
const WPARAM   WPARAM_SWITCH_TO_ONLINE_VIEW                = 203;//切換視窗-線上檢測
const WPARAM   WPARAM_SWITCH_TO_EDIT_MAIN_VIEW             = 204;//切換視窗-主編輯
const WPARAM   WPARAM_SWITCH_TO_EDIT_MODEL_VIEW            = 205;//切換視窗-模組編輯
const WPARAM   WPARAM_SWITCH_TO_EDIT_FD_VIEW               = 206;//切換視窗-定位點編輯
const WPARAM   WPARAM_SWITCH_TO_EDIT_SB_VIEW               = 207;//切換視窗-軟體條碼編輯
const WPARAM   WPARAM_UPDATE_DOCUMENT_TITLE                = 221;//專案更新主視窗標題
const WPARAM   WPARAM_UPDATE_MACHINE_STATES                = 222;//更新機台狀態介面
const WPARAM   WPARAM_UPDATE_RIBBON_UI_TEXT                = 231;//專案更新Ribbon調機介面文字
const LPARAM   LPARAM_UPDATE_RIBBON_TUNE_GROUP_TEXT        = 101;//專案更新Ribbon調機介面文字
const LPARAM   LPARAM_UPDATE_RIBBON_DEFAULT_WND_GROUP_TEXT = 102;//專案更新Ribbon預設檢測框介面文字

const WPARAM   WPARAM_SHOW_PART_LIST_DOCK_PANE             = 301;//顯示元件視窗
const WPARAM   WPARAM_SHOW_RESULT_LIST_DOCK_PANE           = 302;//顯示結果視窗
const WPARAM   WPARAM_SHOW_PROJECT_MAP_DOCK_PANE           = 303;//顯示專案底圖視窗
const WPARAM   WPARAM_SHOW_PROJECT_LIBRARY_DOCK_PANE       = 304;//顯示專案資料庫視窗
const WPARAM   WPARAM_HIDE_GLOBAL_WND_FOR_INSPECTION       = 305;//隱藏全域視窗

const WPARAM   WPARAM_EXEC_FD_CONFIRM_WND                  = 401;//執行定位點確認視窗
const WPARAM   WPARAM_EXEC_BARCODE_CONFIRM_WND             = 501;//執行條碼確認視窗
const WPARAM   WPARAM_EXEC_BARCODE_HANDHELD_WND            = 502;//執行專案條碼輸入視窗
const WPARAM   WPARAM_EXEC_OPEN_PROJECT_BARCODE_WND        = 503;//執行開起專案條碼視窗
const WPARAM   WPARAM_EXEC_USER_LOGIN_WND                  = 504;//執行使用者登入視窗
const WPARAM   WPARAM_EXEC_SHOW_MESSAGE_WND                = 505;//執行顯示訊息視窗
const WPARAM   WPARAM_EXEC_COMPONENT_BARCODE_CONFIRM_WND   = 506;//執行零件條碼確認視窗

const WPARAM   WPARAM_MES_SET_SYSTEM_PARAM                 = 601;//MES設定系統參數
const WPARAM   WPARAM_MES_SET_PROJECT_PARAM                = 602;//MES設定專案參數
const WPARAM   WPARAM_MES_SET_USER_LOGIN_OUT               = 603;//MES設定使用者登入登出
const WPARAM   WPARAM_MES_CMD_OPEN_PROJECT                 = 604;//MES命令開啟專案
const WPARAM   WPARAM_MES_CMD_SHOW_MESSAGE                 = 605;//MES命令顯示訊息
const WPARAM   WPARAM_MES_CMD_ONLINE_RUN                   = 611;//MES命令線上檢測
const WPARAM   WPARAM_MES_CMD_ONLINE_BYPASS                = 612;//MES命令線上流片
const WPARAM   WPARAM_MES_CMD_ONLINE_STOP                  = 613;//MES命令線上停止
//------------------------------------------------------------------------------------------//
const UINT     MSG_EDIT_MAIN_VIEW_WND                      = WM_USER+101;//送至主顯示視窗
const WPARAM   WPARAM_INITIAL_UPDATE                       = 100;//初始化
const WPARAM   WPARAM_REDRAW_VIEW_WND                      = 101;//重繪視窗
const WPARAM   WPARAM_UPDATE_VIEW_PART_SELECTED            = 102;//更新顯示的選取到的零件
const WPARAM   WPARAM_SWITCH_FRAME_IMAGE                   = 103;//專案切換-畫面
const WPARAM   WPARAM_UPDATE_ALG_IMAGE                     = 104;//更新演算法圖像
const WPARAM   WPARAM_EXEC_WND_INSPECT                     = 105;//執行檢測框測試
const WPARAM   WPARAM_CALC_WND_COLOR                       = 106;//計算檢測框顏色
const WPARAM   WPARAM_EXTRACT_WND_COLOR_FILTER             = 107;//萃取檢測框抽色
const WPARAM   WPARAM_SHOW_WND_POSITION                    = 108;//顯示檢測框位置
const WPARAM   WPARAM_TOGGLE_ENCHANGE_IMAGE_MODE           = 109;//切換強化影像模式

const WPARAM   WPARAM_UPDATE_PROJECT_MAP                   = 111;//更新專案底圖
const WPARAM   WPARAM_SHOW_PROJECT_MAP_WND                 = 112;//顯示專案底圖視窗
const WPARAM   WPARAM_REDRAW_PROJECT_MAP                   = 113;//重繪專案底圖
const WPARAM   WPARAM_SET_DRAW_PROJECT_MODE                = 114;//設定繪製專案底圖模式
const WPARAM   WPARAM_BUILD_RAW_MODEL_UNI_FRAME_LIST       = 115;//建立原始模組通用影像列表
const WPARAM   WPARAM_UPDATE_PROJECT_TEST_MAP              = 116;//更新專案檢測底圖

const LPARAM   LPARAM_DRAW_PROJECT_MODE_NORMAL             = 0;//正常
const LPARAM   LPARAM_DRAW_PROJECT_MODE_INSPECTING         = 1;//檢測
//------------------------------------------------------------------------------------------//
const UINT     MSG_EDIT_PART_LIST_WND                      = WM_USER+102;//送至列表控制視窗-零件列表或者料號列表
const WPARAM   WPARAM_BUILD_PART_LIST                      = 101;//建立元件列表
const WPARAM   WPARAM_UPDATE_PART_LIST                     = 102;//更新元件列表
const WPARAM   WPARAM_CLEAR_PART_LIST                      = 103;//清除元件列表
const WPARAM   WPARAM_UPDATE_PART_SELECTED                 = 104;//顯示選取元件
const WPARAM   WPARAM_UPDATE_PART_STATES                   = 105;//顯示選取元件-自我呼叫

const LPARAM   LPARAM_BUILD_DOCK_LIST_ALL                  = 0xFFFFFFFF;//建立列表-全部
const LPARAM   LPARAM_BUILD_DOCK_LIST_PART                 = 0x00000001;//建立列表-零件
const LPARAM   LPARAM_BUILD_DOCK_LIST_MODEL                = 0x00000002;//建立列表-模組
const LPARAM   LPARAM_BUILD_DOCK_LIST_PART_NUMBER          = 0x00000004;//建立列表-料號
const LPARAM   LPARAM_BUILD_DOCK_LIST_RESULT               = 0x00010000;//建立列表-結果
const LPARAM   LPARAM_BUILD_DOCK_LIST_EDIT                 = LPARAM_BUILD_DOCK_LIST_PART+LPARAM_BUILD_DOCK_LIST_MODEL+LPARAM_BUILD_DOCK_LIST_PART_NUMBER;//建立列表-編輯
//------------------------------------------------------------------------------------------//
const UINT     MSG_EDIT_RESULT_LIST_WND                    = WM_USER+103;//送至結果列表控制視窗
const WPARAM   WPARAM_BUILD_RESULT_LIST                    = 101;//建立結果列表
const WPARAM   WPARAM_UPDATE_RESULT_LIST                   = 102;//更新結果列表
const WPARAM   WPARAM_CLEAR_RESULT_LIST                    = 103;//清除結果列表
const WPARAM   WPARAM_UPDATE_RESULT_SELECTED               = 104;//顯示選取結果
//------------------------------------------------------------------------------------------//
const UINT     MSG_EDIT_WND_PROPERTY_WND                   = WM_USER+105;//送至檢測框參數視窗
const WPARAM   WPARAM_BUILD_WND_SELECTED                   = 101;//建立檢測框選取
const WPARAM   WPARAM_UPDATE_WND_SELECTED                  = 102;//更新檢測框選取
const WPARAM   WPARAM_REBUILD_WND_PARAM_LIST               = 103;//重新建立檢測框參數列表
const WPARAM   WPARAM_EXEC_WND_ROI_ADD_AUTO                = 111;//執行小框自動建立
//------------------------------------------------------------------------------------------//
const UINT     MSG_EDIT_IMAGE_VIEW_WND                     = WM_USER+106;//送至編輯影像視窗
const WPARAM   WPARAM_UPDATE_IMAGE_MODEL_SELECTED          = 101;//建立模組選取
const WPARAM   WPARAM_UPDATE_IMAGE_WND_SELECTED            = 102;//更新檢測框選取
const WPARAM   WPARAM_UPDATE_IMAGE_MODEL_NO_PROCESS        = 103;//更新模組選取-不呼叫影像處理
const WPARAM   WPARAM_UPDATE_IMAGE_WND_SELECTED_NO_PROCESS = 104;//更新檢測框選取
const WPARAM   WPARAM_UPDATE_IMAGE_SWITCH_FRAME            = 105;//切換影像
const WPARAM   WPARAM_SHOW_IMAGE_PROCESS_PAGE              = 201;//切換視窗-顯示影像處理頁簽
const WPARAM   WPARAM_SHOW_IMAGE_OPENGL_3D_PAGE            = 202;//切換視窗-顯示影像3D頁簽
const WPARAM   WPARAM_SHOW_IMAGE_BLOB_PAGE                 = 203;//切換視窗-顯示影像Blob頁簽
//------------------------------------------------------------------------------------------//
const UINT     MSG_EDIT_IMAGE_PROCESS_WND                  = WM_USER+107;//送至編輯影像處理視窗
const WPARAM   WPARAM_UPDATE_WND_ALG                       = 101;//更新演算法參數
const WPARAM   WPARAM_UPDATE_ALG_PARAM                     = 102;//更新演算法參數
const WPARAM   WPARAM_UPDATE_ALG_RESULT                    = 103;//更新演算法結果
const WPARAM   WPARAM_UPDATE_ALG_COLOR_FILTER              = 104;//更新演算法抽色
const WPARAM   WPARAM_UPDATE_GATHER_COLOR_CHK              = 105;//更新是否抽色按鈕
const WPARAM   WPARAM_PATTERN_ADD                          = 111;//樣板增加
const WPARAM   WPARAM_PATTERN_TEXT                         = 112;//樣板文字
//------------------------------------------------------------------------------------------//
const UINT     MSG_EDIT_VIEW_3D_WND                        = WM_USER+108;//送至編輯顯示3D視窗
const WPARAM   WPARAM_UPDATE_3D_DATA                       = 101;//更新3D資料
const WPARAM   WPARAM_UPDATE_3D_WND_SELECTED               = 102;//更新選到的檢測框
//------------------------------------------------------------------------------------------//
const UINT     MSG_EDIT_VIEW_BLOB_WND                      = WM_USER+109;//送至編輯顯示Blob視窗
const WPARAM   WPARAM_UPDATE_BLOB_WND_SELECTED             = 102;//更新選到的檢測框
//------------------------------------------------------------------------------------------//
const UINT     MSG_RIBBON_BAR_WND                          = WM_USER+120;//送至Ribbon Bar Wnd
const WPARAM   WPARAM_PROJECT_COMBOX_BUILD                 = 201;//定位點-專案列表-建立
const WPARAM   WPARAM_PROJECT_COMBOX_SEL_CHANGE            = 202;//定位點-專案列表-選取切換
const WPARAM   WPARAM_PANEL_COMBOX_BUILD                   = 501;//定位點-整板列表-建立
const WPARAM   WPARAM_PANEL_COMBOX_SEL_CHANGE              = 502;//定位點-整板列表-選取切換
const WPARAM   WPARAM_BOARD_COMBOX_BUILD                   = 511;//定位點-單板列表-建立
const WPARAM   WPARAM_BOARD_COMBOX_SEL_CHANGE              = 512;//定位點-單板列表-選取切換
const WPARAM   WPARAM_COMPONENT_COMBOX_BUILD               = 521;//定位點-零件列表-建立
const WPARAM   WPARAM_COMPONENT_COMBOX_SEL_CHANGE          = 522;//定位點-零件列表-選取切換
const WPARAM   WPARAM_SHOW_DEBUG_CATEGORY                  = 901;//偵錯群組-顯示切換
const WPARAM   WPARAM_UPDATE_PCB_DIRECTION                 = 902;//專案更新Ribbon調機介面圖示-PCB方向
//------------------------------------------------------------------------------------------//
const UINT     MSG_EDIT_LIBRARY_WND                        = WM_USER+121;//送至資料庫列表
const WPARAM   WPARAM_UPDATE_MODEL_LIST_ICON               = 101;//更新資料庫模組列表圖示
const WPARAM   WPARAM_UPDATE_USE_PARTNUMBER_CHK            = 102;//更新資料庫使用料號模式
//------------------------------------------------------------------------------------------//
const UINT     MSG_TREE_CTRL                               = WM_USER+122;//樹狀控制項
const WPARAM   WPARAM_TREE_VERTICAL_SCROLL_END             = 101;//樹狀控制項垂直滾動結束
//------------------------------------------------------------------------------------------//
const UINT     MSG_LIST_CTRL                               = WM_USER+123;//列表控制項
const WPARAM   WPARAM_LIST_VERTICAL_SCROLL_END             = 101;//列表控制項垂直滾動結束
//------------------------------------------------------------------------------------------//
const UINT     MSG_FINGERPRINT_WND = WM_USER + 130;//送至指紋視窗
const WPARAM   WPARAM_FINGERPRINT_SHOW_STATE = 0;// 指紋執行狀態
const WPARAM   WPARAM_FINGERPRINT_SHOW_OPERATION_STRING = 100;// 指紋操作文字
const WPARAM   WPARAM_FINGERPRINT_SHOW_STATE_STRING		= 101;// 指紋執行狀態文字
const WPARAM   WPARAM_FINGERPRINT_SHOW_REJECT_STRING	= 102;// 指紋裝置拒絕文字

const LPARAM   LPARAM_FINGERPRINT_CONNECT_FAIL = 0;// 指紋連接失敗
const LPARAM   LPARAM_FINGERPRINT_CONNECTING = 1;// 指紋連接中
const LPARAM   LPARAM_FINGERPRINT_WAITING = 10;// 指紋執行等待中
const LPARAM   LPARAM_FINGERPRINT_FINISH = 11;// 指紋執行完成
const LPARAM   LPARAM_FINGERPRINT_EXIST = 20;// 指紋已存在
const LPARAM   LPARAM_FINGERPRINT_ERROR = 100;// 指紋執行錯誤
//------------------------------------------------------------------------------------------//
#endif//JET8000MSGDef