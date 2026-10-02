// AOIModel_Land.cpp: implementation of the CAOIModel class.
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
inline size_t CAOIModel::GetModelLandCount_Inline() const
{
	return CAOIModel::m_ModelLandList.size();
}
//-------------------------------------------------------------------------------------//
inline void CAOIModel::AddModelLandPtr_Inline(CAOILand *LandPtr)
{
	CAOIModel::m_ModelLandList.push_back(LandPtr);		
}
//-------------------------------------------------------------------------------------//
inline CAOILand* CAOIModel::GetModelLandPtr_Inline(size_t index) const
{	
	return (CAOIModel::m_ModelLandList[index]);	
}
//-------------------------------------------------------------------------------------//
void CAOIModel::ClearModelLandList()
{
	size_t       i = 0;
	CAOILand    *LandPtr = NULL;
	const size_t LandCount = GetModelLandCount_Inline();
	for ( i=0; i<LandCount; i++ )
	{	
		LandPtr = CAOIModel::GetModelLandPtr_Inline(i);
		if ( LandPtr == NULL ) { continue; }
		AOIObjManager.DestroyLandObj(LandPtr);		
	}
	m_ModelLandList.clear();
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::RemoveModelWndWithoutFrameIndex(const std::vector<unsigned int> &FrameIndexMapList)//移除模組檢測框-沒有影像引數
{
	size_t     i=0;	
	CAOIWnd   *WndPtr = NULL;	
	CAOIModel *ModelPtr = this;			
	unsigned int FrameIndex = 0;
	unsigned int FrameUniqueID = 0;
	std::vector<CAOIWnd*> RemoveWndList;
	const unsigned int DefaultIndex    = -1;
	const unsigned int DefaultUniqueID = 0;
	const size_t ModelWndCount = ModelPtr->GetModelWndCount();	

	for ( i=0; i<ModelWndCount; i++ )
	{
		WndPtr = ModelPtr->GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		WndPtr->UpdateWndFrameUniqueID(DefaultIndex, DefaultUniqueID, FrameIndexMapList);

		const CAlgParam &AlgParam=WndPtr->GetWndAlgParam();
		FrameIndex = AlgParam.GetAlgImageBinParam().GetBinaryFrameIndex();
		if ( DefaultIndex == FrameIndex )
		{	
			RemoveWndList.push_back(WndPtr);
			continue;
		}

		FrameIndex = AlgParam.GetAlgMaskBinParam().GetBinaryFrameIndex();
		if ( DefaultIndex == FrameIndex )
		{	
			RemoveWndList.push_back(WndPtr);
			continue;
		}
	}	
	
	const size_t RemoveWndCount=RemoveWndList.size();
	if ( RemoveWndCount > 0 )
	{
		ModelPtr->SetModelWndSelected(false);
		for ( i=0; i<RemoveWndCount; i++ )
		{
			WndPtr = RemoveWndList[i];
			WndPtr->SetWndSelected(true);
		}
		ModelPtr->DeleteModelWndSelected(false);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::UpdateModelFrameIndex(unsigned int DefaultIndex, unsigned int DefaultUniqueID, const std::vector<unsigned int> &FrameIndexMapList)
{
	const bool bRemoveWithoutFrameIndexWnd=false;
	if ( true == bRemoveWithoutFrameIndexWnd )
	{
		if ( RemoveModelWndWithoutFrameIndex(FrameIndexMapList) == false )
		{	return false; }
	}

	size_t     i=0;	
	CAOIWnd   *WndPtr = NULL;
	CAOILand  *LandPtr = NULL;
	CAOIModel *ModelPtr = this;	
	CAlgBinaryParam *BinParamPtr = NULL;	
	const size_t ModelWndCount = ModelPtr->GetModelWndCount();
	const size_t ModelLandCount = ModelPtr->GetModelLandCount_Inline();
	
	//本體顏色
	CColorGroup   &BodyColorGroup = GetModelBodyColorGroup();
	BodyColorGroup.UpdateColorGroupFrameUniqueID(DefaultIndex, DefaultUniqueID, FrameIndexMapList);	

	//引腳顏色
	for ( i=0; i<ModelLandCount; i++ )
	{
		LandPtr = ModelPtr->GetModelLandPtr_Inline(i);
		if ( NULL == LandPtr ) { continue; }
		CColorGroup   &LeadColorGroup = LandPtr->GetLandLeadColorGroup();
		LeadColorGroup.UpdateColorGroupFrameUniqueID(DefaultIndex, DefaultUniqueID, FrameIndexMapList);		
	}

	for ( i=0; i<ModelWndCount; i++ )
	{
		WndPtr = ModelPtr->GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		WndPtr->UpdateWndFrameUniqueID(DefaultIndex, DefaultUniqueID, FrameIndexMapList);		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
size_t CAOIModel::GetModelLandCount() const
{ 
	return CAOIModel::GetModelLandCount_Inline();
}
//-------------------------------------------------------------------------------------//
void CAOIModel::AddModelLandPtr_Direct(CAOILand *LandPtr)
{
	AddModelLandPtr_Inline(LandPtr);
}
//-------------------------------------------------------------------------------------//
CAOILand* CAOIModel::AddModelLandPtr(CAOILand *LandPtr, bool Clone)
{	
	if ( NULL == LandPtr ) { return NULL; }	
	if ( false == Clone )
	{
		const unsigned int LandIndex = (unsigned int)(CAOIModel::GetModelLandCount_Inline());
		AddModelLandPtr_Inline(LandPtr);	
		LandPtr->SetLandIndex(LandIndex);
		LandPtr->SetLandModelPtr(this);		
		SetModelModifiedCount(true);
		SetModelNeedSaveFiles(true);		
		return LandPtr;
	}

	CAOILand *NewLandPtr = CopyModelLand(LandPtr);
	SetModelModifiedCount(true);
	SetModelNeedSaveFiles(true);
	return NewLandPtr;		
}
//-------------------------------------------------------------------------------------//
CAOILand* CAOIModel::CopyModelLand(const CAOILand *LandPtr)
{
	if ( (NULL==LandPtr) ) { return NULL; }	
	CAOILand *NewLandPtr = LandPtr->CloneLandObj();
	if( NULL == NewLandPtr ) 
	{	return NULL; }

	size_t       i=0, j=0;
	int          WndBandID = 0;
	int          WndGroupID = 0;
	size_t       TempCount = 0;		
	size_t       LogicWndCount=0;
	CAOIWnd     *WndPtr = NULL;
	CAOILogic   *LogicPtr = NULL;
	CAOIWnd     *NewWndPtr = NULL;
	CAOILogic   *NewLogicPtr = NULL;
	const size_t LandWndCount = LandPtr->GetLandWndCount();
	const size_t LandLogicCount = LandPtr->GetLandLogicCount();

	std::vector<CAOIWnd*>      TempWndList;
	std::vector<CAOILogic*>    TempLogicList;
	std::vector<CAOIWnd*>      ModelWndList = m_ModelWndList;
	std::vector<CAOILand*>     ModelLandList = m_ModelLandList;
	std::vector<CAOILogic*>    ModelLogicList = m_ModelLogicList;

	AddModelLandPtr(NewLandPtr, false);
	NewLandPtr->RemoveLandWndList();
	NewLandPtr->RemoveLandLogicList();
	for ( i=0; i<LandWndCount; i++ )
	{
		WndPtr = LandPtr->GetLandWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		NewWndPtr = WndPtr->CloneWndObj();
		if ( NULL == NewWndPtr )	
		{	
			TempCount = TempWndList.size();
			for ( j=0; j<TempCount; j++ )
			{	AOIObjManager.DestroyWndObj(TempWndList[j]); }
			TempCount = TempLogicList.size();
			for ( j=0; j<TempCount; j++ )
			{	AOIObjManager.DestroyLogicObj(TempLogicList[j]); }
			AOIObjManager.DestroyLandObj(NewLandPtr);
			m_ModelWndList   = ModelWndList;
			m_ModelLandList  = ModelLandList;
			m_ModelLogicList = ModelLogicList;
			return NULL;
		}
		CAOIModel::AddModelWndPtr(NewWndPtr, false);
		NewLandPtr->AddLandWndPtr(NewWndPtr);
		TempWndList.push_back(NewWndPtr);
	}
	for ( i=0; i<LandLogicCount; i++ )
	{
		LogicPtr = LandPtr->GetLandLogicPtr(i, false);
		if ( NULL == LogicPtr ) { continue; }
		NewLogicPtr = LogicPtr->CloneLogicObj();
		if ( NULL == NewLogicPtr )	
		{	
			TempCount = TempWndList.size();
			for ( j=0; j<TempCount; j++ )
			{	AOIObjManager.DestroyWndObj(TempWndList[j]); }
			TempCount = TempLogicList.size();
			for ( j=0; j<TempCount; j++ )
			{	AOIObjManager.DestroyLogicObj(TempLogicList[j]); }
			AOIObjManager.DestroyLandObj(NewLandPtr);

			m_ModelWndList   = ModelWndList;
			m_ModelLandList  = ModelLandList;
			m_ModelLogicList = ModelLogicList;
			return NULL;
		}
		CAOIModel::AddModelLogicPtr(NewLogicPtr, false);
		NewLandPtr->AddLandLogicPtr(NewLogicPtr);
		TempLogicList.push_back(NewLogicPtr);

		NewLogicPtr->RemoveLogicWndPtrList();
		LogicWndCount = LogicPtr->GetLogicWndPtrCount();
		for ( j=0; j<LogicWndCount; j++ )
		{
			WndPtr = LogicPtr->GetLogicWndPtr(j, false);
			if ( NULL == WndPtr ) { continue; }
			WndBandID = WndPtr->GetWndBandID();
			WndGroupID = WndPtr->GetWndGroupID();
			NewWndPtr = NewLandPtr->GetLandWndPtrByGroupID(WndGroupID, WndBandID);
			if ( NULL == NewWndPtr ) { continue; }
			NewLogicPtr->AddLogicWndPtr(NewWndPtr);				
		}
	}
	return NewLandPtr;	
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::DestroyModelLandSelected()//刪除選中的特徵框
{
	size_t       i=0, j=0;	
	unsigned int LandIndex=0;	
	size_t       WndCount = 0;	
	size_t       LandCount = 0;	
	CAOIWnd     *WndPtr = NULL;
	CAOILand    *LandPtr = NULL;		
	std::vector<CAOILand*>  ModelLandList = CAOIModel::m_ModelLandList;	
	
	LandIndex=0;	
	CAOIModel::m_ModelLandList.clear();
	LandCount = ModelLandList.size();
	for ( i=0; i<LandCount; i++ )
	{	
		LandPtr = ModelLandList[i];
		if ( NULL == LandPtr ) { continue; }
		if ( LandPtr->GetLandSelected() == true ) 
		{
			AOIObjManager.DestroyLandObj(ModelLandList[i]);
			continue; 
		}
		LandPtr->SetLandIndex(LandIndex);
		WndCount = LandPtr->GetLandWndCount();
		for ( j=0; j<WndCount; j++ )
		{
			WndPtr = LandPtr->GetLandWndPtr(j, false);
			if ( NULL == WndPtr ) { continue; }
			WndPtr->SetWndLandIndex(LandIndex);
		}
		AddModelLandPtr_Inline(LandPtr);
		LandIndex ++;
	}
	SetModelModifiedCount(true);
	SetModelNeedSaveFiles(true);
	return true;
}
//-------------------------------------------------------------------------------------//
CAOILand* CAOIModel::CreateModelLand(MODEL_TYPE ModelType, TREGION4D Region, LAND_TYPE LandType)
{
	CAOILand  *LandPtr = NULL;
	BOX_TOWARD LandToward = BOX_TOWARD_NULL;	
	TSIZE2D    szPad, szLead, szBody;	
	TPOINT2D   ModelCp, BodyCp, RgnCp;	
	TREGION4D  RgnBody;
	TREGION4D  RgnPad, RgnLead, RgnLeadTip, RgnLeadShoulder;
	double       BodyMarginX=100, BodyMarginY=100;
	double       scPadU=1.0, scPadV=1.0, scPadV2=0.0;
	const double AttachedAngle = GetModelAttachedAngle();
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(AttachedAngle);

	ModelCp.x = ModelCp.y = 0;	
	if ( true == IsExceptionAngle )
	{		
		//將四端點轉正
		TPOINT2D CornerPos[4];
		CAOIModel::m_ModelBodyBox.GetBoxCornerPos(CornerPos);		
		JetAPI::RotateCornerPos(-AttachedAngle, ModelCp.x, ModelCp.y, CornerPos);
		JetAPI::PointsToRegion(CornerPos, 4, RgnBody);		
	}
	else
	{	CAOIModel::GetModelBodyRegion(RgnBody);	 }
	BodyCp.x = (RgnBody.minX+RgnBody.maxX)*0.5;
	BodyCp.y = (RgnBody.minY+RgnBody.maxY)*0.5;
	RgnCp.x = (Region.minX+Region.maxX)*0.5;
	RgnCp.y = (Region.minY+Region.maxY)*0.5;
	szBody.cx = ::fabs(RgnBody.maxX-RgnBody.minX);
	szBody.cy = ::fabs(RgnBody.maxY-RgnBody.minY);

	BodyMarginX = MIN(szBody.cx*0.2, szBody.cy*0.2);//szBody.cx*0.2;
	BodyMarginY = MIN(szBody.cx*0.2, szBody.cy*0.2);//szBody.cy*0.2;

	const BOX_TOWARD ModelToward = GetModelBodyBox().GetBoxToward();
	if ( LAND_TYPE_DIP_LEAD == LandType )
	{	LandToward = ModelToward;	}
	else
	{
		if ( RgnCp.x < (RgnBody.minX) ) 
		{	LandToward = BOX_TOWARD_LEFT;	}
		else if ( RgnCp.x > (RgnBody.maxX) ) 
		{	LandToward = BOX_TOWARD_RIGHT;	}
		else if ( RgnCp.y > (RgnBody.maxY) ) 
		{	LandToward = BOX_TOWARD_UP;	}
		else if ( RgnCp.y < (RgnBody.minY) ) 
		{	LandToward = BOX_TOWARD_DOWN;	}
		else if ( RgnCp.x < (RgnBody.minX+BodyMarginX) ) 
		{	LandToward = BOX_TOWARD_LEFT;	}
		else if ( RgnCp.x > (RgnBody.maxX-BodyMarginX) ) 
		{	LandToward = BOX_TOWARD_RIGHT;	}
		else if ( RgnCp.y > (RgnBody.maxY-BodyMarginY) ) 
		{	LandToward = BOX_TOWARD_UP;	}
		else if ( RgnCp.y < (RgnBody.minY+BodyMarginY) ) 
		{	LandToward = BOX_TOWARD_DOWN;	}
		else
		{			
			switch ( ModelToward )
			{
			case BOX_TOWARD_LEFT:
			case BOX_TOWARD_RIGHT:
				if ( RgnCp.x > BodyCp.x )
				{	LandToward = BOX_TOWARD_RIGHT; }
				else
				{	LandToward = BOX_TOWARD_LEFT; }
				break;
			case BOX_TOWARD_UP:
			case BOX_TOWARD_DOWN:
				if ( RgnCp.y > BodyCp.y )
				{	LandToward = BOX_TOWARD_UP; }
				else
				{	LandToward = BOX_TOWARD_DOWN; }
				break;
			}		
		}
	}

	LandPtr = AOIObjManager.CreateLandObj();
	if ( NULL == LandPtr )
	{	return LandPtr; }

	double MaxLandDif = 500;
	const int LanGroupID = GetModelLandFreeGroupID();

	SetModelLandActived(NULL);
	LandPtr->SetLandToward(LandToward);
	LandPtr->SetLandGroupID(LanGroupID);
	LandPtr->SwitchLandType(LandType);

	if ( LAND_TYPE_PAD == LandType )
	{
		LandPtr->GetLandPadBox().SetBoxRegion(Region);
		LandPtr->GetLandBoxPtr()->SetBoxActived(true);
		LandPtr->GetLandBoxPtr()->SetBoxSelected(true);
	}
	else if ( LAND_TYPE_ELECTRODE == LandType )
	{
		scPadU = 1.2;
		scPadV = 2.4;
		scPadV2 = 0.2;
		MaxLandDif = 500;		
		RgnPad = Region;
		szLead.cx = ::fabs(Region.maxX - Region.minX);
		szLead.cy = ::fabs(Region.maxY - Region.minY);
		LandPtr->GetLandLeadBox().SetBoxRegion(Region);
		switch ( LandToward )
		{
		case BOX_TOWARD_UP:
			szPad.cx = scPadU*szLead.cx;
			szPad.cy = scPadV*szLead.cy;
			if ( (szPad.cy-szLead.cy) > MaxLandDif ) { szPad.cy = szLead.cy+MaxLandDif; }

			RgnPad.minX = RgnCp.x - (szPad.cx*0.5);
			RgnPad.maxX = RgnCp.x + (szPad.cx*0.5);
			RgnPad.minY = Region.minY - scPadV2*szLead.cy;
			RgnPad.maxY = RgnPad.minY + szPad.cy;
			break;
		case BOX_TOWARD_LEFT:
			szPad.cx = scPadV*szLead.cx;
			szPad.cy = scPadU*szLead.cy;
			if ( (szPad.cx-szLead.cx) > MaxLandDif ) { szPad.cx = szLead.cx+MaxLandDif; }

			RgnPad.minY = RgnCp.y - (szPad.cy*0.5);
			RgnPad.maxY = RgnCp.y + (szPad.cy*0.5);
			RgnPad.maxX = Region.maxX + scPadV2*szLead.cx;
			RgnPad.minX = RgnPad.maxX - szPad.cx;
			break;
		case BOX_TOWARD_DOWN:
			szPad.cx = scPadU*szLead.cx;
			szPad.cy = scPadV*szLead.cy;
			if ( (szPad.cy-szLead.cy) > MaxLandDif ) { szPad.cy = szLead.cy+MaxLandDif; }

			RgnPad.minX = RgnCp.x - (szPad.cx*0.5);
			RgnPad.maxX = RgnCp.x + (szPad.cx*0.5);
			RgnPad.maxY = Region.maxY + scPadV2*szLead.cy;
			RgnPad.minY = RgnPad.maxY - szPad.cy;
			break;
		case BOX_TOWARD_RIGHT:
			szPad.cx = scPadV*szLead.cx;
			szPad.cy = scPadU*szLead.cy;
			if ( (szPad.cx-szLead.cx) > MaxLandDif ) { szPad.cx = szLead.cx+MaxLandDif; }

			RgnPad.minY = RgnCp.y - (szPad.cy*0.5);
			RgnPad.maxY = RgnCp.y + (szPad.cy*0.5);
			RgnPad.minX = Region.minX - scPadV2*szLead.cx;
			RgnPad.maxX = RgnPad.minX + szPad.cx;
			break;
		}
		LandPtr->GetLandPadBox().SetBoxRegion(RgnPad);
		LandPtr->GetLandPadBox().SetBoxActived(true);
		LandPtr->GetLandPadBox().SetBoxSelected(true);
		LandPtr->LayoutLeadBox(LandPtr->GetLandLeadBoxPtr(), false);
	}
	else if ( LAND_TYPE_IC_LEAD==LandType || LAND_TYPE_CON_LEAD==LandType )
	{
		scPadU = 1.2;
		scPadV = 2.0;
		scPadV2 = 0.2;
		MaxLandDif = 2000;
		RgnPad = RgnLeadTip = RgnLeadShoulder =Region;		
		szLead.cx = ::fabs(Region.maxX - Region.minX);		
		szLead.cy = ::fabs(Region.maxY - Region.minY);		
		LandPtr->GetLandLeadBox().SetBoxRegion(Region);
		switch ( LandToward )
		{
		case BOX_TOWARD_UP:
			szPad.cx = scPadU*szLead.cx;
			szPad.cy = scPadV*szLead.cy;
			if ( (szPad.cy-szLead.cy) > MaxLandDif ) { szPad.cy = szLead.cy+MaxLandDif; }

			RgnPad.minX = RgnCp.x - (szPad.cx*0.5);
			RgnPad.maxX = RgnCp.x + (szPad.cx*0.5);
			RgnPad.minY = Region.minY - scPadV2*szLead.cy;
			RgnPad.maxY = RgnPad.minY + szPad.cy;

			RgnLeadTip.minY = RgnLeadTip.maxY - (szLead.cy*0.4);
			RgnLeadShoulder.maxY = RgnLeadShoulder.minY + (szLead.cy*0.4);

			break;
		case BOX_TOWARD_LEFT:
			szPad.cx = scPadV*szLead.cx;
			szPad.cy = scPadU*szLead.cy;
			if ( (szPad.cx-szLead.cx) > MaxLandDif ) { szPad.cx = szLead.cx+MaxLandDif; }

			RgnPad.minY = RgnCp.y - (szPad.cy*0.5);
			RgnPad.maxY = RgnCp.y + (szPad.cy*0.5);
			RgnPad.maxX = Region.maxX + scPadV2*szLead.cx;
			RgnPad.minX = RgnPad.maxX - szPad.cx;

			RgnLeadTip.maxX = RgnLeadTip.minX + (szLead.cx*0.4);
			RgnLeadShoulder.minX = RgnLeadShoulder.maxX - (szLead.cx*0.4);
			break;
		case BOX_TOWARD_DOWN:
			szPad.cx = scPadU*szLead.cx;
			szPad.cy = scPadV*szLead.cy;
			if ( (szPad.cy-szLead.cy) > MaxLandDif ) { szPad.cy = szLead.cy+MaxLandDif; }

			RgnPad.minX = RgnCp.x - (szPad.cx*0.5);
			RgnPad.maxX = RgnCp.x + (szPad.cx*0.5);
			RgnPad.maxY = Region.maxY + scPadV2*szLead.cy;
			RgnPad.minY = RgnPad.maxY - szPad.cy;

			RgnLeadTip.maxY = RgnLeadTip.minY + (szLead.cy*0.4);
			RgnLeadShoulder.minY = RgnLeadShoulder.maxY - (szLead.cy*0.4);
			break;
		case BOX_TOWARD_RIGHT:
			szPad.cx = scPadV*szLead.cx;
			szPad.cy = scPadU*szLead.cy;
			if ( (szPad.cx-szLead.cx) > MaxLandDif ) { szPad.cx = szLead.cx+MaxLandDif; }

			RgnPad.minY = RgnCp.y - (szPad.cy*0.5);
			RgnPad.maxY = RgnCp.y + (szPad.cy*0.5);
			RgnPad.minX = Region.minX - scPadV2*szLead.cx;
			RgnPad.maxX = RgnPad.minX + szPad.cx;

			RgnLeadTip.minX = RgnLeadTip.maxX - (szLead.cx*0.4);
			RgnLeadShoulder.maxX = RgnLeadShoulder.minX + (szLead.cx*0.4);
			break;
		}
		LandPtr->GetLandPadBox().SetBoxRegion(RgnPad);
		LandPtr->GetLandLeadTipBox().SetBoxRegion(RgnLeadTip);
		LandPtr->GetLandLeadShoulderBox().SetBoxRegion(RgnLeadShoulder);
		LandPtr->GetLandBoxPtr()->SetBoxActived(true);
		LandPtr->GetLandBoxPtr()->SetBoxSelected(true);		
	}
	else if ( LAND_TYPE_DIP_LEAD == LandType )
	{
		RgnPad = Region;
		JetAPI::ScaleRegion(RgnPad, 0.25, 0.25, SCALE_REGION_BY_CENTER, RgnLead);
		LandPtr->GetLandLeadBox().SetBoxRegion(RgnLead);
		LandPtr->GetLandPadBox().SetBoxRegion(RgnPad);
		LandPtr->GetLandPadBox().SetBoxActived(true);
		LandPtr->GetLandPadBox().SetBoxSelected(true);
		LandPtr->LayoutLeadBox(LandPtr->GetLandLeadBoxPtr(), false);
	}

	if ( true == IsExceptionAngle )
	{	LandPtr->RotateLand(AttachedAngle, ModelCp.x, ModelCp.y); }

	switch ( ModelType )
	{
	case MODEL_TYPE_BGA:
	case MODEL_TYPE_GOLD_FINGER:
		LandToward = GetModelBodyBox().GetBoxToward();
		LandPtr->SetLandToward(LandToward);
		break;
	}
	LandPtr->SetLandAttachedAngle(AttachedAngle);
	LandPtr->SetLandAttachedPosCad(m_ModelAttachedCadPos);
	LandPtr->SetLandAttachedPosStage(m_ModelAttachedStagePos);
	return LandPtr;
}
//-------------------------------------------------------------------------------------//
CAOILand* CAOIModel::CreateModelLand(MODEL_TYPE ModelType, int groupID, TREGION4D PadRegion, TREGION4D LeadRegion, LAND_TYPE LandType)
{
	CAOILand  *LandPtr = NULL;
	BOX_TOWARD LandToward = BOX_TOWARD_NULL;
	TSIZE2D    szPad, szLead, szBody;
	TPOINT2D   ModelCp, BodyCp, RgnCp;
	TREGION4D  RgnBody;
	TREGION4D  RgnPad, RgnLead /*,RgnLeadTip, RgnLeadShoulder*/;
	double       BodyMarginX = 100, BodyMarginY = 100;
	const double AttachedAngle = GetModelAttachedAngle();
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(AttachedAngle);

	ModelCp.x = ModelCp.y = 0;
	if (true == IsExceptionAngle)
	{
		//將四端點轉正
		TPOINT2D CornerPos[4];
		CAOIModel::m_ModelBodyBox.GetBoxCornerPos(CornerPos);
		JetAPI::RotateCornerPos(-AttachedAngle, ModelCp.x, ModelCp.y, CornerPos);
		JetAPI::PointsToRegion(CornerPos, 4, RgnBody);
	}
	else
	{
		CAOIModel::GetModelBodyRegion(RgnBody);
	}
	BodyCp.x = (RgnBody.minX + RgnBody.maxX)*0.5;
	BodyCp.y = (RgnBody.minY + RgnBody.maxY)*0.5;
	RgnCp.x = (PadRegion.minX + PadRegion.maxX)*0.5;
	RgnCp.y = (PadRegion.minY + PadRegion.maxY)*0.5;
	szBody.cx = ::fabs(RgnBody.maxX - RgnBody.minX);
	szBody.cy = ::fabs(RgnBody.maxY - RgnBody.minY);

	BodyMarginX = MIN(szBody.cx*0.2, szBody.cy*0.2);//szBody.cx*0.2;
	BodyMarginY = MIN(szBody.cx*0.2, szBody.cy*0.2);//szBody.cy*0.2;

	if (RgnCp.x < (RgnBody.minX))
	{
		LandToward = BOX_TOWARD_LEFT;
	}
	else if (RgnCp.x >(RgnBody.maxX))
	{
		LandToward = BOX_TOWARD_RIGHT;
	}
	else if (RgnCp.y > (RgnBody.maxY))
	{
		LandToward = BOX_TOWARD_UP;
	}
	else if (RgnCp.y < (RgnBody.minY))
	{
		LandToward = BOX_TOWARD_DOWN;
	}
	else if (RgnCp.x < (RgnBody.minX + BodyMarginX))
	{
		LandToward = BOX_TOWARD_LEFT;
	}
	else if (RgnCp.x >(RgnBody.maxX - BodyMarginX))
	{
		LandToward = BOX_TOWARD_RIGHT;
	}
	else if (RgnCp.y > (RgnBody.maxY - BodyMarginY))
	{
		LandToward = BOX_TOWARD_UP;
	}
	else if (RgnCp.y < (RgnBody.minY + BodyMarginY))
	{
		LandToward = BOX_TOWARD_DOWN;
	}
	else
	{
		BOX_TOWARD ModelToward = GetModelBodyBox().GetBoxToward();
		switch (ModelToward)
		{
		case BOX_TOWARD_LEFT:
		case BOX_TOWARD_RIGHT:
			if (RgnCp.x > BodyCp.x)
			{
				LandToward = BOX_TOWARD_RIGHT;
			}
			else
			{
				LandToward = BOX_TOWARD_LEFT;
			}
			break;
		case BOX_TOWARD_UP:
		case BOX_TOWARD_DOWN:
			if (RgnCp.y > BodyCp.y)
			{
				LandToward = BOX_TOWARD_UP;
			}
			else
			{
				LandToward = BOX_TOWARD_DOWN;
			}
			break;
		}
	}

	LandPtr = AOIObjManager.CreateLandObj();
	if (NULL == LandPtr)
	{
		return LandPtr;
	}

	SetModelLandActived(NULL);
	LandPtr->SetLandToward(LandToward);
	LandPtr->SetLandGroupID(groupID);
	LandPtr->SwitchLandType(LandType);

	if (LAND_TYPE_PAD == LandType)
	{
	}
	else if (LAND_TYPE_ELECTRODE == LandType)
	{
		TREGION4D  leadRegion = LeadRegion; //LeadRegion Y min max是反的
		if (leadRegion.minY > leadRegion.maxY)
		{
			leadRegion.minY = LeadRegion.maxY;
			leadRegion.maxY = LeadRegion.minY;
		}
		TREGION4D  PRegion = PadRegion; //LeadRegion Y min max是反的
		if (PRegion.minY > PRegion.maxY)
		{
			PRegion.minY = PadRegion.maxY;
			PRegion.maxY = PadRegion.minY;
		}

		LandPtr->GetLandLeadBox().SetBoxRegion(leadRegion);
		LandPtr->GetLandPadBox().SetBoxRegion(PRegion);
		LandPtr->GetLandPadBox().SetBoxActived(true);
		LandPtr->GetLandPadBox().SetBoxSelected(true);
		LandPtr->LayoutLeadBox(LandPtr->GetLandLeadBoxPtr(), false);
	}
	else if (LAND_TYPE_IC_LEAD == LandType || LAND_TYPE_CON_LEAD == LandType)
	{

	}

	if (true == IsExceptionAngle)
	{
		LandPtr->RotateLand(AttachedAngle, ModelCp.x, ModelCp.y);
	}

	switch (ModelType)
	{
	case MODEL_TYPE_BGA:
	case MODEL_TYPE_GOLD_FINGER:
		LandToward = GetModelBodyBox().GetBoxToward();
		LandPtr->SetLandToward(LandToward);
		break;
	}
	LandPtr->SetLandAttachedAngle(AttachedAngle);
	LandPtr->SetLandAttachedPosCad(m_ModelAttachedCadPos);
	LandPtr->SetLandAttachedPosStage(m_ModelAttachedStagePos);
	return LandPtr;
}
//-------------------------------------------------------------------------------------//
CAOILand* CAOIModel::CreateModelLand2(MODEL_TYPE ModelType, int groupID, TREGION4D PadRegion, TREGION4D LeadRegion, LAND_TYPE LandType)
{
	CAOILand  *LandPtr = NULL;
	BOX_TOWARD LandToward = BOX_TOWARD_NULL;
	TSIZE2D    szLead, szBody;
	TPOINT2D   ModelCp, BodyCp, RgnCp;
	TREGION4D  RgnBody;
	TREGION4D  RgnLead, RgnLeadTip, RgnLeadShoulder;
	double       BodyMarginX = 0, BodyMarginY = 0;
	double       scPadU = 1.0, scPadV = 1.0, scPadV2 = 0.0;
	const double AttachedAngle = GetModelAttachedAngle();
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(AttachedAngle);

	ModelCp.x = ModelCp.y = 0;
	if (true == IsExceptionAngle)
	{
		//將四端點轉正
		TPOINT2D CornerPos[4];
		CAOIModel::m_ModelBodyBox.GetBoxCornerPos(CornerPos);
		JetAPI::RotateCornerPos(-AttachedAngle, ModelCp.x, ModelCp.y, CornerPos);
		JetAPI::PointsToRegion(CornerPos, 4, RgnBody);
	}
	else
	{
		CAOIModel::GetModelBodyRegion(RgnBody);
	}
	BodyCp.x = (RgnBody.minX + RgnBody.maxX)*0.5;
	BodyCp.y = (RgnBody.minY + RgnBody.maxY)*0.5;
	RgnCp.x = (LeadRegion.minX + LeadRegion.maxX)*0.5;
	RgnCp.y = (LeadRegion.minY + LeadRegion.maxY)*0.5;
	szBody.cx = ::fabs(RgnBody.maxX - RgnBody.minX);
	szBody.cy = ::fabs(RgnBody.maxY - RgnBody.minY);

	BodyMarginX = MIN(szBody.cx*0.2, szBody.cy*0.2);//szBody.cx*0.2;
	BodyMarginY = MIN(szBody.cx*0.2, szBody.cy*0.2);//szBody.cy*0.2;

	const BOX_TOWARD ModelToward = GetModelBodyBox().GetBoxToward();
	if (LAND_TYPE_DIP_LEAD == LandType)
	{
		LandToward = ModelToward;
	}
	else
	{
		if (RgnCp.x < (RgnBody.minX))
		{
			LandToward = BOX_TOWARD_LEFT;
		}
		else if (RgnCp.x >(RgnBody.maxX))
		{
			LandToward = BOX_TOWARD_RIGHT;
		}
		else if (RgnCp.y > (RgnBody.maxY))
		{
			LandToward = BOX_TOWARD_UP;
		}
		else if (RgnCp.y < (RgnBody.minY))
		{
			LandToward = BOX_TOWARD_DOWN;
		}
		else if (RgnCp.x < (RgnBody.minX + BodyMarginX))
		{
			LandToward = BOX_TOWARD_LEFT;
		}
		else if (RgnCp.x >(RgnBody.maxX - BodyMarginX))
		{
			LandToward = BOX_TOWARD_RIGHT;
		}
		else if (RgnCp.y > (RgnBody.maxY - BodyMarginY))
		{
			LandToward = BOX_TOWARD_UP;
		}
		else if (RgnCp.y < (RgnBody.minY + BodyMarginY))
		{
			LandToward = BOX_TOWARD_DOWN;
		}
		else
		{
			switch (ModelToward)
			{
			case BOX_TOWARD_LEFT:
			case BOX_TOWARD_RIGHT:
				if (RgnCp.x > BodyCp.x)
				{
					LandToward = BOX_TOWARD_RIGHT;
				}
				else
				{
					LandToward = BOX_TOWARD_LEFT;
				}
				break;
			case BOX_TOWARD_UP:
			case BOX_TOWARD_DOWN:
				if (RgnCp.y > BodyCp.y)
				{
					LandToward = BOX_TOWARD_UP;
				}
				else
				{
					LandToward = BOX_TOWARD_DOWN;
				}
				break;
			}
		}
	}

	LandPtr = AOIObjManager.CreateLandObj();
	if (NULL == LandPtr)
	{
		return LandPtr;
	}

	
	//const int LanGroupID = GetModelLandFreeGroupID();

	SetModelLandActived(NULL);
	LandPtr->SetLandToward(LandToward);
	LandPtr->SetLandGroupID(groupID);
	LandPtr->SwitchLandType(LandType);

	if (LAND_TYPE_PAD == LandType)
	{

	}
	else if (LAND_TYPE_ELECTRODE == LandType)
	{
		
	}
	else if (LAND_TYPE_IC_LEAD == LandType || LAND_TYPE_CON_LEAD == LandType)
	{
		TREGION4D  leadRegion = LeadRegion; //LeadRegion Y min max是反的
		if (leadRegion.minY > leadRegion.maxY)
		{
			leadRegion.minY = LeadRegion.maxY;
			leadRegion.maxY = LeadRegion.minY;
		}
		TREGION4D  PRegion = PadRegion; //LeadRegion Y min max是反的
		if (PRegion.minY > PRegion.maxY)
		{
			PRegion.minY = PadRegion.maxY;
			PRegion.maxY = PadRegion.minY;
		}

		RgnLeadTip = RgnLeadShoulder = leadRegion;
		szLead.cx = ::fabs(leadRegion.maxX - leadRegion.minX);
		szLead.cy = ::fabs(leadRegion.maxY - leadRegion.minY);
		
		switch (LandToward)
		{
		case BOX_TOWARD_UP:

			RgnLeadTip.minY = RgnLeadTip.maxY - (szLead.cy*0.4);
			RgnLeadShoulder.maxY = RgnLeadShoulder.minY + (szLead.cy*0.4);

			break;
		case BOX_TOWARD_LEFT:

			RgnLeadTip.maxX = RgnLeadTip.minX + (szLead.cx*0.4);
			RgnLeadShoulder.minX = RgnLeadShoulder.maxX - (szLead.cx*0.4);
			break;
		case BOX_TOWARD_DOWN:

			RgnLeadTip.maxY = RgnLeadTip.minY + (szLead.cy*0.4);
			RgnLeadShoulder.minY = RgnLeadShoulder.maxY - (szLead.cy*0.4);
			break;
		case BOX_TOWARD_RIGHT:

			RgnLeadTip.minX = RgnLeadTip.maxX - (szLead.cx*0.4);
			RgnLeadShoulder.maxX = RgnLeadShoulder.minX + (szLead.cx*0.4);
			break;
		}

		LandPtr->GetLandLeadBox().SetBoxRegion(leadRegion);
		LandPtr->GetLandPadBox().SetBoxRegion(PRegion);
		LandPtr->GetLandLeadTipBox().SetBoxRegion(RgnLeadTip);
		LandPtr->GetLandLeadShoulderBox().SetBoxRegion(RgnLeadShoulder);
		LandPtr->GetLandBoxPtr()->SetBoxActived(true);
		LandPtr->GetLandBoxPtr()->SetBoxSelected(true);
	}
	else if (LAND_TYPE_DIP_LEAD == LandType)
	{
	}

	if (true == IsExceptionAngle)
	{
		LandPtr->RotateLand(AttachedAngle, ModelCp.x, ModelCp.y);
	}

	switch (ModelType)
	{
	case MODEL_TYPE_BGA:
	case MODEL_TYPE_GOLD_FINGER:
		LandToward = GetModelBodyBox().GetBoxToward();
		LandPtr->SetLandToward(LandToward);
		break;
	}
	LandPtr->SetLandAttachedAngle(AttachedAngle);
	LandPtr->SetLandAttachedPosCad(m_ModelAttachedCadPos);
	LandPtr->SetLandAttachedPosStage(m_ModelAttachedStagePos);
	return LandPtr;
}
//-------------------------------------------------------------------------------------//
CAOILand* CAOIModel::GetModelLandPtr(size_t index, bool Check) const
{
	if ( Check )
	{
		const size_t size = GetModelLandCount_Inline();
		if ( index >= size ) 
		{	return NULL; }
	}
	return GetModelLandPtr_Inline(index);	
}
//-------------------------------------------------------------------------------------//
CAOILand* CAOIModel::GetModelLandPtrByUUID(const UUID &uuid) const
{
	size_t       i=0;
	UUID         LandUUID;
	UUID         RefUUID = uuid;
	RPC_STATUS   Status=0;	
	CAOILand    *LandPtr = NULL;
	const size_t LandCount = GetModelLandCount_Inline();	
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = GetModelLandPtr_Inline(i);
		if ( NULL == LandPtr ) { continue; }
		LandUUID = LandPtr->GetObjUuid();
		if ( ::UuidEqual(&LandUUID, &RefUUID, &Status) == FALSE ) { continue; }				
		return LandPtr;
	}
	return NULL;
}
//-------------------------------------------------------------------------------------//
CAOILand* CAOIModel::GetModelLandPtrByGroupID(int LandGroupID, int LandAlignID) const
{
	size_t i=0;
	CAOILand    *LandPtr = NULL;
	const size_t LandCount = GetModelLandCount_Inline();	
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = GetModelLandPtr_Inline(i);
		if ( NULL == LandPtr ) { continue; }
		if ( LandPtr->GetLandGroupID() != LandGroupID ) { continue; }		
		if ( LandAlignID >= 0 ) 
		{
			if ( LandPtr->GetLandAlignID() != LandAlignID ) { continue; }
		}
		return LandPtr;
	}
	return NULL;
}
//-------------------------------------------------------------------------------------//
CAOILand* CAOIModel::GetModelLandPtrByGroupID(int LandGroupID, int LandAlignID, BOX_TOWARD LandToward) const
{
	size_t i=0;	
	CAOILand    *LandPtr = NULL;	
	const size_t LandCount = GetModelLandCount_Inline();	

	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = GetModelLandPtr_Inline(i);
		if ( NULL == LandPtr ) { continue; }
		if ( LandPtr->GetLandGroupID() != LandGroupID ) { continue; }		
		if ( LandAlignID >= 0 ) 
		{
			if ( LandPtr->GetLandAlignID() != LandAlignID ) { continue; }
		}
		if ( BOX_TOWARD_NULL != LandToward )
		{
			if ( LandPtr->GetLandToward() != LandToward ) { continue; }
		}
		return LandPtr;		
	}
	return NULL;
}
//-------------------------------------------------------------------------------------//
CAOILand* CAOIModel::GetModelLandPtrByGroupID(int LandGroupID, int LandAlignID, BOX_TOWARD LandToward, bool bMaxWnd) const
{
	if ( false == bMaxWnd )
	{	return GetModelLandPtrByGroupID(LandGroupID, LandAlignID, LandToward);	}

	size_t i=0;
	size_t WndCnt=0;
	size_t MaxWndCnt=0;
	CAOILand    *LandPtr = NULL;
	CAOILand    *LandPtr_MaxWndCnt = NULL;
	const size_t LandCount = GetModelLandCount_Inline();	

	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = GetModelLandPtr_Inline(i);
		if ( NULL == LandPtr ) { continue; }
		if ( LandPtr->GetLandGroupID() != LandGroupID ) { continue; }		
		if ( LandAlignID >= 0 ) 
		{
			if ( LandPtr->GetLandAlignID() != LandAlignID ) { continue; }
		}
		if ( BOX_TOWARD_NULL != LandToward )
		{
			if ( LandPtr->GetLandToward() != LandToward ) { continue; }
		}
		WndCnt=LandPtr->GetLandWndCount();
		if ( NULL==LandPtr_MaxWndCnt || MaxWndCnt<WndCnt )
		{
			MaxWndCnt = WndCnt;
			LandPtr_MaxWndCnt = LandPtr;
		}		
	}
	return LandPtr_MaxWndCnt;
}
//-------------------------------------------------------------------------------------//
CAOILand* CAOIModel::GetModelLandActivted() const
{
#ifdef _DEBUG
	size_t actCount = GetModelLandActivtedCount();
	if ( actCount > 1 ) 
	{	actCount = actCount; }
#endif _DEBUG

	size_t i=0;
	CAOILand    *LandPtr = NULL;
	const size_t LandCount = GetModelLandCount_Inline();	
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = GetModelLandPtr_Inline(i);	
		if ( NULL == LandPtr ) { continue; }
		if ( LandPtr->GetLandActived() == true )
		{	return LandPtr; }
	}
	return NULL;
}
//-------------------------------------------------------------------------------------//
size_t  CAOIModel::GetModelLandActivtedCount() const
{
	size_t       i=0;
	size_t       Count=0;
	CAOILand    *LandPtr = NULL;
	const size_t LandCount = GetModelLandCount_Inline();	

	Count=0;
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = GetModelLandPtr_Inline(i);	
		if ( NULL == LandPtr ) { continue; }
		if ( LandPtr->GetLandActived() == false ) { continue; }
		Count ++;
	}
	return Count;
}
//-------------------------------------------------------------------------------------//
void CAOIModel::SetModelLandActived(CAOILand *ActLandPtr)
{
	size_t i=0;
	CAOILand    *LandPtr = NULL;
	const size_t LandCount = GetModelLandCount_Inline();
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = (GetModelLandPtr_Inline(i));
		if ( NULL == LandPtr ) { continue; }
		LandPtr->SetLandAllBoxActived(false);
	}

	if ( NULL != ActLandPtr )
	{			
		ActLandPtr->GetLandBoxPtr()->SetBoxActived(true);
		ActLandPtr->GetLandBoxPtr()->SetBoxSelected(true);
		ActLandPtr->GetLandBoxPtr()->SetBoxVisibled(true);
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CAOIModel::SetModelLandSelected(bool val)
{
	size_t i=0;
	CAOILand    *LandPtr = NULL;
	const size_t LandCount = GetModelLandCount_Inline();
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = (GetModelLandPtr_Inline(i));
		if ( NULL == LandPtr ) { continue; }
		LandPtr->SetLandAllBoxSelected(val);
	}
	return;
}
//-------------------------------------------------------------------------------------//
CAOILand* CAOIModel::GetModelLandSelected() const
{
	size_t i=0;
	CAOILand    *LandPtr = NULL;
	const size_t LandCount = GetModelLandCount_Inline();	
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = GetModelLandPtr_Inline(i);	
		if ( NULL == LandPtr ) { continue; }
		if ( LandPtr->GetLandSelected() == true )
		{	return LandPtr; }
	}
	return NULL;
}
//-------------------------------------------------------------------------------------//
size_t CAOIModel::GetModelLandSelectedCount() const
{	
	size_t       i=0;
	size_t       Count=0;
	CAOILand    *LandPtr = NULL;
	const size_t LandCount = GetModelLandCount_Inline();	

	Count=0;
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = GetModelLandPtr_Inline(i);
		if ( NULL == LandPtr ) { continue; }		
		if ( LandPtr->GetLandSelected() == false )
		{	continue; }
		Count ++;
	}	
	return Count;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::GetModelLandSelectedList(std::vector<CAOILand*> &LandList)
{
	size_t       i=0, j=0;	
	CAOILand    *LandPtr = NULL;	
	const size_t LandCount = GetModelLandCount_Inline();

	LandList.clear();
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = GetModelLandPtr_Inline(i);
		if ( NULL == LandPtr ) { continue; }		
		if ( LandPtr->GetLandSelected() == false )
		{	continue; }
		LandList.push_back(LandPtr);
	}		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::GetModelLandSelectedList(int LandGroupID, std::vector<CAOILand*> &LandList)
{
	size_t       i=0, j=0;	
	CAOILand    *LandPtr = NULL;	
	const size_t LandCount = GetModelLandCount_Inline();

	LandList.clear();
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = GetModelLandPtr_Inline(i);
		if ( NULL == LandPtr ) { continue; }
		if ( LandPtr->GetLandGroupID() != LandGroupID ) { continue; }
		if ( LandPtr->GetLandSelected() == false )
		{	continue; }
		LandList.push_back(LandPtr);
	}		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::GetModelLandSelectedIndexList(int LandGroupID, std::vector<size_t> &LandIndexList)
{
	size_t       i=0, j=0;	
	CAOILand    *LandPtr = NULL;	
	const size_t LandCount = GetModelLandCount_Inline();

	LandIndexList.clear();
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = GetModelLandPtr_Inline(i);
		if ( NULL == LandPtr ) { continue; }
		if ( LandPtr->GetLandGroupID() != LandGroupID ) { continue; }
		if ( LandPtr->GetLandSelected() == false )
		{	continue; }
		LandIndexList.push_back(i);
	}		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::GetModelLandSelectedLandGroupIDList(std::vector<int> &LandGroupIDList)
{
	size_t       i=0, j=0;
	int          LandGroupID = 0;
	CAOILand    *LandPtr = NULL;
	size_t       LandGroupIDCount = 0;
	const size_t LandCount = CAOIModel::GetModelLandCount_Inline();	

	LandGroupIDList.clear();
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = CAOIModel::GetModelLandPtr_Inline(i);
		if ( NULL == LandPtr ) { continue; }
		if ( LandPtr->GetLandSelected() == false )
		{	continue; }
		
		LandGroupID = LandPtr->GetLandGroupID();
		LandGroupIDCount = LandGroupIDList.size();
		for ( j=0; j<LandGroupIDCount; j++ )
		{
			if ( LandGroupIDList[j] == LandGroupID )
			{	break; }
		}
		if ( j != LandGroupIDCount ) { continue; }
		LandGroupIDList.push_back(LandGroupID);
	}		
	return true;
}
//-------------------------------------------------------------------------------------//	
bool CAOIModel::ListModelLandAlignIDByGroupID(int LandGroupID, std::vector<int> &LandAlignIDList)
{
	size_t       i=0, j=0;
	int          LandAlignID = 0;
	CAOILand    *LandPtr = NULL;
	size_t       LandAlignIDCount = 0;
	const size_t LandCount = GetModelLandCount_Inline();	

	LandAlignIDList.clear();
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = GetModelLandPtr_Inline(i);
		if ( NULL == LandPtr ) { continue; }
		if ( LandPtr->GetLandGroupID() != LandGroupID ) { continue; }
		
		LandAlignID = LandPtr->GetLandAlignID();
		LandAlignIDCount = LandAlignIDList.size();
		for ( j=0; j<LandAlignIDCount; j++ )
		{
			if ( LandAlignIDList[j] == LandAlignID )
			{	break; }
		}
		if ( j != LandAlignIDCount ) { continue; }
		LandAlignIDList.push_back(LandAlignID);
	}	
	return true;
}
//-------------------------------------------------------------------------------------//	
bool  CAOIModel::ListModelLandGroupIDByLandType(LAND_TYPE LandType, std::vector<int> &LandGroupIDList)
{
	size_t       i=0, j=0;
	int          LandGroupID = 0;
	CAOILand    *LandPtr = NULL;
	size_t       LandGroupIDCount = 0;
	const size_t LandCount = GetModelLandCount_Inline();	

	LandGroupIDList.clear();
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = GetModelLandPtr_Inline(i);
		if ( NULL == LandPtr ) { continue; }
		if ( LandPtr->GetLandType() != LandType ) { continue; }
		
		LandGroupID = LandPtr->GetLandGroupID();
		LandGroupIDCount = LandGroupIDList.size();
		for ( j=0; j<LandGroupIDCount; j++ )
		{
			if ( LandGroupIDList[j] == LandGroupID )
			{	break; }
		}
		if ( j != LandGroupIDCount ) { continue; }
		LandGroupIDList.push_back(LandGroupID);
	}	
	return true;
}
//-------------------------------------------------------------------------------------//	
double  CAOIModel::CalcModelLandLeadAverageHeight(int LandGroupID)//計算模組引腳平均高度
{
	size_t       i=0;	
	int          Cnt=0;
	double       Sum=0;
	double       Ave=0;
	CAOILand    *LandPtr = NULL;	
	const size_t LandCount = CAOIModel::GetModelLandCount_Inline();	
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = CAOIModel::GetModelLandPtr_Inline(i);
		if ( NULL == LandPtr ) { continue; }
		if ( LandPtr->GetLandGroupID() != LandGroupID ) { continue; }		

		Sum += LandPtr->GetLandLeadHeight();
		Cnt ++;
	}		
	if ( Cnt > 0 ) 
	{	Ave = Sum/Cnt;	}
	return Ave;
}
//-------------------------------------------------------------------------------------//	
double CAOIModel::CalcModelLandLeadTipAverageHeight(int LandGroupID)//計算模組引腳前端平均高度
{
	size_t       i=0;	
	int          Cnt=0;
	double       Sum=0;
	double       Ave=0;
	CAOILand    *LandPtr = NULL;	
	const size_t LandCount = CAOIModel::GetModelLandCount_Inline();	
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = CAOIModel::GetModelLandPtr_Inline(i);
		if ( NULL == LandPtr ) { continue; }
		if ( LandPtr->GetLandGroupID() != LandGroupID ) { continue; }		

		Sum += LandPtr->GetLandLeadTipHeight();
		Cnt ++;
	}		
	if ( Cnt > 0 ) 
	{	Ave = Sum/Cnt;	}
	return Ave;
}
//-------------------------------------------------------------------------------------//	
double CAOIModel::CalcModelLandLeadShoulderAverageHeight(int LandGroupID)//計算模組引腳根部平均高度
{
	size_t       i=0;	
	int          Cnt=0;
	double       Sum=0;
	double       Ave=0;
	CAOILand    *LandPtr = NULL;	
	const size_t LandCount = CAOIModel::GetModelLandCount_Inline();	
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = CAOIModel::GetModelLandPtr_Inline(i);
		if ( NULL == LandPtr ) { continue; }
		if ( LandPtr->GetLandGroupID() != LandGroupID ) { continue; }		

		Sum += LandPtr->GetLandLeadShoulderHeight();
		Cnt ++;
	}		
	if ( Cnt > 0 ) 
	{	Ave = Sum/Cnt;	}
	return Ave;
}
//-------------------------------------------------------------------------------------//	
int CAOIModel::GetModelLandFreeGroupID() const
{
	size_t       i = 0;
	int          MaxGroupID=-1;
	CAOILand    *LandPtr = NULL;
	const size_t LandCount = CAOIModel::GetModelLandCount_Inline();
	for ( i=0; i<LandCount; i++ )
	{	
		LandPtr = CAOIModel::GetModelLandPtr_Inline(i);
		if ( NULL == LandPtr ) { continue; }
		if ( MaxGroupID < LandPtr->GetLandGroupID() )
		{	MaxGroupID = LandPtr->GetLandGroupID();	}
	}
	return (MaxGroupID+1);
}
//-------------------------------------------------------------------------------------//
int CAOIModel::GetModelLandFreeAlignID(int LandGroupID) const
{
	size_t       i = 0;
	int          MaxAlignID=-1;
	CAOILand    *LandPtr = NULL;
	const size_t LandCount = CAOIModel::GetModelLandCount_Inline();
	for ( i=0; i<LandCount; i++ )
	{	
		LandPtr = CAOIModel::GetModelLandPtr_Inline(i);
		if ( NULL == LandPtr ) { continue; }
		if ( LandPtr->GetLandGroupID() != LandGroupID ) { continue; }
		if ( MaxAlignID < LandPtr->GetLandAlignID() )
		{	MaxAlignID = LandPtr->GetLandAlignID();	}
	}
	return (MaxAlignID+1);
}
//-------------------------------------------------------------------------------------//
void CAOIModel::SetModelLandSelectedByLandGroupID(int LandGroupID, int LandAlignID, bool Selected)
{
	size_t i=0;
	CAOILand    *LandPtr = NULL;
	const size_t LandCount = GetModelLandCount_Inline();
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = (GetModelLandPtr_Inline(i));
		if ( NULL == LandPtr ) { continue; }
		if ( LandPtr->GetLandGroupID() != LandGroupID ) { continue; }
		if ( LandAlignID >= 0 ) 
		{
			if ( LandPtr->GetLandAlignID() != LandAlignID ) { continue; }
		}
		LandPtr->GetLandBoxPtr()->SetBoxSelected(Selected);		
	}
	return;
}
//-------------------------------------------------------------------------------------//
CAOILand* CAOIModel::GetModelLandSelectedByLandGroupID(int LandGroupID, int LandAlignID) const
{
	size_t i=0;
	CAOILand    *LandPtr = NULL;
	const size_t LandCount = CAOIModel::GetModelLandCount_Inline();	
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = CAOIModel::GetModelLandPtr_Inline(i);	
		if ( NULL == LandPtr ) { continue; }
		if ( LandPtr->GetLandGroupID() != LandGroupID ) { continue; }
		if ( LandAlignID >= 0 ) 
		{
			if ( LandPtr->GetLandAlignID() != LandAlignID ) { continue; }
		}
		if ( LandPtr->GetLandSelected() == true )
		{	return LandPtr; }
	}
	return NULL;
}
//-------------------------------------------------------------------------------------//
void CAOIModel::SetModelLandVisibledByLandGroupID(int LandGroupID, int LandAlignID, bool Visibled)
{
	size_t i=0;
	CAOILand    *LandPtr = NULL;
	const size_t LandCount = CAOIModel::GetModelLandCount_Inline();
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = (CAOIModel::GetModelLandPtr_Inline(i));
		if ( NULL == LandPtr ) { continue; }
		if ( LandPtr->GetLandGroupID() != LandGroupID ) { continue; }
		if ( LandAlignID >= 0 ) 
		{
			if ( LandPtr->GetLandAlignID() != LandAlignID ) { continue; }
		}
		LandPtr->SetLandAllBoxVisibled(Visibled);		
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CAOIModel::UnSelectModelLand(bool ToWnd)
{
	size_t       i = 0;			
	CAOILand    *LandPtr = NULL;
	const size_t LandCount = GetModelLandCount_Inline();
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = GetModelLandPtr_Inline(i);
		if ( NULL == LandPtr ) { continue; }
		LandPtr->UnSelectLand(ToWnd);
		LandPtr->SetLandAllBoxActived(false);
	}
}
//-------------------------------------------------------------------------------------//
void CAOIModel::VisibleModelLand(bool ToWnd)
{
	size_t       i = 0;			
	CAOILand    *LandPtr = NULL;
	const size_t LandCount = CAOIModel::GetModelLandCount_Inline();
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = CAOIModel::GetModelLandPtr_Inline(i);
		if ( NULL == LandPtr ) { continue; }
		LandPtr->VisibleLand(ToWnd);
	}
}
//-------------------------------------------------------------------------------------//
void CAOIModel::InvisibleModelLand(bool ToWnd)
{
	size_t       i = 0;			
	CAOILand    *LandPtr = NULL;
	const size_t LandCount = CAOIModel::GetModelLandCount_Inline();
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = CAOIModel::GetModelLandPtr_Inline(i);
		if ( NULL == LandPtr ) { continue; }
		LandPtr->InvisibleLand(ToWnd);
	}
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::CheckModelLandModified()
{
	size_t       i = 0;			
	CAOILand    *LandPtr = NULL;
	const size_t LandCount = GetModelLandCount_Inline();
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = GetModelLandPtr_Inline(i);
		if ( NULL == LandPtr ) { continue; }
		if ( LandPtr->GetLandModified() == false ) { continue; }
		return true;
	}
	return false;
}
//-------------------------------------------------------------------------------------//
void CAOIModel::SetModelLandModified(bool value)
{
	size_t       i = 0;			
	CAOILand    *LandPtr = NULL;
	const size_t LandCount = GetModelLandCount_Inline();
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = GetModelLandPtr_Inline(i);
		if ( NULL == LandPtr ) { continue; }
		LandPtr->SetLandModified(value);
	}
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::CheckModelLandValid(const CAOILand *RefLandPtr)//確認特徵框指標有效-屬於此模組內
{
	if ( NULL == RefLandPtr ) { return false; }

	size_t       i = 0;			
	CAOILand    *LandPtr = NULL;
	const size_t LandCount = CAOIModel::GetModelLandCount_Inline();
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = CAOIModel::GetModelLandPtr_Inline(i);
		if ( NULL == LandPtr ) { continue; }
		if ( RefLandPtr == LandPtr )
		{	return true; }
	}
	return false;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::CheckModelLandOnlyOneGroupIDSelected() const//確認只有一個特徵框群組被選到
{
	size_t       i = 0;			
	int          LandGorupID = -1;
	CAOILand    *LandPtr = NULL;
	const size_t LandCount = CAOIModel::GetModelLandCount_Inline();
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = CAOIModel::GetModelLandPtr_Inline(i);
		if ( NULL == LandPtr ) { continue; }		
		if ( LandPtr->GetLandSelected() == false ) { continue; }

		if ( LandGorupID < 0 )
		{	LandGorupID = LandPtr->GetLandGroupID(); }
		else
		{
			if ( LandGorupID != LandPtr->GetLandGroupID() )
			{	return false; }
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::ApplyModelLandSize(CAOILand *RefLandPtr)//同步化特徵框的尺寸
{
	bool IsOK = true;	
	const double ComponentAngle = CAOIModel::GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComponentAngle);
	if ( IsExceptionAngle == false )
	{
		IsOK = CAOIModel::ApplyModelLandSizeKernel(RefLandPtr);
	}
	else
	{
		const double CPX = 0;
		const double CPY = 0;
		CAOIModel::RotateModel(-ComponentAngle, CPX, CPY);
		IsOK = CAOIModel::ApplyModelLandSizeKernel(RefLandPtr);
		CAOIModel::RotateModel(ComponentAngle, CPX, CPY);
	}	
	return IsOK;
}
//-------------------------------------------------------------------------------------//
inline bool CAOIModel::ApplyModelLandSizeKernel(CAOILand *RefLandPtr)//同步化特徵框的尺寸
{
	if ( NULL == RefLandPtr ) { return false; }
	size_t     i=0, j=0;	
	int        TowardAngle = 0;	
	CAOIBox   *BoxPtr = NULL;
	CAOILand  *LandPtr = NULL;	
	CAOIBox   *RefBoxPtr = NULL;
	BOX_TOWARD LandToward = BOX_TOWARD_NULL;
	
	TSIZE2D                    BoxSize;
	TSIZE2D                    RefBoxSize;
	BOX_TOWARD RefLandToward = RefLandPtr->GetLandToward();
	const int  RefLandGroupID = RefLandPtr->GetLandGroupID();
	const int  RefLandAlignID = RefLandPtr->GetLandAlignID();
	const size_t LandCount = CAOIModel::GetModelLandCount_Inline();
	
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = CAOIModel::GetModelLandPtr_Inline(i);
		if ( NULL == LandPtr ) { continue; }
		if ( LandPtr == RefLandPtr ) { continue; }		
		if ( LandPtr->GetLandGroupID() != RefLandGroupID ) { continue; }
		if ( LandPtr->GetLandAlignID() != RefLandAlignID ) { continue; }

		LandToward = LandPtr->GetLandToward();
		TowardAngle = CAOIBox::CalcBoxTowardAngle(RefLandToward, LandToward);				
		for ( j=0; j<5; j++ )
		{
			switch ( j )
			{
			case 0:
				BoxPtr    = LandPtr->GetLandPadBoxPtr();
				RefBoxPtr = RefLandPtr->GetLandPadBoxPtr();
				break;
			case 1:
				BoxPtr    = LandPtr->GetLandLeadBoxPtr();
				RefBoxPtr = RefLandPtr->GetLandLeadBoxPtr();
				break;
			case 2:
				BoxPtr    = LandPtr->GetLandLeadTipBoxPtr();
				RefBoxPtr = RefLandPtr->GetLandLeadTipBoxPtr();
				break;
			case 3:				
				BoxPtr    = LandPtr->GetLandLeadShoulderBoxPtr();
				RefBoxPtr = RefLandPtr->GetLandLeadShoulderBoxPtr();
				break;
			case 4:				
				BoxPtr    = LandPtr->GetLandBodyEdgeBoxPtr();
				RefBoxPtr = RefLandPtr->GetLandBodyEdgeBoxPtr();
				break;
			default:
				RefBoxPtr = BoxPtr = NULL;
				break;
			}
			if ( NULL == RefBoxPtr ) { continue; }

			RefBoxPtr->GetBoxSize(RefBoxSize);
			switch ( TowardAngle )
			{
			case  90:
			case 270:
				BoxSize.cx = RefBoxSize.cy;
				BoxSize.cy = RefBoxSize.cx;
				break;
			default:
				BoxSize = RefBoxSize;
				break;
			}			
			BoxPtr->SetBoxSize(BoxSize);
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::ApplyModelLandRegion(CAOILand *RefLandPtr)
{
	bool IsOK = true;	
	const double ComponentAngle = CAOIModel::GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComponentAngle);
	if ( IsExceptionAngle == false )
	{
		IsOK = CAOIModel::ApplyModelLandRegionKernel(RefLandPtr);
	}
	else
	{
		const double CPX = 0;
		const double CPY = 0;
		CAOIModel::RotateModel(-ComponentAngle, CPX, CPY);
		IsOK = CAOIModel::ApplyModelLandRegionKernel(RefLandPtr);
		CAOIModel::RotateModel(ComponentAngle, CPX, CPY);
	}	
	return IsOK;
}
//-------------------------------------------------------------------------------------//
inline bool CAOIModel::ApplyModelLandRegionKernel(CAOILand *RefLandPtr)
{
	if ( NULL == RefLandPtr ) { return false; }

	size_t     i=0, j=0, k=0;		
	CAOIBox   *BoxPtr = NULL;
	CAOILand  *LandPtr = NULL;	
	CAOIBox   *RefBoxPtr = NULL;
	BOX_TOWARD LandToward = BOX_TOWARD_NULL;
	
	TREGION4D                  BoxRgn;
	TPOINT2D                   dPos;
	TPOINT2D                   LandPos;
	TPOINT2D                   RefLandPos;	
	BOX_TOWARD RefLandToward = RefLandPtr->GetLandToward();
	const int  RefLandGroupID = RefLandPtr->GetLandGroupID();
	const int  RefLandAlignID = RefLandPtr->GetLandAlignID();
	const size_t LandCount = CAOIModel::GetModelLandCount_Inline();

	RefLandPtr->GetLandBoxPtr()->GetBoxPos(RefLandPos);
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = CAOIModel::GetModelLandPtr_Inline(i);
		if ( NULL == LandPtr ) { continue; }
		if ( LandPtr == RefLandPtr ) { continue; }
		if ( LandPtr->GetLandToward() != RefLandToward ) { continue; }
		if ( LandPtr->GetLandGroupID() != RefLandGroupID ) { continue; }
		if ( LandPtr->GetLandAlignID() != RefLandAlignID ) { continue; }

		LandPtr->GetLandBoxPtr()->GetBoxPos(LandPos);
		dPos.x = LandPos.x - RefLandPos.x;
		dPos.y = LandPos.y - RefLandPos.y;
		switch ( RefLandToward )
		{
		case BOX_TOWARD_UP:
		case BOX_TOWARD_DOWN:
			dPos.y = 0;
			break;
		case BOX_TOWARD_LEFT:
		case BOX_TOWARD_RIGHT:
			dPos.x = 0;
			break;
		}
		for ( j=0; j<5; j++ )
		{
			switch ( j )
			{
			case 0:
				BoxPtr    = LandPtr->GetLandPadBoxPtr();
				RefBoxPtr = RefLandPtr->GetLandPadBoxPtr();
				break;
			case 1:
				BoxPtr    = LandPtr->GetLandLeadBoxPtr();
				RefBoxPtr = RefLandPtr->GetLandLeadBoxPtr();
				break;
			case 2:
				BoxPtr    = LandPtr->GetLandLeadTipBoxPtr();
				RefBoxPtr = RefLandPtr->GetLandLeadTipBoxPtr();
				break;
			case 3:				
				BoxPtr    = LandPtr->GetLandLeadShoulderBoxPtr();
				RefBoxPtr = RefLandPtr->GetLandLeadShoulderBoxPtr();
				break;
			case 4:				
				BoxPtr    = LandPtr->GetLandBodyEdgeBoxPtr();
				RefBoxPtr = RefLandPtr->GetLandBodyEdgeBoxPtr();
				break;
			default:
				RefBoxPtr = BoxPtr = NULL;
				break;
			}
			if ( NULL == RefBoxPtr ) { continue; }
			RefBoxPtr->GetBoxRegion(BoxRgn);
			BoxRgn.minX += dPos.x;
			BoxRgn.minY += dPos.y;
			BoxRgn.maxX += dPos.x;
			BoxRgn.maxY += dPos.y;
			BoxPtr->SetBoxRegion(BoxRgn);
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::ApplyModelLandWndLogic(CAOILand *RefLandPtr)
{
	bool IsOK = true;	
	const double ComponentAngle = CAOIModel::GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComponentAngle);
	if ( IsExceptionAngle == false )
	{
		IsOK = CAOIModel::ApplyModelLandWndLogicKernel(RefLandPtr);
	}
	else
	{
		const double CPX = 0;
		const double CPY = 0;
		CAOIModel::RotateModel(-ComponentAngle, CPX, CPY);
		IsOK = CAOIModel::ApplyModelLandWndLogicKernel(RefLandPtr);
		CAOIModel::RotateModel(ComponentAngle, CPX, CPY);
	}	
	return IsOK;
}
//-------------------------------------------------------------------------------------//
inline bool CAOIModel::ApplyModelLandWndLogicKernel(CAOILand *RefLandPtr)
{
	if ( NULL == RefLandPtr ) { return false; }
	
	int        WndBandID = 0;
	int        WndGroupID = 0;
	int        TowardAngle = 0;
	size_t     i=0, j=0, k=0;
	size_t     LandWndCount = 0;
	size_t     LogicWndCount = 0;
	size_t     LandLogicCount = 0;

	CAOIWnd   *WndPtr = NULL;
	CAOILand  *LandPtr = NULL;
	CAOILogic *LogicPtr = NULL;
	CAOIWnd   *NewWndPtr = NULL;
	CAOILogic *NewLogicPtr = NULL;
	BOX_TOWARD LandToward = BOX_TOWARD_NULL;
	
	TPOINT2D                   LandPos;
	TPOINT2D                   RefLandPos;
	std::vector<unsigned int>  WndIndexList;
	std::vector<unsigned int>  LogicIndexList;
	BOX_TOWARD RefLandToward = RefLandPtr->GetLandToward();
	const int  RefLandGroupID = RefLandPtr->GetLandGroupID();
	const int  RefLandAlignID = RefLandPtr->GetLandAlignID();
	const size_t LandCount = CAOIModel::GetModelLandCount_Inline();
	
	WndIndexList.clear();
	LogicIndexList.clear();
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = CAOIModel::GetModelLandPtr_Inline(i);
		if ( NULL == LandPtr ) { continue; }
		if ( LandPtr == RefLandPtr ) { continue; }
		if ( LandPtr->GetLandGroupID() != RefLandGroupID ) { continue; }		
		
		LandWndCount = LandPtr->GetLandWndCount();
		for ( j=0; j<LandWndCount; j++ )
		{
			WndPtr = LandPtr->GetLandWndPtr(j, false);
			if ( NULL == WndPtr ) { continue; }
			WndIndexList.push_back(WndPtr->GetWndIndex());
		}

		LandLogicCount = LandPtr->GetLandLogicCount();
		for ( j=0; j<LandLogicCount; j++ )
		{
			LogicPtr = LandPtr->GetLandLogicPtr(j, false);
			if ( NULL == LogicPtr ) { continue; }
			LogicIndexList.push_back(LogicPtr->GetLogicIndex());
		}
	}

	LandWndCount = WndIndexList.size();
	if ( LandWndCount > 0 )
	{	CAOIModel::DeleteModelWndIndexList(WndIndexList); }

	LandLogicCount = LogicIndexList.size();
	if ( LandLogicCount > 0 ) 
	{	CAOIModel::DeleteModelLogicIndexList(LogicIndexList); }	

	RefLandPtr->GetLandBoxPtr()->GetBoxPos(RefLandPos);
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = CAOIModel::GetModelLandPtr_Inline(i);
		if ( NULL == LandPtr ) { continue; }
		if ( LandPtr == RefLandPtr ) { continue; }
		if ( LandPtr->GetLandGroupID() != RefLandGroupID ) { continue; }				
		
		LandToward = LandPtr->GetLandToward();
		LandPtr->GetLandBoxPtr()->GetBoxPos(LandPos);
		TowardAngle = CAOIBox::CalcBoxTowardAngle(RefLandToward, LandToward);
		LandWndCount = RefLandPtr->GetLandWndCount();
		for ( j=0; j<LandWndCount; j++ )
		{
			WndPtr = RefLandPtr->GetLandWndPtr(j, false);
			if ( NULL == WndPtr ) { continue; }
			NewWndPtr = WndPtr->CloneWndObj();
			if ( NULL == NewWndPtr ) { return false; }

			NewWndPtr->MoveWnd(-RefLandPos.x, -RefLandPos.y);//移回特徵框原點
			NewWndPtr->RotateWnd(TowardAngle, 0, 0);//旋轉
			NewWndPtr->MoveWnd(LandPos);//移回特徵框原點
			CAOIModel::AddModelWndPtr(NewWndPtr, false);
			LandPtr->AddLandWndPtr(NewWndPtr);
		}

		WndIndexList.clear();
		LandLogicCount = RefLandPtr->GetLandLogicCount();
		for ( j=0; j<LandLogicCount; j++ )
		{
			LogicPtr = RefLandPtr->GetLandLogicPtr(j, false);
			if ( NULL == LogicPtr ) { continue; }
			NewLogicPtr = LogicPtr->CloneLogicObj();
			if ( NULL == NewLogicPtr ) { return false; }

			NewLogicPtr->RemoveLogicWndPtrList();			
			LogicWndCount = LogicPtr->GetLogicWndPtrCount();
			for ( k=0; k<LogicWndCount; k++ )
			{
				WndPtr = LogicPtr->GetLogicWndPtr(k, false);
				if ( NULL == WndPtr ) { continue; }
				WndBandID = WndPtr->GetWndBandID();
				WndGroupID = WndPtr->GetWndGroupID();
				NewWndPtr = LandPtr->GetLandWndPtrByGroupID(WndGroupID, WndBandID);
				if ( NULL == NewWndPtr ) { continue; }
				NewLogicPtr->AddLogicWndPtr(NewWndPtr);
			}

			CAOIModel::AddModelLogicPtr(NewLogicPtr, false);
			LandPtr->AddLandLogicPtr(NewLogicPtr);
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::LayoutModelLandByLandGroupID(int LandGroupID, int LandAlignID)
{
	bool IsOK = true;	
	const double ComponentAngle = CAOIModel::GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComponentAngle);
	if ( IsExceptionAngle == false )
	{
		IsOK = CAOIModel::LayoutModelLandByLandGroupIDKernel(LandGroupID, LandAlignID);
	}
	else
	{
		const double CPX = 0;
		const double CPY = 0;
		CAOIModel::RotateModel(-ComponentAngle, CPX, CPY);
		IsOK = CAOIModel::LayoutModelLandByLandGroupIDKernel(LandGroupID, LandAlignID);
		CAOIModel::RotateModel(ComponentAngle, CPX, CPY);
	}	
	return IsOK;
}
//-------------------------------------------------------------------------------------//
inline bool CAOIModel::LayoutModelLandByLandGroupIDKernel(int LandGroupID, int LandAlignID)
{	
	BOX_TOWARD  Toward=BOX_TOWARD_NULL;
	size_t      i = 0, j=0;
	size_t      LandCount = 0;
	CAOILand   *LandPtr = NULL;
	double      PosX=0, PosY=0;
	double      PosX2=0, PosY2=0;
	std::vector<CAOILand*>   LandPtrListT;
	std::vector<CAOILand*>   LandPtrListL;
	std::vector<CAOILand*>   LandPtrListB;
	std::vector<CAOILand*>   LandPtrListR;
	std::vector<CAOILand*>   LandPtrListOthers;

	CSortObj                 SortNode, *SortNodePtr=NULL;
	std::vector<CSortObj>    SortList;

	const size_t ModelLandCount = CAOIModel::GetModelLandCount_Inline();
	const MODEL_LAND_DIRECTION LandDirection = CAOIModel::GetModelLandDirection();	

	for ( i=0; i<ModelLandCount; i++ )
	{
		LandPtr = CAOIModel::GetModelLandPtr_Inline(i);
		if ( LandPtr == NULL ) { continue; }
		if ( LandPtr->GetLandGroupID() != LandGroupID ) { continue; }
		if ( LandPtr->GetLandAlignID() != LandAlignID ) { continue; }

		LandPtr->SetLandFirstOne(false);
		LandPtr->SetLandLastOne(false);

		Toward = LandPtr->GetLandToward();
		switch ( Toward )
		{
		case BOX_TOWARD_UP:		LandPtrListT.push_back(LandPtr);	break;
		case BOX_TOWARD_LEFT:	LandPtrListL.push_back(LandPtr);	break;
		case BOX_TOWARD_DOWN:	LandPtrListB.push_back(LandPtr);	break;
		case BOX_TOWARD_RIGHT:	LandPtrListR.push_back(LandPtr);	break;
		default:	LandPtrListOthers.push_back(LandPtr);	break;
		}
	}
	
	LandCount = LandPtrListT.size();
	if ( LandCount > 0 ) 
	{		
		SortList.clear();
		SortNode.SetSortMode(SORT_BY_DBL);
		for ( i=0; i<LandCount; i++ )
		{
			LandPtr = LandPtrListT[i];
			if ( LandPtr == NULL ) { continue; }

			LandPtr->GetLandBoxPtr()->GetBoxPos(PosX, PosY);
			SortNode.SetID(i);
			SortNode.SetPtr(LandPtr);
			SortNode.SetValueDbl(PosX);
			SortList.push_back(SortNode);
		}
		std::sort(SortList.begin(), SortList.end());
		if ( LandDirection == MODEL_LAND_COUNTER_CLOCKWISE )
		{			
			SortNodePtr = &(SortList[LandCount-1]);
			LandPtr = LandPtrListT[SortNodePtr->GetID()];
			LandPtr->SetLandFirstOne(true);
			LandPtr->GetLandBoxPtr()->GetBoxPos(PosX, PosY);

			SortNodePtr = &(SortList[0]);	
			LandPtr = LandPtrListT[SortNodePtr->GetID()];
			LandPtr->SetLandLastOne(true);
			LandPtr->GetLandBoxPtr()->GetBoxPos(PosX2, PosY2);
		}
		else
		{
			SortNodePtr = &(SortList[0]);
			LandPtr = LandPtrListT[SortNodePtr->GetID()];
			LandPtr->SetLandFirstOne(true);
			LandPtr->GetLandBoxPtr()->GetBoxPos(PosX, PosY);

			SortNodePtr = &(SortList[LandCount-1]);
			LandPtr = LandPtrListT[SortNodePtr->GetID()];
			LandPtr->SetLandLastOne(true);
			LandPtr->GetLandBoxPtr()->GetBoxPos(PosX2, PosY2);
		}
	}

	LandCount = LandPtrListL.size();
	if ( LandCount > 0 ) 
	{
		SortList.clear();
		SortNode.SetSortMode(SORT_BY_DBL);
		for ( i=0; i<LandCount; i++ )
		{
			LandPtr = LandPtrListL[i];
			if ( LandPtr == NULL ) { continue; }

			LandPtr->GetLandBoxPtr()->GetBoxPos(PosX, PosY);
			SortNode.SetID(i);
			SortNode.SetPtr(LandPtr);
			SortNode.SetValueDbl(PosY);
			SortList.push_back(SortNode);
		}
		std::sort(SortList.begin(), SortList.end());
		if ( LandDirection == MODEL_LAND_COUNTER_CLOCKWISE )
		{			
			SortNodePtr = &(SortList[LandCount-1]);
			LandPtr = LandPtrListL[SortNodePtr->GetID()];
			LandPtr->SetLandFirstOne(true);
			LandPtr->GetLandBoxPtr()->GetBoxPos(PosX, PosY);
			
			SortNodePtr = &(SortList[0]);	
			LandPtr = LandPtrListL[SortNodePtr->GetID()];
			LandPtr->SetLandLastOne(true);
			LandPtr->GetLandBoxPtr()->GetBoxPos(PosX2, PosY2);
		}
		else
		{
			SortNodePtr = &(SortList[0]);			
			LandPtr = LandPtrListL[SortNodePtr->GetID()];
			LandPtr->SetLandFirstOne(true);
			LandPtr->GetLandBoxPtr()->GetBoxPos(PosX, PosY);
			
			SortNodePtr = &(SortList[LandCount-1]);
			LandPtr = LandPtrListL[SortNodePtr->GetID()];
			LandPtr->SetLandLastOne(true);
			LandPtr->GetLandBoxPtr()->GetBoxPos(PosX2, PosY2);
		}
	}

	LandCount = LandPtrListB.size();
	if ( LandCount > 0 ) 
	{
		SortList.clear();
		SortNode.SetSortMode(SORT_BY_DBL);
		for ( i=0; i<LandCount; i++ )
		{
			LandPtr = LandPtrListB[i];
			if ( LandPtr == NULL ) { continue; }

			LandPtr->GetLandBoxPtr()->GetBoxPos(PosX, PosY);
			SortNode.SetID(i);
			SortNode.SetPtr(LandPtr);
			SortNode.SetValueDbl(PosX);
			SortList.push_back(SortNode);
		}
		std::sort(SortList.begin(), SortList.end());
		if ( LandDirection == MODEL_LAND_COUNTER_CLOCKWISE )
		{			
			SortNodePtr = &(SortList[0]);
			LandPtr = LandPtrListB[SortNodePtr->GetID()];
			LandPtr->SetLandFirstOne(true);
			LandPtr->GetLandBoxPtr()->GetBoxPos(PosX, PosY);

			SortNodePtr = &(SortList[LandCount-1]);
			LandPtr = LandPtrListB[SortNodePtr->GetID()];
			LandPtr->SetLandLastOne(true);
			LandPtr->GetLandBoxPtr()->GetBoxPos(PosX2, PosY2);
		}
		else
		{
			SortNodePtr = &(SortList[LandCount-1]);
			LandPtr = LandPtrListB[SortNodePtr->GetID()];
			LandPtr->SetLandFirstOne(true);
			LandPtr->GetLandBoxPtr()->GetBoxPos(PosX, PosY);

			SortNodePtr = &(SortList[0]);	
			LandPtr = LandPtrListB[SortNodePtr->GetID()];
			LandPtr->SetLandLastOne(true);
			LandPtr->GetLandBoxPtr()->GetBoxPos(PosX2, PosY2);
		}
	}

	LandCount = LandPtrListR.size();
	if ( LandCount > 0 ) 
	{
		SortList.clear();
		SortNode.SetSortMode(SORT_BY_DBL);
		for ( i=0; i<LandCount; i++ )
		{
			LandPtr = LandPtrListR[i];
			if ( LandPtr == NULL ) { continue; }

			LandPtr->GetLandBoxPtr()->GetBoxPos(PosX, PosY);
			SortNode.SetID(i);
			SortNode.SetPtr(LandPtr);
			SortNode.SetValueDbl(PosY);
			SortList.push_back(SortNode);
		}
		std::sort(SortList.begin(), SortList.end());
		if ( LandDirection == MODEL_LAND_COUNTER_CLOCKWISE )
		{			
			SortNodePtr = &(SortList[0]);
			LandPtr = LandPtrListR[SortNodePtr->GetID()];
			LandPtr->SetLandFirstOne(true);
			LandPtr->GetLandBoxPtr()->GetBoxPos(PosX, PosY);

			SortNodePtr = &(SortList[LandCount-1]);
			LandPtr = LandPtrListR[SortNodePtr->GetID()];
			LandPtr->SetLandLastOne(true);
			LandPtr->GetLandBoxPtr()->GetBoxPos(PosX2, PosY2);
		}
		else
		{
			SortNodePtr = &(SortList[LandCount-1]);
			LandPtr = LandPtrListR[SortNodePtr->GetID()];
			LandPtr->SetLandFirstOne(true);
			LandPtr->GetLandBoxPtr()->GetBoxPos(PosX, PosY);

			SortNodePtr = &(SortList[0]);	
			LandPtr = LandPtrListR[SortNodePtr->GetID()];
			LandPtr->SetLandLastOne(true);
			LandPtr->GetLandBoxPtr()->GetBoxPos(PosX2, PosY2);
		}
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AlignModelLandByLandGroupID(int LandGroupID, int LandAlignID)
{
	bool IsOK = true;	
	const double ComponentAngle = CAOIModel::GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComponentAngle);
	if ( IsExceptionAngle == false )
	{
		IsOK = CAOIModel::AlignModelLandByLandGroupIDKernel(LandGroupID, LandAlignID);
	}
	else
	{
		const double CPX = 0;
		const double CPY = 0;
		CAOIModel::RotateModel(-ComponentAngle, CPX, CPY);
		IsOK = CAOIModel::AlignModelLandByLandGroupIDKernel(LandGroupID, LandAlignID);
		CAOIModel::RotateModel(ComponentAngle, CPX, CPY);
	}	
	return IsOK;		
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AlignModelLandByLandGroupIDKernel(int LandGroupID, int LandAlignID)
{
	BOX_TOWARD  Toward=BOX_TOWARD_NULL;	
	bool        First = true;
	size_t      i=0, j=0, k=0;
	size_t      LandCount = 0;
	CAOILand   *LandPtr = NULL;
	double      PosX=0, PosY=0;
	double      PosX2=0, PosY2=0;
	double      RgnCPX=0, RgnCPY=0;
	double      MinX=0, MinY=0, MaxX=0, MaxY=0;
	double      RgnMinX=0, RgnMinY=0, RgnMaxX=0, RgnMaxY=0;
	const int   MaxLandAlignID = CAOIModel::GetModelLandFreeAlignID(LandGroupID);
	std::vector<CAOILand*>      LandPtrListT;
	std::vector<CAOILand*>      LandPtrListL;
	std::vector<CAOILand*>      LandPtrListB;
	std::vector<CAOILand*>      LandPtrListR;
	std::vector<CAOILand*>      LandPtrListOthers;	
	const size_t ModelLandCount = CAOIModel::GetModelLandCount_Inline();
	
	for ( i=0; i<ModelLandCount; i++ )
	{
		LandPtr = CAOIModel::GetModelLandPtr_Inline(i);
		if ( NULL == LandPtr ) { continue; }
		if ( LandPtr->GetLandGroupID() != LandGroupID ) { continue; }
		if ( LandPtr->GetLandAlignID() != LandAlignID ) { continue; }

		LandPtr->SetLandFirstOne(false);
		LandPtr->SetLandLastOne(false);

		Toward = LandPtr->GetLandToward();
		switch ( Toward )
		{
		case BOX_TOWARD_UP:		LandPtrListT.push_back(LandPtr);	break;
		case BOX_TOWARD_LEFT:	LandPtrListL.push_back(LandPtr);	break;
		case BOX_TOWARD_DOWN:	LandPtrListB.push_back(LandPtr);	break;
		case BOX_TOWARD_RIGHT:	LandPtrListR.push_back(LandPtr);	break;
		default:				LandPtrListOthers.push_back(LandPtr);	break;
		}
	}

	LandCount = LandPtrListT.size();
	if ( LandCount > 0 ) 
	{	
		First = true;
		for ( i=0; i<LandCount; i++ )
		{
			LandPtr = LandPtrListT[i];
			if ( NULL == LandPtr ) { continue; }
				
			if ( First == true ) 
			{
				First = false;				
				LandPtr->GetLandBoxPtr()->GetBoxRegion(RgnMinX, RgnMinY, RgnMaxX, RgnMaxY);
			}
			else
			{
				LandPtr->GetLandBoxPtr()->GetBoxRegion(MinX, MinY, MaxX, MaxY);

				if ( RgnMinX > MinX ) { RgnMinX = MinX; }
				if ( RgnMinY > MinY ) { RgnMinY = MinY; }
				if ( RgnMaxX < MaxX ) { RgnMaxX = MaxX; }
				if ( RgnMaxY < MaxY ) { RgnMaxY = MaxY; }
			}
		}	
		RgnCPX = (RgnMinX+RgnMaxX)/2;
		RgnCPY = (RgnMinY+RgnMaxY)/2;

		for ( i=0; i<LandCount; i++ )
		{
			LandPtr = LandPtrListT[i];
			if ( LandPtr == NULL ) { continue; }
			LandPtr->MoveLand(-RgnCPX, 0);
		}
	}

	LandCount = LandPtrListL.size();
	if ( LandCount > 0 ) 
	{
		First = true;
		for ( i=0; i<LandCount; i++ )
		{
			LandPtr = LandPtrListL[i];
			if ( NULL == LandPtr ) { continue; }
				
			if ( First == true ) 
			{
				First = false;				
				LandPtr->GetLandBoxPtr()->GetBoxRegion(RgnMinX, RgnMinY, RgnMaxX, RgnMaxY);
			}
			else
			{
				LandPtr->GetLandBoxPtr()->GetBoxRegion(MinX, MinY, MaxX, MaxY);

				if ( RgnMinX > MinX ) { RgnMinX = MinX; }
				if ( RgnMinY > MinY ) { RgnMinY = MinY; }
				if ( RgnMaxX < MaxX ) { RgnMaxX = MaxX; }
				if ( RgnMaxY < MaxY ) { RgnMaxY = MaxY; }
			}
		}	
		RgnCPX = (RgnMinX+RgnMaxX)/2;
		RgnCPY = (RgnMinY+RgnMaxY)/2;

		for ( i=0; i<LandCount; i++ )
		{
			LandPtr = LandPtrListL[i];
			if ( LandPtr == NULL ) { continue; }
			LandPtr->MoveLand(0, -RgnCPY);
		}
	}

	LandCount = LandPtrListB.size();
	if ( LandCount > 0 ) 
	{
		First = true;
		for ( i=0; i<LandCount; i++ )
		{
			LandPtr = LandPtrListB[i];
			if ( NULL == LandPtr ) { continue; }
				
			if ( First == true ) 
			{
				First = false;				
				LandPtr->GetLandBoxPtr()->GetBoxRegion(RgnMinX, RgnMinY, RgnMaxX, RgnMaxY);
			}
			else
			{
				LandPtr->GetLandBoxPtr()->GetBoxRegion(MinX, MinY, MaxX, MaxY);

				if ( RgnMinX > MinX ) { RgnMinX = MinX; }
				if ( RgnMinY > MinY ) { RgnMinY = MinY; }
				if ( RgnMaxX < MaxX ) { RgnMaxX = MaxX; }
				if ( RgnMaxY < MaxY ) { RgnMaxY = MaxY; }
			}
		}	
		RgnCPX = (RgnMinX+RgnMaxX)/2;
		RgnCPY = (RgnMinY+RgnMaxY)/2;

		for ( i=0; i<LandCount; i++ )
		{
			LandPtr = LandPtrListB[i];
			if ( LandPtr == NULL ) { continue; }
			LandPtr->MoveLand(-RgnCPX, 0);
		}
	}

	LandCount = LandPtrListR.size();
	if ( LandCount > 0 ) 
	{
		First = true;
		for ( i=0; i<LandCount; i++ )
		{
			LandPtr = LandPtrListR[i];
			if ( NULL == LandPtr ) { continue; }
				
			if ( First == true ) 
			{
				First = false;				
				LandPtr->GetLandBoxPtr()->GetBoxRegion(RgnMinX, RgnMinY, RgnMaxX, RgnMaxY);
			}
			else
			{
				LandPtr->GetLandBoxPtr()->GetBoxRegion(MinX, MinY, MaxX, MaxY);

				if ( RgnMinX > MinX ) { RgnMinX = MinX; }
				if ( RgnMinY > MinY ) { RgnMinY = MinY; }
				if ( RgnMaxX < MaxX ) { RgnMaxX = MaxX; }
				if ( RgnMaxY < MaxY ) { RgnMaxY = MaxY; }
			}
		}	
		RgnCPX = (RgnMinX+RgnMaxX)/2;
		RgnCPY = (RgnMinY+RgnMaxY)/2;

		for ( i=0; i<LandCount; i++ )
		{
			LandPtr = LandPtrListR[i];
			if ( LandPtr == NULL ) { continue; }
			LandPtr->MoveLand(0, -RgnCPY);
		}
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::DeleteModelLandIndexList(std::vector<size_t> &LandIndexList)
{	
	size_t       LandIndex = 0;
	size_t       i=0, j=0;	
	CAOILand    *LandPtr = NULL;		
	size_t       ItemWndBoxCount = 0;
	size_t       ItemCount = 0;
	const size_t LandIndexCount = LandIndexList.size();
	
	SetModelLogicSelected(false);
	SetModelWndSelected(false);
	SetModelLandSelected(false);

	for ( i=0; i<LandIndexCount; i++ )
	{
		LandIndex = LandIndexList[i];
		LandPtr = CAOIModel::GetModelLandPtr(LandIndex, true);		
		if ( NULL == LandPtr ) { continue; }

		LandPtr->SetLandAllBoxSelected(true);
		LandPtr->SetLandWndSelected(true);
		LandPtr->SetLandLogicSelected(true);
	}
	ClearModelWndOrderList();
	RemoveModelObjectByWndObjSelected();	
	RemoveModelAlgPatternFolderBySelected();
	DestroyModelLogicSelected();
	DestroyModelWndSelected();
	DestroyModelLandSelected();
	UpdateModelLogicWndIndex();	
	return true;
}
//-------------------------------------------------------------------------------------//
int CAOIModel::GetModelLandCountByToward(CAOILand *RefLandPtr, bool bActiveOnly)
{
	int          Count=0;
	if ( NULL == RefLandPtr ) { return Count; }

	size_t       i=0;	
	CAOILand    *LandPtr = NULL;
	BOX_TOWARD   LandToward  = RefLandPtr->GetLandToward();
	const int    LandGroupID = RefLandPtr->GetLandGroupID();	
	const int    LandAlignID = RefLandPtr->GetLandAlignID();
	const size_t LandCount = CAOIModel::GetModelLandCount_Inline();	

	Count=0;
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = CAOIModel::GetModelLandPtr_Inline(i);
		if ( LandPtr == NULL ) { continue; }
		if ( LandPtr->GetLandGroupID() != LandGroupID ) { continue; }
		if ( LandPtr->GetLandToward() != LandToward ) { continue; }
		if ( LandPtr->GetLandAlignID() != LandAlignID ) { continue; }
		if ( true == bActiveOnly )
		{
			if ( LandPtr->GetLandSelected() == false ) { continue; }
		}
		
		Count ++;
	}	
	return Count;
}
//-------------------------------------------------------------------------------------//
double CAOIModel::GetModelLandPitchByToward(CAOILand *RefLandPtr, bool bActiveOnly)
{
	double Pitch = 500;
	if ( NULL == RefLandPtr ) { return Pitch; }
	const double ComAngle = CAOIModel::GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComAngle);
	if ( IsExceptionAngle == false )
	{	Pitch = CAOIModel::GetModelLandPitchByTowardKernel(RefLandPtr, bActiveOnly);	}
	else
	{
		CAOIModel::RotateModel(-ComAngle, 0, 0);
		Pitch = CAOIModel::GetModelLandPitchByTowardKernel(RefLandPtr, bActiveOnly);
		CAOIModel::RotateModel(ComAngle, 0, 0);
	}
	return Pitch;
}
//-------------------------------------------------------------------------------------//
double CAOIModel::GetModelLandPitchByTowardKernel(CAOILand *RefLandPtr, bool bActiveOnly) const
{
	double       Pitch=500;	
	if ( NULL == RefLandPtr ) { return Pitch; }	
	size_t       i=0;
	int          Count=0;	
	double       Range=0;	
	CAOILand    *LandPtr = NULL;
	double       CPX=0, CPY=0;
	double       MinCPX=0, MinCPY=0, MaxCPX=0, MaxCPY=0;
	BOX_TOWARD   LandToward  = RefLandPtr->GetLandToward();
	const int    LandGroupID = RefLandPtr->GetLandGroupID();
	const int    LandAlignID = RefLandPtr->GetLandAlignID();
	const size_t LandCount = CAOIModel::GetModelLandCount_Inline();	
	Count=0;
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = CAOIModel::GetModelLandPtr_Inline(i);
		if ( NULL == LandPtr ) { continue; }		
		if ( LandPtr->GetLandGroupID() != LandGroupID ) { continue; }
		if ( LandPtr->GetLandToward() != LandToward ) { continue; }
		if ( LandPtr->GetLandAlignID() != LandAlignID ) { continue; }
		if ( true == bActiveOnly )
		{
			if ( LandPtr->GetLandSelected() == false ) { continue; }
		}
		
		LandPtr->GetLandBoxPtr()->GetBoxPos(CPX, CPY);
		if ( Count == 0 ) 
		{
			MinCPX = MaxCPX = CPX;
			MinCPY = MaxCPY = CPY;
		}
		else
		{
			if ( MinCPX > CPX ) { MinCPX = CPX; }
			if ( MinCPY > CPY ) { MinCPY = CPY; }
			if ( MaxCPX < CPX ) { MaxCPX = CPX; }
			if ( MaxCPY < CPY ) { MaxCPY = CPY; }
		}
		Count ++;
	}	
	if ( Count < 2 ) 
	{	return Pitch; }

	switch ( LandToward )
	{
	case BOX_TOWARD_UP:
	case BOX_TOWARD_DOWN:
		Range = MaxCPX-MinCPX;
		break;
	case BOX_TOWARD_LEFT:
	case BOX_TOWARD_RIGHT:
		Range = MaxCPY-MinCPY;
		break;
	}
	Pitch = Range/(Count-1);
	return Pitch;
}
//-------------------------------------------------------------------------------------//
void CAOIModel::UpdateModelLandLeadID()//更新引腳編號
{
	//尚未處理
	int i=0;	
	int LeadID = 0;	
	int LandIndex = 0;
	int LandIndexCount = 0;
	double LandPitch = 0.0;
	CAOILand        *LandPtr = NULL;
	std::vector<int> LandIndexListT;
	std::vector<int> LandIndexListL;
	std::vector<int> LandIndexListB;
	std::vector<int> LandIndexListR;	
	std::vector<int> *LandIDListPtr1=NULL, *LandIDListPtr2=NULL, *LandIDListPtr3=NULL, *LandIDListPtr4=NULL;
	
	/*
	const int LocalToward = CAOIModel::GetLocalToward();
	const MODEL_LAND_DIRECTION LandDirection = CLocalObj::GetLocalLandDirection();
	const int LandStartLeadID = CLocalObj::GetLandStartLeadID();

	CLocalObj::GetModelTowardLandIndexList(BOX_TOWARD_UP, LandIndexListT, LandPitch);
	CLocalObj::GetModelTowardLandIndexList(BOX_TOWARD_LEFT, LandIndexListL, LandPitch);
	CLocalObj::GetModelTowardLandIndexList(BOX_TOWARD_DOWN, LandIndexListB, LandPitch);
	CLocalObj::GetModelTowardLandIndexList(BOX_TOWARD_RIGHT, LandIndexListR, LandPitch);

	switch ( LocalToward )
	{
	case BOX_TOWARD_UP:		
		if ( LandDirection == MODEL_LAND_CLOCKWISE )//順時針
		{
			LandIDListPtr1 = &LandIndexListT;//Toward Top
			LandIDListPtr2 = &LandIndexListR;//Toward Right
			LandIDListPtr3 = &LandIndexListB;//Toward Bottom
			LandIDListPtr4 = &LandIndexListL;//Toward Left
		}
		else//逆時針
		{
			LandIDListPtr1 = &LandIndexListT;//Toward Top
			LandIDListPtr2 = &LandIndexListL;//Toward Left
			LandIDListPtr3 = &LandIndexListB;//Toward Bottom
			LandIDListPtr4 = &LandIndexListR;//Toward Right
		}
		break;
	case BOX_TOWARD_LEFT:
		if ( LandDirection == MODEL_LAND_CLOCKWISE )//順時針
		{
			LandIDListPtr1 = &LandIndexListL;//Toward Left
			LandIDListPtr2 = &LandIndexListT;//Toward Top
			LandIDListPtr3 = &LandIndexListR;//Toward Right
			LandIDListPtr4 = &LandIndexListB;//Toward Bottom			
		}
		else//逆時針
		{			
			LandIDListPtr1 = &LandIndexListL;//Toward Left
			LandIDListPtr2 = &LandIndexListB;//Toward Bottom
			LandIDListPtr3 = &LandIndexListR;//Toward Right
			LandIDListPtr4 = &LandIndexListT;//Toward Top
		}
		break;
	case BOX_TOWARD_DOWN:
		if ( LandDirection == MODEL_LAND_CLOCKWISE )//順時針
		{
			LandIDListPtr1 = &LandIndexListB;//Toward Bottom
			LandIDListPtr2 = &LandIndexListL;//Toward Left
			LandIDListPtr3 = &LandIndexListT;//Toward Top
			LandIDListPtr4 = &LandIndexListR;//Toward Right
			
		}
		else//逆時針
		{	
			LandIDListPtr1 = &LandIndexListB;//Toward Bottom
			LandIDListPtr2 = &LandIndexListR;//Toward Right
			LandIDListPtr3 = &LandIndexListT;//Toward Top
			LandIDListPtr4 = &LandIndexListL;//Toward Left
		}
		break;
	case BOX_TOWARD_RIGHT:
		if ( LandDirection == MODEL_LAND_CLOCKWISE )//順時針
		{
			LandIDListPtr1 = &LandIndexListR;//Toward Right
			LandIDListPtr2 = &LandIndexListB;//Toward Bottom
			LandIDListPtr3 = &LandIndexListL;//Toward Left
			LandIDListPtr4 = &LandIndexListT;//Toward Top			
			
		}
		else//逆時針
		{				
			LandIDListPtr1 = &LandIndexListR;//Toward Right
			LandIDListPtr2 = &LandIndexListT;//Toward Top
			LandIDListPtr3 = &LandIndexListL;//Toward Left
			LandIDListPtr4 = &LandIndexListB;//Toward Bottom
		}
		break;
	}
	

	LeadID = LandStartLeadID;
	if ( LandIDListPtr1 != NULL )
	{
		LandIndexCount = (int)(LandIDListPtr1->size());
		for ( i=0; i<LandIndexCount; i++ )
		{
			LandIndex = (*LandIDListPtr1)[i];
			LandPtr = MoelPtr->GetModelLandPtr(LandIndex, true);			
			if ( LandPtr == NULL ) { continue; }				
			LandPtr->SetLandLeadID(LeadID);
			LeadID ++;
		}
	}

	if ( LandIDListPtr2 != NULL )
	{
		LandIndexCount = (int)(LandIDListPtr2->size());
		for ( i=0; i<LandIndexCount; i++ )
		{
			LandIndex = (*LandIDListPtr2)[i];
			LandPtr = MoelPtr->GetModelLandPtr(LandIndex, true);
			if ( LandPtr == NULL ) { continue; }				
			LandPtr->SetLandLeadID(LeadID);
			LeadID ++;
		}
	}

	if ( LandIDListPtr3 != NULL )
	{
		LandIndexCount = (int)(LandIDListPtr3->size());
		for ( i=0; i<LandIndexCount; i++ )
		{
			LandIndex = (*LandIDListPtr3)[i];
			LandPtr = MoelPtr->GetModelLandPtr(LandIndex, true);
			if ( LandPtr == NULL ) { continue; }				
			LandPtr->SetLandLeadID(LeadID);
			LeadID ++;
		}
	}

	if ( LandIDListPtr4 != NULL )
	{
		LandIndexCount = (int)(LandIDListPtr4->size());
		for ( i=0; i<LandIndexCount; i++ )
		{
			LandIndex = (*LandIDListPtr4)[i];
			LandPtr = MoelPtr->GetModelLandPtr(LandIndex, true);
			if ( LandPtr == NULL ) { continue; }				
			LandPtr->SetLandLeadID(LeadID);
			LeadID ++;
		}
	}
	*/
	return ;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::ModifyModelLandAlign(bool SelectedOnly)
{
	bool IsOK = true;	
	const double AttachedAngle = GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(AttachedAngle);
	if ( false == IsExceptionAngle )	
	{
		IsOK = ModifyModelLandAlignKernel(SelectedOnly);
	}
	else
	{
		const double CPX = 0;
		const double CPY = 0;
		RotateModel(-AttachedAngle, CPX, CPY);
		IsOK = ModifyModelLandAlignKernel(SelectedOnly);
		RotateModel(AttachedAngle, CPX, CPY);
	}	
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::ModifyModelLandAlignKernel(bool SelectedOnly)
{	
	size_t       i=0, j=0;	
	TPOINT2D     MovePos;
	TPOINT2D     LandPos;	
	TPOINT2D     RefLandPos;
	BOX_TOWARD   LandToward;
	BOX_TOWARD   RefLandToward;
	unsigned int LandAlignID=0;
	unsigned int LandGroupID=0;
	unsigned int RefLandAlignID=0;
	unsigned int RefLandGroupID=0;
	CAOILand    *LandPtr = NULL;
	CAOILand    *RefLandPtr = NULL;
	const size_t LandCount = GetModelLandCount_Inline();

	for ( i=0; i<LandCount; i++ )
	{	
		LandPtr = GetModelLandPtr_Inline(i);
		if ( NULL == LandPtr ) { continue; }
		LandPtr->SetLandTempInt(FN_DISABLE);
	}

	for ( i=0; i<LandCount; i++ )
	{
		RefLandPtr = GetModelLandPtr_Inline(i);
		if ( NULL == RefLandPtr ) { continue; }
		if ( FN_ENABLE == RefLandPtr->GetLandTempInt() ) { continue; }
		if ( true==SelectedOnly )
		{
			if ( RefLandPtr->GetLandSelected() == false )
			{	continue; }
		}
		
		RefLandToward = RefLandPtr->GetLandToward();
		RefLandGroupID = RefLandPtr->GetLandGroupID();
		RefLandAlignID = RefLandPtr->GetLandAlignID();		
		RefLandPtr->GetLandBoxPtr()->GetBoxPos(RefLandPos);
		RefLandPtr->SetLandModified(true);
		RefLandPtr->SetLandTempInt(FN_ENABLE);
		for ( j=0; j<LandCount; j++ )
		{
			LandPtr = GetModelLandPtr_Inline(j);
			if ( NULL == LandPtr ) { continue; }
			if ( FN_ENABLE == LandPtr->GetLandTempInt() ) { continue; }
			if ( true==SelectedOnly )
			{
				if ( LandPtr->GetLandSelected() == false )
				{	continue; }
			}

			LandToward = LandPtr->GetLandToward();
			LandGroupID = LandPtr->GetLandGroupID();
			LandAlignID = LandPtr->GetLandAlignID();			
			if ( LandToward != RefLandToward ) { continue; }
			if ( LandGroupID != RefLandGroupID ) { continue; }
			if ( LandAlignID != RefLandAlignID ) { continue; }
			LandPtr->GetLandBoxPtr()->GetBoxPos(LandPos);
			switch ( RefLandToward )
			{
			case BOX_TOWARD_UP:
			case BOX_TOWARD_DOWN:
				MovePos.x = 0;
				MovePos.y = RefLandPos.y-LandPos.y;				
				break;
			case BOX_TOWARD_LEFT:
			case BOX_TOWARD_RIGHT:
				MovePos.y = 0;
				MovePos.x = RefLandPos.x-LandPos.x;
				break;
			default:
				MovePos.x = MovePos.y = 0;
				break;
			}
			LandPtr->SetLandModified(true);
			LandPtr->MoveLand(MovePos.x, MovePos.y, true);			
			LandPtr->SetLandTempInt(FN_ENABLE);
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::ModifyModelLandCount(CAOILand *RefLandPtr, size_t LandCount, bool bArray)
{
	bool IsOK = true;
	const double AttachedAngle = GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(AttachedAngle);
	if ( IsExceptionAngle == false )
	{
		IsOK = ModifyModelLandCountKernel(RefLandPtr, LandCount, bArray);
	}
	else
	{
		const double CPX = 0;
		const double CPY = 0;
		RotateModel(-AttachedAngle, CPX, CPY);
		IsOK = ModifyModelLandCountKernel(RefLandPtr, LandCount, bArray);
		RotateModel(AttachedAngle, CPX, CPY);
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
inline bool CAOIModel::ModifyModelLandCountKernel(CAOILand *RefLandPtr, size_t LandCount, bool bArray)
{
	size_t i=0;	
	double LandPitch2 = 0.0;
	double LandPitch3 = 0.0;
	std::vector<size_t> LandIndexList;	
	if ( (RefLandPtr==NULL) || LandCount < 2 ) { return false; }		
	double LandPitch = 500;
	const int LandGroupID = RefLandPtr->GetLandGroupID();
	const BOX_TOWARD RefToward = RefLandPtr->GetLandToward();
	if ( false == bArray )
	{
		GetModelLandIndexList(RefLandPtr, true, LandIndexList, LandPitch2, LandPitch3);
		//GetModelLandIndexList(RefLandPtr, false, LandIndexList, LandPitch2, LandPitch3);
	}
	else
	{	GetModelLandSelectedIndexList(LandGroupID, LandIndexList);	}
	if ( LandCount > 1 ) 
	{	LandPitch = LandPitch3/(LandCount-1);	}
	if ( LandPitch < 0.1 ) { LandPitch = 500; }

	const MODEL_LAND_DIRECTION LandDirection = GetModelLandDirection();	
	double LandCPX=0, LandCPY=0;
	double LandCPX2=0, LandCPY2=0;
	double RegionCPX=0, RegionCPY=0;		
	CAOILand *LandPtr=NULL;	
	CAOILand *NewLandPtr=NULL;
	const size_t LandIndexCount = LandIndexList.size();
	if ( LandCount == LandIndexCount ) { return true; }

	double ModelCPX=0, ModelCPY=0;	
	if ( LandIndexCount > 0 ) 
	{	
		LandPtr = GetModelLandPtr(LandIndexList[0], true);
		if ( NULL == LandPtr ) { return false; }		
		NewLandPtr = CopyModelLand(LandPtr);
		if ( NULL == NewLandPtr ) { return false; }		
		for ( i=0; i<LandIndexCount; i++ )
		{
			LandPtr = GetModelLandPtr(LandIndexList[i], true);
			if ( i == 0 ) 
			{	LandPtr->GetLandBoxPtr()->GetBoxPos(RegionCPX, RegionCPY);	}
			else
			{	
				LandPtr->GetLandBoxPtr()->GetBoxPos(LandCPX, LandCPY);
				RegionCPX = RegionCPX+LandCPX;
				RegionCPY = RegionCPY+LandCPY;
			}
		}
		RegionCPX = RegionCPX/LandIndexCount;
		RegionCPY = RegionCPY/LandIndexCount;		
	}
	if ( NULL == NewLandPtr ) { return true; }

	if ( DeleteModelLandIndexList(LandIndexList) == false )
	{	return false; }		

	if ( LandCount == 0 ) 
	{
		UpdateModelLandLeadID();		
		return true; 
	}
	
	double OffsetX=0, OffsetY=0;
	double StartX=0, StartY=0;
	double DummyX=0, DummyY=0;
	const int HalfPitchCount = (int)((LandCount-1)/2);

	if ( (LandCount%2) == 1 ) 
	{ 
		DummyX = 0; 
		DummyY = 0;
	}
	else 
	{ 
		DummyX = LandPitch/2.0; 
		DummyY = LandPitch/2.0; 
	}	

	//考慮排列一致性, 因此還是得要給予特定位置
	switch ( RefToward )
	{
	case BOX_TOWARD_UP:		
		switch ( LandDirection )
		{
		case MODEL_LAND_CLOCKWISE://Left to Right
			StartX = -(HalfPitchCount*LandPitch+DummyX)+ModelCPX;
			NewLandPtr->GetLandBoxPtr()->SetBoxSelected(true);
			NewLandPtr->GetLandBoxPtr()->GetBoxPos(LandCPX2, LandCPY2);			
			StartX = LandCPX2;
			for ( i=0; i<LandCount; i++ )
			{
				if ( i > 0 )
				{	NewLandPtr = this->CopyModelLand(NewLandPtr);	}
				if ( NewLandPtr == NULL ) { continue; }
			
				LandCPX = StartX+(i*LandPitch);
				NewLandPtr->GetLandBoxPtr()->GetBoxPos(LandCPX2, LandCPY2);
				OffsetX = LandCPX-LandCPX2;
				OffsetY = 0;
				NewLandPtr->MoveLand(OffsetX, OffsetY);
				NewLandPtr->SetLandAttachedPosCad(m_ModelAttachedCadPos);
				NewLandPtr->SetLandAttachedPosStage(m_ModelAttachedStagePos);
			}
			break;
		case MODEL_LAND_COUNTER_CLOCKWISE://Right to Left
			StartX = (HalfPitchCount*LandPitch+DummyX)+ModelCPX;
			NewLandPtr->GetLandBoxPtr()->SetBoxSelected(true);
			NewLandPtr->GetLandBoxPtr()->GetBoxPos(LandCPX2, LandCPY2);			
			StartX = LandCPX2;
			for ( i=0; i<LandCount; i++ )
			{
				if ( i > 0 )				
				{	NewLandPtr = this->CopyModelLand(NewLandPtr);	}
				if ( NewLandPtr == NULL ) { continue; }

				LandCPX = StartX-(i*LandPitch);
				NewLandPtr->GetLandBoxPtr()->GetBoxPos(LandCPX2, LandCPY2);
				OffsetX = LandCPX-LandCPX2;
				OffsetY = 0;
				NewLandPtr->MoveLand(OffsetX, OffsetY);
				NewLandPtr->SetLandAttachedPosCad(m_ModelAttachedCadPos);
				NewLandPtr->SetLandAttachedPosStage(m_ModelAttachedStagePos);
				
			}
			break;
		}
		break;
	case BOX_TOWARD_LEFT://		
		switch ( LandDirection )
		{
		case MODEL_LAND_CLOCKWISE://Bottom to Top
			StartY = -(HalfPitchCount*LandPitch+DummyY)+ModelCPY;
			NewLandPtr->GetLandBoxPtr()->SetBoxSelected(true);
			NewLandPtr->GetLandBoxPtr()->GetBoxPos(LandCPX2, LandCPY2);			
			StartY = LandCPY2;
			for ( i=0; i<LandCount; i++ )
			{
				if ( i > 0 )				
				{	NewLandPtr = this->CopyModelLand(NewLandPtr);	}
				if ( NewLandPtr == NULL ) { continue; }

				LandCPY = StartY+(i*LandPitch);
				NewLandPtr->GetLandBoxPtr()->GetBoxPos(LandCPX2, LandCPY2);
				OffsetX = 0;
				OffsetY = LandCPY-LandCPY2;
				NewLandPtr->MoveLand(OffsetX, OffsetY);
				NewLandPtr->SetLandAttachedPosCad(m_ModelAttachedCadPos);
				NewLandPtr->SetLandAttachedPosStage(m_ModelAttachedStagePos);
			}
			break;
		case MODEL_LAND_COUNTER_CLOCKWISE://Top to Bottom
			StartY = (HalfPitchCount*LandPitch+DummyY)+ModelCPY;
			NewLandPtr->GetLandBoxPtr()->SetBoxSelected(true);
			NewLandPtr->GetLandBoxPtr()->GetBoxPos(LandCPX2, LandCPY2);			
			StartY = LandCPY2;
			for ( i=0; i<LandCount; i++ )
			{
				if ( i > 0 )
				{	NewLandPtr = this->CopyModelLand(NewLandPtr);	}
				if ( NewLandPtr == NULL ) { continue; }

				LandCPY = StartY-(i*LandPitch);
				NewLandPtr->GetLandBoxPtr()->GetBoxPos(LandCPX2, LandCPY2);
				OffsetX = 0;
				OffsetY = LandCPY-LandCPY2;
				NewLandPtr->MoveLand(OffsetX, OffsetY);
				NewLandPtr->SetLandAttachedPosCad(m_ModelAttachedCadPos);
				NewLandPtr->SetLandAttachedPosStage(m_ModelAttachedStagePos);
			}
			break;
		}
		break;
	case BOX_TOWARD_DOWN://
		switch ( LandDirection )
		{
		case MODEL_LAND_CLOCKWISE://Right to Left
			StartX = (HalfPitchCount*LandPitch+DummyX)+ModelCPX;
			NewLandPtr->GetLandBoxPtr()->SetBoxSelected(true);
			NewLandPtr->GetLandBoxPtr()->GetBoxPos(LandCPX2, LandCPY2);			
			StartX = LandCPX2;
			for ( i=0; i<LandCount; i++ )
			{
				if ( i > 0 )
				{	NewLandPtr = this->CopyModelLand(NewLandPtr);	}
				if ( NewLandPtr == NULL ) { continue; }

				LandCPX = StartX-(i*LandPitch);
				NewLandPtr->GetLandBoxPtr()->GetBoxPos(LandCPX2, LandCPY2);
				OffsetX = LandCPX-LandCPX2;
				OffsetY = 0;
				NewLandPtr->MoveLand(OffsetX, OffsetY);
				NewLandPtr->SetLandAttachedPosCad(m_ModelAttachedCadPos);
				NewLandPtr->SetLandAttachedPosStage(m_ModelAttachedStagePos);
				
			}
			break;
		case MODEL_LAND_COUNTER_CLOCKWISE://Left to Right
			StartX = -(HalfPitchCount*LandPitch+DummyX)+ModelCPX;
			NewLandPtr->GetLandBoxPtr()->SetBoxSelected(true);
			NewLandPtr->GetLandBoxPtr()->GetBoxPos(LandCPX2, LandCPY2);			
			StartX = LandCPX2;
			for ( i=0; i<LandCount; i++ )
			{
				if ( i > 0 )
				{	NewLandPtr = this->CopyModelLand(NewLandPtr);	}
				if ( NewLandPtr == NULL ) { continue; }

				LandCPX = StartX+(i*LandPitch);
				NewLandPtr->GetLandBoxPtr()->GetBoxPos(LandCPX2, LandCPY2);
				OffsetX = LandCPX-LandCPX2;
				OffsetY = 0;
				NewLandPtr->MoveLand(OffsetX, OffsetY);
				NewLandPtr->SetLandAttachedPosCad(m_ModelAttachedCadPos);
				NewLandPtr->SetLandAttachedPosStage(m_ModelAttachedStagePos);
			}
			break;
		}
		break;
	case BOX_TOWARD_RIGHT://
		switch ( LandDirection )
		{
		case MODEL_LAND_CLOCKWISE://Top to Bottom
			StartY = (HalfPitchCount*LandPitch+DummyY)+ModelCPY;
			NewLandPtr->GetLandBoxPtr()->SetBoxSelected(true);
			NewLandPtr->GetLandBoxPtr()->GetBoxPos(LandCPX2, LandCPY2);			
			StartY = LandCPY2;
			for ( i=0; i<LandCount; i++ )
			{
				if ( i > 0 )
				{	NewLandPtr = this->CopyModelLand(NewLandPtr);	}
				if ( NewLandPtr == NULL ) { continue; }

				LandCPY = StartY-(i*LandPitch);
				NewLandPtr->GetLandBoxPtr()->GetBoxPos(LandCPX2, LandCPY2);

				OffsetX = 0;
				OffsetY = LandCPY-LandCPY2;
				NewLandPtr->MoveLand(OffsetX, OffsetY);
				NewLandPtr->SetLandAttachedPosCad(m_ModelAttachedCadPos);
				NewLandPtr->SetLandAttachedPosStage(m_ModelAttachedStagePos);
			}
			break;
		case MODEL_LAND_COUNTER_CLOCKWISE://Bottom to Top
			StartY = -(HalfPitchCount*LandPitch+DummyY)+ModelCPY;
			NewLandPtr->GetLandBoxPtr()->SetBoxSelected(true);
			NewLandPtr->GetLandBoxPtr()->GetBoxPos(LandCPX2, LandCPY2);			
			StartY = LandCPY2;
			for ( i=0; i<LandCount; i++ )
			{
				if ( i > 0 )
				{	NewLandPtr = this->CopyModelLand(NewLandPtr);	}
				if ( NewLandPtr == NULL ) { continue; }

				LandCPY = StartY+(i*LandPitch);

				NewLandPtr->GetLandBoxPtr()->GetBoxPos(LandCPX2, LandCPY2);

				OffsetX = 0;
				OffsetY = LandCPY-LandCPY2;
				NewLandPtr->MoveLand(OffsetX, OffsetY);
				NewLandPtr->SetLandAttachedPosCad(m_ModelAttachedCadPos);
				NewLandPtr->SetLandAttachedPosStage(m_ModelAttachedStagePos);
			}
			break;
		}
		break;
	}
	SetModelModifiedCount(true);
	SetModelNeedSaveFiles(true);
	UpdateModelLandLeadID();		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::ModifyModelLandIncludePadAlign(CAOILand *RefLandPtr, bool bInclude)
{
	CAOIModel *ModelPtr = this;
	if ( NULL == ModelPtr ) { return false; }
	if ( NULL == RefLandPtr ) { return true; }
	
	size_t       i=0;
	int          LandGroupID=0;
	CAOILand    *LandPtr = NULL;
	const size_t LandCount = ModelPtr->GetModelLandCount_Inline();
	const int    RefLandGroupID = RefLandPtr->GetLandGroupID();

	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = ModelPtr->GetModelLandPtr_Inline(i);
		if ( NULL == LandPtr ) { continue; }
		LandGroupID = LandPtr->GetLandGroupID();
		if ( RefLandGroupID!=LandGroupID ) { continue; }
		LandPtr->SetLandIncludePadAlign(bInclude);
	}
	UpdateModelWndRgnByLinkMode();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::ModifyModelLandIncludePartAlign(CAOILand *RefLandPtr, bool bInclude)
{
	CAOIModel *ModelPtr = this;
	if ( NULL == ModelPtr ) { return false; }
	if ( NULL == RefLandPtr ) { return true; }
	
	size_t       i=0;
	int          LandGroupID=0;
	CAOILand    *LandPtr = NULL;
	const size_t LandCount = ModelPtr->GetModelLandCount_Inline();
	const int    RefLandGroupID = RefLandPtr->GetLandGroupID();

	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = ModelPtr->GetModelLandPtr_Inline(i);
		if ( NULL == LandPtr ) { continue; }
		LandGroupID = LandPtr->GetLandGroupID();
		if ( RefLandGroupID!=LandGroupID ) { continue; }
		LandPtr->SetLandIncludePartAlign(bInclude);
	}
	UpdateModelWndRgnByLinkMode();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::ModifyModelLandPitch(CAOILand *RefLandPtr, double LandPitch, bool SelectedOnly)
{
	bool IsOK = true;	
	const double AttachedAngle = GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(AttachedAngle);
	if ( false == IsExceptionAngle )	
	{
		IsOK = ModifyModelLandPitchKernel(RefLandPtr, LandPitch, SelectedOnly);
	}
	else
	{
		const double CPX = 0;
		const double CPY = 0;
		RotateModel(-AttachedAngle, CPX, CPY);
		IsOK = ModifyModelLandPitchKernel(RefLandPtr, LandPitch, SelectedOnly);
		RotateModel(AttachedAngle, CPX, CPY);
	}	
	return IsOK;
}
//-------------------------------------------------------------------------------------//
inline bool CAOIModel::ModifyModelLandPitchKernel(CAOILand *RefLandPtr, double LandPitch, bool SelectedOnly)
{
	if ( NULL == RefLandPtr ) { return false; }	
	size_t i=0, j=0;
	double LandPitch2 = 0.0;
	double LandPitch3 = 0.0;
	const BOX_TOWARD RefToward = RefLandPtr->GetLandToward();
	std::vector<size_t> LandIndexList;	
	GetModelLandIndexList(RefLandPtr, SelectedOnly, LandIndexList, LandPitch2, LandPitch3);	
	const size_t LandIndexCount = LandIndexList.size();	
	CAOILand *LandPtr = NULL;

	int    Count = 0;	
	double LandCPX=0, LandCPY=0;
	double LandCPX2=0, LandCPY2=0;
	double RegionCPX=0, RegionCPY=0;
	double MinX=0, MinY=0, MaxX=0, MaxY=0;	
	const  MODEL_LAND_DIRECTION LandDirection = CAOIModel::GetModelLandDirection();

	Count = 0;
	for ( i=0; i<LandIndexCount; i++ )
	{
		LandPtr = this->GetModelLandPtr(LandIndexList[i], true);	
		if ( LandPtr == NULL ) { continue; }
		
		LandPtr->GetLandBoxPtr()->GetBoxPos(LandCPX, LandCPY);
		if( i == 0 ) 
		{
			RegionCPX = LandCPX;
			RegionCPY = LandCPY;
		}
		else
		{
			RegionCPX += LandCPX;
			RegionCPY += LandCPY;
		}
		Count ++;
	}
	if ( Count == 0 ) { return true; }

	double ModelCPX=0, ModelCPY=0;
	RegionCPX = RegionCPX/Count;
	RegionCPY = RegionCPY/Count;

	double OffsetX=0, OffsetY=0;
	double StartX=0, StartY=0;
	double DummyX=0, DummyY=0;
	const int HalfPitchCount = (int)((LandIndexCount-1)/2);

	if ( (LandIndexCount%2) == 1 ) 
	{ 
		DummyX = 0; 
		DummyY = 0;
	}
	else 
	{ 
		DummyX = LandPitch/2.0; 
		DummyY = LandPitch/2.0; 
	}

	LandPtr = this->GetModelLandPtr(LandIndexList[0], true);	
	if ( LandPtr == NULL ) { return false; }
	LandPtr->GetLandBoxPtr()->GetBoxPos(StartX, StartY);
	//考慮排列一致性, 因此還是得要給予特定位置
	switch ( RefToward )
	{
	case BOX_TOWARD_UP:		
		switch ( LandDirection )
		{
		case MODEL_LAND_CLOCKWISE://Left to Right
			//StartX = -(HalfPitchCount*LandPitch+DummyX)+LocalCPX;
			for ( i=0; i<LandIndexCount; i++ )
			{				
				LandCPX = StartX+(i*LandPitch);
				LandPtr = this->GetModelLandPtr(LandIndexList[i], true);	
				if ( LandPtr == NULL ) { continue; }

				LandPtr->GetLandBoxPtr()->GetBoxPos(LandCPX2, LandCPY2);
				OffsetX = LandCPX - LandCPX2;
				OffsetY = 0;
				LandPtr->MoveLand(OffsetX, OffsetY);	
				LandPtr->SetLandAttachedPosCad(m_ModelAttachedCadPos);
				LandPtr->SetLandAttachedPosStage(m_ModelAttachedStagePos);
			}
			break;
		case MODEL_LAND_COUNTER_CLOCKWISE://Right to Left
			//StartX = (HalfPitchCount*LandPitch+DummyX)+LocalCPX;
			for ( i=0; i<LandIndexCount; i++ )
			{
				LandCPX = StartX-(i*LandPitch);
				LandPtr = this->GetModelLandPtr(LandIndexList[i], true);	
				if ( LandPtr == NULL ) { continue; }
				
				LandPtr->GetLandBoxPtr()->GetBoxPos(LandCPX2, LandCPY2);
				OffsetX = LandCPX - LandCPX2;
				OffsetY = 0;
				LandPtr->MoveLand(OffsetX, OffsetY);		
				LandPtr->SetLandAttachedPosCad(m_ModelAttachedCadPos);
				LandPtr->SetLandAttachedPosStage(m_ModelAttachedStagePos);
			}
			break;
		}
		break;
	case BOX_TOWARD_LEFT://		
		switch ( LandDirection )
		{
		case MODEL_LAND_CLOCKWISE://Bottom to Top
			//StartY = -(HalfPitchCount*LandPitch+DummyY)+LocalCPY;
			for ( i=0; i<LandIndexCount; i++ )
			{
				LandCPY = StartY+(i*LandPitch);
				LandPtr = this->GetModelLandPtr(LandIndexList[i], true);	
				if ( LandPtr == NULL ) { continue; }
				
				LandPtr->GetLandBoxPtr()->GetBoxPos(LandCPX2, LandCPY2);
				OffsetX = 0;
				OffsetY = LandCPY - LandCPY2;
				LandPtr->MoveLand(OffsetX, OffsetY);		
				LandPtr->SetLandAttachedPosCad(m_ModelAttachedCadPos);
				LandPtr->SetLandAttachedPosStage(m_ModelAttachedStagePos);
			}
			break;
		case MODEL_LAND_COUNTER_CLOCKWISE://Top to Bottom
			//StartY = (HalfPitchCount*LandPitch+DummyY)+LocalCPY;
			for ( i=0; i<LandIndexCount; i++ )
			{
				LandCPY = StartY-(i*LandPitch);
				LandPtr = this->GetModelLandPtr(LandIndexList[i], true);	
				if ( LandPtr == NULL ) { continue; }
				
				LandPtr->GetLandBoxPtr()->GetBoxPos(LandCPX2, LandCPY2);
				OffsetX = 0;
				OffsetY = LandCPY - LandCPY2;
				LandPtr->MoveLand(OffsetX, OffsetY);		
				LandPtr->SetLandAttachedPosCad(m_ModelAttachedCadPos);
				LandPtr->SetLandAttachedPosStage(m_ModelAttachedStagePos);
			}
			break;
		}
		break;
	case BOX_TOWARD_DOWN://
		switch ( LandDirection )
		{
		case MODEL_LAND_CLOCKWISE://Right to Left
			//StartX = (HalfPitchCount*LandPitch+DummyX)+LocalCPX;
			for ( i=0; i<LandIndexCount; i++ )
			{
				LandCPX = StartX-(i*LandPitch);
				LandPtr = this->GetModelLandPtr(LandIndexList[i], true);	
				if ( LandPtr == NULL ) { continue; }
				
				LandPtr->GetLandBoxPtr()->GetBoxPos(LandCPX2, LandCPY2);
				OffsetX = LandCPX - LandCPX2;
				OffsetY = 0;
				LandPtr->MoveLand(OffsetX, OffsetY);
				LandPtr->SetLandAttachedPosCad(m_ModelAttachedCadPos);
				LandPtr->SetLandAttachedPosStage(m_ModelAttachedStagePos);
			}
			break;
		case MODEL_LAND_COUNTER_CLOCKWISE://Left to Right
			//StartX = -(HalfPitchCount*LandPitch+DummyX)+LocalCPX;
			for ( i=0; i<LandIndexCount; i++ )
			{
				LandCPX = StartX+(i*LandPitch);
				LandPtr = this->GetModelLandPtr(LandIndexList[i], true);	
				if ( LandPtr == NULL ) { continue; }
				
				LandPtr->GetLandBoxPtr()->GetBoxPos(LandCPX2, LandCPY2);
				OffsetX = LandCPX - LandCPX2;
				OffsetY = 0;
				LandPtr->MoveLand(OffsetX, OffsetY);
				LandPtr->SetLandAttachedPosCad(m_ModelAttachedCadPos);
				LandPtr->SetLandAttachedPosStage(m_ModelAttachedStagePos);
			}
			break;
		}
		break;
	case BOX_TOWARD_RIGHT://
		switch ( LandDirection )
		{
		case MODEL_LAND_CLOCKWISE://Top to Bottom
			//StartY = (HalfPitchCount*LandPitch+DummyY)+LocalCPY;
			for ( i=0; i<LandIndexCount; i++ )
			{
				LandCPY = StartY-(i*LandPitch);
				LandPtr = this->GetModelLandPtr(LandIndexList[i], true);	
				if ( LandPtr == NULL ) { continue; }
				
				LandPtr->GetLandBoxPtr()->GetBoxPos(LandCPX2, LandCPY2);
				OffsetX = 0;
				OffsetY = LandCPY - LandCPY2;
				LandPtr->MoveLand(OffsetX, OffsetY);
				LandPtr->SetLandAttachedPosCad(m_ModelAttachedCadPos);
				LandPtr->SetLandAttachedPosStage(m_ModelAttachedStagePos);
			}
			break;
		case MODEL_LAND_COUNTER_CLOCKWISE://Bottom to Top
			//StartY = -(HalfPitchCount*LandPitch+DummyY)+LocalCPY;
			for ( i=0; i<LandIndexCount; i++ )
			{
				LandCPY = StartY+(i*LandPitch);
				LandPtr = this->GetModelLandPtr(LandIndexList[i], true);	
				if ( LandPtr == NULL ) { continue; }
				
				LandPtr->GetLandBoxPtr()->GetBoxPos(LandCPX2, LandCPY2);
				OffsetX = 0;
				OffsetY = LandCPY - LandCPY2;
				LandPtr->MoveLand(OffsetX, OffsetY);
				LandPtr->SetLandAttachedPosCad(m_ModelAttachedCadPos);
				LandPtr->SetLandAttachedPosStage(m_ModelAttachedStagePos);
			}
			break;
		}
		break;
	}	

	SetModelModifiedCount(true);
	SetModelNeedSaveFiles(true);
	UpdateModelLandLeadID();	
	CalcModelTotalRegionAll();
	UpdateModelRegionToAttached();
//	CLocalObj::ModifyLocalRegionKernel();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::ModifyModelLandGroupID(const std::vector<size_t> &LandIndexList, int NewLandGroupID)//修正群組編號
{
	size_t        i=0, j=0, k=0;
	size_t        LandIndex = 0;	
	CAOILand     *LandPtr = NULL;	
	const size_t  LandCount = LandIndexList.size();
	CAOILand     *RefLandPtr = GetModelLandPtrByGroupID(NewLandGroupID, -1);	
	
	CAlgParam    *AlgPtr = NULL;
	CAOIWnd      *WndPtr = NULL;
	CAOILogic    *LogicPtr = NULL;
	bool          bCreateFolder = false;
	size_t        LandWndCount = 0;	
	size_t        LandLogicCount = 0;		
	CString       ModelFolder;
	CString       PatternFolder;
	CString       OldPatternFolder;
	CString       NewPatternFolder;	
	unsigned int  OldWndIndex=0, NewWndIndex=0;
	int           OldWndGroupID=0, NewWndGroupID=0;	
	int           OldLogicGroupID=0, NewLogicGroupID=0;	
	unsigned int  OldPatternFolderIndex=0, NewPatternFolderIndex=0;

	ModelFolder = GetModelFolder();
	UnSelectModelLand(true);	
	if ( NULL == RefLandPtr )
	{
		size_t MaxWndGroupID      = GetModelWndFreeGroupID();
		size_t MaxAlgGroupID      = GetModelAlgFreeGroupID();
		size_t MaxLogicGroupID    = GetModelLogicFreeGroupID();
		std::vector<int> WndGroupIDList;		
		std::vector<int> LogicGroupIDList;
		std::vector<int> WndPatternIndexList;		

		for ( i=0; i<MaxWndGroupID; i++ )
		{	WndGroupIDList.push_back((int)i);	}		
		for ( i=0; i<MaxLogicGroupID; i++ )
		{	LogicGroupIDList.push_back((int)i);	}
		for ( i=0; i<MaxAlgGroupID; i++ )
		{	WndPatternIndexList.push_back(-1);	}

		for ( i=0; i<LandCount; i++ )
		{
			LandIndex = LandIndexList[i];
			LandPtr = CAOIModel::GetModelLandPtr(LandIndex, true);
			if ( NULL == LandPtr ) { continue; }			
			LandPtr->SetLandGroupID(NewLandGroupID);			
			LandPtr->GetLandBoxPtr()->SetBoxSelected(true);
			
			LandWndCount = LandPtr->GetLandWndCount();
			for ( j=0; j<LandWndCount; j++ )
			{
				WndPtr = LandPtr->GetLandWndPtr(j, false);
				if ( NULL == WndPtr ) { continue; }				
				
				OldWndGroupID = WndPtr->GetWndGroupID();
				AlgPtr = WndPtr->GetWndAlgParamPtr();
				OldPatternFolderIndex = WndPtr->GetWndAlgParam().GetAlgGroupID();

				OldWndGroupID = WndPtr->GetWndGroupID();
				if ( OldWndGroupID == WndGroupIDList[OldWndGroupID] )
				{
					WndGroupIDList[OldWndGroupID] = MaxWndGroupID;
					MaxWndGroupID = MaxWndGroupID+1;
				}
				NewWndGroupID = WndGroupIDList[OldWndGroupID];
				WndPtr->SetWndGroupID(NewWndGroupID);				

				NewPatternFolderIndex = WndPtr->GetWndAlgParam().GetAlgGroupID();				
				if ( WndPtr->GetWndAlgParam().GetAlgPatternFileUsed() == true ) 
				{
					OldPatternFolder = WndPtr->GetWndAlgParam().GetAlgPatternFolder();
					PatternFolder = AOIDataDefine.GetAlgPatternFolder(NewPatternFolderIndex);
					NewPatternFolder = ModelFolder+CString(_T("\\"))+PatternFolder;
					WndPtr->GetWndAlgParam().SetAlgPatternFolder(NewPatternFolder);	

					if ( -1 == WndPatternIndexList[OldPatternFolderIndex] )
					{	
						::CreateDirectory(NewPatternFolder, NULL);
						JetAPI::CopyFolderAToFolderB(OldPatternFolder, NewPatternFolder, false, true, _T(""), -1, -1);
						WndPatternIndexList[OldPatternFolderIndex] = NewPatternFolderIndex;
					}					
				}
				
				//AlgPtr->SetAlgPatternFolder
				/*
				OldPatternFolderIndex = AlgPtr->GetAlgPatternFolderIndex();
				if ( OldPatternFolderIndex >= 0 ) //使用樣板
				{
					NewPatternFolderIndex = WndPatternIndexList[OldWndGroupID];
					if ( NewPatternFolderIndex < 0 ) 
					{
						NewPatternFolderIndex = CAOIModel::GetModelFreePatternFolderIndex(true);
						WndPatternIndexList[OldWndGroupID] = NewPatternFolderIndex;
						bCreateFolder = true;
						OldPatternFolder = AlgPtr->GetAlgPatternFolder();
					}
					else 
					{	bCreateFolder = false; }

					PatternFolder = CAlgObj::ObtainAlgPatternFolder(NewPatternFolderIndex);
					AlgPtr->SetAlgPatternFolderIndex(NewPatternFolderIndex);
					FullPatternFolder.Format(_T("%s\\%s"), ModelFolder, PatternFolder);	
					if ( bCreateFolder == true )
					{
						if ( JetAPI::CreateDirectory(FullPatternFolder) == true )
						{	
							JetAPI::CopyFolderAToFolderB(OldPatternFolder, FullPatternFolder, false, true, _T(""), -1, -1);
						}
					}
					AlgPtr->SetAlgPatternFolder(FullPatternFolder);
				}
				*/
			}		
			
			LandLogicCount = LandPtr->GetLandLogicCount();
			for ( j=0; j<LandLogicCount; j++ )
			{
				LogicPtr = LandPtr->GetLandLogicPtr(j, false);
				if ( NULL == LogicPtr ) { continue; }				

				OldLogicGroupID = LogicPtr->GetLogicGroupID();
				if ( OldLogicGroupID == LogicGroupIDList[OldLogicGroupID] )
				{
					LogicGroupIDList[OldLogicGroupID] = MaxLogicGroupID;
					MaxLogicGroupID = MaxLogicGroupID+1;
				}
				NewLogicGroupID = LogicGroupIDList[OldLogicGroupID];
				LogicPtr->SetLogicGroupID(NewLogicGroupID);
			}			
		}
		return true;
	}	
	
	CAOILand  *RefLandPtrT = NULL;
	CAOILand  *RefLandPtrL = NULL;
	CAOILand  *RefLandPtrB = NULL;
	CAOILand  *RefLandPtrR = NULL;	
	BOX_TOWARD LandToward = BOX_TOWARD_NULL;
	const size_t ModelLandCount = GetModelLandCount_Inline();

	for ( i=0; i<ModelLandCount; i++ )
	{
		LandPtr = GetModelLandPtr_Inline(i);
		if ( NULL == LandPtr ) { continue; }		
		if ( LandPtr->GetLandGroupID() != NewLandGroupID ) { continue; }

		LandToward = LandPtr->GetLandToward();
		switch ( LandToward )
		{
		case BOX_TOWARD_UP:			
			if ( NULL == RefLandPtrT ) 
			{	RefLandPtrT = LandPtr; }
			break;
		case BOX_TOWARD_LEFT:			
			if ( NULL == RefLandPtrL ) 
			{	RefLandPtrL = LandPtr; }
			break;
		case BOX_TOWARD_DOWN:			
			if ( NULL == RefLandPtrB ) 
			{	RefLandPtrB = LandPtr; }
			break;
		case BOX_TOWARD_RIGHT:			
			if ( NULL == RefLandPtrR ) 
			{	RefLandPtrR = LandPtr; }			
			break;
		}		
	}	

	//移除舊的檢測框	
	double     OffsetX=0, OffsetY=0;
	double     LandCPX=0, LandCPY=0;	
	std::vector<unsigned int> WndIndexList;
	std::vector<unsigned int> LogicIndexList;

	UnSelectModelWnd();
	UnSelectModelLogic();
	for ( i=0; i<LandCount; i++ )
	{
		LandIndex = LandIndexList[i];
		LandPtr = GetModelLandPtr(LandIndex, true);
		if ( NULL == LandPtr ) { continue; }		
		if ( LandPtr->GetLandGroupID() == NewLandGroupID ) { continue; }

		LandWndCount = LandPtr->GetLandWndCount();
		for ( j=0; j<LandWndCount; j++ )
		{
			WndPtr = LandPtr->GetLandWndPtr(j, false);
			if ( NULL == WndPtr ) { continue; }			
			WndIndexList.push_back(WndPtr->GetWndIndex());
		}		
		
		LandLogicCount = LandPtr->GetLandLogicCount();
		for ( j=0; j<LandLogicCount; j++ )
		{
			LogicPtr = LandPtr->GetLandLogicPtr(j, false);
			if ( NULL == LogicPtr ) { continue; }			
			LogicIndexList.push_back(LogicPtr->GetLogicIndex());
		}
	}

	LandWndCount = WndIndexList.size();
	if ( LandWndCount > 0 )
	{	CAOIModel::DeleteModelWndIndexList(WndIndexList); }

	LandLogicCount = LogicIndexList.size();
	if ( LandLogicCount > 0 ) 
	{	CAOIModel::DeleteModelLogicIndexList(LogicIndexList); }	
	
	size_t     LogicWndCount = 0;
	int        TowardAngle = 0;	
	double     RefLandCPX=0, RefLandCPY=0;
	BOX_TOWARD RefLandToward     = BOX_TOWARD_NULL;
	size_t     RefLandWndCount   = 0;
	size_t     RefLandLogicCount = 0;
	bool       GetTheSameTowardLand = false;

	std::vector<size_t>          WndIndexMap;
	std::vector<size_t>          LogicWndIndexList;
	const size_t ModelWndCount = GetModelWndCount();
	for ( i=0; i<ModelWndCount; i++ )
	{	WndIndexMap.push_back(i);	}	

	for ( i=0; i<LandCount; i++ )
	{
		LandIndex = LandIndexList[i];
		LandPtr = GetModelLandPtr(LandIndex, true);
		if ( NULL == LandPtr ) { continue; }		
		if ( LandPtr->GetLandGroupID() == NewLandGroupID ) 
		{
			LandPtr->GetLandBoxPtr()->SetBoxSelected(true);
			continue; 
		}

		RefLandPtr = NULL;
		GetTheSameTowardLand = true;
		LandToward = LandPtr->GetLandToward();
		switch ( LandToward )
		{
		case BOX_TOWARD_UP:		RefLandPtr = RefLandPtrT;	break;
		case BOX_TOWARD_LEFT:	RefLandPtr = RefLandPtrL;	break;
		case BOX_TOWARD_DOWN:	RefLandPtr = RefLandPtrB;	break;
		case BOX_TOWARD_RIGHT:	RefLandPtr = RefLandPtrR;	break;
		}		
		if ( NULL == RefLandPtr )
		{
			//找其他方位的特徵框
			GetTheSameTowardLand = false;
			if ( RefLandPtrT != NULL ) { RefLandPtr = RefLandPtrT; }
			if ( RefLandPtrL != NULL ) { RefLandPtr = RefLandPtrL; }
			if ( RefLandPtrB != NULL ) { RefLandPtr = RefLandPtrB; }
			if ( RefLandPtrR != NULL ) { RefLandPtr = RefLandPtrR; }
		}
		if ( RefLandPtr == NULL )
		{	return false; }

		LandPtr->GetLandBoxPtr()->GetBoxPos(LandCPX, LandCPY);

		RefLandToward     = RefLandPtr->GetLandToward();
		RefLandWndCount   = RefLandPtr->GetLandWndCount();
		RefLandLogicCount = RefLandPtr->GetLandLogicCount();		
		
		TowardAngle = CAOIBox::CalcBoxTowardAngle(RefLandToward, LandToward);

		*LandPtr = *RefLandPtr;		
		LandPtr->RemoveLandWndList();
		LandPtr->RemoveLandLogicList();
		LandPtr->GetLandBoxPtr()->SetBoxSelected(true);
		
		LandPtr->GetLandBoxPtr()->GetBoxPos(RefLandCPX, RefLandCPY);

		if ( false == GetTheSameTowardLand )
		{	LandPtr->RotateLand(TowardAngle, RefLandCPX, RefLandCPY);	}
		
		OffsetX = LandCPX-RefLandCPX;
		OffsetY = LandCPY-RefLandCPY;

		for ( j=0; j<RefLandWndCount; j++ )
		{
			WndPtr = RefLandPtr->GetLandWndPtr(j, false);
			if ( NULL == WndPtr ) { continue; }			
			OldWndIndex = WndPtr->GetWndIndex();
			WndIndexMap[OldWndIndex] = -1;

			WndPtr = WndPtr->CloneWndObj();
			if ( NULL == WndPtr ) { return false; }

			if ( GetTheSameTowardLand == false )
			{	WndPtr->RotateWnd(TowardAngle, RefLandCPX, RefLandCPY);	}

			AddModelWndPtr(WndPtr, false);
			LandPtr->AddLandWndPtr(WndPtr);

			NewWndIndex = WndPtr->GetWndIndex();					
			WndIndexMap[OldWndIndex] = NewWndIndex;
		}				
		for ( j=0; j<RefLandLogicCount; j++ )
		{
			LogicPtr = RefLandPtr->GetLandLogicPtr(j, false);
			if ( NULL == LogicPtr ) { continue; }			
					
			LogicWndIndexList.clear();
			LogicWndCount = LogicPtr->GetLogicWndPtrCount();
			for ( k=0; k<LogicWndCount; k++ )
			{
				WndPtr = LogicPtr->GetLogicWndPtr(k, false);				
				if ( NULL == WndPtr ) { continue; }
				LogicWndIndexList.push_back(WndPtr->GetWndIndex());
			}

			LogicPtr = LogicPtr->CloneLogicObj();			
			if ( NULL == LogicPtr ) { return false; }
			LogicPtr->RemoveLogicWndPtrList();

			AddModelLogicPtr(LogicPtr, false);
			LandPtr->AddLandLogicPtr(LogicPtr);

			LogicWndCount = LogicWndIndexList.size();
			for ( k=0; k<LogicWndCount; k++ )
			{
				OldWndIndex = LogicWndIndexList[k];
				if ( OldWndIndex < 0 ) { continue; }
				if ( OldWndIndex >= ModelWndCount ) { continue; }
				NewWndIndex = WndIndexMap[OldWndIndex];

				WndPtr = GetModelWndPtr(NewWndIndex, true);
				if ( NULL == WndPtr ) { continue; }				
				LogicPtr->AddLogicWndPtr(WndPtr);
			}
		}
		if ( GetTheSameTowardLand == true )
		{	
			switch ( LandToward )
			{
			case BOX_TOWARD_UP:
			case BOX_TOWARD_DOWN:
				LandPtr->MoveLand(OffsetX, 0); 
				break;
			case BOX_TOWARD_LEFT:
			case BOX_TOWARD_RIGHT:
				LandPtr->MoveLand(0, OffsetY); 
				break;
			default:
				LandPtr->MoveLand(OffsetX, OffsetY);
				break;
			}
		}
		else
		{	LandPtr->MoveLand(OffsetX, OffsetY); }		
	}	

	LayoutModelImageFolderIndexList();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::LinkModelLandAlignID(const std::vector<size_t> &LandIndexList, int LandGroupID)//連動對齊編號
{
	size_t        i=0;	
	size_t        LandIndex = 0;	
	CAOILand     *LandPtr = NULL;		
	int           MinLandAlignID = INT_MAX;
	const size_t  LandCount = LandIndexList.size();
	if ( 0 == LandCount ) { return true; }		
	for ( i=0; i<LandCount; i++ )
	{
		LandIndex = LandIndexList[i];
		LandPtr = GetModelLandPtr(LandIndex, true);
		if ( NULL == LandPtr ) { continue; }	
		if ( LandPtr->GetLandAlignID() < MinLandAlignID )
		{	MinLandAlignID = LandPtr->GetLandAlignID();	}		
	}
	if ( ModifyModelLandAlignID(LandIndexList, LandGroupID, MinLandAlignID) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::UnLinkModelLandAlignID(const std::vector<size_t> &LandIndexList, int LandGroupID)//不連動對齊編號
{
	size_t        i=0;	
	size_t        LandIndex = 0;	
	CAOILand     *LandPtr = NULL;	
	const size_t  LandCount = LandIndexList.size();	
	if ( 0 == LandCount ) { return true; }	

	int AlignID=0;	
	std::set<int> AlignIDList;	
	int MaxLandAlignID = GetModelLandFreeAlignID(LandGroupID);	
	for ( i=0; i<LandCount; i++ )
	{
		LandIndex = LandIndexList[i];
		LandPtr = GetModelLandPtr(LandIndex, true);
		if ( NULL == LandPtr ) { continue; }	
		AlignID = LandPtr->GetLandAlignID();
		auto iter = AlignIDList.find(AlignID);
		if ( iter != AlignIDList.end() )		
		{
			AlignID = MaxLandAlignID;
			LandPtr->SetLandAlignID(AlignID);
			MaxLandAlignID ++;
		}
		AlignIDList.insert(AlignID);
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::ModifyModelLandAlignID(const std::vector<size_t> &LandIndexList, int LandGroupID, int NewLandAlignID)//修正對齊編號
{
	size_t        i=0;	
	size_t        LandIndex = 0;	
	CAOILand     *LandPtr = NULL;	
	const size_t  LandCount = LandIndexList.size();	
	if ( 0 == LandCount ) { return true; }	
	CAOILand     *RefLandPtr = GetModelLandPtrByGroupID(LandGroupID, NewLandAlignID);	
	if ( NULL == RefLandPtr )
	{
		for ( i=0; i<LandCount; i++ )
		{
			LandIndex = LandIndexList[i];
			LandPtr = GetModelLandPtr(LandIndex, true);
			if ( NULL == LandPtr ) { continue; }			
			LandPtr->SetLandAlignID(NewLandAlignID);
		}
		return true;
	}
	
	CAOILand     *RefLandPtrT=NULL;
	CAOILand     *RefLandPtrL=NULL;
	CAOILand     *RefLandPtrB=NULL;
	CAOILand     *RefLandPtrR=NULL;
	BOX_TOWARD LandToward = BOX_TOWARD_NULL;
	const size_t ModelLandCount = GetModelLandCount_Inline();

	for ( i=0; i<ModelLandCount; i++ )
	{
		LandPtr = GetModelLandPtr_Inline(i);
		if ( NULL == LandPtr ) { continue; }		
		if ( LandPtr->GetLandGroupID() != LandGroupID ) { continue; }
		if ( LandPtr->GetLandAlignID() != NewLandAlignID ) { continue; }

		LandToward = LandPtr->GetLandToward();
		switch ( LandToward )
		{
		case BOX_TOWARD_UP:			
			if ( NULL == RefLandPtrT ) 
			{	RefLandPtrT = LandPtr; }
			break;
		case BOX_TOWARD_LEFT:			
			if ( NULL == RefLandPtrL ) 
			{	RefLandPtrL = LandPtr; }
			break;
		case BOX_TOWARD_DOWN:			
			if ( NULL == RefLandPtrB ) 
			{	RefLandPtrB = LandPtr; }
			break;
		case BOX_TOWARD_RIGHT:			
			if ( NULL == RefLandPtrR ) 
			{	RefLandPtrR = LandPtr; }			
			break;
		}		
	}
	
	int    TowardAngle=0;
	double OffsetX=0, OffsetY=0;
	double LandCPX=0, LandCPY=0;
	double RefLandCPX=0, RefLandCPY=0;
	bool   GetTheSameTowardLand=false;
	BOX_TOWARD RefLandToward = BOX_TOWARD_NULL;
	for ( i=0; i<LandCount; i++ )
	{
		LandIndex = LandIndexList[i];
		LandPtr = GetModelLandPtr(LandIndex, true);
		if ( NULL == LandPtr ) { continue; }
		RefLandPtr = NULL;
		LandToward = LandPtr->GetLandToward();
		GetTheSameTowardLand = true;
		switch ( LandToward )
		{
		case BOX_TOWARD_UP:		RefLandPtr = RefLandPtrT;	break;
		case BOX_TOWARD_LEFT:	RefLandPtr = RefLandPtrL;	break;
		case BOX_TOWARD_DOWN:	RefLandPtr = RefLandPtrB;	break;
		case BOX_TOWARD_RIGHT:	RefLandPtr = RefLandPtrR;	break;
		}		
		LandPtr->SetLandAlignID(NewLandAlignID);
		if ( NULL == RefLandPtr )
		{
			//找其他方位的特徵框			
			GetTheSameTowardLand = false;
			if ( RefLandPtrT != NULL ) { RefLandPtr = RefLandPtrT; }
			if ( RefLandPtrL != NULL ) { RefLandPtr = RefLandPtrL; }
			if ( RefLandPtrB != NULL ) { RefLandPtr = RefLandPtrB; }
			if ( RefLandPtrR != NULL ) { RefLandPtr = RefLandPtrR; }
		}		
		if ( NULL == RefLandPtr ) { continue; }

		RefLandToward = RefLandPtr->GetLandToward();
		TowardAngle = CAOIBox::CalcBoxTowardAngle(RefLandToward, LandToward);
		LandPtr->GetLandBoxPtr()->GetBoxPos(LandCPX, LandCPY);

		RefLandPtr->GetLandBoxPtr()->GetBoxPos(RefLandCPX, RefLandCPY);

		if ( false == GetTheSameTowardLand )
		{	JetAPI::RotatePos(TowardAngle, 0, 0, RefLandCPX, RefLandCPY);	}

		OffsetX = LandCPX-RefLandCPX;
		OffsetY = LandCPY-RefLandCPY;

		switch ( LandToward )
		{
		case BOX_TOWARD_UP:
		case BOX_TOWARD_DOWN:
			LandPtr->MoveLand(0, -OffsetY); 			
			break;
		case BOX_TOWARD_LEFT:
		case BOX_TOWARD_RIGHT:
			LandPtr->MoveLand(-OffsetX, 0); 
			break;
		default:
			LandPtr->MoveLand(-OffsetX, -OffsetY);
			break;
		}
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::GetModelLandIndexList(CAOILand *RefLandPtr, bool bSelected, std::vector<size_t> &LandIndexList, double &Pitch, double &MaxPitch)//取得特定方向的焊盤數量
{
	bool IsOK = true;
	if ( RefLandPtr == NULL) { return false; }	
	
	const double ComponentAngle = GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComponentAngle);

	if ( IsExceptionAngle == false )
	{	
		IsOK = GetModelLandIndexListKernel(RefLandPtr, bSelected, LandIndexList, Pitch, MaxPitch); 
	}
	else
	{
		const double CPX = 0;
		const double CPY = 0;
		RotateModel(-ComponentAngle, CPX, CPY);
		IsOK = GetModelLandIndexListKernel(RefLandPtr, bSelected, LandIndexList, Pitch, MaxPitch); 
		RotateModel(ComponentAngle, CPX, CPY);
	}
	return IsOK;	
}
//-------------------------------------------------------------------------------------//
inline bool CAOIModel::GetModelLandIndexListKernel(CAOILand *RefLandPtr, bool bSelected, std::vector<size_t> &LandIndexList, double &Pitch, double &MaxPitch)//取得特定方向的焊盤數量
{
	Pitch = 0;
	MaxPitch = 0;
	LandIndexList.clear();
	if ( NULL == RefLandPtr ) { return false; }	
	
	size_t i=0;
	size_t LandIndex = 0;		
	CAOILand *LandPtr = NULL;	
	const int RefGroupID = RefLandPtr->GetLandGroupID();
	const int RefAlignID = RefLandPtr->GetLandAlignID();
	const BOX_TOWARD RefToward = RefLandPtr->GetLandToward();	
	const size_t LandCount = CAOIModel::GetModelLandCount_Inline();
	double LandPosX=0, LandPosY=0;
	CSortObj    SortObject, *SortObjectPtr=NULL;
	std::vector<CSortObj> SortList;
	const MODEL_LAND_DIRECTION LandDirection = CAOIModel::GetModelLandDirection();
	
	SortList.clear();
	SortObject.SetSortMode(SORT_BY_DBL);	
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = CAOIModel::GetModelLandPtr_Inline(i);
		if ( NULL == LandPtr ) { continue; }
		if ( LandPtr->GetLandToward() != RefToward ) { continue; }
		if ( LandPtr->GetLandGroupID() != RefGroupID ) { continue; }
		if ( LandPtr->GetLandAlignID() != RefAlignID ) { continue; }
		
		if ( true == bSelected )
		{
			if ( LandPtr->GetLandSelected() == false ) 
			{	continue; }
		}

		LandIndex = LandPtr->GetLandIndex();		
		LandPtr->GetLandBoxPtr()->GetBoxPos(LandPosX, LandPosY);

		SortObject.SetID(LandIndex);

		switch ( RefToward )
		{
		case BOX_TOWARD_UP:			
			switch ( LandDirection )
			{
			case MODEL_LAND_CLOCKWISE://Left to Right
				SortObject.SetValueDbl(LandPosX);
				break;
			case MODEL_LAND_COUNTER_CLOCKWISE://Right to Left
				SortObject.SetValueDbl(-LandPosX);
				break;
			}						
			break;
		case BOX_TOWARD_LEFT:
			switch ( LandDirection )
			{
			case MODEL_LAND_CLOCKWISE://Bottom to Top
				SortObject.SetValueDbl(LandPosY);
				break;
			case MODEL_LAND_COUNTER_CLOCKWISE://Top to Bottom
				SortObject.SetValueDbl(-LandPosY);
				break;
			}			
			break;
		case BOX_TOWARD_DOWN:
			switch ( LandDirection )
			{
			case MODEL_LAND_CLOCKWISE://Right to Left
				SortObject.SetValueDbl(-LandPosX);
				break;
			case MODEL_LAND_COUNTER_CLOCKWISE://Left to Right
				SortObject.SetValueDbl(LandPosX);				
				break;
			}			
			break;
		case BOX_TOWARD_RIGHT:
			switch ( LandDirection )
			{
			case MODEL_LAND_CLOCKWISE://Top to Bottom
				SortObject.SetValueDbl(-LandPosY);
				break;
			case MODEL_LAND_COUNTER_CLOCKWISE://Bottom to Top
				SortObject.SetValueDbl(LandPosY);
				break;
			}
			break;
		}		
		SortList.push_back(SortObject);
	}
	const size_t LandIDCount = SortList.size();
	if ( LandIDCount == 0 ) { return true; }

	if ( LandIDCount == 1 ) 
	{
		LandIndexList.push_back(SortList[0].GetID());
		return true; 
	}
	std::sort(SortList.begin(), SortList.end());
	for ( i=0; i<LandIDCount; i++ )
	{	LandIndexList.push_back((unsigned int)(SortList[i].GetID()));	}

	double Value1 = SortList[0].GetValueDbl();
	double Value2 = SortList[1].GetValueDbl();
	double Value3 = SortList[LandIDCount-1].GetValueDbl();
	Pitch = fabs(Value2-Value1);	
	MaxPitch = fabs(Value3-Value1);		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::DeleteModelLandSelected()
{
	size_t              i=0;
	CAOILand           *LandPtr = NULL;
	std::vector<size_t> LandIndexList;
	const size_t LandCount = GetModelLandCount_Inline();		
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = GetModelLandPtr_Inline(i);
		if ( NULL == LandPtr ) { continue; }		
		if ( LandPtr->GetLandSelected() == false )
		{	continue; }
		LandIndexList.push_back(i);
	}	
	if ( LandIndexList.size() == 0 ) { return true; }
	DeleteModelLandIndexList(LandIndexList);
	UpdateModelBodyRgnFromChipLead();
	UpdateModelWndRgnByLinkMode();
	CalcModelTotalRegionAll();
	UpdateModelRegionToAttached();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::CloneModelLandSelected()
{
	size_t     i=0, j=0;
	bool       IsOK = true;
	CAOILand  *LandPtr = NULL;
	CAOILand  *NewLandPtr = NULL;	
	CAOILand  *LastLandPtr = NULL;	
	const size_t LandCount = GetModelLandCount_Inline();		

	IsOK = true;
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = GetModelLandPtr_Inline(i);
		if ( NULL== LandPtr ) { continue; }		
		if ( LandPtr->GetLandSelected() == false )
		{	continue; }
		
		NewLandPtr = CopyModelLand(LandPtr);
		if ( NULL == NewLandPtr ) 
		{	IsOK = false; break;	}

		LastLandPtr = NewLandPtr;
		LandPtr->SetLandAllBoxActived(false);
		LandPtr->SetLandAllBoxSelected(false);		
		NewLandPtr->SetLandAllBoxActived(false);
		NewLandPtr->SetLandAllBoxSelected(false);
		NewLandPtr->GetLandBoxPtr()->SetBoxSelected(true);
	}	

	if ( NULL != LastLandPtr )
	{	LastLandPtr->GetLandBoxPtr()->SetBoxActived(true); }

	if ( false == IsOK )
	{
		for ( j=0; j<LandCount; j++ )
		{
			LandPtr = GetModelLandPtr_Inline(j);
			if ( NULL== LandPtr ) { continue; }		
			if ( LandPtr->GetLandSelected() == false )
			{	continue; }
			LandPtr->SetLandAllBoxSelected(false);
		}
	}
	SetModelModifiedCount(true);
	SetModelNeedSaveFiles(true);
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::CloneMoveModelLandSelected(double MoveX, double MoveY, bool NewAlign)
{
	size_t       i=0, j=0;
	BOX_TOWARD   Toward;
	CAOILand    *LandPtr = NULL;
	CAOILand    *NewLandPtr = NULL;		
	CAOILand    *LastLandPtr = NULL;
	bool         IsOK = true;
	int          TempAlignID = -1;
	int          LandGroupID = -1;
	int          LandAlignID = -1;
	double       fMoveX=0, fMoveY=0;
	std::vector<int> LandAlignIDList;
	const int    MaxLandGroupID = GetModelLandFreeGroupID();
	const double AttachedAngle = GetModelAttachedAngle();	
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(AttachedAngle);
	const size_t LandCount = GetModelLandCount_Inline();

	IsOK = true;
	for ( i=0; i<MaxLandGroupID; i++ )
	{
		LandGroupID = (int)(i);
		LandAlignID = GetModelLandFreeAlignID(LandGroupID);
		LandAlignIDList.push_back(LandAlignID); 
	}

	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = GetModelLandPtr_Inline(i);
		if ( NULL== LandPtr ) { continue; }		
		if ( LandPtr->GetLandSelected() == false )
		{	continue; }
		
		if ( true == NewAlign )
		{
			LandGroupID = LandPtr->GetLandGroupID();
			TempAlignID = LandPtr->GetLandAlignID();
			LandAlignID = LandAlignIDList[LandGroupID];			
			LandAlignID += TempAlignID;
		}

		NewLandPtr = CopyModelLand(LandPtr);
		if ( NULL == NewLandPtr ) 
		{	IsOK = false;	break;	}		

		Toward = LandPtr->GetLandToward();
		if ( true == IsExceptionAngle )
		{	Toward = JetAPI::RotateToward(-AttachedAngle, Toward);	}		

		LastLandPtr = NewLandPtr;
		LandPtr->SetLandAllBoxActived(false);
		LandPtr->SetLandAllBoxSelected(false);		
		NewLandPtr->SetLandAllBoxActived(false);
		NewLandPtr->SetLandAllBoxSelected(false);
		NewLandPtr->GetLandBoxPtr()->SetBoxSelected(true);

		fMoveX = MoveX;
		fMoveY = MoveY;

		if ( true == NewAlign )
		{	
			switch ( Toward )
			{
			case BOX_TOWARD_UP:				
			case BOX_TOWARD_DOWN:
				fMoveX = 0;
				break;
			case BOX_TOWARD_LEFT:
			case BOX_TOWARD_RIGHT:
				fMoveY = 0;
				break;			
			}
			NewLandPtr->SetLandAlignID(LandAlignID);
		}
		else
		{
			switch ( Toward )
			{
			case BOX_TOWARD_UP:				
			case BOX_TOWARD_DOWN:
				fMoveY = 0;
				break;
			case BOX_TOWARD_LEFT:
			case BOX_TOWARD_RIGHT:
				fMoveX = 0;
				break;			
			}
		}
		if ( true == IsExceptionAngle )
		{	JetAPI::RotatePos(AttachedAngle, 0.0, 0.0, fMoveX, fMoveY);	}
		NewLandPtr->MoveLand(fMoveX, fMoveY);
	}
	if ( NULL != LastLandPtr )
	{	LastLandPtr->GetLandBoxPtr()->SetBoxActived(true); }
	if ( IsOK == false )
	{
		for ( j=0; j<LandCount; j++ )
		{
			LandPtr = GetModelLandPtr_Inline(j);
			if ( NULL== LandPtr ) { continue; }		
			if ( LandPtr->GetLandSelected() == false )
			{	continue; }
			LandPtr->SetLandAllBoxActived(false);
			LandPtr->SetLandAllBoxSelected(false);
		}
	}	
	UpdateModelBodyRgnFromChipLead();
	UpdateModelWndRgnByLinkMode();
	SetModelModifiedCount(true);
	SetModelNeedSaveFiles(true);
	CalcModelTotalRegionAll();
	UpdateModelRegionToAttached();
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AlignCenterUModelLandSelected()
{
	bool IsOK = true;	
	const double ComAngle = GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComAngle);
	if ( IsExceptionAngle == false )
	{	IsOK = AlignCenterUModelLandSelectedKernel();	}
	else
	{	
		RotateModel(-ComAngle, 0, 0);
		IsOK = AlignCenterUModelLandSelectedKernel();
		RotateModel(ComAngle, 0, 0);
	}
	CalcModelTotalRegionAll();
	UpdateModelRegionToAttached();
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AlignCenterUModelLandSelectedKernel()
{
	size_t       i=0,j=0;		
	int          LandAlignID=0;
	int          LandGroupID=0;
	int          RefLandAlignID=0;
	int          RefLandGroupID=0;
	CAOILand    *LandPtr = NULL;
	CAOILand    *RefLandPtr = NULL;
	const size_t LandCount = GetModelLandCount_Inline();
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = GetModelLandPtr_Inline(i);
		if ( NULL== LandPtr ) { continue; }	
		LandPtr->SetLandTempInt(FN_DISABLE);				
	}

	for ( j=0; j<LandCount; j++ )
	{
		RefLandPtr = GetModelLandPtr_Inline(j);
		if ( NULL== RefLandPtr ) { continue; }	
		if ( RefLandPtr->GetLandSelected() == false )
		{	continue; }
		if ( FN_ENABLE == RefLandPtr->GetLandTempInt() ) { continue; }

		RefLandAlignID = RefLandPtr->GetLandAlignID();
		RefLandGroupID = RefLandPtr->GetLandGroupID();
		for ( i=0; i<LandCount; i++ )
		{
			LandPtr = GetModelLandPtr_Inline(i);
			if ( NULL== LandPtr ) { continue; }	
			LandAlignID = LandPtr->GetLandAlignID();
			LandGroupID = LandPtr->GetLandGroupID();
			if ( LandAlignID != RefLandAlignID ) { continue; }
			if ( LandGroupID != RefLandGroupID ) { continue; }
			LandPtr->AlignLandU();
			LandPtr->SetLandTempInt(FN_ENABLE);
		}
	}	
	SetModelModifiedCount(true);
	SetModelNeedSaveFiles(true);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::SpinModelLandSelected(double Angle)//自轉模組選到的特徵框
{
	bool IsOK = true;	
	const double ComAngle = GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComAngle);
	if ( IsExceptionAngle == false )
	{	IsOK = SpinModelLandSelectedKernel(Angle);	}
	else
	{	
		RotateModel(-ComAngle, 0, 0);
		IsOK = SpinModelLandSelectedKernel(Angle);
		RotateModel(ComAngle, 0, 0);
	}	
	CalcModelTotalRegionAll();
	UpdateModelRegionToAttached();
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::SpinModelLandSelectedKernel(double Angle)//自轉模組選到的特徵框
{
	size_t     i=0;
	double     CPX=0.0;
	double     CPY=0.0;
	CAOIBox   *BoxPtr = NULL;
	CAOILand  *LandPtr = NULL;	
	const size_t LandCount = GetModelLandCount_Inline();		
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = GetModelLandPtr_Inline(i);
		if ( NULL== LandPtr ) { continue; }
		if ( LandPtr->GetLandSelected() == false )
		{	continue; }

		BoxPtr = LandPtr->GetLandBoxPtr();
		if ( NULL == BoxPtr ) { continue; }
		BoxPtr->GetBoxPos(CPX, CPY);
		LandPtr->RotateLand(Angle, CPX, CPY);
	}
	UpdateModelWndRgnByLinkMode();
	SetModelModifiedCount(true);
	SetModelNeedSaveFiles(true);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::MirrorXModelLandSelected(double CPX, double CPY)
{
	bool IsOK = true;	
	const double ComAngle = GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComAngle);
	if ( IsExceptionAngle == false )
	{	IsOK = MirrorXModelLandSelectedKernel(CPY);	}
	else
	{
		JetAPI::RotatePos(-ComAngle, 0, 0, CPX, CPY);
		RotateModel(-ComAngle, 0, 0);
		IsOK = MirrorXModelLandSelectedKernel(CPY);
		RotateModel(ComAngle, 0, 0);
	}
	CalcModelTotalRegionAll();
	UpdateModelRegionToAttached();
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::MirrorXModelLandSelectedKernel(double CPY)
{
	size_t       i=0;		
	CAOILand    *LandPtr = NULL;			
	const size_t LandCount = GetModelLandCount_Inline();
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = GetModelLandPtr_Inline(i);
		if ( NULL== LandPtr ) { continue; }		
		if ( LandPtr->GetLandSelected() == false )
		{	continue; }
		LandPtr->MirrorLandXAxis(CPY);
	}
	UpdateModelBodyRgnFromChipLead();
	UpdateModelWndRgnByLinkModeKernel();
	SetModelModifiedCount(true);
	SetModelNeedSaveFiles(true);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::MirrorYModelLandSelected(double CPX, double CPY)
{
	bool IsOK = true;	
	const double ComAngle = GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComAngle);
	if ( IsExceptionAngle == false )
	{	IsOK = MirrorYModelLandSelectedKernel(CPX);	}
	else
	{	
		JetAPI::RotatePos(-ComAngle, 0, 0, CPX, CPY);
		RotateModel(-ComAngle, 0, 0);
		IsOK = MirrorYModelLandSelectedKernel(CPX);
		RotateModel(ComAngle, 0, 0);
	}	
	CalcModelTotalRegionAll();
	UpdateModelRegionToAttached();
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::MirrorYModelLandSelectedKernel(double CPX)
{
	size_t     i=0;
	CAOILand  *LandPtr = NULL;
	std::vector<int> LandIndexList;
	const size_t LandCount = GetModelLandCount_Inline();		
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = GetModelLandPtr_Inline(i);
		if ( NULL== LandPtr ) { continue; }
		if ( LandPtr->GetLandSelected() == false )
		{	continue; }
		LandPtr->MirrorLandYAxis(CPX);		
	}
	UpdateModelBodyRgnFromChipLead();
	UpdateModelWndRgnByLinkModeKernel();
	SetModelModifiedCount(true);
	SetModelNeedSaveFiles(true);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::RotateModelLandSelected(double Angle, double CPX, double CPY)
{
	bool IsOK = true;	
	const double ComAngle = GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComAngle);
	if ( IsExceptionAngle == false )
	{	IsOK = RotateModelLandSelectedKernel(Angle, CPX, CPY);	}
	else
	{
		JetAPI::RotatePos(-ComAngle, 0, 0, CPX, CPY);
		RotateModel(-ComAngle, 0, 0);
		IsOK = RotateModelLandSelectedKernel(Angle, CPX, CPY);
		RotateModel(ComAngle, 0, 0);
	}	
	CalcModelTotalRegionAll();
	UpdateModelRegionToAttached();
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::RotateModelLandSelectedKernel(double Angle, double CPX, double CPY)
{
	size_t     i=0;
	CAOILand  *LandPtr = NULL;		
	const size_t LandCount = GetModelLandCount_Inline();		
	const double AttachedAngle = GetModelAttachedAngle();
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = GetModelLandPtr_Inline(i);
		if ( NULL== LandPtr ) { continue; }
		if ( LandPtr->GetLandSelected() == false )
		{	continue; }
		LandPtr->RotateLand(Angle, CPX, CPY);		
		LandPtr->SetLandAttachedAngle(AttachedAngle);
	}
	UpdateModelBodyRgnFromChipLead();
	UpdateModelWndRgnByLinkMode();
	SetModelModifiedCount(true);
	SetModelNeedSaveFiles(true);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelWndToOtherLand(CAOILand *RefLandPtr, CAOIWnd *RefWndPtr, int NewWndBandID)
{
	bool IsOK = true;	
	const double AttachedAngle = GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(AttachedAngle);
	if ( IsExceptionAngle == false )
	{	IsOK = AddModelWndToOtherLandKernel(RefLandPtr, RefWndPtr, NewWndBandID);	}
	else
	{		
		RotateModel(-AttachedAngle, 0, 0);
		RefWndPtr->RotateWnd(-AttachedAngle, 0, 0);
		IsOK = AddModelWndToOtherLandKernel(RefLandPtr, RefWndPtr, NewWndBandID);
		RotateModel(AttachedAngle, 0, 0);
	}	
	CalcModelTotalRegionAll();
	UpdateModelRegionToAttached();
	return IsOK;	
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelWndToOtherLandKernel(CAOILand *RefLandPtr, CAOIWnd *RefWndPtr, int NewWndBandID)
{	
	if ( (NULL==RefLandPtr) || (NULL==RefWndPtr) ) { return false; }

	size_t i=0;
	CAOIWnd   *WndPtr = NULL;
	CAOILand  *LandPtr = NULL;	
	BOX_TOWARD LandToward = BOX_TOWARD_NULL;
	double WndCPX=0, WndCPY=0;	
	double PadCPX=0, PadCPY=0;	
	double OffsetX=0, OffsetY=0;
	double RefWndCPX=0, RefWndCPY=0;
	double RefPadCPX=0, RefPadCPY=0;	
	double RefWndSizeW=0, RefWndSizeH=0;
	double MinX=0, MinY=0, MaxX=0, MaxY=0;
	const double AttachedAngle = GetModelAttachedAngle();
	const int RefLandGroupID = RefLandPtr->GetLandGroupID();
	const int RefLandToward  = RefLandPtr->GetLandToward();
	const size_t LandCount = GetModelLandCount_Inline();

	RefWndPtr->GetWndBox().GetBoxPos(RefWndCPX, RefWndCPY);
	RefLandPtr->GetLandPadBox().GetBoxPos(RefPadCPX, RefPadCPY);
	RefLandPtr->GetLandPadBox().GetBoxSize(RefWndSizeW, RefWndSizeH);
		
	RefLandPtr->GetLandBoxPtr()->GetBoxPos(RefPadCPX, RefPadCPY);
	RefLandPtr->GetLandBoxPtr()->GetBoxSize(RefWndSizeW, RefWndSizeH);	

	OffsetX = RefWndCPX-RefPadCPX;
	OffsetY = RefWndCPY-RefPadCPY;
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = GetModelLandPtr_Inline(i);
		if ( NULL == LandPtr ) { continue; }		
		if ( LandPtr->GetLandGroupID() != RefLandGroupID ) { continue; }

		WndPtr = RefWndPtr->CloneWndObj();
		if ( NULL == WndPtr ) { return false; }		
		AddModelWndPtr(WndPtr, false);
		WndPtr->SetWndBandID(NewWndBandID);

		LandToward = LandPtr->GetLandToward();		
		LandPtr->GetLandBoxPtr()->GetBoxPos(PadCPX, PadCPY);
		if ( LandPtr == RefLandPtr )
		{	WndPtr->SetWndSelected(true);	}
		
		switch ( RefLandToward )
		{
		case BOX_TOWARD_UP:			
			switch ( LandToward )
			{
			case BOX_TOWARD_UP:
				WndCPX = PadCPX+OffsetX;
				WndCPY = PadCPY+OffsetY;
				WndPtr->MoveWnd(WndCPX-RefWndCPX, WndCPY-RefWndCPY);
				break;
			case BOX_TOWARD_LEFT:
				WndPtr->RotateWnd(90, RefWndCPX, RefWndCPY);
				WndCPX = PadCPX-OffsetY;
				WndCPY = PadCPY+OffsetX;
				WndPtr->MoveWnd(WndCPX-RefWndCPX, WndCPY-RefWndCPY);
				break;
			case BOX_TOWARD_DOWN:
				WndPtr->RotateWnd(180, RefWndCPX, RefWndCPY);
				WndCPX = PadCPX-OffsetX;
				WndCPY = PadCPY-OffsetY;
				WndPtr->MoveWnd(WndCPX-RefWndCPX, WndCPY-RefWndCPY);
				break;
			case BOX_TOWARD_RIGHT:
				WndPtr->RotateWnd(270, RefWndCPX, RefWndCPY);
				WndCPX = PadCPX+OffsetY;
				WndCPY = PadCPY-OffsetX;
				WndPtr->MoveWnd(WndCPX-RefWndCPX, WndCPY-RefWndCPY);
				break;		
			}			
			break;
		case BOX_TOWARD_LEFT:
			switch ( LandToward )
			{
			case BOX_TOWARD_UP:
				WndPtr->RotateWnd(270, RefWndCPX, RefWndCPY);
				WndCPX = PadCPX+OffsetY;
				WndCPY = PadCPY-OffsetX;
				WndPtr->MoveWnd(WndCPX-RefWndCPX, WndCPY-RefWndCPY);
				break;
			case BOX_TOWARD_LEFT:
				WndCPX = PadCPX+OffsetX;
				WndCPY = PadCPY+OffsetY;
				WndPtr->MoveWnd(WndCPX-RefWndCPX, WndCPY-RefWndCPY);
				break;
			case BOX_TOWARD_DOWN:
				WndPtr->RotateWnd(90, RefWndCPX, RefWndCPY);
				WndCPX = PadCPX-OffsetY;
				WndCPY = PadCPY+OffsetX;
				WndPtr->MoveWnd(WndCPX-RefWndCPX, WndCPY-RefWndCPY);
				break;
			case BOX_TOWARD_RIGHT:
				WndPtr->RotateWnd(180, RefWndCPX, RefWndCPY);
				WndCPX = PadCPX-OffsetX;
				WndCPY = PadCPY-OffsetY;
				WndPtr->MoveWnd(WndCPX-RefWndCPX, WndCPY-RefWndCPY);
				break;		
			}
			break;
		case BOX_TOWARD_DOWN:
			switch ( LandToward )
			{
			case BOX_TOWARD_UP:
				WndPtr->RotateWnd(180, RefWndCPX, RefWndCPY);
				WndCPX = PadCPX-OffsetX;
				WndCPY = PadCPY-OffsetY;
				WndPtr->MoveWnd(WndCPX-RefWndCPX, WndCPY-RefWndCPY);
				break;
			case BOX_TOWARD_LEFT:
				WndPtr->RotateWnd(270, RefWndCPX, RefWndCPY);
				WndCPX = PadCPX+OffsetY;
				WndCPY = PadCPY-OffsetX;
				WndPtr->MoveWnd(WndCPX-RefWndCPX, WndCPY-RefWndCPY);
				break;
			case BOX_TOWARD_DOWN:
				WndCPX = PadCPX+OffsetX;
				WndCPY = PadCPY+OffsetY;
				WndPtr->MoveWnd(WndCPX-RefWndCPX, WndCPY-RefWndCPY);
				break;
			case BOX_TOWARD_RIGHT:
				WndPtr->RotateWnd(90, RefWndCPX, RefWndCPY);
				WndCPX = PadCPX-OffsetY;
				WndCPY = PadCPY+OffsetX;
				WndPtr->MoveWnd(WndCPX-RefWndCPX, WndCPY-RefWndCPY);
				break;		
			}
			break;
		case BOX_TOWARD_RIGHT:
			switch ( LandToward )
			{
			case BOX_TOWARD_UP:
				WndPtr->RotateWnd(90, RefWndCPX, RefWndCPY);
				WndCPX = PadCPX-OffsetY;
				WndCPY = PadCPY+OffsetX;
				WndPtr->MoveWnd(WndCPX-RefWndCPX, WndCPY-RefWndCPY);
				break;
			case BOX_TOWARD_LEFT:
				WndPtr->RotateWnd(180, RefWndCPX, RefWndCPY);
				WndCPX = PadCPX-OffsetX;
				WndCPY = PadCPY-OffsetY;
				WndPtr->MoveWnd(WndCPX-RefWndCPX, WndCPY-RefWndCPY);
				break;
			case BOX_TOWARD_DOWN:
				WndPtr->RotateWnd(270, RefWndCPX, RefWndCPY);
				WndCPX = PadCPX+OffsetY;
				WndCPY = PadCPY-OffsetX;
				WndPtr->MoveWnd(WndCPX-RefWndCPX, WndCPY-RefWndCPY);
				break;
			case BOX_TOWARD_RIGHT:
				WndCPX = PadCPX+OffsetX;
				WndCPY = PadCPY+OffsetY;
				WndPtr->MoveWnd(WndCPX-RefWndCPX, WndCPY-RefWndCPY);
				break;		
			}
			break;		
		}
		LandPtr->AddLandWndPtr(WndPtr);
		WndPtr->SetWndAttachedAngle(AttachedAngle);
	}	
	SetModelModifiedCount(true);
	SetModelNeedSaveFiles(true);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelLogicToOtherLand(CAOILand *RefLandPtr, CAOILogic *RefLogicPtr)
{	
	if ( (NULL==RefLandPtr) || (NULL==RefLogicPtr) ) { return false; }
	
	size_t    i=0, j=0;
	int       WndBandID=0;
	int       WndGroupID=0;
	size_t    LogicWndCount = 0;
	CAOIWnd   *WndPtr = NULL;	
	CAOILand  *LandPtr = NULL;
	CAOILogic *LogicPtr = NULL;		
	std::vector<int>    WndBandIDList;
	std::vector<int>    WndGroupIDList;
	const int RefLandGroupID = RefLandPtr->GetLandGroupID();
	const size_t LandCount = CAOIModel::GetModelLandCount_Inline();	
	
	LogicWndCount = RefLogicPtr->GetLogicWndPtrCount();
	for ( i=0; i<LogicWndCount; i++ )
	{
		WndPtr = RefLogicPtr->GetLogicWndPtr(i, false);		
		if ( NULL == WndPtr ) { continue; }

		WndBandIDList.push_back(WndPtr->GetWndBandID());
		WndGroupIDList.push_back(WndPtr->GetWndGroupID());
	}
	LogicWndCount = WndGroupIDList.size();
	RefLogicPtr->RemoveLogicWndPtrList();

	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = CAOIModel::GetModelLandPtr_Inline(i);		
		if ( NULL == LandPtr ) { continue; }
		if ( LandPtr->GetLandGroupID() != RefLandGroupID ) { continue; }

		LogicPtr = RefLogicPtr->CloneLogicObj();
		if ( NULL == LogicPtr ) { return false; }		
		CAOIModel::AddModelLogicPtr(LogicPtr, false);

		for ( j=0; j<LogicWndCount; j++ )
		{
			WndBandID = WndBandIDList[j];
			WndGroupID = WndGroupIDList[j];			
			WndPtr = LandPtr->GetLandWndPtrByGroupID(WndGroupID, WndBandID);
			if ( NULL == WndPtr ) { continue; }			
			LogicPtr->AddLogicWndPtr(WndPtr);
		}
		LandPtr->AddLandLogicPtr(LogicPtr);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::ModifyModelLogicToOtherLand(CAOILand *RefLandPtr, CAOILogic *RefLogicPtr)
{
	if ( (NULL==RefLandPtr) || (NULL==RefLogicPtr) ) { return false; }
	if ( RefLogicPtr->GetLogicIsolated() == true ) { return true; }
	
	size_t    i=0, j=0;
	int       WndBandID = 0;
	int       WndGroupID = 0;
	size_t    LogicWndCount = 0;
	CAOIWnd   *WndPtr = NULL;	
	CAOILand  *LandPtr = NULL;
	CAOILogic *LogicPtr = NULL;
	std::vector<int>    WndBandIDList;
	std::vector<int>    WndGroupIDList;
	
	const int     RefLogicGroupID = RefLogicPtr->GetLogicGroupID();
	LOGIC_MODE    RefLogicMode = RefLogicPtr->GetLogicLogicMode();
//	WND_DEFECT_ID RefLogicDefectID = RefLogicPtr->GetLogicDefectID();
	const double  RefLogicParamDBL01 = RefLogicPtr->GetLogicParamDBL1();
	
	const int RefLandGroupID = RefLandPtr->GetLandGroupID();	
	const size_t WndCount = CAOIModel::GetModelWndCount();
	const size_t LandCount = CAOIModel::GetModelLandCount_Inline();	
	
	LogicWndCount = RefLogicPtr->GetLogicWndPtrCount();
	for ( i=0; i<LogicWndCount; i++ )
	{
		WndPtr = RefLogicPtr->GetLogicWndPtr(i, false);		
		if ( NULL == WndPtr ) { continue; }
		WndBandIDList.push_back(WndPtr->GetWndBandID());
		WndGroupIDList.push_back(WndPtr->GetWndGroupID());
	}
	LogicWndCount = WndGroupIDList.size();
	RefLogicPtr->RemoveLogicWndPtrList();
	
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = CAOIModel::GetModelLandPtr_Inline(i);		
		if ( NULL == LandPtr ) { continue; }
		if ( LandPtr->GetLandGroupID() != RefLandGroupID ) { continue; }

		LogicPtr = LandPtr->GetLandLogicPtrByGroupID(RefLogicGroupID);
		if ( NULL == LogicPtr ) { continue; }
		if ( LogicPtr->GetLogicIsolated() == true ) { continue; }

		LogicPtr->SetLogicLogicMode(RefLogicMode);
		//LogicPtr->SetLogicDefectID(RefLogicDefectID);
		LogicPtr->SetLogicParamDBL1(RefLogicParamDBL01);

		LogicPtr->RemoveLogicWndPtrList();
		for ( j=0; j<LogicWndCount; j++ )
		{
			WndBandID = WndBandIDList[j];
			WndGroupID = WndGroupIDList[j];
			WndPtr = LandPtr->GetLandWndPtrByGroupID(WndGroupID, WndBandID);
			if ( NULL == WndPtr ) { continue; }
			LogicPtr->AddLogicWndPtr(WndPtr);
		}
	}
	RefLogicPtr->SetLogicSelected(true);
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIModel::UpdateModelLandTotalRegion()
{
	size_t       i = 0;
	CAOILand    *LandPtr = NULL;
	const size_t LandCount = CAOIModel::GetModelLandCount_Inline();	
	for ( i=0; i<LandCount; i++ )
	{	
		LandPtr = CAOIModel::GetModelLandPtr_Inline(i);
		if ( LandPtr == NULL ) { continue; }
		LandPtr->UpdateLandTotalRegion();
	}
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::CalcModelPadResultOffsetCad(double &OffsetX, double &OffsetY)//計算模組焊盤結果偏移量
{
	int Count = 0;
	CAOILand *LandPtr = NULL;
	TPOINT2D BeforePos, AfterPos, OffsetPos;
	const size_t LandCount = GetModelLandCount();
	OffsetX = OffsetY = 0.0;
	for ( size_t i=0; i<LandCount; i++ )
	{
		LandPtr = GetModelLandPtr(i, false);
		if ( NULL == LandPtr ) { continue; }
		const CAOIBox &PadBox=LandPtr->GetLandPadBox();		
		PadBox.GetBoxPos(BeforePos);
		PadBox.GetBoxPosRes(AfterPos);
		OffsetPos.x = AfterPos.x-BeforePos.x;
		OffsetPos.y = AfterPos.y-BeforePos.y;

		Count ++;
		OffsetX += OffsetPos.x;
		OffsetY += OffsetPos.y;
	}
	if ( Count > 0 )
	{
		OffsetX /= Count;
		OffsetY /= Count;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIModel::GetModelPadRegion(int LandGroupID, int LandAlignID, TREGION4D &Region)//取得模組特徵框範圍
{
	size_t       i = 0;
	bool         bFirst=true;
	TREGION4D    RgnLand;
	CAOILand    *LandPtr = NULL;
	const size_t LandCount = CAOIModel::GetModelLandCount_Inline();		
	bFirst = true;
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = CAOIModel::GetModelLandPtr_Inline(i);
		if ( LandPtr == NULL ) { continue; }
		if ( LandGroupID >= 0 )
		{
			if ( LandPtr->GetLandGroupID() != LandGroupID ) { continue; }
			if ( LandAlignID >= 0 ) 
			{
				if ( LandPtr->GetLandAlignID() != LandAlignID ) { continue; }
			}
		}

		LandPtr->GetLandPadBox().GetBoxRegion(RgnLand);		
		if ( true == bFirst )
		{
			Region = RgnLand;
			bFirst = false;
		}
		else
		{	JetAPI::UnionRegion(Region, RgnLand, Region);	}
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CAOIModel::GetModelLeadRegion(int LandGroupID, int LandAlignID, TREGION4D &Region)//取得模組特徵框範圍
{
	size_t       i = 0;
	bool         bFirst=true;
	TREGION4D    RgnLand;
	CAOILand    *LandPtr = NULL;
	const size_t LandCount = CAOIModel::GetModelLandCount_Inline();		
	bFirst = true;
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = CAOIModel::GetModelLandPtr_Inline(i);
		if ( LandPtr == NULL ) { continue; }
		if ( LandGroupID >= 0 )
		{
			if ( LandPtr->GetLandGroupID() != LandGroupID ) { continue; }
			if ( LandAlignID >= 0 ) 
			{
				if ( LandPtr->GetLandAlignID() != LandAlignID ) { continue; }
			}
		}
		LandPtr->GetLandLeadBox().GetBoxRegion(RgnLand);		
		if ( true == bFirst )
		{
			Region = RgnLand;
			bFirst = false;
		}
		else
		{	JetAPI::UnionRegion(Region, RgnLand, Region);	}
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CAOIModel::GetModelLeadTipRegion(int LandGroupID, int LandAlignID, TREGION4D &Region)//取得模組特徵框範圍
{	
	size_t       i = 0;
	bool         bFirst=true;
	TREGION4D    RgnLand;
	CAOILand    *LandPtr = NULL;
	const size_t LandCount = CAOIModel::GetModelLandCount_Inline();		
	bFirst = true;
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = CAOIModel::GetModelLandPtr_Inline(i);
		if ( LandPtr == NULL ) { continue; }
		if ( LandGroupID >= 0 )
		{
			if ( LandPtr->GetLandGroupID() != LandGroupID ) { continue; }
			if ( LandAlignID >= 0 ) 
			{
				if ( LandPtr->GetLandAlignID() != LandAlignID ) { continue; }
			}
		}
		LandPtr->GetLandLeadTipBox().GetBoxRegion(RgnLand);		
		if ( true == bFirst )
		{
			Region = RgnLand;
			bFirst = false;
		}
		else
		{	JetAPI::UnionRegion(Region, RgnLand, Region);	}		
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CAOIModel::GetModelLeadShoulderRegion(int LandGroupID, int LandAlignID, TREGION4D &Region)//取得模組特徵框範圍
{
	size_t       i = 0;
	bool         bFirst=true;
	TREGION4D    RgnLand;
	CAOILand    *LandPtr = NULL;
	const size_t LandCount = CAOIModel::GetModelLandCount_Inline();	
	bFirst = true;
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = CAOIModel::GetModelLandPtr_Inline(i);
		if ( LandPtr == NULL ) { continue; }
		if ( LandGroupID >= 0 )
		{
			if ( LandPtr->GetLandGroupID() != LandGroupID ) { continue; }
			if ( LandAlignID >= 0 )
			{
				if ( LandPtr->GetLandAlignID() != LandAlignID ) { continue; }
			}
		}
		LandPtr->GetLandLeadShoulderBoxPtr()->GetBoxRegion(RgnLand);		
		if ( true == bFirst )
		{
			Region = RgnLand;
			bFirst = false;
		}
		else
		{	JetAPI::UnionRegion(Region, RgnLand, Region);	}
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CAOIModel::GetModelLeadShoulderTipRegion(int LandGroupID, int LandAlignID, TREGION4D &Region)//取得模組特徵框範圍
{
	size_t       i = 0;
	bool         bFirst=true;
	TREGION4D    RgnLand, RgnLeadTip, RgnLeadShoulder;
	CAOILand    *LandPtr = NULL;
	const size_t LandCount = CAOIModel::GetModelLandCount_Inline();	
	bFirst = true;
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = CAOIModel::GetModelLandPtr_Inline(i);
		if ( LandPtr == NULL ) { continue; }
		if ( LandGroupID >= 0 )
		{
			if ( LandPtr->GetLandGroupID() != LandGroupID ) { continue; }
			if ( LandAlignID >= 0 )
			{
				if ( LandPtr->GetLandAlignID() != LandAlignID ) { continue; }
			}
		}
		LandPtr->GetLandLeadTipBox().GetBoxRegion(RgnLeadTip);
		LandPtr->GetLandLeadShoulderBox().GetBoxRegion(RgnLeadShoulder);
		JetAPI::UnionRegion(RgnLeadTip, RgnLeadShoulder, RgnLand);
		if ( true == bFirst )
		{
			Region = RgnLand;
			bFirst = false;
		}
		else
		{	JetAPI::UnionRegion(Region, RgnLand, Region);	}
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CAOIModel::GetModelLandRegion(int LandGroupID, int LandAlignID, TREGION4D &Region)//取得模組特徵框範圍
{
	size_t       i = 0;
	TREGION4D    RgnBody, RgnLand;
	CAOILand    *LandPtr = NULL;
	const size_t LandCount = CAOIModel::GetModelLandCount_Inline();	
	CAOIModel::GetModelBodyRegion(Region);
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = CAOIModel::GetModelLandPtr_Inline(i);
		if ( LandPtr == NULL ) { continue; }
		if ( LandGroupID >= 0 )
		{
			if ( LandPtr->GetLandGroupID() != LandGroupID ) { continue; }
			if ( LandAlignID >= 0 ) 
			{
				if ( LandPtr->GetLandAlignID() != LandAlignID ) { continue; }
			}
		}
		LandPtr->UpdateLandTotalRegion();
		LandPtr->GetLandTotalRegion(RgnLand);
		JetAPI::UnionRegion(Region, RgnLand, Region);
	}
	return;
}
//-------------------------------------------------------------------------------------//
