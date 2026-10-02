// AlgParam_BlobCount.cpp: implementation of the CAlgParam class.
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
bool CAlgParam::CheckOK_BlobCount(const TALG_PARAM_BLOB_COUNT &Param)
{
	if ( CheckOK_BlobCountUSL(Param) == false ) 
	{	return false; }
	if ( CheckOK_BlobCountLSL(Param) == false ) 
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_BlobCountUSL(const TALG_PARAM_BLOB_COUNT &Param)
{
	if ( Param.bcCountNum > Param.bcCountUSL ) 
	{	return false;	}		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_BlobCountLSL(const TALG_PARAM_BLOB_COUNT &Param)
{
	if ( Param.bcCountNum < Param.bcCountLSL ) 
	{	return false;	}		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::WriteAlgParamFile_BlobCount(const TALG_PARAM_BLOB_COUNT &blobParam, CAOIFileIO &FileIO)//儲存區塊數量參數
{		
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_BLOB_COUNT_START, 0) == false ) { return false; }

	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_BLOB_COUNT_COUNT_USL, blobParam.bcCountUSL) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_BLOB_COUNT_COUNT_LSL, blobParam.bcCountLSL) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_BLOB_COUNT_CONNECTIVITY, blobParam.bcConnectivity) == false ) { return false; }

	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_BLOB_COUNT_X_MAX, blobParam.bcXSizeMax) == false ) { return false; }
	if ( FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_BLOB_COUNT_X_MAX_ENABLED, blobParam.bcXSizeMaxEnabled) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_BLOB_COUNT_X_MIN, blobParam.bcXSizeMin) == false ) { return false; }
	if ( FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_BLOB_COUNT_X_MIN_ENABLED, blobParam.bcXSizeMinEnabled) == false ) { return false; }

	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_BLOB_COUNT_Y_MAX, blobParam.bcYSizeMax) == false ) { return false; }
	if ( FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_BLOB_COUNT_Y_MAX_ENABLED, blobParam.bcYSizeMaxEnabled) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_BLOB_COUNT_Y_MIN, blobParam.bcYSizeMin) == false ) { return false; }
	if ( FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_BLOB_COUNT_Y_MIN_ENABLED, blobParam.bcYSizeMinEnabled) == false ) { return false; }

	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_BLOB_COUNT_AREA_MAX, blobParam.bcAreaSizeMax) == false ) { return false; }
	if ( FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_BLOB_COUNT_AREA_MAX_ENABLED, blobParam.bcAreaSizeMaxEnabled) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_BLOB_COUNT_AREA_MIN, blobParam.bcAreaSizeMin) == false ) { return false; }
	if ( FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_BLOB_COUNT_AREA_MIN_ENABLED, blobParam.bcAreaSizeMinEnabled) == false ) { return false; }

	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_BLOB_COUNT_ASPECT_RATIO_MAX, blobParam.bcAspectRatioMax) == false ) { return false; }
	if ( FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_BLOB_COUNT_ASPECT_RATIO_MAX_ENABLED, blobParam.bcAspectRatioMaxEnabled ) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_BLOB_COUNT_ASPECT_RATIO_MIN, blobParam.bcAspectRatioMin ) == false ) { return false; }
	if ( FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_BLOB_COUNT_ASPECT_RATIO_MIN_ENABLED, blobParam.bcAspectRatioMinEnabled ) == false ) { return false; }

	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_BLOB_COUNT_LONG_SHORT_RATIO_MAX, blobParam.bcLongShortRatioMax) == false ) { return false; }
	if ( FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_BLOB_COUNT_LONG_SHORT_RATIO_MAX_ENB, blobParam.bcLongShortRatioMaxEnabled ) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_BLOB_COUNT_LONG_SHORT_RATIO_MIN, blobParam.bcLongShortRatioMin ) == false ) { return false; }
	if ( FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_BLOB_COUNT_LONG_SHORT_RATIO_MIN_ENB, blobParam.bcLongShortRatioMinEnabled ) == false ) { return false; }

	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_BLOB_COUNT_FILL_RATIO_MAX, blobParam.bcFillRatioMax) == false ) { return false; }
	if ( FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_BLOB_COUNT_FILL_RATIO_MAX_ENABLED, blobParam.bcFillRatioMaxEnabled ) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_BLOB_COUNT_FILL_RATIO_MIN, blobParam.bcFillRatioMin ) == false ) { return false; }
	if ( FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_BLOB_COUNT_FILL_RATIO_MIN_ENABLED, blobParam.bcFillRatioMinEnabled ) == false ) { return false; }	

	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_BLOB_COUNT_L_MAX, blobParam.bcLSizeMax) == false ) { return false; }
	if ( FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_BLOB_COUNT_L_MAX_ENABLED, blobParam.bcLSizeMaxEnabled) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_BLOB_COUNT_L_MIN, blobParam.bcLSizeMin) == false ) { return false; }
	if ( FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_BLOB_COUNT_L_MIN_ENABLED, blobParam.bcLSizeMinEnabled) == false ) { return false; }

	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_BLOB_COUNT_ROI_BOX_EXTEND_X, blobParam.bcRoiBoxExtendX) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_BLOB_COUNT_ROI_BOX_EXTEND_Y, blobParam.bcRoiBoxExtendY) == false ) { return false; }	

	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_BLOB_COUNT_END, 0) == false ) { return false; }			

	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ReadAlgParamFile_BlobCount(TALG_PARAM_BLOB_COUNT &blobParam, CAOIFileIO &FileIO)//載入區塊數量參數
{
	int       index = 0;
	while ( FileIO.CheckFileEnd()==false )
	{ 		
		if ( FileIO.LoadChunk(index) == false )		
		{	continue; }
		switch ( index )
		{
		case FILE_IO_ALG_PARAM_BLOB_COUNT_END://區塊數量參數-終點
			return true;
			break;
		case FILE_IO_ALG_PARAM_BLOB_COUNT_COUNT_USL://區塊數量最大值
			blobParam.bcCountUSL = FileIO.GetData_INT();
			break;
		case FILE_IO_ALG_PARAM_BLOB_COUNT_COUNT_LSL://區塊數量最小值
			blobParam.bcCountLSL = FileIO.GetData_INT();
			break;
		case FILE_IO_ALG_PARAM_BLOB_COUNT_CONNECTIVITY://區塊鄰近模式
			blobParam.bcConnectivity = FileIO.GetData_INT();
			break;
		case FILE_IO_ALG_PARAM_BLOB_COUNT_X_MAX://區塊X尺寸最大值
			blobParam.bcXSizeMax = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_BLOB_COUNT_X_MAX_ENABLED://區塊X尺寸最大值啟用
			blobParam.bcXSizeMaxEnabled = FileIO.GetData_BOL();	
			break;
		case FILE_IO_ALG_PARAM_BLOB_COUNT_X_MIN://區塊X尺寸最小值
			blobParam.bcXSizeMin = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_BLOB_COUNT_X_MIN_ENABLED://區塊X尺寸最小值啟用
			blobParam.bcXSizeMinEnabled = FileIO.GetData_BOL();	
			break;
		case FILE_IO_ALG_PARAM_BLOB_COUNT_Y_MAX://區塊Y尺寸最大值
			blobParam.bcYSizeMax = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_BLOB_COUNT_Y_MAX_ENABLED://區塊Y尺寸最大值啟用
			blobParam.bcYSizeMaxEnabled = FileIO.GetData_BOL();	
			break;
		case FILE_IO_ALG_PARAM_BLOB_COUNT_Y_MIN://區塊Y尺寸最小值
			blobParam.bcYSizeMin = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_BLOB_COUNT_Y_MIN_ENABLED://區塊Y尺寸最小值啟用
			blobParam.bcYSizeMinEnabled = FileIO.GetData_BOL();	
			break;
		case FILE_IO_ALG_PARAM_BLOB_COUNT_AREA_MAX://區塊面積尺寸最大值
			blobParam.bcAreaSizeMax = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_BLOB_COUNT_AREA_MAX_ENABLED://區塊面積尺寸最大值啟用
			blobParam.bcAreaSizeMaxEnabled = FileIO.GetData_BOL();
			break;
		case FILE_IO_ALG_PARAM_BLOB_COUNT_AREA_MIN://區塊面積尺寸最小值
			blobParam.bcAreaSizeMin = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_BLOB_COUNT_AREA_MIN_ENABLED://區塊面積尺寸最小值啟用
			blobParam.bcAreaSizeMinEnabled = FileIO.GetData_BOL();
			break;
		case FILE_IO_ALG_PARAM_BLOB_COUNT_ASPECT_RATIO_MAX://區塊長寬比最大值
			blobParam.bcAspectRatioMax = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_BLOB_COUNT_ASPECT_RATIO_MAX_ENABLED://區塊長寬比最大值啟用
			blobParam.bcAspectRatioMaxEnabled  = FileIO.GetData_BOL();
			break;			
		case FILE_IO_ALG_PARAM_BLOB_COUNT_ASPECT_RATIO_MIN://區塊長寬比最小值
			blobParam.bcAspectRatioMin = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_BLOB_COUNT_ASPECT_RATIO_MIN_ENABLED://區塊長寬比最小值啟用
			blobParam.bcAspectRatioMinEnabled  = FileIO.GetData_BOL();
			break;
		case FILE_IO_ALG_PARAM_BLOB_COUNT_LONG_SHORT_RATIO_MAX://區塊長短比最大值
			blobParam.bcLongShortRatioMax = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_BLOB_COUNT_LONG_SHORT_RATIO_MAX_ENB://區塊長短比最大值啟用
			blobParam.bcLongShortRatioMaxEnabled  = FileIO.GetData_BOL();
			break;			
		case FILE_IO_ALG_PARAM_BLOB_COUNT_LONG_SHORT_RATIO_MIN://區塊長短比最小值
			blobParam.bcLongShortRatioMin = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_BLOB_COUNT_LONG_SHORT_RATIO_MIN_ENB://區塊長短比最小值啟用
			blobParam.bcLongShortRatioMinEnabled  = FileIO.GetData_BOL();
			break;
		case FILE_IO_ALG_PARAM_BLOB_COUNT_FILL_RATIO_MAX://區塊填滿率最大值
			blobParam.bcFillRatioMax = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_BLOB_COUNT_FILL_RATIO_MAX_ENABLED://區塊填滿率最大值啟用
			blobParam.bcFillRatioMaxEnabled  = FileIO.GetData_BOL();
			break;			
		case FILE_IO_ALG_PARAM_BLOB_COUNT_FILL_RATIO_MIN://區塊填滿率最小值
			blobParam.bcFillRatioMin = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_BLOB_COUNT_FILL_RATIO_MIN_ENABLED://區塊填滿率最小值啟用
			blobParam.bcFillRatioMinEnabled  = FileIO.GetData_BOL();
			break;	
		case FILE_IO_ALG_PARAM_BLOB_COUNT_L_MAX://區塊L尺寸最大值
			blobParam.bcLSizeMax = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_BLOB_COUNT_L_MAX_ENABLED://區塊L尺寸最大值啟用
			blobParam.bcLSizeMaxEnabled = FileIO.GetData_BOL();	
			break;
		case FILE_IO_ALG_PARAM_BLOB_COUNT_L_MIN://區塊L尺寸最小值
			blobParam.bcLSizeMin = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_BLOB_COUNT_L_MIN_ENABLED://區塊L尺寸最小值啟用
			blobParam.bcLSizeMinEnabled = FileIO.GetData_BOL();	
			break;	
		case FILE_IO_ALG_PARAM_BLOB_COUNT_ROI_BOX_EXTEND_X://區塊小框延伸尺寸X-um
			blobParam.bcRoiBoxExtendX = FileIO.GetData_FLT();
			break;
		case FILE_IO_ALG_PARAM_BLOB_COUNT_ROI_BOX_EXTEND_Y://區塊小框延伸尺寸Y-um
			blobParam.bcRoiBoxExtendY = FileIO.GetData_FLT();
			break;
		default:
		#ifdef _DEBUG
			index = index;
		#endif//_DEBUG
			break;
		}
	}
	FileIO.SetErrorString(_T("Error, ReadAlgParamFile_BlobCount Fault"));
	return false;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ExecAlgInspection_BlobCount(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList)
{
	if ( NULL == ModelPtr ) { return false; }
	const size_t FrameCount = UniFrameList.size();	
	const size_t MaskFrameIndex = m_AlgMaskBinParam.GetBinaryFrameIndex();
	const size_t ImageFrameIndex = m_AlgImageBinParam.GetBinaryFrameIndex();
	if ( ImageFrameIndex >= FrameCount ) 
	{	return false; }
	
	CAOIWnd         *WndPtr = CAlgParam::GetAlgWndPtr();
	if ( NULL == WndPtr ) { return false; }	

	BOX_TOWARD      WndToward = WndPtr->GetWndToward();
	TUNI_FRAME      *UniFramePtr  = &(UniFrameList[ImageFrameIndex]);
	const IMAGE_SIZE FrameImageW    = UniFramePtr->ImageW;
	const IMAGE_SIZE FrameImageH    = UniFramePtr->ImageH;
	const IMAGE_SIZE FrameImageStep = UniFramePtr->ImageStep;
	const IMAGE_SIZE FrameBitCount  = UniFramePtr->BitCount;		
	IMAGE_PTR        FrameImagePtr  = UniFramePtr->ImagePtr;
	MASK_PTR         FrameMaskPtr   = UniFramePtr->MaskPtr;
	SPACE_PTR        FrameSpacePtr  = UniFramePtr->SpacePtr;
	TALG_PARAM_BLOB_COUNT &blobParam= GetAlgParamBlobCount();
	const int RoiRectSize = (int)((RoiRect.right-RoiRect.left)*(RoiRect.bottom-RoiRect.top));	
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
	
	if ( WndPtr->CheckWndNeedShapeMask() == true )
	{
		MASK_PTR      ShapePtr  = NULL;
		IMAGE_SIZE    ShapeW = MaskW;
		IMAGE_SIZE    ShapeH = MaskH;					
		IMAGE_SIZE    ShapeStep = MaskStep;
		const size_t  ShapeBufferSize = ImageAPI.CalcBufferSize(ShapeStep, ShapeH);
		if ( JetMemory.alloc_func(ShapeBufferSize, ShapePtr, "CAlgParam::ExecAlgInspection_BlobCount", "ShapePtr") == false )
		{	
			JetMemory.free_func(MaskPtr);
			JetMemory.free_func(GrayPtr);
			return false; 
		}		
		ImageAPI.FillImageRoi(ShapeW, ShapeH, ShapeStep, 8, ShapePtr, RoiRect, 0, 0, 0);
		if ( WndPtr->BuildWndShapeMask3(ShapeW, ShapeH, ShapeStep, ShapePtr, RoiRect) == false )
		{			
			JetMemory.free_func(MaskPtr);
			JetMemory.free_func(GrayPtr);
			JetMemory.free_func(ShapePtr);
			return false;
		}	
		if ( ImageAPI.Intersection2MaskImage3(MaskW, MaskH, MaskStep, MaskPtr, ShapePtr, RoiRect, MaskStep, MaskPtr, 255, 0, true) == false )
		{
			JetMemory.free_func(MaskPtr);
			JetMemory.free_func(GrayPtr);
			JetMemory.free_func(ShapePtr);
			return false;
		}
		JetMemory.free_func(ShapePtr);
	}

	const size_t WndRoiCount = WndPtr->GetWndRoiWndCount();
	if ( WndRoiCount > 0 )
	{
		const bool UsResPos=true;	
		IMAGE_PTR  WndMskRoiPtr=NULL;		
		const int  sX = JetAPI::ToInt(blobParam.bcRoiBoxExtendX*ImageScale.x);
		const int  sY = JetAPI::ToInt(blobParam.bcRoiBoxExtendY*ImageScale.y);
		const size_t WndMaskBufferSize = ImageAPI.CalcBufferSize(MaskStep, MaskH);
		if ( JetMemory.alloc_func(WndMaskBufferSize, WndMskRoiPtr, "CAlgParam::ExecAlgInspection_BlobCount", "WndMskRoiPtr") == true )
		{	
			if ( WndPtr->BuildWndRoiImage(MaskW, MaskH, MaskStep, MaskBitCount, WndMskRoiPtr, WndRect, sX, sY, UsResPos) == false )
			{
				JetMemory.free_func(MaskPtr);
				JetMemory.free_func(GrayPtr);
				return false; 
			}						
			for ( size_t i=0; i<WndMaskBufferSize; i++ )
			{
				if ( 0 != WndMskRoiPtr[i] )
				{	continue;	}
				MaskPtr[i] = 0x00;
			}			
			JetMemory.free_func(WndMskRoiPtr);
		}
	}

	CJetBlob BlobDetector;
	BlobDetector.InitialBlob();
	BlobDetector.SetBlobSortMode(BLOB_SORT_PIXELS);
	if ( 8 == blobParam.bcConnectivity )//BLOB_CONNECTIVITY_4, BLOB_CONNECTIVITY_8
	{	BlobDetector.SetBlobConnectivity(BLOB_CONNECTIVITY_8);	}
	else
	{	BlobDetector.SetBlobConnectivity(BLOB_CONNECTIVITY_4);	}	
	if ( BlobDetector.GrayImageRoiBlobDetect(MaskW, MaskH, MaskStep, MaskPtr, RoiRect, 128, 255) == false )
	{	
		JetMemory.free_func(MaskPtr);
		JetMemory.free_func(GrayPtr);
		return false;
	}	

	size_t i=0;	
	TPOINT2D     BoxPos;
	TSIZE2D      BoxSize;	
	RECT         BlobRect={0,0,0,0};
	CAOIBox      ResBox, *BoxPtr=NULL;
	double       BlobW=0, BlobH=0, BlobD=0, BlobArea=0, BlobAspectRatio=0, BlobFillRatio=0, BlobLongShortRatio=0;	
	TBlobResult *BlobPtr=NULL;	
	const size_t BlobResCount = BlobDetector.GetBlobCount();
	std::vector<TBlobResult> BlobList(BlobResCount);	
	
	TPOINT2D           RgnCp;
	TPOINT2D           ImageCp;
	TREGION4D          BoxRgn;
	TREGION4D          WndRgn;

	ImageCp.x = MaskW;
	ImageCp.y = MaskH;
	ImageCp.x = ImageCp.x*0.5;
	ImageCp.y = ImageCp.y*0.5;
	RgnCp.x = ModelRgn.GetCpX();
	RgnCp.y = ModelRgn.GetCpY();	
	
	TPOINT2D           WndRectCalValue;	
	const double ScaleX=1.0/ImageScale.x;
	const double ScaleY=1.0/ImageScale.y;
	const unsigned int WndIndex = WndPtr->GetWndIndex();		

	const double BlobMaxW = (blobParam.bcXSizeMax);
	const double BlobMinW = (blobParam.bcXSizeMin);
	const double BlobMaxH = (blobParam.bcYSizeMax);
	const double BlobMinH = (blobParam.bcYSizeMin);
	const double BlobMaxD = (blobParam.bcLSizeMax);
	const double BlobMinD = (blobParam.bcLSizeMin);
	const double BlobMaxA = (blobParam.bcAreaSizeMax);
	const double BlobMinA = (blobParam.bcAreaSizeMin);
	const bool BlobMaxWEnabled = blobParam.bcXSizeMaxEnabled;
	const bool BlobMinWEnabled = blobParam.bcXSizeMinEnabled;
	const bool BlobMaxHEnabled = blobParam.bcYSizeMaxEnabled;
	const bool BlobMinHEnabled = blobParam.bcYSizeMinEnabled;
	const bool BlobMaxDEnabled = blobParam.bcLSizeMaxEnabled;
	const bool BlobMinDEnabled = blobParam.bcLSizeMinEnabled;
	const bool BlobMaxAEnabled = blobParam.bcAreaSizeMaxEnabled;
	const bool BlobMinAEnabled = blobParam.bcAreaSizeMinEnabled;
	const double BlobMaxAspectR = blobParam.bcAspectRatioMax;
	const double BlobMinAspectR = blobParam.bcAspectRatioMin;
	const bool BlobMaxAspectREnabled = blobParam.bcAspectRatioMaxEnabled;
	const bool BlobMinAspectREnabled = blobParam.bcAspectRatioMinEnabled;
	const double BlobMaxFillR = blobParam.bcFillRatioMax;
	const double BlobMinFillR = blobParam.bcFillRatioMin;
	const bool BlobMaxFillREnabled = blobParam.bcFillRatioMaxEnabled;
	const bool BlobMinFillREnabled = blobParam.bcFillRatioMinEnabled;	
	const double BlobMaxLongShort = blobParam.bcLongShortRatioMax;
	const double BlobMinLongShort = blobParam.bcLongShortRatioMin;
	const bool BlobMaxLongShortEnabled = blobParam.bcLongShortRatioMaxEnabled;
	const bool BlobMinLongShortEnabled = blobParam.bcLongShortRatioMinEnabled;	

	WndPtr->GetWndRectCalValue(WndRectCalValue);
	WndPtr->GetWndRegion(WndRgn);
	//WndRectCalValue.x = -WndRectCalValue.x;
	//WndRectCalValue.y = -WndRectCalValue.y;

	BlobList.clear();
	for ( i=0; i<BlobResCount; i++ )
	{
		BlobPtr = BlobDetector.GetBlobPtr(i, false);
		if ( NULL == BlobPtr ) { continue; }
		//BlobRect = BlobPtr->m_BlobRectRaw;
		BlobRect = BlobPtr->m_BlobRect;//20230628-Blob
		BlobW = BlobRect.right-BlobRect.left;
		BlobH = BlobRect.bottom-BlobRect.top;
		BlobArea = BlobPtr->m_BlobPixels;		
		BlobW *= ScaleX;
		BlobH *= ScaleY;
		BlobArea *= ScaleX*ScaleY;
		BlobD = sqrt((BlobW*BlobW)+(BlobH*BlobH));
		BlobAspectRatio = JetAPI::CalcBlobRatio(BlobW, BlobH, WndToward)*100.0;
		BlobFillRatio = BlobArea*100.0/(BlobW*BlobH);
		BlobLongShortRatio = BlobDetector.CalcBlobLongShortRatio(BlobW, BlobH)*100.0;

		if ( true == BlobMaxWEnabled ) 
		{
			if ( BlobW > BlobMaxW ) { continue; }
		}
		if ( true == BlobMinWEnabled ) 
		{
			if ( BlobW < BlobMinW ) { continue; }
		}

		if ( true == BlobMaxHEnabled ) 
		{
			if ( BlobH > BlobMaxH ) { continue; }
		}
		if ( true == BlobMinHEnabled ) 
		{
			if ( BlobH < BlobMinH ) { continue; }
		}
	
		if ( true == BlobMaxDEnabled ) 
		{
			if ( BlobD > BlobMaxD ) { continue; }
		}
		if ( true == BlobMinDEnabled ) 
		{
			if ( BlobD < BlobMinD ) { continue; }
		}		

		if ( true == BlobMaxAEnabled ) 
		{
			if ( BlobArea > BlobMaxA ) { continue; }
		}
		if ( true == BlobMinAEnabled ) 
		{
			if ( BlobArea < BlobMinA ) { continue; }
		}
		
		if ( true == BlobMaxAspectREnabled ) 
		{
			if ( BlobAspectRatio > BlobMaxAspectR ) { continue; }
		}
		if ( true == BlobMinAspectREnabled ) 
		{
			if ( BlobAspectRatio < BlobMinAspectR ) { continue; }
		}

		if ( true == BlobMaxFillREnabled ) 
		{
			if ( BlobFillRatio > BlobMaxFillR ) { continue; }
		}
		if ( true == BlobMinFillREnabled ) 
		{
			if ( BlobFillRatio < BlobMinFillR ) { continue; }
		}

		if ( true == BlobMaxLongShortEnabled ) 
		{
			if ( BlobLongShortRatio > BlobMaxLongShort ) { continue; }
		}
		if ( true == BlobMinLongShortEnabled ) 
		{
			if ( BlobLongShortRatio < BlobMinLongShort ) { continue; }
		}

		BlobList.push_back(*BlobPtr);

		BoxPtr = WndPtr->GetWndBoxPtr();

		CAOIModel::CalcModelBoxRectRegion(BlobRect, MaskW, MaskH, RgnCp, ImageScale, ImageCp, BoxRgn);
		//JetAPI::MoveRegion(BoxRgn, WndRectCalValue, BoxRgn);
		ResBox.SetBoxRegion(BoxRgn);			
		WndPtr->AddWndResultBox(ResBox);
	}
	
	const size_t BlobCount = BlobList.size();	

	//Judge OK/NG
	CString strResult;	
	RESULT_ID ResultID=RESULT_ID_OK;
	SetAlgResultID(ResultID);

	blobParam.bcCountNum = (int)(BlobCount);
	SetAlgResultReading1(BlobCount);
	SetAlgResultText(_T("OK"));

	int USL = blobParam.bcCountUSL;
	int LSL = blobParam.bcCountLSL;
	int Value = blobParam.bcCountNum;
	if ( Value>USL || Value<LSL )
	{	
		CString Key = AOIDataDefine.GetCountText();
		strResult.Format(_T("%s:%d (%d ~ %d)"), Key, Value, LSL, USL);
		ResultID = RESULT_ID_NG;
		SetAlgResultID(ResultID);
		SetAlgResultText(strResult);
	}
	
	CAOIBox     *ResBoxPtr=NULL;
	const size_t ResultBoxCount = WndPtr->GetWndResultBoxCount();
	for ( i=0; i<ResultBoxCount; i++ )
	{
		ResBoxPtr = WndPtr->GetWndResultBoxPtr(i, false);
		if ( NULL == ResBoxPtr ) { continue; }
		ResBoxPtr->SetBoxResultID(ResultID);
	}
	
	WndPtr->SetWndResultID(m_AlgResultID);
	WndPtr->SetWndResultText(m_AlgResultText);
	SaveAlgDefectImage(MaskW, MaskH, MaskStep, MaskBitCount, GrayPtr, MaskPtr, WndRect);
	JetMemory.free_func(GrayPtr);
	JetMemory.free_func(MaskPtr);
	return true;
}
//-------------------------------------------------------------------------------------//