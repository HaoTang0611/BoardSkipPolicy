// AlgParam_BrightRatio.cpp: implementation of the CAlgParam class.
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
bool CAlgParam::CheckOK_BrightRatioTolerance(const TALG_PARAM_BRIGHT_RATIO &Param)
{
	if ( CheckOK_BrightRatioToleranceUSL(Param) == false )
	{	return false; }
	if ( CheckOK_BrightRatioToleranceLSL(Param) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_BrightRatioToleranceUSL(const TALG_PARAM_BRIGHT_RATIO &Param)
{
	if ( Param.brToleranceReading > (Param.brToleranceUSL) ) 
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_BrightRatioToleranceLSL(const TALG_PARAM_BRIGHT_RATIO &Param)
{
	if ( Param.brToleranceReading < (Param.brToleranceLSL) ) 
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_BrightRatioLimit(const TALG_PARAM_BRIGHT_RATIO &Param)
{
	if ( CheckOK_BrightRatioLimitMin(Param) == false )
	{	return false; }
	if ( CheckOK_BrightRatioLimitMax(Param) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_BrightRatioLimitMin(const TALG_PARAM_BRIGHT_RATIO &Param)
{
	if ( Param.brLimitReadingMin < (Param.brLimitTolLSL) ) 
	{	return false; }
	if ( Param.brLimitReadingMin > (Param.brLimitTolUSL) ) 
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_BrightRatioLimitMax(const TALG_PARAM_BRIGHT_RATIO &Param)
{
	if ( Param.brLimitReadingMax < (Param.brLimitTolLSL) ) 
	{	return false; }
	if ( Param.brLimitReadingMax > (Param.brLimitTolUSL) ) 
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_BrightRatioRatio(const TALG_PARAM_BRIGHT_RATIO &Param)
{
	if ( CheckOK_BrightRatioRatioUSL(Param) == false )
	{	return false; }
	if ( CheckOK_BrightRatioRatioLSL(Param) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_BrightRatioRatioUSL(const TALG_PARAM_BRIGHT_RATIO &Param)
{
	if ( Param.brRatioReading > Param.brRatioUSL ) 
	{	return false; }	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_BrightRatioRatioLSL(const TALG_PARAM_BRIGHT_RATIO &Param)
{
	if ( Param.brRatioReading < Param.brRatioLSL ) 
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_BrightRatioRange(const TALG_PARAM_BRIGHT_RATIO &Param)
{
	if ( CheckOK_BrightRatioRangeUSL(Param) == false )
	{	return false; }
	if ( CheckOK_BrightRatioRangeLSL(Param) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_BrightRatioRangeUSL(const TALG_PARAM_BRIGHT_RATIO &Param)
{
	if ( Param.brRangeReading > Param.brRangeUSL ) 
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_BrightRatioRangeLSL(const TALG_PARAM_BRIGHT_RATIO &Param)
{
	if ( Param.brRangeReading < Param.brRangeLSL ) 
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_BrightRatioContrast(const TALG_PARAM_BRIGHT_RATIO &Param)
{
	if ( CheckOK_BrightRatioContrastUSL(Param) == false )
	{	return false; }
	if ( CheckOK_BrightRatioContrastLSL(Param) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_BrightRatioContrastUSL(const TALG_PARAM_BRIGHT_RATIO &Param)
{
	if ( Param.brContrastReading > Param.brContrastUSL ) 
	{	return false; }	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_BrightRatioContrastLSL(const TALG_PARAM_BRIGHT_RATIO &Param)
{
	if ( Param.brContrastReading < Param.brContrastLSL ) 
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_BrightRatioLineX(const TALG_PARAM_BRIGHT_RATIO &Param)
{
	if ( CheckOK_BrightRatioLineXUSL(Param) == false )
	{	return false; }
	if ( CheckOK_BrightRatioLineXLSL(Param) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_BrightRatioLineXUSL(const TALG_PARAM_BRIGHT_RATIO &Param)
{
	if ( Param.brXLineReading > Param.brXLineUSL ) 
	{	return false; }		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_BrightRatioLineXLSL(const TALG_PARAM_BRIGHT_RATIO &Param)
{
	if ( Param.brXLineReading < Param.brXLineLSL ) 
	{	return false; }	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_BrightRatioLineY(const TALG_PARAM_BRIGHT_RATIO &Param)
{
	if ( CheckOK_BrightRatioLineYUSL(Param) == false )
	{	return false; }
	if ( CheckOK_BrightRatioLineYLSL(Param) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_BrightRatioLineYUSL(const TALG_PARAM_BRIGHT_RATIO &Param)
{
	if ( Param.brYLineReading > Param.brYLineUSL ) 
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_BrightRatioLineYLSL(const TALG_PARAM_BRIGHT_RATIO &Param)
{
	if ( Param.brYLineReading < Param.brYLineLSL ) 
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::WriteAlgParamFile_BrightRatio(const TALG_PARAM_BRIGHT_RATIO &brParam, CAOIFileIO &FileIO)//儲存亮度比例參數
{	
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_BRIGHT_RATIO_START, 0) == false ) { return false; }

	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_BRIGHT_RATIO_TARGET, brParam.brTargetValue) == false ) { return false; }
	
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_BRIGHT_RATIO_RATIO_USL, brParam.brRatioUSL) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_BRIGHT_RATIO_RATIO_LSL, brParam.brRatioLSL) == false ) { return false; }	

	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_BRIGHT_RATIO_AVERAGE_SCALE, brParam.brAverageScale) == false ) { return false; }
	if ( FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_BRIGHT_RATIO_AVERAGE_SCALE_ENB, brParam.brAverageScaleEnabled) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_BRIGHT_RATIO_AVERAGE_MODE, brParam.brAverageMode) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_BRIGHT_RATIO_AVE_PARTIAL_H, brParam.brAveragePartialH) == false ) { return false; }	
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_BRIGHT_RATIO_AVE_PARTIAL_L, brParam.brAveragePartialL) == false ) { return false; }	
	
	if ( FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_BRIGHT_RATIO_RANGE_ENABLED, brParam.brRangeEnabled) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_BRIGHT_RATIO_RANGE_USL, brParam.brRangeUSL) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_BRIGHT_RATIO_RANGE_LSL, brParam.brRangeLSL) == false ) { return false; }

	if ( FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_BRIGHT_RATIO_ROI_BOX_ENB, brParam.brRoiBoxEnabled) == false ) { return false; }	
	if ( FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_BRIGHT_RATIO_RATIO_ENABLED, brParam.brRatioEnabled) == false ) { return false; }	

	if ( FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_BRIGHT_RATIO_CONTRAST_ENABLED, brParam.brContrastEnabled) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_BRIGHT_RATIO_CONTRAST_USL, brParam.brContrastUSL) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_BRIGHT_RATIO_CONTRAST_LSL, brParam.brContrastLSL) == false ) { return false; }

	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_BRIGHT_RATIO_XLINE_RANGE, brParam.brXLineRange) == false ) { return false; }
	if ( FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_BRIGHT_RATIO_XLINE_ENABLED, brParam.brXLineEnabled) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_BRIGHT_RATIO_XLINE_USL, brParam.brXLineUSL) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_BRIGHT_RATIO_XLINE_LSL, brParam.brXLineLSL) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_BRIGHT_RATIO_XLINE_MODE, brParam.brXLineMode) == false ) { return false; }		
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_BRIGHT_RATIO_XLINE_UNIT_MODE, brParam.brXLineUnitMode) == false ) { return false; }

	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_BRIGHT_RATIO_YLINE_RANGE, brParam.brYLineRange) == false ) { return false; }
	if ( FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_BRIGHT_RATIO_YLINE_ENABLED, brParam.brYLineEnabled) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_BRIGHT_RATIO_YLINE_USL, brParam.brYLineUSL) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_BRIGHT_RATIO_YLINE_LSL, brParam.brYLineLSL) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_BRIGHT_RATIO_YLINE_MODE, brParam.brYLineMode) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_BRIGHT_RATIO_YLINE_UNIT_MODE, brParam.brYLineUnitMode) == false ) { return false; }

	if ( FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_BRIGHT_RATIO_TOLERANCE_ENABLED, brParam.brToleranceEnabled) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_BRIGHT_RATIO_TOLERANCE_USL, brParam.brToleranceUSL) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_BRIGHT_RATIO_TOLERANCE_LSL, brParam.brToleranceLSL) == false ) { return false; }	

	if ( FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_BRIGHT_RATIO_LIMIT_MAX_ENB, brParam.brLimitMaxEnabled) == false ) { return false; }
	if ( FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_BRIGHT_RATIO_LIMIT_MIN_ENB, brParam.brLimitMinEnabled) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_BRIGHT_RATIO_LIMIT_TOL_USL, brParam.brLimitTolUSL) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_BRIGHT_RATIO_LIMIT_TOL_LSL, brParam.brLimitTolLSL) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_BRIGHT_RATIO_LIMIT_MAX_SCALE, brParam.brLimitMaxScale) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_BRIGHT_RATIO_LIMIT_MIN_SCALE, brParam.brLimitMinScale) == false ) { return false; }
	if ( FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_BRIGHT_RATIO_LIMIT_MAX_SCALE_ENB, brParam.brLimitMaxScaleEnabled) == false ) { return false; }
	if ( FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_BRIGHT_RATIO_LIMIT_MIN_SCALE_ENB, brParam.brLimitMinScaleEnabled) == false ) { return false; }

	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_BRIGHT_RATIO_END, 0) == false ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ReadAlgParamFile_BrightRatio(TALG_PARAM_BRIGHT_RATIO &brParam, CAOIFileIO &FileIO)//載入亮度比例參數
{
	int       index = 0;
	int       nValue = 0;
	while ( FileIO.CheckFileEnd()==false )
	{ 		
		if ( FileIO.LoadChunk(index) == false )		
		{	continue; }
		switch ( index )
		{
		case FILE_IO_ALG_PARAM_BRIGHT_RATIO_END://亮度比例參數-終點
			return true;
			break;
		case FILE_IO_ALG_PARAM_BRIGHT_RATIO_TARGET://目標高度
			brParam.brTargetValue = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_BRIGHT_RATIO_RATIO_USL://比例上限
			brParam.brRatioUSL = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_BRIGHT_RATIO_RATIO_LSL://比例下限
			brParam.brRatioLSL = FileIO.GetData_DBL();
			break;		

		case FILE_IO_ALG_PARAM_BRIGHT_RATIO_AVERAGE_SCALE://平均比例
			brParam.brAverageScale = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_BRIGHT_RATIO_AVERAGE_SCALE_ENB://平均比例啟用
			brParam.brAverageScaleEnabled = FileIO.GetData_BOL();
			break;
		case FILE_IO_ALG_PARAM_BRIGHT_RATIO_AVERAGE_MODE://目標模式
			brParam.brAverageMode = (ALG_BRIGHT_AVERAGE_MODE)(FileIO.GetData_INT());
			break;
		case FILE_IO_ALG_PARAM_BRIGHT_RATIO_AVE_PARTIAL_H://平均範圍上限
			brParam.brAveragePartialH = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_BRIGHT_RATIO_AVE_PARTIAL_L://平均範圍下限
			brParam.brAveragePartialL = FileIO.GetData_DBL();
			break;		
		case FILE_IO_ALG_PARAM_BRIGHT_RATIO_RANGE_ENABLED://高低差啟用
			brParam.brRangeEnabled = FileIO.GetData_BOL();
			break;
		case FILE_IO_ALG_PARAM_BRIGHT_RATIO_RANGE_USL://高低差上限
			brParam.brRangeUSL = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_BRIGHT_RATIO_RANGE_LSL://高低差下限
			brParam.brRangeLSL = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_BRIGHT_RATIO_ROI_BOX_ENB://子框使用
			brParam.brRoiBoxEnabled = FileIO.GetData_BOL();
			break;
		case FILE_IO_ALG_PARAM_BRIGHT_RATIO_RATIO_ENABLED://比例啟用
			brParam.brRatioEnabled = FileIO.GetData_BOL();
			break;		
		case FILE_IO_ALG_PARAM_BRIGHT_RATIO_CONTRAST_ENABLED://對比啟用
			brParam.brContrastEnabled = FileIO.GetData_BOL();	
			break;
		case FILE_IO_ALG_PARAM_BRIGHT_RATIO_CONTRAST_USL://對比上限
			brParam.brContrastUSL = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_BRIGHT_RATIO_CONTRAST_LSL://對比下限
			brParam.brContrastLSL = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_BRIGHT_RATIO_XLINE_RANGE://X軸貫穿縱向範圍
			brParam.brXLineRange = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_BRIGHT_RATIO_XLINE_ENABLED://X軸貫穿縱向啟用
			brParam.brXLineEnabled = FileIO.GetData_BOL();
			break;
		case FILE_IO_ALG_PARAM_BRIGHT_RATIO_XLINE_USL://X軸貫穿上限
			brParam.brXLineUSL = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_BRIGHT_RATIO_XLINE_LSL://X軸貫穿下限
			brParam.brXLineLSL = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_BRIGHT_RATIO_XLINE_MODE://X軸貫穿模式
			nValue = FileIO.GetData_INT();
			switch ( nValue ) 
			{
			case LINE_MODE_DARK:
			case LINE_MODE_BRIGHT:
				brParam.brXLineMode = nValue;
				break;
			}
			break;
		case FILE_IO_ALG_PARAM_BRIGHT_RATIO_XLINE_UNIT_MODE://X軸貫穿單位模式
			nValue = FileIO.GetData_INT();
			switch ( nValue ) 
			{
			case ALG_CALC_UNIT_ABS:
			case ALG_CALC_UNIT_RATIO:
				brParam.brXLineUnitMode = (ALG_CALC_UNIT_MODE)(nValue);
				break;
			}
			break;

		case FILE_IO_ALG_PARAM_BRIGHT_RATIO_YLINE_RANGE://Y軸貫穿縱向範圍
			brParam.brYLineRange = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_BRIGHT_RATIO_YLINE_ENABLED://Y軸貫穿縱向啟用
			brParam.brYLineEnabled = FileIO.GetData_BOL();
			break;
		case FILE_IO_ALG_PARAM_BRIGHT_RATIO_YLINE_USL://Y軸貫穿上限
			brParam.brYLineUSL = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_BRIGHT_RATIO_YLINE_LSL://Y軸貫穿下限
			brParam.brYLineLSL = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_BRIGHT_RATIO_YLINE_MODE://Y軸貫穿模式
			nValue = FileIO.GetData_INT();
			switch ( nValue ) 
			{
			case LINE_MODE_DARK:
			case LINE_MODE_BRIGHT:
				brParam.brYLineMode = nValue;
				break;
			}
			break;
		case FILE_IO_ALG_PARAM_BRIGHT_RATIO_YLINE_UNIT_MODE://Y軸貫穿單位模式
			nValue = FileIO.GetData_INT();
			switch ( nValue ) 
			{
			case ALG_CALC_UNIT_ABS:
			case ALG_CALC_UNIT_RATIO:
				brParam.brYLineUnitMode = (ALG_CALC_UNIT_MODE)(nValue);
				break;
			}
			break;

		case FILE_IO_ALG_PARAM_BRIGHT_RATIO_TOLERANCE_ENABLED://公差啟用
			brParam.brToleranceEnabled = FileIO.GetData_BOL();
			break;
		case FILE_IO_ALG_PARAM_BRIGHT_RATIO_TOLERANCE_USL://公差上限
			brParam.brToleranceUSL = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_BRIGHT_RATIO_TOLERANCE_LSL://公差下限
			brParam.brToleranceLSL = FileIO.GetData_DBL();
			break;

		case FILE_IO_ALG_PARAM_BRIGHT_RATIO_LIMIT_MAX_ENB://最大值啟用
			brParam.brLimitMaxEnabled = FileIO.GetData_BOL();
			break;
		case FILE_IO_ALG_PARAM_BRIGHT_RATIO_LIMIT_MIN_ENB://最小值啟用
			brParam.brLimitMinEnabled = FileIO.GetData_BOL();
			break;
		case FILE_IO_ALG_PARAM_BRIGHT_RATIO_LIMIT_TOL_USL://極值公差上限
			brParam.brLimitTolUSL = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_BRIGHT_RATIO_LIMIT_TOL_LSL://極值公差下限
			brParam.brLimitTolLSL = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_BRIGHT_RATIO_LIMIT_MAX_SCALE://最大值比例
			brParam.brLimitMaxScale = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_BRIGHT_RATIO_LIMIT_MIN_SCALE://最小值比例
			brParam.brLimitMinScale = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_BRIGHT_RATIO_LIMIT_MAX_SCALE_ENB://最大值比例啟用
			brParam.brLimitMaxScaleEnabled = FileIO.GetData_BOL();
			break;
		case FILE_IO_ALG_PARAM_BRIGHT_RATIO_LIMIT_MIN_SCALE_ENB://最小值比例啟用
			brParam.brLimitMinScaleEnabled = FileIO.GetData_BOL();
			break;

		default:
		#ifdef _DEBUG
			index = index;
		#endif//_DEBUG
			break;
		}
	}
	FileIO.SetErrorString(_T("Error, ReadAlgParamFile_BrightRatio Fault"));
	return false;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ExecAlgInspection_BrightRatio(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList)
{	
	if ( NULL == ModelPtr ) { return false; }	
	CAOIComponent *ComponentPtr = ModelPtr->GetModelComponentPtr();
	const size_t FrameCount = UniFrameList.size();	
	const size_t MaskFrameIndex = m_AlgMaskBinParam.GetBinaryFrameIndex();
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
	TALG_PARAM_BRIGHT_RATIO &brParam=CAlgParam::GetAlgParamBrightRatio();
	//const int RoiW = RoiRect.right-RoiRect.left;
	//const int RoiH = RoiRect.bottom-RoiRect.top;
	//const int RoiRectSize = (int)((RoiRect.right-RoiRect.left)*(RoiRect.bottom-RoiRect.top));
	
	IMAGE_SIZE    MaskW=0;
	IMAGE_SIZE    MaskH=0;	
	IMAGE_SIZE    MaskStep=0;
	IMAGE_SIZE    MaskBitCount=8;
	MASK_PTR      MaskPtr  = NULL;
	IMAGE_PTR     GrayPtr = NULL;	
	const bool    bTestWnd = false;
	if ( ExecAlgUniFrameBinary(m_AlgImageBinParam, WndRect, RoiRect, UniFrameList, MaskW, MaskH, MaskStep, MaskBitCount, MaskPtr, GrayPtr, bTestWnd) == false )
	{		
		JetMemory.free_func(MaskPtr);
		JetMemory.free_func(GrayPtr);
		return false; 
	}
	if ( BINARY_DISABLE!=m_AlgMaskBinParam.GetBinaryMode() && CheckAlgMaskBinFrameUsed()==true )
	{
		IMAGE_SIZE    MaskW2=0;
		IMAGE_SIZE    MaskH2=0;	
		IMAGE_SIZE    MaskStep2=0;
		IMAGE_SIZE    MaskBitCount2=8;
		MASK_PTR      MaskPtr2  = NULL;
		IMAGE_PTR     GrayPtr2 = NULL;		

		if ( ExecAlgUniFrameBinary(m_AlgMaskBinParam, WndRect, RoiRect, UniFrameList, MaskW2, MaskH2, MaskStep2, MaskBitCount2, MaskPtr2, GrayPtr2, bTestWnd) == false )
		{			
			JetMemory.free_func(MaskPtr);
			JetMemory.free_func(GrayPtr);
			
			JetMemory.free_func(MaskPtr2);
			JetMemory.free_func(GrayPtr2);
			return false; 
		}

		if ( MaskW!=MaskW2 || MaskH!=MaskH2 || MaskStep!=MaskStep2 )
		{			
			JetMemory.free_func(MaskPtr);
			JetMemory.free_func(GrayPtr);
			
			JetMemory.free_func(MaskPtr2);
			JetMemory.free_func(GrayPtr2);
			return false;
		}

		MASK_FUNC_MODE  MaskFuncMode=m_AlgMaskBinParam.GetMaskFuncMode();
		if ( MASK_FUNC_ERASE == MaskFuncMode )
		{	ImageAPI.InvertMaskImage3(MaskW2, MaskH2, MaskStep2, MaskPtr2);		}

		//if ( ImageAPI.Union2MaskImage3(MaskW, MaskH, MaskStep, MaskPtr, MaskPtr2, RoiRect, MaskStep, MaskPtr, 255, 0, true) == false )
		if ( ImageAPI.MergeMaskImage3(MaskW, MaskH, MaskStep, MaskPtr, MaskPtr2, RoiRect, MERGE_MASK_AND, MaskPtr) == false ) 
		{			
			JetMemory.free_func(MaskPtr);
			JetMemory.free_func(GrayPtr);
			
			JetMemory.free_func(MaskPtr2);
			JetMemory.free_func(GrayPtr2);
			return false;
		}		
		JetMemory.free_func(MaskPtr2);
		JetMemory.free_func(GrayPtr2);
	}

	MASK_PTR      ShapePtr  = NULL;
	IMAGE_SIZE    ShapeW=FrameImageW;
	IMAGE_SIZE    ShapeH=FrameImageH;			
	IMAGE_SIZE    ShapeBitCount=8;
	IMAGE_SIZE    ShapeStep=JetAPI::GetBMPImagePixelsPerLine(ShapeW, ShapeBitCount, 4);		
	const size_t  ShapeBufferSize = ImageAPI.CalcBufferSize(ShapeStep, ShapeH);
	if ( JetMemory.alloc_func(ShapeBufferSize, ShapePtr, "CAlgParam::ExecAlgInspection_BrightRatio", "ShapePtr") == false )
	{	
		JetMemory.free_func(MaskPtr);
		JetMemory.free_func(GrayPtr);
		return false; 
	}	
	if ( WndPtr->CheckWndNeedShapeMask() == false )
	{	ImageAPI.FillImageRoi(ShapeW, ShapeH, ShapeStep, ShapeBitCount, ShapePtr, RoiRect, 255, 255, 255);		}
	else
	{
		ImageAPI.FillImageRoi(ShapeW, ShapeH, ShapeStep, ShapeBitCount, ShapePtr, RoiRect, 0, 0, 0);
		if ( WndPtr->BuildWndShapeMask3(ShapeW, ShapeH, ShapeStep, ShapePtr, RoiRect) == false )
		{			
			JetMemory.free_func(MaskPtr);
			JetMemory.free_func(GrayPtr);
			JetMemory.free_func(ShapePtr);
			return false;
		}
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
		str.Format(_T("%s\\%s_ModelWndAlgBrightRatio#%d_Gray.PNG"), DebugFolder, ComponentName, WndIndex+1);
		ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, MaskBitCount, GrayPtr, true);

		str.Format(_T("%s\\%s_ModelWndAlgBrightRatio#%d_Mask.PNG"), DebugFolder, ComponentName, WndIndex+1);
		ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, MaskBitCount, MaskPtr, true);

		str.Format(_T("%s\\%s_ModelWndAlgBrightRatio#%d_ShapeMask.PNG"), DebugFolder, ComponentName, WndIndex+1);
		ImageAPI.SaveImage(str, ShapeW, ShapeH, ShapeStep, ShapeBitCount, ShapePtr, true);
	}
#endif//_DEBUG

	//brRoiBoxEnabled
	RECT CalcRect = RoiRect;	
	CAOIWndRoi *WndRoiPtr = WndPtr->GetWndRoiWndPtr(0, true);
	if ( NULL != WndRoiPtr )
	{
		TREGION4D WndRegion;
		TREGION4D WndRoiRegion;
		RECT WndRoiRect={0,0,0,0};			
		WndPtr->GetWndRegion(WndRegion);			
		WndRoiPtr->GetWndRoiRegion(WndRoiRegion);
		if ( JetAPI::CalcRegionRect(WndRegion, RoiRect, WndRoiRegion, WndRoiRect, true) == true ) //Cad和Image的Y是顛倒的
		{
			if ( (WndRoiRect.right!=WndRoiRect.left) && (WndRoiRect.bottom!=WndRoiRect.top) )
			{	CalcRect = WndRoiRect;	}
		}
	}

	double Ave = 0.0, Value=0, Value2=0, USL=0.0, LSL=0.0;
	double Sum=0, Sum2=0, Max=0, Min=0, Std=0;	
	double Weighting = 1.0;	
	size_t i=0, j=0, k=0;
	unsigned int  Count=0;
	unsigned int  index=0;	
	const double  TargetValue = brParam.brTargetValue;	
	const double  SpaceLevel = 0;	
	const int CalcRoiW = CalcRect.right-CalcRect.left;
	const int CalcRoiH = CalcRect.bottom-CalcRect.top;
	const int CalcRectSize = (int)((CalcRect.right-CalcRect.left)*(CalcRect.bottom-CalcRect.top));
	std::vector<int>  ValueList(CalcRectSize);
	const size_t MaxGrayValue=256;
	int GrayCountList[MaxGrayValue]={0};	
	const BINARY_MODE AlgBinaryMode = m_AlgImageBinParam.GetBinaryMode();

	ValueList.clear();
	//Calc Average 
	::memset(GrayCountList, 0x00, sizeof(GrayCountList));
	if ( NULL == FrameSpacePtr )
	{	
		for ( i=CalcRect.top; i<CalcRect.bottom; i++ )
		{
			index = i*MaskStep;
			for ( j=CalcRect.left; j<CalcRect.right; j++ )
			{
				if ( 0 == MaskPtr[index+j] ) { continue; }
				if ( 0 == ShapePtr[index+j] ) { continue; }
				GrayCountList[GrayPtr[index+j]] ++;
				Value = GrayPtr[index+j];
				Value2 = Value*Value;
				if ( 0 == Count ) 
				{	Max = Min = Value;	}
				else
				{
					if ( Value > Max ) { Max = Value; }
					if ( Value < Min ) { Min = Value; }
				}
				Sum += Value;
				Sum2 += Value2;
				ValueList.push_back((int)(Value));
				Count ++;
			}
		}		
	}
	else
	{	
		for ( i=CalcRect.top; i<CalcRect.bottom; i++ )
		{
			index = i*MaskStep;
			for ( j=CalcRect.left; j<CalcRect.right; j++ )
			{
				if ( 0 == MaskPtr[index+j] ) { continue; }
				if ( 0 == ShapePtr[index+j] ) { continue; }
				if ( ImageAPI.CheckSpaceMaskValid(FrameMaskPtr[index+j]) == false ) { continue; }
				Value = FrameSpacePtr[index+j]-SpaceLevel;
				Value2 = Value*Value;
				if ( 0 == Count ) 
				{	Max = Min = Value;	}
				else
				{
					if ( Value > Max ) { Max = Value; }
					if ( Value < Min ) { Min = Value; }
				}				
				Sum += Value;
				Sum2 += Value2;
				ValueList.push_back((int)(Value));
				Count ++;
			}
		}			
	}

	const size_t ValueListSize = ValueList.size();
	ALG_BRIGHT_AVERAGE_MODE AverageMode = brParam.brAverageMode;	
	if ( Count > 0 ) 
	{		
		size_t CalCntL=0;
		size_t CalCntU=ValueListSize;
		if ( ALG_BRIGHT_AVERAGE_PARTIAL == AverageMode )
		{
			double     AverageRatioUSL = brParam.brAveragePartialH;
			double     AverageRatioLSL = brParam.brAveragePartialL;
			if ( AverageRatioUSL > 100 ) { AverageRatioUSL = 100; }
			else if ( AverageRatioUSL < 0 ) { AverageRatioUSL = 0; }
			if ( AverageRatioLSL > 100 ) { AverageRatioLSL = 100; }
			else if ( AverageRatioLSL < 0 ) { AverageRatioLSL = 0; }
			CalCntU = (size_t)(AverageRatioUSL*ValueListSize)/100.0;
			CalCntL = (size_t)(AverageRatioLSL*ValueListSize)/100.0;
			if ( CalCntU > ValueListSize ) { CalCntU = (size_t)(ValueListSize); }
			if ( CalCntL > ValueListSize ) { CalCntL = (size_t)(ValueListSize); }			
		}
		const size_t CalCnt = CalCntU-CalCntL;		
		if ( CalCnt > 0 ) 
		{
			Max = 0;
			Min = 0;				
			int   PartialSize = 1;		
			if ( NULL != FrameSpacePtr )
			{
				PartialSize = 5*CalCnt/100;//計算最高與最低 5%的平均值 Less to More			
				if ( PartialSize < 1 ) { PartialSize = 1; }			
			}
			int SelfCount=0;
			int PartialCnt=0;

			Sum = Sum2 = 0;
			std::sort(ValueList.begin(), ValueList.end());
			for ( i=CalCntL; i<CalCntU; i++ )
			{
				Value = ValueList[i];
				Value2 = Value*Value;				
				Sum += Value;
				Sum2 += Value2;

				if ( PartialCnt < PartialSize )
				{
					Min += ValueList[i];
					Max += ValueList[CalCntU-PartialCnt-1];
					PartialCnt ++;
				}
			}			
			Ave = Sum/CalCnt;
			Std = (Sum2/CalCnt)-(Ave*Ave);
			Std = ::sqrt(Std);
			if ( PartialSize > 0 ) 
			{
				Max = Max/PartialSize;
				Min = Min/PartialSize;
			}

			if ( brParam.brAverageScaleEnabled )
			{	Ave *= brParam.brAverageScale/100.0;	}
			if ( brParam.brLimitMinScaleEnabled )
			{	Min *= brParam.brLimitMinScale/100.0;	}
			if ( brParam.brLimitMaxScaleEnabled )
			{	Max *= brParam.brLimitMaxScale/100.0;	}

			double dAve = Ave;
			int nAve = (int)(Ave+0.5);			
			brParam.brAverageReading = dAve;	
			brParam.brAverageReadingMin = Min;	
			brParam.brAverageReadingMax = Max;
			brParam.brRangeReading = Max-Min;
			brParam.brContrastReading = Std;
			brParam.brToleranceReading = dAve-TargetValue;
			brParam.brLimitReadingMin = Min-TargetValue;
			brParam.brLimitReadingMax = Max-TargetValue;
		}

		const double BaseValueReading = GetAlgBaseValueReading();
		const bool   bBaseValueEnabled = GetAlgBaseValueEnabled();		
		if ( true == bBaseValueEnabled )
		{
			Min = Min-BaseValueReading;
			Max = Max-BaseValueReading;
			Ave = Ave-BaseValueReading;
			brParam.brAverageReading = Ave;			
			brParam.brAverageReadingMin = Min;	
			brParam.brAverageReadingMax = Max;
			brParam.brToleranceReading = Ave-TargetValue;
			brParam.brLimitReadingMin = Min-TargetValue;
			brParam.brLimitReadingMax = Max-TargetValue;
		}
	}
	else
	{
		brParam.brAverageReading = 0;	
		brParam.brAverageReadingMin = 0;	
		brParam.brAverageReadingMax = 0;
		brParam.brRangeReading = 0;
		brParam.brContrastReading = 0;
		brParam.brToleranceReading = -TargetValue;
		brParam.brLimitReadingMin = -TargetValue;
		brParam.brLimitReadingMax = -TargetValue;
	}

	//Calc Ratio
	if ( BINARY_DISABLE != AlgBinaryMode )	
	{
		Sum = 0.0;
		Count = 0;		
		for ( i=CalcRect.top; i<CalcRect.bottom; i++ )
		{
			index = i*MaskStep;
			for ( j=CalcRect.left; j<CalcRect.right; j++ )
			{
				if ( 0 == ShapePtr[index+j] ) { continue; }
				Count ++;
				if ( 0 == MaskPtr[index+j] ) { continue; }				
				Sum ++;
			}
		}
		//Count = (CalcRect.right-CalcRect.left)*(CalcRect.bottom-CalcRect.top);		
		if ( Count > 0 ) 
		{	Ave = 100.0*Sum/Count;	}
		
		double ReadingW = Ave;
		double ReadingB = 100.0-ReadingW;
		double Contrast = 100.0*((100.0-fabs(ReadingW-ReadingB))/(ReadingW+ReadingB));
		brParam.brContrastReading = Contrast;
		brParam.brRatioArea = Sum/(ImageScale.x*ImageScale.y);
	}
	else
	{
		if ( ::fabs(TargetValue)>0.01 )
		{	Ave = 100.0*Ave/TargetValue;	}
		else
		{	Ave = 0.0;	}
	}
	brParam.brRatioReading = Ave;		
	
	if ( true == brParam.brXLineEnabled )
	{		
		const int XLineMode = brParam.brXLineMode;
		const double XLineRange = brParam.brXLineRange;		
		int   nXLineRange = JetAPI::Floor(XLineRange*ImageScale.y);
		if ( nXLineRange < 1 ) { nXLineRange = 1; }		
		if ( nXLineRange > CalcRoiH ) { nXLineRange = CalcRoiH; }
		
		int LineCount=0;
		int SumLineCount=0;
		int MaxLineCount = 0;		
		unsigned char PassChar = 0;
		if ( LINE_MODE_DARK == XLineMode )
		{	PassChar = 255; }
		else
		{	PassChar = 0; }
		for ( i=CalcRect.top; i<CalcRect.bottom-nXLineRange+1; i++ )
		{			
			LineCount = 0;
			for ( j=CalcRect.left; j<CalcRect.right; j++ )
			{
				for ( k=i; k<i+nXLineRange; k++ )
				{
					index = k*MaskStep;
					//if ( 0 == ShapePtr[index+j] ) { continue; }
					if ( PassChar == MaskPtr[index+j] ) { continue; }				
					LineCount ++;
					break;
				}
			}
			if ( MaxLineCount < LineCount ) { MaxLineCount = LineCount; }
		}
		if ( MaxLineCount > CalcRoiW ) { MaxLineCount = CalcRoiW; }
		brParam.brXLineReading = MaxLineCount;
		switch ( brParam.brXLineUnitMode )
		{
		case ALG_CALC_UNIT_ABS:	brParam.brXLineReading /= ImageScale.x;		break;
		default:
		case ALG_CALC_UNIT_RATIO:	brParam.brXLineReading = 100.0*brParam.brXLineReading/CalcRoiW;	break;
		}	
	}

	if ( true == brParam.brYLineEnabled )
	{		
		const int YLineMode = brParam.brYLineMode;
		const double YLineRange = brParam.brYLineRange;
		int   nYLineRange = JetAPI::Floor(YLineRange*ImageScale.x);
		if ( nYLineRange < 1 ) { nYLineRange = 1; }		
		if ( nYLineRange > CalcRoiW ) { nYLineRange = CalcRoiW; }

		int LineCount=0;
		int MaxLineCount = 0;
		unsigned char PassChar = 0;
		if ( LINE_MODE_DARK == YLineMode )
		{	PassChar = 255; }
		else
		{	PassChar = 0; }
		for ( j=CalcRect.left; j<CalcRect.right-nYLineRange+1; j++ )
		{			
			LineCount = 0;			
			for ( i=CalcRect.top; i<CalcRect.bottom; i++ )
			{
				index = i*MaskStep;
				for ( k=j; k<j+nYLineRange; k++ )
				{	
					//if ( 0 == ShapePtr[index+j] ) { continue; }
					if ( PassChar == MaskPtr[index+k] ) { continue; }				
					LineCount ++;
					break;
				}
			}
			if ( MaxLineCount < LineCount ) { MaxLineCount = LineCount; }
		}
		if ( MaxLineCount > CalcRoiH ) { MaxLineCount = CalcRoiH; }
		brParam.brYLineReading = MaxLineCount;
		switch ( brParam.brYLineUnitMode )
		{
		case ALG_CALC_UNIT_ABS:	brParam.brYLineReading /= ImageScale.y;		break;
		default:
		case ALG_CALC_UNIT_RATIO:	brParam.brYLineReading = 100.0*brParam.brYLineReading/CalcRoiH;	break;
		}
	}	
	
	CString strResult;	
	SetAlgResultReading1(Ave);	
	SetAlgResultText(_T("OK"));

	//Judge OK/NG	
	SetAlgResultID(RESULT_ID_OK);

	USL = brParam.brToleranceUSL;
	LSL = brParam.brToleranceLSL;
	Ave = brParam.brToleranceReading;
	if ( true == brParam.brToleranceEnabled )
	{
		if ( Ave>USL || Ave<LSL )
		{	
			CString Key;
			const double Spec = brParam.brTargetValue;
			if ( NULL == FrameSpacePtr )
			{	
				Key = AOIDataDefine.GetGrayText();	
				strResult.Format(_T("%s:%.0f (%.0f~%.0f)"), Key, Ave, LSL, USL);	
			}
			else
			{	
				Key = AOIDataDefine.GetThicknessText();	
				strResult.Format(_T("%s:%.0fum (%.0f~%.0f)"), Key, Ave+Spec, LSL+Spec, USL+Spec);	
			}			
			SetAlgResultID(RESULT_ID_NG);		
			SetAlgResultText(strResult);
		}	
	}

	//Limit-Max
	if ( true == brParam.brLimitMaxEnabled )
	{
		USL = brParam.brLimitTolUSL;
		LSL = brParam.brLimitTolLSL;
		Ave = brParam.brLimitReadingMax;
		if ( Ave>USL || Ave<LSL )
		{	
			CString Key;
			const double Spec = brParam.brTargetValue;
			if ( NULL == FrameSpacePtr )
			{	
				Key = AOIDataDefine.GetGrayText();	
				strResult.Format(_T("Max-%s:%.0f (%.0f~%.0f)"), Key, Ave, LSL, USL);	
			}
			else
			{	
				Key = AOIDataDefine.GetThicknessText();	
				strResult.Format(_T("Max-%s:%.0fum (%.0f~%.0f)"), Key, Ave+Spec, LSL+Spec, USL+Spec);	
			}			
			SetAlgResultID(RESULT_ID_NG);		
			SetAlgResultText(strResult);
		}	
	}

	//Limit-Min
	if ( true == brParam.brLimitMinEnabled )
	{
		USL = brParam.brLimitTolUSL;
		LSL = brParam.brLimitTolLSL;
		Ave = brParam.brLimitReadingMin;
		if ( Ave>USL || Ave<LSL )
		{	
			CString Key;
			const double Spec = brParam.brTargetValue;
			if ( NULL == FrameSpacePtr )
			{	
				Key = AOIDataDefine.GetGrayText();	
				strResult.Format(_T("Min-%s:%.0f (%.0f~%.0f)"), Key, Ave, LSL, USL);	
			}
			else
			{	
				Key = AOIDataDefine.GetThicknessText();	
				strResult.Format(_T("Min-%s:%.0fum (%.0f~%.0f)"), Key, Ave+Spec, LSL+Spec, USL+Spec);	
			}			
			SetAlgResultID(RESULT_ID_NG);		
			SetAlgResultText(strResult);
		}	
	}

	if ( true == brParam.brRatioEnabled )
	{
		USL = brParam.brRatioUSL;
		LSL = brParam.brRatioLSL;
		Ave = brParam.brRatioReading;		
		if ( Ave>USL || Ave<LSL )
		{	
			CString Key;						
			Key = AOIDataDefine.GetRatioText();
			strResult.Format(_T("%s:%.0f (%.0f~%.0f)"), Key, Ave, LSL, USL);				
			SetAlgResultID(RESULT_ID_NG);		
			SetAlgResultText(strResult);
		}
	}

	if ( true == brParam.brRangeEnabled )
	{
		USL = brParam.brRangeUSL;
		LSL = brParam.brRangeLSL;
		Ave = brParam.brRangeReading;
		if ( Ave>USL || Ave<LSL )
		{	
			CString Key;
			Key = AOIDataDefine.GetRangeText();
			strResult.Format(_T("%s:%.0f (%.0f~%.0f)"), Key, Ave, LSL, USL);
			SetAlgResultID(RESULT_ID_NG);		
			SetAlgResultText(strResult);
		}
	}
	
	if ( true == brParam.brContrastEnabled )
	{	
		USL = brParam.brContrastUSL;
		LSL = brParam.brContrastLSL;
		Ave = brParam.brContrastReading;
		if ( Ave>USL || Ave<LSL )
		{	
			CString Key;
			Key = AOIDataDefine.GetContrastText();
			strResult.Format(_T("%s:%.0f (%.0f~%.0f)"), Key, Ave, LSL, USL);
			SetAlgResultID(RESULT_ID_NG);
			SetAlgResultText(strResult);
		}
	}

	if ( true == brParam.brXLineEnabled )
	{
		USL = brParam.brXLineUSL;
		LSL = brParam.brXLineLSL;
		Ave = brParam.brXLineReading;
		if ( Ave>USL || Ave<LSL )
		{	
			CString Key;
			Key = AOIDataDefine.GetThroughText();
			strResult.Format(_T("X-%s:%.0f (%.0f~%.0f)"), Key, Ave, LSL, USL);
			SetAlgResultID(RESULT_ID_NG);
			SetAlgResultText(strResult);
		}
	}
	if ( true == brParam.brYLineEnabled )
	{
		USL = brParam.brYLineUSL;
		LSL = brParam.brYLineLSL;
		Ave = brParam.brYLineReading;
		if ( Ave>USL || Ave<LSL )
		{	
			CString Key;
			Key = AOIDataDefine.GetThroughText();
			strResult.Format(_T("Y-%s:%.0f (%.0f~%.0f)"), Key, Ave, LSL, USL);
			SetAlgResultID(RESULT_ID_NG);
			SetAlgResultText(strResult);
		}
	}

	//加入結果位置	
	if ( NULL != WndRoiPtr )
	{	
		CAOIBox  ResBox;
		CAOIBox *WndRoiBoxPtr=WndRoiPtr->GetWndRoiBoxPtr();		
		ResBox = *WndRoiBoxPtr;
		ResBox.SetBoxVisibled(true);		
		//ResBox.SetBoxResultTextVisibled(true);
		ResBox.SetBoxResultID(GetAlgResultID());
		WndPtr->AddWndResultBox(ResBox);
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