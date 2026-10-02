// AOIMark.h: interface for the CAOIMark class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AOIMARK_H__A7C69DE4_44CE_49D3_B0B6_66AF8CA4DEB3__INCLUDED_)
#define AFX_AOIMARK_H__A7C69DE4_44CE_49D3_B0B6_66AF8CA4DEB3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include "AOIRgn.h"
#include "AOIModel.h"
//-------------------------------------------------------------------------------------//
class CAOIFileIO;
//-------------------------------------------------------------------------------------//
enum MARK_TYPE_MODE
{
	MARK_TASK_NONE        = 0,//未定義
	MARK_TASK_BASE_PLANE  = 1,//基準面
	MARK_TASK_RETURN
};
//-------------------------------------------------------------------------------------//
class CAOIMark : public CAOIRgn  
{
	//---------------------------------------------------------------------------------//	
	DECLARE_DYNAMIC(CAOIMark)
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//
	static CRITICAL_SECTION    m_csMark;//同步機制-關鍵區間
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	static void                InitialMarkLock();//初始化特徵點的關鍵區間
	static void                DeleteMarkLock(); //刪除特徵點的關鍵區間
	static void                LockMark();        //進入特徵點的關鍵區間
	static void                UnlockMark();      //離開特徵點的關鍵區間
	//---------------------------------------------------------------------------------//	
private:	
	//---------------------------------------------------------------------------------//
	bool                       m_MarkDeleted;//特徵點是否刪除
	bool                       m_MarkSelected;//特徵點是否選取到	
	int                        m_MarkUniqueID;//特徵點唯一碼
	int                        m_MarkGroupID;//特徵點群組編號	
	MARK_TYPE_MODE             m_MarkTypeMode;//特徵點樣式模式	
	CJetGroundEquation         m_MarkGroundEquation_Online;//特徵點局部底面方程式參數
	//---------------------------------------------------------------------------------//
	int                        m_MarkTempInt[4];//特徵點暫存整數	
	//---------------------------------------------------------------------------------//
	CAOIModel                  m_MarkModel;	
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//
	void                       PreInitMark();
	void                       InitialMark();
	void                       CloneMark(const CAOIMark &others);
	//---------------------------------------------------------------------------------//	
public:
	//---------------------------------------------------------------------------------//
	CAOIMark();
	CAOIMark(const CAOIMark &others);
	virtual ~CAOIMark();
	CAOIMark& operator=(const CAOIMark &others);
	//---------------------------------------------------------------------------------//
	CAOIMark* CAOIMark::CloneMarkObj() const;//建立且複製一個特徵物件
	//---------------------------------------------------------------------------------//
	bool                       WriteMarkFile(CAOIFileIO &FileIO);//儲存特徵點檔案
	bool                       ReadMarkFile(CAOIFileIO &FileIO);//載入特徵點檔案
	//---------------------------------------------------------------------------------//	
	void                       SetMarkIndex(unsigned int value) { SetRgnIndex(value); }
	unsigned int               GetMarkIndex() const { return GetRgnIndex(); }
	//---------------------------------------------------------------------------------//
	void                       SetMarkIndex_Project(unsigned int value) { SetRgnIndex_Project(value); }
	unsigned int               GetMarkIndex_Project() const { return GetRgnIndex_Project(); }
	//---------------------------------------------------------------------------------//		
	void                       SetMarkIndex_Panel(unsigned int value) { SetRgnIndex_Panel(value); }
	unsigned int               GetMarkIndex_Panel() const { return GetRgnIndex_Panel(); }
	//---------------------------------------------------------------------------------//
	void                       SetMarkIndex_Board(unsigned int value) { SetRgnIndex_Board(value); }
	unsigned int               GetMarkIndex_Board() const { return GetRgnIndex_Board(); }
	//---------------------------------------------------------------------------------//
	void                       SetMarkProjectPtr(CAOIProject *value) { SetRgnProjectPtr(value); }
	CAOIProject*               GetMarkProjectPtr() const { return GetRgnProjectPtr(); }
	//---------------------------------------------------------------------------------//
	void                       SetMarkPanelPtr(CAOIPanel *value) { SetRgnPanelPtr(value); }
	CAOIPanel*                 GetMarkPanelPtr() const { return GetRgnPanelPtr(); }
	//---------------------------------------------------------------------------------//	
	void                       SetMarkPanelIndex_Project(unsigned int value) { SetRgnPanelIndex_Project(value); }
	unsigned int               GetMarkPanelIndex_Project() const { return GetRgnPanelIndex_Project(); }
	//---------------------------------------------------------------------------------//
	void                       SetMarkBoardPtr(CAOIBoard *value) { SetRgnBoardPtr(value); }
	CAOIBoard*                 GetMarkBoardPtr() const { return GetRgnBoardPtr(); }
	//---------------------------------------------------------------------------------//		
	void                       SetMarkBoardIndex_Project(unsigned int value) { SetRgnBoardIndex_Project(value); }
	unsigned int               GetMarkBoardIndex_Project() const { return GetRgnBoardIndex_Project(); }
	//---------------------------------------------------------------------------------//	
	void                       SetMarkBoardIndex_Panel(unsigned int value) { SetRgnBoardIndex_Panel(value); }
	unsigned int               GetMarkBoardIndex_Panel() const { return GetRgnBoardIndex_Panel(); }
	//---------------------------------------------------------------------------------//	
	bool                       CreateMarkSelfFieldPtr();//建立專屬Field指標	
	CAOIField*                 GetMarkSelfFieldPtr() const { return m_RgnSelfFieldPtr; }
	//---------------------------------------------------------------------------------//
	void                       SetMarkSelfFieldEnabled(bool val) { m_RgnSelfFieldEnabled=val; }
	bool                       GetMarkSelfFieldEnabled() const { return m_RgnSelfFieldEnabled; }	
	//---------------------------------------------------------------------------------//	
	void                       SetMarkFieldPtr(CAOIField *Ptr);
	CAOIField*                 GetMarkFieldPtr() const { return m_RgnFieldPtr; }
	//---------------------------------------------------------------------------------//		
	void                       SetMarkFieldIndex(unsigned int value) { m_RgnFieldIdx = value; }
	unsigned int               GetMarkFieldIndex() const { return m_RgnFieldIdx; }
	//---------------------------------------------------------------------------------//
	void                       SetMarkDeleted(bool value) { m_MarkDeleted = value; }
	bool                       GetMarkDeleted() const { return m_MarkDeleted; }
	//---------------------------------------------------------------------------------//
	void                       SetMarkSelected(bool value) { m_MarkSelected = value; }
	bool                       GetMarkSelected() const { return m_MarkSelected; }
	//---------------------------------------------------------------------------------//	
	bool                       GetMarkModelImageIsSaved() const;//特徵點是否儲存過圖像
	void                       SetMarkModelImageIsSaved(bool val);//特徵點是否儲存過圖像
	//---------------------------------------------------------------------------------//	
	//特徵點唯一碼	
	int                        GetMarkUniqueID() const;
	void                       SetMarkUniqueID(int value);		
	//---------------------------------------------------------------------------------//
	int                        GetMarkGroupID() const;
	void                       SetMarkGroupID(int value);	
	bool                       CheckMarkGroupIDValid() const;//確認特徵點群組編號有效
	//---------------------------------------------------------------------------------//		
	//特徵點的暫存整數
	void                       SetMarkTempInt(int value, int idx=0) { m_MarkTempInt[idx] = value; }
	int                        GetMarkTempInt(int idx=0) const { return m_MarkTempInt[idx]; }	
	//---------------------------------------------------------------------------------//
	//特徵點樣式模式
	MARK_TYPE_MODE             GetMarkTypeMode() const;
	void                       SetMarkTypeMode(MARK_TYPE_MODE value);	
	bool                       CheckMarktType_BasePlane() const { return MARK_TASK_BASE_PLANE==m_MarkTypeMode?true:false; }
	//---------------------------------------------------------------------------------//		
	//軌道編號
	LANE_ID                    GetMarkLaneID() const;
	void                       SetMarkLaneID(LANE_ID value);	
	//---------------------------------------------------------------------------------//
	//不檢測
	void                       SetMarkBypassed(bool value);
	bool                       GetMarkBypassed() const { return CAOIRgn::GetRgnBypassed(); }	
	bool                       UpdateMarkBypassed(); //確認特徵點是否為不檢測
	//---------------------------------------------------------------------------------//
	//使用的OpenMP數量
	int                        CalcMarkOpenMPCountByPixels();
	void                       SetMarkOpenMPCount(int value);
	int                        GetMarkOpenMPCount() const { return GetRgnOpenMPCount(); }
	//---------------------------------------------------------------------------------//
	void                       BypassSkipMark(RESULT_ID value);//不檢測或跳過特徵點
	//---------------------------------------------------------------------------------//	
	RESULT_ID                  GetMarkResultID_AOI() const;//取得特徵點結果編號
	void                       SetMarkResultID_AOI(RESULT_ID value);	//設定特徵點結果編號	
	//---------------------------------------------------------------------------------//	
	RESULT_ID                  GetMarkResultID_AOI_LA() const;//取得特徵點結果編號-A軌
	void                       SetMarkResultID_AOI_LA(RESULT_ID value);	//設定特徵點結果編號-A軌
	//---------------------------------------------------------------------------------//	
	RESULT_ID                  GetMarkResultID_AOI_LB() const;//取得特徵點結果編號-B軌
	void                       SetMarkResultID_AOI_LB(RESULT_ID value);	//設定特徵點結果編號-B軌
	//---------------------------------------------------------------------------------//	
	void                       UpdateMarkResultID_AOI_Lane(LANE_ID LaneID);	//更新特徵點結果編號-軌道
	RESULT_ID                  GetMarkResultID_AOI_Lane(LANE_ID LaneID) const;//取得特徵點結果編號-軌道	
	//---------------------------------------------------------------------------------//	
	RESULT_ID                  GetMarkResultID_ARS() const;//取得特徵點結果編號
	void                       SetMarkResultID_ARS(RESULT_ID value);	//設定特徵點結果編號	
	//---------------------------------------------------------------------------------//	
	RESULT_ID                  GetMarkResultID_ARS_LA() const;//取得特徵點結果編號-A軌
	void                       SetMarkResultID_ARS_LA(RESULT_ID value);	//設定特徵點結果編號-A軌
	//---------------------------------------------------------------------------------//	
	RESULT_ID                  GetMarkResultID_ARS_LB() const;//取得特徵點結果編號-B軌
	void                       SetMarkResultID_ARS_LB(RESULT_ID value);	//設定特徵點結果編號-B軌
	//---------------------------------------------------------------------------------//	
	void                       UpdateMarkResultID_ARS_Lane(LANE_ID LaneID);//更新特徵點結果編號-軌道
	RESULT_ID                  GetMarkResultID_ARS_Lane(LANE_ID LaneID) const;//取得特徵點結果編號-軌道	
	//---------------------------------------------------------------------------------//	
	RESULT_ID                  GetMarkResultID_Alarm() const;//取得特徵點結果編號-警報
	void                       SetMarkResultID_Alarm(RESULT_ID value);//設定特徵點結果編號-警報
	//---------------------------------------------------------------------------------//	
	//特徵點保留影像
	void                       SetMarkKeepImage(bool value) { m_RgnKeepImage = value; }
	bool	                   GetMarkKeepImage() const { return m_RgnKeepImage; }
	//---------------------------------------------------------------------------------//	
	//特徵點填滿畫面的時間
	void                       SetMarkFillImageTime(double value) { m_RgnFillImageTime = value; }
	double                     GetMarkFillImageTime() const { return m_RgnFillImageTime; }
	//---------------------------------------------------------------------------------//
	//影像序號
	void                       SetMarkFrameIndex(unsigned int value) { SetRgnFrameIndex(value); }
	unsigned int               GetMarkFrameIndex() const { return GetRgnFrameIndex(); }
	//---------------------------------------------------------------------------------//
	//影像唯一碼
	void                       SetMarkFrameUniqueID(unsigned int value) { SetRgnFrameUniqueID(value); }
	unsigned int               GetMarkFrameUniqueID() const { return GetRgnFrameUniqueID(); }
	//---------------------------------------------------------------------------------//
	//相機編號
	void                       SetMarkCameraID(CAMERA_ID value) { m_RgnCameraID = value; }
	CAMERA_ID                  GetMarkCameraID() const { return m_RgnCameraID; }
	//---------------------------------------------------------------------------------//
	//燈源模式
	void                       SetMarkLightMode(LIGHT_MODE value) { m_RgnLightMode = value; }
	LIGHT_MODE                 GetMarkLightMode() const { return m_RgnLightMode; }
	//---------------------------------------------------------------------------------//	
	//分段編號//兩段式檢測
	void                       SetMarkDistrictID(DISTRICT_ID value) { m_RgnDistrictID = value; }
	DISTRICT_ID                GetMarkDistrictID() const { return m_RgnDistrictID; }
	//---------------------------------------------------------------------------------//
	//要去計算
	void                       SetMarkNeedToCalculate(bool value) { CAOIRgn::SetRgnNeedToCalculate(value); }
	bool                       GetMarkNeedToCalculate() const { return m_RgnNeedToCalculate; }
	//---------------------------------------------------------------------------------//
	//要去計算-備份檔
	void                       SetMarkNeedToCalculateBackup(bool value) { CAOIRgn::SetRgnNeedToCalculateBackup(value); }
	bool                       GetMarkNeedToCalculateBackup() const { return m_RgnNeedToCalculateBackup; }
	//---------------------------------------------------------------------------------//
	//特徵點角度
	void                       SetMarkAngle(double value) { m_RgnAngle = value; }
	double                     GetMarkAngle() const { return m_RgnAngle; }
	//---------------------------------------------------------------------------------//
	//特徵點尺寸寬
	void                       SetMarkRoiSizeW(double value) { m_RgnRoiSize.cx = value; }
	double                     GetMarkRoiSizeW() const { return m_RgnRoiSize.cx; }
	//---------------------------------------------------------------------------------//
	//特徵點尺寸長
	void                       SetMarkRoiSizeH(double value) { m_RgnRoiSize.cy = value; }
	double                     GetMarkRoiSizeH() const { return m_RgnRoiSize.cy; }
	//---------------------------------------------------------------------------------//
	//特徵點尺寸寬
	void                       SetMarkBodySizeW(double value) { m_RgnBodySize.cx = value; }
	double                     GetMarkBodySizeW() const { return m_RgnBodySize.cx; }
	//---------------------------------------------------------------------------------//
	//特徵點尺寸長
	void                       SetMarkBodySizeH(double value) { m_RgnBodySize.cy = value; }
	double                     GetMarkBodySizeH() const { return m_RgnBodySize.cy; }
	//---------------------------------------------------------------------------------//
	//特徵點位置在CAD坐標系中
	void                       SetMarkCadPos(const TPOINT2D &value) { m_RgnCadPos = value; }
	TPOINT2D                   GetMarkCadPos() const { return m_RgnCadPos; }
	//---------------------------------------------------------------------------------//
	//特徵點位置在CAD坐標系中-X
	void                       SetMarkCadPosX(double value) { m_RgnCadPos.x = value; }
	double                     GetMarkCadPosX() const { return m_RgnCadPos.x; }
	//---------------------------------------------------------------------------------//
	//特徵點位置在CAD坐標系中-Y
	void                       SetMarkCadPosY(double value) { m_RgnCadPos.y = value; }
	double                     GetMarkCadPosY() const { return m_RgnCadPos.y; }
	//---------------------------------------------------------------------------------//	
	CAOIWnd*                   GetMarkWndPtr();//取得特徵點檢測框指標
	//---------------------------------------------------------------------------------//		
	WND_LOGIC_TYPE             GetMarkLogicType();//檢測框邏輯樣式	
	//---------------------------------------------------------------------------------//		
	int                        GetMarkLogicGroupID();//檢測框邏輯群組編號	
	//---------------------------------------------------------------------------------//
	//特徵點四端點X
	void                       SetMarkCadCornerPosX(const double value[]) 
	{ 
		m_RgnRoiCadCornerPos[0].x = value[0]; 
		m_RgnRoiCadCornerPos[1].x = value[1]; 
		m_RgnRoiCadCornerPos[2].x = value[2]; 
		m_RgnRoiCadCornerPos[3].x = value[3]; 
	}
	void                      GetMarkCadCornerPosX(double value[]) const 
	{ 
		value[0] = m_RgnRoiCadCornerPos[0].x; 
		value[1] = m_RgnRoiCadCornerPos[1].x; 
		value[2] = m_RgnRoiCadCornerPos[2].x; 
		value[3] = m_RgnRoiCadCornerPos[3].x; 
	}
	//---------------------------------------------------------------------------------//
	//特徵點四端點Y
	void                       SetMarkCadCornerPosY(const double value[]) 
	{ 
		m_RgnRoiCadCornerPos[0].y = value[0]; 
		m_RgnRoiCadCornerPos[1].y = value[1]; 
		m_RgnRoiCadCornerPos[2].y = value[2]; 
		m_RgnRoiCadCornerPos[3].y = value[3]; 
	}
	void                       GetMarkCadCornerPosY(double value[]) const 
	{ 
		value[0] = m_RgnRoiCadCornerPos[0].y; 
		value[1] = m_RgnRoiCadCornerPos[1].y; 
		value[2] = m_RgnRoiCadCornerPos[2].y; 
		value[3] = m_RgnRoiCadCornerPos[3].y; 
	}
	//---------------------------------------------------------------------------------//	
	//特徵點位置在CAD坐標系中
	void                       SetMarkStagePos(const TPOINT3D &value) { m_RgnStagePos = value; }
	TPOINT3D                   GetMarkStagePos() const { return m_RgnStagePos; }
	//---------------------------------------------------------------------------------//
	//特徵點位置在機台坐標系中-X
	void                       SetMarkStagePosX(double value) { m_RgnStagePos.x = value; }
	double                     GetMarkStagePosX() const { return m_RgnStagePos.x; }
	//---------------------------------------------------------------------------------//
	//特徵點位置在機台坐標系中-Y
	void                       SetMarkStagePosY(double value) { m_RgnStagePos.y = value; }
	double                     GetMarkStagePosY() const { return m_RgnStagePos.y; }
	//---------------------------------------------------------------------------------//	
	//特徵點位置在機台坐標系中-Z
	void                       SetMarkStagePosZ(double value) { m_RgnStagePos.z = value; }
	double                     GetMarkStagePosZ() const { return m_RgnStagePos.z; }
	//---------------------------------------------------------------------------------//	
	void                       GetMarkRoiStageCornerPos(TPOINT2D value[]) const 
	{
		value[0].x = m_RgnRoiStageCornerPos[0].x;	value[0].y = m_RgnRoiStageCornerPos[0].y; 	
		value[1].x = m_RgnRoiStageCornerPos[1].x;	value[1].y = m_RgnRoiStageCornerPos[1].y; 	
		value[2].x = m_RgnRoiStageCornerPos[2].x;	value[2].y = m_RgnRoiStageCornerPos[2].y; 	
		value[3].x = m_RgnRoiStageCornerPos[3].x;	value[3].y = m_RgnRoiStageCornerPos[3].y; 	
	}
	//---------------------------------------------------------------------------------//	
	void                       GetMarkBodyStageCornerPos(TPOINT2D value[]) const 
	{
		value[0].x = m_RgnBodyStageCornerPos[0].x;	value[0].y = m_RgnBodyStageCornerPos[0].y; 	
		value[1].x = m_RgnBodyStageCornerPos[1].x;	value[1].y = m_RgnBodyStageCornerPos[1].y; 	
		value[2].x = m_RgnBodyStageCornerPos[2].x;	value[2].y = m_RgnBodyStageCornerPos[2].y; 	
		value[3].x = m_RgnBodyStageCornerPos[3].x;	value[3].y = m_RgnBodyStageCornerPos[3].y; 	
	}
	//---------------------------------------------------------------------------------//	
	//特徵點四端點在機台坐標系中-X
	void                       SetMarkRoiStageCornerPosX(double value[]) 
	{ 
		m_RgnRoiStageCornerPos[0].x = value[0]; 
		m_RgnRoiStageCornerPos[1].x = value[1]; 
		m_RgnRoiStageCornerPos[2].x = value[2]; 
		m_RgnRoiStageCornerPos[3].x = value[3]; 
	}
	void                       GetMarkRoiStageCornerPosX(double value[]) const 
	{ 
		value[0] = m_RgnRoiStageCornerPos[0].x; 
		value[1] = m_RgnRoiStageCornerPos[1].x; 
		value[2] = m_RgnRoiStageCornerPos[2].x; 
		value[3] = m_RgnRoiStageCornerPos[3].x; 
	}	
	//---------------------------------------------------------------------------------//
	//特徵點四端點在機台坐標系中-Y
	void                       SetMarkRoiStageCornerPosY(double value[]) 
	{ 
		m_RgnRoiStageCornerPos[0].y = value[0]; 
		m_RgnRoiStageCornerPos[1].y = value[1]; 
		m_RgnRoiStageCornerPos[2].y = value[2]; 
		m_RgnRoiStageCornerPos[3].y = value[3]; 
	}
	void                       GetMarkRoiStageCornerPosY(double value[]) const 
	{ 
		value[0] = m_RgnRoiStageCornerPos[0].y; 
		value[1] = m_RgnRoiStageCornerPos[1].y; 
		value[2] = m_RgnRoiStageCornerPos[2].y; 
		value[3] = m_RgnRoiStageCornerPos[3].y; 
	}
	//---------------------------------------------------------------------------------//
	TREGION4D                  GetMarkRoiRgnCad() const;//特徵點範圍-Cad
	TREGION4D                  GetMarkBodyRgnCad() const;//特徵點範圍-Cad
	TREGION4D                  GetMarkRoiRgnStage() const;//特徵點範圍-Stage
	TREGION4D                  GetMarkBodyRgnStage() const;//特徵點範圍-Stage
	//---------------------------------------------------------------------------------//
	void                       GetMarkRoiCadRegion(TREGION4D &Region);//取得特徵點在Cad的範圍	
	void                       GetMarkBodyCadRegion(TREGION4D &Region);//取得特徵點在Cad的範圍	
	void                       GetMarkRoiStageRegion(TREGION4D &Region);//取得特徵點在Stage的範圍		
	void                       GetMarkBodyStageRegion(TREGION4D &Region);//取得特徵點在Stage的範圍		
	//---------------------------------------------------------------------------------//
	void                       MoveMarkCadPos(double dX, double dY);//移動特徵點座標	
	void                       MoveMarkStagePos(double dX, double dY);//移動特徵點座標	
	void                       MoveMarkPos(double dX, double dY, CMapCoordinate *MapPtr);//移動特徵點座標	
	//---------------------------------------------------------------------------------//	
	bool                       CheckMarkBePickByCad(const TPOINT2D &PickPos);//確認特徵點被點擊到
	bool                       CheckMarkBePickByStage(const TPOINT2D &PickPos);//確認特徵點被點擊到

	bool                       CheckMarkInRegionByCad(const TREGION4D &SelRgn, bool bEntireIn);//確認特徵點在範圍內
	bool                       CheckMarkInRegionByStage(const TREGION4D &SelRgn, bool bEntireIn);//確認特徵點在範圍內
	//---------------------------------------------------------------------------------//		
	void                       CalcMarkCadCornerPos();//計算特徵點Cad端點座標		
	void                       LayoutMarkStageCornerPos();//更新特徵點機台端點座標
	//---------------------------------------------------------------------------------//
	//特徵點所屬FOV的CAD位置-X
	void                       SetMarkFovCadPosX(double value) { m_RgnFovCadPos.x = value; }
	double                     GetMarkFovCadPosX() const { return m_RgnFovCadPos.x; }
	//---------------------------------------------------------------------------------//
	//特徵點所屬FOV的CAD位置-Y
	void                       SetMarkFovCadPosY(double value) { m_RgnFovCadPos.y = value; }
	double                     GetMarkFovCadPosY() const { return m_RgnFovCadPos.y; }
	//---------------------------------------------------------------------------------//
	//特徵點所屬FOV的Stage位置-X
	void                       SetMarkFovStagePosX(double value) { m_RgnFovStagePos.x = value; }
	double                     GetMarkFovStagePosX() const { return m_RgnFovStagePos.x; }
	//---------------------------------------------------------------------------------//
	//特徵點所屬FOV的Stage位置-Y
	void                       SetMarkFovStagePosY(double value) { m_RgnFovStagePos.y = value; }
	double                     GetMarkFovStagePosY() const { return m_RgnFovStagePos.y; }
	//---------------------------------------------------------------------------------//
	//特徵點所屬影像的區域
	void                       SetMarkFrameImageRect(const RECT &value) { m_RgnFrameImageRect = value; }
	const RECT&                GetMarkFrameImageRect() const { return m_RgnFrameImageRect; }
	//---------------------------------------------------------------------------------//	
	//特徵點影像的物理尺寸
	void                       SetMarkFrameImageSize_um(const TSIZE2D &value);
	const TSIZE2D&             GetMarkFrameImageSize_um() const { return m_RgnFrameImageSize_um; }
	//---------------------------------------------------------------------------------//	
	//特徵點影像的區域Cad偏差-um
	void                       SetMarkFrameImageCadOffset_um(const TPOINT2D &value);	
	const TPOINT2D&            GetMarkFrameImageCadOffset_um() const { return m_RgnFrameImageCadOffset_um; }
	void                       SetMarkFrameImageStageOffset_um(const TPOINT2D &value);
	//---------------------------------------------------------------------------------//	
	void                       SetMarkFrameImageCornerPt(const POINT value[]) 
	{ 
		m_RgnFrameImageCornerPt[0] = value[0]; 
		m_RgnFrameImageCornerPt[1] = value[1]; 
		m_RgnFrameImageCornerPt[2] = value[2]; 
		m_RgnFrameImageCornerPt[3] = value[3]; 
	}
	void                       GetMarkFrameImageCornerPt(POINT value[]) const 
	{ 
		value[0] = m_RgnFrameImageCornerPt[0]; 
		value[1] = m_RgnFrameImageCornerPt[1]; 
		value[2] = m_RgnFrameImageCornerPt[2]; 
		value[3] = m_RgnFrameImageCornerPt[3]; 
	}
	//---------------------------------------------------------------------------------//	
	//特徵點所屬的Field的CAD位置-X
	void                       SetMarkFieldCadPosX(double value) { m_RgnFieldCadPos.x = value; }
	double                     GetMarkFieldCadPosX() const { return m_RgnFieldCadPos.x; }
	//---------------------------------------------------------------------------------//
	//特徵點所屬的Field的CAD位置-Y
	void                       SetMarkFieldCadPosY(double value) { m_RgnFieldCadPos.y = value; }
	double                     GetMarkFieldCadPosY() const { return m_RgnFieldCadPos.y; }
	//---------------------------------------------------------------------------------//
	//特徵點所屬的Field的Stage位置-X
	void                       SetMarkFieldStagePosX(double value) { m_RgnFieldStagePos.x = value; }
	double                     GetMarkFieldStagePosX() const { return m_RgnFieldStagePos.x; }
	//---------------------------------------------------------------------------------//
	//特徵點所屬的Field的Stage位置-Y
	void                       SetMarkFieldStagePosY(double value) { m_RgnFieldStagePos.y = value; }
	double                     GetMarkFieldStagePosY() const { return m_RgnFieldStagePos.y; }
	//---------------------------------------------------------------------------------//	
	void                       MapMarkCadToStagePos(const CMapCoordinate &Map);//將特徵點CAD轉成機台座標
	//---------------------------------------------------------------------------------//
	bool                       SpinMark(double Angle, CMapCoordinate *MapPtr);//特徵點自旋轉
	bool                       RotateMark(double Angle, double CpX, double CpY, CMapCoordinate *MapPtr);//特徵點旋轉	
	bool                       MirrorXMark(double CpX, CMapCoordinate *MapPtr);//特徵點鏡射-X
	bool                       MirrorYMark(double CpY, CMapCoordinate *MapPtr);//特徵點鏡射-Y	
	//---------------------------------------------------------------------------------//	
	CString                    GetMarkFullName() const;//取得特徵點的名稱	
	virtual CString            GetRgnDerivedName() const;//取得特徵點的名稱	
	virtual CString            GetRgnDerivedKeyName() const;//取得特徵點的名稱	
	virtual bool               ExtractRgnDerivedFrame(bool &Finished);//挖取特徵點圖片
	virtual bool               ExecRgnDerivedInspection();//執行特徵點檢測
	//---------------------------------------------------------------------------------//
	bool                       CreateMarkSubRgnList(FIELD_BUILD_MODE BuildMode, bool ByCadRegion);//建立特徵點子檢測區域列表
	void                       ClearMarkSubRgnList();//清除特徵點的子列表
	size_t                     GetMarkSubRgnCount() const;//取得特徵點的子數量
	CAOIRgn*                   GetMarkSubRgnPtr(size_t index, bool check) const;//取得特徵點的子指標
	//---------------------------------------------------------------------------------//	
	CAOIModel*                 GetMarkModelPtr();//取得特徵點模組
	bool                       UpdateMarkParamToModel();//更新特徵點參數至模組內
	bool                       UpdateMarkModelFromLibrary(CAOIModel *ModelPtr);//更新特徵點模組
	//---------------------------------------------------------------------------------//	
	void                       InitMarkInspection();//初始化特徵點檢測
	bool                       ExecMarkInspection();//執行特徵點檢測
	bool                       ExecMarkSaveDefectImage(const std::vector<TUNI_FRAME> &UniFrameList);//執行特徵點儲存瑕疵圖片	
	bool                       UpdateMarkResultID();//更新檢測框檢測結果
	bool                       AnalysisMarkGroundPlaneParam();		
	//---------------------------------------------------------------------------------//		
	//空間基準面參數
	//---------------------------------------------------------------------------------//	
	void                       SetMarkPanelBasePlane(double val);
	double                     GetMarkPanelBasePlane() const { return GetRgnPanelBasePlane(); }
	//---------------------------------------------------------------------------------//		
	void                       SetMarkLocalBasePlaneID(int val) { SetRgnLocalBasePlaneID(val); }
	int                        GetMarkLocalBasePlaneID() const { return GetRgnLocalBasePlaneID(); }
	//---------------------------------------------------------------------------------//	
	void                       SetMarkLocalBasePlaneFinish(bool val) { SetRgnLocalBasePlaneFinish(val); }
	bool                       GetMarkLocalBasePlaneFinish() const { return GetRgnLocalBasePlaneFinish(); }
	//---------------------------------------------------------------------------------//
	bool                       CheckMarkPanelBasePlaneParamValid() const;//確認空間基準面參數有效
	//---------------------------------------------------------------------------------//
	void                       SetMarkLocalBasePlaneParam(const TPOINT3D &val) { SetRgnLocalBasePlaneParam(val); }
	void                       GetMarkLocalBasePlaneParam(TPOINT3D &val) const { GetRgnLocalBasePlaneParam(val); }
	//---------------------------------------------------------------------------------//
	CJetGroundEquation&        GetMarkGroundEquation_Online() { return m_MarkGroundEquation_Online; }	
	const CJetGroundEquation&  GetMarkGroundEquation_Online() const { return m_MarkGroundEquation_Online; }	
	void                       SetMarkGroundEquation_Online(const CJetGroundEquation &val) { m_MarkGroundEquation_Online=val; }
	//---------------------------------------------------------------------------------//	
	TBasePlaneParam&           GetMarkSpaceBasePlaneParam() { return GetRgnSpaceBasePlaneParam(); }
	const TBasePlaneParam&     GetMarkSpaceBasePlaneParam() const { return GetRgnSpaceBasePlaneParam(); }
	void                       SetMarkSpaceBasePlaneParam(const TBasePlaneParam& Param);
	//---------------------------------------------------------------------------------//
	//空間雜訊過濾處理
	TNoiseFilterParam&         GetMarkSpaceNoiseFilterParam() { return GetRgnSpaceNoiseFilterParam(); }
	const TNoiseFilterParam&   GetMarkSpaceNoiseFilterParam() const { return GetRgnSpaceNoiseFilterParam(); }
	void                       SetMarkSpaceNoiseFilterParam(const TNoiseFilterParam& Param);
	//---------------------------------------------------------------------------------//
	//特殊遮罩-基準面
	void                       SetMarkMaskEnable_Base(bool val); 
	bool                       GetMarkMaskEnable_Base() const; 

	void                       SetMarkMaskFrameIndex_Base(unsigned int val);//影像序號
	unsigned int               GetMarkMaskFrameIndex_Base() const;//影像序號

	void                       SetMarkMaskFrameUniqueID_Base(unsigned int val);//影像唯一碼
	unsigned int               GetMarkMaskFrameUniqueID_Base() const;//影像唯一碼

	void                       SetMarkMaskColorGroupLinkIndex(int val);//彩色過濾的連動編號	
	int                        GetMarkMaskColorGroupLinkIndex() const;//彩色過濾的連動編號	
	//---------------------------------------------------------------------------------//
	bool                       UpdateMarkColorGroupLinkIndex(const std::vector<CColorGroup> &ColorGroupList);//更新模組內的彩色過濾連動
	bool                       UpdateMarkFrameIndex(unsigned int DefaultIndex, unsigned int DefaultUniqueID, const std::vector<unsigned int> &FrameIndexMapList);
	//---------------------------------------------------------------------------------//	
	bool                       GetMarkDataModelEnabled() const;//取得特徵資料模型啟用
	void                       SetMarkDataModelEnabled(bool val);//設定特徵資料模型啟用
	//---------------------------------------------------------------------------------/
	int                        GetMarkDataModelLevelID() const;//取得特徵資料模型等級
	void                       SetMarkDataModelLevelID(int val);//設定特徵資料模型等級
	//---------------------------------------------------------------------------------/
};
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_AOIMARK_H__A7C69DE4_44CE_49D3_B0B6_66AF8CA4DEB3__INCLUDED_)
