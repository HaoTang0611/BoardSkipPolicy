// AlgParam_BodyTilt.cpp: implementation of the CAlgParam class.
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
CString CAlgParam::GetAlgTiltCellModeText(ALG_TILE_CELL_MODE CellMode)
{
	CString str;
	switch ( CellMode )
	{
	case ALG_TILE_CELL_HOR:
		str = _T("水平兩邊");
		break;
	case ALG_TILE_CELL_VER:
		str = _T("垂直兩邊");
		break;
	case ALG_TILE_CELL_CORNER:
		str = _T("四個角落");
		break;
	case ALG_TILE_CELL_QUAD:
		str = _T("四個邊線");
		break;
	case ALG_TILE_CELL_OCTA:
		str = _T("八個邊線");
		break;
	}
	return str;
}
//-------------------------------------------------------------------------------------//
ALG_TILE_CELL_MODE CAlgParam::GetAlgTiltCellModeByText(LPCTSTR DirText)
{
	CString  strDirText;
	ALG_TILE_CELL_MODE CellMode = ALG_TILE_CELL_HOR;

	CellMode = ALG_TILE_CELL_HOR;
	strDirText = CAlgParam::GetAlgTiltCellModeText(CellMode);
	if ( strDirText.CompareNoCase(DirText) == 0 ) 
	{	return CellMode; }

	CellMode = ALG_TILE_CELL_VER;
	strDirText = CAlgParam::GetAlgTiltCellModeText(CellMode);
	if ( strDirText.CompareNoCase(DirText) == 0 ) 
	{	return CellMode; }

	CellMode = ALG_TILE_CELL_CORNER;
	strDirText = CAlgParam::GetAlgTiltCellModeText(CellMode);
	if ( strDirText.CompareNoCase(DirText) == 0 ) 
	{	return CellMode; }

	CellMode = ALG_TILE_CELL_QUAD;
	strDirText = CAlgParam::GetAlgTiltCellModeText(CellMode);
	if ( strDirText.CompareNoCase(DirText) == 0 ) 
	{	return CellMode; }

	CellMode = ALG_TILE_CELL_OCTA;
	strDirText = CAlgParam::GetAlgTiltCellModeText(CellMode);
	if ( strDirText.CompareNoCase(DirText) == 0 ) 
	{	return CellMode; }

	return ALG_TILE_CELL_HOR;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ExecAlgInspection_BodyTilt(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList)
{
	if ( NULL == ModelPtr ) { return false; }
	const size_t FrameCount = UniFrameList.size();	
	const size_t MaskFrameIndex = m_AlgMaskBinParam.GetBinaryFrameIndex();
	const size_t ImageFrameIndex = m_AlgImageBinParam.GetBinaryFrameIndex();
	if ( ImageFrameIndex >= FrameCount ) 
	{	return false; }
	
	CAOIWnd         *WndPtr = CAlgParam::GetAlgWndPtr();
	if ( NULL == WndPtr ) { return false; }	

	TUNI_FRAME      *UniFramePtr  = &(UniFrameList[ImageFrameIndex]);
	const IMAGE_SIZE FrameImageW    = UniFramePtr->ImageW;
	const IMAGE_SIZE FrameImageH    = UniFramePtr->ImageH;
	const IMAGE_SIZE FrameImageStep = UniFramePtr->ImageStep;
	const IMAGE_SIZE FrameBitCount  = UniFramePtr->BitCount;		
	IMAGE_PTR        FrameImagePtr  = UniFramePtr->ImagePtr;
	MASK_PTR         FrameMaskPtr   = UniFramePtr->MaskPtr;
	SPACE_PTR        FrameSpacePtr  = UniFramePtr->SpacePtr;
	TALG_PARAM_BODY_TILT &btParam   = GetAlgParamBodyTilt();
	const int RoiW = RoiRect.right-RoiRect.left;
	const int RoiH = RoiRect.bottom-RoiRect.top;
	const int RoiCpx = (int)((RoiRect.left+RoiRect.right)/2);
	const int RoiCpy = (int)((RoiRect.top+RoiRect.bottom)/2);
	const int RoiRectSize = (int)((RoiW)*(RoiH));
	
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
	
	double Ave = 0.0, Value=0, USL=0.0, LSL=0.0;
	double Sum=0, Max=0, Min=0, TiltAngle=0;		
	size_t i=0, j=0, k=0;
	size_t        MaxIdx=0, MinIdx=0;
	RECT          CellRect={0,0,0,0};
	unsigned int  Count=0;
	unsigned int  index=0;
	//const double  SpaceMaxH  = AOIDataCollect.GetSpaceMaxHeight();
	//const double     BaseHeight = AOIDataCollect.GetSpaceBaseHeight();
	const double  BaseHeight = 0;
	const ALG_TILE_CELL_MODE CellMode = btParam.btCellMode;
	const double SizeRatioX = btParam.btSizeRatioX;
	const double SizeRatioY = btParam.btSizeRatioY;
	const int CellW = (int)(RoiW*SizeRatioX/100.0);
	const int CellH = (int)(RoiH*SizeRatioY/100.0);
	TPOINT2D ImageScale = ModelPtr->GetModelImageScale();
	std::vector<RECT>  CellRectList(16);	
	CellRectList.clear();	
	
	if ( BuildBodyTiltCellRectList(RoiRect, CellMode, CellRectList) == false )
	{
		JetMemory.free_func(MaskPtr);
		JetMemory.free_func(GrayPtr);
		return false;	
	}	

	const size_t CellRectCount = CellRectList.size();

	//Calc Average 
	if ( CellRectCount > 0 ) 
	{
		if ( NULL == FrameSpacePtr )
		{	
			Max=0, Min=0;
			for ( k=0; k<CellRectCount; k++ )
			{	
				CellRect = CellRectList[k];
				Sum = 0;
				Count = 0;
				for ( i=CellRect.top; i<CellRect.bottom; i++ )
				{
					index = i*MaskStep;
					for ( j=CellRect.left; j<CellRect.right; j++ )
					{
						Value = GrayPtr[index+j];
						Sum += Value;
						Count ++;
					}
				}
				if ( Count > 0 ) 
				{	Ave = Sum/Count; }
				else
				{	Ave = 0.0; }
				if ( 0 == k ) 
				{	
					Max = Min = Ave; 
					MaxIdx= MinIdx = k;
				}
				else
				{
					if ( Max < Ave ) 
					{	
						Max = Ave; 
						MaxIdx = k;
					}
					if ( Min > Ave ) 
					{ 
						Min = Ave; 
						MinIdx = k;
					}
				}
			}
		}
		else
		{	
			Max=0, Min=0;				
			for ( k=0; k<CellRectCount; k++ )
			{	
				CellRect = CellRectList[k];
				Sum = 0;
				Count = 0;
				for ( i=CellRect.top; i<CellRect.bottom; i++ )
				{
					index = i*MaskStep;
					for ( j=CellRect.left; j<CellRect.right; j++ )
					{	
						if ( ImageAPI.CheckSpaceMaskValid(FrameMaskPtr[index+j]) == false ) { continue; }
						Value = FrameSpacePtr[index+j]-BaseHeight;
						Sum += Value;
						Count ++;
					}
				}
				if ( Count > 0 ) 
				{	Ave = Sum/Count; }
				else
				{	Ave = 0.0; }
				if ( 0 == k ) 
				{	
					Max = Min = Ave; 
					MaxIdx= MinIdx = k;
				}
				else
				{
					if ( Max < Ave ) 
					{	
						Max = Ave; 
						MaxIdx = k;
					}
					if ( Min > Ave ) 
					{ 
						Min = Ave; 
						MinIdx = k;
					}
				}			
			}
		}
		Value = Max-Min;
		if ( MaxIdx != MinIdx )
		{
			RECT CellMaxRect = CellRectList[MaxIdx];
			RECT CellMinRect = CellRectList[MinIdx];
			double dCellL = 0.0;
			double dCellX = ((CellMaxRect.left+CellMaxRect.right)-(CellMinRect.left+CellMinRect.right))*0.5;
			double dCellY = ((CellMaxRect.top+CellMaxRect.bottom)-(CellMinRect.top+CellMinRect.bottom))*0.5;
			if ( NULL != FrameSpacePtr )
			{
				dCellX /= ImageScale.x;
				dCellY /= ImageScale.y;
			}
			dCellL = ::sqrt((dCellX*dCellX)+(dCellY*dCellY));
			TiltAngle = ::atan(Value/dCellL)*RAD_TO_DEG_DBL;
		}
		else
		{
			TiltAngle = 0;
		}
	}
	else
	{
		Value = 0;	
		TiltAngle = 0;
	}
	btParam.btTiltGapReading = Value;
	btParam.btTiltAngleReading = TiltAngle;
	
	CString strResult;	
	SetAlgResultReading1(Value);	
	SetAlgResultText(_T("OK"));
	SetAlgResultID(RESULT_ID_OK);

	USL = btParam.btTiltGapUSL;
	LSL = btParam.btTiltGapLSL;
	Ave = btParam.btTiltGapReading;
	if ( Ave>USL || Ave<LSL )
	{			
		strResult.Format(_T("%.0f"), Ave);		
		SetAlgResultID(RESULT_ID_NG);
		SetAlgResultText(strResult);
	}

	USL = btParam.btTiltAngleUSL;
	LSL = btParam.btTiltAngleLSL;
	Ave = btParam.btTiltAngleReading;
	if ( Ave>USL || Ave<LSL )
	{			
		strResult.Format(_T("%.0f"), Ave);		
		SetAlgResultID(RESULT_ID_NG);
		SetAlgResultText(strResult);
	}
	WndPtr->SetWndResultID(m_AlgResultID);
	WndPtr->SetWndResultText(m_AlgResultText);
	SaveAlgDefectImage(MaskW, MaskH, MaskStep, MaskBitCount, GrayPtr, MaskPtr, WndRect);
	JetMemory.free_func(MaskPtr);	
	JetMemory.free_func(GrayPtr);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::BuildBodyTiltCellRectList(const RECT &RoiRect, ALG_TILE_CELL_MODE CellMode, std::vector<RECT> &CellRectList)
{
	TALG_PARAM_BODY_TILT &btParam = GetAlgParamBodyTilt();	
	const int RoiW = RoiRect.right-RoiRect.left;
	const int RoiH = RoiRect.bottom-RoiRect.top;
	const int RoiCpx = (RoiRect.left+RoiRect.right)/2;
	const int RoiCpy = (RoiRect.top+RoiRect.bottom)/2;
	const double SizeRatioX = btParam.btSizeRatioX;
	const double SizeRatioY = btParam.btSizeRatioY;
	const int CellW = (int)(RoiW*SizeRatioX/100.0);
	const int CellH = (int)(RoiH*SizeRatioY/100.0);
	if ( SizeRatioX>100.0 || SizeRatioY>100.0 ) { return false; }
	RECT CellRect={0,0,0,0};
	CellRectList.clear();	
	if ( ALG_TILE_CELL_HOR == CellMode )//左右
	{
		CellRect.top = RoiCpy-(CellH/2);
		CellRect.bottom = CellRect.top+CellH;
		CellRect.left = RoiRect.left;
		CellRect.right = CellRect.left+CellW;
		CellRectList.push_back(CellRect);

		CellRect.top = RoiCpy-(CellH/2);
		CellRect.bottom = CellRect.top+CellH;
		CellRect.right = RoiRect.right;
		CellRect.left = CellRect.right-CellW;
		CellRectList.push_back(CellRect);
	}	
	else if ( ALG_TILE_CELL_VER == CellMode )//上下
	{
		CellRect.top = RoiRect.top;
		CellRect.bottom = CellRect.top+CellH;
		CellRect.left = RoiCpx-(CellW/2);
		CellRect.right = CellRect.left+CellW;
		CellRectList.push_back(CellRect);

		CellRect.bottom = RoiRect.bottom;
		CellRect.top = CellRect.bottom-CellH;
		CellRect.left = RoiCpx-(CellW/2);
		CellRect.right = CellRect.left+CellW;
		CellRectList.push_back(CellRect);
	}
	else if ( ALG_TILE_CELL_CORNER == CellMode )//四角
	{
		CellRect.left = RoiRect.left;
		CellRect.top = RoiRect.top;
		CellRect.bottom = CellRect.top+CellH;		
		CellRect.right = CellRect.left+CellW;
		CellRectList.push_back(CellRect);

		CellRect.right = RoiRect.right;
		CellRect.top = RoiRect.top;
		CellRect.bottom = CellRect.top+CellH;		
		CellRect.left = CellRect.right-CellW;
		CellRectList.push_back(CellRect);

		CellRect.left = RoiRect.left;
		CellRect.bottom = RoiRect.bottom;
		CellRect.top = CellRect.bottom-CellH;		
		CellRect.right = CellRect.left+CellW;
		CellRectList.push_back(CellRect);

		CellRect.right = RoiRect.right;
		CellRect.bottom = RoiRect.bottom;
		CellRect.top = CellRect.bottom-CellH;
		CellRect.left = CellRect.right-CellW;
		CellRectList.push_back(CellRect);
	}
	else if ( ALG_TILE_CELL_QUAD == CellMode )//四邊
	{
		CellRect.left = RoiRect.left;
		CellRect.top = RoiCpy-(CellH/2);
		CellRect.bottom = CellRect.top+CellH;		
		CellRect.right = CellRect.left+CellW;
		CellRectList.push_back(CellRect);

		CellRect.right = RoiCpx-(CellW/2);
		CellRect.top = RoiRect.top;
		CellRect.bottom = CellRect.top+CellH;		
		CellRect.left = CellRect.right-CellW;
		CellRectList.push_back(CellRect);

		CellRect.right = RoiRect.right;
		CellRect.top = RoiCpy-(CellH/2);
		CellRect.bottom = CellRect.top+CellH;		
		CellRect.left = CellRect.right-CellW;
		CellRectList.push_back(CellRect);
		
		CellRect.right = RoiCpx-(CellW/2);
		CellRect.bottom = RoiRect.bottom;
		CellRect.top = CellRect.bottom-CellH;		
		CellRect.left = CellRect.right-CellW;
		CellRectList.push_back(CellRect);
	}
	else if ( ALG_TILE_CELL_OCTA == CellMode )//八邊
	{
		CellRect.left = RoiRect.left;
		CellRect.top = RoiRect.top;
		CellRect.bottom = CellRect.top+CellH;		
		CellRect.right = CellRect.left+CellW;
		CellRectList.push_back(CellRect);

		CellRect.left = RoiRect.left;
		CellRect.top = RoiCpy-(CellH/2);
		CellRect.bottom = CellRect.top+CellH;		
		CellRect.right = CellRect.left+CellW;
		CellRectList.push_back(CellRect);

		CellRect.right = RoiRect.right;
		CellRect.top = RoiRect.top;
		CellRect.bottom = CellRect.top+CellH;		
		CellRect.left = CellRect.right-CellW;
		CellRectList.push_back(CellRect);

		CellRect.right = RoiCpx-(CellW/2);
		CellRect.top = RoiRect.top;
		CellRect.bottom = CellRect.top+CellH;		
		CellRect.left = CellRect.right-CellW;
		CellRectList.push_back(CellRect);

		CellRect.left = RoiRect.left;
		CellRect.bottom = RoiRect.bottom;
		CellRect.top = CellRect.bottom-CellH;		
		CellRect.right = CellRect.left+CellW;
		CellRectList.push_back(CellRect);

		CellRect.right = RoiRect.right;
		CellRect.top = RoiCpy-(CellH/2);
		CellRect.bottom = CellRect.top+CellH;		
		CellRect.left = CellRect.right-CellW;
		CellRectList.push_back(CellRect);
		

		CellRect.right = RoiRect.right;
		CellRect.bottom = RoiRect.bottom;
		CellRect.top = CellRect.bottom-CellH;
		CellRect.left = CellRect.right-CellW;
		CellRectList.push_back(CellRect);

		CellRect.right = RoiCpx-(CellW/2);
		CellRect.bottom = RoiRect.bottom;
		CellRect.top = CellRect.bottom-CellH;		
		CellRect.left = CellRect.right-CellW;
		CellRectList.push_back(CellRect);		
	}	
	else
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//