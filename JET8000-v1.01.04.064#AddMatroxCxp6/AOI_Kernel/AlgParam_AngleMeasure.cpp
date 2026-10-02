// AlgParam_AngleMeasure.cpp: implementation of the CAlgParam class.
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
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_AngleMeasureTolUSL(const TALG_PARAM_ANGLE_MEASURE &Param)
{
	double Diff=Param.amAngleReading-Param.amAngleSpec;
	if ( Diff > Param.amAngleTolUSL ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_AngleMeasureTolLSL(const TALG_PARAM_ANGLE_MEASURE &Param)
{
	double Diff=Param.amAngleReading-Param.amAngleSpec;
	if ( Diff < Param.amAngleTolLSL ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::WriteAlgParamFile_AngleMeasure(const TALG_PARAM_ANGLE_MEASURE &amParam, CAOIFileIO &FileIO)//纗à秖代把计	
{
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_ANGLE_MEASURE_START, 0) == false ) { return false; }
	
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_ANGLE_MEASURE_ANGLE_SPEC, amParam.amAngleSpec) == false ) { return false; }	
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_ANGLE_MEASURE_ANGLE_TOL_USL, amParam.amAngleTolUSL) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_ANGLE_MEASURE_ANGLE_TOL_LSL, amParam.amAngleTolLSL) == false ) { return false; }	
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_ANGLE_MEASURE_ANGLE_MODE, amParam.amAngleMode) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_ANGLE_MEASURE_BASE_LINE_MODE, amParam.amBaseLineMode) == false ) { return false; }

	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_ANGLE_MEASURE_END, 0) == false ) { return false; }	
	return true;

	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ReadAlgParamFile_AngleMeasure(TALG_PARAM_ANGLE_MEASURE &amParam, CAOIFileIO &FileIO)//更à秖代把计
{	
	int       index = 0;
	while ( FileIO.CheckFileEnd()==false )
	{ 		
		if ( FileIO.LoadChunk(index) == false )		
		{	continue; }
		switch ( index )
		{		
		case FILE_IO_ALG_PARAM_ANGLE_MEASURE_ANGLE_SPEC://à秖代把计-à砏
			amParam.amAngleSpec = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_ANGLE_MEASURE_ANGLE_TOL_USL://à秖代把计-そ畉
			amParam.amAngleTolUSL = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_ANGLE_MEASURE_ANGLE_TOL_LSL://à秖代把计-そ畉
			amParam.amAngleTolLSL = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_ANGLE_MEASURE_ANGLE_MODE://à秖代把计-à家Α
			amParam.amAngleMode = (ANGLE_MEASURE_MODE)(FileIO.GetData_INT());
			break;
		case FILE_IO_ALG_PARAM_ANGLE_MEASURE_BASE_LINE_MODE://à秖代把计-膀非絬家Α
			amParam.amBaseLineMode = (LINE_EQUATION_MODE)(FileIO.GetData_INT());
			break;
		case FILE_IO_ALG_PARAM_ANGLE_MEASURE_END://喷靡把计-沧翴
			return true;
		default:
		#ifdef _DEBUG
			index = index;
		#endif//_DEBUG
			break;		
		}
	}
	FileIO.SetErrorString(_T("Error, ReadAlgParamFile_AngleMeasure Fault"));
	return false;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ExecAlgInspection_AngleMeasure(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList)
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
	TALG_PARAM_ANGLE_MEASURE &amParam  = GetAlgParamAngleMeasure();	
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
	const bool    bTestWnd = false;
	TUNI_FRAME    WndUniFrame;
	std::vector<TUNI_FRAME> WndUniFrameList;

	bool CalcTiltAngle = false;
	if ( ANGLE_MEASURE_TILT == amParam.amAngleMode )
	{
		if ( NULL==FrameMaskPtr || NULL==FrameMaskPtr )
		{	return false; }
		CalcTiltAngle = true;
	}

#ifdef _DEBUG		
	bool         bSave = true;
	CString      ComponentName;
	CString      DebugFolder=GetAlgDebugFolder();
	if ( NULL != ComponentPtr )
	{	ComponentName = ComponentPtr->GetComponentFullName(); }		

	if ( true == bSave )
	{			
		str.Format(_T("%s\\%s_ModelWndAlgAngleMeasure#%d_Model.PNG"), DebugFolder, ComponentName, WndIndex+1);
		ImageAPI.SaveImage(str, FrameImageW, FrameImageH, FrameImageStep, FrameBitCount, FrameImagePtr, true);
	}
#endif//_DEBUG

	if ( BuildWndUniFrameList(UniFrameList, RoiRect, WndUniFrameList) == false )
	{	return false;	}	
	
	RECT RoiWndRect={0,0,0,0};	
	WndPtr->GetWndRegion(WndRegion);	
	JetAPI::SizeToRect(RoiW, RoiH, RoiWndRect);

	TPOINT2D  RgnCp;
	TPOINT2D  ImageCp;	
	TPOINT3D  TiltPt1;
	TPOINT3D  TiltPt2;
	TLineEquation2D Line1;
	TLineEquation2D Line2;	
	ImageCp.x = FrameImageW;
	ImageCp.y = FrameImageH;
	ImageCp.x = ImageCp.x*0.5;
	ImageCp.y = ImageCp.y*0.5;				
	RgnCp.x = ModelRgn.GetCpX();
	RgnCp.y = ModelRgn.GetCpY();	
	const int BoxStartX=RoiRect.left;
	const int BoxStartY=RoiRect.top;
	const size_t WndFrameCount = WndUniFrameList.size();	
	if ( true == CalcTiltAngle )
	{		
		TPOINT2D    BoxPosCad;
		MASK_PTR    BoxMaskPtr   = NULL;
		SPACE_PTR   BoxSpacePtr  = NULL;
		IMAGE_PTR   BoxImagePtr  = NULL;
		const char  fnName[]="CAlgParam::ExecAlgInspection_AngleMeasure";
		TUNI_FRAME *WndUniFramePtr  = &(WndUniFrameList[ImageFrameIndex]);
		IMAGE_SIZE  WndImageW    = WndUniFramePtr->ImageW;
		IMAGE_SIZE  WndImageH    = WndUniFramePtr->ImageH;
		IMAGE_SIZE  WndImageStep = WndUniFramePtr->ImageStep;
		IMAGE_SIZE  WndBitCount  = WndUniFramePtr->BitCount;		
		IMAGE_PTR   WndImagePtr  = WndUniFramePtr->ImagePtr;
		MASK_PTR    WndMaskPtr   = WndUniFramePtr->MaskPtr;
		SPACE_PTR   WndSpacePtr  = WndUniFramePtr->SpacePtr;

		for ( i=0; i<WndRoiCount; i++ )
		{
			WndRoiPtr = WndPtr->GetWndRoiWndPtr(i, false);
			if ( NULL == WndRoiPtr ) { continue; }
			WndRoiPtr->GetWndRoiRegion(WndRoiRegion);
			WndRoiPtr->GetWndRoiBox().GetBoxPosCadRes(BoxPosCad);
			if ( JetAPI::CalcRegionRect(WndRegion, RoiWndRect, WndRoiRegion, WndRoiRect, true) == false ) //Cad㎝ImageY琌腁
			{	continue;	}		

			double Height=0;
			RECT  LocRoiRect;
			const int RoiW=WndRoiRect.right-WndRoiRect.left;
			const int RoiH=WndRoiRect.bottom-WndRoiRect.top;
			const int RoiBitCount=(int)(WndBitCount);
			const int RoiStep=JetAPI::GetBMPImagePixelsPerLine(RoiW, RoiBitCount, 4);
			const size_t RoiBufSize=ImageAPI.CalcBufferSize(RoiStep, RoiH);
			JetAPI::SizeToRect(RoiW, RoiH, LocRoiRect);
			if ( JetMemory.alloc_func(RoiBufSize, BoxMaskPtr, fnName, "BoxMaskPtr") == false ||
				 JetMemory.alloc_func(RoiBufSize, BoxImagePtr, fnName, "BoxImagePtr") == false || 
				 JetMemory.alloc_func(RoiBufSize, BoxSpacePtr, fnName, "BoxSpacePtr") == false )
			{
				JetMemory.free_func(BoxMaskPtr);
				JetMemory.free_func(BoxSpacePtr);
				JetMemory.free_func(BoxImagePtr);
				return false;
			}

			if ( ImageAPI.ExtractRoiImage3(WndImageW, WndImageH, WndImageStep, WndBitCount, WndMaskPtr, WndRoiRect, RoiStep, BoxMaskPtr, false) == false )
			{
				JetMemory.free_func(BoxMaskPtr);
				JetMemory.free_func(BoxSpacePtr);
				JetMemory.free_func(BoxImagePtr);
				return false;
			}
			if ( ImageAPI.ExtractSpaceRoiImage3(WndImageW, WndImageH, WndImageStep, WndBitCount, WndSpacePtr, WndRoiRect, RoiStep, BoxSpacePtr, false) == false )
			{
				JetMemory.free_func(BoxMaskPtr);
				JetMemory.free_func(BoxSpacePtr);
				JetMemory.free_func(BoxImagePtr);
				return false;
			}
			
		#ifdef _DEBUG
			if ( ImageAPI.ExtractRoiImage3(WndImageW, WndImageH, WndImageStep, WndBitCount, WndImagePtr, WndRoiRect, RoiStep, BoxImagePtr, false) == false )
			{
				JetMemory.free_func(BoxMaskPtr);
				JetMemory.free_func(BoxSpacePtr);
				JetMemory.free_func(BoxImagePtr);
				return false;
			}
			if ( true == bSave )
			{			
				str.Format(_T("%s\\%s_ModelWndAlgAngleMeasure#%d_Roi#%d.PNG"), DebugFolder, ComponentName, WndIndex+1, i+1);
				//ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, MaskBitCount, GrayPtr, true);
				ImageAPI.SaveImage(str, RoiW, RoiH, RoiStep, RoiBitCount, BoxImagePtr, true);
			}
		#endif//_DEBUG

			ImageAPI.CalcSpaceImageAverage(RoiW, RoiH, RoiStep, BoxSpacePtr, BoxMaskPtr, LocRoiRect, Height); 			

			switch ( i )
			{
			case 0:
				TiltPt1 = BoxPosCad;
				TiltPt1.z = Height;
				amParam.amHeightA = Height;
				break;
			case 1:	
				TiltPt2 = BoxPosCad;
				TiltPt2.z = Height;
				amParam.amHeightB = Height;
				break;
			}	

			WndRoiPtr->SetWndRoiImageRect(WndRoiRect);		
			//WndRoiPtr->SetWndRoiResultValue(Ave);
			str.Format(_T("%.0f um"), Height);
			WndRoiPtr->SetWndRoiResultText(str);

			JetMemory.free_func(BoxMaskPtr);
			JetMemory.free_func(BoxSpacePtr);
			JetMemory.free_func(BoxImagePtr);
		}
	}
	else
	{
		for ( i=0; i<WndRoiCount; i++ )
		{
			WndRoiPtr = WndPtr->GetWndRoiWndPtr(i, false);
			if ( NULL == WndRoiPtr ) { continue; }		
			WndRoiPtr->GetWndRoiRegion(WndRoiRegion);
			if ( JetAPI::CalcRegionRect(WndRegion, RoiWndRect, WndRoiRegion, WndRoiRect, true) == false ) //Cad㎝ImageY琌腁
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
			TPOINT2D BoxPt1, BoxPt2;		
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
			else //y=ay+c
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

			switch ( i )
			{
			case 0:
				Line1 = BoxLine;
				amParam.amLineResultPtA_1 = BoxPt1;
				amParam.amLineResultPtB_1 = BoxPt2;
				break;
			case 1:
				Line2 = BoxLine;
				amParam.amLineResultPtA_2 = BoxPt1;
				amParam.amLineResultPtB_2 = BoxPt2;
				break;
			}		
			WndRoiPtr->SetWndRoiImageRect(WndRoiRect);		
			//WndRoiPtr->SetWndRoiResultValue(Ave);
			str.Format(_T("Line[%d]=%.3fX+%.4fY+%.4f"), i+1, BoxLine.a, BoxLine.b, BoxLine.c);
			WndRoiPtr->SetWndRoiResultText(str);
		
			JetMemory.free_func(MaskPtr);
			JetMemory.free_func(GrayPtr);
		}
	}
	JetAPI::ClearUniFrameList(WndUniFrameList);

	double Angle = 0;	
	const double Precise = 0.00001;
	const double Spec = amParam.amAngleSpec;
	const double USL = Spec+amParam.amAngleTolUSL;
	const double LSL = Spec+amParam.amAngleTolLSL;		
	if ( true == CalcTiltAngle )
	{	Angle = JetAPI::CalcAngle3D(TiltPt1, TiltPt2);	}
	else
	{
		Angle = ImageAPI.CalcLineIncludedAngle(Line1, Line2);	
		if ( Spec < (90.0-Precise) )//砏綰à
		{	//挡狦秝à, э衡Θ綰à
			if ( Angle > 90.0 ) { Angle = 180.0-Angle; }
		}
		else if ( Spec > (90.0+Precise) )//砏秝à
		{	//挡狦綰à, э衡Θ秝à
			if ( Angle < 90.0 ) { Angle = 180.0-Angle; }
		}
	}
	amParam.amAngleReading = Angle;	

	//挡狦竚
	bool bVisible=true;
	CAOIBox  ResBox;
	CAOIBox *WndRoiBoxPtr=NULL;
	for ( i=0; i<WndRoiCount; i++ )
	{
		WndRoiPtr = WndPtr->GetWndRoiWndPtr(i, false);
		if ( NULL == WndRoiPtr ) { continue; }
		bVisible=true;
		WndRoiBoxPtr = WndRoiPtr->GetWndRoiBoxPtr();
		ResBox = *WndRoiBoxPtr;

		if ( true == CalcTiltAngle )
		{
			switch ( i )
			{
			case 0:	str.Format(_T("%.0f um"), amParam.amHeightA);	break;
			case 1:	str.Format(_T("%.0f um"), amParam.amHeightB);	break;
			}
		}
		else
		{
			TLineEquation2D Line;
			switch ( i )
			{
			case 0:
				Line = Line1;
				ResBox.AddBoxPolygonPt(amParam.amLineResultPtA_1);
				ResBox.AddBoxPolygonPt(amParam.amLineResultPtB_1);
				break;
			case 1:
				Line = Line2;
				ResBox.AddBoxPolygonPt(amParam.amLineResultPtA_2);
				ResBox.AddBoxPolygonPt(amParam.amLineResultPtB_2);
				break;
			}
			if ( fabs(Line.a+1) < 0.00001 )//x=by+c
			{	str.Format(_T("Line[%d] X=%.4fY%+.4f"), i+1, Line.b, Line.c);	}
			else
			{	str.Format(_T("Line[%d] Y=%.4fX%+.4f"), i+1, Line.a, Line.c);	}

			if ( 0!=i || LINE_EQUATION_CALC==amParam.amBaseLineMode )
			{	ResBox.SetBoxPolygonVisibled(true);	}
			else
			{	bVisible = false;	}
		}

		ResBox.SetBoxResultText(str);		
		ResBox.SetBoxVisibled(bVisible);
		ResBox.SetBoxResultTextVisibled(bVisible);

		WndPtr->AddWndResultBox(ResBox);
		ResBox.ClearBoxPolygon();
		ResBox.SetBoxPolygonVisibled(false);
	}

	CString strResult;
	RESULT_ID ResultID=RESULT_ID_OK;	
	if ( Angle > USL || Angle < LSL )
	{	ResultID = RESULT_ID_NG;	}
	else
	{	ResultID = RESULT_ID_OK;	}

	SetAlgResultID(ResultID);
	SetAlgResultReading1(Angle);
	strResult.Format(_T("%.2f(%.2f~%.2f)"), Angle, LSL, USL);
	SetAlgResultText(strResult);

	WndPtr->SetWndResultID(m_AlgResultID);
	WndPtr->SetWndResultText(m_AlgResultText);	
	//SaveAlgDefectImage(MaskW, MaskH, MaskStep, MaskBitCount, GrayPtr, MaskPtr, WndRect);
	return true;
}
//-------------------------------------------------------------------------------------//