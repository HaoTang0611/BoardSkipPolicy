// AlgParam_SolderWetting.cpp: implementation of the CAlgParam class.
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
#include "JetBlob.h"
#include "JetMatch.h"
#include "JetBarcode.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_SolderWettingSectorAngle(const TALG_PARAM_SOLDER_WETTING &Param)
{
	if ( CheckOK_SolderWettingSectorAngleUSL(Param) == false ) { return false; }
	if ( CheckOK_SolderWettingSectorAngleLSL(Param) == false ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_SolderWettingSectorAngleUSL(const TALG_PARAM_SOLDER_WETTING &Param)
{
	if ( Param.swReadingCircleAngle > Param.swCircleAngleUSL ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_SolderWettingSectorAngleLSL(const TALG_PARAM_SOLDER_WETTING &Param)
{
	if ( Param.swReadingCircleAngle < Param.swCircleAngleLSL ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::WriteAlgParamFile_SolderWetting(const TALG_PARAM_SOLDER_WETTING &swParam, CAOIFileIO &FileIO)//儲存焊接檢測參數
{	
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_SOLDER_WETTING_START, 0) == false ) { return false; }	
	if ( FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_SOLDER_WETTING_CIRCLE_ANGLE_ENB, swParam.swCircleAngleEnabled) == false ) { return false; }	
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_SOLDER_WETTING_CIRCLE_ANGLE_USL, swParam.swCircleAngleUSL) == false ) { return false; }	
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_SOLDER_WETTING_CIRCLE_ANGLE_LSL, swParam.swCircleAngleLSL) == false ) { return false; }	
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_SOLDER_WETTING_CIRCLE_ANGLE_LINE_RATIO, swParam.swCircleAngleLinePassRatio) == false ) { return false; }		
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_SOLDER_WETTING_CIRCLE_ANGLE_LINE_START, swParam.swCircleAngleLineStartRatio) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_SOLDER_WETTING_END, 0) == false ) { return false; }	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ReadAlgParamFile_SolderWetting(TALG_PARAM_SOLDER_WETTING &swParam, CAOIFileIO &FileIO)//載入焊接檢測參數
{	
	int       index = 0;
	while ( FileIO.CheckFileEnd()==false )
	{ 		
		if ( FileIO.LoadChunk(index) == false )		
		{	continue; }
		switch ( index )
		{
		case FILE_IO_ALG_PARAM_SOLDER_WETTING_CIRCLE_ANGLE_ENB://環繞角度啟用
			swParam.swCircleAngleEnabled = FileIO.GetData_BOL();
			break;			
		case FILE_IO_ALG_PARAM_SOLDER_WETTING_CIRCLE_ANGLE_USL://環繞角度上限
			swParam.swCircleAngleUSL = (float)(FileIO.GetData_DBL());
			break;
		case FILE_IO_ALG_PARAM_SOLDER_WETTING_CIRCLE_ANGLE_LSL://環繞角度下限
			swParam.swCircleAngleLSL = (float)(FileIO.GetData_DBL());
			break;
		case FILE_IO_ALG_PARAM_SOLDER_WETTING_CIRCLE_ANGLE_LINE_RATIO://環繞角度半徑長度比例
			swParam.swCircleAngleLinePassRatio = (float)(FileIO.GetData_DBL());
			break;
		case FILE_IO_ALG_PARAM_SOLDER_WETTING_CIRCLE_ANGLE_LINE_START://環繞角度半徑開始比例
			swParam.swCircleAngleLineStartRatio = (float)(FileIO.GetData_DBL());
			break;
		case FILE_IO_ALG_PARAM_SOLDER_WETTING_END://焊接檢測-終點
			return true;		
		default:
		#ifdef _DEBUG
			index = index;
		#endif//_DEBUG
			break;		
		}
	}
	FileIO.SetErrorString(_T("Error, ReadAlgParamFile_SolderWetting Fault"));
	return false;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ExecAlgInspection_SolderWetting(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList, bool bTestWnd)	
{	
	if ( NULL == ModelPtr ) { return false; }	
	CAOIComponent *ComponentPtr = ModelPtr->GetModelComponentPtr();
	const size_t FrameCount = UniFrameList.size();		
	const size_t ImageFrameIndex = m_AlgImageBinParam.GetBinaryFrameIndex();
	if ( ImageFrameIndex >= FrameCount ) 
	{	return false; }
	
	CAOIWnd         *WndPtr = CAlgParam::GetAlgWndPtr();
	if ( NULL == WndPtr ) { return false; }		
	const unsigned int WndIndex = WndPtr->GetWndIndex();	
	TUNI_FRAME      *UniFramePtr  = &(UniFrameList[ImageFrameIndex]);
	const IMAGE_SIZE FrameImageW    = UniFramePtr->ImageW;
	const IMAGE_SIZE FrameImageH    = UniFramePtr->ImageH;
	const IMAGE_SIZE FrameImageStep = UniFramePtr->ImageStep;
	const IMAGE_SIZE FrameBitCount  = UniFramePtr->BitCount;		
	IMAGE_PTR        FrameImagePtr  = UniFramePtr->ImagePtr;
	MASK_PTR         FrameMaskPtr   = UniFramePtr->MaskPtr;
	SPACE_PTR        FrameSpacePtr  = UniFramePtr->SpacePtr;
	TPOINT2D         ImageScale     = ModelPtr->GetModelImageScale();
	TALG_PARAM_SOLDER_WETTING &swParam=GetAlgParamSolderWetting();
	const int WndW=WndRect.right-WndRect.left;
	const int WndH=WndRect.bottom-WndRect.top;
	const int RoiW = RoiRect.right-RoiRect.left;
	const int RoiH = RoiRect.bottom-RoiRect.top;
	const int RoiRectSize = (int)((RoiRect.right-RoiRect.left)*(RoiRect.bottom-RoiRect.top));
	
	IMAGE_SIZE    MaskW=0;
	IMAGE_SIZE    MaskH=0;	
	IMAGE_SIZE    MaskStep=0;
	IMAGE_SIZE    MaskBitCount=8;
	MASK_PTR      MaskPtr  = NULL;
	IMAGE_PTR     GrayPtr = NULL;		
	if ( ExecAlgUniFrameBinary(m_AlgImageBinParam, WndRect, RoiRect, UniFrameList, MaskW, MaskH, MaskStep, MaskBitCount, MaskPtr, GrayPtr, bTestWnd) == false )
	{		
		JetMemory.free_func(MaskPtr);
		JetMemory.free_func(GrayPtr);
		return false; 
	}	

	//因為有計算比例, 所以計算的區域與2值化區域是分開的, 
	MASK_PTR      ShapePtr  = NULL;
	IMAGE_SIZE    ShapeW=FrameImageW;
	IMAGE_SIZE    ShapeH=FrameImageH;			
	IMAGE_SIZE    ShapeBitCount=8;
	IMAGE_SIZE    ShapeStep=JetAPI::GetBMPImagePixelsPerLine(ShapeW, ShapeBitCount, 4);
	if ( WndPtr->BuildWndShapeMask(ShapeW, ShapeH, ShapeStep, ShapePtr, RoiRect) == false )
	{
		JetMemory.free_func(MaskPtr);
		JetMemory.free_func(GrayPtr);
		return false;
	}	
	if ( ExecAlgUniFrameMaskBinary(ModelPtr, m_AlgMaskBinParam, WndRect, RoiRect, UniFrameList, ShapeW, ShapeH, ShapeStep, ShapeBitCount, ShapePtr, bTestWnd) == false )
	{
		JetMemory.free_func(MaskPtr);
		JetMemory.free_func(GrayPtr);
		JetMemory.free_func(ShapePtr);
		return false; 
	}		

#ifdef _DEBUG	
	bool         bSave = true;
	CString      ComponentName;
	CString      DebugFolder=GetAlgDebugFolder();
	if ( NULL != ComponentPtr )
	{	ComponentName = ComponentPtr->GetComponentFullName(); }		
	if ( true == bSave )
	{	
		CString str;
		str.Format(_T("%s\\%s_ModelWndAlgSolderWetting#%d_Gray.PNG"), DebugFolder, ComponentName, WndIndex+1);
		ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, MaskBitCount, GrayPtr, true);

		str.Format(_T("%s\\%s_ModelWndAlgSolderWetting#%d_Mask.PNG"), DebugFolder, ComponentName, WndIndex+1);
		ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, MaskBitCount, MaskPtr, true);

		str.Format(_T("%s\\%s_ModelWndAlgSolderWetting#%d_ShapeMask.PNG"), DebugFolder, ComponentName, WndIndex+1);
		ImageAPI.SaveImage(str, ShapeW, ShapeH, ShapeStep, ShapeBitCount, ShapePtr, true);		
	}
#endif//_DEBUG	

	//扇形角度檢測
	if ( true == swParam.swCircleAngleEnabled )
	{
		POINT Pt;
		TPOINT2D Pt2D;
		bool bTemp=false;
		int nWhite=0, nCount=0;
		int LineCount=0, LinePass=0;
		double LinePassRatio=0.0;				
		const double CircleRadius=MIN(WndW, WndH)*0.5;
		const double AngleTanMin=atan(1/CircleRadius)*RAD_TO_DEG_DBL;//計算最小角度
		const double AngleStep=AngleTanMin*0.5;
		const double CircleCpX=(WndRect.left+WndRect.right)*0.5;
		const double CircleCpY=(WndRect.top+WndRect.bottom)*0.5;
		const double RadiusPassRatio=swParam.swCircleAngleLinePassRatio;
		const double RadiusStartRatio=swParam.swCircleAngleLineStartRatio;
		const int    RadiusStartLine=(int)(CircleRadius*RadiusStartRatio/100.0);
		std::vector<TRESULT_CIRCLE_ANGLE> &ResultCircleAngleRadius=swParam.swResultCircleAngleRadius;	
		for ( double i=0; i<360.000001; i+=AngleStep )
		{			
			const double AngleDeg=(i);
			const double AngleImg=JetAPI::MapCadAngleToImageAngle(AngleDeg);			
			const double AngleRad=AngleImg*DEG_TO_RAD_DBL;
			const double COSA=::cos(AngleRad);
			const double SINA=::sin(AngleRad);

			nCount=0;
			nWhite=0;
			LinePassRatio=0.0;
			for ( double j=0; j<CircleRadius-1; j++ )
			{
				if ( j < RadiusStartLine )
				{
					nWhite ++;
					nCount ++;
					continue;
				}

				Pt2D.x = j*COSA;
				Pt2D.y = j*SINA;
				Pt2D.x += CircleCpX;
				Pt2D.y += CircleCpY;
				Pt.x = JetAPI::ToInt(Pt2D.x);
				Pt.y = JetAPI::ToInt(Pt2D.y);

				size_t idx=(Pt.y*MaskStep)+Pt.x;
				if ( MaskPtr[idx] != 0 )
				{	nWhite ++;	}
				else
				{	bTemp = true;	}
				nCount ++;
			}
			if ( nCount > 0 )
			{	LinePassRatio = (nWhite*100.0)/nCount;	}
			if ( LinePassRatio >= RadiusPassRatio )
			{	LinePass ++;	}
			else
			{	bTemp = true;	}
			LineCount ++;

			if ( true==bTestWnd )
			{
				TRESULT_CIRCLE_ANGLE CircleAngleLine;
				CircleAngleLine.caAngle = (float)(AngleDeg);			
				CircleAngleLine.caRadiusPassResult = (float)(LinePassRatio);
				ResultCircleAngleRadius.push_back(CircleAngleLine);
			}
		}
		LinePassRatio=0.0;
		if ( LineCount > 0 )
		{	
			LinePassRatio = (LinePass*100.0)/LineCount;	
			LinePassRatio *= 3.60;
		}
		swParam.swReadingCircleAngle = LinePassRatio;

		swParam.swCircleAngleCpX = (float)(CircleCpX);
		swParam.swCircleAngleCpY = (float)(CircleCpY);
		swParam.swCircleAngleRadius = (float)(CircleRadius);		
		swParam.swCircleAngleRadiusStart = (float)(CircleRadius*RadiusStartRatio/100.0);	
	}
	
	CString strResult;	
	double USL=0.0, LSL=0.0, Ave=0.0;
	SetAlgResultReading1(swParam.swReadingCircleAngle);	
	SetAlgResultText(_T("OK"));

	//Judge OK/NG	
	SetAlgResultID(RESULT_ID_OK);
	if ( true == swParam.swCircleAngleEnabled )
	{
		USL = swParam.swCircleAngleUSL;
		LSL = swParam.swCircleAngleLSL;
		Ave = swParam.swReadingCircleAngle;
		if ( Ave>USL || Ave<LSL )
		{	
			CString Key;
			Key = _T("C-Angle");
			strResult.Format(_T("%s:%.2f (%.2f~%.2f)"), Key, Ave, LSL, USL);
			SetAlgResultID(RESULT_ID_NG);		
			SetAlgResultText(strResult);
		}	
	}
	

	WndPtr->SetWndResultID(m_AlgResultID);
	WndPtr->SetWndResultText(m_AlgResultText);	
	SaveAlgDefectImage(MaskW, MaskH, MaskStep, MaskBitCount, GrayPtr, MaskPtr, WndRect);
	JetMemory.free_func(GrayPtr);
	JetMemory.free_func(MaskPtr);
	JetMemory.free_func(ShapePtr);	
	return true;
}
//-------------------------------------------------------------------------------------//