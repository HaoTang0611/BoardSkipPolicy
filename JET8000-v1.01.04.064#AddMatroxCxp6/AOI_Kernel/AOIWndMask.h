// AOIWndMask.h: interface for the CAOIWndMask class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AOIWNDMASK_H__0C16B1FD_58C9_49FC_A9B4_F7EF7E928A28__INCLUDED_)
#define AFX_AOIWNDMASK_H__0C16B1FD_58C9_49FC_A9B4_F7EF7E928A28__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include "AOIObj.h"
#include "AOIBox.h"
//-------------------------------------------------------------------------------------//
class CAOIWndMask : public CAOIObj  
{
	//---------------------------------------------------------------------------------//
	DECLARE_DYNAMIC(CAOIWndMask)
	//---------------------------------------------------------------------------------//	
private:
	//---------------------------------------------------------------------------------//	
	CAOIBox                    m_WndMaskBox;                  //遮罩框-內框	
	unsigned int               m_WndMaskIndex;                //遮罩框-引數	
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//		
	void                       PreInitWndMask();
	void                       InitialWndMask();
	void                       CloneWndMask(const CAOIWndMask &WndMask);	
	//---------------------------------------------------------------------------------//	
public:
	//---------------------------------------------------------------------------------//	
	CAOIWndMask();
	CAOIWndMask(const CAOIWndMask &WndMask);
	virtual ~CAOIWndMask();
	//---------------------------------------------------------------------------------//	
	CAOIWndMask& operator=(const CAOIWndMask &WndMask);
	//---------------------------------------------------------------------------------//
	CAOIWndMask*               CloneWndMaskObj() const;//建立且複製一個框
	//---------------------------------------------------------------------------------//	
	bool                       WriteWndMaskFile(CAOIFileIO &FileIO);//儲存檢測框檔案
	bool                       ReadWndMaskFile(CAOIFileIO &FileIO);//載入檢測框檔案
	//---------------------------------------------------------------------------------//
	CAOIBox*                   GetWndMaskBoxPtr();//取得遮罩框的Box
	//---------------------------------------------------------------------------------//
	unsigned int               GetWndMaskIndex() const;//取得遮罩框引數
	void                       SetWndMaskIndex(unsigned int val);//設定遮罩框引數
	//---------------------------------------------------------------------------------//
	BOX_TOWARD                 GetWndMaskToward() const;//取得遮罩框朝向
	void                       SetWndMaskToward(BOX_TOWARD val);//設定遮罩框朝向
	//---------------------------------------------------------------------------------//	
	BOX_SHAPE_MODE             GetWndMaskShapeMode() const;//取得遮罩框外型
	void                       SetWndMaskShapeMode(BOX_SHAPE_MODE val);	//設定遮罩框外型
	//---------------------------------------------------------------------------------//	
	double                     GetWndMaskShapeParam() const;//取得遮罩框外型參數
	void                       SetWndMaskShapeParam(double val);	//設定遮罩框外型參數
	//---------------------------------------------------------------------------------/
	double                     GetWndMaskShapeParam2() const;//取得遮罩框外型參數-2
	void                       SetWndMaskShapeParam2(double val);	//設定遮罩框外型參數-2
	//---------------------------------------------------------------------------------/
	bool					   GetWndMaskEnabled() const;//取得是否啟用
	void                       SetWndMaskEnabled(bool val);	//取得是否啟用
	//---------------------------------------------------------------------------------//	
	bool					   GetWndMaskActived() const;//取得是否主要操作
	void                       SetWndMaskActived(bool val);//取得是否主要操作
	//---------------------------------------------------------------------------------//	
	bool                       GetWndMaskVisibled() const;//取得是否顯示
	void                       SetWndMaskVisibled(bool val);//設定是否顯示
	//---------------------------------------------------------------------------------//
	bool                       GetWndMaskSelected() const;//取得是否選取到
	void                       SetWndMaskSelected(bool val);//設定是否選取到 
	//---------------------------------------------------------------------------------//
	bool					   GetWndMaskEditabled() const;//取得是否可編輯
	void                       SetWndMaskEditabled(bool val);//設定是否可編輯
	//---------------------------------------------------------------------------------//
	bool					   GetWndMaskEraseMode() const;//取得是否清除遮罩
	void                       SetWndMaskEraseMode(bool val);//設定是否清除遮罩
	//---------------------------------------------------------------------------------//	/
	double                     GetWndMaskPosX() const;//取得遮罩框座標-X
	double                     GetWndMaskPosY() const;//取得遮罩框座標-Y
	void                       GetWndMaskPos(TPOINT2D &Pos) const;//取得遮罩框座標
	void                       GetWndMaskPos(double &Px, double &Py) const;//取得遮罩框座標
	void                       GetWndMaskPosRes(TPOINT2D &Pos) const;//取得遮罩框座標結果
	void                       GetWndMaskPosRes(double &Px, double &Py) const;//取得遮罩框座標結果
	void                       GetWndMaskPosCad(TPOINT2D &Pos) const;//取得遮罩框座標--Cad
	void                       GetWndMaskPosCadRes(TPOINT2D &Pos) const;//取得遮罩框座標-Cad結果
	void                       GetWndMaskPosStage(TPOINT2D &Pos) const;//取得遮罩框座標-機台結果
	void                       GetWndMaskPosStageRes(TPOINT2D &Pos) const;//取得遮罩框座標-機台結果
	//---------------------------------------------------------------------------------//
	void                       GetWndMaskCornerPos(TPOINT2D CornerPos[]) const;//取得遮罩框4端點
	void                       GetWndMaskCornerPosRes(TPOINT2D CornerPos[]) const;//取得遮罩框4端點
	void                       GetWndMaskCornerPosCad(TPOINT2D CornerPos[]) const;//取得遮罩框4端點
	void                       GetWndMaskCornerPosCadRes(TPOINT2D CornerPos[]) const;//取得遮罩框4端點
	void                       GetWndMaskCornerPosStage(TPOINT2D CornerPos[]) const;//取得遮罩框4端點
	void                       GetWndMaskCornerPosStageRes(TPOINT2D CornerPos[]) const;//取得遮罩框4端點
	//---------------------------------------------------------------------------------//	
	void                       ResetWndMaskRegionRes();
	//---------------------------------------------------------------------------------//	
	void                       GetWndMaskRegion(TREGION4D &Region) const;
	void                       SetWndMaskRegion(const TREGION4D &Region);
	//---------------------------------------------------------------------------------//	
	void                       GetWndMaskRegionRes(TREGION4D &Region) const;
	//---------------------------------------------------------------------------------//
	void                       MoveWndMask(double x, double y, bool bIncludeRes=true);//移動遮罩框
	void                       MoveWndMask(const TPOINT2D &Pos, bool bIncludeRes=true);//移動遮罩框
	void                       SkewWndMaskAngle(double Skew);//旋轉遮罩框
	void                       MoveWndMaskRes(double x, double y);//移動遮罩框結果
	void                       MoveWndMaskRes(const TPOINT2D &Pos);//移動遮罩框結果
	void                       ScaleWndMask(double sx, double sy, bool bIncludeRes=true);//縮放遮罩框
	void                       SpinWndMask(double Angle, bool bIncludeRes=true);//自轉遮罩框
	void                       RotateWndMask(double Angle, double CPX, double CPY, bool bIncludeRes=true);//旋轉遮罩框
	bool                       MirrorWndMaskXAxis(double CPY);//鏡射遮罩框X軸
	bool                       MirrorWndMaskYAxis(double CPX);//鏡射遮罩框Y軸
	//---------------------------------------------------------------------------------//
	void                       DrawWndMaskEdit(HDC hDC, const TBOX_DRAW_PARAM &DrawParam) const;
	void                       DrawWndMaskResult(HDC hDC, const TBOX_DRAW_PARAM &DrawParam) const;
	//---------------------------------------------------------------------------------//
	//零件相關
	double                     GetWndMaskAttachedAngle() const;
	void                       SetWndMaskAttachedAngle(double Angle);	
	
	TPOINT2D                   GetWndMaskAttachedPosCad() const;
	void                       GetWndMaskAttachedPosCad(TPOINT2D &Pos) const;
	void                       SetWndMaskAttachedPosCad(const TPOINT2D &Pos);//設定所屬零件的座標-Cad	
	void                       GetWndMaskAttachedPosCad(double &PosX, double &PosY) const;
	void                       SetWndMaskAttachedPosCad(double PosX, double PosY);//設定所屬零件的座標-Cad	

	TPOINT2D                   GetWndMaskAttachedPosStage() const;
	void                       GetWndMaskAttachedPosStage(TPOINT2D &Pos) const;
	void                       SetWndMaskAttachedPosStage(const TPOINT2D &Pos);//設定所屬零件的座標-Stage			
	void                       SetWndMaskAttachedPosStage(double PosX, double PosY);//設定所屬零件的座標-Stage	
	void                       GetWndMaskAttachedPosStage(double &PosX, double &PosY) const;
	//---------------------------------------------------------------------------------//
	bool                       ApplyWndMask(const CAOIWndMask *RefWndMaskPtr, int TowardAngle);
	bool                       SynchronousWndMask(const CAOIWndMask *RefWndMaskPtr);
	//---------------------------------------------------------------------------------//

};
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_AOIWNDMASK_H__0C16B1FD_58C9_49FC_A9B4_F7EF7E928A28__INCLUDED_)
