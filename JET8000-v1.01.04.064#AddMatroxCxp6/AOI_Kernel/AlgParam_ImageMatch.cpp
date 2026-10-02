// AlgParam_ImageMatch.cpp: implementation of the CAlgParam class.
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
bool CAlgParam::BuildPixelCompareParam(const TALG_PARAM_IMAGE_MATCH &imParam, const TPOINT2D &Scale, TPixelCompareParam &tpcParam)
{
	tpcParam.nImgBlurSize = 3;
	tpcParam.nPatDarkLevel = 0;
	tpcParam.nPatLightLevel = 255;

	tpcParam.nCmpDarkLevel = imParam.imPxlCmpDarkLevel;
	tpcParam.nCmpOpenSize = imParam.imPxlCmpOpenSize;
	tpcParam.nCmpCloseSize = imParam.imPxlCmpCloseSize;
	tpcParam.nCmpGaussianSize = imParam.imPxlCmpGaussianSize;	
	tpcParam.nCmpTolerance = imParam.imPxlCmpTolerance;	

	tpcParam.nBlobMinD = 0;
	tpcParam.nBlobMinW = JetAPI::Ceil(imParam.imPxlCmpXSizeMin*Scale.x);
	tpcParam.nBlobMinH = JetAPI::Ceil(imParam.imPxlCmpYSizeMin*Scale.y);
	tpcParam.nBlobMinArea = JetAPI::Ceil(imParam.imPxlCmpAreaMin*Scale.x*Scale.y);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::WriteAlgParamFile_ImageMatch(const TALG_PARAM_IMAGE_MATCH &imParam, CAOIFileIO &FileIO)//纗紇钩で皌把计
{		
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_IMAGE_MATCH_START, 0) == false ) { return false; }

	if ( FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_IMAGE_MATCH_PXL_CMP_ENABLED, imParam.imPxlCmpEnabled) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_IMAGE_MATCH_PXL_CMP_DARK_LEVEL, imParam.imPxlCmpDarkLevel) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_IMAGE_MATCH_PXL_CMP_TOLERANCE, imParam.imPxlCmpTolerance) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_IMAGE_MATCH_PXL_CMP_OPEN_SIZE, imParam.imPxlCmpOpenSize) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_IMAGE_MATCH_PXL_CMP_CLOSE_SIZE, imParam.imPxlCmpCloseSize) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_IMAGE_MATCH_PXL_CMP_GAUSSIAN_SIZE, imParam.imPxlCmpGaussianSize) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_IMAGE_MATCH_PXL_CMP_XSIZE_MIN, imParam.imPxlCmpXSizeMin) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_IMAGE_MATCH_PXL_CMP_YSIZE_MIN, imParam.imPxlCmpYSizeMin) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_IMAGE_MATCH_PXL_CMP_AREA_MIN, imParam.imPxlCmpAreaMin) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_IMAGE_MATCH_PXL_CMP_COUNT_LSL, imParam.imPxlCmpCountLSL) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_IMAGE_MATCH_PXL_CMP_COUNT_USL, imParam.imPxlCmpCountUSL) == false ) { return false; }

	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_IMAGE_MATCH_END, 0) == false ) { return false; }	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ReadAlgParamFile_ImageMatch(TALG_PARAM_IMAGE_MATCH &imParam, CAOIFileIO &FileIO)//更紇钩で皌把计
{
	int       index = 0;	
	while ( FileIO.CheckFileEnd()==false )
	{ 		
		if ( FileIO.LoadChunk(index) == false )		
		{	continue; }
		switch ( index )
		{
		case FILE_IO_ALG_PARAM_IMAGE_MATCH_END://紇钩で皌把计-沧翴
			return true;
			break;
		case FILE_IO_ALG_PARAM_IMAGE_MATCH_PXL_CMP_ENABLED://钩ゑ耕把计-币ノ
			imParam.imPxlCmpEnabled = FileIO.GetData_BOL();
			break;
		case FILE_IO_ALG_PARAM_IMAGE_MATCH_PXL_CMP_DARK_LEVEL://钩ゑ耕把计-穞场
			imParam.imPxlCmpDarkLevel = FileIO.GetData_INT();
			break;
		case FILE_IO_ALG_PARAM_IMAGE_MATCH_PXL_CMP_TOLERANCE://钩ゑ耕把计-粇畉
			imParam.imPxlCmpTolerance = FileIO.GetData_INT();
			break;		
		case FILE_IO_ALG_PARAM_IMAGE_MATCH_PXL_CMP_OPEN_SIZE://钩ゑ耕把计-Open
			imParam.imPxlCmpOpenSize = FileIO.GetData_INT();
			break;
		case FILE_IO_ALG_PARAM_IMAGE_MATCH_PXL_CMP_CLOSE_SIZE://钩ゑ耕把计-Close
			imParam.imPxlCmpCloseSize = FileIO.GetData_INT();
			break;
		case FILE_IO_ALG_PARAM_IMAGE_MATCH_PXL_CMP_GAUSSIAN_SIZE://钩ゑ耕把计-Gaussian
			imParam.imPxlCmpGaussianSize = FileIO.GetData_INT();
			break;
		case FILE_IO_ALG_PARAM_IMAGE_MATCH_PXL_CMP_XSIZE_MIN://钩ゑ耕把计-程へ
			imParam.imPxlCmpXSizeMin = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_IMAGE_MATCH_PXL_CMP_YSIZE_MIN://钩ゑ耕把计-程へ
			imParam.imPxlCmpYSizeMin = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_IMAGE_MATCH_PXL_CMP_AREA_MIN://钩ゑ耕把计-程縩
			imParam.imPxlCmpAreaMin = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_IMAGE_MATCH_PXL_CMP_COUNT_LSL://钩ゑ耕把计-计秖
			imParam.imPxlCmpCountLSL = FileIO.GetData_INT();
			break;
		case FILE_IO_ALG_PARAM_IMAGE_MATCH_PXL_CMP_COUNT_USL://钩ゑ耕把计-计秖
			imParam.imPxlCmpCountUSL = FileIO.GetData_INT();
			break;		
		default:
		#ifdef _DEBUG
			index = index;
		#endif//_DEBUG
			break;
		}
	}
	FileIO.SetErrorString(_T("Error, ReadAlgParamFile_ImageMatch Fault"));
	return false;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ExecAlgInspection_ImageMatch(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList)
{
#ifdef PATTERN_BINARY_USE
	return ExecAlgInspection_ImageMatch_v2(ModelPtr, ModelRgn, RoiRect, WndRect, UniFrameList);
#endif//PATTERN_BINARY_USE
	return ExecAlgInspection_ImageMatch_v1(ModelPtr, ModelRgn, RoiRect, WndRect, UniFrameList);
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ExecAlgInspection_ImageMatch_v1(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList)
{
	if ( NULL == ModelPtr ) { return false; }
	CAOIComponent *ComponentPtr = ModelPtr->GetModelComponentPtr();
	const size_t FrameCount = UniFrameList.size();
	const size_t ImageFrameIndex = m_AlgImageBinParam.GetBinaryFrameIndex();
	if ( ImageFrameIndex >= FrameCount ) 
	{	return false; }	
	CAOIWnd         *WndPtr = CAlgParam::GetAlgWndPtr();
	if ( NULL == WndPtr ) { return false; }

	const int          nRoiAlign = 4;
	BOX_TOWARD         WndToward = WndPtr->GetWndToward();
	const unsigned int WndIndex = WndPtr->GetWndIndex();		
	TUNI_FRAME      *UniFramePtr  = &(UniFrameList[ImageFrameIndex]);
	const IMAGE_SIZE FrameImageW    = UniFramePtr->ImageW;
	const IMAGE_SIZE FrameImageH    = UniFramePtr->ImageH;
	const IMAGE_SIZE FrameImageStep = UniFramePtr->ImageStep;
	const IMAGE_SIZE FrameBitCount  = UniFramePtr->BitCount;		
	IMAGE_PTR        FrameImagePtr  = UniFramePtr->ImagePtr;
	MASK_PTR         FrameMaskPtr   = UniFramePtr->MaskPtr;
	SPACE_PTR        FrameSpacePtr  = UniFramePtr->SpacePtr;	
	TALG_PARAM_IMAGE_MATCH &imParam = GetAlgParamImageMatch();
	const bool       bPatternTestAll = GetAlgPatternTestAll();
	const bool       bGrayInvert = m_AlgImageBinParam.GetGrayInvert();
	const BINARY_MODE     BinaryMode= m_AlgImageBinParam.GetBinaryMode();
	const IMAGE_SRC_MODE  ImageSourceMode = m_AlgImageBinParam.GetBinaryImageSourceMode();

	bool          IsOK = false;
	IMAGE_SIZE    MaskW=0;
	IMAGE_SIZE    MaskH=0;	
	IMAGE_SIZE    MaskStep=0;
	IMAGE_SIZE    MaskBitCount=8;
	MASK_PTR      MaskPtr  = NULL;
	IMAGE_PTR     GrayPtr = NULL;	
	const bool    bTestWnd = false;
	const TPOINT2D   ImageScale = ModelPtr->GetModelImageScale();	
	
	if ( ExecAlgUniFrameBinary(m_AlgImageBinParam, WndRect, RoiRect, UniFrameList, MaskW, MaskH, MaskStep, MaskBitCount, MaskPtr, GrayPtr, bTestWnd) == false )
	{	
		JetMemory.free_func(MaskPtr);
		JetMemory.free_func(GrayPtr);
		return false; 
	}
	IMAGE_PTR       RoiImagePtr = NULL;
	IMAGE_SIZE      RoiImageW = RoiRect.right-RoiRect.left;
	IMAGE_SIZE      RoiImageH = RoiRect.bottom-RoiRect.top;	
	IMAGE_SIZE      RoiBitCount = 0;
	IMAGE_SIZE      RoiImageStep = 0;	

	if ( BINARY_DISABLE == BinaryMode )
	{
		if ( CheckImageSourceGrayMode(ImageSourceMode) == true )
		{
			RoiBitCount = 8;
			RoiImageStep = JetAPI::GetBMPImagePixelsPerLine(RoiImageW, RoiBitCount, nRoiAlign);
			IsOK = ImageAPI.ExtractGrayRoiImage(MaskW, MaskH, MaskStep, GrayPtr, RoiRect, RoiImageStep, RoiImagePtr, false); 
		}
		else
		{
			RoiBitCount = FrameBitCount;
			RoiImageStep = JetAPI::GetBMPImagePixelsPerLine(RoiImageW, RoiBitCount, nRoiAlign);
			IsOK = ImageAPI.ExtractRoiImage(MaskW, MaskH, FrameImageStep, FrameBitCount, FrameImagePtr, RoiRect, RoiImageStep, RoiImagePtr, false);
		}
	}
	else
	{	
		RoiBitCount = 8;
		RoiImageStep = JetAPI::GetBMPImagePixelsPerLine(RoiImageW, RoiBitCount, nRoiAlign);
		IsOK = ImageAPI.ExtractGrayRoiImage(MaskW, MaskH, MaskStep, MaskPtr, RoiRect, RoiImageStep, RoiImagePtr, false); 
	}	
	if ( false == IsOK )
	{
		JetMemory.free_func(GrayPtr);
		JetMemory.free_func(MaskPtr);
		return false;	
	}

	size_t          i=0;
	RECT            PatRect={0,0,0,0};
	CPatternParam  *PatParamPtr = NULL;
	IMAGE_PTR       PatImgPtr=NULL;
	IMAGE_PTR       PatMaskPtr=NULL;
	IMAGE_PTR       PatImgPtrUse=NULL;
	IMAGE_SIZE      PatImgW=0, PatImgH=0, PatImgStep=0, PatBitCount=0;
	IMAGE_SIZE      PatImgWUse=0, PatImgHUse=0, PatImgStepUse=0, PatBitCountUse=0;
	const int       nPatAlign = 4;
	const size_t    PatternCount = GetAlgPatternCount();
	const int       PatternPolarity = GetAlgPatternPolarity();
	JET_MATCH_LIB_TYPE MatchLibType = AOIDataCollect.GetMatchLibType();

	const int       nRoiImageW = (int)(RoiImageW);
	const int       nRoiImageH = (int)(RoiImageH);
	const int       nRoiImageStep = (int)(RoiImageStep);
	const int       nRoiBitCount = (int)(RoiBitCount);
	const int       nRoiX = RoiRect.left;
	const int       nRoiY = RoiRect.top;
	const int       nRoiW = (int)(nRoiImageW);
	const int       nRoiH = (int)(nRoiImageH);
	const int       nRoiRectSize = (int)((nRoiW)*(nRoiH));
	const double    dRoiCpx = (double)((RoiRect.left+RoiRect.right)*0.5);
	const double    dRoiCpy = (double)((RoiRect.top+RoiRect.bottom)*0.5);
	TPOINT2D        WndRectCalValue = WndPtr->GetWndRectCalValue();	

	int    idx = 0;		
	int    NResults = 0;	
	int    PolarityIdx=0;
	int    BestPatternIndex=-1;	
	double ResultCX=0.0, ResultCY=0.0;
	double RoiOffsetX=0.0, RoiOffsetY=0.0;	
	double CadOffsetX=0.0, CadOffsetY=0.0, CadSkew=0.0, CadScaleX=100.0, CadScaleY=100.0;	
	double ResultX=0, ResultY=0, ResultA=0, ResultS=0, ResultSX=0, ResultSY=0;	
	double BestResX=0, BestResY=0, BestResA=0, BestResS=0, BestResSX=0, BestResSY=0;		
	double Reading=0.0, USL=0.0, LSL=0.0;		
	bool   ApplyScale=false;
	const bool      bRobustness = true;		
	const int       nMinReduceArea = GetAlgPatternMinReducedArea();
	const int       nFinalReduction = GetAlgPatternFinalReduction();
	const bool      UseInterpolate = CheckAlgPatternMatrchInterpolate();
	const bool      bAdvancedLearning = GetAlgPatternAdvancedLearning();
	const bool      bPxlCmpEnabed = imParam.imPxlCmpEnabled;
	const int       nPxlCmpCountLSL = imParam.imPxlCmpCountLSL;
	const int       nPxlCmpCountUSL = imParam.imPxlCmpCountUSL;
	CString      str;	
	CJetMatch    Match;	//紇钩で皌	
	TPixelCompareParam tpcParam;
	int SubRectPosX=0;
	int SubRectPosY=0;
	std::vector<RECT> SubRectList;
#ifdef _DEBUG	
	bool         bSave = true;	
	CString      ComponentName;	
	CString      DebugFolder=GetAlgDebugFolder();
	if ( NULL != ComponentPtr )
	{	ComponentName = ComponentPtr->GetComponentFullName(); }	

	if ( true == bSave )
	{
		str.Format(_T("%s\\%s_ExecAlgInspection_ImageMatch#%d_Roi.PNG"), DebugFolder, ComponentName, WndIndex+1);
		ImageAPI.SaveImage(str, nRoiImageW, nRoiImageH, nRoiImageStep, nRoiBitCount, RoiImagePtr, true);
	}	
#endif//_DEBUG

	
	if ( Match.SetMatchLibType(MatchLibType)==false )
	{	
		JetMemory.free_func(GrayPtr);
		JetMemory.free_func(MaskPtr);
		JetMemory.free_func(RoiImagePtr);
		return false;	
	}

	USL = GetAlgPatternSimilarityUSL();
	LSL = GetAlgPatternSimilarityLSL();	
	BuildPixelCompareParam(imParam, ImageScale, tpcParam);	

	for ( i=0; i<PatternCount; i++ )
	{
		PatParamPtr = CAlgParam::GetAlgPatternParamPtr(i, true);
		if ( NULL == PatParamPtr ) 
		{	continue;	}
		if ( CAlgParam::LoadAlgPatternImage(i, WndToward, PatImgW, PatImgH, PatImgStep, PatBitCount, PatImgPtr) == false ) 
		{	continue;	}

		JetAPI::SizeToRect(PatImgW, PatImgH, PatRect);		
		if ( BINARY_DISABLE != BinaryMode )//惠璶ゑ耕琌ㄏノ2て紇钩
		{	
			PatImgWUse = PatImgW;
			PatImgHUse = PatImgH;
			PatBitCountUse = 8;
			PatImgStepUse = JetAPI::GetBMPImagePixelsPerLine(PatImgW, MaskBitCount, nPatAlign);
			if ( BINARY_DISABLE != PatParamPtr->GetBinaryParam().GetBinaryMode() )
			{	//妓狾把计Τ砞﹚﹚2て把计
				if ( ExecAlgImageBinary_Rect(PatParamPtr->GetBinaryParam(), PatRect, PatRect, PatImgW, PatImgH, PatImgStep, PatBitCount, PatImgPtr, NULL, NULL, nPatAlign, PatMaskPtr) == false )
				{
					JetMemory.free_func(PatImgPtr);
					JetMemory.free_func(PatMaskPtr);
					continue;	
				}				
			}
			else
			{	//妓狾把计ゼ砞﹚2て把计
				if ( ImageAPI.ColorImageToGrayImage(PatImgW, PatImgH, PatImgStep, PatImgPtr, PatRect, PatImgStepUse, PatMaskPtr, 
					ImageSourceMode, //籔浪代紇钩ㄓ方
					PatParamPtr->GetBinaryParam().GetBinarySynthesisWR(), 
					PatParamPtr->GetBinaryParam().GetBinarySynthesisWG(), 
					PatParamPtr->GetBinaryParam().GetBinarySynthesisWB(), 
					false) == false )
				{
					JetMemory.free_func(PatImgPtr);
					JetMemory.free_func(PatMaskPtr);
					continue;
				}
			}
			PatImgPtrUse = PatMaskPtr;
		}
		else
		{
			if ( CheckImageSourceGrayMode(ImageSourceMode) == true )
			{
				if ( 24 == PatBitCount )
				{
					PatImgWUse = PatImgW;
					PatImgHUse = PatImgH;
					PatBitCountUse = 8;
					PatImgStepUse = JetAPI::GetBMPImagePixelsPerLine(PatImgW, MaskBitCount, nPatAlign);
					if ( ImageAPI.ColorImageToGrayImage(PatImgW, PatImgH, PatImgStep, PatImgPtr, PatRect, PatImgStepUse, PatMaskPtr, 
						ImageSourceMode, //籔浪代紇钩ㄓ方
						PatParamPtr->GetBinaryParam().GetBinarySynthesisWR(), 
						PatParamPtr->GetBinaryParam().GetBinarySynthesisWG(), 
						PatParamPtr->GetBinaryParam().GetBinarySynthesisWB(), 
						false) == false )
					{
						JetMemory.free_func(PatImgPtr);
						JetMemory.free_func(PatMaskPtr);
						continue;
					}
					PatImgPtrUse = PatMaskPtr;
				}
				else
				{
					PatImgWUse = PatImgW;
					PatImgHUse = PatImgH;
					PatImgStepUse = PatImgStep;
					PatBitCountUse = PatBitCount;
					PatImgPtrUse = PatImgPtr;
				}
				if ( true == bGrayInvert )
				{	
					if ( ImageAPI.InvertImage(PatImgWUse, PatImgHUse, PatImgStepUse, PatBitCountUse, PatImgPtrUse, PatImgPtrUse) == false )
					{
						JetMemory.free_func(PatImgPtr);
						JetMemory.free_func(PatMaskPtr);
						continue; 
					}
				}
			}
			else
			{
				PatImgWUse = PatImgW;
				PatImgHUse = PatImgH;
				PatImgStepUse = PatImgStep;
				PatBitCountUse = PatBitCount;
				PatImgPtrUse = PatImgPtr;
			}
		}		
		if ( PatBitCountUse != RoiBitCount )
		{
			JetMemory.free_func(PatImgPtr);
			JetMemory.free_func(PatMaskPtr);			
			continue; 
		}
	
		//initial eMatch
		Match.SetMatchDefaultParam();
		Match.SetRobustness(bRobustness);
		Match.SetMinReducedArea(nMinReduceArea);
		Match.SetFinalReduction(nFinalReduction);
		if ( Match.LearnPattern(PatImgWUse, PatImgHUse, PatImgStepUse, PatBitCountUse, PatImgPtrUse, true) == false )
		{
			JetMemory.free_func(PatImgPtr);
			JetMemory.free_func(PatMaskPtr);
			continue; 
		}		
	#ifdef _DEBUG	
		if ( true == bSave )
		{
			str.Format(_T("%s\\%s_ExecAlgInspection_ImageMatch#%d_Pat.PNG"), DebugFolder, ComponentName, WndIndex+1);
			ImageAPI.SaveImage(str, PatImgWUse, PatImgHUse, PatImgStepUse, PatBitCountUse, PatImgPtrUse, true);
		}		
	#endif//_DEBUG
		if ( true == GetAlgSkewEnabled())
		{	
			double dSkew = GetAlgSkewCalcRange();
			float fSkewMax =  (float)(dSkew);
			float fSkewMin = -(float)(dSkew);
			Match.SetMinAngle(fSkewMin);
			Match.SetMaxAngle(fSkewMax);
			Match.SetUseAngle(true);				
		}
		if ( true == GetAlgScaleEnabled() )
		{
			double MatchScaleMin = 1.0, MatchScaleMax = 1.0;
			GetAlgScaleCalcRange(MatchScaleMin, MatchScaleMax);
			Match.SetMinScale(MatchScaleMin);			
			Match.SetMaxScale(MatchScaleMax);
			if ( false == GetAlgPatternScaleIsotropic() )
			{
				Match.SetMinScaleX(MatchScaleMin);
				Match.SetMinScaleY(MatchScaleMin);
				Match.SetMaxScaleX(MatchScaleMax);
				Match.SetMaxScaleY(MatchScaleMax);
			}			
			Match.SetUseScale(true);	
		}

		Match.SetInterpolate(UseInterpolate);
		//Match.SetInterpolate(false);
		Match.SetMinScore(-1);//fMinScore
		if ( Match.Match(nRoiImageW, nRoiImageH, nRoiImageStep, nRoiBitCount, RoiImagePtr, true) == false )
		{
			JetMemory.free_func(PatImgPtr);
			JetMemory.free_func(PatMaskPtr);
			continue;	
		}

		NResults = Match.GetNumPositions();
		if ( NResults > 0 )
		{
			idx = 0;
			ResultS = Match.GetResultScore(idx)*100.0;
			ResultX = Match.GetResultPosX(idx);
			ResultY = Match.GetResultPosY(idx);
			ResultA = Match.GetResultAngle(idx);		
			ResultSX = Match.GetResultScaleX(idx);
			ResultSY = Match.GetResultScaleY(idx);		
			if ( ResultS < 0.0 ) { ResultS = 0.0; }

			ResultCX = ResultX+nRoiX;
			ResultCY = ResultY+nRoiY;
			RoiOffsetX = ResultCX-dRoiCpx;
			RoiOffsetY = dRoiCpy-ResultCY;
			CadSkew = -ResultA;
			CadOffsetX = RoiOffsetX/ImageScale.x;
			CadOffsetY = RoiOffsetY/ImageScale.y;
			CadOffsetX = CadOffsetX + WndRectCalValue.x;
			CadOffsetY = CadOffsetY + WndRectCalValue.y;
			CadScaleX = ResultSX*100.0;
			CadScaleY = ResultSY*100.0;		

			PolarityIdx = 0;
			PatParamPtr->SetPatResultW(PolarityIdx, PatImgWUse);
			PatParamPtr->SetPatResultH(PolarityIdx, PatImgHUse);
			PatParamPtr->SetPatResultX(PolarityIdx, ResultX);
			PatParamPtr->SetPatResultY(PolarityIdx, ResultY);
			PatParamPtr->SetPatResultSkew(PolarityIdx, ResultA);
			PatParamPtr->SetPatResultScore(PolarityIdx, ResultS);
			PatParamPtr->SetPatResultScaleX(PolarityIdx, ResultSX);
			PatParamPtr->SetPatResultScaleY(PolarityIdx, ResultSY);			

			PatParamPtr->SetResultX(PolarityIdx, CadOffsetX);
			PatParamPtr->SetResultY(PolarityIdx, CadOffsetY);
			PatParamPtr->SetResultSkew(PolarityIdx, CadSkew);			
			PatParamPtr->SetResultScaleX(PolarityIdx, ResultSX);
			PatParamPtr->SetResultScaleY(PolarityIdx, ResultSY);			
			PatParamPtr->SetReultID(PolarityIdx, RESULT_ID_NG);
			PatParamPtr->SetResultReading(PolarityIdx, ResultS);
			PatParamPtr->CheckResultIndex();

			if ( ResultS > BestResS )
			{
				BestResS = ResultS;
				BestResX = ResultX;
				BestResY = ResultY;
				BestResA = ResultA;
				BestResSX = ResultSX;
				BestResSY = ResultSY;
				BestPatternIndex = (int)(i);				
			}
			
			if ( ResultS>=LSL && ResultS<=USL )
			{
				PatParamPtr->SetReultID(PolarityIdx, RESULT_ID_OK);
				PatParamPtr->CheckResultIndex();
				if ( false == bPxlCmpEnabed )
				{		
					JetMemory.free_func(PatImgPtr);
					JetMemory.free_func(PatMaskPtr);
					if ( false == bPatternTestAll ) 
					{	break;	}
					else
					{	continue; }
				}
				else
				{					
					RECT SubRect={0,0,0,0};
					IMAGE_PTR  SubImgPtr = NULL;
					IMAGE_SIZE SubImgW=PatImgWUse;
					IMAGE_SIZE SubImgH=PatImgHUse;
					IMAGE_SIZE SubImgStep=PatImgStepUse;					
					//SubRect.left = JetAPI::Floor(ResultX)-(SubImgW/2);
					//SubRect.top  = JetAPI::Floor(ResultY)-(SubImgH/2);
					SubRect.left = (int)(ResultX-(SubImgW*0.5)+0.5);
					SubRect.top  = (int)(ResultY-(SubImgH*0.5)+0.5);
					SubRect.right = SubRect.left+SubImgW;
					SubRect.bottom = SubRect.top+SubImgH;
					SubRectPosX = SubRect.left;
					SubRectPosY = SubRect.top;
					if ( ImageAPI.ExtractRoiImage(RoiImageW, RoiImageH, RoiImageStep, RoiBitCount, RoiImagePtr, SubRect, SubImgStep, SubImgPtr, false) == false )
					{
						JetMemory.free_func(GrayPtr);
						JetMemory.free_func(MaskPtr);
						JetMemory.free_func(PatImgPtr);
						JetMemory.free_func(PatMaskPtr);
						return false;
					}			

				#ifdef _DEBUG
					if ( true == bSave )
					{								
						str.Format(_T("%s\\%s_ModelWndAlgImaegMatchPixelCompare#%d_PatT#%d.PNG"), DebugFolder, ComponentName, WndIndex+1, i+1);
						ImageAPI.SaveImage(str, PatImgWUse, PatImgHUse, PatImgStepUse, PatBitCountUse, PatImgPtrUse, true);				
						str.Format(_T("%s\\%s_ModelWndAlgImaegMatchPixelCompare#%d_ImgT#%d.PNG"), DebugFolder, ComponentName, WndIndex+1, i+1);
						ImageAPI.SaveImage(str, SubImgW, SubImgH, SubImgStep, RoiBitCount, SubImgPtr, true);
					}
				#endif//_DEBUG
					SubRectList.clear();
					if ( ImageAPI.Compare2ImagePixel(PatImgWUse, PatImgHUse, PatImgStepUse, PatBitCountUse, PatImgPtrUse, SubImgPtr, NULL, tpcParam, SubRectList) == false )
					{
						JetMemory.free_func(GrayPtr);
						JetMemory.free_func(MaskPtr);
						JetMemory.free_func(SubImgPtr);
						JetMemory.free_func(PatImgPtr);
						JetMemory.free_func(PatMaskPtr);
						return false;
					}
					JetMemory.free_func(SubImgPtr);
					const int TmpResultCnt = (int)(SubRectList.size());					
					if ( TmpResultCnt>=nPxlCmpCountLSL && TmpResultCnt<=nPxlCmpCountUSL )
					{
						JetMemory.free_func(PatImgPtr);
						JetMemory.free_func(PatMaskPtr);
						if ( false == bPatternTestAll ) 
						{	break;	}
						else
						{	continue; }
					}
				}		
			}				
		}

		if ( 2 == PatternPolarity )
		{
			IMAGE_PTR  PatImgPtr2 = NULL;
			IMAGE_SIZE PatImgW2, PatImgH2, PatImgStep2;
			if ( ImageAPI.RotateImage(180.0, PatImgWUse, PatImgHUse, PatImgStepUse, PatBitCountUse, PatImgPtrUse, PatImgW2, PatImgH2, PatImgStep2, PatImgPtr2) == false )
			{	
				JetMemory.free_func(PatImgPtr);
				JetMemory.free_func(PatMaskPtr);
				continue;
			}
			
			Match.SetMatchDefaultParam();
			if ( Match.LearnPattern(PatImgW2, PatImgH2, PatImgStep2, PatBitCountUse, PatImgPtr2, true) == false )
			{
				JetMemory.free_func(PatImgPtr);
				JetMemory.free_func(PatImgPtr2);
				JetMemory.free_func(PatMaskPtr);
				continue; 
			}		

			if ( true == GetAlgSkewEnabled() )
			{	
				double dSkew = GetAlgSkewCalcRange();
				float fSkewMax =  (float)(dSkew);
				float fSkewMin = -(float)(dSkew);
				Match.SetMinAngle(fSkewMin);
				Match.SetMaxAngle(fSkewMax);
				Match.SetUseAngle(true);				
			}
			if ( true == GetAlgScaleEnabled() )
			{	
				double MatchScaleMin = 1.0, MatchScaleMax = 1.0;
				GetAlgScaleCalcRange(MatchScaleMin, MatchScaleMax);
				Match.SetMinScale(MatchScaleMin);			
				Match.SetMaxScale(MatchScaleMax);			
				if ( false == GetAlgPatternScaleIsotropic() )
				{
					Match.SetMinScaleX(MatchScaleMin);
					Match.SetMinScaleY(MatchScaleMin);
					Match.SetMaxScaleX(MatchScaleMax);
					Match.SetMaxScaleY(MatchScaleMax);
				}			
				Match.SetUseScale(true);				
			}
			Match.SetInterpolate(UseInterpolate);
			Match.SetMinScore(-1);//fMinScore
			if ( Match.Match(nRoiImageW, nRoiImageH, nRoiImageStep, nRoiBitCount, RoiImagePtr, true) == false )
			{
				JetMemory.free_func(PatImgPtr);
				JetMemory.free_func(PatImgPtr2);
				JetMemory.free_func(PatMaskPtr);
				continue;	
			}

			NResults = Match.GetNumPositions();
			if ( NResults > 0 )
			{
				idx = 0;
				ResultS = Match.GetResultScore(idx)*100.0;
				ResultX = Match.GetResultPosX(idx);
				ResultY = Match.GetResultPosY(idx);
				ResultA = Match.GetResultAngle(idx);		
				ResultSX = Match.GetResultScaleX(idx);
				ResultSY = Match.GetResultScaleY(idx);		
				if ( ResultS < 0.0 ) { ResultS = 0.0; }

				ResultCX = ResultX+nRoiX;
				ResultCY = ResultY+nRoiY;
				RoiOffsetX = ResultCX-dRoiCpx;
				RoiOffsetY = dRoiCpy-ResultCY;
				CadSkew = -ResultA;
				CadOffsetX = RoiOffsetX/ImageScale.x;
				CadOffsetY = RoiOffsetY/ImageScale.y;
				CadOffsetX = CadOffsetX + WndRectCalValue.x;
				CadOffsetY = CadOffsetY + WndRectCalValue.y;

				PolarityIdx = 1;
				PatParamPtr->SetPatResultW(PolarityIdx, PatImgWUse);
				PatParamPtr->SetPatResultH(PolarityIdx, PatImgHUse);
				PatParamPtr->SetPatResultX(PolarityIdx, ResultX);
				PatParamPtr->SetPatResultY(PolarityIdx, ResultY);
				PatParamPtr->SetPatResultSkew(PolarityIdx, ResultA);
				PatParamPtr->SetPatResultScore(PolarityIdx, ResultS);
				PatParamPtr->SetPatResultScaleX(PolarityIdx, ResultSX);
				PatParamPtr->SetPatResultScaleY(PolarityIdx, ResultSY);

				PatParamPtr->SetResultX(PolarityIdx, CadOffsetX);
				PatParamPtr->SetResultY(PolarityIdx, CadOffsetY);
				PatParamPtr->SetResultSkew(PolarityIdx, CadSkew);				
				PatParamPtr->SetResultScaleX(PolarityIdx, ResultSX);
				PatParamPtr->SetResultScaleY(PolarityIdx, ResultSY);				
				PatParamPtr->SetReultID(PolarityIdx, RESULT_ID_NG);
				PatParamPtr->SetResultReading(PolarityIdx, ResultS);
				PatParamPtr->CheckResultIndex();

				if ( ResultS > BestResS )
				{
					BestResS = ResultS;
					BestResX = ResultX;
					BestResY = ResultY;
					BestResA = ResultA;
					BestResSX = ResultSX;
					BestResSY = ResultSY;
					BestPatternIndex = (int)(i);					
				}
				if ( ResultS>=LSL && ResultS<=USL )
				{
					PatParamPtr->SetReultID(PolarityIdx, RESULT_ID_OK);
					PatParamPtr->CheckResultIndex();
					if ( false == bPxlCmpEnabed )
					{
						JetMemory.free_func(PatImgPtr2);
						JetMemory.free_func(PatImgPtr);
						JetMemory.free_func(PatMaskPtr);
						if ( false == bPatternTestAll ) 
						{	break;	}
						else
						{	continue; }
					}
					else
					{						
						RECT SubRect={0,0,0,0};
						IMAGE_PTR  SubImgPtr = NULL;
						IMAGE_SIZE SubImgW=PatImgW2;
						IMAGE_SIZE SubImgH=PatImgH2;
						IMAGE_SIZE SubImgStep=PatImgStep2;
						//SubRect.left = JetAPI::Floor(ResultX)-(SubImgW/2);
						//SubRect.top  = JetAPI::Floor(ResultY)-(SubImgH/2);
						SubRect.left = (int)(ResultX-(SubImgW*0.5)+0.5);
						SubRect.top  = (int)(ResultY-(SubImgH*0.5)+0.5);
						SubRect.right = SubRect.left+SubImgW;
						SubRect.bottom = SubRect.top+SubImgH;
						SubRectPosX = SubRect.left;
						SubRectPosY = SubRect.top;
						if ( ImageAPI.ExtractRoiImage(RoiImageW, RoiImageH, RoiImageStep, RoiBitCount, RoiImagePtr, SubRect, SubImgStep, SubImgPtr, false) == false )
						{
							JetMemory.free_func(GrayPtr);
							JetMemory.free_func(MaskPtr);
							JetMemory.free_func(PatImgPtr2);
							JetMemory.free_func(PatImgPtr);
							JetMemory.free_func(PatMaskPtr);
							return false;
						}			
					#ifdef _DEBUG
						if ( true == bSave )
						{								
							str.Format(_T("%s\\%s_ModelWndAlgImaegMatchPixelCompare#%d_PatB#%d.PNG"), DebugFolder, ComponentName, WndIndex+1, i+1);
							ImageAPI.SaveImage(str, PatImgW2, PatImgH2, PatImgStep2, PatBitCountUse, PatImgPtr2, true);				
							str.Format(_T("%s\\%s_ModelWndAlgImaegMatchPixelCompare#%d_ImgB#%d.PNG"), DebugFolder, ComponentName, WndIndex+1, i+1);
							ImageAPI.SaveImage(str, SubImgW, SubImgH, SubImgStep, RoiBitCount, SubImgPtr, true);
						}
					#endif//_DEBUG
						SubRectList.clear();
						if ( ImageAPI.Compare2ImagePixel(PatImgW2, PatImgH2, PatImgStep2, PatBitCountUse, PatImgPtr2, SubImgPtr, NULL, tpcParam, SubRectList) == false )
						{
							JetMemory.free_func(GrayPtr);
							JetMemory.free_func(MaskPtr);
							JetMemory.free_func(SubImgPtr);
							JetMemory.free_func(PatImgPtr2);
							JetMemory.free_func(PatImgPtr);
							JetMemory.free_func(PatMaskPtr);
							return false;
						}
						JetMemory.free_func(SubImgPtr);
						const int TmpResultCnt = (int)(SubRectList.size());					
						if ( TmpResultCnt>=nPxlCmpCountLSL && TmpResultCnt<=nPxlCmpCountUSL )
						{
							JetMemory.free_func(PatImgPtr2);
							JetMemory.free_func(PatImgPtr);
							JetMemory.free_func(PatMaskPtr);
							if ( false == bPatternTestAll ) 
							{	break;	}
							else
							{	continue; }
						}
					}
				}
			}
			JetMemory.free_func(PatImgPtr2);			
		}
		JetMemory.free_func(PatImgPtr);
		JetMemory.free_func(PatMaskPtr);
	}
	JetMemory.free_func(RoiImagePtr);
	
	bool  bMatchScore=false;
	PatParamPtr = GetAlgPatternParamPtr(BestPatternIndex, true);
	if ( NULL != PatParamPtr ) 
	{
		PolarityIdx = PatParamPtr->GetResultPolarityIdx();
		ResultS = PatParamPtr->GetResultReading(PolarityIdx);
		ResultX = PatParamPtr->GetPatResultX(PolarityIdx);
		ResultY = PatParamPtr->GetPatResultY(PolarityIdx);
		ResultA = PatParamPtr->GetPatResultSkew(PolarityIdx);
		ResultSX = PatParamPtr->GetPatResultScaleX(PolarityIdx);
		ResultSY = PatParamPtr->GetPatResultScaleY(PolarityIdx);		

		ResultCX = ResultX+nRoiX;
		ResultCY = ResultY+nRoiY;
		RoiOffsetX = ResultCX-dRoiCpx;
		RoiOffsetY = dRoiCpy-ResultCY;
		CadSkew = -ResultA;
		CadOffsetX = RoiOffsetX/ImageScale.x;
		CadOffsetY = RoiOffsetY/ImageScale.y;
		CadOffsetX = CadOffsetX + WndRectCalValue.x;
		CadOffsetY = CadOffsetY + WndRectCalValue.y;
		CadScaleX = ResultSX*100.0;
		CadScaleY = ResultSY*100.0;		
		bMatchScore = true;
	}
	if ( BestPatternIndex<0 && PatternCount>0 ) 
	{	BestPatternIndex = 0; }
	
	Reading = ResultS;
	USL = GetAlgPatternSimilarityUSL();
	LSL = GetAlgPatternSimilarityLSL();
	SetAlgPatternSimilarityReading(Reading);
	SetAlgPatternResultIndex(BestPatternIndex);		

	CString strResult;	
	SetAlgResultReading1(Reading);	
	SetAlgResultText(_T("OK"));
	SetAlgResultID(RESULT_ID_OK);	
	if ( Reading<LSL || Reading>USL )
	{
		strResult.Format(_T("NG:%.0f / %.0f"), Reading, LSL);		
		SetAlgResultID(RESULT_ID_NG);
		SetAlgResultText(strResult);
	}
	RECT SubRect={0};
	const int SubRectCnt = (int)(SubRectList.size());					
	if ( true == bPxlCmpEnabed )
	{
		imParam.imPxlCmpCountNum = SubRectCnt;
		if ( SubRectCnt<nPxlCmpCountLSL || SubRectCnt>nPxlCmpCountUSL )
		{		
			strResult.Format(_T("NG:%d / %d"), SubRectCnt, nPxlCmpCountUSL);			
			SetAlgResultID(RESULT_ID_NG);
			SetAlgResultText(strResult);
		}
		//Result Box	
		CAOIBox   ResBox;
		TPOINT2D  RgnCp;
		TPOINT2D  ImageCp;
		TREGION4D BoxRgn;
		TREGION4D WndRgn;

		ImageCp.x = RoiImageW;
		ImageCp.y = RoiImageH;
		ImageCp.x = ImageCp.x*0.5;
		ImageCp.y = ImageCp.y*0.5;		
		WndPtr->GetWndRegion(WndRgn);		
		RgnCp.x = WndRgn.GetCpX();
		RgnCp.y = WndRgn.GetCpY();		
		for ( i=0; i<SubRectCnt; i++ )
		{
			SubRect = SubRectList[i];
			::InflateRect(&SubRect, 1, 1);
			SubRect.left   += SubRectPosX;
			SubRect.top    += SubRectPosY;
			SubRect.right  += SubRectPosX;
			SubRect.bottom += SubRectPosY;
			CAOIModel::CalcModelBoxRectRegion(SubRect, RoiImageW, RoiImageH, RgnCp, ImageScale, ImageCp, BoxRgn);			
			ResBox.SetBoxRegion(BoxRgn);			
			WndPtr->AddWndResultBox(ResBox);
		}				
	}
	
	double CadSkew_Self = 0;
	double CadOffsetX_Self = 0;
	double CadOffsetY_Self = 0;
	double CadSkew_Others = 0;
	double CadOffsetX_Others = 0;
	double CadOffsetY_Others = 0;	
	const bool bChkDefect=true;
	const bool ApplySkewAngle = true;	
	const bool AlgScaleEnabled = GetAlgScaleEnabled();
	/*
	bool IsExceptionAngle = false;
	if ( NULL != ComponentPtr )
	{
		const double ComponentAngle = ComponentPtr->GetComponentAngle();
		IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComponentAngle);
		if ( true == IsExceptionAngle )
		{
			double AngleRadius = ComponentAngle*DEG_TO_RAD_DBL;
			const double COS = cos(AngleRadius);
			const double SIN = sin(AngleRadius);
			const double dCPX = 0.0;
			const double dCPY = 0.0;			
			CadOffsetX3 = (CadOffsetX*COS)-(CadOffsetY*SIN)+dCPX;
			CadOffsetY3 = (CadOffsetX*SIN)+(CadOffsetY*COS)+dCPY;			
		}
	}	
	*/
	SetAlgSkewReading(CadSkew);	
	SetAlgImageOffsetX(RoiOffsetX);
	SetAlgImageOffsetY(RoiOffsetY);
	SetAlgOffsetXReading(CadOffsetX);
	SetAlgOffsetYReading(CadOffsetY);
	CalcAlgOffsetL();
	CheckAlgOffset(CadOffsetX_Self, CadOffsetY_Self, CadSkew_Self, CadOffsetX_Others, CadOffsetY_Others, CadSkew_Others, bChkDefect);

	if ( true == AlgScaleEnabled )
	{
		ApplyScale = true;		
		USL = GetAlgScaleUSL();
		LSL = GetAlgScaleLSL();
		SetAlgScaleXReading(CadScaleX);
		SetAlgScaleYReading(CadScaleY);		
		if ( CadScaleX<LSL || CadScaleY<LSL || CadScaleX>USL || CadScaleY>USL )
		{
			ApplyScale = false;
			CString Key = AOIDataDefine.GetRatioText();
			strResult.Format(_T("%s:(%.0f, %.0f) (%.0f~%.0f)"), Key, CadScaleX, CadScaleY, LSL, USL);			
			SetAlgResultID(RESULT_ID_NG);
			SetAlgResultText(strResult);
		}
	}

	WndPtr->SetWndResultID(m_AlgResultID);
	WndPtr->SetWndResultText(m_AlgResultText);	
	SaveAlgDefectImage(MaskW, MaskH, MaskStep, MaskBitCount, GrayPtr, MaskPtr, WndRect);
	JetMemory.free_func(GrayPtr);
	JetMemory.free_func(MaskPtr);

	bool  ApplySkew = false;
	bool  ApplyOffsetX = false;
	bool  ApplyOffsetY = false;
	if ( fabs(CadSkew_Others) > 0.001 ) { ApplySkew = true; }
	if ( fabs(CadOffsetX_Others) > 0.001 ) { ApplyOffsetX = true; }
	if ( fabs(CadOffsetY_Others) > 0.001 ) { ApplyOffsetY = true; }
	//甅ノ浪代
	if ( true == bMatchScore )
	{
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
		if ( true == AlgScaleEnabled )
		{	BoxPtr->ScaleBoxSizeRes(CadScaleX/100.0, CadScaleY/100.0);	}
		BoxPtr->SkewBoxAngle(CadSkew_Self);
		BoxPtr->MoveBoxRes(CadOffsetX_Self, CadOffsetY_Self);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ExecAlgInspection_ImageMatch_v2(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList)
{
	const char fnName[]="CAlgParam::ExecAlgInspection_ImageMatch";
	if ( NULL == ModelPtr ) { return false; }
	CAOIComponent *ComponentPtr = ModelPtr->GetModelComponentPtr();
	const size_t FrameCount = UniFrameList.size();
	const size_t ImageFrameIndex = m_AlgImageBinParam.GetBinaryFrameIndex();
	if ( ImageFrameIndex >= FrameCount ) 
	{	return false; }	
	CAOIWnd         *WndPtr = CAlgParam::GetAlgWndPtr();
	if ( NULL == WndPtr ) { return false; }

	const int          nRoiAlign = 4;
	BOX_TOWARD         WndToward = WndPtr->GetWndToward();
	const unsigned int WndIndex = WndPtr->GetWndIndex();		
	const TUNI_FRAME   &UniFrameRef  = (UniFrameList[ImageFrameIndex]);	
	TALG_PARAM_IMAGE_MATCH &imParam = GetAlgParamImageMatch();
	const bool       bPatternTestAll = GetAlgPatternTestAll();	
	const TPOINT2D   ImageScale = ModelPtr->GetModelImageScale();

	bool          IsOK = false;
	TUNI_FRAME    RawUniFrame;
	const int     nRawAlign = 4;	
	if ( ImageAPI.ExtractUniRoiImage(UniFrameRef, RoiRect, nRawAlign, fnName, RawUniFrame) == false )
	{	return false;	}

	MASK_PTR        RoiMaskPtr = NULL;
	IMAGE_PTR       RoiGrayPtr = NULL;
	IMAGE_PTR       RoiImgPtrUse = NULL;
	IMAGE_SIZE      RoiImageW = 0;
	IMAGE_SIZE      RoiImageH = 0;
	IMAGE_SIZE      RoiBitCount = 0;
	IMAGE_SIZE      RoiImageStep = 0;	

	IMAGE_SIZE      RawImageW = RawUniFrame.ImageW;	
	IMAGE_SIZE      RawImageH = RawUniFrame.ImageH;	
	IMAGE_SIZE      RawBitCount = RawUniFrame.BitCount;	
	IMAGE_SIZE      RawImageStep = RawUniFrame.ImageStep;
	MASK_PTR        RawMaskPtr = RawUniFrame.MaskPtr;
	IMAGE_PTR       RawImagePtr = RawUniFrame.ImagePtr;		
	SPACE_PTR       RawSpacePtr = RawUniFrame.SpacePtr;			
	const size_t    RawImageSize= ImageAPI.CalcBufferSize(RawImageStep, RawImageH);
	IMAGE_PTR       RawImagePtr_Clone = NULL;	

	size_t          i=0;
	RECT            PatRect={0,0,0,0};
	CPatternParam  *PatParamPtr = NULL;
	IMAGE_PTR       PatImgPtr=NULL;
	IMAGE_PTR       PatImgPtr_Clone=NULL;
	IMAGE_PTR       PatGrayPtr=NULL;
	IMAGE_PTR       PatMaskPtr=NULL;
	IMAGE_PTR       PatImgPtrUse=NULL;
	IMAGE_SIZE      PatImgW=0, PatImgH=0, PatImgStep=0, PatBitCount=0;
	IMAGE_SIZE      PatImgWUse=0, PatImgHUse=0, PatImgStepUse=0, PatBitCountUse=0;
	const int       nPatAlign = 4;
	const size_t    PatternCount = GetAlgPatternCount();
	const int       PatternPolarity = GetAlgPatternPolarity();
	JET_MATCH_LIB_TYPE MatchLibType = AOIDataCollect.GetMatchLibType();
	
	const int       nRoiX = RoiRect.left;
	const int       nRoiY = RoiRect.top;
	const int       nRoiW = (int)(RawImageW);
	const int       nRoiH = (int)(RawImageH);
	const int       nRoiRectSize = (int)((nRoiW)*(nRoiH));
	const double    dRoiCpx = (double)((RoiRect.left+RoiRect.right)*0.5);
	const double    dRoiCpy = (double)((RoiRect.top+RoiRect.bottom)*0.5);
	TPOINT2D        WndRectCalValue = WndPtr->GetWndRectCalValue();	

	int    idx = 0;		
	int    NResults = 0;	
	int    PolarityIdx=0;
	int    BestPatternIndex=-1;	
	double ResultCX=0.0, ResultCY=0.0;
	double RoiOffsetX=0.0, RoiOffsetY=0.0;	
	double CadOffsetX=0.0, CadOffsetY=0.0, CadSkew=0.0, CadScaleX=100.0, CadScaleY=100.0;	
	double ResultX=0, ResultY=0, ResultA=0, ResultS=0, ResultSX=0, ResultSY=0;	
	double BestResX=0, BestResY=0, BestResA=0, BestResS=0, BestResSX=0, BestResSY=0;		
	double Reading=0.0, USL=0.0, LSL=0.0;		
	bool   ApplyScale=false;
	const bool      bRobustness = true;		
	const int       nMinReduceArea = GetAlgPatternMinReducedArea();
	const int       nFinalReduction = GetAlgPatternFinalReduction();
	const bool      UseInterpolate = CheckAlgPatternMatrchInterpolate();
	const bool      bAdvancedLearning = GetAlgPatternAdvancedLearning();
	const bool      bPxlCmpEnabed = imParam.imPxlCmpEnabled;
	const int       nPxlCmpCountLSL = imParam.imPxlCmpCountLSL;
	const int       nPxlCmpCountUSL = imParam.imPxlCmpCountUSL;
	CString      str;	
	CJetMatch    Match;	//紇钩で皌	
	TPixelCompareParam tpcParam;
	int SubRectPosX=0;
	int SubRectPosY=0;
	std::vector<RECT> SubRectList;
#ifdef _DEBUG	
	bool         bSave = true;	
	CString      ComponentName;
	CString      DebugFolder=GetAlgDebugFolder();
	if ( NULL != ComponentPtr )
	{	ComponentName = ComponentPtr->GetComponentFullName(); }	
#endif//_DEBUG
	
	if ( Match.SetMatchLibType(MatchLibType)==false )
	{	
		JetAPI::ClearUniFrame(RawUniFrame);		
		return false;	
	}

	USL = GetAlgPatternSimilarityUSL();
	LSL = GetAlgPatternSimilarityLSL();	
	BuildPixelCompareParam(imParam, ImageScale, tpcParam);

	for ( i=0; i<PatternCount; i++ )
	{
		PatParamPtr = CAlgParam::GetAlgPatternParamPtr(i, true);
		if ( NULL == PatParamPtr ) 
		{	continue;	}
		if ( CAlgParam::LoadAlgPatternImage(i, WndToward, PatImgW, PatImgH, PatImgStep, PatBitCount, PatImgPtr) == false ) 
		{	continue;	}

		JetAPI::SizeToRect(PatImgW, PatImgH, PatRect);		
		CAlgBinaryParam  PatBinParam=PatParamPtr->GetBinaryParam();
		const double     OffsetValue=0.0;		
		const double     GainValue = PatBinParam.GetGrayGainValue();
		const bool       GainEnabled=PatBinParam.CheckGrayGainEnabed();
		BINARY_MODE      PatBinaryMode=PatBinParam.GetBinaryMode();				
		IMAGE_SRC_MODE   PatImageSrcMode=PatBinParam.GetBinaryImageSourceMode();
		if ( ExecAlgImageBinary(PatBinParam, RawImageW, RawImageH, RawImageStep, RawBitCount, RawImagePtr, RawMaskPtr, RawSpacePtr, nRoiAlign, RoiGrayPtr, RoiMaskPtr) == false )
		{	continue;	}
		if ( ExecAlgImageBinary(PatBinParam, PatImgW, PatImgH, PatImgStep, PatBitCount, PatImgPtr, NULL, NULL, nPatAlign, PatGrayPtr, PatMaskPtr) == false )
		{					
			JetMemory.free_func(PatImgPtr);
			JetMemory.free_func(PatGrayPtr);
			JetMemory.free_func(PatMaskPtr);
			JetMemory.free_func(RoiGrayPtr);
			JetMemory.free_func(RoiMaskPtr);
			continue;	
		}

		if ( BINARY_DISABLE != PatBinaryMode )//惠璶ゑ耕琌ㄏノ2て紇钩
		{
			RoiBitCount = 8;
			RoiImageW = RawImageW;
			RoiImageH = RawImageH;
			RoiImageStep = JetAPI::GetBMPImagePixelsPerLine(RoiImageW, RoiBitCount, nRoiAlign);
			RoiImgPtrUse = RoiMaskPtr;

			PatBitCountUse = 8;
			PatImgWUse = PatImgW;
			PatImgHUse = PatImgH;
			PatImgStepUse = JetAPI::GetBMPImagePixelsPerLine(PatImgW, PatBitCountUse, nPatAlign);			
			PatImgPtrUse = PatMaskPtr;
		}
		else
		{
			if ( IMAGE_SRC_GRAY==PatImageSrcMode || IMAGE_SRC_COLOR==PatImageSrcMode )
			{	
				RoiBitCount = RawBitCount;
				RoiImageW = RawImageW;
				RoiImageH = RawImageH;
				RoiImageStep = RawImageStep;
				RoiImgPtrUse = RawImagePtr;
				if ( GainEnabled )
				{	
					if ( NULL == RawImagePtr_Clone )
					{	JetMemory.alloc_func(RawImageSize, RawImagePtr_Clone, fnName, "RawImagePtr_Clone");	}
					if ( NULL != RawImagePtr_Clone )
					{	
						if ( ImageAPI.ImageOffsetGain3(RawImageW, RawImageH, RawImageStep, RawBitCount, RawImagePtr, RawImagePtr_Clone, OffsetValue, GainValue) == true )
						{	RoiImgPtrUse = RawImagePtr_Clone;	}	
					}
				}

				PatImgWUse = PatImgW;
				PatImgHUse = PatImgH;
				PatImgStepUse = PatImgStep;
				PatBitCountUse = PatBitCount;
				PatImgPtrUse = PatImgPtr;
				if ( GainEnabled )
				{	
					const size_t PatImgSize=ImageAPI.CalcBufferSize(PatImgStep, PatImgH);
					if ( JetMemory.alloc_func(PatImgSize, PatImgPtr_Clone, fnName, "PatImgPtr_Clone") == true )
					{	
						if ( ImageAPI.ImageOffsetGain3(PatImgW, PatImgH, PatImgStep, PatBitCount, PatImgPtr, PatImgPtr_Clone, OffsetValue, GainValue) == true )
						{	PatImgPtrUse = PatImgPtr_Clone;	}	
					}
				}
			}
			else
			{
				RoiBitCount = 8;
				RoiImageW = RawImageW;
				RoiImageH = RawImageH;
				RoiImageStep = JetAPI::GetBMPImagePixelsPerLine(RoiImageW, RoiBitCount, nRoiAlign);
				RoiImgPtrUse = RoiGrayPtr;

				PatImgWUse = PatImgW;
				PatImgHUse = PatImgH;
				PatBitCountUse = 8;
				PatImgStepUse = JetAPI::GetBMPImagePixelsPerLine(PatImgW, PatBitCountUse, nPatAlign);
				PatImgPtrUse = PatGrayPtr;
			}
		}
	
		if ( PatBitCountUse != RoiBitCount )
		{
			JetMemory.free_func(PatImgPtr);
			JetMemory.free_func(PatGrayPtr);
			JetMemory.free_func(PatMaskPtr);			
			JetMemory.free_func(RoiGrayPtr);
			JetMemory.free_func(RoiMaskPtr);
			JetMemory.free_func(PatImgPtr_Clone);			
			continue; 
		}
	
		//initial eMatch
		Match.SetMatchDefaultParam();
		Match.SetRobustness(bRobustness);
		Match.SetMinReducedArea(nMinReduceArea);
		Match.SetFinalReduction(nFinalReduction);
		if ( Match.LearnPattern(PatImgWUse, PatImgHUse, PatImgStepUse, PatBitCountUse, PatImgPtrUse, true) == false )
		{
			JetMemory.free_func(PatImgPtr);
			JetMemory.free_func(PatGrayPtr);
			JetMemory.free_func(PatMaskPtr);
			JetMemory.free_func(RoiGrayPtr);
			JetMemory.free_func(RoiMaskPtr);
			JetMemory.free_func(PatImgPtr_Clone);
			continue; 
		}		
	#ifdef _DEBUG	
		if ( true == bSave )
		{
			str.Format(_T("%s\\%s_ExecAlgInspection_ImageMatch#%d_Roi.PNG"), DebugFolder, ComponentName, WndIndex+1);
			ImageAPI.SaveImage(str, RoiImageW, RoiImageH, RoiImageStep, RoiBitCount, RoiImgPtrUse, true);	

			str.Format(_T("%s\\%s_ExecAlgInspection_ImageMatch#%d_Pat.PNG"), DebugFolder, ComponentName, WndIndex+1);
			ImageAPI.SaveImage(str, PatImgWUse, PatImgHUse, PatImgStepUse, PatBitCountUse, PatImgPtrUse, true);
		}		
	#endif//_DEBUG
		if ( true == GetAlgSkewEnabled())
		{	
			double dSkew = GetAlgSkewCalcRange();
			float fSkewMax =  (float)(dSkew);
			float fSkewMin = -(float)(dSkew);
			Match.SetMinAngle(fSkewMin);
			Match.SetMaxAngle(fSkewMax);
			Match.SetUseAngle(true);				
		}
		if ( true == GetAlgScaleEnabled() )
		{
			double MatchScaleMin = 1.0, MatchScaleMax = 1.0;
			GetAlgScaleCalcRange(MatchScaleMin, MatchScaleMax);
			Match.SetMinScale(MatchScaleMin);			
			Match.SetMaxScale(MatchScaleMax);
			if ( false == GetAlgPatternScaleIsotropic() )
			{
				Match.SetMinScaleX(MatchScaleMin);
				Match.SetMinScaleY(MatchScaleMin);
				Match.SetMaxScaleX(MatchScaleMax);
				Match.SetMaxScaleY(MatchScaleMax);
			}			
			Match.SetUseScale(true);	
		}

		Match.SetInterpolate(UseInterpolate);
		//Match.SetInterpolate(false);
		Match.SetMinScore(-1);//fMinScore
		if ( Match.Match(RoiImageW, RoiImageH, RoiImageStep, RoiBitCount, RoiImgPtrUse, true) == false )
		{
			JetMemory.free_func(PatImgPtr);
			JetMemory.free_func(PatGrayPtr);
			JetMemory.free_func(PatMaskPtr);
			JetMemory.free_func(RoiGrayPtr);
			JetMemory.free_func(RoiMaskPtr);
			JetMemory.free_func(PatImgPtr_Clone);
			continue;	
		}

		NResults = Match.GetNumPositions();
		if ( NResults > 0 )
		{
			idx = 0;
			ResultS = Match.GetResultScore(idx)*100.0;
			ResultX = Match.GetResultPosX(idx);
			ResultY = Match.GetResultPosY(idx);
			ResultA = Match.GetResultAngle(idx);		
			ResultSX = Match.GetResultScaleX(idx);
			ResultSY = Match.GetResultScaleY(idx);		
			if ( ResultS < 0.0 ) { ResultS = 0.0; }

			ResultCX = ResultX+nRoiX;
			ResultCY = ResultY+nRoiY;
			RoiOffsetX = ResultCX-dRoiCpx;
			RoiOffsetY = dRoiCpy-ResultCY;
			CadSkew = -ResultA;
			CadOffsetX = RoiOffsetX/ImageScale.x;
			CadOffsetY = RoiOffsetY/ImageScale.y;
			CadOffsetX = CadOffsetX + WndRectCalValue.x;
			CadOffsetY = CadOffsetY + WndRectCalValue.y;
			CadScaleX = ResultSX*100.0;
			CadScaleY = ResultSY*100.0;		

			PolarityIdx = 0;
			PatParamPtr->SetPatResultW(PolarityIdx, PatImgWUse);
			PatParamPtr->SetPatResultH(PolarityIdx, PatImgHUse);
			PatParamPtr->SetPatResultX(PolarityIdx, ResultX);
			PatParamPtr->SetPatResultY(PolarityIdx, ResultY);
			PatParamPtr->SetPatResultSkew(PolarityIdx, ResultA);
			PatParamPtr->SetPatResultScore(PolarityIdx, ResultS);
			PatParamPtr->SetPatResultScaleX(PolarityIdx, ResultSX);
			PatParamPtr->SetPatResultScaleY(PolarityIdx, ResultSY);			

			PatParamPtr->SetResultX(PolarityIdx, CadOffsetX);
			PatParamPtr->SetResultY(PolarityIdx, CadOffsetY);
			PatParamPtr->SetResultSkew(PolarityIdx, CadSkew);			
			PatParamPtr->SetResultScaleX(PolarityIdx, ResultSX);
			PatParamPtr->SetResultScaleY(PolarityIdx, ResultSY);			
			PatParamPtr->SetReultID(PolarityIdx, RESULT_ID_NG);
			PatParamPtr->SetResultReading(PolarityIdx, ResultS);
			PatParamPtr->CheckResultIndex();

			if ( ResultS > BestResS )
			{
				BestResS = ResultS;
				BestResX = ResultX;
				BestResY = ResultY;
				BestResA = ResultA;
				BestResSX = ResultSX;
				BestResSY = ResultSY;
				BestPatternIndex = (int)(i);				
			}
			
			if ( ResultS>=LSL && ResultS<=USL )
			{
				PatParamPtr->SetReultID(PolarityIdx, RESULT_ID_OK);
				PatParamPtr->CheckResultIndex();
				if ( false == bPxlCmpEnabed )
				{		
					JetMemory.free_func(PatImgPtr);
					JetMemory.free_func(PatGrayPtr);
					JetMemory.free_func(PatMaskPtr);
					JetMemory.free_func(RoiGrayPtr);
					JetMemory.free_func(RoiMaskPtr);
					JetMemory.free_func(PatImgPtr_Clone);
					if ( false == bPatternTestAll ) 
					{	break;	}
					else
					{	continue;	}
				}
				else
				{					
					RECT SubRect={0,0,0,0};
					IMAGE_PTR  SubImgPtr = NULL;
					IMAGE_SIZE SubImgW=PatImgWUse;
					IMAGE_SIZE SubImgH=PatImgHUse;
					IMAGE_SIZE SubImgStep=PatImgStepUse;					
					//SubRect.left = JetAPI::Floor(ResultX)-(SubImgW/2);
					//SubRect.top  = JetAPI::Floor(ResultY)-(SubImgH/2);
					SubRect.left = (int)(ResultX-(SubImgW*0.5)+0.5);
					SubRect.top  = (int)(ResultY-(SubImgH*0.5)+0.5);
					SubRect.right = SubRect.left+SubImgW;
					SubRect.bottom = SubRect.top+SubImgH;
					SubRectPosX = SubRect.left;
					SubRectPosY = SubRect.top;
					if ( ImageAPI.ExtractRoiImage(RoiImageW, RoiImageH, RoiImageStep, RoiBitCount, RoiImgPtrUse, SubRect, SubImgStep, SubImgPtr, false) == false )
					{
						JetMemory.free_func(PatImgPtr);
						JetMemory.free_func(PatGrayPtr);
						JetMemory.free_func(PatMaskPtr);
						JetMemory.free_func(RoiGrayPtr);
						JetMemory.free_func(RoiMaskPtr);
						JetMemory.free_func(PatImgPtr_Clone);
						JetMemory.free_func(RawImagePtr_Clone);
						JetAPI::ClearUniFrame(RawUniFrame);
						return false;
					}			

				#ifdef _DEBUG
					if ( true == bSave )
					{								
						str.Format(_T("%s\\%s_ModelWndAlgImaegMatchPixelCompare#%d_PatT#%d.PNG"), DebugFolder, ComponentName, WndIndex+1, i+1);
						ImageAPI.SaveImage(str, PatImgWUse, PatImgHUse, PatImgStepUse, PatBitCountUse, PatImgPtrUse, true);				
						str.Format(_T("%s\\%s_ModelWndAlgImaegMatchPixelCompare#%d_ImgT#%d.PNG"), DebugFolder, ComponentName, WndIndex+1, i+1);
						ImageAPI.SaveImage(str, SubImgW, SubImgH, SubImgStep, RoiBitCount, SubImgPtr, true);
					}
				#endif//_DEBUG
					SubRectList.clear();
					if ( ImageAPI.Compare2ImagePixel(PatImgWUse, PatImgHUse, PatImgStepUse, PatBitCountUse, PatImgPtrUse, SubImgPtr, NULL, tpcParam, SubRectList) == false )
					{
						JetMemory.free_func(SubImgPtr);
						JetMemory.free_func(PatImgPtr);
						JetMemory.free_func(PatGrayPtr);
						JetMemory.free_func(PatMaskPtr);
						JetMemory.free_func(RoiGrayPtr);
						JetMemory.free_func(RoiMaskPtr);
						JetMemory.free_func(PatImgPtr_Clone);
						JetMemory.free_func(RawImagePtr_Clone);
						JetAPI::ClearUniFrame(RawUniFrame);
						return false;
					}
					JetMemory.free_func(SubImgPtr);
					const int TmpResultCnt = (int)(SubRectList.size());					
					if ( TmpResultCnt>=nPxlCmpCountLSL && TmpResultCnt<=nPxlCmpCountUSL )
					{
						JetMemory.free_func(PatImgPtr);
						JetMemory.free_func(PatGrayPtr);
						JetMemory.free_func(PatMaskPtr);
						JetMemory.free_func(RoiGrayPtr);
						JetMemory.free_func(RoiMaskPtr);
						JetMemory.free_func(PatImgPtr_Clone);
						if ( false == bPatternTestAll ) 
						{	break;	}
						else
						{	continue; }
					}
				}		
			}				
		}

		if ( 2 == PatternPolarity )
		{
			IMAGE_PTR  PatImgPtr2 = NULL;
			IMAGE_SIZE PatImgW2, PatImgH2, PatImgStep2;
			if ( ImageAPI.RotateImage(180.0, PatImgWUse, PatImgHUse, PatImgStepUse, PatBitCountUse, PatImgPtrUse, PatImgW2, PatImgH2, PatImgStep2, PatImgPtr2) == false )
			{	
				JetMemory.free_func(PatImgPtr);
				JetMemory.free_func(PatGrayPtr);
				JetMemory.free_func(PatMaskPtr);
				JetMemory.free_func(RoiGrayPtr);
				JetMemory.free_func(RoiMaskPtr);
				JetMemory.free_func(PatImgPtr_Clone);
				continue;
			}			
		#ifdef _DEBUG	
			if ( true == bSave )
			{
				str.Format(_T("%s\\%s_ExecAlgInspection_ImageMatch#%d_PatB.PNG"), DebugFolder, ComponentName, WndIndex+1);
				ImageAPI.SaveImage(str, PatImgW2, PatImgH2, PatImgStep2, PatBitCountUse, PatImgPtr2, true);
			}		
		#endif//_DEBUG
			Match.SetMatchDefaultParam();
			if ( Match.LearnPattern(PatImgW2, PatImgH2, PatImgStep2, PatBitCountUse, PatImgPtr2, true) == false )
			{
				JetMemory.free_func(PatImgPtr);
				JetMemory.free_func(PatImgPtr2);
				JetMemory.free_func(PatGrayPtr);
				JetMemory.free_func(PatMaskPtr);
				JetMemory.free_func(RoiGrayPtr);
				JetMemory.free_func(RoiMaskPtr);
				JetMemory.free_func(PatImgPtr_Clone);
				continue; 
			}		

			if ( true == GetAlgSkewEnabled() )
			{	
				double dSkew = GetAlgSkewCalcRange();
				float fSkewMax =  (float)(dSkew);
				float fSkewMin = -(float)(dSkew);
				Match.SetMinAngle(fSkewMin);
				Match.SetMaxAngle(fSkewMax);
				Match.SetUseAngle(true);				
			}
			if ( true == GetAlgScaleEnabled() )
			{	
				double MatchScaleMin = 1.0, MatchScaleMax = 1.0;
				GetAlgScaleCalcRange(MatchScaleMin, MatchScaleMax);
				Match.SetMinScale(MatchScaleMin);			
				Match.SetMaxScale(MatchScaleMax);			
				if ( false == GetAlgPatternScaleIsotropic() )
				{
					Match.SetMinScaleX(MatchScaleMin);
					Match.SetMinScaleY(MatchScaleMin);
					Match.SetMaxScaleX(MatchScaleMax);
					Match.SetMaxScaleY(MatchScaleMax);
				}			
				Match.SetUseScale(true);				
			}
			Match.SetInterpolate(UseInterpolate);
			Match.SetMinScore(-1);//fMinScore
			if ( Match.Match(RoiImageW, RoiImageH, RoiImageStep, RoiBitCount, RoiImgPtrUse, true) == false )
			{
				JetMemory.free_func(PatImgPtr);
				JetMemory.free_func(PatImgPtr2);
				JetMemory.free_func(PatGrayPtr);
				JetMemory.free_func(PatMaskPtr);
				JetMemory.free_func(RoiGrayPtr);
				JetMemory.free_func(RoiMaskPtr);
				JetMemory.free_func(PatImgPtr_Clone);
				continue;	
			}

			NResults = Match.GetNumPositions();
			if ( NResults > 0 )
			{
				idx = 0;
				ResultS = Match.GetResultScore(idx)*100.0;
				ResultX = Match.GetResultPosX(idx);
				ResultY = Match.GetResultPosY(idx);
				ResultA = Match.GetResultAngle(idx);		
				ResultSX = Match.GetResultScaleX(idx);
				ResultSY = Match.GetResultScaleY(idx);		
				if ( ResultS < 0.0 ) { ResultS = 0.0; }

				ResultCX = ResultX+nRoiX;
				ResultCY = ResultY+nRoiY;
				RoiOffsetX = ResultCX-dRoiCpx;
				RoiOffsetY = dRoiCpy-ResultCY;
				CadSkew = -ResultA;
				CadOffsetX = RoiOffsetX/ImageScale.x;
				CadOffsetY = RoiOffsetY/ImageScale.y;
				CadOffsetX = CadOffsetX + WndRectCalValue.x;
				CadOffsetY = CadOffsetY + WndRectCalValue.y;

				PolarityIdx = 1;
				PatParamPtr->SetPatResultW(PolarityIdx, PatImgWUse);
				PatParamPtr->SetPatResultH(PolarityIdx, PatImgHUse);
				PatParamPtr->SetPatResultX(PolarityIdx, ResultX);
				PatParamPtr->SetPatResultY(PolarityIdx, ResultY);
				PatParamPtr->SetPatResultSkew(PolarityIdx, ResultA);
				PatParamPtr->SetPatResultScore(PolarityIdx, ResultS);
				PatParamPtr->SetPatResultScaleX(PolarityIdx, ResultSX);
				PatParamPtr->SetPatResultScaleY(PolarityIdx, ResultSY);

				PatParamPtr->SetResultX(PolarityIdx, CadOffsetX);
				PatParamPtr->SetResultY(PolarityIdx, CadOffsetY);
				PatParamPtr->SetResultSkew(PolarityIdx, CadSkew);				
				PatParamPtr->SetResultScaleX(PolarityIdx, ResultSX);
				PatParamPtr->SetResultScaleY(PolarityIdx, ResultSY);				
				PatParamPtr->SetReultID(PolarityIdx, RESULT_ID_NG);
				PatParamPtr->SetResultReading(PolarityIdx, ResultS);
				PatParamPtr->CheckResultIndex();

				if ( ResultS > BestResS )
				{
					BestResS = ResultS;
					BestResX = ResultX;
					BestResY = ResultY;
					BestResA = ResultA;
					BestResSX = ResultSX;
					BestResSY = ResultSY;
					BestPatternIndex = (int)(i);					
				}
				if ( ResultS>=LSL && ResultS<=USL )
				{
					PatParamPtr->SetReultID(PolarityIdx, RESULT_ID_OK);
					PatParamPtr->CheckResultIndex();
					if ( false == bPxlCmpEnabed )
					{
						JetMemory.free_func(PatImgPtr2);
						JetMemory.free_func(PatImgPtr);
						JetMemory.free_func(PatGrayPtr);
						JetMemory.free_func(PatMaskPtr);
						JetMemory.free_func(RoiGrayPtr);
						JetMemory.free_func(RoiMaskPtr);
						JetMemory.free_func(PatImgPtr_Clone);
						if ( false == bPatternTestAll ) 
						{	break;	}
						else
						{	continue; }
					}
					else
					{						
						RECT SubRect={0,0,0,0};
						IMAGE_PTR  SubImgPtr = NULL;
						IMAGE_SIZE SubImgW=PatImgW2;
						IMAGE_SIZE SubImgH=PatImgH2;
						IMAGE_SIZE SubImgStep=PatImgStep2;
						//SubRect.left = JetAPI::Floor(ResultX)-(SubImgW/2);
						//SubRect.top  = JetAPI::Floor(ResultY)-(SubImgH/2);
						SubRect.left = (int)(ResultX-(SubImgW*0.5)+0.5);
						SubRect.top  = (int)(ResultY-(SubImgH*0.5)+0.5);
						SubRect.right = SubRect.left+SubImgW;
						SubRect.bottom = SubRect.top+SubImgH;
						SubRectPosX = SubRect.left;
						SubRectPosY = SubRect.top;
						if ( ImageAPI.ExtractRoiImage(RoiImageW, RoiImageH, RoiImageStep, RoiBitCount, RoiImgPtrUse, SubRect, SubImgStep, SubImgPtr, false) == false )
						{
							JetMemory.free_func(PatImgPtr2);
							JetMemory.free_func(PatImgPtr);
							JetMemory.free_func(PatGrayPtr);
							JetMemory.free_func(PatMaskPtr);
							JetMemory.free_func(RoiGrayPtr);
							JetMemory.free_func(RoiMaskPtr);
							JetMemory.free_func(PatImgPtr_Clone);
							JetMemory.free_func(RawImagePtr_Clone);
							JetAPI::ClearUniFrame(RawUniFrame);
							return false;
						}			
					#ifdef _DEBUG
						if ( true == bSave )
						{								
							str.Format(_T("%s\\%s_ModelWndAlgImaegMatchPixelCompare#%d_PatB#%d.PNG"), DebugFolder, ComponentName, WndIndex+1, i+1);
							ImageAPI.SaveImage(str, PatImgW2, PatImgH2, PatImgStep2, PatBitCountUse, PatImgPtr2, true);				
							str.Format(_T("%s\\%s_ModelWndAlgImaegMatchPixelCompare#%d_ImgB#%d.PNG"), DebugFolder, ComponentName, WndIndex+1, i+1);
							ImageAPI.SaveImage(str, SubImgW, SubImgH, SubImgStep, RoiBitCount, SubImgPtr, true);
						}
					#endif//_DEBUG
						SubRectList.clear();
						if ( ImageAPI.Compare2ImagePixel(PatImgW2, PatImgH2, PatImgStep2, PatBitCountUse, PatImgPtr2, SubImgPtr, NULL, tpcParam, SubRectList) == false )
						{
							JetMemory.free_func(SubImgPtr);
							JetMemory.free_func(PatImgPtr2);
							JetMemory.free_func(PatImgPtr);
							JetMemory.free_func(PatGrayPtr);
							JetMemory.free_func(PatMaskPtr);
							JetMemory.free_func(RoiGrayPtr);
							JetMemory.free_func(RoiMaskPtr);
							JetMemory.free_func(PatImgPtr_Clone);
							JetMemory.free_func(RawImagePtr_Clone);
							JetAPI::ClearUniFrame(RawUniFrame);
							return false;
						}
						JetMemory.free_func(SubImgPtr);
						const int TmpResultCnt = (int)(SubRectList.size());					
						if ( TmpResultCnt>=nPxlCmpCountLSL && TmpResultCnt<=nPxlCmpCountUSL )
						{
							JetMemory.free_func(PatImgPtr2);
							JetMemory.free_func(PatImgPtr);
							JetMemory.free_func(PatGrayPtr);
							JetMemory.free_func(PatMaskPtr);
							JetMemory.free_func(RoiGrayPtr);
							JetMemory.free_func(RoiMaskPtr);
							JetMemory.free_func(PatImgPtr_Clone);							
							if ( false == bPatternTestAll ) 
							{	break;	}
							else
							{	continue; }
						}
					}
				}
			}
			JetMemory.free_func(PatImgPtr2);			
		}
		JetMemory.free_func(PatImgPtr);
		JetMemory.free_func(PatGrayPtr);
		JetMemory.free_func(PatMaskPtr);		

		JetMemory.free_func(RoiGrayPtr);
		JetMemory.free_func(RoiMaskPtr);
		JetMemory.free_func(PatImgPtr_Clone);
	}
	JetMemory.free_func(RawImagePtr_Clone);
	JetAPI::ClearUniFrame(RawUniFrame);
	
	bool  bMatchScore=false;
	PatParamPtr = GetAlgPatternParamPtr(BestPatternIndex, true);
	if ( NULL != PatParamPtr ) 
	{
		PolarityIdx = PatParamPtr->GetResultPolarityIdx();
		ResultS = PatParamPtr->GetResultReading(PolarityIdx);
		ResultX = PatParamPtr->GetPatResultX(PolarityIdx);
		ResultY = PatParamPtr->GetPatResultY(PolarityIdx);
		ResultA = PatParamPtr->GetPatResultSkew(PolarityIdx);
		ResultSX = PatParamPtr->GetPatResultScaleX(PolarityIdx);
		ResultSY = PatParamPtr->GetPatResultScaleY(PolarityIdx);		

		ResultCX = ResultX+nRoiX;
		ResultCY = ResultY+nRoiY;
		RoiOffsetX = ResultCX-dRoiCpx;
		RoiOffsetY = dRoiCpy-ResultCY;
		CadSkew = -ResultA;
		CadOffsetX = RoiOffsetX/ImageScale.x;
		CadOffsetY = RoiOffsetY/ImageScale.y;
		CadOffsetX = CadOffsetX + WndRectCalValue.x;
		CadOffsetY = CadOffsetY + WndRectCalValue.y;
		CadScaleX = ResultSX*100.0;
		CadScaleY = ResultSY*100.0;		
		bMatchScore = true;
	}
	if ( BestPatternIndex<0 && PatternCount>0 ) 
	{	BestPatternIndex = 0; }
	
	Reading = ResultS;
	USL = GetAlgPatternSimilarityUSL();
	LSL = GetAlgPatternSimilarityLSL();
	SetAlgPatternSimilarityReading(Reading);
	SetAlgPatternResultIndex(BestPatternIndex);		

	CString strResult;	
	SetAlgResultReading1(Reading);	
	SetAlgResultText(_T("OK"));
	SetAlgResultID(RESULT_ID_OK);	
	if ( Reading<LSL || Reading>USL )
	{
		strResult.Format(_T("NG:%.0f / %.0f"), Reading, LSL);		
		SetAlgResultID(RESULT_ID_NG);
		SetAlgResultText(strResult);
	}
	RECT SubRect={0};
	const int SubRectCnt = (int)(SubRectList.size());					
	if ( true == bPxlCmpEnabed )
	{
		imParam.imPxlCmpCountNum = SubRectCnt;
		if ( SubRectCnt<nPxlCmpCountLSL || SubRectCnt>nPxlCmpCountUSL )
		{		
			strResult.Format(_T("NG:%d / %d"), SubRectCnt, nPxlCmpCountUSL);			
			SetAlgResultID(RESULT_ID_NG);
			SetAlgResultText(strResult);
		}
		//Result Box	
		CAOIBox   ResBox;
		TPOINT2D  RgnCp;
		TPOINT2D  ImageCp;
		TREGION4D BoxRgn;
		TREGION4D WndRgn;

		ImageCp.x = RoiImageW;
		ImageCp.y = RoiImageH;
		ImageCp.x = ImageCp.x*0.5;
		ImageCp.y = ImageCp.y*0.5;		
		WndPtr->GetWndRegion(WndRgn);		
		RgnCp.x = WndRgn.GetCpX();
		RgnCp.y = WndRgn.GetCpY();		
		for ( i=0; i<SubRectCnt; i++ )
		{
			SubRect = SubRectList[i];
			::InflateRect(&SubRect, 1, 1);
			SubRect.left   += SubRectPosX;
			SubRect.top    += SubRectPosY;
			SubRect.right  += SubRectPosX;
			SubRect.bottom += SubRectPosY;
			CAOIModel::CalcModelBoxRectRegion(SubRect, RoiImageW, RoiImageH, RgnCp, ImageScale, ImageCp, BoxRgn);			
			ResBox.SetBoxRegion(BoxRgn);			
			WndPtr->AddWndResultBox(ResBox);
		}				
	}
	
	double CadSkew_Self = 0;
	double CadOffsetX_Self = 0;
	double CadOffsetY_Self = 0;
	double CadSkew_Others = 0;
	double CadOffsetX_Others = 0;
	double CadOffsetY_Others = 0;	
	const bool bChkDefect=true;
	const bool ApplySkewAngle = true;	
	const bool AlgScaleEnabled = GetAlgScaleEnabled();
	/*
	bool IsExceptionAngle = false;
	if ( NULL != ComponentPtr )
	{
		const double ComponentAngle = ComponentPtr->GetComponentAngle();
		IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComponentAngle);
		if ( true == IsExceptionAngle )
		{
			double AngleRadius = ComponentAngle*DEG_TO_RAD_DBL;
			const double COS = cos(AngleRadius);
			const double SIN = sin(AngleRadius);
			const double dCPX = 0.0;
			const double dCPY = 0.0;			
			CadOffsetX3 = (CadOffsetX*COS)-(CadOffsetY*SIN)+dCPX;
			CadOffsetY3 = (CadOffsetX*SIN)+(CadOffsetY*COS)+dCPY;			
		}
	}	
	*/
	SetAlgSkewReading(CadSkew);	
	SetAlgImageOffsetX(RoiOffsetX);
	SetAlgImageOffsetY(RoiOffsetY);
	SetAlgOffsetXReading(CadOffsetX);
	SetAlgOffsetYReading(CadOffsetY);
	CalcAlgOffsetL();
	CheckAlgOffset(CadOffsetX_Self, CadOffsetY_Self, CadSkew_Self, CadOffsetX_Others, CadOffsetY_Others, CadSkew_Others, bChkDefect);

	if ( true == AlgScaleEnabled )
	{
		ApplyScale = true;		
		USL = GetAlgScaleUSL();
		LSL = GetAlgScaleLSL();
		SetAlgScaleXReading(CadScaleX);
		SetAlgScaleYReading(CadScaleY);		
		if ( CadScaleX<LSL || CadScaleY<LSL || CadScaleX>USL || CadScaleY>USL )
		{
			ApplyScale = false;
			CString Key = AOIDataDefine.GetRatioText();
			strResult.Format(_T("%s:(%.0f, %.0f) (%.0f~%.0f)"), Key, CadScaleX, CadScaleY, LSL, USL);			
			SetAlgResultID(RESULT_ID_NG);
			SetAlgResultText(strResult);
		}
	}

	WndPtr->SetWndResultID(m_AlgResultID);
	WndPtr->SetWndResultText(m_AlgResultText);	
	//SaveAlgDefectImage(MaskW, MaskH, MaskStep, MaskBitCount, GrayPtr, MaskPtr, WndRect);

	bool  ApplySkew = false;
	bool  ApplyOffsetX = false;
	bool  ApplyOffsetY = false;
	if ( fabs(CadSkew_Others) > 0.001 ) { ApplySkew = true; }
	if ( fabs(CadOffsetX_Others) > 0.001 ) { ApplyOffsetX = true; }
	if ( fabs(CadOffsetY_Others) > 0.001 ) { ApplyOffsetY = true; }
	//甅ノ浪代
	if ( true == bMatchScore )
	{
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
		if ( true == AlgScaleEnabled )
		{	BoxPtr->ScaleBoxSizeRes(CadScaleX/100.0, CadScaleY/100.0);	}
		BoxPtr->SkewBoxAngle(CadSkew_Self);
		BoxPtr->MoveBoxRes(CadOffsetX_Self, CadOffsetY_Self);
	}
	return true;
}
//-------------------------------------------------------------------------------------//