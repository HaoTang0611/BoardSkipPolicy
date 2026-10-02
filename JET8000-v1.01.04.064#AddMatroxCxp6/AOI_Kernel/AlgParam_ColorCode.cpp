// AlgParam_ColorCode.cpp: implementation of the CAlgParam class.
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
bool CAlgParam::CheckOK_ColorCode(const TALG_PARAM_COLOR_CODE &Param)
{
	if ( CheckOK_ColorCodeUSL(Param) == false )
	{	return false; }
	if ( CheckOK_ColorCodeLSL(Param) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_ColorCodeUSL(const TALG_PARAM_COLOR_CODE &Param)
{
	if ( Param.ccPassRatioReading > Param.ccPassRatioUSL )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_ColorCodeLSL(const TALG_PARAM_COLOR_CODE &Param)
{
	if ( Param.ccPassRatioReading < Param.ccPassRatioLSL )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::WriteAlgParamFile_ColorCode(const TALG_PARAM_COLOR_CODE &ccParam, CAOIFileIO &FileIO)//儲存色碼檢測參數
{		
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_COLOR_CODE_START, 0) == false ) { return false; }

	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_COLOR_CODE_POLARITY, ccParam.ccPolarityNum) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_COLOR_CODE_CELL_SCORE_MAX, ccParam.ccCellScoreMax) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_COLOR_CODE_CELL_SCORE_MIN, ccParam.ccCellScoreMin) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_COLOR_CODE_PASS_RATIO_USL, ccParam.ccPassRatioUSL) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_COLOR_CODE_PASS_RATIO_LSL, ccParam.ccPassRatioLSL) == false ) { return false; }

	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_COLOR_CODE_END, 0) == false ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ReadAlgParamFile_ColorCode(TALG_PARAM_COLOR_CODE &ccParam, CAOIFileIO &FileIO)//載入色碼檢測參數
{
	int       index = 0;
	int       nValue = 0;
	while ( FileIO.CheckFileEnd()==false )
	{ 		
		if ( FileIO.LoadChunk(index) == false )		
		{	continue; }
		switch ( index )
		{
		case FILE_IO_ALG_PARAM_COLOR_CODE_END://色碼檢測參數-終點
			return true;
			break;		
		case FILE_IO_ALG_PARAM_COLOR_CODE_POLARITY://極性方向
			nValue = FileIO.GetData_INT();
			switch ( nValue )
			{
			case 1:
			case 2:
				ccParam.ccPolarityNum = nValue;
				break;
			}
			break;
		case FILE_IO_ALG_PARAM_COLOR_CODE_CELL_SCORE_MAX://單元相似度上限
			ccParam.ccCellScoreMax = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_COLOR_CODE_CELL_SCORE_MIN://單元相似度下限
			ccParam.ccCellScoreMin = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_COLOR_CODE_PASS_RATIO_USL://通過比例上限
			ccParam.ccPassRatioUSL = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_COLOR_CODE_PASS_RATIO_LSL://通過比例下限
			ccParam.ccPassRatioLSL = FileIO.GetData_DBL();
			break;
		default:
		#ifdef _DEBUG
			index = index;
		#endif//_DEBUG
			break;
		}
	}
	FileIO.SetErrorString(_T("Error, ReadAlgParamFile_ColorCode Fault"));
	return false;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ExecAlgInspection_ColorCode(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList)
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
	
	CString          str;
	double           Sum=0.0, Ave=0;
	size_t           i=0, Count=0;	
	int              j=0, k=0, index=0;	
	RECT             WndRoiRect;
	double           PassRatio=0;
	double           PassCount=0;
	double           TotalCount=0;
	const int        nAlign = 4;	
	TREGION4D        WndRegion;
	TREGION4D        WndRoiRegion;
	CAOIWndRoi      *WndRoiPtr = NULL;	
	CAlgBinaryParam *BinaryParamPtr=NULL;	
	const unsigned int WndIndex = WndPtr->GetWndIndex();	
	const size_t     WndRoiCount = WndPtr->GetWndRoiWndCount();	
	TUNI_FRAME      *UniFramePtr  = &(UniFrameList[ImageFrameIndex]);
	const IMAGE_SIZE FrameImageW    = UniFramePtr->ImageW;
	const IMAGE_SIZE FrameImageH    = UniFramePtr->ImageH;
	const IMAGE_SIZE FrameImageStep = UniFramePtr->ImageStep;
	const IMAGE_SIZE FrameBitCount  = UniFramePtr->BitCount;		
	IMAGE_PTR        FrameImagePtr  = UniFramePtr->ImagePtr;
	MASK_PTR         FrameMaskPtr   = UniFramePtr->MaskPtr;
	SPACE_PTR        FrameSpacePtr  = UniFramePtr->SpacePtr;
	TPOINT2D         ImageScale     = ModelPtr->GetModelImageScale();
	TALG_PARAM_COLOR_CODE &ccParam  = GetAlgParamColorCode();
	const double     CellScoreMin = ccParam.ccCellScoreMin;
	const double     CellScoreMax = ccParam.ccCellScoreMax;
	const double     SpaceRatio = AOIDataCollect.GetSpaceToGrayRatio();
	const IMAGE_SIZE RoiW = RoiRect.right-RoiRect.left;
	const IMAGE_SIZE RoiH = RoiRect.bottom-RoiRect.top;
	const IMAGE_SIZE RoiStep = JetAPI::GetBMPImagePixelsPerLine(RoiW, FrameBitCount, nAlign);
	const IMAGE_SIZE RoiRectSize = (int)((RoiRect.right-RoiRect.left)*(RoiRect.bottom-RoiRect.top));
	CAlgBinaryParam &ImageBinParam = GetAlgImageBinParam();
	const BINARY_MODE ImageBinMode = ImageBinParam.GetBinaryMode();

	IMAGE_SIZE    MaskW=0;
	IMAGE_SIZE    MaskH=0;	
	IMAGE_SIZE    MaskStep=0;
	IMAGE_SIZE    MaskBitCount=8;
	MASK_PTR      MaskPtr  = NULL;	
	IMAGE_PTR     GrayPtr = NULL;	
	const bool    bTestWnd = false;
	TUNI_FRAME    WndUniFrame;
	std::vector<TUNI_FRAME> WndUniFrameList;

#ifdef _DEBUG	
	bool         bSave = false;
	CString      ComponentName;
	CString      DebugFolder=GetAlgDebugFolder();
	if ( NULL != ComponentPtr )
	{	ComponentName = ComponentPtr->GetComponentFullName(); }		
#endif//_DEBUG

	if ( BuildWndUniFrameList(UniFrameList, RoiRect, WndUniFrameList) == false )
	{
		JetMemory.free_func(MaskPtr);
		JetMemory.free_func(GrayPtr);
		return false;	
	}	
	
	RECT RoiWndRect={0,0,0,0};	
	WndPtr->GetWndRegion(WndRegion);	
	JetAPI::SizeToRect(RoiW, RoiH, RoiWndRect);
	const size_t WndFrameCount = WndUniFrameList.size();	
	for ( i=0; i<WndRoiCount; i++ )
	{
		WndRoiPtr = WndPtr->GetWndRoiWndPtr(i, false);
		if ( NULL == WndRoiPtr ) { continue; }		
		WndRoiPtr->GetWndRoiRegion(WndRoiRegion);
		if ( JetAPI::CalcRegionRect(WndRegion, RoiWndRect, WndRoiRegion, WndRoiRect, true) == false ) //Cad和Image的Y是顛倒的
		{	continue;	}		

		BinaryParamPtr = WndRoiPtr->GetWndRoiBinaryParamPtr();
		if ( ExecAlgUniFrameBinary(*BinaryParamPtr, WndRoiRect, WndRoiRect, WndUniFrameList, MaskW, MaskH, MaskStep, MaskBitCount, MaskPtr, GrayPtr, bTestWnd) == false )
		{				
			JetMemory.free_func(MaskPtr);
			JetMemory.free_func(GrayPtr);
			JetAPI::ClearUniFrameList(WndUniFrameList);
			return false; 
		}		
		
	#ifdef _DEBUG
		if ( true == bSave )
		{			
			str.Format(_T("%s\\%s_ModelWndAlgColorCode#%d_MaskT#%d.PNG"), DebugFolder, ComponentName, WndIndex+1, i+1);
			ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, MaskBitCount, MaskPtr, true);
		}
	#endif//_DEBUG

		Sum = 0.0;
		Count = 0;		
		for ( j=WndRoiRect.top; j<WndRoiRect.bottom; j++ )
		{
			index = j*MaskStep;
			for ( k=WndRoiRect.left; k<WndRoiRect.right; k++ )
			{
				Count ++;
				if ( MaskPtr[index+k] < 128 ) { continue; }
				Sum += 1.0;
			}
		}
		if ( 0 == Count ) 
		{	Ave = 0.0;	}
		else
		{	Ave = 100.0*Sum/Count; }
		str.Format(_T("%.2f %%"), Ave);
		WndRoiPtr->SetWndRoiImageRect(WndRoiRect);
		WndRoiPtr->SetWndRoiResultValue(Ave);
		WndRoiPtr->SetWndRoiResultText(str);
		
		JetMemory.free_func(MaskPtr);
		JetMemory.free_func(GrayPtr);
	}
	//確認是否Pass
	TotalCount = 0;
	for ( i=0; i<WndRoiCount; i++ )
	{
		WndRoiPtr = WndPtr->GetWndRoiWndPtr(i, false);
		if ( NULL == WndRoiPtr ) { continue; }		
		TotalCount += 1.0;

		Ave = WndRoiPtr->GetWndRoiResultValue();
		if ( Ave < CellScoreMin || Ave > CellScoreMax )
		{
			WndRoiPtr->SetWndRoiResultID(RESULT_ID_NG);
			continue; 
		}

		WndRoiPtr->SetWndRoiResultID(RESULT_ID_OK);
		PassCount += 1.0;
	}
	if ( TotalCount < 0.00001 )
	{	PassRatio = 0.0;	}
	else
	{	PassRatio = 100.0*PassCount/TotalCount;	}

	bool ReTest = false;
	if ( PassRatio<ccParam.ccPassRatioLSL || PassRatio>ccParam.ccPassRatioUSL )
	{
		if ( 2 == ccParam.ccPolarityNum )//支援反向計算
		{	ReTest = true;	}
	}

	if ( true == ReTest )
	{	
		for ( i=0; i<WndFrameCount; i++ )
		{
			WndUniFrame = WndUniFrameList[i];
			IMAGE_SIZE    WndImageW=WndUniFrame.ImageW;
			IMAGE_SIZE    WndImageH=WndUniFrame.ImageH;	
			IMAGE_SIZE    WndImageStep=WndUniFrame.ImageStep;
			IMAGE_SIZE    WndImageBitCount=WndUniFrame.BitCount;	
			MASK_PTR      WndMaskPtr  = WndUniFrame.MaskPtr;
			SPACE_PTR     WndSpacePtr = WndUniFrame.SpacePtr;
			IMAGE_PTR     WndImagePtr = WndUniFrame.ImagePtr;

			IMAGE_SIZE    TmpImageW=WndImageW;
			IMAGE_SIZE    TmpImageH=WndImageH;	
			IMAGE_SIZE    TmpImageStep=WndImageStep;
			MASK_PTR      TmpMaskPtr   = NULL;
			SPACE_PTR     TmpSpacePtr  = NULL;
			IMAGE_PTR     TmpImagePtr = NULL;

			if ( NULL != WndMaskPtr )
			{
				if ( ImageAPI.RotateGrayImage_180(WndImageW, WndImageH, WndImageStep, WndMaskPtr, TmpImageW, TmpImageH, TmpImageStep, TmpMaskPtr) == false )
				{				
					JetMemory.free_func(TmpMaskPtr);
					JetMemory.free_func(TmpSpacePtr);
					JetMemory.free_func(TmpImagePtr);
					JetAPI::ClearUniFrameList(WndUniFrameList);
					return false;
				}
				JetMemory.free_func(WndMaskPtr);
				WndUniFrameList[i].MaskPtr = TmpMaskPtr;				
			}
			if ( NULL != WndSpacePtr )
			{
				if ( ImageAPI.RotateSpace_180(WndImageW, WndImageH, WndImageStep, WndSpacePtr, TmpImageW, TmpImageH, TmpImageStep, TmpSpacePtr) == false )
				{				
					JetMemory.free_func(TmpMaskPtr);
					JetMemory.free_func(TmpSpacePtr);
					JetMemory.free_func(TmpImagePtr);
					JetAPI::ClearUniFrameList(WndUniFrameList);
					return false;
				}
				JetMemory.free_func(WndSpacePtr);
				WndUniFrameList[i].SpacePtr = TmpSpacePtr;				
			}
			if ( NULL != WndImagePtr )
			{
				if ( ImageAPI.RotateImage(180.0, WndImageW, WndImageH, WndImageStep, WndImageBitCount, WndImagePtr, TmpImageW, TmpImageH, TmpImageStep, TmpImagePtr) == false )
				{				
					JetMemory.free_func(TmpMaskPtr);
					JetMemory.free_func(TmpSpacePtr);
					JetMemory.free_func(TmpImagePtr);
					JetAPI::ClearUniFrameList(WndUniFrameList);
					return false;
				}
				JetMemory.free_func(WndImagePtr);
				WndUniFrameList[i].ImagePtr = TmpImagePtr;
			}			
		}
		//Second Test
		for ( i=0; i<WndRoiCount; i++ )
		{
			WndRoiPtr = WndPtr->GetWndRoiWndPtr(i, false);
			if ( NULL == WndRoiPtr ) { continue; }		
			WndRoiPtr->GetWndRoiRegion(WndRoiRegion);
			if ( JetAPI::CalcRegionRect(WndRegion, RoiWndRect, WndRoiRegion, WndRoiRect, true) == false ) //Cad和Image的Y是顛倒的
			{	continue;	}

			BinaryParamPtr = WndRoiPtr->GetWndRoiBinaryParamPtr();
			if ( ExecAlgUniFrameBinary(*BinaryParamPtr, WndRoiRect, WndRoiRect, WndUniFrameList, MaskW, MaskH, MaskStep, MaskBitCount, MaskPtr, GrayPtr, bTestWnd) == false )
			{	
				JetMemory.free_func(MaskPtr);
				JetMemory.free_func(GrayPtr);
				JetAPI::ClearUniFrameList(WndUniFrameList);
				return false; 
			}						

		#ifdef _DEBUG
			if ( true == bSave )
			{	
				str.Format(_T("%s\\%s_ModelWndAlgColorCode#%d_MaskB#%d.PNG"), DebugFolder, ComponentName, WndIndex+1, i+1);
				ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, MaskBitCount, MaskPtr, true);
			}
		#endif//_DEBUG

			Sum = 0.0;
			Count = 0;		
			for ( j=WndRoiRect.top; j<WndRoiRect.bottom; j++ )
			{
				index = j*MaskStep;
				for ( k=WndRoiRect.left; k<WndRoiRect.right; k++ )
				{
					Count ++;
					if ( MaskPtr[index+k] < 128 ) { continue; }
					Sum += 1.0;
				}
			}
			if ( 0 == Count ) 
			{	Ave = 0.0;	}
			else
			{	Ave = 100.0*Sum/Count; }
			str.Format(_T("%.2f %%"), Ave);
			WndRoiPtr->SetWndRoiImageRect(WndRoiRect);
			WndRoiPtr->SetWndRoiResultValue(Ave);
			WndRoiPtr->SetWndRoiResultText(str);
		
			JetMemory.free_func(MaskPtr);
			JetMemory.free_func(GrayPtr);
		}

		//確認是否Pass
		TotalCount = 0;
		for ( i=0; i<WndRoiCount; i++ )
		{
			WndRoiPtr = WndPtr->GetWndRoiWndPtr(i, false);
			if ( NULL == WndRoiPtr ) { continue; }		
			TotalCount += 1.0;

			Ave = WndRoiPtr->GetWndRoiResultValue();
			if ( Ave < CellScoreMin || Ave > CellScoreMax )
			{
				WndRoiPtr->SetWndRoiResultID(RESULT_ID_NG);
				continue; 
			}
			WndRoiPtr->SetWndRoiResultID(RESULT_ID_OK);
			PassCount += 1.0;
		}
		if ( TotalCount < 0.00001 )
		{	PassRatio = 0.0;	}
		else
		{	PassRatio = 100.0*PassCount/TotalCount;	}		
	}	
	JetAPI::ClearUniFrameList(WndUniFrameList);

	//加入結果位置
	CAOIBox  ResBox;
	CAOIBox *WndRoiBoxPtr=NULL;
	for ( i=0; i<WndRoiCount; i++ )
	{
		WndRoiPtr = WndPtr->GetWndRoiWndPtr(i, false);
		if ( NULL == WndRoiPtr ) { continue; }
		WndRoiBoxPtr = WndRoiPtr->GetWndRoiBoxPtr();
		ResBox = *WndRoiBoxPtr;
		ResBox.SetBoxVisibled(true);
		ResBox.SetBoxResultTextVisibled(true);
		WndPtr->AddWndResultBox(ResBox);
	}

	CString strResult;	
	ccParam.ccPassRatioReading = PassRatio;
	if ( PassRatio<ccParam.ccPassRatioLSL || PassRatio>ccParam.ccPassRatioUSL )
	{	SetAlgResultID(RESULT_ID_NG);	}
	else
	{	SetAlgResultID(RESULT_ID_OK);	}
	SetAlgResultReading1(PassRatio);
	CString Key = AOIDataDefine.GetRatioText();
	strResult.Format(_T("%s:%.0f%% (%.0f~%.0f)"), Key, PassRatio, ccParam.ccPassRatioLSL, ccParam.ccPassRatioUSL);
	SetAlgResultText(strResult);

	WndPtr->SetWndResultID(m_AlgResultID);
	WndPtr->SetWndResultText(m_AlgResultText);
	//SaveAlgDefectImage(MaskW, MaskH, MaskStep, MaskBitCount, GrayPtr, MaskPtr, WndRect);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ExecAlgInspection_ColorCodeImage(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList)
{
	if ( NULL == ModelPtr ) { return false; }	
	const size_t FrameCount = UniFrameList.size();	
	const size_t MaskFrameIndex = m_AlgMaskBinParam.GetBinaryFrameIndex();
	const size_t ImageFrameIndex = m_AlgImageBinParam.GetBinaryFrameIndex();
	if ( ImageFrameIndex >= FrameCount ) 
	{	return false; }
	
	CAOIWnd         *WndPtr = CAlgParam::GetAlgWndPtr();
	if ( NULL == WndPtr ) { return false; }	
	
	CString          str;
	double           Sum=0.0, Ave=0;
	size_t           i=0, Count=0;	
	int              j=0, k=0, index=0;	
	RECT             WndRoiRect;
	double           PassRatio=0;
	double           PassCount=0;
	double           TotalCount=0;
	const int        nAlign = 4;	
	TREGION4D        WndRegion;
	TREGION4D        WndRoiRegion;
	CAOIWndRoi      *WndRoiPtr = NULL;	
	CAlgBinaryParam *BinaryParamPtr=NULL;	
	const size_t     WndRoiCount = WndPtr->GetWndRoiWndCount();
	TUNI_FRAME      *UniFramePtr  = &(UniFrameList[ImageFrameIndex]);
	const IMAGE_SIZE FrameImageW    = UniFramePtr->ImageW;
	const IMAGE_SIZE FrameImageH    = UniFramePtr->ImageH;
	const IMAGE_SIZE FrameImageStep = UniFramePtr->ImageStep;
	const IMAGE_SIZE FrameBitCount  = UniFramePtr->BitCount;		
	IMAGE_PTR        FrameImagePtr  = UniFramePtr->ImagePtr;
	MASK_PTR         FrameMaskPtr   = UniFramePtr->MaskPtr;
	SPACE_PTR        FrameSpacePtr  = UniFramePtr->SpacePtr;
	TPOINT2D         ImageScale     = ModelPtr->GetModelImageScale();
	TALG_PARAM_COLOR_CODE &ccParam  = GetAlgParamColorCode();
	const double     CellScoreMin = ccParam.ccCellScoreMin;
	const double     CellScoreMax = ccParam.ccCellScoreMax;
	const double     SpaceRatio = AOIDataCollect.GetSpaceToGrayRatio();
	const IMAGE_SIZE RoiW = RoiRect.right-RoiRect.left;
	const IMAGE_SIZE RoiH = RoiRect.bottom-RoiRect.top;
	const IMAGE_SIZE RoiStep = JetAPI::GetBMPImagePixelsPerLine(RoiW, FrameBitCount, nAlign);
	const IMAGE_SIZE RoiRectSize = (int)((RoiRect.right-RoiRect.left)*(RoiRect.bottom-RoiRect.top));
	
	IMAGE_SIZE    MaskW=0;
	IMAGE_SIZE    MaskH=0;	
	IMAGE_SIZE    MaskStep=0;
	IMAGE_SIZE    MaskBitCount=8;
	MASK_PTR      MaskPtr  = NULL;	

	IMAGE_SIZE    WndImageW=0;
	IMAGE_SIZE    WndImageH=0;	
	IMAGE_SIZE    WndImageStep=0;
	IMAGE_SIZE    WndImageBitCount=FrameBitCount;	
	MASK_PTR      WndMaskPtr   = NULL;
	SPACE_PTR     WndSpacePtr  = NULL;
	IMAGE_PTR     WndImagePtr = NULL;	
#ifdef _DEBUG
	bool          bSaved = true;
	CString      DebugFolder=GetAlgDebugFolder();
#endif//_DEBUG
	
	MaskW = RoiW;
	MaskH = RoiH;
	MaskStep = JetAPI::GetBMPImagePixelsPerLine(RoiW, MaskBitCount, nAlign);
	if ( NULL != FrameMaskPtr )
	{	
		if ( ImageAPI.ExtractGrayRoiImage(FrameImageW, FrameImageH, FrameImageStep, FrameMaskPtr, RoiRect, MaskStep, WndMaskPtr, false) == false )
		{
			JetMemory.free_func(WndMaskPtr);
			JetMemory.free_func(WndSpacePtr);
			JetMemory.free_func(WndImagePtr);
			return false;	
		}
	}	
	if ( NULL != FrameSpacePtr )
	{	
		if ( ImageAPI.ExtractSpaceGrayRoiImage(FrameImageW, FrameImageH, FrameImageStep, FrameSpacePtr, RoiRect, MaskStep, WndSpacePtr, false) == false )
		{
			JetMemory.free_func(WndMaskPtr);
			JetMemory.free_func(WndSpacePtr);
			JetMemory.free_func(WndImagePtr);
			return false;	
		}
	}	
	if ( NULL != FrameImagePtr )
	{
		if ( ImageAPI.ExtractRoiImage(FrameImageW, FrameImageH, FrameImageStep, FrameBitCount, FrameImagePtr, RoiRect, RoiStep, WndImagePtr, false) == false )
		{
			JetMemory.free_func(WndMaskPtr);
			JetMemory.free_func(WndSpacePtr);
			JetMemory.free_func(WndImagePtr);
			return false;	
		}
	#ifdef _DEBUG
		if ( true == bSaved )
		{
			str.Format(_T("%s\\ColorCodeWndImage#%d.PNG"), DebugFolder, i+1);
			ImageAPI.SaveImage(str, RoiW, RoiH, RoiStep, FrameBitCount, WndImagePtr, true);
		}
	#endif//_DEBUG
	}
	
	RECT RoiWndRect={0,0,0,0};	
	WndImageW = RoiW;
	WndImageH = RoiH;
	WndImageStep = RoiStep;
	WndImageBitCount=FrameBitCount;
	WndPtr->GetWndRegion(WndRegion);		
	JetAPI::SizeToRect(RoiW, RoiH, RoiWndRect);
	for ( i=0; i<WndRoiCount; i++ )
	{
		WndRoiPtr = WndPtr->GetWndRoiWndPtr(i, false);
		if ( NULL == WndRoiPtr ) { continue; }		
		WndRoiPtr->GetWndRoiRegion(WndRoiRegion);
		if ( JetAPI::CalcRegionRect(WndRegion, RoiWndRect, WndRoiRegion, WndRoiRect, true) == false ) //Cad和Image的Y是顛倒的
		{	continue;	}
		BinaryParamPtr = WndRoiPtr->GetWndRoiBinaryParamPtr();		
		if ( ExecAlgImageBinary_Rect(*BinaryParamPtr, WndRoiRect, WndRoiRect, WndImageW, WndImageH, WndImageStep, WndImageBitCount, WndImagePtr, WndMaskPtr, WndSpacePtr, nAlign, MaskPtr) == false )
		{	
			JetMemory.free_func(MaskPtr);
			JetMemory.free_func(WndMaskPtr);
			JetMemory.free_func(WndSpacePtr);
			JetMemory.free_func(WndImagePtr);
			return false; 
		}		
		
	#ifdef _DEBUG
		if ( true == bSaved )
		{
			str.Format(_T("%s\\ColorCodeMaskT#%d.PNG"), DebugFolder, i+1);
			ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, MaskBitCount, MaskPtr, true);
		}
	#endif//_DEBUG

		Sum = 0.0;
		Count = 0;		
		for ( j=WndRoiRect.top; j<WndRoiRect.bottom; j++ )
		{
			index = j*MaskStep;
			for ( k=WndRoiRect.left; k<WndRoiRect.right; k++ )
			{
				Count ++;
				if ( MaskPtr[index+k] < 128 ) { continue; }
				Sum += 1.0;
			}
		}
		if ( 0 == Count ) 
		{	Ave = 0.0;	}
		else
		{	Ave = 100.0*Sum/Count; }
		str.Format(_T("%.2f %%"), Ave);
		WndRoiPtr->SetWndRoiImageRect(WndRoiRect);
		WndRoiPtr->SetWndRoiResultValue(Ave);
		WndRoiPtr->SetWndRoiResultText(str);
		
		JetMemory.free_func(MaskPtr);
	}
	//確認是否Pass
	TotalCount = 0;
	for ( i=0; i<WndRoiCount; i++ )
	{
		WndRoiPtr = WndPtr->GetWndRoiWndPtr(i, false);
		if ( NULL == WndRoiPtr ) { continue; }		
		TotalCount += 1.0;

		Ave = WndRoiPtr->GetWndRoiResultValue();
		if ( Ave < CellScoreMin || Ave > CellScoreMax )
		{
			WndRoiPtr->SetWndRoiResultID(RESULT_ID_NG);
			continue; 
		}

		WndRoiPtr->SetWndRoiResultID(RESULT_ID_OK);
		PassCount += 1.0;
	}
	if ( TotalCount < 0.00001 )
	{	PassRatio = 0.0;	}
	else
	{	PassRatio = 100.0*PassCount/TotalCount;	}
	if ( PassRatio<ccParam.ccPassRatioLSL || PassRatio>ccParam.ccPassRatioUSL )
	{
		if ( 2 == ccParam.ccPolarityNum )//支援反向計算
		{
			IMAGE_SIZE    TmpImageW=WndImageW;
			IMAGE_SIZE    TmpImageH=WndImageH;	
			IMAGE_SIZE    TmpImageStep=WndImageStep;
			MASK_PTR      TmpMaskPtr   = NULL;
			SPACE_PTR     TmpSpacePtr  = NULL;
			IMAGE_PTR     TmpImagePtr = NULL;
			if ( NULL != WndMaskPtr )
			{
				if ( ImageAPI.RotateGrayImage_180(WndImageW, WndImageH, WndImageStep, WndMaskPtr, TmpImageW, TmpImageH, TmpImageStep, TmpMaskPtr) == false )
				{				
					JetMemory.free_func(WndMaskPtr);
					JetMemory.free_func(WndSpacePtr);
					JetMemory.free_func(WndImagePtr);
					return false;
				}
				JetMemory.free_func(WndMaskPtr);
				WndMaskPtr = TmpMaskPtr;
			}
			if ( NULL != WndSpacePtr )
			{
				if ( ImageAPI.RotateSpace_180(WndImageW, WndImageH, WndImageStep, WndSpacePtr, TmpImageW, TmpImageH, TmpImageStep, TmpSpacePtr) == false )
				{				
					JetMemory.free_func(WndMaskPtr);
					JetMemory.free_func(WndSpacePtr);
					JetMemory.free_func(WndImagePtr);
					return false;
				}
				JetMemory.free_func(WndSpacePtr);
				WndSpacePtr = TmpSpacePtr;
			}
			if ( NULL != WndImagePtr )
			{
				if ( ImageAPI.RotateImage(180.0, WndImageW, WndImageH, WndImageStep, WndImageBitCount, WndImagePtr, TmpImageW, TmpImageH, TmpImageStep, TmpImagePtr) == false )
				{				
					JetMemory.free_func(WndMaskPtr);
					JetMemory.free_func(WndSpacePtr);
					JetMemory.free_func(WndImagePtr);
					return false;
				}
				JetMemory.free_func(WndImagePtr);
				WndImagePtr = TmpImagePtr;
			}
			//Second Test
			for ( i=0; i<WndRoiCount; i++ )
			{
				WndRoiPtr = WndPtr->GetWndRoiWndPtr(i, false);
				if ( NULL == WndRoiPtr ) { continue; }		
				WndRoiPtr->GetWndRoiRegion(WndRoiRegion);
				if ( JetAPI::CalcRegionRect(WndRegion, WndRect, WndRoiRegion, WndRoiRect, true) == false ) //Cad和Image的Y是顛倒的
				{	continue;	}

				BinaryParamPtr = WndRoiPtr->GetWndRoiBinaryParamPtr();		
				if ( ExecAlgImageBinary_Rect(*BinaryParamPtr, WndRoiRect, WndRoiRect, WndImageW, WndImageH, WndImageStep, WndImageBitCount, WndImagePtr, WndMaskPtr, WndSpacePtr, nAlign, MaskPtr) == false )
				{	
					JetMemory.free_func(MaskPtr);
					JetMemory.free_func(WndMaskPtr);
					JetMemory.free_func(WndSpacePtr);
					JetMemory.free_func(WndImagePtr);
					return false; 
				}						

			#ifdef _DEBUG
				if ( true == bSaved )
				{
					str.Format(_T("%s\\ColorCodeMaskB#%d.PNG"), DebugFolder, i+1);
					ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, MaskBitCount, MaskPtr, true);
				}
			#endif//_DEBUG

				Sum = 0.0;
				Count = 0;		
				for ( j=WndRoiRect.top; j<WndRoiRect.bottom; j++ )
				{
					index = j*MaskStep;
					for ( k=WndRoiRect.left; k<WndRoiRect.right; k++ )
					{
						Count ++;
						if ( MaskPtr[index+k] < 128 ) { continue; }
						Sum += 1.0;
					}
				}
				if ( 0 == Count ) 
				{	Ave = 0.0;	}
				else
				{	Ave = 100.0*Sum/Count; }
				str.Format(_T("%.2f %%"), Ave);
				WndRoiPtr->SetWndRoiImageRect(WndRoiRect);
				WndRoiPtr->SetWndRoiResultValue(Ave);
				WndRoiPtr->SetWndRoiResultText(str);
		
				JetMemory.free_func(MaskPtr);
			}

			//確認是否Pass
			TotalCount = 0;
			for ( i=0; i<WndRoiCount; i++ )
			{
				WndRoiPtr = WndPtr->GetWndRoiWndPtr(i, false);
				if ( NULL == WndRoiPtr ) { continue; }		
				TotalCount += 1.0;

				Ave = WndRoiPtr->GetWndRoiResultValue();
				if ( Ave < CellScoreMin || Ave > CellScoreMax )
				{
					WndRoiPtr->SetWndRoiResultID(RESULT_ID_NG);
					continue; 
				}
				WndRoiPtr->SetWndRoiResultID(RESULT_ID_OK);
				PassCount += 1.0;
			}
			if ( TotalCount < 0.00001 )
			{	PassRatio = 0.0;	}
			else
			{	PassRatio = 100.0*PassCount/TotalCount;	}
		}
	}
	JetMemory.free_func(WndMaskPtr);
	JetMemory.free_func(WndSpacePtr);
	JetMemory.free_func(WndImagePtr);

	//加入結果位置
	CAOIBox  ResBox;
	CAOIBox *WndRoiBoxPtr=NULL;
	for ( i=0; i<WndRoiCount; i++ )
	{
		WndRoiPtr = WndPtr->GetWndRoiWndPtr(i, false);
		if ( NULL == WndRoiPtr ) { continue; }
		WndRoiBoxPtr = WndRoiPtr->GetWndRoiBoxPtr();
		ResBox = *WndRoiBoxPtr;
		ResBox.SetBoxResultTextVisibled(true);
		WndPtr->AddWndResultBox(ResBox);
	}

	CString strResult;	
	ccParam.ccPassRatioReading = PassRatio;
	if ( PassRatio<ccParam.ccPassRatioLSL || PassRatio>ccParam.ccPassRatioUSL )
	{	SetAlgResultID(RESULT_ID_NG);	}
	else
	{	SetAlgResultID(RESULT_ID_OK);	}
	SetAlgResultReading1(PassRatio);
	CString Key = AOIDataDefine.GetRatioText();
	strResult.Format(_T("%s:%.0f%% (%.0f~%.0f)"), Key, PassRatio, ccParam.ccPassRatioLSL, ccParam.ccPassRatioUSL);
	SetAlgResultText(strResult);

	WndPtr->SetWndResultID(m_AlgResultID);
	WndPtr->SetWndResultText(m_AlgResultText);
	//SaveAlgDefectImage(MaskW, MaskH, MaskStep, MaskBitCount, GrayPtr, MaskPtr, WndRect);
	return true;
}
//-------------------------------------------------------------------------------------//