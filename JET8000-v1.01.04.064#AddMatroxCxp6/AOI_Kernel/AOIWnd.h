// AOIWnd.h: interface for the CAOIWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AOIWND_H__CDCA81FC_E32D_415B_AE31_0DC418D54B15__INCLUDED_)
#define AFX_AOIWND_H__CDCA81FC_E32D_415B_AE31_0DC418D54B15__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include "AOIObj.h"
#include "AOIBox.h"
#include "AlgParam.h"
#include "AOIWndRoi.h"
#include "AOIWndMask.h"
#include "AOIModelDef.h"
//-------------------------------------------------------------------------------------//
class CAOILand;
class CAOIModel;
class CAOIFileIO;
//-------------------------------------------------------------------------------------//
#define WND_BOX_BASIC               1
#define WND_BOX_EXTEND              2
//-------------------------------------------------------------------------------------//
class CAOIWnd : public CAOIObj  
{
	//---------------------------------------------------------------------------------//
	DECLARE_DYNAMIC(CAOIWnd)
	//---------------------------------------------------------------------------------//	
	static bool                FilterWndDefectID(WND_DEFECT_ID DefectID, CAOILand *LandPtr);//過濾檢測框瑕疵代碼
	static bool                FilterWndAlgType(ALG_TYPE AlgType, WND_DEFECT_ID WndDefectID, CAOILand *LandPtr);
	//---------------------------------------------------------------------------------//	
private:
	//---------------------------------------------------------------------------------//	
	unsigned int               m_WndIndex;                   //檢測框引數
	int                        m_WndBandID;                  //檢測框次群組編號
	int                        m_WndGroupID;                 //檢測框群組編號	
	int                        m_WndClassID;                 //檢測框類別編號, 0:不分類, >0同類才能檢測
	bool                       m_WndBypassed;                //檢測框忽略不測
	bool                       m_WndIsolated;                //檢測框是否隔離
	bool                       m_WndModified;                //檢測框是否變更過	
	WND_DEFECT_ID              m_WndDefectID;                //檢測框瑕疵代碼	
	bool                       m_WndDefectAlarm;             //檢測框瑕疵警報-重大瑕疵
	bool                       m_WndDefectAlarmEnableOnAOI;  //檢測框瑕疵警報-啟用機台警報
	bool                       m_WndDefectAlarmEnableOnARS;  //檢測框瑕疵警報-維修站顯示
	int                        m_WndDefectGroupID;           //檢測框瑕疵群組編號
	size_t                     m_WndOrderIndex;              //檢測框檢測次序	
	double                     m_WndInspectedTime;           //檢測框檢測時間
	WND_CONSTRAIN_MODE         m_WndConstrainMode;           //檢測框侷限模式
	TPOINT2D                   m_WndRectCalValue;            //檢測框影像區域的校正-Float轉Int的誤差修正	
	//------------------------------------------------------------------------------	
	bool                       m_WndUIUpated_Param;          //檢測框介面更新過-參數
	//---------------------------------------------------------------------------------//	
	CAOIModel*                 m_WndModelPtr;	             //檢測框所屬的模組
	//---------------------------------------------------------------------------------//	
	CAOILand*                  m_WndLandPtr;                 //檢測框所屬的特徵框
	unsigned int               m_WndLandIndex;	             //檢測框所屬的特徵框引數 
	//---------------------------------------------------------------------------------//		
	WND_LOGIC_TYPE             m_WndLogicType;               //檢測框邏輯樣式
	int                        m_WndLogicGroupID;            //檢測框邏輯群組編號
	RESULT_ID                  m_WndLogicResultID;           //檢測框邏輯結果 
	//---------------------------------------------------------------------------------//		
	WND_FOLLOW_MODE            m_WndFollowMode;              //檢測框跟隨移動模式
	WND_SYNC_MOVE_MODE         m_WndSyncMoveMode;            //檢測框同時移動模式
	//---------------------------------------------------------------------------------//	
	bool                       m_WndRgnLinkAuto;             //檢測框範圍自動連動
	WND_RGN_LINK_MODE          m_WndRgnLinkMode;             //檢測框範圍連動模式
	double                     m_WndRgnLinkRatioX;           //檢測框範圍連動U方向比例-%
	double                     m_WndRgnLinkRatioY;           //檢測框範圍連動V方向比例-%
	int                        m_WndRgnLinkGoupID;           //檢測框範圍綁定的特徵框群組編號	
	//---------------------------------------------------------------------------------//
	int                        m_WndTempInt[4];              //檢測框-暫時使用變數
	//---------------------------------------------------------------------------------//
	CAOIBox                    m_WndBox;                     //檢測框-內框
	RECT                       m_WndImageRect;               //檢測框-內框影像區域
	RECT                       m_WndImageRect_Raw;           //檢測框-內框影像區域-原始
	//---------------------------------------------------------------------------------//		
	CAOIBox                    m_WndExtendBox;	             //檢測框-外框
	double                     m_WndExtendRangeX;            //檢測框外擴範圍-um
	double                     m_WndExtendRangeY;            //檢測框外擴範圍-um	
	bool                       m_WndExtendBoxUsed;           //檢測框-使用外擴範圍
	RECT                       m_WndExtendImageRect;         //檢測框-外擴影像區域
	RECT                       m_WndExtendImageRect_Raw;     //檢測框-外擴影像區域-原始
	//---------------------------------------------------------------------------------//	
	std::vector<CAOIWndRoi*>   m_WndRoiWndList;              //檢測框-子檢測框列表
	//---------------------------------------------------------------------------------//	
	int                        m_WndModelMaskFlag;           //檢測框-模組遮罩旗標
	std::vector<CAOIWndMask*>  m_WndMaskWndList;             //檢測框-遮罩框列表
	//---------------------------------------------------------------------------------//		
	CAlgParam                  m_WndAlgParam;                //檢測框-演算法參數
	std::vector<CAOIBox>       m_WndResultBoxList;           //檢測框-結果框列表
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//	
	void                       PreInitWnd();
	void                       InitialWnd();
	void                       CloneWnd(const CAOIWnd &Wnd);
	void                       CloneWndRoiWndList(const CAOIWnd &Wnd);
	void                       CloneWndMaskBoxList(const CAOIWnd &Wnd);
	//---------------------------------------------------------------------------------//
	void                       UpdateWndBoxEditabled();
	void                       UpdateWndExtendBoxKernel();
	//---------------------------------------------------------------------------------//
	size_t                     GetWndRoiWndCount_Inline() const;
	void                       AddWndRoiWnd_Inline(CAOIWndRoi *WndRoiPtr);
	CAOIWndRoi*                GetWndRoiWndPtr_Inline(size_t index) const;	
	bool                       BuildWndRoiWndListKernel(int RoiCntX, int RoiCntY, double MarginXRatio, double MarginYRatio);
	bool                       BuildWndRoiWndListKernel_DivideXY(bool Result, int DivideX, int DivideY, double MarginXRatio, double MarginYRatio, const CAOIBox &BoxWnd, std::vector<CAOIWndRoi*> &WndRoiList);
	//---------------------------------------------------------------------------------//	
	void                       ClearWndResultBoxList_Inline();
	size_t                     GetWndResultBoxCount_Inline() const;
	void                       AddWndResultBox_Inline(const CAOIBox &Box);
	CAOIBox*                   GetWndResultBoxPtr_Inline(size_t index);
	//---------------------------------------------------------------------------------//	
	size_t                     GetWndMaskWndCount_Inline() const;
	void                       AddWndMaskWnd_Inline(CAOIWndMask *WndMaskPtr);
	CAOIWndMask*               GetWndMaskWndPtr_Inline(size_t index) const;
	//---------------------------------------------------------------------------------//		
	bool                       ExecWndConstrainKernel();//執行檢測框侷限函式
	bool                       ExecWndInspectionKernel(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const TPOINT2D &RgnCp, const TPOINT2D &Scale, const TPOINT2D &ImageCp, std::vector<TUNI_FRAME> &UniFrameList, bool bTestWnd);//執行檢測框檢測
	//---------------------------------------------------------------------------------//	
	WND_DEFECT_ID              ConvertWndDefectID(int val);//將舊的瑕疵代碼轉成新的瑕疵代碼
	//---------------------------------------------------------------------------------//	
	CString                    m_WndTestTrackFilename;
	bool                       GetWndTestTrackFile() const;//是否儲存檢測框檢測追蹤檔案
	bool                       SaveWndTestTrackFile();//儲存檢測框檢測追蹤檔案
    bool                       DeleteWndTestTrackFile();//刪除檢測框檢測追蹤檔案
	//---------------------------------------------------------------------------------//
	bool                       DumpWndException(LPCTSTR Info, bool bDumped);//輸出檢測框異常訊息
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	CAOIWnd();
	CAOIWnd(const CAOIWnd &Wnd);
	virtual ~CAOIWnd();
	CAOIWnd& operator=(const CAOIWnd &Wnd);
	//---------------------------------------------------------------------------------//
	void                       ResetWndObj();//復歸Wnd物件
	void                       ReleaseWndObj();//釋放Wnd物件
	CAOIWnd*                   CloneWndObj() const;//建立且複製一個檢測框
	//---------------------------------------------------------------------------------//
	bool                       GetSaveWndLog() const;//取得是否儲存檢測框訊息
	bool                       SaveWndLog(LPCTSTR str);//儲存檢測框訊息
	bool                       SaveWndLog(LPCTSTR strAct, CAOIModel *ModelPtr, LPCTSTR str);//儲存檢測框訊息
	//---------------------------------------------------------------------------------//
	bool                       WriteWndFile(CAOIFileIO &FileIO);//儲存檢測框檔案
	bool                       ReadWndFile(CAOIFileIO &FileIO);//載入檢測框檔案
	//---------------------------------------------------------------------------------//
	bool                       BuildWndParamStringList(LPCTSTR Title, std::vector<CString> &strList) const;//建立檢測框參數列表
	//---------------------------------------------------------------------------------//
	bool                       WriteWndSpcFile_JSON_VRS(FILE *pfile, CAOIProject *ProjectPtr);
	bool                       WriteWndSpcFile_JSON_RSM(FILE *pfile, CAOIProject *ProjectPtr);
	bool                       WriteWndSpcHeader_JSON_detail(FILE *pfile, CAOIProject *ProjectPtr);
	//---------------------------------------------------------------------------------//
	bool                       ApplyWnd(const CAOIWnd *RefWndPtr);//更新檢測框參數-給相同群組使用	
	bool                       SynchronousWnd(const CAOIWnd *WndPtr);//同步WndObj參數	
	bool                       ApplyWndDefault(const CAOIWnd *RefWndPtr);//更新檢測框參數-給預設檢測框
	//---------------------------------------------------------------------------------//
	void                       SetWndIndex(unsigned int value) { m_WndIndex = value; }
	unsigned int               GetWndIndex() const { return m_WndIndex; }
	//---------------------------------------------------------------------------------//	
	void                       SetWndBandID(int value) { m_WndBandID = value; }
	int                        GetWndBandID() const { return m_WndBandID; }
	//---------------------------------------------------------------------------------//
	void                       SetWndGroupID(int value) { m_WndGroupID = value; m_WndAlgParam.SetAlgGroupID(value); }
	int                        GetWndGroupID() const { return m_WndGroupID; }
	//---------------------------------------------------------------------------------//
	bool                       CheckWndLinkPos(CAOIWnd *WndPtr) const;//確認檢測框位置連動
	bool                       CheckWndLinkSize(CAOIWnd *WndPtr) const;//確認檢測框尺寸連動
	//---------------------------------------------------------------------------------//
	//檢測框類別編號
	void                       SetWndClassID(int value) { m_WndClassID = value; }
	int                        GetWndClassID() const { return m_WndClassID; }		
	bool                       CheckWndClassIDUsed(int ID) const;//確認檢測框類別是否使用
	//---------------------------------------------------------------------------------//	
	void                       SetWndOrderIndex(size_t value) { m_WndOrderIndex = value;  }
	size_t                     GetWndOrderIndex() const { return m_WndOrderIndex; }
	//---------------------------------------------------------------------------------//
	void                       SetWndInspectedTime(double value) { m_WndInspectedTime = value;  }
	double                     GetWndInspectedTime() const { return m_WndInspectedTime; }
	//---------------------------------------------------------------------------------//
	void                       SetWndConstrainMode(WND_CONSTRAIN_MODE value) { m_WndConstrainMode = value;  }
	WND_CONSTRAIN_MODE         GetWndConstrainMode() const { return m_WndConstrainMode; }
	//---------------------------------------------------------------------------------//	
	void                       SetWndBypassed(bool value) { m_WndBypassed = value;  }
	bool                       GetWndBypassed() const { return m_WndBypassed; }
	//---------------------------------------------------------------------------------//
	void                       SetWndModified(bool value) { m_WndModified = value;  }
	bool                       GetWndModified() const { return m_WndModified; }
	//---------------------------------------------------------------------------------//	
	void                       SetWndRectCalValue(const TPOINT2D &value) { m_WndRectCalValue = value;  }
	void                       GetWndRectCalValue(TPOINT2D &value) const { value=m_WndRectCalValue; }
	TPOINT2D                   GetWndRectCalValue() const { return m_WndRectCalValue; }	
	//---------------------------------------------------------------------------------//	
	void                       SetWndUIUpated_Param(bool value) { m_WndUIUpated_Param = value;  }
	bool                       GetWndUIUpated_Param() const { return m_WndUIUpated_Param; }
	//---------------------------------------------------------------------------------//
	void                       SetWndModelPtr(CAOIModel* Ptr) { m_WndModelPtr = Ptr; }
	CAOIModel*                 GetWndModelPtr() const { return m_WndModelPtr; }
	//---------------------------------------------------------------------------------//
	void                       SetWndLandPtr(CAOILand* Ptr) { m_WndLandPtr = Ptr; }
	CAOILand*                  GetWndLandPtr() const { return m_WndLandPtr; }
	//---------------------------------------------------------------------------------//
	void                       SetWndLandIndex(unsigned int value) { m_WndLandIndex = value; }
	unsigned int               GetWndLandIndex() const { return m_WndLandIndex; }
	//---------------------------------------------------------------------------------//
	void                       SetWndIsolated(bool value) { m_WndIsolated = value; }
	bool                       GetWndIsolated() const { return m_WndIsolated; }
	//---------------------------------------------------------------------------------//
	void                       ChangeWndDefectID(MODEL_TYPE ModelType, WND_DEFECT_ID WndDefectID);
	void                       SetWndDefectID(WND_DEFECT_ID value) { m_WndDefectID = value; }
	WND_DEFECT_ID              GetWndDefectID() const { return m_WndDefectID; }
	//---------------------------------------------------------------------------------//
	//檢測框瑕疵警報-重大瑕疵
	void                       SetWndDefectAlarm(bool value) { m_WndDefectAlarm = value;  }
	bool                       GetWndDefectAlarm() const { return m_WndDefectAlarm; }
	//---------------------------------------------------------------------------------//
	//檢測框瑕疵警報-機台警報
	void                       SetWndDefectAlarmEnableOnAOI(bool value) { m_WndDefectAlarmEnableOnAOI = value;  }
	bool                       GetWndDefectAlarmEnableOnAOI() const { return m_WndDefectAlarmEnableOnAOI; }
	//---------------------------------------------------------------------------------//
	//檢測框瑕疵警報-維修站
	void                       SetWndDefectAlarmEnableOnARS(bool value) { m_WndDefectAlarmEnableOnARS = value;  }
	bool                       GetWndDefectAlarmEnableOnARS() const { return m_WndDefectAlarmEnableOnARS; }
	//---------------------------------------------------------------------------------//
	void                       SetWndDefectGroupID(int value) { m_WndDefectGroupID = value; }
	int                        GetWndDefectGroupID() const { return m_WndDefectGroupID; }
	//---------------------------------------------------------------------------------//
	void                       SetWndToward(BOX_TOWARD value) { m_WndBox.SetBoxToward(value); m_WndExtendBox.SetBoxToward(value); }
	BOX_TOWARD                 GetWndToward() const { return m_WndBox.GetBoxToward(); }
	//---------------------------------------------------------------------------------//
	void                       SetWndShapeMode(BOX_SHAPE_MODE value) { m_WndBox.SetBoxShapeMode(value); m_WndExtendBox.SetBoxShapeMode(value); }
	BOX_SHAPE_MODE             GetWndShapeMode() const { return m_WndBox.GetBoxShapeMode(); }
	//---------------------------------------------------------------------------------//
	void                       SetWndShapeParam(double value) { m_WndBox.SetBoxShapeParam(value); m_WndExtendBox.SetBoxShapeParam(value); }
	double                     GetWndShapeParam() const { return m_WndBox.GetBoxShapeParam(); }
	//---------------------------------------------------------------------------------//
	void                       SetWndShapeParam2(double value) { m_WndBox.SetBoxShapeParam2(value); m_WndExtendBox.SetBoxShapeParam2(value); }
	double                     GetWndShapeParam2() const { return m_WndBox.GetBoxShapeParam2(); }
	//---------------------------------------------------------------------------------//
	void                       SetWndActived(bool value) { m_WndBox.SetBoxActived(value); }
	bool                       GetWndActived() const { return m_WndBox.GetBoxActived(); }
	//---------------------------------------------------------------------------------//
	void                       SetWndSelected(bool value) { m_WndBox.SetBoxSelected(value); }
	bool                       GetWndSelected() const { return m_WndBox.GetBoxSelected(); }
	//---------------------------------------------------------------------------------//
	void                       SetWndVisibled(bool value) { m_WndBox.SetBoxVisibled(value); }
	bool                       GetWndVisibled() const { return m_WndBox.GetBoxVisibled(); }
	//---------------------------------------------------------------------------------//
	void                       SetWndEnabled(bool value) { m_WndBox.SetBoxEnabled(value); }
	bool                       GetWndEnabled() const { return m_WndBox.GetBoxEnabled();; }
	//---------------------------------------------------------------------------------//
	void                       SetWndEditabled(bool value) { m_WndBox.SetBoxEditabled(value); }
	bool                       GetWndEditabled() const { return m_WndBox.GetBoxEditabled(); }
	//---------------------------------------------------------------------------------//	
	void                       SetWndRgnLinkAuto(bool value);
	bool                       GetWndRgnLinkAuto() const { return m_WndRgnLinkAuto; }
	//---------------------------------------------------------------------------------//
	//檢測框邏輯樣式
	void                       SetWndLogicType(WND_LOGIC_TYPE value) { m_WndLogicType=value; }
	WND_LOGIC_TYPE             GetWndLogicType() const { return m_WndLogicType; }
	//---------------------------------------------------------------------------------//
	//檢測框邏輯群組編號
	void                       SetWndLogicGroupID(int value) { m_WndLogicGroupID=value; }
	int                        GetWndLogicGroupID() const { return m_WndLogicGroupID; }
	//---------------------------------------------------------------------------------//
	//檢測框邏輯結果 
	void                       SetWndLogicResultID(RESULT_ID value) { m_WndLogicResultID=value; }
	RESULT_ID                  GetWndLogicResultID() const { return m_WndLogicResultID; }
	//---------------------------------------------------------------------------------//
	void                       SetWndFollowMode(WND_FOLLOW_MODE value) { m_WndFollowMode=value; }
	WND_FOLLOW_MODE            GetWndFollowMode() const { return m_WndFollowMode; }
	//---------------------------------------------------------------------------------//
	//檢測框同時移動模式
	void                       SetWndSyncMoveMode(WND_SYNC_MOVE_MODE value) { m_WndSyncMoveMode=value; }
	WND_SYNC_MOVE_MODE         GetWndSyncMoveMode() const { return m_WndSyncMoveMode; }
	//---------------------------------------------------------------------------------//
	void                       SetWndRgnLinkMode(WND_RGN_LINK_MODE value);
	WND_RGN_LINK_MODE          GetWndRgnLinkMode() const { return m_WndRgnLinkMode; }	
	//---------------------------------------------------------------------------------//
	void                       SetWndRgnLinkRatioX(double value) { m_WndRgnLinkRatioX = value; }
	double                     GetWndRgnLinkRatioX() const { return m_WndRgnLinkRatioX; }
	//---------------------------------------------------------------------------------//
	void                       SetWndRgnLinkRatioY(double value) { m_WndRgnLinkRatioY = value; }
	double                     GetWndRgnLinkRatioY() const { return m_WndRgnLinkRatioY; }
	//---------------------------------------------------------------------------------//
	void                       SetWndRgnLinkGoupID(int value) { m_WndRgnLinkGoupID = value; }
	int                        GetWndRgnLinkGoupID() const { return m_WndRgnLinkGoupID; }
	//---------------------------------------------------------------------------------//
	//檢測框-暫時使用變數
	int                        GetWndTempInt1() const;
	void                       SetWndTempInt1(int value);	
	int                        GetWndTempInt2() const;
	void                       SetWndTempInt2(int value);	
	int                        GetWndTempInt3() const;
	void                       SetWndTempInt3(int value);	
	int                        GetWndTempInt4() const;
	void                       SetWndTempInt4(int value);	
	int                        GetWndTempInt(int idx=0) const;
	void                       SetWndTempInt(int value, int idx=0);	
	//---------------------------------------------------------------------------------//		
	double                     GetWndAngleSkew() const;
	//---------------------------------------------------------------------------------//	
	void                       GetWndUseCornerPos(TPOINT2D CornerPos[]) const;
	void                       GetWndUseCornerPosRes(TPOINT2D CornerPos[]) const;
	void                       GetWndUseCornerPosCad(TPOINT2D CornerPos[]) const;
	void                       GetWndUseCornerPosCadRes(TPOINT2D CornerPos[]) const;
	void                       GetWndUseCornerPosStage(TPOINT2D CornerPos[]) const;
	void                       GetWndUseCornerPosStageRes(TPOINT2D CornerPos[]) const;
	//---------------------------------------------------------------------------------//	
	void                       GetWndUseRegion(TREGION4D &Region) const;
	void                       GetWndUseRegionRes(TREGION4D &Region) const;
	void                       GetWndUseRegionCad(TREGION4D &Region) const;
	void                       GetWndUseRegionCadRes(TREGION4D &Region) const;
	void                       GetWndUseRegionStage(TREGION4D &Region) const;
	void                       GetWndUseRegionStageRes(TREGION4D &Region) const;
	void                       GetWndUseRegion(double &MinX, double &MinY, double &MaxX, double &MaxY) const;
	//---------------------------------------------------------------------------------//	
	void                       GetWndCornerPos(TPOINT2D CornerPos[]) const { CAOIWnd::m_WndBox.GetBoxCornerPos(CornerPos); }	
	void                       GetWndCornerPosRes(TPOINT2D CornerPos[]) const { CAOIWnd::m_WndBox.GetBoxCornerPosRes(CornerPos); }		
	void                       GetWndCornerPosCad(TPOINT2D CornerPos[]) const { CAOIWnd::m_WndBox.GetBoxCornerPosCad(CornerPos); }	
	void                       GetWndCornerPosCadRes(TPOINT2D CornerPos[]) const { CAOIWnd::m_WndBox.GetBoxCornerPosCadRes(CornerPos); }	
	void                       GetWndCornerPosStage(TPOINT2D CornerPos[]) const { CAOIWnd::m_WndBox.GetBoxCornerPosStage(CornerPos); }	
	void                       GetWndCornerPosStageRes(TPOINT2D CornerPos[]) const { CAOIWnd::m_WndBox.GetBoxCornerPosStageRes(CornerPos); }	
	//---------------------------------------------------------------------------------//
	void                       SetWndRegion(const TREGION4D &Region, bool IncludeRes=true);
	void                       SetWndRegion(double MinX, double MinY, double MaxX, double MaxY, bool IncludeRes=true);	

	void                       GetWndRegion(TREGION4D &Region) const { CAOIWnd::m_WndBox.GetBoxRegion(Region); }		
	void                       GetWndRegionRes(TREGION4D &Region) const { CAOIWnd::m_WndBox.GetBoxRegionRes(Region); }
	void                       GetWndRegionCad(TREGION4D &Region) const { CAOIWnd::m_WndBox.GetBoxRegionCad(Region); }
	void                       GetWndRegionCadRes(TREGION4D &Region) const { CAOIWnd::m_WndBox.GetBoxRegionCadRes(Region); }
	void                       GetWndRegionStage(TREGION4D &Region) const { CAOIWnd::m_WndBox.GetBoxRegionStage(Region); }		
	void                       GetWndRegionStageRes(TREGION4D &Region) const { CAOIWnd::m_WndBox.GetBoxRegionStageRes(Region); }	
	void                       GetWndRegion(double &MinX, double &MinY, double &MaxX, double &MaxY) const { CAOIWnd::m_WndBox.GetBoxRegion(MinX, MinY, MaxX, MaxY); }	
	//---------------------------------------------------------------------------------//
	//內框影像區域
	void                       SetWndImageRect(const RECT &Rect) { 	m_WndImageRect = Rect; }
	void                       GetWndImageRect(RECT &Rect) { Rect=m_WndImageRect; }		
	//---------------------------------------------------------------------------------//	
	//檢測框-內框影像區域-原始
	const RECT&                GetWndImageRect_Raw() const { return m_WndImageRect_Raw; }		
	void                       SetWndImageRect_Raw(const RECT &Rect) { 	m_WndImageRect_Raw = Rect; }
	//---------------------------------------------------------------------------------//	
	//檢測框-外擴影像區域
	void                       SetWndExtendImageRect(const RECT &Rect) { 	m_WndExtendImageRect = Rect; }
	void                       GetWndExtendImageRect(RECT &Rect) { Rect=m_WndExtendImageRect; }		
	//---------------------------------------------------------------------------------/
	//檢測框-外擴影像區域-原始
	const RECT&                GetWndExtendImageRect_Raw() const { return m_WndExtendImageRect_Raw; }		
	void                       SetWndExtendImageRect_Raw(const RECT &Rect) { 	m_WndExtendImageRect_Raw = Rect; }
	//---------------------------------------------------------------------------------//
	int                        GetWndBasicBoxID(const CAOIBox *BoxPtr) const;
	CAOIBox*                   GetWndBasicBoxPtr(int BasicBoxID);
	//---------------------------------------------------------------------------------//
	CAOIBox&                   GetWndBox() { return m_WndBox; }
	CAOIBox*                   GetWndBoxPtr() { return &m_WndBox; }
	CAOIBox&                   GetWndExtendBox() { return m_WndExtendBox; }
	CAOIBox*                   GetWndExtendBoxPtr() { return &m_WndExtendBox; }
	//---------------------------------------------------------------------------------//
	void                       GetWndExtendCornerPos(TPOINT2D CornerPos[]) const { m_WndExtendBox.GetBoxCornerPos(CornerPos); }	
	void                       GetWndExtendCornerPosRes(TPOINT2D CornerPos[]) const { m_WndExtendBox.GetBoxCornerPosRes(CornerPos); }		
	void                       GetWndExtendCornerPosCad(TPOINT2D CornerPos[]) const { m_WndExtendBox.GetBoxCornerPosCad(CornerPos); }	
	void                       GetWndExtendCornerPosCadRes(TPOINT2D CornerPos[]) const { m_WndExtendBox.GetBoxCornerPosCadRes(CornerPos); }	
	void                       GetWndExtendCornerPosStage(TPOINT2D CornerPos[]) const { m_WndExtendBox.GetBoxCornerPosStage(CornerPos); }	
	void                       GetWndExtendCornerPosStageRes(TPOINT2D CornerPos[]) const { m_WndExtendBox.GetBoxCornerPosStageRes(CornerPos); }	
	//---------------------------------------------------------------------------------//
	void                       GetWndExtendRegion(TREGION4D &Region) const { m_WndExtendBox.GetBoxRegion(Region); }		
	void                       GetWndExtendRegionRes(TREGION4D &Region) const { m_WndExtendBox.GetBoxRegionRes(Region); }
	void                       GetWndExtendRegionCad(TREGION4D &Region) const { m_WndExtendBox.GetBoxRegionCad(Region); }
	void                       GetWndExtendRegionCadRes(TREGION4D &Region) const { m_WndExtendBox.GetBoxRegionCadRes(Region); }
	void                       GetWndExtendRegionStage(TREGION4D &Region) const { m_WndExtendBox.GetBoxRegionStage(Region); }		
	void                       GetWndExtendRegionStageRes(TREGION4D &Region) const { m_WndExtendBox.GetBoxRegionStageRes(Region); }	
	void                       GetWndExtendRegion(double &MinX, double &MinY, double &MaxX, double &MaxY) const { m_WndExtendBox.GetBoxRegion(MinX, MinY, MaxX, MaxY); }	
	//---------------------------------------------------------------------------------//
	void                       SetWndExtendRangeX(double value) { m_WndExtendRangeX = value; }
	double                     GetWndExtendRangeX() const { return m_WndExtendRangeX; }
	//---------------------------------------------------------------------------------//
	void                       SetWndExtendRangeY(double value) { m_WndExtendRangeY = value; }
	double                     GetWndExtendRangeY() const { return m_WndExtendRangeY; }
	//---------------------------------------------------------------------------------//	
	void                       SetWndExtendBoxUsed(bool value) { m_WndExtendBoxUsed = value; }
	bool                       GetWndExtendBoxUsed() const { return m_WndExtendBoxUsed; }
	//---------------------------------------------------------------------------------//	
	void                       UpdateWndExtendBox();
	bool                       CheckWndUsedShapeMode() const;//確認檢測框演算法是否可以使用外形框
	bool                       CheckWndUsedShapeMode(ALG_TYPE AlgType) const;//確認檢測框演算法是否可以使用外形框
	//---------------------------------------------------------------------------------//	
	size_t                     GetWndRoiWndCount() const;
	CAOIWndRoi*                GetWndRoiWndActived();
	CAOIWndRoi*                GetWndRoiWndSelected();
	void                       SetWndRoiWndActived(CAOIWndRoi *WndRoiPtr);
	bool                       AddWndRoiWndPtr(CAOIWndRoi *WndRoiPtr, bool Clone);	
	CAOIWndRoi*                GetWndRoiWndPtr(size_t index, bool Check);	
	void                       ClearWndRoiWndList();
	bool                       BuildWndRoiWndListDefault(int DefaultCount);	
	bool                       BuildWndRoiWndList(int RoiCntX, int RoiCntY, double MarginXRatio, double MarginYRatio);	
	bool                       UpdateWndParamRoiDefault(int DefaultCount);
	void                       DestroyWndRoiWndSelected();	
	void                       SetWndAllRoiWndActived(bool value);
	void                       SetWndAllRoiWndSelected(bool value);		
	bool                       CalcWndRoiWndRegion(TREGION4D &Region);//計算檢測框子框區域	
	bool                       GetWndRoiWndSelectedList(std::vector<CAOIWndRoi*> &WndRoiList);
	//---------------------------------------------------------------------------------//	
	void                       ClearWndResultBoxList();
	size_t                     GetWndResultBoxCount() const;
	CAOIBox*                   GetWndResultBoxPtr(size_t index, bool Check);
	bool                       AddWndResultBox(const CAOIBox &Box);
	void                       CloneWndResultBoxList(CAOIWnd *WndPtr);//複製檢測框結果框列表
	//---------------------------------------------------------------------------------//		
	int                        GetWndModelMaskFlag() const;//取得模組遮罩旗標
	void                       SetWndModelMaskFlag(int value);//設定模組遮罩旗標
	void                       AddWndModelMaskFlag(int value);//加入模組遮罩旗標
	void                       RemoveWndModelMaskFlag(int value);//加入模組遮罩旗標
	bool                       BuildWndModelMaskWndList(const CAOIModel *ModelPtr, const TREGION4D &BoundRgn, const TREGION4D &BoundRgnRes, std::vector<CAOIBox> &MaskBoxList);//建立模組遮罩匡列表
	//---------------------------------------------------------------------------------//	
	bool                       CheckWndAlgUsedMaskWnd() const;//確認檢測框演算法是否可以使用遮罩框
	bool                       CheckWndAlgUsedMaskWnd(ALG_TYPE AlgType) const;//確認檢測框演算法是否可以使用遮罩框
	//---------------------------------------------------------------------------------//	
	void                       ClearWndMaskWndList();	
	CAOIWndMask*               GetWndMaskWndActived();
	CAOIWndMask*               GetWndMaskWndSelected();
	void                       SetWndMaskWndActived(CAOIWndMask *MaskWndPtr);
	size_t                     GetWndMaskWndCount() const;
	CAOIWndMask*               GetWndMaskWndPtr(size_t index, bool Check);	
	bool                       CheckWndMaskValid(const CAOIWndMask *MaskWndPtr);	
	bool                       AddWndMaskWndPtr(CAOIWndMask *MaskWndPtr, bool Clone);
	void                       DestroyWndMaskWndSelected();	
	void                       SetWndAllMaskWndSelected(bool value);		
	bool                       CalcWndMaskWndRegion(TREGION4D &Region);//計算遮罩框區域	
	bool                       GetWndMaskWndSelectedList(std::vector<size_t> &List);
	bool                       GetWndMaskWndSelectedList(std::vector<CAOIWndMask*> &MaskWndList);
	//---------------------------------------------------------------------------------//
	bool                       VisibleWnd();//啟用檢測框顯示狀態
	bool                       UnSelectWnd();//取消檢測框選取狀態
	bool                       InvisibleWnd();//取消檢測框顯示狀態
	bool                       UnSelectWndRoi();//取消檢測框子框選取狀態
	bool                       InvisibleWndRoi();//取消檢測子框框顯示狀態
	bool                       UnSelectWndMaskBox();//取消檢測框遮罩框選取狀態
	bool                       InvisibleWndMaskBox();//取消檢測框遮罩框顯示狀態
	//---------------------------------------------------------------------------------//		
	void                       DrawWndBoxEdit(HDC hDC, TBOX_DRAW_PARAM &DrawParam) const;
	void                       DrawWndBoxResult(HDC hDC, TBOX_DRAW_PARAM &DrawParam) const;
	//---------------------------------------------------------------------------------//	
	void                       DrawWndExtBoxEdit(HDC hDC, TBOX_DRAW_PARAM &DrawParam) const;
	void                       DrawWndExtBoxResult(HDC hDC, TBOX_DRAW_PARAM &DrawParam) const;
	//---------------------------------------------------------------------------------//	
	void                       ScaleWnd(double sx, double sy, bool bIncludeRes=true);//縮放檢測框
	void                       MoveWnd(double x, double y, bool bIncludeRes=true);
	void                       MoveWnd(const TPOINT2D &Pos, bool bIncludeRes=true);
	void                       MoveWndResult(double x, double y);
	void                       MoveWndResult(const TPOINT2D &Pos);
	void                       SpinWnd(double Angle);
	void                       RotateWnd(double Angle, double CPX, double CPY);	
	void                       MirrorWndXAxis(double CPY);
	void                       MirrorWndYAxis(double CPX);
	//---------------------------------------------------------------------------------//
	void                       SetWndAttachedAngle(double Angle);
	double                     GetWndAttachedAngle() const { return m_WndBox.GetBoxAttachedAngle(); }

	void                       SetWndAttachedPosCad(const TPOINT2D &Pos);
	TPOINT2D                   GetWndAttachedPosCad() const { return m_WndBox.GetBoxAttachedPosCad(); }
	void                       GetWndAttachedPosCad(TPOINT2D &Pos) const { m_WndBox.GetBoxAttachedPosCad(Pos); }
	void                       SetWndAttachedPosCad(double PosX, double PosY);
	void                       GetWndAttachedPosCad(double &PosX, double &PosY) const { m_WndBox.GetBoxAttachedPosCad(PosX, PosY); }

	void                       SetWndAttachedPosStage(const TPOINT2D &Pos);
	TPOINT2D                   GetWndAttachedPosStage() const { return m_WndBox.GetBoxAttachedPosStage(); }
	void                       GetWndAttachedPosStage(TPOINT2D &Pos) const { m_WndBox.GetBoxAttachedPosStage(Pos); }
	void                       SetWndAttachedPosStage(double PosX, double PosY);
	void                       GetWndAttachedPosStage(double &PosX, double &PosY) const { m_WndBox.GetBoxAttachedPosStage(PosX, PosY); }
	//---------------------------------------------------------------------------------//
	CString                    GetWndAlgTypeText() const;	
	void                       SetWndAlgParam(const CAlgParam &value) { m_WndAlgParam = value; }
	CAlgParam&                 GetWndAlgParam() { return m_WndAlgParam; }
	const CAlgParam&           GetWndAlgParam() const { return m_WndAlgParam; }
	CAlgParam*                 GetWndAlgParamPtr() { return &m_WndAlgParam; }
	void                       SetWndAlgType(ALG_TYPE value) { m_WndAlgParam.SetAlgType(value); }
	ALG_TYPE                   GetWndAlgType() const { return m_WndAlgParam.GetAlgType(); }	
	void                       SetWndAlgGroupID(int value) { m_WndAlgParam.SetAlgGroupID(value); }
	int                        GetWndAlgGroupID() const { return m_WndAlgParam.GetAlgGroupID(); }		
	void                       SetWndAlgImageFrameIndex(unsigned int val);
	void                       SetWndAlgImageFrameUniqueID(unsigned int val);	
	//---------------------------------------------------------------------------------//		
	bool                       CheckWndAlgUsing3D() const;//確認檢測框演算法使用3D
	bool                       CheckWndAlgCanUsing3D() const;//確認檢測框演算法使用3D
	//---------------------------------------------------------------------------------//
	void                       SetWndResultID(RESULT_ID val) { m_WndBox.SetBoxResultID(val); }
	RESULT_ID                  GetWndResultID() const { return m_WndBox.GetBoxResultID(); }
	//---------------------------------------------------------------------------------//	
	void                       SetWndResultText(LPCTSTR val) { m_WndBox.SetBoxResultText(val); }
	LPCTSTR                    GetWndResultText() const { return m_WndBox.GetBoxResultText(); }
	//---------------------------------------------------------------------------------//	
	void                       SetWndResultValue(double val) { m_WndBox.SetBoxResultValue(val); }
	double                     GetWndResultValue() const { return m_WndBox.GetBoxResultValue(); }
	//---------------------------------------------------------------------------------//	
	bool                       CheckWndAlgAISupported() const { return m_WndAlgParam.CheckAlgAISupported(); }
	//---------------------------------------------------------------------------------//
	bool                       InitWndInspection(bool bModelInit);//初始化檢測框檢測
	bool                       CalcWndInspectRect(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const TPOINT2D &RgnCp, const TPOINT2D &Scale, const TPOINT2D &ImageCp, RECT &RoiRect, RECT &WndRect);//計算檢測框檢測區域
	bool                       ExecWndInspection(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const TPOINT2D &RgnCp, const TPOINT2D &Scale, const TPOINT2D &ImageCp, std::vector<TUNI_FRAME> &UniFrameList, bool bTestWnd=false);//執行檢測框檢測
	//---------------------------------------------------------------------------------//	
	bool                       BuildWndBoxImage(const CAOIModel *ModelPtr, const TPOINT2D &Scale, IMAGE_SIZE BitCount, IMAGE_SIZE &PatW, IMAGE_SIZE &PatH, IMAGE_SIZE &PatStep, IMAGE_PTR &PatPtr, std::vector<TREGION4D> &FeatureList);//建立檢測框-框樣板圖像	
	bool                       BuildWndRoiImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, const RECT &WndRect, int sX, int sY, bool UsResPos);//建立檢測框-框樣板圖像	
	//---------------------------------------------------------------------------------//	
	bool                       CheckWndNeedShapeMask();//確認檢測框需要外形遮罩
	bool                       BuildWndShapeMask(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_PTR& ImagePtr, const RECT &WndRect);//建立檢測框-外形遮罩圖像	
	bool                       BuildWndShapeMask3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_PTR ImagePtr, const RECT &WndRect);//建立檢測框-外形遮罩圖像	
	bool                       BuildWndShapeImage(bool bMaskWnd, BOX_TOWARD BoxToward, BOX_SHAPE_MODE BoxShapeMode, double BoxShapeParam, double BoxShapeParam2, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE BitCount, IMAGE_SIZE &ImageStep, IMAGE_PTR &ImagePtr);//建立檢測框-外形遮罩圖像		
	//---------------------------------------------------------------------------------//	
	bool                       UpdateWndFrameUniqueID(unsigned int DefaultIndex, unsigned int DefaultUniqueID, const std::vector<unsigned int> &FrameIndexMapList);
	//---------------------------------------------------------------------------------//	
	bool                       CloneWndResult(CAOIWnd *SrcWndPtr);
	//---------------------------------------------------------------------------------//
	CString                    GetWndFullName() const;//取得零件全名	
	//---------------------------------------------------------------------------------//

	//---------------------------------------------------------------------------------//
};
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_AOIWND_H__CDCA81FC_E32D_415B_AE31_0DC418D54B15__INCLUDED_)
