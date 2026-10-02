// AOIBarcode.h: interface for the CAOIBarcode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AOIBARCODE_H__B666DA79_0F66_4D23_8B1C_D814C09E3082__INCLUDED_)
#define AFX_AOIBARCODE_H__B666DA79_0F66_4D23_8B1C_D814C09E3082__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include "AOIRgn.h"
#include "AOIModel.h"
//-------------------------------------------------------------------------------------//
class CAOIFileIO;
//-------------------------------------------------------------------------------------//
class CAOIBarcode  : public CAOIRgn
{
	//---------------------------------------------------------------------------------//	
	DECLARE_DYNAMIC(CAOIBarcode)
	//---------------------------------------------------------------------------------//	
private:
	//---------------------------------------------------------------------------------//
	bool                       m_BarcodeDeleted;//軟體條碼是否刪除
	bool                       m_BarcodeSelected;//軟體條碼是否選取到
	int                        m_BarcodeUniqueID;//軟體條碼唯一碼
	int                        m_BarcodeGroupID;//軟體條碼群組編號
	BARCODE_SPREAD_MODE        m_BarcodeSpreadMode;//軟體條碼擴散模式
	BARCODE_BELONG_MODE        m_BarcodeBelongMode;//軟體條碼屬於哪個	
	//---------------------------------------------------------------------------------//
	int                        m_BarcodeTempInt[4];//特徵點暫存整數	
	//---------------------------------------------------------------------------------//
	CAOIModel                  m_BarcodeModel;	
	//---------------------------------------------------------------------------------//
	UINT                       m_BarcodeConfirmUIResultID;//條碼確認視窗結果(IDOK, IDCANCEL)
	std::wstring               m_BarcodeResultText;//條碼內容
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//
	void                       PreInitBarcode();
	void                       InitialBarcode();
	void                       CloneBarcode(const CAOIBarcode &barcode);
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	CAOIBarcode();
	CAOIBarcode(const CAOIBarcode &sb);
	virtual ~CAOIBarcode();
	CAOIBarcode& operator=(const CAOIBarcode &barcode);
	//---------------------------------------------------------------------------------//
	CAOIBarcode*               CloneBarcodeObj() const;//建立且複製一個軟體條碼
	//---------------------------------------------------------------------------------//	
	bool                       WriteBarcodeFile(CAOIFileIO &FileIO);//儲存軟體條碼檔案
	bool                       ReadBarcodeFile(CAOIFileIO &FileIO);//載入軟體條碼檔案
	//---------------------------------------------------------------------------------//	
	void                       SetBarcodeIndex(unsigned int value) { SetRgnIndex(value); }
	unsigned int               GetBarcodeIndex() const { return GetRgnIndex(); }
	//---------------------------------------------------------------------------------//		
	void                       SetBarcodeIndex_Project(unsigned int value) { SetRgnIndex_Project(value); }
	unsigned int               GetBarcodeIndex_Project() const { return GetRgnIndex_Project(); }
	//---------------------------------------------------------------------------------//		
	void                       SetBarcodeIndex_Panel(unsigned int value) { SetRgnIndex_Panel(value); }
	unsigned int               GetBarcodeIndex_Panel() const { return GetRgnIndex_Panel(); }
	//---------------------------------------------------------------------------------//
	void                       SetBarcodeIndex_Board(unsigned int value) { SetRgnIndex_Board(value); }
	unsigned int               GetBarcodeIndex_Board() const { return GetRgnIndex_Board(); }
	//---------------------------------------------------------------------------------//
	void                       SetBarcodeProjectPtr(CAOIProject *value) { SetRgnProjectPtr(value); }
	CAOIProject*               GetBarcodeProjectPtr() const { return GetRgnProjectPtr(); }
	//---------------------------------------------------------------------------------//
	void                       SetBarcodePanelPtr(CAOIPanel *value) { SetRgnPanelPtr(value); }
	CAOIPanel*                 GetBarcodePanelPtr() const { return GetRgnPanelPtr(); }
	//---------------------------------------------------------------------------------//	
	void                       SetBarcodePanelIndex_Project(unsigned int value) { SetRgnPanelIndex_Project(value); }
	unsigned int               GetBarcodePanelIndex_Project() const { return GetRgnPanelIndex_Project(); }
	//---------------------------------------------------------------------------------//
	void                       SetBarcodeBoardPtr(CAOIBoard *value) { SetRgnBoardPtr(value); }
	CAOIBoard*                 GetBarcodeBoardPtr() const { return GetRgnBoardPtr(); }
	//---------------------------------------------------------------------------------//		
	void                       SetBarcodeBoardIndex_Project(unsigned int value) { SetRgnBoardIndex_Project(value); }
	unsigned int               GetBarcodeBoardIndex_Project() const { return GetRgnBoardIndex_Project(); }
	//---------------------------------------------------------------------------------//	
	void                       SetBarcodeBoardIndex_Panel(unsigned int value) { SetRgnBoardIndex_Panel(value); }
	unsigned int               GetBarcodeBoardIndex_Panel() const { return GetRgnBoardIndex_Panel(); }
	//---------------------------------------------------------------------------------//	
	bool                       CreateBarcodeSelfFieldPtr();//建立專屬Field指標	
	CAOIField*                 GetBarcodeSelfFieldPtr() const { return m_RgnSelfFieldPtr; }
	//---------------------------------------------------------------------------------//
	void                       SetBarcodeSelfFieldEnabled(bool val) { m_RgnSelfFieldEnabled=val; }
	bool                       GetBarcodeSelfFieldEnabled() const { return m_RgnSelfFieldEnabled; }	
	//---------------------------------------------------------------------------------//	
	void                       SetBarcodeFieldPtr(CAOIField *Ptr);
	CAOIField*                 GetBarcodeFieldPtr() const { return m_RgnFieldPtr; }
	//---------------------------------------------------------------------------------//		
	void                       SetBarcodeFieldIndex(unsigned int value) { m_RgnFieldIdx = value; }
	unsigned int               GetBarcodeFieldIndex() const { return m_RgnFieldIdx; }
	//---------------------------------------------------------------------------------//
	void                       SetBarcodeDeleted(bool value) { m_BarcodeDeleted = value; }
	bool                       GetBarcodeDeleted() const { return m_BarcodeDeleted; }
	//---------------------------------------------------------------------------------//
	void                       SetBarcodeSelected(bool value) { m_BarcodeSelected = value; }
	bool                       GetBarcodeSelected() const { return m_BarcodeSelected; }
	//---------------------------------------------------------------------------------//		
	bool                       GetBarcodeModelImageIsSaved() const;//條碼是否儲存過圖像
	void                       SetBarcodeModelImageIsSaved(bool val);//條碼是否儲存過圖像
	//---------------------------------------------------------------------------------//
	SAVE_TEST_IMAGE_MODE       GetBarcodeSaveTestImageMode() const;//取得條碼儲存影像模式
	void                       SetBarcodeSaveTestImageMode(SAVE_TEST_IMAGE_MODE value);	//設定條碼儲存影像模式
	//---------------------------------------------------------------------------------//
	//軟體條碼唯一碼	
	int                        GetBarcodeUniqueID() const;
	void                       SetBarcodeUniqueID(int value);		
	//---------------------------------------------------------------------------------//
	int                        GetBarcodeGroupID() const;
	void                       SetBarcodeGroupID(int value);	
	bool                       CheckBarcodeGroupIDValid() const;//確認條碼群組編號有效
	//---------------------------------------------------------------------------------//	
	//軟體條碼同層擴展(更新至同整板或單板)
	BARCODE_SPREAD_MODE        GetBarcodeSpreadMode() const;//取得條碼擴散模式
	void                       SetBarcodeSpreadMode(BARCODE_SPREAD_MODE value);//設定條碼擴散模式
	//---------------------------------------------------------------------------------//
	BARCODE_BELONG_MODE        CheckBarcodeBelongMode();//確認條碼屬於模式
	BARCODE_BELONG_MODE        GetBarcodeBelongMode() const;//取得條碼屬於模式
	void                       SetBarcodeBelongMode(BARCODE_BELONG_MODE value);//設定條碼屬於模式	
	//---------------------------------------------------------------------------------//	
	//條碼的暫存整數
	void                       SetBarcodeTempInt(int value, int idx=0) { m_BarcodeTempInt[idx] = value; }
	int                        GetBarcodeTempInt(int idx=0) const { return m_BarcodeTempInt[idx]; }	
	//---------------------------------------------------------------------------------//
	//軌道編號
	LANE_ID                    GetBarcodeLaneID() const;
	void                       SetBarcodeLaneID(LANE_ID value);	
	//---------------------------------------------------------------------------------//
	//不檢測
	void                       SetBarcodeBypassed(bool value);
	bool                       GetBarcodeBypassed() const { return CAOIRgn::GetRgnBypassed(); }	
	bool                       UpdateBarcodeBypassed(); //確認零件是否為不檢測
	//---------------------------------------------------------------------------------//
	//使用的OpenMP數量
	int                        CalcBarcodeOpenMPCountByPixels();
	void                       SetBarcodeOpenMPCount(int value);
	int                        GetBarcodeOpenMPCount() const { return GetRgnOpenMPCount(); }
	//---------------------------------------------------------------------------------//
	//條碼結果
	void                       SetBarcodeResultText(const char *value);
	void                       SetBarcodeResultText(const wchar_t *value);
	const wchar_t*             GetBarcodeResultText() const { return m_BarcodeResultText.c_str(); }
	//---------------------------------------------------------------------------------//	
	void                       BypassSkipBarcode(RESULT_ID value);//不檢測或跳過條碼
	//---------------------------------------------------------------------------------//	
	RESULT_ID                  GetBarcodeResultID_AOI() const;//取得條碼結果編號
	void                       SetBarcodeResultID_AOI(RESULT_ID value);	//設定條碼結果編號	
	//---------------------------------------------------------------------------------//	
	RESULT_ID                  GetBarcodeResultID_AOI_LA() const;//取得條碼結果編號-A軌
	void                       SetBarcodeResultID_AOI_LA(RESULT_ID value);	//設定條碼結果編號-A軌
	//---------------------------------------------------------------------------------//	
	RESULT_ID                  GetBarcodeResultID_AOI_LB() const;//取得條碼結果編號-B軌
	void                       SetBarcodeResultID_AOI_LB(RESULT_ID value);	//設定條碼結果編號-B軌
	//---------------------------------------------------------------------------------//	
	void                       UpdateBarcodeResultID_AOI_Lane(LANE_ID LaneID);	//更新條碼結果編號-軌道
	RESULT_ID                  GetBarcodeResultID_AOI_Lane(LANE_ID LaneID) const;//取得條碼結果編號-軌道	
	//---------------------------------------------------------------------------------//	
	RESULT_ID                  GetBarcodeResultID_ARS() const;//取得條碼結果編號
	void                       SetBarcodeResultID_ARS(RESULT_ID value);	//設定條碼結果編號	
	//---------------------------------------------------------------------------------//	
	RESULT_ID                  GetBarcodeResultID_ARS_LA() const;//取得條碼結果編號-A軌
	void                       SetBarcodeResultID_ARS_LA(RESULT_ID value);	//設定條碼結果編號-A軌
	//---------------------------------------------------------------------------------//	
	RESULT_ID                  GetBarcodeResultID_ARS_LB() const;//取得條碼結果編號-B軌
	void                       SetBarcodeResultID_ARS_LB(RESULT_ID value);	//設定條碼結果編號-B軌
	//---------------------------------------------------------------------------------//	
	void                       UpdateBarcodeResultID_ARS_Lane(LANE_ID LaneID);//更新條碼結果編號-軌道
	RESULT_ID                  GetBarcodeResultID_ARS_Lane(LANE_ID LaneID) const;//取得條碼結果編號-軌道	
	//---------------------------------------------------------------------------------//	
	RESULT_ID                  GetBarcodeResultID_Alarm() const;//取得條碼結果編號-警報
	void                       SetBarcodeResultID_Alarm(RESULT_ID value);//設定條碼結果編號-警報
	//---------------------------------------------------------------------------------//
	//條碼確認視窗結果(IDOK, IDCANCEL)
	void                       SetBarcodeConfirmUIResultID(UINT value) { m_BarcodeConfirmUIResultID = value; }
	UINT                       GetBarcodeConfirmUIResultID() const { return m_BarcodeConfirmUIResultID; }
	//---------------------------------------------------------------------------------//	
	//條碼保留影像
	void                       SetBarcodeKeepImage(bool value) { m_RgnKeepImage = value; }
	bool	                   GetBarcodeKeepImage() const { return m_RgnKeepImage; }
	//---------------------------------------------------------------------------------//	
	//條碼填滿畫面的時間
	void                       SetBarcodeFillImageTime(double value) { m_RgnFillImageTime = value; }
	double                     GetBarcodeFillImageTime() const { return m_RgnFillImageTime; }
	//---------------------------------------------------------------------------------//
	//影像序號
	void                       SetBarcodeFrameIndex(unsigned int value) { SetRgnFrameIndex(value); }
	unsigned int               GetBarcodeFrameIndex() const { return GetRgnFrameIndex(); }
	//---------------------------------------------------------------------------------//
	//影像唯一碼
	void                       SetBarcodeFrameUniqueID(unsigned int value) { SetRgnFrameUniqueID(value); }
	unsigned int               GetBarcodeFrameUniqueID() const { return GetRgnFrameUniqueID(); }
	//---------------------------------------------------------------------------------//
	//相機編號
	void                       SetBarcodeCameraID(CAMERA_ID value) { m_RgnCameraID = value; }
	CAMERA_ID                  GetBarcodeCameraID() const { return m_RgnCameraID; }
	//---------------------------------------------------------------------------------//
	//燈源模式
	void                       SetBarcodeLightMode(LIGHT_MODE value) { m_RgnLightMode = value; }
	LIGHT_MODE                 GetBarcodeLightMode() const { return m_RgnLightMode; }
	//---------------------------------------------------------------------------------//
	//分段編號//兩段式檢測
	void                       SetBarcodeDistrictID(DISTRICT_ID value) { m_RgnDistrictID = value; }
	DISTRICT_ID                GetBarcodeDistrictID() const { return m_RgnDistrictID; }
	//---------------------------------------------------------------------------------//
	//要去計算
	void                       SetBarcodeNeedToCalculate(bool value) { CAOIRgn::SetRgnNeedToCalculate(value); }
	bool                       GetBarcodeNeedToCalculate() const { return m_RgnNeedToCalculate; }
	//---------------------------------------------------------------------------------//
	//要去計算-備份檔
	void                       SetBarcodeNeedToCalculateBackup(bool value) { CAOIRgn::SetRgnNeedToCalculateBackup(value); }
	bool                       GetBarcodeNeedToCalculateBackup() const { return m_RgnNeedToCalculateBackup; }
	//---------------------------------------------------------------------------------//
	//軟體條碼角度
	void                       SetBarcodeAngle(double value) { m_RgnAngle = value; }
	double                     GetBarcodeAngle() const { return m_RgnAngle; }
	//---------------------------------------------------------------------------------//
	//軟體條碼尺寸寬
	void                       SetBarcodeRoiSizeW(double value) { m_RgnRoiSize.cx = value; }
	double                     GetBarcodeRoiSizeW() const { return m_RgnRoiSize.cx; }
	//---------------------------------------------------------------------------------//
	//軟體條碼尺寸長
	void                       SetBarcodeRoiSizeH(double value) { m_RgnRoiSize.cy = value; }
	double                     GetBarcodeRoiSizeH() const { return m_RgnRoiSize.cy; }
	//---------------------------------------------------------------------------------//
	//軟體條碼尺寸寬
	void                       SetBarcodeBodySizeW(double value) { m_RgnBodySize.cx = value; }
	double                     GetBarcodeBodySizeW() const { return m_RgnBodySize.cx; }
	//---------------------------------------------------------------------------------//
	//軟體條碼尺寸長
	void                       SetBarcodeBodySizeH(double value) { m_RgnBodySize.cy = value; }
	double                     GetBarcodeBodySizeH() const { return m_RgnBodySize.cy; }
	//---------------------------------------------------------------------------------//
	//軟體條碼位置在CAD坐標系中
	void                       SetBarcodeCadPos(const TPOINT2D &value) { m_RgnCadPos = value; }
	TPOINT2D                   GetBarcodeCadPos() const { return m_RgnCadPos; }
	//---------------------------------------------------------------------------------//
	//軟體條碼位置在CAD坐標系中-X
	void                       SetBarcodeCadPosX(double value) { m_RgnCadPos.x = value; }
	double                     GetBarcodeCadPosX() const { return m_RgnCadPos.x; }
	//---------------------------------------------------------------------------------//
	//軟體條碼位置在CAD坐標系中-Y
	void                       SetBarcodeCadPosY(double value) { m_RgnCadPos.y = value; }
	double                     GetBarcodeCadPosY() const { return m_RgnCadPos.y; }
	//---------------------------------------------------------------------------------//		
	CAOIWnd*                   GetBarcodeWndPtr();//取得條碼檢測框指標
	//---------------------------------------------------------------------------------//		
	WND_LOGIC_TYPE             GetBarcodeLogicType();//檢測框邏輯樣式	
	//---------------------------------------------------------------------------------//		
	int                        GetBarcodeLogicGroupID();//檢測框邏輯群組編號	
	//---------------------------------------------------------------------------------//
	//軟體條碼四端點X
	void                       SetBarcodeCadCornerPosX(const double value[]) 
	{ 
		m_RgnRoiCadCornerPos[0].x = value[0]; 
		m_RgnRoiCadCornerPos[1].x = value[1]; 
		m_RgnRoiCadCornerPos[2].x = value[2]; 
		m_RgnRoiCadCornerPos[3].x = value[3]; 
	}
	void                      GetBarcodeCadCornerPosX(double value[]) const 
	{ 
		value[0] = m_RgnRoiCadCornerPos[0].x; 
		value[1] = m_RgnRoiCadCornerPos[1].x; 
		value[2] = m_RgnRoiCadCornerPos[2].x; 
		value[3] = m_RgnRoiCadCornerPos[3].x; 
	}
	//---------------------------------------------------------------------------------//
	//軟體條碼四端點Y
	void                       SetBarcodeCadCornerPosY(const double value[]) 
	{ 
		m_RgnRoiCadCornerPos[0].y = value[0]; 
		m_RgnRoiCadCornerPos[1].y = value[1]; 
		m_RgnRoiCadCornerPos[2].y = value[2]; 
		m_RgnRoiCadCornerPos[3].y = value[3]; 
	}
	void                       GetBarcodeCadCornerPosY(double value[]) const 
	{ 
		value[0] = m_RgnRoiCadCornerPos[0].y; 
		value[1] = m_RgnRoiCadCornerPos[1].y; 
		value[2] = m_RgnRoiCadCornerPos[2].y; 
		value[3] = m_RgnRoiCadCornerPos[3].y; 
	}
	//---------------------------------------------------------------------------------//	
	//軟體條碼位置在CAD坐標系中
	void                       SetBarcodeStagePos(const TPOINT3D &value) { m_RgnStagePos = value; }
	TPOINT3D                   GetBarcodeStagePos() const { return m_RgnStagePos; }
	//---------------------------------------------------------------------------------//
	//軟體條碼位置在機台坐標系中-X
	void                       SetBarcodeStagePosX(double value) { m_RgnStagePos.x = value; }
	double                     GetBarcodeStagePosX() const { return m_RgnStagePos.x; }
	//---------------------------------------------------------------------------------//
	//軟體條碼位置在機台坐標系中-Y
	void                       SetBarcodeStagePosY(double value) { m_RgnStagePos.y = value; }
	double                     GetBarcodeStagePosY() const { return m_RgnStagePos.y; }
	//---------------------------------------------------------------------------------//	
	//軟體條碼位置在機台坐標系中-Z
	void                       SetBarcodeStagePosZ(double value) { m_RgnStagePos.z = value; }
	double                     GetBarcodeStagePosZ() const { return m_RgnStagePos.z; }
	//---------------------------------------------------------------------------------//	
	void                       GetBarcodeRoiStageCornerPos(TPOINT2D value[]) const 
	{
		value[0].x = m_RgnRoiStageCornerPos[0].x;	value[0].y = m_RgnRoiStageCornerPos[0].y; 	
		value[1].x = m_RgnRoiStageCornerPos[1].x;	value[1].y = m_RgnRoiStageCornerPos[1].y; 	
		value[2].x = m_RgnRoiStageCornerPos[2].x;	value[2].y = m_RgnRoiStageCornerPos[2].y; 	
		value[3].x = m_RgnRoiStageCornerPos[3].x;	value[3].y = m_RgnRoiStageCornerPos[3].y; 	
	}
	//---------------------------------------------------------------------------------//	
	void                       GetBarcodeBodyStageCornerPos(TPOINT2D value[]) const 
	{
		value[0].x = m_RgnBodyStageCornerPos[0].x;	value[0].y = m_RgnBodyStageCornerPos[0].y; 	
		value[1].x = m_RgnBodyStageCornerPos[1].x;	value[1].y = m_RgnBodyStageCornerPos[1].y; 	
		value[2].x = m_RgnBodyStageCornerPos[2].x;	value[2].y = m_RgnBodyStageCornerPos[2].y; 	
		value[3].x = m_RgnBodyStageCornerPos[3].x;	value[3].y = m_RgnBodyStageCornerPos[3].y; 	
	}
	//---------------------------------------------------------------------------------//	
	//軟體條碼四端點在機台坐標系中-X
	void                       SetBarcodeRoiStageCornerPosX(double value[]) 
	{ 
		m_RgnRoiStageCornerPos[0].x = value[0]; 
		m_RgnRoiStageCornerPos[1].x = value[1]; 
		m_RgnRoiStageCornerPos[2].x = value[2]; 
		m_RgnRoiStageCornerPos[3].x = value[3]; 
	}
	void                       GetBarcodeRoiStageCornerPosX(double value[]) const 
	{ 
		value[0] = m_RgnRoiStageCornerPos[0].x; 
		value[1] = m_RgnRoiStageCornerPos[1].x; 
		value[2] = m_RgnRoiStageCornerPos[2].x; 
		value[3] = m_RgnRoiStageCornerPos[3].x; 
	}	
	//---------------------------------------------------------------------------------//
	//軟體條碼四端點在機台坐標系中-Y
	void                       SetBarcodeRoiStageCornerPosY(double value[]) 
	{ 
		m_RgnRoiStageCornerPos[0].y = value[0]; 
		m_RgnRoiStageCornerPos[1].y = value[1]; 
		m_RgnRoiStageCornerPos[2].y = value[2]; 
		m_RgnRoiStageCornerPos[3].y = value[3]; 
	}
	void                       GetBarcodeRoiStageCornerPosY(double value[]) const 
	{ 
		value[0] = m_RgnRoiStageCornerPos[0].y; 
		value[1] = m_RgnRoiStageCornerPos[1].y; 
		value[2] = m_RgnRoiStageCornerPos[2].y; 
		value[3] = m_RgnRoiStageCornerPos[3].y; 
	}
	//---------------------------------------------------------------------------------//
	TREGION4D                  GetBarcodeRoiRgnCad() const;//軟體條碼範圍-Cad
	TREGION4D                  GetBarcodeBodyRgnCad() const;//軟體條碼範圍-Cad
	TREGION4D                  GetBarcodeRoiRgnStage() const;//軟體條碼範圍-Stage
	TREGION4D                  GetBarcodeBodyRgnStage() const;//軟體條碼範圍-Stage
	//---------------------------------------------------------------------------------//
	void                       GetBarcodeRoiCadRegion(TREGION4D &Region);//取得軟體條碼在Cad的範圍	
	void                       GetBarcodeBodyCadRegion(TREGION4D &Region);//取得軟體條碼在Cad的範圍	
	void                       GetBarcodeRoiStageRegion(TREGION4D &Region);//取得軟體條碼在Stage的範圍		
	void                       GetBarcodeBodyStageRegion(TREGION4D &Region);//取得軟體條碼在Stage的範圍		
	//---------------------------------------------------------------------------------//
	void                       MoveBarcodeCadPos(double dX, double dY);//移動軟體條碼座標	
	void                       MoveBarcodeStagePos(double dX, double dY);//移動軟體條碼座標	
	void                       MoveBarcodePos(double dX, double dY, CMapCoordinate *MapPtr);//移動軟體條碼座標	
	//---------------------------------------------------------------------------------//	
	bool                       CheckBarcodeBePickByCad(const TPOINT2D &PickPos);//確認軟體條碼被點擊到
	bool                       CheckBarcodeBePickByStage(const TPOINT2D &PickPos);//確認軟體條碼被點擊到

	bool                       CheckBarcodeInRegionByCad(const TREGION4D &SelRgn, bool bEntireIn);//確認軟體條碼在範圍內
	bool                       CheckBarcodeInRegionByStage(const TREGION4D &SelRgn, bool bEntireIn);//確認軟體條碼在範圍內
	//---------------------------------------------------------------------------------//		
	void                       CalcBarcodeCadCornerPos();//計算軟體條碼Cad端點座標		
	void                       LayoutBarcodeStageCornerPos();//更新軟體條碼機台端點座標
	//---------------------------------------------------------------------------------//
	//軟體條碼所屬FOV的CAD位置-X
	void                       SetBarcodeFovCadPosX(double value) { m_RgnFovCadPos.x = value; }
	double                     GetBarcodeFovCadPosX() const { return m_RgnFovCadPos.x; }
	//---------------------------------------------------------------------------------//
	//軟體條碼所屬FOV的CAD位置-Y
	void                       SetBarcodeFovCadPosY(double value) { m_RgnFovCadPos.y = value; }
	double                     GetBarcodeFovCadPosY() const { return m_RgnFovCadPos.y; }
	//---------------------------------------------------------------------------------//
	//軟體條碼所屬FOV的Stage位置-X
	void                       SetBarcodeFovStagePosX(double value) { m_RgnFovStagePos.x = value; }
	double                     GetBarcodeFovStagePosX() const { return m_RgnFovStagePos.x; }
	//---------------------------------------------------------------------------------//
	//軟體條碼所屬FOV的Stage位置-Y
	void                       SetBarcodeFovStagePosY(double value) { m_RgnFovStagePos.y = value; }
	double                     GetBarcodeFovStagePosY() const { return m_RgnFovStagePos.y; }
	//---------------------------------------------------------------------------------//
	//軟體條碼所屬影像的區域
	void                       SetBarcodeFrameImageRect(const RECT &value) { m_RgnFrameImageRect = value; }
	const RECT&                GetBarcodeFrameImageRect() const { return m_RgnFrameImageRect; }
	//---------------------------------------------------------------------------------//	
	//軟體條碼影像的物理尺寸
	void                       SetBarcodeFrameImageSize_um(const TSIZE2D &value);
	const TSIZE2D&             GetBarcodeFrameImageSize_um() const { return m_RgnFrameImageSize_um; }
	//---------------------------------------------------------------------------------//	
	//軟體條碼影像的區域Cad偏差-um
	void                       SetBarcodeFrameImageCadOffset_um(const TPOINT2D &value);	
	const TPOINT2D&            GetBarcodeFrameImageCadOffset_um() const { return m_RgnFrameImageCadOffset_um; }
	void                       SetBarcodeFrameImageStageOffset_um(const TPOINT2D &value);
	//---------------------------------------------------------------------------------//	
	void                       SetBarcodeFrameImageCornerPt(const POINT value[]) 
	{ 
		m_RgnFrameImageCornerPt[0] = value[0]; 
		m_RgnFrameImageCornerPt[1] = value[1]; 
		m_RgnFrameImageCornerPt[2] = value[2]; 
		m_RgnFrameImageCornerPt[3] = value[3]; 
	}
	void                       GetBarcodeFrameImageCornerPt(POINT value[]) const 
	{ 
		value[0] = m_RgnFrameImageCornerPt[0]; 
		value[1] = m_RgnFrameImageCornerPt[1]; 
		value[2] = m_RgnFrameImageCornerPt[2]; 
		value[3] = m_RgnFrameImageCornerPt[3]; 
	}
	//---------------------------------------------------------------------------------//	
	//軟體條碼所屬的Field的CAD位置-X
	void                       SetBarcodeFieldCadPosX(double value) { m_RgnFieldCadPos.x = value; }
	double                     GetBarcodeFieldCadPosX() const { return m_RgnFieldCadPos.x; }
	//---------------------------------------------------------------------------------//
	//軟體條碼所屬的Field的CAD位置-Y
	void                       SetBarcodeFieldCadPosY(double value) { m_RgnFieldCadPos.y = value; }
	double                     GetBarcodeFieldCadPosY() const { return m_RgnFieldCadPos.y; }
	//---------------------------------------------------------------------------------//
	//軟體條碼所屬的Field的Stage位置-X
	void                       SetBarcodeFieldStagePosX(double value) { m_RgnFieldStagePos.x = value; }
	double                     GetBarcodeFieldStagePosX() const { return m_RgnFieldStagePos.x; }
	//---------------------------------------------------------------------------------//
	//軟體條碼所屬的Field的Stage位置-Y
	void                       SetBarcodeFieldStagePosY(double value) { m_RgnFieldStagePos.y = value; }
	double                     GetBarcodeFieldStagePosY() const { return m_RgnFieldStagePos.y; }
	//---------------------------------------------------------------------------------//	
	void                       MapBarcodeCadToStagePos(const CMapCoordinate &Map);//將軟體條碼CAD轉成機台座標
	//---------------------------------------------------------------------------------//
	bool                       SpinBarcode(double Angle, CMapCoordinate *MapPtr);//軟體條碼自旋轉
	bool                       RotateBarcode(double Angle, double CpX, double CpY, CMapCoordinate *MapPtr);//軟體條碼旋轉	
	bool                       MirrorXBarcode(double CpX, CMapCoordinate *MapPtr);//軟體條碼鏡射-X
	bool                       MirrorYBarcode(double CpY, CMapCoordinate *MapPtr);//軟體條碼鏡射-Y	
	//---------------------------------------------------------------------------------//	
	CString                    GetBarcodeFullName() const;//取得軟體條碼的名稱	
	virtual CString            GetRgnDerivedName() const;//取得軟體條碼的名稱	
	virtual CString            GetRgnDerivedKeyName() const;//取得軟體條碼的名稱	
	virtual bool               ExtractRgnDerivedFrame(bool &Finished);//挖取軟體條碼圖片
	virtual bool               ExecRgnDerivedInspection();//執行軟體條碼檢測
	//---------------------------------------------------------------------------------//
	bool                       CreateBarcodeSubRgnList(FIELD_BUILD_MODE BuildMode, bool ByCadRegion);//建立軟體條碼子檢測區域列表
	void                       ClearBarcodeSubRgnList();//清除軟體條碼的子列表
	size_t                     GetBarcodeSubRgnCount() const;//取得軟體條碼的子數量
	CAOIRgn*                   GetBarcodeSubRgnPtr(size_t index, bool check) const;//取得軟體條碼的子指標
	//---------------------------------------------------------------------------------//	
	CAOIModel*                 GetBarcodeModelPtr();//取得條碼模組
	bool                       UpdateBarcodeParamToModel();//更新條碼參數至模組內
	bool                       UpdateBarcodeModelFromLibrary(CAOIModel *ModelPtr);//更新條碼模組
	//---------------------------------------------------------------------------------//	
	void                       InitBarcodeInspection();//初始化條碼檢測
	bool                       ExecBarcodeInspection();//執行條碼檢測
	bool                       ExecBarcodeSaveDefectImage(const std::vector<TUNI_FRAME> &UniFrameList);//執行條碼儲存瑕疵圖片
	bool                       UpdateBarcodeResultID();//更新檢測框檢測結果
	//---------------------------------------------------------------------------------//	
	bool                       UpdateBarcodeFrameIndex(unsigned int DefaultIndex, unsigned int DefaultUniqueID, const std::vector<unsigned int> &FrameIndexMapList);
	//---------------------------------------------------------------------------------//	
	//空間基準面參數
	void                       SetBarcodePanelBasePlane(double val);
	double                     GetBarcodePanelBasePlane() const { return GetRgnPanelBasePlane(); }
	//---------------------------------------------------------------------------------//
	void                       SetBarcodeLocalBasePlaneID(int val) { SetRgnLocalBasePlaneID(val); }
	int                        GetBarcodeLocalBasePlaneID() const { return GetRgnLocalBasePlaneID(); }
	//---------------------------------------------------------------------------------//
	void                       SetBarcodeLocalBasePlaneFinish(bool val) { SetRgnLocalBasePlaneFinish(val); }
	bool                       GetBarcodeLocalBasePlaneFinish() const { return GetRgnLocalBasePlaneFinish(); }
	//---------------------------------------------------------------------------------//
	void                       SetBarcodeLocalBasePlaneParam(const TPOINT3D &val) { SetRgnLocalBasePlaneParam(val); }
	void                       GetBarcodeLocalBasePlaneParam(TPOINT3D &val) const { GetRgnLocalBasePlaneParam(val); }
	//---------------------------------------------------------------------------------//
	TBasePlaneParam&           GetBarcodeSpaceBasePlaneParam() { return GetRgnSpaceBasePlaneParam(); }
	const TBasePlaneParam&     GetBarcodeSpaceBasePlaneParam() const { return GetRgnSpaceBasePlaneParam(); }
	void                       SetBarcodeSpaceBasePlaneParam(const TBasePlaneParam& Param);
	//---------------------------------------------------------------------------------//
	//空間雜訊過濾處理
	TNoiseFilterParam&         GetBarcodeSpaceNoiseFilterParam() { return GetRgnSpaceNoiseFilterParam(); }
	const TNoiseFilterParam&   GetBarcodeSpaceNoiseFilterParam() const { return GetRgnSpaceNoiseFilterParam(); }
	void                       SetBarcodeSpaceNoiseFilterParam(const TNoiseFilterParam& Param);
	//---------------------------------------------------------------------------------//		
	bool                       GetBarcodeDataModelEnabled() const;//取得條碼資料模型啟用
	void                       SetBarcodeDataModelEnabled(bool val);//設定條碼資料模型啟用
	//---------------------------------------------------------------------------------/
	int                        GetBarcodeDataModelLevelID() const;//取得條碼資料模型等級
	void                       SetBarcodeDataModelLevelID(int val);//設定條碼資料模型等級
	//---------------------------------------------------------------------------------/
};
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_AOIBARCODE_H__B666DA79_0F66_4D23_8B1C_D814C09E3082__INCLUDED_)
