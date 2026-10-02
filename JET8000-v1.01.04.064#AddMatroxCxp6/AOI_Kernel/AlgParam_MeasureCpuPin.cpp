// AlgParam_MeasureCpuPin.cpp: implementation of the CAlgParam class.
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
bool CAlgParam::WriteAlgParamFile_MeasureCpuPin(const TALG_PARAM_MEASURE_CPU_PIN &cpParam, CAOIFileIO &FileIO)//纗秖代CPU钡竲把计-瓁笷
{	
	CString SaveKey;
	CString Filename;
	CString Filename2;
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_MEASURE_CPU_PIN_START, 0) == false ) { return false; }
	if ( ALG_MEASURE_CPU_PIN == GetAlgType() )
	{
	#ifdef ALG_MEASURE_CPU_PIN_USE
		Filename2=GetAlgParamPlugInFilename(_T("MeasurementCpuPin"));
		if ( Filename2.GetLength() > 0 )
		{
			std::string strPathName;
			CPU_Alignment CpuAlignment;
			JetAPI::ExtractMainFileName(Filename2, Filename);
			JetAPI::TCHAR2string(Filename, strPathName);
			if ( CpuAlignment.Save_Parameter(cpParam.cpParam, strPathName) == false )
			{	return false; }

			CString ProjectFolder=FileIO.GetFileFolder();
			if ( JetAPI::ExtractShortFilename(ProjectFolder, Filename, SaveKey) == false )
			{
				SaveKey.Format(_T("Error, ExtractShortFilename Fault[%s, %s]"), ProjectFolder, Filename);
				FileIO.SetErrorString(SaveKey);
				return false;
			}

			if ( FileIO.SaveChunk_STR(FILE_IO_ALG_PARAM_MEASURE_CPU_PIN_SAVE_KEY, SaveKey) == false ) { return false; }
			if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_MEASURE_CPU_PIN_FRAME_ID_01, cpParam.cpFrameUniqueID1) == false ) { return false; }			
		}
	#endif//ALG_MEASURE_CPU_PIN_USE
	}	
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_MEASURE_CPU_PIN_END, 0) == false ) { return false; }		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ReadAlgParamFile_MeasureCpuPin(TALG_PARAM_MEASURE_CPU_PIN &cpParam, CAOIFileIO &FileIO)//更秖代CPU钡竲把计-瓁笷
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
		case FILE_IO_ALG_PARAM_MEASURE_CPU_PIN_SAVE_KEY:
			if ( FileIO.GetLoadWStr()==true )
			{	SaveKey = FileIO.GetData_WSTR();	}
			else
			{	SaveKey = FileIO.GetData_STR();	}			
			break;
		case FILE_IO_ALG_PARAM_MEASURE_CPU_PIN_FRAME_ID_01://Flux紇钩
			FrameUniqueID1 = FileIO.GetData_INT();
			break;	
		case FILE_IO_ALG_PARAM_MEASURE_CPU_PIN_END://秖代Flux縩-沧翴
			if ( ALG_MEASURE_CPU_PIN==GetAlgType() && SaveKey.GetLength()>0 )
			{	
			#ifdef ALG_MEASURE_CPU_PIN_USE
				std::string strPathName;	
				CPU_Alignment CpuAlignment;				
				cpParam.cpFrameUniqueID1 = FrameUniqueID1;				
				Filename.Format(_T("%s\\%s"), FileIO.GetFileFolder(), SaveKey);				
				JetAPI::TCHAR2string(Filename, strPathName);				
				if ( CpuAlignment.Load_Parameter(strPathName, cpParam.cpParam) == false )
				{						
					CString Err;
					CpuAlignment.GetErrorMessage(Err);
					FileIO.SetErrorString(Err);
					return false; 
				}				
			#endif//ALG_MEASURE_CPU_PIN_USE
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
	FileIO.SetErrorString(_T("Error, ReadAlgParamFile_MeasureCpuPin Fault"));
	return false;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ExecAlgInspection_MeasureCpuPin(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, const std::vector<TUNI_FRAME> &UniFrameList, bool bTestWnd)	
{	
#ifdef ALG_MEASURE_CPU_PIN_USE
	if ( NULL == ModelPtr ) { return false; }	
	const char fnName[]="CAlgParam::ExecAlgInspection_MeasureCpuPin";
	const size_t FrameCount = UniFrameList.size();		
	CAOIComponent *ComponentPtr = ModelPtr->GetModelComponentPtr();	
	TALG_PARAM_MEASURE_CPU_PIN &cpParam=GetAlgParamMeasureCpuPin();	
	const size_t ImageFrameIndex = cpParam.cpFrameIndex1;
	if ( ImageFrameIndex >= FrameCount ) 
	{	return false; }
	
	CAOIWnd         *WndPtr = CAlgParam::GetAlgWndPtr();
	if ( NULL == WndPtr ) { return false; }			
	const unsigned int WndIndex = WndPtr->GetWndIndex();	
	const CAMERA_ID    CameraID = PRIMARY_CAMERA_ID;
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
	IMAGE_PTR  CpuImagePtr  = NULL;	
	IMAGE_SIZE CpuImageW    = WndW;
	IMAGE_SIZE CpuImageH    = WndH;	
	IMAGE_SIZE CpuBitCount  = FrameBitCount;
	IMAGE_SIZE CpuImageStep = JetAPI::GetBMPImagePixelsPerLine(CpuImageW, CpuBitCount, nAlign);
	const size_t FluxBufferSize = ImageAPI.CalcBufferSize(CpuImageStep, CpuImageH);

	if ( JetMemory.alloc_func(FluxBufferSize, CpuImagePtr, fnName, "CpuImagePtr")==false )
	{
		JetMemory.free_func(CpuImagePtr);		
		return false;
	}
	std::vector<IMAGE_PTR> ImageList;
	ImageList.push_back(CpuImagePtr);	
	
	if ( ImageAPI.ExtractRoiImage3(FrameImageW, FrameImageH, FrameImageStep, FrameBitCount, FrameImagePtr, WndRect, CpuImageStep, CpuImagePtr, false)==false )
	{
		JetMemory.free_list(ImageList);
		return false;
	}		
	
	stOutputData sResult;
	CPU_Alignment CpuAlignment;			
	InputData_8000 sParam=cpParam.cpParam;
	cv::Mat matCpu = ImageAPI.CreateMat(CpuImageW, CpuImageH, CpuImageStep, CpuBitCount, CpuImagePtr);		

	sParam.fResolutionX = (float)(1.0/ImageScale.x);
	sParam.fResolutionY = (float)(1.0/ImageScale.y);
	cpParam.cpParam.fResolutionX = (float)(1.0/ImageScale.x);
	cpParam.cpParam.fResolutionY = (float)(1.0/ImageScale.y);
	
	// 秨币瓜
	const bool bSaveImage=false;
	std::string  strSaveFolder;
	CString      DebugFilename;
	CString      ComponentName;	
	CString      DebugFolder=GetAlgDebugFolder();
	if ( NULL != ComponentPtr )
	{	ComponentName = ComponentPtr->GetComponentFullName(); }			
	DebugFilename.Format(_T("%s\\%s_ModelWndAlgMeasureCpuPin#%d"), DebugFolder, ComponentName, WndIndex+1);
	JetAPI::TCHAR2string(DebugFilename, strSaveFolder);	
	//CpuAlignment.SetSaveImage(bSaveImage);
	//CpuAlignment.SetSavePathName(strSaveFolder);	
	
	RESULT_ID ResultID=RESULT_ID_OK;	
	bool bOk = CpuAlignment.CPUAlignment_Inspection(matCpu, sParam, sResult);
	if ( false == bOk )
	{
		ResultID = RESULT_ID_NG;	
		CpuAlignment.GetErrorMessage(m_AlgResultText);		 
	}
	else
	{	
		CString ResultStr;
		ResultID = RESULT_ID_NONE;			
		const size_t Count = sResult.vtResult.size();
		for ( size_t i=0; i<Count; i++ )
		{
			const auto &rResult = sResult.vtResult[i];
			if ( false == rResult.bOkNg )
			{	
				ResultID = RESULT_ID_NG;
				ResultStr.Format(_T("CPU Pin Fault[%d, (%d, %d)]"), rResult.nNo, rResult.ptOffset.x, rResult.ptOffset.y); 
				break;
			}
		}		

		if ( RESULT_ID_NONE == ResultID )
		{
			ResultStr = _T("OK");
			ResultID = RESULT_ID_OK; 
		}
		m_AlgResultText = ResultStr;
	}
	cpParam.cpResult=sResult;
	//CpuAlignment.ShowResult(sResult, matCpu);

	m_AlgResultID = ResultID;
	WndPtr->SetWndResultID(m_AlgResultID);
	WndPtr->SetWndResultText(m_AlgResultText);	
	//SaveAlgDefectImage(MaskW, MaskH, MaskStep, MaskBitCount, GrayPtr, MaskPtr, WndRect);		
	JetMemory.free_list(ImageList);
#endif//ALG_MEASURE_CPU_PIN_USE
	return true;
}
//-------------------------------------------------------------------------------------//