// AOIModel.h: interface for the CAOIModel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AOIMODEL_H__CD0BC378_9070_41F3_BF55_6D37A745AF96__INCLUDED_)
#define AFX_AOIMODEL_H__CD0BC378_9070_41F3_BF55_6D37A745AF96__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include "AOIModelDef.h"
#include "AOIObjManager.h"
#include "AOIObj.h"
#include "AOIWnd.h"
#include "AOILand.h"
#include "AOILogic.h"
#include "WndDefectItem.h"
//-------------------------------------------------------------------------------------//
#define   MODEL_CLASS_ID_MAX                              16//模組類別上限
#define   MODEL_CLASS_ID_NONE                             -1//模組類別未定義
#define   MODEL_CLASS_ID_COUNT                            MODEL_CLASS_ID_MAX+1//模組類別數量
//-------------------------------------------------------------------------------------//
#define   MODEL_CLEAR_FOLDER_PATTERNS                     0x00000001
#define   MODEL_CLEAR_FOLDER_BK_IMAGES                    0x00000002
#define   MODEL_CLEAR_FOLDER_ALL_FILES                    0xFFFFFFFF
//-------------------------------------------------------------------------------------//
#define   MODEL_LINK_REGION_NONE                          0x00000000//都不連動
#define   MODEL_LINK_LAND_POS                             0x00000001//特徵框位置連動
#define   MODEL_LINK_LAND_SIZE                            0x00000002//特徵框尺寸連動
#define   MODEL_LINK_BODY_WND_POS                         0x00000010//本體檢測框位置連動
#define   MODEL_LINK_BODY_WND_SIZE                        0x00000020//本體檢測框尺寸連動
#define   MODEL_LINK_LAND_WND_POS                         0x00000100//特徵檢測框位置連動
#define   MODEL_LINK_LAND_WND_SIZE                        0x00000200//特徵檢測框尺寸連動
#define   MODEL_LINK_REGION_ALL                           0xFFFFFFFF//全部連動
#define   MODEL_LINK_REGION_NO_LAND_POS                   MODEL_LINK_REGION_ALL-MODEL_LINK_LAND_POS//沒有焊盤位置
#define   MODEL_LINK_REGION_NO_POS                        MODEL_LINK_REGION_ALL-MODEL_LINK_LAND_POS-MODEL_LINK_BODY_WND_POS-MODEL_LINK_LAND_WND_POS//沒有位置連動
//-------------------------------------------------------------------------------------//
class CAOIModel;
//-------------------------------------------------------------------------------------//
typedef struct tagModelInfo//模組資訊結果
{
	CAOIModel *sModelPtr;
	CString    sModelName;	
	time_t     sModifiedTime;

	int        sTempInt;
	tagModelInfo()
	{
		sModelPtr = NULL;
		sModifiedTime = 0;
		sTempInt = 0;
	}
} TModelInfo, *PModelInfo;
//-------------------------------------------------------------------------------------//
typedef struct tagSpecResult//規格結果
{
	double       sUSL;//規格上限
	double       sLSL;//規格下限
	double       sSpec;//規格中心
	double       sValue;//量測結果
	double       sValueMin;//量測最小值
	double       sValueMax;//量測最大值	
	unsigned int sIndex;//引數
	tagSpecResult()
	{
		sUSL = 0.0;
		sLSL = 0.0;
		sSpec = 0.0;
		sValue = 0.0;
		sValueMin = 0.0;
		sValueMax = 0.0;
		sIndex = -1;	
	}
	void Init()
	{
		sUSL   = INVALID_DOUBLE;
		sLSL   = INVALID_DOUBLE;
		sSpec  = INVALID_DOUBLE;
		sValue = INVALID_DOUBLE;
		sValueMin = INVALID_DOUBLE;
		sValueMax = INVALID_DOUBLE;
		sIndex = INVALID_INDEX;	
	}
} TSpecResult, *PSpecResult;
//-------------------------------------------------------------------------------------//

typedef struct WndInspectionParameter {
	double skew;
	TREGION4D ModelRgn;
	TPOINT2D RgnCp;
	TPOINT2D Scale;
	TPOINT2D ImageCp;
	std::vector<TUNI_FRAME> UniFrameList;
	CAOIModel* ModelCopyPtr;

	WndInspectionParameter() {
		ModelCopyPtr = NULL;
	}
	void Setting(double skewSrc, TREGION4D ModelRgnSrc, TPOINT2D RgnCpSrc, TPOINT2D ScaleSrc, TPOINT2D ImageCpSrc, std::vector<TUNI_FRAME> UniFrameListSrc) {
		skew = skewSrc;
		ModelRgn = ModelRgnSrc;
		RgnCp = RgnCpSrc;
		Scale = ScaleSrc;
		ImageCp = ImageCpSrc;
		UniFrameList = UniFrameListSrc;
	}
	//void FreeUniFrameMem() {
	//	CAOIObjManager  AOIObjManager;
	//	JetAPI::ClearUniFrameList(UniFrameList);
	//	AOIObjManager.DestroyModelObj(ModelCopyPtr);
	//}
};
//-------------------------------------------------------------------------------------//
class CAOIFileIO;
class CAOIProject;
class CAOIRgn;
class CAOIFd;
class CAOIMark;
class CAOIBarcode;
class CAOIComponent;
//-------------------------------------------------------------------------------------//
class CAOIModel : public CAOIObj  
{
	//---------------------------------------------------------------------------------//
	DECLARE_DYNAMIC(CAOIModel)	
	//---------------------------------------------------------------------------------//	
	static bool                g_AIServerIsReady;
	static DWORD               g_AIServerTimeout;
	static CString             g_AIServerFolderSend;
	static CString             g_AIServerFolderRecv;	
	static bool                GetAIServerIsReady();	
	static void                SetAIServerIsReady(bool val);	
	static DWORD               GetAIServerTimeout();	
	static void                SetAIServerTimeout(DWORD val);
	static LPCTSTR             GetAIServerFolderSend();	
	static void                SetAIServerFolderSend(LPCTSTR val);	
	static LPCTSTR             GetAIServerFolderRecv();	
	static void                SetAIServerFolderRecv(LPCTSTR val);	
	static bool                CheckAIServerIsReady();
	static bool                GetAIServerDebugMode();
	static bool                WaitForAIServerSyncFile(LPCTSTR SyncFile);
	//---------------------------------------------------------------------------------//
	static bool                CreateModelPen();
	static bool                DestroyModelPen();
	static HPEN                hNullPen;
	static HPEN                hComPen;
	static HPEN                hPadPen;
	static HPEN                hPadPen2;
	static HPEN                hLeadPen;	
	static HPEN                hLeadPen2;
	static HPEN                hShoulderPen;
	static HPEN                hShoulderPen2;
	static HPEN                hLeadTipPen;
	static HPEN                hLeadTipPen2;
	static HPEN                hSelPenEdit;
	static HPEN                hSelPenEditW;//寬線
	static HPEN                hSelPenResult;
	static HPEN                hSelPenResultW;//寬線
	static HPEN                hLandPen;
	static HPEN                hLandPen2;
	static HPEN                hWndPen;
	static HPEN                hNGPen;
	static HPEN                hOKPen;
	static HPEN                hBoxPen;
	static HPEN                hExtendPen;
	static HPEN                hSubBoxPen1;
	static HPEN                hSubBoxPen2;
	static HPEN                hSubBoxPen3;
	static HPEN                hSubBoxPen4;
	static HPEN                hMaskBoxPen1;
	static HPEN                hMaskBoxPen2;
	static HPEN                hIntervalPen;
	static HPEN                hNGSubBoxPen1;
	static HPEN                hOKSubBoxPen1;
	static HPEN                hNGSubBoxPen2;
	static HPEN                hOKSubBoxPen2;
	static HBRUSH              hEditBrush;
	static HBRUSH              hMaskBoxBrush;
	static HBRUSH              hMaskBoxBrushErase;
	//---------------------------------------------------------------------------------//
	static bool                CheckModelTypeEnabled(MODEL_TYPE Type);
	static bool                GetModelTypeText(MODEL_TYPE Type, char Text[]);
	static bool                GetModelTypeText(MODEL_TYPE Type, wchar_t Text[]);
	static bool                GetModelTypeList(std::vector<MODEL_TYPE> &List);//取得模組樣式表
	static CString             GetModelDefaultFilename(LPCTSTR GroupName);//取得預設模組名稱
	static CString             GetModelDefaultImageFilename(LPCTSTR GroupName);//取得預設模組影像名稱
	static CString             GetModelChipSizeModeText(CHIP_SIZE_MODE Mode);//取得模組被動元件尺寸模式文字
	static bool                CheckModelTypeUseBody(MODEL_TYPE ModelType);//確認模組樣式使用零件本體
	static bool                CheckModelTypUseChipSizeMode(MODEL_TYPE ModelType);//確認模組樣式適合被動元件尺寸模式
	static bool                CheckModelTypeUseTwoElectrode(MODEL_TYPE ModelType);//確認模組樣式有2個電極端
	static bool                GetModelChipSize(CHIP_SIZE_MODE Mode, double &Width, double &Length, double &Height);//取得模組被動元件尺寸
	static bool                GetModelChipSizeGap(CHIP_SIZE_MODE Mode, double &WidthGap, double &LengthGap, double &HeightGap);//取得模組被動元件尺寸公差
	static CHIP_SIZE_MODE      FindModelChipSizeMode(MODEL_TYPE ModelType, const TREGION4D &BodyRgn);//尋找模組被動元件尺寸模式
	static bool                GetModelSimilarTypeList(MODEL_TYPE Type, std::vector<MODEL_TYPE> &TypeList);//取得相同模組樣式的樣式列表
	static bool                CheckModelWndRoiCanBeDeleted(ALG_TYPE Type);//確認該演算法的子框能不能被刪除
	static int                 ObtainModelAlgDefaultWndRoiCount(ALG_TYPE Type);//取得模組演算法預設子框數量	
	static WND_SYNC_MOVE_MODE  ObtainModelDefaultWndSyncMoveMode(MODEL_TYPE ModelType);//取得模組預設檢測框同步移動模式
	static bool                CheckModelWndRoiAutoAdd(ALG_TYPE Type, CAOILand *LandPtr);//確認該演算法的子框能不能自動建立
	static WND_RGN_LINK_MODE   ObtainModelDefaultWndRegionLinkMode(MODEL_TYPE ModelType, WND_DEFECT_ID WndDefectID);
	static bool                CheckModelAlgWndRoiSelfFrameEnabled(ALG_TYPE Type);//確認該演算法的燈源能不能被個別設定
	static bool                CalcModelBoxRegionRect(const TREGION4D &BoxRgn, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const TPOINT2D &RgnCp, const TPOINT2D &Scale, const TPOINT2D &ImageCp, RECT &RoiRect);//計算檢測框圖像區域
	static bool                CalcModelBoxCornerPts(const TPOINT2D CornerPts[], IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const TPOINT2D &RgnCp, const TPOINT2D &Scale, const TPOINT2D &ImageCp, TPOINT2D ResultPts[]);//計算檢測框檢測區域
	static bool                CalcModelBoxPtPoint(const TPOINT2D &Pt, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const TPOINT2D &RgnCp, const TPOINT2D &Scale, const TPOINT2D &ImageCp, TPOINT2D &ResultPt);//計算檢測框檢測位置
	static bool                CalcModelBoxRectRegion(const RECT &BoxRect, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const TPOINT2D &RgnCp, const TPOINT2D &Scale, const TPOINT2D &ImageCp, TREGION4D &RoiRegion);//計算檢測框模組區域
	static bool                RotateModelUniFrameList(double Angle, const TREGION4D &ModelRgn, const TREGION4D ModelRgnRoated, const std::vector<TUNI_FRAME> &UniFrameList, std::vector<TUNI_FRAME> &UniFrameListDst);//旋轉模組通用影像列表
	//---------------------------------------------------------------------------------//
private:
	//---------------------------------------------------------------------------------//
	unsigned int               m_ModelIndex;                 //模組引數
	MODEL_TYPE                 m_ModelType;                  //模組樣式	
	std::wstring               m_ModelName;                  //模組名稱	
	std::wstring               m_ModelGroupName;             //模組群組名稱	
	CHIP_SIZE_MODE             m_ModelChipSizeMode;          //模組被動元件尺寸模式
	MODEL_LAND_DIRECTION       m_ModelLandDirection;         //模組腳位方向
	MODEL_LAND_ADJUST_MODE     m_ModelPadAdjustMode;         //模組焊盤調整模式
	MODEL_LAND_ADJUST_MODE     m_ModelLeadAdjustMode;        //模組引腳調整模式	
	WND_SYNC_MOVE_MODE         m_ModelWndSyncMoveMode;       //模組檢測框同步移動模式
	int                        m_ModelClassID;               //模組類別編號(指定)
	int                        m_ModelActClassID;            //模組啟用類別編號//Active Class ID
	bool                       m_ModelSelected;              //模組選取	
	bool                       m_ModelUsing3D;               //模組使用3D
	bool                       m_ModelBypass3D;              //模組3D不檢測
	bool                       m_ModelIsolated;              //模組是否獨立處理
	bool                       m_ModelSelfTest;              //模組自我測試-非零件,條碼, 定位點檢測
	bool                       m_ModelSaveLeadReport;        //模組儲存引腳報告	
	bool                       m_ModelBodyLinkChipLead;      //模組本體連動被動元件電極
	int                        m_ModelTempInt[4];            //模組暫存整數	
	RESULT_ID                  m_ModelResultID;              //模組的檢測結果
	RESULT_ID                  m_ModelResultID_Alarm;        //模組的檢測結果-警報
	bool                       m_ModelNeedSaveFiles;         //模組要儲存檔案
	double                     m_ModelInspectedTime;         //模組的檢測時間	
	time_t                     m_ModelModifiedDateTime;      //模組修改日期時間//__int64
	int                        m_ModelWndLinkMode;           //模組檢測框連動模式
	int                        m_ModelOpenMPCount;           //模組使用的OpenMP數量
	TPOINT2D                   m_ModelImageScale;            //模組的影像解析度//1um=>?pixels			
	bool                       m_ModelModifiedCount;         //模組修改過數量	
	CWndDefectItem             m_ModelDefectItemTest;        //模組瑕疵檢測項目模式
	CWndDefectItem             m_ModelDefectItemAlarm;       //模組瑕疵警報項目模式		
	CWndDefectItem             m_ModelDefectItemEssential;   //模組瑕疵必要檢測項目
	CWndDefectItem             m_ModelDefectItemRecheck_ARS; //模組瑕疵重複確認項目-ARS
	bool                       m_ModelAutoDeleteImageFolder; //模組自動刪除圖像資料夾				
	//---------------------------------------------------------------------------------//
	unsigned int               m_ModelUsedCount;             //模組使用的次數	
	unsigned int               m_ModelBKImageIndex;          //模組底圖影像編號
	bool                       m_ModelBKImageNeedToGrab;     //模組是否需要重取底圖影像
	//---------------------------------------------------------------------------------//
	bool                       m_ModelExtendAutoAdjust;      //模組整個外擴自動修正
	TSIZE2D                    m_ModelExtendRange;           //模組整個外擴範圍-基準面使用	
	TSIZE2D                    m_ModelImageSize_um;          //模組影像尺寸-um
	TPOINT2D                   m_ModelImageCadOffset_um;     //模組影像Cad偏差-um
	TREGION4D                  m_ModelTotalRgn;              //模組整個區域 
	TREGION4D                  m_ModelTotalRgnRaw;           //模組整個區域-原始 
	TREGION4D                  m_ModelTotalRgnCad;           //模組整個區域-Cad
	TREGION4D                  m_ModelTotalRgnStage;         //模組整個區域-Stage
	TPOINT2D                   m_ModelTotalCornerPts[4];     //模組整個區域-四個端點
	TPOINT2D                   m_ModelTotalCornerPtsCad[4];  //模組整個區域-四個端點-Cad
	TPOINT2D                   m_ModelTotalCornerPtsStage[4];//模組整個區域-四個端點-Stage
	//---------------------------------------------------------------------------------//
	TREGION4D                  m_ModelBodyLandRgn;           //模組特徵區域 
	TPOINT2D                   m_ModelBodyLandCornerPts[4];  //模組特徵區域-四個端點
	//---------------------------------------------------------------------------------//	
	CAOIRgn*                   m_ModelAttachedPtr;           //模組連結的指標
	double                     m_ModelAttachedAngle;         //模組連結的旋轉角度
	TPOINT2D                   m_ModelAttachedCadPos;        //模組連結的座標-Cad
	TPOINT2D                   m_ModelAttachedStagePos;      //模組連結的座標-Stage	
	//---------------------------------------------------------------------------------//
	CAOIBox                    m_ModelBodyBox;               //模組零件本體框
	double                     m_ModelBodySizeX;             //模組本體尺寸X//參數而已
	double                     m_ModelBodySizeY;             //模組本體尺寸Y//參數而已
	double                     m_ModelBodyHeight;            //模組本體高度//參數而已
	RECT                       m_ModelBodyImageRect_Raw;     //模組零件本體影像位置-原始
	CColorGroup                m_ModelBodyColorGroup;        //模組本體顏色
	std::vector<CAOIWnd*>      m_ModelWndList;               //模組檢測框列表
	std::vector<CAOILand*>     m_ModelLandList;              //模組特徵框列表
	std::vector<CAOILogic*>    m_ModelLogicList;             //模組邏輯閘列表
	//---------------------------------------------------------------------------------//
	std::vector<CAOIWnd*>      m_ModelWndOrderList;          //模組檢測框檢測列表
	//---------------------------------------------------------------------------------//
	std::vector<int>           m_ModelDefectWndGroupIDList;  //模組瑕疵檢測框群組列表
	std::vector<UUID>          m_ModelDefectWndUUIDList;     //模組瑕疵檢測框UUID列表
	//---------------------------------------------------------------------------------//
	//樣板資料夾的部分
	CString                    m_ModelFolderModel;           //模組的資料夾-資料庫內	
	CString                    m_ModelFolderComponent;       //模組的資料夾-零件資料庫內	
	//---------------------------------------------------------------------------------//	
	std::vector<TUNI_FRAME>    m_ModelUniFrameList;	         //模組的通用影像列表	
	//---------------------------------------------------------------------------------//	
	double                     m_ModelPanelBasePlane;        //模組的整板基準面高度
	TNoiseFilterParam          m_ModelSpaceNoiseFilterParam;   //模組的空間雜訊過濾處理
	//---------------------------------------------------------------------------------//	
	//特殊遮罩-基準面朝照
	bool                       m_ModelMaskEnable_Base;//是否啟用
	unsigned int               m_ModelMaskFrameIndex_Base;//影像序號
	unsigned int               m_ModelMaskFrameUniqueID_Base;//影像唯一碼
	int                        m_ModelMaskColorGroupLinkIndex;//彩色過濾的連動編號
	//---------------------------------------------------------------------------------//
	bool                       m_ModelDataModelEnabled;//資料模型使用 
	int                        m_ModelDataModelLevelID;//資料模型等級
	//---------------------------------------------------------------------------------//		
protected:
	//---------------------------------------------------------------------------------//
	void                       PreInitModel();
	void                       InitialModel();
	void                       InitialModelColorGroup();
	void                       CloneModel(const CAOIModel &Model);
	void                       CloneModelObjectList(const CAOIModel &Model);
	void                       CloneModelUniFrameList(const CAOIModel &Model);
	//---------------------------------------------------------------------------------//	
	bool                       CheckModelPtr(CAOIModel *Ptr);//確認模組指標
	//---------------------------------------------------------------------------------//	
	void                       ClearModelWndOrderList_Inline(); //清除模組檢測框檢測次序列表
	size_t                     GetModelWndOrderCount_Inline() const;
	CAOIWnd*                   GetModelWndOrderPtr_Inline(size_t index) const;
	//---------------------------------------------------------------------------------//
	size_t                     GetModelLandCount_Inline() const;	
	void                       AddModelLandPtr_Inline(CAOILand *LandPtr);	
	CAOILand*                  GetModelLandPtr_Inline(size_t index) const;
	//---------------------------------------------------------------------------------//
	size_t                     GetModelWndCount_Inline() const;	
	void                       AddModelWndPtr_Inline(CAOIWnd *WndPtr);	
	CAOIWnd*                   GetModelWndPtr_Inline(size_t index) const;
	//---------------------------------------------------------------------------------//
	size_t                     GetModelLogicCount_Inline() const;
	void                       AddModelLogicPtr_Inline(CAOILogic *LogicPtr);	
	CAOILogic*                 GetModelLogicPtr_Inline(size_t index) const;	
	//---------------------------------------------------------------------------------//	
	bool                       ApplyModelLandSizeKernel(CAOILand *RefLandPtr);//同步化特徵框的尺寸
	bool                       ApplyModelLandRegionKernel(CAOILand *RefLandPtr);//同步化特徵框的位置尺寸
	bool                       ApplyModelLandWndLogicKernel(CAOILand *RefLandPtr);//同步化特徵框的檢測框與邏輯閘	
	bool                       AlignCenterUModelLandSelectedKernel();		
	bool                       SpinModelLandSelectedKernel(double Angle);//自轉模組選到的特徵框
	bool                       MirrorXModelLandSelectedKernel(double CPY);
	bool                       MirrorYModelLandSelectedKernel(double CPX);
	bool                       RotateModelLandSelectedKernel(double Angle, double CPX, double CPY);

	double                     GetModelLandPitchByTowardKernel(CAOILand *RefLandPtr, bool bActiveOnly) const;	
	bool                       ModifyModelLandAlignKernel(bool SelectedOnly);	
	bool                       ModifyModelLandCountKernel(CAOILand *RefLandPtr, size_t LandCount, bool bArray);		
	bool                       ModifyModelLandPitchKernel(CAOILand *RefLandPtr, double LandPitch, bool SelectedOnly);	
	bool                       LayoutModelLandByLandGroupIDKernel(int LandGroupID, int LandAlignID);
	bool                       AlignModelLandByLandGroupIDKernel(int LandGroupID, int LandAlignID);	
	bool                       GetModelLandIndexListKernel(CAOILand *RefLandPtr, bool bSelected, std::vector<size_t> &LandIndexList, double &Pitch, double &MaxPitch);//取得特定方向的焊盤數量
	//---------------------------------------------------------------------------------//	
	bool                       CloneModelWndSelectedKernel(bool LinkMode);
	bool                       MoveModelWndSelectedKernel(BOX_TOWARD RefToward, double dX, double dY);
	bool                       AlignCenterModelWndSelectedKernel();//對齊模組檢測框
	bool                       AlignCenterUModelWndSelectedKernel();//對齊模組檢測框-U
	bool                       AlignCenterVModelWndSelectedKernel();//對齊模組檢測框-V
	bool                       MirrorXModelWndSelectedKernel(double CPY);
	bool                       MirrorYModelWndSelectedKernel(double CPX);
	bool                       SpinModelWndSelectedKernel(double Angle);
	bool                       RotateModelWndSelectedKernel(double Angle, double CPX, double CPY);
	bool                       ModifyModelOtherLandWndRegionKernel(CAOIWnd *RefWndPtr);		
	void                       UpdateModelWndRgnByLinkModeKernel();
	//---------------------------------------------------------------------------------//		
	bool                       AnalysisModelProperty_Kernel(std::vector<TUNI_FRAME> &UniFrameList);//分析模組	
	bool                       ApplyModelPropertyToAlgParam_Kernel();//套用模組屬性至演算法參數
	//---------------------------------------------------------------------------------//
	bool                       CalcModelLandOffsetRange(BOX_TOWARD Toward, TPOINT2D &Range);//計算模組特徵框偏移範圍
	bool                       CalcModelOffsetLimit(const TMODEL_DEFAULT_WND_PARAM &Param, TPOINT2D &Limit);//計算模組偏移上限	
	bool                       AddModelDefaultWndKernal(const TMODEL_DEFAULT_WND_PARAM &Param);//增加模組檢測框
	bool                       ApplyModelDefaultLandWnd(CAOIWnd *WndPtr, CAOIWnd *RefWndPtr);//增加模組引腳檢測框
	bool                       ApplyModelDefaultBodyWnd(CAOIWnd *WndPtr, const TMODEL_DEFAULT_WND_PARAM &Param);//增加模組本體檢測框	
	bool                       AddModelDefaultWnd_PadAlignKernel(const TMODEL_DEFAULT_WND_PARAM &Param, int LandGroupID);//增加模組檢測框-焊盤定位
	bool                       AddModelDefaultWnd_PartAlignKernel(const TMODEL_DEFAULT_WND_PARAM &Param, int LandGroupID);//增加模組檢測框-零件定位	
	bool                       AddModelDefaultWnd_PartAlignKernel_v1(const TMODEL_DEFAULT_WND_PARAM &Param, int LandGroupID);//增加模組檢測框-零件定位	
	bool                       AddModelDefaultWnd_PartAlignKernel_v2(const TMODEL_DEFAULT_WND_PARAM &Param, int LandGroupID);//增加模組檢測框-零件定位	
	bool                       AddModelDefaultWnd_PadAdjustKernel(const TMODEL_DEFAULT_WND_PARAM &Param, int LandGroupID);//增加模組檢測框-焊盤調整
	bool                       AddModelDefaultWnd_LeadAdjustKernel(const TMODEL_DEFAULT_WND_PARAM &Param, int LandGroupID);//增加模組檢測框-引腳調整
	bool                       AddModelDefaultWnd_PolarityKernel(const TMODEL_DEFAULT_WND_PARAM &Param);//增加模組檢測框-極性檢測
	bool                       AddModelDefaultWnd_PolarityKernel_v2(const TMODEL_DEFAULT_WND_PARAM &Param);//增加模組檢測框-極性檢測
	bool                       AddModelDefaultWnd_BodyMissingKernel(const TMODEL_DEFAULT_WND_PARAM &Param);//增加模組檢測框-本體缺件
	bool                       AddModelDefaultWnd_BodyMissingKernel_v2(const TMODEL_DEFAULT_WND_PARAM &Param);//增加模組檢測框-本體缺件
	bool                       AddModelDefaultWnd_BodyTiltKernel(const TMODEL_DEFAULT_WND_PARAM &Param);//增加模組檢測框-本體傾斜		
	bool                       AddModelDefaultWnd_BodyTiltKernel_2(const TMODEL_DEFAULT_WND_PARAM &Param);//增加模組檢測框-本體傾斜-2框	
	bool                       AddModelDefaultWnd_BodyTiltKernel_4(const TMODEL_DEFAULT_WND_PARAM &Param);//增加模組檢測框-本體傾斜-4框	
	bool                       AddModelDefaultWnd_BodyTiltKernel_v2(const TMODEL_DEFAULT_WND_PARAM &Param);//增加模組檢測框-本體傾斜		
	bool                       AddModelDefaultWnd_BodyTiltKernel_v2_2(const TMODEL_DEFAULT_WND_PARAM &Param);//增加模組檢測框-本體傾斜		
	bool                       AddModelDefaultWnd_BodyTiltKernel_v2_4(const TMODEL_DEFAULT_WND_PARAM &Param);//增加模組檢測框-本體傾斜		
	bool                       AddModelDefaultWnd_BodyMountKernel(const TMODEL_DEFAULT_WND_PARAM &Param);//增加模組檢測框-本體裝貼
	bool                       AddModelDefaultWnd_BodyMountKernel_1(const TMODEL_DEFAULT_WND_PARAM &Param);//增加模組檢測框-本體裝貼-1框
	bool                       AddModelDefaultWnd_BodyMountKernel_2(const TMODEL_DEFAULT_WND_PARAM &Param);//增加模組檢測框-本體裝貼-2框
	bool                       AddModelDefaultWnd_BodyMountKernel_4(const TMODEL_DEFAULT_WND_PARAM &Param);//增加模組檢測框-本體裝貼-4框
	bool                       AddModelDefaultWnd_BodyMountKernel_v2(const TMODEL_DEFAULT_WND_PARAM &Param);//增加模組檢測框-本體裝貼
	bool                       AddModelDefaultWnd_TextWrongKernel(const TMODEL_DEFAULT_WND_PARAM &Param);//增加模組檢測框-錯件檢測	
	bool                       AddModelDefaultWnd_BodyDamagedKernel(const TMODEL_DEFAULT_WND_PARAM &Param);//增加模組檢測框-破損檢測	
	bool                       AddModelDefaultWnd_BodyDamagedKernel_v2(const TMODEL_DEFAULT_WND_PARAM &Param);//增加模組檢測框-破損檢測	
	bool                       AddModelDefaultWnd_OuterShortKernel(const TMODEL_DEFAULT_WND_PARAM &Param);//增加模組檢測框-外接短路檢測
	bool                       AddModelDefaultWnd_OuterShortKernelFn(const TMODEL_DEFAULT_WND_PARAM &Param, bool bUse3D);//增加模組檢測框-外接短路檢測
	bool                       AddModelDefaultWnd_OuterShortKernelFn_v2(const TMODEL_DEFAULT_WND_PARAM &Param, bool bUse3D);//增加模組檢測框-外接短路檢測
	bool                       AddModelDefaultWnd_ForeignBodyKernel(const TMODEL_DEFAULT_WND_PARAM &Param);//增加模組檢測框-異物檢測
	bool                       AddModelDefaultWnd_OtherDefectKernel(const TMODEL_DEFAULT_WND_PARAM &Param);//增加模組檢測框-其餘瑕疵檢測	
	bool                       AddModelDefaultWnd_DefaultModelDefectKernel(const TMODEL_DEFAULT_WND_PARAM &Param);//增加模組檢測框-預設模組全瑕疵
	bool                       AddModelDefaultWnd_LeadLiftedKernel(const TMODEL_DEFAULT_WND_PARAM &Param, int LandGroupID);//增加模組檢測框-腳翹檢測
	bool                       AddModelDefaultWnd_LeadBendedKernel(const TMODEL_DEFAULT_WND_PARAM &Param, int LandGroupID);//增加模組檢測框-腳歪檢測
	bool                       AddModelDefaultWnd_SolderOpenKernel(const TMODEL_DEFAULT_WND_PARAM &Param, int LandGroupID);//增加模組檢測框-空焊檢測
	bool                       AddModelDefaultWnd_SolderOpenKernel_v2(const TMODEL_DEFAULT_WND_PARAM &Param, int LandGroupID);//增加模組檢測框-空焊檢測
	bool                       AddModelDefaultWnd_SolderOpenKernel_v2(const TMODEL_DEFAULT_WND_PARAM &Param, int LandGroupID, bool bUseSide);//增加模組檢測框-空焊檢測
	bool                       AddModelDefaultWnd_SolderPoorKernel(const TMODEL_DEFAULT_WND_PARAM &Param, int LandGroupID);//增加模組檢測框-少焊檢測
	bool                       AddModelDefaultWnd_SolderPoorKernel_v2(const TMODEL_DEFAULT_WND_PARAM &Param, int LandGroupID);//增加模組檢測框-少焊檢測
	bool                       AddModelDefaultWnd_SolderPoorKernel_v2(const TMODEL_DEFAULT_WND_PARAM &Param, int LandGroupID, bool bUseSide);//增加模組檢測框-少焊檢測
	bool                       AddModelDefaultWnd_SolderPadExposedKernel(const TMODEL_DEFAULT_WND_PARAM &Param, int LandGroupID);//增加模組檢測框-焊盤露出
	bool                       AddModelDefaultWnd_PadScratchKernel(const TMODEL_DEFAULT_WND_PARAM &Param, int LandGroupID);//增加模組檢測框-刮傷檢測	
	bool                       AddModelDefaultWnd_LandBridgeKernel(const TMODEL_DEFAULT_WND_PARAM &Param, int LandGroupID);//增加模組檢測框-短路檢測
	bool                       AddModelDefaultWnd_LandBridgeKernelFn(const TMODEL_DEFAULT_WND_PARAM &Param, int LandGroupID, bool bUse3D);//增加模組檢測框-短路檢測
	bool                       AddModelDefaultWnd_LandBridgeKernelFn_v2(const TMODEL_DEFAULT_WND_PARAM &Param, int LandGroupID, bool bUse3D);//增加模組檢測框-短路檢測
	bool                       AddModelDefaultWnd_LandOuterShortKernel(const TMODEL_DEFAULT_WND_PARAM &Param, int LandGroupID);//增加模組檢測框-焊盤外接短路檢測	
	bool                       AddModelDefaultWnd_LandOuterShortKernelFn(const TMODEL_DEFAULT_WND_PARAM &Param, int LandGroupID, bool bUse3D);//增加模組檢測框-焊盤外接短路檢測	
	bool                       AddModelDefaultWnd_LandOuterShortKernelFn_v2(const TMODEL_DEFAULT_WND_PARAM &Param, int LandGroupID, bool bUse3D);//增加模組檢測框-焊盤外接短路檢測	
	//---------------------------------------------------------------------------------//	
	bool                       ExecModelInspectionKernel(std::vector<TUNI_FRAME> &UniFrameList);//執行模組檢測
	bool                       ExecModelInspectionKernel_SP(std::vector<TUNI_FRAME> &UniFrameList);//執行模組檢測//不再使用
	bool                       ExecModelInspectionKernel_MP(std::vector<TUNI_FRAME> &UniFrameList, bool bOpenMP);//執行模組檢測	
	bool                       ExecModelInspectionKernel_MP_Rotated(std::vector<TUNI_FRAME> &UniFrameList, bool bOpenMP);//執行模組檢測(檢測框跟隨旋轉)
	bool                       ExecModelInspectionKernel_Fn(TREGION4D &ModelRgn, TPOINT2D &RgnCp, TPOINT2D &Scale, TPOINT2D &ImageCp, std::vector<TUNI_FRAME> &UniFrameList, size_t Start, size_t End, int ActClassID, int nOpenMPCnt);//執行模組檢測	 
	bool                       ExecModelInspectionKernel_Fn(std::map<WND_FOLLOW_MODE, WndInspectionParameter> WndParameterMap, size_t Start, size_t End, int ActClassID, int nOpenMPCnt);
	bool                       CheckModelWndOrderListSupportOpenMP(size_t Start, size_t End);//確認檢測框列表支援OpenMP計算
	bool                       ExecModelInspectionKernelOffsetWnd_Fn(size_t Start, size_t End);//執行模組檢測-座標補正
	bool                       ExecModelInspectionKernelBaseValue_Fn(size_t Start, size_t End);//執行模組檢測-基準值更新
	bool                       ExecModelInspectionKernelClassCheck_Fn(size_t Start, size_t End, int &ActClassID);//執行模組檢測-類別確認
	//---------------------------------------------------------------------------------//	
	bool                       CalcModelTotalRegionKernel();
	bool                       CalcModelTotalRegionCadKernel();
	bool                       CalcModelTotalRegionStageKernel();
	bool                       CalcModelTotalRegionExtend(TREGION4D &Rgn);//計算模組的額外外擴範圍	
	bool                       UpdateModelTotalRegionToAttached();//更新模組的全區域至零件位置上
	bool                       CalcModelWndMaxExtendRange(double &ExtendX, double &ExtendY);//計算模組內檢測框最大外擴範圍
	//---------------------------------------------------------------------------------//	
	bool                       SynchronousModelKernel(const CAOIModel *RefModelPtr, bool bPartial);//同步化模組, 需要相同框數量下
	//---------------------------------------------------------------------------------//	
	bool                       AddModelWndRoiWndKernel(CAOIWnd *WndPtr);//新增模組檢測框內的子框
	bool                       AddModelWndRoiWndKernel(CAOIWnd *WndPtr, const TREGION4D &RoiRgn);//新增模組檢測框內的子框
	//---------------------------------------------------------------------------------//	
	bool                       AddModelWndMaskWndKernel(CAOIWnd *WndPtr);//新增模組檢測框內的遮罩框
	bool                       ClonePasteModelWndMaskWndKernel(CAOIWnd *WndPtr, CAOIWndMask *WndMaskPtr);//複製貼上模組檢測框內的遮罩框	
	//---------------------------------------------------------------------------------//	
public:
	//---------------------------------------------------------------------------------//
	CAOIModel();
	CAOIModel(const CAOIModel &Model);
	virtual ~CAOIModel();
	CAOIModel& operator=(const CAOIModel &Mode);
	//---------------------------------------------------------------------------------//
	CAOIModel* CloneModelObj() const;//建立且複製一個模組
	bool                       InitModelWndOrderList_Clone();
	//---------------------------------------------------------------------------------//
	bool                       CloneModelInfo(TModelInfo &ModelInfo);//複製模組訊息
	//---------------------------------------------------------------------------------//	
	bool                       WriteModelFile(CAOIFileIO &FileIO);//儲存模組檔案
	bool                       ReadModelFile(CAOIFileIO &FileIO);//載入模組檔案
	//---------------------------------------------------------------------------------//
	bool                       WriteModelSpcHeader_JSON_detail(FILE *pfile, CAOIProject *ProjectPtr);
	//---------------------------------------------------------------------------------//
	bool                       BuildModelWndParamStringList(LPCTSTR Title, std::vector<CString> &strList);//建立模組檢測框列表
	//---------------------------------------------------------------------------------//
	CString                    GetModelDefaultFilename();//取得預設模組名稱	
	CString                    GetModelDefaultImageFilename();//取得預設模組影像名稱
	//---------------------------------------------------------------------------------//
	bool                       SaveModelParamFile(LPCTSTR pfilename);//儲存模組參數檔案
	bool                       LoadModelParamFile(LPCTSTR pfilename);//儲存模組參數檔案
	//---------------------------------------------------------------------------------//
	void                       SetModelType(MODEL_TYPE value) { m_ModelType=value; }	
	MODEL_TYPE                 GetModelType() const { return m_ModelType; }	
	bool                       CheckModelTypeEnabled() const;
	//---------------------------------------------------------------------------------//		
	void                       SetModelIndex(unsigned int value) { m_ModelIndex = value; }
	unsigned int               GetModelIndex() const { return m_ModelIndex; }
	//---------------------------------------------------------------------------------//	
	//模組啟用類別編號 
	void                       SetModelClassID(int value) { m_ModelClassID = value; }
	int                        GetModelClassID() const { return m_ModelClassID; }
	//---------------------------------------------------------------------------------//
	//模組啟用類別編號 
	void                       SetModelActClassID(int value) { m_ModelActClassID = value; }
	int                        GetModelActClassID() const { return m_ModelActClassID; }
	//---------------------------------------------------------------------------------//
	void                       SetModelSelected(bool value) { m_ModelSelected = value; }
	bool                       GetModelSelected() const { return m_ModelSelected; }
	//---------------------------------------------------------------------------------//
	bool                       GetModelEditMode() const;//取得模組是否編輯模式
	bool                       GetModelUnsetState() const;//取得模組是否為設定狀態
	//---------------------------------------------------------------------------------//
	//模組被動元件尺寸模式
	void                       SetModelChipSizeMode(CHIP_SIZE_MODE value) { m_ModelChipSizeMode = value; }
	CHIP_SIZE_MODE             GetModelChipSizeMode() const { return m_ModelChipSizeMode; }
	//---------------------------------------------------------------------------------//	
	//使用3D
	void                       UpdateModelUsing3D();
	void                       SetModelUsing3D(bool value) { m_ModelUsing3D = value; }
	bool                       GetModelUsing3D() const { return m_ModelUsing3D; }	
	//---------------------------------------------------------------------------------//			
	//3D不檢測 
	void                       UpdateModelBypass3D();
	void                       SetModelBypass3D(bool value) { m_ModelBypass3D = value; }
	bool                       GetModelBypass3D() const { return m_ModelBypass3D; }	
	//---------------------------------------------------------------------------------//		
	void                       SetModelIsolated(bool value) { m_ModelIsolated = value; }
	bool                       GetModelIsolated() const { return m_ModelIsolated; }
	//---------------------------------------------------------------------------------//	
	//模組自我測試-非零件,條碼, 定位點檢測
	void                       SetModelSelfTest(bool value) { m_ModelSelfTest = value; }
	bool                       GetModelSelfTest() const { return m_ModelSelfTest; }
	//---------------------------------------------------------------------------------//	
	//模組儲存引腳報告
	void                       SetModelSaveLeadReport(bool value) { m_ModelSaveLeadReport = value; }
	bool                       GetModelSaveLeadReport() const { return m_ModelSaveLeadReport; }
	//---------------------------------------------------------------------------------//		
	//模組本體尺寸連動被動元件電極
	bool                       CheckModelBodyLinkChipLead() const;//確認模組本體與被動元件電極框連動	
	void                       SetModelBodyLinkChipLead(bool value) { m_ModelBodyLinkChipLead = value; }
	bool                       GetModelBodyLinkChipLead() const { return m_ModelBodyLinkChipLead; }
	//---------------------------------------------------------------------------------//			
	//模組的暫存整數
	void                       SetModelTempInt1(int value) { m_ModelTempInt[0] = value; }
	int                        GetModelTempInt1() const { return m_ModelTempInt[0]; }
	void                       SetModelTempInt2(int value) { m_ModelTempInt[1] = value; }
	int                        GetModelTempInt2() const { return m_ModelTempInt[1]; }
	void                       SetModelTempInt3(int value) { m_ModelTempInt[2] = value; }
	int                        GetModelTempInt3() const { return m_ModelTempInt[2]; }
	void                       SetModelTempInt4(int value) { m_ModelTempInt[3] = value; }
	int                        GetModelTempInt4() const { return m_ModelTempInt[3]; }	
	void                       SetModelTempInt(int value, int idx=0) { m_ModelTempInt[idx] = value; }
	int                        GetModelTempInt(int idx=0) const { return m_ModelTempInt[idx]; }	
	//---------------------------------------------------------------------------------//		
	//模組的檢測結果-警報
	void                       SetModelResultID(RESULT_ID value) { m_ModelResultID = value; }
	RESULT_ID                  GetModelResultID() const { return m_ModelResultID; }
	//---------------------------------------------------------------------------------//
	//模組的檢測結果-警報
	void                       SetModelResultID_Alarm(RESULT_ID value) { m_ModelResultID_Alarm = value; }
	RESULT_ID                  GetModelResultID_Alarm() const { return m_ModelResultID_Alarm; }
	//---------------------------------------------------------------------------------//
	//模組要儲存檔案
	void                       SetModelNeedSaveFiles(bool value) { m_ModelNeedSaveFiles = value; }
	bool                       GetModelNeedSaveFiles() const { return m_ModelNeedSaveFiles; }
	//---------------------------------------------------------------------------------//	
	//模組的檢測耗時
	void                       SetModelInspectedTime(double value) { m_ModelInspectedTime = value; }
	double                     GetModelInspectedTime() const { return m_ModelInspectedTime; }
	//---------------------------------------------------------------------------------//	
	bool                       CheckModelModified();//確認模組修改過參數	
	//---------------------------------------------------------------------------------//	
	//模組的修改日期
	void                       SetupkModelModifiedDateTime();//標誌模組修改時間
	void                       SetModelModifiedDateTime(time_t value) { m_ModelModifiedDateTime = value; }
	time_t                     GetModelModifiedDateTime() const { return m_ModelModifiedDateTime; }
	//---------------------------------------------------------------------------------//		
	//模組修改過數量
	void                       SetModelModifiedCount(bool value) { m_ModelModifiedCount = value; }
	bool                       GetModelModifiedCount() const { return m_ModelModifiedCount; }
	//---------------------------------------------------------------------------------//
	CWndDefectItem             CheckModelTestDefectItems() const;//確認模組檢測瑕疵項目
	CWndDefectItem             CheckModelEnabledDefectItems() const;//確認模組檢測瑕疵項目
	CWndDefectItem             CheckModelBypassedDefectItems() const;//確認模組不檢測瑕疵項目
	//---------------------------------------------------------------------------------//
	//模組瑕疵檢測項目模式
	const CWndDefectItem&      GetModelDefectItemTest() const { return m_ModelDefectItemTest; }
	void                       SetModelDefectItemTest(const CWndDefectItem &value) { m_ModelDefectItemTest = value; }	
	//---------------------------------------------------------------------------------//	
	//模組瑕疵警報項目模式
	bool                       UpdateModelWndDefectItemAlarm();//更新模組檢測框瑕疵警報
	const CWndDefectItem&      GetModelDefectItemAlarm() const { return m_ModelDefectItemAlarm; }
	void                       SetModelDefectItemAlarm(const CWndDefectItem &value) { m_ModelDefectItemAlarm = value; }	
	//---------------------------------------------------------------------------------//	
	//模組瑕疵必要檢測項目	
	bool                       CheckModelDefectItemEssential(std::vector<WND_DEFECT_ID> &BadList) const;
	const CWndDefectItem&      GetModelDefectItemEssential() const { return m_ModelDefectItemEssential; }
	void                       SetModelDefectItemEssential(const CWndDefectItem &value) { m_ModelDefectItemEssential = value; }	
	//---------------------------------------------------------------------------------//	
	//模組瑕疵重複確認項目-ARS	
	const CWndDefectItem&      GetModelDefectItemRecheck_ARS() const { return m_ModelDefectItemRecheck_ARS; }
	void                       SetModelDefectItemRecheck_ARS(const CWndDefectItem &value) { m_ModelDefectItemRecheck_ARS = value; }	
	//---------------------------------------------------------------------------------//	
	void                       SetModelName(const char* value);
	void                       SetModelName(const wchar_t* value);
	const wchar_t*             GetModelName() const { return m_ModelName.c_str(); }
	//---------------------------------------------------------------------------------------//			
	void                       SetModelGroupName(const char* value);
	void                       SetModelGroupName(const wchar_t* value);
	const wchar_t*             GetModelGroupName() const { return m_ModelGroupName.c_str(); }
	//---------------------------------------------------------------------------------------//		
	void                       SetModelLandDirection(MODEL_LAND_DIRECTION value) { m_ModelLandDirection = value; }
	MODEL_LAND_DIRECTION       GetModelLandDirection() const { return m_ModelLandDirection; }
	//---------------------------------------------------------------------------------------//
	//模組焊盤調整模式
	void                       SetModelPadAdjustMode(MODEL_LAND_ADJUST_MODE value) { m_ModelPadAdjustMode = value; }
	MODEL_LAND_ADJUST_MODE     GetModelPadAdjustMode() const { return m_ModelPadAdjustMode; }
	//---------------------------------------------------------------------------------------//
	//模組引腳調整模式
	void                       SetModelLeadAdjustMode(MODEL_LAND_ADJUST_MODE value) { m_ModelLeadAdjustMode = value; }
	MODEL_LAND_ADJUST_MODE     GetModelLeadAdjustMode() const { return m_ModelLeadAdjustMode; }
	//---------------------------------------------------------------------------------------//
	//模組檢測框同步移動模式
	void                       SetModelWndSyncMoveMode(WND_SYNC_MOVE_MODE value) { m_ModelWndSyncMoveMode = value; }
	WND_SYNC_MOVE_MODE         GetModelWndSyncMoveMode() const { return m_ModelWndSyncMoveMode; }
	//---------------------------------------------------------------------------------------//
	void                       SetModelAutoDeleteImageFolder(bool value) { m_ModelAutoDeleteImageFolder = value; }
	bool                       GetModelAutoDeleteImageFolder() const { return m_ModelAutoDeleteImageFolder; }
	//---------------------------------------------------------------------------------//
	void                       SetModelFolderModel(LPCTSTR value) { m_ModelFolderModel = value; }
	LPCTSTR                    GetModelFolderModel() const { return m_ModelFolderModel; }
	//---------------------------------------------------------------------------------------//
	void                       SetModelFolderComponent(LPCTSTR value) { m_ModelFolderComponent = value; }
	LPCTSTR                    GetModelFolderComponent() const { return m_ModelFolderComponent; }
	//---------------------------------------------------------------------------------------//	
	void                       SetModelUsedCount(unsigned int value) { m_ModelUsedCount = value; }
	unsigned int               GetModelUsedCount() const { return m_ModelUsedCount; }
	void                       IncreaseModelUsedCount() { m_ModelUsedCount++; }
	//---------------------------------------------------------------------------------------//
	bool                       ArrangeModelBKImageFiles();//重新整理模組底圖
	//---------------------------------------------------------------------------------------//	
	//模組底圖影像編號
	void                       SetModelBKImageIndex(unsigned int value) { m_ModelBKImageIndex = value; }
	unsigned int               GetModelBKImageIndex() const { return m_ModelBKImageIndex; }
	//---------------------------------------------------------------------------------------//
	void                       SetModelBKImageNeedToGrab(bool value) { m_ModelBKImageNeedToGrab = value; }
	bool                       GetModelBKImageNeedToGrab() const { return m_ModelBKImageNeedToGrab; }
	//---------------------------------------------------------------------------------------//		
	//模組檢測框連動模式
	bool                       GetModelLinkModeWndPos() const;//模組檢測框位置同動	
	bool                       GetModelLinkModeWndSize() const;//模組檢測框尺寸同動
	bool                       GetModelLinkModeLandPos() const;//模組特徵框位置同動
	bool                       GetModelLinkModeLandSize() const;//模組特徵框尺寸同動
	bool                       GetModelLinkModeLandWndPos() const;//模組特徵檢測框位置同動
	bool                       GetModelLinkModeLandWndSize() const;//模組特徵檢測框尺寸同動

	void                       EnableModelLinkModeWndPos(bool bEnable);//啟用模組檢測框位置同動
	void                       EnableModelLinkModeWndSize(bool bEnable);//啟用模組檢測框尺寸同動
	void                       EnableModelLinkModeLandPos(bool bEnable);//啟用模組特徵框位置同動
	void                       EnableModelLinkModeLandSize(bool bEnable);//啟用模組特徵框尺寸同動
	void                       EnableModelLinkModeLandWndPos(bool bEnable);//啟用模組特徵檢測框位置同動
	void                       EnableModelLinkModeLandWndSize(bool bEnable);//啟用模組特徵檢測框尺寸同動
	//---------------------------------------------------------------------------------------//
	bool                       CheckModelWndLinkMode(int LinkMode) const;//確認是否連動
	void                       AddModelWndLinkMode(int LinkMode) { m_ModelWndLinkMode |= LinkMode; }
	void                       RemoveModelWndLinkMode(int LinkMode);
	void                       SetModelWndLinkMode(int LinkMode) { m_ModelWndLinkMode = LinkMode; }
	int                        GetModelWndLinkMode() const { return m_ModelWndLinkMode; }
	//---------------------------------------------------------------------------------------//
	//模組使用的OpenMP數量
	void                       SetModelOpenMPCount(int val) { m_ModelOpenMPCount = val; }
	int                        GetModelOpenMPCount() const { return m_ModelOpenMPCount; }
	//---------------------------------------------------------------------------------------//
	LPCTSTR                    GetModelFolder() const;	
	bool                       AssignModelFolder();//指派模組資料夾至旗下演算法內	
	//---------------------------------------------------------------------------------------//	
	bool                       RotateModelBKImage(double Angle);//旋轉模組底圖
	CString                    GetModelBKImageFilename(int UniFrameIdx) const;
	//---------------------------------------------------------------------------------------//
	void                       GetModelRegion(TREGION4D &Region);
	double                     GetFitScale(const RECT &WndRect);
	//---------------------------------------------------------------------------------------//
	void                       UnSelectModel();
	void                       InvisibleModel();
	//---------------------------------------------------------------------------------------//	
	void                       MoveModel(double x, double y);
	void                       ScaleModel(double cx, double cy);
	void                       ScaleModelProperty(double sx, double sy);
	void                       RotateModel(double Angle, double CPX, double CPY);
	void                       MirrorModelXAxis(double CPY=0);
	void                       MirrorModelYAxis(double CPX=0);
	//---------------------------------------------------------------------------------------//
	void                       DrawModel(HDC hDC, DRAW_MODEL_MODE DrawMode, const TMODEL_DRAW_PARAM &DrawParam);	
	void                       DrawModelEdit(HDC hDC, const TMODEL_DRAW_PARAM &DrawParam);
	void                       DrawModelTemp(HDC hDC, const TMODEL_DRAW_PARAM &DrawParam);
	void                       DrawModelResult(HDC hDC, const TMODEL_DRAW_PARAM &DrawParam);	
	//---------------------------------------------------------------------------------------//
	//本體尺寸-X
	void                       SetModelBodySizeX(double val) { m_ModelBodySizeX=val; }
	double                     GetModelBodySizeX() const { return m_ModelBodySizeX;	}
	//---------------------------------------------------------------------------------------//
	//本體尺寸-Y
	void                       SetModelBodySizeY(double val) { m_ModelBodySizeY=val; }
	double                     GetModelBodySizeY() const { return m_ModelBodySizeY;	}
	//---------------------------------------------------------------------------------------//
	//模組本體高度
	void                       SetModelBodyHeight(double val) { m_ModelBodyHeight=val; }
	double                     GetModelBodyHeight() const { return m_ModelBodyHeight;	}
	//---------------------------------------------------------------------------------------//
	//模組零件本體影像位置-原始
	void                       SetModelBodyImageRect_Raw(const RECT &val) { m_ModelBodyImageRect_Raw=val; }
	const RECT&                GetModelBodyImageRect_Raw() const { return m_ModelBodyImageRect_Raw;	}	
	//---------------------------------------------------------------------------------------//	
	//模組本體顏色
	void                       SetModelBodyColorGroup(const CColorGroup &val) { m_ModelBodyColorGroup=val; }
	CColorGroup&               GetModelBodyColorGroup() { return m_ModelBodyColorGroup;	}
	const CColorGroup&         GetModelBodyColorGroup() const { return m_ModelBodyColorGroup;	}
	//---------------------------------------------------------------------------------------//
	//複製模組屬性參數
	bool                       CopyModelProperty(const CAOIModel *ModelPtr);
	//---------------------------------------------------------------------------------------//
	void                       SetModelBodyBoxActived(bool val);
	CAOIBox&                   GetModelBodyBox() { return m_ModelBodyBox;  }	
	CAOIBox*                   GetModelBodyBoxPtr() { return &m_ModelBodyBox;  }
	const CAOIBox&             GetModelBodyBox() const { return m_ModelBodyBox;  }
	const CAOIBox*             GetModelBodyBoxPtr() const { return &m_ModelBodyBox;  }
	void                       SetModelBodyRegion(const TREGION4D &Region);
	void                       GetModelBodyRegion(TREGION4D &Region);
	void                       GetModelBodyPos(TPOINT2D &Pos);
	void                       CalcModelBodyRegionNoLead(TREGION4D &Region);
	void                       ModifyModelBodyPos(double x, double y);
	void                       ModifyModelBodySize(double w, double h);	
	bool                       ModifyModelBodyRegion(double dMinX, double dMinY, double dMaxX, double dMaxY);	
	//---------------------------------------------------------------------------------------//	
	void                       ClearModelAllObjList();
	void                       ClearModelAllWndList();	
	void                       ClearModelAllDerivative();//刪除模組衍生物-僅保留基本項目
	//---------------------------------------------------------------------------------//		
	bool                       RemoveModelWndWithoutFrameIndex(const std::vector<unsigned int> &FrameIndexMapList);//移除模組檢測框-沒有影像引數
	bool                       UpdateModelFrameIndex(unsigned int DefaultIndex, unsigned int DefaultUniqueID, const std::vector<unsigned int> &FrameIndexMapList);//更新模組的畫面引數
	//---------------------------------------------------------------------------------//	
	void                       ClearModelLandList();
	size_t                     GetModelLandCount() const;
	void                       AddModelLandPtr_Direct(CAOILand *LandPtr);
	CAOILand*                  AddModelLandPtr(CAOILand *LandPtr, bool Clone);	
	CAOILand*                  CopyModelLand(const CAOILand *LandPtr);
	CAOILand*                  CreateModelLand(MODEL_TYPE ModelType, TREGION4D Region, LAND_TYPE LandType);
	CAOILand*                  CreateModelLand(MODEL_TYPE ModelType, int groupID, TREGION4D PadRegion, TREGION4D LeadRegion, LAND_TYPE LandType);
	CAOILand*                  CreateModelLand2(MODEL_TYPE ModelType, int groupID, TREGION4D PadRegion, TREGION4D Region, LAND_TYPE LandType);
	bool                       DestroyModelLandSelected();//刪除選中的特徵框
	CAOILand*                  GetModelLandPtr(size_t index, bool Check) const;
	CAOILand*                  GetModelLandPtrByUUID(const UUID &uuid) const;
	CAOILand*                  GetModelLandPtrByGroupID(int LandGroupID, int LandAlignID) const;	
	CAOILand*                  GetModelLandPtrByGroupID(int LandGroupID, int LandAlignID, BOX_TOWARD LandToward) const;	
	CAOILand*                  GetModelLandPtrByGroupID(int LandGroupID, int LandAlignID, BOX_TOWARD LandToward, bool bMaxWnd) const;	
	CAOILand*                  GetModelLandActivted() const;	
	size_t                     GetModelLandActivtedCount() const;	
	void                       SetModelLandActived(CAOILand *LandPtr);
	void                       SetModelLandSelected(bool val);
	CAOILand*                  GetModelLandSelected() const;	
	size_t                     GetModelLandSelectedCount() const;	
	bool                       GetModelLandSelectedList(std::vector<CAOILand*> &LandList);
	bool                       GetModelLandSelectedList(int LandGroupID, std::vector<CAOILand*> &LandList);
	bool                       GetModelLandSelectedIndexList(int LandGroupID, std::vector<size_t> &LandIndexList);
	bool                       GetModelLandSelectedLandGroupIDList(std::vector<int> &LandGroupIDList);
	bool                       ListModelLandAlignIDByGroupID(int LandGroupID, std::vector<int> &LandAlignIDList);
	bool                       ListModelLandGroupIDByLandType(LAND_TYPE LandType, std::vector<int> &LandGroupIDList);

	double                     CalcModelLandLeadAverageHeight(int LandGroupID);//計算模組引腳平均高度
	double                     CalcModelLandLeadTipAverageHeight(int LandGroupID);//計算模組引腳前端平均高度
	double                     CalcModelLandLeadShoulderAverageHeight(int LandGroupID);//計算模組引腳根部平均高度

	int                        GetModelLandFreeGroupID() const;	
	int                        GetModelLandFreeAlignID(int LandGroupID) const;
	void                       SetModelLandSelectedByLandGroupID(int LandGroupID, int LandAlignID, bool Selected);
	CAOILand*                  GetModelLandSelectedByLandGroupID(int LandGroupID, int LandAlignID) const;	
	void                       SetModelLandVisibledByLandGroupID(int LandGroupID, int LandAlignID, bool Visibled);
	void                       UnSelectModelLand(bool ToWnd);	
	void                       VisibleModelLand(bool ToWnd);
	void                       InvisibleModelLand(bool ToWnd);
	bool                       CheckModelLandModified();
	void                       SetModelLandModified(bool value);
	bool                       CheckModelLandValid(const CAOILand *LandPtr);//確認特徵框指標有效-屬於此模組內
	bool                       CheckModelLandOnlyOneGroupIDSelected() const;//確認只有一個特徵框群組被選到
	
	bool                       ApplyModelLandSize(CAOILand *RefLandPtr);//同步化特徵框的尺寸
	bool                       ApplyModelLandRegion(CAOILand *RefLandPtr);//同步化特徵框的位置尺寸
	bool                       ApplyModelLandWndLogic(CAOILand *RefLandPtr);//同步化特徵框的檢測框與邏輯閘	
	bool                       LayoutModelLandByLandGroupID(int LandGroupID, int LandAlignID);
	bool                       AlignModelLandByLandGroupID(int LandGroupID, int LandAlignID);	
	bool                       DeleteModelLandIndexList(std::vector<size_t> &LandIndexList);	
	//---------------------------------------------------------------------------------//		
	int                        GetModelLandCountByToward(CAOILand *RefLandPtr, bool bActiveOnly);	
	double                     GetModelLandPitchByToward(CAOILand *RefLandPtr, bool bActiveOnly);
	void                       UpdateModelLandLeadID();//更新引腳編號	
	bool                       ModifyModelLandAlign(bool SelectedOnly);	
	bool                       ModifyModelLandCount(CAOILand *RefLandPtr, size_t LandCount, bool bArray);
	bool                       ModifyModelLandIncludePadAlign(CAOILand *RefLandPtr, bool bInclude);
	bool                       ModifyModelLandIncludePartAlign(CAOILand *RefLandPtr, bool bInclude);		
	bool                       ModifyModelLandPitch(CAOILand *RefLandPtr, double LandPitch, bool SelectedOnly);	
	bool                       ModifyModelLandGroupID(const std::vector<size_t> &LandIndexList, int NewLandGroupID);//修正群組編號
	bool                       LinkModelLandAlignID(const std::vector<size_t> &LandIndexList, int LandGroupID);//連動對齊編號
	bool                       UnLinkModelLandAlignID(const std::vector<size_t> &LandIndexList, int LandGroupID);//不連動對齊編號
	bool                       ModifyModelLandAlignID(const std::vector<size_t> &LandIndexList, int LandGroupID, int NewLandAlignID);//修正對齊編號
	bool                       GetModelLandIndexList(CAOILand *RefLandPtr, bool bSelected, std::vector<size_t> &LandIndexList, double &Pitch, double &MaxPitch);//取得特定方向的焊盤數量			
	//---------------------------------------------------------------------------------//		
	bool                       DeleteModelLandSelected();
	bool                       CloneModelLandSelected();
	bool                       CloneMoveModelLandSelected(double MoveX, double MoveY, bool NewAlign);	
	bool                       AlignCenterUModelLandSelected();		
	bool                       SpinModelLandSelected(double Angle);//自轉模組選到的特徵框
	bool                       MirrorXModelLandSelected(double CPX, double CPY);
	bool                       MirrorYModelLandSelected(double CPX, double CPY);		
	bool                       RotateModelLandSelected(double Angle, double CPX, double CPY);		
	bool                       AddModelWndToOtherLand(CAOILand *RefLandPtr, CAOIWnd *RefWndPtr, int NewWndBandID);
	bool                       AddModelWndToOtherLandKernel(CAOILand *RefLandPtr, CAOIWnd *RefWndPtr, int NewWndBandID);
	bool                       AddModelLogicToOtherLand(CAOILand *RefLandPtr, CAOILogic *RefLogicPtr);
	bool                       ModifyModelLogicToOtherLand(CAOILand *RefLandPtr, CAOILogic *RefLogicPtr);
	void                       UpdateModelLandTotalRegion();
	bool                       CalcModelPadResultOffsetCad(double &OffsetX, double &OffsetY);//計算模組焊盤結果偏移量
	void                       GetModelPadRegion(int LandGroupID, int LandAlignID, TREGION4D &Region);//取得模組特徵框範圍
	void                       GetModelLeadRegion(int LandGroupID, int LandAlignID, TREGION4D &Region);//取得模組特徵框範圍
	void                       GetModelLandRegion(int LandGroupID, int LandAlignID, TREGION4D &Region);//取得模組特徵框範圍
	void                       GetModelLeadTipRegion(int LandGroupID, int LandAlignID, TREGION4D &Region);//取得模組特徵框範圍
	void                       GetModelLeadShoulderRegion(int LandGroupID, int LandAlignID, TREGION4D &Region);//取得模組特徵框範圍
	void                       GetModelLeadShoulderTipRegion(int LandGroupID, int LandAlignID, TREGION4D &Region);//取得模組特徵框範圍
	//---------------------------------------------------------------------------------//
	void                       ClearModelWndList();	
	size_t                     GetModelWndCount() const;
	bool                       InitialModelWndPtr(CAOIWnd *WndPtr);//將特定模組參數帶入檢測框
	void                       AddModelWndPtr_Direct(CAOIWnd *WndPtr);	
	CAOIWnd*                   AddModelWndPtr(CAOIWnd *WndPtr, bool Clone);	
	CAOIWnd*                   CopyModelWnd(const CAOIWnd *WndPtr);
	CAOIWnd*                   CreateModelWnd(WND_DEFECT_ID WndDefectID, TREGION4D Region, CAOILand *LandPtr);
	bool                       DestroyModelWndSelected();
	CAOIWnd*                   GetModelWndPtr(size_t index, bool Check) const;
	CAOIWnd*                   GetModelWndPtrByUUID(const UUID &uuid) const;		
	CAOIWnd*                   GetModelWndPtrByDefectID(WND_DEFECT_ID WndDefectID, int DefectGroupID=-1) const;		
	CAOIWnd*                   GetModelWndPtrByGroupID(int WndGroupID, int BandID, bool NoIsolatedWnd) const;	
	CAOIWnd*                   GetModelWndPtrByGroupBandID(int WndGroupID, int BandID, bool NoIsolatedWnd) const;	
	CAOIWnd*                   GetModelWndPtrByLandDefectID(const CAOILand *LandPtr, WND_DEFECT_ID WndDefectID, int DefectGroupID) const;	
	CAOIWnd*                   GetModelWndFirst() const;	
	CAOIWnd*                   GetModelWndActived() const;	
	size_t                     GetModelWndActivedCount() const;
	CAOIWnd*                   GetModelWndDefectMaster() const;
	void                       SetModelWndActived(CAOIWnd *WndPtr);
	void                       SetModelWndSelected(bool val);
	CAOIWnd*                   GetModelWndSelected() const;			
	int                        GetModelWndSelectedCount() const;	
	bool                       PasteModelWndList(std::vector<CAOIWnd*> WndList);//貼上模組選取到檢測匡列表	
	void                       GetModelWndSelectedList(std::vector<CAOIWnd*> &WndList);//取得模組選取到檢測匡列表	
	bool                       UpdateModelWndBypassed(const CWndDefectItem &DefectEnabled);//更新模組檢測框是否忽略
	void                       SetModelWndOtherGroupIDSelected(int WndGroupID, bool Selected);//設定模組其餘群組檢測框選取狀態		
	bool                       SetModelWndSelectedIndexList(const std::vector<unsigned int> &WndIndexList);	
	bool                       GetModelWndSelectedIndexList(int WndGroupID, int WndBandID, std::vector<unsigned int> &WndIndexList);
	bool                       GetModelWndSelectedWndGroupIDList(std::vector<int> &WndGroupIDList);	
	bool                       ListModelWndBandIDByGroupID(int WndGroupID, std::vector<int> &WndBandIDList);
	bool                       ListModelWndGroupIDByDefectID_AlgType(WND_DEFECT_ID WndDefectID, ALG_TYPE AlgType, std::vector<int> &WndGroupIDList);
	
	int                        GetModelWndFreeGroupID() const;	
	int                        GetModelWndFreeBandID() const;		
	int                        GetModelWndFreeBandID(int WndGroupID) const;
	bool                       CheckModelWndGroupEnabled(int WndGroupID) const;
	RESULT_ID                  CheckModelWndGroupResultID(int WndGroupID) const;	
	RESULT_ID                  CheckModelWndGroupLogicResultID(int WndGroupID) const;
	int                        GetModelFreeDefectGroupID(WND_DEFECT_ID DefectID) const;//取得模組檢測框瑕疵群組編號
	void                       SetModelWndSelectedByWndGroupID(int WndGroupID, int WndBandID, bool Selected);
	CAOIWnd*                   GetModelWndSelectedByWndGroupID(int WndGroupID, bool NoIsolatedWnd) const;
	void                       SetModelWndVisibledByWndGroupID(int WndGroupID, int WndBandID, bool Visibled);
	void                       SetModelWndVisibledByWndUniqueID(int LandGroupID, int WndGroupID, int WndBandID, bool Visibled);
	bool                       LayoutModelWndList();//重新排序檢測框列表
	bool                       LayoutModelWndListByDefectID();//重新排序檢測框列表
	void                       VisibleModelWnd();
	void                       UnSelectModelWnd();	
	void                       InvisibleModelWnd();	
	bool                       CheckModelWndModified();//確認模組檢測框是否編輯過	
	void                       SetModelWndModified(bool value);//設定模組檢測框是否編輯過	
	void                       ApplyModelWnd(CAOIWnd *RefWndPtr);//套用檢測框至其它相同檢測框	
	
	bool                       SetModelWndEnabled(bool bEnabled);//設定模組所有檢測框啟用
	bool                       BypassSkipModelWnd(RESULT_ID value);//不檢測或跳過模組檢測框
	bool                       CheckModelWndValid(const CAOIWnd *WndPtr);//確認檢測框指標有效-屬於此模組內	
	bool                       SetModelWndEnabledByWndGroupID(int WndGroupID, bool bEnabled);	
	bool                       SetModelWndAlgTypeByWndGroupID(int WndGroupID, ALG_TYPE AlgType);
	bool                       SetModelWndResultID(RESULT_ID ResultID);//設定模組檢測框結果編號-bypass
	bool                       RemoveModelAlgPatternFolderBySelected();//移除模組演算法樣板資料夾
	bool                       SetModelWndShapeMode(CAOIWnd *WndPtr, BOX_SHAPE_MODE BoxShapeMode);//設定模組檢測框外形	
	bool                       SetModelWndShapeParam(CAOIWnd *WndPtr, double BoxShapeParam);//設定模組檢測框外形參數-1
	bool                       SetModelWndShapeParam2(CAOIWnd *WndPtr, double BoxShapeParam2);//設定模組檢測框外形參數-2
	bool                       GetModelLandWndPtrByLandDefectID(const CAOILand *RefLandPtr, WND_DEFECT_ID WndDefectID, int DefectGroupID, CAOILand *&LandPtr, CAOIWnd *&WndPtr) const;
	//---------------------------------------------------------------------------------//	
	bool                       ResetModelWndAlgParam();//復歸模組檢測框的演算法
	//---------------------------------------------------------------------------------//	
	bool                       ModifyModelOtherLandWndRegion(CAOIWnd *RefWndPtr);	
	bool                       ModifyModelWndGroupIDUp();//上升選取到的檢測框群組
	bool                       ModifyModelWndGroupIDDownd();//下降選取到的檢測框群組
	bool                       ModifyModelWndBandID(const std::vector<unsigned int> &WndIndexList, int WndGroupID, int NewWndBandID);//修正檢測框群組
	bool                       ModifyModelWndGroupID(const std::vector<unsigned int> &WndIndexList, int NewWndGroupID, int NewBandID);//修正檢測框群組
	//---------------------------------------------------------------------------------//	
	bool                       DeleteModelWndSelected(bool LinkMode);//刪除模組內的檢測框
	bool                       DeleteModelWndWithClassID(int ClassID);//刪除模組內的檢測框(類別編號)
	bool                       CloneModelWndSelected(bool LinkMode);
	bool                       CloneModelWndWithClassID(int ClassID);//複製模組內的檢測框(類別編號)
	bool                       AlignCenterModelWndSelected();//對齊模組檢測框
	bool                       AlignCenterUModelWndSelected();//對齊模組檢測框-U
	bool                       AlignCenterVModelWndSelected();//對齊模組檢測框-V
	bool                       MoveModelWndSelected(BOX_TOWARD Toward, double dX, double dY);	
	bool                       MirrorXModelWndSelected(double CPX, double CPY);	
	bool                       MirrorYModelWndSelected(double CPX, double CPY);	
	bool                       SpinModelWndSelected(double Angle);
	bool                       RotateModelWndSelected(double Angle, double CPX, double CPY);		
	void                       UpdateModelWndRgnByLinkMode();
	bool                       DeleteModelWndIndexList(std::vector<unsigned int> &WndIndexList);
	//---------------------------------------------------------------------------------------//
	bool                       AddModelWndRoiWnd(CAOIWnd *WndPtr);//新增模組檢測框內的子框
	bool                       AddModelWndRoiWnd(CAOIWnd *WndPtr, const TREGION4D &RoiRgn);//新增模組檢測框內的子框
	bool                       DeleteModelWndRoiWnd(CAOIWnd *WndPtr);//刪除模組檢測框內的子框
	bool                       ClearModelWndRoiWndList(CAOIWnd *WndPtr);//清除模組檢測框內的子框
	//---------------------------------------------------------------------------------------//
	bool                       AddModelWndMaskWnd(CAOIWnd *WndPtr);//新增模組檢測框內的遮罩框
	bool                       SpinModelWndMaskWndSelected(CAOIWnd *WndPtr, double Angle);//自轉模組檢測框內的遮罩框
	bool                       RotateModelWndMaskWndSelected(CAOIWnd *WndPtr, double Angle);//旋轉模組檢測框內的遮罩框
	bool                       MirrorXModelWndMaskWndSelected(CAOIWnd *WndPtr);	//鏡射模組檢測框內的遮罩框	
	bool                       MirrorYModelWndMaskWndSelected(CAOIWnd *WndPtr);	//鏡射模組檢測框內的遮罩框	
	bool                       MoveModelWndMaskWndSelected(CAOIWnd *WndPtr, const TPOINT2D &Move);	//移動模組檢測框內的遮罩框	
	bool                       ClonePasteModelWndMaskWnd(CAOIWnd *WndPtr, CAOIWndMask *WndMaskPtr);//複製貼上模組檢測框內的遮罩框
	bool                       SetModelWndMaskWndEraseMode(CAOIWnd *WndPtr, bool bEraseMode);//設定模組檢測框內的遮罩框-清除遮罩模式
	bool                       SetModelWndMaskWndShapeMode(CAOIWnd *WndPtr, BOX_SHAPE_MODE BoxShapeMode);//設定模組檢測框內的遮罩框
	bool                       SetModelWndMaskWndShapeParam(CAOIWnd *WndPtr, double ShapeParam);//設定模組檢測框內的遮罩框外形參數
	bool                       SetModelWndMaskWndShapeParam2(CAOIWnd *WndPtr, double ShapeParam2);//設定模組檢測框內的遮罩框外形參數
	bool                       DeleteModelWndMaskWnd(CAOIWnd *WndPtr);//刪除模組檢測框內的遮罩框	
	bool                       ClearModelWndMaskWndList(CAOIWnd *WndPtr);//清除模組檢測框內的遮罩框
	//---------------------------------------------------------------------------------------//
	int                        GetModelAlgFreeGroupID() const;
	//---------------------------------------------------------------------------------------//
	size_t                     GetModelLogicCount() const;
	CAOILogic*                 AddModelLogicPtr(CAOILogic *LogicPtr, bool Clone);	
	CAOILogic*                 CopyModelLogic(const CAOILogic *LogicPtr);
	bool                       DestroyModelLogicSelected();
	CAOILogic*                 GetModelLogicPtr(int index, bool Check) const;		
	CAOILogic*                 GetModelLogicPtrByGroupID(int LogicGroupID, bool NoIsolatedLogic) const;
	void                       SetModelLogicSelected(bool val);
	CAOILogic*                 GetModelLogicSelected() const;		
	int                        GetModelLogicSelectedCount() const;	
	void                       UnSelectModelLogic();
	bool                       CheckModelLogicModified();
	void                       SetModelLogicModified(bool value);
	bool                       CheckModelLogicValid(const CAOILogic *LogicPtr);//確認邏輯閘指標有效-屬於此模組內
	void                       ClearModelLogicList();	
	int                        GetModelLogicFreeGroupID() const;	
	bool                       DeleteModelLogic(CAOILogic *LogicPtr);		
	void                       SetModelLogicSelectedByLogicGroupID(int LogicGroupID, bool Selected);
	CAOILogic*                 GetModelLogicSelectedByLogicGroupID(int LogicGroupID) const;
	void                       SetModelWndVisibledByLogicGroupID(int LogicGroupID, bool Visibled);
	bool                       DeleteModelLogicSelected();
	void                       ApplyModelLogic(CAOILogic *RefLogicPtr);//套用邏輯閘至其他相同群組
	void                       UpdateModelLogicWndIndex();
	bool                       DeleteModelLogicIndexList(std::vector<unsigned int> &LogicIndexList);
	//---------------------------------------------------------------------------------//	
	void                       RemoveModelImageFolder();//刪除模組的影像資料夾
	bool                       ClearModelFolder(DWORD dwClearFlags);//清除模組資料夾
	bool                       LayoutModelImageFolderIndexList();//重新計算樣板資料夾引數列表
	//---------------------------------------------------------------------------------//
	bool                       CombineModelObjectList();	
	bool                       RemoveModelObjectByWndObjSelected();
	bool                       RemoveModelObjectByLogicObjSelected();
	//---------------------------------------------------------------------------------//		
	bool                       BuildModelType(MODEL_TYPE ModelType);//建立模組樣式	
	bool                       BuildModelType(MODEL_TYPE ModelType, double BodyW, double BodyH);//建立模組樣式	
	bool                       BuildModelBasic(MODEL_TYPE ModelType, double BodyW, double BodyH);//未定義的模組
	bool                       BuildModelChip(MODEL_TYPE ModelType, double BodyW, double BodyH);//被動元件的模組
	bool                       BuildModelArray(MODEL_TYPE ModelType, double BodyW, double BodyH);//電極類的模組
	bool                       BuildModelJLeadComponent(MODEL_TYPE ModelType, double BodyW, double BodyH);//J型腳的模組
	bool                       BuildModelFlatLeadComponent(MODEL_TYPE ModelType, double BodyW, double BodyH);//引腳類的模組	
	bool                       BuildModelPadComponent(MODEL_TYPE ModelType, double BodyW, double BodyH);//焊盤元件的模組
	bool                       BuildModelNoLeadComponent(MODEL_TYPE ModelType, double BodyW, double BodyH);//無腳類的模組
	//---------------------------------------------------------------------------------------//
	AOI_OBJ_TYPE               GetModelAttachedType() const;
	CAOIRgn*                   GetModelAttachedPtr() const { return m_ModelAttachedPtr; }
	void                       SetModelAttachedPtr(CAOIRgn *Ptr) { m_ModelAttachedPtr = Ptr; }
	bool                       UpdateModelRegionToAttached();//將模組的本體調整至附屬內
	//---------------------------------------------------------------------------------------//	
	CAOIFd*                    GetModelFdPtr() const;
	void                       SetModelFdPtr(CAOIFd *Ptr);
	bool                       UpdateModelBodyToFd();//將模組的本體調整至定位點內		
	//---------------------------------------------------------------------------------------//
	CAOIMark*                  GetModelMarkPtr() const;
	void                       SetModelMarkPtr(CAOIMark *Ptr);
	bool                       UpdateModelBodyToMark();//將模組的本體調整至特徵內		
	//---------------------------------------------------------------------------------------//	
	CAOIBarcode*               GetModelBarcodePtr() const;
	void                       SetModelBarcodePtr(CAOIBarcode *Ptr);
	bool                       UpdateModelBodyToBarcode();//將模組的本體調整至軟體條碼內		
	//---------------------------------------------------------------------------------------//
	CAOIComponent*             GetModelComponentPtr() const;
	void                       SetModelComponentPtr(CAOIComponent *Ptr);
	bool                       UpdateModelBodyToComponent();//將模組的本體調整至零件內	
	//---------------------------------------------------------------------------------------//
	void                       SetModelAttachedAngle(double Angle);
	void                       SetModelAttachedPosCad(const TPOINT2D &Pos);
	void                       SetModelAttachedPosCad(double PosX, double PosY);
	void                       SetModelAttachedPosStage(const TPOINT2D &Pos);
	void                       SetModelAttachedPosStage(double PosX, double PosY);
	//---------------------------------------------------------------------------------------//
	double                     GetModelAttachedAngle() const { return m_ModelAttachedAngle; }
	void                       GetModelAttachedPosCad(TPOINT2D &Pos) const { Pos = m_ModelAttachedCadPos; }
	void                       GetModelAttachedPosStage(TPOINT2D &Pos) const { Pos = m_ModelAttachedStagePos; }
	//---------------------------------------------------------------------------------------//
	bool                       CheckModelActObjLinkWndRgn(const TActiveObj *ObjPtr) const;//確認模組物件連動檢測框
	bool                       BuildModelSelectedObjListForPos(TActiveObj *ObjPtr, std::vector<TActiveObj> &ActObjList, std::vector<TActiveObj*> &SelObjList);//取得模組內選用的物件
	bool                       BuildModelSelectedObjListForSize(TActiveObj *ObjPtr, std::vector<TActiveObj> &ActObjList, std::vector<TActiveObj*> &SelObjList);//取得模組內選用的物件
	//---------------------------------------------------------------------------------------//
	bool                       ModifyModelObjPos(std::vector<TActiveObj*> ObjPtrList, double dPx, double dPy, int LinkMode);//修正模組內物件的座標
	bool                       ModifyModelBoxPos(CAOIBox *BoxPtr, CAOIWnd *WndPtr, CAOILand *LandPtr, CAOIWndRoi *WndRoiPtr, CAOIWndMask *MaskWndPtr, double dPx, double dPy, int LinkMode, bool bUpdate=true);//修正模組內框的座標
	bool                       ModifyModelWndPos(CAOIBox *BoxPtr, CAOIWnd *WndPtr, double dPx, double dPy, int LinkMode, bool bUpdate);//修正模組內檢測框的座標
	bool                       ModifyModelLandPos(CAOIBox *BoxPtr, CAOILand *LandPtr, double dPx, double dPy, int LinkMode, bool bUpdate);//修正模組內特徵框的座標
	bool                       ModifyModelWndRoiPos(CAOIBox *BoxPtr, CAOIWnd *WndPtr, CAOIWndRoi *WndRoiPtr, double dPx, double dPy, int LinkMode, bool bUpdate);//修正模組內檢測框子框的座標
	bool                       ModifyModelWndMaskPos(CAOIBox *BoxPtr, CAOIWnd *WndPtr, CAOIWndMask *MaskWndPtr, double dPx, double dPy, int LinkMode, bool bUpdate);//修正模組內檢測框遮罩框的座標
	//---------------------------------------------------------------------------------------//
	bool                       UpdateModelBodySizeToAllWnds();//更新模組本體尺寸至全部檢測框
	bool                       UpdateModelBodySizeToAllWndsKernel();//更新模組本體尺寸至全部檢測框
	//---------------------------------------------------------------------------------------//
	bool                       ModifyModelObjSize(std::vector<TActiveObj*> ObjPtrList, const TREGION4D &dPos, int LinkMode);//修正模組內物件的尺寸
	bool                       ModifyModelBoxSize(CAOIBox *BoxPtr, CAOIWnd *WndPtr, CAOILand *LandPtr, CAOIWndRoi *WndRoiPtr, CAOIWndMask *MaskWndPtr, const TREGION4D &dPos, int LinkMode, bool bUpdate=true);//修正模組內框的尺寸
	bool                       ModifyModelWndSize(CAOIBox *BoxPtr, CAOIWnd *WndPtr, const TREGION4D &dPos, int LinkMode, bool bUpdate);//修正模組內檢測框的尺寸
	bool                       ModifyModelLandSize(CAOIBox *BoxPtr, CAOILand *LandPtr, const TREGION4D &dPos, int LinkMode, bool bUpdate);//修正模組特徵框的尺寸
	bool                       ModifyModelWndRoiSize(CAOIBox *BoxPtr, CAOIWnd *WndPtr, CAOIWndRoi *WndRoiPtr, const TREGION4D &dPos, int LinkMode, bool bUpdate);//修正模組內檢測框子框的尺寸
	bool                       ModifyModelWndMaskSize(CAOIBox *BoxPtr, CAOIWnd *WndPtr, CAOIWndMask *MaskWndPtr, const TREGION4D &dPos, int LinkMode, bool bUpdate);//修正模組內檢測框遮罩框的尺寸
	//---------------------------------------------------------------------------------------//
	bool                       CheckLandBasicBoxIDLinkBody(int ID) const;//確認特徵框基本框是否與本體有關
	bool                       UpdateModelBodyRgnFromChipLead();//更新模組本體尺寸-來自被動元件電極
	bool                       UpdateModelBodyRgnFromChipLeadKernel();//更新模組本體尺寸-來自被動元件電極
	bool                       UpdateModelChipLeadRgnFromBody();//更新模組被動元件電極尺寸-來自本體
	bool                       UpdateModelChipLeadRgnFromBodyKernel();//更新模組被動元件電極尺寸-來自本體	
	//---------------------------------------------------------------------------------------//
	bool                       GetModelExtendAutoAdjust() const;//設定模組外擴自動調整
	void                       SetModelExtendAutoAdjust(bool val);//取得模組外擴自動調整
	//---------------------------------------------------------------------------------------//
	void                       SetModelExtendRangeX(double value);
	void                       SetModelExtendRangeY(double value);
	void                       SetModelExtendRange(const TSIZE2D &value);

	double                     GetModelExtendRangeX() const { return m_ModelExtendRange.cx; }	
	double                     GetModelExtendRangeY() const { return m_ModelExtendRange.cy; }	
	void                       GetModelExtendRange(TSIZE2D &value) const { value=m_ModelExtendRange; }
	TSIZE2D                    GetModelExtendRange() const { return m_ModelExtendRange; }
	//---------------------------------------------------------------------------------------//
	bool                       CalcModelTotalRegionAll();//計算模組整個範圍		
	bool                       ModifyModelTotalRegionSize(double W, double H);//修改模組整個範圍
	bool                       GetModelTotalRegion(TREGION4D &Region, bool bRaw=false) const;//取得模組整個範圍		
	bool                       GetModelTotalRegionCad(TREGION4D &Region) const;//取得模組整個範圍		
	bool                       GetModelTotalRegionStage(TREGION4D &Region) const;//取得模組整個範圍	
	bool                       GetModelTotalCornerPts(TPOINT2D CornerPts[4]) const;//取得模組整個範圍四個端點
	bool                       GetModelTotalCornerPtsCad(TPOINT2D CornerPts[4]) const;//取得模組整個範圍四個端點
	bool                       GetModelTotalCornerPtsStage(TPOINT2D CornerPts[4]) const;//取得模組整個範圍四個端點
	//---------------------------------------------------------------------------------------//
	bool                       GetModelBodyLandRegion(TREGION4D &Region) const;//取得模組特徵範圍		
	bool                       GetModelBodyLandCornerPts(TPOINT2D CornerPts[4]) const;//取得模組特徵範圍四個端點
	//---------------------------------------------------------------------------------------//
	const TSIZE2D&             GetModelImageSize_um() const;//取得模組影像的物理尺寸-um
	void                       SetModelImageSize_um(const TSIZE2D &Size);//設定模組影像的物理尺寸-um	
	//---------------------------------------------------------------------------------------//
	const TPOINT2D&            GetModelImageCadOffset_um() const;//取得模組影像的Cad偏差-um
	void                       SetModelImageCadOffset_um(const TPOINT2D &Offset);//設定模組影像的Cad偏差-um		
	//---------------------------------------------------------------------------------------//		
	int                        GetModelSupportLandTypeCount() const;//取得模型支援特徵框樣式種類數量
	LAND_TYPE                  GetModelLandTypeMaster() const;//取得模型主要特徵框樣式
	//---------------------------------------------------------------------------------------//
	bool                       InitialModelProperty();
	bool                       CloneModelProperty(CAOIModel *RefModelPtr);//複製模組屬性參數
	bool                       AnalysisModelProperty(std::vector<TUNI_FRAME> &UniFrameList);//分析模組	
	bool                       ApplyModelPropertyToAlgParam();//套用模組屬相參數至演算法參數
	//---------------------------------------------------------------------------------------//
	bool                       AddModelDefaultWnd(const TMODEL_DEFAULT_WND_PARAM &Param);//增加模組檢測框
	bool                       AddModelDefaultWnd_PadAlign(const TMODEL_DEFAULT_WND_PARAM &Param, int LandGroupID);//增加模組檢測框-焊盤定位
	bool                       AddModelDefaultWnd_PartAlign(const TMODEL_DEFAULT_WND_PARAM &Param, int LandGroupID);//增加模組檢測框-零件定位
	bool                       AddModelDefaultWnd_PadAdjust(const TMODEL_DEFAULT_WND_PARAM &Param, int LandGroupID);//增加模組檢測框-焊盤調整
	bool                       AddModelDefaultWnd_LeadAdjust(const TMODEL_DEFAULT_WND_PARAM &Param, int LandGroupID);//增加模組檢測框-引腳調整
	bool                       AddModelDefaultWnd_Polarity(const TMODEL_DEFAULT_WND_PARAM &Param);//增加模組檢測框-極性檢測	
	bool                       AddModelDefaultWnd_BodyMissing(const TMODEL_DEFAULT_WND_PARAM &Param);//增加模組檢測框-本體缺件
	bool                       AddModelDefaultWnd_BodyTilt(const TMODEL_DEFAULT_WND_PARAM &Param);//增加模組檢測框-本體傾斜	
	bool                       AddModelDefaultWnd_BodyMount(const TMODEL_DEFAULT_WND_PARAM &Param);//增加模組檢測框-本體錯色
	bool                       AddModelDefaultWnd_TextWrong(const TMODEL_DEFAULT_WND_PARAM &Param);//增加模組檢測框-錯件檢測
	bool                       AddModelDefaultWnd_BodyDamaged(const TMODEL_DEFAULT_WND_PARAM &Param);//增加模組檢測框-本體破損
	bool                       AddModelDefaultWnd_OuterShort(const TMODEL_DEFAULT_WND_PARAM &Param);//增加模組檢測框-外接短路檢測
	bool                       AddModelDefaultWnd_ForeignBody(const TMODEL_DEFAULT_WND_PARAM &Param);//增加模組檢測框-異物檢測
	bool                       AddModelDefaultWnd_OtherDefect(const TMODEL_DEFAULT_WND_PARAM &Param);//增加模組檢測框-其餘瑕疵檢測
	bool                       AddModelDefaultWnd_DefaultModelDefect(const TMODEL_DEFAULT_WND_PARAM &Param);//增加模組檢測框-預設模組全瑕疵
	bool                       AddModelDefaultWnd_LeadLifted(const TMODEL_DEFAULT_WND_PARAM &Param, int LandGroupID);//增加模組檢測框-腳翹檢測
	bool                       AddModelDefaultWnd_LeadBended(const TMODEL_DEFAULT_WND_PARAM &Param, int LandGroupID);//增加模組檢測框-腳歪檢測
	bool                       AddModelDefaultWnd_SolderOpen(const TMODEL_DEFAULT_WND_PARAM &Param, int LandGroupID);//增加模組檢測框-空焊檢測
	bool                       AddModelDefaultWnd_SolderPoor(const TMODEL_DEFAULT_WND_PARAM &Param, int LandGroupID);//增加模組檢測框-少焊檢測
	bool                       AddModelDefaultWnd_SolderPadExposed(const TMODEL_DEFAULT_WND_PARAM &Param, int LandGroupID);//增加模組檢測框-焊盤露出
	bool                       AddModelDefaultWnd_PadScratch(const TMODEL_DEFAULT_WND_PARAM &Param, int LandGroupID);//增加模組檢測框-刮傷檢測
	bool                       AddModelDefaultWnd_LandBridge(const TMODEL_DEFAULT_WND_PARAM &Param, int LandGroupID);//增加模組檢測框-短路檢測
	bool                       AddModelDefaultWnd_LandOuterShort(const TMODEL_DEFAULT_WND_PARAM &Param, int LandGroupID);//增加模組檢測框-焊盤外接短路檢測	
	//---------------------------------------------------------------------------------------//	
	bool                       InitModelInspection(bool SelfTest=true);//初始化模組檢測
	bool                       ExecModelInspection(std::vector<TUNI_FRAME> &UniFrameList);//執行模組檢測
	bool                       ExecModelWndGroupInspection(std::vector<TUNI_FRAME> &UniFrameList);//執行模組檢測檢測框群組檢測	
	bool                       CalcModelInspectionParam(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, TPOINT2D &RgnCp, TPOINT2D &ImageCp, TPOINT2D &Scale, bool bRawRgn=false) const;//計算模組檢測參數
	bool                       CalcModelInspectionImageRect_Raw(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const TPOINT2D &RgnCp, const TPOINT2D &ImageCp, const TPOINT2D &Scale);//計算模組檢測影像位置
	bool                       UpdateModelInspectionPosRes(CAOIWnd *RefWndPtr, double OffsetX, double OffsetY, double Skew, bool ApplySkew);//更新模組檢測的位置結果
	bool                       UpdateModelInspectionPosRes_v1(CAOIWnd *RefWndPtr, double OffsetX, double OffsetY, double Skew, bool ApplySkew);//更新模組檢測的位置結果
	bool                       UpdateModelInspectionPosRes_v2(CAOIWnd *RefWndPtr, double OffsetX, double OffsetY, double Skew, bool ApplySkew);//更新模組檢測的位置結果
	//---------------------------------------------------------------------------------------//	
	void                       ReleaseModelUniFrameList();
	void                       SetModelUniFrameList(const std::vector<TUNI_FRAME> &UniFrameList, bool bClone);//設定模組的影像
	bool                       CopyModelUniFrameList(std::vector<TUNI_FRAME> &UniFrameList, bool bClone) const;//複製模組的影像
	//---------------------------------------------------------------------------------//
	void                       SetModelImageScale(const TPOINT2D &value) { m_ModelImageScale = value; }
	void                       GetModelImageScale(TPOINT2D &value) const { value = m_ModelImageScale; }
	TPOINT2D                   GetModelImageScale() const { return m_ModelImageScale; }
	//---------------------------------------------------------------------------------//
	void                       ClearModelWndOrderList();
	size_t                     GetModelWndOrderCount() const;
	CAOIWnd*                   GetModelWndOrderPtr(size_t idx, bool Check) const;
	bool                       SortModelWndOrder(std::vector<CAOIWnd*> &WndList);//排序模組檢測框
	//---------------------------------------------------------------------------------//
	//模組瑕疵檢測框群組列表
	bool                       BuildModelDefectWndGroupIDList();
	void                       ClearModelDefectWndGroupIDList();
	size_t                     GetModelDefectWndGroupIDCount() const;	
	int                        GetModelDefectWndGroupID(size_t idx, bool Check) const;
	//---------------------------------------------------------------------------------//		
	void                       ClearModelDefectWndUUIDList();
	size_t                     GetModelDefectWndUUIDCount() const;	
	UUID                       GetModelDefectWndUUID(size_t idx, bool Check) const;
	//---------------------------------------------------------------------------------//			
	bool                       SynchronousModel(const CAOIModel *RefModelPtr, bool bPartial);//同步化模組, 需要相同框數量下
	//---------------------------------------------------------------------------------//
	bool                       UpdateModelColorGroupLinkIndex(int Index, const CColorGroup &ColorGroup);//更新模組內的彩色過濾連動
	bool                       UpdateModelColorGroupLinkIndex(const std::vector<CColorGroup> &ColorGroupList);//更新模組內的彩色過濾連動
	//---------------------------------------------------------------------------------//
	//空間基準面設定	
	void                       SetModelPanelBasePlane(double val);
	double                     GetModelPanelBasePlane() const { return m_ModelPanelBasePlane; }
	bool                       CheckModelPanelBasePlaneParamValid() const;//確認空間基準面參數有效
	void                       SetModelSpaceBasePlaneParam(const TBasePlaneParam& Param);
	TBasePlaneParam&           GetModelSpaceBasePlaneParam() { return m_ModelSpaceNoiseFilterParam.BasePlaneParam; }
	const TBasePlaneParam&     GetModelSpaceBasePlaneParam() const { return m_ModelSpaceNoiseFilterParam.BasePlaneParam; }	
	//---------------------------------------------------------------------------------//
	//模組的空間雜訊過濾處理	
	void                       SetModelSpaceNoiseFilterParam(const TNoiseFilterParam& Param);
	TNoiseFilterParam&         GetModelSpaceNoiseFilterParam() { return m_ModelSpaceNoiseFilterParam; }
	const TNoiseFilterParam&   GetModelSpaceNoiseFilterParam() const { return m_ModelSpaceNoiseFilterParam; }	
	//---------------------------------------------------------------------------------//
	void                       SetModelSpaceLeveingParam(double nX, double nY, double nZ);
	double                     GetModelSpaceLeveingParamX() const { return m_ModelSpaceNoiseFilterParam.BasePlaneParam.NormalX; }
	double                     GetModelSpaceLeveingParamY() const { return m_ModelSpaceNoiseFilterParam.BasePlaneParam.NormalY; }
	double                     GetModelSpaceLeveingParamZ() const { return m_ModelSpaceNoiseFilterParam.BasePlaneParam.NormalZ; }
	double                     GetModelSpaceLeveingOffsetZ() const { return m_ModelSpaceNoiseFilterParam.BasePlaneParam.OffsetZ; }
	double                     GetModelSpaceLeveingOverHigh() const { return m_ModelSpaceNoiseFilterParam.BasePlaneParam.OverHighFilter; }
	double                     GetModelSpaceLeveingOverLow() const { return m_ModelSpaceNoiseFilterParam.BasePlaneParam.OverLowFilter; }	
	//---------------------------------------------------------------------------------//
	//特殊遮罩
	void                       SetModelMaskEnable_Base(bool value) { m_ModelMaskEnable_Base = value; }
	bool                       GetModelMaskEnable_Base() const { return m_ModelMaskEnable_Base; }

	void                       SetModelMaskFrameIndex_Base(unsigned int value) { m_ModelMaskFrameIndex_Base = value; }
	unsigned int               GetModelMaskFrameIndex_Base() const { return m_ModelMaskFrameIndex_Base; }

	void                       SetModelMaskFrameUniqueID_Base(unsigned int value) { m_ModelMaskFrameUniqueID_Base = value; }
	unsigned int               GetModelMaskFrameUniqueID_Base() const { return m_ModelMaskFrameUniqueID_Base; }

	void                       SetModelMaskColorGroupLinkIndex(int value) { m_ModelMaskColorGroupLinkIndex = value; }
	int                        GetModelMaskColorGroupLinkIndex() const { return m_ModelMaskColorGroupLinkIndex; }
	//---------------------------------------------------------------------------------//
	bool                       BuildModelBarcodeList(std::vector<std::wstring> &BarcodeList);//取得零件條碼列表
	//---------------------------------------------------------------------------------//
	bool                       CheckModelSpecResult(CAOILand *LandPtr, TSpecResult &sX, TSpecResult &sY, TSpecResult &sA, TSpecResult &tA, TSpecResult &rH, TSpecResult &rA, TSpecResult &rV, TSpecResult &szW, TSpecResult &szH, TSpecResult &oA, RESULT_ID &ResultID);
	//---------------------------------------------------------------------------------//
	bool                       CheckModelUseDataModel() const;//取得是否使用資料模型
	bool                       GetModelDataModelEnabled() const;//取得資料模型啟用
	void                       SetModelDataModelEnabled(bool val);//設定資料模型啟用
	int                        GetModelDataModelLevelID() const;//取得資料模型等級
	void                       SetModelDataModelLevelID(int val);//設定資料模型等級
	bool                       BuildModelDataModelParam(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, TDataModelParam &Param);//建立資料模型參數
	//---------------------------------------------------------------------------------//
	//AI Model	
	bool                       BuildModelAIWndList(std::vector<CAOIWnd*> &List);//建立模組AI視窗列表
	bool                       BuildModelAIWndDefectList(std::vector<CAOIWnd*> &List);//建立模組AI視窗瑕疵列表
	bool                       ExecModelAIInspectionKernel(std::vector<TUNI_FRAME> &UniFrameList);//執行模組Ai檢測	
	bool                       LoadModelAIModelFile(LPCTSTR filename);//載入AI檔案	
	bool                       SaveModelAIModelFile(LPCTSTR filename, LPCTSTR ImgExtName, std::vector<TUNI_FRAME> &UniFrameList);//儲存AI檔案
	bool                       SetModelAiWndListResultID(std::vector<CAOIWnd*> &List, RESULT_ID ResultID, LPCTSTR RexultText);//設定模組AI檢測結果
	//---------------------------------------------------------------------------------//	
	bool                       CalcModelImageRect_CustomerAI(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH);//計算模組影像區域-客戶AI;
	bool                       CalcModelBodyOutsideParam(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, TBasePlaneParam &BasePlaneParam);//計算模組本體外圍資料	
	//---------------------------------------------------------------------------------//	
};
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_AOIMODEL_H__CD0BC378_9070_41F3_BF55_6D37A745AF96__INCLUDED_)
