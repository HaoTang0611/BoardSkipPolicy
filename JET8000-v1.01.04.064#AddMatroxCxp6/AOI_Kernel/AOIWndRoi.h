// AOIWndRoi.h: interface for the CAOIWndRoi class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AOIWNDROI_H__D6A13DAE_7E17_4251_BEC0_C2F1DB5A2B68__INCLUDED_)
#define AFX_AOIWNDROI_H__D6A13DAE_7E17_4251_BEC0_C2F1DB5A2B68__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include "AOIObj.h"
#include "AOIBox.h"
#include "AlgParamDef.h"
#include "AlgBinaryParam.h"
//-------------------------------------------------------------------------------------//
class CAOIWnd;
//-------------------------------------------------------------------------------------//
class CAOIWndRoi : public CAOIObj  
{
	//---------------------------------------------------------------------------------//
	DECLARE_DYNAMIC(CAOIWndRoi)
	//---------------------------------------------------------------------------------//	
	//---------------------------------------------------------------------------------//	
private:
	//---------------------------------------------------------------------------------//
	ALG_TYPE                   m_WndRoiAlgType;              //子框-演算法
	CAOIBox                    m_WndRoiBox;                  //子框-內框	
	RECT                       m_WndRoiImageRect;            //子框-影像位置
	//---------------------------------------------------------------------------------//	
	unsigned int               m_WndRoiIndex;                //子框-引數編號
	CAOIWnd                   *m_WndRoiWndPtr;               //子框-所屬的檢測框
	//---------------------------------------------------------------------------------//	
	bool                       m_WndRoiSelfFrameEnabled;     //子框-自有畫面啟用
	//---------------------------------------------------------------------------------//	
	bool                       m_WndRoiBinaryParamEnabled;   //子框-二值化參數啟用
	CAlgBinaryParam            m_WndRoiBinaryParam;          //子框-二值化參數
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//	
	void                       PreInitWndRoi();
	void                       InitialWndRoi();
	void                       CloneWndRoi(const CAOIWndRoi &WndRoi);
	//---------------------------------------------------------------------------------//
	bool                       CheckWndRoiBinaryParamEnabled(ALG_TYPE AlgType);//確認演算法是否支援子框2值化
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//	
	CAOIWndRoi();
	CAOIWndRoi(const CAOIWndRoi &WndRoi);
	virtual ~CAOIWndRoi();
	CAOIWndRoi& operator=(const CAOIWndRoi &WndRoi);
	//---------------------------------------------------------------------------------//	
	CAOIWndRoi*                CloneWndRoiObj() const;//建立且複製一個框
	//---------------------------------------------------------------------------------//
	bool                       WriteWndRoiFile(CAOIFileIO &FileIO);//儲存檢測框檔案
	bool                       ReadWndRoiFile(CAOIFileIO &FileIO);//載入檢測框檔案
	//---------------------------------------------------------------------------------//
	void                       SetWndRoiAlgType(ALG_TYPE AlgType);
	ALG_TYPE                   GetWndRoiAlgType() const { return m_WndRoiAlgType; }	
	//---------------------------------------------------------------------------------//
	const CAOIBox&             GetWndRoiBox() const { return m_WndRoiBox; }
	CAOIBox*                   GetWndRoiBoxPtr() { return &m_WndRoiBox; }
	//---------------------------------------------------------------------------------//	
	void                       SetWndRoiIndex(unsigned int value);
	unsigned int               GetWndRoiIndex() const { return m_WndRoiIndex; }
	//---------------------------------------------------------------------------------//
	void                       SetWndRoiWndPtr(CAOIWnd *Ptr) { m_WndRoiWndPtr = Ptr; }
	CAOIWnd*                   GetWndRoiWndPtr() { return m_WndRoiWndPtr; }
	//---------------------------------------------------------------------------------//
	bool                       VisibleWndRoi();//啟用子框顯示狀態
	bool                       UnSelectWndRoi();//取消子框選取狀態
	bool                       InvisibleWndRoi();//取消子框顯示狀態
	//---------------------------------------------------------------------------------//		
	void                       SetWndRoiToward(BOX_TOWARD value) { m_WndRoiBox.SetBoxToward(value); }
	BOX_TOWARD                 GetWndRoiToward() const { return m_WndRoiBox.GetBoxToward(); }
	//---------------------------------------------------------------------------------//
	void                       SetWndRoiActived(bool value) { m_WndRoiBox.SetBoxActived(value); }
	bool                       GetWndRoiActived() const { return m_WndRoiBox.GetBoxActived(); }
	//---------------------------------------------------------------------------------//
	void                       SetWndRoiSelected(bool value) { m_WndRoiBox.SetBoxSelected(value); }
	bool                       GetWndRoiSelected() const { return m_WndRoiBox.GetBoxSelected(); }
	//---------------------------------------------------------------------------------//
	void                       SetWndRoiVisibled(bool value) { m_WndRoiBox.SetBoxVisibled(value); }
	bool                       GetWndRoiVisibled() const { return m_WndRoiBox.GetBoxVisibled(); }
	//---------------------------------------------------------------------------------//
	void                       SetWndRoiEnabled(bool value) { m_WndRoiBox.SetBoxEnabled(value); }
	bool                       GetWndRoiEnabled() const { return m_WndRoiBox.GetBoxEnabled();; }
	//---------------------------------------------------------------------------------//
	void                       SetWndRoiEditabled(bool value) { m_WndRoiBox.SetBoxEditabled(value); }
	bool                       GetWndRoiEditabled() const { return m_WndRoiBox.GetBoxEditabled(); }	
	//---------------------------------------------------------------------------------//
	void                       GetWndRoiCornerPos(TPOINT2D CornerPos[]) const { m_WndRoiBox.GetBoxCornerPos(CornerPos); }	
	void                       GetWndRoiCornerPosRes(TPOINT2D CornerPos[]) const { m_WndRoiBox.GetBoxCornerPosRes(CornerPos); }		
	void                       GetWndRoiCornerPosCad(TPOINT2D CornerPos[]) const { m_WndRoiBox.GetBoxCornerPosCad(CornerPos); }	
	void                       GetWndRoiCornerPosCadRes(TPOINT2D CornerPos[]) const { m_WndRoiBox.GetBoxCornerPosCadRes(CornerPos); }	
	void                       GetWndRoiCornerPosStage(TPOINT2D CornerPos[]) const { m_WndRoiBox.GetBoxCornerPosStage(CornerPos); }	
	void                       GetWndRoiCornerPosStageRes(TPOINT2D CornerPos[]) const { m_WndRoiBox.GetBoxCornerPosStageRes(CornerPos); }	
	//---------------------------------------------------------------------------------//
	void                       SetWndRoiRegion(const TREGION4D &Region, bool IncludeRes=true);
	void                       SetWndRoiRegion(double MinX, double MinY, double MaxX, double MaxY, bool IncludeRes=true);	

	void                       SetWndRoiRegionRes(const TREGION4D &Region);	
	void                       SetWndRoiRegionRes(double MinX, double MinY, double MaxX, double MaxY);	

	void                       GetWndRoiRegion(TREGION4D &Region) const { m_WndRoiBox.GetBoxRegion(Region); }		
	void                       GetWndRoiRegionRes(TREGION4D &Region) const { m_WndRoiBox.GetBoxRegionRes(Region); }
	void                       GetWndRoiRegionCad(TREGION4D &Region) const { m_WndRoiBox.GetBoxRegionCad(Region); }
	void                       GetWndRoiRegionCadRes(TREGION4D &Region) const { m_WndRoiBox.GetBoxRegionCadRes(Region); }
	void                       GetWndRoiRegionStage(TREGION4D &Region) const { m_WndRoiBox.GetBoxRegionStage(Region); }		
	void                       GetWndRoiRegionStageRes(TREGION4D &Region) const { m_WndRoiBox.GetBoxRegionStageRes(Region); }	
	void                       GetWndRoiRegion(double &MinX, double &MinY, double &MaxX, double &MaxY) const { m_WndRoiBox.GetBoxRegion(MinX, MinY, MaxX, MaxY); }	
	//---------------------------------------------------------------------------------//
	void                       SetWndRoiImageRect(const RECT &Rect) { 	m_WndRoiImageRect = Rect; }
	void                       GetWndRoiImageRect(RECT &Rect) { Rect=m_WndRoiImageRect; }		
	//---------------------------------------------------------------------------------//
	CAlgBinaryParam&           GetWndRoiBinaryParam();
	CAlgBinaryParam*           GetWndRoiBinaryParamPtr();
	void                       SetWndRoiBinaryParam(const CAlgBinaryParam &BinParam);	
	//---------------------------------------------------------------------------------//
	//自有畫面啟用
	void                       SetWndRoiSelfFrameEnabled(bool val) { m_WndRoiSelfFrameEnabled=val; }
	bool                       GetWndRoiSelfFrameEnabled() const { return m_WndRoiSelfFrameEnabled; }
	//---------------------------------------------------------------------------------//
	//子框-二值化參數啟用
	bool                       GetWndRoiBinaryParamEnabled() const { return m_WndRoiBinaryParamEnabled; }
	//---------------------------------------------------------------------------------//
	void                       SetWndRoiResultID(RESULT_ID val) { m_WndRoiBox.SetBoxResultID(val); }
	RESULT_ID                  GetWndRoiResultID() const { return m_WndRoiBox.GetBoxResultID(); }
	//---------------------------------------------------------------------------------//	
	void                       SetWndRoiResultText(LPCTSTR val) { m_WndRoiBox.SetBoxResultText(val); }
	LPCTSTR                    GetWndRoiResultText() const { return m_WndRoiBox.GetBoxResultText(); }
	//---------------------------------------------------------------------------------//
	void                       SetWndRoiResultValue(double value) { m_WndRoiBox.SetBoxResultValue(value); }
	double                     GetWndRoiResultValue() const { return m_WndRoiBox.GetBoxResultValue(); }
	//---------------------------------------------------------------------------------//
	bool                       ApplyWndRoi(const CAOIWndRoi *RefWndRoiPtr, int TowardAngle);
	bool                       SynchronousWndRoi(const CAOIWndRoi *RefWndRoiPtr);	
	//---------------------------------------------------------------------------------//	
	void                       ScaleWndRoi(double sx, double sy, bool bIncludeRes=true);//縮放檢測框
	void                       MoveWndRoi(double x, double y, bool bIncludeRes=true);
	void                       MoveWndRoi(const TPOINT2D &Pos, bool bIncludeRes=true);
	void                       MoveWndRoiResult(double x, double y);
	void                       MoveWndRoiResult(const TPOINT2D &Pos);
	void                       SpinWndRoi(double Angle);
	void                       RotateWndRoi(double Angle, double CPX, double CPY);	
	void                       MirrorWndRoiXAxis(double CPY);
	void                       MirrorWndRoiYAxis(double CPX);
	//---------------------------------------------------------------------------------//	
	void                       SetWndRoiAttachedAngle(double Angle) { m_WndRoiBox.SetBoxAttachedAngle(Angle); }
	double                     GetWndRoiAttachedAngle() const { return m_WndRoiBox.GetBoxAttachedAngle(); }

	void                       SetWndRoiAttachedPosCad(const TPOINT2D &Pos) { m_WndRoiBox.SetBoxAttachedPosCad(Pos); }
	void                       GetWndRoiAttachedPosCad(TPOINT2D &Pos) const { m_WndRoiBox.GetBoxAttachedPosCad(Pos); }
	void                       SetWndRoiAttachedPosCad(double PosX, double PosY) { m_WndRoiBox.SetBoxAttachedPosCad(PosX, PosY); }
	void                       GetWndRoiAttachedPosCad(double &PosX, double &PosY) const { m_WndRoiBox.GetBoxAttachedPosCad(PosX, PosY); }

	void                       SetWndRoiAttachedPosStage(const TPOINT2D &Pos) { m_WndRoiBox.SetBoxAttachedPosStage(Pos);	 }
	void                       GetWndRoiAttachedPosStage(TPOINT2D &Pos) const { m_WndRoiBox.GetBoxAttachedPosStage(Pos); }
	void                       SetWndRoiAttachedPosStage(double PosX, double PosY) { m_WndRoiBox.SetBoxAttachedPosStage(PosX, PosY);	 }
	void                       GetWndRoiAttachedPosStage(double &PosX, double &PosY) const { m_WndRoiBox.GetBoxAttachedPosStage(PosX, PosY); }
	//---------------------------------------------------------------------------------//
	void                       DrawWndRoiBoxEdit(HDC hDC, const TBOX_DRAW_PARAM &DrawParam) const;
	void                       DrawWndRoiBoxResult(HDC hDC, const TBOX_DRAW_PARAM &DrawParam) const;
	//---------------------------------------------------------------------------------//
	bool                       InitWndRoiInspection(bool bModelInit);//初始化檢測框檢測
	//---------------------------------------------------------------------------------//
	bool                       SetWndRoiBoxAlgDrawParam(HDC hDC, TBOX_DRAW_PARAM &BoxDrawParam);//根據演算法設定Roi顯示樣式
	bool                       CheckWndRoiBoxAlgUseExtend();//根據演算法設定Roi顯示樣式
	//---------------------------------------------------------------------------------//
};
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_AOIWNDROI_H__D6A13DAE_7E17_4251_BEC0_C2F1DB5A2B68__INCLUDED_)
