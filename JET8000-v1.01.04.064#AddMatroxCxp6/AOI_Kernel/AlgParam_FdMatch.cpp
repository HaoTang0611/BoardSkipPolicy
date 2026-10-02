// AlgParam_FdMatch.cpp: implementation of the CAlgParam class.
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
bool CAlgParam::WriteAlgParamFile_FdMatch(const TALG_PARAM_FD_MATCH &fdParam, CAOIFileIO &FileIO)//纗﹚翴で皌把计
{		
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_FD_MATCH_START, 0) == false ) { return false; }

	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_FD_MATCH_MODE, fdParam.fmMatchMode) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_FD_FILL_SIZE, fdParam.fmFillSize) == false ) { return false; }	

	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_FD_MATCH_END, 0) == false ) { return false; }		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ReadAlgParamFile_FdMatch(TALG_PARAM_FD_MATCH &fdParam, CAOIFileIO &FileIO)//更﹚翴で皌把计
{
	int       index = 0;
	while ( FileIO.CheckFileEnd()==false )
	{ 		
		if ( FileIO.LoadChunk(index) == false )		
		{	continue; }
		switch ( index )
		{
		case FILE_IO_ALG_PARAM_FD_MATCH_END://﹚翴で皌把计-沧翴
			return true;
			break;
		case FILE_IO_ALG_PARAM_FD_MATCH_MODE://﹚翴で皌把计-家Α
			fdParam.fmMatchMode = (FD_MATCH_MODE)(FileIO.GetData_INT());
			if ( 0 == fdParam.fmMatchMode )
			{	fdParam.fmMatchMode = FD_MATCH_IMAGE; }
			break;	
		case FILE_IO_ALG_PARAM_FD_FILL_SIZE://﹚翴で皌把计-恶骸へ
			fdParam.fmFillSize = (FileIO.GetData_INT());			
			break;		
		default:
		#ifdef _DEBUG
			index = index;
		#endif//_DEBUG
			break;
		}
	}
	FileIO.SetErrorString(_T("Error, ReadAlgParamFile_FdMatch Fault"));
	return false;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ExecAlgInspection_FdMatch(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList)
{
	const char fnName[]="CAlgParam::ExecAlgInspection_FdMatch";
	if ( NULL == ModelPtr ) { return false; }
	CAOIFd *FdPtr = ModelPtr->GetModelFdPtr();
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
	const bool      bRobustness = true;
	TALG_PARAM_FD_MATCH &fmParam   =  GetAlgParamFdMatch();		
	const int FdFillSize = fmParam.fmFillSize;	
	const FD_MATCH_MODE FdMatchMode = fmParam.fmMatchMode;	

	bool          IsOK = false;
	IMAGE_SIZE    MaskW=0;
	IMAGE_SIZE    MaskH=0;	
	IMAGE_SIZE    MaskStep=0;
	IMAGE_SIZE    MaskBitCount=8;
	MASK_PTR      MaskPtr  = NULL;
	IMAGE_PTR     GrayPtr = NULL;	
	const bool    bTestWnd = false;
	const TPOINT2D   ImageScale = ModelPtr->GetModelImageScale();	
	IMAGE_SRC_MODE   ImageSourceMode = m_AlgImageBinParam.GetBinaryImageSourceMode();
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

	if ( BINARY_DISABLE == m_AlgImageBinParam.GetBinaryMode() )
	{		
		if ( FD_MATCH_MODEL == FdMatchMode )
		{
			RoiBitCount = 8;
			RoiImageStep = JetAPI::GetBMPImagePixelsPerLine(RoiImageW, RoiBitCount, nRoiAlign);
			IsOK = ImageAPI.ExtractGrayRoiImage(MaskW, MaskH, MaskStep, GrayPtr, RoiRect, RoiImageStep, RoiImagePtr, false);
		}
		else
		{
			if ( IMAGE_SRC_COLOR==ImageSourceMode || IMAGE_SRC_GRAY==ImageSourceMode )
			{
				RoiBitCount = FrameBitCount;
				RoiImageStep = JetAPI::GetBMPImagePixelsPerLine(RoiImageW, RoiBitCount, nRoiAlign);
				IsOK = ImageAPI.ExtractRoiImage(MaskW, MaskH, FrameImageStep, FrameBitCount, FrameImagePtr, RoiRect, RoiImageStep, RoiImagePtr, false);
			}
			else
			{
				RoiBitCount = 8;
				RoiImageStep = JetAPI::GetBMPImagePixelsPerLine(RoiImageW, RoiBitCount, nRoiAlign);
				IsOK = ImageAPI.ExtractGrayRoiImage(MaskW, MaskH, MaskStep, GrayPtr, RoiRect, RoiImageStep, RoiImagePtr, false);
			}
		}
		if ( FdFillSize > 0 )
		{
			IMAGE_PTR TmpPtr=NULL;
			const size_t TmpSize=ImageAPI.CalcBufferSize(RoiImageStep, RoiImageH);
			if ( JetMemory.alloc_func(TmpSize, TmpPtr, fnName, "TmpPtr") == true )
			{	
				const int MorphMode=MORPH_CLOSE;//MORPH_OPEN,MORPH_CLOSE
				const int MorphShapeMode=MORPH_SHAPE_RECT;
				const int KernSize=FdFillSize;
				const int InterSize=1;
				if ( ImageAPI.MorphImage3(RoiImageW, RoiImageH, RoiImageStep, RoiBitCount, RoiImagePtr, MorphMode, MorphShapeMode, KernSize, InterSize, TmpPtr) == true )
				{	::memcpy(RoiImagePtr, TmpPtr, sizeof(IMAGE_DATA)*TmpSize);	}
				JetMemory.free_func(TmpPtr);
			}
		}
	}
	else
	{	
		RoiBitCount = 8;
		RoiImageStep = JetAPI::GetBMPImagePixelsPerLine(RoiImageW, RoiBitCount, nRoiAlign);
		IsOK = ImageAPI.ExtractGrayRoiImage(MaskW, MaskH, MaskStep, MaskPtr, RoiRect, RoiImageStep, RoiImagePtr, false); 
	}
	JetMemory.free_func(MaskPtr);
	JetMemory.free_func(GrayPtr);
	if ( false == IsOK )
	{	return false;	}	

	size_t          i=0;	
	RECT            PatRect={0,0,0,0};
	CPatternParam  *PatParamPtr = NULL;
	IMAGE_PTR       PatImgPtr=NULL;
	IMAGE_PTR       PatMaskPtr=NULL;
	IMAGE_PTR       PatImgPtrUse=NULL;
	IMAGE_SIZE      PatImgW=0, PatImgH=0, PatImgStep=0, PatBitCount=0;
	IMAGE_SIZE      PatImgWUse=0, PatImgHUse=0, PatImgStepUse=0, PatBitCountUse=0;
	const int       nPatAlign = 4;
	std::vector<TREGION4D> FeatureList;
	const size_t    PatternCount = CAlgParam::GetAlgPatternCount();
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
	const TPOINT2D &WndRectCalValue = WndPtr->GetWndRectCalValue();
	//const bool bTestWndRectCalValueFd=false;//25250829	
	//WndRectCalValue.x = 0;//-WndRectCalValue.x;
	//WndRectCalValue.y = 0;//WndRectCalValue.y;

	int    idx = 0;
	int    NResults = 0;	
	int    BestPatternIndex=-1;		
	double ResultCX=0.0, ResultCY=0.0;
	double RoiOffsetX=0.0, RoiOffsetY=0.0;	
	double CadOffsetX=0.0, CadOffsetY=0.0, CadSkew=0.0, CadScaleX=100.0, CadScaleY=100.0;	
	double ResultX=0, ResultY=0, ResultA=0, ResultS=0, ResultSX=0, ResultSY=0;	
	double BestResX=0, BestResY=0, BestResA=0, BestResS=0, BestResSX=0, BestResSY=0;		
	double Reading=0.0, USL=0.0, LSL=0.0;		
	bool   ApplyScale=false;	
	const int       nMinReduceArea = GetAlgPatternMinReducedArea();	
	const int       nFinalReduction = GetAlgPatternFinalReduction();
	const bool      bAdvancedLearning = GetAlgPatternAdvancedLearning();
	const bool      UseInterpolate = CheckAlgPatternMatrchInterpolate();

	CJetMatch    Match;	//紇钩で皌
	BINARY_MODE  BinaryMode;

#ifdef _DEBUG	
	CString      str;
	CString      FdName;
	CString      DebugFolder=GetAlgDebugFolder();
	bool         bSave = true;
	if ( NULL != FdPtr )
	{	FdName.Format(_T("Fd_%d"), FdPtr->GetFdUniqueID()+1);}	
	//str.Format(_T("%s\\%s_ModelWndAlgFdMatch#%d_Pattern.PNG"), DebugFolder, FdName, WndIndex+1);
#endif//_DEBUG
	if ( Match.SetMatchLibType(MatchLibType)==false )
	{	
		JetMemory.free_func(RoiImagePtr);
		return false;	
	}

	USL = GetAlgPatternSimilarityUSL();
	LSL = GetAlgPatternSimilarityLSL();	
	if ( FD_MATCH_MODEL == FdMatchMode )
	{		
		PatBitCount = RoiBitCount;
		if ( WndPtr->BuildWndBoxImage(ModelPtr, ImageScale, PatBitCount, PatImgW, PatImgH, PatImgStep, PatImgPtr, FeatureList) == false )
		{
			JetMemory.free_func(RoiImagePtr);
			return false; 
		}
		const size_t FeatureCount = FeatureList.size();
		//initial eMatch
		Match.SetMatchDefaultParam();		
		Match.SetMinReducedArea(nMinReduceArea);
		Match.SetFinalReduction(nFinalReduction);
		if ( Match.LearnPattern(PatImgW, PatImgH, PatImgStep, PatBitCount, PatImgPtr, true) == false )
		{
			JetMemory.free_func(PatImgPtr);
			JetMemory.free_func(RoiImagePtr);
			return false;
		}
		
		Match.SetInterpolate(UseInterpolate);
		//Match.SetInterpolate(false);
		Match.SetMinScore(-1);//fMinScore
		if ( Match.Match(nRoiImageW, nRoiImageH, nRoiImageStep, nRoiBitCount, RoiImagePtr, true) == false )
		{
			JetMemory.free_func(PatImgPtr);
			JetMemory.free_func(RoiImagePtr);
			return false;
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
			if ( ResultS > BestResS )
			{
				BestResS = ResultS;
				BestResX = ResultX;
				BestResY = ResultY;
				BestResA = ResultA;
				BestResSX = ResultSX;
				BestResSY = ResultSY;
				//BestPatternIndex = (int)(i);				
			}			
		}		
		JetMemory.free_func(PatImgPtr);
		JetMemory.free_func(PatMaskPtr);		
	}
	else if ( FD_MATCH_IMAGE == FdMatchMode )
	{
		for ( i=0; i<PatternCount; i++ )
		{
			PatParamPtr = CAlgParam::GetAlgPatternParamPtr(i, true);
			if ( NULL == PatParamPtr ) 
			{	continue;	}
			if ( CAlgParam::LoadAlgPatternImage(i, WndToward, PatImgW, PatImgH, PatImgStep, PatBitCount, PatImgPtr) == false ) 
			{	continue;	}

			BinaryMode = m_AlgImageBinParam.GetBinaryMode();
			ImageSourceMode = m_AlgImageBinParam.GetBinaryImageSourceMode();
			if ( BINARY_DISABLE != BinaryMode )//惠璶ゑ耕琌ㄏノ2て紇钩
			{
				PatRect.left   = 0;
				PatRect.top    = 0;
				PatRect.right  = PatImgW;
				PatRect.bottom = PatImgH;
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
						PatParamPtr->GetBinaryParam().GetBinaryImageSourceMode(), 
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
				if ( IMAGE_SRC_COLOR==ImageSourceMode || IMAGE_SRC_GRAY==ImageSourceMode )
				{
					PatImgWUse = PatImgW;
					PatImgHUse = PatImgH;
					PatImgStepUse = PatImgStep;
					PatBitCountUse = PatBitCount;
					PatImgPtrUse = PatImgPtr;
				}
				else
				{
					PatRect.left   = 0;
					PatRect.top    = 0;
					PatRect.right  = PatImgW;
					PatRect.bottom = PatImgH;
					PatImgWUse = PatImgW;
					PatImgHUse = PatImgH;
					PatBitCountUse = 8;
					PatImgStepUse = JetAPI::GetBMPImagePixelsPerLine(PatImgW, MaskBitCount, nPatAlign);
					if ( ImageAPI.ColorImageToGrayImage(PatImgW, PatImgH, PatImgStep, PatImgPtr, PatRect, PatImgStepUse, PatMaskPtr, 
						m_AlgImageBinParam.GetBinaryImageSourceMode(), //PatParamPtr->GetBinaryParam().GetBinaryImageSourceMode(), 
						m_AlgImageBinParam.GetBinarySynthesisWR(), //PatParamPtr->GetBinaryParam().GetBinarySynthesisWR(), 
						m_AlgImageBinParam.GetBinarySynthesisWG(), //PatParamPtr->GetBinaryParam().GetBinarySynthesisWG(), 
						m_AlgImageBinParam.GetBinarySynthesisWB(), //PatParamPtr->GetBinaryParam().GetBinarySynthesisWB(), 
						false) == false )
					{
						JetMemory.free_func(PatImgPtr);
						JetMemory.free_func(PatMaskPtr);
						continue;
					}
					PatImgPtrUse = PatMaskPtr;
				}
			}		
			if ( PatBitCountUse != RoiBitCount )
			{
				JetMemory.free_func(PatImgPtr);
				JetMemory.free_func(PatMaskPtr);
				continue; 
			}

		#ifdef _DEBUG
			if ( true == bSave )
			{
				str.Format(_T("%s\\%s_ModelWndAlgFdMatch#%d_Pattern.PNG"), DebugFolder, FdName, WndIndex+1);
				ImageAPI.SaveImage(str, PatImgWUse, PatImgHUse, PatImgStepUse, PatBitCountUse, PatImgPtrUse, true);
				str.Format(_T("%s\\%s_ModelWndAlgFdMatch#%d_Image.PNG"), DebugFolder, FdName, WndIndex+1);
				ImageAPI.SaveImage(str, nRoiImageW, nRoiImageH, nRoiImageStep, nRoiBitCount, RoiImagePtr, true);
			}
		#endif//_DEBUG
	
			//initial eMatch
			Match.SetMatchDefaultParam();
			Match.SetMinReducedArea(nMinReduceArea);
			Match.SetFinalReduction(nFinalReduction);
			if ( Match.LearnPattern(PatImgWUse, PatImgHUse, PatImgStepUse, PatBitCountUse, PatImgPtrUse, true) == false )
			{
				JetMemory.free_func(PatImgPtr);
				JetMemory.free_func(PatMaskPtr);
				continue; 
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

				const int PatResIdx = 0;
				PatParamPtr->SetPatResultX(PatResIdx, ResultX);
				PatParamPtr->SetPatResultY(PatResIdx, ResultY);
				PatParamPtr->SetPatResultSkew(PatResIdx, CadSkew);
				PatParamPtr->SetPatResultScore(PatResIdx, ResultS);
				PatParamPtr->SetPatResultScaleX(PatResIdx, ResultSX);
				PatParamPtr->SetPatResultScaleY(PatResIdx, ResultSY);

				PatParamPtr->SetResultX(PatResIdx, CadOffsetX);
				PatParamPtr->SetResultY(PatResIdx, CadOffsetY);
				PatParamPtr->SetResultSkew(PatResIdx, CadSkew);
				PatParamPtr->SetResultScaleX(PatResIdx, ResultSX);
				PatParamPtr->SetResultScaleY(PatResIdx, ResultSY);
				PatParamPtr->SetReultID(PatResIdx, RESULT_ID_OK);
				PatParamPtr->SetResultReading(PatResIdx, ResultS);
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
					JetMemory.free_func(PatImgPtr);
					JetMemory.free_func(PatMaskPtr);
					break; 
				}
			}		
			JetMemory.free_func(PatImgPtr);
			JetMemory.free_func(PatMaskPtr);
		}
	}
	JetMemory.free_func(RoiImagePtr);

	bool bMatchScore = false;
	if ( BestPatternIndex>=0 || FD_MATCH_MODEL==FdMatchMode ) 
	{
		ResultS = BestResS;
		ResultX = BestResX;
		ResultY = BestResY;
		ResultA = BestResA;
		ResultSX = BestResSX;
		ResultSY = BestResSY;
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
		CadOffsetX = 0;//ぃì, 玥ㄏノ﹍畒夹
		CadOffsetY = 0;//ぃì, 玥ㄏノ﹍畒夹
		strResult.Format(_T("NG:%.0f / %.0f"), Reading, LSL);		
		SetAlgResultID(RESULT_ID_NG);
		SetAlgResultText(strResult);		
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
	if ( NULL != FdPtr )
	{
		const double FdAngle = FdPtr->GetFdAngle();
		IsExceptionAngle = JetAPI::CheckIsExceptionAngle(FdAngle);
		if ( true == IsExceptionAngle )
		{
			double AngleRadius = FdAngle*DEG_TO_RAD_DBL;
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
			if ( ModelPtr->UpdateModelInspectionPosRes(WndPtr, CadOffsetX_Others, CadOffsetY_Others, CadSkew_Others, ApplySkewAngle) == false )
			{	return false; }
		}
		CAOIBox *BoxPtr = WndPtr->GetWndBoxPtr();
		BoxPtr->SkewBoxAngle(CadSkew_Self);
		BoxPtr->MoveBoxRes(CadOffsetX_Self, CadOffsetY_Self);
	}
	return true;
}
//-------------------------------------------------------------------------------------//