// AlgParam_AiModel.cpp: implementation of the CAlgParam class.
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
bool CAlgParam::WriteAlgParamFile_AiModel(const TALG_PARAM_AI_MODEL &aiParam, CAOIFileIO &FileIO)//纗AI家把计
{
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_AI_MODEL_START, 0) == false ) { return false; }	
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_AI_MODEL_AI_MODEL_ID, aiParam.aiModelID) == false ) { return false; }	
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_AI_MODEL_CONFIDENCE_THRESHOLD, aiParam.aiConfidenceThreshold) == false ) { return false; }	
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_AI_MODEL_PATTERN_ANGLE, aiParam.aiPatternAngle) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_AI_MODEL_CHAR_MATCH_NUM_THRESHOLD, aiParam.aiCharMatchNumThreshold) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_AI_MODEL_CHAR_NUM_UPPER_THRESHOLD, aiParam.aiCharNumUpperThreshold) == false ) { return false; }	

	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_AI_MODEL_END, 0) == false ) { return false; }	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ReadAlgParamFile_AiModel(TALG_PARAM_AI_MODEL &aiParam, CAOIFileIO &FileIO)//更AI家把计
{
	int       index = 0;
	while ( FileIO.CheckFileEnd()==false )
	{ 		
		if ( FileIO.LoadChunk(index) == false )		
		{	continue; }
		switch ( index )
		{
		case FILE_IO_ALG_PARAM_AI_MODEL_AI_MODEL_ID://AI家絪腹
			aiParam.aiModelID = (ALG_AI_MODEL_ID)(FileIO.GetData_INT());
			break;
		case FILE_IO_ALG_PARAM_AI_MODEL_CONFIDENCE_THRESHOLD://獺み恢
			aiParam.aiConfidenceThreshold = (float)(FileIO.GetData_DBL());
			break;
		case FILE_IO_ALG_PARAM_AI_MODEL_PATTERN_ANGLE://妓狾à
			aiParam.aiPatternAngle = (float)(FileIO.GetData_DBL());
			break;
		case FILE_IO_ALG_PARAM_AI_MODEL_CHAR_MATCH_NUM_THRESHOLD://じ才计秖恢
			aiParam.aiCharMatchNumThreshold = FileIO.GetData_INT();
			break;
		case FILE_IO_ALG_PARAM_AI_MODEL_CHAR_NUM_UPPER_THRESHOLD://じ计秖恢
			aiParam.aiCharNumUpperThreshold = FileIO.GetData_INT();
			break;
		case FILE_IO_ALG_PARAM_AI_MODEL_END://AI家把计-沧翴
			return true;
		default:
		#ifdef _DEBUG
			index = index;
		#endif//_DEBUG
			break;		
		}
	}
	FileIO.SetErrorString(_T("Error, ReadAlgParamFile_AiModel Fault"));
	return false;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ExecAlgInspection_AiModel(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList)
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
	RECT             WndRoiRect;
	TREGION4D        WndRegion;
	TREGION4D        WndRoiRegion;
	CAOIWndRoi      *WndRoiPtr = NULL;		
	CString          strResult=GetAlgResultText();
	const unsigned int WndIndex = WndPtr->GetWndIndex();	
	const size_t     WndRoiCount = WndPtr->GetWndRoiWndCount();		
	TUNI_FRAME      *UniFramePtr  = &(UniFrameList[ImageFrameIndex]);
	const IMAGE_SIZE FrameImageW    = UniFramePtr->ImageW;
	const IMAGE_SIZE FrameImageH    = UniFramePtr->ImageH;
	const IMAGE_SIZE FrameImageStep = UniFramePtr->ImageStep;
	const IMAGE_SIZE FrameBitCount  = UniFramePtr->BitCount;		
	TPOINT2D         ImageScale     = ModelPtr->GetModelImageScale();
	TALG_PARAM_AI_MODEL &aiParam  = GetAlgParamAiModel();		
	const double     SpaceRatio = AOIDataCollect.GetSpaceToGrayRatio();
	const IMAGE_SIZE RoiW = RoiRect.right-RoiRect.left;
	const IMAGE_SIZE RoiH = RoiRect.bottom-RoiRect.top;	
	const IMAGE_SIZE RoiRectSize = (int)((RoiRect.right-RoiRect.left)*(RoiRect.bottom-RoiRect.top));	
	const RESULT_ID ResultID = AOIDataCollect.MapAiResultIDToResultID((AI_RESULT_ID)(aiParam.aiResultID));

	const size_t CharCount=aiParam.aiOcrCharList.size();

	switch ( aiParam.aiResultID )
	{
	case AI_RESULT_ID_OK:		strResult = _T("AI-OK"); break;
	case AI_RESULT_ID_NG:		strResult = _T("AI-NG"); break;
	case AI_RESULT_ID_BYPASS:	strResult = _T("AI-Bypass"); break;
	case AI_RESULT_ID_EXCEPTION:strResult = CString(aiParam.aiResultText.c_str()); break;
	default:
	case AI_RESULT_ID_NONE:		
		break;
	}

	RECT CharRect;
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
	WndPtr->ClearWndResultBoxList();
	for ( size_t i=0; i<CharCount; i++ )
	{		
		const TALG_PARAM_AI_CHAR &AiChar=aiParam.aiOcrCharList[i];
		const float CharScore = AiChar.acCharConfidence*100.0f;
		
		CString Char(AiChar.acChar.c_str());
		CharRect = AiChar.acCharRect;
		CAOIModel::CalcModelBoxRectRegion(CharRect, FrameImageW, FrameImageH, RgnCp, ImageScale, ImageCp, ResBoxRgn);
		ResBox.SetBoxRegion(ResBoxRgn);				
		ResBox.SetBoxResultID(ResultID);
		
		str.Format(_T("%s-%.0f%%"), Char, CharScore);
		ResBox.SetBoxResultText(str);
		ResBox.SetBoxResultTextVisibled(true);
		ResBox.SetBoxResultValue(CharScore);
		WndPtr->AddWndResultBox(ResBox);
	}
	
	SetAlgResultID(ResultID);
	SetAlgResultText(strResult);		
	WndPtr->SetWndResultID(m_AlgResultID);
	WndPtr->SetWndResultText(m_AlgResultText);	
	//SaveAlgDefectImage(MaskW, MaskH, MaskStep, MaskBitCount, GrayPtr, MaskPtr, WndRect);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ExecAlgInspection_AiModel_Public(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList)
{
	return ExecAlgInspection_AiModel(ModelPtr, ModelRgn, RoiRect, WndRect, UniFrameList);
}
//-------------------------------------------------------------------------------------//