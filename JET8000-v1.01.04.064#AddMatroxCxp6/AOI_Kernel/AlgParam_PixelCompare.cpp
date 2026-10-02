// AlgParam_PixelCompare.cpp: implementation of the CAlgParam class.
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
bool CAlgParam::BuildPixelCompareParam(const TALG_PARAM_PIXEL_COMPARE &pcParam, const TPOINT2D &Scale, TPixelCompareParam &tpcParam)
{
	const double ImageScaleXY=(Scale.x+Scale.y)*0.5;
	tpcParam.nImgBlurSize = pcParam.pcImgBlurSize;

	tpcParam.nPatOpenSize = 0;
	tpcParam.nPatCloseSize = 0;	
	tpcParam.nPatDarkLevel = pcParam.pcPatDarkLevel;
	tpcParam.nPatLightLevel = pcParam.pcPatLightLevel;	
	tpcParam.nPatErodeSize = pcParam.pcPatErodeSize;
	tpcParam.bPatPureColor = pcParam.pcPatPureColor;//沒有比較好, 因為邊緣會變暗, 導致誤判變多

	tpcParam.nCmpDarkLevel = 0;
	tpcParam.fCmpDarkGain = pcParam.pcCmpDarkGain;
	tpcParam.fCmpLightGain = pcParam.pcCmpLightGain;	
	tpcParam.nCmpTolerance = pcParam.pcCmpTolerance;
	tpcParam.nCmpGaussianSize = pcParam.pcCmpGaussianSize;
	tpcParam.nCmpOpenSize = pcParam.pcCmpOpenSize;
	tpcParam.nCmpCloseSize = pcParam.pcCmpCloseSize;	

	tpcParam.nBlobMinW = 0;
	tpcParam.nBlobMinH = 0;	
	tpcParam.nBlobMinArea = 0;
	tpcParam.nBlobMinD = (int)((pcParam.pcBlobMinSizeD*ImageScaleXY)+0.5);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_PixelCompare(const TALG_PARAM_PIXEL_COMPARE &Param)
{
	if ( CheckOK_PixelCompareUSL(Param) == false )
	{	return false; }
	if ( CheckOK_PixelCompareLSL(Param) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_PixelCompareUSL(const TALG_PARAM_PIXEL_COMPARE &Param)
{
	if ( Param.pcBlobCountNum > Param.pcBlobCountUSL )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_PixelCompareLSL(const TALG_PARAM_PIXEL_COMPARE &Param)
{
	if ( Param.pcBlobCountNum < Param.pcBlobCountLSL )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::WriteAlgParamFile_PixelCompare(const TALG_PARAM_PIXEL_COMPARE &pcParam, CAOIFileIO &FileIO)//儲存長度量測參數
{
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_PIEXEL_COMPARE_START, 0) == false ) { return false; }
	
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_PIEXEL_COMPARE_CELL_EXT_SIZE, pcParam.pcCellExtSize) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_PIEXEL_COMPARE_IMG_BLUR_SIZE, pcParam.pcImgBlurSize) == false ) { return false; }

	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_PIEXEL_COMPARE_PAT_DARK_LEVEL, pcParam.pcPatDarkLevel) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_PIEXEL_COMPARE_PAT_LIGHT_LEVEL, pcParam.pcPatLightLevel) == false ) { return false; }	
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_PIEXEL_COMPARE_PAT_ERODE_SIZE, pcParam.pcPatErodeSize) == false ) { return false; }	
	if ( FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_PIEXEL_COMPARE_PAT_PURE_COLOR, pcParam.pcPatPureColor) == false ) { return false; }		

	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_PIEXEL_COMPARE_CMP_DARK_GAIN, pcParam.pcCmpDarkGain) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_PIEXEL_COMPARE_CMP_LIGHT_GAIN, pcParam.pcCmpLightGain) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_PIEXEL_COMPARE_CMP_TOLERANCE, pcParam.pcCmpTolerance) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_PIEXEL_COMPARE_CMP_GAUSSIAN_SIZE, pcParam.pcCmpGaussianSize) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_PIEXEL_COMPARE_CMP_OPEN_SIZE, pcParam.pcCmpOpenSize) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_PIEXEL_COMPARE_CMP_CLOSE_SIZE, pcParam.pcCmpCloseSize) == false ) { return false; }	

	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_PIEXEL_COMPARE_BLOB_MIN_SIZE_D, pcParam.pcBlobMinSizeD) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_PIEXEL_COMPARE_BLOB_COUNT_USL, pcParam.pcBlobCountUSL) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_PIEXEL_COMPARE_BLOB_COUNT_LSL, pcParam.pcBlobCountLSL) == false ) { return false; }

	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_PIEXEL_COMPARE_END, 0) == false ) { return false; }		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ReadAlgParamFile_PixelCompare(TALG_PARAM_PIXEL_COMPARE &pcParam, CAOIFileIO &FileIO)//載入像素比較參數
{
	int       index = 0;
	int       nValue = 0;	
	while ( FileIO.CheckFileEnd()==false )
	{ 		
		if ( FileIO.LoadChunk(index) == false )		
		{	continue; }
		switch ( index )
		{
		case FILE_IO_ALG_PARAM_PIEXEL_COMPARE_END://像素比較參數-終點
			return true;
			break;		
		case FILE_IO_ALG_PARAM_PIEXEL_COMPARE_CELL_EXT_SIZE://單元外擴尺寸
			pcParam.pcCellExtSize = FileIO.GetData_INT();
			break;
		case FILE_IO_ALG_PARAM_PIEXEL_COMPARE_IMG_BLUR_SIZE://影像模糊尺寸
			pcParam.pcImgBlurSize = FileIO.GetData_INT();			
			break;

		case FILE_IO_ALG_PARAM_PIEXEL_COMPARE_PAT_DARK_LEVEL://樣板過暗不處理			
			pcParam.pcPatDarkLevel = FileIO.GetData_INT();
			break;
		case FILE_IO_ALG_PARAM_PIEXEL_COMPARE_PAT_LIGHT_LEVEL://樣板過亮不處理
			pcParam.pcPatLightLevel = FileIO.GetData_INT();
			break;		
		case FILE_IO_ALG_PARAM_PIEXEL_COMPARE_PAT_ERODE_SIZE://樣板侵蝕尺寸
			pcParam.pcPatErodeSize = FileIO.GetData_INT();
			break;
		case FILE_IO_ALG_PARAM_PIEXEL_COMPARE_PAT_PURE_COLOR://樣板純色模式
			pcParam.pcPatPureColor = FileIO.GetData_BOL();
			break;	

		case FILE_IO_ALG_PARAM_PIEXEL_COMPARE_CMP_DARK_GAIN://比較的暗部增益			
			pcParam.pcCmpDarkGain = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_PIEXEL_COMPARE_CMP_LIGHT_GAIN://比較的亮部增益			
			pcParam.pcCmpLightGain = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_PIEXEL_COMPARE_CMP_TOLERANCE://比較的灰階公差
			pcParam.pcCmpTolerance = FileIO.GetData_INT();
			break;
		case FILE_IO_ALG_PARAM_PIEXEL_COMPARE_CMP_GAUSSIAN_SIZE://比較的高斯濾波尺寸
			pcParam.pcCmpGaussianSize = FileIO.GetData_INT();
			break;
		case FILE_IO_ALG_PARAM_PIEXEL_COMPARE_CMP_OPEN_SIZE://比較的開運算尺寸
			pcParam.pcCmpOpenSize = FileIO.GetData_INT();
		case FILE_IO_ALG_PARAM_PIEXEL_COMPARE_CMP_CLOSE_SIZE://比較的閉運算尺寸
			pcParam.pcCmpCloseSize = FileIO.GetData_INT();
			break;		

		case FILE_IO_ALG_PARAM_PIEXEL_COMPARE_BLOB_MIN_SIZE_D://區塊最小長度-um			
			pcParam.pcBlobMinSizeD = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_PIEXEL_COMPARE_BLOB_COUNT_USL://區塊數量下限
			pcParam.pcBlobCountUSL = FileIO.GetData_INT();
			break;
		case FILE_IO_ALG_PARAM_PIEXEL_COMPARE_BLOB_COUNT_LSL://區塊數量上限
			pcParam.pcBlobCountLSL = FileIO.GetData_INT();
			break;
		default:
		#ifdef _DEBUG
			index = index;
		#endif//_DEBUG
			break;
		}
	}	
	FileIO.SetErrorString(_T("Error, ReadAlgParamFile_PixelCompare Fault"));
	return false;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ExecAlgInspection_PixelCompare(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList)
{
#ifdef PATTERN_BINARY_USE
	return ExecAlgInspection_PixelCompare_v2(ModelPtr, ModelRgn, RoiRect, WndRect, UniFrameList);
#endif//PATTERN_BINARY_USE
	return ExecAlgInspection_PixelCompare_v1(ModelPtr, ModelRgn, RoiRect, WndRect, UniFrameList);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ExecAlgInspection_PixelCompare_v1(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList)
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
	TALG_PARAM_PIXEL_COMPARE &pcParam = GetAlgParamPixelCompare();	
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
	JetMemory.free_func(MaskPtr);
	JetMemory.free_func(GrayPtr);
	if ( false == IsOK )
	{
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
	int    Reading=0.0, USL=0.0, LSL=0.0;		
	bool   ApplyScale=false;	
	const bool      bRobustness = true;	
	const bool      bCompareUseMatch=false;
	const int       nMinReduceArea = GetAlgPatternMinReducedArea();
	const int       nFinalReduction = GetAlgPatternFinalReduction();
	const bool      bAdvancedLearning = GetAlgPatternAdvancedLearning();
	const bool      UseInterpolate = true;//CheckAlgPatternMatrchInterpolate();	
	
	TPixelCompareParam tpcParam;	
	std::vector<RECT>  TotalBlobRectList, BestBlobRectList;
	BuildPixelCompareParam(pcParam, ImageScale, tpcParam);
	
#ifdef _DEBUG	
	bool         bSave = true;
	CString      ComponentName;
	CString      DebugFolder=GetAlgDebugFolder();
	if ( NULL != ComponentPtr )
	{	ComponentName = ComponentPtr->GetComponentFullName(); }	
	if ( true == bSave )
	{
		str.Format(_T("%s\\%s_ModelWndAlgPixelCompare#%d_Roi.PNG"), DebugFolder, ComponentName, WndIndex+1);
		ImageAPI.SaveImage(str, RoiImageW, RoiImageH, RoiImageStep, RoiBitCount, RoiImagePtr, true);

		str.Format(_T("%s\\%s_ModelWndAlgPixelCompare#%d_Src.PNG"), DebugFolder, ComponentName, WndIndex+1);
		ImageAPI.SaveImage(str, FrameImageW, FrameImageH, FrameImageStep, FrameBitCount, FrameImagePtr, true);
	}
#endif//_DEBUG

	TPATTERN_ROI PatternRoi;
	CJetMatch    Match;	//影像匹配
	
	if ( Match.SetMatchLibType(MatchLibType)==false )
	{	
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
	const int    nCellExtSize=pcParam.pcCellExtSize;
	std::vector<TPATTERN_ROI> PatRoiList;	
	
	BestResS = DBL_MAX;
	USL = pcParam.pcBlobCountUSL;
	LSL = pcParam.pcBlobCountLSL;
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
		TotalBlobRectList.clear();		
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
				str.Format(_T("%s\\%s_ModelWndAlgPixelCompare#%d_PatMaskT#%d.PNG"), DebugFolder, ComponentName, WndIndex+1, i+1);
				ImageAPI.SaveImage(str, PatImgWUse, PatImgHUse, PatMaskImageStep, MaskBitCount, PatMaskPtr, true);			
			}
			str.Format(_T("%s\\%s_ModelWndAlgPixelCompare#%d_PatternT#%d.PNG"), DebugFolder, ComponentName, WndIndex+1, i+1);
			ImageAPI.SaveImage(str, PatImgWUse, PatImgHUse, PatImgStepUse, PatBitCountUse, PatImgPtrUse, true);				
			str.Format(_T("%s\\%s_ModelWndAlgPixelCompare#%d_ImageT#%d.PNG"), DebugFolder, ComponentName, WndIndex+1, i+1);
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
							str.Format(_T("%s\\%s_ModelWndAlgPixelCompare#%d_PatRoiT#%d#%d_Ext.PNG"), DebugFolder, ComponentName, WndIndex+1, i+1, j+1);
							ImageAPI.SaveImage(str, PatRoiImgW, PatRoiImgH, PatRoiImgStep, PatRoiBitCountUse, PatRoiImgPtr, true);					
							str.Format(_T("%s\\%s_ModelWndAlgPixelCompare#%d_ImgRoiT#%d#%d_Ext.PNG"), DebugFolder, ComponentName, WndIndex+1, i+1, j+1);
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
				ImgRoiImgStep = JetAPI::GetBMPImagePixelsPerLine(ImgRoiImgW, (unsigned int)nRoiBitCount, nPatAlign);
				if ( ImageAPI.ExtractRoiImage(nRoiImageW, nRoiImageH, nRoiImageStep, nRoiBitCount, RoiImgPtrUsed, ImgRoiRect, ImgRoiImgStep, ImgRoiImgPtr, false) == false )
				{	
					JetMemory.free_func(PatRoiImgPtr);
					continue;	
				}

			#ifdef _DEBUG
				if ( true == bSave )
				{					
					str.Format(_T("%s\\%s_ModelWndAlgPixelCompare#%d_PatRoiT#%d#%d.PNG"), DebugFolder, ComponentName, WndIndex+1, i+1, j+1);
					ImageAPI.SaveImage(str, PatRoiImgW, PatRoiImgH, PatRoiImgStep, PatRoiBitCountUse, PatRoiImgPtr, true);					
					str.Format(_T("%s\\%s_ModelWndAlgPixelCompare#%d_ImgRoiT#%d#%d.PNG"), DebugFolder, ComponentName, WndIndex+1, i+1, j+1);
					ImageAPI.SaveImage(str, ImgRoiImgW, ImgRoiImgH, ImgRoiImgStep, PatRoiBitCountUse, ImgRoiImgPtr, true);
				}
			#endif//_DEBUG
				
				std::vector<RECT> SubRectList;
				if ( ImageAPI.Compare2ImagePixel(ImgRoiImgW, ImgRoiImgH, ImgRoiImgStep, PatRoiBitCountUse, PatRoiImgPtr, ImgRoiImgPtr, NULL, tpcParam, SubRectList) == false )
				{	
					JetMemory.free_func(PatRoiImgPtr);
					JetMemory.free_func(ImgRoiImgPtr);
					continue;
				}
				JetMemory.free_func(PatRoiImgPtr);
				JetMemory.free_func(ImgRoiImgPtr);
				PatRoiPtr->ResultScore=SubRectList.size();
				for ( size_t n=0; n<SubRectList.size(); n++ )
				{
					RECT rc=SubRectList[n];
					::OffsetRect(&rc, nRoiX+ImgRoiRect.left, nRoiY+ImgRoiRect.top);
					TotalBlobRectList.push_back(rc);
				}
			}
			JetMemory.free_func(RoiMaskPtr_Bin);
			JetMemory.free_func(RoiImgPtrRotated);
			//判斷檢測結果				
			ResultS = TotalBlobRectList.size();
			PatParamPtr->SetResultReading(PolarityIdx, ResultS);
			PatParamPtr->CheckResultIndex();
			if ( ResultS < BestResS )
			{
				BestResS = ResultS;
				BestResX = ResultX;
				BestResY = ResultY;
				BestResA = ResultA;
				BestResSX = ResultSX;
				BestResSY = ResultSY;
				BestResMatch = ResultMatch;
				BestPatternIndex = (int)(i);			
				BestBlobRectList = TotalBlobRectList;
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
		}

		JetMemory.free_func(PatImgPtr);
		JetMemory.free_func(PatMaskPtr);
		JetMemory.free_func(PatGrayPtr);
	}
	JetMemory.free_func(RoiMaskPtr);
	JetMemory.free_func(RoiImagePtr);

	TPOINT2D RgnCp;
	bool Matched = false;	
	RgnCp.x=ModelRgn.GetCpX();
	RgnCp.y=ModelRgn.GetCpY();
	PatParamPtr = GetAlgPatternParamPtr(BestPatternIndex, true);
	if ( NULL != PatParamPtr ) 
	{
		Matched = true;
		PolarityIdx = PatParamPtr->GetResultPolarityIdx();
		ResultS = PatParamPtr->GetResultReading(PolarityIdx);
		ResultX = PatParamPtr->GetPatResultX(PolarityIdx);
		ResultY = PatParamPtr->GetPatResultY(PolarityIdx);
		ResultA = PatParamPtr->GetPatResultSkew(PolarityIdx);
		ResultSX = PatParamPtr->GetPatResultScaleX(PolarityIdx);
		ResultSY = PatParamPtr->GetPatResultScaleY(PolarityIdx);		
		ResultMatch = PatParamPtr->GetPatResultScore(PolarityIdx);

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
				PatRoiRect.left   = PatImgWUse-PatRoiRect2.right;
				PatRoiRect.top    = PatImgHUse-PatRoiRect2.bottom;
				PatRoiRect.right  = PatImgWUse-PatRoiRect2.left;
				PatRoiRect.bottom = PatImgHUse-PatRoiRect2.top;
			}
			else
			{	PatRoiRect = PatRoiPtr->RoiRect;	}
			PatRoiRect.left   += ResultSX+ImgRoiX+nRoiX+ShiftX;
			PatRoiRect.top    += ResultSY+ImgRoiY+nRoiY+ShiftY;
			PatRoiRect.right  += ResultSX+ImgRoiX+nRoiX+ShiftX;
			PatRoiRect.bottom += ResultSY+ImgRoiY+nRoiY+ShiftY;
			CAOIModel::CalcModelBoxRectRegion(PatRoiRect, FrameImageW, FrameImageH, RgnCp, ImageScale, ImageCp, ResBoxRgn);
			ResBox.SetBoxRegion(ResBoxRgn);				
			if ( PatRoiPtr->ResultScore > 0.00 )
			{	ResBox.SetBoxResultID(RESULT_ID_NG);	}
			else
			{	ResBox.SetBoxResultID(RESULT_ID_OK);	}

			str.Format(_T("%.0f"), PatRoiPtr->ResultScore);
			ResBox.SetBoxResultText(str);
			ResBox.SetBoxResultTextVisibled(true);
			ResBox.SetBoxResultValue(PatRoiPtr->ResultScore);
			//WndPtr->AddWndResultBox(ResBox);
		}
		
		const size_t BlobCount=BestBlobRectList.size();
		for ( i=0; i<BlobCount; i++ )
		{	
			CAOIBox ResBox;
			TREGION4D BoxRgn;
			const RECT &BlobRect=BestBlobRectList[i];
			CAOIModel::CalcModelBoxRectRegion(BlobRect, FrameImageW, FrameImageH, RgnCp, ImageScale, ImageCp, BoxRgn);
			//JetAPI::MoveRegion(BoxRgn, WndRectCalValue, BoxRgn);
			ResBox.SetBoxRegion(BoxRgn);
			ResBox.SetBoxResultID(RESULT_ID_NG);
			WndPtr->AddWndResultBox(ResBox);
		}	

	}
	if ( BestPatternIndex<0 && PatternCount>0 ) 
	{	BestPatternIndex = 0; }
	
	Reading = (int)(ResultS);
	USL = pcParam.pcBlobCountUSL;
	LSL = pcParam.pcBlobCountLSL;
	pcParam.pcBlobCountNum = Reading;
	SetAlgPatternSimilarityReading(ResultMatch);		
	SetAlgPatternResultIndex(BestPatternIndex);		

	CString strResult;	
	SetAlgResultReading1(Reading);	
	SetAlgResultText(_T("OK"));
	SetAlgResultID(RESULT_ID_OK);
	if ( Reading<LSL || Reading>USL )
	{
		strResult.Format(_T("NG:%d / %d"), Reading, LSL);		
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
bool CAlgParam::ExecAlgInspection_PixelCompare_v2(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList)
{
	const char fnName[]="CAlgParam::ExecAlgInspection_PixelCompare";
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
	TALG_PARAM_PIXEL_COMPARE &pcParam = GetAlgParamPixelCompare();		
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
	int    Reading=0, USL=0, LSL=0;
	bool   ApplyScale=false;
	const bool      bRobustness = true;	
	const bool      bCompareUseMatch=false;
	const int       nMinReduceArea = GetAlgPatternMinReducedArea();
	const int       nFinalReduction = GetAlgPatternFinalReduction();
	const bool      bAdvancedLearning = GetAlgPatternAdvancedLearning();
	const bool      UseInterpolate = true;//CheckAlgPatternMatrchInterpolate();
	
	TPixelCompareParam tpcParam;	
	std::vector<RECT>  TotalBlobRectList, BestBlobRectList;
	BuildPixelCompareParam(pcParam, ImageScale, tpcParam);

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
	const int    nCellExtSize=pcParam.pcCellExtSize;
	std::vector<TPATTERN_ROI> PatRoiList;	

	BestResS = DBL_MAX;
	USL = pcParam.pcBlobCountUSL;
	LSL = pcParam.pcBlobCountLSL;
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
			str.Format(_T("%s\\%s_ModelWndAlgPixelCompare#%d_PatternT#%d.PNG"), DebugFolder, ComponentName, WndIndex+1, i+1);
			ImageAPI.SaveImage(str, PatImgWUse, PatImgHUse, PatImgStepUse, PatBitCountUse, PatImgPtrUse, true);				
			str.Format(_T("%s\\%s_ModelWndAlgPixelCompare#%d_ImageT#%d.PNG"), DebugFolder, ComponentName, WndIndex+1, i+1);
			ImageAPI.SaveImage(str, RoiImageW, RoiImageH, RoiImageStep, RoiBitCount, RoiImgPtrUse, true);
		}
	#endif//_DEBUG

		//initial eMatch
		TotalBlobRectList.clear();		
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
				RoiImgPtrUsed2 = RoiImgPtrSrc;				
				
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
							str.Format(_T("%s\\%s_ModelWndAlgPixelCompare#%d_PatRoiT#%d#%d_Ext.PNG"), DebugFolder, ComponentName, WndIndex+1, i+1, j+1);
							ImageAPI.SaveImage(str, PatRoiImgW, PatRoiImgH, PatRoiImgStep, PatRoiBitCountUse, PatRoiImgPtr, true);					
							str.Format(_T("%s\\%s_ModelWndAlgPixelCompare#%d_ImgRoiT#%d#%d_Ext.PNG"), DebugFolder, ComponentName, WndIndex+1, i+1, j+1);
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
					str.Format(_T("%s\\%s_ModelWndAlgPixelCompare#%d_PatRoiT#%d#%d.PNG"), DebugFolder, ComponentName, WndIndex+1, i+1, j+1);
					ImageAPI.SaveImage(str, PatRoiImgW, PatRoiImgH, PatRoiImgStep, PatRoiBitCountUse, PatRoiImgPtr, true);					
					str.Format(_T("%s\\%s_ModelWndAlgPixelCompare#%d_ImgRoiT#%d#%d.PNG"), DebugFolder, ComponentName, WndIndex+1, i+1, j+1);
					ImageAPI.SaveImage(str, ImgRoiImgW, ImgRoiImgH, ImgRoiImgStep, PatRoiBitCountUse, ImgRoiImgPtr, true);
				}
			#endif//_DEBUG
				std::vector<RECT> SubRectList;				
				if ( ImageAPI.Compare2ImagePixel(ImgRoiImgW, ImgRoiImgH, ImgRoiImgStep, PatRoiBitCountUse, PatRoiImgPtr, ImgRoiImgPtr, NULL, tpcParam, SubRectList) == false )
				{	
					JetMemory.free_func(PatRoiImgPtr);
					JetMemory.free_func(ImgRoiImgPtr);
					continue;
				}
				JetMemory.free_func(PatRoiImgPtr);
				JetMemory.free_func(ImgRoiImgPtr);				
				PatRoiPtr->ResultScore = SubRectList.size();
				for ( size_t n=0; n<SubRectList.size(); n++ )
				{
					RECT rc=SubRectList[n];
					::OffsetRect(&rc, nRoiX+ImgRoiRect.left, nRoiY+ImgRoiRect.top);
					TotalBlobRectList.push_back(rc);
				}
			}
			JetMemory.free_func(RoiMaskPtr_Bin);
			JetMemory.free_func(RoiImgPtrRotated);
			//判斷檢測結果					
			ResultS = TotalBlobRectList.size();
			PatParamPtr->SetResultReading(PolarityIdx, ResultS);
			PatParamPtr->CheckResultIndex();
			if ( ResultS < BestResS )
			{
				BestResS = ResultS;
				BestResX = ResultX;
				BestResY = ResultY;
				BestResA = ResultA;
				BestResSX = ResultSX;
				BestResSY = ResultSY;
				BestResMatch = ResultMatch;
				BestPatternIndex = (int)(i);
				BestBlobRectList = TotalBlobRectList;
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

	TPOINT2D RgnCp;
	bool Matched = false;
	PatParamPtr = GetAlgPatternParamPtr(BestPatternIndex, true);
	if ( NULL != PatParamPtr ) 
	{
		Matched = true;
		PolarityIdx = PatParamPtr->GetResultPolarityIdx();
		ResultS = PatParamPtr->GetResultReading(PolarityIdx);
		ResultX = PatParamPtr->GetPatResultX(PolarityIdx);
		ResultY = PatParamPtr->GetPatResultY(PolarityIdx);
		ResultA = PatParamPtr->GetPatResultSkew(PolarityIdx);
		ResultSX = PatParamPtr->GetPatResultScaleX(PolarityIdx);
		ResultSY = PatParamPtr->GetPatResultScaleY(PolarityIdx);		
		ResultMatch = PatParamPtr->GetPatResultScore(PolarityIdx);

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
				PatRoiRect.left   = PatImgWUse-PatRoiRect2.right;
				PatRoiRect.top    = PatImgHUse-PatRoiRect2.bottom;
				PatRoiRect.right  = PatImgWUse-PatRoiRect2.left;
				PatRoiRect.bottom = PatImgHUse-PatRoiRect2.top;
			}
			else
			{	PatRoiRect = PatRoiPtr->RoiRect;	}
			PatRoiRect.left   += ResultSX+ImgRoiX+nRoiX+ShiftX;
			PatRoiRect.top    += ResultSY+ImgRoiY+nRoiY+ShiftY;
			PatRoiRect.right  += ResultSX+ImgRoiX+nRoiX+ShiftX;
			PatRoiRect.bottom += ResultSY+ImgRoiY+nRoiY+ShiftY;
			CAOIModel::CalcModelBoxRectRegion(PatRoiRect, FrameImageW, FrameImageH, RgnCp, ImageScale, ImageCp, ResBoxRgn);
			ResBox.SetBoxRegion(ResBoxRgn);				
			if ( PatRoiPtr->ResultScore > 0.00 )
			{	ResBox.SetBoxResultID(RESULT_ID_NG);	}
			else
			{	ResBox.SetBoxResultID(RESULT_ID_OK);	}

			str.Format(_T("%.0f"), PatRoiPtr->ResultScore);
			ResBox.SetBoxResultText(str);
			ResBox.SetBoxResultTextVisibled(true);
			ResBox.SetBoxResultValue(PatRoiPtr->ResultScore);
			//WndPtr->AddWndResultBox(ResBox);
		}

		const size_t BlobCount=BestBlobRectList.size();
		for ( i=0; i<BlobCount; i++ )
		{	
			CAOIBox ResBox;
			TREGION4D BoxRgn;
			const RECT &BlobRect=BestBlobRectList[i];
			CAOIModel::CalcModelBoxRectRegion(BlobRect, FrameImageW, FrameImageH, RgnCp, ImageScale, ImageCp, BoxRgn);
			//JetAPI::MoveRegion(BoxRgn, WndRectCalValue, BoxRgn);
			ResBox.SetBoxRegion(BoxRgn);
			ResBox.SetBoxResultID(RESULT_ID_NG);
			WndPtr->AddWndResultBox(ResBox);
		}	
	}
	if ( BestPatternIndex<0 && PatternCount>0 ) 
	{	BestPatternIndex = 0; }
	
	Reading = (int)(ResultS);
	USL = pcParam.pcBlobCountUSL;
	LSL = pcParam.pcBlobCountLSL;
	pcParam.pcBlobCountNum = Reading;
	SetAlgPatternSimilarityReading(ResultMatch);		
	SetAlgPatternResultIndex(BestPatternIndex);		

	CString strResult;	
	SetAlgResultReading1(Reading);	
	SetAlgResultText(_T("OK"));
	SetAlgResultID(RESULT_ID_OK);
	if ( Reading<LSL || Reading>USL )
	{
		strResult.Format(_T("NG:%d / %d"), Reading, LSL);		
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
