// AOIField.h: interface for the CAOIField class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AOIFIELD_H__90051F0D_3F17_4D8E_BE7A_803463EEDA04__INCLUDED_)
#define AFX_AOIFIELD_H__90051F0D_3F17_4D8E_BE7A_803463EEDA04__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
//實際劃分的視野區域
//-------------------------------------------------------------------------------------//
#include "AOIObj.h"
#include "AOIFrame.h"
#include "MapCoordinate.h"
//-------------------------------------------------------------------------------------//
enum FIELD_LIST_MODE
{
	FIELD_LIST_NONE      = 0,
	FIELD_LIST_PROGRAM   = 1,
	FIELD_LIST_INSPECTION = 2
};
//-------------------------------------------------------------------------------------//
enum FIELD_MERGE_STATE   //區域合併狀態
{
	FIELD_MERGE_NONE   = 0,  //尚未合併影像
	FIELD_MERGE_DOING  = 1,  //合併中	
	FIELD_MERGE_DONE   = 2,  //合併完畢	
	FIELD_MERGE_CLEAR  = 9,  //合併清除
	FIELD_MERGE_RETURN
};
//-------------------------------------------------------------------------------------//
enum FIELD_CALC_STATE   //區域計算狀態
{
	FIELD_CALC_NONE   = 0,  //尚未區域計算
	FIELD_CALC_DOING  = 1,  //計算中
	FIELD_CALC_DONE   = 2,  //計算完畢
	FIELD_CALC_CLEAR  = 9   //計算清除
};
//-------------------------------------------------------------------------------------//
class CAOIRgn;
class CAOIFov;
class CAOIBoard;
class CAOIPanel;
class CAOIProject;
class CAOIComponent;
class CAOIFileIO;
//-------------------------------------------------------------------------------------//
class CAOIField : public CAOIObj  //區域
{
	//---------------------------------------------------------------------------------//	
	DECLARE_DYNAMIC(CAOIField)
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//
	static CRITICAL_SECTION    m_csField;//同步機制-關鍵區間
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	static void                InitialFieldLock();//初始化區域的關鍵區間
	static void                DeleteFieldLock(); //刪除區域的關鍵區間
	static void                LockField();        //進入區域的關鍵區間
	static void                UnlockField();      //離開區域的關鍵區間
	//---------------------------------------------------------------------------------//	
private:
	//---------------------------------------------------------------------------------//	
	unsigned int               m_FieldIndex;//區域的引數編號
	FIELD_LIST_MODE            m_FieldListMode;//區域所屬的列表模式
	unsigned int               m_FieldHeightID;//區域的高度編號
	//---------------------------------------------------------------------------------//
	CAOIProject*               m_FieldProjectPtr;//區域的專案指標
	//---------------------------------------------------------------------------------//	
	unsigned int               m_FieldPanelIdx;//區域的整板引數
	CAOIPanel*                 m_FieldPanelPtr;//區域的整板指標
	//---------------------------------------------------------------------------------//
	unsigned int               m_FieldBoardIdx;//區域的單板引數
	CAOIBoard*                 m_FieldBoardPtr;//區域的單板指標
	//---------------------------------------------------------------------------------//
	unsigned int               m_FieldComponentIdx;//區域的零件引數
	CAOIComponent*             m_FieldComponentPtr;//區域的零件指標
	//---------------------------------------------------------------------------------//	
	size_t                     m_FieldGrabIndex;//區域的取像次序
	size_t                     m_FieldGrabIndexByUser;//區域的取像次序
	//---------------------------------------------------------------------------------//
	int                        m_FieldTempInt;//區域暫時用變數
	//---------------------------------------------------------------------------------//
	bool                       m_FieldSelected;//區域的選取狀態
	bool                       m_FieldLinkPointer;//連結指標, 如果是的話不要刪除
	bool                       m_FieldMustToLoad;//區域的必定要載入圖檔	
	bool                       m_FieldLockRelease;//區域的鎖住釋放
	bool                       m_FieldIsInSaveList;//區域已加入儲存列表
	bool                       m_FieldPartImageMode;//區域的零件影像模式
	double                     m_FieldPanelBasePlane;//區域的整板基準面
	DISTRICT_ID                m_FieldDistrictID;//區域的多段編號
	//---------------------------------------------------------------------------------//		
	TPOINT2D                   m_FieldCadPos;//區域在Cad的位置	
	TPOINT3D                   m_FieldStagePos;//區域在Stage的位置	
	//---------------------------------------------------------------------------------//
	TSIZE2D                    m_FieldSize_Inner;//區域的空間內部尺寸um
	TSIZE2D                    m_FieldSize_Outer;//區域的空間外部尺寸um	
	TSIZE2D                    m_FieldSize_Real;//區域的空間全部尺寸um	
	//---------------------------------------------------------------------------------//		
	double                     m_FieldMergeTime;//區域合併花費時間
	FIELD_CALC_STATE           m_FieldCalcState;//區域的計算狀態
	FIELD_MERGE_STATE          m_FieldMergeState;//區域的合併狀態
	//---------------------------------------------------------------------------------//
	std::vector<CAOIRgn*>      m_FieldRgnPtrList;//區域內的檢測指標列表
	std::vector<CAOIBoard*>    m_FieldBoardPtrList;//區域內的單板指標列表
	std::vector<CAOIFrame*>    m_FieldFramePtrList;//區域內的影像指標列表
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//	
	void                       PreInitField();
	void                       InitialField();
	void                       CloneField(const CAOIField &field);
	//---------------------------------------------------------------------------------//
	void                       AddFieldFramePtr_Inline(CAOIFrame *Ptr);//增加區域的影像指標
	size_t                     GetFieldFramePtrCount_Inline() const;//取得區域的影像指標數量
	CAOIFrame*                 GetFieldFramePtr_Inline(size_t index);//取得區域的影像指標	
	void                       RemoveFieldAllFrames_Inline();//移除區域的所有影像	
	//---------------------------------------------------------------------------------//
	int                        GetFieldOpenMPCount() const;//取回區域的OpenMP數量
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	CAOIField();
	CAOIField(const CAOIField &field);
	virtual ~CAOIField();	
	CAOIField& operator=(const CAOIField &field);
	//---------------------------------------------------------------------------------//
	CAOIField*                 CloneFieldObj() const;//建立且複製一個區域
	//---------------------------------------------------------------------------------//
	bool                       WriteFieldFile(CAOIFileIO &FileIO);//儲存區域
	bool                       ReadFieldFile(CAOIFileIO &FileIO);//載入區域
	//---------------------------------------------------------------------------------//	
	void                       SetFieldIndex(unsigned int value) { m_FieldIndex = value; }
	unsigned int               GetFieldIndex() const { return m_FieldIndex; }
	//---------------------------------------------------------------------------------//	
	void                       SetFieldHeightID(unsigned int value) { m_FieldHeightID = value; }
	unsigned int               GetFieldHeightID() const { return m_FieldHeightID; }
	//---------------------------------------------------------------------------------//
	void                       SetFieldListMode(FIELD_LIST_MODE value) { m_FieldListMode = value; }
	FIELD_LIST_MODE            GetFieldListMode() const { return m_FieldListMode; }
	//---------------------------------------------------------------------------------//	
	void                       SetFieldProjectPtr(CAOIProject* value) { m_FieldProjectPtr = value; }
	CAOIProject*               GetFieldProjectPtr() const { return m_FieldProjectPtr; }
	//---------------------------------------------------------------------------------//
	void                       SetFieldPanelIndex(unsigned int value) { m_FieldPanelIdx = value; }
	unsigned int               GetFieldPanelIndex() const { return m_FieldPanelIdx; }
	//---------------------------------------------------------------------------------//	
	void                       SetFieldPanelPtr(CAOIPanel* value) { m_FieldPanelPtr = value; }
	CAOIPanel*                 GetFieldPanelPtr() const { return m_FieldPanelPtr; }
	//---------------------------------------------------------------------------------//
	void                       SetFieldBoardIndex(unsigned int value) { m_FieldBoardIdx = value; }
	unsigned int               GetFieldBoardIndex() const { return m_FieldBoardIdx; }
	//---------------------------------------------------------------------------------//	
	void                       SetFieldBoardPtr(CAOIBoard* value) { m_FieldBoardPtr = value; }
	CAOIBoard*                 GetFieldBoardPtr() const { return m_FieldBoardPtr; }
	//---------------------------------------------------------------------------------//	
	void                       SetFieldComponentIndex(unsigned int value) { m_FieldComponentIdx = value; }
	unsigned int               GetFieldComponentIndex() const { return m_FieldComponentIdx; }
	//---------------------------------------------------------------------------------//	
	void                       SetFieldComponentPtr(CAOIComponent* value) { m_FieldComponentPtr = value; }
	CAOIComponent*             GetFieldComponentPtr() const { return m_FieldComponentPtr; }
	//---------------------------------------------------------------------------------//
	void                       SetFieldSelected(bool value) { m_FieldSelected = value; }
	bool                       GetFieldSelected() const { return m_FieldSelected; }
	//---------------------------------------------------------------------------------//
	void                       SetFieldLinkPointer(bool value) { m_FieldLinkPointer = value; }
	bool                       GetFieldLinkPointer() const { return m_FieldLinkPointer; }
	//---------------------------------------------------------------------------------//
	//區域的必定要載入圖檔
	void                       SetFieldMustToLoad(bool value) { m_FieldMustToLoad = value; }
	bool                       GetFieldMustToLoad() const { return m_FieldMustToLoad; }
	//---------------------------------------------------------------------------------//
	//區域的鎖住釋放
	void                       SetFieldLockRelease(bool value) { m_FieldLockRelease = value; }
	bool                       GetFieldLockRelease() const { return m_FieldLockRelease; }
	//---------------------------------------------------------------------------------//
	//區域已加入儲存列表
	void                       SetFieldIsInSaveList(bool value) { m_FieldIsInSaveList = value; }
	bool                       GetFieldIsInSaveList() const { return m_FieldIsInSaveList; }
	//---------------------------------------------------------------------------------//
	//區域的零件影像模式
	void                       SetFieldPartImageMode(bool value) { m_FieldPartImageMode = value; }
	bool                       GetFieldPartImageMode() const { return m_FieldPartImageMode; }
	//---------------------------------------------------------------------------------//
	//區域的整板基準面
	void                       SetFieldPanelBasePlane(double value) { m_FieldPanelBasePlane = value; }
	double                     GetFieldPanelBasePlane() const { return m_FieldPanelBasePlane; }
	//---------------------------------------------------------------------------------//	 
	//區域的多段編號
	void                       SetFieldDistrictID(DISTRICT_ID value) { m_FieldDistrictID = value; }
	DISTRICT_ID                GetFieldDistrictID() const { return m_FieldDistrictID; }
	//---------------------------------------------------------------------------------//	
	//區域的取像次序
	void                       SetFieldGrabIndex(size_t value) { m_FieldGrabIndex = value; }
	size_t                     GetFieldGrabIndex() const { return m_FieldGrabIndex; }
	//---------------------------------------------------------------------------------//
	//區域的取像次序-使用者定義
	void                       SetFieldGrabIndexByUser(size_t value) { m_FieldGrabIndexByUser = value; }
	size_t                     GetFieldGrabIndexByUser() const { return m_FieldGrabIndexByUser; }
	//---------------------------------------------------------------------------------//	
	//區域已經加入路徑列表
	void                       SetFieldTempInt(int value) { m_FieldTempInt = value; }
	int                        GetFieldTempInt() const { return m_FieldTempInt; }
	//---------------------------------------------------------------------------------//
	void                       SetFieldCadPos(const TPOINT2D &value) { m_FieldCadPos = value; }
	TPOINT2D                   GetFieldCadPos() const { return m_FieldCadPos; }

	void                       SetFieldCadPosX(double value) { m_FieldCadPos.x = value; }
	double                     GetFieldCadPosX() const { return m_FieldCadPos.x; }

	void                       SetFieldCadPosY(double value) { m_FieldCadPos.y = value; }
	double                     GetFieldCadPosY() const { return m_FieldCadPos.y; }
	//---------------------------------------------------------------------------------//	
	void                       SetFieldStagePos(const TPOINT3D &value) { m_FieldStagePos = value; }
	TPOINT3D                   GetFieldStagePos() const { return m_FieldStagePos; }

	void                       SetFieldStagePosX(double value) { m_FieldStagePos.x = value; }
	double                     GetFieldStagePosX() const { return m_FieldStagePos.x; }

	void                       SetFieldStagePosY(double value) { m_FieldStagePos.y = value; }
	double                     GetFieldStagePosY() const { return m_FieldStagePos.y; }

	void                       SetFieldStagePosZ(double value) { m_FieldStagePos.z = value; }
	double                     GetFieldStagePosZ() const { return m_FieldStagePos.z; }
	//---------------------------------------------------------------------------------//	  	
	void                       SetFieldSize_Inner(double w, double h);
	void                       SetFieldSize_Inner(const TSIZE2D &value) { m_FieldSize_Inner = value;}
	void                       SetFieldSizeW_Inner(double value) { m_FieldSize_Inner.cx = value; }
	void                       SetFieldSizeH_Inner(double value) { m_FieldSize_Inner.cy = value; }
	double                     GetFieldSizeW_Inner() const { return m_FieldSize_Inner.cx; }
	double                     GetFieldSizeH_Inner() const { return m_FieldSize_Inner.cy; }
	//---------------------------------------------------------------------------------//	  
	void                       SetFieldSize_Outer(double w, double h);
	void                       SetFieldSize_Outer(const TSIZE2D &value) { m_FieldSize_Outer = value;}
	void                       SetFieldSizeW_Outer(double value) { m_FieldSize_Outer.cx = value; }
	void                       SetFieldSizeH_Outer(double value) { m_FieldSize_Outer.cy = value; }
	double                     GetFieldSizeW_Outer() const { return m_FieldSize_Outer.cx; }
	double                     GetFieldSizeH_Outer() const { return m_FieldSize_Outer.cy; }
	//---------------------------------------------------------------------------------//
	void                       SetFieldSize_Real(double w, double h);
	void                       SetFieldSize_Real(const TSIZE2D &value) { m_FieldSize_Real = value;}
	void                       SetFieldSizeW_Real(double value) { m_FieldSize_Real.cx = value; }
	void                       SetFieldSizeH_Real(double value) { m_FieldSize_Real.cy = value; }
	double                     GetFieldSizeW_Real() const { return m_FieldSize_Real.cx; }
	double                     GetFieldSizeH_Real() const { return m_FieldSize_Real.cy; }
	//---------------------------------------------------------------------------------//	
	void                       GetFieldCadRgn_Inner(TREGION4D &Rgn) const;
	void                       GetFieldCadRgn_Outer(TREGION4D &Rgn) const;
	void                       GetFieldCadRgn_Real(TREGION4D &Rgn) const;	
	//---------------------------------------------------------------------------------//
	void                       GetFieldStageRgn_Inner(TREGION4D &Rgn) const;
	void                       GetFieldStageRgn_Outer(TREGION4D &Rgn) const;
	void                       GetFieldStageRgn_Real(TREGION4D &Rgn) const;
	//---------------------------------------------------------------------------------//
	bool                       CalcFieldImageSize(const TPOINT2D &Res, IMAGE_SIZE &W, IMAGE_SIZE &H) const;//計算視野影像大小
	//---------------------------------------------------------------------------------//	  	
	void                       MapFieldCadToStagePos(const CMapCoordinate &Map);//將CAD轉成機台座標
	void                       MapFieldStageToCadPos(const CMapCoordinate &Map);//將機台轉成CAD座標
	void                       MoveFieldPos(double dX, double dY, CMapCoordinate *MapPtr);//移動區域座標	
	//---------------------------------------------------------------------------------//
	bool                       AddFieldRgnPtr(CAOIRgn *Ptr);//增加區域的檢測區域指標
	bool                       RemoveFieldRgnPtr(CAOIRgn *Ptr);//移除區域的檢測區域指標
	size_t                     GetFieldRgnPtrCount() const;//取得區域的檢測區域指標數量
	CAOIRgn*                   GetFieldRgnPtr(size_t index, bool check);//取得區域的檢測區域指標	
	void                       RemoveFieldAllRgns();//移除區域的所有檢測區域
	void                       LayoutFieldRgnPtrList();//排列區域內的檢測區域指標
	//---------------------------------------------------------------------------------//	
	bool                       CheckFieldUsing3D();//確認區域使用3D影像
	//---------------------------------------------------------------------------------//	
	bool                       AddFieldBoardPtr(CAOIBoard *Ptr);//增加區域的單板指標
	bool                       RemoveFieldBoardPtr(CAOIBoard *Ptr);//移除區域的單板指標
	size_t                     GetFieldBoardPtrCount() const;//取得區域的單板指標數量
	CAOIBoard*                 GetFieldBoardPtr(size_t index, bool check);//取得區域的單板指標	
	void                       RemoveFieldAllBoards();//移除區域的所有單板	
	//---------------------------------------------------------------------------------//	
	bool                       AddFieldFramePtr(CAOIFrame *Ptr);//增加區域的影像指標
	size_t                     GetFieldFramePtrCount() const;//取得區域的影像指標數量
	CAOIFrame*                 GetFieldFramePtr(size_t index, bool check);//取得區域的影像指標	
	void                       ClearFieldAllFrames();//清除區域的所有影像
	void                       RemoveFieldAllFrames();//移除區域的所有影像	
	bool                       SetFieldFrameCalcState(FRAME_CALC_STATE val);//設定區域影像計算狀態
	//---------------------------------------------------------------------------------//	
	bool                       CheckFieldMergeFinish();//確認區域內所有影像都已取得	
	bool                       ExecFieldCalc(unsigned int ThreadIdx);//執行區域的計算		
	bool                       CheckFieldRgnFinish();//確認區域內所有區域都已經計算完畢	
	bool                       CheckFieldFramesMergeFinish();//確認區域內所有影像都已取得	
	bool                       CheckFieldFramesCalcFinish();//確認區域內所有影像都已計算完成
	size_t                     CalcFieldRgnUnCalculatedCount();//計算區域內未計算的區域數量
	//---------------------------------------------------------------------------------//
	double                     GetFieldMergeTime() const;
	void                       SetFieldMergeTime(double value);	
	//---------------------------------------------------------------------------------//	
	FIELD_CALC_STATE           GetFieldCalcState() const;
	bool                       CheckFieldCalcDone() const;
	void                       SetFieldCalcState(FIELD_CALC_STATE value);	
	//---------------------------------------------------------------------------------//	
	FIELD_MERGE_STATE          GetFieldMergeState() const;
	void                       SetFieldMergeState(FIELD_MERGE_STATE value);	
	//---------------------------------------------------------------------------------//
	bool                       CheckRegionInField(const TREGION4D &Region, bool CadMode, bool Inner);//確認區域是否在此圖像區域
	bool                       CheckRegionPartInField(const TREGION4D &Region, bool CadMode, bool Inner);//確認區域是否部分在此圖像區域
	//---------------------------------------------------------------------------------//	
	bool                       CheckFieldFrameMatchFrameParam(std::vector<TFrameParam> &FrameParamList);//確認區域內的所有影像等同於目前的影像參數列表
	//---------------------------------------------------------------------------------//
	bool                       SaveFieldFrames_Offline();//儲存區域影像_離線編程
	bool                       MergeFieldFrames();//執行區域多重聚焦影像		
	//---------------------------------------------------------------------------------//		
	bool                       CombineFrameMultiFocusImage();//區域多重焦點(高度)影像
	//---------------------------------------------------------------------------------//	
	CString                    GetFieldIndexName() const;//取得Field引數名稱
	//---------------------------------------------------------------------------------//		
};
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_AOIFIELD_H__90051F0D_3F17_4D8E_BE7A_803463EEDA04__INCLUDED_)
