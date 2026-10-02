// AOIField.cpp: implementation of the CAOIField class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "AOIField.h"
//-------------------------------------------------------------------------------------//
#include "AOIFileIO.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
CRITICAL_SECTION  CAOIField::m_csField;//同步機制-關鍵區間
//-------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
IMPLEMENT_DYNAMIC(CAOIField, CAOIObj)
//-------------------------------------------------------------------------------------//
void CAOIField::InitialFieldLock()//初始化相機內區域的關鍵區間
{
	::InitializeCriticalSection(&m_csField);
}
//-------------------------------------------------------------------------------------//
void CAOIField::DeleteFieldLock() //刪除相機內區域的關鍵區間
{
	::DeleteCriticalSection(&m_csField);
}
//-------------------------------------------------------------------------------------//
void CAOIField::LockField()//進入相機內區域的關鍵區間
{
	::EnterCriticalSection(&m_csField);
}
//-------------------------------------------------------------------------------------//
void CAOIField::UnlockField()//離開相機內區域的關鍵區間
{
	::LeaveCriticalSection(&m_csField);
}
//-------------------------------------------------------------------------------------//
CAOIField::CAOIField():CAOIObj(AOI_OBJ_FIELD)
{
	PreInitField();
	InitialField();
}
//-------------------------------------------------------------------------------------//
CAOIField::CAOIField(const CAOIField &field):CAOIObj(field)
{
	PreInitField();
	CloneField(field);
}
//-------------------------------------------------------------------------------------//
CAOIField::~CAOIField()
{
	
}
//-------------------------------------------------------------------------------------//
CAOIField& CAOIField::operator=(const CAOIField &field)
{
	if ( this == &field ) { return *this; }
	CAOIObj::operator=(field);
	CAOIField::CloneField(field);
	return *this;
}
//-------------------------------------------------------------------------------------//
inline void CAOIField::PreInitField()
{	
}
//-------------------------------------------------------------------------------------//
inline void CAOIField::InitialField()
{	
	m_FieldIndex = -1;//相機內區域的引數編號		
	m_FieldHeightID = 0;//區域的高度編號
	m_FieldListMode = FIELD_LIST_NONE;//區域所屬的列表模式
	m_FieldProjectPtr = NULL;//區域的專案指標
	m_FieldPanelIdx = -1;//區域的整板引數
	m_FieldPanelPtr = NULL;//區域的整板指標	
	m_FieldBoardIdx = -1;//區域的單板引數
	m_FieldBoardPtr = NULL;//區域的單板指標
	m_FieldComponentIdx = -1;//區域的零件引數
	m_FieldComponentPtr = NULL;//區域的零件指標
	m_FieldSelected = false;//區域的選取狀態
	m_FieldLinkPointer = false;//連結指標, 如果是的話不要刪除
	m_FieldMustToLoad = false;//區域的必定要載入圖檔
	m_FieldLockRelease = false;//區域的鎖住釋放
	m_FieldIsInSaveList = false;//區域已加入儲存列表
	m_FieldPartImageMode = false;//區域的零件影像模式
	m_FieldPanelBasePlane = 0;//區域的整板基準面
	m_FieldDistrictID = DISTRICT_ID_A;//區域的多段編號
	m_FieldTempInt = 0;//區域暫時用變數
	m_FieldGrabIndex = 0;//區域的取像次序
	m_FieldGrabIndexByUser = -1;//區域的取像次序
	m_FieldCadPos = TPOINT2D();//相機內區域在Cad的位置
	m_FieldStagePos = TPOINT3D();//相機內區域在Stage的位置
	m_FieldSize_Inner.cx = 1024;//區域的空間內部尺寸寬度um
	m_FieldSize_Inner.cy = 1024;//區域的空間內部尺寸長度um
	m_FieldSize_Outer.cx = 1024;//區域的空間外部尺寸寬度um
	m_FieldSize_Outer.cy = 1024;//區域的空間外部尺寸長度um
	m_FieldSize_Real.cx = 1024;
	m_FieldSize_Real.cy = 1024;
	m_FieldMergeTime = 0.0;
	m_FieldCalcState = FIELD_CALC_NONE;
	m_FieldMergeState = FIELD_MERGE_NONE;
	m_FieldFramePtrList.clear();
}
//-------------------------------------------------------------------------------------//
inline void CAOIField::CloneField(const CAOIField &field)
{	
	m_FieldIndex = field.m_FieldIndex;//相機內區域的引數編號
	m_FieldHeightID = field.m_FieldHeightID;//區域的高度編號	
	m_FieldListMode = field.m_FieldListMode;//區域所屬的列表模式
	m_FieldProjectPtr = field.m_FieldProjectPtr;//區域的專案指標
	m_FieldPanelIdx = field.m_FieldPanelIdx;//區域的整板引數
	m_FieldPanelPtr = field.m_FieldPanelPtr;//區域的整板指標
	m_FieldBoardIdx = field.m_FieldBoardIdx;//區域的單板引數
	m_FieldBoardPtr = field.m_FieldBoardPtr;//區域的單板指標
	m_FieldComponentIdx = field.m_FieldComponentIdx;//區域的零件引數
	m_FieldComponentPtr = field.m_FieldComponentPtr;//區域的零件指標	
	m_FieldSelected = field.m_FieldSelected;//區域的選取狀態
	m_FieldLinkPointer = field.m_FieldLinkPointer;//連結指標, 如果是的話不要刪除	
	m_FieldMustToLoad  = field.m_FieldMustToLoad;//區域的必定要載入圖檔
	m_FieldLockRelease = field.m_FieldLockRelease;//區域的鎖住釋放
	m_FieldIsInSaveList = field.m_FieldIsInSaveList;//區域已加入儲存列表
	m_FieldPartImageMode = field.m_FieldPartImageMode;//區域的零件影像模式	
	m_FieldPanelBasePlane = field.m_FieldPanelBasePlane;//區域的整板基準面	
	m_FieldDistrictID  = field.m_FieldDistrictID;//區域的多段編號
	m_FieldTempInt = field.m_FieldTempInt;//區域暫時用變數
	m_FieldGrabIndex = field.m_FieldGrabIndex;//區域的取像次序
	m_FieldGrabIndexByUser = field.m_FieldGrabIndexByUser;//區域的取像次序	
	m_FieldCadPos = field.m_FieldCadPos;//相機內區域在Cad的位置	
	m_FieldStagePos = field.m_FieldStagePos;//相機內區域在Stage的位置	
	m_FieldSize_Inner = field.m_FieldSize_Inner;//區域的空間內部尺寸um	
	m_FieldSize_Outer = field.m_FieldSize_Outer;//區域的空間外部尺寸um	
	m_FieldSize_Real = field.m_FieldSize_Real;//區域的空間全部尺寸um	
	m_FieldFramePtrList = field.m_FieldFramePtrList;//區域的空間外部尺寸長度um	
	//ClearFieldAllFrames();
	m_FieldMergeTime = field.m_FieldMergeTime;
	m_FieldCalcState = field.m_FieldCalcState;
	m_FieldMergeState = field.m_FieldMergeState;	
}
//-------------------------------------------------------------------------------------//
CAOIField* CAOIField::CloneFieldObj() const//建立且複製一個相機內區域
{
	CAOIField *ObjPtr = AOIObjManager.CreateFieldObj();
	if ( NULL == ObjPtr ) { return NULL; }
	ObjPtr->operator=(*this);
	return ObjPtr;
}
//-------------------------------------------------------------------------------------//
inline void CAOIField::AddFieldFramePtr_Inline(CAOIFrame *Ptr)//增加區域的影像指標
{
	m_FieldFramePtrList.push_back(Ptr);
}
//-------------------------------------------------------------------------------------//
inline size_t CAOIField::GetFieldFramePtrCount_Inline() const//取得區域的影像指標數量
{
	return m_FieldFramePtrList.size();
}
//-------------------------------------------------------------------------------------//
inline CAOIFrame* CAOIField::GetFieldFramePtr_Inline(size_t index)//取得區域的影像指標	
{
	return m_FieldFramePtrList[index];
}
//-------------------------------------------------------------------------------------//
inline void CAOIField::RemoveFieldAllFrames_Inline()//移除區域的所有影像	
{
	m_FieldFramePtrList.clear();
}
//-------------------------------------------------------------------------------------//
inline int CAOIField::GetFieldOpenMPCount() const//取回區域的OpenMP數量
{
	return 2;
}
//-------------------------------------------------------------------------------------//
void CAOIField::SetFieldSize_Inner(double w, double h)
{
	m_FieldSize_Inner.cx = w;
	m_FieldSize_Inner.cy = h;
}
//-------------------------------------------------------------------------------------//
void CAOIField::SetFieldSize_Outer(double w, double h)
{
	m_FieldSize_Outer.cx = w;
	m_FieldSize_Outer.cy = h;
}
//-------------------------------------------------------------------------------------//
void CAOIField::SetFieldSize_Real(double w, double h)
{
	m_FieldSize_Real.cx = w;
	m_FieldSize_Real.cy = h;
}
//-------------------------------------------------------------------------------------//
void CAOIField::GetFieldCadRgn_Inner(TREGION4D &Rgn) const
{
	Rgn.minX = m_FieldCadPos.x-(m_FieldSize_Inner.cx*0.5);
	Rgn.maxX = Rgn.minX + m_FieldSize_Inner.cx;
	Rgn.minY = m_FieldCadPos.y-(m_FieldSize_Inner.cy*0.5);
	Rgn.maxY = Rgn.minY + m_FieldSize_Inner.cy;
}
//-------------------------------------------------------------------------------------//
void CAOIField::GetFieldCadRgn_Outer(TREGION4D &Rgn) const
{
	Rgn.minX = m_FieldCadPos.x-(m_FieldSize_Outer.cx*0.5);
	Rgn.maxX = Rgn.minX + m_FieldSize_Outer.cx;
	Rgn.minY = m_FieldCadPos.y-(m_FieldSize_Outer.cy*0.5);
	Rgn.maxY = Rgn.minY + m_FieldSize_Outer.cy;
}
//-------------------------------------------------------------------------------------//
void CAOIField::GetFieldCadRgn_Real(TREGION4D &Rgn) const
{
	Rgn.minX = m_FieldCadPos.x-(m_FieldSize_Real.cx*0.5);
	Rgn.maxX = Rgn.minX + m_FieldSize_Real.cx;
	Rgn.minY = m_FieldCadPos.y-(m_FieldSize_Real.cy*0.5);
	Rgn.maxY = Rgn.minY + m_FieldSize_Real.cy;
}
//-------------------------------------------------------------------------------------//
void CAOIField::GetFieldStageRgn_Inner(TREGION4D &Rgn) const
{
	Rgn.minX = m_FieldStagePos.x-(m_FieldSize_Inner.cx*0.5);
	Rgn.maxX = Rgn.minX + m_FieldSize_Inner.cx;
	Rgn.minY = m_FieldStagePos.y-(m_FieldSize_Inner.cy*0.5);
	Rgn.maxY = Rgn.minY + m_FieldSize_Inner.cy;
}
//-------------------------------------------------------------------------------------//
void CAOIField::GetFieldStageRgn_Outer(TREGION4D &Rgn) const
{
	Rgn.minX = m_FieldStagePos.x-(m_FieldSize_Outer.cx*0.5);
	Rgn.maxX = Rgn.minX + m_FieldSize_Outer.cx;
	Rgn.minY = m_FieldStagePos.y-(m_FieldSize_Outer.cy*0.5);
	Rgn.maxY = Rgn.minY + m_FieldSize_Outer.cy;
}
//-------------------------------------------------------------------------------------//
void CAOIField::GetFieldStageRgn_Real(TREGION4D &Rgn) const
{
	Rgn.minX = m_FieldStagePos.x-(m_FieldSize_Real.cx*0.5);
	Rgn.maxX = Rgn.minX + m_FieldSize_Real.cx;
	Rgn.minY = m_FieldStagePos.y-(m_FieldSize_Real.cy*0.5);
	Rgn.maxY = Rgn.minY + m_FieldSize_Real.cy;
}
//-------------------------------------------------------------------------------------//
bool CAOIField::CalcFieldImageSize(const TPOINT2D &Res, IMAGE_SIZE &W, IMAGE_SIZE &H) const//計算視野影像大小
{
	W = (IMAGE_SIZE)((GetFieldSizeW_Real()/Res.x)+0.5);
	H = (IMAGE_SIZE)((GetFieldSizeH_Real()/Res.y)+0.5);
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIField::MapFieldCadToStagePos(const CMapCoordinate &Map)//將CAD轉成機台座標
{	
	Map.Map2D(m_FieldCadPos.x, m_FieldCadPos.y, m_FieldStagePos.x, m_FieldStagePos.y);	
}
//-------------------------------------------------------------------------------------//
void CAOIField::MapFieldStageToCadPos(const CMapCoordinate &Map)//將機台轉成CAD座標
{
	Map.Map2D(m_FieldStagePos.x, m_FieldStagePos.y, m_FieldCadPos.x, m_FieldCadPos.y);	
}
//-------------------------------------------------------------------------------------//
void CAOIField::MoveFieldPos(double dX, double dY, CMapCoordinate *MapPtr)//移動區域座標	
{
	m_FieldCadPos.x += dX;
	m_FieldCadPos.y += dY;	
	if ( NULL != MapPtr )
	{	MapFieldCadToStagePos(*MapPtr);	}	
}
//-------------------------------------------------------------------------------------//
bool CAOIField::AddFieldRgnPtr(CAOIRgn *Ptr)//增加區域的檢測區域指標
{	
	if ( NULL == Ptr ) { return true; }
	Ptr->SetRgnFieldPtr(this);	
	Ptr->SetRgnFieldIndex(GetFieldIndex());
	m_FieldRgnPtrList.push_back(Ptr);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIField::RemoveFieldRgnPtr(CAOIRgn *Ptr)//移除區域的檢測區域指標
{
	if ( NULL == Ptr ) { return true; }
	size_t                  i=0;
	CAOIRgn                *RgnPtr=NULL;
	std::vector<CAOIRgn*>   TmpFieldRgnPtrList=m_FieldRgnPtrList;//區域內的檢測指標列表	
	const size_t RgnCount = TmpFieldRgnPtrList.size();
	m_FieldRgnPtrList.clear();
	for ( i=0; i<RgnCount; i++ )
	{
		RgnPtr = TmpFieldRgnPtrList[i];
		if ( RgnPtr == Ptr ) 
		{
			Ptr->SetRgnFieldPtr(NULL);	
			Ptr->SetRgnFieldIndex(-1);
			continue;
		}
		m_FieldRgnPtrList.push_back(RgnPtr);
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
size_t CAOIField::GetFieldRgnPtrCount() const//取得區域的檢測區域指標數量
{
	return m_FieldRgnPtrList.size();
}	
//-------------------------------------------------------------------------------------//
CAOIRgn* CAOIField::GetFieldRgnPtr(size_t index, bool check)//取得區域的檢測區域指標	
{
	if ( true == check ) 
	{
		const size_t count = this->m_FieldRgnPtrList.size();
		if ( index >= count ) 
		{	return NULL; }
	}
	return m_FieldRgnPtrList[index];
}
//-------------------------------------------------------------------------------------//
void CAOIField::RemoveFieldAllRgns()//移除區域的所有檢測區域
{
	this->m_FieldRgnPtrList.clear();
}
//-------------------------------------------------------------------------------------//
void CAOIField::LayoutFieldRgnPtrList()//排列區域內的檢測區域指標
{
	size_t   i=0;
	AOI_OBJ_TYPE ObjType;
	CAOIRgn *RgnPtr1 = NULL;
	CAOIRgn *RgnPtr2 = NULL;	
	std::vector<CAOIRgn*> TmpFieldRgnPtrList=m_FieldRgnPtrList;//區域內的檢測指標列表
	
	m_FieldRgnPtrList.clear();
	const size_t RgnPtrCount = TmpFieldRgnPtrList.size();

	//Reset List
	for ( i=0; i<RgnPtrCount; i++ )
	{
		RgnPtr1 = TmpFieldRgnPtrList[i];
		if ( NULL == RgnPtr1 ) { continue; }
		RgnPtr1->SetRgnTempInt(FN_DISABLE);
	}

	// Add Fd First
	for ( i=0; i<RgnPtrCount; i++ )
	{
		RgnPtr1 = TmpFieldRgnPtrList[i];
		if ( NULL == RgnPtr1 ) { continue; }
		if ( RgnPtr1->GetRgnTempInt() == FN_ENABLE ) { continue; }
		RgnPtr2 = RgnPtr1->GetRgnParent();
		if ( NULL != RgnPtr2 ) 
		{	ObjType = RgnPtr2->GetObjType();	}
		else
		{	ObjType = RgnPtr1->GetObjType();	}
		if ( AOI_OBJ_FD != ObjType ) { continue; }
		RgnPtr1->SetRgnTempInt(FN_ENABLE);
		AddFieldRgnPtr(RgnPtr1);		
	}

	// Add SB First
	for ( i=0; i<RgnPtrCount; i++ )
	{
		RgnPtr1 = TmpFieldRgnPtrList[i];
		if ( NULL == RgnPtr1 ) { continue; }
		if ( RgnPtr1->GetRgnTempInt() == FN_ENABLE ) { continue; }

		RgnPtr2 = RgnPtr1->GetRgnParent();
		if ( NULL != RgnPtr2 ) 
		{	ObjType = RgnPtr2->GetObjType();	}
		else
		{	ObjType = RgnPtr1->GetObjType();	}
		if ( AOI_OBJ_BARCODE != ObjType ) { continue; }
		RgnPtr1->SetRgnTempInt(FN_ENABLE);
		AddFieldRgnPtr(RgnPtr1);		
	}

	//Add Others
	for ( i=0; i<RgnPtrCount; i++ )
	{
		RgnPtr1 = TmpFieldRgnPtrList[i];
		if ( NULL == RgnPtr1 ) { continue; }
		if ( RgnPtr1->GetRgnTempInt() == FN_ENABLE ) { continue; }
		RgnPtr1->SetRgnTempInt(FN_ENABLE);
		AddFieldRgnPtr(RgnPtr1);		
	}
	return;
}
//-------------------------------------------------------------------------------------//
bool CAOIField::CheckFieldUsing3D()//確認區域沒有使用3D影像
{	
	size_t i=0;
	CAOIRgn *RgnPtr = NULL;
	const size_t RgnCount=GetFieldRgnPtrCount();
	for ( i=0; i<RgnCount; i++ )
	{
		RgnPtr=GetFieldRgnPtr(i, false);
		if ( NULL == RgnPtr ) { continue; }		
		if ( RgnPtr->GetRgnBypassed() == true ) { continue; }
		if ( RgnPtr->GetRgnBypass3D() == true ) { continue; }
		
		if ( RgnPtr->GetRgnUsing3D() == true )
		{	return true; }
	}
	return false;
}
//-------------------------------------------------------------------------------------//
bool CAOIField::AddFieldBoardPtr(CAOIBoard *Ptr)//增加區域的單板指標
{
	if ( NULL == Ptr ) { return true; }
	DISTRICT_ID DistrictID = GetFieldDistrictID();
	Ptr->SetBoardPanelFieldPtr(this, DistrictID);		
	m_FieldBoardPtrList.push_back(Ptr);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIField::RemoveFieldBoardPtr(CAOIBoard *Ptr)//移除區域的單板指標
{
	if ( NULL == Ptr ) { return true; }
	size_t                  i=0;
	CAOIBoard              *BoardPtr=NULL;
	DISTRICT_ID DistrictID = GetFieldDistrictID();
	std::vector<CAOIBoard*> TmpFieldBoardPtrList=m_FieldBoardPtrList;
	const size_t BoardCount = TmpFieldBoardPtrList.size();
	m_FieldBoardPtrList.clear();
	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr = TmpFieldBoardPtrList[i];
		if ( BoardPtr == Ptr ) 
		{
			Ptr->ResetBoardPanelFieldParam(DistrictID);
			continue;
		}
		m_FieldBoardPtrList.push_back(BoardPtr);
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
size_t CAOIField::GetFieldBoardPtrCount() const//取得區域的單板指標數量
{
	return m_FieldBoardPtrList.size();
}
//-------------------------------------------------------------------------------------//
CAOIBoard* CAOIField::GetFieldBoardPtr(size_t index, bool check)//取得區域的單板指標	
{
	if ( true == check ) 
	{
		const size_t count = this->m_FieldBoardPtrList.size();
		if ( index >= count ) 
		{	return NULL; }
	}
	return m_FieldBoardPtrList[index];
}
//-------------------------------------------------------------------------------------//
void CAOIField::RemoveFieldAllBoards()//移除區域的所有單板
{
	this->m_FieldBoardPtrList.clear();
}
//-------------------------------------------------------------------------------------//
bool CAOIField::AddFieldFramePtr(CAOIFrame *Ptr)//增加區域的影像指標
{
	if ( NULL == Ptr ) { return false; }
	const unsigned int FieldIndex = (unsigned int)(CAOIField::GetFieldIndex());	
	Ptr->SetFrameFieldPtr(this);
	Ptr->SetFrameFieldIndex(FieldIndex);
	Ptr->SetFrameDistrictID(GetFieldDistrictID());
	CAOIField::AddFieldFramePtr_Inline(Ptr);	
	return true;
}
//-------------------------------------------------------------------------------------//
size_t CAOIField::GetFieldFramePtrCount() const//取得區域的影像指標數量
{
	return CAOIField::GetFieldFramePtrCount_Inline();
}
//-------------------------------------------------------------------------------------//
CAOIFrame* CAOIField::GetFieldFramePtr(size_t index, bool check)//取得區域的影像指標
{
	if ( true == check ) 
	{
		const size_t count = this->m_FieldFramePtrList.size();
		if ( index >= count ) 
		{	return NULL; }
	}
	return CAOIField::GetFieldFramePtr_Inline(index);
}
//-------------------------------------------------------------------------------------//
void CAOIField::ClearFieldAllFrames()//清除區域的所有影像
{
	size_t       i=0;	
	CAOIFrame   *FramePtr = NULL;
	const size_t FrameCount = GetFieldFramePtrCount_Inline();
	for ( i=0; i<FrameCount; i++ )
	{
		FramePtr = GetFieldFramePtr_Inline(i);
		if ( NULL == FramePtr ) { continue; }
		AOIObjManager.DestroyFrameObj(FramePtr);
	}
	RemoveFieldAllFrames_Inline();
}
//-------------------------------------------------------------------------------------//
void CAOIField::RemoveFieldAllFrames()//移除區域的所有影像
{	
	RemoveFieldAllFrames_Inline();	
}
//-------------------------------------------------------------------------------------//
bool CAOIField::SetFieldFrameCalcState(FRAME_CALC_STATE val)//設定區域影像計算狀態
{		
	const size_t FrameCount=GetFieldFramePtrCount();
	CAOIFrame::LockFrame();
	for ( size_t i=0; i<FrameCount; i++ )
	{
		CAOIFrame *FramePtr = GetFieldFramePtr(i, false);
		if ( NULL == FramePtr ) { continue; }				
		FramePtr->SetFrameCalcState(val);
	}
	CAOIFrame::UnlockFrame();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIField::CheckFieldRgnFinish()//確認區域內所有區域都已經計算完畢
{
	size_t             i = 0;
	CAOIRgn           *RgnPtr = NULL;
	CAOIRgn           *RgnPtr_Parent = NULL;
	REGION_CALC_STATE  RgnCalcState;
	REGION_CALC_STATE  RgnCalcState2;
	const size_t FieldRgnCount = GetFieldRgnPtrCount();			
	for ( i=0; i<FieldRgnCount; i++ )
	{		
		RgnPtr = GetFieldRgnPtr(i, false);
		if ( NULL == RgnPtr ) {	continue; }		
		RgnPtr_Parent = RgnPtr->GetRgnParent();
		if ( NULL != RgnPtr_Parent )
		{
			if ( false == RgnPtr_Parent->GetRgnNeedToCalculate() )
			{	continue; }

			RgnCalcState2 = RgnPtr_Parent->GetRgnCalcState();			
			if ( REGION_CALC_DONE != RgnCalcState2 )
			{	return false; }			
		}
		if ( false == RgnPtr->GetRgnNeedToCalculate() )
		{	continue; }
		RgnCalcState = RgnPtr->GetRgnCalcState();		
		if ( REGION_CALC_DONE != RgnCalcState )
		{	return false; }
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
size_t CAOIField::CalcFieldRgnUnCalculatedCount()//計算區域內未計算的區域數量
{
	size_t             i = 0;
	size_t             Count=0;
	bool               bCalculated=false;
	CAOIRgn           *RgnPtr = NULL;	
	const size_t FieldRgnCount = GetFieldRgnPtrCount();			
	for ( i=0; i<FieldRgnCount; i++ )
	{		
		RgnPtr = GetFieldRgnPtr(i, false);
		if ( NULL == RgnPtr ) {	continue; }
		bCalculated = RgnPtr->GetRgnCalculated();
		if ( true == bCalculated ) { continue; }		
		Count ++;
	}	
	return Count;
}
//-------------------------------------------------------------------------------------//
bool CAOIField::CheckFieldFramesMergeFinish()//確認畫面的相機有取到影像
{
	size_t       i=0, j=0;
	CAOIFrame   *FramePtr = NULL;
	FRAME_MERGE_STATE MergeState=FRAME_MERGE_NONE;
	const size_t FieldFrames = GetFieldFramePtrCount();
	if ( 0 == FieldFrames ) { return true; }

	for ( i=0; i<FieldFrames; i++ )
	{
		FramePtr = GetFieldFramePtr(i, false);
		if ( NULL == FramePtr ) { continue; }
		MergeState = FramePtr->GetFrameMergeState();
		//if ( FRAME_MERGE_NONE == MergeState ) { return false; }
		//if ( FRAME_MERGE_DOING == MergeState ) { return false; }
		//if ( FRAME_MERGE_CLEAR == MergeState ) { return false; }
		if ( FRAME_MERGE_DONE != MergeState )
		{	return false; }
	}
	return true;

	unsigned int HeightID = 0;	
	for ( i=0; i<FieldFrames; i++ )
	{
		FramePtr = GetFieldFramePtr(i, false);
		if ( NULL == FramePtr ) { continue; }
		HeightID = FramePtr->GetFrameHeightID();
		if ( HeightID > 0 )
		{	break; }
	}
	if ( i == FieldFrames )//沒有2次高度
	{	return true; }

	//合併多重高度焦點影像
	if ( CombineFrameMultiFocusImage() == false )
	{	return false; }	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIField::CheckFieldFramesCalcFinish()//確認區域內所有影像都已計算完成
{
	size_t i = 0;
	CAOIFrame   *FramePtr = NULL;
	FRAME_CALC_STATE CalcState=FRAME_CALC_NONE;
	const size_t FieldFrames = GetFieldFramePtrCount();
	for ( i=0; i<FieldFrames; i++ )
	{
		FramePtr = GetFieldFramePtr(i, false);
		if ( NULL == FramePtr ) { continue; }
		CalcState = FramePtr->GetFrameCalcState();
		if ( FRAME_CALC_DONE != CalcState )
		{	return false; }
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIField::CheckFieldMergeFinish()//確認畫面的相機有取到影像
{
	FIELD_MERGE_STATE MergeState=GetFieldMergeState();
	if ( FIELD_MERGE_DONE != MergeState )
	{	return false; }
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CAOIField::ExecFieldCalc(unsigned int ThreadIdx)//執行區域的計算
{	
	const char fnName[] = "CAOIField::ExecFieldCalc";
	CAOIProject *Project = GetFieldProjectPtr();
	if ( NULL == Project ) { return false; }
#ifdef _DEBUG
	CString str;
	str.Format(_T("CAOIField::ExecFieldCalc[%d]::%d//%d\n"), ThreadIdx+1, GetFieldIndex()+1, Project->GetProjectTempFieldCount());
	TRACE(str);
#endif //_DEBUG
	CAOIFrame *FramePtr = NULL;	
	const size_t FrameIdx = 0;	
	FramePtr = CAOIField::GetFieldFramePtr(FrameIdx, true);
	if ( NULL == FramePtr ) { return false; }
	FRAME_TYPE  FrameType = FramePtr->GetFrameType();
	if ( FRAME_SPACE == FrameType ) { return false; }	
	const unsigned int ImageIndex = FramePtr->GetFrameImageIndex();
	if ( -1 == ImageIndex ) { return false; }
	
	bool bAllocated=false;
	const int nAlign = 4;
	IMAGE_SIZE ImageW=0;
	IMAGE_SIZE ImageH=0;
	IMAGE_SIZE ImageStep=0;
	IMAGE_SIZE BitCount=0;
	IMAGE_PTR  ImagePtr = NULL;	
	
	TPOINT2D StagePos;	
	
	StagePos.x = GetFieldStagePosX();
	StagePos.y = GetFieldStagePosY();
	FramePtr->GetFrameImagePtr(ImageW, ImageH, ImageStep, BitCount, ImagePtr);
	if ( FRAME_BAYER == FrameType )
	{	
		IMAGE_SIZE   DeBayerBit=0;
		IMAGE_SIZE   DeBayerStep=0;		
		IMAGE_PTR    DeBayerPtr=NULL;
		BAYER_PATTERN_MODE BayerPattern = FramePtr->GetFrameBayerPattern();		
		if ( AOIDataCollect.ExecDebayerImage(fnName, ImageW, ImageH, ImageStep, ImagePtr, BayerPattern, DeBayerStep, DeBayerBit, DeBayerPtr) == false)		
		{	return false;	}
		ImagePtr = DeBayerPtr;
		BitCount = DeBayerBit;		
		ImageStep = DeBayerStep;		
		DeBayerPtr = NULL;
		bAllocated = true;
	}
	Project->AddProjectMapPtr(StagePos, ImageIndex, ImageW, ImageH, ImageStep, BitCount, ImagePtr);
	FramePtr->ClearFrameBuffer();
	if ( true == bAllocated )
	{	JetMemory.free_func(ImagePtr); }
	return true;
}
//-------------------------------------------------------------------------------------//
double CAOIField::GetFieldMergeTime() const 
{ 
	return m_FieldMergeTime; 
}
//-------------------------------------------------------------------------------------//
void CAOIField::SetFieldMergeTime(double value)
{ 
	m_FieldMergeTime = value; 
}
//-------------------------------------------------------------------------------------//
FIELD_CALC_STATE CAOIField::GetFieldCalcState() const 
{ 
	return m_FieldCalcState; 
}
//-------------------------------------------------------------------------------------//
bool CAOIField::CheckFieldCalcDone() const
{
	if ( FIELD_CALC_DONE != GetFieldCalcState() )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIField::SetFieldCalcState(FIELD_CALC_STATE value) 
{ 
	m_FieldCalcState = value; 
}
//-------------------------------------------------------------------------------------//
FIELD_MERGE_STATE CAOIField::GetFieldMergeState() const 
{ 
	return m_FieldMergeState; 
}
//-------------------------------------------------------------------------------------//
void CAOIField::SetFieldMergeState(FIELD_MERGE_STATE value) 
{ 
	m_FieldMergeState = value; 
}
//-------------------------------------------------------------------------------------//
bool CAOIField::CheckRegionInField(const TREGION4D &Region, bool CadMode, bool Inner)//確認區域是否在此圖像區域
{
	TREGION4D rgnField;
	if ( true == CadMode )
	{
		if ( true == Inner )
		{	GetFieldCadRgn_Inner(rgnField);	}
		else
		{	GetFieldCadRgn_Outer(rgnField);	}
	}
	else
	{
		if ( true == Inner )
		{	GetFieldStageRgn_Inner(rgnField);	}
		else
		{	GetFieldStageRgn_Outer(rgnField);	}
	}

	//if ( Region.maxX < rgnField.minX ) { return false; }
	if ( Region.minX < rgnField.minX ) { return false; }
	//if ( Region.maxY < rgnField.minY ) { return false; }
	if ( Region.minY < rgnField.minY ) { return false; }
	//if ( Region.minX > rgnField.maxX ) { return false; }
	if ( Region.maxX > rgnField.maxX ) { return false; }
	//if ( Region.minY > rgnField.maxY ) { return false; }
	if ( Region.maxY > rgnField.maxY ) { return false; }
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CAOIField::CheckRegionPartInField(const TREGION4D &Region, bool CadMode, bool Inner)//確認區域是否部分在此圖像區域
{
	TREGION4D rgnField;
	if ( true == CadMode )
	{
		if ( true == Inner )
		{	GetFieldCadRgn_Inner(rgnField);	}
		else
		{	GetFieldCadRgn_Outer(rgnField);	}
	}
	else
	{
		if ( true == Inner )
		{	GetFieldStageRgn_Inner(rgnField);	}
		else
		{	GetFieldStageRgn_Outer(rgnField);	}
	}
	
	if ( Region.minX > rgnField.maxX ) { return false; }	
	if ( Region.minY > rgnField.maxY ) { return false; }	
	if ( Region.maxX < rgnField.minX ) { return false; }	
	if ( Region.maxY < rgnField.minY ) { return false; }
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CAOIField::CheckFieldFrameMatchFrameParam(std::vector<TFrameParam> &FrameParamList)//確認區域內的所有影像等同於目前的影像參數列表
{	
	const size_t FrameParamCount = FrameParamList.size();
	const size_t FrameCount = CAOIField::GetFieldFramePtrCount_Inline();
	if ( FrameCount != FrameParamCount ) { return false; }

	size_t       i=0;
	CAOIFrame   *FramePtr = NULL;
	FRAME_TYPE   FrameType = FRAME_NULL;
	TFrameParam *FrameParamPtr = NULL;	
	size_t       SliceCount = 0;
	for ( i=0; i<FrameCount; i++ )
	{
		FramePtr = CAOIField::GetFieldFramePtr_Inline(i);
		if ( NULL == FramePtr ) { return false; }
		FrameParamPtr = &(FrameParamList[i]);

		FrameType = FrameParamPtr->FrameType;
		if ( FramePtr->GetFrameType() != FrameType ) { return false; }		
		SliceCount = FramePtr->GetFrameSliceCount();		
	#ifndef OFFLINE_VERSION
		switch ( FrameType )
		{
		case FRAME_GRAY:
			if ( 1 != SliceCount ) { return false; }
			break;
		case FRAME_BAYER:
			if ( 1 != SliceCount ) { return false; }
			break;
		case FRAME_COLOR:
			if ( 3 != SliceCount ) { return false; }
			break;
		case FRAME_SPACE:
			if ( 4 > SliceCount ) { return false; }
			break;
		default:
			return false;
			break;	  
		}
	#endif//OFFLINE_VERSION
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIField::WriteFieldFile(CAOIFileIO &FileIO)//儲存區域
{	
#ifdef _DEBUG
	if ( FileIO.CheckFileMode() == false ) { return false; }
	if ( FileIO.CheckFileOpened() == false ) { return false; }
#endif//_DEBUG

	CAOIField *pField = this;	
	char     uuidStr[MAX_JET_PATH]="";	
	wchar_t  uuidWStr[MAX_JET_PATH]=L"";	
	UUID     uuid = pField->GetObjUuid();	
	FileIO.SetFnName(_T("CAOIField::WriteFieldFile"));
	JetAPI::UUIDToStringA(uuid, uuidStr);
	JetAPI::UUIDToStringW(uuid, uuidWStr);
	//----------------------------------------------------------------------------------------//
	//區域參數
	if ( FileIO.SaveChunk_INT(FILE_IO_FIELD_START, 0) == false ) { return false; }

	if ( FileIO.SaveChunk_INT(FILE_IO_FIELD_INDEX, pField->GetFieldIndex()) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_FIELD_LIST_MODE, pField->GetFieldListMode()) == false ) { return false; }	
	if ( FileIO.SaveChunk_INT(FILE_IO_FIELD_PANEL_INDEX, pField->GetFieldPanelIndex()) == false ) { return false; }		
	if ( FileIO.SaveChunk_DBL(FILE_IO_FIELD_CAD_POS_X, pField->GetFieldCadPosX()) == false ) { return false; }	
	if ( FileIO.SaveChunk_DBL(FILE_IO_FIELD_CAD_POS_Y, pField->GetFieldCadPosY()) == false ) { return false; }	
	if ( FileIO.SaveChunk_DBL(FILE_IO_FIELD_STAGE_POS_X, pField->GetFieldStagePosX()) == false ) { return false; }	
	if ( FileIO.SaveChunk_DBL(FILE_IO_FIELD_STAGE_POS_Y, pField->GetFieldStagePosY()) == false ) { return false; }	
	if ( FileIO.SaveChunk_DBL(FILE_IO_FIELD_STAGE_POS_Z, pField->GetFieldStagePosZ()) == false ) { return false; }		
	if ( FileIO.SaveChunk_DBL(FILE_IO_FIELD_SIZE_CX_INNER, pField->GetFieldSizeW_Inner()) == false ) { return false; }	
	if ( FileIO.SaveChunk_DBL(FILE_IO_FIELD_SIZE_CY_INNER, pField->GetFieldSizeH_Inner()) == false ) { return false; }	
	if ( FileIO.SaveChunk_DBL(FILE_IO_FIELD_SIZE_CX_OUTER, pField->GetFieldSizeW_Outer()) == false ) { return false; }	
	if ( FileIO.SaveChunk_DBL(FILE_IO_FIELD_SIZE_CY_OUTER, pField->GetFieldSizeH_Outer()) == false ) { return false; }
	if ( FileIO.GetSaveWStr() == true ) 
	{	if ( FileIO.SaveChunk_STR(FILE_IO_FIELD_OBJ_UUID, uuidWStr) == false ) { return false; } }
	else
	{	if ( FileIO.SaveChunk_STR(FILE_IO_FIELD_OBJ_UUID, uuidStr) == false ) { return false; } }

	if ( FileIO.SaveChunk_INT(FILE_IO_FIELD_BOARD_INDEX, pField->GetFieldBoardIndex()) == false ) { return false; }		
	if ( FileIO.SaveChunk_INT(FILE_IO_FIELD_COMPONENT_INDEX, pField->GetFieldComponentIndex()) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_FIELD_DISTRICT_ID, pField->GetFieldDistrictID()) == false ) { return false; }	
	if ( FileIO.SaveChunk_INT(FILE_IO_FIELD_GRAB_INDEX_BY_USER, pField->GetFieldGrabIndexByUser()) == false ) { return false; }		

	if ( FileIO.SaveChunk_INT(FILE_IO_FIELD_END, 0) == false ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIField::ReadFieldFile(CAOIFileIO &FileIO)//載入區域
{
#ifdef _DEBUG
	if ( FileIO.CheckFileMode() == false ) { return false; }
	if ( FileIO.CheckFileOpened() == false ) { return false; }
#endif//_DEBUG

	UUID       uuid;
	int        index=0;	
	CAOIField *pField = this;		
	FileIO.SetFnName(_T("CAOIField::ReadFieldFile"));
	//----------------------------------------------------------------------------------------//
	while ( FileIO.CheckFileEnd()==false )
	{ 		
		if ( FileIO.LoadChunk(index) == false )
		{	continue; }			
		
		switch ( index )
		{
		case FILE_IO_FIELD_START://區域參數-起點
			break;
		case FILE_IO_FIELD_END://區域參數-終點
			return true;
			break;
		case FILE_IO_FIELD_INDEX:
			index = FileIO.GetData_INT();
			pField->SetFieldIndex(index);
			pField->SetFieldGrabIndexByUser(index);
			break;
		case FILE_IO_FIELD_LIST_MODE:
			pField->SetFieldListMode((FIELD_LIST_MODE)(FileIO.GetData_INT()));
			break;
		case FILE_IO_FIELD_PANEL_INDEX:
			pField->SetFieldPanelIndex(FileIO.GetData_INT());
			break;
		case FILE_IO_FIELD_CAD_POS_X:
			pField->SetFieldCadPosX(FileIO.GetData_DBL());			
			break;
		case FILE_IO_FIELD_CAD_POS_Y:
			pField->SetFieldCadPosY(FileIO.GetData_DBL());			
			break;
		case FILE_IO_FIELD_STAGE_POS_X:
			pField->SetFieldStagePosX(FileIO.GetData_DBL());			
			break;
		case FILE_IO_FIELD_STAGE_POS_Y:
			pField->SetFieldStagePosY(FileIO.GetData_DBL());			
			break;
		case FILE_IO_FIELD_STAGE_POS_Z:
			pField->SetFieldStagePosZ(FileIO.GetData_DBL());
			break;
		case FILE_IO_FIELD_SIZE_CX_INNER:			
			pField->SetFieldSizeW_Inner(FileIO.GetData_DBL());			
			break;
		case FILE_IO_FIELD_SIZE_CY_INNER:			
			pField->SetFieldSizeH_Inner(FileIO.GetData_DBL());			
			break;
		case FILE_IO_FIELD_SIZE_CX_OUTER:			
			pField->SetFieldSizeW_Outer(FileIO.GetData_DBL());			
			break;
		case FILE_IO_FIELD_SIZE_CY_OUTER:			
			pField->SetFieldSizeH_Outer(FileIO.GetData_DBL());			
			break;
		case FILE_IO_FIELD_OBJ_UUID:
			if ( FileIO.GetLoadWStr()==true )
			{
				if ( JetAPI::UUIDFromStringW(FileIO.GetData_WSTR(), uuid) == true )
				{	pField->SetObjUuid(uuid);	}
			}
			else
			{
				if ( JetAPI::UUIDFromStringA(FileIO.GetData_STR(), uuid) == true )
				{	pField->SetObjUuid(uuid);	}
			}
			break;
		case FILE_IO_FIELD_BOARD_INDEX://區域參數-的單板引數
			pField->SetFieldBoardIndex(FileIO.GetData_INT());
			break;
		case FILE_IO_FIELD_COMPONENT_INDEX://區域參數-的零件引數
			pField->SetFieldComponentIndex(FileIO.GetData_INT());
			break;
		case FILE_IO_FIELD_DISTRICT_ID://區域參數-的多段編號
			pField->SetFieldDistrictID((DISTRICT_ID)(FileIO.GetData_INT()));
			break;
		case FILE_IO_FIELD_GRAB_INDEX_BY_USER://區域參數-自定義取像次序
			pField->SetFieldGrabIndexByUser((DISTRICT_ID)(FileIO.GetData_INT()));
			break;
		default:
			break;
		}
	};		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIField::SaveFieldFrames_Offline()//儲存區域影像_離線編程
{
	if ( AOIDataCollect.GetSaveOfflineImageFiles() == false )
	{	return true; }
	
	const int  FieldFrames = (int)(GetFieldFramePtrCount_Inline());	
	const int  nOpenMP = MIN(FieldFrames, GetFieldOpenMPCount());
	std::vector<bool> IsOK(FieldFrames, true);

#pragma omp parallel for num_threads(nOpenMP)
	for ( int i=0; i<FieldFrames; i++ )
	{
		if ( i >= FieldFrames ) { continue; }
		CAOIFrame *FramePtr = GetFieldFramePtr_Inline(i);
		if ( NULL == FramePtr ) { continue; }
		if ( FramePtr->SaveFrameImage_Offline() == true )
		{	continue; }
		IsOK[i] = false;
	}	

	for ( int i=0; i<FieldFrames; i++ )
	{
		if ( true == IsOK[i] ) { continue; }
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIField::MergeFieldFrames()//執行區域多重聚焦影像	
{
	if ( SaveFieldFrames_Offline() == false )
	{	return false; }		
	return true; 	
}
//-------------------------------------------------------------------------------------//
bool CAOIField::CombineFrameMultiFocusImage()//區域多重焦點(高度)影像
{
	//合併至第1個高度群
	size_t       i=0, j=0;
	CAOIFrame   *FramePtr=NULL;
	int          TempInt=0;
	unsigned int ImageIndex=0;
	const int FnEnable = FN_ENABLE;
	const int FnDisable = FN_DISABLE;
	const unsigned int MaxFrameCount = 8;
	const size_t FieldFrames = GetFieldFramePtrCount_Inline();
	std::vector<std::vector<CAOIFrame*>> FrameListArray;
	for ( i=0; i<FieldFrames; i++ )
	{
		FramePtr = GetFieldFramePtr(i, false);
		if ( NULL == FramePtr ) { continue; }
		FramePtr->SetFrameTempInt(FnDisable);
	}
	for ( j=0; j<MaxFrameCount; j++ )
	{
		std::vector<CAOIFrame*> FrameList;
		for ( i=0; i<FieldFrames; i++ )
		{
			FramePtr = GetFieldFramePtr(i, false);
			if ( NULL == FramePtr ) { continue; }
			TempInt = FramePtr->GetFrameTempInt();
			if ( FnDisable != TempInt ) { continue; }			
			ImageIndex = FramePtr->GetFrameImageIndex();
			if ( ImageIndex != j ) { continue; }			
			FrameList.push_back(FramePtr);
			FramePtr->SetFrameTempInt(FnEnable);			
		}
		const size_t FrameCount = FrameList.size();
		if ( 0 == FrameCount ) { continue; }
		FrameListArray.push_back(FrameList);		
	}
	const size_t FrameListCount = FrameListArray.size();
	if ( 0 == FrameListCount )
	{	return true; }

	const char fnName[] = "CAOIField::CombineFrameMultiFocusImage";
	const int  nAlign = 4;
	const int  MaxHeightCnt=2;		
	FRAME_TYPE FrameType[MaxHeightCnt];	
	IMAGE_SIZE ImageW[MaxHeightCnt] = {0};
	IMAGE_SIZE ImageH[MaxHeightCnt] = {0};	
	IMAGE_SIZE ImageStep[MaxHeightCnt] = {0};
	IMAGE_SIZE BitCount[MaxHeightCnt] = {0};	
	size_t     BufferSize[MaxHeightCnt] = {0};		
	IMAGE_PTR  ImagePtr[MaxHeightCnt] = {NULL};	
	MASK_PTR   MaskPtr[MaxHeightCnt] = {NULL};	
	SPACE_PTR  SpacePtr[MaxHeightCnt] = {NULL};	

	IMAGE_SIZE GrayBitCnt = 8;
	IMAGE_SIZE GrayStep[MaxHeightCnt] = {0};
	IMAGE_PTR  GrayPtr[MaxHeightCnt] = {NULL};		
	for ( i=0; i<MaxHeightCnt; i++ )
	{
		ImageW[i] = 0;
		ImageH[i] = 0;
		ImageStep[i] = 0;
		BitCount[i] = 0;		
		ImagePtr[i] = NULL;
		MaskPtr[i] = NULL;
		SpacePtr[i] = NULL;

		GrayStep[i] = NULL;
		GrayPtr[i] = NULL;
	}
	for ( j=0; j<FrameListCount; j++ )
	{
		std::vector<CAOIFrame*> FrameList=FrameListArray[j];
		const size_t FrameCount = FrameList.size();
		if ( FrameCount < MaxHeightCnt )
		{	continue; }

		for ( i=0; i<MaxHeightCnt; i++ )
		{
			FramePtr = FrameList[i];
			if ( NULL == FramePtr ) { continue; }
			FrameType[i] = FramePtr->GetFrameType();
			if ( FramePtr->GetFrameImagePtr(ImageW[i], ImageH[i], ImageStep[i], BitCount[i], ImagePtr[i]) == false )
			{	continue; }
			if ( NULL == ImagePtr[i] )
			{	continue; }			
			GrayStep[i] = JetAPI::GetBMPImagePixelsPerLine(ImageW[i], GrayBitCnt, nAlign);
			BufferSize[i] = ImageAPI.CalcBufferSize(GrayStep[i], ImageH[i]);
			if ( JetMemory.alloc_func(BufferSize[i], GrayPtr[i], fnName, "GrayPtr") == false )
			{
				for ( i=0; i<MaxHeightCnt; i++ )
				{	JetMemory.free_func(GrayPtr[i]);	}
				return true;
			}
			if ( GrayStep[i] == ImageStep[i] )
			{	::memcpy(GrayPtr[i], ImagePtr[i], sizeof(IMAGE_DATA)*BufferSize[i]);	}
			else if ( 24 == BitCount[i])
			{
				RECT RoiRect={0};
				bool Reverse = false;
				int WR=100, WG=100, WB=100;
				IMAGE_SRC_MODE SrcMode=IMAGE_SRC_RED;
				JetAPI::SizeToRect(ImageW[i], ImageH[i], RoiRect);				
				if ( ImageAPI.ColorImageToGrayImage3(ImageW[i], ImageH[i], ImageStep[i], ImagePtr[i], RoiRect, GrayStep[i], GrayPtr[i], SrcMode, WR, WG, WB, Reverse) == false )
				{	JetMemory.free_func(GrayPtr[i]);	}
			}
			else
			{	JetMemory.free_func(GrayPtr[i]);	}
		}
		
		for ( i=0; i<MaxHeightCnt; i++ )
		{
			if ( NULL == GrayPtr[i] )
			{	break; }			
		}
		if ( i != MaxHeightCnt )
		{
			for ( i=0; i<MaxHeightCnt; i++ )
			{	JetMemory.free_func(GrayPtr[i]);	}
			continue;
		}
		//找到對應的兩個比較圖檔
		break;
	}

	const IMAGE_SIZE CmpW = ImageW[0];
	const IMAGE_SIZE CmpH = ImageH[0];
	const IMAGE_SIZE CmpStep = GrayStep[0];
	const IMAGE_SIZE CmpBitCnt = GrayBitCnt;	
	for ( i=0; i<MaxHeightCnt; i++ )
	{
		if ( ImageW[i]!=CmpW || ImageH[i]!=CmpH || GrayStep[i]!=CmpStep || NULL==GrayPtr[i] )
		{	
			for ( i=0; i<MaxHeightCnt; i++ )
			{	JetMemory.free_func(GrayPtr[i]);	}
			return true; 
		}
	}		

	const int  RoiSize = 21;
	const int  BlurSize = 21;
	MASK_PTR   FocusPtr[MaxHeightCnt] = {NULL};	
	const size_t CmpSize = ImageAPI.CalcBufferSize(CmpStep, CmpH);
	for ( i=0; i<MaxHeightCnt; i++ )
	{	FocusPtr[i] = NULL;	}	
	for ( i=0; i<MaxHeightCnt; i++ )
	{
		if ( JetMemory.alloc_func(CmpSize, FocusPtr[i], fnName, "FocusPtr") == false )
		{
			for ( i=0; i<MaxHeightCnt; i++ )
			{					
				JetMemory.free_func(GrayPtr[i]);	
				JetMemory.free_func(FocusPtr[i]);	
			}
			return true;
		}
		if ( ImageAPI.BuildFocusPixelImage3(CmpW, CmpH, CmpStep, CmpBitCnt, GrayPtr[i], BlurSize, RoiSize, FocusPtr[i]) == false )
		{
			for ( i=0; i<MaxHeightCnt; i++ )
			{					
				JetMemory.free_func(GrayPtr[i]);	
				JetMemory.free_func(FocusPtr[i]);	
			}
			return true;
		}
	}
	/*
	CString str;
	CString strFolder = AOIDataCollect.GetAOITempDirectory();
	str.Format(_T("%s\\%s"), strFolder, _T("GrayPtr1.PNG"));	
	ImageAPI.SaveImage(str, CmpW, CmpH, CmpStep, CmpBitCnt, GrayPtr[0], true);
	str.Format(_T("%s\\%s"), strFolder, _T("GrayPtr2.PNG"));	
	ImageAPI.SaveImage(str, CmpW, CmpH, CmpStep, CmpBitCnt, GrayPtr[1], true);

	str.Format(_T("%s\\%s"), strFolder, _T("FocusPtr1.PNG"));	
	ImageAPI.SaveImage(str, CmpW, CmpH, CmpStep, CmpBitCnt, FocusPtr[0], true);
	str.Format(_T("%s\\%s"), strFolder, _T("FocusPtr2.PNG"));	
	ImageAPI.SaveImage(str, CmpW, CmpH, CmpStep, CmpBitCnt, FocusPtr[1], true);	
	//*/
	bool bIsOK = true;
	for ( j=0; j<FrameListCount; j++ )
	{
		std::vector<CAOIFrame*> FrameList=FrameListArray[j];
		const size_t FrameCount = FrameList.size();
		if ( FrameCount < MaxHeightCnt )
		{	continue; }

		for ( i=0; i<MaxHeightCnt; i++ )
		{
			FramePtr = FrameList[i];
			if ( NULL == FramePtr ) { continue; }
			FrameType[i] = FramePtr->GetFrameType();
			if ( FRAME_SPACE == FrameType[i] )
			{	
				if ( FramePtr->GetFrameSpacePtr(ImageW[i], ImageH[i], ImageStep[i], BitCount[i], MaskPtr[i], SpacePtr[i]) == false )
				{	continue; }	
			}
			else
			{
				if ( FramePtr->GetFrameImagePtr(ImageW[i], ImageH[i], ImageStep[i], BitCount[i], ImagePtr[i]) == false )
				{	continue; }			
			}
		}
		for ( i=0; i<MaxHeightCnt; i++ )
		{
			if ( FrameType[i] != FrameType[0] )
			{	break; }
		}
		if ( i != MaxHeightCnt )
		{	continue; }

		switch ( FrameType[0] )
		{
		case FRAME_GRAY:
			bIsOK = ImageAPI.MergeImage2FrameByFocus3(ImageW[0], ImageH[0], ImageStep[0], BitCount[0], ImagePtr[0], ImagePtr[1], CmpStep, FocusPtr[0], FocusPtr[1], ImagePtr[0]);
			break;
		case FRAME_BAYER:
			break;
		case FRAME_COLOR:
			bIsOK = ImageAPI.MergeImage2FrameByFocus3(ImageW[0], ImageH[0], ImageStep[0], BitCount[0], ImagePtr[0], ImagePtr[1], CmpStep, FocusPtr[0], FocusPtr[1], ImagePtr[0]);
			break;
		case FRAME_SPACE:
			bIsOK = ImageAPI.MergeSpace2FrameByFocus3(ImageW[0], ImageH[0], ImageStep[0], SpacePtr[0], SpacePtr[1], MaskPtr[0], MaskPtr[1], CmpStep, FocusPtr[0], FocusPtr[1], SpacePtr[0], MaskPtr[0]);
			break;
		}
		if ( false == bIsOK )
		{	break;	}
	}
	for ( i=0; i<MaxHeightCnt; i++ )
	{					
		JetMemory.free_func(GrayPtr[i]);	
		JetMemory.free_func(FocusPtr[i]);	
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
CString CAOIField::GetFieldIndexName() const
{
	CString IndexName;	
	IndexName.Format(_T("Field_%04d"), GetFieldIndex()+1);
	return IndexName;
}
//-------------------------------------------------------------------------------------//