// AlgParam_OuterShort.cpp: implementation of the CAlgParam class.
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
bool CAlgParam::CheckOK_OuterShort_R(const TALG_PARAM_OUTER_SHORT &Param)
{
	if ( CheckOK_OuterShortUSL_R(Param) == false ) 
	{	return false; }
	if ( CheckOK_OuterShortLSL_R(Param) == false ) 
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_OuterShortUSL_R(const TALG_PARAM_OUTER_SHORT &Param)
{
	if ( Param.osLine_R.olReading > Param.osLine_R.olUSL ) 
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_OuterShortLSL_R(const TALG_PARAM_OUTER_SHORT &Param)
{
	if ( Param.osLine_R.olReading < Param.osLine_R.olLSL ) 
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_OuterShort_T(const TALG_PARAM_OUTER_SHORT &Param)
{
	if ( CheckOK_OuterShortUSL_T(Param) == false ) 
	{	return false; }
	if ( CheckOK_OuterShortLSL_T(Param) == false ) 
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_OuterShortUSL_T(const TALG_PARAM_OUTER_SHORT &Param)
{
	if ( Param.osLine_T.olReading > Param.osLine_T.olUSL ) 
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_OuterShortLSL_T(const TALG_PARAM_OUTER_SHORT &Param)
{
	if ( Param.osLine_T.olReading < Param.osLine_T.olLSL ) 
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_OuterShort_L(const TALG_PARAM_OUTER_SHORT &Param)
{
	if ( CheckOK_OuterShortUSL_L(Param) == false ) 
	{	return false; }
	if ( CheckOK_OuterShortLSL_L(Param) == false ) 
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_OuterShortUSL_L(const TALG_PARAM_OUTER_SHORT &Param)
{
	if ( Param.osLine_L.olReading > Param.osLine_L.olUSL ) 
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_OuterShortLSL_L(const TALG_PARAM_OUTER_SHORT &Param)
{
	if ( Param.osLine_L.olReading < Param.osLine_L.olLSL ) 
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_OuterShort_B(const TALG_PARAM_OUTER_SHORT &Param)
{
	if ( CheckOK_OuterShortUSL_B(Param) == false ) 
	{	return false; }
	if ( CheckOK_OuterShortLSL_B(Param) == false ) 
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_OuterShortUSL_B(const TALG_PARAM_OUTER_SHORT &Param)
{
	if ( Param.osLine_B.olReading > Param.osLine_B.olUSL ) 
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_OuterShortLSL_B(const TALG_PARAM_OUTER_SHORT &Param)
{
	if ( Param.osLine_B.olReading < Param.osLine_B.olLSL ) 
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::WriteAlgParamFile_OuterShort(const TALG_PARAM_OUTER_SHORT &osParam, CAOIFileIO &FileIO)//儲存外接短路參數
{		
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_OUTER_SHORT_START, 0) == false ) { return false; }

	if ( FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_OUTER_SHORT_ENABLED_R, osParam.osLine_R.olEnabled) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_OUTER_SHORT_RANGE_R, osParam.osLine_R.olRange) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_OUTER_SHORT_MODE_R, osParam.osLine_R.olMode) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_OUTER_SHORT_USL_R, osParam.osLine_R.olUSL) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_OUTER_SHORT_LSL_R, osParam.osLine_R.olLSL) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_OUTER_SHORT_EXT_MODE_R, osParam.osLine_R.olExtMode) == false ) { return false; }	
	
	if ( FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_OUTER_SHORT_ENABLED_T, osParam.osLine_T.olEnabled) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_OUTER_SHORT_RANGE_T, osParam.osLine_T.olRange) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_OUTER_SHORT_MODE_T, osParam.osLine_T.olMode) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_OUTER_SHORT_USL_T, osParam.osLine_T.olUSL) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_OUTER_SHORT_LSL_T, osParam.osLine_T.olLSL) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_OUTER_SHORT_EXT_MODE_T, osParam.osLine_T.olExtMode) == false ) { return false; }	

	if ( FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_OUTER_SHORT_ENABLED_L, osParam.osLine_L.olEnabled) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_OUTER_SHORT_RANGE_L, osParam.osLine_L.olRange) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_OUTER_SHORT_MODE_L, osParam.osLine_L.olMode) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_OUTER_SHORT_USL_L, osParam.osLine_L.olUSL) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_OUTER_SHORT_LSL_L, osParam.osLine_L.olLSL) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_OUTER_SHORT_EXT_MODE_L, osParam.osLine_L.olExtMode) == false ) { return false; }	

	if ( FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_OUTER_SHORT_ENABLED_B, osParam.osLine_B.olEnabled) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_OUTER_SHORT_RANGE_B, osParam.osLine_B.olRange) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_OUTER_SHORT_MODE_B, osParam.osLine_B.olMode) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_OUTER_SHORT_USL_B, osParam.osLine_B.olUSL) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_OUTER_SHORT_LSL_B, osParam.osLine_B.olLSL) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_OUTER_SHORT_EXT_MODE_B, osParam.osLine_B.olExtMode) == false ) { return false; }	

	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_OUTER_SHORT_END, 0) == false ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ReadAlgParamFile_OuterShort(TALG_PARAM_OUTER_SHORT &osParam, CAOIFileIO &FileIO)//載入外接短路參數
{
	int       index = 0;
	int       nValue = 0;
	while ( FileIO.CheckFileEnd()==false )
	{ 		
		if ( FileIO.LoadChunk(index) == false )		
		{	continue; }
		switch ( index )
		{
		case FILE_IO_ALG_PARAM_OUTER_SHORT_END://外接短路參數-終點
			return true;
			break;

		case FILE_IO_ALG_PARAM_OUTER_SHORT_ENABLED_R://啟用-R
			osParam.osLine_R.olEnabled = FileIO.GetData_BOL();
			break;
		case FILE_IO_ALG_PARAM_OUTER_SHORT_RANGE_R://範圍-R
			osParam.osLine_R.olRange = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_OUTER_SHORT_MODE_R://模式-R
			nValue = FileIO.GetData_INT();
			switch ( nValue )
			{
			case LINE_MODE_BRIGHT:
			case LINE_MODE_DARK:
				osParam.osLine_R.olMode = nValue;
				break;
			}
			break;
		case FILE_IO_ALG_PARAM_OUTER_SHORT_USL_R://上限-R
			osParam.osLine_R.olUSL = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_OUTER_SHORT_LSL_R://下限-R
			osParam.osLine_R.olLSL = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_OUTER_SHORT_EXT_MODE_R://延伸-R
			nValue = FileIO.GetData_INT();
			switch ( nValue )
			{
			case ALG_OUTER_SHORT_EXT_NONE:
			case ALG_OUTER_SHORT_EXT_LEFT:
			case ALG_OUTER_SHORT_EXT_RIGHT:
			case ALG_OUTER_SHORT_EXT_BOTH:
				osParam.osLine_R.olExtMode = (ALG_OUTER_SHORT_EXT_MODE)(nValue);
				break;
			}
			break;	

		case FILE_IO_ALG_PARAM_OUTER_SHORT_ENABLED_T://啟用-T
			osParam.osLine_T.olEnabled = FileIO.GetData_BOL();
			break;
		case FILE_IO_ALG_PARAM_OUTER_SHORT_RANGE_T://範圍-T
			osParam.osLine_T.olRange = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_OUTER_SHORT_MODE_T://模式-T
			nValue = FileIO.GetData_INT();
			switch ( nValue )
			{
			case LINE_MODE_BRIGHT:
			case LINE_MODE_DARK:
				osParam.osLine_T.olMode = nValue;
				break;
			}
			break;
		case FILE_IO_ALG_PARAM_OUTER_SHORT_USL_T://上限-T
			osParam.osLine_T.olUSL = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_OUTER_SHORT_LSL_T://下限-T
			osParam.osLine_T.olLSL = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_OUTER_SHORT_EXT_MODE_T://延伸-T
			nValue = FileIO.GetData_INT();
			switch ( nValue )
			{
			case ALG_OUTER_SHORT_EXT_NONE:
			case ALG_OUTER_SHORT_EXT_LEFT:
			case ALG_OUTER_SHORT_EXT_RIGHT:
			case ALG_OUTER_SHORT_EXT_BOTH:
				osParam.osLine_T.olExtMode = (ALG_OUTER_SHORT_EXT_MODE)(nValue);
				break;
			}
			break;

		case FILE_IO_ALG_PARAM_OUTER_SHORT_ENABLED_L://啟用-L
			osParam.osLine_L.olEnabled = FileIO.GetData_BOL();
			break;
		case FILE_IO_ALG_PARAM_OUTER_SHORT_RANGE_L://範圍-L
			osParam.osLine_L.olRange = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_OUTER_SHORT_MODE_L://模式-L
			nValue = FileIO.GetData_INT();
			switch ( nValue )
			{
			case LINE_MODE_BRIGHT:
			case LINE_MODE_DARK:
				osParam.osLine_L.olMode = nValue;
				break;
			}
			break;
		case FILE_IO_ALG_PARAM_OUTER_SHORT_USL_L://上限-L
			osParam.osLine_L.olUSL = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_OUTER_SHORT_LSL_L://下限-L
			osParam.osLine_L.olLSL = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_OUTER_SHORT_EXT_MODE_L://延伸-L
			nValue = FileIO.GetData_INT();
			switch ( nValue )
			{
			case ALG_OUTER_SHORT_EXT_NONE:
			case ALG_OUTER_SHORT_EXT_LEFT:
			case ALG_OUTER_SHORT_EXT_RIGHT:
			case ALG_OUTER_SHORT_EXT_BOTH:
				osParam.osLine_L.olExtMode = (ALG_OUTER_SHORT_EXT_MODE)(nValue);
				break;
			}
			break;

		case FILE_IO_ALG_PARAM_OUTER_SHORT_ENABLED_B://啟用-B
			osParam.osLine_B.olEnabled = FileIO.GetData_BOL();
			break;
		case FILE_IO_ALG_PARAM_OUTER_SHORT_RANGE_B://範圍-B
			osParam.osLine_B.olRange = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_OUTER_SHORT_MODE_B://模式-B
			nValue = FileIO.GetData_INT();
			switch ( nValue )
			{
			case LINE_MODE_BRIGHT:
			case LINE_MODE_DARK:
				osParam.osLine_B.olMode = nValue;
				break;
			}
			break;
		case FILE_IO_ALG_PARAM_OUTER_SHORT_USL_B://上限-B
			osParam.osLine_B.olUSL = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_OUTER_SHORT_LSL_B://下限-B
			osParam.osLine_B.olLSL = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_OUTER_SHORT_EXT_MODE_B://延伸-B
			nValue = FileIO.GetData_INT();
			switch ( nValue )
			{
			case ALG_OUTER_SHORT_EXT_NONE:
			case ALG_OUTER_SHORT_EXT_LEFT:
			case ALG_OUTER_SHORT_EXT_RIGHT:
			case ALG_OUTER_SHORT_EXT_BOTH:
				osParam.osLine_B.olExtMode = (ALG_OUTER_SHORT_EXT_MODE)(nValue);
				break;
			}
			break;

		default:
		#ifdef _DEBUG
			index = index;
		#endif//_DEBUG
			break;
		}
	}
	FileIO.SetErrorString(_T("Error, ReadAlgParamFile_OuterShort Fault"));
	return false;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ExecAlgInspection_OuterShort(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList)
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
	TALG_PARAM_OUTER_SHORT &osParam = GetAlgParamOuterShort();
	const int RoiW = RoiRect.right-RoiRect.left;
	const int RoiH = RoiRect.bottom-RoiRect.top;
	const int RoiRectSize = (int)((RoiRect.right-RoiRect.left)*(RoiRect.bottom-RoiRect.top));
	
	CString       str;
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
	
	RECT   CalcRect={0};	
	ALG_OUTER_SHORT_EXT_MODE LineExtMode;
	double Ave = 0.0, Value=0, Ratio=0, USL=0.0, LSL=0.0;
	size_t i=0, j=0, k=0;
	unsigned int  Count=0;
	unsigned int  index=0;	

#ifdef _DEBUG	
	bool         bSave = true;
	CString      ComponentName;
	CString      DebugFolder=GetAlgDebugFolder();	
	if ( NULL != ComponentPtr )
	{	ComponentName = ComponentPtr->GetComponentFullName(); }		
	if ( true == bSave )
	{	
		str.Format(_T("%s\\%s_ModelWndAlgOuterShort#%d_MaskImage.PNG"), DebugFolder, ComponentName, WndIndex+1);
		ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, MaskBitCount, MaskPtr, true);
	}
#endif//_DEBUG
	//在右側的短路-X
	if ( true == osParam.osLine_R.olEnabled )
	{	
		CalcRect = RoiRect;				
		CalcRect.left = WndRect.right;
		LineExtMode=osParam.osLine_R.olExtMode;
		switch ( LineExtMode )
		{
		case ALG_OUTER_SHORT_EXT_LEFT:	CalcRect.bottom  = WndRect.bottom;	break;
		case ALG_OUTER_SHORT_EXT_RIGHT:	CalcRect.top  = WndRect.top;		break;
		case ALG_OUTER_SHORT_EXT_NONE:
			CalcRect.top  = WndRect.top;
			CalcRect.bottom  = WndRect.bottom;
			break;
		default:
		case ALG_OUTER_SHORT_EXT_BOTH:			
			break;
		}
		const int CalcRoiW = CalcRect.right-CalcRect.left;
		const int CalcRoiH = CalcRect.bottom-CalcRect.top;

		const int LineMode = osParam.osLine_R.olMode;
		const double LineRange = osParam.osLine_R.olRange;
		int   nLineRange = JetAPI::Floor(LineRange*ImageScale.y);
		if ( nLineRange < 1 ) { nLineRange = 1; }		
		if ( nLineRange > CalcRoiH ) { nLineRange = CalcRoiH; }
		
		int LineCount=0;
		int MaxLineCount = 0;		
		unsigned char PassChar = 0;
		if ( LINE_MODE_DARK == LineMode )
		{	PassChar = 255; }
		else
		{	PassChar = 0; }
		for ( i=CalcRect.top; i<CalcRect.bottom-nLineRange+1; i++ )
		{			
			LineCount = 0;
			for ( j=CalcRect.left; j<CalcRect.right; j++ )
			{
				for ( k=i; k<i+nLineRange; k++ )
				{
					index = k*MaskStep;
					if ( PassChar == MaskPtr[index+j] ) { continue; }				
					LineCount ++;
					break;
				}
			}
			if ( MaxLineCount < LineCount ) { MaxLineCount = LineCount; }
		}
		if ( MaxLineCount > CalcRoiW ) { MaxLineCount = CalcRoiW; }
		Ratio = MaxLineCount;
		osParam.osLine_R.olReading = 100.0*Ratio/CalcRoiW;
	}

	//在上方的短路
	if ( true == osParam.osLine_T.olEnabled )
	{		
		CalcRect = RoiRect;
		CalcRect.bottom = WndRect.top;
		LineExtMode=osParam.osLine_T.olExtMode;
		switch ( LineExtMode )
		{
		case ALG_OUTER_SHORT_EXT_LEFT:	CalcRect.right = WndRect.right;	break;
		case ALG_OUTER_SHORT_EXT_RIGHT:	CalcRect.left = WndRect.left;	break;
		case ALG_OUTER_SHORT_EXT_NONE:
			CalcRect.left = WndRect.left;
			CalcRect.right = WndRect.right;
			break;
		default:
		case ALG_OUTER_SHORT_EXT_BOTH:			
			break;
		}
		const int CalcRoiW = CalcRect.right-CalcRect.left;
		const int CalcRoiH = CalcRect.bottom-CalcRect.top;

		const int LineMode = osParam.osLine_T.olMode;
		const double LineRange = osParam.osLine_T.olRange;
		int   nLineRange = JetAPI::Floor(LineRange*ImageScale.x);
		if ( nLineRange < 1 ) { nLineRange = 1; }		
		if ( nLineRange > CalcRoiW ) { nLineRange = CalcRoiW; }

		int LineCount=0;
		int MaxLineCount = 0;
		unsigned char PassChar = 0;
		if ( LINE_MODE_DARK == LineMode )
		{	PassChar = 255; }
		else
		{	PassChar = 0; }
		for ( j=CalcRect.left; j<CalcRect.right-nLineRange+1; j++ )
		{			
			LineCount = 0;			
			for ( i=CalcRect.top; i<CalcRect.bottom; i++ )
			{
				index = i*MaskStep;
				for ( k=j; k<j+nLineRange; k++ )
				{	
					if ( PassChar == MaskPtr[index+k] ) { continue; }				
					LineCount ++;
					break;
				}
			}
			if ( MaxLineCount < LineCount ) { MaxLineCount = LineCount; }
		}
		if ( MaxLineCount > CalcRoiH ) { MaxLineCount = CalcRoiH; }
		Ratio = MaxLineCount;
		osParam.osLine_T.olReading = 100.0*Ratio/CalcRoiH;
	}	
	
	//在左側的短路-X
	if ( true == osParam.osLine_L.olEnabled )
	{	
		CalcRect = RoiRect;		
		CalcRect.right = WndRect.left;
		LineExtMode=osParam.osLine_L.olExtMode;
		switch ( LineExtMode )
		{
		case ALG_OUTER_SHORT_EXT_LEFT:	CalcRect.top  = WndRect.top;	break;
		case ALG_OUTER_SHORT_EXT_RIGHT:	CalcRect.bottom = WndRect.bottom;	break;
		case ALG_OUTER_SHORT_EXT_NONE:
			CalcRect.top  = WndRect.top;
			CalcRect.bottom  = WndRect.bottom;
			break;
		default:
		case ALG_OUTER_SHORT_EXT_BOTH:			
			break;
		}	
		const int CalcRoiW = CalcRect.right-CalcRect.left;
		const int CalcRoiH = CalcRect.bottom-CalcRect.top;

		const int LineMode = osParam.osLine_L.olMode;
		const double LineRange = osParam.osLine_L.olRange;
		int   nLineRange = JetAPI::Floor(LineRange*ImageScale.y);
		if ( nLineRange < 1 ) { nLineRange = 1; }		
		if ( nLineRange > CalcRoiH ) { nLineRange = CalcRoiH; }
		
		int LineCount=0;
		int MaxLineCount = 0;		
		unsigned char PassChar = 0;
		if ( LINE_MODE_DARK == LineMode )
		{	PassChar = 255; }
		else
		{	PassChar = 0; }
		for ( i=CalcRect.top; i<CalcRect.bottom-nLineRange+1; i++ )
		{			
			LineCount = 0;
			for ( j=CalcRect.left; j<CalcRect.right; j++ )
			{
				for ( k=i; k<i+nLineRange; k++ )
				{
					index = k*MaskStep;
					if ( PassChar == MaskPtr[index+j] ) { continue; }				
					LineCount ++;
					break;
				}
			}
			if ( MaxLineCount < LineCount ) { MaxLineCount = LineCount; }
		}
		if ( MaxLineCount > CalcRoiW ) { MaxLineCount = CalcRoiW; }
		Ratio = MaxLineCount;
		osParam.osLine_L.olReading = 100.0*Ratio/CalcRoiW;
	}

	//在下方的短路
	if ( true == osParam.osLine_B.olEnabled )
	{		
		CalcRect = RoiRect;
		CalcRect.top = WndRect.bottom;
		LineExtMode=osParam.osLine_B.olExtMode;
		switch ( LineExtMode )
		{
		case ALG_OUTER_SHORT_EXT_LEFT:	CalcRect.left = WndRect.left;	break;
		case ALG_OUTER_SHORT_EXT_RIGHT:	CalcRect.right = WndRect.right;	break;
		case ALG_OUTER_SHORT_EXT_NONE:
			CalcRect.left = WndRect.left;
			CalcRect.right = WndRect.right;
			break;
		default:
		case ALG_OUTER_SHORT_EXT_BOTH:			
			break;
		}
		const int CalcRoiW = CalcRect.right-CalcRect.left;
		const int CalcRoiH = CalcRect.bottom-CalcRect.top;

		const int LineMode = osParam.osLine_B.olMode;
		const double LineRange = osParam.osLine_B.olRange;
		int   nLineRange = JetAPI::Floor(LineRange*ImageScale.x);
		if ( nLineRange < 1 ) { nLineRange = 1; }		
		if ( nLineRange > CalcRoiW ) { nLineRange = CalcRoiW; }

		int LineCount=0;
		int MaxLineCount = 0;
		unsigned char PassChar = 0;
		if ( LINE_MODE_DARK == LineMode )
		{	PassChar = 255; }
		else
		{	PassChar = 0; }
		for ( j=CalcRect.left; j<CalcRect.right-nLineRange+1; j++ )
		{			
			LineCount = 0;			
			for ( i=CalcRect.top; i<CalcRect.bottom; i++ )
			{
				index = i*MaskStep;
				for ( k=j; k<j+nLineRange; k++ )
				{	
					if ( PassChar == MaskPtr[index+k] ) { continue; }				
					LineCount ++;
					break;
				}
			}
			if ( MaxLineCount < LineCount ) { MaxLineCount = LineCount; }
		}
		if ( MaxLineCount > CalcRoiH ) { MaxLineCount = CalcRoiH; }
		Ratio = MaxLineCount;
		osParam.osLine_B.olReading = 100.0*Ratio/CalcRoiH;
	}	

	CString strResult;	
	SetAlgResultReading1(Ratio);	
	SetAlgResultText(_T("OK"));

	//Judge OK/NG	
	CAOIBox      ResBox;	
	TREGION4D    WndRgn;
	TREGION4D    WndExtendRgn;
	TREGION4D    ResBoxRgn;
	RESULT_ID    RoiResultID;
	CString      Key = AOIDataDefine.GetThroughText();
	WndPtr->GetWndRegionRes(WndRgn);
	WndPtr->GetWndExtendRegionRes(WndExtendRgn);

	SetAlgResultID(RESULT_ID_OK);
	if ( true == osParam.osLine_R.olEnabled )
	{
		USL = osParam.osLine_R.olUSL;
		LSL = osParam.osLine_R.olLSL;
		Ave = osParam.osLine_R.olReading;
		str.Format(_T("%.0f%%"), Ave);
		RoiResultID = RESULT_ID_OK;
		if ( Ave>USL || Ave<LSL )
		{	
			RoiResultID = RESULT_ID_NG;			
			strResult.Format(_T("R-%s:%.0f (%.0f~%.0f)"), Key, Ave, LSL, USL);
			SetAlgResultID(RESULT_ID_NG);
			SetAlgResultText(strResult);
		}
		ResBoxRgn = WndExtendRgn;
		ResBoxRgn.minX = WndRgn.maxX;
		LineExtMode=osParam.osLine_R.olExtMode;		
		switch ( LineExtMode )
		{
		case ALG_OUTER_SHORT_EXT_LEFT:	ResBoxRgn.minY = WndRgn.minY;	break;
		case ALG_OUTER_SHORT_EXT_RIGHT:	ResBoxRgn.maxY = WndRgn.maxY;	break;
		case ALG_OUTER_SHORT_EXT_NONE:
			ResBoxRgn.minY = WndRgn.minY;
			ResBoxRgn.maxY = WndRgn.maxY;
			break;
		default:
		case ALG_OUTER_SHORT_EXT_BOTH:			
			break;
		}
		ResBox.SetBoxRegion(ResBoxRgn);
		ResBox.SetBoxResultID(RoiResultID);
		ResBox.SetBoxResultValue(Ave);
		ResBox.SetBoxResultText(str);
		ResBox.SetBoxResultTextVisibled(true);
		WndPtr->AddWndResultBox(ResBox);
	}
	if ( true == osParam.osLine_T.olEnabled )
	{
		USL = osParam.osLine_T.olUSL;
		LSL = osParam.osLine_T.olLSL;
		Ave = osParam.osLine_T.olReading;
		str.Format(_T("%.0f%%"), Ave);
		RoiResultID = RESULT_ID_OK;
		if ( Ave>USL || Ave<LSL )
		{	
			RoiResultID = RESULT_ID_NG;			
			strResult.Format(_T("T-%s:%.0f (%.0f~%.0f)"), Key, Ave, LSL, USL);
			SetAlgResultID(RESULT_ID_NG);
			SetAlgResultText(strResult);
		}
		ResBoxRgn = WndExtendRgn;
		ResBoxRgn.minY = WndRgn.maxY;
		LineExtMode=osParam.osLine_T.olExtMode;
		switch ( LineExtMode )
		{
		case ALG_OUTER_SHORT_EXT_LEFT:	ResBoxRgn.maxX = WndRgn.maxX;	break;
		case ALG_OUTER_SHORT_EXT_RIGHT:	ResBoxRgn.minX = WndRgn.minX;	break;
		case ALG_OUTER_SHORT_EXT_NONE:
			ResBoxRgn.minX = WndRgn.minX;
			ResBoxRgn.maxX = WndRgn.maxX;
			break;
		default:
		case ALG_OUTER_SHORT_EXT_BOTH:			
			break;
		}		
		ResBox.SetBoxRegion(ResBoxRgn);
		ResBox.SetBoxResultID(RoiResultID);
		ResBox.SetBoxResultValue(Ave);
		ResBox.SetBoxResultText(str);
		ResBox.SetBoxResultTextVisibled(true);
		WndPtr->AddWndResultBox(ResBox);
	}

	if ( true == osParam.osLine_L.olEnabled )
	{
		USL = osParam.osLine_L.olUSL;
		LSL = osParam.osLine_L.olLSL;
		Ave = osParam.osLine_L.olReading;
		str.Format(_T("%.0f%%"), Ave);
		RoiResultID = RESULT_ID_OK;
		if ( Ave>USL || Ave<LSL )
		{	
			RoiResultID = RESULT_ID_NG;			
			strResult.Format(_T("L-%s:%.0f (%.0f~%.0f)"), Key, Ave, LSL, USL);
			SetAlgResultID(RESULT_ID_NG);
			SetAlgResultText(strResult);
		}
		ResBoxRgn = WndExtendRgn;
		ResBoxRgn.maxX = WndRgn.minX;
		LineExtMode=osParam.osLine_L.olExtMode;
		switch ( LineExtMode )
		{
		case ALG_OUTER_SHORT_EXT_LEFT:	ResBoxRgn.maxY = WndRgn.maxY;	break;
		case ALG_OUTER_SHORT_EXT_RIGHT:	ResBoxRgn.minY = WndRgn.minY;	break;
		case ALG_OUTER_SHORT_EXT_NONE:
			ResBoxRgn.minY = WndRgn.minY;
			ResBoxRgn.maxY = WndRgn.maxY;
			break;
		default:
		case ALG_OUTER_SHORT_EXT_BOTH:			
			break;
		}	
		ResBox.SetBoxRegion(ResBoxRgn);
		ResBox.SetBoxResultID(RoiResultID);
		ResBox.SetBoxResultValue(Ave);
		ResBox.SetBoxResultText(str);
		ResBox.SetBoxResultTextVisibled(true);
		WndPtr->AddWndResultBox(ResBox);
	}
	if ( true == osParam.osLine_B.olEnabled )
	{
		USL = osParam.osLine_B.olUSL;
		LSL = osParam.osLine_B.olLSL;
		Ave = osParam.osLine_B.olReading;
		str.Format(_T("%.0f%%"), Ave);
		RoiResultID = RESULT_ID_OK;
		if ( Ave>USL || Ave<LSL )
		{	
			RoiResultID = RESULT_ID_NG;			
			strResult.Format(_T("B-%s:%.0f (%.0f~%.0f)"), Key, Ave, LSL, USL);
			SetAlgResultID(RESULT_ID_NG);
			SetAlgResultText(strResult);
		}
		ResBoxRgn = WndExtendRgn;
		ResBoxRgn.maxY = WndRgn.minY;
		LineExtMode=osParam.osLine_B.olExtMode;
		switch ( LineExtMode )
		{
		case ALG_OUTER_SHORT_EXT_LEFT:	ResBoxRgn.minX = WndRgn.minX;	break;
		case ALG_OUTER_SHORT_EXT_RIGHT:	ResBoxRgn.maxX = WndRgn.maxX;	break;
		case ALG_OUTER_SHORT_EXT_NONE:
			ResBoxRgn.minX = WndRgn.minX;
			ResBoxRgn.maxX = WndRgn.maxX;
			break;
		default:
		case ALG_OUTER_SHORT_EXT_BOTH:			
			break;
		}	
		ResBox.SetBoxRegion(ResBoxRgn);
		ResBox.SetBoxResultID(RoiResultID);
		ResBox.SetBoxResultValue(Ave);
		ResBox.SetBoxResultText(str);
		ResBox.SetBoxResultTextVisibled(true);
		WndPtr->AddWndResultBox(ResBox);
	}
	WndPtr->SetWndResultID(m_AlgResultID);
	WndPtr->SetWndResultText(m_AlgResultText);
	SaveAlgDefectImage(MaskW, MaskH, MaskStep, MaskBitCount, GrayPtr, MaskPtr, WndRect);
	JetMemory.free_func(GrayPtr);
	JetMemory.free_func(MaskPtr);
	return true;
}
//-------------------------------------------------------------------------------------//