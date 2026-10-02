// AOIBox.h: interface for the CAOIBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AOIBOX_H__84CC292B_53A6_4F36_A4DE_C68239587955__INCLUDED_)
#define AFX_AOIBOX_H__84CC292B_53A6_4F36_A4DE_C68239587955__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include "AOIObj.h"
#include "AOIRgn.h"
//-------------------------------------------------------------------------------------//
class CAOIFileIO;
//-------------------------------------------------------------------------------------//
typedef struct tagBOX_DRAW_PARAM//模組繪圖參數
{	
	TPOINT2D                   Zoom;	                     //縮放比例
	POINT                      ViewCP;                       //顯示中心
	TPOINT2D                   Extend;                       //繪製外擴範圍 
	BOX_TOWARD                 Toward;                       //繪製朝向特徵
	POINT                      WndCP;                        //視窗區域中心
	RECT                       WndRect;	                     //視窗區域	
	TPOINT2D                   ModelCP;                      //模組幾何中心 	
	BOX_SHAPE_MODE             ShapeMode;                    //外型模式
	TPOINT2D                   ViewOffset;                   //顯示的偏移量
	double                     LineScale;                    //繪製的線寬
	bool                       DrawEditLine;                 //是否繪製編輯線段 
	double                     ComponentAngle;
	bool                       IsExceptionAngle;	         //是否為特殊角度
	int                        TowardSize;                   //朝向的尺寸
	int                        EditLineSize;                 //編輯格的尺寸
	bool                       DrawResultText;               //繪製結果文字
	bool                       FillRegion;                   //填滿區域
	HPEN                       hPenNull;                     //空筆
	HBRUSH                     hBrushMask;                   //畫遮罩的Brush
	bool                       DrawTowardFeature;            //是否畫朝向特徵
	bool                       DrawBoxIndex;                 //顯示文字-編號
	CString                    strBoxIndex;                  //文字-編號
	tagBOX_DRAW_PARAM()
	{
		Toward = BOX_TOWARD_NULL;		
		Zoom.x = Zoom.y = 1;
		ShapeMode = BOX_SHAPE_RECTANGLE;
		LineScale = 1.0;
		DrawEditLine = false;
		ComponentAngle = 0.0;
		IsExceptionAngle = false;
		TowardSize = 7;
		EditLineSize = 4;
		WndCP.x = WndCP.y = 0;
		WndRect.left = WndRect.right = WndRect.top = WndRect.bottom = 0;
		DrawResultText = false;
		FillRegion = false;
		hPenNull = NULL;
		hBrushMask = NULL;
		DrawTowardFeature = true;
		DrawBoxIndex = false;
		strBoxIndex = _T("");
	}

	double RealToImageX(double val) const//um換成影像座標-X
	{	return ((val-ModelCP.x)/Zoom.x + ViewOffset.x) + WndCP.x + ViewCP.x;	}
	double RealToImageY(double val) const//um換成影像座標-Y
	{	return WndRect.bottom-(((val-ModelCP.y)/Zoom.y + ViewOffset.y) + WndCP.y) + ViewCP.y;	}

} TBOX_DRAW_PARAM, *PBOX_DRAW_PARAM;
 
//-------------------------------------------------------------------------------------//
class CAOIBox : public CAOIObj
//class CAOIBox : public CAOIRgn
{
	DECLARE_DYNAMIC(CAOIBox)	
	//---------------------------------------------------------------------------------//
	static bool                CheckBoxShapeUseParam1(BOX_SHAPE_MODE ShapeMode);//確認框外型是否使用參數1
	static bool                CheckBoxShapeUseParam2(BOX_SHAPE_MODE ShapeMode);//確認框外型是否使用參數2
	//---------------------------------------------------------------------------------//
	static int                 CalcRoundRectRectRadius(const RECT &Rect, double Param);//計算矩形圓角半徑
	static int                 CalcHalfRoundRectRectRadius(const RECT &Rect, double Param, BOX_TOWARD Toward);//計算矩形半圓角半徑
	static int                 CalcBoxTowardAngle(BOX_TOWARD TowardFrom, BOX_TOWARD TowardTo);//取得兩個朝向的角度	

	static void                DrawRectangleRect(HDC hDC, const RECT &Rect, HPEN hPen, HBRUSH hBrush);//繪製矩形	
	static void                DrawRoundRectRect(HDC hDC, const RECT &Rect, double Param, HPEN hPen, HBRUSH hBrush);//繪製圓矩形
	static void                DrawHalfRoundRectRect(HDC hDC, const RECT &Rect, double Param, BOX_TOWARD Toward, HPEN hPen, HBRUSH hBrush);//繪製半圓矩形
	static void                DrawEllipseRect(HDC hDC, const RECT &Rect, HPEN hPen, HBRUSH hBrush);//繪製橢圓形
	static void                DrawCapsuleRect(HDC hDC, const RECT &Rect, HPEN hPen, HBRUSH hBrush);//繪製膠囊型
	static void                DrawBulletRect(HDC hDC, const RECT &Rect, BOX_TOWARD Toward, HPEN hPen, HBRUSH hBrush);//繪製子彈形
	static void                DrawTShapeRect(HDC hDC, const RECT &Rect, double Param, double Param2, BOX_TOWARD Toward, HPEN hPen, HBRUSH hBrush);//繪製T形

	static void                DrawRectangleLine(HDC hDC, const RECT &Rect, BOX_TOWARD Toward=BOX_TOWARD_NULL, int TowardSize=7);//繪製矩形	
	static void                DrawRoundRectLine(HDC hDC, const RECT &Rect, double Param, BOX_TOWARD Toward=BOX_TOWARD_NULL, int TowardSize=7);//繪製圓矩形
	static void                DrawHalfRoundRectLine(HDC hDC, const RECT &Rect, double Param, BOX_TOWARD Toward=BOX_TOWARD_NULL, int TowardSize=7);//繪製半圓矩形
	static void                DrawEllipseLine(HDC hDC, const RECT &Rect, BOX_TOWARD Toward=BOX_TOWARD_NULL, int TowardSize=7);//繪製橢圓形
	static void                DrawCapsuleLine(HDC hDC, const RECT &Rect, BOX_TOWARD Toward=BOX_TOWARD_NULL, int TowardSize=7);//繪製膠囊型
	static void                DrawBulletLine(HDC hDC, const RECT &Rect, BOX_TOWARD Toward=BOX_TOWARD_NULL, int TowardSize=7);//繪製子彈形
	static void                DrawTShapeRectLine(HDC hDC, const RECT &Rect, double Param, double Param2, BOX_TOWARD Toward=BOX_TOWARD_NULL, int TowardSize=7);//繪製半圓矩形

	static void                DrawTowardFeature(HDC hDC, const RECT &Rect, BOX_TOWARD Toward, int Size=7);//繪製朝向特徵
	static void                DrawTriangleLine(HDC hDC, const POINT PTList[]);//繪製三角形
	static void                DrawEditRect(HDC hDC, const RECT &EditRect, int HalfSize);//繪製選取編輯框	
	static void                DrawEditRect(HDC hDC, const POINT CornPoint[], int HalfSize);//繪製選取編輯框	
	//---------------------------------------------------------------------------------//
private:
	//---------------------------------------------------------------------------------//		
	BOX_TOWARD                 m_BoxToward;                  //框朝向
	BOX_SHAPE_MODE             m_BoxShapeMode;               //框形狀
	double                     m_BoxShapeParam;              //框形狀參數
	double                     m_BoxShapeParam2;             //框形狀參數
	bool                       m_BoxMaskEraseMode;           //清除遮罩
	//---------------------------------------------------------------------------------//
	unsigned int               m_BoxIndex;                   //佇列引數
	bool                       m_BoxEnabled;                 //是否啟用
	bool                       m_BoxActived;                 //是否焦點中
	bool                       m_BoxSelected;                //是否選取中
	bool                       m_BoxVisibled;                //是否顯示中	
	bool                       m_BoxEditabled;               //是否可編輯	
	//---------------------------------------------------------------------------------//
	double                     m_BoxAngle;                   //框角度
	double                     m_BoxAngleRes;                //框角度
	double                     m_BoxAngleSkew;               //框角度偏差

	TSIZE2D                    m_BoxSize;                    //框尺寸
	TSIZE2D                    m_BoxSizeRes;                 //框尺寸-結果

	//Model
	TPOINT2D                   m_BoxPos;                     //框座標-模組內-編輯
	TPOINT2D                   m_BoxPosRes;                  //框座標-模組內-結果
	TPOINT2D                   m_BoxCornerPos[4];            //框角落座標-模組內
	TPOINT2D                   m_BoxCornerPosRes[4];         //框角落座標-模組內-結果
	
	double                     m_BoxAttachedAngle;          //框所屬零件角度
	TPOINT2D                   m_BoxAttachedPosCad;         //框所屬零件座標-Cad內
	TPOINT2D                   m_BoxAttachedPosStage;       //框所屬零件座標-Cad內
	std::vector<float>         m_BoxAttachedHeight;			//框所屬零件高度
	std::vector<float>         m_BoxEdgePadHeight;			//框所屬零件邊緣物質高度

	//CAD
	TPOINT2D                   m_BoxPosCad;                  //框座標-Cad內
	TPOINT2D                   m_BoxPosCadRes;               //框座標-Cad內-結果
	TPOINT2D                   m_BoxCornerPosCad[4];         //框角落座標-Cad內
	TPOINT2D                   m_BoxCornerPosCadRes[4];      //框角落座標-Cad內-結果

	//Stage
	TPOINT2D                   m_BoxPosStage;                //框座標-機台內
	TPOINT2D                   m_BoxPosStageRes;             //框座標-機台內-結果
	TPOINT2D                   m_BoxCornerPosStage[4];       //框角落座標-機台內
	TPOINT2D                   m_BoxCornerPosStageRes[4];    //框角落座標-機台內-結果
	//---------------------------------------------------------------------------------//	
	RESULT_ID                  m_BoxResultID;                //框結果編號
	CString                    m_BoxResultText;              //框結果文字
	double                     m_BoxResultValue;             //框結果數據
	bool                       m_BoxResultTextVisibled;      //框結果文字顯示
	//---------------------------------------------------------------------------------//
	std::vector<TPOINT2D>      m_BoxPolygon;                 //框多邊形線段
	bool                       m_BoxPolygonVisibled;         //框多邊形顯示
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//	
	void                       PreInitBox();
	void                       InitialBox();
	void                       CloneBox(const CAOIBox &Box);		
	//---------------------------------------------------------------------------------//
	void                       UpdateBoxCornerPos();//更新框的角落座標
	void                       UpdateBoxCornerPosRes();//更新框的角落座標-結果
	//---------------------------------------------------------------------------------//
	void                       UpdateBoxAttachedPosCadRes();//更新框的零件座標-CAD
	void                       UpdateBoxAttachedPosCad(bool bIncludeRes);//更新框的零件座標-CAD
	void                       UpdateBoxAttachedPosStageRes();//更新框的零件座標-機台
	void                       UpdateBoxAttachedPosStage(bool bIncludeRes);//更新框的零件座標-機台
	//---------------------------------------------------------------------------------//
	bool                       CheckDrawBoxText(const RECT &Rect) const;
	void                       DrawBoxEditKernel_1(HDC hDC, const TBOX_DRAW_PARAM &DrawParam) const;
	void                       DrawBoxEditKernel_2(HDC hDC, const TBOX_DRAW_PARAM &DrawParam) const;
	void                       DrawBoxResultKernel_1(HDC hDC, const TBOX_DRAW_PARAM &DrawParam) const;
	void                       DrawBoxResultKernel_2(HDC hDC, const TBOX_DRAW_PARAM &DrawParam) const;
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	CAOIBox();
	CAOIBox(const CAOIBox &Box);
	virtual ~CAOIBox();
	CAOIBox& operator=(const CAOIBox &Box);
	//---------------------------------------------------------------------------------//
	CAOIBox*                   CloneBoxObj() const;//建立且複製一個框
	//---------------------------------------------------------------------------------//
	bool                       WriteBoxFile(CAOIFileIO &FileIO);//儲存框檔案
	bool                       ReadBoxFile(CAOIFileIO &FileIO);//載入框檔案
	//---------------------------------------------------------------------------------//	
	void                       SetBoxToward(BOX_TOWARD value) { m_BoxToward = value; }
	BOX_TOWARD                 GetBoxToward() const { return m_BoxToward; }
	//---------------------------------------------------------------------------------//
	void                       SetBoxShapeMode(BOX_SHAPE_MODE value) { m_BoxShapeMode = value; }
	BOX_SHAPE_MODE             GetBoxShapeMode() const { return m_BoxShapeMode; }
	//---------------------------------------------------------------------------------//	
	void                       SetBoxShapeParam(double value) { m_BoxShapeParam = value; }
	double                     GetBoxShapeParam() const { return m_BoxShapeParam; }
	//---------------------------------------------------------------------------------//		
	void                       SetBoxShapeParam2(double value) { m_BoxShapeParam2 = value; }
	double                     GetBoxShapeParam2() const { return m_BoxShapeParam2; }
	//---------------------------------------------------------------------------------//
	void                       SetBoxMaskEraseMode(bool value) { m_BoxMaskEraseMode = value; }
	bool                       GetBoxMaskEraseMode() const { return m_BoxMaskEraseMode; }
	//---------------------------------------------------------------------------------//	
	//佇列引數
	void                       SetBoxIndex(unsigned int value) { m_BoxIndex = value; }
	unsigned int			   GetBoxIndex() const { return m_BoxIndex; }
	//---------------------------------------------------------------------------------//	
	void                       SetBoxEnabled(bool value) { m_BoxEnabled = value; }
	bool					   GetBoxEnabled() const { return m_BoxEnabled; }
	//---------------------------------------------------------------------------------//		
	void                       SetBoxActived(bool value) { m_BoxActived = value; }
	bool					   GetBoxActived() const { return m_BoxActived; }
	//---------------------------------------------------------------------------------//	
	void                       SetBoxSelected(bool value) { m_BoxSelected = value; }
	bool					   GetBoxSelected() const { return m_BoxSelected; }
	//---------------------------------------------------------------------------------//	
	void                       SetBoxVisibled(bool value) { m_BoxVisibled = value; }
	bool					   GetBoxVisibled() const { return m_BoxVisibled; }
	//---------------------------------------------------------------------------------//	
	void                       SetBoxEditabled(bool value) { m_BoxEditabled = value; }
	bool					   GetBoxEditabled() const { return m_BoxEditabled; }
	//---------------------------------------------------------------------------------//
	void                       SetBoxAngle(double value) { m_BoxAngle = value; }
	double					   GetBoxAngle() const { return m_BoxAngle; }
	//---------------------------------------------------------------------------------//	
	void                       SetBoxAngleRes(double value) { m_BoxAngleRes = value; }
	double					   GetBoxAngleRes() const { return m_BoxAngleRes; }	
	//---------------------------------------------------------------------------------//	
	void                       SetBoxAngleSkew(double value) { m_BoxAngleSkew = value; }
	double					   GetBoxAngleSkew() const { return m_BoxAngleSkew; }
	//---------------------------------------------------------------------------------//
	void                       SetBoxPosX(double value) { m_BoxPos.x = value; }
	void                       SetBoxPosY(double value) { m_BoxPos.y = value; }
	void                       SetBoxPos(const TPOINT2D &Pos, bool bIncludeRes=true);
	void                       SetBoxPos(double px, double py, bool bIncludeRes=true);

	void                       SetBoxPosResX(double value) { m_BoxPosRes.x = value; }
	void                       SetBoxPosResY(double value) { m_BoxPosRes.y = value; }
	void                       SetBoxPosRes(const TPOINT2D &Pos);
	void                       SetBoxPosRes(double px, double py);
	//---------------------------------------------------------------------------------//	
	void                       SetBoxSizeX(double value) { m_BoxSize.cx = value; }
	void                       SetBoxSizeY(double value) { m_BoxSize.cy = value; }
	void                       SetBoxSize(const TSIZE2D &Sz, bool bIncludeRes=true);
	void                       SetBoxSize(double cx, double cy, bool bIncludeRes=true);
	
	void                       SetBoxSizeRes(double szX, double szY, bool UpdateCorner);
	void                       SetBoxSizeResX(double value) { m_BoxSizeRes.cx = value; }
	void                       SetBoxSizeResY(double value) { m_BoxSizeRes.cy = value; }	
	void                       ResetBoxRegionRes();
	//---------------------------------------------------------------------------------//
	double                     GetBoxPosX() const { return m_BoxPos.x; }
	double                     GetBoxPosY() const { return m_BoxPos.y; }
	void                       GetBoxPos(TPOINT2D &Pos) const { Pos = m_BoxPos; }	
	void                       GetBoxPos(double &Px, double &Py) const { Px=m_BoxPos.x; Py=m_BoxPos.y;	}
	void                       GetBoxPosRes(TPOINT2D &Pos) const { Pos = m_BoxPosRes; }
	void                       GetBoxPosRes(double &Px, double &Py) const { Px=m_BoxPosRes.x; Py=m_BoxPosRes.y;	}
	double                     GetBoxPosCadX() const { return m_BoxPosCad.x; }	
	double                     GetBoxPosCadY() const { return m_BoxPosCad.y; }	
	void                       GetBoxPosCad(TPOINT2D &Pos) const { Pos = m_BoxPosCad; }	
	double                     GetBoxPosCadResX() const { return m_BoxPosCadRes.x; }
	double                     GetBoxPosCadResY() const { return m_BoxPosCadRes.y; }
	void                       GetBoxPosCadRes(TPOINT2D &Pos) const { Pos = m_BoxPosCadRes; }	
	void                       GetBoxPosStage(TPOINT2D &Pos) const { Pos = m_BoxPosStage; }	
	void                       GetBoxPosStageRes(TPOINT2D &Pos) const { Pos = m_BoxPosStageRes; }		
	//---------------------------------------------------------------------------------//
	double                     GetBoxSizeX() const { return m_BoxSize.cx; }
	double                     GetBoxSizeY() const { return m_BoxSize.cy; }
	void                       GetBoxSize(TSIZE2D &Sz) const { Sz = m_BoxSize; }
	void                       GetBoxSize(double &Cx, double &Cy) const { Cx=m_BoxSize.cx; Cy=m_BoxSize.cy;	}
	void                       GetBoxSizeRes(TSIZE2D &Sz) const { Sz = m_BoxSizeRes; }
	void                       GetBoxSizeRes(double &Cx, double &Cy) const { Cx=m_BoxSizeRes.cx; Cy=m_BoxSizeRes.cy;	}
	//---------------------------------------------------------------------------------//
	void                       SetBoxRegion(const TREGION4D &Rgn, bool bIncludeRes=true);
	void                       SetBoxRegion(double MinX, double MinY, double MaxX, double MaxY, bool bIncludeRes=true);

	void                       SetBoxRegionRes(const TREGION4D &Rgn);
	void                       SetBoxRegionRes(double MinX, double MinY, double MaxX, double MaxY);
	//---------------------------------------------------------------------------------//	
	void                       LayoutBoxCornerPos(bool bIncludeRes=true);	
	void                       ModifyBoxRegion(const TREGION4D &dRgn, bool bIncludeRes=true);
	void                       ModifyBoxRegion(double dMinX, double dMinY, double dMaxX, double dMaxY, bool bIncludeRes=true);
	void                       ModifyBoxRegionRes(const TREGION4D &dRgn);
	void                       ModifyBoxRegionRes(double dMinX, double dMinY, double dMaxX, double dMaxY);
	//---------------------------------------------------------------------------------//
	void                       GetBoxRegion(TREGION4D &Region) const;
	void                       GetBoxRegion(double &MinX, double &MinY, double &MaxX, double &MaxY) const;
	void                       GetBoxRegionRes(TREGION4D &Region) const;
	void                       GetBoxRegionRes(double &MinX, double &MinY, double &MaxX, double &MaxY) const;
	void                       GetBoxCornerPos(TPOINT2D CornerPos[]) const;
	void                       GetBoxCornerPosRes(TPOINT2D CornerPos[]) const;

	void                       GetBoxRegionCad(TREGION4D &Region) const;
	void                       GetBoxRegionCadRes(TREGION4D &Region) const;
	void                       GetBoxCornerPosCad(TPOINT2D CornerPos[]) const;
	void                       GetBoxCornerPosCadRes(TPOINT2D CornerPos[]) const;	

	void                       GetBoxRegionStage(TREGION4D &Region) const;
	void                       GetBoxRegionStageRes(TREGION4D &Region) const;
	void                       GetBoxCornerPosStage(TPOINT2D CornerPos[]) const;
	void                       GetBoxCornerPosStageRes(TPOINT2D CornerPos[]) const;
	//---------------------------------------------------------------------------------//
	//框結果編號
	void                       SetBoxResultID(RESULT_ID val) { m_BoxResultID=val; }
	RESULT_ID                  GetBoxResultID() const { return m_BoxResultID; }
	//---------------------------------------------------------------------------------//
	//框結果文字
	void                       SetBoxResultText(LPCTSTR val) { m_BoxResultText=val; }
	LPCTSTR                    GetBoxResultText() const { return m_BoxResultText; }
	//---------------------------------------------------------------------------------//
	//框結果數據
	void                       SetBoxResultValue(double val) { m_BoxResultValue=val; }
	double                     GetBoxResultValue() const { return m_BoxResultValue; }
	//---------------------------------------------------------------------------------//
	//框結果文字顯示
	void                       SetBoxResultTextVisibled(bool val) { m_BoxResultTextVisibled=val; }
	bool                       GetBoxResultTextVisibled() const { return m_BoxResultTextVisibled; }
	//---------------------------------------------------------------------------------//
	//框多邊形線段
	void                       ClearBoxPolygon() { m_BoxPolygon.clear(); }
	void                       AddBoxPolygonPt(const TPOINT2D &Pt) { m_BoxPolygon.push_back(Pt); }
	size_t                     GetBoxPolygonPtCount() const { return m_BoxPolygon.size(); }
	const TPOINT2D*            GetBoxPolygonPt(size_t idx, bool bChk) const
	{
		if ( true == bChk )
		{
			const size_t Cnt=m_BoxPolygon.size();
			if ( idx >= Cnt )
			{	return NULL; }
		}
		return &m_BoxPolygon[idx];
	}
	bool                       RotateBoxPolygon(double Angle, double CPX, double CPY);
	void                       SetBoxPolygon(const std::vector<TPOINT2D> &Polygon) { m_BoxPolygon = Polygon;  }
	void                       GetBoxPolygon(std::vector<TPOINT2D> &Polygon) const { Polygon=m_BoxPolygon;  }
	//---------------------------------------------------------------------------------//	
	//框多邊形顯示
	void                       SetBoxPolygonVisibled(bool val) { m_BoxPolygonVisibled=val; }
	bool                       GetBoxPolygonVisibled() const { return m_BoxPolygonVisibled; }
	//---------------------------------------------------------------------------------//		
	virtual bool               CheckBoxBePickByCad(const TPOINT2D &pt, bool IsExceptionAngle) const;
	virtual bool               CheckBoxBePickByCadRes(const TPOINT2D &pt, bool IsExceptionAngle) const;
	virtual bool               CheckBoxBePickByStage(const TPOINT2D &pt, bool IsExceptionAngle) const;
	virtual bool               CheckBoxBePickByStageRes(const TPOINT2D &pt, bool IsExceptionAngle) const;
	
	virtual bool               CheckBoxInRegionByCad(const TREGION4D &SelRgn, bool IsExceptionAngle, bool bEntireIn) const;
	virtual bool               CheckBoxInRegionByCadRes(const TREGION4D &SelRgn, bool IsExceptionAngle, bool bEntireIn) const;
	virtual bool               CheckBoxInRegionByStage(const TREGION4D &SelRgn, bool IsExceptionAngle, bool bEntireIn) const;
	virtual bool               CheckBoxInRegionByStageRes(const TREGION4D &SelRgn, bool IsExceptionAngle, bool bEntireIn) const;
	//---------------------------------------------------------------------------------//
	virtual void               MoveBox(double x, double y, bool bIncludeRes=true);		
	virtual void               MoveBox(const TPOINT2D &Pos, bool bIncludeRes=true);	
	virtual void               SkewBoxAngle(double Skew);
	virtual void               MoveBoxRes(double x, double y);
	virtual void               MoveBoxRes(const TPOINT2D &Pos);
	virtual void               ScaleBox(double sx, double sy, bool bIncludeRes=true);
	virtual void               ScaleBoxRes(double sx, double sy);
	virtual void               ScaleBoxSize(double sx, double sy, bool bIncludeRes=true);
	virtual void               ScaleBoxSizeRes(double sx, double sy);
	virtual void               RotateBox(double Angle, double CPX, double CPY, bool bIncludeRes=true);
	virtual bool               MirrorBoxXAxis(double CPY);
	virtual bool               MirrorBoxYAxis(double CPX);
	//---------------------------------------------------------------------------------//
	virtual void               DrawBoxEdit(HDC hDC, const TBOX_DRAW_PARAM &DrawParam) const;
	virtual void               DrawBoxResult(HDC hDC, const TBOX_DRAW_PARAM &DrawParam) const;
	//---------------------------------------------------------------------------------//
	//零件相關
	void                       SetBoxAttachedAngle(double Angle) { m_BoxAttachedAngle = Angle; }
	double                     GetBoxAttachedAngle() const { return m_BoxAttachedAngle; }
	
	void                       SetBoxAttachedPosCad(const TPOINT2D &Pos);//設定所屬零件的座標-Cad
	TPOINT2D                   GetBoxAttachedPosCad() const { return m_BoxAttachedPosCad; }
	void                       GetBoxAttachedPosCad(TPOINT2D &Pos) const { Pos = m_BoxAttachedPosCad; }	
	void                       SetBoxAttachedPosCad(double PosX, double PosY);//設定所屬零件的座標-Cad
	void                       GetBoxAttachedPosCad(double &PosX, double &PosY) const { PosX=m_BoxAttachedPosCad.x; PosY=m_BoxAttachedPosCad.y; }//設定所屬零件的座標-Cad
	void                       SetBoxHeight(const float height) { m_BoxAttachedHeight.push_back(height); }
	std::vector<float>         GetBoxHeight() { return m_BoxAttachedHeight; }
	void                       SetBoxEdgePadHeight(const float height) { m_BoxEdgePadHeight.push_back(height); }
	std::vector<float>         GetBoxEdgePadHeight() { return m_BoxEdgePadHeight; }
	void					   ResetBoxHeight() { m_BoxAttachedHeight.clear(); m_BoxEdgePadHeight.clear(); }

	void                       SetBoxAttachedPosStage(const TPOINT2D &Pos);//設定所屬零件的座標-Stage	
	TPOINT2D                   GetBoxAttachedPosStage() const { return m_BoxAttachedPosStage; }
	void                       GetBoxAttachedPosStage(TPOINT2D &Pos) const { Pos = m_BoxAttachedPosStage; }
	void                       SetBoxAttachedPosStage(double PosX, double PosY);//設定所屬零件的座標-Stage	
	void                       GetBoxAttachedPosStage(double &PosX, double &PosY) const { PosX=m_BoxAttachedPosStage.x; PosY=m_BoxAttachedPosStage.y; }//設定所屬零件的座標-Stage	
	//---------------------------------------------------------------------------------//
	virtual bool               ApplyBox(const CAOIBox *RefBoxPtr);
	virtual bool               SynchronousBox(const CAOIBox *RefBoxPtr);
	//---------------------------------------------------------------------------------//	
};
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_AOIBOX_H__84CC292B_53A6_4F36_A4DE_C68239587955__INCLUDED_)
