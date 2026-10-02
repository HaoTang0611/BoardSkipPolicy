// JETPropertyGridCtrl.cpp : 實作檔
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "JET8000.h"
#include "JETPropertyGridCtrl.h"
//-------------------------------------------------------------------------------------//
#if FRAME_STYLE_TYPE  != FRAME_STYLE_MFC
//-------------------------------------------------------------------------------------//
CJETPropertyGridProperty* CreateGridPropertyWndLogicTypeList(LPCTSTR Caption, CAOIWnd *WndPtr, LPCTSTR Descr)
{
	if ( NULL == WndPtr ) { return NULL; }
	CJETPropertyGridProperty *pProp = NULL;	
	CString strCaption, strValue, strDescr, strLogicType;	
	WND_LOGIC_TYPE LogicType = WndPtr->GetWndLogicType();

	strDescr = Descr;			
	strCaption = Caption;
	strValue = AOIDataDefine.GetWndLogicTypeText(LogicType);
	pProp = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pProp ) { return NULL; }

	LogicType = WND_LOGIC_NONE;
	strLogicType = AOIDataDefine.GetWndLogicTypeText(LogicType);	
	pProp->AddOption(strLogicType);

	LogicType = WND_LOGIC_GROUP_ID;
	strLogicType = AOIDataDefine.GetWndLogicTypeText(LogicType);	
	//pProp->AddOption(strLogicType);//先拿掉

	LogicType = WND_LOGIC_DEFECT_ID;
	strLogicType = AOIDataDefine.GetWndLogicTypeText(LogicType);	
	pProp->AddOption(strLogicType);

	pProp->AllowEdit(FALSE);
	return pProp;
}
//-------------------------------------------------------------------------------------//
CJETPropertyGridProperty* CreateGridPropertyWndLogicGroupIDList(LPCTSTR Caption, CAOIWnd *WndPtr, LPCTSTR Descr)
{
	if ( NULL == WndPtr ) { return NULL; }
	CJETPropertyGridProperty *pProp = NULL;	
	CString strCaption, strValue, strDescr, strGroupID;	
	const int LogicGroupID = WndPtr->GetWndLogicGroupID();

	strDescr = Descr;			
	strCaption = Caption;
	strValue.Format(_T("%d"), LogicGroupID+1);
	pProp = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pProp ) { return NULL; }

	int i=0; 
	int GroupID=0;
	const int MaxLogicGroupID = 8;
	for ( i=0; i<MaxLogicGroupID; i++ )
	{
		GroupID = i;
		strGroupID.Format(_T("%d"), GroupID+1);
		pProp->AddOption(strGroupID);
	}
	pProp->AllowEdit(FALSE);
	return pProp;
}
//-------------------------------------------------------------------------------------//
CJETPropertyGridProperty* CreateGridPropertyWndFollowModeList(LPCTSTR Caption, CAOIWnd *WndPtr, LPCTSTR Descr)
{
	if ( NULL == WndPtr ) { return NULL; }
	CJETPropertyGridProperty *pProp = NULL;		
	CString strCaption, strValue, strDescr, strFollowMode;
	CAOILand *LandPtr = WndPtr->GetWndLandPtr();
	//WND_DEFECT_ID WndDefectID = WndPtr->GetWndDefectID();
	WND_FOLLOW_MODE WndFollowMode = WndPtr->GetWndFollowMode();

	strDescr = Descr;			
	strCaption = Caption;
	strValue = AOIDataDefine.GetWndFollowModeText(WndFollowMode);
	pProp = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pProp ) { return NULL; }

	WndFollowMode = WND_FOLLOW_NONE;
	strFollowMode = AOIDataDefine.GetWndFollowModeText(WndFollowMode);	
	pProp->AddOption(strFollowMode);

	WndFollowMode = WND_FOLLOW_PAD;
	strFollowMode = AOIDataDefine.GetWndFollowModeText(WndFollowMode);	
	pProp->AddOption(strFollowMode);

	WndFollowMode = WND_FOLLOW_PART;
	strFollowMode = AOIDataDefine.GetWndFollowModeText(WndFollowMode);	
	pProp->AddOption(strFollowMode);

	if ( NULL != LandPtr )
	{
		WndFollowMode = WND_FOLLOW_PAD_BODY;
		strFollowMode = AOIDataDefine.GetWndFollowModeText(WndFollowMode);	
		pProp->AddOption(strFollowMode);

		WndFollowMode = WND_FOLLOW_PART_BODY;
		strFollowMode = AOIDataDefine.GetWndFollowModeText(WndFollowMode);	
		pProp->AddOption(strFollowMode);
	}

	pProp->AllowEdit(FALSE);
	return pProp;
}
//-------------------------------------------------------------------------------------//
CJETPropertyGridProperty* CreateGridPropertyWndRgnLinkModeList(LPCTSTR Caption, CAOIWnd *WndPtr, LPCTSTR Descr)
{
	if ( NULL == WndPtr ) { return NULL; }
	CAOIModel *ModelPtr = WndPtr->GetWndModelPtr();
	if ( NULL == ModelPtr ) { return NULL; }	
	CAOILand *LandPtr = WndPtr->GetWndLandPtr();
	CJETPropertyGridProperty *pProp = NULL;	
	CString strCaption, strValue, strDescr, strLinkMode;
	ALG_TYPE AlgType = WndPtr->GetWndAlgType();
	WND_DEFECT_ID WndDefectID = WndPtr->GetWndDefectID();
	WND_RGN_LINK_MODE WndRgnLinkMode = WndPtr->GetWndRgnLinkMode();
	LAND_TYPE LandType = ModelPtr->GetModelLandTypeMaster();
	int LandTypeCount = ModelPtr->GetModelSupportLandTypeCount();
	if ( NULL != LandPtr )
	{	LandType = LandPtr->GetLandType(); }

	strDescr = Descr;			
	strCaption = Caption;
	strValue = AOIDataDefine.GetWndRgnLinkModeText(WndRgnLinkMode);
	pProp = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pProp ) { return NULL; }

	switch ( WndDefectID )
	{
	case WND_DEFECT_PAD_ALIGN://焊盤定位
		WndRgnLinkMode = WND_RGN_LINK_NONE;
		strLinkMode = AOIDataDefine.GetWndRgnLinkModeText(WndRgnLinkMode);	
		pProp->AddOption(strLinkMode);
		
		switch ( AlgType )
		{
		case ALG_EMPTY:
		case ALG_OUTER_SHORT:
		case ALG_EDGE_SEARCH:
		case ALG_OBJECT_MEASURE:		
			break;
		default:
			WndRgnLinkMode = WND_RGN_LINK_PAD;
			strLinkMode = AOIDataDefine.GetWndRgnLinkModeText(WndRgnLinkMode);	
			pProp->AddOption(strLinkMode);
			break;
		}		

		switch ( AlgType )
		{
		case ALG_EMPTY:
		case ALG_OUTER_SHORT:
		case ALG_EDGE_SEARCH:			
		case ALG_OBJECT_MEASURE:
			break;
		default:
			WndRgnLinkMode = WND_RGN_LINK_PAD_TIP;
			strLinkMode = AOIDataDefine.GetWndRgnLinkModeText(WndRgnLinkMode);	
			pProp->AddOption(strLinkMode);	
			break;
		}

		switch ( AlgType )
		{
		case ALG_EMPTY:
			break;
		default:
			WndRgnLinkMode = WND_RGN_LINK_PAD_RGN;
			strLinkMode = AOIDataDefine.GetWndRgnLinkModeText(WndRgnLinkMode);	
			pProp->AddOption(strLinkMode);

			WndRgnLinkMode = WND_RGN_LINK_PAD_RGN_INNER;
			strLinkMode = AOIDataDefine.GetWndRgnLinkModeText(WndRgnLinkMode);	
			pProp->AddOption(strLinkMode);
			break;
		}
		break;
	case WND_DEFECT_PART_ALIGN://本體定位
		WndRgnLinkMode = WND_RGN_LINK_NONE;
		strLinkMode = AOIDataDefine.GetWndRgnLinkModeText(WndRgnLinkMode);	
		pProp->AddOption(strLinkMode);

		switch ( AlgType )
		{
		case ALG_EMPTY:
			break;
		default:
			WndRgnLinkMode = WND_RGN_LINK_BODY;
			strLinkMode = AOIDataDefine.GetWndRgnLinkModeText(WndRgnLinkMode);	
			pProp->AddOption(strLinkMode);
			break;
		}

		if ( ALG_MODEL_MATCH == AlgType )
		{			
			bool bMultiLandType=false;
			if ( LandTypeCount > 1 ) { bMultiLandType = true; }
			if ( CAOILand::GetLandTypeUseLead(LandType)==true || true==bMultiLandType )
			{
				WndRgnLinkMode = WND_RGN_LINK_LEAD;
				strLinkMode = AOIDataDefine.GetWndRgnLinkModeText(WndRgnLinkMode);	
				pProp->AddOption(strLinkMode);
			}
			if ( CAOILand::GetLandTypeUseLeadTip(LandType)==true || true==bMultiLandType )	
			{
				WndRgnLinkMode = WND_RGN_LINK_LEAD_TIP;
				strLinkMode = AOIDataDefine.GetWndRgnLinkModeText(WndRgnLinkMode);	
				pProp->AddOption(strLinkMode);
			}
			if ( CAOILand::GetLandTypeUseLeadShoulder(LandType)==true || true==bMultiLandType )	
			{
				WndRgnLinkMode = WND_RGN_LINK_LEAD_SHOULDER;
				strLinkMode = AOIDataDefine.GetWndRgnLinkModeText(WndRgnLinkMode);	
				pProp->AddOption(strLinkMode);
			}
			if ( CAOILand::GetLandTypeUseLeadTipShoulder(LandType)==true || true==bMultiLandType )
			{	
				WndRgnLinkMode = WND_RGN_LINK_LEAD_TIP_SHOULDER;
				strLinkMode = AOIDataDefine.GetWndRgnLinkModeText(WndRgnLinkMode);	
				pProp->AddOption(strLinkMode);
			}
		}		
		break;
	case WND_DEFECT_PAD_ADJUST://焊盤調整
		WndRgnLinkMode = WND_RGN_LINK_NONE;
		strLinkMode = AOIDataDefine.GetWndRgnLinkModeText(WndRgnLinkMode);	
		pProp->AddOption(strLinkMode);

		WndRgnLinkMode = WND_RGN_LINK_PAD;
		strLinkMode = AOIDataDefine.GetWndRgnLinkModeText(WndRgnLinkMode);	
		pProp->AddOption(strLinkMode);

		switch ( AlgType )
		{
		case ALG_EMPTY:
		case ALG_OUTER_SHORT:		
		case ALG_OBJECT_MEASURE:
			break;
		default:
		case ALG_EDGE_SEARCH:
			WndRgnLinkMode = WND_RGN_LINK_PAD_TIP;
			strLinkMode = AOIDataDefine.GetWndRgnLinkModeText(WndRgnLinkMode);	
			pProp->AddOption(strLinkMode);		
			break;
		}
		break;
	case WND_DEFECT_LEAD_ADJUST://管腳調整
		WndRgnLinkMode = WND_RGN_LINK_NONE;
		strLinkMode = AOIDataDefine.GetWndRgnLinkModeText(WndRgnLinkMode);	
		pProp->AddOption(strLinkMode);

		if ( CAOILand::GetLandTypeUseLead(LandType) == true )
		{
			WndRgnLinkMode = WND_RGN_LINK_LEAD;
			strLinkMode = AOIDataDefine.GetWndRgnLinkModeText(WndRgnLinkMode);	
			pProp->AddOption(strLinkMode);
		}
		if ( CAOILand::GetLandTypeUseLeadTip(LandType) == true )	
		{
			WndRgnLinkMode = WND_RGN_LINK_LEAD_TIP;
			strLinkMode = AOIDataDefine.GetWndRgnLinkModeText(WndRgnLinkMode);	
			pProp->AddOption(strLinkMode);
		}
		if ( CAOILand::GetLandTypeUseLeadShoulder(LandType) == true )	
		{
			WndRgnLinkMode = WND_RGN_LINK_LEAD_SHOULDER;
			strLinkMode = AOIDataDefine.GetWndRgnLinkModeText(WndRgnLinkMode);	
			pProp->AddOption(strLinkMode);
		}
		if ( ALG_MODEL_MATCH == AlgType )
		{
			if ( CAOILand::GetLandTypeUseLeadTipShoulder(LandType) == true )
			{	
				WndRgnLinkMode = WND_RGN_LINK_LEAD_TIP_SHOULDER;
				strLinkMode = AOIDataDefine.GetWndRgnLinkModeText(WndRgnLinkMode);	
				pProp->AddOption(strLinkMode);
			}
		}	
		break;
	case WND_DEFECT_SOLDER_BRIDGE://短路
		WndRgnLinkMode = WND_RGN_LINK_NONE;
		strLinkMode = AOIDataDefine.GetWndRgnLinkModeText(WndRgnLinkMode);	
		pProp->AddOption(strLinkMode);

		switch ( AlgType )
		{
		case ALG_EMPTY:
			break;
		default:
			if ( NULL == LandPtr )
			{
				WndRgnLinkMode = WND_RGN_LINK_BODY;
				strLinkMode = AOIDataDefine.GetWndRgnLinkModeText(WndRgnLinkMode);	
				pProp->AddOption(strLinkMode);

				WndRgnLinkMode = WND_RGN_LINK_PAD_RGN;
				strLinkMode = AOIDataDefine.GetWndRgnLinkModeText(WndRgnLinkMode);	
				pProp->AddOption(strLinkMode);	

				WndRgnLinkMode = WND_RGN_LINK_PAD_BODY_RGN;
				strLinkMode = AOIDataDefine.GetWndRgnLinkModeText(WndRgnLinkMode);	
				pProp->AddOption(strLinkMode);
			}
			else if ( LAND_TYPE_NULL != LandType )
			{
				WndRgnLinkMode = WND_RGN_LINK_PAD;
				strLinkMode = AOIDataDefine.GetWndRgnLinkModeText(WndRgnLinkMode);	
				pProp->AddOption(strLinkMode);	

				if ( LAND_TYPE_PAD != LandType )
				{
					WndRgnLinkMode = WND_RGN_LINK_LEAD;
					strLinkMode = AOIDataDefine.GetWndRgnLinkModeText(WndRgnLinkMode);	
					pProp->AddOption(strLinkMode);	

					if ( LAND_TYPE_IC_LEAD == LandType || LAND_TYPE_CON_LEAD == LandType )
					{
						WndRgnLinkMode = WND_RGN_LINK_LEAD_TIP;
						strLinkMode = AOIDataDefine.GetWndRgnLinkModeText(WndRgnLinkMode);	
						pProp->AddOption(strLinkMode);

						WndRgnLinkMode = WND_RGN_LINK_LEAD_SHOULDER;
						strLinkMode = AOIDataDefine.GetWndRgnLinkModeText(WndRgnLinkMode);	
						pProp->AddOption(strLinkMode);
					}
				}
			}
			break;
		}
		break;
	}		
	pProp->AllowEdit(FALSE);
	return pProp;
}
//-------------------------------------------------------------------------------------//
CJETPropertyGridProperty* CreateGridPropertyWndBoxShapeModeList(LPCTSTR Caption, CAOIWnd *WndPtr, LPCTSTR Descr)
{
	if ( NULL == WndPtr ) { return NULL; }
	CJETPropertyGridProperty *pProp = NULL;	
	CString strCaption, strValue, strDescr, strBoxShape;
	//WND_DEFECT_ID WndDefectID = WndPtr->GetWndDefectID();
	BOX_SHAPE_MODE BoxShapeMode = WndPtr->GetWndBox().GetBoxShapeMode();

	strDescr = Descr;			
	strCaption = Caption;
	strValue = AOIDataDefine.GetBoxShapeModeText(BoxShapeMode);
	pProp = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pProp ) { return NULL; }

	BoxShapeMode = BOX_SHAPE_RECTANGLE;
	strBoxShape = AOIDataDefine.GetBoxShapeModeText(BoxShapeMode);	
	pProp->AddOption(strBoxShape);

	BoxShapeMode = BOX_SHAPE_ROUND_RECT;
	strBoxShape = AOIDataDefine.GetBoxShapeModeText(BoxShapeMode);	
	pProp->AddOption(strBoxShape);

	BoxShapeMode = BOX_SHAPE_ELLIPSE;
	strBoxShape = AOIDataDefine.GetBoxShapeModeText(BoxShapeMode);	
	pProp->AddOption(strBoxShape);

	BoxShapeMode = BOX_SHAPE_CAPSULE;
	strBoxShape = AOIDataDefine.GetBoxShapeModeText(BoxShapeMode);	
	pProp->AddOption(strBoxShape);

	BoxShapeMode = BOX_SHAPE_BULLET;
	strBoxShape = AOIDataDefine.GetBoxShapeModeText(BoxShapeMode);	
	pProp->AddOption(strBoxShape);

	BoxShapeMode = BOX_SHAPE_HALF_ROUND_RECT;
	strBoxShape = AOIDataDefine.GetBoxShapeModeText(BoxShapeMode);	
	pProp->AddOption(strBoxShape);

	BoxShapeMode = BOX_SHAPE_T_SHAPE;
	strBoxShape = AOIDataDefine.GetBoxShapeModeText(BoxShapeMode);	
	pProp->AddOption(strBoxShape);
	
	pProp->AllowEdit(FALSE);
	return pProp;
}
//-------------------------------------------------------------------------------------//
CJETPropertyGridProperty* CreateGridPropertyWndSyncMoveModeList(LPCTSTR Caption, CAOIWnd *WndPtr, LPCTSTR Descr)
{
	if ( NULL == WndPtr ) { return NULL; }
	CJETPropertyGridProperty *pProp = NULL;	
	CString strCaption, strValue, strDescr, strSyncMove;
	//WND_DEFECT_ID WndDefectID = WndPtr->GetWndDefectID();
	CAOILand *LandPtr = WndPtr->GetWndLandPtr();
	WND_SYNC_MOVE_MODE SyncMoveMode = WndPtr->GetWndSyncMoveMode();

	strDescr = Descr;			
	strCaption = Caption;
	strValue = AOIDataDefine.GetWndSyncMoveModeText(SyncMoveMode);
	pProp = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pProp ) { return NULL; }

	SyncMoveMode = WND_SYNC_MOVE_ROTATE;
	strSyncMove = AOIDataDefine.GetWndSyncMoveModeText(SyncMoveMode);	
	pProp->AddOption(strSyncMove);

	SyncMoveMode = WND_SYNC_MOVE_MIRROR;
	strSyncMove = AOIDataDefine.GetWndSyncMoveModeText(SyncMoveMode);	
	pProp->AddOption(strSyncMove);

	SyncMoveMode = WND_SYNC_MOVE_SYMMETRY;
	strSyncMove = AOIDataDefine.GetWndSyncMoveModeText(SyncMoveMode);	
	pProp->AddOption(strSyncMove);
	
	pProp->AllowEdit(FALSE);
	return pProp;
}
//-------------------------------------------------------------------------------------//
CJETPropertyGridProperty* CreateGridPropertyWndConstrainModeList(LPCTSTR Caption, CAOIWnd *WndPtr, LPCTSTR Descr)
{
	if ( NULL == WndPtr ) { return NULL; }
	CJETPropertyGridProperty *pProp = NULL;	
	CString strCaption, strValue, strDescr, strConstrain;
	//WND_DEFECT_ID WndDefectID = WndPtr->GetWndDefectID();
	CAOILand *LandPtr = WndPtr->GetWndLandPtr();
	WND_CONSTRAIN_MODE ConstrainMode = WndPtr->GetWndConstrainMode();

	strDescr = Descr;			
	strCaption = Caption;
	strValue = AOIDataDefine.GetWndConstrainModeText(ConstrainMode);
	pProp = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pProp ) { return NULL; }

	ConstrainMode = WND_CONSTRAIN_DISABLE;
	strConstrain = AOIDataDefine.GetWndConstrainModeText(ConstrainMode);	
	pProp->AddOption(strConstrain);

	if ( NULL != LandPtr )
	{
		ConstrainMode = WND_CONSTRAIN_PAD_RGN_MOVE;
		strConstrain = AOIDataDefine.GetWndConstrainModeText(ConstrainMode);	
		pProp->AddOption(strConstrain);

		ConstrainMode = WND_CONSTRAIN_PAD_RGN_X_MOVE;
		strConstrain = AOIDataDefine.GetWndConstrainModeText(ConstrainMode);	
		//pProp->AddOption(strConstrain);

		ConstrainMode = WND_CONSTRAIN_PAD_RGN_Y_MOVE;
		strConstrain = AOIDataDefine.GetWndConstrainModeText(ConstrainMode);	
		//pProp->AddOption(strConstrain);
		
		ConstrainMode = WND_CONSTRAIN_PAD_RGN_SCALE;
		strConstrain = AOIDataDefine.GetWndConstrainModeText(ConstrainMode);	
		pProp->AddOption(strConstrain);

		ConstrainMode = WND_CONSTRAIN_PAD_RGN_X_SCALE;
		strConstrain = AOIDataDefine.GetWndConstrainModeText(ConstrainMode);	
		//pProp->AddOption(strConstrain);

		ConstrainMode = WND_CONSTRAIN_PAD_RGN_Y_SCALE;
		strConstrain = AOIDataDefine.GetWndConstrainModeText(ConstrainMode);	
		//pProp->AddOption(strConstrain);
	}
	
	pProp->AllowEdit(FALSE);
	return pProp;
}
//-------------------------------------------------------------------------------------//
CJETPropertyGridProperty* CreateGridPropertyAlgBaseValueGroupIDList(LPCTSTR Caption, CAOIWnd *WndPtr, LPCTSTR Descr)
{
	if ( NULL == WndPtr ) { return NULL; }
	CJETPropertyGridProperty *pProp = NULL;	
	CString strCaption, strValue, strDescr, strGroupID;	
	const int BaseValueGroupID = WndPtr->GetWndAlgParam().GetAlgBaseValueGroupID();

	strDescr = Descr;			
	strCaption = Caption;
	strValue.Format(_T("%d"), BaseValueGroupID+1);
	pProp = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pProp ) { return NULL; }

	int   i=0; 
	int   GroupID=0;
	const int MaxGroupID = 8;
	for ( i=0; i<MaxGroupID; i++ )
	{
		GroupID = i;
		strGroupID.Format(_T("%d"), GroupID+1);
		pProp->AddOption(strGroupID);
	}
	pProp->AllowEdit(FALSE);
	return pProp;
}
//-------------------------------------------------------------------------------------//
CJETPropertyGridProperty* CreateGridPropertyAlgEdgeFeatureModeList(LPCTSTR Caption, ALG_EDGE_FEATURE_MODE EdgeMode, LPCTSTR Descr)
{
	CJETPropertyGridProperty *pProp = NULL;	
	CString strCaption, strValue, strDescr, strEdgeMode;
	strDescr = Descr;			
	strCaption = Caption;
	strValue=AOIDataDefine.GetAlgEdgeFeatureText(EdgeMode);		
	pProp = new CJETPropertyGridProperty(strCaption, strValue, strDescr, NULL);	
	if ( NULL == pProp ) { return NULL; }

	EdgeMode = ALG_EDGE_FEATURE_W2B;
	strEdgeMode = AOIDataDefine.GetAlgEdgeFeatureText(EdgeMode);	
	pProp->AddOption(strEdgeMode);

	EdgeMode = ALG_EDGE_FEATURE_B2W;
	strEdgeMode = AOIDataDefine.GetAlgEdgeFeatureText(EdgeMode);	
	pProp->AddOption(strEdgeMode);

	pProp->AllowEdit(FALSE);
	return pProp;
}
//-------------------------------------------------------------------------------------//
CJETPropertyGridProperty* CreateGridPropertyAlgSearchDirectionList(LPCTSTR Caption, ALG_SEARCH_DIRECTION SearchDir, LPCTSTR Descr)
{	
	CJETPropertyGridProperty *pProp = NULL;	
	CString strCaption, strValue, strDescr, strDirection;
	strDescr = Descr;			
	strCaption = Caption;
	strValue=AOIDataDefine.GetAlgSearchDirectionText(SearchDir);		
	pProp = new CJETPropertyGridProperty(strCaption, strValue, strDescr, NULL);	
	if ( NULL == pProp ) { return NULL; }

	SearchDir = SEARCH_DIRECTION_FORWARD;
	strDirection = AOIDataDefine.GetAlgSearchDirectionText(SearchDir);	
	pProp->AddOption(strDirection);

	SearchDir = SEARCH_DIRECTION_BACKWARD;
	strDirection = AOIDataDefine.GetAlgSearchDirectionText(SearchDir);	
	pProp->AddOption(strDirection);

	pProp->AllowEdit(FALSE);
	return pProp;
}
//-------------------------------------------------------------------------------------//
CJETPropertyGridProperty* CreateGridPropertyWndFrameList(LPCTSTR Caption, CAOIProject *pProject, CAOIWnd *WndPtr, unsigned int FrameUniqueID, LPCTSTR Descr)
{
	if ( NULL == WndPtr ) { return NULL; }
	if ( NULL == pProject ) { return NULL; }					
	
	TFrameParam *FrameParamPtr = NULL;
	CJETPropertyGridProperty *pProp = NULL;
	CString strCaption, strValue, strDescr, strFrameName;	
	const size_t FrameCount = pProject->GetProjectFrameUniqueIDCount();
	unsigned int FrameIndex = pProject->GetProjectFrameIndexByUniqueID(FrameUniqueID);
	if ( FrameIndex >= FrameCount ) { return NULL; }

	strDescr = Descr;		
	strCaption = Caption;//_T("畫面來源");
	FrameUniqueID = pProject->GetProjectFrameUniqueID(FrameIndex, true);
	if ( FrameUniqueID == FRAME_UNIQUE_ID_NULL ) { return NULL; }
	FrameParamPtr = AOIDataCollect.GetSystemFrameParamPtrByUniqueID(FrameUniqueID);
	if ( NULL != FrameParamPtr )
	{	strValue = FrameParamPtr->GetFrameGridName();	}
	else
	{	strValue.Format(_T("%d [%d]"), FrameUniqueID, FrameUniqueID);	}

	pProp = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pProp ) { return NULL; }
	size_t  i = 0;		
	for ( i=0; i<FrameCount; i++ )
	{
		FrameUniqueID = pProject->GetProjectFrameUniqueID(i, false);
		FrameParamPtr = AOIDataCollect.GetSystemFrameParamPtrByUniqueID(FrameUniqueID);
		if ( NULL == FrameParamPtr ) { continue; }				
		if (FrameUniqueID == FRAME_UNIQUE_ID_DLP) {
			if (false == WndPtr->CheckWndAlgCanUsing3D()) { continue; }
		}
		strFrameName = FrameParamPtr->GetFrameGridName();
		pProp->AddOption(strFrameName);
	}
	pProp->AllowEdit(FALSE);
	return pProp;
}
//-------------------------------------------------------------------------------------//
CJETPropertyGridProperty* CreateGridPropertyBarcodeDecoderList(LPCTSTR Caption, BARCODE_DECODER_TYPE Decoder, void *Ptr, LPCTSTR Descr)
{
	CJETPropertyGridProperty *pProp = NULL;	
	CString strCaption, strValue, strDescr, strDefect;

	strDescr = Descr;	
	strValue = AOIDataDefine.GetBarcodeDecoderText(Decoder);
	strCaption = Caption;
	pProp = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)Ptr);
	if ( NULL == pProp ) { return NULL; }	

	Decoder = BARCODE_DECODER_EVS;//Open-Evision
	strDefect = AOIDataDefine.GetBarcodeDecoderText(Decoder);
	pProp->AddOption(strDefect);

#ifdef DTK_BARCODE_USE
	Decoder = BARCODE_DECODER_DTK;//DTK
	strDefect = AOIDataDefine.GetBarcodeDecoderText(Decoder);
	pProp->AddOption(strDefect);
#endif//DTK_BARCODE_USE

	if ( AOIDataCollect.CheckHoneywellSwiftDecoderEnabled() == true )
	{
		Decoder = BARCODE_DECODER_HON;//Honeywell
		strDefect = AOIDataDefine.GetBarcodeDecoderText(Decoder);
		pProp->AddOption(strDefect);
	}

	pProp->AllowEdit(FALSE);
	return pProp;
}
//-------------------------------------------------------------------------------------//
CJETPropertyGridProperty* CreateGridPropertyBarcodeSpreadModeList(LPCTSTR Caption, BARCODE_SPREAD_MODE Mode, void *Ptr, LPCTSTR Descr)
{
	CJETPropertyGridProperty *pProp = NULL;	
	CString strCaption, strValue, strDescr, strDefect;

	strDescr = Descr;	
	strValue = AOIDataDefine.GetBarcodeSpreadModeText(Mode);
	strCaption = Caption;
	pProp = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)Ptr);
	if ( NULL == pProp ) { return NULL; }	

	Mode = BARCODE_SPREAD_OFF;
	strDefect = AOIDataDefine.GetBarcodeSpreadModeText(Mode);
	pProp->AddOption(strDefect);

	Mode = BARCODE_SPREAD_LOCAL;
	strDefect = AOIDataDefine.GetBarcodeSpreadModeText(Mode);
	pProp->AddOption(strDefect);

	Mode = BARCODE_SPREAD_ALL;
	strDefect = AOIDataDefine.GetBarcodeSpreadModeText(Mode);
	pProp->AddOption(strDefect);		
	
	pProp->AllowEdit(FALSE);
	return pProp;
}
//-------------------------------------------------------------------------------------//
CJETPropertyGridProperty* CreateGridPropertyBarcodeBelongModeList(LPCTSTR Caption, BARCODE_BELONG_MODE Mode, void *Ptr, LPCTSTR Descr, BARCODE_BELONG_MODE Score)
{
	CJETPropertyGridProperty *pProp = NULL;	
	CString strCaption, strValue, strDescr, strDefect;

	strDescr = Descr;	
	strValue = AOIDataDefine.GetBarcodeBelongModeText(Mode);
	strCaption = Caption;
	pProp = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)Ptr);
	if ( NULL == pProp ) { return NULL; }	

	Mode = BARCODE_BELONG_PROJECT;//專案
	strDefect = AOIDataDefine.GetBarcodeBelongModeText(Mode);
	pProp->AddOption(strDefect);

	Mode = BARCODE_BELONG_TRAY;//載具
	strDefect = AOIDataDefine.GetBarcodeBelongModeText(Mode);
	pProp->AddOption(strDefect);

	Mode = BARCODE_BELONG_COVER;//蓋板
	strDefect = AOIDataDefine.GetBarcodeBelongModeText(Mode);
	pProp->AddOption(strDefect);

	if ( BARCODE_BELONG_BOARD !=  Score )
	{
		Mode = BARCODE_BELONG_PANEL;//整板
		strDefect = AOIDataDefine.GetBarcodeBelongModeText(Mode);
		pProp->AddOption(strDefect);	
	}

	if ( BARCODE_BELONG_PANEL !=  Score )
	{
		Mode = BARCODE_BELONG_BOARD;//單板
		strDefect = AOIDataDefine.GetBarcodeBelongModeText(Mode);
		pProp->AddOption(strDefect);	
	}
	
	pProp->AllowEdit(FALSE);
	return pProp;
}
//-------------------------------------------------------------------------------------//
CJETPropertyGridProperty* CreateGridPropertySaveTestImageModeList(LPCTSTR Caption, SAVE_TEST_IMAGE_MODE Mode, void *Ptr, LPCTSTR Descr)
{
	CJETPropertyGridProperty *pProp = NULL;	
	CString strCaption, strValue, strDescr, strDefect;

	strDescr = Descr;	
	strValue = AOIDataDefine.GetSaveTestImageModeText(Mode);
	strCaption = Caption;
	pProp = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)Ptr);
	if ( NULL == pProp ) { return NULL; }

	
	Mode = SAVE_TEST_IMAGE_DISABLE;//不儲存	
	strDefect = AOIDataDefine.GetSaveTestImageModeText(Mode);
	pProp->AddOption(strDefect);

	Mode = SAVE_TEST_IMAGE_DEFECT;//瑕疵儲存
	strDefect = AOIDataDefine.GetSaveTestImageModeText(Mode);
	pProp->AddOption(strDefect);

	Mode = SAVE_TEST_IMAGE_EVERYONE;//總是儲存
	strDefect = AOIDataDefine.GetSaveTestImageModeText(Mode);
	pProp->AddOption(strDefect);	
	
	pProp->AllowEdit(FALSE);
	return pProp;
}
//-------------------------------------------------------------------------------------//
CJETPropertyGridProperty* CreateGridPropertyBarcodeDeviceIndexList(LPCTSTR Caption, unsigned int BarcodeDeviceIndex, void *Ptr, LPCTSTR Descr)
{
	CJETPropertyGridProperty *pProp = NULL;	
	CString strCaption, strValue, strDescr, strDefect;

	strDescr = Descr;		
	strCaption = Caption;	
	strValue.Format(_T("%d"), BarcodeDeviceIndex+1);
	pProp = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)Ptr);
	if ( NULL == pProp ) { return NULL; }

	size_t       i=0;
	const size_t MaxBarcodeDeviceCount = MAX_BARCODE_DEVICE_COUNT;
	for ( i=0; i<MaxBarcodeDeviceCount; i++ )
	{		
		strDefect.Format(_T("%d"), i+1);
		pProp->AddOption(strDefect);
	}
	pProp->AllowEdit(FALSE);
	return pProp;
}
//-------------------------------------------------------------------------------------//
CJETPropertyGridProperty* CreateGridPropertyBarcodeDeviceCodeIndexList(LPCTSTR Caption, unsigned int CodeIndex, void *Ptr, LPCTSTR Descr)
{
	CJETPropertyGridProperty *pProp = NULL;	
	CString strCaption, strValue, strDescr, strDefect;

	strDescr = Descr;	
	strValue.Format(_T("%d"), CodeIndex+1);
	strCaption = Caption;
	pProp = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)Ptr);
	if ( NULL == pProp ) { return NULL; }

	size_t       i=0;
	const size_t MaxBarcodeCodeCount = MAX_BARCODE_DEVICE_CODE_COUNT;
	for ( i=0; i<MaxBarcodeCodeCount; i++ )
	{		
		strDefect.Format(_T("%d"), i+1);
		pProp->AddOption(strDefect);
	}
	pProp->AllowEdit(FALSE);
	return pProp;
}
//-------------------------------------------------------------------------------------//
CJETPropertyGridProperty* CreateGridPropertyAIModelIDList(LPCTSTR Caption, CAOIWnd *WndPtr, LPCTSTR Descr)
{
	if ( NULL == WndPtr ) { return NULL; }
	
	CJETPropertyGridProperty *pProp = NULL;
	const TALG_PARAM_AI_MODEL &aiParam=WndPtr->GetWndAlgParam().GetAlgParamAiModel();
	ALG_AI_MODEL_ID AIModelID = aiParam.aiModelID;
	CString strCaption, strValue, strDescr, strDefect;

	strDescr = Descr;	
	strValue = AOIDataDefine.GetAIModelIDText(AIModelID);	
	strCaption = Caption;
	pProp = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);
	if ( NULL == pProp ) { return NULL; }
	//===================================================//
	std::vector<ALG_AI_MODEL_ID> AIModelIDList;	
	AOIDataCollect.BuildAIModelIDList(AIModelIDList);
	const size_t Count=AIModelIDList.size();
	for ( size_t i=0; i<Count; i++ )
	{
		AIModelID = AIModelIDList[i];
		strDefect = AOIDataDefine.GetAIModelIDText(AIModelID);	
		pProp->AddOption(strDefect);
	}
	//===================================================//
	pProp->AllowEdit(FALSE);
	return pProp;

}
//-------------------------------------------------------------------------------------//
CJETPropertyGridProperty* CreateGridPropertyWndDefectList(LPCTSTR Caption, CAOIWnd *WndPtr, LPCTSTR Descr)
{
	if ( NULL == WndPtr ) { return NULL; }
	
	CJETPropertyGridProperty *pProp = NULL;
	WND_DEFECT_ID WndDefectID = WndPtr->GetWndDefectID();
	CString strCaption, strValue, strDescr, strDefect;

	strDescr = Descr;	
	strValue = AOIDataDefine.GetWndDefectIDText(WndDefectID);	
	strCaption = Caption;
	pProp = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);
	if ( NULL == pProp ) { return NULL; }
	//===================================================//
	const std::vector<WND_DEFECT_ID> &List=AOIDataCollect.GetWndDefectIDList();
	const size_t Count=List.size();
	for ( size_t i=0; i<Count; i++ )
	{
		WndDefectID = List[i];
		strDefect = AOIDataDefine.GetWndDefectIDText(WndDefectID);	
		pProp->AddOption(strDefect);
	}
	//===================================================//
	pProp->AllowEdit(FALSE);
	return pProp;
}
//-------------------------------------------------------------------------------------//
CJETPropertyGridProperty* CreateGridPropertyRotateAngleList(LPCTSTR Caption, CAOIWnd *WndPtr, LPCTSTR Descr)
{
	CJETPropertyGridProperty *pProp = NULL;	
	CString strCaption, strValue, strDescr;

	strDescr = Descr;	
	strValue = _T("0");
	strCaption = Caption;
	pProp = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);
	if ( NULL == pProp ) { return NULL; }

	strValue = _T("0");
	pProp->AddOption(strValue);

	strValue = _T("90");
	pProp->AddOption(strValue);

	strValue = _T("180");
	pProp->AddOption(strValue);

	strValue = _T("270");
	pProp->AddOption(strValue);

	pProp->AllowEdit(FALSE);
	return pProp;
}
//-------------------------------------------------------------------------------------//
CJETPropertyGridProperty* CreateGridPropertyAlgorithmList(AOI_OBJ_TYPE AttachedType, int WndGroupID, WND_DEFECT_ID WndDefectID, ALG_TYPE AlgType, CAOILand *LandPtr, LPCTSTR Descr)
{	
	CJETPropertyGridProperty *pProp = NULL;
	CString strCaption, strValue, strDescr, strAlgText, strDefect;

	strDescr = Descr;	
	strValue = AOIDataDefine.GetAlgTypeText(AlgType);
	strDefect = AOIDataDefine.GetWndDefectIDText(WndDefectID);;
	strCaption.Format(_T("%s[%02d]"), strDefect, WndGroupID+1);

	pProp = new CJETPropertyGridProperty(strCaption, strValue, strDescr, WndGroupID);	
	if ( NULL == pProp ) { return NULL; }

	CAOILand     *LandPtr2 = LandPtr;
	ALG_TYPE      AlgType2 = AlgType;
	WND_DEFECT_ID DefectID = WndDefectID;	

	//加入目前的
	strAlgText = AOIDataDefine.GetAlgTypeText(AlgType);	
	pProp->AddOption(strAlgText);
	
	if ( AOI_OBJ_FD == AttachedType )
	{
		AlgType2 = ALG_FD_MATCH;//定位點搜尋
		if ( AlgType2 != AlgType )
		{
			if ( CAOIWnd::FilterWndAlgType(AlgType2, DefectID, LandPtr2) == true )
			{
				strAlgText = AOIDataDefine.GetAlgTypeText(AlgType2);	
				pProp->AddOption(strAlgText);
			}
		}

	}
	else if ( AOI_OBJ_BARCODE == AttachedType )
	{
		AlgType2 = ALG_BARCODE_RECOGNIZE;//條碼辨識
		if ( AlgType2 != AlgType )
		{
			if ( CAOIWnd::FilterWndAlgType(AlgType2, DefectID, LandPtr2) == true )
			{
				strAlgText = AOIDataDefine.GetAlgTypeText(AlgType2);	
				pProp->AddOption(strAlgText);
			}
		}
	}
	else
	{
		std::vector<ALG_TYPE> AlgList;
		CAlgParam::BuildAlgTypeList(AlgList);
		const size_t AlgCount=AlgList.size();
		for ( size_t i=0; i<AlgCount; i++ )
		{
			AlgType2 = AlgList[i];
			if ( AlgType2 == AlgType )
			{	continue; }

			if ( CAOIWnd::FilterWndAlgType(AlgType2, DefectID, LandPtr2) == false )
			{	continue; }

			strAlgText = AOIDataDefine.GetAlgTypeText(AlgType2);	
			pProp->AddOption(strAlgText);						
		}
	}
	pProp->AllowEdit(FALSE);
	return pProp;
}
//-------------------------------------------------------------------------------------//
CJETPropertyGridProperty* CreateGridPropertyAlgDirectionList(LPCTSTR Caption, ALG_DIRECTION Direction, LPCTSTR Descr)
{
	CJETPropertyGridProperty *pProp = NULL;
	CString strCaption, strValue, strDescr, strText;

	strDescr = Descr;		
	strCaption = Caption;	
	strValue = AOIDataDefine.GetAlgDirectionText(Direction);

	pProp = new CJETPropertyGridProperty(strCaption, strValue, strDescr);	
	if ( NULL == pProp ) { return NULL; }

	Direction = ALG_HORIZONTAL;//水平
	strText = AOIDataDefine.GetAlgDirectionText(Direction);	
	pProp->AddOption(strText);

	Direction = ALG_VERTICAL;//垂直
	strText = AOIDataDefine.GetAlgDirectionText(Direction);	
	pProp->AddOption(strText);

	pProp->AllowEdit(FALSE);
	return pProp;
}
//-------------------------------------------------------------------------------------//
CJETPropertyGridProperty* CreateGridPropertyAlgMatchDockModeList(LPCTSTR Caption, ALG_MATCH_DOCK_MODE DockMode, void *Ptr, LPCTSTR Descr)
{
	CJETPropertyGridProperty *pProp = NULL;
	CString strCaption, strValue, strDescr, strText;

	strDescr = Descr;		
	strCaption = Caption;	
	strValue = AOIDataDefine.GetAlgMatchDockModeText(DockMode);

	pProp = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)(Ptr));	
	if ( NULL == pProp ) { return NULL; }

	DockMode = ALG_MATCH_DOCK_DISABLE;//關閉
	strText = AOIDataDefine.GetAlgMatchDockModeText(DockMode);	
	pProp->AddOption(strText);

	DockMode = ALG_MATCH_DOCK_TO_TIP;//靠前端
	strText = AOIDataDefine.GetAlgMatchDockModeText(DockMode);	
	pProp->AddOption(strText);

	DockMode = ALG_MATCH_DOCK_TO_SHOULDER;//靠根部
	strText = AOIDataDefine.GetAlgMatchDockModeText(DockMode);	
	pProp->AddOption(strText);

	pProp->AllowEdit(FALSE);
	return pProp;
}
//-------------------------------------------------------------------------------------//
CJETPropertyGridProperty* CreateGridPropertyTiltCellModeList(LPCTSTR Caption, ALG_TILE_CELL_MODE CellMode, LPCTSTR Descr, CAOIWnd *WndPtr)
{
	CJETPropertyGridProperty *pProp = NULL;
	CString strCaption, strValue, strDescr, strText;

	strDescr = Descr;		
	strCaption = Caption;	
	strValue = CAlgParam::GetAlgTiltCellModeText(CellMode);

	pProp = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)(WndPtr));	
	if ( NULL == pProp ) { return NULL; }

	CellMode = ALG_TILE_CELL_HOR;//水平
	strText = CAlgParam::GetAlgTiltCellModeText(CellMode);	
	pProp->AddOption(strText);

	CellMode = ALG_TILE_CELL_VER;//垂直
	strText = CAlgParam::GetAlgTiltCellModeText(CellMode);	
	pProp->AddOption(strText);

	CellMode = ALG_TILE_CELL_CORNER;//角落
	strText = CAlgParam::GetAlgTiltCellModeText(CellMode);	
	pProp->AddOption(strText);

	CellMode = ALG_TILE_CELL_QUAD;//四邊
	strText = CAlgParam::GetAlgTiltCellModeText(CellMode);	
	pProp->AddOption(strText);

	CellMode = ALG_TILE_CELL_OCTA;//八邊
	strText = CAlgParam::GetAlgTiltCellModeText(CellMode);	
	pProp->AddOption(strText);	

	pProp->AllowEdit(FALSE);
	return pProp;
}
//-------------------------------------------------------------------------------------//
CJETPropertyGridProperty* CreateGridPropertyBarcodeDirModeList(LPCTSTR Caption, ALG_BARCODE_DIR_MODE DirMode, LPCTSTR Descr, CAOIWnd *WndPtr)
{
	CJETPropertyGridProperty *pProp = NULL;
	CString strCaption, strValue, strDescr, strText;

	strDescr = Descr;		
	strCaption = Caption;	
	strValue = AOIDataDefine.GetBarcodeDirectionModeText(DirMode);

	pProp = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)(WndPtr));	
	if ( NULL == pProp ) { return NULL; }

	DirMode = ALG_BARCODE_DIR_AUTO;//自動
	strText = AOIDataDefine.GetBarcodeDirectionModeText(DirMode);
	pProp->AddOption(strText);

	DirMode = ALG_BARCODE_DIR_HOR;//水平
	strText = AOIDataDefine.GetBarcodeDirectionModeText(DirMode);
	pProp->AddOption(strText);

	DirMode = ALG_BARCODE_DIR_VER;//垂直
	strText = AOIDataDefine.GetBarcodeDirectionModeText(DirMode);
	pProp->AddOption(strText);

	DirMode = ALG_BARCODE_DIR_ALL;//全部
	strText = AOIDataDefine.GetBarcodeDirectionModeText(DirMode);
	pProp->AddOption(strText);	

	pProp->AllowEdit(FALSE);
	return pProp;
}
//-------------------------------------------------------------------------------------//
CJETPropertyGridProperty* CreateGridPropertyBarcodeStepParam(LPCTSTR Caption, const TALG_BARCODE_STEP_PARAM &StepParam, UINT ParamID, LPCTSTR Descr, CAOIWnd *WndPtr)
{
	CJETPropertyGridProperty *pGroup = NULL;
	CJETPropertyGridProperty *pProp = NULL;
	CString strCaption, strValue, strDescr, strText, strDisable, strResult;
	//WndPtr

	int i=0;
	strDescr = Descr;		
	strValue = AOIDataDefine.GetAlgBarcodeDecodeStepText(StepParam.BarcodeStep);	
	if ( true == StepParam.Decoded )
	{	strResult = _T("OK");	}
	else
	{	strResult = _T("NG");	}
	strCaption.Format(_T("%s [%s] [%s]"), Caption, strValue, strResult);	
	pGroup = new CJETPropertyGridProperty(strCaption);	
	if ( NULL == pGroup ) { return NULL; }
	pGroup->SetID(ParamID);
	pGroup->SetData((DWORD_PTR)(WndPtr));
	
	strCaption = AOIDataDefine.GetMethodText();
	strValue = AOIDataDefine.GetAlgBarcodeDecodeStepText(StepParam.BarcodeStep);	
	pProp = new CJETPropertyGridProperty(strCaption, strValue, strDescr);	
	//pProp = new CJETPropertyGridProperty(strCaption, (DWORD_PTR)(WndPtr), FALSE);
	if ( NULL == pProp ) 
	{
		delete pGroup; pGroup=NULL;
		return NULL; 
	}
	pProp->SetID(ParamID+1);
	pProp->SetData((DWORD_PTR)(WndPtr));
	pGroup->AddSubItem(pProp);
	//pGroup = pProp;
	std::vector<ALG_BARCODE_STEP_MODE> DecodeList;
	CAlgParam::BuildBarcodeDecodeStepList(DecodeList);
	const size_t DecodeCount=(DecodeList.size());
	for ( size_t i=0; i<DecodeCount; i++ )
	{
		strText = AOIDataDefine.GetAlgBarcodeDecodeStepText(DecodeList[i]);
		pProp->AddOption(strText);
	}	
	pProp->AllowEdit(FALSE);

	strDisable = AOIDataDefine.GetAlgBarcodeDecodeStepParamText(ALG_BARCODE_STEP_NONE, 0);
	strCaption = AOIDataDefine.GetAlgBarcodeDecodeStepParamText(StepParam.BarcodeStep, 0);
	strValue.Format(_T("%.2f"), StepParam.Param1);	
	pProp = new CJETPropertyGridProperty(strCaption, strValue, strDescr);	
	if ( NULL == pProp ) 
	{ 
		delete pGroup; pGroup=NULL;
		return NULL; 
	}
	pProp->SetID(ParamID+2);
	pProp->SetData((DWORD_PTR)(WndPtr));
	pGroup->AddSubItem(pProp);
	if ( strDisable == strCaption )
	{	pProp->Show(FALSE, FALSE); }

	strDescr = Descr;		
	strCaption = AOIDataDefine.GetAlgBarcodeDecodeStepParamText(StepParam.BarcodeStep, 1);
	strValue.Format(_T("%.2f"), StepParam.Param2);	
	pProp = new CJETPropertyGridProperty(strCaption, strValue, strDescr);	
	if ( NULL == pProp ) 
	{
		delete pGroup; pGroup=NULL;
		return NULL; 
	}
	pProp->SetID(ParamID+3);
	pProp->SetData((DWORD_PTR)(WndPtr));
	pGroup->AddSubItem(pProp);
	if ( strDisable == strCaption )
	{	pProp->Show(FALSE, FALSE); }
	
	strCaption = AOIDataDefine.GetDecodeText();
	strValue =AOIDataDefine.GetEnableDisableText(StepParam.Enabled);
	pProp = new CJETPropertyGridProperty(strCaption, strValue, strDescr);	
	if ( NULL == pProp ) 
	{
		delete pGroup; pGroup=NULL;
		return NULL; 
	}	
	pProp->SetID(ParamID+4);
	pProp->SetData((DWORD_PTR)(WndPtr));
	pProp->SetCheckValue(StepParam.Enabled);	
	pGroup->AddSubItem(pProp);	

	/*
	COLORREF   clrText=0x000000;
	strDescr = Descr;		
	strCaption = _T("解碼");
	if ( true == StepParam.Decoded )
	{	
		strValue = _T("OK");	
		clrText = 0x00FF00;
	}
	else
	{	
		strValue = _T("NG"); 
		clrText = 0x0000FF;
	}	
	pProp = new CJETPropertyGridProperty(strCaption, strValue, strDescr);	
	if ( NULL == pProp ) 
	{
		delete pGroup; pGroup=NULL;
		return NULL; 
	}
	pProp->SetID(ParamID+4);
	pProp->SetData((DWORD_PTR)(WndPtr));
	pProp->Enable(FALSE);
	pProp->SetValueTextColor(clrText);
	pGroup->AddSubItem(pProp);
	*/

	//if ( strDisable == strCaption )
	//{	pProp->Show(FALSE, FALSE); }	
	
	strCaption = AOIDataDefine.GetAlgBarcodeDecodeStepText(StepParam.BarcodeStep);
	strValue = AOIDataDefine.GetAlgBarcodeDecodeStepText(ALG_BARCODE_STEP_NONE);	
	if ( strCaption == strValue )
	{	pGroup->Expand(FALSE);	}
	return pGroup;
}
//-------------------------------------------------------------------------------------//
CJETPropertyGridProperty* CreateGridPropertyMatchMinReduceAreaList(LPCTSTR Caption, int MinReduceArea, LPCTSTR Descr)
{
	CJETPropertyGridProperty *pProp = NULL;
	CString strCaption, strValue, strDescr, strText;

	int i=0;
	strDescr = Descr;		
	strCaption = Caption;	
	strValue.Format(_T("%d"), MinReduceArea);
	pProp = new CJETPropertyGridProperty(strCaption, strValue, strDescr);	
	if ( NULL == pProp ) { return NULL; }

	MinReduceArea = 64;
	strText.Format(_T("%d"), MinReduceArea);
	pProp->AddOption(strText);

	MinReduceArea = 128;
	strText.Format(_T("%d"), MinReduceArea);
	pProp->AddOption(strText);

	MinReduceArea = 256;
	strText.Format(_T("%d"), MinReduceArea);
	pProp->AddOption(strText);

	MinReduceArea = 512;
	strText.Format(_T("%d"), MinReduceArea);
	pProp->AddOption(strText);
	pProp->AllowEdit(FALSE);

	MinReduceArea = 768;
	strText.Format(_T("%d"), MinReduceArea);
	pProp->AddOption(strText);
	pProp->AllowEdit(FALSE);

	MinReduceArea = 1024;
	strText.Format(_T("%d"), MinReduceArea);
	pProp->AddOption(strText);

	MinReduceArea = 1536;
	strText.Format(_T("%d"), MinReduceArea);
	pProp->AddOption(strText);

	MinReduceArea = 2048;
	strText.Format(_T("%d"), MinReduceArea);
	pProp->AddOption(strText);

	MinReduceArea = 2560;
	strText.Format(_T("%d"), MinReduceArea);
	pProp->AddOption(strText);

	MinReduceArea = 3072;
	strText.Format(_T("%d"), MinReduceArea);
	pProp->AddOption(strText);

	MinReduceArea = 3584;
	strText.Format(_T("%d"), MinReduceArea);
	pProp->AddOption(strText);

	MinReduceArea = 4096;
	strText.Format(_T("%d"), MinReduceArea);
	pProp->AddOption(strText);
	return pProp;
}
//-------------------------------------------------------------------------------------//
CJETPropertyGridProperty* CreateGridPropertyMatchFinalReductionList(LPCTSTR Caption, int FinalReduction, LPCTSTR Descr)
{
	CJETPropertyGridProperty *pProp = NULL;
	CString strCaption, strValue, strDescr, strText;

	int i=0;
	strDescr = Descr;		
	strCaption = Caption;	
	strValue.Format(_T("%d"), FinalReduction);
	pProp = new CJETPropertyGridProperty(strCaption, strValue, strDescr);	
	if ( NULL == pProp ) { return NULL; }

	FinalReduction = 0;
	strText.Format(_T("%d"), FinalReduction);
	pProp->AddOption(strText);

	FinalReduction = 1;
	strText.Format(_T("%d"), FinalReduction);
	pProp->AddOption(strText);

	FinalReduction = 2;
	strText.Format(_T("%d"), FinalReduction);
	pProp->AddOption(strText);
	
	FinalReduction = 3;
	strText.Format(_T("%d"), FinalReduction);
	pProp->AddOption(strText);

	FinalReduction = 4;
	strText.Format(_T("%d"), FinalReduction);
	pProp->AddOption(strText);
	return pProp;
}
//-------------------------------------------------------------------------------------//
IMPLEMENT_DYNAMIC(CJETPropertyGridProperty, CMFCPropertyGridProperty)
//-------------------------------------------------------------------------------------//
CJETPropertyGridProperty::CJETPropertyGridProperty(const CString& strGroupName, DWORD_PTR dwData, BOOL bIsValueList):CMFCPropertyGridProperty(strGroupName, dwData, bIsValueList)
{	
	InitTextFormat();
	m_bReadingMode = FALSE;	
	m_bReadingRight = TRUE;
	
	m_bHasUserBtn = FALSE;
	m_bClickUserBtn = FALSE;

	m_bCheckValue = true;
	m_bCheckValueOrig = m_bCheckValue;
	m_bCheckBoxEnabled = FALSE;
	m_rectCheckBox.SetRectEmpty();
	m_clrValueText = afxGlobalData.clrTextHilite;	
	m_clrCaptionText = afxGlobalData.clrTextHilite;	
	m_clrReadingText = afxGlobalData.clrTextHilite;		
}
//-------------------------------------------------------------------------------------//
CJETPropertyGridProperty::CJETPropertyGridProperty(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr, DWORD_PTR dwData, LPCTSTR lpszEditMask, LPCTSTR lpszEditTemplate, LPCTSTR lpszValidChars):CMFCPropertyGridProperty(strName, varValue, lpszDescr, dwData, lpszEditMask, lpszEditTemplate, lpszValidChars)
{		
	InitTextFormat();
	m_bReadingMode = FALSE;
	m_bReadingRight = TRUE;

	m_bHasUserBtn = FALSE;
	m_bClickUserBtn = FALSE;

	m_bCheckValue = true;
	m_bCheckValueOrig = m_bCheckValue;
	m_bCheckBoxEnabled = FALSE;
	m_rectCheckBox.SetRectEmpty();
	m_clrValueText = afxGlobalData.clrTextHilite;	
	m_clrCaptionText = afxGlobalData.clrTextHilite;	
	m_clrReadingText = afxGlobalData.clrTextHilite;	
}
//-------------------------------------------------------------------------------------//
CJETPropertyGridProperty::~CJETPropertyGridProperty(void)
{
}
//-------------------------------------------------------------------------------------//
CString CJETPropertyGridProperty::FormatProperty()
{
	return CMFCPropertyGridProperty::FormatProperty();
}
//-------------------------------------------------------------------------------------//
inline void CJETPropertyGridProperty::InitTextFormat()
{
	m_strTextFormatShort = _T("%d");
	m_strTextFormatLong  = _T("%ld");
	m_strTextFormatChar  = _T("%c");
	m_strTextFormatUShort= _T("%u");
	m_strTextFormatULong = _T("%u");
	m_strTextFormatFloat = _T("%.8f");
	m_strTextFormatDouble= _T("%.16f");
	return;
}
//-------------------------------------------------------------------------------------//
inline CString CJETPropertyGridProperty::FormatReading()
{
	ASSERT_VALID(this);

	CString strVal;
	COleVariant& var = m_varReading;		
	switch (var.vt)
	{
	case VT_BSTR:
		strVal = var.bstrVal;
		break;

	case VT_I2:
		strVal.Format(m_strTextFormatShort, (short)var.iVal);
		break;

	case VT_I4:
	case VT_INT:
		strVal.Format(m_strTextFormatLong, (long)var.lVal);
		break;

	case VT_UI1:
		if ((BYTE)var.bVal != 0)
		{
			strVal.Format(m_strTextFormatChar, (TCHAR)(BYTE)var.bVal);
		}
		break;

	case VT_UI2:
		strVal.Format( m_strTextFormatUShort, var.uiVal);
		break;

	case VT_UINT:
	case VT_UI4:
		strVal.Format(m_strTextFormatULong, var.ulVal);
		break;

	case VT_R4:
		strVal.Format(m_strTextFormatFloat, (float)var.fltVal);
		break;

	case VT_R8:
		strVal.Format(m_strTextFormatDouble, (double)var.dblVal);
		break;

	case VT_BOOL:
		if ( VARIANT_TRUE == var.boolVal ) 
		{	strVal = _T("Yes"); }
		else
		{	strVal = _T("No"); }		
		break;

	default:
		// Unsupported type
		strVal = _T("*** error ***");
	}

	return strVal;
}
//-------------------------------------------------------------------------------------//
void CJETPropertyGridProperty::ClickCheckBox()
{	
	m_bCheckValue = !(m_bCheckValue);
	m_pWndList->InvalidateRect(m_rectCheckBox);
	m_pWndList->OnPropertyChanged(this);
}
//-------------------------------------------------------------------------------------//
inline void CJETPropertyGridProperty::CalcRect(const CRect &rect, int &nMid, CRect &rcValue, CRect &rcReading, BOOL ReadingOnRight)
{	
	nMid = (rect.left+rect.right)/2;
	rcValue = rcReading = rect;
	if ( TRUE == ReadingOnRight )
	{
		rcValue.right = nMid-1;
		rcReading.left = nMid+1;
	}
	else
	{
		rcReading.right = nMid-1;
		rcValue.left = nMid+1;
	}
}
//-------------------------------------------------------------------------------------//
void CJETPropertyGridProperty::SetCheckValue(BOOL bCheck, BOOL bCheckBoxEnabled)
{
	m_bCheckValue = bCheck;
	m_bCheckValueOrig = m_bCheckValue;
	m_bCheckBoxEnabled = bCheckBoxEnabled;
}
//-------------------------------------------------------------------------------------//
bool CJETPropertyGridProperty::GetCheckValue()
{
	return m_bCheckValue;
}
//-------------------------------------------------------------------------------------//
bool CJETPropertyGridProperty::GetOriginalCheckValue()
{
	return m_bCheckValueOrig;
}
//-------------------------------------------------------------------------------------//
CJETPropertyGridProperty* CJETPropertyGridProperty::FindSubItemByID(UINT ID) const
{	
	ASSERT_VALID(this);
	for (POSITION pos = m_lstSubItems.GetHeadPosition(); pos != NULL;)
	{
		CMFCPropertyGridProperty* pProp = m_lstSubItems.GetNext(pos);
		ASSERT_VALID(pProp);
		if ( pProp->IsKindOf(RUNTIME_CLASS(CJETPropertyGridProperty)) == FALSE ) { continue; }

		CJETPropertyGridProperty* pProp2 = (CJETPropertyGridProperty*)(pProp);
		if (pProp2->m_uID == ID)
		{	return pProp2;	}

		pProp2 = pProp2->FindSubItemByID(ID);
		if (pProp2 != NULL)
		{	return pProp2;	}
	}
	return NULL;
}
//-------------------------------------------------------------------------------------//
void CJETPropertyGridProperty::SetHasUserBtn(BOOL Has, UINT BMPID)
{
	m_bHasUserBtn = Has;
	if ( TRUE == m_bHasUserBtn )
	{	
		CBitmap bmp;
		if ( 0 == BMPID )
		{	BMPID = IDB_PROPERTY_BTN;	}
		bmp.LoadBitmap(BMPID);
		m_BtnImages.DeleteImageList();
		m_BtnImages.Create(16, 16, ILC_MASK | ILC_COLOR24, 0, 0);
		m_BtnImages.Add(&bmp, RGB(0, 0, 0));
	}
}
//-------------------------------------------------------------------------------------//
BOOL CJETPropertyGridProperty::HasButton() const
{
	if ( TRUE == m_bHasUserBtn ) { return TRUE; }
	return CMFCPropertyGridProperty::HasButton();	
}
//-------------------------------------------------------------------------------------//
void CJETPropertyGridProperty::OnClickUserBtn(CPoint point)
{
	m_bClickUserBtn = TRUE;
	m_pWndList->OnPropertyChanged(this);
	m_bClickUserBtn = FALSE;
}
//-------------------------------------------------------------------------------------//
void CJETPropertyGridProperty::OnClickButton(CPoint point)
{
	if ( TRUE == m_bHasUserBtn )
	{	CJETPropertyGridProperty::OnClickUserBtn(point);	}
	else
	{	CMFCPropertyGridProperty::OnClickButton(point);	}	
}
//-------------------------------------------------------------------------------------//
void CJETPropertyGridProperty::OnDrawName(CDC* pDC, CRect rect)
{
	if ( FALSE == m_bCheckBoxEnabled )
	{
		COLORREF clrText = pDC->SetTextColor(m_clrCaptionText);
		CMFCPropertyGridProperty::OnDrawName(pDC, rect);	
		pDC->SetTextColor(clrText);
	}
	else
	{
		m_rectCheckBox = rect;
		m_rectCheckBox.DeflateRect(1, 1);
		m_rectCheckBox.right = m_rectCheckBox.left + m_rectCheckBox.Height();
		rect.left = m_rectCheckBox.right + 1;
		COLORREF clrText = pDC->SetTextColor(m_clrCaptionText);
		CMFCPropertyGridProperty::OnDrawName(pDC, rect);
		pDC->SetTextColor(clrText);
		OnDrawCheckBox(pDC, m_rectCheckBox, m_bCheckValue);
	}
}
//-------------------------------------------------------------------------------------//
void CJETPropertyGridProperty::OnDrawValue(CDC* pDC, CRect rect)
{	
	if ( TRUE == m_bReadingMode )
	{
		int nMid = 0;
		CalcRect(rect, nMid, m_rectValue, m_rectReading, m_bReadingRight);
		pDC->MoveTo(nMid, rect.top);
		pDC->LineTo(nMid, rect.bottom);
		COLORREF clrText = pDC->SetTextColor(m_clrValueText);
		CMFCPropertyGridProperty::OnDrawValue(pDC, m_rectValue);
		pDC->SetTextColor(clrText);
		CJETPropertyGridProperty::OnDrawReading(pDC, m_rectReading);		
	}
	else
	{
		m_rectValue = rect;
		m_rectReading.SetRectEmpty();
		COLORREF clrText = pDC->SetTextColor(m_clrValueText);
		CMFCPropertyGridProperty::OnDrawValue(pDC, rect);
		pDC->SetTextColor(clrText);
	}	
}
//-------------------------------------------------------------------------------------//
void CJETPropertyGridProperty::OnDrawUserBtn(CDC* pDC, CRect rectButton)
{
	int i = 0;
	const int BtnCount = 1;
	CRect rect = rectButton;
	CMFCToolBarButton button;	
	CMFCVisualManager::AFX_BUTTON_STATE state = CMFCVisualManager::ButtonsIsHighlighted;
	for (i = 0; i<BtnCount; i++)
	{	
		CMFCVisualManager::GetInstance()->OnFillButtonInterior(pDC, &button, rect, state);
		m_BtnImages.Draw(pDC, i, CPoint(rect.left, rect.top), ILD_NORMAL);
		CMFCVisualManager::GetInstance()->OnDrawButtonBorder(pDC, &button, rect, state);
	}	
	return;
}
//-------------------------------------------------------------------------------------//
void CJETPropertyGridProperty::OnDrawButton(CDC* pDC, CRect rectButton)
{
	if ( TRUE == m_bHasUserBtn )
	{	CJETPropertyGridProperty::OnDrawUserBtn(pDC, rectButton);	}
	else
	{	CMFCPropertyGridProperty::OnDrawButton(pDC, rectButton);	}
	return;
}
//-------------------------------------------------------------------------------------//
void CJETPropertyGridProperty::OnDrawCheckBox(CDC * pDC, CRect rect, BOOL bChecked)
{
	COLORREF clrTextOld = pDC->GetTextColor();
	CMFCVisualManager::GetInstance()->OnDrawCheckBox(pDC, rect, FALSE, bChecked, m_bEnabled);
	pDC->SetTextColor(clrTextOld);
}
//-------------------------------------------------------------------------------------//
void CJETPropertyGridProperty::OnDrawReading(CDC* pDC, CRect rect)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDC);
	ASSERT_VALID(m_pWndList);

	if ((IsGroup() && !m_bIsValueList) || !HasValueField())
	{	return;	}

	CFont* pOldFont = NULL;
	//if (IsModified() && m_pWndList->m_bMarkModifiedProperties)
	//{	pOldFont = pDC->SelectObject(&m_pWndList->m_fontBold);	}

	CString strVal = FormatReading();
	COLORREF clrText = pDC->SetTextColor(m_clrReadingText);
	rect.DeflateRect(AFX_TEXT_MARGIN, 0);
	
	pDC->DrawText(strVal, rect, DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX | DT_END_ELLIPSIS);

	m_bValueIsTruncated = pDC->GetTextExtent(strVal).cx > rect.Width();

	pDC->SetTextColor(clrText);
	if (pOldFont != NULL)
	{	pDC->SelectObject(pOldFont);	}
}
//-------------------------------------------------------------------------------------//
void CJETPropertyGridProperty::OnClickName(CPoint point)
{
	if ( FALSE == m_bCheckBoxEnabled ) 
	{ 
		CMFCPropertyGridProperty::OnClickName(point);
		/*
		BOOL bEnable = IsEnabled();
		if ( TRUE == bEnable )
		{
			if ( CMFCPropertyGridProperty::OnEdit(NULL) == TRUE ) 
			{			
				if ( NULL!=m_pWndInPlace && m_pWndInPlace->GetSafeHwnd()!=NULL )
				{				
					if ( m_pWndInPlace->IsKindOf(RUNTIME_CLASS(CEdit)) == TRUE )
					{
						CEdit *EditWndPtr = (CEdit*)(m_pWndInPlace);
						EditWndPtr->SetSel(0, -1);
					}
				}
			}
		}*/
		return; 
	}
	if (m_bEnabled && m_rectCheckBox.PtInRect(point))
	{	ClickCheckBox();	}	
}
//-------------------------------------------------------------------------------------//
BOOL CJETPropertyGridProperty::OnDblClk(CPoint point)
{
	if ( FALSE == m_bCheckBoxEnabled ) 
	{ 
		return CMFCPropertyGridProperty::OnDblClk(point); 
	}
	if (m_bEnabled )
	{
		/*
		if ( m_rectCheckBox.PtInRect(point) )
		{	return TRUE;	}
		else
		{	ClickCheckBox(); }
		*/
		if ( m_rectCheckBox.PtInRect(point) )
		{	ClickCheckBox();	}
		else
		{	return CMFCPropertyGridProperty::OnDblClk(point);  }
	}	
	return TRUE;
}
//-------------------------------------------------------------------------------------//
void CJETPropertyGridProperty::AdjustButtonRect()
{
	/*
	if ( FALSE == m_bHasUserBtn ) { return; }
	CJETPropertyGridProperty::AdjustButtonRect();
	m_rectButton.left -= m_rectButton.Width();
	*/	
	if ( FALSE==m_bReadingMode || FALSE==m_bReadingRight )
	{	
		CMFCPropertyGridProperty::AdjustButtonRect();	
		//m_rectButton.left -= m_rectButton.Width();
	}
	else
	{
		CMFCPropertyGridProperty::AdjustButtonRect();
		int nOffsetX = m_rectButton.right-m_rectValue.right;
		m_rectButton.right -= nOffsetX;
		m_rectButton.left  -= nOffsetX;
	}	
}
//-------------------------------------------------------------------------------------//
void CJETPropertyGridProperty::AdjustInPlaceEditRect(CRect& rectEdit, CRect& rectSpin)
{	
	CMFCPropertyGridProperty::AdjustInPlaceEditRect(rectEdit, rectSpin);
	if ( rectEdit.left < m_rectValue.left ) { rectEdit.left = m_rectValue.left; }
	if ( HasButton() == TRUE ) { return; }	
	if ( TRUE == m_bReadingRight )
	{
		if ( rectSpin.IsRectEmpty() )
		{	
			int nOffsetX = m_Rect.right-rectEdit.right;
			rectEdit.right = m_rectValue.right-nOffsetX;	
		}
		else
		{
			int nOffsetX = rectSpin.right-m_rectValue.right;
			rectSpin.right -= nOffsetX;
			rectSpin.left  -= nOffsetX;
			rectEdit.right -= nOffsetX;
		}
	}
	else
	{	rectEdit.left = m_rectValue.left;	}
}
//-------------------------------------------------------------------------------------//
// CJETPropertyGridCtrl
//-------------------------------------------------------------------------------------//
IMPLEMENT_DYNAMIC(CJETPropertyGridCtrl, CMFCPropertyGridCtrl)
//-------------------------------------------------------------------------------------//
CJETPropertyGridCtrl::CJETPropertyGridCtrl()
{
	m_EnableLBtnDbClick = TRUE;
	m_EnableRBtnDbClick = TRUE;
	m_fLeftColumnWidthRatio = 0.5f;
}
//-------------------------------------------------------------------------------------//
CJETPropertyGridCtrl::~CJETPropertyGridCtrl()
{
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CJETPropertyGridCtrl, CMFCPropertyGridCtrl)
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDBLCLK()
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
// CJETPropertyGridCtrl 訊息處理常式
//-------------------------------------------------------------------------------------//
void CJETPropertyGridCtrl::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	EndEditItem();

	//m_nLeftColumnWidth = cx / 2;
	m_nLeftColumnWidth = (int)(cx*m_fLeftColumnWidthRatio);
	AdjustLayout();
}
//-------------------------------------------------------------------------------------//
void CJETPropertyGridCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值

	CMFCPropertyGridCtrl::OnLButtonDown(nFlags, point);
	CMFCPropertyGridProperty *pProp = HitTest(point);
	if ( NULL!=pProp )
	{	GetOwner()->SendMessage(AFX_WM_PROPERTY_LCLICKED, GetDlgCtrlID(), (LPARAM)pProp);	}
}
//-------------------------------------------------------------------------------------//
void CJETPropertyGridCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值

	CMFCPropertyGridCtrl::OnRButtonDown(nFlags, point);
	CMFCPropertyGridProperty *pProp = HitTest(point);
	if ( NULL!=pProp )
	{	GetOwner()->SendMessage(AFX_WM_PROPERTY_RCLICKED, GetDlgCtrlID(), (LPARAM)pProp);	}
}
//-------------------------------------------------------------------------------------//
void CJETPropertyGridCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CMFCPropertyGridCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}
//-------------------------------------------------------------------------------------//
void CJETPropertyGridCtrl::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CMFCPropertyGridCtrl::OnKeyUp(nChar, nRepCnt, nFlags);
}
//-------------------------------------------------------------------------------------//
void CJETPropertyGridCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if ( TRUE == m_EnableLBtnDbClick )
	{	CMFCPropertyGridCtrl::OnLButtonDblClk(nFlags, point);	}
	else
	{	CWnd::OnLButtonDblClk(nFlags, point);	}	

	CMFCPropertyGridProperty *pProp = HitTest(point);
	if ( NULL!=pProp )
	{	GetOwner()->SendMessage(AFX_WM_PROPERTY_LDBCLICK, GetDlgCtrlID(), (LPARAM)pProp);	}	
}
//-------------------------------------------------------------------------------------//
void CJETPropertyGridCtrl::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	if ( TRUE == m_EnableRBtnDbClick )
	{	CMFCPropertyGridCtrl::OnRButtonDblClk(nFlags, point);	}
	else
	{	CWnd::OnRButtonDblClk(nFlags, point);	}		

	CMFCPropertyGridProperty *pProp = HitTest(point);
	if ( NULL!=pProp )
	{	GetOwner()->SendMessage(AFX_WM_PROPERTY_RDBCLICK, GetDlgCtrlID(), (LPARAM)pProp);	}
}
//-------------------------------------------------------------------------------------//
void CJETPropertyGridCtrl::OnPropertyChanged(CMFCPropertyGridProperty* pProp) const
{
	// TODO: 在此加入特定的程式碼和 (或) 呼叫基底類別

	CMFCPropertyGridCtrl::OnPropertyChanged(pProp);
}
//-------------------------------------------------------------------------------------//
void CJETPropertyGridCtrl::OnChangeSelection(CMFCPropertyGridProperty* pNewSel, CMFCPropertyGridProperty* pOldSel)
{
	//ASSERT_VALID(this);	
	CMFCPropertyGridCtrl::OnChangeSelection(pNewSel, pOldSel);
	GetOwner()->SendMessage(AFX_WM_PROPERTY_SEL_CHANGED, GetDlgCtrlID(), (LPARAM)pNewSel);
}
//-------------------------------------------------------------------------------------//
CJETPropertyGridProperty* CJETPropertyGridCtrl::FindItemByID(UINT ID, BOOL bSearchSubItems) const
{
	ASSERT_VALID(this);

	for (POSITION pos = m_lstProps.GetHeadPosition(); pos != NULL;)
	{
		CMFCPropertyGridProperty* pProp = m_lstProps.GetNext(pos);
		ASSERT_VALID(pProp);
		if ( pProp->IsKindOf(RUNTIME_CLASS(CJETPropertyGridProperty)) == FALSE ) { continue; }

		CJETPropertyGridProperty *pProp2 = (CJETPropertyGridProperty*)(pProp);	
		if (pProp2->m_uID == ID)
		{	return pProp2;	}

		if (bSearchSubItems)
		{
			pProp2 = pProp2->FindSubItemByID(ID);

			if (pProp2 != NULL)
			{
				ASSERT_VALID(pProp2);
				return pProp2;
			}
		}
	}
	return NULL;	
}
//-------------------------------------------------------------------------------------//
COLORREF CJETPropertyGridCtrl::SetBackgroundColor(COLORREF clr)// Control background color
{
	COLORREF OldClr = m_clrBackground;
	m_clrBackground = clr;
	return OldClr;
}
//-------------------------------------------------------------------------------------//
COLORREF CJETPropertyGridCtrl::SetTextColor(COLORREF clr)// Control foreground color
{
	COLORREF OldClr = m_clrText;
	m_clrText = clr;
	return OldClr;
}
//-------------------------------------------------------------------------------------//
COLORREF CJETPropertyGridCtrl::SetGroupBackgroundColor(COLORREF clr)// Group background text
{
	COLORREF OldClr = m_clrGroupBackground;
	m_clrGroupBackground = clr;
	return OldClr;
}
//-------------------------------------------------------------------------------------//
COLORREF CJETPropertyGridCtrl::SetGroupTextColor(COLORREF clr)// Group foreground text
{
	COLORREF OldClr = m_clrGroupText;
	m_clrGroupText = clr;
	return OldClr;
}
//-------------------------------------------------------------------------------------//
COLORREF CJETPropertyGridCtrl::SetDescriptionBackgroundColor(COLORREF clr)// Description background text
{
	COLORREF OldClr = m_clrDescriptionBackground;
	m_clrDescriptionBackground = clr;
	return OldClr;
}
//-------------------------------------------------------------------------------------//
COLORREF CJETPropertyGridCtrl::SetDescriptionTextColor(COLORREF clr)// Description foreground text
{
	COLORREF OldClr = m_clrDescriptionText;
	m_clrDescriptionText = clr;
	return OldClr;	
}
//-------------------------------------------------------------------------------------//
COLORREF CJETPropertyGridCtrl::SetLineColor(COLORREF clr)// Color of the grid lines
{
	COLORREF OldClr = m_clrLine;
	m_clrLine = clr;
	return OldClr;
}
//-------------------------------------------------------------------------------------//
#endif//FRAME_STYLE_TYPE
