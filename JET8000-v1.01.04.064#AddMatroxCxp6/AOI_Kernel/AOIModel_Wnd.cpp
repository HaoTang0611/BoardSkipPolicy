// AOIModel_Wnd.cpp: implementation of the CAOIModel class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "AOIModel.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
inline void CAOIModel::ClearModelWndOrderList_Inline()//清除模組檢測框檢測次序列表
{
	CAOIModel::m_ModelWndOrderList.clear();
}
//-------------------------------------------------------------------------------------//
inline size_t CAOIModel::GetModelWndOrderCount_Inline() const
{
	return CAOIModel::m_ModelWndOrderList.size();
}
//-------------------------------------------------------------------------------------//
inline CAOIWnd* CAOIModel::GetModelWndOrderPtr_Inline(size_t index) const
{
	return (CAOIModel::m_ModelWndOrderList[index]);	
}
//-------------------------------------------------------------------------------------//
inline size_t CAOIModel::GetModelWndCount_Inline() const
{
	return CAOIModel::m_ModelWndList.size();
}
//-------------------------------------------------------------------------------------//
inline void CAOIModel::AddModelWndPtr_Inline(CAOIWnd *WndPtr)
{
	CAOIModel::m_ModelWndList.push_back(WndPtr);
}
//-------------------------------------------------------------------------------------//
inline CAOIWnd* CAOIModel::GetModelWndPtr_Inline(size_t index) const
{	
	return (CAOIModel::m_ModelWndList[index]);	
}
//-------------------------------------------------------------------------------------//
void CAOIModel::ClearModelWndList()
{
	size_t       i = 0;
	CAOIWnd     *WndPtr = NULL;
	const size_t WndCount = GetModelWndCount_Inline();
	for ( i=0; i<WndCount; i++ )
	{	
		WndPtr = GetModelWndPtr_Inline(i);
		if ( NULL == WndPtr ) { continue; }
		AOIObjManager.DestroyWndObj(WndPtr);		
	}
	m_ModelWndList.clear();	
	m_ModelWndOrderList.clear();
}
//-------------------------------------------------------------------------------------//
size_t CAOIModel::GetModelWndCount() const
{ 
	return GetModelWndCount_Inline();
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::InitialModelWndPtr(CAOIWnd *WndPtr)//將特定模組參數帶入檢測框
{
	if ( NULL == WndPtr ) { return false; }
	WndPtr->SetWndClassID(m_ModelActClassID);
	WndPtr->SetWndAttachedAngle(m_ModelAttachedAngle);	
	WndPtr->SetWndAttachedPosCad(m_ModelAttachedCadPos);
	WndPtr->SetWndAttachedPosStage(m_ModelAttachedStagePos);
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIModel::AddModelWndPtr_Direct(CAOIWnd *WndPtr)
{
	AddModelWndPtr_Inline(WndPtr);
}
//-------------------------------------------------------------------------------------//
CAOIWnd* CAOIModel::AddModelWndPtr(CAOIWnd *WndPtr, bool Clone)
{
	if ( NULL == WndPtr ) { return NULL; }
	if ( false == Clone )
	{
		unsigned int WndIndex = (unsigned int)(GetModelWndCount_Inline());
		AddModelWndPtr_Inline(WndPtr);
		WndPtr->SetWndIndex(WndIndex);	
		WndPtr->SetWndModelPtr(this);
		SetModelModifiedCount(true);
		SetModelNeedSaveFiles(true);
		return WndPtr;
	}
	CAOIWnd *NewWndPtr = WndPtr;
	NewWndPtr = CopyModelWnd(WndPtr);
	SetModelModifiedCount(true);
	SetModelNeedSaveFiles(true);
	return NewWndPtr;
}
//-------------------------------------------------------------------------------------//
CAOIWnd* CAOIModel::CopyModelWnd(const CAOIWnd *WndPtr)
{
	if ( NULL == WndPtr ) { return NULL; }
	CAOIWnd *NewWndPtr = WndPtr->CloneWndObj();
	if( NewWndPtr == NULL )	{	return NULL; }
	const int WndGroupID = NewWndPtr->GetWndGroupID();
	const int WndBandID = GetModelWndFreeBandID(WndGroupID);
	NewWndPtr->SetWndBandID(WndBandID);
	CAOILand *LandPtr = NewWndPtr->GetWndLandPtr();
	if ( NULL != LandPtr )
	{	LandPtr->AddLandWndPtr(NewWndPtr); }

	NewWndPtr = AddModelWndPtr(NewWndPtr, false);	
	return NewWndPtr;
}
//-------------------------------------------------------------------------------------//
CAOIWnd* CAOIModel::CreateModelWnd(WND_DEFECT_ID WndDefectID, TREGION4D Region, CAOILand *LandPtr)
{
	CAOIWnd   *WndPtr = NULL;
	BOX_TOWARD WndToward = BOX_TOWARD_NULL;	
	TSIZE2D    szPad, szLead, szBody;	
	TPOINT2D   BoxCp;	
	TREGION4D  RgnBody;
	TREGION4D  RgnPad, RgnLead, RgnLeadTip, RgnLeadShoulder;
	double       BodyMarginX=100, BodyMarginY=100;
	double       scPadU=1.0, scPadV=1.0;
	const double AttachedAngle = GetModelAttachedAngle();
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(AttachedAngle);

	GetModelBodyRegion(RgnBody);
	BoxCp.x = (Region.minX+Region.maxX)*0.5;
	BoxCp.y = (Region.minY+Region.maxY)*0.5;
	szBody.cx = ::fabs(RgnBody.maxX-RgnBody.minX);
	szBody.cy = ::fabs(RgnBody.maxY-RgnBody.minY);
	BodyMarginX = szBody.cx*0.2;
	BodyMarginY = szBody.cy*0.2;

	if ( NULL == LandPtr )
	{	WndToward = GetModelBodyBox().GetBoxToward();	}
	else
	{	WndToward = LandPtr->GetLandToward();	}

	if ( true == IsExceptionAngle ) 
	{	WndToward = JetAPI::RotateToward(-AttachedAngle, WndToward);	}

	WndPtr = AOIObjManager.CreateWndObj();
	if ( NULL == WndPtr )
	{	return WndPtr; }

	MODEL_TYPE ModelType=GetModelType();
	const int WnGroupID = GetModelWndFreeGroupID();
	const int DefectGroupID = GetModelFreeDefectGroupID(WndDefectID);
	const WND_SYNC_MOVE_MODE WndSyncMoveMode = GetModelWndSyncMoveMode();

	WndPtr->SetWndToward(WndToward);	
	WndPtr->SetWndGroupID(WnGroupID);	
	WndPtr->SetWndDefectID(WndDefectID);	
	WndPtr->SetWndDefectGroupID(DefectGroupID);		
	WndPtr->SetWndRegion(Region);
	WndPtr->GetWndBoxPtr()->SetBoxSelected(true);	
	WndPtr->UpdateWndExtendBox();	
	if ( NULL != LandPtr )
	{	
		WndPtr->SetWndSyncMoveMode(WndSyncMoveMode);	
		if ( WND_SYNC_MOVE_SYMMETRY == WndSyncMoveMode )
		{
			TPOINT2D LandPos;			
			LandPtr->GetLandBoxPtr()->GetBoxPos(LandPos);			
			Region.MoveTo(LandPos.x, LandPos.y);
			WndPtr->SetWndRegion(Region);
		}
	}

	WndPtr->SetWndClassID(m_ModelActClassID);
	if ( true == IsExceptionAngle )
	{	WndPtr->RotateWnd(AttachedAngle, BoxCp.x, BoxCp.y);		}
	else
	{	WndPtr->SetWndAttachedAngle(AttachedAngle);	}
	WndPtr->SetWndAttachedPosCad(m_ModelAttachedCadPos);
	WndPtr->SetWndAttachedPosStage(m_ModelAttachedStagePos);
	return WndPtr;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::DestroyModelWndSelected()
{
	size_t       i=0;	
	size_t       WndCount = 0;		
	unsigned int WndIndex=0;
	CAOIWnd     *WndPtr = NULL;	
	std::vector<CAOIWnd*>   ModelWndList = m_ModelWndList;	

	WndIndex=0;	
	m_ModelWndList.clear();
	WndCount = ModelWndList.size();
	for ( i=0; i<WndCount; i++ )
	{	
		WndPtr = ModelWndList[i];
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndSelected() == true ) 
		{
			AOIObjManager.DestroyWndObj(ModelWndList[i]);
			continue; 
		}
		WndPtr->SetWndIndex(WndIndex);
		AddModelWndPtr_Inline(WndPtr);		
		WndIndex ++;
	}
	SetModelModifiedCount(true);
	SetModelNeedSaveFiles(true);
	return true;
}
//-------------------------------------------------------------------------------------//
CAOIWnd* CAOIModel::GetModelWndPtr(size_t index, bool Check) const
{
	if ( Check )
	{
		const size_t size = GetModelWndCount_Inline();
		if ( index >= size ) 
		{	return NULL; }
	}
	return GetModelWndPtr_Inline(index);	
}
//-------------------------------------------------------------------------------------//
CAOIWnd* CAOIModel::GetModelWndPtrByUUID(const UUID &uuid) const
{
	size_t       i=0;
	UUID         WndUUID;
	UUID         RefUUID = uuid;
	RPC_STATUS   Status=0;	
	CAOIWnd     *WndPtr = NULL;
	const size_t WndCount = GetModelWndCount_Inline();
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = (GetModelWndPtr_Inline(i));
		if ( NULL == WndPtr ) { continue; }
		WndUUID = WndPtr->GetObjUuid();
		if ( ::UuidEqual(&WndUUID, &RefUUID, &Status) == FALSE ) { continue; }		
		return WndPtr;
	}
	return NULL;
}
//-------------------------------------------------------------------------------------//
CAOIWnd* CAOIModel::GetModelWndPtrByDefectID(WND_DEFECT_ID WndDefectID, int DefectGroupID) const
{
	size_t       i=0;
	CAOIWnd     *WndPtr = NULL;
	const size_t WndCount = GetModelWndCount_Inline();
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = (GetModelWndPtr_Inline(i));
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndDefectID() != WndDefectID ) { continue; }
		if ( -1 != DefectGroupID )
		{
			if ( WndPtr->GetWndDefectGroupID() != DefectGroupID ) { continue; }
		}
		return WndPtr;
	}
	return NULL;
}
//-------------------------------------------------------------------------------------//
CAOIWnd* CAOIModel::GetModelWndPtrByGroupID(int WndGroupID, int BandID, bool NoIsolatedWnd) const
{
	size_t       i=0;
	CAOIWnd     *WndPtr = NULL;
	const size_t WndCount = GetModelWndCount_Inline();
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = (GetModelWndPtr_Inline(i));
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndGroupID() != WndGroupID ) { continue; }
		if ( BandID >= 0 ) 
		{
			if ( WndPtr->GetWndBandID() != BandID ) { continue; }
		}
		if ( true == NoIsolatedWnd )
		{
			if ( WndPtr->GetWndIsolated() == true )
			{	continue; }
		}
		return WndPtr;
	}
	return NULL;
}
//-------------------------------------------------------------------------------------//
CAOIWnd* CAOIModel::GetModelWndPtrByGroupBandID(int WndGroupID, int BandID, bool NoIsolatedWnd) const
{
	size_t i=0;
	CAOIWnd     *WndPtr = NULL;
	const size_t WndCount = CAOIModel::GetModelWndCount_Inline();
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = (CAOIModel::GetModelWndPtr_Inline(i));
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndGroupID() != WndGroupID ) { continue; }
		if ( BandID >= 0 )
		{
			if ( WndPtr->GetWndBandID() != BandID ) { continue; }
		}

		if ( true == NoIsolatedWnd )
		{
			if ( WndPtr->GetWndIsolated() == true )
			{	continue; }
		}
		return WndPtr;
	}
	return NULL;
}
//-------------------------------------------------------------------------------------//
CAOIWnd* CAOIModel::GetModelWndPtrByLandDefectID(const CAOILand *RefLandPtr, WND_DEFECT_ID WndDefectID, int DefectGroupID) const
{	
	CAOIWnd  *WndPtr = NULL;
	CAOILand *LandPtr = NULL;
	if ( NULL == RefLandPtr ) { return NULL; }
	size_t       i=0;
	LAND_TYPE    LandType = RefLandPtr->GetLandType();
	const int    LandGroupID = RefLandPtr->GetLandGroupID();
	const size_t WndCount = GetModelWndCount_Inline();
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = (GetModelWndPtr_Inline(i));
		if ( NULL == WndPtr ) { continue; }
		LandPtr = WndPtr->GetWndLandPtr();
		if ( NULL == LandPtr ) { continue; }
		if ( LandPtr->GetLandType() != LandType ) { continue; }
		if ( LandPtr->GetLandGroupID() != LandGroupID ) { continue; }

		if ( WndPtr->GetWndDefectID() != WndDefectID ) { continue; }
		if ( -1 != DefectGroupID )
		{
			if ( WndPtr->GetWndDefectGroupID() != DefectGroupID ) { continue; }
		}
		return WndPtr;
	}
	return NULL;
}
//-------------------------------------------------------------------------------------//
CAOIWnd* CAOIModel::GetModelWndFirst() const
{
	size_t         i=0;
	WND_DEFECT_ID  WndDefectID;
	CAOIWnd       *WndPtr = NULL;
	const int      ActClassID = GetModelActClassID();
	const size_t   WndCount = GetModelWndCount_Inline();	
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr_Inline(i);	
		if ( NULL == WndPtr ) { continue; }
		WndDefectID = WndPtr->GetWndDefectID();
		if ( WND_DEFECT_CLASS_CHECK == WndDefectID ) { continue; }		
		if ( WndPtr->CheckWndClassIDUsed(ActClassID) == false ) { continue; }
		return WndPtr;
	}
	return NULL;
}
//-------------------------------------------------------------------------------------//
CAOIWnd* CAOIModel::GetModelWndActived() const
{
#ifdef _DEBUG
	size_t actCount = GetModelWndActivedCount();
	if ( actCount > 1 ) 
	{	actCount = actCount; }
#endif _DEBUG


	size_t   i=0;
	CAOIWnd *WndPtr = NULL;
	const size_t WndCount = GetModelWndCount_Inline();	
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr_Inline(i);	
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndActived() == true )
		{	return WndPtr; }
	}
	return NULL;
}
//-------------------------------------------------------------------------------------//
size_t CAOIModel::GetModelWndActivedCount() const
{
	size_t   i=0;
	size_t   Count=0;
	CAOIWnd *WndPtr = NULL;
	const size_t WndCount = GetModelWndCount_Inline();	

	Count=0;
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr_Inline(i);	
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndActived() == false ) { continue; }
		Count ++;
	}
	return Count;
}
//-------------------------------------------------------------------------------------//
CAOIWnd* CAOIModel::GetModelWndDefectMaster() const
{
	size_t   i=0;	
	bool     IsDefect=false;
	CAOIWnd *WndPtr = NULL;
	RESULT_ID WndResultID;
	WND_DEFECT_ID WndDefectID;	
	const size_t WndCount = GetModelWndCount_Inline();	
	
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr_Inline(i);	
		if ( NULL == WndPtr ) { continue; }		
		WndResultID = WndPtr->GetWndResultID();
		WndDefectID = WndPtr->GetWndDefectID();

		switch ( WndResultID )
		{
		case RESULT_ID_NG:
		case RESULT_ID_EXCEPTION:
			IsDefect = true;
			break;
		default:
			IsDefect = false;
			break;
		}
		if ( false == IsDefect ) { continue; }		           
		return WndPtr;
	}
	return NULL;
}
//-------------------------------------------------------------------------------------//
void CAOIModel::SetModelWndActived(CAOIWnd *ActWndPtr)
{
	size_t   i=0;
	CAOIWnd *WndPtr = NULL;
	const size_t WndCount = GetModelWndCount_Inline();	
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr_Inline(i);	
		if ( NULL == WndPtr ) { continue; }
		WndPtr->SetWndActived(false);
	}
	if ( NULL != ActWndPtr )
	{		
		ActWndPtr->SetWndActived(true); 
		ActWndPtr->SetWndSelected(true);
		ActWndPtr->SetWndVisibled(true);
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CAOIModel::SetModelWndSelected(bool val)
{
	size_t i=0;
	CAOIWnd     *WndPtr = NULL;
	const size_t WndCount = GetModelWndCount_Inline();
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = (GetModelWndPtr_Inline(i));
		if ( NULL == WndPtr ) { continue; }
		WndPtr->SetWndSelected(val);
	}
	return;
}
//-------------------------------------------------------------------------------------//
CAOIWnd* CAOIModel::GetModelWndSelected() const
{
	size_t   i=0;
	CAOIWnd *WndPtr = NULL;
	const size_t WndCount = GetModelWndCount_Inline();	
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr_Inline(i);	
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndSelected() == true )
		{	return WndPtr; }
	}
	return NULL;
}
//-------------------------------------------------------------------------------------//
int CAOIModel::GetModelWndSelectedCount() const
{	
	size_t i=0;
	int    Count = 0;
	CAOIWnd *WndPtr = NULL;
	size_t size = GetModelWndCount_Inline();	
	Count = 0;
	for ( i=0; i<size; i++ )
	{
		WndPtr = GetModelWndPtr_Inline(i);	
		if ( NULL == WndPtr ) { continue; }		
		if ( WndPtr->GetWndSelected() == false )
		{	continue; }
		Count ++;
	}
	return Count;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::PasteModelWndList(std::vector<CAOIWnd*> WndList)//貼上模組選取到檢測匡列表
{
	size_t        i=0;
	CAOIWnd      *WndPtr = NULL;
	CAOIWnd      *NewWndPtr = NULL;
	int           WndGroupID=0;
	int           DefectGoupID=0;
	WND_DEFECT_ID WndDefectID;
	unsigned int  LandIndex=0;
	const size_t  WndCount = WndList.size();

	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = WndList[i];
		if ( NULL == WndPtr ) { continue; }
		LandIndex = WndPtr->GetWndLandIndex();
		if ( -1 != LandIndex ) { continue; }//是焊盤上的框
		NewWndPtr = WndPtr->CloneWndObj();
		if ( NULL == NewWndPtr ) { continue; }
		WndDefectID = NewWndPtr->GetWndDefectID();
		
		WndGroupID = GetModelWndFreeGroupID();	
		DefectGoupID = GetModelFreeDefectGroupID(WndDefectID);

		NewWndPtr->SetWndGroupID(WndGroupID);
		NewWndPtr->SetWndDefectGroupID(DefectGoupID);

		NewWndPtr->SetWndAttachedAngle(m_ModelAttachedAngle);
		NewWndPtr->SetWndAttachedPosCad(m_ModelAttachedCadPos);
		NewWndPtr->SetWndAttachedPosStage(m_ModelAttachedStagePos);	

		AddModelWndPtr(NewWndPtr, false);
	}

	CalcModelTotalRegionAll();
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIModel::GetModelWndSelectedList(std::vector<CAOIWnd*> &WndList)//取得模組選取到檢測匡列表
{
	size_t i=0;	
	CAOIWnd *WndPtr = NULL;
	size_t size = GetModelWndCount_Inline();
	for ( i=0; i<size; i++ )
	{
		WndPtr = GetModelWndPtr_Inline(i);	
		if ( NULL == WndPtr ) { continue; }		
		if ( WndPtr->GetWndSelected() == false )
		{	continue; }
		WndList.push_back(WndPtr);
	}
	return ;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::UpdateModelWndBypassed(const CWndDefectItem &DefectEnabled)//更新模組檢測框是否忽略
{
	size_t i=0;	
	int    DefectCount=0;
	WND_DEFECT_ID WndDefectID;
	CAOIWnd *WndPtr = NULL;
	const size_t size = GetModelWndCount_Inline();		
	for ( i=0; i<size; i++ )
	{
		WndPtr = GetModelWndPtr_Inline(i);	
		if ( NULL == WndPtr ) { continue; }				
		WndPtr->SetWndBypassed(false);
		if ( WndPtr->GetWndEnabled() == false )
		{
			WndPtr->SetWndBypassed(true);
			continue;
		}

		WndDefectID = WndPtr->GetWndDefectID();
		DefectCount = DefectEnabled.GetItemCount(WndDefectID);
		if ( WND_DEFECT_ITEM_DISABLE == DefectCount )
		{	
			WndPtr->SetWndBypassed(true); 
			continue;
		}		
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIModel::SetModelWndOtherGroupIDSelected(int WndGroupID, bool Selected)//設定模組其餘群組檢測框選取狀態
{
	size_t i=0;
	int    Count = 0;
	CAOIWnd *WndPtr = NULL;
	const size_t size = GetModelWndCount_Inline();	
	Count = 0;
	for ( i=0; i<size; i++ )
	{
		WndPtr = GetModelWndPtr_Inline(i);	
		if ( NULL == WndPtr ) { continue; }		
		if ( WndPtr->GetWndGroupID() == WndGroupID ) { continue; }
		WndPtr->SetWndSelected(Selected);		
	}
	return ;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::SetModelWndSelectedIndexList(const std::vector<unsigned int> &WndIndexList)
{
	size_t       i=0;	
	unsigned int Index=0;
	CAOIWnd     *WndPtr = NULL;	
	const size_t SelCount = WndIndexList.size();	
	
	UnSelectModelWnd();
	for ( i=0; i<SelCount; i++ )
	{
		Index = WndIndexList[i];
		WndPtr = GetModelWndPtr(Index, true);
		if ( NULL == WndPtr ) { continue; }
		WndPtr->SetWndSelected(true);		
	}		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::GetModelWndSelectedIndexList(int WndGroupID, int WndBandID, std::vector<unsigned int> &WndIndexList)
{
	size_t       i=0;	
	CAOIWnd     *WndPtr = NULL;	
	const size_t WndCount = GetModelWndCount_Inline();	

	WndIndexList.clear();
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr_Inline(i);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndSelected() == false )
		{	continue; }

		if ( WndGroupID >= 0 ) 
		{
			if ( WndPtr->GetWndGroupID() != WndGroupID ) 
			{	continue; }

			if ( WndBandID >= 0 )
			{
				if ( WndPtr->GetWndBandID() != WndBandID )
				{	continue; }
			}
		}
		
		WndIndexList.push_back(i);
	}		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::GetModelWndSelectedWndGroupIDList(std::vector<int> &WndGroupIDList)
{
	size_t       i=0, j=0;
	int          WndGroupID = 0;
	CAOIWnd     *WndPtr = NULL;
	size_t       WndGroupIDCount = 0;
	const size_t WndCount = GetModelWndCount_Inline();	

	WndGroupIDList.clear();
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr_Inline(i);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndSelected() == false )
		{	continue; }
		
		WndGroupID = WndPtr->GetWndGroupID();
		WndGroupIDCount = WndGroupIDList.size();
		for ( j=0; j<WndGroupIDCount; j++ )
		{
			if ( WndGroupIDList[j] == WndGroupID )
			{	break; }
		}
		if ( j != WndGroupIDCount ) { continue; }
		WndGroupIDList.push_back(WndGroupID);
	}		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::ListModelWndBandIDByGroupID(int WndGroupID, std::vector<int> &WndBandIDList)
{
	size_t       i=0, j=0;
	int          WndBandID = 0;
	CAOIWnd     *WndPtr = NULL;
	size_t       WndBandIDCount = 0;
	const size_t WndCount = GetModelWndCount_Inline();	

	WndBandIDList.clear();
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr_Inline(i);
		if ( NULL == WndPtr ) { continue; }		
		if ( WndPtr->GetWndGroupID() != WndGroupID ) { continue; }
		
		WndBandID = WndPtr->GetWndBandID();
		WndBandIDCount = WndBandIDList.size();
		for ( j=0; j<WndBandIDCount; j++ )
		{
			if ( WndBandIDList[j] == WndBandID )
			{	break; }
		}
		if ( j != WndBandIDCount ) { continue; }
		WndBandIDList.push_back(WndBandID);
	}		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::ListModelWndGroupIDByDefectID_AlgType(WND_DEFECT_ID WndDefectID, ALG_TYPE AlgType, std::vector<int> &WndGroupIDList)
{
	size_t       i=0, j=0;
	int          WndGroupID = 0;
	CAOIWnd     *WndPtr = NULL;
	size_t       WndGroupIDCount = 0;
	const size_t WndCount = GetModelWndCount_Inline();	

	WndGroupIDList.clear();
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr_Inline(i);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndAlgType() != AlgType ) { continue; }
		if ( WndPtr->GetWndDefectID() != WndDefectID ) { continue; }
		
		WndGroupID = WndPtr->GetWndGroupID();
		WndGroupIDCount = WndGroupIDList.size();
		for ( j=0; j<WndGroupIDCount; j++ )
		{
			if ( WndGroupIDList[j] == WndGroupID )
			{	break; }
		}
		if ( j != WndGroupIDCount ) { continue; }
		WndGroupIDList.push_back(WndGroupID);
	}		
	return true;
}
//-------------------------------------------------------------------------------------//
int CAOIModel::GetModelWndFreeGroupID() const
{
	size_t       i = 0;
	int          MaxGroupID=-1;
	CAOIWnd     *WndPtr = NULL;
	const size_t WndCount = GetModelWndCount_Inline();
	for ( i=0; i<WndCount; i++ )
	{	
		WndPtr = GetModelWndPtr_Inline(i);
		if ( NULL == WndPtr ) { continue; }
		if ( MaxGroupID < WndPtr->GetWndGroupID() )
		{	MaxGroupID = WndPtr->GetWndGroupID();	}
	}
	return (MaxGroupID+1);
}
//-------------------------------------------------------------------------------------//
int CAOIModel::GetModelWndFreeBandID() const
{
	size_t       i = 0;
	int          MaxBandID=-1;
	CAOIWnd     *WndPtr = NULL;
	const size_t WndCount = GetModelWndCount_Inline();
	for ( i=0; i<WndCount; i++ )
	{	
		WndPtr = GetModelWndPtr_Inline(i);
		if ( NULL == WndPtr ) { continue; }
		if ( MaxBandID < WndPtr->GetWndBandID() )
		{	MaxBandID = WndPtr->GetWndBandID();	}
	}
	return (MaxBandID+1);
}
//-------------------------------------------------------------------------------------//
int CAOIModel::GetModelWndFreeBandID(int WndGroupID) const
{
	size_t       i = 0;
	int          MaxBandID=-1;
	CAOIWnd     *WndPtr = NULL;
	const size_t WndCount = GetModelWndCount_Inline();
	for ( i=0; i<WndCount; i++ )
	{	
		WndPtr = GetModelWndPtr_Inline(i);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndGroupID() != WndGroupID ) { continue; }
		if ( MaxBandID < WndPtr->GetWndBandID() )
		{	MaxBandID = WndPtr->GetWndBandID();	}
	}
	return (MaxBandID+1);
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::CheckModelWndGroupEnabled(int WndGroupID) const
{
	size_t       i = 0;	
	CAOIWnd     *WndPtr = NULL;
	const size_t WndCount = GetModelWndCount_Inline();
	for ( i=0; i<WndCount; i++ )
	{	
		WndPtr = GetModelWndPtr_Inline(i);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndGroupID() != WndGroupID ) { continue; }
		if ( WndPtr->GetWndEnabled() == true )
		{	return true; }
	}
	return false;
}
//-------------------------------------------------------------------------------------//
RESULT_ID CAOIModel::CheckModelWndGroupResultID(int WndGroupID) const
{
	size_t       i = 0;	
	size_t       nOK = 0;
	size_t       nNG = 0;
	size_t       nSkip = 0;
	size_t       nBypass = 0;
	size_t       nException = 0;
	CAOIWnd     *WndPtr = NULL;
	RESULT_ID    WndResultID=RESULT_ID_NONE;
	const size_t WndCount = GetModelWndCount_Inline();
	for ( i=0; i<WndCount; i++ )
	{	
		WndPtr = GetModelWndPtr_Inline(i);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndGroupID() != WndGroupID ) { continue; }
		WndResultID = WndPtr->GetWndResultID();
		switch ( WndResultID )
		{
		case RESULT_ID_OK:	nOK ++;	break;
		case RESULT_ID_NG:	nNG ++;	break;
		case RESULT_ID_BYPASS: nBypass ++;	break;
		case RESULT_ID_EXCEPTION: nException ++;	break;
		case RESULT_ID_SKIP:	nSkip ++;	break;
		}
		//if ( RESULT_ID_NG==WndResultID || RESULT_ID_EXCEPTION==WndResultID )
		//{	return RESULT_ID_NG; }
	}
	//return RESULT_ID_OK;	
	if ( nException > 0 ) { return RESULT_ID_EXCEPTION; }
	if ( nNG > 0 ) { return RESULT_ID_NG; }
	if ( nOK > 0 ) { return RESULT_ID_OK; }
	if ( nBypass > 0 ) { return RESULT_ID_BYPASS; }
	if ( nSkip > 0 ) { return RESULT_ID_SKIP; }
	return RESULT_ID_NONE;	
}
//-------------------------------------------------------------------------------------//
RESULT_ID CAOIModel::CheckModelWndGroupLogicResultID(int WndGroupID) const
{
	size_t       i = 0;	
	CAOIWnd     *WndPtr = NULL;
	RESULT_ID    WndResultID=RESULT_ID_NONE;
	const size_t WndCount = GetModelWndCount_Inline();
	for ( i=0; i<WndCount; i++ )
	{	
		WndPtr = GetModelWndPtr_Inline(i);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndGroupID() != WndGroupID ) { continue; }
		WndResultID = WndPtr->GetWndLogicResultID();
		if ( RESULT_ID_NG==WndResultID || RESULT_ID_EXCEPTION==WndResultID )
		{	return RESULT_ID_NG; }
	}
	return RESULT_ID_OK;	
}
//-------------------------------------------------------------------------------------//
int CAOIModel::GetModelFreeDefectGroupID(WND_DEFECT_ID DefectID) const//取得模組檢測框瑕疵群組編號
{
	size_t        i = 0;	
	int           DefectGroupID=0; 
	int           FreeDefectGroupID=-1;
	CAOIWnd      *WndPtr = NULL;	
	WND_DEFECT_ID WndDefectID;
	const size_t WndCount = GetModelWndCount_Inline();
	for ( i=0; i<WndCount; i++ )
	{	
		WndPtr = GetModelWndPtr_Inline(i);
		if ( NULL == WndPtr ) { continue; }
		WndDefectID = WndPtr->GetWndDefectID();
		if ( WndDefectID != DefectID ) { continue; }
		DefectGroupID = WndPtr->GetWndDefectGroupID();
		if ( FreeDefectGroupID < DefectGroupID )
		{	FreeDefectGroupID = DefectGroupID; }
	}
	return (FreeDefectGroupID+1);	
}
//-------------------------------------------------------------------------------------//
void CAOIModel::SetModelWndSelectedByWndGroupID(int WndGroupID, int WndBandID, bool Selected)
{
	size_t       i=0;
	CAOIWnd     *WndPtr = NULL;
	const size_t WndCount = GetModelWndCount_Inline();
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = (GetModelWndPtr_Inline(i));
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndGroupID() != WndGroupID ) { continue; }
		if ( WndBandID >= 0 ) 
		{
			if ( WndPtr->GetWndBandID() != WndBandID ) { continue; }
		}

		WndPtr->SetWndSelected(Selected);
	}
	return;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::SetModelWndEnabled(bool bEnabled)//設定模組所有檢測框啟用
{
	size_t       i=0;
	CAOIWnd     *WndPtr = NULL;
	const size_t WndCount = GetModelWndCount_Inline();	
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr_Inline(i);	
		if ( NULL == WndPtr ) { continue; }		
		WndPtr->SetWndEnabled(bEnabled);
		WndPtr->SetWndModified(true);
	}
	SetModelNeedSaveFiles(true);
	return false;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::BypassSkipModelWnd(RESULT_ID value)//不檢測或跳過模組檢測框
{
	CAOIWnd *WndPtr = NULL;
	const size_t WndCount=GetModelWndCount();
	for ( size_t i=0; i<WndCount; i++ )
	{
		WndPtr=GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		WndPtr->SetWndResultID(value);
		WndPtr->SetWndLogicResultID(value);
		//WndPtr->GetWndAlgParam().SetAlgResultID(value);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::CheckModelWndValid(const CAOIWnd *RefWndPtr)//確認檢測框指標有效-屬於此模組內
{
	if ( NULL == RefWndPtr ) { return false; }

	size_t       i=0;
	CAOIWnd     *WndPtr = NULL;
	const size_t WndCount = GetModelWndCount_Inline();	
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr_Inline(i);	
		if ( NULL == WndPtr ) { continue; }		
		if ( RefWndPtr == WndPtr ) 
		{	return true; }
	}
	return false;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::SetModelWndEnabledByWndGroupID(int WndGroupID, bool bEnabled)
{
	size_t       i=0;
	bool         bFirst=true;
	CAOIWnd     *WndPtr = NULL;	
	const size_t WndCount = GetModelWndCount_Inline();
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr_Inline(i);	
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndGroupID() != WndGroupID ) { continue; }
		WndPtr->SetWndEnabled(bEnabled);
		WndPtr->SetWndModified(true);
	}	
	SetModelNeedSaveFiles(true);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::SetModelWndAlgTypeByWndGroupID(int WndGroupID, ALG_TYPE AlgType)
{
	size_t       i=0;	
	bool         bFirst=true;	
	ALG_TYPE     AlgTypeBefore;
	CAOIWnd     *WndPtr = NULL;	
	const size_t WndCount = GetModelWndCount_Inline();		
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr_Inline(i);	
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndGroupID() != WndGroupID ) { continue; }
		AlgTypeBefore = WndPtr->GetWndAlgType();
		if ( AlgTypeBefore == AlgType ) { continue; }

		if ( true == bFirst )
		{
			bFirst = false;
			WndPtr->GetWndAlgParam().ChangeAlgType(AlgType, true);			
		}
		else
		{
			WndPtr->GetWndAlgParam().ChangeAlgType(AlgType, false);
			//WndPtr->GetWndAlgParam().CheckAlgPatternFileUsed();			
		}
		WndPtr->SetWndModified(true);		
		WndPtr->SetWndUIUpated_Param(false);
	}	
	SetModelModifiedCount(true);
	SetModelNeedSaveFiles(true);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::SetModelWndResultID(RESULT_ID ResultID)//設定模組檢測框結果編號-bypass
{
	size_t       i=0;
	bool         bFirst=true;
	CAOIWnd     *WndPtr = NULL;
	TPOINT2D     ExtendRangePt;	
	const size_t WndCount = GetModelWndCount_Inline();
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr_Inline(i);	
		if ( NULL == WndPtr ) { continue; }
		WndPtr->SetWndResultID(ResultID);
		WndPtr->GetWndAlgParam().SetAlgResultID(ResultID);
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::RemoveModelAlgPatternFolderBySelected()//移除模組演算法樣板資料夾
{
	int          j=0;	
	size_t       i=0;	
	int          AlgGroupID=0;
	int          PatternUseCount = 0;
	CAOIWnd     *WndPtr = NULL;	
	CString      FullFolder;
	CString      ModelFolder;
	CString      PatternFolder;
	std::vector<int>   ModelPaternIDList;
	const int    ModelAlgID = GetModelAlgFreeGroupID();
	const size_t ModelWndCount = GetModelWndCount_Inline();

	for ( j=0; j<ModelAlgID; j++ )
	{	ModelPaternIDList.push_back(0);	}	
	
	for ( i=0; i<ModelWndCount; i++ )
	{	
		WndPtr = GetModelWndPtr_Inline(i);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndSelected() == true ) { continue; }		
		AlgGroupID = WndPtr->GetWndAlgParam().GetAlgGroupID();
		if ( AlgGroupID<0 || AlgGroupID>=ModelAlgID ) { continue; }
		ModelPaternIDList[AlgGroupID] ++;
	}

	ModelFolder = GetModelFolder();
	for ( j=0; j<ModelAlgID; j++ )
	{
		PatternUseCount = ModelPaternIDList[j];
		if ( PatternUseCount > 0 ) { continue; }
		PatternFolder = AOIDataDefine.GetAlgPatternFolder(j);
		FullFolder.Format(_T("%s\\%s"), ModelFolder, PatternFolder);			
		JetAPI::RemoveFolder(FullFolder);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::SetModelWndShapeMode(CAOIWnd *RefWndPtr, BOX_SHAPE_MODE BoxShapeMode)//設定模組檢測框外形
{
	if ( NULL == RefWndPtr ) { return false; }
	if ( CheckModelWndValid(RefWndPtr) == false ) { return false; }

	size_t              i=0;	
	CAOIWnd            *WndPtr = NULL;		
	const int WndGroupID = RefWndPtr->GetWndGroupID();
	const size_t WndCount = GetModelWndCount_Inline();
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr_Inline(i);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndGroupID() != WndGroupID ) { continue; }		
		WndPtr->SetWndShapeMode(BoxShapeMode);
		WndPtr->SetWndModified(true);
	}	
	SetModelNeedSaveFiles(true);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::SetModelWndShapeParam(CAOIWnd *RefWndPtr, double BoxShapeParam)//設定模組檢測框外形參數-1
{
	if ( NULL == RefWndPtr ) { return false; }
	if ( CheckModelWndValid(RefWndPtr) == false ) { return false; }

	size_t              i=0;	
	CAOIWnd            *WndPtr = NULL;		
	const int WndGroupID = RefWndPtr->GetWndGroupID();
	const size_t WndCount = GetModelWndCount_Inline();
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr_Inline(i);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndGroupID() != WndGroupID ) { continue; }		
		WndPtr->SetWndShapeParam(BoxShapeParam);
		WndPtr->SetWndModified(true);
	}		
	SetModelNeedSaveFiles(true);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::SetModelWndShapeParam2(CAOIWnd *RefWndPtr, double BoxShapeParam2)//設定模組檢測框外形參數-2
{
	if ( NULL == RefWndPtr ) { return false; }
	if ( CheckModelWndValid(RefWndPtr) == false ) { return false; }

	size_t              i=0;	
	CAOIWnd            *WndPtr = NULL;		
	const int WndGroupID = RefWndPtr->GetWndGroupID();
	const size_t WndCount = GetModelWndCount_Inline();
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr_Inline(i);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndGroupID() != WndGroupID ) { continue; }		
		WndPtr->SetWndShapeParam2(BoxShapeParam2);
		WndPtr->SetWndModified(true);
	}		
	SetModelNeedSaveFiles(true);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::GetModelLandWndPtrByLandDefectID(const CAOILand *RefLandPtr, WND_DEFECT_ID WndDefectID, int DefectGroupID, CAOILand *&rLandPtr, CAOIWnd *&rWndPtr) const
{
	CAOIWnd  *WndPtr = NULL;
	CAOILand *LandPtr = NULL;
	rWndPtr = WndPtr;
	rLandPtr = LandPtr;
	if ( NULL == RefLandPtr ) { return false; }
	size_t       i=0;	
	LAND_TYPE    LandType = RefLandPtr->GetLandType();
	BOX_TOWARD   LandToward = RefLandPtr->GetLandToward();
	const int    LandGroupID = RefLandPtr->GetLandGroupID();
	const size_t WndCount = GetModelWndCount_Inline();
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = (GetModelWndPtr_Inline(i));
		if ( NULL == WndPtr ) { continue; }
		LandPtr = WndPtr->GetWndLandPtr();
		if ( NULL == LandPtr ) { continue; }		
		if ( LandPtr->GetLandType() != LandType ) { continue; }
		if ( LandPtr->GetLandToward() != LandToward ) { continue; }
		if ( LandPtr->GetLandGroupID() != LandGroupID ) { continue; }

		if ( WndPtr->GetWndDefectID() != WndDefectID ) { continue; }
		if ( -1 != DefectGroupID )
		{
			if ( WndPtr->GetWndDefectGroupID() != DefectGroupID ) { continue; }
		}
		rWndPtr = WndPtr;
		rLandPtr = LandPtr;		
		return true;
	}
	return false;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::ResetModelWndAlgParam()//復歸模組檢測框的演算法
{
	size_t       i=0;
	CAOIWnd     *WndPtr = NULL;	
	const size_t ModelWndCount = GetModelWndCount_Inline();

	for ( i=0; i<ModelWndCount; i++ )
	{
		WndPtr = GetModelWndPtr_Inline(i);
		if ( NULL == WndPtr ) { continue; }		
		WndPtr->GetWndAlgParam().SetAlgBinParamActived(BIN_PARAM_BELONG_TO_ALG_IMAGE);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
CAOIWnd* CAOIModel::GetModelWndSelectedByWndGroupID(int WndGroupID, bool NoIsolatedWnd) const
{
	size_t       i=0;
	CAOIWnd     *WndPtr = NULL;
	const size_t WndCount = GetModelWndCount_Inline();	
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr_Inline(i);	
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndGroupID() != WndGroupID ) { continue; }
		if ( true == NoIsolatedWnd )
		{
			if ( WndPtr->GetWndIsolated() == true ) 
			{ continue; }
		}
		if ( WndPtr->GetWndSelected() == true )
		{	return WndPtr; }
	}
	return NULL;
}
//-------------------------------------------------------------------------------------//
void CAOIModel::SetModelWndVisibledByWndGroupID(int WndGroupID, int WndBandID, bool Visibled)
{
	size_t       i=0;
	CAOIWnd     *WndPtr = NULL;
	const size_t WndCount = GetModelWndCount_Inline();
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = (GetModelWndPtr_Inline(i));
		if ( NULL == WndPtr ) { continue; }		
		if ( WndPtr->GetWndGroupID() != WndGroupID ) { continue; }
		if ( WndBandID >= 0 ) 
		{
			if ( WndPtr->GetWndBandID() != WndBandID ) { continue; }
		}
		WndPtr->SetWndVisibled(Visibled);
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CAOIModel::SetModelWndVisibledByWndUniqueID(int LandGroupID, int WndGroupID, int WndBandID, bool Visibled)
{
	size_t       i=0;
	CAOIWnd     *WndPtr = NULL;
	CAOILand    *LandPtr = NULL;
	const size_t WndCount = GetModelWndCount_Inline();
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = (GetModelWndPtr_Inline(i));
		if ( NULL == WndPtr ) { continue; }
		LandPtr = WndPtr->GetWndLandPtr();
		if ( LandPtr == NULL ) { continue; }
		if ( LandPtr->GetLandGroupID() != LandGroupID ) { continue; }
		if ( WndPtr->GetWndGroupID() != WndGroupID ) { continue; }
		if ( WndPtr->GetWndBandID() != WndBandID ) { continue; }
		WndPtr->SetWndVisibled(Visibled);
	}
	return;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::LayoutModelWndList()//重新排序檢測框列表-依據檢測框群組編號
{		
	size_t          i = 0;
	unsigned int    WndIndex = 0;	
	int             ValueInt = 0;
	int             WndBandID = 0;
	int             WndGroupID = 0;	
	CAOIWnd        *WndPtr = NULL;	
	CAOILand       *LandPtr = NULL;	
	const int       MaxWndBandID = GetModelWndFreeBandID();
	const int       MaxWndGroupID = GetModelWndFreeGroupID();
	std::vector<CAOIWnd*>  ModelWndList = m_ModelWndList;
	const size_t   ModelWndCount = ModelWndList.size();
	const size_t   ModelLandCount = GetModelLandCount();	
	CSortObj              SortNode, *SortNodePtr=NULL;
	std::vector<CSortObj> SortList;

	SortList.clear();
	SortNode.SetSortMode(SORT_BY_INT);	
	for ( i=0; i<ModelWndCount; i++ )
	{
		WndPtr = ModelWndList[i];
		if ( NULL == WndPtr ) { continue; }
		WndBandID = WndPtr->GetWndBandID();
		WndGroupID = WndPtr->GetWndGroupID();
		ValueInt = (WndGroupID*MaxWndBandID)+WndBandID;
		SortNode.SetID(i);
		SortNode.SetValueInt(ValueInt);
		SortNode.SetPtr(WndPtr);
		SortList.push_back(SortNode);
	}
	std::sort(SortList.begin(), SortList.end());
	const size_t SortNodeCount = SortList.size();

	WndIndex = 0;
	m_ModelWndList.clear();
	for ( i=0; i<SortNodeCount; i++ )
	{
		SortNodePtr = &(SortList[i]);
		WndGroupID = SortNodePtr->GetValueInt();
		WndPtr     = (CAOIWnd*)(SortNodePtr->GetPtr());
		WndPtr->SetWndIndex(WndIndex);
		m_ModelWndList.push_back(WndPtr);
		WndIndex ++;
	}	
	
	for ( i=0; i<ModelLandCount; i++ )
	{
		LandPtr = GetModelLandPtr(i, false);
		if ( NULL == LandPtr ) { continue; }		
		if ( LandPtr->LayoutLandWndList() == false )
		{	return false; }
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::LayoutModelWndListByDefectID()//重新排序檢測框列表
{
	size_t        i=0;
	int           WndOrder=0;
	WND_DEFECT_ID WndDefectID;
	CAOIWnd       *WndPtr = NULL;
	std::vector<CAOIWnd*> ModelWndList;
	const size_t WndCount = m_ModelWndList.size();
	CSortObj              SortObj;
	CSortObj             *SortObjPtr=NULL;
	std::vector<CSortObj> WndSortList(WndCount);
	
	WndSortList.clear();
	SortObj.SetSortMode(SORT_BY_INT);	
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = m_ModelWndList[i];
		if ( NULL == WndPtr ) { continue; }

		WndDefectID = WndPtr->GetWndDefectID();
		WndOrder = AOIDataDefine.GetWndDefectIDOrder(WndDefectID);
		WndOrder = WndOrder+(int)(i);
		SortObj.SetValueInt(WndOrder);
		SortObj.SetPtr(WndPtr);
		WndSortList.push_back(SortObj);		
	}

	std::sort(WndSortList.begin(), WndSortList.end());
	const size_t SortCount = WndSortList.size();
	for ( i=0; i<SortCount; i++ )	
	{
		SortObjPtr = &(WndSortList[i]);
		WndPtr = (CAOIWnd*)(SortObjPtr->GetPtr());		
		WndPtr->SetWndIndex((unsigned int)(i));
		ModelWndList.push_back(WndPtr);
	}
	m_ModelWndList = ModelWndList;
	SetModelModifiedCount(true);
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIModel::VisibleModelWnd()
{
	size_t       i=0;
	CAOIWnd     *WndPtr = NULL;
	const size_t WndCount = CAOIModel::GetModelWndCount_Inline();
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = (CAOIModel::GetModelWndPtr_Inline(i));
		if ( NULL == WndPtr ) { continue; }
		WndPtr->VisibleWnd();		
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CAOIModel::UnSelectModelWnd()
{
	size_t       i=0;
	CAOIWnd     *WndPtr = NULL;
	const size_t WndCount = GetModelWndCount_Inline();
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = (GetModelWndPtr_Inline(i));
		if ( NULL == WndPtr ) { continue; }
		WndPtr->UnSelectWnd();
		WndPtr->UnSelectWndRoi();
		WndPtr->UnSelectWndMaskBox();
		WndPtr->SetWndActived(false);
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CAOIModel::InvisibleModelWnd()
{
	size_t       i=0;
	CAOIWnd     *WndPtr = NULL;
	const size_t WndCount = CAOIModel::GetModelWndCount_Inline();
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = (CAOIModel::GetModelWndPtr_Inline(i));
		if ( NULL == WndPtr ) { continue; }
		WndPtr->InvisibleWnd();
		WndPtr->InvisibleWndRoi();
		WndPtr->InvisibleWndMaskBox();
	}
	return;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::CheckModelWndModified()//確認模組檢測框是否編輯過	
{
	size_t       i=0;
	CAOIWnd     *WndPtr = NULL;
	const size_t WndCount = CAOIModel::GetModelWndCount_Inline();
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = (CAOIModel::GetModelWndPtr_Inline(i));
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndModified() == false ) { continue; }
		return true;
	}
	return false;
}
//-------------------------------------------------------------------------------------//
void CAOIModel::SetModelWndModified(bool value)//設定模組檢測框是否編輯過
{
	size_t       i=0;
	CAOIWnd     *WndPtr = NULL;
	const size_t WndCount = CAOIModel::GetModelWndCount_Inline();
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = (CAOIModel::GetModelWndPtr_Inline(i));
		if ( NULL == WndPtr ) { continue; }
		WndPtr->SetWndModified(value);
	}
	SetModelNeedSaveFiles(true);
	return;
}
//-------------------------------------------------------------------------------------//
void CAOIModel::ApplyModelWnd(CAOIWnd *RefWndPtr)//套用檢測框至其它相同檢測框
{	
	if ( NULL == RefWndPtr ) { return; }
	if ( RefWndPtr->GetWndIsolated() == true ) { return ; }
	if ( CAOIModel::CheckModelWndValid(RefWndPtr) == false ) { return ; }

	size_t       i=0;
	CAOIWnd     *WndPtr = NULL;
	const int    RefWndGroupID = RefWndPtr->GetWndGroupID();
	const size_t WndCount = CAOIModel::GetModelWndCount_Inline();
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = (CAOIModel::GetModelWndPtr_Inline(i));		
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndIsolated() == true ) { continue; }
		if ( WndPtr->GetWndGroupID() != RefWndGroupID ) { continue; }		
		if ( WndPtr == RefWndPtr ) { continue; }	
		WndPtr->ApplyWnd(RefWndPtr);		
	}
	//SetModelModifiedCount(true);
	SetModelNeedSaveFiles(true);
	return ;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::ModifyModelOtherLandWndRegion(CAOIWnd *RefWndPtr)
{
	if ( RefWndPtr == NULL ) { return false; }	
	CAOILand    *RefLandPtr = RefWndPtr->GetWndLandPtr();
	if ( RefLandPtr== NULL ) { return false;	}

	bool IsOK = true;
	const double ComAngle = CAOIModel::GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComAngle);
	if ( IsExceptionAngle == false )
	{	IsOK = CAOIModel::ModifyModelOtherLandWndRegionKernel(RefWndPtr);	}
	else
	{
		CAOIModel::RotateModel(-ComAngle, 0, 0);
		IsOK = CAOIModel::ModifyModelOtherLandWndRegionKernel(RefWndPtr);
		CAOIModel::RotateModel(ComAngle, 0, 0);
	}
	return IsOK;
}
//--------------------------------------------------------------------------------------------//
bool CAOIModel::ModifyModelOtherLandWndRegionKernel(CAOIWnd *RefWndPtr)
{	
	if ( NULL == RefWndPtr ) { return false; }	
	CAOILand    *RefLandPtr = RefWndPtr->GetWndLandPtr();
	if ( NULL== RefLandPtr ) { return true;	}	

	size_t       i = 0;	
	int          Toward;
	int          TowardAngle=0;	
	CAOIWnd     *WndPtr = NULL;
	CAOILand    *LandPtr = NULL;
	double       SizeW=0,SizeH=0;	
	const BOX_TOWARD RefToward  = RefWndPtr->GetWndToward(); 
	const int        RefGroupID = RefWndPtr->GetWndGroupID();	
	const size_t WndCount = CAOIModel::GetModelWndCount_Inline();	
	double MinX=0, MinY=0, MaxX=0, MaxY=0;	
	double OffsetX=0, OffsetY=0;
	
	double PadCPX=0, PadCPY=0;
	double WndCPX=0, WndCPY=0;
	double WndSizeW=0, WndSizeH=0;
	double RefSizeW=0, RefSizeH=0;
	double RefWndCPX=0, RefWndCPY=0;
	double RefPadCPX=0, RefPadCPY=0;
	double PadMinX=0, PadMinY=0, PadMaxX=0, PadMaxY=0;
	double RefPadMinX=0, RefPadMinY=0, RefPadMaxX=0, RefPadMaxY=0;	
	
	RefWndPtr->GetWndRegion(MinX, MinY, MaxX, MaxY);	
	RefWndCPX = (MinX+MaxX)/2.0;
	RefWndCPY = (MinY+MaxY)/2.0;
	RefSizeW = MaxX-MinX;
	RefSizeH = MaxY-MinY;
	
	RefLandPtr->GetLandBoxPtr()->GetBoxRegion(RefPadMinX, RefPadMinY, RefPadMaxX, RefPadMaxY); 	

	RefPadCPX = (RefPadMinX+RefPadMaxX)/2.0;
	RefPadCPY = (RefPadMinY+RefPadMaxY)/2.0;
	
	OffsetX = RefWndCPX-RefPadCPX;
	OffsetY = RefWndCPY-RefPadCPY;
	
	for ( i=0; i<WndCount; i++ )
	{	
		WndPtr = CAOIModel::GetModelWndPtr_Inline(i);
		if ( WndPtr == NULL ) { continue; }
		if ( WndPtr->GetWndGroupID() != RefGroupID ) { continue; }		
		LandPtr = WndPtr->GetWndLandPtr();
		if ( LandPtr == NULL ) { continue; }
		
		Toward = WndPtr->GetWndToward();		
		//if ( Toward != RefToward ) { continue; }
		
		LandPtr->GetLandBoxPtr()->GetBoxRegion(PadMinX, PadMinY, PadMaxX, PadMaxY);
		PadCPX = (PadMinX+PadMaxX)/2.0;
		PadCPY = (PadMinY+PadMaxY)/2.0;

		switch ( RefToward )
		{
		case BOX_TOWARD_UP:
			switch ( Toward )
			{
			case BOX_TOWARD_UP:
				WndCPX = PadCPX+OffsetX;
				WndCPY = PadCPY+OffsetY;
				WndSizeW = RefSizeW;
				WndSizeH = RefSizeH;
				MinX = WndCPX-WndSizeW/2.0;	MinY = WndCPY-WndSizeH/2.0;
				MaxX = WndCPX+WndSizeW/2.0;	MaxY = WndCPY+WndSizeH/2.0;;	
				WndPtr->SetWndRegion(MinX, MinY, MaxX, MaxY);
				break;
			case BOX_TOWARD_LEFT:			
				WndCPX = PadCPX-OffsetY;
				WndCPY = PadCPY+OffsetX;
				WndSizeW = RefSizeH;
				WndSizeH = RefSizeW;
				MinX = WndCPX-WndSizeW/2.0;	MinY = WndCPY-WndSizeH/2.0;
				MaxX = WndCPX+WndSizeW/2.0;	MaxY = WndCPY+WndSizeH/2.0;;	
				WndPtr->SetWndRegion(MinX, MinY, MaxX, MaxY);
				break;
			case BOX_TOWARD_DOWN:
				WndCPX = PadCPX-OffsetX;
				WndCPY = PadCPY-OffsetY;
				WndSizeW = RefSizeW;
				WndSizeH = RefSizeH;
				MinX = WndCPX-WndSizeW/2.0;	MinY = WndCPY-WndSizeH/2.0;
				MaxX = WndCPX+WndSizeW/2.0;	MaxY = WndCPY+WndSizeH/2.0;;	
				WndPtr->SetWndRegion(MinX, MinY, MaxX, MaxY);
				break;
			case BOX_TOWARD_RIGHT:
				WndCPX = PadCPX+OffsetY;
				WndCPY = PadCPY-OffsetX;
				WndSizeW = RefSizeH;
				WndSizeH = RefSizeW;
				MinX = WndCPX-WndSizeW/2.0;	MinY = WndCPY-WndSizeH/2.0;
				MaxX = WndCPX+WndSizeW/2.0;	MaxY = WndCPY+WndSizeH/2.0;;	
				WndPtr->SetWndRegion(MinX, MinY, MaxX, MaxY);
				break;
			}
			break;
		case BOX_TOWARD_LEFT:
			switch ( Toward )
			{
			case BOX_TOWARD_UP:
				WndCPX = PadCPX+OffsetY;
				WndCPY = PadCPY-OffsetX;
				WndSizeW = RefSizeH;
				WndSizeH = RefSizeW;
				MinX = WndCPX-WndSizeW/2.0;	MinY = WndCPY-WndSizeH/2.0;
				MaxX = WndCPX+WndSizeW/2.0;	MaxY = WndCPY+WndSizeH/2.0;;	
				WndPtr->SetWndRegion(MinX, MinY, MaxX, MaxY);
				break;
			case BOX_TOWARD_LEFT:
				WndCPX = PadCPX+OffsetX;
				WndCPY = PadCPY+OffsetY;
				WndSizeW = RefSizeW;
				WndSizeH = RefSizeH;
				MinX = WndCPX-WndSizeW/2.0;	MinY = WndCPY-WndSizeH/2.0;
				MaxX = WndCPX+WndSizeW/2.0;	MaxY = WndCPY+WndSizeH/2.0;;	
				WndPtr->SetWndRegion(MinX, MinY, MaxX, MaxY);
				break;
			case BOX_TOWARD_DOWN:		
				WndCPX = PadCPX-OffsetY;
				WndCPY = PadCPY+OffsetX;
				WndSizeW = RefSizeH;
				WndSizeH = RefSizeW;
				MinX = WndCPX-WndSizeW/2.0;	MinY = WndCPY-WndSizeH/2.0;
				MaxX = WndCPX+WndSizeW/2.0;	MaxY = WndCPY+WndSizeH/2.0;;	
				WndPtr->SetWndRegion(MinX, MinY, MaxX, MaxY);
				break;
			case BOX_TOWARD_RIGHT:
				WndCPX = PadCPX-OffsetX;
				WndCPY = PadCPY-OffsetY;
				WndSizeW = RefSizeW;
				WndSizeH = RefSizeH;
				MinX = WndCPX-WndSizeW/2.0;	MinY = WndCPY-WndSizeH/2.0;
				MaxX = WndCPX+WndSizeW/2.0;	MaxY = WndCPY+WndSizeH/2.0;;	
				WndPtr->SetWndRegion(MinX, MinY, MaxX, MaxY);
				break;
			}
			break;
		case BOX_TOWARD_DOWN:
			switch ( Toward )
			{
			case BOX_TOWARD_UP:
				WndCPX = PadCPX-OffsetX;
				WndCPY = PadCPY-OffsetY;
				WndSizeW = RefSizeW;
				WndSizeH = RefSizeH;
				MinX = WndCPX-WndSizeW/2.0;	MinY = WndCPY-WndSizeH/2.0;
				MaxX = WndCPX+WndSizeW/2.0;	MaxY = WndCPY+WndSizeH/2.0;;	
				WndPtr->SetWndRegion(MinX, MinY, MaxX, MaxY);
				break;
			case BOX_TOWARD_LEFT:
				WndCPX = PadCPX+OffsetY;
				WndCPY = PadCPY-OffsetX;
				WndSizeW = RefSizeH;
				WndSizeH = RefSizeW;
				MinX = WndCPX-WndSizeW/2.0;	MinY = WndCPY-WndSizeH/2.0;
				MaxX = WndCPX+WndSizeW/2.0;	MaxY = WndCPY+WndSizeH/2.0;;	
				WndPtr->SetWndRegion(MinX, MinY, MaxX, MaxY);
				break;
			case BOX_TOWARD_DOWN:
				WndCPX = PadCPX+OffsetX;
				WndCPY = PadCPY+OffsetY;
				WndSizeW = RefSizeW;
				WndSizeH = RefSizeH;
				MinX = WndCPX-WndSizeW/2.0;	MinY = WndCPY-WndSizeH/2.0;
				MaxX = WndCPX+WndSizeW/2.0;	MaxY = WndCPY+WndSizeH/2.0;;	
				WndPtr->SetWndRegion(MinX, MinY, MaxX, MaxY);
				break;
			case BOX_TOWARD_RIGHT:
				WndCPX = PadCPX-OffsetY;
				WndCPY = PadCPY+OffsetX;
				WndSizeW = RefSizeH;
				WndSizeH = RefSizeW;
				MinX = WndCPX-WndSizeW/2.0;	MinY = WndCPY-WndSizeH/2.0;
				MaxX = WndCPX+WndSizeW/2.0;	MaxY = WndCPY+WndSizeH/2.0;;	
				WndPtr->SetWndRegion(MinX, MinY, MaxX, MaxY);
				break;
			}
			break;
		case BOX_TOWARD_RIGHT:
			switch ( Toward )
			{
			case BOX_TOWARD_UP:
				WndCPX = PadCPX-OffsetY;
				WndCPY = PadCPY+OffsetX;
				WndSizeW = RefSizeH;
				WndSizeH = RefSizeW;
				MinX = WndCPX-WndSizeW/2.0;	MinY = WndCPY-WndSizeH/2.0;
				MaxX = WndCPX+WndSizeW/2.0;	MaxY = WndCPY+WndSizeH/2.0;;	
				WndPtr->SetWndRegion(MinX, MinY, MaxX, MaxY);
				break;
			case BOX_TOWARD_LEFT:
				WndCPX = PadCPX-OffsetX;
				WndCPY = PadCPY-OffsetY;
				WndSizeW = RefSizeW;
				WndSizeH = RefSizeH;
				MinX = WndCPX-WndSizeW/2.0;	MinY = WndCPY-WndSizeH/2.0;
				MaxX = WndCPX+WndSizeW/2.0;	MaxY = WndCPY+WndSizeH/2.0;;	
				WndPtr->SetWndRegion(MinX, MinY, MaxX, MaxY);
				break;
			case BOX_TOWARD_DOWN:
				WndCPX = PadCPX+OffsetY;
				WndCPY = PadCPY-OffsetX;
				WndSizeW = RefSizeH;
				WndSizeH = RefSizeW;
				MinX = WndCPX-WndSizeW/2.0;	MinY = WndCPY-WndSizeH/2.0;
				MaxX = WndCPX+WndSizeW/2.0;	MaxY = WndCPY+WndSizeH/2.0;;	
				WndPtr->SetWndRegion(MinX, MinY, MaxX, MaxY);
				break;
			case BOX_TOWARD_RIGHT:
				WndCPX = PadCPX+OffsetX;
				WndCPY = PadCPY+OffsetY;
				WndSizeW = RefSizeW;
				WndSizeH = RefSizeH;
				MinX = WndCPX-WndSizeW/2.0;	MinY = WndCPY-WndSizeH/2.0;
				MaxX = WndCPX+WndSizeW/2.0;	MaxY = WndCPY+WndSizeH/2.0;;	
				WndPtr->SetWndRegion(MinX, MinY, MaxX, MaxY);
				break;
			}
			break;
		}
	}	
	SetModelModifiedCount(true);
	SetModelNeedSaveFiles(true);
	return true;
}
//--------------------------------------------------------------------------------------------//
bool CAOIModel::ModifyModelWndGroupIDUp()//上升選取到的檢測框群組
{
	std::vector<int> WndGroupIDList;
	if ( GetModelWndSelectedWndGroupIDList(WndGroupIDList) == false )
	{	return false; }

	const size_t WndGroupIDCount = WndGroupIDList.size();
	if ( WndGroupIDCount != 1 ) { return false; }

	const int NowWndGroupID = WndGroupIDList[0];
	if ( NowWndGroupID == 0 ) { return true; }
	
	size_t       i = 0;
	int          WndGroupID = 0;
	CAOIWnd     *WndPtr = NULL;
	const int    PreWndGroupID = NowWndGroupID-1;
	const size_t ModelWndCount = CAOIModel::GetModelWndCount_Inline();

	std::vector<CAOIWnd*>  PreWndPtrList;
	std::vector<CAOIWnd*>  NowWndPtrList;

	for ( i=0; i<ModelWndCount; i++ )
	{
		WndPtr = CAOIModel::GetModelWndPtr_Inline(i);
		if ( WndPtr == NULL ) { continue; }

		WndGroupID = WndPtr->GetWndGroupID();
		if ( WndGroupID == PreWndGroupID )
		{	PreWndPtrList.push_back(WndPtr);	}
		else if ( WndGroupID == NowWndGroupID )
		{	NowWndPtrList.push_back(WndPtr);	}
	}

	const size_t PreWndPtrCount = PreWndPtrList.size();
	const size_t NowWndPtrCount = NowWndPtrList.size();

	if ( NowWndPtrCount != NULL )
	{
		for ( i=0; i<NowWndPtrCount; i++ )
		{
			WndPtr = NowWndPtrList[i];
			if ( WndPtr == NULL ) { continue; }
			WndPtr->SetWndGroupID(PreWndGroupID);
		}
	}

	if ( PreWndPtrCount != 0 ) 
	{
		for ( i=0; i<PreWndPtrCount; i++ )
		{
			WndPtr = PreWndPtrList[i];
			if ( WndPtr == NULL ) { continue; }
			WndPtr->SetWndGroupID(NowWndGroupID);
		}

		//重新排序
		if ( LayoutModelWndList() == false )
		{	return false; }
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::ModifyModelWndGroupIDDownd()//下降選取到的檢測框群組
{
	std::vector<int> WndGroupIDList;
	if ( GetModelWndSelectedWndGroupIDList(WndGroupIDList) == false )
	{	return false; }

	const size_t WndGroupIDCount = WndGroupIDList.size();
	if ( WndGroupIDCount != 1 ) { return false; }

	const int MaxWndGroupID = GetModelWndFreeGroupID();
	const int NowWndGroupID = WndGroupIDList[0];
	if ( NowWndGroupID == (MaxWndGroupID-1) ) { return true; }
	
	size_t       i = 0;
	int          WndGroupID = 0;
	CAOIWnd     *WndPtr = NULL;
	const int    NextWndGroupID = NowWndGroupID+1;
	const size_t ModelWndCount = GetModelWndCount_Inline();

	std::vector<CAOIWnd*>  NextWndPtrList;
	std::vector<CAOIWnd*>  NowWndPtrList;

	for ( i=0; i<ModelWndCount; i++ )
	{
		WndPtr = GetModelWndPtr_Inline(i);
		if ( WndPtr == NULL ) { continue; }

		WndGroupID = WndPtr->GetWndGroupID();
		if ( WndGroupID == NextWndGroupID )
		{	NextWndPtrList.push_back(WndPtr);	}
		else if ( WndGroupID == NowWndGroupID )
		{	NowWndPtrList.push_back(WndPtr);	}
	}

	const size_t NextWndPtrCount = NextWndPtrList.size();
	const size_t NowWndPtrCount = NowWndPtrList.size();

	if ( NowWndPtrCount != NULL )
	{
		for ( i=0; i<NowWndPtrCount; i++ )
		{
			WndPtr = NowWndPtrList[i];
			if ( WndPtr == NULL ) { continue; }
			WndPtr->SetWndGroupID(NextWndGroupID);
		}
	}

	if ( NextWndPtrCount != 0 ) 
	{
		for ( i=0; i<NextWndPtrCount; i++ )
		{
			WndPtr = NextWndPtrList[i];
			if ( WndPtr == NULL ) { continue; }
			WndPtr->SetWndGroupID(NowWndGroupID);
		}

		//重新排序
		if ( LayoutModelWndList() == false )
		{	return false; }
	}
	return true;
}
//-------------------------------------------------------------------------------------//	
bool CAOIModel::ModifyModelWndBandID(const std::vector<unsigned int> &WndIndexList, int WndGroupID, int NewWndBandID)//修正檢測框群組
{
	size_t       i=0;
	unsigned int WndIndex=0;
	CAOIWnd     *WndPtr = NULL;
	const size_t WndIndexCount = WndIndexList.size();
	CAOIWnd     *RefWndPtr = GetModelWndPtrByGroupBandID(WndGroupID, NewWndBandID, false);

	if ( NULL == RefWndPtr )
	{
		for ( i=0; i<WndIndexCount; i++ )
		{
			WndIndex = WndIndexList[i];
			WndPtr = GetModelWndPtr(WndIndex, true);
			if ( NULL == WndPtr ) { continue; }
			WndPtr->SetWndBandID(NewWndBandID);
		}
		return true; 
	}

	CAOIWnd *RefWndPtrT=NULL;
	CAOIWnd *RefWndPtrL=NULL;
	CAOIWnd *RefWndPtrB=NULL;
	CAOIWnd *RefWndPtrR=NULL;
	BOX_TOWARD WndToward=BOX_TOWARD_NULL;
	const size_t ModelWndCount = GetModelWndCount_Inline();
	for ( i=0; i<ModelWndCount; i++ )
	{
		WndPtr = GetModelWndPtr_Inline(i);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndGroupID() != WndGroupID ) { continue; }
		if ( WndPtr->GetWndBandID() != NewWndBandID ) { continue; }
		WndToward = WndPtr->GetWndToward();
		switch ( WndToward )
		{
		case BOX_TOWARD_UP:
			if ( NULL == RefWndPtrT ) { RefWndPtrT = WndPtr; }
			break;
		case BOX_TOWARD_LEFT:
			if ( NULL == RefWndPtrL ) { RefWndPtrL = WndPtr; }
			break;
		case BOX_TOWARD_DOWN:
			if ( NULL == RefWndPtrB ) { RefWndPtrB = WndPtr; }
			break;
		case BOX_TOWARD_RIGHT:
			if ( NULL == RefWndPtrR ) { RefWndPtrR = WndPtr; }
			break;
		}
	}

	int    TowardAngle=0;
	CAOILand *LandPtr=NULL;
	CAOILand *RefLandPtr=NULL;
	double OffsetX=0, OffsetY=0;
	double WndCPX=0, WndCPY=0;
	double LandCPX=0, LandCPY=0;
	double RefWndCPX=0, RefWndCPY=0;
	double RefLandCPX=0, RefLandCPY=0;
	bool   GetTheSameTowardWnd=false;
	BOX_TOWARD RefWndToward=BOX_TOWARD_NULL;
	for ( i=0; i<WndIndexCount; i++ )
	{
		WndIndex = WndIndexList[i];
		WndPtr = GetModelWndPtr(WndIndex, true);
		if ( NULL == WndPtr ) { continue; }
		WndPtr->SetWndBandID(NewWndBandID);
		LandPtr = WndPtr->GetWndLandPtr();
		if ( NULL == LandPtr ) { continue; }

		RefWndPtr=NULL;		
		WndToward = WndPtr->GetWndToward();		
		switch ( WndToward )
		{
		case BOX_TOWARD_UP:		RefWndPtr = RefWndPtrT; break;
		case BOX_TOWARD_LEFT:	RefWndPtr = RefWndPtrL;	break;
		case BOX_TOWARD_DOWN:	RefWndPtr = RefWndPtrB; break;
		case BOX_TOWARD_RIGHT:	RefWndPtr = RefWndPtrR; break;
		}				
		if ( NULL == RefWndPtr )
		{
			//找其他方位的檢測			
			GetTheSameTowardWnd = false;
			if ( RefWndPtrT != NULL ) { RefWndPtr = RefWndPtrT; }
			if ( RefWndPtrL != NULL ) { RefWndPtr = RefWndPtrL; }
			if ( RefWndPtrB != NULL ) { RefWndPtr = RefWndPtrB; }
			if ( RefWndPtrR != NULL ) { RefWndPtr = RefWndPtrR; }
		}		
		if ( NULL == RefWndPtr ) { continue; }
		RefLandPtr = RefWndPtr->GetWndLandPtr();			
		if ( NULL == RefLandPtr ) { continue; }

		RefWndToward = RefWndPtr->GetWndToward();
		TowardAngle = CAOIBox::CalcBoxTowardAngle(RefWndToward, WndToward);
		WndPtr->GetWndBoxPtr()->GetBoxPos(WndCPX, WndCPY);
		RefWndPtr->GetWndBoxPtr()->GetBoxPos(RefWndCPX, RefWndCPY);				
		
		LandPtr->GetLandBoxPtr()->GetBoxPos(LandCPX, LandCPY);	
		RefLandPtr->GetLandBoxPtr()->GetBoxPos(RefLandCPX, RefLandCPY);		
		WndCPX = WndCPX-LandCPX;
		WndCPY = WndCPY-LandCPY;
		RefWndCPX = RefWndCPX-RefLandCPX;
		RefWndCPY = RefWndCPY-RefLandCPY;
		JetAPI::RotatePos(TowardAngle, 0, 0, RefWndCPX, RefWndCPY);			
		
		OffsetX = WndCPX-RefWndCPX;
		OffsetY = WndCPY-RefWndCPY;
		WndPtr->MoveWnd(-OffsetX, -OffsetY);		
	}

	return true;
}
//-------------------------------------------------------------------------------------//	
bool CAOIModel::ModifyModelWndGroupID(const std::vector<unsigned int> &WndIndexList, int NewWndGroupID, int NewBandID)//修正檢測框群組
{
	size_t       i=0;
	unsigned int WndIndex=0;
	CAOIWnd     *WndPtr = NULL;
	const size_t WndIndexCount = WndIndexList.size();

	for ( i=0; i<WndIndexCount; i++ )
	{
		WndIndex = WndIndexList[i];
		WndPtr = GetModelWndPtr(WndIndex, true);
		if ( NULL == WndPtr ) { continue; }
		WndPtr->SetWndGroupID(NewWndGroupID);
		if ( NewBandID >= 0 ) 
		{	WndPtr->SetWndBandID(NewBandID);	}
	}
	return true;
}
//-------------------------------------------------------------------------------------//	
bool CAOIModel::DeleteModelWndSelected(bool LinkMode)
{	
	size_t       i=0, j=0;	
	unsigned int WndIndex=0;
	int          WndBandID = 0;
	int          WndGroupID = 0;	
	CAOIWnd     *WndPtr = NULL;
	CAOILand    *LandPtr = NULL;
	CAOILand    *RefLandPtr = NULL;
	std::vector<unsigned int> WndIndexList;
	const size_t WndCount = GetModelWndCount_Inline();		
	const size_t LandCount = GetModelLandCount();		
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr_Inline(i);		
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndSelected() == false )
		{	continue; }

		WndIndexList.push_back(i);		
		WndBandID = WndPtr->GetWndBandID();
		WndGroupID = WndPtr->GetWndGroupID();
		RefLandPtr = WndPtr->GetWndLandPtr();
		if ( NULL!=RefLandPtr && true==LinkMode)
		{	
			for ( j=0; j<LandCount; j++ )
			{
				LandPtr = GetModelLandPtr(j, false);				
				if ( NULL == LandPtr ) { continue; }
				if ( LandPtr == RefLandPtr ) { continue; }
				if ( LandPtr->GetLandGroupID() != RefLandPtr->GetLandGroupID() ) { continue; }
				WndPtr = LandPtr->GetLandWndPtrByGroupID(WndGroupID, WndBandID);
				if ( NULL == WndPtr ) { continue; }				
				WndIndex = WndPtr->GetWndIndex();
				WndIndexList.push_back(WndIndex);
			}
		}		
	}	
	if ( WndIndexList.size() == 0 ) { return true; }
	DeleteModelWndIndexList(WndIndexList);	
	CalcModelTotalRegionAll();
	UpdateModelRegionToAttached();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::DeleteModelWndWithClassID(int ClassID)//刪除模組內的檢測框(類別編號)
{	
	size_t       i=0;	
	bool         bSelected=false;
	CAOIWnd     *WndPtr = NULL;	
	const size_t WndCount = GetModelWndCount_Inline();			
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr_Inline(i);		
		if ( NULL == WndPtr ) { continue; }
		WndPtr->SetWndSelected(false);
		if ( WndPtr->GetWndClassID() != ClassID ) { continue; }
		bSelected = true;
		WndPtr->SetWndSelected(true);
	}	
	if ( false == bSelected ) 
	{	return true; }

	if ( DeleteModelWndSelected(false) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::CloneModelWndSelected(bool LinkMode)
{
	bool IsOK = true;	
	const double ComAngle = GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComAngle);
	if ( IsExceptionAngle == false )
	{	IsOK = CloneModelWndSelectedKernel(LinkMode);	}
	else
	{
		RotateModel(-ComAngle, 0, 0);
		IsOK = CloneModelWndSelectedKernel(LinkMode);
		RotateModel(ComAngle, 0, 0);
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
inline bool CAOIModel::CloneModelWndSelectedKernel(bool LinkMode)
{
	size_t    i=0, j=0;	
	int       WndBandID = 0;
	int       WndGroupID = 0;
	int       LandGroupID = 0;
	size_t    WndPtrCount = 0;
	CAOIWnd  *WndPtr = NULL;
	CAOIWnd  *WndPtr2= NULL;
	CAOILand *LandPtr = NULL;
	CAOILand *LandPtr2 = NULL;
	CAOIWnd  *NewWndPtr = NULL;	
	CAOIWnd  *LastWndPtr = NULL;		
	std::vector<CAOIWnd*>   WndPtrList;
	const size_t WndCount = GetModelWndCount_Inline();		
	const size_t LandCount = GetModelLandCount();
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr_Inline(i);
		if ( NULL== WndPtr ) { continue; }		
		if ( WndPtr->GetWndSelected() == false )
		{	continue; }

		LandPtr = WndPtr->GetWndLandPtr();		
		if ( NULL != LandPtr ) 
		{			
			LandGroupID = LandPtr->GetLandGroupID();
			WndPtrCount = WndPtrList.size();
			for ( j=0; j<WndPtrCount; j++ )
			{
				WndPtr2 = WndPtrList[j];
				if ( NULL == WndPtr2 ) { continue; }				
				if ( WndPtr2->GetWndLandPtr()->GetLandGroupID() != LandGroupID )
				{	continue; }
				if ( WndPtr2->GetWndGroupID() != WndPtr->GetWndGroupID() )
				{	continue;	}
				if ( WndPtr2->GetWndBandID() != WndPtr->GetWndBandID() )
				{	continue;	}
				break; 
			}
			if ( j == WndPtrCount )
			{	WndPtrList.push_back(WndPtr);	}			
			continue;
		}	

		//Add Body Wnd
		NewWndPtr = CopyModelWnd(WndPtr);		
		if ( NULL == NewWndPtr ) { return false; }
		LastWndPtr = NewWndPtr;		
		WndPtr->SetWndSelected(false);
	}	

	//Add Land Wnd
	WndPtrCount = WndPtrList.size();
	for ( i=0; i<WndPtrCount; i++ )
	{
		WndPtr = WndPtrList[i];
		if ( NULL == WndPtr ) { continue; }		
		WndGroupID = WndPtr->GetWndGroupID();
		WndBandID = GetModelWndFreeBandID(WndGroupID);
		LandPtr = WndPtr->GetWndLandPtr();
		if ( true == LinkMode )
		{	
			AddModelWndToOtherLand(LandPtr, WndPtr, WndBandID); 
			NewWndPtr = LandPtr->GetLandWndPtrLastOne();
		}
		else
		{	NewWndPtr = CopyModelWnd(WndPtr); }
		LastWndPtr = NewWndPtr;
		WndPtr->SetWndSelected(false);
	}
	SetModelWndActived(LastWndPtr);
	SetModelModifiedCount(true);
	SetModelNeedSaveFiles(true);
	return true;
}
//-------------------------------------------------------------------------------------//
bool  CAOIModel::CloneModelWndWithClassID(int ClassID)//複製模組內的檢測框(類別編號)
{
	size_t    i=0;
	int       WndClassID=0;	
	int       WndGroupID=0;
	int       MaxGroupID=0;
	bool      bSelected=false;
	CAOIWnd  *WndPtr = NULL;	
	std::map<int, int> GroupMap;
	std::vector<int> ClassIDUsed(MODEL_CLASS_ID_COUNT);
	const size_t ClassIDCount = ClassIDUsed.size();
	const size_t WndCount = GetModelWndCount_Inline();
	for ( i=0; i<ClassIDCount; i++ )
	{	ClassIDUsed[i] = 0; }
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr_Inline(i);
		if ( NULL== WndPtr ) { continue; }
		WndPtr->SetWndSelected(false);
		WndGroupID = WndPtr->GetWndGroupID();
		WndClassID = WndPtr->GetWndClassID();
		if ( WndClassID < ClassIDCount ) 
		{	ClassIDUsed[WndClassID] ++; }
		GroupMap.insert(std::pair<int, int>(WndGroupID, WndGroupID));
		if ( MaxGroupID < WndGroupID ) { MaxGroupID = WndGroupID; }
		if ( WndClassID != ClassID ) { continue; }
		bSelected = true;
		WndPtr->SetWndSelected(true);
	}
	if ( false == bSelected ) 
	{	return true; }

	int NewClassID = 0;	
	for ( i=1; i<ClassIDCount; i++ )
	{
		if ( 0 != ClassIDUsed[i] ) { continue; }
		NewClassID = (int)(i);
		break;
	}
	if ( 0 == NewClassID )
	{	return false; }
	if ( CloneModelWndSelected(true) == false )
	{	return false; }
	const size_t WndCountNew = GetModelWndCount_Inline();

	int AlgGroupID = 0;
	int PatternFolderIndex=0;
	CString PatternFolder;	
	CString SrcPatternFolder;
	CString AlgPatternFolder;
	CString ModelFolder = GetModelFolder();
	const bool bClearDst = false;
	const bool bDeleteSrc = false;	

	int NewGroupID = MaxGroupID+1;
	for ( auto Iter=GroupMap.begin(); Iter!=GroupMap.end(); ++Iter )
	{
		Iter->second = NewGroupID;
		NewGroupID ++;
	}
	for ( i=WndCount; i<WndCountNew; i++ )
	{
		WndPtr = GetModelWndPtr_Inline(i);
		if ( NULL== WndPtr ) { continue; }
		WndGroupID = WndPtr->GetWndGroupID();
		auto Iter = GroupMap.find(WndGroupID);
		if ( GroupMap.end() != Iter )
		{	WndGroupID = Iter->second;	}
		WndPtr->SetWndGroupID(WndGroupID);
		WndPtr->SetWndClassID(NewClassID);		

		CAlgParam &AlgParam=WndPtr->GetWndAlgParam();
		if ( AlgParam.GetAlgPatternFileUsed() == true )
		{
			AlgGroupID = AlgParam.GetAlgGroupID();
			PatternFolderIndex = AlgGroupID;	
			SrcPatternFolder = WndPtr->GetWndAlgParam().GetAlgPatternFolder();
			PatternFolder = AOIDataDefine.GetAlgPatternFolder(PatternFolderIndex);
			AlgPatternFolder.Format(_T("%s\\%s"), ModelFolder, PatternFolder);
			AlgParam.SetAlgPatternFolder(AlgPatternFolder);
			JetAPI::CreateFolder(AlgPatternFolder);
			JetAPI::CopyFolderAToFolderB(SrcPatternFolder, AlgPatternFolder, bDeleteSrc, bClearDst, _T(""), -1, -1);
		}
	}

	WndPtr = GetModelWndPtr(WndCount, true);
	if ( NULL != WndPtr )
	{	SetModelWndActived(WndPtr);	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AlignCenterModelWndSelected()//對齊模組檢測框
{
	bool IsOK = true;	
	const double ComAngle = GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComAngle);
	if ( IsExceptionAngle == false )
	{	IsOK = AlignCenterModelWndSelectedKernel();	}
	else
	{		
		RotateModel(-ComAngle, 0, 0);
		IsOK = AlignCenterModelWndSelectedKernel();
		RotateModel(ComAngle, 0, 0);
	}
	CalcModelTotalRegionAll();
	UpdateModelRegionToAttached();
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AlignCenterModelWndSelectedKernel()//對齊模組檢測框
{
	size_t     i=0, j=0;
	double     dX=0;
	double     dY=0;
	TPOINT2D   WndPos;
	TPOINT2D   LandPos;
	TPOINT2D   BodyPos;	
	int        WndBandID=0;
	int        WndTempInt=0;
	int        WndGroupID=0;
	int        RefWndBandID=0;
	int        RefWndGroupID=0;		
	CAOIWnd   *WndPtr = NULL;
	CAOILand  *LandPtr = NULL;	
	CAOIWnd   *RefWndPtr = NULL;
	BOX_TOWARD WndToward = BOX_TOWARD_NULL;	

	GetModelBodyBox().GetBoxPos(BodyPos);
	const size_t WndCount = GetModelWndCount_Inline();		
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr_Inline(i);
		if ( NULL== WndPtr ) { continue; }		
		WndPtr->SetWndTempInt(FN_DISABLE);				
	}
	
	for ( j=0; j<WndCount; j++ )
	{
		RefWndPtr = GetModelWndPtr_Inline(j);
		if ( NULL== RefWndPtr ) { continue; }				
		if ( RefWndPtr->GetWndSelected() == false )
		{	continue; }
		WndTempInt = RefWndPtr->GetWndTempInt();
		if ( FN_ENABLE == WndTempInt )  { continue; }
		RefWndBandID = RefWndPtr->GetWndBandID();
		RefWndGroupID = RefWndPtr->GetWndGroupID();

		for ( i=0; i<WndCount; i++ )
		{
			WndPtr = GetModelWndPtr_Inline(i);
			if ( NULL== WndPtr ) { continue; }	
			WndBandID = WndPtr->GetWndBandID();
			WndGroupID = WndPtr->GetWndGroupID();
			if ( WndBandID != RefWndBandID ) { continue; }
			if ( WndGroupID != RefWndGroupID ) { continue; }

			LandPtr = WndPtr->GetWndLandPtr();
			WndToward = WndPtr->GetWndToward();			
			WndPtr->GetWndBox().GetBoxPos(WndPos);
			if ( LandPtr == NULL ) 
			{	
				dX = BodyPos.x-WndPos.x;
				dY = BodyPos.y-WndPos.y;
			}
			else
			{
				LandPtr->GetLandBoxPtr()->GetBoxPos(LandPos);			
				dX = LandPos.x-WndPos.x;
				dY = LandPos.y-WndPos.y;
			}
			
			WndPtr->MoveWnd(dX, dY);			
			WndPtr->SetWndTempInt(FN_ENABLE);
		}
	}
	SetModelModifiedCount(true);	
	SetModelNeedSaveFiles(true);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AlignCenterUModelWndSelected()//對齊模組檢測框-U
{
	bool IsOK = true;	
	const double ComAngle = GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComAngle);
	if ( IsExceptionAngle == false )
	{	IsOK = AlignCenterUModelWndSelectedKernel();	}
	else
	{		
		RotateModel(-ComAngle, 0, 0);
		IsOK = AlignCenterUModelWndSelectedKernel();
		RotateModel(ComAngle, 0, 0);
	}
	CalcModelTotalRegionAll();
	UpdateModelRegionToAttached();
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AlignCenterUModelWndSelectedKernel()//對齊模組檢測框-U
{
	size_t     i=0, j=0;
	double     dX=0;
	double     dY=0;
	TPOINT2D   WndPos;
	TPOINT2D   LandPos;
	TPOINT2D   BodyPos;	
	int        WndBandID=0;
	int        WndTempInt=0;
	int        WndGroupID=0;
	int        RefWndBandID=0;
	int        RefWndGroupID=0;		
	CAOIWnd   *WndPtr = NULL;
	CAOILand  *LandPtr = NULL;	
	CAOIWnd   *RefWndPtr = NULL;
	BOX_TOWARD WndToward = BOX_TOWARD_NULL;	

	GetModelBodyBox().GetBoxPos(BodyPos);
	const size_t WndCount = GetModelWndCount_Inline();		
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr_Inline(i);
		if ( NULL== WndPtr ) { continue; }		
		WndPtr->SetWndTempInt(FN_DISABLE);				
	}
	
	for ( j=0; j<WndCount; j++ )
	{
		RefWndPtr = GetModelWndPtr_Inline(j);
		if ( NULL== RefWndPtr ) { continue; }				
		if ( RefWndPtr->GetWndSelected() == false )
		{	continue; }
		WndTempInt = RefWndPtr->GetWndTempInt();
		if ( FN_ENABLE == WndTempInt )  { continue; }
		RefWndBandID = RefWndPtr->GetWndBandID();
		RefWndGroupID = RefWndPtr->GetWndGroupID();

		for ( i=0; i<WndCount; i++ )
		{
			WndPtr = GetModelWndPtr_Inline(i);
			if ( NULL== WndPtr ) { continue; }	
			WndBandID = WndPtr->GetWndBandID();
			WndGroupID = WndPtr->GetWndGroupID();
			if ( WndBandID != RefWndBandID ) { continue; }
			if ( WndGroupID != RefWndGroupID ) { continue; }

			LandPtr = WndPtr->GetWndLandPtr();
			WndToward = WndPtr->GetWndToward();			
			WndPtr->GetWndBox().GetBoxPos(WndPos);
			if ( LandPtr == NULL ) 
			{	
				dX = BodyPos.x-WndPos.x;
				dY = BodyPos.y-WndPos.y;
			}
			else
			{
				LandPtr->GetLandBoxPtr()->GetBoxPos(LandPos);			
				dX = LandPos.x-WndPos.x;
				dY = LandPos.y-WndPos.y;
			}
			switch ( WndToward )
			{
			case BOX_TOWARD_UP:
			case BOX_TOWARD_DOWN:
				WndPtr->MoveWnd(dX, 0);
				break;
			case BOX_TOWARD_LEFT:			
			case BOX_TOWARD_RIGHT:
				WndPtr->MoveWnd(0, dY);
				break;
			}
			WndPtr->SetWndTempInt(FN_ENABLE);
		}
	}
	SetModelModifiedCount(true);
	SetModelNeedSaveFiles(true);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AlignCenterVModelWndSelected()//對齊模組檢測框-V
{
	bool IsOK = true;	
	const double ComAngle = GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComAngle);
	if ( IsExceptionAngle == false )
	{	IsOK = AlignCenterVModelWndSelectedKernel();	}
	else
	{		
		RotateModel(-ComAngle, 0, 0);
		IsOK = AlignCenterVModelWndSelectedKernel();
		RotateModel(ComAngle, 0, 0);
	}
	CalcModelTotalRegionAll();
	UpdateModelRegionToAttached();
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AlignCenterVModelWndSelectedKernel()//對齊模組檢測框-V
{
	size_t     i=0, j=0;
	double     dX=0;
	double     dY=0;
	TPOINT2D   WndPos;
	TPOINT2D   LandPos;
	TPOINT2D   BodyPos;	
	int        WndBandID=0;
	int        WndTempInt=0;
	int        WndGroupID=0;
	int        RefWndBandID=0;
	int        RefWndGroupID=0;		
	CAOIWnd   *WndPtr = NULL;
	CAOILand  *LandPtr = NULL;	
	CAOIWnd   *RefWndPtr = NULL;
	BOX_TOWARD WndToward = BOX_TOWARD_NULL;	

	GetModelBodyBox().GetBoxPos(BodyPos);
	const size_t WndCount = GetModelWndCount_Inline();		
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr_Inline(i);
		if ( NULL== WndPtr ) { continue; }		
		WndPtr->SetWndTempInt(FN_DISABLE);				
	}
	
	for ( j=0; j<WndCount; j++ )
	{
		RefWndPtr = GetModelWndPtr_Inline(j);
		if ( NULL== RefWndPtr ) { continue; }				
		if ( RefWndPtr->GetWndSelected() == false )
		{	continue; }
		WndTempInt = RefWndPtr->GetWndTempInt();
		if ( FN_ENABLE == WndTempInt )  { continue; }
		RefWndBandID = RefWndPtr->GetWndBandID();
		RefWndGroupID = RefWndPtr->GetWndGroupID();

		for ( i=0; i<WndCount; i++ )
		{
			WndPtr = GetModelWndPtr_Inline(i);
			if ( NULL== WndPtr ) { continue; }	
			WndBandID = WndPtr->GetWndBandID();
			WndGroupID = WndPtr->GetWndGroupID();
			if ( WndBandID != RefWndBandID ) { continue; }
			if ( WndGroupID != RefWndGroupID ) { continue; }

			LandPtr = WndPtr->GetWndLandPtr();
			WndToward = WndPtr->GetWndToward();			
			WndPtr->GetWndBox().GetBoxPos(WndPos);
			if ( LandPtr == NULL ) 
			{	
				dX = BodyPos.x-WndPos.x;
				dY = BodyPos.y-WndPos.y;
			}
			else
			{
				LandPtr->GetLandBoxPtr()->GetBoxPos(LandPos);			
				dX = LandPos.x-WndPos.x;
				dY = LandPos.y-WndPos.y;
			}
			switch ( WndToward )
			{
			case BOX_TOWARD_UP:
			case BOX_TOWARD_DOWN:
				WndPtr->MoveWnd(0, dY);
				break;
			case BOX_TOWARD_LEFT:			
			case BOX_TOWARD_RIGHT:
				WndPtr->MoveWnd(dX, 0);				
				break;
			}
			WndPtr->SetWndTempInt(FN_ENABLE);
		}
	}
	SetModelModifiedCount(true);	
	SetModelNeedSaveFiles(true);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::MoveModelWndSelected(BOX_TOWARD RefToward, double dX, double dY)
{
	bool IsOK = true;	
	const double ComAngle = GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComAngle);
	if ( IsExceptionAngle == false )
	{	IsOK = MoveModelWndSelectedKernel(RefToward, dX, dY);	}
	else
	{		
		RotateModel(-ComAngle, 0, 0);
		IsOK = MoveModelWndSelectedKernel(RefToward, dX, dY);
		RotateModel(ComAngle, 0, 0);
	}
	CalcModelTotalRegionAll();
	UpdateModelRegionToAttached();
	return IsOK;
}
//-------------------------------------------------------------------------------------//
inline bool CAOIModel::MoveModelWndSelectedKernel(BOX_TOWARD RefToward, double dX, double dY)
{
	size_t     i=0;
	int        TowardAngle = 0;
	BOX_TOWARD WndToward = BOX_TOWARD_NULL;	
	CAOIWnd   *WndPtr = NULL;
	CAOILand  *LandPtr = NULL;	
	const size_t WndCount = GetModelWndCount_Inline();		
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr_Inline(i);
		if ( NULL== WndPtr ) { continue; }		
		if ( WndPtr->GetWndSelected() == false )
		{	continue; }
		LandPtr = WndPtr->GetWndLandPtr();
		if ( LandPtr == NULL ) 
		{	WndPtr->MoveWnd(dX, dY);	}
		else
		{
			WndToward = WndPtr->GetWndToward();
			TowardAngle = CAOIBox::CalcBoxTowardAngle(RefToward, WndToward);
			switch ( TowardAngle )
			{
			case 90:
				WndPtr->MoveWnd(-dY, dX);
				break;
			case 180:
				WndPtr->MoveWnd(-dX, -dY);
				break;
			case 270:
				WndPtr->MoveWnd(dY, -dX);
				break;
			default:
				WndPtr->MoveWnd(dX, dY);
				break;
			}			
		}		
	}	
	SetModelModifiedCount(true);
	SetModelNeedSaveFiles(true);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::MirrorXModelWndSelected(double CPX, double CPY)
{
	bool IsOK = true;	
	const double ComAngle = GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComAngle);
	if ( IsExceptionAngle == false )
	{	IsOK = MirrorXModelWndSelectedKernel(CPY);	}
	else
	{
		JetAPI::RotatePos(-ComAngle, 0, 0, CPX, CPY);
		RotateModel(-ComAngle, 0, 0);
		IsOK = MirrorXModelWndSelectedKernel(CPY);
		RotateModel(ComAngle, 0, 0);
	}
	CalcModelTotalRegionAll();
	UpdateModelRegionToAttached();
	return IsOK;
}
//-------------------------------------------------------------------------------------//
inline bool CAOIModel::MirrorXModelWndSelectedKernel(double CPY)
{
	size_t     i=0;
	BOX_TOWARD LandToward = BOX_TOWARD_NULL;
	double     LandCPX=0, LandCPY=0;
	CAOIBox   *BoxPtr = NULL;
	CAOIWnd   *WndPtr = NULL;
	CAOILand  *LandPtr = NULL;	
	const size_t WndCount = GetModelWndCount_Inline();		
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr_Inline(i);
		if ( NULL== WndPtr ) { continue; }		
		if ( WndPtr->GetWndSelected() == false )
		{	continue; }
		LandPtr = WndPtr->GetWndLandPtr();
		if ( LandPtr == NULL ) 
		{	WndPtr->MirrorWndXAxis(CPY);	}
		else
		{
			LandToward = LandPtr->GetLandToward();
			BoxPtr = LandPtr->GetLandBoxPtr();			
			BoxPtr->GetBoxPos(LandCPX, LandCPY);
			switch ( LandToward )
			{
			case BOX_TOWARD_UP:
				WndPtr->MirrorWndYAxis(LandCPX+CPY);
				break;
			case BOX_TOWARD_LEFT:
				WndPtr->MirrorWndXAxis(LandCPY+CPY);
				break;
			case BOX_TOWARD_DOWN:
				WndPtr->MirrorWndYAxis(LandCPX+CPY);
				break;
			case BOX_TOWARD_RIGHT:
				WndPtr->MirrorWndXAxis(LandCPY+CPY);
				break;
			}			
		}		
	}	
	SetModelModifiedCount(true);
	SetModelNeedSaveFiles(true);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::MirrorYModelWndSelected(double CPX, double CPY)
{
	bool IsOK = true;	
	const double ComAngle = GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComAngle);
	if ( IsExceptionAngle == false )
	{	IsOK = MirrorYModelWndSelectedKernel(CPX);	}
	else
	{
		JetAPI::RotatePos(-ComAngle, 0, 0, CPX, CPY);
		RotateModel(-ComAngle, 0, 0);
		IsOK = MirrorYModelWndSelectedKernel(CPX);
		RotateModel(ComAngle, 0, 0);
	}
	CalcModelTotalRegionAll();
	UpdateModelRegionToAttached();
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::MirrorYModelWndSelectedKernel(double CPX)
{
	size_t i=0;
	BOX_TOWARD LandToward = BOX_TOWARD_NULL;
	double     LandCPX=0, LandCPY=0;	
	CAOIBox   *BoxPtr = NULL;
	CAOIWnd   *WndPtr = NULL;
	CAOILand  *LandPtr = NULL;	
	const size_t WndCount = GetModelWndCount_Inline();		
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr_Inline(i);
		if ( NULL== WndPtr ) { continue; }		
		if ( WndPtr->GetWndSelected() == false )
		{	continue; }
		LandPtr = WndPtr->GetWndLandPtr();
		if ( LandPtr == NULL ) 		
		{	WndPtr->MirrorWndYAxis(CPX); }
		else
		{		
			LandToward = LandPtr->GetLandToward();
			BoxPtr = LandPtr->GetLandBoxPtr();			
			BoxPtr->GetBoxPos(LandCPX, LandCPY);

			switch ( LandToward )
			{
			case BOX_TOWARD_UP:
				WndPtr->MirrorWndYAxis(LandCPX+CPX);		
				break;
			case BOX_TOWARD_LEFT:
				WndPtr->MirrorWndXAxis(LandCPY+CPX);		
				break;
			case BOX_TOWARD_DOWN:
				WndPtr->MirrorWndYAxis(LandCPX+CPX);		
				break;			
			case BOX_TOWARD_RIGHT:
				WndPtr->MirrorWndXAxis(LandCPY+CPX);		
				break;
			}			
		}
	}	
	SetModelModifiedCount(true);
	SetModelNeedSaveFiles(true);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::RotateModelWndSelected(double Angle, double CPX, double CPY)
{
	bool IsOK = true;	
	const double ComAngle = GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComAngle);
	if ( IsExceptionAngle == false )
	{	IsOK = RotateModelWndSelectedKernel(Angle, CPX, CPY);	}
	else
	{
		JetAPI::RotatePos(-ComAngle, 0, 0, CPX, CPY);
		RotateModel(-ComAngle, 0, 0);
		IsOK = RotateModelWndSelectedKernel(Angle, CPX, CPY);
		RotateModel(ComAngle, 0, 0);
	}
	CalcModelTotalRegionAll();
	UpdateModelRegionToAttached();
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::RotateModelWndSelectedKernel(double Angle, double CPX, double CPY)
{
	size_t    i=0;
	double    LandCPX=0, LandCPY=0;
	CAOIBox  *BoxPtr = NULL;
	CAOIWnd  *WndPtr = NULL;
	CAOILand *LandPtr = NULL;	
	const size_t WndCount = GetModelWndCount_Inline();		
	const double AttachedAngle = GetModelAttachedAngle();
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr_Inline(i);
		if ( NULL== WndPtr ) { continue; }
		if ( WndPtr->GetWndSelected() == false )
		{	continue; }
		LandPtr = WndPtr->GetWndLandPtr();		
		if ( NULL == LandPtr ) 
		{	WndPtr->RotateWnd(Angle, CPX, CPY);	}
		else
		{
			BoxPtr = LandPtr->GetLandBoxPtr();
			BoxPtr->GetBoxPos(LandCPX, LandCPY);
			WndPtr->RotateWnd(Angle, LandCPX+CPX, LandCPY+CPY);
		}		
		WndPtr->SetWndAttachedAngle(AttachedAngle);
	}			
	SetModelModifiedCount(true);
	SetModelNeedSaveFiles(true);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::SpinModelWndSelected(double Angle)
{	
	bool IsOK = true;	
	const double ComAngle = GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComAngle);
	if ( IsExceptionAngle == false )
	{	IsOK = SpinModelWndSelectedKernel(Angle);	}
	else
	{	
		RotateModel(-ComAngle, 0, 0);
		IsOK = SpinModelWndSelectedKernel(Angle);
		RotateModel(ComAngle, 0, 0);
	}
	CalcModelTotalRegionAll();
	UpdateModelRegionToAttached();
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::SpinModelWndSelectedKernel(double Angle)
{
	size_t    i=0, j=0;	
	size_t    WndPtrCount = 0;
	int       WndBandID = 0;
	int       WndGroupID = 0;
	int       LandGroupID = 0;
	double    CPX=0, CPY=0;
	CAOIWnd  *WndPtr = NULL;		
	CAOIWnd  *WndPtr2= NULL;		
	CAOILand *LandPtr = NULL;
	std::vector<CAOIWnd*>   WndPtrList;
	const double AttachedAngle = GetModelAttachedAngle();
	const size_t WndCount = GetModelWndCount_Inline();		
	const size_t LandCount = GetModelLandCount();		
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr_Inline(i);
		if ( NULL== WndPtr ) { continue; }		
		if ( WndPtr->GetWndSelected() == false )
		{	continue; }		
		
		LandPtr = WndPtr->GetWndLandPtr();
		if ( LandPtr != NULL )
		{
			WndBandID = WndPtr->GetWndBandID();
			WndGroupID = WndPtr->GetWndGroupID();
			LandGroupID = LandPtr->GetLandGroupID();
			WndPtrCount = WndPtrList.size();
			for ( j=0; j<WndPtrCount; j++ )
			{
				WndPtr2 = WndPtrList[j];
				if ( WndPtr2 == NULL ) { continue; }
				if ( WndPtr2->GetWndLandPtr()->GetLandGroupID() != LandGroupID )
				{	continue; }

				if ( WndPtr2->GetWndGroupID() != WndGroupID )
				{	continue; }
				if ( WndPtr2->GetWndBandID() != WndBandID )
				{	continue; }
				break;
			}
			if ( j == WndPtrCount )
			{	WndPtrList.push_back(WndPtr);	}
			continue; 
		}
		WndPtr->SpinWnd(Angle);		
		WndPtr->SetWndModified(true);
		WndPtr->SetWndAttachedAngle(AttachedAngle);
	}		

	WndPtrCount = WndPtrList.size();
	for ( i=0; i<WndPtrCount; i++ )
	{
		WndPtr = WndPtrList[i];
		if ( NULL == WndPtr ) { continue; }		

		WndBandID = WndPtr->GetWndBandID();
		WndGroupID = WndPtr->GetWndGroupID();;
		for ( j=0; j<LandCount; j++ )
		{
			LandPtr = GetModelLandPtr(j, false);
			if ( NULL == LandPtr ) { continue; }			
			WndPtr2 = LandPtr->GetLandWndPtrByGroupID(WndGroupID, WndBandID);			
			if ( NULL == WndPtr2 ) { continue; }

			WndPtr2->SpinWnd(Angle);
			WndPtr2->SetWndModified(true);
			WndPtr2->SetWndAttachedAngle(AttachedAngle);			
		}		
	}
	SetModelModifiedCount(true);
	SetModelNeedSaveFiles(true);
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIModel::UpdateModelWndRgnByLinkMode()
{
	bool IsOK = true;	
	const double ComAngle = CAOIModel::GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComAngle);
	if ( IsExceptionAngle == false )
	{	UpdateModelWndRgnByLinkModeKernel();	}
	else
	{
		RotateModel(-ComAngle, 0, 0);
		UpdateModelWndRgnByLinkModeKernel();
		RotateModel(ComAngle, 0, 0);
	}
	return ;
}
//-------------------------------------------------------------------------------------//
inline void CAOIModel::UpdateModelWndRgnByLinkModeKernel()
{	
	size_t         i=0, j=0;	
	CAOIBox       *BoxPtr      = NULL;
	CAOIWnd       *WndPtr      = NULL;
	CAOILand      *LandPtr     = NULL;	
	BOX_TOWARD     LandToward;
	double WndRgnLinkRatioX = 0;
	double WndRgnLinkRatioY = 0;
	WND_RGN_LINK_MODE   WndRgnLinkMode = WND_RGN_LINK_NONE;
	
	bool           FirstPad = true;
	bool           FirstPart = true;
	int            ScaleMode=0;
	TPOINT2D       PadTipSz;
	TPOINT2D       PtComCad, PtComStage;	
	TREGION4D      Region, RgnBody, RgnPad, RgnLead, RgnLeadTip, RgnLeadShoulder;
	TREGION4D      RgnBodyRgn, RgnPadRgn, RgnLeadRgn, RgnLeadTipRgn, RgnLeadShoulderRgn, RgnPadBodyRgn;
	
	double         LeadEdgeMinX=0, LeadEdgeMinY=0, LeadEdgeMaxX=0, LeadEdgeMaxY=0;	
	double         LeadEdgeRgnMinX=0, LeadEdgeRgnMinY=0, LeadEdgeRgnMaxX=0, LeadEdgeRgnMaxY=0;	

	const size_t WndCount = GetModelWndCount_Inline();		
	const size_t LandCount = GetModelLandCount();		
	
	BoxPtr = GetModelBodyBoxPtr();
	BoxPtr->GetBoxRegion(RgnBody);
	BoxPtr->GetBoxRegion(RgnBodyRgn);
	
	GetModelAttachedPosCad(PtComCad);
	GetModelAttachedPosStage(PtComStage);
	
	FirstPad = true;
	FirstPart = true;
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = GetModelLandPtr(i, false);
		if ( NULL== LandPtr ) { continue; }		

		LandPtr->GetLandPadBox().GetBoxRegion(RgnPad);
		LandPtr->GetLandLeadBox().GetBoxRegion(RgnLead);
		LandPtr->GetLandLeadTipBox().GetBoxRegion(RgnLeadTip);
		LandPtr->GetLandLeadShoulderBox().GetBoxRegion(RgnLeadShoulder);

		if ( LandPtr->GetLandIncludePadAlign() == true )
		{
			if ( FirstPad == true )
			{		
				FirstPad = false;
				LandPtr->GetLandPadBox().GetBoxRegion(RgnPadRgn);
			}
			else
			{	JetAPI::UnionRegion(RgnPad, RgnPadRgn, RgnPadRgn);	}
		}

		if ( LandPtr->GetLandIncludePartAlign() == true )
		{
			if ( FirstPart == true )
			{		
				FirstPart = false;
				LandPtr->GetLandLeadBox().GetBoxRegion(RgnLeadRgn);
				LandPtr->GetLandLeadTipBox().GetBoxRegion(RgnLeadTipRgn);
				LandPtr->GetLandLeadShoulderBox().GetBoxRegion(RgnLeadShoulderRgn);
			}
			else
			{
				JetAPI::UnionRegion(RgnLead, RgnLeadRgn, RgnLeadRgn);
				JetAPI::UnionRegion(RgnLeadTip, RgnLeadTipRgn, RgnLeadTipRgn);
				JetAPI::UnionRegion(RgnLeadShoulder, RgnLeadShoulderRgn, RgnLeadShoulderRgn);
			}
		}	
	}

	RgnPadBodyRgn = RgnBodyRgn;
	if ( FirstPart == false )
	{	
		JetAPI::UnionRegion(RgnLeadRgn, RgnBodyRgn, RgnBodyRgn);	
		JetAPI::UnionRegion(RgnPadRgn, RgnBodyRgn, RgnPadBodyRgn);	
	}

	TREGION4D RgnPadRgnInner = RgnPadRgn;
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = GetModelLandPtr(i, false);
		if ( NULL== LandPtr ) { continue; }		

		LandToward = LandPtr->GetLandToward();
		LandPtr->GetLandPadBox().GetBoxRegion(RgnPad);		
		if ( LandPtr->GetLandIncludePadAlign() == true )
		{
			switch ( LandToward )
			{
			case BOX_TOWARD_UP:
				if ( RgnPadRgnInner.maxY > RgnPad.minY )
				{	RgnPadRgnInner.maxY = RgnPad.minY;	}				
				break;
			case BOX_TOWARD_LEFT:
				if ( RgnPadRgnInner.minX < RgnPad.maxX )
				{	RgnPadRgnInner.minX = RgnPad.maxX;	}
				break;
			case BOX_TOWARD_DOWN:
				if ( RgnPadRgnInner.minY < RgnPad.maxY )
				{	RgnPadRgnInner.minY = RgnPad.maxY;	}
				break;
			case BOX_TOWARD_RIGHT:
				if ( RgnPadRgnInner.maxX > RgnPad.minX )
				{	RgnPadRgnInner.maxX = RgnPad.minX;	}
				break;
			}
		}
	}

	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr_Inline(i);
		if ( NULL== WndPtr ) { continue; }		
		if ( WndPtr->GetWndRgnLinkAuto() == false ) { continue; }		
		WndRgnLinkMode = WndPtr->GetWndRgnLinkMode();
		if ( WND_RGN_LINK_NONE == WndRgnLinkMode ) { continue; }
		WndRgnLinkRatioX = WndPtr->GetWndRgnLinkRatioX();
		WndRgnLinkRatioY = WndPtr->GetWndRgnLinkRatioY();
		WndRgnLinkRatioX = WndRgnLinkRatioX/100.0;
		WndRgnLinkRatioY = WndRgnLinkRatioY/100.0;
		
		LandPtr = WndPtr->GetWndLandPtr();
		if ( NULL == LandPtr )
		{	LandToward = BOX_TOWARD_NULL; }
		else
		{	LandToward = LandPtr->GetLandToward(); }
		ScaleMode = SCALE_REGION_BY_CENTER;
		switch ( WndRgnLinkMode )
		{
		case WND_RGN_LINK_PAD:			
		case WND_RGN_LINK_PAD_RGN:
			if ( LandPtr == NULL )
			{	Region = RgnPadRgn;	}
			else
			{	LandPtr->GetLandPadBox().GetBoxRegion(Region);	}
			break;
		case WND_RGN_LINK_PAD_BODY_RGN:
			if ( LandPtr == NULL )
			{	Region = RgnPadBodyRgn;	}
			else
			{	LandPtr->GetLandPadBox().GetBoxRegion(Region);	}
			break;
		case WND_RGN_LINK_PAD_RGN_INNER:
			if ( LandPtr == NULL )
			{	Region = RgnPadRgnInner;	}
			else
			{	LandPtr->GetLandPadBox().GetBoxRegion(Region);	}
			break;
		case WND_RGN_LINK_PAD_TIP:			
			if ( LandPtr == NULL )
			{	
				Region = RgnPadRgn;	
				WndRgnLinkRatioX = 1.0;
				WndRgnLinkRatioY = 1.0;
			}
			else
			{
				switch ( LandToward )
				{
				case BOX_TOWARD_LEFT:  ScaleMode = SCALE_REGION_BY_SIDE_MIN_X; break;
				case BOX_TOWARD_UP:	   ScaleMode = SCALE_REGION_BY_SIDE_MAX_Y; break;
				case BOX_TOWARD_RIGHT: ScaleMode = SCALE_REGION_BY_SIDE_MAX_X; break;
				case BOX_TOWARD_DOWN:  ScaleMode = SCALE_REGION_BY_SIDE_MIN_Y; break;
				}
				if ( LandPtr->GetLandLeadBox().GetBoxEnabled() == true )
				{
					LandPtr->GetLandPadBox().GetBoxRegion(RgnPad);
					LandPtr->GetLandLeadBox().GetBoxRegion(RgnLead);
					Region = RgnPad;
					switch ( LandToward )
					{
					case BOX_TOWARD_LEFT:
						if ( RgnLead.minX>RgnPad.minX && RgnLead.minX<RgnPad.maxX )
						{	Region.maxX = RgnLead.minX;	}
						break;
					case BOX_TOWARD_UP:
						if ( RgnLead.maxY>RgnPad.minY && RgnLead.maxY<RgnPad.maxY )
						{	Region.minY = RgnLead.maxY;		}
						break;
					case BOX_TOWARD_RIGHT:
						if ( RgnLead.maxX>RgnPad.minX && RgnLead.maxX<RgnPad.maxX )
						{	Region.minX = RgnLead.maxX;		}
						break;
					case BOX_TOWARD_DOWN:
						if ( RgnLead.minY>RgnPad.minY && RgnLead.minY<RgnPad.maxY )
						{	Region.maxY = RgnLead.minY;		}
						break;
					}									
				}
				else
				{	LandPtr->GetLandPadBox().GetBoxRegion(Region);	}
			}
			break;
		case WND_RGN_LINK_BODY:
			if ( LandPtr == NULL )
			{	Region = RgnBody;	}
			else
			{	Region = RgnBody;	}			
			break;
		case WND_RGN_LINK_LEAD:		
			if ( LandPtr == NULL )
			{	
				Region = RgnLeadRgn;
				WndRgnLinkRatioX = 1.0;
				WndRgnLinkRatioY = 1.0;
			}
			else
			{	LandPtr->GetLandLeadBox().GetBoxRegion(Region);	}
			ScaleMode = SCALE_REGION_BY_CENTER;
			break;
		case WND_RGN_LINK_LEAD_TIP:
			if ( LandPtr == NULL )
			{	
				WndRgnLinkRatioX = 1.0;
				WndRgnLinkRatioY = 1.0;
				Region = RgnLeadTipRgn;					
			}
			else
			{	LandPtr->GetLandLeadTipBox().GetBoxRegion(Region);	}
			break;
		case WND_RGN_LINK_LEAD_SHOULDER:
			if ( LandPtr == NULL )
			{	
				WndRgnLinkRatioX = 1.0;
				WndRgnLinkRatioY = 1.0;
				Region = RgnLeadShoulderRgn;				
			}
			else
			{	LandPtr->GetLandLeadShoulderBox().GetBoxRegion(Region);	}
			break;
		case WND_RGN_LINK_LEAD_TIP_SHOULDER:
			if ( LandPtr == NULL )
			{
				WndRgnLinkRatioX = 1.0;
				WndRgnLinkRatioY = 1.0;
				JetAPI::UnionRegion(RgnLeadTipRgn, RgnLeadShoulderRgn, Region);	
			}
			else
			{
				LandPtr->GetLandLeadTipBox().GetBoxRegion(RgnLeadTip);
				LandPtr->GetLandLeadShoulderBox().GetBoxRegion(RgnLeadShoulder);
				JetAPI::UnionRegion(RgnLeadTip, RgnLeadShoulder, Region);
			}
			break;
		}
		JetAPI::ScaleRegion(Region, WndRgnLinkRatioX, WndRgnLinkRatioY, ScaleMode, Region);
		WndPtr->SetWndRegion(Region);
		WndPtr->UpdateWndExtendBox();
		WndPtr->SetWndAttachedPosCad(PtComCad);
		WndPtr->SetWndAttachedPosStage(PtComStage);	
		//WndPtr->BuildWndFeatureBoxList();
	}
	//SetModelModifiedCount(true);
	SetModelNeedSaveFiles(true);
	return;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::DeleteModelWndIndexList(std::vector<unsigned int> &WndIndexList)
{	
	size_t       i=0, j=0;	
	unsigned int WndIndex = 0;
	CAOIWnd     *WndPtr = NULL;			
	const size_t WndIndexCount = WndIndexList.size();
	
	SetModelLogicSelected(false);
	SetModelWndSelected(false);
	SetModelLandSelected(false);

	for ( i=0; i<WndIndexCount; i++ )
	{
		WndIndex = WndIndexList[i];
		WndPtr = CAOIModel::GetModelWndPtr(WndIndex, true);		
		if ( NULL == WndPtr ) { continue; }
		WndPtr->SetWndSelected(true);
	}

	ClearModelWndOrderList_Inline();
	RemoveModelObjectByWndObjSelected();	
	RemoveModelAlgPatternFolderBySelected();
	DestroyModelWndSelected();	
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIModel::ClearModelWndOrderList()
{
	ClearModelWndOrderList_Inline();
}
//-------------------------------------------------------------------------------------//
size_t CAOIModel::GetModelWndOrderCount() const
{
	return m_ModelWndOrderList.size();
}
//-------------------------------------------------------------------------------------//
CAOIWnd* CAOIModel::GetModelWndOrderPtr(size_t idx, bool Check) const
{
	if ( true == Check )
	{
		const size_t Count = m_ModelWndOrderList.size();
		if ( idx >= Count ) 
		{	return NULL; }
	}
	return m_ModelWndOrderList[idx];
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::SortModelWndOrder(std::vector<CAOIWnd*> &WndList)//排序模組檢測框
{
	const size_t WndCount = WndList.size();
	if ( 0 == WndCount ) { return true; }

	size_t                i=0;
	CAOIWnd              *WndPtr=NULL;
	CSortObj              SortObj;
	CSortObj             *SortObjPtr=NULL;
	int                   WndOrder=0;
	WND_DEFECT_ID         WndDefectID;	
	std::vector<CSortObj> WndSortList(WndCount);
	
	WndSortList.clear();
	SortObj.SetSortMode(SORT_BY_INT);	
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = WndList[i];
		if ( NULL == WndPtr ) { continue; }

		WndDefectID = WndPtr->GetWndDefectID();
		WndOrder = AOIDataDefine.GetWndDefectIDOrder(WndDefectID);
		WndOrder = WndOrder+(int)(i);
		SortObj.SetValueInt(WndOrder);
		SortObj.SetPtr(WndPtr);
		WndSortList.push_back(SortObj);		
	}

	WndList.clear();
	std::sort(WndSortList.begin(), WndSortList.end());
	const size_t SortCount = WndSortList.size();
	for ( i=0; i<SortCount; i++ )	
	{
		SortObjPtr = &(WndSortList[i]);
		WndPtr = (CAOIWnd*)(SortObjPtr->GetPtr());
		WndList.push_back(WndPtr);		
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
