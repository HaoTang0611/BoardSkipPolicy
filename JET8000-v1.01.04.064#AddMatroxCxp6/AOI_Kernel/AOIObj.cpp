// AOIObj.cpp: implementation of the CAOIObj class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "AOIObj.h"
//-------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
IMPLEMENT_DYNAMIC(CAOIObj, CObject)
//-------------------------------------------------------------------------------------//
CString CAOIObj::ObtainAOITypeText(AOI_OBJ_TYPE value)
{
	CString text;
	switch ( value )
	{
	case AOI_OBJ_BASIC: text=_T("AOI Obj"); break;
	case AOI_OBJ_RGN: text=_T("Rgn Obj"); break;
	case AOI_OBJ_COMPONENT: text=_T("Component Obj"); break;
	case AOI_OBJ_BOARD: text=_T("Board Obj"); break;
	case AOI_OBJ_PANEL: text=_T("Panel Obj"); break;
	case AOI_OBJ_MODEL: text=_T("Model Obj"); break;
	case AOI_OBJ_LAND: text=_T("Land Obj"); break;
	case AOI_OBJ_WND: text=_T("Wnd Obj"); break;
	case AOI_OBJ_FD: text=_T("Fd Obj"); break;	
	case AOI_OBJ_FOV: text=_T("Fov Obj"); break;	
	case AOI_OBJ_MARK: text=_T("Mark Obj"); break;
	case AOI_OBJ_SLICE: text=_T("Slice Obj"); break;	
	case AOI_OBJ_FIELD: text=_T("Field Obj"); break;
	case AOI_OBJ_FRAME: text=_T("Frame Obj"); break;	
	case AOI_OBJ_WND_ROI: text=_T("Wnd Roi Obj"); break;
	case AOI_OBJ_BARCODE: text=_T("Barcode Obj"); break;
	case AOI_OBJ_PROJECT: text=_T("Project Obj"); break;		
	case AOI_OBJ_WND_MASK: text=_T("Wnd Mask Obj"); break;		
	default: text=_T("No-Defined"); break;
	}
	return text;
}
//-------------------------------------------------------------------------------------//
CAOIObj::CAOIObj():m_ObjGlobalPtrIndex(-1),m_ObjType(AOI_OBJ_BASIC)
{
	PreInitObj();	
}
//-------------------------------------------------------------------------------------//
CAOIObj::CAOIObj(AOI_OBJ_TYPE type):m_ObjGlobalPtrIndex(-1),m_ObjType(type)
{
	PreInitObj();
}
//-------------------------------------------------------------------------------------//
CAOIObj::CAOIObj(const CAOIObj &obj):m_ObjGlobalPtrIndex(-1),m_ObjType(obj.m_ObjType)
{	
	PreInitObj();
}
//-------------------------------------------------------------------------------------//
CAOIObj::~CAOIObj()
{
}
//-------------------------------------------------------------------------------------//
CAOIObj& CAOIObj::operator=(const CAOIObj &obj)
{
	if ( this == &obj ) { return *this; }	
	CAOIObj::CloneObj(obj);
	return *this;
}
//-------------------------------------------------------------------------------------//
inline void CAOIObj::PreInitObj()//預先初始化
{
	m_ObjUsed = false;	
	m_ObjRecycleMode = false;
	m_ObjRecycleIndex = -1;	
	m_ObjRecycleIndex_MP = -1;
	::UuidCreate(&m_ObjUuid);
}
//-------------------------------------------------------------------------------------//
inline void CAOIObj::CloneObj(const CAOIObj &obj)
{
	//CAOIObj::m_ObjUuid = obj.m_ObjUuid;
	//CAOIObj::m_ObjUsed = obj.m_ObjUsed;	
	//CAOIObj::m_ObjRecycleMode = obj.m_ObjRecycleMode;
	//CAOIObj::m_ObjRecycleIndex = obj.m_ObjRecycleIndex;
	//CAOIObj::m_ObjRecycleIndex_MP = obj.m_ObjRecycleIndex_MP;	
	CAOIObj::m_ObjType = obj.m_ObjType;	
	//CAOIObj::m_ObjGlobalPtrIndex = obj.m_ObjGlobalPtrIndex;	
}
//-------------------------------------------------------------------------------------//