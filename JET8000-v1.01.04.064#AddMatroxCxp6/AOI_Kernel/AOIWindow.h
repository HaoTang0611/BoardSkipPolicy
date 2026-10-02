// AOIWindow.h: interface for the CAOIWindow class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AOIWINDOW_H__FB94A32D_6321_4FFA_9D43_5B365FD4338F__INCLUDED_)
#define AFX_AOIWINDOW_H__FB94A32D_6321_4FFA_9D43_5B365FD4338F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include "AOIRgn.h"
//-------------------------------------------------------------------------------------//
class CAOIPanel;
class CAOIBoard;
class CAOIField;
class CAOIComponent;
//-------------------------------------------------------------------------------------//
class CAOIWindow : public CAOIRgn  
{
	//---------------------------------------------------------------------------------//	
	DECLARE_DYNAMIC(CAOIWindow)
	//---------------------------------------------------------------------------------//
private:
	//---------------------------------------------------------------------------------//	
	unsigned int               m_WindowIndex;
	//---------------------------------------------------------------------------------//	
	unsigned int               m_WindowComponentIdx;
	CAOIComponent*             m_WindowComponentPtr;
	//---------------------------------------------------------------------------------//	
	bool                       m_WindowSelected;
	bool                       m_WindowDeleted;
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//
	void                       PreInitWindow();
	void                       InitialWindow();
	//---------------------------------------------------------------------------------//	
	void                       CloneWindow(const CAOIWindow &window);
public:
	//---------------------------------------------------------------------------------//	
	CAOIWindow();
	CAOIWindow(const CAOIWindow &window);
	virtual ~CAOIWindow();
	//---------------------------------------------------------------------------------//	
	CAOIWindow& operator=(const CAOIWindow &window);
	//---------------------------------------------------------------------------------//		
	void                       SetWindowIndexComponent(unsigned int val) { m_WindowIndex = val; }
	unsigned int               GetWindowIndexComponent() const { return m_WindowIndex; }
	//---------------------------------------------------------------------------------//	
	void                       SetWindowComponentPtr(CAOIComponent *Ptr);
	CAOIComponent*             GetWindowComponentPtr() const { return m_WindowComponentPtr; }
	//---------------------------------------------------------------------------------//	
	void                       SetWindowComponentIndex(unsigned int val) { m_WindowComponentIdx = val; }
	unsigned int               GetWindowComponentIndex() const { return m_WindowComponentIdx; }
	//---------------------------------------------------------------------------------//	
	void                       SetWindowFieldPtr(CAOIField* Ptr) { m_RgnFieldPtr=Ptr; }
	CAOIField*                 GetWindowFieldPtr() const { return m_RgnFieldPtr; }
	//---------------------------------------------------------------------------------//	
	void                       SetWindowSelected(bool val) { m_WindowSelected = val; }
	bool                       GetWindowSelected() const { return m_WindowSelected; }
	//---------------------------------------------------------------------------------//	
	void                       SetWindowDeleted(bool val) { m_WindowDeleted = val; }
	bool                       GetWindowDeleted() const { return m_WindowDeleted; }
	//---------------------------------------------------------------------------------//	
	//檢測框在CAD坐標系
	void                       SetWindowCadPos(const TPOINT2D &value) { m_RgnCadPos = value; }
	TPOINT2D                   GetWindowCadPos() const { return m_RgnCadPos; }
	//---------------------------------------------------------------------------------//
	void                       SetWindowCadPosX(double value) { m_RgnCadPos.x = value;} 
	double                     GetWindowCadPosX() const { return m_RgnCadPos.x; }

	void                       SetWindowCadPosY(double value) { m_RgnCadPos.y = value;}
	double                     GetWindowCadPosY() const { return m_RgnCadPos.y; }
	//---------------------------------------------------------------------------------//	
	//檢測框四端點X
	void                       SetWindowCadCornerPosX(const double value[]) 
	{ 
		m_RgnRoiCadCornerPos[0].x = value[0]; 
		m_RgnRoiCadCornerPos[1].x = value[1]; 
		m_RgnRoiCadCornerPos[2].x = value[2]; 
		m_RgnRoiCadCornerPos[3].x = value[3]; 
	}
	void                      GetWindowCadCornerPosX(double value[]) const 
	{ 
		value[0] = m_RgnRoiCadCornerPos[0].x; 
		value[1] = m_RgnRoiCadCornerPos[1].x; 
		value[2] = m_RgnRoiCadCornerPos[2].x; 
		value[3] = m_RgnRoiCadCornerPos[3].x; 
	}
	//---------------------------------------------------------------------------------//
	//檢測框四端點Y
	void                       SetWindowCadCornerPosY(const double value[]) 
	{ 
		m_RgnRoiCadCornerPos[0].y = value[0]; 
		m_RgnRoiCadCornerPos[1].y = value[1]; 
		m_RgnRoiCadCornerPos[2].y = value[2]; 
		m_RgnRoiCadCornerPos[3].y = value[3]; 
	}
	void                       GetWindowCadCornerPosY(double value[]) const 
	{ 
		value[0] = m_RgnRoiCadCornerPos[0].y; 
		value[1] = m_RgnRoiCadCornerPos[1].y; 
		value[2] = m_RgnRoiCadCornerPos[2].y; 
		value[3] = m_RgnRoiCadCornerPos[3].y; 
	}
	//---------------------------------------------------------------------------------//	
	//檢測框在CAD零件位置在機台坐標系中
	void                       SetWindowStagePos(const TPOINT3D &value) { m_RgnStagePos = value; }
	TPOINT3D                   GetWindowStagePos() const { return m_RgnStagePos; }
	//---------------------------------------------------------------------------------//
	void                       SetWindowStagePosX(double value) { m_RgnStagePos.x=value;} 
	double                     GetWindowStagePosX() const { return m_RgnStagePos.x; }

	void                       SetWindowStagePosY(double value) { m_RgnStagePos.y=value;}
	double                     GetWindowStagePosY() const { return m_RgnStagePos.y; }
	//---------------------------------------------------------------------------------//	
	void                       GetWindowStageCornerPos(TPOINT2D value[]) const 
	{
		value[0].x = m_RgnRoiStageCornerPos[0].x;	value[0].y = m_RgnRoiStageCornerPos[0].y; 	
		value[1].x = m_RgnRoiStageCornerPos[1].x;	value[1].y = m_RgnRoiStageCornerPos[1].y; 	
		value[2].x = m_RgnRoiStageCornerPos[2].x;	value[2].y = m_RgnRoiStageCornerPos[2].y; 	
		value[3].x = m_RgnRoiStageCornerPos[3].x;	value[3].y = m_RgnRoiStageCornerPos[3].y; 	
	}
	//---------------------------------------------------------------------------------//	
	//檢測框四端點在機台坐標系中-X
	void                       SetWindowStageCornerPosX(double value[]) 
	{ 
		m_RgnRoiStageCornerPos[0].x = value[0]; 
		m_RgnRoiStageCornerPos[1].x = value[1]; 
		m_RgnRoiStageCornerPos[2].x = value[2]; 
		m_RgnRoiStageCornerPos[3].x = value[3]; 
	}
	void                       GetWindowStageCornerPosX(double value[]) const 
	{ 
		value[0] = m_RgnRoiStageCornerPos[0].x; 
		value[1] = m_RgnRoiStageCornerPos[1].x; 
		value[2] = m_RgnRoiStageCornerPos[2].x; 
		value[3] = m_RgnRoiStageCornerPos[3].x; 
	}	
	//---------------------------------------------------------------------------------//
	//檢測框四端點在機台坐標系中-Y
	void                       SetWindowStageCornerPosY(double value[]) 
	{ 
		m_RgnRoiStageCornerPos[0].y = value[0]; 
		m_RgnRoiStageCornerPos[1].y = value[1]; 
		m_RgnRoiStageCornerPos[2].y = value[2]; 
		m_RgnRoiStageCornerPos[3].y = value[3]; 
	}
	void                       GetWindowStageCornerPosY(double value[]) const 
	{ 
		value[0] = m_RgnRoiStageCornerPos[0].y; 
		value[1] = m_RgnRoiStageCornerPos[1].y; 
		value[2] = m_RgnRoiStageCornerPos[2].y; 
		value[3] = m_RgnRoiStageCornerPos[3].y; 
	}
	//---------------------------------------------------------------------------------//
	void                       GetWindowRoiCadRegion(TREGION4D &Region);//取得檢測框在Cad的範圍	
	void                       GetWindowRoiStageRegion(TREGION4D &Region);//取得檢測框在Stage的範圍	
	//---------------------------------------------------------------------------------//	
	void                       CalcWindowCadCornerPos();//計算檢測框Cad端點座標	
	void                       LayoutWindowStageCornerPos();//更新檢測框機台端點座標	
	//---------------------------------------------------------------------------------//	
	//檢測框所屬FOV的CAD位置-X
	void                       SetWindowFovCadPosX(double value) { m_RgnFovCadPos.x = value; }
	double                     GetWindowFovCadPosX() const { return m_RgnFovCadPos.y; }
	//---------------------------------------------------------------------------------//
	//檢測框所屬FOV的CAD位置-Y
	void                       SetWindowFovCadPosY(double value) { m_RgnFovCadPos.y = value; }
	double                     GetWindowFovCadPosY() const { return m_RgnFovCadPos.y; }
	//---------------------------------------------------------------------------------//
	//檢測框所屬FOV的Stage位置-X
	void                       SetWindowFovStagePosX(double value) { m_RgnFovStagePos.x = value; }
	double                     GetWindowFovStagePosX() const { return m_RgnFovStagePos.x; }
	//---------------------------------------------------------------------------------//
	//檢測框所屬FOV的Stage位置-Y
	void                       SetWindowFovStagePosY(double value) { m_RgnFovStagePos.y = value; }
	double                     GetWindowFovStagePosY() const { return m_RgnFovStagePos.y; }
	//---------------------------------------------------------------------------------//
	//檢測框所屬影像的區域
	void                       SetWindowFrameImageRect(const RECT &value) { m_RgnFrameImageRect = value; }
	RECT                       GetWindowFrameImageRect() const { return m_RgnFrameImageRect; }
	//---------------------------------------------------------------------------------//	
	void                       SetWindowFrameImageSize_um(const TSIZE2D &value) { m_RgnFrameImageSize_um = value; }
	TSIZE2D                    GetWindowFrameImageSize_um() const { return m_RgnFrameImageSize_um; }
	//---------------------------------------------------------------------------------//
	//檢測框影像的區域Cad偏差-um
	void                       SetWindowFrameImageCadOffset_um(const TPOINT2D &value) { m_RgnFrameImageCadOffset_um = value; }
	const TPOINT2D&            GetWindowFrameImageCadOffset_um() const { return m_RgnFrameImageCadOffset_um; }
	void                       SetWindowFrameImageStageOffset_um(const TPOINT2D &value) { SetRgnFrameImageStageOffset_um(value); }	
	//---------------------------------------------------------------------------------//	
	void                       SetWindowFrameImageCornerPt(const POINT value[]) 
	{ 
		m_RgnFrameImageCornerPt[0] = value[0]; 
		m_RgnFrameImageCornerPt[1] = value[1]; 
		m_RgnFrameImageCornerPt[2] = value[2]; 
		m_RgnFrameImageCornerPt[3] = value[3]; 
	}
	void                       GetWindowFrameImageCornerPt(POINT value[]) const 
	{ 
		value[0] = m_RgnFrameImageCornerPt[0]; 
		value[1] = m_RgnFrameImageCornerPt[1]; 
		value[2] = m_RgnFrameImageCornerPt[2]; 
		value[3] = m_RgnFrameImageCornerPt[3]; 
	}
	//---------------------------------------------------------------------------------//	
	//檢測框所屬的Field的CAD位置-X
	void                       SetWindowFieldCadPosX(double value) { m_RgnFieldCadPos.x = value; }
	double                     GetWindowFieldCadPosX() const { return m_RgnFieldCadPos.x; }
	//---------------------------------------------------------------------------------//
	//檢測框所屬的Field的CAD位置-Y
	void                       SetWindowFieldCadPosY(double value) { m_RgnFieldCadPos.y = value; }
	double                     GetWindowFieldCadPosY() const { return m_RgnFieldCadPos.y; }
	//---------------------------------------------------------------------------------//
	//檢測框所屬的Field的Stage位置-X
	void                       SetWindowFieldStagePosX(double value) { m_RgnFieldStagePos.x = value; }
	double                     GetWindowFieldStagePosX() const { return m_RgnFieldStagePos.x; }
	//---------------------------------------------------------------------------------//
	//檢測框所屬的Field的Stage位置-Y
	void                       SetWindowFieldStagePosY(double value) { m_RgnFieldStagePos.y = value; }
	double                     GetWindowFieldStagePosY() const { return m_RgnFieldStagePos.y; }
	//---------------------------------------------------------------------------------//
	void                       MapWindowCadToStagePos(const CMapCoordinate &Map);//將檢測框CAD轉成機台座標
	//---------------------------------------------------------------------------------//
	bool                       SpinWindow(double Angle);//檢測框自旋轉
	bool                       RotateWindowCad(double Angle, double CpX, double CpY);//檢測框旋轉
	bool                       RotateWindowStage(double Angle, double CpX, double CpY);//檢測框旋轉
	bool                       MirrorXWindowCad(double CpX);//檢測框鏡射-X
	bool                       MirrorYWindowCad(double CpY);//檢測框鏡射-Y
	bool                       MirrorXWindowStage(double CpX);//檢測框鏡射-X
	bool                       MirrorYWindowStage(double CpY);//檢測框鏡射-Y
	//---------------------------------------------------------------------------------//	
	virtual CString            GetRgnDerivedName() const;//取得檢測框的名稱	
	virtual CString            GetRgnDerivedKeyName() const;//取得檢測框的名稱	
	virtual bool               ExtractRgnDerivedFrame(bool &Finished);//挖取檢測框圖片
	virtual bool               ExecRgnDerivedInspection();//執行檢測框檢測	
	//---------------------------------------------------------------------------------//
	bool                       CreateWindowSubRgnList(FIELD_BUILD_MODE BuildMode, bool ByCadRegion);//建立檢測框子檢測區域列表
	void                       ClearWindowSubRgnList();//清除檢測框的子列表
	size_t                     GetWindowSubRgnCount() const;//取得檢測框的子數量
	CAOIRgn*                   GetWindowSubRgnPtr(size_t index, bool check) const;//取得檢測框的子指標		
	//---------------------------------------------------------------------------------//
	void                       InitWindowInspection();//初始化檢測框檢測
	bool                       ExecWindowInspection();//執行檢測框檢測
	//---------------------------------------------------------------------------------//
};
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_AOIWINDOW_H__FB94A32D_6321_4FFA_9D43_5B365FD4338F__INCLUDED_)
