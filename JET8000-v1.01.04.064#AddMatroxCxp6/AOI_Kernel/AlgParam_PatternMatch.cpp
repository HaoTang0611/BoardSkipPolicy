// AlgParam_PatternMatch.cpp: implementation of the CAlgParam class.
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
bool CAlgParam::CheckAlgPatternFileUsed(ALG_TYPE Type)
{
	bool Used = false;
	switch ( Type )
	{
	case ALG_IMAGE_MATCH:
	case ALG_CHAR_VERIFY:
	case ALG_FD_MATCH:
	case ALG_PIXEL_COMPARE:
	case ALG_MEASURE_SIP_DISTANCE:
		Used = true;
		break;
	}
	return Used;
}
//-------------------------------------------------------------------------------------//
int CAlgParam::ExtractAlgPatternIndex(LPCTSTR ImageName)
{
	size_t i=0, j=0;
	TCHAR IndexS[32]=_T("");
	const size_t BaseLen = ::_tcslen(_T("IMG"));
	const size_t Len = ::_tcslen(ImageName);
	if ( Len < (4+BaseLen+1) ) { return -1; }

	j = 0;
	for ( i=Len-BaseLen; i<Len-4; i++ )
	{	IndexS[j++] = ImageName[i];	}
	IndexS[j++] = _T('\0');
	return _ttoi(IndexS);
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_PatternMatchOffsetX(const CAlgParam &Param)
{
	if ( CheckOK_PatternMatchOffsetXUSL(Param) == false )
	{	return false; }
	if ( CheckOK_PatternMatchOffsetXLSL(Param) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_PatternMatchOffsetXUSL(const CAlgParam &Param)
{
	if ( Param.m_AlgOffsetXReading > Param.m_AlgOffsetXUSL ) 
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_PatternMatchOffsetXLSL(const CAlgParam &Param)
{
	if ( Param.m_AlgOffsetXReading < Param.m_AlgOffsetXLSL ) 
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_PatternMatchOffsetY(const CAlgParam &Param)
{
	if ( CheckOK_PatternMatchOffsetYUSL(Param) == false )
	{	return false; }
	if ( CheckOK_PatternMatchOffsetYLSL(Param) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_PatternMatchOffsetYUSL(const CAlgParam &Param)
{
	if ( Param.m_AlgOffsetYReading > Param.m_AlgOffsetYUSL ) 
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_PatternMatchOffsetYLSL(const CAlgParam &Param)
{
	if ( Param.m_AlgOffsetYReading < Param.m_AlgOffsetYLSL ) 
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_PatternMatchOffsetL(const CAlgParam &Param)
{
	if ( CheckOK_PatternMatchOffsetLUSL(Param) == false )
	{	return false; }
	if ( CheckOK_PatternMatchOffsetLLSL(Param) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_PatternMatchOffsetLUSL(const CAlgParam &Param)
{
	if ( Param.m_AlgOffsetLReading > (Param.m_AlgOffsetLUSL) ) 
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_PatternMatchOffsetLLSL(const CAlgParam &Param)
{
	if ( Param.m_AlgOffsetLReading < (Param.m_AlgOffsetLLSL) ) 
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_PatternMatchOffsetA(const CAlgParam &Param)
{
	if ( CheckOK_PatternMatchOffsetAUSL(Param) == false )
	{	return false; }
	if ( CheckOK_PatternMatchOffsetALSL(Param) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_PatternMatchOffsetAUSL(const CAlgParam &Param)
{
	if ( Param.m_AlgOffsetAReading > (Param.m_AlgOffsetAUSL) ) 
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_PatternMatchOffsetALSL(const CAlgParam &Param)
{
	if ( Param.m_AlgOffsetAReading < (Param.m_AlgOffsetALSL) ) 
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_PatternMatchSkewAngle(const CAlgParam &Param)
{
	if ( CheckOK_PatternMatchSkewAngleUSL(Param) == false )
	{	return false; }
	if ( CheckOK_PatternMatchSkewAngleLSL(Param) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_PatternMatchSkewAngleUSL(const CAlgParam &Param)
{
	if ( Param.m_AlgSkewReading > Param.m_AlgSkewUSL ) 
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_PatternMatchSkewAngleLSL(const CAlgParam &Param)
{
	if ( Param.m_AlgSkewReading < Param.m_AlgSkewLSL ) 
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_PatternMatchScale(const CAlgParam &Param)
{
	if ( CheckOK_PatternMatchScaleUSL(Param) == false )
	{	return false; }
	if ( CheckOK_PatternMatchScaleLSL(Param) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_PatternMatchScaleUSL(const CAlgParam &Param)
{
	if ( Param.m_AlgScaleXReading > Param.m_AlgScaleUSL ) 
	{	return false; }
	if ( Param.m_AlgScaleYReading > Param.m_AlgScaleUSL ) 
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_PatternMatchScaleLSL(const CAlgParam &Param)
{
	if ( Param.m_AlgScaleXReading < Param.m_AlgScaleLSL ) 
	{	return false; }
	if ( Param.m_AlgScaleYReading < Param.m_AlgScaleLSL ) 
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_PatternMatchScore(const CAlgParam &Param)
{
	if ( CheckOK_PatternMatchScoreUSL(Param) == false )
	{	return false; }
	if ( CheckOK_PatternMatchScoreLSL(Param) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_PatternMatchScoreUSL(const CAlgParam &Param)
{
	if ( Param.m_AlgPatternSimilarityReading > Param.m_AlgPatternSimilarityUSL ) 
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_PatternMatchScoreLSL(const CAlgParam &Param)
{
	if ( Param.m_AlgPatternSimilarityReading < Param.m_AlgPatternSimilarityLSL ) 
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::WriteAlgParamFile_PatternParam(CAlgParam *AlgParamPtr, CAOIFileIO &FileIO)//纗妓狾把计
{
	size_t         i=0;	
	CPatternParam *PatParamPtr = NULL;
	const size_t   PatternCount = AlgParamPtr->GetAlgPatternCount();
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_PATTERN_COUNT, AlgParamPtr->GetAlgPatternCount()) == false ) { return false; }
	if ( FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_PATTERN_ELABLED, AlgParamPtr->GetAlgPatternFileUsed()) == false ) { return false; }	
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_PATTERN_SIMILARITY_USL, AlgParamPtr->GetAlgPatternSimilarityUSL()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_PATTERN_SIMILARITY_LSL, AlgParamPtr->GetAlgPatternSimilarityLSL()) == false ) { return false; }	
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_PATTERN_MIN_AREA, AlgParamPtr->GetAlgPatternMinReducedArea()) == false ) { return false; }		
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_PATTERN_POLARITY, AlgParamPtr->GetAlgPatternPolarity()) == false ) { return false; }			
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_PATTERN_FINAL_REDUCTION, AlgParamPtr->GetAlgPatternFinalReduction()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_PATTERN_ANGLE_EXPAND, AlgParamPtr->GetAlgPatternAngleExpand()) == false ) { return false; }	
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_PATTERN_SCALE_EXPAND, AlgParamPtr->GetAlgPatternScaleExpand()) == false ) { return false; }		
	if ( FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_PATTERN_SCALE_ISOTROPIC, AlgParamPtr->GetAlgPatternScaleIsotropic()) == false ) { return false; }			
	if ( FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_PATTERN_ADVANCED_LEARNING, AlgParamPtr->GetAlgPatternAdvancedLearning()) == false ) { return false; }
	
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_PATTERN_NODE, 0) == false ) { return false; }	
	for ( i=0; i<PatternCount; i++ )
	{
		PatParamPtr = AlgParamPtr->GetAlgPatternParamPtr(i, false);
		if ( NULL == PatParamPtr ) { continue; }
		if ( PatParamPtr->WritePatternParamFile(FileIO) == false ) { return false; }		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ReadAlgParamFile_PatternParam(CAlgParam *AlgParamPtr, CAOIFileIO &FileIO)//更妓狾把计把计
{
	int       index = 0;
	while ( FileIO.CheckFileEnd()==false )
	{ 		
		if ( FileIO.LoadChunk(index) == false )		
		{	continue; }
		switch ( index )
		{
		default:
		#ifdef _DEBUG
			index = index;
		#endif//_DEBUG
			break;
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//