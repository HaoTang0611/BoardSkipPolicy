// AOIModelDefaultWnd_v2.cpp: implementation of the CAOIModel class.
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
bool CAOIModel::AddModelDefaultWnd_PartAlignKernel_v2(const TMODEL_DEFAULT_WND_PARAM &Param, int LandGroupID)//增加模組檢測框-零件定位	
{
	TMODEL_DEFAULT_WND_PARAM Param2=Param;
	const bool bChipMode=CheckModelTypUseChipSizeMode(Param2.eModelType);
	if ( true == bChipMode )
	{	Param2.bUseLogic = true;	}
	else
	{	Param2.bUseLogic = false;	}
	if ( true == bChipMode )
	{	Param2.nPartAlignMode = PART_ALIGN_MODE_BY_MODEL_MATCH_2D;		}
	else
	{	Param2.nPartAlignMode = PART_ALIGN_MODE_BY_MODEL_MATCH_3D;	}
	if ( AddModelDefaultWnd_PartAlignKernel_v1(Param2, LandGroupID) == false )
	{	return false; }
	if ( true == Param2.bUseLogic )
	{
		switch ( Param2.nPartAlignMode )
		{
		case PART_ALIGN_MODE_BY_MODEL_MATCH_2D:	Param2.nPartAlignMode = PART_ALIGN_MODE_BY_MODEL_MATCH_3D;	break;
		case PART_ALIGN_MODE_BY_MODEL_MATCH_3D:	Param2.nPartAlignMode = PART_ALIGN_MODE_BY_MODEL_MATCH_2D;	break;
		case PART_ALIGN_MODE_BY_OBJECT_MEASURE:	Param2.nPartAlignMode = PART_ALIGN_MODE_BY_MODEL_MATCH_2D;	break;			
		}
		if ( AddModelDefaultWnd_PartAlignKernel_v1(Param2, LandGroupID) == false )
		{	return false; }	
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd_PolarityKernel_v2(const TMODEL_DEFAULT_WND_PARAM &Param)//增加模組檢測框-極性檢測
{
	CAOIWnd *WndPtr = NULL;
	TPOINT2D  psBody, psWnd;
	TSIZE2D   szBody, szWnd;	
	TREGION4D RgnBody, RgnWnd;
	
	CAOIModel::GetModelBodyPos(psBody);
	CAOIModel::GetModelBodyRegion(RgnBody);
	szBody.cx = RgnBody.maxX-RgnBody.minX;
	szBody.cy = RgnBody.maxY-RgnBody.minY;	
	const double ExtX = Param.dPolarityExtendRange;
	const double ExtY = Param.dPolarityExtendRange;

	psWnd = psBody;
	szWnd = szBody;

	WndPtr = AOIObjManager.CreateWndObj();
	if ( NULL == WndPtr ) { return false; }

	double SizeU = 0;
	double SizeV = 0;
	double RatioU = 0.4;
	double RatioV = 0.25;
	const double GapU = 50;
	const double GapV = 50;
	const double MaxSizeU = 1000;
	const double MaxSizeV = 1000;
	ALG_TYPE   AlgType = ALG_IMAGE_MATCH;
	BOX_TOWARD Toward = GetModelBodyBox().GetBoxToward();
	const int WndGouprID = GetModelWndFreeGroupID();
	const int WndBandID = GetModelWndFreeBandID(WndGouprID);
	const WND_DEFECT_ID WndDefectID=WND_DEFECT_BODY_POLARITY;
	const int DefectGroupID = GetModelFreeDefectGroupID(WndDefectID);
	const bool UseExtendBox = CAlgParam::CheckAlgUseExtendBox(AlgType);
	WND_FOLLOW_MODE WndFollowMode = AOIDataDefine.GetWndFollowModeByWndDefectID(WndDefectID);
	unsigned int FrameIndex = Param.nFrameIndex_Low;
	unsigned int FrameUniqueID = Param.nFrameUniqueID_Low;

	switch ( Toward )
	{
	case BOX_TOWARD_UP:
		SizeU = szWnd.cx*RatioU;
		if ( SizeU > MaxSizeU ) { SizeU = MaxSizeU; }
		SizeV = szWnd.cy*RatioV;
		if ( SizeV > MaxSizeV ) { SizeV = MaxSizeV; }	

		RgnWnd.maxY = RgnBody.maxY-(GapV);
		RgnWnd.maxX = RgnBody.maxX-(GapU);
		RgnWnd.minX = RgnWnd.maxX-(SizeU);		
		RgnWnd.minY = RgnWnd.maxY-(SizeV);
		break;
	case BOX_TOWARD_LEFT:
		SizeU = szWnd.cy*RatioU;
		if ( SizeU > MaxSizeU ) { SizeU = MaxSizeU; }
		SizeV = szWnd.cx*RatioV;
		if ( SizeV > MaxSizeV ) { SizeV = MaxSizeV; }

		RgnWnd.minX = RgnBody.minX+(GapV);
		RgnWnd.maxY = RgnBody.maxY-(GapU);
		RgnWnd.minY = RgnWnd.maxY-(SizeU);
		RgnWnd.maxX = RgnWnd.minX+(SizeV);
		break;
	case BOX_TOWARD_DOWN:
		SizeU = szWnd.cx*RatioU;
		if ( SizeU > MaxSizeU ) { SizeU = MaxSizeU; }
		SizeV = szWnd.cy*RatioV;
		if ( SizeV > MaxSizeV ) { SizeV = MaxSizeV; }

		RgnWnd.minY = RgnBody.minY+(GapV);
		RgnWnd.minX = RgnBody.minX+(GapU);
		RgnWnd.maxX = RgnWnd.minX+(SizeU);
		RgnWnd.maxY = RgnWnd.minY+(SizeV);
		break;
	case BOX_TOWARD_RIGHT:
		SizeU = szWnd.cy*RatioU;
		if ( SizeU > MaxSizeU ) { SizeU = MaxSizeU; }
		SizeV = szWnd.cx*RatioV;
		if ( SizeV > MaxSizeV ) { SizeV = MaxSizeV; }

		RgnWnd.maxX = RgnBody.maxX-(GapV);
		RgnWnd.minY = RgnBody.minY+(GapU);
		RgnWnd.maxY = RgnWnd.minY+(SizeU);
		RgnWnd.minX = RgnWnd.maxX-(SizeV);
		break;
	}	

	WndPtr->SetWndToward(Toward);
	WndPtr->SetWndBandID(WndBandID);
	WndPtr->SetWndGroupID(WndGouprID);
	WndPtr->SetWndRgnLinkAuto(false);
	WndPtr->SetWndRgnLinkMode(WND_RGN_LINK_NONE);
	WndPtr->SetWndDefectID(WndDefectID);
	WndPtr->SetWndDefectGroupID(DefectGroupID);
	WndPtr->SetWndFollowMode(WndFollowMode);
	WndPtr->SetWndRegion(RgnWnd);	
	WndPtr->SetWndSelected(true);
	WndPtr->SetWndExtendBoxUsed(UseExtendBox);
	WndPtr->SetWndExtendRangeX(ExtX);
	WndPtr->SetWndExtendRangeY(ExtY);	
	WndPtr->UpdateWndExtendBox();
	WndPtr->SetWndAlgType(AlgType);

	CAlgParam                 &AlgParam = WndPtr->GetWndAlgParam();
	CAlgBinaryParam           &BinaryParam = AlgParam.GetAlgImageBinParam();
	AlgParam.CheckAlgPatternFileUsed();	
	BinaryParam.SetBinaryFrameIndex(FrameIndex);
	BinaryParam.SetBinaryFrameUniqueID(FrameUniqueID); 

	ApplyModelDefaultBodyWnd(WndPtr, Param);
	InitialModelWndPtr(WndPtr);
	AddModelWndPtr(WndPtr, false);		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd_BodyMissingKernel_v2(const TMODEL_DEFAULT_WND_PARAM &Param)//增加模組檢測框-本體缺件
{
	CAOIWnd *WndPtr = NULL;
	TPOINT2D  psBody, psWnd;
	TSIZE2D   szBody, szWnd;	
	TREGION4D RgnBody, RgnWnd;

	GetModelBodyPos(psBody);
	GetModelBodyRegion(RgnBody);
	szBody.cx = RgnBody.maxX-RgnBody.minX;
	szBody.cy = RgnBody.maxY-RgnBody.minY;

	psWnd = psBody;
	szWnd = szBody;

	WndPtr = AOIObjManager.CreateWndObj();
	if ( NULL == WndPtr ) { return false; }

	ALG_TYPE   AlgType = ALG_BRIGHT_RATIO;	

	double Size = 0;
	double SizeX = 0;
	double SizeY = 0;	
	double RatioX = 0.5;
	double RatioY = 0.5;	
	const double MaxSizeX = 1000;
	const double MaxSizeY = 1000;
	const double MinSizeX = 50;
	const double MinSizeY = 50;
	const double BodyHeight = GetModelBodyHeight();
	const double BodyArea = szWnd.cx*szWnd.cy;
	const double BodyVolume = BodyArea*BodyHeight;
	CHIP_SIZE_MODE ChipSizeMode = Param.eChipSizeMode;
	
	BOX_TOWARD Toward = GetModelBodyBox().GetBoxToward();
	const int WndGouprID = GetModelWndFreeGroupID();
	const int WndBandID = GetModelWndFreeBandID(WndGouprID);		
	const WND_DEFECT_ID WndDefectID=WND_DEFECT_BODY_MISSING;	
	const unsigned int FrameIndex = Param.nFrameIndex_3D;
	const unsigned int FrameUniqueID = Param.nFrameUniqueID_3D;		
	const int DefectGroupID = GetModelFreeDefectGroupID(WndDefectID);
	const bool UseExtendBox = CAlgParam::CheckAlgUseExtendBox(AlgType);		
	WND_FOLLOW_MODE WndFollowMode = AOIDataDefine.GetWndFollowModeByWndDefectID(WndDefectID);
	
	if ( ALG_OBJECT_MEASURE == AlgType )
	{
		SizeX = szWnd.cx;
		SizeY = szWnd.cy;
	}
	else
	{
		SizeX = szWnd.cx*RatioX;
		if ( SizeX > MaxSizeX ) { SizeX = MaxSizeX; }
		if ( SizeX < MinSizeX ) { SizeX = MinSizeX; }
		SizeY = szWnd.cy*RatioY;
		if ( SizeY > MaxSizeY ) { SizeY = MaxSizeY; }	
		if ( SizeY < MinSizeY ) { SizeY = MinSizeY; }
		Size = MIN(SizeX, SizeY);	
		SizeX = SizeY = Size;
	}
	RgnWnd.minX = psBody.x-(SizeX/2.0);
	RgnWnd.minY = psBody.y-(SizeY/2.0);	
	RgnWnd.maxX = psBody.x+(SizeX/2.0);
	RgnWnd.maxY = psBody.y+(SizeY/2.0);	

	WndPtr->SetWndToward(Toward);
	WndPtr->SetWndBandID(WndBandID);
	WndPtr->SetWndGroupID(WndGouprID);
	WndPtr->SetWndRgnLinkAuto(true);
	WndPtr->SetWndRgnLinkMode(WND_RGN_LINK_NONE);
	WndPtr->SetWndDefectID(WndDefectID);
	WndPtr->SetWndDefectGroupID(DefectGroupID);
	WndPtr->SetWndFollowMode(WndFollowMode);
	WndPtr->SetWndRegion(RgnWnd);	
	WndPtr->SetWndSelected(true);
	WndPtr->SetWndExtendBoxUsed(UseExtendBox);
	if ( true == UseExtendBox )
	{
		WndPtr->SetWndExtendRangeX(400);
		WndPtr->SetWndExtendRangeY(400);
	}
	WndPtr->UpdateWndExtendBox();
	WndPtr->SetWndAlgType(AlgType);
	
	const double               RangeH=Param.dBodyMissingHeightTolerance;
	CAlgParam                 &AlgParam = WndPtr->GetWndAlgParam();
	CAlgBinaryParam           &BinaryParam = AlgParam.GetAlgImageBinParam();	
	TALG_PARAM_BRIGHT_RATIO   &brParam = AlgParam.GetAlgParamBrightRatio();//演算法-亮度比例參數		
	TALG_PARAM_OBJECT_MEASURE &omParam = AlgParam.GetAlgParamObjectMeasure();//演算法-物件量測參數
	TALG_PARAM_GROUP_COMPARE  &gcParam = AlgParam.GetAlgParamGroupCompare();
	
	AlgParam.CheckAlgPatternFileUsed();		
	BinaryParam.SetBinaryFrameIndex(FrameIndex);
	BinaryParam.SetBinaryFrameUniqueID(FrameUniqueID); 
	BinaryParam.SetBinaryImageSourceMode(IMAGE_SRC_GRAY);		
	BinaryParam.SetRatioThresholdTarget(BodyHeight);
	brParam.brTargetValue = BodyHeight;					

	brParam.brAverageMode = ALG_BRIGHT_AVERAGE_FULL;	
	brParam.brAveragePartialL = 10;
	brParam.brAveragePartialH = 40;

	brParam.brRatioUSL = 150;
	brParam.brRatioLSL =  50;
	brParam.brToleranceUSL =  RangeH;
	brParam.brToleranceLSL = -RangeH;
	if ( FRAME_UNIQUE_ID_DLP==Param.nFrameUniqueID_3D )
	{	
		brParam.brRatioEnabled = true;			
		brParam.brToleranceEnabled = false;
		gcParam.gc3DHeightEnabled = false;//關閉
		gcParam.gc3DHeightBaseMode = ALG_3D_BASE_HEIGHT_AVE;
	}
	else
	{	
		brParam.brRatioEnabled = true;			
		brParam.brToleranceEnabled = false;
	}	
	ApplyModelDefaultBodyWnd(WndPtr, Param);
	InitialModelWndPtr(WndPtr);
	AddModelWndPtr(WndPtr, false);		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd_BodyTiltKernel_v2(const TMODEL_DEFAULT_WND_PARAM &Param)//增加模組檢測框-本體傾斜		
{
	//JetAPI::ShowMessageBox(_T("Error, AddModelDefaultWnd_BodyTiltKernel_v2 Not Ready"));
	//return false;

	bool IsOK = false;	
	switch ( Param.nBodyTiltNum )
	{
	case 2:		IsOK = AddModelDefaultWnd_BodyTiltKernel_v2_2(Param);	break;
	case 4:		IsOK = AddModelDefaultWnd_BodyTiltKernel_v2_4(Param);	break;	
	}
	return IsOK;	
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd_BodyTiltKernel_v2_2(const TMODEL_DEFAULT_WND_PARAM &Param)//增加模組檢測框-本體傾斜		
{
	CAOIWnd *WndPtr = NULL;
	TPOINT2D  psBody, psWnd;
	TSIZE2D   szBody, szWnd;	
	TREGION4D RgnBody, RgnWnd;
	
	GetModelBodyPos(psBody);
	GetModelBodyRegion(RgnBody);
	szBody.cx = RgnBody.maxX-RgnBody.minX;
	szBody.cy = RgnBody.maxY-RgnBody.minY;

	psWnd = psBody;
	szWnd = szBody;

	WndPtr = AOIObjManager.CreateWndObj();
	if ( NULL == WndPtr ) { return false; }	
	
	double SizeU = 0;
	double SizeV = 0;
	double RatioU = 0.30;//0.40;
	double RatioV = 0.15;//0.25;		
	const double GapV = 50;	
	const double Theata=20;//頃斜角度
	const double MaxSizeU = 2500;
	const double MaxSizeV = 1250;		
	const double BodyHeight = GetModelBodyHeight();
	ALG_TYPE   AlgType = ALG_BRIGHT_RATIO;
	BOX_TOWARD Toward = GetModelBodyBox().GetBoxToward();	
	const int WndGouprID = GetModelWndFreeGroupID();
	const int WndBandID = GetModelWndFreeBandID(WndGouprID);
	const WND_DEFECT_ID WndDefectID=WND_DEFECT_BODY_TILT;	
	const int DefectGroupID = GetModelFreeDefectGroupID(WndDefectID);
	const bool UseExtendBox = CAlgParam::CheckAlgUseExtendBox(AlgType);	
	WND_FOLLOW_MODE WndFollowMode = AOIDataDefine.GetWndFollowModeByWndDefectID(WndDefectID);

	switch ( Toward )
	{
	case BOX_TOWARD_UP:
		SizeU = szWnd.cx*RatioU;
		if ( SizeU > MaxSizeU ) { SizeU = MaxSizeU; }
		SizeV = szWnd.cy*RatioV;
		if ( SizeV > MaxSizeV ) { SizeV = MaxSizeV; }		
		
		RgnWnd.maxY = RgnBody.maxY-GapV;
		RgnWnd.minX = psWnd.x-SizeU;
		RgnWnd.maxX = psWnd.x+SizeU;
		RgnWnd.minY = RgnWnd.maxY-(SizeV);
		break;
	case BOX_TOWARD_LEFT:
		SizeU = szWnd.cy*RatioU;
		if ( SizeU > MaxSizeU ) { SizeU = MaxSizeU; }
		SizeV = szWnd.cx*RatioV;
		if ( SizeV > MaxSizeV ) { SizeV = MaxSizeV; }
		RgnWnd.minX = RgnBody.minX+GapV;
		RgnWnd.minY = psWnd.y-(SizeU);
		RgnWnd.maxY = psWnd.y+(SizeU);
		RgnWnd.maxX = RgnWnd.minX+(SizeV);
		break;
	case BOX_TOWARD_DOWN:
		SizeU = szWnd.cx*RatioU;
		if ( SizeU > MaxSizeU ) { SizeU = MaxSizeU; }
		SizeV = szWnd.cy*RatioV;
		if ( SizeV > MaxSizeV ) { SizeV = MaxSizeV; }
		RgnWnd.minY = RgnBody.minY+GapV;
		RgnWnd.minX = psWnd.x-(SizeU);
		RgnWnd.maxX = psWnd.x+(SizeU);
		RgnWnd.maxY = RgnWnd.minY+(SizeV);
		break;
	case BOX_TOWARD_RIGHT:
		SizeU = szWnd.cy*RatioU;
		if ( SizeU > MaxSizeU ) { SizeU = MaxSizeU; }
		SizeV = szWnd.cx*RatioV;
		if ( SizeV > MaxSizeV ) { SizeV = MaxSizeV; }
		RgnWnd.maxX = RgnBody.maxX-GapV;
		RgnWnd.minY = psWnd.y-(SizeU);
		RgnWnd.maxY = psWnd.y+(SizeU);
		RgnWnd.minX = RgnWnd.maxX-(SizeV);
		break;
	}
	
	WndPtr->SetWndToward(Toward);
	WndPtr->SetWndBandID(WndBandID);
	WndPtr->SetWndGroupID(WndGouprID);
	WndPtr->SetWndRgnLinkAuto(true);
	WndPtr->SetWndRgnLinkMode(WND_RGN_LINK_NONE);
	WndPtr->SetWndDefectID(WndDefectID);
	WndPtr->SetWndDefectGroupID(DefectGroupID);
	WndPtr->SetWndFollowMode(WndFollowMode);
	WndPtr->SetWndRegion(RgnWnd);	
	WndPtr->SetWndSelected(true);
	WndPtr->SetWndExtendBoxUsed(UseExtendBox);
	WndPtr->UpdateWndExtendBox();
	WndPtr->SetWndAlgType(AlgType);

	const double               GapH=Param.dBodyTiltHeightTolerance;
	const double               TolGapH=Param.dBodyMissingHeightTolerance;	
	CAlgParam                 &AlgParam = WndPtr->GetWndAlgParam();
	CAlgBinaryParam           &BinaryParam = AlgParam.GetAlgImageBinParam();	
	TALG_PARAM_BRIGHT_RATIO  &brParam = AlgParam.GetAlgParamBrightRatio();//演算法-亮度比例參數		
	TALG_PARAM_GROUP_COMPARE &gcParam = AlgParam.GetAlgParamGroupCompare();

	AlgParam.CheckAlgPatternFileUsed();
	brParam.brRatioUSL = 125;
	brParam.brRatioLSL =  75;
	brParam.brToleranceUSL =  TolGapH;
	brParam.brToleranceLSL = -TolGapH;
	if ( FRAME_UNIQUE_ID_DLP==Param.nFrameUniqueID_3D )
	{	
		unsigned int FrameIndex = Param.nFrameIndex_3D;
		unsigned int FrameUniqueID = Param.nFrameUniqueID_3D;		
		BinaryParam.SetBinaryFrameIndex(FrameIndex);
		BinaryParam.SetBinaryFrameUniqueID(FrameUniqueID); 
		BinaryParam.SetBinaryImageSourceMode(IMAGE_SRC_GRAY);
		BinaryParam.SetRatioThresholdTarget(BodyHeight);

		brParam.brTargetValue = BodyHeight;		
		brParam.brRatioEnabled = false;			
		brParam.brToleranceEnabled = false;
		gcParam.gc3DHeightEnabled = true;
		gcParam.gc3DHeightBaseMode = ALG_3D_BASE_HEIGHT_AVE;
		gcParam.gc3DHeightUSL    =  GapH*0.5;
		gcParam.gc3DHeightLSL    = -GapH*0.5;
	}
	else
	{	
		brParam.brRatioEnabled = true;			
		brParam.brToleranceEnabled = false;
	}

	ApplyModelDefaultBodyWnd(WndPtr, Param);
	InitialModelWndPtr(WndPtr);
	AddModelWndPtr(WndPtr, false);	

	//Add Other one
	CAOIWnd *WndPtr2 = CAOIModel::CopyModelWnd(WndPtr);
	if ( NULL != WndPtr2 )
	{
		WndPtr2->SetWndSelected(false);
		switch ( Toward )
		{
		case BOX_TOWARD_UP:
		case BOX_TOWARD_DOWN:
			WndPtr2->MirrorWndXAxis(psBody.y);
			break;
		case BOX_TOWARD_LEFT:
		case BOX_TOWARD_RIGHT:
			WndPtr2->MirrorWndYAxis(psBody.x);
			break;
		}
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd_BodyTiltKernel_v2_4(const TMODEL_DEFAULT_WND_PARAM &Param)//增加模組檢測框-本體傾斜		
{
	CAOIWnd *WndPtr = NULL;
	TPOINT2D  psBody, psWnd;
	TSIZE2D   szBody, szWnd;	
	TREGION4D RgnBody, RgnWnd;
	
	GetModelBodyPos(psBody);
	GetModelBodyRegion(RgnBody);
	szBody.cx = RgnBody.maxX-RgnBody.minX;
	szBody.cy = RgnBody.maxY-RgnBody.minY;

	psWnd = psBody;
	szWnd = szBody;

	WndPtr = AOIObjManager.CreateWndObj();
	if ( NULL == WndPtr ) { return false; }

	double SizeU = 0;
	double SizeV = 0;
	double RatioU = 0.30;//0.40;
	double RatioV = 0.15;//0.25;	
	const double GapU = 50;
	const double GapV = 50;
	const double MaxSizeU = 1000;
	const double MaxSizeV = 1000;
	const double BodyHeight = GetModelBodyHeight();
	ALG_TYPE   AlgType = ALG_BRIGHT_RATIO;
	BOX_TOWARD Toward = GetModelBodyBox().GetBoxToward();	
	const int WndGouprID = GetModelWndFreeGroupID();
	const int WndBandID = GetModelWndFreeBandID(WndGouprID);
	const WND_DEFECT_ID WndDefectID=WND_DEFECT_BODY_TILT;	
	const int DefectGroupID = GetModelFreeDefectGroupID(WndDefectID);
	const bool UseExtendBox = CAlgParam::CheckAlgUseExtendBox(AlgType);
	WND_FOLLOW_MODE WndFollowMode = AOIDataDefine.GetWndFollowModeByWndDefectID(WndDefectID);

	switch ( Toward )
	{
	case BOX_TOWARD_UP:
		SizeU = szWnd.cx*RatioU;
		if ( SizeU > MaxSizeU ) { SizeU = MaxSizeU; }
		SizeV = szWnd.cy*RatioV;
		if ( SizeV > MaxSizeV ) { SizeV = MaxSizeV; }	

		RgnWnd.maxY = RgnBody.maxY-(GapV);
		RgnWnd.maxX = RgnBody.maxX-(GapU);
		RgnWnd.minX = RgnWnd.maxX-(SizeU);		
		RgnWnd.minY = RgnWnd.maxY-(SizeV);
		break;
	case BOX_TOWARD_LEFT:
		SizeU = szWnd.cy*RatioU;
		if ( SizeU > MaxSizeU ) { SizeU = MaxSizeU; }
		SizeV = szWnd.cx*RatioV;
		if ( SizeV > MaxSizeV ) { SizeV = MaxSizeV; }

		RgnWnd.minX = RgnBody.minX+(GapV);
		RgnWnd.maxY = RgnBody.maxY-(GapU);
		RgnWnd.minY = RgnWnd.maxY-(SizeU);
		RgnWnd.maxX = RgnWnd.minX+(SizeV);
		break;
	case BOX_TOWARD_DOWN:
		SizeU = szWnd.cx*RatioU;
		if ( SizeU > MaxSizeU ) { SizeU = MaxSizeU; }
		SizeV = szWnd.cy*RatioV;
		if ( SizeV > MaxSizeV ) { SizeV = MaxSizeV; }

		RgnWnd.minY = RgnBody.minY+(GapV);
		RgnWnd.minX = RgnBody.minX+(GapU);
		RgnWnd.maxX = RgnWnd.minX+(SizeU);
		RgnWnd.maxY = RgnWnd.minY+(SizeV);
		break;
	case BOX_TOWARD_RIGHT:
		SizeU = szWnd.cy*RatioU;
		if ( SizeU > MaxSizeU ) { SizeU = MaxSizeU; }
		SizeV = szWnd.cx*RatioV;
		if ( SizeV > MaxSizeV ) { SizeV = MaxSizeV; }

		RgnWnd.maxX = RgnBody.maxX-(GapV);
		RgnWnd.minY = RgnBody.minY+(GapU);
		RgnWnd.maxY = RgnWnd.minY+(SizeU);
		RgnWnd.minX = RgnWnd.maxX-(SizeV);
		break;
	}
	WndPtr->SetWndToward(Toward);
	WndPtr->SetWndBandID(WndBandID);
	WndPtr->SetWndGroupID(WndGouprID);
	WndPtr->SetWndRgnLinkAuto(true);
	WndPtr->SetWndRgnLinkMode(WND_RGN_LINK_NONE);
	WndPtr->SetWndDefectID(WndDefectID);
	WndPtr->SetWndDefectGroupID(DefectGroupID);
	WndPtr->SetWndFollowMode(WndFollowMode);
	WndPtr->SetWndRegion(RgnWnd);	
	WndPtr->SetWndSelected(true);
	WndPtr->SetWndExtendBoxUsed(UseExtendBox);
	WndPtr->UpdateWndExtendBox();
	WndPtr->SetWndAlgType(AlgType);

	const double              GapH=Param.dBodyTiltHeightTolerance;
	const double              TolGapH=Param.dBodyMissingHeightTolerance;
	CAlgParam                &AlgParam = WndPtr->GetWndAlgParam();
	CAlgBinaryParam          &BinaryParam = AlgParam.GetAlgImageBinParam();	
	TALG_PARAM_BRIGHT_RATIO  &brParam = AlgParam.GetAlgParamBrightRatio();//演算法-亮度比例參數		
	TALG_PARAM_GROUP_COMPARE &gcParam = AlgParam.GetAlgParamGroupCompare();

	AlgParam.CheckAlgPatternFileUsed();	
	brParam.brRatioUSL = 125;
	brParam.brRatioLSL =  75;
	brParam.brToleranceUSL =  TolGapH;
	brParam.brToleranceLSL = -TolGapH;		
	if ( FRAME_UNIQUE_ID_DLP==Param.nFrameUniqueID_3D )
	{	
		unsigned int FrameIndex = Param.nFrameIndex_3D;
		unsigned int FrameUniqueID = Param.nFrameUniqueID_3D;		
		BinaryParam.SetBinaryFrameIndex(FrameIndex);
		BinaryParam.SetBinaryFrameUniqueID(FrameUniqueID); 
		BinaryParam.SetBinaryImageSourceMode(IMAGE_SRC_GRAY);
		BinaryParam.SetRatioThresholdTarget(BodyHeight);

		brParam.brTargetValue = BodyHeight;
		brParam.brRatioEnabled = false;			
		brParam.brToleranceEnabled = false;
		gcParam.gc3DHeightEnabled = true;
		gcParam.gc3DHeightBaseMode = ALG_3D_BASE_HEIGHT_AVE;		
		gcParam.gc3DHeightUSL    =  GapH/2;
		gcParam.gc3DHeightLSL    = -GapH/2;
	}
	else
	{	
		brParam.brRatioEnabled = true;			
		brParam.brToleranceEnabled = false;
	}	

	ApplyModelDefaultBodyWnd(WndPtr, Param);
	InitialModelWndPtr(WndPtr);
	AddModelWndPtr(WndPtr, false);	

	//return true;//delete every time, so only create one wnd 
	//Add Other one	
	CAOIWnd *WndPtr2 = CAOIModel::CopyModelWnd(WndPtr);
	if ( NULL != WndPtr2 )
	{
		WndPtr2->SetWndSelected(false);
		switch ( Toward )
		{
		case BOX_TOWARD_UP:
		case BOX_TOWARD_DOWN:
			WndPtr2->MirrorWndYAxis(psBody.x);
			break;
		case BOX_TOWARD_LEFT:
		case BOX_TOWARD_RIGHT:
			WndPtr2->MirrorWndXAxis(psBody.y);
			break;
		}
	}

	CAOIWnd *WndPtr3 = CAOIModel::CopyModelWnd(WndPtr);
	if ( NULL != WndPtr3 )
	{
		WndPtr3->SetWndSelected(false);
		switch ( Toward )
		{
		case BOX_TOWARD_UP:
		case BOX_TOWARD_DOWN:
			WndPtr3->MirrorWndXAxis(psBody.y);
			break;
		case BOX_TOWARD_LEFT:
		case BOX_TOWARD_RIGHT:
			WndPtr3->MirrorWndYAxis(psBody.x);
			break;
		}
	}
	CAOIWnd *WndPtr4 = CAOIModel::CopyModelWnd(WndPtr2);
	if ( NULL != WndPtr4 )
	{
		WndPtr4->SetWndSelected(false);
		switch ( Toward )
		{
		case BOX_TOWARD_UP:
		case BOX_TOWARD_DOWN:
			WndPtr4->MirrorWndXAxis(psBody.y);
			break;
		case BOX_TOWARD_LEFT:
		case BOX_TOWARD_RIGHT:
			WndPtr4->MirrorWndYAxis(psBody.x);
			break;
		}
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd_BodyMountKernel_v2(const TMODEL_DEFAULT_WND_PARAM &Param)//增加模組檢測框-本體裝貼
{
	CAOIWnd *WndPtr = NULL;
	TPOINT2D  psBody, psWnd;
	TSIZE2D   szBody, szWnd;	
	TREGION4D RgnBody, RgnWnd;
	
	GetModelBodyPos(psBody);
	GetModelBodyRegion(RgnBody);
	CalcModelBodyRegionNoLead(RgnBody);
	psBody.x = RgnBody.GetCpX();
	psBody.y = RgnBody.GetCpY();
	szBody.cx = RgnBody.GetWidth();
	szBody.cy = RgnBody.GetHeight();

	psWnd = psBody;
	szWnd = szBody;

	WndPtr = AOIObjManager.CreateWndObj();
	if ( NULL == WndPtr ) { return false; }

	RgnWnd.minX = psWnd.x - (szWnd.cx*0.25);//*0.4
	RgnWnd.minY = psWnd.y - (szWnd.cy*0.25);//*0.4
	RgnWnd.maxX = psWnd.x + (szWnd.cx*0.25);//*0.4
	RgnWnd.maxY = psWnd.y + (szWnd.cy*0.25);//*0.4

	ALG_TYPE   AlgType = ALG_BRIGHT_RATIO;
	BOX_TOWARD Toward = GetModelBodyBox().GetBoxToward();	
	const int WndGouprID = GetModelWndFreeGroupID();
	const int WndBandID = GetModelWndFreeBandID(WndGouprID);
	const WND_DEFECT_ID WndDefectID=WND_DEFECT_BODY_MOUNT;
	const int DefectGroupID = GetModelFreeDefectGroupID(WndDefectID);
	const bool UseExtendBox = CAlgParam::CheckAlgUseExtendBox(AlgType);
	const CColorGroup &BodyColorGroup = GetModelBodyColorGroup();
	const unsigned int FrameIndex = Param.nFrameIndex_Text;
	const unsigned int FrameUniqueID = Param.nFrameUniqueID_Text;
	WND_FOLLOW_MODE WndFollowMode = AOIDataDefine.GetWndFollowModeByWndDefectID(WndDefectID);	

	WndPtr->SetWndToward(Toward);
	WndPtr->SetWndBandID(WndBandID);
	WndPtr->SetWndGroupID(WndGouprID);
	WndPtr->SetWndRgnLinkAuto(false);
	WndPtr->SetWndRgnLinkMode(WND_RGN_LINK_NONE);
	WndPtr->SetWndDefectID(WndDefectID);
	WndPtr->SetWndDefectGroupID(DefectGroupID);
	WndPtr->SetWndFollowMode(WndFollowMode);
	WndPtr->SetWndRegion(RgnWnd);	
	WndPtr->SetWndSelected(true);
	WndPtr->SetWndExtendBoxUsed(UseExtendBox);
	WndPtr->UpdateWndExtendBox();	
	WndPtr->SetWndAlgType(AlgType);

	CAlgParam                 &AlgParam = WndPtr->GetWndAlgParam();
	CAlgBinaryParam           &BinaryParam = AlgParam.GetAlgImageBinParam();	
	AlgParam.CheckAlgPatternFileUsed();	
	
	BinaryParam.SetBinaryFrameIndex(FrameIndex);
	BinaryParam.SetBinaryFrameUniqueID(FrameUniqueID);		
	BinaryParam.SetBinaryMode(BINARY_FIXED_THRESHOLD);	
	switch ( Param.eModelType )
	{
	case MODEL_TYPE_CHIP_C:
	case MODEL_TYPE_CHIP_LED:
	case MODEL_TYPE_LED_ARRAY:
	case MODEL_TYPE_CAPACITY_ARRAY:
		BinaryParam.SetFixedThresholdLow(20);
		BinaryParam.SetFixedThresholdHigh(255);
		break;
	default:
		BinaryParam.SetFixedThresholdLow(0);
		BinaryParam.SetFixedThresholdHigh(20);
		break;
	}	

	TALG_PARAM_BRIGHT_RATIO   &brParam = AlgParam.GetAlgParamBrightRatio();//演算法-亮度比例參數		
	brParam.brRatioUSL = 100;
	brParam.brRatioLSL = 50;
	brParam.brRatioEnabled = true;
	brParam.brToleranceEnabled = false;	

	ApplyModelDefaultBodyWnd(WndPtr, Param);
	InitialModelWndPtr(WndPtr);
	AddModelWndPtr(WndPtr, false);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd_BodyDamagedKernel_v2(const TMODEL_DEFAULT_WND_PARAM &Param)//增加模組檢測框-破損檢測	
{
	CAOIWnd *WndPtr = NULL;
	TPOINT2D  psBody, psWnd;
	TSIZE2D   szBody, szWnd;	
	TREGION4D RgnBody, RgnWnd;	
	
	GetModelBodyPos(psBody);
	GetModelBodyRegion(RgnBody);
	CalcModelBodyRegionNoLead(RgnBody);
	psBody.x = RgnBody.GetCpX();
	psBody.y = RgnBody.GetCpY();
	szBody.cx = RgnBody.GetWidth();
	szBody.cy = RgnBody.GetHeight();

	psWnd = psBody;
	szWnd = szBody;
	
	const double ScaleX=0.95;
	const double ScaleY=0.95;
	const double GapX=MIN(100, szWnd.cx*(1.0-ScaleX));
	const double GapY=MIN(100, szWnd.cy*(1.0-ScaleY));

	WndPtr = AOIObjManager.CreateWndObj();
	if ( NULL == WndPtr ) { return false; }

	RgnWnd.minX = psWnd.x - (szWnd.cx*0.50) + GapX;//*0.5
	RgnWnd.minY = psWnd.y - (szWnd.cy*0.50) + GapY;//*0.5
	RgnWnd.maxX = psWnd.x + (szWnd.cx*0.50) - GapX;//*0.5
	RgnWnd.maxY = psWnd.y + (szWnd.cy*0.50) - GapY;//*0.5

	ALG_TYPE   AlgType = ALG_BLOB_COUNT;
	BOX_TOWARD Toward = GetModelBodyBox().GetBoxToward();	
	const int WndGouprID = GetModelWndFreeGroupID();
	const int WndBandID = GetModelWndFreeBandID(WndGouprID);
	const WND_DEFECT_ID WndDefectID=WND_DEFECT_BODY_DAMAGED;
	const int DefectGroupID = GetModelFreeDefectGroupID(WndDefectID);
	const bool UseExtendBox = CAlgParam::CheckAlgUseExtendBox(AlgType);	
	WND_FOLLOW_MODE WndFollowMode = AOIDataDefine.GetWndFollowModeByWndDefectID(WndDefectID);	
	unsigned int FrameIndex = Param.nFrameIndex_3D;
	unsigned int FrameUniqueID = Param.nFrameUniqueID_3D;	

	WndPtr->SetWndToward(Toward);
	WndPtr->SetWndBandID(WndBandID);
	WndPtr->SetWndGroupID(WndGouprID);
	WndPtr->SetWndRgnLinkAuto(false);
	WndPtr->SetWndRgnLinkMode(WND_RGN_LINK_NONE);
	WndPtr->SetWndRgnLinkRatioX(ScaleX*100.0);
	WndPtr->SetWndRgnLinkRatioX(ScaleY*100.0);
	WndPtr->SetWndDefectID(WndDefectID);
	WndPtr->SetWndDefectGroupID(DefectGroupID);
	WndPtr->SetWndFollowMode(WndFollowMode);
	WndPtr->SetWndRegion(RgnWnd);	
	WndPtr->SetWndSelected(true);
	WndPtr->SetWndExtendBoxUsed(UseExtendBox);
	WndPtr->UpdateWndExtendBox();	
	WndPtr->SetWndAlgType(AlgType);

	CAlgParam                 &AlgParam = WndPtr->GetWndAlgParam();
	CAlgBinaryParam           &BinaryParam = AlgParam.GetAlgImageBinParam();	
	AlgParam.CheckAlgPatternFileUsed();	
	BinaryParam.SetBinaryInvert(true);
	BinaryParam.SetBinaryFrameIndex(FrameIndex);
	BinaryParam.SetBinaryFrameUniqueID(FrameUniqueID);	
	BinaryParam.SetBinaryMode(BINARY_RELATIVE_AVE_THRESHOLD);
	BinaryParam.SetRelativeAveThresholdAbove(50.0);
	BinaryParam.SetRelativeAveThresholdBelow(50.0);

	TALG_PARAM_BLOB_COUNT   &blobParam = AlgParam.GetAlgParamBlobCount();
	blobParam.bcXSizeMin = 50;
	blobParam.bcXSizeMinEnabled = true;
	blobParam.bcYSizeMin = 50;
	blobParam.bcYSizeMinEnabled = true;
	blobParam.bcAreaSizeMin = 10000;
	blobParam.bcCountUSL = 0;
	blobParam.bcCountLSL = 0;

	ApplyModelDefaultBodyWnd(WndPtr, Param);
	InitialModelWndPtr(WndPtr);
	AddModelWndPtr(WndPtr, false);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd_OuterShortKernelFn_v2(const TMODEL_DEFAULT_WND_PARAM &Param, bool bUse3D)//增加模組檢測框-外接短路檢測
{
	CAOIWnd *WndPtr = NULL;
	TPOINT2D  psBody, psWnd, psPad;
	TSIZE2D   szBody, szWnd, szPad;	
	TREGION4D RgnBody, RgnWnd, RgnPad;
	
	GetModelBodyPos(psBody);
	GetModelBodyRegion(RgnBody);
	GetModelPadRegion(-1, -1, RgnPad);
	szBody.cx = RgnBody.maxX-RgnBody.minX;
	szBody.cy = RgnBody.maxY-RgnBody.minY;
	szPad.cx = RgnPad.maxX-RgnPad.minX;
	szPad.cy = RgnPad.maxY-RgnPad.minY;	

	psWnd = psBody;
	szWnd = szBody;

	psWnd = psPad;
	szWnd = szPad;

	WndPtr = AOIObjManager.CreateWndObj();
	if ( NULL == WndPtr ) { return false; }

	RgnWnd.minX = psWnd.x - (szWnd.cx*0.5);
	RgnWnd.minY = psWnd.y - (szWnd.cy*0.5);
	RgnWnd.maxX = psWnd.x + (szWnd.cx*0.5);
	RgnWnd.maxY = psWnd.y + (szWnd.cy*0.5);	
	const double ExtX = Param.dBridgeExtendRange;
	const double ExtY = Param.dBridgeExtendRange;	
	const bool   bBridgeTwoSide=Param.bBridgeTwoSide;//雙邊增加檢測框

	ALG_TYPE   AlgType = ALG_OUTER_SHORT;
	BOX_TOWARD Toward = GetModelBodyBox().GetBoxToward();	
	const int WndGouprID = GetModelWndFreeGroupID();
	const int WndBandID = GetModelWndFreeBandID(WndGouprID);
	const WND_DEFECT_ID WndDefectID=WND_DEFECT_SOLDER_BRIDGE;
	const int DefectGroupID = GetModelFreeDefectGroupID(WndDefectID);
	const bool UseExtendBox = CAlgParam::CheckAlgUseExtendBox(AlgType);	
	WND_FOLLOW_MODE WndFollowMode = AOIDataDefine.GetWndFollowModeByWndDefectID(WndDefectID);
	unsigned int FrameIndex = Param.nFrameIndex_Low;
	unsigned int FrameUniqueID = Param.nFrameUniqueID_Low;	
	int          FixedThreshold = 128;

	if ( true == bUse3D )
	{
		FrameIndex = Param.nFrameIndex_3D;
		FrameUniqueID = Param.nFrameUniqueID_3D;	
	}

	WndPtr->SetWndToward(Toward);
	WndPtr->SetWndBandID(WndBandID);
	WndPtr->SetWndGroupID(WndGouprID);
	WndPtr->SetWndRgnLinkAuto(true);
	WndPtr->SetWndRgnLinkMode(WND_RGN_LINK_PAD_BODY_RGN);//WND_RGN_LINK_NONE, WND_RGN_LINK_PAD_RGN
	WndPtr->SetWndDefectID(WndDefectID);	
	WndPtr->SetWndDefectGroupID(DefectGroupID);
	if ( true == bUse3D )
	{
		if ( CheckModelTypeUseBody(Param.eModelType) == true )
		{	WndPtr->SetWndFollowMode(WND_FOLLOW_PART);	}
		else
		{	WndPtr->SetWndFollowMode(WND_FOLLOW_PAD);	}
	}
	else
	{	WndPtr->SetWndFollowMode(WndFollowMode);	}
	WndPtr->SetWndRegion(RgnWnd);		
	WndPtr->SetWndSelected(true);
	WndPtr->SetWndExtendRangeX(ExtX);
	WndPtr->SetWndExtendRangeY(ExtY);
	WndPtr->SetWndExtendBoxUsed(UseExtendBox);
	WndPtr->UpdateWndExtendBox();
	WndPtr->SetWndAlgType(AlgType);

	CAlgParam                 &AlgParam = WndPtr->GetWndAlgParam();
	CAlgBinaryParam           &BinaryParam = AlgParam.GetAlgImageBinParam();

	AlgParam.CheckAlgPatternFileUsed();
	if ( true == bBridgeTwoSide )
	{
		ALG_OUTER_SHORT_EXT_MODE ExtMode=ALG_OUTER_SHORT_EXT_BOTH;
		AlgParam.GetAlgParamOuterShort().osLine_R.olExtMode=ExtMode;
		AlgParam.GetAlgParamOuterShort().osLine_T.olExtMode=ExtMode;
		AlgParam.GetAlgParamOuterShort().osLine_L.olExtMode=ExtMode;
		AlgParam.GetAlgParamOuterShort().osLine_B.olExtMode=ExtMode;
	}
	BinaryParam.SetBinaryFrameIndex(FrameIndex);
	BinaryParam.SetBinaryFrameUniqueID(FrameUniqueID);	
	BinaryParam.SetBinaryMode(BINARY_FIXED_THRESHOLD);
	if ( FRAME_UNIQUE_ID_DLP==FrameUniqueID )
	{
		BinaryParam.SetFixedThresholdLow(Param.nBridgeFixThresholdL3D);	
		BinaryParam.SetFixedThresholdHigh(FIXED_THRESHOLD_MAX_3D);
		BinaryParam.SetBinaryImageSourceMode(IMAGE_SRC_GRAY);
	}
	else
	{			
		FixedThreshold = Param.nBridgeFixThresholdL2D;
		BinaryParam.SetFixedThresholdLow(FixedThreshold);	 
		BinaryParam.SetFixedThresholdHigh(FIXED_THRESHOLD_MAX_2D);
		BinaryParam.SetBinaryImageSourceMode(IMAGE_SRC_LIGHTNESS);
	}

	TBINARY_FILTER BinaryFilter;
	BinaryFilter.FilterMode = NOISE_FILTER_OPEN;
	BinaryFilter.FilterParam1 = 3;
	BinaryParam.SetBinaryNoiseFilter1(BinaryFilter);

	ApplyModelDefaultBodyWnd(WndPtr, Param);
	InitialModelWndPtr(WndPtr);
	AddModelWndPtr(WndPtr, false);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd_SolderOpenKernel_v2(const TMODEL_DEFAULT_WND_PARAM &Param, int RefLandGroupID)//增加模組檢測框-空焊檢測
{	
	TMODEL_DEFAULT_WND_PARAM Param2=Param;
	if ( true == Param2.bSolderOpenUseSideWnd )
	{	Param2.bUseLogic = true; }
	else
	{	Param2.bUseLogic = false; }
	if ( AddModelDefaultWnd_SolderOpenKernel_v2(Param2, RefLandGroupID, false) == false )
	{	return false; }
	if ( true == Param2.bSolderOpenUseSideWnd )
	{
		if ( AddModelDefaultWnd_SolderOpenKernel_v2(Param2, RefLandGroupID, true) == false )
		{	return false; }	
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd_SolderOpenKernel_v2(const TMODEL_DEFAULT_WND_PARAM &Param, int RefLandGroupID, bool bUseSide)//增加模組檢測框-空焊檢測
{
	int          k=0;
	size_t       i=0, j=0;
	int          WndBandID = 0;
	int          WndGouprID = 0;
	int          LandGroupID = 0;
	CAOIWnd     *WndPtr = NULL;
	CAOILand    *LandPtr = NULL;
	TREGION4D    RgnWnd, RgnLead, RgnPad, RgnBody, RgnPadInner;
	BOX_TOWARD   LandToward = BOX_TOWARD_NULL;	
	double       GapU=50;
	double       GapV=50;
	double       SizeU=0;
	double       SizeV=0;	
	double       StartV=0;	
	
	ALG_TYPE     AlgType = ALG_BRIGHT_RATIO;
	LAND_TYPE    LandType=LAND_TYPE_NULL;		
	BOX_TOWARD   BodyToward = GetModelBodyBox().GetBoxToward();		
	const int    MaxLandGroupID = GetModelLandFreeGroupID();
	const size_t LandCount = GetModelLandCount();	
	const WND_DEFECT_ID WndDefectID=WND_DEFECT_SOLDER_OPEN;
	const int DefectGroupID = GetModelFreeDefectGroupID(WndDefectID);
	const bool UseExtendBox = CAlgParam::CheckAlgUseExtendBox(AlgType);
	const WND_SYNC_MOVE_MODE WndSyncMoveMode = GetModelWndSyncMoveMode();
	WND_FOLLOW_MODE WndFollowMode = AOIDataDefine.GetWndFollowModeByWndDefectID(WndDefectID);	
	unsigned int FrameIndex = Param.nFrameIndex_Solder;
	unsigned int FrameUniqueID = Param.nFrameUniqueID_Solder;	
	CAOIWnd   *RefWndPtr = NULL;
	CAOIModel *RefModelPtr = (CAOIModel*)(Param.pModel);		
	
	GetModelBodyRegion(RgnBody);
	for ( k=0; k<MaxLandGroupID; k ++ )
	{
		if ( RefLandGroupID >= 0 ) 
		{
			if ( RefLandGroupID != k ) { continue; }
		}

		LandGroupID = k;		
		WndGouprID = GetModelWndFreeGroupID();
		WndBandID = GetModelWndFreeBandID(WndGouprID);
		for ( i=0; i<LandCount; i++ )
		{
			LandPtr = GetModelLandPtr(i, false);
			if ( NULL == LandPtr ) { continue; }			
			if ( LandPtr->GetLandGroupID() != LandGroupID ) { continue; }
			LandType   = LandPtr->GetLandType();
			LandToward = LandPtr->GetLandToward();
			LandPtr->GetLandPadBox().GetBoxRegion(RgnPad);
			LandPtr->GetLandLeadBox().GetBoxRegion(RgnLead);
			RgnPadInner = RgnPad;
			RgnPadInner.minX += 10;
			RgnPadInner.minY += 10;
			RgnPadInner.maxX -= 10;
			RgnPadInner.maxY -= 10;

			if ( true == bUseSide )
			{
				TPOINT2D PadPos(RgnPad.GetCpX(), RgnPad.GetCpY());
				switch ( LandToward )
				{
				case BOX_TOWARD_UP:
				case BOX_TOWARD_DOWN:
					if ( PadPos.x > 0 ) { LandToward = BOX_TOWARD_RIGHT; }
					else { LandToward = BOX_TOWARD_LEFT; }
					break;
				case BOX_TOWARD_LEFT:
				case BOX_TOWARD_RIGHT:
					if ( PadPos.y > 0 ) { LandToward = BOX_TOWARD_UP; }
					else { LandToward = BOX_TOWARD_DOWN; }
					break;
				}
			}

			if ( LAND_TYPE_PAD == LandType )
			{				
				switch ( LandToward )
				{
				case BOX_TOWARD_UP:					
					if ( RgnBody.maxY>RgnPad.minY && RgnBody.maxY<RgnPad.maxY )					
					{	RgnWnd.minY = RgnBody.maxY+(GapV); }
					else
					{	RgnWnd.minY = RgnPad.minY+(GapV); }
					RgnWnd.maxY = RgnPad.maxY-(GapV);					
					RgnWnd.maxX = RgnPad.maxX-GapU;
					RgnWnd.minX = RgnPad.minX+GapU;
					if ( RgnWnd.maxY < RgnWnd.minY )
					{	RgnWnd.maxY = RgnWnd.minY+GapV; }
					break;
				case BOX_TOWARD_LEFT:
					if ( RgnBody.minX<RgnPad.maxX && RgnBody.minX>RgnPad.minX )
					{	RgnWnd.maxX = RgnBody.minX-(GapV); }
					else
					{	RgnWnd.maxX = RgnPad.maxX-(GapV); }
					RgnWnd.minX = RgnPad.minX+(GapV);
					RgnWnd.maxY = RgnPad.maxY-GapU;
					RgnWnd.minY = RgnPad.minY+GapU;
					if ( RgnWnd.minX > RgnWnd.maxX )
					{	RgnWnd.minX = RgnWnd.maxX-GapV; }
					break;
				case BOX_TOWARD_DOWN:					
					if ( RgnBody.minY<RgnPad.maxY && RgnBody.minY>RgnPad.minY )					
					{	RgnWnd.maxY = RgnBody.minY-(GapV); }
					else
					{	RgnWnd.maxY = RgnPad.maxY-(GapV); }					
					RgnWnd.minY = RgnPad.minY+(GapV);
					RgnWnd.maxX = RgnPad.maxX-GapU;
					RgnWnd.minX = RgnPad.minX+GapU;
					if ( RgnWnd.minY > RgnWnd.maxY )
					{	RgnWnd.minY = RgnWnd.maxY-GapV; }
					break;
				case BOX_TOWARD_RIGHT:
					if ( RgnBody.maxX>RgnPad.minX && RgnBody.maxX<RgnPad.maxX )
					{	RgnWnd.minX = RgnBody.maxX+(GapV); }
					else
					{	RgnWnd.minX = RgnPad.minX+(GapV); }
					RgnWnd.maxX = RgnPad.maxX-(GapV);					
					RgnWnd.maxY = RgnPad.maxY-GapU;
					RgnWnd.minY = RgnPad.minY+GapU;
					if ( RgnWnd.maxX < RgnWnd.minX )
					{	RgnWnd.maxX = RgnWnd.minX+GapV; }
					break;
				}				
				//JetAPI::ScaleRegion(RgnPad, 0.5, 0.5, SCALE_REGION_BY_CENTER, RgnWnd);
			}
			else if ( LAND_TYPE_ELECTRODE == LandType )
			{					
				switch ( LandToward )
				{
				case BOX_TOWARD_UP:
					GapU = RgnLead.GetWidth()*0.3;
					GapV = 10;//RgnPad.GetHeight()*0.25;
					SizeV = RgnLead.GetHeight()*0.6;
					StartV = MAX(RgnPad.minY, RgnLead.maxY);
					StartV = MAX(StartV, RgnBody.maxY);

					RgnWnd.minY = StartV+(GapV);
					RgnWnd.maxY = RgnWnd.minY+(SizeV);
					RgnWnd.maxX = RgnLead.maxX-GapU;
					RgnWnd.minX = RgnLead.minX+GapU;
					break;
				case BOX_TOWARD_LEFT:
					GapU = RgnLead.GetHeight()*0.3;
					GapV = 10;//RgnPad.GetWidth()*0.25;
					SizeV = RgnLead.GetWidth()*0.6;
					StartV = MIN(RgnPad.maxX, RgnLead.minX);
					StartV = MIN(StartV, RgnBody.minX);

					RgnWnd.maxX = StartV-(GapV);
					RgnWnd.minX = RgnWnd.maxX-(SizeV);
					RgnWnd.maxY = RgnLead.maxY-GapU;
					RgnWnd.minY = RgnLead.minY+GapU;
					break;
				case BOX_TOWARD_DOWN:
					GapU = RgnLead.GetWidth()*0.3;
					GapV = 10;//RgnPad.GetHeight()*0.25;
					SizeV = RgnLead.GetHeight()*0.6;
					StartV = MIN(RgnPad.maxY, RgnLead.minY);
					StartV = MIN(StartV, RgnBody.minY);
					
					RgnWnd.maxY = StartV-(GapV);
					RgnWnd.minY = RgnWnd.maxY-(SizeV);
					RgnWnd.maxX = RgnLead.maxX-GapU;
					RgnWnd.minX = RgnLead.minX+GapU;
					break;
				case BOX_TOWARD_RIGHT:
					GapU = RgnLead.GetHeight()*0.3;
					GapV = 10;//RgnPad.GetWidth()*0.25;
					SizeV = RgnLead.GetWidth()*0.6;
					StartV = MAX(RgnPad.minX, RgnLead.maxX);
					StartV = MAX(StartV, RgnBody.maxX);

					RgnWnd.minX = StartV+(GapV);
					RgnWnd.maxX = RgnWnd.minX+(SizeV);
					RgnWnd.maxY = RgnLead.maxY-GapU;
					RgnWnd.minY = RgnLead.minY+GapU;
					break;
				}
				JetAPI::BoundaryRegion(RgnPad, RgnWnd);
				//JetAPI::ScaleRegion(RgnPad, 0.4, 0.4, SCALE_REGION_BY_CENTER, RgnWnd);
			}
			else if ( LAND_TYPE_DIP_LEAD == LandType )
			{	
				JetAPI::ScaleRegion(RgnPad, 0.4, 0.4, SCALE_REGION_BY_CENTER, RgnWnd);
			}
			else//if ( LAND_TYPE_IC_LEAD/LAND_TYPE_CON_LEAD == LandType )			
			{
				GapU=00;
				GapV=20;	
				SizeV = 100;
				switch ( LandToward )
				{
				case BOX_TOWARD_UP:
					RgnWnd.minY = RgnLead.maxY+(GapV);					
					RgnWnd.maxY = RgnWnd.minY+(SizeV);
					RgnWnd.maxX = RgnLead.maxX-GapU;
					RgnWnd.minX = RgnLead.minX+GapU;
					if ( RgnWnd.minX < RgnPadInner.minX ) { RgnWnd.minX = RgnPadInner.minX; }
					if ( RgnWnd.maxX > RgnPadInner.maxX ) { RgnWnd.maxX = RgnPadInner.maxX; }
					break;
				case BOX_TOWARD_LEFT:					
					RgnWnd.maxX = RgnLead.minX-(GapV);
					RgnWnd.minX = RgnWnd.maxX-(SizeV);
					RgnWnd.maxY = RgnLead.maxY-GapU;
					RgnWnd.minY = RgnLead.minY+GapU;
					if ( RgnWnd.minY < RgnPadInner.minY ) { RgnWnd.minY = RgnPadInner.minY; }
					if ( RgnWnd.maxY > RgnPadInner.maxY ) { RgnWnd.maxY = RgnPadInner.maxY; }
					break;
				case BOX_TOWARD_DOWN:					
					RgnWnd.maxY = RgnLead.minY-(GapV);
					RgnWnd.minY = RgnWnd.maxY-(SizeV);
					RgnWnd.maxX = RgnLead.maxX-GapU;
					RgnWnd.minX = RgnLead.minX+GapU;
					if ( RgnWnd.minX < RgnPadInner.minX ) { RgnWnd.minX = RgnPadInner.minX; }
					if ( RgnWnd.maxX > RgnPadInner.maxX ) { RgnWnd.maxX = RgnPadInner.maxX; }
					break;
				case BOX_TOWARD_RIGHT:					
					RgnWnd.minX = RgnLead.maxX+(GapV);
					RgnWnd.maxX = RgnWnd.minX+(SizeV);
					RgnWnd.maxY = RgnLead.maxY-GapU;
					RgnWnd.minY = RgnLead.minY+GapU;
					if ( RgnWnd.minY < RgnPadInner.minY ) { RgnWnd.minY = RgnPadInner.minY; }
					if ( RgnWnd.maxY > RgnPadInner.maxY ) { RgnWnd.maxY = RgnPadInner.maxY; }
					break;
				}
				JetAPI::BoundaryRegion(RgnPad, RgnWnd);
			}

			WndPtr = AOIObjManager.CreateWndObj();
			if ( NULL == WndPtr ) { return false; }
			WndPtr->SetWndToward(LandToward);
			WndPtr->SetWndBandID(WndBandID);
			WndPtr->SetWndGroupID(WndGouprID);
			WndPtr->SetWndRgnLinkAuto(false);
			WndPtr->SetWndRgnLinkMode(WND_RGN_LINK_NONE);
			WndPtr->SetWndDefectID(WndDefectID);
			WndPtr->SetWndDefectGroupID(DefectGroupID);
			if ( LAND_TYPE_PAD==LandType )
			{
				if ( CheckModelTypeUseBody(Param.eModelType) == true )
				{	WndPtr->SetWndFollowMode(WND_FOLLOW_PART);	}
				else
				{	WndPtr->SetWndFollowMode(WND_FOLLOW_PAD);	}
			}
			else if ( LAND_TYPE_DIP_LEAD==LandType )
			{				
				WndPtr->SetWndFollowMode(WND_FOLLOW_PAD);
				WndPtr->SetWndShapeMode(BOX_SHAPE_ELLIPSE);
			}
			else
			{	WndPtr->SetWndFollowMode(WndFollowMode); }
			WndPtr->SetWndRegion(RgnWnd);	
			WndPtr->SetWndExtendBoxUsed(UseExtendBox);
			WndPtr->SetWndConstrainMode(WND_CONSTRAIN_PAD_RGN_MOVE);
			WndPtr->UpdateWndExtendBox();	
			WndPtr->SetWndAlgType(AlgType);
			WndPtr->GetWndAlgParam().CheckAlgPatternFileUsed();
			if ( true==Param.bUseLogic )
			{	WndPtr->SetWndLogicType(WND_LOGIC_DEFECT_ID);	}
			WndPtr->SetWndSyncMoveMode(WndSyncMoveMode);

			CAlgBinaryParam &BinaryParam=WndPtr->GetWndAlgParam().GetAlgImageBinParam();
			BinaryParam.SetBinaryFrameIndex(FrameIndex);
			BinaryParam.SetBinaryFrameUniqueID(FrameUniqueID);			
			BinaryParam.SetBinaryImageSourceMode(IMAGE_SRC_SYNTHESIS);//IMAGE_SRC_LIGHTNESS
			BinaryParam.SetBinarySynthesisWR(180);
			BinaryParam.SetBinarySynthesisWG(50);
			BinaryParam.SetBinarySynthesisWB(10);
			BinaryParam.SetBinaryMode(BINARY_FIXED_THRESHOLD);
			BinaryParam.SetFixedThresholdHigh(255);
			BinaryParam.SetFixedThresholdLow(100);
			//BinaryParam.SetBinaryColorGroupLinkIndex(PROJECT_COLOR_ID_SOLDER_BEGIN);			
			WndPtr->GetWndAlgParam().GetAlgParamBrightRatio().brRatioUSL = Param.dSolderOpenBrRatioUSL;
			WndPtr->GetWndAlgParam().GetAlgParamBrightRatio().brRatioLSL =  0;			
			WndPtr->GetWndAlgParam().GetAlgParamBrightRatio().brRatioEnabled = true;//比例啟用
			WndPtr->GetWndAlgParam().GetAlgParamBrightRatio().brToleranceEnabled = false;

			if ( NULL != RefModelPtr )
			{	RefWndPtr = RefModelPtr->GetModelWndPtrByLandDefectID(LandPtr, WndDefectID, DefectGroupID);	}
			ApplyModelDefaultLandWnd(WndPtr, RefWndPtr);
			InitialModelWndPtr(WndPtr);
			AddModelWndPtr(WndPtr, false);
			LandPtr->AddLandWndPtr(WndPtr);
		}
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd_SolderPoorKernel_v2(const TMODEL_DEFAULT_WND_PARAM &Param, int RefLandGroupID)//增加模組檢測框-少焊檢測
{
	TMODEL_DEFAULT_WND_PARAM Param2=Param;	
	Param2.bUseLogic = false;
	if ( AddModelDefaultWnd_SolderPoorKernel_v2(Param2, RefLandGroupID, false) == false )
	{	return false; }
	if ( true == Param2.bSolderPoorUseSideWnd )
	{
		if ( AddModelDefaultWnd_SolderPoorKernel_v2(Param2, RefLandGroupID, true) == false )
		{	return false; }
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd_SolderPoorKernel_v2(const TMODEL_DEFAULT_WND_PARAM &Param, int RefLandGroupID, bool bUseSide)//增加模組檢測框-少焊檢測
{
	int          k=0;
	size_t       i=0, j=0;
	int          WndBandID = 0;
	int          WndGouprID = 0;
	int          LandGroupID = 0;
	CAOIWnd     *WndPtr = NULL;
	CAOILand    *LandPtr = NULL;
	TREGION4D    RgnWnd, RgnLead, RgnPad, RgnBody, RgnPadInner;
	LAND_TYPE    LandType=LAND_TYPE_NULL;
	BOX_TOWARD   LandToward = BOX_TOWARD_NULL;	
	double GapU=50;
	double GapV=50;
	double SizeV = 200;
	double StartV = 0.0;
	double RangeU = 0.0;
	double RangeV = 0.0;
	double WndSizeW=0;
	double WndSizeH=0;
	const double MinSizeW=50;
	const double MinSizeH=50;
	ALG_TYPE     AlgType = ALG_BRIGHT_RATIO;
	BOX_TOWARD   BodyToward = GetModelBodyBox().GetBoxToward();		
	const int    MaxLandGroupID = GetModelLandFreeGroupID();
	const size_t LandCount = GetModelLandCount();	
	const WND_DEFECT_ID WndDefectID=WND_DEFECT_SOLDER_POOR;
	const int DefectGroupID = GetModelFreeDefectGroupID(WndDefectID);
	const bool UseExtendBox = CAlgParam::CheckAlgUseExtendBox(AlgType);
	const WND_SYNC_MOVE_MODE WndSyncMoveMode = GetModelWndSyncMoveMode();
	WND_FOLLOW_MODE WndFollowMode = AOIDataDefine.GetWndFollowModeByWndDefectID(WndDefectID);	
	const unsigned int FrameIndex = Param.nFrameIndex_Low;
	const unsigned int FrameUniqueID = Param.nFrameUniqueID_Low;
	CAOIWnd   *RefWndPtr = NULL;
	CAOIModel *RefModelPtr = (CAOIModel*)(Param.pModel);

	GetModelBodyRegion(RgnBody);
	for ( k=0; k<MaxLandGroupID; k ++ )
	{
		if ( RefLandGroupID >= 0 ) 
		{
			if ( RefLandGroupID != k ) { continue; }
		}

		LandGroupID = k;
		WndGouprID = GetModelWndFreeGroupID();
		WndBandID = GetModelWndFreeBandID(WndGouprID);
		for ( i=0; i<LandCount; i++ )
		{
			LandPtr = GetModelLandPtr(i, false);
			if ( NULL == LandPtr ) { continue; }			
			if ( LandPtr->GetLandGroupID() != LandGroupID ) { continue; }
			LandType   = LandPtr->GetLandType();
			LandToward = LandPtr->GetLandToward();
			LandPtr->GetLandPadBox().GetBoxRegion(RgnPad);
			LandPtr->GetLandLeadBox().GetBoxRegion(RgnLead);
			RgnPadInner = RgnPad;
			RgnPadInner.minX += 10;
			RgnPadInner.minY += 10;
			RgnPadInner.maxX -= 10;
			RgnPadInner.maxY -= 10;

			if ( true == bUseSide )
			{
				TPOINT2D PadPos(RgnPad.GetCpX(), RgnPad.GetCpY());
				switch ( LandToward )
				{
				case BOX_TOWARD_UP:
				case BOX_TOWARD_DOWN:
					if ( PadPos.x > 0 ) { LandToward = BOX_TOWARD_RIGHT; }
					else { LandToward = BOX_TOWARD_LEFT; }
					break;
				case BOX_TOWARD_LEFT:
				case BOX_TOWARD_RIGHT:
					if ( PadPos.y > 0 ) { LandToward = BOX_TOWARD_UP; }
					else { LandToward = BOX_TOWARD_DOWN; }
					break;
				}
			}

			if ( LAND_TYPE_PAD == LandType )
			{
				GapU=50;
				GapV=50;
				SizeV = 200;
				switch ( LandToward )
				{
				case BOX_TOWARD_UP:
					if ( RgnBody.maxY>RgnPad.minY && RgnBody.maxY<RgnPad.maxY )					
					{	RgnWnd.minY = RgnBody.maxY+(GapV); }
					else
					{	RgnWnd.minY = RgnPad.minY+(GapV); }										
					RgnWnd.maxY = RgnPad.maxY-GapV;
					RgnWnd.maxX = RgnPad.maxX-GapU;
					RgnWnd.minX = RgnPad.minX+GapU;
					break;
				case BOX_TOWARD_LEFT:
					if ( RgnBody.minX<RgnPad.maxX && RgnBody.minX>RgnPad.minX )
					{	RgnWnd.maxX = RgnBody.minX-(GapV); }
					else
					{	RgnWnd.maxX = RgnPad.maxX-(GapV); }					
					RgnWnd.minX = RgnPad.minX+(GapV);
					RgnWnd.maxY = RgnPad.maxY-GapU;
					RgnWnd.minY = RgnPad.minY+GapU;
					break;
				case BOX_TOWARD_DOWN:
					if ( RgnBody.minY<RgnPad.maxY && RgnBody.minY>RgnPad.minY )					
					{	RgnWnd.maxY = RgnBody.minY-(GapV); }
					else
					{	RgnWnd.maxY = RgnPad.maxY-(GapV); }							
					RgnWnd.minY = RgnPad.minY-(GapV);
					RgnWnd.maxX = RgnPad.maxX-GapU;
					RgnWnd.minX = RgnPad.minX+GapU;
					break;
				case BOX_TOWARD_RIGHT:
					if ( RgnBody.maxX>RgnPad.minX && RgnBody.maxX<RgnPad.maxX )
					{	RgnWnd.minX = RgnBody.maxX+(GapV); }
					else
					{	RgnWnd.minX = RgnPad.minX+(GapV); }					
					RgnWnd.maxX = RgnPad.maxX-(GapV);
					RgnWnd.maxY = RgnPad.maxY-GapU;
					RgnWnd.minY = RgnPad.minY+GapU;
					break;
				}
				JetAPI::BoundaryRegion(RgnPad, RgnWnd);
			}
			else if ( LAND_TYPE_ELECTRODE == LandType )
			{
				GapU=50;
				GapV=50;
				SizeV = 50;
				switch ( LandToward )
				{
				case BOX_TOWARD_UP:
					StartV = MAX(RgnPad.minY, RgnLead.maxY);
					StartV = MAX(StartV, RgnBody.maxY);

					RangeV = RgnPad.maxY-GapV-StartV;
					if ( RangeV > 0 )
					{	SizeV = RangeV*0.5; }
					RgnWnd.minY = StartV+(GapV);
					RgnWnd.maxY = RgnPad.maxY-SizeV;
					RgnWnd.maxX = RgnLead.maxX-GapU;
					RgnWnd.minX = RgnLead.minX+GapU;
					break;
				case BOX_TOWARD_LEFT:
					StartV = MIN(RgnPad.maxX, RgnLead.minX);
					StartV = MIN(StartV, RgnBody.minX);

					RangeV = StartV-GapV-RgnPad.minX;
					if ( RangeV > 0 )
					{	SizeV = RangeV*0.5; }
					RgnWnd.maxX = StartV-(GapV);
					RgnWnd.minX = RgnPad.minX+SizeV;
					RgnWnd.maxY = RgnLead.maxY-GapU;
					RgnWnd.minY = RgnLead.minY+GapU;
					break;
				case BOX_TOWARD_DOWN:
					StartV = MIN(RgnPad.maxY, RgnLead.minY);
					StartV = MIN(StartV, RgnBody.minY);

					RangeV = StartV-GapV-RgnPad.minY;
					if ( RangeV > 0 )
					{	SizeV = RangeV*0.5; }
					RgnWnd.maxY = StartV-(GapV);
					RgnWnd.minY = RgnPad.minY+SizeV;
					RgnWnd.maxX = RgnLead.maxX-GapU;
					RgnWnd.minX = RgnLead.minX+GapU;
					break;
				case BOX_TOWARD_RIGHT:
					StartV = MAX(RgnPad.minX, RgnLead.maxX);
					StartV = MAX(StartV, RgnBody.maxX);

					RangeV = RgnPad.maxX-GapV-StartV;
					if ( RangeV > 0 )
					{	SizeV = RangeV*0.5; }
					RgnWnd.minX = StartV+(GapV);
					RgnWnd.maxX = RgnPad.maxX-SizeV;
					RgnWnd.maxY = RgnLead.maxY-GapU;
					RgnWnd.minY = RgnLead.minY+GapU;
					break;
				}
				JetAPI::BoundaryRegion(RgnPad, RgnWnd);
			}
			else if ( LAND_TYPE_DIP_LEAD == LandType )
			{	
				JetAPI::ScaleRegion(RgnPad, 0.6, 0.6, SCALE_REGION_BY_CENTER, RgnWnd);
			}
			else //if ( LAND_TYPE_IC_LEAD/LAND_TYPE_CON_LEAD == LandType )
			{
				GapU=-50;
				GapV=20;
				SizeV = 150;
				switch ( LandToward )
				{
				case BOX_TOWARD_UP:				
					RgnWnd.minY = RgnLead.maxY+(GapV);
					RgnWnd.maxY = RgnWnd.minY+(SizeV);
					if ( RgnWnd.maxY > RgnPad.maxY ) 
					{	RgnWnd.maxY = RgnPad.maxY; }
				
					RgnWnd.maxX = RgnLead.maxX-GapU;
					RgnWnd.minX = RgnLead.minX+GapU;
					if ( RgnWnd.minX < RgnPadInner.minX ) { RgnWnd.minX = RgnPadInner.minX; }
					if ( RgnWnd.maxX > RgnPadInner.maxX ) { RgnWnd.maxX = RgnPadInner.maxX; }
					break;
				case BOX_TOWARD_LEFT:
					RgnWnd.maxX = RgnLead.minX-(GapV);
					RgnWnd.minX = RgnWnd.maxX-(SizeV);
					if ( RgnWnd.minX < RgnPad.minX ) 
					{	RgnWnd.minX = RgnPad.minX; }
				
					RgnWnd.maxY = RgnLead.maxY-GapU;
					RgnWnd.minY = RgnLead.minY+GapU;
					if ( RgnWnd.minY < RgnPadInner.minY ) { RgnWnd.minY = RgnPadInner.minY; }
					if ( RgnWnd.maxY > RgnPadInner.maxY ) { RgnWnd.maxY = RgnPadInner.maxY; }
					break;
				case BOX_TOWARD_DOWN:
					RgnWnd.maxY = RgnLead.minY-(GapV);
					RgnWnd.minY = RgnWnd.maxY-(SizeV);
					if ( RgnWnd.minY < RgnPad.minY ) 
					{	RgnWnd.minY = RgnPad.minY; }

					RgnWnd.maxX = RgnLead.maxX-GapU;
					RgnWnd.minX = RgnLead.minX+GapU;
					if ( RgnWnd.minX < RgnPadInner.minX ) { RgnWnd.minX = RgnPadInner.minX; }
					if ( RgnWnd.maxX > RgnPadInner.maxX ) { RgnWnd.maxX = RgnPadInner.maxX; }
					break;
				case BOX_TOWARD_RIGHT:
					RgnWnd.minX = RgnLead.maxX+(GapV);
					RgnWnd.maxX = RgnWnd.minX+(SizeV);
					if ( RgnWnd.maxX > RgnPad.maxX ) 
					{	RgnWnd.maxX = RgnPad.maxX; }
				
					RgnWnd.maxY = RgnLead.maxY-GapU;
					RgnWnd.minY = RgnLead.minY+GapU;
					if ( RgnWnd.minY < RgnPadInner.minY ) { RgnWnd.minY = RgnPadInner.minY; }
					if ( RgnWnd.maxY > RgnPadInner.maxY ) { RgnWnd.maxY = RgnPadInner.maxY; }
					break;
				}				
			}

			WndSizeW = RgnWnd.GetWidth();
			WndSizeH = RgnWnd.GetHeight();
			WndSizeW = MAX(WndSizeW, MinSizeW);
			WndSizeH = MAX(WndSizeH, MinSizeH);
			RgnWnd.SetSize(WndSizeW, WndSizeH);
	
			WndPtr = AOIObjManager.CreateWndObj();
			if ( NULL == WndPtr ) { return false; }
			WndPtr->SetWndToward(LandToward);
			WndPtr->SetWndBandID(WndBandID);
			WndPtr->SetWndGroupID(WndGouprID);
			WndPtr->SetWndRgnLinkAuto(false);
			WndPtr->SetWndRgnLinkMode(WND_RGN_LINK_NONE);
			WndPtr->SetWndDefectID(WndDefectID);
			WndPtr->SetWndDefectGroupID(DefectGroupID);
			if ( LAND_TYPE_PAD==LandType )
			{
				if ( CheckModelTypeUseBody(Param.eModelType) == true )
				{	WndPtr->SetWndFollowMode(WND_FOLLOW_PART);	}
				else
				{	WndPtr->SetWndFollowMode(WND_FOLLOW_PAD);	}
			}
			else if ( LAND_TYPE_DIP_LEAD==LandType )
			{
				WndPtr->SetWndFollowMode(WND_FOLLOW_PAD);
				WndPtr->SetWndShapeMode(BOX_SHAPE_ELLIPSE);
			}
			else
			{	WndPtr->SetWndFollowMode(WndFollowMode); }			
			WndPtr->SetWndRegion(RgnWnd);	
			WndPtr->SetWndExtendBoxUsed(UseExtendBox);			
			WndPtr->SetWndConstrainMode(WND_CONSTRAIN_PAD_RGN_MOVE);
			WndPtr->UpdateWndExtendBox();	
			WndPtr->SetWndAlgType(AlgType);
			WndPtr->GetWndAlgParam().CheckAlgPatternFileUsed();
			if ( true==Param.bUseLogic )
			{	WndPtr->SetWndLogicType(WND_LOGIC_DEFECT_ID);	}
			WndPtr->SetWndSyncMoveMode(WndSyncMoveMode);

			CAlgBinaryParam &BinaryParam=WndPtr->GetWndAlgParam().GetAlgImageBinParam();
			BinaryParam.SetBinaryFrameIndex(FrameIndex);
			BinaryParam.SetBinaryFrameUniqueID(FrameUniqueID);			

			BinaryParam.SetBinaryMode(BINARY_COLOR_FILTER);
			BinaryParam.SetBinaryColorGroupLinkIndex(PROJECT_COLOR_ID_PAD_BEGIN);
			WndPtr->GetWndAlgParam().GetAlgParamBrightRatio().brRatioUSL = Param.dSolderPoorBrRatioUSL;
			WndPtr->GetWndAlgParam().GetAlgParamBrightRatio().brRatioLSL =  0;			
			WndPtr->GetWndAlgParam().GetAlgParamBrightRatio().brRatioEnabled = true;//比例啟用
			WndPtr->GetWndAlgParam().GetAlgParamBrightRatio().brToleranceEnabled = false;

			if ( NULL != RefModelPtr )
			{	RefWndPtr = RefModelPtr->GetModelWndPtrByLandDefectID(LandPtr, WndDefectID, DefectGroupID);	}
			ApplyModelDefaultLandWnd(WndPtr, RefWndPtr);
			InitialModelWndPtr(WndPtr);
			AddModelWndPtr(WndPtr, false);
			LandPtr->AddLandWndPtr(WndPtr);
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd_LandBridgeKernelFn_v2(const TMODEL_DEFAULT_WND_PARAM &Param, int RefLandGroupID, bool bUse3D)//增加模組檢測框-短路檢測
{
	int          s=0, t=0;
	size_t       i=0, j=0;
	size_t       LandPtrCount=0;
	int          WndBandID = 0;
	int          WndGouprID = 0;
	int          WndBandID1=0, WndBandID2=0;
	int          LandGroupID = 0;
	int          LandAlignID = 0;
	int          MaxLandAlignID = 0;
	bool         GetBridgeRgn = false;
	bool         GetBridgeRgnT=false, GetBridgeRgnL=false, GetBridgeRgnR=false, GetBridgeRgnB=false;
	double       dMinX=0, dMinY=0, dMaxX=0, dMaxY=0;	
	LAND_TYPE    LandType=LAND_TYPE_NULL;
	CAOIWnd     *WndPtr = NULL;
	CAOIWnd     *WndPtr2 = NULL;
	CAOILand    *LandPtr = NULL;
	CAOILand    *LandPtrNext = NULL;
	CAOILand    *LandPtrNext2 = NULL;
	CAOILand    *LandPtrFirst = NULL;
	CAOILand    *LandPtrLast = NULL;	
	TPOINT2D     PtPad, PtPadNext;
	TREGION4D    RgnLead, RgnLeadNext;
	TREGION4D    RgnWnd1, RgnWnd2;
	TREGION4D    RgnWnd, RgnPad, RgnPadNext;
	TREGION4D    RgnWndT, RgnWndL, RgnWndR, RgnWndB;
	BOX_TOWARD   LandToward = BOX_TOWARD_NULL;
	BOX_TOWARD   ModelBodyToward = GetModelBodyBox().GetBoxToward();
	std::vector<CAOILand*> LandPtrList;
	CAlgParam    *AlgPtr = NULL;	
	const double GapU= 0;
	const double GapV=50;	
	const double dMoveRatio_3D=0.4;
	const double dScaleRatio_3D = 0.7;

	size_t                SortCount=0;
	CSortObj              SortObj;
	std::vector<CSortObj> SortListTmp;
	std::vector<CSortObj> SortListToUp;
	std::vector<CSortObj> SortListToRight;
	std::vector<CSortObj> SortListToLeft;
	std::vector<CSortObj> SortListToDown;
	bool                 bSortForward=true;
	const bool           bBridgeTwoSide=Param.bBridgeTwoSide;//雙邊增加檢測框
	ALG_TYPE     AlgType = ALG_BRIGHT_RATIO;
	BOX_TOWARD   BodyToward = GetModelBodyBox().GetBoxToward();			
	const int    MaxLandGroupID = GetModelLandFreeGroupID();
	const size_t LandCount = GetModelLandCount();		
	const WND_DEFECT_ID WndDefectID=WND_DEFECT_SOLDER_BRIDGE;
	const int DefectGroupID = GetModelFreeDefectGroupID(WndDefectID);
	MODEL_LAND_DIRECTION ModelLandDir = GetModelLandDirection();
	const bool UseExtendBox = CAlgParam::CheckAlgUseExtendBox(AlgType);	
	WND_FOLLOW_MODE WndFollowMode = AOIDataDefine.GetWndFollowModeByWndDefectID(WndDefectID);
	unsigned int FrameIndex = Param.nFrameIndex_Low;
	unsigned int FrameUniqueID = Param.nFrameUniqueID_Low;
	int          FixedThreshold = 128;
	TBINARY_FILTER BinaryFilter;
	BinaryFilter.FilterMode = NOISE_FILTER_OPEN;
	if ( LandCount < 1 ) { return true; }	
	
	CAOIWnd   *RefWndPtr = NULL;
	CAOIModel *RefModelPtr = (CAOIModel*)(Param.pModel);
	if ( true == bUse3D )
	{
		FrameIndex = Param.nFrameIndex_3D;
		FrameUniqueID = Param.nFrameUniqueID_3D;		
	}

	for ( s=0; s<MaxLandGroupID; s ++ )
	{
		if ( RefLandGroupID >= 0 ) 
		{
			if ( RefLandGroupID != s ) { continue; }
		}
		LandGroupID = s;
		MaxLandAlignID = GetModelLandFreeAlignID(LandGroupID);
		WndGouprID = GetModelWndFreeGroupID();
		WndBandID = GetModelWndFreeBandID(WndGouprID);
		WndBandID1 = WndBandID+1;
		WndBandID2 = WndBandID+2;		
		for ( t=0; t<MaxLandAlignID; t++ )
		{
			LandAlignID = t;
			//要先排序個方向的腳數			
			LandPtrList.clear();	
			SortListToUp.clear();
			SortListToRight.clear();
			SortListToLeft.clear();
			SortListToDown.clear();
			SortObj.SetSortMode(SORT_BY_INT);
			for ( i=0; i<LandCount; i++ )
			{
				LandPtr = GetModelLandPtr(i, false);
				if ( NULL == LandPtr ) { continue; }
				if ( LandPtr->GetLandGroupID() != LandGroupID ) { continue; }
				if ( LandPtr->GetLandAlignID() != LandAlignID ) { continue; }

				LandToward  = LandPtr->GetLandToward();				
				LandPtr->GetLandBoxPtr()->GetBoxRegion(RgnPad);

				SortObj.SetID(i);
				SortObj.SetPtr(LandPtr);				
				switch ( LandToward )
				{
				case BOX_TOWARD_UP:					
					SortObj.SetValueInt((int)(RgnPad.minX));
					SortListToUp.push_back(SortObj);
					break;
				case BOX_TOWARD_DOWN:					
					SortObj.SetValueInt((int)(RgnPad.minX));
					SortListToDown.push_back(SortObj);
					break;
				case BOX_TOWARD_LEFT:						
					SortObj.SetValueInt((int)(RgnPad.minY));
					SortListToLeft.push_back(SortObj);
					break;
				case BOX_TOWARD_RIGHT:					
					SortObj.SetValueInt((int)(RgnPad.minY));
					SortListToRight.push_back(SortObj);
					break;				
				}				
			}			
			//1. 先加入和模組同向的
			switch ( ModelBodyToward )
			{
			case BOX_TOWARD_UP:
				bSortForward = false;
				SortListTmp = SortListToUp;	
				break;
			case BOX_TOWARD_DOWN:
				bSortForward = true;
				SortListTmp = SortListToDown; 
				break;
			case BOX_TOWARD_LEFT: 
				bSortForward = false;
				SortListTmp = SortListToLeft; 
				break;
			case BOX_TOWARD_RIGHT: 
				bSortForward = true;
				SortListTmp = SortListToRight;
				break;
			default:	SortListTmp.clear(); break;
			}			
			SortCount = SortListTmp.size();
			if ( SortCount > 1 )
			{	
				if ( MODEL_LAND_CLOCKWISE == ModelLandDir )
				{
					if ( true == bSortForward ) { bSortForward = false; }
					else { bSortForward = true; }
				}
				std::sort(SortListTmp.begin(), SortListTmp.end());
				SortCount = SortListTmp.size();
				for ( i=0; i<SortCount; i++ )
				{
					if ( true==bSortForward )
					{	SortObj = SortListTmp[i]; }
					else
					{	SortObj = SortListTmp[SortCount-i-1]; }
					LandPtr = (CAOILand*)(SortObj.GetPtr());
					LandPtrList.push_back(LandPtr);
				}				
			}

			//2. 先加入和模組同向的
			if ( MODEL_LAND_CLOCKWISE == ModelLandDir )
			{	LandToward = JetAPI::RotateToward(-90, ModelBodyToward);	}
			else
			{	LandToward = JetAPI::RotateToward(90, ModelBodyToward);	}
			switch ( LandToward )
			{
			case BOX_TOWARD_UP:
				bSortForward = false;
				SortListTmp = SortListToUp;
				break;
			case BOX_TOWARD_RIGHT: 
				bSortForward = true;
				SortListTmp = SortListToRight;
				break;
			case BOX_TOWARD_DOWN: 
				bSortForward = true;
				SortListTmp = SortListToDown; 
				break;
			case BOX_TOWARD_LEFT: 
				bSortForward = false;
				SortListTmp = SortListToLeft; 
				break;				
			default:	SortListTmp.clear(); break;
			}
			SortCount = SortListTmp.size();
			if ( SortCount > 1 )
			{	
				if ( MODEL_LAND_CLOCKWISE == ModelLandDir )
				{
					if ( true == bSortForward ) { bSortForward = false; }
					else { bSortForward = true; }
				}
				std::sort(SortListTmp.begin(), SortListTmp.end());
				SortCount = SortListTmp.size();
				for ( i=0; i<SortCount; i++ )
				{
					if ( true==bSortForward )
					{	SortObj = SortListTmp[i]; }
					else
					{	SortObj = SortListTmp[SortCount-i-1]; }
					LandPtr = (CAOILand*)(SortObj.GetPtr());
					LandPtrList.push_back(LandPtr);
				}				
			}

			//3. 先加入和模組同向的
			LandToward = JetAPI::RotateToward(180, ModelBodyToward);
			switch ( LandToward )
			{
			case BOX_TOWARD_UP:
				bSortForward = false;
				SortListTmp = SortListToUp;
				break;
			case BOX_TOWARD_RIGHT: 
				bSortForward = true;
				SortListTmp = SortListToRight;
				break;
			case BOX_TOWARD_DOWN: 
				bSortForward = true;
				SortListTmp = SortListToDown; 
				break;
			case BOX_TOWARD_LEFT: 
				bSortForward = false;
				SortListTmp = SortListToLeft; 
				break;				
			default:	SortListTmp.clear(); break;
			}		
			SortCount = SortListTmp.size();
			if ( SortCount > 1 )
			{
				if ( MODEL_LAND_CLOCKWISE == ModelLandDir )
				{
					if ( true == bSortForward ) { bSortForward = false; }
					else { bSortForward = true; }
				}
				std::sort(SortListTmp.begin(), SortListTmp.end());
				SortCount = SortListTmp.size();
				for ( i=0; i<SortCount; i++ )
				{
					if ( true==bSortForward )
					{	SortObj = SortListTmp[i]; }
					else
					{	SortObj = SortListTmp[SortCount-i-1]; }
					LandPtr = (CAOILand*)(SortObj.GetPtr());
					LandPtrList.push_back(LandPtr);
				}				
			}
			//4. 先加入和模組同向的			
			if ( MODEL_LAND_CLOCKWISE == ModelLandDir )
			{	LandToward = JetAPI::RotateToward(-270, ModelBodyToward);	}
			else
			{	LandToward = JetAPI::RotateToward(270, ModelBodyToward);	}
			switch ( LandToward )
			{
			case BOX_TOWARD_UP:
				bSortForward = false;
				SortListTmp = SortListToUp;
				break;
			case BOX_TOWARD_RIGHT: 
				bSortForward = true;
				SortListTmp = SortListToRight;
				break;
			case BOX_TOWARD_DOWN: 
				bSortForward = true;
				SortListTmp = SortListToDown; 
				break;
			case BOX_TOWARD_LEFT: 
				bSortForward = false;
				SortListTmp = SortListToLeft; 
				break;				
			default:	SortListTmp.clear(); break;
			}					
			SortCount = SortListTmp.size();
			if ( SortCount > 1 )
			{
				if ( MODEL_LAND_CLOCKWISE == ModelLandDir )
				{
					if ( true == bSortForward ) { bSortForward = false; }
					else { bSortForward = true; }
				}
				std::sort(SortListTmp.begin(), SortListTmp.end());
				SortCount = SortListTmp.size();
				for ( i=0; i<SortCount; i++ )
				{
					if ( true==bSortForward )
					{	SortObj = SortListTmp[i]; }
					else
					{	SortObj = SortListTmp[SortCount-i-1]; }
					LandPtr = (CAOILand*)(SortObj.GetPtr());
					LandPtrList.push_back(LandPtr);
				}				
			}
			LandPtrCount = LandPtrList.size();
			if ( LandPtrCount < 2) { continue; }
			
			LandPtrFirst = NULL;
			LandPtrLast  = NULL;			
			GetBridgeRgnT = GetBridgeRgnL = GetBridgeRgnR = GetBridgeRgnB = false;
			for ( i=0; i<LandPtrCount-1; i++ )
			{
				LandPtr = LandPtrList[i];
				LandPtrNext = LandPtrList[i+1];
				if ( NULL == LandPtr ) { continue; }
				if ( NULL == LandPtrNext ) { continue; }
				if ( LandPtr->GetLandGroupID() != LandGroupID ) { continue; }
				if ( LandPtrNext->GetLandGroupID() != LandGroupID ) { continue; }				

				LandType    = LandPtr->GetLandType();
				LandToward  = LandPtr->GetLandToward();
				LandAlignID = LandPtr->GetLandAlignID();
				if ( LAND_TYPE_DIP_LEAD == LandType ) { continue; }
				if ( LandPtrNext->GetLandToward() != LandToward ) { continue;	}
				if ( LandPtrNext->GetLandAlignID() != LandAlignID ) { continue; }				

				LandPtr->GetLandPadBox().GetBoxRegion(RgnPad);
				LandPtr->GetLandLeadBox().GetBoxRegion(RgnLead);			
				LandPtrNext->GetLandPadBox().GetBoxRegion(RgnPadNext);
				LandPtrNext->GetLandLeadBox().GetBoxRegion(RgnLeadNext);

				PtPad.x = (RgnPad.minX+RgnPad.maxX)*0.5;
				PtPad.y = (RgnPad.minY+RgnPad.maxY)*0.5;
				PtPadNext.x = (RgnPadNext.minX+RgnPadNext.maxX)*0.5;
				PtPadNext.y = (RgnPadNext.minY+RgnPadNext.maxY)*0.5;
				GetBridgeRgn = false;				
				switch ( LandToward )
				{
				case BOX_TOWARD_UP:
					if ( RgnPad.maxY < RgnPadNext.maxY )
					{	RgnWnd.maxY = RgnPad.maxY-(GapV);	}
					else
					{	RgnWnd.maxY = RgnPadNext.maxY-(GapV);	}

					if ( RgnPad.minY > RgnPadNext.minY )
					{	RgnWnd.minY = RgnPad.minY+(GapV);	}
					else
					{	RgnWnd.minY = RgnPadNext.minY+(GapV);	}					
					if ( LAND_TYPE_ELECTRODE == LandType )
					{	dMaxY = MAX(RgnLead.maxY, RgnLeadNext.maxY);	}
					else if ( LAND_TYPE_IC_LEAD == LandType )
					{	dMaxY = MAX(RgnLead.minY, RgnLeadNext.minY);	}	
					else if ( LAND_TYPE_CON_LEAD == LandType )
					{	dMaxY = MAX(RgnLead.minY, RgnLeadNext.minY);	}						
					else { dMaxY = RgnWnd.minY; }
					dMinY = MAX(RgnWnd.minY, dMaxY);
					if ( dMinY < RgnWnd.maxY )
					{	RgnWnd.minY = dMinY;	}					
					if ( RgnPad.minX > RgnPadNext.maxX )
					{
						RgnWnd.maxX = RgnPad.minX-GapU;
						RgnWnd.minX = RgnPadNext.maxX+GapU;
						GetBridgeRgn = true;
					}
					else if (RgnPadNext.minX > RgnPad.maxX )
					{
						RgnWnd.maxX = RgnPadNext.minX-GapU;
						RgnWnd.minX = RgnPad.maxX+GapU;
						GetBridgeRgn = true;
					}
					if ( true == GetBridgeRgn )
					{						
						if ( true == bUse3D )
						{
							double Size=RgnWnd.GetSizeY();
							double dMove=Size*dMoveRatio_3D;
							double dSize=Size*dScaleRatio_3D;
							RgnWnd.Move(0, dMove);
							RgnWnd.maxY=RgnWnd.minY+dSize;
						}
						GetBridgeRgnT = true;
						RgnWndT = RgnWnd; 
					}	
					break;
				case BOX_TOWARD_LEFT:
					if ( RgnPad.minX > RgnPadNext.minX )
					{	RgnWnd.minX = RgnPad.minX+(GapV);	}
					else
					{	RgnWnd.minX = RgnPadNext.minX+(GapV);	}

					if ( RgnPad.maxX < RgnPadNext.maxX )
					{	RgnWnd.maxX = RgnPad.maxX-(GapV);	}
					else
					{	RgnWnd.maxX = RgnPadNext.maxX-(GapV);	}
					if ( LAND_TYPE_ELECTRODE == LandType )
					{	dMinX = MIN(RgnLead.minX, RgnLeadNext.minX);	}
					else if ( LAND_TYPE_IC_LEAD == LandType )
					{	dMinX = MIN(RgnLead.maxX, RgnLeadNext.maxX);	}
					else if ( LAND_TYPE_CON_LEAD == LandType )
					{	dMinX = MIN(RgnLead.maxX, RgnLeadNext.maxX);	}					
					else { dMinX = RgnWnd.maxX; }
					dMaxX = MIN(RgnWnd.maxX, dMinX);
					if ( dMaxX > RgnWnd.minX ) 
					{	RgnWnd.maxX = dMaxX; }

					if ( RgnPad.minY > RgnPadNext.maxY )
					{
						RgnWnd.maxY = RgnPad.minY-GapU;
						RgnWnd.minY = RgnPadNext.maxY+GapU;
						GetBridgeRgn = true;
					}
					else if (RgnPadNext.minY > RgnPad.maxY )
					{
						RgnWnd.maxY = RgnPadNext.minY-GapU;
						RgnWnd.minY = RgnPad.maxY+GapU;
						GetBridgeRgn = true;
					}
					if ( true == GetBridgeRgn )
					{						
						if ( true == bUse3D )
						{						
							double Size=RgnWnd.GetSizeX();
							double dMove=Size*dMoveRatio_3D;
							double dSize=Size*dScaleRatio_3D;
							RgnWnd.Move(-dMove, 0);
							RgnWnd.minX=RgnWnd.maxX-dSize;
						}
						GetBridgeRgnL = true;
						RgnWndL = RgnWnd; 
					}	
					break;
				case BOX_TOWARD_DOWN:
					if ( RgnPad.minY > RgnPadNext.minY )
					{	RgnWnd.minY = RgnPad.minY+(GapV);	}
					else
					{	RgnWnd.minY = RgnPadNext.minY+(GapV);	}

					if ( RgnPad.maxY < RgnPadNext.maxY )
					{	RgnWnd.maxY = RgnPad.maxY-(GapV);	}
					else
					{	RgnWnd.maxY = RgnPadNext.maxY-(GapV);	}				
					if ( LAND_TYPE_ELECTRODE == LandType )
					{	dMinY = MIN(RgnLead.minY, RgnLeadNext.minY);	}
					else if ( LAND_TYPE_IC_LEAD == LandType )
					{	dMinY = MIN(RgnLead.maxY, RgnLeadNext.maxY);	}
					else if ( LAND_TYPE_CON_LEAD == LandType )
					{	dMinY = MIN(RgnLead.maxY, RgnLeadNext.maxY);	}
					else { dMinY = RgnWnd.maxY; }
					dMaxY = MIN(RgnWnd.maxY, dMinY);
					if ( dMaxY > RgnWnd.minY ) 
					{	RgnWnd.maxY = dMaxY; }

					if ( RgnPad.minX > RgnPadNext.maxX )
					{
						RgnWnd.maxX = RgnPad.minX-GapU;
						RgnWnd.minX = RgnPadNext.maxX+GapU;
						GetBridgeRgn = true;
					}
					else if (RgnPadNext.minX > RgnPad.maxX )
					{
						RgnWnd.maxX = RgnPadNext.minX-GapU;
						RgnWnd.minX = RgnPad.maxX+GapU;
						GetBridgeRgn = true;
					}
					if ( true == GetBridgeRgn )
					{						
						if ( true == bUse3D )
						{
							double Size=RgnWnd.GetSizeY();
							double dMove=Size*dMoveRatio_3D;
							double dSize=Size*dScaleRatio_3D;
							RgnWnd.Move(0, -dMove);
							RgnWnd.minY=RgnWnd.maxY-dSize;
						}
						GetBridgeRgnB = true;
						RgnWndB = RgnWnd; 
					}	
					break;
				case BOX_TOWARD_RIGHT:
					if ( RgnPad.maxX < RgnPadNext.maxX )
					{	RgnWnd.maxX = RgnPad.maxX-(GapV);	}
					else
					{	RgnWnd.maxX = RgnPadNext.maxX-(GapV);	}

					if ( RgnPad.minX > RgnPadNext.minX )
					{	RgnWnd.minX = RgnPad.minX+(GapV);	}
					else
					{	RgnWnd.minX = RgnPadNext.minX+(GapV);	}
					if ( LAND_TYPE_ELECTRODE == LandType )
					{	dMaxX = MAX(RgnLead.maxX, RgnLeadNext.maxX);	}
					else if ( LAND_TYPE_IC_LEAD == LandType )
					{	dMaxX = MAX(RgnLead.minX, RgnLeadNext.minX);	}
					else if ( LAND_TYPE_CON_LEAD == LandType )
					{	dMaxX = MAX(RgnLead.minX, RgnLeadNext.minX);	}
					else {	dMaxX = RgnWnd.minX; }				
					dMinX = MAX(RgnWnd.minX, dMaxX);
					if ( dMinX < RgnWnd.maxX )
					{	RgnWnd.minX = dMinX;	}
					
					if ( RgnPad.minY > RgnPadNext.maxY )
					{
						RgnWnd.maxY = RgnPad.minY-GapU;
						RgnWnd.minY = RgnPadNext.maxY+GapU;
						GetBridgeRgn = true;
					}
					else if (RgnPadNext.minY > RgnPad.maxY )
					{
						RgnWnd.maxY = RgnPadNext.minY-GapU;
						RgnWnd.minY = RgnPad.maxY+GapU;
						GetBridgeRgn = true;
					}
					if ( true == GetBridgeRgn )
					{
						if ( true == bUse3D )
						{						
							double Size=RgnWnd.GetSizeX();
							double dMove=Size*dMoveRatio_3D;
							double dSize=Size*dScaleRatio_3D;
							RgnWnd.Move(dMove, 0);
							RgnWnd.maxX=RgnWnd.minX+dSize;
						}
						GetBridgeRgnR = true;
						RgnWndR = RgnWnd; 
					}	
					break;
				}
				if ( false == GetBridgeRgn ) { continue; }

				WndPtr = AOIObjManager.CreateWndObj();
				if ( NULL == WndPtr ) { return false; }
				WndPtr->SetWndToward(LandToward);
				WndPtr->SetWndBandID(WndBandID);
				WndPtr->SetWndGroupID(WndGouprID);
				WndPtr->SetWndRgnLinkAuto(false);
				WndPtr->SetWndRgnLinkMode(WND_RGN_LINK_NONE);
				WndPtr->SetWndDefectID(WndDefectID);
				WndPtr->SetWndDefectGroupID(DefectGroupID);
				if ( true == bUse3D )
				{
					if ( CheckModelTypeUseBody(Param.eModelType) == true )
					{	WndPtr->SetWndFollowMode(WND_FOLLOW_PART);	}
					else
					{	WndPtr->SetWndFollowMode(WND_FOLLOW_PAD);	}
				}
				else
				{	WndPtr->SetWndFollowMode(WndFollowMode);	}
				WndPtr->SetWndRegion(RgnWnd);	
				WndPtr->SetWndExtendBoxUsed(UseExtendBox);
				WndPtr->UpdateWndExtendBox();	
				WndPtr->SetWndAlgType(AlgType);
				WndPtr->GetWndAlgParam().CheckAlgPatternFileUsed();

				AlgPtr = WndPtr->GetWndAlgParamPtr();	
				AlgPtr->GetAlgImageBinParam().SetBinaryFrameIndex(FrameIndex);
				AlgPtr->GetAlgImageBinParam().SetBinaryFrameUniqueID(FrameUniqueID);				
				AlgPtr->GetAlgImageBinParam().SetBinaryMode(BINARY_FIXED_THRESHOLD);				
				AlgPtr->GetAlgImageBinParam().SetBinaryNoiseFilter1(BinaryFilter);
				if ( FRAME_UNIQUE_ID_DLP == FrameUniqueID )
				{	
					AlgPtr->GetAlgImageBinParam().SetFixedThresholdLow(Param.nBridgeFixThresholdL3D);
					AlgPtr->GetAlgImageBinParam().SetFixedThresholdHigh(FIXED_THRESHOLD_MAX_3D);					
					AlgPtr->GetAlgImageBinParam().SetBinaryImageSourceMode(IMAGE_SRC_GRAY);
				}
				else
				{	
					AlgPtr->GetAlgImageBinParam().SetFixedThresholdLow(Param.nBridgeFixThresholdL2D);
					AlgPtr->GetAlgImageBinParam().SetFixedThresholdHigh(FIXED_THRESHOLD_MAX_2D);
					AlgPtr->GetAlgImageBinParam().SetBinaryImageSourceMode(IMAGE_SRC_LIGHTNESS);
				}
				switch ( LandToward )
				{
				case BOX_TOWARD_UP:
				case BOX_TOWARD_DOWN:
					AlgPtr->GetAlgParamBrightRatio().brXLineEnabled = true;
					break;
				case BOX_TOWARD_LEFT:
				case BOX_TOWARD_RIGHT:
					AlgPtr->GetAlgParamBrightRatio().brYLineEnabled = true;
					break;
				}
				AlgPtr->GetAlgParamBrightRatio().brRatioEnabled = false;
				AlgPtr->GetAlgParamBrightRatio().brToleranceEnabled = false;

				if ( NULL != RefModelPtr )
				{	RefWndPtr = RefModelPtr->GetModelWndPtrByLandDefectID(LandPtr, WndDefectID, DefectGroupID);	}
				ApplyModelDefaultLandWnd(WndPtr, RefWndPtr);
				if ( false == bBridgeTwoSide )
				{	
					InitialModelWndPtr(WndPtr);
					AddModelWndPtr(WndPtr, false);
					LandPtr->AddLandWndPtr(WndPtr);
				}
				else
				{					
					//第1隻腳
					if ( NULL==LandPtrFirst )
					{
						WndPtr2 = WndPtr->CloneWndObj();
						if ( NULL == WndPtr2 ) { return false; }
						
						switch ( LandToward )
						{
						case BOX_TOWARD_UP:							
						case BOX_TOWARD_DOWN:							
							WndPtr2->MirrorWndYAxis(RgnPad.GetCpX());
							break;
						case BOX_TOWARD_LEFT:							
						case BOX_TOWARD_RIGHT:
							WndPtr2->MirrorWndXAxis(RgnPad.GetCpY());
							break;
						}						
						WndPtr2->SetWndBandID(WndBandID1);
						InitialModelWndPtr(WndPtr2);
						AddModelWndPtr(WndPtr2, false);
						LandPtr->AddLandWndPtr(WndPtr2);
						LandPtrFirst = LandPtr;
					}
					InitialModelWndPtr(WndPtr);
					AddModelWndPtr(WndPtr, false);
					LandPtr->AddLandWndPtr(WndPtr);					

					//最末一隻					
					if ( i < LandPtrCount-2 )
					{
						LandPtrNext2 = LandPtrList[i+2];
						if ( NULL != LandPtrNext2 )
						{							
							if ( LandPtrNext2->GetLandGroupID() != LandGroupID ||
								 LandPtrNext2->GetLandToward() != LandToward ||
								 LandPtrNext2->GetLandAlignID() != LandAlignID)
							{	LandPtrNext2 = NULL;	}
						}						
					}
					else
					{	LandPtrNext2 = NULL; }

					if ( NULL == LandPtrNext2 )
					{
						WndPtr2 = WndPtr->CloneWndObj();
						if ( NULL == WndPtr2 ) { return false; }
						
						switch ( LandToward )
						{
						case BOX_TOWARD_UP:							
						case BOX_TOWARD_DOWN:							
							WndPtr2->MirrorWndYAxis(RgnPadNext.GetCpX());
							break;
						case BOX_TOWARD_LEFT:							
						case BOX_TOWARD_RIGHT:
							WndPtr2->MirrorWndXAxis(RgnPadNext.GetCpY());
							break;
						}
						WndPtr2->SetWndBandID(WndBandID2);
						InitialModelWndPtr(WndPtr2);
						AddModelWndPtr(WndPtr2, false);
						LandPtrNext->AddLandWndPtr(WndPtr2);						
						LandPtrFirst = NULL;
					}
				}
			}
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd_LandOuterShortKernelFn_v2(const TMODEL_DEFAULT_WND_PARAM &Param, int RefLandGroupID, bool bUse3D)//增加模組檢測框-焊盤外接短路檢測	
{
	int          k=0;
	size_t       i=0, j=0;
	int          WndBandID = 0;
	int          WndGouprID = 0;
	int          LandGroupID = 0;
	CAOIWnd     *WndPtr = NULL;
	CAOILand    *LandPtr = NULL;
	TREGION4D    RgnWnd, RgnLead, RgnPad, RgnBody, RgnPadInner;
	BOX_TOWARD   LandToward = BOX_TOWARD_NULL;	
	double       GapU=50;
	double       GapV=50;
	double       SizeU=0;
	double       SizeV=0;	
	double       StartV=0;	
	const double ExtX = Param.dBridgeExtendRange;
	const double ExtY = Param.dBridgeExtendRange;	

	ALG_TYPE     AlgType = ALG_OUTER_SHORT;
	LAND_TYPE    LandType=LAND_TYPE_NULL;	
	BOX_TOWARD   BodyToward = GetModelBodyBox().GetBoxToward();		
	const int    MaxLandGroupID = GetModelLandFreeGroupID();
	const size_t LandCount = GetModelLandCount();	
	const WND_DEFECT_ID WndDefectID=WND_DEFECT_SOLDER_BRIDGE;
	const int DefectGroupID = GetModelFreeDefectGroupID(WndDefectID);
	const bool UseExtendBox = CAlgParam::CheckAlgUseExtendBox(AlgType);	
	WND_FOLLOW_MODE WndFollowMode = AOIDataDefine.GetWndFollowModeByWndDefectID(WndDefectID);
	unsigned int FrameIndex = Param.nFrameIndex_Low;
	unsigned int FrameUniqueID = Param.nFrameUniqueID_Low;	
	int          FixedThreshold = 128;
	CAOIWnd   *RefWndPtr = NULL;
	CAOIModel *RefModelPtr = (CAOIModel*)(Param.pModel);
	if ( true == bUse3D )
	{
		FrameIndex = Param.nFrameIndex_3D;
		FrameUniqueID = Param.nFrameUniqueID_3D;	
	}
	GetModelBodyRegion(RgnBody);
	for ( k=0; k<MaxLandGroupID; k ++ )
	{
		if ( RefLandGroupID >= 0 ) 
		{
			if ( RefLandGroupID != k ) { continue; }
		}

		LandGroupID = k;		
		WndGouprID = GetModelWndFreeGroupID();
		WndBandID = GetModelWndFreeBandID(WndGouprID);
		for ( i=0; i<LandCount; i++ )
		{
			LandPtr = GetModelLandPtr(i, false);
			if ( NULL == LandPtr ) { continue; }			
			if ( LandPtr->GetLandGroupID() != LandGroupID ) { continue; }
			LandType   = LandPtr->GetLandType();
			LandToward = LandPtr->GetLandToward();
			LandPtr->GetLandPadBox().GetBoxRegion(RgnPad);
			LandPtr->GetLandLeadBox().GetBoxRegion(RgnLead);
			RgnPadInner = RgnPad;
			RgnPadInner.minX += 10;
			RgnPadInner.minY += 10;
			RgnPadInner.maxX -= 10;
			RgnPadInner.maxY -= 10;

			//JetAPI::BoundaryRegion(RgnPad, RgnWnd);

			WndPtr = AOIObjManager.CreateWndObj();
			if ( NULL == WndPtr ) { return false; }
			WndPtr->SetWndToward(LandToward);
			WndPtr->SetWndBandID(WndBandID);
			WndPtr->SetWndGroupID(WndGouprID);
			WndPtr->SetWndRgnLinkAuto(true);
			WndPtr->SetWndRgnLinkMode(WND_RGN_LINK_PAD);//WND_RGN_LINK_NONE, WND_RGN_LINK_PAD_RGN
			WndPtr->SetWndDefectID(WndDefectID);
			WndPtr->SetWndDefectGroupID(DefectGroupID);
			if ( true == bUse3D )
			{
				if ( CheckModelTypeUseBody(Param.eModelType) == true )
				{	WndPtr->SetWndFollowMode(WND_FOLLOW_PART);	}
				else
				{	WndPtr->SetWndFollowMode(WND_FOLLOW_PAD);	}
			}
			else
			{	WndPtr->SetWndFollowMode(WndFollowMode);	}
			WndPtr->SetWndRegion(RgnWnd);	
			WndPtr->SetWndExtendRangeX(ExtX);
			WndPtr->SetWndExtendRangeY(ExtY);			
			WndPtr->SetWndExtendBoxUsed(UseExtendBox);
			WndPtr->SetWndConstrainMode(WND_CONSTRAIN_DISABLE);
			WndPtr->UpdateWndExtendBox();	
			WndPtr->SetWndAlgType(AlgType);			

			CAlgParam                 &AlgParam = WndPtr->GetWndAlgParam();
			CAlgBinaryParam           &BinaryParam = AlgParam.GetAlgImageBinParam();			

			AlgParam.CheckAlgPatternFileUsed();			
			BinaryParam.SetBinaryFrameIndex(FrameIndex);
			BinaryParam.SetBinaryFrameUniqueID(FrameUniqueID);
			BinaryParam.SetBinaryMode(BINARY_FIXED_THRESHOLD);
			if ( FRAME_UNIQUE_ID_DLP==FrameUniqueID )
			{				
				BinaryParam.SetFixedThresholdLow(Param.nBridgeFixThresholdL3D);
				BinaryParam.SetFixedThresholdHigh(FIXED_THRESHOLD_MAX_3D);
				BinaryParam.SetBinaryImageSourceMode(IMAGE_SRC_GRAY);
			}
			else
			{	
				FixedThreshold = Param.nBridgeFixThresholdL2D;
				BinaryParam.SetFixedThresholdLow(FixedThreshold);	 
				BinaryParam.SetFixedThresholdHigh(FIXED_THRESHOLD_MAX_2D);
				BinaryParam.SetBinaryImageSourceMode(IMAGE_SRC_LIGHTNESS);
			}
			TBINARY_FILTER BinaryFilter;
			BinaryFilter.FilterMode = NOISE_FILTER_OPEN;
			BinaryFilter.FilterParam1 = 3;
			BinaryParam.SetBinaryNoiseFilter1(BinaryFilter);

			if ( NULL != RefModelPtr )
			{	RefWndPtr = RefModelPtr->GetModelWndPtrByLandDefectID(LandPtr, WndDefectID, DefectGroupID);	}
			ApplyModelDefaultLandWnd(WndPtr, RefWndPtr);
			InitialModelWndPtr(WndPtr);
			AddModelWndPtr(WndPtr, false);
			LandPtr->AddLandWndPtr(WndPtr);
		}
	}	
	return true;
}
//-------------------------------------------------------------------------------------//