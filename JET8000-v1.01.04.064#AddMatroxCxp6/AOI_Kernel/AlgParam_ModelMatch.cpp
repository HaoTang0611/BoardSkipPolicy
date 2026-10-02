// AlgParam_ModelMatch.cpp: implementation of the CAlgParam class.
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
bool CAlgParam::WriteAlgParamFile_ModelMatch(const TALG_PARAM_MODEL_MATCH &mmParam, CAOIFileIO &FileIO)//儲存模板匹配參數
{	
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_MODEL_MATCH_START, 0) == false ) { return false; }

	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_MODEL_MATCH_DOCK_MODE, mmParam.mmDockMode) == false ) { return false; }	
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_MODEL_MATCH_RECHECK_BOX, mmParam.mmReCheckBox) == false ) { return false; }		

	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_MODEL_MATCH_END, 0) == false ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ReadAlgParamFile_ModelMatch(TALG_PARAM_MODEL_MATCH &mmParam, CAOIFileIO &FileIO)//載入模板匹配參數
{
	int       index = 0;
	while ( FileIO.CheckFileEnd()==false )
	{ 		
		if ( FileIO.LoadChunk(index) == false )		
		{	continue; }
		switch ( index )
		{
		case FILE_IO_ALG_PARAM_MODEL_MATCH_END://模板匹配參數-終點
			return true;
			break;
		case FILE_IO_ALG_PARAM_MODEL_MATCH_DOCK_MODE://靠邊模式
			mmParam.mmDockMode = (ALG_MATCH_DOCK_MODE)(FileIO.GetData_INT());
			break;
		case FILE_IO_ALG_PARAM_MODEL_MATCH_RECHECK_BOX://重新確認特徵
			mmParam.mmReCheckBox = (FileIO.GetData_INT());
			break;
		default:
		#ifdef _DEBUG
			index = index;
		#endif//_DEBUG
			break;
		}
	}
	FileIO.SetErrorString(_T("Error, ReadAlgParamFile_ModelMatch Fault"));
	return false;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ExecAlgInspection_ModelMatch(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList)
{	
	if ( NULL == ModelPtr ) { return false; }
	CAOIComponent *ComponentPtr = ModelPtr->GetModelComponentPtr();
	const size_t FrameCount = UniFrameList.size();
	const size_t ImageFrameIndex = m_AlgImageBinParam.GetBinaryFrameIndex();
	if ( ImageFrameIndex >= FrameCount ) 
	{	return false; }	
	CAOIWnd         *WndPtr = CAlgParam::GetAlgWndPtr();
	if ( NULL == WndPtr ) { return false; }	
	const unsigned int WndIndex = CAlgParam::GetAlgWndPtr()->GetWndIndex();		

	const BOX_TOWARD WndToward = WndPtr->GetWndToward();
	TUNI_FRAME      *UniFramePtr  = &(UniFrameList[ImageFrameIndex]);
	const IMAGE_SIZE FrameImageW    = UniFramePtr->ImageW;
	const IMAGE_SIZE FrameImageH    = UniFramePtr->ImageH;
	const IMAGE_SIZE FrameImageStep = UniFramePtr->ImageStep;
	const IMAGE_SIZE FrameBitCount  = UniFramePtr->BitCount;		
	IMAGE_PTR        FrameImagePtr  = UniFramePtr->ImagePtr;
	MASK_PTR         FrameMaskPtr   = UniFramePtr->MaskPtr;
	SPACE_PTR        FrameSpacePtr  = UniFramePtr->SpacePtr;
	TALG_PARAM_MODEL_MATCH   &mmParam = GetAlgParamModelMatch();
	const int    ModelMaskFlag = WndPtr->GetWndModelMaskFlag();
	const size_t MaskWndCount = WndPtr->GetWndMaskWndCount();	
	
	bool          IsOK = false;
	IMAGE_SIZE    MaskW=0;
	IMAGE_SIZE    MaskH=0;	
	IMAGE_SIZE    MaskStep=0;
	IMAGE_SIZE    MaskBitCount=8;
	MASK_PTR      MaskPtr  = NULL;
	IMAGE_PTR     GrayPtr = NULL;	
	const bool    bTestWnd = false;

	IMAGE_PTR        PatImgPtr  = NULL;
	IMAGE_SIZE       PatImgW    = 1024;
	IMAGE_SIZE       PatImgH    = 1024;
	IMAGE_SIZE       PatImgStep = 1024;
	const IMAGE_SIZE PatBitCount  = 8;
	std::vector<TREGION4D> FeatureList;
	const TPOINT2D   ImageScale = ModelPtr->GetModelImageScale();	

	if (GetAlgParamResinHeight().enabled && (FN_DISABLE!=AOIDataCollect.GetSystemParameter().m_ResinHeightAlignEnabled))
	{
		TALG_PARAM_RESIN_HEIGHT &Param = CAlgParam::GetAlgParamResinHeight();
		IsOK = ExecAlgInspection_Height(ModelPtr, ModelRgn, RoiRect, WndRect, UniFrameList);

		if (IsOK == true && GetAlgParamResinHeight().enableDoubleCheck && Param.resultID != 1)
		{
			// enableDoubleCheck=true、NG後正常執行2D定位
		}
		else
		{
			ExecAlgInspection_IPC(ModelPtr, ModelRgn, RoiRect, WndRect, UniFrameList);
			return IsOK;
		}
	}

#ifdef _DEBUG
	CString      str;
	bool         bSave = true;
	CString      ComponentName;
	CString      DebugFolder=GetAlgDebugFolder();	
	if ( NULL != ComponentPtr )
	{	ComponentName = ComponentPtr->GetComponentFullName(); }
#endif//_DEBUG
	
	if ( WndPtr->BuildWndBoxImage(ModelPtr, ImageScale, PatBitCount, PatImgW, PatImgH, PatImgStep, PatImgPtr, FeatureList) == false )
	{	return false; }

#ifdef _DEBUG
	if ( TRUE == bSave )
	{
		if ( NULL != FrameImagePtr )
		{
			str.Format(_T("%s\\%s_ModelWndAlgModelMatchFrame#%d.BMP"), DebugFolder, ComponentName, WndIndex+1);
			ImageAPI.SaveBMPImage(str, FrameImageW, FrameImageH, FrameImageStep, FrameBitCount, FrameImagePtr, true);	
		}
	}
#endif//_DEBUG

	if ( ExecAlgUniFrameBinary(m_AlgImageBinParam, WndRect, RoiRect, UniFrameList, MaskW, MaskH, MaskStep, MaskBitCount, MaskPtr, GrayPtr, bTestWnd) == false )
	{	
		JetMemory.free_func(MaskPtr);
		JetMemory.free_func(GrayPtr);
		JetMemory.free_func(PatImgPtr);
		return false; 
	}

#ifdef _DEBUG
	if ( true == bSave )
	{
		str.Format(_T("%s\\%s_ModelWndAlgModelMatchGray#%d.BMP"), DebugFolder, ComponentName, WndIndex+1);
		ImageAPI.SaveBMPImage(str, MaskW, MaskH, MaskStep, MaskBitCount, GrayPtr, true);	
	}
#endif//_DEBUG
	const size_t FeatureCount = FeatureList.size();

	IMAGE_PTR       RoiImagePtr = NULL;
	IMAGE_SIZE      RoiImageW = RoiRect.right-RoiRect.left;
	IMAGE_SIZE      RoiImageH = RoiRect.bottom-RoiRect.top;	
	IMAGE_SIZE      RoiBitCount = 8;
	IMAGE_SIZE      RoiImageStep = JetAPI::GetBMPImagePixelsPerLine(RoiImageW, RoiBitCount, 4);

	if ( BINARY_DISABLE == m_AlgImageBinParam.GetBinaryMode() )
	{	IsOK = ImageAPI.ExtractGrayRoiImage(MaskW, MaskH, MaskStep, GrayPtr, RoiRect, RoiImageStep, RoiImagePtr, false); }
	else
	{	IsOK = ImageAPI.ExtractGrayRoiImage(MaskW, MaskH, MaskStep, MaskPtr, RoiRect, RoiImageStep, RoiImagePtr, false); }
	if ( false == IsOK )
	{	
		JetMemory.free_func(MaskPtr);
		JetMemory.free_func(GrayPtr);
		JetMemory.free_func(PatImgPtr);
		return false; 
	}		

	int             nMinReduceAreaRes=0;
	const int       nPatBitCount = 8;
	const int       nPatImgW = (int)(PatImgW);
	const int       nPatImgH = (int)(PatImgH);
	const int       nPatImgStep = (int)(PatImgStep);
	const int       nImageW = (int)(FrameImageW);
	const int       nImageH = (int)(FrameImageH);
	const int       nImageStep = (int)(FrameImageStep);
	const int       nGrayStep = (int)(MaskStep);
	const int       nRoiImageW = (int)(RoiImageW);
	const int       nRoiImageH = (int)(RoiImageH);
	const int       nRoiImageStep = (int)(RoiImageStep);
	const int       nRoiX = RoiRect.left;
	const int       nRoiY = RoiRect.top;
	const int       nRoiW = (int)(nRoiImageW);
	const int       nRoiH = (int)(nRoiImageH);
	const int       nRoiRectSize = (int)((nRoiW)*(nRoiH));
	const double    dRoiCpx = (double)((RoiRect.left+RoiRect.right)*0.5);
	const double    dRoiCpy = (double)((RoiRect.top+RoiRect.bottom)*0.5);
	const float     fMinScore = (float)(GetAlgPatternSimilarityLSL()/100.0);	
	const bool      bRobustness = true;		
	const int       nMinReduceArea = GetAlgPatternMinReducedArea();
	const int       nFinalReduction = GetAlgPatternFinalReduction();
	const bool      bAdvancedLearning = GetAlgPatternAdvancedLearning();
	const int       nRecheckBox = mmParam.mmReCheckBox;
	const ALG_MATCH_DOCK_MODE DockMode = mmParam.mmDockMode;
	JET_MATCH_LIB_TYPE MatchLibType = AOIDataCollect.GetMatchLibType();
	TPOINT2D        WndRectCalValue = WndPtr->GetWndRectCalValue();

#ifdef _DEBUG
	if ( true == bSave )
	{		
		str.Format(_T("%s\\%s_ModelWndAlgModelMatchRoi#%d.BMP"), DebugFolder, ComponentName, WndIndex+1);
		ImageAPI.SaveBMPImage(str, RoiImageW, RoiImageH, RoiImageStep, RoiBitCount, RoiImagePtr, true);	
		str.Format(_T("%s\\%s_ModelWndAlgModelMatchPat#%d.BMP"), DebugFolder, ComponentName, WndIndex+1);
		ImageAPI.SaveBMPImage(str, PatImgW, PatImgH, PatImgStep, RoiBitCount, PatImgPtr, true);			
	}
#endif//_DEBUG

	//影像匹配
	int          i=0;
	CString      ErrStr;
	int          NResults = 0;
	int          ResultIndex=0;
	double       ResultCX=0.0, ResultCY=0.0;
	double       RoiOffsetX=0.0, RoiOffsetY=0.0;	
	double       CadOffsetX=0.0, CadOffsetY=0.0, CadSkew=0.0, CadScaleX=100.0, CadScaleY=100.0;
	double       ResultX=0, ResultY=0, ResultA=0, ResultS=0, ResultSX=0, ResultSY=0;	
	char         tempStr[128]="";	
	const bool   bUseDontCareArea = false;	
	CJetMatch    Match;
	IsOK = false;	
	if ( 1!=FeatureCount || FN_ENABLE!=nRecheckBox )//只有一個特徵且開啟重複確認就直接去使用Blob計算
	{
		while ( true )
		{
			//JET_MATCH_LIB_EVS = 1,JET_MATCH_LIB_MIM = 2
			if ( Match.SetMatchLibType(MatchLibType)==false )
			{	
				ErrStr=Match.GetErrorString();
				break; 
			}

			//initial eMatch
			nMinReduceAreaRes=nMinReduceArea;
			Match.SetMatchDefaultParam();
			Match.SetRobustness(bRobustness);
			Match.SetMinReducedArea(nMinReduceArea);
			Match.SetFinalReduction(nFinalReduction);		
			if ( true==bUseDontCareArea && (MaskWndCount>0 || MODEL_MASK_NONE!=ModelMaskFlag) )
			{
				Match.SetUseDontCareArea(true);
				Match.SetDontCareThreshold(1); 
			}
			else
			{
				Match.SetUseDontCareArea(false);
				Match.SetDontCareThreshold(0); 
			}
			Match.SetAdvancedLearning(bAdvancedLearning);				
			if ( Match.LearnPattern(nPatImgW, nPatImgH, nPatImgStep, nPatBitCount, PatImgPtr, true) == false )
			{
				ErrStr=Match.GetErrorString();
				break; 
			}
			nMinReduceAreaRes = Match.GetMinReducedArea();

			if ( true==GetAlgSkewEnabled() )//&& FN_DISABLE==nRecheckBox )
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
			Match.SetInterpolate(true);
			Match.SetMinScore(0);//fMinScore
			//Match.SetMinScore(-1);//fMinScore

			if ( ALG_MATCH_DOCK_DISABLE == DockMode )
			{	Match.SetMaxPositions(1); }
			else
			{	Match.SetMaxPositions(10); }
			if ( Match.Match(nRoiImageW, nRoiImageH, nRoiImageStep, nPatBitCount, RoiImagePtr, true) == false )
			{
				ErrStr=Match.GetErrorString();
				break; 
			}

			IsOK = true;
			break;
		};	
		if ( false == IsOK )
		{
			SetAlgResultText(ErrStr);
			WndPtr->SetWndResultText(ErrStr);
			JetMemory.free_func(GrayPtr);
			JetMemory.free_func(MaskPtr);
			JetMemory.free_func(PatImgPtr);	
			JetMemory.free_func(RoiImagePtr);
			return false;	
		}	
		
		NResults = Match.GetNumPositions();	
		if ( ALG_MATCH_DOCK_DISABLE != DockMode )
		{
			bool bFind=true;
			int  BestIdx=0;
			double ResX=0, ResY=0, ResS=0;
			double BestResX=0, BestResY=0;
			double ResLSL = GetAlgPatternSimilarityLSL();
			BOX_TOWARD DockToward = WndToward;
			if ( ALG_MATCH_DOCK_TO_SHOULDER == DockMode )
			{
				switch ( WndToward )
				{
				case BOX_TOWARD_LEFT:	DockToward=BOX_TOWARD_RIGHT; break;
				case BOX_TOWARD_UP:		DockToward=BOX_TOWARD_DOWN; break;
				case BOX_TOWARD_RIGHT:	DockToward=BOX_TOWARD_LEFT; break;			
				case BOX_TOWARD_DOWN:	DockToward=BOX_TOWARD_UP; break;
				}
			}
			for ( i=0; i<NResults; i++)
			{
				ResS = Match.GetResultScore(i)*100.0;
				if ( ResS < ResLSL ) { continue; }
				ResX = Match.GetResultPosX(i);
				ResY = Match.GetResultPosY(i);
				if ( true == bFind )
				{
					BestIdx = i;
					BestResX = ResX;
					BestResY = ResY;
					bFind = false;
					continue;
				}
				switch ( DockToward )
				{
				case BOX_TOWARD_LEFT:
					if ( ResX < BestResX )
					{
						BestIdx = i;
						BestResX = ResX;
						BestResY = ResY;
					}
					break;
				case BOX_TOWARD_UP:
					if ( ResY < BestResY )
					{
						BestIdx = i;
						BestResX = ResX;
						BestResY = ResY;
					}
					break;
				case BOX_TOWARD_RIGHT:
					if ( ResX > BestResX )
					{
						BestIdx = i;
						BestResX = ResX;
						BestResY = ResY;
					}
					break;
				case BOX_TOWARD_DOWN:
					if ( ResY > BestResY )
					{
						BestIdx = i;
						BestResX = ResX;
						BestResY = ResY;
					}
					break;
				}
			}
			ResultIndex = BestIdx;
		}

		if ( 0 == NResults ) 
		{
			ResultX = nRoiImageW/2;
			ResultY = nRoiImageH/2;
		}
		else
		{
			const int idx = ResultIndex;
			ResultX = Match.GetResultPosX(idx);
			ResultY = Match.GetResultPosY(idx);
			ResultA = Match.GetResultAngle(idx);
			ResultS = Match.GetResultScore(idx)*100.0;
			ResultSX = Match.GetResultScaleX(idx);
			ResultSY = Match.GetResultScaleY(idx);
			if ( FN_ENABLE==nRecheckBox && 8==nPatBitCount )
			{	
				CJetBlob BlobDetector;
				double   BlobAvePosX=0;
				double   BlobAvePosY=0;
				RECT     TmpRect={0,0,0,0};		
				RECT     BlobPatRect={0,0,0,0};
				RECT     BlobRoiRect={0,0,0,0};
				IMAGE_PTR  BlobPatPtr=NULL;
				IMAGE_SIZE BlobPatW=0, BlobPatH=0, BlobPatStep=0;
				IMAGE_PTR  BlobRoiPtr=NULL;
				IMAGE_SIZE BlobRoiW=0, BlobRoiH=0, BlobRoiStep=0;

				BlobDetector.InitialBlob();
				BlobDetector.SetBlobSortMode(BLOB_SORT_PIXELS);
				BlobDetector.SetBlobConnectivity(BLOB_CONNECTIVITY_4);
				JetAPI::SizeToRect(nPatImgW, nPatImgH, TmpRect);
				if ( BlobDetector.GrayImageRoiBlobDetect(nPatImgW, nPatImgH, nPatImgStep, PatImgPtr, TmpRect, 128, 255) == true )
				{
					size_t ii=0;
					int   RoiExtRealW=10;
					int   RoiExtRealH=10;
					const int RoiExt=10;
					const int BlobPatExt=2;
					const int BlobRoiExt=BlobPatExt+RoiExt;
					const int nAlign = 4;
					TPOINT4D BlobPos;
					TBlobResult  Blob;
					TBlobResult *BlobPtr=NULL;					
					const size_t BlobResCount = BlobDetector.GetBlobCount();				
					std::vector<TPOINT4D> BlobPosList;
					std::vector<TBlobResult> BlobList(BlobResCount);					
					BlobList.clear();
					BlobAvePosX = BlobAvePosY = 0.0;
					for ( ii=0; ii<BlobResCount; ii++ )
					{
						BlobPtr = BlobDetector.GetBlobPtr(ii, false);
						if ( NULL == BlobPtr ) { continue; }
						//BlobPatRect = BlobPtr->m_BlobRectRaw;
						BlobPatRect = BlobPtr->m_BlobRect;//20230628-Blob						
						::InflateRect(&BlobPatRect, BlobPatExt, BlobPatExt);
						BlobPatW = BlobPatRect.right-BlobPatRect.left;
						BlobPatH = BlobPatRect.bottom-BlobPatRect.top;
						BlobPatStep = JetAPI::GetBMPImagePixelsPerLine(BlobPatW, PatBitCount, nAlign);
					
						BlobRoiW = BlobPatW+RoiExt+RoiExt;
						BlobRoiH = BlobPatH+RoiExt+RoiExt;
						BlobRoiW = MIN(nRoiImageW, BlobRoiW);
						BlobRoiH = MIN(nRoiImageH, BlobRoiH);
						RoiExtRealW=(BlobRoiW-BlobPatW)/2;
						RoiExtRealH=(BlobRoiH-BlobPatH)/2;
						BlobRoiStep = JetAPI::GetBMPImagePixelsPerLine(BlobRoiW, PatBitCount, nAlign);
						BlobRoiRect.left = (int)(ResultX-(nPatImgW/2)+BlobPatRect.left-RoiExtRealW);
						BlobRoiRect.top  = (int)(ResultY-(nPatImgH/2)+BlobPatRect.top-RoiExtRealH);
						if ( BlobRoiRect.left < 0 ) { BlobRoiRect.left = 0; }
						if ( BlobRoiRect.top  < 0 ) { BlobRoiRect.top  = 0; }
						BlobRoiRect.right = BlobRoiRect.left+BlobRoiW;
						BlobRoiRect.bottom = BlobRoiRect.top+BlobRoiH;
						if ( BlobRoiRect.right > nRoiImageW ) 
						{
							BlobRoiRect.right = nRoiImageW;
							BlobRoiRect.left  = BlobRoiRect.right-BlobRoiW;
						}
						if ( BlobRoiRect.bottom > nRoiImageH ) 
						{
							BlobRoiRect.bottom = nRoiImageH;
							BlobRoiRect.top  = BlobRoiRect.bottom-BlobRoiH;
						}					
						if ( ImageAPI.ExtractRoiImage(nRoiImageW, nRoiImageH, nRoiImageStep, PatBitCount, RoiImagePtr, BlobRoiRect, BlobRoiStep, BlobRoiPtr, false) == false ) 
						{
							JetMemory.free_func(BlobPatPtr);	
							JetMemory.free_func(BlobRoiPtr);
							continue; 
						}
					#ifdef _DEBUG
						if ( true == bSave )
						{		
							str.Format(_T("%s\\%s_ModelWndAlgModelMatchBlobRoi#%d#%d.BMP"), DebugFolder, ComponentName, WndIndex+1, ii+1);
							ImageAPI.SaveBMPImage(str, BlobRoiW, BlobRoiH, BlobRoiStep, PatBitCount, BlobRoiPtr, true);							
						}
					#endif//_DEBUG

						int LocalThreshold=128;
						if ( BINARY_DISABLE == m_AlgImageBinParam.GetBinaryMode() )
						{						
							RECT LocalRect={0,0,0,0};
							JetAPI::SizeToRect(BlobRoiW, BlobRoiH, LocalRect);
							ImageAPI.CalcGrayImageOTSUThreshold(BlobRoiW, BlobRoiH, BlobRoiStep, BlobRoiPtr, LocalRect, LocalThreshold);
						}					
					
						RECT LocalTempRect={0,0,0,0};
						CJetBlob LocalBlobDetector;						
						LocalBlobDetector.InitialBlob();
						LocalBlobDetector.SetBlobSortMode(BLOB_SORT_PIXELS);
						LocalBlobDetector.SetBlobConnectivity(BLOB_CONNECTIVITY_4);
						JetAPI::SizeToRect(BlobRoiW, BlobRoiH, LocalTempRect);

						if ( LocalBlobDetector.GrayImageRoiBlobDetect(BlobRoiW, BlobRoiH, BlobRoiStep, BlobRoiPtr, LocalTempRect, LocalThreshold, 255) == false )
						{
							JetMemory.free_func(BlobRoiPtr);
							continue; 
						}
						JetMemory.free_func(BlobRoiPtr);	
						const size_t BlobResCount2 = LocalBlobDetector.GetBlobCount();				
						if ( 0 == BlobResCount2 ) { continue; }
						TBlobResult *LocalBlobPtr=LocalBlobDetector.GetBlobPtr(0, true);
						if ( NULL == LocalBlobPtr ) { continue; }
						Blob.m_BlobGCPosX = LocalBlobPtr->m_BlobGCPosX+BlobRoiRect.left;
						Blob.m_BlobGCPosY = LocalBlobPtr->m_BlobGCPosY+BlobRoiRect.top;
						Blob.m_BlobRect = BlobRoiRect;
						BlobList.push_back(Blob);

						BlobPos.x = (BlobRoiRect.left+BlobRoiRect.right)/2.0;
						BlobPos.y = (BlobRoiRect.top+BlobRoiRect.bottom)/2.0;
						BlobPos.u = Blob.m_BlobGCPosX;
						BlobPos.v = Blob.m_BlobGCPosY;
						BlobPosList.push_back(BlobPos);

						BlobAvePosX += Blob.m_BlobGCPosX;
						BlobAvePosY += Blob.m_BlobGCPosY;
					}
					const size_t BlobCount2 = BlobList.size();
					if ( BlobCount2 > 0 ) 
					{
						BlobAvePosX /= BlobCount2;
						BlobAvePosY /= BlobCount2;
						ResultX = BlobAvePosX;
						ResultY = BlobAvePosY;
					}
					double dSkew = GetAlgSkewCalcRange();
					double BlobSkew = CalcRotateAngle(BlobPosList, dSkew);
					ResultA = BlobSkew;
				}
			}
		}		
	}
	else
	{
		RECT WndTempRect={0,0,0,0};
		int  WndThreshold=128;				
		JetAPI::SizeToRect(nRoiImageW, nRoiImageH, WndTempRect);
		if ( BINARY_DISABLE == m_AlgImageBinParam.GetBinaryMode() )
		{	ImageAPI.CalcGrayImageOTSUThreshold(nRoiImageW, nRoiImageH, nRoiImageStep, RoiImagePtr, WndTempRect, WndThreshold);	}					
				
		CJetBlob WndBlobDetector;
		std::vector<POINT> Contour;
		WndBlobDetector.InitialBlob();
		WndBlobDetector.SetBlobSortMode(BLOB_SORT_PIXELS);
		WndBlobDetector.SetBlobConnectivity(BLOB_CONNECTIVITY_4);
		JetAPI::SizeToRect(nRoiImageW, nRoiImageH, WndTempRect);
		if ( WndBlobDetector.GrayImageRoiBlobDetect(nRoiImageW, nRoiImageH, nRoiImageStep, RoiImagePtr, WndTempRect, WndThreshold, 255) == true )
		{	
			TBlobResult *BlobPtr=WndBlobDetector.GetBlobPtr(0, true);
			const size_t WndBlobCount = WndBlobDetector.GetBlobCount();
			if ( NULL == BlobPtr ) 
			{
				ResultX = nRoiImageW/2;
				ResultY = nRoiImageH/2;
			}
			else
			{	
				bool bBlobOutsideRect=false;//確認區塊範圍是否超過ROI
				const bool bCheckBlobRect=true;
				TREGION4D   FeatureRng=FeatureList[0];
				const double FeatureW=FeatureRng.GetSizeX();
				const double FeatureH=FeatureRng.GetSizeY();
				const double FeatureImgW=FeatureW*ImageScale.x;
				const double FeatureImgH=FeatureH*ImageScale.y;
				double TmpCpX=0, TmpCpY=0, TmpW=0, TmpH=0, TmpAngle=0;
				if ( false == bCheckBlobRect )
				{	bBlobOutsideRect = false;	}
				else
				{
					//const RECT BlobRect=BlobPtr->m_BlobRectRaw;
					const RECT BlobRect=BlobPtr->m_BlobRect;//20230628-Blob
					const double BlobRectW=BlobRect.right-BlobRect.left;
					const double BlobRectH=BlobRect.bottom-BlobRect.top;
					//碰到外圍, 且尺寸縮小75%視為超過ROI
					if ( BlobRect.left<=WndTempRect.left || BlobRect.top<=WndTempRect.top ||
						 BlobRect.right>=WndTempRect.right || BlobRect.bottom>=WndTempRect.bottom )
					{	
						double BlobRatioW=0.0, BlobRatioH=0.0;
						if ( fabs(FeatureImgW) > 0.001 )
						{	BlobRatioW=BlobRectW/FeatureImgW; }
						if ( fabs(FeatureImgH) > 0.001 )
						{	BlobRatioH=BlobRectH/FeatureImgH; }
						if ( BlobRatioW<0.75 || BlobRatioH<0.75 )
						{	bBlobOutsideRect = true;	}
					}
					else
					{	bBlobOutsideRect = false; }
				}
				if ( true == bBlobOutsideRect )
				{
					ResultX = nRoiImageW/2;
					ResultY = nRoiImageH/2;
				}
				else
				{
					if ( true==GetAlgSkewEnabled() )//&& FN_DISABLE==nRecheckBox )
					{	
						double dSkew = GetAlgSkewCalcRange();
						float fSkewMax =  (float)(dSkew);
						float fSkewMin = -(float)(dSkew);
						WndBlobDetector.GetBlobContour(BlobPtr, Contour);
						ImageAPI.CalcPointListMinAreaRect(Contour, fSkewMin, fSkewMax, 1, TmpCpX, TmpCpY, TmpW, TmpH, TmpAngle);
					}
					else
					{
						TmpAngle = 0.0;							
						TmpW = BlobPtr->GetBlobRectW();
						TmpH = BlobPtr->GetBlobRectH();
						TmpCpX = BlobPtr->GetBlobRectCpX();
						TmpCpY = BlobPtr->GetBlobRectCpY();
					}
					NResults = 1;
					ResultX = TmpCpX;
					ResultY = TmpCpY;
					ResultA = TmpAngle;
					ResultS = 100;
					ResultSX = TmpW/FeatureImgW;
					ResultSY = TmpH/FeatureImgH;	
				#ifdef _DEBUG
					if ( true == bSave )
					{	
						IMAGE_PTR  BP=NULL;
						IMAGE_SIZE BW=0, BH=0, BC=0, BS=0;
						str.Format(_T("%s\\%s_ModelWndAlgModelMatchBlobContour#%d.BMP"), DebugFolder, ComponentName, WndIndex+1);							
						ImageAPI.CreatePointListImage(Contour, BW, BH, BS, BC, BP);
						ImageAPI.SaveImage(str, BW, BH, BS, BC, BP, true);
						JetMemory.free_func(BP);
					}
				#endif//_DEBUG
				}
			}
		}	
	}
	ResultCX = ResultX+nRoiX;
	ResultCY = ResultY+nRoiY;
	RoiOffsetX = ResultCX-dRoiCpx;
	RoiOffsetY = dRoiCpy-ResultCY;
	CadSkew = -ResultA;
	CadOffsetX = RoiOffsetX/ImageScale.x;
	CadOffsetY = RoiOffsetY/ImageScale.y;

	CadOffsetX += WndRectCalValue.x;
	CadOffsetY += WndRectCalValue.y;

	CadScaleX = ResultSX*100.0;
	CadScaleY = ResultSY*100.0;		
	if ( ResultS < 0.0 ) { ResultS = 0.0; }		
	JetMemory.free_func(PatImgPtr);	
	JetMemory.free_func(RoiImagePtr);

	bool   bMatchScore=true;
	double Reading=0.0, USL=0.0, LSL=0.0;
	bool   ApplyScale=false;
	Reading = ResultS;
	USL = GetAlgPatternSimilarityUSL();
	LSL = GetAlgPatternSimilarityLSL();
	SetAlgPatternSimilarityReading(Reading);	

	CString strResult;	
	bMatchScore = true;
	SetAlgResultReading1(Reading);	
	SetAlgResultText(_T("OK"));
	SetAlgResultID(RESULT_ID_OK);
	if ( Reading<LSL || Reading>USL )
	{
		bMatchScore = false;
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
	if ( NULL != ComponentPtr )
	{
		double ComponentAngle = ComponentPtr->GetComponentAngle();		
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
	}	*/
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
		USL = GetAlgScaleUSL()+1;
		LSL = GetAlgScaleLSL()-1;
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

	//套用至後面的檢測框
	if ( NResults>0 && true==bMatchScore ) 
	{
		//將Box變動提前，因為如果置後會導致UpdateModelInspectionPosRes用來當作參考的WndPtr不同。
		//當有設定邏輯 WND_LOGIC_NONE != WndLogicType ，會在其他地方執行 UpdateModelInspectionPosRes，
		//如果尺寸連動又設定成本體，檢測框跟本體框就會不同。
		CAOIBox *BoxPtr = WndPtr->GetWndBoxPtr();
		if (true == AlgScaleEnabled)
		{
			BoxPtr->ScaleBoxSizeRes(CadScaleX / 100.0, CadScaleY / 100.0);
		}
		BoxPtr->SkewBoxAngle(CadSkew_Self);
		BoxPtr->MoveBoxRes(CadOffsetX_Self, CadOffsetY_Self);

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
	}

	ExecAlgInspection_IPC(ModelPtr, ModelRgn, RoiRect, WndRect, UniFrameList);
	return true;
}
//-------------------------------------------------------------------------------------//