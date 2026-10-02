// AOIRgn.h: interface for the CAOIRgn class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AOIRGN_H__10ECBF44_0DAA_475F_8F3D_58212E9639DD__INCLUDED_)
#define AFX_AOIRGN_H__10ECBF44_0DAA_475F_8F3D_58212E9639DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include "AOIObj.h"
#include "AOIField.h"
#include "MapCoordinate.h"
//-------------------------------------------------------------------------------------//
class CAOIPanel;
class CAOIBoard;
class CAOIProject;
//-------------------------------------------------------------------------------------//
enum REGION_CALC_STATE   //區域計算狀態
{
	REGION_CALC_NONE           = 0,  //尚未區域計算
	REGION_CALC_DOING          = 1,  //計算中
	REGION_CALC_DONE           = 2,  //計算完畢	
	REGION_CALC_CLEAR          = 9   //計算清除
};
//-------------------------------------------------------------------------------------//
class CAOIRgn : public CAOIObj
{
	//---------------------------------------------------------------------------------//	
	DECLARE_DYNAMIC(CAOIRgn)
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//
	static CRITICAL_SECTION    m_csRgn;//同步機制-關鍵區間
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	static void                InitialRgnLock();//初始化區域的關鍵區間
	static void                DeleteRgnLock(); //刪除區域的關鍵區間
	static void                LockRgn();        //進入區域的關鍵區間
	static void                UnlockRgn();      //離開區域的關鍵區間
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//	
	unsigned int               m_RgnIdx;//檢測區域的引數	
	//---------------------------------------------------------------------------------//
	unsigned int               m_RgnIndex_Project;//檢測區域在專案的引數編號
	unsigned int               m_RgnIndex_Panel;//檢測區域在整板的引數編號
	unsigned int               m_RgnIndex_Board;//檢測區域在單板的引數編號
	//---------------------------------------------------------------------------------//
	CAOIProject               *m_RgnProjectPtr;//檢測區域的專案指標
	//---------------------------------------------------------------------------------//
	CAOIPanel                 *m_RgnPanelPtr;//檢測區域的整板指標
	unsigned int               m_RgnPanelIndex_Project;//檢測區域的整板引數編號
	//---------------------------------------------------------------------------------//
	CAOIBoard                 *m_RgnBoardPtr;//檢測區域的單板指標
	unsigned int               m_RgnBoardIndex_Project;//檢測區域的單板在專案的引數編號
	unsigned int               m_RgnBoardIndex_Panel;//檢測區域的單板在整板的引數編號
	//---------------------------------------------------------------------------------//
	CAOIRgn*                   m_RgnParent;//檢測區域的父指標
	std::vector<CAOIRgn*>      m_RgnSubList;//檢測區域的子列表
	size_t                     m_RgnSubFillFrameCount;//檢測區域的子列表取得影像的數量
	//---------------------------------------------------------------------------------//		
	unsigned int               m_RgnFrameIndex;//影像序號
	unsigned int               m_RgnFrameUniqueID;//影像唯一碼
	//---------------------------------------------------------------------------------//	
	LANE_ID                    m_RgnLaneID;//軌道編號
	CAMERA_ID                  m_RgnCameraID;//相機編號
	LIGHT_MODE                 m_RgnLightMode;//燈源模式	
	DISTRICT_ID                m_RgnDistrictID;//分段編號//兩段式檢測
	bool                       m_RgnBypassed;//區域不檢測
	bool                       m_RgnUsing3D;//區域使用3D資料
	bool                       m_RgnBypass3D;//區域不檢測3D	
	int                        m_RgnOpenMPCount;//區域使用的OpenMP數量
	bool                       m_RgnNeedToCalculate;//區域要去計算
	bool                       m_RgnNeedToCalculateBackup;//區域要去計算-備份檔		
	//---------------------------------------------------------------------------------//	
	int                        m_RgnTempInt;//檢測區域暫時資料-int
	//---------------------------------------------------------------------------------//	
	RESULT_ID                  m_RgnResultID_AOI;  //區域結果編號-設備
	RESULT_ID                  m_RgnResultID_AOI_LA;//區域結果編號-設備-A軌
	RESULT_ID                  m_RgnResultID_AOI_LB;//區域結果編號-設備-B軌
	//---------------------------------------------------------------------------------//	
	RESULT_ID                  m_RgnResultID_ARS;  //區域結果編號-維修
	RESULT_ID                  m_RgnResultID_ARS_LA;//區域結果編號-維修-A軌
	RESULT_ID                  m_RgnResultID_ARS_LB;//區域結果編號-維修-B軌
	//---------------------------------------------------------------------------------//	
	RESULT_ID                  m_RgnResultID_Alarm; //區域結果編號-警報
	//---------------------------------------------------------------------------------//	
	bool                       m_RgnModelImageIsSaved;//區域是否儲存過圖像
	SAVE_TEST_IMAGE_MODE       m_RgnSaveTestImageMode;//區域儲存檢測影像模式	
	//---------------------------------------------------------------------------------//
	RECT                       m_RgnModelImageRect_AI;//區域圖像裡的區域-AI	
	bool                       m_RgnModelImageIsSaved_AI;//區域是否儲存過圖像-AI	
	//---------------------------------------------------------------------------------//
	CAOIField*                 m_RgnSelfFieldPtr;//專屬的Field指標
	bool                       m_RgnSelfFieldEnabled;//專屬的Field啟用
	//---------------------------------------------------------------------------------//
	unsigned int               m_RgnFieldIdx;//所屬的Field引數編號
	CAOIField*                 m_RgnFieldPtr;//所屬的Field指標
	unsigned int               m_RgnFieldIdxBackup;//所屬的Field引數編號-備份
	//---------------------------------------------------------------------------------//
	bool                       m_RgnLinkPointer;//連結指標, 如果是的話不要刪除
	bool                       m_RgnCalculated;//區域計算過
	REGION_CALC_STATE          m_RgnCalcState;//區域檢測結果 
	//---------------------------------------------------------------------------------//
	double                     m_RgnAngle;//區域角度
	TSIZE2D                    m_RgnRoiSize;//區域搜尋尺寸	
	TSIZE2D                    m_RgnBodySize;//區域本體尺寸	
	TPOINT2D                   m_RgnCadPos;//區域在CAD坐標系中位置
	TPOINT2D                   m_RgnSpecialCadPos;//區域在以PCB左下為原點的CAD坐標系中位置
	TPOINT2D                   m_RgnCadBiasPos;//區域在CAD坐標系中偏心差
	TPOINT2D                   m_RgnRoiCadCornerPos[4];//區域搜尋四端點	
	TPOINT2D                   m_RgnBodyCadCornerPos[4];//區域本體四端點	
	//---------------------------------------------------------------------------------//
	TPOINT3D                   m_RgnStagePos;//區域位置在機台坐標系中
	TPOINT3D                   m_RgnRoiStageCornerPos[4];//區域搜尋四端點在機台坐標系中
	TPOINT3D                   m_RgnBodyStageCornerPos[4];//區域本體四端點在機台坐標系中
	//---------------------------------------------------------------------------------//
	TPOINT2D                   m_RgnFovCadPos;//區域所屬FOV的CAD位置
	TPOINT2D                   m_RgnFovStagePos;//區域所屬FOV的Stage位置
	//---------------------------------------------------------------------------------//
	RECT                       m_RgnFrameImageRect;//區域所屬影像的區域
	TSIZE2D                    m_RgnFrameImageSize_um;//區域所屬影像的物理尺寸-um
	TPOINT2D                   m_RgnFrameImageCadOffset_um;//區域所屬影像的區域Cad偏差-um
	POINT                      m_RgnFrameImageCornerPt[4];//區域所屬影像的四個端點
	//---------------------------------------------------------------------------------//	
	TPOINT2D                   m_RgnFieldCadPos;//區域所屬的Field的CAD位置	
	TPOINT3D                   m_RgnFieldStagePos;//區域所屬Field的Stage位置	
	TREGION4D                  m_RgnFieldCadRegion;//區域所屬Field的CAD範圍
	TREGION4D                  m_RgnFieldStageRegion;//區域所屬Field的Stage範圍
	//---------------------------------------------------------------------------------//
	bool                       m_RgnKeepImage;//區域的影像保留
	//---------------------------------------------------------------------------------//
	double                     m_RgnFillImageTime;//區域填滿畫面的時間
	//---------------------------------------------------------------------------------//	
	bool                       m_RgnRawSpaceEnabled;//區域的原始空間資料啟用
	bool                       m_RgnDynamicFrameRectMode;//區域的動態影像尺寸模式	
	IMAGE_SIZE                 m_RgnImageW[FRAME_MAX_COUNT];//區域的影像寬度
	IMAGE_SIZE                 m_RgnImageH[FRAME_MAX_COUNT];//區域的影像長度
	IMAGE_SIZE                 m_RgnImageStep[FRAME_MAX_COUNT];//區域的影像步長
	IMAGE_SIZE                 m_RgnImageBitCount[FRAME_MAX_COUNT];//區域的影像位元數	
	MASK_PTR                   m_RgnMaskPtr[FRAME_MAX_COUNT];//區域的空間遮罩
	IMAGE_PTR                  m_RgnImagePtr[FRAME_MAX_COUNT];//區域的影像記憶體區塊
	SPACE_PTR                  m_RgnSpacePtr[FRAME_MAX_COUNT];//區域的空間記憶體區塊		
	MASK_PTR                   m_RgnRawMaskPtr[FRAME_MAX_COUNT];//區域的原始空間遮罩
	SPACE_PTR                  m_RgnRawSpacePtr[FRAME_MAX_COUNT];//區域的原始空間記憶體區塊		
	unsigned int               m_RgnImageUniqueID[FRAME_MAX_COUNT];//區域的影像唯一碼
	//---------------------------------------------------------------------------------//
	double                     m_RgnPanelBasePlane;//區域的整板基本面
	int                        m_RgnLocalBasePlaneID;//區域的局部平面編號
	bool                       m_RgnLocalBasePlaneFinish;//區域的局部平面完成
	TPOINT3D                   m_RgnLocalBasePlaneParam;//區域的局部平面參數
	TNoiseFilterParam          m_RgnSpaceNoiseFilterParam;//空間雜訊過濾處理
	//---------------------------------------------------------------------------------//
	//基準面的2D遮罩		
	IMAGE_SIZE                 m_RgnMaskW_Base;//區域的影像寬度
	IMAGE_SIZE                 m_RgnMaskH_Base;//區域的影像長度
	IMAGE_SIZE                 m_RgnMaskStep_Base;//區域的影像步長
	IMAGE_SIZE                 m_RgnMaskBitCount_Base;//區域的影像位元數	
	MASK_PTR                   m_RgnMaskPtr_Base;
	//---------------------------------------------------------------------------------//
	bool                       m_RgnDataModelEnabled;//資料模型啟用
	int                        m_RgnDataModelLevelID;//資料模型等級
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//
	CAOIRgn(AOI_OBJ_TYPE type);
	//---------------------------------------------------------------------------------//
	void                       PreInitRgn();
	void                       InitialRgn();
	void                       CloneRgn(const CAOIRgn &Rgn);	
	void                       CloneRgnSubList(const CAOIRgn &Rgn);
	void                       CloneRgnImageBuffer(const CAOIRgn &Rgn);
	//---------------------------------------------------------------------------------//	
	bool                       SaveRgnMovingTimeMsg(const char *pContext);//儲存移動時間訊息
	bool                       SaveRgnMovingTimeMsg(const wchar_t *pContext);//儲存移動時間訊
	//---------------------------------------------------------------------------------//
	bool                       ExtractRgnFrame(bool &Finished);//挖取區域圖像
	bool                       ExtractRgnImage(size_t index, CAOIFrame *FramePtr, bool &Finished);//挖取區域圖像
	bool                       ExtractRgnSpace(size_t index, CAOIFrame *FramePtr, bool &Finished, const IMAGE_SIZE ImageW, const IMAGE_SIZE ImageH, const IMAGE_SIZE ImageStep, const int BitCount, const IMAGE_PTR ImagePtr);//挖取區域圖像
	bool                       ExtractRgnSpace_I(size_t index, CAOIFrame *FramePtr, bool &Finished);//挖取區域圖像
	bool                       ExtractRgnSpace_II(size_t index, CAOIFrame *FramePtr, bool &Finished, const IMAGE_SIZE ImageW, const IMAGE_SIZE ImageH, const IMAGE_SIZE ImageStep, const int BitCount, const IMAGE_PTR ImagePtr);//挖取區域圖像
	//---------------------------------------------------------------------------------//	
	bool                       AddRgnSubRgnPtr(CAOIRgn *RgnSubPtr);//加入區域的子區域
	//---------------------------------------------------------------------------------//	
	bool                       SetRgnSubRgnAllFinish();//設定子檢測框執行完畢	
	bool                       AllocateRgnFullImageBuffer(size_t index, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, IMAGE_PTR &ImagePtr);//建立子檢測框全圖的圖像記憶體區塊
	bool                       FillRgnSubRgnFullImageBuffer(size_t index, IMAGE_SIZE FullImageW, IMAGE_SIZE FullImageH, IMAGE_SIZE FullImageStep, IMAGE_SIZE FullBitCount, IMAGE_PTR FullImagePtr);//拼接子檢測框全圖

	bool                       AllocateRgnFullSpaceBuffer(size_t index, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, SPACE_PTR &SpacePtr, MASK_PTR &MaskPtr);//建立子檢測框全圖的圖像記憶體區塊
	bool                       FillRgnSubRgnFullSpaceBuffer(size_t index, IMAGE_SIZE FullImageW, IMAGE_SIZE FullImageH, IMAGE_SIZE FullImageStep, IMAGE_SIZE FullBitCount, SPACE_PTR FullSpacePtr, MASK_PTR FullMaskPtr);//拼接子檢測框全圖	
	//---------------------------------------------------------------------------------//	
	bool                       FillRgnSubRgnFullMaskBuffer_Base(IMAGE_SIZE FullImageW, IMAGE_SIZE FullImageH, IMAGE_SIZE FullImageStep, IMAGE_SIZE FullBitCount, MASK_PTR FullMaskPtr);//拼接子檢測框基準面遮罩全圖	
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	CAOIRgn();
	CAOIRgn(const CAOIRgn &Rgn);
	virtual ~CAOIRgn();
	CAOIRgn& operator=(const CAOIRgn &Rgn);
	//---------------------------------------------------------------------------------//
	CAOIRgn*                   CloneRgnObj() const;	
	//---------------------------------------------------------------------------------//
	void                       SetRgnIndex(unsigned int value) { m_RgnIdx = value; }
	unsigned int               GetRgnIndex() const { return m_RgnIdx; }
	//---------------------------------------------------------------------------------//		
	//檢測區域在專案的引數編號
	void                       SetRgnIndex_Project(unsigned int value) { m_RgnIndex_Project = value; }
	unsigned int               GetRgnIndex_Project() const { return m_RgnIndex_Project; }
	//---------------------------------------------------------------------------------//		
	//檢測區域在整板的引數編號
	void                       SetRgnIndex_Panel(unsigned int value) { m_RgnIndex_Panel = value; }
	unsigned int               GetRgnIndex_Panel() const { return m_RgnIndex_Panel; }
	//---------------------------------------------------------------------------------//
	//檢測區域在單板的引數編號
	void                       SetRgnIndex_Board(unsigned int value) { m_RgnIndex_Board = value; }
	unsigned int               GetRgnIndex_Board() const { return m_RgnIndex_Board; }
	//---------------------------------------------------------------------------------//
	//檢測區域的專案指標
	void                       SetRgnProjectPtr(CAOIProject *value) { m_RgnProjectPtr = value; }
	CAOIProject*               GetRgnProjectPtr() const { return m_RgnProjectPtr; }
	//---------------------------------------------------------------------------------//	
	//檢測區域的整板指標
	void                       SetRgnPanelPtr(CAOIPanel *value) { m_RgnPanelPtr = value; }
	CAOIPanel*                 GetRgnPanelPtr() const { return m_RgnPanelPtr; }
	//---------------------------------------------------------------------------------//	
	//檢測區域的整板引數編號
	void                       SetRgnPanelIndex_Project(unsigned int value) { m_RgnPanelIndex_Project = value; }
	unsigned int               GetRgnPanelIndex_Project() const { return m_RgnPanelIndex_Project; }
	//---------------------------------------------------------------------------------//
	//檢測區域的單板指標
	void                       SetRgnBoardPtr(CAOIBoard *value) { m_RgnBoardPtr = value; }
	CAOIBoard*                 GetRgnBoardPtr() const { return m_RgnBoardPtr; }
	//---------------------------------------------------------------------------------//		
	//檢測區域的單板在專案的引數編號
	void                       SetRgnBoardIndex_Project(unsigned int value) { m_RgnBoardIndex_Project = value; }
	unsigned int               GetRgnBoardIndex_Project() const { return m_RgnBoardIndex_Project; }
	//---------------------------------------------------------------------------------//	
	//檢測區域的單板在整板的引數編號
	void                       SetRgnBoardIndex_Panel(unsigned int value) { m_RgnBoardIndex_Panel = value; }
	unsigned int               GetRgnBoardIndex_Panel() const { return m_RgnBoardIndex_Panel; }		
	//---------------------------------------------------------------------------------//
	//所屬的Field引數編號
	void                       SetRgnFieldIndex(unsigned int value) { m_RgnFieldIdx = value; }
	unsigned int               GetRgnFieldIndex() const { return m_RgnFieldIdx; }
	//-------------------------------------------------------------------
	//所屬的Field引數編號-備份
	void                       BackupRgnFieldIndex() { m_RgnFieldIdxBackup=m_RgnFieldIdx; }
	void                       RestoreRgnFieldIndex() { m_RgnFieldIdx=m_RgnFieldIdxBackup; }
	void                       SetRgnFieldIdxBackup(unsigned int value) { m_RgnFieldIdxBackup = value; }
	unsigned int               GetRgnFieldIdxBackup() const { return m_RgnFieldIdxBackup; }
	//-------------------------------------------------------------------	
	//檢測區域的父指標
	void                       SetRgnParent(CAOIRgn* value) { m_RgnParent = value; }
	CAOIRgn*                   GetRgnParent() const { return m_RgnParent; }	
	//---------------------------------------------------------------------------------//	
	void                       IncrementRgnSubFillFrameCount();
	size_t                     GetRgnSubFillFrameCount() const;
	//---------------------------------------------------------------------------------//	
	//影像序號
	void                       SetRgnFrameIndex(unsigned int value) { m_RgnFrameIndex = value; }
	unsigned int               GetRgnFrameIndex() const { return m_RgnFrameIndex; }
	//---------------------------------------------------------------------------------//
	//影像唯一碼
	void                       SetRgnFrameUniqueID(unsigned int value) { m_RgnFrameUniqueID = value; }
	unsigned int               GetRgnFrameUniqueID() const { return m_RgnFrameUniqueID; }
	//---------------------------------------------------------------------------------//
	//軌道編號	
	void                       SetRgnLaneID(LANE_ID value) { m_RgnLaneID = value; }
	LANE_ID                    GetRgnLaneID() const { return m_RgnLaneID; }
	//---------------------------------------------------------------------------------//
	//相機編號
	void                       SetRgnCameraID(CAMERA_ID value) { m_RgnCameraID = value; }
	CAMERA_ID                  GetRgnCameraID() const { return m_RgnCameraID; }
	//---------------------------------------------------------------------------------//
	//燈源模式
	void                       SetRgnLightMode(LIGHT_MODE value) { m_RgnLightMode = value; }
	LIGHT_MODE                 GetRgnLightMode() const { return m_RgnLightMode; }
	//---------------------------------------------------------------------------------//
	//分段編號//兩段式檢測
	void                       SetRgnDistrictID(DISTRICT_ID value) { m_RgnDistrictID = value; }
	DISTRICT_ID                GetRgnDistrictID() const { return m_RgnDistrictID; }
	//---------------------------------------------------------------------------------//
	//不檢測區域
	void                       SetRgnBypassed(bool value);
	bool                       GetRgnBypassed() const { return m_RgnBypassed; }
	//---------------------------------------------------------------------------------//
	//區域使用3D資料
	void                       SetRgnUsing3D(bool value) { m_RgnUsing3D = value; }
	bool                       GetRgnUsing3D() const { return m_RgnUsing3D; }
	//---------------------------------------------------------------------------------//
	//區域不檢測3D
	void                       SetRgnBypass3D(bool value) { m_RgnBypass3D = value; }
	bool                       GetRgnBypass3D() const { return m_RgnBypass3D; }
	//---------------------------------------------------------------------------------//
	//區域使用的OpenMP數量
	int                        CalcRgnOpenMPCountByPixels();
	void                       SetRgnOpenMPCount(int value) { m_RgnOpenMPCount = value; }
	int                        GetRgnOpenMPCount() const { return m_RgnOpenMPCount; }	
	//---------------------------------------------------------------------------------//
	//區域的影像保留
	void                       SetRgnKeepImage(bool value) { m_RgnKeepImage = value; }
	bool                       GetRgnKeepImage() const { return m_RgnKeepImage; }
	//---------------------------------------------------------------------------------//
	//區域填滿畫面的時間
	void                       SetRgnFillImageTime(double value) { m_RgnFillImageTime = value; }
	double                     GetRgnFillImageTime() const { return m_RgnFillImageTime; }
	//---------------------------------------------------------------------------------//	
	//要去計算
	void                       SetRgnNeedToCalculate(bool value);
	bool                       GetRgnNeedToCalculate() const { return m_RgnNeedToCalculate; }
	//---------------------------------------------------------------------------------//
	//要去計算-備份檔
	void                       SetRgnNeedToCalculateBackup(bool value);
	bool                       GetRgnNeedToCalculateBackup() const { return m_RgnNeedToCalculateBackup; }
	//---------------------------------------------------------------------------------//
	//檢測區域暫時資料-int
	int                        GetRgnTempInt() const { return m_RgnTempInt; }
	void                       SetRgnTempInt(int value) { m_RgnTempInt=value; }	
	//---------------------------------------------------------------------------------//
	//區域結果編號-設備
	RESULT_ID                  GetRgnResultID_AOI() const { return m_RgnResultID_AOI; }
	void                       SetRgnResultID_AOI(RESULT_ID value) { m_RgnResultID_AOI=value; }	
	//---------------------------------------------------------------------------------//
	//區域結果編號-設備-A軌
	RESULT_ID                  GetRgnResultID_AOI_LA() const { return m_RgnResultID_AOI_LA; }
	void                       SetRgnResultID_AOI_LA(RESULT_ID value) { m_RgnResultID_AOI_LA=value; }	
	//---------------------------------------------------------------------------------//
	//區域結果編號-設備-B軌
	RESULT_ID                  GetRgnResultID_AOI_LB() const { return m_RgnResultID_AOI_LB; }
	void                       SetRgnResultID_AOI_LB(RESULT_ID value) { m_RgnResultID_AOI_LB=value; }	
	//---------------------------------------------------------------------------------//	
	//區域結果編號-維修	
	RESULT_ID                  GetRgnResultID_ARS() const { return m_RgnResultID_ARS; }
	void                       SetRgnResultID_ARS(RESULT_ID value) { m_RgnResultID_ARS=value; }
	//---------------------------------------------------------------------------------//
	//區域結果編號-維修-A軌
	RESULT_ID                  GetRgnResultID_ARS_LA() const { return m_RgnResultID_ARS_LA; }
	void                       SetRgnResultID_ARS_LA(RESULT_ID value) { m_RgnResultID_ARS_LA=value; }
	//---------------------------------------------------------------------------------//
	//區域結果編號-維修-B軌
	RESULT_ID                  GetRgnResultID_ARS_LB() const { return m_RgnResultID_ARS_LB; }
	void                       SetRgnResultID_ARS_LB(RESULT_ID value) { m_RgnResultID_ARS_LB=value; }
	//---------------------------------------------------------------------------------//	
	//區域結果編號-警報	
	RESULT_ID                  GetRgnResultID_Alarm() const { return m_RgnResultID_Alarm; }
	void                       SetRgnResultID_Alarm(RESULT_ID value) { m_RgnResultID_Alarm=value; }
	//---------------------------------------------------------------------------------//
	//區域是否儲存過圖像
	bool                       GetRgnModelImageIsSaved() const { return m_RgnModelImageIsSaved; }
	void                       SetRgnModelImageIsSaved(bool value) { m_RgnModelImageIsSaved=value; }	
	//---------------------------------------------------------------------------------//
	//區域儲存檢測影像模式	
	SAVE_TEST_IMAGE_MODE       GetRgnSaveTestImageMode() const { return m_RgnSaveTestImageMode; }
	void                       SetRgnSaveTestImageMode(SAVE_TEST_IMAGE_MODE  value) { m_RgnSaveTestImageMode=value; }	
	//---------------------------------------------------------------------------------//	
	//區域圖像裡的區域-AI	
	const RECT&                GetRgnModelImageRect_AI() const { return m_RgnModelImageRect_AI; }
	void                       SetRgnModelImageRect_AI(const RECT &value) { m_RgnModelImageRect_AI=value; }	
	//---------------------------------------------------------------------------------//	
	//區域是否儲存過圖像-AI
	bool                       GetRgnModelImageIsSaved_AI() const { return m_RgnModelImageIsSaved_AI; }
	void                       SetRgnModelImageIsSaved_AI(bool value) { m_RgnModelImageIsSaved_AI=value; }	
	//---------------------------------------------------------------------------------//		
	//區域角度
	void                       SetRgnAngle(double value) { m_RgnAngle = value; }
	double                     GetRgnAngle() const { return m_RgnAngle; }
	//---------------------------------------------------------------------------------//
	//區域尺寸寬
	void                       SetRgnRoiSizeW(double value) { m_RgnRoiSize.cx = value; }
	double                     GetRgnRoiSizeW() const { return m_RgnRoiSize.cx; }
	//---------------------------------------------------------------------------------//
	//區域尺寸長
	void                       SetRgnRoiSizeH(double value) { m_RgnRoiSize.cy = value; }
	double                     GetRgnRoiSizeH() const { return m_RgnRoiSize.cy; }
	//---------------------------------------------------------------------------------//	
	//區域位置在CAD坐標系中
	void                       SetRgnCadPos(const TPOINT2D &value) { m_RgnCadPos = value; }
	TPOINT2D                   GetRgnCadPos() const { return m_RgnCadPos; }
	//---------------------------------------------------------------------------------//
	//區域位置在CAD坐標系中-X
	void                       SetRgnCadPosX(double value) { m_RgnCadPos.x = value; }
	double                     GetRgnCadPosX() const { return m_RgnCadPos.x; }
	//---------------------------------------------------------------------------------//
	//區域位置在CAD坐標系中-Y
	void                       SetRgnCadPosY(double value) { m_RgnCadPos.y = value; }
	double                     GetRgnCadPosY() const { return m_RgnCadPos.y; }
	//---------------------------------------------------------------------------------//	
	//區域在CAD坐標系中偏心差
	void                       SetRgnCadBiasPos(const TPOINT2D &value) { m_RgnCadBiasPos = value; }
	TPOINT2D                   GetRgnCadBiasPos() const { return m_RgnCadBiasPos; }
	//---------------------------------------------------------------------------------//
	//區域在CAD坐標系中偏心差-X
	void                       SetRgnCadBiasPosX(double value) { m_RgnCadBiasPos.x = value; }
	double                     GetRgnCadBiasPosX() const { return m_RgnCadBiasPos.x; }
	//---------------------------------------------------------------------------------//
	//區域在CAD坐標系中偏心差-Y
	void                       SetRgnCadBiasPosY(double value) { m_RgnCadBiasPos.y = value; }
	double                     GetRgnCadBiasPosY() const { return m_RgnCadBiasPos.y; }
	//---------------------------------------------------------------------------------//
	//區域在以PCB左下為原點的CAD坐標系中位置
	void                       SetRgnCadSpecialPos(const TPOINT2D &value) { m_RgnSpecialCadPos = value; }
	TPOINT2D                   GetRgnCadSpecialPos() const { return m_RgnSpecialCadPos; }
	//---------------------------------------------------------------------------------//
	//區域在以PCB左下為原點的CAD坐標系中位置-X
	void                       SetRgnCadSpecialPosX(double value) { m_RgnSpecialCadPos.x = value; }
	double                     GetRgnCadSpecialPosX() const { return m_RgnSpecialCadPos.x; }
	//---------------------------------------------------------------------------------//
	//區域在以PCB左下為原點的CAD坐標系中位置-Y
	void                       SetRgnCadSpecialPosY(double value) { m_RgnSpecialCadPos.y = value; }
	double                     GetRgnCadSpecialPosY() const { return m_RgnSpecialCadPos.y; }
	//---------------------------------------------------------------------------------//	
	//取得在CAD坐標系中旋轉中心
	TPOINT2D                   GetRgnCadRotateCenterPos() const { return m_RgnCadPos; }
	//取得在CAD坐標系中對稱中心
	TPOINT2D                   GetRgnCadSymmetryCenterPos() const { return TPOINT2D(m_RgnCadPos.x+m_RgnCadBiasPos.x, m_RgnCadPos.y+m_RgnCadBiasPos.y); }
	//---------------------------------------------------------------------------------//	
	//區域四端點X
	void                       SetRgnCadCornerPosX(const double value[]) 
	{ 
		m_RgnRoiCadCornerPos[0].x = value[0]; 
		m_RgnRoiCadCornerPos[1].x = value[1]; 
		m_RgnRoiCadCornerPos[2].x = value[2]; 
		m_RgnRoiCadCornerPos[3].x = value[3]; 
	}
	void                      GetRgnCadCornerPosX(double value[]) const 
	{ 
		value[0] = m_RgnRoiCadCornerPos[0].x; 
		value[1] = m_RgnRoiCadCornerPos[1].x; 
		value[2] = m_RgnRoiCadCornerPos[2].x; 
		value[3] = m_RgnRoiCadCornerPos[3].x; 
	}
	//---------------------------------------------------------------------------------//
	//區域四端點Y
	void                       SetRgnCadCornerPosY(const double value[]) 
	{ 
		m_RgnRoiCadCornerPos[0].y = value[0]; 
		m_RgnRoiCadCornerPos[1].y = value[1]; 
		m_RgnRoiCadCornerPos[2].y = value[2]; 
		m_RgnRoiCadCornerPos[3].y = value[3]; 
	}
	void                       GetRgnCadCornerPosY(double value[]) const 
	{ 
		value[0] = m_RgnRoiCadCornerPos[0].y; 
		value[1] = m_RgnRoiCadCornerPos[1].y; 
		value[2] = m_RgnRoiCadCornerPos[2].y; 
		value[3] = m_RgnRoiCadCornerPos[3].y; 
	}
	//---------------------------------------------------------------------------------//	
	//區域位置在機台坐標系中-X
	void                       SetRgnStagePos(const TPOINT3D &value) { m_RgnStagePos = value; }
	TPOINT3D                   GetRgnStagePos() const { return m_RgnStagePos; }
	//---------------------------------------------------------------------------------//	
	//區域位置在機台坐標系中-X
	void                       SetRgnStagePosX(double value) { m_RgnStagePos.x = value; }
	double                     GetRgnStagePosX() const { return m_RgnStagePos.x; }
	//---------------------------------------------------------------------------------//
	//區域位置在機台坐標系中-Y
	void                       SetRgnStagePosY(double value) { m_RgnStagePos.y = value; }
	double                     GetRgnStagePosY() const { return m_RgnStagePos.y; }
	//---------------------------------------------------------------------------------//		
	//區域位置在機台坐標系中-Z
	void                       SetRgnStagePosZ(double value) { m_RgnStagePos.z = value; }
	double                     GetRgnStagePosZ() const { return m_RgnStagePos.z; }
	//---------------------------------------------------------------------------------//		
	//區域四端點在機台坐標系中-X
	void                       SetRgnRoiStageCornerPosX(double value[]) 
	{ 
		m_RgnRoiStageCornerPos[0].x = value[0]; 
		m_RgnRoiStageCornerPos[1].x = value[1]; 
		m_RgnRoiStageCornerPos[2].x = value[2]; 
		m_RgnRoiStageCornerPos[3].x = value[3]; 
	}
	void                       GetRgnRoiStageCornerPosX(double value[]) const 
	{ 
		value[0] = m_RgnRoiStageCornerPos[0].x; 
		value[1] = m_RgnRoiStageCornerPos[1].x; 
		value[2] = m_RgnRoiStageCornerPos[2].x; 
		value[3] = m_RgnRoiStageCornerPos[3].x; 
	}
	void                       GetRgnRoiStageCornerPos(TPOINT2D value[]) const 
	{
		value[0].x = m_RgnRoiStageCornerPos[0].x;	value[0].y = m_RgnRoiStageCornerPos[0].y; 	
		value[1].x = m_RgnRoiStageCornerPos[1].x;	value[1].y = m_RgnRoiStageCornerPos[1].y; 	
		value[2].x = m_RgnRoiStageCornerPos[2].x;	value[2].y = m_RgnRoiStageCornerPos[2].y; 	
		value[3].x = m_RgnRoiStageCornerPos[3].x;	value[3].y = m_RgnRoiStageCornerPos[3].y; 	
	}
	//---------------------------------------------------------------------------------//
	//區域四端點在機台坐標系中-Y
	void                       SetRgnRoiStageCornerPosY(double value[]) 
	{ 
		m_RgnRoiStageCornerPos[0].y = value[0]; 
		m_RgnRoiStageCornerPos[1].y = value[1]; 
		m_RgnRoiStageCornerPos[2].y = value[2]; 
		m_RgnRoiStageCornerPos[3].y = value[3]; 
	}
	void                       GetRgnRoiStageCornerPosY(double value[]) const 
	{ 
		value[0] = m_RgnRoiStageCornerPos[0].y; 
		value[1] = m_RgnRoiStageCornerPos[1].y; 
		value[2] = m_RgnRoiStageCornerPos[2].y; 
		value[3] = m_RgnRoiStageCornerPos[3].y; 
	}
	//---------------------------------------------------------------------------------//	
	void                       MoveRgnCadPos(double dX, double dY);//移動區域Cad座標	
	void                       MoveRgnStagePos(double dX, double dY);//移動區域機台座標	
	//---------------------------------------------------------------------------------//
	void                       ModifyRgnRoiRegion(const TREGION4D &dRgn);//修正區域搜尋尺寸
	void                       ModifyRgnBodyRegion(const TREGION4D &dRgn);//修正區域本體尺寸
	//---------------------------------------------------------------------------------//
	void                       CalcRgnCadCornerPos();//計算Cad端點座標		
	void                       LayoutRgnStageCornerPos();//更新機台端點座標	
	void                       CalcRgnCornerPos(double W, double H, double Angle, TPOINT2D CornerPos[4])  const;//計算端點座標	
	//---------------------------------------------------------------------------------//
	//區域所屬FOV的CAD位置-X
	void                       SetRgnFovCadPosX(double value) { m_RgnFovCadPos.x = value; }
	double                     GetRgnFovCadPosX() const { return m_RgnFovCadPos.x; }
	//---------------------------------------------------------------------------------//
	//區域所屬FOV的CAD位置-Y
	void                       SetRgnFovCadPosY(double value) { m_RgnFovCadPos.y = value; }
	double                     GetRgnFovCadPosY() const { return m_RgnFovCadPos.y; }
	//---------------------------------------------------------------------------------//
	//區域所屬FOV的Stage位置-X
	void                       SetRgnFovStagePosX(double value) { m_RgnFovStagePos.x = value; }
	double                     GetRgnFovStagePosX() const { return m_RgnFovStagePos.x; }
	//---------------------------------------------------------------------------------//
	//區域所屬FOV的Stage位置-Y
	void                       SetRgnFovStagePosY(double value) { m_RgnFovStagePos.y = value; }
	double                     GetRgnFovStagePosY() const { return m_RgnFovStagePos.y; }
	//---------------------------------------------------------------------------------//
	//區域所屬影像的區域
	bool                       CalcRgnFrameImageRect();//重新計算影像區域
	void                       SetRgnFrameImageRect(const RECT &value) { m_RgnFrameImageRect = value; }
	const RECT&                GetRgnFrameImageRect() const { return m_RgnFrameImageRect; }
	//---------------------------------------------------------------------------------//
	//區域影像的物理尺寸
	void                       SetRgnFrameImageSize_um(const TSIZE2D &value) { m_RgnFrameImageSize_um = value; }	
	const TSIZE2D&             GetRgnFrameImageSize_um() const { return m_RgnFrameImageSize_um; }	
	//---------------------------------------------------------------------------------//	
	//區域影像的區域Cad偏差-um
	void                       SetRgnFrameImageCadOffset_um(const TPOINT2D &value) { m_RgnFrameImageCadOffset_um = value; }
	const TPOINT2D&            GetRgnFrameImageCadOffset_um() const { return m_RgnFrameImageCadOffset_um; }
	void                       SetRgnFrameImageStageOffset_um(const TPOINT2D &value);
	//---------------------------------------------------------------------------------//	
	void                       SetRgnFrameImageCornerPt(const POINT value[]) 
	{ 
		m_RgnFrameImageCornerPt[0] = value[0]; 
		m_RgnFrameImageCornerPt[1] = value[1]; 
		m_RgnFrameImageCornerPt[2] = value[2]; 
		m_RgnFrameImageCornerPt[3] = value[3]; 
	}
	void                       GetRgnFrameImageCornerPt(POINT value[]) const 
	{ 
		value[0] = m_RgnFrameImageCornerPt[0]; 
		value[1] = m_RgnFrameImageCornerPt[1]; 
		value[2] = m_RgnFrameImageCornerPt[2]; 
		value[3] = m_RgnFrameImageCornerPt[3]; 
	}
	//---------------------------------------------------------------------------------//		
	//專屬的Field指標	
	bool                       CreateRgnSelfFieldPtr();//建立專屬Field指標	
	bool                       DestroyRgnSelfFieldPtr();//刪除專屬Field指標	
	CAOIField*                 GetRgnSelfFieldPtr() const { return m_RgnSelfFieldPtr; }
	//---------------------------------------------------------------------------------//	
	//專屬的Field啟用
	void                       SetRgnSelfFieldEnabled(bool val) { m_RgnSelfFieldEnabled=val; }
	bool                       GetRgnSelfFieldEnabled() const { return m_RgnSelfFieldEnabled; }	
	//---------------------------------------------------------------------------------//	
	//所屬的Field指標
	void                       SetRgnFieldPtr(CAOIField* Ptr);
	CAOIField*                 GetRgnFieldPtr() const { return m_RgnFieldPtr; }
	//---------------------------------------------------------------------------------//		
	//區域所屬的Field的CAD位置
	void                       SetRgnFieldCadPos(const TPOINT2D &value) { m_RgnFieldCadPos = value; }
	TPOINT2D                   GetRgnFieldCadPos() const { return m_RgnFieldCadPos; }
	//---------------------------------------------------------------------------------//
	//區域所屬的Field的CAD範圍
	void                       SetRgnFieldCadRegion(const TREGION4D &value) { m_RgnFieldCadRegion = value; }
	TREGION4D                  GetRgnFieldCadRegion() const { return m_RgnFieldCadRegion; }
	//---------------------------------------------------------------------------------//
	//區域所屬的Field的CAD位置-X
	void                       SetRgnFieldCadPosX(double value) { m_RgnFieldCadPos.x = value; }
	double                     GetRgnFieldCadPosX() const { return m_RgnFieldCadPos.x; }
	//---------------------------------------------------------------------------------//
	//區域所屬的Field的CAD位置-Y
	void                       SetRgnFieldCadPosY(double value) { m_RgnFieldCadPos.y = value; }
	double                     GetRgnFieldCadPosY() const { return m_RgnFieldCadPos.y; }
	//---------------------------------------------------------------------------------//	
	//區域所屬的Field的Stage位置
	void                       SetRgnFieldStagePos(const TPOINT3D &value) { m_RgnFieldStagePos = value; }
	TPOINT3D                   GetRgnFieldStagePos() const { return m_RgnFieldStagePos; }
	//---------------------------------------------------------------------------------//
	//區域所屬的Field的CAD範圍
	void                       SetRgnFieldStageRegion(const TREGION4D &value) { m_RgnFieldStageRegion = value; }
	TREGION4D                  GetRgnFieldStageRegion() const { return m_RgnFieldStageRegion; }
	//---------------------------------------------------------------------------------//
	//區域所屬的Field的Stage位置-X
	void                       SetRgnFieldStagePosX(double value) { m_RgnFieldStagePos.x = value; }
	double                     GetRgnFieldStagePosX() const { return m_RgnFieldStagePos.x; }
	//---------------------------------------------------------------------------------//
	//區域所屬的Field的Stage位置-Y
	void                       SetRgnFieldStagePosY(double value) { m_RgnFieldStagePos.y = value; }
	double                     GetRgnFieldStagePosY() const { return m_RgnFieldStagePos.y; }
	//---------------------------------------------------------------------------------//
	void                       MapRgnCadToStagePos(const CMapCoordinate &Map);//將CAD轉成機台座標
	//---------------------------------------------------------------------------------//
	bool                       SpinRgn(double Angle);//區域自旋轉
	bool                       RotateRgnCad(double Angle, double CpX, double CpY);//區域旋轉	
	bool                       RotateRgnStage(double Angle, double CpX, double CpY);//區域旋轉	
	bool                       MirrorXRgnCad(double CpX);//區域鏡射-X	
	bool                       MirrorYRgnCad(double CpY);//區域鏡射-Y	
	bool                       MirrorXRgnStage(double CpX);//區域鏡射-X
	bool                       MirrorYRgnStage(double CpY);//區域鏡射-Y
	//---------------------------------------------------------------------------------//	
	void                       GetRgnRoiCadRegion(TREGION4D &Region);//取得計算區域在Cad的範圍
	void                       GetRgnBodyCadRegion(TREGION4D &Region);//取得計算區域在Cad的範圍
	void                       GetRgnRoiStageRegion(TREGION4D &Region);//取得計算區域在Stage的範圍	
	void                       GetRgnBodyStageRegion(TREGION4D &Region);//取得計算區域在Stage的範圍	
	//---------------------------------------------------------------------------------//		
	bool                       CheckRgnReadyToCalc();//確認檢測區域可以正常計算
	bool                       CheckRgnNoNeedToCalculate() const;//確認檢測區域是否不再需要計算
	bool                       CheckRgnFieldAllFrameMergeFinish();//確認區域內的所有Frame都已經完成
	bool                       WaitForRgnFieldAllFrameMergeFinish();//等待區域內的所有Frame都已經完成
	bool                       ExecRgnCalc_ProjectMap();//執行區域的專案底圖
	//---------------------------------------------------------------------------------//
	virtual CString            GetRgnDerivedName() const;//取得區域的名稱	
	virtual CString            GetRgnDerivedKeyName() const;//取得區域的名稱
	virtual bool               ExtractRgnDerivedFrame(bool &Finished);//挖取區域圖像
	virtual bool               ExecRgnDerivedInspection();//執行區域檢測
	virtual bool               ExecRgnSaveDefectImage(const std::vector<TUNI_FRAME> &UniFrameList);//執行區域儲存瑕疵圖片
	virtual bool               ExecRgnSaveDefectImage_AI(const std::vector<TUNI_FRAME> &UniFrameList);//執行區域儲存瑕疵圖片-AI
	//---------------------------------------------------------------------------------//	
	void                       SetRgnLinkPointer(bool value) { m_RgnLinkPointer = value; }
	bool                       GetRgnLinkPointer() const { return m_RgnLinkPointer; }
	//---------------------------------------------------------------------------------//		
	void                       SetRgnCalculated(bool value) { m_RgnCalculated = value; }
	bool                       GetRgnCalculated() const { return m_RgnCalculated; }
	//---------------------------------------------------------------------------------//	
	void                       SetRgnCalcState(REGION_CALC_STATE value) { m_RgnCalcState = value; }
	REGION_CALC_STATE          GetRgnCalcState() const { return m_RgnCalcState; }
	void                       UpdateRgnParentCalcStateDone();//更新父區域計算完畢
	//---------------------------------------------------------------------------------//
	bool                       CreateRgnSubList_MatrixField(bool ByCadRegion);//建立區域的子列表-等間距
	bool                       CreateRgnSubList_MatrixField_Cad();//建立區域的子列表-等間距
	bool                       CreateRgnSubList_MatrixField_Stage();//建立區域的子列表-等間距
	bool                       CreateRgnSubList_RandomField();//建立區域的子列表-任意位置
	void                       ClearRgnSubList();//清除檢測區域的子列表	
	//---------------------------------------------------------------------------------//
	//區域的原始空間資料啟用
	bool                       GetRgnRawSpaceEnabled() const;
	void                       SetRgnRawSpaceEnabled(bool val);
	//---------------------------------------------------------------------------------//	
	//區域的動態影像尺寸模式
	bool                       GetRgnDynamicFrameRectMode() const;
	void                       SetRgnDynamicFrameRectMode(bool val);
	//---------------------------------------------------------------------------------//	
	size_t                     GetRgnSubRgnCount() const;//取得檢測區域的子數量
	CAOIRgn*                   GetRgnSubRgnPtr(size_t index, bool check) const;//取得檢測區域的子指標	
	bool                       FillRgnSubRgnFullFrame();//執行區域子檢測框圖像拼接		
	bool                       FillRgnSubRgnFullImage(size_t FrameIdx);//執行區域子檢測框圖像拼接-2D
	bool                       FillRgnSubRgnFullSpace(size_t FrameIdx, const IMAGE_SIZE ImageW, const IMAGE_SIZE ImageH, const IMAGE_SIZE ImageStep, const int BitCount, const IMAGE_PTR ImagePtr);//執行區域子檢測框圖像拼接-3D
	bool                       FillRgnSubRgnFullSpace_I(size_t FrameIdx);//執行區域子檢測框圖像拼接-3D
	bool                       FillRgnSubRgnFullSpace_II(size_t FrameIdx, const IMAGE_SIZE ImageW, const IMAGE_SIZE ImageH, const IMAGE_SIZE ImageStep, const int BitCount, const IMAGE_PTR ImagePtr);//執行區域子檢測框圖像拼接-3D
	//---------------------------------------------------------------------------------//	
	void                       ClearRgnImageBuffer();	
	bool                       GetRgnUniFrameList(std::vector<TUNI_FRAME> &UniFrameList);
	bool                       SetRgnUniFrame(size_t idx, TUNI_FRAME UniFrame);
	bool                       SetRgnImageUniqueID(size_t idx, unsigned int UniqueID);
	bool                       GetRgnImageUniqueID(size_t idx, unsigned int &UniqueID);
	bool                       SetRgnImageBuffer(size_t idx, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, MASK_PTR MaskPtr, SPACE_PTR SpacePtr, MASK_PTR RawMaskPtr, SPACE_PTR RawSpacePtr);	
	bool                       GetRgnImageBuffer(size_t idx, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, IMAGE_PTR &ImagePtr, MASK_PTR &MaskPtr, SPACE_PTR &SpacePtr, MASK_PTR &RawMaskPtr, SPACE_PTR &RawSpacePtr);
	//---------------------------------------------------------------------------------//
	bool                       ExecRgnCalc_ProjectMark();//執行區域的專案標記
	//---------------------------------------------------------------------------------//
	//空間基準面設定	
	void                       SetRgnPanelBasePlane(double val);
	double                     GetRgnPanelBasePlane() const { return m_RgnPanelBasePlane; }
	//---------------------------------------------------------------------------------//
	bool                       CheckRgnLocalBasePlaneUsed() const;//確認是否使用局部基準面
	void                       SetRgnLocalBasePlaneID(int val) { m_RgnLocalBasePlaneID=val; }
	int                        GetRgnLocalBasePlaneID() const { return m_RgnLocalBasePlaneID; }	
	bool                       CheckRgnLocalBasePlaneIDEnabled() const { return 0==m_RgnLocalBasePlaneID?false:true; }
	//---------------------------------------------------------------------------------//	
	void                       SetRgnLocalBasePlaneFinish(bool val);
	bool                       GetRgnLocalBasePlaneFinish() const { return m_RgnLocalBasePlaneFinish; }
	//---------------------------------------------------------------------------------//	
	void                       SetRgnLocalBasePlaneParam(const TPOINT3D &val);
	void                       GetRgnLocalBasePlaneParam(TPOINT3D &val) const;
	//---------------------------------------------------------------------------------//	
	void                       SetRgnSpaceBasePlaneParam(const TBasePlaneParam& Param);
	TBasePlaneParam&           GetRgnSpaceBasePlaneParam() { return m_RgnSpaceNoiseFilterParam.BasePlaneParam; }
	const TBasePlaneParam&     GetRgnSpaceBasePlaneParam() const { return m_RgnSpaceNoiseFilterParam.BasePlaneParam; }	
	//---------------------------------------------------------------------------------//
	//空間雜訊過濾處理	
	void                       SetRgnSpaceNoiseFilterParam(const TNoiseFilterParam& Param);
	TNoiseFilterParam&         GetRgnSpaceNoiseFilterParam() { return m_RgnSpaceNoiseFilterParam; }
	const TNoiseFilterParam&   GetRgnSpaceNoiseFilterParam() const { return m_RgnSpaceNoiseFilterParam; }	
	//---------------------------------------------------------------------------------//
	//遮罩部分-基準面	
	void                       SetRgnMaskEnable_Base(bool value) { GetRgnSpaceBasePlaneParam().BasePlane2DMaskEnabled = value; }
	bool                       GetRgnMaskEnable_Base() const { return GetRgnSpaceBasePlaneParam().BasePlane2DMaskEnabled; }

	void                       SetRgnMaskColorGroupLinkIndex(int value) { GetRgnSpaceBasePlaneParam().BasePlane2DMaskGroupLinkIndex = value; }
	int                        GetRgnMaskColorGroupLinkIndex() const { return GetRgnSpaceBasePlaneParam().BasePlane2DMaskGroupLinkIndex; }

	void                       SetRgnMaskFrameIndex_Base(unsigned int value) { GetRgnSpaceBasePlaneParam().BasePlane2DMaskFrameIndex = value; }
	unsigned int               GetRgnMaskFrameIndex_Base() const { return GetRgnSpaceBasePlaneParam().BasePlane2DMaskFrameIndex; }

	void                       SetRgnMaskFrameUniqueID_Base(unsigned int value) { GetRgnSpaceBasePlaneParam().BasePlane2DMaskFrameUniqueID = value; }
	unsigned int               GetRgnMaskFrameUniqueID_Base() const { return GetRgnSpaceBasePlaneParam().BasePlane2DMaskFrameUniqueID; }

	void                       ClearRgnMaskBuffer_Base();
	bool                       CheckRgnSubRgnMaskBuffer_Base();//確認區域內子區域的基準面遮罩指標
	void                       ClearRgnSubRgnMaskBuffer_Base();//清除子區域的基準面遮罩指標
	bool                       SetRgnMaskBuffer_Base(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, MASK_PTR MaskPtr, bool bClone);
	bool                       GetRgnMaskBuffer_Base(IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, MASK_PTR &MaskPtr);
	//---------------------------------------------------------------------------------//	
	bool                       GetRgnDataModelEnabled() const;//取得資料模型啟用
	void                       SetRgnDataModelEnabled(bool val);//設定資料模型啟用
	//---------------------------------------------------------------------------------//
	int                        GetRgnDataModelLevelID() const;//取得資料模型等級
	void                       SetRgnDataModelLevelID(int val);//設定資料模型等級
	//---------------------------------------------------------------------------------//
	bool                       CreateRgnLocalRgnField(const std::vector<TUNI_FRAME> &UniFrameList, CAOIField *&rFieldPtr);//建立區域的局部區域指標
	//---------------------------------------------------------------------------------//
	bool                       CheckRgnNeedSaveImage(TASK_MODE TaskMode, SAVE_TEST_IMAGE_MODE SaveMode, RESULT_ID ResultID) const;//確認是否需要存圖
	//---------------------------------------------------------------------------------//
	bool                       CalcRgnBodyOutsideParam(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, TBasePlaneParam &BasePlaneParam);//計算區域的本體外圍參數	
	//---------------------------------------------------------------------------------//
};
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_AOIRGN_H__10ECBF44_0DAA_475F_8F3D_58212E9639DD__INCLUDED_)
