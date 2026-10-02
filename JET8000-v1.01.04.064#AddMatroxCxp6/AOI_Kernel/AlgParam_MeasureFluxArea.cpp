// AlgParam_MeasureFluxArea.cpp: implementation of the CAlgParam class.
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
bool CAlgParam::WriteAlgParamFile_MeasureFluxArea(const TALG_PARAM_MEASURE_FLUX_AREA &faParam, CAOIFileIO &FileIO)//纗秖代Flux縩把计-瓁笷	
{	
	CString SaveKey;
	CString Filename;
	CString Filename2;
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_MEASURE_FLUX_AREA_START, 0) == false ) { return false; }
	if ( ALG_MEASURE_FLUX_AREA == GetAlgType() )
	{
	#ifdef ALG_MEASURE_FLUX_AREA_USE
		Filename2=GetAlgParamPlugInFilename(_T("MeasurementFluxArea"));
		if ( Filename2.GetLength() > 0 )
		{
			std::string strPathName;
			JET::alg::MeasurementBlackGlue sBG;	
			JetAPI::ExtractMainFileName(Filename2, Filename);
			JetAPI::TCHAR2string(Filename, strPathName);
			if ( sBG.SaveParameter_FluxArea(faParam.faParam, strPathName) == false )
			{	return false; }

			CString ProjectFolder=FileIO.GetFileFolder();
			if ( JetAPI::ExtractShortFilename(ProjectFolder, Filename, SaveKey) == false )
			{
				SaveKey.Format(_T("Error, ExtractShortFilename Fault[%s, %s]"), ProjectFolder, Filename);
				FileIO.SetErrorString(SaveKey);
				return false;
			}

			if ( FileIO.SaveChunk_STR(FILE_IO_ALG_PARAM_MEASURE_FLUX_AREA_SAVE_KEY, SaveKey) == false ) { return false; }
			if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_MEASURE_FLUX_AREA_FRAME_ID_01, faParam.faFrameUniqueID1) == false ) { return false; }			
		}
	#endif//ALG_MEASURE_FLUX_AREA_USE
	}	
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_MEASURE_FLUX_AREA_END, 0) == false ) { return false; }		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ReadAlgParamFile_MeasureFluxArea(TALG_PARAM_MEASURE_FLUX_AREA &faParam, CAOIFileIO &FileIO)//更秖代Flux縩把计-瓁笷
{	
	int     index = 0;
	CString Filename;
	CString SaveKey;		
	const CAlgBinaryParam &ImageBinaryParam=GetAlgImageBinParam();		
	unsigned int FrameUniqueID1 = FRAME_UNIQUE_ID_DEFAULT;	
	FrameUniqueID1=ImageBinaryParam.GetBinaryFrameUniqueID();	
	while ( FileIO.CheckFileEnd()==false )
	{ 		
		if ( FileIO.LoadChunk(index) == false )		
		{	continue; }
		switch ( index )
		{	
		case FILE_IO_ALG_PARAM_MEASURE_FLUX_AREA_SAVE_KEY:
			if ( FileIO.GetLoadWStr()==true )
			{	SaveKey = FileIO.GetData_WSTR();	}
			else
			{	SaveKey = FileIO.GetData_STR();	}			
			break;
		case FILE_IO_ALG_PARAM_MEASURE_FLUX_AREA_FRAME_ID_01://Flux紇钩
			FrameUniqueID1 = FileIO.GetData_INT();
			break;	
		case FILE_IO_ALG_PARAM_MEASURE_FLUX_AREA_END://秖代Flux縩-沧翴
			if ( ALG_MEASURE_FLUX_AREA==GetAlgType() && SaveKey.GetLength()>0 )
			{	
			#ifdef ALG_MEASURE_FLUX_AREA_USE
				std::string strPathName;
				JET::alg::MeasurementBlackGlue sBG;	
				faParam.faFrameUniqueID1 = FrameUniqueID1;				
				Filename.Format(_T("%s\\%s"), FileIO.GetFileFolder(), SaveKey);				
				JetAPI::TCHAR2string(Filename, strPathName);				
				if ( sBG.LoadParameter_FluxArea(strPathName, faParam.faParam) == false )
				{	
					CString Err=sBG.GetErrorMessage().c_str();
					FileIO.SetErrorString(Err);
					return false; 
				}				
			#endif//ALG_MEASURE_FLUX_AREA_USE
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
	FileIO.SetErrorString(_T("Error, ReadAlgParamFile_MeasureFluxArea Fault"));
	return false;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ExecAlgInspection_MeasureFluxArea(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, const std::vector<TUNI_FRAME> &UniFrameList, bool bTestWnd)	
{	
#ifdef ALG_MEASURE_FLUX_AREA_USE
	if ( NULL == ModelPtr ) { return false; }	
	const char fnName[]="CAlgParam::ExecAlgInspection_MeasureFluxArea";
	const size_t FrameCount = UniFrameList.size();		
	CAOIComponent *ComponentPtr = ModelPtr->GetModelComponentPtr();	
	TALG_PARAM_MEASURE_FLUX_AREA &faParam=GetAlgParamMeasureFluxArea();	
	const size_t ImageFrameIndex = faParam.faFrameIndex1;
	if ( ImageFrameIndex >= FrameCount ) 
	{	return false; }
	
	CAOIWnd         *WndPtr = CAlgParam::GetAlgWndPtr();
	if ( NULL == WndPtr ) { return false; }			
	const unsigned int WndIndex = WndPtr->GetWndIndex();	
	const CAMERA_ID    CameraID = PRIMARY_CAMERA_ID;;
	const TUNI_FRAME *UniFramePtr  = &(UniFrameList[ImageFrameIndex]);
	const IMAGE_SIZE FrameImageW    = UniFramePtr->ImageW;
	const IMAGE_SIZE FrameImageH    = UniFramePtr->ImageH;
	const IMAGE_SIZE FrameImageStep = UniFramePtr->ImageStep;
	const IMAGE_SIZE FrameBitCount  = UniFramePtr->BitCount;		
	IMAGE_PTR        FrameImagePtr  = UniFramePtr->ImagePtr;
	MASK_PTR         FrameMaskPtr   = UniFramePtr->MaskPtr;
	SPACE_PTR        FrameSpacePtr  = UniFramePtr->SpacePtr;

	const TPOINT2D  &ImageScale     = ModelPtr->GetModelImageScale();	

	const int WndW=WndRect.right-WndRect.left;
	const int WndH=WndRect.bottom-WndRect.top;
	const int RoiW = RoiRect.right-RoiRect.left;
	const int RoiH = RoiRect.bottom-RoiRect.top;
	const int RoiRectSize = (int)((RoiRect.right-RoiRect.left)*(RoiRect.bottom-RoiRect.top));
	if ( NULL == FrameImagePtr )
	{	return false;	}	

	const int nAlign = 1;
	IMAGE_PTR  FluxImagePtr  = NULL;	
	IMAGE_SIZE FluxImageW    = WndW;
	IMAGE_SIZE FluxImageH    = WndH;	
	IMAGE_SIZE FluxBitCount  = FrameBitCount;
	IMAGE_SIZE FluxImageStep = JetAPI::GetBMPImagePixelsPerLine(FluxImageW, FluxBitCount, nAlign);
	const size_t FluxBufferSize = ImageAPI.CalcBufferSize(FluxImageStep, FluxImageH);

	if ( JetMemory.alloc_func(FluxBufferSize, FluxImagePtr, fnName, "FluxImagePtr")==false )
	{
		JetMemory.free_func(FluxImagePtr);		
		return false;
	}
	std::vector<IMAGE_PTR> ImageList;
	ImageList.push_back(FluxImagePtr);	
	
	if ( ImageAPI.ExtractRoiImage3(FrameImageW, FrameImageH, FrameImageStep, FrameBitCount, FrameImagePtr, WndRect, FluxImageStep, FluxImagePtr, false)==false )
	{
		JetMemory.free_list(ImageList);
		return false;
	}		
	
	JET::alg::MeasurementBlackGlue sBG;
	JET::alg::SMeasurementFluxArea_Result sResult;
	JET::alg::SMeasurementFluxArea_Parameter sParam=faParam.faParam;
	cv::Mat matFlux = ImageAPI.CreateMat(FluxImageW, FluxImageH, FluxImageStep, FluxBitCount, FluxImagePtr);		

	sParam.fResolutionX = (float)(1.0/ImageScale.x);
	sParam.fResolutionY = (float)(1.0/ImageScale.y);
	faParam.faParam.fResolutionX = (float)(1.0/ImageScale.x);
	faParam.faParam.fResolutionY = (float)(1.0/ImageScale.y);
	
	// 秨币瓜
	const bool bSaveImage=false;
	std::string  strSaveFolder;
	CString      DebugFilename;
	CString      ComponentName;	
	CString      DebugFolder=GetAlgDebugFolder();
	if ( NULL != ComponentPtr )
	{	ComponentName = ComponentPtr->GetComponentFullName(); }			
	DebugFilename.Format(_T("%s\\%s_ModelWndAlgMeasureFluxArea#%d"), DebugFolder, ComponentName, WndIndex+1);
	JetAPI::TCHAR2string(DebugFilename, strSaveFolder);	
	sBG.SetSaveImage(bSaveImage);
	sBG.SetSavePathName(strSaveFolder);
	
	RESULT_ID ResultID=RESULT_ID_OK;	
	bool bOk = sBG.Measurement_FluxArea(matFlux, sParam, sResult);
	if ( false == bOk )
	{
		ResultID = RESULT_ID_NG;	
		m_AlgResultText = sBG.GetErrorMessage().c_str();
	}
	else
	{	
		CString ResultStr;
		ResultID = RESULT_ID_NONE;			
		
		if ( sResult.bResult == false )
		{
			ResultID = RESULT_ID_NG;				
			ResultStr.Format(_T("Flux Fault[%.2f]"), sResult.fAreaRatio);	
		}

		if ( RESULT_ID_NONE == ResultID )
		{
			ResultStr = _T("OK");
			ResultID = RESULT_ID_OK; 
		}
		m_AlgResultText = ResultStr;
	}
	faParam.faResult=sResult;
	//sBG.ShowResult(sResult, matGlue);

	m_AlgResultID = ResultID;
	WndPtr->SetWndResultID(m_AlgResultID);
	WndPtr->SetWndResultText(m_AlgResultText);	
	//SaveAlgDefectImage(MaskW, MaskH, MaskStep, MaskBitCount, GrayPtr, MaskPtr, WndRect);	
	JetMemory.free_list(ImageList);
#endif//ALG_MEASURE_FLUX_AREA_USE
	return true;
}
//-------------------------------------------------------------------------------------//