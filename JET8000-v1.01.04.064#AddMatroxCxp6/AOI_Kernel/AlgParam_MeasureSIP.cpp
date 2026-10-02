// AlgParam_MeasureSIP.cpp: implementation of the CAlgParam class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "AlgParam.h"
//-------------------------------------------------------------------------------------//
#include "AOIWnd.h"
#include "AOILand.h"
#include "AOIModel.h"
#include "AOIFileIO.h"
#include "JetMatch.h"
#include "JetBlob.h"
#include "JetBarcode.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//

bool CAlgParam::ExecAlgInspection_MeasureSIP(CAOIModel * ModelPtr, const TREGION4D & ModelRgn, const RECT & RoiRect, const RECT & WndRect, std::vector<TUNI_FRAME>& UniFrameList)
{
	if ( NULL == ModelPtr ) { return false; }	
	CAOIComponent *ComponentPtr = ModelPtr->GetModelComponentPtr();
	const size_t FrameCount = UniFrameList.size();		
	const size_t ImageFrameIndex = m_AlgImageBinParam.GetBinaryFrameIndex();
	if ( ImageFrameIndex >= FrameCount ) 
	{	return false; }
	
	CAOIWnd         *WndPtr = CAlgParam::GetAlgWndPtr();
	if ( NULL == WndPtr ) { return false; }	
	
	CString          str;	
	size_t           i=0, Count=0;	
	//int              j=0, k=0, index=0;	
	RECT             WndRoiRect;	
	const int        nAlign = 4;	
	TREGION4D        WndRegion;
	TREGION4D        WndRoiRegion;
	CAOIWndRoi      *WndRoiPtr = NULL;	
	CAlgBinaryParam *BinaryParamPtr=NULL;	
	const unsigned int WndIndex = WndPtr->GetWndIndex();	
	const size_t     WndRoiCount = WndPtr->GetWndRoiWndCount();	

	TUNI_FRAME      *UniFramePtr  = &(UniFrameList[ImageFrameIndex]);
	const IMAGE_SIZE FrameImageW    = UniFramePtr->ImageW;
	const IMAGE_SIZE FrameImageH    = UniFramePtr->ImageH;
	const IMAGE_SIZE FrameImageStep = UniFramePtr->ImageStep;
	const IMAGE_SIZE FrameBitCount  = UniFramePtr->BitCount;		
	IMAGE_PTR        FrameImagePtr  = UniFramePtr->ImagePtr;
	MASK_PTR         FrameMaskPtr   = UniFramePtr->MaskPtr;
	SPACE_PTR        FrameSpacePtr  = UniFramePtr->SpacePtr;
	TPOINT2D         ImageScale     = ModelPtr->GetModelImageScale();
	TALG_PARAM_MEASURE_SIP &msParam = GetAlgParamMeasureSIP();
	TALG_PARAM_ANGLE_MEASURE &amPartParam = msParam.msPart_am;
	TALG_PARAM_ANGLE_MEASURE &amRefParam = msParam.msRef_am;
	//TALG_PARAM_IMAGE_MATCH &imParam = msParam.msMark_im;
	const size_t InspectionEdgeCount = msParam.msInspecEdgeCount;
	const size_t RefEdgeCount = msParam.msRefEdgeCount;
	const size_t TotalCount = 1 + InspectionEdgeCount + RefEdgeCount;
	
	if (TotalCount != WndRoiCount) { 
		SetAlgResultID(RESULT_ID_EXCEPTION);
	}
	//TALG_PARAM_ANGLE_MEASURE &amParam  = GetAlgParamAngleMeasure();	
	
	const double     SpaceRatio = AOIDataCollect.GetSpaceToGrayRatio();
	const IMAGE_SIZE RoiW = RoiRect.right-RoiRect.left;
	const IMAGE_SIZE RoiH = RoiRect.bottom-RoiRect.top;
	const IMAGE_SIZE RoiStep = JetAPI::GetBMPImagePixelsPerLine(RoiW, FrameBitCount, nAlign);
	const IMAGE_SIZE RoiRectSize = (int)((RoiRect.right-RoiRect.left)*(RoiRect.bottom-RoiRect.top));
	
	IMAGE_SIZE    MaskW=0;
	IMAGE_SIZE    MaskH=0;	
	IMAGE_SIZE    MaskStep=0;
	IMAGE_SIZE    MaskBitCount=8;
	MASK_PTR      MaskPtr  = NULL;	
	IMAGE_PTR     GrayPtr = NULL;	
	IMAGE_PTR     GrayRoiPtr = NULL;
	const bool    bTestWnd = false;
	TUNI_FRAME    WndUniFrame;
	std::vector<TUNI_FRAME> WndUniFrameList;
#ifdef _DEBUG		
	bool         bSave = true;
	CString      ComponentName;
	CString      DebugFolder=GetAlgDebugFolder();
	if ( NULL != ComponentPtr )
	{	ComponentName = ComponentPtr->GetComponentFullName(); }		

	if ( true == bSave )
	{			
		str.Format(_T("%s\\%s_ModelWndAlgMeasureSIP#%d_Model.PNG"), DebugFolder, ComponentName, WndIndex+1);
		ImageAPI.SaveImage(str, FrameImageW, FrameImageH, FrameImageStep, FrameBitCount, FrameImagePtr, true);
	}
#endif//_DEBUG

	if ( BuildWndUniFrameList(UniFrameList, RoiRect, WndUniFrameList) == false )
	{	return false;	}	
	
	RECT RoiWndRect={0,0,0,0};	
	WndPtr->GetWndExtendRegion(WndRegion);
	//WndPtr->GetWndRegion(WndRegion);
	JetAPI::SizeToRect(RoiW, RoiH, RoiWndRect);
	
	double CadSkew_Self = 0;
	double CadOffsetX_Self = 0;
	double CadOffsetY_Self = 0;
	double CadSkew_Others = 0;
	double CadOffsetX_Others = 0;
	double CadOffsetY_Others = 0;
	const bool bChkDefect = true;

	CAOIBox  ResBox;
	CAOIBox *WndRoiBoxPtr = NULL;

	TPOINT2D  RgnCp;
	TPOINT2D  ImageCp;	
	TPOINT3D  TiltPt1;
	TPOINT3D  TiltPt2;
	std::vector<TPOINT2D>  CenterPtList;
	std::vector<TLineEquation2D> LineList;
	RESULT_ID MatchResultID;

	ImageCp.x = FrameImageW;
	ImageCp.y = FrameImageH;
	ImageCp.x = ImageCp.x*0.5;
	ImageCp.y = ImageCp.y*0.5;				
	RgnCp.x = ModelRgn.GetCpX();
	RgnCp.y = ModelRgn.GetCpY();	
	const int BoxStartX=RoiRect.left;
	const int BoxStartY=RoiRect.top;
	const size_t WndFrameCount = WndUniFrameList.size();
	
	for ( i=0; i<WndRoiCount; i++ )
	{
		WndRoiPtr = WndPtr->GetWndRoiWndPtr(i, false);
		if ( NULL == WndRoiPtr ) { continue; }		
		WndRoiBoxPtr = WndRoiPtr->GetWndRoiBoxPtr();
		ResBox = *WndRoiBoxPtr;
		if (i == 0) {
			//定位點
			WndRoiPtr->GetWndRoiRegion(WndRoiRegion);
			const double ExtandSizeX = WndPtr->GetWndExtendRangeX();
			const double ExtandSizeY = WndPtr->GetWndExtendRangeY();
			double RoiRegionW = WndRoiRegion.GetWidth() + 2*ExtandSizeX;
			double RoiRegionH = WndRoiRegion.GetHeight() + 2*ExtandSizeY;
			WndRoiRegion.SetSize(RoiRegionW, RoiRegionH);
			if (JetAPI::CalcRegionRect(WndRegion, RoiWndRect, WndRoiRegion, WndRoiRect, true) == false) //Cad和Image的Y是顛倒的
			{	continue;	}
			if (false == ExecAlgInspection_ImageMatch(ModelPtr, ModelRgn, WndRoiRect, WndRect, WndUniFrameList)) { return false; }
			MatchResultID = GetAlgResultID();
			CheckAlgOffset(CadOffsetX_Self, CadOffsetY_Self, CadSkew_Self, CadOffsetX_Others, CadOffsetY_Others, CadSkew_Others, bChkDefect);
			//位置修正
			ResBox.MoveBox(CadOffsetX_Self, CadOffsetY_Self, true);
			WndPtr->AddWndResultBox(ResBox);
			continue;
		}
		//位置跟隨
		ResBox.MoveBox(CadOffsetX_Self, CadOffsetY_Self, true);
		WndPtr->AddWndResultBox(ResBox);
		ResBox.GetBoxRegion(WndRoiRegion);
		
		if (JetAPI::CalcRegionRect(WndRegion, RoiWndRect, WndRoiRegion, WndRoiRect, true) == false) //Cad和Image的Y是顛倒的
		{	continue;	}

		BinaryParamPtr = WndRoiPtr->GetWndRoiBinaryParamPtr();
		if ( ExecAlgUniFrameBinary(*BinaryParamPtr, WndRoiRect, WndRoiRect, WndUniFrameList, MaskW, MaskH, MaskStep, MaskBitCount, MaskPtr, GrayPtr, bTestWnd) == false )
		{				
			JetMemory.free_func(MaskPtr);
			JetMemory.free_func(GrayPtr);
			JetAPI::ClearUniFrameList(WndUniFrameList);
			return false; 
		}		

	#ifdef _DEBUG
		if ( true == bSave )
		{			
			str.Format(_T("%s\\%s_ModelWndAlgAngleMeasure#%d_Gray#%d.PNG"), DebugFolder, ComponentName, WndIndex+1, i+1);
			ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, MaskBitCount, GrayPtr, true);
			str.Format(_T("%s\\%s_ModelWndAlgAngleMeasure#%d_Mask#%d.PNG"), DebugFolder, ComponentName, WndIndex+1, i+1);
			ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, MaskBitCount, MaskPtr, true);
		}
	#endif//_DEBUG


		TLineEquation2D ImgLine;
		if ( BINARY_DISABLE == BinaryParamPtr->GetBinaryMode() )
		{	ImageAPI.CalcGrayImageLineEquation2D(MaskW, MaskH, MaskStep, GrayPtr, WndRoiRect, ImgLine);	}
		else
		{	ImageAPI.CalcGrayImageLineEquation2D(MaskW, MaskH, MaskStep, MaskPtr, WndRoiRect, ImgLine);	}
		
		TPOINT2D ImgPt1, ImgPt2;
		TPOINT2D BoxPt1, BoxPt2, BoxPtCenter;
		TLineEquation2D BoxLine=ImgLine;
		if ( fabs(ImgLine.a+1) < 0.00001 )//x=by+c
		{
			ImgPt1.y = WndRoiRect.top;
			ImgPt1.x = (ImgPt1.y*ImgLine.b)+ImgLine.c;

			ImgPt2.y = WndRoiRect.bottom;
			ImgPt2.x = (ImgPt2.y*ImgLine.b)+ImgLine.c;

			ImgPt1.x += BoxStartX;	ImgPt1.y += BoxStartY;
			ImgPt2.x += BoxStartX;	ImgPt2.y += BoxStartY;

			ModelPtr->CalcModelBoxPtPoint(ImgPt1, FrameImageW, FrameImageH, RgnCp, ImageScale, ImageCp, BoxPt2);
			ModelPtr->CalcModelBoxPtPoint(ImgPt2, FrameImageW, FrameImageH, RgnCp, ImageScale, ImageCp, BoxPt1);			

			double dx = BoxPt2.x-BoxPt1.x;
			double dy = BoxPt2.y-BoxPt1.y;
			if ( fabs(dy) < 0.00001 )
			{
				BoxLine.b = 0;
				BoxLine.c = (BoxPt1.x+BoxPt2.x)*0.5;
			}
			else
			{
				BoxLine.b = dx/dy;
				BoxLine.c = BoxPt1.x-(BoxLine.b*BoxPt1.y);
			}
		}
		else //y=ax+c
		{
			ImgPt1.x = WndRoiRect.left;
			ImgPt1.y = (ImgPt1.x*ImgLine.a)+ImgLine.c;

			ImgPt2.x = WndRoiRect.right;
			ImgPt2.y = (ImgPt2.x*ImgLine.a)+ImgLine.c;

			ImgPt1.x += BoxStartX;	ImgPt1.y += BoxStartY;
			ImgPt2.x += BoxStartX;	ImgPt2.y += BoxStartY;

			ModelPtr->CalcModelBoxPtPoint(ImgPt1, FrameImageW, FrameImageH, RgnCp, ImageScale, ImageCp, BoxPt1);
			ModelPtr->CalcModelBoxPtPoint(ImgPt2, FrameImageW, FrameImageH, RgnCp, ImageScale, ImageCp, BoxPt2);
			
			double dx = BoxPt2.x-BoxPt1.x;
			double dy = BoxPt2.y-BoxPt1.y;
			if ( fabs(dx) < 0.00001 )
			{
				BoxLine.a = 0;
				BoxLine.c = (BoxPt1.y+BoxPt2.y)*0.5;
			}
			else
			{
				BoxLine.a = dy/dx;
				BoxLine.c = BoxPt1.y-(BoxLine.a*BoxPt1.x);				
			}		
		}

		LineList.push_back(BoxLine);
		switch ( i )
		{
		case 1:
			amPartParam.amLineResultPtA_1 = BoxPt1;
			amPartParam.amLineResultPtB_1 = BoxPt2;
			break;
		case 2:
			amPartParam.amLineResultPtA_2 = BoxPt1;
			amPartParam.amLineResultPtB_2 = BoxPt2;
			break;
		case 3:
			amRefParam.amLineResultPtA_1 = BoxPt1;
			amRefParam.amLineResultPtB_1 = BoxPt2;
			break;
		case 4:
			amRefParam.amLineResultPtA_2 = BoxPt1;
			amRefParam.amLineResultPtB_2 = BoxPt2;
			break;
		}		
		BoxPtCenter.x = (BoxPt1.x + BoxPt2.x)*0.5;
		BoxPtCenter.y = (BoxPt1.y + BoxPt2.y)*0.5;
		CenterPtList.push_back(BoxPtCenter);
		WndRoiPtr->SetWndRoiImageRect(WndRoiRect);		
		str.Format(_T("Line[%d]=%.3fX+%.4fY+%.4f"), i+1, BoxLine.a, BoxLine.b, BoxLine.c);
		WndRoiPtr->SetWndRoiResultText(str);
		
		JetMemory.free_func(MaskPtr);
		JetMemory.free_func(GrayPtr);
	}
	
	JetAPI::ClearUniFrameList(WndUniFrameList);

	double Angle = 0;	
	const double Precise = 0.00001;
	const double Spec = msParam.msAngleSpec;
	const double USL = Spec+ msParam.msAngleTolUSL;
	const double LSL = Spec+ msParam.msAngleTolLSL;
	
	TPOINT2D BoxCenterPt1, BoxCenterPt2;
	TLineEquation2D LineTemp1, LineTemp2;
	if (InspectionEdgeCount == 1) { LineTemp1 = LineList[0]; }
	else {
		BoxCenterPt1.x = CenterPtList[0].x;
		BoxCenterPt1.y = CenterPtList[0].y;
		BoxCenterPt2.x = CenterPtList[1].x;
		BoxCenterPt2.y = CenterPtList[1].y;
		LineTemp1.a = BoxCenterPt1.y - BoxCenterPt2.y;
		LineTemp1.b = BoxCenterPt2.x - BoxCenterPt1.x;
		LineTemp1.c = -(LineTemp1.a*BoxCenterPt1.x + LineTemp1.b*BoxCenterPt1.y);
	}
	const size_t RefStartIndex = InspectionEdgeCount;
	
	if (RefEdgeCount == 0) { Angle = 0; }//沒有參考邊
	else if (RefEdgeCount == 1) { LineTemp2 = LineList[RefStartIndex]; }
	else {
		BoxCenterPt1.x = CenterPtList[RefStartIndex].x;
		BoxCenterPt1.y = CenterPtList[RefStartIndex].y;
		BoxCenterPt2.x = CenterPtList[RefStartIndex+1].x;
		BoxCenterPt2.y = CenterPtList[RefStartIndex+1].y;
		LineTemp2.a = BoxCenterPt1.y - BoxCenterPt2.y;
		LineTemp2.b = BoxCenterPt2.x - BoxCenterPt1.x;
		LineTemp2.c = -(LineTemp2.a*BoxCenterPt1.x + LineTemp2.b*BoxCenterPt1.y);
	}
	if (RefEdgeCount > 0) {
		Angle = ImageAPI.CalcLineIncludedAngle(LineTemp1, LineTemp2);
	}

	if ( Spec < (90.0-Precise) )//規格為銳角
	{	//結果為鈍角, 改算成銳角
		if ( Angle > 90.0 ) { Angle = 180.0-Angle; }
	}
	else if ( Spec > (90.0+Precise) )//規格為鈍角
	{	//結果為銳角, 改算成鈍角
		if ( Angle < 90.0 ) { Angle = 180.0-Angle; }
	}
	msParam.msAngleReading = Angle;

	//加入結果位置
	bool bVisible=true;
	RESULT_ID DistanceResultID=RESULT_ID_OK;	
	TLineEquation2D Line;
	TPOINT2D ImgPt, RefPt;
	for ( i=0; i<WndRoiCount; i++ )
	{
		WndRoiBoxPtr = WndPtr->GetWndResultBoxPtr(i, false);
		bVisible=true;
		if (i == 0) {
			WndRoiBoxPtr->SetBoxVisibled(bVisible);
			WndRoiBoxPtr->SetBoxResultID(MatchResultID);
			WndRoiBoxPtr->GetBoxRegion(WndRoiRegion);
			if (JetAPI::CalcRegionRect(WndRegion, RoiWndRect, WndRoiRegion, WndRoiRect, true) == false) //Cad和Image的Y是顛倒的
			{	continue;	}
			ImgPt.x = (WndRoiRect.left + WndRoiRect.right)*0.5;
			ImgPt.y = (WndRoiRect.top + WndRoiRect.bottom)*0.5;
			ImgPt.x += BoxStartX; ImgPt.y += BoxStartY;
			ModelPtr->CalcModelBoxPtPoint(ImgPt, FrameImageW, FrameImageH, RgnCp, ImageScale, ImageCp, RefPt);
			continue;
		}
		else if (i < InspectionEdgeCount + 1) {
			double *PtReadingPtr, PtSpec, PtTolUSL, PtTolLSL, PtDiff;
			if (i == 1) { 
				PtReadingPtr = &msParam.msPtAReading;
				PtSpec    = msParam.msPtASpec;
				PtTolUSL  = msParam.msPtATolUSL;
				PtTolLSL  = msParam.msPtATolLSL;
			}
			else { 
				PtReadingPtr = &msParam.msPtBReading;
				PtSpec   = msParam.msPtBSpec;
				PtTolUSL = msParam.msPtBTolUSL;
				PtTolLSL = msParam.msPtBTolLSL;
			}
			if (msParam.msDirection == ALG_VERTICAL) { //垂直距離
				*PtReadingPtr = fabs(CenterPtList[i - 1].y - RefPt.y);
			}
			else {	//水平距離
				*PtReadingPtr = fabs(CenterPtList[i - 1].x - RefPt.x);
			}
			PtDiff = *PtReadingPtr - PtSpec;
			if (PtDiff > PtTolUSL || -PtDiff > PtTolLSL) {
				WndRoiBoxPtr->SetBoxResultID(RESULT_ID_NG);
				DistanceResultID = RESULT_ID_NG;
			}
			else { WndRoiBoxPtr->SetBoxResultID(RESULT_ID_OK); }
		}
		
		Line = LineList.at(i - 1);
		switch ( i )
		{
		case 1:
			WndRoiBoxPtr->AddBoxPolygonPt(amPartParam.amLineResultPtA_1);
			WndRoiBoxPtr->AddBoxPolygonPt(amPartParam.amLineResultPtB_1);
			break;
		case 2:
			WndRoiBoxPtr->AddBoxPolygonPt(amPartParam.amLineResultPtA_2);
			WndRoiBoxPtr->AddBoxPolygonPt(amPartParam.amLineResultPtB_2);
			break;
		case 3:
			WndRoiBoxPtr->AddBoxPolygonPt(amRefParam.amLineResultPtA_1);
			WndRoiBoxPtr->AddBoxPolygonPt(amRefParam.amLineResultPtB_1);
			break;
		case 4:
			WndRoiBoxPtr->AddBoxPolygonPt(amRefParam.amLineResultPtA_2);
			WndRoiBoxPtr->AddBoxPolygonPt(amRefParam.amLineResultPtB_2);
			break;
		}
		if ( fabs(Line.a+1) < 0.00001 )//x=by+c
		{	str.Format(_T("Line[%d] X=%.4fY%+.4f"), i, Line.b, Line.c);	}
		else
		{	str.Format(_T("Line[%d] Y=%.4fX%+.4f"), i, Line.a, Line.c);	}

		WndRoiBoxPtr->SetBoxPolygonVisibled(true);
		WndRoiBoxPtr->SetBoxResultText(str);
		WndRoiBoxPtr->SetBoxVisibled(bVisible);
		WndRoiBoxPtr->SetBoxResultTextVisibled(bVisible);

	}
	CString strResult;
	RESULT_ID ResultID = RESULT_ID_OK;
	if (Angle > USL || Angle < LSL || MatchResultID == RESULT_ID_NG	|| DistanceResultID == RESULT_ID_NG)
	{	ResultID = RESULT_ID_NG;	}
	if( msParam.msRefEdgeCount>0 ){
		strResult.Format(_T("Angle:%.2f(%.2f~%.2f)"), Angle, LSL, USL);
		SetAlgResultReading1(Angle);
		SetAlgResultText(strResult);
	}
	else {
		strResult.Format(_T("Distance:A:%.2f,B:%.2f"), msParam.msPtAReading, msParam.msPtBReading);
		SetAlgResultReading1(msParam.msPtAReading);
		SetAlgResultReading2(msParam.msPtBReading);
		SetAlgResultText(strResult);
	}
	SetAlgResultID(ResultID);;
	WndPtr->SetWndResultID(m_AlgResultID);
	WndPtr->SetWndResultText(m_AlgResultText);	
	//SaveAlgDefectImage(MaskW, MaskH, MaskStep, MaskBitCount, GrayPtr, MaskPtr, WndRect);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_AngleMeasureTolUSL(const TALG_PARAM_MEASURE_SIP &Param)
{
	double Diff = Param.msAngleReading - Param.msAngleSpec;
	if (Diff > Param.msAngleTolUSL) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_AngleMeasureTolLSL(const TALG_PARAM_MEASURE_SIP &Param)
{
	double Diff = Param.msAngleReading - Param.msAngleSpec;
	if (Diff < Param.msAngleTolLSL) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::WriteAlgParamFile_MeasureSIP(const TALG_PARAM_MEASURE_SIP &Param, CAOIFileIO &FileIO) {
	// 儲存量測 SIP
	if (false == FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_MEASURE_SIP_START, 0)) return false;

	// 檢測邊數量
	if (false == FileIO.SaveChunk_INT(FILE_IO_ALG_MEASURE_SIP_INSPEC_EDGE_COUNT, Param.msInspecEdgeCount)) return false;
	// 參考邊數量
	if (false == FileIO.SaveChunk_INT(FILE_IO_ALG_MEASURE_SIP_REF_EDGE_COUNT, Param.msRefEdgeCount)) return false;
	// 計算方向
	if (false == FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_MEASURE_SIP_DIRECTION, Param.msDirection)) return false;
	// 角度規格與公差
	if (false == FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_MEASURE_SIP_ANGLE_SPEC, Param.msAngleSpec)) return false;
	if (false == FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_MEASURE_SIP_ANGLE_TOLERANCE_USL, Param.msAngleTolUSL)) return false;
	if (false == FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_MEASURE_SIP_ANGLE_TOLERANCE_LSL, Param.msAngleTolLSL)) return false;

	// A 點距離
	if (false == FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_MEASURE_SIP_DISTANCE_TO_A_SPEC, Param.msPtASpec)) return false;
	if (false == FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_MEASURE_SIP_DISTANCE_TO_A_TOLERANCE_USL, Param.msPtATolUSL)) return false;
	if (false == FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_MEASURE_SIP_DISTANCE_TO_A_TOLERANCE_LSL, Param.msPtATolLSL)) return false;

	// B 點距離
	if (false == FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_MEASURE_SIP_DISTANCE_TO_B_SPEC, Param.msPtBSpec)) return false;
	if (false == FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_MEASURE_SIP_DISTANCE_TO_B_TOLERANCE_USL, Param.msPtBTolUSL)) return false;
	if (false == FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_MEASURE_SIP_DISTANCE_TO_B_TOLERANCE_LSL, Param.msPtBTolLSL)) return false;

	// 結束標記
	if (false == FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_MEASURE_SIP_END, 0)) return false;


	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ReadAlgParamFile_MeasureSIP(TALG_PARAM_MEASURE_SIP &Param, CAOIFileIO &FileIO) {
	// 讀取量測 SIP
	int index = 0;
	while (false == FileIO.CheckFileEnd())
	{
		if (false == FileIO.LoadChunk(index))
		{
			continue;
		}

		switch (index)
		{
		case FILE_IO_ALG_PARAM_MEASURE_SIP_DIRECTION: // 計算方向
			Param.msDirection = ALG_DIRECTION(FileIO.GetData_INT());
			break;
		case FILE_IO_ALG_MEASURE_SIP_INSPEC_EDGE_COUNT:// 檢測邊數量
			Param.msInspecEdgeCount = FileIO.GetData_INT();
			break;
		case FILE_IO_ALG_MEASURE_SIP_REF_EDGE_COUNT:// 參考邊數量
			Param.msRefEdgeCount = FileIO.GetData_INT();
			break;
		case FILE_IO_ALG_PARAM_MEASURE_SIP_ANGLE_SPEC: // 角度規格
			Param.msAngleSpec = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_MEASURE_SIP_ANGLE_TOLERANCE_USL: // 角度上限
			Param.msAngleTolUSL = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_MEASURE_SIP_ANGLE_TOLERANCE_LSL: // 角度下限
			Param.msAngleTolLSL = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_MEASURE_SIP_DISTANCE_TO_A_SPEC: // A 點距離
			Param.msPtASpec = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_MEASURE_SIP_DISTANCE_TO_A_TOLERANCE_USL: // A 點距離上限
			Param.msPtATolUSL = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_MEASURE_SIP_DISTANCE_TO_A_TOLERANCE_LSL: // A 點距離下限
			Param.msPtATolLSL = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_MEASURE_SIP_DISTANCE_TO_B_SPEC: // B 點距離
			Param.msPtBSpec = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_MEASURE_SIP_DISTANCE_TO_B_TOLERANCE_USL: // B 點距離上限
			Param.msPtBTolUSL = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_MEASURE_SIP_DISTANCE_TO_B_TOLERANCE_LSL: // B 點距離下限
			Param.msPtBTolLSL = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_MEASURE_SIP_END: // 結束標記
			return true;
		default://v1.01.04.061
		#ifdef _DEBUG
			index = index;
		#endif//_DEBUG
			break;
		}
	}
	FileIO.SetErrorString(_T("Error, ReadAlgParamFile_MeasureSIP Fault"));
	return false;
}
//-------------------------------------------------------------------------------------//
