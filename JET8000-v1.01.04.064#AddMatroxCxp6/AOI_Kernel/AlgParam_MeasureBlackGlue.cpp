// AlgParam_MeasurementBlackGlue.cpp: implementation of the CAlgParam class.
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
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
bool CAlgParam::WriteAlgParamFile_MeasureBlackGlue(const TALG_PARAM_MEASURE_BLACK_GLUE &bgParam, CAOIFileIO &FileIO)//儲存量測黑膠參數
{	
	CString SaveKey;
	CString Filename;
	CString Filename2;
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_MEASURE_BLACK_GLUE_START, 0) == false ) { return false; }
	if ( ALG_MEASURE_BLACK_GLUE == GetAlgType() )
	{
	#ifdef ALG_MEASURE_BLACK_GLUE_USE
		Filename2=GetAlgParamPlugInFilename(_T("MeasurementBlackGlue"));
		if ( Filename2.GetLength() > 0 )
		{
			std::string strPathName;
			JET::alg::MeasurementBlackGlue sBG;	
			JetAPI::ExtractMainFileName(Filename2, Filename);
			JetAPI::TCHAR2string(Filename, strPathName);
			if ( sBG.Save_Parameter(bgParam.bgParam, strPathName) == false )
			{	return false; }

			CString ProjectFolder=FileIO.GetFileFolder();
			if ( JetAPI::ExtractShortFilename(ProjectFolder, Filename, SaveKey) == false )
			{
				SaveKey.Format(_T("Error, ExtractShortFilename Fault[%s, %s]"), ProjectFolder, Filename);
				FileIO.SetErrorString(SaveKey);
				return false;
			}

			if ( FileIO.SaveChunk_STR(FILE_IO_ALG_PARAM_MEASURE_BLACK_GLUE_SAVE_KEY, SaveKey) == false ) { return false; }			
			if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_MEASURE_BLACK_GLUE_FRAME_ID_01, bgParam.bgFrameUniqueID1) == false ) { return false; }
			if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_MEASURE_BLACK_GLUE_FRAME_ID_02, bgParam.bgFrameUniqueID2) == false ) { return false; }
			if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_MEASURE_BLACK_GLUE_FRAME_ID_03, bgParam.bgFrameUniqueID3) == false ) { return false; }
			if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_MEASURE_BLACK_GLUE_FRAME_ID_04, bgParam.bgFrameUniqueID4) == false ) { return false; }
		}
	#endif//ALG_MEASURE_BLACK_GLUE_USE
	}	
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_MEASURE_BLACK_GLUE_END, 0) == false ) { return false; }		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ReadAlgParamFile_MeasureBlackGlue(TALG_PARAM_MEASURE_BLACK_GLUE &bgParam, CAOIFileIO &FileIO)//載入量測黑膠參數
{	
	int     index = 0;
	CString Filename;
	CString SaveKey;
	const CAlgBinaryParam &MaskBinaryParam=GetAlgMaskBinParam();		
	const CAlgBinaryParam &ImageBinaryParam=GetAlgImageBinParam();		
	unsigned int FrameUniqueID1 = FRAME_UNIQUE_ID_DEFAULT;
	unsigned int FrameUniqueID2 = FRAME_UNIQUE_ID_DEFAULT;
	unsigned int FrameUniqueID3 = FRAME_UNIQUE_ID_DEFAULT;
	unsigned int FrameUniqueID4 = FRAME_UNIQUE_ID_DEFAULT;	
	FrameUniqueID1=ImageBinaryParam.GetBinaryFrameUniqueID();
	FrameUniqueID2=MaskBinaryParam.GetBinaryFrameUniqueID();
	while ( FileIO.CheckFileEnd()==false )
	{ 		
		if ( FileIO.LoadChunk(index) == false )		
		{	continue; }
		switch ( index )
		{	
		case FILE_IO_ALG_PARAM_MEASURE_BLACK_GLUE_SAVE_KEY:
			if ( FileIO.GetLoadWStr()==true )
			{	SaveKey = FileIO.GetData_WSTR();	}
			else
			{	SaveKey = FileIO.GetData_STR();	}			
			break;
		case FILE_IO_ALG_PARAM_MEASURE_BLACK_GLUE_FRAME_ID_01://黑膠影像
			FrameUniqueID1 = FileIO.GetData_INT();
			break;
		case FILE_IO_ALG_PARAM_MEASURE_BLACK_GLUE_FRAME_ID_02://板邊影像
			FrameUniqueID2 = FileIO.GetData_INT();
			break;
		case FILE_IO_ALG_PARAM_MEASURE_BLACK_GLUE_FRAME_ID_03://熱熔膠影像
			FrameUniqueID3 = FileIO.GetData_INT();
			break;
		case FILE_IO_ALG_PARAM_MEASURE_BLACK_GLUE_FRAME_ID_04://Coating影像
			FrameUniqueID4 = FileIO.GetData_INT();
			break;
		case FILE_IO_ALG_PARAM_MEASURE_BLACK_GLUE_END://量測黑膠-終點
			if ( ALG_MEASURE_BLACK_GLUE==GetAlgType() && SaveKey.GetLength()>0 )
			{	
			#ifdef ALG_MEASURE_BLACK_GLUE_USE
				std::string strPathName;
				JET::alg::MeasurementBlackGlue sBG;	
				bgParam.bgFrameUniqueID1 = FrameUniqueID1;
				bgParam.bgFrameUniqueID2 = FrameUniqueID2;
				bgParam.bgFrameUniqueID3 = FrameUniqueID3;
				bgParam.bgFrameUniqueID4 = FrameUniqueID4;
				Filename.Format(_T("%s\\%s"), FileIO.GetFileFolder(), SaveKey);				
				JetAPI::TCHAR2string(Filename, strPathName);
				if ( sBG.Load_Parameter(strPathName, bgParam.bgParam) == false )
				{	
					CString Err=sBG.GetErrorMessage().c_str();
					FileIO.SetErrorString(Err);
					return false; 
				}				
			#endif//ALG_MEASURE_BLACK_GLUE_USE
				return true;
			}
			return true;
		default:
		#ifdef _DEBUG
			index = index;
		#endif//_DEBUG
			break;		
		}
	}
	FileIO.SetErrorString(_T("Error, ReadAlgParamFile_MeasureBlackGlue Fault"));
	return false;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ExecAlgInspection_MeasureBlackGlue(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList, bool bTestWnd)	
{	
#ifdef ALG_MEASURE_BLACK_GLUE_USE
	if ( NULL == ModelPtr ) { return false; }	
	const char fnName[]="CAlgParam::ExecAlgInspection_MeasureBlackGlue";
	const size_t FrameCount = UniFrameList.size();		
	CAOIComponent *ComponentPtr = ModelPtr->GetModelComponentPtr();
	TALG_PARAM_MEASURE_BLACK_GLUE &bgParam=GetAlgParamMeasureBlackGlue();	
	const size_t ImageFrameIndex = bgParam.bgFrameIndex1;
	if ( ImageFrameIndex >= FrameCount ) 
	{	return false; }
	const size_t ImageFrameIndex2 = bgParam.bgFrameIndex2;
	if ( ImageFrameIndex2 >= FrameCount ) 
	{	return false; }
	const size_t ImageFrameIndex3 = bgParam.bgFrameIndex3;
	if ( ImageFrameIndex3 >= FrameCount ) 
	{	return false; }
	const size_t ImageFrameIndex4 = bgParam.bgFrameIndex4;
	if ( ImageFrameIndex4 >= FrameCount ) 
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

	TUNI_FRAME      *UniFramePtr2  = &(UniFrameList[ImageFrameIndex2]);
	const IMAGE_SIZE FrameImageW2    = UniFramePtr2->ImageW;
	const IMAGE_SIZE FrameImageH2    = UniFramePtr2->ImageH;
	const IMAGE_SIZE FrameImageStep2 = UniFramePtr2->ImageStep;
	const IMAGE_SIZE FrameBitCount2  = UniFramePtr2->BitCount;		
	IMAGE_PTR        FrameImagePtr2  = UniFramePtr2->ImagePtr;
	MASK_PTR         FrameMaskPtr2   = UniFramePtr2->MaskPtr;
	SPACE_PTR        FrameSpacePtr2  = UniFramePtr2->SpacePtr;

	TUNI_FRAME      *UniFramePtr3  = &(UniFrameList[ImageFrameIndex3]);
	const IMAGE_SIZE FrameImageW3    = UniFramePtr3->ImageW;
	const IMAGE_SIZE FrameImageH3    = UniFramePtr3->ImageH;
	const IMAGE_SIZE FrameImageStep3 = UniFramePtr3->ImageStep;
	const IMAGE_SIZE FrameBitCount3  = UniFramePtr3->BitCount;		
	IMAGE_PTR        FrameImagePtr3  = UniFramePtr3->ImagePtr;
	MASK_PTR         FrameMaskPtr3   = UniFramePtr3->MaskPtr;
	SPACE_PTR        FrameSpacePtr3  = UniFramePtr3->SpacePtr;

	TUNI_FRAME      *UniFramePtr4  = &(UniFrameList[ImageFrameIndex4]);
	const IMAGE_SIZE FrameImageW4    = UniFramePtr4->ImageW;
	const IMAGE_SIZE FrameImageH4    = UniFramePtr4->ImageH;
	const IMAGE_SIZE FrameImageStep4 = UniFramePtr4->ImageStep;
	const IMAGE_SIZE FrameBitCount4  = UniFramePtr4->BitCount;		
	IMAGE_PTR        FrameImagePtr4  = UniFramePtr4->ImagePtr;
	MASK_PTR         FrameMaskPtr4   = UniFramePtr4->MaskPtr;
	SPACE_PTR        FrameSpacePtr4  = UniFramePtr4->SpacePtr;

	TPOINT2D         ImageScale     = ModelPtr->GetModelImageScale();	

	const int WndW=WndRect.right-WndRect.left;
	const int WndH=WndRect.bottom-WndRect.top;
	const int RoiW = RoiRect.right-RoiRect.left;
	const int RoiH = RoiRect.bottom-RoiRect.top;
	const int RoiRectSize = (int)((RoiRect.right-RoiRect.left)*(RoiRect.bottom-RoiRect.top));

	if ( NULL==FrameImagePtr || NULL==FrameImagePtr2 )
	{	return false;	}	

	const int nAlign = 1;
	IMAGE_PTR  GlueImagePtr  = NULL;	
	IMAGE_SIZE GlueImageW    = WndW;
	IMAGE_SIZE GlueImageH    = WndH;	
	IMAGE_SIZE GlueBitCount  = FrameBitCount;
	IMAGE_SIZE GlueImageStep = JetAPI::GetBMPImagePixelsPerLine(GlueImageW, GlueBitCount, nAlign);
	const size_t GlueBufferSize = ImageAPI.CalcBufferSize(GlueImageStep, GlueImageH);

	IMAGE_PTR  BoardImagePtr  = NULL;
	IMAGE_SIZE BoardImageW    = WndW;
	IMAGE_SIZE BoardImageH    = WndH;	
	IMAGE_SIZE BoardBitCount  = FrameBitCount2;
	IMAGE_SIZE BoardImageStep = JetAPI::GetBMPImagePixelsPerLine(BoardImageW, BoardBitCount, nAlign);
	const size_t BoardBufferSize = ImageAPI.CalcBufferSize(BoardImageStep, BoardImageH);

	IMAGE_PTR  ThermalImagePtr  = NULL;
	IMAGE_SIZE ThermalImageW    = WndW;
	IMAGE_SIZE ThermalImageH    = WndH;	
	IMAGE_SIZE ThermalBitCount  = FrameBitCount3;
	IMAGE_SIZE ThermalImageStep = JetAPI::GetBMPImagePixelsPerLine(ThermalImageW, ThermalBitCount, nAlign);
	const size_t ThermalBufferSize = ImageAPI.CalcBufferSize(ThermalImageStep, ThermalImageH);

	IMAGE_PTR  CoatingImagePtr  = NULL;
	IMAGE_SIZE CoatingImageW    = WndW;
	IMAGE_SIZE CoatingImageH    = WndH;	
	IMAGE_SIZE CoatingBitCount  = FrameBitCount4;
	IMAGE_SIZE CoatingImageStep = JetAPI::GetBMPImagePixelsPerLine(CoatingImageW, CoatingBitCount, nAlign);
	const size_t CoatingBufferSize = ImageAPI.CalcBufferSize(CoatingImageStep, CoatingImageH);

	if ( JetMemory.alloc_func(GlueBufferSize, GlueImagePtr, fnName, "GlueImagePtr")==false ||
		 JetMemory.alloc_func(BoardBufferSize, BoardImagePtr, fnName, "BoardImagePtr")==false ||
		 JetMemory.alloc_func(ThermalBufferSize, ThermalImagePtr, fnName, "ThermalImagePtr")==false ||
		 JetMemory.alloc_func(CoatingBufferSize, CoatingImagePtr, fnName, "CoatingImagePtr")==false )
	{
		JetMemory.free_func(GlueImagePtr);
		JetMemory.free_func(BoardImagePtr);
		JetMemory.free_func(ThermalImagePtr);
		JetMemory.free_func(CoatingImagePtr);
		return false;
	}
	std::vector<IMAGE_PTR> ImageList;
	ImageList.push_back(GlueImagePtr);
	ImageList.push_back(BoardImagePtr);
	ImageList.push_back(ThermalImagePtr);
	ImageList.push_back(CoatingImagePtr);

	if ( ImageAPI.ExtractRoiImage3(FrameImageW, FrameImageH, FrameImageStep, FrameBitCount, FrameImagePtr, WndRect, GlueImageStep, GlueImagePtr, false)==false ||
		 ImageAPI.ExtractRoiImage3(FrameImageW2, FrameImageH2, FrameImageStep2, FrameBitCount2, FrameImagePtr2, WndRect, BoardImageStep, BoardImagePtr, false)==false ||
		 ImageAPI.ExtractRoiImage3(FrameImageW3, FrameImageH3, FrameImageStep3, FrameBitCount3, FrameImagePtr3, WndRect, ThermalImageStep, ThermalImagePtr, false)==false ||
		 ImageAPI.ExtractRoiImage3(FrameImageW4, FrameImageH4, FrameImageStep4, FrameBitCount4, FrameImagePtr4, WndRect, CoatingImageStep, CoatingImagePtr, false)==false  )
	{
		JetMemory.free_list(ImageList);
		return false;
	}	
	
	JET::alg::MeasurementBlackGlue sBG;
	JET::alg::SMeasurementBlackGlue_Result sResult;
	JET::alg::SMeasurementBlackGlue_Parameter sParam=bgParam.bgParam;
	cv::Mat matGlue = ImageAPI.CreateMat(GlueImageW, GlueImageH, GlueImageStep, GlueBitCount, GlueImagePtr);	
	cv::Mat matBoard = ImageAPI.CreateMat(BoardImageW, BoardImageH, BoardImageStep, BoardBitCount, BoardImagePtr);	
	cv::Mat matThermal = ImageAPI.CreateMat(ThermalImageW, ThermalImageH, ThermalImageStep, ThermalBitCount, ThermalImagePtr);	
	cv::Mat matCoating = ImageAPI.CreateMat(CoatingImageW, CoatingImageH, CoatingImageStep, CoatingBitCount, CoatingImagePtr);	

	sParam.fResolutionX = (float)(1.0/ImageScale.x);
	sParam.fResolutionY = (float)(1.0/ImageScale.y);
	bgParam.bgParam.fResolutionX = (float)(1.0/ImageScale.x);
	bgParam.bgParam.fResolutionY = (float)(1.0/ImageScale.y);

	//cv::Mat matGlue = imread("R:\\BlackGlue\\Glue.PNG", -1);
	//cv::Mat matBoard = imread("R:\\BlackGlue\\Board.PNG", -1);
	//cv::Mat matThermal = imread("R:\\BlackGlue\\Thermal.PNG", -1);
	//cv::Mat matCoating = imread("R:\\BlackGlue\\Coating.PNG", -1);
	//sBG.Load_Parameter("R:\\BlackGlue\\Param", sParam);	
	//imwrite("R:\\TestGlue.PNG", matGlue);
	//imwrite("R:\\TestBoard.PNG", matBoard);

	std::vector<cv::Mat> vtImage;
	vtImage.push_back(matGlue);
	vtImage.push_back(matBoard);	
	vtImage.push_back(matThermal);
	vtImage.push_back(matCoating);	

	// 開啟存圖功能
	const bool bSaveImage=false;
	std::string  strSaveFolder;
	CString      DebugFilename;
	CString      ComponentName;	
	CString      DebugFolder=GetAlgDebugFolder();
	if ( NULL != ComponentPtr )
	{	ComponentName = ComponentPtr->GetComponentFullName(); }			
	DebugFilename.Format(_T("%s\\%s_ModelWndAlgMeasureBlackGlue#%d"), DebugFolder, ComponentName, WndIndex+1);
	JetAPI::TCHAR2string(DebugFilename, strSaveFolder);	
	sBG.SetSaveImage(bSaveImage);
	sBG.SetSavePathName(strSaveFolder);	

	RESULT_ID ResultID=RESULT_ID_OK;
	bool bOk = sBG.Measurement(vtImage, sParam, sResult);
	if ( false == bOk )
	{
		ResultID = RESULT_ID_NG;	
		m_AlgResultText = sBG.GetErrorMessage().c_str();
	}
	else
	{	
		const int SaveAll     = 1;
		const int SaveOK_Only = 2;
		const int SaveNG_Only = 3;		
		const int SymbolType = 2;
		std::vector<std::string> strList;
		ResultID = RESULT_ID_NONE;
		if ( sBG.ReportTxt_BlackGlue(sParam, sResult, strList, SaveNG_Only, SymbolType) == false )
		{
			ResultID = RESULT_ID_NG;	
			m_AlgResultText = sBG.GetErrorMessage().c_str();
		}
		else
		{
			CString ResultStr;
			if ( 0 == strList.size() )
			{
				ResultStr = _T("OK");
				ResultID = RESULT_ID_OK; 
			}
			else
			{
				ResultID = RESULT_ID_NG;
				ResultStr = CString(strList[0].c_str());	
			}			
			m_AlgResultText = ResultStr;
		}
	}
	bgParam.bgResult=sResult;	

	m_AlgResultID = ResultID;
	WndPtr->SetWndResultID(m_AlgResultID);
	WndPtr->SetWndResultText(m_AlgResultText);	
	if ( CheckAlgSaveDefectImage() == true )
	{
		const bool bFixImg=true;
		if ( sBG.ShowResult(sResult, matGlue) == true )
		{	
			IMAGE_PTR  rPtr = NULL;
			IMAGE_SIZE rW = matGlue.size().width;
			IMAGE_SIZE rH = matGlue.size().height;
			IMAGE_SIZE rBitCount = (matGlue.channels()==1) ? 8:24;
			IMAGE_SIZE rStep = JetAPI::GetBMPImagePixelsPerLine(rW, rBitCount, 4);
			const size_t rBufferSize = ImageAPI.CalcBufferSize(rStep, rH);
			if ( JetMemory.alloc_func(rBufferSize, rPtr, fnName, "rPtr") == true )
			{
				RECT GlueRect;
				JetAPI::SizeToRect(rW, rH, GlueRect);
				if ( ImageAPI.CloneMatData(matGlue, rW, rH, rStep, rBitCount, rPtr, false) == true )
				{	SaveAlgDefectImage(rW, rH, rStep, rBitCount, rPtr, rPtr, GlueRect, bFixImg);	}
				JetMemory.free_func(rPtr);
			}
		}
	}
	JetMemory.free_list(ImageList);
#endif//ALG_MEASURE_BLACK_GLUE_USE
	return true;
}
//-------------------------------------------------------------------------------------//