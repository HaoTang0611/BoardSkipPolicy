// AOIDataDefine.h: interface for the CAOIDataDefine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AOIDATADEFINE_H__57FD96E3_C17E_482F_B397_2B50A54A87CA__INCLUDED_)
#define AFX_AOIDATADEFINE_H__57FD96E3_C17E_482F_B397_2B50A54A87CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include "ParamUni.h"
#include "WndDefectItem.h"
//-------------------------------------------------------------------------------------//
class CAOIDataDefine  
{
private:
	//---------------------------------------------------------------------------------//	
	CString                    m_IDText;//編號文字	
	CString                    m_ToText;//至文字	
	CString                    m_FdText;//定位點文字
	CString                    m_AddText;//新增文字
	CString                    m_SetText;//設定文字
	CString                    m_GetText;//取得文字
	CString                    m_PadText;//焊盤文字
	CString                    m_WndText;//檢測框文字
	CString                    m_AveText;//平均文字	
	CString                    m_AllText;//全部文字	
	CString                    m_MaxText;//最大值文字
	CString                    m_MinText;//最小值文字		
	CString                    m_FromText;//來文字	
	CString                    m_AxisText;//軸文字		
	CString                    m_LineText;//線文字
	CString                    m_AreaText;//面積文字	
	CString                    m_LaneText;//軌道文字
	CString                    m_TimeText;//時間文字
	CString                    m_TestText;//檢測文字
	CString                    m_NameText;//名稱文字
	CString                    m_SkewText;//偏角文字	
	CString                    m_MarkText;//特徵文字	
	CString                    m_GainText;//增益文字	
	CString                    m_GrayText;//灰階文字	
	CString                    m_SizeText;//尺寸文字
	CString                    m_LandText;//特徵框文字
	CString                    m_RangeText;//範圍文字	
	CString                    m_ClassText;//類別文字	
	CString                    m_ClearText;//清除文字	
	CString                    m_RatioText;//比例文字		
	CString                    m_IndexText;//序號文字		
	CString                    m_ErrorText;//錯誤文字	
	CString                    m_PanelText;//整板文字
	CString                    m_BoardText;//單板文字	
	CString                    m_ModelText;//模組文字	
	CString                    m_GroupText;//群組文字	
	CString                    m_ScoreText;//分數文字
	CString                    m_ScaleText;//縮放文字
	CString                    m_YieldText;//良率文字	
	CString                    m_PixelText;//像素文字
	CString                    m_SizeXText;//X尺寸文字
	CString                    m_SizeYText;//Y尺寸文字	
	CString                    m_MethodText;//方法文字
	CString                    m_TowardText;//朝向文字	
	CString                    m_CloneText;//複製文字	
	CString                    m_CreateText;//創建文字	
	CString                    m_ModifyText;//修改文字
	CString                    m_DeleteText;//刪除文字	
	CString                    m_DefectText;//瑕疵文字	
	CString                    m_ResultText;//結果文字	
	CString                    m_UnsetText;//未設定文字
	CString                    m_AngleText;//角度文字			
	CString                    m_WidthText;//寬度文字			
	CString                    m_HeightText;//長度文字	
	CString                    m_VolumeText;//體積文字	
	CString                    m_OffsetText;//偏移文字	
	CString                    m_FinishText;//完成文字
	CString                    m_ProjectText;//專案文字
	CString                    m_PatternText;//樣板文字
	CString                    m_DefaultText;//預設文字
	CString                    m_ThroughText;//貫穿文字	
	CString                    m_BarcodeText;//條碼文字	
	CString                    m_WarningText;//警告文字			
	CString                    m_GroupIDText;//群組編號文字			
	CString                    m_BypassedText;//不檢測文字
	CString                    m_DistrictText;//分區文字
	CString                    m_DistanceText;//距離文字	
	CString                    m_VelocityText;//速度文字
	CString                    m_RelativeText;//相對文字	
	CString                    m_ContinueText;//連續文字	
	CString                    m_ContrastText;//對比文字
	CString                    m_ThicknessText;//厚度文字	
	CString                    m_AlgorithmText;//演算法文字		
	CString                    m_ComponentText;//零件文字		
	CString                    m_VerticalText;//垂直文字
	CString                    m_LockScreenText;//鎖住螢幕文字
	CString                    m_HorizontalText;//水平文字
	CString                    m_PartNumberText;//料號文字		
	CString                    m_NozzleNameText;//吸嘴文字		
	CString                    m_UndefinedText;//未定義文字	
	CString                    m_AccelerationText;//加速度文字
	CString                    m_BarcodeDeviceText;//條碼機文字	
	CString                    m_SaftyBypassText;//忽略安全檢知文字	
	CString                    m_WaitForCCSText;//等待中控判定文字	
	CString                    m_WaitForRepairText;//等待維修站文字		
	//---------------------------------------------------------------------------------//	
	CString                    m_OKText;//確定文字
	CString                    m_CancelText;//取消文字
	//---------------------------------------------------------------------------------//	
	CString                    m_LaneText_A;//軌道文字-A
	CString                    m_LaneText_B;//軌道文字-B
	//---------------------------------------------------------------------------------//	
	CString                    m_DecodeText;//解碼文字
	//---------------------------------------------------------------------------------//	
	CString                    m_LevelText;//等級文字
	//---------------------------------------------------------------------------------//	
	CString                    m_EnableText;//啟用文字
	CString                    m_DisabeText;//關閉文字
	//---------------------------------------------------------------------------------//
	CString                    m_PositiveText;//正向文字
	CString                    m_NegativeText;//反向文字
	//---------------------------------------------------------------------------------//	
	CString                    m_DistrictText_A;//分區文字-A
	CString                    m_DistrictText_B;//分區文字-B
	//---------------------------------------------------------------------------------//
	CString                    m_WndDefectItem_Disable;//檢測框瑕疵模式-關閉
	CString                    m_WndDefectItem_Enable;//檢測框瑕疵模式-啟用
	CString                    m_WndDefectItem_NoShow;//檢測框瑕疵模式-不顯示
	//---------------------------------------------------------------------------------//	
	CString                    m_MultiLaneText_1;//多軌道文字-單軌
	CString                    m_MultiLaneText_2;//多軌道文字-雙軌
	//---------------------------------------------------------------------------------//	
	CString                    m_FuncExecText_Off;//函式執行文字-關閉
	CString                    m_FuncExecText_Auto;//函式執行文字-自動
	CString                    m_FuncExecText_Ask;//函式執行文字-詢問
	//---------------------------------------------------------------------------------//	
	CString                    m_LaneWorkText_Disable;//軌道運轉文字-關閉
	CString                    m_LaneWorkText_Run;//軌道運轉文字-正常
	CString                    m_LaneWorkText_Bypass;//軌道運轉文字-輸送帶
	//---------------------------------------------------------------------------------//
	CString                    m_ConnectedBufferType_Fixed;//軌道機-固定式
	CString                    m_ConnectedBufferType_Movable;//軌道機-移動式
	//---------------------------------------------------------------------------------//
	CString                    m_HASI_AOIStage_Pre; //爐前
	CString                    m_HASI_AOIStage_Post;//爐後
	CString                    m_HASI_State_mode_STOP;
	CString                    m_HASI_State_mode_AC;
	CString                    m_HASI_State_mode_SC;
	CString                    m_HASI_State_mode_SCAC;
	CString                    m_HASI_State_mode_RUN;
	CString                    m_HASI_State_mode_TEST;
	//---------------------------------------------------------------------------------//
	CString                    m_GrabText;//取像文字
	CString                    m_CountText;//數量文字
	CString                    m_CalculateText;//計算文字
	CString                    m_VersionCodeText;//版本號文字
	//---------------------------------------------------------------------------------//	
	CString                    m_ColorText_Red;//顏色文字-紅色
	CString                    m_ColorText_Green;//顏色文字-綠色
	CString                    m_ColorText_Blue;//顏色文字-藍色
	CString                    m_ColorText_Black;//顏色文字-黑色	
	CString                    m_ColorText_Gray;//顏色文字-灰色
	CString                    m_ColorText_White;//顏色文字-白色	
	CString                    m_ColorText_Color;//顏色文字-彩色	
	//---------------------------------------------------------------------------------//	
	CString                    m_UserLevelText_SignOut;//使用者權限-登出
	CString                    m_UserLevelText_Operator;//使用者權限-作業員
	CString                    m_UserLevelText_Engineer;//使用者權限-工程師
	CString                    m_UserLevelText_Supervisor;//使用者權限-管理者
	CString                    m_UserLevelText_JETFAE;//使用者權限-JET廠商
	CString                    m_UserLevelText_JETSENIOR;//使用者權限-JET廠商-資深
	//---------------------------------------------------------------------------------//		
	CString                    m_UserLoginText_Disable;//使用者登入模式-關閉
	CString                    m_UserLoginText_Operator;//使用者登入模式-作業員
	CString                    m_UserLoginText_Engineer;//使用者登入模式-工程師	
	//---------------------------------------------------------------------------------//		
	CString                    m_UserLoginOptionsText_Password;//使用者登入選項-密碼
	CString                    m_UserLoginOptionsText_Fingerprint;//使用者登入選項-指紋
	CString                    m_UserLoginOptionsText_FingerprintOnly;//使用者登入選項-只能指紋
	//---------------------------------------------------------------------------------//		
	CString                    m_OpenProjectText_File;//開啟專案模式-檔案
	CString                    m_OpenProjectText_Code;//開啟專案模式-編碼
	//---------------------------------------------------------------------------------//
	CString                    m_VerifyProjectText_Disable;//驗證專案模式-關閉
	CString                    m_VerifyProjectText_Filename;//驗證專案模式-檔名
	//---------------------------------------------------------------------------------//	
	CString                    m_OnlineInputTiming_Disalbe;//線上輸入時機-關閉
	CString                    m_OnlineInputTiming_BeforeSignIn;//線上輸入時機-登入前
	//---------------------------------------------------------------------------------//	
	CString                    m_AOICustomerIDText_JET_TWN;//客戶編號-捷智
	CString                    m_AOICustomerIDText_PegaTron_TWN;//和碩-台灣
	CString                    m_AOICustomerIDText_Kinpo_YueYang;//泰金寶-岳陽
	CString                    m_AOICustomerIDText_Foxconn_LongHua;//富士康-龍華
	//---------------------------------------------------------------------------------//
	CString                    m_OfflineVersionText_Normal;//軟體版本模式-一般
	CString                    m_OfflineVersionText_HostTuning;//軟體版本模式-本機調適
	CString                    m_OfflineVersionText_RemoteTuning;//軟體版本模式-遠端調適
	//---------------------------------------------------------------------------------//	
	CString                    m_OnlineOpenProjectText_Disable;//線上開啟專案-關閉
	CString                    m_OnlineOpenProjectText_BarcodeDevice;//線上開啟專案-外接條碼
	CString                    m_OnlineOpenProjectText_BarcodeHandHeld;//線上開啟專案-手持條碼
	CString                    m_OnlineOpenProjectText_BarcodeCamera;//線上開啟專案-相機條碼
	//---------------------------------------------------------------------------------//
	CString                    m_ProjectLinkServerText_Disable;//專案連線伺服器-關閉
	CString                    m_ProjectLinkServerText_EnableAll;//專案連線伺服器-啟用
	CString                    m_ProjectLinkServerText_ProjectOnly;//專案連線伺服器-僅專案
	CString                    m_ProjectLinkServerText_EnableAllAsk;//專案連線伺服器-詢問
	CString                    m_ProjectLinkServerText_ProjectOnlyAsk;//專案連線伺服器-詢問專案
	//---------------------------------------------------------------------------------//
	CString                    m_XBoardMappingFileText_Disable;//報廢板映射檔案-關閉
	CString                    m_XBoardMappingFileText_MES_Comm;//報廢板映射檔案-MES通訊
	//---------------------------------------------------------------------------------//
	CString                    m_XBoardMappingFileFlowText_Default;//報廢板映射檔案-預設
	CString                    m_XBoardMappingFileFlowText_After_Barcode;//報廢板映射檔案-在Barcode檢查之後
	//---------------------------------------------------------------------------------//
	CString                    m_XBoardMappingFileMESCheckText_False;//報廢板MES Comm 條件檢測-不檢測
	CString                    m_XBoardMappingFileMESCheckText_True; //報廢板MES Comm 條件檢測-檢測
	//---------------------------------------------------------------------------------//
	CString                    m_PCBOutText_Normal;//預設
	CString                    m_PCBOutText_SideOut;//兩段式, 機台內側, 再出板
	CString                    m_PCBOutText_WithIn;//出板帶進板
	CString                    m_PCBOutText_LaneAuto;//軌道自行運作
	CString                    m_PCBOutText_OkOutNgSide;//OK出板, NG停內側
	//---------------------------------------------------------------------------------//	
	CString                    m_PCBOutDirText_Forward;//正向
	CString                    m_PCBOutDirText_Backward;//反向
	CString                    m_PCBOutDirText_BackwardOut;//反向出板	
	//---------------------------------------------------------------------------------//
	CString                    m_PanelSideText_Top;//板面-上面
	CString                    m_PanelSideText_Bottom;//板面-下面
	CString                    m_PanelSideText_Hybrid;//板面-混合
	//---------------------------------------------------------------------------------//
	CString                    m_BoardSideText_Top;//板面-上面
	CString                    m_BoardSideText_Bottom;//板面-下面
	//---------------------------------------------------------------------------------//
	CString                    m_BarcodeDecoder_EVS;//Open-Evision
	CString                    m_BarcodeDecoder_DTK;//DTK
	CString                    m_BarcodeDecoder_HON;//Honeywell-SwiftDecoder
	//---------------------------------------------------------------------------------//
	CString                    m_BarcodeSpread_Off;//關閉
	CString                    m_BarcodeSpread_Local;//局部
	CString                    m_BarcodeSpread_All;//全部
	//---------------------------------------------------------------------------------//
	CString                    m_BarcodeBelong_None;//未定義
	CString                    m_BarcodeBelong_Project;//專案
	CString                    m_BarcodeBelong_Panel;//整板
	CString                    m_BarcodeBelong_Board;//單板
	CString                    m_BarcodeBelong_Tray;//載具
	CString                    m_BarcodeBelong_Cover;//蓋板
	//---------------------------------------------------------------------------------//
	CString                    m_SaveTestMapText_Disable;//不儲存	
	CString                    m_SaveTestMapText_Prog;//儲存專案-全圖
	CString                    m_SaveTestMapText_Panel;//儲存整板
	CString                    m_SaveTestMapText_ProgPanel;//儲存全圖+整板
	CString                    m_SaveTestMapText_Board;//儲存單板
	CString                    m_SaveTestMapText_ProgBoard;//儲存全圖+單板
	//---------------------------------------------------------------------------------//	
	CString                    m_OfflineImageText_Fov;//視野範疇
	CString                    m_OfflineImageText_Part;//部分範疇
	//---------------------------------------------------------------------------------//	
	CString                    m_SaveTestImageText_Disable;//不儲存	
	CString                    m_SaveTestImageText_Defect;//瑕疵儲存
	CString                    m_SaveTestImageText_EveryOne;//總是儲存
	//---------------------------------------------------------------------------------//
	CString                    m_SaveTestDataText_Disable;//不儲存	
	CString                    m_SaveTestDataText_Enable;//總是儲存	
	CString                    m_SaveTestDataText_Defect;//瑕疵儲存
	//---------------------------------------------------------------------------------//
	CString                    m_FdNGHandleText_None;//無作動
	CString                    m_FdNGHandleText_Pass;//直通
	CString                    m_FdNGHandleText_Stop;//停機
	CString                    m_FdNGHandleText_XBoard;//報廢板	
	//---------------------------------------------------------------------------------//	
	CString                    m_BoardFdGrabText_AfterPanel;//整板號
	CString                    m_BoardFdGrabText_Inspecting;//檢測中
	//---------------------------------------------------------------------------------//	
	CString                    m_DefectHandleText_Pass;//通過, 燒機模式
	CString                    m_DefectHandleText_Stop;//停機警報
	CString                    m_DefectHandleText_Next;//停於下一站
	CString                    m_DefectHandleText_Repair;//等人員判定
	CString                    m_DefectHandleText_ControlCenter;//中控中心
	//---------------------------------------------------------------------------------//	
	CString                    m_OnlineState_InspectionStop;//停止檢測
	CString                    m_OnlineState_PCBReady;//PCB就緒
	CString                    m_OnlineState_InputBarcode;//輸入條碼
	CString                    m_OnlineState_ProjectMap;//專案底圖
	CString                    m_OnlineState_ProjectMark;//專案標記
	CString                    m_OnlineState_ProjectOpenCode;//專案開啟條碼
	CString                    m_OnlineState_ProjectReload;//專案重載	
	CString                    m_OnlineState_ProjectReloadServer;//專案重載伺服器
	CString                    m_OnlineState_ProjectSwitchByTurn;//專案切換-輪流
	CString                    m_OnlineState_ProjectSwitchByTurnOneCycleReset;//專案切換-重設輪流一次檢測
	CString                    m_OnlineState_InspectionStart;//開始檢測
	CString                    m_OnlineState_InspectionWaittng;//等待檢測
	CString                    m_OnlineState_InspectFDPanel;//定位整板
	CString                    m_OnlineState_InspectFDBoard;//定位單板
	CString                    m_OnlineState_InspectBarcode;//檢測條碼
	CString                    m_OnlineState_InspectProject;//檢測專案
	CString                    m_OnlineState_StatisticProject;//統計專案
	CString                    m_OnlineState_InspectionFinish;//檢測結束
	CString                    m_OnlineState_WaitForLast;//等待上一站訊號
	CString                    m_OnlineState_WaitForNext;//等待下一站訊號
	CString                    m_OnlineState_WaitForPCBRemoved;//等待板子移走
	CString                    m_OnlineState_WaitForRepairVerify;//等待維修站確認
	CString                    m_OnlineState_PCBInStart;//進板開始
	CString                    m_OnlineState_PCBInChecking;//進板確認
	CString                    m_OnlineState_PCBInFinish;//進板結束
	CString                    m_OnlineState_PCBOutStart;//出板開始
	CString                    m_OnlineState_PCBOutChecking;//出板確認
	CString                    m_OnlineState_PCBOutFinish;//出板結束	
	CString                    m_OnlineState_PCBOutInsideStart;//停側邊開始
	CString                    m_OnlineState_PCBOutInsideChecking;//停側邊確認
	CString                    m_OnlineState_PCBOutInsideFinish;//停側邊結束
	CString                    m_OnlineState_PCBBackStart;//退板開始
	CString                    m_OnlineState_PCBBackChecking;//退板確認
	CString                    m_OnlineState_PCBBackFinish;//退板結束
	CString                    m_OnlineState_PCBBackOutStart;//退出板開始
	CString                    m_OnlineState_PCBBackOutChecking;//退出板確認
	CString                    m_OnlineState_PCBBackOutFinish;//退出板結束
	CString                    m_OnlineState_PCBOutInStart;//出板帶進板開始
	CString                    m_OnlineState_PCBOutInChecking;//出板帶進板確認
	CString                    m_OnlineState_PCBOutInFinish;//出板帶進板結束
	CString                    m_OnlineState_PCBAutoRunStart;//自動進出板開始
	CString                    m_OnlineState_PCBAutoRunChecking;//自動進出板確認
	CString                    m_OnlineState_PCBAutoRunFinish;//自動進出板完成	
	CString                    m_OnlineState_PCBDualRunStart;//雙軌進出板開始
	CString                    m_OnlineState_PCBDualRunChecking;//雙軌進出板確認
	CString                    m_OnlineState_PCBDualRunFinish;//雙軌進出板完成		
	CString                    m_OnlineState_PCBInspectionPause;//雙軌-單軌暫停		
	CString                    m_OnlineState_AutoCalibration_XYZ_Home;//自動校正-XYZ歸零
	CString                    m_OnlineState_AutoCalibration_2D_Current;//自動校正-2D電流
	CString                    m_OnlineState_AutoCalibration_3D_Current;//自動校正-3D電流
	CString                    m_OnlineState_AutoCalibration_3D_ZeroPlane;//自動校正-3D相平面
	CString                    m_OnlineState_AutoCalibration_3D_FactorFactor;//自動校正-3D高度比例
	CString                    m_OnlineState_AppOpen;//軟體開啟
	CString                    m_OnlineState_AppClose;//軟體關閉
	//---------------------------------------------------------------------------------//		
	CString                    m_MesEqpCtrlStateText_None;//MES機台控制模式-關閉
	CString                    m_MesEqpCtrlStateText_Offline;//MES機台控制模式-離線
	CString                    m_MesEqpCtrlStateText_Local;//MES機台控制模式-本地上線
	CString                    m_MesEqpCtrlStateText_Remote;//MES機台控制模式-遠端上線
	//---------------------------------------------------------------------------------//	
	CString                    m_FieldPathModeText_Hor;//水平優先
	CString                    m_FieldPathModeText_Ver;//垂直優先
	CString                    m_FieldPathModeText_User;//手動配置
	//---------------------------------------------------------------------------------//		
	CString                    m_FieldDivisionModeText_MassArea;//最大面積
	CString                    m_FieldDivisionModeText_Diagonal;//對角線
	CString                    m_FieldDivisionModeText_Horizontal;//水平線
	CString                    m_FieldDivisionModeText_Vertical;//垂直線
	//---------------------------------------------------------------------------------//		
	CString                    m_FieldBuildModeText_Matrix;//等間距
	CString                    m_FieldBuildModeText_Random_Panel;//任意位置-整板
	CString                    m_FieldBuildModeText_Random_Board;//任意位置-單板
	CString                    m_FieldBuildModeText_Random_Project;//任意位置-專案	
	//---------------------------------------------------------------------------------//		
	CString                    m_BarcodeInputText_Disabled;//關閉
	CString                    m_BarcodeInputText_Device;//外接條碼機
	CString                    m_BarcodeInputText_Handheld;//手持條碼機	
	//---------------------------------------------------------------------------------//		
	CString                    m_BarcodeNGHandleText_Pass;//條碼失敗處理-直通
	CString                    m_BarcodeNGHandleText_Alarm;//條碼失敗處理-警報
	CString                    m_BarcodeNGHandleText_Input;//條碼失敗處理-輸入
	//---------------------------------------------------------------------------------//		
	CString                    m_MultiProjectTestOrderText_ByMark;//多專案檢測次序-專案特徵
	CString                    m_MultiProjectTestOrderText_InTurn;//多專案檢測次序-輪流切換
	CString                    m_MultiProjectTestOrderText_OneCycleAB;//多專案檢測次序-一次檢測切換, A->B
	CString                    m_MultiProjectTestOrderText_OneCycleBA;//多專案檢測次序-一次檢測切換, B->A
	//---------------------------------------------------------------------------------//		
	CString                    m_BarcodeCameraGrabModeText_AfterFd;//相機條碼讀取模式-定位點後
	CString                    m_BarcodeCameraGrabModeText_Inspecting;//相機條碼讀取模式-檢測中
	//---------------------------------------------------------------------------------//		
	CString                    m_BarcodeDeviceGrabModeText_BeforePCBIn;//條碼機讀取模式-進板前
	CString                    m_BarcodeDeviceGrabModeText_WhilePCBIn;//條碼機讀取模式-進板中
	CString                    m_BarcodeDeviceGrabModeText_AfterPCBIn;//條碼機讀取模式-進板後
	CString                    m_BarcodeDeviceGrabModeText_BeforeInspect;//條碼機讀取模式-檢測前
	//---------------------------------------------------------------------------------//		
	CString                    m_BarcodeHandHeldReadModeText_Manual;//手持條碼機讀取模式-手動
	CString                    m_BarcodeHandHeldReadModeText_Project;//手持條碼機讀取模式-依專案
	CString                    m_BarcodeHandHeldReadModeText_Panel;//手持條碼機讀取模式-依整板
	CString                    m_BarcodeHandHeldReadModeText_Board;//手持條碼機讀取模式-依單板
	//---------------------------------------------------------------------------------//		
	CString                    m_BarcodeAutoExpandModeText_Disable;//條碼自動擴展模式-關閉
	CString                    m_BarcodeAutoExpandModeText_Increment;//條碼自動擴展模式-自動疊加
	CString                    m_BarcodeAutoExpandModeText_AddChar01;//條碼自動擴展模式-增加1字元
	CString                    m_BarcodeAutoExpandModeText_AddChar02;//條碼自動擴展模式-增加2字元
	CString                    m_BarcodeAutoExpandModeText_Replace01;//條碼自動擴展模式-取代1字元
	CString                    m_BarcodeAutoExpandModeText_Replace02;//條碼自動擴展模式-取代2字元
	CString                    m_BarcodeAutoExpandModeText_Inc_Base36;//條碼自動擴展模式-自動疊加-36進位
	//---------------------------------------------------------------------------------//		
	CString                    m_BarcodeDirectionModeText_Auto;//條碼方向-自動
	CString                    m_BarcodeDirectionModeText_Hor;//條碼方向-水平
	CString                    m_BarcodeDirectionModeText_Ver;//條碼方向-垂直
	CString                    m_BarcodeDirectionModeText_All;//條碼方向-全部
	//---------------------------------------------------------------------------------//	
	CString                    m_AutoSwitchWnd3DFrame_Disable;//自動切換檢測框3D畫面-關閉
	CString                    m_AutoSwitchWnd3DFrame_Enable;//自動切換檢測框3D畫面-啟用
	CString                    m_AutoSwitchWnd3DFrame_BySize;//自動切換檢測框3D畫面-依據尺寸
	CString                    m_AutoSwitchWnd3DFrame_ByType;//自動切換檢測框3D畫面-依據樣式
	CString                    m_AutoSwitchWnd3DFrame_ByGroupChange;//自動切換檢測框3D畫面-依據群組切換
	//---------------------------------------------------------------------------------//	
	CString                    m_AlarmLockText_None;//警報鎖住-None
	CString                    m_AlarmLockText_AOI;//警報鎖住-AOI
	CString                    m_AlarmLockText_ARS;//警報鎖住-ARS
	//---------------------------------------------------------------------------------//	
	CString                    m_DefectFromText_None;//瑕疵來源-None
	CString                    m_DefectFromText_AOI;//瑕疵來源-AOI
	CString                    m_DefectFromText_ARS;//瑕疵來源-ARS
	//---------------------------------------------------------------------------------//		
	CString                    m_Top10ScopeText_Model;//前十大範疇-模組
	CString                    m_Top10ScopeText_PartNumber;//前十大範疇-料號
	CString                    m_Top10ScopeText_Component;//前十大範疇-零件
	//---------------------------------------------------------------------------------//	
	CString                    m_YieldingScopeText_Test;//良率範疇-檢測
	CString                    m_YieldingScopeText_Panel;//良率範疇-整板
	CString                    m_YieldingScopeText_Board;//良率範疇-單板
	CString                    m_YieldingScopeText_Component;//良率範疇-零件
	//---------------------------------------------------------------------------------//
	CString                    m_DefectParamFromText_Disable;//瑕疵參數來源-關閉
	CString                    m_DefectParamFromText_Project;//瑕疵參數來源-專案
	CString                    m_DefectParamFromText_Component;//瑕疵參數來源-零件
	//---------------------------------------------------------------------------------//
	CString                    m_CpkFromText_OffsetX;//Cpk來源-偏移量-X
	CString                    m_CpkFromText_OffsetY;//Cpk來源-偏移量-Y
	CString                    m_CpkFromText_SkewAngle;//Cpk來源-偏移角度
	//---------------------------------------------------------------------------------//
	CString                    m_MultiLanguageText_English;//英文版
	CString                    m_MultiLanguageText_ChinTrad;//中文繁體版
	CString                    m_MultiLanguageText_ChinSimp;//中文簡體版
	CString                    m_MultiLanguageText_Local;//當地版
	//---------------------------------------------------------------------------------//
	CString                    m_AlgText_BrightRatio;//演算法-亮度比例
	CString                    m_AlgText_OuterShort;//演算法-外接短路
	CString                    m_AlgText_BlobCount;//演算法-區塊數量
	CString                    m_AlgText_BodyTilt;//演算法-本體傾斜
	CString                    m_AlgText_BarcodeRecognize;//演算法-條碼辨識
	CString                    m_AlgText_ObjectMeasure;//演算法-物件量測
	CString                    m_AlgText_WidthRatio;//演算法-錫寬度
	CString                    m_AlgText_ColorCode;//演算法-色碼檢測
	CString                    m_AlgText_ModelMatch;//演算法-模板匹配
	CString                    m_AlgText_ImageMatch;//演算法-影像匹配
	CString                    m_AlgText_CharVerify;//演算法-文字驗證
	CString                    m_AlgText_FdMatch;//演算法-定位點匹配	
	CString                    m_AlgText_EdgeSearch;//演算法-邊緣搜尋
	CString                    m_AlgText_ShapeVerify;//演算法-外形驗證
	CString                    m_AlgText_AngleMeasure;//演算法-角度量測
	CString                    m_AlgText_PixelCompare;//演算法-像素比較	
	CString                    m_AlgText_SolderWetting;//演算法-焊接檢測
	CString                    m_AlgText_MeasureBlackGlue;//演算法-量測黑膠
	CString                    m_AlgText_MeasureFluxArea;//演算法-量測Flux面積
	CString                    m_AlgText_MeasureCpuPin;//演算法-量測CPU接腳
	CString                    m_AlgText_MeasureSIP;//演算法-量測SIP
	CString                    m_AlgText_MeasureConnector;//演算法-量測Connector-廣達Amphenol
	CString                    m_AlgText_GroupCompare;//演算法-群組比較
	
	CString                    m_AlgText_Resin;//演算法
	CString                    m_AlgText_WireWidth;//演算法
	//---------------------------------------------------------------------------------//	
	CString                    m_AlgCalcUnitMode_Abs;//計算單位模式-絕對數值 
	CString                    m_AlgCalcUnitMode_Diff;//計算單位模式-相對差距
	CString                    m_AlgCalcUnitMode_Ratio;//計算單位模式-比例數值
	//---------------------------------------------------------------------------------//		
	CString                    m_AlgBrightAverageMode_Full;//平均模式-全平均
	CString                    m_AlgBrightAverageMode_Partial;//平均模式-部分平均	
	//---------------------------------------------------------------------------------//	
	CString                    m_AlgMatchDockMode_Disable;//匹配靠邊模式-關閉
	CString                    m_AlgMatchDockMode_ToTip;//匹配靠邊模式-靠前端
	CString                    m_AlgMatchDockMode_Shoulder;//匹配靠邊模式-靠根部
	//---------------------------------------------------------------------------------//
	CString                    m_AlgObjectSizeCalcMode_Boundary;//物件尺寸計算模式-邊界
	CString                    m_AlgObjectSizeCalcMode_Average;//物件尺寸計算模式-平均
	CString                    m_AlgObjectSizeCalcMode_AveRect;//物件尺寸計算模式-平均+邊界		
	CString                    m_AlgObjectSizeCalcMode_BlurRect;//物件尺寸計算模式-平滑矩形
	//---------------------------------------------------------------------------------//	
	CString                    m_AlgObjectHeightAverageMode_Full;//物件高度平均模式-全平均
	CString                    m_AlgObjectHeightAverageMode_Partial;//物件高度平均模式-部分平均	
	//---------------------------------------------------------------------------------//	
	CString                    m_AlgAngleMeasureAngleMode_Skew;//角度量測角度模式-偏移角度
	CString                    m_AlgAngleMeasureAngleMode_Tilt;//角度量測角度模式-傾斜角度
	//---------------------------------------------------------------------------------//	
	CString                    m_AlgAngleMeasureBaseLineMode_Calc;//角度量測基準線模式-計算 
	CString                    m_AlgAngleMeasureBaseLineMode_Hor;//角度量測基準線模式-水平
	CString                    m_AlgAngleMeasureBaseLineMode_Ver;//角度量測基準線模式-垂直
	//---------------------------------------------------------------------------------//	
	CString                    m_ResultText_None;//結果-尚未檢測
	CString                    m_ResultText_OK;//結果-良品
	CString                    m_ResultText_NG;//結果-瑕疵
	CString                    m_ResultText_Skip;//結果-不檢測
	CString                    m_ResultText_Bypass;//結果-不檢測
	CString                    m_ResultText_Exception;//結果-異常
	//---------------------------------------------------------------------------------//		
	CString                    m_PartGroupText_Colinearity;//共線性
	CString                    m_PartGroupText_ColinearityToLine;//共線性-對線
	CString                    m_PartGroupText_DistPartToPart;//距離-零件對零件
	CString                    m_PartGroupText_DistPartNeighbor;//距離-相鄰零件
	CString                    m_PartGroupText_DistPartToGroup;//距離-零件對群組中心
	CString                    m_PartGroupText_DistGroupToPart;//距離-群組零件對零件	
	CString                    m_PartGroupText_DistGroupCoordMap;//距離-群組座標轉換
	//---------------------------------------------------------------------------------//		
	CString                    m_ImageSrcText_Gray;//影像來源-灰階
	CString                    m_ImageSrcText_Color;//影像來源-彩色
	CString                    m_ImageSrcText_Red;//影像來源-紅色
	CString                    m_ImageSrcText_Green;//影像來源-綠色
	CString                    m_ImageSrcText_Blue;//影像來源-藍色
	CString                    m_ImageSrcText_Lightness;//影像來源-亮色
	CString                    m_ImageSrcText_Darkness;//影像來源-暗色
	CString                    m_ImageSrcText_Saturation;//影像來源-飽和
	CString                    m_ImageSrcText_Synthesis;//影像來源-合成
	CString                    m_ImageSrcText_RedRatio;//影像來源-紅色
	CString                    m_ImageSrcText_GreenRatio;//影像來源-綠色
	CString                    m_ImageSrcText_BlueRatio;//影像來源-藍色
	CString                    m_ImageSrcText_MaxGrnBlu;//影像來源-最亮綠藍色
	//---------------------------------------------------------------------------------//
	CString                    m_MaskFuncText_Calc;//遮罩圖形-計算
	CString                    m_MaskFuncText_Erase;//遮罩圖形-忽略
	//---------------------------------------------------------------------------------//	
	CString                    m_BinaryText_Disable;//二值化-關閉
	CString                    m_BinaryText_ColorFilter;//二值化-彩色過濾
	CString                    m_BinaryText_FixedTh;//二值化-固定閥值
	CString                    m_BinaryText_DynamicTh;//二值化-動態閥值
	CString                    m_BinaryText_RelativeTh;//二值化-相對閥值	
	CString                    m_BinaryText_AdaptiveTh;//二值化-適應閥值			
	//---------------------------------------------------------------------------------//		
	CString                    m_EdgeEnhanceText_Disable;//邊緣強化-關閉
	CString                    m_EdgeEnhanceText_Sobel;//邊緣強化-Sobel
	CString                    m_EdgeEnhanceText_DarkTop;//邊緣強化-上黑
	CString                    m_EdgeEnhanceText_DarkLeft;//邊緣強化-左黑
	CString                    m_EdgeEnhanceText_DarkBot;//邊緣強化-下黑
	CString                    m_EdgeEnhanceText_DarkRight;//邊緣強化-右黑
	//---------------------------------------------------------------------------------//	
	CString                    m_NoiseFilterText_Diable;//雜訊過濾-關閉	
	CString                    m_NoiseFilterText_Level;//雜訊過濾-分等濾波	
	CString                    m_NoiseFilterText_Smooth;//雜訊過濾-平均濾波	
	CString                    m_NoiseFilterText_Median;//雜訊過濾-中值濾波		
	CString                    m_NoiseFilterText_Median2;//雜訊過濾-中值濾波2
	CString                    m_NoiseFilterText_PyramidMedian;//雜訊過濾-金字塔中值濾波
	CString                    m_NoiseFilterText_ContentAware;//雜訊過濾-ContentAware濾波
	CString                    m_NoiseFilterText_Fast_Median;//雜訊過濾-AVX Median Filter
	CString                    m_NoiseFilterText_Fast_Average;//雜訊過濾-AVX Average Filter
	CString                    m_NoiseFilterText_Open;//雜訊過濾-開運算	
	CString                    m_NoiseFilterText_Close;//雜訊過濾-閉運算	
	CString                    m_NoiseFilterText_Erosion;//雜訊過濾-侵蝕
	CString                    m_NoiseFilterText_Dilation;//雜訊過濾-膨脹
	CString                    m_NoiseFilterText_Gradient;//雜訊過濾-Gradient
	//---------------------------------------------------------------------------------//		
	CString                    m_AlgBrightLineMode_Bright;//貫穿線模式-抓亮的
	CString                    m_AlgBrightLineMode_Dark;//貫穿線模式-抓暗的
	//---------------------------------------------------------------------------------//	
	CString                    m_AlgOuterShortExtendText_None;//外接短路延伸文字-無
	CString                    m_AlgOuterShortExtendText_Left;//外接短路延伸文字-單左
	CString                    m_AlgOuterShortExtendText_Right;//外接短路延伸文字-單右
	CString                    m_AlgOuterShortExtendText_Both;//外接短路延伸文字-雙側
	//---------------------------------------------------------------------------------//
	CString                    m_AlgDirText_Hor;//水平貫穿
	CString                    m_AlgDirText_Ver;//垂直貫穿
	//---------------------------------------------------------------------------------//	
	CString                    m_AlgBarcodeStepText_None;//無意義
	CString                    m_AlgBarcodeStepText_Scale;//縮放
	CString                    m_AlgBarcodeStepText_GainOffset;//增益與偏移
	CString                    m_AlgBarcodeStepText_Smooth;//平均濾波
	CString                    m_AlgBarcodeStepText_Open;//開運算
	CString                    m_AlgBarcodeStepText_Close;//閉運算
	CString                    m_AlgBarcodeStepText_Median;//中值濾波
	CString                    m_AlgBarcodeStepText_Invert;//反相處理
	CString                    m_AlgBarcodeStepText_Flip;//翻轉或鏡射
	CString                    m_AlgBarcodeStepText_Fill;//外圈填滿
	CString                    m_AlgBarcodeStepText_Erode;//侵蝕
	CString                    m_AlgBarcodeStepText_Dilate;//膨脹
	CString                    m_AlgBarcodeStepText_Fill2D;//外圈填滿-2D
	CString                    m_AlgBarcodeStepText_Sharp;//銳利化
	CString                    m_AlgBarcodeStepText_Range;//灰階範圍	
	//---------------------------------------------------------------------------------//	
	CString                    m_AlgFdMatchText_Model;//定位點-模板匹配
	CString                    m_AlgFdMatchText_Image;//定位點-影像搜尋
	//---------------------------------------------------------------------------------//		
	CString                    m_AlgGroupCompareDirText_Any;//群組比較方向-任方向
	CString                    m_AlgGroupCompareDirText_One;//群組比較方向-同方向
	//---------------------------------------------------------------------------------//	
	CString                    m_Alg3DHeightBaseText_Min;//3D高度基本模式-最小
	CString                    m_Alg3DHeightBaseText_Max;//3D高度基本模式-最大
	CString                    m_Alg3DHeightBaseText_Ave;//3D高度基本模式-平均
	CString                    m_Alg3DHeightBaseText_Mid;//3D高度基本模式-中位數
	CString                    m_Alg3DHeightBaseText_SQR;//3D高度基本模式-
	//---------------------------------------------------------------------------------//	
	CString                    m_AlgSearchDirectionText_Forward;//搜尋方向-同向
	CString                    m_AlgSearchDirectionText_Backward;//搜尋方向-反向
	//---------------------------------------------------------------------------------//	
	CString                    m_AlgEdgeFeatureText_W2B;//邊緣特徵-白到黑
	CString                    m_AlgEdgeFeatureText_B2W;//邊緣特徵-黑到白
	//---------------------------------------------------------------------------------//
	CString                    m_AlgHeightDetectionType1;//
	CString                    m_AlgHeightDetectionType2;//
	CString                    m_AlgHeightDetectionDirection;//
	CString                    m_AlgHeightDetectionMeasureMode1;//
	CString                    m_AlgHeightDetectionMeasureMode2;//
	CString                    m_AlgHeightDetectionOutputType1;//
	CString                    m_AlgHeightDetectionOutputType2;//
	//---------------------------------------------------------------------------------//
	CString                    m_BoxTowardText_Up;//框朝向-上
	CString                    m_BoxTowardText_Left;//框朝向-左
	CString                    m_BoxTowardText_Down;//框朝向-下
	CString                    m_BoxTowardText_Right;//框朝向-右
	//---------------------------------------------------------------------------------//	
	CString                    m_BoxShapeText_Rect;//框外形-矩形
	CString                    m_BoxShapeText_RectRound;//框外形-矩形-圓角
	CString                    m_BoxShapeText_Ellipse;//框外形-橢圓形
	CString                    m_BoxShapeText_Capsule;//框外形-膠囊形
	CString                    m_BoxShapeText_Bullet;//框外形-子彈形
	CString                    m_BoxShapeText_RectHalfRound;//框外形-矩形-半圓角
	CString                    m_BoxShapeText_TShape;//框外形-T形
	//---------------------------------------------------------------------------------//		
	CString                    m_LandTypeText_Pad;//特徵框-焊盤
	CString                    m_LandTypeText_Electrode;//特徵框-電極
	CString                    m_LandTypeText_ICLead;//特徵框-IC-引腳
	CString                    m_LandTypeText_ConLead;//特徵框-連接器引腳
	CString                    m_LandTypeText_DipLead;//特徵框-插件引腳
	//---------------------------------------------------------------------------------//			
	CString                    m_ModelPadText;//模組部位-焊盤
	CString                    m_ModelBodyText;//模組部位-本體
	CString                    m_ModelLeadText;//模組部位-引腳
	CString                    m_ModelLeadTipText;//模組部位-引腳前端
	CString                    m_ModelLeadShoulderText;//模組部位-引腳根部
	//---------------------------------------------------------------------------------//			
	CString                    m_ModelMaskPadText;//模組遮罩-焊盤
	CString                    m_ModelMaskBodyText;//模組遮罩-本體
	CString                    m_ModelMaskBodyNoLeadText;//模組遮罩-本體去腳
	CString                    m_ModelMaskLeadText;//模組遮罩-引腳
	CString                    m_ModelMaskLeadTipText;//模組遮罩-引腳前端
	CString                    m_ModelMaskLeadShoulderText;//模組遮罩-引腳根部
	//---------------------------------------------------------------------------------//			
	CString                    m_ModelGroupText_All;//模組群組-全部
	//---------------------------------------------------------------------------------//
	CString                    m_ModelTypeText_Null;//模組樣式-未定義
	CString                    m_ModelTypeText_Chip;//模組樣式-被動元件
	CString                    m_ModelTypeText_ChipC;//模組樣式-被動元件-電容
	CString                    m_ModelTypeText_ChipR;//模組樣式-被動元件-電阻
	CString                    m_ModelTypeText_ChipL;//模組樣式-被動元件-電感
	CString                    m_ModelTypeText_ChipLed;//模組樣式-LED雙腳
	CString                    m_ModelTypeText_Melf;//模組樣式-被動元件

	CString                    m_ModelTypeText_Electrode;//模組樣式-電極元件
	CString                    m_ModelTypeText_Tant;//模組樣式-鉭質電容
	CString                    m_ModelTypeText_CN;//模組樣式-排容
	CString                    m_ModelTypeText_RN;//模組樣式-排阻
	CString                    m_ModelTypeText_SOT;//模組樣式-三腳晶體
	CString                    m_ModelTypeText_ElecCap;//模組樣式-電解電容
	CString                    m_ModelTypeText_LedArray;//模組樣式-LED

	CString                    m_ModelTypeText_NoLead;//模組樣式-無腳元件
	CString                    m_ModelTypeText_NoLeadDN;//模組樣式-無腳元件-DFN
	CString                    m_ModelTypeText_NoLeadQFN;//模組樣式-無腳元件-QFN
	CString                    m_ModelTypeText_NoLeadOSC;//模組樣式-無腳元件-振盪器

	CString                    m_ModelTypeText_LeadCom;//模組樣式-Lead元件
	CString                    m_ModelTypeText_LeadComSOP;//模組樣式-Lead元件-SOP
	CString                    m_ModelTypeText_LeadComQFP;//模組樣式-Lead元件-QFP
	CString                    m_ModelTypeText_LeadComSOT;//模組樣式-Lead元件-SOT

	CString                    m_ModelTypeText_JLeadCom;//模組樣式-J-Lead元件
	CString                    m_ModelTypeText_JLeadComSOJ;//模組樣式-J-Lead元件-SOJ
	CString                    m_ModelTypeText_JLeadComPLCC;//模組樣式-J-Lead元件-PLCC

	CString                    m_ModelTypeText_CompositeCom;//模組樣式-複合元件
	CString                    m_ModelTypeText_PowerTransistor;//模組樣式-功率電晶體
	CString                    m_ModelTypeText_Connector;//模組樣式-連接器

	CString                    m_ModelTypeText_BGA;//模組樣式-BGA元件
	CString                    m_ModelTypeText_Fd;//模組樣式-Fd元件	
	CString                    m_ModelTypeText_Barcode;//模組樣式-Barcode元件	

	CString                    m_ModelTypeText_Pad;//模組樣式-焊盤元件
	CString                    m_ModelTypeText_GoldFinger;//模組樣式-金手指

	CString                    m_ModelTypeText_DipLead;//模組樣式-標準DIP引腳	
	CString                    m_ModelTypeText_Ohters;//模組樣式-其餘的	
	//---------------------------------------------------------------------------------//
	CString                    m_WndLogText_None;//框邏輯-關閉
	CString                    m_WndLogText_GroupID;//框邏輯-同群組編號
	CString                    m_WndLogText_DefectID;//框邏輯-同瑕疵代碼
	//---------------------------------------------------------------------------------//
	CString                    m_WndFollowText_None;//框同動-關閉
	CString                    m_WndFollowText_Pad;//框同動-跟焊盤移動
	CString                    m_WndFollowText_Part;//框同動-跟零件移動
	CString                    m_WndFollowText_PadBody;//框同動-跟焊盤移動-本體
	CString                    m_WndFollowText_PadLead;//框同動-跟焊盤移動-引腳
	CString                    m_WndFollowText_PartBody;//框同動-跟零件移動-本體
	CString                    m_WndFollowText_PartLead;//框同動-跟零件移動-引腳
	//---------------------------------------------------------------------------------//
	CString                    m_WndRgnLinkText_None;//框連動-無
	CString                    m_WndRgnLinkText_Pad;//框連動-焊盤
	CString                    m_WndRgnLinkText_Body;//框連動-本體
	CString                    m_WndRgnLinkText_Lead;//框連動-引腳
	CString                    m_WndRgnLinkText_PadTip;//框連動-焊盤前端
	CString                    m_WndRgnLinkText_PadRgn;//框連動-焊盤範圍
	CString                    m_WndRgnLinkText_PadBodyRgn;//框連動-焊盤+本體範圍
	CString                    m_WndRgnLinkText_PadRgnInner;//框連動-焊盤範圍-內部
	CString                    m_WndRgnLinkText_LeadTip;//框連動-引腳前端
	CString                    m_WndRgnLinkText_LeadShoulder;//框連動-引腳根部
	CString                    m_WndRgnLinkText_LeadTipShoulder;//框連動-引腳根部與前端
	//---------------------------------------------------------------------------------//	
	CString                    m_WndSyncMoveText_Rotate;//同步-旋轉
	CString                    m_WndSyncMoveText_Mirror;//同步-鏡射
	CString                    m_WndSyncMoveText_Symmetry;//同步-對稱
	//---------------------------------------------------------------------------------//	
	CString                    m_WndConstrainText_Disable;//局限-關閉
	CString                    m_WndConstrainText_PadRgnMove;//局限-焊盤範圍內-移動
	CString                    m_WndConstrainText_PadRgnXMove;//局限-焊盤範圍內-X-移動
	CString                    m_WndConstrainText_PadRgnYMove;//局限-焊盤範圍內-Y-移動	
	CString                    m_WndConstrainText_PadRgnScale;//局限-焊盤範圍內-縮放
	CString                    m_WndConstrainText_PadRgnXScale;//局限-焊盤範圍內-X-縮放
	CString                    m_WndConstrainText_PadRgnYScale;//局限-焊盤範圍內-Y-縮放	
	//---------------------------------------------------------------------------------//	
	std::map<WND_DEFECT_ID, size_t> m_MapWndDefectIDIndex;//映射表-瑕疵代碼-引數
	bool                       BuildWndDefectIDMapIndex();
	//---------------------------------------------------------------------------------//
	CString                    m_WndDefectText_None;//框瑕疵-無定義
	CString                    m_WndDefectText_PadAlign;//框瑕疵-焊盤定位
	CString                    m_WndDefectText_PartAlign;//框瑕疵-本體定位
	CString                    m_WndDefectText_PadAdjust;//框瑕疵-焊盤調整
	CString                    m_WndDefectText_LeadAdjust;//框瑕疵-管腳調整

	CString                    m_WndDefectText_ClassCheck;//框瑕疵-類別確認
	CString                    m_WndDefectText_BaseValue;//框瑕疵-基準數值

	CString                    m_WndDefectText_BodyMissing;//框瑕疵-缺件
	CString                    m_WndDefectText_BodyOffset;//框瑕疵-偏移
	CString                    m_WndDefectText_BodyTilt;//框瑕疵-本體傾斜
	CString                    m_WndDefectText_BodyPolarity;//框瑕疵-極反
	CString                    m_WndDefectText_BodyTurnOver;//框瑕疵-反件
	CString                    m_WndDefectText_BodyMount;//框瑕疵-錯件-裝貼
	CString                    m_WndDefectText_BodyWrongCode;//框瑕疵-錯件-條碼
	CString                    m_WndDefectText_BodyWrongText;//框瑕疵-錯件-文字
	CString                    m_WndDefectText_BodyTombstone;//框瑕疵-立碑
	CString                    m_WndDefectText_BodyBillboard;//框瑕疵-側立
	CString                    m_WndDefectText_BodyDamaged;//框瑕疵-破損	

	CString                    m_WndDefectText_SolderPoor;//框瑕疵-焊錫不足
	CString                    m_WndDefectText_SolderOpen;//框瑕疵-焊錫空焊
	CString                    m_WndDefectText_SolderPadExposed;//框瑕疵-焊錫沒有-漏銅
	CString                    m_WndDefectText_SolderBridge;//框瑕疵-焊錫短路
	CString                    m_WndDefectText_SolderBead;//框瑕疵-焊錫錫珠
	CString                    m_WndDefectText_SolderExcess;//框瑕疵-焊錫過量

	CString                    m_WndDefectText_LeadLifted;//框瑕疵-引腳翹起
	CString                    m_WndDefectText_LeadBended;//框瑕疵-引腳彎曲
	CString                    m_WndDefectText_LeadProtruded;//框瑕疵-引腳凸出

	CString                    m_WndDefectText_PadScratch;//框瑕疵-焊盤刮傷
	CString                    m_WndDefectText_ForeignBody;//框瑕疵-異物

	CString                    m_WndDefectText_UserDefine_01;//框瑕疵-使用者定義-01
	CString                    m_WndDefectText_UserDefine_02;//框瑕疵-使用者定義-02
	CString                    m_WndDefectText_UserDefine_03;//框瑕疵-使用者定義-03
	CString                    m_WndDefectText_UserDefine_04;//框瑕疵-使用者定義-04
	CString                    m_WndDefectText_UserDefine_05;//框瑕疵-使用者定義-05
	CString                    m_WndDefectText_UserDefine_06;//框瑕疵-使用者定義-06
	CString                    m_WndDefectText_UserDefine_07;//框瑕疵-使用者定義-07
	CString                    m_WndDefectText_UserDefine_08;//框瑕疵-使用者定義-08
	CString                    m_WndDefectText_UserDefine_09;//框瑕疵-使用者定義-09
	CString                    m_WndDefectText_UserDefine_10;//框瑕疵-使用者定義-10
	//---------------------------------------------------------------------------------//	
	CString                    m_BasePlaneProcText_1;//樣式-1
	CString                    m_BasePlaneProcText_2;//樣式-2
	//---------------------------------------------------------------------------------//	
	CString                    m_BasePlaneAutoRgnText_Disable;//關閉
	CString                    m_BasePlaneAutoRgnText_Group;//群組比較
	CString                    m_BasePlaneAutoRgnText_Lowest;//最低高度
	//---------------------------------------------------------------------------------//	
	CString                    m_BasePlaneBodyOutsideText_Disable;//關閉
	CString                    m_BasePlaneBodyOutsideText_Body;//本體
	CString                    m_BasePlaneBodyOutsideText_BodyLand;//本體+特徵框	
	//---------------------------------------------------------------------------------//
	CString                    m_CalcBasePlaneText_Disable;//關閉
	CString                    m_CalcBasePlaneText_Ave;//平均值
	CString                    m_CalcBasePlaneText_Corner;//四個端點
	CString                    m_CalcBasePlaneText_IsoData;//Iso data
	CString                    m_CalcBasePlaneText_Ostu;//OTSU
	CString                    m_CalcBasePlaneText_CornerOnly;//只有四個端點
	CString                    m_CalcBasePlaneText_Surround;//周圍
	CString                    m_CalcBasePlaneText_AutoLower;//自動最低
	CString                    m_CalcBasePlaneText_Panel;//參考整板
	CString                    m_CalcBasePlaneText_Local;//參考局部平面
	//---------------------------------------------------------------------------------//
	CString                    m_FdText_NG;//定位點文字-瑕疵   
	CString                    m_BarcodeText_NG;//條碼文字-瑕疵   
	//---------------------------------------------------------------------------------//
	CString                    m_ProjectColorGroupText_Pad;//色彩群組文字-銅箔
	CString                    m_ProjectColorGroupText_Body;//色彩群組文字-本體
	CString                    m_ProjectColorGroupText_Void;//色彩群組文字-空焊
	CString                    m_ProjectColorGroupText_Board;//色彩群組文字-基板
	CString                    m_ProjectColorGroupText_Solder;//色彩群組文字-焊錫	
	CString                    m_ProjectColorGroupText_Ohters;//色彩群組文字-其它
	//---------------------------------------------------------------------------------//
	CString                    m_InspectionResultFaultText;//檢測結果異常
	CString                    m_DoYouWantToClearTheStatisticRecordsText;//是否清除統計資料
	//---------------------------------------------------------------------------------//
	CString                    m_MotionAccTimeAdjustText_Off;//軸控加速度調整模式-關閉
	CString                    m_MotionAccTimeAdjustText_Fix;//軸控加速度調整模式-固定
	CString                    m_MotionAccTimeAdjustText_Min;//軸控加速度調整模式-最小值
	CString                    m_MotionAccTimeAdjustText_Gamma;//軸控加速度調整模式-Gamma
	//---------------------------------------------------------------------------------//
	CString					m_AS608CommandText_GetImage;
	CString					m_AS608CommandText_GenChar;
	CString					m_AS608CommandText_Match;
	CString					m_AS608CommandText_RegMode;
	CString					m_AS608CommandText_UpChar;
	CString					m_AS608CommandText_DownChar;

	CString					m_AS608StatusText_OK;
	CString					m_AS608StatusText_Error;
	CString					m_AS608StatusText_NOFingerprint;
	CString					m_AS608StatusText_InputError;
	CString					m_AS608StatusText_ImageTooDry;
	CString					m_AS608StatusText_ImageTooWet;
	CString					m_AS608StatusText_ImageTooClutter;
	CString					m_AS608StatusText_ImageTooFewFeature;
	CString					m_AS608StatusText_NotMatch;
	CString					m_AS608StatusText_NoMove;
	CString					m_AS608StatusText_FeatureCombineError;
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//		
	void                       PreInitDefine();
	void                       InitialDefine();
	void                       CloneDefine(const CAOIDataDefine &Define);		
	//---------------------------------------------------------------------------------//	
	bool                       GetUILanguageString(LPCTSTR Section, LPCTSTR Key, LPCTSTR Default, CString &String);//取得視窗文字
	//---------------------------------------------------------------------------------//	
public:		
	//---------------------------------------------------------------------------------//		
	CAOIDataDefine();
	//CAOIDataDefine(const CAOIDataDefine &Define);
	virtual ~CAOIDataDefine();
	//CAOIDataDefine& operator=(const CAOIDataDefine &Define);
	//---------------------------------------------------------------------------------//
	bool                      LoadDefineTextFile();//載入定義文字檔案
	//---------------------------------------------------------------------------------//	
	CString                   GetLaneIDText(LANE_ID LaneID);//取得軌道編號名稱	
	bool                      BuildLaneIDCombox(CComboBox &Combox);//建立軌道編號列表
	//---------------------------------------------------------------------------------//				
	LPCTSTR                   GetIDText() const;//取得編號文字
	LPCTSTR                   GetToText() const;//取得去文字	
	LPCTSTR                   GetFdText() const;//取得定位點文字
	LPCTSTR                   GetAddText() const;//取得新增文字
	LPCTSTR                   GetSetText() const;//取得設定文字
	LPCTSTR                   GetGetText() const;//取得取得文字
	LPCTSTR                   GetPadText() const;//取得焊盤文字	
	LPCTSTR                   GetWndText() const;//取得檢測框文字	
	LPCTSTR                   GetAveText() const;//取得平均文字	
	LPCTSTR                   GetAllText() const;//取得全部文字	
	LPCTSTR                   GetMaxText() const;//取得最大值文字	
	LPCTSTR                   GetMinText() const;//取得最小值文字	
	LPCTSTR                   GetFromText() const;//來文字	
	LPCTSTR                   GetAxisText() const;//軸文字	
	LPCTSTR                   GetLineText() const;//線文字	
	LPCTSTR                   GetAreaText() const;//面積文字		
	LPCTSTR                   GetLaneText() const;//軌道文字	
	LPCTSTR                   GetMarkText() const;//特徵文字	
	LPCTSTR                   GetNameText() const;//名稱文字	
	LPCTSTR                   GetSkewText() const;//偏角文字	
	LPCTSTR                   GetGainText() const;//增益文字
	LPCTSTR                   GetGrayText() const;//灰階文字
	LPCTSTR                   GetSizeText() const;//尺寸文字	
	LPCTSTR                   GetLandText() const;//特徵框文字
	LPCTSTR                   GetTimeText() const;//時間文字
	LPCTSTR                   GetTestText() const;//檢測文字		
	LPCTSTR                   GetRangeText() const;//範圍文字	
	LPCTSTR                   GetClassText() const;//類別文字
	LPCTSTR                   GetClearText() const;//清除文字	
	LPCTSTR                   GetRatioText() const;//比例文字
	LPCTSTR                   GetIndexText() const;//取得序號文字
	LPCTSTR                   GetGrabText() const;//取得取像文字
	LPCTSTR                   GetCountText() const;//取得數量文字
	LPCTSTR                   GetErrorText() const;//取得錯誤文字
	LPCTSTR                   GetPanelText() const;//整板文字
	LPCTSTR                   GetBoardText() const;//單板文字
	LPCTSTR                   GetModelText() const;//模組文字
	LPCTSTR                   GetGroupText() const;//群組文字	
	LPCTSTR                   GetScoreText() const;//分數文字	
	LPCTSTR                   GetScaleText() const;//縮放文字	
	LPCTSTR                   GetYieldText() const;//良率文字		
	LPCTSTR                   GetPixelText() const;//像素文字	
	LPCTSTR                   GetSizeXText() const;//X尺寸文字
	LPCTSTR                   GetSizeYText() const;//Y尺寸文字	
	LPCTSTR                   GetMethodText() const;//方法文字
	LPCTSTR                   GetTowardText() const;//朝向文字
	LPCTSTR                   GetCloneText() const;//複製文字
	LPCTSTR                   GetCreateText() const;//創建文字
	LPCTSTR                   GetModifyText() const;//修改文字
	LPCTSTR                   GetDeleteText() const;//刪除文字
	LPCTSTR                   GetDefectText() const;//瑕疵文字
	LPCTSTR                   GetResultText() const;//結果文字		
	LPCTSTR                   GetUnsetText() const;//未設定文字			
	LPCTSTR                   GetAngleText() const;//角度文字
	LPCTSTR                   GetWidthText() const;//寬度文字
	LPCTSTR                   GetHeightText() const;//長度文字	
	LPCTSTR                   GetVolumeText() const;//體積文字		
	LPCTSTR                   GetOffsetText() const;//偏移文字	
	LPCTSTR                   GetFinishText() const;//完成文字
	LPCTSTR                   GetProjectText() const;//專案文字	
	LPCTSTR                   GetPatternText() const;//樣板文字	
	LPCTSTR                   GetDefaultText() const;//預設文字
	LPCTSTR                   GetThroughText() const;//貫穿文字	
	LPCTSTR                   GetBarcodeText() const;//條碼文字	
	LPCTSTR                   GetWarningText() const;//警告文字		
	LPCTSTR                   GetGroupIDText() const;//群組編號文字			
	LPCTSTR                   GetBypassedText() const;//不檢測文字				
	LPCTSTR                   GetDistrictText() const;//分段文字
	LPCTSTR                   GetDistanceText() const;//距離文字
	LPCTSTR                   GetVelocityText() const;//速度文字
	LPCTSTR                   GetRelativeText() const;//相對文字		
	LPCTSTR                   GetContinueText() const;//連續文字	
	LPCTSTR                   GetContrastText() const;//對比文字	
	LPCTSTR                   GetThicknessText() const;//厚度文字
	LPCTSTR                   GetAlgorithmText() const;//零件文字
	LPCTSTR                   GetLockScreenText() const;//鎖住螢幕文字	
	LPCTSTR                   GetVerticalText() const;//垂直文字
	LPCTSTR                   GetHorizontalText() const;//水平文字
	LPCTSTR                   GetComponentText() const;//零件文字
	LPCTSTR                   GetPartNumberText() const;//料號文字	
	LPCTSTR                   GetNozzleNameText() const;//吸嘴文字		
	LPCTSTR                   GetCalculateText() const;//取得計算文字
	LPCTSTR                   GetVersionCodeText() const;//取得版本號文字	
	LPCTSTR                   GetUndefinedText() const;//取得未定義文字		
	LPCTSTR                   GetAccelerationText() const;//取得加速度文字
	LPCTSTR                   GetBarcodeDeviceText() const;//條碼機文字	
	LPCTSTR                   GetSaftyBypassText() const;//安全檢知文字
	LPCTSTR                   GetWaitForCCSText() const;//等待中控文字	
	LPCTSTR                   GetWaitForRepairText() const;//等待維修站文字	
	LPCTSTR                   GetInspectionResultFaultText() const;//檢測結果異常	
	LPCTSTR                   GetDoYouWantToClearTheStatisticRecordsText() const;//是否清除統計資料
	//---------------------------------------------------------------------------------//
	LPCTSTR                   GetColorText_Red() const;//顏色文字-紅色
	LPCTSTR                   GetColorText_Green() const;//顏色文字-綠色
	LPCTSTR                   GetColorText_Blue() const;//顏色文字-藍色
	LPCTSTR                   GetColorText_Black() const;//顏色文字-黑色
	LPCTSTR                   GetColorText_Gray() const;//顏色文字-灰色
	LPCTSTR                   GetColorText_White() const;//顏色文字-白色
	LPCTSTR                   GetColorText_Color() const;//顏色文字-彩色	
	//---------------------------------------------------------------------------------//
	LPCTSTR                   GetWndOKText() const;//視窗OK文字
	LPCTSTR                   GetWndCancelText() const;//視窗Cancel文字
	//---------------------------------------------------------------------------------//
	LPCTSTR                   GetDecodeText() const;//取得解碼文字	
	LPCTSTR                   GetLevelText() const;//等級文字
	//---------------------------------------------------------------------------------//
	LPCTSTR                   GetEnableText() const;//取得啟用文字
	LPCTSTR                   GetDisableText() const;//取得啟用, 關閉文字
	CString                   GetEnableDisableText(int nEnable);//取得啟用, 關閉文字
	CString                   GetPositiveNegativeText(int nPositive);//取得正負向文字
	CString                   GetDistrictIDText(DISTRICT_ID Mode);//取得分區文字
	CString                   GetWndDefectItemModeText(int Mode);//取得檢測框瑕疵項目模式文字
	//---------------------------------------------------------------------------------//
	int                       FindEnableDisableIDByText(LPCTSTR IDText);//取得啟用關閉編號
	bool                      BuildEnableDisableParamUni(CParamUni &ParamUnit);//建立啟用關閉模式列表	
	//---------------------------------------------------------------------------------//
	CString                   GetPCBOutModeText(PCB_OUT_MODE Mode);//取得出板模式文字
	CString                   GetMultiLaneModeText(MULTI_LANE_MODE Mode);//取得多軌道模式名稱
	CString                   GetFuncExecModeText(FUNC_EXEC_MODE Mode);//取得函式執行模式名稱
	CString                   GetLaneWorkModeText(LANE_WORK_MODE Mode);//取得軌道運轉模式名稱	
	CString                   GetUserLevelModeText(USER_LEVEL_MODE Mode);//取得使用者權限模式文字
	CString                   GetUserLoginModeText(USER_LOGIN_MODE Mode);//取得使用者登入模式文字
	CString                   GetUserLoginOptionsText(USER_LOGIN_OPTIONS Mode);//取得使用者登入選項文字
	CString                   GetOpenProjectModeText(OPEN_PROJECT_MODE Mode);//取得開啟專案模式文字	
	CString                   GetVerifyProjectModeText(VERIFY_PROJECT_MODE Mode);//取得驗證專案模式文字	
	CString                   GetOnlineInputTimingText(ONLINE_INPUT_TIMING Mode);//取得線上輸入時機文字	
	CString                   GetAOICustomerIDKey(AOI_CUSTOMER_ID ID);//取得客戶編號文字
	CString                   GetAOICustomerIDText(AOI_CUSTOMER_ID ID);//取得客戶編號文字
	CString                   GetOfflineVersionModeText(OFFLINE_VERSION_MODE Mode);//取得離線版本模式文字	
	CString                   GetOnlineOpenProjectModeText(ONLINE_OPEN_PROJECT_MODE Mode);//取得線上開啟專案模式文字	
	CString                   GetProjectLinkServerModeText(PROJECT_LINK_SERVER_MODE Mode);//取得專案連線伺服器模式文字
	CString                   GetXBoardMappingFileModeText(XBOARD_MAPPING_FILE_MODE Mode);//取得報廢板檔案映射模式文字
	CString                   GetXBoardMappingFileFlowText(XBOARD_MAPPING_FILE_FLOW Mode);//取得報廢板檔案映射順序文字
	CString                   GetPCBOutDirectionText(PCB_OUT_DIRECTION Mode);//取得出板流向文字	
	CString                   GetFdNGHandleModeText(FD_NG_HANDLE_MODE Mode);//取得定位點錯誤處理模式文字
	CString                   GetBoardFdGrabModeText(BOARD_FD_GRAB_MODE Mode);//取得單板定位點取像模式文字	
	CString                   GetDefectHandleModeText(DEFECT_HANDLE_MODE Mode);//取得檢出瑕疵處理模式文字	
	CString                   GetOnlineStateText(ONLINE_STATE_MODE State);//取得在線檢測狀態文字
	CString                   GetMesEqpCtrlStateText(MES_EQP_CTRL_STATE_MODE Mode);//取得MES機台控制模式文字
	CString                   GetFieldPathMode(FIELD_PATH_MODE Mode);//取得區域路徑模式文字
	CString                   GetFieldDivisionMode(FIELD_DIVISION_MODE Mode);//取得區域分割模式文字	
	CString                   GetInspectionFieldBuildMode(FIELD_BUILD_MODE Mode);//取得檢測區域配置模式文字
	CString                   GetConnectedBufferTypeText(CONNECTED_BUFFER_TYPE Type);//取得連接軌道機樣式名稱	
	CString                   GetHASIStageText(HASI_AOI_STAGE Stage);//取得HASI AOI Stage文字
	CString                   GetHASIStateModeText(HASI_STATE_MODE Mode);//取得HASI狀態文字
	//---------------------------------------------------------------------------------//	
	bool                      BuildRS232PortCombox(CComboBox &Combox);//建立RS232的埠列表
	bool                      BuildRS232BaudCombox(CComboBox &Combox);//建立RS232的鮑率列表
	bool                      BuildRS232ParityCombox(CComboBox &Combox);//建立RS232的極性列表
	bool                      BuildRS232StopBitsCombox(CComboBox &Combox);//建立RS232的停止位元列表	
	//---------------------------------------------------------------------------------//
	bool                      BuildLoadCadxyTextFilterCombox(CComboBox &Combox);//建立載入CadXY的文字過濾列表
	//---------------------------------------------------------------------------------//
	PANEL_SIDE_MODE           FindPanelSideModeByText(LPCTSTR SideText);//依文字取得板面模式
	CString                   GetPanelSideModeText(PANEL_SIDE_MODE Mode);//取得出板面文字
	bool                      BuildPanelSideModeCombox(CComboBox &Combox);//建立板面樣式列表	
	//---------------------------------------------------------------------------------//
	BOARD_SIDE_MODE           FindBoardSideModeByText(LPCTSTR SideText);//依文字取得板面模式
	CString                   GetBoardSideModeText(BOARD_SIDE_MODE Mode);//取得出板面文字
	bool                      BuildBoardSideModeCombox(CComboBox &Combox);//建立板面樣式列表	
	//---------------------------------------------------------------------------------//
	BARCODE_DECODER_TYPE      FindBarcodeDecoderTypeByText(LPCTSTR SideText);//依文字取得條碼解碼器
	CString                   GetBarcodeDecoderText(BARCODE_DECODER_TYPE Mode);//取得條碼解碼器文字
	//---------------------------------------------------------------------------------//
	BARCODE_SPREAD_MODE       FindBarcodeSpreadModeByText(LPCTSTR ModeText);//依文字取得條碼擴散模式
	CString                   GetBarcodeSpreadModeText(BARCODE_SPREAD_MODE Mode);//取得儲條碼擴散模式文字	
	//---------------------------------------------------------------------------------//	
	BARCODE_BELONG_MODE       FindBarcodeBelongModeByText(LPCTSTR ModeText);//依文字取得條碼屬於模式
	CString                   GetBarcodeBelongModeText(BARCODE_BELONG_MODE Mode);//取得儲條碼屬於模式文字	
	//---------------------------------------------------------------------------------//	
	SAVE_TEST_MAP_MODE        FindSaveTestMapModeByText(LPCTSTR ModeText);//依文字取得儲存檢測底圖模式
	CString                   GetSaveTestMapModeText(SAVE_TEST_MAP_MODE Mode);//取得儲存檢測底圖模式文字	
	//---------------------------------------------------------------------------------//
	OFFLINE_IMAGE_SCOPE       FindOfflineImageScopeByText(LPCTSTR ScopeText);//依文字取得離線影像範疇
	CString                   GetOfflineImageScopeText(OFFLINE_IMAGE_SCOPE Mode);//取得離線影像範疇
	//---------------------------------------------------------------------------------//	
	SAVE_TEST_IMAGE_MODE      FindSaveTestImageModeByText(LPCTSTR ModeText);//依文字取得儲存檢測圖片模式
	CString                   GetSaveTestImageModeText(SAVE_TEST_IMAGE_MODE Mode);//取得儲存檢測圖片模式文字	
	//---------------------------------------------------------------------------------//	
	SAVE_TEST_DATA_MODE       FindSaveTestDataModeByText(LPCTSTR ModeText);//依文字取得儲存資料模式
	CString                   GetSaveTestDataText(SAVE_TEST_DATA_MODE Mode);//取得儲存檢測資料模式文字
	//---------------------------------------------------------------------------------//	
	CString 	              GetBarcodeInputTypeText(BARCODE_INPUT_TYPE ReadType);//取得條碼輸入樣式名稱
	bool                      BuildBarcodeInputTypeCombox(CComboBox &Combox);//建立條碼輸入樣式列表	
	//---------------------------------------------------------------------------------//
	CString 	              GetBarcodeNGHandleModeText(BARCODE_NG_HANDLE_MODE Mode);//取得條碼失敗處理模式名稱
	bool                      BuildBarcodeNGHandleModeCombox(CComboBox &Combox);//建立條碼失敗處理模式列表	
	//---------------------------------------------------------------------------------//
	CString 	              GetMultiProjectTestOrderText(MULTI_PROJECT_TEST_ORDER_MODE Mode);//取得多專案檢測次序文字
	bool                      BuildMultiProjectTestOrderCombox(CComboBox &Combox);//建立多專案檢測次序列表	
	//---------------------------------------------------------------------------------//
	CString 	              GetBarcodeCameraGrabModeText(BARCODE_CAMERA_GRAB_MODE Mode);//取得相機條碼取像模式文字
	bool                      BuildBarcodeCameraGrabModeCombox(CComboBox &Combox);//建立相機條碼取像模式列表	
	//---------------------------------------------------------------------------------//
	CString 	              GetBarcodeDeviceGrabModeText(BARCODE_DEVICE_GRAB_MODE Mode);//取得條碼機取像模式文字
	bool                      BuildBarcodeDeviceGrabModeCombox(CComboBox &Combox);//建立條碼機取像模式列表	
	//---------------------------------------------------------------------------------//
	CString 	              GetBarcodeHandHeldReadModeText(BARCODE_HANDHELD_READ_MODE Mode);//取得手持條碼機取像模式文字
	bool                      BuildBarcodeHandHeldReadModeCombox(CComboBox &Combox);//建立手持條碼機取像模式列表	
	//---------------------------------------------------------------------------------//
	BARCODE_AUTO_EXPAND_MODE  GetBarcodeAutoExpandModeByInt(int Param) const;
	bool                      GetBarcodeAutoExpandModeList(std::vector<BARCODE_AUTO_EXPAND_MODE> &List);//取得條碼自動擴展模式列表
	CString 	              GetBarcodeAutoExpandModeText(BARCODE_AUTO_EXPAND_MODE Mode);//取得條碼自動擴展模式文字
	bool                      BuildBarcodeAutoExpandModeCombox(CComboBox &Combox);//建立條碼自動擴展模式列表	
	bool                      BuildBarcodeAutoExpandModeParamUni(CParamUni &ParamUnit);//建立條碼自動擴展模式列表	
	//---------------------------------------------------------------------------------//
	CString 	              GetBarcodeDirectionModeText(ALG_BARCODE_DIR_MODE Mode);//取得條碼方向模式文字
	ALG_BARCODE_DIR_MODE      FindBarcodeDirectionModeByText(LPCTSTR DirText);//依文字取得條碼方向模式
	//---------------------------------------------------------------------------------//	
	bool                      CheckAutoSwitchWnd3DFrameByType(MODEL_TYPE Type);//確認自動切換檢測框3D畫面依據樣式
	CString 	              GetAutoSwitchWnd3DFrameModeText(AUTO_SWITCH_WND_3D_FRAME_MODE Mode);//取得自動切換檢測框3D模式文字
	AUTO_SWITCH_WND_3D_FRAME_MODE FindAutoSwitchWnd3DFrameModeByText(LPCTSTR SwitchText);//依自動切換檢測框3D模式
	//---------------------------------------------------------------------------------//
	CString 	              GetAlarmLockText(ALARM_LOCK_MODE Mode);//取得警報鎖住名稱
	bool                      BuildAlamLockCombox(CComboBox &Combox);//建立警報鎖住列表
	//---------------------------------------------------------------------------------//
	CString 	              GetDefectFromText(DEFECT_FROM_MODE DefectFrom);//取得瑕疵來源名稱
	bool                      BuildDefectFromCombox(CComboBox &Combox);//建立瑕疵來源列表
	//---------------------------------------------------------------------------------//
	CString                   GetTop10ScopeText(TOP10_SCOPE Scope);//取得前十大模式
	bool                      BuildTop10ScopeCombox(CComboBox &Combox);//建立前十大來源列表
	//---------------------------------------------------------------------------------//
	CString                   GetYieldingScopeText(YIELDING_SCOPE Scope);//取得良率來源名稱
	bool                      BuildYieldingScopeCombox(CComboBox &Combox);//建立良率來源列表
	//---------------------------------------------------------------------------------//
	CString                   GetDefectParamFromText(DEFECT_PARAM_FROM_MODE Mode);//取得瑕疵參數來源名稱
	bool                      BuildDefectParamFromCombox(CComboBox &Combox);//建立瑕疵參數來源列表
	//---------------------------------------------------------------------------------//
	CString                   GetCpkFromText(CPK_FROM_MODE Mode);//取得Cpk來源名稱
	bool                      BuildCpkFromCombox(CComboBox &Combox);//建立Cpk來源列表
	//---------------------------------------------------------------------------------//
	CString                   GetOnlineLaneImageName(LANE_ID LaneID);//取得線上軌道圖檔名稱	
	//---------------------------------------------------------------------------------//
	CString                   GetTestResultImageName(TEST_RESULT_ID ResultID);//取得檢測結果圖檔名稱	
	CString                   GetMultiLanguageModeText(MULTI_LANGUAGE_MODE Mode);//取得多國語系文字
	//---------------------------------------------------------------------------------//	
	CString                    GetAlgTypeText(ALG_TYPE Type);//取得演算法文字
	LPCTSTR                    GetAlgGroupCompareText() const;//取得群組比較文字
	ALG_TYPE                   FindAlgTypeByText(LPCTSTR AlgText);//依名稱尋找演算法編號
	//---------------------------------------------------------------------------------//
	CString                    GetAlgCalcUnitModeText(ALG_CALC_UNIT_MODE Mode);//取得計算單位模式文字
	ALG_CALC_UNIT_MODE         FindAlgCalcUnitModeByText(LPCTSTR Text);//依名稱尋找計算單位模式
	//---------------------------------------------------------------------------------//
	CString                    GetAlgBrightAverageModeText(ALG_BRIGHT_AVERAGE_MODE Mode);//取得亮度平均模式文字
	ALG_BRIGHT_AVERAGE_MODE    FindAlgBrightAverageModeByText(LPCTSTR Text);//依名稱尋找亮度平均模式
	//---------------------------------------------------------------------------------//
	CString                    GetAlgMatchDockModeText(ALG_MATCH_DOCK_MODE Mode);//取得匹配靠邊模式文字
	ALG_MATCH_DOCK_MODE        FindAlgMatchDockModeByText(LPCTSTR Text);//依名稱尋找匹配靠邊模式
	//---------------------------------------------------------------------------------//
	CString                    GetAlgObjectSizeCalcModeText(ALG_OBJECT_SIZE_CALC_MODE Mode);//取得物件尺寸計算模式文字
	ALG_OBJECT_SIZE_CALC_MODE  FindAlgObjectSizeCalcModeByText(LPCTSTR Text);//依名稱尋找物件尺寸計算模式
	//---------------------------------------------------------------------------------//
	CString                    GetAlgObjectHeightAverageModeText(ALG_OBJECT_HEIGHT_AVERAGE_MODE Mode);//取得物件高度平均模式文字
	ALG_OBJECT_HEIGHT_AVERAGE_MODE    FindAlgObjectHeightAverageModeByText(LPCTSTR Text);//依名稱尋找物件高度平均模式
	//---------------------------------------------------------------------------------//
	CString                    GetAlgAngleMeasureAngleModeText(ANGLE_MEASURE_MODE Mode);//取得角度量測角度模式文字
	ANGLE_MEASURE_MODE         FindAlgAngleMeasureAngleModeByText(LPCTSTR Text);//依名稱尋找角度量角度模式
	//---------------------------------------------------------------------------------//	
	CString                    GetAlgAngleMeasureLineEqnModeText(LINE_EQUATION_MODE Mode);//取得角度量測基準線模式文字
	LINE_EQUATION_MODE         FindAlgAngleMeasureLineEqnModeByText(LPCTSTR Text);//依名稱尋找角度量測基準線模式
	//---------------------------------------------------------------------------------//	
	CString                    GetResultIDText(RESULT_ID ResultID);//取得演算法結果文字
	//---------------------------------------------------------------------------------//
	CString                    GetAOIGroupModeText(PART_GROUP_MODE Mode);//取得AOI群組模式文字
	bool                       BuildAOIGroupModeCombox(CComboBox &Combox);//建立群組模式列表
	//---------------------------------------------------------------------------------//
	CString                    GetAlgImageSourceModeText(IMAGE_SRC_MODE Mode);//取得演算法影像來源文字
	bool                       BuildImageSourceModeCombox(CComboBox &Combox, FRAME_TYPE FrameType);
	//---------------------------------------------------------------------------------//
	CString                    GetAlgMaskFuncModeText(MASK_FUNC_MODE Mode);//取得遮罩功能模式文字
	MASK_FUNC_MODE             FindAlgMaskFuncModeByText(LPCTSTR Text);//取得遮罩功能模式
	//---------------------------------------------------------------------------------//
	CString                    GetAlgBinaryModeText(BINARY_MODE Mode);//取得二值化模式文字
	bool                       BuildBinaryModeCombox(CComboBox &Combox, FRAME_TYPE FrameType);
	//---------------------------------------------------------------------------------//
	CString                    GetAlgEdgeEnhanceModeText(EDGE_ENHANCE_MODE Mode);//取得邊緣強化模式文字
	bool                       BuildEdgeEnhanceModeCombox(CComboBox &Combox);
	bool                       BuildEdgeEnhanceFilterModeCombox(CComboBox &Combox);
	//---------------------------------------------------------------------------------//
	CString                    GetAlgColorRGBVModeText(COLOR_RGBV_MODE Mode);//取得彩色-RGBV模式文字
	//---------------------------------------------------------------------------------//
	 CString                   GetAlgNoiseFilterModeText(NOISE_FILTER_MODE Mode);//取得雜訊過濾文字
	 bool                      BuildNoiseFilterModeCombox(CComboBox &Combox, FRAME_TYPE FrameType);
	 //---------------------------------------------------------------------------------//
	 bool                      BuildGrayFilterParamCombox(CComboBox &Combox);
	 bool                      BuildGrayFilterModeCombox(CComboBox &Combox, FRAME_TYPE FrameType);
	 //---------------------------------------------------------------------------------//
	 bool                      BuildBinaryFilterParamCombox(CComboBox &Combox);
	 bool                      BuildBinaryFilterModeCombox(CComboBox &Combox, FRAME_TYPE FrameType);
	 //---------------------------------------------------------------------------------//
	 CString                   GetAlgBrightLineModeText(int Mode);//取得演算法亮度貫穿模式
	 int                       FindAlgBrightLineModeByText(LPCTSTR DirText);
	 //---------------------------------------------------------------------------------//
	 CString                   GetAlgOuterShortExtendModeText(ALG_OUTER_SHORT_EXT_MODE Mode);//取得演算法外接短路延伸模式
	 ALG_OUTER_SHORT_EXT_MODE  FindAlgOuterShortExtendModeByText(LPCTSTR DirText);
	 //---------------------------------------------------------------------------------//
	 CString                   GetAlgDirectionText(ALG_DIRECTION Direction);//取得演算法方向文字
	 ALG_DIRECTION             FindAlgDirectionByText(LPCTSTR DirText);
	 //---------------------------------------------------------------------------------//
	 CString                   GetAlgDirectionXYText(ALG_DIRECTION Direction);//取得演算法方向文字
	 ALG_DIRECTION             FindAlgDirectionByXYText(LPCTSTR DirText);
	 //---------------------------------------------------------------------------------//
	 CString                   GetAlgBarcodeDecodeStepText(ALG_BARCODE_STEP_MODE Mode);//取得條碼步驟模式文字
	 ALG_BARCODE_STEP_MODE     FindAlgBarcodeStepModeByText(LPCTSTR StepText);//取得條碼解碼步驟模式
	 CString                   GetAlgBarcodeDecodeStepParamText(ALG_BARCODE_STEP_MODE Mode, int index);//取得條碼步驟參數文字
	 //---------------------------------------------------------------------------------//
	 CString                   GetAlgFdMatchModeText(FD_MATCH_MODE MatchMode);//取得演算法定位點匹配模式文字
	 FD_MATCH_MODE             FindAlgFdMatchModeByText(LPCTSTR ModeText);
	 //---------------------------------------------------------------------------------//
	 CString                   GetAlgGroupCompareDirText(ALG_GROUP_CMP_DIR_MODE DirMode);//取得演算法群組比較方向文字
	 ALG_GROUP_CMP_DIR_MODE    FindAlgGroupCompareDirByText(LPCTSTR DirText);	 
	 //---------------------------------------------------------------------------------//
	 CString                   GetAlg3DHeightBaseText(ALG_3D_BASE_HEIGHT_MODE BaseMode);//取得演算法3D高度基本模式文字
	 ALG_3D_BASE_HEIGHT_MODE   FindAlg3DHeightBaseModeByText(LPCTSTR BaseText);	 
	 //---------------------------------------------------------------------------------//
	 CString                   GetAlgSearchDirectionText(ALG_SEARCH_DIRECTION Direction);//取得演算法搜尋方向文字
	 ALG_SEARCH_DIRECTION      FindAlgSearchDirectionByText(LPCTSTR DirText);	 
	 //---------------------------------------------------------------------------------//
	 CString                   GetAlgEdgeFeatureText(ALG_EDGE_FEATURE_MODE EdgeMode);//取得演算法邊緣特徵文字
	 ALG_EDGE_FEATURE_MODE     FindAlgEdgeFeatureModeByText(LPCTSTR FeatureText);
	 //---------------------------------------------------------------------------------//
	 CString                   GetAlgPatternFolder(int ImageFolderIndex);//取得演算法樣板資料夾
	 CString                   GetAlgPatternName(int ImaegIndex, BOX_TOWARD Toward);//取得演算法樣板圖檔名稱	 
	 //---------------------------------------------------------------------------------//
	 CString				   GetAlgHeightDetectionTypeText(int type);
	 int					   FindAlgHeightDetectionTypeByText(LPCTSTR FeatureText);
	 CString                   GetAlgHeightDetectionDirectionText(int type);
	 int					   FindAlgHeightDetectionDirectionByText(LPCTSTR FeatureText);
	 CString				   GetAlgHeightDetectionMeasureModeText(int mode);
	 int					   FindAlgHeightDetectionMeasureModeByText(LPCTSTR FeatureText);
	 CString				   GetAlgHeightDetectionOutputTypeText(int mode);
	 int					   FindAlgHeightDetectionOutputTypeByText(LPCTSTR FeatureText);
	 //---------------------------------------------------------------------------------//
	 CString                   GetMarkFullName(unsigned int Index, LPCTSTR Name);//取得標記點全名
	 CString                   GetBarcodeFullName(unsigned int Index, LPCTSTR Name);//取得條碼全名
	 //---------------------------------------------------------------------------------//
	 CString                   GetBoxTowardText(BOX_TOWARD Toward);//取得框朝向文字
	 //---------------------------------------------------------------------------------//
	 BOX_SHAPE_MODE            FindBoxShapeModeByText(LPCTSTR ModeText);
	 CString                   GetBoxShapeModeText(BOX_SHAPE_MODE ShapeMode);//取得框外形文字
	 //---------------------------------------------------------------------------------//	 
	 CString                   GetAITempName(LPCTSTR Name, unsigned int LightIdx);//取得AI暫存檔名
	 //---------------------------------------------------------------------------------//	 
	 CString                   GetComponentFullName(unsigned int PanelIndex, unsigned int BoardIndex, LPCTSTR Name);//取得零件全名
	 CString                   GetComponentFullNameReverse(unsigned int PanelIndex, unsigned int BoardIndex, LPCTSTR Name);//取得零件全名
	 //---------------------------------------------------------------------------------//	 
	 CString                   GetFdSectionText(unsigned int Index);//取得定位點節點文字
	 CString                   GetFdFullName(unsigned int Index, LPCTSTR Name);//取得定位點全名
	 CString                   GetFdModelFolder(LPCTSTR FdFolder, int FdUniqueID);//取得定位點模組資料夾
	 CString                   GetFdPatternName(LPCTSTR ProjectFoder, unsigned int FdIdx, unsigned int PatIdx, bool bMask);//取得定位點樣板圖檔
	 //---------------------------------------------------------------------------------//
	 CString                   GetFieldSectionText(unsigned int Index);//取得區域節點文字
	 //---------------------------------------------------------------------------------//
	 CString                   GetFrameMaskName(LPCTSTR filename);//取得影像遮罩圖像名稱
	 CString                   GetFrameSectionText(unsigned int Index);//取得影像節點文字
	 CString                   GetFrameShortName(unsigned int Index, FRAME_TYPE FrameType, bool bMask, DISTRICT_ID DistrictID);//取得影像短名
	 //---------------------------------------------------------------------------------//
	 CString                   GetLandTypeText(LAND_TYPE Type);//取得特徵框樣式文字	 
	 bool                      BuildLandTypeCombox(CComboBox &Combox);//建立特徵框樣式列表	
	 //---------------------------------------------------------------------------------//
	 CString                   GetModelMaskText(int Mask);//取得模組遮罩文字
	 //---------------------------------------------------------------------------------//
	 LPCTSTR                   GetModelPadText() const; //取得模組部位文字-焊盤
	 LPCTSTR                   GetModelBodyText() const; //取得模組部位文字-本體
	 LPCTSTR                   GetModelLeadText() const; //取得模組部位文字-引腳電極
	 LPCTSTR                   GetModelLeadTipText() const; //取得模組部位文字-引腳前端
	 LPCTSTR                   GetModelLeadShoulderText() const; //取得模組部位文字-引腳根部
	 //---------------------------------------------------------------------------------//	 
	 LPCTSTR                   GetModelGroupAllText();//取得模組全群組的文字
	 CString                   GetModelTypeText(MODEL_TYPE Type);//取得模組樣式文字	 
	 bool                      GetModelTypePolarity(MODEL_TYPE Type);//取得模組是否有極性
	 CWndDefectItem            GetModelBasicDefectItem(MODEL_TYPE Type);//取得模組基本瑕疵項目
	 bool                      GetModelDefaultWndParam(MODEL_TYPE Type, CHIP_SIZE_MODE ChipSizeMode, TMODEL_DEFAULT_WND_PARAM &Param);
	 //---------------------------------------------------------------------------------//	 
	 bool                      BuildModelTypeCombox(CComboBox &Combox);	 
	 UINT                      GetModelTypeIcon(MODEL_TYPE ModelType, bool Small);	//取得模組樣式的圖示編號
	 CString                   GetModelParamFilename(LPCTSTR LibraryFolder, LPCTSTR ModelName);//命名::模組參數檔名
	 CString                   GetModelBKImageFilename(LPCTSTR ModelFolder, LPCTSTR ModelName, int UniFrameIdx);//命名::模組底圖名稱
	 //---------------------------------------------------------------------------------//
	 int                       GetProjectSpaceToGrayRatioModeCount() const;
	 bool                      BuildProjectMapIndexCombox(CComboBox &Combox);//專案底圖編號模式
	 bool                      BuildProjectMapScaleModeCombox(CComboBox &Combox);//專案底圖縮放比例模式
	 bool                      BuidlProjectSpaceToGrayRatioCombox(CComboBox &Combox);//取得高度轉灰階比例
	 bool                      BuildProjectColorGroupIDList(CComboBox &Combox);//建立專案彩色群組編號列表
	 bool                      BuidlProjectDlpLedColorCombox(CComboBox &Combox);//建立專案DLP-LED顏色
	 bool                      BuildProjectFieldSizeModeCombox(CComboBox &Combox);//建立專案區域尺寸模式
	 CString                   GetProjectTempFilename(LPCTSTR Filename);//形成專案暫存檔案名稱
	 CString                   GetProjectOfflineFolderName(LPCTSTR Folder);//取得專案離線資料夾
	 CString                   GetProjectMapFileName(LPCTSTR ProjectFolder);//取得專案底圖檔案名稱	 	 
	 CString                   GetProjectGrrSigmaItemFileName(LPCTSTR ProjectFolder);//取得專案Grr標準差項目檔案名稱
	 CString                   GetProjectOfflineFdName(LPCTSTR Folder, DISTRICT_ID DistrictID);//取得專案離線定位點檔名
	 CString                   GetProjectOfflineMapName(LPCTSTR Folder, DISTRICT_ID DistrictID);//取得專案離線底圖檔名
	 CString                   GetProjectOfflineFileName(LPCTSTR Folder, DISTRICT_ID DistrictID);//取得專案離線檔案名稱	 	 
	 CString                   GetProjectOfflineLocalName(LPCTSTR Folder, DISTRICT_ID DistrictID);//取得專案局部離線名稱	 	 
	 CString                   GetProjectMapImageName(LPCTSTR ProjectFolder, unsigned int MapIndex, bool bSmallMap);//取得專案底圖檔案名稱	 
	 CString                   GetProjectMapImageName(LPCTSTR Folder, LPCTSTR MainName, int MapIdx, bool bSmall=false, LPCTSTR ExtName=_T("JPG"));//取得專案底圖檔案名稱
	 CString                   GetProjectMarkFileName(LPCTSTR ProjectFolder);//取得專案底標記檔案名稱	 
	 CString                   GetProjectMapMaskFileName(LPCTSTR ProjectFolder);//取得專案底圖遮罩檔案名稱	 
	 CString                   GetProjectFdFolderName(LPCTSTR ProjectFolder);//取得專案定位點資料夾
	 CString                   GetProjectSystemFolderName(LPCTSTR ProjectFolder);//取得專案系統資料夾
	 CString                   GetProjectLibraryFolderName(LPCTSTR ProjectFolder);//取得專案資料庫資料夾
	 CString                   GetProjectOperLogFolderName(LPCTSTR ProjectFolder);//取得專案操錯訊息資料夾	 
	 CString                   GetProjectFieldMarkFolderName(LPCTSTR ProjectFolder);//取得專案區域定位資料夾
	 CString                   GetProjectPartLibraryFolderName(LPCTSTR ProjectFolder);//取得專案元件庫資料夾	 
	 CString                   GetProjectSpcResultFolder(LPCTSTR SpcProjectFolder, SAVE_SPC_FILE_MODE Mode);//取得專案維修站結果資料夾
	 //---------------------------------------------------------------------------------//
	 CString                   GetWndIndexText(size_t index);//取得檢測框引數名稱 
	 CString                   GetWndLogicTypeText(WND_LOGIC_TYPE Type);//取得檢測框邏輯樣式文字
	 WND_LOGIC_TYPE            FindWndLogicTypeByText(LPCTSTR Text);//依據文字尋找檢測框邏輯樣式
	 //---------------------------------------------------------------------------------//
	 CString                   GetWndFollowModeText(WND_FOLLOW_MODE Mode);//取得檢測框跟隨移動文字
	 WND_FOLLOW_MODE           FindWndFollowModeByLinkModeText(LPCTSTR LinkText);//依據文字取得跟隨移動
	 WND_FOLLOW_MODE           GetWndFollowModeByWndDefectID(WND_DEFECT_ID DefectID);//依據瑕疵代碼取得跟隨移動
	 //---------------------------------------------------------------------------------//
	 CString                   GetWndRgnLinkModeText(WND_RGN_LINK_MODE Mode);//取得檢測框範圍連動文字
	 WND_RGN_LINK_MODE         FindWndRgnLinkModeByLinkModeText(LPCTSTR LinkText);//取得範圍連動	 
	 //---------------------------------------------------------------------------------//
	 CString                   GetWndSyncMoveModeText(WND_SYNC_MOVE_MODE SyncMoveMode);//取得檢測同動模式
	 WND_SYNC_MOVE_MODE        FindWndSyncMoveModeByText(LPCTSTR SyncMoveText);//取得檢測框同動模式	 
	 //---------------------------------------------------------------------------------//
	 CString                   GetWndConstrainModeText(WND_CONSTRAIN_MODE ConstrainMode);//取得檢測侷限模式文字
	 WND_CONSTRAIN_MODE        FindWndConstrainModeByText(LPCTSTR ConstrainText);//取得檢測框侷限模式
	 //---------------------------------------------------------------------------------//
	 ALG_AI_MODEL_ID           FindAIModelIDByText(LPCTSTR IDText);//取得AI模型代碼
	 CString                   GetAIModelIDText(ALG_AI_MODEL_ID AIModelID);//取得AI模型代碼文字	 
	 //---------------------------------------------------------------------------------//	 
	 CString                   GetWndDefectIDText(WND_DEFECT_ID DefectID);//取得檢測框瑕疵代碼文字
	 WND_DEFECT_ID             FindWndDefectIDByDefectText(LPCTSTR DefectText);//取得檢測框瑕疵代碼	
	 int                       GetWndDefectIDOrder(WND_DEFECT_ID DefectID);//取得檢測框瑕疵代碼檢測次序
	 bool                      CheckWndDefectIDCanToAlign(WND_DEFECT_ID DefectID);//確認檢測框瑕疵代碼可以補償座標	 	 	 
	 //---------------------------------------------------------------------------------//
	 CString                   GetBasePlaneProcTypeText(BASE_PLANE_PROC_TYPE eType);//取得基準面程序樣式文字
	 bool                      BuildBasePlaneProcCombox(CComboBox &Combox);//建立基準面程序樣式列表
	 //---------------------------------------------------------------------------------//
	 LPCTSTR                   GetBasePlaneAutoRegionText(BASE_PLANE_AUTO_REGION_MODE eMode);//取得基準面自動區域文字
	 bool                      BuildBasePlaneAutoRegionCombox(CComboBox &Combox);//建立基準面自動區域列表
	 //---------------------------------------------------------------------------------//
	 LPCTSTR                   GetBasePlaneBodyOutsideText(BASE_PLANE_BODY_OUTSIDE_MODE eMode);//取得基準面本體外圍文字
	 bool                      BuildBasePlaneBodyOutsideCombox(CComboBox &Combox);//建立基準面本體外圍列表
	 //---------------------------------------------------------------------------------//
	 CString                   GetCalcBasePlaneModeText(CALC_BASE_PLANE_MODE eMode);//取得計算基準面模式文字
	 bool                      BuildCalcBasePlaneCombox(CComboBox &Combox);//建立基準面平面列表
	 bool                      BuildBasePlaneTowardCombox(CComboBox &Combox);//建立基準面平面朝向
	 //---------------------------------------------------------------------------------//
	 CString                   GetLight3DCastIDText(LIGHT_3D_CAST_ID CastID);//取得投光文字
	 CString                   GetDLPPhaseModeText(int Mode);//取得相位模式的文字
	 CString                   GetDLPBitPosText(int index);//取得位元位置文字
	 CString                   GetDLPPatternTriggerTypeText(int TriggerType);//取得樣板觸發樣式文字
	 CString                   GetDLPPatternLEDColorText(int LEDIndex);//取得樣版LED編號文字
	 bool                      CalcDLPBitPosRange(int BitDepth, int PatNum, int &first, int &end);//計算位元位置與範圍	
	 //---------------------------------------------------------------------------------//	 
	 bool                      BuildDLPLEDCurrentIDCombox(CComboBox &Combox);//建立LED電流編號列表
	 bool                      BuildDLPOperationModeCombox(CComboBox &Combox);//建立投射編號視窗	
	 bool                      BuildDLPPhaseLEDColorCombox(CComboBox &Combox, bool bDebug);//建立相位LED燈源視窗
	 bool                      BuildDLPPatternExpNumCombox(CComboBox &Combox);//建立樣板曝光數量
	 bool                      BuildDLPPatternLEDColorCombox(CComboBox &Combox);//建立樣板LED燈源視窗
	 bool                      BuildDLPPatternFlashIndexCombox(CComboBox &Combox);//建立樣板圖像編號視窗
	 bool                      BuildDLPPatternBitDepthCombox(CComboBox &Combox);//建立樣板位元深度視窗
	 bool                      BuildDLPPatternBitRangeCombox(int BitDepth, CComboBox &Combox);//建立樣板位元區間視窗	
	 bool                      BuildDLPPatternSequenceModeCombox(CComboBox &Combox);//建立樣板序列模式視窗
	 bool                      BuildDLPPatternTriggerTypeCombox(CComboBox &Combox);//建立樣板觸發樣式視窗	
	 bool                      BuildDLPPatternSouceCombox(CComboBox &Combox);//建立樣板來源視窗
	 bool                      BuildDLPSequenceTriggerModeCombox(CComboBox &Combox);//建立序列觸發視窗	
	 //---------------------------------------------------------------------------------//	 
	 LPCTSTR                   GetFdText_NG() const;//取得定位點文字-瑕疵
	 LPCTSTR                   GetBarcodeText_NG() const;//取得條碼文字-瑕疵
	 //---------------------------------------------------------------------------------//	 
	 LPCTSTR                   GetProjectColorGroupText_Pad() const;//色彩群組文字-銅箔
	 LPCTSTR                   GetProjectColorGroupText_Body() const;//色彩群組文字-本體
	 LPCTSTR                   GetProjectColorGroupText_Void() const;//色彩群組文字-空焊
	 LPCTSTR                   GetProjectColorGroupText_Board() const;//色彩群組文字-基板
	 LPCTSTR                   GetProjectColorGroupText_Solder() const;//色彩群組文字-焊錫
	 LPCTSTR                   GetProjectColorGroupText_Others() const;//色彩群組文字-其它
	 CString                   GetProjectColorGroupText(size_t Index);//取得專案顏色群組文字
	 //---------------------------------------------------------------------------------//
	 CString                   GetMotionAccTimeAdjustText(ACC_TIME_ADJUST_MODE Mode);//取得軸控加速度調整文字	 
	 //---------------------------------------------------------------------------------//
	 bool                      BuildLEDCurrentCaliModeCombox(CComboBox &Combox);
	 bool                      BuildSliceFuncModeCombox(CComboBox &Combox, bool Include2D, bool Include3D);
	 bool                      BuildSystemSliceParamCombox(CComboBox &Combox, bool IncludeNone, bool IncludeDLP, bool bUseCaliMode);
	 //---------------------------------------------------------------------------------//
	 CString                   GetFrameTypeName(FRAME_TYPE type);
	 bool                      BuildSystemFrameParamTypeCombox(CComboBox &Combox);//建立Frame樣式列表		
	 bool                      BuildSystemFrameParamCombox(CComboBox &Combox, bool IncludeNone, bool Include3D);//建立Frame列表		
	 //---------------------------------------------------------------------------------//
	 CString                   GetBarcodeDeviceName(BARCODE_DEVICE_TYPE type);
	 bool                      BuildBarcodeDeviceIDCombox(CComboBox &Combox, bool IncDisable);
	 bool                      BuildBarcodeDeviceTypeCombox(CComboBox &Combox);
	 CString                   LoadMultiLanguageString_BarcodeDevice(LPCTSTR KeyName, LPCTSTR Default);//載入多國語系
	 CString                   LoadMultiLanguageString_BarcodeHandHeld(LPCTSTR KeyName, LPCTSTR Default);//載入多國語系
	 //---------------------------------------------------------------------------------//
	 bool                      BuidlSwitchModelItemCombox(CComboBox &Combox);//切換模組項目列表
	//---------------------------------------------------------------------------------//	 
	 bool                      BuidlSpaceMergeModeCombox(CComboBox &Combox);//空間合併模式列表	 
	 bool                      BuidlSpaceMergeBestModeCombox(CComboBox &Combox);//空間合併最可靠模式列表
	 bool                      BuidlSpaceMergeIntensityModeCombox(CComboBox &Combox);//空間合併亮度模式列表
	 bool                      BuildCastSpaceFilterModeCombox(CComboBox &Combox);//投光後空間濾波(Cuda)模式列表
	 bool                      BuidlSpaceNoiseDefineModeCombox(CComboBox &Combox);//空間雜訊定義模式列表	 
	 bool                      BuidlSpaceNoiseFilterModeCombox(CComboBox &Combox);//空間雜訊過濾模式列表
	 bool                      BuildPhaseHeightFactorNumCombox(CComboBox &Combox);//相位高度係數第幾個列表	 	 
	 bool                      BuildPhaseConvertHeightModeCombox(CComboBox &Combox);//相位轉高度的模式
	 bool                      BuildSpaceHeightCorrectModelCombox(CComboBox &Combox);//高度校正模式列表
	//---------------------------------------------------------------------------------//
	 CString                   GetMESStatusText(int StatusID);
	 //---------------------------------------------------------------------------------//
	 CString                   GetPartGroupTargetModeText(PART_GROUP_TARGET_MODE Mode);
	 CString                   GetPartGroupColinearityeModeText(PART_GROUP_COLINEARITY_MODE Mode);
	 //---------------------------------------------------------------------------------//
	 CString                   GetOnlineAutoStopBySpecTimeText(__int64 Time);
	 //---------------------------------------------------------------------------------//
	 CString                   GetMESContactSoftwareName(int nContact);//取得與MES對接軟體名稱
	 //---------------------------------------------------------------------------------//
	 bool                      BuildDataModeLevelList(std::vector<int> &List);//建立資料模型等級列表
	 //---------------------------------------------------------------------------------//	 
	 CString                   GetAS608StatusText(AS608_STATUS status);
	 CString                   GetAS608CommandText(AS608_COMMAND command);
	 CString                   GetFingerPrintConfigFilename() const;//指紋
	 //---------------------------------------------------------------------------------//
};
//-------------------------------------------------------------------------------------//
extern CAOIDataDefine AOIDataDefine;
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_AOIDATADEFINE_H__57FD96E3_C17E_482F_B397_2B50A54A87CA__INCLUDED_)
