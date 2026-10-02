// AOIFd.h: interface for the CAOIFd class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#if !defined(AFX_AOIFD_H__6DDD663A_05EC_4E66_B07A_0374B3A413C9__INCLUDED_)
#define AFX_AOIFD_H__6DDD663A_05EC_4E66_B07A_0374B3A413C9__INCLUDED_
//-------------------------------------------------------------------------------------//
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#define FD_MAX_PATTERN               4
//----------------------------------------------------------------------------//
#include "AOIRgn.h"
#include "AOIModel.h"
//-------------------------------------------------------------------------------------//
class CAOIFileIO;
//-------------------------------------------------------------------------------------//
class CAOIFd;
typedef struct tagFdRect
{		
	unsigned int   FdIndex;	
	CAOIFd        *FdPtr;
	TREGION4D      FdRgn;	
	tagFdRect()
	{
		FdIndex = -1;		
		FdPtr = NULL;
		FdRgn = TREGION4D();
	}
} TFdRect, *PFdRect; 
//-------------------------------------------------------------------------------------//
class CAOIFd : public CAOIRgn  
{
	//---------------------------------------------------------------------------------//	
	DECLARE_DYNAMIC(CAOIFd)
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//
	static CRITICAL_SECTION    m_csFd;//同步機制-關鍵區間
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	static void                InitialFdLock();//初始化定位點的關鍵區間
	static void                DeleteFdLock(); //刪除定位點的關鍵區間
	static void                LockFd();       //進入定位點的關鍵區間
	static void                UnlockFd();     //離開定位點的關鍵區間
	//---------------------------------------------------------------------------------//
private:
	//---------------------------------------------------------------------------------//	
	bool                       m_FdSelected;//定位點是否選取到
	bool                       m_FdDeleted;//定位點是否刪除	
	int                        m_FdGroupID;//定位點的群組編號
	int                        m_FdUniqueID;//定位點唯一碼	
	bool                       m_FdNeedCalcMap;//定位點需要計算作標轉換	
	//---------------------------------------------------------------------------------//
	double                     m_FdMinScore;//定位點最低相似度
	//---------------------------------------------------------------------------------//
	CAOIModel                  m_FdModel;  //定位點模組	
	//---------------------------------------------------------------------------------//	
	int                        m_FdTempInt[4];//定位點暫存整數	
	//---------------------------------------------------------------------------------//
	unsigned int               m_FdSortID;     //定位點檢測順序	
	UINT                       m_FdConfirmUIResultID; //定位點確認結果編號(視窗的IDOK, IDCancel)
	//---------------------------------------------------------------------------------//
	CString                    m_FdImageNameORG[FD_MAX_PATTERN];//定位點圖檔名稱-原圖
	CString                    m_FdImageNameMSK[FD_MAX_PATTERN];//定位點圖檔名稱-遮罩圖
	//---------------------------------------------------------------------------------//
	IMAGE_PTR                  m_FdPatternPtr[FD_MAX_PATTERN];      //定位點影像資料
	IMAGE_SIZE                 m_FdPatternW[FD_MAX_PATTERN];     //定位點影像寬度
	IMAGE_SIZE                 m_FdPatternH[FD_MAX_PATTERN];     //定位點影像高度
	IMAGE_SIZE                 m_FdPatternStep[FD_MAX_PATTERN];  //定位點影像步長
	IMAGE_SIZE                 m_FdBitCount[FD_MAX_PATTERN];   //定位點影像位元數
	//---------------------------------------------------------------------------------//		
	TSIZE2D                    m_FdPatExtendSize;//定位點樣板外擴尺寸	
	TPOINT3D                   m_FdTeachStagePos;//定位點教導時機台座標		
	double                     m_FdSpaceBasePlane;//定位點空間基準面
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//	
	void                       PreInitFd();
	void                       InitialFd();
	void                       CloneFd(const CAOIFd &fd);	
	void                       CloneFdImage(const CAOIFd &fd);	
	//---------------------------------------------------------------------------------//
	bool                       CheckPatternIdx(unsigned int idx);
	//---------------------------------------------------------------------------------//
	void                       ReleaseFdPatternBuffer(unsigned int idx);
	void                       ReleaseFdAllImageBuffer();
	//---------------------------------------------------------------------------------//			
public:
	//---------------------------------------------------------------------------------//
	CAOIFd();
	CAOIFd(const CAOIFd &fd);
	virtual ~CAOIFd();
	CAOIFd& operator=(const CAOIFd &fd);
	//---------------------------------------------------------------------------------//
	CAOIFd*                    CloneFdObj() const;//建立且複製一個定位點
	//---------------------------------------------------------------------------------//
	bool                       WriteFdFile(CAOIFileIO &FileIO);//儲存定位點
	bool                       ReadFdFile(CAOIFileIO &FileIO);//載入定位點
	//---------------------------------------------------------------------------------//	
	bool                       WriteFdSpcFile_JSON_VRS(FILE *pfile, CAOIProject *ProjectPtr);	
	bool                       WriteFdSpcFile_JSON_RSM(FILE *pfile, CAOIProject *ProjectPtr);	
	//---------------------------------------------------------------------------------//
	void                       SetFdIndex_Project(unsigned int value) { SetRgnIndex_Project(value); }
	unsigned int               GetFdIndex_Project() const { return GetRgnIndex_Project(); }
	//---------------------------------------------------------------------------------//		
	void                       SetFdIndex_Panel(unsigned int value) { SetRgnIndex_Panel(value); }
	unsigned int               GetFdIndex_Panel() const { return GetRgnIndex_Panel(); }
	//---------------------------------------------------------------------------------//
	void                       SetFdIndex_Board(unsigned int value) { SetRgnIndex_Board(value); }
	unsigned int               GetFdIndex_Board() const { return GetRgnIndex_Board(); }
	//---------------------------------------------------------------------------------//
	void                       SetFdProjectPtr(CAOIProject *value) { SetRgnProjectPtr(value); }
	CAOIProject*               GetFdProjectPtr() const { return GetRgnProjectPtr(); }
	//---------------------------------------------------------------------------------//
	void                       SetFdPanelPtr(CAOIPanel *value) { SetRgnPanelPtr(value); }
	CAOIPanel*                 GetFdPanelPtr() const { return GetRgnPanelPtr(); }
	//---------------------------------------------------------------------------------//	
	void                       SetFdPanelIndex_Project(unsigned int value) { SetRgnPanelIndex_Project(value); }
	unsigned int               GetFdPanelIndex_Project() const { return GetRgnPanelIndex_Project(); }
	//---------------------------------------------------------------------------------//
	void                       SetFdBoardPtr(CAOIBoard *value) { SetRgnBoardPtr(value); }
	CAOIBoard*                 GetFdBoardPtr() const { return GetRgnBoardPtr(); }
	//---------------------------------------------------------------------------------//	
	void                       SetFdBoardIndex_Project(unsigned int value) { SetRgnBoardIndex_Project(value); }
	unsigned int               GetFdBoardIndex_Project() const { return GetRgnBoardIndex_Project(); }
	//---------------------------------------------------------------------------------//	
	void                       SetFdBoardIndex_Panel(unsigned int value) { SetRgnBoardIndex_Panel(value); }
	unsigned int               GetFdBoardIndex_Panel() const { return GetRgnBoardIndex_Panel(); }
	//---------------------------------------------------------------------------------//
	void                       SetFdSelected(bool value) { m_FdSelected = value; }
	bool                       GetFdSelected() const { return m_FdSelected; }
	//---------------------------------------------------------------------------------//
	void                       SetFdDeleted(bool value) { m_FdDeleted = value; }
	bool                       GetFdDeleted() const { return m_FdDeleted; }
	//---------------------------------------------------------------------------------//
	//定位點的群組編號
	void                       SetFdGroupID(int value) { m_FdGroupID = value; }
	int                        GetFdGroupID() const { return m_FdGroupID; }	
	bool                       CheckFdGroupIDValid() const;//確認定位點群組編號有效
	//---------------------------------------------------------------------------------//	
	void                       SetFdUniqueID(int value) { m_FdUniqueID = value; }
	int                        GetFdUniqueID() const { return m_FdUniqueID; }
	//---------------------------------------------------------------------------------//	
	CString                    GetFdFullName() const;//取得定位點全名
	//---------------------------------------------------------------------------------//	
	bool                       GetFdModelImageIsSaved() const;//取得定位點模組影像是否儲存
	void                       SetFdModelImageIsSaved(bool value);//設定定位點模組影像是否儲存
	//---------------------------------------------------------------------------------//
	SAVE_TEST_IMAGE_MODE       GetFdSaveTestImageMode() const;//取得定位點儲存影像模式
	void                       SetFdSaveTestImageMode(SAVE_TEST_IMAGE_MODE value);//設定定位點儲存影像模式	
	//---------------------------------------------------------------------------------//
	//定位點需要計算作標轉換
	void                       SetFdNeedCalcMap(bool value) { m_FdNeedCalcMap = value; }
	bool                       GetFdNeedCalcMap() const { return m_FdNeedCalcMap; }
	//---------------------------------------------------------------------------------//	
	void                       SetFdMinScore(double value) { m_FdMinScore = value; }
	double                     GetFdMinScore() const { return m_FdMinScore; }
	//---------------------------------------------------------------------------------//
	void                       SetFdTempInt(int value, int idx=0) { m_FdTempInt[idx] = value; }
	int                        GetFdTempInt(int idx=0) const { return m_FdTempInt[idx]; }
	//---------------------------------------------------------------------------------//
	//定位點軌道編號
	LANE_ID                    GetFdLaneID() const;
	void                       SetFdLaneID(LANE_ID value);
	//---------------------------------------------------------------------------------//
	//定位點檢測順序
	unsigned int               GetFdSortID() const;
	void                       SetFdSortID(unsigned int value);	
	//---------------------------------------------------------------------------------//
	bool                       CheckFdIsPanelFd() const;//確認是否為整板條碼
	bool                       CheckFdIsBoardFd() const;//確認是否為單板條碼
	//---------------------------------------------------------------------------------//
	//使用的OpenMP數量
	int                        CalcFdOpenMPCountByPixels();
	void                       SetFdOpenMPCount(int value);
	int                        GetFdOpenMPCount() const { return GetRgnOpenMPCount(); }
	//---------------------------------------------------------------------------------//
	void                       BypassSkipFd(RESULT_ID value);//不檢測或跳過定位點
	//---------------------------------------------------------------------------------//	
	RESULT_ID                  GetFdResultID_AOI() const;//取得定位點結果編號
	void                       SetFdResultID_AOI(RESULT_ID value);//設定定位點結果編號	
	//---------------------------------------------------------------------------------//	
	RESULT_ID                  GetFdResultID_AOI_LA() const;//取得定位點結果編號-A軌
	void                       SetFdResultID_AOI_LA(RESULT_ID value);//設定定位點結果編號-A軌
	//---------------------------------------------------------------------------------//	
	RESULT_ID                  GetFdResultID_AOI_LB() const;//取得定位點結果編號-B軌
	void                       SetFdResultID_AOI_LB(RESULT_ID value);//設定定位點結果編號-B軌
	//---------------------------------------------------------------------------------//	
	void                       UpdateFdResultID_AOI_Lane(LANE_ID LaneID);//更新定位點結果編號-軌道
	RESULT_ID                  GetFdResultID_AOI_Lane(LANE_ID LaneID) const;//取得定位點結果編號-軌道
	//---------------------------------------------------------------------------------//	
	RESULT_ID                  GetFdResultID_ARS() const;//取得定位點結果編號
	void                       SetFdResultID_ARS(RESULT_ID value);//設定定位點結果編號
	//---------------------------------------------------------------------------------//	
	RESULT_ID                  GetFdResultID_ARS_LA() const;//取得定位點結果編號-A軌
	void                       SetFdResultID_ARS_LA(RESULT_ID value);//設定定位點結果編號-A軌
	//---------------------------------------------------------------------------------//	
	RESULT_ID                  GetFdResultID_ARS_LB() const;//取得定位點結果編號-B軌
	void                       SetFdResultID_ARS_LB(RESULT_ID value);//設定定位點結果編號-B軌
	//---------------------------------------------------------------------------------//	
	void                       UpdateFdResultID_ARS_Lane(LANE_ID LaneID);//更新定位點結果編號-軌道
	RESULT_ID                  GetFdResultID_ARS_Lane(LANE_ID LaneID) const;//取得定位點結果編號-軌道	
	//---------------------------------------------------------------------------------//	
	RESULT_ID                  GetFdResultID_Alarm() const;//取得定位點結果編號-停機
	void                       SetFdResultID_Alarm(RESULT_ID value);//設定定位點結果編號-停機
	//---------------------------------------------------------------------------------//	
	//定位點確認結果編號(視窗的IDOK, IDCancel)
	void                       SetFdConfirmUIResultID(UINT value) { m_FdConfirmUIResultID = value; }
	UINT                       GetFdConfirmUIResultID() const { return m_FdConfirmUIResultID; }
	//---------------------------------------------------------------------------------//	
	//定位點保留影像
	void                       SetFdKeepImage(bool value) { m_RgnKeepImage = value; }
	bool	                   GetFdKeepImage() const { return m_RgnKeepImage; }
	//---------------------------------------------------------------------------------//
	//定位點填滿畫面的時間
	void                       SetFdFillImageTime(double value) { m_RgnFillImageTime = value; }
	double                     GetFdFillImageTime() const { return m_RgnFillImageTime; }
	//影像編號
	void                       SetFdFrameIndex(unsigned int value) { SetRgnFrameIndex(value); }
	unsigned int               GetFdFrameIndex() const { return GetRgnFrameIndex(); }
	//---------------------------------------------------------------------------------//
	void                       SetFdFrameUniqueID(unsigned int value) { SetRgnFrameUniqueID(value); }
	unsigned int               GetFdFrameUniqueID() const { return GetRgnFrameUniqueID(); }
	//---------------------------------------------------------------------------------//
	//相機編號
	void                       SetFdCameraID(CAMERA_ID value) { m_RgnCameraID = value; }
	CAMERA_ID                  GetFdCameraID() const { return m_RgnCameraID; }
	//---------------------------------------------------------------------------------//
	//燈源模式
	void                       SetFdLightMode(LIGHT_MODE value) { m_RgnLightMode = value; }
	LIGHT_MODE                 GetFdLightMode() const { return m_RgnLightMode; }
	//---------------------------------------------------------------------------------//
	//分段編號//兩段式檢測
	void                       SetFdDistrictID(DISTRICT_ID value) { m_RgnDistrictID = value; }
	DISTRICT_ID                GetFdDistrictID() const { return m_RgnDistrictID; }
	//---------------------------------------------------------------------------------//
	//要去計算
	void                       SetFdNeedToCalculate(bool value) { CAOIRgn::SetRgnNeedToCalculate(value); }
	bool                       GetFdNeedToCalculate() const { return m_RgnNeedToCalculate; }
	//---------------------------------------------------------------------------------//
	//要去計算-備份檔
	void                       SetFdNeedToCalculateBackup(bool value) { CAOIRgn::SetRgnNeedToCalculateBackup(value); }
	bool                       GetFdNeedToCalculateBackup() const { return m_RgnNeedToCalculateBackup; }
	//---------------------------------------------------------------------------------//
	//定位點角度
	void                       SetFdAngle(double value) { m_RgnAngle = value; }
	double                     GetFdAngle() const { return m_RgnAngle; }
	//---------------------------------------------------------------------------------//
	//定位點尺寸寬
	void                       SetFdRoiSizeW(double value) { m_RgnRoiSize.cx = value; }
	double                     GetFdRoiSizeW() const { return m_RgnRoiSize.cx; }
	//---------------------------------------------------------------------------------//
	//定位點尺寸長
	void                       SetFdRoiSizeH(double value) { m_RgnRoiSize.cy = value; }
	double                     GetFdRoiSizeH() const { return m_RgnRoiSize.cy; }
	//---------------------------------------------------------------------------------//
	//定位點本體尺寸寬
	void                       SetFdBodySizeW(double value) { m_RgnBodySize.cx = value; }
	double                     GetFdBodySizeW() const { return m_RgnBodySize.cx; }
	//---------------------------------------------------------------------------------//
	//定位點本體尺寸長
	void                       SetFdBodySizeH(double value) { m_RgnBodySize.cy = value; }
	double                     GetFdBodySizeH() const { return m_RgnBodySize.cy; }
	//---------------------------------------------------------------------------------//
	//定位點位置在CAD坐標系中
	void                       SetFdCadPos(const TPOINT2D &value) { m_RgnCadPos=value; }
	TPOINT2D                   GetFdCadPos() const { return m_RgnCadPos; }
	//---------------------------------------------------------------------------------//
	//定位點位置在CAD坐標系中-X
	void                       SetFdCadPosX(double value) { m_RgnCadPos.x = value; }
	double                     GetFdCadPosX() const { return m_RgnCadPos.x; }
	//---------------------------------------------------------------------------------//
	//定位點位置在CAD坐標系中-Y
	void                       SetFdCadPosY(double value) { m_RgnCadPos.y = value; }
	double                     GetFdCadPosY() const { return m_RgnCadPos.y; }
	//---------------------------------------------------------------------------------//
	//定位點位置在以PCB左下為原點的CAD坐標系中
	void                       SetFdSpecialCadPos(const TPOINT2D &value) { m_RgnSpecialCadPos = value; }
	TPOINT2D                   GetFdSpecialCadPos() const { return m_RgnSpecialCadPos; }
	//---------------------------------------------------------------------------------//
	//定位點位置在以PCB左下為原點的CAD坐標系中-X
	void                       SetFdSpecialCadPosX(double value) { m_RgnSpecialCadPos.x = value; }
	double                     GetFdSpecialCadPosX() const { return m_RgnSpecialCadPos.x; }
	//---------------------------------------------------------------------------------//
	//定位點位置在以PCB左下為原點的CAD坐標系中-Y
	void                       SetFdSpecialCadPosY(double value) { m_RgnSpecialCadPos.y = value; }
	double                     GetFdSpecialCadPosY() const { return m_RgnSpecialCadPos.y; }
	//---------------------------------------------------------------------------------//	
	//定位點四端點X
	void                       SetFdCadCornerPosX(const double value[]) 
	{ 
		m_RgnRoiCadCornerPos[0].x = value[0]; 
		m_RgnRoiCadCornerPos[1].x = value[1]; 
		m_RgnRoiCadCornerPos[2].x = value[2]; 
		m_RgnRoiCadCornerPos[3].x = value[3]; 
	}
	void                      GetFdCadCornerPosX(double value[]) const 
	{ 
		value[0] = m_RgnRoiCadCornerPos[0].x; 
		value[1] = m_RgnRoiCadCornerPos[1].x; 
		value[2] = m_RgnRoiCadCornerPos[2].x; 
		value[3] = m_RgnRoiCadCornerPos[3].x; 
	}
	//---------------------------------------------------------------------------------//
	//定位點四端點Y
	void                       SetFdCadCornerPosY(const double value[]) 
	{ 
		m_RgnRoiCadCornerPos[0].y = value[0]; 
		m_RgnRoiCadCornerPos[1].y = value[1]; 
		m_RgnRoiCadCornerPos[2].y = value[2]; 
		m_RgnRoiCadCornerPos[3].y = value[3]; 
	}
	void                       GetFdCadCornerPosY(double value[]) const 
	{ 
		value[0] = m_RgnRoiCadCornerPos[0].y; 
		value[1] = m_RgnRoiCadCornerPos[1].y; 
		value[2] = m_RgnRoiCadCornerPos[2].y; 
		value[3] = m_RgnRoiCadCornerPos[3].y; 
	}
	//---------------------------------------------------------------------------------//	
	//定位點位置在機台坐標系中
	void                       SetFdStagePos(const TPOINT3D &value) { m_RgnStagePos=value; }
	TPOINT3D                   GetFdStagePos() const { return m_RgnStagePos; }
	//---------------------------------------------------------------------------------//	
	//定位點位置在機台坐標系中-X	
	void                       SetFdStagePosX(double value) { m_RgnStagePos.x = value; }
	double                     GetFdStagePosX() const { return m_RgnStagePos.x; }
	//---------------------------------------------------------------------------------//
	//定位點位置在機台坐標系中-Y
	void                       SetFdStagePosY(double value) { m_RgnStagePos.y = value; }
	double                     GetFdStagePosY() const { return m_RgnStagePos.y; }
	//定位點位置在機台坐標系中-Z
	void                       SetFdStagePosZ(double value) { m_RgnStagePos.z = value; }
	double                     GetFdStagePosZ() const { return m_RgnStagePos.z; }
	//---------------------------------------------------------------------------------//
	//定位點搜尋外擴尺寸
	void                       SetFdRoiSize(const TSIZE2D &value) { m_RgnRoiSize=value; }
	TSIZE2D                    GetFdRoiSize() const { return m_RgnRoiSize; }

	void                       SetFdRoiExtendSizeW(double value) { m_RgnRoiSize.cx = value; }
	double                     GetFdRoiExtendSizeW() const { return m_RgnRoiSize.cx; }	

	void                       SetFdRoiExtendSizeH(double value) { m_RgnRoiSize.cy = value; }
	double                     GetFdRoiExtendSizeH() const { return m_RgnRoiSize.cy; }	
	//---------------------------------------------------------------------------------//	
	//定位點樣板外擴尺寸
	void                       SetFdPatExtendSize(const TSIZE2D &value) { m_FdPatExtendSize=value; }
	TSIZE2D                    GetFdPatExtendSize() const { return m_FdPatExtendSize; }

	void                       SetFdPatExtendSizeW(double value) { m_FdPatExtendSize.cx = value; }
	double                     GetFdPatExtendSizeW() const { return m_FdPatExtendSize.cx; }	

	void                       SetFdPatExtendSizeH(double value) { m_FdPatExtendSize.cy = value; }
	double                     GetFdPatExtendSizeH() const { return m_FdPatExtendSize.cy; }	
	//---------------------------------------------------------------------------------//
	//定位點影像結果位置
	void                       GetFdImageResultPos(TPOINT2D &value) const;
	void                       SetFdImageResultPos(const TPOINT2D &value);	
	//---------------------------------------------------------------------------------//
	//定位點教導時在機台座標
	const TPOINT3D&            GetFdTeachStagePos() const;	
	void                       GetFdTeachStagePos(TPOINT2D &value) const;
	void                       SetFdTeachStagePos(const TPOINT2D &value);
	void                       GetFdTeachStagePos(TPOINT3D &value) const;
	void                       SetFdTeachStagePos(const TPOINT3D &value);
	//---------------------------------------------------------------------------------//	
	double                     GetFdTeachStagePosX() const;
	void                       SetFdTeachStagePosX(double value);
	double                     GetFdTeachStagePosY() const;
	void                       SetFdTeachStagePosY(double value);
	double                     GetFdTeachStagePosZ() const;
	void                       SetFdTeachStagePosZ(double value);
	//---------------------------------------------------------------------------------//		
	void                       GetFdRoiStageCornerPos(TPOINT2D value[]) const 
	{
		value[0].x = m_RgnRoiStageCornerPos[0].x;	value[0].y = m_RgnRoiStageCornerPos[0].y; 	
		value[1].x = m_RgnRoiStageCornerPos[1].x;	value[1].y = m_RgnRoiStageCornerPos[1].y; 	
		value[2].x = m_RgnRoiStageCornerPos[2].x;	value[2].y = m_RgnRoiStageCornerPos[2].y; 	
		value[3].x = m_RgnRoiStageCornerPos[3].x;	value[3].y = m_RgnRoiStageCornerPos[3].y; 	
	}
	void                       GetFdBodyStageCornerPos(TPOINT2D value[]) const 
	{
		value[0].x = m_RgnBodyStageCornerPos[0].x;	value[0].y = m_RgnBodyStageCornerPos[0].y; 	
		value[1].x = m_RgnBodyStageCornerPos[1].x;	value[1].y = m_RgnBodyStageCornerPos[1].y; 	
		value[2].x = m_RgnBodyStageCornerPos[2].x;	value[2].y = m_RgnBodyStageCornerPos[2].y; 	
		value[3].x = m_RgnBodyStageCornerPos[3].x;	value[3].y = m_RgnBodyStageCornerPos[3].y; 	
	}
	//---------------------------------------------------------------------------------//
	//定位點四端點在機台坐標系中-X
	void                       SetFdRoiStageCornerPosX(double value[]) 
	{ 
		m_RgnRoiStageCornerPos[0].x = value[0]; 
		m_RgnRoiStageCornerPos[1].x = value[1]; 
		m_RgnRoiStageCornerPos[2].x = value[2]; 
		m_RgnRoiStageCornerPos[3].x = value[3]; 
	}
	void                       GetFdRoiStageCornerPosX(double value[]) const 
	{ 
		value[0] = m_RgnRoiStageCornerPos[0].x; 
		value[1] = m_RgnRoiStageCornerPos[1].x; 
		value[2] = m_RgnRoiStageCornerPos[2].x; 
		value[3] = m_RgnRoiStageCornerPos[3].x; 
	}	
	//定位點四端點在機台坐標系中-Y
	void                       SetFdRoiStageCornerPosY(double value[]) 
	{ 
		m_RgnRoiStageCornerPos[0].y = value[0]; 
		m_RgnRoiStageCornerPos[1].y = value[1]; 
		m_RgnRoiStageCornerPos[2].y = value[2]; 
		m_RgnRoiStageCornerPos[3].y = value[3]; 
	}
	void                       GetFdRoiStageCornerPosY(double value[]) const 
	{ 
		value[0] = m_RgnRoiStageCornerPos[0].y; 
		value[1] = m_RgnRoiStageCornerPos[1].y; 
		value[2] = m_RgnRoiStageCornerPos[2].y; 
		value[3] = m_RgnRoiStageCornerPos[3].y; 
	}
	//---------------------------------------------------------------------------------//
	TREGION4D                  GetFdRgnCad() const;//定位點範圍-Cad
	TREGION4D                  GetFdRgnStage() const;//定位點範圍-Stage
	TREGION4D                  GetFdExtendRgnCad() const;//定位點搜尋範圍-Cad
	TREGION4D                  GetFdExtendRgnStage() const;//定位點搜尋範圍-Stage
	//---------------------------------------------------------------------------------//		
	void                       GetFdRoiCadRegion(TREGION4D &Region);//取得定位點在Cad的範圍	
	void                       GetFdBodyCadRegion(TREGION4D &Region);//取得定位點在Cad的範圍	
	void                       GetFdRoiStageRegion(TREGION4D &Region);//取得定位點在Stage的範圍		
	void                       GetFdBodyStageRegion(TREGION4D &Region);//取得定位點在Stage的範圍		

	void                       GetFdExtendCadRegion(TREGION4D &Region);//取得定位點在Cad的搜尋範圍
	void                       GetFdExtendStageRegion(TREGION4D &Region);//取得定位點在Stage的搜尋範圍		
	//---------------------------------------------------------------------------------//			
	void                       MoveFdCadPos(double dX, double dY);//移動定位點座標
	void                       MoveFdStagePos(double dX, double dY);//移動定位點座標
	void                       MoveFdPos(double dX, double dY, CMapCoordinate *MapPtr);//移動定位點座標			
	//---------------------------------------------------------------------------------//
	void                       ModifyFdRoiRegion(const TREGION4D &dRgn);//修正定位點尺寸
	void                       ModifyFdBodyRegion(const TREGION4D &dRgn);//修正定位點尺寸
	//---------------------------------------------------------------------------------//		
	void                       CalcFdCadCornerPos();//計算定位點Cad端點座標	
	void                       LayoutFdStageCornerPos();//更新定位點機台端點座標
	void                       CalcFdExtendCadCornerPos(TPOINT2D CornerPt[]) const;//計算定位點外擴Cad端點座標
	void                       CalcFdExtendStageCornerPos(TPOINT2D CornerPt[])  const;//計算定位點外擴機台端點座標
	//---------------------------------------------------------------------------------//
	//定位點所屬FOV的CAD位置-X
	void                       SetFdFovCadPosX(double value) { m_RgnFovCadPos.x = value; }
	double                     GetFdFovCadPosX() const { return m_RgnFovCadPos.x; }
	//---------------------------------------------------------------------------------//
	//定位點所屬FOV的CAD位置-Y
	void                       SetFdFovCadPosY(double value) { m_RgnFovCadPos.y = value; }
	double                     GetFdFovCadPosY() const { return m_RgnFovCadPos.y; }
	//---------------------------------------------------------------------------------//
	//定位點所屬FOV的Stage位置-X
	void                       SetFdFovStagePosX(double value) { m_RgnFovStagePos.x = value; }
	double                     GetFdFovStagePosX() const { return m_RgnFovStagePos.x; }
	//---------------------------------------------------------------------------------//
	//定位點所屬FOV的Stage位置-Y
	void                       SetFdFovStagePosY(double value) { m_RgnFovStagePos.y = value; }
	double                     GetFdFovStagePosY() const { return m_RgnFovStagePos.y; }
	//---------------------------------------------------------------------------------//
	//定位點所屬影像的區域
	void                       SetFdFrameImageRect(const RECT &value) { m_RgnFrameImageRect = value; }
	const RECT&                GetFdFrameImageRect() const { return m_RgnFrameImageRect; }
	//---------------------------------------------------------------------------------//	
	//定位點影像的物理尺寸
	void                       SetFdFrameImageSize_um(const TSIZE2D &value);
	const TSIZE2D&             GetFdFrameImageSize_um() const { return m_RgnFrameImageSize_um; }
	//---------------------------------------------------------------------------------//	
	//定位點影像的區域Cad偏差-um
	void                       SetFdFrameImageCadOffset_um(const TPOINT2D &value);			
	const TPOINT2D&            GetFdFrameImageCadOffset_um() const { return m_RgnFrameImageCadOffset_um; }
	void                       SetFdFrameImageStageOffset_um(const TPOINT2D &value);
	//---------------------------------------------------------------------------------//	
	void                       SetFdFrameImageCornerPt(const POINT value[]) 
	{ 
		m_RgnFrameImageCornerPt[0] = value[0]; 
		m_RgnFrameImageCornerPt[1] = value[1]; 
		m_RgnFrameImageCornerPt[2] = value[2]; 
		m_RgnFrameImageCornerPt[3] = value[3]; 
	}
	void                       GetFdFrameImageCornerPt(POINT value[]) const 
	{ 
		value[0] = m_RgnFrameImageCornerPt[0]; 
		value[1] = m_RgnFrameImageCornerPt[1]; 
		value[2] = m_RgnFrameImageCornerPt[2]; 
		value[3] = m_RgnFrameImageCornerPt[3]; 
	}
	//---------------------------------------------------------------------------------//	
	bool                       CheckFdBePickByCad(const TPOINT2D &PickPos);//確認定位點被點擊到
	bool                       CheckFdBePickByStage(const TPOINT2D &PickPos);//確認定位點被點擊到

	bool                       CheckFdInRegionByCad(const TREGION4D &SelRgn, bool bEntireIn);//確認定位點在範圍內
	bool                       CheckFdInRegionByStage(const TREGION4D &SelRgn, bool bEntireIn);//確認定位點在範圍內
	//---------------------------------------------------------------------------------//	
	bool                       CreateFdSelfFieldPtr();//建立專屬Field指標	
	CAOIField*                 GetFdSelfFieldPtr() const { return m_RgnSelfFieldPtr; }
	//---------------------------------------------------------------------------------//
	void                       SetFdSelfFieldEnabeld(bool val) { m_RgnSelfFieldEnabled=val; }
	bool                       GetFdSelfFieldEnabled() const { return m_RgnSelfFieldEnabled; }	
	//---------------------------------------------------------------------------------//	
	CAOIWnd*                   GetFdWndPtr();//取得定位點檢測框
	//---------------------------------------------------------------------------------//	
	void                       SetFdFieldPtr(CAOIField* Ptr);
	CAOIField*                 GetFdFieldPtr() const { return m_RgnFieldPtr; }
	//---------------------------------------------------------------------------------//	
	void                       SetFdFieldIndex(unsigned int value) { m_RgnFieldIdx = value; }
	unsigned int               GetFdFieldIndex() const { return m_RgnFieldIdx; }
	//---------------------------------------------------------------------------------//	
	//定位點所屬的Field的CAD位置-X
	void                       SetFdFieldCadPosX(double value) { m_RgnFieldCadPos.x = value; }
	double                     GetFdFieldCadPosX() const { return m_RgnFieldCadPos.x; }
	//---------------------------------------------------------------------------------//
	//定位點所屬的Field的CAD位置-Y
	void                       SetFdFieldCadPosY(double value) { m_RgnFieldCadPos.y = value; }
	double                     GetFdFieldCadPosY() const { return m_RgnFieldCadPos.y; }
	//---------------------------------------------------------------------------------//
	//定位點所屬的Field的Stage位置-X
	void                       SetFdFieldStagePosX(double value) { m_RgnFieldStagePos.x = value; }
	double                     GetFdFieldStagePosX() const { return m_RgnFieldStagePos.x; }
	//---------------------------------------------------------------------------------//
	//定位點所屬的Field的Stage位置-Y
	void                       SetFdFieldStagePosY(double value) { m_RgnFieldStagePos.y = value; }
	double                     GetFdFieldStagePosY() const { return m_RgnFieldStagePos.y; }
	//---------------------------------------------------------------------------------//
	void                       MapFdCadToStagePos(const CMapCoordinate &Map);//將CAD轉成機台座標
	//---------------------------------------------------------------------------------//
	bool                       SpinFd(double Angle, CMapCoordinate *MapPtr);//定位點自旋轉
	bool                       RotateFd(double Angle, double CpX, double CpY, CMapCoordinate *MapPtr);//定位點旋轉	
	bool                       MirrorXFd(double CpX, CMapCoordinate *MapPtr);//定位點鏡射-X
	bool                       MirrorYFd(double CpY, CMapCoordinate *MapPtr);//定位點鏡射-Y	
	//---------------------------------------------------------------------------------//
	void                       SetFdImageNameORG(unsigned int idx, LPCTSTR filename);
	LPCTSTR                    GetFdImageNameORG(unsigned int idx) const;
	//---------------------------------------------------------------------------------//	
	void                       SetFdImageNameMSK(unsigned int idx, LPCTSTR filename);
	LPCTSTR                    GetFdImageNameMSK(unsigned int idx) const;
	//---------------------------------------------------------------------------------//	
	bool                       SetFdPattern(unsigned int idx, IMAGE_SIZE PatW, IMAGE_SIZE PatH, IMAGE_SIZE PatStep, IMAGE_SIZE BitCount, IMAGE_PTR PatPtr, bool Invert);
	bool                       GetFdPattern(unsigned int idx, IMAGE_SIZE &PatW, IMAGE_SIZE &PatH, IMAGE_SIZE &PatStep, IMAGE_SIZE &BitCount, IMAGE_PTR &PatPtr);
	bool                       CloneFdPattern(unsigned int idx, IMAGE_SIZE &PatW, IMAGE_SIZE &PatH, IMAGE_SIZE &PatStep, IMAGE_SIZE &BitCount, IMAGE_PTR &PatPtr);
	bool                       CloneFdPattern3(unsigned int idx, IMAGE_SIZE &PatW, IMAGE_SIZE &PatH, IMAGE_SIZE &PatStep, IMAGE_SIZE &BitCount, IMAGE_PTR PatPtr);
	//---------------------------------------------------------------------------------//	
	bool                       SaveFdPattern(unsigned int idx, LPCTSTR filename);//儲存定位點樣版圖檔
	bool                       LoadFdPattern(unsigned int idx, LPCTSTR filename);//載入定位點樣版圖檔
	//---------------------------------------------------------------------------------//	
	bool                       FindFiducial();//尋找定位點	
	bool                       FindFiducial(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr);//尋找定位點
	//---------------------------------------------------------------------------------//
	virtual CString            GetRgnDerivedName() const;//取得定位點的名稱	
	virtual CString            GetRgnDerivedKeyName() const;//取得定位點的名稱	
	virtual bool               ExtractRgnDerivedFrame(bool &Finished);//挖取定位點圖片
	virtual bool               ExecRgnDerivedInspection();//執行定位點檢測
	//---------------------------------------------------------------------------------//
	bool                       CreateFdSubRgnList(FIELD_BUILD_MODE BuildMode, bool ByCadRegion);//建立定位點子檢測區域列表
	void                       ClearFdSubRgnList();//清除定位點的子列表
	size_t                     GetFdSubRgnCount() const;//取得定位點的子數量
	CAOIRgn*                   GetFdSubRgnPtr(size_t index, bool check) const;//取得定位點的子指標
	//---------------------------------------------------------------------------------//	
	CAOIModel*                 GetFdModelPtr();//取得定位點模組
	bool                       UpdateFdParamToModel();//更新定位點參數至模組內
	bool                       UpdateFdModelFromLibrary(CAOIModel *ModelPtr);//更新定位點模組
	//---------------------------------------------------------------------------------//
	void                       InitFdInspection(LANE_ID LaneID);//初始化定位點檢測
	bool                       ExecFdInspection();//執行定位點檢測
	bool                       CalcFdSpaceBasePlane(const std::vector<TUNI_FRAME> &UniFrameList);//計算定位點基準面
	bool                       ExecFdSaveDefectImage(const std::vector<TUNI_FRAME> &UniFrameList);//執行零件儲存瑕疵圖片	
	bool                       UpdateFdResultID();//更新定位點檢測結果
	//---------------------------------------------------------------------------------//
	bool                       ExtractFdFrame();//挖取定位點的影像
	bool                       BuildNewFd(TPOINT2D CadPos, TPOINT2D StagePos, TSIZE2D szFd, TSIZE2D szRoi, unsigned int FrameIndex, unsigned int FrameUniqueID, FRAME_TYPE FrameType, LPCTSTR FdFolder, TUNI_FRAME UniFrame);//創建新的定位點
	//---------------------------------------------------------------------------------//
	bool                       UpdateFdFrameIndex(unsigned int DefaultIndex, unsigned int DefaultUniqueID, const std::vector<unsigned int> &FrameIndexMapList);
	//---------------------------------------------------------------------------------//
	double                     GetFdSpaceBasePlane() const; 
	void                       SetFdSpaceBasePlane(double val);	
	//---------------------------------------------------------------------------------//
	//空間基準面參數
	void                       SetFdPanelBasePlane(double val);
	double                     GetFdPanelBasePlane() const { return GetRgnPanelBasePlane(); }
	//---------------------------------------------------------------------------------//
	void                       SetFdLocalBasePlaneID(int val) { SetRgnLocalBasePlaneID(val); }
	int                        GetFdLocalBasePlaneID() const { return GetRgnLocalBasePlaneID(); }
	//---------------------------------------------------------------------------------//
	void                       SetFdLocalBasePlaneFinish(bool val) { SetRgnLocalBasePlaneFinish(val); }
	bool                       GetFdLocalBasePlaneFinish() const { return GetRgnLocalBasePlaneFinish(); }
	//---------------------------------------------------------------------------------//
	void                       SetFdLocalBasePlaneParam(const TPOINT3D &val) { SetRgnLocalBasePlaneParam(val); }
	void                       GetFdLocalBasePlaneParam(TPOINT3D &val) const { GetRgnLocalBasePlaneParam(val); }
	//---------------------------------------------------------------------------------//
	TBasePlaneParam&           GetFdSpaceBasePlaneParam() { return GetRgnSpaceBasePlaneParam(); }
	const TBasePlaneParam&     GetFdSpaceBasePlaneParam() const { return GetRgnSpaceBasePlaneParam(); }
	void                       SetFdSpaceBasePlaneParam(const TBasePlaneParam& Param);
	//---------------------------------------------------------------------------------//
	//空間雜訊過濾處理
	TNoiseFilterParam&         GetFdSpaceNoiseFilterParam() { return GetRgnSpaceNoiseFilterParam(); }
	const TNoiseFilterParam&   GetFdSpaceNoiseFilterParam() const { return GetRgnSpaceNoiseFilterParam(); }
	void                       SetFdSpaceNoiseFilterParam(const TNoiseFilterParam& Param);
	//---------------------------------------------------------------------------------//	
	bool                       GetFdDataModelEnabled() const;//取得定位點資料模型啟用
	void                       SetFdDataModelEnabled(bool val);//設定定位點資料模型啟用
	//---------------------------------------------------------------------------------/
	int                        GetFdDataModelLevelID() const;//取得定位點資料模型等級
	void                       SetFdDataModelLevelID(int val);//設定定位點資料模型等級
	//---------------------------------------------------------------------------------/
};
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_AOIFD_H__6DDD663A_05EC_4E66_B07A_0374B3A413C9__INCLUDED_)
