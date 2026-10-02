// AOIComponent.cpp: implementation of the CAOIComponent class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "AOIComponent.h"
//-------------------------------------------------------------------------------------//
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
IMPLEMENT_DYNAMIC(CAOIComponent, CAOIRgn)
//-------------------------------------------------------------------------------------//
CAOIComponent::CAOIComponent():CAOIRgn(AOI_OBJ_COMPONENT)
{	
	PreInitComponent();
	InitialComponent();
}
//-------------------------------------------------------------------------------------//
CAOIComponent::CAOIComponent(const CAOIComponent &Component):CAOIRgn(Component)
{
	PreInitComponent();
	CloneComponent(Component);
}
//-------------------------------------------------------------------------------------//
CAOIComponent::~CAOIComponent()
{	
}
//-------------------------------------------------------------------------------------//
CAOIComponent& CAOIComponent::operator=(const CAOIComponent &Component)
{
	if ( this == &Component ) { return *this; }
	CAOIRgn::operator=(Component);
	CloneComponent(Component);
	return *this;
}
//-------------------------------------------------------------------------------------//
inline void CAOIComponent::PreInitComponent()
{	
	m_ComponentName.clear();
	m_ComponentModelName.clear();
	m_ComponentPartNumber.clear();
	m_ComponentNozzleName.clear();
	m_ComponentTempText.clear();
}
//-------------------------------------------------------------------------------------//
inline void CAOIComponent::InitialComponent()
{
	//---------------------------------------------------------------------------------//
	//CAOIRgn::InitialRgn();//CAOIRgn建構子會自動呼叫
	//---------------------------------------------------------------------------------//
	m_ComponentUniqueID = -1;
	m_ComponentModelIndex = -1;
	m_ComponentModelClassID = MODEL_CLASS_ID_NONE;
	m_ComponentModelIsolated = false;
	m_ComponentModel.SetModelComponentPtr(this);
	m_ComponentModel.SetModelIsolated(false);
	//---------------------------------------------------------------------------------//
	m_ComponentDeleted = false;//零件是否刪除
	m_ComponentSelected = false;//零件是否選取到	
	m_ComponentVisibled = true;//零件是否顯示	
	m_ComponentXBoardUnit = false;//零件是否X板單位
	m_ComponentNeedToUpdate = false;//零件是否需要重新計算	
	m_ComponentColIndex = -1;
	m_ComponentRowIndex = -1;
	m_ComponentType = COMPONENT_TYPE_NORMAL;		
	m_ComponentOrgCadPos = TPOINT2D();	
	m_ComponentConfirmUIResultID = 0;
	m_ComponentSaveWndList = false;
	SetComponentSaveTestImageMode(SAVE_TEST_IMAGE_DEFECT);
	//---------------------------------------------------------------------------------//
	m_ComponentVersionParamList.resize(PROJECT_VERSION_CODE_SIZE, TVersionParam());	
	m_ComponentVersionParamListBackup.resize(PROJECT_VERSION_CODE_SIZE, TVersionParam());	
	//---------------------------------------------------------------------------------//
	m_ComponentName = L"NewPart";
	m_ComponentModelName, L"Model";	
	m_ComponentPartNumber = L"PartNumber";
	m_ComponentNozzleName=L"Unset";
	//---------------------------------------------------------------------------------//
	m_ComponentSaveReport_ARS = true;
	//---------------------------------------------------------------------------------//
	m_ComponentTempText=L"";
	//---------------------------------------------------------------------------------//		
	m_ComponentTempIndex = -1;
	::memset(m_ComponentTempInt, 0x00, sizeof(m_ComponentTempInt));
	//---------------------------------------------------------------------------------//
	m_ComponentTempIndexModel = -1;
	m_ComponentTempIndexPartNumber = -1;		
	//---------------------------------------------------------------------------------//
	m_ComponentTop10IndexModel = -1;
	m_ComponentTop10IndexModel_LA = -1;
	m_ComponentTop10IndexModel_LB = -1;
	m_ComponentTop10IndexPartNumber = -1;
	m_ComponentTop10IndexPartNumber_LA = -1;
	m_ComponentTop10IndexPartNumber_LB = -1;
	//---------------------------------------------------------------------------------//
	m_ComponentResultWidth=0.0;
	m_ComponentResultLength=0.0;
	m_ComponentResultHeight=0.0;
	m_ComponentResultArea=0.0;
	m_ComponentResultVolume=0.0;
	m_ComponentResultHeightMin=0.0;
	m_ComponentResultHeightMax=0.0;
	//---------------------------------------------------------------------------------//
	m_ComponentResultOffsetX = 0.0;
	m_ComponentResultOffsetY = 0.0;
	m_ComponentResultOffsetA = 0.0;
	m_ComponentResultSkewAngle = 0.0;
	m_ComponentResultTiltAngle = 0.0;
	m_ComponentResultOffsetX_LA = 0.0;
	m_ComponentResultOffsetY_LA = 0.0;
	m_ComponentResultSkewAngle_LA = 0.0;
	m_ComponentResultTiltAngle_LA = 0.0;
	m_ComponentResultOffsetX_LB = 0.0;
	m_ComponentResultOffsetY_LB = 0.0;
	m_ComponentResultSkewAngle_LB = 0.0;
	m_ComponentResultTiltAngle_LB = 0.0;
	//---------------------------------------------------------------------------------//	
	m_ComponentResultWidth_USL = 0.0;
	m_ComponentResultWidth_LSL = 0.0;
	m_ComponentResultWidth_WndIdx = -1;
	m_ComponentResultLength_USL = 0.0;
	m_ComponentResultLength_LSL = 0.0;
	m_ComponentResultLength_WndIdx = -1;
	m_ComponentResultHeight_USL = 0.0;
	m_ComponentResultHeight_LSL = 0.0;
	m_ComponentResultHeight_WndIdx = -1;
	m_ComponentResultArea_USL = 0.0;
	m_ComponentResultArea_LSL = 0.0;
	m_ComponentResultArea_WndIdx = -1;
	m_ComponentResultVolume_USL = 0.0;
	m_ComponentResultVolume_LSL = 0.0;
	m_ComponentResultVolume_WndIdx = -1;
	m_ComponentResultOffsetX_USL = 0.0;
	m_ComponentResultOffsetX_LSL = 0.0;
	m_ComponentResultOffsetX_WndIdx = -1;
	m_ComponentResultOffsetY_USL = 0.0;
	m_ComponentResultOffsetY_LSL = 0.0;
	m_ComponentResultOffsetY_WndIdx = -1;
	m_ComponentResultOffsetA_USL = 0.0;
	m_ComponentResultOffsetA_LSL = 0.0;
	m_ComponentResultOffsetA_WndIdx = -1;
	m_ComponentResultSkewAngle_USL = 0.0;
	m_ComponentResultSkewAngle_LSL = 0.0;
	m_ComponentResultSkewAngle_WndIdx = -1;
	m_ComponentResultTiltAngle_USL = 0.0;
	m_ComponentResultTiltAngle_LSL = 0.0;
	m_ComponentResultTiltAngle_WndIdx = -1;
	//---------------------------------------------------------------------------------//	
	m_ComponentGrrOffsetX = 0;
	m_ComponentGrrOffsetY = 0;
	m_ComponentGrrSkewA = 0;
	m_ComponentGrrBodyHeight = 0;
	m_ComponentGrrSigmaItemIdx = -1;	
	m_ComponentGrrSigmaItemList.clear();
	m_ComponentGrrSigmaItemNull = TGrrSigmaItem();
	m_ComponentGrrSigmaItemList.push_back(TGrrSigmaItem());
	//---------------------------------------------------------------------------------//
	//TSigmaItem                 m_ComponentResultSigmaItem_LA;//零件檢測標準差項目 
	//TSigmaItem                 m_ComponentResultSigmaItem_LB;//零件檢測標準差項目 
	//---------------------------------------------------------------------------------//		
	m_ComponentCadResultX = 0.0;
	m_ComponentCadResultY = 0.0;
	//---------------------------------------------------------------------------------//
	m_ComponentStageResultX = 0.0;
	m_ComponentStageResultY = 0.0;
	//---------------------------------------------------------------------------------//	
	m_ComponentStageOffsetX = 0.0;
	m_ComponentStageOffsetY = 0.0;
	//---------------------------------------------------------------------------------//
	m_ComponentMaskExtendW_Body = 200.0;
	m_ComponentMaskExtendH_Body = 200.0;
	m_ComponentMaskExtendW_Land = 0.0;
	m_ComponentMaskExtendH_Land = 0.0;
	//---------------------------------------------------------------------------------//	
	m_ComponentGroupID = 0;
	m_ComponentGroupOrg = false;	
	m_ComponentGroupOffsetResX = 0;
	m_ComponentGroupOffsetResY = 0;
	m_ComponentGroupOffsetMaxX = 0;
	m_ComponentGroupOffsetMaxY = 0;
	m_ComponentGroupOffsetMinX = 0;
	m_ComponentGroupOffsetMinY = 0;	
	m_ComponentGroupDistanceResX = 0;
	m_ComponentGroupDistanceResY = 0;
	//---------------------------------------------------------------------------------//
	m_ComponentEnableAlarm=true;		
	m_ComponentDefectCountEnableOnARS=false;
	m_ComponentDefectAlarmEnableOnAOI=true;
	m_ComponentDefectAlarmEnableOnARS=false;
	m_ComponentDefectAlarmResultOnAOI = false;
	m_ComponentDefectAlarmResultOnARS = false;
	m_ComponentDefectItemAlarmAOI=CWndDefectItem();
	m_ComponentDefectItemAlarmARS=CWndDefectItem();
	m_ComponentDefectAlarmFromModeAOI=DEFECT_PARAM_FROM_PROJECT;
	m_ComponentDefectAlarmFromModeARS=DEFECT_PARAM_FROM_PROJECT;
	m_ComponentTotalTestCountAOI = 0;
	m_ComponentTotalTestCountAOI_LA = 0;
	m_ComponentTotalTestCountAOI_LB = 0;
	m_ComponentTotalTestCountARS = 0;
	m_ComponentTotalTestCountARS_LA = 0;
	m_ComponentTotalTestCountARS_LB = 0;
	m_ComponentTotalNGCountAOI = 0;
	m_ComponentTotalNGCountAOI_LA = 0;
	m_ComponentTotalNGCountAOI_LB = 0;
	m_ComponentTotalNGCountARS = 0;	
	m_ComponentTotalNGCountARS_LA = 0;
	m_ComponentTotalNGCountARS_LB = 0;	
	m_ComponentTotalNGCountLimit = 0;	
	m_ComponentTotalNGCountEnable = true;
	m_ComponentTotalNGCountAlarm = false;
	m_ComponentContinueNGCountAOI = 0;
	m_ComponentContinueNGCountARS = 0;		
	m_ComponentContinueNGCountLimit = 0;
	m_ComponentContinueNGCountEnable = true;
	m_ComponentContinueNGCountAlarm = false;
	m_ComponentResultListAOI.clear();
	m_ComponentResultListARS.clear();	
	m_ComponentCurrentDefectCountAOI=CWndDefectItem();
	m_ComponentCurrentDefectCountARS=CWndDefectItem();
	m_ComponentTotaEachlDefectCountAOI=CWndDefectItem();
	m_ComponentTotaEachlDefectCountAOI_LA =CWndDefectItem();
	m_ComponentTotaEachlDefectCountAOI_LB =CWndDefectItem();	
	m_ComponentTotalEachDefectCountARS=CWndDefectItem();	
	m_ComponentTotalEachDefectCountARS_LA=CWndDefectItem();	
	m_ComponentTotalEachDefectCountARS_LB=CWndDefectItem();	
	m_ComponentTotaEachlDefectCountAOI_LA.SetLaneID(LANE_ID_A);
	m_ComponentTotaEachlDefectCountAOI_LB.SetLaneID(LANE_ID_B);
	m_ComponentTotalEachDefectCountARS_LA.SetLaneID(LANE_ID_A);
	m_ComponentTotalEachDefectCountARS_LB.SetLaneID(LANE_ID_B);
	//---------------------------------------------------------------------------------//
	m_NPM_APC_MffX = 0;
	m_NPM_APC_MffY = 0;
	m_NPM_APC_MffA = 0;
	m_NPM_APC_FF1_IDNUM = -1;
	m_NPM_APC_EPosX = 0;
	m_NPM_APC_EPosY = 0;
	m_NPM_APC_EPosA = 0;
	m_NPM_APC_MFB_IDNUM = -1;
	//---------------------------------------------------------------------------------//	
	m_HASI_SPIOffset_Enable = true;
	m_HASI_SPIOffset_IsApplied = false;
	m_HASI_SaveImage = false;
	//---------------------------------------------------------------------------------//	
	m_ComponentResultPtr = this;
	m_ComponentMasterPtr = NULL;
	m_ComponentMasterIndex = -1;
	m_ComponentAgentIndex = -1;
	m_ComponentAgentList.clear();
	//---------------------------------------------------------------------------------//
}
//-------------------------------------------------------------------------------------//
inline void CAOIComponent::CloneComponent(const CAOIComponent &Component)
{
	//---------------------------------------------------------------------------------//
	CAOIRgn::CloneRgn(Component);
	//---------------------------------------------------------------------------------//
	m_ComponentUniqueID = Component.m_ComponentUniqueID;
	m_ComponentDeleted = Component.m_ComponentDeleted;//零件是否刪除
	m_ComponentSelected = Component.m_ComponentSelected;//零件是否選取到	
	m_ComponentVisibled = Component.m_ComponentVisibled;//零件是否顯示	
	m_ComponentXBoardUnit = Component.m_ComponentXBoardUnit;//零件是否X板單位	
	m_ComponentNeedToUpdate = Component.m_ComponentNeedToUpdate;//零件是否需要重新計算	
	m_ComponentColIndex = Component.m_ComponentColIndex;
	m_ComponentRowIndex = Component.m_ComponentRowIndex;	
	m_ComponentType = Component.m_ComponentType;		
	m_ComponentOrgCadPos = Component.m_ComponentOrgCadPos;	
	m_ComponentConfirmUIResultID = Component.m_ComponentConfirmUIResultID;
	//m_ComponentSpecialCadPos = Component.m_ComponentSpecialCadPos;
	//---------------------------------------------------------------------------------//
	m_ComponentVersionParamList = Component.m_ComponentVersionParamList;
	m_ComponentVersionParamListBackup = Component.m_ComponentVersionParamListBackup;
	//---------------------------------------------------------------------------------//
	m_ComponentName = Component.m_ComponentName;	
	m_ComponentModelName = Component.m_ComponentModelName;
	m_ComponentPartNumber = Component.m_ComponentPartNumber;
	m_ComponentNozzleName = Component.m_ComponentNozzleName;
	//---------------------------------------------------------------------------------//
	m_ComponentSaveReport_ARS = Component.m_ComponentSaveReport_ARS;
	//---------------------------------------------------------------------------------//
	m_ComponentTempText = Component.m_ComponentTempText;
	//---------------------------------------------------------------------------------//	
	m_ComponentTempIndex = Component.m_ComponentTempIndex;
	::memcpy(m_ComponentTempInt, Component.m_ComponentTempInt, sizeof(m_ComponentTempInt));
	//---------------------------------------------------------------------------------//
	m_ComponentTempIndexModel = Component.m_ComponentTempIndexModel;
	m_ComponentTempIndexPartNumber = Component.m_ComponentTempIndexPartNumber;	
	//---------------------------------------------------------------------------------//
	m_ComponentTop10IndexModel = Component.m_ComponentTop10IndexModel;
	m_ComponentTop10IndexModel_LA = Component.m_ComponentTop10IndexModel_LA;
	m_ComponentTop10IndexModel_LB = Component.m_ComponentTop10IndexModel_LB;
	m_ComponentTop10IndexPartNumber = Component.m_ComponentTop10IndexPartNumber;	
	m_ComponentTop10IndexPartNumber_LA = Component.m_ComponentTop10IndexPartNumber_LA;
	m_ComponentTop10IndexPartNumber_LB = Component.m_ComponentTop10IndexPartNumber_LB;
	//---------------------------------------------------------------------------------//	
	m_ComponentModel = Component.m_ComponentModel;
	m_ComponentModelIndex = Component.m_ComponentModelIndex;
	m_ComponentModelClassID = Component.m_ComponentModelClassID;	
	m_ComponentModelIsolated = Component.m_ComponentModelIsolated;			
	m_ComponentModel.SetModelComponentPtr(this);
	//---------------------------------------------------------------------------------//	
	CloneComponentWindow(Component);	
	m_ComponentSaveWndList = Component.m_ComponentSaveWndList;
	//---------------------------------------------------------------------------------//
	m_ComponentResultWidth = Component.m_ComponentResultWidth;
	m_ComponentResultLength = Component.m_ComponentResultLength;
	m_ComponentResultHeight = Component.m_ComponentResultHeight;
	m_ComponentResultArea = Component.m_ComponentResultArea;
	m_ComponentResultVolume = Component.m_ComponentResultVolume;
	m_ComponentResultHeightMin=Component.m_ComponentResultHeightMin;
	m_ComponentResultHeightMax=Component.m_ComponentResultHeightMax;
	//---------------------------------------------------------------------------------//	 
	m_ComponentResultOffsetX = Component.m_ComponentResultOffsetX;
	m_ComponentResultOffsetY = Component.m_ComponentResultOffsetY;	
	m_ComponentResultOffsetA = Component.m_ComponentResultOffsetA;		
	m_ComponentResultSkewAngle = Component.m_ComponentResultSkewAngle;
	m_ComponentResultTiltAngle = Component.m_ComponentResultTiltAngle;	
	m_ComponentResultOffsetX_LA = Component.m_ComponentResultOffsetX_LA;
	m_ComponentResultOffsetY_LA = Component.m_ComponentResultOffsetY_LA;
	m_ComponentResultSkewAngle_LA = Component.m_ComponentResultSkewAngle_LA;
	m_ComponentResultTiltAngle_LA = Component.m_ComponentResultTiltAngle_LA;	
	m_ComponentResultOffsetX_LB = Component.m_ComponentResultOffsetX_LB;
	m_ComponentResultOffsetY_LB = Component.m_ComponentResultOffsetY_LB;
	m_ComponentResultSkewAngle_LB = Component.m_ComponentResultSkewAngle_LB;
	m_ComponentResultTiltAngle_LB = Component.m_ComponentResultTiltAngle_LB;
	//---------------------------------------------------------------------------------//		
	m_ComponentResultWidth_USL = Component.m_ComponentResultWidth_USL;
	m_ComponentResultWidth_LSL = Component.m_ComponentResultWidth_LSL;
	m_ComponentResultWidth_WndIdx = Component.m_ComponentResultWidth_WndIdx;
	m_ComponentResultLength_USL = Component.m_ComponentResultLength_USL;
	m_ComponentResultLength_LSL = Component.m_ComponentResultLength_LSL;
	m_ComponentResultLength_WndIdx = Component.m_ComponentResultLength_WndIdx;
	m_ComponentResultHeight_USL = Component.m_ComponentResultHeight_USL;
	m_ComponentResultHeight_LSL = Component.m_ComponentResultHeight_LSL;
	m_ComponentResultHeight_WndIdx = Component.m_ComponentResultHeight_WndIdx;
	m_ComponentResultArea_USL = Component.m_ComponentResultArea_USL;
	m_ComponentResultArea_LSL = Component.m_ComponentResultArea_LSL;
	m_ComponentResultArea_WndIdx = Component.m_ComponentResultArea_WndIdx;
	m_ComponentResultVolume_USL = Component.m_ComponentResultVolume_USL;
	m_ComponentResultVolume_LSL = Component.m_ComponentResultVolume_LSL;
	m_ComponentResultVolume_WndIdx = Component.m_ComponentResultVolume_WndIdx;
	m_ComponentResultOffsetX_USL = Component.m_ComponentResultOffsetX_USL;
	m_ComponentResultOffsetX_LSL = Component.m_ComponentResultOffsetX_LSL;
	m_ComponentResultOffsetX_WndIdx = Component.m_ComponentResultOffsetX_WndIdx;
	m_ComponentResultOffsetY_USL = Component.m_ComponentResultOffsetY_USL;
	m_ComponentResultOffsetY_LSL = Component.m_ComponentResultOffsetY_LSL;
	m_ComponentResultOffsetY_WndIdx = Component.m_ComponentResultOffsetY_WndIdx;
	m_ComponentResultOffsetA_USL = Component.m_ComponentResultOffsetA_USL;
	m_ComponentResultOffsetA_LSL = Component.m_ComponentResultOffsetA_LSL;
	m_ComponentResultOffsetA_WndIdx = Component.m_ComponentResultOffsetA_WndIdx;
	m_ComponentResultSkewAngle_USL = Component.m_ComponentResultSkewAngle_USL;
	m_ComponentResultSkewAngle_LSL = Component.m_ComponentResultSkewAngle_LSL;
	m_ComponentResultSkewAngle_WndIdx = Component.m_ComponentResultSkewAngle_WndIdx;
	m_ComponentResultTiltAngle_USL = Component.m_ComponentResultTiltAngle_USL;
	m_ComponentResultTiltAngle_LSL = Component.m_ComponentResultTiltAngle_LSL;
	m_ComponentResultTiltAngle_WndIdx = Component.m_ComponentResultTiltAngle_WndIdx;	
	//---------------------------------------------------------------------------------//		 
	m_ComponentGrrOffsetX = Component.m_ComponentGrrOffsetX;
	m_ComponentGrrOffsetY = Component.m_ComponentGrrOffsetY;
	m_ComponentGrrSkewA = Component.m_ComponentGrrSkewA;
	m_ComponentGrrBodyHeight = Component.m_ComponentGrrBodyHeight;
	m_ComponentGrrSigmaItemIdx = Component.m_ComponentGrrSigmaItemIdx;
	m_ComponentGrrSigmaItemNull = Component.m_ComponentGrrSigmaItemNull;
	m_ComponentGrrSigmaItemList = Component.m_ComponentGrrSigmaItemList;
	//---------------------------------------------------------------------------------//
	m_ComponentResultSigmaItem_LA = Component.m_ComponentResultSigmaItem_LA;
	m_ComponentResultSigmaItem_LB = Component.m_ComponentResultSigmaItem_LB;	
	//---------------------------------------------------------------------------------//	
	m_ComponentCadResultX = Component.m_ComponentCadResultX;
	m_ComponentCadResultY = Component.m_ComponentCadResultY;
	//---------------------------------------------------------------------------------//
	m_ComponentStageResultX = Component.m_ComponentStageResultX;
	m_ComponentStageResultY = Component.m_ComponentStageResultY;
	//---------------------------------------------------------------------------------//
	m_ComponentStageOffsetX = Component.m_ComponentStageOffsetX;
	m_ComponentStageOffsetY = Component.m_ComponentStageOffsetY;
	//---------------------------------------------------------------------------------//
	m_ComponentMaskExtendW_Body = Component.m_ComponentMaskExtendW_Body;
	m_ComponentMaskExtendH_Body = Component.m_ComponentMaskExtendH_Body;
	m_ComponentMaskExtendW_Land = Component.m_ComponentMaskExtendW_Land;
	m_ComponentMaskExtendH_Land = Component.m_ComponentMaskExtendH_Land;
	//---------------------------------------------------------------------------------//	
	m_ComponentGroupID = Component.m_ComponentGroupID;
	m_ComponentGroupOrg = Component.m_ComponentGroupOrg;	
	m_ComponentGroupOffsetResX = Component.m_ComponentGroupOffsetResX;
	m_ComponentGroupOffsetResY = Component.m_ComponentGroupOffsetResY;
	m_ComponentGroupOffsetMaxX = Component.m_ComponentGroupOffsetMaxX;
	m_ComponentGroupOffsetMaxY = Component.m_ComponentGroupOffsetMaxY;
	m_ComponentGroupOffsetMinX = Component.m_ComponentGroupOffsetMinX;
	m_ComponentGroupOffsetMinY = Component.m_ComponentGroupOffsetMinY;	
	m_ComponentGroupDistanceResX = Component.m_ComponentGroupDistanceResX;
	m_ComponentGroupDistanceResY = Component.m_ComponentGroupDistanceResY;	
	//---------------------------------------------------------------------------------//
	m_ComponentEnableAlarm = Component.m_ComponentEnableAlarm;		
	m_ComponentDefectCountEnableOnARS = Component.m_ComponentDefectCountEnableOnARS;
	m_ComponentDefectAlarmEnableOnAOI = Component.m_ComponentDefectAlarmEnableOnAOI;		
	m_ComponentDefectAlarmEnableOnARS = Component.m_ComponentDefectAlarmEnableOnARS;	
	m_ComponentDefectAlarmResultOnAOI = Component.m_ComponentDefectAlarmResultOnAOI;
	m_ComponentDefectAlarmResultOnARS = Component.m_ComponentDefectAlarmResultOnARS;
	m_ComponentDefectItemAlarmAOI = Component.m_ComponentDefectItemAlarmAOI;
	m_ComponentDefectItemAlarmARS = Component.m_ComponentDefectItemAlarmARS;
	m_ComponentDefectAlarmFromModeAOI = Component.m_ComponentDefectAlarmFromModeAOI;
	m_ComponentDefectAlarmFromModeARS = Component.m_ComponentDefectAlarmFromModeARS;
	m_ComponentTotalTestCountAOI = Component.m_ComponentTotalTestCountAOI;
	m_ComponentTotalTestCountAOI_LA = Component.m_ComponentTotalTestCountAOI_LA;
	m_ComponentTotalTestCountAOI_LB = Component.m_ComponentTotalTestCountAOI_LB;
	m_ComponentTotalTestCountARS = Component.m_ComponentTotalTestCountARS;
	m_ComponentTotalTestCountARS_LA = Component.m_ComponentTotalTestCountARS_LA;
	m_ComponentTotalTestCountARS_LB = Component.m_ComponentTotalTestCountARS_LB;	
	m_ComponentTotalNGCountAOI = Component.m_ComponentTotalNGCountAOI;
	m_ComponentTotalNGCountAOI_LA = Component.m_ComponentTotalNGCountAOI_LA;
	m_ComponentTotalNGCountAOI_LB = Component.m_ComponentTotalNGCountAOI_LB;
	m_ComponentTotalNGCountARS = Component.m_ComponentTotalNGCountARS;
	m_ComponentTotalNGCountARS_LA = Component.m_ComponentTotalNGCountARS_LA;
	m_ComponentTotalNGCountARS_LB = Component.m_ComponentTotalNGCountARS_LB;	
	m_ComponentTotalNGCountLimit = Component.m_ComponentTotalNGCountLimit;
	m_ComponentTotalNGCountEnable = Component.m_ComponentTotalNGCountEnable;
	m_ComponentTotalNGCountAlarm = Component.m_ComponentTotalNGCountAlarm;		
	m_ComponentContinueNGCountAOI = Component.m_ComponentContinueNGCountAOI;
	m_ComponentContinueNGCountARS = Component.m_ComponentContinueNGCountARS;		
	m_ComponentContinueNGCountLimit = Component.m_ComponentContinueNGCountLimit;
	m_ComponentContinueNGCountEnable = Component.m_ComponentContinueNGCountEnable;
	m_ComponentContinueNGCountAlarm = Component.m_ComponentContinueNGCountAlarm;
	m_ComponentCurrentDefectCountAOI = Component.m_ComponentCurrentDefectCountAOI;
	m_ComponentCurrentDefectCountARS = Component.m_ComponentCurrentDefectCountARS;		
	m_ComponentTotaEachlDefectCountAOI = Component.m_ComponentTotaEachlDefectCountAOI;
	m_ComponentTotaEachlDefectCountAOI_LA = Component.m_ComponentTotaEachlDefectCountAOI_LA;
	m_ComponentTotaEachlDefectCountAOI_LB = Component.m_ComponentTotaEachlDefectCountAOI_LB;	
	m_ComponentTotalEachDefectCountARS = Component.m_ComponentTotalEachDefectCountARS;	
	m_ComponentTotalEachDefectCountARS_LA = Component.m_ComponentTotalEachDefectCountARS_LA;	
	m_ComponentTotalEachDefectCountARS_LB = Component.m_ComponentTotalEachDefectCountARS_LB;	
	m_ComponentResultListAOI = Component.m_ComponentResultListAOI;
	m_ComponentResultListARS = Component.m_ComponentResultListARS;
	//---------------------------------------------------------------------------------//
	m_NPM_APC_MffX = Component.m_NPM_APC_MffX;
	m_NPM_APC_MffY = Component.m_NPM_APC_MffY;
	m_NPM_APC_MffA = Component.m_NPM_APC_MffA;
	m_NPM_APC_FF1_IDNUM = Component.m_NPM_APC_FF1_IDNUM;
	m_NPM_APC_EPosX = Component.m_NPM_APC_EPosX;
	m_NPM_APC_EPosY = Component.m_NPM_APC_EPosY;
	m_NPM_APC_EPosA = Component.m_NPM_APC_EPosA;
	m_NPM_APC_MFB_IDNUM = Component.m_NPM_APC_MFB_IDNUM;
	//---------------------------------------------------------------------------------//
	m_ComponentResultPtr = this;	
	m_ComponentMasterPtr = Component.m_ComponentMasterPtr;
	m_ComponentMasterIndex = Component.m_ComponentMasterIndex;	
	m_ComponentAgentIndex = Component.m_ComponentAgentIndex;
	m_ComponentAgentList = Component.m_ComponentAgentList;
	//---------------------------------------------------------------------------------//
	m_HASI_SPIOffset_Enable = Component.m_HASI_SPIOffset_Enable;
	m_HASI_SaveImage = Component.m_HASI_SaveImage;
	//m_HASI_SPIOffset_IsApplied = Component.m_HASI_SPIOffset_IsApplied;
	//---------------------------------------------------------------------------------//
}
//-------------------------------------------------------------------------------------//
inline void CAOIComponent::CloneComponentWindow(const CAOIComponent &Component)
{
	//CAOIComponent::ClearComponentWin
	//是否改成指標列表
	this->m_ComponentWindowList = Component.m_ComponentWindowList;
}
//-------------------------------------------------------------------------------------//
CAOIComponent* CAOIComponent::CloneComponentObj() const
{
	CAOIComponent *ObjPtr = AOIObjManager.CreateComponentObj();
	if ( NULL == ObjPtr ) { return NULL; }
	ObjPtr->operator=(*this);
	return ObjPtr;
}
//-------------------------------------------------------------------------------------//
inline size_t CAOIComponent::GetComponentWindowCount_Inline() const
{
	return CAOIComponent::m_ComponentWindowList.size();
}
//-------------------------------------------------------------------------------------//
inline void CAOIComponent::AddComponentWindow_Inline(const CAOIWindow &Window)
{
	CAOIComponent::m_ComponentWindowList.push_back(Window);
}
//-------------------------------------------------------------------------------------//
inline CAOIWindow* CAOIComponent::GetComponentWindowPtr_Inline(size_t index)
{	
	return &(CAOIComponent::m_ComponentWindowList[index]);
}
//-------------------------------------------------------------------------------------//
CString CAOIComponent::GetRgnDerivedName() const//取得區域的名稱
{
	return CAOIComponent::GetComponentFullName();
}
//-------------------------------------------------------------------------------------//
CString CAOIComponent::GetRgnDerivedKeyName() const//取得零件的名稱
{
	return CString(m_ComponentName.c_str());
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentName(const char* value)
{
	if ( NULL == value ) { return; }	
	JetAPI::char2wstring(value, m_ComponentName);	
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentName(const wchar_t* value)
{
	if ( NULL == value ) { return; }
	m_ComponentName = value;
}
//-------------------------------------------------------------------------------------//
CString CAOIComponent::GetComponentFullName() const//取得零件全名
{	
	CString ComName = m_ComponentName.c_str();
	unsigned int PanelIndex = CAOIRgn::GetRgnPanelIndex_Project();
	unsigned int BoardIndex = CAOIRgn::GetRgnBoardIndex_Panel();
	return AOIDataDefine.GetComponentFullName(PanelIndex, BoardIndex, ComName);
}
//-------------------------------------------------------------------------------------//
CString CAOIComponent::GetComponentShowName() const//取得零件顯示名
{
	CString Name=GetComponentName();
	if ( CheckComponentIsAgent() == true )
	{
		CString Key;
		CString Tmp=Name;
		CString PN=GetComponentPartNumber();		
		Key.Format(_T("%02d"), GetComponentAgentIndex()+1);
		if ( Key.GetLength() != 0 )
		{	Name.Format(_T("%s<%s>"), Tmp, Key);	}
	}
	return Name;
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::ChangeComponentName(LPCTSTR Name)//變更零件名稱
{
	if ( CheckComponentIsMasterOrAgent() == true )	
	{	ChangeComponentMasterName(Name);	return;	}	
	SetComponentName(Name);
	return;
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentModelName(const char* value)
{
	if ( NULL==value ) { return; }
	JetAPI::char2wstring(value, m_ComponentModelName);
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentModelName(const wchar_t* value)
{
	if ( NULL==value ) { return; }
	m_ComponentModelName = value;	
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentPartNumber(const char* value)
{
	if ( NULL == value ) { return; }	
	JetAPI::char2wstring(value, m_ComponentPartNumber);		
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentPartNumber(const wchar_t* value)
{
	if ( NULL == value ) { return; }
	m_ComponentPartNumber=value;	
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentNozzleName(const char* value)
{
	if ( NULL == value ) { return; }
	JetAPI::char2wstring(value, m_ComponentNozzleName);	
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentNozzleName(const wchar_t* value)
{
	if ( NULL == value ) { return; }
	m_ComponentNozzleName=value;	
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentTempText(const char* value)
{
	if ( NULL == value ) { return; }
	JetAPI::char2wstring(value, m_ComponentTempText);
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentTempText(const wchar_t* value)
{
	if ( NULL == value ) { return; }
	m_ComponentTempText = value;
}
//-------------------------------------------------------------------------------------//
size_t CAOIComponent::GetComponentTop10IndexModel_Lane(LANE_ID LaneID) const
{
	if ( LANE_ID_B == LaneID )
	{	return GetComponentTop10IndexModel_LB(); }
	return GetComponentTop10IndexModel_LA();
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentTop10IndexModel_Lane(LANE_ID LaneID, size_t index)
{
	switch ( LaneID )
	{
	case LANE_ID_A:	SetComponentTop10IndexModel_LA(index); break;
	case LANE_ID_B:	SetComponentTop10IndexModel_LB(index); break;
	}
}
//-------------------------------------------------------------------------------------//
size_t CAOIComponent::GetComponentTop10IndexPartNumber_Lane(LANE_ID LaneID) const
{
	if ( LANE_ID_B == LaneID )
	{	return GetComponentTop10IndexPartNumber_LB(); }
	return GetComponentTop10IndexPartNumber_LA();
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentTop10IndexPartNumber_Lane(LANE_ID LaneID, size_t index)
{
	switch ( LaneID )
	{
	case LANE_ID_A:	SetComponentTop10IndexPartNumber_LA(index); break;
	case LANE_ID_B:	SetComponentTop10IndexPartNumber_LB(index); break;
	}
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::AddComponentToTop10ModeList(std::vector<TTop10Node> &Top10List, bool DefectAOI)
{
	size_t  j=0;
	size_t  DefectCount = 0;
	TTop10Node *Top10NodelPtr=NULL;
	CAOIComponent *ComponentPtr = this;	
	const size_t Top10Count=Top10List.size();
	size_t Index = ComponentPtr->GetComponentTop10IndexModel();	
	if ( false == DefectAOI )
	{	DefectCount = ComponentPtr->GetComponentTotalNGCountARS();	}
	else
	{	DefectCount = ComponentPtr->GetComponentTotalNGCountAOI();	}
	if ( Index < Top10Count )
	{
		Top10NodelPtr = &(Top10List[Index]);			
		if ( false == DefectAOI )
		{	Top10NodelPtr->DefectCountARS += DefectCount;	}
		else
		{	Top10NodelPtr->DefectCountAOI += DefectCount; }
		return true;
	}
	
	CString ModelName = ComponentPtr->GetComponentModelName();
	for ( j=0; j<Top10Count; j++ )
	{
		Top10NodelPtr = &(Top10List[j]);
		if ( ModelName.CompareNoCase(Top10NodelPtr->NodeName) == 0 )
		{	break; }
	}
	if ( j == Top10Count )
	{
		TTop10Node NewTop10Node = TTop10Node();
		NewTop10Node.NodeName = ComponentPtr->GetComponentModelName();
		if ( false == DefectAOI )
		{	NewTop10Node.DefectCountARS = DefectCount;	}
		else
		{	NewTop10Node.DefectCountAOI = DefectCount; }
		NewTop10Node.StatisticsIndex = Top10Count;
		Top10List.push_back(NewTop10Node);		
		ComponentPtr->SetComponentTop10IndexModel(NewTop10Node.StatisticsIndex);
	}
	else
	{
		ComponentPtr->SetComponentTop10IndexModel(j);
		if ( false == DefectAOI )
		{	Top10NodelPtr->DefectCountARS += DefectCount;	}
		else
		{	Top10NodelPtr->DefectCountAOI += DefectCount; }
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::AddComponentToTop10ModeList_Lane(LANE_ID LaneID, std::vector<TTop10Node> &Top10List, bool DefectAOI)
{
	size_t  j=0;
	size_t  DefectCount = 0;
	TTop10Node *Top10NodelPtr=NULL;
	CAOIComponent *ComponentPtr = this;	
	const size_t Top10Count=Top10List.size();
	size_t Index = ComponentPtr->GetComponentTop10IndexModel_Lane(LaneID);	
	if ( false == DefectAOI )
	{	DefectCount = ComponentPtr->GetComponentTotalNGCountARS_Lane(LaneID);	}
	else
	{	DefectCount = ComponentPtr->GetComponentTotalNGCountAOI_Lane(LaneID);	}
	if ( Index < Top10Count )
	{
		Top10NodelPtr = &(Top10List[Index]);			
		if ( false == DefectAOI )
		{	Top10NodelPtr->DefectCountARS += DefectCount;	}
		else
		{	Top10NodelPtr->DefectCountAOI += DefectCount; }		
		return true;
	}

	CString ModelName = ComponentPtr->GetComponentModelName();
	for ( j=0; j<Top10Count; j++ )
	{
		Top10NodelPtr = &(Top10List[j]);
		if ( ModelName.CompareNoCase(Top10NodelPtr->NodeName) == 0 )
		{	break; }
	}
	if ( j == Top10Count )
	{
		TTop10Node NewTop10Node = TTop10Node();
		NewTop10Node.NodeName = ComponentPtr->GetComponentModelName();
		if ( false == DefectAOI )
		{	NewTop10Node.DefectCountARS = DefectCount;	}
		else
		{	NewTop10Node.DefectCountAOI = DefectCount; }
		NewTop10Node.StatisticsIndex = Top10Count;
		Top10List.push_back(NewTop10Node);		
		ComponentPtr->SetComponentTop10IndexModel_Lane(LaneID, NewTop10Node.StatisticsIndex);
	}
	else
	{
		ComponentPtr->SetComponentTop10IndexModel_Lane(LaneID, j);
		if ( false == DefectAOI )
		{	Top10NodelPtr->DefectCountARS += DefectCount;	}
		else
		{	Top10NodelPtr->DefectCountAOI += DefectCount; }
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::AddComponentToTop10PartNumberList(std::vector<TTop10Node> &Top10List, bool DefectAOI)
{
	size_t  j=0;
	size_t  DefectCount = 0;
	TTop10Node *Top10NodelPtr=NULL;
	CAOIComponent *ComponentPtr = this;	
	const size_t Top10Count=Top10List.size();
	size_t Index = ComponentPtr->GetComponentTop10IndexPartNumber();	
	if ( false == DefectAOI )
	{	DefectCount = ComponentPtr->GetComponentTotalNGCountARS();	}
	else
	{	DefectCount = ComponentPtr->GetComponentTotalNGCountAOI();	}
	if ( Index < Top10Count )
	{
		Top10NodelPtr = &(Top10List[Index]);			
		if ( false == DefectAOI )
		{	Top10NodelPtr->DefectCountARS += DefectCount;	}
		else
		{	Top10NodelPtr->DefectCountAOI += DefectCount; }
		return true;
	}
	
	CString ModelName = ComponentPtr->GetComponentPartNumber();
	for ( j=0; j<Top10Count; j++ )
	{
		Top10NodelPtr = &(Top10List[j]);
		if ( ModelName.CompareNoCase(Top10NodelPtr->NodeName) == 0 )
		{	break; }
	}
	if ( j == Top10Count )
	{
		TTop10Node NewTop10Node = TTop10Node();
		NewTop10Node.NodeName = ComponentPtr->GetComponentPartNumber();
		if ( false == DefectAOI )
		{	NewTop10Node.DefectCountARS = DefectCount;	}
		else
		{	NewTop10Node.DefectCountAOI = DefectCount; }
		NewTop10Node.StatisticsIndex = Top10Count;
		Top10List.push_back(NewTop10Node);		
		ComponentPtr->SetComponentTop10IndexPartNumber(NewTop10Node.StatisticsIndex);
	}
	else
	{
		ComponentPtr->SetComponentTop10IndexPartNumber(j);
		if ( false == DefectAOI )
		{	Top10NodelPtr->DefectCountARS += DefectCount;	}
		else
		{	Top10NodelPtr->DefectCountAOI += DefectCount; }
	}	
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::AddComponentToTop10PartNumberList_Lane(LANE_ID LaneID, std::vector<TTop10Node> &Top10List, bool DefectAOI)
{
	size_t  j=0;
	size_t  DefectCount = 0;
	TTop10Node *Top10NodelPtr=NULL;
	CAOIComponent *ComponentPtr = this;	
	const size_t Top10Count=Top10List.size();
	size_t Index = ComponentPtr->GetComponentTop10IndexPartNumber_Lane(LaneID);	
	if ( false == DefectAOI )
	{	DefectCount = ComponentPtr->GetComponentTotalNGCountARS_Lane(LaneID);	}
	else
	{	DefectCount = ComponentPtr->GetComponentTotalNGCountAOI_Lane(LaneID);	}
	if ( Index < Top10Count )
	{
		Top10NodelPtr = &(Top10List[Index]);			
		if ( false == DefectAOI )
		{	Top10NodelPtr->DefectCountARS += DefectCount;	}
		else
		{	Top10NodelPtr->DefectCountAOI += DefectCount; }
		return true;
	}

	CString ModelName = ComponentPtr->GetComponentPartNumber();
	for ( j=0; j<Top10Count; j++ )
	{
		Top10NodelPtr = &(Top10List[j]);
		if ( ModelName.CompareNoCase(Top10NodelPtr->NodeName) == 0 )
		{	break; }
	}
	if ( j == Top10Count )
	{
		TTop10Node NewTop10Node = TTop10Node();
		NewTop10Node.NodeName = ComponentPtr->GetComponentPartNumber();
		if ( false == DefectAOI )
		{	NewTop10Node.DefectCountARS = DefectCount;	}
		else
		{	NewTop10Node.DefectCountAOI = DefectCount; }
		NewTop10Node.StatisticsIndex = Top10Count;
		Top10List.push_back(NewTop10Node);		
		ComponentPtr->SetComponentTop10IndexPartNumber_Lane(LaneID, NewTop10Node.StatisticsIndex);
	}
	else
	{
		ComponentPtr->SetComponentTop10IndexPartNumber_Lane(LaneID, j);
		if ( false == DefectAOI )
		{	Top10NodelPtr->DefectCountARS += DefectCount;	}
		else
		{	Top10NodelPtr->DefectCountAOI += DefectCount; }
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::CreateComponentSelfFieldPtr()//建立專屬Field指標	
{
	bool bIsOK = true;
	bIsOK = CAOIRgn::CreateRgnSelfFieldPtr();
	return bIsOK;
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentFieldPtr(CAOIField *Ptr)
{
	CAOIRgn::SetRgnFieldPtr(Ptr);
}
//-------------------------------------------------------------------------------------//
LANE_ID CAOIComponent::GetComponentLaneID() const
{
	return CAOIRgn::GetRgnLaneID();
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentLaneID(LANE_ID value)
{
	CAOIRgn::SetRgnLaneID(value);
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentBypassed(bool value)
{
	CAOIRgn::SetRgnBypassed(value);
}
//-------------------------------------------------------------------------------------//
size_t CAOIComponent::GetComponentWindowCount() const
{
	return GetComponentWindowCount_Inline();	
}
//-------------------------------------------------------------------------------------//
CAOIWindow* CAOIComponent::GetComponentWindowPtr(size_t index, bool Check)
{
	if ( Check )
	{
		const size_t Count = CAOIComponent::GetComponentWindowCount_Inline();
		if ( index >= Count ) 
		{	return NULL; }
	}
	return GetComponentWindowPtr_Inline(index);	
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::AddComponentWindow(CAOIWindow &Window)//增加零件的檢測框
{
	const unsigned int Index = (unsigned int)(CAOIComponent::GetComponentWindowCount_Inline());
	CAOIComponent::AddComponentWindow_Inline(Window);	
	CAOIWindow *pWindow = CAOIComponent::GetComponentWindowPtr_Inline(Index);
	pWindow->SetWindowIndexComponent(Index);
	pWindow->SetWindowComponentIndex(GetComponentIndex_Project());
	pWindow->SetWindowComponentPtr(this);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::SelectComponentAllWindows(bool Select)//選取零件的檢測框
{
	size_t i=0;
	CAOIWindow *pWindow = NULL;
	const size_t WindowCount = this->GetComponentWindowCount_Inline();
	for ( i=0; i<WindowCount; i++ )
	{
		pWindow = this->GetComponentWindowPtr_Inline(i);
		if ( NULL == pWindow ) { continue; }
		pWindow->SetWindowSelected(Select);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::DeleteComponentWindowSelected()//移除選取到的零件的檢測框
{
	size_t i=0;
	unsigned int idx=0;
	CAOIWindow *pWindow = NULL;
	std::vector<CAOIWindow>    ComponentWindowList = m_ComponentWindowList;
	const size_t WindowCount = ComponentWindowList.size();

	idx=0;
	m_ComponentWindowList.clear();
	for ( i=0; i<WindowCount; i++ )
	{
		pWindow = &(ComponentWindowList[i]);
		if ( NULL == pWindow ) { continue; }
		if ( true == pWindow->GetWindowSelected() ) { continue; }
		if ( true == pWindow->GetWindowDeleted() ) { continue; }

		pWindow->SetWindowIndexComponent(idx);
		m_ComponentWindowList.push_back(ComponentWindowList[i]);		
		idx ++;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::DeleteComponentAllWindows()//移除零件的檢測框
{
	m_ComponentWindowList.clear();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::LayoutComponentWindowList()//重整零件的檢測框列表
{
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::RestoreComponentCadPos(CMapCoordinate *MapPtr)//恢復零件Cad座標
{
	double dX = m_ComponentOrgCadPos.x-m_RgnCadPos.x;
	double dY = m_ComponentOrgCadPos.y-m_RgnCadPos.y;
	
	//m_RgnCadBiasPos.x = 0;
	//m_RgnCadBiasPos.y = 0;
	const bool bMoveOrg = false;
	MoveComponentPos(dX, dY, MapPtr, bMoveOrg);
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::MoveComponentCadPos(double dX, double dY)//移動零件座標	
{
	MoveRgnCadPos(dX, dY);	
	m_ComponentModel.SetModelAttachedPosCad(m_RgnCadPos);
}
//-------------------------------------------------------------------------------------//
void  CAOIComponent::MoveComponentStagePos(double dX, double dY)//移動零件座標	
{
	CAOIRgn::MoveRgnStagePos(dX, dY);
	m_ComponentModel.SetModelAttachedPosStage(m_RgnStagePos.x, m_RgnStagePos.y);
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::MoveComponentOrgCadPos(double dX, double dY)//移動零件原始座標	
{
	TPOINT2D CadPos = GetComponentOrgCadPos();	
	CadPos.x += dX;	
	CadPos.y += dY;
	SetComponentOrgCadPos(CadPos);
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::MoveComponentPos(double dX, double dY, CMapCoordinate *MapPtr, bool MoveOrg)//移動零件座標
{
	MoveComponentCadPos(dX, dY);
	if ( true == MoveOrg )
	{	MoveComponentOrgCadPos(dX, dY);	}
	if ( NULL != MapPtr )
	{
		MapComponentCadToStagePos(*MapPtr);
		m_ComponentModel.SetModelAttachedPosStage(m_RgnStagePos.x, m_RgnStagePos.y);
	}
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::GetComponentRoiCadRegion(TREGION4D &Region)//取得零件在Cad的範圍	
{
	Region.minX = Region.maxX = CAOIRgn::m_RgnRoiCadCornerPos[0].x;
	Region.minY = Region.maxY = CAOIRgn::m_RgnRoiCadCornerPos[0].y;

	if ( Region.minX > CAOIRgn::m_RgnRoiCadCornerPos[1].x ) { Region.minX = CAOIRgn::m_RgnRoiCadCornerPos[1].x; }
	if ( Region.minY > CAOIRgn::m_RgnRoiCadCornerPos[1].y ) { Region.minY = CAOIRgn::m_RgnRoiCadCornerPos[1].y; }
	if ( Region.maxX < CAOIRgn::m_RgnRoiCadCornerPos[1].x ) { Region.maxX = CAOIRgn::m_RgnRoiCadCornerPos[1].x; }
	if ( Region.maxY < CAOIRgn::m_RgnRoiCadCornerPos[1].y ) { Region.maxY = CAOIRgn::m_RgnRoiCadCornerPos[1].y; }

	if ( Region.minX > CAOIRgn::m_RgnRoiCadCornerPos[2].x ) { Region.minX = CAOIRgn::m_RgnRoiCadCornerPos[2].x; }
	if ( Region.minY > CAOIRgn::m_RgnRoiCadCornerPos[2].y ) { Region.minY = CAOIRgn::m_RgnRoiCadCornerPos[2].y; }
	if ( Region.maxX < CAOIRgn::m_RgnRoiCadCornerPos[2].x ) { Region.maxX = CAOIRgn::m_RgnRoiCadCornerPos[2].x; }
	if ( Region.maxY < CAOIRgn::m_RgnRoiCadCornerPos[2].y ) { Region.maxY = CAOIRgn::m_RgnRoiCadCornerPos[2].y; }

	if ( Region.minX > CAOIRgn::m_RgnRoiCadCornerPos[3].x ) { Region.minX = CAOIRgn::m_RgnRoiCadCornerPos[3].x; }
	if ( Region.minY > CAOIRgn::m_RgnRoiCadCornerPos[3].y ) { Region.minY = CAOIRgn::m_RgnRoiCadCornerPos[3].y; }
	if ( Region.maxX < CAOIRgn::m_RgnRoiCadCornerPos[3].x ) { Region.maxX = CAOIRgn::m_RgnRoiCadCornerPos[3].x; }
	if ( Region.maxY < CAOIRgn::m_RgnRoiCadCornerPos[3].y ) { Region.maxY = CAOIRgn::m_RgnRoiCadCornerPos[3].y; }
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::GetComponentBodyCadRegion(TREGION4D &Region)//取得零件在Cad的範圍	
{	
	Region.minX = Region.maxX = CAOIRgn::m_RgnBodyCadCornerPos[0].x;
	Region.minY = Region.maxY = CAOIRgn::m_RgnBodyCadCornerPos[0].y;

	if ( Region.minX > CAOIRgn::m_RgnBodyCadCornerPos[1].x ) { Region.minX = CAOIRgn::m_RgnBodyCadCornerPos[1].x; }
	if ( Region.minY > CAOIRgn::m_RgnBodyCadCornerPos[1].y ) { Region.minY = CAOIRgn::m_RgnBodyCadCornerPos[1].y; }
	if ( Region.maxX < CAOIRgn::m_RgnBodyCadCornerPos[1].x ) { Region.maxX = CAOIRgn::m_RgnBodyCadCornerPos[1].x; }
	if ( Region.maxY < CAOIRgn::m_RgnBodyCadCornerPos[1].y ) { Region.maxY = CAOIRgn::m_RgnBodyCadCornerPos[1].y; }

	if ( Region.minX > CAOIRgn::m_RgnBodyCadCornerPos[2].x ) { Region.minX = CAOIRgn::m_RgnBodyCadCornerPos[2].x; }
	if ( Region.minY > CAOIRgn::m_RgnBodyCadCornerPos[2].y ) { Region.minY = CAOIRgn::m_RgnBodyCadCornerPos[2].y; }
	if ( Region.maxX < CAOIRgn::m_RgnBodyCadCornerPos[2].x ) { Region.maxX = CAOIRgn::m_RgnBodyCadCornerPos[2].x; }
	if ( Region.maxY < CAOIRgn::m_RgnBodyCadCornerPos[2].y ) { Region.maxY = CAOIRgn::m_RgnBodyCadCornerPos[2].y; }

	if ( Region.minX > CAOIRgn::m_RgnBodyCadCornerPos[3].x ) { Region.minX = CAOIRgn::m_RgnBodyCadCornerPos[3].x; }
	if ( Region.minY > CAOIRgn::m_RgnBodyCadCornerPos[3].y ) { Region.minY = CAOIRgn::m_RgnBodyCadCornerPos[3].y; }
	if ( Region.maxX < CAOIRgn::m_RgnBodyCadCornerPos[3].x ) { Region.maxX = CAOIRgn::m_RgnBodyCadCornerPos[3].x; }
	if ( Region.maxY < CAOIRgn::m_RgnBodyCadCornerPos[3].y ) { Region.maxY = CAOIRgn::m_RgnBodyCadCornerPos[3].y; }
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::GetComponentRoiStageRegion(TREGION4D &Region) const//取得零件在Stage的範圍
{
	//m_ComponentModel.GetModelTotalRegionStage(Region);
	//return ;
	Region.minX = Region.maxX = CAOIRgn::m_RgnRoiStageCornerPos[0].x;
	Region.minY = Region.maxY = CAOIRgn::m_RgnRoiStageCornerPos[0].y;

	if ( Region.minX > CAOIRgn::m_RgnRoiStageCornerPos[1].x ) { Region.minX = CAOIRgn::m_RgnRoiStageCornerPos[1].x; }
	if ( Region.minY > CAOIRgn::m_RgnRoiStageCornerPos[1].y ) { Region.minY = CAOIRgn::m_RgnRoiStageCornerPos[1].y; }
	if ( Region.maxX < CAOIRgn::m_RgnRoiStageCornerPos[1].x ) { Region.maxX = CAOIRgn::m_RgnRoiStageCornerPos[1].x; }
	if ( Region.maxY < CAOIRgn::m_RgnRoiStageCornerPos[1].y ) { Region.maxY = CAOIRgn::m_RgnRoiStageCornerPos[1].y; }

	if ( Region.minX > CAOIRgn::m_RgnRoiStageCornerPos[2].x ) { Region.minX = CAOIRgn::m_RgnRoiStageCornerPos[2].x; }
	if ( Region.minY > CAOIRgn::m_RgnRoiStageCornerPos[2].y ) { Region.minY = CAOIRgn::m_RgnRoiStageCornerPos[2].y; }
	if ( Region.maxX < CAOIRgn::m_RgnRoiStageCornerPos[2].x ) { Region.maxX = CAOIRgn::m_RgnRoiStageCornerPos[2].x; }
	if ( Region.maxY < CAOIRgn::m_RgnRoiStageCornerPos[2].y ) { Region.maxY = CAOIRgn::m_RgnRoiStageCornerPos[2].y; }

	if ( Region.minX > CAOIRgn::m_RgnRoiStageCornerPos[3].x ) { Region.minX = CAOIRgn::m_RgnRoiStageCornerPos[3].x; }
	if ( Region.minY > CAOIRgn::m_RgnRoiStageCornerPos[3].y ) { Region.minY = CAOIRgn::m_RgnRoiStageCornerPos[3].y; }
	if ( Region.maxX < CAOIRgn::m_RgnRoiStageCornerPos[3].x ) { Region.maxX = CAOIRgn::m_RgnRoiStageCornerPos[3].x; }
	if ( Region.maxY < CAOIRgn::m_RgnRoiStageCornerPos[3].y ) { Region.maxY = CAOIRgn::m_RgnRoiStageCornerPos[3].y; }
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::GetComponentBodyStageRegion(TREGION4D &Region) const//取得零件本體在Stage的範圍	
{
	Region.minX = Region.maxX = CAOIRgn::m_RgnBodyStageCornerPos[0].x;
	Region.minY = Region.maxY = CAOIRgn::m_RgnBodyStageCornerPos[0].y;

	if ( Region.minX > CAOIRgn::m_RgnBodyStageCornerPos[1].x ) { Region.minX = CAOIRgn::m_RgnBodyStageCornerPos[1].x; }
	if ( Region.minY > CAOIRgn::m_RgnBodyStageCornerPos[1].y ) { Region.minY = CAOIRgn::m_RgnBodyStageCornerPos[1].y; }
	if ( Region.maxX < CAOIRgn::m_RgnBodyStageCornerPos[1].x ) { Region.maxX = CAOIRgn::m_RgnBodyStageCornerPos[1].x; }
	if ( Region.maxY < CAOIRgn::m_RgnBodyStageCornerPos[1].y ) { Region.maxY = CAOIRgn::m_RgnBodyStageCornerPos[1].y; }

	if ( Region.minX > CAOIRgn::m_RgnBodyStageCornerPos[2].x ) { Region.minX = CAOIRgn::m_RgnBodyStageCornerPos[2].x; }
	if ( Region.minY > CAOIRgn::m_RgnBodyStageCornerPos[2].y ) { Region.minY = CAOIRgn::m_RgnBodyStageCornerPos[2].y; }
	if ( Region.maxX < CAOIRgn::m_RgnBodyStageCornerPos[2].x ) { Region.maxX = CAOIRgn::m_RgnBodyStageCornerPos[2].x; }
	if ( Region.maxY < CAOIRgn::m_RgnBodyStageCornerPos[2].y ) { Region.maxY = CAOIRgn::m_RgnBodyStageCornerPos[2].y; }

	if ( Region.minX > CAOIRgn::m_RgnBodyStageCornerPos[3].x ) { Region.minX = CAOIRgn::m_RgnBodyStageCornerPos[3].x; }
	if ( Region.minY > CAOIRgn::m_RgnBodyStageCornerPos[3].y ) { Region.minY = CAOIRgn::m_RgnBodyStageCornerPos[3].y; }
	if ( Region.maxX < CAOIRgn::m_RgnBodyStageCornerPos[3].x ) { Region.maxX = CAOIRgn::m_RgnBodyStageCornerPos[3].x; }
	if ( Region.maxY < CAOIRgn::m_RgnBodyStageCornerPos[3].y ) { Region.maxY = CAOIRgn::m_RgnBodyStageCornerPos[3].y; }
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::CalcComponentCadCornerPos()//計算零件Cad端點座標	
{
	CAOIRgn::CalcRgnCadCornerPos();	
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::LayoutComponentStageCornerPos()//更新零件機台端點座標	
{
	CAOIRgn::LayoutRgnStageCornerPos();	
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::MapComponentCadToStagePos(const CMapCoordinate &Map)//將零件CAD轉成機台座標
{
	CAOIRgn::MapRgnCadToStagePos(Map);	
	const double SagePosX = CAOIRgn::GetRgnStagePosX();
	const double SagePosY = CAOIRgn::GetRgnStagePosY();
	CAOIComponent::m_ComponentModel.SetModelAttachedPosStage(SagePosX, SagePosY);
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::MapComponentStageResultToCadPos(const CMapCoordinate &Map)//將零件機台結果座標轉成Cad座標
{	
	double CadResX=0;
	double CadResY=0;
	const double SagePosX = GetComponentStagePosX();
	const double SagePosY = GetComponentStagePosY();
	const double SageResPosX = GetComponentStageResultX();
	const double SageResPosY = GetComponentStageResultY();
	Map.Map2D(SageResPosX, SageResPosY, CadResX, CadResY);
	double CadResXOld=GetComponentCadResultX();
	double CadResYOld=GetComponentCadResultY();
	double CadResGapX=CadResX-CadResXOld;
	double CadResGapY=CadResY-CadResYOld;
	if ( fabs(CadResGapX)>1 || fabs(CadResGapY)>1 )
	{
		CadResGapX = CadResGapX;

	}
	SetComponentCadResultX(CadResX);
	SetComponentCadResultY(CadResY);
	return;
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentFrameImageSize_um(const TSIZE2D &value)
{ 	
	CAOIRgn::SetRgnFrameImageSize_um(value);
	m_ComponentModel.SetModelImageSize_um(value);
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentFrameImageCadOffset_um(const TPOINT2D &value)
{
	CAOIRgn::SetRgnFrameImageCadOffset_um(value);
	m_ComponentModel.SetModelImageCadOffset_um(value);
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentFrameImageStageOffset_um(const TPOINT2D &value)
{
	TPOINT2D CadOffset;
	AOIDataCollect.MapStageOffsetPtToCad(value, CadOffset);
	SetComponentFrameImageCadOffset_um(CadOffset);
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::SpinComponent(double Angle, CMapCoordinate *MapPtr)//零件自旋轉
{
	double CadAngle = 0;
	double StageAngle = 0;	
	CadAngle = Angle;
	AOIDataCollect.MapCadAngleToStage(CadAngle, StageAngle);

	const double CadCpX = CAOIRgn::m_RgnCadPos.x;
	const double CadCpY = CAOIRgn::m_RgnCadPos.y;
	const double StageCpX = CAOIRgn::m_RgnStagePos.x;
	const double StageCpY = CAOIRgn::m_RgnStagePos.y;
	RotateComponent(CadAngle, CadCpX, CadCpY, MapPtr);		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::RotateComponent(double Angle, double CpX, double CpY, CMapCoordinate *MapPtr)//零件旋轉
{
	const double CadCpX = CAOIRgn::m_RgnCadPos.x;
	const double CadCpY = CAOIRgn::m_RgnCadPos.y;
	const double OffsetX = CpX-CadCpX;
	const double OffsetY = CpY-CadCpY;
	
	CAOIRgn::RotateRgnCad(Angle, CpX, CpY);	
	CAOIRgn::LayoutRgnStageCornerPos();
	
	const double AngleRad = Angle*DEG_TO_RAD_DBL;			
	JetAPI::RotatePos(m_ComponentOrgCadPos.x, m_ComponentOrgCadPos.y, CpX, CpY, AngleRad, m_ComponentOrgCadPos.x, m_ComponentOrgCadPos.y);		

	CAOIComponent::m_ComponentModel.RotateModel(Angle, 0, 0);	
	CAOIComponent::m_ComponentModel.SetModelAttachedPosCad(m_RgnCadPos);	
	if ( NULL != MapPtr )
	{
		MapComponentCadToStagePos(*MapPtr);
		CAOIComponent::m_ComponentModel.SetModelAttachedPosStage(m_RgnStagePos.x, m_RgnStagePos.y);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::MirrorXComponent(double CpX, CMapCoordinate *MapPtr)//零件鏡射-X
{	
	CAOIRgn::MirrorXRgnCad(CpX);	
	JetAPI::MirrorYAxisPos(CpX, m_ComponentOrgCadPos);
	CAOIComponent::m_ComponentModel.MirrorModelYAxis(0);		
	CAOIComponent::m_ComponentModel.SetModelAttachedPosCad(m_RgnCadPos);	
	CAOIComponent::m_ComponentModel.SetModelAttachedAngle(m_RgnAngle);
	if ( NULL != MapPtr )
	{
		MapComponentCadToStagePos(*MapPtr);
		CAOIComponent::m_ComponentModel.SetModelAttachedPosStage(m_RgnStagePos.x, m_RgnStagePos.y);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::MirrorYComponent(double CpY, CMapCoordinate *MapPtr)//零件鏡射-Y	
{	
	CAOIRgn::MirrorYRgnCad(CpY);
	JetAPI::MirrorXAxisPos(CpY, m_ComponentOrgCadPos);
	CAOIComponent::m_ComponentModel.MirrorModelXAxis(0);
	CAOIComponent::m_ComponentModel.SetModelAttachedPosCad(m_RgnCadPos);
	CAOIComponent::m_ComponentModel.SetModelAttachedAngle(m_RgnAngle);
	if ( NULL != MapPtr )
	{
		MapComponentCadToStagePos(*MapPtr);
		CAOIComponent::m_ComponentModel.SetModelAttachedPosStage(m_RgnStagePos.x, m_RgnStagePos.y);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::ExtractRgnDerivedFrame(bool &Finished)//挖取零件圖片
{	
	if ( CAOIRgn::ExtractRgnFrame(Finished) == false ) { return false; }
	if ( true == Finished )
	{
		bool IsOK = true;
		IsOK = ExecComponentInspection();
		ClearRgnMaskBuffer_Base();
		if ( GetComponentKeepImage() == false )
		{	CAOIRgn::ClearRgnImageBuffer();		}
		if ( false == IsOK )
		{	return false; }	
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::CreateComponentSubRgnList(FIELD_BUILD_MODE BuildMode, bool ByCadRegion)//建立零件子檢測區域列表
{
	switch ( BuildMode )
	{	
	case FIELD_BUILD_RANDOM_PANEL:
	case FIELD_BUILD_RANDOM_BOARD:
	case FIELD_BUILD_RANDOM_PROJECT:
		if ( CAOIRgn::CreateRgnSubList_RandomField() == false )
		{	return false; }
		break;
	default:
		if ( CAOIRgn::CreateRgnSubList_MatrixField(ByCadRegion) == false )
		{	return false; }		
		break;
	}	

	size_t       i=0;
	CAOIRgn     *RgnPtr = NULL;	
	const size_t SubRgnCount = GetComponentSubRgnCount();	
	const bool   MaskEnable_Base = GetComponentMaskEnable_Base();	
	unsigned int MaskFrameIndex_Base = GetComponentMaskFrameIndex_Base();
	unsigned int MaskFrameUniqueID_Base = GetComponentMaskFrameUniqueID_Base();
	const int    MaskColorGroupLinkIndex = GetComponentMaskColorGroupLinkIndex();
	const TNoiseFilterParam &NoiseFilterParam = GetComponentSpaceNoiseFilterParam();

	for ( i=0; i<SubRgnCount; i++ )
	{
		RgnPtr = GetComponentSubRgnPtr(i, false);
		if ( NULL == RgnPtr ) { continue; }
		RgnPtr->SetRgnMaskEnable_Base(MaskEnable_Base);
		RgnPtr->SetRgnMaskFrameIndex_Base(MaskFrameIndex_Base);
		RgnPtr->SetRgnMaskFrameUniqueID_Base(MaskFrameUniqueID_Base);
		RgnPtr->SetRgnMaskColorGroupLinkIndex(MaskColorGroupLinkIndex);

		RgnPtr->SetRgnSpaceNoiseFilterParam(NoiseFilterParam);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::ClearComponentSubRgnList()//清除零件的子列表
{
	CAOIRgn::ClearRgnSubList();
}
//-------------------------------------------------------------------------------------//
size_t CAOIComponent::GetComponentSubRgnCount() const//取得零件的子數量
{
	return CAOIRgn::m_RgnSubList.size();
}
//-------------------------------------------------------------------------------------//
CAOIRgn* CAOIComponent::GetComponentSubRgnPtr(size_t index, bool check) const//取得零件的子指標	
{
	if ( true == check )
	{
		const size_t Count = m_RgnSubList.size();
		if ( index >= Count ) 
		{	return NULL; }
	}
	return m_RgnSubList[index];
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::CheckComponentBePickByCad(const TPOINT2D &StagePos, bool RestMode)//確認零件被點擊到
{
	CAOIModel *ModelPtr = CAOIComponent::GetComponentModelPtr();
	if ( NULL == ModelPtr ) { return false; }
	
	size_t       i=0, j=0;
	int          BasicBoxID=0;
	bool         Pick = false;
	CAOIBox     *BoxPtr = NULL;
	CAOIWnd     *WndPtr = NULL;
	CAOILand    *LandPtr = NULL;	
	const size_t WndCount = ModelPtr->GetModelWndCount();
	const size_t LandCount = ModelPtr->GetModelLandCount();
	const double ComponentAngle = CAOIComponent::GetComponentAngle();
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComponentAngle);

	Pick = false;
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = ModelPtr->GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndVisibled() == false ) { continue; }

		BoxPtr = WndPtr->GetWndBoxPtr();		
		if ( false == RestMode )
		{	Pick = BoxPtr->CheckBoxBePickByCad(StagePos, IsExceptionAngle); }
		else
		{	Pick = BoxPtr->CheckBoxBePickByCadRes(StagePos,IsExceptionAngle); }		
		if ( true == Pick ) 
		{
			BoxPtr->SetBoxSelected(true);
			return true; 
		}
	}
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = ModelPtr->GetModelLandPtr(i, false);
		if ( NULL == LandPtr ) { continue; }

		for ( j=0; j<5; j++ )
		{
			switch ( j )
			{
			case 0:	BasicBoxID = LAND_BOX_PAD; break;
			case 1:	BasicBoxID = LAND_BOX_LEAD; break;
			case 2:	BasicBoxID = LAND_BOX_LEAD_TIP; break;
			case 3:	BasicBoxID = LAND_BOX_LEAD_SHOULDER; break;
			case 4:	BasicBoxID = LAND_BOX_BODY_EDGE; break;
			default: BasicBoxID = 0; break;
			}
			BoxPtr = LandPtr->GetLandBasicBoxPtr(BasicBoxID);
			if ( NULL == BoxPtr ) { continue; }
			if ( false == BoxPtr->GetBoxEnabled() ) { continue; }
			
			if ( false == RestMode )
			{	Pick = BoxPtr->CheckBoxBePickByCad(StagePos, IsExceptionAngle); }
			else
			{	Pick = BoxPtr->CheckBoxBePickByCadRes(StagePos,IsExceptionAngle); }		
			if ( true == Pick ) 
			{
				BoxPtr->SetBoxSelected(true);
				return true; 
			}			
		}
	}
	
	BoxPtr = ModelPtr->GetModelBodyBoxPtr();	
	if ( false == RestMode )
	{	Pick = BoxPtr->CheckBoxBePickByCad(StagePos, IsExceptionAngle); }
	else
	{	Pick = BoxPtr->CheckBoxBePickByCadRes(StagePos,IsExceptionAngle); }		
	if ( true == Pick ) 
	{
		BoxPtr->SetBoxSelected(true);		
		return true; 
	}
	return false;
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::CheckComponentBePickByStage(const TPOINT2D &StagePos, bool RestMode)//確認零件被點擊到
{
	CAOIModel *ModelPtr = CAOIComponent::GetComponentModelPtr();
	if ( NULL == ModelPtr ) { return false; }
	
	size_t       i=0, j=0;
	int          BasicBoxID=0;
	bool         Pick = false;
	CAOIBox     *BoxPtr = NULL;
	CAOIWnd     *WndPtr = NULL;
	CAOILand    *LandPtr = NULL;	
	const size_t WndCount = ModelPtr->GetModelWndCount();
	const size_t LandCount = ModelPtr->GetModelLandCount();
	const double ComponentAngle = CAOIComponent::GetComponentAngle();
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComponentAngle);

	Pick = false;
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = ModelPtr->GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndVisibled() == false ) { continue; }

		BoxPtr = WndPtr->GetWndBoxPtr();		
		if ( false == RestMode )
		{	Pick = BoxPtr->CheckBoxBePickByStage(StagePos, IsExceptionAngle); }
		else
		{	Pick = BoxPtr->CheckBoxBePickByStageRes(StagePos,IsExceptionAngle); }		
		if ( true == Pick ) 
		{
			//BoxPtr->SetBoxSelected(true);
			return true; 
		}
	}
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = ModelPtr->GetModelLandPtr(i, false);
		if ( NULL == LandPtr ) { continue; }

		for ( j=0; j<5; j++ )
		{
			switch ( j )
			{
			case 0:	BasicBoxID = LAND_BOX_PAD; break;
			case 1:	BasicBoxID = LAND_BOX_LEAD; break;
			case 2:	BasicBoxID = LAND_BOX_LEAD_TIP; break;
			case 3:	BasicBoxID = LAND_BOX_LEAD_SHOULDER; break;
			case 4:	BasicBoxID = LAND_BOX_BODY_EDGE; break;
			default: BasicBoxID = 0; break;
			}
			BoxPtr = LandPtr->GetLandBasicBoxPtr(BasicBoxID);
			if ( NULL == BoxPtr ) { continue; }
			if ( false == BoxPtr->GetBoxEnabled() ) { continue; }
			
			if ( false == RestMode )
			{	Pick = BoxPtr->CheckBoxBePickByStage(StagePos, IsExceptionAngle); }
			else
			{	Pick = BoxPtr->CheckBoxBePickByStageRes(StagePos,IsExceptionAngle); }		
			if ( true == Pick ) 
			{
				//BoxPtr->SetBoxSelected(true);
				return true; 
			}			
		}
	}
	
	BoxPtr = ModelPtr->GetModelBodyBoxPtr();	
	if ( false == RestMode )
	{	Pick = BoxPtr->CheckBoxBePickByStage(StagePos, IsExceptionAngle); }
	else
	{	Pick = BoxPtr->CheckBoxBePickByStageRes(StagePos,IsExceptionAngle); }		
	if ( true == Pick ) 
	{
		//BoxPtr->SetBoxSelected(true);
		return true; 
	}
	return false;
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::CheckComponentInRegionByCad(const TREGION4D &SelRgn, bool RestMode, bool bEntireIn)//確認零件在範圍內
{
	CAOIModel *ModelPtr = GetComponentModelPtr();
	if ( NULL == ModelPtr ) { return false; }
	
	size_t       i=0, j=0;
	int          BasicBoxID=0;
	bool         Pick = false;
	CAOIBox     *BoxPtr = NULL;
	CAOIWnd     *WndPtr = NULL;
	CAOILand    *LandPtr = NULL;		
	const double ComponentAngle = GetComponentAngle();
	const size_t WndCount = ModelPtr->GetModelWndCount();
	const size_t LandCount = ModelPtr->GetModelLandCount();		
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComponentAngle);

	Pick = false;
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = ModelPtr->GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndVisibled() == false ) { continue; }

		BoxPtr = WndPtr->GetWndBoxPtr();		
		if ( false == RestMode )
		{	Pick = BoxPtr->CheckBoxInRegionByCad(SelRgn, IsExceptionAngle, bEntireIn); }
		else
		{	Pick = BoxPtr->CheckBoxInRegionByCadRes(SelRgn,IsExceptionAngle, bEntireIn); }		
		if ( true == Pick ) 
		{
			//BoxPtr->SetBoxSelected(true);
			return true; 
		}
	}
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = ModelPtr->GetModelLandPtr(i, false);
		if ( NULL == LandPtr ) { continue; }

		for ( j=0; j<5; j++ )
		{
			switch ( j )
			{
			case 0:	BasicBoxID = LAND_BOX_PAD; break;
			case 1:	BasicBoxID = LAND_BOX_LEAD; break;
			case 2:	BasicBoxID = LAND_BOX_LEAD_TIP; break;
			case 3:	BasicBoxID = LAND_BOX_LEAD_SHOULDER; break;
			case 4:	BasicBoxID = LAND_BOX_BODY_EDGE; break;
			default: BasicBoxID = 0; break;
			}
			BoxPtr = LandPtr->GetLandBasicBoxPtr(BasicBoxID);
			if ( NULL == BoxPtr ) { continue; }
			if ( false == BoxPtr->GetBoxEnabled() ) { continue; }
			
			if ( false == RestMode )
			{	Pick = BoxPtr->CheckBoxInRegionByCad(SelRgn, IsExceptionAngle, bEntireIn); }
			else
			{	Pick = BoxPtr->CheckBoxInRegionByCadRes(SelRgn,IsExceptionAngle, bEntireIn); }		
			if ( true == Pick ) 
			{
				//BoxPtr->SetBoxSelected(true);
				return true; 
			}			
		}
	}
	
	BoxPtr = ModelPtr->GetModelBodyBoxPtr();	
	if ( false == RestMode )
	{	Pick = BoxPtr->CheckBoxInRegionByCad(SelRgn, IsExceptionAngle, bEntireIn); }
	else
	{	Pick = BoxPtr->CheckBoxInRegionByCadRes(SelRgn,IsExceptionAngle, bEntireIn); }
	if ( true == Pick ) 
	{
		//BoxPtr->SetBoxSelected(true);
		return true; 
	}
	return false;
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::CheckComponentInRegionByStage(const TREGION4D &SelRgn, bool RestMode, bool bEntireIn)//確認零件在範圍內
{
	CAOIModel *ModelPtr = CAOIComponent::GetComponentModelPtr();
	if ( NULL == ModelPtr ) { return false; }
	
	size_t       i=0, j=0;
	int          BasicBoxID=0;
	bool         Pick = false;
	CAOIBox     *BoxPtr = NULL;
	CAOIWnd     *WndPtr = NULL;
	CAOILand    *LandPtr = NULL;		
	const size_t WndCount = ModelPtr->GetModelWndCount();
	const size_t LandCount = ModelPtr->GetModelLandCount();	
	const double ComponentAngle = CAOIComponent::GetComponentAngle();
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComponentAngle);

	Pick = false;
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = ModelPtr->GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndVisibled() == false ) { continue; }

		BoxPtr = WndPtr->GetWndBoxPtr();		
		if ( false == RestMode )
		{	Pick = BoxPtr->CheckBoxInRegionByStage(SelRgn, IsExceptionAngle, bEntireIn); }
		else
		{	Pick = BoxPtr->CheckBoxInRegionByStageRes(SelRgn,IsExceptionAngle, bEntireIn); }		
		if ( true == Pick ) 
		{
			//BoxPtr->SetBoxSelected(true);
			return true; 
		}
	}
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = ModelPtr->GetModelLandPtr(i, false);
		if ( NULL == LandPtr ) { continue; }

		for ( j=0; j<5; j++ )
		{
			switch ( j )
			{
			case 0:	BasicBoxID = LAND_BOX_PAD; break;
			case 1:	BasicBoxID = LAND_BOX_LEAD; break;
			case 2:	BasicBoxID = LAND_BOX_LEAD_TIP; break;
			case 3:	BasicBoxID = LAND_BOX_LEAD_SHOULDER; break;
			case 4:	BasicBoxID = LAND_BOX_BODY_EDGE; break;
			default: BasicBoxID = 0; break;
			}
			BoxPtr = LandPtr->GetLandBasicBoxPtr(BasicBoxID);
			if ( NULL == BoxPtr ) { continue; }
			if ( false == BoxPtr->GetBoxEnabled() ) { continue; }
			
			if ( false == RestMode )
			{	Pick = BoxPtr->CheckBoxInRegionByStage(SelRgn, IsExceptionAngle, bEntireIn); }
			else
			{	Pick = BoxPtr->CheckBoxInRegionByStageRes(SelRgn,IsExceptionAngle, bEntireIn); }		
			if ( true == Pick ) 
			{
				//BoxPtr->SetBoxSelected(true);
				return true; 
			}			
		}
	}
	
	BoxPtr = ModelPtr->GetModelBodyBoxPtr();	
	if ( false == RestMode )
	{	Pick = BoxPtr->CheckBoxInRegionByStage(SelRgn, IsExceptionAngle, bEntireIn); }
	else
	{	Pick = BoxPtr->CheckBoxInRegionByStageRes(SelRgn,IsExceptionAngle, bEntireIn); }
	if ( true == Pick ) 
	{
		//BoxPtr->SetBoxSelected(true);
		return true; 
	}
	return false;
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::GetComponentWindowCadRegion(TREGION4D &Region)//取得零件與檢測框在Cad的範圍	
{
	size_t       i=0;
	TREGION4D    rgnWindow;
	CAOIWindow*  WindowPtr = NULL;
	const size_t WindowCount = CAOIComponent::GetComponentWindowCount_Inline();

	CAOIRgn::GetRgnRoiCadRegion(Region);
	for ( i=0; i<WindowCount; i++ )
	{
		WindowPtr = CAOIComponent::GetComponentWindowPtr_Inline(i);
		if ( NULL == WindowPtr ) { continue; }
		if ( WindowPtr->GetWindowDeleted() == true ) { continue; }
		WindowPtr->GetWindowRoiCadRegion(rgnWindow);

		if ( Region.minX > rgnWindow.minX ) { Region.minX = rgnWindow.minX; }
		if ( Region.minY > rgnWindow.minY ) { Region.minY = rgnWindow.minY; }
		if ( Region.maxX < rgnWindow.maxX ) { Region.maxX = rgnWindow.maxX; }
		if ( Region.maxY < rgnWindow.maxY ) { Region.maxY = rgnWindow.maxY; }
	}
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::GetComponentWindowStageRegion(TREGION4D &Region)//取得零件與檢測框在Stage的範圍	
{
	size_t       i=0;
	TREGION4D    rgnWindow;
	CAOIWindow*  WindowPtr = NULL;
	const size_t WindowCount = CAOIComponent::GetComponentWindowCount_Inline();

	CAOIRgn::GetRgnRoiStageRegion(Region);
	for ( i=0; i<WindowCount; i++ )
	{
		WindowPtr = CAOIComponent::GetComponentWindowPtr_Inline(i);
		if ( NULL == WindowPtr ) { continue; }
		if ( WindowPtr->GetWindowDeleted() == true ) { continue; }
		WindowPtr->GetWindowRoiStageRegion(rgnWindow);

		if ( Region.minX > rgnWindow.minX ) { Region.minX = rgnWindow.minX; }
		if ( Region.minY > rgnWindow.minY ) { Region.minY = rgnWindow.minY; }
		if ( Region.maxX < rgnWindow.maxX ) { Region.maxX = rgnWindow.maxX; }
		if ( Region.maxY < rgnWindow.maxY ) { Region.maxY = rgnWindow.maxY; }
	}
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::ResetComponentModel()//復歸零件模組
{
	CAOIModel &ModelRef=m_ComponentModel;	

	//CheckModelTypSpiPad
	SetComponentModelIndex(-1);	
	ModelRef.SetModelName(GetComponentModelName());	
	ModelRef.ClearModelAllObjList();
	if ( GetComponentModelIsolated() == true )
	{	ModelRef.RemoveModelImageFolder();	}
	ModelRef.SetModelType(MODEL_TYPE_NULL);
	UpdateComponentParamToModel(false);	
	return;
}
//-------------------------------------------------------------------------------------//
CAOIModel* CAOIComponent::GetComponentModelPtr()
{
	return &m_ComponentModel;
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentModelIndex(unsigned int value)
{
	CAOIComponent::m_ComponentModelIndex = value;
	CAOIComponent::m_ComponentModel.SetModelIndex(value);
}
//-------------------------------------------------------------------------------------//	
bool CAOIComponent::CheckComponentModelEnabled() const//確認零件模組啟用中
{		
	return m_ComponentModel.CheckModelTypeEnabled();
}
//-------------------------------------------------------------------------------------//	
bool CAOIComponent::UpdateComponentResultID()//更新零件結果編號
{
	CAOIModel *ModelPtr = GetComponentModelPtr();		
	const LANE_ID LaneID = GetComponentLaneID();
	RESULT_ID ResultID = ModelPtr->GetModelResultID();
	RESULT_ID ResultID_Alarm = ModelPtr->GetModelResultID_Alarm();	
	if ( RESULT_ID_EXCEPTION == ResultID_Alarm )
	{
		CString str;
		CString ComponentName = GetComponentFullName();
		str.Format(_T("Error, Test %s is exception"), ComponentName);
		AOIDataCollect.SaveCurrentProcess(str);
	}
	SetComponentResultID_AOI(ResultID);
	SetComponentResultID_Alarm(ResultID_Alarm);	
	UpdateComponentResultID_AOI_Lane(LaneID);	
	return true;
}
//-------------------------------------------------------------------------------------//	
bool CAOIComponent::UpdateComponentDefectCount()//更新零件瑕疵數量
{
	size_t         i=0;
	size_t         idx=0;		
	unsigned int   WndIdx=0;	

	RESULT_ID      WndResultID;
	WND_DEFECT_ID  WndDefectID;
	CWndDefectItem ComponentDefectItem;
	CAOIWnd       *WndPtr = NULL;
	CAOILand      *LandPtr = NULL;	
	CAOIModel     *ModelPtr = GetComponentModelPtr();
	const size_t WndOrderCount = ModelPtr->GetModelWndOrderCount();

	ComponentDefectItem.SetAll(0);
	for ( i=0; i<WndOrderCount; i++ )
	{
		idx = WndOrderCount-i-1;
		WndPtr = ModelPtr->GetModelWndOrderPtr(idx, false);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndEnabled() == false ) { continue; }

		WndIdx = WndPtr->GetWndIndex();
		WndDefectID = WndPtr->GetWndDefectID();
		WndResultID = WndPtr->GetWndResultID();
		if ( RESULT_ID_NG==WndResultID || RESULT_ID_EXCEPTION==WndResultID )
		{	ComponentDefectItem.AddItemCount(WndDefectID);	}		
	}

	const LANE_ID LaneID = GetComponentLaneID();
	RESULT_ID ResultID = ModelPtr->GetModelResultID();
	RESULT_ID ResultID_Alarm = ModelPtr->GetModelResultID_Alarm();	
	ComponentDefectItem.SetLaneID(LaneID);
	ComponentDefectItem.SetResultID(ResultID);
	if ( RESULT_ID_NG==ResultID_Alarm || RESULT_ID_EXCEPTION==ResultID_Alarm )
	{
		if ( GetComponentDefectAlarmEnableOnAOI() == true )
		{	SetComponentDefectAlarmResultOnAOI(true);	}
		if ( GetComponentDefectAlarmEnableOnARS() == true )
		{	SetComponentDefectAlarmResultOnARS(true);	}
	}
	SetComponentCurrentDefectCountAOI(ComponentDefectItem);	
	return true;
}
//-------------------------------------------------------------------------------------//	
bool CAOIComponent::UpdateComponentResultValue()//更新零件結果數值
{
	return UpdateComponentResultValue_V1();
	//return UpdateComponentResultValue_V2();
}
//-------------------------------------------------------------------------------------//	
bool CAOIComponent::UpdateComponentResultValue_V1()//更新零件結果數值
{
	size_t         i=0;
	size_t         idx=0;		
	DWORD          Res=0;	
	unsigned int   WndIdx=0;
	double         dTiltA=INVALID_DOUBLE;	
	double         dSkewA=INVALID_DOUBLE;
	double         dOffsetX=INVALID_DOUBLE;
	double         dOffsetY=INVALID_DOUBLE;
	double         dOffsetA=INVALID_DOUBLE;
	double         dResultHeight=INVALID_DOUBLE;
	double         dResultArea=INVALID_DOUBLE;
	double         dResultVolume=INVALID_DOUBLE;
	double         dResultWidth=INVALID_DOUBLE;
	double         dResultLength=INVALID_DOUBLE;
	double         dResultHeightMin=INVALID_DOUBLE;
	double         dResultHeightMax=INVALID_DOUBLE;

	double         dTiltA_USL=INVALID_DOUBLE;
	double         dTiltA_LSL=INVALID_DOUBLE;
	unsigned int   uTiltA_Idx=INVALID_INDEX;
	double         dSkewA_USL=INVALID_DOUBLE;
	double         dSkewA_LSL=INVALID_DOUBLE;
	unsigned int   uSkewA_Idx=INVALID_INDEX;
	double         dOffsetX_USL=INVALID_DOUBLE;
	double         dOffsetX_LSL=INVALID_DOUBLE;
	unsigned int   uOffsetX_Idx=INVALID_INDEX;
	double         dOffsetY_USL=INVALID_DOUBLE;
	double         dOffsetY_LSL=INVALID_DOUBLE;
	unsigned int   uOffsetY_Idx=INVALID_INDEX;
	double         dOffsetA_USL=INVALID_DOUBLE;
	double         dOffsetA_LSL=INVALID_DOUBLE;
	unsigned int   uOffsetA_Idx=INVALID_INDEX;
	double         dResultHeight_USL=INVALID_DOUBLE;
	double         dResultHeight_LSL=INVALID_DOUBLE;
	unsigned int   uResultHeight_Idx=INVALID_INDEX;
	double         dResultArea_USL=INVALID_DOUBLE;
	double         dResultArea_LSL=INVALID_DOUBLE;
	unsigned int   uResultArea_Idx=INVALID_INDEX;
	double         dResultVolume_USL=INVALID_DOUBLE;
	double         dResultVolume_LSL=INVALID_DOUBLE;
	unsigned int   uResultVolume_Idx=INVALID_INDEX;
	double         dResultWidth_USL=INVALID_DOUBLE;
	double         dResultWidth_LSL=INVALID_DOUBLE;
	unsigned int   uResultWidth_Idx=INVALID_INDEX;
	double         dResultLength_USL=INVALID_DOUBLE;
	double         dResultLength_LSL=INVALID_DOUBLE;
	unsigned int   uResultLength_Idx=INVALID_INDEX;

	ALG_TYPE       AlgType;
	RESULT_ID      WndResultID;
	WND_DEFECT_ID  WndDefectID;
	unsigned int   FrameUniqueID = 0;
	CAOIWnd       *WndPtr = NULL;
	CAOILand      *LandPtr = NULL;	
	CAOIModel     *ModelPtr = GetComponentModelPtr();
	TALG_PARAM_MODEL_MATCH  *ModelMatchPtr=NULL;//演算法-模板匹配參數
	TALG_PARAM_IMAGE_MATCH  *ImageMatchPtr=NULL;//演算法-影像匹配參數
	TALG_PARAM_FD_MATCH     *FdMatchPtr=NULL;//演算法-定位點搜尋參數
	const size_t WndOrderCount = ModelPtr->GetModelWndOrderCount();

	dTiltA=INVALID_DOUBLE;
	dSkewA=INVALID_DOUBLE;
	dOffsetX=INVALID_DOUBLE;
	dOffsetY=INVALID_DOUBLE;
	dOffsetA=INVALID_DOUBLE;
	dResultHeight=INVALID_DOUBLE;
	dResultArea=INVALID_DOUBLE;
	dResultVolume=INVALID_DOUBLE;
	dResultWidth=INVALID_DOUBLE;
	dResultLength=INVALID_DOUBLE;
	dResultHeightMin=INVALID_DOUBLE;
	dResultHeightMax=INVALID_DOUBLE;
	for ( i=0; i<WndOrderCount; i++ )
	{
		idx = WndOrderCount-i-1;
		WndPtr = ModelPtr->GetModelWndOrderPtr(idx, false);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndEnabled() == false ) { continue; }

		WndIdx = WndPtr->GetWndIndex();
		WndDefectID = WndPtr->GetWndDefectID();
		WndResultID = WndPtr->GetWndResultID();

		LandPtr = WndPtr->GetWndLandPtr();
		if ( NULL != LandPtr ) { continue; }		
		
		CAlgParam &AlgParam = WndPtr->GetWndAlgParam();		
		AlgType = AlgParam.GetAlgType();
		FrameUniqueID = AlgParam.GetAlgImageBinParam().GetBinaryFrameUniqueID();
		switch ( AlgType )
		{
		case ALG_BRIGHT_RATIO:
			if ( FRAME_UNIQUE_ID_DLP==FrameUniqueID )
			{
				bool bUsed=false;
				if ( INVALID_DOUBLE==dResultHeight  )
				{	bUsed = true;	}
				else
				{
					if ( WND_DEFECT_BODY_MISSING == WndDefectID )
					{	bUsed = true;	}
				}
				if ( true == bUsed )
				{
					uResultHeight_Idx = (unsigned int)(WndIdx);
					double dSpec = AlgParam.GetAlgParamBrightRatio().brTargetValue;	
					dResultHeight = AlgParam.GetAlgParamBrightRatio().brAverageReading;	
					if ( AlgParam.GetAlgParamBrightRatio().brToleranceEnabled == true )
					{
						dResultHeight_USL = AlgParam.GetAlgParamBrightRatio().brToleranceUSL+dSpec;	
						dResultHeight_LSL = AlgParam.GetAlgParamBrightRatio().brToleranceLSL+dSpec;	
					}
					else if ( AlgParam.GetAlgParamBrightRatio().brRatioEnabled == true )
					{
						dResultHeight_USL = AlgParam.GetAlgParamBrightRatio().brRatioUSL*dSpec/100.0;	
						dResultHeight_LSL = AlgParam.GetAlgParamBrightRatio().brRatioLSL*dSpec/100.0;	
					}
					if ( AlgParam.GetAlgParamBrightRatio().brLimitMaxEnabled == true )
					{	dResultHeightMax = AlgParam.GetAlgParamBrightRatio().brAverageReadingMax;	}
					if ( AlgParam.GetAlgParamBrightRatio().brLimitMinEnabled == true )
					{	dResultHeightMin = AlgParam.GetAlgParamBrightRatio().brAverageReadingMin;	}
				}
			}
			break;
		case ALG_OBJECT_MEASURE:
			if ( FRAME_UNIQUE_ID_DLP==FrameUniqueID )
			{				
				if ( true == AlgParam.GetAlgParamObjectMeasure().omHeightEnabled )
				{
					bool bUsed=false;
					if ( INVALID_DOUBLE==dResultHeight  )
					{	bUsed=true;	}
					else
					{
						if ( WND_DEFECT_BODY_MISSING == WndDefectID )
						{	bUsed=true;	}
					}
					if ( true == bUsed )
					{
						uResultHeight_Idx = (unsigned int)(WndIdx);
						double dSpec=AlgParam.GetAlgParamObjectMeasure().omHeightSpec;
						dResultHeight = AlgParam.GetAlgParamObjectMeasure().omHeightReading;
						switch ( AlgParam.GetAlgParamObjectMeasure().omHeightCalcUnitMode )
						{
						case ALG_CALC_UNIT_ABS://um					
						case ALG_CALC_UNIT_DIFF://um
							dResultHeight_USL = AlgParam.GetAlgParamObjectMeasure().omHeightDiffUSL+dSpec;
							dResultHeight_LSL = AlgParam.GetAlgParamObjectMeasure().omHeightDiffLSL+dSpec;
							break;
						case ALG_CALC_UNIT_RATIO://%
							dResultHeight_USL = AlgParam.GetAlgParamObjectMeasure().omHeightRatioUSL*dSpec/100.0;
							dResultHeight_LSL = AlgParam.GetAlgParamObjectMeasure().omHeightRatioLSL*dSpec/100.0;
							break;
						}
					}
				}
			}
			if ( WND_DEFECT_PART_ALIGN == WndDefectID )
			{
				if ( INVALID_DOUBLE==dSkewA && true==AlgParam.GetAlgSkewEnabled() )
				{
					uSkewA_Idx = (unsigned int)(WndIdx);
					dSkewA = AlgParam.GetAlgSkewReading();
					dSkewA_USL = AlgParam.GetAlgSkewUSL();
					dSkewA_LSL = AlgParam.GetAlgSkewLSL();
				}
				if ( INVALID_DOUBLE==dOffsetX && true==AlgParam.GetAlgOffsetXEnabled() )
				{	
					uOffsetX_Idx = (unsigned int)(WndIdx);
					dOffsetX = AlgParam.GetAlgOffsetXReading();	
					dOffsetX_USL = AlgParam.GetAlgOffsetXUSL();
					dOffsetX_LSL = AlgParam.GetAlgOffsetXLSL();
				}
				if ( INVALID_DOUBLE==dOffsetY && true==AlgParam.GetAlgOffsetYEnabled() )
				{	
					uOffsetY_Idx = (unsigned int)(WndIdx);
					dOffsetY = AlgParam.GetAlgOffsetYReading();
					dOffsetY_USL = AlgParam.GetAlgOffsetYUSL();
					dOffsetY_LSL = AlgParam.GetAlgOffsetYLSL();
				}
				if ( INVALID_DOUBLE==dOffsetA && true==AlgParam.GetAlgOffsetAEnabled() )
				{	
					uOffsetA_Idx = (unsigned int)(WndIdx);
					dOffsetA = AlgParam.GetAlgOffsetAReading();
					dOffsetA_USL = AlgParam.GetAlgOffsetAUSL();
					dOffsetA_LSL = AlgParam.GetAlgOffsetALSL();
				}
			}
			if ( true == AlgParam.GetAlgParamObjectMeasure().omAreaEnabled )
			{	
				uResultArea_Idx = (unsigned int)(WndIdx);
				double dSpec=AlgParam.GetAlgParamObjectMeasure().omAreaSpec;
				dResultArea = AlgParam.GetAlgParamObjectMeasure().omAreaReading;
				dResultArea_USL = AlgParam.GetAlgParamObjectMeasure().omAreaUSL*dSpec/100.0;
				dResultArea_LSL = AlgParam.GetAlgParamObjectMeasure().omAreaLSL*dSpec/100.0;

			}
			if ( true == AlgParam.GetAlgParamObjectMeasure().omVolumeEnabled )
			{	
				uResultVolume_Idx = (unsigned int)(WndIdx);
				double dSpec=AlgParam.GetAlgParamObjectMeasure().omVolumeSpec;
				dResultVolume = AlgParam.GetAlgParamObjectMeasure().omVolumeReading;
				dResultVolume_USL = AlgParam.GetAlgParamObjectMeasure().omVolumeUSL*dSpec/100.0;
				dResultVolume_LSL = AlgParam.GetAlgParamObjectMeasure().omVolumeLSL*dSpec/100.0;
			}
			if ( true == AlgParam.GetAlgParamObjectMeasure().omSizeXEnabled )
			{	
				uResultWidth_Idx = (unsigned int)(WndIdx);
				double dSpec=AlgParam.GetAlgParamObjectMeasure().omSizeXSpec;
				dResultWidth = AlgParam.GetAlgParamObjectMeasure().omSizeXReading;	
				switch ( AlgParam.GetAlgParamObjectMeasure().omSizeCalcUnitMode )
				{
				case ALG_CALC_UNIT_ABS://um					
				case ALG_CALC_UNIT_DIFF://um
					dResultWidth_USL = AlgParam.GetAlgParamObjectMeasure().omSizeXDiffUSL+dSpec;
					dResultWidth_LSL = AlgParam.GetAlgParamObjectMeasure().omSizeXDiffLSL+dSpec;
					break;
				case ALG_CALC_UNIT_RATIO://%
					dResultWidth_USL = AlgParam.GetAlgParamObjectMeasure().omSizeXRatioUSL*dSpec/100.0;
					dResultWidth_LSL = AlgParam.GetAlgParamObjectMeasure().omSizeXRatioLSL*dSpec/100.0;
					break;
				}
			}
			if ( true == AlgParam.GetAlgParamObjectMeasure().omSizeYEnabled )
			{	
				uResultLength_Idx = (unsigned int)(WndIdx);
				double dSpec=AlgParam.GetAlgParamObjectMeasure().omSizeYSpec;
				dResultLength = AlgParam.GetAlgParamObjectMeasure().omSizeYReading;	
				switch ( AlgParam.GetAlgParamObjectMeasure().omSizeCalcUnitMode )
				{
				case ALG_CALC_UNIT_ABS://um					
				case ALG_CALC_UNIT_DIFF://um
					dResultLength_USL = AlgParam.GetAlgParamObjectMeasure().omSizeYDiffUSL+dSpec;
					dResultLength_LSL = AlgParam.GetAlgParamObjectMeasure().omSizeYDiffLSL+dSpec;
					break;
				case ALG_CALC_UNIT_RATIO://%
					dResultLength_USL = AlgParam.GetAlgParamObjectMeasure().omSizeYRatioUSL*dSpec/100.0;
					dResultLength_LSL = AlgParam.GetAlgParamObjectMeasure().omSizeYRatioLSL*dSpec/100.0;
					break;
				}
			}			
			break;
		case ALG_MODEL_MATCH:			
		case ALG_IMAGE_MATCH:			
		case ALG_FD_MATCH:
		case ALG_EDGE_SEARCH:
			if ( WND_DEFECT_PART_ALIGN == WndDefectID )
			{
				if ( INVALID_DOUBLE==dSkewA && true==AlgParam.GetAlgSkewEnabled() )
				{	
					uSkewA_Idx = (unsigned int)(WndIdx);
					dSkewA = AlgParam.GetAlgSkewReading();	
					dSkewA_USL = AlgParam.GetAlgSkewUSL();
					dSkewA_LSL = AlgParam.GetAlgSkewLSL();
				}
				if ( INVALID_DOUBLE==dOffsetX && true==AlgParam.GetAlgOffsetXEnabled() )
				{	
					uOffsetX_Idx = (unsigned int)(WndIdx);
					dOffsetX = AlgParam.GetAlgOffsetXReading();	
					dOffsetX_USL = AlgParam.GetAlgOffsetXUSL();
					dOffsetX_LSL = AlgParam.GetAlgOffsetXLSL();
				}
				if ( INVALID_DOUBLE==dOffsetY && true==AlgParam.GetAlgOffsetYEnabled() )
				{	
					uOffsetY_Idx = (unsigned int)(WndIdx);
					dOffsetY = AlgParam.GetAlgOffsetYReading();	
					dOffsetY_USL = AlgParam.GetAlgOffsetYUSL();
					dOffsetY_LSL = AlgParam.GetAlgOffsetYLSL();
				}
				if ( INVALID_DOUBLE==dOffsetA && true==AlgParam.GetAlgOffsetAEnabled() )
				{	
					uOffsetA_Idx = (unsigned int)(WndIdx);
					dOffsetA = AlgParam.GetAlgOffsetAReading();
					dOffsetA_USL = AlgParam.GetAlgOffsetAUSL();
					dOffsetA_LSL = AlgParam.GetAlgOffsetALSL();
				}
			}
			break;		
		}

		//Group Compare
		if ( FRAME_UNIQUE_ID_DLP==FrameUniqueID )
		{
			if ( true == AlgParam.GetAlgParamGroupCompare().gcTiltAngleEnabled )
			{
				if ( INVALID_DOUBLE==dTiltA || WND_DEFECT_BODY_TILT==WndDefectID )
				{	
					uTiltA_Idx = (unsigned int)(WndIdx);
					dTiltA = AlgParam.GetAlgParamGroupCompare().gcTiltAngleReading;	
					dTiltA_USL = AlgParam.GetAlgParamGroupCompare().gcTiltAngleUSL;	
					dTiltA_LSL = AlgParam.GetAlgParamGroupCompare().gcTiltAngleLSL;	
				}
			}
		}
	}

	if ( INVALID_DOUBLE != dTiltA )
	{	
		SetComponentResultTiltAngle(dTiltA);
		SetComponentResultTiltAngle_USL(dTiltA_USL);
		SetComponentResultTiltAngle_LSL(dTiltA_LSL);
		SetComponentResultTiltAngle_WndIdx(uTiltA_Idx);
	}
	if ( INVALID_DOUBLE != dSkewA )
	{	
		SetComponentResultSkewAngle(dSkewA);
		SetComponentResultSkewAngle_USL(dSkewA_USL);
		SetComponentResultSkewAngle_LSL(dSkewA_LSL);
		SetComponentResultSkewAngle_WndIdx(uSkewA_Idx);
	}
	if ( INVALID_DOUBLE != dOffsetX )
	{	
		SetComponentResultOffsetX(dOffsetX);
		SetComponentResultOffsetX_USL(dOffsetX_USL);
		SetComponentResultOffsetX_LSL(dOffsetX_LSL);
		SetComponentResultOffsetX_WndIdx(uOffsetX_Idx);
	}
	if ( INVALID_DOUBLE != dOffsetY )
	{	
		SetComponentResultOffsetY(dOffsetY);
		SetComponentResultOffsetY_USL(dOffsetY_USL);
		SetComponentResultOffsetY_LSL(dOffsetY_LSL);
		SetComponentResultOffsetY_WndIdx(uOffsetY_Idx);
	}
	if ( INVALID_DOUBLE != dOffsetA )
	{	
		SetComponentResultOffsetA(dOffsetA);
		SetComponentResultOffsetA_USL(dOffsetA_USL);
		SetComponentResultOffsetA_LSL(dOffsetA_LSL);
		SetComponentResultOffsetA_WndIdx(uOffsetA_Idx);
	}
	if ( INVALID_DOUBLE != dResultHeight )
	{	
		SetComponentResultHeight(dResultHeight);
		SetComponentResultHeight_USL(dResultHeight_USL);
		SetComponentResultHeight_LSL(dResultHeight_LSL);
		SetComponentResultHeight_WndIdx(uResultHeight_Idx);
	}
	if ( INVALID_DOUBLE != dResultArea )
	{	
		SetComponentResultArea(dResultArea);
		SetComponentResultArea_USL(dResultArea_USL);
		SetComponentResultArea_LSL(dResultArea_LSL);
		SetComponentResultArea_WndIdx(uResultArea_Idx);
	}
	if ( INVALID_DOUBLE != dResultVolume )
	{	
		SetComponentResultVolume(dResultVolume);
		SetComponentResultVolume_USL(dResultVolume_USL);
		SetComponentResultVolume_LSL(dResultVolume_LSL);
		SetComponentResultVolume_WndIdx(uResultVolume_Idx);
	}	
	if ( INVALID_DOUBLE != dResultWidth )
	{	
		SetComponentResultWidth(dResultWidth);
		SetComponentResultWidth_USL(dResultWidth_USL);
		SetComponentResultWidth_LSL(dResultWidth_LSL);
		SetComponentResultWidth_WndIdx(uResultWidth_Idx);
	}	
	if ( INVALID_DOUBLE != dResultLength )
	{	
		SetComponentResultLength(dResultLength);
		SetComponentResultLength_USL(dResultLength_USL);
		SetComponentResultLength_LSL(dResultLength_LSL);
		SetComponentResultLength_WndIdx(uResultLength_Idx);
	}

	if ( INVALID_DOUBLE != dResultHeightMax )
	{	SetComponentResultHeightMax(dResultHeightMax);	}
	if ( INVALID_DOUBLE != dResultHeightMin )
	{	SetComponentResultHeightMin(dResultHeightMin);	}	
	
	const LANE_ID LaneID = GetComponentLaneID();	
	SetComponentGrrTempValue(dOffsetX, dOffsetY, dSkewA, dResultHeight);		
	UpdateComponentResultStagePos();		
	UpdateComponentResultOffset_Lane(LaneID);
	return true;
}
//-------------------------------------------------------------------------------------//	
bool CAOIComponent::UpdateComponentResultValue_V2()//更新零件結果數值
{	
	size_t         i=0;
	size_t         idx=0;		
	DWORD          Res=0;	
	unsigned int   WndIdx=0;	

	ALG_TYPE       AlgType;
	RESULT_ID      WndResultID;
	WND_DEFECT_ID  WndDefectID;
	unsigned int   FrameUniqueID = 0;
	CAOIWnd       *WndPtr = NULL;
	CAOILand      *LandPtr = NULL;	
	CAOIModel     *ModelPtr = GetComponentModelPtr();	
	const size_t WndOrderCount = ModelPtr->GetModelWndOrderCount();
	
	//Result Value
	RESULT_ID   ResultIDTmp;
	TSpecResult szW, szH;
	TSpecResult rH, rA, rV;
	TSpecResult sX, sY, sA, tA, oA;
	ModelPtr->CheckModelSpecResult(NULL, sX, sY, sA, tA, rH, rA, rV, szW, szH, oA, ResultIDTmp);
	if ( INVALID_DOUBLE != tA.sValue )
	{	
		SetComponentResultTiltAngle(tA.sValue);
		SetComponentResultTiltAngle_USL(tA.sUSL);
		SetComponentResultTiltAngle_LSL(tA.sLSL);
		SetComponentResultTiltAngle_WndIdx(tA.sIndex);
	}
	if ( INVALID_DOUBLE != sA.sValue )
	{	
		SetComponentResultSkewAngle(sA.sValue);
		SetComponentResultSkewAngle_USL(sA.sUSL);
		SetComponentResultSkewAngle_LSL(sA.sLSL);
		SetComponentResultSkewAngle_WndIdx(sA.sIndex);
	}
	if ( INVALID_DOUBLE != sX.sValue )
	{	
		SetComponentResultOffsetX(sX.sValue);
		SetComponentResultOffsetX_USL(sX.sUSL);
		SetComponentResultOffsetX_LSL(sX.sLSL);
		SetComponentResultOffsetX_WndIdx(sX.sIndex);
	}
	if ( INVALID_DOUBLE != sY.sValue )
	{	
		SetComponentResultOffsetY(sY.sValue);
		SetComponentResultOffsetY_USL(sY.sUSL);
		SetComponentResultOffsetY_LSL(sY.sLSL);
		SetComponentResultOffsetY_WndIdx(sY.sIndex);
	}
	if ( INVALID_DOUBLE != oA.sValue )
	{	
		SetComponentResultOffsetA(oA.sValue);
		SetComponentResultOffsetA_USL(oA.sUSL);
		SetComponentResultOffsetA_LSL(oA.sLSL);
		SetComponentResultOffsetA_WndIdx(oA.sIndex);
	}
	if ( INVALID_DOUBLE != rH.sValue )
	{	
		SetComponentResultHeight(rH.sValue);
		SetComponentResultHeight_USL(rH.sUSL);
		SetComponentResultHeight_LSL(rH.sLSL);
		SetComponentResultHeight_WndIdx(rH.sIndex);
	}
	if ( INVALID_DOUBLE != rA.sValue )
	{	
		SetComponentResultArea(rA.sValue);
		SetComponentResultArea_USL(rA.sUSL);
		SetComponentResultArea_LSL(rA.sLSL);
		SetComponentResultArea_WndIdx(rA.sIndex);
	}
	if ( INVALID_DOUBLE != rV.sValue )
	{	
		SetComponentResultVolume(rV.sValue);
		SetComponentResultVolume_USL(rV.sUSL);
		SetComponentResultVolume_LSL(rV.sLSL);
		SetComponentResultVolume_WndIdx(rV.sIndex);
	}	
	if ( INVALID_DOUBLE != szW.sValue )
	{	
		SetComponentResultWidth(szW.sValue);
		SetComponentResultWidth_USL(szW.sUSL);
		SetComponentResultWidth_LSL(szW.sLSL);
		SetComponentResultWidth_WndIdx(szW.sIndex);
	}	
	if ( INVALID_DOUBLE != szH.sValue )
	{	
		SetComponentResultLength(szH.sValue);
		SetComponentResultLength_USL(szH.sUSL);
		SetComponentResultLength_LSL(szH.sLSL);
		SetComponentResultLength_WndIdx(szH.sIndex);
	}
	
	if ( INVALID_DOUBLE != rH.sValueMax )
	{	SetComponentResultHeightMax(rH.sValueMax);	}
	if ( INVALID_DOUBLE != rH.sValueMin )
	{	SetComponentResultHeightMin(rH.sValueMin);	}	 

	const LANE_ID LaneID = GetComponentLaneID();
	SetComponentGrrTempValue(sX.sValue, sY.sValue, sA.sValue, rH.sValue);	
	UpdateComponentResultStagePos();	
	UpdateComponentResultOffset_Lane(LaneID);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::UpdateComponentGrrValue(double OffsetX, double OffsetY, double Skew, double BodyHeight, bool &Added)//更新零件GRR數值
{
	Added = true;
	const TSystemParameter &SysParam=AOIDataCollect.GetSystemParameter();
	const int GrrRandomValue = SysParam.m_GrrOffsetRandomValue;//nm
	const int GrrAverageEnbPxl = SysParam.m_GrrOffsetAverageEnbPxl;//pxl
	const float GrrAverageEnbAngle = SysParam.m_GrrSkewAverageEnbVal;//Angle		
	const float GrrAverageEnbHeight = SysParam.m_GrrHeightAverageEnbVal;//um
	if ( GrrRandomValue > 0 ) 
	{	
		int    nModX = GrrRandomValue;//nm
		int    nModY = (int)(GrrRandomValue*1.5);//nm
		int    nModA = (int)(GrrRandomValue*0.1);//nm
		CString ComponentName = GetComponentFullName();
		unsigned int Seed = (unsigned int)time(NULL);
		unsigned int NameValue = JetAPI::GetStringCharValue(ComponentName);
		Seed += NameValue;

		int    nRandX = JetAPI::GetRandomValue(Seed, nModX);
		int    nRandY = JetAPI::GetRandomValue(Seed+(nRandX)*127, nModY);
		int    nRandA = JetAPI::GetRandomValue(Seed+(nRandX*13)+(nRandY*127), nModA);		
		nRandX = JetAPI::GetRandomValue(Seed+(nRandY)*63, nModX);
		nRandY = JetAPI::GetRandomValue(Seed+(nRandX)*127, nModY);
		nRandA = JetAPI::GetRandomValue(Seed+(nRandX*13)+(nRandY*127), nModA);

		double dRandX=nRandX*0.001;
		double dRandY=nRandY*0.001;
		double dRandA=nRandA*0.001;

		if ( GrrAverageEnbAngle < 0.00001 )
		{
			if ( INVALID_DOUBLE != Skew )
			{
				if ( 0 == (nRandA%2) )
				{	SetComponentResultSkewAngle(Skew+dRandA);	}
				else
				{	SetComponentResultSkewAngle(Skew-dRandA);	}
			}
		}
		if ( 0 == GrrAverageEnbPxl )
		{
			if ( INVALID_DOUBLE != OffsetX )
			{
				if ( 0 == (nRandX%2) )
				{	SetComponentResultOffsetX(OffsetX+dRandX);	}
				else
				{	SetComponentResultOffsetX(OffsetX-(dRandX*1.23));	}
			}
			if ( INVALID_DOUBLE != OffsetY )
			{
				if ( 0 == (nRandY%2) )
				{	SetComponentResultOffsetY(OffsetY+dRandY);	}
				else
				{	SetComponentResultOffsetY(OffsetY-(dRandY*0.87));	}
			}		
		}
	}

	if ( 0 == GrrRandomValue )
	{
		double Mean = 0.0; 		
		double Value_Old = 0.0;
		double Value_New = 0.0;		
		CAMERA_ID CameraID = PRIMARY_CAMERA_ID;		
		const double RexX=AOIDataCollect.GetCameraResolutionX(CameraID);
		const double RexY=AOIDataCollect.GetCameraResolutionY(CameraID);
		const double EnableUm_X=GrrAverageEnbPxl*RexX;
		const double EnableUm_Y=GrrAverageEnbPxl*RexY;
		const double EnableUm_Skew=GrrAverageEnbAngle;
		const double EnableUm_BodyHeight=GrrAverageEnbHeight;
		const TSigmaItem &SigmaItem = GetComponentGrrSigmaItem();				
		
		if ( GrrAverageEnbHeight > 0.00001 )
		{
			const double AllowGap = SysParam.m_GrrHeightAverageStartGap;
			const int GrrAverageWighting = SysParam.m_GrrHeightAverageWeighting;	
			const double Weightting=MIN(100.0, GrrAverageWighting)/100.0;
			if ( INVALID_DOUBLE!=BodyHeight && SigmaItem.BodyHeight.GetCount()>0 )
			{
				Value_Old = BodyHeight;				
				Mean = SigmaItem.BodyHeight.CalcMean();
				const double Dif_Abs = fabs(Value_Old-Mean);	
				if (  (Dif_Abs>AllowGap) && (Dif_Abs<EnableUm_BodyHeight) )
				{
					Value_New = ((1.0-Weightting)*Value_Old)+(Weightting*Mean);					
					SetComponentResultHeight(Value_New);
				}
				if ( Dif_Abs > EnableUm_BodyHeight )
				{	Added = false; }
			}
		}
		if ( GrrAverageEnbAngle > 0.00001 )
		{	
			const double AllowGap = SysParam.m_GrrSkewAverageStartGap;
			const int GrrAverageWighting = SysParam.m_GrrSkewAverageWeighting;	
			const double Weightting=MIN(100.0, GrrAverageWighting)/100.0;
			if ( INVALID_DOUBLE!=Skew && SigmaItem.SkewAngle.GetCount()>0 )
			{
				Value_Old = Skew;				
				Mean = SigmaItem.SkewAngle.CalcMean();
				const double Dif_Abs = fabs(Value_Old-Mean);	
				if (  (Dif_Abs>AllowGap) && (Dif_Abs<EnableUm_Skew) )
				{
					Value_New = ((1.0-Weightting)*Value_Old)+(Weightting*Mean);
					SetComponentResultSkewAngle(Value_New);
				}
				if ( Dif_Abs > EnableUm_Skew)
				{	Added = false; }
			}
		}
		if ( GrrAverageEnbPxl > 0 )
		{
			const double AllowGap = SysParam.m_GrrOffsetAverageStartGap;
			const int GrrAverageWighting = SysParam.m_GrrOffsetAverageWeighting;
			const double Weightting=MIN(100.0, GrrAverageWighting)/100.0;		
			if ( INVALID_DOUBLE!=OffsetX && SigmaItem.OffsetX.GetCount()>0 )
			{
				Value_Old = OffsetX;				
				Mean = SigmaItem.OffsetX.CalcMean();
				const double Dif_Abs = fabs(Value_Old-Mean);	
				if (  (Dif_Abs>AllowGap) && (Dif_Abs<EnableUm_X) ) 
				{
					Value_New = ((1.0-Weightting)*Value_Old)+(Weightting*Mean);
					SetComponentResultOffsetX(Value_New);
				}
				if ( Dif_Abs > EnableUm_X )
				{	Added = false; }
			}
			if ( INVALID_DOUBLE!=OffsetY && SigmaItem.OffsetY.GetCount()>0 )
			{
				Value_Old = OffsetY;				
				Mean = SigmaItem.OffsetY.CalcMean();
				const double Dif_Abs = fabs(Value_Old-Mean);	
				if (  (Dif_Abs>AllowGap) && (Dif_Abs<EnableUm_Y) ) 				
				{
					Value_New = ((1.0-Weightting)*Value_Old)+(Weightting*Mean);
					SetComponentResultOffsetY(Value_New);
				}
				if ( Dif_Abs > EnableUm_Y )
				{	Added = false; }
			}
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::SetComponentGrrTempValue(double OffsetX, double OffsetY, double Skew, double BodyHeight)//設定零件GRR暫存數值
{
	m_ComponentGrrOffsetX = OffsetX;
	m_ComponentGrrOffsetY = OffsetY;
	m_ComponentGrrSkewA = Skew;
	m_ComponentGrrBodyHeight = BodyHeight;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::GetComponentGrrTempValue(double &OffsetX, double &OffsetY, double &Skew, double &BodyHeight) const//取回零件GRR暫存數值
{
	OffsetX = m_ComponentGrrOffsetX;
	OffsetY = m_ComponentGrrOffsetY;
	Skew = m_ComponentGrrSkewA;
	BodyHeight = m_ComponentGrrBodyHeight;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::UpdateComponentResultStagePos()//更新零件機台座標結果
{
	//計算零件本體的機台偏移量
	CAOIModel     *ModelPtr = GetComponentModelPtr();
	if ( NULL == ModelPtr ) { return false; }

	TPOINT2D  BodyCp,  BodyCpRes, BodyOffsetCad, BodyOffsetStage;
	TREGION4D BodyRgn, BodyRgnRes;	
	const double StagePosX=GetComponentStagePosX();
	const double StagePosY=GetComponentStagePosY();

	ModelPtr->GetModelBodyBox().GetBoxRegion(BodyRgn);
	ModelPtr->GetModelBodyBox().GetBoxRegionRes(BodyRgnRes);
	BodyCp.x = BodyRgn.GetCpX();
	BodyCp.y = BodyRgn.GetCpY();
	BodyCpRes.x = BodyRgnRes.GetCpX();
	BodyCpRes.y = BodyRgnRes.GetCpY();
	BodyOffsetCad.x = BodyCpRes.x-BodyCp.x;
	BodyOffsetCad.y = BodyCpRes.y-BodyCp.y;
	AOIDataCollect.MapCadOffsetPtToStage(BodyOffsetCad, BodyOffsetStage);

	const double StageResPosX=StagePosX+BodyOffsetStage.x;
	const double StageResPosY=StagePosY+BodyOffsetStage.y;
	SetComponentStageOffsetX(BodyOffsetStage.x);
	SetComponentStageOffsetY(BodyOffsetStage.y);
	SetComponentStageResultX(StageResPosX);
	SetComponentStageResultY(StageResPosY);
	return true;//由座標轉換為主(Map)

	const double CadPosX=GetComponentCadPosX();
	const double CadPosY=GetComponentCadPosY();
	
	const double CadResPosX=CadPosX+BodyOffsetCad.x;
	const double CadResPosY=CadPosY+BodyOffsetCad.y;
	
	SetComponentCadResultX(CadResPosX);
	SetComponentCadResultY(CadResPosY);
	SetComponentStageResultX(StageResPosX);
	SetComponentStageResultY(StageResPosY);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::UpdateComponentParamToModel(bool Resize)//更新零件參數至模組內
{
	//Resize = false;
	TPOINT2D PosCad, PosStage;	
	double SizeW = GetComponentRoiSizeW();
	double SizeH = GetComponentRoiSizeH();
	double BodyW = GetComponentBodySizeW();
	double BodyH = GetComponentBodySizeH();
	double ModelBodySizeW = BodyW;
	double ModelBodySizeH = BodyH;
	const double Angle = GetComponentAngle();
	const double CadPosX = GetComponentCadPosX();
	const double CadPosY = GetComponentCadPosY();
	const double StagePosX = GetComponentStagePosX();
	const double StagePosY = GetComponentStagePosY();
	const bool   ModelIsolated = GetComponentModelIsolated();
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(Angle);
	CAOIBox *BoxPtr = m_ComponentModel.GetModelBodyBoxPtr();

	if ( true == Resize )
	{
		SizeW = 2000;
		SizeH = 2000;
		BodyW = 1000;
		BodyH = 1000;
	}	
	PosCad.x = CadPosX;
	PosCad.y = CadPosY;
	PosStage.x = StagePosX;
	PosStage.y = StagePosY;

	if ( true == Resize )
	{
		SetComponentRoiSizeW(SizeW);
		SetComponentRoiSizeH(SizeH);
		SetComponentBodySizeW(BodyW);
		SetComponentBodySizeH(BodyH);
		SetComponentCadBiasPosX(0);
		SetComponentCadBiasPosY(0);
	}
	JetAPI::RotateSize(Angle, ModelBodySizeW, ModelBodySizeH);

	double BiasPosX = CAOIComponent::GetComponentCadBiasPosX();
	double BiasPosY = CAOIComponent::GetComponentCadBiasPosY();
	//BoxPtr->SetBoxPosX(BiasPosX);
	//BoxPtr->SetBoxPosY(BiasPosY);
	BoxPtr->SetBoxSize(ModelBodySizeW, ModelBodySizeH, true);	

	if ( false == IsExceptionAngle )
	{	m_ComponentModel.SetModelAttachedAngle(Angle);	}
	else
	{	
		m_ComponentModel.SetModelAttachedAngle(0);
		m_ComponentModel.RotateModel(Angle, 0, 0);
	}	
	m_ComponentModel.SetModelComponentPtr(this);
	m_ComponentModel.SetModelAttachedPosCad(PosCad);
	m_ComponentModel.SetModelAttachedPosStage(PosStage);	
	m_ComponentModel.CalcModelTotalRegionAll();
	if ( true == Resize )
	{
		CalcComponentCadCornerPos();
		LayoutComponentStageCornerPos();
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::SyncComponentModelFromLibrary(CAOIModel *ModelPtr, bool bPartial)//由資料庫模組同步化至更新零件模組
{
	if ( NULL == ModelPtr ) { return false; }	
	if ( m_ComponentModelName.compare(ModelPtr->GetModelName()) != 0 ) 
	{	ModelPtr = ModelPtr; }

	m_ComponentModel.SynchronousModel(ModelPtr, bPartial);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::UpdateComponentModelFromLibrary(CAOIModel *ModelPtr)//更新零件模組至資料庫模組內
{
	if ( NULL == ModelPtr ) { return false; }	
	if ( m_ComponentModelName.compare(ModelPtr->GetModelName()) != 0 ) 
	{	ModelPtr = ModelPtr;	}

	TREGION4D rgnBody;	
	TREGION4D rgnFull;	
	TREGION4D rgnFullRotated;	
	TPOINT2D  PosStage;
	m_ComponentModel = *ModelPtr;
	ModelPtr->GetModelBodyRegion(rgnBody);
	ModelPtr->GetModelTotalRegion(rgnFull);
	double BodyW = rgnBody.maxX-rgnBody.minX;
	double BodyH = rgnBody.maxY-rgnBody.minY;
	double FullW = rgnFull.maxX-rgnFull.minX;
	double FullH = rgnFull.maxY-rgnFull.minY;
	const double BiasPosX = rgnFull.GetCpX();
	const double BiasPosY = rgnFull.GetCpY();
	const double ComponentAngle = GetComponentAngle();
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComponentAngle);
	TPOINT2D PosCad = GetComponentCadPos();
	const double StageX = GetComponentStagePosX();
	const double StageY = GetComponentStagePosY();	

	PosStage.x = StageX;
	PosStage.y = StageY;
	
	//JetAPI::RotateSize(ComponentAngle, BodyW, BodyH);	
	m_ComponentModel.SetModelAttachedAngle(0);	
	m_ComponentModel.RotateModel(ComponentAngle, 0, 0);

	m_ComponentModel.GetModelTotalRegion(rgnFullRotated);
	const double RotatedBiasPosX = rgnFullRotated.GetCpX();
	const double RotatedBiasPosY = rgnFullRotated.GetCpY();
	
	const TNoiseFilterParam &NoiseFilterParam = GetComponentSpaceNoiseFilterParam();
	m_ComponentModel.SetModelSpaceBasePlaneParam(NoiseFilterParam.BasePlaneParam);
	m_ComponentModel.SetModelSpaceNoiseFilterParam(NoiseFilterParam);

	//特殊遮罩-基板顏色
	bool RgnMaskEnable_Base = GetRgnMaskEnable_Base();		
	unsigned int RgnMaskFrameIndex_Base = GetRgnMaskFrameIndex_Base();	
	unsigned int RgnMaskFrameUniqueID_Base = GetRgnMaskFrameUniqueID_Base();	
	const int RgnMaskColorGroupLinkIndex = GetRgnMaskColorGroupLinkIndex();	

	m_ComponentModel.SetModelMaskEnable_Base(RgnMaskEnable_Base);
	m_ComponentModel.SetModelMaskFrameIndex_Base(RgnMaskFrameIndex_Base);
	m_ComponentModel.SetModelMaskFrameUniqueID_Base(RgnMaskFrameUniqueID_Base);
	m_ComponentModel.SetModelMaskColorGroupLinkIndex(RgnMaskColorGroupLinkIndex);	

	const double PanelBasePlane=GetComponentPanelBasePlane();
	m_ComponentModel.SetModelPanelBasePlane(PanelBasePlane);

	const bool DataModelEnabled=GetComponentDataModelEnabled();
	m_ComponentModel.SetModelDataModelEnabled(DataModelEnabled);
	const int DataModelLevelID=GetComponentDataModelLevelID();
	m_ComponentModel.SetModelDataModelLevelID(DataModelLevelID);

	m_ComponentModel.SetModelComponentPtr(this);
	m_ComponentModel.SetModelAttachedPosCad(PosCad);
	m_ComponentModel.SetModelAttachedPosStage(PosStage);
	m_ComponentModel.InitModelInspection();	
	m_ComponentModel.SetModelModifiedCount(false);
	m_ComponentModel.SetModelLandModified(false);
	m_ComponentModel.SetModelWndModified(false);
	m_ComponentModel.SetModelLogicModified(false);
	
	SetComponentRoiSizeW(FullW);
	SetComponentRoiSizeH(FullH);	
	SetComponentBodySizeW(BodyW);
	SetComponentBodySizeH(BodyH);
	if ( false == IsExceptionAngle )
	{
		SetComponentCadBiasPosX(RotatedBiasPosX);
		SetComponentCadBiasPosY(RotatedBiasPosY);
	}
	else
	{
		SetComponentCadBiasPosX(BiasPosX);
		SetComponentCadBiasPosY(BiasPosY);
	}
	SetComponentModelName(m_ComponentModel.GetModelName());
	SetComponentModelIndex(m_ComponentModel.GetModelIndex());
	//UpdateComponentParamToModel(false);
	CalcComponentCadCornerPos();
	LayoutComponentStageCornerPos();
	return true;
}
//-------------------------------------------------------------------------------------//
CAOIWnd* CAOIComponent::GetComponentModelWndPtrByDefectID(WND_DEFECT_ID WndDefectID)//依照檢測框瑕疵編號找到檢測框
{	
	CAOIModel *ModelPtr = GetComponentModelPtr();
	if ( NULL == ModelPtr ) { return NULL; }
	return ModelPtr->GetModelWndPtrByDefectID(WndDefectID, -1);
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::UpdateComponentModelWndBypassed(const CWndDefectItem &DefectEnabled)//更新模組檢測框是否忽略	
{
	bool bSucc=true;
	if ( m_ComponentModel.UpdateModelWndBypassed(DefectEnabled) == false )
	{	bSucc = false; }
	return bSucc;
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentModelClassID(int value)//設定零件指定模組類別編號
{
	m_ComponentModelClassID = value;
	m_ComponentModel.SetModelClassID(value);
}
//-------------------------------------------------------------------------------------//
int CAOIComponent::GetComponentModelClassID() const//取得零件指定模組類別編號
{
	return m_ComponentModelClassID;
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentModelIsolated(bool value) 
{ 
	m_ComponentModelIsolated = value; 
	m_ComponentModel.SetModelIsolated(value);
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::GetComponentModelIsolated() const 
{ 
	return m_ComponentModelIsolated; 
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::GetComponentSaveWndList() const//取得零件儲存檢測框列表
{
	return m_ComponentSaveWndList;
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentSaveWndList(bool value)//設定零件儲存檢測框列表
{
	m_ComponentSaveWndList = value;
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::GetComponentModelImageIsSaved() const//取得零件模組是否存圖
{
	return GetRgnModelImageIsSaved();
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentModelImageIsSaved(bool value)//設定零件模組是否存圖
{
	CAOIRgn::SetRgnModelImageIsSaved(value);
}
//-------------------------------------------------------------------------------------//
SAVE_TEST_IMAGE_MODE CAOIComponent::GetComponentSaveTestImageMode() const//取得零件儲存影像模式
{
	return CAOIRgn::GetRgnSaveTestImageMode();
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentSaveTestImageMode(SAVE_TEST_IMAGE_MODE val)//設定零件儲存影像模式	
{
	CAOIRgn::SetRgnSaveTestImageMode(val);
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::GetComponentModelImageIsSaved_AI() const//取得零件模組是否存圖-AI
{
	return GetRgnModelImageIsSaved_AI();
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentModelImageIsSaved_AI(bool value)//設定零件模組是否存圖-AI
{
	CAOIRgn::SetRgnModelImageIsSaved_AI(value);
}
//---------------------------------------------------------------------------------//
size_t CAOIComponent::GetComponentTotalTestCountAOI_Lane(LANE_ID LaneID) const
{
	if ( LANE_ID_B == LaneID )
	{	return GetComponentTotalTestCountAOI_LB(); }
	return GetComponentTotalTestCountAOI_LA();
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentTotalTestCountAOI_Lane(LANE_ID LaneID, size_t val)
{
	switch ( LaneID )
	{
	case LANE_ID_A: SetComponentTotalTestCountAOI_LA(val);	break;
	case LANE_ID_B: SetComponentTotalTestCountAOI_LB(val);	break;
	}
	return;
}
//-------------------------------------------------------------------------------------//
size_t CAOIComponent::GetComponentTotalTestCountARS_Lane(LANE_ID LaneID) const
{
	if ( LANE_ID_B == LaneID )
	{	return GetComponentTotalTestCountARS_LB(); }
	return GetComponentTotalTestCountARS_LA();
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentTotalTestCountARS_Lane(LANE_ID LaneID, size_t val)
{
	switch ( LaneID )
	{
	case LANE_ID_A: SetComponentTotalTestCountARS_LA(val);	break;
	case LANE_ID_B: SetComponentTotalTestCountARS_LB(val);	break;
	}
	return;
}
//-------------------------------------------------------------------------------------//
size_t CAOIComponent::GetComponentTotalNGCountAOI_Lane(LANE_ID LaneID) const
{
	if ( LANE_ID_B == LaneID )
	{	return GetComponentTotalNGCountAOI_LB(); }
	return GetComponentTotalNGCountAOI_LA();
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentTotalNGCountAOI_Lane(LANE_ID LaneID, size_t val)
{
	switch ( LaneID )
	{
	case LANE_ID_A: SetComponentTotalNGCountAOI_LA(val);	break;
	case LANE_ID_B: SetComponentTotalNGCountAOI_LB(val);	break;
	}
	return;
}
//-------------------------------------------------------------------------------------//
size_t CAOIComponent::GetComponentTotalNGCountARS_Lane(LANE_ID LaneID) const
{
	if ( LANE_ID_B == LaneID )
	{	return GetComponentTotalNGCountARS_LB(); }
	return GetComponentTotalNGCountARS_LA();
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentTotalNGCountARS_Lane(LANE_ID LaneID, size_t val)
{
	switch ( LaneID )
	{
	case LANE_ID_A: SetComponentTotalNGCountARS_LA(val);	break;
	case LANE_ID_B: SetComponentTotalNGCountARS_LB(val);	break;
	}
	return;
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::ChangeComponentBoard(CAOIBoard *RefBoardPtr)//變更零件的單板
{
	CAOIComponent *ComponentPtr=this;
	if ( NULL == ComponentPtr ) { return false; }
	if ( NULL == RefBoardPtr ) { return false; }

	CAOIPanel *RefPanelPtr = RefBoardPtr->GetBoardPanelPtr();
	CAOIPanel *PanelPtr = ComponentPtr->GetComponentPanelPtr();
	CAOIBoard *BoardPtr = ComponentPtr->GetComponentBoardPtr();	
	if ( BoardPtr == RefBoardPtr ) { return true; }

	if ( PanelPtr != RefPanelPtr )
	{		
		if ( NULL != PanelPtr )
		{
			PanelPtr->SelectPanelAllObjects(false);
			ComponentPtr->SetComponentSelected(true);	
			PanelPtr->RemovePanelComponentSelected();
		}
		if ( NULL != RefPanelPtr )
		{
			CMapCoordinate PanelMapCTS;
			CMapCoordinate PanelMapSTC;
			CMapCoordinate PanelMapCTS_DB;
			CMapCoordinate PanelMapSTC_DB;				
			RefPanelPtr->GetPanelMapCTS(DISTRICT_ID_A, PanelMapCTS);
			RefPanelPtr->GetPanelMapSTC(DISTRICT_ID_A, PanelMapSTC);
			RefPanelPtr->GetPanelMapCTS(DISTRICT_ID_B, PanelMapCTS_DB);
			RefPanelPtr->GetPanelMapSTC(DISTRICT_ID_B, PanelMapSTC_DB);

			double CadDifX=0;
			double CadDifY=0;
			double CadPosXNew=0;
			double CadPosYNew=0;
			double CadPosX = ComponentPtr->GetComponentCadPosX();
			double CadPosY = ComponentPtr->GetComponentCadPosY();
			double StagePosX=ComponentPtr->GetComponentStagePosX();
			double StagePosY=ComponentPtr->GetComponentStagePosY();
			DISTRICT_ID    DistrictID=ComponentPtr->GetComponentDistrictID();			
			
			if ( DISTRICT_ID_A == DistrictID )
			{	PanelMapSTC.Map2D(StagePosX, StagePosY, CadPosXNew, CadPosYNew);	}
			if ( DISTRICT_ID_B == DistrictID )
			{	PanelMapSTC_DB.Map2D(StagePosX, StagePosY, CadPosXNew, CadPosYNew);	}		

			CadDifX = CadPosXNew-CadPosX;
			CadDifY = CadPosYNew-CadPosY;
			if ( DISTRICT_ID_A == DistrictID )
			{	ComponentPtr->MoveComponentPos(CadDifX, CadDifY, &PanelMapCTS); }		
			if ( DISTRICT_ID_B == DistrictID )
			{	ComponentPtr->MoveComponentPos(CadDifX, CadDifY, &PanelMapCTS_DB); }	
			RefPanelPtr->AddPanelComponentPtr(ComponentPtr); 			
		}
	}

	if ( NULL != BoardPtr )
	{
		BoardPtr->SelectBoardAllObjects(false);
		ComponentPtr->SetComponentSelected(true);	
		BoardPtr->RemoveBoardComponentSelected();
		BoardPtr->LayoutBoardRegion();
	}
	RefBoardPtr->AddBoardComponentPtr(ComponentPtr);
	RefBoardPtr->LayoutBoardRegion();

	const bool ModelIsolated=ComponentPtr->GetComponentModelIsolated();
	if ( true == ModelIsolated )
	{
		CAOIModel *ModelPtr = ComponentPtr->GetComponentModelPtr();
		if ( NULL != ModelPtr )
		{	
			CString ModelFolder;
			CString ModelFolderOld;
			CString ModelFolderNew;
			ModelFolderOld = ModelPtr->GetModelFolderComponent();	
			JetAPI::ExtractLastPath(ModelFolderOld, ModelFolder);
			CString FullComponentName = ComponentPtr->GetComponentFullName();		
			ModelFolderNew.Format(_T("%s\\%s"), ModelFolder, FullComponentName);
			::MoveFile(ModelFolderOld, ModelFolderNew);//直接變更資料夾名稱			
			ModelPtr->SetModelFolderComponent(ModelFolderNew);
			ModelPtr->AssignModelFolder();
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::CheckComponentTypeBeClicked() const//確認零件樣式是否可以被點選
{
	bool bClicked=true;
	COMPONENT_TYPE ComponentType = GetComponentType();
	switch ( ComponentType )
	{
	//case COMPONENT_TYPE_NORMAL:
	case COMPONENT_TYPE_FULL_MAP:
	case COMPONENT_TYPE_SPECIAL:
	case COMPONENT_TYPE_DROP_OUT:
	case COMPONENT_TYPE_SCRATCH:
	case COMPONENT_TYPE_TEMPORARY:
		bClicked = false;
		break;	
	}
	return bClicked;
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::CheckComponentType_FullMap() const//確認零件樣式-全底圖零件
{
	if ( COMPONENT_TYPE_FULL_MAP == GetComponentType() )
	{	return true; }	
	return false;
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::CheckComponentType_ModelTest() const//確認零件樣式-支援模組檢測
{
	if ( COMPONENT_TYPE_NORMAL == GetComponentType() )
	{	return true; }	
	return false;
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::BypassSkipComponent(RESULT_ID value)//不檢測或跳過零件
{
	CAOIComponent *ComponentPtr = this;
	LANE_ID LaneID = ComponentPtr->GetComponentLaneID();
	ComponentPtr->SetComponentNeedToCalculate(false);		
	ComponentPtr->SetComponentNeedToCalculateBackup(false);
	ComponentPtr->SetComponentResultID_AOI(value);
	ComponentPtr->SetComponentResultID_Alarm(value);
	ComponentPtr->UpdateComponentResultID_AOI_Lane(LaneID);

	CAOIModel *ModelPtr = ComponentPtr->GetComponentModelPtr();
	ModelPtr->SetModelResultID(value);
	ModelPtr->SetModelResultID_Alarm(value);	
	ModelPtr->BypassSkipModelWnd(value);
	return;
}
//-------------------------------------------------------------------------------------//
RESULT_ID CAOIComponent::CheckComponentResultID_AOI()//確認零件結果編號-AOI	
{	
	CAOIComponent *ResultPtr=GetComponentResultPtr();	
	return ResultPtr->GetComponentResultID_AOI(); 
}
//-------------------------------------------------------------------------------------//
RESULT_ID CAOIComponent::GetComponentResultID_AOI() const//取得零件結果編號-AOI
{
	return CAOIRgn::GetRgnResultID_AOI();
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentResultID_AOI(RESULT_ID value)//設定零件結果編號-AOI
{
	CAOIRgn::SetRgnResultID_AOI(value);
}
//-------------------------------------------------------------------------------------//
RESULT_ID CAOIComponent::GetComponentResultID_AOI_LA() const//取得零件結果編號-AOI-A軌
{
	return CAOIRgn::GetRgnResultID_AOI_LA();
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentResultID_AOI_LA(RESULT_ID value)//設定零件結果編號-AOI-A軌
{
	CAOIRgn::SetRgnResultID_AOI_LA(value);
}
//-------------------------------------------------------------------------------------//
RESULT_ID CAOIComponent::GetComponentResultID_AOI_LB() const//取得零件結果編號-AOI-B軌
{
	return CAOIRgn::GetRgnResultID_AOI_LB();
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentResultID_AOI_LB(RESULT_ID value)//設定零件結果編號-AOI-B軌
{
	CAOIRgn::SetRgnResultID_AOI_LB(value);
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::UpdateComponentResultID_AOI_Lane(LANE_ID LaneID)//更新零件結果編號-AOI-軌道
{
	RESULT_ID ResultID=CheckComponentResultID_AOI();
	switch ( LaneID )
	{
	case LANE_ID_A: SetComponentResultID_AOI_LA(ResultID); break;
	case LANE_ID_B: SetComponentResultID_AOI_LB(ResultID); break;	
	}
	return;
}
//-------------------------------------------------------------------------------------//
RESULT_ID CAOIComponent::GetComponentResultID_AOI_Lane(LANE_ID LaneID) const//取得零件結果編號-AOI-軌道
{
	RESULT_ID ResultID;
	switch ( LaneID )
	{
	case LANE_ID_A: ResultID = GetComponentResultID_AOI_LA(); break;
	case LANE_ID_B: ResultID = GetComponentResultID_AOI_LB(); break;
	default:		ResultID = GetComponentResultID_AOI(); break;
	}
	return ResultID;
}
//-------------------------------------------------------------------------------------//
RESULT_ID CAOIComponent::GetComponentResultID_ARS() const//取得零件結果編號-ARS
{
	return CAOIRgn::GetRgnResultID_ARS();
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentResultID_ARS(RESULT_ID value)//設定零件結果編號-ARS
{
	CAOIRgn::SetRgnResultID_ARS(value);
}
//-------------------------------------------------------------------------------------//
RESULT_ID CAOIComponent::GetComponentResultID_ARS_LA() const//取得零件結果編號-ARS-A軌
{
	return CAOIRgn::GetRgnResultID_ARS_LA();
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentResultID_ARS_LA(RESULT_ID value)//設定零件結果編號-ARS-A軌
{
	CAOIRgn::SetRgnResultID_ARS_LA(value);
}
//-------------------------------------------------------------------------------------//
RESULT_ID CAOIComponent::GetComponentResultID_ARS_LB() const//取得零件結果編號-ARS-B軌
{
	return CAOIRgn::GetRgnResultID_ARS_LB();
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentResultID_ARS_LB(RESULT_ID value)//設定零件結果編號-ARS-B軌
{
	CAOIRgn::SetRgnResultID_ARS_LB(value);
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::UpdateComponentResultID_ARS_Lane(LANE_ID LaneID)//更新零件結果編號-ARS-軌道
{
	RESULT_ID ResultID=GetComponentResultID_ARS();
	switch ( LaneID )
	{
	case LANE_ID_A: SetComponentResultID_ARS_LA(ResultID); break;
	case LANE_ID_B: SetComponentResultID_ARS_LB(ResultID); break;	
	}
	return;
}
//-------------------------------------------------------------------------------------//
RESULT_ID CAOIComponent::GetComponentResultID_ARS_Lane(LANE_ID LaneID) const//取得零件結果編號-ARS-軌道
{
	RESULT_ID ResultID;
	switch ( LaneID )
	{
	case LANE_ID_A: ResultID = GetComponentResultID_ARS_LA(); break;
	case LANE_ID_B: ResultID = GetComponentResultID_ARS_LB(); break;
	default:		ResultID = GetComponentResultID_ARS(); break;
	}
	return ResultID;
}
//-------------------------------------------------------------------------------------//
RESULT_ID CAOIComponent::GetComponentResultID_Alarm() const//取得零件結果編號-停機
{
	return CAOIRgn::GetRgnResultID_Alarm();
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentResultID_Alarm(RESULT_ID value)//設定零件結果編號-停機
{
	CAOIRgn::SetRgnResultID_Alarm(value);
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::UpdateComponentBypassed()//確認零件是否為不檢測
{
	bool bBypassed = false;
	CAOIPanel *PanelPtr = NULL;
	CAOIBoard *BoardPtr = NULL;
	
	bBypassed = GetComponentBypassed();
	if ( false == bBypassed )
	{
		BoardPtr = GetComponentBoardPtr();
		if ( NULL != BoardPtr )
		{
			if ( BoardPtr->GetBoardBypassed() == true ) 
			{	bBypassed = true; }
		}
	}
	if ( false == bBypassed )
	{
		PanelPtr = GetComponentPanelPtr();
		if ( NULL != PanelPtr )
		{
			if ( PanelPtr->GetPanelBypassed() == true ) 
			{	bBypassed = true; }
		}
	}
	
	RESULT_ID  ModelResultID=RESULT_ID_NONE;
	CAOIModel *ModelPtr = GetComponentModelPtr();
	if ( NULL != ModelPtr )
	{
		ModelResultID = ModelPtr->GetModelResultID();		
		if ( true == bBypassed )
		{
			ModelPtr->SetModelResultID(RESULT_ID_BYPASS);
			ModelPtr->SetModelResultID_Alarm(RESULT_ID_BYPASS);
			ModelPtr->SetModelWndResultID(RESULT_ID_BYPASS);
		}
		else
		{
			if ( ModelResultID == RESULT_ID_BYPASS)
			{
				ModelPtr->SetModelResultID(RESULT_ID_NONE);
				ModelPtr->SetModelResultID_Alarm(RESULT_ID_NONE);
				ModelPtr->SetModelWndResultID(RESULT_ID_NONE);
			}
		}
	}	
	if ( true == bBypassed )
	{
		SetComponentNeedToCalculate(false);
		SetComponentResultID_AOI(RESULT_ID_BYPASS);
		SetComponentResultID_Alarm(RESULT_ID_BYPASS);				
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
int CAOIComponent::CalcComponentOpenMPCountByPixels()
{
	return CalcRgnOpenMPCountByPixels();
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentOpenMPCount(int value)
{ 
	SetRgnOpenMPCount(value);
	CAOIModel *ModelPtr = GetComponentModelPtr();
	if ( NULL != ModelPtr )
	{	ModelPtr->SetModelOpenMPCount(value); }
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::ChangeComponentXBoardUnit(bool value)
{
	if ( CheckComponentIsMasterOrAgent() == true )
	{	ChangeComponentXBoardUnitMaster(value); return;	}
	SetComponentXBoardUnit(value);
	return;
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::GetComponentOffsetText(CString &Text) const
{
	//Text.Format(_T("Offset(%.0f, %.0f) um, Angle=%.2f"), GetComponentCadPosX(), GetComponentCadPosY(), GetComponentAngle());
	Text.Format(_T("Offset(%.0f, %.0f, %.2f)"), GetComponentCadPosX(), GetComponentCadPosY(), GetComponentAngle());
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::GetComponentOffsetText(TCHAR TextBuffer[]) const
{
	//::_stprintf(TextBuffer, _T("Offset(%.0f, %.0f) um, Angle=%.2f"), GetComponentCadPosX(), GetComponentCadPosY(), GetComponentAngle());
	::_stprintf(TextBuffer, _T("Offset(%.0f, %.0f, %.2f)"), GetComponentCadPosX(), GetComponentCadPosY(), GetComponentAngle());
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::GetComponentLocationText(CString &Text) const
{
	//Text.Format( _T("Pos(%.0f, %.0f) um, Angle=%.2f"), GetComponentCadPosX(), GetComponentCadPosY(), GetComponentAngle());
	Text.Format( _T("Cad(%.0f, %.0f, %.2f)"), GetComponentCadPosX(), GetComponentCadPosY(), GetComponentAngle());
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::GetComponentLocationText(TCHAR TextBuffer[]) const
{
	//::_stprintf(TextBuffer, _T("Pos(%.0f, %.0f) um, Angle=%.2f"), GetComponentCadPosX(), GetComponentCadPosY(), GetComponentAngle());
	::_stprintf(TextBuffer, _T("Cad(%.0f, %.0f, %.2f)"), GetComponentCadPosX(), GetComponentCadPosY(), GetComponentAngle());
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::UpdateComponentResultOffset_Lane(LANE_ID LaneID)//設定零件偏移	
{
	CAOIComponent *ResultPtr=GetComponentResultPtr();
	switch ( LaneID )
	{
	case LANE_ID_A:
		//m_ComponentResultOffsetA;//結果偏移量-角
		SetComponentResultOffsetX_LA(ResultPtr->m_ComponentResultOffsetX);
		SetComponentResultOffsetY_LA(ResultPtr->m_ComponentResultOffsetY);
		SetComponentResultSkewAngle_LA(ResultPtr->m_ComponentResultSkewAngle);
		SetComponentResultTiltAngle_LA(ResultPtr->m_ComponentResultTiltAngle);		
		break;
	case LANE_ID_B:
		//m_ComponentResultOffsetA;//結果偏移量-角
		SetComponentResultOffsetX_LB(ResultPtr->m_ComponentResultOffsetX);
		SetComponentResultOffsetY_LB(ResultPtr->m_ComponentResultOffsetY);
		SetComponentResultSkewAngle_LB(ResultPtr->m_ComponentResultSkewAngle);
		SetComponentResultTiltAngle_LB(ResultPtr->m_ComponentResultTiltAngle);		
		break;
	}
	return;
}
//-------------------------------------------------------------------------------------//
double CAOIComponent::GetComponentResultOffsetX_Lane(LANE_ID LaneID) const//取得零件偏移
{
	if ( LANE_ID_B == LaneID )
	{	return GetComponentResultOffsetX_LB(); }
	return GetComponentResultOffsetX_LA(); 
}
//-------------------------------------------------------------------------------------//
double CAOIComponent::GetComponentResultOffsetY_Lane(LANE_ID LaneID) const//取得零件偏移
{
	if ( LANE_ID_B == LaneID )
	{	return GetComponentResultOffsetY_LB(); }
	return GetComponentResultOffsetY_LA(); 
}
//-------------------------------------------------------------------------------------//
double CAOIComponent::GetComponentResultSkewAngle_Lane(LANE_ID LaneID) const//取得零件偏移	
{
	if ( LANE_ID_B == LaneID )
	{	return GetComponentResultSkewAngle_LB(); }
	return GetComponentResultSkewAngle_LA(); 
}
//-------------------------------------------------------------------------------------//
double CAOIComponent::GetComponentResultTiltAngle_Lane(LANE_ID LaneID) const//取得零件傾斜
{
	if ( LANE_ID_B == LaneID )
	{	return GetComponentResultTiltAngle_LB(); }
	return GetComponentResultTiltAngle_LA(); 
}
//-------------------------------------------------------------------------------------//
int CAOIComponent::GetComponentGrrSigmaItemIdx() const//取得零件GRR項目引數 
{
	return m_ComponentGrrSigmaItemIdx;
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentGrrSigmaItemIdx(int val)//設定零件GRR項目引數
{
	m_ComponentGrrSigmaItemIdx = val;
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::ResetComponentGrrSigmaItem(const wchar_t *Barcode, bool bChkBarcode)//重設零件檢GRR項目 	
{	
	auto &rIdx=m_ComponentGrrSigmaItemIdx;
	auto &rList=m_ComponentGrrSigmaItemList;
	const size_t Count=rList.size();
	if ( 0 == Count )
	{	return; }
	if ( -1 == rIdx )
	{
		rIdx = 0;
		auto &Ref=rList[rIdx];
		Ref.Count = 0;
		Ref.Barcode = Barcode;		
		Ref.SigmaItem = TSigmaItem();
		return;
	}
	if ( true == bChkBarcode )
	{
		for ( size_t i=0; i<Count; i++ )
		{
			const auto &Ref=rList[i];
			if ( Ref.Barcode.CompareNoCase(Barcode) == 0 )
			{
				rIdx = i;
				return;
			}
		}
	}
	const size_t MaxCount=GRR_SIGMA_ITEM_MAX_COUNT;
	if ( Count < MaxCount )
	{
		rIdx = Count;
		rList.push_back(TGrrSigmaItem());
		auto &Ref=rList[rIdx];
		Ref.Count = 0;
		Ref.Barcode = Barcode;
		//Ref.SigmaItem = TSigmaItem();
		return;
	}
	if ( Count >= MaxCount )
	{
		size_t MinIndex=0;
		unsigned int MinCount=INT_MAX;
		for ( size_t i=0; i<Count; i++ )
		{
			if ( rList[i].Count < MinCount )
			{
				MinIndex = i;
				MinCount = rList[i].Count;
			}
		}
		for ( size_t i=MinIndex; i<Count-1; i++ )
		{	rList[i]=rList[i+1];	}
		rIdx = Count-1;
		auto &Ref=rList[rIdx];
		Ref.Count = 0;
		Ref.Barcode = Barcode;
		Ref.SigmaItem = TSigmaItem();
		return;
	}
	return;
}
//-------------------------------------------------------------------------------------//
const TSigmaItem& CAOIComponent::GetComponentGrrSigmaItem() const//零件檢GRR差項目 	
{	
	size_t Idx=m_ComponentGrrSigmaItemIdx;
	const size_t Count=m_ComponentGrrSigmaItemList.size();
	if ( 0 == Count )
	{	return m_ComponentGrrSigmaItemNull.SigmaItem; }
	if ( Idx >= Count )
	{	Idx = 0;	}
	return m_ComponentGrrSigmaItemList[Idx].SigmaItem;
}
//-------------------------------------------------------------------------------------//
TGrrSigmaItem& CAOIComponent::GetComponentGrrSigmaItemUsed()//零件檢GRR項目-使用中
{
	size_t Idx=m_ComponentGrrSigmaItemIdx;
	const size_t Count=m_ComponentGrrSigmaItemList.size();
	if ( 0 == Count )
	{	return m_ComponentGrrSigmaItemNull; }
	if ( Idx >= Count )
	{	Idx = 0;	}
	return m_ComponentGrrSigmaItemList[Idx];
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::AddComponentGrrSigmaItem()//加入零件GRR項目
{	
	bool Added = true;
	const TSystemParameter &SystemParam = AOIDataCollect.GetSystemParameter();	
	UpdateComponentGrrValue(m_ComponentGrrOffsetX, m_ComponentGrrOffsetY, m_ComponentGrrSkewA, m_ComponentGrrBodyHeight, Added);
	if ( true == Added || FN_ENABLE==SystemParam.m_GrrAverageResetEnabled )
	{
		TGrrSigmaItem &GrrSigmaItem=GetComponentGrrSigmaItemUsed();
		AddComponentResultSigmaItem(GrrSigmaItem.SigmaItem); 	
		GrrSigmaItem.Count ++;
	}
}
//-------------------------------------------------------------------------------------//
const std::vector<TGrrSigmaItem>& CAOIComponent::GetComponentGrrSigmaItemList() const//零件檢測GRR項目列表 
{
	return m_ComponentGrrSigmaItemList;
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentGrrSigmaItemList(const std::vector<TGrrSigmaItem> &List)//零件檢測GRR項目列表 
{
	m_ComponentGrrSigmaItemList = List;
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::AddComponentResultSigmaItem_Lane(LANE_ID LaneID)//加入零件標準差項目
{
	if ( LANE_ID_B == LaneID )
	{	
		AddComponentResultSigmaItem(m_ComponentResultSigmaItem_LB); 
		return;
	}
	AddComponentResultSigmaItem(m_ComponentResultSigmaItem_LA); 
	return;
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::AddComponentResultSigmaItem(TSigmaItem &SigmaItem)//加入零件標準差項目
{
	CAOIComponent *ResultPtr=GetComponentResultPtr();
	SigmaItem.OffsetX.AddValue(ResultPtr->m_ComponentResultOffsetX);
	SigmaItem.OffsetY.AddValue(ResultPtr->m_ComponentResultOffsetY);
	SigmaItem.SkewAngle.AddValue(ResultPtr->m_ComponentResultSkewAngle);
	SigmaItem.BodyHeight.AddValue(ResultPtr->m_ComponentResultHeight);
	return;
}
//-------------------------------------------------------------------------------------//
const TSigmaItem& CAOIComponent::GetComponentResultSigmaItem_Lane(LANE_ID LaneID) const//零件檢測標準差項目 	
{
	if ( LANE_ID_B == LaneID )
	{	return m_ComponentResultSigmaItem_LB;	}
	return m_ComponentResultSigmaItem_LA;
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::ExecRgnDerivedInspection()//執行零件檢測
{		
	return ExecComponentInspection();
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::GetSaveComponentLog() const//取得是否儲存零件訊息
{
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::SaveComponentLog(LPCTSTR str)//儲存零件訊息
{
	if ( GetSaveComponentLog() == false ) { return true; }	
	AOIDataCollect.SaveDebugMessage_RGN(str);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::SaveComponentLog(LPCTSTR strSet, LPCTSTR str)//儲存零件訊息
{
	if ( GetSaveComponentLog() == false ) { return true; }
	CString Str2;
	CString FullName = GetComponentFullName();
	if ( NULL == strSet )
	{	Str2.Format(_T("[%s] %s"), FullName, str);	}
	else
	{	Str2.Format(_T("%s [%s] %s"), strSet, FullName, str);	}
	if ( SaveComponentLog(Str2) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::WriteComponentFile(CAOIFileIO &FileIO)//儲存零件檔案
{
#ifdef _DEBUG
	if ( FileIO.CheckFileMode() == false ) { return false; }
	if ( FileIO.CheckFileOpened() == false ) { return false; }
#endif//_DEBUG

	int     index = 0;
	CAOIComponent *pComponent = this;
	char     uuidStr[MAX_JET_PATH]="";	
	wchar_t  uuidWStr[MAX_JET_PATH]=L"";	
	UUID     uuid = pComponent->GetObjUuid();	
	FileIO.SetFnName(_T("CAOIComponent::WriteComponentFile"));
	JetAPI::UUIDToStringA(uuid, uuidStr);
	JetAPI::UUIDToStringW(uuid, uuidWStr);
	//----------------------------------------------------------------------------------------//
	CAOIModel  *ModelPtr = NULL;
	if ( FileIO.SaveChunk_INT(FILE_IO_COMPONENT_START, 0) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_COMPONENT_UNIQUE_ID, pComponent->GetComponentUniqueID()) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_COMPONENT_PANEL_INDEX, pComponent->GetComponentPanelIndex_Project()) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_COMPONENT_BOARD_INDEX, pComponent->GetComponentBoardIndex_Project()) == false ) { return false; }
	if ( FileIO.SaveChunk_STR(FILE_IO_COMPONENT_NAME, pComponent->GetComponentName()) == false ) { return false; }
	if ( FileIO.SaveChunk_STR(FILE_IO_COMPONENT_MODEL_NAME, pComponent->GetComponentModelName()) == false ) { return false; }
	if ( FileIO.SaveChunk_STR(FILE_IO_COMPONENT_PART_NUMBER, pComponent->GetComponentPartNumber()) == false ) { return false; }
	if ( FileIO.SaveChunk_STR(FILE_IO_COMPONENT_NOZZLE_NAME, pComponent->GetComponentNozzleName()) == false ) { return false; }	
	if ( FileIO.SaveChunk_INT(FILE_IO_COMPONENT_FRAME_INDEX, pComponent->GetComponentFrameIndex()) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_COMPONENT_FIELD_INDEX_RANDOM, pComponent->GetComponentFieldIndex()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_COMPONENT_ANGLE, pComponent->GetComponentAngle()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_COMPONENT_BODY_SIZE_CX, pComponent->GetComponentBodySizeW()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_COMPONENT_BODY_SIZE_CY, pComponent->GetComponentBodySizeH()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_COMPONENT_ROI_SIZE_CX, pComponent->GetComponentRoiSizeW()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_COMPONENT_ROI_SIZE_CY, pComponent->GetComponentRoiSizeH()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_COMPONENT_CAD_POS_X, pComponent->GetComponentCadPosX()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_COMPONENT_CAD_POS_Y, pComponent->GetComponentCadPosY()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_COMPONENT_ORG_CAD_POS_X, pComponent->GetComponentOrgCadPosX()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_COMPONENT_ORG_CAD_POS_Y, pComponent->GetComponentOrgCadPosY()) == false ) { return false; }	
	if ( FileIO.SaveChunk_DBL(FILE_IO_COMPONENT_STAGE_POS_X, pComponent->GetComponentStagePosX()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_COMPONENT_STAGE_POS_Y, pComponent->GetComponentStagePosY()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_COMPONENT_STAGE_POS_Z, pComponent->GetComponentStagePosZ()) == false ) { return false; }
	if ( FileIO.SaveChunk_BOL(FILE_IO_COMPONENT_BYPASS, pComponent->GetComponentBypassed()) == false ) { return false; }
	if ( FileIO.SaveChunk_BOL(FILE_IO_COMPONENT_XBOARD_UNIT, pComponent->GetComponentXBoardUnit()) == false ) { return false; }	
	if ( FileIO.SaveChunk_INT(FILE_IO_COMPONENT_MODEL_INDEX, pComponent->GetComponentModelIndex()) == false ) { return false; }	
	if ( FileIO.SaveChunk_BOL(FILE_IO_COMPONENT_MODEL_ISOLATED, pComponent->GetComponentModelIsolated()) == false ) { return false; }
	if ( FileIO.GetSaveWStr() == true ) 
	{	if ( FileIO.SaveChunk_STR(FILE_IO_COMPONENT_OBJ_UUID, uuidWStr) == false ) { return false; } }
	else
	{	if ( FileIO.SaveChunk_STR(FILE_IO_COMPONENT_OBJ_UUID, uuidStr) == false ) { return false; } }
	if ( FileIO.SaveChunk_BOL(FILE_IO_COMPONENT_BYPASS_3D, pComponent->GetComponentBypass3D()) == false ) { return false; }	

	//v1.01.04.038版本逐漸移除
	//if ( FileIO.SaveChunk_BOL(FILE_IO_COMPONENT_2DMASK_BASE_ENABLE, pComponent->GetComponentMaskEnable_Base()) == false ) { return false; }
	//if ( FileIO.SaveChunk_INT(FILE_IO_COMPONENT_2DMASK_FRAME_UNIQUE_ID, pComponent->GetComponentMaskFrameUniqueID_Base()) == false ) { return false; }
	//if ( FileIO.SaveChunk_INT(FILE_IO_COMPONENT_2DMASK_COLOR_GROUP_INDEX, pComponent->GetComponentMaskColorGroupLinkIndex()) == false ) { return false; }	

	if ( FileIO.SaveChunk_INT(FILE_IO_COMPONENT_COMPONENT_TYPE, pComponent->GetComponentType()) == false ) { return false; }		
	if ( FileIO.SaveChunk_DBL(FILE_IO_COMPONENT_MASK_EXTEND_W_BODY, pComponent->GetComponentMaskExtendW_Body()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_COMPONENT_MASK_EXTEND_H_BODY, pComponent->GetComponentMaskExtendH_Body()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_COMPONENT_MASK_EXTEND_W_LAND, pComponent->GetComponentMaskExtendW_Land()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_COMPONENT_MASK_EXTEND_H_LAND, pComponent->GetComponentMaskExtendH_Land()) == false ) { return false; }

	if ( FileIO.SaveChunk_BOL(FILE_IO_COMPONENT_SELF_FIELD_ENABLED, pComponent->GetComponentSelfFieldEnabled()) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_COMPONENT_DISTRICT_ID, pComponent->GetComponentDistrictID()) == false ) { return false; }	
	if ( FileIO.SaveChunk_BOL(FILE_IO_COMPONENT_ENABLE_ALARM, pComponent->GetComponentEnableAlarm()) == false ) { return false; }//取消輸出	
	if ( FileIO.SaveChunk_INT(FILE_IO_COMPONENT_GROUP_ID, pComponent->GetComponentGroupID()) == false ) { return false; }	
	if ( FileIO.SaveChunk_BOL(FILE_IO_COMPONENT_GROUP_ORG, pComponent->GetComponentGroupOrg()) == false ) { return false; }	
	if ( FileIO.SaveChunk_INT(FILE_IO_COMPONENT_LOCAL_BASE_PLANE_ID, pComponent->GetComponentLocalBasePlaneID()) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_COMPONENT_MODEL_CLASS_ID, pComponent->GetComponentModelClassID()) == false ) { return false; }		

	if ( FileIO.SaveChunk_INT(FILE_IO_COMPONENT_DEFECT_ALARM_FROM_MODE_AOI, pComponent->GetComponentDefectAlarmFromModeAOI()) == false ) { return false; }		
	//if ( FileIO.SaveChunk_INT(FILE_IO_COMPONENT_DEFECT_ALARM_FROM_MODE_ARS, pComponent->GetComponentDefectAlarmFromModeARS()) == false ) { return false; }				

	if ( FileIO.SaveChunk_BOL(FILE_IO_COMPONENT_DATA_MODEL_ENABLED, pComponent->GetComponentDataModelEnabled()) == false ) { return false; }	
	if ( FileIO.SaveChunk_INT(FILE_IO_COMPONENT_DATA_MODEL_LEVEL_ID, pComponent->GetComponentDataModelLevelID()) == false ) { return false; }

	if ( FileIO.SaveChunk_BOL(FILE_IO_COMPONENT_SAVE_WND_LIST, pComponent->GetComponentSaveWndList()) == false ) { return false; }	
	if ( FileIO.SaveChunk_BOL(FILE_IO_COMPONENT_SAVE_REPORT_ARS, pComponent->GetComponentSaveReport_ARS()) == false ) { return false; }		

	if ( FileIO.SaveChunk_BOL(FILE_IO_COMPONENT_DEFECT_ALARM_ENABLE_ON_AOI, pComponent->GetComponentDefectAlarmEnableOnAOI()) == false ) { return false; }		
	if ( FileIO.SaveChunk_BOL(FILE_IO_COMPONENT_DEFECT_ALARM_ENABLE_ON_ARS, pComponent->GetComponentDefectAlarmEnableOnARS()) == false ) { return false; }		
	if ( FileIO.SaveChunk_BOL(FILE_IO_COMPONENT_DEFECT_COUNT_ENABLE_ON_ARS, pComponent->GetComponentDefectCountEnableOnARS()) == false ) { return false; }			
	//if ( FileIO.SaveChunk_BOL(FILE_IO_COMPONENT_TOTAL_NG_COUNT_ENABLE, pComponent->GetComponentTotalNGCountEnable()) == false ) { return false; }		
	//if ( FileIO.SaveChunk_BOL(FILE_IO_COMPONENT_CONTINUE_NG_COUNT_ENABLE, pComponent->GetComponentContinueNGCountEnable()) == false ) { return false; }		
	//if ( FileIO.SaveChunk_INT(FILE_IO_COMPONENT_TOTAL_NG_COUNT_LIMIT, pComponent->GetComponentTotalNGCountLimit()) == false ) { return false; }	
	//if ( FileIO.SaveChunk_INT(FILE_IO_COMPONENT_CONTINUE_NG_COUNT_LIMIT, pComponent->GetComponentContinueNGCountLimit()) == false ) { return false; }	

	if ( FileIO.SaveChunk_INT(FILE_IO_COMPONENT_MASTER_INDEX, pComponent->GetComponentMasterIndex()) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_COMPONENT_COL_INDEX, pComponent->GetComponentColIndex()) == false ) { return false; }		
	if ( FileIO.SaveChunk_INT(FILE_IO_COMPONENT_ROW_INDEX, pComponent->GetComponentRowIndex()) == false ) { return false; }		

	if (FileIO.SaveChunk_INT(FILE_IO_COMPONENT_HASI_SCO_ENABLE, pComponent->GetComponentHASI_SPIOffset_Enable()) == false) { return false; }
	if (FileIO.SaveChunk_INT(FILE_IO_COMPONENT_M2M_SAVE_IMAGE, pComponent->GetComponentHASI_SaveImage()) == false) { return false; }
	if (FileIO.SaveChunk_INT(FILE_IO_COMPONENT_SPECIAL_CAD_POS_X, pComponent->GetComponentSpecialCadPosX()) == false) { return false; }
	if (FileIO.SaveChunk_INT(FILE_IO_COMPONENT_SPECIAL_CAD_POS_Y, pComponent->GetComponentSpecialCadPosY()) == false) { return false; }

	const TNoiseFilterParam &NoiseFilterParam = pComponent->GetComponentSpaceNoiseFilterParam();	
	if ( FileIO.SaveChunk_INT(FILE_IO_COMPONENT_3D_NOISE_FILTER_NODE, 0) == false ) { return false; }
	if ( FileIO.WriteSpaceNoistFilterParamFile(NoiseFilterParam) == false ) { return false; }

	if ( true == pComponent->GetComponentModelIsolated() )
	{
		ModelPtr = pComponent->GetComponentModelPtr();
		if ( NULL != ModelPtr )
		{
			if ( FileIO.SaveChunk_INT(FILE_IO_COMPONENT_MODEL_NODE, 0) == false ) { return false; }	
			
			CString ModelFolder;
			CString ComponentName = pComponent->GetComponentName();
			CString FullComponentName = pComponent->GetComponentFullName();
			const double ComponentAngle = pComponent->GetComponentAngle();
			const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComponentAngle);

			ModelFolder.Format(_T("%s\\%s"), FileIO.GetPartLibraryFolder(), FullComponentName);										
			ModelPtr->SetModelFolderComponent(ModelFolder);
			if ( true == IsExceptionAngle )
			{	
				ModelPtr->RotateModel(-ComponentAngle, 0.0, 0.0); 
				if ( ModelPtr->WriteModelFile(FileIO) == false ) { return false; }
				ModelPtr->RotateModel(ComponentAngle, 0.0, 0.0);
			}
			else
			{
				if ( ModelPtr->WriteModelFile(FileIO) == false ) { return false; }			
			}			
		}
	}

	if ( FileIO.SaveChunk_INT(FILE_IO_COMPONENT_VERSION_PARAM_NODE, 0) == false ) { return false; }	
	if ( FileIO.WriteVersionParamListFile(m_ComponentVersionParamList) == false ) { return false; }	
	
	if ( m_ComponentDefectItemAlarmAOI.CalcAllItemCount() > 0  )
	{
		if ( FileIO.SaveChunk_INT(FILE_IO_COMPONENT_DEFECT_ITEM_ALARM_AOI, 0) == false ) { return false; }	
		if ( m_ComponentDefectItemAlarmAOI.WriteWndDefectItemFile(FileIO) == false ) { return false; }		
	}
	if ( m_ComponentDefectItemAlarmARS.CalcAllItemCount() > 0  )
	{	//取消輸出
		//if ( FileIO.SaveChunk_INT(FILE_IO_COMPONENT_DEFECT_ITEM_ALARM_ARS, 0) == false ) { return false; }	
		//if ( m_ComponentDefectItemAlarmARS.WriteWndDefectItemFile(FileIO) == false ) { return false; }		
	}

	if ( FileIO.SaveChunk_INT(FILE_IO_COMPONENT_END, 0) == false ) { return false; }		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::ReadComponentFile(CAOIFileIO &FileIO)//載入零件檔案
{
#ifdef _DEBUG
	if ( FileIO.CheckFileMode() == false ) { return false; }
	if ( FileIO.CheckFileOpened() == false ) { return false; }
#endif//_DEBUG

	UUID    uuid;
	int     index = 0;
	double  dValue = 0.0;		
	CAOIComponent *pComponent = this;	
	FileIO.SetFnName(_T("CAOIComponent::ReadComponentFile"));
	//----------------------------------------------------------------------------------------//
	bool       bIsolated=false;
	double     ComponentAngle=0;
	CString    ModelName;	
	CString    ModelFolder;	
	CString    ComponentName;
	CString    FullComponentName;
	MODEL_TYPE ModelType;
	CAOIModel *ModelPtr = NULL;
	BOX_TOWARD ModelBodyToward;	
	const double dPrecision = 0.001;
	std::vector<TVersionParam> VersionList;
	//----------------------------------------------------------------------------------------//
	//有設定才有讀取, 所以設定預設值
	m_ComponentDefectItemAlarmAOI.SetAllItemCount(0);
	m_ComponentDefectItemAlarmARS.SetAllItemCount(0);
	//----------------------------------------------------------------------------------------//
	while ( FileIO.CheckFileEnd()==false )
	{ 		
		if ( FileIO.LoadChunk(index) == false )		
		{	continue; }		
		
		switch ( index )
		{
		case FILE_IO_COMPONENT_START://零件參數-起點
			break;
		case FILE_IO_COMPONENT_END://零件參數-終點
			ModelPtr = pComponent->GetComponentModelPtr();
			ComponentAngle = pComponent->GetComponentAngle();
			if ( ModelPtr->CheckModelTypeEnabled() == false )
			{
				if ( ::fabs(ComponentAngle-90) < dPrecision )
				{	ModelBodyToward = BOX_TOWARD_UP;	}
				else if ( ::fabs(ComponentAngle-180) < dPrecision )
				{	ModelBodyToward = BOX_TOWARD_LEFT;	}
				else if ( ::fabs(ComponentAngle-270) < dPrecision )
				{	ModelBodyToward = BOX_TOWARD_DOWN;	}
				else
				{	ModelBodyToward = BOX_TOWARD_RIGHT;	}
				ModelPtr->GetModelBodyBox().SetBoxToward(ModelBodyToward); 
			}
			pComponent->CalcComponentCadCornerPos();
			pComponent->LayoutComponentStageCornerPos();
			pComponent->UpdateComponentParamToModel(false);
			return true;
			break;
		case FILE_IO_COMPONENT_UNIQUE_ID:
			pComponent->SetComponentUniqueID(FileIO.GetData_INT());
			break;
		case FILE_IO_COMPONENT_PANEL_INDEX://零件參數-的整板引數
			pComponent->SetComponentPanelIndex_Project(FileIO.GetData_INT());
			break;
		case FILE_IO_COMPONENT_BOARD_INDEX://零件參數-的單板引數
			pComponent->SetComponentBoardIndex_Project(FileIO.GetData_INT());
			break;
		case FILE_IO_COMPONENT_NAME://零件參數-名稱
			if ( FileIO.GetLoadWStr()==true )
			{	pComponent->SetComponentName(FileIO.GetData_WSTR());	}
			else
			{	pComponent->SetComponentName(FileIO.GetData_STR()); }
			break;
		case FILE_IO_COMPONENT_MODEL_NAME://零件參數-模組名稱
			ModelPtr = pComponent->GetComponentModelPtr();
			if ( FileIO.GetLoadWStr()==true )
			{
				pComponent->SetComponentModelName(FileIO.GetData_WSTR());
				if ( NULL != ModelPtr )
				{	ModelPtr->SetModelName(FileIO.GetData_WSTR()); }
			}
			else
			{
				pComponent->SetComponentModelName(FileIO.GetData_STR());			
				if ( NULL != ModelPtr )
				{	ModelPtr->SetModelName(FileIO.GetData_STR()); }
			}			
			break;
		case FILE_IO_COMPONENT_PART_NUMBER://零件參數-料號
			if ( FileIO.GetLoadWStr()==true )
			{	pComponent->SetComponentPartNumber(FileIO.GetData_WSTR());	}
			else
			{	pComponent->SetComponentPartNumber(FileIO.GetData_STR()); }
			break;
		case FILE_IO_COMPONENT_NOZZLE_NAME://零件參數-吸嘴名稱
			if ( FileIO.GetLoadWStr()==true )
			{	pComponent->SetComponentNozzleName(FileIO.GetData_WSTR());	}
			else
			{	pComponent->SetComponentNozzleName(FileIO.GetData_STR()); }
			break;			
		case FILE_IO_COMPONENT_FRAME_INDEX:
			pComponent->SetComponentFrameIndex(FileIO.GetData_INT());
			break;
		case FILE_IO_COMPONENT_FIELD_INDEX_RANDOM://零件參數-的區域引數
			pComponent->SetComponentFieldIndex(FileIO.GetData_INT());
			break;
		case FILE_IO_COMPONENT_ANGLE://零件參數-的角度
			pComponent->SetComponentAngle(FileIO.GetData_DBL());
			break;
		case FILE_IO_COMPONENT_BODY_SIZE_CX://零件參數-本體的尺寸-寬度-um			
			pComponent->SetComponentBodySizeW(FileIO.GetData_DBL());
			break;
		case FILE_IO_COMPONENT_BODY_SIZE_CY://零件參數-本體的尺寸-高度-um			
			pComponent->SetComponentBodySizeH(FileIO.GetData_DBL());
			break;
		case FILE_IO_COMPONENT_ROI_SIZE_CX://零件參數-的尺寸-寬度-um
			dValue = FileIO.GetData_DBL();
			pComponent->SetComponentRoiSizeW(dValue);			
			break;
		case FILE_IO_COMPONENT_ROI_SIZE_CY://零件參數-的尺寸-高度-um
			dValue = FileIO.GetData_DBL();
			pComponent->SetComponentRoiSizeH(dValue);			
			break;
		case FILE_IO_COMPONENT_CAD_POS_X://零件參數-的Cad座標-X-um
			dValue = FileIO.GetData_DBL();
			pComponent->SetComponentCadPosX(dValue);			
			break;
		case FILE_IO_COMPONENT_CAD_POS_Y://零件參數-的Cad座標-Y-um
			dValue = FileIO.GetData_DBL();
			pComponent->SetComponentCadPosY(dValue);			
			break;
		case FILE_IO_COMPONENT_ORG_CAD_POS_X://零件參數-的原始Cad座標-X-um			
			pComponent->SetComponentOrgCadPosX(FileIO.GetData_DBL());
			break;
		case FILE_IO_COMPONENT_ORG_CAD_POS_Y://零件參數-的原始Cad座標-Y-um
			pComponent->SetComponentOrgCadPosY(FileIO.GetData_DBL());
			break;
		case FILE_IO_COMPONENT_STAGE_POS_X://零件參數-的機台座標-X-um
			pComponent->SetComponentStagePosX(FileIO.GetData_DBL());
			break;
		case FILE_IO_COMPONENT_STAGE_POS_Y://零件參數-的機台座標-Y-um
			pComponent->SetComponentStagePosY(FileIO.GetData_DBL());
			break;
		case FILE_IO_COMPONENT_STAGE_POS_Z://零件參數-的機台座標-Z-um
			pComponent->SetComponentStagePosZ(FileIO.GetData_DBL());
			break;
		case FILE_IO_COMPONENT_BYPASS://零件參數-不檢測
			pComponent->SetComponentBypassed(FileIO.GetData_BOL());
			break;
		case FILE_IO_COMPONENT_XBOARD_UNIT://零件參數-X板單位
			pComponent->SetComponentXBoardUnit(FileIO.GetData_BOL());
			break;		
		case FILE_IO_COMPONENT_MODEL_INDEX://零件參數-模組引數
			pComponent->SetComponentModelIndex(FileIO.GetData_INT());
			break;
		case FILE_IO_COMPONENT_MODEL_ISOLATED://零件參數-是否模組隔離
			bIsolated = FileIO.GetData_BOL();
			pComponent->SetComponentModelIsolated(bIsolated);
			break;
		case FILE_IO_COMPONENT_OBJ_UUID://零件參數-的UUID
			if ( FileIO.GetLoadWStr()==true )
			{
				if ( JetAPI::UUIDFromStringW(FileIO.GetData_WSTR(), uuid) == true )
				{	pComponent->SetObjUuid(uuid);	}
			}
			else
			{
				if ( JetAPI::UUIDFromStringA(FileIO.GetData_STR(), uuid) == true )
				{	pComponent->SetObjUuid(uuid);	}
			}
			break;
		case FILE_IO_COMPONENT_BYPASS_3D://零件參數-是否3D不檢測
			pComponent->SetComponentBypass3D(FileIO.GetData_BOL());
			break;
		
		case FILE_IO_COMPONENT_2DMASK_BASE_ENABLE://零件參數-基準面2D遮罩-啟用
			pComponent->SetComponentMaskEnable_Base(FileIO.GetData_BOL());
			break;
		case FILE_IO_COMPONENT_2DMASK_FRAME_UNIQUE_ID://零件參數-基準面2D遮罩-唯一碼
			pComponent->SetComponentMaskFrameUniqueID_Base(FileIO.GetData_INT());
			break;
		case FILE_IO_COMPONENT_2DMASK_COLOR_GROUP_INDEX://零件參數-基準面2D遮罩-專案顏色序號
			pComponent->SetComponentMaskColorGroupLinkIndex(FileIO.GetData_INT());
			break;		
		
		case FILE_IO_COMPONENT_COMPONENT_TYPE://零件參數-零件樣式
			pComponent->SetComponentType((COMPONENT_TYPE)FileIO.GetData_INT());
			break;
		case FILE_IO_COMPONENT_MASK_EXTEND_W_BODY://零件參數-遮罩外擴寬度-本體
			pComponent->SetComponentMaskExtendW_Body(FileIO.GetData_DBL());
			break;
		case FILE_IO_COMPONENT_MASK_EXTEND_H_BODY://零件參數-遮罩外擴長度-本體
			pComponent->SetComponentMaskExtendH_Body(FileIO.GetData_DBL());
			break;
		case FILE_IO_COMPONENT_MASK_EXTEND_W_LAND://零件參數-遮罩外擴寬度-焊盤
			pComponent->SetComponentMaskExtendW_Land(FileIO.GetData_DBL());
			break;
		case FILE_IO_COMPONENT_MASK_EXTEND_H_LAND://零件參數-遮罩外擴長度-焊盤
			pComponent->SetComponentMaskExtendH_Land(FileIO.GetData_DBL());
			break;

		case FILE_IO_COMPONENT_DISTRICT_ID://零件參數-分段編號
			pComponent->SetComponentDistrictID((DISTRICT_ID)(FileIO.GetData_INT()));
			break;
		case FILE_IO_COMPONENT_SELF_FIELD_ENABLED://零件參數-專屬區域啟用
			pComponent->SetComponentSelfFieldEnabled(FileIO.GetData_BOL());
			break;
		case FILE_IO_COMPONENT_ENABLE_ALARM://零件參數-啟用警報
			pComponent->SetComponentEnableAlarm(FileIO.GetData_BOL());			
			break;		
		case FILE_IO_COMPONENT_GROUP_ID://零件參數-群組編號
			pComponent->SetComponentGroupID(FileIO.GetData_INT());
			break;		
		case FILE_IO_COMPONENT_GROUP_ORG://零件參數-群組原點
			pComponent->SetComponentGroupOrg(FileIO.GetData_BOL());
			break;		
		case FILE_IO_COMPONENT_LOCAL_BASE_PLANE_ID://零件參數-局部基準面編號
			pComponent->SetComponentLocalBasePlaneID(FileIO.GetData_INT());
			break;
		case FILE_IO_COMPONENT_MODEL_CLASS_ID://零件參數-模組類別編號(指定)
			pComponent->SetComponentModelClassID(FileIO.GetData_INT());
			break;

		case FILE_IO_COMPONENT_DEFECT_ALARM_FROM_MODE_AOI://零件參數-瑕疵警報來源模式-AOI
			pComponent->SetComponentDefectAlarmFromModeAOI((DEFECT_PARAM_FROM_MODE)(FileIO.GetData_INT()));
			break;
		case FILE_IO_COMPONENT_DEFECT_ALARM_FROM_MODE_ARS://零件參數-瑕疵警報來源模式-ARS
			pComponent->SetComponentDefectAlarmFromModeARS((DEFECT_PARAM_FROM_MODE)(FileIO.GetData_INT()));
			break;		

		case FILE_IO_COMPONENT_DATA_MODEL_ENABLED://零件參數-資料模組啟用
			pComponent->SetComponentDataModelEnabled(FileIO.GetData_BOL());
			break;
		case FILE_IO_COMPONENT_DATA_MODEL_LEVEL_ID://零件參數-資料模組等級
			pComponent->SetComponentDataModelLevelID(FileIO.GetData_INT());
			break;
		case FILE_IO_COMPONENT_SAVE_WND_LIST://零件參數-儲存檢測框列表
			pComponent->SetComponentSaveWndList(FileIO.GetData_BOL());
			break;
		case FILE_IO_COMPONENT_SAVE_REPORT_ARS://零件參數-零件儲存報告-維修站
			pComponent->SetComponentSaveReport_ARS(FileIO.GetData_BOL());
			break;

		case FILE_IO_COMPONENT_DEFECT_ALARM_ENABLE_ON_AOI://零件參數-瑕疵警報設備警報
			pComponent->SetComponentDefectAlarmEnableOnAOI(FileIO.GetData_BOL());
			break;
		case FILE_IO_COMPONENT_DEFECT_ALARM_ENABLE_ON_ARS://零件參數-瑕疵警報維修站顯示
			pComponent->SetComponentDefectAlarmEnableOnARS(FileIO.GetData_BOL());
			break;
		case FILE_IO_COMPONENT_DEFECT_COUNT_ENABLE_ON_ARS://零件參數-瑕疵計數維修站顯示
			pComponent->SetComponentDefectCountEnableOnARS(FileIO.GetData_BOL());
			break;
		case FILE_IO_COMPONENT_TOTAL_NG_COUNT_ENABLE://零件參數-累計瑕疵上限啟用
			//pComponent->SetComponentTotalNGCountEnable(FileIO.GetData_BOL());
			break;
		case FILE_IO_COMPONENT_CONTINUE_NG_COUNT_ENABLE://零件參數-連續瑕疵上限啟用
			//pComponent->SetComponentContinueNGCountEnable(FileIO.GetData_BOL());			
			break;
		case FILE_IO_COMPONENT_TOTAL_NG_COUNT_LIMIT://零件參數-累計瑕疵上限上限
			//pComponent->SetComponentTotalNGCountLimit(FileIO.GetData_INT());
			break;
		case FILE_IO_COMPONENT_CONTINUE_NG_COUNT_LIMIT://零件參數-連續瑕疵上限上限
			//pComponent->SetComponentContinueNGCountLimit(FileIO.GetData_INT());
			break;
		case FILE_IO_COMPONENT_MASTER_INDEX://零件參數-本尊引數
			pComponent->SetComponentMasterIndex(FileIO.GetData_INT());
			break;
		case FILE_IO_COMPONENT_COL_INDEX://零件參數-欄引數-X
			pComponent->SetComponentColIndex(FileIO.GetData_INT());
			break;
		case FILE_IO_COMPONENT_ROW_INDEX://零件參數-列引數-Y
			pComponent->SetComponentRowIndex(FileIO.GetData_INT());
			break;

		case FILE_IO_COMPONENT_HASI_SCO_ENABLE://零件參數-HAS I-套用SPI校正
			pComponent->SetComponentHASI_SPIOffset_Enable(FileIO.GetData_INT());
			break;
		case FILE_IO_COMPONENT_M2M_SAVE_IMAGE://零件參數-M2M專用-保存圖片至M2M資料夾
			pComponent->SetComponentHASI_SaveImage(FileIO.GetData_INT());
			break;
		case FILE_IO_COMPONENT_SPECIAL_CAD_POS_X://零件參數-以PCB左下角為O點的CAD座標-X
			pComponent->SetComponentSpecialCadPosX(FileIO.GetData_INT());
			break;
		case FILE_IO_COMPONENT_SPECIAL_CAD_POS_Y://零件參數-以PCB左下角為O點的CAD座標-Y
			pComponent->SetComponentSpecialCadPosY(FileIO.GetData_INT());
			break;

		case FILE_IO_COMPONENT_3D_NOISE_FILTER_NODE:
			if ( FileIO.ReadSpaceNoiseFilterParamFile(m_RgnSpaceNoiseFilterParam) == false ) { return false; }
			break;			
		case FILE_IO_COMPONENT_MODEL_NODE:
			ModelPtr = pComponent->GetComponentModelPtr();
			if ( NULL != ModelPtr )
			{
				ModelName = ModelPtr->GetModelName();
				ModelFolder.Format(_T("%s\\%s"), FileIO.GetLibraryFolder(), ModelName);
				ModelPtr->SetModelFolderModel(ModelFolder);	
				ModelPtr->SetModelFolderComponent(_T(""));
				if ( true == bIsolated ) 
				{					
					FullComponentName = pComponent->GetComponentFullName();//此時還不知道單板在整板內的編號
					//ModelFolder.Format(_T("%s\\%s"), FileIO.GetPartLibraryFolder(), FullComponentName);										
					//ModelPtr->SetModelFolderComponent(ModelFolder);
				}
				if ( ModelPtr->ReadModelFile(FileIO) == false )
				{	return false; }				
			}
			break;			
		case FILE_IO_COMPONENT_VERSION_PARAM_NODE:
			if ( FileIO.ReadVersionParamListFile(VersionList) == false )
			{	return false; }
			pComponent->SetComponentVersionParamList(VersionList);
			break;
	
		case FILE_IO_COMPONENT_DEFECT_ITEM_ALARM_AOI://零件參數-瑕疵項目警報-AOI
			if ( m_ComponentDefectItemAlarmAOI.ReadWndDefectItemFile(FileIO) == false )			
			{	return false; }
			break;			
		case FILE_IO_COMPONENT_DEFECT_ITEM_ALARM_ARS://零件參數-瑕疵項目警報-ARS
			if ( m_ComponentDefectItemAlarmARS.ReadWndDefectItemFile(FileIO) == false )			
			{	return false; }
			break;
			
		default:
			break;
		}
	};		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::WriteComponentReportText(FILE *pfile)//儲存零件報告
{
	CAOIComponent *ComponentPtr=this;
	if ( NULL == pfile ) { return false; }
	if ( NULL == ComponentPtr ) { return false; }
	const int      TempSize=8;
	double         Tilt[TempSize]={0};
	double         Height[TempSize]={0};
	double         TempDbl[TempSize]={0};	
	double         TempTilt[TempSize]={0};		
	int            TempTiltID[TempSize]={0};

	RESULT_ID ResultID = ComponentPtr->GetComponentResultID_AOI();		
	CAOIModel *ModelPtr = ComponentPtr->GetComponentModelPtr();
	const int WndCount = (int)(ModelPtr->GetModelWndCount());
		
	unsigned int PanelIndex = ComponentPtr->GetComponentPanelIndex_Project();
	unsigned int BoardIndex = ComponentPtr->GetComponentBoardIndex_Panel();
	CString strComponentName = ComponentPtr->GetComponentName();
		
	double CadX = ComponentPtr->GetComponentCadPosX();
	double CadY = ComponentPtr->GetComponentCadPosY();
	double Angle = ComponentPtr->GetComponentAngle();
	double SizeX = ComponentPtr->GetComponentResultWidth();
	double SizeY = ComponentPtr->GetComponentResultLength();
	double OffsetX = ComponentPtr->GetComponentResultOffsetX();
	double OffsetY = ComponentPtr->GetComponentResultOffsetY();
	double OffsetA = ComponentPtr->GetComponentResultOffsetA();	
	double Skew = ComponentPtr->GetComponentResultSkewAngle();
	double RelOffsetX = ComponentPtr->GetComponentGroupOffsetResX();
	double RelOffsetY = ComponentPtr->GetComponentGroupOffsetResY();
	double RelDistX = ComponentPtr->GetComponentGroupDistanceResX();
	double RelDistY = ComponentPtr->GetComponentGroupDistanceResY();
	double RelDistL = sqrt((RelDistX*RelDistX)+(RelDistY*RelDistY));	
		
	int j=0, k=0;
	int nTilts=0;
	int nHeights=0;
	CString strFullName;
	for ( j=0; j<TempSize; j++ )
	{	TempTiltID[j]=-1; }
	::memset(Tilt, 0x00, sizeof(Tilt));
	::memset(Height, 0x00, sizeof(Height));
	::memset(TempDbl, 0x00, sizeof(TempDbl));				
	::memset(TempTilt, 0x00, sizeof(TempTilt));			
	for ( j=WndCount-1; j!=-1; j-- )
	{
		CAOIWnd *WndPtr = ModelPtr->GetModelWndPtr(j, false);
		if ( NULL == WndPtr ) { continue; }
		if ( NULL != WndPtr->GetWndLandPtr() ) { continue; }

		ALG_TYPE AlgType = WndPtr->GetWndAlgType();
		if ( ALG_BRIGHT_RATIO != AlgType ) { continue; }
		if ( WndPtr->GetWndEnabled() == false ) { continue; }
		const int WndGroupID = WndPtr->GetWndGroupID();
		unsigned int FrameUniqueID = WndPtr->GetWndAlgParam().GetAlgImageBinParam().GetBinaryFrameUniqueID();
		if ( FRAME_UNIQUE_ID_DLP != FrameUniqueID ) { continue; }
			
		if ( true == WndPtr->GetWndAlgParam().GetAlgParamGroupCompare().gcTiltAngleEnabled )
		{
			if ( nTilts < 4 )
			{				
				double Tilte = WndPtr->GetWndAlgParam().GetAlgParamGroupCompare().gcTiltAngleReading;
				for ( k=0; k<nTilts; k++ )//找出同群組內的最大傾斜角度
				{
					if ( TempTiltID[k] == WndGroupID )
					{
						if ( fabs(TempTilt[k]) < fabs(Tilte) )
						{	TempTilt[k] = Tilte;	}
						break;
					}
				}
				if ( k == nTilts )					
				{
					TempTilt[nTilts] = Tilte;
					TempTiltID[nTilts] = WndGroupID;
					nTilts ++;
				}
			}
		}
		if ( nHeights < 4 )
		{
			TempDbl[nHeights] = WndPtr->GetWndAlgParam().GetAlgParamBrightRatio().brAverageReading;
			nHeights ++;
		}	
	}
	for ( k=0; k<nTilts; k++ )
	{	Tilt[k] = TempTilt[nTilts-k-1];	}
	for ( k=0; k<nHeights; k++ )
	{	Height[k] = TempDbl[nHeights-k-1];	}

	int LeadID = 0;
	//Height[0] = ComponentPtr->GetComponentResultHeight();
	int BodyHeightIdx=-1;
	double BodyHeightDif=0.0;	
	const double BodyHeight= ComponentPtr->GetComponentResultHeight();	
	for ( j=0; j<nHeights; j++ )
	{
		BodyHeightDif = Height[j]-BodyHeight;
		if ( fabs(BodyHeightDif) < 0.0001 )
		{	BodyHeightIdx = j;	}		
	}
	if ( BodyHeightIdx > 0 )//將零件高度調到第1個位置
	{	JetAPI::Swap(Height[0], Height[BodyHeightIdx]);	}
	
	CString strResultText = AOIDataDefine.GetResultIDText(ResultID);		
	strFullName.Format(_T("%s#%d#%d"), (LPCTSTR)strComponentName, PanelIndex+1, BoardIndex+1);
	::_ftprintf(pfile, _T("%d, %d, %s, %s, %d, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %s\n"), 
		PanelIndex+1, BoardIndex+1, (LPCTSTR)strComponentName, (LPCTSTR)strFullName, LeadID, 
		CadX, CadY, Angle,
		OffsetX, OffsetY, OffsetA, Skew, 
		SizeX, SizeY, 
		Height[0],  Height[1], Height[2], Height[3], 
		RelOffsetX, RelOffsetY,
		RelDistX, RelDistY, RelDistL,
		Tilt[0], Tilt[1],
		(LPCTSTR)strResultText
		);


	//儲存Lead的數值
	if ( ModelPtr->GetModelSaveLeadReport() )
	{	
		TREGION4D BoxRgn;
		TSpecResult szW, szH;
		TSpecResult rH, rA, rV;
		TSpecResult sX, sY, sA, tA, oA;		
		const size_t LandCount = ModelPtr->GetModelLandCount();
		::memset(Tilt, 0x00, sizeof(Tilt));
		::memset(Height, 0x00, sizeof(Height));
		RelOffsetX = RelOffsetY = 0.0;
		RelDistX = RelDistY = RelDistL = 0.0;
		for ( j=0; j<LandCount; j++ )
		{
			CAOILand *LandPtr = ModelPtr->GetModelLandPtr(j, false);
			if ( NULL == LandPtr ) { continue; }
			const CAOIBox &LeadBoxRef = LandPtr->GetLandLeadBox();
			if ( LeadBoxRef.GetBoxEnabled() == false ) { continue; }

			LeadID = j+1;			
			LeadBoxRef.GetBoxRegionCad(BoxRgn);
			Angle= 0;
			CadX = BoxRgn.GetCpX();
			CadY = BoxRgn.GetCpY();
			if ( RESULT_ID_SKIP==ResultID || RESULT_ID_BYPASS==ResultID )
			{
				Height[0] = 0.0;
				OffsetX = OffsetY = OffsetA = Skew = 0.0;					
				SizeX = SizeY = 0.0;
			}
			else
			{
				ModelPtr->CheckModelSpecResult(LandPtr, sX, sY, sA, tA, rH, rA, rV, szW, szH, oA, ResultID);
				if ( INVALID_DOUBLE == sX.sValue )	{	OffsetX = 0; }
				else {	OffsetX = sX.sValue; }

				if ( INVALID_DOUBLE == sY.sValue )	{	OffsetY = 0; }
				else {	OffsetY = sY.sValue; }

				if ( INVALID_DOUBLE == oA.sValue )	{	OffsetA = 0; }
				else {	OffsetA = oA.sValue; }				

				if ( INVALID_DOUBLE == sA.sValue )	{	Skew = 0; }
				else {	Skew = sA.sValue; }				 

				if ( INVALID_DOUBLE == rH.sValue )	{	Height[0] = 0.0; }
				else {	Height[0] = rH.sValue; }

				if ( INVALID_DOUBLE == szW.sValue )	{	SizeX = 0; }
				else {	SizeX = szW.sValue; }

				if ( INVALID_DOUBLE == szH.sValue )	{	SizeY = 0; }
				else {	SizeY = szH.sValue; }
				strResultText = AOIDataDefine.GetResultIDText(ResultID);
			}
				
			::_ftprintf(pfile, _T("%d, %d, %s, %s, %d, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %s\n"), 
				PanelIndex+1, BoardIndex+1, (LPCTSTR)strComponentName, (LPCTSTR)strFullName, LeadID, 
				CadX, CadY, Angle,
				OffsetX, OffsetY, OffsetA, Skew, 
				SizeX, SizeY, 
				Height[0],  Height[1], Height[2], Height[3], 
				RelOffsetX, RelOffsetY,
				RelDistX, RelDistY, RelDistL,
				Tilt[0], Tilt[1],
				(LPCTSTR)strResultText
				);
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::WriteComponentSpcFile_JSON_VRS(FILE *pfile, CAOIProject *ProjectPtr)
{
	if ( NULL == pfile ) { return false; }
	if ( NULL == ProjectPtr ) { return false; }

	CAOIComponent *ComponentPtr = this;
	if ( NULL == ComponentPtr ) { return false; }	
	ComponentPtr = ComponentPtr->GetComponentResultPtr();	

	size_t           u=0;
	//TREGION4D        MapRgn;	
	TREGION4D        Region;
	TREGION4D        ModelRgn;
	TREGION4D        ImageRgn;
	//TPOINT2D         ModelRgnCp;
	CString          strText;
	CAOIWnd         *WndPtr = NULL;	
	CAOIModel       *ModelPtr = NULL;
	double           MapPosX=0;
	double           MapPosY=0;	
	double           StagePosX=0;
	double           StagePosY=0;	
	double           MapCornerPosX[4]={0};
	double           MapCornerPosY[4]={0};
	TPOINT2D         CadCornerPos[4];
	TPOINT2D         ImageCornerPos[4];
	double           StageCornerPosX[4]={0};
	double           StageCornerPosY[4]={0};
	//IMAGE_SIZE       ModelImageW=0;
	//IMAGE_SIZE       ModelImageH=0;
	std::vector<int> NGWndIdxList;

	const size_t     szBuffer = 256;
	wchar_t          strTag[szBuffer] = L"";
	wchar_t          strBuffer[szBuffer] = L"";	
	
	LANE_ID    LaneID = ProjectPtr->GetProjectActLaneID();	
	DISTRICT_ID DistrictID = ProjectPtr->GetProjectActDistrictID();	
	const double     ImageResX = AOIDataCollect.GetCameraResolutionX(PRIMARY_CAMERA_ID);
	const double     ImageResY = AOIDataCollect.GetCameraResolutionY(PRIMARY_CAMERA_ID);
	const TPOINT2D   ImageRes(ImageResX, ImageResY);

	::wcscpy(strTag, L"Name");	
	strText = ComponentPtr->GetComponentName();
	JetAPI::TCHAR2wchar(strText, strBuffer, szBuffer);
	::fwprintf(pfile, L"        \"%s\": \"%s\",\n", strTag, strBuffer);

	::wcscpy(strTag, L"Type");
	::fwprintf(pfile, L"        \"%s\": %d,\n", strTag, ComponentPtr->GetComponentType());

	::wcscpy(strTag, L"Model");	
	strText = ComponentPtr->GetComponentModelName();
	JetAPI::TCHAR2wchar(strText, strBuffer, szBuffer);
	::fwprintf(pfile, L"        \"%s\": \"%s\",\n", strTag, strBuffer);

	::wcscpy(strTag, L"PN");	
	strText = ComponentPtr->GetComponentPartNumber();
	JetAPI::TCHAR2wchar(strText, strBuffer, szBuffer);
	::fwprintf(pfile, L"        \"%s\": \"%s\",\n", strTag, strBuffer);

	::wcscpy(strTag, L"Nozzle");	
	strText = ComponentPtr->GetComponentNozzleName();
	JetAPI::TCHAR2wchar(strText, strBuffer, szBuffer);
	::fwprintf(pfile, L"        \"%s\": \"%s\",\n", strTag, strBuffer);				
				
	StagePosX=ComponentPtr->GetComponentStagePosX();
	StagePosY=ComponentPtr->GetComponentStagePosY();				
	DistrictID = ComponentPtr->GetComponentDistrictID();
	ProjectPtr->MapProjectStageToMapPos(StagePosX, StagePosY, MapPosX, MapPosY, LaneID, DistrictID);
	//::wcscpy(strTag, L"Loc");//"Loc" : [100,100],		// 在底圖上的位置(x y)(pixel)				
	//::fwprintf(pfile, L"        \"%s\": [%.0f,%.0f],\n", strTag, MapPosX, MapPosY);

	ComponentPtr->GetComponentRoiStageCornerPosX(StageCornerPosX);
	ComponentPtr->GetComponentRoiStageCornerPosY(StageCornerPosY);
	ProjectPtr->MapProjectStageToMapPos(StageCornerPosX[0], StageCornerPosY[0], MapCornerPosX[0], MapCornerPosY[0], LaneID, DistrictID);
	ProjectPtr->MapProjectStageToMapPos(StageCornerPosX[1], StageCornerPosY[1], MapCornerPosX[1], MapCornerPosY[1], LaneID, DistrictID);
	ProjectPtr->MapProjectStageToMapPos(StageCornerPosX[2], StageCornerPosY[2], MapCornerPosX[2], MapCornerPosY[2], LaneID, DistrictID);
	ProjectPtr->MapProjectStageToMapPos(StageCornerPosX[3], StageCornerPosY[3], MapCornerPosX[3], MapCornerPosY[3], LaneID, DistrictID);
	::wcscpy(strTag, L"Loc");//"Location" : [100,100,300,200,100,100,300,200],	// 在底圖上的位置(LTx LTy RTx RTy LBx LBy RBx RBy)(pixel)
	::fwprintf(pfile, L"        \"%s\": [%.0f,%.0f,%.0f,%.0f,%.0f,%.0f,%.0f,%.0f],\n", strTag, 
		MapCornerPosX[0], MapCornerPosY[0], MapCornerPosX[1], MapCornerPosY[1], 
		MapCornerPosX[2], MapCornerPosY[2], MapCornerPosX[3], MapCornerPosY[3]);
				
	::wcscpy(strTag, L"H");
	::fwprintf(pfile, L"        \"%s\": %.2f,\n", strTag, ComponentPtr->GetComponentResultHeight());

	::wcscpy(strTag, L"A");
	::fwprintf(pfile, L"        \"%s\": %.2f,\n", strTag, ComponentPtr->GetComponentResultArea());

	::wcscpy(strTag, L"V");
	::fwprintf(pfile, L"        \"%s\": %.2f,\n", strTag, ComponentPtr->GetComponentResultVolume());

	::wcscpy(strTag, L"Offset_X");
	::fwprintf(pfile, L"        \"%s\": %.2f,\n", strTag, ComponentPtr->GetComponentResultOffsetX());

	::wcscpy(strTag, L"Offset_Y");
	::fwprintf(pfile, L"        \"%s\": %.2f,\n", strTag, ComponentPtr->GetComponentResultOffsetY());

	::wcscpy(strTag, L"Skew");
	::fwprintf(pfile, L"        \"%s\": %.2f,\n", strTag, ComponentPtr->GetComponentResultSkewAngle());

	::wcscpy(strTag, L"Tilt");
	::fwprintf(pfile, L"        \"%s\": %.2f,\n", strTag, ComponentPtr->GetComponentResultTiltAngle());

	//20200801-輸出零件規格	
	bool bSaveA_Spec = false;//先不輸出
	bool bSaveV_Spec = false;//先不輸出
	bool bSaveSkew_Spec = false;//先不輸出
	::wcscpy(strTag, L"H_USL");
	::fwprintf(pfile, L"        \"%s\": %.2f,\n", strTag, ComponentPtr->GetComponentResultHeight_USL());
	::wcscpy(strTag, L"H_LSL");
	::fwprintf(pfile, L"        \"%s\": %.2f,\n", strTag, ComponentPtr->GetComponentResultHeight_LSL());
	::wcscpy(strTag, L"H_WndIdx");
	::fwprintf(pfile, L"        \"%s\": %d,\n", strTag, ComponentPtr->GetComponentResultHeight_WndIdx());
if ( true == bSaveA_Spec )
{
	::wcscpy(strTag, L"A_USL");
	::fwprintf(pfile, L"        \"%s\": %.2f,\n", strTag, ComponentPtr->GetComponentResultArea_USL());
	::wcscpy(strTag, L"A_LSL");
	::fwprintf(pfile, L"        \"%s\": %.2f,\n", strTag, ComponentPtr->GetComponentResultArea_LSL());
	::wcscpy(strTag, L"A_WndIdx");
	::fwprintf(pfile, L"        \"%s\": %d,\n", strTag, ComponentPtr->GetComponentResultArea_WndIdx());
}

if ( true == bSaveV_Spec )
{
	::wcscpy(strTag, L"V_USL");
	::fwprintf(pfile, L"        \"%s\": %.2f,\n", strTag, ComponentPtr->GetComponentResultVolume_USL());
	::wcscpy(strTag, L"V_LSL");
	::fwprintf(pfile, L"        \"%s\": %.2f,\n", strTag, ComponentPtr->GetComponentResultVolume_LSL());
	::wcscpy(strTag, L"V_WndIdx");
	::fwprintf(pfile, L"        \"%s\": %d,\n", strTag, ComponentPtr->GetComponentResultVolume_WndIdx());
}

	::wcscpy(strTag, L"Offset_X_USL");
	::fwprintf(pfile, L"        \"%s\": %.2f,\n", strTag, ComponentPtr->GetComponentResultOffsetX_USL());
	::wcscpy(strTag, L"Offset_X_LSL");
	::fwprintf(pfile, L"        \"%s\": %.2f,\n", strTag, ComponentPtr->GetComponentResultOffsetX_LSL());
	::wcscpy(strTag, L"Offset_X_WndIdx");
	::fwprintf(pfile, L"        \"%s\": %d,\n", strTag, ComponentPtr->GetComponentResultOffsetX_WndIdx());

	::wcscpy(strTag, L"Offset_Y_USL");
	::fwprintf(pfile, L"        \"%s\": %.2f,\n", strTag, ComponentPtr->GetComponentResultOffsetY_USL());
	::wcscpy(strTag, L"Offset_Y_LSL");
	::fwprintf(pfile, L"        \"%s\": %.2f,\n", strTag, ComponentPtr->GetComponentResultOffsetY_LSL());
	::wcscpy(strTag, L"Offset_Y_WndIdx");
	::fwprintf(pfile, L"        \"%s\": %d,\n", strTag, ComponentPtr->GetComponentResultOffsetY_WndIdx());

if ( true == bSaveSkew_Spec )
{
	::wcscpy(strTag, L"Skew_USL");
	::fwprintf(pfile, L"        \"%s\": %.2f,\n", strTag, ComponentPtr->GetComponentResultSkewAngle_USL());
	::wcscpy(strTag, L"Skew_LSL");
	::fwprintf(pfile, L"        \"%s\": %.2f,\n", strTag, ComponentPtr->GetComponentResultSkewAngle_LSL());
	::wcscpy(strTag, L"Skew_WndIdx");
	::fwprintf(pfile, L"        \"%s\": %d,\n", strTag, ComponentPtr->GetComponentResultSkewAngle_WndIdx());
}

	::wcscpy(strTag, L"Tilt_USL");
	::fwprintf(pfile, L"        \"%s\": %.2f,\n", strTag, ComponentPtr->GetComponentResultTiltAngle_USL());
	::wcscpy(strTag, L"Tilt_LSL");
	::fwprintf(pfile, L"        \"%s\": %.2f,\n", strTag, ComponentPtr->GetComponentResultTiltAngle_LSL());
	::wcscpy(strTag, L"Tilt_WndIdx");
	::fwprintf(pfile, L"        \"%s\": %d,\n", strTag, ComponentPtr->GetComponentResultTiltAngle_WndIdx());	
				
	//::wcscpy(strTag, L"Test_Code");//"Test Code" : 1024,	// 系統MultiCode
	//::fwprintf(pfile, L"        \"%s\": %d,\n", strTag, ComponentPtr->GetComponentDefectResultAOI());
			
	//::wcscpy(strTag, L"Check_Code");//"Check Code" : 1024,	// 人員MultiCode
	//::fwprintf(pfile, L"        \"%s\": %d,\n", strTag, 0);				

	RESULT_ID PartResultID = ComponentPtr->GetComponentResultID_AOI();				
	SPC_RESULT_ID SpcResultID = AOIDataCollect.MapResultIDToSpcResultID(PartResultID);

	::wcscpy(strTag, L"Test_Result");			
	::fwprintf(pfile, L"        \"%s\": %d,\n", strTag, SpcResultID);				
				
	ModelPtr = ComponentPtr->GetComponentModelPtr();
	if ( NULL == ModelPtr )
	{
		::wcscpy(strTag, L"N_Wnds");			
		::fwprintf(pfile, L"        \"%s\": %d,\n", strTag, 0);
		::wcscpy(strTag, L"N_TestWnds");			
		::fwprintf(pfile, L"        \"%s\": %d,\n", strTag, 0);
		::wcscpy(strTag, L"N_TestNgWnds");			
		::fwprintf(pfile, L"        \"%s\": %d,\n", strTag, 0);
	}
	else
	{						
		//ModelPtr->GetModelTotalRegion(ModelRgn);					
		//ModelRgnCp.x = ModelRgn.GetCpX();
		//ModelRgnCp.y = ModelRgn.GetCpY();
		//ModelImageW = JetAPI::Floor(ModelRgn.GetWidth()/ImageRes.x);
		//ModelImageH = JetAPI::Floor(ModelRgn.GetHeight()/ImageRes.y);

		int   NGWndIdx=0;
		RESULT_ID WndResultID;		
		size_t TestWndCount = 0;
		WND_LOGIC_TYPE WndLogicType;
		size_t ModelWndCount = ModelPtr->GetModelWndCount();		
		int    TotalWndCount = (int)(ModelWndCount);
		for ( u=0; u<ModelWndCount; u++ )
		{
			WndPtr = ModelPtr->GetModelWndPtr(u, false);
			if ( NULL == WndPtr ) { continue; }
			WndResultID = WndPtr->GetWndResultID();
			WndLogicType = WndPtr->GetWndLogicType();
			if ( WND_LOGIC_NONE != WndLogicType )
			{	WndResultID = WndPtr->GetWndLogicResultID(); }
			if ( RESULT_ID_NONE==WndResultID ) { continue; }						
			if ( RESULT_ID_SKIP==WndResultID ) { continue; }						
			if ( RESULT_ID_BYPASS==WndResultID ) { continue; }
			TestWndCount ++;
			if ( RESULT_ID_OK==WndResultID ) { continue; }
			NGWndIdxList.push_back(u);
		}
		const int NGWndIdxCount = (int)(NGWndIdxList.size());
					
		::wcscpy(strTag, L"N_Wnds");			
		::fwprintf(pfile, L"        \"%s\": %d,\n", strTag, TotalWndCount);
		::wcscpy(strTag, L"N_TestWnds");			
		::fwprintf(pfile, L"        \"%s\": %d,\n", strTag, TestWndCount);
		::wcscpy(strTag, L"N_TestNgWnds");			
		::fwprintf(pfile, L"        \"%s\": %d,\n", strTag, NGWndIdxCount);
		for ( u=0; u<NGWndIdxCount; u++ )
		{
			NGWndIdx = NGWndIdxList[u];
			WndPtr = ModelPtr->GetModelWndPtr(NGWndIdx, false);
			if ( NULL == WndPtr ) { continue; }
			::fwprintf(pfile, L"        \"Wnd_%d\":\n", u+1);
			::fwprintf(pfile, L"        {\n");
			::fwprintf(pfile, L"          \"WndIdx\": %d,\n", NGWndIdx);
			if ( WndPtr->WriteWndSpcFile_JSON_VRS(pfile, ProjectPtr) == false )
			{	return false; }			
			::fwprintf(pfile, L"        },\n");
		}										
	}				
	::wcscpy(strTag, L"Check_Result");//Component Check Result
	::fwprintf(pfile, L"        \"%s\": %d\n", strTag, 0);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::WriteComponentSpcFile_JSON_RSM(FILE *pfile, CAOIProject *ProjectPtr)
{
	if ( NULL == pfile ) { return false; }
	if ( NULL == ProjectPtr ) { return false; }

	CAOIComponent *ComponentPtr = this;
	if ( NULL == ComponentPtr ) { return false; }	
	ComponentPtr = ComponentPtr->GetComponentResultPtr();	

	size_t           u=0;
	//TREGION4D        MapRgn;	
	TREGION4D        Region;
	TREGION4D        ModelRgn;
	TREGION4D        ImageRgn;
	//TPOINT2D         ModelRgnCp;
	CString          strText;
	CAOIWnd         *WndPtr = NULL;	
	CAOIModel       *ModelPtr = NULL;
	double           MapPosX=0;
	double           MapPosY=0;	
	double           StagePosX=0;
	double           StagePosY=0;	
	double           MapCornerPosX[4]={0};
	double           MapCornerPosY[4]={0};
	TPOINT2D         CadCornerPos[4];
	TPOINT2D         ImageCornerPos[4];
	double           StageCornerPosX[4]={0};
	double           StageCornerPosY[4]={0};
	//IMAGE_SIZE       ModelImageW=0;
	//IMAGE_SIZE       ModelImageH=0;
	std::vector<int> NGWndIdxList;
	std::vector<int> OkWndIdxList;

	const size_t     szBuffer = 256;
	wchar_t          strTag[szBuffer] = L"";
	wchar_t          strBuffer[szBuffer] = L"";	
	
	LANE_ID    LaneID = ProjectPtr->GetProjectActLaneID();	
	DISTRICT_ID DistrictID = ProjectPtr->GetProjectActDistrictID();	
	bool        DefectAlarmEnableOnARS=GetComponentDefectAlarmEnableOnARS();
	const double     ImageResX = AOIDataCollect.GetCameraResolutionX(PRIMARY_CAMERA_ID);
	const double     ImageResY = AOIDataCollect.GetCameraResolutionY(PRIMARY_CAMERA_ID);
	const TPOINT2D   ImageRes(ImageResX, ImageResY);

	RESULT_ID PartResultID = ComponentPtr->GetComponentResultID_AOI();
	SPC_RESULT_ID SpcResultID = AOIDataCollect.MapResultIDToSpcResultID(PartResultID);
	SAVE_TEST_DATA_MODE SaveWndListMode=ProjectPtr->GetProjectSaveComponentWndListMode();
	const ALARM_LOCK_MODE AlarmLockMode=ProjectPtr->GetProjectParameter().m_AlarmLockMode;	

	bool             SaveOkWndList=false;
	if ( GetComponentSaveWndList()==true || SAVE_TEST_DATA_ENABLE==SaveWndListMode )
	{	SaveOkWndList = true;	}
	else if ( SAVE_TEST_DATA_DEFECT == SaveWndListMode )
	{
		if ( RESULT_ID_NG==PartResultID || RESULT_ID_EXCEPTION==PartResultID )
		{	SaveOkWndList = true;	}
	}
	else
	{	SaveOkWndList = false;	}

	if ( false == DefectAlarmEnableOnARS )
	{
		if ( ALARM_LOCK_ARS == AlarmLockMode )
		{
			if ( GetComponentTotalNGCountAlarm() || GetComponentContinueNGCountAlarm() )
			{	DefectAlarmEnableOnARS = true;	}
		}
	}

	//::fwprintf(pfile, L"        \"ID\":%d\n", k+1);				

	::wcscpy(strTag, L"Name");	
	strText = ComponentPtr->GetComponentName();
	JetAPI::TCHAR2wchar(strText, strBuffer, szBuffer);
	::fwprintf(pfile, L"        \"%s\": \"%s\",\n", strTag, strBuffer);

	::wcscpy(strTag, L"Type");
	::fwprintf(pfile, L"        \"%s\": %d,\n", strTag, ComponentPtr->GetComponentType());

	::wcscpy(strTag, L"Model");	
	strText = ComponentPtr->GetComponentModelName();
	JetAPI::TCHAR2wchar(strText, strBuffer, szBuffer);
	::fwprintf(pfile, L"        \"%s\": \"%s\",\n", strTag, strBuffer);

	::wcscpy(strTag, L"PN");	
	strText = ComponentPtr->GetComponentPartNumber();
	JetAPI::TCHAR2wchar(strText, strBuffer, szBuffer);
	::fwprintf(pfile, L"        \"%s\": \"%s\",\n", strTag, strBuffer);

	::wcscpy(strTag, L"Nozzle");	
	strText = ComponentPtr->GetComponentNozzleName();
	JetAPI::TCHAR2wchar(strText, strBuffer, szBuffer);
	::fwprintf(pfile, L"        \"%s\": \"%s\",\n", strTag, strBuffer);				
	
	//TagPart
	::wcscpy(strTag, L"TagPart");	
	if ( ComponentPtr->GetComponentSaveWndList() == true )
	{	::fwprintf(pfile, L"        \"%s\": %s,\n", strTag, L"true");	}
	else
	{	::fwprintf(pfile, L"        \"%s\": %s,\n", strTag, L"false");	}

	//Angle
	::wcscpy(strTag, L"Angle");	
	::fwprintf(pfile, L"        \"%s\": %.0f,\n", strTag, ComponentPtr->GetComponentAngle());

	StagePosX=ComponentPtr->GetComponentStagePosX();
	StagePosY=ComponentPtr->GetComponentStagePosY();				
	DistrictID = ComponentPtr->GetComponentDistrictID();
	ProjectPtr->MapProjectStageToMapPos(StagePosX, StagePosY, MapPosX, MapPosY, LaneID, DistrictID);
	//::wcscpy(strTag, L"Loc");//"Loc" : [100,100],		// 在底圖上的位置(x y)(pixel)				
	//::fwprintf(pfile, L"        \"%s\": [%.0f,%.0f],\n", strTag, MapPosX, MapPosY);

	ComponentPtr->GetComponentRoiStageCornerPosX(StageCornerPosX);
	ComponentPtr->GetComponentRoiStageCornerPosY(StageCornerPosY);
	ProjectPtr->MapProjectStageToMapPos(StageCornerPosX[0], StageCornerPosY[0], MapCornerPosX[0], MapCornerPosY[0], LaneID, DistrictID);
	ProjectPtr->MapProjectStageToMapPos(StageCornerPosX[1], StageCornerPosY[1], MapCornerPosX[1], MapCornerPosY[1], LaneID, DistrictID);
	ProjectPtr->MapProjectStageToMapPos(StageCornerPosX[2], StageCornerPosY[2], MapCornerPosX[2], MapCornerPosY[2], LaneID, DistrictID);
	ProjectPtr->MapProjectStageToMapPos(StageCornerPosX[3], StageCornerPosY[3], MapCornerPosX[3], MapCornerPosY[3], LaneID, DistrictID);
	::wcscpy(strTag, L"Loc");//"Location" : [100,100,300,200,100,100,300,200],	// 在底圖上的位置(LTx LTy RTx RTy LBx LBy RBx RBy)(pixel)
	::fwprintf(pfile, L"        \"%s\": [%.0f,%.0f,%.0f,%.0f,%.0f,%.0f,%.0f,%.0f],\n", strTag, 
		MapCornerPosX[0], MapCornerPosY[0], MapCornerPosX[1], MapCornerPosY[1], 
		MapCornerPosX[2], MapCornerPosY[2], MapCornerPosX[3], MapCornerPosY[3]);
				
	const RECT    &PadRect=GetRgnModelImageRect_AI();
	::wcscpy(strTag, L"PadLoc");//"PadLoc" : [100,100,300,200,100,100,300,200],	// 在零件圖上的位置(LTx LTy LBx LBy RBx RBy RTx RTy)(pixel)
	::fwprintf(pfile, L"        \"%s\": [%d,%d,%d,%d,%d,%d,%d,%d],\n", strTag, 
		PadRect.left, PadRect.top, PadRect.left, PadRect.bottom, PadRect.right, PadRect.bottom, PadRect.right, PadRect.top);

	::wcscpy(strTag, L"H");
	::fwprintf(pfile, L"        \"%s\": %.2f,\n", strTag, ComponentPtr->GetComponentResultHeight());

	::wcscpy(strTag, L"A");
	::fwprintf(pfile, L"        \"%s\": %.2f,\n", strTag, ComponentPtr->GetComponentResultArea());

	::wcscpy(strTag, L"V");
	::fwprintf(pfile, L"        \"%s\": %.2f,\n", strTag, ComponentPtr->GetComponentResultVolume());

	::wcscpy(strTag, L"Offset_X");
	::fwprintf(pfile, L"        \"%s\": %.2f,\n", strTag, ComponentPtr->GetComponentResultOffsetX());

	::wcscpy(strTag, L"Offset_Y");
	::fwprintf(pfile, L"        \"%s\": %.2f,\n", strTag, ComponentPtr->GetComponentResultOffsetY());

	::wcscpy(strTag, L"Offset_A");
	::fwprintf(pfile, L"        \"%s\": %.2f,\n", strTag, ComponentPtr->GetComponentResultOffsetA());

	::wcscpy(strTag, L"Skew");
	::fwprintf(pfile, L"        \"%s\": %.2f,\n", strTag, ComponentPtr->GetComponentResultSkewAngle());
				
	//::wcscpy(strTag, L"Test_Code");//"Test Code" : 1024,	// 系統MultiCode
	//::fwprintf(pfile, L"        \"%s\": %d,\n", strTag, ComponentPtr->GetComponentDefectResultAOI());
				
	//::wcscpy(strTag, L"Check_Code");//"Check Code" : 1024,	// 人員MultiCode
	//::fwprintf(pfile, L"        \"%s\": %d,\n", strTag, 0);

	::wcscpy(strTag, L"Test_Result");			
	::fwprintf(pfile, L"        \"%s\": %d,\n", strTag, SpcResultID);				
				
	ModelPtr = ComponentPtr->GetComponentModelPtr();
	if ( NULL == ModelPtr )
	{
		//20210906
		::wcscpy(strTag, L"Standard_H");			
		::fwprintf(pfile, L"        \"%s\": %.2f,\n", strTag, 0.0);

		::wcscpy(strTag, L"N_Wnds");			
		::fwprintf(pfile, L"        \"%s\": %d,\n", strTag, 0);
		::wcscpy(strTag, L"N_Test_Wnds");			
		::fwprintf(pfile, L"        \"%s\": %d,\n", strTag, 0);
		::wcscpy(strTag, L"N_Test_NG_Wnds");			
		::fwprintf(pfile, L"        \"%s\": %d,\n", strTag, 0);					
	}
	else
	{						
		//ModelPtr->GetModelTotalRegion(ModelRgn);					
		//ModelRgnCp.x = ModelRgn.GetCpX();
		//ModelRgnCp.y = ModelRgn.GetCpY();
		//ModelImageW = JetAPI::Floor(ModelRgn.GetWidth()/ImageRes.x);
		//ModelImageH = JetAPI::Floor(ModelRgn.GetHeight()/ImageRes.y);
		
		//20210906
		::wcscpy(strTag, L"Standard_H");			
		::fwprintf(pfile, L"        \"%s\": %.2f,\n", strTag, ModelPtr->GetModelBodyHeight());

		::wcscpy(strTag, L"Lead_List");//Test_NG_Wnd_List					
		::fwprintf(pfile, L"        \"%s\": %s\n", strTag, L"[");//Lead_List [		
		if ( ModelPtr->GetModelSaveLeadReport() == true )
		{
			size_t LeadID=0;
			TREGION4D BoxRgn;
			TSpecResult szW, szH;
			TSpecResult rH, rA, rV;
			TSpecResult sX, sY, sA, tA, oA;
			TPOINT2D LeadCornerPos[4];
			RESULT_ID LeadResultID;
			
			double LeadPosX=0, LeadPosY=0, LeadAngle=0;
			double LeadHeight=0, LeadSizeX=0, LeadSizeY=0;
			double LeadOffsetX=0, LeadOffsetY=0, LeadSkew=0;			
			size_t ModelLandCount = ModelPtr->GetModelLandCount();
			std::vector<CAOILand*> LeadLandList;
			for ( u=0; u<ModelLandCount; u++ )
			{
				CAOILand *LandPtr = ModelPtr->GetModelLandPtr(u, false);
				if ( NULL == LandPtr ) { continue; }
				const CAOIBox &LeadBoxRef = LandPtr->GetLandLeadBox();
				if ( LeadBoxRef.GetBoxEnabled() == false ) { continue; }
				LeadLandList.push_back(LandPtr);
			}

			const size_t LeadLandCount=LeadLandList.size();
			for ( u=0; u<LeadLandCount; u++ )
			{
				CAOILand *LandPtr = LeadLandList[u];
				if ( NULL == LandPtr ) { continue; }
				const CAOIBox &LeadBoxRef = LandPtr->GetLandLeadBox();
				
				LeadAngle= 0;
				LeadID = LandPtr->GetLandIndex()+1;
				LeadBoxRef.GetBoxRegionCad(BoxRgn);				
				LeadPosX = BoxRgn.GetCpX();
				LeadPosY = BoxRgn.GetCpY();
				if ( RESULT_ID_SKIP==PartResultID || RESULT_ID_BYPASS==PartResultID )
				{
					LeadHeight = 0.0;
					LeadOffsetX = LeadOffsetY = LeadSkew = 0.0;					
					LeadSizeX = LeadSizeY = 0.0;
				}
				else
				{
					ModelPtr->CheckModelSpecResult(LandPtr, sX, sY, sA, tA, rH, rA, rV, szW, szH, oA, LeadResultID);
					if ( INVALID_DOUBLE == sX.sValue )	{	LeadOffsetX = 0; }
					else {	LeadOffsetX = sX.sValue; }

					if ( INVALID_DOUBLE == sY.sValue )	{	LeadOffsetY = 0; }
					else {	LeadOffsetY = sY.sValue; }

					if ( INVALID_DOUBLE == sA.sValue )	{	LeadSkew = 0; }
					else {	LeadSkew = sA.sValue; }				 

					if ( INVALID_DOUBLE == rH.sValue )	{	LeadHeight = 0.0; }
					else {	LeadHeight = rH.sValue; }

					if ( INVALID_DOUBLE == szW.sValue )	{	LeadSizeX = 0; }
					else {	LeadSizeX = szW.sValue; }

					if ( INVALID_DOUBLE == szH.sValue )	{	LeadSizeY = 0; }
					else {	LeadSizeY = szH.sValue; }					
				}
				::fwprintf(pfile, L"        {\n");
				LeadBoxRef.GetBoxCornerPosStage(LeadCornerPos);
				ProjectPtr->MapProjectStageToMapPos(LeadCornerPos[0].x, LeadCornerPos[0].y, MapCornerPosX[0], MapCornerPosY[0], LaneID, DistrictID);
				ProjectPtr->MapProjectStageToMapPos(LeadCornerPos[1].x, LeadCornerPos[1].y, MapCornerPosX[1], MapCornerPosY[1], LaneID, DistrictID);
				ProjectPtr->MapProjectStageToMapPos(LeadCornerPos[2].x, LeadCornerPos[2].y, MapCornerPosX[2], MapCornerPosY[2], LaneID, DistrictID);
				ProjectPtr->MapProjectStageToMapPos(LeadCornerPos[3].x, LeadCornerPos[3].y, MapCornerPosX[3], MapCornerPosY[3], LaneID, DistrictID);
				::wcscpy(strTag, L"Loc");//"Lead ID":0, 1	// 腳位-在底圖上的位置				
				::fwprintf(pfile, L"          \"%s\": [%.0f,%.0f,%.0f,%.0f,%.0f,%.0f,%.0f,%.0f],\n", strTag, 
							MapCornerPosX[0], MapCornerPosY[0], MapCornerPosX[1], MapCornerPosY[1], 
							MapCornerPosX[2], MapCornerPosY[2], MapCornerPosX[3], MapCornerPosY[3]);

				::wcscpy(strTag, L"H");//"H":0	// 腳位平均高度
				::fwprintf(pfile, L"          \"%s\": %.2f,\n", strTag, LeadHeight);

				::wcscpy(strTag, L"Offset_X");//"Offset_X":0	// 腳位-X偏移
				::fwprintf(pfile, L"          \"%s\": %.2f,\n", strTag, LeadOffsetX);

				::wcscpy(strTag, L"Offset_Y");//"Offset_Y":0	// 腳位-Y偏移
				::fwprintf(pfile, L"          \"%s\": %.2f,\n", strTag, LeadOffsetY);

				::wcscpy(strTag, L"Skew");//"Offset_Y":0	// 腳位-角度偏移
				::fwprintf(pfile, L"          \"%s\": %.2f,\n", strTag, LeadSkew);

				::wcscpy(strTag, L"Lead_ID");//"Lead ID":0, 1	// 腳位-標號(整數)
				::fwprintf(pfile, L"          \"%s\": %d\n", strTag, LeadID);
				
				if ( u < (LeadLandCount-1) )
				{	::fwprintf(pfile, L"        },\n"); }
				else
				{	::fwprintf(pfile, L"        }\n"); }
			}
		}
		::fwprintf(pfile, L"        ],\n");//Lead_List ]

		int   NGWndIdx=0;
		RESULT_ID WndResultID;		
		size_t TestWndCount = 0;
		WND_LOGIC_TYPE WndLogicType;
		size_t ModelWndCount = ModelPtr->GetModelWndCount();		
		int    TotalWndCount = (int)(ModelWndCount);
		for ( u=0; u<ModelWndCount; u++ )
		{
			WndPtr = ModelPtr->GetModelWndPtr(u, false);
			if ( NULL == WndPtr ) { continue; }
			WndResultID = WndPtr->GetWndResultID();
			WndLogicType = WndPtr->GetWndLogicType();
			if ( WND_LOGIC_NONE != WndLogicType )
			{	WndResultID = WndPtr->GetWndLogicResultID(); }
			if ( RESULT_ID_NONE==WndResultID ) { continue; }						
			if ( RESULT_ID_SKIP==WndResultID ) { continue; }						
			if ( RESULT_ID_BYPASS==WndResultID ) { continue; }
			TestWndCount ++;
			if ( RESULT_ID_OK==WndResultID )
			{
				OkWndIdxList.push_back(u);
				continue; 
			}
			NGWndIdxList.push_back(u);
		}		
		const int NGWndIdxCount = (int)(NGWndIdxList.size());

		::wcscpy(strTag, L"N_Wnds");			
		::fwprintf(pfile, L"        \"%s\": %d,\n", strTag, TotalWndCount);
		::wcscpy(strTag, L"N_Test_Wnds");			
		::fwprintf(pfile, L"        \"%s\": %d,\n", strTag, TestWndCount);
		::wcscpy(strTag, L"N_Test_NG_Wnds");
		::fwprintf(pfile, L"        \"%s\": %d,\n", strTag, NGWndIdxCount);
		::wcscpy(strTag, L"Test_NG_Wnd_List");//Test_NG_Wnd_List					
		::fwprintf(pfile, L"        \"%s\": %s\n", strTag, L"[");//Test_NG_Wnd_List [		
		for ( u=0; u<NGWndIdxCount; u++ )
		{
			NGWndIdx = NGWndIdxList[u];
			WndPtr = ModelPtr->GetModelWndPtr(NGWndIdx, false);
			if ( NULL == WndPtr ) { continue; }
			::fwprintf(pfile, L"        {\n");
			::fwprintf(pfile, L"          \"Index\": %d,\n", u+1);
			::fwprintf(pfile, L"          \"ActualIndex\": %d,\n", NGWndIdx+1);
			WndPtr->SetWndDefectAlarmEnableOnARS(DefectAlarmEnableOnARS);
			if ( WndPtr->WriteWndSpcFile_JSON_RSM(pfile, ProjectPtr) == false )
			{	return false; }
			if ( u < (NGWndIdxCount-1) )
			{	::fwprintf(pfile, L"        },\n"); }
			else
			{	::fwprintf(pfile, L"        }\n"); }
		}
		::fwprintf(pfile, L"        ],\n");//Test_NG_Wnd_List ]

		::wcscpy(strTag, L"Test_OK_Wnd_List");//Test_OK_Wnd_List					
		::fwprintf(pfile, L"        \"%s\": %s\n", strTag, L"[");//Test_OK_Wnd_List [		
		if ( true == SaveOkWndList )
		{
			int   OkWndIdx=0;		
			const int OkWndIdxCount = (int)(OkWndIdxList.size());			
			for ( u=0; u<OkWndIdxCount; u++ )
			{
				OkWndIdx = OkWndIdxList[u];
				WndPtr = ModelPtr->GetModelWndPtr(OkWndIdx, false);
				if ( NULL == WndPtr ) { continue; }
				::fwprintf(pfile, L"        {\n");
				::fwprintf(pfile, L"          \"Index\": %d,\n", u+1);
				::fwprintf(pfile, L"          \"ActualIndex\": %d,\n", OkWndIdx+1);
				WndPtr->SetWndDefectAlarmEnableOnARS(DefectAlarmEnableOnARS);
				if ( WndPtr->WriteWndSpcFile_JSON_RSM(pfile, ProjectPtr) == false )
				{	return false; }
				if ( u < (OkWndIdxCount-1) )
				{	::fwprintf(pfile, L"        },\n"); }
				else
				{	::fwprintf(pfile, L"        }\n"); }
			}			
		}
		::fwprintf(pfile, L"        ],\n");//Test_OK_Wnd_List ]
	}				
	::wcscpy(strTag, L"Check_Result");//Component Check Result
	::fwprintf(pfile, L"        \"%s\": %d\n", strTag, 0);
	return true;
}

bool CAOIComponent::WriteComponentSpcHeader_JSON_detail(FILE *pfile, CAOIProject *ProjectPtr) {

	CAOIComponent* ComponentPtr = this;

	const size_t     szBuffer = 256;
	wchar_t          strTag[szBuffer] = L"";
	wchar_t          strBuffer[szBuffer] = L"";

	TPOINT2D ComponentCad = ComponentPtr->GetComponentCadPos();;

	::fwprintf(pfile, L"            {\n");
	::wcscpy(strTag, L"ID");
	::fwprintf(pfile, L"              \"%s\":%d,\n", strTag, ComponentPtr->GetComponentIndex_Board() + 1);

	::wcscpy(strTag, L"Name");
	JetAPI::TCHAR2wchar(ComponentPtr->GetComponentName(), strBuffer, szBuffer);
	::fwprintf(pfile, L"              \"%s\":\"%s\",\n", strTag, strBuffer);

	::wcscpy(strTag, L"Model");
	JetAPI::TCHAR2wchar(ComponentPtr->GetComponentModelName(), strBuffer, szBuffer);
	::fwprintf(pfile, L"              \"%s\":\"%s\",\n", strTag, strBuffer);

	::wcscpy(strTag, L"Angle");
	::fwprintf(pfile, L"              \"%s\":%.3f,\n", strTag, ComponentPtr->GetComponentAngle());

	::wcscpy(strTag, L"CenterX");
	::fwprintf(pfile, L"              \"%s\":%.3f,\n", strTag, ComponentCad.x);

	::wcscpy(strTag, L"CenterY");
	::fwprintf(pfile, L"              \"%s\":%.3f,\n", strTag, ComponentCad.y);

	//IsMaxNumberDefectsPart 
	::wcscpy(strTag, L"IsMaxNumberDefectsPart");
	//if ( GetComponentDefectAlarmEnableOnARS() )
	if ( GetComponentDefectCountEnableOnARS() )	
	{	::fwprintf(pfile, L"              \"%s\":true,\n", strTag);	}
	else
	{	::fwprintf(pfile, L"              \"%s\":false,\n", strTag);	}

	::wcscpy(strTag, L"TagPart");
	if ( true == GetComponentSaveReport_ARS() )
	{	::fwprintf(pfile, L"              \"%s\":true\n", strTag);	}
	else
	{	::fwprintf(pfile, L"              \"%s\":false\n", strTag);	}

	::fwprintf(pfile, L"            }");
	return TRUE;
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::ConvertToSpcComponent(TSpcComponent &SpcComponent)//轉成Spc零件
{
	CAOIComponent *ComponentPtr = this;
	if ( NULL == ComponentPtr ) { return false; }
	ComponentPtr = ComponentPtr->GetComponentResultPtr();

	double StageCornerPosX[4]={0};
	double StageCornerPosY[4]={0};
	ComponentPtr->GetComponentRoiStageCornerPosX(StageCornerPosX);
	ComponentPtr->GetComponentRoiStageCornerPosY(StageCornerPosY);

	SpcComponent.uuidComponent = ComponentPtr->GetObjUuid();
	SpcComponent.uPanelIndex = ComponentPtr->GetComponentPanelIndex_Project();//整板編號
	SpcComponent.uBoardIndex = ComponentPtr->GetComponentBoardIndex_Project();//單板編號
	SpcComponent.uComponentIndex = ComponentPtr->GetComponentIndex_Project();//零件編號
	::wcscpy(SpcComponent.sComponentName, ComponentPtr->GetComponentName());//零件名稱
	::wcscpy(SpcComponent.sPartNumber, ComponentPtr->GetComponentPartNumber());//料號名稱
	::wcscpy(SpcComponent.sModelName, ComponentPtr->GetComponentModelName());//模組名稱
	::wcscpy(SpcComponent.sNozzleName, ComponentPtr->GetComponentNozzleName());//吸嘴名稱
	//ComponentPtr->GetComponentAngle;	
	SpcComponent.fStageCornerPosX[0] = StageCornerPosX[0];//機台角落座標-X
	SpcComponent.fStageCornerPosY[0] = StageCornerPosY[0];//機台角落座標-Y
	SpcComponent.fStageCornerPosX[1] = StageCornerPosX[1];//機台角落座標-X
	SpcComponent.fStageCornerPosY[1] = StageCornerPosY[1];//機台角落座標-Y
	SpcComponent.fStageCornerPosX[2] = StageCornerPosX[2];//機台角落座標-X
	SpcComponent.fStageCornerPosY[2] = StageCornerPosY[2];//機台角落座標-Y
	SpcComponent.fStageCornerPosX[3] = StageCornerPosX[3];//機台角落座標-X
	SpcComponent.fStageCornerPosY[3] = StageCornerPosY[3];//機台角落座標-Y
	
	SpcComponent.fOffsetX = ComponentPtr->GetComponentResultOffsetX();//偏移X
	SpcComponent.fOffsetY = ComponentPtr->GetComponentResultOffsetY();//偏移Y
	SpcComponent.fSkewAngle = ComponentPtr->GetComponentResultSkewAngle();//偏移角度

	SpcComponent.nTestResultID = ComponentPtr->GetComponentResultID_AOI();//檢測結果
    SpcComponent.uTestDefectID = 0;//檢測瑕疵
	SpcComponent.nCheckResultID = ComponentPtr->GetComponentResultID_AOI();//確認結果
    SpcComponent.uCheckDefectID = 0;//確認瑕疵
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::ConvertToComponentNode(TComponentNode &ComponentNode)//轉成零件節點
{
	CAOIComponent *ComponentPtr = this;
	if ( NULL == ComponentPtr ) { return false; }
	CAOIModel *ModelPtr = ComponentPtr->GetComponentModelPtr();
	if ( NULL == ModelPtr ) { return false; }

	ComponentNode.pCompoennt = this;
	ComponentNode.pPanel = ComponentPtr->GetComponentPanelPtr();
	ComponentNode.pBoard = ComponentPtr->GetComponentBoardPtr();		

	//ComponentNode.sBoardName
	ComponentNode.fAngle = ComponentPtr->GetComponentAngle();
	ComponentNode.fCadPosX = ComponentPtr->GetComponentCadPosX();
	ComponentNode.fCadPosY = ComponentPtr->GetComponentCadPosY();
	ComponentNode.sComponentName = ComponentPtr->GetComponentName();

	ComponentNode.sModelName = ComponentPtr->GetComponentModelName();
	ComponentNode.sPartNumber = ComponentPtr->GetComponentPartNumber();
	ComponentNode.sNozzleName = ComponentPtr->GetComponentNozzleName();
	if ( ComponentPtr->CheckComponentIsAgent() == true )
	{	ComponentNode.bMainPartNumber = false;}
	else
	{	ComponentNode.bMainPartNumber = true;	}
	
	ComponentNode.dwModelType = ModelPtr->GetModelType();
	ComponentNode.dwResultID = ComponentPtr->GetComponentResultID_AOI();
	
	ComponentNode.bSelected = ComponentPtr->GetComponentSelected();	
	//ComponentNode.nTempInt;	
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::InitComponentInspection()//初始化零件檢測
{
	unsigned int InvalidIdx = INVALID_INDEX;
	LANE_ID LaneID = GetComponentLaneID();	
	int    ModelClassID = GetComponentModelClassID();
	SetRgnUsing3D(true);	
	ResetComponentResultPtr();

	SetComponentResultWidth(0);
	SetComponentResultLength(0);
	SetComponentResultHeight(0);
	SetComponentResultArea(0);
	SetComponentResultVolume(0);
	SetComponentResultOffsetX(0.0);
	SetComponentResultOffsetY(0.0);
	SetComponentResultOffsetA(0.0);
	SetComponentResultSkewAngle(0.0);	
	SetComponentResultTiltAngle(0.0);
	SetComponentResultHeightMin(0.0);
	SetComponentResultHeightMax(0.0);
	SetComponentTotalNGCountAlarm(false);
	SetComponentContinueNGCountAlarm(false);

	SetComponentResultWidth_USL(0.0);
	SetComponentResultWidth_LSL(0.0);
	SetComponentResultWidth_WndIdx(InvalidIdx);
	SetComponentResultLength_USL(0.0);
	SetComponentResultLength_LSL(0.0);
	SetComponentResultLength_WndIdx(InvalidIdx);
	SetComponentResultHeight_USL(0.0);
	SetComponentResultHeight_LSL(0.0);
	SetComponentResultHeight_WndIdx(InvalidIdx);
	SetComponentResultArea_USL(0.0);
	SetComponentResultArea_LSL(0.0);
	SetComponentResultArea_WndIdx(InvalidIdx);
	SetComponentResultVolume_USL(0.0);
	SetComponentResultVolume_LSL(0.0);
	SetComponentResultVolume_WndIdx(InvalidIdx);
	SetComponentResultOffsetX_USL(0.0);
	SetComponentResultOffsetX_LSL(0.0);
	SetComponentResultOffsetX_WndIdx(InvalidIdx);
	SetComponentResultOffsetY_USL(0.0);
	SetComponentResultOffsetY_LSL(0.0);
	SetComponentResultOffsetY_WndIdx(InvalidIdx);
	SetComponentResultOffsetA_USL(0.0);
	SetComponentResultOffsetA_LSL(0.0);
	SetComponentResultOffsetA_WndIdx(InvalidIdx);
	SetComponentResultSkewAngle_USL(0.0);
	SetComponentResultSkewAngle_LSL(0.0);
	SetComponentResultSkewAngle_WndIdx(InvalidIdx);
	SetComponentResultTiltAngle_USL(0.0);
	SetComponentResultTiltAngle_LSL(0.0);
	SetComponentResultTiltAngle_WndIdx(InvalidIdx);
	SetComponentGrrTempValue(INVALID_DOUBLE, INVALID_DOUBLE, INVALID_DOUBLE, INVALID_DOUBLE);

	SetComponentStageOffsetX(0.0);
	SetComponentStageOffsetY(0.0);
	SetComponentGroupOffsetResX(0.0);
	SetComponentGroupOffsetResY(0.0);
	SetComponentGroupDistanceResX(0.0);
	SetComponentGroupDistanceResY(0.0);
	SetComponentDefectAlarmResultOnAOI(false);
	SetComponentDefectAlarmResultOnARS(false);
	SetComponentCadResultX(GetComponentCadPosX());
	SetComponentCadResultY(GetComponentCadPosY());
	SetComponentStageResultX(GetComponentStagePosX());
	SetComponentStageResultY(GetComponentStagePosY());		

	ClearRgnImageBuffer();	
	SetComponentKeepImage(false);		
	SetComponentFillImageTime(0.0);
	SetComponentModelImageIsSaved(false);
	SetComponentModelImageIsSaved_AI(false);
	SetComponentResultID_AOI(RESULT_ID_NONE);	
	SetComponentResultID_Alarm(RESULT_ID_NONE);
	UpdateComponentResultID_AOI_Lane(LaneID);		
	m_ComponentCurrentDefectCountAOI.SetLaneID(LANE_ID_NULL);
	m_ComponentCurrentDefectCountAOI.SetResultID(RESULT_ID_NONE);
	m_ComponentCurrentDefectCountAOI.SetAll(0);

	const bool bComponentEnableAlarm = GetComponentEnableAlarm();
	SetComponentTotalNGCountEnable(bComponentEnableAlarm);
	SetComponentContinueNGCountEnable(bComponentEnableAlarm);

	double PanelBasePlane=GetComponentPanelBasePlane();
	TNoiseFilterParam &NoiseFilterParam = GetComponentSpaceNoiseFilterParam();
	NoiseFilterParam.BasePlaneParam.InitialBasePlaneParam();	
	NoiseFilterParam.BasePlaneParam.PanelBasePlane = PanelBasePlane;

	m_ComponentModel.SetModelClassID(ModelClassID);
	m_ComponentModel.SetModelPanelBasePlane(PanelBasePlane);
	m_ComponentModel.SetModelSpaceBasePlaneParam(NoiseFilterParam.BasePlaneParam);
	m_ComponentModel.SetModelSpaceNoiseFilterParam(NoiseFilterParam);

	//特殊遮罩-基板顏色
	bool RgnMaskEnable_Base = CAOIRgn::GetRgnMaskEnable_Base();		
	unsigned int RgnMaskFrameIndex_Base = CAOIRgn::GetRgnMaskFrameIndex_Base();	
	unsigned int RgnMaskFrameUniqueID_Base = CAOIRgn::GetRgnMaskFrameUniqueID_Base();	
	const int RgnMaskColorGroupLinkIndex = CAOIRgn::GetRgnMaskColorGroupLinkIndex();	

	m_ComponentModel.SetModelMaskEnable_Base(RgnMaskEnable_Base);
	m_ComponentModel.SetModelMaskFrameIndex_Base(RgnMaskFrameIndex_Base);
	m_ComponentModel.SetModelMaskFrameUniqueID_Base(RgnMaskFrameUniqueID_Base);
	m_ComponentModel.SetModelMaskColorGroupLinkIndex(RgnMaskColorGroupLinkIndex);	

	const bool DataModelEnabled=GetComponentDataModelEnabled();
	m_ComponentModel.SetModelDataModelEnabled(DataModelEnabled);
	const int DataModelLevelID=GetComponentDataModelLevelID();
	m_ComponentModel.SetModelDataModelLevelID(DataModelLevelID);

	m_ComponentModel.InitModelInspection(false);		
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::ExecComponentInspection()//執行零件檢測
{
	size_t     i=0;
	CString    str;			
	TUNI_FRAME UniFrame;	
	std::vector<TUNI_FRAME> UniFrameList;	
	if ( CAOIRgn::GetRgnUniFrameList(UniFrameList) == false ) { return false; }		
	CAOIModel *ModelPtr = GetComponentModelPtr();	
	CAOIProject *ProjectPtr = GetComponentProjectPtr();
	const size_t UniFrameCount = UniFrameList.size();
	if ( 0==UniFrameCount || NULL==ProjectPtr ) 
	{ 
		LANE_ID LaneID = GetComponentLaneID();
		RESULT_ID ResultID = RESULT_ID_EXCEPTION;
		SetComponentResultID_AOI(ResultID);
		SetComponentResultID_Alarm(ResultID);
		UpdateComponentResultID_AOI_Lane(LaneID);
		ModelPtr->SetModelResultID(ResultID);
		ModelPtr->SetModelResultID_Alarm(ResultID);		
		return true; 
	}

	const bool DataModelEnabled=ModelPtr->CheckModelUseDataModel();
	if ( true == DataModelEnabled )
	{
		IMAGE_SIZE SpaceW=0;
		IMAGE_SIZE SpaceH=0;
		IMAGE_SIZE SpaceStep=0;
		IMAGE_SIZE SpaceBitCnt=8;
		MASK_PTR   MaskPtr =NULL;
		SPACE_PTR  SpacePtr=NULL;
		for ( i=0; i<UniFrameCount; i++ )
		{
			TUNI_FRAME UniFrameRef=UniFrameList[i];
			if ( NULL == UniFrameRef.SpacePtr ) { continue; }
			SpaceW = UniFrameRef.ImageW;
			SpaceH = UniFrameRef.ImageH;
			SpaceStep = UniFrameRef.ImageStep;
			MaskPtr = UniFrameRef.MaskPtr;
			SpacePtr = UniFrameRef.SpacePtr;
			break;
		}
		if ( NULL==SpacePtr || NULL==MaskPtr )
		{	SpaceBitCnt = 8;	}
		else	
		{
			TDataModelParam Param;
			if ( ModelPtr->BuildModelDataModelParam(SpaceW, SpaceH, Param) == true )
			{
				if ( ImageAPI.BuildDataModel(SpaceW, SpaceH, SpaceStep, SpacePtr, MaskPtr, Param) == false )
				{
					LANE_ID LaneID = GetComponentLaneID();
					RESULT_ID ResultID = RESULT_ID_EXCEPTION;
					SetComponentResultID_AOI(ResultID);
					SetComponentResultID_Alarm(ResultID);
					UpdateComponentResultID_AOI_Lane(LaneID);
					ModelPtr->SetModelResultID(ResultID);
					ModelPtr->SetModelResultID_Alarm(ResultID);		
					return true; 
				}
			}
		}
	}
	
	CString    ModelName;
	CString    ComponentName;	
	CString    PartImageFolder;	
	double     SpaceRatio=50.0;
	ModelName = ModelPtr->GetModelName();
	ComponentName = GetComponentFullName();
	SAVE_SPC_PART_IMAGE_MODE  SaveSpcPartImageMode = SAVE_SPC_PART_IMAGE_DISABLE;
	if ( NULL != ProjectPtr )
	{
		SpaceRatio = ProjectPtr->GetProjectSpaceToGrayRatio();
		PartImageFolder = ProjectPtr->GetProjectSpcPartImageFolder();		
		SaveSpcPartImageMode = ProjectPtr->GetProjectSaveSpcPartImageMode();		
		if ( SAVE_SPC_PART_IMAGE_DISABLE != SaveSpcPartImageMode )
		{
			bool       bAppend=false;
			const bool bEnhance=true;
			const bool bSave3D = ProjectPtr->GetProjectSaveModelImage3DFile_AI();
			switch ( SaveSpcPartImageMode )
			{
			default:
			case SAVE_SPC_PART_IMAGE_EVERYONE:
			case SAVE_SPC_PART_IMAGE_SELECTED:
			case SAVE_SPC_PART_IMAGE_SELECTED_COPY:
				bAppend = false;
				break;
			case SAVE_SPC_PART_IMAGE_SELECTED_ADD:
				bAppend = true;
				break;
			}
			str.Format(_T("%s\\%s.PNG"), PartImageFolder, ComponentName);
			if ( ImageAPI.SaveUniFrameImage(str, UniFrameList, true, bEnhance, bSave3D, bAppend, SpaceRatio) == false )
			{	return false;	}
			return true;//不做額外檢測
		}		
	}	
#ifdef _DEBUG
	BOOL bSave = TRUE;
	if ( bSave == TRUE )
	{	
		const bool bAppend=false;
		const bool bEnhance=false;
		const bool bSave3D=false;				
		str.Format(_T("%s\\%s.PNG"), AOIDataCollect.GetAOITempDirectory(), ComponentName);
		ImageAPI.SaveUniFrameImage(str, UniFrameList, true, bEnhance, bSave3D, bAppend, SpaceRatio);		
	}
#endif//_DEBUG	

	TREGION4D  rgnModel;
	TSIZE2D    ImageSizeUm;
	CAMERA_ID  CameraID = PRIMARY_CAMERA_ID;
	IMAGE_SIZE ImageW = UniFrameList[0].ImageW;
	IMAGE_SIZE ImageH = UniFrameList[0].ImageH;
	double BodyW = GetComponentBodySizeW();
	double BodyH = GetComponentBodySizeH();
	double ComponentW = GetComponentRoiSizeW();
	double ComponentH = GetComponentRoiSizeH();
	AOIDataCollect.MapImageSizeToReal(CameraID, ImageW, ImageH, ImageSizeUm);
	SetComponentFrameImageSize_um(ImageSizeUm);//修正正跨FOV的尺寸

	bool bIsOK = true;	
	//Ash:test
	//CAOIComponent* _j_cmpn = ModelPtr->GetModelComponentPtr();
	//str.Format(_T("%s\\%s_EC.PNG"), AOIDataCollect.GetAOITempDirectory(), _j_cmpn->GetComponentName());
	//ImageAPI.SaveUniFrameImage(str, UniFrameList, true, false, true, false, -1);
	ModelPtr->GetModelTotalRegion(rgnModel);
	const double ModelW = rgnModel.GetWidth();
	const double ModelH = rgnModel.GetHeight();

	SaveComponentLog(_T("Test Component"), _T("Start"));
	ModelPtr->ExecModelInspection(UniFrameList);
	SaveComponentLog(_T("Test Component"), _T("End"));

	UpdateComponentResultID();
	UpdateComponentResultValue();
	ModelPtr->CalcModelImageRect_CustomerAI(ImageW, ImageH);

	SaveComponentLog(_T("Save Component Image"), _T("Start"));
	bIsOK = ExecComponentSaveDefectImage(UniFrameList);	
	SaveComponentLog(_T("Save Component Image"), _T("End"));

	if ( FN_ENABLE == AOIDataCollect.GetSystemParameter().m_SaveTestObjectFinishLog )
	{
		const int nOpenMPCnt = GetComponentOpenMPCount();
		if ( true == bIsOK )
		{	str.Format(_T("Test Component Finish-[%s, MP:%d]"), ComponentName, nOpenMPCnt); }
		else
		{	str.Format(_T("Test Component Exception-[%s, MP:%d]"), ComponentName, nOpenMPCnt); }
		AOIDataCollect.SaveCurrentProcess(str);
	}
	
	bool CheckXBoard=false;
	RESULT_ID ResultID = GetComponentResultID_AOI();
	CAOIBoard *BoardPtr = GetComponentBoardPtr();	
	if ( FN_ENABLE == AOIDataCollect.GetSystemParameter().m_ConfirmComponentBarcodeEnabled )
	{
		if ( RESULT_ID_NG==ResultID || RESULT_ID_EXCEPTION==ResultID )
		{
			bool bBarcodeFault=false;
			const size_t WndCount=ModelPtr->GetModelWndCount();
			for ( size_t i=0; i<WndCount; i++ )
			{
				CAOIWnd *WndPtr=ModelPtr->GetModelWndPtr(i, false);
				if ( NULL == WndPtr ) { continue; }
				if ( WndPtr->GetWndEnabled() == false ) { continue; }				
				ALG_TYPE AlgType=WndPtr->GetWndAlgType();
				if ( ALG_BARCODE_RECOGNIZE != AlgType ) { continue; }
				RESULT_ID WndResultID = WndPtr->GetWndResultID();
				if ( RESULT_ID_NONE==WndResultID || RESULT_ID_OK==WndResultID || RESULT_ID_BYPASS==WndResultID || RESULT_ID_SKIP==WndResultID )
				{	continue; }
				RESULT_ID LogicResultID = WndPtr->GetWndLogicResultID();
				if ( RESULT_ID_NONE==LogicResultID || RESULT_ID_OK==LogicResultID || RESULT_ID_BYPASS==LogicResultID || RESULT_ID_SKIP==LogicResultID )
				{	continue; }
				SetComponentKeepImage(true);
				break;
			}
		}
	}
	//if ( NULL==BoardPtr || RESULT_ID_NG!=ResultID )
	if ( NULL==BoardPtr || RESULT_ID_NONE==ResultID || RESULT_ID_OK==ResultID || RESULT_ID_BYPASS==ResultID || RESULT_ID_SKIP==ResultID )
	{	CheckXBoard = false; }
	else
	{
		const bool XBoardUnit = GetComponentXBoardUnit();	
		const LANE_ID LaneID = ProjectPtr->GetProjectActLaneID();
		const bool XBoardRatioMode = ProjectPtr->GetProjectXBoardUseRatioMode();
		if ( true==XBoardRatioMode || true==XBoardUnit )
		{	CheckXBoard = true; }
		if ( true == CheckXBoard )
		{		
			ProjectPtr->LockProject();
			BoardPtr->IncrementBoardXBoardUnitCount();
			if ( BoardPtr->AnalyzeBoardIsXBoard() == true )
			{	BoardPtr->ExecBoardBeXBoard(LaneID); }
			ProjectPtr->UnlockProject();
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::ExecComponentSaveDefectImage(const std::vector<TUNI_FRAME> &UniFrameList)//執行零件儲存瑕疵圖片
{
	bool IsOK = true;
	const bool IsAgent=CheckComponentIsAgent();//分身不存零件圖
	if ( false == IsAgent )
	{
		IsOK = CAOIRgn::ExecRgnSaveDefectImage(UniFrameList);
		if ( false == IsOK ) 
		{	return false; }

		IsOK = CAOIRgn::ExecRgnSaveDefectImage_AI(UniFrameList);
		if ( false == IsOK ) 
		{	return false; }
	}

	size_t       i=0;
	IMAGE_SIZE   DstW=0;
	IMAGE_SIZE   DstH=0;
	IMAGE_SIZE   DstStep=0;	
	IMAGE_PTR    DstPtr=NULL;

	IMAGE_SIZE   ImageW=0;
	IMAGE_SIZE   ImageH=0;
	IMAGE_SIZE   ImageStep=0;
	IMAGE_SIZE   BitCount=0;
	IMAGE_PTR    ImagePtr=NULL;
	
	CAOIModel   *ModelPtr = GetComponentModelPtr();
	const double ComponentAngle=GetComponentAngle();
	const size_t UniFrameCount=UniFrameList.size();
	CAOIProject *ProjectPtr = GetComponentProjectPtr();
	const bool   ModelIsolated = GetComponentModelIsolated();
	const bool   SaveModelBkImage = AOIDataCollect.GetSaveModelBkImage();
	if ( NULL != ProjectPtr )
	{
		if ( true==SaveModelBkImage && NULL!=ModelPtr )
		{	
			CString ModelFolder;
			CString ModelBkImageName;
			CString ModelName = GetComponentModelName();			
			ModelFolder = ModelPtr->GetModelFolder();			
			for ( i=0; i<UniFrameCount; i++ )
			{	
				ImageW = UniFrameList[i].ImageW;
				ImageH = UniFrameList[i].ImageH;
				ImageStep = UniFrameList[i].ImageStep;
				BitCount = UniFrameList[i].BitCount;
				ImagePtr = UniFrameList[i].ImagePtr;				
				ModelBkImageName = ModelPtr->GetModelBKImageFilename(i);
				if ( ImageAPI.RotateImage(-ComponentAngle, ImageW, ImageH, ImageStep, BitCount, ImagePtr, DstW, DstH, DstStep, DstPtr) == false )
				{	continue; }
				ProjectPtr->LockProject();
				ImageAPI.SaveImage(ModelBkImageName, DstW, DstH, DstStep, BitCount, DstPtr, true);	
				ProjectPtr->UnlockProject();
				JetMemory.free_func(DstPtr); DstPtr=NULL;
			}
		}		
	}	
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::ExecComponentSaveDefectImage_backup(const std::vector<TUNI_FRAME> &UniFrameList)//執行零件儲存瑕疵圖片
{
	size_t     i=0;
	size_t     j=0;
	CString    str;		
	CString    ComName;
	TUNI_FRAME UniFrame;
	TUNI_FRAME UniFrameTmp;
	CAOIField   *FieldPtr = GetComponentFieldPtr();	
	CAOIProject *ProjectPtr = GetComponentProjectPtr();
	if ( NULL==FieldPtr || NULL==ProjectPtr ) { return false; }
	const size_t UniFrameCount = UniFrameList.size();
	const char fnName[] = "CAOIComponent::ExecComponentSaveDefectImage_backup";

	bool      CheckModelSaveImage = false;
	bool      CheckFieldSaveImage_Tuning = false;
	bool      CheckFieldSaveImage_Offline = false;
	RESULT_ID ResultID = GetComponentResultID_AOI();
	TASK_MODE TaskMode = ProjectPtr->GetProjectActTaskMode();//TASK_TUNING_PROJECT, TASK_INSPECT_PROJECT
	const bool SaveDefectImage = AOIDataCollect.GetSaveDefectImage();	
	const bool OnlineTuningEnable = ProjectPtr->CheckProjectOnlineTuningRunning();
	const double SpaceRatio = ProjectPtr->GetProjectSpaceToGrayRatio();	
	CString SpcImageFolder = ProjectPtr->GetProjectSpcImageFolder();
	SAVE_TEST_IMAGE_MODE  SaveModelImageMode = GetComponentSaveTestImageMode();	
	SAVE_TEST_IMAGE_MODE  OnlineTuningMode = ProjectPtr->GetProjectOnlineTuningMode();
	SAVE_TEST_IMAGE_MODE  SaveFieldImageMode = ProjectPtr->GetProjectSaveFieldImageMode();		
	
	if ( ProjectPtr->GetProjectUseLocalFolder() )
	{	SpcImageFolder = ProjectPtr->GetProjectSpcImageFolderLocal();	}	

	if ( true == SaveDefectImage )
	{
		switch ( TaskMode )
		{
		case TASK_TUNING_PROJECT://任務-檢測調適專案
		case TASK_TUNING_OFFLINE://任務-檢測離線專案-20181026
		case TASK_INSPECT_PROJECT://任務-檢測檢測專案
			CheckModelSaveImage = true;
			break;
		default:
			CheckModelSaveImage = false;
			break;
		}
		if ( true == CheckModelSaveImage )
		{
			switch ( SaveModelImageMode )
			{
			case SAVE_TEST_IMAGE_DISABLE:
				CheckModelSaveImage = false;
				break;
			case SAVE_TEST_IMAGE_DEFECT:
				switch ( ResultID )
				{
				case RESULT_ID_NG:
				case RESULT_ID_EXCEPTION:
					CheckModelSaveImage = true;
					break;
				default:
					CheckModelSaveImage = false;
					break;
				}
				break;
			}
		}		
	}
	else 
	{	CheckModelSaveImage = false;	}
	
					
	RECT           RoiRect={0,0,0,0};			
	IMAGE_SIZE     RoiStep = 0;
	IMAGE_SIZE     BufferBitCount = 24;	
	ComName = GetComponentFullName();
	if ( true == CheckModelSaveImage )
	{
		const bool bAppend=false;
		const bool bEnhance=true;
		const bool bSave3D=true;
		const double SpaceRatio = ProjectPtr->GetProjectSpaceToGrayRatio();
		str.Format(_T("%s\\%s.%s"), SpcImageFolder, ComName, _T("JPG"));
		if ( ImageAPI.SaveUniFrameImage(str, UniFrameList, true, bEnhance, bSave3D, bAppend, SpaceRatio) == false )
		{
			SetComponentResultID_AOI(RESULT_ID_EXCEPTION);
			SetComponentResultID_Alarm(RESULT_ID_EXCEPTION);
			return false;	
		}		
		SetComponentModelImageIsSaved(true);
	}

	//儲存區域圖片
	if ( true == OnlineTuningEnable )//開起在線調機
	{	
		switch ( OnlineTuningMode )
		{
		case SAVE_TEST_IMAGE_DEFECT:
			switch ( ResultID )
			{
			case RESULT_ID_NG:
			case RESULT_ID_EXCEPTION:
				CheckFieldSaveImage_Tuning = true;
				break;
			default:
				CheckFieldSaveImage_Tuning = false;
				break;
			}
			break;
		case SAVE_TEST_IMAGE_EVERYONE:
			CheckFieldSaveImage_Tuning = true;
			break;
		default:
			CheckFieldSaveImage_Tuning = false;				
			break;
		}		
	}
	if ( false == SaveDefectImage )//關閉儲存圖像檔案
	{	CheckFieldSaveImage_Offline = false;	}
	else
	{
		switch ( TaskMode )
		{
		case TASK_TUNING_PROJECT://任務-檢測調適專案
		case TASK_TUNING_OFFLINE://任務-檢測離線專案-20181026
		case TASK_INSPECT_PROJECT://任務-檢測檢測專案
			CheckFieldSaveImage_Offline = true;			
			break;
		default:
			CheckFieldSaveImage_Offline = false;			
			break;
		}	
		if ( true == CheckFieldSaveImage_Offline )
		{
			switch ( SaveFieldImageMode )
			{
			case SAVE_TEST_IMAGE_DEFECT:
				switch ( ResultID )
				{
				case RESULT_ID_NG:
				case RESULT_ID_EXCEPTION:
					CheckFieldSaveImage_Offline = true;
					break;
				default:
					CheckFieldSaveImage_Offline = false;
					break;
				}
				break;
			case SAVE_TEST_IMAGE_EVERYONE:
				CheckFieldSaveImage_Offline = true;
				break;
			default:
				CheckFieldSaveImage_Offline = false;				
				break;
			}
		}
	}
	if ( true==CheckFieldSaveImage_Tuning || true==CheckFieldSaveImage_Offline )
	{
		CAOIFrame   *FramePtr = NULL;
		const size_t FrameCount = FieldPtr->GetFieldFramePtrCount();
		for ( i=0; i<FrameCount; i++ )
		{
			FramePtr = FieldPtr->GetFieldFramePtr(i, false);
			if ( NULL == FramePtr ) { continue; }
			FramePtr->ExecFrameSave_Thread();			
		}
		
		size_t       SubFrameCount=0;
		CAOIRgn     *SubRgnPtr=NULL;
		CAOIField   *SubFieldPtr = NULL;
		CAOIFrame   *SubFramePtr = NULL;
		const size_t SubRgnCount = GetComponentSubRgnCount();
		for ( j=0; j<SubRgnCount; j++ )
		{
			SubRgnPtr = GetComponentSubRgnPtr(j, false);
			if ( NULL == SubRgnPtr ) { continue; }
			SubFieldPtr = SubRgnPtr->GetRgnFieldPtr();
			if ( NULL == SubFieldPtr ) { continue; }
			SubFrameCount = SubFieldPtr->GetFieldFramePtrCount();			
			for ( i=0; i<SubFrameCount; i++ )
			{
				SubFramePtr = SubFieldPtr->GetFieldFramePtr(i, false);
				if ( NULL == SubFramePtr ) { continue; }
				SubFramePtr->ExecFrameSave_Thread();				
			}
		}
	}		

	if ( ExecRgnSaveDefectImage_AI(UniFrameList) == false )
	{	return false;	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentTotalEachDefectCountAOI(const CWndDefectItem &DefectItem)
{
	m_ComponentTotaEachlDefectCountAOI=DefectItem;	
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::GetComponentTotalEachDefectCountAOI(CWndDefectItem &DefectItem) const
{
	DefectItem = m_ComponentTotaEachlDefectCountAOI;	
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentTotalEachDefectCountAOI_Lane(LANE_ID LaneID, const CWndDefectItem &DefectItem)
{
	switch ( LaneID )
	{
	case LANE_ID_A:	SetComponentTotalEachDefectCountAOI_LA(DefectItem);	break;
	case LANE_ID_B:	SetComponentTotalEachDefectCountAOI_LB(DefectItem);	break;
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::GetComponentTotalEachDefectCountAOI_Lane(LANE_ID LaneID, CWndDefectItem &DefectItem) const
{
	switch ( LaneID )
	{
	case LANE_ID_A:	GetComponentTotalEachDefectCountAOI_LA(DefectItem);	break;
	case LANE_ID_B:	GetComponentTotalEachDefectCountAOI_LB(DefectItem);	break;
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentTotalEachDefectCountAOI_LA(const CWndDefectItem &DefectItem)
{
	m_ComponentTotaEachlDefectCountAOI_LA = DefectItem;
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::GetComponentTotalEachDefectCountAOI_LA(CWndDefectItem &DefectItem) const
{
	DefectItem = m_ComponentTotaEachlDefectCountAOI_LA;
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentTotalEachDefectCountAOI_LB(const CWndDefectItem &DefectItem)
{
	m_ComponentTotaEachlDefectCountAOI_LB = DefectItem;
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::GetComponentTotalEachDefectCountAOI_LB(CWndDefectItem &DefectItem) const
{
	DefectItem = m_ComponentTotaEachlDefectCountAOI_LB;
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentTotalEachDefectCountARS(const CWndDefectItem &DefectItem)
{
	m_ComponentTotalEachDefectCountARS = DefectItem;	
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::GetComponentTotalEachDefectCountARS(CWndDefectItem &DefectItem) const
{
	DefectItem = m_ComponentTotalEachDefectCountARS;
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentTotalEachDefectCountARS_Lane(LANE_ID LaneID, const CWndDefectItem &DefectItem)
{
	switch ( LaneID )
	{
	case LANE_ID_A:	SetComponentTotalEachDefectCountARS_LA(DefectItem);	break;
	case LANE_ID_B:	SetComponentTotalEachDefectCountARS_LB(DefectItem);	break;
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::GetComponentTotalEachDefectCountARS_Lane(LANE_ID LaneID, CWndDefectItem &DefectItem) const
{
	switch ( LaneID )
	{
	case LANE_ID_A:	GetComponentTotalEachDefectCountARS_LA(DefectItem);	break;
	case LANE_ID_B:	GetComponentTotalEachDefectCountARS_LB(DefectItem);	break;
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentTotalEachDefectCountARS_LA(const CWndDefectItem &DefectItem)
{
	m_ComponentTotalEachDefectCountARS_LA = DefectItem;
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::GetComponentTotalEachDefectCountARS_LA(CWndDefectItem &DefectItem) const
{
	DefectItem = m_ComponentTotalEachDefectCountARS_LA;
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentTotalEachDefectCountARS_LB(const CWndDefectItem &DefectItem)
{
	m_ComponentTotalEachDefectCountARS_LB = DefectItem;
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::GetComponentTotalEachDefectCountARS_LB(CWndDefectItem &DefectItem) const
{
	DefectItem = m_ComponentTotalEachDefectCountARS_LB;
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::ResetComponentResultStatistic()//覆歸零件結果統計
{		
	RESULT_ID ResultID=RESULT_ID_NONE;
	SetComponentTotalTestCountAOI(0);
	SetComponentTotalTestCountARS(0);
	SetComponentTotalNGCountAOI(0);
	SetComponentTotalNGCountARS(0);	
	SetComponentContinueNGCountAOI(0);
	SetComponentContinueNGCountARS(0);
	m_ComponentCurrentDefectCountAOI.SetAll(0);
	m_ComponentCurrentDefectCountARS.SetAll(0);
	m_ComponentTotaEachlDefectCountAOI.SetAll(0);
	m_ComponentTotaEachlDefectCountAOI_LA.SetAll(0);
	m_ComponentTotaEachlDefectCountAOI_LB.SetAll(0);
	m_ComponentTotalEachDefectCountARS.SetAll(0);	
	m_ComponentTotalEachDefectCountARS_LA.SetAll(0);	
	m_ComponentTotalEachDefectCountARS_LB.SetAll(0);	
	SetComponentResultID_AOI(ResultID);
	SetComponentResultID_AOI_LA(ResultID);
	SetComponentResultID_AOI_LB(ResultID);
	SetComponentResultID_ARS(ResultID);
	SetComponentResultID_ARS_LA(ResultID);
	SetComponentResultID_ARS_LB(ResultID);
	SetComponentResultID_Alarm(ResultID);
	
	m_ComponentResultListAOI.clear();//零件最近結果列表
	m_ComponentResultListARS.clear();//零件最近結果列表	
	
	m_ComponentGrrOffsetX = 0;
	m_ComponentGrrOffsetY = 0;
	m_ComponentGrrSkewA = 0;
	m_ComponentGrrBodyHeight = 0;
	m_ComponentGrrSigmaItemIdx = -1;
	m_ComponentGrrSigmaItemList.clear();
	m_ComponentGrrSigmaItemNull = TGrrSigmaItem();
	m_ComponentGrrSigmaItemList.push_back(TGrrSigmaItem());	

	m_ComponentResultSigmaItem_LA = TSigmaItem();
	m_ComponentResultSigmaItem_LB = TSigmaItem();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::AddComponentResultToStatisticAOI(LANE_ID LaneID)//加入零件結果至統計內
{
	size_t          i=0;	
	CWndDefectItem  DefectResult;
	const RESULT_ID  ComponentResultID = CheckComponentResultID_AOI();
	
	m_ComponentCurrentDefectCountAOI.SetLaneID(LaneID);	
	m_ComponentCurrentDefectCountAOI.SetResultID(ComponentResultID);
	m_ComponentTotaEachlDefectCountAOI.AddWndDefectItemCount(m_ComponentCurrentDefectCountAOI);	
	switch ( LaneID )
	{
	case LANE_ID_A:	m_ComponentTotaEachlDefectCountAOI_LA.AddWndDefectItemCount(m_ComponentCurrentDefectCountAOI); break;
	case LANE_ID_B:	m_ComponentTotaEachlDefectCountAOI_LB.AddWndDefectItemCount(m_ComponentCurrentDefectCountAOI); break;
	}
	if ( RESULT_ID_NG==ComponentResultID || RESULT_ID_EXCEPTION==ComponentResultID )
	{	
		switch ( LaneID )
		{
		case LANE_ID_A:	m_ComponentTotalNGCountAOI_LA ++; break;
		case LANE_ID_B:	m_ComponentTotalNGCountAOI_LB ++; break;
		}
		m_ComponentTotalNGCountAOI ++;		
		AddComponentContinueNGCountAOI();
	}	
	else
	{	SetComponentContinueNGCountAOI(0); }

	m_ComponentTotalTestCountAOI ++;
	switch ( LaneID )
	{
	case LANE_ID_A:	m_ComponentTotalTestCountAOI_LA ++; break;
	case LANE_ID_B:	m_ComponentTotalTestCountAOI_LB ++; break;
	}	
	
	DefectResult = m_ComponentCurrentDefectCountAOI;
	//DefectResult.SetLaneID(LaneID);
	//DefectResult.SetResultID(ComponentResultID);
	m_ComponentResultListAOI.push_back(DefectResult);
	return true;
}
//-------------------------------------------------------------------------------------//
bool  CAOIComponent::ShiftComponentResultToStatisticAOI(size_t val)//偏移零件結果統計
{
	size_t    i=0;
	RESULT_ID ResultID;
	CWndDefectItem  TotalWndDefectItem;
	std::vector<CWndDefectItem>  &ResultList=m_ComponentResultListAOI;
	const size_t ResultCount = ResultList.size();
	if ( ResultCount <= val ) { return true; }
	const size_t EraseCount = ResultCount-val;
	ResultList.erase(ResultList.begin(), ResultList.begin()+EraseCount);
	const size_t ResultCount2 = ResultList.size();

	m_ComponentTotalNGCountAOI = 0;	
	for ( i=0; i<ResultCount2; i++ )
	{
		ResultID = ResultList[i].GetResultID();
		TotalWndDefectItem.AddWndDefectItemCount(ResultList[i]);

		if ( RESULT_ID_NG==ResultID || RESULT_ID_EXCEPTION==ResultID )
		{	m_ComponentTotalNGCountAOI ++;	}
	}	
	m_ComponentTotalTestCountAOI = ResultCount2;
	m_ComponentTotaEachlDefectCountAOI=TotalWndDefectItem;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::AddComponentResultToStatisticARS(LANE_ID LaneID)//加入零件結果至統計內	
{
	size_t          i=0;	
	CWndDefectItem  DefectResult;
	const RESULT_ID  ComponentResultID = GetComponentResultID_ARS();

	m_ComponentCurrentDefectCountARS.SetLaneID(LaneID);
	m_ComponentCurrentDefectCountARS.SetResultID(ComponentResultID);
	m_ComponentTotalEachDefectCountARS.AddWndDefectItemCount(m_ComponentCurrentDefectCountARS);
	switch ( LaneID )
	{
	case LANE_ID_A:	m_ComponentTotalEachDefectCountARS_LA.AddWndDefectItemCount(m_ComponentCurrentDefectCountARS); break;
	case LANE_ID_B:	m_ComponentTotalEachDefectCountARS_LB.AddWndDefectItemCount(m_ComponentCurrentDefectCountARS); break;
	}

	if ( RESULT_ID_NG==ComponentResultID || RESULT_ID_EXCEPTION==ComponentResultID )
	{
		switch ( LaneID )
		{
		case LANE_ID_A:	m_ComponentTotalNGCountARS_LA ++; break;
		case LANE_ID_B:	m_ComponentTotalNGCountARS_LB ++; break;
		}
		m_ComponentTotalNGCountARS ++;	
		AddComponentContinueNGCountARS();
	}	
	else
	{	SetComponentContinueNGCountARS(0);	}
	switch ( LaneID )
	{
	case LANE_ID_A:	m_ComponentTotalTestCountARS_LA ++; break;
	case LANE_ID_B:	m_ComponentTotalTestCountARS_LB ++; break;
	}
	m_ComponentTotalTestCountARS ++;
	
	DefectResult = m_ComponentCurrentDefectCountARS;
	//DefectResult.SetLaneID(LaneID);
	//DefectResult.SetResultID(ComponentResultID);
	m_ComponentResultListARS.push_back(DefectResult);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::ShiftComponentResultToStatisticARS(size_t val)//偏移零件結果統計
{
	size_t    i=0;
	RESULT_ID ResultID;
	CWndDefectItem  TotalWndDefectItem;
	std::vector<CWndDefectItem>  &ResultList=m_ComponentResultListARS;
	const size_t ResultCount = ResultList.size();
	if ( ResultCount <= val ) { return true; }
	const size_t EraseCount = ResultCount-val;
	ResultList.erase(ResultList.begin(), ResultList.begin()+EraseCount);
	const size_t ResultCount2 = ResultList.size();

	m_ComponentTotalNGCountARS = 0;
	for ( i=0; i<ResultCount2; i++ )
	{
		ResultID = ResultList[i].GetResultID();
		TotalWndDefectItem.AddWndDefectItemCount(ResultList[i]);

		if ( RESULT_ID_NG==ResultID || RESULT_ID_EXCEPTION==ResultID )
		{	m_ComponentTotalNGCountARS ++;	}
	}
	m_ComponentTotalTestCountARS = ResultCount2;
	m_ComponentTotalEachDefectCountARS=TotalWndDefectItem;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::CopyComponentStatisticFrom(CAOIComponent *RefComponentPtr)//複製零件統計資料
{
	if ( NULL == RefComponentPtr ) { return false; }
	
	//m_ComponentEnableAlarm = RefComponentPtr->m_ComponentEnableAlarm;//零件警報
	//m_ComponentDefectCountEnableOnARS = RefComponentPtr->m_ComponentDefectCountEnableOnARS;
	//m_ComponentDefectAlarmEnableOnAOI = RefComponentPtr->m_ComponentDefectAlarmEnableOnAOI;//零件警報
	//m_ComponentDefectAlarmEnableOnARS = RefComponentPtr->m_ComponentDefectAlarmEnableOnARS;//零件警報
	//m_ComponentDefectItemAlarmAOI = Component.m_ComponentDefectItemAlarmAOI;
	//m_ComponentDefectItemAlarmARS = Component.m_ComponentDefectItemAlarmARS;
	//m_ComponentDefectAlarmFromModeAOI = Component.m_ComponentDefectAlarmFromModeAOI;
	//m_ComponentDefectAlarmFromModeARS = Component.m_ComponentDefectAlarmFromModeARS;
	m_ComponentTotalTestCountAOI   = RefComponentPtr->m_ComponentTotalTestCountAOI;//零件檢測數量
	m_ComponentTotalTestCountAOI_LA   = RefComponentPtr->m_ComponentTotalTestCountAOI_LA;//零件檢測數量
	m_ComponentTotalTestCountAOI_LB   = RefComponentPtr->m_ComponentTotalTestCountAOI_LB;//零件檢測數量	 
	m_ComponentTotalTestCountARS   = RefComponentPtr->m_ComponentTotalTestCountARS;//零件檢測數量
	m_ComponentTotalTestCountARS_LA   = RefComponentPtr->m_ComponentTotalTestCountARS_LA;//零件檢測數量
	m_ComponentTotalTestCountARS_LB   = RefComponentPtr->m_ComponentTotalTestCountARS_LB;//零件檢測數量	
	m_ComponentTotalNGCountAOI = RefComponentPtr->m_ComponentTotalNGCountAOI ;//零件瑕疵數量
	m_ComponentTotalNGCountAOI_LA = RefComponentPtr->m_ComponentTotalNGCountAOI_LA ;//零件瑕疵數量
	m_ComponentTotalNGCountAOI_LB = RefComponentPtr->m_ComponentTotalNGCountAOI_LB ;//零件瑕疵數量
	m_ComponentTotalNGCountARS = RefComponentPtr->m_ComponentTotalNGCountARS;//零件瑕疵數量
	m_ComponentTotalNGCountARS_LA = RefComponentPtr->m_ComponentTotalNGCountARS_LA;//零件瑕疵數量
	m_ComponentTotalNGCountARS_LB = RefComponentPtr->m_ComponentTotalNGCountARS_LB;//零件瑕疵數量
	m_ComponentTotalNGCountLimit = RefComponentPtr->m_ComponentTotalNGCountLimit;//零件累計不良數量上限
	//m_ComponentTotalNGCountEnable = RefComponentPtr->m_ComponentTotalNGCountEnable;//零件累計不良數量啟用
	m_ComponentTotalNGCountAlarm = RefComponentPtr->m_ComponentTotalNGCountAlarm;//零件累計不良數量警報	
	m_ComponentContinueNGCountAOI = RefComponentPtr->m_ComponentContinueNGCountAOI;//零件連續瑕疵數量
	m_ComponentContinueNGCountARS = RefComponentPtr->m_ComponentContinueNGCountARS;//零件連續瑕疵數量		
	m_ComponentContinueNGCountLimit = RefComponentPtr->m_ComponentContinueNGCountLimit;//零件連續不良數量上限
	//m_ComponentContinueNGCountEnable = RefComponentPtr->m_ComponentContinueNGCountEnable;//零件連續不良數量啟用
	m_ComponentContinueNGCountAlarm = RefComponentPtr->m_ComponentContinueNGCountAlarm;//零件連續不良數量警報	

	m_ComponentCurrentDefectCountAOI = RefComponentPtr->m_ComponentCurrentDefectCountAOI;//零件現今每個瑕疵數量-AOI
	m_ComponentCurrentDefectCountARS = RefComponentPtr->m_ComponentCurrentDefectCountARS;//零件現今每個瑕疵數量-ARS
	m_ComponentTotaEachlDefectCountAOI = RefComponentPtr->m_ComponentTotaEachlDefectCountAOI;//零件累計每個瑕疵數量-AOI
	m_ComponentTotaEachlDefectCountAOI_LA = RefComponentPtr->m_ComponentTotaEachlDefectCountAOI_LA;//零件累計每個瑕疵數量-AOI
	m_ComponentTotaEachlDefectCountAOI_LB = RefComponentPtr->m_ComponentTotaEachlDefectCountAOI_LB;//零件累計每個瑕疵數量-AOI
	m_ComponentTotalEachDefectCountARS = RefComponentPtr->m_ComponentTotalEachDefectCountARS;//零件累計每個瑕疵數量-ARS
	m_ComponentTotalEachDefectCountARS_LA = RefComponentPtr->m_ComponentTotalEachDefectCountARS_LA;//零件累計每個瑕疵數量-ARS
	m_ComponentTotalEachDefectCountARS_LB = RefComponentPtr->m_ComponentTotalEachDefectCountARS_LB;//零件累計每個瑕疵數量-ARS
	
	m_ComponentResultListAOI = RefComponentPtr->m_ComponentResultListAOI;//零件檢測結果列表-AOI
	m_ComponentResultListARS = RefComponentPtr->m_ComponentResultListARS;//零件檢測結果列表-ARS	
	
	m_ComponentGrrSigmaItemIdx = RefComponentPtr->m_ComponentGrrSigmaItemIdx;
	m_ComponentGrrSigmaItemNull = RefComponentPtr->m_ComponentGrrSigmaItemNull;	
	m_ComponentGrrSigmaItemList = RefComponentPtr->m_ComponentGrrSigmaItemList;

	m_ComponentResultSigmaItem_LA = RefComponentPtr->m_ComponentResultSigmaItem_LA;//零件檢測標準差項目 
	m_ComponentResultSigmaItem_LB = RefComponentPtr->m_ComponentResultSigmaItem_LB;//零件檢測標準差項目 
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentPanelBasePlane(double val)
{
	SetRgnPanelBasePlane(val);
	CAOIModel *ModelPtr = GetComponentModelPtr();
	if ( NULL != ModelPtr )
	{	ModelPtr->SetModelPanelBasePlane(val); }
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::RotateComponentBasePlaneParam(double Angle)
{	
	TBasePlaneParam& Param=GetComponentSpaceBasePlaneParam();
	DWORD UseSideMode=JetAPI::RotateSideMode(Angle, Param.UseSideMode);
	if ( UseSideMode != Param.UseSideMode )
	{		
		Param.UseSideMode = UseSideMode;
		CAOIModel *ModelPtr = GetComponentModelPtr();
		if ( NULL != ModelPtr )
		{	ModelPtr->SetModelSpaceBasePlaneParam(Param); }
	}
	return ;
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentSpaceBasePlaneParam(const TBasePlaneParam& Param)
{
	SetRgnSpaceBasePlaneParam(Param); 
	CAOIModel *ModelPtr = GetComponentModelPtr();
	if ( NULL != ModelPtr )
	{	ModelPtr->SetModelSpaceBasePlaneParam(Param); }
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentSpaceNoiseFilterParam(const TNoiseFilterParam& Param)
{ 
	SetRgnSpaceNoiseFilterParam(Param); 
	CAOIModel *ModelPtr = GetComponentModelPtr();
	if ( NULL != ModelPtr )
	{	ModelPtr->SetModelSpaceNoiseFilterParam(Param); }
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentMaskEnable_Base(bool val)
{	
	CAOIRgn::SetRgnMaskEnable_Base(val);

	CAOIModel *ModelPtr = GetComponentModelPtr();
	if ( NULL != ModelPtr )
	{	ModelPtr->SetModelMaskEnable_Base(val); }
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::GetComponentMaskEnable_Base() const
{
	return GetRgnMaskEnable_Base();
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentMaskFrameIndex_Base(unsigned int val)//影像序號
{
	CAOIRgn::SetRgnMaskFrameIndex_Base(val);

	CAOIModel *ModelPtr = GetComponentModelPtr();
	ModelPtr->SetModelMaskFrameIndex_Base(val);
}
//-------------------------------------------------------------------------------------//
unsigned int CAOIComponent::GetComponentMaskFrameIndex_Base() const//影像序號
{
	return GetRgnMaskFrameIndex_Base();
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentMaskFrameUniqueID_Base(unsigned int val)//影像唯一碼
{
	CAOIRgn::SetRgnMaskFrameUniqueID_Base(val);

	CAOIModel *ModelPtr = GetComponentModelPtr();
	ModelPtr->SetModelMaskFrameUniqueID_Base(val);
}
//-------------------------------------------------------------------------------------//
unsigned int CAOIComponent::GetComponentMaskFrameUniqueID_Base() const//影像唯一碼
{
	return GetRgnMaskFrameUniqueID_Base();
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentMaskColorGroupLinkIndex(int val)//彩色過濾的連動編號
{
	CAOIRgn::SetRgnMaskColorGroupLinkIndex(val);

	CAOIModel *ModelPtr = GetComponentModelPtr();
	ModelPtr->SetModelMaskColorGroupLinkIndex(val);
}
//-------------------------------------------------------------------------------------//
int CAOIComponent::GetComponentMaskColorGroupLinkIndex() const//彩色過濾的連動編號	
{
	return GetRgnMaskColorGroupLinkIndex();
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::UpdateComponentColorGroupLinkIndex(const std::vector<CColorGroup> &ColorGroupList)//更新模組內的彩色過濾連動
{
	size_t ColorGroupIndex = 0;
	unsigned int FrameIndex = 0;
	unsigned int FrameUniqueID = 0;
	const size_t ColorGroupCount = ColorGroupList.size();

	//基板顏色
	ColorGroupIndex = GetComponentMaskColorGroupLinkIndex();
	if ( ColorGroupIndex>=0 && ColorGroupIndex<ColorGroupCount )
	{
		FrameIndex = ColorGroupList[ColorGroupIndex].GetColorGroupFrameIndex();
		FrameUniqueID = ColorGroupList[ColorGroupIndex].GetColorGroupFrameUniqueID();

		SetComponentMaskFrameIndex_Base(FrameIndex);
		SetComponentMaskFrameUniqueID_Base(FrameUniqueID);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::UpdateComponentFrameIndex(unsigned int DefaultIndex, unsigned int DefaultUniqueID, const std::vector<unsigned int> &FrameIndexMapList)
{
	unsigned int  FrameIndex=0;
	unsigned int  FrameUniqueID = 0;
	const size_t  FrameIndexMapSize = FrameIndexMapList.size();

	FrameUniqueID = GetComponentFrameUniqueID();
	if ( FrameUniqueID<0 || FrameUniqueID>=FrameIndexMapSize )
	{	
		FrameIndex = DefaultIndex;	
		FrameUniqueID = DefaultUniqueID;
	}
	else
	{	
		FrameIndex = (FrameIndexMapList[FrameUniqueID]);
		if ( -1 == FrameIndex )
		{ 
			FrameIndex = DefaultIndex;	
			FrameUniqueID = DefaultUniqueID;
		}
	}	
	SetComponentFrameIndex(FrameIndex);
	SetComponentFrameUniqueID(FrameUniqueID);

	FrameUniqueID = CAOIRgn::GetRgnMaskFrameUniqueID_Base();
	if ( FrameUniqueID<0 || FrameUniqueID>=FrameIndexMapSize )
	{	
		FrameIndex = DefaultIndex;	
		FrameUniqueID = DefaultUniqueID;
	}
	else
	{	
		FrameIndex = (FrameIndexMapList[FrameUniqueID]);
		if ( -1 == FrameIndex )
		{ 
			FrameIndex = DefaultIndex;	
			FrameUniqueID = DefaultUniqueID;
		}
	}	
	SetComponentMaskFrameIndex_Base(FrameIndex);
	SetComponentMaskFrameUniqueID_Base(FrameUniqueID);
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::UpdateComponentCurrentDefectCountARS(LANE_ID LaneID, WND_DEFECT_ID nDefect, std::vector<WND_DEFECT_ID> &nDefectList)
{
	int            i=0;
	RESULT_ID      ResultID;
	WND_DEFECT_ID  WndDefectID;
	CWndDefectItem WndDefectItem;
	const size_t   DefectCount = nDefectList.size();
	for ( i=0; i<DefectCount; i++ )
	{
		WndDefectID = nDefectList[i];
		if ( WND_DEFECT_NONE != WndDefectID ) 
		{	ResultID = RESULT_ID_NG; }
		WndDefectItem.AddItemCount(WndDefectID);
	}
	WndDefectItem.SetLaneID(LaneID);
	WndDefectItem.SetResultID(ResultID);
	SetComponentResultID_ARS(ResultID);
	UpdateComponentResultID_ARS_Lane(LaneID);
	SetComponentCurrentDefectCountARS(WndDefectItem);	
	return;
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::BackupComponentParamByVersion()//備份零件所有版本號下的狀態
{
	m_ComponentVersionParamListBackup = m_ComponentVersionParamList;
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::RestoreComponentParamByVersion()//還原零件所有版本號下的狀態
{
	m_ComponentVersionParamList = m_ComponentVersionParamListBackup;
}
//-------------------------------------------------------------------------------------//
size_t CAOIComponent::GetComponentVersionParamCount() const//取得零件版本號參數數量
{
	return m_ComponentVersionParamList.size();
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::ResetComponentVersionParam(size_t idx)//復歸零件版本號參數
{
	if ( CheckComponentVersionParamIndex(idx) == false ) { return false; }	
	TVersionParam &VersionParamRef=m_ComponentVersionParamList[idx];
	VersionParamRef = TVersionParam();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::CheckComponentVersionParamIndex(size_t idx) const//確認零件版本號參數引數
{
	if ( idx >= GetComponentVersionParamCount() )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
const TVersionParam* CAOIComponent::GetComponentVersionParamPtr(size_t idx, bool Check) const//取得零件版本號參數指標
{
	if ( CheckComponentVersionParamIndex(idx) == false ) { return NULL; }	
	return &(m_ComponentVersionParamList[idx]);
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::ChangeComponentVersionParam(size_t idx, bool &ModelChanged)//切換零件狀態至該版本參數下
{
	ModelChanged = false;
	if ( CheckComponentVersionParamIndex(idx) == false ) { return false; }
	const TVersionParam &VersionParamRef=m_ComponentVersionParamList[idx];
	SetComponentBypassed(VersionParamRef.bBypassed);
	SetComponentBypass3D(VersionParamRef.bBypassed3D);
	SetComponentXBoardUnit(VersionParamRef.bXBoardUnit);

	if ( GetComponentModelIsolated() == false )
	{
		if ( VersionParamRef.wsModelName.length() > 0 )
		{	
			const wchar_t *OldModelName=GetComponentModelName();
			if ( VersionParamRef.wsModelName != OldModelName )
			{
				ModelChanged = true;
				SetComponentModelName(VersionParamRef.wsModelName.c_str());	
				ResetComponentModel();				
			}
		}
	}

	if ( VersionParamRef.wsPartNumber.length() > 0 )
	{	SetComponentPartNumber(VersionParamRef.wsPartNumber.c_str());	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::UpdateToComponentVersionParam(size_t idx)//更新目前零件狀態至版本參數資料
{	
	if ( CheckComponentVersionParamIndex(idx) == false ) { return false; }
	TVersionParam &VersionParamRef=m_ComponentVersionParamList[idx];
	VersionParamRef.bBypassed = GetComponentBypassed();
	VersionParamRef.bBypassed3D = GetComponentBypass3D();
	VersionParamRef.bXBoardUnit = GetComponentXBoardUnit();
	VersionParamRef.wsModelName = GetComponentModelName();
	VersionParamRef.wsPartNumber = GetComponentPartNumber();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::CopyComponentVersionParam(size_t idxSrc, size_t idxDst)//複製不同版本參數下的零件狀態
{
	const size_t VersionCnt=GetComponentVersionParamCount();
	if ( idxSrc<0 || idxSrc>=VersionCnt ) { return false; }
	if ( idxDst<0 || idxDst>=VersionCnt ) { return false; }	
	m_ComponentVersionParamList[idxDst] = m_ComponentVersionParamList[idxSrc];
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::SetComponentVersionParamList(const std::vector<TVersionParam> &VersionList)//設定零件版本參數列表	
{
	size_t i=0;	
	const size_t VersionCount=VersionList.size();
	const size_t ParamCount=GetComponentVersionParamCount();
	const size_t UsedCnt=MIN(VersionCount, ParamCount);	
	for ( i=0; i<UsedCnt; i++ )
	{	m_ComponentVersionParamList[i] = VersionList[i];	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::SetComponentGroupResult(unsigned int WndIdx, RESULT_ID ResultID, LPCTSTR DefectText)//設定零件群組結果
{		
	CAOIModel *ModelPtr = GetComponentModelPtr();
	if ( NULL == ModelPtr ) { return false; }
	
	CAOIWnd *WndPtr = ModelPtr->GetModelWndPtr(WndIdx, true);
	if ( NULL == WndPtr ) { return false; }
	
	RESULT_ID WndResultID=RESULT_ID_NG;	
	WndResultID = WndPtr->GetWndResultID();
	WndResultID = WndPtr->GetWndLogicResultID();				
	if ( RESULT_ID_OK == WndResultID )
	{					
		WndPtr->SetWndResultID(ResultID);
		WndPtr->SetWndResultText(DefectText);
		WndPtr->SetWndLogicResultID(ResultID);
		ModelPtr->SetModelResultID(ResultID);
		SetComponentResultID_AOI(ResultID);
	}
	
	if ( GetComponentModelImageIsSaved() == false  )
	{
		std::vector<TUNI_FRAME> UniFrameList;	
		if ( GetRgnUniFrameList(UniFrameList) == true )
		{	ExecComponentSaveDefectImage(UniFrameList);	}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::BuildComponentBarcodeList(std::vector<std::wstring> &BarcodeList)//取得零件條碼列表
{
	CAOIModel *ModelPtr = GetComponentModelPtr();
	if ( NULL == ModelPtr ) { return false; }
	if ( ModelPtr->BuildModelBarcodeList(BarcodeList) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::GetComponentDataModelEnabled() const//取得零件資料模型啟用
{
	return CAOIRgn::GetRgnDataModelEnabled();
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentDataModelEnabled(bool val)//設定零件資料模型啟用
{
	CAOIRgn::SetRgnDataModelEnabled(val);	
	CAOIModel *ModelPtr = GetComponentModelPtr();
	if ( NULL != ModelPtr )
	{	ModelPtr->SetModelDataModelEnabled(val); }
}
//-------------------------------------------------------------------------------------//
int CAOIComponent::GetComponentDataModelLevelID() const//取得零件資料模型等級
{
	return CAOIRgn::GetRgnDataModelLevelID();
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentDataModelLevelID(int val)//設定零件資料模型等級
{
	CAOIRgn::SetRgnDataModelLevelID(val);	
	CAOIModel *ModelPtr = GetComponentModelPtr();
	if ( NULL != ModelPtr )
	{	ModelPtr->SetModelDataModelLevelID(val); }
}
//-------------------------------------------------------------------------------------//
double CAOIComponent::GetComponentNPM_APC_MffX() const//取得APC-零件偏移-X-um
{
	return m_NPM_APC_MffX;
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentNPM_APC_MffX(double val)//設定APC-零件偏移-X-um
{
	m_NPM_APC_MffX = val;
}
//-------------------------------------------------------------------------------------//
double CAOIComponent::GetComponentNPM_APC_MffY() const//取得APC-零件偏移-Y-um
{
	return m_NPM_APC_MffY;
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentNPM_APC_MffY(double val)//設定APC-零件偏移-Y-um	
{
	m_NPM_APC_MffY = val;
}
//-------------------------------------------------------------------------------------//
double CAOIComponent::GetComponentNPM_APC_MffA() const//取得APC-零件偏移-A-%
{
	return m_NPM_APC_MffA;
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentNPM_APC_MffA(double val)//設定APC-零件偏移-A-%
{
	m_NPM_APC_MffA = val;
}
//-------------------------------------------------------------------------------------//
unsigned int CAOIComponent::GetComponentNPM_APC_FF1_IDNUM() const//取得APC-FF1-零件編號
{
	return m_NPM_APC_FF1_IDNUM;
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentNPM_APC_FF1_IDNUM(unsigned int val)//設定APC-FF1-零件編號
{
	m_NPM_APC_FF1_IDNUM = val;
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::CheckComponentNPM_APC_FF1_Done() const//確認APC-FF1-已經設定
{
	if ( -1 == GetComponentNPM_APC_FF1_IDNUM() ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
double CAOIComponent::GetComponentNPM_APC_EPosX() const//取得APC-電極偏移-X-um
{
	return m_NPM_APC_EPosX;
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentNPM_APC_EPosX(double val)//設定APC-電極偏移-X-um
{
	m_NPM_APC_EPosX = val;
}
//-------------------------------------------------------------------------------------//
double CAOIComponent::GetComponentNPM_APC_EPosY() const//取得APC-電極偏移-Y-um
{
	return m_NPM_APC_EPosY;
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentNPM_APC_EPosY(double val)//設定APC-電極偏移-Y-um
{
	m_NPM_APC_EPosY = val;
}
//-------------------------------------------------------------------------------------//
double CAOIComponent::GetComponentNPM_APC_EPosA() const//取得APC-電極偏移-A-%
{
	return m_NPM_APC_EPosA;
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentNPM_APC_EPosA(double val)//設定APC-電極偏移-A-%
{
	m_NPM_APC_EPosA = val;
}
//-------------------------------------------------------------------------------------//
unsigned int CAOIComponent::GetComponentNPM_APC_MFB_IDNUM() const//取得APC-MFB-零件編號
{
	return m_NPM_APC_MFB_IDNUM;
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentNPM_APC_MFB_IDNUM(unsigned int val)//設定APC-MFB-零件編號
{
	m_NPM_APC_MFB_IDNUM = val;
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::CheckComponentNPM_APC_MFB_Done() const//確認APC-MFB-已經設定
{
	if ( -1 == GetComponentNPM_APC_MFB_IDNUM() ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::CalcComponentNPM_APC_Result(TNPM_APC_Result &APC_Result)//計算零件的NPM-APC結果
{
	bool bCalc = false;	
	if ( CheckComponentNPM_APC_FF1_Done()==true || CheckComponentNPM_APC_MFB_Done()==true )
	{	bCalc = true;	}
	else
	{	return true;	}

	const double Raio=1000.0;
	const double CadPosX=GetComponentCadPosX();
	const double CadPosY=GetComponentCadPosY();
	const double APC_Skew=(GetComponentNPM_APC_MffA()+GetComponentNPM_APC_EPosA());
	const double APC_OffsetX=(GetComponentNPM_APC_MffX()+GetComponentNPM_APC_EPosX())*Raio;
	const double APC_OffsetY=(GetComponentNPM_APC_MffY()+GetComponentNPM_APC_EPosY())*Raio;
	const double APC_CadCalPosX=CadPosX+APC_OffsetX;
	const double APC_CadCalPosY=CadPosY+APC_OffsetY;	
	const double CadResultPosX=GetComponentCadResultX();
	const double CadResultPosY=GetComponentCadResultY();
	APC_Result.dGapX = (CadResultPosX-APC_CadCalPosX)/Raio;//um -> mm
	APC_Result.dGapY = (CadResultPosY-APC_CadCalPosY)/Raio;//um -> mm
	APC_Result.dGapA = 0.0;

	APC_Result.dSX = 0.0;
	APC_Result.dSY = 0.0;
	APC_Result.dST = 0.0;

	APC_Result.nSTS = 0;
	APC_Result.nNgCode = 0;
	 
	DWORD NgCode=0;
	const CWndDefectItem &WndDefectItem=GetComponentCurrentDefectCountAOI();
	//Bit0:0x00000001 Missing component NG
	//Bit1:0x00000002 Gap X NG 
	//Bit2:0x00000004 Gap Y NG
	//Bit3:0x00000008 Gap A (angle) NG
	//Bit4:0x00000010 Size X NG
	//Bit5:0x00000020 Size Y NG
	//Bit6:0x00000040 Score NG (Position measurement process score, etc.)
	//Bit7:0x00000080 Top/Bottom reverse NG
	//Bit8:0x00000100 Polarity NG
	//Bit9:0x00000200 Foreign object NG
	//Bit10:0x00000400 Other NG
	//Bit11:0x00000800 Measure in different mode at position measurement error △7
	//Bit11~15: Unused
	//Bit16:0x00010000 (Warning) Missing component NG
	//Bit17:0x00020000 (Warning) Gap X NG
	//Bit18:0x00040000 (Warning) Gap Y NG
	//Bit19:0x00080000 (Warning) Gap A (angle) NG
	//Bit20:0x00100000 (Warning) Size X NG
	//Bit21:0x00200000 (Warning) Size Y NG
	//Bit22:0x00400000 (Warning) Score NG
	//Bit23:0x00800000 (Warning) Top/Bottom reverse NG
	//Bit24:0x01000000 (Warning) Polarity NG
	//Bit25:0x02000000 (Warning) Foreign object NG
	//Bit26:0x04000000 (Warning) Other NG
	//Bit27:0x08000000 Uninspected due to bad mark(Inspection is not performed because the pattern is set with bad marks.)
	//Bit28:0x10000000 Grand NG(Inspection is not performed because ref-erence height measurement failed.)
	//Bit29:0x20000000Measurement unavailable(The component measurement process failed due to internal process errors.)
	//Bit30:0x40000000 Uninspected due to calibration error(Inspection is not performed because a calibration mark recognition error occurred.)
	//Bit31:0x80000000 Uninspected due to inspection flag OFF(Inspection is not performed because the
	size_t i=0;
	bool   bIsPassX=true;
	bool   bIsPassY=true;
	bool   bIsPassA=true;
	size_t WndCount = 0;	
	CAOIWnd   *WndPtr=NULL;
	WND_DEFECT_ID WndDefectID;
	CAOIModel *ModelPtr=GetComponentModelPtr();
	if ( NULL != ModelPtr )
	{	WndCount = ModelPtr->GetModelWndCount();	}

	if ( 0!=WndDefectItem.GetItemCount(WND_DEFECT_PART_ALIGN) || 0!=WndDefectItem.GetItemCount(WND_DEFECT_LEAD_ADJUST) || 0!=WndDefectItem.GetItemCount(WND_DEFECT_BODY_OFFSET) )
	{
		for ( i=0; i<WndCount; i++ )
		{
			WndPtr = ModelPtr->GetModelWndPtr(i, false);
			if ( NULL == WndPtr ) { continue; }
			if ( WndPtr->GetWndEnabled() == false ) { continue; }
			WndDefectID = WndPtr->GetWndDefectID();
			switch ( WndDefectID )
			{
			case WND_DEFECT_PART_ALIGN:				
			case WND_DEFECT_LEAD_ADJUST:				
			case WND_DEFECT_BODY_OFFSET:				
				if ( CAlgParam::CheckOK_PatternMatchOffsetX(WndPtr->GetWndAlgParam()) == false )
				{	bIsPassX = false; }
				if ( CAlgParam::CheckOK_PatternMatchOffsetY(WndPtr->GetWndAlgParam()) == false )
				{	bIsPassY = false; }
				if ( CAlgParam::CheckOK_PatternMatchSkewAngle(WndPtr->GetWndAlgParam()) == false )
				{	bIsPassA = false; }				 
				break;
			}
		}

		int nOffsetNg=0;
		if ( false==bIsPassX )
		{
			nOffsetNg ++;
			NgCode |= 0x00000002;	
		}
		if ( false==bIsPassY )
		{
			nOffsetNg ++;
			NgCode |= 0x00000004;	
		}
		if ( false==bIsPassA )
		{
			nOffsetNg ++;
			NgCode |= 0x00000008;	
		}
		if ( 0 == nOffsetNg )
		{	NgCode |= 0x00000040;	}		
	}

	if ( 0 != WndDefectItem.GetItemCount(WND_DEFECT_BODY_MISSING) )
	{	NgCode |= 0x00000001;	}

	if ( 0 != WndDefectItem.GetItemCount(WND_DEFECT_BODY_TILT) )
	{	NgCode |= 0x00000400;	}

	if ( 0 != WndDefectItem.GetItemCount(WND_DEFECT_BODY_POLARITY) )
	{	NgCode |= 0x00000100;	}

	if ( 0 != WndDefectItem.GetItemCount(WND_DEFECT_BODY_TURNOVER) )
	{	NgCode |= 0x00000080;	}

	if ( 0!=WndDefectItem.GetItemCount(WND_DEFECT_BODY_MOUNT) || 0!=WndDefectItem.GetItemCount(WND_DEFECT_BODY_WRONG_CODE) || 0!=WndDefectItem.GetItemCount(WND_DEFECT_BODY_WRONG_TEXT) )
	{	NgCode |= 0x00000200;	}
	
	if ( 0 == NgCode )
	{
		if ( RESULT_ID_NG == WndDefectItem.GetResultID() )
		{	NgCode |= 0x00000400;	}
	}

	APC_Result.nNgCode = NgCode;
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::ResetComponentNPM_APC_FF1()//復歸NPM-APC-FF1-參數
{
	double val=0.0;	
	SetComponentNPM_APC_MffX(val);
	SetComponentNPM_APC_MffY(val);
	SetComponentNPM_APC_MffA(val);
	SetComponentNPM_APC_FF1_IDNUM(-1);
	return;
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::ResetComponentNPM_APC_MFB()//復歸NPM-APC-MFB-參數
{
	double val=0.0;
	SetComponentNPM_APC_EPosX(val);
	SetComponentNPM_APC_EPosY(val);
	SetComponentNPM_APC_EPosA(val);
	SetComponentNPM_APC_MFB_IDNUM(-1);
	return;
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::ResetComponentNPM_APC_Param()//復歸NPM-APC-參數
{
	ResetComponentNPM_APC_FF1();
	ResetComponentNPM_APC_MFB();
	return;
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::GetComponentHASI_SPIOffset_Enable()
{
	return m_HASI_SPIOffset_Enable;
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentHASI_SPIOffset_Enable(bool State)
{
	m_HASI_SPIOffset_Enable = State;
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::GetComponentHASI_SPIOffset_IsApplied()
{
	return m_HASI_SPIOffset_IsApplied;
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentHASI_SPIOffset_IsApplied(bool State)
{
	m_HASI_SPIOffset_IsApplied = State;
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::GetComponentHASI_SaveImage()
{
	return m_HASI_SaveImage;
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentHASI_SaveImage(bool value)
{
	m_HASI_SaveImage = value;
}
//-------------------------------------------------------------------------------------//
bool CAOIComponent::CalcComponentHASI_Result(CString & DefectCode)
{

	const CWndDefectItem &WndDefectItem = GetComponentCurrentDefectCountARS();
	size_t WndCount = 0;
	CAOIWnd   *WndPtr = NULL;
	WND_DEFECT_ID WndDefectID;
	CAOIModel *ModelPtr = GetComponentModelPtr();
	if (NULL == ModelPtr) { return false; }
	int DefectCount = 0;
	WndCount = ModelPtr->GetModelWndOrderCount();
	for (size_t i = 0; i < WndCount; i++)
	{
		WndPtr = ModelPtr->GetModelWndOrderPtr(i, false);
		if (NULL == WndPtr) { continue; }
		if (WndPtr->GetWndEnabled() == false) { continue; }
		WndDefectID = WndPtr->GetWndDefectID();
		DefectCount = WndDefectItem.GetItemCount(WndDefectID);
		if (DefectCount > 0) {
			DefectCode = AOIDataDefine.GetWndDefectIDText(WndDefectID);
			return true;
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::ResetComponentResultPtr()//重設零件結果指標
{
	SetComponentResultPtr(this);
}
//-------------------------------------------------------------------------------------//
CAOIComponent* CAOIComponent::GetComponentResultPtr()//取得零件結果指標
{
	if ( NULL == m_ComponentResultPtr ) { return this; }
	return m_ComponentResultPtr;
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::SetComponentResultPtr(CAOIComponent* Ptr)//設定零件結果指標
{
	m_ComponentResultPtr = Ptr;
}
//-------------------------------------------------------------------------------------//
void CAOIComponent::ResetComponentMaster()//重設零件本尊
{
	SetComponentMasterPtr(NULL);	
	return;
}
//-------------------------------------------------------------------------------------//
CAOIComponent* CAOIComponent::GetComponentMasterPtr()//取得零件本尊
{
	return m_ComponentMasterPtr;
}
//---------------------------------------------------------------------------------//
void CAOIComponent::SetComponentMasterPtr(CAOIComponent* Ptr)//設定零件本尊
{
	m_ComponentMasterPtr = Ptr;	
	if ( NULL == Ptr )
	{	SetComponentMasterIndex(-1);	}
	else
	{	SetComponentMasterIndex(Ptr->GetComponentIndex_Project());	}	
	return;
}
//---------------------------------------------------------------------------------//
unsigned int CAOIComponent::GetComponentMasterIndex() const//取得零件本尊引數
{
	return m_ComponentMasterIndex;
}
//---------------------------------------------------------------------------------//
void CAOIComponent::SetComponentMasterIndex(unsigned int value)//設定零件本尊引數
{
	m_ComponentMasterIndex = value;
}
//---------------------------------------------------------------------------------//
bool CAOIComponent::CheckComponentIsMaster() const//確認零件是否為本尊
{
	if ( 0 == m_ComponentAgentList.size() ) { return false; }
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIComponent::CheckComponentIsAgent() const//確認零件是否為代理人
{
	if ( -1 == m_ComponentMasterIndex ) { return false; }	
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIComponent::CheckComponentIsMasterOrAgent() const//確認零件是否為本尊或代理人
{
	if ( CheckComponentIsMaster() || CheckComponentIsAgent() ) { return true; }
	return false;
}
//---------------------------------------------------------------------------------//
bool CAOIComponent::ChangeComponentMaster(CAOIComponent *MasterPtr)//變更零件本尊
{
	if ( NULL == MasterPtr ) { return false; }
	if ( this == MasterPtr ) { return true; }
	
	CAOIComponent *AgentPtr=NULL;
	const size_t AgentCount=GetComponentAgentCount();
	
	MasterPtr->ResetComponentMaster();
	MasterPtr->ClearComponentAgentList();
		

	MasterPtr->AddComponentAgentPtr(this);
	for ( size_t i=0; i<AgentCount; i++ )
	{
		AgentPtr = GetComponentAgentPtr(i, false);
		if ( NULL == AgentPtr ) { continue; }	
		if ( AgentPtr == MasterPtr ) { continue; }
		MasterPtr->AddComponentAgentPtr(AgentPtr);
	}
	ClearComponentAgentList();
	if ( GetComponentModelIsolated() == true )
	{			
		SetComponentModelIsolated(false);
		CAOIModel *ModelPtr=GetComponentModelPtr();
		ModelPtr->SetModelIsolated(false);
		ModelPtr->ClearModelAllObjList();			
	}	
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIComponent::UpdateComponentMasterResultID()//確認零件本尊結果編號
{	
	RESULT_ID AgentResultID;	
	CAOIComponent *AgentPtr=NULL;
	CAOIComponent *ResultPtr=this;	
	RESULT_ID ResultID = GetComponentResultID_AOI();
	const size_t AgentCount=GetComponentAgentCount();
	if ( RESULT_ID_NONE == ResultID )//本身沒有檢測-調適代理模組
	{
		for ( size_t i=0; i<AgentCount; i++ )
		{
			AgentPtr = GetComponentAgentPtr(i, false);
			if ( NULL == AgentPtr ) { continue; }
			if ( AgentPtr->GetComponentNeedToCalculate() == false ) { continue; }
			AgentResultID = AgentPtr->GetComponentResultID_AOI();						
			if ( RESULT_ID_NONE == AgentResultID ) { continue; }			
			ResultPtr = AgentPtr;
			ResultID = AgentResultID;	
			break;
		}
	}
	if ( RESULT_ID_OK != ResultID )
	{	
		for ( size_t i=0; i<AgentCount; i++ )
		{
			AgentPtr = GetComponentAgentPtr(i, false);
			if ( NULL == AgentPtr ) { continue; }
			if ( AgentPtr->GetComponentNeedToCalculate() == false ) { continue; }
			AgentResultID = AgentPtr->GetComponentResultID_AOI();						
			if ( RESULT_ID_OK == AgentResultID )
			{
				ResultPtr = AgentPtr;
				ResultID = AgentResultID;
				break;
			}
		}		
	}	
	SetComponentResultPtr(ResultPtr);		
	return true;
}
//---------------------------------------------------------------------------------//
unsigned int CAOIComponent::GetComponentAgentIndex() const//取得零件代理人編號
{
	return m_ComponentAgentIndex;
}
//---------------------------------------------------------------------------------//
void CAOIComponent::SetComponentAgentIndex(unsigned int value)//設定零件代理人編號	
{
	m_ComponentAgentIndex = value;
}
//---------------------------------------------------------------------------------//
void CAOIComponent::ClearComponentAgentList()//清除零件代理人列表
{
	m_ComponentAgentList.clear();
}
//---------------------------------------------------------------------------------//
size_t CAOIComponent::GetComponentAgentCount() const//取得零件代理人數量
{
	return m_ComponentAgentList.size();
}
//---------------------------------------------------------------------------------//
bool CAOIComponent::CheckComponentAgentSelected()//確認零件代理人有被選到
{
	CAOIComponent *AgentPtr=NULL;
	const size_t AgentCount=GetComponentAgentCount();
	for ( size_t i=0; i<AgentCount; i++ )
	{
		AgentPtr = GetComponentAgentPtr(i, false);
		if ( NULL == AgentPtr ) { continue; }
		if ( AgentPtr->GetComponentSelected() == true )
		{	return true; }
	}
	return false;
}
//---------------------------------------------------------------------------------//
bool CAOIComponent::SetComponentAllAgentSelected(bool val)//設定零件所有代理人選取狀態
{
	CAOIComponent *AgentPtr=NULL;
	const size_t AgentCount=GetComponentAgentCount();
	for ( size_t i=0; i<AgentCount; i++ )
	{
		AgentPtr = GetComponentAgentPtr(i, false);
		if ( NULL == AgentPtr ) { continue; }
		AgentPtr->SetComponentSelected(val);
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIComponent::CheckComponentOrAgentNeedToCalculate()//確認零件或代理人有要去計算
{
	if ( GetComponentNeedToCalculate() == true ) { return true; }
	const size_t AgentCount=GetComponentAgentCount();
	for ( size_t i=0; i<AgentCount; i++ )
	{
		CAOIComponent *AgentPtr=GetComponentAgentPtr(i, false);
		if ( NULL == AgentPtr ) { continue; }
		if ( AgentPtr->GetComponentNeedToCalculate() == true )
		{	return true; }
	}
	return false;
}
//---------------------------------------------------------------------------------//
bool CAOIComponent::CheckComponentAgentExist(LPCTSTR PartNumber)//確認零件代理人已經存在
{
	CAOIComponent *AgentPtr=GetComponentAgentPtrByPartNumber(PartNumber);
	if ( NULL == AgentPtr )
	{	return false; }
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIComponent::SetComponentBeAgent(LPCTSTR PartNumber, LPCTSTR LibFolder)//設定零件成為代理人
{	
	ClearComponentAgentList();
	SetComponentUniqueID(-1);
	SetComponentAgentIndex(-1);
	SetComponentModelName(PartNumber);
	SetComponentPartNumber(PartNumber);	

	CAOIModel *ModelPtr=GetComponentModelPtr();	
	if ( NULL != ModelPtr )
	{
		CString ModelFolder;
		ModelPtr->ClearModelAllObjList();	
		ModelPtr->SetModelType(MODEL_TYPE_NULL);
		ModelPtr->SetModelName(PartNumber);
		ModelFolder.Format(_T("%s\\%s"), LibFolder, PartNumber);
		ModelPtr->SetModelFolderModel(ModelFolder);
		ModelPtr->AssignModelFolder();
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIComponent::AddComponentAgentPtr(CAOIComponent *Ptr)//加入零件代理人
{
	if ( NULL == Ptr ) { return false; }	
	CString PartNumber=GetComponentPartNumber();
	CString PartNumber2=Ptr->GetComponentPartNumber();
	if ( PartNumber.CompareNoCase(PartNumber2) == 0 )
	{	return false; }
	const size_t Index=GetComponentAgentCount();

	Ptr->SetComponentMasterPtr(this);
	Ptr->SetComponentAgentIndex(Index);
	Ptr->UpdateComponentAgentPtr(this);

	m_ComponentAgentList.push_back(Ptr);
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIComponent::RemoveComponentAgentSelected()//移除零件代理人選取到 
{
	CAOIComponent *AgentPtr=NULL;
	std::vector<CAOIComponent*> SelList;
	std::vector<CAOIComponent*> List=m_ComponentAgentList;
	const size_t Count=List.size();
	for ( size_t i=0; i<Count; i++ )
	{
		AgentPtr = List[i];
		if ( NULL == AgentPtr ) { continue; }
		if ( AgentPtr->GetComponentSelected() == false ) { continue; }
		SelList.push_back(AgentPtr);
	}
	const size_t SelCount=SelList.size();
	if ( 0 == SelCount ) { return true; }
	
	for ( size_t i=0; i<SelCount; i++ )
	{
		AgentPtr = SelList[i];
		if ( NULL == AgentPtr ) { continue; }
		AgentPtr->ResetComponentMaster();		
		AgentPtr->SetComponentAgentIndex(-1);
	}	

	ClearComponentAgentList();
	for ( size_t i=0; i<Count; i++ )
	{		
		AgentPtr=List[i];
		if ( NULL == AgentPtr ) { continue; }
		if ( AgentPtr->GetComponentSelected() == true ) { continue; }
		unsigned int index=(unsigned int)(m_ComponentAgentList.size());
		AgentPtr->SetComponentAgentIndex(index);
		m_ComponentAgentList.push_back(AgentPtr);
	}	

	CAOIComponent *ResultPtr=GetComponentResultPtr();
	if ( ResultPtr != this )
	{
		if ( ResultPtr->GetComponentSelected() == true )
		{	ResetComponentResultPtr(); }
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIComponent::UpdateComponentAgentPtr(CAOIComponent *Ptr)//更新零件代理人
{
	if ( NULL == Ptr ) { return false; }
	SetComponentName(Ptr->GetComponentName());	
	SetComponentDistrictID(Ptr->GetComponentDistrictID());	
	SetComponentXBoardUnit(Ptr->GetComponentXBoardUnit());
	SetComponentNozzleName(Ptr->GetComponentNozzleName());	

	//SetComponentName(Ptr->GetComponentName());
	//SetComponentAngle(Ptr->GetComponentAngle());
	//SetComponentCadPosX(Ptr->GetComponentCadPosX());
	//SetComponentCadPosY(Ptr->GetComponentCadPosY());
	//SetComponentStagePosX(Ptr->GetComponentStagePosX());
	//SetComponentStagePosY(Ptr->GetComponentStagePosY());
	return true;
}
//---------------------------------------------------------------------------------//
CAOIComponent* CAOIComponent::GetComponentAgentPtr(size_t idx, bool bCheck)//取得零件代理人指標	
{
	if ( true == bCheck )
	{
		const size_t Count=GetComponentAgentCount();
		if ( idx<0 || idx>=Count )
		{	return NULL; }
	}
	return m_ComponentAgentList[idx];
}
//---------------------------------------------------------------------------------//
CAOIComponent* CAOIComponent::GetComponentAgentPtrByPartNumber(LPCTSTR PartNumber)//取得零件代理人指標-依照料號
{
	CString strPartNumber=GetComponentPartNumber();
	if ( strPartNumber.CompareNoCase(PartNumber) == 0 ) { return this; }

	CAOIComponent *AgentPtr=NULL;
	const size_t AgentCount=GetComponentAgentCount();
	for ( size_t i=0; i<AgentCount; i++ )
	{
		AgentPtr = GetComponentAgentPtr(i, false);
		if ( NULL == AgentPtr ) { continue; }
		strPartNumber=AgentPtr->GetComponentPartNumber();
		if ( strPartNumber.CompareNoCase(PartNumber) != 0 ) { continue; }
		return AgentPtr;		
	}
	return NULL;
}
//---------------------------------------------------------------------------------//
unsigned int CAOIComponent::FindComponentAgentIndex(const CAOIComponent *Ptr)//尋找零件代理人編號		
{
	unsigned int index=-1;
	const size_t AgentCount=GetComponentAgentCount();
	for ( size_t i=0; i<AgentCount; i++ )
	{
		if ( Ptr != GetComponentAgentPtr(i, false) ) { continue; }
		index = (unsigned int)(i);
		break;
	}
	return index;
}
//---------------------------------------------------------------------------------//
void CAOIComponent::ChangeComponentSelected(bool value)//變更零件選取狀態
{
	ResetComponentResultPtr();
	SetComponentSelected(value);
	if ( CheckComponentIsAgent() == true )
	{
		CAOIComponent *MasterPtr=GetComponentMasterPtr();
		if ( NULL != MasterPtr )
		{	MasterPtr->SetComponentResultPtr(this);	}
	}	
	return;
}
//---------------------------------------------------------------------------------//
void CAOIComponent::ChangeComponentMasterName(LPCTSTR Name)//變更零件本尊名稱
{
	CAOIComponent *MasterPtr=GetComponentMasterPtr();
	if ( NULL == MasterPtr )
	{	MasterPtr = this; }
	MasterPtr->SetComponentName(Name);

	const size_t AgentCount=MasterPtr->GetComponentAgentCount();
	for ( size_t i=0; i<AgentCount; i++ )
	{
		CAOIComponent *AgentPtr=MasterPtr->GetComponentAgentPtr(i, false);	
		if ( NULL == AgentPtr ) { continue; }
		AgentPtr->SetComponentName(Name);
	}
	return;
}
//---------------------------------------------------------------------------------//
void CAOIComponent::ChangeComponentXBoardUnitMaster(bool value)//變更零件本尊報廢件
{
	bool bSucc=true;
	CAOIComponent *MasterPtr=GetComponentMasterPtr();
	if ( NULL == MasterPtr )
	{	MasterPtr = this; }
	MasterPtr->SetComponentXBoardUnit(value);

	const size_t AgentCount=MasterPtr->GetComponentAgentCount();
	for ( size_t i=0; i<AgentCount; i++ )
	{
		CAOIComponent *AgentPtr=MasterPtr->GetComponentAgentPtr(i, false);	
		if ( NULL == AgentPtr ) { continue; }
		AgentPtr->SetComponentXBoardUnit(value);
	}	
	return ;
}
//---------------------------------------------------------------------------------//