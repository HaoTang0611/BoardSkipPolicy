// AOIObj.h: interface for the CAOIObj class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AOIOBJ_H__540F6D96_6DA9_4DD9_A9D0_463F8DF0827B__INCLUDED_)
#define AFX_AOIOBJ_H__540F6D96_6DA9_4DD9_A9D0_463F8DF0827B__INCLUDED_
//-------------------------------------------------------------------------------------//
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
enum AOI_OBJ_TYPE
{
	AOI_OBJ_NULL=0,
	AOI_OBJ_BASIC,
	AOI_OBJ_RGN,
	AOI_OBJ_COMPONENT,
	AOI_OBJ_BOARD,
	AOI_OBJ_PANEL,
	AOI_OBJ_FD,	
	AOI_OBJ_FOV,
	AOI_OBJ_BOX,
	AOI_OBJ_WND,
	AOI_OBJ_LAND,
	AOI_OBJ_MARK,
	AOI_OBJ_SLICE,	
	AOI_OBJ_LOGIC,
	AOI_OBJ_MODEL,
	AOI_OBJ_FIELD,
	AOI_OBJ_FRAME,	
	AOI_OBJ_WINDOW,	
	AOI_OBJ_WND_ROI,	
	AOI_OBJ_BARCODE,	
	AOI_OBJ_PROJECT,
	AOI_OBJ_WND_MASK,
	AOI_OBJ_PART_GROUP,
	AOI_OBJ_TOTAL
};
//-------------------------------------------------------------------------------------//
class CAOIObjManager;
//-------------------------------------------------------------------------------------//
class CAOIObj : public CObject  
{
	friend CAOIObjManager;
	//---------------------------------------------------------------------------------//	
	DECLARE_DYNAMIC(CAOIObj)
	//---------------------------------------------------------------------------------//
	static CString             ObtainAOITypeText(AOI_OBJ_TYPE value);
	//---------------------------------------------------------------------------------//
private:
	//---------------------------------------------------------------------------------//
	UUID                       m_ObjUuid;//AOI Object的唯一碼
	bool                       m_ObjUsed;//AOI Obj使用中
	bool                       m_ObjRecycleMode;//循環使用模式
	size_t                     m_ObjRecycleIndex;//循環使用容器引數	
	size_t                     m_ObjRecycleIndex_MP;//循環使用容器引數-MP
	size_t                     m_ObjGlobalPtrIndex;//在自定義指標容器內的引數, 唯一且不能複製	
	AOI_OBJ_TYPE               m_ObjType;//AOI Object的樣式	
	//---------------------------------------------------------------------------------//
	bool                       GetObjUsed() const { return m_ObjUsed; }
	void                       SetObjUsed(bool val) { m_ObjUsed=val; }	
	//---------------------------------------------------------------------------------//
	bool                       GetObjRecycleMode() const { return m_ObjRecycleMode; }
	void                       SetObjRecycleMode(bool val) { m_ObjRecycleMode=val; }
	bool                       CheckObjRecycleMode() const { return m_ObjRecycleMode; }
	//---------------------------------------------------------------------------------//	
	bool                       CheckObjRecycleIndex() const { return -1==m_ObjRecycleIndex ? false:true;}	
	//---------------------------------------------------------------------------------//
	bool                       CheckObjRecycleIndex_MP() const { return -1==m_ObjRecycleIndex_MP ? false:true;}	
	//---------------------------------------------------------------------------------//	
	void                       SetObjGlobalPtrIndex(size_t value) { CAOIObj::m_ObjGlobalPtrIndex=value;}
	size_t                     GetObjGlobalPtrIndex() const { return CAOIObj::m_ObjGlobalPtrIndex; }
	//---------------------------------------------------------------------------------//
	size_t                     GetObjRecycleIndex(bool bMP) const { return false==bMP ? m_ObjRecycleIndex:m_ObjRecycleIndex_MP; }
	void                       SetObjRecycleIndex(size_t val, bool bMP) { false==bMP ? m_ObjRecycleIndex=val:m_ObjRecycleIndex_MP=val; }	
	bool                       CheckObjRecycleIndex(bool bMP) const { return false==bMP ? CheckObjRecycleIndex():CheckObjRecycleIndex_MP();}	
	void                       AddInObjRecycleList(std::vector<CAOIObj*> &List, bool bMP)	 { SetObjRecycleIndex(List.size(), bMP); List.push_back(this); }
	//---------------------------------------------------------------------------------//		
protected:
	//---------------------------------------------------------------------------------//				
	CAOIObj(AOI_OBJ_TYPE type);
	//---------------------------------------------------------------------------------//
	void                       PreInitObj();//預先初始化
	//---------------------------------------------------------------------------------//				
	void                       SetObjType(AOI_OBJ_TYPE value) { CAOIObj::m_ObjType=value;}	
	//---------------------------------------------------------------------------------//
	void                       CloneObj(const CAOIObj &Obj);
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	CAOIObj();
	CAOIObj(const CAOIObj &obj);
	virtual ~CAOIObj();
	CAOIObj& operator=(const CAOIObj &obj);
	//---------------------------------------------------------------------------------//
	void                       SetObjUuid(const UUID &uuid) { CAOIObj::m_ObjUuid = uuid; }
	UUID                       GetObjUuid() const { return CAOIObj::m_ObjUuid; }
	const UUID*                GetObjUuidPtr() const { return &(CAOIObj::m_ObjUuid); }
	AOI_OBJ_TYPE               GetObjType() const { return CAOIObj::m_ObjType; }
	//---------------------------------------------------------------------------------//
};
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_AOIOBJ_H__540F6D96_6DA9_4DD9_A9D0_463F8DF0827B__INCLUDED_)
