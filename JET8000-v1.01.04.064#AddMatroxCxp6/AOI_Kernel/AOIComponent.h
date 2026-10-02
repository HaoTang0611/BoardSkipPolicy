// AOIComponent.h: interface for the CAOIComponent class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#if !defined(AFX_AOICOMPONENT_H__C766CB12_822D_4845_9EA9_D0E70DC0C0CC__INCLUDED_)
#define AFX_AOICOMPONENT_H__C766CB12_822D_4845_9EA9_D0E70DC0C0CC__INCLUDED_
//-------------------------------------------------------------------------------------//
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include <vector>
#include "AOIRgn.h"
#include "AOIWindow.h"
#include "AOIModel.h"
#include "WndDefectItem.h"
//-------------------------------------------------------------------------------------//
class CAOIPanel;
class CAOIBoard;
class CAOIFileIO;
//-------------------------------------------------------------------------------------//
const int MAX_CADXY_NAME = 64; 
//-------------------------------------------------------------------------------------//
#define SEL_COL_ROW_IDX_NONE          0
#define SEL_COL_ROW_IDX_ODD           1
#define SEL_COL_ROW_IDX_EVEN          2
//-------------------------------------------------------------------------------------//
#define ARRAY_PASTE_NAME_DEFAULT      1
#define ARRAY_PASTE_NAME_ROW_FIRST    2
#define ARRAY_PASTE_NAME_COL_FIRST    3
//-------------------------------------------------------------------------------------//car
enum COMPONENT_TYPE
{
	COMPONENT_TYPE_NORMAL     = 1,
	COMPONENT_TYPE_FULL_MAP   = 2,
	COMPONENT_TYPE_SPECIAL    = 10,
	COMPONENT_TYPE_DROP_OUT   = 11,
	COMPONENT_TYPE_SCRATCH    = 12,
	COMPONENT_TYPE_TEMPORARY  = 21,
	COMPONENT_TYPE_RETURN
};
//-------------------------------------------------------------------------------------//
class CAOIComponent;
typedef struct tagComponentRect
{		
	unsigned int   PanelIndex;
	unsigned int   BoardIndex;
	unsigned int   ComponentIndex;
	double         ComponentAngle;
	bool           IsExceptionAngle;
	CAOIComponent *ComponentPtr;
	TRECT4D        Rect;
	TPOINT2D       CornerPts[4];
	tagComponentRect()
	{
		PanelIndex = -1;
		BoardIndex = -1;
		ComponentIndex = -1;
		ComponentAngle = 0;
		IsExceptionAngle = false;
		ComponentPtr = NULL;
		Rect = TRECT4D();
		CornerPts[0] = TPOINT2D();
		CornerPts[1] = TPOINT2D();
		CornerPts[2] = TPOINT2D();
		CornerPts[3] = TPOINT2D();
	}
} TComponentRect, *PComponentRect; 
//-------------------------------------------------------------------------------------//
class CAOIComponent : public CAOIRgn  
{
	//---------------------------------------------------------------------------------//	
	DECLARE_DYNAMIC(CAOIComponent)
	//---------------------------------------------------------------------------------//		
private:
	//---------------------------------------------------------------------------------//
	int                        m_ComponentUniqueID;//零件唯一碼
	bool                       m_ComponentDeleted;//零件是否刪除
	bool                       m_ComponentSelected;//零件是否選取到	
	bool                       m_ComponentVisibled;//零件是否顯示	
	bool                       m_ComponentXBoardUnit;//零件是否X板單位
	bool                       m_ComponentNeedToUpdate;//零件是否需要重新計算	
	int                        m_ComponentColIndex;//零件欄位(Col)編號
	int                        m_ComponentRowIndex;//零件列位(Row)編號
	TPOINT2D                   m_ComponentOrgCadPos;//零件原始座標在CAD坐標系中
	COMPONENT_TYPE             m_ComponentType;//零件樣式		
	UINT                       m_ComponentConfirmUIResultID;//零件確認視窗結果(IDOK, IDCANCEL)
	//---------------------------------------------------------------------------------//
	std::vector<TVersionParam> m_ComponentVersionParamList;//零件版本號參數
	std::vector<TVersionParam> m_ComponentVersionParamListBackup;//零件版本號參數-備份
	//---------------------------------------------------------------------------------//
	std::wstring               m_ComponentName;//零件名稱
	std::wstring               m_ComponentModelName;//零件模組
	std::wstring               m_ComponentPartNumber;//零件料號	
	std::wstring               m_ComponentNozzleName;//零件吸嘴名稱
	//---------------------------------------------------------------------------------//
	bool                       m_ComponentSaveReport_ARS;//零件儲存報告-維修站
	//---------------------------------------------------------------------------------//
	std::wstring               m_ComponentTempText;//零件暫存用的文字
	//---------------------------------------------------------------------------------//		
	size_t                     m_ComponentTempIndex;//零件暫時編號, 無意義
	int                        m_ComponentTempInt[4];//零件暫時變數, 無意義	
	//---------------------------------------------------------------------------------//
	size_t                     m_ComponentTempIndexModel;//零件模組編號, 建立樹狀圖使用, 無意義
	size_t                     m_ComponentTempIndexPartNumber;//零件料號編號, 建立樹狀圖使用, 無意義
	//---------------------------------------------------------------------------------//	
	size_t                     m_ComponentTop10IndexModel;//零件模組編號, 前十大不良
	size_t                     m_ComponentTop10IndexModel_LA;//零件模組編號, 前十大不良
	size_t                     m_ComponentTop10IndexModel_LB;//零件模組編號, 前十大不良
	size_t                     m_ComponentTop10IndexPartNumber;//零件料號編號, 前十大不良
	size_t                     m_ComponentTop10IndexPartNumber_LA;//零件料號編號, 前十大不良
	size_t                     m_ComponentTop10IndexPartNumber_LB;//零件料號編號, 前十大不良
	//---------------------------------------------------------------------------------//	
	CAOIModel                  m_ComponentModel;
	unsigned int               m_ComponentModelIndex;
	int                        m_ComponentModelClassID;//零件指定模組類別編號
	bool                       m_ComponentModelIsolated;//零件模組隔離			
	//---------------------------------------------------------------------------------//	
	std::vector<CAOIWindow>    m_ComponentWindowList;
	bool                       m_ComponentSaveWndList;//零件儲存檢測框列表
	//---------------------------------------------------------------------------------//
	double                     m_ComponentResultWidth;//結果寬度-um	
	double                     m_ComponentResultLength;//結果長度-um
	double                     m_ComponentResultHeight;//結果高度-um	
	double                     m_ComponentResultArea;//結果面積-um^2
	double                     m_ComponentResultVolume;//結果體積-um^3
	double                     m_ComponentResultOffsetX;//結果偏移量-X
	double                     m_ComponentResultOffsetY;//結果偏移量-Y	
	double                     m_ComponentResultOffsetA;//結果偏移量-角度
	double                     m_ComponentResultSkewAngle;//結果偏移角度	
	double                     m_ComponentResultTiltAngle;//結果傾斜角度
	double                     m_ComponentResultHeightMin;//結果高度最小-um
	double                     m_ComponentResultHeightMax;//結果高度最大-um
	//---------------------------------------------------------------------------------//
	double                     m_ComponentResultOffsetX_LA;//結果偏移量-X
	double                     m_ComponentResultOffsetY_LA;//結果偏移量-Y
	double                     m_ComponentResultSkewAngle_LA;//結果偏移角度
	double                     m_ComponentResultTiltAngle_LA;//結果傾斜角度
	double                     m_ComponentResultOffsetX_LB;//結果偏移量-X
	double                     m_ComponentResultOffsetY_LB;//結果偏移量-Y
	double                     m_ComponentResultSkewAngle_LB;//結果偏移角度
	double                     m_ComponentResultTiltAngle_LB;//結果傾斜角度
	//---------------------------------------------------------------------------------//	
	double                     m_ComponentResultWidth_USL;//結果寬度-um-上限
	double                     m_ComponentResultWidth_LSL;//結果寬度-um-下限
	unsigned int               m_ComponentResultWidth_WndIdx;//結果寬度-檢測框引數	
	double                     m_ComponentResultLength_USL;//結果長度-um-上限
	double                     m_ComponentResultLength_LSL;//結果長度-um-下限
	unsigned int               m_ComponentResultLength_WndIdx;//結果長度-檢測框引數
	double                     m_ComponentResultHeight_USL;//結果高度-um-上限
	double                     m_ComponentResultHeight_LSL;//結果高度-um-下限
	unsigned int               m_ComponentResultHeight_WndIdx;//結果高度-檢測框引數
	double                     m_ComponentResultArea_USL;//結果面積-um^2-上限
	double                     m_ComponentResultArea_LSL;//結果面積-um^2-下限
	unsigned int               m_ComponentResultArea_WndIdx;//結果面積-檢測框引數
	double                     m_ComponentResultVolume_USL;//結果體積-um^3-上限
	double                     m_ComponentResultVolume_LSL;//結果體積-um^3-下限
	unsigned int               m_ComponentResultVolume_WndIdx;//結果體積-檢測框引數
	double                     m_ComponentResultOffsetX_USL;//結果偏移量-X-上限
	double                     m_ComponentResultOffsetX_LSL;//結果偏移量-X-下限
	unsigned int               m_ComponentResultOffsetX_WndIdx;//結果偏移量-X-檢測框引數
	double                     m_ComponentResultOffsetY_USL;//結果偏移量-Y-上限	
	double                     m_ComponentResultOffsetY_LSL;//結果偏移量-Y-下限
	unsigned int               m_ComponentResultOffsetY_WndIdx;//結果偏移量-Y-檢測框引數
	double                     m_ComponentResultOffsetA_USL;//結果偏移量-角-上限	
	double                     m_ComponentResultOffsetA_LSL;//結果偏移量-角-下限
	unsigned int               m_ComponentResultOffsetA_WndIdx;//結果偏移量-角-檢測框引數
	double                     m_ComponentResultSkewAngle_USL;//結果偏移角度-上限	
	double                     m_ComponentResultSkewAngle_LSL;//結果偏移角度-下限
	unsigned int               m_ComponentResultSkewAngle_WndIdx;//結果偏移角度-檢測框引數
	double                     m_ComponentResultTiltAngle_USL;//結果傾斜角度-上限
	double                     m_ComponentResultTiltAngle_LSL;//結果傾斜角度-下限
	unsigned int               m_ComponentResultTiltAngle_WndIdx;//結果傾斜角度-檢測框引數
	//---------------------------------------------------------------------------------//		
	double                     m_ComponentGrrOffsetX;
	double                     m_ComponentGrrOffsetY;
	double                     m_ComponentGrrSkewA;
	double                     m_ComponentGrrBodyHeight;
	int                        m_ComponentGrrSigmaItemIdx;	
	TGrrSigmaItem              m_ComponentGrrSigmaItemNull; 
	std::vector<TGrrSigmaItem> m_ComponentGrrSigmaItemList;//零件檢測GRR項目 
	TSigmaItem                 m_ComponentResultSigmaItem_LA;//零件檢測標準差項目 
	TSigmaItem                 m_ComponentResultSigmaItem_LB;//零件檢測標準差項目 
	//---------------------------------------------------------------------------------//
	double                     m_ComponentCadResultX;//Cad結果位置-X
	double                     m_ComponentCadResultY;//Cad結果位置-Y
	//---------------------------------------------------------------------------------//
	double                     m_ComponentStageResultX;//機台最後位置-X
	double                     m_ComponentStageResultY;//機台最後位置-Y
	//---------------------------------------------------------------------------------//
	double                     m_ComponentStageOffsetX;//機台偏差量-X
	double                     m_ComponentStageOffsetY;//機台偏差量-Y
	//---------------------------------------------------------------------------------//
	double                     m_ComponentMaskExtendW_Body;//零件遮罩外擴寬度-本體
	double                     m_ComponentMaskExtendH_Body;//零件遮罩外擴長度-本體
	double                     m_ComponentMaskExtendW_Land;//零件遮罩外擴寬度-焊盤
	double                     m_ComponentMaskExtendH_Land;//零件遮罩外擴長度-焊盤
	//---------------------------------------------------------------------------------//
	//零件群組	
	int                        m_ComponentGroupID;
	bool                       m_ComponentGroupOrg;	
	double                     m_ComponentGroupOffsetResX;
	double                     m_ComponentGroupOffsetResY;
	double                     m_ComponentGroupOffsetMaxX;
	double                     m_ComponentGroupOffsetMaxY;
	double                     m_ComponentGroupOffsetMinX;
	double                     m_ComponentGroupOffsetMinY;
	double                     m_ComponentGroupDistanceResX;
	double                     m_ComponentGroupDistanceResY;
	//---------------------------------------------------------------------------------//	
	bool                       m_ComponentEnableAlarm;//零件警報		
	bool                       m_ComponentDefectCountEnableOnARS;//零件瑕疵計數警報-維修站顯示		
	bool                       m_ComponentDefectAlarmEnableOnAOI;//零件瑕疵警報啟用-機台警報
	bool                       m_ComponentDefectAlarmEnableOnARS;//零件瑕疵警報啟用-維修站顯示
	bool                       m_ComponentDefectAlarmResultOnAOI;//零件是否瑕疵警報結果-機台停機
	bool                       m_ComponentDefectAlarmResultOnARS;//零件是否瑕疵警報結果-維修站顯示停機
	CWndDefectItem             m_ComponentDefectItemAlarmAOI;//零件瑕疵警報項目模式-AOI
	CWndDefectItem             m_ComponentDefectItemAlarmARS;//零件瑕疵警報項目模式-ARS
	DEFECT_PARAM_FROM_MODE     m_ComponentDefectAlarmFromModeAOI;//零件瑕疵警報參數來源模式-AOI
	DEFECT_PARAM_FROM_MODE     m_ComponentDefectAlarmFromModeARS;//零件瑕疵警報參數來源-ARS
	//---------------------------------------------------------------------------------//
	size_t                     m_ComponentTotalTestCountAOI;//零件檢測數量-設備檢出
	size_t                     m_ComponentTotalTestCountAOI_LA;//零件檢測數量-設備檢出
	size_t                     m_ComponentTotalTestCountAOI_LB;//零件檢測數量-設備檢出
	size_t                     m_ComponentTotalTestCountARS;//零件檢測數量-人員判定
	size_t                     m_ComponentTotalTestCountARS_LA;//零件檢測數量-人員判定
	size_t                     m_ComponentTotalTestCountARS_LB;//零件檢測數量-人員判定
	//---------------------------------------------------------------------------------//	
	size_t                     m_ComponentTotalNGCountAOI;//零件累計不良數量-設備檢出
	size_t                     m_ComponentTotalNGCountAOI_LA;//零件累計不良數量-設備檢出
	size_t                     m_ComponentTotalNGCountAOI_LB;//零件累計不良數量-設備檢出
	size_t                     m_ComponentTotalNGCountARS;//零件累計不良數量-人員判定
	size_t                     m_ComponentTotalNGCountARS_LA;//零件累計不良數量-人員判定
	size_t                     m_ComponentTotalNGCountARS_LB;//零件累計不良數量-人員判定
	size_t                     m_ComponentTotalNGCountLimit;//零件累計不良數量上限
	bool                       m_ComponentTotalNGCountEnable;//零件累計不良數量啟用
	bool                       m_ComponentTotalNGCountAlarm;//零件累計不良數量警報
	//---------------------------------------------------------------------------------//
	size_t                     m_ComponentContinueNGCountAOI;//零件連續不良數量-設備檢出
	size_t                     m_ComponentContinueNGCountARS;//零件連續不良數量-人員判定		
	size_t                     m_ComponentContinueNGCountLimit;//零件連續不良數量上限
	bool                       m_ComponentContinueNGCountEnable;//零件連續不良數量啟用
	bool                       m_ComponentContinueNGCountAlarm;//零件連續不良數量警報
	//---------------------------------------------------------------------------------//
	CWndDefectItem             m_ComponentCurrentDefectCountAOI;//零件現今每個瑕疵數量-設備檢出
	CWndDefectItem             m_ComponentCurrentDefectCountARS;//零件現今每個瑕疵數量-人員判定	
	CWndDefectItem             m_ComponentTotaEachlDefectCountAOI;//零件累計每個瑕疵數量-設備檢出
	CWndDefectItem             m_ComponentTotaEachlDefectCountAOI_LA;//零件累計每個瑕疵數量-設備檢出
	CWndDefectItem             m_ComponentTotaEachlDefectCountAOI_LB;//零件累計每個瑕疵數量-設備檢出
	CWndDefectItem             m_ComponentTotalEachDefectCountARS;//零件累計每個瑕疵數量-人員判定
	CWndDefectItem             m_ComponentTotalEachDefectCountARS_LA;//零件累計每個瑕疵數量-人員判定
	CWndDefectItem             m_ComponentTotalEachDefectCountARS_LB;//零件累計每個瑕疵數量-人員判定
	std::vector<CWndDefectItem> m_ComponentResultListAOI;//零件檢測結果列表-設備檢出
	std::vector<CWndDefectItem> m_ComponentResultListARS;//零件檢測結果列表-人員判定
	//---------------------------------------------------------------------------------//
	//NPM-APC-Panasonic NPM APC Param	
	double                     m_NPM_APC_MffX;//APC-零件偏移-X-um
	double                     m_NPM_APC_MffY;//APC-零件偏移-Y-um
	double                     m_NPM_APC_MffA;//APC-零件偏移-A-%
	unsigned int               m_NPM_APC_FF1_IDNUM;//APC-FF1-零件編號
	double                     m_NPM_APC_EPosX;//APC-電極偏移-X-um
	double                     m_NPM_APC_EPosY;//APC-電極偏移-Y-um
	double                     m_NPM_APC_EPosA;//APC-電極偏移-A-%
	unsigned int               m_NPM_APC_MFB_IDNUM;//APC-MFB-零件編號
	//---------------------------------------------------------------------------------//
	//M2M Hanwha
	bool                       m_HASI_SPIOffset_Enable;//HANWHA-是否要套用SPI-OFFSET
	bool                       m_HASI_SPIOffset_IsApplied;//HANWHA-SPI-OFFSET套用是否成功
	bool                       m_HASI_SaveImage;//HANWHA-存圖
	//---------------------------------------------------------------------------------//
	CAOIComponent*             m_ComponentResultPtr;//零件結果指標-預設自己或者代理件
	unsigned int               m_ComponentMasterIndex;//零件本尊編號
	CAOIComponent*             m_ComponentMasterPtr;//零件本尊指標	
	unsigned int               m_ComponentAgentIndex;//零件代理人編號
	std::vector<CAOIComponent*> m_ComponentAgentList;//零件代理人列表	
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//	
	void                       PreInitComponent();
	void                       InitialComponent();
	void                       CloneComponent(const CAOIComponent &Component);
	void                       CloneComponentWindow(const CAOIComponent &Component);
	//---------------------------------------------------------------------------------//	
	size_t                     GetComponentWindowCount_Inline() const;
	void                       AddComponentWindow_Inline(const CAOIWindow &Window);
	CAOIWindow*                GetComponentWindowPtr_Inline(size_t index);
	//---------------------------------------------------------------------------------//	
public:
	CAOIComponent();
	CAOIComponent(const CAOIComponent &Component);
	virtual ~CAOIComponent();
	CAOIComponent& operator=(const CAOIComponent &Component);
	//---------------------------------------------------------------------------------//
	CAOIComponent*             CloneComponentObj() const;//建立且複製一個零件
	//---------------------------------------------------------------------------------//	
	bool                       GetSaveComponentLog() const;//取得是否儲存零件訊息
	bool                       SaveComponentLog(LPCTSTR str);//儲存零件訊息
	bool                       SaveComponentLog(LPCTSTR strSet, LPCTSTR str);//儲存零件訊息
	//---------------------------------------------------------------------------------//	
	bool                       WriteComponentFile(CAOIFileIO &FileIO);//儲存零件檔案
	bool                       ReadComponentFile(CAOIFileIO &FileIO);//載入零件檔案
	//---------------------------------------------------------------------------------//	
	bool                       WriteComponentReportText(FILE *pfile);//儲存零件報告
	//---------------------------------------------------------------------------------//	
	bool                       WriteComponentSpcFile_JSON_VRS(FILE *pfile, CAOIProject *ProjectPtr);
	bool                       WriteComponentSpcFile_JSON_RSM(FILE *pfile, CAOIProject *ProjectPtr);
	bool                       WriteComponentSpcHeader_JSON_detail(FILE *pfile, CAOIProject *ProjectPtr);
	//---------------------------------------------------------------------------------//
	bool                       ConvertToSpcComponent(TSpcComponent &SpcComponent);//轉成Spc零件
	bool                       ConvertToComponentNode(TComponentNode &ComponentNode);//轉成零件節點
	//---------------------------------------------------------------------------------//
	//零件唯一碼
	void                       SetComponentUniqueID(int value) { m_ComponentUniqueID=value; }
	int                        GetComponentUniqueID() const { return m_ComponentUniqueID; }
	//---------------------------------------------------------------------------------//		
	void                       SetComponentIndex_Project(unsigned int value) { SetRgnIndex_Project(value); }
	unsigned int               GetComponentIndex_Project() const { return GetRgnIndex_Project(); }
	//---------------------------------------------------------------------------------//		
	void                       SetComponentIndex_Panel(unsigned int value) { SetRgnIndex_Panel(value); }
	unsigned int               GetComponentIndex_Panel() const { return GetRgnIndex_Panel(); }
	//---------------------------------------------------------------------------------//
	void                       SetComponentIndex_Board(unsigned int value) { SetRgnIndex_Board(value); }
	unsigned int               GetComponentIndex_Board() const { return GetRgnIndex_Board(); }
	//---------------------------------------------------------------------------------//
	void                       SetComponentProjectPtr(CAOIProject *value) { SetRgnProjectPtr(value); }
	CAOIProject*               GetComponentProjectPtr() const { return GetRgnProjectPtr(); }
	//---------------------------------------------------------------------------------//
	void                       SetComponentPanelPtr(CAOIPanel *value) { SetRgnPanelPtr(value); }
	CAOIPanel*                 GetComponentPanelPtr() const { return GetRgnPanelPtr(); }
	//---------------------------------------------------------------------------------//	
	void                       SetComponentPanelIndex_Project(unsigned int value) { SetRgnPanelIndex_Project(value); }
	unsigned int               GetComponentPanelIndex_Project() const { return GetRgnPanelIndex_Project(); }
	//---------------------------------------------------------------------------------//
	void                       SetComponentBoardPtr(CAOIBoard *value) { SetRgnBoardPtr(value); }
	CAOIBoard*                 GetComponentBoardPtr() const { return GetRgnBoardPtr(); }
	//---------------------------------------------------------------------------------//		
	void                       SetComponentBoardIndex_Project(unsigned int value) { SetRgnBoardIndex_Project(value); }
	unsigned int               GetComponentBoardIndex_Project() const { return GetRgnBoardIndex_Project(); }
	//---------------------------------------------------------------------------------//	
	void                       SetComponentBoardIndex_Panel(unsigned int value) { SetRgnBoardIndex_Panel(value); }
	unsigned int               GetComponentBoardIndex_Panel() const { return GetRgnBoardIndex_Panel(); }
	//---------------------------------------------------------------------------------//	
	bool                       CreateComponentSelfFieldPtr();//建立專屬Field指標	
	void                       SetComponentSelfFieldPtr(CAOIField *Ptr) { m_RgnSelfFieldPtr = Ptr; }
	CAOIField*                 GetComponentSelfFieldPtr() const { return m_RgnSelfFieldPtr; }
	//---------------------------------------------------------------------------------//
	void                       SetComponentSelfFieldEnabled(bool val) { m_RgnSelfFieldEnabled=val; }
	bool                       GetComponentSelfFieldEnabled() const { return m_RgnSelfFieldEnabled; }	
	//---------------------------------------------------------------------------------//	
	void                       SetComponentFieldPtr(CAOIField *Ptr);
	CAOIField*                 GetComponentFieldPtr() const { return m_RgnFieldPtr; }
	//---------------------------------------------------------------------------------//		
	void                       SetComponentFieldIndex(unsigned int index) { m_RgnFieldIdx = index; }
	unsigned int               GetComponentFieldIndex() const { return m_RgnFieldIdx; }
	//---------------------------------------------------------------------------------//		
	void                       SetComponentDeleted(bool value) { m_ComponentDeleted = value; }
	bool                       GetComponentDeleted() const { return m_ComponentDeleted; }
	//---------------------------------------------------------------------------------//	
	void                       SetComponentSelected(bool value) { m_ComponentSelected = value; }
	bool                       GetComponentSelected() const { return m_ComponentSelected; }
	//---------------------------------------------------------------------------------//	
	void                       SetComponentVisibled(bool value) { m_ComponentVisibled = value; }
	bool                       GetComponentVisibled() const { return m_ComponentVisibled; }
	//---------------------------------------------------------------------------------//
	bool                       ChangeComponentBoard(CAOIBoard *RefBoardPtr);//變更零件的單板
	//---------------------------------------------------------------------------------//
	bool                       CheckComponentTypeBeClicked() const;//確認零件樣式是否可以被點選
	//---------------------------------------------------------------------------------//
	//軌道編號
	LANE_ID                    GetComponentLaneID() const;
	void                       SetComponentLaneID(LANE_ID value);	
	//---------------------------------------------------------------------------------//
	//不檢測
	void                       SetComponentBypassed(bool value);
	bool                       GetComponentBypassed() const { return GetRgnBypassed(); }	
	bool                       UpdateComponentBypassed(); //確認零件是否為不檢測
	//---------------------------------------------------------------------------------//	
	//3D不檢測
	void                       SetComponentBypass3D(bool value) { SetRgnBypass3D(value); }
	bool                       GetComponentBypass3D() const { return GetRgnBypass3D(); }	
	//---------------------------------------------------------------------------------//
	//使用的OpenMP數量
	int                        CalcComponentOpenMPCountByPixels();
	void                       SetComponentOpenMPCount(int value);
	int                        GetComponentOpenMPCount() const { return GetRgnOpenMPCount(); }
	//---------------------------------------------------------------------------------//
	void                       ChangeComponentXBoardUnit(bool value);
	void                       SetComponentXBoardUnit(bool value) { m_ComponentXBoardUnit = value; }
	bool                       GetComponentXBoardUnit() const { return m_ComponentXBoardUnit; }
	//---------------------------------------------------------------------------------//		
	void                       SetComponentNeedToUpdate(bool value) { m_ComponentNeedToUpdate = value; }
	bool                       GetComponentNeedToUpdate() const { return m_ComponentNeedToUpdate; }
	//---------------------------------------------------------------------------------//	
	void                       SetComponentColIndex(int value) { m_ComponentColIndex = value; }
	int                        GetComponentColIndex() const { return m_ComponentColIndex; }
	//---------------------------------------------------------------------------------//	
	void                       SetComponentRowIndex(int value) { m_ComponentRowIndex = value; }
	int                        GetComponentRowIndex() const { return m_ComponentRowIndex; }
	//---------------------------------------------------------------------------------//	
	//零件確認視窗結果(IDOK, IDCANCEL)
	void                       SetComponentConfirmUIResultID(UINT value) { m_ComponentConfirmUIResultID = value; }
	UINT                       GetComponentConfirmUIResultID() const { return m_ComponentConfirmUIResultID; }
	//---------------------------------------------------------------------------------//	
	//零件樣式
	bool                       CheckComponentType_FullMap() const;//確認零件樣式-全底圖零件
	bool                       CheckComponentType_ModelTest() const;//確認零件樣式-支援模組檢測
	void                       SetComponentType(COMPONENT_TYPE value) { m_ComponentType = value; }
	COMPONENT_TYPE             GetComponentType() const { return m_ComponentType; }
	//---------------------------------------------------------------------------------//	
	void                       BypassSkipComponent(RESULT_ID value);//不檢測或跳過零件
	//---------------------------------------------------------------------------------//	
	RESULT_ID                  CheckComponentResultID_AOI();//確認零件結果編號-AOI	
	RESULT_ID                  GetComponentResultID_AOI() const;//取得零件結果編號-AOI
	void                       SetComponentResultID_AOI(RESULT_ID value);//設定零件結果編號-AOI	
	//---------------------------------------------------------------------------------//	
	RESULT_ID                  GetComponentResultID_AOI_LA() const;//取得零件結果編號-AOI-A軌
	void                       SetComponentResultID_AOI_LA(RESULT_ID value);//設定零件結果編號-AOI-A軌
	//---------------------------------------------------------------------------------//	
	RESULT_ID                  GetComponentResultID_AOI_LB() const;//取得零件結果編號-AOI-B軌
	void                       SetComponentResultID_AOI_LB(RESULT_ID value);//設定零件結果編號-AOI-B軌
	//---------------------------------------------------------------------------------//	
	void                       UpdateComponentResultID_AOI_Lane(LANE_ID LaneID);//更新零件結果編號-AOI-軌道
	RESULT_ID                  GetComponentResultID_AOI_Lane(LANE_ID LaneID) const;//取得零件結果編號-AOI-軌道	
	//---------------------------------------------------------------------------------//	
	RESULT_ID                  GetComponentResultID_ARS() const;//取得零件結果編號-ARS
	void                       SetComponentResultID_ARS(RESULT_ID value);//設定零件結果編號-ARS
	//---------------------------------------------------------------------------------//		
	RESULT_ID                  GetComponentResultID_ARS_LA() const;//取得零件結果編號-ARS-A軌
	void                       SetComponentResultID_ARS_LA(RESULT_ID value);//設定零件結果編號-ARS-A軌
	//---------------------------------------------------------------------------------//		
	RESULT_ID                  GetComponentResultID_ARS_LB() const;//取得零件結果編號-ARS-B軌
	void                       SetComponentResultID_ARS_LB(RESULT_ID value);//設定零件結果編號-ARS-B軌
	//---------------------------------------------------------------------------------//		
	void                       UpdateComponentResultID_ARS_Lane(LANE_ID LaneID);//更新零件結果編號-ARS-軌道
	RESULT_ID                  GetComponentResultID_ARS_Lane(LANE_ID LaneID) const;//取得零件結果編號-ARS-軌道	
	//---------------------------------------------------------------------------------//		
	RESULT_ID                  GetComponentResultID_Alarm() const;//取得零件結果編號-停機
	void                       SetComponentResultID_Alarm(RESULT_ID value);//設定零件結果編號-停機
	//---------------------------------------------------------------------------------//	
	//零件保留影像
	void                       SetComponentKeepImage(bool value) { m_RgnKeepImage = value; }
	bool	                   GetComponentKeepImage() const { return m_RgnKeepImage; }
	//---------------------------------------------------------------------------------//
	//零件填滿畫面的時間
	void                       SetComponentFillImageTime(double value) { m_RgnFillImageTime = value; }
	double                     GetComponentFillImageTime() const { return m_RgnFillImageTime; }
	//---------------------------------------------------------------------------------//
	void                       RestoreComponentCadPos(CMapCoordinate *MapPtr);//恢復零件Cad座標
	//---------------------------------------------------------------------------------//
	void                       SetComponentOrgCadPos(const TPOINT2D &Pos) { m_ComponentOrgCadPos = Pos; }
	const TPOINT2D&            GetComponentOrgCadPos() const { return m_ComponentOrgCadPos; }
	//---------------------------------------------------------------------------------//	
	void                       SetComponentOrgCadPosX(double value) { m_ComponentOrgCadPos.x = value; }
	double                     GetComponentOrgCadPosX() const { return m_ComponentOrgCadPos.x; }
	//---------------------------------------------------------------------------------//	
	void                       SetComponentOrgCadPosY(double value) { m_ComponentOrgCadPos.y = value; }
	double                     GetComponentOrgCadPosY() const { return m_ComponentOrgCadPos.y; }
	//---------------------------------------------------------------------------------//
	void                       SetComponentSpecialCadPos(const TPOINT2D &Pos) { m_RgnSpecialCadPos = Pos; }
	const TPOINT2D&            GetComponentSpecialCadPos() const { return m_RgnSpecialCadPos; }
	//---------------------------------------------------------------------------------//	
	void                       SetComponentSpecialCadPosX(double value) { m_RgnSpecialCadPos.x = value; }
	double                     GetComponentSpecialCadPosX() const { return m_RgnSpecialCadPos.x; }
	//---------------------------------------------------------------------------------//	
	void                       SetComponentSpecialCadPosY(double value) { m_RgnSpecialCadPos.y = value; }
	double                     GetComponentSpecialCadPosY() const { return m_RgnSpecialCadPos.y; }
	//---------------------------------------------------------------------------------//
	void                       SetComponentName(const char* value);
	void                       SetComponentName(const wchar_t* value);
	const wchar_t*             GetComponentName() const { return m_ComponentName.c_str(); }
	CString                    GetComponentFullName() const;//取得零件全名	
	CString                    GetComponentShowName() const;//取得零件顯示名
	void                       ChangeComponentName(LPCTSTR Name);//變更零件名稱
	//---------------------------------------------------------------------------------//
	void                       SetComponentModelName(const char* value);
	void                       SetComponentModelName(const wchar_t* value);
	const wchar_t*             GetComponentModelName() const { return m_ComponentModelName.c_str(); }	
	//---------------------------------------------------------------------------------//
	void                       SetComponentPartNumber(const char* value);
	void                       SetComponentPartNumber(const wchar_t* value);
	const wchar_t*             GetComponentPartNumber() const { return m_ComponentPartNumber.c_str(); }
	//---------------------------------------------------------------------------------//
	void                       SetComponentNozzleName(const char* value);
	void                       SetComponentNozzleName(const wchar_t* value);
	const wchar_t*             GetComponentNozzleName() const { return m_ComponentNozzleName.c_str(); }
	//---------------------------------------------------------------------------------//
	//零件儲存報告-維修站
	void                       SetComponentSaveReport_ARS(bool val) { m_ComponentSaveReport_ARS = val; }
	bool                       GetComponentSaveReport_ARS() const { return m_ComponentSaveReport_ARS; }
	//---------------------------------------------------------------------------------//	
	void                       SetComponentTempText(const char* value);
	void                       SetComponentTempText(const wchar_t* value);	
	const wchar_t*             GetComponentTempText() const { return m_ComponentTempText.c_str(); }
	//---------------------------------------------------------------------------------//	
	void                       SetComponentTempIndex(size_t index) { m_ComponentTempIndex = index; }
	size_t                     GetComponentTempIndex() const { return m_ComponentTempIndex; }
	//---------------------------------------------------------------------------------//	
	void                       SetComponentTempInt(int val, int idx=0) { m_ComponentTempInt[idx]=val;  }
	int                        GetComponentTempInt(int idx=0) const { return m_ComponentTempInt[idx]; }
	//---------------------------------------------------------------------------------//
	void                       SetComponentTempInt_01(int val) { m_ComponentTempInt[1]=val;  }
	int                        GetComponentTempInt_01() const { return m_ComponentTempInt[1]; }
	//---------------------------------------------------------------------------------//
	void                       SetComponentTempInt_02(int val) { m_ComponentTempInt[2]=val;  }
	int                        GetComponentTempInt_02() const { return m_ComponentTempInt[2]; }
	//---------------------------------------------------------------------------------//
	void                       SetComponentTempInt_03(int val) { m_ComponentTempInt[3]=val;  }
	int                        GetComponentTempInt_03() const { return m_ComponentTempInt[3]; }
	//---------------------------------------------------------------------------------//
	void                       SetComponentTempIndexModel(size_t index) { m_ComponentTempIndexModel = index; }
	size_t                     GetComponentTempIndexModel() const { return m_ComponentTempIndexModel; }
	//---------------------------------------------------------------------------------//	
	void                       SetComponentTempIndexPartNumber(size_t index) { m_ComponentTempIndexPartNumber = index; }
	size_t                     GetComponentTempIndexPartNumber() const { return m_ComponentTempIndexPartNumber; }
	//---------------------------------------------------------------------------------//
	//零件模組編號, 前十大不良
	void                       SetComponentTop10IndexModel(size_t index) { m_ComponentTop10IndexModel = index; }
	size_t                     GetComponentTop10IndexModel() const { return m_ComponentTop10IndexModel; }
	//---------------------------------------------------------------------------------//	
	size_t                     GetComponentTop10IndexModel_Lane(LANE_ID LaneID) const;
	void                       SetComponentTop10IndexModel_Lane(LANE_ID LaneID, size_t index);	
	//---------------------------------------------------------------------------------//	
	void                       SetComponentTop10IndexModel_LA(size_t index) { m_ComponentTop10IndexModel_LA = index; }
	size_t                     GetComponentTop10IndexModel_LA() const { return m_ComponentTop10IndexModel_LA; }
	//---------------------------------------------------------------------------------//	
	void                       SetComponentTop10IndexModel_LB(size_t index) { m_ComponentTop10IndexModel_LB = index; }
	size_t                     GetComponentTop10IndexModel_LB() const { return m_ComponentTop10IndexModel_LB; }
	//---------------------------------------------------------------------------------//	
	//零件料號編號, 前十大不良
	void                       SetComponentTop10IndexPartNumber(size_t index) { m_ComponentTop10IndexPartNumber = index; }
	size_t                     GetComponentTop10IndexPartNumber() const { return m_ComponentTop10IndexPartNumber; }
	//---------------------------------------------------------------------------------//	
	size_t                     GetComponentTop10IndexPartNumber_Lane(LANE_ID LaneID) const;
	void                       SetComponentTop10IndexPartNumber_Lane(LANE_ID LaneID, size_t index);	
	//---------------------------------------------------------------------------------//	
	void                       SetComponentTop10IndexPartNumber_LA(size_t index) { m_ComponentTop10IndexPartNumber_LA = index; }
	size_t                     GetComponentTop10IndexPartNumber_LA() const { return m_ComponentTop10IndexPartNumber_LA; }
	//---------------------------------------------------------------------------------//	
	void                       SetComponentTop10IndexPartNumber_LB(size_t index) { m_ComponentTop10IndexPartNumber_LB = index; }
	size_t                     GetComponentTop10IndexPartNumber_LB() const { return m_ComponentTop10IndexPartNumber_LB; }
	//---------------------------------------------------------------------------------//
	bool                       AddComponentToTop10ModeList(std::vector<TTop10Node> &Top10List, bool DefectAOI);
	bool                       AddComponentToTop10ModeList_Lane(LANE_ID LaneID, std::vector<TTop10Node> &Top10List, bool DefectAOI);
	bool                       AddComponentToTop10PartNumberList(std::vector<TTop10Node> &Top10List, bool DefectAOI);
	bool                       AddComponentToTop10PartNumberList_Lane(LANE_ID LaneID, std::vector<TTop10Node> &Top10List, bool DefectAOI);
	//---------------------------------------------------------------------------------//
	CAOIRgn*                   GetComponentRegionPtr() { return this; }
	//---------------------------------------------------------------------------------//
	size_t                     GetComponentWindowCount() const;
	CAOIWindow*                GetComponentWindowPtr(size_t index, bool check);
	bool                       AddComponentWindow(CAOIWindow &Window);//增加零件的檢測框
	bool                       SelectComponentAllWindows(bool Select);//選取零件的檢測框
	bool                       DeleteComponentWindowSelected();//移除選取到的零件的檢測框
	bool                       DeleteComponentAllWindows();//移除零件的檢測框
	bool                       LayoutComponentWindowList();//重整零件的檢測框列表
	//---------------------------------------------------------------------------------//	
	//影像序號
	void                       SetComponentFrameIndex(unsigned int value) { SetRgnFrameIndex(value); }
	unsigned int               GetComponentFrameIndex() const { return GetRgnFrameIndex(); }
	//---------------------------------------------------------------------------------//
	void                       SetComponentFrameUniqueID(unsigned int value) { SetRgnFrameUniqueID(value); }
	unsigned int               GetComponentFrameUniqueID() const { return GetRgnFrameUniqueID(); }
	//---------------------------------------------------------------------------------//
	//相機編號
	void                       SetComponentCameraID(CAMERA_ID value) { m_RgnCameraID = value; }
	CAMERA_ID                  GetComponentCameraID() const { return m_RgnCameraID; }
	//---------------------------------------------------------------------------------//
	//燈源模式
	void                       SetComponentLightMode(LIGHT_MODE value) { m_RgnLightMode = value; }
	LIGHT_MODE                 GetComponentLightMode() const { return m_RgnLightMode; }
	//---------------------------------------------------------------------------------//
	//分段編號//兩段式檢測
	void                       SetComponentDistrictID(DISTRICT_ID value) { m_RgnDistrictID = value; }
	DISTRICT_ID                GetComponentDistrictID() const { return m_RgnDistrictID; }
	//---------------------------------------------------------------------------------//
	//要去計算
	void                       SetComponentNeedToCalculate(bool value) { CAOIRgn::SetRgnNeedToCalculate(value); }
	bool                       GetComponentNeedToCalculate() const { return m_RgnNeedToCalculate; }
	//---------------------------------------------------------------------------------//
	//要去計算-備份檔
	void                       SetComponentNeedToCalculateBackup(bool value) { CAOIRgn::SetRgnNeedToCalculateBackup(value); }
	bool                       GetComponentNeedToCalculateBackup() const { return m_RgnNeedToCalculateBackup; }
	//---------------------------------------------------------------------------------//
	//零件角度
	void                       SetComponentAngle(double value) { m_RgnAngle = value; }
	double                     GetComponentAngle() const { return m_RgnAngle; }
	//---------------------------------------------------------------------------------//
	//零件尺寸寬
	void                       SetComponentRoiSizeW(double value) { m_RgnRoiSize.cx = value; }
	double                     GetComponentRoiSizeW() const { return m_RgnRoiSize.cx; }
	//---------------------------------------------------------------------------------//
	//零件尺寸長
	void                       SetComponentRoiSizeH(double value) { m_RgnRoiSize.cy = value; }
	double                     GetComponentRoiSizeH() const { return m_RgnRoiSize.cy; }
	//---------------------------------------------------------------------------------//
	//零件本體尺寸寬
	void                       SetComponentBodySizeW(double value) { m_RgnBodySize.cx = value; }
	double                     GetComponentBodySizeW() const { return m_RgnBodySize.cx; }
	//---------------------------------------------------------------------------------//
	//零件本體尺寸長
	void                       SetComponentBodySizeH(double value) { m_RgnBodySize.cy = value; }
	double                     GetComponentBodySizeH() const { return m_RgnBodySize.cy; }
	//---------------------------------------------------------------------------------//
	//零件位置在CAD坐標系
	void                       SetComponentCadPos(const TPOINT2D &value) { m_RgnCadPos = value; }
	TPOINT2D                   GetComponentCadPos() const { return m_RgnCadPos; }
	//---------------------------------------------------------------------------------//
	//零件位置在CAD坐標系中-X
	void                       SetComponentCadPosX(double value) { m_RgnCadPos.x = value; }
	double                     GetComponentCadPosX() const { return m_RgnCadPos.x; }
	//---------------------------------------------------------------------------------//
	//零件位置在CAD坐標系中-Y
	void                       SetComponentCadPosY(double value) { m_RgnCadPos.y = value; }
	double                     GetComponentCadPosY() const { return m_RgnCadPos.y; }
	//---------------------------------------------------------------------------------//	
	//零件偏心差在CAD坐標系
	void                       SetComponentCadBiasPos(const TPOINT2D &Pos) { m_RgnCadBiasPos = Pos; }
	TPOINT2D                   GetComponentCadBiasPos() const { return m_RgnCadBiasPos; }
	//---------------------------------------------------------------------------------//	
	//零件偏心差在CAD坐標系-X
	void                       SetComponentCadBiasPosX(double value) { m_RgnCadBiasPos.x = value; }
	double                     GetComponentCadBiasPosX() const { return m_RgnCadBiasPos.x; }
	//---------------------------------------------------------------------------------//
	//零件偏心差在CAD坐標系-Y
	void                       SetComponentCadBiasPosY(double value) { m_RgnCadBiasPos.y = value; }
	double                     GetComponentCadBiasPosY() const { return m_RgnCadBiasPos.y; }
	//---------------------------------------------------------------------------------//
	//零件四端點X
	void                       SetComponentCadCornerPosX(const double value[]) 
	{ 
		m_RgnRoiCadCornerPos[0].x = value[0]; 
		m_RgnRoiCadCornerPos[1].x = value[1]; 
		m_RgnRoiCadCornerPos[2].x = value[2]; 
		m_RgnRoiCadCornerPos[3].x = value[3]; 
	}
	void                      GetComponentCadCornerPosX(double value[]) const 
	{ 
		value[0] = m_RgnRoiCadCornerPos[0].x; 
		value[1] = m_RgnRoiCadCornerPos[1].x; 
		value[2] = m_RgnRoiCadCornerPos[2].x; 
		value[3] = m_RgnRoiCadCornerPos[3].x; 
	}
	//---------------------------------------------------------------------------------//
	//零件四端點Y
	void                       SetComponentCadCornerPosY(const double value[]) 
	{ 
		m_RgnRoiCadCornerPos[0].y = value[0]; 
		m_RgnRoiCadCornerPos[1].y = value[1]; 
		m_RgnRoiCadCornerPos[2].y = value[2]; 
		m_RgnRoiCadCornerPos[3].y = value[3]; 
	}
	void                       GetComponentCadCornerPosY(double value[]) const 
	{ 
		value[0] = m_RgnRoiCadCornerPos[0].y; 
		value[1] = m_RgnRoiCadCornerPos[1].y; 
		value[2] = m_RgnRoiCadCornerPos[2].y; 
		value[3] = m_RgnRoiCadCornerPos[3].y; 
	}
	//---------------------------------------------------------------------------------//	
	//零件位置在CAD零件位置在機台坐標系中
	void                       SetComponentStagePos(const TPOINT3D &value) { m_RgnStagePos = value; }
	TPOINT3D                   GetComponentStagePos() const { return m_RgnStagePos; }
	//---------------------------------------------------------------------------------//
	//零件位置在機台坐標系中-X
	void                       SetComponentStagePosX(double value) { m_RgnStagePos.x = value; }
	double                     GetComponentStagePosX() const { return m_RgnStagePos.x; }
	//---------------------------------------------------------------------------------//
	//零件位置在機台坐標系中-Y
	void                       SetComponentStagePosY(double value) { m_RgnStagePos.y = value; }
	double                     GetComponentStagePosY() const { return m_RgnStagePos.y; }
	//---------------------------------------------------------------------------------//	
	//零件位置在機台坐標系中-Z
	void                       SetComponentStagePosZ(double value) { m_RgnStagePos.z = value; }
	double                     GetComponentStagePosZ() const { return m_RgnStagePos.z; }
	//---------------------------------------------------------------------------------//	
	void                       GetComponentBodyStageCornerPos(TPOINT2D value[]) const 
	{
		value[0].x = m_RgnBodyStageCornerPos[0].x;	value[0].y = m_RgnBodyStageCornerPos[0].y; 	
		value[1].x = m_RgnBodyStageCornerPos[1].x;	value[1].y = m_RgnBodyStageCornerPos[1].y; 	
		value[2].x = m_RgnBodyStageCornerPos[2].x;	value[2].y = m_RgnBodyStageCornerPos[2].y; 	
		value[3].x = m_RgnBodyStageCornerPos[3].x;	value[3].y = m_RgnBodyStageCornerPos[3].y; 	
	}
	//---------------------------------------------------------------------------------//	
	void                       GetComponentRoiStageCornerPos(TPOINT2D value[]) const 
	{
		value[0].x = m_RgnRoiStageCornerPos[0].x;	value[0].y = m_RgnRoiStageCornerPos[0].y; 	
		value[1].x = m_RgnRoiStageCornerPos[1].x;	value[1].y = m_RgnRoiStageCornerPos[1].y; 	
		value[2].x = m_RgnRoiStageCornerPos[2].x;	value[2].y = m_RgnRoiStageCornerPos[2].y; 	
		value[3].x = m_RgnRoiStageCornerPos[3].x;	value[3].y = m_RgnRoiStageCornerPos[3].y; 	
	}
	//---------------------------------------------------------------------------------//		
	//零件四端點在機台坐標系中-X
	void                       SetComponentRoiStageCornerPosX(double value[]) 
	{ 
		m_RgnRoiStageCornerPos[0].x = value[0]; 
		m_RgnRoiStageCornerPos[1].x = value[1]; 
		m_RgnRoiStageCornerPos[2].x = value[2]; 
		m_RgnRoiStageCornerPos[3].x = value[3]; 
	}
	void                       GetComponentRoiStageCornerPosX(double value[]) const 
	{ 
		value[0] = m_RgnRoiStageCornerPos[0].x; 
		value[1] = m_RgnRoiStageCornerPos[1].x; 
		value[2] = m_RgnRoiStageCornerPos[2].x; 
		value[3] = m_RgnRoiStageCornerPos[3].x; 
	}	
	//---------------------------------------------------------------------------------//
	//零件四端點在機台坐標系中-Y
	void                       SetComponentRoiStageCornerPosY(double value[]) 
	{ 
		m_RgnRoiStageCornerPos[0].y = value[0]; 
		m_RgnRoiStageCornerPos[1].y = value[1]; 
		m_RgnRoiStageCornerPos[2].y = value[2]; 
		m_RgnRoiStageCornerPos[3].y = value[3]; 
	}
	void                       GetComponentRoiStageCornerPosY(double value[]) const 
	{ 
		value[0] = m_RgnRoiStageCornerPos[0].y; 
		value[1] = m_RgnRoiStageCornerPos[1].y; 
		value[2] = m_RgnRoiStageCornerPos[2].y; 
		value[3] = m_RgnRoiStageCornerPos[3].y; 
	}
	//---------------------------------------------------------------------------------//
	void                       MoveComponentCadPos(double dX, double dY);//移動零件座標	
	void                       MoveComponentStagePos(double dX, double dY);//移動零件座標	
	void                       MoveComponentOrgCadPos(double dX, double dY);//移動零件原始座標	
	void                       MoveComponentPos(double dX, double dY, CMapCoordinate *MapPtr, bool MoveOrg=true);//移動零件座標		
	//---------------------------------------------------------------------------------//
	void                       GetComponentRoiCadRegion(TREGION4D &Region);//取得零件在Cad的範圍	
	void                       GetComponentBodyCadRegion(TREGION4D &Region);//取得零件在Cad的範圍	
	void                       GetComponentRoiStageRegion(TREGION4D &Region) const;//取得零件在Stage的範圍	
	void                       GetComponentBodyStageRegion(TREGION4D &Region) const;//取得零件本體在Stage的範圍	
	//---------------------------------------------------------------------------------//
	void                       CalcComponentCadCornerPos();//計算零件Cad端點座標		
	void                       LayoutComponentStageCornerPos();//更新零件機台端點座標	
	//---------------------------------------------------------------------------------//
	//零件所屬FOV的CAD位置-X
	void                       SetComponentFovCadPosX(double value) { m_RgnFovCadPos.x = value; }
	double                     GetComponentFovCadPosX() const { return m_RgnFovCadPos.y; }
	//---------------------------------------------------------------------------------//
	//零件所屬FOV的CAD位置-Y
	void                       SetComponentFovCadPosY(double value) { m_RgnFovCadPos.y = value; }
	double                     GetComponentFovCadPosY() const { return m_RgnFovCadPos.y; }
	//---------------------------------------------------------------------------------//
	//零件所屬FOV的Stage位置-X
	void                       SetComponentFovStagePosX(double value) { m_RgnFovStagePos.x = value; }
	double                     GetComponentFovStagePosX() const { return m_RgnFovStagePos.x; }
	//---------------------------------------------------------------------------------//
	//零件所屬FOV的Stage位置-Y
	void                       SetComponentFovStagePosY(double value) { m_RgnFovStagePos.y = value; }
	double                     GetComponentFovStagePosY() const { return m_RgnFovStagePos.y; }
	//---------------------------------------------------------------------------------//
	//零件所屬影像的區域
	void                       SetComponentFrameImageRect(const RECT &value) { m_RgnFrameImageRect = value; }
	const RECT&                GetComponentFrameImageRect() const { return m_RgnFrameImageRect; }
	//---------------------------------------------------------------------------------//	
	//零件影像的物理尺寸
	void                       SetComponentFrameImageSize_um(const TSIZE2D &value);
	const TSIZE2D&             GetComponentFrameImageSize_um() const { return m_RgnFrameImageSize_um; }
	//---------------------------------------------------------------------------------//	
	//零件影像的區域Cad偏差-um
	void                       SetComponentFrameImageCadOffset_um(const TPOINT2D &value);	
	const TPOINT2D&            GetComponentFrameImageCadOffset_um() const { return m_RgnFrameImageCadOffset_um; }
	void                       SetComponentFrameImageStageOffset_um(const TPOINT2D &value);
	//---------------------------------------------------------------------------------//	
	void                       SetComponentFrameImageCornerPt(const POINT value[]) 
	{ 
		m_RgnFrameImageCornerPt[0] = value[0]; 
		m_RgnFrameImageCornerPt[1] = value[1]; 
		m_RgnFrameImageCornerPt[2] = value[2]; 
		m_RgnFrameImageCornerPt[3] = value[3]; 
	}
	void                       GetComponentFrameImageCornerPt(POINT value[]) const 
	{ 
		value[0] = m_RgnFrameImageCornerPt[0]; 
		value[1] = m_RgnFrameImageCornerPt[1]; 
		value[2] = m_RgnFrameImageCornerPt[2]; 
		value[3] = m_RgnFrameImageCornerPt[3]; 
	}
	//---------------------------------------------------------------------------------//	
	//零件所屬的Field的CAD位置-X
	void                       SetComponentFieldCadPosX(double value) { m_RgnFieldCadPos.x = value; }
	double                     GetComponentFieldCadPosX() const { return m_RgnFieldCadPos.x; }
	//---------------------------------------------------------------------------------//
	//零件所屬的Field的CAD位置-Y
	void                       SetComponentFieldCadPosY(double value) { m_RgnFieldCadPos.y = value; }
	double                     GetComponentFieldCadPosY() const { return m_RgnFieldCadPos.y; }
	//---------------------------------------------------------------------------------//
	//零件所屬的Field的Stage位置-X
	void                       SetComponentFieldStagePosX(double value) { m_RgnFieldStagePos.x = value; }
	double                     GetComponentFieldStagePosX() const { return m_RgnFieldStagePos.x; }
	//---------------------------------------------------------------------------------//
	//零件所屬的Field的Stage位置-Y
	void                       SetComponentFieldStagePosY(double value) { m_RgnFieldStagePos.y = value; }
	double                     GetComponentFieldStagePosY() const { return m_RgnFieldStagePos.y; }
	//---------------------------------------------------------------------------------//
	void                       MapComponentCadToStagePos(const CMapCoordinate &Map);//將零件CAD轉成機台座標
	void                       MapComponentStageResultToCadPos(const CMapCoordinate &Map);//將零件機台結果座標轉成Cad座標
	//---------------------------------------------------------------------------------//
	bool                       SpinComponent(double Angle, CMapCoordinate *MapPtr);//零件自旋轉	
	bool                       RotateComponent(double Angle, double CpX, double CpY, CMapCoordinate *MapPtr);//零件旋轉		
	bool                       MirrorXComponent(double CpX, CMapCoordinate *MapPtr);//零件鏡射-X	
	bool                       MirrorYComponent(double CpY, CMapCoordinate *MapPtr);//零件鏡射-Y	
	//---------------------------------------------------------------------------------//
	virtual CString            GetRgnDerivedName() const;//取得零件的名稱	
	virtual CString            GetRgnDerivedKeyName() const;//取得零件的名稱	
	virtual bool               ExtractRgnDerivedFrame(bool &Finished);//挖取零件圖片
	virtual bool               ExecRgnDerivedInspection();//執行零件檢測	
	//---------------------------------------------------------------------------------//
	bool                       CreateComponentSubRgnList(FIELD_BUILD_MODE BuildMode, bool ByCadRegion);//建立零件子檢測區域列表
	void                       ClearComponentSubRgnList();//清除零件的子列表
	size_t                     GetComponentSubRgnCount() const;//取得零件的子數量
	CAOIRgn*                   GetComponentSubRgnPtr(size_t index, bool check) const;//取得零件的子指標		
	//---------------------------------------------------------------------------------//
	bool                       CheckComponentBePickByCad(const TPOINT2D &PickPos, bool RestMode);//確認零件被點擊到
	bool                       CheckComponentBePickByStage(const TPOINT2D &PickPos, bool RestMode);//確認零件被點擊到

	bool                       CheckComponentInRegionByCad(const TREGION4D &SelRgn, bool RestMode, bool bEntireIn);//確認零件在範圍內
	bool                       CheckComponentInRegionByStage(const TREGION4D &SelRgn, bool RestMode, bool bEntireIn);//確認零件在範圍內
	//---------------------------------------------------------------------------------//
	void                       GetComponentWindowCadRegion(TREGION4D &Region);//取得零件與檢測框在Cad的範圍	
	void                       GetComponentWindowStageRegion(TREGION4D &Region);//取得零件與檢測框在Stage的範圍	
	//---------------------------------------------------------------------------------//
	void                       ResetComponentModel();//復歸零件模組
	CAOIModel*                 GetComponentModelPtr();//取得零件模組指標
	bool                       CheckComponentModelEnabled() const;//確認零件模組啟用中
	void                       SetComponentModelIndex(unsigned int value);	
	unsigned int               GetComponentModelIndex() const { return m_ComponentModelIndex; }		
	bool                       UpdateComponentResultID();//更新零件結果編號
	bool                       UpdateComponentDefectCount();//更新零件瑕疵數量
	bool                       UpdateComponentResultValue();//更新零件結果數值
	bool                       UpdateComponentResultValue_V1();//更新零件結果數值
	bool                       UpdateComponentResultValue_V2();//更新零件結果數值	
	bool                       UpdateComponentGrrValue(double OffsetX, double OffsetY, double Skew, double BodyHeight, bool &Added);//更新零件GRR數值
	bool                       SetComponentGrrTempValue(double OffsetX, double OffsetY, double Skew, double BodyHeight);//設定零件GRR暫存數值
	bool                       GetComponentGrrTempValue(double &OffsetX, double &OffsetY, double &Skew, double &BodyHeight) const;//取回零件GRR暫存數值
	bool                       UpdateComponentResultStagePos();//更新零件機台座標結果
	bool                       UpdateComponentParamToModel(bool Resize);//更新零件參數至模組內
	bool                       SyncComponentModelFromLibrary(CAOIModel *ModelPtr, bool bPartial);//由資料庫模組同步化至更新零件模組
	bool                       UpdateComponentModelFromLibrary(CAOIModel *ModelPtr);//由資料庫模組套用至更新零件模組	
	CAOIWnd*                   GetComponentModelWndPtrByDefectID(WND_DEFECT_ID WndDefectID);//依照檢測框瑕疵編號找到檢測框
	bool                       UpdateComponentModelWndBypassed(const CWndDefectItem &DefectEnabled);//更新模組檢測框是否忽略	
	//---------------------------------------------------------------------------------//	
	int                        GetComponentModelClassID() const;//取得零件指定模組類別編號
	void                       SetComponentModelClassID(int value);//設定零件指定模組類別編號
	//---------------------------------------------------------------------------------//
	bool                       GetComponentModelIsolated() const;//取得零件模組是否隔離	
	void                       SetComponentModelIsolated(bool value);//設定零件模組是否隔離	
	//---------------------------------------------------------------------------------//	
	bool                       GetComponentSaveWndList() const;//取得零件儲存檢測框列表
	void                       SetComponentSaveWndList(bool value);//設定零件儲存檢測框列表
	//---------------------------------------------------------------------------------//
	bool                       GetComponentModelImageIsSaved() const;//取得零件模組是否存圖
	void                       SetComponentModelImageIsSaved(bool value);//設定零件模組是否存圖	
	//---------------------------------------------------------------------------------//	
	SAVE_TEST_IMAGE_MODE       GetComponentSaveTestImageMode() const;//取得零件儲存影像模式
	void                       SetComponentSaveTestImageMode(SAVE_TEST_IMAGE_MODE val);//設定零件儲存影像模式	
	//---------------------------------------------------------------------------------//		
	bool                       GetComponentModelImageIsSaved_AI() const;//取得零件模組是否存圖-AI
	void                       SetComponentModelImageIsSaved_AI(bool value);//設定零件模組是否存圖-AI
	//---------------------------------------------------------------------------------//	
	bool                       GetComponentOffsetText(CString &Text) const;
	bool                       GetComponentOffsetText(TCHAR TextBuffer[]) const;
	bool                       GetComponentLocationText(CString &Text) const;
	bool                       GetComponentLocationText(TCHAR TextBuffer[]) const;
	//---------------------------------------------------------------------------------//	
	void                       SetComponentResultWidth(double val) { m_ComponentResultWidth = val; }
	double                     GetComponentResultWidth() const { return m_ComponentResultWidth; }
	void                       SetComponentResultLength(double val) { m_ComponentResultLength = val; }
	double                     GetComponentResultLength() const { return m_ComponentResultLength; }	
	void                       SetComponentResultHeight(double val) { m_ComponentResultHeight = val; }
	double                     GetComponentResultHeight() const { return m_ComponentResultHeight; }
	void                       SetComponentResultArea(double val) { m_ComponentResultArea = val; }
	double                     GetComponentResultArea() const { return m_ComponentResultArea; }
	void                       SetComponentResultVolume(double val) { m_ComponentResultVolume = val; }
	double                     GetComponentResultVolume() const { return m_ComponentResultVolume; }
	void                       SetComponentResultHeightMin(double val) { m_ComponentResultHeightMin = val; }
	double                     GetComponentResultHeightMin() const { return m_ComponentResultHeightMin; }
	void                       SetComponentResultHeightMax(double val) { m_ComponentResultHeightMax = val; }
	double                     GetComponentResultHeightMax() const { return m_ComponentResultHeightMax; }	
	//---------------------------------------------------------------------------------//	
	//相對焊盤的偏移量-CAD
	void                       SetComponentResultOffsetX(double val) { m_ComponentResultOffsetX = val; }
	double                     GetComponentResultOffsetX() const { return m_ComponentResultOffsetX; }//零件偏移量-X
	void                       SetComponentResultOffsetY(double val) { m_ComponentResultOffsetY = val; }
	double                     GetComponentResultOffsetY() const { return m_ComponentResultOffsetY; }//零件偏移量-Y
	void                       SetComponentResultOffsetA(double val) { m_ComponentResultOffsetA = val; }
	double                     GetComponentResultOffsetA() const { return m_ComponentResultOffsetA; }//零件偏移量-角	
	void                       SetComponentResultSkewAngle(double val) { m_ComponentResultSkewAngle = val; }
	double                     GetComponentResultSkewAngle() const { return m_ComponentResultSkewAngle; }//零件偏移角度
	void                       SetComponentResultTiltAngle(double val) { m_ComponentResultTiltAngle = val; }
	double                     GetComponentResultTiltAngle() const { return m_ComponentResultTiltAngle; }//零件傾斜角度	
	//---------------------------------------------------------------------------------//
	void                       SetComponentResultWidth_USL(double val) { m_ComponentResultWidth_USL = val; }
	double                     GetComponentResultWidth_USL() const { return m_ComponentResultWidth_USL; }//結果寬度-um-上限
	void                       SetComponentResultWidth_LSL(double val) { m_ComponentResultWidth_LSL = val; }
	double                     GetComponentResultWidth_LSL() const { return m_ComponentResultWidth_LSL; }//結果寬度-um-下限
	void                       SetComponentResultWidth_WndIdx(unsigned int val) { m_ComponentResultWidth_WndIdx = val; }
	unsigned int               GetComponentResultWidth_WndIdx() const { return m_ComponentResultWidth_WndIdx; }//結果寬度-檢測框引數
	//---------------------------------------------------------------------------------//
	void                       SetComponentResultLength_USL(double val) { m_ComponentResultLength_USL = val; }
	double                     GetComponentResultLength_USL() const { return m_ComponentResultLength_USL; }//結果長度-um-上限
	void                       SetComponentResultLength_LSL(double val) { m_ComponentResultLength_LSL = val; }
	double                     GetComponentResultLength_LSL() const { return m_ComponentResultLength_LSL; }//結果長度-um-下限
	void                       SetComponentResultLength_WndIdx(unsigned int val) { m_ComponentResultLength_WndIdx = val; }
	unsigned int               GetComponentResultLength_WndIdx() const { return m_ComponentResultLength_WndIdx; }//結果長度-檢測框引數
	//---------------------------------------------------------------------------------//
	void                       SetComponentResultHeight_USL(double val) { m_ComponentResultHeight_USL = val; }
	double                     GetComponentResultHeight_USL() const { return m_ComponentResultHeight_USL; }//結果高度-um-上限
	void                       SetComponentResultHeight_LSL(double val) { m_ComponentResultHeight_LSL = val; }
	double                     GetComponentResultHeight_LSL() const { return m_ComponentResultHeight_LSL; }//結果高度-um-下限
	void                       SetComponentResultHeight_WndIdx(unsigned int val) { m_ComponentResultHeight_WndIdx = val; }
	unsigned int               GetComponentResultHeight_WndIdx() const { return m_ComponentResultHeight_WndIdx; }//結果高度-檢測框引數
	//---------------------------------------------------------------------------------//
	void                       SetComponentResultArea_USL(double val) { m_ComponentResultArea_USL = val; }
	double                     GetComponentResultArea_USL() const { return m_ComponentResultArea_USL; }//結果面積-um^2-上限
	void                       SetComponentResultArea_LSL(double val) { m_ComponentResultArea_LSL = val; }
	double                     GetComponentResultArea_LSL() const { return m_ComponentResultArea_LSL; }//結果面積-um^2-下限
	void                       SetComponentResultArea_WndIdx(unsigned int val) { m_ComponentResultArea_WndIdx = val; }
	unsigned int               GetComponentResultArea_WndIdx() const { return m_ComponentResultArea_WndIdx; }//結果面積-檢測框引數
	//---------------------------------------------------------------------------------//
	void                       SetComponentResultVolume_USL(double val) { m_ComponentResultVolume_USL = val; }
	double                     GetComponentResultVolume_USL() const { return m_ComponentResultVolume_USL; }//結果體積-um^3-上限
	void                       SetComponentResultVolume_LSL(double val) { m_ComponentResultVolume_LSL = val; }
	double                     GetComponentResultVolume_LSL() const { return m_ComponentResultVolume_LSL; }//結果體積-um^3-下限
	void                       SetComponentResultVolume_WndIdx(unsigned int val) { m_ComponentResultVolume_WndIdx = val; }
	unsigned int               GetComponentResultVolume_WndIdx() const { return m_ComponentResultVolume_WndIdx; }//結果體積-檢測框引數
	//---------------------------------------------------------------------------------//
	void                       SetComponentResultOffsetX_USL(double val) { m_ComponentResultOffsetX_USL = val; }
	double                     GetComponentResultOffsetX_USL() const { return m_ComponentResultOffsetX_USL; }//結果偏移量-X-上限
	void                       SetComponentResultOffsetX_LSL(double val) { m_ComponentResultOffsetX_LSL = val; }
	double                     GetComponentResultOffsetX_LSL() const { return m_ComponentResultOffsetX_LSL; }//結果偏移量-X-下限
	void                       SetComponentResultOffsetX_WndIdx(unsigned int val) { m_ComponentResultOffsetX_WndIdx = val; }
	unsigned int               GetComponentResultOffsetX_WndIdx() const { return m_ComponentResultOffsetX_WndIdx; }//結果偏移量-X-檢測框引數
	//---------------------------------------------------------------------------------//
	void                       SetComponentResultOffsetY_USL(double val) { m_ComponentResultOffsetY_USL = val; }
	double                     GetComponentResultOffsetY_USL() const { return m_ComponentResultOffsetY_USL; }//結果偏移量-Y-上限
	void                       SetComponentResultOffsetY_LSL(double val) { m_ComponentResultOffsetY_LSL = val; }
	double                     GetComponentResultOffsetY_LSL() const { return m_ComponentResultOffsetY_LSL; }//結果偏移量-Y-下限
	void                       SetComponentResultOffsetY_WndIdx(unsigned int val) { m_ComponentResultOffsetY_WndIdx = val; }
	unsigned int               GetComponentResultOffsetY_WndIdx() const { return m_ComponentResultOffsetY_WndIdx; }//結果偏移量-Y-檢測框引數
	//---------------------------------------------------------------------------------//
	void                       SetComponentResultOffsetA_USL(double val) { m_ComponentResultOffsetA_USL = val; }
	double                     GetComponentResultOffsetA_USL() const { return m_ComponentResultOffsetA_USL; }//結果偏移量-角-上限
	void                       SetComponentResultOffsetA_LSL(double val) { m_ComponentResultOffsetA_LSL = val; }
	double                     GetComponentResultOffsetA_LSL() const { return m_ComponentResultOffsetA_LSL; }//結果偏移量-角-下限
	void                       SetComponentResultOffsetA_WndIdx(unsigned int val) { m_ComponentResultOffsetA_WndIdx = val; }
	unsigned int               GetComponentResultOffsetA_WndIdx() const { return m_ComponentResultOffsetA_WndIdx; }//結果偏移量-角-檢測框引數
	//---------------------------------------------------------------------------------//
	void                       SetComponentResultSkewAngle_USL(double val) { m_ComponentResultSkewAngle_USL = val; }
	double                     GetComponentResultSkewAngle_USL() const { return m_ComponentResultSkewAngle_USL; }//結果偏移角度-上限
	void                       SetComponentResultSkewAngle_LSL(double val) { m_ComponentResultSkewAngle_LSL = val; }
	double                     GetComponentResultSkewAngle_LSL() const { return m_ComponentResultSkewAngle_LSL; }//結果偏移角度-下限
	void                       SetComponentResultSkewAngle_WndIdx(unsigned int val) { m_ComponentResultSkewAngle_WndIdx = val; }
	unsigned int               GetComponentResultSkewAngle_WndIdx() const { return m_ComponentResultSkewAngle_WndIdx; }//結果偏移角度-檢測框引數
	//---------------------------------------------------------------------------------//
	void                       SetComponentResultTiltAngle_USL(double val) { m_ComponentResultTiltAngle_USL = val; }
	double                     GetComponentResultTiltAngle_USL() const { return m_ComponentResultTiltAngle_USL; }//結果傾斜角度-上限
	void                       SetComponentResultTiltAngle_LSL(double val) { m_ComponentResultTiltAngle_LSL = val; }
	double                     GetComponentResultTiltAngle_LSL() const { return m_ComponentResultTiltAngle_LSL; }//結果傾斜角度-下限
	void                       SetComponentResultTiltAngle_WndIdx(unsigned int val) { m_ComponentResultTiltAngle_WndIdx = val; }
	unsigned int               GetComponentResultTiltAngle_WndIdx() const { return m_ComponentResultTiltAngle_WndIdx; }//結果傾斜角度-檢測框引數
	//---------------------------------------------------------------------------------//		
	void                       UpdateComponentResultOffset_Lane(LANE_ID LaneID);//設定零件偏移	
	double                     GetComponentResultOffsetX_Lane(LANE_ID LaneID) const;//取得零件偏移
	double                     GetComponentResultOffsetY_Lane(LANE_ID LaneID) const;//取得零件偏移
	double                     GetComponentResultSkewAngle_Lane(LANE_ID LaneID) const;//取得零件偏移
	double                     GetComponentResultTiltAngle_Lane(LANE_ID LaneID) const;//取得零件傾斜
	//---------------------------------------------------------------------------------//
	void                       SetComponentResultOffsetX_LA(double val) { m_ComponentResultOffsetX_LA = val; }
	double                     GetComponentResultOffsetX_LA() const { return m_ComponentResultOffsetX_LA; }//零件偏移量-X
	void                       SetComponentResultOffsetY_LA(double val) { m_ComponentResultOffsetY_LA = val; }
	double                     GetComponentResultOffsetY_LA() const { return m_ComponentResultOffsetY_LA; }//零件偏移量-Y
	void                       SetComponentResultSkewAngle_LA(double val) { m_ComponentResultSkewAngle_LA = val; }
	double                     GetComponentResultSkewAngle_LA() const { return m_ComponentResultSkewAngle_LA; }//零件偏移角度
	void                       SetComponentResultTiltAngle_LA(double val) { m_ComponentResultTiltAngle_LA = val; }
	double                     GetComponentResultTiltAngle_LA() const { return m_ComponentResultTiltAngle_LA; }//零件傾斜角度	
	//---------------------------------------------------------------------------------//	
	void                       SetComponentResultOffsetX_LB(double val) { m_ComponentResultOffsetX_LB = val; }
	double                     GetComponentResultOffsetX_LB() const { return m_ComponentResultOffsetX_LB; }//零件偏移量-X
	void                       SetComponentResultOffsetY_LB(double val) { m_ComponentResultOffsetY_LB = val; }
	double                     GetComponentResultOffsetY_LB() const { return m_ComponentResultOffsetY_LB; }//零件偏移量-Y
	void                       SetComponentResultSkewAngle_LB(double val) { m_ComponentResultSkewAngle_LB = val; }
	double                     GetComponentResultSkewAngle_LB() const { return m_ComponentResultSkewAngle_LB; }//零件偏移角度
	void                       SetComponentResultTiltAngle_LB(double val) { m_ComponentResultTiltAngle_LB = val; }
	double                     GetComponentResultTiltAngle_LB() const { return m_ComponentResultTiltAngle_LB; }//零件傾斜角度	
	//---------------------------------------------------------------------------------//	
	int                        GetComponentGrrSigmaItemIdx() const;//取得零件GRR項目引數 
	void                       SetComponentGrrSigmaItemIdx(int val);//設定零件GRR項目引數 
	void                       ResetComponentGrrSigmaItem(const wchar_t *Barcode, bool bChkBarcode);//重設零件檢GRR項目 	
	const TSigmaItem&          GetComponentGrrSigmaItem() const;//零件檢GRR項目 	
	TGrrSigmaItem&             GetComponentGrrSigmaItemUsed();//零件檢GRR項目-使用中
	void                       AddComponentGrrSigmaItem();//加入零件GRR項目	
	const std::vector<TGrrSigmaItem>& GetComponentGrrSigmaItemList() const;//零件檢測GRR項目列表 
	void                       SetComponentGrrSigmaItemList(const std::vector<TGrrSigmaItem> &List);//零件檢測GRR項目列表 
	//---------------------------------------------------------------------------------//	
	void                       AddComponentResultSigmaItem_Lane(LANE_ID LaneID);//加入零件標準差項目
	void                       AddComponentResultSigmaItem(TSigmaItem &SigmaItem);//加入零件標準差項目
	const TSigmaItem&          GetComponentResultSigmaItem_Lane(LANE_ID LaneID) const;//零件檢測標準差項目 	
	//---------------------------------------------------------------------------------//
	//Cad結果位置
	void                       SetComponentCadResultX(double val) { m_ComponentCadResultX = val; }
	double                     GetComponentCadResultX() const { return m_ComponentCadResultX; }
	void                       SetComponentCadResultY(double val) { m_ComponentCadResultY = val; }
	double                     GetComponentCadResultY() const { return m_ComponentCadResultY; }
	//---------------------------------------------------------------------------------//
	//機台最後位置
	void                       SetComponentStageResultX(double val) { m_ComponentStageResultX = val; }
	double                     GetComponentStageResultX() const { return m_ComponentStageResultX; }
	void                       SetComponentStageResultY(double val) { m_ComponentStageResultY = val; }
	double                     GetComponentStageResultY() const { return m_ComponentStageResultY; }
	//---------------------------------------------------------------------------------//
	//相對機台座標偏移量
	void                       SetComponentStageOffsetX(double val) { m_ComponentStageOffsetX = val; }
	double                     GetComponentStageOffsetX() const { return m_ComponentStageOffsetX; }
	void                       SetComponentStageOffsetY(double val) { m_ComponentStageOffsetY = val; }
	double                     GetComponentStageOffsetY() const { return m_ComponentStageOffsetY; }
	//---------------------------------------------------------------------------------//
	//零件遮罩外擴尺寸-本體
	void                       SetComponentMaskExtendW_Body(double val) { m_ComponentMaskExtendW_Body = val; }
	double                     GetComponentMaskExtendW_Body() const { return m_ComponentMaskExtendW_Body; }
	void                       SetComponentMaskExtendH_Body(double val) { m_ComponentMaskExtendH_Body = val; }
	double                     GetComponentMaskExtendH_Body() const { return m_ComponentMaskExtendH_Body; }
	//---------------------------------------------------------------------------------//
	//零件遮罩外擴尺寸-焊盤
	void                       SetComponentMaskExtendW_Land(double val) { m_ComponentMaskExtendW_Land = val; }
	double                     GetComponentMaskExtendW_Land() const { return m_ComponentMaskExtendW_Land; }
	void                       SetComponentMaskExtendH_Land(double val) { m_ComponentMaskExtendH_Land = val; }
	double                     GetComponentMaskExtendH_Land() const { return m_ComponentMaskExtendH_Land; }
	//---------------------------------------------------------------------------------//	
	void                       SetComponentGroupID(int val) { m_ComponentGroupID = val; }
	int                        GetComponentGroupID() const { return m_ComponentGroupID; }
	void                       SetComponentGroupOrg(bool val) { m_ComponentGroupOrg = val; }
	bool                       GetComponentGroupOrg() const { return m_ComponentGroupOrg; }
	void                       SetComponentGroupOffsetResX(double val) { m_ComponentGroupOffsetResX = val; }
	double                     GetComponentGroupOffsetResX() const { return m_ComponentGroupOffsetResX; }
	void                       SetComponentGroupOffsetResY(double val) { m_ComponentGroupOffsetResY = val; }
	double                     GetComponentGroupOffsetResY() const { return m_ComponentGroupOffsetResY; }
	void                       SetComponentGroupOffsetMaxX(double val) { m_ComponentGroupOffsetMaxX = val; }
	double                     GetComponentGroupOffsetMaxX() const { return m_ComponentGroupOffsetMaxX; }
	void                       SetComponentGroupOffsetMaxY(double val) { m_ComponentGroupOffsetMaxY = val; }
	double                     GetComponentGroupOffsetMaxY() const { return m_ComponentGroupOffsetMaxY; }
	void                       SetComponentGroupOffsetMinX(double val) { m_ComponentGroupOffsetMinX = val; }
	double                     GetComponentGroupOffsetMinX() const { return m_ComponentGroupOffsetMinX; }
	void                       SetComponentGroupOffsetMinY(double val) { m_ComponentGroupOffsetMinY = val; }
	double                     GetComponentGroupOffsetMinY() const { return m_ComponentGroupOffsetMinY; }
	void                       SetComponentGroupDistanceResX(double val) { m_ComponentGroupDistanceResX = val; }
	double                     GetComponentGroupDistanceResX() const { return m_ComponentGroupDistanceResX; }
	void                       SetComponentGroupDistanceResY(double val) { m_ComponentGroupDistanceResY = val; }
	double                     GetComponentGroupDistanceResY() const { return m_ComponentGroupDistanceResY; }		
	//---------------------------------------------------------------------------------//	
	//零件警報
	void                       SetComponentEnableAlarm(bool val) { m_ComponentEnableAlarm = val; }
	bool                       GetComponentEnableAlarm() const { return m_ComponentEnableAlarm; }
	//---------------------------------------------------------------------------------//	
	//零件瑕疵計數警報-維修站顯示		
	void                       SetComponentDefectCountEnableOnARS(bool val) { m_ComponentDefectCountEnableOnARS = val; }
	bool                       GetComponentDefectCountEnableOnARS() const { return m_ComponentDefectCountEnableOnARS; }
	//---------------------------------------------------------------------------------//	
	//零件瑕疵警報-機台警報
	void                       SetComponentDefectAlarmEnableOnAOI(bool val) { m_ComponentDefectAlarmEnableOnAOI = val; }
	bool                       GetComponentDefectAlarmEnableOnAOI() const { return m_ComponentDefectAlarmEnableOnAOI; }
	//---------------------------------------------------------------------------------//		
	//零件瑕疵警報-維修站顯示
	void                       SetComponentDefectAlarmEnableOnARS(bool val) { m_ComponentDefectAlarmEnableOnARS = val; }
	bool                       GetComponentDefectAlarmEnableOnARS() const { return m_ComponentDefectAlarmEnableOnARS; }
	//---------------------------------------------------------------------------------//
	//零件是否瑕疵警報停機
	void                       SetComponentDefectAlarmResultOnAOI(bool value) { m_ComponentDefectAlarmResultOnAOI = value; }
	bool                       GetComponentDefectAlarmResultOnAOI() const { return m_ComponentDefectAlarmResultOnAOI; }
	//---------------------------------------------------------------------------------//
	//零件是否瑕疵警報停機
	void                       SetComponentDefectAlarmResultOnARS(bool value) { m_ComponentDefectAlarmResultOnARS = value; }
	bool                       GetComponentDefectAlarmResultOnARS() const { return m_ComponentDefectAlarmResultOnARS; }
	//---------------------------------------------------------------------------------//
	//零件停機瑕疵設定-AOI
	void                       SetComponentDefectItemAlarmAOI(const CWndDefectItem &val) { m_ComponentDefectItemAlarmAOI = val; }
	const CWndDefectItem&      GetComponentDefectItemAlarmAOI() const { return m_ComponentDefectItemAlarmAOI; }
	//---------------------------------------------------------------------------------//		
	//零件停機瑕疵設定-ARS
	void                       SetComponentDefectItemAlarmARS(const CWndDefectItem &val) { m_ComponentDefectItemAlarmARS = val; }
	const CWndDefectItem&      GetComponentDefectItemAlarmARS() const { return m_ComponentDefectItemAlarmARS; }
	//---------------------------------------------------------------------------------//	
	//零件瑕疵警報項目來源-AOI
	void                       SetComponentDefectAlarmFromModeAOI(DEFECT_PARAM_FROM_MODE val) { m_ComponentDefectAlarmFromModeAOI = val; }
	DEFECT_PARAM_FROM_MODE     GetComponentDefectAlarmFromModeAOI() const { return m_ComponentDefectAlarmFromModeAOI; }
	//---------------------------------------------------------------------------------//	
	//零件瑕疵警報項目來源-ARS
	void                       SetComponentDefectAlarmFromModeARS(DEFECT_PARAM_FROM_MODE val) { m_ComponentDefectAlarmFromModeARS = val; }
	DEFECT_PARAM_FROM_MODE     GetComponentDefectAlarmFromModeARS() const { return m_ComponentDefectAlarmFromModeARS; }
	//---------------------------------------------------------------------------------//	
	//零件檢測數量-AOI
	void                       SetComponentTotalTestCountAOI(size_t val) { m_ComponentTotalTestCountAOI = val; }
	size_t                     GetComponentTotalTestCountAOI() const { return m_ComponentTotalTestCountAOI; }
	//---------------------------------------------------------------------------------//
	size_t                     GetComponentTotalTestCountAOI_Lane(LANE_ID LaneID) const;
	void                       SetComponentTotalTestCountAOI_Lane(LANE_ID LaneID, size_t val);	
	//---------------------------------------------------------------------------------//
	void                       SetComponentTotalTestCountAOI_LA(size_t val) { m_ComponentTotalTestCountAOI_LA = val; }
	size_t                     GetComponentTotalTestCountAOI_LA() const { return m_ComponentTotalTestCountAOI_LA; }
	//---------------------------------------------------------------------------------//
	void                       SetComponentTotalTestCountAOI_LB(size_t val) { m_ComponentTotalTestCountAOI_LB = val; }
	size_t                     GetComponentTotalTestCountAOI_LB() const { return m_ComponentTotalTestCountAOI_LB; }
	//---------------------------------------------------------------------------------//
	//零件檢測數量-ARS
	void                       SetComponentTotalTestCountARS(size_t val) { m_ComponentTotalTestCountARS = val; }
	size_t                     GetComponentTotalTestCountARS() const { return m_ComponentTotalTestCountARS; }
	//---------------------------------------------------------------------------------//
	size_t                     GetComponentTotalTestCountARS_Lane(LANE_ID LaneID) const;
	void                       SetComponentTotalTestCountARS_Lane(LANE_ID LaneID, size_t val);	
	//---------------------------------------------------------------------------------//
	void                       SetComponentTotalTestCountARS_LA(size_t val) { m_ComponentTotalTestCountARS_LA = val; }
	size_t                     GetComponentTotalTestCountARS_LA() const { return m_ComponentTotalTestCountARS_LA; }
	//---------------------------------------------------------------------------------//
	void                       SetComponentTotalTestCountARS_LB(size_t val) { m_ComponentTotalTestCountARS_LB = val; }
	size_t                     GetComponentTotalTestCountARS_LB() const { return m_ComponentTotalTestCountARS_LB; }
	//---------------------------------------------------------------------------------//
	//零件不良數量-AOI
	void                       SetComponentTotalNGCountAOI(size_t val) { m_ComponentTotalNGCountAOI = val; }
	size_t                     GetComponentTotalNGCountAOI() const { return m_ComponentTotalNGCountAOI; }
	//---------------------------------------------------------------------------------//
	size_t                     GetComponentTotalNGCountAOI_Lane(LANE_ID LaneID) const;
	void                       SetComponentTotalNGCountAOI_Lane(LANE_ID LaneID, size_t val);	
	//---------------------------------------------------------------------------------//
	void                       SetComponentTotalNGCountAOI_LA(size_t val) { m_ComponentTotalNGCountAOI_LA = val; }
	size_t                     GetComponentTotalNGCountAOI_LA() const { return m_ComponentTotalNGCountAOI_LA; }
	//---------------------------------------------------------------------------------//
	void                       SetComponentTotalNGCountAOI_LB(size_t val) { m_ComponentTotalNGCountAOI_LB = val; }
	size_t                     GetComponentTotalNGCountAOI_LB() const { return m_ComponentTotalNGCountAOI_LB; }
	//---------------------------------------------------------------------------------//
	//零件不良數量-ARS
	void                       SetComponentTotalNGCountARS(size_t val) { m_ComponentTotalNGCountARS = val; }
	size_t                     GetComponentTotalNGCountARS() const { return m_ComponentTotalNGCountARS; }
	//---------------------------------------------------------------------------------//
	size_t                     GetComponentTotalNGCountARS_Lane(LANE_ID LaneID) const;
	void                       SetComponentTotalNGCountARS_Lane(LANE_ID LaneID, size_t val);	
	//---------------------------------------------------------------------------------//
	void                       SetComponentTotalNGCountARS_LA(size_t val) { m_ComponentTotalNGCountARS_LA = val; }
	size_t                     GetComponentTotalNGCountARS_LA() const { return m_ComponentTotalNGCountARS_LA; }
	//---------------------------------------------------------------------------------//
	void                       SetComponentTotalNGCountARS_LB(size_t val) { m_ComponentTotalNGCountARS_LB = val; }
	size_t                     GetComponentTotalNGCountARS_LB() const { return m_ComponentTotalNGCountARS_LB; }
	//---------------------------------------------------------------------------------//
	//零件累計不良數量上限
	void                       SetComponentTotalNGCountLimit(size_t val) { m_ComponentTotalNGCountLimit = val; }
	size_t                     GetComponentTotalNGCountLimit() const { return m_ComponentTotalNGCountLimit; }
	//---------------------------------------------------------------------------------//	
	//零件累計不良數量啟用
	void                       SetComponentTotalNGCountEnable(bool val) { m_ComponentTotalNGCountEnable = val; }
	bool                       GetComponentTotalNGCountEnable() const { return m_ComponentTotalNGCountEnable; }
	//---------------------------------------------------------------------------------//
	//零件累計不良數量警報
	void                       SetComponentTotalNGCountAlarm(bool val) { m_ComponentTotalNGCountAlarm = val; }
	bool                       GetComponentTotalNGCountAlarm() const { return m_ComponentTotalNGCountAlarm; }
	//---------------------------------------------------------------------------------//	
	//零件連續瑕疵數量-AOI
	void                       AddComponentContinueNGCountAOI() { m_ComponentContinueNGCountAOI++; }
	void                       SetComponentContinueNGCountAOI(size_t val) { m_ComponentContinueNGCountAOI = val; }
	size_t                     GetComponentContinueNGCountAOI() const { return m_ComponentContinueNGCountAOI; }
	//---------------------------------------------------------------------------------//
	//零件連續瑕疵數量-ARS
	void                       AddComponentContinueNGCountARS() { m_ComponentContinueNGCountARS++; }
	void                       SetComponentContinueNGCountARS(size_t val) { m_ComponentContinueNGCountARS = val; }
	size_t                     GetComponentContinueNGCountARS() const { return m_ComponentContinueNGCountARS; }
	//---------------------------------------------------------------------------------//	
	//零件連續不良數量上限
	void                       SetComponentContinueNGCountLimit(size_t val) { m_ComponentContinueNGCountLimit = val; }
	size_t                     GetComponentContinueNGCountLimit() const { return m_ComponentContinueNGCountLimit; }
	//---------------------------------------------------------------------------------//
	//零件連續不良數量啟用
	void                       SetComponentContinueNGCountEnable(bool val) { m_ComponentContinueNGCountEnable = val; }
	bool                       GetComponentContinueNGCountEnable() const { return m_ComponentContinueNGCountEnable; }
	//---------------------------------------------------------------------------------//	
	//零件連續不良數量警報	
	void                       SetComponentContinueNGCountAlarm(bool val) { m_ComponentContinueNGCountAlarm = val; }
	bool                       GetComponentContinueNGCountAlarm() const { return m_ComponentContinueNGCountAlarm; }
	//---------------------------------------------------------------------------------//
	//零件瑕疵結果-AOI
	void                       SetComponentCurrentDefectCountAOI(const CWndDefectItem &val) { m_ComponentCurrentDefectCountAOI = val; }
	const CWndDefectItem&      GetComponentCurrentDefectCountAOI() const { return m_ComponentCurrentDefectCountAOI; }
	//---------------------------------------------------------------------------------//
	//零件瑕疵結果-ARS	
	void                       SetComponentCurrentDefectCountARS(const CWndDefectItem &val) { m_ComponentCurrentDefectCountARS = val; }
	const CWndDefectItem&      GetComponentCurrentDefectCountARS() const { return m_ComponentCurrentDefectCountARS; }
	void                       UpdateComponentCurrentDefectCountARS(LANE_ID LaneID, WND_DEFECT_ID nDefect, std::vector<WND_DEFECT_ID> &nDefectList);
	//---------------------------------------------------------------------------------//
	//零件每個瑕疵數量-AOI
	void                       SetComponentTotalEachDefectCountAOI(const CWndDefectItem &DefectItem);
	void                       GetComponentTotalEachDefectCountAOI(CWndDefectItem &DefectItem) const;
	//---------------------------------------------------------------------------------//
	void                       SetComponentTotalEachDefectCountAOI_Lane(LANE_ID LaneID, const CWndDefectItem &DefectItem);
	void                       GetComponentTotalEachDefectCountAOI_Lane(LANE_ID LaneID, CWndDefectItem &DefectItem) const;
	//---------------------------------------------------------------------------------//
	void                       SetComponentTotalEachDefectCountAOI_LA(const CWndDefectItem &DefectItem);
	void                       GetComponentTotalEachDefectCountAOI_LA(CWndDefectItem &DefectItem) const;
	//---------------------------------------------------------------------------------//
	void                       SetComponentTotalEachDefectCountAOI_LB(const CWndDefectItem &DefectItem);
	void                       GetComponentTotalEachDefectCountAOI_LB(CWndDefectItem &DefectItem) const;
	//---------------------------------------------------------------------------------//
	//零件每個瑕疵數量-ARS
	void                       SetComponentTotalEachDefectCountARS(const CWndDefectItem &DefectItem);
	void                       GetComponentTotalEachDefectCountARS(CWndDefectItem &DefectItem) const;	
	//---------------------------------------------------------------------------------//
	void                       SetComponentTotalEachDefectCountARS_Lane(LANE_ID LaneID, const CWndDefectItem &DefectItem);
	void                       GetComponentTotalEachDefectCountARS_Lane(LANE_ID LaneID, CWndDefectItem &DefectItem) const;	
	//---------------------------------------------------------------------------------//
	void                       SetComponentTotalEachDefectCountARS_LA(const CWndDefectItem &DefectItem);
	void                       GetComponentTotalEachDefectCountARS_LA(CWndDefectItem &DefectItem) const;	
	//---------------------------------------------------------------------------------//
	void                       SetComponentTotalEachDefectCountARS_LB(const CWndDefectItem &DefectItem);
	void                       GetComponentTotalEachDefectCountARS_LB(CWndDefectItem &DefectItem) const;	
	//---------------------------------------------------------------------------------//
	bool                       ResetComponentResultStatistic();	//覆歸零件結果統計
	bool                       AddComponentResultToStatisticAOI(LANE_ID LaneID);//加入零件結果至統計內	
	bool                       ShiftComponentResultToStatisticAOI(size_t val);//偏移零件結果統計
	//---------------------------------------------------------------------------------//
	bool                       AddComponentResultToStatisticARS(LANE_ID LaneID);//加入零件結果至統計內	
	bool                       ShiftComponentResultToStatisticARS(size_t val);//偏移零件結果統計
	//---------------------------------------------------------------------------------//
	bool                       CopyComponentStatisticFrom(CAOIComponent *RefComponentPtr);//複製零件統計資料
	//---------------------------------------------------------------------------------//
	void                       InitComponentInspection();//初始化零件檢測
	bool                       ExecComponentInspection();//執行零件檢測
	bool                       ExecComponentSaveDefectImage(const std::vector<TUNI_FRAME> &UniFrameList);//執行零件儲存瑕疵圖片
	bool                       ExecComponentSaveDefectImage_backup(const std::vector<TUNI_FRAME> &UniFrameList);//執行零件儲存瑕疵圖片
	//---------------------------------------------------------------------------------//
	//空間基準面設定	
	void                       SetComponentPanelBasePlane(double val);	
	double                     GetComponentPanelBasePlane() const { return GetRgnPanelBasePlane(); }
	//---------------------------------------------------------------------------------//
	void                       SetComponentLocalBasePlaneID(int val) { SetRgnLocalBasePlaneID(val); }
	int                        GetComponentLocalBasePlaneID() const { return GetRgnLocalBasePlaneID(); }
	//---------------------------------------------------------------------------------//
	void                       SetComponentLocalBasePlaneFinish(bool val) { SetRgnLocalBasePlaneFinish(val); }
	bool                       GetComponentLocalBasePlaneFinish() const { return GetRgnLocalBasePlaneFinish(); }
	//---------------------------------------------------------------------------------//	
	void                       SetComponentLocalBasePlaneParam(const TPOINT3D &val) { SetRgnLocalBasePlaneParam(val); }
	void                       GetComponentLocalBasePlaneParam(TPOINT3D &val) const { GetRgnLocalBasePlaneParam(val); }
	//---------------------------------------------------------------------------------//
	void                       RotateComponentBasePlaneParam(double Angle);
	void                       SetComponentSpaceBasePlaneParam(const TBasePlaneParam& Param);
	TBasePlaneParam&           GetComponentSpaceBasePlaneParam() { return GetRgnSpaceBasePlaneParam(); }
	const TBasePlaneParam&     GetComponentSpaceBasePlaneParam() const { return GetRgnSpaceBasePlaneParam(); }
	//---------------------------------------------------------------------------------//
	//空間雜訊過濾處理	
	void                       SetComponentSpaceNoiseFilterParam(const TNoiseFilterParam& Param);
	TNoiseFilterParam&         GetComponentSpaceNoiseFilterParam() { return GetRgnSpaceNoiseFilterParam(); }
	const TNoiseFilterParam&   GetComponentSpaceNoiseFilterParam() const { return GetRgnSpaceNoiseFilterParam(); }	
	//---------------------------------------------------------------------------------//
	//特殊遮罩-基準面
	void                       SetComponentMaskEnable_Base(bool val); 
	bool                       GetComponentMaskEnable_Base() const; 

	void                       SetComponentMaskFrameIndex_Base(unsigned int val);//影像序號
	unsigned int               GetComponentMaskFrameIndex_Base() const;//影像序號

	void                       SetComponentMaskFrameUniqueID_Base(unsigned int val);//影像唯一碼
	unsigned int               GetComponentMaskFrameUniqueID_Base() const;//影像唯一碼

	void                       SetComponentMaskColorGroupLinkIndex(int val);//彩色過濾的連動編號	
	int                        GetComponentMaskColorGroupLinkIndex() const;//彩色過濾的連動編號	
	//---------------------------------------------------------------------------------//
	bool                       UpdateComponentColorGroupLinkIndex(const std::vector<CColorGroup> &ColorGroupList);//更新模組內的彩色過濾連動
	bool                       UpdateComponentFrameIndex(unsigned int DefaultIndex, unsigned int DefaultUniqueID, const std::vector<unsigned int> &FrameIndexMapList);
	//---------------------------------------------------------------------------------//	
	void                       BackupComponentParamByVersion();//備份零件所有版本號下的狀態
	void                       RestoreComponentParamByVersion();//還原零件所有版本號下的狀態
	//---------------------------------------------------------------------------------//		
	size_t                     GetComponentVersionParamCount() const;//取得零件版本號參數數量
	bool                       ResetComponentVersionParam(size_t idx);//復歸零件版本號參數
	bool                       CheckComponentVersionParamIndex(size_t idx) const;//確認零件版本號參數引數
	const TVersionParam*       GetComponentVersionParamPtr(size_t idx, bool Check) const;//取得零件版本號參數指標
	bool                       ChangeComponentVersionParam(size_t idx, bool &ModelChanged);//切換零件狀態至該版本參數下
	bool                       UpdateToComponentVersionParam(size_t idx);//更新目前零件狀態至版本參數資料
	bool                       CopyComponentVersionParam(size_t idxSrc, size_t idxDst);//複製不同版本參數的零件狀態
	bool                       SetComponentVersionParamList(const std::vector<TVersionParam> &VersionList);//設定零件版本參數列表	
	//---------------------------------------------------------------------------------//	
	bool                       SetComponentGroupResult(unsigned int WndIdx, RESULT_ID ResultID, LPCTSTR DefectText);//設定零件群組結果
	//---------------------------------------------------------------------------------//	
	bool                       BuildComponentBarcodeList(std::vector<std::wstring> &BarcodeList);//取得零件條碼列表
	//---------------------------------------------------------------------------------//	
	bool                       GetComponentDataModelEnabled() const;//取得零件資料模型啟用
	void                       SetComponentDataModelEnabled(bool val);//設定零件資料模型啟用
	//---------------------------------------------------------------------------------/
	int                        GetComponentDataModelLevelID() const;//取得零件資料模型等級
	void                       SetComponentDataModelLevelID(int val);//設定零件資料模型等級
	//---------------------------------------------------------------------------------/	
	//NPM-APC		
	double                     GetComponentNPM_APC_MffX() const;//取得APC-零件偏移-X-um
	void                       SetComponentNPM_APC_MffX(double val);//設定APC-零件偏移-X-um
	double                     GetComponentNPM_APC_MffY() const;//取得APC-零件偏移-Y-um
	void                       SetComponentNPM_APC_MffY(double val);//設定APC-零件偏移-Y-um	
	double                     GetComponentNPM_APC_MffA() const;//取得APC-零件偏移-A-%
	void                       SetComponentNPM_APC_MffA(double val);//設定APC-零件偏移-A-%
	unsigned int               GetComponentNPM_APC_FF1_IDNUM() const;//取得APC-FF1-零件編號
	void                       SetComponentNPM_APC_FF1_IDNUM(unsigned int val);//設定APC-FF1-零件編號
	bool                       CheckComponentNPM_APC_FF1_Done() const;//確認APC-FF1-已經設定

	double                     GetComponentNPM_APC_EPosX() const;//取得APC-電極偏移-X-um
	void                       SetComponentNPM_APC_EPosX(double val);//設定APC-電極偏移-X-um
	double                     GetComponentNPM_APC_EPosY() const;//取得APC-電極偏移-Y-um
	void                       SetComponentNPM_APC_EPosY(double val);//設定APC-電極偏移-Y-um
	double                     GetComponentNPM_APC_EPosA() const;//取得APC-電極偏移-A-%
	void                       SetComponentNPM_APC_EPosA(double val);//設定APC-電極偏移-A-%
	unsigned int               GetComponentNPM_APC_MFB_IDNUM() const;//取得APC-MFB-零件編號
	void                       SetComponentNPM_APC_MFB_IDNUM(unsigned int val);//設定APC-MFB-零件編號
	bool                       CheckComponentNPM_APC_MFB_Done() const;//確認APC-MFB-已經設定
	bool                       CalcComponentNPM_APC_Result(TNPM_APC_Result &APC_Result);//計算零件的NPM-APC結果
	void                       ResetComponentNPM_APC_FF1();//復歸NPM-APC-FF1-參數
	void                       ResetComponentNPM_APC_MFB();//復歸NPM-APC-MFB-參數
	void                       ResetComponentNPM_APC_Param();//復歸NPM-APC-參數
	//---------------------------------------------------------------------------------/	
	// M2M Machine to Machine - HAS I(Hanwha AOI Solution MAOI_SAOI)
	bool                       GetComponentHASI_SPIOffset_Enable(); //取得啟用SPI校正
	void                       SetComponentHASI_SPIOffset_Enable(bool State);//設定啟用SPI校正
	bool                       GetComponentHASI_SPIOffset_IsApplied(); //取得SPI校正是否成功套用
	void                       SetComponentHASI_SPIOffset_IsApplied(bool State);//設定SPI校正是否成功套用
	bool                       GetComponentHASI_SaveImage();//取得啟用存圖到共享資料夾
	void                       SetComponentHASI_SaveImage(bool value);//設定啟用存圖到共享資料夾
	bool                       CalcComponentHASI_Result(CString &DefectCode);
	//---------------------------------------------------------------------------------/	
	void                       ResetComponentResultPtr();//重設零件結果指標
	CAOIComponent*             GetComponentResultPtr();//取得零件結果指標
	void                       SetComponentResultPtr(CAOIComponent* Ptr);//設定零件結果指標
	//---------------------------------------------------------------------------------/	
	void                       ResetComponentMaster();//重設零件本尊
	CAOIComponent*             GetComponentMasterPtr();//取得零件本尊
	void                       SetComponentMasterPtr(CAOIComponent* Ptr);//設定零件本尊
	unsigned int               GetComponentMasterIndex() const;//取得零件本尊引數
	void                       SetComponentMasterIndex(unsigned int value);//設定零件本尊引數		
	bool                       CheckComponentIsMaster() const;//確認零件是否為本尊
	bool                       CheckComponentIsAgent() const;//確認零件是否為代理人
	bool                       CheckComponentIsMasterOrAgent() const;//確認零件是否為本尊或代理人
	bool                       ChangeComponentMaster(CAOIComponent *MasterPtr);//變更零件本尊
	bool                       UpdateComponentMasterResultID();//確認零件本尊結果編號
	//---------------------------------------------------------------------------------//
	unsigned int               GetComponentAgentIndex() const;//取得零件代理人編號
	void                       SetComponentAgentIndex(unsigned int value);//設定零件代理人編號	
	//---------------------------------------------------------------------------------//		
	void                       ClearComponentAgentList();//清除零件代理人列表
	size_t                     GetComponentAgentCount() const;//取得零件代理人數量	
	bool                       CheckComponentAgentSelected();//確認零件代理人有被選到
	bool                       SetComponentAllAgentSelected(bool val);//設定零件所有代理人選取狀態
	bool                       CheckComponentOrAgentNeedToCalculate();//確認零件或代理人有要去計算
	bool                       CheckComponentAgentExist(LPCTSTR PartNumber);//確認零件代理人已經存在
	bool                       SetComponentBeAgent(LPCTSTR PartNumber, LPCTSTR LibFolder);//設定零件成為代理人
	bool                       AddComponentAgentPtr(CAOIComponent *Ptr);//加入零件代理人
	bool                       RemoveComponentAgentSelected();//移除零件代理人選取到 	
	bool                       UpdateComponentAgentPtr(CAOIComponent *Ptr);//更新零件代理人
	CAOIComponent*             GetComponentAgentPtr(size_t idx, bool bCheck);//取得零件代理人指標	
	CAOIComponent*             GetComponentAgentPtrByPartNumber(LPCTSTR PartNumber);//取得零件代理人指標-依照料號
	unsigned int               FindComponentAgentIndex(const CAOIComponent *Ptr);//尋找零件代理人編號			
	//---------------------------------------------------------------------------------//		
	void                       ChangeComponentSelected(bool value);//變更零件選取狀態
	void                       ChangeComponentMasterName(LPCTSTR Name);//變更零件本尊名稱
	void                       ChangeComponentXBoardUnitMaster(bool value);//變更零件本尊報廢件	
	//---------------------------------------------------------------------------------//
};
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_AOICOMPONENT_H__C766CB12_822D_4845_9EA9_D0E70DC0C0CC__INCLUDED_)
