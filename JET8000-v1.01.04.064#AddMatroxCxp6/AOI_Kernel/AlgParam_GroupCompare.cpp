// AlgParam_GroupCompare.cpp: implementation of the CAlgParam class.
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
bool CAlgParam::CheckAlgGroupCompareSupported(WND_DEFECT_ID WndDefectID)
{
	bool bSupported = false;
	switch ( WndDefectID )
	{
	case WND_DEFECT_BODY_OFFSET:
	case WND_DEFECT_BODY_TILT:
	case WND_DEFECT_BODY_POLARITY:
	case WND_DEFECT_BODY_MOUNT:
	case WND_DEFECT_BASE_VALUE://Ryzen
	case WND_DEFECT_USER_DEFINE_01://Ryzen
		bSupported = true;
		break;
	case WND_DEFECT_LEAD_LIFTED:
		bSupported = true;
		break;
	}
	return bSupported;	
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_GroupCompare2DGray(const TALG_PARAM_GROUP_COMPARE &Param)
{
	if ( CheckOK_GroupCompare2DGrayUSL(Param) == false )
	{	return false; }
	if ( CheckOK_GroupCompare2DGrayLSL(Param) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_GroupCompare2DGrayUSL(const TALG_PARAM_GROUP_COMPARE &Param)
{
	if ( Param.gc2DGrayReading > Param.gc2DGrayUSL )
	{	return false; }	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_GroupCompare2DGrayLSL(const TALG_PARAM_GROUP_COMPARE &Param)
{
	if ( Param.gc2DGrayReading < Param.gc2DGrayLSL )
	{	return false; }	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_GroupCompare3DHeight(const TALG_PARAM_GROUP_COMPARE &Param)
{
	if ( CheckOK_GroupCompare3DHeightUSL(Param) == false )
	{	return false; }
	if ( CheckOK_GroupCompare3DHeightLSL(Param) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_GroupCompare3DHeightUSL(const TALG_PARAM_GROUP_COMPARE &Param)
{
	if ( Param.gc3DHeightReading > Param.gc3DHeightUSL )
	{	return false; }	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_GroupCompare3DHeightLSL(const TALG_PARAM_GROUP_COMPARE &Param)
{
	if ( Param.gc3DHeightReading < Param.gc3DHeightLSL )
	{	return false; }	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_GroupCompareTiltAngle(const TALG_PARAM_GROUP_COMPARE &Param)
{
	if ( CheckOK_GroupCompareTiltAngleUSL(Param) == false )
	{	return false; }
	if ( CheckOK_GroupCompareTiltAngleLSL(Param) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_GroupCompareTiltAngleUSL(const TALG_PARAM_GROUP_COMPARE &Param)
{
	if ( Param.gcTiltAngleReading > Param.gcTiltAngleUSL )
	{	return false; }	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_GroupCompareTiltAngleLSL(const TALG_PARAM_GROUP_COMPARE &Param)
{
	if ( Param.gcTiltAngleReading < Param.gcTiltAngleLSL )
	{	return false; }	
	return true;
}
//-------------------------------------------------------------------------------------//	
bool CAlgParam::WriteAlgParamFile_GroupCompare(const TALG_PARAM_GROUP_COMPARE &gcParam, CAOIFileIO &FileIO)//儲存群組比較參數
{		
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_GROUP_COMPARE_START, 0) == false ) { return false; }

	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_GROUP_COMPARE_DIRECTION_MODE, gcParam.gcDirectionMode) == false ) { return false; }		

	if ( FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_GROUP_COMPARE_2D_GRAY_ENABLED, gcParam.gc2DGrayEnabled) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_GROUP_COMPARE_2D_GRAY_USL, gcParam.gc2DGrayUSL) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_GROUP_COMPARE_2D_GRAY_LSL, gcParam.gc2DGrayLSL) == false ) { return false; }

	if ( FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_GROUP_COMPARE_3D_HEIGHT_ENABLED, gcParam.gc3DHeightEnabled) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_GROUP_COMPARE_3D_HEIGHT_USL, gcParam.gc3DHeightUSL) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_GROUP_COMPARE_3D_HEIGHT_LSL, gcParam.gc3DHeightLSL) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_GROUP_COMPARE_3D_HEIGHT_BASE_MODE, gcParam.gc3DHeightBaseMode) == false ) { return false; }		

	if ( FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_GROUP_COMPARE_TILT_ANGLE_ENB, gcParam.gcTiltAngleEnabled) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_GROUP_COMPARE_TILT_ANGLE_USL, gcParam.gcTiltAngleUSL) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_GROUP_COMPARE_TILT_ANGLE_LSL, gcParam.gcTiltAngleLSL) == false ) { return false; }
	
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_GROUP_COMPARE_END, 0) == false ) { return false; }	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ReadAlgParamFile_GroupCompare(TALG_PARAM_GROUP_COMPARE &gcParam, CAOIFileIO &FileIO)//載入群組比較參數
{
	int       index = 0;
	while ( FileIO.CheckFileEnd()==false )
	{ 		
		if ( FileIO.LoadChunk(index) == false )		
		{	continue; }
		switch ( index )
		{
		case FILE_IO_ALG_PARAM_GROUP_COMPARE_END://群組比較參數-終點
			return true;
			break;
		case FILE_IO_ALG_PARAM_GROUP_COMPARE_DIRECTION_MODE://方向模式
			gcParam.gcDirectionMode = (ALG_GROUP_CMP_DIR_MODE)(FileIO.GetData_INT());
			break;
		case FILE_IO_ALG_PARAM_GROUP_COMPARE_2D_GRAY_ENABLED://2D影像啟用
			gcParam.gc2DGrayEnabled = FileIO.GetData_BOL();
			break;
		case FILE_IO_ALG_PARAM_GROUP_COMPARE_2D_GRAY_USL://2D影像上限
			gcParam.gc2DGrayUSL = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_GROUP_COMPARE_2D_GRAY_LSL://2D影像下限
			gcParam.gc2DGrayLSL = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_GROUP_COMPARE_3D_HEIGHT_ENABLED://3D高度啟用
			gcParam.gc3DHeightEnabled = FileIO.GetData_BOL();
			break;
		case FILE_IO_ALG_PARAM_GROUP_COMPARE_3D_HEIGHT_USL://3D高度上限
			gcParam.gc3DHeightUSL = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_GROUP_COMPARE_3D_HEIGHT_LSL://3D高度下限
			gcParam.gc3DHeightLSL = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_GROUP_COMPARE_3D_HEIGHT_BASE_MODE://3D高度基準高度
			gcParam.gc3DHeightBaseMode = (ALG_3D_BASE_HEIGHT_MODE)(FileIO.GetData_INT());
			break;		
		case FILE_IO_ALG_PARAM_GROUP_COMPARE_TILT_ANGLE_ENB://傾斜角度啟用
			gcParam.gcTiltAngleEnabled = FileIO.GetData_BOL();
			break;
		case FILE_IO_ALG_PARAM_GROUP_COMPARE_TILT_ANGLE_USL://傾斜角度上限
			gcParam.gcTiltAngleUSL = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_GROUP_COMPARE_TILT_ANGLE_LSL://傾斜角度下限
			gcParam.gcTiltAngleLSL = FileIO.GetData_DBL();
			break;
		default:
		#ifdef _DEBUG
			index = index;
		#endif//_DEBUG
			break;
		}
	}
	FileIO.SetErrorString(_T("Error, ReadAlgParamFile_GroupCompare Fault"));
	return false;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ExecAlgInspection_GroupCompare(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList)
{
	if ( NULL == ModelPtr ) { return false; }
	if ( false==m_AlgParamGroupCompare.gc2DGrayEnabled && false==m_AlgParamGroupCompare.gc3DHeightEnabled &&false==m_AlgParamGroupCompare.gcTiltAngleEnabled ) { return true; }

	size_t i=0, j=0, k=0, idx=0;
	const size_t FrameCount = UniFrameList.size();
	const size_t ImageFrameIndex = m_AlgImageBinParam.GetBinaryFrameIndex();
	if ( ImageFrameIndex >= FrameCount ) 
	{	return false; }	

	TUNI_FRAME      *UniFramePtr  = NULL;
	IMAGE_SIZE FrameImageW    = NULL;
	IMAGE_SIZE FrameImageH    = NULL;
	IMAGE_SIZE FrameImageStep = NULL;
	IMAGE_SIZE FrameBitCount  = NULL;		
	IMAGE_PTR        FrameImagePtr  = NULL;
	MASK_PTR         FrameMaskPtr   = NULL;
	SPACE_PTR        FrameSpacePtr  = NULL;	
	std::vector<int> ValueList;
	const bool   BaseValueEnabled = GetAlgBaseValueEnabled();
	const double BaseValueReading = GetAlgBaseValueReading();
	ALG_BRIGHT_AVERAGE_MODE AverageMode = m_AlgParamBrightRatio.brAverageMode;

	int RoiRectSize = 0;
	double     AverageRatioUSL = m_AlgParamBrightRatio.brAveragePartialH;
	double     AverageRatioLSL = m_AlgParamBrightRatio.brAveragePartialL;
	if ( AverageRatioUSL > 100 ) { AverageRatioUSL = 100; }
	else if ( AverageRatioUSL < 0 ) { AverageRatioUSL = 0; }
	if ( AverageRatioLSL > 100 ) { AverageRatioLSL = 100; }
	else if ( AverageRatioLSL < 0 ) { AverageRatioLSL = 0; }

	if ( true == m_AlgParamGroupCompare.gc2DGrayEnabled ) 
	{
		UniFramePtr  = &(UniFrameList[ImageFrameIndex]);		
		FrameImageW    = UniFramePtr->ImageW;
		FrameImageH    = UniFramePtr->ImageH;
		FrameImageStep = UniFramePtr->ImageStep;
		FrameBitCount  = UniFramePtr->BitCount;		
		FrameImagePtr  = UniFramePtr->ImagePtr;
		FrameMaskPtr   = UniFramePtr->MaskPtr;
		FrameSpacePtr  = UniFramePtr->SpacePtr;	
		RoiRectSize = (int)((RoiRect.right-RoiRect.left)*(RoiRect.bottom-RoiRect.top));

		bool          bCloned = false;
		IMAGE_PTR     GrayPtr = NULL;
		IMAGE_SIZE    GrayBitCount = 8;
		const IMAGE_SIZE GrayStep = JetAPI::GetBMPImagePixelsPerLine(FrameImageW, GrayBitCount, 4);
		if ( 24 == FrameBitCount )
		{
			IMAGE_SRC_MODE SourceMode = m_AlgImageBinParam.GetBinaryImageSourceMode();
			const int WR = m_AlgImageBinParam.GetBinarySynthesisWR();
			const int WG = m_AlgImageBinParam.GetBinarySynthesisWG();
			const int WB = m_AlgImageBinParam.GetBinarySynthesisWB();
			if ( ImageAPI.ColorImageToGrayImage(FrameImageW, FrameImageH, FrameImageStep, FrameImagePtr, RoiRect, GrayStep, GrayPtr, SourceMode, WR, WG, WB, false) == false ) 
			{	return false; }
			bCloned = true;
		}
		else
		{
			GrayPtr = FrameImagePtr;
			bCloned = false;
		}

		double dAve=0.0;
		size_t uSum=0;
		if ( ALG_BRIGHT_AVERAGE_FULL == AverageMode )
		{
			for ( i=RoiRect.top; i<RoiRect.bottom; i++ )
			{
				idx = i*FrameImageStep;
				for ( j=RoiRect.left; j<RoiRect.right; j++ )
				{	uSum += GrayPtr[idx+j];	}
			}
		}
		else
		{
			for ( i=RoiRect.top; i<RoiRect.bottom; i++ )
			{
				idx = i*FrameImageStep;
				for ( j=RoiRect.left; j<RoiRect.right; j++ )
				{					
					uSum += GrayPtr[idx+j];	
					ValueList.push_back(GrayPtr[idx+j]);
				}
			}
		}
		if ( RoiRectSize > 0 ) 
		{
			dAve = uSum;
			dAve = dAve/RoiRectSize;
		}
		else 
		{	dAve = 0.0; }

		if ( ALG_BRIGHT_AVERAGE_PARTIAL == AverageMode )
		{
			const size_t ValCnt = ValueList.size();
			size_t CalCntU = (size_t)(AverageRatioUSL*ValCnt)/100.0;
			size_t CalCntL = (size_t)(AverageRatioLSL*ValCnt)/100.0;
			if ( CalCntU > ValCnt ) { CalCntU = (size_t)(ValCnt); }
			if ( CalCntL > ValCnt ) { CalCntL = (size_t)(ValCnt); }
			const size_t CalCnt = CalCntU-CalCntL;
			uSum = 0;
			std::sort(ValueList.begin(), ValueList.end());
			for ( i=CalCntL; i<CalCntU; i++ )
			{	uSum += ValueList[i];	}
			if ( CalCnt > 0 ) 
			{
				dAve = uSum;
				dAve = dAve/CalCnt;
			}
		}
		if ( m_AlgParamBrightRatio.brAverageScaleEnabled )
		{	dAve *= m_AlgParamBrightRatio.brAverageScale/100.0;	}
		if ( true == BaseValueEnabled ) 
		{	dAve = dAve-BaseValueReading;	}
		m_AlgParamGroupCompare.gc2DGrayValue = dAve;
		if ( true == bCloned ) 
		{	JetMemory.free_func(GrayPtr); }		
	}

	if ( true==m_AlgParamGroupCompare.gc3DHeightEnabled || true==m_AlgParamGroupCompare.gcTiltAngleEnabled ) 
	{
		for ( i=0; i<FrameCount; i++ )
		{
			UniFramePtr  = &(UniFrameList[i]);
			if ( NULL == UniFramePtr->SpacePtr ) { continue; }
			break;
		}
		if ( FrameCount != i )//取得3D資料
		{
			size_t uCount = 0;
			double fSum = 0.0f;			
			double dAve = 0.0;
			UniFramePtr  = &(UniFrameList[i]);		
			FrameImageW    = UniFramePtr->ImageW;
			FrameImageH    = UniFramePtr->ImageH;
			FrameImageStep = UniFramePtr->ImageStep;
			FrameBitCount  = UniFramePtr->BitCount;		
			FrameImagePtr  = UniFramePtr->ImagePtr;
			FrameMaskPtr   = UniFramePtr->MaskPtr;
			FrameSpacePtr  = UniFramePtr->SpacePtr;	
			RoiRectSize = (int)((RoiRect.right-RoiRect.left)*(RoiRect.bottom-RoiRect.top));
			if ( ALG_BRIGHT_AVERAGE_FULL == AverageMode )
			{
				for ( i=RoiRect.top; i<RoiRect.bottom; i++ )
				{
					idx = i*FrameImageStep;
					for ( j=RoiRect.left; j<RoiRect.right; j++ )
					{
						if ( ImageAPI.CheckSpaceMaskValid(FrameMaskPtr[idx+j]) == false ) { continue; }
						fSum += (int)(FrameSpacePtr[idx+j]);//為了與BrightRatio同步, 所以轉成整數值再相加
						uCount ++;
					}
				}
			}
			else
			{
				for ( i=RoiRect.top; i<RoiRect.bottom; i++ )
				{
					idx = i*FrameImageStep;
					for ( j=RoiRect.left; j<RoiRect.right; j++ )
					{	
						if ( ImageAPI.CheckSpaceMaskValid(FrameMaskPtr[idx+j]) == false ) { continue; }
						fSum += FrameSpacePtr[idx+j];
						ValueList.push_back((int)(FrameSpacePtr[idx+j]));
						uCount ++;
					}
				}
			}
			if ( uCount > 0 ) 
			{
				dAve = fSum;
				dAve = dAve/uCount;
			}
			else 
			{	dAve = 0.0; }

			if ( ALG_BRIGHT_AVERAGE_PARTIAL == AverageMode )
			{
				const size_t ValCnt = ValueList.size();
				size_t CalCntU = (size_t)(AverageRatioUSL*ValCnt)/100.0;
				size_t CalCntL = (size_t)(AverageRatioLSL*ValCnt)/100.0;
				if ( CalCntU > ValCnt ) { CalCntU = (size_t)(ValCnt); }
				if ( CalCntL > ValCnt ) { CalCntL = (size_t)(ValCnt); }
				const size_t CalCnt = CalCntU-CalCntL;
				fSum = 0;
				std::sort(ValueList.begin(), ValueList.end());
				for ( i=CalCntL; i<CalCntU; i++ )
				{	fSum += ValueList[i];	}
				if ( CalCnt > 0 ) 
				{
					dAve = fSum;
					dAve = dAve/CalCnt;
				}
			}
			if ( m_AlgParamBrightRatio.brAverageScaleEnabled )
			{	dAve *= m_AlgParamBrightRatio.brAverageScale/100.0;	}
			if ( true == BaseValueEnabled ) 
			{	dAve = dAve-BaseValueReading;	}
			m_AlgParamGroupCompare.gc3DHeightValue = dAve;
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//