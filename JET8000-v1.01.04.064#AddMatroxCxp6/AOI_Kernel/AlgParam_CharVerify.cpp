// AlgParam_CharVerify.cpp: implementation of the CAlgParam class.
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
bool CAlgParam::CheckOK_CharVerify(const TALG_PARAM_CHAR_VERIFY &Param)
{
	if ( CheckOK_CharVerifyUSL(Param) == false )
	{	return false; }
	if ( CheckOK_CharVerifyLSL(Param) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_CharVerifyUSL(const TALG_PARAM_CHAR_VERIFY &Param)
{
	if ( Param.cvPassRatioReading > Param.cvPassRatioUSL )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_CharVerifyLSL(const TALG_PARAM_CHAR_VERIFY &Param)
{
	if ( Param.cvPassRatioReading < Param.cvPassRatioLSL )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::WriteAlgParamFile_CharVerify(const TALG_PARAM_CHAR_VERIFY &cvParam, CAOIFileIO &FileIO)//儲存字元驗證參數
{	
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_CHAR_VERIFY_START, 0) == false ) { return false; }

	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_CHAR_VERIFY_CELL_SCORE_MAX, cvParam.cvCellScoreMax) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_CHAR_VERIFY_CELL_SCORE_MIN, cvParam.cvCellScoreMin) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_CHAR_VERIFY_PASS_RATIO_USL, cvParam.cvPassRatioUSL) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_CHAR_VERIFY_PASS_RATIO_LSL, cvParam.cvPassRatioLSL) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_CHAR_VERIFY_CELL_GRID_CNT, cvParam.cvCellGridCnt) == false ) { return false; }		
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_CHAR_VERIFY_CELL_EXT_SIZE, cvParam.cvCellExtSize) == false ) { return false; }

	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_CHAR_VERIFY_END, 0) == false ) { return false; }	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ReadAlgParamFile_CharVerify(TALG_PARAM_CHAR_VERIFY &cvParam, CAOIFileIO &FileIO)//載入字元驗證參數
{
	int       index = 0;
	int       nValue = 0;	
	while ( FileIO.CheckFileEnd()==false )
	{ 		
		if ( FileIO.LoadChunk(index) == false )		
		{	continue; }
		switch ( index )
		{
		case FILE_IO_ALG_PARAM_CHAR_VERIFY_END://文字驗證參數-終點
			return true;
			break;
		case FILE_IO_ALG_PARAM_CHAR_VERIFY_CELL_SCORE_MAX://單元相似度上限
			cvParam.cvCellScoreMax = FileIO.GetData_DBL();
			break;		
		case FILE_IO_ALG_PARAM_CHAR_VERIFY_CELL_SCORE_MIN://單元相似度下限
			cvParam.cvCellScoreMin = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_CHAR_VERIFY_PASS_RATIO_USL://通過比例上限
			cvParam.cvPassRatioUSL = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_CHAR_VERIFY_PASS_RATIO_LSL://通過比例下限
			cvParam.cvPassRatioLSL = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_CHAR_VERIFY_CELL_GRID_CNT://單元細分數量
			nValue = FileIO.GetData_INT();
			if ( nValue > 0 ) 
			{	cvParam.cvCellGridCnt = nValue; }
			break;
		case FILE_IO_ALG_PARAM_CHAR_VERIFY_CELL_EXT_SIZE://單元外擴尺寸
			nValue = FileIO.GetData_INT();
			if ( nValue > 0 ) 
			{	cvParam.cvCellExtSize = nValue; }
			break;
		default:
		#ifdef _DEBUG
			index = index;
		#endif//_DEBUG
			break;
		}
	}
	FileIO.SetErrorString(_T("Error, ReadAlgParamFile_CharVerify Fault"));
	return false;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ExecAlgInspection_CharVerify(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList)
{	
#ifdef PATTERN_BINARY_USE
	return ExecAlgInspection_CharVerify_v2(ModelPtr, ModelRgn, RoiRect, WndRect, UniFrameList);
#endif//PATTERN_BINARY_USE
	return ExecAlgInspection_CharVerify_v1(ModelPtr, ModelRgn, RoiRect, WndRect, UniFrameList);
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ExecAlgInspection_CharVerify_v1(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList)
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
	const int          nPatAlign = 4;
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
	TALG_PARAM_CHAR_VERIFY &cvParam = GetAlgParamCharVerify();
	const double     CellScoreMin = cvParam.cvCellScoreMin;
	const double     CellScoreMax = cvParam.cvCellScoreMax;
	const int        PatternPolarity = GetAlgPatternPolarity();	
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
	IMAGE_PTR       RoiMaskPtr = NULL;	
	IMAGE_PTR       RoiImagePtr = NULL;	
	IMAGE_SIZE      RoiImageW = RoiRect.right-RoiRect.left;
	IMAGE_SIZE      RoiImageH = RoiRect.bottom-RoiRect.top;	
	IMAGE_SIZE      RoiBitCount = 0;
	IMAGE_SIZE      RoiImageStep = 0;	
	IMAGE_SIZE      MaskImageStep = 0;

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
	
	if ( BINARY_DISABLE != BinaryMode )
	{	
		MaskImageStep = JetAPI::GetBMPImagePixelsPerLine(RoiImageW, MaskBitCount, nRoiAlign);
		IsOK = ImageAPI.ExtractGrayRoiImage(MaskW, MaskH, MaskStep, MaskPtr, RoiRect, MaskImageStep, RoiMaskPtr, false); 
	}	
	if ( false == IsOK )
	{
		JetMemory.free_func(MaskPtr);
		JetMemory.free_func(GrayPtr);
		JetMemory.free_func(RoiMaskPtr);		
		JetMemory.free_func(RoiImagePtr);		
		return false;	
	}	

	CString         str;
	size_t          i=0, j=0;	
	RECT            PatRect={0,0,0,0};
	RECT            PatRoiRect={0,0,0,0};
	RECT            PatRoiRect2={0,0,0,0};
	RECT            ImgRoiRect={0,0,0,0};
	TPATTERN_ROI   *PatRoiPtr = NULL;
	CPatternParam  *PatParamPtr = NULL;
	size_t          PatternRoiCount=0;
	IMAGE_PTR       PatImgPtr=NULL;
	IMAGE_PTR       PatMaskPtr=NULL;
	IMAGE_PTR       PatGrayPtr=NULL;
	IMAGE_PTR       PatImgPtrUse=NULL;
	IMAGE_PTR       PatRoiImgPtr=NULL;
	IMAGE_PTR       ImgRoiImgPtr=NULL;
	IMAGE_PTR       PatRoiImgPtrUse=NULL;
	IMAGE_SIZE      PatImgW=0, PatImgH=0, PatImgStep=0, PatBitCount=0, PatMaskImageStep=0;
	IMAGE_SIZE      PatImgWUse=0, PatImgHUse=0, PatImgStepUse=0, PatBitCountUse=0;
	IMAGE_SIZE      PatRoiWUse=0, PatRoiHUse=0, PatRoiStepUse=0, PatRoiBitCountUse=0;
	IMAGE_SIZE      PatRoiImgW=0, PatRoiImgH=0, PatRoiImgStep=0;
	IMAGE_SIZE      ImgRoiImgW=0, ImgRoiImgH=0, ImgRoiImgStep=0;	
	const size_t    PatternCount = GetAlgPatternCount();
	const bool      bPatternTestAll = GetAlgPatternTestAll();	
	JET_MATCH_LIB_TYPE MatchLibType = AOIDataCollect.GetMatchLibType();

	int       nRoiImageStep = (int)(RoiImageStep);
	int       nRoiBitCount = (int)(RoiBitCount);
	const int       nRoiImageW = (int)(RoiImageW);
	const int       nRoiImageH = (int)(RoiImageH);	
	const int       nRoiX = RoiRect.left;
	const int       nRoiY = RoiRect.top;
	const int       nRoiW = (int)(nRoiImageW);
	const int       nRoiH = (int)(nRoiImageH);
	const int       nRoiBitCountBackup = nRoiBitCount;
	const int       nRoiImageStepBackup = nRoiImageStep;	
	const int       nRoiRectSize = (int)((nRoiW)*(nRoiH));
	const double    dRoiCpx = (double)((RoiRect.left+RoiRect.right)*0.5);
	const double    dRoiCpy = (double)((RoiRect.top+RoiRect.bottom)*0.5);
	TPOINT2D        WndRectCalValue = WndPtr->GetWndRectCalValue();
	//WndRectCalValue.x = 0;//-WndRectCalValue.x;
	//WndRectCalValue.y = 0;//WndRectCalValue.y;

	int   idx = 0;
	int    NResults = 0;		
	int    PolarityIdx=0;
	int    BestPatternIndex=-1;	
	double ResultCX=0.0, ResultCY=0.0;	
	double RoiOffsetX=0.0, RoiOffsetY=0.0;	
	double CadOffsetX=0.0, CadOffsetY=0.0, CadSkew=0.0, CadScaleX=100.0, CadScaleY=100.0;	
	double ResultX=0, ResultY=0, ResultA=0, ResultS=0, ResultSX=0, ResultSY=0, ResultMatch=0;	
	double BestResX=0, BestResY=0, BestResA=0, BestResS=0, BestResSX=0, BestResSY=0, BestResMatch=0;		
	double Reading=0.0, USL=0.0, LSL=0.0;		
	bool   ApplyScale=false;
	const bool      bRobustness = true;	
	const int       nMinReduceArea = GetAlgPatternMinReducedArea();
	const int       nFinalReduction = GetAlgPatternFinalReduction();
	const bool      bAdvancedLearning = GetAlgPatternAdvancedLearning();
	const bool      UseInterpolate = true;//CheckAlgPatternMatrchInterpolate();

#ifdef _DEBUG	
	bool         bSave = true;	
	CString      ComponentName;	
	CString      DebugFolder=GetAlgDebugFolder();
	if ( NULL != ComponentPtr )
	{	ComponentName = ComponentPtr->GetComponentFullName(); }	
	if ( true == bSave )
	{
		str.Format(_T("%s\\%s_ModelWndAlgCharVerify#%d_Roi.PNG"), DebugFolder, ComponentName, WndIndex+1);
		ImageAPI.SaveImage(str, RoiImageW, RoiImageH, RoiImageStep, RoiBitCount, RoiImagePtr, true);
	}
#endif//_DEBUG

	TPATTERN_ROI PatternRoi;
	CJetMatch    Match;	//影像匹配
	
	if ( Match.SetMatchLibType(MatchLibType)==false )
	{	
		JetMemory.free_func(MaskPtr);
		JetMemory.free_func(GrayPtr);
		JetMemory.free_func(RoiMaskPtr);
		JetMemory.free_func(RoiImagePtr);
		return false;	
	}
	
	double       ImgRoiScore=0;	
	double       CompareScore=0;
	double       ImgRoiOKCnt=0;		
	double       ImgRoiTotalCnt=0;
	double       ImgRoiPassRatio=0;
	const double ImgRoiScoreMax=128.0;
	const int    nGridX = cvParam.cvCellGridCnt;
	const int    nGridY = cvParam.cvCellGridCnt;
	const int    nCellExtSize=cvParam.cvCellExtSize;
	std::vector<TPATTERN_ROI> PatRoiList;	

	BestResS = -1.0;
	USL = cvParam.cvPassRatioUSL;
	LSL = cvParam.cvPassRatioLSL;
	for ( i=0; i<PatternCount; i++ )
	{
		PatParamPtr = CAlgParam::GetAlgPatternParamPtr(i, true);
		if ( NULL == PatParamPtr ) 
		{	continue;	}

		PolarityIdx=0;
		PatRoiList=PatParamPtr->GetPatRoiList(WndToward, PolarityIdx);
		PatternRoiCount = PatRoiList.size();
		if ( 0 == PatternRoiCount )
		{	continue; }

		if ( CAlgParam::LoadAlgPatternImage(i, WndToward, PatImgW, PatImgH, PatImgStep, PatBitCount, PatImgPtr) == false ) 
		{	continue;	}

		PatRect.left   = 0;
		PatRect.top    = 0;
		PatRect.right  = PatImgW;
		PatRect.bottom = PatImgH;
		nRoiBitCount = nRoiBitCountBackup;
		nRoiImageStep = nRoiImageStepBackup;	
		if ( CheckImageSourceGrayMode(ImageSourceMode) == true )
		{
			if ( 24 == PatBitCount )
			{
				PatImgWUse = PatImgW;
				PatImgHUse = PatImgH;
				PatBitCountUse = 8;
				PatImgStepUse = JetAPI::GetBMPImagePixelsPerLine(PatImgW, MaskBitCount, nPatAlign);
				if ( ImageAPI.ColorImageToGrayImage(PatImgW, PatImgH, PatImgStep, PatImgPtr, PatRect, PatImgStepUse, PatGrayPtr, 
					ImageSourceMode, //與檢測框相同的影像來源
					PatParamPtr->GetBinaryParam().GetBinarySynthesisWR(), 
					PatParamPtr->GetBinaryParam().GetBinarySynthesisWG(), 
					PatParamPtr->GetBinaryParam().GetBinarySynthesisWB(), 
					false) == false )
				{
					JetMemory.free_func(PatImgPtr);
					JetMemory.free_func(PatMaskPtr);
					JetMemory.free_func(PatGrayPtr);
					continue;
				}
				PatImgPtrUse = PatGrayPtr;
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
					JetMemory.free_func(PatGrayPtr);
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
		if ( BINARY_DISABLE != BinaryMode )//需要比較是否使用2值化影像
		{				
			PatMaskImageStep = JetAPI::GetBMPImagePixelsPerLine(PatImgW, MaskBitCount, nPatAlign);			
			if ( BINARY_DISABLE != PatParamPtr->GetBinaryParam().GetBinaryMode() )
			{	//樣板參數有設定定2值化參數				
				if ( ExecAlgImageBinary_Rect(PatParamPtr->GetBinaryParam(), PatRect, PatRect, PatImgW, PatImgH, PatImgStep, PatBitCount, PatImgPtr, NULL, NULL, nPatAlign, PatMaskPtr) == false )
				{	
					JetMemory.free_func(PatImgPtr);
					JetMemory.free_func(PatMaskPtr);
					JetMemory.free_func(PatGrayPtr);
					continue;	
				}				
			}
			else
			{	//樣板參數未設定2值化參數
				if ( ImageAPI.ColorImageToGrayImage(PatImgW, PatImgH, PatImgStep, PatImgPtr, PatRect, PatMaskImageStep, PatMaskPtr, 
					ImageSourceMode, //與檢測框相同的影像來源
					PatParamPtr->GetBinaryParam().GetBinarySynthesisWR(), 
					PatParamPtr->GetBinaryParam().GetBinarySynthesisWG(), 
					PatParamPtr->GetBinaryParam().GetBinarySynthesisWB(), 
					false) == false )
				{
					JetMemory.free_func(PatImgPtr);
					JetMemory.free_func(PatMaskPtr);
					JetMemory.free_func(PatGrayPtr);
					continue;
				}
			}			
		}		
		if ( PatBitCountUse != RoiBitCount )
		{
			JetMemory.free_func(PatImgPtr);
			JetMemory.free_func(PatMaskPtr);
			JetMemory.free_func(PatGrayPtr);
			continue; 
		}
	
	#ifdef _DEBUG
		if ( true == bSave )
		{			
			if ( NULL != PatMaskPtr )
			{
				str.Format(_T("%s\\%s_ModelWndAlgCharVerify#%d_PatMaskT#%d.PNG"), DebugFolder, ComponentName, WndIndex+1, i+1);
				ImageAPI.SaveImage(str, PatImgWUse, PatImgHUse, PatMaskImageStep, MaskBitCount, PatMaskPtr, true);			
			}
			str.Format(_T("%s\\%s_ModelWndAlgCharVerify#%d_PatternT#%d.PNG"), DebugFolder, ComponentName, WndIndex+1, i+1);
			ImageAPI.SaveImage(str, PatImgWUse, PatImgHUse, PatImgStepUse, PatBitCountUse, PatImgPtrUse, true);				
			str.Format(_T("%s\\%s_ModelWndAlgCharVerify#%d_ImageT#%d.PNG"), DebugFolder, ComponentName, WndIndex+1, i+1);
			ImageAPI.SaveImage(str, nRoiImageW, nRoiImageH, nRoiImageStep, nRoiBitCount, RoiImagePtr, true);
		}
	#endif//_DEBUG

		//initial eMatch
		Match.SetMatchDefaultParam();
		Match.SetRobustness(bRobustness);
		Match.SetMinReducedArea(nMinReduceArea);
		Match.SetFinalReduction(nFinalReduction);
		if ( Match.LearnPattern(PatImgWUse, PatImgHUse, PatImgStepUse, PatBitCountUse, PatImgPtrUse, true) == false )
		{
			JetMemory.free_func(PatImgPtr);
			JetMemory.free_func(PatMaskPtr);
			JetMemory.free_func(PatGrayPtr);
			continue; 
		}		

		if ( true == GetAlgSkewEnabled())
		{
			double dSkew = GetAlgSkewCalcRange();
			float fSkewMax =  (float)(dSkew);
			float fSkewMin = -(float)(dSkew);
			Match.SetMinAngle(fSkewMin);
			Match.SetMaxAngle(fSkewMax);
			Match.SetUseAngle(true);				
		}
		Match.SetInterpolate(UseInterpolate);
		//Match.SetInterpolate(false);
		Match.SetMinScore(-1);//fMinScore
		if ( Match.Match(nRoiImageW, nRoiImageH, nRoiImageStep, nRoiBitCount, RoiImagePtr, true) == false )
		{
			JetMemory.free_func(PatImgPtr);
			JetMemory.free_func(PatMaskPtr);
			JetMemory.free_func(PatGrayPtr);
			continue;	
		}
		
		NResults = Match.GetNumPositions();
		PatternRoiCount = PatRoiList.size();
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
			ResultMatch = ResultS;

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

			//const int PatImgWUse2 = (int)(PatImgWUse*0.5);
			//const int PatImgHUse2 = (int)(PatImgHUse*0.5);
			const int ImgRoiX=(int)(ResultX-(PatImgWUse*0.5)+0.5);
			const int ImgRoiY=(int)(ResultY-(PatImgHUse*0.5)+0.5);
			
			bool       RotateImageUsed=false;
			IMAGE_PTR  RoiImgPtrSrc=NULL;
			IMAGE_PTR  RoiMaskPtr_Bin = NULL;
			IMAGE_PTR  RoiImgPtrRotated=NULL;
			IMAGE_PTR  RoiMaskPtrUsed= RoiMaskPtr;
			IMAGE_PTR  RoiImgPtrUsed = RoiImagePtr;
			if ( BINARY_DISABLE == BinaryMode )
			{	RoiImgPtrSrc = RoiImagePtr;	}
			else
			{	RoiMaskPtrUsed = RoiMaskPtr; }
			if ( CheckRotateImage(ResultA) == true )//確認影像要角度補正
			{
				if ( BINARY_DISABLE == BinaryMode )
				{	ImageAPI.RotateImage(ResultA, ResultX, ResultY, RoiImageW, RoiImageH, RoiImageStep, RoiBitCount, RoiImagePtr, RoiImageW, RoiImageH, RoiImageStep, RoiImgPtrRotated);	}
				else
				{	ImageAPI.RotateImage(ResultA, ResultX, ResultY, RoiImageW, RoiImageH, MaskImageStep, MaskBitCount, RoiMaskPtr, RoiImageW, RoiImageH, MaskImageStep, RoiImgPtrRotated);	}
				if ( NULL != RoiImgPtrRotated )			
				{	
					RotateImageUsed = true;
					if ( BINARY_DISABLE == BinaryMode )
					{	RoiImgPtrSrc = RoiImgPtrRotated;	}
					else
					{	RoiMaskPtrUsed = RoiImgPtrRotated; }
				}
			}
			if ( BINARY_DISABLE != BinaryMode )
			{
				if ( BINARY_DYNAMIC_THRESHOLD == BinaryMode )//使用相對閥值, 需要重新計算
				{
					RECT    BinRect;					
					BinRect.left = (ImgRoiX);
					BinRect.top = (ImgRoiY);
					BinRect.right = (ImgRoiX)+(int)(PatImgWUse);
					BinRect.bottom = (ImgRoiY)+(int)(PatImgHUse);
					JetAPI::BoundaryRect(RoiImageW, RoiImageH, BinRect);
					if ( ExecAlgImageBinary_Rect(m_AlgImageBinParam, BinRect, BinRect, RoiImageW, RoiImageH, RoiImageStep, RoiBitCount, RoiImagePtr, NULL, NULL, nRoiAlign, RoiMaskPtr_Bin) == true )
					{	
						RoiMaskPtrUsed = RoiMaskPtr_Bin;
						if ( NULL != RoiImgPtrRotated )
						{
							JetMemory.free_func(RoiImgPtrRotated);
							ImageAPI.RotateImage(ResultA, ResultX, ResultY, RoiImageW, RoiImageH, MaskImageStep, MaskBitCount, RoiMaskPtr_Bin, RoiImageW, RoiImageH, MaskImageStep, RoiImgPtrRotated);
							if ( NULL != RoiImgPtrRotated )
							{	RoiMaskPtrUsed = RoiImgPtrRotated;	}
						}
					}
				}
				RoiImgPtrUsed = RoiMaskPtrUsed;
				nRoiBitCount = (int)(MaskBitCount);
				nRoiImageStep = (int)(MaskImageStep);

				PatRoiWUse = PatImgWUse;
				PatRoiHUse = PatImgHUse;
				PatRoiStepUse = PatMaskImageStep;
				PatRoiBitCountUse = MaskBitCount;
				PatRoiImgPtrUse = PatMaskPtr;				
			}
			else
			{
				RoiImgPtrUsed = RoiImgPtrSrc;
				nRoiBitCount = (int)(RoiBitCount);
				nRoiImageStep = (int)(RoiImageStep);	
				
				PatRoiWUse = PatImgWUse;
				PatRoiHUse = PatImgHUse;
				PatRoiStepUse = PatImgStepUse;
				PatRoiBitCountUse = PatBitCountUse;
				PatRoiImgPtrUse = PatImgPtrUse;				
			}
			for ( j=0; j<PatternRoiCount; j++ )
			{
				PatRoiPtr = &(PatRoiList[j]);
				PatRoiPtr->RoiShift.x = 0;
				PatRoiPtr->RoiShift.y = 0;
				PatRoiRect = PatRoiPtr->RoiRect;
				PatRoiPtr->RoiRectRes=PatRoiPtr->RoiRect;
				if ( true == RotateImageUsed )
				{	
					TPOINT2D TmpCornerPt[4];
					RECT TmpRect=PatRoiPtr->RoiRect;					
					const double CpX=(PatRoiWUse*0.5);
					const double CpY=(PatRoiHUse*0.5);
					JetAPI::RectToCornerPt(TmpRect, TmpCornerPt);
					JetAPI::RotateCornerPos(ResultA, CpX, CpY, TmpCornerPt);
					JetAPI::CornerPtToRect(TmpCornerPt, TmpRect);
					PatRoiPtr->RoiRectRes = TmpRect;					
				}	

				JetAPI::BoundaryRect(PatRect, PatRoiRect);
				PatRoiImgW = PatRoiRect.right-PatRoiRect.left;
				PatRoiImgH = PatRoiRect.bottom-PatRoiRect.top;
				PatRoiImgStep = JetAPI::GetBMPImagePixelsPerLine(PatRoiImgW, PatRoiBitCountUse, nPatAlign);
				if ( ImageAPI.ExtractRoiImage(PatRoiWUse, PatRoiHUse, PatRoiStepUse, PatRoiBitCountUse, PatRoiImgPtrUse, PatRoiRect, PatRoiImgStep, PatRoiImgPtr, false) == false )
				{	
					JetMemory.free_func(PatGrayPtr);
					continue;	
				}

				int RoiShiftX=0, RoiShiftY=0;
				if ( nCellExtSize > 0 )//子框重新定位
				{	
					float TmpScore=0.0f, TmpX=0.0f, TmpY=0.0f, TmpA=0.0f;
					const float RoiCpX=(PatRoiRect.right+PatRoiRect.left)*0.5f+ImgRoiX;
					const float RoiCpY=(PatRoiRect.bottom+PatRoiRect.top)*0.5f+ImgRoiY;
					ImgRoiRect.left   = PatRoiRect.left + ImgRoiX - nCellExtSize;
					ImgRoiRect.top    = PatRoiRect.top + ImgRoiY - nCellExtSize;
					ImgRoiRect.right  = PatRoiRect.right + ImgRoiX + nCellExtSize;
					ImgRoiRect.bottom = PatRoiRect.bottom + ImgRoiY + nCellExtSize;
					JetAPI::BoundaryRect(RoiImageW, RoiImageH, ImgRoiRect);
					ImgRoiImgW = ImgRoiRect.right-ImgRoiRect.left;
					ImgRoiImgH = ImgRoiRect.bottom-ImgRoiRect.top;
					ImgRoiImgStep = JetAPI::GetBMPImagePixelsPerLine(ImgRoiImgW, (unsigned int)nRoiBitCount, nPatAlign);
					if ( ImageAPI.ExtractRoiImage(nRoiImageW, nRoiImageH, nRoiImageStep, nRoiBitCount, RoiImgPtrUsed, ImgRoiRect, ImgRoiImgStep, ImgRoiImgPtr, false) == true )
					{	
					#ifdef _DEBUG
						if ( true == bSave )
						{					
							str.Format(_T("%s\\%s_ModelWndAlgCharVerify#%d_PatRoiT#%d#%d_Ext.PNG"), DebugFolder, ComponentName, WndIndex+1, i+1, j+1);
							ImageAPI.SaveImage(str, PatRoiImgW, PatRoiImgH, PatRoiImgStep, PatRoiBitCountUse, PatRoiImgPtr, true);					
							str.Format(_T("%s\\%s_ModelWndAlgCharVerify#%d_ImgRoiT#%d#%d_Ext.PNG"), DebugFolder, ComponentName, WndIndex+1, i+1, j+1);
							ImageAPI.SaveImage(str, ImgRoiImgW, ImgRoiImgH, ImgRoiImgStep, PatRoiBitCountUse, ImgRoiImgPtr, true);
						}
					#endif//_DEBUG
						if ( ImageAPI.MatchImage(PatRoiImgW, PatRoiImgH, PatRoiImgStep, PatRoiBitCountUse, PatRoiImgPtr, ImgRoiImgW, ImgRoiImgH, ImgRoiImgStep, RoiBitCount, ImgRoiImgPtr, TmpScore, TmpX, TmpY, TmpA) == true )
						{
							const float ResCpX=ImgRoiRect.left+TmpX;
							const float ResCpY=ImgRoiRect.top +TmpY;
							const float ShiftX=ResCpX-RoiCpX;
							const float ShiftY=ResCpY-RoiCpY;
							if ( ShiftX > 0.0f )
							{	RoiShiftX = (int)(ShiftX+0.5f); }
							else
							{	RoiShiftX = (int)(ShiftX-0.5f); }
							if ( ShiftY > 0.0f )
							{	RoiShiftY = (int)(ShiftY+0.5f); }
							else
							{	RoiShiftY = (int)(ShiftY-0.5f); }							
						}
						JetMemory.free_func(ImgRoiImgPtr);
					}
				}

				PatRoiPtr->RoiShift.x = RoiShiftX;
				PatRoiPtr->RoiShift.y = RoiShiftY;
				ImgRoiRect.left   = PatRoiRect.left + ImgRoiX + RoiShiftX;
				ImgRoiRect.top    = PatRoiRect.top + ImgRoiY + RoiShiftY;
				ImgRoiRect.right  = PatRoiRect.right + ImgRoiX + RoiShiftX;
				ImgRoiRect.bottom = PatRoiRect.bottom + ImgRoiY + RoiShiftY;
				JetAPI::BoundaryRect(nRoiImageW, nRoiImageH, ImgRoiRect);
				ImgRoiImgW = ImgRoiRect.right-ImgRoiRect.left;
				ImgRoiImgH = ImgRoiRect.bottom-ImgRoiRect.top;
				ImgRoiImgStep = JetAPI::GetBMPImagePixelsPerLine(ImgRoiImgW, (unsigned int)nRoiBitCount, nPatAlign);
				if ( ImageAPI.ExtractRoiImage(nRoiImageW, nRoiImageH, nRoiImageStep, nRoiBitCount, RoiImgPtrUsed, ImgRoiRect, ImgRoiImgStep, ImgRoiImgPtr, false) == false )
				{	
					JetMemory.free_func(PatRoiImgPtr);
					continue;	
				}

			#ifdef _DEBUG
				if ( true == bSave )
				{					
					str.Format(_T("%s\\%s_ModelWndAlgCharVerify#%d_PatRoiT#%d#%d.PNG"), DebugFolder, ComponentName, WndIndex+1, i+1, j+1);
					ImageAPI.SaveImage(str, PatRoiImgW, PatRoiImgH, PatRoiImgStep, PatRoiBitCountUse, PatRoiImgPtr, true);					
					str.Format(_T("%s\\%s_ModelWndAlgCharVerify#%d_ImgRoiT#%d#%d.PNG"), DebugFolder, ComponentName, WndIndex+1, i+1, j+1);
					ImageAPI.SaveImage(str, ImgRoiImgW, ImgRoiImgH, ImgRoiImgStep, PatRoiBitCountUse, ImgRoiImgPtr, true);
				}
			#endif//_DEBUG

				if ( ImageAPI.Compare2Image(ImgRoiImgW, ImgRoiImgH, ImgRoiImgStep, PatRoiBitCountUse, PatRoiImgPtr, ImgRoiImgPtr, nGridX, nGridY, CompareScore) == false )
				{	
					JetMemory.free_func(PatRoiImgPtr);
					JetMemory.free_func(ImgRoiImgPtr);
					continue;
				}
				JetMemory.free_func(PatRoiImgPtr);
				JetMemory.free_func(ImgRoiImgPtr);
				PatRoiPtr->ResultScore = CompareScore;				
			}
			JetMemory.free_func(RoiMaskPtr_Bin);
			JetMemory.free_func(RoiImgPtrRotated);
			//判斷檢測結果
			ImgRoiOKCnt = 0.0;
			ImgRoiTotalCnt = 0.0;
			for ( j=0; j<PatternRoiCount; j++ )
			{
				PatRoiPtr = &(PatRoiList[j]);
				ImgRoiTotalCnt  = ImgRoiTotalCnt + 1.0;
				if ( PatRoiPtr->ResultScore<CellScoreMin || PatRoiPtr->ResultScore>CellScoreMax )
				{
					PatRoiPtr->ResultID = RESULT_ID_NG;
					continue;;	
				}
				PatRoiPtr->ResultID = RESULT_ID_OK;
				ImgRoiOKCnt = ImgRoiOKCnt+1.0;				
			}
			if ( ImgRoiTotalCnt > 0 ) 
			{	ImgRoiPassRatio = 100.0*ImgRoiOKCnt/ImgRoiTotalCnt; }
			else
			{	ImgRoiPassRatio = 0.0; }
			switch ( WndToward )
			{
			case BOX_TOWARD_UP:    PatParamPtr->SetPatRoiListT(PolarityIdx, PatRoiList);	break;
			case BOX_TOWARD_LEFT:  PatParamPtr->SetPatRoiListL(PolarityIdx, PatRoiList);	break;
			case BOX_TOWARD_DOWN:  PatParamPtr->SetPatRoiListB(PolarityIdx, PatRoiList);	break;
			case BOX_TOWARD_RIGHT: PatParamPtr->SetPatRoiListR(PolarityIdx, PatRoiList);	break;
			default:
				break;
			}			
			ResultS = ImgRoiPassRatio;
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
				BestResMatch = ResultMatch;
				BestPatternIndex = (int)(i);				
			}
			if ( ResultS>=LSL && ResultS<=USL )
			{
				PatParamPtr->SetReultID(PolarityIdx, RESULT_ID_OK);
				PatParamPtr->CheckResultIndex();
				JetMemory.free_func(PatImgPtr);
				JetMemory.free_func(PatMaskPtr);				
				JetMemory.free_func(PatGrayPtr);
				if ( false == bPatternTestAll ) 
				{	break;	}
				else
				{	continue; }
			}
			/*
			else
			{
				if ( ResultMatch > BestResMatch )
				{
					BestResS = ResultS;
					BestResX = ResultX;
					BestResY = ResultY;
					BestResA = ResultA;
					BestResSX = ResultSX;
					BestResSY = ResultSY;
					BestResMatch = ResultMatch;
					BestPatternIndex = (int)(i);					
				}
			}*/		
		}

		if ( 2 == PatternPolarity )
		{
			PolarityIdx = 1;
			IMAGE_PTR  PatImgPtr2 = NULL;
			IMAGE_PTR  PatMaskPtr2 = NULL;
			IMAGE_SIZE PatImgW2=0, PatImgH2=0, PatImgStep2=0;
			IMAGE_SIZE PatMaskW2=0, PatMaskH2=0, PatMaskStep2=0;
			if ( ImageAPI.RotateImage(180.0, PatImgWUse, PatImgHUse, PatImgStepUse, PatBitCountUse, PatImgPtrUse, PatImgW2, PatImgH2, PatImgStep2, PatImgPtr2) == false )
			{	
				JetMemory.free_func(PatImgPtr);
				JetMemory.free_func(PatMaskPtr);
				JetMemory.free_func(PatGrayPtr);
				continue;
			}
			if ( NULL != PatMaskPtr )
			{
				if ( ImageAPI.RotateImage(180.0, PatImgWUse, PatImgHUse, PatMaskImageStep, MaskBitCount, PatMaskPtr, PatMaskW2, PatMaskH2, PatMaskStep2, PatMaskPtr2) == false )
				{	
					JetMemory.free_func(PatImgPtr);
					JetMemory.free_func(PatImgPtr2);
					JetMemory.free_func(PatMaskPtr);
					JetMemory.free_func(PatMaskPtr2);
					JetMemory.free_func(PatGrayPtr);					
					continue;
				}				
			}			
		#ifdef _DEBUG
			if ( true == bSave )
			{				
				str.Format(_T("%s\\%s_ModelWndAlgCharVerify#%d_PatternB#%d.PNG"), DebugFolder, ComponentName, WndIndex+1, i+1);
				ImageAPI.SaveImage(str, PatImgW2, PatImgH2, PatImgStep2, PatBitCountUse, PatImgPtr2, true);
				if ( NULL != PatMaskPtr2 )
				{
					str.Format(_T("%s\\%s_ModelWndAlgCharVerify#%d_PatMaskB#%d.PNG"), DebugFolder, ComponentName, WndIndex+1, i+1);
					ImageAPI.SaveImage(str, PatMaskW2, PatMaskH2, PatMaskStep2, MaskBitCount, PatMaskPtr2, true);
				}
			}
		#endif//_DEBUG
			Match.SetMatchDefaultParam();
			if ( Match.LearnPattern(PatImgW2, PatImgH2, PatImgStep2, PatBitCountUse, PatImgPtr2, true) == false )
			{
				JetMemory.free_func(PatImgPtr);
				JetMemory.free_func(PatImgPtr2);
				JetMemory.free_func(PatMaskPtr);
				JetMemory.free_func(PatMaskPtr2);
				JetMemory.free_func(PatGrayPtr);
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
			Match.SetInterpolate(UseInterpolate);
			Match.SetMinScore(-1);//fMinScore
			nRoiBitCount = (int)(RoiBitCount);
			nRoiImageStep = (int)(RoiImageStep);	
			nRoiBitCount = nRoiBitCountBackup;
			nRoiImageStep = nRoiImageStepBackup;	
			if ( Match.Match(nRoiImageW, nRoiImageH, nRoiImageStep, nRoiBitCount, RoiImagePtr, true) == false )
			{
				JetMemory.free_func(PatImgPtr);
				JetMemory.free_func(PatImgPtr2);
				JetMemory.free_func(PatMaskPtr);
				JetMemory.free_func(PatMaskPtr2);
				JetMemory.free_func(PatGrayPtr);
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
				ResultMatch = ResultS;

				ResultCX = ResultX+nRoiX;
				ResultCY = ResultY+nRoiY;
				RoiOffsetX = ResultCX-dRoiCpx;
				RoiOffsetY = dRoiCpy-ResultCY;
				CadSkew = -ResultA;
				CadOffsetX = RoiOffsetX/ImageScale.x;
				CadOffsetY = RoiOffsetY/ImageScale.y;
				CadOffsetX = CadOffsetX + WndRectCalValue.x;
				CadOffsetY = CadOffsetY + WndRectCalValue.y;

				PatParamPtr->SetPatResultW(PolarityIdx, PatImgW2);
				PatParamPtr->SetPatResultH(PolarityIdx, PatImgH2);
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

				const int ImgRoiX=(int)(ResultX-(PatImgW2*0.5)+0.5);
				const int ImgRoiY=(int)(ResultY-(PatImgH2*0.5)+0.5);				
				
				bool       RotateImageUsed=false;
				IMAGE_PTR  RoiImgPtrSrc=NULL;
				IMAGE_PTR  RoiMaskPtr_Bin = NULL;
				IMAGE_PTR  RoiImgPtrRotated=NULL;
				IMAGE_PTR  RoiMaskPtrUsed= RoiMaskPtr;
				IMAGE_PTR  RoiImgPtrUsed = RoiImagePtr;
				if ( BINARY_DISABLE == BinaryMode )
				{	RoiImgPtrSrc = RoiImagePtr;	}
				else
				{	RoiMaskPtrUsed = RoiMaskPtr; }
				if ( CheckRotateImage(ResultA) == true )//確認影像要角度補正
				{
					RotateImageUsed = true;
					if ( BINARY_DISABLE == BinaryMode )
					{	ImageAPI.RotateImage(ResultA, ResultX, ResultY, RoiImageW, RoiImageH, RoiImageStep, RoiBitCount, RoiImagePtr, RoiImageW, RoiImageH, RoiImageStep, RoiImgPtrRotated);	}
					else
					{	ImageAPI.RotateImage(ResultA, ResultX, ResultY, RoiImageW, RoiImageH, MaskImageStep, MaskBitCount, RoiMaskPtr, RoiImageW, RoiImageH, MaskImageStep, RoiImgPtrRotated);	}
					if ( NULL != RoiImgPtrRotated )				
					{	
						if ( BINARY_DISABLE == BinaryMode )
						{	RoiImgPtrSrc = RoiImgPtrRotated;	}
						else
						{	RoiMaskPtrUsed = RoiImgPtrRotated; }
					}
				}
				if ( BINARY_DISABLE != BinaryMode )
				{
					if ( BINARY_DYNAMIC_THRESHOLD == BinaryMode )//使用相對閥值, 需要重新計算
					{
						RECT    BinRect;						
						BinRect.left = (ImgRoiX);
						BinRect.top = (ImgRoiY);
						BinRect.right = (ImgRoiX)+(int)(PatImgWUse);
						BinRect.bottom = (ImgRoiY)+(int)(PatImgHUse);
						JetAPI::BoundaryRect(RoiImageW, RoiImageH, BinRect);
						if ( ExecAlgImageBinary_Rect(m_AlgImageBinParam, BinRect, BinRect, RoiImageW, RoiImageH, RoiImageStep, RoiBitCount, RoiImagePtr, NULL, NULL, nRoiAlign, RoiMaskPtr_Bin) == true )
						{								
							RoiMaskPtrUsed = RoiMaskPtr_Bin;
							if ( NULL != RoiImgPtrRotated )
							{
								JetMemory.free_func(RoiImgPtrRotated);
								ImageAPI.RotateImage(ResultA, ResultX, ResultY, RoiImageW, RoiImageH, MaskImageStep, MaskBitCount, RoiMaskPtr_Bin, RoiImageW, RoiImageH, MaskImageStep, RoiImgPtrRotated);
								if ( NULL != RoiImgPtrRotated )
								{	RoiMaskPtrUsed = RoiImgPtrRotated;	}
							}
						}
					}
					RoiImgPtrUsed = RoiMaskPtrUsed;
					nRoiBitCount = (int)(MaskBitCount);
					nRoiImageStep = (int)(MaskImageStep);

					PatRoiWUse = PatMaskW2;
					PatRoiHUse = PatMaskH2;
					PatRoiStepUse = PatMaskStep2;
					PatRoiBitCountUse = MaskBitCount;
					PatRoiImgPtrUse = PatMaskPtr2;
				}
				else
				{
					RoiImgPtrUsed = RoiImgPtrSrc;
					nRoiBitCount = (int)(RoiBitCount);
					nRoiImageStep = (int)(RoiImageStep);	

					PatRoiWUse = PatImgW2;
					PatRoiHUse = PatImgH2;
					PatRoiStepUse = PatImgStep2;
					PatRoiBitCountUse = PatBitCountUse;
					PatRoiImgPtrUse = PatImgPtr2;
				}				
				for ( j=0; j<PatternRoiCount; j++ )
				{
					PatRoiPtr = &(PatRoiList[j]);
					PatRoiPtr->RoiShift.x = 0;
					PatRoiPtr->RoiShift.y = 0;
					PatRoiRect = PatRoiPtr->RoiRect;		
					PatRoiPtr->RoiRectRes=PatRoiPtr->RoiRect;
					if ( true == RotateImageUsed )
					{						
						TPOINT2D TmpCornerPt[4];
						RECT TmpRect=PatRoiPtr->RoiRect;						
						const double CpX=(PatRoiWUse*0.5);
						const double CpY=(PatRoiHUse*0.5);
						JetAPI::RotateRect(180, PatRoiWUse, PatRoiHUse, TmpRect);
						JetAPI::RectToCornerPt(TmpRect, TmpCornerPt);
						JetAPI::RotateCornerPos(ResultA, CpX, CpY, TmpCornerPt);
						JetAPI::CornerPtToRect(TmpCornerPt, TmpRect);
						JetAPI::RotateRect(180, PatRoiWUse, PatRoiHUse, TmpRect);
						PatRoiPtr->RoiRectRes = TmpRect;						
					}

					//因為旋轉180, 所以位置要轉回去
					PatRoiRect2.left   = PatRoiWUse-PatRoiRect.right;
					PatRoiRect2.top    = PatRoiHUse-PatRoiRect.bottom;
					PatRoiRect2.right  = PatRoiWUse-PatRoiRect.left;
					PatRoiRect2.bottom = PatRoiHUse-PatRoiRect.top;

					JetAPI::BoundaryRect(PatRect, PatRoiRect2);
					PatRoiImgW = PatRoiRect2.right-PatRoiRect2.left;
					PatRoiImgH = PatRoiRect2.bottom-PatRoiRect2.top;
					PatRoiImgStep = JetAPI::GetBMPImagePixelsPerLine(PatRoiImgW, PatRoiBitCountUse, nPatAlign);
					if ( ImageAPI.ExtractRoiImage(PatRoiWUse, PatRoiHUse, PatRoiStepUse, PatRoiBitCountUse, PatRoiImgPtrUse, PatRoiRect2, PatRoiImgStep, PatRoiImgPtr, false) == false )
					{	
						JetMemory.free_func(PatGrayPtr);
						continue;	
					}
					
					int RoiShiftX=0, RoiShiftY=0;
					if ( nCellExtSize > 0 )//子框重新定位
					{	
						float TmpScore=0.0f, TmpX=0.0f, TmpY=0.0f, TmpA=0.0f;
						const float RoiCpX=(PatRoiRect2.right+PatRoiRect2.left)*0.5f+ImgRoiX;
						const float RoiCpY=(PatRoiRect2.bottom+PatRoiRect2.top)*0.5f+ImgRoiY;
						ImgRoiRect.left   = PatRoiRect2.left + ImgRoiX - nCellExtSize;
						ImgRoiRect.top    = PatRoiRect2.top + ImgRoiY - nCellExtSize;
						ImgRoiRect.right  = PatRoiRect2.right + ImgRoiX + nCellExtSize;
						ImgRoiRect.bottom = PatRoiRect2.bottom + ImgRoiY + nCellExtSize;
						JetAPI::BoundaryRect(RoiImageW, RoiImageH, ImgRoiRect);
						ImgRoiImgW = ImgRoiRect.right-ImgRoiRect.left;
						ImgRoiImgH = ImgRoiRect.bottom-ImgRoiRect.top;
						ImgRoiImgStep = JetAPI::GetBMPImagePixelsPerLine(ImgRoiImgW, (unsigned int)nRoiBitCount, nPatAlign);
						if ( ImageAPI.ExtractRoiImage(nRoiImageW, nRoiImageH, nRoiImageStep, nRoiBitCount, RoiImgPtrUsed, ImgRoiRect, ImgRoiImgStep, ImgRoiImgPtr, false) == true )
						{	
						#ifdef _DEBUG
							if ( true == bSave )
							{					
								str.Format(_T("%s\\%s_ModelWndAlgCharVerify#%d_PatRoiB#%d#%d_Ext.PNG"), DebugFolder, ComponentName, WndIndex+1, i+1, j+1);
								ImageAPI.SaveImage(str, PatRoiImgW, PatRoiImgH, PatRoiImgStep, PatRoiBitCountUse, PatRoiImgPtr, true);					
								str.Format(_T("%s\\%s_ModelWndAlgCharVerify#%d_ImgRoiB#%d#%d_Ext.PNG"), DebugFolder, ComponentName, WndIndex+1, i+1, j+1);
								ImageAPI.SaveImage(str, ImgRoiImgW, ImgRoiImgH, ImgRoiImgStep, PatRoiBitCountUse, ImgRoiImgPtr, true);
							}
						#endif//_DEBUG
							if ( ImageAPI.MatchImage(PatRoiImgW, PatRoiImgH, PatRoiImgStep, PatRoiBitCountUse, PatRoiImgPtr, ImgRoiImgW, ImgRoiImgH, ImgRoiImgStep, RoiBitCount, ImgRoiImgPtr, TmpScore, TmpX, TmpY, TmpA) == true )
							{
								const float ResCpX=ImgRoiRect.left+TmpX;
								const float ResCpY=ImgRoiRect.top +TmpY;
								const float ShiftX=ResCpX-RoiCpX;
								const float ShiftY=ResCpY-RoiCpY;
								if ( ShiftX > 0.0f )
								{	RoiShiftX = (int)(ShiftX+0.5f); }
								else
								{	RoiShiftX = (int)(ShiftX-0.5f); }
								if ( ShiftY > 0.0f )
								{	RoiShiftY = (int)(ShiftY+0.5f); }
								else
								{	RoiShiftY = (int)(ShiftY-0.5f); }
							}
							JetMemory.free_func(ImgRoiImgPtr);
						}
					}

					PatRoiPtr->RoiShift.x = RoiShiftX;
					PatRoiPtr->RoiShift.y = RoiShiftY;
					ImgRoiRect.left   = PatRoiRect2.left + ImgRoiX + RoiShiftX;
					ImgRoiRect.top    = PatRoiRect2.top + ImgRoiY + RoiShiftY;
					ImgRoiRect.right  = PatRoiRect2.right + ImgRoiX + RoiShiftX;
					ImgRoiRect.bottom = PatRoiRect2.bottom + ImgRoiY + RoiShiftY;
					JetAPI::BoundaryRect(nRoiImageW, nRoiImageH, ImgRoiRect);
					ImgRoiImgW = ImgRoiRect.right-ImgRoiRect.left;
					ImgRoiImgH = ImgRoiRect.bottom-ImgRoiRect.top;
					ImgRoiImgStep = JetAPI::GetBMPImagePixelsPerLine(ImgRoiImgW, (unsigned int)nRoiBitCount, nPatAlign);
					if ( ImageAPI.ExtractRoiImage(nRoiImageW, nRoiImageH, nRoiImageStep, nRoiBitCount, RoiImgPtrUsed, ImgRoiRect, ImgRoiImgStep, ImgRoiImgPtr, false) == false )
					{	
						JetMemory.free_func(PatRoiImgPtr);
						continue;	
					}

				#ifdef _DEBUG
					if ( true == bSave )
					{	
						str.Format(_T("%s\\%s_ModelWndAlgCharVerify#%d_PatRoiB#%d#%d.PNG"), DebugFolder, ComponentName, WndIndex+1, i+1, j+1);
						ImageAPI.SaveImage(str, PatRoiImgW, PatRoiImgH, PatRoiImgStep, PatRoiBitCountUse, PatRoiImgPtr, true);						
						str.Format(_T("%s\\%s_ModelWndAlgCharVerify#%d_ImgRoiB#%d#%d.PNG"), DebugFolder, ComponentName, WndIndex+1, i+1, j+1);
						ImageAPI.SaveImage(str, ImgRoiImgW, ImgRoiImgH, ImgRoiImgStep, PatRoiBitCountUse, ImgRoiImgPtr, true);
					}
				#endif//_DEBUG

					if ( ImageAPI.Compare2Image(ImgRoiImgW, ImgRoiImgH, ImgRoiImgStep, PatRoiBitCountUse, PatRoiImgPtr, ImgRoiImgPtr, nGridX, nGridY, CompareScore) == false )
					{	
						JetMemory.free_func(PatRoiImgPtr);
						JetMemory.free_func(ImgRoiImgPtr);
						continue;
					}
					JetMemory.free_func(PatRoiImgPtr);
					JetMemory.free_func(ImgRoiImgPtr);
					PatRoiPtr->ResultScore = CompareScore;
				}
				JetMemory.free_func(RoiMaskPtr_Bin);
				JetMemory.free_func(RoiImgPtrRotated);
				//判斷檢測結果
				ImgRoiOKCnt = 0.0;
				ImgRoiTotalCnt = 0.0;
				for ( j=0; j<PatternRoiCount; j++ )
				{
					PatRoiPtr = &(PatRoiList[j]);
					ImgRoiTotalCnt  = ImgRoiTotalCnt + 1.0;
					if ( PatRoiPtr->ResultScore<CellScoreMin || PatRoiPtr->ResultScore>CellScoreMax )
					{
						PatRoiPtr->ResultID = RESULT_ID_NG;
						continue;	
					}
					PatRoiPtr->ResultID = RESULT_ID_OK;
					ImgRoiOKCnt = ImgRoiOKCnt+1.0;				
				}
				if ( ImgRoiTotalCnt > 0 ) 
				{	ImgRoiPassRatio = 100.0*ImgRoiOKCnt/ImgRoiTotalCnt; }
				else
				{	ImgRoiPassRatio = 0.0; }

				switch ( WndToward )
				{
				case BOX_TOWARD_UP:    PatParamPtr->SetPatRoiListT(PolarityIdx, PatRoiList);	break;
				case BOX_TOWARD_LEFT:  PatParamPtr->SetPatRoiListL(PolarityIdx, PatRoiList);	break;
				case BOX_TOWARD_DOWN:  PatParamPtr->SetPatRoiListB(PolarityIdx, PatRoiList);	break;
				case BOX_TOWARD_RIGHT: PatParamPtr->SetPatRoiListR(PolarityIdx, PatRoiList);	break;
				default:
					break;
				}
				ResultS = ImgRoiPassRatio;
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
					BestResMatch = ResultMatch;
					BestPatternIndex = (int)(i);					
				}
				if ( ResultS>=LSL && ResultS<=USL )
				{
					PatParamPtr->SetReultID(PolarityIdx, RESULT_ID_OK);
					PatParamPtr->CheckResultIndex();

					JetMemory.free_func(PatImgPtr2);
					JetMemory.free_func(PatImgPtr);
					JetMemory.free_func(PatMaskPtr);
					JetMemory.free_func(PatMaskPtr2);
					JetMemory.free_func(PatGrayPtr);
					if ( false == bPatternTestAll ) 
					{	break;	}
					else
					{	continue; }
				}
				/*
				else
				{
					if ( ResultMatch > BestResMatch )
					{
						BestResS = ResultS;
						BestResX = ResultX;
						BestResY = ResultY;
						BestResA = ResultA;
						BestResSX = ResultSX;
						BestResSY = ResultSY;
						BestResMatch = ResultMatch;
						BestPatternIndex = (int)(i);						
					}
				}*/
			}
			JetMemory.free_func(PatImgPtr2);
			JetMemory.free_func(PatMaskPtr2);
		}
		JetMemory.free_func(PatImgPtr);
		JetMemory.free_func(PatMaskPtr);
		JetMemory.free_func(PatGrayPtr);
	}
	JetMemory.free_func(RoiMaskPtr);
	JetMemory.free_func(RoiImagePtr);

	CString PatText;
	bool Matched = false;
	PatParamPtr = GetAlgPatternParamPtr(BestPatternIndex, true);
	if ( NULL != PatParamPtr ) 
	{
		Matched = true;
		PatText = PatParamPtr->GetPatText();
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

		TPOINT2D    RgnCp;
		TPOINT2D    ImageCp;
		CAOIBox     ResBox;
		TREGION4D   ResBoxRgn;

		RgnCp.x = ModelRgn.GetCpX();
		RgnCp.y = ModelRgn.GetCpY();
		ImageCp.x = FrameImageW;
		ImageCp.y = FrameImageH;
		ImageCp.x = ImageCp.x*0.5;
		ImageCp.y = ImageCp.y*0.5;
		PatImgWUse = PatParamPtr->GetPatResultW(PolarityIdx);
		PatImgHUse = PatParamPtr->GetPatResultH(PolarityIdx);			

		const int ImgRoiX=(int)(ResultX-(PatImgWUse*0.5)-0.5);
		const int ImgRoiY=(int)(ResultY-(PatImgHUse*0.5)-0.5);				
		//WndRectCalValue.x = -WndRectCalValue.x;
		//WndRectCalValue.y = -WndRectCalValue.y;
		PatRoiList=PatParamPtr->GetPatRoiList(WndToward, PolarityIdx);		
		PatternRoiCount = PatRoiList.size();
		for ( j=0; j<PatternRoiCount; j++ )
		{				
			PatRoiPtr = &(PatRoiList[j]);
			const int ShiftX = PatRoiPtr->RoiShift.x;
			const int ShiftY = PatRoiPtr->RoiShift.y;
			if ( 1 == PatParamPtr->GetResultPolarityIdx() )
			{
				//因為旋轉180, 所以位置要轉回去	
				PatRoiRect2 = PatRoiPtr->RoiRect;
				PatRoiRect2 = PatRoiPtr->RoiRectRes;
				PatRoiRect.left   = PatImgWUse-PatRoiRect2.right;
				PatRoiRect.top    = PatImgHUse-PatRoiRect2.bottom;
				PatRoiRect.right  = PatImgWUse-PatRoiRect2.left;
				PatRoiRect.bottom = PatImgHUse-PatRoiRect2.top;
			}
			else
			{	
				PatRoiRect = PatRoiPtr->RoiRect;
				PatRoiRect = PatRoiPtr->RoiRectRes;
			}
			PatRoiRect.left   += ResultSX+ImgRoiX+nRoiX+ShiftX;
			PatRoiRect.top    += ResultSY+ImgRoiY+nRoiY+ShiftY;
			PatRoiRect.right  += ResultSX+ImgRoiX+nRoiX+ShiftX;
			PatRoiRect.bottom += ResultSY+ImgRoiY+nRoiY+ShiftY;
			CAOIModel::CalcModelBoxRectRegion(PatRoiRect, FrameImageW, FrameImageH, RgnCp, ImageScale, ImageCp, ResBoxRgn);
			ResBox.SetBoxRegion(ResBoxRgn);				
			if ( PatRoiPtr->ResultScore<CellScoreMin || PatRoiPtr->ResultScore>CellScoreMax )
			{	ResBox.SetBoxResultID(RESULT_ID_NG);	}
			else
			{	ResBox.SetBoxResultID(RESULT_ID_OK);	}

			str.Format(_T("%.0f%%"), PatRoiPtr->ResultScore);
			ResBox.SetBoxResultText(str);
			ResBox.SetBoxResultTextVisibled(true);
			ResBox.SetBoxResultValue(PatRoiPtr->ResultScore);
			WndPtr->AddWndResultBox(ResBox);
		}		
	}
	if ( BestPatternIndex<0 && PatternCount>0 ) 
	{	BestPatternIndex = 0; }
	
	Reading = ResultS;
	USL = cvParam.cvPassRatioUSL;
	LSL = cvParam.cvPassRatioLSL;
	cvParam.cvPassRatioReading = Reading;
	SetAlgPatternSimilarityReading(Reading);		
	SetAlgPatternResultIndex(BestPatternIndex);		

	CString strResult;	
	SetAlgResultReading1(Reading);	
	SetAlgResultText(_T("OK"));
	SetAlgResultID(RESULT_ID_OK);
	if ( Reading<LSL || Reading>USL )
	{
		if ( PatText.GetLength() == 0 )
		{	strResult.Format(_T("NG:%.0f / %.0f"), Reading, LSL);	}
		else
		{	strResult.Format(_T("NG:%.0f/%.0f [%s]"), Reading, LSL, PatText);	}
		SetAlgResultID(RESULT_ID_NG);
		SetAlgResultText(strResult);
	}
	
	double CadSkew_Self = 0;
	double CadOffsetX_Self = 0;
	double CadOffsetY_Self = 0;
	double CadSkew_Others = 0;
	double CadOffsetX_Others = 0;
	double CadOffsetY_Others = 0;	
	const bool bChkDefect=false;
	const bool ApplySkewAngle = true;
	const bool AlgScaleEnabled = GetAlgScaleEnabled();	
	
	SetAlgSkewReading(CadSkew);	
	SetAlgImageOffsetX(RoiOffsetX);
	SetAlgImageOffsetY(RoiOffsetY);
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
	//套用至後面的檢測框
	if ( true == Matched )
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
		BoxPtr->SkewBoxAngle(CadSkew_Self);
		BoxPtr->MoveBoxRes(CadOffsetX_Self, CadOffsetY_Self);
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ExecAlgInspection_CharVerify_v2(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList)
{
	const char fnName[]="CAlgParam::ExecAlgInspection_CharVerify";
	if ( NULL == ModelPtr ) { return false; }
	CAOIComponent *ComponentPtr = ModelPtr->GetModelComponentPtr();
	const size_t FrameCount = UniFrameList.size();
	const size_t ImageFrameIndex = m_AlgImageBinParam.GetBinaryFrameIndex();
	if ( ImageFrameIndex >= FrameCount ) 
	{	return false; }	
	CAOIWnd         *WndPtr = CAlgParam::GetAlgWndPtr();
	if ( NULL == WndPtr ) { return false; }
	
	const int          nRoiAlign = 4;
	const int          nPatAlign = 4;
	BOX_TOWARD         WndToward = WndPtr->GetWndToward();
	const unsigned int WndIndex = WndPtr->GetWndIndex();		
	const TUNI_FRAME  &UniFrameRef  = UniFrameList[ImageFrameIndex];	
	const IMAGE_SIZE FrameImageW    = UniFrameRef.ImageW;
	const IMAGE_SIZE FrameImageH    = UniFrameRef.ImageH;
	TALG_PARAM_CHAR_VERIFY &cvParam = GetAlgParamCharVerify();
	const double     CellScoreMin = cvParam.cvCellScoreMin;
	const double     CellScoreMax = cvParam.cvCellScoreMax;
	const int        PatternPolarity = GetAlgPatternPolarity();	
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

	CString         str;
	size_t          i=0, j=0;	
	RECT            PatRect={0,0,0,0};
	RECT            PatRoiRect={0,0,0,0};
	RECT            PatRoiRect2={0,0,0,0};
	RECT            ImgRoiRect={0,0,0,0};
	TPATTERN_ROI   *PatRoiPtr = NULL;
	CPatternParam  *PatParamPtr = NULL;
	size_t          PatternRoiCount=0;
	IMAGE_PTR       PatImgPtr=NULL;
	IMAGE_PTR       PatImgPtr_Clone=NULL;
	IMAGE_PTR       PatMaskPtr=NULL;
	IMAGE_PTR       PatGrayPtr=NULL;
	IMAGE_PTR       PatImgPtrUse=NULL;
	IMAGE_PTR       PatRoiImgPtr=NULL;
	IMAGE_PTR       ImgRoiImgPtr=NULL;
	IMAGE_PTR       PatRoiImgPtrUse=NULL;
	IMAGE_SIZE      PatImgW=0, PatImgH=0, PatImgStep=0, PatBitCount=0, PatMaskImageStep=0;
	IMAGE_SIZE      PatImgWUse=0, PatImgHUse=0, PatImgStepUse=0, PatBitCountUse=0;
	IMAGE_SIZE      PatRoiWUse=0, PatRoiHUse=0, PatRoiStepUse=0, PatRoiBitCountUse=0;
	IMAGE_SIZE      PatRoiImgW=0, PatRoiImgH=0, PatRoiImgStep=0;
	IMAGE_SIZE      ImgRoiImgW=0, ImgRoiImgH=0, ImgRoiImgStep=0;	
	const size_t    PatternCount = GetAlgPatternCount();
	const bool      bPatternTestAll = GetAlgPatternTestAll();	
	JET_MATCH_LIB_TYPE MatchLibType = AOIDataCollect.GetMatchLibType();

	const int       nRoiX = RoiRect.left;
	const int       nRoiY = RoiRect.top;
	const int       nRoiW = (int)(RawImageW);
	const int       nRoiH = (int)(RawImageH);	
	const int       nRoiRectSize = (int)((nRoiW)*(nRoiH));
	const double    dRoiCpx = (double)((RoiRect.left+RoiRect.right)*0.5);
	const double    dRoiCpy = (double)((RoiRect.top+RoiRect.bottom)*0.5);
	TPOINT2D        WndRectCalValue = WndPtr->GetWndRectCalValue();
	//WndRectCalValue.x = 0;//-WndRectCalValue.x;
	//WndRectCalValue.y = 0;//WndRectCalValue.y;

	int   idx = 0;
	int    NResults = 0;		
	int    PolarityIdx=0;
	int    BestPatternIndex=-1;	
	double ResultCX=0.0, ResultCY=0.0;	
	double RoiOffsetX=0.0, RoiOffsetY=0.0;	
	double CadOffsetX=0.0, CadOffsetY=0.0, CadSkew=0.0, CadScaleX=100.0, CadScaleY=100.0;	
	double ResultX=0, ResultY=0, ResultA=0, ResultS=0, ResultSX=0, ResultSY=0, ResultMatch=0;	
	double BestResX=0, BestResY=0, BestResA=0, BestResS=0, BestResSX=0, BestResSY=0, BestResMatch=0;		
	double Reading=0.0, USL=0.0, LSL=0.0;		
	bool   ApplyScale=false;
	const bool      bRobustness = true;	
	const int       nMinReduceArea = GetAlgPatternMinReducedArea();
	const int       nFinalReduction = GetAlgPatternFinalReduction();
	const bool      bAdvancedLearning = GetAlgPatternAdvancedLearning();
	const bool      UseInterpolate = true;//CheckAlgPatternMatrchInterpolate();

#ifdef _DEBUG	
	bool         bSave = true;	
	CString      ComponentName;	
	CString      DebugFolder=GetAlgDebugFolder();
	if ( NULL != ComponentPtr )
	{	ComponentName = ComponentPtr->GetComponentFullName(); }		
#endif//_DEBUG

	TPATTERN_ROI PatternRoi;
	CJetMatch    Match;	//影像匹配
	
	if ( Match.SetMatchLibType(MatchLibType)==false )
	{	
		JetAPI::ClearUniFrame(RawUniFrame);		
		return false;	
	}
	
	double       ImgRoiScore=0;	
	double       CompareScore=0;
	double       ImgRoiOKCnt=0;		
	double       ImgRoiTotalCnt=0;
	double       ImgRoiPassRatio=0;
	const double ImgRoiScoreMax=128.0;
	const int    nGridX = cvParam.cvCellGridCnt;
	const int    nGridY = cvParam.cvCellGridCnt;
	const int    nCellExtSize=cvParam.cvCellExtSize;
	std::vector<TPATTERN_ROI> PatRoiList;	

	BestResS = -1.0;
	USL = cvParam.cvPassRatioUSL;
	LSL = cvParam.cvPassRatioLSL;
	for ( i=0; i<PatternCount; i++ )
	{
		PatParamPtr = CAlgParam::GetAlgPatternParamPtr(i, true);
		if ( NULL == PatParamPtr ) 
		{	continue;	}

		PolarityIdx=0;
		PatRoiList=PatParamPtr->GetPatRoiList(WndToward, PolarityIdx);		
		PatternRoiCount = PatRoiList.size();
		if ( 0 == PatternRoiCount )
		{	continue; }

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

		if ( BINARY_DISABLE != PatBinaryMode )//需要比較是否使用2值化影像
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
	
	#ifdef _DEBUG
		if ( true == bSave )
		{	
			str.Format(_T("%s\\%s_ModelWndAlgCharVerify#%d_PatternT#%d.PNG"), DebugFolder, ComponentName, WndIndex+1, i+1);
			ImageAPI.SaveImage(str, PatImgWUse, PatImgHUse, PatImgStepUse, PatBitCountUse, PatImgPtrUse, true);				
			str.Format(_T("%s\\%s_ModelWndAlgCharVerify#%d_ImageT#%d.PNG"), DebugFolder, ComponentName, WndIndex+1, i+1);
			ImageAPI.SaveImage(str, RoiImageW, RoiImageH, RoiImageStep, RoiBitCount, RoiImgPtrUse, true);
		}
	#endif//_DEBUG

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

		if ( true == GetAlgSkewEnabled())
		{
			double dSkew = GetAlgSkewCalcRange();
			float fSkewMax =  (float)(dSkew);
			float fSkewMin = -(float)(dSkew);
			Match.SetMinAngle(fSkewMin);
			Match.SetMaxAngle(fSkewMax);
			Match.SetUseAngle(true);				
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
		PatternRoiCount = PatRoiList.size();
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
			ResultMatch = ResultS;

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

			//const int PatImgWUse2 = (int)(PatImgWUse*0.5);
			//const int PatImgHUse2 = (int)(PatImgHUse*0.5);
			const int ImgRoiX=(int)(ResultX-(PatImgWUse*0.5)+0.5);
			const int ImgRoiY=(int)(ResultY-(PatImgHUse*0.5)+0.5);
			
			bool       RotateImageUsed=false;
			IMAGE_PTR  RoiImgPtrSrc=NULL;
			IMAGE_PTR  RoiMaskPtr_Bin = NULL;
			IMAGE_PTR  RoiImgPtrRotated=NULL;
			IMAGE_PTR  RoiMaskPtrUsed = RoiMaskPtr;
			IMAGE_PTR  RoiImgPtrUsed2 = RoiImgPtrUse;
			if ( CheckRotateImage(ResultA) == true )//確認影像要角度補正
			{	ImageAPI.RotateImage(ResultA, ResultX, ResultY, RoiImageW, RoiImageH, RoiImageStep, RoiBitCount, RoiImgPtrUse, RoiImageW, RoiImageH, RoiImageStep, RoiImgPtrRotated);	}
			if ( NULL == RoiImgPtrRotated )
			{	RoiImgPtrSrc = RoiImgPtrUse; }
			else
			{	
				RotateImageUsed = true;
				RoiImgPtrSrc = RoiImgPtrRotated; 
				if ( BINARY_DISABLE != PatBinaryMode )
				{	RoiMaskPtrUsed = RoiImgPtrRotated; }
			}	
			if ( BINARY_DISABLE != PatBinaryMode )
			{
				if ( BINARY_DYNAMIC_THRESHOLD == PatBinaryMode )//使用相對閥值, 需要重新計算
				{
					RECT    BinRect;					
					BinRect.left = (ImgRoiX);
					BinRect.top = (ImgRoiY);
					BinRect.right = (ImgRoiX)+(int)(PatImgWUse);
					BinRect.bottom = (ImgRoiY)+(int)(PatImgHUse);
					JetAPI::BoundaryRect(RoiImageW, RoiImageH, BinRect);
					if ( ExecAlgImageBinary_Rect(PatBinParam, BinRect, BinRect, RawImageW, RawImageH, RawImageStep, RawBitCount, RawImagePtr, NULL, NULL, nRoiAlign, RoiMaskPtr_Bin) == true )
					{	
						RoiMaskPtrUsed = RoiMaskPtr_Bin;
						if ( NULL != RoiImgPtrRotated )
						{
							JetMemory.free_func(RoiImgPtrRotated);						
							ImageAPI.RotateImage(ResultA, ResultX, ResultY, RoiImageW, RoiImageH, RoiImageStep, RoiBitCount, RoiMaskPtr_Bin, RoiImageW, RoiImageH, RoiImageStep, RoiImgPtrRotated);						
							if ( NULL != RoiImgPtrRotated )
							{	RoiMaskPtrUsed = RoiImgPtrRotated;	}						
						}
					}
				}
				RoiImgPtrUsed2 = RoiMaskPtrUsed;				

				PatRoiWUse = PatImgWUse;
				PatRoiHUse = PatImgHUse;
				PatRoiStepUse = PatImgStepUse;
				PatRoiBitCountUse = PatBitCountUse;
				PatRoiImgPtrUse = PatMaskPtr;				
			}
			else
			{
				RoiImgPtrUsed2 = RoiImgPtrUse;				
				
				PatRoiWUse = PatImgWUse;
				PatRoiHUse = PatImgHUse;
				PatRoiStepUse = PatImgStepUse;
				PatRoiBitCountUse = PatBitCountUse;
				PatRoiImgPtrUse = PatImgPtrUse;				
			}
			for ( j=0; j<PatternRoiCount; j++ )
			{
				PatRoiPtr = &(PatRoiList[j]);
				PatRoiPtr->RoiShift.x = 0;
				PatRoiPtr->RoiShift.y = 0;
				PatRoiRect = PatRoiPtr->RoiRect;
				PatRoiPtr->RoiRectRes=PatRoiPtr->RoiRect;
				if ( true == RotateImageUsed )
				{	
					TPOINT2D TmpCornerPt[4];
					RECT TmpRect=PatRoiPtr->RoiRect;
					const double CpX=(PatRoiWUse*0.5);
					const double CpY=(PatRoiHUse*0.5);
					JetAPI::RectToCornerPt(TmpRect, TmpCornerPt);
					JetAPI::RotateCornerPos(ResultA, CpX, CpY, TmpCornerPt);
					JetAPI::CornerPtToRect(TmpCornerPt, TmpRect);
					PatRoiPtr->RoiRectRes = TmpRect;					
				}

				JetAPI::BoundaryRect(PatRect, PatRoiRect);
				PatRoiImgW = PatRoiRect.right-PatRoiRect.left;
				PatRoiImgH = PatRoiRect.bottom-PatRoiRect.top;
				PatRoiImgStep = JetAPI::GetBMPImagePixelsPerLine(PatRoiImgW, PatRoiBitCountUse, nPatAlign);
				if ( ImageAPI.ExtractRoiImage(PatRoiWUse, PatRoiHUse, PatRoiStepUse, PatRoiBitCountUse, PatRoiImgPtrUse, PatRoiRect, PatRoiImgStep, PatRoiImgPtr, false) == false )
				{	continue;	}

				int RoiShiftX=0, RoiShiftY=0;
				if ( nCellExtSize > 0 )//子框重新定位
				{	
					float TmpScore=0.0f, TmpX=0.0f, TmpY=0.0f, TmpA=0.0f;
					const float RoiCpX=(PatRoiRect.right+PatRoiRect.left)*0.5f+ImgRoiX;
					const float RoiCpY=(PatRoiRect.bottom+PatRoiRect.top)*0.5f+ImgRoiY;
					ImgRoiRect.left   = PatRoiRect.left + ImgRoiX - nCellExtSize;
					ImgRoiRect.top    = PatRoiRect.top + ImgRoiY - nCellExtSize;
					ImgRoiRect.right  = PatRoiRect.right + ImgRoiX + nCellExtSize;
					ImgRoiRect.bottom = PatRoiRect.bottom + ImgRoiY + nCellExtSize;
					JetAPI::BoundaryRect(RoiImageW, RoiImageH, ImgRoiRect);
					ImgRoiImgW = ImgRoiRect.right-ImgRoiRect.left;
					ImgRoiImgH = ImgRoiRect.bottom-ImgRoiRect.top;
					ImgRoiImgStep = JetAPI::GetBMPImagePixelsPerLine(ImgRoiImgW, (unsigned int)RoiBitCount, nPatAlign);
					if ( ImageAPI.ExtractRoiImage(RoiImageW, RoiImageH, RoiImageStep, RoiBitCount, RoiImgPtrUsed2, ImgRoiRect, ImgRoiImgStep, ImgRoiImgPtr, false) == true )
					{	
					#ifdef _DEBUG
						if ( true == bSave )
						{					
							str.Format(_T("%s\\%s_ModelWndAlgCharVerify#%d_PatRoiT#%d#%d_Ext.PNG"), DebugFolder, ComponentName, WndIndex+1, i+1, j+1);
							ImageAPI.SaveImage(str, PatRoiImgW, PatRoiImgH, PatRoiImgStep, PatRoiBitCountUse, PatRoiImgPtr, true);					
							str.Format(_T("%s\\%s_ModelWndAlgCharVerify#%d_ImgRoiT#%d#%d_Ext.PNG"), DebugFolder, ComponentName, WndIndex+1, i+1, j+1);
							ImageAPI.SaveImage(str, ImgRoiImgW, ImgRoiImgH, ImgRoiImgStep, PatRoiBitCountUse, ImgRoiImgPtr, true);
						}
					#endif//_DEBUG
						if ( ImageAPI.MatchImage(PatRoiImgW, PatRoiImgH, PatRoiImgStep, PatRoiBitCountUse, PatRoiImgPtr, ImgRoiImgW, ImgRoiImgH, ImgRoiImgStep, RoiBitCount, ImgRoiImgPtr, TmpScore, TmpX, TmpY, TmpA) == true )
						{
							const float ResCpX=ImgRoiRect.left+TmpX;
							const float ResCpY=ImgRoiRect.top +TmpY;
							const float ShiftX=ResCpX-RoiCpX;
							const float ShiftY=ResCpY-RoiCpY;
							if ( ShiftX > 0.0f )
							{	RoiShiftX = (int)(ShiftX+0.5f); }
							else
							{	RoiShiftX = (int)(ShiftX-0.5f); }
							if ( ShiftY > 0.0f )
							{	RoiShiftY = (int)(ShiftY+0.5f); }
							else
							{	RoiShiftY = (int)(ShiftY-0.5f); }							
						}
						JetMemory.free_func(ImgRoiImgPtr);
					}
				}

				PatRoiPtr->RoiShift.x = RoiShiftX;
				PatRoiPtr->RoiShift.y = RoiShiftY;
				ImgRoiRect.left   = PatRoiRect.left + ImgRoiX + RoiShiftX;
				ImgRoiRect.top    = PatRoiRect.top + ImgRoiY + RoiShiftY;
				ImgRoiRect.right  = PatRoiRect.right + ImgRoiX + RoiShiftX;
				ImgRoiRect.bottom = PatRoiRect.bottom + ImgRoiY + RoiShiftY;
				JetAPI::BoundaryRect(RoiImageW, RoiImageH, ImgRoiRect);
				ImgRoiImgW = ImgRoiRect.right-ImgRoiRect.left;
				ImgRoiImgH = ImgRoiRect.bottom-ImgRoiRect.top;
				ImgRoiImgStep = JetAPI::GetBMPImagePixelsPerLine(ImgRoiImgW, (unsigned int)RoiBitCount, nPatAlign);
				if ( ImageAPI.ExtractRoiImage(RoiImageW, RoiImageH, RoiImageStep, RoiBitCount, RoiImgPtrUsed2, ImgRoiRect, ImgRoiImgStep, ImgRoiImgPtr, false) == false )
				{	
					JetMemory.free_func(PatRoiImgPtr);
					continue;	
				}

			#ifdef _DEBUG
				if ( true == bSave )
				{					
					str.Format(_T("%s\\%s_ModelWndAlgCharVerify#%d_PatRoiT#%d#%d.PNG"), DebugFolder, ComponentName, WndIndex+1, i+1, j+1);
					ImageAPI.SaveImage(str, PatRoiImgW, PatRoiImgH, PatRoiImgStep, PatRoiBitCountUse, PatRoiImgPtr, true);					
					str.Format(_T("%s\\%s_ModelWndAlgCharVerify#%d_ImgRoiT#%d#%d.PNG"), DebugFolder, ComponentName, WndIndex+1, i+1, j+1);
					ImageAPI.SaveImage(str, ImgRoiImgW, ImgRoiImgH, ImgRoiImgStep, PatRoiBitCountUse, ImgRoiImgPtr, true);
				}
			#endif//_DEBUG

				if ( ImageAPI.Compare2Image(ImgRoiImgW, ImgRoiImgH, ImgRoiImgStep, PatRoiBitCountUse, PatRoiImgPtr, ImgRoiImgPtr, nGridX, nGridY, CompareScore) == false )
				{	
					JetMemory.free_func(PatRoiImgPtr);
					JetMemory.free_func(ImgRoiImgPtr);
					continue;
				}
				JetMemory.free_func(PatRoiImgPtr);
				JetMemory.free_func(ImgRoiImgPtr);
				PatRoiPtr->ResultScore = CompareScore;				
			}
			JetMemory.free_func(RoiMaskPtr_Bin);
			JetMemory.free_func(RoiImgPtrRotated);
			//判斷檢測結果
			ImgRoiOKCnt = 0.0;
			ImgRoiTotalCnt = 0.0;
			for ( j=0; j<PatternRoiCount; j++ )
			{
				PatRoiPtr = &(PatRoiList[j]);
				ImgRoiTotalCnt  = ImgRoiTotalCnt + 1.0;
				if ( PatRoiPtr->ResultScore<CellScoreMin || PatRoiPtr->ResultScore>CellScoreMax )
				{
					PatRoiPtr->ResultID = RESULT_ID_NG;
					continue;;	
				}
				PatRoiPtr->ResultID = RESULT_ID_OK;
				ImgRoiOKCnt = ImgRoiOKCnt+1.0;				
			}
			if ( ImgRoiTotalCnt > 0 ) 
			{	ImgRoiPassRatio = 100.0*ImgRoiOKCnt/ImgRoiTotalCnt; }
			else
			{	ImgRoiPassRatio = 0.0; }
			switch ( WndToward )
			{
			case BOX_TOWARD_UP:    PatParamPtr->SetPatRoiListT(PolarityIdx, PatRoiList);	break;
			case BOX_TOWARD_LEFT:  PatParamPtr->SetPatRoiListL(PolarityIdx, PatRoiList);	break;
			case BOX_TOWARD_DOWN:  PatParamPtr->SetPatRoiListB(PolarityIdx, PatRoiList);	break;
			case BOX_TOWARD_RIGHT: PatParamPtr->SetPatRoiListR(PolarityIdx, PatRoiList);	break;
			default:
				break;
			}			
			ResultS = ImgRoiPassRatio;
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
				BestResMatch = ResultMatch;
				BestPatternIndex = (int)(i);				
			}
			if ( ResultS>=LSL && ResultS<=USL )
			{
				PatParamPtr->SetReultID(PolarityIdx, RESULT_ID_OK);
				PatParamPtr->CheckResultIndex();
				JetMemory.free_func(PatImgPtr);
				JetMemory.free_func(PatMaskPtr);
				JetMemory.free_func(PatGrayPtr);
				JetMemory.free_func(RoiGrayPtr);
				JetMemory.free_func(RoiMaskPtr);
				JetMemory.free_func(PatImgPtr_Clone);
				if ( false == bPatternTestAll ) 
				{	break;	}
				else
				{	continue; }
			}
			/*
			else
			{
				if ( ResultMatch > BestResMatch )
				{
					BestResS = ResultS;
					BestResX = ResultX;
					BestResY = ResultY;
					BestResA = ResultA;
					BestResSX = ResultSX;
					BestResSY = ResultSY;
					BestResMatch = ResultMatch;
					BestPatternIndex = (int)(i);					
				}
			}*/		
		}

		if ( 2 == PatternPolarity )
		{
			PolarityIdx = 1;
			IMAGE_PTR  PatImgPtr2 = NULL;			
			IMAGE_SIZE PatImgW2=0, PatImgH2=0, PatImgStep2=0;			
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
				str.Format(_T("%s\\%s_ModelWndAlgCharVerify#%d_PatternB#%d.PNG"), DebugFolder, ComponentName, WndIndex+1, i+1);
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
				ResultMatch = ResultS;

				ResultCX = ResultX+nRoiX;
				ResultCY = ResultY+nRoiY;
				RoiOffsetX = ResultCX-dRoiCpx;
				RoiOffsetY = dRoiCpy-ResultCY;
				CadSkew = -ResultA;
				CadOffsetX = RoiOffsetX/ImageScale.x;
				CadOffsetY = RoiOffsetY/ImageScale.y;
				CadOffsetX = CadOffsetX + WndRectCalValue.x;
				CadOffsetY = CadOffsetY + WndRectCalValue.y;

				PatParamPtr->SetPatResultW(PolarityIdx, PatImgW2);
				PatParamPtr->SetPatResultH(PolarityIdx, PatImgH2);
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

				const int ImgRoiX=(int)(ResultX-(PatImgW2*0.5)+0.5);
				const int ImgRoiY=(int)(ResultY-(PatImgH2*0.5)+0.5);				
				
				bool       RotateImageUsed=false;
				IMAGE_PTR  RoiImgPtrSrc=NULL;
				IMAGE_PTR  RoiImgPtrRotated=NULL;
				IMAGE_PTR  RoiMaskPtr_Bin = NULL;
				IMAGE_PTR  RoiMaskPtrUsed = RoiMaskPtr;
				IMAGE_PTR  RoiImgPtrUsed2 = RoiImgPtrUse;
				if ( CheckRotateImage(ResultA) == true )//確認影像要角度補正
				{	ImageAPI.RotateImage(ResultA, ResultX, ResultY, RoiImageW, RoiImageH, RoiImageStep, RoiBitCount, RoiImgPtrUse, RoiImageW, RoiImageH, RoiImageStep, RoiImgPtrRotated);	}
				if ( NULL == RoiImgPtrRotated )
				{	RoiImgPtrSrc = RoiImgPtrUse; }
				else
				{	
					RotateImageUsed = true;
					RoiImgPtrSrc = RoiImgPtrRotated; 
					if ( BINARY_DISABLE != PatBinaryMode )
					{	RoiMaskPtrUsed = RoiImgPtrRotated; }
				}
				if ( BINARY_DISABLE != PatBinaryMode )
				{
					if ( BINARY_DYNAMIC_THRESHOLD == PatBinaryMode )//使用相對閥值, 需要重新計算
					{
						RECT    BinRect;						
						BinRect.left = (ImgRoiX);
						BinRect.top = (ImgRoiY);
						BinRect.right = (ImgRoiX)+(int)(PatImgWUse);
						BinRect.bottom = (ImgRoiY)+(int)(PatImgHUse);
						JetAPI::BoundaryRect(RoiImageW, RoiImageH, BinRect);
						if ( ExecAlgImageBinary_Rect(PatBinParam, BinRect, BinRect, RawImageW, RawImageH, RawImageStep, RawBitCount, RawImagePtr, NULL, NULL, nRoiAlign, RoiMaskPtr_Bin) == true )
						{	
							RoiMaskPtrUsed = RoiMaskPtr_Bin;
							if ( NULL != RoiImgPtrRotated )
							{
								JetMemory.free_func(RoiImgPtrRotated);
								ImageAPI.RotateImage(ResultA, ResultX, ResultY, RoiImageW, RoiImageH, RoiImageStep, RoiBitCount, RoiMaskPtr_Bin, RoiImageW, RoiImageH, RoiImageStep, RoiImgPtrRotated);
								if ( NULL != RoiImgPtrRotated )
								{	RoiMaskPtrUsed = RoiImgPtrRotated;	}
							}
						}
					}
					RoiImgPtrUsed2 = RoiMaskPtrUsed;					

					PatRoiWUse = PatImgW2;
					PatRoiHUse = PatImgH2;
					PatRoiStepUse = PatImgStep2;
					PatRoiBitCountUse = PatBitCountUse;
					PatRoiImgPtrUse = PatImgPtr2;
				}
				else
				{
					RoiImgPtrUsed2 = RoiImgPtrSrc;					

					PatRoiWUse = PatImgW2;
					PatRoiHUse = PatImgH2;
					PatRoiStepUse = PatImgStep2;
					PatRoiBitCountUse = PatBitCountUse;
					PatRoiImgPtrUse = PatImgPtr2;
				}				
				for ( j=0; j<PatternRoiCount; j++ )
				{
					PatRoiPtr = &(PatRoiList[j]);
					PatRoiPtr->RoiShift.x = 0;
					PatRoiPtr->RoiShift.y = 0;
					PatRoiRect = PatRoiPtr->RoiRect;
					PatRoiPtr->RoiRectRes=PatRoiPtr->RoiRect;
					if ( true == RotateImageUsed )
					{						
						TPOINT2D TmpCornerPt[4];
						RECT TmpRect=PatRoiPtr->RoiRect;
						const double CpX=(PatRoiWUse*0.5);
						const double CpY=(PatRoiHUse*0.5);
						JetAPI::RotateRect(180, PatRoiWUse, PatRoiHUse, TmpRect);
						JetAPI::RectToCornerPt(TmpRect, TmpCornerPt);
						JetAPI::RotateCornerPos(ResultA, CpX, CpY, TmpCornerPt);
						JetAPI::CornerPtToRect(TmpCornerPt, TmpRect);
						JetAPI::RotateRect(180, PatRoiWUse, PatRoiHUse, TmpRect);
						PatRoiPtr->RoiRectRes = TmpRect;						
					}

					//因為旋轉180, 所以位置要轉回去
					PatRoiRect2.left   = PatRoiWUse-PatRoiRect.right;
					PatRoiRect2.top    = PatRoiHUse-PatRoiRect.bottom;
					PatRoiRect2.right  = PatRoiWUse-PatRoiRect.left;
					PatRoiRect2.bottom = PatRoiHUse-PatRoiRect.top;

					JetAPI::BoundaryRect(PatRect, PatRoiRect2);
					PatRoiImgW = PatRoiRect2.right-PatRoiRect2.left;
					PatRoiImgH = PatRoiRect2.bottom-PatRoiRect2.top;
					PatRoiImgStep = JetAPI::GetBMPImagePixelsPerLine(PatRoiImgW, PatRoiBitCountUse, nPatAlign);
					if ( ImageAPI.ExtractRoiImage(PatRoiWUse, PatRoiHUse, PatRoiStepUse, PatRoiBitCountUse, PatRoiImgPtrUse, PatRoiRect2, PatRoiImgStep, PatRoiImgPtr, false) == false )
					{	continue;	}
					
					int RoiShiftX=0, RoiShiftY=0;
					if ( nCellExtSize > 0 )//子框重新定位
					{	
						float TmpScore=0.0f, TmpX=0.0f, TmpY=0.0f, TmpA=0.0f;
						const float RoiCpX=(PatRoiRect2.right+PatRoiRect2.left)*0.5f+ImgRoiX;
						const float RoiCpY=(PatRoiRect2.bottom+PatRoiRect2.top)*0.5f+ImgRoiY;
						ImgRoiRect.left   = PatRoiRect2.left + ImgRoiX - nCellExtSize;
						ImgRoiRect.top    = PatRoiRect2.top + ImgRoiY - nCellExtSize;
						ImgRoiRect.right  = PatRoiRect2.right + ImgRoiX + nCellExtSize;
						ImgRoiRect.bottom = PatRoiRect2.bottom + ImgRoiY + nCellExtSize;
						JetAPI::BoundaryRect(RoiImageW, RoiImageH, ImgRoiRect);
						ImgRoiImgW = ImgRoiRect.right-ImgRoiRect.left;
						ImgRoiImgH = ImgRoiRect.bottom-ImgRoiRect.top;
						ImgRoiImgStep = JetAPI::GetBMPImagePixelsPerLine(ImgRoiImgW, (unsigned int)RoiBitCount, nPatAlign);
						if ( ImageAPI.ExtractRoiImage(RoiImageW, RoiImageH, RoiImageStep, RoiBitCount, RoiImgPtrUsed2, ImgRoiRect, ImgRoiImgStep, ImgRoiImgPtr, false) == true )
						{	
						#ifdef _DEBUG
							if ( true == bSave )
							{					
								str.Format(_T("%s\\%s_ModelWndAlgCharVerify#%d_PatRoiB#%d#%d_Ext.PNG"), DebugFolder, ComponentName, WndIndex+1, i+1, j+1);
								ImageAPI.SaveImage(str, PatRoiImgW, PatRoiImgH, PatRoiImgStep, PatRoiBitCountUse, PatRoiImgPtr, true);					
								str.Format(_T("%s\\%s_ModelWndAlgCharVerify#%d_ImgRoiB#%d#%d_Ext.PNG"), DebugFolder, ComponentName, WndIndex+1, i+1, j+1);
								ImageAPI.SaveImage(str, ImgRoiImgW, ImgRoiImgH, ImgRoiImgStep, PatRoiBitCountUse, ImgRoiImgPtr, true);
							}
						#endif//_DEBUG
							if ( ImageAPI.MatchImage(PatRoiImgW, PatRoiImgH, PatRoiImgStep, PatRoiBitCountUse, PatRoiImgPtr, ImgRoiImgW, ImgRoiImgH, ImgRoiImgStep, RoiBitCount, ImgRoiImgPtr, TmpScore, TmpX, TmpY, TmpA) == true )
							{
								const float ResCpX=ImgRoiRect.left+TmpX;
								const float ResCpY=ImgRoiRect.top +TmpY;
								const float ShiftX=ResCpX-RoiCpX;
								const float ShiftY=ResCpY-RoiCpY;
								if ( ShiftX > 0.0f )
								{	RoiShiftX = (int)(ShiftX+0.5f); }
								else
								{	RoiShiftX = (int)(ShiftX-0.5f); }
								if ( ShiftY > 0.0f )
								{	RoiShiftY = (int)(ShiftY+0.5f); }
								else
								{	RoiShiftY = (int)(ShiftY-0.5f); }
							}
							JetMemory.free_func(ImgRoiImgPtr);
						}
					}

					PatRoiPtr->RoiShift.x = RoiShiftX;
					PatRoiPtr->RoiShift.y = RoiShiftY;
					ImgRoiRect.left   = PatRoiRect2.left + ImgRoiX + RoiShiftX;
					ImgRoiRect.top    = PatRoiRect2.top + ImgRoiY + RoiShiftY;
					ImgRoiRect.right  = PatRoiRect2.right + ImgRoiX + RoiShiftX;
					ImgRoiRect.bottom = PatRoiRect2.bottom + ImgRoiY + RoiShiftY;
					JetAPI::BoundaryRect(RoiImageW, RoiImageH, ImgRoiRect);
					ImgRoiImgW = ImgRoiRect.right-ImgRoiRect.left;
					ImgRoiImgH = ImgRoiRect.bottom-ImgRoiRect.top;
					ImgRoiImgStep = JetAPI::GetBMPImagePixelsPerLine(ImgRoiImgW, RoiBitCount, nPatAlign);
					if ( ImageAPI.ExtractRoiImage(RoiImageW, RoiImageH, RoiImageStep, RoiBitCount, RoiImgPtrUsed2, ImgRoiRect, ImgRoiImgStep, ImgRoiImgPtr, false) == false )
					{	
						JetMemory.free_func(PatRoiImgPtr);
						continue;	
					}

				#ifdef _DEBUG
					if ( true == bSave )
					{	
						str.Format(_T("%s\\%s_ModelWndAlgCharVerify#%d_PatRoiB#%d#%d.PNG"), DebugFolder, ComponentName, WndIndex+1, i+1, j+1);
						ImageAPI.SaveImage(str, PatRoiImgW, PatRoiImgH, PatRoiImgStep, PatRoiBitCountUse, PatRoiImgPtr, true);						
						str.Format(_T("%s\\%s_ModelWndAlgCharVerify#%d_ImgRoiB#%d#%d.PNG"), DebugFolder, ComponentName, WndIndex+1, i+1, j+1);
						ImageAPI.SaveImage(str, ImgRoiImgW, ImgRoiImgH, ImgRoiImgStep, PatRoiBitCountUse, ImgRoiImgPtr, true);
					}
				#endif//_DEBUG

					if ( ImageAPI.Compare2Image(ImgRoiImgW, ImgRoiImgH, ImgRoiImgStep, PatRoiBitCountUse, PatRoiImgPtr, ImgRoiImgPtr, nGridX, nGridY, CompareScore) == false )
					{	
						JetMemory.free_func(PatRoiImgPtr);
						JetMemory.free_func(ImgRoiImgPtr);
						continue;
					}
					JetMemory.free_func(PatRoiImgPtr);
					JetMemory.free_func(ImgRoiImgPtr);
					PatRoiPtr->ResultScore = CompareScore;
				}
				JetMemory.free_func(RoiMaskPtr_Bin);
				JetMemory.free_func(RoiImgPtrRotated);
				//判斷檢測結果
				ImgRoiOKCnt = 0.0;
				ImgRoiTotalCnt = 0.0;
				for ( j=0; j<PatternRoiCount; j++ )
				{
					PatRoiPtr = &(PatRoiList[j]);
					ImgRoiTotalCnt  = ImgRoiTotalCnt + 1.0;
					if ( PatRoiPtr->ResultScore<CellScoreMin || PatRoiPtr->ResultScore>CellScoreMax )
					{
						PatRoiPtr->ResultID = RESULT_ID_NG;
						continue;	
					}
					PatRoiPtr->ResultID = RESULT_ID_OK;
					ImgRoiOKCnt = ImgRoiOKCnt+1.0;				
				}
				if ( ImgRoiTotalCnt > 0 ) 
				{	ImgRoiPassRatio = 100.0*ImgRoiOKCnt/ImgRoiTotalCnt; }
				else
				{	ImgRoiPassRatio = 0.0; }

				switch ( WndToward )
				{
				case BOX_TOWARD_UP:    PatParamPtr->SetPatRoiListT(PolarityIdx, PatRoiList);	break;
				case BOX_TOWARD_LEFT:  PatParamPtr->SetPatRoiListL(PolarityIdx, PatRoiList);	break;
				case BOX_TOWARD_DOWN:  PatParamPtr->SetPatRoiListB(PolarityIdx, PatRoiList);	break;
				case BOX_TOWARD_RIGHT: PatParamPtr->SetPatRoiListR(PolarityIdx, PatRoiList);	break;
				default:
					break;
				}
				ResultS = ImgRoiPassRatio;
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
					BestResMatch = ResultMatch;
					BestPatternIndex = (int)(i);					
				}
				if ( ResultS>=LSL && ResultS<=USL )
				{
					PatParamPtr->SetReultID(PolarityIdx, RESULT_ID_OK);
					PatParamPtr->CheckResultIndex();

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
				/*
				else
				{
					if ( ResultMatch > BestResMatch )
					{
						BestResS = ResultS;
						BestResX = ResultX;
						BestResY = ResultY;
						BestResA = ResultA;
						BestResSX = ResultSX;
						BestResSY = ResultSY;
						BestResMatch = ResultMatch;
						BestPatternIndex = (int)(i);						
					}
				}*/
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

	CString PatText;
	bool Matched = false;
	PatParamPtr = GetAlgPatternParamPtr(BestPatternIndex, true);
	if ( NULL != PatParamPtr ) 
	{
		Matched = true;
		PatText = PatParamPtr->GetPatText();
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

		TPOINT2D    RgnCp;
		TPOINT2D    ImageCp;
		CAOIBox     ResBox;
		TREGION4D   ResBoxRgn;

		RgnCp.x = ModelRgn.GetCpX();
		RgnCp.y = ModelRgn.GetCpY();
		ImageCp.x = FrameImageW;
		ImageCp.y = FrameImageH;
		ImageCp.x = ImageCp.x*0.5;
		ImageCp.y = ImageCp.y*0.5;
		PatImgWUse = PatParamPtr->GetPatResultW(PolarityIdx);
		PatImgHUse = PatParamPtr->GetPatResultH(PolarityIdx);			

		const int ImgRoiX=(int)(ResultX-(PatImgWUse*0.5)-0.5);
		const int ImgRoiY=(int)(ResultY-(PatImgHUse*0.5)-0.5);				
		//WndRectCalValue.x = -WndRectCalValue.x;
		//WndRectCalValue.y = -WndRectCalValue.y;
		PatRoiList=PatParamPtr->GetPatRoiList(WndToward, PolarityIdx);		
		PatternRoiCount = PatRoiList.size();
		for ( j=0; j<PatternRoiCount; j++ )
		{				
			PatRoiPtr = &(PatRoiList[j]);
			const int ShiftX = PatRoiPtr->RoiShift.x;
			const int ShiftY = PatRoiPtr->RoiShift.y;
			if ( 1 == PatParamPtr->GetResultPolarityIdx() )
			{
				//因為旋轉180, 所以位置要轉回去	
				PatRoiRect2 = PatRoiPtr->RoiRect;
				PatRoiRect2 = PatRoiPtr->RoiRectRes;
				PatRoiRect.left   = PatImgWUse-PatRoiRect2.right;
				PatRoiRect.top    = PatImgHUse-PatRoiRect2.bottom;
				PatRoiRect.right  = PatImgWUse-PatRoiRect2.left;
				PatRoiRect.bottom = PatImgHUse-PatRoiRect2.top;
			}
			else
			{	
				PatRoiRect = PatRoiPtr->RoiRect;
				PatRoiRect = PatRoiPtr->RoiRectRes;
			}
			PatRoiRect.left   += ResultSX+ImgRoiX+nRoiX+ShiftX;
			PatRoiRect.top    += ResultSY+ImgRoiY+nRoiY+ShiftY;
			PatRoiRect.right  += ResultSX+ImgRoiX+nRoiX+ShiftX;
			PatRoiRect.bottom += ResultSY+ImgRoiY+nRoiY+ShiftY;
			CAOIModel::CalcModelBoxRectRegion(PatRoiRect, FrameImageW, FrameImageH, RgnCp, ImageScale, ImageCp, ResBoxRgn);
			ResBox.SetBoxRegion(ResBoxRgn);				
			if ( PatRoiPtr->ResultScore<CellScoreMin || PatRoiPtr->ResultScore>CellScoreMax )
			{	ResBox.SetBoxResultID(RESULT_ID_NG);	}
			else
			{	ResBox.SetBoxResultID(RESULT_ID_OK);	}

			str.Format(_T("%.0f%%"), PatRoiPtr->ResultScore);
			ResBox.SetBoxResultText(str);
			ResBox.SetBoxResultTextVisibled(true);
			ResBox.SetBoxResultValue(PatRoiPtr->ResultScore);
			WndPtr->AddWndResultBox(ResBox);
		}		
	}
	if ( BestPatternIndex<0 && PatternCount>0 ) 
	{	BestPatternIndex = 0; }
	
	Reading = ResultS;
	USL = cvParam.cvPassRatioUSL;
	LSL = cvParam.cvPassRatioLSL;
	cvParam.cvPassRatioReading = Reading;
	SetAlgPatternSimilarityReading(Reading);		
	SetAlgPatternResultIndex(BestPatternIndex);		

	CString strResult;	
	SetAlgResultReading1(Reading);	
	SetAlgResultText(_T("OK"));
	SetAlgResultID(RESULT_ID_OK);
	if ( Reading<LSL || Reading>USL )
	{
		if ( PatText.GetLength() == 0 )
		{	strResult.Format(_T("NG:%.0f / %.0f"), Reading, LSL); }
		else
		{	strResult.Format(_T("NG:%.0f/%.0f [%s]"), Reading, LSL, PatText); }
		SetAlgResultID(RESULT_ID_NG);
		SetAlgResultText(strResult);
	}
	
	double CadSkew_Self = 0;
	double CadOffsetX_Self = 0;
	double CadOffsetY_Self = 0;
	double CadSkew_Others = 0;
	double CadOffsetX_Others = 0;
	double CadOffsetY_Others = 0;	
	const bool bChkDefect=false;
	const bool ApplySkewAngle = true;
	const bool AlgScaleEnabled = GetAlgScaleEnabled();
	
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
	//套用至後面的檢測框
	if ( true == Matched )
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
		BoxPtr->SkewBoxAngle(CadSkew_Self);
		BoxPtr->MoveBoxRes(CadOffsetX_Self, CadOffsetY_Self);
	}	
	return true;
}
//-------------------------------------------------------------------------------------//