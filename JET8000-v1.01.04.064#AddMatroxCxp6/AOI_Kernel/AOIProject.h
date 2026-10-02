// AOIProject.h: interface for the CAOIProject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AOIPROJECT_H__B9BED84A_8D4E_47E2_9E23_98908A354313__INCLUDED_)
#define AFX_AOIPROJECT_H__B9BED84A_8D4E_47E2_9E23_98908A354313__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#define PROJECT_COLOR_ID_PAD_BEGIN          0
#define PROJECT_COLOR_ID_PAD_END            7
#define PROJECT_COLOR_ID_VOID_BEGIN         8
#define PROJECT_COLOR_ID_VOID_END          15
#define PROJECT_COLOR_ID_BODY_BEGIN        16
#define PROJECT_COLOR_ID_BODY_END          23
#define PROJECT_COLOR_ID_BOARD_BEGIN       24
#define PROJECT_COLOR_ID_BOARD_END         31
#define PROJECT_COLOR_ID_SOLDER_BEGIN      32
#define PROJECT_COLOR_ID_SOLDER_END        39
#define PROJECT_COLOR_ID_OTHERS_BEGIN      40
#define PROJECT_COLOR_ID_OTHERS_END        47
#define MAX_PROJECT_COLOR_COUNT            48//最多48個顏色
//-------------------------------------------------------------------------------------//
#define      PROJECT_PART_MASK_BODY         0x01//專案零件遮罩-本體
#define      PROJECT_PART_MASK_LAND_RGN     0x02//專案零件遮罩-焊盤區域
#define      PROJECT_PART_MASK_ALL          0x0F//專案零件遮罩-全部

#define      PROJECT_CODE_MASK_BODY         0x10//專案條碼遮罩-本體
#define      PROJECT_CODE_MASK_LAND_RGN     0x20//專案條碼遮罩-焊盤區域
#define      PROJECT_CODE_MASK_ALL          0xF0//專案條碼遮罩-全部

#define      PROJECT_OBJ_MASK_ALL           0xFF//專案物件遮罩-全部
//-------------------------------------------------------------------------------------//
#define PROJECT_CREATE_FULL_MAP_COMPONENT_V1     1//建立專案底圖零件模式-1
#define PROJECT_CREATE_FULL_MAP_COMPONENT_V2     2//建立專案底圖零件模式-2
//-------------------------------------------------------------------------------------//
#define CPK_CHART_VALUE_MAX_COUNT            5//Cpk圖表數據最多數量-5筆//A, B軌道各自5筆
//-------------------------------------------------------------------------------------//
//檢測涵蓋率檔案模式
#define TEST_COVERAGE_FILE_MODE_NORMAL           1//標準模式
#define TEST_COVERAGE_FILE_MODE_PERCENT          2//百分比模式
//-------------------------------------------------------------------------------------//
#include <vector>
#include <thread>
#include "AOIObj.h"
#include "AOIPanel.h"
#include "AOIFov.h"
#include "AOISlice.h"
#include "AOIFrame.h"
#include "AOIField.h"
#include "JetIniFile.h"
#include "Barcode_Handheld.h"
//-------------------------------------------------------------------------------------//
class CSPIPad;
class CSPIComponent;
class CAOIFileIO;
//-------------------------------------------------------------------------------------//
enum CHANGE_COMPARE_PARAM_MODE//變更零件參數模式
{
	CHANGE_COMPARE_PARAM_NONE        = 0,//無動作
	CHANGE_COMPARE_PARAM_TEST        = 1,//要檢測
	CHANGE_COMPARE_PARAM_BYPASS      = 2,//不檢測
	CHANGE_COMPARE_PARAM_RETURN
};
//-------------------------------------------------------------------------------------//
typedef struct tagCopyFolder
{
	CString     SrcFolder;
	CString     DstFolder;
	bool        bFileMode;//檔案模式
	tagCopyFolder()
	{
		bFileMode = false;
	}
} TCopyFolder, *PCopyFolder;
//-------------------------------------------------------------------------------------//
typedef struct tagProjectStatisticBackup//專案統計備份
{
	bool                        ProjectResultYieldOnAOI;//專案檢測良率-開始
	bool                        ProjectResultYieldOnARS;//專案檢測良率-開始
	TTestResult                 ProjectResultAlarm;//專案檢測紀錄-警報
	TTestResult                 ProjectResultCurrent;//專案檢測紀錄-目前
	TTestResult                 ProjectResultLatest;//專案檢測紀錄-最近
	TTestResult                 ProjectResultLatest_LA;//專案檢測紀錄-A軌最近
	TTestResult                 ProjectResultLatest_LB;//專案檢測紀錄-B軌最近
	TTestResult                 ProjectResultStatistic;//專案檢測紀錄-統計
	TTestResult                 ProjectResultStatistic_LA;//專案檢測紀錄-A軌統計
	TTestResult                 ProjectResultStatistic_LB;//專案檢測紀錄-B軌統計
	TTestResult                 ProjectResultLatest_ARS;//專案檢測紀錄-最近
	TTestResult                 ProjectResultLatest_ARS_LA;//專案檢測紀錄-A軌最近
	TTestResult                 ProjectResultLatest_ARS_LB;//專案檢測紀錄-B軌最近
	TTestResult                 ProjectResultStatistic_ARS;//專案檢測紀錄-統計
	TTestResult                 ProjectResultStatistic_ARS_LA;//專案檢測紀錄-A軌統計
	TTestResult                 ProjectResultStatistic_ARS_LB;//專案檢測紀錄-B軌統計
	std::vector<TCpkItem>       ProjectResultCpkList_LA;//專案檢測紀錄-A軌CPK列表	
	std::vector<TCpkItem>       ProjectResultCpkList_LB;//專案檢測紀錄-B軌CPK列表
	std::vector<CAOIComponent*> ProjectComponentList;
} TProjectStatisticBackup, *PProjectStatisticBackup;
//-------------------------------------------------------------------------------------//
class CAOIProject : public CAOIObj  
{
	//---------------------------------------------------------------------------------//	
	DECLARE_DYNAMIC(CAOIProject)
	//---------------------------------------------------------------------------------//	
public:
	//---------------------------------------------------------------------------------//	
	static bool                DeleteProjectTempFile(LPCTSTR filename);//刪除暫存檔案
	//---------------------------------------------------------------------------------//		
	static CString             ObtainProjectParameterDescText(PROJECT_PARAM_ID ParamID);//取得專案參數的說明文字		
	static bool                SetProjectParameterStringByID(PROJECT_PARAM_ID ParamID, TProjectParameter &Param, LPCTSTR String);//設定專案參數
	static bool                GetProjectParameterStringByID(PROJECT_PARAM_ID ParamID, const TProjectParameter &Param, CString &String);//取得專案參數
	//---------------------------------------------------------------------------------//	
private:
	//---------------------------------------------------------------------------------//			
	unsigned int               m_ProjectOperLogIndex;//專案操作紀錄引數
	//---------------------------------------------------------------------------------//
	CJetIniFile                m_ProjectIniFile;//專案用的INI寫檔用	
	CString                    m_ProjectFileName;//專案名稱
	CString                    m_ProjectShowName;//專案名稱
	CString                    m_ProjectShowMainName;//專案名稱	
	CString                    m_ProjectFdFolder;//專案的定位點資料夾
	CString                    m_ProjectLibraryFolder;//專案的資料庫資料夾
	CString                    m_ProjectPartLibraryFolder;//專案的零件資料庫資料夾
	CString                    m_ProjectOfflineFolder;//專案的離線編輯資料夾
	CString                    m_ProjectServerLibraryFolder;//專案的伺服器群組資料庫資料夾
	bool                       m_ProjectIsClosing;//專案正在關閉
	bool                       m_ProjectReloadingFile;//專案正在載入檔案
	bool                       m_ProjectUseLocalFolder;//專案使用本機資料夾
	bool                       m_ProjectMultiDistrictMode;//專案使用多段檢測模式
	bool                       m_ProjectXBoardUseRatioMode;//專案X板使用比例模式
	bool                       m_ProjectSaveOfflineImageFiles;//專案儲存離線圖檔
	time_t                     m_ProjectSavedDateTime;//專案存檔日期時間
	time_t                     m_ProjectServerSavedDateTime;//專案伺服器存檔日期時間
	time_t                     m_ProjectServerProjectSavedDateTime;//專案伺服器專案存檔日期時間	
	//---------------------------------------------------------------------------------//		
	CString                    m_ProjectAIImageFolder;//專案AI圖檔資料夾	
	CString                    m_ProjectTestTrackFolder;//專案檢測追蹤資料夾
	//---------------------------------------------------------------------------------//		
	CString                    m_ProjectSpcFolder;//專案Spc資料夾
	CString                    m_ProjectSpcFileFolder;//專案Spc檔案資料夾
	CString                    m_ProjectSpcImageFolder;//專案Spc圖檔資料夾	
	CString                    m_ProjectSpcImageFolderLocal;//專案Spc圖檔資料夾-本機
	CString                    m_ProjectSpcResultFolder;//專案Spc結果資料夾-ARS給AOI
	bool                       m_ProjectSpcFileSaveEnabled;//專案儲存Spc檔案啟用	
	CString                    m_ProjectOfflineFdFilename;//專案離線定位點檔名
	CString                    m_ProjectOnlineTuningFolder;//專案線上調機資料夾	
	CString                    m_ProjectOnlineTuningDateTime;//專案線上調機日期時間
	CString                    m_ProjectOnlineBarcodeFolder;//專案線上條碼資料夾	
	CString                    m_ProjectOnlineOfflineFolder;//專案線上離線編程資料夾
	CString                    m_ProjectDebugFolderDropOut;//專案除錯資料夾-拋件
	CString                    m_ProjectDebugFolderScratch;//專案除錯資料夾-刮傷	
	CString                    m_ProjectDebugFolderDimonsion;//專案除錯資料夾-尺寸
	CString                    m_ProjectInspectionResultFileTmp;//專案檢測結果檔案-暫存
	//---------------------------------------------------------------------------------//	
	FIELD_BUILD_MODE           m_ProjectFieldBuildMode;//區域建立(配置)模式
	FIELD_BUILD_MODE           m_ProjectInspectionFieldBuildMode;//檢測建立(配置)模式
	FIELD_BUILD_AREA_MODE      m_ProjectFieldBuildAreaMode;//區域建立(配置)面積模式
	FIELD_BUILD_AREA_MODE      m_ProjectInspectionFieldBuildAreaMode;//檢測建立(配置)面積模式
	OFFLINE_FILE_MODE          m_ProjectOfflineFileMode;//離線檔案模式
	OFFLINE_IMAGE_SCOPE        m_ProjectOfflineImageScope;//離線影像範疇
	CString                    m_ProjectProgramOfflineDA;//編程離線檔案
	CString                    m_ProjectProgramOfflineDB;//編程離線檔案
	CString                    m_ProjectProgramOfflineFolder;//編程離線檔案	
	CString                    m_ProjectInspectionOfflineFolder;//檢測離線檔案
	CString                    m_ProjectInspectionOfflineFolderDefault;//檢測離線檔案
	TPOINT2D                   m_ProjectProgramOfflineStartPos;//離線編程的檢測起始位置
	TPOINT2D                   m_ProjectInspectionOfflineStartPos;//離線檢測的檢測起始位置
	double                     m_ProjectProgramOfflineFocusOffsetZ;//離線編程的焦距位置Z
	double                     m_ProjectInspectionOfflineFocusOffsetZ;//離線檢測的焦距位置Z
	//---------------------------------------------------------------------------------//	
	unsigned int               m_ProjectIndex;//專案引數
	bool                       m_ProjectDeleted;//專案是否要刪除
	bool                       m_ProjectSelected;//專案是否被選取到	
	bool                       m_ProjectFdModified;//專案是否變更定位點	
	bool                       m_ProjectAlarmStop;//專案是否警報停機
	bool                       m_ProjectAlarmDefect;//專案是否警報瑕疵	
	bool                       m_ProjectIsException;//專案異常	
	bool                       m_ProjectFdException;//專案定位點異常	
	double                     m_ProjectSpaceToGrayRatio;//專案高度轉灰階比例	
	bool                       m_ProjectNeedGrabFdForTuning;//專案需要取像定位點-調機
	bool                       m_ProjectNeedCheckDistrictPos;//專案需要確認段落位置
	bool                       m_ProjectPartialCopyLibraryFolder;//專案局部分複製資料庫資料夾
	//---------------------------------------------------------------------------------//
	SAVE_SPC_PART_IMAGE_MODE   m_ProjectSaveSpcPartImageMode;//專案是否儲存Spc零件圖檔
	CString                    m_ProjectSpcPartImageFolder;//專案Spc零件圖檔資料夾
	//---------------------------------------------------------------------------------//
	LANE_ID                    m_ProjectActLaneID;//專案目前所在的軌道	
	TASK_MODE                  m_ProjectActTaskMode;//專案目前任務模式
	DISTRICT_ID                m_ProjectActDistrictID;//專案目前檢測區域//兩段式檢測
	bool                       m_ProjectLaneEnable_LA;//專案在軌道A啟用
	bool                       m_ProjectLaneEnable_LB;//專案在軌道B啟用
	bool                       m_ProjectTestGrabFinish;//專案檢測取像結束
	bool                       m_ProjectTestCalcFinish;//專案檢測計算結束
	bool                       m_ProjectTestAllFinish;//專案檢測結束
	bool                       m_ProjectTestReportSaved;//專案檢測報告已儲存
	bool                       m_ProjectOnlineTuningEnable;//專案線上調機啟用
	bool                       m_ProjectOnlineTuningEnableUI;//專案線上調機啟用-介面
	size_t                     m_ProjectOnlineTuningSavedCount;//專案線上調機儲存次數
	bool                       m_ProjectHasTestInOneCycle_LA;//在一次檢測是否已經檢測
	bool                       m_ProjectHasTestInOneCycle_LB;//在一次檢測是否已經檢測	
	bool                       m_ProjectConveyerPreRunRunning;//軌道提前運轉啟用中	
	bool                       m_ProjectLoadInspectionResultFile;//載入檢測結果檔案	
	//---------------------------------------------------------------------------------//		
	bool                       m_ProjectHasModified; //專案是否被修改過且尚未保存
	//---------------------------------------------------------------------------------//		
	CString                    m_ErrorString;//錯誤訊息	
	CString                    m_ProjectAlarmString;//專案警報文字
	ONLINE_STATE_MODE          m_ProjectOnlineState;//線上檢測狀態
	INSPECTION_STATE_MODE      m_ProjectInspectionState;//檢測狀態
	//---------------------------------------------------------------------------------//	
	BOARD_FD_GRAB_MODE         m_ProjectBoardFdGrabMode;//專案單板定位點取像模式
	//---------------------------------------------------------------------------------//			
	bool                       m_ProjectIsGetBarcode;//專案是否設定條碼
	std::wstring               m_ProjectBarcode;//專案條碼	
	std::wstring               m_ProjectBarcodeLast;//專案條碼-上一筆	
	std::wstring               m_ProjectTrayBarcode;//專案載具條碼	
	std::wstring               m_ProjectCoverBarcode;//專案蓋板條碼	
	std::wstring               m_ProjectBarcodeBackup;//專案條碼-備份
	std::wstring               m_ProjectBarcodeDefault;//專案條碼預設
	unsigned int               m_ProjectBarcodeDeviceIndex;//專案條碼機編號
	unsigned int               m_ProjectBarcodeDeviceCodeIndex;//專案條碼機第幾碼
	bool                       m_ProjectBarcodeDeviceUsedList[MAX_BARCODE_DEVICE_COUNT];		
	BARCODE_BELONG_MODE        m_ProjectBarcodeBelongMode;//專案條碼屬於模式
	//---------------------------------------------------------------------------------//
	CTime                      m_ProjectInspectedDateTime;//專案檢測開始日期時間
	DWORD                      m_ProjectTickCountStamp;//專案時脈-戳記-每次檢測到相同函式的時間標記
	DWORD                      m_ProjectTickCountPCBIn;//專案時脈-進板
	DWORD                      m_ProjectTickCountPCBExit;//專案時脈-離板
	//---------------------------------------------------------------------------------//
	DWORD                      m_ProjectTickCountTestFirst_DA;//專案時脈-檢測開始
	DWORD                      m_ProjectTickCountFdFinish_DA;//專案時脈-定位點結束
	DWORD                      m_ProjectTickCountGrabFinish_DA;//專案時脈-取像結束
	DWORD                      m_ProjectTickCountCalcFinish_DA;//專案時脈-計算結束
	DWORD                      m_ProjectTickCountTestFinish_DA;//專案時脈-檢測結束
	//---------------------------------------------------------------------------------//
	DWORD                      m_ProjectTickCountTestFirst_DB;//專案時脈-檢測開始
	DWORD                      m_ProjectTickCountFdFinish_DB;//專案時脈-定位點結束
	DWORD                      m_ProjectTickCountGrabFinish_DB;//專案時脈-取像結束
	DWORD                      m_ProjectTickCountCalcFinish_DB;//專案時脈-計算結束
	DWORD                      m_ProjectTickCountTestFinish_DB;//專案時脈-檢測結束
	//---------------------------------------------------------------------------------//
	bool                       m_ProjectResultYieldOnAOI;//專案檢測良率-開始
	bool                       m_ProjectResultYieldOnARS;//專案檢測良率-開始
	TTestResult                m_ProjectResultAlarm;//專案檢測紀錄-警報
	TTestResult                m_ProjectResultCurrent;//專案檢測紀錄-目前	
	TTestResult                m_ProjectResultLatest;//專案檢測紀錄-最近	
	TTestResult                m_ProjectResultLatest_LA;//專案檢測紀錄-A軌最近	
	TTestResult                m_ProjectResultLatest_LB;//專案檢測紀錄-B軌最近	
	TTestResult                m_ProjectResultStatistic;//專案檢測紀錄-統計
	TTestResult                m_ProjectResultStatistic_LA;//專案檢測紀錄-A軌統計
	TTestResult                m_ProjectResultStatistic_LB;//專案檢測紀錄-B軌統計
	TTestResult                m_ProjectResultLatest_ARS;//專案檢測紀錄-最近_ARS
	TTestResult                m_ProjectResultLatest_ARS_LA;//專案檢測紀錄-A軌最近_ARS
	TTestResult                m_ProjectResultLatest_ARS_LB;//專案檢測紀錄-B軌最近_ARS
	TTestResult                m_ProjectResultStatistic_ARS;//專案檢測紀錄-統計_ARS
	TTestResult                m_ProjectResultStatistic_ARS_LA;//專案檢測紀錄-A軌統計_ARS
	TTestResult                m_ProjectResultStatistic_ARS_LB;//專案檢測紀錄-B軌統計_ARS
	std::vector<TCpkItem>      m_ProjectResultCpkList_LA;//專案檢測紀錄-A軌CPK列表	
	std::vector<TCpkItem>      m_ProjectResultCpkList_LB;//專案檢測紀錄-B軌CPK列表
	std::vector<TTestResult>   m_ProjectResultListAOI;//專案檢測結果列表-AOI
	std::vector<TTestResult>   m_ProjectResultListARS;//專案檢測結果列表-ARS
	std::vector<TTop10Node>    m_ProjectTop10ListModel;//前十大不良-模組
	std::vector<TTop10Node>    m_ProjectTop10ListModel_LA;//前十大不良-模組
	std::vector<TTop10Node>    m_ProjectTop10ListModel_LB;//前十大不良-模組
	std::vector<TTop10Node>    m_ProjectTop10ListPartNumber;//前十大不良-料號
	std::vector<TTop10Node>    m_ProjectTop10ListPartNumber_LA;//前十大不良-料號
	std::vector<TTop10Node>    m_ProjectTop10ListPartNumber_LB;//前十大不良-料號
	//---------------------------------------------------------------------------------//	
	unsigned int               m_ProjectActFdIndex;//操作的定位點序號
	unsigned int               m_ProjectActMarkIndex;//操作的特徵點序號
	unsigned int               m_ProjectActPanelIndex;//操作的整板序號
	unsigned int               m_ProjectActBoardIndex;//操作的單板序號
	unsigned int               m_ProjectActBarcodeIndex;//操作的條碼序號
	unsigned int               m_ProjectActComponentIndex;//操作的零件序號
	unsigned int               m_ProjectActComponentWindowIndex;//操作的零件檢測框序號
	UUID                       m_ProjectActModelBoxUuid;//物件框的唯一碼
	UUID                       m_ProjectActModelWndUuid;//檢測框的唯一碼
	UUID                       m_ProjectActModelLandUuid;//特徵框的唯一碼
	//---------------------------------------------------------------------------------//	
	unsigned int               m_ProjectRibbonFdIndex;//給Ribbon操作的定位點序號 
	unsigned int               m_ProjectRibbonPanelIndex;//給Ribbon操作的整板序號
	unsigned int               m_ProjectRibbonBoardIndex;//給Ribbon操作的單板序號
	unsigned int               m_ProjectRibbonBarcodeIndex;//給Ribbon操作的條碼序號
	unsigned int               m_ProjectRibbonComponentIndex;//給Ribbon操作的零件序號	
	bool                       m_ProjectRibbonComponentReBuild;////給Ribbon操作的零件重建
	//---------------------------------------------------------------------------------//	
	int                         m_ProjectFdMaxUniqueID;//專案定位點最大的唯一碼
	int                         m_ProjectMarkMaxUniqueID;//專案特徵點最大的唯一碼
	int                         m_ProjectGroupMaxUniqueID;//專案群組最大的唯一碼
	int                         m_ProjectBarcodeMaxUniqueID;//專案條碼最大的唯一碼
	int                         m_ProjectComponentMaxUniqueID;//專案零件最大的唯一碼
	//---------------------------------------------------------------------------------//
	bool                        m_ProjectProgramFieldDoNotSave;//專案編程檔案不要儲存
	bool                        m_ProjectProgramFieldBufferReleased;//專案編程區域記憶體釋放
	bool                        m_ProjectInspectionFieldBufferReleased;//專案檢測區域記憶體釋放
	//---------------------------------------------------------------------------------//
	TProjectParameter           m_ProjectParameter;
	std::vector<CAOIFd*>        m_ProjectFdPtrList;//專案定位點指標列表	
	std::vector<CAOIFd*>        m_ProjectFdSortedList;//專案定位點排序列表	
	std::vector<CAOIRgn*>       m_ProjectMapRgnPtrList;//專案底圖區域指標列表
	std::vector<CAOIPanel*>     m_ProjectPanelPtrList;//專案整板指標列表
	std::vector<CAOIBoard*>     m_ProjectBoardPtrList;//專案單板指標列表
	std::vector<CAOIField*>     m_ProjectFieldPtrList;//專案區域指標列表-編程或檢測的備份列表
	std::vector<CAOIField*>     m_ProjectProgramFieldPtrList;//專案編程區域指標列表
	std::vector<CAOIField*>     m_ProjectInspectionFieldPtrList;//專案檢測區域指標列表	
	std::vector<CAOIField*>     m_ProjectInspectionPartFieldPtrList;//專案檢測局部區域指標列表	
	std::vector<CAOIMark*>      m_ProjectMarkPtrList;//專案特徵點指標列表
	std::vector<CAOIBarcode*>   m_ProjectBarcodePtrList;//專案軟體條碼指標列表
	std::vector<CAOIPartGroup*> m_ProjectPartGroupPtrList;//專案零件群組指標列表
	std::vector<CAOIComponent*> m_ProjectComponentPtrList;//專案零件指標列表
	std::vector<CAOIComponent*> m_ProjectDropOutPartList;//專案拋件指標列表
	std::vector<CAOIComponent*> m_ProjectScratchPartList;//專案刮傷指標列表
	std::vector<CAOIComponent*> m_ProjectPartDimensionList;//專案尺寸指標列表
	std::vector<CAOIComponent*> m_ProjectComponentSelectedList;//專案零件指標列表-選取到 
	std::vector<CAOIComponent*> m_ProjectDefectComponentPtrList;//專案瑕疵零件指標列表
	std::vector<CAOIComponent*> m_ProjectDefectComponentPtrList_LA;//專案瑕疵零件指標列表
	std::vector<CAOIComponent*> m_ProjectDefectComponentPtrList_LB;//專案瑕疵零件指標列表
	std::vector<CAOIComponent*> m_ProjectDefectComponentPtrList_All;//專案瑕疵零件全部列表
	std::vector<CAOIModel*>     m_ProjectModelPtrList;//專案模組指標列表	
	std::vector<CColorGroup>    m_ProjectColorGroupList;//專案內定顏色
	//---------------------------------------------------------------------------------//
	std::vector<TVersionCode>   m_ProjectVersionCodeList;//專案版本號列表
	std::vector<TVersionCode>   m_ProjectVersionCodeListBackup;//專案版本號列表
	//---------------------------------------------------------------------------------//
	std::vector<CAOIModel*>     m_ProjectModelPtrList_Saved;//專案模組指標列表-要存檔的
	std::vector<CAOIModel*>     m_ProjectPartModelPtrList_Saved;//專案零件模組指標列表-要存檔的
	//---------------------------------------------------------------------------------//
	std::vector<CAOIRgn*>      m_ProjectTempRgnPtrList;//專案檢測區域指標列表
	std::vector<CAOIFov*>      m_ProjectTempFovPtrList;//專案視野指標列表		
	std::vector<CAOISlice*>    m_ProjectTempSlicePtrList;//專案碎片指標列表			
	std::vector<CAOIFrame*>    m_ProjectTempFramePtrList;//專案影像指標列表
	std::vector<CAOIField*>    m_ProjectTempFieldPtrList;//專案區域指標列表		
	//---------------------------------------------------------------------------------//	
	//專案底圖-Project Map	
	double                     m_ProjectMapResX;//專案底圖解析度-X
	double                     m_ProjectMapResY;//專案底圖解析度-Y
	int                        m_ProjectMapDividePos;//專案底圖分割位置
	int                        m_ProjectMapScaleMode;//專案底圖縮小模式	
	//---------------------------------------------------------------------------------//	
	RECT                       m_ProjectMapLocRect_DA;//專案底圖局部區域
	TSIZE2D                    m_ProjectMapLocSize_DA;;//專案底圖局部尺寸
	TREGION4D                  m_ProjectMapLocCadRgn_DA;//專案底圖局部Cad範圍	
	TREGION4D                  m_ProjectMapLocStageRgn_DA;//專案底圖局部機台範圍
	//---------------------------------------------------------------------------------//	
	TREGION4D                  m_ProjectMapCadRgn_DA;//專案底圖Cad範圍	
	TREGION4D                  m_ProjectMapStageRgn_DA;//專案底圖機台範圍
	TREGION4D                  m_ProjectMapTeachRgn_DA;//專案底圖教導範圍	
	//---------------------------------------------------------------------------------//
	RECT                       m_ProjectMapLocRect_DB;//專案底圖局部區域	
	TSIZE2D                    m_ProjectMapLocSize_DB;;//專案底圖局部尺寸
	TREGION4D                  m_ProjectMapLocCadRgn_DB;//專案底圖局部Cad範圍	
	TREGION4D                  m_ProjectMapLocStageRgn_DB;//專案底圖局部機台範圍
	//---------------------------------------------------------------------------------//
	TREGION4D                  m_ProjectMapCadRgn_DB;//專案底圖Cad範圍	
	TREGION4D                  m_ProjectMapStageRgn_DB;//專案底圖機台範圍	
	TREGION4D                  m_ProjectMapTeachRgn_DB;//專案底圖教導範圍	
	//---------------------------------------------------------------------------------//
	unsigned int               m_ProjectMapShowIndex;//專案底圖顯示的引數		
	IMAGE_PTR                  m_ProjectMapShowPtr;//專案底圖顯示指標
	size_t                     m_ProjectMapShowSize;//專案底圖顯示尺寸
	//---------------------------------------------------------------------------------//	
	unsigned int               m_ProjectMapIndex;//專案底圖的引數
	int                        m_ProjectMapIndex3D;//專案3D底圖的引數
	IMAGE_SIZE                 m_ProjectMapW[FRAME_MAX_COUNT];//專案底圖寬度
	IMAGE_SIZE                 m_ProjectMapH[FRAME_MAX_COUNT];//專案底圖長度
	IMAGE_SIZE                 m_ProjectMapStep[FRAME_MAX_COUNT];//專案底圖步長
	IMAGE_SIZE                 m_ProjectBitCount[FRAME_MAX_COUNT];//專案底圖位元數
	IMAGE_PTR                  m_ProjectMapPtr[FRAME_MAX_COUNT];//專案底圖指標
	CString                    m_ProjectMapFilename[FRAME_MAX_COUNT];//專案底圖檔名
	CString                    m_ProjectMapFilenameSmall[FRAME_MAX_COUNT];//專案底圖檔名-小圖	
	//---------------------------------------------------------------------------------//	
	//專案底圖-Project Map 遮罩圖	
	IMAGE_SIZE                 m_ProjectMapMaskW;//專案底圖遮罩寬度
	IMAGE_SIZE                 m_ProjectMapMaskH;//專案底圖遮罩長度
	IMAGE_SIZE                 m_ProjectMapMaskStep;//專案底圖遮罩步長
	IMAGE_SIZE                 m_ProjectMapMaskBitCount;//專案底圖遮罩位元數
	IMAGE_PTR                  m_ProjectMapMaskPtr;//專案底圖遮罩指標
	//---------------------------------------------------------------------------------//	
	IMAGE_PTR                  m_ProjectMapMaskTempPtr;//專案底圖遮罩指標
	//---------------------------------------------------------------------------------//	
	//專案標記-Project Mark	
	double                     m_ProjectMarkScore;//專案標記圖成績
	unsigned int               m_ProjectMarkIndex;//專案標記圖引數
	IMAGE_SIZE                 m_ProjectMarkW;//專案標記圖寬度
	IMAGE_SIZE                 m_ProjectMarkH;//專案標記圖長度
	IMAGE_SIZE                 m_ProjectMarkStep;//專案標記圖步長
	IMAGE_SIZE                 m_ProjectMarkBitCount;//專案標記圖位元數
	IMAGE_PTR                  m_ProjectMarkPtr;//專案標記圖指標	
	CString                    m_ProjectMarkFilename;//專案標記圖檔名	
	TPOINT3D                   m_ProjectMarkStagePos;//專案標記圖機台座標
	unsigned int               m_ProjectMarkIndex_System;//專案標記圖引數-系統
	//---------------------------------------------------------------------------------//
	//專案檢測底圖-Project Test Map
	double                     m_ProjectTestMapResX;//專案底圖解析度-X
	double                     m_ProjectTestMapResY;//專案底圖解析度-Y
	IMAGE_SIZE                 m_ProjectTestMapW[FRAME_MAX_COUNT];//專案檢測底圖寬度
	IMAGE_SIZE                 m_ProjectTestMapH[FRAME_MAX_COUNT];//專案檢測底圖長度
	IMAGE_SIZE                 m_ProjectTestMapStep[FRAME_MAX_COUNT];//專案檢測底圖步長
	IMAGE_SIZE                 m_ProjectTestBitCount[FRAME_MAX_COUNT];//專案檢測底圖位元數
	IMAGE_PTR                  m_ProjectTestMapPtr[FRAME_MAX_COUNT];//專案檢測底圖指標
	DWORD                      m_ProjectTestMapTickCount[FRAME_MAX_COUNT];//專案檢測底圖的時間碼
	CString                    m_ProjectTestMapFilename[FRAME_MAX_COUNT];//專案檢測底圖檔名	
	bool                       m_ProjectTestMapEnableSaved[FRAME_MAX_COUNT];//專案檢測底圖啟用存檔
	//---------------------------------------------------------------------------------//
	LONG                       m_ProjectPreLoadFrameImageCount;//專案預先載入圖檔的畫面數量
	THREAD_COMMAND_MODE        m_ProjectPreLoadFrameImageThreadCmd;//專案預先載入編程圖檔的執行緒狀態
	//---------------------------------------------------------------------------------//	
	unsigned int               m_ProjectFrameUniqueID_Fd;//專案定位點的影像表格
	std::vector<TSliceParam>   m_ProjectSliceParamList;//單1影像參數列表
	std::vector<TFrameParam>   m_ProjectFrameParamList;//影像參數列表	
	std::vector<unsigned int>  m_ProjectFrameUniqueIDList;//專案影像表格-檢測使用
	//---------------------------------------------------------------------------------//
	double                     m_ProjectSpaceMaxHeight;//空間最高高度
	double                     m_ProjectSpaceBaseHeight;//空間基本平面	
	//---------------------------------------------------------------------------------//	
	CRITICAL_SECTION           m_csProject;//同步機制-關鍵區間
	//---------------------------------------------------------------------------------//
	unsigned int               m_ProjectCopyFileID;//專案複製檔案	
	HANDLE                     m_ProjectCopyFileEvent;//專案複製檔案
	HANDLE                     m_ProjectCopyFileHandle;//專案複製檔案
	THREAD_STATE_MODE          m_ProjectCopyFileThreadState;//專案複製檔案
	THREAD_COMMAND_MODE        m_ProjectCopyFileThreadCmd;//專案複製檔案
	std::vector<TCopyFolder>   m_ProjectCopyFileList;//專案複製檔案列表
	//---------------------------------------------------------------------------------//		
	unsigned int               m_ProjectSaveFieldID;//專案儲存視野編號
	HANDLE                     m_ProjectSaveFieldEvent;//專案儲存視野事件
	HANDLE                     m_ProjectSaveFieldHandle;//專案儲存視野執行緒
	THREAD_STATE_MODE          m_ProjectSaveFieldThreadState;//專案儲存視野
	THREAD_COMMAND_MODE        m_ProjectSaveFieldThreadCmd;//專案儲存視野		
	bool                       m_ProjectSaveFieldThreadExecuted;//專案儲存視野被執行中
	std::vector<CAOIFrame*>    m_ProjectSaveFrameList;//專案儲存圖像列表
	std::vector<CAOIField*>    m_ProjectSaveFieldList;//專案儲存視野列表
	//---------------------------------------------------------------------------------//
	std::thread                m_WriteStaticDataToLibraryThread;
	THREAD_STATE_MODE          m_WriteStaticDataToLibraryThreadState;
	THREAD_COMMAND_MODE        m_WriteStaticDataToLibraryThreadCmd;
	//---------------------------------------------------------------------------------//
	std::vector<CString>       m_ProjectServerModelLockList;//專案伺服器模組鎖住列表
	//---------------------------------------------------------------------------------//
	TPOINT2D                   m_HASI_MinCadPos;
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//	
	void                       PreInitProject();
	void                       InitialProject();
	void                       InitialProjecColorGroupList();//初始化專案顏色
	//---------------------------------------------------------------------------------//	
	void                       CloneProject(const CAOIProject &project);
	void                       CloneProjectMap(const CAOIProject &project);		
	void                       CloneProjectMark(const CAOIProject &project);
	void                       CloneProjectMapMask(const CAOIProject &project);
	void                       CloneProjectTestMap(const CAOIProject &project);
	void                       CloneProjectObjList(const CAOIProject &project);			
	//---------------------------------------------------------------------------------//
	CJetIniFile*               GetProjectIniFilePtr();
	bool                       SaveProjectIniFile(LPCTSTR filename, CJetIniFile *pIniFile);
	bool                       SaveProjectIniFileFn(LPCTSTR filename, CJetIniFile *pIniFile);
	bool                       SaveINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pString, LPCTSTR pfilename, CString &Error, CJetIniFile *pIniFile=NULL);
	bool                       LoadINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pDefault, TCHAR *pString, int StringSize, LPCTSTR pfilename, bool IsCheckLens, CString &Error);	
	//---------------------------------------------------------------------------------//
	void                       ClearProjectMapShowBuffer();//清除專案底圖顯示指標	
	void                       SetProjectMapShowBuffer(unsigned int MapIndex, size_t size, IMAGE_PTR Ptr);
	void                       ClearProjectTestMapBuffer(bool ResetFileName=true);//清除專案檢測底圖指標	
	//---------------------------------------------------------------------------------//	
	CAOIProject*               GetProjectPtr_Inline();//取得專案指標
	CAOIProject*               GetProjectPtr_Direct();//取得專案指標
	//---------------------------------------------------------------------------------//
	bool                       CheckProjectPtr(CAOIProject *Ptr);//確認專案指標
	//---------------------------------------------------------------------------------//
	size_t                     GetProjectPanelCount_Inline() const;//取得專案整板數量	
	void                       AddProjectPanelPtr_Inline(CAOIPanel *PanelPtr);//增加專案整板	
	CAOIPanel*                 GetProjectPanelPtr_Inline(size_t index) const;//取得專案整板指標	
	//---------------------------------------------------------------------------------//
	size_t                     GetProjectBoardCount_Inline() const;//取得專案單板數量
	void                       AddProjectBoardPtr_Inline(CAOIBoard *BoardPtr);//增加專案單板
	CAOIBoard*                 GetProjectBoardPtr_Inline(size_t index) const;//取得專案單板指標	
	//---------------------------------------------------------------------------------//
	size_t                     GetProjectComponentCount_Inline() const;//取得專案零件數量
	void                       AddProjectComponentPtr_Inline(CAOIComponent *ComponentPtr);//增加專案零件
	CAOIComponent*             GetProjectComponentPtr_Inline(size_t index) const;//取得專案零件指標	
	//---------------------------------------------------------------------------------//	
	size_t                     GetProjectDropOutPartCount_Inline() const;//取得專案拋件數量
	void                       AddProjectDropOutPartPtr_Inline(CAOIComponent *ComponentPtr);//增加專案拋件
	CAOIComponent*             GetProjectDropOutPartPtr_Inline(size_t index) const;//取得專案拋件指標	
	//---------------------------------------------------------------------------------//
	size_t                     GetProjectScratchPartCount_Inline() const;//取得專案刮傷數量
	void                       AddProjectScratchPartPtr_Inline(CAOIComponent *ComponentPtr);//增加專案刮傷
	CAOIComponent*             GetProjectScratchPartPtr_Inline(size_t index) const;//取得專案刮傷指標	
	//---------------------------------------------------------------------------------//
	size_t                     GetProjectPartDimensionCount_Inline() const;//取得專案尺寸數量
	void                       AddProjectPartDimensionPtr_Inline(CAOIComponent *ComponentPtr);//增加專案尺寸
	CAOIComponent*             GetProjectPartDimensionPtr_Inline(size_t index) const;//取得專案尺寸指標	
	//---------------------------------------------------------------------------------//
	size_t                     GetProjectModelCount_Inline() const;//取得專案模組數量
	void                       AddProjectModelPtr_Inline(CAOIModel *ModelPtr);//增加專案模組
	CAOIModel*                 GetProjectModelPtr_Inline(size_t index) const;//取得專案模組指標	
	//---------------------------------------------------------------------------------//	
	size_t                     GetProjectDefectComponentCount_Inline() const;//取得專案瑕疵零件數量
	void                       AddProjectDefectComponentPtr_Inline(CAOIComponent *ComponentPtr);//增加專案瑕疵零件
	CAOIComponent*             GetProjectDefectComponentPtr_Inline(size_t index) const;//取得專案瑕疵零件指標	
	void                       ClearProjectDefectComponentPtrList_Inline();//清除專案瑕疵列表
	//---------------------------------------------------------------------------------//
	size_t                     GetProjectDefectComponentCount_LA_Inline() const;//取得專案瑕疵零件數量	
	CAOIComponent*             GetProjectDefectComponentPtr_LA_Inline(size_t index) const;//取得專案瑕疵零件指標	
	void                       ClearProjectDefectComponentPtrList_LA_Inline();//清除專案瑕疵列表
	//---------------------------------------------------------------------------------//
	size_t                     GetProjectDefectComponentCount_LB_Inline() const;//取得專案瑕疵零件數量	
	CAOIComponent*             GetProjectDefectComponentPtr_LB_Inline(size_t index) const;//取得專案瑕疵零件指標	
	void                       ClearProjectDefectComponentPtrList_LB_Inline();//清除專案瑕疵列表
	//---------------------------------------------------------------------------------//
	void                       AddProjectDefectComponentPtr_All_Inline(CAOIComponent *ComponentPtr);//增加專案全瑕疵零件
	void                       ClearProjectDefectComponentPtrList_All_Inline();//清除專案全瑕疵列表
	//---------------------------------------------------------------------------------//	
	size_t                     GetProjectFdCount_Inline() const;//取得專案定位點數量
	void                       AddProjectFdPtr_Inline(CAOIFd *FdPtr);//增加專案定位點
	CAOIFd*                    GetProjectFdPtr_Inline(size_t index) const;//取得專案定位點指標	
	//---------------------------------------------------------------------------------//
	size_t                     GetProjectFdSortedCount_Inline() const;//取得專案定位點排序數量
	void                       AddProjectFdSorted_Inline(CAOIFd *FdPtr);//增加專案定位點排序
	CAOIFd*                    GetProjectFdSorted_Inline(size_t index) const;//取得專案定位點排序指標	
	void                       ClearProjectFdSortedList_Inline();//清除專案定位點排序列表
	//---------------------------------------------------------------------------------//
	size_t                     GetProjectMapRgnCount_Inline() const;//取得專案底圖區域數量
	void                       AddProjectMapRgnPtr_Inline(CAOIRgn *RgnPtr);//增加專案底圖區域
	CAOIRgn*                   GetProjectMapRgnPtr_Inline(size_t index) const;//取得專案底圖區域指標
	//---------------------------------------------------------------------------------//
	size_t                     GetProjectMarkCount_Inline() const;//取得專案特徵點數量
	void                       AddProjectMarkPtr_Inline(CAOIMark *MarkPtr);//增加專案特徵點
	CAOIMark*                  GetProjectMarkPtr_Inline(size_t index) const;//取得專案特徵點指標	
	//---------------------------------------------------------------------------------//
	size_t                     GetProjectPartGroupCount_Inline() const;//取得專案零件群組數量
	void                       AddProjectPartGroupPtr_Inline(CAOIPartGroup *PartGroupPtr);//增加專案零件群組
	CAOIPartGroup*             GetProjectPartGroupPtr_Inline(size_t index) const;//取得專案零件群組指標	
	//---------------------------------------------------------------------------------//
	size_t                     GetProjectBarcodeCount_Inline() const;//取得專案軟體條碼數量
	void                       AddProjectBarcodePtr_Inline(CAOIBarcode *BarcodePtr);//增加專案軟體條碼
	CAOIBarcode*               GetProjectBarcodePtr_Inline(size_t index) const;//取得專案軟體條碼指標	
	//---------------------------------------------------------------------------------//
	size_t                     GetProjectFieldCount_Inline() const;//取得專案區域數量
	CAOIField*                 GetProjectFieldPtr_Inline(size_t index) const;//取得專案區域指標
	//---------------------------------------------------------------------------------//
	void                       ClearProjectProgramField_Inline();//清除專案編程區域列表
	size_t                     GetProjectProgramFieldCount_Inline() const;//取得專案編程區域數量
	void                       AddProjectProgramFieldPtr_Inline(CAOIField *FieldPtr);//增加專案編程區域
	CAOIField*                 GetProjectProgramFieldPtr_Inline(size_t index) const;//取得專案編程區域指標
	//---------------------------------------------------------------------------------//
	void                       ClearProjectInspectionField_Inline();//清除專案檢測區域列表		
	size_t                     GetProjectInspectionFieldCount_Inline() const;//取得專案檢測區域數量	
	void                       AddProjectInspectionFieldPtr_Inline(CAOIField *FieldPtr);//增加專案檢測區域
	CAOIField*                 GetProjectInspectionFieldPtr_Inline(size_t index) const;//取得專案檢測區域指標
	//---------------------------------------------------------------------------------//	
	void                       ClearProjectInspectionPartField_Inline();//清除專案檢測局部區域列表		
	size_t                     GetProjectInspectionPartFieldCount_Inline() const;//取得專案檢測局部區域數量	
	void                       AddProjectInspectionPartFieldPtr_Inline(CAOIField *FieldPtr);//增加專案檢測局部區域
	CAOIField*                 GetProjectInspectionPartFieldPtr_Inline(size_t index) const;//取得專案檢測局部區域指標
	//---------------------------------------------------------------------------------//
	void                       InitialProjectLock(); //初始化專案的關鍵區間
	void                       DeleteProjectLock();  //刪除專案的關鍵區間	
	//---------------------------------------------------------------------------------//
	bool                       CheckProjectSpecTestFieldDefectPart();//確認專案特殊檢測視野瑕疵零件-整板零件
	bool                       AddProjectSpecTestComonentList(std::vector<CAOIComponent*> &SpecComponentList);
	//---------------------------------------------------------------------------------//
	bool                       ResetProjectTop10List(std::vector<TTop10Node> &Top10List, bool ResetAOI, bool ResetARS);//重設前十大不良
	//---------------------------------------------------------------------------------//		
	bool                       AssignProjectFieldList(DISTRICT_ID DistrictID);//分配區域列表
	CAOIField*                 MatchProjectFieldPtr(const TREGION4D &Region, bool CadMode, bool Inner, DISTRICT_ID DistrictID);
	bool                       CreateProjectFieldList(FIELD_BUILD_MODE BuildMode, DISTRICT_ID DistrictID, std::vector<CAOIField*> &FieldPtrList, FIELD_DIVISION_MODE DivMode, bool ByCadRegion);//建立專案區域列表
	bool                       CreateProjectFieldListFn(FIELD_BUILD_MODE BuildMode, DISTRICT_ID DistrictID, std::vector<CAOIField*> &FieldPtrList, FIELD_DIVISION_MODE DivMode, bool ByCadRegion);//建立專案區域列表
	bool                       CreateProjectFieldListKernel(FIELD_BUILD_MODE BuildMode, DISTRICT_ID DistrictID, FIELD_DIVISION_MODE DivisionMode, bool ByCadRegion, bool CheckOldField, TJetRgnList &RgnList, TJetFieldList &FieldList);//建立區塊分割資料	
	//---------------------------------------------------------------------------------//	
public:
	//---------------------------------------------------------------------------------//
	CAOIProject();
	CAOIProject(const CAOIProject &project);
	virtual ~CAOIProject();
	CAOIProject& operator=(const CAOIProject &project);
	//---------------------------------------------------------------------------------//
	LPCTSTR                    GetErrorString() const;
	LPCTSTR                    GetErrorStringRaw() const;
	void                       SetErrorString(const char *str);
	void                       SetErrorString(const wchar_t *str);	
	//---------------------------------------------------------------------------------//
	void                       SetProjectExceptionCode_FileRead(LPCTSTR Err=NULL);
	void                       SetProjectExceptionCode_FileWrite(LPCTSTR Err=NULL);
	void                       SetProjectExceptionCode(AOI_EXCEPTION_CODE Code, LPCTSTR Err=NULL);
	//---------------------------------------------------------------------------------//
	void                       SetProjectParameter(const TProjectParameter &Param) { m_ProjectParameter = Param; }
	TProjectParameter&         GetProjectParameter() { return m_ProjectParameter; }
	TProjectParameter*         GetProjectParameterPtr() { return &m_ProjectParameter; }
	const TProjectParameter&   GetProjectParameter() const { return m_ProjectParameter; }
	bool                       ApplyProjectParameter();//套用專案參數	
	bool                       ApplySystemParameter(LANE_ID LaneID);//套用系統參數
	//---------------------------------------------------------------------------------//	
	bool                       BackupProjectParameter();//備份專案參數
	bool                       RestoreProjectParameter();//還原專案參數
	//---------------------------------------------------------------------------------//	
	int                        GetProjectEditOpenMpCount() const;
	//---------------------------------------------------------------------------------//
	bool                       CheckProjectShowNewProjectWizardWnd() const;//確認專案要顯示新專案精靈視窗
	//---------------------------------------------------------------------------------//
	bool                       FlushProjectOperLogFile();//傾寫專案的操作訊息檔案
	bool                       RevokeProjectOperLogFile();//撤銷專案的操作訊息檔案
	bool                       RegisterProjectOperLogFile();//註冊專案的操作訊息檔案	
	unsigned int               GetProjectOperLogIndex() const;//取得專案操作紀錄引數
	bool                       CheckProjectOperLogIndexValid() const;//確認專案操作紀錄引數有效
	//---------------------------------------------------------------------------------//
	bool                       CheckProjectOthersUsing3D() const;//確認專案其餘有使用3D	
	//---------------------------------------------------------------------------------//
	LPCTSTR                    GetProjectFileMainName() const;//取得專案主檔名
	//---------------------------------------------------------------------------------//	
	void                       SetProjectFileName(LPCTSTR value) { m_ProjectFileName= value ; }
	LPCTSTR                    GetProjectFileName() const { return m_ProjectFileName ; }	
	//---------------------------------------------------------------------------------//	
	void                       SetProjectShowName(LPCTSTR value);
	LPCTSTR                    GetProjectShowName() const { return m_ProjectShowName ; }
	//---------------------------------------------------------------------------------//	
	void                       SetProjectFdFolder(LPCTSTR value) { m_ProjectFdFolder= value ; }
	LPCTSTR                    GetProjectFdFolder() const { return m_ProjectFdFolder ; }	
	//---------------------------------------------------------------------------------//
	void                       SetProjectLibraryFolder(LPCTSTR value) { m_ProjectLibraryFolder= value ; }
	LPCTSTR                    GetProjectLibraryFolder() const { return m_ProjectLibraryFolder ; }	
	//---------------------------------------------------------------------------------//
	void                       SetProjectPartLibraryFolder(LPCTSTR value) { m_ProjectPartLibraryFolder= value ; }
	LPCTSTR                    GetProjectPartLibraryFolder() const { return m_ProjectPartLibraryFolder ; }	
	//---------------------------------------------------------------------------------//
	void                       SetProjectOfflineFolder(LPCTSTR value) { m_ProjectOfflineFolder= value ; }
	LPCTSTR                    GetProjectOfflineFolder() const { return m_ProjectOfflineFolder ; }	
	//---------------------------------------------------------------------------------//
	void                       RemoveProjectAllInspectionFolder();//移除專案所有檢測資料夾
	//---------------------------------------------------------------------------------//
	//專案AI圖檔資料夾
	void                       SetProjectAIImageFolder(LPCTSTR value) { m_ProjectAIImageFolder= value ; }
	LPCTSTR                    GetProjectAIImageFolder() const { return m_ProjectAIImageFolder ; }	
	//---------------------------------------------------------------------------------//
	//專案檢測足跡資料夾
	void                       RemoveProjectTestTrackFolder();
	void                       SetProjectTestTrackFolder(LPCTSTR value) { m_ProjectTestTrackFolder= value ; }
	LPCTSTR                    GetProjectTestTrackFolder() const { return m_ProjectTestTrackFolder ; }	
	//---------------------------------------------------------------------------------//
	//專案Spc資料夾
	void                       SetProjectSpcFolder(LPCTSTR value) { m_ProjectSpcFolder= value ; }
	LPCTSTR                    GetProjectSpcFolder() const { return m_ProjectSpcFolder ; }	
	//---------------------------------------------------------------------------------//
	//專案Spc檔案資料夾
	void                       SetProjectSpcFileFolder(LPCTSTR value) { m_ProjectSpcFileFolder= value ; }
	LPCTSTR                    GetProjectSpcFileFolder() const { return m_ProjectSpcFileFolder ; }	
	//---------------------------------------------------------------------------------//	
	//專案Spc圖檔資料夾
	void                       SetProjectSpcImageFolder(LPCTSTR value) { m_ProjectSpcImageFolder= value ; }
	LPCTSTR                    GetProjectSpcImageFolder() const { return m_ProjectSpcImageFolder ; }	
	//---------------------------------------------------------------------------------//
	//專案Spc圖檔資料夾-本機
	void                       SetProjectSpcImageFolderLocal(LPCTSTR value) { m_ProjectSpcImageFolderLocal= value ; }
	LPCTSTR                    GetProjectSpcImageFolderLocal() const { return m_ProjectSpcImageFolderLocal ; }	
	//---------------------------------------------------------------------------------//	
	//專案Spc結果資料夾-ARS給AOI
	void                       SetProjectSpcResultFolder(LPCTSTR value) { m_ProjectSpcResultFolder= value ; }
	LPCTSTR                    GetProjectSpcResultFolder() const { return m_ProjectSpcResultFolder ; }	
	//---------------------------------------------------------------------------------//
	//專案線上條碼資料夾
	void                       SetProjectOnlineBarcodeFolder(LPCTSTR value) { m_ProjectOnlineBarcodeFolder= value ; }
	LPCTSTR                    GetProjectOnlineBarcodeFolder() const { return m_ProjectOnlineBarcodeFolder ; }	
	//---------------------------------------------------------------------------------//
	//專案儲存Spc檔案啟用
	void                       SetProjectSpcFileSaveEnabled(bool value) { m_ProjectSpcFileSaveEnabled= value ; }
	bool                       GetProjectSpcFileSaveEnabled() const { return m_ProjectSpcFileSaveEnabled ; }	
	//---------------------------------------------------------------------------------//
	//專案離線定位點檔名
	void                       SetProjectOfflineFdFilename(LPCTSTR value) { m_ProjectOfflineFdFilename= value ; }
	LPCTSTR                    GetProjectOfflineFdFilename() const { return m_ProjectOfflineFdFilename ; }	
	//---------------------------------------------------------------------------------//	
	//專案在線微調資料夾
	void                       SetProjectOnlineTuningFolder(LPCTSTR value) { m_ProjectOnlineTuningFolder= value ; }
	LPCTSTR                    GetProjectOnlineTuningFolder() const { return m_ProjectOnlineTuningFolder ; }	
	//---------------------------------------------------------------------------------//
	//專案線上調機日期時間
	void                       ResetProjectOnlineTuningDateTime();
	void                       SetProjectOnlineTuningDateTime(LPCTSTR value) { m_ProjectOnlineTuningDateTime= value ; }
	LPCTSTR                    GetProjectOnlineTuningDateTime() const { return m_ProjectOnlineTuningDateTime ; }	
	//---------------------------------------------------------------------------------//	
	//專案線上離線編程資料夾
	void                       SetProjectOnlineOfflineFolder(LPCTSTR value) { m_ProjectOnlineOfflineFolder= value ; }
	LPCTSTR                    GetProjectOnlineOfflineFolder() const { return m_ProjectOnlineOfflineFolder ; }	
	//---------------------------------------------------------------------------------//
	//拋件檢測的除錯資料夾
	void                       SetProjectDebugFolderDropOut(LPCTSTR value) { m_ProjectDebugFolderDropOut= value ; }
	LPCTSTR                    GetProjectDebugFolderDropOut() const { return m_ProjectDebugFolderDropOut ; }	
	//---------------------------------------------------------------------------------//
	//刮傷檢測的除錯資料夾
	void                       SetProjectDebugFolderScratch(LPCTSTR value) { m_ProjectDebugFolderScratch= value ; }
	LPCTSTR                    GetProjectDebugFolderScratch() const { return m_ProjectDebugFolderScratch ; }	
	//---------------------------------------------------------------------------------//	
	//專案除錯資料夾-尺寸
	void                       SetProjectDebugFolderDimonsion(LPCTSTR value) { m_ProjectDebugFolderDimonsion= value ; }
	LPCTSTR                    GetProjectDebugFolderDimonsion() const { return m_ProjectDebugFolderDimonsion ; }	
	//---------------------------------------------------------------------------------//		
	//專案檢測結果檔案-暫存
	void                       SetProjectInspectionResultFileTmp(LPCTSTR value) { m_ProjectInspectionResultFileTmp= value ; }
	LPCTSTR                    GetProjectInspectionResultFileTmp() const { return m_ProjectInspectionResultFileTmp ; }	
	//---------------------------------------------------------------------------------//
	//專案是否儲存Spc零件圖檔
	void                       SetProjectSaveSpcPartImageMode(SAVE_SPC_PART_IMAGE_MODE value) { m_ProjectSaveSpcPartImageMode= value ; }
	SAVE_SPC_PART_IMAGE_MODE   GetProjectSaveSpcPartImageMode() const { return m_ProjectSaveSpcPartImageMode ; }	
	//---------------------------------------------------------------------------------//
	//專案Spc零件圖檔資料夾
	bool                       CopyProjectSpcPartImageSelected();//複製選取到零件的Spc零件圖檔
	bool                       CopyProjectSpcPartImage(const CAOIComponent *SrcPtr, const CAOIComponent *DstPtr);//複製選取到零件的Spc零件圖檔
	bool                       DeleteProjectSpcPartImageSelected(LPCTSTR Folder);//刪除選取到零件的Spc零件圖檔
	bool                       DeleteProjectSpcPartImageSelectedWithTheSameName(LPCTSTR Folder);//刪除選取到零件的Spc零件圖檔-與相同名稱零件
	void                       SetProjectSpcPartImageFolder(LPCTSTR value) { m_ProjectSpcPartImageFolder= value ; }
	LPCTSTR                    GetProjectSpcPartImageFolder() const { return m_ProjectSpcPartImageFolder ; }	
	//---------------------------------------------------------------------------------//
	void                       SetProjectIndex(unsigned int value) { m_ProjectIndex = value; }
	unsigned int               GetProjectIndex() const { return m_ProjectIndex; }
	//---------------------------------------------------------------------------------//
	void                       SetProjectDeleted(bool value) { m_ProjectDeleted = value; }
	bool                       GetProjectDeleted() const { return m_ProjectDeleted; }
	//---------------------------------------------------------------------------------//
	void                       SetProjectSelected(bool value) { m_ProjectSelected = value; }
	bool                       GetProjectSelected() const { return m_ProjectSelected; }
	//---------------------------------------------------------------------------------//	
	void                       SetProjectFdModified(bool value) { m_ProjectFdModified = value; }
	bool                       GetProjectFdModified() const { return m_ProjectFdModified; }
	//---------------------------------------------------------------------------------//	
	LANE_ID                    GetProjectActLaneID() const;//目前專案所屬軌道	
	void                       SetProjectActLaneID(LANE_ID LaneID);//目前專案所屬軌道	
	//---------------------------------------------------------------------------------//
	bool                       GetProjectLaneEnable(LANE_ID LaneID) const;//取得專案在軌道上啟用
	void                       SetProjectLaneEnable(LANE_ID LaneID, bool value);//設定專案在軌道上啟用
	//---------------------------------------------------------------------------------//		
	//專案在軌道A啟用
	void                       SetProjectLaneEnable_LA(bool value) { m_ProjectLaneEnable_LA = value; }
	bool                       GetProjectLaneEnable_LA() const { return m_ProjectLaneEnable_LA; }
	//---------------------------------------------------------------------------------//
	//專案在軌道B啟用
	void                       SetProjectLaneEnable_LB(bool value) { m_ProjectLaneEnable_LB = value; }
	bool                       GetProjectLaneEnable_LB() const { return m_ProjectLaneEnable_LB; }
	//---------------------------------------------------------------------------------//
	//專案檢測取像結束
	void                       SetProjectTestGrabFinish(bool value) { m_ProjectTestGrabFinish = value; }
	bool                       GetProjectTestGrabFinish() const { return m_ProjectTestGrabFinish; }
	//---------------------------------------------------------------------------------//
	//專案檢測計算結束
	void                       SetProjectTestCalcFinish(bool value) { m_ProjectTestCalcFinish = value; }
	bool                       GetProjectTestCalcFinish() const { return m_ProjectTestCalcFinish; }
	//---------------------------------------------------------------------------------//
	//專案檢測結束
	void                       SetProjectTestAllFinish(bool value) { m_ProjectTestAllFinish = value; }
	bool                       GetProjectTestAllFinish() const { return m_ProjectTestAllFinish; }
	//---------------------------------------------------------------------------------//
	//專案檢測報告結束
	void                       SetProjectTestReportSaved(bool value) { m_ProjectTestReportSaved = value; }
	bool                       GetProjectTestReportSaved() const { return m_ProjectTestReportSaved; }
	//---------------------------------------------------------------------------------//
	bool                       GetProjectHasTestInOneCycle(LANE_ID LaneID) const;//取得專案在軌道上在一次檢測是否已經檢測
	void                       SetProjectHasTestInOneCycle(LANE_ID LaneID, bool value);//設定專案在軌道上在一次檢測是否已經檢測
	//---------------------------------------------------------------------------------//		
	//專案在軌道A的一次檢測是否已經檢測
	void                       SetProjectHasTestInOneCycle_LA(bool value) { m_ProjectHasTestInOneCycle_LA = value; }
	bool                       GetProjectHasTestInOneCycle_LA() const { return m_ProjectHasTestInOneCycle_LA; }
	//---------------------------------------------------------------------------------//
	//專案在軌道B的一次檢測是否已經檢測
	void                       SetProjectHasTestInOneCycle_LB(bool value) { m_ProjectHasTestInOneCycle_LB = value; }
	bool                       GetProjectHasTestInOneCycle_LB() const { return m_ProjectHasTestInOneCycle_LB; }
	//---------------------------------------------------------------------------------//
	//專案是否被修改過且尚未保存
	void                       SetProjectHasModified(bool value) { m_ProjectHasModified = value; }
	bool                       GetProjectHasModified() const { return m_ProjectHasModified; }
	//---------------------------------------------------------------------------------//
	//專案目前任務模式
	TASK_MODE                  GetProjectActTaskMode() const;
	void                       SetProjectActTaskMode(TASK_MODE value);	
	//---------------------------------------------------------------------------------//
	DISTRICT_ID                GetProjectActDistrictID() const;
	void                       SetProjectActDistrictID(DISTRICT_ID DistrictID, bool bAssign);	
	void                       AssignProjectActDistrictID();
	//---------------------------------------------------------------------------------//				
	bool                       GetProjectIsClosing() const;//取得專案是否正在關閉
	void                       SetProjectIsClosing(bool val);//設定專案是否正在關閉
	//---------------------------------------------------------------------------------//	
	bool                       GetProjectReloadingFile() const;//取得專案是否正在重載檔案
	void                       SetProjectReloadingFile(bool val);//設定專案是否正在重載檔案
	//---------------------------------------------------------------------------------//	
	bool                       GetProjectUseLocalFolder() const;//取得專案是否使用本機資料夾
	void                       SetProjectUseLocalFolder(bool value);//設定專案是否使用本機資料夾	
	//---------------------------------------------------------------------------------//	
	bool                       GetProjectMultiDistrictMode() const;//取得專案是否多段檢測模式
	void                       SetProjectMultiDistrictMode(bool val);//設定專案是否多段檢測模式
	//---------------------------------------------------------------------------------//
	void                       CheckProjectXBoardUseRatioMode();//確認專案X板是否使用比例模式
	bool                       GetProjectXBoardUseRatioMode() const;//取得專案X板是否使用比例模式
	void                       SetProjectXBoardUseRatioMode(bool val);//設定專案X板是否使用比例模式
	//---------------------------------------------------------------------------------//
	bool                       GetProjectSaveOfflineImageFiles() const;//取得專案儲存離線圖檔
	void                       SetProjectSaveOfflineImageFiles(bool val);//設定專案儲存離線圖檔
	//---------------------------------------------------------------------------------//
	void                       SetupProjectSavedDateTime();//設定專案寫檔日期時間
	time_t                     GetProjectSavedDateTime() const;//取得專案寫檔日期時間
	void                       SetProjectSavedDateTime(time_t val);//設定專案寫檔日期時間
	//---------------------------------------------------------------------------------//		
	time_t                     GetProjectServerSavedDateTime() const;//取得專案伺服器存檔日期時間	
	void                       SetProjectServerSavedDateTime(time_t val);//設定專案伺服器存檔日期時間	
	//---------------------------------------------------------------------------------//
	time_t                     GetProjectServerProjectSavedDateTime() const;//取得專案伺服器專案存檔日期時間	
	void                       SetProjectServerProjectSavedDateTime(time_t val);//設定專案伺服器專案存檔日期時間	
	//---------------------------------------------------------------------------------//
	bool                       GetProjectNeedGrabFdForTuning() const;//取得專案需要取像定位點-調機
	void                       SetProjectNeedGrabFdForTuning(bool val);//設定專案需要取像定位點-調機
	//---------------------------------------------------------------------------------//		
	bool                       GetProjectNeedCheckDistrictPos() const;//取得專案需要確認段落位置
	void                       SetProjectNeedCheckDistrictPos(bool val);//設定專案需要確認段落位置	
	//---------------------------------------------------------------------------------//		
	bool                       GetProjectPartialCopyLibraryFolder() const;//取得專案局部分複製資料庫資料夾
	void                       SetProjectPartialCopyLibraryFolder(bool val);//設定專案局部分複製資料庫資料夾
	//---------------------------------------------------------------------------------//	
	bool                       GetProjectConveyerPreRunEnabled();//專案軌道提前運轉
	//---------------------------------------------------------------------------------//	
	void                       SetProjectConveyerPreRunRunning(bool val);//專案軌道提前運轉-啟用中
	bool                       GetProjectConveyerPreRunRunning() const;//專案軌道提前運轉-啟用中
	//---------------------------------------------------------------------------------//
	//專案是否警報停機(一般瑕疵)
	void                       SetProjectAlarmStop(bool value) { m_ProjectAlarmStop = value; }
	bool                       GetProjectAlarmStop() const { return m_ProjectAlarmStop; }
	//---------------------------------------------------------------------------------//	
	//專案是否警報瑕疵停機
	void                       SetProjectAlarmDefect(bool value) { m_ProjectAlarmDefect = value; }
	bool                       GetProjectAlarmDefect() const { return m_ProjectAlarmDefect; }
	//---------------------------------------------------------------------------------//	
	//專案異常
	void                       SetProjectIsException(bool value) { m_ProjectIsException = value; }
	bool                       GetProjectIsException() const { return m_ProjectIsException; }
	//---------------------------------------------------------------------------------//
	//專案定位點異常
	void                       SetProjectFdException(bool value) { m_ProjectFdException = value; }
	bool                       GetProjectFdException() const { return m_ProjectFdException; }
	//---------------------------------------------------------------------------------//
	//專案警報瑕疵內容
	void                       SetProjectAlarmString(LPCTSTR  value) { m_ProjectAlarmString = value; }
	LPCTSTR                    GetProjectAlarmString() const { return m_ProjectAlarmString; }
	//---------------------------------------------------------------------------------//
	double                     GetProjectFocusPos();//取得專案焦距位置	
	double                     GetProjectFocusPos(LANE_ID LaneID);//取得專案焦距位置
	double                     GetProjectFocusPosOffset() const;//取得專案焦距位置
	void                       SetProjectFocusPosOffset(double FocusOffset);//設定專案焦距位置
	//---------------------------------------------------------------------------------//
	bool                       GetProjectMapCTS(DISTRICT_ID DistrictID, CMapCoordinate &Map);//取得專案的座標轉換-Cad->Stage
	bool                       GetProjectMapSTC(DISTRICT_ID DistrictID, CMapCoordinate &Map);//取得專案的座標轉換-Stage->Cad
	//---------------------------------------------------------------------------------//	
	bool                       CheckProjectBarcodeCameraGrabAfterFd();//確定專案的相機條碼是否在取定位點後檢測
	//---------------------------------------------------------------------------------//	
	//專案條碼	
	void                       SetProjectBarcode(const char* value);
	void                       SetProjectBarcode(const wchar_t* value);
	const wchar_t*             GetProjectBarcode() const { return m_ProjectBarcode.c_str(); }
	//---------------------------------------------------------------------------------//		
	//專案條碼-上一筆	
	void                       SetProjectBarcodeLast(const char* value);
	void                       SetProjectBarcodeLast(const wchar_t* value);
	const wchar_t*             GetProjectBarcodeLast() const { return m_ProjectBarcodeLast.c_str(); }
	//---------------------------------------------------------------------------------//		
	//專案載具條碼
	void                       SetProjectTrayBarcode(const char* value);
	void                       SetProjectTrayBarcode(const wchar_t* value);
	const wchar_t*             GetProjectTrayBarcode() const { return m_ProjectTrayBarcode.c_str(); }
	//---------------------------------------------------------------------------------//		
	//專案蓋板條碼
	void                       SetProjectCoverBarcode(const char* value);
	void                       SetProjectCoverBarcode(const wchar_t* value);
	const wchar_t*             GetProjectCoverBarcode() const { return m_ProjectCoverBarcode.c_str(); }
	//---------------------------------------------------------------------------------//		
	//專案條碼-備份	
	void                       BackupProjectBarcode();//備份專案條碼
	void                       RestoreProjectBarcode();//恢復專案條碼
	bool                       AssignProjectPanelBoardBarcode();//分配專案整板單板的條碼
	bool                       AssignProjectPanelBoardBarcodeFn();//分配專案整板單板的條碼
	//---------------------------------------------------------------------------------//		
	//專案條碼-預設
	void                       SetProjectBarcodeDefault(const char* value);
	void                       SetProjectBarcodeDefault(const wchar_t* value);
	const wchar_t*             GetProjectBarcodeDefault() const { return m_ProjectBarcodeDefault.c_str(); }
	//---------------------------------------------------------------------------------//			
	//專案是否設定條碼
	void                       SetProjectIsGetBarcode(bool value) { m_ProjectIsGetBarcode = value; }
	bool                       GetProjectIsGetBarcode() const { return m_ProjectIsGetBarcode; }
	//---------------------------------------------------------------------------------//		
	//外接條碼機-條碼機編號
	void                       SetProjectBarcodeDeviceIndex(unsigned int value) { m_ProjectBarcodeDeviceIndex = value; }
	unsigned int               GetProjectBarcodeDeviceIndex() const { return m_ProjectBarcodeDeviceIndex; }
	//外接條碼機-條碼機第幾碼
	void                       SetProjectBarcodeDeviceCodeIndex(unsigned int value) { m_ProjectBarcodeDeviceCodeIndex = value; }
	unsigned int               GetProjectBarcodeDeviceCodeIndex() const { return m_ProjectBarcodeDeviceCodeIndex; }	
	//---------------------------------------------------------------------------------//
	void                       SetProjectBarcodeBelongMode(BARCODE_BELONG_MODE value) { m_ProjectBarcodeBelongMode = value; }
	BARCODE_BELONG_MODE        GetProjectBarcodeBelongMode() const { return m_ProjectBarcodeBelongMode; }	
	//---------------------------------------------------------------------------------//
	bool                       CheckProjectSaveProjectTestMap() const;//確認專案是否儲存檢測底圖
	SAVE_TEST_MAP_MODE         GetProjectSaveProjectTestMap() const { return m_ProjectParameter.m_SaveProjectTestMap; }
	void                       SetProjectSaveProjectTestMap(SAVE_TEST_MAP_MODE val) { m_ProjectParameter.m_SaveProjectTestMap = val; }
	//---------------------------------------------------------------------------------//
	bool                       CheckProjectSaveProjectTestMapToRepair() const;//確認專案是否儲存檢測底圖至維修站
	//---------------------------------------------------------------------------------//
	//專案時脈	
	void                       SetProjectTickCountStamp(DWORD Tick);//專案時脈-戳記-每次檢測到相同函式的時間標記
	void                       SetProjectTickCountPCBIn(DWORD Tick);//專案時脈-進板
	void                       SetProjectTickCountPCBExit(DWORD Tick);//專案時脈-離板
	void                       SetProjectTickCountTestFirst();//專案時脈-檢測開始
	DWORD                      SetProjectTickCountFdFinish();//專案時脈-定位點結束
	DWORD                      SetProjectTickCountGrabFinish();//專案時脈-取像結束
	DWORD                      SetProjectTickCountCalcFinish();//專案時脈-計算結束
	DWORD                      SetProjectTickCountTestFinish();//專案時脈-檢測結束	

	DWORD                      SetProjectTickCountTestFirst_DA();//專案時脈-檢測開始
	void                       SetProjectTickCountFdFinish_DA(DWORD TickCount);//專案時脈-定位點結束
	void                       SetProjectTickCountGrabFinish_DA(DWORD TickCount);//專案時脈-取像結束
	void                       SetProjectTickCountCalcFinish_DA(DWORD TickCount);//專案時脈-計算結束
	void                       SetProjectTickCountTestFinish_DA(DWORD TickCount);//專案時脈-檢測結束	

	DWORD                      SetProjectTickCountTestFirst_DB();//專案時脈-檢測開始
	void                       SetProjectTickCountFdFinish_DB(DWORD TickCount);//專案時脈-定位點結束
	void                       SetProjectTickCountGrabFinish_DB(DWORD TickCount);//專案時脈-取像結束
	void                       SetProjectTickCountCalcFinish_DB(DWORD TickCount);//專案時脈-計算結束
	void                       SetProjectTickCountTestFinish_DB(DWORD TickCount);//專案時脈-檢測結束	

	void                       CalcProjectTestTime(TTestTime &TestTime, DWORD CTStamp) const;//計算專案檢測花費時間
	//---------------------------------------------------------------------------------//
	//專案檢測紀錄
	bool                       SaveProjectResultAlarmString(LPCTSTR filename);//儲存專案結果停機文字檔案
	bool                       SaveProjectResultAlarmStringFn(LPCTSTR filename);//儲存專案結果停機文字檔案
	bool                       SaveProjectResultAlarmComponentList(LPCTSTR filename);//儲存專案結果停機零件列表
	bool                       SaveProjectResultAlarmComponentListFn(LPCTSTR filename);//儲存專案結果停機零件列表
	
	void                       SetProjectResultYieldOnAOI(bool value) { m_ProjectResultYieldOnAOI = value; }
	bool                       GetProjectResultYieldOnAOI() const { return m_ProjectResultYieldOnAOI; }

	void                       SetProjectResultYieldOnARS(bool value) { m_ProjectResultYieldOnARS = value; }
	bool                       GetProjectResultYieldOnARS() const { return m_ProjectResultYieldOnARS; }

	void                       SetProjectResultAlarm(const TTestResult &value) { m_ProjectResultAlarm = value; }
	const TTestResult&         GetProjectResultAlarm() const { return m_ProjectResultAlarm; }

	CString                    GetProjectInspectionDateTime() const;
	void                       GetProjectInspectionDateTime(TCHAR DateTime[]) const;
	void                       SetProjectResultCurrent(const TTestResult &value) { m_ProjectResultCurrent = value; }
	const TTestResult&         GetProjectResultCurrent() const { return m_ProjectResultCurrent; }		
	//---------------------------------------------------------------------------------//	
	void                       SetProjectResultLatest(const TTestResult &value) { m_ProjectResultLatest = value; }
	const TTestResult&         GetProjectResultLatest() const { return m_ProjectResultLatest; }	
	//---------------------------------------------------------------------------------//	
	const TTestResult&         GetProjectResultLatest_Lane(LANE_ID LaneID) const;
	void                       SetProjectResultLatest_Lane(LANE_ID LaneID, const TTestResult &value);	
	//---------------------------------------------------------------------------------//	
	void                       SetProjectResultLatest_LA(const TTestResult &value) { m_ProjectResultLatest_LA = value; }
	const TTestResult&         GetProjectResultLatest_LA() const { return m_ProjectResultLatest_LA; }	
	//---------------------------------------------------------------------------------//	
	void                       SetProjectResultLatest_LB(const TTestResult &value) { m_ProjectResultLatest_LB = value; }
	const TTestResult&         GetProjectResultLatest_LB() const { return m_ProjectResultLatest_LB; }	
	//---------------------------------------------------------------------------------//	
	void                       SetProjectResultStatistic(const TTestResult &value) { m_ProjectResultStatistic = value; }
	const TTestResult&         GetProjectResultStatistic() const { return m_ProjectResultStatistic; }	
	//---------------------------------------------------------------------------------//
	const TTestResult&         GetProjectResultStatistic_Lane(LANE_ID LaneID) const;
	void                       SetProjectResultStatistic_Lane(LANE_ID LaneID, const TTestResult &value);	
	//---------------------------------------------------------------------------------//
	void                       SetProjectResultStatistic_LA(const TTestResult &value) { m_ProjectResultStatistic_LA = value; }
	const TTestResult&         GetProjectResultStatistic_LA() const { return m_ProjectResultStatistic_LA; }	
	//---------------------------------------------------------------------------------//
	void                       SetProjectResultStatistic_LB(const TTestResult &value) { m_ProjectResultStatistic_LB = value; }
	const TTestResult&         GetProjectResultStatistic_LB() const { return m_ProjectResultStatistic_LB; }	
	//---------------------------------------------------------------------------------//
	//專案檢測紀錄-最近_ARS
	void                       SetProjectResultLatest_ARS(const TTestResult &value) { m_ProjectResultLatest_ARS = value; }
	const TTestResult&         GetProjectResultLatest_ARS() const { return m_ProjectResultLatest_ARS; }	
	//---------------------------------------------------------------------------------//
	const TTestResult&         GetProjectResultLatest_ARS_Lane(LANE_ID LaneID) const;
	void                       SetProjectResultLatest_ARS_Lane(LANE_ID LaneID, const TTestResult &value);	
	//---------------------------------------------------------------------------------//
	void                       SetProjectResultLatest_ARS_LA(const TTestResult &value) { m_ProjectResultLatest_ARS_LA = value; }
	const TTestResult&         GetProjectResultLatest_ARS_LA() const { return m_ProjectResultLatest_ARS_LA; }	
	//---------------------------------------------------------------------------------//
	void                       SetProjectResultLatest_ARS_LB(const TTestResult &value) { m_ProjectResultLatest_ARS_LB = value; }
	const TTestResult&         GetProjectResultLatest_ARS_LB() const { return m_ProjectResultLatest_ARS_LB; }	
	//---------------------------------------------------------------------------------//
	//專案檢測紀錄-統計_ARS
	void                       SetProjectResultStatistic_ARS(const TTestResult &value) { m_ProjectResultStatistic_ARS = value; }
	const TTestResult&         GetProjectResultStatistic_ARS() const { return m_ProjectResultStatistic_ARS; }	
	//---------------------------------------------------------------------------------//
	const TTestResult&         GetProjectResultStatistic_ARS_Lane(LANE_ID LaneID) const;
	void                       SetProjectResultStatistic_ARS_Lane(LANE_ID LaneID, const TTestResult &value);	
	//---------------------------------------------------------------------------------//
	void                       SetProjectResultStatistic_ARS_LA(const TTestResult &value) { m_ProjectResultStatistic_ARS_LA = value; }
	const TTestResult&         GetProjectResultStatistic_ARS_LA() const { return m_ProjectResultStatistic_ARS_LA; }	
	//---------------------------------------------------------------------------------//
	void                       SetProjectResultStatistic_ARS_LB(const TTestResult &value) { m_ProjectResultStatistic_ARS_LB = value; }
	const TTestResult&         GetProjectResultStatistic_ARS_LB() const { return m_ProjectResultStatistic_ARS_LB; }	
	//---------------------------------------------------------------------------------//
	bool                       AddProjectResultCpkList_Lane(LANE_ID LaneID);
	const std::vector<TCpkItem>& GetProjectResultCpkList_Lane(LANE_ID LaneID) const;		
	void                       ResetProjectResultCpkList(std::vector<TCpkItem> &List);
	void                       AddProjectResultCpkList(const TCpkItem &CpkItem, std::vector<TCpkItem> &List);
	//---------------------------------------------------------------------------------//	
	void                       AddProjectResultAOI(const TTestResult &value);//增加專案檢測結果-AOI
	size_t                     GetProjectResultCountAOI() const;//取得專案檢測數量-AOI
	bool                       ShiftProjectResultListAOI();//偏移專案檢測列表-AOI
	bool                       CalcProjectResultYieldingAOI(double &rtTest, double &rtPanel, double &rtBoard, double &rtComponent);//計算專案結果良率-AOI
	//---------------------------------------------------------------------------------//
	void                       CalcProjectResultLatest_ARS(LANE_ID LaneID, RESULT_ID ResultID, CTime &cTime);//計算專案最新結果-ARS
	//---------------------------------------------------------------------------------//
	void                       InitialProjectResultARS(LANE_ID LaneID);//初始化專案檢測結果-ARS
	void                       AddProjectResultARS(const TTestResult &value);//增加專案檢測結果-ARS
	size_t                     GetProjectResultCountARS() const;//取得專案檢測數量-ARS
	bool                       ShiftProjectResultListARS();//偏移專案檢測列表-ARS
	bool                       CalcProjectResultYieldingARS(double &rtTest, double &rtPanel, double &rtBoard, double &rtComponent);//計算專案結果良率-AOI
	//---------------------------------------------------------------------------------//
	//線上檢測狀態
	void                       SetProjectOnlineStateMode(ONLINE_STATE_MODE value) { m_ProjectOnlineState = value; }
	ONLINE_STATE_MODE          GetProjectOnlineStateMode() const { return m_ProjectOnlineState; }
	//---------------------------------------------------------------------------------//
	//檢測狀態
	void                       SetProjectInspectionState(INSPECTION_STATE_MODE value)  { m_ProjectInspectionState = value; }
	INSPECTION_STATE_MODE      GetProjectInspectionState() const { return m_ProjectInspectionState; }
	//---------------------------------------------------------------------------------//
	int                        GetProjectFdFreeGroupID();//取得專案定位點可用的群組編號
	int                        GetProjectMarkFreeGroupID();//取得專案特徵點可用的群組編號
	int                        GetProjectBarcodeFreeGroupID();//取得專案條碼可用的群組編號
	//---------------------------------------------------------------------------------//
	//專案定位點最大的唯一碼
	void                       SetProjectFdMaxUniqueID(int value) { m_ProjectFdMaxUniqueID = value; }
	int                        GetProjectFdMaxUniqueID() const { return m_ProjectFdMaxUniqueID; }
	//---------------------------------------------------------------------------------//	
	//專案特徵點最大的唯一碼
	void                       SetProjectMarkMaxUniqueID(int value) { m_ProjectMarkMaxUniqueID = value; }
	int                        GetProjectMarkMaxUniqueID() const { return m_ProjectMarkMaxUniqueID; }
	//---------------------------------------------------------------------------------//	
	//專案零件群組最大的唯一碼
	void                       SetProjectGroupMaxUniqueID(int value) { m_ProjectGroupMaxUniqueID = value; }
	int                        GetProjectGroupMaxUniqueID() const { return m_ProjectGroupMaxUniqueID; }
	//---------------------------------------------------------------------------------//
	//專案條碼最大的唯一碼
	void                       SetProjectBarcodeMaxUniqueID(int value) { m_ProjectBarcodeMaxUniqueID = value; }
	int                        GetProjectBarcodeMaxUniqueID() const { return m_ProjectBarcodeMaxUniqueID; }
	//---------------------------------------------------------------------------------//	
	//專案零件最大的唯一碼
	void                       SetProjectComponentMaxUniqueID(int value) { m_ProjectComponentMaxUniqueID = value; }
	int                        GetProjectComponentMaxUniqueID() const { return m_ProjectComponentMaxUniqueID; }
	//---------------------------------------------------------------------------------//	
	CAOIFd*                    GetProjectActiveFd();
	CAOIMark*                  GetProjectActiveMark();
	bool                       SetProjectActiveMark(CAOIMark *MarkPtr);
	CAOIPanel*                 GetProjectActivePanel();
	bool                       SetProjectActivePanel(CAOIPanel *PanelPtr);
	CAOIBoard*                 GetProjectActiveBoard();
	bool                       SetProjectActiveBoard(CAOIBoard *BoardPtr);
	CAOIBarcode*               GetProjectActiveBarcode();
	bool                       SetProjectActiveBarcode(CAOIBarcode *BarcodePtr);
	CAOIComponent*             GetProjectActiveComponent();
	bool                       SetProjectActiveComponent(CAOIComponent *ComponentPtr);	
	bool                       CloseProjectActiveComponent(CAOIComponent *ComponentPtr, bool UpdateToeOthers);
	bool                       SelectProjectActiveComponent(bool bSel=true);//設定主要零件為選取狀態	
	CAOIWindow*                GetProjectActiveWindow();
	//---------------------------------------------------------------------------------//	
	bool                       SetProjectActiveModelBox(CAOIBox *BoxPtr);	
	bool                       SetProjectActiveModelWnd(CAOIWnd *WndPtr);	
	bool                       SetProjectActiveModelLand(CAOILand *LandPtr);	
	//---------------------------------------------------------------------------------//	
	CAOIWnd*                   GetProjectActiveFdModelWnd();	
	CAOIWnd*                   GetProjectActiveBarcodeModelWnd();	
	CAOIWnd*                   GetProjectActiveComponentModelWnd();	
	CAOILand*                  GetProjectActiveComponentModelLand();	
	//---------------------------------------------------------------------------------//	
	//主要操作的物件序號
	void                       ResetProjectActiveIndex();
	void                       ResetProjectActiveModelBoxUUID();
	void                       ResetProjectActiveModelWndUUID();
	void                       ResetProjectActiveModelLandUUID();
	unsigned int               GetProjectActiveFdIndex() const;
	void                       SetProjectActiveFdIndex(unsigned int value);	
	unsigned int               GetProjectActiveMarkIndex() const;
	void                       SetProjectActiveMarkIndex(unsigned int value);	
	unsigned int               GetProjectActivePanelIndex() const;
	void                       SetProjectActivePanelIndex(unsigned int value);	
	unsigned int               GetProjectActiveBoardIndex() const;
	void                       SetProjectActiveBoardIndex(unsigned int value);
	unsigned int               GetProjectActiveBarcodeIndex() const;
	void                       SetProjectActiveBarcodeIndex(unsigned int value);	
	unsigned int               GetProjectActiveComponentIndex() const;
	void                       SetProjectActiveComponentIndex(unsigned int value);
	unsigned int               GetProjectActiveComponentWindowIndex() const;
	void                       SetProjectActiveComponentWindowIndex(unsigned int value);	
	UUID                       GetProjectActiveModelBoxUUID() const;
	void                       SetProjectActiveModelBoxUUID(const UUID &value);
	UUID                       GetProjectActiveModelWndUUID() const;
	void                       SetProjectActiveModelWndUUID(const UUID &value);	
	UUID                       GetProjectActiveModelLandUUID() const;
	void                       SetProjectActiveModelLandUUID(const UUID &value);	
	//---------------------------------------------------------------------------------//
	void                       SetProjectActiveBarcodePtr(CAOIBarcode *BarcodePtr);	
	void                       SetProjectActiveComponentPtr(CAOIComponent *ComponentPtr);	
	//---------------------------------------------------------------------------------//
	//給Ribbon操作的物件序號 
	void                       ResetProjectRibbonIndex();
	void                       SetProjectRibbonFdIndex(unsigned int value) { m_ProjectRibbonFdIndex = value; }
	unsigned int               GetProjectRibbonFdIndex() const { return m_ProjectRibbonFdIndex; }
	void                       SetProjectRibbonPanelIndex(unsigned int value) { m_ProjectRibbonPanelIndex = value; }
	unsigned int               GetProjectRibbonPanelIndex() const { return m_ProjectRibbonPanelIndex; }
	void                       SetProjectRibbonBoardIndex(unsigned int value) { m_ProjectRibbonBoardIndex = value; }
	unsigned int               GetProjectRibbonBoardIndex() const { return m_ProjectRibbonBoardIndex; }
	void                       SetProjectRibbonBarcodeIndex(unsigned int value) { m_ProjectRibbonBarcodeIndex = value; }
	unsigned int               GetProjectRibbonBarcodeIndex() const { return m_ProjectRibbonBarcodeIndex; }	
	void                       SetProjectRibbonComponentIndex(unsigned int value) { m_ProjectRibbonComponentIndex = value; }
	unsigned int               GetProjectRibbonComponentIndex() const { return m_ProjectRibbonComponentIndex; }
	void                       SetProjectRibbonComponentReBuild(bool value) { m_ProjectRibbonComponentReBuild = value; }
	bool                       GetProjectRibbonComponentReBuild() const { return m_ProjectRibbonComponentReBuild; }	
	//---------------------------------------------------------------------------------//	
	//區域使用尺寸模式		
	FIELD_SIZE_MODE            GetProjectFieldSizeModeW() const;
	void                       SetProjectFieldSizeModeW(FIELD_SIZE_MODE value);
	FIELD_SIZE_MODE            GetProjectFieldSizeModeH() const;
	void                       SetProjectFieldSizeModeH(FIELD_SIZE_MODE value);
	bool                       ModifyProjectFieldSize(double &FovW, double &FovH);//修正視野使用範圍	
	//---------------------------------------------------------------------------------//	
	OFFLINE_IMAGE_SCOPE        GetProjectOfflineImageScope() const;//離線影像範疇
	void                       SetProjectOfflineImageScope(OFFLINE_IMAGE_SCOPE val);//離線影像範疇		
	//---------------------------------------------------------------------------------//
	bool                       SwtichProjectOfflineFileMode(OFFLINE_FILE_MODE OfflineFileMode, bool DistrictChange, bool LoadFrame);
	OFFLINE_FILE_MODE          GetProjectOfflineFileMode() const { return m_ProjectOfflineFileMode; }		
	void                       SetProjectOfflineFileMode(OFFLINE_FILE_MODE Mode) { m_ProjectOfflineFileMode = Mode; }		
	//---------------------------------------------------------------------------------//	
	FIELD_BUILD_MODE           GetProjectFieldBuildMode() const { return m_ProjectFieldBuildMode; }	
	void                       SetProjectFieldBuildMode(FIELD_BUILD_MODE val) { m_ProjectFieldBuildMode=val; }		
	FIELD_BUILD_MODE           GetProjectInspectionFieldBuildMode() const { return m_ProjectInspectionFieldBuildMode; }	
	void                       SetProjectInspectionFieldBuildMode(FIELD_BUILD_MODE val) { m_ProjectInspectionFieldBuildMode=val; }	
	//---------------------------------------------------------------------------------//	
	void                       SetProjectFieldBuildAreaMode(FIELD_BUILD_AREA_MODE val) { m_ProjectFieldBuildAreaMode=val; }	
	FIELD_BUILD_AREA_MODE      GetProjectFieldBuildAreaMode() const { return m_ProjectFieldBuildAreaMode; }	
	void                       SetProjectInspectionFieldBuildAreaMode(FIELD_BUILD_AREA_MODE val) { m_ProjectInspectionFieldBuildAreaMode=val; }	
	FIELD_BUILD_AREA_MODE      GetProjectInspectionFieldBuildAreaMode() const { return m_ProjectInspectionFieldBuildAreaMode; }	
	//---------------------------------------------------------------------------------//	
	bool                       CombineProjectAllObjects(CAOIFileIO &FileIO);//連結所有物件-使用引數	
	//---------------------------------------------------------------------------------//
	void                       DumpProjectFile(FILE *pfile);//偵錯匯出	
	bool                       DumpProjectRgnList(LPCTSTR filename, const std::vector<CAOIRgn*> &RgnList);	
	bool                       DumpProjectProgramFieldFrameList(LPCTSTR filename);	
	bool                       DumpProjectInspectionFieldFrameList(LPCTSTR filename);	
	bool                       DumpProjectFieldFrameList(LPCTSTR filename, const std::vector<CAOIField*> &FieldList);	
	//---------------------------------------------------------------------------------//	
	CAOIProject*               CloneProjectObj() const;//建立且複製一個專案
	void                       LayoutProjectRegion();//更新專案的區域
	void                       LayoutProjectRegion(DISTRICT_ID DistricID);//更新專案的區域
	void                       ClearProjectAllObjects();//刪除所有物件
	void                       ClearProjectAllTempObjects();//刪除所有取像用的物件
	void                       DestroyProjectObjectSelected();//摧毀選取到的專案物件
	void                       SelectProjectAllObjects(bool value);//選取專案所有物件
	void                       LayoutProjectAllObjectsIndex();//更新專案所有物件的引數	
	//---------------------------------------------------------------------------------//
	LPCTSTR                    GetProjectErrorString() const;
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	CString                    LoadMultiLanguageString2(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//	
	bool                       CloseProject();//關閉專案, 並且清除暫存資料夾	
	bool                       CopyProjectFileToOtherFile(LPCTSTR filename, DWORD dwFolderFlags);//複製專案檔案至其它專案
	//---------------------------------------------------------------------------------//	
	bool                       SaveProject(LPCTSTR filename, LPCTSTR filename_show, DWORD dwCopyFlag, bool bPartialCopy);//儲存專案
	bool                       SaveProjectFn(LPCTSTR filename, LPCTSTR filename_show, DWORD dwCopyFlag, bool bPartialCopy);//儲存專案
	bool                       LoadProject(LPCTSTR filename, LPCTSTR filename_show, bool LibraryMode);//載入專案	
	bool                       LoadProjectFn(LPCTSTR filename, LPCTSTR filename_show, bool LibraryMode);//載入專案	
	//---------------------------------------------------------------------------------//		
	bool                       SaveProjectFile(LPCTSTR filename);//儲存專案檔案
	bool                       SaveProjectFileFn(LPCTSTR filename);//儲存專案檔案
	bool                       LoadProjectFile(LPCTSTR filename);//載入專案檔案
	bool                       LoadProjectFileFn(LPCTSTR filename);//載入專案檔案
	bool                       ReLoadProjectFile(LPCTSTR filename);//重載專案檔案
	bool                       ReLoadProjectFileFn(LPCTSTR filename);//重載專案檔案
	//---------------------------------------------------------------------------------//		
	bool                       BackupProjectStatistic(TProjectStatisticBackup &StatisticBackup);
	bool                       RestoreProjectStatistic(TProjectStatisticBackup &StatisticBackup);//還原專案統計資料
	//---------------------------------------------------------------------------------//	
	bool                       InitialLoadProjectFile();//初始化載入專案
	bool                       WriteProjectFile(CAOIFileIO &FileIO);//儲存專案
	bool                       WriteProjectFileFn(CAOIFileIO &FileIO);//儲存專案
	bool                       ReadProjectFile(CAOIFileIO &FileIO);//載入專案	
	bool                       ReadProjectFileFn(CAOIFileIO &FileIO);//載入專案	
	//---------------------------------------------------------------------------------//
	bool                       ReadProjectSpiPidFile(CAOIFileIO &FileIO, std::vector<CSPIPad> &List);//讀入SPI的PID檔案	
	bool                       ReadProjectSpiPidFileFn(CAOIFileIO &FileIO, std::vector<CSPIPad> &List);//讀入SPI的PID檔案	
	bool                       LoadProjectSpiPidFile(LPCTSTR pfilename, CAOIPanel *PanelPtr, double AddAngle, bool bReverseAngle);//讀入SPI的PID檔案	
	bool                       LoadProjectSpiPidFileFn(LPCTSTR pfilename, CAOIPanel *PanelPtr, double AddAngle, bool bReverseAngle);//讀入SPI的PID檔案		
	bool                       LoadProjectSpiPidFileToCurrent(LPCTSTR pfilename, CAOIPanel *PanelPtr, double AddAngle, bool bReverseAngle);//讀入SPI的PID檔案		
	bool                       GetProjectBoardIndexLRTB(std::vector<CAOIBoard*> &list, int & Row, int & Col);
	bool                       ConvertSpiComponentListToAOIComponentList(const std::vector<CSPIComponent> &PartList, std::vector<CAOIBoard*> &BoardList, std::vector<CAOIComponent*> &ComponentList);
	//---------------------------------------------------------------------------------//
	bool                       LoadProjectCadxyFile(LPCTSTR pfilename, CAOIPanel *PanelPtr, bool Unicode, size_t StartLine, const std::vector<int> &ColumnDef, const std::vector<char> &Delimiters, const std::vector<wchar_t> &wDelimiters, char TextFilter, const double Factor, double ComW, double ComH, char NewBoardName[]);//將CADXY檔案讀入資料陣列(CADX_CS)
	bool                       LoadProjectCadxyFileFn(LPCTSTR pfilename, CAOIPanel *PanelPtr, bool Unicode, size_t StartLine, const std::vector<int> &ColumnDef, const std::vector<char> &Delimiters, const std::vector<wchar_t> &wDelimiters, char TextFilter, const double Factor, double ComW, double ComH, char NewBoardName[]);//將CADXY檔案讀入資料陣列(CADX_CS)
	//---------------------------------------------------------------------------------//
	bool                       ConvertToSpcProjectInfo(TSpcProjectInfo &SpcProjectInfo);//轉成Spc專案資訊
	//---------------------------------------------------------------------------------//
	bool                       SaveProjectObjectPosition(LPCTSTR pfilename);//儲存專案物件的座標
	bool                       SaveProjectObjectPositionFn(LPCTSTR pfilename);//儲存專案物件的座標
	//---------------------------------------------------------------------------------//
	bool                       CheckProjectLocking();//確認專案是否鎖住
	bool                       ChangeProjectObjUUID();//變更專案的物件UUID-另存專案時使用	
	//---------------------------------------------------------------------------------//
	void                       MoveProjectPos(double dX, double dY);//移動專案
	void                       MoveProjectStagePos(double dX, double dY);//移動專案
	//---------------------------------------------------------------------------------//
	bool                       InitialProjectBasePlaneParam(TBasePlaneParam &Param);//初始化基準面參數
	bool                       RotateProjectBasePlaneParam(double Angle, TBasePlaneParam &Param);//旋轉基準面參數
	bool                       UpdateProjectBasePlaneColorGroupLinkIndex(const std::vector<CColorGroup> &ColorGroupList, TBasePlaneParam &Param);//更新基準面抽色參數
	bool                       UpdateProjectBasePlaneFrameIndex(unsigned int DefaultIndex, unsigned int DefaultUniqueID, const std::vector<unsigned int> &FrameIndexMapList, TBasePlaneParam &Param);//更新基準面的畫面引數
	//---------------------------------------------------------------------------------//	
	size_t                     GetProjectPanelCount() const;//取得專案整板數量	
	CAOIPanel*                 GetProjectPanelPtr(size_t index, bool check) const;//取得專案整板指標
	CAOIPanel*                 AddProjectPanelPtr(CAOIPanel *PanelPtr, bool clone);//增加專案整板	
	bool                       DestroyProjectPanelSelected();//摧毀選取到的專案整板
	bool                       ClearProjectAllPanels();//刪除專案整板
	bool                       LayoutProjectPanelList();//重整專案的整板列表
	bool                       SelectProjectAllPanels(bool value);//選取專案的所有整板
	bool                       SelectProjectPanelsByStage(const TREGION4D &Rgn, bool ResultMode, std::vector<CAOIPanel*> &PanelList);//選取整板
	CAOIPanel*                 GetProjectPanelPtrBySelected();//取得專案整板指標-依據選取到	
	CAOIPanel*                 GetProjectPanelPtrByPanelUUID(const UUID &uuid);//取得專案整板指標-依據萬用字碼
	size_t                     GetProjectPanelSelectedCount() const;//取得專案整板選取到的數量
	bool                       GetProjectPanelSelected(std::vector<CAOIPanel*> &SelPanelList);//取得專案整板選取到	
	bool                       DeleteProjectPanelSelected();//刪除選取到的專案整板
	bool                       RotateProjectPanelSelected(double Angle);//選轉整板角度
	bool                       InvertSelectProjectPanel(bool bUpdate);//反向選取整板
	bool                       MoveProjectPanelSelected(double dX, double dY);//移動專案選取到的整板
	bool                       MirrorXProjectPanelSelected();//鏡射專案選取到整板的X座標
	bool                       MirrorYProjectPanelSelected();//鏡射專案選取到整板的Y座標
	bool                       SwitchProjectPanelBypassed();//切換專案整板不檢測模式
	bool                       CheckProjectPanelValid(const CAOIPanel *RefPanelPtr);//確認專案的整板指標有效
	bool                       PasteProjectPanelList(std::vector<CAOIPanel*> &ClonePanelList, double StageOffsetX, double StageOffsetY);//貼上專案整板
	bool                       ArrayPasteProjectPanelList(std::vector<CAOIPanel*> &ClonePanelList, const std::vector<TPOINT2D> &PosList);//陣列貼上專案整板
	bool                       SetProjectPanelSelectedBarcodeCodeIndex(unsigned int BarcodeCodeIndex);//設定專案整板條碼序號
	bool                       SetProjectPanelSelectedBarcodeDeviceIndex(unsigned int BarcodeDeviceIndex);//設定專案整板條碼機序號
	//---------------------------------------------------------------------------------//
	size_t                     GetProjectBoardCount() const;//取得專案單板數量
	CAOIBoard*                 GetProjectBoardPtr(size_t index, bool check) const;//取得專案單板指標
	CAOIBoard*                 AddProjectBoardPtr(CAOIBoard *BoardPtr, bool clone);//增加專案單板
	bool                       DestroyProjectBoardSelected();//摧毀選取到的專案單板
	bool                       ClearProjectAllBoards();//刪除專案單板
	bool                       LayoutProjectBoardList();//重整專案的單板列表
	bool                       SelectProjectAllBoards(bool value);//選取專案的所有單板
	bool                       SelectProjectBoardsByStage(const TREGION4D &Rgn, bool ResultMode, std::vector<CAOIBoard*> &BoardList);//選取單板	
	CAOIBoard*                 GetProjectBoardPtrByLastOne();//取得專案單板指標-依據最末個
	CAOIBoard*                 GetProjectBoardPtrByFirstOne();//取得專案單板指標-依據最前個
	CAOIBoard*                 GetProjectBoardPtrBySelected();//取得專案單板指標-依據選取到	
	CAOIBoard*                 GetProjectBoardPtrByBoardUUID(const UUID &uuid);//取得專案單板指標-依據萬用字碼
	size_t                     GetProjectBoardSelectedCount() const;//取得專案單板選取到的數量
	bool                       GetProjectBoardSelected(std::vector<CAOIBoard*> &SelBoardList);//取得專案單板選取到
	bool                       RemoveProjectBoardSelected();//移除選取到的專案單板
	bool                       DeleteProjectBoardSelected();//刪除選取到的專案單板
	bool                       RotateProjectBoardSelected(double Angle);//選轉單板角度
	bool                       InvertSelectProjectBoard(bool bUpdate);//反向選取單板
	bool                       MoveProjectBoardSelected(double dX, double dY);//移動專案選取到的單板
	bool                       MirrorXProjectBoardSelected();//鏡射專案選取到單板的X座標
	bool                       MirrorYProjectBoardSelected();//鏡射專案選取到單板的Y座標
	bool                       SwitchProjectBoardBypassed();//切換專案單板不檢測模式
	bool                       ChangeProjectBoardSelectedPanel(CAOIPanel *PanelPtr);//切換專案選取單板的整板指標
	bool                       CheckProjectBoardValid(const CAOIBoard *RefBoardPtr);//確認專案的單板指標有效
	bool                       PasteProjectBoardList(std::vector<CAOIBoard*> &CloneBoardList, double StageOffsetX, double StageOffsetY);//貼上專案單板
	bool                       ArrayPasteProjectBoardList(std::vector<CAOIBoard*> &CloneBoardList, const std::vector<TPOINT2D> &PosList);//陣列貼上專案單板
	bool                       SetProjectBoardSelectedBarcodeCodeIndex(int BarcodeCodeIndex);//設定專案單板條碼序號
	bool                       SetProjectBoardSelectedBarcodeDeviceIndex(int BarcodeDeviceIndex);//設定專案單板條碼機序號	
	bool                       CheckProjectBoardAllObjectInOneField() const;//確認專案單板所有物件在1個視野內
	bool                       ResetProjectBoardPanelFieldParam();//清除專案單板整板區域參數
	//---------------------------------------------------------------------------------//
	size_t                     GetProjectFdSortCount() const;//取得專案定位點排序數量
	CAOIFd*                    GetProjectFdSortPtr(size_t index, bool check) const;//取得專案定位點排序指標
	//---------------------------------------------------------------------------------//
	size_t                     GetProjectFdCount() const;//取得專案定位點數量	
	size_t                     CalcProjectPanelFdCount(DISTRICT_ID DistrictID) const;//計算專案整板定位點數量
	size_t                     CalcProjectBoardFdCount(DISTRICT_ID DistrictID) const;//計算專案單板定位點數量
	size_t                     GetProjectFdCount(DISTRICT_ID DistrictID, bool PanelFdOnly) const;//取得專案定位點數量	
	CAOIFd*                    GetProjectFdPtr(size_t index, bool check) const;//取得專案定位點指標
	CAOIFd*                    AddProjectFdPtr(CAOIFd *FdPtr, bool clone);//增加專案定位點
	bool                       DestroyProjectFdSelected();//摧毀選取到的專案定位點	
	bool                       ClearProjectAllFds();//刪除專案定位點
	bool                       LayoutProjectFdList();//重整專案的定位點列表			
	bool                       SortProjectFdListFull();//排序專案定位點列表-全部	
	bool                       SelectProjectAllFds(bool value);//選取專案的所有定位點
	bool                       ClearProjectFdImageBuffer();//清除專案定位點影像資料
	bool                       SelectProjectFdByObjectSelected(bool bPanelFd, bool bBoardFd);//依據專案選取道的零件選取專案的定位點		
	bool                       SelectProjectFdForTuning(bool bOffline, bool bNeedGrabFd);//選取專案定位點來調適專案
	CAOIFd*                    GetProjectFdPtrBySelected();//取得專案定位點指標-依據選取到	
	bool                       GetProjectFdSelected(std::vector<CAOIFd*> &SelFdList);//取得專案定位點選取到
	size_t                     GetProjectFdSelectedCount() const;//計算專案定位點選取到數量	
	bool                       GetProjectBoardFdSelected(std::vector<CAOIFd*> &SelFdList);//取得專案單板定位點選取到
	bool                       DeleteProjectFdSelected();//刪除選取到的專案定位點
	bool                       CheckProjectFdInsideStageLimit();//確認專案定位點落於機台內
	bool                       CheckProjectFdValid(const CAOIFd *RefFdPtr);//確認專案的定位點指標有效	
	bool                       SelectProjectFdsByStage(const TREGION4D &Rgn, bool ResultMode, std::vector<CAOIFd*> &FdList);//選取定位點
	bool                       SetProjectFdNeedToCalculate(bool val);//設定專案定位點需要去檢測	
	bool                       SetProjectBoardFdNeedToCalculateByBarcode();//設定專案單板定位點需要去檢測-依據條碼	
	bool                       RestoreProjectFdNeedToCalculate();//還原專案定位點需要去檢測	
	bool                       BackupProjectFdNeedToCalculate();//備份專案定位點需要去檢測
	bool                       CheckProjectFdUse3DLight_Panel() const;//確認專案定位點使用3D燈源-整板
	bool                       CheckProjectFdUse3DLight_Board() const;//確認專案定位點使用3D燈源-單板
	bool                       UpdateProjectFdToOtherByGroupID(CAOIFd *FdPtr);//更新專案定位點至其他定位點-依據群組編號
	bool                       CheckProjectFdReadyToInspection();//確認定位點準備好檢測
	bool                       PasteProjectFdToOtherBoards(std::vector<CAOIFd*> &CloneFdList);//將定位點貼上其餘整板單板上	
	bool                       FilterProjectFdAdded(const std::vector<CAOIFd*> &SelList, const std::vector<CAOIFd*> &SelList2, std::vector<CAOIFd*> &AddList);//建立專案單板定位點新增加
	//---------------------------------------------------------------------------------//		
	size_t                     GetProjectFdSortedCount() const;//取得專案排序好的專案定位點數量
	CAOIFd*                    GetProjectFdSortedPtr(size_t index, bool check);//取得專案定排序好的定位點指標
	CAOIFd*                    GetProjectFdLastSortedPtr(DISTRICT_ID DistrictID);//取得專案定排序好的最末定位點指標
	//---------------------------------------------------------------------------------//		
	BOARD_FD_GRAB_MODE         CheckProjectBoardFdGragMode();//確認專案單板定位點取像模式
	BOARD_FD_GRAB_MODE         GetProjectBoardFdGrabMode() const { return m_ProjectBoardFdGrabMode; }//專案單板定位點取像模式	
	//---------------------------------------------------------------------------------//	
	size_t                     GetProjectMapRgnCount() const;//取得專案底圖區域數量
	CAOIRgn*                   GetProjectMapRgnPtr(size_t index, bool check) const;//取得專案底圖區域指標
	CAOIRgn*                   AddProjectMapRgnPtr(CAOIRgn *MapRgnPtr, bool clone);//增加專案底圖區域
	bool                       ClearProjectAllMapRgns();//刪除專案底圖區域
	//---------------------------------------------------------------------------------//
	size_t                     GetProjectMarkCount() const;//取得專案特徵點數量
	CAOIMark*                  GetProjectMarkPtr(size_t index, bool check) const;//取得專案特徵點指標
	CAOIMark*                  AddProjectMarkPtr(CAOIMark *MarkPtr, bool clone);//增加專案特徵點
	bool                       CheckProjectMarkUseDrawRoughLine() const;//確認專案特徵點使用粗糙線段
	bool                       DestroyProjectMarkSelected();//摧毀選取到的專案特徵點
	bool                       ClearProjectAllMarks();//刪除專案特徵點
	bool                       LayoutProjectMarkList();//重整專案的特徵點列表	
	bool                       SelectProjectAllMarks(bool value);//選取專案的所有特徵點
	CAOIMark*                  GetProjectMarkPtrBySelected();//取得專案特徵點指標-依據選取到	
	CAOIMark*                  GetProjectMarkPtrByMarkUUID(const UUID &uuid);//取得專案特徵點指標-依據萬用字碼
	bool                       RotateProjectMarkSelected(double Angle);//旋轉專案選取到特徵點
	bool                       GetProjectMarkSelected(std::vector<CAOIMark*> &SelMarkList);//取得專案特徵點選取到
	size_t                     GetProjectMarkSelectedCount() const;//計算專案特徵點選取到數量	
	bool                       BypassProjectMarkSelected();//不檢測選取到的專案特徵點
	bool                       DeleteProjectMarkGroup();//刪除同群組的專案特徵點
	bool                       DeleteProjectMarkSelected();//刪除選取到的專案特徵點
	bool                       DeleteProjectMarkUnselected();//刪除未選取到的專案特徵點
	bool                       DeleteProjectMarkGroupOthers();//刪除同群組其他的專案特徵點
	bool                       CheckProjectMarkValid(const CAOIMark *RefMarkPtr);//確認專案的特徵點指標有效
	bool                       SelectProjectMarksByStage(const TREGION4D &Rgn, bool ResultMode, std::vector<CAOIMark*> &MarkList);//選取特徵點	
	bool                       PasteProjectMarkToOtherBoards(std::vector<CAOIMark*> &CloneMarkList);//將特徵點貼上其餘整板單板上	
	bool                       SetProjectMarkNeedToCalculate(bool val);//設定專案特徵點需要去檢測	
	bool                       RestoreProjectMarkNeedToCalculate();//還原專案特徵點需要去檢測	
	bool                       BackupProjectMarkNeedToCalculate();//備份專案特徵點需要去檢測	
	bool                       UpdateProjectMarkToOtherByGroupID(CAOIMark *MarkPtr);//更新專案特徵點至其他特徵點-依據群組編號
	bool                       SetProjectMarkSelectedSpaceBasePlaneParam(const TBasePlaneParam& Param);//設定專案特徵點空間基準面參數
	bool                       SetProjectMarkSelectedSpaceNoiseFilterParam(const TNoiseFilterParam& Param);//設定專案特徵點空間雜訊過濾參數
	bool                       EnableProjectMarkSelectedMaskFunc_Base(bool bEnable);//啟用專案特徵點遮罩函式
	bool                       SetProjectMarkSelectedMaskColorIndex_Base(int ColorIndex);//設定專案特徵點遮罩畫面
	bool                       SetProjectMarkSelectedLocalBasePlaneID(int BasePlaneID);//設定專案特徵點局部基準面編號
	//---------------------------------------------------------------------------------//
	bool                       CheckProjectPartGroupSetting();//確認專案零件群組的設定
	size_t                     GetProjectPartGroupCount() const;//取得專案零件群組數量
	bool                       CheckProjectPartGroupPtr(CAOIPartGroup *PartGroupPtr);//增加專案零件群組
	CAOIPartGroup*             GetProjectPartGroupPtr(size_t index, bool check) const;//取得專案零件群組指標
	CAOIPartGroup*             AddProjectPartGroupPtr(CAOIPartGroup *PartGroupPtr);//增加專案零件群組
	bool                       DestroyProjectPartGroupSelected();//摧毀選取到的專案零件群組
	bool                       ClearProjectAllPartGroups();//刪除專案零件群組
	bool                       LayoutProjectPartGroupList();//重整專案的零件群組列表	
	bool                       SelectProjectAllPartGroups(bool value);//選取專案的所有零件群組
	CAOIPartGroup*             GetProjectPartGroupPtrBySelected();//取得專案零件群組指標-依據選取到		
	bool                       GetProjectPartGroupSelected(std::vector<CAOIPartGroup*> &SelGroupList);//取得專案零件群組選取到	
	size_t                     GetProjectPartGroupSelectedCount() const;//計算專案零件群組選取到數量	
	bool                       RemoveProjectPartGroupComponentSelected();//移除專案零件選取到的零件群組
	/*
	bool                       DeleteProjectPartGroupkGroup();//刪除同群組的專案零件群組
	bool                       DeleteProjectPartGroupSelected();//刪除選取到的專案零件群組
	bool                       DeleteProjectPartGroupUnselected();//刪除未選取到的專案零件群組
	bool                       CheckProjectPartGroupValid(const CAOIPartGroup *RefGroupPtr);//確認專案的零件群組指標有效	
	bool                       PasteProjectPartGroupToOtherBoards(std::vector<CAOIPartGroup*> &CloneGroupList);//將零件群組貼上其餘整板單板上		
	bool                       UpdateProjectPartGroupToOtherByGroupID(CAOIPartGroup *GroupPtr);//更新專案零件群組至其他群組-依據群組編號	
	*/
	bool                       AssignProjectPartGroupToPanelBoard();//分配專案的零件群組至整板與單板
	//---------------------------------------------------------------------------------//	
	size_t                     GetProjectBarcodeCount() const;//取得專案軟體條碼數量
	CAOIBarcode*               GetProjectBarcodePtr(size_t index, bool check) const;//取得專案軟體條碼指標
	CAOIBarcode*               AddProjectBarcodePtr(CAOIBarcode *BarcodePtr, bool clone);//增加專案軟體條碼
	bool                       DestroyProjectBarcodeSelected();//摧毀選取到的專案軟體條碼
	bool                       ClearProjectAllBarcodes();//刪除專案軟體條碼
	bool                       LayoutProjectBarcodeList();//重整專案的軟體條碼列表	
	bool                       SelectProjectAllBarcodes(bool value);//選取專案的所有軟體條碼	
	CAOIBarcode*               GetProjectBarcodePtrBySelected();//取得專案軟體條碼指標-依據選取到	
	size_t                     GetProjectBarcodeEnabledCount() const;//取得專案軟體條碼啟用數量
	bool                       RotateProjectBarcodeSelected(double Angle);//旋轉專案選取到條碼
	bool                       GetProjectBarcodeSelected(std::vector<CAOIBarcode*> &SelBarcodeList);//取得專案軟體條碼選取到
	size_t                     GetProjectBarcodeSelectedCount() const;//計算專案軟體條碼選取到數量	
	bool                       DeleteProjectBarcodeSelected();//刪除選取到的專案軟體條碼
	bool                       DeleteProjectBarcodeUnselected();//刪除未選取到的專案軟體條碼
	bool                       CheckProjectBarcodeValid(const CAOIBarcode *RefBarcodePtr);//確認專案的軟體條碼指標有效
	bool                       SelectProjectBarcodesByStage(const TREGION4D &Rgn, bool ResultMode, std::vector<CAOIBarcode*> &BarcodeList);//選取軟體條碼
	bool                       PasteProjectBarcodeToOtherPanels(std::vector<CAOIBarcode*> &CloneBarcodeList);//將軟體條碼貼上其餘整板整板上	
	bool                       PasteProjectBarcodeToOtherBoards(std::vector<CAOIBarcode*> &CloneBarcodeList);//將軟體條碼貼上其餘整板單板上	
	bool                       AnalysisProjectBarcodes();//分析專案條碼是否正常
	bool                       AssignProjectBarcodeToBoards();//分配專案條碼
	bool                       ExpandProjectBarcodeToPanels();//擴展專案條碼
	bool                       ExpandProjectBarcodeToBoards();//擴展專案條碼
	bool                       ToggleProjectBarcodeNeedToCalculate();//切換專案條碼需要去檢測
	bool                       SetProjectBarcodeNeedToCalculate(bool val);//設定專案條碼需要去檢測	
	bool                       RestoreProjectBarcodeNeedToCalculate();//還原專案條碼需要去檢測	
	bool                       BackupProjectBarcodeNeedToCalculate();//備份專案條碼需要去檢測	
	bool                       UpdateProjectBarcodeToOtherByGroupID(CAOIBarcode *BarcodePtr);//更新專案條碼至其他條碼-依據群組編號
	bool                       SetProjectBarcodeSelectedLocalBasePlaneID(int BasePlaneID);//設定專案條碼局部基準面編號
	//---------------------------------------------------------------------------------//
	size_t                     GetProjectComponentCount() const;//取得專案零件數量
	size_t                     GetProjectComponentCount(DISTRICT_ID DistrictID) const;//取得專案零件數量
	size_t                     GetProjectComponentBypassCount() const;//取得專案不檢測零件數量
	size_t                     GetProjectComponentBypassCount(DISTRICT_ID DistrictID) const;//取得專案不檢測零件數量
	size_t                     GetProjectComponentNotAgentCount() const;//取得專案非代理零件數量	
	size_t                     GetProjectComponentNotAgentCount(DISTRICT_ID DistrictID) const;//取得專案非代理零件數量
	CAOIComponent*             GetProjectComponentPtr(size_t index, bool check) const;//取得專案零件指標
	CAOIComponent*             AddProjectComponentPtr(CAOIComponent *ComponentPtr, bool clone);//增加專案零件
	bool                       AddProjectComponentList(std::vector<CAOIComponent*> &List);//增加專案零件列表
	bool                       CheckProjectComponentBarcode();//確認專案零件條碼
	bool                       CheckProjectComponentUseDrawRoughLine() const;//確認專案零件使用粗糙線段
	bool                       DestroyProjectComponentSelected();//摧毀選取到的專案零件
	bool                       DeleteProjectComponentType(COMPONENT_TYPE Type);//摧毀特定的專案零件樣式
	bool                       ClearProjectAllComponents();//刪除專案零件
	bool                       LayoutProjectComponentList();//重整專案的零件列表	
	bool                       SelectProjectAllComponents(bool value);//選取專案的所有零件		
	bool                       DeleteProjectComponentSelected();//刪除選取到的專案零件
	bool                       DeleteProjectComponentAgentInvalid(const std::vector<CAOIComponent*> &AgentList);//刪除無效的代理零件
	bool                       ReLinkProjectComponentMasterAgent(CAOIComponent *Master, const std::vector<CAOIComponent*> &ComponentMapList); //重新連結專案零件本尊與代理人
	bool                       ReLinkProjectComponentMasterAgentList(const std::vector<CAOIComponent*> &MasterList, const std::vector<CAOIComponent*> &ComponentMapList); //重新連結專案零件本尊與代理人
	bool                       BypassProjectComponentSelected(bool bBypassed);//不檢測選取到的專案零件
	bool                       BackupProjectComponentSelected();//備份選取到的零件
	bool                       RestoreProjectComponentSelected();//還原選取到的零件
	bool                       SelectProjectComponentsByColRowIndex(int ColMode, int RowMode);//選取零件
	bool                       SelectProjectComponentsBySelectedCadCp(const std::vector<CAOIComponent*> &SelComponentList, bool bXPos, bool bYPos);//選取零件
	bool                       SelectProjectComponentsByStage(const TREGION4D &Rgn, bool ResultMode, std::vector<CAOIComponent*> &ComponentList);//選取零件		
	bool                       SelectProjectComponentByMultiBoardCtrlMode(std::vector<CAOIComponent*> &SelComponentList, MULTI_BOARD_CTRL_MODE CtrlMode);//選取零件
	bool                       SelectProjectComponentsByComponentType(COMPONENT_TYPE ComponentType);
	size_t                     CheckProjectComponentCountByComponentType(COMPONENT_TYPE ComponentType);	
	bool                       SelectProjectComponentsByModelName(LPCTSTR ModelName, bool ModelUnSet);	
	bool                       SelectProjectComponentsByPartNumber(LPCTSTR PartNumber, bool ModelUnSet);
	bool                       SelectProjectComponentsByModelGroup(LPCTSTR ModelGroupName, bool ModelUnSet);		
	bool                       SelectProjectComponentsForModelBkImage();//選取專案零件-為了模組底圖
	bool                       RemoveProjectComponentAgnetSelected();//移除專案零件選取到的代理人
	bool                       SelectProjectComponentsMaster();//選取專案零件-為了本尊與代理人
	bool                       SelectProjectComponentsMasterAgent();//選取專案零件-為了本尊與代理人
	CAOIComponent*             GetProjectComponentPtrByHighest();//取得專案零件指標-依據本體最高
	CAOIComponent*             GetProjectComponentPtrBySelected();//取得專案零件指標-依據選取到	
	CAOIComponent*             GetProjectComponentPtrByModelName(LPCTSTR ModelName);//取得專案零件指標-依據料號
	CAOIComponent*             GetProjectComponentPtrByPartNumber(LPCTSTR PartNumber);//取得專案零件指標-依據料號	
	CAOIComponent*             GetProjectComponentPtrByComponentUUID(const UUID &uuid);//取得專案零件指標-依據萬用字碼
	CAOIComponent*             GetProjectComponentPtrByComponentName(LPCTSTR ComponentName);//取得專案零件指標-依據名稱	
	CAOIComponent*             GetProjectComponentPtrByComponentFullName(LPCTSTR ComponentName, bool bModelIsolated);//取得專案零件指標-依據全名
	CAOIComponent*             GetProjectComponentPtrByComponentFullName(int PanelIndex, int BoardIndex, LPCSTR ComponentName);//取得專案零件指標-依據全名
	CAOIComponent*             GetProjectComponentPtrByComponentFullName(int PanelIndex, int BoardIndex, LPCWSTR ComponentName);//取得專案零件指標-依據全名
	size_t                     GetProjectComponentSelectedCount() const;//計算專案零件選取到數量	
	bool                       GetProjectComponentSelected(std::vector<CAOIComponent*> &SelComponentList);//取得專案零件選取到			
	bool                       GetProjectComponentModelIsolated(std::vector<CAOIComponent*> &SelComponentList);//取得專案零件模組隔離的
	bool                       ApplyProjectCompnentModelToLibraryGroup(TModelUpdateToGroupParam Param, CAOIModel *ModelPtr);//更新零件模組至資料庫群組
	bool                       ApplyProjectCompnentModelToLibraryModel(CAOIComponent *ComponentPtr, bool UpdateToeOthers);//更新零件模組至資料庫	
	bool                       SyncProjectCompnentModelToLibraryModel(CAOIComponent *ComponentPtr, bool bPartial, bool UpdateToeOthers);//同步化零件模組至資料庫
	bool                       RotateProjectComponentSelected(double Angle);//選轉零件角度
	bool                       ReverseProjectComponentSelected();//反轉零件角度
	bool                       InvertSelectProjectComponent();//反向選取零件	
	bool                       SetProjectComponentSelectedSaveLeadReport(bool bSaved);//設定專案選取到零件的儲存引腳報告
	bool                       SetProjectComponentSelectedNozzleName(LPCTSTR NozzleName);//設定專案選取到零件的吸嘴名稱
	bool                       SetProjectComponentSelectedPartNumber(LPCTSTR PartNumber);//設定專案選取到零件的料號
	bool                       SetProjectComponentName(LPCTSTR RefName, LPCTSTR NewName);//設定專案零件的名稱
	bool                       SetProjectComponentPartNumber(LPCTSTR RefPartNumber, LPCTSTR NewPartNumber);//設定專案零件的料號
	bool                       SetProjectComponentSelectedComponentName(LPCTSTR RefName, LPCTSTR NewName);//設定專案選取到零件的名稱
	bool                       SetProjectComponentSelectedDefectItemEssential(const CWndDefectItem &DefectItem);//設定專案選取到零件瑕疵必要項目
	bool                       SetProjectComponentSelectedDefectItemRecheck_ARS(const CWndDefectItem &DefectItem);//設定專案選取到零件瑕疵重複確認-ARS
	bool                       MoveProjectComponentSelected(double dX, double dY);//移動專案選取到的零件	
	bool                       MirrorXProjectComponentSelected();//鏡射專案選取到零件的X座標
	bool                       MirrorYProjectComponentSelected();//鏡射專案選取到零件的Y座標
	bool                       ResetProjectComponentModelSelected();//復歸選取到的專案零件模組
	bool                       ChangeProjectComponentSelectedBoard(CAOIBoard *RefBoardPtr);//切換專案選取單板的整板指標
	bool                       CheckProjectComponentValid(const CAOIComponent *RefComponentPtr);//確認專案的零件指標有效		
	bool                       GetProjectComponentModelInfoList(bool bModelIsolated, std::vector<TModelInfo> &ModelInfoList);//取得專案零件模組資訊列表
	bool                       PasteProjectComponentList(std::vector<CAOIComponent*> &CloneComponentList, double StageOffsetX, double StageOffsetY, double RotateAngle);//貼上專案零件
	bool                       PasteProjectComponentListToOtherBoards(std::vector<CAOIComponent*> &CloneComponentList);//貼上專案零件至其餘單板上
	bool                       PasteProjectComponentListToOtherPanels(std::vector<CAOIComponent*> &CloneComponentList);//貼上專案零件至其餘整板上
	bool                       ArrayPasteProjectComponentList(std::vector<CAOIComponent*> &CloneComponentList, const std::vector<TPOINT2D> &PosList, const std::vector<POINT> &IdxList, int NameMode, bool ChangeSelName);//陣列貼上專案零件	
	bool                       SwitchProjectComponentBypassed();//切換專案零件不檢測模式	
	bool                       SwitchProjectComponentBypass3D();//切換專案零件3D不檢測模式	
	bool                       SwitchProjectComponentXBoardUnit();//切換專案零件報廢件模式 	
	bool                       SwitchProjectComponentModelIsolated();//切換專案零件模組隔離	
	bool                       SyncProjectCompoenntSelectedToModel();//同步化零件選取至模組內
	bool                       UpdateProjectComponentModelIsolatedFolder();//更新專案零件模組隔離資料夾
	bool                       SetProjectComponentNeedToCalculate(bool val);//設定專案零件需要去檢測	
	bool                       RestoreProjectComponentNeedToCalculate();//還原專案零件需要去檢測	
	bool                       BackupProjectComponentNeedToCalculate();//備份專案零件需要去檢測	
	bool                       SortProjectComponentList();//排序專案零件列表	
	bool                       RestoreProjectComponentCadPosBySelected();//恢復專案零件選取到的Cad座標
	bool                       SetProjectComponentSelectedCadPosByResult(bool UsePadResult);//設定專案零件選取到的Cad座標-結果座標
	bool                       SetProjectComponentSelectedSaveWndList(bool bSave);//設定專案零件是否儲存檢測框列表
	bool                       SetProjectComponentSelectedEnableAlarm(bool bAlarm);//設定專案零件是否停機警報	
	bool                       SetProjectComponentSelectedEnableAlarmOnAOI(bool bAlarm);//設定專案零件是否機台警報	
	bool                       SetProjectComponentSelectedEnableAlarmOnARS(bool bAlarm);//設定專案零件是否維修站顯示
	bool                       SetProjectComponentSelectedEnableDefectCountOnARS(bool bAlarm);//設定專案零件是否計數維修站顯示
	bool                       SetProjectComponentSelectedDefectAlarmAOI(DEFECT_PARAM_FROM_MODE From, const CWndDefectItem &Item);//設定專案零件停機警報參數
	bool                       SetProjectComponentSelectedDefectAlarmARS(DEFECT_PARAM_FROM_MODE From, const CWndDefectItem &Item);//設定專案零件停機警報參數
	bool                       SetProjectComponentSelectedSaveReportARS(bool bSave);//設定專案零件是否儲存報告-維修站
	bool                       SetProjectComponentSelectedEnableSelfField(bool bEnabled);//設定專案零件是否啟用專屬區域
	bool                       SetProjectComponentSelectedSpaceBasePlaneParam(const TBasePlaneParam& Param, double RefAngle);//設定專案零件空間基準面參數
	bool                       SetProjectComponentSelectedSpaceNoiseFilterParam(const TNoiseFilterParam& Param);//設定專案零件空間雜訊過濾參數
	bool                       EnableProjectComponentSelectedMaskFunc_Base(bool bEnable);//啟用專案零件遮罩函式
	bool                       SetProjectComponentSelectedMaskColorIndex_Base(int ColorIndex);//設定專案零件遮罩畫面
	bool                       SetProjectComponentSelectedMaskFrameUniqueID_Base(unsigned int FrameIndex, unsigned int FrameUniqueID);//設定專案零件遮罩畫面	
	bool                       SetProjectComponentSelectedMaskExtendSize_Body(double ExtendW, double ExtendH);//設定專案零件遮罩外擴尺寸-本體
	bool                       SetProjectComponentSelectedMaskExtendSize_Land(double ExtendW, double ExtendH);//設定專案零件遮罩外擴尺寸-焊盤
	bool                       SetProjectComponentSelectedGroupID(int GroupID);//設定專案零件群組編號
	bool                       SetProjectComponentSelectedGroupOrg(bool bOrg);//設定專案零件群組圓點
	bool                       SetProjectComponentSelectedLocalBasePlaneID(int BasePlaneID);//設定專案零件局部基準面編號
	bool                       SetProjectComponentSelectedDataModelParam(int Param);//設定專案零件資料物件參數
	bool                       AddProjectComponentAgentByBomNodeList(const std::vector<TComponentNode> &BomNodeList);//加入專案零件代理人-載入BOM列表
	int                        GetCreateProjectComponentForFullProjectMapMode() const;//建立專案零件-模式
	bool                       CreateProjectComponentForFullProjectMap();//建立專案零件-全底圖	
	bool                       CreateProjectComponentForSpecRegion(TREGION4D SpecRegion);//建立專案零件-限定區域
	bool                       CheckProjectComponentReadyToInspection();//確認零件準備好檢測
	bool                       CheckProjectComponentBarcodeReadyToInspection();//確認零件條碼準備好檢測	
	bool                       ChangeProjectComponentParam(const std::vector<std::wstring> &NameList, CHANGE_COMPARE_PARAM_MODE ChangeMode, bool bResetAll);//變更零件參數 
	bool                       ResetProjectComponentNPM_APC_FF1();//設定專案零件的NPM-APC-FF1參數
	bool                       ResetProjectComponentNPM_APC_MFB();//設定專案零件的NPM-APC-MFB參數
	bool                       ResetProjectComponentNPM_APC_Param();//復歸專案零件的NPM_APC參數	
	bool                       BuildProjectComponentNPM_APC_List(std::vector<CSortObj> &SortList);//建立專案零件的APC列表
	bool                       SetProjectComponentNPM_APC_FF1(unsigned int IDNUM, int PNUM, LPCTSTR CName, double MffX, double MffY, double MffA);//設定專案零件的NPM-APC-FF1參數
	bool                       SetProjectComponentNPM_APC_MFB(unsigned int IDNUM, int PNUM, LPCTSTR CName, double EPosX, double EPosY, double EPosA);//設定專案零件的NPM-APC-FF3參數
	//---------------------------------------------------------------------------------//	
	bool                       UpdateProjectSpaceBasePlaneParam(const TBasePlaneParam &Param);//更新專案空間基準面參數
	bool                       UpdateProjectSpaceBasePlaneParamList(const std::vector<TBasePlaneParam> &ParamList);//更新專案空間基準面參數列表
	//---------------------------------------------------------------------------------//
	bool                       UpdateProjectSpaceNoiseFilterParam(const TNoiseFilterParam &Param);//更新專案空間過濾參數
	bool                       UpdateProjectSpaceNoiseFilterParamList(const std::vector<TNoiseFilterParam> &FilterList);//更新專案空間過濾參數列表
	//---------------------------------------------------------------------------------//
	bool                       CheckProjectSpecTestEnabled() const;//確認專案特殊檢測啟用
	bool                       SaveProjectSpecTestField(CAOIField *FieldPtr);//儲存專案特殊檢測視野
	bool                       TestProjectSpecialInspection(CAOIField *FieldPtr);//測試專案特殊檢測
	//---------------------------------------------------------------------------------//		
	bool                       CreateProjectFieldMapMaskImage(IMAGE_SIZE MaskW, IMAGE_SIZE MaskH, IMAGE_SIZE MaskStep, MASK_PTR MaskPtr, const TREGION4D &FieldRgn);//建立區域底圖遮罩圖	
	bool                       CreateProjectFieldMapImage(int MapIndex, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_PTR ImagePtr, const TREGION4D &FieldRgn);//建立區域底圖	
	bool                       CreateProjectPartMaskImage(IMAGE_SIZE MaskW, IMAGE_SIZE MaskH, IMAGE_SIZE MaskStep, MASK_PTR MaskPtr, const TREGION4D &StageRgn, const TPOINT2D &ImageRes, int MaskMode, MASK_DATA mskPart);//建立零件遮罩圖
	bool                       CreateProjectPartMaskImage_v2(IMAGE_SIZE MaskW, IMAGE_SIZE MaskH, IMAGE_SIZE MaskStep, MASK_PTR MaskPtr, const TREGION4D &StageRgn, const TPOINT2D &ImageRes, int MaskMode, MASK_DATA mskPart);//建立零件遮罩圖
	bool                       CreateProjectColorMaskImage(IMAGE_SIZE MaskW, IMAGE_SIZE MaskH, IMAGE_SIZE MaskStep, MASK_PTR MaskPtr, RECT MapRoiRect, MASK_DATA msk, const char *fnName, std::vector<CString> &SaveFileList);//建立顏色遮罩
	bool                       CreateProjectSkewMaskImage(CAOIField *FieldPtr,IMAGE_SIZE MaskW, IMAGE_SIZE MaskH, IMAGE_SIZE MaskStep, MASK_PTR MaskPtr, RECT MapRoiRect, MASK_DATA msk, const char *fnName, std::vector<CString> &SaveFileList);//建立顏色遮罩
	bool                       CreateProjectComponentByBlob(IMAGE_SIZE MaskW, IMAGE_SIZE MaskH, IMAGE_SIZE MaskStep, MASK_PTR MaskPtr, MASK_PTR SpaceMaskPtr, SPACE_PTR SpacePtr, const TREGION4D &StageRgn, const TPOINT2D &ImageRes, double PartW, double PartH, double PartWHRatio, double MinThickness, bool bMeasure, std::vector<CAOIComponent*> &ComponentList);//建立零件列表
	bool                       CreateProjectComponentByBlob(IMAGE_SIZE MaskW, IMAGE_SIZE MaskH, IMAGE_SIZE MaskStep, MASK_PTR MaskPtr, MASK_PTR SpaceMaskPtr, SPACE_PTR SpacePtr, const TREGION4D &StageRgn, const TPOINT2D &ImageRes, const CJetGroundEquation &GroundEuation, double PartW, double PartH, double PartWHRatio, double MinThickness, bool bMeasure, std::vector<CAOIComponent*> &ComponentList);//建立零件列表
	//---------------------------------------------------------------------------------//		
	bool                       CreateProjectComponentScatchByBlob(IMAGE_SIZE MaskW, IMAGE_SIZE MaskH, IMAGE_SIZE MaskStep, MASK_PTR MaskPtr, MASK_PTR SpaceMaskPtr, SPACE_PTR SpacePtr, const TREGION4D &StageRgn, const TPOINT2D &ImageRes, double PartW, double PartH, double PartD, double PartArea, double PartWHRatio, double MinThickness, bool bMeasure, std::vector<CAOIComponent*> &ComponentList, std::vector<RECT> &RectList);//建立零件列表
	//---------------------------------------------------------------------------------//		
	//拋件檢測	
	size_t                     GetProjectDropOutPartCount() const;//取得專案拋件數量
	CAOIComponent*             GetProjectDropOutPartPtr(size_t index, bool check) const;//取得專案拋件指標
	CAOIComponent*             AddProjectDropOutPartPtr(CAOIComponent *ComponentPtr, bool clone);//增加專案拋件
	bool                       ClearProjectAllDropOutParts();//刪除專案拋件	
	bool                       ClearXBoardDropOutParts();//刪除報廢版專案拋件	
	int                        GetProjectTestDropOutPart() const;//檢測拋件-啟用
	void                       SetProjectTestDropOutPart(int val);//檢測拋件-啟用
	int                        GetProjectDropOutPartSaveImage() const;//檢測拋件-存圖
	void                       SetProjectDropOutPartSaveImage(int val);//檢測拋件-存圖	
	bool                       BuildProjectDropOutComponentList();//合併程專案拋件列表
	bool                       SaveProjectDropOutPartLog(LPCTSTR str);
	bool                       TestProjectDropOutPart(CAOIField *FieldPtr);		
	bool                       TestProjectDropOutPartFn(CAOIField *FieldPtr);
	bool                       TestProjectDropOutPartBy3DHeight(CAOIField *FieldPtr);	
	bool                       TestProjectDropOutPartByMapCompare(CAOIField *FieldPtr);		
	bool                       TestProjectDropOutPartByColorFilter(CAOIField *FieldPtr);
	bool                       ExecProjectDropOutPartMaskFilter(CAOIField *FieldPtr, const TPOINT2D &MapRes, IMAGE_SIZE MaskW, IMAGE_SIZE MaskH, IMAGE_SIZE MaskStep, MASK_PTR MaskPtr, MASK_PTR DstPtr, std::vector<CString> &SaveFileList);//執行拋件遮罩過濾	
	bool                       CreateProjectDropOutPartByComponentList(CAOIField *FieldPtr, std::vector<CAOIComponent*> &ComponentList);//依照零件列表建立拋件列表	
	bool                       TestProjectDropOutPartByMap();//拋件檢測-使用檢測底圖
	//---------------------------------------------------------------------------------//	
	//刮傷檢測
	size_t                     GetProjectScratchPartCount() const;//取得專案刮傷數量
	CAOIComponent*             GetProjectScratchPartPtr(size_t index, bool check) const;//取得專案刮傷指標
	CAOIComponent*             AddProjectScratchPartPtr(CAOIComponent *ComponentPtr, bool clone);//增加專案刮傷
	bool                       ClearProjectAllScratchParts();//刪除專案刮傷	
	int                        GetProjectTestScratchPart() const;//檢測刮傷-啟用
	void                       SetProjectTestScratchPart(int val);//檢測刮傷-啟用
	int                        GetProjectScratchPartSaveImage() const;//檢測刮傷-存圖
	void                       SetProjectScratchPartSaveImage(int val);//檢測刮傷-存圖
	bool                       BuildProjectScratchComponentList();//合併程專案刮傷列表
	bool                       SaveProjectScratchPartLog(LPCTSTR str);
	bool                       TestProjectScratchPart(CAOIField *FieldPtr);
	bool                       TestProjectScratchPartFn(CAOIField *FieldPtr);
	bool                       TestProjectScratchPartByColorFilter(CAOIField *FieldPtr);//檢測刮傷-使用抽色
	bool                       TestProjectScratchPart_v2(CAOIField *FieldPtr);//檢測刮傷-//Alan
	bool                       TestProjectScratchPart_v4(CAOIField *FieldPtr);
	bool                       CreateProjectScratchPartByComponentList(CAOIField *FieldPtr, std::vector<CAOIComponent*> &ComponentList);//依照零件列表建立刮傷列表
	//---------------------------------------------------------------------------------//	
	//檢測尺寸
	size_t                     GetProjectPartDimensionCount() const;//取得專案尺寸數量
	CAOIComponent*             GetProjectPartDimensionPtr(size_t index, bool check) const;//取得專案尺寸指標
	CAOIComponent*             AddProjectPartDimensionPtr(CAOIComponent *ComponentPtr, bool clone);//增加專案尺寸
	bool                       ClearProjectAllPartDimension();//刪除專案尺寸	
	int                        GetProjectPartDimensionEnabled() const;//檢測尺寸-啟用
	int                        GetProjectPartDimensionSaveImage() const;//檢測尺寸-存圖
	bool                       ApplyProjectDimensionComponentList();//套用程專案尺寸列表
	bool                       BuildProjectDimensionComponentList();//合併程專案尺寸列表
	bool                       TestProjectPartDimension(CAOIField *FieldPtr);
	bool                       TestProjectPartDimensionFn(CAOIField *FieldPtr);
	bool                       TestProjectPartDimensionBy3DHeight(CAOIField *FieldPtr);//檢測尺寸-使用高度
	bool                       ExecProjectPartDimensionMaskFilter(CAOIField *FieldPtr, const TPOINT2D &MapRes, IMAGE_SIZE MaskW, IMAGE_SIZE MaskH, IMAGE_SIZE MaskStep, MASK_PTR MaskPtr, MASK_PTR DstPtr, std::vector<CString> &SaveFileList);//執行尺寸遮罩過濾
	bool                       CreateProjectPartDimensionByComponentList(CAOIField *FieldPtr, std::vector<CAOIComponent*> &ComponentList);//依照零件列表建立尺寸列表	
	//---------------------------------------------------------------------------------//
	bool                       ClearProjectLibrary();//清除專案資料庫
	bool                       ArrangeProjectLibrary();//整理專案資料庫
	bool                       UpdateProjectLibraryFolder();//更新專案資料庫資料夾
	bool                       RemoveProjectLibraryFolderNoUsed();//移除專案資料庫資料夾-未使用
	bool                       BuildProjectDefaultLibrary();//建立專案預設的資料庫
	bool                       ApplyProjectLibraryConfiguration();//套用專案資料庫組態
	bool                       ApplyProjectLibraryToComponents();//套用專案資料庫至零件上		
	bool                       ApplyProjectLibraryToComponentsSelected();//更新模組至選取到的零件上
	bool                       DeleteProjectLibraryGroupWnd(CAOIModel *ModelPtr);//刪除專案資料庫群組檢測框
	//---------------------------------------------------------------------------------//
	size_t                     GetProjectModelCount() const;//取得專案模組數量
	CAOIModel*                 ReplaceProjectModel(CAOIModel *ModelPtr);//取代專案模組
	CAOIModel*                 GetProjectModelPtr(size_t index, bool check) const;//取得專案模組指標	
	CAOIModel*                 AddProjectModelPtr(CAOIModel *ModelPtr, bool clone);//增加專案模組	
	bool                       DestroyProjectModelSelected();//摧毀選取到的專案模組
	bool                       ClearProjectAllModels();//刪除專案模組
	bool                       LayoutProjectModelList();//重整專案的模組列表
	bool                       SelectProjectAllModels(bool value);//選取專案的所有模組
	bool                       DeleteProjectModelSelected();//刪除選取到的專案模組	
	bool                       DeleteProjectModelNameRepeated();//刪除重複名稱的專案模組	
	bool                       UnSelectProjectModel();//設定專案模組選取狀態	
	bool                       CheckProjectModelNameExist(LPCTSTR ModelName);	
	CAOIModel*                 GetProjectModelPtrByModelName(LPCTSTR ModelName);
	size_t                     GetProjectModelIndexByModelName(LPCTSTR ModelName);		
	CAOIModel*                 CloneProjectModel(LPCTSTR ModelName, LPCTSTR NewModelName);//複製模組		
	bool                       RenameProjectModel(LPCTSTR ModelName, LPCTSTR NewModelName);//變更模組名稱		
	bool                       SetProjectModelSelectedSaveLeadReport(bool bSaved);//設定專案所有模組存儲引腳報告
	bool                       SetProjectModelSelectedDefectItemEssential(const CWndDefectItem &DefectItem);//設定專案選取到模組瑕疵必要項目
	bool                       SetProjectModelSelectedDefectItemRecheck_ARS(const CWndDefectItem &DefectItem);//設定專案選取到模組瑕疵重複確認-ARS
	bool                       ApplyProjectModelToComponents(LPCTSTR ModelName);//更新模組至零件上
	bool                       ApplyProjectModelToComponents(CAOIModel *ModelPtr);//更新模組至選零件上
	bool                       ApplyProjectModelToComponentsSelected(LPCTSTR ModelName);//更新模組至選取到的零件上
	bool                       ApplyProjectModelToComponentsSelected(CAOIModel *ModelPtr);//更新模組至選取到的零件上
	bool                       CheckProjectModelValid(const CAOIModel *RefModelPtr);//確認專案的模組指標有效
	bool                       GetProjectModelNameList(std::vector<CString> &ModelNameList);//取得專案模組名稱列表
	bool                       GetProjectModelInfoList(std::vector<TModelInfo> &ModelInfoList);//取得專案模組資訊列表
	bool                       ActiveProjectComponentModelWnd(LPCTSTR ModelName, size_t WndIdx);//選取專案模組視窗
	bool                       ActiveProjectComponentModelWnd(LPCTSTR ModelName, CAOIModel *RefModelPtr, size_t WndIdx);//選取專案模組視窗
	bool                       ModifyProjectModelDefaultWndParam(TMODEL_DEFAULT_WND_PARAM &Param);//修正模組預設檢測框參數
	bool                       UpdateProjectModellToOtherModels(const CAOIModel *RefModelPtr, LPCTSTR KeyName, bool bIncluedUsed);//套用專案模組至其他模組內	
	bool                       ListProjectModelByGroupName(CAOIModel *ModelPtr, std::vector<CAOIModel*> &ModelList);//依群組名稱列出專案模組
	bool                       ListProjectModelByGroupName(MODEL_TYPE ModelType, LPCTSTR GroupName, std::vector<CAOIModel*> &ModelList);//依群組名稱列出專案模組	
	//---------------------------------------------------------------------------------//	
	bool                       BuildProjectModelList_Saved();//建立專案存檔模組列表
	void                       ClearProjectModelList_Saved();//清除專案存檔模組列表
	size_t                     GetProjectModelCount_Saved() const;//取得專案存檔模組數量	
	void                       AddProjectModelPtr_Saved(CAOIModel *ModelPtr);//增加專案存檔模組	
	CAOIModel*                 GetProjectModelPtr_Saved(size_t index, bool check) const;//取得專案存檔模組指標
	bool                       CopyProjectModelFolderTo(LPCTSTR DstFolder);//複製專案
	//---------------------------------------------------------------------------------//
	bool                       BuildProjectPartModelList_Saved();//建立專案零件存檔模組列表
	void                       ClearProjectPartModelList_Saved();//清除專案零件存檔模組列表
	size_t                     GetProjectPartModelCount_Saved() const;//取得專案零件存檔模組數量	
	void                       AddProjectPartModelPtr_Saved(CAOIModel *ModelPtr);//增加專案零件存檔模組	
	CAOIModel*                 GetProjectPartModelPtr_Saved(size_t index, bool check) const;//取得專案零件存檔模組指標		
	bool                       CopyProjectPartModelFolderTo(LPCTSTR DstFolder);//複製專案
	//---------------------------------------------------------------------------------//
	CAOIWnd*                   FindProjectWndByUUID(const UUID &uuid);//尋找專案內的檢測框
	CAOILand*                  FindProjectLandByUUID(const UUID &uuid);//尋找專案內的檢測框
	CAOIModel*                 FindProjectModelByUUID(const UUID &uuid);//尋找專案內的檢測框
	//---------------------------------------------------------------------------------//
	bool                       SyncProjectModelToComponentsSelected(LPCTSTR ModelName, bool bPartial);//同步化模組至選取到的零件上-不變更模組的框數量下的更新
	bool                       SyncProjectModelToComponentsSelected(CAOIModel *ModelPtr, bool bPartial);//同步化模組至選取到的零件上-不變更模組的框數量下的更新
	//---------------------------------------------------------------------------------//
	bool                       GetProjectModelGroupList(MODEL_TYPE ModelType, std::vector<CString> &GroupNameList);	
	bool                       CheckProjectModelGroupNameExist(LPCTSTR GroupName);//確認模組群組名稱是否存在
	MODEL_TYPE                 CheckProjectModelGroupNameModelType(LPCTSTR GroupName);//確認模組群組名稱的模組樣式
	bool                       CheckProjectModelGroupNameExist(MODEL_TYPE ModelType, LPCTSTR GroupName);//僅確認不同樣式	
	bool                       RenameProjectModelGroupName(MODEL_TYPE ModelType, LPCTSTR GroupName, LPCTSTR NewGroupName);//重設專案模組群組名稱
	bool                       ChangeProjectModelTypeByName(LPCTSTR ModelName, MODEL_TYPE NewModelType, LPCTSTR NewGroupName);//變更專案模組樣式
	//---------------------------------------------------------------------------------//
	size_t                     GetProjectFieldCount() const;//取得專案區域數量
	size_t                     GetProjectFieldCount(DISTRICT_ID DistrictID) const;//取得專案區域數量
	size_t                     CalcProjectFieldCountGrab() const;//計算專案區域數量要取像
	size_t                     CalcProjectFieldCountGrab(DISTRICT_ID DistrictID) const;//計算專案區域數量要取像
	CAOIField*                 GetProjectFieldPtr(size_t index, bool check) const;//取得專案區域指標
	bool                       ResetProjectObjectFieldPtr();//復歸專案內物件的區域指標
	bool                       ResetProjectObjectFieldPtr(DISTRICT_ID DistrictID);//復歸專案內物件的區域指標	
	bool                       BackupProjectObjectFieldIndex();//備份專案內物件的區域引數
	bool                       RestoreProjectObjectFieldIndex();//還原專案內物件的區域引數
	bool                       SetProjectObjectFieldPtrByIndex();//設定專案內物件的區域指標以引數為主
	//---------------------------------------------------------------------------------//
	size_t                     GetProjectProgramFieldCount() const;//取得專案編程區域數量
	CAOIField*                 GetProjectProgramFieldPtr(size_t index, bool check) const;//取得專案編程區域指標
	CAOIField*                 AddProjectProgramFieldPtr(CAOIField *FieldPtr, bool clone);//增加專案編程區域
	bool                       ClearProjectAllProgramField();//刪除專案編程區域
	bool                       LayoutProjectProgramFieldList();//重整專案編程區域列表
	bool                       CheckProjectProgramFieldValid(const CAOIField *RefFieldPtr);//確認專案編程區域指標有效
	bool                       ReleaseProjectProgramFieldFrameImageBuffer();//釋放專案編程區域影像記憶體	
	bool                       GetProjectProgramFieldDoNotSave() const;//取得專案編程區域是否存檔
	void                       SetProjectProgramFieldDoNotSave(bool NoSave);//設定專案編程區域是否存檔
	bool                       SetProjectProgramFieldMustToLoad(bool ToLoad);//設定專案編程區域是否強迫載入圖檔
	void                       SetProjectProgramFieldBufferReleased(bool val) { m_ProjectProgramFieldBufferReleased=val; }
	bool                       GetProjectProgramFieldBufferReleased() const { return m_ProjectProgramFieldBufferReleased; }//取得是否釋放專案編程區域記憶體
	//---------------------------------------------------------------------------------//
	size_t                     GetProjectInspectionFieldCount() const;//取得專案檢測區域數量
	size_t                     GetProjectInspectionFieldCount(DISTRICT_ID DistrictID) const;//取得專案檢測區域數量
	CAOIField*                 GetProjectInspectionFieldPtr(size_t index, bool check) const;//取得專案檢測區域指標
	CAOIField*                 AddProjectInspectionFieldPtr(CAOIField *FieldPtr, bool clone);//增加專案檢測區域
	void                       GetProjectInspectionFileList(std::vector<CAOIField*> &FieldList);//取得專案檢測區域列表
	bool                       SetProjectInspectionFileList(std::vector<CAOIField*> &FieldList);//設定專案檢測區域列表
	bool                       ModifyProjectInspectionFileList(const std::vector<CAOIField*> &FieldList);//修改專案檢測區域列表
	bool                       ClearProjectAllInspectionField();//刪除專案檢測區域
	bool                       ClearProjectAllInspectionField(DISTRICT_ID DistrictID);//刪除專案檢測區域	
	bool                       ResetProjectAllInspectionField();//復歸專案檢測區域
	bool                       ResetProjectAllInspectionField(DISTRICT_ID DistrictID);//復歸專案檢測區域
	bool                       LayoutProjectInspectionFieldList();//重整專案檢測區域列表	
	bool                       DestroyProjectInspectionFieldSelected();//刪除專案選取的檢測區域
	bool                       SelectProjectAllInspectionField(bool bSel);//選取專案整個檢測區域
	bool                       CheckProjectInspectionFieldValid(const CAOIField *RefFieldPtr);//確認專案檢測區域指標有效
	bool                       ReleaseProjectInspectionFieldFrameImageBuffer();//釋放專案檢測區域影像記憶體
	bool                       SetProjectInspectionFieldMustToLoad(bool ToLoad);//設定專案檢測區域是否強迫載入圖檔
	void                       SetProjectInspectionFieldBufferReleased(bool val) { m_ProjectInspectionFieldBufferReleased=val; }
	bool                       GetProjectInspectionFieldBufferReleased() const { return m_ProjectInspectionFieldBufferReleased; }
	//---------------------------------------------------------------------------------//	
	void                       ClearProjectAllInspectionPartField();//清除專案檢測局部區域列表		
	size_t                     GetProjectInspectionPartFieldCount() const;//取得專案檢測局部區域數量	
	void                       AddProjectInspectionPartFieldPtr(CAOIField *FieldPtr, bool SetIdx);//增加專案檢測局部區域
	CAOIField*                 GetProjectInspectionPartFieldPtr(size_t index, bool check) const;//取得專案檢測局部區域指標	
	bool                       SaveProjectInspectionPartOfflineFile(LPCTSTR pfilename);//儲存檢測局部離線檔案				
	size_t                     GetProjectInspectionFieldUsedCount(OFFLINE_IMAGE_SCOPE Scope) const;//取得專案檢測區域使用數量	
	CAOIField*                 GetProjectInspectionFieldUsedPtr(size_t index, bool check, OFFLINE_IMAGE_SCOPE Scope) const;//取得專案檢測區域使用指標
	//---------------------------------------------------------------------------------//
	bool                       CreateProjectMatrixField(bool ByCadRegion, std::vector<CAOIField*> &FieldPtrList);//建立等間距區域位置
	bool                       CreateProjectMatrixField_v2(bool ByCadRegion, std::vector<CAOIField*> &FieldPtrList);//建立等間距區域位置
	bool                       CreateProjectRandomField_Panel(FIELD_BUILD_MODE BuildMode, DISTRICT_ID DistrictID, std::vector<CAOIField*> &FieldPtrList, bool ByCadRegion, FIELD_BUILD_AREA_MODE AreaMode);//建立任意位置區域位置-整板
	bool                       CreateProjectRandomField_Board(FIELD_BUILD_MODE BuildMode, DISTRICT_ID DistrictID, std::vector<CAOIField*> &FieldPtrList, bool ByCadRegion);//建立任意位置區域位置-單板
	bool                       CreateProjectRandomField_Project(FIELD_BUILD_MODE BuildMode, DISTRICT_ID DistrictID, std::vector<CAOIField*> &FieldPtrList, bool ByCadRegion);//建立任意位置區域位置-專案
	bool                       CreateProjectRandomField_Component(std::vector<CAOIField*> &FieldPtrList);//建立任意位置區域位置-零件
	bool                       AdjustProjectField(OFFLINE_FILE_MODE OfflineFileMode, FIELD_BUILD_MODE BuildMode, FIELD_BUILD_AREA_MODE AreaMode);//調整區域位置
	bool                       AdjustProjectFieldFn(OFFLINE_FILE_MODE OfflineFileMode, FIELD_BUILD_MODE BuildMode, FIELD_BUILD_AREA_MODE AreaMode);//調整區域位置	

	bool                       CreateProjectProgramObject();//建立編程區域物件
	bool                       CreateProjectProgramField();//建立編程區域位置

	bool                       CreateProjectInspectionObject(FIELD_BUILD_MODE BuildMode, FIELD_BUILD_AREA_MODE AreaMode);//建立檢測區域物件	
	bool                       CreateProjectInspectionField_Matrix();//建立檢測區域位置
	bool                       CreateProjectInspectionField_RandomPanel(FIELD_BUILD_MODE BuildMode, bool ByCadRegion, FIELD_BUILD_AREA_MODE AreaMode);//建立檢測區域位置-任意位置-整板
	bool                       CreateProjectInspectionField_RandomBoard(FIELD_BUILD_MODE BuildMode, bool ByCadRegion);//建立檢測區域位置-任意位置-單板
	bool                       CreateProjectInspectionField_RandomProject(FIELD_BUILD_MODE BuildMode, bool ByCadRegion);//建立檢測區域位置-任意位置-專案
	bool                       CreateProjectInspectionField_RandomComponent();//建立檢測區域位置-任意位置-零件
	
	bool                       AdjustProjectProgramField();//調整編程區域位置
	bool                       CheckProjectProgramField(bool CheckObjRgn);//確認編程區域位置	

	bool                       GetProjectInspectionFieldCadMode_Matrix();//取得檢測區域是否使用Cad座標-等間距
	bool                       AdjustProjectInspectionField(FIELD_BUILD_MODE BuildMode, FIELD_BUILD_AREA_MODE AreaMode);//調整檢測區域位置
	bool                       CheckProjectInspectionField_Matrix(bool CheckObjRgn);//確認檢測區域位置	

	bool                       AssignProjectField(OFFLINE_FILE_MODE OfflineFileMode);//配置區域位置
	bool                       AssignProjectProgramField();//配置編程區域位置
	bool                       AssignProjectInspectionField();//配置檢測區域位置	
	bool                       AssignProjectInspectionPartField();//配置檢測區域位置-部分視野
	bool                       AssignProjectInspectionField_Matrix(FIELD_BUILD_MODE BuildMode);//配置檢測區域位置-等間距
	bool                       AssignProjectInspectionField_RandomPanel(FIELD_BUILD_MODE BuildMode, FIELD_BUILD_AREA_MODE AreaMode);//配置檢測區域位置-任意位置-整板
	bool                       AssignProjectInspectionField_RandomBoard(FIELD_BUILD_MODE BuildMode);//配置檢測區域位置-任意位置-單板	
	bool                       AssignProjectInspectionField_RandomProject(FIELD_BUILD_MODE BuildMode);//配置檢測區域位置-任意位置-專案
	//---------------------------------------------------------------------------------//	
	bool                       CreateProjectSubRgn(FIELD_BUILD_MODE BuildMode, bool ByCadRegion);//建立專案的子檢測框-走停
	//---------------------------------------------------------------------------------//	
	CAOIField*                 MatchProjectProgramFieldPtr(const TREGION4D &Region, bool CadMode, bool Inner, DISTRICT_ID DistrictID);
	CAOIField*                 MatchProjectInspectionPartFieldPtr(const TREGION4D &Region, bool CadMode, bool Inner, DISTRICT_ID DistrictID);	
	CAOIField*                 MatchProjectInspectionFieldPtr_Matrix(const TREGION4D &Region, bool CadMode, bool Inner, DISTRICT_ID DistrictID);	
	//---------------------------------------------------------------------------------//	
	bool                       AddFieldFrameToCurrentFrame(LPCTSTR OfflineFolder, CAOIField *FieldPtr, const TPOINT3D &Pos, const TSIZE2D &res, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, TUNI_FRAME UniFrameList[], size_t Count, bool bUseInner);		
	bool                       AddFieldFrameToCurrentFrameKernel(int FrameIndex, LPCTSTR OfflineFolder, CAOIField *FieldPtr, const TPOINT3D &Pos, const TSIZE2D &res, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, TUNI_FRAME &UniFrame, size_t Count, bool bUseInner, CString &strError);	
	bool                       FillCurrentFrame(OFFLINE_FILE_MODE OfflineFileMode, const TPOINT3D &Pos, const TSIZE2D &res, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, TUNI_FRAME UniFrameList[], size_t Count);
	bool                       FillCurrentFrameFn(OFFLINE_FILE_MODE OfflineFileMode, const TPOINT3D &Pos, const TSIZE2D &res, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, TUNI_FRAME UniFrameList[], size_t Count);
	bool                       FillCurrentProgramFrame(const TPOINT3D &Pos, const TSIZE2D &res, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, TUNI_FRAME UniFrameList[], size_t Count);	
	bool                       FillCurrentInspectionFrame(const TPOINT3D &Pos, const TSIZE2D &res, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, TUNI_FRAME UniFrameList[], size_t Count);			
	//---------------------------------------------------------------------------------//
	bool                       ClearProjectAllTempRgns();//刪除整個檢測區域指標
	bool                       AddProjectTempRgnPtr(CAOIRgn *Ptr, bool clone);//新增加檢測區域指標
	size_t                     GetProjectTempRgnCount() const;//取得檢測區域指標張數
	CAOIRgn*                   GetProjectTempRgnPtr(size_t idx, bool check);//取得檢測區域指標	
	void                       CloneProjectTempRgnPtrList(std::vector<CAOIRgn*> &RgnPtrList);//複製專案檢測區域列表
	bool                       CheckProjectTempRgnValid(const CAOIRgn *RefRgnPtr);//確認專案的檢測區域指標有效
	//---------------------------------------------------------------------------------//
	bool                       ClearProjectAllTempFovs();//刪除整個視野指標
	bool                       AddProjectTempFovPtr(CAOIFov *Ptr, bool clone);//新增加視野指標
	size_t                     GetProjectTempFovCount() const;//取得視野指標張數
	CAOIFov*                   GetProjectTempFovPtr(size_t idx, bool check);//取得視野指標	
	void                       CloneProjectTempFovPtrList(std::vector<CAOIFov*> &FovPtrList);//複製專案視野列表
	bool                       CheckProjectTempFovValid(const CAOIFov *RefFovPtr);//確認專案的視野指標有效
	//---------------------------------------------------------------------------------//
	bool                       ClearProjectAllTempSlices();//刪除整個相機圖指標
	bool                       AddProjectTempSlicePtr(CAOISlice *Ptr, bool clone);//新增加相機圖指標
	size_t                     GetProjectTempSliceCount() const;//取得相機圖指標張數
	CAOISlice*                 GetProjectTempSlicePtr(size_t idx, bool check);//取得相機圖指標	
	void                       CloneProjectTempSlicePtrList(std::vector<CAOISlice*> &SlicePtrList);//複製專案圖區域列表
	bool                       CheckProjectTempSliceValid(const CAOISlice *RefSlicePtr);//確認專案的圖區域指標有效
	//---------------------------------------------------------------------------------//	
	bool                       ClearProjectAllTempFields();//刪除整個相機區域指標
	bool                       AddProjectTempFieldPtr(CAOIField *Ptr, bool clone);//新增加相機區域指標
	size_t                     GetProjectTempFieldCount() const;//取得相機區域指標張數
	CAOIField*                 GetProjectTempFieldPtr(size_t idx, bool check);//取得相機區域指標	
	void                       CloneProjectTempFieldPtrList(std::vector<CAOIField*> &FieldPtrList);//複製專案相機區域列表
	bool                       CheckProjectTempFieldValid(const CAOIField *RefFieldPtr);//確認專案的相機區域指標有效
	//---------------------------------------------------------------------------------//
	bool                       ClearProjectAllTempFrames();//刪除整個畫面指標
	bool                       AddProjectTempFramePtr(CAOIFrame *Ptr, bool clone);//新增加畫面指標
	size_t                     GetProjectTempFrameCount() const;//取得畫面指標張數
	CAOIFrame*                 GetProjectTempFramePtr(size_t idx, bool check);//取得畫面指標
	void                       CloneProjectTempFramePtrList(std::vector<CAOIFrame*> &FramePtrList);//複製專案畫面列表
	bool                       CheckProjectTempFrameValid(const CAOIFrame *RefFramePtr);//確認專案的畫面指標有效
	//---------------------------------------------------------------------------------//
	bool                       ResetProjectColorGroupList();//復歸專案色彩群組列表	
	size_t                     GetProjectColorGroupCount() const;//專案色彩數量	
	bool                       UpdateProjectColorGroupListInfo();//更新專案色彩資料
	bool                       UpdateProjectColorGroupListInfo(std::vector<CColorGroup>  &ColorGroupList);//更新專案色彩資料
	CColorGroup*               GetProjectColorGroupPtr(size_t idx, bool bCheck);//取得專案色彩指標		
	bool                       UpdateProjectColorGroup(CAlgBinaryParam *BinaryParamPtr);//更新專案色彩
	bool                       UpdateProjectColorGroup(const CAlgBinaryParam &BinaryParam);//更新專案色彩
	bool                       SetProjectColorGroup(size_t idx, const CColorGroup &ColorGroup);//設定專案色彩	
	bool                       CloneProjectColorGroupList(std::vector<CColorGroup>  &ColorGroupList);//複製專案顏色列表
	bool                       SetProjectColorGroupList(const std::vector<CColorGroup>  &ColorGroupList);//設定專案色彩
	bool                       UpdateProjectColorGroupListToModel();//將專案色彩更新至模組
	bool                       UpdateProjectColorGroupToModel(int idx, const CColorGroup &ColorGroup);//更新專案色彩更新
	bool                       ReadProjectColorGroup(CAOIFileIO &FileIO);//載入專案色彩
	bool                       LoadProjectColorGroup(LPCTSTR filename, std::vector<CColorGroup>  &ColorGroupList);//載入專案色彩	
	bool                       LoadProjectColorGroupFn(LPCTSTR filename, std::vector<CColorGroup>  &ColorGroupList);//載入專案色彩	
	//---------------------------------------------------------------------------------//
	void                       MapProjectMapCadToStagePos(const CMapCoordinate &Map);//將專案影像CAD轉成機台座標
	void                       MapProjectMapStageToCadPos(const CMapCoordinate &Map);//將專案影像機台轉成CAD座標	
	void                       MapProjectMapLocCadToStagePos(const CMapCoordinate &Map);//將專案影像CAD轉成機台座標
	void                       MapProjectMapLocStageToCadPos(const CMapCoordinate &Map);//將專案影像機台轉成CAD座標
	void                       MapProjectFieldMatrixStageToCadPos(const CMapCoordinate &Map);//將專案區域機台轉成CAD座標

	void                       MapProjectMapCadToStagePos(DISTRICT_ID DistrictID, const CMapCoordinate &Map);//將專案影像CAD轉成機台座標
	void                       MapProjectMapStageToCadPos(DISTRICT_ID DistrictID, const CMapCoordinate &Map);//將專案影像機台轉成CAD座標
	void                       MapProjectMapLocCadToStagePos(DISTRICT_ID DistrictID, const CMapCoordinate &Map);//將專案影像CAD轉成機台座標
	void                       MapProjectMapLocStageToCadPos(DISTRICT_ID DistrictID, const CMapCoordinate &Map);//將專案影像機台轉成CAD座標
	//---------------------------------------------------------------------------------//	
	//專案底圖
	int                        GetProjectMapIndex3D();//取得3D的影像編號
	unsigned int               GetProjectMapUniqueID();
	//---------------------------------------------------------------------------------//		
	unsigned int               GetProjectMapIndex() const;
	bool                       SetProjectMapIndex(unsigned int value);	
	CString                    GetProjectMapIndexName();		
	unsigned int               GetProjectMapIndexNext(unsigned int value);	
	//---------------------------------------------------------------------------------//	
	bool                       CreateProjectMapShowPtr(unsigned int MapIndex, IMAGE_PTR &ShowPtr, bool bForce, bool bTestMap=false);
	bool                       UpdateProjectMapShowPtr();
	//---------------------------------------------------------------------------------//	
	int                        GetProjectMapScaleMode() const;
	void                       SetProjectMapScaleMode(int value);	
	//---------------------------------------------------------------------------------//	
	int                        GetProjectMapDividePos() const;//專案底圖分割位置
	void                       SetProjectMapDividePos(int val);//專案底圖分割位置	
	//---------------------------------------------------------------------------------//
	void                       GetProjectMapCalcRgn(TREGION4D &StageRgn) const;//取得專案計算位置	
	void                       GetProjectMapLocRect(RECT &Rect) const;//專案底圖局部區域
	void                       SetProjectMapLocRect(const RECT &Rect);//專案底圖局部區域	
	void                       GetProjectMapLocRectOff(RECT &Rect, RECT &Rect2) const;//專案底圖局部不檢測區域	
	void                       GetProjectMapLocSize(TSIZE2D &Size) const;//專案底圖局部尺寸
	void                       SetProjectMapLocSize(const TSIZE2D &Size);//專案底圖局部尺寸	
	void                       GetProjectMapLocCadRgn(TREGION4D &CadRgn) const;//專案底圖局部Cad範圍
	void                       SetProjectMapLocCadRgn(const TREGION4D &CadRgn);//專案底圖局部Cad範圍	
	void                       GetProjectMapLocStageRgn(TREGION4D &StageRgn) const;//專案底圖局部機台範圍
	void                       SetProjectMapLocStageRgn(const TREGION4D &StageRgn);//專案底圖局部機台範圍
	void                       GetProjectMapCadRgn(TREGION4D &CadRgn) const;//取得專案Cad區域
	void                       SetProjectMapCadRgn(const TREGION4D &CadRgn);//設定專案Cad區域	
	void                       GetProjectMapStageRgn(TREGION4D &StageRgn) const;//取得專案機台區域
	void                       SetProjectMapStageRgn(const TREGION4D &StageRgn);//設定專案機台區域	
	void                       GetProjectMapTeachRgn(TREGION4D &StageRgn) const;//取得專案教導位置	
	void                       SetProjectMapTeachRgn(const TREGION4D &StageRgn);//設定專案教導位置		
	void                       GetProjectMapTeachRgn(DISTRICT_ID DistrictID, TREGION4D &StageRgn) const;//取得專案教導位置	
	void                       ResetProjectMapTeachRgn(const TREGION4D &StageRgn);//重設專案教導位置
	//---------------------------------------------------------------------------------//	
	void                       GetProjectMapLocRect_DA(RECT &Rect) const;//專案底圖局部區域
	void                       SetProjectMapLocRect_DA(const RECT &Rect);//專案底圖局部區域	
	void                       GetProjectMapLocSize_DA(TSIZE2D &Size) const;//專案底圖局部尺寸
	void                       SetProjectMapLocSize_DA(const TSIZE2D &Size);//專案底圖局部尺寸
	void                       GetProjectMapLocCadRgn_DA(TREGION4D &CadRgn) const;//專案底圖局部Cad範圍
	void                       SetProjectMapLocCadRgn_DA(const TREGION4D &CadRgn);//專案底圖局部Cad範圍	
	void                       GetProjectMapLocStageRgn_DA(TREGION4D &StageRgn) const;//專案底圖局部機台範圍
	void                       SetProjectMapLocStageRgn_DA(const TREGION4D &StageRgn);//專案底圖局部機台範圍
	void                       GetProjectMapCadRgn_DA(TREGION4D &CadRgn) const;//取得專案Cad區域
	void                       SetProjectMapCadRgn_DA(const TREGION4D &CadRgn);//設定專案Cad區域	
	void                       GetProjectMapStageRgn_DA(TREGION4D &StageRgn) const;//取得專案機台區域
	void                       SetProjectMapStageRgn_DA(const TREGION4D &StageRgn);//設定專案機台區域	
	void                       GetProjectMapTeachRgn_DA(TREGION4D &StageRgn) const;//取得專案教導位置	
	void                       SetProjectMapTeachRgn_DA(const TREGION4D &StageRgn);//設定專案教導位置	
	void                       ResetProjectMapTeachRgn_DA(const TREGION4D &StageRgn);//重設專案教導位置
	//---------------------------------------------------------------------------------//	
	void                       GetProjectMapLocRect_DB(RECT &Rect) const;//專案底圖局部區域
	void                       SetProjectMapLocRect_DB(const RECT &Rect);//專案底圖局部區域
	void                       GetProjectMapLocSize_DB(TSIZE2D &Size) const;//專案底圖局部尺寸
	void                       SetProjectMapLocSize_DB(const TSIZE2D &Size);//專案底圖局部尺寸	
	void                       GetProjectMapLocCadRgn_DB(TREGION4D &CadRgn) const;//專案底圖局部Cad範圍
	void                       SetProjectMapLocCadRgn_DB(const TREGION4D &CadRgn);//專案底圖局部Cad範圍	
	void                       GetProjectMapLocStageRgn_DB(TREGION4D &StageRgn) const;//專案底圖局部機台範圍
	void                       SetProjectMapLocStageRgn_DB(const TREGION4D &StageRgn);//專案底圖局部機台範圍	
	void                       GetProjectMapCadRgn_DB(TREGION4D &CadRgn) const;//取得專案Cad區域
	void                       SetProjectMapCadRgn_DB(const TREGION4D &CadRgn);//設定專案Cad區域	
	void                       GetProjectMapStageRgn_DB(TREGION4D &StageRgn) const;//取得專案機台區域
	void                       SetProjectMapStageRgn_DB(const TREGION4D &StageRgn);//設定專案機台區域	
	void                       GetProjectMapTeachRgn_DB(TREGION4D &StageRgn) const;//取得專案教導位置	
	void                       SetProjectMapTeachRgn_DB(const TREGION4D &StageRgn);//設定專案教導位置	
	void                       ResetProjectMapTeachRgn_DB(const TREGION4D &StageRgn);//重設專案教導位置
	//---------------------------------------------------------------------------------//	
	void                       GetProjectMapResolution(double &ResX, double &ResY);//取得專案底圖解析度
	//---------------------------------------------------------------------------------//	
	bool                       SetProjectMapInfo(const TPOINT2D &Res, const TREGION4D &CadRgn, const TREGION4D &StageRgn);//取得專案底圖資訊	
	bool                       GetProjectMapInfo(TPOINT2D &Res, TREGION4D &CadRgn, TREGION4D &StageRgn);//取得專案底圖資訊	
	bool                       SetProjectMapInfo_DA(const TPOINT2D &Res, const TREGION4D &CadRgn, const TREGION4D &StageRgn);//取得專案底圖資訊	
	bool                       GetProjectMapInfo_DA(TPOINT2D &Res, TREGION4D &CadRgn, TREGION4D &StageRgn);//取得專案底圖資訊
	bool                       SetProjectMapInfo_DB(const TPOINT2D &Res, const TREGION4D &CadRgn, const TREGION4D &StageRgn);//取得專案底圖資訊	
	bool                       GetProjectMapInfo_DB(TPOINT2D &Res, TREGION4D &CadRgn, TREGION4D &StageRgn);//取得專案底圖資訊	
	//---------------------------------------------------------------------------------//	
	void                       ClearProjectMapBuffer();//清除專案底圖指標	
	bool                       CheckProjectMapPtr(unsigned int MapIndex);//確認專案影像		
	bool                       CalcProjectMapBuffer(const TPOINT3D &StagePos1, const TPOINT3D &StagePos2);//計算專案影像資料	
	bool                       CalcProjectMapBufferFn(const TPOINT3D &StagePos1, const TPOINT3D &StagePos2);//計算專案影像資料	
	bool                       SetProjectMapPtr(unsigned int MapIndex, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR Ptr);//設定專案影像	
	bool                       GetProjectMapPtr(unsigned int MapIndex, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, IMAGE_PTR &Ptr);//取得專案影像		
	//------------------------------------------------------------------------------
	bool                       MapProjectCadToMapPos(double PosX, double PosY, double &rPosX, double &rPosY, DISTRICT_ID DistrictID);//將專案Cad座標轉成底圖座標
	bool                       MapProjectStageToMapPos(double PosX, double PosY, double &rPosX, double &rPosY, LANE_ID LaneID, DISTRICT_ID DistrictID);//將專案機台座標轉成底圖座標
	bool                       MapProjectTeachToMapPos(double PosX, double PosY, double &rPosX, double &rPosY, LANE_ID LaneID, DISTRICT_ID DistrictID);//將專案機台座標轉成底圖座標
	//------------------------------------------------------------------------------
	bool                       AddProjectMapPtr(TPOINT2D StagePos, unsigned int MapIndex, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR Ptr);
	//------------------------------------------------------------------------------
	bool                       SaveProjectMapFile(LPCTSTR filename);//儲存專案底圖檔案	
	bool                       SaveProjectMapFileFn(LPCTSTR filename);//儲存專案底圖檔案	
	bool                       LoadProjectMapFile(LPCTSTR filename);//載入專案底圖檔案
	bool                       LoadProjectMapFileFn(LPCTSTR filename);//載入專案底圖檔案
	//------------------------------------------------------------------------------
	bool                       SaveProjectMapImage(LPCTSTR filename);//儲存專案底圖檔案
	bool                       SaveProjectMapImage_SP(LPCTSTR filename);//儲存專案底圖檔案
	bool                       SaveProjectMapImage_MP(LPCTSTR filename);//儲存專案底圖檔案
	//------------------------------------------------------------------------------
	bool                       LoadProjectMapImage(LPCTSTR filename);//載入專案底圖檔案		
	bool                       LoadProjectMapImage_SP(LPCTSTR filename);//載入專案底圖檔案		
	bool                       LoadProjectMapImage_MP(LPCTSTR filename);//載入專案底圖檔案			
	//------------------------------------------------------------------------------
	//專案底圖遮罩
	void                       ClearProjectMapMaskBuffer();//清除專案底圖遮罩指標	
	bool                       SaveProjectMapMaskFile(LPCTSTR filename);//儲存專案底圖遮罩檔案
	bool                       SaveProjectMapMaskFileFn(LPCTSTR filename);//儲存專案底圖遮罩檔案
	bool                       LoadProjectMapMaskFile(LPCTSTR filename);//載入專案底圖遮罩檔案	
	bool                       LoadProjectMapMaskFileFn(LPCTSTR filename);//載入專案底圖遮罩檔案			
	IMAGE_PTR                  GetProjectMapMaskImage(IMAGE_SIZE &MaskW, IMAGE_SIZE &MaskH, IMAGE_SIZE &MaskStep, IMAGE_SIZE &BitCount);//取得專案底圖遮罩影像
	bool                       SetProjectMapMaskImage(IMAGE_SIZE MaskW, IMAGE_SIZE MaskH, IMAGE_SIZE MaskStep, IMAGE_SIZE BitCount, IMAGE_PTR MaskPtr, bool bClone);//設定專案底圖遮罩影像
	//------------------------------------------------------------------------------
	//專案標記成績
	void                       SetProjectMarkScore(double val) { m_ProjectMarkScore=val;}
	double                     GetProjectMarkScore() const { return m_ProjectMarkScore; }
	//------------------------------------------------------------------------------
	bool                       SaveProjectMarkFile(LPCTSTR filename);//儲存專案標記圖檔案
	bool                       SaveProjectMarkFileFn(LPCTSTR filename);//儲存專案標記圖檔案
	bool                       LoadProjectMarkFile(LPCTSTR filename);//載入專案標記圖檔案
	bool                       LoadProjectMarkFileFn(LPCTSTR filename);//載入專案標記圖檔案
	void                       ClearProjectMarkBuffer();//清除專案標記圖指標	
	unsigned int               GetProjectMarkFrameIndex() const;//設定專案標記圖影像引數
	bool                       SetProjectMarkFrameIndex(unsigned int index);//設定專案標記圖影像引數
	unsigned int               GetProjectMarkFrameIndex_System() const;//取得專案標記圖影像引數-系統
	bool                       SetProjectMarkFrameIndex_System(unsigned int index);//設定專案標記圖影像引數-系統
	bool                       SetProjectMarkStage(double PosX, double PosY, double PosZ);//設定專案標記圖機台座標
	bool                       GetProjectMarkStage(double &PosX, double &PosY, double &PosZ) const;//設定專案標記圖機台座標	
	bool                       SetProjectMarkImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr);//取得專案標記圖資訊	
	bool                       GetProjectMarkImage(IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, IMAGE_PTR &ImagePtr);//取得專案標記圖資訊	
	bool                       CloneProjectMarkImage(IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, IMAGE_PTR &ImagePtr);//取得專案標記圖資訊	
	bool                       InspectProjectMark(const TPOINT2D &StagePos, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR Ptr);//檢測專案標記
	bool                       InspectProjectMarkFn(const TPOINT2D &StagePos, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR Ptr);//檢測專案標記
	//------------------------------------------------------------------------------	
	bool                       BackupProjectAllObjNeedToCalculate();//回復專案所有物件要去檢測狀態
	bool                       RestoreProjectAllObjNeedToCalculate();//備份專案所有物件要去檢測狀態
	//------------------------------------------------------------------------------	
	bool                       UpdateSelectObjToNeedToCalculateRgn();//將選到的物品設定成要去檢測
	bool                       SetProjectAllObjToNeedToCalculateRgn(bool value);//設定所有物品設定成要去檢測		
	//------------------------------------------------------------------------------
	bool                       GetProjectFirstStagePos(size_t ProjectCount, DISTRICT_ID DistrictID, TPOINT3D &StagePos);//取得專案第1個機台位置	
	//---------------------------------------------------------------------------------//
	bool                       CreateProjectCaptureMapObj();//建立專案底圖取像的物件
	bool                       CreateProjectCaptureMapObjFn();//建立專案底圖取像的物件
	bool                       CreateProjectPanelAlignObj(bool MultiFdLight);//建立專案整板對位的物件
	bool                       CreateProjectPanelAlignObjFn(bool MultiFdLight);//建立專案整板對位的物件
	bool                       CreateProjectBoardAlignObj(bool MultiFdLight);//建立專案單板對位的物件	
	bool                       CreateProjectBoardAlignObjFn(bool MultiFdLight);//建立專案單板對位的物件	
	bool                       CreateProjectInspectionObj(bool AutoReleaseFieldFrame);//建立專案檢測的物件	
	bool                       CreateProjectInspectionObjFn(bool AutoReleaseFieldFrame);//建立專案檢測的物件		
	//---------------------------------------------------------------------------------//	
	bool                       SetProjectFullMapComponentFinsih();//設定專案整板零件的檢測完畢
	CAOIFrame*                 CreateProjectFramePtrByFrameParam(const CAOIField *FieldPtr, TFrameParam *FrameParamPtr, unsigned int &CameraFrameIdx);//建立專案的Frame指標	
	//---------------------------------------------------------------------------------//	
	bool                       CheckProjectFdResult_Panel(bool OfflineMode);//確認專案的定位點結果-整板
	bool                       CheckProjectFdResult_Board(bool OfflineMode);//確認專案的定位點結果-單板	
	bool                       CalcProjectStagePosition_Panel();//計算專案座標
	bool                       CalcProjectStagePosition_Board(bool bCalcFov);//計算專案座標
	bool                       CalcProjectCadResultPosition_Panel_All();//計算專案Cad結果座標
	bool                       CalcProjectCadResultPosition_Board_All();//計算專案Cad結果座標
	bool                       CalcProjectStagePosition_Panel(DISTRICT_ID DistrictID);//計算專案座標
	bool                       CalcProjectStagePosition_PanelFn(DISTRICT_ID DistrictID);//計算專案機台座標
	bool                       CalcProjectStagePosition_Board(DISTRICT_ID DistrictID, bool bCalcFov);//計算專案座標
	bool                       CalcProjectStagePosition_BoardFn(DISTRICT_ID DistrictID, bool bCalcFov);//計算專案機台座標
	bool                       CalcProjectCadResultPosition_Panel(DISTRICT_ID DistrictID);//計算專案Cad結果座標	
	bool                       CalcProjectCadResultPosition_PanelFn(DISTRICT_ID DistrictID);//計算專案Cad結果座標	
	bool                       CalcProjectCadResultPosition_Board(DISTRICT_ID DistrictID);//計算專案Cad結果座標
	bool                       CalcProjectCadResultPosition_BoardFn(DISTRICT_ID DistrictID);//計算專案Cad結果座標
	bool                       CheckProjectPositionInsideStageLimit();//確認專案座標
	bool                       CheckProjectPositionInsideStageLimit(DISTRICT_ID DistrictID);//確認專案座標
	//---------------------------------------------------------------------------------//		
	bool                       CalcProjectObjMapRect();//計算專案物件在底圖的位置
	bool                       CheckProjectObjectRegion(bool ByCadRegion);//確認專案物件範圍
	//---------------------------------------------------------------------------------//				
	bool                       CheckProjectFrameUniqueID_3D();//確認專案影像唯一碼-3D燈源
	unsigned int               GetProjectFrameUniqueID_Fd() const;//取得專案定位點的Frame編號
	size_t                     GetProjectFrameUniqueIDCount() const;//取得專案影像參數唯一碼的數量	
	unsigned int               GetProjectFrameUniqueID(size_t idx, bool check);//取得專案的影像參數唯一碼	
	bool                       UpdateProjectFrameUniqueIDToFrameList(std::vector<TUNI_FRAME> &UniFrameList);//更新專案影像唯一碼至影像列表
	void                       SetProjectFrameUniqueIDList(const std::vector<unsigned int> &FrameUniqueIDList);//設定專案影像唯一碼列表
	void                       GetProjectFrameUniqueIDList(std::vector<unsigned int> &FrameUniqueIDList) const;//取得專案影像唯一碼列表
	unsigned int               GetProjectFrameIndexByUniqueID(unsigned int FrameUniqueID) const;//取得專案的影像引數, 依據影像參數唯一碼	
	bool                       BuildProjectFrameIndexMapList(std::vector<unsigned int> &FrameIndexMapList);//建立專案影像的對應列表	
	bool                       BuildProjectFrameIndexMapParam(std::vector<unsigned int> &FrameIndexMapList, unsigned int &DefaultIndex, unsigned int &DefaultUniqueID);//建立專案影像的對應參數
	//---------------------------------------------------------------------------------//
	unsigned int               CalcProjectFrameGrabTime() const;//計算專案影像取像時間
	bool                       CloneProjectSliceParamList(std::vector<TSliceParam> &ParamList);//複製專案單張影像參數
	bool                       CloneProjectSliceParamList_No3D(std::vector<TSliceParam> &ParamList) const;//複製專案單張影像參數-無3D		
	bool                       CloneProjectSliceParamList_PanelFd(std::vector<TSliceParam> &ParamList);//複製專案單張影像參數
	bool                       CloneProjectSliceParamList_BoardFd(std::vector<TSliceParam> &ParamList);//複製專案單張影像參數
	bool                       FilterProjectSliceParamList_No3D(const std::vector<TSliceParam> &SrcList, std::vector<TSliceParam> &DstList) const;//過濾專案單張影像參數-無3D	
	//---------------------------------------------------------------------------------//	
	bool                       CloneProjectFrameParamList(std::vector<TFrameParam> &ParamList);//複製專案影像參數	
	bool                       CloneProjectFrameParamList_No3D(std::vector<TFrameParam> &ParamList) const;//複製專案影像參數-無3D
	bool                       CloneProjectFrameParamList_PanelFd(std::vector<TFrameParam> &ParamList);//複製專案影像參數	
	bool                       CloneProjectFrameParamList_BoardFd(std::vector<TFrameParam> &ParamList);//複製專案影像參數	
	void                       CloneProjectFrameUniqueIDList(std::vector<unsigned int> &UniqueIDList);//複製專案影像表格編號列表
	bool                       FilterProjectFrameParamList_No3D(const std::vector<TFrameParam> &SrcList, std::vector<TFrameParam> &DstList) const;//過濾專案影像參數-無3D
	//---------------------------------------------------------------------------------//	
	bool                       BuildProjectImageConfig();//建立專案的影像組態	
	bool                       BuildProjectImageConfig(std::vector<unsigned int> &FrameUniqueIDList);//建立專案的影像組態	
	bool                       BuildProjectImageConfigFn(std::vector<unsigned int> &FrameUniqueIDList);//建立專案的影像組態	
	bool                       CheckProjectUniqueIDList(const std::vector<unsigned int> &FrameUniqueIDList);//確認專案影像唯一碼列表
	bool                       BuildProjectLightCtrlBoardTable(std::vector<TLCB_TRIG_TABLE> &TableList);//將Slice轉成Light Ctrl Board的表格
	//---------------------------------------------------------------------------------//
	bool                       SetProjectFrameFileName(OFFLINE_FILE_MODE OfflineFileMode, LPCTSTR Folder);		
	bool                       ResetProjectProgramFrameFileName();	
	bool                       SetProjectProgramFrameFileName(LPCTSTR Folder);	
	bool                       SetProjectInspectionFrameFileName(LPCTSTR Folder);
	//---------------------------------------------------------------------------------//	
	bool                       SaveProjectOfflineFdFile(LPCTSTR pfilename);//儲存專案定位點資料-整板
	bool                       SaveProjectOfflineFdFileFn(LPCTSTR pfilename);//儲存專案定位點資料-整板
	bool                       LoadProjectOfflineFdFile(LPCTSTR pfilename);//載入專案定位點資料-整板
	bool                       LoadProjectOfflineFdFileFn(LPCTSTR pfilename);//載入專案定位點資料-整板
	bool                       SaveProjectOfflineFdFileToInspection(LPCTSTR pfilename);//備份專案定位點資料-整板
	//---------------------------------------------------------------------------------//	
	void                       IncrementProjectPreLoadFrameImageCount();//累加專案載入畫面數量
	bool                       CloseProjectPreLoadFrameImageThread(size_t ThreadCount, DWORD dwDelay);//關閉專案預先載入執行緒	
	THREAD_COMMAND_MODE        GetProjectPreLoadFrameImageThreadCmd() { return m_ProjectPreLoadFrameImageThreadCmd; }//專案預先載入圖檔的執行緒狀態
	//---------------------------------------------------------------------------------//
	void                       SetProjectProgramOfflineStartPos(const TPOINT2D &Pos);//離線編程的檢測位置
	void                       GetProjectProgramOfflineStartPos(TPOINT2D &Pos) const;//離線編程的檢測位置
	double                     GetProjectProgramOfflineFocusOffsetZ() const;//離線編程的焦距位置Z
	void                       SetProjectProgramOfflineFocusOffsetZ(double val);//離線編程的焦距位置Z		
	LPCTSTR                    GetProjectProgramOfflineFolder() const;
	void                       SetProjectProgramOfflineFolder(LPCTSTR Folder);
	CString                    GetProjectProgramOfflinename() const;//編程離線檔案
	void                       SetProjectProgramOfflinename(LPCTSTR Filename);//編程離線檔案
	LPCTSTR                    GetProjectProgramOfflinename_DA() const;//編程離線檔案
	void                       SetProjectProgramOfflinename_DA(LPCTSTR Filename);//編程離線檔案
	LPCTSTR                    GetProjectProgramOfflinename_DB() const;//編程離線檔案
	void                       SetProjectProgramOfflinename_DB(LPCTSTR Filename);//編程離線檔案
	bool                       SaveProjectProgramOfflineFile(LPCTSTR pfilename);//儲存編程離線檔案		
	bool                       SaveProjectProgramOfflineFileFn(LPCTSTR pfilename);//儲存編程離線檔案		
	bool                       WriteProjectProgramOfflineFile(CAOIFileIO &FileIO);//寫入編程離線檔案
	bool                       LoadProjectProgramOfflineFile(LPCTSTR pfilename);//載入編程離線檔案
	bool                       LoadProjectProgramOfflineFile(LPCTSTR pfilename, std::vector<unsigned int> &FrameUniqueIDList);//載入編程離線檔案		
	bool                       LoadProjectProgramOfflineFileFn(LPCTSTR pfilename, std::vector<unsigned int> &FrameUniqueIDList);//載入編程離線檔案		
	bool                       ReadProjectProgramOfflineFile(CAOIFileIO &FileIO, std::vector<unsigned int> &FrameUniqueIDList);//讀取編程離線檔案
	bool                       AssignProjectProgramOfflineFolder(LPCTSTR folder);//重新指派編程離線檔案資料夾	
	bool                       ExecProjectPreLoadProgramImage();//執行專案預先載入編程圖檔		
	bool                       ExecProjectPreLoadProgramImageFn();//執行專案預先載入編程圖檔		
	bool                       ExecProjectPreLoadProgramImage_MT();//執行專案預先載入編程圖檔-多執行緒
	bool                       ExecProjectPreLoadProgramImage_MP();//執行專案預先載入編程圖檔-平行載入
	//---------------------------------------------------------------------------------//
	void                       SetProjectInspectionOfflineStartPos(const TPOINT2D &Pos);//離線檢測的檢測位置
	void                       GetProjectInspectionOfflineStartPos(TPOINT2D &Pos) const;//離線檢測的檢測位置
	double                     GetProjectInspectionOfflineFocusOffsetZ() const;//離線檢測的焦距位置Z
	void                       SetProjectInspectionOfflineFocusOffsetZ(double val);//離線檢測的焦距位置Z
	LPCTSTR                    GetProjectInspectionOfflineFolder() const { return m_ProjectInspectionOfflineFolder; }
	void                       SetProjectInspectionOfflineFolder(LPCTSTR Folder) { m_ProjectInspectionOfflineFolder= Folder; }
	LPCTSTR                    GetProjectInspectionOfflineFolderDefault() const { return m_ProjectInspectionOfflineFolderDefault; }
	void                       SetProjectInspectionOfflineFolderDefault(LPCTSTR Folder) { m_ProjectInspectionOfflineFolderDefault= Folder; }
	bool                       SaveProjectInspectionOfflineFile(LPCTSTR pfilename);//儲存檢測離線檔案		
	bool                       SaveProjectInspectionOfflineFileFn(LPCTSTR pfilename, OFFLINE_IMAGE_SCOPE Scope);//儲存檢測離線檔案		
	bool                       WriteProjectInspectionOfflineFile(CAOIFileIO &FileIO, OFFLINE_IMAGE_SCOPE Scope);//寫入檢測離線檔案
	bool                       LoadProjectInspectionOfflineFile(LPCTSTR pfilename);//載入檢測離線檔案		
	bool                       LoadProjectInspectionOfflineFileFn(LPCTSTR pfilename);//載入檢測離線檔案		
	bool                       ReadProjectInspectionOfflineFile(CAOIFileIO &FileIO);//讀取檢測離線檔案
	bool                       AssignProjectInspectionOfflineFolder(LPCTSTR folder);//重新指派檢測離線檔案資料夾	
	bool                       ExecProjectPreLoadInspectionImage();//執行專案預先載入檢測圖檔
	bool                       ExecProjectPreLoadInspectionImageFn();//執行專案預先載入檢測圖檔
	bool                       ExecProjectPreLoadInspectionImage_MT();//執行專案預先載入檢測圖檔-多執行緒
	bool                       ExecProjectPreLoadInspectionImage_MP();//執行專案預先載入檢測圖檔-平行載入		
	//---------------------------------------------------------------------------------//			
	void                       SetProjectSpaceMaxHeight(double dHeight);//設定專案空間最高高度
	double                     GetProjectSpaceMaxHeight() const;//取得專案空間最高高度
	//---------------------------------------------------------------------------------//	
	void                       SetProjectSpaceBaseHeight(double dLevel);//設定專案空間平面高度
	double                     GetProjectSpaceBaseHeight() const;//取得專案空間平面高度
	//---------------------------------------------------------------------------------//	
	void                       SetProjectSpaceToGrayRatioMode(int nRatio);//設定專案高度轉灰階比例
	int                        GetProjectSpaceToGrayRatioMode() const;//取得專案高度轉灰階比例
	double                     GetProjectSpaceToGrayRatio() const { return m_ProjectSpaceToGrayRatio; } //取得專案高度轉灰階比例
	//---------------------------------------------------------------------------------//	
	void                       SetProjectDlpLedColorMode(int val);//設定專案DLP-LED顏色
	int                        GetProjectDlpLedColorMode() const;//取得專案DLP-LED顏色
	//---------------------------------------------------------------------------------//	
	bool                       WaitProjectLastInspectionEnd();//等待專案上一次檢測結束
	//---------------------------------------------------------------------------------//	
	bool                       InitProjectInspection(LANE_ID LaneID);//初始化專案檢測
	bool                       InitProjectInspectionData(LANE_ID LaneID);//初始化專案檢測資料
	bool                       InitProjectInspectionData_Fd(LANE_ID LaneID);//初始化專案檢測資料-定位點
	bool                       InitProjectInspectionData_Mark(LANE_ID LaneID);//初始化專案檢測資料-特徵點
	bool                       InitProjectInspectionData_Barcode(LANE_ID LaneID);//初始化專案檢測資料-條碼
	bool                       InitProjectInspectionData_Component(LANE_ID LaneID);//初始化專案檢測資料-零件
	bool                       InitProjectInspectionData_Board(LANE_ID LaneID);//初始化專案檢測資料-單板
	bool                       InitProjectInspectionData_Panel(LANE_ID LaneID);//初始化專案檢測資料-整板
	bool                       InitProjectInspectionData_Field(LANE_ID LaneID);//初始化專案檢測資料-視野
	bool                       InitProjectInspectionData_PartGroup();//初始化專案檢測資料-群組資料
	bool                       InitProjectInspectionData_LocalBasePlane(LANE_ID LaneID);//初始化專案檢測資料-局部基準面
	//---------------------------------------------------------------------------------//
	const wchar_t*             GetProjectModuleName() const;//專案機種名稱
	bool                       SetProjectModuleName(LPCTSTR Name);//設定專案機種名稱
	//---------------------------------------------------------------------------------//
	const wchar_t*             GetProjectVersion() const;//專案版本
	bool                       SetProjectVersion(LPCTSTR Version);//設定專案版本
	//---------------------------------------------------------------------------------//	
	PANEL_SIDE_MODE            GetProjectPanelSideMode() const;//專案產品正背面
	bool                       SetProjectPanelSideMode(PANEL_SIDE_MODE Mode);//設定專案整板正背面	
	//---------------------------------------------------------------------------------//		
	const wchar_t*             GetProjectWorkNumber() const;//專案工單號碼
	bool                       SetProjectWorkNumber(LPCTSTR WorkNumber);//設定專案工單編號
	//---------------------------------------------------------------------------------//
	double                     GetProjectLaneWidth() const;//專案軌道寬度
	void                       SetProjectLaneWidth(double val);	//專案軌道寬度
	//---------------------------------------------------------------------------------//
	SAVE_TEST_IMAGE_MODE       GetProjectOnlineTuningMode() const;//在線調機樣式
	SAVE_TEST_IMAGE_MODE       GetProjectSaveModelImageMode() const;//儲存模組圖片模式
	SAVE_TEST_IMAGE_MODE       GetProjectSaveFieldImageMode() const;//儲存區域圖片模式
	SAVE_TEST_IMAGE_MODE       GetProjectSaveModelImageMode_AI() const;//儲存模組圖片模式-AI	
	int                        GetProjectSaveModelImageOnOff_AI() const;//儲存模組圖片開關-AI
	bool                       GetProjectSaveModelImage3DFile_AI() const;//儲存模組圖片3D檔案-AI
	OFFLINE_IMAGE_SCOPE        GetProjectSaveOfflineImageScope() const;//儲存離線影像範疇
	SAVE_TEST_DATA_MODE        GetProjectSaveComponentWndListMode() const;//儲存零件檢測框列表模式
	void                       BuildProjectSaveModelImageOnOff_AI_Text(int Val, CString &Text) const;//儲存模組圖片開關-AI	
	bool                       DecodeProjectSaveModelImageOnOff_AI(int OnOff, size_t Count, std::vector<bool> &OnOffList) const;//解碼儲存模組圖片開關-AI	
	//---------------------------------------------------------------------------------//
	bool                       AnalyzeProjectXBoard();//分析專案報廢板
	bool                       AnalyzeProjectComponentDefectCount();//分析專案零件瑕疵數量
	bool                       AnalyzeProjectComponentMasterResult();//分析專案零件本尊結果
	//---------------------------------------------------------------------------------//
	bool                       BuildProjectDefectComponentList();//建立專案瑕疵零件列表	
	bool                       BuildProjectDefectComponentListKernel();//建立專案瑕疵零件列表	
	bool                       SortProjectDefectComponentListByName();//排序專案瑕疵零件列表-根據名稱
	size_t                     GetProjectDefectComponentCount() const;//取得專案瑕疵零件數量	
	size_t                     GetProjectDefectComponentCount(DISTRICT_ID DistrictID) const;//取得專案瑕疵零件數量	
	CAOIComponent*             GetProjectDefectComponentPtr(size_t index, bool bCheck) const;//取得專案瑕疵零件指標	
	bool                       RemoveProjectDefectComponentSelected();//移除專案零件選取到的指標
	void                       ClearProjectAllDefectComponents();//移除專案瑕疵零件列表
	//---------------------------------------------------------------------------------//	
	bool                       RemoveProjectComponentSelected(std::vector<CAOIComponent*> &ComponentList);//移除專案零件選取到的指標
	bool                       SelectProjectComponentList(std::vector<CAOIComponent*> &ComponentList, bool val);//選取全部零件選取到的指標
	//---------------------------------------------------------------------------------//	
	size_t                     GetProjectDefectComponentCount_Lane(LANE_ID LaneID) const;//取得專案瑕疵零件數量	
	CAOIComponent*             GetProjectDefectComponentPtr_Lane(LANE_ID LaneID, size_t index, bool bCheck) const;//取得專案瑕疵零件指標	
	//---------------------------------------------------------------------------------//	
	size_t                     GetProjectDefectComponentCount_LA() const;//取得專案瑕疵零件數量	
	CAOIComponent*             GetProjectDefectComponentPtr_LA(size_t index, bool bCheck) const;//取得專案瑕疵零件指標	
	bool                       RemoveProjectDefectComponentSelected_LA();//移除專案零件選取到的指標
	void                       SetProjectDefectComponentList_LA(const std::vector<CAOIComponent*> &ComponentList);//設定專案瑕疵零件列表
	//---------------------------------------------------------------------------------//		
	size_t                     GetProjectDefectComponentCount_LB() const;//取得專案瑕疵零件數量	
	CAOIComponent*             GetProjectDefectComponentPtr_LB(size_t index, bool bCheck) const;//取得專案瑕疵零件指標	
	bool                       RemoveProjectDefectComponentSelected_LB();//移除專案零件選取到的指標
	void                       SetProjectDefectComponentList_LB(const std::vector<CAOIComponent*> &ComponentList);//設定專案瑕疵零件列表
	//---------------------------------------------------------------------------------//		
	bool                       RemoveProjectDefectComponentSelected_All();//移除專案零件選取到的指標
	void                       SelectProjectDefectComponentList_All(bool val);//選取專案瑕疵零件列表-全部
	//---------------------------------------------------------------------------------//	
	bool                       CheckProjectGrrSigmaItemEnabled() const;	
	bool                       SaveProjectGrrSigmaItemFile();	
	bool                       SaveProjectGrrSigmaItemFile(LPCTSTR pfilename);
	bool                       WriteProjectGrrSigmaItemFile(CAOIFileIO &FileIO);
	bool                       LoadProjectGrrSigmaItemFile();
	bool                       LoadProjectGrrSigmaItemFile(LPCTSTR pfilename);
	bool                       ReadProjectGrrSigmaItemFile(CAOIFileIO &FileIO);
	//---------------------------------------------------------------------------------//	
	bool                       ResetProjectGrrSigmaItem_Barcode();
	bool                       ResetProjectGrrSigmaItem_Match();
	//---------------------------------------------------------------------------------//	
	bool                       AddProjectStatisticRecords();//加入專案統計紀錄	
	bool                       AddProjectStatisticRecords_ARS(LANE_ID LaneID);//加入專案統計紀錄_ARS
	bool                       ResetProjectStatisticRecords();//復歸專案統計紀錄
	bool                       AnalyzeProjectStatisticAlarm();//分析專案統計警報	
	bool                       AnalyzeProjectStatisticRecords();//分析專案統計紀錄-前十大不良
	bool                       AnalyzeProjectStatisticRecords_ARS(LANE_ID LaneID);//分析專案統計紀錄-前十大不良_ARS
	bool                       CalcProjectTestAverageTime(const TTestResult &Now, TTestResult &Ave);//計算專案統計時間
	bool                       AddProjectStatisticResult(const TTestResult &TestResult, TTestResult &StatisResult);//加入專案統計
	//---------------------------------------------------------------------------------//
	bool                       CalcProjectTop10ListModel(DEFECT_FROM_MODE DefectFrom, std::vector<TTop10Node> &Top10List);//取出前十大模組不良
	bool                       CalcProjectTop10ListModel_Lane(LANE_ID LaneID, DEFECT_FROM_MODE DefectFrom, std::vector<TTop10Node> &Top10List);//取出前十大模組不良
	bool                       CalcProjectTop10ListComponent(DEFECT_FROM_MODE DefectFrom, std::vector<TTop10Node> &Top10List);//取出前十大零件不良
	bool                       CalcProjectTop10ListComponent_Lane(LANE_ID LaneID, DEFECT_FROM_MODE DefectFrom, std::vector<TTop10Node> &Top10List);//取出前十大零件不良
	bool                       CalcProjectTop10ListPartNumber(DEFECT_FROM_MODE DefectFrom, std::vector<TTop10Node> &Top10List);//取出前十大料號不良
	bool                       CalcProjectTop10ListPartNumber_Lane(LANE_ID LaneID, DEFECT_FROM_MODE DefectFrom, std::vector<TTop10Node> &Top10List);//取出前十大料號不良
	//---------------------------------------------------------------------------------//		
	//專案瑕疵檢測項目	
	CWndDefectItem&            GetProjectDefectItemTest() { return m_ProjectParameter.m_DefectTestItem; }	
	//---------------------------------------------------------------------------------//
	//專案瑕疵警報項目
	CWndDefectItem&            GetProjectDefectItemAlarm() { return m_ProjectParameter.m_DefectAlarmItem; }		
	void                       ModifyProjectWndDefectItemAlarm(CWndDefectItem &DefectItem);
	//---------------------------------------------------------------------------------//	
	//專案瑕疵警報啟用
	CWndDefectItem&            GetProjectDefectItemEnable() { return m_ProjectParameter.m_DefectEnableItem; }		
	//---------------------------------------------------------------------------------//
	bool                       WriteProjectOfflineSaveFinish();//寫入專案離線存檔完成
	bool                       ReadProjectOnlineTuningEnable();//讀取專案線上調機啟用
	bool                       WriteProjectOnlineTuningEnable(bool Enable);//寫入專案線上調機啟用
	bool                       CheckProjectOfflineSaveFinish();//確認專案離線存檔完成	
	bool                       CheckProjectOnlineTuningEnable();//確認專案線上調機啟用
	bool                       WriteProjectShareFile(LPCTSTR KeyName, LPCTSTR Value);//寫入專案共享資料		
	bool                       ReadProjectShareFile(LPCTSTR KeyName, LPCTSTR Default, CString &Value);//讀取專案共享資料
	bool                       CheckProjectOnlineTuningRunning() const;//確定專案線上調機運作中
	void                       SetProjectOnlineTuningEnable(bool val) { m_ProjectOnlineTuningEnable=val; }
	bool                       GetProjectOnlineTuningEnable() const{ return m_ProjectOnlineTuningEnable; }
	void                       SetProjectOnlineTuningEnableUI(bool val) { m_ProjectOnlineTuningEnableUI=val; }
	bool                       GetProjectOnlineTuningEnableUI() const{ return m_ProjectOnlineTuningEnableUI; }		
	void                       AddProjectOnlineTuningSavedCount() { m_ProjectOnlineTuningSavedCount++; }
	void                       ResetProjectOnlineTuningSavedCount() { m_ProjectOnlineTuningSavedCount=0; }
	size_t                     GetProjectOnlineTuningSavedCount() const { return m_ProjectOnlineTuningSavedCount; }
	bool                       CheckProjectOnlineTuningSaveInspectionFile() const;//確認線上調機儲存檢測檔案
	//---------------------------------------------------------------------------------//	
	bool                       StartWriteStaticDataToLibraryThread();
	bool                       DeleteWriteStaticDataToLibraryThread();
	void                       SetWriteStaticDataToLibraryThreadState(THREAD_STATE_MODE State);//設定執行緒狀態
	THREAD_STATE_MODE          GetWriteStaticDataToLibraryThreadState() const;              //取得執行緒狀態
	void                       SetWriteStaticDataToLibraryThreadCmd(THREAD_COMMAND_MODE Cmd); //設定執行緒命令
	THREAD_COMMAND_MODE        GetWriteStaticDataToLibraryThreadCmd() const;	               //取得執行緒命令	
	bool                       WaitForWriteStaticDataToLibraryThreadFinish();         //等待
	//---------------------------------------------------------------------------------//	
	bool                       DeleteProjectServerLockFile();//刪除專案伺服器鎖住檔案
	bool                       CreateProjectServerLockFile(bool &bLock);//建立專案伺服器鎖住檔案	
	//---------------------------------------------------------------------------------//	
	bool                       WriteProjectServerActiveProjectName(LPCTSTR ProjectName);//寫入專案伺服器共享資料-專案名稱
	CString                    GetProjectServerShareFilename();//取得專案伺服器共享檔名	
	bool                       CheckProjectServerShareFileSavedDateTime(bool &bNewOne);//確認專案伺服器檔案寫入時間	
	bool                       WriteProjectServerShareFile(LPCTSTR KeyName, LPCTSTR Value);//寫入專案伺服器共享資料	
	bool                       ReadProjectServerShareFile(LPCTSTR KeyName, LPCTSTR Default, CString &Value);//讀取專案伺服器共享資料
	//---------------------------------------------------------------------------------//
	bool                       WriteProjectServerProjectLinkMode(PROJECT_LINK_SERVER_MODE LinkMode);//寫入專案伺服器共享資料-連線模式
	bool                       ReadProjectServerProjectLinkMode(PROJECT_LINK_SERVER_MODE &LinkMode);//讀取專案伺服器共享資料-連線模式	
	CString                    GetProjectServerProjectShareFilename();//取得專案伺服器專案共享檔名
	bool                       CheckProjectServerProjectShareFileSavedDateTime(bool &bNewOne);//確認專案伺服器專案寫入時間	
	bool                       WriteProjectServerProjectShareFile(LPCTSTR KeyName, LPCTSTR Value);//寫入專案伺服器共享資料	
	bool                       ReadProjectServerProjectShareFile(LPCTSTR KeyName, LPCTSTR Default, CString &Value);//讀取專案伺服器共享資料
	//---------------------------------------------------------------------------------//
	bool                       SaveProjectAliasFile(LPCTSTR pfilename);//儲存別名檔案
	bool                       BuildProjectAliasList(std::vector<TAliasNode> &AliasList);//建立別名列表
	//---------------------------------------------------------------------------------//	
	bool                       ConvertProjectToLibrary();//將專案轉成資料庫模式	
	bool                       MergeProjectLibrary(CAOIProject *ProjectPtr);//合併專案的模組列表
	bool                       ArrangeProjectLibraryBkImageFiles();//整理專案資料庫底圖檔案
	//---------------------------------------------------------------------------------//	
	bool                       SaveProjectPartBarcodeFile();//儲存專案零件條碼檔案
	bool                       SaveProjectPartBarcodeFileFn();//儲存專案零件條碼檔案
	//---------------------------------------------------------------------------------//	
	void                       SetProjectLoadInspectionResultFile(bool value);
	bool                       GetProjectLoadInspectionResultFile() const;
	//---------------------------------------------------------------------------------//	
	bool                       SaveProjectInspectionResultFile();//儲存專案檢測結果檔案-給線上調機使用			
	bool                       SaveProjectInspectionResultFileFn();//儲存專案檢測結果檔案-給線上調機使用			
	bool                       LoadProjectInspectionResultFile(LPCTSTR pfilename);//載入專案檢測結果檔案-給線上調機使用
	bool                       LoadProjectInspectionResultFileFn(LPCTSTR pfilename);//載入專案檢測結果檔案-給線上調機使用
	bool                       WriteProjectInspectionResultFile(CAOIFileIO &FileIO);//儲存專案檢測結果檔案-給線上調機使用
	bool                       ReadProjectInspectionResultFile(CAOIFileIO &FileIO);//載入專案檢測結果檔案-給線上調機使用		
	//---------------------------------------------------------------------------------//
	bool                       CopyProjectSpcImage();//複製專案SPC圖檔
	bool                       CopyProjectSpcComponentImage(LPCTSTR FileMainNameSrc);//複製專案SPC零件圖檔
	//---------------------------------------------------------------------------------//		
	bool                       SaveProjectSpcHeader();//儲存專案SPC檔頭
	bool                       SaveProjectSpcHeaderFn();//儲存專案SPC檔頭
	bool                       SaveProjectSpcHeader_JSON(bool bChkModified);//儲存專案SPC檔頭
	bool                       SaveProjectSpcHeader_JSON(LPCTSTR pfilename);//儲存專案SPC檔頭
	bool                       SaveProjectSpcHeader_JSON_detail(FILE *pfile);//儲存專案SPC檔頭
	//---------------------------------------------------------------------------------//	
	bool                       SaveProjectSpcFile(LPCTSTR pfilename=NULL);//儲存專案SPC檔案
	bool                       LoadProjectSpcFile(LPCTSTR pfilename);//載入專案SPC檔案
	//---------------------------------------------------------------------------------//
	bool                       SaveProjectSpcFile_Binary(LPCTSTR pfilename = NULL);//儲存專案SPC檔案
	bool                       LoadProjectSpcFile_Binary(LPCTSTR pfilename);//載入專案SPC檔案
	//---------------------------------------------------------------------------------//
	bool                       SaveProjectSpcFile_TXT(LPCTSTR pfilename = NULL);//儲存專案SPC檔案
	bool                       LoadProjectSpcFile_TXT(LPCTSTR pfilename);//載入專案SPC檔案
	//---------------------------------------------------------------------------------//
	bool                       SaveProjectSpcFile_TXT2(LPCTSTR pfilename = NULL);//儲存專案SPC檔案
	//---------------------------------------------------------------------------------//
	bool                       SaveProjectSpcFile_XML(LPCTSTR pfilename = NULL);//儲存專案SPC檔案
	bool                       SaveProjectSpcFile_JSON(LPCTSTR pfilename = NULL);//儲存專案SPC檔案
	bool                       SaveProjectSpcFile_JSON_VRS(LPCTSTR pfilename = NULL);//儲存專案SPC檔案-VRS	
	bool                       SaveProjectSpcFile_JSON_RSM(LPCTSTR pfilename = NULL);//儲存專案SPC檔案-RSM
	//---------------------------------------------------------------------------------//
	bool                       LoadProjectOnlineTuningFile(LPCTSTR pfilename);//在入專案線上調機檔案
	bool                       LoadProjectOnlineTuningFileFn(LPCTSTR pfilename);//在入專案線上調機檔案
	bool                       ClearProjectOnlineTuningFolder();//清除專案線上調機資料夾
	bool                       CopyToProjectOnlineTuningFolder();//複製到專案線上調機資料夾
	bool                       CopyToProjectOnlineOfflineFolder();//複製到專案線上離線編程資料夾
	//---------------------------------------------------------------------------------//
	void                       LockProject();        //進入專案的關鍵區間
	void                       UnlockProject();      //離開專案的關鍵區間	
	//---------------------------------------------------------------------------------//		
	bool                       ExecProjectCopyFileFn();                      //執行專案複製檔案執行緒
	bool                       SetProjectCopyFileEvent();                    //啟用專案複製檔案事件		
	bool                       CreateProjectCopyFileThread();                //建立專案複製檔案執行緒
	bool                       DeleteProjectCopyFileThread();                //刪除專案複製檔案執行緒
	bool                       IdleProjectCopyFileThread(bool WaitOn);       //閒置專案複製檔案執行緒	
	bool                       StartProjectCopyFileThread(bool WaitOn);      //開始專案複製檔案執行緒	
	bool                       WaitForProjectCopyFileThreadFinish();         //等待專案複製檔案執行緒
	bool                       ClearProjectCopyFileList();                   //清除 專案複製檔案列表
	bool                       AddProjectCopyFile(LPCTSTR SrcFolder, LPCTSTR DstFolder, bool bFile);//新增專案複製檔案資料夾		
	bool                       CheckProjectCopyFileThreadState(THREAD_STATE_MODE State);//確認專案複製執行緒狀態
	void                       SetProjectCopyFileThreadState(THREAD_STATE_MODE State);//設定專案複製執行緒狀態
	THREAD_STATE_MODE          GetProjectCopyFileThreadState() const;              //取得專案複製執行緒狀態
	void                       SetProjectCopyFileThreadCmd(THREAD_COMMAND_MODE Cmd); //設定專案複製執行緒命令
	THREAD_COMMAND_MODE        GetProjectCopyFileThreadCmd() const;	               //取得專案複製執行緒命令	
	//---------------------------------------------------------------------------------//	
	bool                       ExecProjectSaveFieldFn();                      //執行專案儲存視野執行緒
	bool                       SetProjectSaveFieldEvent();                    //啟用專案儲存視野事件		
	bool                       CreateProjectSaveFieldThread();                //建立專案儲存視野執行緒
	bool                       DeleteProjectSaveFieldThread();                //刪除專案儲存視野執行緒
	bool                       IdleProjectSaveFieldThread(bool WaitOn);       //閒置專案儲存視野執行緒	
	bool                       StartProjectSaveFieldThread(bool WaitOn);      //開始專案儲存視野執行緒	
	bool                       WaitForProjectSaveFieldThreadFinish();         //等待專案儲存視野執行緒
	bool                       AddProjectSaveField(CAOIField *FieldPtr);      //新增專案儲存視野資料夾	
	bool                       CheckProjectSaveFieldThreadState(THREAD_STATE_MODE State);//確認專案儲存視野執行緒狀態
	void                       SetProjectSaveFieldThreadState(THREAD_STATE_MODE State);//設定專案儲存視野執行緒狀態
	THREAD_STATE_MODE          GetProjectSaveFieldThreadState() const;              //取得專案儲存視野執行緒狀態
	void                       SetProjectSaveFieldThreadCmd(THREAD_COMMAND_MODE Cmd); //設定專案儲存視野執行緒命令
	THREAD_COMMAND_MODE        GetProjectSaveFieldThreadCmd() const;	               //取得專案儲存視野執行緒命令	
	void                       SetProjectSaveFieldThreadExecuted(bool val);   //設定專案儲存視野被執行中
	bool                       GetProjectSaveFieldThreadExecuted() const;     //取得專案儲存視野被執行中
	void                       ClearProjectSaveFiledList();                   //清除專案儲存視野列表
	bool                       CheckProjectSaveFiledCompleted();              //確認專案儲存視野完成
	bool                       GetProjectSaveFiledUsingThread() const;        //取得專案儲存視野使用執行緒			
	//---------------------------------------------------------------------------------//	
	std::string                RemoveProjectBarcodeCharCount(const char *Barcode);//移除專案條碼字元數
	std::wstring               RemoveProjectBarcodeCharCount(const wchar_t *Barcode);//移除專案條碼字元數
	//---------------------------------------------------------------------------------//	
	bool                       SaveProjectBarcodeFile(TASK_MODE TaskMode);//儲存專案條碼檔案
	bool                       LoadProjectBarcodeFile(TASK_MODE TaskMode);//載入專案條碼檔案

	bool                       SaveProjectBarcodeFile_INIFile(TASK_MODE TaskMode);//儲存專案條碼檔案-JET_INI
	bool                       LoadProjectBarcodeFile_INIFile(TASK_MODE TaskMode);//載入專案條碼檔案-JET_INI
	bool                       DecodeProjectBarcodeText(LPCTSTR Text, LPCTSTR Gap, int &PanelID, int &BoardID, CString &Barcode);
	//---------------------------------------------------------------------------------//	
	//專案條碼專置編號
	bool                       CheckProjectBarcodeDeviceIDList();//確認專案的條碼裝置列表
	bool                       CloneProjectBarcodeDeviceUsedList(bool UsedList[]);//複製專案的條碼裝置使用列表			
	bool                       AssignProjectBarcodeDeviceCode(LANE_ID LaneID);//分派專案條碼裝置內容
	//---------------------------------------------------------------------------------//
	bool                       AssignProjectBarcodeHandHeldCode(LANE_ID LaneID);//分派專案手持條碼機內容
	bool                       AssignProjectBarcodeHandHeldCode_Fn(CBarcode_Handheld *BarcodeHandHeldPtr);//分派專案手持條碼機內容	
	bool                       ModifyProjectBarcodeHandHeldCode(CBarcode_Handheld &BarcodeHandHeld);//修改專案手持條碼機內容
	//---------------------------------------------------------------------------------//	
	bool                       SaveProjectTestMapFile();//儲存專案檢測畫面	
	bool                       SaveProjectTestMapFileFn();//儲存專案檢測畫面
	bool                       SaveProjectTestMapFile(LPCTSTR Folder, LPCTSTR Folder2=NULL);//儲存專案檢測畫面
	bool                       SaveProjectTestMapToRepair();//儲存專案檢測畫面至維修站
	bool                       SaveProjectTestMapToRepairFn();//儲存專案檢測畫面至維修站
	bool                       CreateProjectTestMapBuffer();//建立專案檢測底圖畫面 
	bool                       CreateProjectTestMapBufferFn();//建立專案檢測底圖畫面 
	int                        GetProjectTestMapScaleMode() const;//取得專案檢測底圖比例	
	void                       SetProjectTestMapScaleMode(int Mode);//設定專案檢測底圖比例	
	bool                       AddProjectTestMapField(CAOIField *FieldPtr);//增加專案檢測底圖區域
	bool                       AddProjectTestMapFrame(CAOIFrame *FramePtr);//增加專案檢測底圖畫面	
	DWORD                      GetProjectTestMapTickCount(unsigned int index) const;//專案檢測底圖的時間碼
	bool                       GetProjectTestMapPtr(unsigned int MapIndex, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, IMAGE_PTR &Ptr);//取得專案檢測底圖
	//---------------------------------------------------------------------------------//	
	bool                       SaveProjectReport_Text();//輸出專案基本報告
	bool                       SaveProjectReport_Text(LPCTSTR filename);//輸出專案基本報告
	bool                       SaveProjectReport_TextFn(LPCTSTR filename);//輸出專案基本報告
	bool                       SaveProjectReportPartGroup_Text(FILE *pfile);//輸出專案群組零件報告
	//---------------------------------------------------------------------------------//
	bool                       SaveProjectCustomerReport_PegatronTwan();//輸出客戶報告-和碩台灣廠
	bool                       SaveProjectCustomerReport(AOI_CUSTOMER_ID CustomerID);//輸出客戶報告		
	//---------------------------------------------------------------------------------//	
	bool                       SaveProjectCustomerAIFile(AOI_CUSTOMER_ID CustomerID);//輸出客戶AI檔案
	bool                       SaveProjectCustomerAIFile_FoxconnLonghua();//輸出客戶AI檔案-富士康-龍華
	//---------------------------------------------------------------------------------//
	bool                       SaveProjectCustomerAIImage(AOI_CUSTOMER_ID CustomerID);//輸出至客戶AI圖像
	bool                       SaveProjectCustomerAIImage_FoxconnLonghua();//輸出至客戶AI圖像-富士康-龍華
	//---------------------------------------------------------------------------------//
	bool                       SaveProjectReportWndReading_Text();//輸出專案檢測框數據
	bool                       SaveProjectReportWndReading_Text(LPCTSTR filename);//輸出專案檢測框數據
	bool                       SaveProjectReportWndReading_TextFn(LPCTSTR filename);//輸出專案檢測框數據
	//---------------------------------------------------------------------------------//
	bool                       SaveProjectReportWndList_Text();//輸出專案檢測框列表
	bool                       SaveProjectReportWndList_Text(LPCTSTR filename);//輸出專案檢測框列表
	bool                       SaveProjectReportWndList_TextFn(LPCTSTR filename);//輸出專案檢測框列表
	//---------------------------------------------------------------------------------//
	bool                       SaveProjectCadXYFile(LPCTSTR filename);//輸出專案CadXY檔案
	bool                       SaveProjectCadXYFileFn(LPCTSTR filename);//輸出專案CadXY檔案
	bool                       SaveProjectLocationFile(LPCTSTR filename);//輸出專案位置檔案
	bool                       SaveProjectLocationFileFn(LPCTSTR filename);//輸出專案位置檔案
	bool                       SaveProjectTestCoverageFile(LPCTSTR filename, int UnitMode);//輸出專案檢測涵蓋率
	bool                       SaveProjectTestCoverageFileFn(LPCTSTR filename, int UnitMode);//輸出專案檢測涵蓋率
	//---------------------------------------------------------------------------------//	
	bool                       SaveProjectComponentMapLocation(LPCTSTR filename, bool bUnsetModelOnlye);//儲存零件在專案底圖上的位置
	bool                       SaveProjectComponentMapLocationFn(LPCTSTR filename, bool bUnsetModelOnlye);//儲存零件在專案底圖上的位置
	//---------------------------------------------------------------------------------//	
	bool                       SaveProjectComponentDefectAlarmFile(LPCTSTR filename, DEFECT_FROM_MODE Mode);//輸出專案零件瑕疵警報檔案
	bool                       SaveProjectComponentDefectAlarmFileFn(LPCTSTR filename, DEFECT_FROM_MODE Mode);//輸出專案零件瑕疵警報檔案
	//---------------------------------------------------------------------------------//			
	bool                       InitialProjectGroupInspection();//初始化專案群組檢測
	bool                       AnalysisProjectGroupInspection();//分析專案群組檢測	
	bool                       ExecProjectGroupInspection_Colinearity(CAOIPartGroup *GroupPtr);//執行專案群組檢測-直線度	
	bool                       ExecProjectGroupInspection_ColinearityToLine(CAOIPartGroup *GroupPtr);//執行專案群組檢測-共線性-對線
	bool                       ExecProjectGroupInspection_PartToPart(CAOIPartGroup *GroupPtr);//執行專案群組檢測-點對點
	bool                       ExecProjectGroupInspection_NeighborPart(CAOIPartGroup *GroupPtr);//執行專案群組檢測-相鄰點
	bool                       ExecProjectGroupInspection_PartToGroup(CAOIPartGroup *GroupPtr);//執行專案群組檢測-點對群組	
	bool                       ExecProjectGroupInspection_GroupToPart(CAOIPartGroup *GroupPtr);//執行專案群組檢測-群組對點
	bool                       ExecProjectGroupInspection_GroupCoordMap(CAOIPartGroup *GroupPtr);//執行專案群組檢測-群組座標轉換
	//---------------------------------------------------------------------------------//
	bool                       UpdateProjectLaneResult();//更新專案軌道結果
	bool                       AnalysisProjectComponentGroupOffset();//分析專案零件群組偏移量
	//---------------------------------------------------------------------------------//
	bool                       ReleaseProjectObjectKeepImage();//釋放專案物件保留的影像
	//---------------------------------------------------------------------------------//
	bool                       RotateProject(double Angle);//旋轉專案
	//---------------------------------------------------------------------------------//		
	void                       ClearProjectServerModelLockList();//清除專案伺服器模組鎖住列表
	size_t                     GetProjectServerModelLockCount() const;//取得專案伺服器模組鎖住數量
	void                       CloneProjectServerModelLockList(std::vector<CString> &List);//複製專案伺服器模組鎖住列表
	bool                       SaveProjectServerModelLockListToFile(LPCTSTR filename);//儲存專案伺服器模組鎖住列表至檔案
	bool                       SaveProjectServerModelLockListToFileFn(LPCTSTR filename);//儲存專案伺服器模組鎖住列表至檔案
	//---------------------------------------------------------------------------------//	
	COPY_HUGE_FILES_MODE       GetProjectCopyServerHugeFileMode();
	//---------------------------------------------------------------------------------//
	//專案的伺服器群組資料庫資料夾
	bool                       CreateProjectServerLibraryFolder();
	LPCTSTR                    GetProjectServerLibraryFolder() const;	
	void                       SetProjectServerLibraryFolder(LPCTSTR value);	
	//---------------------------------------------------------------------------------//
	bool                       ClearProjectServerLibrary();//清除專案伺服器資料庫
	bool                       LoadProjectServerLibrary(LPCTSTR ServerFolder);//載入專案伺服器資料庫
	bool                       LoadProjectServerLibraryFn(LPCTSTR ServerFolder);//載入專案伺服器資料庫
	//---------------------------------------------------------------------------------//	
	bool                       SaveProjectToServerProject();//儲存專案庫至伺服器專案	
	bool                       SaveProjectToServerProjectFn();//儲存專案庫至伺服器專案	
	bool                       LoadProjectFromServerProject(PROJECT_LINK_SERVER_MODE LinkMode);//載入專案從伺服器專案
	bool                       LoadProjectFromServerProjectFn(PROJECT_LINK_SERVER_MODE LinkMode);//載入專案從伺服器專案
	bool                       SaveProjectFileToServerProject(LPCTSTR ServerProjectFolder, LPCTSTR ServerLibraryFolder);//儲存專案至伺服器專案
	bool                       SaveProjectFileToServerProjectFn(LPCTSTR ServerProjectFolder, LPCTSTR ServerLibraryFolder);//儲存專案至伺服器專案
	bool                       LoadProjectFileFromServerProject(LPCTSTR ServerProjectFolder, LPCTSTR ServerLibraryFolder);//載入專案從伺服器專案			
	bool                       LoadProjectFileFromServerProjectFn(LPCTSTR ServerProjectFolder, LPCTSTR ServerLibraryFolder);//載入專案從伺服器專案 
	bool                       SaveProjectLibryToServerProjectLibrary(LPCTSTR ServerFolder);//儲存專案資料庫至伺服器專案資料庫	
	bool                       SaveProjectLibryToServerProjectLibraryFn(LPCTSTR ServerFolder);//儲存專案資料庫至伺服器專案資料庫	
	bool                       SaveProjectPartLibryToServerProjectPartLibrary(LPCTSTR ServerFolder);//儲存專案零件資料庫至伺服器專案零件資料庫	
	bool                       SaveProjectPartLibryToServerProjectPartLibraryFn(LPCTSTR ServerFolder);//儲存專案零件資料庫至伺服器專案零件資料庫	
	bool                       SaveProjectLibryToServerLibrary(LPCTSTR ServerLibraryFolder, LPCTSTR ServerProjectLibraryFolder);//儲存專案資料庫至伺服器資料庫
	bool                       SaveProjectLibryToServerLibraryFn(LPCTSTR ServerLibraryFolder, LPCTSTR ServerProjectLibraryFolder);//儲存專案資料庫至伺服器資料庫
	bool                       LoadProjectLibryFromServerLibrary(LPCTSTR ServerFolder, bool bIncNewModel, bool bLoadAll);//載入專案資料庫從伺服器資料庫
	bool                       LoadProjectLibryFromServerLibraryFn(LPCTSTR ServerFolder, bool bIncNewModel, bool bLoadAll);//載入專案資料庫從伺服器資料庫
	bool                       LoadProjectLibryFromServerProjectLibrary(LPCTSTR LibraryFolderSrc, LPCTSTR LibraryFolderDst, std::vector<TModelInfo> &BefList, std::vector<TModelInfo> &AftList, std::vector<TModelInfo> &NewList, std::vector<TModelInfo> &DelList, std::vector<TModelInfo> &RepList, std::vector<CString> &CopyList);//載入專案資料庫從伺服器專案資料庫	
	bool                       LoadProjectLibryFromServerProjectLibraryFn(LPCTSTR LibraryFolderSrc, LPCTSTR LibraryFolderDst, std::vector<TModelInfo> &BefList, std::vector<TModelInfo> &AftList, std::vector<TModelInfo> &NewList, std::vector<TModelInfo> &DelList, std::vector<TModelInfo> &RepList, std::vector<CString> &CopyList);//載入專案資料庫從伺服器專案資料庫	
	//---------------------------------------------------------------------------------//
	bool                       SaveProjectLibryToServerLibrary_Backup(LPCTSTR ServerFolder);//儲存專案資料庫至伺服器資料庫
	bool                       LoadProjectLibryFromServerLibrary_Backup(LPCTSTR ServerFolder);//載入專案資料庫從伺服器資料庫
	//---------------------------------------------------------------------------------//
	bool                       SaveProjectServerLogFile(LPCTSTR filename, std::vector<CString> &LogList);
	bool                       SaveProjectServerLogFileFn(LPCTSTR filename, std::vector<CString> &LogList);
	//---------------------------------------------------------------------------------//			
	bool                       ResetProjectVersionCodeList();//復歸專案版本號列表	
	size_t                     GetProjectVersionCodeCount() const;//取得專案版本號數量	
	bool                       CopyProjectVersionCode(size_t idx, LPCTSTR Name);//複製專案版本號
	bool                       DeleteProjectVersionCode(size_t idx);//刪除專案版本號	
	bool                       AddProjectVersionCode(LPCTSTR Name);//新增加專案版本號	
	size_t                     GetProjectVersionCodeNewIndex() const;//取得新的版本號引數	
	TVersionCode*              GetProjectVersionCodeActivePtr();//取得目前專案版本號	
	bool                       UpdateToProjectVersionCode();//將狀態更新至目前專案版本號碼
	bool                       LayoutProjectVersionCodeList();//整理專案版本號列表
	bool                       ChangeProjectVersionCode(size_t idx);//切換專案版本號碼引數			
	bool                       CheckProjectVersionCodeIndex(size_t index);//確認專案版本號碼引數			
	TVersionCode*              GetProjectVersionCodePtr(size_t idx, bool bCheck);//取得專案版本號		
	bool                       SetProjectVersionCodeList(const std::vector<TVersionCode> &VersionList);//設定專案版本號列表	
	//---------------------------------------------------------------------------------//
	bool                       ReadProjectAIModelComponentLabelFile(LPCTSTR Filename);
	bool                       ReadProjectAIModelComponentLabel_JSON(LPCTSTR Filename);
	bool                       WriteProjectAIModelComponentLabel_JSON(LPCTSTR Filename, LPCTSTR ImgName1, LPCTSTR ImgName2);
	bool                       AutoLabelProjectComponentModelGroupByAI();//自動分類專案零件模組群組-AI		
	//---------------------------------------------------------------------------------//
	void                       SetHASI_MinCadPos(TPOINT2D Cad) { m_HASI_MinCadPos = Cad; }
	TPOINT2D                   GetHASI_MinCadPos() { return m_HASI_MinCadPos; }
	//---------------------------------------------------------------------------------//
	bool                       SaveProjectMappingCadXYFile(LPCTSTR filename);//輸出專案CadXY檔案
	bool                       SaveProjectMappingCadXYFileFn(LPCTSTR filename);//輸出專案CadXY檔案
	//---------------------------------------------------------------------------------//

};
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_AOIPROJECT_H__B9BED84A_8D4E_47E2_9E23_98908A354313__INCLUDED_)
