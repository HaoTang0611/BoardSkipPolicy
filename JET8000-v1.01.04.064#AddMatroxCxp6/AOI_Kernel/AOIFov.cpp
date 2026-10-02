// AOIFov.cpp: implementation of the CAOIFov class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "AOIFov.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
CRITICAL_SECTION  CAOIFov::m_csFov;//同步機制-關鍵區間
//-------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
void CAOIFov::InitialFovLock()//初始化視野的關鍵區間
{
	::InitializeCriticalSection(&m_csFov);
}
//-------------------------------------------------------------------------------------//
void CAOIFov::DeleteFovLock() //刪除視野的關鍵區間
{
	::DeleteCriticalSection(&m_csFov);
}
//-------------------------------------------------------------------------------------//
void CAOIFov::LockFov()//進入視野的關鍵區間
{
	::EnterCriticalSection(&m_csFov);
}
//-------------------------------------------------------------------------------------//
void CAOIFov::UnlockFov()//離開視野的關鍵區間
{
	::LeaveCriticalSection(&m_csFov);
}
//-------------------------------------------------------------------------------------//
CAOIFov::CAOIFov():CAOIObj(AOI_OBJ_FOV)
{
	PreInitFov();
	InitialFov();
}
//-------------------------------------------------------------------------------------//
CAOIFov::CAOIFov(const CAOIFov &fov):CAOIObj(fov)
{
	PreInitFov();
	CloneFov(fov);
}
//-------------------------------------------------------------------------------------//
CAOIFov::~CAOIFov()
{
}
//-------------------------------------------------------------------------------------//
CAOIFov& CAOIFov::operator=(const CAOIFov &fov)
{
	if ( this == &fov ) { return *this; }
	CAOIObj::operator=(fov);
	CloneFov(fov);
	return *this;
}
//-------------------------------------------------------------------------------------//
void CAOIFov::PreInitFov()
{	
}
//-------------------------------------------------------------------------------------//
void CAOIFov::InitialFov()
{
	m_FovGrabMode = 0;//FOV取像模式
	m_FovCadPos = TPOINT2D();//Fov在Cad的位置
	m_FovStagePos = TPOINT3D();//Fov在Stage的位置
	m_FovUsing3D = true;
	m_FovTempInt = 0;
	m_FovFrameParamList.clear();
	m_FovSliceParamList.clear();
}
//-------------------------------------------------------------------------------------//
void CAOIFov::CloneFov(const CAOIFov &fov)
{
	//m_FovIndex = fov.m_FovIndex;//Fov的引數編號		

	m_FovGrabMode = fov.m_FovGrabMode;//FOV取像模式
	m_FovCadPos = fov.m_FovCadPos;//Fov在Cad的位置	
	m_FovStagePos = fov.m_FovStagePos;//Fov在Stage的位置
	m_FovUsing3D = fov.m_FovUsing3D;
	//---------------------------------------------------------------------------------//
	m_FovTempInt = fov.m_FovTempInt;
	//---------------------------------------------------------------------------------//
	m_FovFrameParamList = fov.m_FovFrameParamList;//影像參數列表
	m_FovSliceParamList = fov.m_FovSliceParamList;//單1影像參數列表
	m_FovFieldPtrList = fov.m_FovFieldPtrList;//Fov的相機影像區域指標列表
	//---------------------------------------------------------------------------------//	
}
//-------------------------------------------------------------------------------------//
CAOIFov* CAOIFov::CloneFovObj() const//建立且複製一個視野
{
	CAOIFov *ObjPtr = AOIObjManager.CreateFovObj();
	if ( NULL == ObjPtr ) { return NULL; }
	ObjPtr->operator=(*this);
	return ObjPtr;
}
//-------------------------------------------------------------------------------------//
void CAOIFov::MapFovCadToStagePos(const CMapCoordinate &Map)//將CAD轉成機台座標
{	
	Map.Map2D(m_FovCadPos.x, m_FovCadPos.y, m_FovStagePos.x, m_FovStagePos.y);	
}
//-------------------------------------------------------------------------------------//
size_t CAOIFov::GetFovFrameParamCount() const//取得Fov的影像參數列表
{
	return m_FovFrameParamList.size();
}
//-------------------------------------------------------------------------------------//
TFrameParam* CAOIFov::GetFovFrameParamPtr(size_t index, bool check)//取得Fov的影像參數指標
{
	if ( true == check )
	{
		const size_t count = m_FovFrameParamList.size();
		if ( index >= count ) 
		{	return NULL; }
	}
	return &(m_FovFrameParamList[index]);
}
//-------------------------------------------------------------------------------------//
bool CAOIFov::AddFovFrameParamPtr(const TFrameParam &Param)//加入Fov的影像參數	
{
	m_FovFrameParamList.push_back(Param);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFov::ClearFoveAllFrameParams()//清除Fov的影像參數列表	
{
	m_FovFrameParamList.clear();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFov::CloneFovFrameParamList(std::vector<TFrameParam> &ParamList)//複製Fov的影像參數列表
{
	ParamList = m_FovFrameParamList;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFov::SetFovFrameParamList(const std::vector<TFrameParam> &ParamList)//設定Fov的影像參數列表
{
	m_FovFrameParamList = ParamList;
	return true;
}
//-------------------------------------------------------------------------------------//
size_t CAOIFov::GetFovSliceParamCount() const//取得Fov的單1影像參數列表
{
	return m_FovSliceParamList.size();
}
//-------------------------------------------------------------------------------------//
TSliceParam* CAOIFov::GetFovSliceParamPtr(size_t index, bool check)//取得Fov的單1影像參數指標
{
	if ( true == check ) 
	{
		const size_t Count = m_FovSliceParamList.size();
		if ( index >= Count ) 
		{	return NULL; }
	}
	return &(m_FovSliceParamList[index]);
}
//-------------------------------------------------------------------------------------//
bool CAOIFov::AddFovSliceParamPtr(const TSliceParam &Param)//加入Fov的單1影像參數
{
	m_FovSliceParamList.push_back(Param);
	return true; 
}
//-------------------------------------------------------------------------------------//
bool CAOIFov::ClearFoveAllSliceParams()//清除Fov的單1影像參數列表
{
	m_FovSliceParamList.clear();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFov::CloneFovSliceParamList(std::vector<TSliceParam> &ParamList)//複製Fov的單1影像參數列表
{
	ParamList = m_FovSliceParamList;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFov::SetFovSliceParamList(const std::vector<TSliceParam> &ParamList)//設定Fov的單1影像參數列表
{
	m_FovSliceParamList = ParamList;
	return true;
}
//-------------------------------------------------------------------------------------//