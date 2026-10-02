// AOIWnd.cpp: implementation of the CAOIWnd class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "AOIWnd.h"
//-------------------------------------------------------------------------------------//
#include "AOILand.h"
#include "AOIModel.h"
#include "AOIFileIO.h"
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
IMPLEMENT_DYNAMIC(CAOIWnd, CAOIObj)
//-------------------------------------------------------------------------------------//
bool CAOIWnd::FilterWndDefectID(WND_DEFECT_ID DefectID, CAOILand *LandPtr)//過濾檢測框瑕疵代碼
{
	bool bSuit=true;
	LAND_TYPE LandType=LAND_TYPE_NULL;
	if ( NULL != LandPtr )
	{	LandType = LandPtr->GetLandType(); }
	switch ( DefectID )
	{
	case WND_DEFECT_PAD_ALIGN:
		if ( NULL != LandPtr ) { bSuit = false; }
		break;
	case WND_DEFECT_PAD_ADJUST:
		if ( NULL == LandPtr ) { bSuit = false; }
		break;
	case WND_DEFECT_LEAD_ADJUST:
		if ( NULL == LandPtr ) { bSuit = false; }
		if ( LAND_TYPE_PAD == LandType ) { bSuit = false; }
		break;
	case WND_DEFECT_LEAD_LIFTED:
		if ( NULL == LandPtr ) { bSuit = false; }
		if ( LAND_TYPE_NULL == LandType ) { bSuit = false; }
		if ( LAND_TYPE_PAD == LandType ) { bSuit = false; }
		//if ( LAND_TYPE_ELECTRODE == LandType ) { bSuit = false; }		
		break;
	case WND_DEFECT_CLASS_CHECK:
		break;
	case WND_DEFECT_BASE_VALUE:
		break;
	case WND_DEFECT_LEAD_BENDED:
	case WND_DEFECT_LEAD_PROTRUDED:
		if ( NULL == LandPtr ) { bSuit = false; }
		if ( LAND_TYPE_NULL == LandType ) { bSuit = false; }
		if ( LAND_TYPE_PAD == LandType ) { bSuit = false; }
		if ( LAND_TYPE_ELECTRODE == LandType ) { bSuit = false; }		
		break;
	case WND_DEFECT_PART_ALIGN:
		if ( NULL != LandPtr ) { bSuit = false; }
		break;
	case WND_DEFECT_BODY_MISSING:
	case WND_DEFECT_BODY_OFFSET:
	case WND_DEFECT_BODY_TILT:
	case WND_DEFECT_BODY_POLARITY:
	case WND_DEFECT_BODY_TURNOVER:
	case WND_DEFECT_BODY_MOUNT:
	case WND_DEFECT_BODY_WRONG_CODE:
	case WND_DEFECT_BODY_WRONG_TEXT:
	case WND_DEFECT_BODY_TOMBSTONE:
	case WND_DEFECT_BODY_BILLBOARD:
	case WND_DEFECT_BODY_DAMAGED:
		if ( NULL != LandPtr ) { bSuit = false; }
		break;
	case WND_DEFECT_SOLDER_BRIDGE:
		break;
	case WND_DEFECT_SOLDER_POOR:
	case WND_DEFECT_SOLDER_OPEN:
	case WND_DEFECT_SOLDER_PAD_EXPOSED:			
	case WND_DEFECT_SOLDER_BEAD:
	case WND_DEFECT_SOLDER_EXCESS:
		if ( NULL == LandPtr ) { bSuit = false; }
		break;
	case WND_DEFECT_PAD_SCRATCH:
		if ( NULL == LandPtr ) { bSuit = false; }
		break;
	case WND_DEFECT_FOREIGN_BODY:
		break;
	case WND_DEFECT_USER_DEFINE_01:
	case WND_DEFECT_USER_DEFINE_02:
	case WND_DEFECT_USER_DEFINE_03:
	case WND_DEFECT_USER_DEFINE_04:
	case WND_DEFECT_USER_DEFINE_05:
	case WND_DEFECT_USER_DEFINE_06:
	case WND_DEFECT_USER_DEFINE_07:
	case WND_DEFECT_USER_DEFINE_08:
	case WND_DEFECT_USER_DEFINE_09:
	case WND_DEFECT_USER_DEFINE_10:
		bSuit = true;
		break;
	}	
	return bSuit;
}
//-------------------------------------------------------------------------------------//
bool CAOIWnd::FilterWndAlgType(ALG_TYPE AlgType, WND_DEFECT_ID WndDefectID, CAOILand *LandPtr)
{
	bool bSuit = true;
	switch ( AlgType )
	{
	case ALG_BRIGHT_RATIO:	
		switch ( WndDefectID )
		{
		case WND_DEFECT_PAD_ALIGN:
		case WND_DEFECT_PART_ALIGN:
		case WND_DEFECT_PAD_ADJUST:
		case WND_DEFECT_LEAD_ADJUST:
		case WND_DEFECT_BODY_WRONG_CODE:
		case WND_DEFECT_BODY_WRONG_TEXT:
			bSuit = false;
			break;
		}
		break;	
	case ALG_OUTER_SHORT:	
		switch ( WndDefectID )
		{
		case WND_DEFECT_SOLDER_BRIDGE:
		case WND_DEFECT_SOLDER_BEAD:
		case WND_DEFECT_SOLDER_EXCESS:
		case WND_DEFECT_LEAD_PROTRUDED:
		case WND_DEFECT_LEAD_BENDED:
		case WND_DEFECT_FOREIGN_BODY:
			bSuit = true;
			break;
		default:
			bSuit = false;
			break;
		}
		break;	
	case ALG_BODY_TILT:
		switch ( WndDefectID )
		{
		case WND_DEFECT_BODY_OFFSET:
		case WND_DEFECT_BODY_TILT:
		case WND_DEFECT_BODY_POLARITY:
		case WND_DEFECT_BODY_TURNOVER:
		case WND_DEFECT_BODY_MOUNT:
			bSuit = true;
			break;
		default:
			bSuit = false;
			break;
		}
		break;	
	case ALG_BLOB_COUNT:
		switch ( WndDefectID )
		{
		case WND_DEFECT_PAD_ALIGN:
		case WND_DEFECT_PART_ALIGN:
		case WND_DEFECT_PAD_ADJUST:
		case WND_DEFECT_LEAD_ADJUST:
		case WND_DEFECT_BASE_VALUE:
		case WND_DEFECT_BODY_WRONG_CODE:
		case WND_DEFECT_BODY_WRONG_TEXT:
			bSuit = false;
			break;
		}
		break;
	case ALG_BARCODE_RECOGNIZE:
		switch ( WndDefectID )
		{
		case WND_DEFECT_CLASS_CHECK:
		case WND_DEFECT_BODY_WRONG_CODE:
		case WND_DEFECT_BODY_WRONG_TEXT:
			bSuit = true;
			break;
		default:
			bSuit = false;
			break;
		}
		break;
	case ALG_OBJECT_MEASURE:
		switch ( WndDefectID )
		{
		case WND_DEFECT_PAD_ALIGN:
		case WND_DEFECT_PART_ALIGN:
		case WND_DEFECT_PAD_ADJUST:			
		case WND_DEFECT_LEAD_ADJUST:
		case WND_DEFECT_CLASS_CHECK:
			bSuit = true;
			break;
		case WND_DEFECT_BODY_MISSING:
		case WND_DEFECT_BODY_MOUNT:
		case WND_DEFECT_SOLDER_POOR:
		case WND_DEFECT_SOLDER_BEAD:
		case WND_DEFECT_FOREIGN_BODY:
			bSuit = true;
			break;
		default:
			bSuit = false;
			break;
		}
		break;
	case ALG_WIDTH_RATIO:
		switch (WndDefectID)
		{
		case WND_DEFECT_SOLDER_POOR:
			bSuit = true;
			break;
		default:
			bSuit = false;
			break;
		}
		break;
	case ALG_COLOR_CODE:
		switch ( WndDefectID )
		{
		case WND_DEFECT_CLASS_CHECK:
		case WND_DEFECT_BODY_MISSING:
		case WND_DEFECT_BODY_OFFSET:
		case WND_DEFECT_BODY_POLARITY:
		case WND_DEFECT_BODY_TURNOVER:
		case WND_DEFECT_BODY_MOUNT:
		case WND_DEFECT_SOLDER_POOR:
		case WND_DEFECT_SOLDER_OPEN:
		case WND_DEFECT_SOLDER_PAD_EXPOSED:
		case WND_DEFECT_SOLDER_EXCESS:
			bSuit = true;
			break;
		default:
			bSuit = false;
			break;
		}
		break;
	case ALG_MODEL_MATCH:
		switch ( WndDefectID )
		{
		case WND_DEFECT_PAD_ALIGN:
		case WND_DEFECT_PART_ALIGN:
		case WND_DEFECT_PAD_ADJUST:
		case WND_DEFECT_LEAD_ADJUST:
			bSuit = true;
			break;
		default:
			bSuit = false;
			break;
		}
		break;
	case ALG_IMAGE_MATCH:
		switch ( WndDefectID )
		{		
		case WND_DEFECT_PAD_ALIGN:
		case WND_DEFECT_PART_ALIGN:
		case WND_DEFECT_PAD_ADJUST:
		case WND_DEFECT_LEAD_ADJUST:		
		case WND_DEFECT_CLASS_CHECK:
		case WND_DEFECT_BODY_POLARITY:
		case WND_DEFECT_BODY_TURNOVER:
		case WND_DEFECT_BODY_MOUNT:
		case WND_DEFECT_BODY_WRONG_TEXT:		
		case WND_DEFECT_PAD_SCRATCH:
		case WND_DEFECT_FOREIGN_BODY:
			bSuit = true;
			break;
		default:
			bSuit = false;
			break;
		}
		break;
	case ALG_CHAR_VERIFY:
		switch ( WndDefectID )
		{
		case WND_DEFECT_CLASS_CHECK:
		case WND_DEFECT_BODY_POLARITY:
		case WND_DEFECT_BODY_WRONG_TEXT:
		case WND_DEFECT_PAD_SCRATCH:
		case WND_DEFECT_FOREIGN_BODY:
			bSuit = true;
			break;
		default:
			bSuit = false;
			break;
		}
		break;
	case ALG_FD_MATCH:
		switch ( WndDefectID )
		{
		case WND_DEFECT_PAD_ALIGN:
		case WND_DEFECT_PART_ALIGN:
		case WND_DEFECT_PAD_ADJUST:
		case WND_DEFECT_LEAD_ADJUST:		
			bSuit = true;
			break;
		default:
			bSuit = false;
			break;
		}
		break;
	case ALG_EDGE_SEARCH:
		switch ( WndDefectID )
		{
		case WND_DEFECT_PAD_ALIGN:
		case WND_DEFECT_PART_ALIGN:
		case WND_DEFECT_PAD_ADJUST:
		case WND_DEFECT_LEAD_ADJUST:		
			bSuit = true;
			break;
		default:
			bSuit = false;
			break;
		}
		break;
	case ALG_SHAPE_VERIFY://外形驗證
		switch ( WndDefectID )
		{
		case WND_DEFECT_PAD_ALIGN:
		case WND_DEFECT_PART_ALIGN:
		case WND_DEFECT_PAD_ADJUST:
		case WND_DEFECT_LEAD_ADJUST:
		//case WND_DEFECT_CLASS_CHECK;
		case WND_DEFECT_BASE_VALUE:
		case WND_DEFECT_BODY_WRONG_CODE:
		case WND_DEFECT_BODY_WRONG_TEXT:
			bSuit = false;
			break;
		}
		break;
	case ALG_ANGLE_MEASURE://角度量測
		switch ( WndDefectID )
		{
		case WND_DEFECT_PAD_ALIGN:
		case WND_DEFECT_PART_ALIGN:
		case WND_DEFECT_PAD_ADJUST:
		case WND_DEFECT_LEAD_ADJUST:
		//case WND_DEFECT_CLASS_CHECK;
		case WND_DEFECT_BASE_VALUE:
		case WND_DEFECT_BODY_WRONG_CODE:
		case WND_DEFECT_BODY_WRONG_TEXT:
			bSuit = false;
			break;
		}
		break;
	case ALG_PIXEL_COMPARE:
		switch ( WndDefectID )
		{
		case WND_DEFECT_PAD_SCRATCH:
		case WND_DEFECT_FOREIGN_BODY:
			bSuit = true;
			break;
		default:
			bSuit = false;
			break;
		}
		break;
	case ALG_HEIGHT:
		switch (WndDefectID)
		{
		case WND_DEFECT_PAD_ALIGN:
		case WND_DEFECT_PART_ALIGN:
		case WND_DEFECT_PAD_ADJUST:
		case WND_DEFECT_BASE_VALUE:
		case WND_DEFECT_BODY_OFFSET:
		case WND_DEFECT_BODY_POLARITY:
		case WND_DEFECT_BODY_TURNOVER:
		case WND_DEFECT_BODY_MOUNT:
		case WND_DEFECT_BODY_WRONG_CODE:
		case WND_DEFECT_BODY_WRONG_TEXT:
		case WND_DEFECT_SOLDER_BRIDGE:
		case WND_DEFECT_LEAD_LIFTED:
		case WND_DEFECT_LEAD_BENDED:
		case WND_DEFECT_LEAD_PROTRUDED:
		case WND_DEFECT_PAD_SCRATCH:
		case WND_DEFECT_FOREIGN_BODY:
			bSuit = false;
			break;
		}	
		break;
	case ALG_WIRE_WIDTH:
		switch (WndDefectID)
		{
		case WND_DEFECT_PAD_ALIGN:
		case WND_DEFECT_PART_ALIGN:
		case WND_DEFECT_PAD_ADJUST:
		case WND_DEFECT_BASE_VALUE:
		case WND_DEFECT_BODY_OFFSET:
		case WND_DEFECT_BODY_TILT:
		case WND_DEFECT_BODY_POLARITY:
		case WND_DEFECT_BODY_TURNOVER:
		case WND_DEFECT_BODY_MOUNT:
		case WND_DEFECT_BODY_WRONG_CODE:
		case WND_DEFECT_BODY_WRONG_TEXT:
		case WND_DEFECT_BODY_TOMBSTONE:
		case WND_DEFECT_BODY_BILLBOARD:
		case WND_DEFECT_BODY_DAMAGED:
		case WND_DEFECT_SOLDER_BRIDGE:
		case WND_DEFECT_LEAD_LIFTED:
		case WND_DEFECT_LEAD_BENDED:
		case WND_DEFECT_LEAD_PROTRUDED:
		case WND_DEFECT_PAD_SCRATCH:
		case WND_DEFECT_FOREIGN_BODY:
			bSuit = false;
			break;
		}
		break;
	case ALG_SOLDER_WETTING:
		switch ( WndDefectID )
		{
		case WND_DEFECT_SOLDER_POOR:
		case WND_DEFECT_SOLDER_OPEN:
		case WND_DEFECT_SOLDER_EXCESS:		
		case WND_DEFECT_USER_DEFINE_01:
		case WND_DEFECT_USER_DEFINE_02:
		case WND_DEFECT_USER_DEFINE_03:
		case WND_DEFECT_USER_DEFINE_04:
		case WND_DEFECT_USER_DEFINE_05:
		case WND_DEFECT_USER_DEFINE_06:
		case WND_DEFECT_USER_DEFINE_07:
		case WND_DEFECT_USER_DEFINE_08:
		case WND_DEFECT_USER_DEFINE_09:
		case WND_DEFECT_USER_DEFINE_10:
			bSuit = true;
			break;
		default:
			bSuit = false;
			break;
		}
		break;
	case ALG_MEASURE_BLACK_GLUE:
	case ALG_MEASURE_FLUX_AREA:
	case ALG_MEASURE_CPU_PIN:
		switch ( WndDefectID )
		{
		case WND_DEFECT_PAD_ALIGN:
		case WND_DEFECT_PART_ALIGN:
		case WND_DEFECT_PAD_ADJUST:
		case WND_DEFECT_LEAD_ADJUST:
		case WND_DEFECT_BODY_WRONG_CODE:
		case WND_DEFECT_BODY_WRONG_TEXT:
			bSuit = false;
			break;
		}		
		break;
	case ALG_MEASURE_SIP_DISTANCE:
		switch (WndDefectID)
		{
			case WND_DEFECT_USER_DEFINE_01:
			case WND_DEFECT_USER_DEFINE_02:
			case WND_DEFECT_USER_DEFINE_03:
			case WND_DEFECT_USER_DEFINE_04:
			case WND_DEFECT_USER_DEFINE_05:
			case WND_DEFECT_USER_DEFINE_06:
			case WND_DEFECT_USER_DEFINE_07:
			case WND_DEFECT_USER_DEFINE_08:
			case WND_DEFECT_USER_DEFINE_09:
			case WND_DEFECT_USER_DEFINE_10:
				bSuit = true;
			break;
			default:
				bSuit = false;
				break;
		}
		break;
	case ALG_MEASURE_CONNECTOR:
		switch (WndDefectID)
		{
		case WND_DEFECT_PART_ALIGN:
			bSuit = true;
			break;
		default:
			bSuit = false;
			break;
		}
		break;
	case ALG_MEASURE_CONNECTOR_PIN:
		switch (WndDefectID)
		{
		case WND_DEFECT_USER_DEFINE_01:
		case WND_DEFECT_USER_DEFINE_02:
		case WND_DEFECT_USER_DEFINE_03:
		case WND_DEFECT_USER_DEFINE_04:
		case WND_DEFECT_USER_DEFINE_05:
		case WND_DEFECT_USER_DEFINE_06:
		case WND_DEFECT_USER_DEFINE_07:
		case WND_DEFECT_USER_DEFINE_08:
		case WND_DEFECT_USER_DEFINE_09:
		case WND_DEFECT_USER_DEFINE_10:
			bSuit = true;
			break;
		default:
			bSuit = false;
			break;
		}
		break;
	}

	return bSuit;
}
//-------------------------------------------------------------------------------------//
CAOIWnd::CAOIWnd():CAOIObj(AOI_OBJ_WND)
{
	PreInitWnd();
	InitialWnd();
}
//-------------------------------------------------------------------------------------//
CAOIWnd::CAOIWnd(const CAOIWnd &Wnd):CAOIObj(Wnd)
{
	PreInitWnd();
	CloneWnd(Wnd);
}
//-------------------------------------------------------------------------------------//
CAOIWnd::~CAOIWnd()
{	
	ReleaseWndObj();
}
//-------------------------------------------------------------------------------------//
CAOIWnd& CAOIWnd::operator=(const CAOIWnd &Wnd)
{
	if ( &Wnd == this ) { return *this; }
	CAOIObj::operator=(Wnd);
	CloneWnd(Wnd);
	return *this;
}
//-------------------------------------------------------------------------------------//
inline void CAOIWnd::PreInitWnd()
{	
}
//-------------------------------------------------------------------------------------//
inline void CAOIWnd::InitialWnd()
{
	m_WndIndex = -1;
	m_WndBandID = 0;
	m_WndGroupID = -1;	
	m_WndClassID = 0;
	m_WndBypassed = false;
	m_WndIsolated = false;	
	m_WndModified = false;
	m_WndDefectID = WND_DEFECT_NONE;
	m_WndDefectAlarm = false;
	m_WndDefectAlarmEnableOnAOI = true;
	m_WndDefectAlarmEnableOnARS = false;
	m_WndDefectGroupID = 0;
	m_WndOrderIndex = -1;
	m_WndInspectedTime = 0.0;	
	m_WndConstrainMode=WND_CONSTRAIN_DISABLE;
	m_WndRectCalValue = TPOINT2D();	

	m_WndUIUpated_Param = false;

	m_WndModelPtr = NULL;
	m_WndLandPtr = NULL;
	m_WndLandIndex = -1;		

	m_WndLogicType = WND_LOGIC_NONE;
	m_WndLogicGroupID = 0;
	m_WndLogicResultID = RESULT_ID_NONE;

	m_WndBox = CAOIBox();

	m_WndExtendBox = CAOIBox();
	m_WndExtendRangeX = 0;
	m_WndExtendRangeY = 0;
	m_WndExtendBoxUsed = false;
	
	m_WndFollowMode = WND_FOLLOW_PAD;
	m_WndSyncMoveMode = WND_SYNC_MOVE_ROTATE;

	m_WndRgnLinkAuto = false;
	m_WndRgnLinkMode = WND_RGN_LINK_NONE;
	m_WndRgnLinkRatioX = 100;
	m_WndRgnLinkRatioY = 100;
	m_WndRgnLinkGoupID = -1;

	m_WndTempInt[0] = 0;
	m_WndTempInt[1] = 0;
	m_WndTempInt[2] = 0;
	m_WndTempInt[3] = 0;
	m_WndImageRect.left = 0;
	m_WndImageRect.top = 0;
	m_WndImageRect.right = 0;
	m_WndImageRect.bottom = 0;
	m_WndImageRect_Raw = m_WndImageRect;

	m_WndExtendImageRect.left = 0;
	m_WndExtendImageRect.top = 0;
	m_WndExtendImageRect.right = 0;
	m_WndExtendImageRect.bottom = 0;
	m_WndExtendImageRect_Raw = m_WndExtendImageRect;
	
	m_WndModelMaskFlag=MODEL_MASK_NONE;
	m_WndAlgParam.SetAlgWndPtr(this);	
	m_WndResultBoxList.clear();
}
//-------------------------------------------------------------------------------------//
inline void CAOIWnd::CloneWnd(const CAOIWnd &Wnd)
{
	m_WndIndex = Wnd.m_WndIndex;
	m_WndBandID = Wnd.m_WndBandID;
	m_WndGroupID = Wnd.m_WndGroupID;
	m_WndClassID = Wnd.m_WndClassID;	
	m_WndBypassed = Wnd.m_WndBypassed;
	m_WndIsolated = Wnd.m_WndIsolated;	
	m_WndModified = Wnd.m_WndModified;
	m_WndDefectID = Wnd.m_WndDefectID;
	m_WndDefectAlarm = Wnd.m_WndDefectAlarm;	
	m_WndDefectAlarmEnableOnAOI = Wnd.m_WndDefectAlarmEnableOnAOI;
	m_WndDefectAlarmEnableOnARS = Wnd.m_WndDefectAlarmEnableOnARS;
	m_WndDefectGroupID = Wnd.m_WndDefectGroupID;	
	m_WndOrderIndex = Wnd.m_WndOrderIndex;
	m_WndInspectedTime = Wnd.m_WndInspectedTime;	
	m_WndConstrainMode = Wnd.m_WndConstrainMode;
	m_WndRectCalValue = Wnd.m_WndRectCalValue;	

	m_WndUIUpated_Param = Wnd.m_WndUIUpated_Param;
	m_WndModelPtr = Wnd.m_WndModelPtr;
	m_WndLandPtr = Wnd.m_WndLandPtr;
	m_WndLandIndex = Wnd.m_WndLandIndex;

	m_WndLogicType = Wnd.m_WndLogicType;
	m_WndLogicGroupID = Wnd.m_WndLogicGroupID;
	m_WndLogicResultID = Wnd.m_WndLogicResultID;

	m_WndBox = Wnd.m_WndBox;

	m_WndExtendBox = Wnd.m_WndExtendBox;
	m_WndExtendBoxUsed = Wnd.m_WndExtendBoxUsed;
	m_WndExtendRangeX = Wnd.m_WndExtendRangeX;
	m_WndExtendRangeY = Wnd.m_WndExtendRangeY;

	m_WndFollowMode = Wnd.m_WndFollowMode;
	m_WndSyncMoveMode = Wnd.m_WndSyncMoveMode;	

	m_WndRgnLinkAuto = Wnd.m_WndRgnLinkAuto;
	m_WndRgnLinkMode = Wnd.m_WndRgnLinkMode;
	m_WndRgnLinkRatioX = Wnd.m_WndRgnLinkRatioX;
	m_WndRgnLinkRatioY = Wnd.m_WndRgnLinkRatioY;
	m_WndRgnLinkGoupID = Wnd.m_WndRgnLinkGoupID;

	m_WndTempInt[0] = Wnd.m_WndTempInt[0];
	m_WndTempInt[1] = Wnd.m_WndTempInt[1];
	m_WndTempInt[2] = Wnd.m_WndTempInt[2];
	m_WndTempInt[3] = Wnd.m_WndTempInt[3];

	m_WndImageRect = Wnd.m_WndImageRect;	
	m_WndImageRect_Raw = Wnd.m_WndImageRect_Raw;
	m_WndExtendImageRect = Wnd.m_WndExtendImageRect;
	m_WndExtendImageRect_Raw = Wnd.m_WndExtendImageRect_Raw;

	m_WndModelMaskFlag = Wnd.m_WndModelMaskFlag;

	m_WndAlgParam = Wnd.m_WndAlgParam;	
	m_WndAlgParam.SetAlgWndPtr(this);	
	m_WndResultBoxList = Wnd.m_WndResultBoxList;

	CloneWndRoiWndList(Wnd);
	CloneWndMaskBoxList(Wnd);
}
//-------------------------------------------------------------------------------------//
void CAOIWnd::CloneWndRoiWndList(const CAOIWnd &Wnd)
{
	size_t       i=0;
	CAOIWndRoi  *WndRoiPtr = NULL;
	const size_t RoiCount = Wnd.GetWndRoiWndCount_Inline();
	
	ClearWndRoiWndList();
	for ( i=0; i<RoiCount; i++ )
	{
		WndRoiPtr = Wnd.GetWndRoiWndPtr_Inline(i);
		if ( NULL == WndRoiPtr ) { continue; }
		WndRoiPtr = WndRoiPtr->CloneWndRoiObj();
		if ( NULL == WndRoiPtr ) { continue; }
		AddWndRoiWndPtr(WndRoiPtr, false);
	}
}
//-------------------------------------------------------------------------------------//
void CAOIWnd::CloneWndMaskBoxList(const CAOIWnd &Wnd)
{
	size_t       i=0;
	CAOIWndMask *MaskWndPtr = NULL;
	const size_t MaskWndCount = Wnd.GetWndMaskWndCount_Inline();
	
	ClearWndMaskWndList();
	for ( i=0; i<MaskWndCount; i++ )
	{
		MaskWndPtr = Wnd.GetWndMaskWndPtr_Inline(i);
		if ( NULL == MaskWndPtr ) { continue; }
		MaskWndPtr = MaskWndPtr->CloneWndMaskObj();
		if ( NULL == MaskWndPtr ) { continue; }
		MaskWndPtr->SetWndMaskToward(GetWndToward());
		AddWndMaskWndPtr(MaskWndPtr, false);
	}
}
//-------------------------------------------------------------------------------------//
void CAOIWnd::ResetWndObj()//復歸Wnd物件
{	
	InitialWnd();
}
//-------------------------------------------------------------------------------------//
void CAOIWnd::ReleaseWndObj()//釋放Wnd物件
{
	ClearWndRoiWndList();
	ClearWndMaskWndList();
	m_WndAlgParam = CAlgParam();
	ResetWndObj();
}
//-------------------------------------------------------------------------------------//
CAOIWnd* CAOIWnd::CloneWndObj() const//建立且複製一個檢測框
{
	CAOIWnd *ObjPtr = AOIObjManager.CreateWndObj();
	if ( NULL == ObjPtr ) { return NULL; }
	ObjPtr->operator=(*this);
	return ObjPtr;
}
//-------------------------------------------------------------------------------------//
bool CAOIWnd::GetSaveWndLog() const//取得是否儲存檢測框訊息
{
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIWnd::SaveWndLog(LPCTSTR str)
{
	if ( GetSaveWndLog() == false ) { return true; }
	AOIDataCollect.SaveDebugMessage_RGN(str);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIWnd::SaveWndLog(LPCTSTR strAct, CAOIModel *ModelPtr, LPCTSTR str)//儲存檢測框訊息
{
	if ( GetSaveWndLog() == false ) { return true; }	
	if ( NULL == ModelPtr ) { return false;	}

	CString strName;
	const unsigned int nIndex = GetWndIndex();	
	AOI_OBJ_TYPE AttachedType=ModelPtr->GetModelAttachedType();
	if ( AOI_OBJ_FD == AttachedType )
	{
		CAOIFd *FdPtr = ModelPtr->GetModelFdPtr();
		if ( NULL != FdPtr )
		{	strName = FdPtr->GetFdFullName();	}
	}
	else if ( AOI_OBJ_MARK == AttachedType )
	{
		CAOIMark *MarkPtr = ModelPtr->GetModelMarkPtr();
		if ( NULL != MarkPtr )
		{	strName = MarkPtr->GetMarkFullName();	}
	}
	else if ( AOI_OBJ_BARCODE == AttachedType )
	{
		CAOIBarcode *BarcodePtr = ModelPtr->GetModelBarcodePtr();
		if ( NULL != BarcodePtr )
		{	strName = BarcodePtr->GetBarcodeFullName();	}
	}
	else if ( AOI_OBJ_COMPONENT == AttachedType )
	{
		CAOIComponent *ComponentPtr = ModelPtr->GetModelComponentPtr();
		if ( NULL != ComponentPtr )
		{	strName = ComponentPtr->GetComponentFullName();	}
	}	

	CString Str2;
	CString FullName;
	CString strAlgName=AOIDataDefine.GetAlgTypeText(GetWndAlgType());	
	if ( 0 == strName.GetLength() )
	{	FullName.Format(_T("[Wnd:%d][%s]"), nIndex+1, strAlgName);	}
	else
	{	FullName.Format(_T("[%s][Wnd:%d][%s]"), strName, nIndex+1, strAlgName);	}
	if ( NULL == strAct )
	{	Str2.Format(_T("%s %s"), FullName, str);	}
	else
	{	Str2.Format(_T("%s %s %s"), strAct, FullName, str);	}
	if ( SaveWndLog(Str2) == false )
	{	return false; }	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIWnd::WriteWndFile(CAOIFileIO &FileIO)//儲存檢測框檔案
{
#ifdef _DEBUG
	if ( FileIO.CheckFileMode() == false ) { return false; }
	if ( FileIO.CheckFileOpened() == false ) { return false; }
#endif//_DEBUG
	
	size_t       i=0;
	int          index = 0;
	int          nValue = 0;
	double       dValue = 0.0;
	CAOIWnd     *WndPtr = this;
	CAOIBox     *BoxPtr = NULL;	
	CAOIWndRoi  *WndRoiPtr = NULL;	
	CAOIWndMask *MaskWndPtr = NULL;
	char         uuidStr[MAX_JET_PATH]="";	
	wchar_t      uuidWStr[MAX_JET_PATH]=L"";	
	UUID         uuid = WndPtr->GetObjUuid();	
	FileIO.SetFnName(_T("CAOIWnd::WriteWndFile"));
	JetAPI::UUIDToStringA(uuid, uuidStr);
	JetAPI::UUIDToStringW(uuid, uuidWStr);
	//----------------------------------------------------------------------------------------//
	if ( FileIO.SaveChunk_INT(FILE_IO_WND_START, 0) == false ) { return false;; }
	
	if ( FileIO.SaveChunk_INT(FILE_IO_WND_GROUP_ID, WndPtr->GetWndGroupID()) == false ) { return false;; }
	if ( FileIO.SaveChunk_INT(FILE_IO_WND_BAND_ID, WndPtr->GetWndBandID()) == false ) { return false;; }	
	if ( FileIO.SaveChunk_BOL(FILE_IO_WND_ISOLATED, WndPtr->GetWndIsolated()) == false ) { return false;; }
	//if ( FileIO.SaveChunk_INT(FILE_IO_WND_DEFECT_ID, WndPtr->GetWndDefectID()) == false ) { return false;; }
	if ( FileIO.SaveChunk_INT(FILE_IO_WND_LAND_INDEX, WndPtr->GetWndLandIndex()) == false ) { return false;; }
	if ( FileIO.SaveChunk_INT(FILE_IO_WND_FOLLOW_MODE, WndPtr->GetWndFollowMode()) == false ) { return false;; }	
	if ( FileIO.SaveChunk_BOL(FILE_IO_WND_ENABLED, WndPtr->GetWndEnabled()) == false ) { return false;; }	
	
	//模組框參數-範圍
	if ( FileIO.SaveChunk_BOL(FILE_IO_WND_RGN_LINK_AUTO, WndPtr->GetWndRgnLinkAuto()) == false ) { return false;; }
	if ( FileIO.SaveChunk_INT(FILE_IO_WND_RGN_LINK_MODE, WndPtr->GetWndRgnLinkMode()) == false ) { return false;; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_WND_RGN_LINK_RATIO_X, WndPtr->GetWndRgnLinkRatioX()) == false ) { return false;; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_WND_RGN_LINK_RATIO_Y, WndPtr->GetWndRgnLinkRatioY()) == false ) { return false;; }
	if ( FileIO.SaveChunk_INT(FILE_IO_WND_RGN_LINK_GROUP_ID, WndPtr->GetWndRgnLinkGoupID()) == false ) { return false;; }

	//模組框參數-外擴範圍
	if ( FileIO.SaveChunk_DBL(FILE_IO_WND_EXTEND_RANGE_X, WndPtr->GetWndExtendRangeX()) == false ) { return false;; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_WND_EXTEND_RANGE_Y, WndPtr->GetWndExtendRangeY()) == false ) { return false;; }
	if ( FileIO.SaveChunk_BOL(FILE_IO_WND_EXTEND_ENABLED, WndPtr->GetWndExtendBoxUsed()) == false ) { return false;; }

	//模組框參數-邏輯閘參數
	if ( FileIO.SaveChunk_INT(FILE_IO_WND_LOGIC_TYPE, WndPtr->GetWndLogicType()) == false ) { return false;; }	
	if ( FileIO.SaveChunk_INT(FILE_IO_WND_LOGIC_GROUP_ID, WndPtr->GetWndLogicGroupID()) == false ) { return false;; }	

	if ( FileIO.GetSaveWStr() == true ) 
	{	if ( FileIO.SaveChunk_STR(FILE_IO_WND_OBJ_UUID, uuidWStr) == false ) { return false; } }
	else
	{	if ( FileIO.SaveChunk_STR(FILE_IO_WND_OBJ_UUID, uuidStr) == false ) { return false; } }

	if ( FileIO.SaveChunk_INT(FILE_IO_WND_CONSTRAIN_MODE, WndPtr->GetWndConstrainMode()) == false ) { return false;; }	
	if ( FileIO.SaveChunk_INT(FILE_IO_WND_MODEL_MASK, WndPtr->GetWndModelMaskFlag()) == false ) { return false;; }		
	if ( FileIO.SaveChunk_INT(FILE_IO_WND_DEFECT_ID, WndPtr->GetWndDefectID()) == false ) { return false;; }
	if ( FileIO.SaveChunk_INT(FILE_IO_WND_SYNC_MOVE_MODE, WndPtr->GetWndSyncMoveMode()) == false ) { return false;; }	
	if ( FileIO.SaveChunk_INT(FILE_IO_WND_DEFECT_GROUP_ID, WndPtr->GetWndDefectGroupID()) == false ) { return false;; }	
	if ( FileIO.SaveChunk_INT(FILE_IO_WND_CLASS_ID, WndPtr->GetWndClassID()) == false ) { return false;; }	

	//模組框參數-基本框參數
	BoxPtr = WndPtr->GetWndBoxPtr();
	if ( NULL != BoxPtr )
	{		
		if ( FileIO.SaveChunk_INT(FILE_IO_WND_BOX_NODE, 0) == false ) { return false;; }
		if ( BoxPtr->WriteBoxFile(FileIO) == false ) { return false; }		
	}

	//檢測框-子檢測框列表
	const size_t WndRoiCount = GetWndRoiWndCount_Inline();	
	for ( i=0; i<WndRoiCount; i++ )
	{
		WndRoiPtr = GetWndRoiWndPtr_Inline(i);
		if ( NULL == WndRoiPtr ) { continue; }
		if ( FileIO.SaveChunk_INT(FILE_IO_WND_ROI_NODE, 0) == false ) { return false;; }	
		if ( WndRoiPtr->WriteWndRoiFile(FileIO) == false ) { return false; }
	}

	//模組框參數-遮罩框參數
	const size_t MaskWndCount = GetWndMaskWndCount_Inline();	
	for ( i=0; i<MaskWndCount; i++ )
	{
		MaskWndPtr = GetWndMaskWndPtr_Inline(i);
		if ( NULL == MaskWndPtr ) { continue; }
		if ( FileIO.SaveChunk_INT(FILE_IO_WND_MASK_NODE_WND, 0) == false ) { return false;; }	
		if ( MaskWndPtr->WriteWndMaskFile(FileIO) == false ) { return false; }
	}
	
	//模組框參數-演算法參數
	CAlgParam  &AlgParam = WndPtr->GetWndAlgParam();	
	if ( FileIO.SaveChunk_INT(FILE_IO_WND_ALG_PARAM, 0) == false ) { return false;; }
	if ( AlgParam.WriteAlgParamFile(FileIO) == false ) { return false; }			
	
	if ( FileIO.SaveChunk_INT(FILE_IO_WND_END, 0) == false ) { return false;; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIWnd::ReadWndFile(CAOIFileIO &FileIO)//載入檢測框檔案
{
#ifdef _DEBUG
	if ( FileIO.CheckFileMode() == false ) { return false; }
	if ( FileIO.CheckFileOpened() == false ) { return false; }
#endif//_DEBUG
	
	UUID      uuid;
	int          index = 0;
	int          nValue = 0;
	double       dValue = 0.0;
	CAOIWnd     *WndPtr = this;
	CAOIBox     *BoxPtr = NULL;	
	CAlgParam   *AlgParamPtr = NULL;
	CAOIWndRoi  *WndRoiPtr = NULL;
	CAOIWndMask *MaskWndPtr = NULL;	
	FileIO.SetFnName(_T("CAOIWnd::ReadWndFile"));
	//----------------------------------------------------------------------------------------//
	while ( FileIO.CheckFileEnd()==false )
	{ 		
		if ( FileIO.LoadChunk(index) == false )		
		{	continue; }

		switch ( index )
		{
		case FILE_IO_WND_START://模組框參數-起點
			break;
		case FILE_IO_WND_END://模組框參數-終點
			WndPtr->UpdateWndExtendBox();			
			WndPtr->SetWndModified(false);
			return true;
			break;
		case FILE_IO_WND_GROUP_ID://模組框參數-群組編號
			WndPtr->SetWndGroupID(FileIO.GetData_INT());
			break;
		case FILE_IO_WND_BAND_ID://模組框參數-次群組編號
			WndPtr->SetWndBandID(FileIO.GetData_INT());
			break;		
		case FILE_IO_WND_ISOLATED://模組框參數-是否隔離			
			WndPtr->SetWndIsolated(FileIO.GetData_BOL());
			break;
		case FILE_IO_WND_DEFECT_ID_OLD://模組框參數-瑕疵代碼-舊的代碼
			WndPtr->SetWndDefectID(ConvertWndDefectID(FileIO.GetData_INT()));
			break;
		case FILE_IO_WND_LAND_INDEX://模組框參數-特徵框引數
			WndPtr->SetWndLandIndex(FileIO.GetData_INT());
			break;
		case FILE_IO_WND_FOLLOW_MODE://模組框參數-跟隨移動模式
			WndPtr->SetWndFollowMode((WND_FOLLOW_MODE)(FileIO.GetData_INT()));
			break;
		case FILE_IO_WND_ENABLED://模組框參數-啟動
			WndPtr->SetWndEnabled(FileIO.GetData_BOL());
			break;
		case FILE_IO_WND_RGN_LINK_AUTO://模組框參數-範圍自動連動			
			WndPtr->SetWndRgnLinkAuto(FileIO.GetData_BOL());
			break;
		case FILE_IO_WND_RGN_LINK_MODE://模組框參數-範圍自動模式
			WndPtr->SetWndRgnLinkMode((WND_RGN_LINK_MODE)(FileIO.GetData_INT()));
			break;
		case FILE_IO_WND_RGN_LINK_RATIO_X://模組框參數-範圍U方向比例-%
			WndPtr->SetWndRgnLinkRatioX(FileIO.GetData_DBL());
			break;
		case FILE_IO_WND_RGN_LINK_RATIO_Y://模組框參數-範圍V方向比例-%
			WndPtr->SetWndRgnLinkRatioY(FileIO.GetData_DBL());
			break;
		case FILE_IO_WND_RGN_LINK_GROUP_ID://模組框參數-綁定的特徵框群組編號
			WndPtr->SetWndRgnLinkGoupID(FileIO.GetData_INT());
			break;
		case FILE_IO_WND_EXTEND_RANGE_X://模組框參數-外擴範圍-X-um
			WndPtr->SetWndExtendRangeX(FileIO.GetData_DBL());
			break;
		case FILE_IO_WND_EXTEND_RANGE_Y://模組框參數-外擴範圍-Y-um
			WndPtr->SetWndExtendRangeY(FileIO.GetData_DBL());
			break;
		case FILE_IO_WND_EXTEND_ENABLED://模組框參數-外擴範圍-是否使用			
			WndPtr->SetWndExtendBoxUsed(FileIO.GetData_BOL());
			break;
		case FILE_IO_WND_LOGIC_TYPE://模組框參數-邏輯樣式
			WndPtr->SetWndLogicType((WND_LOGIC_TYPE)(FileIO.GetData_INT()));
			break;
		case FILE_IO_WND_LOGIC_GROUP_ID://模組框參數-邏輯群組編號
			WndPtr->SetWndLogicGroupID(FileIO.GetData_INT());
			break;
		case FILE_IO_WND_OBJ_UUID://模組框參數-OBJ-UUID
			if ( FileIO.GetLoadWStr()==true )
			{
				if ( JetAPI::UUIDFromStringW(FileIO.GetData_WSTR(), uuid) == true )
				{	WndPtr->SetObjUuid(uuid);	}
			}
			else
			{
				if ( JetAPI::UUIDFromStringA(FileIO.GetData_STR(), uuid) == true )
				{	WndPtr->SetObjUuid(uuid);	}
			}
			break;
		case FILE_IO_WND_CONSTRAIN_MODE://模組框參數-侷限模式
			WndPtr->SetWndConstrainMode((WND_CONSTRAIN_MODE)(FileIO.GetData_INT()));
			break;
		case FILE_IO_WND_MODEL_MASK://模組框參數-模組遮罩
			WndPtr->SetWndModelMaskFlag(FileIO.GetData_INT());
			break;
		case FILE_IO_WND_DEFECT_ID:
			WndPtr->SetWndDefectID((WND_DEFECT_ID)(FileIO.GetData_INT()));
			break;
		case FILE_IO_WND_SYNC_MOVE_MODE://模組框參數-不同特徵腳的連動模式
			WndPtr->SetWndSyncMoveMode((WND_SYNC_MOVE_MODE)(FileIO.GetData_INT()));
			break;
		case FILE_IO_WND_DEFECT_GROUP_ID://模組框參數-瑕疵群組編號
			WndPtr->SetWndDefectGroupID(FileIO.GetData_INT());
			break;
		case FILE_IO_WND_CLASS_ID://模組框參數-類別編號
			WndPtr->SetWndClassID(FileIO.GetData_INT());
			break;		
		case FILE_IO_WND_BOX_NODE://模組框參數-基本框參數
			BoxPtr = WndPtr->GetWndBoxPtr();
			if ( NULL != BoxPtr )
			{
				if ( BoxPtr->ReadBoxFile(FileIO) == false )				
				{	return false; }
			}
			break;
		case FILE_IO_WND_ROI_NODE://模組框參數-子框參數		
			WndRoiPtr = AOIObjManager.CreateWndRoiObj();
			if ( NULL == WndRoiPtr ) 
			{
				FileIO.SetErrorString(_T("Error, Create Wnd Roi Ptr Fault"));
				return false;
			}			
			if ( WndRoiPtr->ReadWndRoiFile(FileIO) == false )
			{
				AOIObjManager.DestroyWndRoiObj(WndRoiPtr);
				return false; 
			}
			WndPtr->AddWndRoiWndPtr(WndRoiPtr, false);
			break;	
		case FILE_IO_WND_MASK_NODE_BOX://模組框參數-遮罩框參數
			MaskWndPtr = AOIObjManager.CreateWndMaskObj();
			if ( NULL == MaskWndPtr ) 
			{
				FileIO.SetErrorString(_T("Error, Create Wnd Mask Wnd Ptr Fault"));
				return false;
			}
			BoxPtr = MaskWndPtr->GetWndMaskBoxPtr();
			if ( NULL != BoxPtr )
			{
				if ( BoxPtr->ReadBoxFile(FileIO) == false )
				{
					AOIObjManager.DestroyWndMaskObj(MaskWndPtr);
					return false; 
				}
			}
			WndPtr->AddWndMaskWndPtr(MaskWndPtr, false);
			break;
		case FILE_IO_WND_MASK_NODE_WND://模組框參數-遮罩框參數
			MaskWndPtr = AOIObjManager.CreateWndMaskObj();
			if ( NULL == MaskWndPtr ) 
			{
				FileIO.SetErrorString(_T("Error, Create Wnd Mask Wnd Ptr Fault"));
				return false;
			}
			if ( MaskWndPtr->ReadWndMaskFile(FileIO) == false )
			{
				AOIObjManager.DestroyWndMaskObj(MaskWndPtr);
				return false; 
			}
			WndPtr->AddWndMaskWndPtr(MaskWndPtr, false);
			break;
		case FILE_IO_WND_ALG_PARAM://模組框參數-演算法參數
			AlgParamPtr = WndPtr->GetWndAlgParamPtr();
			if ( NULL != AlgParamPtr )
			{
				if ( AlgParamPtr->ReadAlgParamFile(FileIO) == false )
				{	return false; }				
			}
			break;
		default:
		#ifdef _DEBUG
			index = index;
		#endif//_DEBUG
			break;
		}
	};	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIWnd::BuildWndParamStringList(LPCTSTR Title, std::vector<CString> &strList) const//建立檢測框參數列表
{	
	CString strTitle;	
	const CAlgParam &AlgParam=GetWndAlgParam();
	const ALG_TYPE AlgType=AlgParam.GetAlgType();
	const WND_DEFECT_ID WndDefectID=GetWndDefectID();	
	CString strDefect=AOIDataDefine.GetWndDefectIDText(WndDefectID);
	strTitle.Format(_T("%s, %s"), Title, strDefect);
	if ( AlgParam.BuildAlgParamStringList(strTitle, AlgType, strList) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIWnd::WriteWndSpcFile_JSON_VRS(FILE *pfile, CAOIProject *ProjectPtr)
{
	if ( NULL == pfile ) { return false; }
	if ( NULL == ProjectPtr ) { return false; }

	CAOIWnd *WndPtr = this;	
	if ( NULL == WndPtr ) { return false; }	
	CAOIModel *ModelPtr = WndPtr->GetWndModelPtr();
	if ( NULL == ModelPtr ) { return false; }	

	size_t           v=0;	
	unsigned int     LandIndex=0;
	RECT             WndRect;		
	TREGION4D        WndRgn;
	TREGION4D        ModelRgn;
	TREGION4D        ImageRgn;
	TPOINT2D         ModelRgnCp;
	CString          strText;	
	CAOIBox         *BoxPtr = NULL;
	CAOILand        *LandPtr = NULL;
	TPOINT2D         ImgCornerPos[4];
	TPOINT2D         CadCornerPos[4];
	TPOINT2D         BoxCornerPos[4];	
	TPOINT2D         ImageCornerPos[4];	
	IMAGE_SIZE       ModelImageW=0;
	IMAGE_SIZE       ModelImageH=0;
	std::vector<int> ResBoxIdxList;

	const size_t     szBuffer = 256;
	wchar_t          strTag[szBuffer] = L"";
	wchar_t          strBuffer[szBuffer] = L"";	
	
	LANE_ID    LaneID = ProjectPtr->GetProjectActLaneID();	
	DISTRICT_ID DistrictID = ProjectPtr->GetProjectActDistrictID();	
	const double     ImageResX = AOIDataCollect.GetCameraResolutionX(PRIMARY_CAMERA_ID);
	const double     ImageResY = AOIDataCollect.GetCameraResolutionY(PRIMARY_CAMERA_ID);
	const TPOINT2D   ImageRes(ImageResX, ImageResY);

	ModelPtr->GetModelTotalRegion(ModelRgn);					
	ModelRgnCp.x = ModelRgn.GetCpX();
	ModelRgnCp.y = ModelRgn.GetCpY();
	ModelImageW = JetAPI::Floor(ModelRgn.GetWidth()/ImageRes.x);
	ModelImageH = JetAPI::Floor(ModelRgn.GetHeight()/ImageRes.y);	

	//WndPtr->GetWndExtendImageRect(WndRect);
	if ( WndPtr->GetWndExtendBoxUsed() == false )
	{	WndPtr->GetWndRegionRes(WndRgn); }
	else
	{	WndPtr->GetWndExtendBox().GetBoxRegionRes(WndRgn);	}
	AOIDataCollect.MapCadRegionToCamera(ModelImageW, ModelImageH, ImageRes, WndRgn, ModelRgnCp, ImageRgn);
	JetAPI::Region4DToRect(ImageRgn, WndRect, true);
	//::wcscpy(strTag, L"Loc");//Loc" : [100,100,300,200],	// 在零件圖上的位置(x1 y1 x2 y2)(pixel)
	//::fwprintf(pfile, L"          \"%s\": [%d,%d,%d,%d],\n", strTag, WndRect.left, WndRect.top, WndRect.right, WndRect.bottom);	

	if ( WndPtr->GetWndExtendBoxUsed() == false )
	{	WndPtr->GetWndCornerPosRes(CadCornerPos); }
	else
	{	WndPtr->GetWndExtendBox().GetBoxCornerPosRes(CadCornerPos); }
	AOIDataCollect.MapCadCornerToCamera(ModelImageW, ModelImageH, ImageRes, CadCornerPos, ModelRgnCp, ImageCornerPos);						
	::wcscpy(strTag, L"Loc");//"Location" : [100,100,300,200,100,100,300,200],	// 在零件圖上的位置(x1 y1 x2 y2)(pixel)
	::fwprintf(pfile, L"          \"%s\": [%.0f,%.0f,%.0f,%.0f,%.0f,%.0f,%.0f,%.0f],\n", strTag, 
		ImageCornerPos[0].x, ImageCornerPos[0].y, ImageCornerPos[1].x, ImageCornerPos[1].y, 
		ImageCornerPos[2].x, ImageCornerPos[2].y, ImageCornerPos[3].x, ImageCornerPos[3].y);

	::wcscpy(strTag, L"Frame Index");//"Frame Index":1~8	// 影像編號(整數)
	::fwprintf(pfile, L"          \"%s\": %d,\n", strTag, WndPtr->GetWndAlgParam().GetAlgImageBinParam().GetBinaryFrameIndex()+1);

	LandPtr = WndPtr->GetWndLandPtr();
	if ( NULL == LandPtr ) { LandIndex = 0; }
	else { LandIndex = LandPtr->GetLandIndex()+1; }
	::wcscpy(strTag, L"Lead ID");//"Lead ID":0, 1	// 腳位標號(整數)
	::fwprintf(pfile, L"          \"%s\": %d,\n", strTag, LandIndex);

	::wcscpy(strTag, L"Test_Defect");//"Defect" : 34		// 瑕疵ID(整數-2 Byte)
	::fwprintf(pfile, L"          \"%s\": %d,\n", strTag, WndPtr->GetWndDefectID());

	::wcscpy(strTag, L"Check_Defect");//"Defect" : 34		// 瑕疵ID(整數-2 Byte)
	::fwprintf(pfile, L"          \"%s\": %d,\n", strTag, WndPtr->GetWndDefectID());

	// 系統判定(整數-1 Byte) 0:No Test  1:Skip 2:Bypass 3:Sys-Ok 4:Sys-Ng 5:Op-Ok 6:Op-Ng 7:Fd-Ng 
	RESULT_ID WndResultID = WndPtr->GetWndResultID();
	SPC_RESULT_ID SpcResultID = AOIDataCollect.MapResultIDToSpcResultID(WndResultID);
	::wcscpy(strTag, L"Test_Result");			
	::fwprintf(pfile, L"          \"%s\": %d,\n", strTag, SpcResultID);						
					
	::wcscpy(strTag, L"Text");//"Text" : "offset x 10 um", // 額外文字(字串-64 Byte)
	strText = WndPtr->GetWndResultText();
	JetAPI::TCHAR2wchar(strText, strBuffer, szBuffer);
	::fwprintf(pfile, L"          \"%s\": \"%s\",\n", strTag, strBuffer);
	
	int       ResBoxIdx=0;
	RESULT_ID BoxResultID;
	ResBoxIdxList.clear();
	size_t WndResBoxCount = WndPtr->GetWndResultBoxCount();
	for ( v=0; v<WndResBoxCount; v++ )
	{
		BoxPtr = WndPtr->GetWndResultBoxPtr(v, false);
		if ( NULL == BoxPtr )  { continue; }
		BoxResultID = BoxPtr->GetBoxResultID();
		if ( RESULT_ID_NONE == BoxResultID ) { continue; }
		if ( RESULT_ID_OK == BoxResultID ) { continue; }
		if ( RESULT_ID_SKIP == BoxResultID ) { continue; }
		if ( RESULT_ID_BYPASS == BoxResultID ) { continue; }							
		ResBoxIdxList.push_back(v);
	}
	int ResBoxIdxCount = (int)(ResBoxIdxList.size());

	::wcscpy(strTag, L"N_Boxs");//"N Boxs" : 2,		// Boxs數量
	::fwprintf(pfile, L"          \"%s\": %d,\n", strTag, ResBoxIdxCount);
	for ( v=0; v<ResBoxIdxCount; v++ )
	{
		ResBoxIdx = ResBoxIdxList[v];
		BoxPtr = WndPtr->GetWndResultBoxPtr(ResBoxIdx, false);
		if ( NULL == BoxPtr )  { continue; }							

		//::fwprintf(pfile, L"        \"Box %d\":\n", v+1);
		//::fwprintf(pfile, L"        {\n");

		BoxPtr->GetBoxCornerPosRes(BoxCornerPos);
		AOIDataCollect.MapCadCornerToCamera(ModelImageW, ModelImageH, ImageRes, BoxCornerPos, ModelRgnCp, ImgCornerPos);							
		::swprintf(strTag, L"Box_%d", v+1);//"Box 1" : [100,100,300,200,100,100,300,200],	// 在零件圖上的位置(LTx LTy RTx RTy LBx LBy RBx RBy)(pixel)							
		::fwprintf(pfile, L"          \"%s\": [%.0f,%.0f,%.0f,%.0f,%.0f,%.0f,%.0f,%.0f],\n", strTag, ImgCornerPos[0].x, ImgCornerPos[0].y, ImgCornerPos[1].x, ImgCornerPos[1].y, ImgCornerPos[2].x, ImgCornerPos[2].y, ImgCornerPos[3].x, ImgCornerPos[3].y);

		//::fwprintf(pfile, L"        },\n");
	}
	::wcscpy(strTag, L"Check_Result");//Wnd Check Result
	::fwprintf(pfile, L"          \"%s\": %d\n", strTag, 0);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIWnd::WriteWndSpcFile_JSON_RSM(FILE *pfile, CAOIProject *ProjectPtr)
{
	if ( NULL == pfile ) { return false; }
	if ( NULL == ProjectPtr ) { return false; }

	CAOIWnd *WndPtr = this;	
	if ( NULL == WndPtr ) { return false; }	
	CAOIModel *ModelPtr = WndPtr->GetWndModelPtr();
	if ( NULL == ModelPtr ) { return false; }	

	size_t           v=0;	
	unsigned int     LandIndex=0;
	RECT             WndRect;		
	TREGION4D        WndRgn;
	TREGION4D        ModelRgn;
	TREGION4D        ImageRgn;
	TPOINT2D         ModelRgnCp;
	CString          strText;	
	CAOIBox         *BoxPtr = NULL;
	CAOILand        *LandPtr = NULL;
	TPOINT2D         ImgCornerPos[4];
	TPOINT2D         CadCornerPos[4];
	TPOINT2D         BoxCornerPos[4];	
	TPOINT2D         ImageCornerPos[4];	
	IMAGE_SIZE       ModelImageW=0;
	IMAGE_SIZE       ModelImageH=0;
	std::vector<int> ResBoxIdxList;

	const size_t     szBuffer = 256;
	wchar_t          strTag[szBuffer] = L"";
	wchar_t          strBuffer[szBuffer] = L"";		
	const bool       bNoSaveExtendRgn=true;//不要輸出外擴範圍

	WND_DEFECT_ID WndDefectID = WndPtr->GetWndDefectID();
	LANE_ID    LaneID = ProjectPtr->GetProjectActLaneID();	
	DISTRICT_ID DistrictID = ProjectPtr->GetProjectActDistrictID();		
	const double     ImageResX = AOIDataCollect.GetCameraResolutionX(PRIMARY_CAMERA_ID);
	const double     ImageResY = AOIDataCollect.GetCameraResolutionY(PRIMARY_CAMERA_ID);
	const TPOINT2D   ImageRes(ImageResX, ImageResY);

	ModelPtr->GetModelTotalRegion(ModelRgn);					
	ModelRgnCp.x = ModelRgn.GetCpX();
	ModelRgnCp.y = ModelRgn.GetCpY();
	ModelImageW = JetAPI::Floor(ModelRgn.GetWidth()/ImageRes.x);
	ModelImageH = JetAPI::Floor(ModelRgn.GetHeight()/ImageRes.y);		

	//WndPtr->GetWndExtendImageRect(WndRect);	
	if ( WndPtr->GetWndExtendBoxUsed()==false || true==bNoSaveExtendRgn )
	{	WndPtr->GetWndRegionRes(WndRgn); }
	else
	{	WndPtr->GetWndExtendBox().GetBoxRegionRes(WndRgn);	}
	AOIDataCollect.MapCadRegionToCamera(ModelImageW, ModelImageH, ImageRes, WndRgn, ModelRgnCp, ImageRgn);
	JetAPI::Region4DToRect(ImageRgn, WndRect, true);
	//::wcscpy(strTag, L"Loc");//Loc" : [100,100,300,200],	// 在零件圖上的位置(x1 y1 x2 y2)(pixel)
	//::fwprintf(pfile, L"          \"%s\": [%d,%d,%d,%d],\n", strTag, WndRect.left, WndRect.top, WndRect.right, WndRect.bottom);	

	if ( WndPtr->GetWndExtendBoxUsed()==false || true==bNoSaveExtendRgn )
	{	WndPtr->GetWndCornerPosRes(CadCornerPos); }
	else
	{	WndPtr->GetWndExtendBox().GetBoxCornerPosRes(CadCornerPos); }
	AOIDataCollect.MapCadCornerToCamera(ModelImageW, ModelImageH, ImageRes, CadCornerPos, ModelRgnCp, ImageCornerPos);						
	::wcscpy(strTag, L"Loc");//"Location" : [100,100,300,200,100,100,300,200],	// 在零件圖上的位置(x1 y1 x2 y2)(pixel)
	::fwprintf(pfile, L"          \"%s\": [%.0f,%.0f,%.0f,%.0f,%.0f,%.0f,%.0f,%.0f],\n", strTag, 
		ImageCornerPos[0].x, ImageCornerPos[0].y, ImageCornerPos[1].x, ImageCornerPos[1].y, 
		ImageCornerPos[2].x, ImageCornerPos[2].y, ImageCornerPos[3].x, ImageCornerPos[3].y);

	::wcscpy(strTag, L"Toward");//20210906
	::fwprintf(pfile, L"          \"%s\": %d,\n", strTag, AOIDataCollect.MapBoxTowardToSpcToward(WndPtr->GetWndToward()));	

	LandPtr = WndPtr->GetWndLandPtr();
	if ( NULL == LandPtr ) { LandIndex = 0; }
	else { LandIndex = LandPtr->GetLandIndex()+1; }
	::wcscpy(strTag, L"Lead_ID");//"Lead ID":0, 1	// 腳位標號(整數)
	::fwprintf(pfile, L"          \"%s\": %d,\n", strTag, LandIndex);

	::wcscpy(strTag, L"Test_Defect");//"Defect" : 34		// 瑕疵ID(整數-2 Byte)
	::fwprintf(pfile, L"          \"%s\": %d,\n", strTag, WndDefectID);

	::wcscpy(strTag, L"Check_Defect");//"Defect" : 34		// 瑕疵ID(整數-2 Byte)
	//::fwprintf(pfile, L"          \"%s\": %d,\n", strTag, WndDefectID);
	::fwprintf(pfile, L"          \"%s\": %d,\n", strTag, WND_DEFECT_NONE);	

	// 系統判定(整數-1 Byte) 0:No Test  1:Skip 2:Bypass 3:Sys-Ok 4:Sys-Ng 5:Op-Ok 6:Op-Ng 7:Fd-Ng 
	RESULT_ID WndResultID = WndPtr->GetWndResultID();
	SPC_RESULT_ID SpcResultID = AOIDataCollect.MapResultIDToSpcResultID(WndResultID);
	::wcscpy(strTag, L"Test_Result");			
	::fwprintf(pfile, L"          \"%s\": %d,\n", strTag, SpcResultID);						
						
	::wcscpy(strTag, L"Text");//"Text" : "offset x 10 um", // 額外文字(字串-64 Byte)
	strText = WndPtr->GetWndResultText();
	JetAPI::TCHAR2wchar(strText, strBuffer, szBuffer);
	JetAPI::AddBackslash(strBuffer, szBuffer);
	::fwprintf(pfile, L"          \"%s\": \"%s\",\n", strTag, strBuffer);
						
	::wcscpy(strTag, L"IsSeriousDefect");//"IsSeriousDefect":true ? false	
	if ( WndPtr->GetWndDefectAlarmEnableOnARS()==false || WndPtr->GetWndDefectAlarm()==false  )
	{	::wcscpy(strBuffer, L"false"); }
	else
	{	::wcscpy(strBuffer, L"true"); }	
	::fwprintf(pfile, L"          \"%s\": %s,\n", strTag, strBuffer);	

	int       ResBoxIdx=0;
	RESULT_ID BoxResultID;
	ResBoxIdxList.clear();
	size_t WndResBoxCount = WndPtr->GetWndResultBoxCount();
	for ( v=0; v<WndResBoxCount; v++ )
	{
		BoxPtr = WndPtr->GetWndResultBoxPtr(v, false);
		if ( NULL == BoxPtr )  { continue; }
		BoxResultID = BoxPtr->GetBoxResultID();
		if ( RESULT_ID_NONE == BoxResultID ) { continue; }
		if ( RESULT_ID_OK == BoxResultID ) { continue; }
		if ( RESULT_ID_SKIP == BoxResultID ) { continue; }
		if ( RESULT_ID_BYPASS == BoxResultID ) { continue; }							
		ResBoxIdxList.push_back(v);
	}
	int ResBoxIdxCount = (int)(ResBoxIdxList.size());

	::wcscpy(strTag, L"N_Boxs");//"N Boxs" : 2,		// Boxs數量
	::fwprintf(pfile, L"          \"%s\": %d,\n", strTag, ResBoxIdxCount);

	::wcscpy(strTag, L"Test_NG_Box_List");//Test_NG_Box_List
	::fwprintf(pfile, L"          \"%s\": %s\n", strTag, L"[");//Test_NG_Box_List [
	bool bDumped=false;
	for ( v=0; v<ResBoxIdxCount; v++ )
	{
		ResBoxIdx = ResBoxIdxList[v];
		BoxPtr = WndPtr->GetWndResultBoxPtr(ResBoxIdx, false);
		if ( NULL == BoxPtr )  { continue; }							

		::fwprintf(pfile, L"        {\n");
		::fwprintf(pfile, L"          \"Index\": %d,\n", v+1);
		
		::wcscpy(strTag, L"Test_Defect");//"Defect" : 34		// 瑕疵ID(整數-2 Byte)
		::fwprintf(pfile, L"          \"%s\": %d,\n", strTag, WndDefectID);	

		::wcscpy(strTag, L"Check_Defect");//"Defect" : 34		// 瑕疵ID(整數-2 Byte)
	//::fwprintf(pfile, L"          \"%s\": %d,\n", strTag, WndDefectID);
		::fwprintf(pfile, L"          \"%s\": %d,\n", strTag, WND_DEFECT_NONE);	

		::wcscpy(strTag, L"Test_Result");
		::fwprintf(pfile, L"          \"%s\": %d,\n", strTag, SpcResultID);						

		::wcscpy(strTag, L"Check_Result");//Box Check Result
		::fwprintf(pfile, L"          \"%s\": %d,\n", strTag, 0);

		BoxPtr->GetBoxCornerPosRes(BoxCornerPos);
		AOIDataCollect.MapCadCornerToCamera(ModelImageW, ModelImageH, ImageRes, BoxCornerPos, ModelRgnCp, ImgCornerPos);
		::wcscpy(strTag, L"Loc");//"Loc" : [100,100,300,200,100,100,300,200],	// 在零件圖上的位置(LTx LTy RTx RTy LBx LBy RBx RBy)(pixel)							
		::fwprintf(pfile, L"          \"%s\": [%.0f,%.0f,%.0f,%.0f,%.0f,%.0f,%.0f,%.0f]\n", strTag, ImgCornerPos[0].x, ImgCornerPos[0].y, ImgCornerPos[1].x, ImgCornerPos[1].y, ImgCornerPos[2].x, ImgCornerPos[2].y, ImgCornerPos[3].x, ImgCornerPos[3].y);

		if ( v < (ResBoxIdxCount-1) )
		{	::fwprintf(pfile, L"        },\n"); }
		else
		{	::fwprintf(pfile, L"        }\n"); }

		RECT ImgRect;
		JetAPI::SizeToRect(ModelImageW, ModelImageH, ImgRect);		
		if ( JetAPI::CheckCornerInRect(ImgCornerPos, ImgRect) == false )		
		{	
			CString strErr, strErr2, strCornerCad, strCornerImg;			
			strErr2.Format(_T("ResBox[%d] MapCadCornerToCamera Exception, (ImgW:%d, ImgH:%d, ResX:%.6f, ResY:%.6f, RgnCpX:%.2f, RgnCpY:%.2f)"), ResBoxIdx+1, ModelImageW, ModelImageH, ImageRes.x, ImageRes.y, ModelRgnCp.x, ModelRgnCp.y);
			strCornerCad.Format(_T("Cad Corner P1(%.2f, %.2f), P2(%.2f, %.2f), P3(%.2f, %.2f), P4(%.2f, %.2f)"), BoxCornerPos[0].x, BoxCornerPos[0].y, BoxCornerPos[1].x, BoxCornerPos[1].y, BoxCornerPos[2].x, BoxCornerPos[2].y, BoxCornerPos[3].x, BoxCornerPos[3].y);
			strCornerImg.Format(_T("Img Corner P1(%.2f, %.2f), P2(%.2f, %.2f), P3(%.2f, %.2f), P4(%.2f, %.2f)"), ImgCornerPos[0].x, ImgCornerPos[0].y, ImgCornerPos[1].x, ImgCornerPos[1].y, ImgCornerPos[2].x, ImgCornerPos[2].y, ImgCornerPos[3].x, ImgCornerPos[3].y);
			strErr.Format(_T("%s\n%s\n%s\n"), strErr2, strCornerCad, strCornerImg);
			DumpWndException(strErr, bDumped);
			bDumped = true;
		}
	}
	::fwprintf(pfile, L"        ],\n");//Test_NG_Box_List ]
	
	if ( WndPtr->GetWndAlgParam().WriteAlgSpcFile_JSON_RSM(pfile, ProjectPtr) == false )//20211025
	{	return false; }

	::wcscpy(strTag, L"Check_Result");//Wnd Check Result
	::fwprintf(pfile, L"          \"%s\": %d\n", strTag, 0);
	return true;
}

bool CAOIWnd::WriteWndSpcHeader_JSON_detail(FILE *pfile, CAOIProject *ProjectPtr) {

	CAOIWnd *WndPtr = this;
	CAOILand *LandPtr = NULL;
	CAOIBox *BoxPtr = NULL;
	CAlgParam* WndAlgParamPtr = NULL;
	CAOIModel *ModelPtr = WndPtr->GetWndModelPtr();
	if ( NULL == ModelPtr ) { return false; }	

	CString          strText;
	const size_t     szBuffer = 256;
	wchar_t          strTag[szBuffer] = L"";
	wchar_t          strBuffer[szBuffer] = L"";

	TPOINT2D BoxCad;
	TSIZE2D BoxSize;

	LandPtr = WndPtr->GetWndLandPtr();
	BoxPtr = WndPtr->GetWndBoxPtr();
	BoxPtr->GetBoxPos(BoxCad);
	BoxPtr->GetBoxSize(BoxSize);
	WndAlgParamPtr = WndPtr->GetWndAlgParamPtr();

	unsigned int LandID = (LandPtr == NULL) ? 0 : LandPtr->GetLandIndex() + 1;

	::fwprintf(pfile, L"        {\n");

	::wcscpy(strTag, L"Wnd_ID");
	::fwprintf(pfile, L"          \"%s\":%d ,\n", strTag, WndPtr->GetWndIndex() + 1);

	::wcscpy(strTag, L"Toward");
	::fwprintf(pfile, L"          \"%s\":%d ,\n", strTag, AOIDataCollect.MapBoxTowardToSpcToward(WndPtr->GetWndToward()));

	::wcscpy(strTag, L"Lead_ID");
	::fwprintf(pfile, L"          \"%s\":%d ,\n", strTag, LandID);

	::wcscpy(strTag, L"Pad_ID");
	::fwprintf(pfile, L"          \"%s\":%d ,\n", strTag, LandID);
	

	WndAlgParamPtr->WriteAlgSpcFile_Parameter_JSON_ALG_ParaProperty(pfile);

	::wcscpy(strTag, L"Group_ID");
	::fwprintf(pfile, L"          \"%s\":%d ,\n", strTag, WndPtr->GetWndGroupID() + 1);

	::wcscpy(strTag, L"CenterX");
	::fwprintf(pfile, L"          \"%s\":%.3f,\n", strTag, BoxCad.x);

	::wcscpy(strTag, L"CenterY");
	::fwprintf(pfile, L"          \"%s\":%.3f,\n", strTag, BoxCad.y);

	::wcscpy(strTag, L"Width");
	::fwprintf(pfile, L"          \"%s\":%.3f,\n", strTag, BoxSize.cx);

	::wcscpy(strTag, L"Height");
	::fwprintf(pfile, L"          \"%s\":%.3f,\n", strTag, BoxSize.cy);

	//Add Loc	
	TREGION4D ModelRgn;
	TPOINT2D ModelRgnCp;
	TPOINT2D CadCornerPos[4];
	TPOINT2D ImageCornerPos[4];
	const bool bNoSaveExtendRgn = true;
	const double     ImageResX = AOIDataCollect.GetCameraResolutionX(PRIMARY_CAMERA_ID);
	const double     ImageResY = AOIDataCollect.GetCameraResolutionY(PRIMARY_CAMERA_ID);
	const TPOINT2D   ImageRes(ImageResX, ImageResY);
	ModelPtr->GetModelTotalRegion(ModelRgn);
	ModelRgnCp.x = ModelRgn.GetCpX();
	ModelRgnCp.y = ModelRgn.GetCpY();
	IMAGE_SIZE ModelImageW = JetAPI::Floor(ModelRgn.GetWidth()/ImageRes.x);
	IMAGE_SIZE ModelImageH = JetAPI::Floor(ModelRgn.GetHeight()/ImageRes.y);
	if ( WndPtr->GetWndExtendBoxUsed()==false || true==bNoSaveExtendRgn )
	{	WndPtr->GetWndCornerPos(CadCornerPos); }
	else
	{	WndPtr->GetWndExtendBox().GetBoxCornerPos(CadCornerPos); }
	AOIDataCollect.MapCadCornerToCamera(ModelImageW, ModelImageH, ImageRes, CadCornerPos, ModelRgnCp, ImageCornerPos);						
	::wcscpy(strTag, L"Loc");//"Location" : [100,100,300,200,100,100,300,200],	// 在零件圖上的位置(x1 y1 x2 y2)(pixel)
	::fwprintf(pfile, L"          \"%s\": [%.0f,%.0f,%.0f,%.0f,%.0f,%.0f,%.0f,%.0f],\n", strTag, 
		ImageCornerPos[0].x, ImageCornerPos[0].y, ImageCornerPos[1].x, ImageCornerPos[1].y, 
		ImageCornerPos[2].x, ImageCornerPos[2].y, ImageCornerPos[3].x, ImageCornerPos[3].y);	
	
	::wcscpy(strTag, L"AbleToCustomerStatus");//AbleToCustomerStatus=false
	const CWndDefectItem &DefectRecheck = ModelPtr->GetModelDefectItemRecheck_ARS();
	if ( 0 == DefectRecheck.GetItemCount(GetWndDefectID()) )
	{	::fwprintf(pfile, L"          \"%s\":false, \n", strTag);	}
	else
	{	::fwprintf(pfile, L"          \"%s\":true, \n", strTag);	}

	::wcscpy(strTag, L"Test_Defect");
	::fwprintf(pfile, L"          \"%s\":%d \n", strTag, WndPtr->GetWndDefectID());
	::fwprintf(pfile, L"        }");
	return TRUE;
}
//-------------------------------------------------------------------------------------//
inline void CAOIWnd::UpdateWndBoxEditabled()
{
	bool Editabled = true;
	CAOILand *LandPtr = CAOIWnd::GetWndLandPtr();
	if ( NULL == LandPtr )
	{
		//在本體上, 因考慮特徵的尺寸, 所以有些要限制
		switch ( m_WndRgnLinkMode )
		{
		case WND_RGN_LINK_NONE:
			Editabled = true;
			break;
		case WND_RGN_LINK_PAD:
			Editabled = false;
			break;
		case WND_RGN_LINK_PAD_TIP:
			Editabled = false;
			break;
		case WND_RGN_LINK_PAD_RGN:
			Editabled = false;
			break;
		case WND_RGN_LINK_PAD_BODY_RGN:
			Editabled = false;
			break;
		case WND_RGN_LINK_PAD_RGN_INNER:
			Editabled = false;
			break;
		case WND_RGN_LINK_BODY:
			if ( false == m_WndRgnLinkAuto )
			{	Editabled = true; }
			else
			{	Editabled = false; }
			break;		
		case WND_RGN_LINK_LEAD:
			Editabled = false;
			break;
		case WND_RGN_LINK_LEAD_TIP:
			Editabled = false;
			break;
		case WND_RGN_LINK_LEAD_SHOULDER:
			Editabled = false;
			break;
		case WND_RGN_LINK_LEAD_TIP_SHOULDER:
			Editabled = false;
			break;
		}		
	}
	else
	{
		//在特徵框上
		if ( WND_RGN_LINK_NONE==m_WndRgnLinkMode || false==m_WndRgnLinkAuto )
		{	Editabled = true;	}
		else
		{	Editabled = false;	}	 
	}
	m_WndBox.SetBoxEditabled(Editabled);
}
//-------------------------------------------------------------------------------------//
void CAOIWnd::ChangeWndDefectID(MODEL_TYPE ModelType, WND_DEFECT_ID WndDefectID)
{
	bool WndRgnLinkAuto = false;	
	WND_FOLLOW_MODE WndFollowMode=WND_FOLLOW_NONE;	
	WND_RGN_LINK_MODE WndRgnLinkMode = WND_RGN_LINK_NONE;
	ALG_TYPE AlgType = CAOIWnd::m_WndAlgParam.GetAlgType();	
	if ( ALG_MODEL_MATCH == AlgType )
	{
		switch ( WndDefectID )
		{
		case WND_DEFECT_PAD_ALIGN:
			WndRgnLinkAuto = true;
			WndRgnLinkMode = WND_RGN_LINK_PAD;
			break;
		case WND_DEFECT_PART_ALIGN:
			WndRgnLinkAuto = true;			
			WndRgnLinkMode = CAOIModel::ObtainModelDefaultWndRegionLinkMode(ModelType, WndDefectID);
			break;
		case WND_DEFECT_PAD_ADJUST:
			WndRgnLinkAuto = true;
			WndRgnLinkMode = WND_RGN_LINK_PAD_TIP;		
			break;
		case WND_DEFECT_LEAD_ADJUST:
			WndRgnLinkAuto = true;
			WndRgnLinkMode = WND_RGN_LINK_LEAD;		
			break;	
		}		
	}
	WndFollowMode = AOIDataDefine.GetWndFollowModeByWndDefectID(WndDefectID);
	SetWndDefectID(WndDefectID);	
	SetWndFollowMode(WndFollowMode);
	SetWndRgnLinkAuto(WndRgnLinkAuto);
	SetWndRgnLinkMode(WndRgnLinkMode);
	return;
}
//-------------------------------------------------------------------------------------//
bool CAOIWnd::ApplyWnd(const CAOIWnd *RefWndPtr)//更新檢測框參數-給相同群組使用
{
	if ( NULL == RefWndPtr ) { return false; }
	if ( m_WndGroupID != RefWndPtr->m_WndGroupID ) { return false; }
	if ( true == m_WndIsolated || true==RefWndPtr->m_WndIsolated ) { return true; }
	
	size_t i=0;
	DWORD  Res = 0;
	double dValue = 0;
	double nValue = 0;
	const BOX_TOWARD RefToward = RefWndPtr->GetWndToward();
	const BOX_TOWARD CurToward = GetWndToward();
	const int TowardAngle = CAOIBox::CalcBoxTowardAngle(RefToward, CurToward);	
	
	//CAOIBox
	m_WndBox.ApplyBox(&(RefWndPtr->m_WndBox));
	m_WndExtendBox.ApplyBox(&(RefWndPtr->m_WndExtendBox));	
	
	//CAOIWnd
	m_WndUIUpated_Param = false;
	m_WndClassID  = RefWndPtr->m_WndClassID;
	m_WndBypassed = RefWndPtr->m_WndBypassed;
	m_WndModified = RefWndPtr->m_WndModified;
	m_WndDefectID = RefWndPtr->m_WndDefectID;
	m_WndDefectAlarm = RefWndPtr->m_WndDefectAlarm;	
	m_WndDefectAlarmEnableOnAOI = RefWndPtr->m_WndDefectAlarmEnableOnAOI;
	m_WndDefectAlarmEnableOnARS = RefWndPtr->m_WndDefectAlarmEnableOnARS;
	m_WndDefectGroupID = RefWndPtr->m_WndDefectGroupID;	
	m_WndFollowMode = RefWndPtr->m_WndFollowMode;		
	m_WndSyncMoveMode = RefWndPtr->m_WndSyncMoveMode;

	m_WndLogicType = RefWndPtr->m_WndLogicType;		
	m_WndLogicGroupID = RefWndPtr->m_WndLogicGroupID;			
	//m_WndLogicResultID = RefWndPtr->m_WndLogicResultID;

	//Link Param
	m_WndRgnLinkAuto = RefWndPtr->m_WndRgnLinkAuto;
	m_WndRgnLinkMode = RefWndPtr->m_WndRgnLinkMode;
	m_WndRgnLinkGoupID = RefWndPtr->m_WndRgnLinkGoupID;

	m_WndTempInt[0] = RefWndPtr->m_WndTempInt[0];
	m_WndTempInt[1] = RefWndPtr->m_WndTempInt[1];
	m_WndTempInt[2] = RefWndPtr->m_WndTempInt[2];
	m_WndTempInt[3] = RefWndPtr->m_WndTempInt[3];
	switch ( TowardAngle )
	{
	case  90:
	case 270:
		m_WndRgnLinkRatioX = RefWndPtr->m_WndRgnLinkRatioY;
		m_WndRgnLinkRatioY = RefWndPtr->m_WndRgnLinkRatioX;
		switch ( RefWndPtr->m_WndConstrainMode )
		{
		case WND_CONSTRAIN_PAD_RGN_X_MOVE:	m_WndConstrainMode = WND_CONSTRAIN_PAD_RGN_Y_MOVE;	break;
		case WND_CONSTRAIN_PAD_RGN_Y_MOVE:	m_WndConstrainMode = WND_CONSTRAIN_PAD_RGN_X_MOVE;	break;
		default:
			m_WndConstrainMode = RefWndPtr->m_WndConstrainMode;
			break;
		}		
		break;
	default:
		m_WndRgnLinkRatioX = RefWndPtr->m_WndRgnLinkRatioX;
		m_WndRgnLinkRatioY = RefWndPtr->m_WndRgnLinkRatioY;
		m_WndConstrainMode = RefWndPtr->m_WndConstrainMode;
		break;
	}

	//Extend Box
	m_WndExtendBoxUsed = RefWndPtr->m_WndExtendBoxUsed;
	switch ( TowardAngle )
	{
	case  90:
	case 270:
		m_WndExtendRangeX     = RefWndPtr->m_WndExtendRangeY;
		m_WndExtendRangeY     = RefWndPtr->m_WndExtendRangeX;
		break;
	default:
		m_WndExtendRangeX     = RefWndPtr->m_WndExtendRangeX;
		m_WndExtendRangeY     = RefWndPtr->m_WndExtendRangeY;
		break;
	}	
	
	//檢測框-子檢測框列表
	CAOIWndRoi  *WndRoiPtr = NULL;
	CAOIWndRoi  *RefWndRoiPtr = NULL;
	const size_t RefWndRoiCount = RefWndPtr->GetWndRoiWndCount_Inline();
	const size_t CurWndRoiCount = GetWndRoiWndCount_Inline();
	if ( RefWndRoiCount == CurWndRoiCount )
	{
		for ( i=0; i<CurWndRoiCount; i++ )
		{
			WndRoiPtr = GetWndRoiWndPtr_Inline(i);
			RefWndRoiPtr = RefWndPtr->GetWndRoiWndPtr_Inline(i);
			if ( NULL==WndRoiPtr || NULL==RefWndRoiPtr ) { continue; }		
			WndRoiPtr->ApplyWndRoi(RefWndRoiPtr, TowardAngle);
		}
	}
	
	//檢測框-遮罩框列表
	CAOIWndMask *MaskWndPtr = NULL;
	CAOIWndMask *RefMaskWndPtr = NULL;
	const size_t RefMaskWndCount = RefWndPtr->GetWndMaskWndCount_Inline();	
	const size_t CurMaskWndCount = GetWndMaskWndCount_Inline();

	m_WndModelMaskFlag = RefWndPtr->m_WndModelMaskFlag;
	if ( RefMaskWndCount == CurMaskWndCount )
	{
		for ( i=0; i<CurMaskWndCount; i++ )
		{
			MaskWndPtr = GetWndMaskWndPtr_Inline(i);
			RefMaskWndPtr = RefWndPtr->GetWndMaskWndPtr_Inline(i);
			if ( NULL==MaskWndPtr || NULL==RefMaskWndPtr ) { continue; }		
			MaskWndPtr->ApplyWndMask(RefMaskWndPtr, TowardAngle);
		}
	}

	//Wnd Algorithm
	CAlgParam  tmpAlgParam = m_WndAlgParam;
	m_WndAlgParam = RefWndPtr->m_WndAlgParam;
	m_WndAlgParam.RotateAlgParam(TowardAngle);
	m_WndAlgParam.SetAlgWndPtr(this);
	m_WndAlgParam.SetAlgResultID(RESULT_ID_NONE);	
	m_WndAlgParam.CopyAlgParamResultValue(tmpAlgParam);	
	//m_WndAlgParam.CopyAlgParamIsolatedValue(tmpAlgParam);	

	/*	
	m_WndShapeMaskSupported = RefWndPtr->m_WndShapeMaskSupported;
	m_WndInnerMaskEnabled = RefWndPtr->m_WndInnerMaskEnabled;	
	switch ( TowardAngle )
	{
	case  90:
	case 270:
		m_WndInnerMaskRatioX     = RefWndPtr->m_WndInnerMaskRatioY;
		m_WndInnerMaskRatioY     = RefWndPtr->m_WndInnerMaskRatioX;
		break;
	default:
		m_WndInnerMaskRatioX     = RefWndPtr->m_WndInnerMaskRatioX;
		m_WndInnerMaskRatioY     = RefWndPtr->m_WndInnerMaskRatioY;
		break;
	}
	m_WndInnerMaskShapeMode = RefWndPtr->m_WndInnerMaskShapeMode;
	m_WndOuterMaskShapeMode = RefWndPtr->m_WndOuterMaskShapeMode;			
	*/
	const double AttachedAngle = GetWndAttachedAngle();
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(AttachedAngle);
	if ( IsExceptionAngle == false )
	{	
		UpdateWndExtendBoxKernel(); 
		//UpdateWndRoiBoxBind();
	}
	else
	{
		RotateWnd(-AttachedAngle, 0, 0);
		UpdateWndExtendBoxKernel();
		//UpdateWndRoiBoxBind();
		RotateWnd(AttachedAngle, 0, 0);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIWnd::SynchronousWnd(const CAOIWnd *RefWndPtr)//同步WndObj參數	
{
	if ( NULL == RefWndPtr ) { return false; }

	size_t i=0;
	DWORD  Res = 0;
	double dValue = 0;
	double nValue = 0;	
	
	//CAOIWnd
	m_WndUIUpated_Param = false;
	m_WndIndex = RefWndPtr->m_WndIndex;
	m_WndBandID = RefWndPtr->m_WndBandID;
	m_WndGroupID = RefWndPtr->m_WndGroupID;
	m_WndClassID = RefWndPtr->m_WndClassID;
	m_WndIsolated = RefWndPtr->m_WndIsolated;	
	m_WndDefectID = RefWndPtr->m_WndDefectID;	
	m_WndDefectAlarm = RefWndPtr->m_WndDefectAlarm;		
	m_WndDefectAlarmEnableOnAOI = RefWndPtr->m_WndDefectAlarmEnableOnAOI;
	m_WndDefectAlarmEnableOnARS = RefWndPtr->m_WndDefectAlarmEnableOnARS;
	m_WndDefectGroupID = RefWndPtr->m_WndDefectGroupID;		
	m_WndOrderIndex = RefWndPtr->m_WndOrderIndex;			
	//TPOINT2D                   m_WndRectCalValue;            //檢測框影像區域的校正-Float轉Int的誤差修正

	//CAOIBox
	m_WndBox.SynchronousBox(&(RefWndPtr->m_WndBox));		

	m_WndBypassed = RefWndPtr->m_WndBypassed;
	m_WndModified = RefWndPtr->m_WndModified;
	m_WndLandIndex = RefWndPtr->m_WndLandIndex;	
	m_WndFollowMode = RefWndPtr->m_WndFollowMode;	
	m_WndSyncMoveMode = RefWndPtr->m_WndSyncMoveMode;		
	m_WndConstrainMode = RefWndPtr->m_WndConstrainMode;

	m_WndLogicType = RefWndPtr->m_WndLogicType;		
	m_WndLogicGroupID = RefWndPtr->m_WndLogicGroupID;		
	//m_WndLogicResultID = RefWndPtr->m_WndLogicResultID;

	//Link Param
	m_WndRgnLinkAuto = RefWndPtr->m_WndRgnLinkAuto;
	m_WndRgnLinkMode = RefWndPtr->m_WndRgnLinkMode;
	m_WndRgnLinkGoupID = RefWndPtr->m_WndRgnLinkGoupID;
	m_WndRgnLinkRatioX     = RefWndPtr->m_WndRgnLinkRatioX;
	m_WndRgnLinkRatioY     = RefWndPtr->m_WndRgnLinkRatioY;

	m_WndTempInt[0] = RefWndPtr->m_WndTempInt[0];
	m_WndTempInt[1] = RefWndPtr->m_WndTempInt[1];
	m_WndTempInt[2] = RefWndPtr->m_WndTempInt[2];
	m_WndTempInt[3] = RefWndPtr->m_WndTempInt[3];

	//Extend Box
	m_WndExtendBoxUsed = RefWndPtr->m_WndExtendBoxUsed;	
	m_WndExtendRangeX     = RefWndPtr->m_WndExtendRangeX;
	m_WndExtendRangeY     = RefWndPtr->m_WndExtendRangeY;
	m_WndExtendBox.SynchronousBox(&(RefWndPtr->m_WndExtendBox));	

	//Wnd Roi Box
	CAOIWndRoi  *WndRoiPtr = NULL;
	CAOIWndRoi  *RefWndRoiPtr = NULL;
	const size_t RefWndRoiCount = RefWndPtr->GetWndRoiWndCount_Inline();
	const size_t CurWndRoiCount = GetWndRoiWndCount_Inline();
	if ( RefWndRoiCount == CurWndRoiCount )
	{
		for ( i=0; i<CurWndRoiCount; i++ )
		{
			WndRoiPtr = GetWndRoiWndPtr_Inline(i);
			RefWndRoiPtr = RefWndPtr->GetWndRoiWndPtr_Inline(i);
			if ( NULL==WndRoiPtr || NULL==RefWndRoiPtr ) { continue; }		
			WndRoiPtr->SynchronousWndRoi(RefWndRoiPtr);
		}
	}

	//Wnd Mask Box
	CAOIWndMask *MaskWndPtr = NULL;
	CAOIWndMask *RefMaskWndPtr = NULL;
	const size_t RefMaskWndCount = RefWndPtr->GetWndMaskWndCount_Inline();	
	const size_t CurMaskWndCount = GetWndMaskWndCount_Inline();
	m_WndModelMaskFlag = RefWndPtr->m_WndModelMaskFlag;
	if ( RefMaskWndCount == CurMaskWndCount )
	{
		for ( i=0; i<CurMaskWndCount; i++ )
		{
			MaskWndPtr = GetWndMaskWndPtr_Inline(i);
			RefMaskWndPtr = RefWndPtr->GetWndMaskWndPtr_Inline(i);
			if ( NULL==MaskWndPtr || NULL==RefMaskWndPtr ) { continue; }		
			MaskWndPtr->SynchronousWndMask(RefMaskWndPtr);
		}
	}
	//Wnd Algorithm
	CAlgParam  tmpAlgParam = m_WndAlgParam;
	m_WndAlgParam = RefWndPtr->m_WndAlgParam;
	m_WndAlgParam.SetAlgWndPtr(this);
	m_WndAlgParam.CopyAlgParamResultValue(tmpAlgParam);	
	//m_WndAlgParam.CopyAlgParamIsolatedValue(tmpAlgParam);	

	/*	
	m_WndShapeMaskSupported = WndPtr->m_WndShapeMaskSupported;
	m_WndInnerMaskEnabled = WndPtr->m_WndInnerMaskEnabled;		
	m_WndInnerMaskRatioX     = WndPtr->m_WndInnerMaskRatioX;
	m_WndInnerMaskRatioY     = WndPtr->m_WndInnerMaskRatioY;	
	m_WndInnerMaskShapeMode = WndPtr->m_WndInnerMaskShapeMode;
	m_WndOuterMaskShapeMode = WndPtr->m_WndOuterMaskShapeMode;			
	*/	
	return true;
}
//-------------------------------------------------------------------------------------//  
bool CAOIWnd::ApplyWndDefault(const CAOIWnd *RefWndPtr)//更新檢測框參數-給預設檢測框
{
	if ( NULL == RefWndPtr ) { return false; }

	size_t i=0;
	DWORD  Res = 0;
	double dValue = 0;
	double nValue = 0;	
	const bool bApplyBox=true;
	const int TowardAngle = 0;
	//CAOIWnd
	m_WndUIUpated_Param = false;	
	//TPOINT2D                   m_WndRectCalValue;            //檢測框影像區域的校正-Float轉Int的誤差修正

	//CAOIBox
	if ( true == bApplyBox )
	{	
		//m_WndBox.ApplyBox(&(RefWndPtr->m_WndBox)); 
		m_WndBox.SynchronousBox(&(RefWndPtr->m_WndBox)); 
	}

	m_WndModified = true;	
	m_WndFollowMode = RefWndPtr->m_WndFollowMode;	
	m_WndSyncMoveMode = RefWndPtr->m_WndSyncMoveMode;		
	m_WndConstrainMode = RefWndPtr->m_WndConstrainMode;

	m_WndLogicType = RefWndPtr->m_WndLogicType;
	m_WndLogicGroupID = RefWndPtr->m_WndLogicGroupID;		
	//m_WndLogicResultID = RefWndPtr->m_WndLogicResultID;

	//Link Param
	m_WndRgnLinkAuto = RefWndPtr->m_WndRgnLinkAuto;
	m_WndRgnLinkMode = RefWndPtr->m_WndRgnLinkMode;
	m_WndRgnLinkGoupID = RefWndPtr->m_WndRgnLinkGoupID;
	m_WndRgnLinkRatioX     = RefWndPtr->m_WndRgnLinkRatioX;
	m_WndRgnLinkRatioY     = RefWndPtr->m_WndRgnLinkRatioY;

	m_WndTempInt[0] = RefWndPtr->m_WndTempInt[0];
	m_WndTempInt[1] = RefWndPtr->m_WndTempInt[1];
	m_WndTempInt[2] = RefWndPtr->m_WndTempInt[2];
	m_WndTempInt[3] = RefWndPtr->m_WndTempInt[3];

	//Extend Box
	m_WndExtendBoxUsed = RefWndPtr->m_WndExtendBoxUsed;	
	m_WndExtendRangeX     = RefWndPtr->m_WndExtendRangeX;
	m_WndExtendRangeY     = RefWndPtr->m_WndExtendRangeY;
	if ( true == bApplyBox )
	{	
		//m_WndExtendBox.ApplyBox(&(RefWndPtr->m_WndExtendBox)); 
		m_WndExtendBox.SynchronousBox(&(RefWndPtr->m_WndExtendBox)); 
	}

	//Wnd Roi Box
	CAOIWndRoi  *WndRoiPtr = NULL;
	CAOIWndRoi  *RefWndRoiPtr = NULL;
	const size_t RefWndRoiCount = RefWndPtr->GetWndRoiWndCount_Inline();
	const size_t CurWndRoiCount = GetWndRoiWndCount_Inline();
	if ( RefWndRoiCount == CurWndRoiCount )
	{
		for ( i=0; i<CurWndRoiCount; i++ )
		{
			WndRoiPtr = GetWndRoiWndPtr_Inline(i);
			RefWndRoiPtr = RefWndPtr->GetWndRoiWndPtr_Inline(i);
			if ( NULL==WndRoiPtr || NULL==RefWndRoiPtr ) { continue; }		
			if ( true == bApplyBox )
			{	WndRoiPtr->ApplyWndRoi(RefWndRoiPtr, TowardAngle); }
		}
	}

	//Wnd Mask Box
	CAOIWndMask *MaskWndPtr = NULL;
	CAOIWndMask *RefMaskWndPtr = NULL;
	const size_t RefMaskWndCount = RefWndPtr->GetWndMaskWndCount_Inline();	
	const size_t CurMaskWndCount = GetWndMaskWndCount_Inline();
	m_WndModelMaskFlag = RefWndPtr->m_WndModelMaskFlag;
	if ( RefMaskWndCount == CurMaskWndCount )
	{
		for ( i=0; i<CurMaskWndCount; i++ )
		{
			MaskWndPtr = GetWndMaskWndPtr_Inline(i);
			RefMaskWndPtr = RefWndPtr->GetWndMaskWndPtr_Inline(i);
			if ( NULL==MaskWndPtr || NULL==RefMaskWndPtr ) { continue; }	
			if ( true == bApplyBox )
			{	MaskWndPtr->ApplyWndMask(RefMaskWndPtr, TowardAngle); }
		}
	}
	//Wnd Algorithm		
	CAlgParam  tmpAlgParam = m_WndAlgParam;
	CString    AlgPatternFolder = tmpAlgParam.GetAlgPatternFolder();
	m_WndAlgParam = RefWndPtr->m_WndAlgParam;
	m_WndAlgParam.SetAlgWndPtr(this);
	m_WndAlgParam.CopyAlgParamResultValue(tmpAlgParam);	
	m_WndAlgParam.ClearAlgPatternFiles();
	m_WndAlgParam.SetAlgPatternFolder(AlgPatternFolder);
	//m_WndAlgParam.CopyAlgParamIsolatedValue(tmpAlgParam);	

	/*	
	m_WndShapeMaskSupported = WndPtr->m_WndShapeMaskSupported;
	m_WndInnerMaskEnabled = WndPtr->m_WndInnerMaskEnabled;		
	m_WndInnerMaskRatioX     = WndPtr->m_WndInnerMaskRatioX;
	m_WndInnerMaskRatioY     = WndPtr->m_WndInnerMaskRatioY;	
	m_WndInnerMaskShapeMode = WndPtr->m_WndInnerMaskShapeMode;
	m_WndOuterMaskShapeMode = WndPtr->m_WndOuterMaskShapeMode;			
	*/	
	return true;
}
//-------------------------------------------------------------------------------------//  
bool CAOIWnd::CheckWndLinkPos(CAOIWnd *WndPtr) const//確認檢測框位置連動
{
	if ( NULL == WndPtr ) { return false; }
	if ( GetWndBandID() != WndPtr->GetWndBandID() ) { return false; }
	if ( GetWndGroupID() != WndPtr->GetWndGroupID() ) { return false; }	
	return true;
}
//-------------------------------------------------------------------------------------//  
bool CAOIWnd::CheckWndLinkSize(CAOIWnd *WndPtr) const//確認檢測框尺寸連動
{
	if ( NULL == WndPtr ) { return false; }	
	if ( GetWndGroupID() != WndPtr->GetWndGroupID() ) { return false; }	
	return true;
}
//-------------------------------------------------------------------------------------//  
bool CAOIWnd::CheckWndClassIDUsed(int ID) const//確認檢測框類別-檢測用
{	
	if ( 0 == m_WndClassID ) { return true; }
	if ( MODEL_CLASS_ID_NONE == ID ) { return true; }
	return m_WndClassID==ID ? true:false;		
}
//-------------------------------------------------------------------------------------//  
 void CAOIWnd::SetWndRgnLinkAuto(bool value)
 {
	 m_WndRgnLinkAuto = value;
	 UpdateWndBoxEditabled();
 }
 //-------------------------------------------------------------------------------------//
 void CAOIWnd::SetWndRgnLinkMode(WND_RGN_LINK_MODE value)
 {
	 m_WndRgnLinkMode = value;	 
	 UpdateWndBoxEditabled();
 }
 //-------------------------------------------------------------------------------------// 
 int CAOIWnd::GetWndTempInt1() const
 {
	 return m_WndTempInt[0];
 }
 //-------------------------------------------------------------------------------------// 
void CAOIWnd::SetWndTempInt1(int value)
{
	m_WndTempInt[0] = value;
}
//-------------------------------------------------------------------------------------// 
int CAOIWnd::GetWndTempInt2() const
{
	return m_WndTempInt[1];
}
//-------------------------------------------------------------------------------------// 
void CAOIWnd::SetWndTempInt2(int value)
{
	m_WndTempInt[1] = value;
}
//-------------------------------------------------------------------------------------// 
int CAOIWnd::GetWndTempInt3() const
{
	return m_WndTempInt[2];
}
//-------------------------------------------------------------------------------------// 
void CAOIWnd::SetWndTempInt3(int value)
{
	m_WndTempInt[2] = value;
}
//-------------------------------------------------------------------------------------// 
int CAOIWnd::GetWndTempInt4() const
{
	return m_WndTempInt[3];
}
//-------------------------------------------------------------------------------------// 
void CAOIWnd::SetWndTempInt4(int value)
{
	m_WndTempInt[3] = value;
}
//-------------------------------------------------------------------------------------// 
int CAOIWnd::GetWndTempInt(int idx) const
{
	return m_WndTempInt[idx];
}
 //-------------------------------------------------------------------------------------//
void CAOIWnd::SetWndTempInt(int value, int idx)
{
	m_WndTempInt[idx] = value;
}
 //-------------------------------------------------------------------------------------//
double CAOIWnd::GetWndAngleSkew() const
{ 
	return m_WndBox.GetBoxAngleSkew(); 
}
//-------------------------------------------------------------------------------------//
void CAOIWnd::GetWndUseCornerPos(TPOINT2D CornerPos[]) const
{
	const bool bExtendUsed=GetWndExtendBoxUsed();
	if ( false == bExtendUsed )
	{	GetWndCornerPos(CornerPos);	}
	else
	{	GetWndExtendCornerPos(CornerPos);	}
}
//-------------------------------------------------------------------------------------//
void CAOIWnd::GetWndUseCornerPosRes(TPOINT2D CornerPos[]) const
{ 
	const bool bExtendUsed=GetWndExtendBoxUsed();
	if ( false == bExtendUsed )
	{	GetWndCornerPosRes(CornerPos);	}
	else
	{	GetWndExtendCornerPosRes(CornerPos);	}
}
//-------------------------------------------------------------------------------------//
void CAOIWnd::GetWndUseCornerPosCad(TPOINT2D CornerPos[]) const
{ 
	const bool bExtendUsed=GetWndExtendBoxUsed();
	if ( false == bExtendUsed )
	{	GetWndCornerPosCad(CornerPos);	}
	else
	{	GetWndExtendCornerPosCad(CornerPos);	}
}
//-------------------------------------------------------------------------------------//
void CAOIWnd::GetWndUseCornerPosCadRes(TPOINT2D CornerPos[]) const
{ 
	const bool bExtendUsed=GetWndExtendBoxUsed();
	if ( false == bExtendUsed )
	{	GetWndCornerPosCadRes(CornerPos);	}
	else
	{	GetWndExtendCornerPosCadRes(CornerPos);	}
}
//-------------------------------------------------------------------------------------//
void CAOIWnd::GetWndUseCornerPosStage(TPOINT2D CornerPos[]) const
{ 
	const bool bExtendUsed=GetWndExtendBoxUsed();
	if ( false == bExtendUsed )
	{	GetWndCornerPosStage(CornerPos);	}
	else
	{	GetWndExtendCornerPosStage(CornerPos);	}
}
//-------------------------------------------------------------------------------------//
void CAOIWnd::GetWndUseCornerPosStageRes(TPOINT2D CornerPos[]) const
{ 
	const bool bExtendUsed=GetWndExtendBoxUsed();
	if ( false == bExtendUsed )
	{	GetWndCornerPosStageRes(CornerPos);	}
	else
	{	GetWndExtendCornerPosStageRes(CornerPos);	}
}
//-------------------------------------------------------------------------------------//
void CAOIWnd::GetWndUseRegion(TREGION4D &Region) const
{ 
	const bool bExtendUsed=GetWndExtendBoxUsed();
	if ( false == bExtendUsed )
	{	GetWndRegion(Region);	}
	else
	{	GetWndExtendRegion(Region);	}
}
//-------------------------------------------------------------------------------------//
void CAOIWnd::GetWndUseRegionRes(TREGION4D &Region) const
{ 
	const bool bExtendUsed=GetWndExtendBoxUsed();
	if ( false == bExtendUsed )
	{	GetWndRegionRes(Region);	}
	else
	{	GetWndExtendRegionRes(Region);	}
}
//-------------------------------------------------------------------------------------//
void CAOIWnd::GetWndUseRegionCad(TREGION4D &Region) const
{ 
	const bool bExtendUsed=GetWndExtendBoxUsed();
	if ( false == bExtendUsed )
	{	GetWndRegionCad(Region);	}
	else
	{	GetWndExtendRegionCad(Region);	}
}
//-------------------------------------------------------------------------------------//
void CAOIWnd::GetWndUseRegionCadRes(TREGION4D &Region) const
{
	const bool bExtendUsed=GetWndExtendBoxUsed();
	if ( false == bExtendUsed )
	{	GetWndRegionCadRes(Region);	}
	else
	{	GetWndExtendRegionCadRes(Region);	}
}
//-------------------------------------------------------------------------------------//
void CAOIWnd::GetWndUseRegionStage(TREGION4D &Region) const
{ 
	const bool bExtendUsed=GetWndExtendBoxUsed();
	if ( false == bExtendUsed )
	{	GetWndRegionStage(Region);	}
	else
	{	GetWndExtendRegionStage(Region);	}
}
//-------------------------------------------------------------------------------------//
void CAOIWnd::GetWndUseRegionStageRes(TREGION4D &Region) const
{
	const bool bExtendUsed=GetWndExtendBoxUsed();
	if ( false == bExtendUsed )
	{	GetWndRegionStageRes(Region);	}
	else
	{	GetWndExtendRegionStageRes(Region);	}
}
//-------------------------------------------------------------------------------------//
void CAOIWnd::GetWndUseRegion(double &MinX, double &MinY, double &MaxX, double &MaxY) const
{ 
	const bool bExtendUsed=GetWndExtendBoxUsed();
	if ( false == bExtendUsed )
	{	GetWndRegion(MinX, MinY, MaxX, MaxY);	}
	else
	{	GetWndExtendRegion(MinX, MinY, MaxX, MaxY);	}
}
//-------------------------------------------------------------------------------------//
void CAOIWnd::SetWndRegion(const TREGION4D &Region, bool IncludeRes)
{
	CAOIWnd::m_WndBox.SetBoxRegion(Region, IncludeRes);
}
//-------------------------------------------------------------------------------------//
void CAOIWnd::SetWndRegion(double MinX, double MinY, double MaxX, double MaxY, bool IncludeRes)
{
	CAOIWnd::m_WndBox.SetBoxRegion(MinX, MinY, MaxX, MaxY, IncludeRes);
}
//-------------------------------------------------------------------------------------//
int CAOIWnd::GetWndBasicBoxID(const CAOIBox *BoxPtr) const
{
	if ( BoxPtr == &m_WndBox )
	{	return WND_BOX_BASIC; }
	if ( BoxPtr == &m_WndExtendBox )
	{	return WND_BOX_EXTEND; }
	return 0;
}
//-------------------------------------------------------------------------------------//
CAOIBox* CAOIWnd::GetWndBasicBoxPtr(int BasicBoxID)
{
	CAOIBox *BoxPtr = NULL;
	switch ( BasicBoxID )
	{
	case WND_BOX_BASIC:		BoxPtr = &m_WndBox;	break;
	case WND_BOX_EXTEND:	BoxPtr = &m_WndExtendBox;	break;
	}
	return BoxPtr;
}
//-------------------------------------------------------------------------------------//
void CAOIWnd::UpdateWndExtendBox()
{
	const double AttachedAngle = CAOIWnd::GetWndAttachedAngle();
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(AttachedAngle);	

	if ( IsExceptionAngle == false )
	{	CAOIWnd::UpdateWndExtendBoxKernel(); }
	else
	{
		CAOIWnd::RotateWnd(-AttachedAngle, 0, 0);
		CAOIWnd::UpdateWndExtendBoxKernel();
		CAOIWnd::RotateWnd(AttachedAngle, 0, 0);
	}
}
//-------------------------------------------------------------------------------------//
bool CAOIWnd::CheckWndUsedShapeMode() const//確認檢測框演算法是否可以使用外形框
{	
	ALG_TYPE AlgType = GetWndAlgType();
	return CheckWndUsedShapeMode(AlgType);	
}
//-------------------------------------------------------------------------------------//
bool CAOIWnd::CheckWndUsedShapeMode(ALG_TYPE AlgType) const//確認檢測框演算法是否可以使用外形框
{
	bool bUsed = false;	
	switch ( AlgType )
	{
	case ALG_BRIGHT_RATIO:
	case ALG_BLOB_COUNT:
	case ALG_BODY_TILT:
	case ALG_MODEL_MATCH:	
	case ALG_OBJECT_MEASURE:
	case ALG_SHAPE_VERIFY://外形驗證
	case ALG_SOLDER_WETTING:
		bUsed = true;
		break;	
	case ALG_FD_MATCH:	
	case ALG_OUTER_SHORT:
	case ALG_BARCODE_RECOGNIZE:	
	case ALG_COLOR_CODE:
	case ALG_IMAGE_MATCH:
	case ALG_CHAR_VERIFY:
	case ALG_EDGE_SEARCH:
	case ALG_ANGLE_MEASURE:
	case ALG_PIXEL_COMPARE:	
	case ALG_MEASURE_BLACK_GLUE:
	case ALG_MEASURE_FLUX_AREA:
	case ALG_MEASURE_CPU_PIN:
	case ALG_MEASURE_SIP_DISTANCE:
	case ALG_MEASURE_CONNECTOR:
	case ALG_MEASURE_CONNECTOR_PIN:
		bUsed = false;
		break;
	}	
	return bUsed;
}
//-------------------------------------------------------------------------------------//
inline void CAOIWnd::UpdateWndExtendBoxKernel()
{
	double   MinX=0, MinY=0, MaxX=0, MaxY=0;	
	const double ExtendX = GetWndExtendRangeX();
	const double ExtendY = GetWndExtendRangeY();	
	CAOIBox *ExtendBoxPtr = GetWndExtendBoxPtr();
	GetWndRegion(MinX, MinY, MaxX, MaxY);
	MinX -= ExtendX;
	MinY -= ExtendY;
	MaxX += ExtendX;
	MaxY += ExtendY;
	ExtendBoxPtr->SetBoxRegion(MinX, MinY, MaxX, MaxY);	
	
	TPOINT2D AttachedPosCad, AttachedPosStage;	
	const double AttachedAngle = CAOIWnd::GetWndAttachedAngle();
	GetWndAttachedPosCad(AttachedPosCad);
	GetWndAttachedPosStage(AttachedPosStage);	

	ExtendBoxPtr->SetBoxAttachedAngle(AttachedAngle);
	ExtendBoxPtr->SetBoxAttachedPosCad(AttachedPosCad);
	ExtendBoxPtr->SetBoxAttachedPosStage(AttachedPosStage);
}
//-------------------------------------------------------------------------------------//
inline void CAOIWnd::ClearWndResultBoxList_Inline()
{
	m_WndResultBoxList.clear();
}
//-------------------------------------------------------------------------------------//
inline size_t CAOIWnd::GetWndResultBoxCount_Inline() const
{
	return CAOIWnd::m_WndResultBoxList.size();
}
//-------------------------------------------------------------------------------------//
inline void CAOIWnd::AddWndResultBox_Inline(const CAOIBox &Box)
{
	m_WndResultBoxList.push_back(Box);
}
//-------------------------------------------------------------------------------------//
inline CAOIBox* CAOIWnd::GetWndResultBoxPtr_Inline(size_t index)
{
	return &(m_WndResultBoxList[index]);
}
//-------------------------------------------------------------------------------------//
inline size_t CAOIWnd::GetWndMaskWndCount_Inline() const
{
	return m_WndMaskWndList.size();
}
//-------------------------------------------------------------------------------------//
inline void CAOIWnd::AddWndMaskWnd_Inline(CAOIWndMask *WndMaskPtr)
{
	m_WndMaskWndList.push_back(WndMaskPtr);
}
//-------------------------------------------------------------------------------------//
inline CAOIWndMask* CAOIWnd::GetWndMaskWndPtr_Inline(size_t index) const
{
	return m_WndMaskWndList[index];
}
//-------------------------------------------------------------------------------------//
inline size_t CAOIWnd::GetWndRoiWndCount_Inline() const
{
	return m_WndRoiWndList.size();
}
//-------------------------------------------------------------------------------------//	
inline void CAOIWnd::AddWndRoiWnd_Inline(CAOIWndRoi *WndRoiPtr)
{	
	m_WndRoiWndList.push_back(WndRoiPtr);
}
//-------------------------------------------------------------------------------------//	
inline CAOIWndRoi* CAOIWnd::GetWndRoiWndPtr_Inline(size_t index) const
{
	return m_WndRoiWndList[index];
}
//-------------------------------------------------------------------------------------//	
bool CAOIWnd::BuildWndRoiWndListKernel(int RoiCntX, int RoiCntY, double MarginXRatio, double MarginYRatio)
{
	int nRoiCountX = RoiCntX;
	int nRoiCountY = RoiCntY;
	if ( nRoiCountX <0 ) { nRoiCountX = 0; }
	if ( nRoiCountY <0 ) { nRoiCountY = 0; }

	size_t       i = 0;
	const size_t RoiCountX = nRoiCountX;
	const size_t RoiCountY = nRoiCountY;
	const size_t RoiCountXY = RoiCountX*RoiCountY;
	size_t RoiBoxCount = GetWndRoiWndCount_Inline();
	if ( RoiCountXY != (RoiCountX*RoiBoxCount) )
	{	ClearWndRoiWndList();	}
	if ( 0 == RoiCountXY )
	{	return true; }	
	
	CAOIWndRoi *WndRoiPtr=NULL;
	CAOIModel *ModelPtr = GetWndModelPtr();
	ALG_TYPE AlgType = GetWndAlgType();
	CAOIBox    BoxWnd = CAOIWnd::m_WndBox;
	if ( RoiCountXY != (RoiCountX*RoiBoxCount) )
	{	
		for ( i=0; i<RoiCountXY; i++ )
		{	
			WndRoiPtr = AOIObjManager.CreateWndRoiObj();
			if ( NULL == WndRoiPtr ) { return false; }
			WndRoiPtr->SetWndRoiToward(GetWndToward());
			AddWndRoiWndPtr(WndRoiPtr, false);
			if (false == ModelPtr->CheckModelAlgWndRoiSelfFrameEnabled(AlgType)) { continue; }
			WndRoiPtr->SetWndRoiSelfFrameEnabled(true);
		}
	}
	CAOIWnd::BuildWndRoiWndListKernel_DivideXY(false, nRoiCountX, nRoiCountY, MarginXRatio, MarginYRatio, BoxWnd, m_WndRoiWndList);
	CAOIWnd::BuildWndRoiWndListKernel_DivideXY(true, nRoiCountX, nRoiCountY, MarginXRatio, MarginYRatio, BoxWnd, m_WndRoiWndList);
	return true;
}
//-------------------------------------------------------------------------------------//	
bool CAOIWnd::BuildWndRoiWndListKernel_DivideXY(bool Result, int DivideX, int DivideY, double MarginXRatio, double MarginYRatio, const CAOIBox &BoxWnd, std::vector<CAOIWndRoi*> &WndRoiList)
{
	TREGION4D RgnWnd;	
	double SizeX=0, SizeY=0;	
	double MinX=0, MinY=0, MaxX=0, MaxY=0;
	double RoiCPX=0, RoiCPY=0;
	double RoiSizeX=0, RoiSizeY=0;
	double RoiSizeX2=0, RoiSizeY2=0;
	double RoiSizeX3=0, RoiSizeY3=0;
	double WndCPX=0, WndCPY=0;
	double WndSizeX=0, WndSizeY=0;		
	const BOX_TOWARD Toward = BoxWnd.GetBoxToward();	
	
	size_t i=0;
	size_t idxX=0, idxY=0;
	CAOIWndRoi  *WndRoiPtr=NULL;	
	const size_t WndRoiCount = WndRoiList.size();

	if ( true == Result )
	{	BoxWnd.GetBoxRegionRes(RgnWnd);	}
	else
	{	BoxWnd.GetBoxRegion(RgnWnd); }	

	WndSizeX = (RgnWnd.maxX-RgnWnd.minX);
	WndSizeY = (RgnWnd.maxY-RgnWnd.minY);
	WndCPX = (RgnWnd.maxX+RgnWnd.minX)*0.5;
	WndCPY = (RgnWnd.maxY+RgnWnd.minY)*0.5;		

	if ( 100>MarginXRatio && MarginXRatio>0.001 )
	{	
		WndSizeX *= (100.0-MarginXRatio)/100.0;	
		RgnWnd.minX = WndCPX-(WndSizeX*0.5);
		RgnWnd.maxX = WndCPX+(WndSizeX*0.5);
	}

	if ( 100>MarginYRatio && MarginYRatio>0.001 )
	{	
		WndSizeY *= (100.0-MarginYRatio)/100.0;	
		RgnWnd.minY = WndCPY-(WndSizeY*0.5);
		RgnWnd.maxY = WndCPY+(WndSizeY*0.5);
	}

	const double RoiSizeScaleX=0.8;
	const double RoiSizeScaleY=0.8;
	switch ( Toward )
	{
	case BOX_TOWARD_UP:
		RoiCPX= RoiCPY = 0;
		RoiSizeX  = WndSizeX/DivideX;
		RoiSizeY  = WndSizeY/DivideY;		
		RoiSizeX2 = RoiSizeX/2;
		RoiSizeY2 = RoiSizeY/2;
		RoiSizeX3 = RoiSizeX2*RoiSizeScaleX;
		RoiSizeY3 = RoiSizeY2*RoiSizeScaleY;
		for ( i=0; i<WndRoiCount; i++ )
		{
			WndRoiPtr = (WndRoiList[i]);	
			
			idxX = i%DivideX;
			idxY = i/DivideX;

			RoiCPX = (idxX*RoiSizeX)+RoiSizeX2+RgnWnd.minX;
			RoiCPY = RgnWnd.maxY-RoiSizeY2-(idxY*RoiSizeY);
			MinX = RoiCPX-RoiSizeX3;
			MinY = RoiCPY-RoiSizeY3;
			MaxX = RoiCPX+RoiSizeX3;
			MaxY = RoiCPY+RoiSizeY3;
			
			if ( true == Result )
			{	WndRoiPtr->SetWndRoiRegionRes(MinX, MinY, MaxX, MaxY); }
			else
			{	WndRoiPtr->SetWndRoiRegion(MinX, MinY, MaxX, MaxY, false); }
		}
		break;
	case BOX_TOWARD_LEFT:
		RoiCPX= RoiCPY = 0;
		RoiSizeX  = WndSizeX/DivideX;
		RoiSizeY  = WndSizeY/DivideY;		
		RoiSizeX2 = RoiSizeX/2;
		RoiSizeY2 = RoiSizeY/2;
		RoiSizeX3 = RoiSizeX2*RoiSizeScaleX;
		RoiSizeY3 = RoiSizeY2*RoiSizeScaleY;
		for ( i=0; i<WndRoiCount; i++ )
		{
			WndRoiPtr = (WndRoiList[i]);	
			
			idxX = i%DivideX;
			idxY = i/DivideX;

			RoiCPX = (idxX*RoiSizeX)+RoiSizeX2+RgnWnd.minX;
			RoiCPY = (idxY*RoiSizeY)+RoiSizeY2+RgnWnd.minY;
			MinX = RoiCPX-RoiSizeX3;
			MinY = RoiCPY-RoiSizeY3;
			MaxX = RoiCPX+RoiSizeX3;
			MaxY = RoiCPY+RoiSizeY3;

			if ( true == Result )
			{	WndRoiPtr->SetWndRoiRegionRes(MinX, MinY, MaxX, MaxY); }
			else
			{	WndRoiPtr->SetWndRoiRegion(MinX, MinY, MaxX, MaxY, false); }
		}
		break;
	case BOX_TOWARD_DOWN:
		RoiCPX= RoiCPY = 0;
		RoiSizeX  = WndSizeX/DivideX;
		RoiSizeY  = WndSizeY/DivideY;		
		RoiSizeX2 = RoiSizeX/2;
		RoiSizeY2 = RoiSizeY/2;
		RoiSizeX3 = RoiSizeX2*RoiSizeScaleX;
		RoiSizeY3 = RoiSizeY2*RoiSizeScaleY;
		for ( i=0; i<WndRoiCount; i++ )
		{
			WndRoiPtr = (WndRoiList[i]);	
			
			idxX = i%DivideX;
			idxY = i/DivideX;

			RoiCPX = RgnWnd.maxX-RoiSizeX2-(idxX*RoiSizeX);
			RoiCPY = (idxY*RoiSizeY)+RoiSizeY2+RgnWnd.minY;
			MinX = RoiCPX-RoiSizeX3;
			MinY = RoiCPY-RoiSizeY3;
			MaxX = RoiCPX+RoiSizeX3;
			MaxY = RoiCPY+RoiSizeY3;

			if ( true == Result )
			{	WndRoiPtr->SetWndRoiRegionRes(MinX, MinY, MaxX, MaxY); }
			else
			{	WndRoiPtr->SetWndRoiRegion(MinX, MinY, MaxX, MaxY, false); }
		}
		break;
	case BOX_TOWARD_RIGHT:
		RoiCPX= RoiCPY = 0;
		RoiSizeX  = WndSizeX/DivideX;
		RoiSizeY  = WndSizeY/DivideY;		
		RoiSizeX2 = RoiSizeX/2;
		RoiSizeY2 = RoiSizeY/2;
		RoiSizeX3 = RoiSizeX2*RoiSizeScaleX;
		RoiSizeY3 = RoiSizeY2*RoiSizeScaleY;
		for ( i=0; i<WndRoiCount; i++ )
		{
			WndRoiPtr = (WndRoiList[i]);	
			
			idxX = i%DivideX;
			idxY = i/DivideX;

			RoiCPX = RgnWnd.maxX-RoiSizeX2-(idxX*RoiSizeX);
			RoiCPY = RgnWnd.maxY-RoiSizeY2-(idxY*RoiSizeY);
			MinX = RoiCPX-RoiSizeX3;
			MinY = RoiCPY-RoiSizeY3;
			MaxX = RoiCPX+RoiSizeX3;
			MaxY = RoiCPY+RoiSizeY3;

			if ( true == Result )
			{	WndRoiPtr->SetWndRoiRegionRes(MinX, MinY, MaxX, MaxY); }
			else
			{	WndRoiPtr->SetWndRoiRegion(MinX, MinY, MaxX, MaxY, false); }
		}
		break;
	}		
	return true;
}
//-------------------------------------------------------------------------------------//	
bool CAOIWnd::VisibleWnd()//啟用檢測框顯示狀態
{
	const bool Visibled = true;
	m_WndBox.SetBoxVisibled(Visibled);
	m_WndExtendBox.SetBoxVisibled(Visibled);				
	return true;
}
//-------------------------------------------------------------------------------------//		
bool CAOIWnd::UnSelectWnd()//取消檢測框選取狀態
{
	const bool Selected = false;	
	m_WndBox.SetBoxSelected(Selected);
	m_WndExtendBox.SetBoxSelected(Selected);			
	return true;
}
//--------------------------------------------------------------------------------------------//
bool CAOIWnd::InvisibleWnd()//取消檢測框顯示狀態
{
	const bool Visibled = false;
	m_WndBox.SetBoxVisibled(Visibled);
	m_WndExtendBox.SetBoxVisibled(Visibled);				
	return true;
}
//--------------------------------------------------------------------------------------------//
bool CAOIWnd::UnSelectWndRoi()//取消檢測框子框選取狀態
{
	size_t       i=0;
	CAOIWndRoi  *WndRoiPtr = NULL;
	const size_t RoiCount = GetWndRoiWndCount_Inline();
	for (i=0; i<RoiCount; i++ )
	{
		WndRoiPtr = GetWndRoiWndPtr_Inline(i);
		if ( NULL == WndRoiPtr ) { continue; }
		WndRoiPtr->SetWndRoiSelected(false);
		WndRoiPtr->SetWndRoiActived(false);
	}	
	return true;
}
//--------------------------------------------------------------------------------------------//
bool CAOIWnd::InvisibleWndRoi()//取消檢測子框框顯示狀態
{
	size_t       i=0;
	CAOIWndRoi  *WndRoiPtr = NULL;
	const size_t RoiCount = GetWndRoiWndCount_Inline();
	for (i=0; i<RoiCount; i++ )
	{
		WndRoiPtr = GetWndRoiWndPtr_Inline(i);
		if ( NULL == WndRoiPtr ) { continue; }
		WndRoiPtr->InvisibleWndRoi();
	}	
	return true;
}
//--------------------------------------------------------------------------------------------//
bool CAOIWnd::UnSelectWndMaskBox()//取消檢測框遮罩框選取狀態
{
	size_t       i=0;
	CAOIWndMask *MaskWndPtr = NULL;
	const size_t MaskWndCount = GetWndMaskWndCount_Inline();
	for (i=0; i<MaskWndCount; i++ )
	{
		MaskWndPtr = GetWndMaskWndPtr_Inline(i);
		if ( NULL == MaskWndPtr ) { continue; }
		MaskWndPtr->SetWndMaskSelected(false);
		MaskWndPtr->SetWndMaskActived(false);
	}	
	return true;
}
//--------------------------------------------------------------------------------------------//
bool CAOIWnd::InvisibleWndMaskBox()//取消檢測框遮罩框顯示狀態
{
	size_t       i=0;
	CAOIWndMask *MaskWndPtr = NULL;
	const size_t MaskWndCount = GetWndMaskWndCount_Inline();
	for (i=0; i<MaskWndCount; i++ )
	{
		MaskWndPtr = GetWndMaskWndPtr_Inline(i);
		if ( NULL == MaskWndPtr ) { continue; }
		MaskWndPtr->SetWndMaskVisibled(false);
	}	
	return true;
}
//--------------------------------------------------------------------------------------------//
void CAOIWnd::DrawWndBoxEdit(HDC hDC, TBOX_DRAW_PARAM &DrawParam) const
{	
	CAOILand    *LandPtr = GetWndLandPtr();
	WND_RGN_LINK_MODE WndRgnLinkMode = GetWndRgnLinkMode();
	if ( NULL == LandPtr )
	{
		switch ( WndRgnLinkMode )
		{
		case WND_RGN_LINK_NONE:
		case WND_RGN_LINK_BODY:
			DrawParam.ShapeMode = GetWndShapeMode();
			break;
		default:
			DrawParam.ShapeMode = BOX_SHAPE_RECTANGLE;
			break;
		}		
	}
	else
	{	DrawParam.ShapeMode = GetWndShapeMode();	}
	m_WndBox.DrawBoxEdit(hDC, DrawParam);
	DrawParam.ShapeMode = BOX_SHAPE_RECTANGLE;
	return;
}
//--------------------------------------------------------------------------------------------//
void CAOIWnd::DrawWndBoxResult(HDC hDC, TBOX_DRAW_PARAM &DrawParam) const
{	
	CAOILand    *LandPtr = GetWndLandPtr();
	WND_RGN_LINK_MODE WndRgnLinkMode = GetWndRgnLinkMode();
	if ( NULL == LandPtr )
	{
		switch ( WndRgnLinkMode )
		{
		case WND_RGN_LINK_NONE:
		case WND_RGN_LINK_BODY:
			DrawParam.ShapeMode = GetWndShapeMode();
			break;
		default:
			DrawParam.ShapeMode = BOX_SHAPE_RECTANGLE;
			break;
		}		
	}
	else
	{	DrawParam.ShapeMode = GetWndShapeMode();	}
	m_WndBox.DrawBoxResult(hDC, DrawParam);
	DrawParam.ShapeMode = BOX_SHAPE_RECTANGLE;
	return;
}
//--------------------------------------------------------------------------------------------//
void CAOIWnd::DrawWndExtBoxEdit(HDC hDC, TBOX_DRAW_PARAM &DrawParam) const
{
	CAOILand    *LandPtr = GetWndLandPtr();
	WND_RGN_LINK_MODE WndRgnLinkMode = GetWndRgnLinkMode();
	const bool DrawTowardFeature = DrawParam.DrawTowardFeature;
	DrawParam.DrawTowardFeature = false;
	//DrawParam.Toward = BOX_TOWARD_NULL;
	DrawParam.DrawEditLine = false;	
	DrawParam.Extend.x = DrawParam.Extend.y = 0;	

	if ( NULL == LandPtr )
	{
		switch ( WndRgnLinkMode )
		{
		case WND_RGN_LINK_NONE:
		case WND_RGN_LINK_BODY:
			DrawParam.ShapeMode = GetWndShapeMode();
			break;
		default:
			DrawParam.ShapeMode = BOX_SHAPE_RECTANGLE;
			break;
		}		
	}
	else
	{	DrawParam.ShapeMode = GetWndShapeMode();	}
	m_WndExtendBox.DrawBoxEdit(hDC, DrawParam);
	DrawParam.ShapeMode = BOX_SHAPE_RECTANGLE;
	DrawParam.DrawTowardFeature = DrawTowardFeature;
	return;
}
//--------------------------------------------------------------------------------------------//
void CAOIWnd::DrawWndExtBoxResult(HDC hDC, TBOX_DRAW_PARAM &DrawParam) const
{
	CAOILand    *LandPtr = GetWndLandPtr();
	WND_RGN_LINK_MODE WndRgnLinkMode = GetWndRgnLinkMode();
	const bool DrawTowardFeature = DrawParam.DrawTowardFeature;
	DrawParam.DrawTowardFeature = false;
	//DrawParam.Toward = BOX_TOWARD_NULL;
	DrawParam.DrawEditLine = false;	
	DrawParam.Extend.x = DrawParam.Extend.y = 0;

	if ( NULL == LandPtr )
	{
		switch ( WndRgnLinkMode )
		{
		case WND_RGN_LINK_NONE:
		case WND_RGN_LINK_BODY:
			DrawParam.ShapeMode = GetWndShapeMode();
			break;
		default:
			DrawParam.ShapeMode = BOX_SHAPE_RECTANGLE;
			break;
		}		
	}
	else
	{	DrawParam.ShapeMode = GetWndShapeMode();	}
	m_WndExtendBox.DrawBoxResult(hDC, DrawParam);
	DrawParam.ShapeMode = BOX_SHAPE_RECTANGLE;
	DrawParam.DrawTowardFeature = DrawTowardFeature;
	return;
}
//--------------------------------------------------------------------------------------------//
void CAOIWnd::ScaleWnd(double sx, double sy, bool bIncludeRes)//縮放檢測框
{
	m_WndBox.ScaleBox(sx, sy, bIncludeRes);	
	m_WndExtendBox.ScaleBox(sx, sy, bIncludeRes);	
	
	size_t       i=0;
	CAOIWndRoi  *WndRoiPtr = NULL;
	const size_t WndRoiCount = GetWndRoiWndCount_Inline();
	for ( i=0; i<WndRoiCount; i++ )
	{
		WndRoiPtr = GetWndRoiWndPtr_Inline(i);
		if ( NULL == WndRoiPtr ) { continue; }		
		WndRoiPtr->ScaleWndRoi(sx, sy, bIncludeRes);
	}

	CAOIWndMask *MaskWndPtr = NULL;
	const size_t MaskWndCount = GetWndMaskWndCount_Inline();
	for ( i=0; i<MaskWndCount; i++ )
	{
		MaskWndPtr = GetWndMaskWndPtr_Inline(i);
		if ( NULL == MaskWndPtr ) { continue; }		
		MaskWndPtr->ScaleWndMask(sx, sy, bIncludeRes);	
	}
	/*
	CAOIBox     *BoxPtr = NULL;
	const size_t BoxCount = GetWndFeatureBoxCount();
	for ( i=0; i<BoxCount; i++ )
	{
		BoxPtr = GetWndFeatureBoxPtr(i, false);
		if ( BoxPtr == NULL ) { continue; }
		BoxPtr->ScaleBox(sx, sy, bIncludeRes);	
	}*/

	CAOIBox  *BoxPtr = NULL;
	const size_t ResultBoxCount = GetWndResultBoxCount_Inline();
	for ( i=0; i<ResultBoxCount; i++ )
	{
		BoxPtr = GetWndResultBoxPtr_Inline(i);
		if ( NULL == BoxPtr ) { continue; }
		BoxPtr->ScaleBox(sx, sy, bIncludeRes);	
	}
	return ;
}
//--------------------------------------------------------------------------------------------//
void CAOIWnd::MoveWnd(double x, double y, bool bIncludeRes)
{
	m_WndBox.MoveBox(x, y, bIncludeRes);	
	m_WndExtendBox.MoveBox(x, y, bIncludeRes);	
	
	size_t       i=0;
	CAOIWndRoi  *WndRoiPtr = NULL;
	const size_t WndRoiCount = GetWndRoiWndCount_Inline();
	for ( i=0; i<WndRoiCount; i++ )
	{
		WndRoiPtr = GetWndRoiWndPtr_Inline(i);
		if ( NULL == WndRoiPtr ) { continue; }		
		WndRoiPtr->MoveWndRoi(x, y, bIncludeRes);
	}

	CAOIWndMask *MaskWndPtr = NULL;
	const size_t MaskWndCount = GetWndMaskWndCount_Inline();
	for ( i=0; i<MaskWndCount; i++ )
	{
		MaskWndPtr = GetWndMaskWndPtr_Inline(i);
		if ( NULL == MaskWndPtr ) { continue; }		
		MaskWndPtr->MoveWndMask(x, y, bIncludeRes);
	}
	/*
	CAOIBox     *BoxPtr = NULL;
	const size_t BoxCount = GetWndFeatureBoxCount();
	for ( i=0; i<BoxCount; i++ )
	{
		BoxPtr = GetWndFeatureBoxPtr(i, false);
		if ( BoxPtr == NULL ) { continue; }
		BoxPtr->MoveBox(x, y, bIncludeRes);
	}*/

	CAOIBox  *BoxPtr = NULL;
	const size_t ResultBoxCount = GetWndResultBoxCount_Inline();
	for ( i=0; i<ResultBoxCount; i++ )
	{
		BoxPtr = GetWndResultBoxPtr_Inline(i);
		if ( NULL == BoxPtr ) { continue; }
		BoxPtr->MoveBox(x, y, bIncludeRes);
	}
	return ;
}
//--------------------------------------------------------------------------------------------//
void CAOIWnd::MoveWnd(const TPOINT2D &Pos, bool bIncludeRes)
{
	m_WndBox.MoveBox(Pos, bIncludeRes);	
	m_WndExtendBox.MoveBox(Pos, bIncludeRes);	
	
	size_t       i=0;
	CAOIWndRoi  *WndRoiPtr = NULL;
	const size_t WndRoiCount = GetWndRoiWndCount_Inline();
	for ( i=0; i<WndRoiCount; i++ )
	{
		WndRoiPtr = GetWndRoiWndPtr_Inline(i);
		if ( NULL == WndRoiPtr ) { continue; }		
		WndRoiPtr->MoveWndRoi(Pos, bIncludeRes);
	}

	CAOIWndMask *MaskWndPtr = NULL;
	const size_t MaskWndCount = GetWndMaskWndCount_Inline();
	for ( i=0; i<MaskWndCount; i++ )
	{
		MaskWndPtr = GetWndMaskWndPtr_Inline(i);
		if ( NULL == MaskWndPtr ) { continue; }		
		MaskWndPtr->MoveWndMask(Pos, bIncludeRes);
	}
	/*
	CAOIBox     *BoxPtr = NULL;
	const size_t BoxCount = GetWndFeatureBoxCount();
	for ( i=0; i<BoxCount; i++ )
	{
		BoxPtr = GetWndFeatureBoxPtr(i, false);
		if ( BoxPtr == NULL ) { continue; }
		BoxPtr->MoveBox(Pos, bIncludeRes);
	}*/

	CAOIBox  *BoxPtr = NULL;
	const size_t ResultBoxCount = GetWndResultBoxCount_Inline();
	for ( i=0; i<ResultBoxCount; i++ )
	{
		BoxPtr = GetWndResultBoxPtr_Inline(i);
		if ( NULL == BoxPtr ) { continue; }
		BoxPtr->MoveBox(Pos, bIncludeRes);
	}
	return ;
}
//--------------------------------------------------------------------------------------------//
void CAOIWnd::MoveWndResult(double x, double y)
{
	m_WndBox.MoveBoxRes(x, y);		
	//m_WndBox.MoveBoxResStage(-x, -y);
	m_WndExtendBox.MoveBoxRes(x, y);	
	
	size_t       i=0;
	CAOIWndRoi  *WndRoiPtr = NULL;
	const size_t WndRoiCount = GetWndRoiWndCount_Inline();
	for ( i=0; i<WndRoiCount; i++ )
	{
		WndRoiPtr = GetWndRoiWndPtr_Inline(i);
		if ( NULL == WndRoiPtr ) { continue; }		
		WndRoiPtr->MoveWndRoiResult(x, y);
	}

	CAOIWndMask *MaskWndPtr = NULL;
	const size_t MaskWndCount = GetWndMaskWndCount_Inline();
	for ( i=0; i<MaskWndCount; i++ )
	{
		MaskWndPtr = GetWndMaskWndPtr_Inline(i);
		if ( NULL == MaskWndPtr ) { continue; }		
		MaskWndPtr->MoveWndMaskRes(x, y);
	}
	/*
	CAOIBox *BoxPtr = NULL;
	const size_t BoxCount = GetWndFeatureBoxCount();
	for ( i=0; i<BoxCount; i++ )
	{
		BoxPtr = GetWndFeatureBoxPtr(i, false);
		if ( BoxPtr == NULL ) { continue; }
		BoxPtr->MoveBoxRes(x, y);
	}*/

	CAOIBox  *BoxPtr = NULL;
	const size_t ResultBoxCount = GetWndResultBoxCount_Inline();
	for ( i=0; i<ResultBoxCount; i++ )
	{
		BoxPtr = GetWndResultBoxPtr_Inline(i);
		if ( NULL == BoxPtr ) { continue; }
		BoxPtr->MoveBoxRes(x, y);
	}
	return ;
}
//--------------------------------------------------------------------------------------------//
void CAOIWnd::MoveWndResult(const TPOINT2D &Pos)
{
	CAOIWnd::m_WndBox.MoveBoxRes(Pos);		
	//CAOIWnd::m_WndBox.MoveBoxResStage(-x, -y);
	CAOIWnd::m_WndExtendBox.MoveBoxRes(Pos);	
	
	size_t       i=0;	
	CAOIWndRoi  *WndRoiPtr = NULL;
	const size_t WndRoiCount = GetWndRoiWndCount_Inline();
	for ( i=0; i<WndRoiCount; i++ )
	{
		WndRoiPtr = GetWndRoiWndPtr_Inline(i);
		if ( NULL == WndRoiPtr ) { continue; }		
		WndRoiPtr->MoveWndRoiResult(Pos);
	}

	CAOIWndMask *MaskWndPtr = NULL;
	const size_t MaskWndCount = GetWndMaskWndCount_Inline();
	for ( i=0; i<MaskWndCount; i++ )
	{
		MaskWndPtr = GetWndMaskWndPtr_Inline(i);
		if ( NULL == MaskWndPtr ) { continue; }		
		MaskWndPtr->MoveWndMaskRes(Pos);
	}
	/*
	CAOIBox *BoxPtr = NULL;
	const size_t BoxCount = GetWndFeatureBoxCount();
	for ( i=0; i<BoxCount; i++ )
	{
		BoxPtr = GetWndFeatureBoxPtr(i, false);
		if ( BoxPtr == NULL ) { continue; }
		BoxPtr->MoveBoxRes(Pos);
	}*/

	CAOIBox  *BoxPtr = NULL;
	const size_t ResultBoxCount = GetWndResultBoxCount_Inline();
	for ( i=0; i<ResultBoxCount; i++ )
	{
		BoxPtr = GetWndResultBoxPtr_Inline(i);
		if ( NULL == BoxPtr ) { continue; }
		BoxPtr->MoveBoxRes(Pos);
	}
	return ;
}
//--------------------------------------------------------------------------------------------//
void CAOIWnd::SpinWnd(double Angle)
{
	double PosX=0, PosY=0;
	m_WndBox.GetBoxPos(PosX, PosY);
	m_WndBox.RotateBox(Angle, PosX, PosY);
	m_WndExtendBox.RotateBox(Angle, PosX, PosY);

	int    nTemp = 0;
	double dTemp = 0;
	const int AngleLable = JetAPI::GetAngleLabel(Angle);
	switch ( AngleLable )
	{
	case  90:
	case 270:
		dTemp = m_WndRgnLinkRatioX;
		m_WndRgnLinkRatioX	= m_WndRgnLinkRatioY;
		m_WndRgnLinkRatioY	= dTemp;

		dTemp = m_WndExtendRangeX;
		m_WndExtendRangeX	= m_WndExtendRangeY;
		m_WndExtendRangeY	= dTemp;

	//	dTemp = m_WndInnerMaskRatioX;
	//	m_WndInnerMaskRatioX     = m_WndInnerMaskRatioY;
	//	m_WndInnerMaskRatioY     = dTemp;
		break;
	default:
		break;
	}
	
	size_t       i=0;	
	CAOIWndRoi  *WndRoiPtr = NULL;
	const size_t WndRoiCount = GetWndRoiWndCount_Inline();
	for ( i=0; i<WndRoiCount; i++ )
	{
		WndRoiPtr = GetWndRoiWndPtr_Inline(i);
		if ( NULL == WndRoiPtr ) { continue; }		
		WndRoiPtr->RotateWndRoi(Angle, PosX, PosY);
	}

	CAOIWndMask *MaskWndPtr = NULL;
	const size_t MaskWndCount = GetWndMaskWndCount_Inline();
	for ( i=0; i<MaskWndCount; i++ )
	{
		MaskWndPtr = GetWndMaskWndPtr_Inline(i);
		if ( NULL == MaskWndPtr ) { continue; }		
		MaskWndPtr->RotateWndMask(Angle, PosX, PosY);
	}
	/*
	CAOIBox *BoxPtr = NULL;
	const size_t BoxCount = GetWndFeatureBoxCount();
	for ( i=0; i<BoxCount; i++ )
	{
		BoxPtr = GetWndFeatureBoxPtr(i, false);
		if ( BoxPtr == NULL ) { continue; }
		BoxPtr->RotateBox(Angle, PosX, PosY);
	}*/

	CAOIBox  *BoxPtr = NULL;
	const size_t ResultBoxCount = GetWndResultBoxCount_Inline();
	for ( i=0; i<ResultBoxCount; i++ )
	{
		BoxPtr = GetWndResultBoxPtr_Inline(i);
		if ( NULL == BoxPtr ) { continue; }
		BoxPtr->RotateBox(Angle, PosX, PosY);
	}

	m_WndAlgParam.RotateAlgParam(Angle);
	return ;
}
//--------------------------------------------------------------------------------------------//
void CAOIWnd::RotateWnd(double Angle, double CPX, double CPY)
{
	m_WndBox.RotateBox(Angle, CPX, CPY);
	m_WndExtendBox.RotateBox(Angle, CPX, CPY);

	int    nTemp = 0;
	double dTemp = 0;
	const int AngleLable = JetAPI::GetAngleLabel(Angle);
	switch ( AngleLable )
	{
	case  90:
	case 270:
		dTemp = m_WndRgnLinkRatioX;
		m_WndRgnLinkRatioX = m_WndRgnLinkRatioY;
		m_WndRgnLinkRatioY = dTemp;

		dTemp = m_WndExtendRangeX;
		m_WndExtendRangeX = m_WndExtendRangeY;
		m_WndExtendRangeY = dTemp;

	//	dTemp = CAOIWnd::m_WndInnerMaskRatioX;
	//	m_WndInnerMaskRatioX     = m_WndInnerMaskRatioY;
	//	m_WndInnerMaskRatioY     = dTemp;		
		break;
	default:		
		break;
	}
	
	size_t       i=0;	
	CAOIWndRoi  *WndRoiPtr = NULL;
	const size_t WndRoiCount = GetWndRoiWndCount_Inline();
	for ( i=0; i<WndRoiCount; i++ )
	{
		WndRoiPtr = GetWndRoiWndPtr_Inline(i);
		if ( NULL == WndRoiPtr ) { continue; }		
		WndRoiPtr->RotateWndRoi(Angle, CPX, CPY);
	}

	CAOIWndMask *MaskWndPtr = NULL;
	const size_t MaskWndCount = GetWndMaskWndCount_Inline();
	for ( i=0; i<MaskWndCount; i++ )
	{
		MaskWndPtr = GetWndMaskWndPtr_Inline(i);
		if ( NULL == MaskWndPtr ) { continue; }		
		MaskWndPtr->RotateWndMask(Angle, CPX, CPY);
	}
	/*
	CAOIBox *BoxPtr = NULL;
	const size_t BoxCount = GetWndFeatureBoxCount();
	for ( i=0; i<BoxCount; i++ )
	{
		BoxPtr = GetWndFeatureBoxPtr(i, false);
		if ( BoxPtr == NULL ) { continue; }
		BoxPtr->RotateBox(Angle, CPX, CPY);
	}*/

	CAOIBox  *BoxPtr = NULL;
	const size_t ResultBoxCount = GetWndResultBoxCount_Inline();
	for ( i=0; i<ResultBoxCount; i++ )
	{
		BoxPtr = GetWndResultBoxPtr_Inline(i);
		if ( NULL == BoxPtr ) { continue; }
		BoxPtr->RotateBox(Angle, CPX, CPY);
	}

	m_WndAlgParam.RotateAlgParam(Angle);
	return ;
}
//--------------------------------------------------------------------------------------------//
void CAOIWnd::MirrorWndXAxis(double CPY)
{	
	CAOIWnd::m_WndBox.MirrorBoxXAxis(CPY);	
	CAOIWnd::m_WndExtendBox.MirrorBoxXAxis(CPY);
	
	size_t       i=0;
	CAOIWndRoi  *WndRoiPtr = NULL;
	const size_t WndRoiCount = GetWndRoiWndCount_Inline();
	for ( i=0; i<WndRoiCount; i++ )
	{
		WndRoiPtr = GetWndRoiWndPtr_Inline(i);
		if ( NULL == WndRoiPtr ) { continue; }		
		WndRoiPtr->MirrorWndRoiXAxis(CPY);
	}

	CAOIWndMask *MaskWndPtr = NULL;
	const size_t MaskWndCount = GetWndMaskWndCount_Inline();
	for ( i=0; i<MaskWndCount; i++ )
	{
		MaskWndPtr = GetWndMaskWndPtr_Inline(i);
		if ( NULL == MaskWndPtr ) { continue; }		
		MaskWndPtr->MirrorWndMaskXAxis(CPY);
	}
	/*
	CAOIBox *BoxPtr = NULL;
	const size_t BoxCount = GetWndFeatureBoxCount();
	for ( i=0; i<BoxCount; i++ )
	{
		BoxPtr = GetWndFeatureBoxPtr(i, false);
		if ( BoxPtr == NULL ) { continue; }
		BoxPtr->MirrorBoxXAxis(CPY);
	}*/

	CAOIBox  *BoxPtr = NULL;
	const size_t ResultBoxCount = GetWndResultBoxCount_Inline();
	for ( i=0; i<ResultBoxCount; i++ )
	{
		BoxPtr = GetWndResultBoxPtr_Inline(i);
		if ( NULL == BoxPtr ) { continue; }
		BoxPtr->MirrorBoxXAxis(CPY);
	}

	m_WndAlgParam.MirrorAlgParamXAxis();
	return ;
}
//--------------------------------------------------------------------------------------------//
void CAOIWnd::MirrorWndYAxis(double CPX)
{	
	CAOIWnd::m_WndBox.MirrorBoxYAxis(CPX);	
	CAOIWnd::m_WndExtendBox.MirrorBoxYAxis(CPX);
	
	size_t       i=0;
	CAOIWndRoi  *WndRoiPtr = NULL;
	const size_t WndRoiCount = GetWndRoiWndCount_Inline();
	for ( i=0; i<WndRoiCount; i++ )
	{
		WndRoiPtr = GetWndRoiWndPtr_Inline(i);
		if ( NULL == WndRoiPtr ) { continue; }		
		WndRoiPtr->MirrorWndRoiYAxis(CPX);
	}

	CAOIWndMask *MaskWndPtr = NULL;
	const size_t MaskWndCount = GetWndMaskWndCount_Inline();
	for ( i=0; i<MaskWndCount; i++ )
	{
		MaskWndPtr = GetWndMaskWndPtr_Inline(i);
		if ( NULL == MaskWndPtr ) { continue; }		
		MaskWndPtr->MirrorWndMaskYAxis(CPX);
	}

	/*
	CAOIBox *BoxPtr = NULL;
	const size_t BoxCount = GetWndFeatureBoxCount();
	for ( i=0; i<BoxCount; i++ )
	{
		BoxPtr = GetWndFeatureBoxPtr(i, false);
		if ( BoxPtr == NULL ) { continue; }
		BoxPtr->MirrorBoxYAxis(CPX);
	}*/

	CAOIBox  *BoxPtr = NULL;
	const size_t ResultBoxCount = GetWndResultBoxCount_Inline();
	for ( i=0; i<ResultBoxCount; i++ )
	{
		BoxPtr = GetWndResultBoxPtr_Inline(i);
		if ( NULL == BoxPtr ) { continue; }
		BoxPtr->MirrorBoxYAxis(CPX);
	}
	m_WndAlgParam.MirrorAlgParamYAxis();
	return ;
}
//--------------------------------------------------------------------------------------------//
void CAOIWnd::SetWndAttachedAngle(double Angle)
{
	m_WndBox.SetBoxAttachedAngle(Angle);		
	m_WndExtendBox.SetBoxAttachedAngle(Angle);	
	
	size_t       i=0;
	CAOIWndRoi  *WndRoiPtr = NULL;
	const size_t WndRoiCount = GetWndRoiWndCount_Inline();
	for ( i=0; i<WndRoiCount; i++ )
	{
		WndRoiPtr = GetWndRoiWndPtr_Inline(i);
		if ( NULL == WndRoiPtr ) { continue; }		
		WndRoiPtr->SetWndRoiAttachedAngle(Angle);
	}

	CAOIWndMask *MaskWndPtr = NULL;
	const size_t MaskWndCount = GetWndMaskWndCount_Inline();
	for ( i=0; i<MaskWndCount; i++ )
	{
		MaskWndPtr = GetWndMaskWndPtr_Inline(i);
		if ( NULL == MaskWndPtr ) { continue; }		
		MaskWndPtr->SetWndMaskAttachedAngle(Angle);
	}

	/*
	CAOIBox *BoxPtr = NULL;
	const size_t BoxCount = GetWndFeatureBoxCount();
	for ( i=0; i<BoxCount; i++ )
	{
		BoxPtr = GetWndFeatureBoxPtr(i, false);
		if ( BoxPtr == NULL ) { continue; }
		BoxPtr->SetBoxAttachedAngle(Angle);
	}
	*/
	CAOIBox  *BoxPtr = NULL;
	const size_t ResultBoxCount = GetWndResultBoxCount_Inline();
	for ( i=0; i<ResultBoxCount; i++ )
	{
		BoxPtr = GetWndResultBoxPtr_Inline(i);
		if ( NULL == BoxPtr ) { continue; }
		BoxPtr->SetBoxAttachedAngle(Angle);
	}
}
//--------------------------------------------------------------------------------------------//
void CAOIWnd::SetWndAttachedPosCad(const TPOINT2D &Pos)
{
	CAOIWnd::m_WndBox.SetBoxAttachedPosCad(Pos);		
	CAOIWnd::m_WndExtendBox.SetBoxAttachedPosCad(Pos);	
	
	size_t       i=0;
	CAOIWndRoi  *WndRoiPtr = NULL;
	const size_t WndRoiCount = GetWndRoiWndCount_Inline();
	for ( i=0; i<WndRoiCount; i++ )
	{
		WndRoiPtr = GetWndRoiWndPtr_Inline(i);
		if ( NULL == WndRoiPtr ) { continue; }		
		WndRoiPtr->SetWndRoiAttachedPosCad(Pos);
	}

	CAOIWndMask *MaskWndPtr = NULL;
	const size_t MaskWndCount = GetWndMaskWndCount_Inline();
	for ( i=0; i<MaskWndCount; i++ )
	{
		MaskWndPtr = GetWndMaskWndPtr_Inline(i);
		if ( NULL == MaskWndPtr ) { continue; }		
		MaskWndPtr->SetWndMaskAttachedPosCad(Pos);
	}
	/*
	CAOIBox *BoxPtr = NULL;
	const size_t BoxCount = GetWndFeatureBoxCount();
	for ( i=0; i<BoxCount; i++ )
	{
		BoxPtr = GetWndFeatureBoxPtr(i, false);
		if ( BoxPtr == NULL ) { continue; }
		BoxPtr->SetBoxAttachedPosCad(Pos);
	}
	*/
	CAOIBox  *BoxPtr = NULL;
	const size_t ResultBoxCount = GetWndResultBoxCount_Inline();
	for ( i=0; i<ResultBoxCount; i++ )
	{
		BoxPtr = GetWndResultBoxPtr_Inline(i);
		if ( NULL == BoxPtr ) { continue; }
		BoxPtr->SetBoxAttachedPosCad(Pos);
	}
}
//--------------------------------------------------------------------------------------------//
void CAOIWnd::SetWndAttachedPosCad(double PosX, double PosY)
{
	CAOIWnd::m_WndBox.SetBoxAttachedPosCad(PosX, PosY);		
	CAOIWnd::m_WndExtendBox.SetBoxAttachedPosCad(PosX, PosY);	
	
	size_t       i=0;
	CAOIWndRoi  *WndRoiPtr = NULL;
	const size_t WndRoiCount = GetWndRoiWndCount_Inline();
	for ( i=0; i<WndRoiCount; i++ )
	{
		WndRoiPtr = GetWndRoiWndPtr_Inline(i);
		if ( NULL == WndRoiPtr ) { continue; }		
		WndRoiPtr->SetWndRoiAttachedPosCad(PosX, PosY);
	}

	CAOIWndMask *MaskWndPtr = NULL;
	const size_t MaskWndCount = GetWndMaskWndCount_Inline();
	for ( i=0; i<MaskWndCount; i++ )
	{
		MaskWndPtr = GetWndMaskWndPtr_Inline(i);
		if ( NULL == MaskWndPtr ) { continue; }		
		MaskWndPtr->SetWndMaskAttachedPosCad(PosX, PosY);
	}
	/*
	CAOIBox *BoxPtr = NULL;
	const size_t BoxCount = GetWndFeatureBoxCount();
	for ( i=0; i<BoxCount; i++ )
	{
		BoxPtr = GetWndFeatureBoxPtr(i, false);
		if ( BoxPtr == NULL ) { continue; }
		BoxPtr->SetBoxAttachedPosCad(PosX, PosY);
	}
	*/
	CAOIBox  *BoxPtr = NULL;
	const size_t ResultBoxCount = GetWndResultBoxCount_Inline();
	for ( i=0; i<ResultBoxCount; i++ )
	{
		BoxPtr = GetWndResultBoxPtr_Inline(i);
		if ( NULL == BoxPtr ) { continue; }
		BoxPtr->SetBoxAttachedPosCad(PosX, PosY);
	}
}
//--------------------------------------------------------------------------------------------//
void CAOIWnd::SetWndAttachedPosStage(const TPOINT2D &Pos)
{
	CAOIWnd::m_WndBox.SetBoxAttachedPosStage(Pos);		
	CAOIWnd::m_WndExtendBox.SetBoxAttachedPosStage(Pos);	
	
	size_t       i=0;
	CAOIWndRoi  *WndRoiPtr = NULL;
	const size_t WndRoiCount = GetWndRoiWndCount_Inline();
	for ( i=0; i<WndRoiCount; i++ )
	{
		WndRoiPtr = GetWndRoiWndPtr_Inline(i);
		if ( NULL == WndRoiPtr ) { continue; }		
		WndRoiPtr->SetWndRoiAttachedPosStage(Pos);
	}

	CAOIWndMask *MaskWndPtr = NULL;
	const size_t MaskWndCount = GetWndMaskWndCount_Inline();
	for ( i=0; i<MaskWndCount; i++ )
	{
		MaskWndPtr = GetWndMaskWndPtr_Inline(i);
		if ( NULL == MaskWndPtr ) { continue; }		
		MaskWndPtr->SetWndMaskAttachedPosStage(Pos);
	}

	/*
	CAOIBox *BoxPtr = NULL;
	const size_t BoxCount = GetWndFeatureBoxCount();
	for ( i=0; i<BoxCount; i++ )
	{
		BoxPtr = GetWndFeatureBoxPtr(i, false);
		if ( BoxPtr == NULL ) { continue; }
		BoxPtr->SetBoxAttachedPosStage(Pos);
	}
	*/
	CAOIBox  *BoxPtr = NULL;
	const size_t ResultBoxCount = GetWndResultBoxCount_Inline();
	for ( i=0; i<ResultBoxCount; i++ )
	{
		BoxPtr = GetWndResultBoxPtr_Inline(i);
		if ( NULL == BoxPtr ) { continue; }
		BoxPtr->SetBoxAttachedPosStage(Pos);
	}
}
//--------------------------------------------------------------------------------------------//
void CAOIWnd::SetWndAttachedPosStage(double PosX, double PosY)
{
	CAOIWnd::m_WndBox.SetBoxAttachedPosStage(PosX, PosY);
	CAOIWnd::m_WndExtendBox.SetBoxAttachedPosStage(PosX, PosY);	
	
	size_t       i=0;
	CAOIWndRoi  *WndRoiPtr = NULL;
	const size_t WndRoiCount = GetWndRoiWndCount_Inline();
	for ( i=0; i<WndRoiCount; i++ )
	{
		WndRoiPtr = GetWndRoiWndPtr_Inline(i);
		if ( NULL == WndRoiPtr ) { continue; }		
		WndRoiPtr->SetWndRoiAttachedPosStage(PosX, PosY);
	}

	CAOIWndMask *MaskWndPtr = NULL;
	const size_t MaskWndCount = GetWndMaskWndCount_Inline();
	for ( i=0; i<MaskWndCount; i++ )
	{
		MaskWndPtr = GetWndMaskWndPtr_Inline(i);
		if ( NULL == MaskWndPtr ) { continue; }		
		MaskWndPtr->SetWndMaskAttachedPosStage(PosX, PosY);
	}

	/*
	CAOIBox *BoxPtr = NULL;
	const size_t BoxCount = GetWndFeatureBoxCount();
	for ( i=0; i<BoxCount; i++ )
	{
		BoxPtr = GetWndFeatureBoxPtr(i, false);
		if ( BoxPtr == NULL ) { continue; }
		BoxPtr->SetBoxAttachedPosStage(PosX, PosY);
	}
	*/
	CAOIBox  *BoxPtr = NULL;
	const size_t ResultBoxCount = GetWndResultBoxCount_Inline();
	for ( i=0; i<ResultBoxCount; i++ )
	{
		BoxPtr = GetWndResultBoxPtr_Inline(i);
		if ( NULL == BoxPtr ) { continue; }
		BoxPtr->SetBoxAttachedPosStage(PosX, PosY);
	}
}
//--------------------------------------------------------------------------------------------//
size_t CAOIWnd::GetWndRoiWndCount() const
{
	return GetWndRoiWndCount_Inline();
}
//--------------------------------------------------------------------------------------------//
CAOIWndRoi*  CAOIWnd::GetWndRoiWndActived()
{
	size_t       i=0;
	CAOIWndRoi  *WndRoiPtr = NULL;
	const size_t RoiCount = GetWndRoiWndCount_Inline();
	for (i=0; i<RoiCount; i++ )
	{
		WndRoiPtr = GetWndRoiWndPtr_Inline(i);
		if ( NULL == WndRoiPtr ) { continue; }
		if ( WndRoiPtr->GetWndRoiActived() == false ) { continue; }
		return WndRoiPtr; 
	}
	return NULL;
}
//--------------------------------------------------------------------------------------------//
void CAOIWnd::SetWndRoiWndActived(CAOIWndRoi *RefWndRoiPtr)
{
	size_t       i=0;
	CAOIWndRoi  *WndRoiPtr = NULL;
	const size_t RoiCount = GetWndRoiWndCount_Inline();
	for (i=0; i<RoiCount; i++ )
	{
		WndRoiPtr = GetWndRoiWndPtr_Inline(i);
		if ( NULL == WndRoiPtr ) { continue; }
		WndRoiPtr->SetWndRoiActived(false);		
	}
	if ( NULL != RefWndRoiPtr )
	{
		RefWndRoiPtr->SetWndRoiActived(true);
		RefWndRoiPtr->SetWndRoiSelected(true);
	}
	return ;
}
//--------------------------------------------------------------------------------------------//
CAOIWndRoi* CAOIWnd::GetWndRoiWndSelected()
{
	size_t       i=0;
	CAOIWndRoi  *WndRoiPtr = NULL;
	const size_t RoiCount = GetWndRoiWndCount_Inline();
	for (i=0; i<RoiCount; i++ )
	{
		WndRoiPtr = GetWndRoiWndPtr_Inline(i);
		if ( NULL == WndRoiPtr ) { continue; }
		if ( WndRoiPtr->GetWndRoiSelected() == false ) { continue; }
		return WndRoiPtr; 
	}
	return NULL;
}
//--------------------------------------------------------------------------------------------//
bool CAOIWnd::AddWndRoiWndPtr(CAOIWndRoi *WndRoiPtr, bool Clone)
{
	if ( NULL == WndRoiPtr ) { return false; }

	CAOIWndRoi *WndRoiPtrNew = WndRoiPtr;
	if ( true == Clone )
	{
		WndRoiPtrNew = WndRoiPtr->CloneWndRoiObj();
		if ( NULL == WndRoiPtrNew ) { return false; }
	}
	ALG_TYPE    AlgType;
	BOX_TOWARD  Toward;
	double    AttachedAngle=0;
	TPOINT2D  AttachedPosCad;
	TPOINT2D  AttachedPosStage;
	
	Toward = GetWndToward();
	AlgType = GetWndAlgType();
	AttachedAngle = GetWndAttachedAngle();
	GetWndAttachedPosCad(AttachedPosCad);
	GetWndAttachedPosStage(AttachedPosStage);

	unsigned int index = (unsigned int)(GetWndRoiWndCount_Inline());
	WndRoiPtrNew->SetWndRoiAlgType(AlgType);
	WndRoiPtrNew->SetWndRoiToward(Toward);
	WndRoiPtrNew->SetWndRoiWndPtr(this);
	WndRoiPtrNew->SetWndRoiIndex(index);
	WndRoiPtrNew->SetWndRoiAttachedAngle(AttachedAngle);
	WndRoiPtrNew->SetWndRoiAttachedPosCad(AttachedPosCad);
	WndRoiPtrNew->SetWndRoiAttachedPosStage(AttachedPosStage);
	AddWndRoiWnd_Inline(WndRoiPtrNew);
	return true;
}
//--------------------------------------------------------------------------------------------//
CAOIWndRoi* CAOIWnd::GetWndRoiWndPtr(size_t index, bool Check)
{
	if ( true == Check )
	{
		const size_t Count = GetWndRoiWndCount_Inline();
		if ( index >= Count ) { return NULL; }
	}
	return GetWndRoiWndPtr_Inline(index);
}
//--------------------------------------------------------------------------------------------//
void CAOIWnd::ClearWndRoiWndList()
{
	size_t       i=0;
	CAOIWndRoi  *WndRoiPtr = NULL;
	const size_t RoiCount = GetWndRoiWndCount_Inline();

	for (i=0; i<RoiCount; i++ )
	{
		WndRoiPtr = GetWndRoiWndPtr_Inline(i);
		if ( NULL == WndRoiPtr ) { continue; }
		AOIObjManager.DestroyWndRoiObj(WndRoiPtr);
	}
	m_WndRoiWndList.clear();
}
//--------------------------------------------------------------------------------------------//
bool CAOIWnd::BuildWndRoiWndListDefault(int DefaultCount)
{
	TREGION4D WndRgn;
	bool      IsOK = true;	
	int       RoiCntX = 0;
	int       RoiCntY = 0;
	double    RgnSizeX=0;
	double    RgnSizeY=0;
	double    MarginXRatio = 10;
	double    MarginYRatio = 10;
	
	GetWndRegion(WndRgn);
	RgnSizeX = WndRgn.GetWidth();
	RgnSizeY = WndRgn.GetHeight();

	if ( 0 == DefaultCount )
	{	return true; }
	
	if ( RgnSizeX > RgnSizeY )
	{	
		RoiCntX = DefaultCount;
		RoiCntY = 1;
	}
	else
	{
		RoiCntX = 1;
		RoiCntY = DefaultCount;
	}		
	IsOK = BuildWndRoiWndList(RoiCntX, RoiCntY, MarginXRatio, MarginYRatio);
	return IsOK;
}
//--------------------------------------------------------------------------------------------//
bool CAOIWnd::BuildWndRoiWndList(int RoiCntX, int RoiCntY, double MarginXRatio, double MarginYRatio)
{
	bool IsOK = true;
	const double AttachedAngle = CAOIWnd::GetWndAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(AttachedAngle);
	if ( IsExceptionAngle == false )
	{	BuildWndRoiWndListKernel(RoiCntX, RoiCntY, MarginXRatio, MarginYRatio);	}
	else
	{
		RotateWnd(-AttachedAngle, 0, 0);
		BuildWndRoiWndListKernel(RoiCntX, RoiCntY, MarginXRatio, MarginYRatio);
		RotateWnd(AttachedAngle, 0, 0);
	}
	return IsOK;
}
//--------------------------------------------------------------------------------------------//
bool CAOIWnd::UpdateWndParamRoiDefault(int DefaultCount)
{
	ALG_TYPE AlgType = GetWndAlgType();
	if (ALG_MEASURE_SIP_DISTANCE != AlgType) { return true; }

	CAlgParam &AlgParam = GetWndAlgParam();
	TALG_PARAM_MEASURE_SIP &msParam = AlgParam.GetAlgParamMeasureSIP();
	switch (DefaultCount)
	{
	case(2):
		msParam.msInspecEdgeCount = 1;
		msParam.msRefEdgeCount = 0;
	case(3):
		msParam.msInspecEdgeCount = 1;
		msParam.msRefEdgeCount = 1;
		break;
	case(4):
		msParam.msInspecEdgeCount = 2;
		msParam.msRefEdgeCount = 1;
		break;
	case(5):
		msParam.msInspecEdgeCount = 2;
		msParam.msRefEdgeCount = 2;
		break;
	default:
		return false;
	}
	return true;
}
//--------------------------------------------------------------------------------------------//
void CAOIWnd::DestroyWndRoiWndSelected()
{
	size_t       i=0;
	unsigned int index=0;
	CAOIWndRoi  *WndRoiPtr = NULL;
	std::vector<CAOIWndRoi*>  TmpWndRoiWndList=m_WndRoiWndList;
	const size_t RoiCount = TmpWndRoiWndList.size();

	index = 0;
	m_WndRoiWndList.clear();
	for (i=0; i<RoiCount; i++ )
	{
		WndRoiPtr = TmpWndRoiWndList[i];
		if ( NULL == WndRoiPtr ) { continue; }
		if ( WndRoiPtr->GetWndRoiSelected() == false )
		{
			WndRoiPtr->SetWndRoiIndex(index);
			AddWndRoiWnd_Inline(WndRoiPtr);
			index ++;
			continue;
		}
		AOIObjManager.DestroyWndRoiObj(WndRoiPtr);
	}	
}
//--------------------------------------------------------------------------------------------//
void CAOIWnd::SetWndAllRoiWndActived(bool value)
{
	size_t       i=0;
	CAOIWndRoi  *WndRoiPtr = NULL;
	const size_t RoiCount = GetWndRoiWndCount_Inline();
	for (i=0; i<RoiCount; i++ )
	{
		WndRoiPtr = GetWndRoiWndPtr_Inline(i);
		if ( NULL == WndRoiPtr ) { continue; }
		WndRoiPtr->SetWndRoiActived(value);
	}
	return;
}
//--------------------------------------------------------------------------------------------//
void CAOIWnd::SetWndAllRoiWndSelected(bool value)
{
	size_t       i=0;
	CAOIWndRoi  *WndRoiPtr = NULL;
	const size_t RoiCount = GetWndRoiWndCount_Inline();
	for (i=0; i<RoiCount; i++ )
	{
		WndRoiPtr = GetWndRoiWndPtr_Inline(i);
		if ( NULL == WndRoiPtr ) { continue; }
		WndRoiPtr->SetWndRoiSelected(value);
	}
	return;
}
//--------------------------------------------------------------------------------------------//
bool CAOIWnd::CalcWndRoiWndRegion(TREGION4D &Region)//計算檢測框子框區域
{	
	size_t       i=0;
	bool         First=true;
	TREGION4D    RoiRgn;
	CAOIWndRoi  *WndRoiPtr = NULL;
	const size_t RoiCount = GetWndRoiWndCount_Inline();
	for (i=0; i<RoiCount; i++ )
	{
		WndRoiPtr = GetWndRoiWndPtr_Inline(i);
		if ( NULL == WndRoiPtr ) { continue; }
		WndRoiPtr->GetWndRoiRegion(RoiRgn);
		if ( true == First )
		{
			Region = RoiRgn;
			First = false;
		}
		else
		{	JetAPI::UnionRegion(RoiRgn, Region, Region);	}
	}
	return true;
}
//--------------------------------------------------------------------------------------------//
bool CAOIWnd::GetWndRoiWndSelectedList(std::vector<CAOIWndRoi*> &WndRoiList)
{
	size_t       i=0;		
	CAOIWndRoi  *WndRoiPtr = NULL;
	const size_t RoiCount = GetWndRoiWndCount_Inline();
	for (i=0; i<RoiCount; i++ )
	{
		WndRoiPtr = GetWndRoiWndPtr_Inline(i);
		if ( NULL == WndRoiPtr ) { continue; }
		if ( WndRoiPtr->GetWndRoiSelected() == false ) { continue; }
		WndRoiList.push_back(WndRoiPtr);
	}
	return true;
}
//--------------------------------------------------------------------------------------------//
void CAOIWnd::ClearWndResultBoxList()
{
	ClearWndResultBoxList_Inline();
}
//--------------------------------------------------------------------------------------------//
size_t CAOIWnd::GetWndResultBoxCount() const
{
	return CAOIWnd::GetWndResultBoxCount_Inline();
}
//--------------------------------------------------------------------------------------------//
CAOIBox* CAOIWnd::GetWndResultBoxPtr(size_t index, bool Check)
{
	if ( Check )
	{
		const size_t size = CAOIWnd::GetWndResultBoxCount_Inline();
		if ( index >= size )
		{	return NULL; }
	}
	return CAOIWnd::GetWndResultBoxPtr_Inline(index);
}
//--------------------------------------------------------------------------------------------//
bool CAOIWnd::AddWndResultBox(const CAOIBox &Box)
{	
	CAOIBox ResBox=Box;	
	ResBox.SetBoxToward(GetWndToward());
	ResBox.SetBoxAttachedAngle(GetWndAttachedAngle());
	ResBox.SetBoxAttachedPosCad(GetWndAttachedPosCad());
	ResBox.SetBoxAttachedPosStage(GetWndAttachedPosStage());
	CAOIWnd::AddWndResultBox_Inline(ResBox);	
	return true;
}
//--------------------------------------------------------------------------------------------//
void CAOIWnd::CloneWndResultBoxList(CAOIWnd *WndPtr)//複製檢測框結果框列表
{
	if ( NULL == WndPtr ) { return; }
	CAOIBox *ResBoxPtr=NULL;
	const size_t ResBoxCount=WndPtr->GetWndResultBoxCount();

	ClearWndResultBoxList_Inline();
	for ( size_t i=0; i<ResBoxCount; i++ )
	{
		ResBoxPtr = WndPtr->GetWndResultBoxPtr(i, false);
		if ( NULL == ResBoxPtr ) { continue; }
		AddWndResultBox(*ResBoxPtr);
	}
	return;
}
//--------------------------------------------------------------------------------------------//
int CAOIWnd::GetWndModelMaskFlag() const//取得模組遮罩旗標
{
	return m_WndModelMaskFlag;
}
//--------------------------------------------------------------------------------------------//
void CAOIWnd::SetWndModelMaskFlag(int value)//設定模組遮罩旗標
{
	m_WndModelMaskFlag = value;
}
//--------------------------------------------------------------------------------------------//
void CAOIWnd::AddWndModelMaskFlag(int value)//加入模組遮罩旗標
{
	m_WndModelMaskFlag = JetAPI::BitMask_Add(m_WndModelMaskFlag, value);	
}
//--------------------------------------------------------------------------------------------//
void CAOIWnd::RemoveWndModelMaskFlag(int value)//加入模組遮罩旗標
{
	m_WndModelMaskFlag = JetAPI::BitMask_Remove(m_WndModelMaskFlag, value);		
}
//--------------------------------------------------------------------------------------------//
bool CAOIWnd::CheckWndAlgUsedMaskWnd() const//確認檢測框演算法是否可以使用遮罩框
{
	ALG_TYPE AlgType = GetWndAlgType();
	return CheckWndAlgUsedMaskWnd(AlgType);	
}
//--------------------------------------------------------------------------------------------//
bool CAOIWnd::CheckWndAlgUsedMaskWnd(ALG_TYPE AlgType) const//確認檢測框演算法是否可以使用遮罩框
{
	bool bUsed=false;	
	switch ( AlgType )
	{
	case ALG_BRIGHT_RATIO:
	case ALG_BLOB_COUNT:
	case ALG_MODEL_MATCH:
	case ALG_SHAPE_VERIFY:	
	//case ALG_SOLDER_WETTING:
		bUsed = true;
		break;
	}	
	return bUsed;
}
//--------------------------------------------------------------------------------------------//
void CAOIWnd::ClearWndMaskWndList()
{
	size_t       i=0;
	CAOIWndMask *MaskWndPtr = NULL;
	const size_t MaskWndCount = GetWndMaskWndCount_Inline();
	for ( i=0; i<MaskWndCount; i++ )
	{
		MaskWndPtr = GetWndMaskWndPtr_Inline(i);
		if ( NULL == MaskWndPtr ) { continue; }
		AOIObjManager.DestroyWndMaskObj(MaskWndPtr);		
	}
	m_WndMaskWndList.clear();
}
//--------------------------------------------------------------------------------------------//
CAOIWndMask* CAOIWnd::GetWndMaskWndActived()
{
	size_t       i=0;
	CAOIWndMask *MaskWndPtr = NULL;
	const size_t MaskWndCount = GetWndMaskWndCount_Inline();
	for (i=0; i<MaskWndCount; i++ )
	{
		MaskWndPtr = GetWndMaskWndPtr_Inline(i);
		if ( NULL == MaskWndPtr ) { continue; }
		if ( MaskWndPtr->GetWndMaskActived() == false ) { continue; }
		return MaskWndPtr; 
	}
	return NULL;
}
//--------------------------------------------------------------------------------------------//
void CAOIWnd::SetWndMaskWndActived(CAOIWndMask *ActMaskWndPtr)
{
	size_t       i=0;
	CAOIWndMask *MaskWndPtr = NULL;
	const size_t MaskWndCount = GetWndMaskWndCount_Inline();
	for (i=0; i<MaskWndCount; i++ )
	{
		MaskWndPtr = GetWndMaskWndPtr_Inline(i);
		if ( NULL == MaskWndPtr ) { continue; }
		MaskWndPtr->SetWndMaskActived(false);
	}
	if ( NULL != ActMaskWndPtr )
	{
		ActMaskWndPtr->SetWndMaskActived(true);
		ActMaskWndPtr->SetWndMaskSelected(true);
	}
	return ;
}
//--------------------------------------------------------------------------------------------//
CAOIWndMask* CAOIWnd::GetWndMaskWndSelected()
{
	size_t       i=0;
	CAOIWndMask *MaskWndPtr = NULL;
	const size_t MaskWndCount = GetWndMaskWndCount_Inline();
	for (i=0; i<MaskWndCount; i++ )
	{
		MaskWndPtr = GetWndMaskWndPtr_Inline(i);
		if ( NULL == MaskWndPtr ) { continue; }
		if ( MaskWndPtr->GetWndMaskSelected() == false ) { continue; }
		return MaskWndPtr; 
	}
	return NULL;
}
//--------------------------------------------------------------------------------------------//
size_t CAOIWnd::GetWndMaskWndCount() const
{
	return CAOIWnd::GetWndMaskWndCount_Inline();
}
//--------------------------------------------------------------------------------------------//
CAOIWndMask* CAOIWnd::GetWndMaskWndPtr(size_t index, bool Check)
{
	if ( true==Check )
	{
		const size_t size = CAOIWnd::GetWndMaskWndCount_Inline();
		if ( index >= size )
		{	return NULL; }
	}
	return GetWndMaskWndPtr_Inline(index);
}
//--------------------------------------------------------------------------------------------//
bool CAOIWnd::CheckWndMaskValid(const CAOIWndMask *RefMaskWndPtr)
{
	if ( NULL == RefMaskWndPtr ) { return false; }
	const size_t WndMaskCount = GetWndMaskWndCount();
	for ( size_t i=0; i<WndMaskCount; i++ )
	{
		CAOIWndMask *MaskWndPtr = GetWndMaskWndPtr(i, false);
		if ( RefMaskWndPtr != MaskWndPtr ) { continue; }
		return true;
	}
	return false;
}
//--------------------------------------------------------------------------------------------//
bool CAOIWnd::AddWndMaskWndPtr(CAOIWndMask *MaskWndPtr, bool Clone)
{	
	if ( NULL == MaskWndPtr ) { return false; }
	CAOIWndMask *NewMaskWndPtr=MaskWndPtr;	
	if ( true == Clone )
	{
		NewMaskWndPtr = MaskWndPtr->CloneWndMaskObj();
		if ( NULL == NewMaskWndPtr ) { return false; }
	}
	unsigned int index = (unsigned int)(GetWndMaskWndCount_Inline());
	NewMaskWndPtr->SetWndMaskIndex(index);
	//NewMaskWndPtr->SetWndMaskToward(GetWndToward());//由於有各自的朝向, 所以不能在此設定
	NewMaskWndPtr->SetWndMaskAttachedAngle(GetWndAttachedAngle());
	NewMaskWndPtr->SetWndMaskAttachedPosCad(GetWndAttachedPosCad());
	NewMaskWndPtr->SetWndMaskAttachedPosStage(GetWndAttachedPosStage());
	AddWndMaskWnd_Inline(NewMaskWndPtr);	
	return true;
}
//--------------------------------------------------------------------------------------------//
void CAOIWnd::DestroyWndMaskWndSelected()
{
	size_t       i=0;
	unsigned int index=0;
	CAOIWndMask *MaskWndPtr = NULL;
	std::vector<CAOIWndMask*>  TmpWndMaskWndList=m_WndMaskWndList;
	const size_t WndCount = TmpWndMaskWndList.size();

	index = 0;
	m_WndMaskWndList.clear();
	for (i=0; i<WndCount; i++ )
	{
		MaskWndPtr = TmpWndMaskWndList[i];
		if ( NULL == MaskWndPtr ) { continue; }
		if ( MaskWndPtr->GetWndMaskSelected() == false )
		{	
			MaskWndPtr->SetWndMaskIndex(index);
			AddWndMaskWnd_Inline(MaskWndPtr);
			index ++;
			continue;
		}
		AOIObjManager.DestroyWndMaskObj(MaskWndPtr);
	}
	return ;
}
//--------------------------------------------------------------------------------------------//
void CAOIWnd::SetWndAllMaskWndSelected(bool value)
{
	size_t       i=0;
	CAOIWndMask *MaskWndPtr = NULL;
	const size_t BoxCount = GetWndMaskWndCount_Inline();
	for (i=0; i<BoxCount; i++ )
	{
		MaskWndPtr = GetWndMaskWndPtr_Inline(i);
		if ( NULL == MaskWndPtr ) { continue; }
		MaskWndPtr->SetWndMaskSelected(value);
	}
	return;
}
//--------------------------------------------------------------------------------------------//
bool CAOIWnd::CalcWndMaskWndRegion(TREGION4D &Region)//計算遮罩框區域	
{
	size_t       i=0;
	bool         First=true;
	TREGION4D    RoiRgn;
	CAOIWndMask *MaskWndPtr = NULL;
	const size_t BoxCount = GetWndMaskWndCount_Inline();
	for (i=0; i<BoxCount; i++ )
	{
		MaskWndPtr = GetWndMaskWndPtr_Inline(i);
		if ( NULL == MaskWndPtr ) { continue; }
		MaskWndPtr->GetWndMaskRegion(RoiRgn);
		if ( true == First )
		{
			Region = RoiRgn;
			First = false;
		}
		else
		{	JetAPI::UnionRegion(RoiRgn, Region, Region);	}
	}
	return true;
}
//--------------------------------------------------------------------------------------------//
bool CAOIWnd::GetWndMaskWndSelectedList(std::vector<size_t> &List)
{	
	const size_t MaskWndCount = GetWndMaskWndCount();
	for ( size_t i=0; i<MaskWndCount; i++ )
	{
		CAOIWndMask *MaskWndPtr = GetWndMaskWndPtr(i, false);
		if ( NULL == MaskWndPtr ) { continue; }
		if ( MaskWndPtr->GetWndMaskSelected() == false ) { continue; }
		List.push_back(i);
	}
	return true;
}
//--------------------------------------------------------------------------------------------//
bool CAOIWnd::GetWndMaskWndSelectedList(std::vector<CAOIWndMask*> &MaskWndList)
{
	size_t       i=0;	
	CAOIWndMask *MaskWndPtr = NULL;
	const size_t BoxCount = GetWndMaskWndCount_Inline();
	for (i=0; i<BoxCount; i++ )
	{
		MaskWndPtr = GetWndMaskWndPtr_Inline(i);
		if ( NULL == MaskWndPtr ) { continue; }
		if ( MaskWndPtr->GetWndMaskSelected() == false ) { continue; }
		MaskWndList.push_back(MaskWndPtr);
	}
	return true;
}
//--------------------------------------------------------------------------------------------//
CString CAOIWnd::GetWndAlgTypeText() const
{
	CString str;
	ALG_TYPE AlgType = CAOIWnd::m_WndAlgParam.GetAlgType();
	str = AOIDataDefine.GetAlgTypeText(AlgType);
	return str;
}
//--------------------------------------------------------------------------------------------//
void CAOIWnd::SetWndAlgImageFrameIndex(unsigned int val)
{
	size_t        i=0;
	CAOIWndRoi   *WndRoiPtr = NULL;
	const size_t  WndRoiCount = GetWndRoiWndCount_Inline();

	m_WndAlgParam.GetAlgImageBinParamPtr()->SetBinaryFrameIndex(val);
	for ( i=0; i<WndRoiCount; i++ )
	{
		WndRoiPtr = GetWndRoiWndPtr_Inline(i);
		if ( NULL == WndRoiPtr ) { continue; }
		if ( WndRoiPtr->GetWndRoiSelfFrameEnabled() == true ) { continue; }
		WndRoiPtr->GetWndRoiBinaryParam().SetBinaryFrameIndex(val);
	}
}
//--------------------------------------------------------------------------------------------//
void CAOIWnd::SetWndAlgImageFrameUniqueID(unsigned int val)
{
	size_t        i=0;
	CAOIWndRoi   *WndRoiPtr = NULL;
	const size_t  WndRoiCount = GetWndRoiWndCount_Inline();

	m_WndAlgParam.GetAlgImageBinParamPtr()->SetBinaryFrameUniqueID(val);
	for ( i=0; i<WndRoiCount; i++ )
	{
		WndRoiPtr = GetWndRoiWndPtr_Inline(i);
		if ( NULL == WndRoiPtr ) { continue; }
		if ( WndRoiPtr->GetWndRoiSelfFrameEnabled() == true ) { continue; }
		WndRoiPtr->GetWndRoiBinaryParam().SetBinaryFrameUniqueID(val);
	}
}
//--------------------------------------------------------------------------------------------//
bool CAOIWnd::CheckWndAlgUsing3D() const//確認檢測框演算法使用3D
{
	return GetWndAlgParam().CheckAlgParamUsing3D();
}
//--------------------------------------------------------------------------------------------//
bool CAOIWnd::CheckWndAlgCanUsing3D() const//確認檢測框演算法可以使用3D畫面進行檢測
{
	bool bIsOK = true;
	ALG_TYPE AlgType = GetWndAlgType();
	switch (AlgType)
	{
	case ALG_IMAGE_MATCH:
	case ALG_MEASURE_SIP_DISTANCE:
		bIsOK = false;
		break;
	default:
		break;
	}
	return bIsOK;
}
//--------------------------------------------------------------------------------------------//
bool CAOIWnd::InitWndInspection(bool bModelInit)//初始化檢測框檢測
{
	RESULT_ID ResultID = RESULT_ID_NONE;
	const bool bEnabled = GetWndEnabled();
	const unsigned int FrameIndex = m_WndAlgParam.GetAlgImageBinParamPtr()->GetBinaryFrameIndex();
	const unsigned int FrameUniqueID = m_WndAlgParam.GetAlgImageBinParamPtr()->GetBinaryFrameUniqueID();	
	
	if ( false == bEnabled ) 
	{	ResultID = RESULT_ID_BYPASS; }
	m_WndBypassed = false;
	m_WndUIUpated_Param = false;
	m_WndInspectedTime = 0.0;	
	m_WndRectCalValue.x = m_WndRectCalValue.y = 0;
	m_WndLogicResultID = ResultID;	

	m_WndBox.SetBoxResultValue(0);
	m_WndBox.SetBoxResultText(_T(""));
	m_WndBox.SetBoxResultID(ResultID);	
	m_WndBox.ResetBoxRegionRes();

	if ( true == m_WndExtendBoxUsed )
	{	m_WndExtendBox.ResetBoxRegionRes(); }
	
	size_t       i = 0;	
	CAOIWndRoi  *WndRoiPtr = NULL;
	const size_t RoiWndCount = GetWndRoiWndCount_Inline();
	for ( i=0; i<RoiWndCount; i++ )
	{
		WndRoiPtr = GetWndRoiWndPtr_Inline(i);
		if ( NULL == WndRoiPtr ) { continue; }
		if ( WndRoiPtr->GetWndRoiSelfFrameEnabled() == false )
		{
			WndRoiPtr->GetWndRoiBinaryParam().SetBinaryFrameIndex(FrameIndex);
			WndRoiPtr->GetWndRoiBinaryParam().SetBinaryFrameUniqueID(FrameUniqueID);
		}
		WndRoiPtr->SetWndRoiActived(false);
		WndRoiPtr->InitWndRoiInspection(bModelInit);		
	}

	CAOIWndMask *MaskWndPtr = NULL;
	const size_t MaskWndCount = GetWndMaskWndCount_Inline();
	for ( i=0; i<MaskWndCount; i++ )
	{
		MaskWndPtr = GetWndMaskWndPtr_Inline(i);
		if ( NULL == MaskWndPtr ) { continue; }		
		MaskWndPtr->SetWndMaskActived(false);
		MaskWndPtr->ResetWndMaskRegionRes();
	}

	ClearWndResultBoxList_Inline();
	if ( m_WndAlgParam.InitAlgInspection(bModelInit) == false ) { return false; }
	return true;
}
//--------------------------------------------------------------------------------------------//
bool CAOIWnd::CalcWndInspectRect(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const TPOINT2D &RgnCp, const TPOINT2D &Scale, const TPOINT2D &ImageCp, RECT &RoiRect, RECT &WndRect)//計算檢測框檢測區域
{
	TREGION4D    WndRgn;	
	TREGION4D    WndCalcRgn;	
	TREGION4D    WndExtendRgn;	
	const bool UseResPos = true;
	if ( false == UseResPos )
	{	
		m_WndBox.GetBoxRegion(WndRgn);  
		m_WndExtendBox.GetBoxRegion(WndExtendRgn);
	}
	else
	{	
		m_WndBox.GetBoxRegionRes(WndRgn);  
		m_WndExtendBox.GetBoxRegionRes(WndExtendRgn);
	}		

	CAOIModel::CalcModelBoxRegionRect(WndRgn, ImageW, ImageH, RgnCp, Scale, ImageCp, WndRect);
	SetWndImageRect(WndRect);

	//Extend
	if ( false == m_WndExtendBoxUsed )
	{	
		RoiRect = WndRect;
		WndCalcRgn = WndRgn;
	}
	else
	{	
		WndCalcRgn = WndExtendRgn;	
		CAOIModel::CalcModelBoxRegionRect(WndExtendRgn, ImageW, ImageH, RgnCp, Scale, ImageCp, RoiRect);	
	}
	SetWndExtendImageRect(RoiRect);

	TREGION4D BoxRgn;
	CAOIModel::CalcModelBoxRectRegion(RoiRect, ImageW, ImageH, RgnCp, Scale, ImageCp, BoxRgn);
	m_WndRectCalValue.x = BoxRgn.GetCpX()-WndCalcRgn.GetCpX();
	m_WndRectCalValue.y = BoxRgn.GetCpY()-WndCalcRgn.GetCpY();
	return true;
}
//--------------------------------------------------------------------------------------------//
bool CAOIWnd::ExecWndInspection(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const TPOINT2D &RgnCp, const TPOINT2D &Scale, const TPOINT2D &ImageCp, std::vector<TUNI_FRAME> &UniFrameList, bool bTestWnd)
{	
	LARGE_INTEGER          nStartTime;
	LARGE_INTEGER          nEndTime;
	SaveWndLog(_T("Test"), ModelPtr, _T("Start"));
	QueryPerformanceCounter(&nStartTime);
	bool IsOK = ExecWndInspectionKernel(ModelPtr, ModelRgn, RgnCp, Scale, ImageCp, UniFrameList, bTestWnd);
	QueryPerformanceCounter(&nEndTime);
	SaveWndLog(_T("Test"), ModelPtr, _T("End"));
	m_WndInspectedTime = (nEndTime.QuadPart-nStartTime.QuadPart)*1000.0/AOIDataCollect.m_SystemFreq.QuadPart;			
	return IsOK;
}
//--------------------------------------------------------------------------------------------//
bool CAOIWnd::ExecWndConstrainKernel()//執行檢測框侷限函式
{	
	WND_CONSTRAIN_MODE WndConstrainMode = GetWndConstrainMode();
	if ( WND_CONSTRAIN_DISABLE == WndConstrainMode ) { return true; }

	TREGION4D LandRegion, WndRegion;	
	double WndSizeX=0, WndSizeY=0;
	double LandSizeX=0, LandSizeY=0;	
	CAOILand *LandPtr = GetWndLandPtr();	
	if ( WND_CONSTRAIN_PAD_RGN_MOVE==WndConstrainMode || WND_CONSTRAIN_PAD_RGN_X_MOVE==WndConstrainMode || WND_CONSTRAIN_PAD_RGN_Y_MOVE==WndConstrainMode )
	{
		bool   bContrainOffset=false;
		double ContrainOffsetX=0, ContrainOffsetY=0;
		if ( NULL != LandPtr )
		{			
			GetWndBox().GetBoxRegionRes(WndRegion);
			GetWndBox().GetBoxSizeRes(WndSizeX, WndSizeY);
			LandPtr->GetLandPadBox().GetBoxRegionRes(LandRegion);
			LandPtr->GetLandPadBox().GetBoxSizeRes(LandSizeX, LandSizeY);	
			if ( WND_CONSTRAIN_PAD_RGN_MOVE==WndConstrainMode || WND_CONSTRAIN_PAD_RGN_X_MOVE==WndConstrainMode )
			{	
				if ( WndSizeX < LandSizeX )
				{
					if ( WndRegion.minX < LandRegion.minX )
					{
						bContrainOffset = true;
						ContrainOffsetX = LandRegion.minX-WndRegion.minX;
					}
					if ( WndRegion.maxX > LandRegion.maxX )
					{
						bContrainOffset = true;
						ContrainOffsetX = LandRegion.maxX-WndRegion.maxX;
					}
				}
			}
			if ( WND_CONSTRAIN_PAD_RGN_MOVE==WndConstrainMode || WND_CONSTRAIN_PAD_RGN_Y_MOVE==WndConstrainMode )
			{	
				if ( WndSizeY < LandSizeY )
				{
					if ( WndRegion.minY < LandRegion.minY )
					{
						bContrainOffset = true;
						ContrainOffsetY = LandRegion.minY-WndRegion.minY;
					}
					if ( WndRegion.maxY > LandRegion.maxY )
					{
						bContrainOffset = true;
						ContrainOffsetY = LandRegion.maxY-WndRegion.maxY;
					}
				}
			}
			if ( true == bContrainOffset )
			{	MoveWndResult(ContrainOffsetX, ContrainOffsetY);	}			
		}
		return true;
	}

	const size_t WndRoiCount = GetWndRoiWndCount_Inline();
	const size_t WndMaskCount = GetWndMaskWndCount_Inline();
	if ( 0 == WndRoiCount && 0 == WndMaskCount )
	{
		if ( WND_CONSTRAIN_PAD_RGN_SCALE==WndConstrainMode || WND_CONSTRAIN_PAD_RGN_X_SCALE==WndConstrainMode || WND_CONSTRAIN_PAD_RGN_Y_SCALE==WndConstrainMode )
		{		
			double    SizeX=0;
			double    SizeY=0;			
			TREGION4D WndRegionNew;					
			bool      bContrainScale=false;			
			const double SysMinSize = 50;
			if ( NULL != LandPtr )
			{	
				GetWndBox().GetBoxRegionRes(WndRegion);
				GetWndBox().GetBoxSizeRes(WndSizeX, WndSizeY);
				LandPtr->GetLandPadBox().GetBoxRegionRes(LandRegion);
				LandPtr->GetLandPadBox().GetBoxSizeRes(LandSizeX, LandSizeY);	
				WndRegionNew = WndRegion;
				if ( WND_CONSTRAIN_PAD_RGN_SCALE==WndConstrainMode || WND_CONSTRAIN_PAD_RGN_X_SCALE==WndConstrainMode )
				{	
					if ( WndSizeX < LandSizeX )
					{
						bool      bContrainMinX=false;
						bool      bContrainMaxX=false;
						double    MinSize = MIN(SysMinSize, WndSizeX);
						if ( WndRegionNew.minX < LandRegion.minX )
						{
							bContrainMinX = true;
							bContrainScale = true;						
							WndRegionNew.minX = LandRegion.minX;
						}
						if ( WndRegionNew.maxX > LandRegion.maxX )
						{
							bContrainMaxX = true;
							bContrainScale = true;
							WndRegionNew.maxX = LandRegion.maxX;
						}
						//確保尺寸不要小於50um
						SizeX = WndRegionNew.maxX-WndRegionNew.minX;
						if ( SizeX < 0 )
						{
							WndRegionNew.minX = WndRegion.minX;
							WndRegionNew.maxX = WndRegion.maxX;
						}
						else if ( SizeX < MinSize )
						{
							if ( true == bContrainMinX ) 
							{	WndRegionNew.maxX = WndRegionNew.minX+MinSize;	}
							if ( true == bContrainMaxX ) 
							{	WndRegionNew.minX = WndRegionNew.maxX-MinSize;	}
						}
					}
				}
				if ( WND_CONSTRAIN_PAD_RGN_SCALE==WndConstrainMode || WND_CONSTRAIN_PAD_RGN_Y_SCALE==WndConstrainMode )
				{	
					if ( WndSizeY < LandSizeY )
					{
						bool      bContrainMinY=false;
						bool      bContrainMaxY=false;
						double    MinSize = MIN(SysMinSize, WndSizeY);
						if ( WndRegion.minY < LandRegion.minY )
						{
							bContrainMinY = true;
							bContrainScale = true;						
							WndRegionNew.minY = LandRegion.minY;
						}
						if ( WndRegion.maxY > LandRegion.maxY )
						{
							bContrainMaxY = true;
							bContrainScale = true;
							WndRegionNew.maxY = LandRegion.maxY;
						}
						//確保尺寸不要小於50um
						SizeY = WndRegionNew.maxY-WndRegionNew.minY;
						if ( SizeY < 0 )
						{
							WndRegionNew.minY = WndRegion.minY;
							WndRegionNew.maxY = WndRegion.maxY;
						}
						else if ( SizeY < MinSize )
						{
							if ( true == bContrainMinY ) 
							{	WndRegionNew.maxY = WndRegionNew.minY+MinSize;	}
							if ( true == bContrainMaxY ) 
							{	WndRegionNew.minY = WndRegionNew.maxY-MinSize;	}
						}
					}
				}
				if ( true == bContrainScale )
				{	
					TREGION4D WndRegionExt;
					const double OffsetMinX=WndRegionNew.minX-WndRegion.minX;
					const double OffsetMinY=WndRegionNew.minY-WndRegion.minY;
					const double OffsetMaxX=WndRegionNew.maxX-WndRegion.maxX;
					const double OffsetMaxY=WndRegionNew.maxY-WndRegion.maxY;
					
					GetWndExtendBox().GetBoxRegionRes(WndRegionExt);
					WndRegionExt.minX += OffsetMinX;
					WndRegionExt.minY += OffsetMinY;
					WndRegionExt.maxX += OffsetMaxX;
					WndRegionExt.maxY += OffsetMaxY;

					GetWndBox().SetBoxRegionRes(WndRegionNew);
					GetWndExtendBox().SetBoxRegionRes(WndRegionExt);					
				}
			}
			return true;
		}
	}
	return true;
}
//--------------------------------------------------------------------------------------------//
bool CAOIWnd::ExecWndInspectionKernel(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const TPOINT2D &RgnCp, const TPOINT2D &Scale, const TPOINT2D &ImageCp, std::vector<TUNI_FRAME> &UniFrameList, bool bTestWnd)
{
	if ( NULL == ModelPtr ) 
	{	return false; }
	CString      str;		
	TUNI_FRAME  *UniFramePtr=NULL;		
	CAlgParam       &AlgParam = GetWndAlgParam();
	CAlgBinaryParam *BinMaskPtr = AlgParam.GetAlgMaskBinParamPtr();
	CAlgBinaryParam *BinImagePtr = AlgParam.GetAlgImageBinParamPtr();	
	CAOIComponent   *ComponentPtr = ModelPtr->GetModelComponentPtr();
	const size_t FrameCount = UniFrameList.size();
	const bool Bypass3D = ModelPtr->GetModelBypass3D();
	const size_t MaskFrameIndex = BinMaskPtr->GetBinaryFrameIndex();
	unsigned int MaskFrameUniqueID = BinMaskPtr->GetBinaryFrameUniqueID();
	const size_t ImageFrameIndex = BinImagePtr->GetBinaryFrameIndex();
	unsigned int ImageFrameUniqueID = BinImagePtr->GetBinaryFrameUniqueID();
	if ( true == Bypass3D )
	{
		if ( FRAME_UNIQUE_ID_DLP==MaskFrameUniqueID || FRAME_UNIQUE_ID_DLP==ImageFrameUniqueID )
		{
			SetWndResultID(RESULT_ID_BYPASS);//RESULT_ID_SKIP
			AlgParam.SetAlgResultID(RESULT_ID_BYPASS);			
			return true;
		}
	}
	if ( MaskFrameIndex>=FrameCount || ImageFrameIndex>=FrameCount ) 
	{	return false; }

	RECT         RoiRect={0, 0, 0, 0};
	RECT         WndRect={0, 0, 0, 0};	
	UniFramePtr = &(UniFrameList[ImageFrameIndex]);
	const IMAGE_SIZE ImageW    = UniFramePtr->ImageW;
	const IMAGE_SIZE ImageH    = UniFramePtr->ImageH;
	const IMAGE_SIZE ImageStep = UniFramePtr->ImageStep;
	const IMAGE_SIZE BitCount  = UniFramePtr->BitCount;		
	IMAGE_PTR        ImagePtr  = UniFramePtr->ImagePtr;
	MASK_PTR         MaskPtr   = UniFramePtr->MaskPtr;
	SPACE_PTR        SpacePtr  = UniFramePtr->SpacePtr;
	BOOL             bSaved = TRUE;

	//確認此檢測框是否有侷限於焊盤內
	if ( ExecWndConstrainKernel() == false )
	{	return false; }

	if ( CalcWndInspectRect(ImageW, ImageH, RgnCp, Scale, ImageCp, RoiRect, WndRect) == false )
	{	return false; }

	if ( FN_ENABLE==AOIDataCollect.GetSystemParameter().m_ModelImageCadOffsetEnabled )	
	{
		m_WndRectCalValue.x += ModelPtr->GetModelImageCadOffset_um().x;//25250829
		m_WndRectCalValue.y += ModelPtr->GetModelImageCadOffset_um().y;//25250829
	}

	IMAGE_SIZE   RoiW=0, RoiH=0, RoiStep=0;
	RoiW = RoiRect.right-RoiRect.left;
	RoiH = RoiRect.bottom-RoiRect.top;
	RoiStep=JetAPI::GetBMPImagePixelsPerLine(RoiW, BitCount, 4);	
#ifdef _DEBUG	
	if ( TRUE == bSaved ) 
	{
		CString      ComponentName;
		IMAGE_PTR    RoiImagePtr  = NULL;
		MASK_PTR     RoiMaskPtr   = NULL;
		SPACE_PTR    RoiSpacePtr  = NULL;	

		if ( NULL != ComponentPtr )
		{	ComponentName = ComponentPtr->GetComponentFullName();	}
		if ( NULL != ImagePtr )
		{
			if ( ImageAPI.ExtractRoiImage(ImageW, ImageH, ImageStep, BitCount, ImagePtr, RoiRect, RoiStep, RoiImagePtr, false) == false )
			{	return false;	}
			str.Format(_T("%s\\%s_ModelWndImage#%u.BMP"), AOIDataCollect.GetAOITempDirectory(), ComponentName, this->GetWndIndex()+1);
			ImageAPI.SaveBMPImage(str, RoiW, RoiH, RoiStep, BitCount, RoiImagePtr, true);
			JetMemory.free_func(RoiImagePtr);
		}

		if ( NULL != MaskPtr )
		{
			if ( ImageAPI.ExtractRoiImage(ImageW, ImageH, ImageStep, BitCount, MaskPtr, RoiRect, RoiStep, RoiMaskPtr, false) == false )
			{	return false;	}
			str.Format(_T("%s\\%s_ModelWndMask#%u.BMP"), AOIDataCollect.GetAOITempDirectory(), ComponentName, this->GetWndIndex()+1);
			ImageAPI.SaveBMPImage(str, RoiW, RoiH, RoiStep, BitCount, RoiMaskPtr, true);
			JetMemory.free_func(RoiMaskPtr);
		}
	
		if ( NULL!=SpacePtr && NULL!=RoiMaskPtr )
		{			
			if ( ImageAPI.ExtractSpaceRoiImage(ImageW, ImageH, ImageStep, BitCount, SpacePtr, RoiRect, RoiStep, RoiSpacePtr, false) == false )
			{
				JetMemory.free_func(RoiMaskPtr);
				return false;
			}
			RECT  RoiRect2={0, 0, 0, 0};			
			//const double Raito = -1;
			const double SpaceRatio = AOIDataCollect.GetSpaceToGrayRatio();
			RoiRect2.left = 0;
			RoiRect2.top = 0;
			RoiRect2.right = (int)(RoiW);
			RoiRect2.bottom = (int)(RoiH);
			if ( ImageAPI.SpaceGrayImageConvertToGray(RoiW, RoiH, RoiStep, RoiSpacePtr, RoiMaskPtr, RoiRect2, RoiStep, RoiImagePtr, SpaceRatio, false) == true )
			{
				str.Format(_T("%s\\%s_ModelWndSpace#%u.BMP"), AOIDataCollect.GetAOITempDirectory(), ComponentName, this->GetWndIndex()+1);			
				ImageAPI.SaveBMPImage(str, RoiW, RoiH, RoiStep, BitCount, RoiImagePtr, true);
			}
			JetMemory.free_func(RoiMaskPtr);
			JetMemory.free_func(RoiImagePtr);
			JetMemory.free_func(RoiSpacePtr);			
		}
		else
		{	JetMemory.free_func(RoiMaskPtr); }
	}
#endif//_DEBUG
	SaveWndTestTrackFile();
	const bool bSuccAlg=AlgParam.ExecAlgInspection(ModelPtr, ModelRgn, RoiRect, WndRect, UniFrameList, bTestWnd);
	DeleteWndTestTrackFile();
	if ( false == bSuccAlg )
	{	return false;	}

	WND_LOGIC_TYPE WndLogicType = GetWndLogicType();
	if ( WND_LOGIC_NONE == WndLogicType )
	{	SetWndLogicResultID(GetWndResultID());	}
	return true;
}
//--------------------------------------------------------------------------------------------//
bool CAOIWnd::BuildWndModelMaskWndList(const CAOIModel *ModelPtr, const TREGION4D &BoundRgn, const TREGION4D &BoundRgnRes, std::vector<CAOIBox> &MaskBoxList)//建立模組遮罩匡列表
{
	//加入遮罩框	
	if ( NULL == ModelPtr ) { return false; }

	size_t   i=0;
	CAOIBox *BoxPtr=NULL;
	CAOIBox  tmpBox;
	CAOIBox  MaskBox;
	TREGION4D BoxRgn;
	TREGION4D BoxRgnRes;
	BOX_TOWARD BoxToward;
	BOX_SHAPE_MODE BoxShapeMode;
	CAOILand *RefLandPtr = GetWndLandPtr();
	const int WndMaskModelFlag = GetWndModelMaskFlag();
	const double ComAngle = 0;//ModelPtr->GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComAngle);
	const TPOINT2D AttachedPosCad = GetWndAttachedPosCad();
	const TPOINT2D AttachedPosStage = GetWndAttachedPosStage();	
	BOX_SHAPE_MODE WndShapeMode = GetWndShapeMode();
	
	MaskBox.SetBoxAttachedAngle(0);
	MaskBox.SetBoxAttachedPosCad(AttachedPosCad);
	MaskBox.SetBoxAttachedPosStage(AttachedPosStage);	

	bool MaskBody = JetAPI::BitMask_Check(WndMaskModelFlag, MODEL_MASK_BODY);	
	if ( true == MaskBody )
	{
		tmpBox = ModelPtr->GetModelBodyBox();		
		if ( true == IsExceptionAngle )
		{	tmpBox.RotateBox(-ComAngle, 0, 0);	}
		
		BoxToward = tmpBox.GetBoxToward();
		BoxShapeMode = tmpBox.GetBoxShapeMode();

		tmpBox.GetBoxRegion(BoxRgn);
		tmpBox.GetBoxRegionRes(BoxRgnRes);
		if ( JetAPI::CheckRgnInRegion(BoxRgn, BoundRgn, false ) == true ) 
		{
			JetAPI::BoundaryRegion(BoundRgn, BoxRgn);
			JetAPI::BoundaryRegion(BoundRgnRes, BoxRgnRes);

			MaskBox.SetBoxToward(BoxToward);
			MaskBox.SetBoxShapeMode(BoxShapeMode);
			MaskBox.SetBoxRegion(BoxRgn, false);
			MaskBox.SetBoxRegionRes(BoxRgnRes);					
			MaskBoxList.push_back(MaskBox);		
		}
	}	
	
	const size_t LandCount = ModelPtr->GetModelLandCount();
	bool MaskPad = JetAPI::BitMask_Check(WndMaskModelFlag, MODEL_MASK_PAD);
	bool MaskLead = JetAPI::BitMask_Check(WndMaskModelFlag, MODEL_MASK_LEAD);
	bool MaskLeadTip = JetAPI::BitMask_Check(WndMaskModelFlag, MODEL_MASK_LEAD_TIP);
	bool MaskLeadShoulder = JetAPI::BitMask_Check(WndMaskModelFlag, MODEL_MASK_LEAD_SHOULDER);
	bool MaskBodyNoLead = JetAPI::BitMask_Check(WndMaskModelFlag, MODEL_MASK_BODY_NO_LEAD);	
	if ( true==MaskPad || true==MaskLead || true==MaskLeadTip || true==MaskLeadShoulder || true==MaskBodyNoLead )
	{		
		TREGION4D  BodyRgn;
		TREGION4D  BodyRgnRes;
		TREGION4D  BodyNoLeadRgn;
		TREGION4D  BodyNoLeadRgnRes;
		CAOIBox    BodyBox;	
		CAOILand  *LandPtr = NULL;
		if ( true == MaskBodyNoLead )
		{
			BodyBox=ModelPtr->GetModelBodyBox();	
			if ( true == IsExceptionAngle )
			{	BodyBox.RotateBox(-ComAngle, 0, 0);	}
			BodyBox.GetBoxRegion(BodyRgn);
			BodyBox.GetBoxRegionRes(BodyRgnRes);
			BodyNoLeadRgn = BodyRgn;
			BodyNoLeadRgnRes = BodyRgnRes;
		}

		if ( NULL != RefLandPtr )//腳上的框
		{
			LandPtr = RefLandPtr;
			
			//Pad
			BoxPtr = LandPtr->GetLandPadBoxPtr();
			if ( true==MaskPad && BoxPtr->GetBoxEnabled()==true )
			{
				tmpBox = *BoxPtr;
				if ( true == IsExceptionAngle )
				{	tmpBox.RotateBox(-ComAngle, 0, 0);	}

				BoxToward = tmpBox.GetBoxToward();
				BoxShapeMode = tmpBox.GetBoxShapeMode();

				tmpBox.GetBoxRegion(BoxRgn);
				tmpBox.GetBoxRegionRes(BoxRgnRes);
				if ( JetAPI::CheckRgnInRegion(BoxRgn, BoundRgn, false ) == true ) 
				{
					JetAPI::BoundaryRegion(BoundRgn, BoxRgn);
					JetAPI::BoundaryRegion(BoundRgnRes, BoxRgnRes);

					MaskBox.SetBoxToward(BoxToward);
					MaskBox.SetBoxShapeMode(BoxShapeMode);
					MaskBox.SetBoxRegion(BoxRgn, false);
					MaskBox.SetBoxRegionRes(BoxRgnRes);					
					MaskBoxList.push_back(MaskBox);		
				}
			}

			//Lead
			BoxPtr = LandPtr->GetLandLeadBoxPtr();
			if ( BoxPtr->GetBoxEnabled()==true )
			{
				if ( true==MaskLead || true==MaskBodyNoLead )
				{
					tmpBox = *BoxPtr;
					if ( true == IsExceptionAngle )
					{	tmpBox.RotateBox(-ComAngle, 0, 0);	}
		
					BoxToward = tmpBox.GetBoxToward();
					BoxShapeMode = tmpBox.GetBoxShapeMode();

					tmpBox.GetBoxRegion(BoxRgn);
					tmpBox.GetBoxRegionRes(BoxRgnRes);
					if ( true == MaskBodyNoLead )
					{
						JetAPI::ExcludeRegion(BodyRgn, BoxRgn, BoxToward, BodyNoLeadRgn);
						JetAPI::ExcludeRegion(BodyRgnRes, BoxRgnRes, BoxToward, BodyNoLeadRgnRes);
						BodyRgn = BodyNoLeadRgn;
						BodyRgnRes = BodyNoLeadRgnRes;							
					}
					if ( true == MaskLead )
					{
						if ( JetAPI::CheckRgnInRegion(BoxRgn, BoundRgn, false ) == true ) 
						{
							JetAPI::BoundaryRegion(BoundRgn, BoxRgn);
							JetAPI::BoundaryRegion(BoundRgnRes, BoxRgnRes);

							MaskBox.SetBoxToward(BoxToward);
							MaskBox.SetBoxShapeMode(BoxShapeMode);
							MaskBox.SetBoxRegion(BoxRgn, false);
							MaskBox.SetBoxRegionRes(BoxRgnRes);					
							MaskBoxList.push_back(MaskBox);		
						}
					}
				}
			}

			if ( false == MaskLead )
			{
				//Lead Tip
				BoxPtr = LandPtr->GetLandLeadTipBoxPtr();
				if ( true==MaskLeadTip && BoxPtr->GetBoxEnabled()==true )
				{
					tmpBox = *BoxPtr;
					if ( true == IsExceptionAngle )
					{	tmpBox.RotateBox(-ComAngle, 0, 0);	}
		
					BoxToward = tmpBox.GetBoxToward();
					BoxShapeMode = tmpBox.GetBoxShapeMode();

					tmpBox.GetBoxRegion(BoxRgn);
					tmpBox.GetBoxRegionRes(BoxRgnRes);
					if ( JetAPI::CheckRgnInRegion(BoxRgn, BoundRgn, false ) == true ) 
					{
						JetAPI::BoundaryRegion(BoundRgn, BoxRgn);
						JetAPI::BoundaryRegion(BoundRgnRes, BoxRgnRes);

						MaskBox.SetBoxToward(BoxToward);
						MaskBox.SetBoxShapeMode(BoxShapeMode);
						MaskBox.SetBoxRegion(BoxRgn, false);
						MaskBox.SetBoxRegionRes(BoxRgnRes);					
						MaskBoxList.push_back(MaskBox);		
					}
				}

				//Lead Shoulder
				BoxPtr = LandPtr->GetLandLeadShoulderBoxPtr();
				if ( true==MaskLeadShoulder && BoxPtr->GetBoxEnabled()==true )
				{
					tmpBox = *BoxPtr;
					if ( true == IsExceptionAngle )
					{	tmpBox.RotateBox(-ComAngle, 0, 0);	}
		
					BoxToward = tmpBox.GetBoxToward();
					BoxShapeMode = tmpBox.GetBoxShapeMode();

					tmpBox.GetBoxRegion(BoxRgn);
					tmpBox.GetBoxRegionRes(BoxRgnRes);
					if ( JetAPI::CheckRgnInRegion(BoxRgn, BoundRgn, false ) == true ) 
					{
						JetAPI::BoundaryRegion(BoundRgn, BoxRgn);
						JetAPI::BoundaryRegion(BoundRgnRes, BoxRgnRes);

						MaskBox.SetBoxToward(BoxToward);
						MaskBox.SetBoxShapeMode(BoxShapeMode);
						MaskBox.SetBoxRegion(BoxRgn, false);
						MaskBox.SetBoxRegionRes(BoxRgnRes);					
						MaskBoxList.push_back(MaskBox);		
					}
				}
			}			
		}
		else//本體上的框
		{
			for ( i=0; i<LandCount; i++ )
			{
				LandPtr = ModelPtr->GetModelLandPtr(i, false);
				if ( NULL == LandPtr ) { continue; }

				//Pad
				BoxPtr = LandPtr->GetLandPadBoxPtr();
				if ( true==MaskPad && BoxPtr->GetBoxEnabled()==true )
				{
					tmpBox = *BoxPtr;
					if ( true == IsExceptionAngle )
					{	tmpBox.RotateBox(-ComAngle, 0, 0);	}
		
					BoxToward = tmpBox.GetBoxToward();
					BoxShapeMode = tmpBox.GetBoxShapeMode();

					tmpBox.GetBoxRegion(BoxRgn);
					tmpBox.GetBoxRegionRes(BoxRgnRes);
					if ( JetAPI::CheckRgnInRegion(BoxRgn, BoundRgn, false ) == true ) 
					{
						JetAPI::BoundaryRegion(BoundRgn, BoxRgn);
						JetAPI::BoundaryRegion(BoundRgnRes, BoxRgnRes);

						MaskBox.SetBoxToward(BoxToward);
						MaskBox.SetBoxShapeMode(BoxShapeMode);
						MaskBox.SetBoxRegion(BoxRgn, false);
						MaskBox.SetBoxRegionRes(BoxRgnRes);					
						MaskBoxList.push_back(MaskBox);		
					}
				}

				//Lead
				BoxPtr = LandPtr->GetLandLeadBoxPtr();
				if ( BoxPtr->GetBoxEnabled()==true )
				{
					if ( true==MaskLead || true==MaskBodyNoLead )
					{
						tmpBox = *BoxPtr;
						if ( true == IsExceptionAngle )
						{	tmpBox.RotateBox(-ComAngle, 0, 0);	}
		
						BoxToward = tmpBox.GetBoxToward();
						BoxShapeMode = tmpBox.GetBoxShapeMode();

						tmpBox.GetBoxRegion(BoxRgn);
						tmpBox.GetBoxRegionRes(BoxRgnRes);

						if ( true == MaskBodyNoLead )
						{
							JetAPI::ExcludeRegion(BodyRgn, BoxRgn, BoxToward, BodyNoLeadRgn);
							JetAPI::ExcludeRegion(BodyRgnRes, BoxRgnRes, BoxToward, BodyNoLeadRgnRes);
							BodyRgn = BodyNoLeadRgn;
							BodyRgnRes = BodyNoLeadRgnRes;							
						}
						if ( true == MaskLead )
						{
							if ( JetAPI::CheckRgnInRegion(BoxRgn, BoundRgn, false ) == true ) 
							{
								JetAPI::BoundaryRegion(BoundRgn, BoxRgn);
								JetAPI::BoundaryRegion(BoundRgnRes, BoxRgnRes);

								MaskBox.SetBoxToward(BoxToward);
								MaskBox.SetBoxShapeMode(BoxShapeMode);
								MaskBox.SetBoxRegion(BoxRgn, false);
								MaskBox.SetBoxRegionRes(BoxRgnRes);					
								MaskBoxList.push_back(MaskBox);		
							}
						}
					}
				}

				if ( false == MaskLead )
				{
					//Lead Tip
					BoxPtr = LandPtr->GetLandLeadTipBoxPtr();
					if ( true==MaskLeadTip && BoxPtr->GetBoxEnabled()==true )
					{
						tmpBox = *BoxPtr;
						if ( true == IsExceptionAngle )
						{	tmpBox.RotateBox(-ComAngle, 0, 0);	}
		
						BoxToward = tmpBox.GetBoxToward();
						BoxShapeMode = tmpBox.GetBoxShapeMode();

						tmpBox.GetBoxRegion(BoxRgn);
						tmpBox.GetBoxRegionRes(BoxRgnRes);
						if ( JetAPI::CheckRgnInRegion(BoxRgn, BoundRgn, false ) == true ) 
						{
							JetAPI::BoundaryRegion(BoundRgn, BoxRgn);
							JetAPI::BoundaryRegion(BoundRgnRes, BoxRgnRes);

							MaskBox.SetBoxToward(BoxToward);
							MaskBox.SetBoxShapeMode(BoxShapeMode);
							MaskBox.SetBoxRegion(BoxRgn, false);
							MaskBox.SetBoxRegionRes(BoxRgnRes);					
							MaskBoxList.push_back(MaskBox);		
						}
					}

					//Lead Shoulder
					BoxPtr = LandPtr->GetLandLeadShoulderBoxPtr();
					if ( true==MaskLeadShoulder && BoxPtr->GetBoxEnabled()==true )
					{
						tmpBox = *BoxPtr;
						if ( true == IsExceptionAngle )
						{	tmpBox.RotateBox(-ComAngle, 0, 0);	}
		
						BoxToward = tmpBox.GetBoxToward();
						BoxShapeMode = tmpBox.GetBoxShapeMode();

						tmpBox.GetBoxRegion(BoxRgn);
						tmpBox.GetBoxRegionRes(BoxRgnRes);
						if ( JetAPI::CheckRgnInRegion(BoxRgn, BoundRgn, false ) == true ) 
						{
							JetAPI::BoundaryRegion(BoundRgn, BoxRgn);
							JetAPI::BoundaryRegion(BoundRgnRes, BoxRgnRes);

							MaskBox.SetBoxToward(BoxToward);
							MaskBox.SetBoxShapeMode(BoxShapeMode);
							MaskBox.SetBoxRegion(BoxRgn, false);
							MaskBox.SetBoxRegionRes(BoxRgnRes);					
							MaskBoxList.push_back(MaskBox);		
						}
					}
				}
			}
		}

		if ( true == MaskBodyNoLead )
		{			
			BoxToward = BodyBox.GetBoxToward();
			BoxShapeMode = BodyBox.GetBoxShapeMode();
			BoxRgn = BodyNoLeadRgn;
			BoxRgnRes = BodyNoLeadRgnRes;
			if ( JetAPI::CheckRgnInRegion(BoxRgn, BoundRgn, false ) == true ) 
			{
				JetAPI::BoundaryRegion(BoundRgn, BoxRgn);
				JetAPI::BoundaryRegion(BoundRgnRes, BoxRgnRes);

				MaskBox.SetBoxToward(BoxToward);
				MaskBox.SetBoxShapeMode(BoxShapeMode);
				MaskBox.SetBoxRegion(BoxRgn, false);
				MaskBox.SetBoxRegionRes(BoxRgnRes);					
				MaskBoxList.push_back(MaskBox);		
			}
		}

	}	
	return true;
}
//--------------------------------------------------------------------------------------------//
bool CAOIWnd::BuildWndBoxImage(const CAOIModel *ModelPtr, const TPOINT2D &Scale, IMAGE_SIZE BitCount, IMAGE_SIZE &PatW, IMAGE_SIZE &PatH, IMAGE_SIZE &PatStep, IMAGE_PTR &PatPtr, std::vector<TREGION4D> &FeatureList)//建立檢測框-框樣板圖像
{
	JetMemory.free_func(PatPtr);
	if ( NULL == ModelPtr ) { return false; }
	CAOIComponent *ComponentPtr = ModelPtr->GetModelComponentPtr();

	const char fnName[] = "CAOIWnd::BuildWndBoxImage";
	double WndRgnLinkRatioX = GetWndRgnLinkRatioX();
	double WndRgnLinkRatioY = GetWndRgnLinkRatioY();	
	WND_RGN_LINK_MODE WndLinkMode = GetWndRgnLinkMode();		
	WndRgnLinkRatioX = WndRgnLinkRatioX/100.0;
	WndRgnLinkRatioY = WndRgnLinkRatioY/100.0;

	size_t    i=0, j=0, k=0;	
	bool      bFirst=true;
	int       LandGroupID=0;	
	int       ScaleMode=0;
	RECT      BoxRect={0,0,0,0};
	TPOINT2D   PadTipCp;	
	TPOINT2D   PadTipSz;
	TPOINT2D   PadTipSzRes;	
	TPOINT2D   PadTipCpRes;	
	TREGION4D  BoxRgn;
	TREGION4D  BoxRgnRes;
	TREGION4D  PadRgn;
	TREGION4D  PadRgnRes;
	TREGION4D  LeadRgn;
	TREGION4D  LeadRgnRes;
	TREGION4D  PadTipRgn;
	TREGION4D  PadTipRgnRes;
	BOX_TOWARD BoxToward;
	BOX_TOWARD LandToward;	
	CAOIBox  *BoxPtr = NULL;
	CAOIWnd  *WndPtr = NULL;
	CAOILand *LandPtr = NULL;	
	CAOILand *RefLandPtr = GetWndLandPtr();	
	CAOIBox  tmpBox, WndBox;	
	CAOIBox   PadBox, LeadBox;
	std::vector<CAOIBox> BoxList;
	std::vector<CAOIBox> MaskBoxList;	
	double         BoxShapeParam=0.0;
	double         BoxShapeParam2=0.0;
	BOX_SHAPE_MODE BoxShapeMode=BOX_SHAPE_RECTANGLE;

	const double ComAngle = 0;
	const bool   IsExceptionAngle = false;
	const size_t WndMaskBoxCount = GetWndMaskWndCount();
	const size_t ModelLandCount = ModelPtr->GetModelLandCount();
	const TPOINT2D AttachedPosCad = GetWndAttachedPosCad();
	const TPOINT2D AttachedPosStage = GetWndAttachedPosStage();	
	BOX_SHAPE_MODE WndShapeMode = GetWndShapeMode();
	const double   WndShapeParam= GetWndShapeParam();
	const double   WndShapeParam2= GetWndShapeParam2();

	bFirst=true;
	WndBox.SetBoxAttachedAngle(0);
	WndBox.SetBoxAttachedPosCad(AttachedPosCad);
	WndBox.SetBoxAttachedPosStage(AttachedPosStage);	
	
	BoxList.clear();	
	if ( WND_RGN_LINK_PAD == WndLinkMode )
	{
		if ( NULL != RefLandPtr ) 
		{	
			tmpBox = RefLandPtr->GetLandPadBox();
			if ( true == IsExceptionAngle )
			{	tmpBox.RotateBox(-ComAngle, 0, 0);	}

			BoxToward = tmpBox.GetBoxToward();			
			tmpBox.GetBoxRegion(BoxRgn);
			tmpBox.GetBoxRegionRes(BoxRgnRes);
			ScaleMode=SCALE_REGION_BY_CENTER;
			JetAPI::ScaleRegion(BoxRgn, WndRgnLinkRatioX, WndRgnLinkRatioY, ScaleMode, BoxRgn);
			JetAPI::ScaleRegion(BoxRgnRes, WndRgnLinkRatioX, WndRgnLinkRatioY, ScaleMode, BoxRgnRes);

			WndBox.SetBoxToward(BoxToward);
			WndBox.SetBoxShapeMode(WndShapeMode);
			WndBox.SetBoxShapeParam(WndShapeParam);
			WndBox.SetBoxShapeParam2(WndShapeParam2);			
			WndBox.SetBoxRegion(BoxRgn, false);
			WndBox.SetBoxRegionRes(BoxRgnRes);		
			BoxList.push_back(WndBox);
		}
		else
		{
			for ( i=0; i<ModelLandCount; i++ )
			{
				LandPtr = ModelPtr->GetModelLandPtr(i, false);
				if ( NULL == LandPtr ) { continue; }
				if ( LandPtr->GetLandIncludePadAlign() == false ) { continue; }

				tmpBox = LandPtr->GetLandPadBox();
				if ( true == IsExceptionAngle )
				{	tmpBox.RotateBox(-ComAngle, 0, 0);	}				

				BoxToward = tmpBox.GetBoxToward();
				tmpBox.GetBoxRegion(BoxRgn);
				tmpBox.GetBoxRegionRes(BoxRgnRes);
				ScaleMode=SCALE_REGION_BY_CENTER;
				JetAPI::ScaleRegion(BoxRgn, WndRgnLinkRatioX, WndRgnLinkRatioY, ScaleMode, BoxRgn);
				JetAPI::ScaleRegion(BoxRgnRes, WndRgnLinkRatioX, WndRgnLinkRatioY, ScaleMode, BoxRgnRes);

				WndBox.SetBoxToward(BoxToward);
				WndBox.SetBoxShapeMode(WndShapeMode);
				WndBox.SetBoxShapeParam(WndShapeParam);
				WndBox.SetBoxShapeParam2(WndShapeParam2);
				WndBox.SetBoxRegion(BoxRgn, false);
				WndBox.SetBoxRegionRes(BoxRgnRes);			
				BoxList.push_back(WndBox);
			}
		}
	}
	else if ( WND_RGN_LINK_PAD_TIP == WndLinkMode )
	{
		if ( NULL != RefLandPtr ) 
		{			
			LandToward = RefLandPtr->GetLandToward();
			PadBox = RefLandPtr->GetLandPadBox();
			LeadBox = RefLandPtr->GetLandLeadBox();
			BoxToward = PadBox.GetBoxToward();
			if ( true == IsExceptionAngle )
			{				
				PadBox.RotateBox(-ComAngle, 0, 0);
				LeadBox.RotateBox(-ComAngle, 0, 0);
				BoxToward = PadBox.GetBoxToward();
			}
			if ( LeadBox.GetBoxEnabled() == true ) 
			{
				PadBox.GetBoxRegion(PadRgn);
				PadBox.GetBoxRegionRes(PadRgnRes);
				LeadBox.GetBoxRegion(LeadRgn);
				LeadBox.GetBoxRegionRes(LeadRgnRes);
				PadTipRgn = PadRgn;
				PadTipRgnRes = PadRgnRes;
				switch ( BoxToward )
				{
				case BOX_TOWARD_LEFT:
					if ( LeadRgnRes.minX>PadRgnRes.minX && LeadRgnRes.minX<PadRgnRes.maxX )
					{	
						PadTipRgn.maxX = LeadRgn.minX;	
						PadTipRgnRes.maxX = LeadRgnRes.minX;						
					}
					break;
				case BOX_TOWARD_UP:
					if ( LeadRgnRes.maxY>PadRgnRes.minY && LeadRgnRes.maxY<PadRgnRes.maxY )
					{	
						PadTipRgn.minY = LeadRgn.maxY;	
						PadTipRgnRes.minY = LeadRgnRes.maxY;						
					}
					break;
				case BOX_TOWARD_RIGHT:
					if ( LeadRgnRes.maxX>PadRgnRes.minX && LeadRgnRes.maxX<PadRgnRes.maxX )
					{	
						PadTipRgn.minX = LeadRgn.maxX;	
						PadTipRgnRes.minX = LeadRgnRes.maxX;						
					}
					break;
				case BOX_TOWARD_DOWN:
					if ( LeadRgnRes.minY>PadRgnRes.minY && LeadRgnRes.minY<PadRgnRes.maxY )
					{	
						PadTipRgn.maxY = LeadRgn.minY;	
						PadTipRgnRes.maxY = LeadRgnRes.minY;						
					}
					break;
				}				
			}
			else
			{	
				PadBox.GetBoxRegion(PadTipRgn);
				PadBox.GetBoxRegionRes(PadTipRgnRes);
			}
			switch ( BoxToward )
			{
			case BOX_TOWARD_LEFT:				
				ScaleMode=SCALE_REGION_BY_SIDE_MIN_X;
				JetAPI::ScaleRegion(PadTipRgn, WndRgnLinkRatioX, WndRgnLinkRatioY, ScaleMode, PadTipRgn);
				JetAPI::ScaleRegion(PadTipRgnRes, WndRgnLinkRatioX, WndRgnLinkRatioY, ScaleMode, PadTipRgnRes);				
				break;
			case BOX_TOWARD_UP:				
				ScaleMode=SCALE_REGION_BY_SIDE_MAX_Y;
				JetAPI::ScaleRegion(PadTipRgn, WndRgnLinkRatioX, WndRgnLinkRatioY, ScaleMode, PadTipRgn);
				JetAPI::ScaleRegion(PadTipRgnRes, WndRgnLinkRatioX, WndRgnLinkRatioY, ScaleMode, PadTipRgnRes);				
				break;
			case BOX_TOWARD_RIGHT:				
				ScaleMode=SCALE_REGION_BY_SIDE_MAX_X;
				JetAPI::ScaleRegion(PadTipRgn, WndRgnLinkRatioX, WndRgnLinkRatioY, ScaleMode, PadTipRgn);
				JetAPI::ScaleRegion(PadTipRgnRes, WndRgnLinkRatioX, WndRgnLinkRatioY, ScaleMode, PadTipRgnRes);				
				break;
			case BOX_TOWARD_DOWN:				
				ScaleMode=SCALE_REGION_BY_SIDE_MIN_Y;
				JetAPI::ScaleRegion(PadTipRgn, WndRgnLinkRatioX, WndRgnLinkRatioY, ScaleMode, PadTipRgn);
				JetAPI::ScaleRegion(PadTipRgnRes, WndRgnLinkRatioX, WndRgnLinkRatioY, ScaleMode, PadTipRgnRes);				
				break;
			}
			WndBox.SetBoxToward(BoxToward);
			WndBox.SetBoxShapeMode(WndShapeMode);
			WndBox.SetBoxShapeParam(WndShapeParam);
			WndBox.SetBoxShapeParam2(WndShapeParam2);
			WndBox.SetBoxRegion(PadTipRgn, false);
			WndBox.SetBoxRegionRes(PadTipRgnRes);			
			BoxList.push_back(WndBox);
		}
		else
		{
			for ( i=0; i<ModelLandCount; i++ )
			{
				LandPtr = ModelPtr->GetModelLandPtr(i, false);
				if ( NULL == LandPtr ) { continue; }	
				if ( LandPtr->GetLandIncludePadAlign() == false ) { continue; }

				LandToward = LandPtr->GetLandToward();
				PadBox = LandPtr->GetLandPadBox();
				LeadBox = LandPtr->GetLandLeadBox();
				BoxToward = PadBox.GetBoxToward();
				if ( true == IsExceptionAngle )
				{				
					PadBox.RotateBox(-ComAngle, 0, 0);
					LeadBox.RotateBox(-ComAngle, 0, 0);
					BoxToward = PadBox.GetBoxToward();
				}
				if ( LeadBox.GetBoxEnabled() == true ) 
				{
					PadBox.GetBoxRegion(PadRgn);
					PadBox.GetBoxRegionRes(PadRgnRes);
					LeadBox.GetBoxRegion(LeadRgn);
					LeadBox.GetBoxRegionRes(LeadRgnRes);
					PadTipRgn = PadRgn;
					PadTipRgnRes = PadRgnRes;
					switch ( BoxToward )
					{
					case BOX_TOWARD_LEFT:
						if ( LeadRgnRes.minX>PadRgnRes.minX && LeadRgnRes.minX<PadRgnRes.maxX )
						{	
							PadTipRgn.maxX = LeadRgn.minX;	
							PadTipRgnRes.maxX = LeadRgnRes.minX;	
						}
						break;
					case BOX_TOWARD_UP:
						if ( LeadRgnRes.maxY>PadRgnRes.minY && LeadRgnRes.maxY<PadRgnRes.maxY )
						{	
							PadTipRgn.minY = LeadRgn.maxY;	
							PadTipRgnRes.minY = LeadRgnRes.maxY;
						}
						break;
					case BOX_TOWARD_RIGHT:
						if ( LeadRgnRes.maxX>PadRgnRes.minX && LeadRgnRes.maxX<PadRgnRes.maxX )
						{	
							PadTipRgn.minX = LeadRgn.maxX;	
							PadTipRgnRes.minX = LeadRgnRes.maxX;
						}
						break;
					case BOX_TOWARD_DOWN:
						if ( LeadRgnRes.minY>PadRgnRes.minY && LeadRgnRes.minY<PadRgnRes.maxY )
						{	
							PadTipRgn.maxY = LeadRgn.minY;	
							PadTipRgnRes.maxY = LeadRgnRes.minY;
						}
						break;
					}					
				}
				else
				{
					PadBox.GetBoxRegion(PadTipRgn);
					PadBox.GetBoxRegionRes(PadTipRgnRes);
				}
				switch ( BoxToward )
				{
				case BOX_TOWARD_LEFT:				
					ScaleMode=SCALE_REGION_BY_SIDE_MIN_X;
					JetAPI::ScaleRegion(PadTipRgn, WndRgnLinkRatioX, WndRgnLinkRatioY, ScaleMode, PadTipRgn);
					JetAPI::ScaleRegion(PadTipRgnRes, WndRgnLinkRatioX, WndRgnLinkRatioY, ScaleMode, PadTipRgnRes);				
					break;
				case BOX_TOWARD_UP:				
					ScaleMode=SCALE_REGION_BY_SIDE_MAX_Y;
					JetAPI::ScaleRegion(PadTipRgn, WndRgnLinkRatioX, WndRgnLinkRatioY, ScaleMode, PadTipRgn);
					JetAPI::ScaleRegion(PadTipRgnRes, WndRgnLinkRatioX, WndRgnLinkRatioY, ScaleMode, PadTipRgnRes);				
					break;
				case BOX_TOWARD_RIGHT:				
					ScaleMode=SCALE_REGION_BY_SIDE_MAX_X;
					JetAPI::ScaleRegion(PadTipRgn, WndRgnLinkRatioX, WndRgnLinkRatioY, ScaleMode, PadTipRgn);
					JetAPI::ScaleRegion(PadTipRgnRes, WndRgnLinkRatioX, WndRgnLinkRatioY, ScaleMode, PadTipRgnRes);				
					break;
				case BOX_TOWARD_DOWN:				
					ScaleMode=SCALE_REGION_BY_SIDE_MIN_Y;
					JetAPI::ScaleRegion(PadTipRgn, WndRgnLinkRatioX, WndRgnLinkRatioY, ScaleMode, PadTipRgn);
					JetAPI::ScaleRegion(PadTipRgnRes, WndRgnLinkRatioX, WndRgnLinkRatioY, ScaleMode, PadTipRgnRes);
					break;
				}
				WndBox.SetBoxToward(BoxToward);
				WndBox.SetBoxShapeMode(WndShapeMode);
				WndBox.SetBoxShapeParam(WndShapeParam);
				WndBox.SetBoxShapeParam2(WndShapeParam2);
				WndBox.SetBoxRegion(PadTipRgn, false);
				WndBox.SetBoxRegionRes(PadTipRgnRes);				
				BoxList.push_back(WndBox);
			}
		}
	}
	else if ( WND_RGN_LINK_BODY == WndLinkMode )
	{
		tmpBox = ModelPtr->GetModelBodyBox();				
		if ( true == IsExceptionAngle )
		{	tmpBox.RotateBox(-ComAngle, 0, 0);	}

		BoxToward = tmpBox.GetBoxToward();			
		tmpBox.GetBoxRegion(BoxRgn);
		tmpBox.GetBoxRegionRes(BoxRgnRes);
		ScaleMode=SCALE_REGION_BY_CENTER;
		JetAPI::ScaleRegion(BoxRgn, WndRgnLinkRatioX, WndRgnLinkRatioY, ScaleMode, BoxRgn);
		JetAPI::ScaleRegion(BoxRgnRes, WndRgnLinkRatioX, WndRgnLinkRatioY, ScaleMode, BoxRgnRes);
		
		WndBox.SetBoxToward(BoxToward);
		WndBox.SetBoxShapeMode(WndShapeMode);
		WndBox.SetBoxShapeParam(WndShapeParam);
		WndBox.SetBoxShapeParam2(WndShapeParam2);
		WndBox.SetBoxRegion(BoxRgn, false);
		WndBox.SetBoxRegionRes(BoxRgnRes);		
		BoxList.push_back(WndBox);
	}
	else if ( WND_RGN_LINK_LEAD == WndLinkMode )
	{
		if ( NULL != RefLandPtr ) 
		{
			tmpBox = RefLandPtr->GetLandLeadBox();
			if ( true == IsExceptionAngle )
			{	tmpBox.RotateBox(-ComAngle, 0, 0);	}
			
			BoxToward = tmpBox.GetBoxToward();			
			tmpBox.GetBoxRegion(BoxRgn);
			tmpBox.GetBoxRegionRes(BoxRgnRes);
			ScaleMode=SCALE_REGION_BY_CENTER;
			JetAPI::ScaleRegion(BoxRgn, WndRgnLinkRatioX, WndRgnLinkRatioY, ScaleMode, BoxRgn);
			JetAPI::ScaleRegion(BoxRgnRes, WndRgnLinkRatioX, WndRgnLinkRatioY, ScaleMode, BoxRgnRes);

			WndBox.SetBoxToward(BoxToward);
			WndBox.SetBoxShapeMode(WndShapeMode);
			WndBox.SetBoxShapeParam(WndShapeParam);
			WndBox.SetBoxShapeParam2(WndShapeParam2);
			WndBox.SetBoxRegion(BoxRgn, false);
			WndBox.SetBoxRegionRes(BoxRgnRes);			
			BoxList.push_back(WndBox);
		}
		else
		{
			for ( i=0; i<ModelLandCount; i++ )
			{
				LandPtr = ModelPtr->GetModelLandPtr(i, false);
				if ( NULL == LandPtr ) { continue; }
				if ( LandPtr->GetLandIncludePartAlign() == false ) { continue; }
				tmpBox = LandPtr->GetLandLeadBox();
				if ( true == IsExceptionAngle )
				{	tmpBox.RotateBox(-ComAngle, 0, 0); }
				
				BoxToward = tmpBox.GetBoxToward();			
				tmpBox.GetBoxRegion(BoxRgn);
				tmpBox.GetBoxRegionRes(BoxRgnRes);
				ScaleMode=SCALE_REGION_BY_CENTER;
				JetAPI::ScaleRegion(BoxRgn, WndRgnLinkRatioX, WndRgnLinkRatioY, ScaleMode, BoxRgn);
				JetAPI::ScaleRegion(BoxRgnRes, WndRgnLinkRatioX, WndRgnLinkRatioY, ScaleMode, BoxRgnRes);

				WndBox.SetBoxToward(BoxToward);
				WndBox.SetBoxShapeMode(WndShapeMode);
				WndBox.SetBoxShapeParam(WndShapeParam);
				WndBox.SetBoxShapeParam2(WndShapeParam2);
				WndBox.SetBoxRegion(BoxRgn, false);
				WndBox.SetBoxRegionRes(BoxRgnRes);				
				BoxList.push_back(WndBox);
			}
		}
	}
	else if ( WND_RGN_LINK_LEAD_TIP == WndLinkMode )
	{
		if ( NULL != RefLandPtr ) 
		{
			tmpBox = RefLandPtr->GetLandLeadTipBox();
			if ( true == IsExceptionAngle )
			{	tmpBox.RotateBox(-ComAngle, 0, 0);	}
			
			BoxToward = tmpBox.GetBoxToward();			
			tmpBox.GetBoxRegion(BoxRgn);
			tmpBox.GetBoxRegionRes(BoxRgnRes);
			ScaleMode=SCALE_REGION_BY_CENTER;
			JetAPI::ScaleRegion(BoxRgn, WndRgnLinkRatioX, WndRgnLinkRatioY, ScaleMode, BoxRgn);
			JetAPI::ScaleRegion(BoxRgnRes, WndRgnLinkRatioX, WndRgnLinkRatioY, ScaleMode, BoxRgnRes);

			WndBox.SetBoxToward(BoxToward);
			WndBox.SetBoxShapeMode(WndShapeMode);
			WndBox.SetBoxShapeParam(WndShapeParam);
			WndBox.SetBoxShapeParam2(WndShapeParam2);
			WndBox.SetBoxRegion(BoxRgn, false);
			WndBox.SetBoxRegionRes(BoxRgnRes);			
			BoxList.push_back(WndBox);
		}
		else
		{
			for ( i=0; i<ModelLandCount; i++ )
			{
				LandPtr = ModelPtr->GetModelLandPtr(i, false);
				if ( NULL == LandPtr ) { continue; }
				if ( LandPtr->GetLandIncludePartAlign() == false ) { continue; }
				tmpBox = LandPtr->GetLandLeadTipBox();
				if ( true == IsExceptionAngle )
				{	tmpBox.RotateBox(-ComAngle, 0, 0);	}
				
				BoxToward = tmpBox.GetBoxToward();			
				tmpBox.GetBoxRegion(BoxRgn);
				tmpBox.GetBoxRegionRes(BoxRgnRes);
				ScaleMode=SCALE_REGION_BY_CENTER;
				JetAPI::ScaleRegion(BoxRgn, WndRgnLinkRatioX, WndRgnLinkRatioY, ScaleMode, BoxRgn);
				JetAPI::ScaleRegion(BoxRgnRes, WndRgnLinkRatioX, WndRgnLinkRatioY, ScaleMode, BoxRgnRes);

				WndBox.SetBoxToward(BoxToward);
				WndBox.SetBoxShapeMode(WndShapeMode);
				WndBox.SetBoxShapeParam(WndShapeParam);
				WndBox.SetBoxShapeParam2(WndShapeParam2);
				WndBox.SetBoxRegion(BoxRgn, false);
				WndBox.SetBoxRegionRes(BoxRgnRes);				
				BoxList.push_back(WndBox);
			}
		}
	}
	else if ( WND_RGN_LINK_LEAD_SHOULDER == WndLinkMode )
	{
		if ( NULL != RefLandPtr ) 
		{
			tmpBox = RefLandPtr->GetLandLeadShoulderBox();
			if ( true == IsExceptionAngle )
			{	tmpBox.RotateBox(-ComAngle, 0, 0);	}

			BoxToward = tmpBox.GetBoxToward();			
			tmpBox.GetBoxRegion(BoxRgn);
			tmpBox.GetBoxRegionRes(BoxRgnRes);
			ScaleMode=SCALE_REGION_BY_CENTER;
			JetAPI::ScaleRegion(BoxRgn, WndRgnLinkRatioX, WndRgnLinkRatioY, ScaleMode, BoxRgn);
			JetAPI::ScaleRegion(BoxRgnRes, WndRgnLinkRatioX, WndRgnLinkRatioY, ScaleMode, BoxRgnRes);

			WndBox.SetBoxToward(BoxToward);
			WndBox.SetBoxShapeMode(WndShapeMode);
			WndBox.SetBoxShapeParam(WndShapeParam);
			WndBox.SetBoxShapeParam2(WndShapeParam2);
			WndBox.SetBoxRegion(BoxRgn, false);
			WndBox.SetBoxRegionRes(BoxRgnRes);			
			BoxList.push_back(WndBox);
		}
		else
		{
			for ( i=0; i<ModelLandCount; i++ )
			{
				LandPtr = ModelPtr->GetModelLandPtr(i, false);
				if ( NULL == LandPtr ) { continue; }
				if ( LandPtr->GetLandIncludePartAlign() == false ) { continue; }
				tmpBox = LandPtr->GetLandLeadShoulderBox();
				if ( true == IsExceptionAngle )
				{	tmpBox.RotateBox(-ComAngle, 0, 0);	}
				
				BoxToward = tmpBox.GetBoxToward();			
				tmpBox.GetBoxRegion(BoxRgn);
				tmpBox.GetBoxRegionRes(BoxRgnRes);
				ScaleMode=SCALE_REGION_BY_CENTER;
				JetAPI::ScaleRegion(BoxRgn, WndRgnLinkRatioX, WndRgnLinkRatioY, ScaleMode, BoxRgn);
				JetAPI::ScaleRegion(BoxRgnRes, WndRgnLinkRatioX, WndRgnLinkRatioY, ScaleMode, BoxRgnRes);

				WndBox.SetBoxToward(BoxToward);
				WndBox.SetBoxShapeMode(WndShapeMode);
				WndBox.SetBoxShapeParam(WndShapeParam);
				WndBox.SetBoxShapeParam2(WndShapeParam2);
				WndBox.SetBoxRegion(BoxRgn, false);
				WndBox.SetBoxRegionRes(BoxRgnRes);				
				BoxList.push_back(WndBox);
			}
		}
	}
	else if ( WND_RGN_LINK_LEAD_TIP_SHOULDER == WndLinkMode )
	{
		if ( NULL != RefLandPtr ) 
		{
			//Tip
			tmpBox = RefLandPtr->GetLandLeadTipBox();
			if ( true == IsExceptionAngle )
			{	tmpBox.RotateBox(-ComAngle, 0, 0);	}

			BoxToward = tmpBox.GetBoxToward();
			tmpBox.GetBoxRegion(BoxRgn);
			tmpBox.GetBoxRegionRes(BoxRgnRes);
			ScaleMode=SCALE_REGION_BY_CENTER;
			JetAPI::ScaleRegion(BoxRgn, WndRgnLinkRatioX, WndRgnLinkRatioY, ScaleMode, BoxRgn);
			JetAPI::ScaleRegion(BoxRgnRes, WndRgnLinkRatioX, WndRgnLinkRatioY, ScaleMode, BoxRgnRes);

			WndBox.SetBoxToward(BoxToward);
			WndBox.SetBoxShapeMode(WndShapeMode);
			WndBox.SetBoxShapeParam(WndShapeParam);
			WndBox.SetBoxShapeParam2(WndShapeParam2);
			WndBox.SetBoxRegion(BoxRgn, false);
			WndBox.SetBoxRegionRes(BoxRgnRes);			
			BoxList.push_back(WndBox);

			//Shoulder
			tmpBox = RefLandPtr->GetLandLeadShoulderBox();
			if ( true == IsExceptionAngle )
			{	tmpBox.RotateBox(-ComAngle, 0, 0);	}

			BoxToward = tmpBox.GetBoxToward();
			tmpBox.GetBoxRegion(BoxRgn);
			tmpBox.GetBoxRegionRes(BoxRgnRes);
			ScaleMode=SCALE_REGION_BY_CENTER;
			JetAPI::ScaleRegion(BoxRgn, WndRgnLinkRatioX, WndRgnLinkRatioY, ScaleMode, BoxRgn);
			JetAPI::ScaleRegion(BoxRgnRes, WndRgnLinkRatioX, WndRgnLinkRatioY, ScaleMode, BoxRgnRes);

			WndBox.SetBoxToward(BoxToward);
			WndBox.SetBoxShapeMode(BOX_SHAPE_RECTANGLE);
			WndBox.SetBoxShapeParam(WndShapeParam);
			WndBox.SetBoxShapeParam2(WndShapeParam2);
			WndBox.SetBoxRegion(BoxRgn, false);
			WndBox.SetBoxRegionRes(BoxRgnRes);			
			BoxList.push_back(WndBox);
		}
		else
		{
			for ( i=0; i<ModelLandCount; i++ )
			{
				LandPtr = ModelPtr->GetModelLandPtr(i, false);
				if ( NULL == LandPtr ) { continue; }
				if ( LandPtr->GetLandIncludePartAlign() == false ) { continue; }
				//Tip
				tmpBox = LandPtr->GetLandLeadTipBox();
				if ( true == IsExceptionAngle )
				{	tmpBox.RotateBox(-ComAngle, 0, 0);	}

				BoxToward = tmpBox.GetBoxToward();			
				tmpBox.GetBoxRegion(BoxRgn);
				tmpBox.GetBoxRegionRes(BoxRgnRes);
				ScaleMode=SCALE_REGION_BY_CENTER;
				JetAPI::ScaleRegion(BoxRgn, WndRgnLinkRatioX, WndRgnLinkRatioY, ScaleMode, BoxRgn);
				JetAPI::ScaleRegion(BoxRgnRes, WndRgnLinkRatioX, WndRgnLinkRatioY, ScaleMode, BoxRgnRes);

				WndBox.SetBoxToward(BoxToward);
				WndBox.SetBoxShapeMode(WndShapeMode);
				WndBox.SetBoxShapeParam(WndShapeParam);
				WndBox.SetBoxShapeParam2(WndShapeParam2);
				WndBox.SetBoxRegion(BoxRgn, false);
				WndBox.SetBoxRegionRes(BoxRgnRes);				
				BoxList.push_back(WndBox);

				//Shoulder
				tmpBox = LandPtr->GetLandLeadShoulderBox();
				if ( true == IsExceptionAngle )
				{	tmpBox.RotateBox(-ComAngle, 0, 0);	}

				BoxToward = tmpBox.GetBoxToward();			
				tmpBox.GetBoxRegion(BoxRgn);
				tmpBox.GetBoxRegionRes(BoxRgnRes);
				ScaleMode=SCALE_REGION_BY_CENTER;
				JetAPI::ScaleRegion(BoxRgn, WndRgnLinkRatioX, WndRgnLinkRatioY, ScaleMode, BoxRgn);
				JetAPI::ScaleRegion(BoxRgnRes, WndRgnLinkRatioX, WndRgnLinkRatioY, ScaleMode, BoxRgnRes);

				WndBox.SetBoxToward(BoxToward);
				WndBox.SetBoxShapeMode(BOX_SHAPE_RECTANGLE);
				WndBox.SetBoxShapeParam(WndShapeParam);
				WndBox.SetBoxShapeParam2(WndShapeParam2);
				WndBox.SetBoxRegion(BoxRgn, false);
				WndBox.SetBoxRegionRes(BoxRgnRes);				
				BoxList.push_back(WndBox);
			}
		}
	}
	else//WND_RGN_LINK_NONE, WND_RGN_LINK_PAD_RGN, WND_RGN_LINK_PAD_BODY_RGN, WND_RGN_LINK_PAD_RGN_INNER
	{
		tmpBox = CAOIWnd::GetWndBox();
		if ( true == IsExceptionAngle )
		{	tmpBox.RotateBox(-ComAngle, 0, 0);	}

		BoxToward = tmpBox.GetBoxToward();
		tmpBox.GetBoxRegion(BoxRgn);
		tmpBox.GetBoxRegionRes(BoxRgnRes);
		ScaleMode=SCALE_REGION_BY_CENTER;
		JetAPI::ScaleRegion(BoxRgn, WndRgnLinkRatioX, WndRgnLinkRatioY, ScaleMode, BoxRgn);
		JetAPI::ScaleRegion(BoxRgnRes, WndRgnLinkRatioX, WndRgnLinkRatioY, ScaleMode, BoxRgnRes);

		WndBox.SetBoxToward(BoxToward);
		WndBox.SetBoxShapeMode(WndShapeMode);
		WndBox.SetBoxShapeParam(WndShapeParam);
		WndBox.SetBoxShapeParam2(WndShapeParam2);
		WndBox.SetBoxRegion(BoxRgn, false);
		WndBox.SetBoxRegionRes(BoxRgnRes);			
		BoxList.push_back(WndBox);
	}

	//確定範圍
	bool  UsResPos = true;
	bFirst=true;	
	TREGION4D AllRgn;	
	TREGION4D AllRgnRes;	
	const size_t BoxCount = BoxList.size();	
	for ( i=0; i<BoxCount; i++ )
	{
		BoxPtr = &(BoxList[i]);
		if ( NULL == BoxPtr ) { continue; }
		
		BoxPtr->GetBoxRegion(BoxRgn);
		BoxPtr->GetBoxRegionRes(BoxRgnRes);
		FeatureList.push_back(BoxRgnRes);
		if ( true == bFirst )
		{
			bFirst = false;
			AllRgn = BoxRgn;
			AllRgnRes = BoxRgnRes;
		}
		else
		{	
			JetAPI::UnionRegion(AllRgn, BoxRgn, AllRgn);
			JetAPI::UnionRegion(AllRgnRes, BoxRgnRes, AllRgnRes);			
		}		
	}
	
	if ( NULL == RefLandPtr ) //修正單邊焊盤的臭蟲
	{	
		tmpBox = CAOIWnd::GetWndBox();
		if ( true == IsExceptionAngle )
		{	tmpBox.RotateBox(-ComAngle, 0, 0);	}
		BoxToward = tmpBox.GetBoxToward();
		tmpBox.GetBoxRegion(BoxRgn);
		tmpBox.GetBoxRegionRes(BoxRgnRes);
		if ( true == bFirst )
		{
			bFirst = false;
			AllRgn = BoxRgn;
			AllRgnRes = BoxRgnRes;
		}
		else
		{	
			JetAPI::UnionRegion(AllRgn, BoxRgn, AllRgn);
			JetAPI::UnionRegion(AllRgnRes, BoxRgnRes, AllRgnRes);
		}
	}	

	//加入模組遮罩框	
	MaskBoxList.clear();
	if ( BuildWndModelMaskWndList(ModelPtr, AllRgn, AllRgnRes, MaskBoxList) == false )
	{	return false; }

	//加入一般遮罩框		
	CAOIWndMask *MaskWndPtr=NULL;
	for ( i=0; i<WndMaskBoxCount; i++ )
	{
		MaskWndPtr = GetWndMaskWndPtr_Inline(i);
		if ( NULL == MaskWndPtr ) { continue; }
		BoxPtr = MaskWndPtr->GetWndMaskBoxPtr();
		if ( NULL == BoxPtr ) { continue; }
		tmpBox = *BoxPtr;
		if ( true == IsExceptionAngle )
		{	tmpBox.RotateBox(-ComAngle, 0, 0);	}

		BoxToward = tmpBox.GetBoxToward();
		BoxShapeMode = tmpBox.GetBoxShapeMode();
		BoxShapeParam = tmpBox.GetBoxShapeParam();
		BoxShapeParam2= tmpBox.GetBoxShapeParam2();
		tmpBox.GetBoxRegion(BoxRgn);
		tmpBox.GetBoxRegionRes(BoxRgnRes);

		WndBox.SetBoxToward(BoxToward);
		WndBox.SetBoxShapeMode(BoxShapeMode);
		WndBox.SetBoxShapeParam(BoxShapeParam);
		WndBox.SetBoxShapeParam2(BoxShapeParam2);
		WndBox.SetBoxRegion(BoxRgn, false);
		WndBox.SetBoxRegionRes(BoxRgnRes);					
		MaskBoxList.push_back(WndBox);
	}

	//加入遮罩範圍
	const size_t MaskWndCount = MaskBoxList.size();	
	for ( i=0; i<MaskWndCount; i++ )
	{
		BoxPtr = &(MaskBoxList[i]);
		if ( NULL == BoxPtr ) { continue; }
		BoxPtr->GetBoxRegion(BoxRgn);
		BoxPtr->GetBoxRegionRes(BoxRgnRes);
		if ( true == bFirst )
		{
			bFirst = false;						
			AllRgn = BoxRgn;
			AllRgnRes = BoxRgnRes;
		}
		else
		{	
			JetAPI::UnionRegion(AllRgn, BoxRgn, AllRgn);
			JetAPI::UnionRegion(AllRgnRes, BoxRgnRes, AllRgnRes);
		}	
	}	
	
	//後面程式使用AllRgn, 所以這裡要切換
	BoxRgn = AllRgn;
	BoxRgnRes = AllRgnRes;
	if ( true == UsResPos )
	{	AllRgn = BoxRgnRes;	}
	else
	{	AllRgn = BoxRgn;	}	
	const int nAlign = 4;
	IMAGE_PTR pPattern = NULL;
	const IMAGE_SIZE MarginW=5;
	const IMAGE_SIZE MarginH=5;
	const double RegionCPX = AllRgn.GetCpX();
	const double RegionCPY = AllRgn.GetCpY();
	const double RegionW = AllRgn.GetWidth();
	const double RegionH = AllRgn.GetHeight();
	//const IMAGE_SIZE RgnImageW = (IMAGE_SIZE)(RegionW*Scale.x+0.9999);
	//const IMAGE_SIZE RgnImageH = (IMAGE_SIZE)(RegionH*Scale.y+0.9999);
	const IMAGE_SIZE RgnImageW = (IMAGE_SIZE)(RegionW*Scale.x+0.5);//改成四捨五入
	const IMAGE_SIZE RgnImageH = (IMAGE_SIZE)(RegionH*Scale.y+0.5);//改成四捨五入
	const IMAGE_SIZE PatternW = RgnImageW+(MarginW*2);
	const IMAGE_SIZE PatternH = RgnImageH+(MarginH*2);
	const IMAGE_SIZE PatBitCount = BitCount;
	const IMAGE_SIZE PatternStep = JetAPI::GetBMPImagePixelsPerLine(PatternW, PatBitCount, nAlign);
	const int PatternCPX = (int)(PatternW/2);
	const int PatternCPY = (int)(PatternH/2);
	const size_t BufferSize=ImageAPI.CalcBufferSize(PatternStep, PatternH);	
	if ( JetMemory.alloc_func(BufferSize, pPattern, fnName, "pPattern") == false )
	{	return false; }	
	
	cv::Mat    Img;
	IplImage  *pImg = NULL;
	const bool bUseMat=true;
	int        CreateMode=CREATE_OPENCV_IMAGE_NULL;
	if ( true == bUseMat )
	{
		Img = ImageAPI.CreateMat(PatternW, PatternH, PatternStep, PatBitCount, pPattern);
		if ( ImageAPI.CheckMatIsValid(Img) == false )
		{
			JetMemory.free_func(pPattern);
			return false;
		}
	}
	else
	{
		if ( 24 == PatBitCount )
		{	pImg = ImageAPI.ColorImageToCVImage(PatternW, PatternH, PatternStep, pPattern, false, CreateMode);	}
		else
		{	pImg = ImageAPI.GrayImageToCVImage(PatternW, PatternH, PatternStep, pPattern, false, CreateMode);	}		
		if ( pImg == NULL )
		{
			JetMemory.free_func(pPattern);
			return false;	
		}		
	}
	
	int   Radius=0;
	int   RectW=0, RectH=0;	
	unsigned char BKClr   = 0x00;
	const unsigned char FillClr = 0xFF;			
	const unsigned char MaskClr = 0x00;
	RECT  MainRect={0,0,0,0};
	RECT  MaskRect={0,0,0,0};
	
	CvScalar color;
	CvScalar bkcolor;

	TPOINT2D PatRgnCp, PatImageCp;
	PatRgnCp.x = RegionCPX;
	PatRgnCp.y = RegionCPY;
	PatImageCp.x = (PatternW*0.5);
	PatImageCp.y = (PatternH*0.5);
	
	MainRect.left   = MarginW;
	//MainRect.right  = PatternW-MarginW-1;
	MainRect.right  = PatternW-MarginW;//改成左右對稱
	MainRect.top    = MarginH;
	//MainRect.bottom = PatternH-MarginH-1;
	MainRect.bottom = PatternH-MarginH;//改成上下對稱

	//Bk Image
	if ( 0 == MaskWndCount )
	{	BKClr   = 0x00;	}
	else
	{	BKClr   = 0x10;}
	bkcolor.val[0] = BKClr;
	bkcolor.val[1] = BKClr;
	bkcolor.val[2] = BKClr;
	bkcolor.val[3] =   0;	
	::memset(pPattern, BKClr, sizeof(unsigned char)*BufferSize);	

	color.val[0] = FillClr;
	color.val[1] = FillClr;
	color.val[2] = FillClr;
	color.val[3] =   0;			
	
	//Wnd Section
	//BoxToward       = WndPtr->GetWndToward();
	//BoxShapeMode = WndPtr->GetWndOuterMaskShapeMode();	
	BoxToward = BOX_TOWARD_NULL;
	for ( i=0; i<BoxCount; i++ )
	{
		BoxPtr = &(BoxList[i]);
		if ( BoxPtr == NULL ) { continue; }
		
		if ( true == UsResPos )
		{	BoxPtr->GetBoxRegionRes(BoxRgn);	}
		else
		{	BoxPtr->GetBoxRegion(BoxRgn);	}		
		//if ( CAOIModel::CalcModelBoxRegionRect(BoxRgn, ImageW, ImageH, RgnCp, Scale, ImageCp, MaskRect) == false ) 
		if ( CAOIModel::CalcModelBoxRegionRect(BoxRgn, PatternW, PatternH, PatRgnCp, Scale, PatImageCp, MaskRect) == false ) 
		{	continue; }		
				
		if ( MaskRect.left<MainRect.left ) { MaskRect.left = MainRect.left; }
		if ( MaskRect.left>MainRect.right ) { MaskRect.left=MainRect.right; }
		if ( MaskRect.top<MainRect.top ) { MaskRect.top = MainRect.top; }
		if ( MaskRect.top>MainRect.bottom ) { MaskRect.top=MainRect.bottom; }
		if ( MaskRect.right<MainRect.left ) { MaskRect.right = MainRect.left; }
		if ( MaskRect.right>MainRect.right ) { MaskRect.right=MainRect.right; }
		if ( MaskRect.bottom<MainRect.top ) { MaskRect.bottom = MainRect.top; }
		if ( MaskRect.bottom>MainRect.bottom ) { MaskRect.bottom=MainRect.bottom; }

		BoxToward = BoxPtr->GetBoxToward();
		BoxShapeMode = BoxPtr->GetBoxShapeMode();
		BoxShapeParam = BoxPtr->GetBoxShapeParam();
		BoxShapeParam2= BoxPtr->GetBoxShapeParam2();
		//因為OpenCV繪製該位置上
		//MaskRect.right = MaskRect.right + 1;
		//MaskRect.bottom = MaskRect.bottom +1;
		switch ( BoxShapeMode )
		{
		case BOX_SHAPE_ROUND_RECT:			
			Radius = CAOIBox::CalcRoundRectRectRadius(MaskRect, BoxShapeParam);
			if ( true == bUseMat )
			{	ImageAPI.DrawBoxImage_RroundRect(Img, color, bkcolor, BoxToward, MaskRect, Radius);	}
			else
			{	ImageAPI.DrawBoxImage_RroundRect(pImg, color, bkcolor, BoxToward, MaskRect, Radius);	}
			break;
		case BOX_SHAPE_ELLIPSE:
			if ( true == bUseMat )
			{	ImageAPI.DrawBoxImage_Ellipse(Img, color, bkcolor, BoxToward, MaskRect);	}
			else
			{	ImageAPI.DrawBoxImage_Ellipse(pImg, color, bkcolor, BoxToward, MaskRect);	}						
			break;
		case BOX_SHAPE_CAPSULE:
			if ( true == bUseMat )
			{	ImageAPI.DrawBoxImage_Capsule(Img, color, bkcolor, BoxToward, MaskRect);	}
			else
			{	ImageAPI.DrawBoxImage_Capsule(pImg, color, bkcolor, BoxToward, MaskRect);	}
			break;
		case BOX_SHAPE_BULLET:
			if ( true == bUseMat )
			{	ImageAPI.DrawBoxImage_Bullet(Img, color, bkcolor, BoxToward, MaskRect);	}
			else
			{	ImageAPI.DrawBoxImage_Bullet(pImg, color, bkcolor, BoxToward, MaskRect);	}
			break;
		case BOX_SHAPE_HALF_ROUND_RECT:			
			Radius = CAOIBox::CalcHalfRoundRectRectRadius(MaskRect, BoxShapeParam, BoxToward);			
			if ( true == bUseMat )
			{	ImageAPI.DrawBoxImage_HalfRroundRect(Img, color, bkcolor, BoxToward, MaskRect, Radius);	}
			else
			{	ImageAPI.DrawBoxImage_HalfRroundRect(pImg, color, bkcolor, BoxToward, MaskRect, Radius);	}
			break;
		case BOX_SHAPE_T_SHAPE:
			if ( true == bUseMat )
			{	ImageAPI.DrawBoxImage_TShapeRect(Img, color, bkcolor, BoxToward, MaskRect, BoxShapeParam, BoxShapeParam2);	}
			else
			{	ImageAPI.DrawBoxImage_TShapeRect(pImg, color, bkcolor, BoxToward, MaskRect, BoxShapeParam, BoxShapeParam2);	}			
			break;
		default://BOX_SHAPE_RECTANGLE
			if ( true == bUseMat )
			{	ImageAPI.DrawBoxImage_Rect(Img, color, bkcolor, BoxToward, MaskRect);	}
			else
			{	ImageAPI.DrawBoxImage_Rect(pImg, color, bkcolor, BoxToward, MaskRect);	}
			break;
		}
	}	

	//繪製遮罩框
	color.val[0] = MaskClr;
	color.val[1] = MaskClr;
	color.val[2] = MaskClr;
	color.val[3] =   0;		
	BoxToward = BOX_TOWARD_NULL;
	for ( i=0; i<MaskWndCount; i++ )
	{
		BoxPtr = &(MaskBoxList[i]);
		if ( BoxPtr == NULL ) { continue; }
		
		if ( true == UsResPos )
		{	BoxPtr->GetBoxRegionRes(BoxRgn); }
		else
		{	BoxPtr->GetBoxRegion(BoxRgn); }
		
		//if ( CAOIModel::CalcModelBoxRegionRect(BoxRgn, ImageW, ImageH, RgnCp, Scale, ImageCp, MaskRect) == false ) 
		if ( CAOIModel::CalcModelBoxRegionRect(BoxRgn, PatternW, PatternH, PatRgnCp, Scale, PatImageCp, MaskRect) == false ) 
		{	continue; }		
				
		if ( MaskRect.left<MainRect.left ) { MaskRect.left = MainRect.left; }
		if ( MaskRect.left>MainRect.right ) { MaskRect.left=MainRect.right; }
		if ( MaskRect.top<MainRect.top ) { MaskRect.top = MainRect.top; }
		if ( MaskRect.top>MainRect.bottom ) { MaskRect.top=MainRect.bottom; }
		if ( MaskRect.right<MainRect.left ) { MaskRect.right = MainRect.left; }
		if ( MaskRect.right>MainRect.right ) { MaskRect.right=MainRect.right; }
		if ( MaskRect.bottom<MainRect.top ) { MaskRect.bottom = MainRect.top; }
		if ( MaskRect.bottom>MainRect.bottom ) { MaskRect.bottom=MainRect.bottom; }

		BoxToward = BoxPtr->GetBoxToward();
		BoxShapeMode = BoxPtr->GetBoxShapeMode();
		BoxShapeParam = BoxPtr->GetBoxShapeParam();
		switch ( BoxShapeMode )
		{
		case BOX_SHAPE_ROUND_RECT:
			Radius = CAOIBox::CalcRoundRectRectRadius(MaskRect, BoxShapeParam);
			if ( true == bUseMat )
			{	ImageAPI.DrawBoxImage_RroundRect(Img, color, bkcolor, BoxToward, MaskRect, Radius);	}
			else
			{	ImageAPI.DrawBoxImage_RroundRect(pImg, color, bkcolor, BoxToward, MaskRect, Radius);	}			
			break;
		case BOX_SHAPE_ELLIPSE:
			if ( true == bUseMat )
			{	ImageAPI.DrawBoxImage_Ellipse(Img, color, bkcolor, BoxToward, MaskRect);	}
			else
			{	ImageAPI.DrawBoxImage_Ellipse(pImg, color, bkcolor, BoxToward, MaskRect);	}	
			break;
		case BOX_SHAPE_CAPSULE:
			if ( true == bUseMat )
			{	ImageAPI.DrawBoxImage_Capsule(Img, color, bkcolor, BoxToward, MaskRect);	}
			else
			{	ImageAPI.DrawBoxImage_Capsule(pImg, color, bkcolor, BoxToward, MaskRect);	}
			break;
		case BOX_SHAPE_BULLET:
			if ( true == bUseMat )
			{	ImageAPI.DrawBoxImage_Bullet(Img, color, bkcolor, BoxToward, MaskRect);	}
			else
			{	ImageAPI.DrawBoxImage_Bullet(pImg, color, bkcolor, BoxToward, MaskRect);	}
			break;
		case BOX_SHAPE_HALF_ROUND_RECT:			
			Radius = CAOIBox::CalcHalfRoundRectRectRadius(MaskRect, BoxShapeParam, BoxToward);
			if ( true == bUseMat )
			{	ImageAPI.DrawBoxImage_HalfRroundRect(Img, color, bkcolor, BoxToward, MaskRect, Radius);	}
			else
			{	ImageAPI.DrawBoxImage_HalfRroundRect(pImg, color, bkcolor, BoxToward, MaskRect, Radius);	}
			break;
		case BOX_SHAPE_T_SHAPE:
			if ( true == bUseMat )
			{	ImageAPI.DrawBoxImage_TShapeRect(Img, color, bkcolor, BoxToward, MaskRect, BoxShapeParam, BoxShapeParam2);	}
			else
			{	ImageAPI.DrawBoxImage_TShapeRect(pImg, color, bkcolor, BoxToward, MaskRect, BoxShapeParam, BoxShapeParam2);	}			
			break;
		default://BOX_SHAPE_RECTANGLE
			if ( true == bUseMat )
			{	ImageAPI.DrawBoxImage_Rect(Img, color, bkcolor, BoxToward, MaskRect);	}
			else
			{	ImageAPI.DrawBoxImage_Rect(pImg, color, bkcolor, BoxToward, MaskRect);	}
			break;
		}
	}	
	//Copy OpenCV to Memory Buffer
	//::memcpy(pPattern, pImg->imageData, pImg->imageSize);//指標派指到自己的記憶體指標, 無需此項複製複製
	if ( true == bUseMat )
	{
		if ( ImageAPI.CloneMatData(Img, PatternW, PatternH, PatternStep, PatBitCount, pPattern) == false )
		{
			JetMemory.free_func(pPattern);
			return false;	
		}
	}
	else
	{
		//::cvReleaseImage(&pImg);	
		//::cvReleaseImageHeader(&pImg);//釋放影像[檔頭]記憶體空間
		ImageAPI.DestroyCVImage(CreateMode, pImg);			
	}	
#ifdef _DEBUG
	BOOL bSaved = TRUE;
	if ( TRUE == bSaved )
	{
		CString str;
		CString ComponentName;
		if ( NULL != ComponentPtr )
		{	ComponentName = ComponentPtr->GetComponentFullName();	}
		str.Format(_T("%s\\%s_WndBoxImage#%d.PNG"), AOIDataCollect.GetAOITempDirectory(), ComponentName, CAOIWnd::GetWndIndex()+1);
		ImageAPI.SavePNGImage(str, PatternW, PatternH, PatternStep, PatBitCount, pPattern, true);
	}
#endif//_DEBUG
	PatW = PatternW;
	PatH = PatternH;
	PatStep = PatternStep;	
	PatPtr = pPattern;	
	return true;
}
//--------------------------------------------------------------------------------------------//
bool CAOIWnd::BuildWndRoiImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, const RECT &WndRect, int sX, int sY, bool UsResPos)//建立檢測框-框樣板圖像	
{	
	if ( NULL == ImagePtr )
	{	return false; }
	CAOIModel *ModelPtr = GetWndModelPtr();
	if ( NULL == ModelPtr ) { return false; }
	CAOIComponent *ComponentPtr = ModelPtr->GetModelComponentPtr();

	cv::Mat    Img;
	IplImage  *pImg = NULL;	
	const bool bUseMat=true;
	int        CreateMode=CREATE_OPENCV_IMAGE_NULL;	
	if ( true == bUseMat )
	{
		Img = ImageAPI.CreateMat(ImageW, ImageH, ImageStep, BitCount, ImagePtr);
		if ( ImageAPI.CheckMatIsValid(Img) == false )
		{	return false;	}
	}
	else
	{
		if ( 24 == BitCount )
		{	pImg = ImageAPI.ColorImageToCVImage(ImageW, ImageH, ImageStep, ImagePtr, false, CreateMode);	}
		else
		{	pImg = ImageAPI.GrayImageToCVImage(ImageW, ImageH, ImageStep, ImagePtr, false, CreateMode);	}		
		if ( pImg == NULL )
		{	return false;	}
	}
	
	int   Radius=0;
	int   RectW=0, RectH=0;	
	unsigned char BKClr   = 0x00;
	const unsigned char FillClr = 0xFF;			
	const unsigned char MaskClr = 0x00;
	RECT  MainRect={0,0,0,0};
	RECT  MaskRect={0,0,0,0};
	CvScalar color;
	CvScalar bkcolor;
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	JetAPI::SizeToRect(ImageW, ImageH, MainRect);	

	//Bk Image	
	bkcolor.val[0] = BKClr;
	bkcolor.val[1] = BKClr;
	bkcolor.val[2] = BKClr;
	bkcolor.val[3] =   0;	
	::memset(ImagePtr, BKClr, sizeof(unsigned char)*BufferSize);	

	color.val[0] = FillClr;
	color.val[1] = FillClr;
	color.val[2] = FillClr;
	color.val[3] =   0;			
	
	//Wnd Section
	//BoxToward       = WndPtr->GetWndToward();
	//BoxShapeMode = WndPtr->GetWndOuterMaskShapeMode();	
	TREGION4D BoxRgn;
	TREGION4D WndRgn;	
	BOX_TOWARD BoxToward = BOX_TOWARD_NULL;	
	double BoxShapeParam=0, BoxShapeParam2=0;
	BOX_SHAPE_MODE BoxShapeMode = BOX_SHAPE_RECTANGLE;
	const size_t WndRoiCount = GetWndRoiWndCount();
	const int MarginX = sX;
	const int MarginY = sY;
	const int MarginX2 = ::abs(MarginX*2);
	const int MarginY2 = ::abs(MarginY*2);
	if ( true == UsResPos )
	{	GetWndBox().GetBoxRegionRes(WndRgn);	}
	else
	{	GetWndBox().GetBoxRegion(WndRgn);	}

	for ( size_t i=0; i<WndRoiCount; i++ )
	{
		CAOIWndRoi *WndRoiPtr = GetWndRoiWndPtr(i, false);
		if ( NULL == WndRoiPtr ) { continue; }
		CAOIBox *BoxPtr = WndRoiPtr->GetWndRoiBoxPtr();
		if ( true == UsResPos )
		{	BoxPtr->GetBoxRegionRes(BoxRgn);	}
		else
		{	BoxPtr->GetBoxRegion(BoxRgn);	}		
		//if ( CAOIModel::CalcModelBoxRegionRect(BoxRgn, ImageW, ImageH, RgnCp, Scale, ImageCp, MaskRect) == false ) 
		//if ( CAOIModel::CalcModelBoxRegionRect(BoxRgn, PatternW, PatternH, PatRgnCp, Scale, PatImageCp, MaskRect) == false ) 
		if ( JetAPI::MapRegionToRect(WndRgn, BoxRgn, WndRect, MaskRect) == false )
		{	continue; }

		if ( (MaskRect.right-MaskRect.left) <= MarginX2 ) { continue; }
		if ( (MaskRect.bottom-MaskRect.top) <= MarginY2 ) { continue; }		
		::InflateRect(&MaskRect, MarginX, MarginY);	
		//::InflateRect(&MaskRect, -MarginX, -MarginY);		

		if ( MaskRect.left<MainRect.left ) { MaskRect.left = MainRect.left; }
		if ( MaskRect.left>MainRect.right ) { MaskRect.left=MainRect.right; }
		if ( MaskRect.top<MainRect.top ) { MaskRect.top = MainRect.top; }
		if ( MaskRect.top>MainRect.bottom ) { MaskRect.top=MainRect.bottom; }
		if ( MaskRect.right<MainRect.left ) { MaskRect.right = MainRect.left; }
		if ( MaskRect.right>MainRect.right ) { MaskRect.right=MainRect.right; }
		if ( MaskRect.bottom<MainRect.top ) { MaskRect.bottom = MainRect.top; }
		if ( MaskRect.bottom>MainRect.bottom ) { MaskRect.bottom=MainRect.bottom; }

		BoxToward = BoxPtr->GetBoxToward();
		BoxShapeMode = BoxPtr->GetBoxShapeMode();
		BoxShapeParam = BoxPtr->GetBoxShapeParam();
		BoxShapeParam2= BoxPtr->GetBoxShapeParam2();
		//因為OpenCV繪製該位置上
		//MaskRect.right = MaskRect.right + 1;
		//MaskRect.bottom = MaskRect.bottom +1;
		switch ( BoxShapeMode )
		{
		case BOX_SHAPE_ROUND_RECT:			
			Radius = CAOIBox::CalcRoundRectRectRadius(MaskRect, BoxShapeParam);
			if ( true == bUseMat )
			{	ImageAPI.DrawBoxImage_RroundRect(Img, color, bkcolor, BoxToward, MaskRect, Radius);	}
			else
			{	ImageAPI.DrawBoxImage_RroundRect(pImg, color, bkcolor, BoxToward, MaskRect, Radius);	}
			break;
		case BOX_SHAPE_ELLIPSE:
			if ( true == bUseMat )
			{	ImageAPI.DrawBoxImage_Ellipse(Img, color, bkcolor, BoxToward, MaskRect);	}
			else
			{	ImageAPI.DrawBoxImage_Ellipse(pImg, color, bkcolor, BoxToward, MaskRect);	}						
			break;
		case BOX_SHAPE_CAPSULE:
			if ( true == bUseMat )
			{	ImageAPI.DrawBoxImage_Capsule(Img, color, bkcolor, BoxToward, MaskRect);	}
			else
			{	ImageAPI.DrawBoxImage_Capsule(pImg, color, bkcolor, BoxToward, MaskRect);	}
			break;
		case BOX_SHAPE_BULLET:
			if ( true == bUseMat )
			{	ImageAPI.DrawBoxImage_Bullet(Img, color, bkcolor, BoxToward, MaskRect);	}
			else
			{	ImageAPI.DrawBoxImage_Bullet(pImg, color, bkcolor, BoxToward, MaskRect);	}
			break;
		case BOX_SHAPE_HALF_ROUND_RECT:			
			Radius = CAOIBox::CalcHalfRoundRectRectRadius(MaskRect, BoxShapeParam, BoxToward);			
			if ( true == bUseMat )
			{	ImageAPI.DrawBoxImage_HalfRroundRect(Img, color, bkcolor, BoxToward, MaskRect, Radius);	}
			else
			{	ImageAPI.DrawBoxImage_HalfRroundRect(pImg, color, bkcolor, BoxToward, MaskRect, Radius);	}
			break;
		case BOX_SHAPE_T_SHAPE:
			if ( true == bUseMat )
			{	ImageAPI.DrawBoxImage_TShapeRect(Img, color, bkcolor, BoxToward, MaskRect, BoxShapeParam, BoxShapeParam2);	}
			else
			{	ImageAPI.DrawBoxImage_TShapeRect(pImg, color, bkcolor, BoxToward, MaskRect, BoxShapeParam, BoxShapeParam2);	}
			break;		
		default://BOX_SHAPE_RECTANGLE
			if ( true == bUseMat )
			{	ImageAPI.DrawBoxImage_Rect(Img, color, bkcolor, BoxToward, MaskRect);	}
			else
			{	ImageAPI.DrawBoxImage_Rect(pImg, color, bkcolor, BoxToward, MaskRect);	}
			break;
		}
	}	

	//繪製遮罩框
	color.val[0] = MaskClr;
	color.val[1] = MaskClr;
	color.val[2] = MaskClr;
	color.val[3] =   0;		
	BoxToward = BOX_TOWARD_NULL;
	const size_t MaskWndCount = 0;
	for ( size_t i=0; i<MaskWndCount; i++ )
	{
		//BoxPtr = &(MaskBoxList[i]);
		CAOIBox *BoxPtr = NULL;
		if ( BoxPtr == NULL ) { continue; }
		
		if ( true == UsResPos )
		{	BoxPtr->GetBoxRegionRes(BoxRgn); }
		else
		{	BoxPtr->GetBoxRegion(BoxRgn); }
		
		//if ( CAOIModel::CalcModelBoxRegionRect(BoxRgn, ImageW, ImageH, RgnCp, Scale, ImageCp, MaskRect) == false ) 
		if ( JetAPI::MapRegionToRect(WndRgn, BoxRgn, WndRect, MaskRect) == false )
		{	continue; }

		if ( (MaskRect.right-MaskRect.left) <= MarginX2 ) { continue; }
		if ( (MaskRect.bottom-MaskRect.top) <= MarginY2 ) { continue; }		
		::InflateRect(&MaskRect, -MarginX, -MarginY);	
				
		if ( MaskRect.left<MainRect.left ) { MaskRect.left = MainRect.left; }
		if ( MaskRect.left>MainRect.right ) { MaskRect.left=MainRect.right; }
		if ( MaskRect.top<MainRect.top ) { MaskRect.top = MainRect.top; }
		if ( MaskRect.top>MainRect.bottom ) { MaskRect.top=MainRect.bottom; }
		if ( MaskRect.right<MainRect.left ) { MaskRect.right = MainRect.left; }
		if ( MaskRect.right>MainRect.right ) { MaskRect.right=MainRect.right; }
		if ( MaskRect.bottom<MainRect.top ) { MaskRect.bottom = MainRect.top; }
		if ( MaskRect.bottom>MainRect.bottom ) { MaskRect.bottom=MainRect.bottom; }

		BoxToward = BoxPtr->GetBoxToward();
		BoxShapeMode = BoxPtr->GetBoxShapeMode();
		BoxShapeParam = BoxPtr->GetBoxShapeParam();
		switch ( BoxShapeMode )
		{
		case BOX_SHAPE_ROUND_RECT:
			Radius = CAOIBox::CalcRoundRectRectRadius(MaskRect, BoxShapeParam);
			if ( true == bUseMat )
			{	ImageAPI.DrawBoxImage_RroundRect(Img, color, bkcolor, BoxToward, MaskRect, Radius);	}
			else
			{	ImageAPI.DrawBoxImage_RroundRect(pImg, color, bkcolor, BoxToward, MaskRect, Radius);	}			
			break;
		case BOX_SHAPE_ELLIPSE:
			if ( true == bUseMat )
			{	ImageAPI.DrawBoxImage_Ellipse(Img, color, bkcolor, BoxToward, MaskRect);	}
			else
			{	ImageAPI.DrawBoxImage_Ellipse(pImg, color, bkcolor, BoxToward, MaskRect);	}	
			break;
		case BOX_SHAPE_CAPSULE:
			if ( true == bUseMat )
			{	ImageAPI.DrawBoxImage_Capsule(Img, color, bkcolor, BoxToward, MaskRect);	}
			else
			{	ImageAPI.DrawBoxImage_Capsule(pImg, color, bkcolor, BoxToward, MaskRect);	}
			break;
		case BOX_SHAPE_BULLET:
			if ( true == bUseMat )
			{	ImageAPI.DrawBoxImage_Bullet(Img, color, bkcolor, BoxToward, MaskRect);	}
			else
			{	ImageAPI.DrawBoxImage_Bullet(pImg, color, bkcolor, BoxToward, MaskRect);	}
			break;
		case BOX_SHAPE_HALF_ROUND_RECT:			
			Radius = CAOIBox::CalcHalfRoundRectRectRadius(MaskRect, BoxShapeParam, BoxToward);
			if ( true == bUseMat )
			{	ImageAPI.DrawBoxImage_HalfRroundRect(Img, color, bkcolor, BoxToward, MaskRect, Radius);	}
			else
			{	ImageAPI.DrawBoxImage_HalfRroundRect(pImg, color, bkcolor, BoxToward, MaskRect, Radius);	}
			break;
		case BOX_SHAPE_T_SHAPE:
			if ( true == bUseMat )
			{	ImageAPI.DrawBoxImage_TShapeRect(Img, color, bkcolor, BoxToward, MaskRect, BoxShapeParam, BoxShapeParam2);	}
			else
			{	ImageAPI.DrawBoxImage_TShapeRect(pImg, color, bkcolor, BoxToward, MaskRect, BoxShapeParam, BoxShapeParam2);	}			
			break;		
		default://BOX_SHAPE_RECTANGLE
			if ( true == bUseMat )
			{	ImageAPI.DrawBoxImage_Rect(Img, color, bkcolor, BoxToward, MaskRect);	}
			else
			{	ImageAPI.DrawBoxImage_Rect(pImg, color, bkcolor, BoxToward, MaskRect);	}
			break;
		}
	}	
	//Copy OpenCV to Memory Buffer
	//::memcpy(pPattern, pImg->imageData, pImg->imageSize);//指標派指到自己的記憶體指標, 無需此項複製複製
	if ( true == bUseMat )
	{
		if ( ImageAPI.CloneMatData(Img, ImageW, ImageH, ImageStep, BitCount, ImagePtr) == false )
		{	return false;	}
	}
	else
	{
		//::cvReleaseImage(&pImg);	
		//::cvReleaseImageHeader(&pImg);//釋放影像[檔頭]記憶體空間
		ImageAPI.DestroyCVImage(CreateMode, pImg);			
	}	
#ifdef _DEBUG
	BOOL bSaved = TRUE;
	if ( TRUE == bSaved )
	{
		CString str;
		CString ComponentName;
		if ( NULL != ComponentPtr )
		{	ComponentName = ComponentPtr->GetComponentFullName();	}
		str.Format(_T("%s\\%s_WndRoiImage#%d.PNG"), AOIDataCollect.GetAOITempDirectory(), ComponentName, CAOIWnd::GetWndIndex()+1);
		ImageAPI.SavePNGImage(str, ImageW, ImageH, ImageStep, BitCount, ImagePtr, true);
	}
#endif//_DEBUG	
	return true;
}
//--------------------------------------------------------------------------------------------//
bool CAOIWnd::CheckWndNeedShapeMask()//確認檢測框需要外形遮罩
{
	BOX_SHAPE_MODE BoxShapeMode = GetWndShapeMode();
	if ( BOX_SHAPE_RECTANGLE != BoxShapeMode ) { return true; }

	//模組遮罩
	if ( MODEL_MASK_NONE != GetWndModelMaskFlag() ) { return true; }

	//確認是否有不計算的區域
	const size_t MaskWndCount = GetWndMaskWndCount_Inline();
	if ( MaskWndCount > 0 ) { return true; }

	return false;
}
//--------------------------------------------------------------------------------------------//
bool CAOIWnd::BuildWndShapeMask(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_PTR &ImagePtr, const RECT &WndRect)//建立檢測框-外形遮罩圖像	
{
	IMAGE_SIZE ShapeBitCount = 8;
	const size_t BufferSize=ImageAPI.CalcBufferSize(ImageStep, ImageH);
	if ( JetMemory.alloc_func(BufferSize, ImagePtr, "CAOIWnd::BuildWndShapeMask", "ImagePtr") == false )
	{	return false;	}		
	if ( CheckWndNeedShapeMask() == false )
	{	
		ImageAPI.FillImageRoi(ImageW, ImageH, ImageStep, ShapeBitCount, ImagePtr, WndRect, 255, 255, 255);		
		return true;
	}	
	ImageAPI.FillImageRoi(ImageW, ImageH, ImageStep, ShapeBitCount, ImagePtr, WndRect, 0, 0, 0);	
	if ( BuildWndShapeMask3(ImageW, ImageH, ImageStep, ImagePtr, WndRect) == false )
	{	
		JetMemory.free_func(ImagePtr);
		return false;
	}	
	return true;
}
//--------------------------------------------------------------------------------------------//
bool CAOIWnd::BuildWndShapeMask3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_PTR ImagePtr, const RECT &WndRect)//建立檢測框-外形遮罩圖像
{
	if ( NULL == ImagePtr ) { return false; }
	if ( 0==ImageW || 0==ImageH || ImageStep<ImageW ) { return false; }
	CAOIModel *ModelPtr = this->GetWndModelPtr();
	if ( NULL == ModelPtr ) { return false; }
	CAOIComponent *ComponentPtr = ModelPtr->GetModelComponentPtr();

	size_t     i=0;
	int        j=0, k=0;
	int        imgIdx=0, mskIdx=0;	
	IMAGE_SIZE BitCount = 8;
	BOX_TOWARD BoxToward = GetWndToward();	
	double     BoxShapeParam = GetWndShapeParam();
	double     BoxShapeParam2 = GetWndShapeParam2();
	BOX_SHAPE_MODE BoxShapeMode = GetWndShapeMode();
	
	IMAGE_PTR  WndImgPtr=NULL;
	IMAGE_PTR  ShapeImgPtr=NULL;
	IMAGE_SIZE WndImgW=(IMAGE_SIZE)(WndRect.right-WndRect.left);
	IMAGE_SIZE WndImgH=(IMAGE_SIZE)(WndRect.bottom-WndRect.top);
	IMAGE_SIZE WndImgStep=0;

	//建立遮罩框圖像	
	IMAGE_PTR  BoxImgPtr=NULL;
	IMAGE_SIZE BoxImgW=0;
	IMAGE_SIZE BoxImgH=0;
	IMAGE_SIZE BoxImgStep=0;

	bool         bMaskWnd=false;
	bool         bEraseMode=false;	
	TREGION4D    AllRgn;
	TREGION4D    WndRgn;
	TREGION4D    MaskWndRgn;
	RECT         TempRect={0,0,0,0};
	RECT         TempWndRect={0,0,0,0};
	RECT         MaskWndRect={0,0,0,0};	
	std::vector<CAOIBox> MaskBoxList;
	const size_t MaskWndCount = GetWndMaskWndCount_Inline();

	ModelPtr->GetModelTotalRegion(AllRgn);			
	if ( BuildWndModelMaskWndList(ModelPtr, AllRgn, AllRgn, MaskBoxList) == false )
	{	return false;	}

	//建立檢測框-外形遮罩圖像	
	if ( BuildWndShapeImage(bMaskWnd, BoxToward, BoxShapeMode, BoxShapeParam, BoxShapeParam2, WndImgW, WndImgH, BitCount, WndImgStep, WndImgPtr) == false )
	{	return false; }
	const size_t BufferSize = ImageAPI.CalcBufferSize(WndImgStep, WndImgH);
	if ( JetMemory.alloc_func(BufferSize, ShapeImgPtr, "CAOIWnd::BuildWndShapeMask3", "ShapeImgPtr") == false )
	{
		JetMemory.free_func(WndImgPtr);
		return false;
	}	
	GetWndRegionRes(WndRgn);
	JetAPI::SizeToRect(WndImgW, WndImgH, TempWndRect);	
	::memcpy(ShapeImgPtr, WndImgPtr, sizeof(IMAGE_DATA)*BufferSize);
	for ( i=0; i<MaskWndCount; i++ )
	{
		CAOIWndMask *MaskWndPtr = GetWndMaskWndPtr_Inline(i);
		if ( NULL == MaskWndPtr ) { continue; }
		CAOIBox *MaskBoxPtr=MaskWndPtr->GetWndMaskBoxPtr();
		MaskBoxList.push_back(*MaskBoxPtr);
	}

	const size_t MaskBoxCount=MaskBoxList.size();
	for ( i=0; i<MaskBoxCount; i++ )
	{
		CAOIBox *BoxPtr=&(MaskBoxList[i]);
		if ( NULL == BoxPtr ) { continue; }
		BoxPtr->GetBoxRegionRes(MaskWndRgn);
		if ( JetAPI::CheckRgnInRegion(MaskWndRgn, WndRgn, false) == false )
		{	continue; }
		if ( JetAPI::MapRegionToRect(WndRgn, MaskWndRgn, TempWndRect, MaskWndRect) == false )
		{	continue; }

		bMaskWnd = true;
		bEraseMode = BoxPtr->GetBoxMaskEraseMode();
		BoxImgW = (IMAGE_SIZE)(MaskWndRect.right-MaskWndRect.left);
		BoxImgH = (IMAGE_SIZE)(MaskWndRect.bottom-MaskWndRect.top);
		BoxImgStep = 0;
		BoxToward = BoxPtr->GetBoxToward();
		BoxShapeMode = BoxPtr->GetBoxShapeMode();
		BoxShapeParam = BoxPtr->GetBoxShapeParam();
		BoxShapeParam2= BoxPtr->GetBoxShapeParam2();
		if ( BuildWndShapeImage(bMaskWnd, BoxToward, BoxShapeMode, BoxShapeParam, BoxShapeParam2, BoxImgW, BoxImgH, BitCount, BoxImgStep, BoxImgPtr) == false )
		{	continue; }		
	
		for ( j=MaskWndRect.top; j<MaskWndRect.bottom; j++ )
		{
			imgIdx = j*WndImgStep+MaskWndRect.left-1;//後面直接++, 所以先扣1
			mskIdx = (j-MaskWndRect.top)*BoxImgStep-1;
			for ( k=MaskWndRect.left; k<MaskWndRect.right; k++ )
			{
				imgIdx++;
				mskIdx++;
				if ( 0 == BoxImgPtr[mskIdx] )//遮罩無資料
				{	continue;	}
				if ( 0 == ShapeImgPtr[imgIdx] )//框外型無資料
				{	continue;	}
				if ( false == bEraseMode ) 
				{	WndImgPtr[imgIdx] = 0x00; }
				else
				{	WndImgPtr[imgIdx] = 0xFF; }
			}
		}
		JetMemory.free_func(BoxImgPtr);
	#ifdef _DEBUG		
		if ( FALSE )
		{
			CString str;
			CString ComponentName;
			if ( NULL != ComponentPtr )
			{	ComponentName = ComponentPtr->GetComponentFullName();	}
			str.Format(_T("%s\\%s_WndShapeMaskStep#%d.PNG"), AOIDataCollect.GetAOITempDirectory(), ComponentName, CAOIWnd::GetWndIndex()+1);
			ImageAPI.SavePNGImage(str, WndImgW, WndImgH, WndImgStep, BitCount, WndImgPtr, true);
		}
	#endif//_DEBUG	
	}
	JetMemory.free_func(ShapeImgPtr);
	if ( ImageAPI.PasteGrayRoiImage3(ImageW, ImageH, ImageStep, ImagePtr, WndRect, WndImgStep, WndImgPtr, false, false) == false )
	{	
		JetMemory.free_func(WndImgPtr);
		return false;
	}	
	JetMemory.free_func(WndImgPtr);

#ifdef _DEBUG
	BOOL bSaved = FALSE;
	if ( TRUE == bSaved )
	{
		CString str;
		CString ComponentName;
		if ( NULL != ComponentPtr )
		{	ComponentName = ComponentPtr->GetComponentFullName();	}
		str.Format(_T("%s\\%s_WndShapeMask#%d.PNG"), AOIDataCollect.GetAOITempDirectory(), ComponentName, CAOIWnd::GetWndIndex()+1);
		ImageAPI.SavePNGImage(str, ImageW, ImageH, ImageStep, BitCount, ImagePtr, true);
	}
#endif//_DEBUG	

	return true;
}
//--------------------------------------------------------------------------------------------//
bool CAOIWnd::BuildWndShapeImage(bool bMaskWnd, BOX_TOWARD BoxToward, BOX_SHAPE_MODE BoxShapeMode, double BoxShapeParam, double BoxShapeParam2, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE BitCount, IMAGE_SIZE &ImageStep, IMAGE_PTR &ImagePtr)//建立檢測框-外形遮罩圖像	
{
	if ( 0==ImageW || 0==ImageH || 0==BitCount ) { return false; }	
	CAOIModel *ModelPtr = this->GetWndModelPtr();
	if ( NULL == ModelPtr ) { return false; }
	const char fnName[] = "CAOIWnd::BuildWndShapeImage";
	CAOIComponent *ComponentPtr = ModelPtr->GetModelComponentPtr();

	ImageStep = JetAPI::GetBMPImagePixelsPerLine(ImageW, BitCount, 4);	
	const size_t BufferSize=ImageAPI.CalcBufferSize(ImageStep, ImageH);		
	const unsigned char BKClr   = 0x00;
	const unsigned char FillClr = 0xFF;		

	cv::Mat    Img;
	IplImage  *pImg = NULL;
	const bool bUseMat=true;	
	int        CreateMode=CREATE_OPENCV_IMAGE_NULL;
	if ( JetMemory.alloc_func(BufferSize, ImagePtr, fnName, "ImagePtr") == false )
	{	return false; }

	//Bk Image
	::memset(ImagePtr, BKClr, BufferSize);
	if ( true == bUseMat )
	{
		Img = ImageAPI.CreateMat(ImageW, ImageH, ImageStep, BitCount, ImagePtr);
		if ( ImageAPI.CheckMatIsValid(Img) == false )
		{
			JetMemory.free_func(ImagePtr);
			return false; 
		}		
	}
	else
	{
		if ( 24 == BitCount )
		{	pImg = ImageAPI.ColorImageToCVImage(ImageW, ImageH, ImageStep, ImagePtr, false, CreateMode);	}
		else
		{	pImg = ImageAPI.GrayImageToCVImage(ImageW, ImageH, ImageStep, ImagePtr, false, CreateMode);	}				
		if ( NULL == pImg )
		{
			JetMemory.free_func(ImagePtr);
			return false;	
		}		
	}

	int   Radius=0;
	RECT  MaskRect={0,0,0,0};
	CvScalar color;
	CvScalar bkcolor;

	color.val[0] = FillClr;
	color.val[1] = FillClr;
	color.val[2] = FillClr;
	color.val[3] =   0;		

	bkcolor.val[0] = BKClr;
	bkcolor.val[1] = BKClr;
	bkcolor.val[2] = BKClr;
	bkcolor.val[3] =   0;
	
	JetAPI::SizeToRect(ImageW, ImageH, MaskRect);
	switch ( BoxShapeMode )
	{
	case BOX_SHAPE_ROUND_RECT:
		Radius = CAOIBox::CalcRoundRectRectRadius(MaskRect, BoxShapeParam);
		if ( true == bUseMat )
		{	ImageAPI.DrawBoxImage_RroundRect(Img, color, bkcolor, BoxToward, MaskRect, Radius);	}
		else
		{	ImageAPI.DrawBoxImage_RroundRect(pImg, color, bkcolor, BoxToward, MaskRect, Radius);	}		
		break;
	case BOX_SHAPE_ELLIPSE:
		if ( true == bUseMat )
		{	ImageAPI.DrawBoxImage_Ellipse(Img, color, bkcolor, BoxToward, MaskRect);	}
		else
		{	ImageAPI.DrawBoxImage_Ellipse(pImg, color, bkcolor, BoxToward, MaskRect);	}
		break;
	case BOX_SHAPE_CAPSULE:
		if ( true == bUseMat )
		{	ImageAPI.DrawBoxImage_Capsule(Img, color, bkcolor, BoxToward, MaskRect);	}
		else
		{	ImageAPI.DrawBoxImage_Capsule(pImg, color, bkcolor, BoxToward, MaskRect);	}
		break;
	case BOX_SHAPE_BULLET:
		if ( true == bUseMat )
		{	ImageAPI.DrawBoxImage_Bullet(Img, color, bkcolor, BoxToward, MaskRect);	}
		else
		{	ImageAPI.DrawBoxImage_Bullet(pImg, color, bkcolor, BoxToward, MaskRect);	}
		break;			
	case BOX_SHAPE_HALF_ROUND_RECT:
		Radius = CAOIBox::CalcHalfRoundRectRectRadius(MaskRect, BoxShapeParam, BoxToward);
		if ( true == bUseMat )
		{	ImageAPI.DrawBoxImage_HalfRroundRect(Img, color, bkcolor, BoxToward, MaskRect, Radius);	}
		else
		{	ImageAPI.DrawBoxImage_HalfRroundRect(pImg, color, bkcolor, BoxToward, MaskRect, Radius);	}
		break;
	case BOX_SHAPE_T_SHAPE:
		if ( true == bUseMat )
		{	ImageAPI.DrawBoxImage_TShapeRect(Img, color, bkcolor, BoxToward, MaskRect, BoxShapeParam, BoxShapeParam2);	}
		else
		{	ImageAPI.DrawBoxImage_TShapeRect(pImg, color, bkcolor, BoxToward, MaskRect, BoxShapeParam, BoxShapeParam2);	}		
		break;
	default://BOX_SHAPE_RECTANGLE
		if ( true == bUseMat )
		{	ImageAPI.DrawBoxImage_Rect(Img, color, bkcolor, BoxToward, MaskRect);	}
		else
		{	ImageAPI.DrawBoxImage_Rect(pImg, color, bkcolor, BoxToward, MaskRect);	}
		break;
	}
	//Copy OpenCV to Memory Buffer
	//::memcpy(pPattern, pImg->imageData, pImg->imageSize);//指標派指到自己的記憶體指標, 無需此項複製複製
	if ( true == bUseMat )
	{
		if ( ImageAPI.CloneMatData(Img, ImageW, ImageH, ImageStep, BitCount, ImagePtr) == false )
		{
			JetMemory.free_func(ImagePtr);
			return false;	
		}
	}
	else
	{
		//::cvReleaseImage(&pImg);	
		//::cvReleaseImageHeader(&pImg);//釋放影像[檔頭]記憶體空間
		ImageAPI.DestroyCVImage(CreateMode, pImg);			
	}
#ifdef _DEBUG
	BOOL bSaved = FALSE;
	if ( TRUE == bSaved )
	{
		CString str;
		CString ComponentName;
		if ( NULL != ComponentPtr )
		{	ComponentName = ComponentPtr->GetComponentFullName();	}
		if ( false == bMaskWnd )
		{	str.Format(_T("%s\\%s_WndShapeImage#%d.PNG"), AOIDataCollect.GetAOITempDirectory(), ComponentName, CAOIWnd::GetWndIndex()+1); }
		else
		{	str.Format(_T("%s\\%s_WndMaskShapeImage#%d.PNG"), AOIDataCollect.GetAOITempDirectory(), ComponentName, CAOIWnd::GetWndIndex()+1); }
		ImageAPI.SavePNGImage(str, ImageW, ImageH, ImageStep, BitCount, ImagePtr, true);
	}
#endif//_DEBUG	
	return true;
}
//--------------------------------------------------------------------------------------------//
WND_DEFECT_ID CAOIWnd::ConvertWndDefectID(int val)//將舊的瑕疵代碼轉成新的瑕疵代碼
{
	WND_DEFECT_ID WndDefectID=WND_DEFECT_NONE;
	switch ( val )
	{
	case 0x00000000: WndDefectID=WND_DEFECT_NONE; break;
	case 0x00000001: WndDefectID=WND_DEFECT_PAD_ALIGN; break;
	case 0x00000002: WndDefectID=WND_DEFECT_PART_ALIGN; break;
	case 0x00000004: WndDefectID=WND_DEFECT_PAD_ADJUST; break;
	case 0x00000008: WndDefectID=WND_DEFECT_LEAD_ADJUST; break;

	case 0x00000010: WndDefectID=WND_DEFECT_BODY_MISSING; break;
	case 0x00000020: WndDefectID=WND_DEFECT_BODY_OFFSET; break;
	case 0x00000040: WndDefectID=WND_DEFECT_BODY_TILT; break;
	case 0x00000080: WndDefectID=WND_DEFECT_BODY_POLARITY; break;
	case 0x00000100: WndDefectID=WND_DEFECT_BODY_TURNOVER; break;
	case 0x00000200: WndDefectID=WND_DEFECT_BODY_MOUNT; break;
	case 0x00000400: WndDefectID=WND_DEFECT_BODY_WRONG_CODE; break;
	case 0x00000800: WndDefectID=WND_DEFECT_BODY_WRONG_TEXT; break;

	case 0x00001000: WndDefectID=WND_DEFECT_SOLDER_POOR; break;
	case 0x00002000: WndDefectID=WND_DEFECT_SOLDER_OPEN; break;
	case 0x00004000: WndDefectID=WND_DEFECT_SOLDER_PAD_EXPOSED; break;
	case 0x00008000: WndDefectID=WND_DEFECT_SOLDER_BRIDGE; break;
	case 0x00010000: WndDefectID=WND_DEFECT_SOLDER_BEAD; break;

	case 0x00020000: WndDefectID=WND_DEFECT_LEAD_LIFTED; break;
	case 0x00040000: WndDefectID=WND_DEFECT_LEAD_BENDED; break;
	case 0x00080000: WndDefectID=WND_DEFECT_LEAD_PROTRUDED; break;

	case 0x00100000: WndDefectID=WND_DEFECT_PAD_SCRATCH; break;
	case 0x00200000: WndDefectID=WND_DEFECT_FOREIGN_BODY; break;	

	case 0x00400000: WndDefectID=WND_DEFECT_USER_DEFINE_10; break;	
	case 0x00800000: WndDefectID=WND_DEFECT_USER_DEFINE_09; break;
	case 0x01000000: WndDefectID=WND_DEFECT_USER_DEFINE_08; break;	
	case 0x02000000: WndDefectID=WND_DEFECT_USER_DEFINE_07; break;
	case 0x04000000: WndDefectID=WND_DEFECT_USER_DEFINE_06; break;
	case 0x08000000: WndDefectID=WND_DEFECT_USER_DEFINE_05; break;
	case 0x10000000: WndDefectID=WND_DEFECT_USER_DEFINE_04; break;
	case 0x20000000: WndDefectID=WND_DEFECT_USER_DEFINE_03; break;
	case 0x40000000: WndDefectID=WND_DEFECT_USER_DEFINE_02; break;
	case 0x80000000: WndDefectID=WND_DEFECT_USER_DEFINE_01; break;
	}
	return WndDefectID;
}
//--------------------------------------------------------------------------------------------//
bool CAOIWnd::GetWndTestTrackFile() const//是否儲存檢測框檢測追蹤檔案
{	
	if ( FN_ENABLE != AOIDataCollect.GetSystemParameter().m_SaveTestTrackFile )
	{	return false; }
	return true;
}
//--------------------------------------------------------------------------------------------//
bool CAOIWnd::SaveWndTestTrackFile()//儲存檢測框檢測追蹤檔案
{
	m_WndTestTrackFilename = _T("");
	if ( GetWndTestTrackFile() == false ) { return true; }

	CAOIModel *ModelPtr=GetWndModelPtr();
	if ( NULL == ModelPtr ) { return false; }		
	CString        AttachedName;	
	CAOIProject   *ProjectPtr=NULL;
	CAOIFd        *FdPtr=NULL;
	CAOIBarcode   *BarcodePtr=NULL;
	CAOIComponent *ComponentPtr=NULL;
	AOI_OBJ_TYPE  AttachedType=ModelPtr->GetModelAttachedType();
	switch ( AttachedType )
	{
	case AOI_OBJ_FD:		
		FdPtr = ModelPtr->GetModelFdPtr();		
		if ( NULL != FdPtr )
		{	
			ProjectPtr = FdPtr->GetFdProjectPtr();
			AttachedName = FdPtr->GetFdFullName();				
		}
		break;
	case AOI_OBJ_BARCODE:	
		BarcodePtr = ModelPtr->GetModelBarcodePtr();
		if ( NULL != BarcodePtr )
		{	
			ProjectPtr = BarcodePtr->GetBarcodeProjectPtr();
			AttachedName = BarcodePtr->GetBarcodeFullName();				
		}
		break;		
	case AOI_OBJ_COMPONENT:	
		ComponentPtr = ModelPtr->GetModelComponentPtr();
		if ( NULL != ComponentPtr )
		{
			ProjectPtr = ComponentPtr->GetComponentProjectPtr();
			AttachedName = ComponentPtr->GetComponentFullName();	
		}
		break;
	}
	if ( AttachedName.GetLength()==0 || NULL==ProjectPtr )
	{	return false; }
	CString strFolder = ProjectPtr->GetProjectTestTrackFolder();	
	if ( strFolder.GetLength() == 0 )
	{	return false; }

	CString str;	
	const unsigned int WndIndex=GetWndIndex();	
	CString AlgName=AOIDataDefine.GetAlgTypeText(GetWndAlgType());
	CString DefectName=AOIDataDefine.GetWndDefectIDText(GetWndDefectID());	
	str.Format(_T("%s\\%s_Wnd#%04d[%s][%s].TXT"), strFolder, AttachedName, WndIndex+1, DefectName, AlgName);
	FILE *pfile = ::_tfopen(str, _T("w+"));
	if ( NULL == pfile )
	{	return false; }
	_ftprintf(pfile, _T("%s_Wnd#%04d[%s][%s]"), AttachedName, WndIndex+1, DefectName, AlgName);
	::fclose(pfile);	pfile=NULL;
	m_WndTestTrackFilename = str;
	return true;
}
//--------------------------------------------------------------------------------------------//
bool CAOIWnd::DeleteWndTestTrackFile()//刪除檢測框檢測追蹤檔案
{
	if ( GetWndTestTrackFile() == false ) { return true; }
	if ( m_WndTestTrackFilename.GetLength() == 0 ) { return true; }
	::DeleteFile(m_WndTestTrackFilename);	
	m_WndTestTrackFilename=_T("");
	return true;
}
//--------------------------------------------------------------------------------------------//
bool CAOIWnd::DumpWndException(LPCTSTR Info, bool bDumped)//輸出檢測框異常訊息
{
	CAOIWnd *WndPtr = this;
	if ( NULL == WndPtr ) { return false; }

	FILE *pfile = NULL;
	CString Filename;
	TCHAR fileMode[32]=_T("a+");
	AOIDataCollect.LockGlobal();
	
	JetAPI::ModifyOpenFileMode_Write(fileMode);	
	Filename.Format(_T("%s\\%s"), AOIDataCollect.GetAOILogDirectory(), _T("WndExcpetion.TXT"));
	pfile = ::_tfopen(Filename, fileMode);
	if ( NULL != pfile ) 
	{ 	
		if ( false == bDumped )
		{
			CString str, str2;
			CAOIModel *ModelPtr = NULL;
			CAOIComponent *ComponentPtr = NULL;		
			ModelPtr = WndPtr->GetWndModelPtr();
			const CAlgParam &AlgParam = WndPtr->GetWndAlgParam();

			str = _T("====================================================================");
			::_ftprintf(pfile, _T("%s\n"), str);

			if ( NULL != ModelPtr )
			{	ComponentPtr = ModelPtr->GetModelComponentPtr();	}

			if ( NULL != ComponentPtr )
			{
				str = ComponentPtr->GetComponentFullName();
				::_ftprintf(pfile, _T("ComponentName:%s\n"), str);
			}
			::_ftprintf(pfile, _T("Wnd Index:%d\n"), WndPtr->GetWndIndex()+1);

			str = AOIDataDefine.GetAlgTypeText(AlgParam.GetAlgType());
			::_ftprintf(pfile, _T("Alg Type:%s\n"), str);

			str = AOIDataDefine.GetWndDefectIDText(WndPtr->GetWndDefectID());
			::_ftprintf(pfile, _T("Wnd Defect ID:%s\n"), str);
		
			if ( ALG_AI_MODEL_NONE == AlgParam.GetAlgParamAiModel().aiModelID )
			{	str = _T("false");	}
			else
			{	str = _T("true");	}
			::_ftprintf(pfile, _T("AI Enabled:%s\n"), str);		

			str = WndPtr->GetWndResultText();
			::_ftprintf(pfile, _T("Text:%s\n"), str);
		}
		if ( NULL != Info )
		{	::_ftprintf(pfile, _T("Error:%s\n"), Info);	}
		::fclose(pfile); pfile = NULL;
	}
	AOIDataCollect.UnlockGlobal();
	return true;
}
//--------------------------------------------------------------------------------------------//
bool CAOIWnd::UpdateWndFrameUniqueID(unsigned int DefaultIndex, unsigned int DefaultUniqueID, const std::vector<unsigned int> &FrameIndexMapList)
{	
	size_t i=0;	
	CAOIWndRoi  *WndRoiPtr = NULL;
	CAlgBinaryParam *BinParamPtr = NULL;
	const size_t WndRoiCount = GetWndRoiWndCount();

	GetWndAlgParam().UpdateAlgFrameUniqueID(DefaultIndex, DefaultUniqueID, FrameIndexMapList);
	for ( i=0; i<WndRoiCount; i++ )
	{
		WndRoiPtr = GetWndRoiWndPtr(i, false);
		if ( NULL == WndRoiPtr ) { continue; }
		BinParamPtr = WndRoiPtr->GetWndRoiBinaryParamPtr();
		BinParamPtr->UpdateBinaryFrameUniqueID(DefaultIndex, DefaultUniqueID, FrameIndexMapList);
	}
	return true;
}
//--------------------------------------------------------------------------------------------//
bool CAOIWnd::CloneWndResult(CAOIWnd *SrcWndPtr)
{
	if ( NULL == SrcWndPtr ) { return false; }
	CAlgParam &AlgParam=GetWndAlgParam();
	const CAlgParam &SrcAlgParam=SrcWndPtr->GetWndAlgParam();

	AlgParam = SrcAlgParam;
	AlgParam.SetAlgWndPtr(this);

	CloneWndResultBoxList(SrcWndPtr);	
	SetWndResultID(SrcWndPtr->GetWndResultID());
	SetWndResultText(SrcWndPtr->GetWndResultText());
	SetWndResultValue(SrcWndPtr->GetWndResultValue());
	SetWndLogicResultID(SrcWndPtr->GetWndLogicResultID());			
	return true;
}
//--------------------------------------------------------------------------------------------//
CString CAOIWnd::GetWndFullName() const//取得零件全名
{
	CString Name = _T("Wnd");
	CAOIModel *ModelPtr = GetWndModelPtr();
	if ( NULL == ModelPtr) { return Name; }	
	CAOIRgn *AttachedPtr=ModelPtr->GetModelAttachedPtr();
	if ( NULL == AttachedPtr ) { return Name; }
	
	CAOIFd            *FdPtr = NULL;
	CAOIMark          *MarkPtr = NULL;
	CAOIBarcode       *BarcodePtr = NULL;
	CAOIComponent     *ComponentPtr = NULL;
	const unsigned int WndIndex = GetWndIndex();
	const AOI_OBJ_TYPE AttachedType=AttachedPtr->GetObjType();
	const unsigned int BoardIndex=AttachedPtr->GetRgnBoardIndex_Panel();
	const unsigned int PanelIndex=AttachedPtr->GetRgnPanelIndex_Project();	
	

	CString AttachedName;
	switch ( AttachedType )
	{
	case AOI_OBJ_FD:	
		FdPtr=dynamic_cast<CAOIFd*>(AttachedPtr);		
		if ( NULL != FdPtr )
		{	AttachedName = AOIDataDefine.GetFdFullName(FdPtr->GetFdIndex_Project(), _T("Fd"));	}
		break;
	case AOI_OBJ_MARK:	
		MarkPtr=dynamic_cast<CAOIMark*>(AttachedPtr);
		if ( NULL != MarkPtr )
		{	AttachedName = AOIDataDefine.GetMarkFullName(MarkPtr->GetMarkIndex_Project(), _T("Mark"));	}
		break;
	case AOI_OBJ_BARCODE:	
		BarcodePtr=dynamic_cast<CAOIBarcode*>(AttachedPtr);
		if ( NULL != BarcodePtr )
		{	AttachedName = AOIDataDefine.GetBarcodeFullName(BarcodePtr->GetBarcodeIndex_Project(), _T("Barcode"));	}
		break;
	default:
	case AOI_OBJ_COMPONENT:	
		ComponentPtr=dynamic_cast<CAOIComponent*>(AttachedPtr);
		if ( NULL != ComponentPtr )
		{	AttachedName = AOIDataDefine.GetComponentFullName(PanelIndex, BoardIndex, ComponentPtr->GetComponentName());	}
		break;
	}	
	if ( AttachedName.GetLength() == 0 )
	{	return Name;	}

	CString WndIndexName=AOIDataDefine.GetWndIndexText(WndIndex);
	Name.Format(_T("%s_%s"), AttachedName, WndIndexName);	
	return Name;
}
//--------------------------------------------------------------------------------------------//