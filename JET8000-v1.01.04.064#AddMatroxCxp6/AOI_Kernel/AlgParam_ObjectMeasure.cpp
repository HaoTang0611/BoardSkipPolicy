// AlgParam_ObjectMeasure.cpp: implementation of the CAlgParam class.
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
bool CAlgParam::CheckOK_ObjectMeasureSizeX(const TALG_PARAM_OBJECT_MEASURE &Param)
{
	bool IsOK = true;	
	if ( ALG_CALC_UNIT_DIFF == Param.omSizeCalcUnitMode )
	{	IsOK = CheckOK_ObjectMeasureSizeXDiff(Param);	}
	else
	{	IsOK = CheckOK_ObjectMeasureSizeXRatio(Param);	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_ObjectMeasureSizeXDiff(const TALG_PARAM_OBJECT_MEASURE &Param)
{
	if ( CheckOK_ObjectMeasureSizeXDiffUSL(Param) == false )
	{	return false; }
	if ( CheckOK_ObjectMeasureSizeXDiffLSL(Param) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_ObjectMeasureSizeXDiffUSL(const TALG_PARAM_OBJECT_MEASURE &Param)
{
	if ( Param.omSizeXReading > (Param.omSizeXSpec+Param.omSizeXDiffUSL) )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_ObjectMeasureSizeXDiffLSL(const TALG_PARAM_OBJECT_MEASURE &Param)
{
	if ( Param.omSizeXReading < (Param.omSizeXSpec+Param.omSizeXDiffLSL) )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_ObjectMeasureSizeXRatio(const TALG_PARAM_OBJECT_MEASURE &Param)
{
	if ( CheckOK_ObjectMeasureSizeXRatioUSL(Param) == false ) 
	{	return false; }
	if ( CheckOK_ObjectMeasureSizeXRatioLSL(Param) == false ) 
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_ObjectMeasureSizeXRatioUSL(const TALG_PARAM_OBJECT_MEASURE &Param)
{
	double Ratio=0.0;
	if ( fabs(Param.omSizeXSpec) > 0.0001 )
	{	Ratio = 100.0*Param.omSizeXReading/Param.omSizeXSpec; }
	else
	{	Ratio = 0.0; }		
	if ( Ratio > Param.omSizeXRatioUSL )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_ObjectMeasureSizeXRatioLSL(const TALG_PARAM_OBJECT_MEASURE &Param)
{
	double Ratio=0.0;
	if ( fabs(Param.omSizeXSpec) > 0.0001 )
	{	Ratio = 100.0*Param.omSizeXReading/Param.omSizeXSpec; }
	else
	{	Ratio = 0.0; }		
	if ( Ratio < Param.omSizeXRatioLSL )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_ObjectMeasureSizeY(const TALG_PARAM_OBJECT_MEASURE &Param)
{
	bool IsOK = true;	
	if ( ALG_CALC_UNIT_DIFF == Param.omSizeCalcUnitMode )
	{	IsOK = CheckOK_ObjectMeasureSizeYDiff(Param);	}
	else
	{	IsOK = CheckOK_ObjectMeasureSizeYRatio(Param);	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_ObjectMeasureSizeYDiff(const TALG_PARAM_OBJECT_MEASURE &Param)
{
	if ( CheckOK_ObjectMeasureSizeYDiffUSL(Param) == false )
	{	return false; }
	if ( CheckOK_ObjectMeasureSizeYDiffLSL(Param) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_ObjectMeasureSizeYDiffUSL(const TALG_PARAM_OBJECT_MEASURE &Param)
{
	if ( Param.omSizeYReading > (Param.omSizeYSpec+Param.omSizeYDiffUSL) )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_ObjectMeasureSizeYDiffLSL(const TALG_PARAM_OBJECT_MEASURE &Param)
{
	if ( Param.omSizeYReading < (Param.omSizeYSpec+Param.omSizeYDiffLSL) )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_ObjectMeasureSizeYRatio(const TALG_PARAM_OBJECT_MEASURE &Param)
{
	if ( CheckOK_ObjectMeasureSizeYRatioUSL(Param) == false ) 
	{	return false; }
	if ( CheckOK_ObjectMeasureSizeYRatioLSL(Param) == false ) 
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_ObjectMeasureSizeYRatioUSL(const TALG_PARAM_OBJECT_MEASURE &Param)
{
	double Ratio=0.0;
	if ( fabs(Param.omSizeYSpec) > 0.0001 )
	{	Ratio = 100.0*Param.omSizeYReading/Param.omSizeYSpec; }
	else
	{	Ratio = 0.0; }		
	if ( Ratio > Param.omSizeYRatioUSL )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_ObjectMeasureSizeYRatioLSL(const TALG_PARAM_OBJECT_MEASURE &Param)
{
	double Ratio=0.0;
	if ( fabs(Param.omSizeYSpec) > 0.0001 )
	{	Ratio = 100.0*Param.omSizeYReading/Param.omSizeYSpec; }
	else
	{	Ratio = 0.0; }		
	if ( Ratio < Param.omSizeYRatioLSL )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_ObjectMeasureHeight(const TALG_PARAM_OBJECT_MEASURE &Param)
{
	bool IsOK = true;	
	if ( ALG_CALC_UNIT_DIFF == Param.omHeightCalcUnitMode )
	{	IsOK = CheckOK_ObjectMeasureHeightDiff(Param);	}
	else
	{	IsOK = CheckOK_ObjectMeasureHeightRatio(Param);	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_ObjectMeasureHeightDiff(const TALG_PARAM_OBJECT_MEASURE &Param)
{
	if ( CheckOK_ObjectMeasureHeightDiffUSL(Param) == false )
	{	return false; }
	if ( CheckOK_ObjectMeasureHeightDiffLSL(Param) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_ObjectMeasureHeightDiffUSL(const TALG_PARAM_OBJECT_MEASURE &Param)
{
	if ( Param.omHeightReading > (Param.omHeightSpec+Param.omHeightDiffUSL) )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_ObjectMeasureHeightDiffLSL(const TALG_PARAM_OBJECT_MEASURE &Param)
{
	if ( Param.omHeightReading < (Param.omHeightSpec+Param.omHeightDiffLSL) )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_ObjectMeasureHeightRatio(const TALG_PARAM_OBJECT_MEASURE &Param)
{
	if ( CheckOK_ObjectMeasureHeightRatioUSL(Param) == false ) 
	{	return false; }
	if ( CheckOK_ObjectMeasureHeightRatioLSL(Param) == false ) 
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_ObjectMeasureHeightRatioUSL(const TALG_PARAM_OBJECT_MEASURE &Param)
{
	double Ratio=0.0;
	if ( fabs(Param.omHeightSpec) > 0.0001 )
	{	Ratio = 100.0*Param.omHeightReading/Param.omHeightSpec; }
	else
	{	Ratio = 0.0; }		
	if ( Ratio > Param.omHeightRatioUSL )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_ObjectMeasureHeightRatioLSL(const TALG_PARAM_OBJECT_MEASURE &Param)
{
	double Ratio=0.0;
	if ( fabs(Param.omHeightSpec) > 0.0001 )
	{	Ratio = 100.0*Param.omHeightReading/Param.omHeightSpec; }
	else
	{	Ratio = 0.0; }		
	if ( Ratio < Param.omHeightRatioLSL )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_ObjectMeasureArea(const TALG_PARAM_OBJECT_MEASURE &Param)
{
	if ( CheckOK_ObjectMeasureAreaRatioUSL(Param) == false )
	{	return false; }
	if ( CheckOK_ObjectMeasureAreaRatioLSL(Param) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_ObjectMeasureAreaRatioUSL(const TALG_PARAM_OBJECT_MEASURE &Param)
{
	double Ratio=0.0;
	if ( fabs(Param.omAreaSpec) > 0.0001 )
	{	Ratio = 100.0*Param.omAreaReading/Param.omAreaSpec; }
	else
	{	Ratio = 0.0; }
	if ( Ratio > Param.omAreaUSL )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_ObjectMeasureAreaRatioLSL(const TALG_PARAM_OBJECT_MEASURE &Param)
{
	double Ratio=0.0;
	if ( fabs(Param.omAreaSpec) > 0.0001 )
	{	Ratio = 100.0*Param.omAreaReading/Param.omAreaSpec; }
	else
	{	Ratio = 0.0; }
	if ( Ratio < Param.omAreaLSL )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_ObjectMeasureVolume(const TALG_PARAM_OBJECT_MEASURE &Param)
{
	if ( CheckOK_ObjectMeasureVolumeRatioUSL(Param) == false )
	{	return false; }
	if ( CheckOK_ObjectMeasureVolumeRatioLSL(Param) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_ObjectMeasureVolumeRatioUSL(const TALG_PARAM_OBJECT_MEASURE &Param)
{
	double Ratio=0.0;
	if ( fabs(Param.omVolumeSpec) > 0.0001 )
	{	Ratio = 100.0*Param.omVolumeReading/Param.omVolumeSpec; }
	else
	{	Ratio = 0.0; }
	if ( Ratio > Param.omVolumeUSL )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_ObjectMeasureVolumeRatioLSL(const TALG_PARAM_OBJECT_MEASURE &Param)
{
	double Ratio=0.0;
	if ( fabs(Param.omVolumeSpec) > 0.0001 )
	{	Ratio = 100.0*Param.omVolumeReading/Param.omVolumeSpec; }
	else
	{	Ratio = 0.0; }
	if ( Ratio < Param.omVolumeLSL )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::WriteAlgParamFile_ObjectMeasure(const TALG_PARAM_OBJECT_MEASURE &omParam, CAOIFileIO &FileIO)//儲存物件量測參數
{		
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_OBJECT_MEASURE_START, 0) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_OBJECT_MEASURE_SIZE_CALC_MODE, omParam.omSizeCalcMode) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_OBJECT_MEASURE_SIZE_CALC_UNIT_MODE, omParam.omSizeCalcUnitMode) == false ) { return false; }	
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_OBJECT_MEASURE_SIZE_BLUR_SIZE, omParam.omSizeBlurSize) == false ) { return false; }		
	if ( FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_OBJECT_MEASURE_MULTI_BLOB, omParam.omUseMultiBlob) == false ) { return false; }

	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_OBJECT_MEASURE_SIZE_X_SPEC, omParam.omSizeXSpec) == false ) { return false; }
	if ( FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_OBJECT_MEASURE_SIZE_X_ENB, omParam.omSizeXEnabled) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_OBJECT_MEASURE_SIZE_X_USL_DIFF, omParam.omSizeXDiffUSL) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_OBJECT_MEASURE_SIZE_X_LSL_DIFF, omParam.omSizeXDiffLSL) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_OBJECT_MEASURE_SIZE_X_USL_RATIO, omParam.omSizeXRatioUSL) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_OBJECT_MEASURE_SIZE_X_LSL_RATIO, omParam.omSizeXRatioLSL) == false ) { return false; }	
	
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_OBJECT_MEASURE_SIZE_Y_SPEC, omParam.omSizeYSpec) == false ) { return false; }
	if ( FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_OBJECT_MEASURE_SIZE_Y_ENB, omParam.omSizeYEnabled) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_OBJECT_MEASURE_SIZE_Y_USL_DIFF, omParam.omSizeYDiffUSL) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_OBJECT_MEASURE_SIZE_Y_LSL_DIFF, omParam.omSizeYDiffLSL) == false ) { return false; }	
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_OBJECT_MEASURE_SIZE_Y_USL_RATIO, omParam.omSizeYRatioUSL) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_OBJECT_MEASURE_SIZE_Y_LSL_RATIO, omParam.omSizeYRatioLSL) == false ) { return false; }	
	
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_OBJECT_MEASURE_HEIGHT_SPEC, omParam.omHeightSpec) == false ) { return false; }
	if ( FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_OBJECT_MEASURE_HEIGHT_ENB, omParam.omHeightEnabled) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_OBJECT_MEASURE_HEIGHT_AVE_MODE, omParam.omHeightAverageMode) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_OBJECT_MEASURE_HEIGHT_AVE_PART_H, omParam.omHeightAveragePartialH) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_OBJECT_MEASURE_HEIGHT_AVE_PART_L, omParam.omHeightAveragePartialL) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_OBJECT_MEASURE_HEIGHT_CALC_UNIT_MODE, omParam.omHeightCalcUnitMode) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_OBJECT_MEASURE_HEIGHT_USL_DIFF, omParam.omHeightDiffUSL) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_OBJECT_MEASURE_HEIGHT_LSL_DIFF, omParam.omHeightDiffLSL) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_OBJECT_MEASURE_HEIGHT_USL_RATIO, omParam.omHeightRatioUSL) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_OBJECT_MEASURE_HEIGHT_LSL_RATIO, omParam.omHeightRatioLSL) == false ) { return false; }	

	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_OBJECT_MEASURE_AREA_SPEC, omParam.omAreaSpec) == false ) { return false; }
	if ( FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_OBJECT_MEASURE_AREA_ENB, omParam.omAreaEnabled) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_OBJECT_MEASURE_AREA_USL, omParam.omAreaUSL) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_OBJECT_MEASURE_AREA_LSL, omParam.omAreaLSL) == false ) { return false; }
	
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_OBJECT_MEASURE_VOLUME_SPEC, omParam.omVolumeSpec) == false ) { return false; }
	if ( FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_OBJECT_MEASURE_VOLUME_ENB, omParam.omVolumeEnabled) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_OBJECT_MEASURE_VOLUME_USL, omParam.omVolumeUSL) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_OBJECT_MEASURE_VOLUME_LSL, omParam.omVolumeLSL) == false ) { return false; }	

	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_OBJECT_MEASURE_END, 0) == false ) { return false; }	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ReadAlgParamFile_ObjectMeasure(TALG_PARAM_OBJECT_MEASURE &omParam, CAOIFileIO &FileIO)//載入物件量測參數
{
	int       index = 0;
	while ( FileIO.CheckFileEnd()==false )
	{ 		
		if ( FileIO.LoadChunk(index) == false )		
		{	continue; }
		switch ( index )
		{		
		case FILE_IO_ALG_PARAM_OBJECT_MEASURE_END://物件量測參數-終點
			return true;	
		case FILE_IO_ALG_PARAM_OBJECT_MEASURE_SIZE_CALC_MODE://尺寸計算模式
			omParam.omSizeCalcMode = (ALG_OBJECT_SIZE_CALC_MODE)(FileIO.GetData_INT());
			break;
		case FILE_IO_ALG_PARAM_OBJECT_MEASURE_SIZE_CALC_UNIT_MODE://尺寸計算單位模式
			omParam.omSizeCalcUnitMode = (ALG_CALC_UNIT_MODE)(FileIO.GetData_INT());			
			break;
		case FILE_IO_ALG_PARAM_OBJECT_MEASURE_SIZE_BLUR_SIZE://尺寸平滑大小
			omParam.omSizeBlurSize = FileIO.GetData_INT();
			break;
		case FILE_IO_ALG_PARAM_OBJECT_MEASURE_MULTI_BLOB://多個區塊
			omParam.omUseMultiBlob = FileIO.GetData_BOL();
			break;
		case FILE_IO_ALG_PARAM_OBJECT_MEASURE_SIZE_X_SPEC://尺寸X規格
			omParam.omSizeXSpec = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_OBJECT_MEASURE_SIZE_X_ENB://尺寸X啟用
			omParam.omSizeXEnabled = FileIO.GetData_BOL();
			break;
		case FILE_IO_ALG_PARAM_OBJECT_MEASURE_SIZE_X_USL_RATIO://尺寸X上限-%
			omParam.omSizeXRatioUSL = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_OBJECT_MEASURE_SIZE_X_LSL_RATIO://尺寸X下限-%
			omParam.omSizeXRatioLSL = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_OBJECT_MEASURE_SIZE_X_USL_DIFF://尺寸X上限-um
			omParam.omSizeXDiffUSL = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_OBJECT_MEASURE_SIZE_X_LSL_DIFF://尺寸X下限-um
			omParam.omSizeXDiffLSL = FileIO.GetData_DBL();
			break;
		
		case FILE_IO_ALG_PARAM_OBJECT_MEASURE_SIZE_Y_SPEC://尺寸Y規格
			omParam.omSizeYSpec = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_OBJECT_MEASURE_SIZE_Y_ENB://尺寸Y啟用
			omParam.omSizeYEnabled = FileIO.GetData_BOL();
			break;			
		case FILE_IO_ALG_PARAM_OBJECT_MEASURE_SIZE_Y_USL_RATIO://尺寸Y上限-%
			omParam.omSizeYRatioUSL = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_OBJECT_MEASURE_SIZE_Y_LSL_RATIO://尺寸Y下限-%
			omParam.omSizeYRatioLSL = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_OBJECT_MEASURE_SIZE_Y_USL_DIFF://尺寸Y上限-um
			omParam.omSizeYDiffUSL = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_OBJECT_MEASURE_SIZE_Y_LSL_DIFF://尺寸Y下限-um
			omParam.omSizeYDiffLSL = FileIO.GetData_DBL();
			break;		
		
		case FILE_IO_ALG_PARAM_OBJECT_MEASURE_HEIGHT_SPEC://高度規格
			omParam.omHeightSpec = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_OBJECT_MEASURE_HEIGHT_ENB://高度啟用
			omParam.omHeightEnabled = FileIO.GetData_BOL();
			break;
		case FILE_IO_ALG_PARAM_OBJECT_MEASURE_HEIGHT_AVE_MODE://高度平均模式
			omParam.omHeightAverageMode = (ALG_OBJECT_HEIGHT_AVERAGE_MODE)(FileIO.GetData_INT());
			break;
		case FILE_IO_ALG_PARAM_OBJECT_MEASURE_HEIGHT_AVE_PART_H://高度平均範圍上限
			omParam.omHeightAveragePartialH = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_OBJECT_MEASURE_HEIGHT_AVE_PART_L://高度平均範圍下限
			omParam.omHeightAveragePartialL = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_OBJECT_MEASURE_HEIGHT_CALC_UNIT_MODE://高度計算單位模式
			omParam.omHeightCalcUnitMode = (ALG_CALC_UNIT_MODE)(FileIO.GetData_INT());
			break;
		case FILE_IO_ALG_PARAM_OBJECT_MEASURE_HEIGHT_USL_RATIO://高度上限-%
			omParam.omHeightRatioUSL = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_OBJECT_MEASURE_HEIGHT_LSL_RATIO://高度下限-%
			omParam.omHeightRatioLSL = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_OBJECT_MEASURE_HEIGHT_USL_DIFF://高度上限-um
			omParam.omHeightDiffUSL = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_OBJECT_MEASURE_HEIGHT_LSL_DIFF://高度下限-um
			omParam.omHeightDiffLSL = FileIO.GetData_DBL();
			break;
		
		case FILE_IO_ALG_PARAM_OBJECT_MEASURE_AREA_SPEC://面積規格
			omParam.omAreaSpec = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_OBJECT_MEASURE_AREA_ENB://面積啟用
			omParam.omAreaEnabled = FileIO.GetData_BOL();
			break;
		case FILE_IO_ALG_PARAM_OBJECT_MEASURE_AREA_USL://面積上限-%
			omParam.omAreaUSL = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_OBJECT_MEASURE_AREA_LSL://面積下限-%
			omParam.omAreaLSL = FileIO.GetData_DBL();
			break;
		
		case FILE_IO_ALG_PARAM_OBJECT_MEASURE_VOLUME_SPEC://體積規格
			omParam.omVolumeSpec = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_OBJECT_MEASURE_VOLUME_ENB://體積啟用
			omParam.omVolumeEnabled = FileIO.GetData_BOL();
			break;
		case FILE_IO_ALG_PARAM_OBJECT_MEASURE_VOLUME_USL://體積上限-%
			omParam.omVolumeUSL = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_OBJECT_MEASURE_VOLUME_LSL://體積下限-%
			omParam.omVolumeLSL = FileIO.GetData_DBL();
			break;
		default:
		#ifdef _DEBUG
			index = index;
		#endif//_DEBUG
			break;
		}
	}
	FileIO.SetErrorString(_T("Error, ReadAlgParamFile_ObjectMeasure Fault"));
	return false;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ExecAlgInspection_ObjectMeasure(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList)
{
	if ( NULL == ModelPtr ) { return false; }
	CAOIComponent *ComponentPtr = ModelPtr->GetModelComponentPtr();
	const size_t FrameCount = UniFrameList.size();
	const size_t ImageFrameIndex = m_AlgImageBinParam.GetBinaryFrameIndex();
	if ( ImageFrameIndex >= FrameCount ) 
	{	return false; }	
	CAOIWnd         *WndPtr = GetAlgWndPtr();
	if ( NULL == WndPtr ) { return false; }	
	CAOIBox         &ExtendBox = WndPtr->GetWndExtendBox();
	const size_t     WndIndex = WndPtr->GetWndIndex();
	BOX_SHAPE_MODE   WndShapeMode = WndPtr->GetWndShapeMode();

	size_t           i=0, j=0, k=0, idx=0;		
	TUNI_FRAME      *UniFramePtr  = &(UniFrameList[ImageFrameIndex]);
	BINARY_MODE      BinaryMode= m_AlgImageBinParam.GetBinaryMode();
	const IMAGE_SIZE FrameImageW    = UniFramePtr->ImageW;
	const IMAGE_SIZE FrameImageH    = UniFramePtr->ImageH;
	const IMAGE_SIZE FrameImageStep = UniFramePtr->ImageStep;
	const IMAGE_SIZE FrameBitCount  = UniFramePtr->BitCount;		
	IMAGE_PTR        FrameImagePtr  = UniFramePtr->ImagePtr;
	MASK_PTR         FrameMaskPtr   = UniFramePtr->MaskPtr;
	SPACE_PTR        FrameSpacePtr  = UniFramePtr->SpacePtr;
	TPOINT2D         ImageScale     = ModelPtr->GetModelImageScale();	
	TALG_PARAM_OBJECT_MEASURE &omParam= GetAlgParamObjectMeasure();	
	ALG_OBJECT_SIZE_CALC_MODE omSizeCalcMode = omParam.omSizeCalcMode;
	ALG_OBJECT_HEIGHT_AVERAGE_MODE omHeightAverageMode = omParam.omHeightAverageMode;
	const double ResX = 1.0/ImageScale.x;
	const double ResY = 1.0/ImageScale.y;	
	const int RoiW = RoiRect.right-RoiRect.left;
	const int RoiH = RoiRect.bottom-RoiRect.top;
	const int RoiRectSize = (int)((RoiRect.right-RoiRect.left)*(RoiRect.bottom-RoiRect.top));	

	CString       str;	
	double        USL=0.0;
	double        LSL=0.0;
	double        Value=0.0;
	int           Pixels=0;	
	double        Ratio=0.0;
	double        SizeX=0.0;
	double        SizeY=0.0;
	double        Area=0.0;	
	double        Height=0.0;	
	double        Volume=0.0;	
	double        AreaHeight=0.0;	
	IMAGE_SIZE    MaskW=0;
	IMAGE_SIZE    MaskH=0;	
	IMAGE_SIZE    MaskStep=0;
	IMAGE_SIZE    MaskBitCount=8;
	MASK_PTR      MaskPtr  = NULL;
	IMAGE_PTR     GrayPtr = NULL;	
	const bool    bTestWnd = false;
	std::vector<int> ValueList;
#ifdef _DEBUG	
	CString      ComponentName;
	CString      DebugFolder=GetAlgDebugFolder();
	bool         bSave = true;
	bool         bSaveContour=true;
	if ( NULL != ComponentPtr )
	{	ComponentName = ComponentPtr->GetComponentFullName(); }			
#endif//_DEBUG

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

		if ( MaskW2!=MaskW || MaskH2!=MaskH || MaskStep2!=MaskStep )
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

		//合併遮罩
		ImageAPI.MergeMaskImage3(MaskW, MaskH, MaskStep, MaskPtr, MaskPtr2, RoiRect, MERGE_MASK_AND, MaskPtr);
		
		JetMemory.free_func(MaskPtr2);
		JetMemory.free_func(GrayPtr2);

	#ifdef _DEBUG
		if ( true == bSave )
		{	
			str.Format(_T("%s\\%s_ModelWndAlgObjectMeasure#%d_MergeMask.PNG"), DebugFolder, ComponentName, WndIndex+1);
			ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, MaskBitCount2, MaskPtr, true);
		}
	#endif
	}

	if ( WndPtr->CheckWndNeedShapeMask() == true )
	{
		MASK_PTR      ShapePtr  = NULL;
		IMAGE_SIZE    ShapeW = MaskW;
		IMAGE_SIZE    ShapeH = MaskH;					
		IMAGE_SIZE    ShapeStep = MaskStep;
		const size_t  ShapeBufferSize = ImageAPI.CalcBufferSize(ShapeStep, ShapeH);
		if ( JetMemory.alloc_func(ShapeBufferSize, ShapePtr, "CAlgParam::ExecAlgInspection_ObjectMeasure", "ShapePtr") == false )
		{	
			JetMemory.free_func(MaskPtr);
			JetMemory.free_func(GrayPtr);
			return false; 
		}		
		ImageAPI.FillImageRoi(ShapeW, ShapeH, ShapeStep, 8, ShapePtr, RoiRect, 0, 0, 0);
		if ( WndPtr->BuildWndShapeMask3(ShapeW, ShapeH, ShapeStep, ShapePtr, RoiRect) == false )
		{			
			JetMemory.free_func(MaskPtr);
			JetMemory.free_func(GrayPtr);
			JetMemory.free_func(ShapePtr);
			return false;
		}	
		if ( ImageAPI.Intersection2MaskImage3(MaskW, MaskH, MaskStep, MaskPtr, ShapePtr, RoiRect, MaskStep, MaskPtr, 255, 0, true) == false )
		{
			JetMemory.free_func(MaskPtr);
			JetMemory.free_func(GrayPtr);
			JetMemory.free_func(ShapePtr);
			return false;
		}
		JetMemory.free_func(ShapePtr);
	}

#ifdef _DEBUG
	if ( true == bSave )
	{
		str.Format(_T("%s\\%s_ModelWndAlgObjectMeasure#%d_FinalMask.PNG"), DebugFolder, ComponentName, WndIndex+1);
		ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, MaskBitCount, MaskPtr, true);
	}
#endif//_DEBUG

	//做Blob偵測	
	CJetBlob BlobDetector;	
	IMAGE_PTR  BlobPatPtr=NULL;
	IMAGE_SIZE BlobPatW=0, BlobPatH=0, BlobPatStep=0;
	IMAGE_PTR  BlobRoiPtr=NULL;
	IMAGE_SIZE BlobRoiW=0, BlobRoiH=0, BlobRoiStep=0;
	const size_t MaskBufferSize = ImageAPI.CalcBufferSize(MaskStep, MaskH);

	BlobDetector.InitialBlob();
	BlobDetector.SetBlobSortMode(BLOB_SORT_PIXELS);
	BlobDetector.SetBlobConnectivity(BLOB_CONNECTIVITY_4);		
	if ( BlobDetector.GrayImageRoiBlobDetect(MaskW, MaskH, MaskStep, MaskPtr, RoiRect, 164, 255) == false )	
	{
		JetMemory.free_func(MaskPtr);
		JetMemory.free_func(GrayPtr);			
		return false;
	}

	double    SkewW=0;
	double    SkewH=0;	
	double    SkewCpX=0;
	double    SkewCpY=0;
	double    SkewAngle=0;
	double    BlobResX=0.0;
	double    BlobResY=0.0;	
	const double dSkew = GetAlgSkewCalcRange();
	const int SkewMin = (int)(-dSkew);
	const int SkewMax = (int)(+dSkew);
	const bool bUseMultiBlob= omParam.omUseMultiBlob;
	const bool SizeXEnabled = omParam.omSizeXEnabled;
	const bool SizeYEnabled = omParam.omSizeYEnabled;
	const double RoiRectCpX = (RoiRect.left+RoiRect.right)*0.5;
	const double RoiRectCpY = (RoiRect.top+RoiRect.bottom)*0.5;	

	TBlobResult *BlobPtr=NULL;	
	int FilterSizeXMax=INT_MAX, FilterSizeXMin=-INT_MAX;
	int FilterSizeYMax=INT_MAX, FilterSizeYMin=-INT_MAX;
	const size_t ObjBlobCount = BlobDetector.GetBlobCount();

	BlobResX = ResX;
	BlobResY = ResY;
#ifdef _DEBUG
	if ( true == bSave )
	{
		str.Format(_T("%s\\%s_ModelWndAlgObjectMeasure#%d_BlobMask.PNG"), DebugFolder, ComponentName, WndIndex+1);
		ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, MaskBitCount, MaskPtr, true);
	}
#endif

	if ( true == bUseMultiBlob )
	{
		bool bFirst=true;			
		RECT MultiBlobRect;
		TBlobResult *BlobPtrTmp=NULL;
		omSizeCalcMode = ALG_OBJECT_SIZE_CALC_BOUNDARY;
		for ( i=0; i<ObjBlobCount; i++ )
		{
			BlobPtrTmp = BlobDetector.GetBlobPtr(i, false);
			if ( NULL == BlobPtrTmp ) { continue; }
			if ( true == bFirst )
			{
				bFirst = false;
				MultiBlobRect = BlobPtrTmp->m_BlobRect;
				continue;
			}
			MultiBlobRect.left = MIN(MultiBlobRect.left, BlobPtrTmp->m_BlobRect.left);
			MultiBlobRect.top = MIN(MultiBlobRect.top, BlobPtrTmp->m_BlobRect.top);
			MultiBlobRect.right = MAX(MultiBlobRect.right, BlobPtrTmp->m_BlobRect.right);
			MultiBlobRect.bottom = MAX(MultiBlobRect.bottom, BlobPtrTmp->m_BlobRect.bottom);
		}
		if ( true == bFirst )
		{
			SkewCpX = RoiRectCpX;
			SkewCpY = RoiRectCpY;
			SizeX = SkewW = 0;//WndRect.right-WndRect.left;
			SizeY = SkewH = 0;//WndRect.bottom-WndRect.top;
			::memset(MaskPtr, 0x00, sizeof(MASK_DATA)*MaskBufferSize);	
		}
		else
		{
			const int BlobRectW = MultiBlobRect.right-MultiBlobRect.left;
			const int BlobRectH = MultiBlobRect.bottom-MultiBlobRect.top;
			const double BlobRectCpX = (MultiBlobRect.left+MultiBlobRect.right)*0.5;
			const double BlobRectCpY = (MultiBlobRect.top+MultiBlobRect.bottom)*0.5;

			SkewW = BlobRectW;
			SkewH = BlobRectH;
			SkewCpX = BlobRectCpX;
			SkewCpY = BlobRectCpY;
			SizeX = SkewW;
			SizeY = SkewH;			
		}
	}
	else
	{
		if ( true==SizeXEnabled || true==SizeYEnabled )
		{
			double SizeMax=0, SizeMin=0, SizeTag=0;		
			if ( true == SizeXEnabled ) 
			{
				SizeTag = omParam.omSizeXSpec;
				if ( ALG_CALC_UNIT_DIFF == omParam.omSizeCalcUnitMode )
				{	
					SizeMax = SizeTag+omParam.omSizeXDiffUSL;
					SizeMin = SizeTag+omParam.omSizeXDiffLSL;
				}
				else
				{
					SizeMax = SizeTag*omParam.omSizeXRatioUSL;
					SizeMin = SizeTag*omParam.omSizeXRatioLSL;				
				}
				FilterSizeXMax = (int)((SizeMax/ResX)+0.5);
				FilterSizeXMin = (int)((SizeMin/ResX)+0.5);
			}
			if ( true == SizeYEnabled ) 
			{
				SizeTag = omParam.omSizeYSpec;
				if ( ALG_CALC_UNIT_DIFF == omParam.omSizeCalcUnitMode )
				{	
					SizeMax = SizeTag+omParam.omSizeYDiffUSL;
					SizeMin = SizeTag+omParam.omSizeYDiffLSL;
				}
				else
				{
					SizeMax = SizeTag*omParam.omSizeYRatioUSL;
					SizeMin = SizeTag*omParam.omSizeYRatioLSL;				
				}
				FilterSizeYMax = (int)((SizeMax/ResY)+0.5);
				FilterSizeYMin = (int)((SizeMin/ResY)+0.5);
			}
			int BlobTmpW=0;
			int BlobTmpH=0;
			TBlobResult *BlobPtrTmp=NULL;
			for ( i=0; i<ObjBlobCount; i++ )
			{
				BlobPtrTmp = BlobDetector.GetBlobPtr(i, false);
				if ( NULL == BlobPtrTmp ) { continue; }
				BlobTmpW = BlobPtrTmp->GetBlobRectW();
				BlobTmpH = BlobPtrTmp->GetBlobRectH();
				if ( BlobTmpW > FilterSizeXMax ) { continue; } 
				if ( BlobTmpW < FilterSizeXMin ) { continue; }
				if ( BlobTmpH > FilterSizeYMax ) { continue; } 
				if ( BlobTmpH < FilterSizeYMin ) { continue; }
				BlobPtr = BlobPtrTmp;
				break;
			}
		}	
		if ( NULL == BlobPtr )
		{	BlobPtr=BlobDetector.GetBlobPtr(0, true); }
	
		if ( NULL == BlobPtr ) 
		{	
			SkewCpX = RoiRectCpX;
			SkewCpY = RoiRectCpY;
			SizeX = SkewW = 0;//WndRect.right-WndRect.left;
			SizeY = SkewH = 0;//WndRect.bottom-WndRect.top;
			::memset(MaskPtr, 0x00, sizeof(MASK_DATA)*MaskBufferSize);	
		}
		else
		{	
			const int BlobRectW = BlobPtr->GetBlobRectW();
			const int BlobRectH = BlobPtr->GetBlobRectH();
			const double BlobRectCpX = BlobPtr->GetBlobRectCpX();
			const double BlobRectCpY = BlobPtr->GetBlobRectCpY();		
		
			SkewW = BlobRectW;
			SkewH = BlobRectH;
			SkewCpX = BlobRectCpX;
			SkewCpY = BlobRectCpY;
			SizeX = SkewW;
			SizeY = SkewH;

			BlobDetector.GetBlobImage(BlobPtr, MaskW, MaskH, MaskStep, MaskBitCount, MaskPtr);		
		#ifdef _DEBUG
			if ( true == bSave )
			{
				str.Format(_T("%s\\%s_ModelWndAlgObjectMeasure#%d_BlobMask2.PNG"), DebugFolder, ComponentName, WndIndex+1);
				ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, MaskBitCount, MaskPtr, true);
			}
		#endif
			if ( ALG_OBJECT_SIZE_CALC_BOUNDARY != omSizeCalcMode )		
			{
				std::vector<POINT> Contour;	
				RECT   TmpRect=BlobPtr->m_BlobRect;
				BlobDetector.GetBlobContour(BlobPtr, Contour);
			#ifdef _DEBUG
				if ( true == bSaveContour )
				{
					IMAGE_PTR  TmpPtr=NULL;
					IMAGE_SIZE TmpW=0, TmpH=0, TmpStep=0, TmpBitCnt=0;
					ImageAPI.CreatePointListImage(Contour, TmpW, TmpH, TmpStep, TmpBitCnt, TmpPtr);
					str.Format(_T("%s\\%s_ModelWndAlgObjectMeasure#%d_BlobContour.PNG"), DebugFolder, ComponentName, WndIndex+1);
					ImageAPI.SaveImage(str, TmpW, TmpH, TmpStep, TmpBitCnt, TmpPtr, true);
					JetMemory.free_func(TmpPtr);			
				}
			#endif			
				if ( ALG_OBJECT_SIZE_CALC_BLUR_RECT == omSizeCalcMode )
				{	ImageAPI.CalcPointListBlurMaxRect(Contour, TmpRect, omParam.omSizeBlurSize, SizeX, SizeY, SkewCpX, SkewCpY);	}			
				else
				{
					if ( BOX_SHAPE_ELLIPSE == WndShapeMode )
					{	
						std::vector<POINT> Outline;
						JetAPI::ExtractOutline(Contour, Outline);
						Contour = Outline;
						SkewCpX = BlobRectCpX;
						SkewCpY = BlobRectCpY;
						ImageAPI.CalcPointListAveSizeByCircle(Contour, SkewCpX, SkewCpY, SkewW, SkewH);
						SizeX = SkewW;
						SizeY = SkewH;
						BlobResX = (ResX+ResY)*0.5;
						BlobResY = (ResX+ResY)*0.5;
					}
					else
					{				
						ImageAPI.CalcPointListMinAreaRect(Contour, SkewMin, SkewMax, 1, SkewCpX, SkewCpY, SkewW, SkewH, SkewAngle);			
						//只取中央的3/5的資料			
						const int UsedW = (int)(SkewW*3/5);
						const int UsedH = (int)(SkewH*3/5);
						const int nLeft  = (int)(SkewCpX-(UsedW/2));
						const int nRight = (int)(SkewCpX+(UsedW/2));
						const int nTop = (int)(SkewCpY-(UsedH/2));
						const int nBot = (int)(SkewCpY+(UsedH/2));
						RECT   TmpRect={nLeft, nTop, nRight, nBot};
						JetAPI::RotatePosList(-SkewAngle, SkewCpX, SkewCpY, Contour);
						ImageAPI.CalcPointListAveSizeByRect(Contour, TmpRect, SizeX, SizeY);
						if ( ALG_OBJECT_SIZE_CALC_AVE_RECT == omSizeCalcMode )
						{
							if ( SizeX>FilterSizeXMax || SizeX<FilterSizeXMin || SizeY>FilterSizeYMax || SizeY<FilterSizeYMin )
							{
								SkewW = BlobRectW;
								SkewH = BlobRectH;
								SkewCpX = BlobRectCpX;
								SkewCpY = BlobRectCpY;
								SizeX = SkewW;
								SizeY = SkewH;
							}
						}					
					}	
				}
			#ifdef _DEBUG
				if ( true == bSaveContour )
				{
					IMAGE_PTR  TmpPtr=NULL;
					IMAGE_SIZE TmpW=0, TmpH=0, TmpStep=0, TmpBitCnt=0;
					ImageAPI.CreatePointListImage(Contour, TmpW, TmpH, TmpStep, TmpBitCnt, TmpPtr);
					str.Format(_T("%s\\%s_ModelWndAlgObjectMeasure#%d_BlobContour2.PNG"), DebugFolder, ComponentName, WndIndex+1);
					ImageAPI.SaveImage(str, TmpW, TmpH, TmpStep, TmpBitCnt, TmpPtr, true);
					JetMemory.free_func(TmpPtr);
				}
			#endif	
			}	
		}
	}

	Pixels=0;
	Height=0.0;
	AreaHeight = 0.0;				
	if ( NULL==UniFramePtr->SpacePtr || NULL==UniFramePtr->MaskPtr )
	{
		for ( i=RoiRect.top; i<RoiRect.bottom; i++ )
		{
			idx = (i*MaskStep)+RoiRect.left;
			for ( j=RoiRect.left; j<RoiRect.right; j++ )
			{
				if ( 0 == MaskPtr[idx] )
				{	idx ++;	continue;	}
				if ( ALG_OBJECT_HEIGHT_AVERAGE_PARTIAL == omHeightAverageMode )
				{	ValueList.push_back(GrayPtr[idx]);	}
				
				AreaHeight += GrayPtr[idx];
				Pixels ++;				
				idx ++;				
			}
		}
	}
	else
	{
		if ( MaskStep != FrameImageStep ) 
		{ 			
			JetMemory.free_func(MaskPtr);
			JetMemory.free_func(GrayPtr);
			return false; 
		}

		for ( i=RoiRect.top; i<RoiRect.bottom; i++ )
		{
			idx = (i*MaskStep)+RoiRect.left-1;//GrayStep ?= FrameImageStep
			for ( j=RoiRect.left; j<RoiRect.right; j++ )
			{
				//上下高度遮罩, 相位計算雜訊
				idx ++;
				if ( 0==MaskPtr[idx] )
				{	continue;	}
				if ( ImageAPI.CheckSpaceMaskValid(UniFramePtr->MaskPtr[idx]) == false )
				{	continue;	}
				if ( ALG_OBJECT_HEIGHT_AVERAGE_PARTIAL == omHeightAverageMode )
				{	ValueList.push_back((int)(UniFramePtr->SpacePtr[idx]));	}
				AreaHeight += UniFramePtr->SpacePtr[idx];
				Pixels ++;
			}
		}
	}	

	if ( Pixels > 0 ) 
	{	AreaHeight = AreaHeight/Pixels;	}
	const size_t ValueListSize = (size_t)(ValueList.size());	
	if ( ALG_OBJECT_HEIGHT_AVERAGE_PARTIAL==omHeightAverageMode )
	{	
		double AverageRatioUSL = omParam.omHeightAveragePartialH;
		double AverageRatioLSL = omParam.omHeightAveragePartialL;
		if ( AverageRatioUSL > 100 ) { AverageRatioUSL = 100; }
		else if ( AverageRatioUSL < 0 ) { AverageRatioUSL = 0; }
		if ( AverageRatioLSL > 100 ) { AverageRatioLSL = 100; }
		else if ( AverageRatioLSL < 0 ) { AverageRatioLSL = 0; }
		size_t CalCntU = (size_t)(AverageRatioUSL*ValueListSize)/100.0;
		size_t CalCntL = (size_t)(AverageRatioLSL*ValueListSize)/100.0;
		if ( CalCntU > ValueListSize ) { CalCntU = (size_t)(ValueListSize); }
		if ( CalCntL > ValueListSize ) { CalCntL = (size_t)(ValueListSize); }

		Height = 0.0;		
		std::sort(ValueList.begin(), ValueList.end());
		for ( i=CalCntL; i<CalCntU; i++ )
		{	Height += ValueList[i];	}
		const size_t CalCnt=CalCntU-CalCntL;
		if ( CalCnt > 0 ) 
		{	
			Height /= CalCnt;
			Height += 0.5;
		}
	}
	else
	{	Height = AreaHeight;	}

	
	const double RoiSizeWum=ResX*RoiW;
	const double RoiSizeHum=ResY*RoiH;
	const double ExtWndSizeW=ExtendBox.GetBoxSizeX();
	const double ExtWndSizeH=ExtendBox.GetBoxSizeY();
	const double PxlUmRatioX=ExtWndSizeW/RoiSizeWum;//修正像素與um轉換誤差
	const double PxlUmRatioY=ExtWndSizeH/RoiSizeHum;//修正像素與um轉換誤差

	SizeX = SizeX*BlobResX*PxlUmRatioX;
	SizeY = SizeY*BlobResY*PxlUmRatioY;
	Height = (int)(Height+0.5);
	Area = Pixels*ResX*ResY;	
	//Area = Area*PxlUmRatioX*PxlUmRatioY;//還需要確認是否加入
	Volume=Area*AreaHeight;		
	
	omParam.omSizeXReading = SizeX;
	omParam.omSizeYReading = SizeY;
	omParam.omHeightReading = Height;
	omParam.omAreaReading = Area;
	omParam.omVolumeReading = Volume;	

	//Judge OK/NG
	CString strResult;	
	SetAlgResultText(_T("OK"));
	SetAlgResultID(RESULT_ID_OK);
	if ( true == omParam.omSizeXEnabled )
	{
		if ( ALG_CALC_UNIT_DIFF == omParam.omSizeCalcUnitMode )
		{
			Value = SizeX;
			USL = omParam.omSizeXSpec+omParam.omSizeXDiffUSL;
			LSL = omParam.omSizeXSpec+omParam.omSizeXDiffLSL;
			if ( Value>USL || Value<LSL )
			{	
				strResult.Format(_T("Size X(%.2f um)"), Value);
				SetAlgResultID(RESULT_ID_NG);
				SetAlgResultText(strResult);
			}
		}
		else
		{
			if ( fabs(omParam.omSizeXSpec) > 0.0001 )
			{	Ratio = 100.0*SizeX/omParam.omSizeXSpec; }
			else
			{	Ratio = 0.0; }
			if ( Ratio>omParam.omSizeXRatioUSL || Ratio<omParam.omSizeXRatioLSL )
			{	
				strResult.Format(_T("Size X(%.2f %%)"), Ratio);
				SetAlgResultID(RESULT_ID_NG);
				SetAlgResultText(strResult);
			}
		}
	}

	if ( true == omParam.omSizeYEnabled )
	{
		if ( ALG_CALC_UNIT_DIFF == omParam.omSizeCalcUnitMode )
		{
			Value = SizeY;
			USL = omParam.omSizeYSpec+omParam.omSizeYDiffUSL;
			LSL = omParam.omSizeYSpec+omParam.omSizeYDiffLSL;
			if ( Value>USL || Value<LSL )
			{	
				strResult.Format(_T("Size Y(%.2f um)"), Value);
				SetAlgResultID(RESULT_ID_NG);
				SetAlgResultText(strResult);
			}
		}
		else
		{
			if ( fabs(omParam.omSizeYSpec) > 0.0001 )
			{	Ratio = 100.0*SizeY/omParam.omSizeYSpec; }
			else
			{	Ratio = 0.0; }
			if ( Ratio>omParam.omSizeYRatioUSL || Ratio<omParam.omSizeYRatioLSL )
			{	
				strResult.Format(_T("Size Y(%.2f %%)"), Ratio);
				SetAlgResultID(RESULT_ID_NG);
				SetAlgResultText(strResult);
			}
		}
	}

	if ( true == omParam.omHeightEnabled )
	{
		if ( ALG_CALC_UNIT_DIFF == omParam.omHeightCalcUnitMode )
		{
			Value = Height;
			USL = omParam.omHeightSpec+omParam.omHeightDiffUSL;
			LSL = omParam.omHeightSpec+omParam.omHeightDiffLSL;
			if ( Value>USL || Value<LSL )
			{	
				strResult.Format(_T("Height(%.2f um)"), Value);
				SetAlgResultID(RESULT_ID_NG);
				SetAlgResultText(strResult);
			}
		}
		else
		{
			if ( fabs(omParam.omHeightSpec) > 0.0001 )
			{	Ratio = 100.0*Height/omParam.omHeightSpec; }
			else
			{	Ratio = 0.0; }
			if ( Ratio>omParam.omHeightRatioUSL || Ratio<omParam.omHeightRatioLSL )
			{	
				strResult.Format(_T("Height(%.2f %%)"), Ratio);
				SetAlgResultID(RESULT_ID_NG);
				SetAlgResultText(strResult);
			}
		}
	}
	
	if ( true == omParam.omAreaEnabled )
	{
		if ( fabs(omParam.omAreaSpec) > 0.0001 )
		{	Ratio = 100.0*Area/omParam.omAreaSpec; }
		else
		{	Ratio = 0.0; }
		if ( Ratio>omParam.omAreaUSL || Ratio<omParam.omAreaLSL )
		{	
			strResult.Format(_T("Area(%.2f %%)"), Ratio);
			SetAlgResultID(RESULT_ID_NG);
			SetAlgResultText(strResult);
		}
	}

	if ( true == omParam.omVolumeEnabled )
	{
		if ( fabs(omParam.omVolumeSpec) > 0.0001 )
		{	Ratio = 100.0*Volume/omParam.omVolumeSpec; }
		else
		{	Ratio = 0.0; }
		if ( Ratio>omParam.omVolumeUSL || Ratio<omParam.omVolumeLSL )
		{				
			strResult.Format(_T("Volume(%.2f %%)"), Ratio);
			SetAlgResultID(RESULT_ID_NG);
			SetAlgResultText(strResult);
		}
	}
	
	double CadSkew_Self=0;//更新給別人
	double CadSkew_Others=0;//更新自己
	double CadOffsetX_Self=0;
	double CadOffsetY_Self=0;
	double CadOffsetX_Others=0;
	double CadOffsetY_Others=0;	
	const bool bChkDefect=true;
	const bool ApplySkewAngle = true;
	const double CadSkew = -SkewAngle;
	const double CadSkewW = SizeX;
	const double CadSkewH = SizeY;
	const double ResOffsetX=SkewCpX-RoiRectCpX;
	const double ResOffsetY=SkewCpY-RoiRectCpY;	
	const double CadOffsetX = ResOffsetX*ResX;
	const double CadOffsetY =-ResOffsetY*ResY;

	SetAlgSkewReading(CadSkew);	
	SetAlgImageOffsetX(ResOffsetX);
	SetAlgImageOffsetY(-ResOffsetY);
	SetAlgOffsetXReading(CadOffsetX);
	SetAlgOffsetYReading(CadOffsetY);
	CalcAlgOffsetL();
	CheckAlgOffset(CadOffsetX_Self, CadOffsetY_Self, CadSkew_Self, CadOffsetX_Others, CadOffsetY_Others, CadSkew_Others, bChkDefect);
	WndPtr->SetWndResultID(m_AlgResultID);
	WndPtr->SetWndResultText(m_AlgResultText);
	SaveAlgDefectImage(MaskW, MaskH, MaskStep, MaskBitCount, GrayPtr, MaskPtr, WndRect);	
	JetMemory.free_func(MaskPtr);
	JetMemory.free_func(GrayPtr);

	bool  ApplySkew = false;
	bool  ApplyOffsetX = false;
	bool  ApplyOffsetY = false;
	if ( fabs(CadSkew_Others) > 0.001 ) { ApplySkew = true; }
	if ( fabs(CadOffsetX_Others) > 0.001 ) { ApplyOffsetX = true; }
	if ( fabs(CadOffsetY_Others) > 0.001 ) { ApplyOffsetY = true; }
	if ( true==ApplyOffsetX || true==ApplyOffsetY || true == ApplySkew )	
	{	
		WND_DEFECT_ID    WndDefectID = WndPtr->GetWndDefectID();
		WND_LOGIC_TYPE   WndLogicType = WndPtr->GetWndLogicType();
		if ( AOIDataDefine.CheckWndDefectIDCanToAlign(WndDefectID) == true ) 
		{
			if ( WND_LOGIC_NONE == WndLogicType )
			{
				if ( ModelPtr->UpdateModelInspectionPosRes(WndPtr, CadOffsetX_Others, CadOffsetY_Others, CadSkew_Others, ApplySkewAngle) == false )
				{	return false; }
			}
		}
	}
	CAOIBox *BoxPtr = WndPtr->GetWndBoxPtr();
	BoxPtr->SkewBoxAngle(CadSkew_Self);	
	BoxPtr->SetBoxSizeRes(CadSkewW, CadSkewH, true);
	BoxPtr->MoveBoxRes(CadOffsetX_Self, CadOffsetY_Self);				
	return true;
}
//-------------------------------------------------------------------------------------//