// AOIFov.h: interface for the CAOIFov class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AOIFOV_H__2DD461BD_7107_4297_9D0F_D4E940FF8D1F__INCLUDED_)
#define AFX_AOIFOV_H__2DD461BD_7107_4297_9D0F_D4E940FF8D1F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
//主要是相機位置看到的視野, 其可以包含1至多個Field
//掃描式: 1個FOV包含多個Field
//走停式: 1個FOV包含多個Field
//-------------------------------------------------------------------------------------//
#include <vector>
#include "AOIObj.h"
#include "AOISlice.h"
#include "AOIField.h"
#include "AOIFrame.h"
//-------------------------------------------------------------------------------------//
class CAOIFov : public CAOIObj  
{
protected:
	//---------------------------------------------------------------------------------//
	static CRITICAL_SECTION    m_csFov;//同步機制-關鍵區間
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	static void                InitialFovLock();//初始化視野的關鍵區間
	static void                DeleteFovLock(); //刪除視野的關鍵區間
	static void                LockFov();        //進入視野的關鍵區間
	static void                UnlockFov();      //離開視野的關鍵區間
	//---------------------------------------------------------------------------------//
private:
	//---------------------------------------------------------------------------------//
	unsigned int               m_FovIndex;//Fov的引數編號		
	DWORD                      m_FovGrabMode;//FOV取像模式
	TPOINT2D                   m_FovCadPos;//Fov在Cad的位置	
	TPOINT3D                   m_FovStagePos;//Fov在Stage的位置	
	bool                       m_FovUsing3D;//Fov有使用3D
	//---------------------------------------------------------------------------------//
	int                        m_FovTempInt;//Fov的暫存整數
	//---------------------------------------------------------------------------------//
	std::vector<TFrameParam>   m_FovFrameParamList;//影像參數列表
	std::vector<TSliceParam>   m_FovSliceParamList;//單1影像參數列表
	//---------------------------------------------------------------------------------//		
	std::vector<CAOIFrame*>    m_FovFramePtrList;//Fov的畫面指標列表
	std::vector<CAOIField*>    m_FovFieldPtrList;//Fov的相機影像區域指標列表
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//	
	void                       PreInitFov();
	void                       InitialFov();
	void                       CloneFov(const CAOIFov &fov);
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	CAOIFov();
	CAOIFov(const CAOIFov &fov);
	virtual ~CAOIFov();
	CAOIFov& operator=(const CAOIFov &fov);
	//---------------------------------------------------------------------------------//
	CAOIFov*                   CloneFovObj() const;//建立且複製一個視野
	//---------------------------------------------------------------------------------//	
	void                       SetFovIndex(unsigned int value) { m_FovIndex = value; }
	unsigned int               GetFovIndex() const { return m_FovIndex; }
	//---------------------------------------------------------------------------------//
	void                       SetFovGrabMode(DWORD value) { m_FovGrabMode = value; }
	DWORD                      GetFovGrabMode() const { return m_FovGrabMode; }
	//---------------------------------------------------------------------------------//
	//Fov有使用3D
	void                       SetFovUsing3D(bool value) { m_FovUsing3D = value; }
	bool                       GetFovUsing3D() const { return m_FovUsing3D; }
	//---------------------------------------------------------------------------------//	
	void                       SetFovTempInt(int value) { m_FovTempInt = value; }
	int                        GetFovTempInt() const { return m_FovTempInt; }
	//---------------------------------------------------------------------------------//
	void                       SetFovCadPos(const TPOINT2D &value) { m_FovCadPos = value; }
	TPOINT2D                   GetFovCadPos() const { return m_FovCadPos; }
	void                       SetFovCadPosX(double value) { m_FovCadPos.x = value; }
	double                     GetFovCadPosX() const { return m_FovCadPos.x; }
	void                       SetFovCadPosY(double value) { m_FovCadPos.y = value; }
	double                     GetFovCadPosY() const { return m_FovCadPos.y; }	
	//---------------------------------------------------------------------------------//
	//m_FovStagePos
	void                       SetFovStagePos(const TPOINT3D &value) { m_FovStagePos = value; }
	TPOINT3D                   GetFovStagePos() const { return m_FovStagePos; }
	void                       SetFovStagePosX(double value) { m_FovStagePos.x = value; }
	double                     GetFovStagePosX() const { return m_FovStagePos.x; }
	void                       SetFovStagePosY(double value) { m_FovStagePos.y = value; }
	double                     GetFovStagePosY() const { return m_FovStagePos.y; }	
	void                       SetFovStagePosZ(double value) { m_FovStagePos.z = value; }
	double                     GetFovStagePosZ() const { return m_FovStagePos.z; }	
	//---------------------------------------------------------------------------------//
	void                       MapFovCadToStagePos(const CMapCoordinate &Map);//將CAD轉成機台座標
	//---------------------------------------------------------------------------------//		
	size_t                     GetFovFrameParamCount() const;//取得Fov的影像參數列表
	TFrameParam*               GetFovFrameParamPtr(size_t index, bool check);//取得Fov的影像參數指標
	bool                       AddFovFrameParamPtr(const TFrameParam &Param);//加入Fov的影像參數	
	bool                       ClearFoveAllFrameParams();//清除Fov的影像參數列表	
	bool                       CloneFovFrameParamList(std::vector<TFrameParam> &ParamList);//複製Fov的影像參數列表
	bool                       SetFovFrameParamList(const std::vector<TFrameParam> &ParamList);//設定Fov的影像參數列表
	//---------------------------------------------------------------------------------//	
	size_t                     GetFovSliceParamCount() const;//取得Fov的單1影像參數列表
	TSliceParam*               GetFovSliceParamPtr(size_t index, bool check);//取得Fov的單1影像參數指標
	bool                       AddFovSliceParamPtr(const TSliceParam &Param);//加入Fov的單1影像參數	
	bool                       ClearFoveAllSliceParams();//清除Fov的單1影像參數列表	
	bool                       CloneFovSliceParamList(std::vector<TSliceParam> &ParamList);//複製Fov的單1影像參數列表
	bool                       SetFovSliceParamList(const std::vector<TSliceParam> &ParamList);//設定Fov的單1影像參數列表
	//---------------------------------------------------------------------------------//
	size_t                     GetFovFrameCount() const;//取得Fov的畫面數量
	CAOIFrame*                 GetFovFramePtr(size_t index, bool check);//取得Fov的畫面指標
	bool                       AddFovFramePtr(CAOIFrame *Ptr);//加入Fov的畫面指標
	bool                       RemoveFovAllFrames();//清除Fov的畫面指標列表	
	//---------------------------------------------------------------------------------//
	size_t                     GetFovFieldCount() const;//取得Fov的影像區域數量
	CAOIField*                 GetFovFieldPtr(size_t index, bool check);//取得Fov的影像區域指標
	bool                       AddFovFieldPtr(CAOIField *Ptr);//加入Fov的影像區域指標
	bool                       RemoveFovAllFields();//清除Fov的影像區域指標列表	//m_FovFieldPtrList
	//---------------------------------------------------------------------------------//	
};
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_AOIFOV_H__2DD461BD_7107_4297_9D0F_D4E940FF8D1F__INCLUDED_)
