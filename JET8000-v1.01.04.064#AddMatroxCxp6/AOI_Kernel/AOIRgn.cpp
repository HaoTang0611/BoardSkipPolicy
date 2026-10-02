// AoiRgn.cpp: implementation of the CAOIRgn class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "AOIRgn.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
CRITICAL_SECTION CAOIRgn::m_csRgn;//同步機制-關鍵區間
//-------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
IMPLEMENT_DYNAMIC(CAOIRgn, CAOIObj)
//-------------------------------------------------------------------------------------//
void CAOIRgn::InitialRgnLock()//初始化區域的關鍵區間
{
	::InitializeCriticalSection(&m_csRgn);
}
//-------------------------------------------------------------------------------------//
void CAOIRgn::DeleteRgnLock()//刪除區域的關鍵區間
{
	::DeleteCriticalSection(&m_csRgn);
}
//-------------------------------------------------------------------------------------//
void CAOIRgn::LockRgn()//進入區域的關鍵區間
{
	::EnterCriticalSection(&m_csRgn);
}
//-------------------------------------------------------------------------------------//
void CAOIRgn::UnlockRgn()//離開區域的關鍵區間
{
	::LeaveCriticalSection(&m_csRgn);
}
//-------------------------------------------------------------------------------------//
CAOIRgn::CAOIRgn():CAOIObj(AOI_OBJ_RGN)
{
	PreInitRgn();
	InitialRgn();
}
//-------------------------------------------------------------------------------------//
CAOIRgn::CAOIRgn(AOI_OBJ_TYPE type):CAOIObj(type)
{
	PreInitRgn();
	InitialRgn();
}
//-------------------------------------------------------------------------------------//
CAOIRgn::CAOIRgn(const CAOIRgn &Rgn):CAOIObj(Rgn)
{
	PreInitRgn();
	CloneRgn(Rgn);
}
//-------------------------------------------------------------------------------------//
CAOIRgn::~CAOIRgn()
{
	ClearRgnSubList();
	ClearRgnImageBuffer();
	ClearRgnMaskBuffer_Base();
	//DestroyRgnSelfFieldPtr();
}
//-------------------------------------------------------------------------------------//
CAOIRgn& CAOIRgn::operator=(const CAOIRgn &Rgn)
{
	if ( this == &Rgn ) { return *this; }
	CAOIObj::operator=(Rgn);
	CAOIRgn::CloneRgn(Rgn);
	return *this;
}
//-------------------------------------------------------------------------------------//
inline void CAOIRgn::PreInitRgn()
{
	size_t i=0;
	for ( i=0; i<FRAME_MAX_COUNT; i++ )
	{
		m_RgnMaskPtr[i] = NULL;
		m_RgnImagePtr[i] = NULL;
		m_RgnSpacePtr[i] = NULL;		
		m_RgnRawMaskPtr[i] = NULL;
		m_RgnRawSpacePtr[i] = NULL;
	}		
	m_RgnMaskPtr_Base = NULL;
}
//-------------------------------------------------------------------------------------//
inline void CAOIRgn::InitialRgn()
{
	size_t  i=0;
	//---------------------------------------------------------------------------------//
	m_RgnIdx = -1;//檢測區域的引數
	//---------------------------------------------------------------------------------//	
	m_RgnIndex_Project = -1;//檢測區域在專案的引數編號
	m_RgnIndex_Panel = -1;//檢測區域在整板的引數編號
	m_RgnIndex_Board = -1;//檢測區域在單板的引數編號
	//---------------------------------------------------------------------------------//
	m_RgnProjectPtr = NULL;//檢測區域的專案指標
	//---------------------------------------------------------------------------------//
	m_RgnPanelPtr = NULL;//檢測區域的整板指標
	m_RgnPanelIndex_Project = -1;//檢測區域的整板引數編號
	//---------------------------------------------------------------------------------//
	m_RgnBoardPtr = NULL;//檢測區域的單板指標
	m_RgnBoardIndex_Project = -1;//檢測區域的單板在專案的引數編號
	m_RgnBoardIndex_Panel = -1;//檢測區域的單板在整板的引數編號
	//---------------------------------------------------------------------------------//
	m_RgnParent = NULL;//檢測區域的父指標	
	ClearRgnSubList();//檢測區域的子列表		
	//---------------------------------------------------------------------------------//	
	m_RgnFrameIndex = 0;//影像序號
	m_RgnFrameUniqueID = FRAME_UNIQUE_ID_DEFAULT;//影像唯一碼
	//---------------------------------------------------------------------------------//	
	m_RgnLaneID = LANE_ID_A;//軌道編號
	m_RgnCameraID = PRIMARY_CAMERA_ID;//相機編號
	m_RgnLightMode = DEFAULT_LIGHT_MODE;//燈源模式	
	m_RgnDistrictID = DISTRICT_ID_A;//分段編號//兩段式檢測
	m_RgnBypassed = false;//不檢測區域
	m_RgnUsing3D = true;//區域使用3D資料
	m_RgnBypass3D = false;//區域不檢測3D
	m_RgnOpenMPCount = 0;//區域使用的OpenMP數量
	m_RgnNeedToCalculate = true;//區域要去計算	
	m_RgnNeedToCalculateBackup = true;
	//---------------------------------------------------------------------------------//	
	m_RgnTempInt = 0;//檢測區域暫時資料-int
	//---------------------------------------------------------------------------------//
	m_RgnResultID_AOI = RESULT_ID_NONE;  //區域結果編號-設備
	m_RgnResultID_AOI_LA = RESULT_ID_NONE;  //區域結果編號-設備-A軌
	m_RgnResultID_AOI_LB = RESULT_ID_NONE;  //區域結果編號-設備-B軌
	m_RgnResultID_ARS = RESULT_ID_NONE;  //區域結果編號-維修
	m_RgnResultID_ARS_LA = RESULT_ID_NONE;  //區域結果編號-維修-A軌
	m_RgnResultID_ARS_LB = RESULT_ID_NONE;  //區域結果編號-維修-B軌	
	m_RgnResultID_Alarm = RESULT_ID_NONE; //區域結果編號-警報
	//---------------------------------------------------------------------------------//
	m_RgnModelImageIsSaved = false;//區域是否儲存過圖像
	m_RgnSaveTestImageMode = SAVE_TEST_IMAGE_DISABLE;//區域儲存檢測影像模式	
	//---------------------------------------------------------------------------------//
	::memset(&m_RgnModelImageRect_AI, 0x00, sizeof(m_RgnModelImageRect_AI));//區域圖像裡的區域-AI	
	m_RgnModelImageIsSaved_AI = false;//區域是否儲存過圖像-AI	
	//---------------------------------------------------------------------------------//
	m_RgnFieldIdx = -1;//所屬的Field引數編號
	m_RgnFieldPtr = NULL;//所屬的Field指標
	m_RgnFieldIdxBackup = -1;
	//---------------------------------------------------------------------------------//	
	m_RgnSelfFieldPtr = NULL;//專屬的Field指標
	m_RgnSelfFieldEnabled = false;//專屬的Field啟用
	//---------------------------------------------------------------------------------//
	m_RgnLinkPointer = false;//連結指標, 如果是的話不要刪除
	m_RgnCalculated = false;//區域計算過
	m_RgnCalcState = REGION_CALC_NONE;//區域檢測結果
	//---------------------------------------------------------------------------------//
	m_RgnAngle = 0;//區域角度
	m_RgnRoiSize.cx = 1000;//1000;//區域尺寸寬
	m_RgnRoiSize.cy = 1000;//1000;//區域尺寸長	
	m_RgnBodySize.cx = 1000;
	m_RgnBodySize.cy = 1000;
	m_RgnCadPos = TPOINT2D();//區域位置在CAD坐標系中
	m_RgnCadBiasPos = TPOINT2D();//區域偏心差在CAD坐標系中
	m_RgnSpecialCadPos = TPOINT2D(INVALID_DOUBLE, INVALID_DOUBLE);//區域在以PCB左下為原點的CAD坐標系中位置
	//區域搜尋四端點
	m_RgnRoiCadCornerPos[0]=TPOINT2D();
	m_RgnRoiCadCornerPos[1]=TPOINT2D();
	m_RgnRoiCadCornerPos[2]=TPOINT2D();
	m_RgnRoiCadCornerPos[3]=TPOINT2D();
	//區域本體四端點	
	m_RgnBodyCadCornerPos[0]=TPOINT2D();
	m_RgnBodyCadCornerPos[1]=TPOINT2D();
	m_RgnBodyCadCornerPos[2]=TPOINT2D();
	m_RgnBodyCadCornerPos[3]=TPOINT2D();
	//---------------------------------------------------------------------------------//		
	m_RgnStagePos = TPOINT3D();//區域位置在機台坐標系中
	//區域四端點在機台坐標系中
	m_RgnRoiStageCornerPos[0] = TPOINT3D();
	m_RgnRoiStageCornerPos[1] = TPOINT3D();
	m_RgnRoiStageCornerPos[2] = TPOINT3D();
	m_RgnRoiStageCornerPos[3] = TPOINT3D();
	//區域本體四端點在機台坐標系中
	m_RgnBodyStageCornerPos[0] = TPOINT3D();
	m_RgnBodyStageCornerPos[1] = TPOINT3D();
	m_RgnBodyStageCornerPos[2] = TPOINT3D();
	m_RgnBodyStageCornerPos[3] = TPOINT3D();
	//---------------------------------------------------------------------------------//	
	m_RgnFovCadPos = TPOINT2D();//區域所屬FOV的CAD位置
	m_RgnFovStagePos = TPOINT2D();//區域所屬FOV的Stage位置	

	m_RgnFrameImageRect.left   = 0;//區域所屬影像的區域
	m_RgnFrameImageRect.right  = 0;//區域所屬影像的區域
	m_RgnFrameImageRect.top    = 0;//區域所屬影像的區域
	m_RgnFrameImageRect.bottom = 0;//區域所屬影像的區域

	m_RgnFrameImageCornerPt[0].x = m_RgnFrameImageCornerPt[0].y = 0;//區域所屬影像的四個端點
	m_RgnFrameImageCornerPt[1].x = m_RgnFrameImageCornerPt[1].y = 0;//區域所屬影像的四個端點
	m_RgnFrameImageCornerPt[2].x = m_RgnFrameImageCornerPt[2].y = 0;//區域所屬影像的四個端點
	m_RgnFrameImageCornerPt[3].x = m_RgnFrameImageCornerPt[3].y = 0;//區域所屬影像的四個端點

	m_RgnFieldCadPos = TPOINT2D();//區域所屬的Field的CAD位置	
	m_RgnFieldStagePos = TPOINT3D();//區域所屬的Field的Stage位置	
	m_RgnFieldCadRegion = TREGION4D();//區域所屬Field的CAD範圍
	m_RgnFieldStageRegion = TREGION4D();//區域所屬Field的Stage範圍

	m_RgnKeepImage = false;
	m_RgnFillImageTime = 0;
	m_RgnRawSpaceEnabled = false;
	m_RgnDynamicFrameRectMode = false;
	ClearRgnImageBuffer();

	//空間雜訊過濾處理
	m_RgnPanelBasePlane = 0;
	m_RgnLocalBasePlaneID = 1;//區域的局部平面編號
	m_RgnLocalBasePlaneFinish = false;
	m_RgnLocalBasePlaneParam = TPOINT3D();//區域的局部平面參數
	AOIDataCollect.GetSpaceNoiseFilterParam(m_RgnSpaceNoiseFilterParam);

	//基板抽色影像
	TBasePlaneParam &BasePlaneParam=GetRgnSpaceBasePlaneParam();
	BasePlaneParam.BasePlane2DMaskEnabled = false;
	BasePlaneParam.BasePlane2DMaskFrameIndex = 0;
	BasePlaneParam.BasePlane2DMaskFrameUniqueID = FRAME_UNIQUE_ID_DEFAULT;
	BasePlaneParam.BasePlane2DMaskGroupLinkIndex = PROJECT_COLOR_ID_BOARD_BEGIN;

	ClearRgnMaskBuffer_Base();
	//---------------------------------------------------------------------------------//	
	m_RgnDataModelEnabled = false;//資料模型啟用
	m_RgnDataModelLevelID = 3;//資料模型等級
	//---------------------------------------------------------------------------------//		
}
//-------------------------------------------------------------------------------------//
inline void CAOIRgn::CloneRgn(const CAOIRgn &Rgn)
{
	size_t  i=0;
	//---------------------------------------------------------------------------------//	
	m_RgnIdx = Rgn.m_RgnIdx;//檢測區域的引數
	//---------------------------------------------------------------------------------//	
	m_RgnIndex_Project = Rgn.m_RgnIndex_Project;//檢測區域在專案的引數編號
	m_RgnIndex_Panel = Rgn.m_RgnIndex_Panel;//檢測區域在整板的引數編號
	m_RgnIndex_Board = Rgn.m_RgnIndex_Board;//檢測區域在單板的引數編號
	//---------------------------------------------------------------------------------//
	m_RgnProjectPtr = Rgn.m_RgnProjectPtr;//檢測區域的專案指標
	//---------------------------------------------------------------------------------//
	m_RgnPanelPtr = Rgn.m_RgnPanelPtr;//檢測區域的整板指標
	m_RgnPanelIndex_Project = Rgn.m_RgnPanelIndex_Project;//檢測區域的整板引數編號
	//---------------------------------------------------------------------------------//
	m_RgnBoardPtr = Rgn.m_RgnBoardPtr;//檢測區域的單板指標
	m_RgnBoardIndex_Project = Rgn.m_RgnBoardIndex_Project;//檢測區域的單板在專案的引數編號
	m_RgnBoardIndex_Panel = Rgn.m_RgnBoardIndex_Panel;//檢測區域的單板在整板的引數編號	
	//---------------------------------------------------------------------------------//
	m_RgnParent = Rgn.m_RgnParent;//檢測區域的父指標	
	m_RgnSubFillFrameCount = Rgn.m_RgnSubFillFrameCount;//檢測區域的子列表取得影像的數量	
	//---------------------------------------------------------------------------------//	
	m_RgnFrameIndex = Rgn.m_RgnFrameIndex;//影像序號
	m_RgnFrameUniqueID = Rgn.m_RgnFrameUniqueID;//影像唯一碼
	//---------------------------------------------------------------------------------//
	m_RgnLaneID = Rgn.m_RgnLaneID;//軌道編號
	m_RgnCameraID = Rgn.m_RgnCameraID;//相機編號
	m_RgnLightMode = Rgn.m_RgnLightMode;//燈源模式
	m_RgnDistrictID = Rgn.m_RgnDistrictID;//分段編號	
	m_RgnBypassed = Rgn.m_RgnBypassed;//不檢測區域	
	m_RgnUsing3D = Rgn.m_RgnUsing3D;//區域使用3D資料
	m_RgnBypass3D = Rgn.m_RgnBypass3D;//區域不檢測3D
	m_RgnOpenMPCount = Rgn.m_RgnOpenMPCount;//區域使用的OpenMP數量
	m_RgnNeedToCalculate = Rgn.m_RgnNeedToCalculate;//區域要去計算
	m_RgnNeedToCalculateBackup = Rgn.m_RgnNeedToCalculateBackup;//區域要去計算-備份	
	m_RgnTempInt = Rgn.m_RgnTempInt;//檢測區域暫時資料-int
	m_RgnResultID_AOI = Rgn.m_RgnResultID_AOI;  //區域結果編號-設備
	m_RgnResultID_AOI_LA = Rgn.m_RgnResultID_AOI_LA;//區域結果編號-設備-A軌
	m_RgnResultID_AOI_LB = Rgn.m_RgnResultID_AOI_LB;//區域結果編號-設備-B軌
	m_RgnResultID_ARS = Rgn.m_RgnResultID_ARS;  //區域結果編號-維修
	m_RgnResultID_ARS_LA = Rgn.m_RgnResultID_ARS_LA;  //區域結果編號-維修-A軌
	m_RgnResultID_ARS_LB = Rgn.m_RgnResultID_ARS_LB;  //區域結果編號-維修-B軌	
	m_RgnResultID_Alarm = Rgn.m_RgnResultID_Alarm; //區域結果編號-警報
	m_RgnModelImageIsSaved = Rgn.m_RgnModelImageIsSaved;//區域是否儲存過圖像
	m_RgnSaveTestImageMode = Rgn.m_RgnSaveTestImageMode;//區域儲存檢測影像模式	
	m_RgnModelImageRect_AI = Rgn.m_RgnModelImageRect_AI;//區域圖像裡的區域-AI	
	m_RgnModelImageIsSaved_AI = Rgn.m_RgnModelImageIsSaved_AI;//區域是否儲存過圖像-AI	
	//---------------------------------------------------------------------------------//
	m_RgnFieldIdx = Rgn.m_RgnFieldIdx;//所屬的Field引數編號
	m_RgnFieldPtr = Rgn.m_RgnFieldPtr;//所屬的Field指標
	m_RgnFieldIdxBackup = Rgn.m_RgnFieldIdxBackup;
	//---------------------------------------------------------------------------------//		
	m_RgnSelfFieldPtr = NULL;//專屬的Field指標	
	m_RgnSelfFieldEnabled = Rgn.m_RgnSelfFieldEnabled;//專屬的Field啟用
	//---------------------------------------------------------------------------------//
	m_RgnLinkPointer = Rgn.m_RgnLinkPointer;//連結指標, 如果是的話不要刪除
	m_RgnCalculated = Rgn.m_RgnCalculated;//區域計算過	
	m_RgnCalcState = Rgn.m_RgnCalcState;//區域檢測結果
	//---------------------------------------------------------------------------------//
	m_RgnAngle = Rgn.m_RgnAngle;//區域角度
	m_RgnRoiSize = Rgn.m_RgnRoiSize;//區域尺寸	
	m_RgnBodySize = Rgn.m_RgnBodySize;//區域本體尺寸
	m_RgnCadPos = Rgn.m_RgnCadPos;//區域位置在CAD坐標系中
	m_RgnCadBiasPos = Rgn.m_RgnCadBiasPos;//區域在CAD坐標系中偏心差
	//區域搜尋四端點
	m_RgnRoiCadCornerPos[0] = Rgn.m_RgnRoiCadCornerPos[0];
	m_RgnRoiCadCornerPos[1] = Rgn.m_RgnRoiCadCornerPos[1];
	m_RgnRoiCadCornerPos[2] = Rgn.m_RgnRoiCadCornerPos[2];
	m_RgnRoiCadCornerPos[3] = Rgn.m_RgnRoiCadCornerPos[3];
	//區域本體四端點	
	m_RgnBodyCadCornerPos[0] = Rgn.m_RgnBodyCadCornerPos[0];
	m_RgnBodyCadCornerPos[1] = Rgn.m_RgnBodyCadCornerPos[1];
	m_RgnBodyCadCornerPos[2] = Rgn.m_RgnBodyCadCornerPos[2];
	m_RgnBodyCadCornerPos[3] = Rgn.m_RgnBodyCadCornerPos[3];
	//---------------------------------------------------------------------------------//		
	m_RgnStagePos = Rgn.m_RgnStagePos;//區域位置在機台坐標系中
	//區域四端點在機台坐標系中
	m_RgnRoiStageCornerPos[0] = Rgn.m_RgnRoiStageCornerPos[0];
	m_RgnRoiStageCornerPos[1] = Rgn.m_RgnRoiStageCornerPos[1];
	m_RgnRoiStageCornerPos[2] = Rgn.m_RgnRoiStageCornerPos[2];
	m_RgnRoiStageCornerPos[3] = Rgn.m_RgnRoiStageCornerPos[3];
	//區域四端點在機台坐標系中
	m_RgnBodyStageCornerPos[0] = Rgn.m_RgnBodyStageCornerPos[0];
	m_RgnBodyStageCornerPos[1] = Rgn.m_RgnBodyStageCornerPos[1];
	m_RgnBodyStageCornerPos[2] = Rgn.m_RgnBodyStageCornerPos[2];
	m_RgnBodyStageCornerPos[3] = Rgn.m_RgnBodyStageCornerPos[3];
	//---------------------------------------------------------------------------------//	
	m_RgnFovCadPos = Rgn.m_RgnFovCadPos;//區域所屬FOV的CAD位置
	m_RgnFovStagePos = Rgn.m_RgnFovStagePos;//區域所屬FOV的Stage位置	
	//---------------------------------------------------------------------------------//
	m_RgnFrameImageRect = Rgn.m_RgnFrameImageRect;//區域所屬影像的區域
	m_RgnFrameImageSize_um = Rgn.m_RgnFrameImageSize_um;
	m_RgnFrameImageCadOffset_um = Rgn.m_RgnFrameImageCadOffset_um;
	
	m_RgnFrameImageCornerPt[0].x = Rgn.m_RgnFrameImageCornerPt[0].x;
	m_RgnFrameImageCornerPt[0].y = Rgn.m_RgnFrameImageCornerPt[0].y;//區域所屬影像的四個端點
	m_RgnFrameImageCornerPt[1].x = Rgn.m_RgnFrameImageCornerPt[1].x;
	m_RgnFrameImageCornerPt[1].y = Rgn.m_RgnFrameImageCornerPt[1].y;//區域所屬影像的四個端點
	m_RgnFrameImageCornerPt[2].x = Rgn.m_RgnFrameImageCornerPt[2].x;
	m_RgnFrameImageCornerPt[2].y = Rgn.m_RgnFrameImageCornerPt[2].y;//區域所屬影像的四個端點
	m_RgnFrameImageCornerPt[3].x = Rgn.m_RgnFrameImageCornerPt[3].x;
	m_RgnFrameImageCornerPt[3].y = Rgn.m_RgnFrameImageCornerPt[3].y;//區域所屬影像的四個端點

	m_RgnFieldCadPos = Rgn.m_RgnFieldCadPos;//區域所屬的Field的CAD位置
	m_RgnFieldStagePos = Rgn.m_RgnFieldStagePos;//區域所屬的Field的Stage位置	
	m_RgnFieldCadRegion = Rgn.m_RgnFieldCadRegion;//區域所屬Field的CAD範圍
	m_RgnFieldStageRegion = Rgn.m_RgnFieldStageRegion;//區域所屬Field的Stage範圍

	m_RgnKeepImage = Rgn.m_RgnKeepImage;
	m_RgnFillImageTime = Rgn.m_RgnFillImageTime;
	m_RgnRawSpaceEnabled = Rgn.m_RgnRawSpaceEnabled;
	m_RgnDynamicFrameRectMode = Rgn.m_RgnDynamicFrameRectMode;

	m_RgnPanelBasePlane = Rgn.m_RgnPanelBasePlane;
	m_RgnLocalBasePlaneID = Rgn.m_RgnLocalBasePlaneID;
	m_RgnLocalBasePlaneFinish = Rgn.m_RgnLocalBasePlaneFinish;
	m_RgnLocalBasePlaneParam = Rgn.m_RgnLocalBasePlaneParam;
	m_RgnSpaceNoiseFilterParam = Rgn.m_RgnSpaceNoiseFilterParam;//空間雜訊過濾處理
	
	ClearRgnMaskBuffer_Base();

	m_RgnDataModelEnabled = Rgn.m_RgnDataModelEnabled;
	m_RgnDataModelLevelID = Rgn.m_RgnDataModelLevelID;	

	CloneRgnSubList(Rgn);
	CloneRgnImageBuffer(Rgn);
}
//-------------------------------------------------------------------------------------//
inline bool CAOIRgn::AddRgnSubRgnPtr(CAOIRgn *RgnSubPtr)//加入區域的子區域
{
	if ( NULL == RgnSubPtr ) { return false; }		
	RgnSubPtr->SetRgnPanelPtr(GetRgnPanelPtr());
	RgnSubPtr->SetRgnBoardPtr(GetRgnBoardPtr());
	RgnSubPtr->SetRgnProjectPtr(GetRgnProjectPtr());	

	RgnSubPtr->SetRgnLaneID(GetRgnLaneID());
	RgnSubPtr->SetRgnUsing3D(GetRgnUsing3D());
	RgnSubPtr->SetRgnBypass3D(GetRgnBypass3D());
	RgnSubPtr->SetRgnBypassed(GetRgnBypassed());
	//RgnSubPtr->SetRgnCameraID(GetRgnCameraID());
	//RgnSubPtr->SetRgnLightMode(GetRgnLightMode());
	RgnSubPtr->SetRgnDistrictID(GetRgnDistrictID());
	RgnSubPtr->SetRgnNeedToCalculate(GetRgnNeedToCalculate());
	RgnSubPtr->SetRgnBoardIndex_Panel(GetRgnBoardIndex_Panel());
	RgnSubPtr->SetRgnPanelIndex_Project(GetRgnPanelIndex_Project());	
	RgnSubPtr->SetRgnBoardIndex_Project(GetRgnBoardIndex_Project());

	RgnSubPtr->SetRgnRawSpaceEnabled(GetRgnRawSpaceEnabled());
	RgnSubPtr->SetRgnDynamicFrameRectMode(GetRgnDynamicFrameRectMode());

	RgnSubPtr->SetRgnLocalBasePlaneID(GetRgnLocalBasePlaneID());
	RgnSubPtr->SetRgnLocalBasePlaneFinish(GetRgnLocalBasePlaneFinish());

	m_RgnSubList.push_back(RgnSubPtr);
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIRgn::CloneRgnSubList(const CAOIRgn &Rgn)
{
	size_t   i = 0;
	CAOIRgn  *RgnSubPtr = NULL;
	const size_t SubCount = Rgn.GetRgnSubRgnCount();

	CAOIRgn::ClearRgnSubList();
	for ( i=0; i<SubCount; i++ )
	{
		RgnSubPtr = Rgn.GetRgnSubRgnPtr(i, false);
		if ( NULL == RgnSubPtr ) { continue; }
		RgnSubPtr = RgnSubPtr->CloneRgnObj();
		if ( NULL == RgnSubPtr ) { continue; }
		RgnSubPtr->SetRgnParent(this);
		m_RgnSubList.push_back(RgnSubPtr);
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CAOIRgn::CloneRgnImageBuffer(const CAOIRgn &Rgn)
{
	const char fnName[] = "CAOIRgn::CloneRgnImageBuffer";

	size_t i=0;
	size_t BufferSize=0;	
	const bool Cloned = true;
	CAOIRgn::ClearRgnImageBuffer();	
	for ( i=0; i<FRAME_MAX_COUNT; i++ )
	{		
		m_RgnImageW[i] = Rgn.m_RgnImageW[i];
		m_RgnImageH[i] = Rgn.m_RgnImageH[i];
		m_RgnImageStep[i] = Rgn.m_RgnImageStep[i];
		m_RgnImageBitCount[i] = Rgn.m_RgnImageBitCount[i];
		m_RgnMaskPtr[i] = Rgn.m_RgnMaskPtr[i];//區域的空間遮罩
		m_RgnImagePtr[i] = Rgn.m_RgnImagePtr[i];//區域的影像記憶體區塊
		m_RgnSpacePtr[i] = Rgn.m_RgnSpacePtr[i];//區域的空間記憶體區塊
		m_RgnRawMaskPtr[i] = Rgn.m_RgnRawMaskPtr[i];//區域的原始空間遮罩
		m_RgnRawSpacePtr[i] = Rgn.m_RgnRawSpacePtr[i];//區域的原始空間記憶體區塊
		m_RgnImageUniqueID[i] = Rgn.m_RgnImageUniqueID[i];

		if ( true == Cloned )
		{
			BufferSize = ImageAPI.CalcBufferSize(m_RgnImageStep[i], m_RgnImageH[i]);
			if ( NULL != Rgn.m_RgnMaskPtr[i] )
			{
				if ( JetMemory.alloc_func(BufferSize, m_RgnMaskPtr[i], fnName, "m_RgnMaskPtr") == true )
				{	::memcpy(m_RgnMaskPtr[i], Rgn.m_RgnMaskPtr[i], sizeof(MASK_DATA)*BufferSize);	}
			}
			if ( NULL != Rgn.m_RgnSpacePtr[i] )
			{
				if ( JetMemory.alloc_func(BufferSize, m_RgnSpacePtr[i], fnName, "m_RgnSpacePtr") == true )
				{	::memcpy(m_RgnSpacePtr[i], Rgn.m_RgnSpacePtr[i], sizeof(SPACE_DATA)*BufferSize);	}
			}
			if ( NULL != Rgn.m_RgnImagePtr[i] )
			{
				if ( JetMemory.alloc_func(BufferSize, m_RgnImagePtr[i], fnName, "m_RgnImagePtr") == true )
				{	::memcpy(m_RgnImagePtr[i], Rgn.m_RgnImagePtr[i], sizeof(IMAGE_DATA)*BufferSize);	}
			}

			if ( NULL != Rgn.m_RgnRawMaskPtr[i] )
			{
				if ( JetMemory.alloc_func(BufferSize, m_RgnRawMaskPtr[i], fnName, "m_RgnRawMaskPtr") == true )
				{	::memcpy(m_RgnRawMaskPtr[i], Rgn.m_RgnRawMaskPtr[i], sizeof(MASK_DATA)*BufferSize);	}
			}
			if ( NULL != Rgn.m_RgnRawSpacePtr[i] )
			{
				if ( JetMemory.alloc_func(BufferSize, m_RgnRawSpacePtr[i], fnName, "m_RgnRawSpacePtr") == true )
				{	::memcpy(m_RgnRawSpacePtr[i], Rgn.m_RgnRawSpacePtr[i], sizeof(SPACE_DATA)*BufferSize);	}
			}
		}
	}
}
//-------------------------------------------------------------------------------------//
CAOIRgn* CAOIRgn::CloneRgnObj() const
{
	CAOIRgn *ObjPtr = AOIObjManager.CreateRgnObj();
	if ( NULL == ObjPtr ) { return NULL; }
	ObjPtr->operator=(*this);
	return ObjPtr;
}
//-------------------------------------------------------------------------------------//
CString CAOIRgn::GetRgnDerivedName() const//取得區域的名稱
{
	return CString(_T("Rgn"));
}
//-------------------------------------------------------------------------------------//
CString CAOIRgn::GetRgnDerivedKeyName() const//取得區域的名稱	
{
	return CString(_T("Rgn"));
}
//-------------------------------------------------------------------------------------//
void CAOIRgn::SetRgnBypassed(bool value)
{
	m_RgnBypassed = value; 

	size_t   i = 0;
	CAOIRgn *SubRgnPtr = NULL;
	const size_t SubRgnCount = GetRgnSubRgnCount();
	for ( i=0; i<SubRgnCount; i++ )
	{
		SubRgnPtr = GetRgnSubRgnPtr(i, false);
		if ( NULL == SubRgnPtr ) { continue; }
		SubRgnPtr->SetRgnBypassed(m_RgnBypassed);
	}
}
//-------------------------------------------------------------------------------------//
int  CAOIRgn::CalcRgnOpenMPCountByPixels()
{
	int nOpenMPCnt=0;	
	TREGION4D  StageRgn;
	double     ResX=0, ResY=0;
	IMAGE_SIZE CameraW=0, CameraH=0;
	CAMERA_ID CameraID = GetRgnCameraID();	
	GetRgnRoiStageRegion(StageRgn);	
	AOIDataCollect.GetCameraImageInfo(CameraID, CameraW, CameraH, ResX, ResY);		
	const IMAGE_SIZE FullImageW = JetAPI::Floor((StageRgn.maxX-StageRgn.minX)/ResX);
	const IMAGE_SIZE FullImageH = JetAPI::Floor((StageRgn.maxY-StageRgn.minY)/ResY);
	const IMAGE_SIZE ImageSize = FullImageW*FullImageH;	
	
	const size_t BaseSize=AOIDataCollect.GetOpenMPCheckSize_Inspection();
	const int nLevel=(int)(ImageSize/BaseSize);
	nOpenMPCnt = nLevel+1;	
	return nOpenMPCnt;
}
//-------------------------------------------------------------------------------------//
void CAOIRgn::SetRgnNeedToCalculate(bool value)
{ 
	m_RgnNeedToCalculate = value; 

	size_t   i = 0;
	CAOIRgn *SubRgnPtr = NULL;
	const size_t SubRgnCount = GetRgnSubRgnCount();
	for ( i=0; i<SubRgnCount; i++ )
	{
		SubRgnPtr = GetRgnSubRgnPtr(i, false);
		if ( NULL == SubRgnPtr ) { continue; }
		SubRgnPtr->SetRgnNeedToCalculate(value);
	}
}
//-------------------------------------------------------------------------------------//
void CAOIRgn::SetRgnNeedToCalculateBackup(bool value)
{
	m_RgnNeedToCalculateBackup = value; 

	size_t   i = 0;
	CAOIRgn *SubRgnPtr = NULL;
	const size_t SubRgnCount = GetRgnSubRgnCount();
	for ( i=0; i<SubRgnCount; i++ )
	{
		SubRgnPtr = GetRgnSubRgnPtr(i, false);
		if ( NULL == SubRgnPtr ) { continue; }
		SubRgnPtr->SetRgnNeedToCalculateBackup(value);
	}
}
//-------------------------------------------------------------------------------------//
void CAOIRgn::IncrementRgnSubFillFrameCount()
{
	m_RgnSubFillFrameCount ++;	
}
//-------------------------------------------------------------------------------------//
size_t CAOIRgn::GetRgnSubFillFrameCount() const
{
	return m_RgnSubFillFrameCount;	
}
//-------------------------------------------------------------------------------------//
void CAOIRgn::MoveRgnCadPos(double dX, double dY)
{
	m_RgnCadPos.x += dX;//區域位置在CAD坐標系中-X
	m_RgnCadPos.y += dY;//區域位置在CAD坐標系中-Y

	//m_RgnCadBiasPos.x += dX;//區域在CAD坐標系中偏心差
	//m_RgnCadBiasPos.y += dY;//區域在CAD坐標系中偏心差	
	//Roi
	m_RgnRoiCadCornerPos[0].x += dX;//區域四端點X
	m_RgnRoiCadCornerPos[0].y += dY;//區域四端點Y
	m_RgnRoiCadCornerPos[1].x += dX;//區域四端點X
	m_RgnRoiCadCornerPos[1].y += dY;//區域四端點Y
	m_RgnRoiCadCornerPos[2].x += dX;//區域四端點X
	m_RgnRoiCadCornerPos[2].y += dY;//區域四端點Y
	m_RgnRoiCadCornerPos[3].x += dX;//區域四端點X
	m_RgnRoiCadCornerPos[3].y += dY;//區域四端點Y
	//Body
	m_RgnBodyCadCornerPos[0].x += dX;//區域四端點X
	m_RgnBodyCadCornerPos[0].y += dY;//區域四端點Y
	m_RgnBodyCadCornerPos[1].x += dX;//區域四端點X
	m_RgnBodyCadCornerPos[1].y += dY;//區域四端點Y
	m_RgnBodyCadCornerPos[2].x += dX;//區域四端點X
	m_RgnBodyCadCornerPos[2].y += dY;//區域四端點Y
	m_RgnBodyCadCornerPos[3].x += dX;//區域四端點X
	m_RgnBodyCadCornerPos[3].y += dY;//區域四端點Y
}
//-------------------------------------------------------------------------------------//
void CAOIRgn::MoveRgnStagePos(double dX, double dY)//移動區域Cad座標
{
	m_RgnStagePos.x += dX;//區域位置在機台坐標系中-X
	m_RgnStagePos.y += dY;//區域位置在機台坐標系中-Y
	//Roi
	m_RgnRoiStageCornerPos[0].x += dX;//區域四端點在機台坐標系中-X
	m_RgnRoiStageCornerPos[0].y += dY;//區域四端點在機台坐標系中-Y
	m_RgnRoiStageCornerPos[1].x += dX;//區域四端點在機台坐標系中-X
	m_RgnRoiStageCornerPos[1].y += dY;//區域四端點在機台坐標系中-Y
	m_RgnRoiStageCornerPos[2].x += dX;//區域四端點在機台坐標系中-X
	m_RgnRoiStageCornerPos[2].y += dY;//區域四端點在機台坐標系中-Y
	m_RgnRoiStageCornerPos[3].x += dX;//區域四端點在機台坐標系中-X
	m_RgnRoiStageCornerPos[3].y += dY;//區域四端點在機台坐標系中-Y
	//Body
	m_RgnBodyStageCornerPos[0].x += dX;//區域四端點在機台坐標系中-X
	m_RgnBodyStageCornerPos[0].y += dY;//區域四端點在機台坐標系中-Y
	m_RgnBodyStageCornerPos[1].x += dX;//區域四端點在機台坐標系中-X
	m_RgnBodyStageCornerPos[1].y += dY;//區域四端點在機台坐標系中-Y
	m_RgnBodyStageCornerPos[2].x += dX;//區域四端點在機台坐標系中-X
	m_RgnBodyStageCornerPos[2].y += dY;//區域四端點在機台坐標系中-Y
	m_RgnBodyStageCornerPos[3].x += dX;//區域四端點在機台坐標系中-X
	m_RgnBodyStageCornerPos[3].y += dY;//區域四端點在機台坐標系中-Y
}
//-------------------------------------------------------------------------------------//
void CAOIRgn::ModifyRgnRoiRegion(const TREGION4D &dRgn)//修正區域尺寸
{
	double W2=0, H2=0;
	double MinX=0, MinY=0, MaxX=0, MaxY=0;
	const double CadPosX = m_RgnCadPos.x;
	const double CadPosY = m_RgnCadPos.y;
	W2 = CAOIRgn::m_RgnRoiSize.cx*0.5;
	H2 = CAOIRgn::m_RgnRoiSize.cy*0.5;
	MinX = CAOIRgn::m_RgnCadPos.x-W2+dRgn.minX;
	MaxX = CAOIRgn::m_RgnCadPos.x+W2+dRgn.maxX;
	MinY = CAOIRgn::m_RgnCadPos.y-H2+dRgn.minY;
	MaxY = CAOIRgn::m_RgnCadPos.y+H2+dRgn.maxY;

	CAOIRgn::m_RgnCadPos.x = (MinX+MaxX)/2.0f;
	CAOIRgn::m_RgnCadPos.y = (MinY+MaxY)/2.0f;
	CAOIRgn::m_RgnRoiSize.cx = (MaxX-MinX);
	CAOIRgn::m_RgnRoiSize.cy = (MaxY-MinY);
	if ( CAOIRgn::m_RgnRoiSize.cx < 0 ) { CAOIRgn::m_RgnRoiSize.cx = -CAOIRgn::m_RgnRoiSize.cx; }
	if ( CAOIRgn::m_RgnRoiSize.cy < 0 ) { CAOIRgn::m_RgnRoiSize.cy = -CAOIRgn::m_RgnRoiSize.cy; }
	CAOIRgn::CalcRgnCadCornerPos();

	TPOINT2D CadOffset;
	TPOINT2D StageOffset;
	CadOffset.x = m_RgnCadPos.x-CadPosX;
	CadOffset.y = m_RgnCadPos.y-CadPosY;
	AOIDataCollect.MapCadOffsetPtToStage(CadOffset, StageOffset);	
	m_RgnStagePos.x += StageOffset.x;//區域位置在機台坐標系中-X
	m_RgnStagePos.y += StageOffset.y;//區域位置在機台坐標系中-Y
	CAOIRgn::LayoutRgnStageCornerPos();
}
//-------------------------------------------------------------------------------------//
void CAOIRgn::ModifyRgnBodyRegion(const TREGION4D &dRgn)//修正區域本體尺寸
{
	double W2=0, H2=0;
	double MinX=0, MinY=0, MaxX=0, MaxY=0;
	const double CadPosX = m_RgnCadPos.x;
	const double CadPosY = m_RgnCadPos.y;
	W2 = CAOIRgn::m_RgnBodySize.cx*0.5;
	H2 = CAOIRgn::m_RgnBodySize.cy*0.5;
	MinX = CAOIRgn::m_RgnCadPos.x-W2+dRgn.minX;
	MaxX = CAOIRgn::m_RgnCadPos.x+W2+dRgn.maxX;
	MinY = CAOIRgn::m_RgnCadPos.y-H2+dRgn.minY;
	MaxY = CAOIRgn::m_RgnCadPos.y+H2+dRgn.maxY;

	CAOIRgn::m_RgnCadPos.x = (MinX+MaxX)/2.0f;
	CAOIRgn::m_RgnCadPos.y = (MinY+MaxY)/2.0f;
	CAOIRgn::m_RgnBodySize.cx = (MaxX-MinX);
	CAOIRgn::m_RgnBodySize.cy = (MaxY-MinY);
	if ( CAOIRgn::m_RgnBodySize.cx < 0 ) { CAOIRgn::m_RgnBodySize.cx = -CAOIRgn::m_RgnBodySize.cx; }
	if ( CAOIRgn::m_RgnBodySize.cy < 0 ) { CAOIRgn::m_RgnBodySize.cy = -CAOIRgn::m_RgnBodySize.cy; }
	CAOIRgn::CalcRgnCadCornerPos();

	TPOINT2D CadOffset;
	TPOINT2D StageOffset;
	CadOffset.x = m_RgnCadPos.x-CadPosX;
	CadOffset.y = m_RgnCadPos.y-CadPosY;
	AOIDataCollect.MapCadOffsetPtToStage(CadOffset, StageOffset);	
	m_RgnStagePos.x += StageOffset.x;//區域位置在機台坐標系中-X
	m_RgnStagePos.y += StageOffset.y;//區域位置在機台坐標系中-Y
	CAOIRgn::LayoutRgnStageCornerPos();
}
//-------------------------------------------------------------------------------------//
void CAOIRgn::CalcRgnCadCornerPos()//計算Cad端點座標
{	
	const double DBL_PRESION = 0.1;	//需使用與CheckIsExceptionAngle相同精度
	const double RoiSizeW = m_RgnRoiSize.cx;
	const double RoiSizeH = m_RgnRoiSize.cy;	
	const double BodySizeW = m_RgnBodySize.cx;
	const double BodySizeH = m_RgnBodySize.cy;	
	const double AngleDEG = JetAPI::AdjustRotationAngle(m_RgnAngle);//角度, 注意回傳區間為-360 ~ 360
	//const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(AngleDEG);
	TPOINT2D RoiSymCenterPos = CAOIRgn::GetRgnCadSymmetryCenterPos();
	TPOINT2D BodySymCenterPos = CAOIRgn::m_RgnCadPos;//GetRgnCadSymmetryCenterPos()
	TPOINT2D RotateCenterPos = CAOIRgn::GetRgnCadRotateCenterPos();

	if ( fabs(AngleDEG-0.0)<DBL_PRESION || fabs(AngleDEG-180.0)<DBL_PRESION || fabs(AngleDEG-360.0)<DBL_PRESION )
	{
		//Roi
		m_RgnRoiCadCornerPos[0].x = RoiSymCenterPos.x-(RoiSizeW*0.5);
		m_RgnRoiCadCornerPos[0].y = RoiSymCenterPos.y-(RoiSizeH*0.5);
		m_RgnRoiCadCornerPos[1].x = RoiSymCenterPos.x+(RoiSizeW*0.5);
		m_RgnRoiCadCornerPos[1].y = m_RgnRoiCadCornerPos[0].y;
		m_RgnRoiCadCornerPos[2].x = m_RgnRoiCadCornerPos[1].x;
		m_RgnRoiCadCornerPos[2].y = RoiSymCenterPos.y+(RoiSizeH*0.5);
		m_RgnRoiCadCornerPos[3].x = m_RgnRoiCadCornerPos[0].x;
		m_RgnRoiCadCornerPos[3].y = m_RgnRoiCadCornerPos[2].y;

		//Body
		m_RgnBodyCadCornerPos[0].x = BodySymCenterPos.x-(BodySizeW*0.5);
		m_RgnBodyCadCornerPos[0].y = BodySymCenterPos.y-(BodySizeH*0.5);
		m_RgnBodyCadCornerPos[1].x = BodySymCenterPos.x+(BodySizeW*0.5);
		m_RgnBodyCadCornerPos[1].y = m_RgnBodyCadCornerPos[0].y;
		m_RgnBodyCadCornerPos[2].x = m_RgnBodyCadCornerPos[1].x;
		m_RgnBodyCadCornerPos[2].y = BodySymCenterPos.y+(BodySizeH*0.5);
		m_RgnBodyCadCornerPos[3].x = m_RgnBodyCadCornerPos[0].x;
		m_RgnBodyCadCornerPos[3].y = m_RgnBodyCadCornerPos[2].y;
	}
	else if ( fabs(AngleDEG-90.0)<DBL_PRESION || fabs(AngleDEG-270.0)<DBL_PRESION )
	{
		//Roi
		m_RgnRoiCadCornerPos[0].x = RoiSymCenterPos.x-(RoiSizeH*0.5);
		m_RgnRoiCadCornerPos[0].y = RoiSymCenterPos.y-(RoiSizeW*0.5);
		m_RgnRoiCadCornerPos[1].x = RoiSymCenterPos.x+(RoiSizeH*0.5);
		m_RgnRoiCadCornerPos[1].y = m_RgnRoiCadCornerPos[0].y;
		m_RgnRoiCadCornerPos[2].x = m_RgnRoiCadCornerPos[1].x;
		m_RgnRoiCadCornerPos[2].y = RoiSymCenterPos.y+(RoiSizeW*0.5);
		m_RgnRoiCadCornerPos[3].x = m_RgnRoiCadCornerPos[0].x;
		m_RgnRoiCadCornerPos[3].y = m_RgnRoiCadCornerPos[2].y;
		//Body
		m_RgnBodyCadCornerPos[0].x = BodySymCenterPos.x-(BodySizeH*0.5);
		m_RgnBodyCadCornerPos[0].y = BodySymCenterPos.y-(BodySizeW*0.5);
		m_RgnBodyCadCornerPos[1].x = BodySymCenterPos.x+(BodySizeH*0.5);
		m_RgnBodyCadCornerPos[1].y = m_RgnBodyCadCornerPos[0].y;
		m_RgnBodyCadCornerPos[2].x = m_RgnBodyCadCornerPos[1].x;
		m_RgnBodyCadCornerPos[2].y = BodySymCenterPos.y+(BodySizeW*0.5);
		m_RgnBodyCadCornerPos[3].x = m_RgnBodyCadCornerPos[0].x;
		m_RgnBodyCadCornerPos[3].y = m_RgnBodyCadCornerPos[2].y;
	}	
	else
	{
		double angle = 0;
		double SizeH = 0, SizeW = 0;
		double SizeH2 = 0, SizeW2 = 0;
		double Length = 0;
		double Arc = 0;
		const double AngleRAD = AngleDEG*DEG_TO_RAD_DBL;
		const double AngleRad090 = PI_RAD*0.5;
		const double AngleRad270 = PI_RAD*1.5;

		//Roi
		//Left Up, +90
		SizeW = RoiSizeW;
		SizeH = RoiSizeH;
		SizeW2 = SizeW*0.5;
		SizeH2 = SizeH*0.5;	
		Length = sqrt((SizeW*SizeW)+(SizeH*SizeH))*0.5;		
		Arc = ::atan2(SizeH, SizeW);		

		m_RgnRoiCadCornerPos[0].x = RoiSymCenterPos.x-SizeW2;
		m_RgnRoiCadCornerPos[0].y = RoiSymCenterPos.y-SizeH2;
		m_RgnRoiCadCornerPos[1].x = RoiSymCenterPos.x+SizeW2;
		m_RgnRoiCadCornerPos[1].y = RoiSymCenterPos.y-SizeH2;
		m_RgnRoiCadCornerPos[2].x = RoiSymCenterPos.x+SizeW2;
		m_RgnRoiCadCornerPos[2].y = RoiSymCenterPos.y+SizeH2;
		m_RgnRoiCadCornerPos[3].x = RoiSymCenterPos.x-SizeW2;
		m_RgnRoiCadCornerPos[3].y = RoiSymCenterPos.y+SizeH2;
		JetAPI::RotateCornerPos(AngleDEG, RotateCenterPos.x, RotateCenterPos.y, m_RgnRoiCadCornerPos);

		//Body		
		SizeW = BodySizeW;
		SizeH = BodySizeH;
		SizeW2 = SizeW*0.5;
		SizeH2 = SizeH*0.5;	
		Length = sqrt((SizeW*SizeW)+(SizeH*SizeH))*0.5;		
		Arc = ::atan2(SizeH, SizeW);

		m_RgnBodyCadCornerPos[0].x = BodySymCenterPos.x-SizeW2;
		m_RgnBodyCadCornerPos[0].y = BodySymCenterPos.y-SizeH2;
		m_RgnBodyCadCornerPos[1].x = BodySymCenterPos.x+SizeW2;
		m_RgnBodyCadCornerPos[1].y = BodySymCenterPos.y-SizeH2;
		m_RgnBodyCadCornerPos[2].x = BodySymCenterPos.x+SizeW2;
		m_RgnBodyCadCornerPos[2].y = BodySymCenterPos.y+SizeH2;
		m_RgnBodyCadCornerPos[3].x = BodySymCenterPos.x-SizeW2;
		m_RgnBodyCadCornerPos[3].y = BodySymCenterPos.y+SizeH2;
		JetAPI::RotateCornerPos(AngleDEG, RotateCenterPos.x, RotateCenterPos.y, m_RgnBodyCadCornerPos);	
		
		/*
		angle = AngleRad090+Arc+AngleRAD;
		m_RgnRoiCadCornerPos[0].x = Length*cos(angle)+SymCenterPos.x;
		m_RgnRoiCadCornerPos[0].y = Length*sin(angle)+SymCenterPos.y;

		angle = AngleRad090-Arc+AngleRAD;
		m_RgnRoiCadCornerPos[1].x = Length*cos(angle)+SymCenterPos.x;
		m_RgnRoiCadCornerPos[1].y = Length*sin(angle)+SymCenterPos.y;

		angle = AngleRad270+Arc+AngleRAD;
		m_RgnRoiCadCornerPos[2].x = Length*cos(angle)+SymCenterPos.x;
		m_RgnRoiCadCornerPos[2].y = Length*sin(angle)+SymCenterPos.y;

		angle = AngleRad270-Arc+AngleRAD;
		m_RgnRoiCadCornerPos[3].x = Length*cos(angle)+SymCenterPos.x;
		m_RgnRoiCadCornerPos[3].y = Length*sin(angle)+SymCenterPos.y;		
		*/		
	}
	/*
	TPOINT2D BiasPos = this->m_RgnCadBiasPos;
	m_RgnRoiCadCornerPos[0].x += BiasPos.x;
	m_RgnRoiCadCornerPos[0].y += BiasPos.y;
	m_RgnRoiCadCornerPos[1].x += BiasPos.x;
	m_RgnRoiCadCornerPos[1].y += BiasPos.y;
	m_RgnRoiCadCornerPos[2].x += BiasPos.x;
	m_RgnRoiCadCornerPos[2].y += BiasPos.y;
	m_RgnRoiCadCornerPos[3].x += BiasPos.x;
	m_RgnRoiCadCornerPos[3].y += BiasPos.y;
	*/
	
}
//-------------------------------------------------------------------------------------//
void CAOIRgn::CalcRgnCornerPos(double W, double H, double Angle, TPOINT2D CornerPos[4])  const//計算端點座標	
{
	const double DBL_PRESION = 0.001;	
	const double SizeW = W;
	const double SizeH = H;	
	const double AngleDEG = JetAPI::AdjustRotationAngle(Angle);//角度, 注意回傳區間為-360 ~ 360	
	TPOINT2D SymCenterPos = CAOIRgn::GetRgnCadSymmetryCenterPos();
	TPOINT2D RotateCenterPos = CAOIRgn::GetRgnCadRotateCenterPos();

	if ( fabs(AngleDEG-0.0)<DBL_PRESION || fabs(AngleDEG-180.0)<DBL_PRESION || fabs(AngleDEG-360.0)<DBL_PRESION )
	{
		CornerPos[0].x = SymCenterPos.x-(SizeW*0.5);
		CornerPos[0].y = SymCenterPos.y-(SizeH*0.5);
		CornerPos[1].x = SymCenterPos.x+(SizeW*0.5);
		CornerPos[1].y = CornerPos[0].y;
		CornerPos[2].x = CornerPos[1].x;
		CornerPos[2].y = SymCenterPos.y+(SizeH*0.5);
		CornerPos[3].x = CornerPos[0].x;
		CornerPos[3].y = CornerPos[2].y;
	}
	else if ( fabs(AngleDEG-90.0)<DBL_PRESION || fabs(AngleDEG-270.0)<DBL_PRESION )
	{
		CornerPos[0].x = SymCenterPos.x-(SizeH*0.5);
		CornerPos[0].y = SymCenterPos.y-(SizeW*0.5);
		CornerPos[1].x = SymCenterPos.x+(SizeH*0.5);
		CornerPos[1].y = CornerPos[0].y;
		CornerPos[2].x = CornerPos[1].x;
		CornerPos[2].y = SymCenterPos.y+(SizeW*0.5);
		CornerPos[3].x = CornerPos[0].x;
		CornerPos[3].y = CornerPos[2].y;
	}	
	else
	{
		double angle = 0;
		const double AngleRAD = AngleDEG*DEG_TO_RAD_DBL;
		const double Length = sqrt((SizeW*SizeW)+(SizeH*SizeH))*0.5;
		const double Arc = ::atan2(SizeH, SizeW);
		//Left Up, +90
		const double AngleRad090 = PI_RAD*0.5;
		const double AngleRad270 = PI_RAD*1.5;
		
		const double SizeW2 = SizeW*0.5;
		const double SizeH2 = SizeH*0.5;	
		CornerPos[0].x = SymCenterPos.x-SizeW2;
		CornerPos[0].y = SymCenterPos.y-SizeH2;
		CornerPos[1].x = SymCenterPos.x+SizeW2;
		CornerPos[1].y = SymCenterPos.y-SizeH2;
		CornerPos[2].x = SymCenterPos.x+SizeW2;
		CornerPos[2].y = SymCenterPos.y+SizeH2;
		CornerPos[3].x = SymCenterPos.x-SizeW2;
		CornerPos[3].y = SymCenterPos.y+SizeH2;
		JetAPI::RotateCornerPos(AngleDEG, RotateCenterPos.x, RotateCenterPos.y, CornerPos);		
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CAOIRgn::LayoutRgnStageCornerPos()//更新機台端點座標
{
	TPOINT2D CenterPos = m_RgnCadPos;
	const bool SignX = AOIDataCollect.GetStageSignPositiveX();
	const bool SignY = AOIDataCollect.GetStageSignPositiveY();

	if ( true == SignX )
	{
		//Roi
		m_RgnRoiStageCornerPos[0].x = m_RgnStagePos.x+m_RgnRoiCadCornerPos[0].x-CenterPos.x;
		m_RgnRoiStageCornerPos[1].x = m_RgnStagePos.x+m_RgnRoiCadCornerPos[1].x-CenterPos.x;
		m_RgnRoiStageCornerPos[2].x = m_RgnStagePos.x+m_RgnRoiCadCornerPos[2].x-CenterPos.x;
		m_RgnRoiStageCornerPos[3].x = m_RgnStagePos.x+m_RgnRoiCadCornerPos[3].x-CenterPos.x;
		//Body
		m_RgnBodyStageCornerPos[0].x = m_RgnStagePos.x+m_RgnBodyCadCornerPos[0].x-CenterPos.x;
		m_RgnBodyStageCornerPos[1].x = m_RgnStagePos.x+m_RgnBodyCadCornerPos[1].x-CenterPos.x;
		m_RgnBodyStageCornerPos[2].x = m_RgnStagePos.x+m_RgnBodyCadCornerPos[2].x-CenterPos.x;
		m_RgnBodyStageCornerPos[3].x = m_RgnStagePos.x+m_RgnBodyCadCornerPos[3].x-CenterPos.x;
	}
	else
	{
		//Roi
		m_RgnRoiStageCornerPos[0].x = m_RgnStagePos.x-m_RgnRoiCadCornerPos[0].x+CenterPos.x;
		m_RgnRoiStageCornerPos[1].x = m_RgnStagePos.x-m_RgnRoiCadCornerPos[1].x+CenterPos.x;
		m_RgnRoiStageCornerPos[2].x = m_RgnStagePos.x-m_RgnRoiCadCornerPos[2].x+CenterPos.x;
		m_RgnRoiStageCornerPos[3].x = m_RgnStagePos.x-m_RgnRoiCadCornerPos[3].x+CenterPos.x;
		//Body
		m_RgnBodyStageCornerPos[0].x = m_RgnStagePos.x-m_RgnBodyCadCornerPos[0].x+CenterPos.x;
		m_RgnBodyStageCornerPos[1].x = m_RgnStagePos.x-m_RgnBodyCadCornerPos[1].x+CenterPos.x;
		m_RgnBodyStageCornerPos[2].x = m_RgnStagePos.x-m_RgnBodyCadCornerPos[2].x+CenterPos.x;
		m_RgnBodyStageCornerPos[3].x = m_RgnStagePos.x-m_RgnBodyCadCornerPos[3].x+CenterPos.x;
	}

	if ( true == SignY )
	{
		//Roi
		m_RgnRoiStageCornerPos[0].y = m_RgnStagePos.y+m_RgnRoiCadCornerPos[0].y-CenterPos.y;	
		m_RgnRoiStageCornerPos[1].y = m_RgnStagePos.y+m_RgnRoiCadCornerPos[1].y-CenterPos.y;	
		m_RgnRoiStageCornerPos[2].y = m_RgnStagePos.y+m_RgnRoiCadCornerPos[2].y-CenterPos.y;	
		m_RgnRoiStageCornerPos[3].y = m_RgnStagePos.y+m_RgnRoiCadCornerPos[3].y-CenterPos.y;
		//Body
		m_RgnBodyStageCornerPos[0].y = m_RgnStagePos.y+m_RgnBodyCadCornerPos[0].y-CenterPos.y;	
		m_RgnBodyStageCornerPos[1].y = m_RgnStagePos.y+m_RgnBodyCadCornerPos[1].y-CenterPos.y;	
		m_RgnBodyStageCornerPos[2].y = m_RgnStagePos.y+m_RgnBodyCadCornerPos[2].y-CenterPos.y;	
		m_RgnBodyStageCornerPos[3].y = m_RgnStagePos.y+m_RgnBodyCadCornerPos[3].y-CenterPos.y;
	}
	else
	{
		//Roi
		m_RgnRoiStageCornerPos[0].y = m_RgnStagePos.y-m_RgnRoiCadCornerPos[0].y+CenterPos.y;	
		m_RgnRoiStageCornerPos[1].y = m_RgnStagePos.y-m_RgnRoiCadCornerPos[1].y+CenterPos.y;	
		m_RgnRoiStageCornerPos[2].y = m_RgnStagePos.y-m_RgnRoiCadCornerPos[2].y+CenterPos.y;	
		m_RgnRoiStageCornerPos[3].y = m_RgnStagePos.y-m_RgnRoiCadCornerPos[3].y+CenterPos.y;
		//Body
		m_RgnBodyStageCornerPos[0].y = m_RgnStagePos.y-m_RgnBodyCadCornerPos[0].y+CenterPos.y;	
		m_RgnBodyStageCornerPos[1].y = m_RgnStagePos.y-m_RgnBodyCadCornerPos[1].y+CenterPos.y;	
		m_RgnBodyStageCornerPos[2].y = m_RgnStagePos.y-m_RgnBodyCadCornerPos[2].y+CenterPos.y;	
		m_RgnBodyStageCornerPos[3].y = m_RgnStagePos.y-m_RgnBodyCadCornerPos[3].y+CenterPos.y;
	}	
}
//-------------------------------------------------------------------------------------//
void CAOIRgn::SetRgnFrameImageStageOffset_um(const TPOINT2D &value)
{
	TPOINT2D CadOffset;
	AOIDataCollect.MapStageOffsetPtToCad(value, CadOffset);
	SetRgnFrameImageCadOffset_um(CadOffset);	
}
//-------------------------------------------------------------------------------------//
void CAOIRgn::MapRgnCadToStagePos(const CMapCoordinate &Map)//將CAD轉成機台座標
{
	Map.Map2D(m_RgnCadPos.x, m_RgnCadPos.y, m_RgnStagePos.x, m_RgnStagePos.y);
	CAOIRgn::LayoutRgnStageCornerPos();
}
//-------------------------------------------------------------------------------------//
bool CAOIRgn::SpinRgn(double Angle)//區域自旋轉
{
	double CadAngle = 0;
	double StageAngle = 0;
	CadAngle = Angle;
	AOIDataCollect.MapCadAngleToStage(CadAngle, StageAngle);

	const double CadCpX = CAOIRgn::m_RgnCadPos.x;
	const double CadCpY = CAOIRgn::m_RgnCadPos.y;
	const double StageCpX = CAOIRgn::m_RgnStagePos.x;
	const double StageCpY = CAOIRgn::m_RgnStagePos.y;

	CAOIRgn::RotateRgnCad(CadAngle, CadCpX, CadCpY);
	CAOIRgn::RotateRgnStage(StageAngle, StageCpX, StageCpY);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIRgn::RotateRgnCad(double Angle, double CpX, double CpY)//區域旋轉-Cad
{
	double dX=0, dY=0;
	const double AngleDeg = JetAPI::AdjustRotationAngle(Angle);	//角度, 注意回傳區間為-360 ~ 360
	const double DBL_Precesion = DBL_PRECISION;	
	if ( fabs(AngleDeg-0.0)<DBL_Precesion || fabs(AngleDeg-360.0)<DBL_Precesion )
	{
	}
	else if ( fabs(AngleDeg-90.0)<DBL_Precesion )//X->-Y, Y->X
	{
		dX = CAOIRgn::m_RgnCadBiasPos.x-0;
		dY = CAOIRgn::m_RgnCadBiasPos.y-0;
		CAOIRgn::m_RgnCadBiasPos.x = 0-dY;
		CAOIRgn::m_RgnCadBiasPos.y = 0+dX;

		dX = CAOIRgn::m_RgnCadPos.x-CpX;
		dY = CAOIRgn::m_RgnCadPos.y-CpY;
		CAOIRgn::m_RgnCadPos.x = CpX-dY;
		CAOIRgn::m_RgnCadPos.y = CpY+dX;				
		//Roi
		dX = CAOIRgn::m_RgnRoiCadCornerPos[0].x-CpX;
		dY = CAOIRgn::m_RgnRoiCadCornerPos[0].y-CpY;
		CAOIRgn::m_RgnRoiCadCornerPos[0].x = CpX-dY;
		CAOIRgn::m_RgnRoiCadCornerPos[0].y = CpY+dX;
		dX = CAOIRgn::m_RgnRoiCadCornerPos[1].x-CpX;
		dY = CAOIRgn::m_RgnRoiCadCornerPos[1].y-CpY;
		CAOIRgn::m_RgnRoiCadCornerPos[1].x = CpX-dY;
		CAOIRgn::m_RgnRoiCadCornerPos[1].y = CpY+dX;
		dX = CAOIRgn::m_RgnRoiCadCornerPos[2].x-CpX;
		dY = CAOIRgn::m_RgnRoiCadCornerPos[2].y-CpY;
		CAOIRgn::m_RgnRoiCadCornerPos[2].x = CpX-dY;
		CAOIRgn::m_RgnRoiCadCornerPos[2].y = CpY+dX;
		dX = CAOIRgn::m_RgnRoiCadCornerPos[3].x-CpX;
		dY = CAOIRgn::m_RgnRoiCadCornerPos[3].y-CpY;
		CAOIRgn::m_RgnRoiCadCornerPos[3].x = CpX-dY;
		CAOIRgn::m_RgnRoiCadCornerPos[3].y = CpY+dX;
		//Body
		dX = CAOIRgn::m_RgnBodyCadCornerPos[0].x-CpX;
		dY = CAOIRgn::m_RgnBodyCadCornerPos[0].y-CpY;
		CAOIRgn::m_RgnBodyCadCornerPos[0].x = CpX-dY;
		CAOIRgn::m_RgnBodyCadCornerPos[0].y = CpY+dX;
		dX = CAOIRgn::m_RgnBodyCadCornerPos[1].x-CpX;
		dY = CAOIRgn::m_RgnBodyCadCornerPos[1].y-CpY;
		CAOIRgn::m_RgnBodyCadCornerPos[1].x = CpX-dY;
		CAOIRgn::m_RgnBodyCadCornerPos[1].y = CpY+dX;
		dX = CAOIRgn::m_RgnBodyCadCornerPos[2].x-CpX;
		dY = CAOIRgn::m_RgnBodyCadCornerPos[2].y-CpY;
		CAOIRgn::m_RgnBodyCadCornerPos[2].x = CpX-dY;
		CAOIRgn::m_RgnBodyCadCornerPos[2].y = CpY+dX;
		dX = CAOIRgn::m_RgnBodyCadCornerPos[3].x-CpX;
		dY = CAOIRgn::m_RgnBodyCadCornerPos[3].y-CpY;
		CAOIRgn::m_RgnBodyCadCornerPos[3].x = CpX-dY;
		CAOIRgn::m_RgnBodyCadCornerPos[3].y = CpY+dX;

		CAOIRgn::m_RgnAngle = JetAPI::AdjustRotationAngle(CAOIRgn::m_RgnAngle+Angle);
	}
	else if ( fabs(AngleDeg-180.0)<DBL_Precesion )//X->-X, Y->-Y
	{
		dX = CAOIRgn::m_RgnCadBiasPos.x-0;
		dY = CAOIRgn::m_RgnCadBiasPos.y-0;
		CAOIRgn::m_RgnCadBiasPos.x = 0-dX;
		CAOIRgn::m_RgnCadBiasPos.y = 0-dY;

		dX = CAOIRgn::m_RgnCadPos.x-CpX;
		dY = CAOIRgn::m_RgnCadPos.y-CpY;
		CAOIRgn::m_RgnCadPos.x = CpX-dX;
		CAOIRgn::m_RgnCadPos.y = CpY-dY;		
		//Roi
		dX = CAOIRgn::m_RgnRoiCadCornerPos[0].x-CpX;
		dY = CAOIRgn::m_RgnRoiCadCornerPos[0].y-CpY;
		CAOIRgn::m_RgnRoiCadCornerPos[0].x = CpX-dX;
		CAOIRgn::m_RgnRoiCadCornerPos[0].y = CpY-dY;
		dX = CAOIRgn::m_RgnRoiCadCornerPos[1].x-CpX;
		dY = CAOIRgn::m_RgnRoiCadCornerPos[1].y-CpY;
		CAOIRgn::m_RgnRoiCadCornerPos[1].x = CpX-dX;
		CAOIRgn::m_RgnRoiCadCornerPos[1].y = CpY-dY;
		dX = CAOIRgn::m_RgnRoiCadCornerPos[2].x-CpX;
		dY = CAOIRgn::m_RgnRoiCadCornerPos[2].y-CpY;
		CAOIRgn::m_RgnRoiCadCornerPos[2].x = CpX-dX;
		CAOIRgn::m_RgnRoiCadCornerPos[2].y = CpY-dY;
		dX = CAOIRgn::m_RgnRoiCadCornerPos[3].x-CpX;
		dY = CAOIRgn::m_RgnRoiCadCornerPos[3].y-CpY;
		CAOIRgn::m_RgnRoiCadCornerPos[3].x = CpX-dX;
		CAOIRgn::m_RgnRoiCadCornerPos[3].y = CpY-dY;				
		//Body
		dX = CAOIRgn::m_RgnBodyCadCornerPos[0].x-CpX;
		dY = CAOIRgn::m_RgnBodyCadCornerPos[0].y-CpY;
		CAOIRgn::m_RgnBodyCadCornerPos[0].x = CpX-dX;
		CAOIRgn::m_RgnBodyCadCornerPos[0].y = CpY-dY;
		dX = CAOIRgn::m_RgnBodyCadCornerPos[1].x-CpX;
		dY = CAOIRgn::m_RgnBodyCadCornerPos[1].y-CpY;
		CAOIRgn::m_RgnBodyCadCornerPos[1].x = CpX-dX;
		CAOIRgn::m_RgnBodyCadCornerPos[1].y = CpY-dY;
		dX = CAOIRgn::m_RgnBodyCadCornerPos[2].x-CpX;
		dY = CAOIRgn::m_RgnBodyCadCornerPos[2].y-CpY;
		CAOIRgn::m_RgnBodyCadCornerPos[2].x = CpX-dX;
		CAOIRgn::m_RgnBodyCadCornerPos[2].y = CpY-dY;
		dX = CAOIRgn::m_RgnBodyCadCornerPos[3].x-CpX;
		dY = CAOIRgn::m_RgnBodyCadCornerPos[3].y-CpY;
		CAOIRgn::m_RgnBodyCadCornerPos[3].x = CpX-dX;
		CAOIRgn::m_RgnBodyCadCornerPos[3].y = CpY-dY;	
		CAOIRgn::m_RgnAngle = JetAPI::AdjustRotationAngle(CAOIRgn::m_RgnAngle+Angle);	
	}
	else if ( fabs(AngleDeg-270.0)<DBL_Precesion )//X->Y, Y->-X
	{
		dX = CAOIRgn::m_RgnCadBiasPos.x-0;
		dY = CAOIRgn::m_RgnCadBiasPos.y-0;
		CAOIRgn::m_RgnCadBiasPos.x = 0+dY;
		CAOIRgn::m_RgnCadBiasPos.y = 0-dX;

		dX = CAOIRgn::m_RgnCadPos.x-CpX;
		dY = CAOIRgn::m_RgnCadPos.y-CpY;
		CAOIRgn::m_RgnCadPos.x = CpX+dY;
		CAOIRgn::m_RgnCadPos.y = CpY-dX;		
		//Roi
		dX = CAOIRgn::m_RgnRoiCadCornerPos[0].x-CpX;
		dY = CAOIRgn::m_RgnRoiCadCornerPos[0].y-CpY;
		CAOIRgn::m_RgnRoiCadCornerPos[0].x = CpX+dY;
		CAOIRgn::m_RgnRoiCadCornerPos[0].y = CpY-dX;
		dX = CAOIRgn::m_RgnRoiCadCornerPos[1].x-CpX;
		dY = CAOIRgn::m_RgnRoiCadCornerPos[1].y-CpY;
		CAOIRgn::m_RgnRoiCadCornerPos[1].x = CpX+dY;
		CAOIRgn::m_RgnRoiCadCornerPos[1].y = CpY-dX;
		dX = CAOIRgn::m_RgnRoiCadCornerPos[2].x-CpX;
		dY = CAOIRgn::m_RgnRoiCadCornerPos[2].y-CpY;
		CAOIRgn::m_RgnRoiCadCornerPos[2].x = CpX+dY;
		CAOIRgn::m_RgnRoiCadCornerPos[2].y = CpY-dX;
		dX = CAOIRgn::m_RgnRoiCadCornerPos[3].x-CpX;
		dY = CAOIRgn::m_RgnRoiCadCornerPos[3].y-CpY;
		CAOIRgn::m_RgnRoiCadCornerPos[3].x = CpX+dY;
		CAOIRgn::m_RgnRoiCadCornerPos[3].y = CpY-dX;
		//Body
		dX = CAOIRgn::m_RgnBodyCadCornerPos[0].x-CpX;
		dY = CAOIRgn::m_RgnBodyCadCornerPos[0].y-CpY;
		CAOIRgn::m_RgnBodyCadCornerPos[0].x = CpX+dY;
		CAOIRgn::m_RgnBodyCadCornerPos[0].y = CpY-dX;
		dX = CAOIRgn::m_RgnBodyCadCornerPos[1].x-CpX;
		dY = CAOIRgn::m_RgnBodyCadCornerPos[1].y-CpY;
		CAOIRgn::m_RgnBodyCadCornerPos[1].x = CpX+dY;
		CAOIRgn::m_RgnBodyCadCornerPos[1].y = CpY-dX;
		dX = CAOIRgn::m_RgnBodyCadCornerPos[2].x-CpX;
		dY = CAOIRgn::m_RgnBodyCadCornerPos[2].y-CpY;
		CAOIRgn::m_RgnBodyCadCornerPos[2].x = CpX+dY;
		CAOIRgn::m_RgnBodyCadCornerPos[2].y = CpY-dX;
		dX = CAOIRgn::m_RgnBodyCadCornerPos[3].x-CpX;
		dY = CAOIRgn::m_RgnBodyCadCornerPos[3].y-CpY;
		CAOIRgn::m_RgnBodyCadCornerPos[3].x = CpX+dY;
		CAOIRgn::m_RgnBodyCadCornerPos[3].y = CpY-dX;
		CAOIRgn::m_RgnAngle = JetAPI::AdjustRotationAngle(CAOIRgn::m_RgnAngle+Angle);	
	}
	else
	{
		const double AngleRad = AngleDeg*DEG_TO_RAD_DBL;		
		JetAPI::RotatePos(CAOIRgn::m_RgnCadBiasPos.x, CAOIRgn::m_RgnCadBiasPos.y, 0, 0, AngleRad, CAOIRgn::m_RgnCadBiasPos.x, CAOIRgn::m_RgnCadBiasPos.y);
		JetAPI::RotatePos(CAOIRgn::m_RgnCadPos.x, CAOIRgn::m_RgnCadPos.y, CpX, CpY, AngleRad, CAOIRgn::m_RgnCadPos.x, CAOIRgn::m_RgnCadPos.y);		
		//Roi
		JetAPI::RotatePos(CAOIRgn::m_RgnRoiCadCornerPos[0].x, CAOIRgn::m_RgnRoiCadCornerPos[0].y, CpX, CpY, AngleRad, CAOIRgn::m_RgnRoiCadCornerPos[0].x, CAOIRgn::m_RgnRoiCadCornerPos[0].y);
		JetAPI::RotatePos(CAOIRgn::m_RgnRoiCadCornerPos[1].x, CAOIRgn::m_RgnRoiCadCornerPos[1].y, CpX, CpY, AngleRad, CAOIRgn::m_RgnRoiCadCornerPos[1].x, CAOIRgn::m_RgnRoiCadCornerPos[1].y);
		JetAPI::RotatePos(CAOIRgn::m_RgnRoiCadCornerPos[2].x, CAOIRgn::m_RgnRoiCadCornerPos[2].y, CpX, CpY, AngleRad, CAOIRgn::m_RgnRoiCadCornerPos[2].x, CAOIRgn::m_RgnRoiCadCornerPos[2].y);
		JetAPI::RotatePos(CAOIRgn::m_RgnRoiCadCornerPos[3].x, CAOIRgn::m_RgnRoiCadCornerPos[3].y, CpX, CpY, AngleRad, CAOIRgn::m_RgnRoiCadCornerPos[3].x, CAOIRgn::m_RgnRoiCadCornerPos[3].y);		
		//Body
		JetAPI::RotatePos(CAOIRgn::m_RgnBodyCadCornerPos[0].x, CAOIRgn::m_RgnBodyCadCornerPos[0].y, CpX, CpY, AngleRad, CAOIRgn::m_RgnBodyCadCornerPos[0].x, CAOIRgn::m_RgnBodyCadCornerPos[0].y);
		JetAPI::RotatePos(CAOIRgn::m_RgnBodyCadCornerPos[1].x, CAOIRgn::m_RgnBodyCadCornerPos[1].y, CpX, CpY, AngleRad, CAOIRgn::m_RgnBodyCadCornerPos[1].x, CAOIRgn::m_RgnBodyCadCornerPos[1].y);
		JetAPI::RotatePos(CAOIRgn::m_RgnBodyCadCornerPos[2].x, CAOIRgn::m_RgnBodyCadCornerPos[2].y, CpX, CpY, AngleRad, CAOIRgn::m_RgnBodyCadCornerPos[2].x, CAOIRgn::m_RgnBodyCadCornerPos[2].y);
		JetAPI::RotatePos(CAOIRgn::m_RgnBodyCadCornerPos[3].x, CAOIRgn::m_RgnBodyCadCornerPos[3].y, CpX, CpY, AngleRad, CAOIRgn::m_RgnBodyCadCornerPos[3].x, CAOIRgn::m_RgnBodyCadCornerPos[3].y);		
		CAOIRgn::m_RgnAngle = JetAPI::AdjustRotationAngle(CAOIRgn::m_RgnAngle+Angle);	
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIRgn::RotateRgnStage(double Angle, double CpX, double CpY)//區域旋轉-Stage
{
	double dX=0, dY=0;
	const double AngleDeg = JetAPI::AdjustRotationAngle(Angle);	//角度, 注意回傳區間為-360 ~ 360
	const double DBL_Precesion = DBL_PRECISION;	
	if ( fabs(AngleDeg-0.0)<DBL_Precesion || fabs(AngleDeg-360.0)<DBL_Precesion )
	{
	}
	else if ( fabs(AngleDeg-90.0)<DBL_Precesion )//X->Y, Y->-X
	{
		dX = CAOIRgn::m_RgnStagePos.x-CpX;
		dY = CAOIRgn::m_RgnStagePos.y-CpY;
		CAOIRgn::m_RgnStagePos.x = CpX-dY;
		CAOIRgn::m_RgnStagePos.y = CpY+dX;
		//Roi
		dX = CAOIRgn::m_RgnRoiStageCornerPos[0].x-CpX;
		dY = CAOIRgn::m_RgnRoiStageCornerPos[0].y-CpY;
		CAOIRgn::m_RgnRoiStageCornerPos[0].x = CpX-dY;
		CAOIRgn::m_RgnRoiStageCornerPos[0].y = CpY+dX;
		dX = CAOIRgn::m_RgnRoiStageCornerPos[1].x-CpX;
		dY = CAOIRgn::m_RgnRoiStageCornerPos[1].y-CpY;
		CAOIRgn::m_RgnRoiStageCornerPos[1].x = CpX-dY;
		CAOIRgn::m_RgnRoiStageCornerPos[1].y = CpY+dX;
		dX = CAOIRgn::m_RgnRoiStageCornerPos[2].x-CpX;
		dY = CAOIRgn::m_RgnRoiStageCornerPos[2].y-CpY;
		CAOIRgn::m_RgnRoiStageCornerPos[2].x = CpX-dY;
		CAOIRgn::m_RgnRoiStageCornerPos[2].y = CpY+dX;
		dX = CAOIRgn::m_RgnRoiStageCornerPos[3].x-CpX;
		dY = CAOIRgn::m_RgnRoiStageCornerPos[3].y-CpY;
		CAOIRgn::m_RgnRoiStageCornerPos[3].x = CpX-dY;
		CAOIRgn::m_RgnRoiStageCornerPos[3].y = CpY+dX;
		//Body
		dX = CAOIRgn::m_RgnBodyStageCornerPos[0].x-CpX;
		dY = CAOIRgn::m_RgnBodyStageCornerPos[0].y-CpY;
		CAOIRgn::m_RgnBodyStageCornerPos[0].x = CpX-dY;
		CAOIRgn::m_RgnBodyStageCornerPos[0].y = CpY+dX;
		dX = CAOIRgn::m_RgnBodyStageCornerPos[1].x-CpX;
		dY = CAOIRgn::m_RgnBodyStageCornerPos[1].y-CpY;
		CAOIRgn::m_RgnBodyStageCornerPos[1].x = CpX-dY;
		CAOIRgn::m_RgnBodyStageCornerPos[1].y = CpY+dX;
		dX = CAOIRgn::m_RgnBodyStageCornerPos[2].x-CpX;
		dY = CAOIRgn::m_RgnBodyStageCornerPos[2].y-CpY;
		CAOIRgn::m_RgnBodyStageCornerPos[2].x = CpX-dY;
		CAOIRgn::m_RgnBodyStageCornerPos[2].y = CpY+dX;
		dX = CAOIRgn::m_RgnBodyStageCornerPos[3].x-CpX;
		dY = CAOIRgn::m_RgnBodyStageCornerPos[3].y-CpY;
		CAOIRgn::m_RgnBodyStageCornerPos[3].x = CpX-dY;
		CAOIRgn::m_RgnBodyStageCornerPos[3].y = CpY+dX;
	}
	else if ( fabs(AngleDeg-180.0)<DBL_Precesion )//X->-X, Y->-Y
	{
		dX = CAOIRgn::m_RgnStagePos.x-CpX;
		dY = CAOIRgn::m_RgnStagePos.y-CpY;
		CAOIRgn::m_RgnStagePos.x = CpX-dX;
		CAOIRgn::m_RgnStagePos.y = CpY-dY;
		//Roi
		dX = CAOIRgn::m_RgnRoiStageCornerPos[0].x-CpX;
		dY = CAOIRgn::m_RgnRoiStageCornerPos[0].y-CpY;
		CAOIRgn::m_RgnRoiStageCornerPos[0].x = CpX-dX;
		CAOIRgn::m_RgnRoiStageCornerPos[0].y = CpY-dY;
		dX = CAOIRgn::m_RgnRoiStageCornerPos[1].x-CpX;
		dY = CAOIRgn::m_RgnRoiStageCornerPos[1].y-CpY;
		CAOIRgn::m_RgnRoiStageCornerPos[1].x = CpX-dX;
		CAOIRgn::m_RgnRoiStageCornerPos[1].y = CpY-dY;
		dX = CAOIRgn::m_RgnRoiStageCornerPos[2].x-CpX;
		dY = CAOIRgn::m_RgnRoiStageCornerPos[2].y-CpY;
		CAOIRgn::m_RgnRoiStageCornerPos[2].x = CpX-dX;
		CAOIRgn::m_RgnRoiStageCornerPos[2].y = CpY-dY;
		dX = CAOIRgn::m_RgnRoiStageCornerPos[3].x-CpX;
		dY = CAOIRgn::m_RgnRoiStageCornerPos[3].y-CpY;
		CAOIRgn::m_RgnRoiStageCornerPos[3].x = CpX-dX;
		CAOIRgn::m_RgnRoiStageCornerPos[3].y = CpY-dY;	
		//Body
		dX = CAOIRgn::m_RgnBodyStageCornerPos[0].x-CpX;
		dY = CAOIRgn::m_RgnBodyStageCornerPos[0].y-CpY;
		CAOIRgn::m_RgnBodyStageCornerPos[0].x = CpX-dX;
		CAOIRgn::m_RgnBodyStageCornerPos[0].y = CpY-dY;
		dX = CAOIRgn::m_RgnBodyStageCornerPos[1].x-CpX;
		dY = CAOIRgn::m_RgnBodyStageCornerPos[1].y-CpY;
		CAOIRgn::m_RgnBodyStageCornerPos[1].x = CpX-dX;
		CAOIRgn::m_RgnBodyStageCornerPos[1].y = CpY-dY;
		dX = CAOIRgn::m_RgnBodyStageCornerPos[2].x-CpX;
		dY = CAOIRgn::m_RgnBodyStageCornerPos[2].y-CpY;
		CAOIRgn::m_RgnBodyStageCornerPos[2].x = CpX-dX;
		CAOIRgn::m_RgnBodyStageCornerPos[2].y = CpY-dY;
		dX = CAOIRgn::m_RgnBodyStageCornerPos[3].x-CpX;
		dY = CAOIRgn::m_RgnBodyStageCornerPos[3].y-CpY;
		CAOIRgn::m_RgnBodyStageCornerPos[3].x = CpX-dX;
		CAOIRgn::m_RgnBodyStageCornerPos[3].y = CpY-dY;		
	}
	else if ( fabs(AngleDeg-270.0)<DBL_Precesion )//X->-Y, Y->X
	{
		dX = CAOIRgn::m_RgnStagePos.x-CpX;
		dY = CAOIRgn::m_RgnStagePos.y-CpY;
		CAOIRgn::m_RgnStagePos.x = CpX-dY;
		CAOIRgn::m_RgnStagePos.y = CpY+dX;
		//Roi
		dX = CAOIRgn::m_RgnRoiStageCornerPos[0].x-CpX;
		dY = CAOIRgn::m_RgnRoiStageCornerPos[0].y-CpY;
		CAOIRgn::m_RgnRoiStageCornerPos[0].x = CpX-dY;
		CAOIRgn::m_RgnRoiStageCornerPos[0].y = CpY+dX;
		dX = CAOIRgn::m_RgnRoiStageCornerPos[1].x-CpX;
		dY = CAOIRgn::m_RgnRoiStageCornerPos[1].y-CpY;
		CAOIRgn::m_RgnRoiStageCornerPos[1].x = CpX-dY;
		CAOIRgn::m_RgnRoiStageCornerPos[1].y = CpY+dX;
		dX = CAOIRgn::m_RgnRoiStageCornerPos[2].x-CpX;
		dY = CAOIRgn::m_RgnRoiStageCornerPos[2].y-CpY;
		CAOIRgn::m_RgnRoiStageCornerPos[2].x = CpX-dY;
		CAOIRgn::m_RgnRoiStageCornerPos[2].y = CpY+dX;
		dX = CAOIRgn::m_RgnRoiStageCornerPos[3].x-CpX;
		dY = CAOIRgn::m_RgnRoiStageCornerPos[3].y-CpY;
		CAOIRgn::m_RgnRoiStageCornerPos[3].x = CpX-dY;
		CAOIRgn::m_RgnRoiStageCornerPos[3].y = CpY+dX;
		//Body
		dX = CAOIRgn::m_RgnBodyStageCornerPos[0].x-CpX;
		dY = CAOIRgn::m_RgnBodyStageCornerPos[0].y-CpY;
		CAOIRgn::m_RgnBodyStageCornerPos[0].x = CpX-dY;
		CAOIRgn::m_RgnBodyStageCornerPos[0].y = CpY+dX;
		dX = CAOIRgn::m_RgnBodyStageCornerPos[1].x-CpX;
		dY = CAOIRgn::m_RgnBodyStageCornerPos[1].y-CpY;
		CAOIRgn::m_RgnBodyStageCornerPos[1].x = CpX-dY;
		CAOIRgn::m_RgnBodyStageCornerPos[1].y = CpY+dX;
		dX = CAOIRgn::m_RgnBodyStageCornerPos[2].x-CpX;
		dY = CAOIRgn::m_RgnBodyStageCornerPos[2].y-CpY;
		CAOIRgn::m_RgnBodyStageCornerPos[2].x = CpX-dY;
		CAOIRgn::m_RgnBodyStageCornerPos[2].y = CpY+dX;
		dX = CAOIRgn::m_RgnBodyStageCornerPos[3].x-CpX;
		dY = CAOIRgn::m_RgnBodyStageCornerPos[3].y-CpY;
		CAOIRgn::m_RgnBodyStageCornerPos[3].x = CpX-dY;
		CAOIRgn::m_RgnBodyStageCornerPos[3].y = CpY+dX;
	}
	else
	{
		const double AngleRad = AngleDeg*DEG_TO_RAD_DBL;		
		JetAPI::RotatePos(CAOIRgn::m_RgnStagePos.x, CAOIRgn::m_RgnStagePos.y, CpX, CpY, AngleRad, CAOIRgn::m_RgnStagePos.x, CAOIRgn::m_RgnStagePos.y);
		//Roi
		JetAPI::RotatePos(CAOIRgn::m_RgnRoiStageCornerPos[0].x, CAOIRgn::m_RgnRoiStageCornerPos[0].y, CpX, CpY, AngleRad, CAOIRgn::m_RgnRoiStageCornerPos[0].x, CAOIRgn::m_RgnRoiStageCornerPos[0].y);
		JetAPI::RotatePos(CAOIRgn::m_RgnRoiStageCornerPos[1].x, CAOIRgn::m_RgnRoiStageCornerPos[1].y, CpX, CpY, AngleRad, CAOIRgn::m_RgnRoiStageCornerPos[1].x, CAOIRgn::m_RgnRoiStageCornerPos[1].y);
		JetAPI::RotatePos(CAOIRgn::m_RgnRoiStageCornerPos[2].x, CAOIRgn::m_RgnRoiStageCornerPos[2].y, CpX, CpY, AngleRad, CAOIRgn::m_RgnRoiStageCornerPos[2].x, CAOIRgn::m_RgnRoiStageCornerPos[2].y);
		JetAPI::RotatePos(CAOIRgn::m_RgnRoiStageCornerPos[3].x, CAOIRgn::m_RgnRoiStageCornerPos[3].y, CpX, CpY, AngleRad, CAOIRgn::m_RgnRoiStageCornerPos[3].x, CAOIRgn::m_RgnRoiStageCornerPos[3].y);
		//Body
		JetAPI::RotatePos(CAOIRgn::m_RgnBodyStageCornerPos[0].x, CAOIRgn::m_RgnBodyStageCornerPos[0].y, CpX, CpY, AngleRad, CAOIRgn::m_RgnBodyStageCornerPos[0].x, CAOIRgn::m_RgnBodyStageCornerPos[0].y);
		JetAPI::RotatePos(CAOIRgn::m_RgnBodyStageCornerPos[1].x, CAOIRgn::m_RgnBodyStageCornerPos[1].y, CpX, CpY, AngleRad, CAOIRgn::m_RgnBodyStageCornerPos[1].x, CAOIRgn::m_RgnBodyStageCornerPos[1].y);
		JetAPI::RotatePos(CAOIRgn::m_RgnBodyStageCornerPos[2].x, CAOIRgn::m_RgnBodyStageCornerPos[2].y, CpX, CpY, AngleRad, CAOIRgn::m_RgnBodyStageCornerPos[2].x, CAOIRgn::m_RgnBodyStageCornerPos[2].y);
		JetAPI::RotatePos(CAOIRgn::m_RgnBodyStageCornerPos[3].x, CAOIRgn::m_RgnBodyStageCornerPos[3].y, CpX, CpY, AngleRad, CAOIRgn::m_RgnBodyStageCornerPos[3].x, CAOIRgn::m_RgnBodyStageCornerPos[3].y);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIRgn::MirrorXRgnCad(double CpX)//區域鏡射-X
{
	double dX=0;

	dX = CAOIRgn::m_RgnCadPos.x-CpX;	
	CAOIRgn::m_RgnCadPos.x = CpX-dX;

	//dX = CAOIRgn::m_RgnCadBiasPos.x-CpX;	
	//CAOIRgn::m_RgnCadBiasPos.x = CpX-dX;	
	CAOIRgn::m_RgnCadBiasPos.x = -CAOIRgn::m_RgnCadBiasPos.x;

	//Roi
	dX = CAOIRgn::m_RgnRoiCadCornerPos[0].x-CpX;
	CAOIRgn::m_RgnRoiCadCornerPos[0].x = CpX-dX;
	dX = CAOIRgn::m_RgnRoiCadCornerPos[1].x-CpX;
	CAOIRgn::m_RgnRoiCadCornerPos[1].x = CpX-dX;
	dX = CAOIRgn::m_RgnRoiCadCornerPos[2].x-CpX;
	CAOIRgn::m_RgnRoiCadCornerPos[2].x = CpX-dX;
	dX = CAOIRgn::m_RgnRoiCadCornerPos[3].x-CpX;
	CAOIRgn::m_RgnRoiCadCornerPos[3].x = CpX-dX;

	//Body
	dX = CAOIRgn::m_RgnBodyCadCornerPos[0].x-CpX;
	CAOIRgn::m_RgnBodyCadCornerPos[0].x = CpX-dX;
	dX = CAOIRgn::m_RgnBodyCadCornerPos[1].x-CpX;
	CAOIRgn::m_RgnBodyCadCornerPos[1].x = CpX-dX;
	dX = CAOIRgn::m_RgnBodyCadCornerPos[2].x-CpX;
	CAOIRgn::m_RgnBodyCadCornerPos[2].x = CpX-dX;
	dX = CAOIRgn::m_RgnBodyCadCornerPos[3].x-CpX;
	CAOIRgn::m_RgnBodyCadCornerPos[3].x = CpX-dX;

	CAOIRgn::m_RgnAngle = 180.0-CAOIRgn::m_RgnAngle;
	CAOIRgn::m_RgnAngle = JetAPI::AdjustRotationAngle(CAOIRgn::m_RgnAngle);		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIRgn::MirrorYRgnCad(double CpY)//區域鏡射-Y
{
	double dY=0;

	dY = CAOIRgn::m_RgnCadPos.y-CpY;	
	CAOIRgn::m_RgnCadPos.y = CpY-dY;

	//dY = CAOIRgn::m_RgnCadBiasPos.y-CpY;	
	//CAOIRgn::m_RgnCadBiasPos.y = CpY-dY;
	CAOIRgn::m_RgnCadBiasPos.y = -CAOIRgn::m_RgnCadBiasPos.y;

	//Roi
	dY = CAOIRgn::m_RgnRoiCadCornerPos[0].y-CpY;
	CAOIRgn::m_RgnRoiCadCornerPos[0].y = CpY-dY;
	dY = CAOIRgn::m_RgnRoiCadCornerPos[1].y-CpY;
	CAOIRgn::m_RgnRoiCadCornerPos[1].y = CpY-dY;
	dY = CAOIRgn::m_RgnRoiCadCornerPos[2].y-CpY;
	CAOIRgn::m_RgnRoiCadCornerPos[2].y = CpY-dY;
	dY = CAOIRgn::m_RgnRoiCadCornerPos[3].y-CpY;
	CAOIRgn::m_RgnRoiCadCornerPos[3].y = CpY-dY;

	//Body
	dY = CAOIRgn::m_RgnBodyCadCornerPos[0].y-CpY;
	CAOIRgn::m_RgnBodyCadCornerPos[0].y = CpY-dY;
	dY = CAOIRgn::m_RgnBodyCadCornerPos[1].y-CpY;
	CAOIRgn::m_RgnBodyCadCornerPos[1].y = CpY-dY;
	dY = CAOIRgn::m_RgnBodyCadCornerPos[2].y-CpY;
	CAOIRgn::m_RgnBodyCadCornerPos[2].y = CpY-dY;
	dY = CAOIRgn::m_RgnBodyCadCornerPos[3].y-CpY;
	CAOIRgn::m_RgnBodyCadCornerPos[3].y = CpY-dY;
	
	CAOIRgn::m_RgnAngle = 360-CAOIRgn::m_RgnAngle;
	CAOIRgn::m_RgnAngle = JetAPI::AdjustRotationAngle(CAOIRgn::m_RgnAngle);		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIRgn::MirrorXRgnStage(double CpX)//區域鏡射-X
{
	double dX=0;

	dX = CAOIRgn::m_RgnStagePos.x-CpX;	
	CAOIRgn::m_RgnStagePos.x = CpX-dX;		
	//Roi
	dX = CAOIRgn::m_RgnRoiStageCornerPos[0].x-CpX;
	CAOIRgn::m_RgnRoiStageCornerPos[0].x = CpX-dX;
	dX = CAOIRgn::m_RgnRoiStageCornerPos[1].x-CpX;
	CAOIRgn::m_RgnRoiStageCornerPos[1].x = CpX-dX;
	dX = CAOIRgn::m_RgnRoiStageCornerPos[2].x-CpX;
	CAOIRgn::m_RgnRoiStageCornerPos[2].x = CpX-dX;
	dX = CAOIRgn::m_RgnRoiStageCornerPos[3].x-CpX;
	CAOIRgn::m_RgnRoiStageCornerPos[3].x = CpX-dX;

	//Body
	dX = CAOIRgn::m_RgnBodyStageCornerPos[0].x-CpX;
	CAOIRgn::m_RgnBodyStageCornerPos[0].x = CpX-dX;
	dX = CAOIRgn::m_RgnBodyStageCornerPos[1].x-CpX;
	CAOIRgn::m_RgnBodyStageCornerPos[1].x = CpX-dX;
	dX = CAOIRgn::m_RgnBodyStageCornerPos[2].x-CpX;
	CAOIRgn::m_RgnBodyStageCornerPos[2].x = CpX-dX;
	dX = CAOIRgn::m_RgnBodyStageCornerPos[3].x-CpX;
	CAOIRgn::m_RgnBodyStageCornerPos[3].x = CpX-dX;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIRgn::MirrorYRgnStage(double CpY)//區域鏡射-Y
{
	double dY=0;

	dY = CAOIRgn::m_RgnStagePos.y-CpY;	
	CAOIRgn::m_RgnStagePos.y = CpY-dY;
	//Roi
	dY = CAOIRgn::m_RgnRoiStageCornerPos[0].y-CpY;
	CAOIRgn::m_RgnRoiStageCornerPos[0].y = CpY-dY;
	dY = CAOIRgn::m_RgnRoiStageCornerPos[1].y-CpY;
	CAOIRgn::m_RgnRoiStageCornerPos[1].y = CpY-dY;
	dY = CAOIRgn::m_RgnRoiStageCornerPos[2].y-CpY;
	CAOIRgn::m_RgnRoiStageCornerPos[2].y = CpY-dY;
	dY = CAOIRgn::m_RgnRoiStageCornerPos[3].y-CpY;
	CAOIRgn::m_RgnRoiStageCornerPos[3].y = CpY-dY;

	//Body
	dY = CAOIRgn::m_RgnBodyStageCornerPos[0].y-CpY;
	CAOIRgn::m_RgnBodyStageCornerPos[0].y = CpY-dY;
	dY = CAOIRgn::m_RgnBodyStageCornerPos[1].y-CpY;
	CAOIRgn::m_RgnBodyStageCornerPos[1].y = CpY-dY;
	dY = CAOIRgn::m_RgnBodyStageCornerPos[2].y-CpY;
	CAOIRgn::m_RgnBodyStageCornerPos[2].y = CpY-dY;
	dY = CAOIRgn::m_RgnBodyStageCornerPos[3].y-CpY;
	CAOIRgn::m_RgnBodyStageCornerPos[3].y = CpY-dY;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIRgn::CalcRgnFrameImageRect()//重新計算影像區域
{	
	CAOIField *FieldPtr = NULL;
	FieldPtr = CAOIRgn::GetRgnFieldPtr();
	if ( NULL == FieldPtr ) { return false; }	

	//分配影像區域
	RECT       FrameRect;
	TSIZE2D    FrameSizeUm;
	TPOINT2D   StagePos2D;
	TREGION4D  Region, CameraRgn;
	CAMERA_ID  CameraID = CAOIRgn::GetRgnCameraID();
	IMAGE_SIZE ImageW = AOIDataCollect.GetCameraImageW(CameraID);
	IMAGE_SIZE ImageH = AOIDataCollect.GetCameraImageH(CameraID);
	const int  nAlign = 4;
	const int  nImageW = (int)(ImageW);
	const int  nImageH = (int)(ImageH);	
	const size_t SubRgnCount = CAOIRgn::GetRgnSubRgnCount();
	CAOIRgn::GetRgnRoiStageRegion(Region);
	StagePos2D.x = FieldPtr->GetFieldStagePosX();
	StagePos2D.y = FieldPtr->GetFieldStagePosY();	
	AOIDataCollect.MapStageRegionToCamera(CameraID, Region, StagePos2D, CameraRgn);		
	JetAPI::Region4DToRect(CameraRgn, FrameRect, true);
	JetAPI::AdjustRectByAlignW(FrameRect, nAlign);//調成4倍寬
	if ( JetAPI::CheckImageRoi(nImageW, nImageH, FrameRect) == false )
	{
		if ( 0 == SubRgnCount )
		{	return false;  }
		else
		{
			if ( FrameRect.left < 0 ) { FrameRect.left = 0; }
			if ( FrameRect.top  < 0 ) { FrameRect.top  = 0; }
			if ( FrameRect.right > nImageW ) { FrameRect.right = nImageW; }
			if ( FrameRect.bottom > nImageH ) { FrameRect.bottom = nImageH; }
		}
	}
	AOIDataCollect.MapImageSizeToReal(CameraID, FrameRect, FrameSizeUm);
	//是否計算四個端點呢!?
	SetRgnFrameImageRect(FrameRect);
	SetRgnFrameImageSize_um(FrameSizeUm);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIRgn::CheckRgnReadyToCalc()//確認檢測區域可以正常計算
{
	//先判定-定位點是否計算完畢
	AOI_OBJ_TYPE ObjType = CAOIRgn::GetObjType();
	if ( AOI_OBJ_FD == ObjType ) { return true; }	

	CAOIRgn   *RgnParent = CAOIRgn::GetRgnParent();
	CAOIBoard *BoardPtr = CAOIRgn::GetRgnBoardPtr();
	if ( NULL != RgnParent )
	{
		ObjType = RgnParent->GetObjType();
		if ( AOI_OBJ_FD == ObjType ) { return true; }
		BoardPtr = RgnParent->GetRgnBoardPtr();
	}
	if ( NULL == BoardPtr ) { return true; }
	if ( BoardPtr->GetBoardCalcMapFinish() == false )
	{	return false; }

	//判定局部基準面是否計算完畢
	if ( GetRgnLocalBasePlaneFinish() == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIRgn::CheckRgnNoNeedToCalculate() const//確認檢測區域是否不再需要計算
{	
	const bool bNeedToCalculate=true;
	if ( GetRgnNeedToCalculate() == false )
	{	return false; }	
	CAOIRgn *RgnParentPtr=GetRgnParent();
	if ( NULL == RgnParentPtr ) { return bNeedToCalculate; }
	if ( RgnParentPtr->GetRgnNeedToCalculate() == false )
	{	return false; }
	return bNeedToCalculate;
}
//-------------------------------------------------------------------------------------//
bool CAOIRgn::CheckRgnFieldAllFrameMergeFinish()//確認區域內的Frame都已經完成
{
	CAOIField* FieldPtr = CAOIRgn::GetRgnFieldPtr();
	if ( NULL == FieldPtr ) { return true; }
	if ( FieldPtr->CheckFieldMergeFinish() == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIRgn::WaitForRgnFieldAllFrameMergeFinish()//等待區域內的所有Frame都已經完成
{
	DWORD CheckCnt=0;
	const DWORD DelayTime=10;
	const DWORD MaxCheckCnt=150;
	while ( true )
	{
		if ( CheckRgnFieldAllFrameMergeFinish() == true )
		{	break; }
		CheckCnt ++;
		if ( CheckCnt >= MaxCheckCnt )
		{	return false; }
		if ( DelayTime > 0 )
		{	::Sleep(DelayTime);  }
	};
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIRgn::ExecRgnCalc_ProjectMap()//執行區域的專案底圖
{
	const char fnName[] = "CAOIRgn::ExecRgnCalc_ProjectMap";
	CAOIField* FieldPtr = CAOIRgn::GetRgnFieldPtr();
	if ( NULL == FieldPtr ) { return false; }
	CAOIProject *Project = FieldPtr->GetFieldProjectPtr();
	if ( NULL == Project ) { return false; }
	CAOIFrame *FramePtr = NULL;	
	size_t i = 0;
	size_t FrameIdx = 0;
	unsigned int ImageIndex = 0;
	FRAME_TYPE   FrameType = FRAME_NULL;
	const size_t FieldIndex = FieldPtr->GetFieldIndex();

	bool           bAllocated = false;
	TPOINT2D       StagePos;
	RECT           RoiRect={0};
	const int      nAlign = 4;
	IMAGE_SIZE     ImageW=0;
	IMAGE_SIZE     ImageH=0;	
	IMAGE_SIZE     ImageStep=0;	
	IMAGE_SIZE     BitCount=0;
	IMAGE_SIZE     BufferSize=0;	
	IMAGE_PTR      ImagePtr = NULL;
	MASK_PTR       MaskPtr = NULL;
	SPACE_PTR      SpacePtr = NULL;	
	const double   SpaceRatio = Project->GetProjectSpaceToGrayRatioMode();

	StagePos.x = FieldPtr->GetFieldStagePosX();
	StagePos.y = FieldPtr->GetFieldStagePosY();	
	const size_t FrameCount = FieldPtr->GetFieldFramePtrCount();
	for ( i=0; i<FrameCount; i++ )
	{
		FrameIdx = i;
		FramePtr = FieldPtr->GetFieldFramePtr(FrameIdx, true);
		if ( NULL == FramePtr ) { return false; }
		FrameType = FramePtr->GetFrameType();
		ImageIndex = FramePtr->GetFrameImageIndex();
		if ( -1 == ImageIndex ) { return false; }
		bAllocated = false;
		if ( FRAME_SPACE == FrameType )
		{
			FramePtr->GetFrameSpacePtr(ImageW, ImageH, ImageStep, BitCount, MaskPtr, SpacePtr);
			if ( NULL==MaskPtr || NULL==SpacePtr ) 
			{	return false; }

			BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
			JetAPI::SizeToRect(ImageW, ImageH, RoiRect);
			if ( JetMemory.alloc_func(BufferSize, ImagePtr, fnName, "ImagePtr") == false )
			{	return false;	}
			if ( ImageAPI.SpaceGrayImageConvertToGray3(ImageW, ImageH, ImageStep, SpacePtr, MaskPtr, RoiRect, ImageStep, ImagePtr, SpaceRatio, false) == false )
			{	return false; }
			bAllocated = true;
		}
		else if ( FRAME_BAYER == FrameType )
		{			
			FramePtr->GetFrameImagePtr(ImageW, ImageH, ImageStep, BitCount, ImagePtr);	
			if ( NULL == ImagePtr ) 
			{	return false; }			
			IMAGE_SIZE   DeBayerBit=0;
			IMAGE_SIZE   DeBayerStep=0;		
			IMAGE_PTR    DeBayerPtr=NULL;
			BAYER_PATTERN_MODE BayerPattern = FramePtr->GetFrameBayerPattern();		
			if ( AOIDataCollect.ExecDebayerImage(fnName, ImageW, ImageH, ImageStep, ImagePtr, BayerPattern, DeBayerStep, DeBayerBit, DeBayerPtr) == false)		
			{	return false;	}
			ImagePtr = DeBayerPtr;
			BitCount = DeBayerBit;
			ImageStep = DeBayerStep;
			bAllocated = true;
			DeBayerPtr = NULL;			
		}
		else
		{	
			FramePtr->GetFrameImagePtr(ImageW, ImageH, ImageStep, BitCount, ImagePtr);	
			if ( NULL == ImagePtr ) 
			{	return false; }
		}
		Project->AddProjectMapPtr(StagePos, ImageIndex, ImageW, ImageH, ImageStep, BitCount, ImagePtr);
	#ifndef _X64
		FramePtr->ClearFrameBuffer();	
	#endif//_X64
		if ( true == bAllocated )
		{	JetMemory.free_func(ImagePtr);	}
		
		ImageW = 0;
		ImageH = 0;
		BitCount = 0;
		ImageStep = 0;
		MaskPtr = NULL;
		SpacePtr = NULL;
		ImagePtr = NULL;		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIRgn::ExtractRgnDerivedFrame(bool &Finished)//挖取區域圖像
{
	if ( CAOIRgn::ExtractRgnFrame(Finished) == false ) { return false; }

	if ( true == Finished )
	{
		if ( CAOIRgn::ExecRgnDerivedInspection() == false )
		{
			CAOIRgn::ClearRgnImageBuffer(); 
			return false;
		}
		CAOIRgn::ClearRgnImageBuffer(); 
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIRgn::SaveRgnMovingTimeMsg(const char *pContext)//儲存移動時間訊息
{	
	return AOIDataCollect.SaveMovingTimeMsg(pContext);
}
//-------------------------------------------------------------------------------------//
bool CAOIRgn::SaveRgnMovingTimeMsg(const wchar_t *pContext)//儲存移動時間訊
{
	return AOIDataCollect.SaveMovingTimeMsg(pContext);
}
//-------------------------------------------------------------------------------------//
bool CAOIRgn::ExtractRgnFrame(bool &Finished)//挖取區域圖像	
{
	Finished = false;
	bool bIsOK = false;	
	if ( CAOIRgn::GetRgnNeedToCalculate() == false ) { return true; }

	CAOIField* FieldPtr = CAOIRgn::GetRgnFieldPtr();
	if ( NULL == FieldPtr ) { return false; }		
	size_t i = 0;
	bool  bFinsih[FRAME_MAX_COUNT] = {true};
	double        Time=0.0;
	LARGE_INTEGER nStartTime;
	LARGE_INTEGER nEndTime;
	unsigned int  FrameIdx = 0;	
	CAOIFrame    *FramePtr=NULL;	
	CAOIProject  *RgnProjectPtr = CAOIRgn::GetRgnProjectPtr();
	FRAME_TYPE    FrameType = FRAME_NULL;
	unsigned int  FrameUniqueID = 0;
	const size_t  FrameCount = FieldPtr->GetFieldFramePtrCount();		
	IMAGE_PTR	  ImagePtr = NULL;
	IMAGE_SIZE	  ImageW = 0;
	IMAGE_SIZE	  ImageH = 0;
	IMAGE_SIZE	  ImageStep = 0;
	unsigned int  BitCount = 0;	
	QueryPerformanceCounter(&nStartTime);
	//CAOIRgn::ClearRgnImageBuffer();
	for ( i=0; i<FrameCount; i++ )
	{
		FramePtr = FieldPtr->GetFieldFramePtr(i, false);
		if ( NULL == FramePtr ) { continue; }
		FrameType = FramePtr->GetFrameType();
		FrameUniqueID = FramePtr->GetFrameUniqueID();
		if ( NULL != RgnProjectPtr )
		{	FrameUniqueID = RgnProjectPtr->GetProjectFrameUniqueID(i, false); }
		SetRgnImageUniqueID(i, FrameUniqueID);
		switch ( FrameType )
		{
		case FRAME_SPACE:			
			bIsOK = ExtractRgnSpace(i, FramePtr, bFinsih[i], ImageW, ImageH, ImageStep, BitCount, ImagePtr);			
			break;
		default:
			bIsOK = ExtractRgnImage(i, FramePtr, bFinsih[i]);
			if (FrameUniqueID==GUIDEIMAGE_UNIQUE_ID && FrameType==FRAME_COLOR && ImagePtr==NULL)//20191203 - Joe
			{
				BitCount = FramePtr->GetFrameImageBitCount();
				ImagePtr = FramePtr->GetFrameImagePtr();
				ImageW = FramePtr->GetFrameImageW();
				ImageH = FramePtr->GetFrameImageH();
				ImageStep = FramePtr->GetFrameImageStep();				
			}
			break;
		}
		if ( false == bIsOK )
		{
			CAOIRgn::ClearRgnImageBuffer();
			if ( GetRgnNeedToCalculate() == false )//v1.01.04.034
			{	return true;	}
			return false;
		}
	}
	QueryPerformanceCounter(&nEndTime);
	Time = (nEndTime.QuadPart-nStartTime.QuadPart)*1000.0/AOIDataCollect.m_SystemFreq.QuadPart;
	CAOIRgn::SetRgnFillImageTime(Time);

	Finished = true;
	for ( i=0; i<FrameCount; i++ )
	{
		if ( false == bFinsih[i] ) 
		{
			Finished = false;
			break;
		}
	}	
	
	//非子檢測框
	CAOIRgn *ParentPtr=CAOIRgn::GetRgnParent();	
	if ( NULL == ParentPtr ) 
	{	return true; }
	
	FrameIdx = 0;
	//判斷是否為最後一個子區域
	CAOIRgn::LockRgn();
	ParentPtr->IncrementRgnSubFillFrameCount();
	const size_t SubRgnCount = ParentPtr->GetRgnSubRgnCount();		
	const size_t FillFrameCount = ParentPtr->GetRgnSubFillFrameCount();
	if ( FillFrameCount != SubRgnCount )
	{
		//Keep Frame Image Buffer, don't set Finish = true;
		CAOIRgn::UnlockRgn();
		return true;
	}	
	CAOIRgn::UnlockRgn();	
	
	//拼出跨FOV的圖檔
	CString ErrStr, ErrName;
	ErrName.Format(_T("%s_Fill"), ParentPtr->GetRgnDerivedName());
	QueryPerformanceCounter(&nStartTime);
	if ( ParentPtr->WaitForRgnFieldAllFrameMergeFinish() == false )
	{
		ErrStr.Format(_T("Error, %s WaitForRgnFieldAllFrameMergeFinish Fault"), ErrName);
		SaveRgnMovingTimeMsg(ErrStr);

		CAOIField* RgnFieldPtr = ParentPtr->GetRgnFieldPtr();
		if ( NULL != RgnFieldPtr )
		{
			FIELD_MERGE_STATE MergeState=RgnFieldPtr->GetFieldMergeState();
			ErrStr.Format(_T("Error, %s WaitForRgnFieldAllFrameMergeFinish Fault [FieldMergeState=%d]"), ErrName, MergeState);
			SaveRgnMovingTimeMsg(ErrStr);
		}
		return false; 
	}
	if ( ParentPtr->FillRgnSubRgnFullFrame() == false )
	{		
		ParentPtr->ClearRgnImageBuffer();
		ParentPtr->ClearRgnMaskBuffer_Base();
		ParentPtr->ClearRgnSubRgnMaskBuffer_Base();
		if ( ParentPtr->GetRgnNeedToCalculate() == false )//v1.01.03.234
		{	return true;	}
		ErrStr.Format(_T("Error, %s FillRgnSubRgnFullFrame Fault"), ErrName);
		SaveRgnMovingTimeMsg(ErrStr);
		return false;
	}	
	QueryPerformanceCounter(&nEndTime);
	Time = (nEndTime.QuadPart-nStartTime.QuadPart)*1000.0/AOIDataCollect.m_SystemFreq.QuadPart;
	ParentPtr->SetRgnFillImageTime(Time);

	CAOIRgn       *RgnPtr = NULL;
	CAOIRgn       *SubRgnPtr = NULL;
	CAOIWindow    *WindowPtr = NULL;
	CAOIBarcode   *BarcodePtr=NULL;	
	CAOIComponent *ComponentPtr = NULL;
	AOI_OBJ_TYPE ParentType = ParentPtr->GetObjType();
	bIsOK = ParentPtr->ExecRgnDerivedInspection();
	ParentPtr->SetRgnSubRgnAllFinish();
	ParentPtr->SetRgnCalculated(true);
	ParentPtr->SetRgnCalcState(REGION_CALC_DONE);
	ParentPtr->ClearRgnMaskBuffer_Base();
	ParentPtr->ClearRgnSubRgnMaskBuffer_Base();
	if ( ParentPtr->GetRgnKeepImage() == false )
	{	ParentPtr->ClearRgnImageBuffer(); }
	if ( false == bIsOK )
	{
		ErrStr.Format(_T("Error, %s ExecRgnDerivedInspection Fault"), ErrName);
		SaveRgnMovingTimeMsg(ErrStr);
		return false;	
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIRgn::ExtractRgnImage(size_t index, CAOIFrame *FramePtr, bool &Finished)//挖取區域圖像
{	
	const char fnName[] = "CAOIRgn::ExtractRgnImage";
	Finished = false;
	CAOIField* FieldPtr = GetRgnFieldPtr();
	if ( NULL == FieldPtr ) { return false; }		
	if ( NULL == FramePtr ) { return false; }
	const size_t FrameIdx = index;
	FRAME_TYPE   FrameType = FramePtr->GetFrameType();
	if ( FRAME_SPACE == FrameType ) { return false; }	

	CString    str;
	CString    ErrStr, ErrName;
	CString    ComName = _T("Rgn");
	bool       bSave = false;
	size_t     k=0;	
	size_t     FillFrameCount = 0;	
	const int  nAlign = 4;
	IMAGE_SIZE ImageW=0;
	IMAGE_SIZE ImageH=0;
	IMAGE_SIZE ImageStep=0;
	IMAGE_SIZE BitCount=0;
	TREGION4D  ImageRgn;
	TREGION4D  ComponentRoiRgn;
	IMAGE_PTR  ImagePtr = NULL;	
	CAOIRgn       *RgnPtr = NULL;
	CAOIRgn       *SubRgnPtr = NULL;
	CAOIWindow    *WindowPtr = NULL;
	CAOIBarcode   *BarcodePtr = NULL;	
	CAOIComponent *ComponentPtr = NULL;
	CAOIRgn       *ParentPtr=GetRgnParent();
	AOI_OBJ_TYPE ParentType = AOI_OBJ_RGN;//AOI_OBJ_RGN;	
	const CAMERA_ID CameraID = GetRgnCameraID();
	const size_t RgnIndex = GetRgnIndex();
	const size_t SubRgnCount = GetRgnSubRgnCount();
	FramePtr->GetFrameImagePtr(ImageW, ImageH, ImageStep, BitCount, ImagePtr);
	if ( NULL!=ParentPtr || 0!=SubRgnCount )//子區域, 或跨FOV, 在拼圖時作業, 這裡跳過
	{	return true; }	
	if ( NULL == ParentPtr )
	{	ComName = this->GetRgnDerivedName();	}
	else
	{	ComName = ParentPtr->GetRgnDerivedName();	}
	if ( NULL == ParentPtr )
	{	ErrName = ComName;	}
	else
	{	ErrName.Format(_T("%s Sub-Rgn"), ComName);	}	

	IMAGE_SIZE RoiStep=0;
	IMAGE_SIZE RoiBitCount=0;
	IMAGE_PTR  RoiPtr=NULL;
	RECT       RoiRect = m_RgnFrameImageRect;
	IMAGE_SIZE RoiW=RoiRect.right-RoiRect.left;
	IMAGE_SIZE RoiH=RoiRect.bottom-RoiRect.top;
	RoiStep = JetAPI::GetBMPImagePixelsPerLine(RoiW, BitCount, nAlign);
	RoiBitCount = BitCount;
	const bool DynamicFrameRectMode = GetRgnDynamicFrameRectMode();
	if ( false == DynamicFrameRectMode )
	{
		if ( ImageAPI.ExtractRoiImage(ImageW, ImageH, ImageStep, BitCount, ImagePtr, RoiRect, RoiStep, RoiPtr, false) == false )
		{
			ErrStr.Format(_T("Error, %s Extract Roi Image Fault (%d, %d, %d, %d)"), ErrName, RoiRect.left, RoiRect.top, RoiRect.right, RoiRect.bottom);
			SaveRgnMovingTimeMsg(ErrStr);
			return false;	
		}	
	}
	else
	{
		if ( ImageAPI.ExtractDynamicRoiImage(ImageW, ImageH, ImageStep, BitCount, ImagePtr, RoiRect, RoiStep, RoiPtr) == false )
		{
			ErrStr.Format(_T("Error, %s Extract Dynamic Roi Image Fault (%d, %d, %d, %d)"), ErrName, RoiRect.left, RoiRect.top, RoiRect.right, RoiRect.bottom);
			SaveRgnMovingTimeMsg(ErrStr);
			return false; 
		}
	}
	if ( FRAME_BAYER == FrameType )
	{	
		IMAGE_SIZE   DeBayerBit=0;
		IMAGE_SIZE   DeBayerStep=0;
		IMAGE_PTR    DeBayerPtr=NULL;
		BAYER_PATTERN_MODE BayerPattern = FramePtr->GetFrameBayerPattern();	
		BAYER_PATTERN_MODE BayerRoi = ImageAPI.ShiftBayerPattern(BayerPattern, RoiRect.left, RoiRect.top);
		if ( AOIDataCollect.ExecDebayerImage(fnName, RoiW, RoiH, RoiStep, RoiPtr, BayerRoi, DeBayerStep, DeBayerBit, DeBayerPtr) == false)		
		{
			JetMemory.free_func(RoiPtr);
			ErrStr.Format(_T("Error, %s ExecDebayerImage Fault"), ErrName);
			SaveRgnMovingTimeMsg(ErrStr);
			return false;	
		}
		JetMemory.free_func(RoiPtr);
		RoiPtr = DeBayerPtr;		
		RoiStep = DeBayerStep;
		RoiBitCount = DeBayerBit;
		DeBayerPtr = NULL;
	}	

	bool bUseMask_Base = true;
	const bool bEnableMask_Base = GetRgnMaskEnable_Base();
	const unsigned int MaskFrameIndex_Base = GetRgnMaskFrameIndex_Base();
	if ( false==bEnableMask_Base || MaskFrameIndex_Base!=FrameIdx || 0 != SubRgnCount )
	{	bUseMask_Base = false; }
	else
	{	bUseMask_Base = true; }
	if ( true == bUseMask_Base  )
	{
		ClearRgnMaskBuffer_Base();
		CColorGroup *ColorGroupPtr=NULL;
		CAOIProject *ProjectPtr = GetRgnProjectPtr();
		if ( NULL != ProjectPtr )
		{			
			RECT       CalcRect={0};			
			IMAGE_SIZE MaskStep_Base=0;
			IMAGE_SIZE MaskBitCount_Base=8;
			MASK_PTR   MaskPtr_Base=NULL;
			const bool bOpenMP = false;
			const int  nMaskColorGroupLinkIndex = GetRgnMaskColorGroupLinkIndex();
			ColorGroupPtr = ProjectPtr->GetProjectColorGroupPtr(nMaskColorGroupLinkIndex, true);
			JetAPI::SizeToRect(RoiW, RoiH, CalcRect);
			MaskStep_Base = JetAPI::GetBMPImagePixelsPerLine(RoiW, MaskBitCount_Base, 4);
			if ( 24 == BitCount )
			{
				if ( ImageAPI.ColorImageColorFilter(RoiW, RoiH, RoiStep, RoiPtr, *ColorGroupPtr, CalcRect, MaskStep_Base, MaskPtr_Base, false, bOpenMP) == true )
				{	SetRgnMaskBuffer_Base(RoiW, RoiH, MaskStep_Base, MaskBitCount_Base, MaskPtr_Base, false);	}
			}	
		}		
	}

	//非子區域, 或跨FOV
	//if ( 0 == SubRgnCount )
	if ( NULL!=ParentPtr || 0!=SubRgnCount )
	{	bSave = bSave;	}
	else
	{	SetRgnImageBuffer(FrameIdx, RoiW, RoiH, RoiStep, RoiBitCount, RoiPtr, NULL, NULL, NULL, NULL);		}
#ifdef _DEBUG	
	if ( true == bSave )
	{
	//	str.Format(_T("%s\\SubFov#%d.JPG"), AOIDataCollect.GetAOITempDirectory(), ComponentIndex+1);
	//	ImageAPI.SaveImage(str, ImageW, ImageH, ImageStep, BitCount, ImagePtr, true);
	}
	
	if ( true == bSave )
	{
		if ( NULL == ParentPtr )
		{	str.Format(_T("%s\\%s[%d#%d].JPG"), AOIDataCollect.GetAOITempDirectory(), ComName, RgnIndex+1, FrameIdx+1); }
		else
		{	str.Format(_T("%s\\%s_Sub[%d#%d].JPG"), AOIDataCollect.GetAOITempDirectory(), ComName, RgnIndex+1, FrameIdx+1); }
		ImageAPI.SaveImage(str, RoiW, RoiH, RoiStep, RoiBitCount, RoiPtr, true);
	}
#else	
	if ( true == bSave )
	{
		if ( NULL == ParentPtr )
		{	str.Format(_T("%s\\%s[%d#%d].JPG"), AOIDataCollect.GetAOITempDirectory(), ComName, RgnIndex+1, FrameIdx+1); }
		else
		{	str.Format(_T("%s\\%s_Sub[%d#%d].JPG"), AOIDataCollect.GetAOITempDirectory(), ComName, RgnIndex+1, FrameIdx+1); }
		ImageAPI.SaveImage(str, RoiW, RoiH, RoiStep, RoiBitCount, RoiPtr, true);
	}
#endif//_DEBUG

	if ( NULL!=ParentPtr || 0!=SubRgnCount )
	{	JetMemory.free_func(RoiPtr);	}
	else
	{	Finished = true;	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIRgn::ExtractRgnSpace(size_t index, CAOIFrame *FramePtr, bool &Finished, const IMAGE_SIZE ImageW, const IMAGE_SIZE ImageH, const IMAGE_SIZE ImageStep, const int BitCount, const IMAGE_PTR ImagePtr)//挖取區域圖像
{	
	//return ExtractRgnSpace_I(index, FramePtr, Finished);
	return ExtractRgnSpace_II(index, FramePtr, Finished, ImageW, ImageH, ImageStep, BitCount, ImagePtr);
}
//-------------------------------------------------------------------------------------//
bool CAOIRgn::ExtractRgnSpace_I(size_t index, CAOIFrame *FramePtr, bool &Finished)//挖取區域圖像
{
	Finished = false;
	CAOIField* FieldPtr = GetRgnFieldPtr();
	if ( NULL == FieldPtr ) { return false; }	
	if ( NULL == FramePtr ) { return false; }
	const size_t FrameIdx = index;	
	FRAME_TYPE   FrameType = FramePtr->GetFrameType();
	if ( FRAME_SPACE != FrameType ) { return false; }	

	CString    str;
	CString    ErrStr, ErrName;
	CString    ComName = _T("Rgn");
	bool       bSave = false;
	size_t     k=0;		
	size_t     FillFrameCount = 0;
	IMAGE_SIZE FrameW=0;
	IMAGE_SIZE FrameH=0;
	IMAGE_SIZE FrameStep=0;
	IMAGE_SIZE FrameBits=0;		
	TREGION4D  ImageRgn;
	TREGION4D  ComponentRoiRgn;
	MASK_PTR   FrameMaskPtr = NULL;
	SPACE_PTR  FrameSpacePtr = NULL;
	CAOIRgn       *RgnPtr=NULL;	
	CAOIRgn       *SubRgnPtr = NULL;
	CAOIWindow    *WindowPtr = NULL;
	CAOIBarcode   *BarcodePtr = NULL;	
	CAOIComponent *ComponentPtr = NULL;
	CAOIRgn       *ParentPtr=GetRgnParent();
	AOI_OBJ_TYPE ParentType = AOI_OBJ_RGN;	
	const CAMERA_ID CameraID = GetRgnCameraID();
	const size_t RgnIndex = GetRgnIndex();
	const size_t SubRgnCount = GetRgnSubRgnCount();
	const double SpaceRatio = AOIDataCollect.GetSpaceToGrayRatio();	
	FramePtr->GetFrameSpacePtr(FrameW, FrameH, FrameStep, FrameBits, FrameMaskPtr, FrameSpacePtr);
	if ( NULL!=ParentPtr || 0!=SubRgnCount )//子區域, 或跨FOV, 在拼圖時作業, 這裡跳過
	{	return true; }	
	if ( NULL == ParentPtr )
	{	ComName = this->GetRgnDerivedName();	}
	else
	{	ComName = ParentPtr->GetRgnDerivedName();	}
	
	if ( NULL == ParentPtr )
	{	ErrName = ComName;	}
	else
	{	ErrName.Format(_T("%s Sub-Rgn"), ComName);	}	

	const int nAlign = 4;
	const bool bModifyRect=false;
	if ( true == bModifyRect ) 
	{	JetAPI::AdjustRectByAlignW(m_RgnFrameImageRect, nAlign);	}
		
	IMAGE_SIZE RoiStep=0;
	IMAGE_SIZE RoiBitCount=0;	
	MASK_PTR   RoiMaskPtr = NULL;
	MASK_PTR   RoiMaskPtr2 = NULL;
	SPACE_PTR  RoiSpacePtr = NULL;
	SPACE_PTR  RoiSpacePtr2 = NULL;
	IMAGE_PTR  RoiImagePtr=NULL;
	IMAGE_PTR  RoiImagePtr2=NULL;
	RECT       RoiRect = m_RgnFrameImageRect;
	IMAGE_SIZE RoiW=RoiRect.right-RoiRect.left;
	IMAGE_SIZE RoiH=RoiRect.bottom-RoiRect.top;
	RoiStep = JetAPI::GetBMPImagePixelsPerLine(RoiW, FrameBits, 4);
	RoiBitCount=FrameBits;
	const bool DynamicFrameRectMode = GetRgnDynamicFrameRectMode();
	if ( false == DynamicFrameRectMode )
	{
		if ( ImageAPI.ExtractRoiImage(FrameW, FrameH, FrameStep, FrameBits, FrameMaskPtr, RoiRect, RoiStep, RoiMaskPtr, false) == false ||
			 ImageAPI.ExtractSpaceRoiImage(FrameW, FrameH, FrameStep, FrameBits, FrameSpacePtr, RoiRect, RoiStep, RoiSpacePtr, false) == false	)
		{	
			JetMemory.free_func(RoiMaskPtr);
			JetMemory.free_func(RoiSpacePtr);
			ErrStr.Format(_T("Error, %s Extract Roi Image Fault (%d, %d, %d, %d)"), ErrName, RoiRect.left, RoiRect.top, RoiRect.right, RoiRect.bottom);
			SaveRgnMovingTimeMsg(ErrStr);
			return false;	
		}
	}
	else
	{
		MASK_DATA DefaultMask=PHASE_MASK_NOISE_ONLY;
		if ( ImageAPI.ExtractDynamicMaskRoiImage(FrameW, FrameH, FrameStep, FrameMaskPtr, RoiRect, RoiStep, RoiMaskPtr, DefaultMask) == false ||
			 ImageAPI.ExtractDynamicSpaceRoiImage(FrameW, FrameH, FrameStep, FrameBits, FrameSpacePtr, RoiRect, RoiStep, RoiSpacePtr) == false )
		{
			JetMemory.free_func(RoiMaskPtr);
			JetMemory.free_func(RoiSpacePtr);
			ErrStr.Format(_T("Error, %s Extract Dynamic Roi Image Fault (%d, %d, %d, %d)"), ErrName, RoiRect.left, RoiRect.top, RoiRect.right, RoiRect.bottom);
			SaveRgnMovingTimeMsg(ErrStr);
			return false; 
		}		
	}

	IMAGE_SIZE Mask2DW=0;
	IMAGE_SIZE Mask2DH=0;
	IMAGE_SIZE Mask2DStep=0;
	IMAGE_SIZE Mask2DBitCount=0;
	MASK_PTR   Mask2DPtr=NULL;
	const bool bUsing3D = GetRgnUsing3D();//是否使用3D資料群
	const bool bEnableMask_Base = GetRgnMaskEnable_Base();
	if ( true==bEnableMask_Base && true==bUsing3D )
	{
		GetRgnMaskBuffer_Base(Mask2DW, Mask2DH, Mask2DStep, Mask2DBitCount, Mask2DPtr);
		if ( RoiW!=Mask2DW || RoiH!=Mask2DH || RoiStep!=Mask2DStep )
		{	Mask2DPtr = NULL;	}
	}
	
	double     ResX=0, ResY=0;
	IMAGE_SIZE CameraW=0, CameraH=0;	
	AOIDataCollect.GetCameraImageInfo(CameraID, CameraW, CameraH, ResX, ResY);	

	TNoiseFilterParam FilterParam = GetRgnSpaceNoiseFilterParam();		
	if ( false == bUsing3D )
	{	AOIDataCollect.DisableSpaceNoiseFilterParam(FilterParam);	}
	FilterParam.BasePlaneParam.RotatedAngle = GetRgnAngle();
	FilterParam.BasePlaneParam.RotatedSizeW = GetRgnRoiSizeW()/ResX;
	FilterParam.BasePlaneParam.RotatedSizeH = GetRgnRoiSizeH()/ResY;

	const int nOpenMPCnt = GetRgnOpenMPCount();	
	CalcRgnBodyOutsideParam(RoiW, RoiH, FilterParam.BasePlaneParam);
	if ( ImageAPI.BuildSpaceData(RoiW, RoiH, RoiStep, RoiSpacePtr, RoiMaskPtr, Mask2DPtr, nOpenMPCnt, FilterParam, RoiSpacePtr2, RoiMaskPtr2) == false )
	{	
		JetMemory.free_func(RoiMaskPtr);
		JetMemory.free_func(RoiSpacePtr);
		ErrStr.Format(_T("Error, %s BuildSpaceData Fault"), ErrName);
		SaveRgnMovingTimeMsg(ErrStr);
		return false;
	}
	if ( NULL!=ParentPtr || 0!=SubRgnCount )//非子區域, 或跨FOV	
	{	bSave = bSave; }
	else
	{
		TBasePlaneParam &BasePlaneParamRef = GetRgnSpaceBasePlaneParam();	
		BasePlaneParamRef.CopyBasePlaneGroundEquationFrom(FilterParam.BasePlaneParam);
	}
	if ( true == bSave )
	{
		RECT CellRect={0};
		CellRect.left = 0;
		CellRect.top = 0;
		CellRect.right = (int)(RoiW);
		CellRect.bottom = (int)(RoiH);
		if ( ImageAPI.SpaceGrayImageConvertToGray(RoiW, RoiH, RoiStep, RoiSpacePtr, RoiMaskPtr, CellRect, RoiStep, RoiImagePtr, SpaceRatio, false) == false )
		{
			JetMemory.free_func(RoiMaskPtr);
			JetMemory.free_func(RoiSpacePtr);
			JetMemory.free_func(RoiImagePtr);		

			JetMemory.free_func(RoiMaskPtr2);
			JetMemory.free_func(RoiSpacePtr2);
			JetMemory.free_func(RoiImagePtr2);	
			ErrStr.Format(_T("Error, %s SpaceGrayImageConvertToGray Fault"), ErrName);
			SaveRgnMovingTimeMsg(ErrStr);
			return false;	
		}

		if ( ImageAPI.SpaceGrayImageConvertToGray(RoiW, RoiH, RoiStep, RoiSpacePtr2, RoiMaskPtr2, CellRect, RoiStep, RoiImagePtr2, SpaceRatio, false) == false )
		{
			JetMemory.free_func(RoiMaskPtr);
			JetMemory.free_func(RoiSpacePtr);
			JetMemory.free_func(RoiImagePtr);			

			JetMemory.free_func(RoiMaskPtr2);
			JetMemory.free_func(RoiSpacePtr2);
			JetMemory.free_func(RoiImagePtr2);	
			ErrStr.Format(_T("Error, %s SpaceGrayImageConvertToGray2 Fault"), ErrName);
			SaveRgnMovingTimeMsg(ErrStr);
			return false;	
		}
	}
	
	const bool bEnableRawSpace=GetRgnRawSpaceEnabled();
	if ( NULL!=ParentPtr || 0!=SubRgnCount )//非子區域, 或跨FOV	
	{	bSave = bSave; }
	else
	{
		if ( false == bEnableRawSpace )
		{	SetRgnImageBuffer(FrameIdx, RoiW, RoiH, RoiStep, RoiBitCount, NULL, RoiMaskPtr2, RoiSpacePtr2, NULL, NULL);	 }
		else
		{	SetRgnImageBuffer(FrameIdx, RoiW, RoiH, RoiStep, RoiBitCount, NULL, RoiMaskPtr2, RoiSpacePtr2, RoiMaskPtr, RoiSpacePtr);	 }
	}

#ifdef _DEBUG	
	//bSave = true;
	if ( true == bSave )
	{
		if ( NULL == ParentPtr )
		{	str.Format(_T("%s\\%s[%d#%d]_Space.PNG"), AOIDataCollect.GetAOITempDirectory(), ComName, RgnIndex+1, FrameIdx+1); }
		else
		{	str.Format(_T("%s\\%s_Sub[%d#%d]_Space.PNG"), AOIDataCollect.GetAOITempDirectory(), ComName, RgnIndex+1, FrameIdx+1); }
		ImageAPI.SavePNGImage(str, RoiW, RoiH, RoiStep, RoiBitCount, RoiImagePtr, true);

		if ( NULL == ParentPtr )
		{	str.Format(_T("%s\\%s[%d#%d]_Space2.PNG"), AOIDataCollect.GetAOITempDirectory(), ComName, RgnIndex+1, FrameIdx+1); }
		else
		{	str.Format(_T("%s\\%s_Sub[%d#%d]_Space2.PNG"), AOIDataCollect.GetAOITempDirectory(), ComName, RgnIndex+1, FrameIdx+1); }
		ImageAPI.SavePNGImage(str, RoiW, RoiH, RoiStep, RoiBitCount, RoiImagePtr2, true);

		if ( NULL == ParentPtr )
		{	str.Format(_T("%s\\%s[%d#%d]_Mask.PNG"), AOIDataCollect.GetAOITempDirectory(), ComName, RgnIndex+1, FrameIdx+1); }
		else
		{	str.Format(_T("%s\\%s_Sub[%d#%d]_Mask.PNG"), AOIDataCollect.GetAOITempDirectory(), ComName, RgnIndex+1, FrameIdx+1); }
		ImageAPI.SavePNGImage(str, RoiW, RoiH, RoiStep, RoiBitCount, RoiMaskPtr, true);

		if ( NULL == ParentPtr )
		{	str.Format(_T("%s\\%s[%d#%d]_Mask2.PNG"), AOIDataCollect.GetAOITempDirectory(), ComName, RgnIndex+1, FrameIdx+1); }
		else
		{	str.Format(_T("%s\\%s_Sub[%d#%d]_Mask2.PNG"), AOIDataCollect.GetAOITempDirectory(), ComName, RgnIndex+1, FrameIdx+1); }
		ImageAPI.SavePNGImage(str, RoiW, RoiH, RoiStep, RoiBitCount, RoiMaskPtr2, true);

		if ( NULL != Mask2DPtr )
		{
			if ( NULL == ParentPtr )
			{	str.Format(_T("%s\\%s[%d#%d]_BaseMask.PNG"), AOIDataCollect.GetAOITempDirectory(), ComName, RgnIndex+1, FrameIdx+1); }
			else
			{	str.Format(_T("%s\\%s_Sub[%d#%d]_BaseMask.PNG"), AOIDataCollect.GetAOITempDirectory(), ComName, RgnIndex+1, FrameIdx+1); }
			ImageAPI.SavePNGImage(str, Mask2DW, Mask2DH, Mask2DStep, Mask2DBitCount, Mask2DPtr, true);
		}
	}
#else	
	if ( true == bSave )
	{
		if ( NULL == ParentPtr )
		{	str.Format(_T("%s\\%s[%d#%d]_Space.PNG"), AOIDataCollect.GetAOITempDirectory(), ComName, RgnIndex+1, FrameIdx+1); }
		else
		{	str.Format(_T("%s\\%s_Sub[%d#%d]_Space.PNG"), AOIDataCollect.GetAOITempDirectory(), ComName, RgnIndex+1, FrameIdx+1); }
		ImageAPI.SavePNGImage(str, RoiW, RoiH, RoiStep, RoiBitCount, RoiImagePtr, true);

		if ( NULL == ParentPtr )
		{	str.Format(_T("%s\\%s[%d#%d]_Space2.PNG"), AOIDataCollect.GetAOITempDirectory(), ComName, RgnIndex+1, FrameIdx+1); }
		else
		{	str.Format(_T("%s\\%s_Sub[%d#%d]_Space2.PNG"), AOIDataCollect.GetAOITempDirectory(), ComName, RgnIndex+1, FrameIdx+1); }
		ImageAPI.SavePNGImage(str, RoiW, RoiH, RoiStep, RoiBitCount, RoiImagePtr2, true);

		if ( NULL == ParentPtr )
		{	str.Format(_T("%s\\%s[%d#%d]_Mask.PNG"), AOIDataCollect.GetAOITempDirectory(), ComName, RgnIndex+1, FrameIdx+1); }
		else
		{	str.Format(_T("%s\\%s_Sub[%d#%d]_Mask.PNG"), AOIDataCollect.GetAOITempDirectory(), ComName, RgnIndex+1, FrameIdx+1); }
		ImageAPI.SavePNGImage(str, RoiW, RoiH, RoiStep, RoiBitCount, RoiMaskPtr, true);

		if ( NULL == ParentPtr )
		{	str.Format(_T("%s\\%s[%d#%d]_Mask2.PNG"), AOIDataCollect.GetAOITempDirectory(), ComName, RgnIndex+1, FrameIdx+1); }
		else
		{	str.Format(_T("%s\\%s_Sub[%d#%d]_Mask2.PNG"), AOIDataCollect.GetAOITempDirectory(), ComName, RgnIndex+1, FrameIdx+1); }
		ImageAPI.SavePNGImage(str, RoiW, RoiH, RoiStep, RoiBitCount, RoiMaskPtr2, true);

		if ( NULL != Mask2DPtr )
		{
			if ( NULL == ParentPtr )
			{	str.Format(_T("%s\\%s[%d#%d]_BaseMask.PNG"), AOIDataCollect.GetAOITempDirectory(), ComName, RgnIndex+1, FrameIdx+1); }
			else
			{	str.Format(_T("%s\\%s_Sub[%d#%d]_BaseMask.PNG"), AOIDataCollect.GetAOITempDirectory(), ComName, RgnIndex+1, FrameIdx+1); }
			ImageAPI.SavePNGImage(str, Mask2DW, Mask2DH, Mask2DStep, Mask2DBitCount, Mask2DPtr, true);
		}
	}
#endif//_DEBUG	
	if ( false == bEnableRawSpace )
	{
		JetMemory.free_func(RoiMaskPtr);
		JetMemory.free_func(RoiSpacePtr);
	}
	JetMemory.free_func(RoiImagePtr);
	JetMemory.free_func(RoiImagePtr2);
	
	//if ( 0 == SubRgnCount )
	//if ( NULL==ParentPtr && 0==SubRgnCount )
	if ( NULL!=ParentPtr || 0!=SubRgnCount )
	{
		JetMemory.free_func(RoiMaskPtr2);
		JetMemory.free_func(RoiSpacePtr2);		
	}
	else
	{	Finished = true;	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIRgn::ExtractRgnSpace_II(size_t index, CAOIFrame *FramePtr, bool &Finished, const IMAGE_SIZE ImageW, const IMAGE_SIZE ImageH, const IMAGE_SIZE ImageStep, const int BitCount, const IMAGE_PTR ImagePtr)//挖取區域圖像
{	
	Finished = false;
	CAOIField* FieldPtr = GetRgnFieldPtr();
	if ( NULL == FieldPtr ) { return false; }	
	if ( NULL == FramePtr ) { return false; }
	const size_t FrameIdx = index;	
	FRAME_TYPE   FrameType = FramePtr->GetFrameType();
	if ( FRAME_SPACE != FrameType ) { return false; }	

	CString    str;
	CString    ErrStr, ErrName;
	CString    ComName = _T("Rgn");
	bool       bSave = false;
	size_t     k=0;		
	size_t     FillFrameCount = 0;
	IMAGE_SIZE FrameW=0;
	IMAGE_SIZE FrameH=0;
	IMAGE_SIZE FrameStep=0;
	IMAGE_SIZE FrameBits=0;	
	TREGION4D  ImageRgn;
	TREGION4D  ComponentRoiRgn;
	MASK_PTR   FrameMaskPtr = NULL;
	SPACE_PTR  FrameSpacePtr = NULL;	
	CAOIRgn       *RgnPtr=NULL;	
	CAOIRgn       *SubRgnPtr = NULL;
	CAOIWindow    *WindowPtr = NULL;
	CAOIBarcode   *BarcodePtr = NULL;	
	CAOIComponent *ComponentPtr = NULL;
	CAOIRgn       *ParentPtr=GetRgnParent();
	AOI_OBJ_TYPE ParentType = AOI_OBJ_RGN;	
	const CAMERA_ID CameraID = GetRgnCameraID();
	const size_t RgnIndex = GetRgnIndex();
	const size_t SubRgnCount = GetRgnSubRgnCount();
	const double SpaceRatio = AOIDataCollect.GetSpaceToGrayRatio();
	FramePtr->GetFrameSpacePtr(FrameW, FrameH, FrameStep, FrameBits, FrameMaskPtr, FrameSpacePtr); 
	if ( NULL!=ParentPtr || 0!=SubRgnCount )//子區域, 或跨FOV, 在拼圖時作業, 這裡跳過
	{	return true; }	
	if ( NULL == ParentPtr )
	{	ComName = this->GetRgnDerivedName();	}
	else
	{	ComName = ParentPtr->GetRgnDerivedName();	}
	if ( NULL == ParentPtr )
	{	ErrName = ComName;	}
	else
	{	ErrName.Format(_T("%s Sub-Rgn"), ComName);	}	

	const int nAlign = 4;
	const bool bModifyRect=false;
	if ( true == bModifyRect ) 
	{	JetAPI::AdjustRectByAlignW(m_RgnFrameImageRect, nAlign);	}
		
	IMAGE_SIZE RoiStep=0;
	IMAGE_SIZE RoiStep_GuidedImage = 0;
	IMAGE_SIZE RoiBitCount=0;	
	MASK_PTR   RoiMaskPtr = NULL;
	MASK_PTR   RoiMaskPtr2 = NULL;
	SPACE_PTR  RoiSpacePtr = NULL;
	SPACE_PTR  RoiSpacePtr2 = NULL;
	IMAGE_PTR  RoiImagePtr=NULL;
	IMAGE_PTR  RoiImagePtr2=NULL;
	RECT       RoiRect = m_RgnFrameImageRect;
	IMAGE_SIZE RoiW=RoiRect.right-RoiRect.left;
	IMAGE_SIZE RoiH=RoiRect.bottom-RoiRect.top;
	RoiStep = JetAPI::GetBMPImagePixelsPerLine(RoiW, FrameBits, 4);
	RoiStep_GuidedImage = JetAPI::GetBMPImagePixelsPerLine(RoiW, (unsigned int)BitCount, 4);
	RoiBitCount=FrameBits;
		
	const bool DynamicFrameRectMode = GetRgnDynamicFrameRectMode();
	if ( false == DynamicFrameRectMode )
	{
		if ( ImageAPI.ExtractRoiImage(FrameW, FrameH, FrameStep, FrameBits, FrameMaskPtr, RoiRect, RoiStep, RoiMaskPtr, false) == false ||
			 ImageAPI.ExtractSpaceRoiImage(FrameW, FrameH, FrameStep, FrameBits, FrameSpacePtr, RoiRect, RoiStep, RoiSpacePtr, false) == false	)
		{	
			JetMemory.free_func(RoiImagePtr);
			JetMemory.free_func(RoiMaskPtr);
			JetMemory.free_func(RoiSpacePtr);
			ErrStr.Format(_T("Error, %s Extract Roi Space Fault (%d, %d, %d, %d)"), ErrName, RoiRect.left, RoiRect.top, RoiRect.right, RoiRect.bottom);
			SaveRgnMovingTimeMsg(ErrStr);
			return false;	
		}
		if ( NULL != ImagePtr )
		{
			if ( ImageAPI.ExtractRoiImage(ImageW, ImageH, ImageStep, BitCount, ImagePtr, RoiRect, RoiStep_GuidedImage, RoiImagePtr, false) == false )
			{	
				JetMemory.free_func(RoiImagePtr);
				JetMemory.free_func(RoiMaskPtr);
				JetMemory.free_func(RoiSpacePtr);
				ErrStr.Format(_T("Error, %s Extract Roi Image Fault (%d, %d, %d, %d)"), ErrName, RoiRect.left, RoiRect.top, RoiRect.right, RoiRect.bottom);
				SaveRgnMovingTimeMsg(ErrStr);
				return false;	
			}
		}
	}
	else
	{
		MASK_DATA DefaultMask=PHASE_MASK_NOISE_ONLY;
		if ( ImageAPI.ExtractDynamicMaskRoiImage(FrameW, FrameH, FrameStep, FrameMaskPtr, RoiRect, RoiStep, RoiMaskPtr, DefaultMask) == false ||
			 ImageAPI.ExtractDynamicSpaceRoiImage(FrameW, FrameH, FrameStep, FrameBits, FrameSpacePtr, RoiRect, RoiStep, RoiSpacePtr) == false )
		{
			JetMemory.free_func(RoiImagePtr);
			JetMemory.free_func(RoiMaskPtr);
			JetMemory.free_func(RoiSpacePtr);
			ErrStr.Format(_T("Error, %s Extract Dynamic Roi Space Fault (%d, %d, %d, %d)"), ErrName, RoiRect.left, RoiRect.top, RoiRect.right, RoiRect.bottom);
			SaveRgnMovingTimeMsg(ErrStr);
			return false; 
		}
		if ( NULL != ImagePtr )
		{
			if ( ImageAPI.ExtractDynamicRoiImage(ImageW, ImageH, ImageStep, BitCount, ImagePtr, RoiRect, RoiStep_GuidedImage, RoiImagePtr) == false )
			{	
				JetMemory.free_func(RoiImagePtr);
				JetMemory.free_func(RoiMaskPtr);
				JetMemory.free_func(RoiSpacePtr);
				ErrStr.Format(_T("Error, %s Extract Dynamic Roi Image Fault (%d, %d, %d, %d)"), ErrName, RoiRect.left, RoiRect.top, RoiRect.right, RoiRect.bottom);
				SaveRgnMovingTimeMsg(ErrStr);
				return false;	
			}
		}
	}

	IMAGE_SIZE Mask2DW=0;
	IMAGE_SIZE Mask2DH=0;
	IMAGE_SIZE Mask2DStep=0;
	IMAGE_SIZE Mask2DBitCount=0;
	MASK_PTR   Mask2DPtr=NULL;
	const bool bUsing3D = GetRgnUsing3D();//是否使用3D資料群
	const bool bEnableMask_Base = GetRgnMaskEnable_Base();
	if ( true==bEnableMask_Base && true==bUsing3D )
	{
		GetRgnMaskBuffer_Base(Mask2DW, Mask2DH, Mask2DStep, Mask2DBitCount, Mask2DPtr);
		if ( RoiW!=Mask2DW || RoiH!=Mask2DH || RoiStep!=Mask2DStep )
		{	Mask2DPtr = NULL;	}
	}
	
	double     ResX=0, ResY=0;
	IMAGE_SIZE CameraW=0, CameraH=0;	
	AOIDataCollect.GetCameraImageInfo(CameraID, CameraW, CameraH, ResX, ResY);	

	TNoiseFilterParam FilterParam = GetRgnSpaceNoiseFilterParam();		
	if ( false == bUsing3D )
	{	AOIDataCollect.DisableSpaceNoiseFilterParam(FilterParam);	}

	FilterParam.BasePlaneParam.RotatedAngle = GetRgnAngle();
	FilterParam.BasePlaneParam.RotatedSizeW = GetRgnRoiSizeW()/ResX;
	FilterParam.BasePlaneParam.RotatedSizeH = GetRgnRoiSizeH()/ResY;

	const int nOpenMPCnt = GetRgnOpenMPCount();	
	CalcRgnBodyOutsideParam(RoiW, RoiH, FilterParam.BasePlaneParam);
	IMAGE_PTR  GuidedImagePtr = NULL;
	ImageAPI.GetGuidedImage(RoiW, RoiH, RoiStep_GuidedImage, BitCount, RoiImagePtr, GuidedImagePtr);
	if ( ImageAPI.BuildSpaceData(RoiW, RoiH, RoiStep, RoiSpacePtr, RoiMaskPtr, Mask2DPtr, nOpenMPCnt, FilterParam, RoiSpacePtr2, RoiMaskPtr2, GuidedImagePtr) == false )
	{			
		JetMemory.free_func(RoiMaskPtr);
		JetMemory.free_func(RoiSpacePtr);
		JetMemory.free_func(GuidedImagePtr);
		ErrStr.Format(_T("Error, %s BuildSpaceData Fault"), ErrName);
		SaveRgnMovingTimeMsg(ErrStr);
		return false;
	}
	JetMemory.free_func(GuidedImagePtr);
	if ( NULL!=ParentPtr || 0!=SubRgnCount )//非子區域, 或跨FOV	
	{	bSave = bSave; }
	else
	{
		TBasePlaneParam &BasePlaneParamRef = GetRgnSpaceBasePlaneParam();	
		BasePlaneParamRef.CopyBasePlaneGroundEquationFrom(FilterParam.BasePlaneParam);
	}
	if ( true == bSave )
	{
		RECT CellRect={0};
		CellRect.left = 0;
		CellRect.top = 0;
		CellRect.right = (int)(RoiW);
		CellRect.bottom = (int)(RoiH);
		if ( ImageAPI.SpaceGrayImageConvertToGray(RoiW, RoiH, RoiStep, RoiSpacePtr, RoiMaskPtr, CellRect, RoiStep, RoiImagePtr, SpaceRatio, false) == false )
		{
			JetMemory.free_func(RoiMaskPtr);
			JetMemory.free_func(RoiSpacePtr);
			JetMemory.free_func(RoiImagePtr);		

			JetMemory.free_func(RoiMaskPtr2);
			JetMemory.free_func(RoiSpacePtr2);
			JetMemory.free_func(RoiImagePtr2);	
			ErrStr.Format(_T("Error, %s SpaceGrayImageConvertToGray Fault"), ErrName);
			SaveRgnMovingTimeMsg(ErrStr);
			return false;	
		}

		if ( ImageAPI.SpaceGrayImageConvertToGray(RoiW, RoiH, RoiStep, RoiSpacePtr2, RoiMaskPtr2, CellRect, RoiStep, RoiImagePtr2, SpaceRatio, false) == false )
		{
			JetMemory.free_func(RoiMaskPtr);
			JetMemory.free_func(RoiSpacePtr);
			JetMemory.free_func(RoiImagePtr);			

			JetMemory.free_func(RoiMaskPtr2);
			JetMemory.free_func(RoiSpacePtr2);
			JetMemory.free_func(RoiImagePtr2);
			ErrStr.Format(_T("Error, %s SpaceGrayImageConvertToGray2 Fault"), ErrName);
			SaveRgnMovingTimeMsg(ErrStr);
			return false;	
		}
	}
	
	const bool bEnableRawSpace=GetRgnRawSpaceEnabled();
	if ( NULL!=ParentPtr || 0!=SubRgnCount )//非子區域, 或跨FOV	
	{	bSave = bSave; }
	else
	{	
		if ( false == bEnableRawSpace )
		{	SetRgnImageBuffer(FrameIdx, RoiW, RoiH, RoiStep, RoiBitCount, NULL, RoiMaskPtr2, RoiSpacePtr2, NULL, NULL);	 }
		else
		{	SetRgnImageBuffer(FrameIdx, RoiW, RoiH, RoiStep, RoiBitCount, NULL, RoiMaskPtr2, RoiSpacePtr2, RoiMaskPtr, RoiSpacePtr);	 }
	}

#ifdef _DEBUG	
	//bSave = true;
	if ( true == bSave )
	{
		if ( NULL == ParentPtr )
		{	str.Format(_T("%s\\%s[%d#%d]_Space.PNG"), AOIDataCollect.GetAOITempDirectory(), ComName, RgnIndex+1, FrameIdx+1); }
		else
		{	str.Format(_T("%s\\%s_Sub[%d#%d]_Space.PNG"), AOIDataCollect.GetAOITempDirectory(), ComName, RgnIndex+1, FrameIdx+1); }
		ImageAPI.SavePNGImage(str, RoiW, RoiH, RoiStep, RoiBitCount, RoiImagePtr, true);

		if ( NULL == ParentPtr )
		{	str.Format(_T("%s\\%s[%d#%d]_Space2.PNG"), AOIDataCollect.GetAOITempDirectory(), ComName, RgnIndex+1, FrameIdx+1); }
		else
		{	str.Format(_T("%s\\%s_Sub[%d#%d]_Space2.PNG"), AOIDataCollect.GetAOITempDirectory(), ComName, RgnIndex+1, FrameIdx+1); }
		ImageAPI.SavePNGImage(str, RoiW, RoiH, RoiStep, RoiBitCount, RoiImagePtr2, true);

		if ( NULL == ParentPtr )
		{	str.Format(_T("%s\\%s[%d#%d]_Mask.PNG"), AOIDataCollect.GetAOITempDirectory(), ComName, RgnIndex+1, FrameIdx+1); }
		else
		{	str.Format(_T("%s\\%s_Sub[%d#%d]_Mask.PNG"), AOIDataCollect.GetAOITempDirectory(), ComName, RgnIndex+1, FrameIdx+1); }
		ImageAPI.SavePNGImage(str, RoiW, RoiH, RoiStep, RoiBitCount, RoiMaskPtr, true);

		if ( NULL == ParentPtr )
		{	str.Format(_T("%s\\%s[%d#%d]_Mask2.PNG"), AOIDataCollect.GetAOITempDirectory(), ComName, RgnIndex+1, FrameIdx+1); }
		else
		{	str.Format(_T("%s\\%s_Sub[%d#%d]_Mask2.PNG"), AOIDataCollect.GetAOITempDirectory(), ComName, RgnIndex+1, FrameIdx+1); }
		ImageAPI.SavePNGImage(str, RoiW, RoiH, RoiStep, RoiBitCount, RoiMaskPtr2, true);

		if ( NULL != Mask2DPtr )
		{
			if ( NULL == ParentPtr )
			{	str.Format(_T("%s\\%s[%d#%d]_BaseMask.PNG"), AOIDataCollect.GetAOITempDirectory(), ComName, RgnIndex+1, FrameIdx+1); }
			else
			{	str.Format(_T("%s\\%s_Sub[%d#%d]_BaseMask.PNG"), AOIDataCollect.GetAOITempDirectory(), ComName, RgnIndex+1, FrameIdx+1); }
			ImageAPI.SavePNGImage(str, Mask2DW, Mask2DH, Mask2DStep, Mask2DBitCount, Mask2DPtr, true);
		}
	}
#else	
	if ( true == bSave )
	{
		if ( NULL == ParentPtr )
		{	str.Format(_T("%s\\%s[%d#%d]_Space.PNG"), AOIDataCollect.GetAOITempDirectory(), ComName, RgnIndex+1, FrameIdx+1); }
		else
		{	str.Format(_T("%s\\%s_Sub[%d#%d]_Space.PNG"), AOIDataCollect.GetAOITempDirectory(), ComName, RgnIndex+1, FrameIdx+1); }
		ImageAPI.SavePNGImage(str, RoiW, RoiH, RoiStep, RoiBitCount, RoiImagePtr, true);

		if ( NULL == ParentPtr )
		{	str.Format(_T("%s\\%s[%d#%d]_Space2.PNG"), AOIDataCollect.GetAOITempDirectory(), ComName, RgnIndex+1, FrameIdx+1); }
		else
		{	str.Format(_T("%s\\%s_Sub[%d#%d]_Space2.PNG"), AOIDataCollect.GetAOITempDirectory(), ComName, RgnIndex+1, FrameIdx+1); }
		ImageAPI.SavePNGImage(str, RoiW, RoiH, RoiStep, RoiBitCount, RoiImagePtr2, true);

		if ( NULL == ParentPtr )
		{	str.Format(_T("%s\\%s[%d#%d]_Mask.PNG"), AOIDataCollect.GetAOITempDirectory(), ComName, RgnIndex+1, FrameIdx+1); }
		else
		{	str.Format(_T("%s\\%s_Sub[%d#%d]_Mask.PNG"), AOIDataCollect.GetAOITempDirectory(), ComName, RgnIndex+1, FrameIdx+1); }
		ImageAPI.SavePNGImage(str, RoiW, RoiH, RoiStep, RoiBitCount, RoiMaskPtr, true);

		if ( NULL == ParentPtr )
		{	str.Format(_T("%s\\%s[%d#%d]_Mask2.PNG"), AOIDataCollect.GetAOITempDirectory(), ComName, RgnIndex+1, FrameIdx+1); }
		else
		{	str.Format(_T("%s\\%s_Sub[%d#%d]_Mask2.PNG"), AOIDataCollect.GetAOITempDirectory(), ComName, RgnIndex+1, FrameIdx+1); }
		ImageAPI.SavePNGImage(str, RoiW, RoiH, RoiStep, RoiBitCount, RoiMaskPtr2, true);

		if ( NULL != Mask2DPtr )
		{
			if ( NULL == ParentPtr )
			{	str.Format(_T("%s\\%s[%d#%d]_BaseMask.PNG"), AOIDataCollect.GetAOITempDirectory(), ComName, RgnIndex+1, FrameIdx+1); }
			else
			{	str.Format(_T("%s\\%s_Sub[%d#%d]_BaseMask.PNG"), AOIDataCollect.GetAOITempDirectory(), ComName, RgnIndex+1, FrameIdx+1); }
			ImageAPI.SavePNGImage(str, Mask2DW, Mask2DH, Mask2DStep, Mask2DBitCount, Mask2DPtr, true);
		}
	}
#endif//_DEBUG	
	if ( false == bEnableRawSpace )
	{
		JetMemory.free_func(RoiMaskPtr);
		JetMemory.free_func(RoiSpacePtr);
	}
	JetMemory.free_func(RoiImagePtr);
	JetMemory.free_func(RoiImagePtr2);	

	//if ( 0 == SubRgnCount )
	//if ( NULL==ParentPtr && 0==SubRgnCount )
	if ( NULL!=ParentPtr || 0!=SubRgnCount )
	{
		JetMemory.free_func(RoiMaskPtr2);
		JetMemory.free_func(RoiSpacePtr2);		
	}
	else
	{	Finished = true;	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIRgn::CreateRgnSelfFieldPtr()//建立專屬Field指標
{	
	CAOIRgn*     RgnPtr = this;
	if ( NULL == RgnPtr ) { return false; }
	if ( false == m_RgnSelfFieldEnabled ) { return false; }	
	if ( NULL != m_RgnSelfFieldPtr ) { return false; }

	CAOIPanel*     PanelPtr = NULL;
	CAOIBoard*     BoardPtr = NULL;
	PanelPtr = RgnPtr->GetRgnPanelPtr();
	BoardPtr = RgnPtr->GetRgnBoardPtr();
	if ( NULL==BoardPtr && NULL==PanelPtr )
	{	return false; }	

	//Create CAOIField Obj and Assign Index	
	size_t         i=0, j=0, k=0;	
	size_t         RgnIdx = 0;
	CAMERA_ID      CameraID;	
	RECT           FrameRect={0};	
	AOI_OBJ_TYPE   AOIType;	
	
	CAOIRgn       *SubRgnPtr = NULL;
	CAOIField     *FieldPtr = NULL;
	CAOIBarcode   *BarcodePtr = NULL;
	CAOIWindow    *WindowPtr = NULL;
	CAOIComponent *ComponentPtr = NULL;	
	CAOIField     *FieldPtr_Before = NULL;
	TSIZE2D        FrameSizeUm;
	TSIZE2D        FovSizeReal;
	TSIZE2D        FovSizeInner;
	TSIZE2D        FovSizeOuter;
	TPOINT2D       CadPos, StagePos2D;
	TPOINT3D       StagePos;	
	TREGION4D      Region, CameraRgn;
	TREGION4D      rgnFOVCad, rgnFOVStage;	
	DISTRICT_ID     DistrictID = GetRgnDistrictID();
	AOIDataCollect.GetFovSizeReal(FovSizeReal.cx, FovSizeReal.cy);
	AOIDataCollect.GetFovSizeInner(FovSizeInner.cx, FovSizeInner.cy);
	AOIDataCollect.GetFovSizeOuter(FovSizeOuter.cx, FovSizeOuter.cy);

	CMapCoordinate *MapCTSPtr=NULL;
	const double FOVW2 = FovSizeInner.cx/2;
	const double FOVH2 = FovSizeInner.cy/2;	
	
	CadPos.x = RgnPtr->GetRgnCadPosX();
	CadPos.y = RgnPtr->GetRgnCadPosY();

	if ( NULL != BoardPtr ) 
	{	MapCTSPtr = BoardPtr->GetBoardMapCTSPtr(DistrictID); }
	else if ( NULL != PanelPtr ) 
	{	MapCTSPtr = PanelPtr->GetPanelMapCTSPtr(DistrictID); }
	if ( NULL == MapCTSPtr ) { return false; }
	MapCTSPtr->Map2D(CadPos.x, CadPos.y, StagePos.x, StagePos.y);		

	rgnFOVStage.minX = StagePos.x-FOVW2;
	rgnFOVStage.minY = StagePos.y-FOVH2;
	rgnFOVStage.maxX = StagePos.x+FOVW2;
	rgnFOVStage.maxY = StagePos.y+FOVH2;
	RgnPtr->GetRgnRoiStageRegion(Region);
	if ( Region.minX < rgnFOVStage.minX ) { return false; }
	if ( Region.minY < rgnFOVStage.minY ) { return false; }
	if ( Region.maxX > rgnFOVStage.maxX ) { return false; }
	if ( Region.maxY > rgnFOVStage.maxY ) { return false; }
	FieldPtr = AOIObjManager.CreateFieldObj();
	if ( FieldPtr == NULL ) { return false; }

	size_t SubRgnCount = RgnPtr->GetRgnSubRgnCount();
	for ( i=0; i<SubRgnCount; i++ )
	{
		SubRgnPtr = RgnPtr->GetRgnSubRgnPtr(i, false);
		if ( NULL == SubRgnPtr ) { continue; }
		FieldPtr_Before = SubRgnPtr->GetRgnFieldPtr();
		if ( NULL != FieldPtr_Before ) 
		{	FieldPtr_Before->RemoveFieldRgnPtr(SubRgnPtr);	}
	}
	FieldPtr_Before = GetRgnFieldPtr();
	if ( NULL != FieldPtr_Before ) 
	{	FieldPtr_Before->RemoveFieldRgnPtr(RgnPtr);	}
	RgnPtr->ClearRgnSubList();

	FieldPtr->SetFieldCadPos(CadPos);
	FieldPtr->SetFieldStagePos(StagePos);
	FieldPtr->SetFieldDistrictID(DistrictID);
	FieldPtr->SetFieldSize_Real(FovSizeReal);
	FieldPtr->SetFieldSize_Inner(FovSizeInner);
	FieldPtr->SetFieldSize_Outer(FovSizeOuter);		

	StagePos2D.x = StagePos.x;
	StagePos2D.y = StagePos.y;	

	//將CAOIField指標設定給予檢測區域內的指標	
	AOIType = RgnPtr->GetObjType();
	switch ( AOIType )
	{
	case AOI_OBJ_FD:
		break;			
	case AOI_OBJ_RGN:
		break;
	case AOI_OBJ_WINDOW:
		break;
	case AOI_OBJ_BARCODE:
		break;
	case AOI_OBJ_COMPONENT:
		break;
	}
	//設定區域影像位置
	const int  nAlign = 4;
	CameraID = PRIMARY_CAMERA_ID;
	AOIDataCollect.MapStageRegionToCamera(CameraID, Region, StagePos2D, CameraRgn);
	JetAPI::Region4DToRect(CameraRgn, FrameRect, true);
	JetAPI::AdjustRectByAlignW(FrameRect, nAlign);//調成4倍寬
	AOIDataCollect.MapImageSizeToReal(CameraID, FrameRect, FrameSizeUm);
	//是否計算四個端點呢!?
	RgnPtr->SetRgnFrameImageRect(FrameRect);
	RgnPtr->SetRgnFrameImageSize_um(FrameSizeUm);

	m_RgnSelfFieldPtr = FieldPtr;
	RgnPtr->SetRgnFieldIndex(-1);
	RgnPtr->SetRgnFieldPtr(FieldPtr);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIRgn::DestroyRgnSelfFieldPtr()//刪除專屬Field指標	
{
	CAOIField *FieldPtr = m_RgnSelfFieldPtr;
	if ( NULL == FieldPtr ) { return true; }
	AOIObjManager.DestroyFieldObj(m_RgnSelfFieldPtr);
	m_RgnSelfFieldPtr = NULL;
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIRgn::SetRgnFieldPtr(CAOIField* Ptr)
{
	m_RgnFieldPtr = Ptr;
	if ( NULL == Ptr ) 
	{ 
		m_RgnFieldIdx = -1;
		m_RgnFieldCadPos = TPOINT2D();
		m_RgnFieldStagePos = TPOINT3D();
		m_RgnFieldCadRegion = TREGION4D();
		m_RgnFieldStageRegion = TREGION4D();		
		return; 
	}

	m_RgnFieldIdx = Ptr->GetFieldIndex();
	m_RgnFieldCadPos = Ptr->GetFieldCadPos();
	m_RgnFieldStagePos = Ptr->GetFieldStagePos();
	Ptr->GetFieldCadRgn_Inner(m_RgnFieldCadRegion);//區域所屬Field的CAD範圍
	Ptr->GetFieldStageRgn_Inner(m_RgnFieldStageRegion);//區域所屬Field的Stage範圍	
}
//-------------------------------------------------------------------------------------//
void CAOIRgn::GetRgnRoiCadRegion(TREGION4D &Region)//取得計算區域在Cad的範圍
{
	Region.minX = Region.maxX = CAOIRgn::m_RgnRoiCadCornerPos[0].x;
	Region.minY = Region.maxY = CAOIRgn::m_RgnRoiCadCornerPos[0].y;

	if ( Region.minX > CAOIRgn::m_RgnRoiCadCornerPos[1].x ) { Region.minX = CAOIRgn::m_RgnRoiCadCornerPos[1].x; }
	if ( Region.minY > CAOIRgn::m_RgnRoiCadCornerPos[1].y ) { Region.minY = CAOIRgn::m_RgnRoiCadCornerPos[1].y; }
	if ( Region.maxX < CAOIRgn::m_RgnRoiCadCornerPos[1].x ) { Region.maxX = CAOIRgn::m_RgnRoiCadCornerPos[1].x; }
	if ( Region.maxY < CAOIRgn::m_RgnRoiCadCornerPos[1].y ) { Region.maxY = CAOIRgn::m_RgnRoiCadCornerPos[1].y; }

	if ( Region.minX > CAOIRgn::m_RgnRoiCadCornerPos[2].x ) { Region.minX = CAOIRgn::m_RgnRoiCadCornerPos[2].x; }
	if ( Region.minY > CAOIRgn::m_RgnRoiCadCornerPos[2].y ) { Region.minY = CAOIRgn::m_RgnRoiCadCornerPos[2].y; }
	if ( Region.maxX < CAOIRgn::m_RgnRoiCadCornerPos[2].x ) { Region.maxX = CAOIRgn::m_RgnRoiCadCornerPos[2].x; }
	if ( Region.maxY < CAOIRgn::m_RgnRoiCadCornerPos[2].y ) { Region.maxY = CAOIRgn::m_RgnRoiCadCornerPos[2].y; }

	if ( Region.minX > CAOIRgn::m_RgnRoiCadCornerPos[3].x ) { Region.minX = CAOIRgn::m_RgnRoiCadCornerPos[3].x; }
	if ( Region.minY > CAOIRgn::m_RgnRoiCadCornerPos[3].y ) { Region.minY = CAOIRgn::m_RgnRoiCadCornerPos[3].y; }
	if ( Region.maxX < CAOIRgn::m_RgnRoiCadCornerPos[3].x ) { Region.maxX = CAOIRgn::m_RgnRoiCadCornerPos[3].x; }
	if ( Region.maxY < CAOIRgn::m_RgnRoiCadCornerPos[3].y ) { Region.maxY = CAOIRgn::m_RgnRoiCadCornerPos[3].y; }
}
//-------------------------------------------------------------------------------------//
void CAOIRgn::GetRgnBodyCadRegion(TREGION4D &Region)//取得計算區域在Cad的範圍
{
	Region.minX = Region.maxX = CAOIRgn::m_RgnBodyCadCornerPos[0].x;
	Region.minY = Region.maxY = CAOIRgn::m_RgnBodyCadCornerPos[0].y;

	if ( Region.minX > CAOIRgn::m_RgnBodyCadCornerPos[1].x ) { Region.minX = CAOIRgn::m_RgnBodyCadCornerPos[1].x; }
	if ( Region.minY > CAOIRgn::m_RgnBodyCadCornerPos[1].y ) { Region.minY = CAOIRgn::m_RgnBodyCadCornerPos[1].y; }
	if ( Region.maxX < CAOIRgn::m_RgnBodyCadCornerPos[1].x ) { Region.maxX = CAOIRgn::m_RgnBodyCadCornerPos[1].x; }
	if ( Region.maxY < CAOIRgn::m_RgnBodyCadCornerPos[1].y ) { Region.maxY = CAOIRgn::m_RgnBodyCadCornerPos[1].y; }

	if ( Region.minX > CAOIRgn::m_RgnBodyCadCornerPos[2].x ) { Region.minX = CAOIRgn::m_RgnBodyCadCornerPos[2].x; }
	if ( Region.minY > CAOIRgn::m_RgnBodyCadCornerPos[2].y ) { Region.minY = CAOIRgn::m_RgnBodyCadCornerPos[2].y; }
	if ( Region.maxX < CAOIRgn::m_RgnBodyCadCornerPos[2].x ) { Region.maxX = CAOIRgn::m_RgnBodyCadCornerPos[2].x; }
	if ( Region.maxY < CAOIRgn::m_RgnBodyCadCornerPos[2].y ) { Region.maxY = CAOIRgn::m_RgnBodyCadCornerPos[2].y; }

	if ( Region.minX > CAOIRgn::m_RgnBodyCadCornerPos[3].x ) { Region.minX = CAOIRgn::m_RgnBodyCadCornerPos[3].x; }
	if ( Region.minY > CAOIRgn::m_RgnBodyCadCornerPos[3].y ) { Region.minY = CAOIRgn::m_RgnBodyCadCornerPos[3].y; }
	if ( Region.maxX < CAOIRgn::m_RgnBodyCadCornerPos[3].x ) { Region.maxX = CAOIRgn::m_RgnBodyCadCornerPos[3].x; }
	if ( Region.maxY < CAOIRgn::m_RgnBodyCadCornerPos[3].y ) { Region.maxY = CAOIRgn::m_RgnBodyCadCornerPos[3].y; }
}
//-------------------------------------------------------------------------------------//
void CAOIRgn::GetRgnRoiStageRegion(TREGION4D &Region)//取得計算區域在Stage的範圍
{
	Region.minX = Region.maxX = CAOIRgn::m_RgnRoiStageCornerPos[0].x;
	Region.minY = Region.maxY = CAOIRgn::m_RgnRoiStageCornerPos[0].y;

	if ( Region.minX > CAOIRgn::m_RgnRoiStageCornerPos[1].x ) { Region.minX = CAOIRgn::m_RgnRoiStageCornerPos[1].x; }
	if ( Region.minY > CAOIRgn::m_RgnRoiStageCornerPos[1].y ) { Region.minY = CAOIRgn::m_RgnRoiStageCornerPos[1].y; }
	if ( Region.maxX < CAOIRgn::m_RgnRoiStageCornerPos[1].x ) { Region.maxX = CAOIRgn::m_RgnRoiStageCornerPos[1].x; }
	if ( Region.maxY < CAOIRgn::m_RgnRoiStageCornerPos[1].y ) { Region.maxY = CAOIRgn::m_RgnRoiStageCornerPos[1].y; }

	if ( Region.minX > CAOIRgn::m_RgnRoiStageCornerPos[2].x ) { Region.minX = CAOIRgn::m_RgnRoiStageCornerPos[2].x; }
	if ( Region.minY > CAOIRgn::m_RgnRoiStageCornerPos[2].y ) { Region.minY = CAOIRgn::m_RgnRoiStageCornerPos[2].y; }
	if ( Region.maxX < CAOIRgn::m_RgnRoiStageCornerPos[2].x ) { Region.maxX = CAOIRgn::m_RgnRoiStageCornerPos[2].x; }
	if ( Region.maxY < CAOIRgn::m_RgnRoiStageCornerPos[2].y ) { Region.maxY = CAOIRgn::m_RgnRoiStageCornerPos[2].y; }

	if ( Region.minX > CAOIRgn::m_RgnRoiStageCornerPos[3].x ) { Region.minX = CAOIRgn::m_RgnRoiStageCornerPos[3].x; }
	if ( Region.minY > CAOIRgn::m_RgnRoiStageCornerPos[3].y ) { Region.minY = CAOIRgn::m_RgnRoiStageCornerPos[3].y; }
	if ( Region.maxX < CAOIRgn::m_RgnRoiStageCornerPos[3].x ) { Region.maxX = CAOIRgn::m_RgnRoiStageCornerPos[3].x; }
	if ( Region.maxY < CAOIRgn::m_RgnRoiStageCornerPos[3].y ) { Region.maxY = CAOIRgn::m_RgnRoiStageCornerPos[3].y; }
}
//-------------------------------------------------------------------------------------//
void CAOIRgn::GetRgnBodyStageRegion(TREGION4D &Region)//取得計算區域在Stage的範圍	
{
	Region.minX = Region.maxX = CAOIRgn::m_RgnBodyStageCornerPos[0].x;
	Region.minY = Region.maxY = CAOIRgn::m_RgnBodyStageCornerPos[0].y;

	if ( Region.minX > CAOIRgn::m_RgnBodyStageCornerPos[1].x ) { Region.minX = CAOIRgn::m_RgnBodyStageCornerPos[1].x; }
	if ( Region.minY > CAOIRgn::m_RgnBodyStageCornerPos[1].y ) { Region.minY = CAOIRgn::m_RgnBodyStageCornerPos[1].y; }
	if ( Region.maxX < CAOIRgn::m_RgnBodyStageCornerPos[1].x ) { Region.maxX = CAOIRgn::m_RgnBodyStageCornerPos[1].x; }
	if ( Region.maxY < CAOIRgn::m_RgnBodyStageCornerPos[1].y ) { Region.maxY = CAOIRgn::m_RgnBodyStageCornerPos[1].y; }

	if ( Region.minX > CAOIRgn::m_RgnBodyStageCornerPos[2].x ) { Region.minX = CAOIRgn::m_RgnBodyStageCornerPos[2].x; }
	if ( Region.minY > CAOIRgn::m_RgnBodyStageCornerPos[2].y ) { Region.minY = CAOIRgn::m_RgnBodyStageCornerPos[2].y; }
	if ( Region.maxX < CAOIRgn::m_RgnBodyStageCornerPos[2].x ) { Region.maxX = CAOIRgn::m_RgnBodyStageCornerPos[2].x; }
	if ( Region.maxY < CAOIRgn::m_RgnBodyStageCornerPos[2].y ) { Region.maxY = CAOIRgn::m_RgnBodyStageCornerPos[2].y; }

	if ( Region.minX > CAOIRgn::m_RgnBodyStageCornerPos[3].x ) { Region.minX = CAOIRgn::m_RgnBodyStageCornerPos[3].x; }
	if ( Region.minY > CAOIRgn::m_RgnBodyStageCornerPos[3].y ) { Region.minY = CAOIRgn::m_RgnBodyStageCornerPos[3].y; }
	if ( Region.maxX < CAOIRgn::m_RgnBodyStageCornerPos[3].x ) { Region.maxX = CAOIRgn::m_RgnBodyStageCornerPos[3].x; }
	if ( Region.maxY < CAOIRgn::m_RgnBodyStageCornerPos[3].y ) { Region.maxY = CAOIRgn::m_RgnBodyStageCornerPos[3].y; }
}
//-------------------------------------------------------------------------------------//
bool CAOIRgn::CreateRgnSubList_MatrixField(bool ByCadRegion)//建立區域的子列表-等間距
{	
	bool IsOK = true;
	if ( false == ByCadRegion )
	{	IsOK = CreateRgnSubList_MatrixField_Stage(); }
	else
	{	IsOK = CreateRgnSubList_MatrixField_Cad(); }
	return IsOK;	
}
//-------------------------------------------------------------------------------------//
bool CAOIRgn::CreateRgnSubList_MatrixField_Cad()//建立區域的子列表-等間距
{
	CAOIRgn::ClearRgnSubList();	
	CAOIField *FieldPtr = CAOIRgn::GetRgnFieldPtr();
	if ( NULL == FieldPtr ) { return false; }

	TREGION4D  rgnCad;		
	TREGION4D  rgnCadFieldOuter;
	CAOIRgn::GetRgnRoiCadRegion(rgnCad);
	FieldPtr->GetFieldCadRgn_Outer(rgnCadFieldOuter);
	if ( rgnCad.minX>=rgnCadFieldOuter.minX && 
	 rgnCad.minY>=rgnCadFieldOuter.minY &&
	 rgnCad.maxX<=rgnCadFieldOuter.maxX && 
	 rgnCad.maxY<=rgnCadFieldOuter.maxY )
	{
		return true;
	}

	int        i=0, j=0;	
	TSIZE2D    szGrid;
	TPOINT2D   ptGrid;		
	TREGION4D  rgnGrid;	
	CAOIRgn   *RgnSubPtr = NULL;
	TREGION4D  rgnStageField;
	TREGION4D  rgnCadFieldInner;

	const double CadSizeW = rgnCad.maxX-rgnCad.minX;
	const double CadSizeH = rgnCad.maxY-rgnCad.minY;
	const TPOINT2D   PosCadField = FieldPtr->GetFieldCadPos();		
	const double FieldSizeW_Inner = FieldPtr->GetFieldSizeW_Inner();
	const double FieldSizeH_Inner = FieldPtr->GetFieldSizeH_Inner();
	const double HalfFieldSizeW_Inner = FieldSizeW_Inner;
	const double HalfFieldSizeH_Inner = FieldSizeH_Inner;
	const double ExtSubDummyW = 50;//額外放大尺寸
	const double ExtSubDummyH = 50;//額外放大尺寸
	const bool   SignX = AOIDataCollect.GetStageSignPositiveX();
	const bool   SignY = AOIDataCollect.GetStageSignPositiveY();

	int    NMinX = 0;//扣除目前的FOV邊界範圍後向左邊還延伸多少各FOV
	int    NMaxX = 0;//扣除目前的FOV邊界範圍後向右邊還延伸多少各FOV
	int    NMinY = 0;//扣除目前的FOV邊界範圍後向上邊還延伸多少各FOV
	int    NMaxY = 0;//扣除目前的FOV邊界範圍後向下邊還延伸多少各FOV

	//先扣除目前FOV的邊界範圍
	FieldPtr->GetFieldCadRgn_Inner(rgnCadFieldInner);
	double ExtMinW = rgnCadFieldInner.minX-rgnCad.minX;
	double ExtMaxW = rgnCad.maxX-rgnCadFieldInner.maxX;
	double ExtMinH = rgnCadFieldInner.minY-rgnCad.minY;
	double ExtMaxH = rgnCad.maxY-rgnCadFieldInner.maxY;
	
	if ( ExtMinW < 0 ) 
	{	ExtMinW = 0.0;	}
	else
	{	NMinX = JetAPI::Ceil(ExtMinW/FieldSizeW_Inner);	}
	if ( ExtMaxW < 0 ) 
	{	ExtMaxW = 0.0;	}
	else
	{	NMaxX = JetAPI::Ceil(ExtMaxW/FieldSizeW_Inner);	}
	if ( ExtMinH < 0 ) 
	{	ExtMinH = 0.0;	}
	else
	{	NMinY = JetAPI::Ceil(ExtMinH/FieldSizeH_Inner);	}
	if ( ExtMaxH < 0 ) 
	{	ExtMaxH = 0.0;	}
	else
	{	NMaxY = JetAPI::Ceil(ExtMaxH/FieldSizeH_Inner);	}	
	
	TPOINT2D  DifCadPos;
	TPOINT3D  GridStagePos;	
	TREGION4D rgnCadStartField, rgnCadNextField;
	TPOINT2D  RgnCadPos = CAOIRgn::GetRgnCadPos();
	TPOINT3D  RgnStagePos = CAOIRgn::GetRgnStagePos();
	const int NGridX = NMinX+NMaxX+1;
	const int NGridY = NMinY+NMaxY+1;	

	rgnCadStartField.minX = rgnCadFieldInner.minX-(NMinX*FieldSizeW_Inner);
	rgnCadStartField.minY = rgnCadFieldInner.minY-(NMinY*FieldSizeH_Inner);
	rgnCadStartField.maxX = rgnCadStartField.minX + FieldSizeW_Inner;
	rgnCadStartField.maxY = rgnCadStartField.minY + FieldSizeH_Inner;

	for ( i=0; i<NGridY; i++ )
	{
		for ( j=0; j<NGridX; j++ )
		{
			rgnCadNextField.minX = rgnCadStartField.minX+(j*FieldSizeW_Inner);
			rgnCadNextField.minY = rgnCadStartField.minY+(i*FieldSizeH_Inner);
			rgnCadNextField.maxX = rgnCadNextField.minX + FieldSizeW_Inner;
			rgnCadNextField.maxY = rgnCadNextField.minY + FieldSizeH_Inner;

			rgnGrid = rgnCad;
			//避免Field位置匹配錯誤, 導致子框尺寸異常
			if ( rgnGrid.minX > rgnCadNextField.maxX ) { continue; }
			if ( rgnGrid.minY > rgnCadNextField.maxY ) { continue; }
			if ( rgnGrid.maxX < rgnCadNextField.minX ) { continue; }
			if ( rgnGrid.maxY < rgnCadNextField.minY ) { continue; }

			if ( 0 != i )//Y
			{
				if ( rgnGrid.minY < rgnCadNextField.minY ) 
				{	rgnGrid.minY = rgnCadNextField.minY; }
			}
			if ( (NGridY-1)!=i )
			{
				if ( rgnGrid.maxY > rgnCadNextField.maxY ) 
				{	rgnGrid.maxY = rgnCadNextField.maxY; }
			}

			if ( 0 != j )//X
			{
				if ( rgnGrid.minX < rgnCadNextField.minX ) 
				{	rgnGrid.minX = rgnCadNextField.minX; }
			}
			if ( (NGridX-1)!=j )
			{
				if ( rgnGrid.maxX > rgnCadNextField.maxX ) 
				{	rgnGrid.maxX = rgnCadNextField.maxX; }
			}			

			//放大區域
			rgnGrid.minX -= ExtSubDummyW;
			rgnGrid.minY -= ExtSubDummyH;
			rgnGrid.maxX += ExtSubDummyW;
			rgnGrid.maxY += ExtSubDummyH;

			ptGrid.x = (rgnGrid.minX+rgnGrid.maxX)*0.5;
			ptGrid.y = (rgnGrid.minY+rgnGrid.maxY)*0.5;
			szGrid.cx = rgnGrid.maxX-rgnGrid.minX;
			szGrid.cy = rgnGrid.maxY-rgnGrid.minY;
			RgnSubPtr = AOIObjManager.CreateRgnObj();
			if ( NULL == RgnSubPtr ) { return false; }

			RgnSubPtr->SetRgnParent(this);
			RgnSubPtr->SetRgnAngle(0);
			RgnSubPtr->SetRgnCadPos(ptGrid);
			RgnSubPtr->SetRgnRoiSizeW(szGrid.cx);
			RgnSubPtr->SetRgnRoiSizeH(szGrid.cy);
			RgnSubPtr->CalcRgnCadCornerPos();

			DifCadPos.x = ptGrid.x-RgnCadPos.x;
			DifCadPos.y = ptGrid.y-RgnCadPos.y;
			if ( true == SignX )
			{	GridStagePos.x = RgnStagePos.x + DifCadPos.x; }
			else
			{	GridStagePos.x = RgnStagePos.x - DifCadPos.x; }
			if ( true == SignY )
			{	GridStagePos.y = RgnStagePos.y + DifCadPos.y; }
			else
			{	GridStagePos.y = RgnStagePos.y - DifCadPos.y; }
			RgnSubPtr->SetRgnStagePos(GridStagePos);
			RgnSubPtr->LayoutRgnStageCornerPos();
			AddRgnSubRgnPtr(RgnSubPtr);			
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIRgn::CreateRgnSubList_MatrixField_Stage()//建立區域的子列表-等間距
{
	CAOIRgn::ClearRgnSubList();	
	CAOIField *FieldPtr = CAOIRgn::GetRgnFieldPtr();
	if ( NULL == FieldPtr ) { return false; }
	
	TREGION4D  rgnStage;
	TREGION4D  rgnStageFieldOuter;		
	CAOIRgn::GetRgnRoiStageRegion(rgnStage);
	FieldPtr->GetFieldStageRgn_Outer(rgnStageFieldOuter);
	if ( rgnStage.minX>=rgnStageFieldOuter.minX && 
	 rgnStage.minY>=rgnStageFieldOuter.minY &&
	 rgnStage.maxX<=rgnStageFieldOuter.maxX && 
	 rgnStage.maxY<=rgnStageFieldOuter.maxY )
	{
		return true;
	}

	int        i=0, j=0;	
	TSIZE2D    szGrid;
	TPOINT3D   ptGrid;		
	TREGION4D  rgnGrid;	
	CAOIRgn   *RgnSubPtr = NULL;
	TREGION4D  rgnStageField;
	TREGION4D  rgnStageFieldInner;
	const double StageSizeW = rgnStage.maxX-rgnStage.minX;
	const double StageSizeH = rgnStage.maxY-rgnStage.minY;
	const TPOINT3D   PosStageField = FieldPtr->GetFieldStagePos();		
	const double FieldSizeW_Inner = FieldPtr->GetFieldSizeW_Inner();
	const double FieldSizeH_Inner = FieldPtr->GetFieldSizeH_Inner();	
	const double ExtSubDummyW = 50;//額外放大尺寸
	const double ExtSubDummyH = 50;//額外放大尺寸
	const bool   SignX = AOIDataCollect.GetStageSignPositiveX();
	const bool   SignY = AOIDataCollect.GetStageSignPositiveY();

	int    NMinX = 0;//扣除目前的FOV邊界範圍後向左邊還延伸多少各FOV
	int    NMaxX = 0;//扣除目前的FOV邊界範圍後向右邊還延伸多少各FOV
	int    NMinY = 0;//扣除目前的FOV邊界範圍後向上邊還延伸多少各FOV
	int    NMaxY = 0;//扣除目前的FOV邊界範圍後向下邊還延伸多少各FOV

	//先扣除目前FOV的邊界範圍
	FieldPtr->GetFieldStageRgn_Inner(rgnStageFieldInner);
	double ExtMinW = rgnStageFieldInner.minX-rgnStage.minX;
	double ExtMaxW = rgnStage.maxX-rgnStageFieldInner.maxX;
	double ExtMinH = rgnStageFieldInner.minY-rgnStage.minY;
	double ExtMaxH = rgnStage.maxY-rgnStageFieldInner.maxY;
	
	if ( ExtMinW < 0 ) 
	{	ExtMinW = 0.0;	}
	else
	{	NMinX = JetAPI::Ceil(ExtMinW/FieldSizeW_Inner);	}
	if ( ExtMaxW < 0 ) 
	{	ExtMaxW = 0.0;	}
	else
	{	NMaxX = JetAPI::Ceil(ExtMaxW/FieldSizeW_Inner);	}
	if ( ExtMinH < 0 ) 
	{	ExtMinH = 0.0;	}
	else
	{	NMinY = JetAPI::Ceil(ExtMinH/FieldSizeH_Inner);	}
	if ( ExtMaxH < 0 ) 
	{	ExtMaxH = 0.0;	}
	else
	{	NMaxY = JetAPI::Ceil(ExtMaxH/FieldSizeH_Inner);	}		
	
	TPOINT2D  GridCadPos;
	TPOINT3D  DifStagePos;
	TREGION4D SubRgnRegion;
	TREGION4D rgnStageStartField, rgnStageNextField;
	TPOINT2D  RgnCadPos = CAOIRgn::GetRgnCadPos();
	TPOINT3D  RgnStagePos = CAOIRgn::GetRgnStagePos();
	const int NGridX = NMaxX+NMinX+1;
	const int NGridY = NMaxY+NMinY+1;	

	rgnStageStartField.minX = rgnStageFieldInner.minX-(NMinX*FieldSizeW_Inner);
	rgnStageStartField.minY = rgnStageFieldInner.minY-(NMinY*FieldSizeH_Inner);
	rgnStageStartField.maxX = rgnStageStartField.minX + FieldSizeW_Inner;
	rgnStageStartField.maxY = rgnStageStartField.minY + FieldSizeH_Inner;

	for ( i=0; i<NGridY; i++ )
	{
		for ( j=0; j<NGridX; j++ )
		{
			rgnStageNextField.minX = rgnStageStartField.minX+(j*FieldSizeW_Inner);
			rgnStageNextField.minY = rgnStageStartField.minY+(i*FieldSizeH_Inner);
			rgnStageNextField.maxX = rgnStageNextField.minX + FieldSizeW_Inner;
			rgnStageNextField.maxY = rgnStageNextField.minY + FieldSizeH_Inner;

			rgnGrid = rgnStage;
			//避免Field位置匹配錯誤, 導致子框尺寸異常
			if ( rgnGrid.minX > rgnStageNextField.maxX ) { continue; }
			if ( rgnGrid.minY > rgnStageNextField.maxY ) { continue; }
			if ( rgnGrid.maxX < rgnStageNextField.minX ) { continue; }
			if ( rgnGrid.maxY < rgnStageNextField.minY ) { continue; }

			if ( 0 != i )//Y
			{
				if ( rgnGrid.minY < rgnStageNextField.minY ) 
				{	rgnGrid.minY = rgnStageNextField.minY; }
			}
			if ( (NGridY-1)!=i )
			{
				if ( rgnGrid.maxY > rgnStageNextField.maxY ) 
				{	rgnGrid.maxY = rgnStageNextField.maxY; }
			}

			if ( 0 != j )//X
			{
				if ( rgnGrid.minX < rgnStageNextField.minX ) 
				{	rgnGrid.minX = rgnStageNextField.minX; }
			}
			if ( (NGridX-1)!=j )
			{
				if ( rgnGrid.maxX > rgnStageNextField.maxX ) 
				{	rgnGrid.maxX = rgnStageNextField.maxX; }
			}			

			//放大區域
			rgnGrid.minX -= ExtSubDummyW;
			rgnGrid.minY -= ExtSubDummyH;
			rgnGrid.maxX += ExtSubDummyW;
			rgnGrid.maxY += ExtSubDummyH;

			ptGrid.x = (rgnGrid.minX+rgnGrid.maxX)*0.5;
			ptGrid.y = (rgnGrid.minY+rgnGrid.maxY)*0.5;
			szGrid.cx = ::fabs(rgnGrid.maxX-rgnGrid.minX);
			szGrid.cy = ::fabs(rgnGrid.maxY-rgnGrid.minY);
			RgnSubPtr = AOIObjManager.CreateRgnObj();
			if ( NULL == RgnSubPtr ) { return false; }

			DifStagePos.x = ptGrid.x-RgnStagePos.x;
			DifStagePos.y = ptGrid.y-RgnStagePos.y;
			if ( true == SignX )
			{	GridCadPos.x = RgnCadPos.x + DifStagePos.x; }
			else
			{	GridCadPos.x = RgnCadPos.x - DifStagePos.x; }
			if ( true == SignY )
			{	GridCadPos.y = RgnCadPos.y + DifStagePos.y; }
			else
			{	GridCadPos.y = RgnCadPos.y - DifStagePos.y; }

			RgnSubPtr->SetRgnParent(this);
			RgnSubPtr->SetRgnAngle(0);
			RgnSubPtr->SetRgnRoiSizeW(szGrid.cx);
			RgnSubPtr->SetRgnRoiSizeH(szGrid.cy);
			RgnSubPtr->SetRgnCadPos(GridCadPos);			
			RgnSubPtr->CalcRgnCadCornerPos();
			RgnSubPtr->SetRgnStagePos(ptGrid);
			RgnSubPtr->LayoutRgnStageCornerPos();
			AddRgnSubRgnPtr(RgnSubPtr);
			
			RgnSubPtr->GetRgnRoiStageRegion(SubRgnRegion);
			if ( SubRgnRegion.minX<rgnStage.minX || 
				 SubRgnRegion.minY<rgnStage.minY || 
				 SubRgnRegion.maxX>rgnStage.maxX || 
				 SubRgnRegion.maxY>rgnStage.maxY )
			{
				i = i;
			}
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIRgn::CreateRgnSubList_RandomField()//建立區域的子列表-任意位置
{
	int        i=0, j=0;
	TPOINT2D   ptGrid;
	TSIZE2D    szGrid;
	double     FOVInnerW=0, FOVInnerH=0;
	double     FOVOuterW=0, FOVOuterH=0;
	TREGION4D  rgnCad, rgnGrid;
	CAOIRgn   *RgnSubPtr = NULL;			

	CAOIRgn::ClearRgnSubList();
	CAOIRgn::GetRgnRoiCadRegion(rgnCad);
	AOIDataCollect.GetFovSizeOuter(FOVOuterW, FOVOuterH);	
	const double CadSizeW = rgnCad.maxX-rgnCad.minX;
	const double CadSizeH = rgnCad.maxY-rgnCad.minY;
	if ( CadSizeW<FOVOuterW && CadSizeH<FOVOuterH )
	{	return true; }	
	
	TPOINT2D  DifCadPos;
	TPOINT3D  GridStagePos;		
	TPOINT2D  RgnCadPos = CAOIRgn::GetRgnCadPos();
	TPOINT3D  RgnStagePos = CAOIRgn::GetRgnStagePos();
	AOIDataCollect.GetFovSizeInner(FOVInnerW, FOVInnerH);
	AOIDataCollect.GetFovSizeUsed(FOVInnerW, FOVInnerH);

	const int NGridX = JetAPI::Ceil(CadSizeW/FOVInnerW);
	const int NGridY = JetAPI::Ceil(CadSizeH/FOVInnerH);
	const double ExtSubDummyW = 50;//額外放大尺寸
	const double ExtSubDummyH = 50;//額外放大尺寸
	const double CellSizeW = CadSizeW/NGridX;//FOVInnerW
	const double CellSizeH = CadSizeH/NGridY;//FOVInnerH
	const bool   SignX = AOIDataCollect.GetStageSignPositiveX();
	const bool   SignY = AOIDataCollect.GetStageSignPositiveY();	

	GridStagePos.z = CAOIRgn::m_RgnStagePos.z;
	for ( i=0; i<NGridY; i++ )
	{
		for ( j=0; j<NGridX; j++ )
		{
			rgnGrid.minX = rgnCad.minX+(j*CellSizeW);
			rgnGrid.maxX = rgnGrid.minX+CellSizeW;
			rgnGrid.minY = rgnCad.minY+(i*CellSizeH);
			rgnGrid.maxY = rgnGrid.minY+CellSizeH;			
			if ( rgnGrid.maxX > rgnCad.maxX ) { rgnGrid.maxX = rgnCad.maxX; }
			if ( rgnGrid.maxY > rgnCad.maxY ) { rgnGrid.maxY = rgnCad.maxY; }

			//放大區域
			rgnGrid.minX -= ExtSubDummyW;
			rgnGrid.minY -= ExtSubDummyH;
			rgnGrid.maxX += ExtSubDummyW;
			rgnGrid.maxY += ExtSubDummyH;

			ptGrid.x = (rgnGrid.minX+rgnGrid.maxX)*0.5;
			ptGrid.y = (rgnGrid.minY+rgnGrid.maxY)*0.5;
			szGrid.cx = rgnGrid.maxX-rgnGrid.minX;
			szGrid.cy = rgnGrid.maxY-rgnGrid.minY;
			RgnSubPtr = AOIObjManager.CreateRgnObj();
			if ( NULL == RgnSubPtr ) { return false; }

			RgnSubPtr->SetRgnParent(this);
			RgnSubPtr->SetRgnAngle(0);
			RgnSubPtr->SetRgnCadPos(ptGrid);
			RgnSubPtr->SetRgnRoiSizeW(szGrid.cx);
			RgnSubPtr->SetRgnRoiSizeH(szGrid.cy);
			RgnSubPtr->CalcRgnCadCornerPos();

			DifCadPos.x = ptGrid.x-RgnCadPos.x;
			DifCadPos.y = ptGrid.y-RgnCadPos.y;
			if ( true == SignX )
			{	GridStagePos.x = RgnStagePos.x + DifCadPos.x; }
			else
			{	GridStagePos.x = RgnStagePos.x - DifCadPos.x; }
			if ( true == SignY )
			{	GridStagePos.y = RgnStagePos.y + DifCadPos.y; }
			else
			{	GridStagePos.y = RgnStagePos.y - DifCadPos.y; }
			RgnSubPtr->SetRgnStagePos(GridStagePos);
			RgnSubPtr->LayoutRgnStageCornerPos();
			AddRgnSubRgnPtr(RgnSubPtr);			
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIRgn::ClearRgnSubList()//清除檢測區域的子列表
{
	size_t  i =0;	
	const size_t SubCount = CAOIRgn::m_RgnSubList.size();
	for ( i=0; i<SubCount; i++ )
	{
		if ( NULL == m_RgnSubList[i] ) { continue; }
		AOIObjManager.DestroyRgnObj(m_RgnSubList[i]);
		m_RgnSubList[i] = NULL;		
	}
	m_RgnSubList.clear();

	//檢測區域的子列表取得影像的數量	
	m_RgnSubFillFrameCount = 0;	
}
//-------------------------------------------------------------------------------------//
bool CAOIRgn::GetRgnRawSpaceEnabled() const
{
	return m_RgnRawSpaceEnabled;
}
//-------------------------------------------------------------------------------------//
void CAOIRgn::SetRgnRawSpaceEnabled(bool val)
{
	m_RgnRawSpaceEnabled = val;
}
//-------------------------------------------------------------------------------------//
bool CAOIRgn::GetRgnDynamicFrameRectMode() const
{
	return m_RgnDynamicFrameRectMode;
}
//-------------------------------------------------------------------------------------//
void CAOIRgn::SetRgnDynamicFrameRectMode(bool val)
{
	m_RgnDynamicFrameRectMode = val;
}
//-------------------------------------------------------------------------------------//
size_t CAOIRgn::GetRgnSubRgnCount() const//取得檢測區域的子數量
{
	return m_RgnSubList.size();
}
//-------------------------------------------------------------------------------------//
CAOIRgn* CAOIRgn::GetRgnSubRgnPtr(size_t index, bool check) const//取得檢測區域的子指標	
{
	if ( true == check )
	{
		const size_t Count = m_RgnSubList.size();
		if ( index >= Count ) 
		{	return NULL; }
	}
	return m_RgnSubList[index];
}
//-------------------------------------------------------------------------------------//
bool CAOIRgn::SetRgnSubRgnAllFinish()//設定子檢測框執行完畢
{
	size_t i=0;
	CAOIRgn*   SubRgnPtr = NULL;
	const size_t SubRgnCount = GetRgnSubRgnCount();
	for ( i=0; i<SubRgnCount; i++ )
	{
		SubRgnPtr = GetRgnSubRgnPtr(i, false);
		if ( NULL == SubRgnPtr ) { continue; }		
		SubRgnPtr->SetRgnCalculated(true);
		SubRgnPtr->SetRgnCalcState(REGION_CALC_DONE);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIRgn::AllocateRgnFullImageBuffer(size_t index, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, IMAGE_PTR &ImagePtr)//建立子檢測框全圖的圖像記憶體區塊
{
	const char fnName[] = "CAOIRgn::AllocateRgnFullImageBuffer";
	if ( index >= FRAME_MAX_COUNT ) { return false; }
	CAOIField *FieldPtr = GetRgnFieldPtr();
	if ( NULL == FieldPtr ) { return false; }
	CAOIFrame *FramePtr = FieldPtr->GetFieldFramePtr(index, true);
	if ( NULL == FramePtr )	{	return false;	}
	FRAME_TYPE FrameType = FramePtr->GetFrameType();
	if ( FRAME_SPACE == FrameType ) { return false; }
	IMAGE_SIZE ImageBitCount=FramePtr->GetFrameImageBitCount();
	if ( FRAME_BAYER == FrameType )
	{	ImageBitCount = 24; }

	TREGION4D  StageRgn;
	double     ResX=0, ResY=0;
	IMAGE_SIZE CameraW=0, CameraH=0;
	CAMERA_ID CameraID = CAOIRgn::GetRgnCameraID();	
	CAOIRgn::GetRgnRoiStageRegion(StageRgn);	
	AOIDataCollect.GetCameraImageInfo(CameraID, CameraW, CameraH, ResX, ResY);	

	const int    nAlign = 4;
	IMAGE_PTR    FullImagePtr = NULL;		
	const IMAGE_SIZE FullImageW = JetAPI::Floor((StageRgn.maxX-StageRgn.minX)/ResX);
	const IMAGE_SIZE FullImageH = JetAPI::Floor((StageRgn.maxY-StageRgn.minY)/ResY);
	const IMAGE_SIZE FullBitCount = ImageBitCount;
	const IMAGE_SIZE FullImageStep = JetAPI::GetBMPImagePixelsPerLine(FullImageW, FullBitCount, nAlign);
	const size_t FullImageBufferSize = ImageAPI.CalcBufferSize(FullImageStep, FullImageH);
	if ( JetMemory.alloc_func(FullImageBufferSize, FullImagePtr, fnName, "FullImagePtr") == false )
	{	return false;	}

	ImageW = FullImageW;
	ImageH = FullImageH;
	ImageStep = FullImageStep;
	BitCount = FullBitCount;
	ImagePtr = FullImagePtr;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIRgn::FillRgnSubRgnFullImageBuffer(size_t index, IMAGE_SIZE FullImageW, IMAGE_SIZE FullImageH, IMAGE_SIZE FullImageStep, IMAGE_SIZE FullBitCount, IMAGE_PTR FullImagePtr)//拼接子檢測框全圖
{
	const char fnName[] = "CAOIRgn::FillRgnSubRgnFullImageBuffer";
	if ( index >= FRAME_MAX_COUNT ) { return false; }
	TREGION4D StageRgn;
	FRAME_TYPE FrameType = FRAME_NULL;	
	double ResX=0, ResY=0;
	CAMERA_ID  CameraID = GetRgnCameraID();
	IMAGE_PTR  ImagePtr=NULL;
	IMAGE_SIZE ImageW=0, ImageH=0, ImageStep=0, BitCount = 24;		
	const size_t FrameIdx = index;
	CAOIRgn*   SubRgnPtr = NULL;
	CAOIField* FieldPtr = NULL;
	CAOIFrame* FramePtr = NULL;	
	CAOIRgn::GetRgnRoiStageRegion(StageRgn);	
	AOIDataCollect.GetCameraImageInfo(CameraID, ImageW, ImageH, ResX, ResY);	

	CString str;
	CString ErrStr, ErrName;
	size_t k=0;
	int    i=0, j=0;
	int    u=0, v=0;	
	RECT   RoiRect={0};	
	int    SrcIdx=0, DstIdx=0;			
	IMAGE_PTR RoiPtr = NULL;
	IMAGE_SIZE RoiW=0, RoiH=0, RoiStep=0, RoiBitCount=0;
	int    CellW=0, CellH=0, CellStep=0;	
	IMAGE_PTR  CellPtr = NULL;
	double FovCpX=0, FovCpY=0;
	int    LocalX=0, LocalY=0;
	int    ImageCpX=0, ImageCpY=0;//跟機台方向有關係
	int    ImageStartX=0, ImageStartY=0;	
	const int    nAlign = 4;
	const double ImageGain = 4.0;
	const size_t RgnIndex = GetRgnIndex();
	const size_t SubRgnCount = GetRgnSubRgnCount();
	//const size_t SubRgnCount = 1;
	const double StartX = StageRgn.minX;
	const double StartY = StageRgn.minY;	
	const int nFullImageW = (int)(FullImageW);
	const int nFullImageH = (int)(FullImageH);
	const int nFullImageStep = (int)(FullImageStep);
	const bool SignX = AOIDataCollect.GetStageSignPositiveX();
	const bool SignY = AOIDataCollect.GetStageSignPositiveY();
	for ( k=0; k<SubRgnCount; k++ )
	{
		SubRgnPtr = GetRgnSubRgnPtr(k, false);
		if ( NULL == SubRgnPtr ) { continue; }
		FieldPtr = SubRgnPtr->GetRgnFieldPtr();		
		FramePtr = FieldPtr->GetFieldFramePtr(FrameIdx, true);
		ErrName.Format(_T("%s Sub[%02d]"), this->GetRgnDerivedName(), k+1);
		if ( NULL == FramePtr ) 
		{	return false;	}
		FrameType = FramePtr->GetFrameType();
		if ( FRAME_SPACE == FrameType ) 
		{	return false;	}		
		FramePtr->GetFrameImagePtr(ImageW, ImageH, ImageStep, BitCount, ImagePtr);		
		RoiRect = SubRgnPtr->GetRgnFrameImageRect();		
		RoiW=RoiRect.right-RoiRect.left;
		RoiH=RoiRect.bottom-RoiRect.top;
		RoiStep=JetAPI::GetBMPImagePixelsPerLine(RoiW, BitCount, nAlign);
		RoiBitCount=BitCount;		
		if ( ImageAPI.ExtractRoiImage(ImageW, ImageH, ImageStep, BitCount, ImagePtr, RoiRect, RoiStep, RoiPtr, false) == false )
		{
			ErrStr.Format(_T("Error, %s Extract Roi Image Fault (%d, %d, %d, %d)"), ErrName, RoiRect.left, RoiRect.top, RoiRect.right, RoiRect.bottom);
			SaveRgnMovingTimeMsg(ErrStr);
			return false;	
		}
		if ( FRAME_BAYER == FrameType )
		{	
			IMAGE_SIZE   DeBayerBit=0;
			IMAGE_SIZE   DeBayerStep=0;		
			IMAGE_PTR    DeBayerPtr=NULL;	
			BAYER_PATTERN_MODE BayerPattern = FramePtr->GetFrameBayerPattern();	
			BAYER_PATTERN_MODE BayerRoi = ImageAPI.ShiftBayerPattern(BayerPattern, RoiRect.left, RoiRect.top);
			if ( AOIDataCollect.ExecDebayerImage(fnName, RoiW, RoiH, RoiStep, RoiPtr, BayerRoi, DeBayerStep, DeBayerBit, DeBayerPtr) == false)		
			{
				JetMemory.free_func(RoiPtr);
				ErrStr.Format(_T("Error, %s ExecDebayerImage Fault"), ErrName);
				SaveRgnMovingTimeMsg(ErrStr);
				return false;	
			}
			JetMemory.free_func(RoiPtr);
			RoiPtr = DeBayerPtr;			
			RoiStep = DeBayerStep;
			RoiBitCount = DeBayerBit;
			DeBayerPtr = NULL;
		}
		CellW  = (int)(RoiW);
		CellH  = (int)(RoiH);
		CellPtr = RoiPtr;
		CellStep = (int)(RoiStep);
		FovCpX = SubRgnPtr->GetRgnStagePosX();
		FovCpY = SubRgnPtr->GetRgnStagePosY();		
		LocalX = (int)(((FovCpX-StartX)/ResX)+0.5);
		LocalY = (int)(((FovCpY-StartY)/ResY)+0.5);
		//LocalX = JetAPI::Floor((FovCpX-StartX)/ResX);
		//LocalY = JetAPI::Floor((FovCpY-StartY)/ResY);		
		ImageCpX = FullImageW-LocalX;//跟機台方向有關係
		ImageCpY = FullImageH-LocalY;//跟機台方向有關係		
		if ( true == SignX )
		{	ImageCpX = LocalX;	}
		else
		{	ImageCpX = FullImageW-LocalX;	}//JET6500
		if ( true == SignY )
		{	ImageCpY = FullImageH-LocalY;	}//JET6500
		else
		{	ImageCpY = LocalY; }
		ImageStartX = ImageCpX-(CellW/2);
		ImageStartY = ImageCpY-(CellH/2);
		
		if ( 8 == RoiBitCount )
		{
			for ( i=0; i<CellH; i++ )
			{
				v = i+ImageStartY;
				if ( v<0 || v>=nFullImageH ) { continue; }
				SrcIdx = i*CellStep;
				v = v*FullImageStep;
				for ( j=0; j<CellW; j++ )
				{
					u = j+ImageStartX;
					if ( u<0 || u>=nFullImageW ) 
					{ 
						SrcIdx ++;
						continue; 
					}				
					DstIdx = v+u;
					FullImagePtr[DstIdx++] = CellPtr[SrcIdx++];//B				
				}
			}
		}
		else if ( 24 == RoiBitCount )
		{
			for ( i=0; i<CellH; i++ )
			{
				v = i+ImageStartY;
				if ( v<0 || v>=nFullImageH ) { continue; }
				SrcIdx = i*CellStep;
				v = v*FullImageStep;
				for ( j=0; j<CellW; j++ )
				{
					u = j+ImageStartX;
					if ( u<0 || u>=nFullImageW ) 
					{ 
						SrcIdx +=3;
						continue; 
					}
					u = u*3;
					DstIdx = v+u;
					FullImagePtr[DstIdx++] = CellPtr[SrcIdx++];//B
					FullImagePtr[DstIdx++] = CellPtr[SrcIdx++];//G
					FullImagePtr[DstIdx++] = CellPtr[SrcIdx++];//R
				}
			}		
		}
		JetMemory.free_func(RoiPtr);
		//SubRgnPtr->SetRgnCalculated(true);
		//SubRgnPtr->SetRgnCalcState(REGION_CALC_DONE);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIRgn::FillRgnSubRgnFullFrame()//執行區域子檢測框圖像拼接
{	
	bool bIsOK = false;	
	CAOIField* FieldPtr = CAOIRgn::GetRgnFieldPtr();
	const size_t SubRgnCount = CAOIRgn::GetRgnSubRgnCount();	
	if ( NULL == FieldPtr ) { return false; }		
	size_t i = 0;	
	unsigned int FrameIdx = 0;		
	CAOIFrame *FramePtr=NULL;
	CAOIProject  *RgnProjectPtr = CAOIRgn::GetRgnProjectPtr();
	FRAME_TYPE FrameType = FRAME_NULL;	
	unsigned int  FrameUniqueID = 0;
	const size_t FrameCount = FieldPtr->GetFieldFramePtrCount();	
	MASK_PTR      MaskPtr = NULL;
	SPACE_PTR     SpacePtr = NULL;
	IMAGE_PTR	  ImagePtr = NULL;
	MASK_PTR      RawMaskPtr = NULL;
	SPACE_PTR     RawSpacePtr =NULL;
	IMAGE_SIZE	  ImageW = 0;
	IMAGE_SIZE	  ImageH = 0;
	IMAGE_SIZE	  ImageStep = 0;
	unsigned int  BitCount = 0;
	for ( i=0; i<FrameCount; i++ )
	{
		FramePtr = FieldPtr->GetFieldFramePtr(i, false);
		if ( NULL == FramePtr ) { continue; }
		FrameType = FramePtr->GetFrameType();
		FrameUniqueID = FramePtr->GetFrameUniqueID();
		if ( NULL != RgnProjectPtr )
		{	FrameUniqueID = RgnProjectPtr->GetProjectFrameUniqueID(i, false); }
		SetRgnImageUniqueID(i, FrameUniqueID);
		switch ( FrameType )
		{
		case FRAME_SPACE:
			bIsOK = FillRgnSubRgnFullSpace(i, ImageW, ImageH, ImageStep, BitCount, ImagePtr);
			break;
		default:			
			bIsOK = FillRgnSubRgnFullImage(i);	
			if ( true == bIsOK )
			{
				if (FrameUniqueID==GUIDEIMAGE_UNIQUE_ID && FrameType == FRAME_COLOR && ImagePtr == NULL)
				{	GetRgnImageBuffer(i, ImageW, ImageH, ImageStep, BitCount, ImagePtr, MaskPtr, SpacePtr, RawMaskPtr, RawSpacePtr);	}
			}
			break;
		}
		if ( false == bIsOK )
		{	return false;	}		
	}		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIRgn::FillRgnSubRgnFullImage(size_t FrameIdx)//執行區域子檢測框圖像拼接
{	
	CString str;
	CString ErrStr, ErrName;
	ErrName.Format(_T("%s_Fill"), this->GetRgnDerivedName());
	const size_t SubRgnCount = GetRgnSubRgnCount();
	if ( 0 == SubRgnCount ) 
	{
		ErrStr.Format(_T("Error, %s [0 == SubRgnCount] Fault"), ErrName);
		SaveRgnMovingTimeMsg(ErrStr);
		return false; 
	}
	if ( FrameIdx >= FRAME_MAX_COUNT ) { return false; }
	
	bool         bSave = false;
	IMAGE_PTR    FullImagePtr = NULL;	
	IMAGE_SIZE   FullImageW = 0;
	IMAGE_SIZE   FullImageH = 0;
	IMAGE_SIZE   FullBitCount = 0;
	IMAGE_SIZE   FullImageStep = 0;	
	const size_t RgnIndex = GetRgnIndex();
	
	if ( CAOIRgn::AllocateRgnFullImageBuffer(FrameIdx, FullImageW, FullImageH, FullImageStep, FullBitCount, FullImagePtr) == false )
	{	
		SetRgnSubRgnAllFinish();
		ErrStr.Format(_T("Error, %s AllocateRgnFullImageBuffer Fault"), ErrName);
		SaveRgnMovingTimeMsg(ErrStr);
		return false; 
	}
	
	if ( CAOIRgn::FillRgnSubRgnFullImageBuffer(FrameIdx, FullImageW, FullImageH, FullImageStep, FullBitCount, FullImagePtr) == false )
	{
		JetMemory.free_func(FullImagePtr);
		SetRgnSubRgnAllFinish();
		ErrStr.Format(_T("Error, %s FillRgnSubRgnFullImageBuffer Fault"), ErrName);
		SaveRgnMovingTimeMsg(ErrStr);
		return false;
	}
	CString ComName = this->GetRgnDerivedName();	
	SetRgnImageBuffer(FrameIdx, FullImageW, FullImageH, FullImageStep, FullBitCount, FullImagePtr, NULL, NULL, NULL, NULL);		
#ifdef _DEBUG	
	if ( true == bSave )
	{
		str.Format(_T("%s\\%s_SubAll[%d#%d].JPG"), AOIDataCollect.GetAOITempDirectory(), ComName, RgnIndex+1, FrameIdx+1);
		ImageAPI.SaveImage(str, FullImageW, FullImageH, FullImageStep, FullBitCount, FullImagePtr, true);
	}
#else	
	if ( true == bSave )
	{
		str.Format(_T("%s\\%s_SubAll[%d#%d].JPG"), AOIDataCollect.GetAOITempDirectory(), ComName, RgnIndex+1, FrameIdx+1);
		ImageAPI.SaveImage(str, FullImageW, FullImageH, FullImageStep, FullBitCount, FullImagePtr, true);
	}
#endif//_DEBUG
	//JetMemory.free_func(FullImagePtr);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIRgn::AllocateRgnFullSpaceBuffer(size_t index, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, SPACE_PTR &SpacePtr, MASK_PTR &MaskPtr)//建立子檢測框全圖的圖像記憶體區塊
{
	const char fnName[] = "CAOIRgn::AllocateRgnFullSpaceBuffer";
	CAOIField *FieldPtr = GetRgnFieldPtr();		
	if ( NULL == FieldPtr ) { return false; }
	CAOIFrame *FramePtr = FieldPtr->GetFieldFramePtr(index, true);
	if ( NULL == FramePtr )	{	return false;	}
	FRAME_TYPE FrameType = FramePtr->GetFrameType();
	if ( FRAME_SPACE != FrameType )	{	return false;	}
	IMAGE_SIZE ImageBitCount=FramePtr->GetFrameImageBitCount();

	TREGION4D  StageRgn;
	double     ResX=0, ResY=0;
	IMAGE_SIZE CameraW=0, CameraH=0;
	CAMERA_ID CameraID = CAOIRgn::GetRgnCameraID();	
	CAOIRgn::GetRgnRoiStageRegion(StageRgn);	
	AOIDataCollect.GetCameraImageInfo(CameraID, CameraW, CameraH, ResX, ResY);	

	MASK_PTR     FullMaskPtr = NULL;
	SPACE_PTR    FullSpacePtr = NULL;	
	const IMAGE_SIZE FullImageW = JetAPI::Floor((StageRgn.maxX-StageRgn.minX)/ResX);
	const IMAGE_SIZE FullImageH = JetAPI::Floor((StageRgn.maxY-StageRgn.minY)/ResY);
	const IMAGE_SIZE FullBitCount = ImageBitCount;
	const IMAGE_SIZE FullImageStep = JetAPI::GetBMPImagePixelsPerLine(FullImageW, FullBitCount, 4);
	const size_t FullImageBufferSize = ImageAPI.CalcBufferSize(FullImageStep, FullImageH);
	if ( JetMemory.alloc_func(FullImageBufferSize, FullMaskPtr, fnName, "FullMaskPtr") == false ||
		 JetMemory.alloc_func(FullImageBufferSize, FullSpacePtr, fnName, "FullSpacePtr") == false )
	{	
		JetMemory.free_func(FullMaskPtr);
		JetMemory.free_func(FullSpacePtr);
		return false;	
	}

	ImageW = FullImageW;
	ImageH = FullImageH;
	ImageStep = FullImageStep;
	BitCount = FullBitCount;
	MaskPtr = FullMaskPtr;
	SpacePtr = FullSpacePtr;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIRgn::FillRgnSubRgnFullSpaceBuffer(size_t index, IMAGE_SIZE FullImageW, IMAGE_SIZE FullImageH, IMAGE_SIZE FullImageStep, IMAGE_SIZE FullBitCount, SPACE_PTR FullSpacePtr, MASK_PTR FullMaskPtr)//拼接子檢測框全圖
{
	if ( index >= FRAME_MAX_COUNT ) { return false; }
	TREGION4D StageRgn;
	FRAME_TYPE FrameType = FRAME_NULL;
	double ResX=0, ResY=0;
	CAMERA_ID CameraID = CAOIRgn::GetRgnCameraID();
	MASK_PTR   FrameMaskPtr=NULL;
	SPACE_PTR  FrameSpacePtr=NULL;
	IMAGE_SIZE ImageW=0, ImageH=0, ImageStep=0, BitCount = 24;		
	const size_t FrameIdx = index;
	CAOIRgn*   SubRgnPtr = NULL;
	CAOIField* FieldPtr = NULL;
	CAOIFrame* FramePtr = NULL;	
	CAOIRgn::GetRgnRoiStageRegion(StageRgn);	
	AOIDataCollect.GetCameraImageInfo(CameraID, ImageW, ImageH, ResX, ResY);	

	CString str;
	CString ErrStr, ErrName;
	size_t k=0;
	int    i=0, j=0;
	int    u=0, v=0;	
	RECT   RoiRect={0};	
	int    SrcIdx=0, DstIdx=0;			
	MASK_PTR  RoiMaskPtr = NULL;
	IMAGE_SIZE RoiW=0, RoiH=0, RoiStep=0, RoiBitCount=0;
	SPACE_PTR RoiSpacePtr = NULL;
	int    CellW=0, CellH=0, CellStep=0;	
	MASK_PTR  CellMaskPtr = NULL;
	SPACE_PTR CellSpacePtr = NULL;
	double FovCpX=0, FovCpY=0;
	int    LocalX=0, LocalY=0;
	int    ImageCpX=0, ImageCpY=0;//跟機台方向有關係
	int    ImageStartX=0, ImageStartY=0;	
	const int    nAlign = 4;
	const double ImageGain = 4.0;
	const size_t RgnIndex = GetRgnIndex();
	const size_t SubRgnCount = GetRgnSubRgnCount();
	const double StartX = StageRgn.minX;
	const double StartY = StageRgn.minY;	
	const int nFullImageW = (int)(FullImageW);
	const int nFullImageH = (int)(FullImageH);
	const int nFullImageStep = (int)(FullImageStep);
	const bool SignX = AOIDataCollect.GetStageSignPositiveX();
	const bool SignY = AOIDataCollect.GetStageSignPositiveY();

	const size_t BufferSize=ImageAPI.CalcBufferSize(FullImageStep, FullImageH);	
	::memset(FullSpacePtr, 0x00, sizeof(SPACE_DATA)*BufferSize);
	::memset(FullMaskPtr, PHASE_MASK_NOISE_ONLY, sizeof(MASK_DATA)*BufferSize);

	for ( k=0; k<SubRgnCount; k++ )
	{
		SubRgnPtr = GetRgnSubRgnPtr(k, false);
		if ( NULL == SubRgnPtr ) { continue; }
		FieldPtr = SubRgnPtr->GetRgnFieldPtr();		
		FramePtr = FieldPtr->GetFieldFramePtr(FrameIdx, true);
		ErrName.Format(_T("%s Sub[%02d]"), this->GetRgnDerivedName(), k+1);
		if ( NULL == FramePtr ) 
		{	return false;	}
		FrameType = FramePtr->GetFrameType();
		if ( FRAME_SPACE != FrameType ) 
		{	return false;	}
		FramePtr->GetFrameSpacePtr(ImageW, ImageH, ImageStep, BitCount, FrameMaskPtr, FrameSpacePtr);

		RoiRect = SubRgnPtr->GetRgnFrameImageRect();		
		RoiW=RoiRect.right-RoiRect.left;
		RoiH=RoiRect.bottom-RoiRect.top;
		RoiStep=JetAPI::GetBMPImagePixelsPerLine(RoiW, BitCount, nAlign);
		RoiBitCount=BitCount;		
		if ( ImageAPI.ExtractRoiImage(ImageW, ImageH, ImageStep, BitCount, FrameMaskPtr, RoiRect, RoiStep, RoiMaskPtr, false) == false )
		{
			ErrStr.Format(_T("Error, %s Extract Roi Mask Fault (%d, %d, %d, %d)"), ErrName, RoiRect.left, RoiRect.top, RoiRect.right, RoiRect.bottom);
			SaveRgnMovingTimeMsg(ErrStr);
			return false;	
		}
		if ( ImageAPI.ExtractSpaceRoiImage(ImageW, ImageH, ImageStep, BitCount, FrameSpacePtr, RoiRect, RoiStep, RoiSpacePtr, false) == false )
		{
			JetMemory.free_func(RoiMaskPtr);
			JetMemory.free_func(RoiSpacePtr);
			ErrStr.Format(_T("Error, %s Extract Roi Space Fault (%d, %d, %d, %d)"), ErrName, RoiRect.left, RoiRect.top, RoiRect.right, RoiRect.bottom);
			SaveRgnMovingTimeMsg(ErrStr);
			return false;	
		}

		CellW  = (int)(RoiW);
		CellH  = (int)(RoiH);
		CellMaskPtr = RoiMaskPtr;
		CellSpacePtr = RoiSpacePtr;
		CellStep = (int)(RoiStep);
		FovCpX = SubRgnPtr->GetRgnStagePosX();
		FovCpY = SubRgnPtr->GetRgnStagePosY();		
		LocalX = (int)(((FovCpX-StartX)/ResX)+0.5);
		LocalY = (int)(((FovCpY-StartY)/ResY)+0.5);
		//LocalX = JetAPI::Floor((FovCpX-StartX)/ResX);
		//LocalY = JetAPI::Floor((FovCpY-StartY)/ResY);		
		ImageCpX = FullImageW-LocalX;//跟機台方向有關係
		ImageCpY = FullImageH-LocalY;//跟機台方向有關係
		if ( true == SignX )
		{	ImageCpX = LocalX;	}
		else
		{	ImageCpX = FullImageW-LocalX;	}//JET6500
		if ( true == SignY )
		{	ImageCpY = FullImageH-LocalY;	}//JET6500
		else
		{	ImageCpY = LocalY; }
		ImageStartX = ImageCpX-(CellW/2);
		ImageStartY = ImageCpY-(CellH/2);
		
		if ( 8 == BitCount )
		{
			for ( i=0; i<CellH; i++ )
			{
				v = i+ImageStartY;
				if ( v<0 || v>=nFullImageH ) { continue; }
				SrcIdx = i*CellStep;
				v = v*FullImageStep;
				for ( j=0; j<CellW; j++ )
				{
					u = j+ImageStartX;
					if ( u<0 || u>=nFullImageW ) 
					{ 
						SrcIdx ++;
						continue; 
					}				
					DstIdx = v+u;
					FullMaskPtr[DstIdx] = CellMaskPtr[SrcIdx];
					FullSpacePtr[DstIdx] = CellSpacePtr[SrcIdx];
					DstIdx ++;
					SrcIdx ++;
				}
			}
		}
		else if ( 24 == BitCount )
		{
			for ( i=0; i<CellH; i++ )
			{
				v = i+ImageStartY;
				if ( v<0 || v>=nFullImageH ) { continue; }
				SrcIdx = i*CellStep;
				v = v*FullImageStep;
				for ( j=0; j<CellW; j++ )
				{
					u = j+ImageStartX;
					if ( u<0 || u>=nFullImageW ) 
					{ 
						SrcIdx +=3;
						continue; 
					}
					u = u*3;
					DstIdx = v+u;
					FullMaskPtr[DstIdx] = CellMaskPtr[SrcIdx];//B
					FullMaskPtr[DstIdx+1] = CellMaskPtr[SrcIdx+1];//G
					FullMaskPtr[DstIdx+2] = CellMaskPtr[SrcIdx+2];//R

					FullSpacePtr[DstIdx] = CellSpacePtr[SrcIdx];//B
					FullSpacePtr[DstIdx+1] = CellSpacePtr[SrcIdx+1];//G
					FullSpacePtr[DstIdx+2] = CellSpacePtr[SrcIdx+2];//R

					DstIdx += 3;
					SrcIdx += 3;
				}
			}		
		}
		JetMemory.free_func(RoiMaskPtr);
		JetMemory.free_func(RoiSpacePtr);		
		//SubRgnPtr->SetRgnCalculated(true);
		//SubRgnPtr->SetRgnCalcState(REGION_CALC_DONE);
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIRgn::FillRgnSubRgnFullSpace(size_t FrameIdx, const IMAGE_SIZE ImageW, const IMAGE_SIZE ImageH, const IMAGE_SIZE ImageStep, const int BitCount, const IMAGE_PTR ImagePtr)//執行區域子檢測框圖像拼接		
{
	//return FillRgnSubRgnFullSpace_I(FrameIdx);
	return FillRgnSubRgnFullSpace_II(FrameIdx, ImageW, ImageH, ImageStep, BitCount, ImagePtr);	
}
//-------------------------------------------------------------------------------------//
bool CAOIRgn::FillRgnSubRgnFullSpace_I(size_t FrameIdx)//執行區域子檢測框圖像拼接		
{
	CString str;
	CString ErrStr, ErrName;
	ErrName.Format(_T("%s_Fill"), this->GetRgnDerivedName());
	const size_t SubRgnCount = CAOIRgn::GetRgnSubRgnCount();
	if ( 0 == SubRgnCount ) 
	{ 
		ErrStr.Format(_T("Error, %s [0 == SubRgnCount] Fault"), ErrName);
		SaveRgnMovingTimeMsg(ErrStr);
		return false; 
	}
	if ( FrameIdx >= FRAME_MAX_COUNT ) { return false; }

	bool         bSave = false;
	MASK_PTR     FullMaskPtr = NULL;	
	SPACE_PTR    FullSpacePtr = NULL;
	IMAGE_SIZE   FullImageW = 0;
	IMAGE_SIZE   FullImageH = 0;
	IMAGE_SIZE   FullBitCount = 8;
	IMAGE_SIZE   FullImageStep = 0;	
	CAMERA_ID    CameraID = GetRgnCameraID();
	CString ComName = this->GetRgnDerivedName();
	const size_t RgnIndex = GetRgnIndex();
	const double SpaceRatio = AOIDataCollect.GetSpaceToGrayRatio();
	if ( AllocateRgnFullSpaceBuffer(FrameIdx, FullImageW, FullImageH, FullImageStep, FullBitCount, FullSpacePtr, FullMaskPtr) == false )
	{	
		SetRgnSubRgnAllFinish();
		ErrStr.Format(_T("Error, %s AllocateRgnFullSpaceBuffer Fault"), ErrName);
		SaveRgnMovingTimeMsg(ErrStr);
		return false; 
	}
	
	if ( FillRgnSubRgnFullSpaceBuffer(FrameIdx, FullImageW, FullImageH, FullImageStep, FullBitCount, FullSpacePtr, FullMaskPtr) == false )
	{
		JetMemory.free_func(FullMaskPtr);
		JetMemory.free_func(FullSpacePtr);
		SetRgnSubRgnAllFinish();
		ErrStr.Format(_T("Error, %s FillRgnSubRgnFullSpaceBuffer Fault"), ErrName);
		SaveRgnMovingTimeMsg(ErrStr);
		return false;
	}	
	RECT         FullRect={0};
	MASK_PTR     DstMaskPtr = NULL;
	IMAGE_PTR    DstImagePtr = NULL;
	SPACE_PTR    DstSpacePtr = NULL;
	
	IMAGE_SIZE  Mask2DW=0;
	IMAGE_SIZE  Mask2DH=0;
	IMAGE_SIZE  Mask2DStep=0;
	IMAGE_SIZE  Mask2DBitCount=0;
	MASK_PTR    Mask2DPtr=NULL;		
	bool bUseMask_Base = false;
	const bool  bUsing3D = GetRgnUsing3D();//是否使用3D資料群
	const bool  bEnableMask_Base = GetRgnMaskEnable_Base();
	const bool  bCehckSubRgnMask_Base = CheckRgnSubRgnMaskBuffer_Base();
	if ( false==bEnableMask_Base || false==bCehckSubRgnMask_Base || false==bUsing3D)
	{	bUseMask_Base = false;	}
	else
	{	bUseMask_Base = true; }

	JetAPI::SizeToRect(FullImageW, FullImageH, FullRect);
	if ( true == bUseMask_Base )
	{	
		MASK_PTR     BaseMask2DPtr=NULL;		
		IMAGE_SIZE   MaskDBitCount=8;
		IMAGE_SIZE   BaseMaskStep=JetAPI::GetBMPImagePixelsPerLine(FullImageW, MaskDBitCount, 4);
		const size_t BaseMaskBufferSize = ImageAPI.CalcBufferSize(BaseMaskStep, FullImageH);
		if ( JetMemory.alloc_func(BaseMaskBufferSize, BaseMask2DPtr, "CAOIRgn::FillRgnSubRgnFullSpace", "BaseMask2DPtr") == true ) 
		{
			if ( FillRgnSubRgnFullMaskBuffer_Base(FullImageW, FullImageH, BaseMaskStep, MaskDBitCount, BaseMask2DPtr) == true )
			{	SetRgnMaskBuffer_Base(FullImageW, FullImageH, BaseMaskStep, MaskDBitCount, BaseMask2DPtr, false);	}
			else
			{	JetMemory.free_func(BaseMask2DPtr);	}			
		}
		GetRgnMaskBuffer_Base(Mask2DW, Mask2DH, Mask2DStep, Mask2DBitCount, Mask2DPtr);
		if ( FullImageW!=Mask2DW || FullImageH!=Mask2DH || FullImageStep!=Mask2DStep )
		{	Mask2DPtr = NULL;	}
	}
	
	double     ResX=0, ResY=0;
	IMAGE_SIZE CameraW=0, CameraH=0;	
	AOIDataCollect.GetCameraImageInfo(CameraID, CameraW, CameraH, ResX, ResY);	

	TNoiseFilterParam FilterParam = GetRgnSpaceNoiseFilterParam();		
	if ( false == bUsing3D )
	{	AOIDataCollect.DisableSpaceNoiseFilterParam(FilterParam);	}
	FilterParam.BasePlaneParam.RotatedAngle = GetRgnAngle();
	FilterParam.BasePlaneParam.RotatedSizeW = GetRgnRoiSizeW()/ResX;
	FilterParam.BasePlaneParam.RotatedSizeH = GetRgnRoiSizeH()/ResY;

	const int nOpenMPCnt = GetRgnOpenMPCount();	
	CalcRgnBodyOutsideParam(FullImageW, FullImageH, FilterParam.BasePlaneParam);
	if ( ImageAPI.BuildSpaceData(FullImageW, FullImageH, FullImageStep, FullSpacePtr, FullMaskPtr, Mask2DPtr, nOpenMPCnt, FilterParam, DstSpacePtr, DstMaskPtr) == false )
	{		
		JetMemory.free_func(FullMaskPtr);
		JetMemory.free_func(FullSpacePtr);
		SetRgnSubRgnAllFinish();
		ErrStr.Format(_T("Error, %s BuildSpaceData Fault"), ErrName);
		SaveRgnMovingTimeMsg(ErrStr);
		return false;
	}
	TBasePlaneParam &BasePlaneParamRef = GetRgnSpaceBasePlaneParam();	
	BasePlaneParamRef.CopyBasePlaneGroundEquationFrom(FilterParam.BasePlaneParam);
	if ( ImageAPI.SpaceGrayImageConvertToGray(FullImageW, FullImageH, FullImageStep, FullSpacePtr, FullMaskPtr, FullRect, FullImageStep, DstImagePtr, SpaceRatio, false) == false )
	{
		JetMemory.free_func(DstMaskPtr);
		JetMemory.free_func(DstSpacePtr);
		JetMemory.free_func(FullMaskPtr);
		JetMemory.free_func(FullSpacePtr);
		SetRgnSubRgnAllFinish();
		ErrStr.Format(_T("Error, %s SpaceGrayImageConvertToGray Fault"), ErrName);
		SaveRgnMovingTimeMsg(ErrStr);
		return false;
	}	
#ifdef _DEBUG	
	if ( true == bSave )
	{
		str.Format(_T("%s\\%s_SubAll[%d#%d].JPG"), AOIDataCollect.GetAOITempDirectory(), ComName, RgnIndex+1, FrameIdx+1);
		ImageAPI.SaveImage(str, FullImageW, FullImageH, FullImageStep, FullBitCount, DstImagePtr, true);
		if ( NULL != Mask2DPtr )
		{
			str.Format(_T("%s\\%s_SubAllBaseMask[%d#%d].PNG"), AOIDataCollect.GetAOITempDirectory(), ComName, RgnIndex+1, FrameIdx+1);
			ImageAPI.SaveImage(str, Mask2DW, Mask2DH, Mask2DStep, Mask2DBitCount, Mask2DPtr, true);			
		}
	}
#else	
	//bSave = true;
	if ( true == bSave )
	{
		str.Format(_T("%s\\%s_SubAll[%d#%d].JPG"), AOIDataCollect.GetAOITempDirectory(), ComName, RgnIndex+1, FrameIdx+1);
		ImageAPI.SaveImage(str, FullImageW, FullImageH, FullImageStep, FullBitCount, DstImagePtr, true);
		if ( NULL != Mask2DPtr )
		{
			str.Format(_T("%s\\%s_SubAllBaseMask[%d#%d].PNG"), AOIDataCollect.GetAOITempDirectory(), ComName, RgnIndex+1, FrameIdx+1);
			ImageAPI.SaveImage(str, Mask2DW, Mask2DH, Mask2DStep, Mask2DBitCount, Mask2DPtr, true);			
		}
	}
#endif//_DEBUG	
	const bool bEnableRawSpace=GetRgnRawSpaceEnabled();
	if ( false == bEnableRawSpace )
	{	SetRgnImageBuffer(FrameIdx, FullImageW, FullImageH, FullImageStep, FullBitCount, NULL, DstMaskPtr, DstSpacePtr, NULL, NULL);	 }
	else
	{	SetRgnImageBuffer(FrameIdx, FullImageW, FullImageH, FullImageStep, FullBitCount, NULL, DstMaskPtr, DstSpacePtr, FullMaskPtr, FullSpacePtr);	 }
	
	//JetMemory.free_func(DstMaskPtr);
	JetMemory.free_func(DstImagePtr);
	//JetMemory.free_func(DstSpacePtr);
	if ( false == bEnableRawSpace )
	{
		JetMemory.free_func(FullMaskPtr);	
		JetMemory.free_func(FullSpacePtr);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIRgn::FillRgnSubRgnFullSpace_II(size_t FrameIdx, const IMAGE_SIZE ImageW, const IMAGE_SIZE ImageH, const IMAGE_SIZE ImageStep, const int BitCount, const IMAGE_PTR ImagePtr)//執行區域子檢測框圖像拼接		
{
	CString str;
	CString ErrStr, ErrName;
	ErrName.Format(_T("%s_Fill"), this->GetRgnDerivedName());
	const char fnName[] = "CAOIRgn::FillRgnSubRgnFullSpace_II";
	const size_t SubRgnCount = CAOIRgn::GetRgnSubRgnCount();
	if (0 == SubRgnCount) 
	{ 
		ErrStr.Format(_T("Error, %s [0 == SubRgnCount] Fault"), ErrName);
		SaveRgnMovingTimeMsg(ErrStr);
		return false; 
	}
	if (FrameIdx >= FRAME_MAX_COUNT) { return false; }

	bool         bSave = false;
	MASK_PTR     FullMaskPtr = NULL;
	SPACE_PTR    FullSpacePtr = NULL;
	IMAGE_SIZE   FullImageW = 0;
	IMAGE_SIZE   FullImageH = 0;
	IMAGE_SIZE   FullBitCount = 8;
	IMAGE_SIZE   FullImageStep = 0;
	CAMERA_ID    CameraID = GetRgnCameraID();
	CString ComName = this->GetRgnDerivedName();
	const size_t RgnIndex = GetRgnIndex();
	const double SpaceRatio = AOIDataCollect.GetSpaceToGrayRatio();
	if (AllocateRgnFullSpaceBuffer(FrameIdx, FullImageW, FullImageH, FullImageStep, FullBitCount, FullSpacePtr, FullMaskPtr) == false)
	{
		SetRgnSubRgnAllFinish();
		ErrStr.Format(_T("Error, %s AllocateRgnFullSpaceBuffer Fault"), ErrName);
		SaveRgnMovingTimeMsg(ErrStr);
		return false;
	}

	if (FillRgnSubRgnFullSpaceBuffer(FrameIdx, FullImageW, FullImageH, FullImageStep, FullBitCount, FullSpacePtr, FullMaskPtr) == false)
	{
		JetMemory.free_func(FullMaskPtr);
		JetMemory.free_func(FullSpacePtr);
		SetRgnSubRgnAllFinish();
		ErrStr.Format(_T("Error, %s FillRgnSubRgnFullSpaceBuffer Fault"), ErrName);
		SaveRgnMovingTimeMsg(ErrStr);
		return false;
	}
	RECT         FullRect = { 0 };
	MASK_PTR     DstMaskPtr = NULL;
	IMAGE_PTR    DstImagePtr = NULL;
	SPACE_PTR    DstSpacePtr = NULL;

	IMAGE_SIZE  Mask2DW = 0;
	IMAGE_SIZE  Mask2DH = 0;
	IMAGE_SIZE  Mask2DStep = 0;
	IMAGE_SIZE  Mask2DBitCount = 0;
	MASK_PTR    Mask2DPtr = NULL;
	bool bUseMask_Base = false;
	const bool  bUsing3D = GetRgnUsing3D();//是否使用3D資料群
	const bool  bEnableMask_Base = GetRgnMaskEnable_Base();
	const bool  bCehckSubRgnMask_Base = CheckRgnSubRgnMaskBuffer_Base();
	if (false == bEnableMask_Base || false == bCehckSubRgnMask_Base || false == bUsing3D)
	{	bUseMask_Base = false;	}
	else
	{	bUseMask_Base = true;	}

	JetAPI::SizeToRect(FullImageW, FullImageH, FullRect);
	if (true == bUseMask_Base)
	{
		MASK_PTR     BaseMask2DPtr = NULL;
		IMAGE_SIZE   MaskDBitCount = 8;
		IMAGE_SIZE   BaseMaskStep = JetAPI::GetBMPImagePixelsPerLine(FullImageW, MaskDBitCount, 4);
		const size_t BaseMaskBufferSize = ImageAPI.CalcBufferSize(BaseMaskStep, FullImageH);
		if (JetMemory.alloc_func(BaseMaskBufferSize, BaseMask2DPtr, fnName, "BaseMask2DPtr") == true)
		{
			if (FillRgnSubRgnFullMaskBuffer_Base(FullImageW, FullImageH, BaseMaskStep, MaskDBitCount, BaseMask2DPtr) == true)
			{	SetRgnMaskBuffer_Base(FullImageW, FullImageH, BaseMaskStep, MaskDBitCount, BaseMask2DPtr, false);	}
			else
			{	JetMemory.free_func(BaseMask2DPtr);	}
		}
		GetRgnMaskBuffer_Base(Mask2DW, Mask2DH, Mask2DStep, Mask2DBitCount, Mask2DPtr);
		if (FullImageW != Mask2DW || FullImageH != Mask2DH || FullImageStep != Mask2DStep)
		{
			Mask2DPtr = NULL;
		}
	}

	double     ResX=0, ResY=0;
	IMAGE_SIZE CameraW=0, CameraH=0;	
	AOIDataCollect.GetCameraImageInfo(CameraID, CameraW, CameraH, ResX, ResY);	

	TNoiseFilterParam FilterParam = GetRgnSpaceNoiseFilterParam();
	if (false == bUsing3D)
	{	AOIDataCollect.DisableSpaceNoiseFilterParam(FilterParam);	}
	FilterParam.BasePlaneParam.RotatedAngle = GetRgnAngle();
	FilterParam.BasePlaneParam.RotatedSizeW = GetRgnRoiSizeW()/ResX;
	FilterParam.BasePlaneParam.RotatedSizeH = GetRgnRoiSizeH()/ResY;

	IMAGE_PTR  RoiImagePtr = NULL;
	IMAGE_PTR  GuidedImagePtr = NULL;	
	const size_t GuidedSize=ImageAPI.CalcBufferSize(ImageStep, ImageH);
	if ( NULL==ImagePtr || ImageW!=FullImageW || ImageH!=FullImageH || 24!=BitCount )
	{
		//Error for ROI_Size
		bool ForDebug = false;
	}
	else
	{
		if ( JetMemory.alloc_func(GuidedSize, GuidedImagePtr, fnName, "GuidedImagePtr") == false )
		{	
			JetMemory.free_func(FullMaskPtr);
			JetMemory.free_func(FullSpacePtr);
			JetMemory.free_func(GuidedImagePtr);
			JetMemory.free_func(RoiImagePtr);
			SetRgnSubRgnAllFinish();
			return false; 
		}
		::memcpy(GuidedImagePtr, ImagePtr, sizeof(IMAGE_DATA)*GuidedSize);	
	}	
	const int nOpenMPCnt = GetRgnOpenMPCount();	
	CalcRgnBodyOutsideParam(FullImageW, FullImageH, FilterParam.BasePlaneParam);
	if (ImageAPI.BuildSpaceData(FullImageW, FullImageH, FullImageStep, FullSpacePtr, FullMaskPtr, Mask2DPtr, nOpenMPCnt, FilterParam, DstSpacePtr, DstMaskPtr, GuidedImagePtr) == false)
	{
		JetMemory.free_func(FullMaskPtr);
		JetMemory.free_func(FullSpacePtr);
		JetMemory.free_func(GuidedImagePtr);
		JetMemory.free_func(RoiImagePtr);
		SetRgnSubRgnAllFinish();
		ErrStr.Format(_T("Error, %s BuildSpaceData Fault"), ErrName);
		SaveRgnMovingTimeMsg(ErrStr);
		return false;
	}
	JetMemory.free_func(GuidedImagePtr);
	JetMemory.free_func(RoiImagePtr);
	TBasePlaneParam &BasePlaneParamRef = GetRgnSpaceBasePlaneParam();	
	BasePlaneParamRef.CopyBasePlaneGroundEquationFrom(FilterParam.BasePlaneParam);
	if (ImageAPI.SpaceGrayImageConvertToGray(FullImageW, FullImageH, FullImageStep, FullSpacePtr, FullMaskPtr, FullRect, FullImageStep, DstImagePtr, SpaceRatio, false) == false)
	{
		JetMemory.free_func(DstMaskPtr);
		JetMemory.free_func(DstSpacePtr);
		JetMemory.free_func(FullMaskPtr);
		JetMemory.free_func(FullSpacePtr);
		SetRgnSubRgnAllFinish();
		ErrStr.Format(_T("Error, %s SpaceGrayImageConvertToGray Fault"), ErrName);
		SaveRgnMovingTimeMsg(ErrStr);
		return false;
	}
#ifdef _DEBUG	
	if (true == bSave)
	{
		str.Format(_T("%s\\%s_SubAll[%d#%d].JPG"), AOIDataCollect.GetAOITempDirectory(), ComName, RgnIndex + 1, FrameIdx + 1);
		ImageAPI.SaveImage(str, FullImageW, FullImageH, FullImageStep, FullBitCount, DstImagePtr, true);
		if (NULL != Mask2DPtr)
		{
			str.Format(_T("%s\\%s_SubAllBaseMask[%d#%d].PNG"), AOIDataCollect.GetAOITempDirectory(), ComName, RgnIndex + 1, FrameIdx + 1);
			ImageAPI.SaveImage(str, Mask2DW, Mask2DH, Mask2DStep, Mask2DBitCount, Mask2DPtr, true);
		}
	}
#else	
	//bSave = true;
	if (true == bSave)
	{
		str.Format(_T("%s\\%s_SubAll[%d#%d].JPG"), AOIDataCollect.GetAOITempDirectory(), ComName, RgnIndex + 1, FrameIdx + 1);
		ImageAPI.SaveImage(str, FullImageW, FullImageH, FullImageStep, FullBitCount, DstImagePtr, true);
		if (NULL != Mask2DPtr)
		{
			str.Format(_T("%s\\%s_SubAllBaseMask[%d#%d].PNG"), AOIDataCollect.GetAOITempDirectory(), ComName, RgnIndex + 1, FrameIdx + 1);
			ImageAPI.SaveImage(str, Mask2DW, Mask2DH, Mask2DStep, Mask2DBitCount, Mask2DPtr, true);
		}
	}
#endif//_DEBUG	
	const bool bEnableRawSpace=GetRgnRawSpaceEnabled();
	if ( false == bEnableRawSpace )
	{	SetRgnImageBuffer(FrameIdx, FullImageW, FullImageH, FullImageStep, FullBitCount, NULL, DstMaskPtr, DstSpacePtr, NULL, NULL); }
	else
	{	SetRgnImageBuffer(FrameIdx, FullImageW, FullImageH, FullImageStep, FullBitCount, NULL, DstMaskPtr, DstSpacePtr, FullMaskPtr, FullSpacePtr); }

	//JetMemory.free_func(DstMaskPtr);
	JetMemory.free_func(DstImagePtr);
	//JetMemory.free_func(DstSpacePtr);
	if ( false == bEnableRawSpace )
	{
		JetMemory.free_func(FullMaskPtr);
		JetMemory.free_func(FullSpacePtr);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIRgn::ClearRgnImageBuffer()
{
	size_t i = 0;	
	for ( i=0; i<FRAME_MAX_COUNT; i++ )
	{
		if ( m_RgnMaskPtr[i] != NULL )
		{	JetMemory.free_func(m_RgnMaskPtr[i]);	}
		if ( m_RgnSpacePtr[i] != NULL )
		{	JetMemory.free_func(m_RgnSpacePtr[i]);	}
		if ( m_RgnImagePtr[i] != NULL )
		{	JetMemory.free_func(m_RgnImagePtr[i]);	}		
		if ( m_RgnRawMaskPtr[i] != NULL )
		{	JetMemory.free_func(m_RgnRawMaskPtr[i]);	}
		if ( m_RgnRawSpacePtr[i] != NULL )
		{	JetMemory.free_func(m_RgnRawSpacePtr[i]);	}

		m_RgnImageW[i] = 0;
		m_RgnImageH[i] = 0;
		m_RgnImageStep[i] = 0;
		m_RgnImageBitCount[i] = 0;		
		m_RgnImageUniqueID[i] = 0;
	}	
	return;
}
//-------------------------------------------------------------------------------------//
void CAOIRgn::ClearRgnMaskBuffer_Base()
{
	if ( NULL != m_RgnMaskPtr_Base )
	{	JetMemory.free_func(m_RgnMaskPtr_Base);		}
	m_RgnMaskW_Base = 0;
	m_RgnMaskH_Base = 0;
	m_RgnMaskStep_Base = 0;
	m_RgnMaskBitCount_Base = 0;
}
//-------------------------------------------------------------------------------------//
bool CAOIRgn::CheckRgnSubRgnMaskBuffer_Base()//確認區域內子區域的基準面遮罩指標
{
	size_t       i=0;
	IMAGE_PTR    MaskPtr=NULL;
	IMAGE_SIZE   MaskW=0, MaskH=0, MaskStep=0, MaskBitCount=0;
	CAOIRgn     *SubRgnPtr = NULL;
	const size_t SubRgnCount = this->GetRgnSubRgnCount();
	for ( i=0; i<SubRgnCount; i++ )
	{
		SubRgnPtr = this->GetRgnSubRgnPtr(i, false);
		if ( NULL == SubRgnPtr ) { continue; }
		SubRgnPtr->GetRgnMaskBuffer_Base(MaskW, MaskH, MaskStep, MaskBitCount, MaskPtr);
		if ( NULL == MaskPtr ) 
		{	return false; }
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIRgn::ClearRgnSubRgnMaskBuffer_Base()//清除子區域的基準面遮罩指標
{
	size_t       i=0;
	IMAGE_PTR    MaskPtr=NULL;
	IMAGE_SIZE   MaskW=0, MaskH=0, MaskStep=0, MaskBitCount=0;
	CAOIRgn     *SubRgnPtr = NULL;
	const size_t SubRgnCount = this->GetRgnSubRgnCount();
	for ( i=0; i<SubRgnCount; i++ )
	{
		SubRgnPtr = this->GetRgnSubRgnPtr(i, false);
		if ( NULL == SubRgnPtr ) { continue; }
		SubRgnPtr->ClearRgnMaskBuffer_Base();
	}
	return ;
}
//-------------------------------------------------------------------------------------//
bool CAOIRgn::GetRgnUniFrameList(std::vector<TUNI_FRAME> &UniFrameList)
{	
	char vaName[32]="";
	const char fnName[] = "CAOIRgn::GetRgnUniFrameList";
	size_t i=0;
	RECT   RoiRect={0,0,0,0};
	IMAGE_SIZE RoiStep=0;
	TUNI_FRAME UniFrame;
	UniFrameList.clear();
	for ( i=0; i<FRAME_MAX_COUNT; i++ )
	{
		if ( 0==m_RgnImageW[i] || 0==m_RgnImageH[i] || 0==m_RgnImageStep[i] ) { break; }

		if ( NULL!=m_RgnSpacePtr[i] && NULL!=m_RgnMaskPtr[i] &&NULL==m_RgnImagePtr[i])
		{	
			/*
			::sprintf(vaName, "m_RgnImagePtr[%d]", i+1);
			const size_t BufferSize = m_RgnImageStep[i]*m_RgnImageW[i];
			if ( JetMemory.alloc_func(BufferSize, m_RgnImagePtr[i], fnName, vaName) == true )
			{
				RoiRect.left = 0;
				RoiRect.top = 0;
				RoiRect.right = m_RgnImageW[i];
				RoiRect.bottom = m_RgnImageH[i];
				RoiStep = JetAPI::GetBMPImagePixelsPerLine(m_RgnImageW[i], 8, 4);
				ImageAPI.SpaceGrayImageConvertToGray3(m_RgnImageW[i], m_RgnImageH[i], m_RgnImageStep[i], m_RgnSpacePtr[i], m_RgnMaskPtr[i], RoiRect, RoiStep, m_RgnImagePtr[i], SpaceRatio, false);
			}
			*/
		}

		UniFrame.ImageW    = m_RgnImageW[i];
		UniFrame.ImageH    = m_RgnImageH[i];
		UniFrame.ImageStep = m_RgnImageStep[i];
		UniFrame.BitCount  = m_RgnImageBitCount[i];

		UniFrame.MaskPtr   = m_RgnMaskPtr[i];
		UniFrame.ImagePtr  = m_RgnImagePtr[i];
		UniFrame.SpacePtr  = m_RgnSpacePtr[i];
		UniFrame.RawMaskPtr= m_RgnRawMaskPtr[i];
		UniFrame.RawSpacePtr=m_RgnRawSpacePtr[i];
		
		UniFrame.FrameUniqueID=m_RgnImageUniqueID[i];

		UniFrame.PhasePtr  = NULL;
		
		UniFrameList.push_back(UniFrame);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIRgn::SetRgnUniFrame(size_t idx, TUNI_FRAME UniFrame)
{
	if ( idx >= FRAME_MAX_COUNT ) { return false; }
	m_RgnImageW[idx] = UniFrame.ImageW;
	m_RgnImageH[idx] = UniFrame.ImageH;
	m_RgnImageStep[idx] = UniFrame.ImageStep;
	m_RgnImageBitCount[idx] = UniFrame.BitCount;
	m_RgnMaskPtr[idx] = UniFrame.MaskPtr;//區域的空間遮罩
	m_RgnImagePtr[idx] = UniFrame.ImagePtr;//區域的影像記憶體區塊
	m_RgnSpacePtr[idx] = UniFrame.SpacePtr;//區域的空間記憶體區塊
	m_RgnRawMaskPtr[idx] = UniFrame.RawMaskPtr;//區域的原始空間遮罩
	m_RgnRawSpacePtr[idx] = UniFrame.RawSpacePtr;//區域的原始空間記憶體區塊	
	m_RgnImageUniqueID[idx] = UniFrame.FrameUniqueID;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIRgn::SetRgnImageUniqueID(size_t idx, unsigned int UniqueID)
{
	if ( idx >= FRAME_MAX_COUNT ) { return false; }
	m_RgnImageUniqueID[idx] = UniqueID;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIRgn::GetRgnImageUniqueID(size_t idx, unsigned int &UniqueID)
{
	if ( idx >= FRAME_MAX_COUNT ) { return false; }
	UniqueID = m_RgnImageUniqueID[idx];
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIRgn::SetRgnImageBuffer(size_t idx, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, MASK_PTR MaskPtr, SPACE_PTR SpacePtr, MASK_PTR RawMaskPtr, SPACE_PTR RawSpacePtr)
{
	if ( idx >= FRAME_MAX_COUNT ) { return false; }
	m_RgnImageW[idx] = ImageW;
	m_RgnImageH[idx] = ImageH;
	m_RgnImageStep[idx] = ImageStep;
	m_RgnImageBitCount[idx] = BitCount;
	m_RgnMaskPtr[idx] = MaskPtr;//區域的空間遮罩
	m_RgnImagePtr[idx] = ImagePtr;//區域的影像記憶體區塊
	m_RgnSpacePtr[idx] = SpacePtr;//區域的空間記憶體區塊
	m_RgnRawMaskPtr[idx] = RawMaskPtr;//區域的原始空間遮罩
	m_RgnRawSpacePtr[idx] = RawSpacePtr;//區域的原始空間記憶體區塊	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIRgn::GetRgnImageBuffer(size_t idx, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, IMAGE_PTR &ImagePtr, MASK_PTR &MaskPtr, SPACE_PTR &SpacePtr, MASK_PTR &RawMaskPtr, SPACE_PTR &RawSpacePtr)
{
	if ( idx >= FRAME_MAX_COUNT ) { return false; }
	ImageW    = m_RgnImageW[idx];
	ImageH    = m_RgnImageH[idx];
	ImageStep = m_RgnImageStep[idx];
	BitCount  = m_RgnImageBitCount[idx];
	MaskPtr   = m_RgnMaskPtr[idx];//區域的空間遮罩
	ImagePtr  = m_RgnImagePtr[idx];//區域的影像記憶體區塊
	SpacePtr  = m_RgnSpacePtr[idx];//區域的空間記憶體區塊
	RawMaskPtr= m_RgnRawMaskPtr[idx];//區域的原始空間遮罩
	RawSpacePtr=m_RgnRawSpacePtr[idx];//區域的原始空間記憶體區塊	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIRgn::ExecRgnDerivedInspection()//執行區域檢測
{
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIRgn::ExecRgnSaveDefectImage(const std::vector<TUNI_FRAME> &UniFrameList)//執行區域儲存瑕疵圖片
{
	size_t     i=0;
	size_t     j=0;
	CString    str;		
	CString    DerivedName;
	TUNI_FRAME UniFrame;
	TUNI_FRAME UniFrameTmp;
	CAOIField   *FieldPtr = GetRgnFieldPtr();	
	CAOIProject *ProjectPtr = GetRgnProjectPtr();
	if ( NULL==FieldPtr || NULL==ProjectPtr ) { return false; }
	const size_t UniFrameCount = UniFrameList.size();
	const char fnName[] = "CAOIRgn::ExecRgnSaveDefectImage";

	bool      CheckModelSaveImage = false;	
	bool      CheckFieldSaveImage_Tuning = false;	
	bool      CheckFieldSaveImage_Offline = false;	
	RESULT_ID ResultID = GetRgnResultID_AOI();
	TASK_MODE TaskMode = ProjectPtr->GetProjectActTaskMode();//TASK_TUNING_PROJECT, TASK_INSPECT_PROJECT
	const bool SaveDefectImage = AOIDataCollect.GetSaveDefectImage();		
	const bool OnlineTuningEnable = ProjectPtr->CheckProjectOnlineTuningRunning();
	const double SpaceRatio = ProjectPtr->GetProjectSpaceToGrayRatio();		
	CString SpcImageFolder = ProjectPtr->GetProjectSpcImageFolder();		
	SAVE_TEST_IMAGE_MODE  SaveModelImageMode = GetRgnSaveTestImageMode();
	SAVE_TEST_IMAGE_MODE  OnlineTuningMode = ProjectPtr->GetProjectOnlineTuningMode();
	SAVE_TEST_IMAGE_MODE  SaveFieldImageMode = ProjectPtr->GetProjectSaveFieldImageMode();		
	
	if ( ProjectPtr->GetProjectUseLocalFolder() )
	{	SpcImageFolder = ProjectPtr->GetProjectSpcImageFolderLocal();	}	

	if ( false == SaveDefectImage )
	{	CheckModelSaveImage = false;	}	
	else 
	{	CheckModelSaveImage = CheckRgnNeedSaveImage(TaskMode, SaveModelImageMode, ResultID);	}

	//if (true == AOIDataCollect.GetHASI_Enable()) { CheckModelSaveImage = true; }

	RECT           RoiRect={0,0,0,0};			
	IMAGE_SIZE     RoiStep = 0;
	IMAGE_SIZE     BufferBitCount = 24;	
	DerivedName = GetRgnDerivedName();
	if ( true == CheckModelSaveImage )
	{
		const bool bAppend=false;
		const bool bEnhance=true;
		const bool bSave3D=true;
		const double SpaceRatio = ProjectPtr->GetProjectSpaceToGrayRatio();
		str.Format(_T("%s\\%s.%s"), SpcImageFolder, DerivedName, _T("JPG"));
		if ( ImageAPI.SaveUniFrameImage(str, UniFrameList, true, bEnhance, bSave3D, bAppend, SpaceRatio) == false )
		{	return false;	}		
		SetRgnModelImageIsSaved(true);
	}	

	//儲存區域圖片
	if ( false == OnlineTuningEnable )//開起在線調機
	{	CheckFieldSaveImage_Tuning = false; }
	else
	{	CheckFieldSaveImage_Tuning = CheckRgnNeedSaveImage(TaskMode, OnlineTuningMode, ResultID);	}

	if ( false == SaveDefectImage )//關閉儲存圖像檔案
	{	CheckFieldSaveImage_Offline = false;	}
	else
	{	CheckFieldSaveImage_Offline = CheckRgnNeedSaveImage(TaskMode, SaveFieldImageMode, ResultID);	}

	if ( true==CheckFieldSaveImage_Tuning || true==CheckFieldSaveImage_Offline )	
	{	
		const bool bEnableRawSpace = GetRgnRawSpaceEnabled();	
		const bool bSaveFiledUsingThread=ProjectPtr->GetProjectSaveFiledUsingThread();
		if ( true == bEnableRawSpace )
		{	
			CString OfflineFolder=ProjectPtr->GetProjectInspectionOfflineFolder();
			if ( false == bSaveFiledUsingThread )
			{
				str.Format(_T("%s\\%s.%s"), OfflineFolder, DerivedName, _T("PNG"));
				if ( ImageAPI.SaveUniFrameImage_Offline(str, UniFrameList, true) == false )
				{	return false;	}
			}
			else
			{
				CAOIField *FieldPtr=NULL;
				if ( CreateRgnLocalRgnField(UniFrameList, FieldPtr) == false )
				{	return false; }
				if ( NULL != FieldPtr )
				{
					ProjectPtr->AddProjectInspectionPartFieldPtr(FieldPtr, true);
					ProjectPtr->AddProjectSaveField(FieldPtr);
				}
			}				
		}
		else
		{
			if ( true == bSaveFiledUsingThread )
			{		
				CAOIRgn     *SubRgnPtr=NULL;
				CAOIField   *SubFieldPtr = NULL;
				const size_t SubRgnCount = GetRgnSubRgnCount();
				ProjectPtr->AddProjectSaveField(FieldPtr);
				for ( j=0; j<SubRgnCount; j++ )
				{
					SubRgnPtr = GetRgnSubRgnPtr(j, false);
					if ( NULL == SubRgnPtr ) { continue; }
					SubFieldPtr = SubRgnPtr->GetRgnFieldPtr();
					if ( NULL == SubFieldPtr ) { continue; }
					ProjectPtr->AddProjectSaveField(SubFieldPtr);
				}
			}
			else		
			{
				CAOIFrame   *FramePtr = NULL;
				const size_t FrameCount = FieldPtr->GetFieldFramePtrCount();
				for ( i=0; i<FrameCount; i++ )
				{
					FramePtr = FieldPtr->GetFieldFramePtr(i, false);
					if ( NULL == FramePtr ) { continue; }
					FramePtr->ExecFrameSave_Thread();				
				}

				size_t       SubFrameCount=0;
				CAOIRgn     *SubRgnPtr=NULL;
				CAOIField   *SubFieldPtr = NULL;
				CAOIFrame   *SubFramePtr = NULL;
				const size_t SubRgnCount = GetRgnSubRgnCount();
				for ( j=0; j<SubRgnCount; j++ )
				{
					SubRgnPtr = GetRgnSubRgnPtr(j, false);
					if ( NULL == SubRgnPtr ) { continue; }
					SubFieldPtr = SubRgnPtr->GetRgnFieldPtr();
					if ( NULL == SubFieldPtr ) { continue; }
					SubFrameCount = SubFieldPtr->GetFieldFramePtrCount();			
					for ( i=0; i<SubFrameCount; i++ )
					{
						SubFramePtr = SubFieldPtr->GetFieldFramePtr(i, false);
						if ( NULL == SubFramePtr ) { continue; }
						SubFramePtr->ExecFrameSave_Thread();					
					}
				}
			}
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIRgn::ExecRgnSaveDefectImage_AI(const std::vector<TUNI_FRAME> &UniFrameList)//執行區域儲存瑕疵圖片
{
	size_t     i=0;
	size_t     j=0;
	CString    str;			
	TUNI_FRAME UniFrame, UniFrame_AI;
	CAOIField   *FieldPtr = GetRgnFieldPtr();	
	CAOIProject *ProjectPtr = GetRgnProjectPtr();
	if ( NULL==FieldPtr || NULL==ProjectPtr ) { return false; }
	const size_t UniFrameCount = UniFrameList.size();
	const char fnName[] = "CAOIRgn::ExecRgnSaveDefectImage_AI";
	
	bool      CheckModelSaveImage_AI = false;		
	RESULT_ID ResultID = GetRgnResultID_AOI();	
	TASK_MODE TaskMode = ProjectPtr->GetProjectActTaskMode();//TASK_TUNING_PROJECT, TASK_INSPECT_PROJECT
	const bool SaveDefectImage = AOIDataCollect.GetSaveDefectImage();	
	if ( false == SaveDefectImage )
	{	return true; }

	SAVE_TEST_IMAGE_MODE  SaveModelImageMode_AI = ProjectPtr->GetProjectSaveModelImageMode_AI();	
	CheckModelSaveImage_AI = CheckRgnNeedSaveImage(TaskMode, SaveModelImageMode_AI, ResultID);
	if ( false == CheckModelSaveImage_AI )
	{	return true; }
	
	std::vector<bool> SaveOnOffList;
	const int SaveModelImageOnOff_AI = ProjectPtr->GetProjectSaveModelImageOnOff_AI();
	if ( 0 == SaveModelImageOnOff_AI )
	{	return true; }		
	CString AIImageFolder = ProjectPtr->GetProjectAIImageFolder();		
	ProjectPtr->DecodeProjectSaveModelImageOnOff_AI(SaveModelImageOnOff_AI, UniFrameCount, SaveOnOffList);	

	CString        ImageName;	
	size_t         ImageSize = 0;		
	size_t         BufferSize = 0;		
	size_t         ImageSize_AI = 0;
	size_t         BufferSize_AI = 0;	
	const bool     bEnhance = true;
	unsigned char *ImagePtr = NULL;	
	unsigned char *BufferPtr_AI = NULL;	
	unsigned char *UniFrame3DPtr = NULL;
	IMAGE_SIZE     BufferBitCount = 24;		
	CString        DeriveName=GetRgnDerivedName();		
	const RECT    &RoiRect=GetRgnModelImageRect_AI();
	const IMAGE_SIZE RoiW = RoiRect.right-RoiRect.left;
	const IMAGE_SIZE RoiH = RoiRect.bottom-RoiRect.top;
	const double SpaceRatio = ProjectPtr->GetProjectSpaceToGrayRatio();
	const bool bSave3D = ProjectPtr->GetProjectSaveModelImage3DFile_AI();
	

	for ( i=0; i<UniFrameCount; i++ )
	{
		if ( i < SaveOnOffList.size() )
		{
			if ( false == SaveOnOffList[i] )
			{	continue; }
		}

		UniFrame = UniFrameList[i];
		ImageSize = ImageAPI.CalcBufferSize(UniFrame.ImageStep, UniFrame.ImageH);
		if ( NULL == UniFrame.ImagePtr )
		{ 
			if ( NULL == UniFrame.MaskPtr || NULL==UniFrame.SpacePtr )
			{	continue;	}			
			if ( JetMemory.alloc_func(ImageSize, UniFrame3DPtr, fnName, "UniFrame3DPtr") == false )
			{	
				JetMemory.free_func(BufferPtr_AI);
				return false;	
			}			
			RECT FullRect;
			IMAGE_SIZE W = UniFrame.ImageW;
			IMAGE_SIZE H = UniFrame.ImageH;
			IMAGE_SIZE Step = UniFrame.ImageStep;			
			JetAPI::SizeToRect(W, H, FullRect);
			if ( ImageAPI.SpaceGrayImageConvertToGray3(W, H, Step, UniFrame.SpacePtr, UniFrame.MaskPtr, FullRect, Step, UniFrame3DPtr, SpaceRatio, false) == false )
			{				
				JetMemory.free_func(BufferPtr_AI);
				JetMemory.free_func(UniFrame3DPtr);
				return false;
			}
		}
		if ( ImageSize > BufferSize )
		{
			JetMemory.free_func(ImagePtr);
			BufferSize = ImageAPI.CalcBufferSize(JetAPI::GetBMPImagePixelsPerLine(UniFrame.ImageW, BufferBitCount, 4), UniFrame.ImageH);
			if ( JetMemory.alloc_func(BufferSize, ImagePtr, fnName, "ImagePtr") == false )
			{				
				JetMemory.free_func(BufferPtr_AI);
				JetMemory.free_func(UniFrame3DPtr);
				return false;	
			}
		}

		bool bSucc = true;
		IMAGE_SIZE ImageW=UniFrame.ImageW;
		IMAGE_SIZE ImageH=UniFrame.ImageH;
		IMAGE_SIZE BitCount=UniFrame.BitCount;
		IMAGE_SIZE ImageStep=UniFrame.ImageStep;
		IMAGE_SIZE RoiStep = JetAPI::GetBMPImagePixelsPerLine(RoiW, BitCount, 4);
		if ( NULL == UniFrame.ImagePtr )
		{	bSucc = ImageAPI.ExtractRoiImage3(ImageW, ImageH, ImageStep, BitCount, UniFrame3DPtr, RoiRect, RoiStep, ImagePtr, false);	}
		else
		{	bSucc = ImageAPI.ExtractRoiImage3(ImageW, ImageH, ImageStep, BitCount, UniFrame.ImagePtr, RoiRect, RoiStep, ImagePtr, false);	}
		JetMemory.free_func(UniFrame3DPtr);
		if ( false == bSucc )
		{
			JetMemory.free_func(ImagePtr);			
			JetMemory.free_func(BufferPtr_AI);	
			return false;
		}
		UniFrame_AI = UniFrame;
		UniFrame_AI.ImageW = RoiW;
		UniFrame_AI.ImageH = RoiH;
		UniFrame_AI.ImageStep = RoiStep;
		UniFrame_AI.ImagePtr = ImagePtr;

		ImageSize_AI = UniFrame_AI.ImageStep*UniFrame_AI.ImageH;
		if ( ImageSize_AI > BufferSize_AI )
		{
			JetMemory.free_func(BufferPtr_AI);
			BufferSize_AI = ImageAPI.CalcBufferSize(JetAPI::GetBMPImagePixelsPerLine(UniFrame_AI.ImageW, BufferBitCount, 4), UniFrame_AI.ImageH);
			if ( JetMemory.alloc_func(BufferSize_AI, BufferPtr_AI, fnName, "BufferPtr_AI") == false )
			{				
				JetMemory.free_func(ImagePtr);				
				return false;	
			}
		}

		unsigned char *ImagePtr_AI = NULL;
		if ( true == bEnhance )
		{
			ImagePtr_AI = BufferPtr_AI;
			AOIDataCollect.ExecEnhanceDisplayImage(UniFrame_AI, BufferPtr_AI);
		}
		else
		{	ImagePtr_AI = UniFrame_AI.ImagePtr;	}		
		ImageName = AOIDataDefine.GetAITempName(DeriveName, i);
		str.Format(_T("%s\\%s.%s"), AIImageFolder, ImageName, _T("JPG"));
		if ( ImageAPI.SaveImage(str, UniFrame_AI, ImagePtr_AI, true) == false )
		{
			JetMemory.free_func(ImagePtr);			
			JetMemory.free_func(BufferPtr_AI);			
			return false;
		}

		if ( NULL == UniFrame.MaskPtr || NULL==UniFrame.SpacePtr )
		{	continue;	}
		if ( false == bSave3D )
		{	continue; }

		SPACE_PTR SpacePtr = NULL;
		if ( JetMemory.alloc_func(ImageSize, SpacePtr, fnName, "SpacePtr") == false )
		{
			JetMemory.free_func(ImagePtr);			
			JetMemory.free_func(BufferPtr_AI);							
			return false;
		}
		ImageName = AOIDataDefine.GetAITempName(DeriveName, i);
		str.Format(_T("%s\\%s.%s"), AIImageFolder, ImageName, EXT_NAME_SPACE);
		if ( ImageAPI.ExtractSpaceRoiImage3(ImageW, ImageH, ImageStep, BitCount, UniFrame.SpacePtr, RoiRect, RoiStep, SpacePtr, false) == false )
		{
			JetMemory.free_func(ImagePtr);
			JetMemory.free_func(SpacePtr);			
			JetMemory.free_func(BufferPtr_AI);										
			return false;
		}
		if ( ImageAPI.SaveSpaceGrayImage(str, ImageW, ImageH, ImageStep, SpacePtr, true) == false )
		{
			JetMemory.free_func(ImagePtr);
			JetMemory.free_func(SpacePtr);			
			JetMemory.free_func(BufferPtr_AI);										
			return false;
		}
		JetMemory.free_func(SpacePtr);
	}	
	JetMemory.free_func(ImagePtr);	
	JetMemory.free_func(BufferPtr_AI);	
	SetRgnModelImageIsSaved_AI(true);
	return true;
}
//-------------------------------------------------------------6------------------------//
void CAOIRgn::UpdateRgnParentCalcStateDone()//更新父區域計算完畢
{
	CAOIRgn *ParentPtr = CAOIRgn::GetRgnParent();
	if ( NULL == ParentPtr ) { return; }

	size_t       i=0;	
	CAOIRgn     *SubRgnPtr = NULL;
	REGION_CALC_STATE RgnCalcState;
	const size_t SubRgnCount = ParentPtr->GetRgnSubRgnCount();
	RgnCalcState = ParentPtr->GetRgnCalcState();
	//if ( REGION_CALC_DONE != RgnCalcState )
	//{	return; }
	for ( i=0; i<SubRgnCount; i++ )
	{
		SubRgnPtr = ParentPtr->GetRgnSubRgnPtr(i, false);
		if ( NULL == SubRgnPtr ) { continue; }
		RgnCalcState = SubRgnPtr->GetRgnCalcState();
		if ( REGION_CALC_DONE != RgnCalcState )
		{	return; }
	}	
	ParentPtr->SetRgnCalcState(REGION_CALC_DONE);
	return;
}
//-------------------------------------------------------------------------------------//
bool CAOIRgn::ExecRgnCalc_ProjectMark()//執行區域的專案標記
{
	const char fnName[] = "CAOIRgn::ExecRgnCalc_ProjectMark";
	CAOIField* FieldPtr = CAOIRgn::GetRgnFieldPtr();
	if ( NULL == FieldPtr ) { return false; }
	CAOIProject *Project = FieldPtr->GetFieldProjectPtr();
	if ( NULL == Project ) { return false; }
	CAOIFrame *FramePtr = NULL;	
	size_t i = 0;
	size_t FrameIdx = 0;
	unsigned int ImageIndex = 0;
	FRAME_TYPE   FrameType = FRAME_NULL;
	const size_t FieldIndex = FieldPtr->GetFieldIndex();

	const int      nAlign = 4;
	bool           bAllocated = false;
	TPOINT2D       StagePos;
	RECT           RoiRect={0};
	IMAGE_SIZE     ImageW=0;
	IMAGE_SIZE     ImageH=0;
	IMAGE_SIZE     ImageStep=0;
	IMAGE_SIZE     BitCount=0;
	IMAGE_SIZE     BufferSize=0;
	IMAGE_PTR      ImagePtr = NULL;
	MASK_PTR       MaskPtr = NULL;
	SPACE_PTR      SpacePtr = NULL;
	const double   SpaceRatio = Project->GetProjectSpaceToGrayRatioMode();
	const unsigned int MarkFrameIndex = Project->GetProjectMarkFrameIndex_System();

	StagePos.x = FieldPtr->GetFieldStagePosX();
	StagePos.y = FieldPtr->GetFieldStagePosY();	
	const size_t FrameCount = FieldPtr->GetFieldFramePtrCount();
	for ( i=0; i<FrameCount; i++ )
	{
		FrameIdx = i;
		FramePtr = FieldPtr->GetFieldFramePtr(FrameIdx, true);
		if ( NULL == FramePtr ) { return false; }
		FrameType = FramePtr->GetFrameType();
		ImageIndex = FramePtr->GetFrameImageIndex();
		if ( -1 == ImageIndex ) { return false; }
		if ( MarkFrameIndex != i ) 
		{
			FramePtr->ClearFrameBuffer();	
			continue;
		}
		bAllocated = false;
		if ( FRAME_SPACE == FrameType )
		{
			FramePtr->GetFrameSpacePtr(ImageW, ImageH, ImageStep, BitCount, MaskPtr, SpacePtr);
			if ( NULL==MaskPtr || NULL==SpacePtr ) 
			{	return false; }

			RoiRect.left   = 0;
			RoiRect.top    = 0;
			RoiRect.right  = (int)(ImageW);
			RoiRect.bottom = (int)(ImageH);
			BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
			if ( JetMemory.alloc_func(BufferSize, ImagePtr, "CAOIRgn::ExecRgnCalc_ProjectMap", "ImagePtr") == false )
			{	return false;	}
			if ( ImageAPI.SpaceGrayImageConvertToGray3(ImageW, ImageH, ImageStep, SpacePtr, MaskPtr, RoiRect, ImageStep, ImagePtr, SpaceRatio, false) == false )
			{	return false; }
			bAllocated = true;
		}
		else if ( FRAME_BAYER == FrameType )
		{			
			FramePtr->GetFrameImagePtr(ImageW, ImageH, ImageStep, BitCount, ImagePtr);	
			if ( NULL == ImagePtr ) 
			{	return false; }					
			IMAGE_SIZE   DeBayerBit=0;
			IMAGE_SIZE   DeBayerStep=0;
			IMAGE_PTR    DeBayerPtr=NULL;
			BAYER_PATTERN_MODE BayerPattern = FramePtr->GetFrameBayerPattern();	
			if ( AOIDataCollect.ExecDebayerImage(fnName, ImageW, ImageH, ImageStep, ImagePtr, BayerPattern, DeBayerStep, DeBayerBit, DeBayerPtr) == false)		
			{	return false;	}
			ImagePtr = DeBayerPtr;
			BitCount = DeBayerBit;			
			ImageStep = DeBayerStep;
			DeBayerPtr = NULL;
			bAllocated = true;
		}
		else
		{	
			FramePtr->GetFrameImagePtr(ImageW, ImageH, ImageStep, BitCount, ImagePtr);	
			if ( NULL == ImagePtr ) 
			{	return false; }
		}
		Project->InspectProjectMark(StagePos, ImageW, ImageH, ImageStep, BitCount, ImagePtr);
	//#ifndef _X64
		FramePtr->ClearFrameBuffer();	
	//#endif//_X64
		if ( true == bAllocated )
		{	JetMemory.free_func(ImagePtr);	}
		
		ImageW = 0;
		ImageH = 0;
		BitCount = 0;
		ImageStep = 0;
		MaskPtr = NULL;
		SpacePtr = NULL;
		ImagePtr = NULL;		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIRgn::SetRgnPanelBasePlane(double val)
{
	m_RgnPanelBasePlane = val;
	m_RgnSpaceNoiseFilterParam.BasePlaneParam.PanelBasePlane = val;
}
//-------------------------------------------------------------------------------------//
bool CAOIRgn::CheckRgnLocalBasePlaneUsed() const
{
	if ( CALC_BASE_PLANE_LOCAL != m_RgnSpaceNoiseFilterParam.BasePlaneParam.CalcBasePlaneMode )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIRgn::SetRgnLocalBasePlaneFinish(bool val)
{	
	m_RgnLocalBasePlaneFinish=val;

	size_t       i=0;
	CAOIRgn     *SubRgnPtr = NULL;
	const size_t SubRgnCount = GetRgnSubRgnCount();
	for ( i=0; i<SubRgnCount; i++ )
	{
		SubRgnPtr = GetRgnSubRgnPtr(i, false);
		if ( NULL == SubRgnPtr ) { continue; }
		SubRgnPtr->SetRgnLocalBasePlaneFinish(val);
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CAOIRgn::SetRgnLocalBasePlaneParam(const TPOINT3D &val)
{
	m_RgnLocalBasePlaneParam = val;
}
//-------------------------------------------------------------------------------------//
void CAOIRgn::GetRgnLocalBasePlaneParam(TPOINT3D &val) const
{
	val = m_RgnLocalBasePlaneParam;
}
//-------------------------------------------------------------------------------------//
void CAOIRgn::SetRgnSpaceBasePlaneParam(const TBasePlaneParam& Param)
{	
	m_RgnSpaceNoiseFilterParam.BasePlaneParam = Param;
}
//-------------------------------------------------------------------------------------//
void CAOIRgn::SetRgnSpaceNoiseFilterParam(const TNoiseFilterParam& Param)
{
	TBasePlaneParam LevelBaseParam = m_RgnSpaceNoiseFilterParam.BasePlaneParam;
	m_RgnSpaceNoiseFilterParam = Param;
	m_RgnSpaceNoiseFilterParam.BasePlaneParam = LevelBaseParam;
}
//-------------------------------------------------------------------------------------//
bool CAOIRgn::SetRgnMaskBuffer_Base(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, MASK_PTR MaskPtr, bool bClone)
{
	ClearRgnMaskBuffer_Base();
	MASK_PTR TempPtr = NULL;
	if ( true == bClone )
	{
		const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
		if ( JetMemory.alloc_func(BufferSize, TempPtr, "CAOIRgn::SetRgnMaskBuffer_Base", "TempPtr") == false )
		{	return false; }
		::memcpy(TempPtr, MaskPtr, sizeof(MASK_DATA)*BufferSize);
	}
	else
	{	TempPtr = MaskPtr;	}

	m_RgnMaskW_Base = ImageW;
	m_RgnMaskH_Base = ImageH;
	m_RgnMaskStep_Base = ImageStep;
	m_RgnMaskBitCount_Base = BitCount;
	m_RgnMaskPtr_Base = TempPtr;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIRgn::GetRgnMaskBuffer_Base(IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, MASK_PTR &MaskPtr)
{
	ImageW = m_RgnMaskW_Base;
	ImageH = m_RgnMaskH_Base;
	ImageStep = m_RgnMaskStep_Base;
	BitCount = m_RgnMaskBitCount_Base;
	MaskPtr = m_RgnMaskPtr_Base;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIRgn::FillRgnSubRgnFullMaskBuffer_Base(IMAGE_SIZE FullImageW, IMAGE_SIZE FullImageH, IMAGE_SIZE FullImageStep, IMAGE_SIZE FullBitCount, MASK_PTR FullMaskPtr)//拼接子檢測框基準面遮罩全圖	
{	
	TREGION4D StageRgn;
	FRAME_TYPE FrameType = FRAME_NULL;
	double ResX=0, ResY=0;
	CAMERA_ID  CameraID = GetRgnCameraID();
	IMAGE_PTR  ImagePtr=NULL;
	CAOIRgn*   SubRgnPtr = NULL;
	IMAGE_SIZE ImageW=0, ImageH=0, ImageStep=0, BitCount = 8;	

	GetRgnRoiStageRegion(StageRgn);	
	AOIDataCollect.GetCameraImageInfo(CameraID, ImageW, ImageH, ResX, ResY);	

	CString str;
	size_t k=0;
	int    i=0, j=0;
	int    u=0, v=0;		
	int    SrcIdx=0, DstIdx=0;			
	IMAGE_PTR RoiPtr = NULL;
	IMAGE_SIZE RoiW=0, RoiH=0, RoiStep=0, RoiBitCount=0;
	int    CellW=0, CellH=0, CellStep=0;	
	IMAGE_PTR  CellPtr = NULL;
	double FovCpX=0, FovCpY=0;
	int    LocalX=0, LocalY=0;
	int    ImageCpX=0, ImageCpY=0;//跟機台方向有關係
	int    ImageStartX=0, ImageStartY=0;	
	const double ImageGain = 4.0;
	const size_t RgnIndex = GetRgnIndex();
	const size_t SubRgnCount = GetRgnSubRgnCount();
	const double StartX = StageRgn.minX;
	const double StartY = StageRgn.minY;	
	const int nFullImageW = (int)(FullImageW);
	const int nFullImageH = (int)(FullImageH);
	const int nFullImageStep = (int)(FullImageStep);	
	const bool SignX = AOIDataCollect.GetStageSignPositiveX();
	const bool SignY = AOIDataCollect.GetStageSignPositiveY();
	for ( k=0; k<SubRgnCount; k++ )
	{
		SubRgnPtr = GetRgnSubRgnPtr(k, false);
		if ( NULL == SubRgnPtr ) { continue; }		
		if ( SubRgnPtr->GetRgnMaskBuffer_Base(RoiW, RoiH, RoiStep, BitCount, RoiPtr) == false )
		{
			RoiPtr = NULL;
			SubRgnPtr->ClearRgnMaskBuffer_Base();
			return false;
		}
		if ( NULL == RoiPtr )
		{
			SubRgnPtr->ClearRgnMaskBuffer_Base();
			return false;
		}

		CellW  = (int)(RoiW);
		CellH  = (int)(RoiH);
		CellPtr = RoiPtr;
		CellStep = (int)(RoiStep);
		FovCpX = SubRgnPtr->GetRgnStagePosX();
		FovCpY = SubRgnPtr->GetRgnStagePosY();		
		LocalX = JetAPI::Floor((FovCpX-StartX)/ResX);
		LocalY = JetAPI::Floor((FovCpY-StartY)/ResY);
		ImageCpX = FullImageW-LocalX;//跟機台方向有關係
		ImageCpY = FullImageH-LocalY;//跟機台方向有關係
		if ( true == SignX )
		{	ImageCpX = LocalX;	}
		else
		{	ImageCpX = FullImageW-LocalX;	}//JET6500
		if ( true == SignY )
		{	ImageCpY = FullImageH-LocalY;	}//JET6500
		else
		{	ImageCpY = LocalY; }
		ImageStartX = ImageCpX-(CellW/2);
		ImageStartY = ImageCpY-(CellH/2);		
		
		for ( i=0; i<CellH; i++ )
		{
			v = i+ImageStartY;
			if ( v<0 || v>=nFullImageH ) { continue; }
			SrcIdx = i*CellStep;
			v = v*FullImageStep;
			for ( j=0; j<CellW; j++ )
			{
				u = j+ImageStartX;
				if ( u<0 || u>=nFullImageW ) 
				{ 
					SrcIdx ++;
					continue; 
				}				
				DstIdx = v+u;
				FullMaskPtr[DstIdx++] = CellPtr[SrcIdx++];		
			}
		}			
		RoiPtr = NULL;
		SubRgnPtr->ClearRgnMaskBuffer_Base();
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIRgn::GetRgnDataModelEnabled() const//取得資料模型啟用
{
	return m_RgnDataModelEnabled;
}
//-------------------------------------------------------------------------------------//
void CAOIRgn::SetRgnDataModelEnabled(bool val)//設定資料模型啟用
{
	m_RgnDataModelEnabled = val;
}
//-------------------------------------------------------------------------------------//
int CAOIRgn::GetRgnDataModelLevelID() const//取得資料模型等級
{
	return m_RgnDataModelLevelID;
}
//-------------------------------------------------------------------------------------//
void CAOIRgn::SetRgnDataModelLevelID(int val)//設定資料模型等級
{
	m_RgnDataModelLevelID = val;
}
//-------------------------------------------------------------------------------------//
bool CAOIRgn::CreateRgnLocalRgnField(const std::vector<TUNI_FRAME> &UniFrameList, CAOIField *&rFieldPtr)//建立區域的局部區域指標
{		
	CString Filename;
	TPOINT2D CadPos;
	TPOINT3D StagePos;
	TPOINT2D CadOffset, StageOffset;
	TREGION4D CadRegion, StageRegion;
	CString DerivedName = GetRgnDerivedName();
	const CAMERA_ID CameraID=GetRgnCameraID();	
	const size_t UniFrameCount=UniFrameList.size();
	if ( DerivedName.GetLength() == 0 )
	{	return false; }

	GetRgnRoiCadRegion(CadRegion);
	GetRgnRoiStageRegion(StageRegion);
	if ( FN_ENABLE==AOIDataCollect.GetSystemParameter().m_ModelImageCadOffsetEnabled )	
	{
		CadOffset=GetRgnFrameImageCadOffset_um();
		AOIDataCollect.MapCadOffsetPtToStage(CadOffset, StageOffset);
	}

	CadPos.x = CadRegion.GetCpX()+CadOffset.x;
	CadPos.y = CadRegion.GetCpY()+CadOffset.y;
	StagePos.x = StageRegion.GetCpX()+StageOffset.x;
	StagePos.y = StageRegion.GetCpY()+StageOffset.y;
	const double RgnSizeW=StageRegion.GetSizeX();
	const double RgnSizeH=StageRegion.GetSizeY();	
	const double ImageResX = AOIDataCollect.GetCameraResolutionX(CameraID);
	const double ImageResY = AOIDataCollect.GetCameraResolutionY(CameraID);	
	CAOIField *FieldPtr=AOIObjManager.CreateFieldObj();
	if ( NULL == FieldPtr ) { return false; }
	FieldPtr->SetFieldListMode(FIELD_LIST_INSPECTION);	
	FieldPtr->SetFieldLinkPointer(false);
	FieldPtr->SetFieldDistrictID(GetRgnDistrictID());
	//FieldPtr->SetFieldCadPos(GetRgnCadPos());
	//FieldPtr->SetFieldStagePos(GetRgnStagePos());
	FieldPtr->SetFieldCadPos(CadPos);
	FieldPtr->SetFieldStagePos(StagePos);
	FieldPtr->SetFieldSizeW_Inner(RgnSizeW);
	FieldPtr->SetFieldSizeH_Inner(RgnSizeH);
	FieldPtr->SetFieldSizeW_Outer(RgnSizeW);
	FieldPtr->SetFieldSizeH_Outer(RgnSizeH);
	FieldPtr->SetFieldSizeW_Real(RgnSizeW);
	FieldPtr->SetFieldSizeH_Real(RgnSizeH);
	FieldPtr->SetFieldCalcState(FIELD_CALC_DONE);	
	FieldPtr->SetFieldMergeState(FIELD_MERGE_DONE);

	const bool bClone=true;
	for ( size_t i=0; i<UniFrameCount; i++ )
	{
		const TUNI_FRAME &UniFrame=UniFrameList[i];
		if ( UniFrame.CheckFrameValid() == false ) { break; }
		MASK_PTR  MaskPtr=UniFrame.MaskPtr;
		SPACE_PTR SpacePtr=UniFrame.SpacePtr;				
		if ( NULL != UniFrame.RawMaskPtr )
		{	MaskPtr = UniFrame.RawMaskPtr; }
		if ( NULL != UniFrame.RawSpacePtr )
		{	SpacePtr = UniFrame.RawSpacePtr;	}

		CAOIFrame *FramePtr=AOIObjManager.CreateFrameObj();
		if ( NULL == FramePtr )
		{							
			FieldPtr->ClearFieldAllFrames();			
			AOIObjManager.DestroyFieldObj(FieldPtr);
			return false;
		}						
		FieldPtr->AddFieldFramePtr(FramePtr);
		//FramePtr->SetFrameType();
		FramePtr->SetFrameLinkPointer(true);
		FramePtr->SetFrameUniqueID(UniFrame.FrameUniqueID);								
		FramePtr->SetFrameCameraID(GetRgnCameraID());
		FramePtr->SetFrameLightMode(GetRgnLightMode());		
		FramePtr->SetFrameDistrictID(GetRgnDistrictID());
		FramePtr->SetFrameCalcState(FRAME_CALC_DONE);
		FramePtr->SetFrameMergeState(FRAME_MERGE_DONE);
		FramePtr->SetFrameImageIndex((unsigned int)(i));
		bool bSucc=true;
		if ( NULL==MaskPtr || NULL==SpacePtr )
		{	
			if ( 8 == UniFrame.BitCount )
			{	FramePtr->SetFrameType(FRAME_GRAY); }
			else
			{	FramePtr->SetFrameType(FRAME_COLOR); }			
			Filename.Format(_T("%s#%d.%s"), DerivedName, i+1, _T("PNG"));
			bSucc=FramePtr->SetFrameImagePtr(UniFrame.ImageW, UniFrame.ImageH, UniFrame.ImageStep, UniFrame.BitCount, UniFrame.ImagePtr, bClone);	
		}
		else
		{
			Filename.Format(_T("%s#%d.%s"), DerivedName, i+1, EXT_NAME_SPACE);
			FramePtr->SetFrameType(FRAME_SPACE);
			bSucc=FramePtr->SetFrameSpacePtr(UniFrame.ImageW, UniFrame.ImageH, UniFrame.ImageStep, UniFrame.BitCount, MaskPtr, SpacePtr, bClone);
		}
		FramePtr->SetFrameImageValid(bSucc);
		if ( false == bSucc )
		{
			FieldPtr->ClearFieldAllFrames();			
			AOIObjManager.DestroyFieldObj(FieldPtr);
			return false;
		}
		FramePtr->SetFrameFileName(Filename);
		FramePtr->SetFrameFileNameOffline(Filename);
		FramePtr->SetFrameResolutionX(ImageResX);
		FramePtr->SetFrameResolutionX(ImageResY);
		//FramePtr->SetFrameFileFolder(TuningFolder);						
		//FramePtr->SetFrameFileFolderOffline(TuningFolder);
	}
	rFieldPtr = FieldPtr;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIRgn::CheckRgnNeedSaveImage(TASK_MODE TaskMode, SAVE_TEST_IMAGE_MODE SaveMode, RESULT_ID ResultID) const//確認是否需要存圖
{
	bool SaveImage=false;
	switch ( TaskMode )
	{
	case TASK_TUNING_PROJECT://任務-檢測調適專案
	case TASK_TUNING_OFFLINE://任務-檢測離線專案-20181026
	case TASK_INSPECT_PROJECT://任務-檢測檢測專案
		SaveImage = true;			
		break;
	default:
		SaveImage = false;			
		break;
	}
	if ( true == SaveImage )
	{
		switch ( SaveMode )
		{
		case SAVE_TEST_IMAGE_DEFECT:
			switch ( ResultID )
			{
			case RESULT_ID_NG:
			case RESULT_ID_EXCEPTION:
				SaveImage = true;
				break;
			default:
				SaveImage = false;
				break;
			}
			break;
		case SAVE_TEST_IMAGE_EVERYONE:
			SaveImage = true;
			break;
		default:
		case SAVE_TEST_IMAGE_DISABLE:
			SaveImage = false;				
			break;
		}
	}
	return SaveImage;
}
//-------------------------------------------------------------------------------------//
bool CAOIRgn::CalcRgnBodyOutsideParam(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, TBasePlaneParam &BasePlaneParam)//計算區域的本體外圍參數
{
	if ( BASE_PLANE_BODY_OUTSIDE_DISABLE == BasePlaneParam.BodyOutsideMode ) { return true; }
	const size_t SubRgnCount=GetRgnSubRgnCount();
	CAOIComponent *ComponentPtr=DYNAMIC_DOWNCAST(CAOIComponent, this);
	if ( NULL == ComponentPtr ) { return false; }
	CAOIModel *ModelPtr=ComponentPtr->GetComponentModelPtr();
	if ( NULL == ModelPtr )	{ return false; }
	if ( ModelPtr->CalcModelBodyOutsideParam(ImageW, ImageH, BasePlaneParam) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//