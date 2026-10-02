// AlgParam_EdgeSearch.cpp: implementation of the CAlgParam class.
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
bool GetSearchResultRect(int BlobImgW, int BlobImgH, BOX_TOWARD AlignToward, BOX_TOWARD SearchToward, int Pos, int Range, RECT &Roi)//20260325
{
	RECT &BlobRect_Calc=Roi;
	JetAPI::SizeToRect(BlobImgW, BlobImgH, BlobRect_Calc);
	switch ( AlignToward )
	{
	case BOX_TOWARD_UP:
		if ( AlignToward == SearchToward )
		{
			BlobRect_Calc.bottom= Pos;	
			BlobRect_Calc.top = Pos-Range+1;
			if ( BlobRect_Calc.top  < 0 ) { BlobRect_Calc.top = 0; }
		}
		else
		{
			BlobRect_Calc.top= Pos;	
			BlobRect_Calc.bottom = Pos+Range-1;
			if ( BlobRect_Calc.bottom  > BlobImgH ) { BlobRect_Calc.bottom = BlobImgH; }
		}					
		break;
	case BOX_TOWARD_LEFT:	
		if ( AlignToward == SearchToward )
		{
			BlobRect_Calc.right = Pos;	
			BlobRect_Calc.left = Pos-Range+1;
			if ( BlobRect_Calc.left < 0 ) { BlobRect_Calc.left = 0; }
		}
		else
		{
			BlobRect_Calc.left = Pos;	
			BlobRect_Calc.right = Pos+Range-1;
			if ( BlobRect_Calc.right > BlobImgW ) { BlobRect_Calc.right = BlobImgW; }
		}
		break;					
	case BOX_TOWARD_DOWN:	
		if ( AlignToward == SearchToward )
		{
			BlobRect_Calc.top = Pos;		
			BlobRect_Calc.bottom = Pos+Range-1;
			if ( BlobRect_Calc.bottom > BlobImgH ) { BlobRect_Calc.bottom = BlobImgH; }
		}
		else
		{
			BlobRect_Calc.bottom= Pos;	
			BlobRect_Calc.top = Pos-Range+1;
			if ( BlobRect_Calc.top  < 0 ) { BlobRect_Calc.top = 0; }
		}
		break;
	case BOX_TOWARD_RIGHT:	
		if ( AlignToward == SearchToward )
		{
			BlobRect_Calc.left = Pos;	
			BlobRect_Calc.right = Pos+Range-1;
			if ( BlobRect_Calc.right > BlobImgW ) { BlobRect_Calc.right = BlobImgW; }
		}
		else
		{
			BlobRect_Calc.right = Pos;	
			BlobRect_Calc.left = Pos-Range+1;
			if ( BlobRect_Calc.left < 0 ) { BlobRect_Calc.left = 0; }
		}
		break;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::WriteAlgParamFile_EdgeSearch(const TALG_PARAM_EDGE_SEARCH &esParam, CAOIFileIO &FileIO)//儲存邊緣搜尋參數
{		
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_EDGE_SEARCH_START, 0) == false ) { return false; }	

	if ( FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_EDGE_SEARCH_CUT_LINE_ENB, esParam.esCutLineEnabled) == false ) { return false; }
	if ( FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_EDGE_SEARCH_INVERT_ALIGN, esParam.esInvertAlign) == false ) { return false; }

	if ( FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_EDGE_SEARCH_ENABLE_F, esParam.esEnabled_F) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_EDGE_SEARCH_MIN_RATIO_U_F, esParam.esMinRatioU_F) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_EDGE_SEARCH_MAX_RATIO_U_F, esParam.esMaxRatioU_F) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_EDGE_SEARCH_MIN_RANGE_V_F, esParam.esMinRangeV_F) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_EDGE_SEARCH_MAX_RANGE_V_F, esParam.esMaxRangeV_F) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_EDGE_SEARCH_SIZE_RATIO_V_F, esParam.esSizeRatioV_F) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_EDGE_SEARCH_SIZE_CALC_MODE_F, esParam.esSizeCalcMode_F) == false ) { return false; }	
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_EDGE_SEARCH_FIND_DIRECTION_F, esParam.esSearchDir_F) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_EDGE_SEARCH_FIND_RATIO_F, esParam.esSearchRatioV_F) == false ) { return false; }	
	
	if ( FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_EDGE_SEARCH_ENABLE_B, esParam.esEnabled_B) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_EDGE_SEARCH_MIN_RATIO_U_B, esParam.esMinRatioU_B) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_EDGE_SEARCH_MAX_RATIO_U_B, esParam.esMaxRatioU_B) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_EDGE_SEARCH_MIN_RANGE_V_B, esParam.esMinRangeV_B) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_EDGE_SEARCH_MAX_RANGE_V_B, esParam.esMaxRangeV_B) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_EDGE_SEARCH_SIZE_RATIO_V_B, esParam.esSizeRatioV_B) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_EDGE_SEARCH_SIZE_CALC_MODE_B, esParam.esSizeCalcMode_B) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_EDGE_SEARCH_FIND_DIRECTION_B, esParam.esSearchDir_B) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_EDGE_SEARCH_FIND_RATIO_B, esParam.esSearchRatioV_B) == false ) { return false; }

	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_EDGE_SEARCH_END, 0) == false ) { return false; }	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ReadAlgParamFile_EdgeSearch(TALG_PARAM_EDGE_SEARCH &esParam, CAOIFileIO &FileIO)//載入邊緣搜尋參數
{
	int       index = 0;
	while ( FileIO.CheckFileEnd()==false )
	{ 		
		if ( FileIO.LoadChunk(index) == false )		
		{	continue; }
		switch ( index )
		{
		case FILE_IO_ALG_PARAM_EDGE_SEARCH_END://邊緣搜尋參數-終點
			return true;	

		case FILE_IO_ALG_PARAM_EDGE_SEARCH_CUT_LINE_ENB://啟用-切斷線-中央
			esParam.esCutLineEnabled = FileIO.GetData_BOL();
			break;
		case FILE_IO_ALG_PARAM_EDGE_SEARCH_INVERT_ALIGN://反向對齊
			esParam.esInvertAlign = FileIO.GetData_BOL();
			break;

		case FILE_IO_ALG_PARAM_EDGE_SEARCH_ENABLE_F://啟用-前端
			esParam.esEnabled_F = FileIO.GetData_BOL();
			break;
		case FILE_IO_ALG_PARAM_EDGE_SEARCH_MIN_RATIO_U_F://最小寬度比例-前端
			esParam.esMinRatioU_F = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_EDGE_SEARCH_MAX_RATIO_U_F://最大寬度比例-前端
			esParam.esMaxRatioU_F = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_EDGE_SEARCH_MIN_RANGE_V_F://最小長度範圍-前端
			esParam.esMinRangeV_F = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_EDGE_SEARCH_MAX_RANGE_V_F://最大長度範圍-前端
			esParam.esMaxRangeV_F = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_EDGE_SEARCH_SIZE_RATIO_V_F://尺寸比例長度-前端
			esParam.esSizeRatioV_F = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_EDGE_SEARCH_SIZE_CALC_MODE_F://尺寸比例長度-前端
			esParam.esSizeCalcMode_F = (ALG_OBJECT_SIZE_CALC_MODE)(FileIO.GetData_INT());
			break;
		case FILE_IO_ALG_PARAM_EDGE_SEARCH_FIND_DIRECTION_F://搜尋方向-前端
			esParam.esSearchDir_F = (ALG_SEARCH_DIRECTION)(FileIO.GetData_INT());
			break;
		case FILE_IO_ALG_PARAM_EDGE_SEARCH_FIND_RATIO_F://搜尋比例-前端
			esParam.esSearchRatioV_F = (FileIO.GetData_DBL());
			break;

		case FILE_IO_ALG_PARAM_EDGE_SEARCH_ENABLE_B://啟用-後端
			esParam.esEnabled_B = FileIO.GetData_BOL();
			break;
		case FILE_IO_ALG_PARAM_EDGE_SEARCH_MIN_RATIO_U_B://最小寬度比例-後端
			esParam.esMinRatioU_B = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_EDGE_SEARCH_MAX_RATIO_U_B://最大寬度比例-後端
			esParam.esMaxRatioU_B = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_EDGE_SEARCH_MIN_RANGE_V_B://最小長度範圍-後端
			esParam.esMinRangeV_B = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_EDGE_SEARCH_MAX_RANGE_V_B://最大長度範圍-後端
			esParam.esMaxRangeV_B = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_EDGE_SEARCH_SIZE_RATIO_V_B://尺寸比例長度-後端
			esParam.esSizeRatioV_B = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_EDGE_SEARCH_SIZE_CALC_MODE_B://尺寸比例長度-後端
			esParam.esSizeCalcMode_B = (ALG_OBJECT_SIZE_CALC_MODE)(FileIO.GetData_INT());
			break;
		case FILE_IO_ALG_PARAM_EDGE_SEARCH_FIND_DIRECTION_B://搜尋方向-後端
			esParam.esSearchDir_B = (ALG_SEARCH_DIRECTION)(FileIO.GetData_INT());
			break;
		case FILE_IO_ALG_PARAM_EDGE_SEARCH_FIND_RATIO_B://搜尋比例-後端
			esParam.esSearchRatioV_B = (FileIO.GetData_DBL());
			break;

		default:
		#ifdef _DEBUG
			index = index;
		#endif//_DEBUG
			break;
		}
	}	
	FileIO.SetErrorString(_T("Error, ReadAlgParamFile_EdgeSearch Fault"));
	return false;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ExecAlgInspection_EdgeSearch(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList)
{
	//return ExecAlgInspection_EdgeSearch_V47(ModelPtr, ModelRgn, RoiRect, WndRect, UniFrameList);
	//return ExecAlgInspection_EdgeSearch_V50(ModelPtr, ModelRgn, RoiRect, WndRect, UniFrameList);
	return ExecAlgInspection_EdgeSearch_V61(ModelPtr, ModelRgn, RoiRect, WndRect, UniFrameList);
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ExecAlgInspection_EdgeSearch_V47(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList)
{
	if ( NULL == ModelPtr ) { return false; }
	CAOIComponent *ComponentPtr = ModelPtr->GetModelComponentPtr();
	const size_t FrameCount = UniFrameList.size();	
	const size_t MaskFrameIndex = m_AlgMaskBinParam.GetBinaryFrameIndex();
	const size_t ImageFrameIndex = m_AlgImageBinParam.GetBinaryFrameIndex();
	if ( ImageFrameIndex >= FrameCount ) 
	{	return false; }
	
	CAOIWnd         *WndPtr = GetAlgWndPtr();
	if ( NULL == WndPtr ) { return false; }		
	const unsigned int WndIndex = WndPtr->GetWndIndex();	
	BOX_TOWARD       WndToward = WndPtr->GetWndToward();
	TUNI_FRAME      *UniFramePtr  = &(UniFrameList[ImageFrameIndex]);
	const IMAGE_SIZE FrameImageW    = UniFramePtr->ImageW;
	const IMAGE_SIZE FrameImageH    = UniFramePtr->ImageH;
	const IMAGE_SIZE FrameImageStep = UniFramePtr->ImageStep;
	const IMAGE_SIZE FrameBitCount  = UniFramePtr->BitCount;		
	IMAGE_PTR        FrameImagePtr  = UniFramePtr->ImagePtr;
	MASK_PTR         FrameMaskPtr   = UniFramePtr->MaskPtr;
	SPACE_PTR        FrameSpacePtr  = UniFramePtr->SpacePtr;
	TALG_PARAM_EDGE_SEARCH &esParam= GetAlgParamEdgeSearch();
	const int RoiRectSize = (int)((RoiRect.right-RoiRect.left)*(RoiRect.bottom-RoiRect.top));	
	IMAGE_SIZE    MaskW=0;
	IMAGE_SIZE    MaskH=0;	
	IMAGE_SIZE    MaskStep=0;
	IMAGE_SIZE    MaskBitCount=8;
	MASK_PTR      MaskPtr  = NULL;
	IMAGE_PTR     GrayPtr = NULL;	
	const bool    bTestWnd = false;
	const TPOINT2D   ImageScale = ModelPtr->GetModelImageScale();	
	const int RoiSizeW=RoiRect.right-RoiRect.left;
	const int RoiSizeH=RoiRect.bottom-RoiRect.top;
	const int WndSizeW=WndRect.right-WndRect.left;
	const int WndSizeH=WndRect.bottom-WndRect.top;
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
		if ( JetMemory.alloc_func(ShapeBufferSize, ShapePtr, "CAlgParam::ExecAlgInspection_EdgeSearch", "ShapePtr") == false )
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

#ifdef _DEBUG	
	CString str;
	CString ComponentName;
	CString      DebugFolder=GetAlgDebugFolder();
	bool   bSave = true;
	bool   bSaveBlob=true;
	if ( NULL != ComponentPtr )
	{	ComponentName = ComponentPtr->GetComponentFullName(); }		
	if ( true == bSave )
	{	
		str.Format(_T("%s\\%s_ModelWndAlgEdgeSearch#%d.PNG"), DebugFolder, ComponentName, WndIndex+1);
		ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, 8, MaskPtr, true);
	}
#endif//_DEBUG

	CJetBlob BlobDetector;	
	BlobDetector.InitialBlob();
	BlobDetector.SetBlobSortMode(BLOB_SORT_PIXELS);
	BlobDetector.SetBlobConnectivity(BLOB_CONNECTIVITY_4);	
	if ( BlobDetector.GrayImageRoiBlobDetect(MaskW, MaskH, MaskStep, MaskPtr, RoiRect, 164, 255) == false )	
	{
		JetMemory.free_func(MaskPtr);
		JetMemory.free_func(GrayPtr);			
		return false;
	}		

	size_t       i=0;	
	int          Pos=0;
	int          Range=0;
	bool         bAscMode=true;
	const bool   bUseSubBlob=true;
	bool         bUseLocBlob=false;
	bool         bSearchRect=false;//搜尋區域	
	BOX_TOWARD   SerchToward;	
	double       dSizeW=0.0;
	double       dSizeH=0.0;
	double       MinRatioU = 0.0;
	double       MaxRatioU = 0.0;
	double       MinRangeV = 0.0;
	double       MaxRangeV = 0.0;
	double       SizeRatioV = 0.0;
	double       SearchRatioV = 0.0;
	int          LocPosX = 0;
	int          LocPosY = 0;
	int          SearchSizeW=0;
	int          SearchSizeH=0;
	int          MinW = 0, MinH = 0;
	int          MaxW = 0, MaxH = 0;
	int          BlobW = 0, BlobH = 0;
	int          nPosT=-1, nPosB=-1;
	int          nPosL=-1, nPosR=-1;
	int          nPosTL=-1, nPosTR=-1;
	int          nPosBL=-1, nPosBR=-1;
	int          nPosLT=-1, nPosLB=-1;
	int          nPosRT=-1, nPosRB=-1;
	const int    LocOffsetX=0;//局部起點修正值
	const int    LocOffsetY=0;//局部起點修正值
	const int    Threshold = 164;
	const int    WndRectW = WndRect.right-WndRect.left;
	const int    WndRectH = WndRect.bottom-WndRect.top;
	RECT         Limit={0,0,0,0};	
	RECT         EdgeRect={0,0,0,0};
	RECT         BlobRect={0,0,0,0};
	RECT         SearchRect={0,0,0,0};
	RECT         BlobRect_Loc={0,0,0,0};
	RECT         BlobRect_Sub={0,0,0,0};
	RECT         BlobRect_Calc={0,0,0,0};	
	IMAGE_SIZE   BlobImgW=0;
	IMAGE_SIZE   BlobImgH=0;
	IMAGE_SIZE   BlobImgStep=0;
	IMAGE_SIZE   BlobBitCnt=8;
	IMAGE_PTR    BlobImgPtr=NULL;
	
	TBlobResult *BlobPtr=NULL;		
	std::vector<POINT> PixelList;
	ALG_SEARCH_DIRECTION SarchDirection;
	ALG_OBJECT_SIZE_CALC_MODE SizeCalcMode;
	const size_t BlobCount = BlobDetector.GetBlobCount();	

	//前端, 箭頭端
	if ( true == esParam.esEnabled_F )
	{			
		bUseLocBlob = true;
		//bUseSubBlob = true;
		MaxW = MaxH = INT_MAX;		
		MinRatioU = esParam.esMinRatioU_F;		
		MaxRatioU = esParam.esMaxRatioU_F;
		MinRangeV = esParam.esMinRangeV_F;
		MaxRangeV = esParam.esMaxRangeV_F;		
		SizeRatioV = esParam.esSizeRatioV_F;
		SearchRatioV = esParam.esSearchRatioV_F;
		SarchDirection = esParam.esSearchDir_F;
		SizeCalcMode = esParam.esSizeCalcMode_F;		
		if ( SizeRatioV > 99 ) { bUseLocBlob = false; }
		else if ( SizeRatioV < 1 ) { bUseLocBlob = false; }
		else
		{	bUseLocBlob = true; }		

		bSearchRect = true;
		SearchSizeW=RoiSizeW-(WndSizeW/2);
		SearchSizeH=RoiSizeH-(WndSizeH/2);
		if ( SearchRatioV > 99 ) { bSearchRect = true; }
		else if ( SearchRatioV < 1 ) { bSearchRect = true; }
		else
		{	
			bSearchRect = true;
			SearchSizeW=(SearchRatioV*SearchSizeW/100);
			SearchSizeH=(SearchRatioV*SearchSizeH/100);
			SearchSizeW=MAX(2, SearchSizeW);
			SearchSizeH=MAX(2, SearchSizeH);
		}	
		switch ( WndToward )
		{
		case BOX_TOWARD_UP:
		case BOX_TOWARD_DOWN:
			MinW = (int)(MinRatioU*WndRectW/100.0);
			MaxW = (int)(MaxRatioU*WndRectW/100.0);			 
			MinH = (int)(MinRangeV*ImageScale.y);
			MaxH = (int)(MaxRangeV*ImageScale.y);
			break;
		case BOX_TOWARD_LEFT:
		case BOX_TOWARD_RIGHT:
			MinH = (int)(MinRatioU*WndRectH/100.0);
			MaxH = (int)(MaxRatioU*WndRectH/100.0);
			MinW = (int)(MinRangeV*ImageScale.x);
			MaxW = (int)(MaxRangeV*ImageScale.x);			
			break;
		default:
			break;
		}
		bAscMode = CAlgParam::CheckSortAscMode(WndToward, SarchDirection);		
		SerchToward = CAlgParam::ChangeTowardByDirection(WndToward, SEARCH_DIRECTION_BACKWARD);				
		if ( BOX_TOWARD_NULL != SerchToward )
		{	
			Limit.left   = MAX(1, MinW);
			Limit.top    = MAX(1, MinH);
			Limit.right  = MAX(1, MaxW);
			Limit.bottom = MAX(1, MaxH);
			SearchRect = RoiRect;
			BlobDetector.SortBlobListByToward(SerchToward, bAscMode);
			if ( true == bSearchRect )
			{
				switch ( SerchToward )
				{
				case BOX_TOWARD_UP:		SearchRect.top    = SearchRect.bottom-SearchSizeH;	break;
				case BOX_TOWARD_LEFT:	SearchRect.left   = SearchRect.right-SearchSizeW;	break;
				case BOX_TOWARD_DOWN:	SearchRect.bottom = SearchRect.top+SearchSizeH;	break;				
				case BOX_TOWARD_RIGHT:	SearchRect.right  = SearchRect.left+SearchSizeW;	break;				
				}
			}
			for ( i=0; i<BlobCount; i++ )
			{
				BlobPtr = BlobDetector.GetBlobPtr(i, false);
				if ( NULL == BlobPtr ) { continue; }
				BlobW = BlobPtr->GetBlobRectW();
				BlobH = BlobPtr->GetBlobRectH();
				if ( BlobW < MinW ) { continue; }
				if ( BlobH < MinH ) { continue; }
				if ( true == bSearchRect )
				{
					//BlobRect = BlobPtr->m_BlobRectRaw;
					BlobRect = BlobPtr->m_BlobRect;//20230628-Blob
					switch ( SerchToward )
					{
					case BOX_TOWARD_UP:						
						if ( BlobRect.bottom < SearchRect.top )
						{	continue; }
						break;
					case BOX_TOWARD_LEFT:
						if ( BlobRect.right < SearchRect.left )
						{	continue; }
						break;
					case BOX_TOWARD_DOWN:
						if ( BlobRect.top > SearchRect.bottom )
						{	continue; }
						break;
					case BOX_TOWARD_RIGHT:
						if ( BlobRect.left > SearchRect.right )
						{	continue; }
						break;
					}
				}
				if ( BlobDetector.GetBlobPixelList(BlobPtr, PixelList) == false )
				{	continue; }				
				if ( ImageAPI.CreatePointListImage(PixelList, BlobImgW, BlobImgH, BlobImgStep, BlobBitCnt, BlobImgPtr) == false ) 
				{	continue; }
			#ifdef _DEBUG
				if ( true == bSaveBlob )
				{						
					str.Format(_T("%s\\%s_ModelWndAlgEdgeSearchBlobF#%d#%d.PNG"), DebugFolder, ComponentName, WndIndex+1, i+1);
					ImageAPI.SaveImage(str, BlobImgW, BlobImgH, BlobImgStep, BlobBitCnt, BlobImgPtr, true);
				}
			#endif//_DEBUG
				//BlobRect = BlobPtr->m_BlobRectRaw;
				BlobRect = BlobPtr->m_BlobRect;//20230628-Blob
				JetAPI::SizeToRect(BlobImgW, BlobImgH, EdgeRect);
				if ( true == bSearchRect )				
				{
					LocPosX = BlobRect.left;
					LocPosY = BlobRect.top;
					switch ( SerchToward )
					{
					case BOX_TOWARD_UP:
						if ( (EdgeRect.top+LocPosY) < SearchRect.top )
						{	EdgeRect.top = (SearchRect.top)-LocPosY;	}
						break;
					case BOX_TOWARD_LEFT:
						if ( (EdgeRect.left+LocPosX) < SearchRect.left )
						{	EdgeRect.left = (SearchRect.left)-LocPosX;	}
						break;
					case BOX_TOWARD_DOWN:
						if ( EdgeRect.bottom+LocPosY > (SearchRect.bottom) )
						{	EdgeRect.bottom = (SearchRect.bottom)-LocPosY; }
						break;
					case BOX_TOWARD_RIGHT:
						if ( (EdgeRect.right+LocPosX) > (SearchRect.right) )
						{	EdgeRect.right = (SearchRect.right)-LocPosX;	}
						break;
					}
				}				
				if ( ImageAPI.EdgeSearchImageRoi(BlobImgW, BlobImgH, BlobImgStep, BlobImgPtr, SerchToward, EdgeRect, Limit, Threshold, Pos, Range) == false ) 
				{
					JetMemory.free_func(BlobImgPtr);
					continue; 
				}
				if ( -1 == Pos )
				{
					JetMemory.free_func(BlobImgPtr);
					continue; 
				}
				LocPosX = BlobRect.left+LocOffsetX;
				LocPosY = BlobRect.top+LocOffsetY;
				JetAPI::SizeToRect(BlobImgW, BlobImgH, BlobRect_Calc);
				switch ( SerchToward )
				{
				case BOX_TOWARD_UP:								
					BlobRect_Calc.bottom= Pos;	
					BlobRect_Calc.top = Pos-Range;
					if ( BlobRect_Calc.top  < 0 ) { BlobRect_Calc.top = 0; }
					break;
				case BOX_TOWARD_LEFT:	
					BlobRect_Calc.right = Pos;	
					BlobRect_Calc.left = Pos-Range;
					if ( BlobRect_Calc.left < 0 ) { BlobRect_Calc.left = 0; }
					break;					
				case BOX_TOWARD_DOWN:	
					BlobRect_Calc.top = Pos;		
					BlobRect_Calc.bottom = Pos+Range;
					if ( BlobRect_Calc.bottom > BlobImgH ) { BlobRect_Calc.bottom = BlobImgH; }
					break;
				case BOX_TOWARD_RIGHT:	
					BlobRect_Calc.left = Pos;	
					BlobRect_Calc.right = Pos+Range;
					if ( BlobRect_Calc.right > BlobImgW ) { BlobRect_Calc.right = BlobImgW; }
					break;
				}
				if ( false == bUseSubBlob )
				{	BlobRect_Sub = BlobRect_Calc;	}
				else
				{
					CJetBlob SubBlobDetector;
					SubBlobDetector.InitialBlob();
					SubBlobDetector.SetBlobSortMode(BLOB_SORT_PIXELS);
					SubBlobDetector.SetBlobConnectivity(BLOB_CONNECTIVITY_4);						
					if ( SubBlobDetector.GrayImageRoiBlobDetect(BlobImgW, BlobImgH, BlobImgStep, BlobImgPtr, BlobRect_Calc, Threshold, 255) == false )
					{
						JetMemory.free_func(BlobImgPtr);
						continue; 
					}
					TBlobResult *SubBlobPtr=SubBlobDetector.GetBlobPtr(0, true);
					if ( NULL == SubBlobPtr ) 
					{	
						JetMemory.free_func(BlobImgPtr);
						continue; 
					}				
					//BlobRect_Sub = SubBlobPtr->m_BlobRectRaw;
					BlobRect_Sub = SubBlobPtr->m_BlobRect;//20230628-Blob
				}
				BlobRect.left   = LocPosX + BlobRect_Sub.left;
				BlobRect.top    = LocPosY + BlobRect_Sub.top;
				BlobRect.right  = LocPosX + BlobRect_Sub.right;
				BlobRect.bottom = LocPosY + BlobRect_Sub.bottom;

				BlobW = BlobRect.right-BlobRect.left+1;
				BlobH = BlobRect.bottom-BlobRect.top+1;				
				if ( ALG_OBJECT_SIZE_CALC_AVERAGE == SizeCalcMode )
				{	
					ImageAPI.CalcGrayImageAveSizeByRect(BlobImgW, BlobImgH, BlobImgStep, BlobImgPtr, BlobRect_Sub, Threshold, dSizeW, dSizeH);
					BlobW = (int)(dSizeW+0.5);
					BlobH = (int)(dSizeH+0.5);
				}
				JetMemory.free_func(BlobImgPtr);
				if ( BlobW < MinW ) { continue; }
				if ( BlobW > MaxW ) { continue; }
				if ( BlobH < MinH ) { continue; }
				if ( BlobH > MaxH ) { continue; }

				BlobRect_Calc = BlobRect;
				BlobW = BlobRect.right-BlobRect.left+1;
				BlobH = BlobRect.bottom-BlobRect.top+1;
				switch ( WndToward )
				{
				case BOX_TOWARD_UP:					
					nPosT = BlobRect.top;					
					if ( false == bUseLocBlob )
					{	
						nPosTL = BlobRect.left;
						nPosTR = BlobRect.right;
					}
					else
					{
						BlobRect_Calc.bottom = (int)(BlobRect.top+(BlobH*SizeRatioV/100.0));
						BlobDetector.CalcBlobSubRect(BlobPtr, BlobRect_Calc, BlobRect_Loc);
						nPosTL = BlobRect_Loc.left;
						nPosTR = BlobRect_Loc.right;
					}
					break;
				case BOX_TOWARD_LEFT:
					nPosL = BlobRect.left;
					if ( false == bUseLocBlob )
					{
						nPosLT = BlobRect.top;
						nPosLB = BlobRect.bottom;
					}
					else
					{
						BlobRect_Calc.right = (int)(BlobRect.left+(BlobW*SizeRatioV/100.0));
						BlobDetector.CalcBlobSubRect(BlobPtr, BlobRect_Calc, BlobRect_Loc);
						nPosLT = BlobRect_Loc.top;
						nPosLB = BlobRect_Loc.bottom;
					}
					break;
				case BOX_TOWARD_DOWN:
					nPosB = BlobRect.bottom;
					if ( false == bUseLocBlob )
					{
						nPosBL = BlobRect.left;
						nPosBR = BlobRect.right;
					}
					else					
					{
						BlobRect_Calc.top = (int)(BlobRect.bottom-(BlobH*SizeRatioV/100.0));
						BlobDetector.CalcBlobSubRect(BlobPtr, BlobRect_Calc, BlobRect_Loc);
						nPosBL = BlobRect_Loc.left;
						nPosBR = BlobRect_Loc.right;
					}
					break;
				case BOX_TOWARD_RIGHT:
					nPosR = BlobRect.right;	
					if ( false == bUseLocBlob )
					{
						nPosRT = BlobRect.top;
						nPosRB = BlobRect.bottom;
					}
					else
					{
						BlobRect_Calc.left = (int)(BlobRect.right-(BlobW*SizeRatioV/100.0));
						BlobDetector.CalcBlobSubRect(BlobPtr, BlobRect_Calc, BlobRect_Loc);
						nPosRT = BlobRect_Loc.top;
						nPosRB = BlobRect_Loc.bottom;
					}
					break;
				}
				break;
			}
		}		
	}

	//後端
	if ( true == esParam.esEnabled_B )
	{	
		bUseLocBlob = true;
		//bUseSubBlob = true;
		MaxW = MaxH = INT_MAX;		
		MinRatioU = esParam.esMinRatioU_B;
		MaxRatioU = esParam.esMaxRatioU_B;		
		MinRangeV = esParam.esMinRangeV_B;
		MaxRangeV = esParam.esMaxRangeV_B;	
		SizeRatioV = esParam.esSizeRatioV_B;
		SearchRatioV = esParam.esSearchRatioV_B;
		SarchDirection = esParam.esSearchDir_B;
		SizeCalcMode = esParam.esSizeCalcMode_B;
		if ( SizeRatioV > 99 ) { bUseLocBlob = false; }
		else if ( SizeRatioV < 1 ) { bUseLocBlob = false; }
		else
		{	bUseLocBlob = true; }		

		bSearchRect = true;
		SearchSizeW=RoiSizeW-(WndSizeW/2);
		SearchSizeH=RoiSizeH-(WndSizeH/2);
		if ( SearchRatioV > 99 ) { bSearchRect = true; }
		else if ( SearchRatioV < 1 ) { bSearchRect = true; }
		else
		{	
			bSearchRect = true;			
			SearchSizeW=(SearchRatioV*SearchSizeW/100);
			SearchSizeH=(SearchRatioV*SearchSizeH/100);
			SearchSizeW=MAX(2, SearchSizeW);
			SearchSizeH=MAX(2, SearchSizeH);
		}	
		switch ( WndToward )
		{
		case BOX_TOWARD_UP:
		case BOX_TOWARD_DOWN:
			MinW = (int)(MinRatioU*WndRectW/100.0);
			MaxW = (int)(MaxRatioU*WndRectW/100.0);
			MinH = (int)(MinRangeV*ImageScale.y);
			MaxH = (int)(MaxRangeV*ImageScale.y);			
			break;
		case BOX_TOWARD_LEFT:
		case BOX_TOWARD_RIGHT:
			MinH = (int)(MinRatioU*WndRectH/100.0);
			MaxH = (int)(MaxRatioU*WndRectH/100.0);
			MinW = (int)(MinRangeV*ImageScale.x);
			MaxW = (int)(MaxRangeV*ImageScale.x);
			break;		
		default:			
			break;
		}
		bAscMode = CAlgParam::CheckSortAscMode(WndToward, SarchDirection);		
		SerchToward = CAlgParam::ChangeTowardByDirection(WndToward, SEARCH_DIRECTION_FORWARD);
		if ( BOX_TOWARD_NULL != SerchToward )
		{	
			Limit.left   = MAX(1, MinW);
			Limit.top    = MAX(1, MinH);
			Limit.right  = MAX(1, MaxW);
			Limit.bottom = MAX(1, MaxH);
			SearchRect=RoiRect;
			BlobDetector.SortBlobListByToward(SerchToward, bAscMode); 
			if ( true == bSearchRect )
			{
				switch ( SerchToward )
				{
				case BOX_TOWARD_UP:		SearchRect.top    = SearchRect.bottom-SearchSizeH;	break;
				case BOX_TOWARD_LEFT:	SearchRect.left   = SearchRect.right-SearchSizeW;	break;
				case BOX_TOWARD_DOWN:	SearchRect.bottom = SearchRect.top+SearchSizeH;	break;				
				case BOX_TOWARD_RIGHT:	SearchRect.right  = SearchRect.left+SearchSizeW;	break;				
				}
			}
			for ( i=0; i<BlobCount; i++ )
			{
				BlobPtr = BlobDetector.GetBlobPtr(i, false);
				if ( NULL == BlobPtr ) { continue; }
				BlobW = BlobPtr->GetBlobRectW();
				BlobH = BlobPtr->GetBlobRectH();
				if ( BlobW < MinW ) { continue; }
				if ( BlobH < MinH ) { continue; }
				if ( true == bSearchRect )
				{
					//BlobRect = BlobPtr->m_BlobRectRaw;
					BlobRect = BlobPtr->m_BlobRect;//20230628-Blob
					switch ( SerchToward )
					{
					case BOX_TOWARD_UP:
						if ( BlobRect.bottom < (SearchRect.top) )
						{	continue; }
						break;
					case BOX_TOWARD_LEFT:
						if ( BlobRect.right < (SearchRect.left) )
						{	continue; }
						break;
					case BOX_TOWARD_DOWN:
						if ( BlobRect.top > (SearchRect.bottom) )
						{	continue; }
						break;
					case BOX_TOWARD_RIGHT:
						if ( BlobRect.left > (SearchRect.right) )
						{	continue; }
						break;
					}
				}
				if ( BlobDetector.GetBlobPixelList(BlobPtr, PixelList) == false )
				{	continue; }				
				if ( ImageAPI.CreatePointListImage(PixelList, BlobImgW, BlobImgH, BlobImgStep, BlobBitCnt, BlobImgPtr) == false ) 
				{	continue; }
			#ifdef _DEBUG
				if ( true == bSaveBlob )
				{	
					str.Format(_T("%s\\%s_ModelWndAlgEdgeSearchBlobB#%d#%d.PNG"), DebugFolder, ComponentName, WndIndex+1, i+1);
					ImageAPI.SaveImage(str, BlobImgW, BlobImgH, BlobImgStep, BlobBitCnt, BlobImgPtr, true);
				}
			#endif//_DEBUG
				//BlobRect = BlobPtr->m_BlobRectRaw;
				BlobRect = BlobPtr->m_BlobRect;//20230628-Blob
				JetAPI::SizeToRect(BlobImgW, BlobImgH, EdgeRect);
				if ( true == bSearchRect )				
				{
					LocPosX = BlobRect.left;
					LocPosY = BlobRect.top;
					switch ( SerchToward )
					{
					case BOX_TOWARD_UP:
						if ( (EdgeRect.top+LocPosY) < SearchRect.top )
						{	EdgeRect.top = (SearchRect.top)-LocPosY;	}
						break;
					case BOX_TOWARD_LEFT:
						if ( (EdgeRect.left+LocPosX) < SearchRect.left )
						{	EdgeRect.left = (SearchRect.left)-LocPosX;	}
						break;
					case BOX_TOWARD_DOWN:
						if ( EdgeRect.bottom+LocPosY > (SearchRect.bottom) )
						{	EdgeRect.bottom = (SearchRect.bottom)-LocPosY; }
						break;
					case BOX_TOWARD_RIGHT:
						if ( (EdgeRect.right+LocPosX) > (SearchRect.right) )
						{	EdgeRect.right = (SearchRect.right)-LocPosX;	}
						break;
					}
				}				
				if ( ImageAPI.EdgeSearchImageRoi(BlobImgW, BlobImgH, BlobImgStep, BlobImgPtr, SerchToward, EdgeRect, Limit, Threshold, Pos, Range) == false ) 
				{
					JetMemory.free_func(BlobImgPtr);
					continue; 
				}
				if ( -1 == Pos )
				{ 
					JetMemory.free_func(BlobImgPtr);
					continue; 
				}
				LocPosX = BlobRect.left+LocOffsetX;
				LocPosY = BlobRect.top+LocOffsetY;
				JetAPI::SizeToRect(BlobImgW, BlobImgH, BlobRect_Calc);
				switch ( SerchToward )
				{
				case BOX_TOWARD_UP:								
					BlobRect_Calc.bottom= Pos;	
					BlobRect_Calc.top = Pos-Range;
					if ( BlobRect_Calc.top  < 0 ) { BlobRect_Calc.top = 0; }
					break;
				case BOX_TOWARD_LEFT:	
					BlobRect_Calc.right = Pos;	
					BlobRect_Calc.left = Pos-Range;
					if ( BlobRect_Calc.left < 0 ) { BlobRect_Calc.left = 0; }
					break;					
				case BOX_TOWARD_DOWN:	
					BlobRect_Calc.top = Pos;		
					BlobRect_Calc.bottom = Pos+Range;
					if ( BlobRect_Calc.bottom > BlobImgH ) { BlobRect_Calc.bottom = BlobImgH; }	
					break;
				case BOX_TOWARD_RIGHT:	
					BlobRect_Calc.left = Pos;	
					BlobRect_Calc.right = Pos+Range;
					if ( BlobRect_Calc.right > BlobImgW ) { BlobRect_Calc.right = BlobImgW; }
					break;
				}
				if ( false == bUseSubBlob )
				{	BlobRect_Sub = BlobRect_Calc;	}
				else
				{
					CJetBlob SubBlobDetector;
					SubBlobDetector.InitialBlob();
					SubBlobDetector.SetBlobSortMode(BLOB_SORT_PIXELS);
					SubBlobDetector.SetBlobConnectivity(BLOB_CONNECTIVITY_4);					
					if ( SubBlobDetector.GrayImageRoiBlobDetect(BlobImgW, BlobImgH, BlobImgStep, BlobImgPtr, BlobRect_Calc, Threshold, 255) == false )
					{
						JetMemory.free_func(BlobImgPtr);
						continue; 
					}
					TBlobResult *SubBlobPtr=SubBlobDetector.GetBlobPtr(0, true);
					if ( NULL == SubBlobPtr ) 
					{	
						JetMemory.free_func(BlobImgPtr);
						continue; 
					}				
					//BlobRect_Sub = SubBlobPtr->m_BlobRectRaw;
					BlobRect_Sub = SubBlobPtr->m_BlobRect;//20230628-Blob
				}
				BlobRect.left   = LocPosX + BlobRect_Sub.left;
				BlobRect.top    = LocPosY + BlobRect_Sub.top;
				BlobRect.right  = LocPosX + BlobRect_Sub.right;
				BlobRect.bottom = LocPosY + BlobRect_Sub.bottom;

				BlobW = BlobRect.right-BlobRect.left+1;
				BlobH = BlobRect.bottom-BlobRect.top+1;
				if ( ALG_OBJECT_SIZE_CALC_AVERAGE == SizeCalcMode )
				{	
					ImageAPI.CalcGrayImageAveSizeByRect(BlobImgW, BlobImgH, BlobImgStep, BlobImgPtr, BlobRect_Sub, Threshold, dSizeW, dSizeH);
					BlobW = (int)(dSizeW+0.5);
					BlobH = (int)(dSizeH+0.5);
				}
				JetMemory.free_func(BlobImgPtr);
				if ( BlobW < MinW ) { continue; }
				if ( BlobW > MaxW ) { continue; }
				if ( BlobH < MinH ) { continue; }
				if ( BlobH > MaxH ) { continue; }

				BlobRect_Calc = BlobRect;
				BlobW = BlobPtr->GetBlobRectW();
				BlobH = BlobPtr->GetBlobRectH();
				switch ( WndToward )
				{
				case BOX_TOWARD_UP:
					nPosB = BlobRect.bottom;
					if ( false == bUseLocBlob )
					{
						nPosBL = BlobRect.left;
						nPosBR = BlobRect.right;
					}
					else					
					{
						BlobRect_Calc.top = (int)(BlobRect.bottom-(BlobH*SizeRatioV/100.0));
						BlobDetector.CalcBlobSubRect(BlobPtr, BlobRect_Calc, BlobRect_Loc);
						nPosBL = BlobRect_Loc.left;
						nPosBR = BlobRect_Loc.right;
					}
					break;
				case BOX_TOWARD_LEFT:
					nPosR = BlobRect.right;					
					if ( false == bUseLocBlob )
					{
						nPosRT = BlobRect.top;
						nPosRB = BlobRect.bottom;
					}
					else
					{
						BlobRect_Calc.left = (int)(BlobRect.right-(BlobW*SizeRatioV/100.0));
						BlobDetector.CalcBlobSubRect(BlobPtr, BlobRect_Calc, BlobRect_Loc);
						nPosRT = BlobRect_Loc.top;
						nPosRB = BlobRect_Loc.bottom;
					}
					break;
				case BOX_TOWARD_DOWN:
					nPosT = BlobRect.top;					
					if ( false == bUseLocBlob )
					{	
						nPosTL = BlobRect.left;
						nPosTR = BlobRect.right;
					}
					else
					{
						BlobRect_Calc.bottom = (int)(BlobRect.top+(BlobH*SizeRatioV/100.0));
						BlobDetector.CalcBlobSubRect(BlobPtr, BlobRect_Calc, BlobRect_Loc);
						nPosTL = BlobRect_Loc.left;
						nPosTR = BlobRect_Loc.right;
					}
					break;
				case BOX_TOWARD_RIGHT:
					nPosL = BlobRect.left;					
					if ( false == bUseLocBlob )
					{
						nPosLT = BlobRect.top;
						nPosLB = BlobRect.bottom;
					}
					else
					{
						BlobRect_Calc.right = (int)(BlobRect.left+(BlobW*SizeRatioV/100.0));
						BlobDetector.CalcBlobSubRect(BlobPtr, BlobRect_Calc, BlobRect_Loc);
						nPosLT = BlobRect_Loc.top;
						nPosLB = BlobRect_Loc.bottom;
					}
					break;
				}
				break;
			}
		}		
	}

	double OffsetX=0;
	double OffsetY=0;	
	double CadSizeW = 0;
	double CadSizeH = 0;
	double CadSizeW2 = 0;
	double CadSizeH2 = 0;
	double CadSkew = 0;
	double CadOffsetX = 0;
	double CadOffsetY = 0;
	double CadSkew_Self=0.0;	
	double CadOffsetX_Self=0.0;
	double CadOffsetY_Self=0.0;
	double CadSkew_Others=0.0;
	double CadOffsetX_Others=0.0;
	double CadOffsetY_Others=0.0;	
	const bool AlgSkewEnabled = GetAlgSkewEnabled();
	const double WndCpX = (WndRect.left+WndRect.right)*0.5;
	const double WndCpY = (WndRect.top+WndRect.bottom)*0.5;	
	const double dSkew = GetAlgSkewCalcRange();
	const bool bChkDefect = true;
	const bool ApplySkewAngle = true;	

	TPOINT4D BlobPos;
	std::vector<TPOINT4D> BlobPosList;

	WndPtr->GetWndBox().GetBoxSizeRes(CadSizeW, CadSizeH);
	WndPtr->GetWndBox().GetBoxSizeRes(CadSizeW2, CadSizeH2);	
	//Up-Down	
	if ( -1!=nPosT && -1!=nPosB )
	{
		if ( nPosT < nPosB )
		{
			OffsetY = (nPosT+nPosB)*0.5;
			OffsetY = OffsetY-WndCpY;
			CadSizeH = nPosB-nPosT;
			CadSizeH /= ImageScale.y;

			OffsetX = (nPosTL+nPosTR+nPosBL+nPosBR)*0.25;
			OffsetX = OffsetX-WndCpX;

			CadSizeW = (nPosTR+nPosBR-nPosTL-nPosBL)*0.5;
			CadSizeW /= ImageScale.x;

			if ( true == AlgSkewEnabled )
			{
				BlobPos.x = WndCpX;
				BlobPos.y = WndRect.top;
				BlobPos.u = (nPosTL+nPosTR)*0.5;
				BlobPos.v = nPosT;
				BlobPosList.push_back(BlobPos);

				BlobPos.x = WndCpX;
				BlobPos.y = WndRect.bottom;
				BlobPos.u = (nPosBL+nPosBR)*0.5;
				BlobPos.v = nPosB;
				BlobPosList.push_back(BlobPos);
				CadSkew = -1*CalcRotateAngle(BlobPosList, dSkew);					
			}
		}
	}
	else if ( -1 != nPosT )
	{
		OffsetY = nPosT-WndRect.top;		
		if ( (OffsetY+WndRect.bottom) > RoiRect.bottom )
		{	OffsetY = RoiRect.bottom-WndRect.bottom; }

		OffsetX = (nPosTL+nPosTR)*0.5;
		OffsetX = OffsetX-WndCpX;
		CadSizeW = nPosTR-nPosTL;
		CadSizeW /= ImageScale.x;
	}
	else if ( -1 != nPosB )
	{	
		OffsetY = nPosB-WndRect.bottom;	
		if ( (OffsetY+WndRect.top) < RoiRect.top )
		{	OffsetY = RoiRect.top-WndRect.top; }

		OffsetX = (nPosBL+nPosBR)*0.5;
		OffsetX = OffsetX-WndCpX;
		CadSizeW = nPosBR-nPosBL;
		CadSizeW /= ImageScale.x;
	}
	
	//Left-Right
	if ( -1!=nPosL && -1!=nPosR )
	{
		if ( nPosL < nPosR )
		{
			OffsetX = (nPosL+nPosR)*0.5;
			OffsetX = OffsetX-WndCpX;
			CadSizeW = nPosR-nPosL;
			CadSizeW /= ImageScale.x;

			OffsetY = (nPosLT+nPosLB+nPosRT+nPosRB)*0.25;
			OffsetY = OffsetY-WndCpY;

			CadSizeH = (nPosLB+nPosRB-nPosLT-nPosRT)*0.5;
			CadSizeH /= ImageScale.y;

			if ( true == AlgSkewEnabled )
			{
				BlobPos.x = WndRect.right;
				BlobPos.y = WndCpY;
				BlobPos.u = nPosR;
				BlobPos.v = (nPosRT+nPosRB)*0.5;
				BlobPosList.push_back(BlobPos);

				BlobPos.x = WndRect.left;
				BlobPos.y = WndCpY;
				BlobPos.u = nPosL;
				BlobPos.v = (nPosLT+nPosLB)*0.5;
				BlobPosList.push_back(BlobPos);
				CadSkew = -1*CalcRotateAngle(BlobPosList, dSkew);					
			}
		}
	}
	else if ( -1 != nPosL )
	{
		OffsetX = nPosL-WndRect.left;
		if ( (OffsetX+WndRect.right) > RoiRect.right )
		{	OffsetX = RoiRect.right-WndRect.right; }

		OffsetY = (nPosLT+nPosLB)*0.5;
		OffsetY = OffsetY-WndCpY;
		CadSizeH = nPosLB-nPosLT;
		CadSizeH /= ImageScale.y;
	}
	else if ( -1 != nPosR )
	{	
		OffsetX = nPosR-WndRect.right;	
		if ( (OffsetX+WndRect.left) < RoiRect.left )
		{	OffsetX = RoiRect.left-WndRect.left; }

		OffsetY = (nPosRT+nPosRB)*0.5;
		OffsetY = OffsetY-WndCpY;
		CadSizeH = nPosRB-nPosRT;
		CadSizeH /= ImageScale.y;
	}
	
	double ResOffsetX=OffsetX;
	double ResOffsetY=OffsetY;
	OffsetX /= ImageScale.x;
	OffsetY /= ImageScale.y;
	CadOffsetX =  OffsetX;
	CadOffsetY = -OffsetY;	

	//Judge OK/NG
	if ( true == esParam.esEnabled_F )
	{
		switch ( WndToward )
		{
		case BOX_TOWARD_UP:
			if ( -1 != nPosT ) { esParam.esResultID_F = RESULT_ID_OK; }
			else { esParam.esResultID_F = RESULT_ID_NG; }
			break;
		case BOX_TOWARD_LEFT:
			if ( -1 != nPosL ) { esParam.esResultID_F = RESULT_ID_OK; }
			else { esParam.esResultID_F = RESULT_ID_NG; }
			break;
		case BOX_TOWARD_DOWN:
			if ( -1 != nPosB ) { esParam.esResultID_F = RESULT_ID_OK; }
			else { esParam.esResultID_F = RESULT_ID_NG; }
			break;
		case BOX_TOWARD_RIGHT:
			if ( -1 != nPosR ) { esParam.esResultID_F = RESULT_ID_OK; }
			else { esParam.esResultID_F = RESULT_ID_NG; }
			break;
		}		
	}
	if ( true == esParam.esEnabled_B )
	{
		switch ( WndToward )
		{
		case BOX_TOWARD_UP:
			if ( -1 != nPosB ) { esParam.esResultID_B = RESULT_ID_OK; }
			else { esParam.esResultID_B = RESULT_ID_NG; }
			break;
		case BOX_TOWARD_LEFT:
			if ( -1 != nPosR ) { esParam.esResultID_B = RESULT_ID_OK; }
			else { esParam.esResultID_B = RESULT_ID_NG; }
			break;
		case BOX_TOWARD_DOWN:
			if ( -1 != nPosT ) { esParam.esResultID_B = RESULT_ID_OK; }
			else { esParam.esResultID_B = RESULT_ID_NG; }
			break;
		case BOX_TOWARD_RIGHT:
			if ( -1 != nPosL ) { esParam.esResultID_B = RESULT_ID_OK; }
			else { esParam.esResultID_B = RESULT_ID_NG; }
			break;
		}
	}

	CString strResult;		
	RESULT_ID ResultID = RESULT_ID_NONE;
	if ( RESULT_ID_OK==esParam.esResultID_F || RESULT_ID_OK==esParam.esResultID_B )
	{	ResultID = RESULT_ID_OK; }
	else
	{
		if ( RESULT_ID_NONE==esParam.esResultID_F && RESULT_ID_NONE==esParam.esResultID_B )
		{	ResultID = RESULT_ID_BYPASS; }
		else
		{	ResultID = RESULT_ID_NG; }
	}
	switch ( ResultID )
	{
	case RESULT_ID_OK:	strResult = _T("OK"); break;
	case RESULT_ID_NONE: strResult = _T("Bypass"); break;
	default:
		strResult = _T("NG");
		break;
	}		
	SetAlgResultReading1(0);	
	SetAlgResultID(ResultID);
	SetAlgResultText(strResult);

	SetAlgSkewReading(CadSkew);	
	SetAlgImageOffsetX(ResOffsetX);
	SetAlgImageOffsetY(-ResOffsetY);
	SetAlgOffsetXReading(CadOffsetX);
	SetAlgOffsetYReading(CadOffsetY);
	CalcAlgOffsetL();
	CheckAlgOffset(CadOffsetX_Self, CadOffsetY_Self, CadSkew_Self, CadOffsetX_Others, CadOffsetY_Others, CadSkew_Others, bChkDefect);
	
	if ( GetAlgOffsetXEnabled() == false ) 
	{	CadSizeW = CadSizeW2;	}
	if ( GetAlgOffsetYEnabled() == false ) 
	{	CadSizeH = CadSizeH2;	}	

	WndPtr->SetWndResultID(m_AlgResultID);
	WndPtr->SetWndResultText(m_AlgResultText);	
	SaveAlgDefectImage(MaskW, MaskH, MaskStep, MaskBitCount, GrayPtr, MaskPtr, WndRect);
	JetMemory.free_func(MaskPtr);
	JetMemory.free_func(GrayPtr);

	bool  ApplySkew = false;
	bool  ApplyOffsetX = false;
	bool  ApplyOffsetY = false;
	if ( fabs(CadSkew_Others) > 0.001 ) { ApplySkew = true; }
	if ( fabs(CadOffsetX_Others) > 0.001 ) { ApplyOffsetX = true; }
	if ( fabs(CadOffsetY_Others) > 0.001 ) { ApplyOffsetY = true; }
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
	CAOIBox *BoxPtr = WndPtr->GetWndBoxPtr();
	BoxPtr->SkewBoxAngle(CadSkew_Self);	
	BoxPtr->SetBoxSizeRes(CadSizeW, CadSizeH, true);
	BoxPtr->MoveBoxRes(CadOffsetX_Self, CadOffsetY_Self);		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ExecAlgInspection_EdgeSearch_V50(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList)
{
	if ( NULL == ModelPtr ) { return false; }
	CAOIComponent *ComponentPtr = ModelPtr->GetModelComponentPtr();
	const size_t FrameCount = UniFrameList.size();	
	const size_t MaskFrameIndex = m_AlgMaskBinParam.GetBinaryFrameIndex();
	const size_t ImageFrameIndex = m_AlgImageBinParam.GetBinaryFrameIndex();
	if ( ImageFrameIndex >= FrameCount ) 
	{	return false; }
	
	CAOIWnd         *WndPtr = GetAlgWndPtr();
	if ( NULL == WndPtr ) { return false; }		
	const unsigned int WndIndex = WndPtr->GetWndIndex();	
	BOX_TOWARD       WndToward = WndPtr->GetWndToward();
	TUNI_FRAME      *UniFramePtr  = &(UniFrameList[ImageFrameIndex]);
	const IMAGE_SIZE FrameImageW    = UniFramePtr->ImageW;
	const IMAGE_SIZE FrameImageH    = UniFramePtr->ImageH;
	const IMAGE_SIZE FrameImageStep = UniFramePtr->ImageStep;
	const IMAGE_SIZE FrameBitCount  = UniFramePtr->BitCount;		
	IMAGE_PTR        FrameImagePtr  = UniFramePtr->ImagePtr;
	MASK_PTR         FrameMaskPtr   = UniFramePtr->MaskPtr;
	SPACE_PTR        FrameSpacePtr  = UniFramePtr->SpacePtr;
	TALG_PARAM_EDGE_SEARCH &esParam= GetAlgParamEdgeSearch();
	const int RoiRectSize = (int)((RoiRect.right-RoiRect.left)*(RoiRect.bottom-RoiRect.top));	
	IMAGE_SIZE    MaskW=0;
	IMAGE_SIZE    MaskH=0;	
	IMAGE_SIZE    MaskStep=0;
	IMAGE_SIZE    MaskBitCount=8;
	MASK_PTR      MaskPtr  = NULL;
	IMAGE_PTR     GrayPtr = NULL;	
	const bool    bTestWnd = false;
	const TPOINT2D   ImageScale = ModelPtr->GetModelImageScale();	
	const int RoiSizeW=RoiRect.right-RoiRect.left;
	const int RoiSizeH=RoiRect.bottom-RoiRect.top;
	const int WndSizeW=WndRect.right-WndRect.left;
	const int WndSizeH=WndRect.bottom-WndRect.top;
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
		if ( JetMemory.alloc_func(ShapeBufferSize, ShapePtr, "CAlgParam::ExecAlgInspection_EdgeSearch", "ShapePtr") == false )
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

#ifdef _DEBUG	
	CString str;
	CString ComponentName;
	CString      DebugFolder=GetAlgDebugFolder();
	bool   bSave = true;
	bool   bSaveBlob=true;
	if ( NULL != ComponentPtr )
	{	ComponentName = ComponentPtr->GetComponentFullName(); }		
	if ( true == bSave )
	{	
		str.Format(_T("%s\\%s_ModelWndAlgEdgeSearch#%d.PNG"), DebugFolder, ComponentName, WndIndex+1);
		ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, 8, MaskPtr, true);
	}
#endif//_DEBUG

	//中間切割模式	
	const bool CutCenterLine = esParam.esCutLineEnabled;	
	if ( CutCenterLine )
	{
		const int RoiCpX=(RoiRect.left+RoiRect.right)/2;
		const int RoiCpY=(RoiRect.top+RoiRect.bottom)/2;		
		if ( BOX_TOWARD_UP==WndToward || BOX_TOWARD_DOWN==WndToward )
		{
			for ( int i=RoiCpY; i<RoiCpY+1; i++ )
			{
				for ( int j=RoiRect.left; j<RoiRect.right; j++ )
				{
					int k=(i*MaskStep)+j;
					MaskPtr[k] = 0x00;
				}
			}
		}
		if ( BOX_TOWARD_LEFT==WndToward || BOX_TOWARD_RIGHT==WndToward )
		{
			for ( int i=RoiCpX; i<RoiCpX+1; i++ )
			{
				for ( int j=RoiRect.top; j<RoiRect.bottom; j++ )
				{
					int k=(j*MaskStep)+i;
					MaskPtr[k] = 0x00;
				}
			}
		}
	}
#ifdef _DEBUG
	if ( true == bSave )
	{	
		str.Format(_T("%s\\%s_ModelWndAlgEdgeSearch#%d_CutMid.PNG"), DebugFolder, ComponentName, WndIndex+1);
		ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, 8, MaskPtr, true);
	}
#endif//_DEBUG

	CJetBlob BlobDetector;	
	BlobDetector.InitialBlob();
	BlobDetector.SetBlobSortMode(BLOB_SORT_PIXELS);
	BlobDetector.SetBlobConnectivity(BLOB_CONNECTIVITY_4);	
	if ( BlobDetector.GrayImageRoiBlobDetect(MaskW, MaskH, MaskStep, MaskPtr, RoiRect, 164, 255) == false )	
	{
		JetMemory.free_func(MaskPtr);
		JetMemory.free_func(GrayPtr);			
		return false;
	}		

	size_t       i=0;	
	int          Pos=0;
	int          Range=0;
	bool         bAscMode=true;
	const bool   bUseSubBlob=true;
	bool         bUseLocBlob=false;
	bool         bSearchRect=false;//搜尋區域	
	BOX_TOWARD   AlignToward;
	BOX_TOWARD   SerchToward;	
	double       dSizeW=0.0;
	double       dSizeH=0.0;
	double       MinRatioU = 0.0;
	double       MaxRatioU = 0.0;
	double       MinRangeV = 0.0;
	double       MaxRangeV = 0.0;
	double       SizeRatioV = 0.0;
	double       SearchRatioV = 0.0;
	int          LocPosX = 0;
	int          LocPosY = 0;
	int          SearchSizeW=0;
	int          SearchSizeH=0;
	int          MinW = 0, MinH = 0;
	int          MaxW = 0, MaxH = 0;
	int          BlobW = 0, BlobH = 0;
	int          nPosT=-1, nPosB=-1;
	int          nPosL=-1, nPosR=-1;
	int          nPosTL=-1, nPosTR=-1;
	int          nPosBL=-1, nPosBR=-1;
	int          nPosLT=-1, nPosLB=-1;
	int          nPosRT=-1, nPosRB=-1;
	int          AlignT=-1, AlignL=-1, AlignR=-1, AlignB=-1;
	const int    LocOffsetX=0;//局部起點修正值
	const int    LocOffsetY=0;//局部起點修正值
	const int    Threshold = 164;
	const int    WndRectW = WndRect.right-WndRect.left;
	const int    WndRectH = WndRect.bottom-WndRect.top;
	RECT         Limit={0,0,0,0};	
	RECT         EdgeRect={0,0,0,0};
	RECT         BlobRect={0,0,0,0};
	RECT         SearchRect={0,0,0,0};
	RECT         BlobRect_Loc={0,0,0,0};
	RECT         BlobRect_Sub={0,0,0,0};
	RECT         BlobRect_Calc={0,0,0,0};	
	IMAGE_SIZE   BlobImgW=0;
	IMAGE_SIZE   BlobImgH=0;
	IMAGE_SIZE   BlobImgStep=0;
	IMAGE_SIZE   BlobBitCnt=8;
	IMAGE_PTR    BlobImgPtr=NULL;
	
	TBlobResult *BlobPtr=NULL;		
	std::vector<POINT> PixelList;
	ALG_SEARCH_DIRECTION SarchDirection;
	ALG_OBJECT_SIZE_CALC_MODE SizeCalcMode;
	const bool   bInvertAlign = esParam.esInvertAlign;
	const size_t BlobCount = BlobDetector.GetBlobCount();	

	//前端, 箭頭端
	if ( true == esParam.esEnabled_F )
	{			
		bUseLocBlob = true;
		//bUseSubBlob = true;
		MaxW = MaxH = INT_MAX;		
		MinRatioU = esParam.esMinRatioU_F;		
		MaxRatioU = esParam.esMaxRatioU_F;
		MinRangeV = esParam.esMinRangeV_F;
		MaxRangeV = esParam.esMaxRangeV_F;		
		SizeRatioV = esParam.esSizeRatioV_F;
		SearchRatioV = esParam.esSearchRatioV_F;
		SarchDirection = esParam.esSearchDir_F;
		SizeCalcMode = esParam.esSizeCalcMode_F;		
		if ( SizeRatioV > 99 ) { bUseLocBlob = false; }
		else if ( SizeRatioV < 1 ) { bUseLocBlob = false; }
		else
		{	bUseLocBlob = true; }		

		bSearchRect = true;
		SearchSizeW=RoiSizeW;
		SearchSizeH=RoiSizeH;
		//SearchSizeW=RoiSizeW-(WndSizeW/2);
		//SearchSizeH=RoiSizeH-(WndSizeH/2);
		if ( SearchRatioV > 99 ) { bSearchRect = false; }
		else if ( SearchRatioV < 1 ) { bSearchRect = false; }
		else
		{	
			bSearchRect = true;
			SearchSizeW=(SearchRatioV*SearchSizeW/100);
			SearchSizeH=(SearchRatioV*SearchSizeH/100);
			SearchSizeW=MAX(2, SearchSizeW);
			SearchSizeH=MAX(2, SearchSizeH);
		}	
		switch ( WndToward )
		{
		case BOX_TOWARD_UP:
		case BOX_TOWARD_DOWN:
			MinW = (int)(MinRatioU*WndRectW/100.0);
			MaxW = (int)(MaxRatioU*WndRectW/100.0);			 
			MinH = (int)(MinRangeV*ImageScale.y);
			MaxH = (int)(MaxRangeV*ImageScale.y);
			break;
		case BOX_TOWARD_LEFT:
		case BOX_TOWARD_RIGHT:
			MinH = (int)(MinRatioU*WndRectH/100.0);
			MaxH = (int)(MaxRatioU*WndRectH/100.0);
			MinW = (int)(MinRangeV*ImageScale.x);
			MaxW = (int)(MaxRangeV*ImageScale.x);			
			break;
		default:
			break;
		}
		AlignToward = WndToward;
		bAscMode = CAlgParam::CheckSortAscMode(WndToward, SarchDirection);		
		SerchToward = CAlgParam::ChangeTowardByDirection(WndToward, SEARCH_DIRECTION_BACKWARD);		
		if ( bInvertAlign )
		{	AlignToward = JetAPI::RotateToward(180, WndToward);	}
		if ( BOX_TOWARD_NULL != SerchToward )
		{	
			Limit.left   = MAX(1, MinW);
			Limit.top    = MAX(1, MinH);
			Limit.right  = MAX(1, MaxW);
			Limit.bottom = MAX(1, MaxH);
			SearchRect = RoiRect;
			BlobDetector.SortBlobListByToward(SerchToward, bAscMode);
			if ( true == bSearchRect )
			{
				switch ( SerchToward )
				{
				case BOX_TOWARD_UP:		SearchRect.top    = SearchRect.bottom-SearchSizeH;	break;
				case BOX_TOWARD_LEFT:	SearchRect.left   = SearchRect.right-SearchSizeW;	break;
				case BOX_TOWARD_DOWN:	SearchRect.bottom = SearchRect.top+SearchSizeH;	break;				
				case BOX_TOWARD_RIGHT:	SearchRect.right  = SearchRect.left+SearchSizeW;	break;				
				}
			}
			for ( i=0; i<BlobCount; i++ )
			{
				BlobPtr = BlobDetector.GetBlobPtr(i, false);
				if ( NULL == BlobPtr ) { continue; }
				BlobW = BlobPtr->GetBlobRectW();
				BlobH = BlobPtr->GetBlobRectH();
				if ( BlobW < MinW ) { continue; }
				if ( BlobH < MinH ) { continue; }
				if ( true == bSearchRect )
				{
					//BlobRect = BlobPtr->m_BlobRectRaw;
					BlobRect = BlobPtr->m_BlobRect;//20230628-Blob
					switch ( SerchToward )
					{
					case BOX_TOWARD_UP:						
						if ( BlobRect.bottom < SearchRect.top )
						{	continue; }
						break;
					case BOX_TOWARD_LEFT:
						if ( BlobRect.right < SearchRect.left )
						{	continue; }
						break;
					case BOX_TOWARD_DOWN:
						if ( BlobRect.top > SearchRect.bottom )
						{	continue; }
						break;
					case BOX_TOWARD_RIGHT:
						if ( BlobRect.left > SearchRect.right )
						{	continue; }
						break;
					}
				}
				if ( BlobDetector.GetBlobPixelList(BlobPtr, PixelList) == false )
				{	continue; }				
				if ( ImageAPI.CreatePointListImage(PixelList, BlobImgW, BlobImgH, BlobImgStep, BlobBitCnt, BlobImgPtr) == false ) 
				{	continue; }
			#ifdef _DEBUG
				if ( true == bSaveBlob )
				{						
					str.Format(_T("%s\\%s_ModelWndAlgEdgeSearchBlobF#%d#%d.PNG"), DebugFolder, ComponentName, WndIndex+1, i+1);
					ImageAPI.SaveImage(str, BlobImgW, BlobImgH, BlobImgStep, BlobBitCnt, BlobImgPtr, true);
				}
			#endif//_DEBUG
				//BlobRect = BlobPtr->m_BlobRectRaw;
				BlobRect = BlobPtr->m_BlobRect;//20230628-Blob
				JetAPI::SizeToRect(BlobImgW, BlobImgH, EdgeRect);
				if ( true == bSearchRect )				
				{
					LocPosX = BlobRect.left;
					LocPosY = BlobRect.top;					
					switch ( AlignToward )
					{
					case BOX_TOWARD_UP:
						if ( (EdgeRect.top+LocPosY) < SearchRect.top )
						{	EdgeRect.top = (SearchRect.top)-LocPosY;	}
						break;
					case BOX_TOWARD_LEFT:
						if ( (EdgeRect.left+LocPosX) < SearchRect.left )
						{	EdgeRect.left = (SearchRect.left)-LocPosX;	}
						break;
					case BOX_TOWARD_DOWN:
						if ( EdgeRect.bottom+LocPosY > (SearchRect.bottom) )
						{	EdgeRect.bottom = (SearchRect.bottom)-LocPosY; }
						break;
					case BOX_TOWARD_RIGHT:
						if ( (EdgeRect.right+LocPosX) > (SearchRect.right) )
						{	EdgeRect.right = (SearchRect.right)-LocPosX;	}
						break;
					}
				}				
				if ( ImageAPI.EdgeSearchImageRoi(BlobImgW, BlobImgH, BlobImgStep, BlobImgPtr, AlignToward, EdgeRect, Limit, Threshold, Pos, Range) == false ) 				
				{
					JetMemory.free_func(BlobImgPtr);
					continue; 
				}
				if ( -1 == Pos )
				{
					JetMemory.free_func(BlobImgPtr);
					continue; 
				}
				LocPosX = BlobRect.left+LocOffsetX;
				LocPosY = BlobRect.top+LocOffsetY;
				JetAPI::SizeToRect(BlobImgW, BlobImgH, BlobRect_Calc);
				switch ( AlignToward )
				{
				case BOX_TOWARD_UP:					
					BlobRect_Calc.bottom= Pos;	
					BlobRect_Calc.top = Pos-Range+1;
					if ( BlobRect_Calc.top  < 0 ) { BlobRect_Calc.top = 0; }					
					break;
				case BOX_TOWARD_LEFT:	
					BlobRect_Calc.right = Pos;	
					BlobRect_Calc.left = Pos-Range+1;
					if ( BlobRect_Calc.left < 0 ) { BlobRect_Calc.left = 0; }
					break;					
				case BOX_TOWARD_DOWN:						
					BlobRect_Calc.top = Pos;		
					BlobRect_Calc.bottom = Pos+Range-1;
					if ( BlobRect_Calc.bottom > BlobImgH ) { BlobRect_Calc.bottom = BlobImgH; }					
					break;
				case BOX_TOWARD_RIGHT:	
					BlobRect_Calc.left = Pos;	
					BlobRect_Calc.right = Pos+Range-1;
					if ( BlobRect_Calc.right > BlobImgW ) { BlobRect_Calc.right = BlobImgW; }
					break;
				}
				if ( false == bUseSubBlob )
				{	BlobRect_Sub = BlobRect_Calc;	}
				else
				{
					CJetBlob SubBlobDetector;
					SubBlobDetector.InitialBlob();
					SubBlobDetector.SetBlobSortMode(BLOB_SORT_PIXELS);
					SubBlobDetector.SetBlobConnectivity(BLOB_CONNECTIVITY_4);						
					if ( SubBlobDetector.GrayImageRoiBlobDetect(BlobImgW, BlobImgH, BlobImgStep, BlobImgPtr, BlobRect_Calc, Threshold, 255) == false )
					{
						JetMemory.free_func(BlobImgPtr);
						continue; 
					}
					TBlobResult *SubBlobPtr=SubBlobDetector.GetBlobPtr(0, true);
					if ( NULL == SubBlobPtr ) 
					{	
						JetMemory.free_func(BlobImgPtr);
						continue; 
					}				
					//BlobRect_Sub = SubBlobPtr->m_BlobRectRaw;
					BlobRect_Sub = SubBlobPtr->m_BlobRect;//20230628-Blob
				}
				BlobRect.left   = LocPosX + BlobRect_Sub.left;
				BlobRect.top    = LocPosY + BlobRect_Sub.top;
				BlobRect.right  = LocPosX + BlobRect_Sub.right;
				BlobRect.bottom = LocPosY + BlobRect_Sub.bottom;

				BlobW = BlobRect.right-BlobRect.left+1;
				BlobH = BlobRect.bottom-BlobRect.top+1;				
				if ( ALG_OBJECT_SIZE_CALC_AVERAGE == SizeCalcMode )
				{	
					ImageAPI.CalcGrayImageAveSizeByRect(BlobImgW, BlobImgH, BlobImgStep, BlobImgPtr, BlobRect_Sub, Threshold, dSizeW, dSizeH);
					BlobW = (int)(dSizeW+0.5);
					BlobH = (int)(dSizeH+0.5);
				}
				JetMemory.free_func(BlobImgPtr);
				if ( BlobW < MinW ) { continue; }
				if ( BlobW > MaxW ) { continue; }
				if ( BlobH < MinH ) { continue; }
				if ( BlobH > MaxH ) { continue; }

				BlobRect_Calc = BlobRect;
				BlobW = BlobRect.right-BlobRect.left+1;
				BlobH = BlobRect.bottom-BlobRect.top+1;
				AlignT = AlignL = AlignR = AlignB = -1;
				switch ( AlignToward )
				{
				case BOX_TOWARD_UP:					
					AlignB = AlignT = BlobRect.top;					
					if ( false == bUseLocBlob )
					{	
						AlignL = BlobRect.left;
						AlignR = BlobRect.right;
					}
					else
					{
						BlobRect_Calc.bottom = (int)(BlobRect.top+(BlobH*SizeRatioV/100.0));
						BlobDetector.CalcBlobSubRect(BlobPtr, BlobRect_Calc, BlobRect_Loc);
						AlignL = BlobRect_Loc.left;
						AlignR = BlobRect_Loc.right;
					}
					break;
				case BOX_TOWARD_LEFT:
					AlignR = AlignL = BlobRect.left;
					if ( false == bUseLocBlob )
					{
						AlignT = BlobRect.top;
						AlignB = BlobRect.bottom;
					}
					else
					{
						BlobRect_Calc.right = (int)(BlobRect.left+(BlobW*SizeRatioV/100.0));
						BlobDetector.CalcBlobSubRect(BlobPtr, BlobRect_Calc, BlobRect_Loc);
						AlignT = BlobRect_Loc.top;
						AlignB = BlobRect_Loc.bottom;
					}
					break;
				case BOX_TOWARD_DOWN:
					AlignT = AlignB = BlobRect.bottom;
					if ( false == bUseLocBlob )
					{
						AlignL = BlobRect.left;
						AlignR = BlobRect.right;
					}
					else					
					{
						BlobRect_Calc.top = (int)(BlobRect.bottom-(BlobH*SizeRatioV/100.0));
						BlobDetector.CalcBlobSubRect(BlobPtr, BlobRect_Calc, BlobRect_Loc);
						AlignL = BlobRect_Loc.left;
						AlignR = BlobRect_Loc.right;
					}
					break;
				case BOX_TOWARD_RIGHT:
					AlignL = AlignR = BlobRect.right;	
					if ( false == bUseLocBlob )
					{
						AlignT = BlobRect.top;
						AlignB = BlobRect.bottom;
					}
					else
					{
						BlobRect_Calc.left = (int)(BlobRect.right-(BlobW*SizeRatioV/100.0));
						BlobDetector.CalcBlobSubRect(BlobPtr, BlobRect_Calc, BlobRect_Loc);
						AlignT = BlobRect_Loc.top;
						AlignB = BlobRect_Loc.bottom;
					}
					break;
				}

				switch ( WndToward )
				{
				case BOX_TOWARD_UP:
					nPosT = AlignT;
					nPosTL = AlignL;
					nPosTR = AlignR;					
					break;
				case BOX_TOWARD_LEFT:
					nPosL = AlignL;
					nPosLT = AlignT;
					nPosLB = AlignB;					
					break;
				case BOX_TOWARD_DOWN:					
					nPosB = AlignB;
					nPosBL = AlignL;
					nPosBR = AlignR;	
					break;
				case BOX_TOWARD_RIGHT:					
					nPosR = AlignR;
					nPosRT = AlignT;
					nPosRB = AlignB;
					break;
				}
				break;
			}
		}		
	}

	//後端
	if ( true == esParam.esEnabled_B )
	{	
		bUseLocBlob = true;
		//bUseSubBlob = true;
		MaxW = MaxH = INT_MAX;		
		MinRatioU = esParam.esMinRatioU_B;
		MaxRatioU = esParam.esMaxRatioU_B;		
		MinRangeV = esParam.esMinRangeV_B;
		MaxRangeV = esParam.esMaxRangeV_B;	
		SizeRatioV = esParam.esSizeRatioV_B;
		SearchRatioV = esParam.esSearchRatioV_B;
		SarchDirection = esParam.esSearchDir_B;
		SizeCalcMode = esParam.esSizeCalcMode_B;
		if ( SizeRatioV > 99 ) { bUseLocBlob = false; }
		else if ( SizeRatioV < 1 ) { bUseLocBlob = false; }
		else
		{	bUseLocBlob = true; }		

		bSearchRect = true;
		SearchSizeW=RoiSizeW;
		SearchSizeH=RoiSizeH;
		//SearchSizeW=RoiSizeW-(WndSizeW/2);
		//SearchSizeH=RoiSizeH-(WndSizeH/2);
		if ( SearchRatioV > 99 ) { bSearchRect = false; }
		else if ( SearchRatioV < 1 ) { bSearchRect = false; }
		else
		{	
			bSearchRect = true;			
			SearchSizeW=(SearchRatioV*SearchSizeW/100);
			SearchSizeH=(SearchRatioV*SearchSizeH/100);
			SearchSizeW=MAX(2, SearchSizeW);
			SearchSizeH=MAX(2, SearchSizeH);
		}	
		switch ( WndToward )
		{
		case BOX_TOWARD_UP:
		case BOX_TOWARD_DOWN:
			MinW = (int)(MinRatioU*WndRectW/100.0);
			MaxW = (int)(MaxRatioU*WndRectW/100.0);
			MinH = (int)(MinRangeV*ImageScale.y);
			MaxH = (int)(MaxRangeV*ImageScale.y);			
			break;
		case BOX_TOWARD_LEFT:
		case BOX_TOWARD_RIGHT:
			MinH = (int)(MinRatioU*WndRectH/100.0);
			MaxH = (int)(MaxRatioU*WndRectH/100.0);
			MinW = (int)(MinRangeV*ImageScale.x);
			MaxW = (int)(MaxRangeV*ImageScale.x);
			break;		
		default:			
			break;
		}
		AlignToward = JetAPI::RotateToward(180, WndToward);
		bAscMode = CAlgParam::CheckSortAscMode(WndToward, SarchDirection);		
		SerchToward = CAlgParam::ChangeTowardByDirection(WndToward, SEARCH_DIRECTION_FORWARD);		
		if ( bInvertAlign )
		{	AlignToward = WndToward;	}
		if ( BOX_TOWARD_NULL != SerchToward )
		{	
			Limit.left   = MAX(1, MinW);
			Limit.top    = MAX(1, MinH);
			Limit.right  = MAX(1, MaxW);
			Limit.bottom = MAX(1, MaxH);
			SearchRect=RoiRect;
			BlobDetector.SortBlobListByToward(SerchToward, bAscMode); 
			if ( true == bSearchRect )
			{
				switch ( SerchToward )
				{
				case BOX_TOWARD_UP:		SearchRect.top    = SearchRect.bottom-SearchSizeH;	break;
				case BOX_TOWARD_LEFT:	SearchRect.left   = SearchRect.right-SearchSizeW;	break;
				case BOX_TOWARD_DOWN:	SearchRect.bottom = SearchRect.top+SearchSizeH;	break;				
				case BOX_TOWARD_RIGHT:	SearchRect.right  = SearchRect.left+SearchSizeW;	break;				
				}
			}
			for ( i=0; i<BlobCount; i++ )
			{
				BlobPtr = BlobDetector.GetBlobPtr(i, false);
				if ( NULL == BlobPtr ) { continue; }
				BlobW = BlobPtr->GetBlobRectW();
				BlobH = BlobPtr->GetBlobRectH();
				if ( BlobW < MinW ) { continue; }
				if ( BlobH < MinH ) { continue; }
				if ( true == bSearchRect )
				{
					//BlobRect = BlobPtr->m_BlobRectRaw;
					BlobRect = BlobPtr->m_BlobRect;//20230628-Blob
					switch ( SerchToward )
					{
					case BOX_TOWARD_UP:
						if ( BlobRect.bottom < (SearchRect.top) )
						{	continue; }
						break;
					case BOX_TOWARD_LEFT:
						if ( BlobRect.right < (SearchRect.left) )
						{	continue; }
						break;
					case BOX_TOWARD_DOWN:
						if ( BlobRect.top > (SearchRect.bottom) )
						{	continue; }
						break;
					case BOX_TOWARD_RIGHT:
						if ( BlobRect.left > (SearchRect.right) )
						{	continue; }
						break;
					}
				}
				if ( BlobDetector.GetBlobPixelList(BlobPtr, PixelList) == false )
				{	continue; }				
				if ( ImageAPI.CreatePointListImage(PixelList, BlobImgW, BlobImgH, BlobImgStep, BlobBitCnt, BlobImgPtr) == false ) 
				{	continue; }
			#ifdef _DEBUG
				if ( true == bSaveBlob )
				{	
					str.Format(_T("%s\\%s_ModelWndAlgEdgeSearchBlobB#%d#%d.PNG"), DebugFolder, ComponentName, WndIndex+1, i+1);
					ImageAPI.SaveImage(str, BlobImgW, BlobImgH, BlobImgStep, BlobBitCnt, BlobImgPtr, true);
				}
			#endif//_DEBUG
				//BlobRect = BlobPtr->m_BlobRectRaw;
				BlobRect = BlobPtr->m_BlobRect;//20230628-Blob
				JetAPI::SizeToRect(BlobImgW, BlobImgH, EdgeRect);
				if ( true == bSearchRect )				
				{
					LocPosX = BlobRect.left;
					LocPosY = BlobRect.top;
					switch ( AlignToward )
					{
					case BOX_TOWARD_UP:
						if ( (EdgeRect.top+LocPosY) < SearchRect.top )
						{	EdgeRect.top = (SearchRect.top)-LocPosY;	}
						break;
					case BOX_TOWARD_LEFT:
						if ( (EdgeRect.left+LocPosX) < SearchRect.left )
						{	EdgeRect.left = (SearchRect.left)-LocPosX;	}
						break;
					case BOX_TOWARD_DOWN:
						if ( EdgeRect.bottom+LocPosY > (SearchRect.bottom) )
						{	EdgeRect.bottom = (SearchRect.bottom)-LocPosY; }
						break;
					case BOX_TOWARD_RIGHT:
						if ( (EdgeRect.right+LocPosX) > (SearchRect.right) )
						{	EdgeRect.right = (SearchRect.right)-LocPosX;	}
						break;
					}
				}				
				if ( ImageAPI.EdgeSearchImageRoi(BlobImgW, BlobImgH, BlobImgStep, BlobImgPtr, AlignToward, EdgeRect, Limit, Threshold, Pos, Range) == false ) 				
				{
					JetMemory.free_func(BlobImgPtr);
					continue; 
				}
				if ( -1 == Pos )
				{ 
					JetMemory.free_func(BlobImgPtr);
					continue; 
				}
				LocPosX = BlobRect.left+LocOffsetX;
				LocPosY = BlobRect.top+LocOffsetY;
				JetAPI::SizeToRect(BlobImgW, BlobImgH, BlobRect_Calc);
				switch ( AlignToward )
				{
				case BOX_TOWARD_UP:								
					BlobRect_Calc.bottom= Pos;	
					BlobRect_Calc.top = Pos-Range+1;
					if ( BlobRect_Calc.top  < 0 ) { BlobRect_Calc.top = 0; }
					break;
				case BOX_TOWARD_LEFT:	
					BlobRect_Calc.right = Pos;	
					BlobRect_Calc.left = Pos-Range+1;
					if ( BlobRect_Calc.left < 0 ) { BlobRect_Calc.left = 0; }
					break;					
				case BOX_TOWARD_DOWN:	
					BlobRect_Calc.top = Pos;		
					BlobRect_Calc.bottom = Pos+Range-1;
					if ( BlobRect_Calc.bottom > BlobImgH ) { BlobRect_Calc.bottom = BlobImgH; }	
					break;
				case BOX_TOWARD_RIGHT:	
					BlobRect_Calc.left = Pos;	
					BlobRect_Calc.right = Pos+Range-1;
					if ( BlobRect_Calc.right > BlobImgW ) { BlobRect_Calc.right = BlobImgW; }
					break;
				}
				if ( false == bUseSubBlob )
				{	BlobRect_Sub = BlobRect_Calc;	}
				else
				{
					CJetBlob SubBlobDetector;
					SubBlobDetector.InitialBlob();
					SubBlobDetector.SetBlobSortMode(BLOB_SORT_PIXELS);
					SubBlobDetector.SetBlobConnectivity(BLOB_CONNECTIVITY_4);					
					if ( SubBlobDetector.GrayImageRoiBlobDetect(BlobImgW, BlobImgH, BlobImgStep, BlobImgPtr, BlobRect_Calc, Threshold, 255) == false )
					{
						JetMemory.free_func(BlobImgPtr);
						continue; 
					}
					TBlobResult *SubBlobPtr=SubBlobDetector.GetBlobPtr(0, true);
					if ( NULL == SubBlobPtr ) 
					{	
						JetMemory.free_func(BlobImgPtr);
						continue; 
					}				
					//BlobRect_Sub = SubBlobPtr->m_BlobRectRaw;
					BlobRect_Sub = SubBlobPtr->m_BlobRect;//20230628-Blob
				}
				BlobRect.left   = LocPosX + BlobRect_Sub.left;
				BlobRect.top    = LocPosY + BlobRect_Sub.top;
				BlobRect.right  = LocPosX + BlobRect_Sub.right;
				BlobRect.bottom = LocPosY + BlobRect_Sub.bottom;

				BlobW = BlobRect.right-BlobRect.left+1;
				BlobH = BlobRect.bottom-BlobRect.top+1;
				if ( ALG_OBJECT_SIZE_CALC_AVERAGE == SizeCalcMode )
				{	
					ImageAPI.CalcGrayImageAveSizeByRect(BlobImgW, BlobImgH, BlobImgStep, BlobImgPtr, BlobRect_Sub, Threshold, dSizeW, dSizeH);
					BlobW = (int)(dSizeW+0.5);
					BlobH = (int)(dSizeH+0.5);
				}
				JetMemory.free_func(BlobImgPtr);
				if ( BlobW < MinW ) { continue; }
				if ( BlobW > MaxW ) { continue; }
				if ( BlobH < MinH ) { continue; }
				if ( BlobH > MaxH ) { continue; }

				BlobRect_Calc = BlobRect;
				BlobW = BlobPtr->GetBlobRectW();
				BlobH = BlobPtr->GetBlobRectH();
				AlignT = AlignL = AlignR = AlignB = -1;
				switch ( AlignToward )
				{
				case BOX_TOWARD_UP:
					AlignT = AlignB = BlobRect.top;
					if ( false == bUseLocBlob )
					{
						AlignL = BlobRect.left;
						AlignR = BlobRect.right;
					}
					else					
					{
						BlobRect_Calc.bottom = (int)(BlobRect.top+(BlobH*SizeRatioV/100.0));
						BlobDetector.CalcBlobSubRect(BlobPtr, BlobRect_Calc, BlobRect_Loc);
						AlignL = BlobRect_Loc.left;
						AlignR = BlobRect_Loc.right;
					}
					break;
				case BOX_TOWARD_LEFT:
					AlignL = AlignR = BlobRect.left;					
					if ( false == bUseLocBlob )
					{
						AlignT = BlobRect.top;
						AlignB = BlobRect.bottom;
					}
					else
					{
						BlobRect_Calc.right = (int)(BlobRect.left+(BlobW*SizeRatioV/100.0));
						BlobDetector.CalcBlobSubRect(BlobPtr, BlobRect_Calc, BlobRect_Loc);
						AlignT = BlobRect_Loc.top;
						AlignB = BlobRect_Loc.bottom;
					}
					break;
				case BOX_TOWARD_DOWN:
					AlignB = AlignT = BlobRect.bottom;
					if ( false == bUseLocBlob )
					{	
						AlignL = BlobRect.left;
						AlignR = BlobRect.right;
					}
					else
					{
						BlobRect_Calc.top = (int)(BlobRect.bottom-(BlobH*SizeRatioV/100.0));
						BlobDetector.CalcBlobSubRect(BlobPtr, BlobRect_Calc, BlobRect_Loc);
						AlignL = BlobRect_Loc.left;
						AlignR = BlobRect_Loc.right;
					}
					break;
				case BOX_TOWARD_RIGHT:
					AlignR = AlignL = BlobRect.right;
					if ( false == bUseLocBlob )
					{
						AlignT = BlobRect.top;
						AlignB = BlobRect.bottom;
					}
					else
					{
						BlobRect_Calc.left = (int)(BlobRect.right-(BlobW*SizeRatioV/100.0));
						BlobDetector.CalcBlobSubRect(BlobPtr, BlobRect_Calc, BlobRect_Loc);
						AlignT = BlobRect_Loc.top;
						AlignB = BlobRect_Loc.bottom;
					}
					break;
				}

				switch ( WndToward )
				{
				case BOX_TOWARD_UP:
					nPosB = AlignB;
					nPosBL = AlignL;
					nPosBR = AlignR;					
					break;
				case BOX_TOWARD_LEFT:
					nPosR = AlignR;
					nPosRT = AlignT;
					nPosRB = AlignB;					
					break;
				case BOX_TOWARD_DOWN:					
					nPosT = AlignT;
					nPosTL = AlignL;
					nPosTR = AlignR;	
					break;
				case BOX_TOWARD_RIGHT:					
					nPosL = AlignL;
					nPosLT = AlignT;
					nPosLB = AlignB;
					break;
				}
				break;
			}
		}		
	}

	double OffsetX=0;
	double OffsetY=0;	
	double CadSizeW = 0;
	double CadSizeH = 0;
	double CadSizeW2 = 0;
	double CadSizeH2 = 0;
	double CadSkew = 0;
	double CadOffsetX = 0;
	double CadOffsetY = 0;
	double CadSkew_Self=0.0;	
	double CadOffsetX_Self=0.0;
	double CadOffsetY_Self=0.0;
	double CadSkew_Others=0.0;
	double CadOffsetX_Others=0.0;
	double CadOffsetY_Others=0.0;	
	const bool AlgSkewEnabled = GetAlgSkewEnabled();
	const double WndCpX = (WndRect.left+WndRect.right)*0.5;
	const double WndCpY = (WndRect.top+WndRect.bottom)*0.5;	
	const double dSkew = GetAlgSkewCalcRange();
	const bool bChkDefect = true;
	const bool ApplySkewAngle = true;	

	TPOINT4D BlobPos;
	std::vector<TPOINT4D> BlobPosList;

	WndPtr->GetWndBox().GetBoxSizeRes(CadSizeW, CadSizeH);
	WndPtr->GetWndBox().GetBoxSizeRes(CadSizeW2, CadSizeH2);	
	//Up-Down	
	if ( -1!=nPosT && -1!=nPosB )
	{
		if ( nPosT < nPosB )
		{
			OffsetY = (nPosT+nPosB)*0.5;
			OffsetY = OffsetY-WndCpY;
			CadSizeH = nPosB-nPosT;
			CadSizeH /= ImageScale.y;

			OffsetX = (nPosTL+nPosTR+nPosBL+nPosBR)*0.25;
			OffsetX = OffsetX-WndCpX;

			CadSizeW = (nPosTR+nPosBR-nPosTL-nPosBL)*0.5;
			CadSizeW /= ImageScale.x;

			if ( true == AlgSkewEnabled )
			{
				BlobPos.x = WndCpX;
				BlobPos.y = WndRect.top;
				BlobPos.u = (nPosTL+nPosTR)*0.5;
				BlobPos.v = nPosT;
				BlobPosList.push_back(BlobPos);

				BlobPos.x = WndCpX;
				BlobPos.y = WndRect.bottom;
				BlobPos.u = (nPosBL+nPosBR)*0.5;
				BlobPos.v = nPosB;
				BlobPosList.push_back(BlobPos);
				CadSkew = -1*CalcRotateAngle(BlobPosList, dSkew);					
			}
		}
	}
	else if ( -1 != nPosT )
	{
		OffsetY = nPosT-WndRect.top;		
		if ( (OffsetY+WndRect.bottom) > RoiRect.bottom )
		{	OffsetY = RoiRect.bottom-WndRect.bottom; }

		OffsetX = (nPosTL+nPosTR)*0.5;
		OffsetX = OffsetX-WndCpX;
		CadSizeW = nPosTR-nPosTL;
		CadSizeW /= ImageScale.x;
	}
	else if ( -1 != nPosB )
	{	
		OffsetY = nPosB-WndRect.bottom;	
		if ( (OffsetY+WndRect.top) < RoiRect.top )
		{	OffsetY = RoiRect.top-WndRect.top; }

		OffsetX = (nPosBL+nPosBR)*0.5;
		OffsetX = OffsetX-WndCpX;
		CadSizeW = nPosBR-nPosBL;
		CadSizeW /= ImageScale.x;
	}
	
	//Left-Right
	if ( -1!=nPosL && -1!=nPosR )
	{
		if ( nPosL < nPosR )
		{
			OffsetX = (nPosL+nPosR)*0.5;
			OffsetX = OffsetX-WndCpX;
			CadSizeW = nPosR-nPosL;
			CadSizeW /= ImageScale.x;

			OffsetY = (nPosLT+nPosLB+nPosRT+nPosRB)*0.25;
			OffsetY = OffsetY-WndCpY;

			CadSizeH = (nPosLB+nPosRB-nPosLT-nPosRT)*0.5;
			CadSizeH /= ImageScale.y;

			if ( true == AlgSkewEnabled )
			{
				BlobPos.x = WndRect.right;
				BlobPos.y = WndCpY;
				BlobPos.u = nPosR;
				BlobPos.v = (nPosRT+nPosRB)*0.5;
				BlobPosList.push_back(BlobPos);

				BlobPos.x = WndRect.left;
				BlobPos.y = WndCpY;
				BlobPos.u = nPosL;
				BlobPos.v = (nPosLT+nPosLB)*0.5;
				BlobPosList.push_back(BlobPos);
				CadSkew = -1*CalcRotateAngle(BlobPosList, dSkew);					
			}
		}
	}
	else if ( -1 != nPosL )
	{
		OffsetX = nPosL-WndRect.left;
		if ( (OffsetX+WndRect.right) > RoiRect.right )
		{	OffsetX = RoiRect.right-WndRect.right; }

		OffsetY = (nPosLT+nPosLB)*0.5;
		OffsetY = OffsetY-WndCpY;
		CadSizeH = nPosLB-nPosLT;
		CadSizeH /= ImageScale.y;
	}
	else if ( -1 != nPosR )
	{	
		OffsetX = nPosR-WndRect.right;	
		if ( (OffsetX+WndRect.left) < RoiRect.left )
		{	OffsetX = RoiRect.left-WndRect.left; }

		OffsetY = (nPosRT+nPosRB)*0.5;
		OffsetY = OffsetY-WndCpY;
		CadSizeH = nPosRB-nPosRT;
		CadSizeH /= ImageScale.y;
	}
	
	double ResOffsetX=OffsetX;
	double ResOffsetY=OffsetY;
	OffsetX /= ImageScale.x;
	OffsetY /= ImageScale.y;
	CadOffsetX =  OffsetX;
	CadOffsetY = -OffsetY;	

	//Judge OK/NG
	if ( true == esParam.esEnabled_F )
	{
		switch ( WndToward )
		{
		case BOX_TOWARD_UP:
			if ( -1 != nPosT ) { esParam.esResultID_F = RESULT_ID_OK; }
			else { esParam.esResultID_F = RESULT_ID_NG; }
			break;
		case BOX_TOWARD_LEFT:
			if ( -1 != nPosL ) { esParam.esResultID_F = RESULT_ID_OK; }
			else { esParam.esResultID_F = RESULT_ID_NG; }
			break;
		case BOX_TOWARD_DOWN:
			if ( -1 != nPosB ) { esParam.esResultID_F = RESULT_ID_OK; }
			else { esParam.esResultID_F = RESULT_ID_NG; }
			break;
		case BOX_TOWARD_RIGHT:
			if ( -1 != nPosR ) { esParam.esResultID_F = RESULT_ID_OK; }
			else { esParam.esResultID_F = RESULT_ID_NG; }
			break;
		}		
	}
	if ( true == esParam.esEnabled_B )
	{
		switch ( WndToward )
		{
		case BOX_TOWARD_UP:
			if ( -1 != nPosB ) { esParam.esResultID_B = RESULT_ID_OK; }
			else { esParam.esResultID_B = RESULT_ID_NG; }
			break;
		case BOX_TOWARD_LEFT:
			if ( -1 != nPosR ) { esParam.esResultID_B = RESULT_ID_OK; }
			else { esParam.esResultID_B = RESULT_ID_NG; }
			break;
		case BOX_TOWARD_DOWN:
			if ( -1 != nPosT ) { esParam.esResultID_B = RESULT_ID_OK; }
			else { esParam.esResultID_B = RESULT_ID_NG; }
			break;
		case BOX_TOWARD_RIGHT:
			if ( -1 != nPosL ) { esParam.esResultID_B = RESULT_ID_OK; }
			else { esParam.esResultID_B = RESULT_ID_NG; }
			break;
		}
	}

	CString strResult;		
	RESULT_ID ResultID = RESULT_ID_NONE;
	if ( RESULT_ID_OK==esParam.esResultID_F || RESULT_ID_OK==esParam.esResultID_B )
	{	ResultID = RESULT_ID_OK; }
	else
	{
		if ( RESULT_ID_NONE==esParam.esResultID_F && RESULT_ID_NONE==esParam.esResultID_B )
		{	ResultID = RESULT_ID_BYPASS; }
		else
		{	ResultID = RESULT_ID_NG; }
	}
	switch ( ResultID )
	{
	case RESULT_ID_OK:	strResult = _T("OK"); break;
	case RESULT_ID_NONE: strResult = _T("Bypass"); break;
	default:
		strResult = _T("NG");
		break;
	}		
	SetAlgResultReading1(0);	
	SetAlgResultID(ResultID);
	SetAlgResultText(strResult);

	SetAlgSkewReading(CadSkew);	
	SetAlgImageOffsetX(ResOffsetX);
	SetAlgImageOffsetY(-ResOffsetY);
	SetAlgOffsetXReading(CadOffsetX);
	SetAlgOffsetYReading(CadOffsetY);
	CalcAlgOffsetL();
	CheckAlgOffset(CadOffsetX_Self, CadOffsetY_Self, CadSkew_Self, CadOffsetX_Others, CadOffsetY_Others, CadSkew_Others, bChkDefect);
	
	if ( GetAlgOffsetXEnabled() == false ) 
	{	CadSizeW = CadSizeW2;	}
	if ( GetAlgOffsetYEnabled() == false ) 
	{	CadSizeH = CadSizeH2;	}	

	WndPtr->SetWndResultID(m_AlgResultID);
	WndPtr->SetWndResultText(m_AlgResultText);	
	SaveAlgDefectImage(MaskW, MaskH, MaskStep, MaskBitCount, GrayPtr, MaskPtr, WndRect);
	JetMemory.free_func(MaskPtr);
	JetMemory.free_func(GrayPtr);

	bool  ApplySkew = false;
	bool  ApplyOffsetX = false;
	bool  ApplyOffsetY = false;
	if ( fabs(CadSkew_Others) > 0.001 ) { ApplySkew = true; }
	if ( fabs(CadOffsetX_Others) > 0.001 ) { ApplyOffsetX = true; }
	if ( fabs(CadOffsetY_Others) > 0.001 ) { ApplyOffsetY = true; }
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
	CAOIBox *BoxPtr = WndPtr->GetWndBoxPtr();
	BoxPtr->SkewBoxAngle(CadSkew_Self);	
	BoxPtr->SetBoxSizeRes(CadSizeW, CadSizeH, true);
	BoxPtr->MoveBoxRes(CadOffsetX_Self, CadOffsetY_Self);		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ExecAlgInspection_EdgeSearch_V61(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList)
{
	if ( NULL == ModelPtr ) { return false; }
	CAOIComponent *ComponentPtr = ModelPtr->GetModelComponentPtr();
	const size_t FrameCount = UniFrameList.size();	
	const size_t MaskFrameIndex = m_AlgMaskBinParam.GetBinaryFrameIndex();
	const size_t ImageFrameIndex = m_AlgImageBinParam.GetBinaryFrameIndex();
	if ( ImageFrameIndex >= FrameCount ) 
	{	return false; }

	CAOIWnd         *WndPtr = GetAlgWndPtr();
	if ( NULL == WndPtr ) { return false; }		
	const unsigned int WndIndex = WndPtr->GetWndIndex();	
	BOX_TOWARD       WndToward = WndPtr->GetWndToward();
	TUNI_FRAME      *UniFramePtr  = &(UniFrameList[ImageFrameIndex]);
	const IMAGE_SIZE FrameImageW    = UniFramePtr->ImageW;
	const IMAGE_SIZE FrameImageH    = UniFramePtr->ImageH;
	const IMAGE_SIZE FrameImageStep = UniFramePtr->ImageStep;
	const IMAGE_SIZE FrameBitCount  = UniFramePtr->BitCount;		
	IMAGE_PTR        FrameImagePtr  = UniFramePtr->ImagePtr;
	MASK_PTR         FrameMaskPtr   = UniFramePtr->MaskPtr;
	SPACE_PTR        FrameSpacePtr  = UniFramePtr->SpacePtr;
	TALG_PARAM_EDGE_SEARCH &esParam= GetAlgParamEdgeSearch();
	const int RoiRectSize = (int)((RoiRect.right-RoiRect.left)*(RoiRect.bottom-RoiRect.top));	
	IMAGE_SIZE    MaskW=0;
	IMAGE_SIZE    MaskH=0;	
	IMAGE_SIZE    MaskStep=0;
	IMAGE_SIZE    MaskBitCount=8;
	MASK_PTR      MaskPtr  = NULL;
	IMAGE_PTR     GrayPtr = NULL;	
	const bool    bTestWnd = false;
	const TPOINT2D   ImageScale = ModelPtr->GetModelImageScale();	
	const int RoiSizeW=RoiRect.right-RoiRect.left;
	const int RoiSizeH=RoiRect.bottom-RoiRect.top;
	const int WndSizeW=WndRect.right-WndRect.left;
	const int WndSizeH=WndRect.bottom-WndRect.top;
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
		if ( JetMemory.alloc_func(ShapeBufferSize, ShapePtr, "CAlgParam::ExecAlgInspection_EdgeSearch", "ShapePtr") == false )
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

#ifdef _DEBUG	
	CString str;
	CString ComponentName;
	CString      DebugFolder=GetAlgDebugFolder();
	bool   bSave = true;
	bool   bSaveBlob=true;
	if ( NULL != ComponentPtr )
	{	ComponentName = ComponentPtr->GetComponentFullName(); }		
	if ( true == bSave )
	{	
		str.Format(_T("%s\\%s_ModelWndAlgEdgeSearch#%d.PNG"), DebugFolder, ComponentName, WndIndex+1);
		ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, 8, MaskPtr, true);
	}
#endif//_DEBUG

	//中間切割模式	
	const bool CutCenterLine = esParam.esCutLineEnabled;	
	if ( CutCenterLine )
	{
		const int RoiCpX=(RoiRect.left+RoiRect.right)/2;
		const int RoiCpY=(RoiRect.top+RoiRect.bottom)/2;		
		if ( BOX_TOWARD_UP==WndToward || BOX_TOWARD_DOWN==WndToward )
		{
			for ( int i=RoiCpY; i<RoiCpY+1; i++ )
			{
				for ( int j=RoiRect.left; j<RoiRect.right; j++ )
				{
					int k=(i*MaskStep)+j;
					MaskPtr[k] = 0x00;
				}
			}
		}
		if ( BOX_TOWARD_LEFT==WndToward || BOX_TOWARD_RIGHT==WndToward )
		{
			for ( int i=RoiCpX; i<RoiCpX+1; i++ )
			{
				for ( int j=RoiRect.top; j<RoiRect.bottom; j++ )
				{
					int k=(j*MaskStep)+i;
					MaskPtr[k] = 0x00;
				}
			}
		}
	}
#ifdef _DEBUG
	if ( true == bSave )
	{	
		str.Format(_T("%s\\%s_ModelWndAlgEdgeSearch#%d_CutMid.PNG"), DebugFolder, ComponentName, WndIndex+1);
		ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, 8, MaskPtr, true);
	}
#endif//_DEBUG

	CJetBlob BlobDetector;	
	BlobDetector.InitialBlob();
	BlobDetector.SetBlobSortMode(BLOB_SORT_PIXELS);
	BlobDetector.SetBlobConnectivity(BLOB_CONNECTIVITY_4);	
	if ( BlobDetector.GrayImageRoiBlobDetect(MaskW, MaskH, MaskStep, MaskPtr, RoiRect, 164, 255) == false )	
	{
		JetMemory.free_func(MaskPtr);
		JetMemory.free_func(GrayPtr);			
		return false;
	}		

	size_t       i=0;	
	int          Pos=0;
	int          Range=0;
	bool         bAscMode=true;
	const bool   bUseSubBlob=true;
	bool         bUseLocBlob=false;
	bool         bSearchRect=false;//搜尋區域	
	BOX_TOWARD   AlignToward;
	BOX_TOWARD   SerchToward;	
	double       dSizeW=0.0;
	double       dSizeH=0.0;
	double       MinRatioU = 0.0;
	double       MaxRatioU = 0.0;
	double       MinRangeV = 0.0;
	double       MaxRangeV = 0.0;
	double       SizeRatioV = 0.0;
	double       SearchRatioV = 0.0;
	int          LocPosX = 0;
	int          LocPosY = 0;
	int          SearchSizeW=0;
	int          SearchSizeH=0;
	int          MinW = 0, MinH = 0;
	int          MaxW = 0, MaxH = 0;
	int          BlobW = 0, BlobH = 0;
	int          nPosT=-1, nPosB=-1;
	int          nPosL=-1, nPosR=-1;
	int          nPosTL=-1, nPosTR=-1;
	int          nPosBL=-1, nPosBR=-1;
	int          nPosLT=-1, nPosLB=-1;
	int          nPosRT=-1, nPosRB=-1;
	int          AlignT=-1, AlignL=-1, AlignR=-1, AlignB=-1;
	const int    LocOffsetX=0;//局部起點修正值
	const int    LocOffsetY=0;//局部起點修正值
	const int    Threshold = 164;
	const int    WndRectW = WndRect.right-WndRect.left;
	const int    WndRectH = WndRect.bottom-WndRect.top;
	RECT         Limit={0,0,0,0};	
	RECT         EdgeRect={0,0,0,0};
	RECT         BlobRect={0,0,0,0};
	RECT         SearchRect={0,0,0,0};
	RECT         BlobRect_Loc={0,0,0,0};
	RECT         BlobRect_Sub={0,0,0,0};
	RECT         BlobRect_Calc={0,0,0,0};	
	IMAGE_SIZE   BlobImgW=0;
	IMAGE_SIZE   BlobImgH=0;
	IMAGE_SIZE   BlobImgStep=0;
	IMAGE_SIZE   BlobBitCnt=8;
	IMAGE_PTR    BlobImgPtr=NULL;

	TBlobResult *BlobPtr=NULL;		
	std::vector<POINT> PixelList;
	ALG_SEARCH_DIRECTION SarchDirection;
	ALG_OBJECT_SIZE_CALC_MODE SizeCalcMode;
	const bool   bInvertAlign = esParam.esInvertAlign;
	const size_t BlobCount = BlobDetector.GetBlobCount();	

	//前端, 箭頭端
	if ( true == esParam.esEnabled_F )
	{			
		bUseLocBlob = true;
		//bUseSubBlob = true;
		MaxW = MaxH = INT_MAX;		
		MinRatioU = esParam.esMinRatioU_F;		
		MaxRatioU = esParam.esMaxRatioU_F;
		MinRangeV = esParam.esMinRangeV_F;
		MaxRangeV = esParam.esMaxRangeV_F;		
		SizeRatioV = esParam.esSizeRatioV_F;
		SearchRatioV = esParam.esSearchRatioV_F;
		SarchDirection = esParam.esSearchDir_F;
		SizeCalcMode = esParam.esSizeCalcMode_F;		
		if ( SizeRatioV > 99 ) { bUseLocBlob = false; }
		else if ( SizeRatioV < 1 ) { bUseLocBlob = false; }
		else
		{	bUseLocBlob = true; }		

		bSearchRect = true;
		SearchSizeW=RoiSizeW;
		SearchSizeH=RoiSizeH;
		//SearchSizeW=RoiSizeW-(WndSizeW/2);
		//SearchSizeH=RoiSizeH-(WndSizeH/2);
		if ( SearchRatioV > 99 ) { bSearchRect = false; }
		else if ( SearchRatioV < 1 ) { bSearchRect = false; }
		else
		{	
			bSearchRect = true;
			SearchSizeW=(SearchRatioV*SearchSizeW/100);
			SearchSizeH=(SearchRatioV*SearchSizeH/100);
			SearchSizeW=MAX(2, SearchSizeW);
			SearchSizeH=MAX(2, SearchSizeH);
		}	
		switch ( WndToward )
		{
		case BOX_TOWARD_UP:
		case BOX_TOWARD_DOWN:
			MinW = (int)(MinRatioU*WndRectW/100.0);
			MaxW = (int)(MaxRatioU*WndRectW/100.0);			 
			MinH = (int)(MinRangeV*ImageScale.y);
			MaxH = (int)(MaxRangeV*ImageScale.y);
			break;
		case BOX_TOWARD_LEFT:
		case BOX_TOWARD_RIGHT:
			MinH = (int)(MinRatioU*WndRectH/100.0);
			MaxH = (int)(MaxRatioU*WndRectH/100.0);
			MinW = (int)(MinRangeV*ImageScale.x);
			MaxW = (int)(MaxRangeV*ImageScale.x);			
			break;
		default:
			break;
		}
		AlignToward = WndToward;
		bAscMode = CAlgParam::CheckSortAscMode(WndToward, SarchDirection);		
		SerchToward = CAlgParam::ChangeTowardByDirection(WndToward, SarchDirection);//20260325
		if ( bInvertAlign )
		{	AlignToward = JetAPI::RotateToward(180, WndToward);	}
		if ( BOX_TOWARD_NULL != SerchToward )
		{	
			Limit.left   = MAX(1, MinW);
			Limit.top    = MAX(1, MinH);
			Limit.right  = MAX(1, MaxW);
			Limit.bottom = MAX(1, MaxH);
			SearchRect = RoiRect;
			BlobDetector.SortBlobListByToward(SerchToward, bAscMode);
			if ( true == bSearchRect )
			{
				switch ( WndToward )//20260326
				{
				case BOX_TOWARD_UP:		SearchRect.bottom = SearchRect.top+SearchSizeH;	break;
				case BOX_TOWARD_LEFT:	SearchRect.right  = SearchRect.left+SearchSizeW;	break;
				case BOX_TOWARD_DOWN:	SearchRect.top    = SearchRect.bottom-SearchSizeH;	break;
				case BOX_TOWARD_RIGHT:	SearchRect.left   = SearchRect.right-SearchSizeW;	break;
				}
			}
			for ( i=0; i<BlobCount; i++ )
			{
				BlobPtr = BlobDetector.GetBlobPtr(i, false);
				if ( NULL == BlobPtr ) { continue; }
				BlobW = BlobPtr->GetBlobRectW();
				BlobH = BlobPtr->GetBlobRectH();
				if ( BlobW < MinW ) { continue; }
				if ( BlobH < MinH ) { continue; }
				if ( true == bSearchRect )
				{
					//BlobRect = BlobPtr->m_BlobRectRaw;
					BlobRect = BlobPtr->m_BlobRect;//20230628-Blob
					switch ( WndToward )//20260326
					{
					case BOX_TOWARD_UP:
						if ( BlobRect.top > SearchRect.bottom )
						{	continue; }
						break;
					case BOX_TOWARD_LEFT:
						if ( BlobRect.left > SearchRect.right )
						{	continue; }
						break;
					case BOX_TOWARD_DOWN:
						if ( BlobRect.bottom < SearchRect.top )
						{	continue; }
						break;
					case BOX_TOWARD_RIGHT:
						if ( BlobRect.right < SearchRect.left )
						{	continue; }
						break;
					}
				}
				if ( BlobDetector.GetBlobPixelList(BlobPtr, PixelList) == false )
				{	continue; }				
				if ( ImageAPI.CreatePointListImage(PixelList, BlobImgW, BlobImgH, BlobImgStep, BlobBitCnt, BlobImgPtr) == false ) 
				{	continue; }
			#ifdef _DEBUG
				if ( true == bSaveBlob )
				{						
					str.Format(_T("%s\\%s_ModelWndAlgEdgeSearchBlobF#%d#%d.PNG"), DebugFolder, ComponentName, WndIndex+1, i+1);
					ImageAPI.SaveImage(str, BlobImgW, BlobImgH, BlobImgStep, BlobBitCnt, BlobImgPtr, true);
				}
			#endif//_DEBUG
				//BlobRect = BlobPtr->m_BlobRectRaw;
				BlobRect = BlobPtr->m_BlobRect;//20230628-Blob
				JetAPI::SizeToRect(BlobImgW, BlobImgH, EdgeRect);
				if ( true == bSearchRect )				
				{
					LocPosX = BlobRect.left;
					LocPosY = BlobRect.top;					
					switch ( AlignToward )
					{
					case BOX_TOWARD_UP:
						if ( (EdgeRect.top+LocPosY) < SearchRect.top )
						{	EdgeRect.top = (SearchRect.top)-LocPosY;	}
						break;
					case BOX_TOWARD_LEFT:
						if ( (EdgeRect.left+LocPosX) < SearchRect.left )
						{	EdgeRect.left = (SearchRect.left)-LocPosX;	}
						break;
					case BOX_TOWARD_DOWN:
						if ( EdgeRect.bottom+LocPosY > (SearchRect.bottom) )
						{	EdgeRect.bottom = (SearchRect.bottom)-LocPosY; }
						break;
					case BOX_TOWARD_RIGHT:
						if ( (EdgeRect.right+LocPosX) > (SearchRect.right) )
						{	EdgeRect.right = (SearchRect.right)-LocPosX;	}
						break;
					}
				}
				if ( ImageAPI.EdgeSearchImageRoi(BlobImgW, BlobImgH, BlobImgStep, BlobImgPtr, SerchToward, EdgeRect, Limit, Threshold, Pos, Range) == false ) //20260325
				{
					JetMemory.free_func(BlobImgPtr);
					continue; 
				}
				if ( -1 == Pos )
				{
					JetMemory.free_func(BlobImgPtr);
					continue; 
				}
				LocPosX = BlobRect.left+LocOffsetX;
				LocPosY = BlobRect.top+LocOffsetY;
				//JetAPI::SizeToRect(BlobImgW, BlobImgH, BlobRect_Calc);
				GetSearchResultRect(BlobImgW, BlobImgH, AlignToward, SerchToward, Pos, Range, BlobRect_Calc);//20260325
				if ( false == bUseSubBlob )
				{	BlobRect_Sub = BlobRect_Calc;	}
				else
				{
					CJetBlob SubBlobDetector;
					SubBlobDetector.InitialBlob();
					SubBlobDetector.SetBlobSortMode(BLOB_SORT_PIXELS);
					SubBlobDetector.SetBlobConnectivity(BLOB_CONNECTIVITY_4);						
					if ( SubBlobDetector.GrayImageRoiBlobDetect(BlobImgW, BlobImgH, BlobImgStep, BlobImgPtr, BlobRect_Calc, Threshold, 255) == false )
					{
						JetMemory.free_func(BlobImgPtr);
						continue; 
					}
					TBlobResult *SubBlobPtr=SubBlobDetector.GetBlobPtr(0, true);
					if ( NULL == SubBlobPtr ) 
					{	
						JetMemory.free_func(BlobImgPtr);
						continue; 
					}				
					//BlobRect_Sub = SubBlobPtr->m_BlobRectRaw;
					BlobRect_Sub = SubBlobPtr->m_BlobRect;//20230628-Blob
				}
				BlobRect.left   = LocPosX + BlobRect_Sub.left;
				BlobRect.top    = LocPosY + BlobRect_Sub.top;
				BlobRect.right  = LocPosX + BlobRect_Sub.right;
				BlobRect.bottom = LocPosY + BlobRect_Sub.bottom;

				BlobW = BlobRect.right-BlobRect.left+1;
				BlobH = BlobRect.bottom-BlobRect.top+1;				
				if ( ALG_OBJECT_SIZE_CALC_AVERAGE == SizeCalcMode )
				{	
					ImageAPI.CalcGrayImageAveSizeByRect(BlobImgW, BlobImgH, BlobImgStep, BlobImgPtr, BlobRect_Sub, Threshold, dSizeW, dSizeH);
					BlobW = (int)(dSizeW+0.5);
					BlobH = (int)(dSizeH+0.5);
				}
				JetMemory.free_func(BlobImgPtr);
				if ( BlobW < MinW ) { continue; }
				if ( BlobW > MaxW ) { continue; }
				if ( BlobH < MinH ) { continue; }
				if ( BlobH > MaxH ) { continue; }

				BlobRect_Calc = BlobRect;
				BlobW = BlobRect.right-BlobRect.left+1;
				BlobH = BlobRect.bottom-BlobRect.top+1;
				AlignT = AlignL = AlignR = AlignB = -1;
				switch ( AlignToward )
				{
				case BOX_TOWARD_UP:					
					AlignB = AlignT = BlobRect.top;					
					if ( false == bUseLocBlob )
					{	
						AlignL = BlobRect.left;
						AlignR = BlobRect.right;
					}
					else
					{
						BlobRect_Calc.bottom = (int)(BlobRect.top+(BlobH*SizeRatioV/100.0));
						BlobDetector.CalcBlobSubRect(BlobPtr, BlobRect_Calc, BlobRect_Loc);
						AlignL = BlobRect_Loc.left;
						AlignR = BlobRect_Loc.right;
					}
					break;
				case BOX_TOWARD_LEFT:
					AlignR = AlignL = BlobRect.left;
					if ( false == bUseLocBlob )
					{
						AlignT = BlobRect.top;
						AlignB = BlobRect.bottom;
					}
					else
					{
						BlobRect_Calc.right = (int)(BlobRect.left+(BlobW*SizeRatioV/100.0));
						BlobDetector.CalcBlobSubRect(BlobPtr, BlobRect_Calc, BlobRect_Loc);
						AlignT = BlobRect_Loc.top;
						AlignB = BlobRect_Loc.bottom;
					}
					break;
				case BOX_TOWARD_DOWN:
					AlignT = AlignB = BlobRect.bottom;
					if ( false == bUseLocBlob )
					{
						AlignL = BlobRect.left;
						AlignR = BlobRect.right;
					}
					else					
					{
						BlobRect_Calc.top = (int)(BlobRect.bottom-(BlobH*SizeRatioV/100.0));
						BlobDetector.CalcBlobSubRect(BlobPtr, BlobRect_Calc, BlobRect_Loc);
						AlignL = BlobRect_Loc.left;
						AlignR = BlobRect_Loc.right;
					}
					break;
				case BOX_TOWARD_RIGHT:
					AlignL = AlignR = BlobRect.right;	
					if ( false == bUseLocBlob )
					{
						AlignT = BlobRect.top;
						AlignB = BlobRect.bottom;
					}
					else
					{
						BlobRect_Calc.left = (int)(BlobRect.right-(BlobW*SizeRatioV/100.0));
						BlobDetector.CalcBlobSubRect(BlobPtr, BlobRect_Calc, BlobRect_Loc);
						AlignT = BlobRect_Loc.top;
						AlignB = BlobRect_Loc.bottom;
					}
					break;
				}

				switch ( WndToward )
				{
				case BOX_TOWARD_UP:
					nPosT = AlignT;
					nPosTL = AlignL;
					nPosTR = AlignR;					
					break;
				case BOX_TOWARD_LEFT:
					nPosL = AlignL;
					nPosLT = AlignT;
					nPosLB = AlignB;					
					break;
				case BOX_TOWARD_DOWN:					
					nPosB = AlignB;
					nPosBL = AlignL;
					nPosBR = AlignR;	
					break;
				case BOX_TOWARD_RIGHT:					
					nPosR = AlignR;
					nPosRT = AlignT;
					nPosRB = AlignB;
					break;
				}
				break;
			}
		}		
	}

	//後端
	if ( true == esParam.esEnabled_B )
	{	
		bUseLocBlob = true;
		//bUseSubBlob = true;
		MaxW = MaxH = INT_MAX;		
		MinRatioU = esParam.esMinRatioU_B;
		MaxRatioU = esParam.esMaxRatioU_B;		
		MinRangeV = esParam.esMinRangeV_B;
		MaxRangeV = esParam.esMaxRangeV_B;	
		SizeRatioV = esParam.esSizeRatioV_B;
		SearchRatioV = esParam.esSearchRatioV_B;
		SarchDirection = esParam.esSearchDir_B;
		SizeCalcMode = esParam.esSizeCalcMode_B;
		if ( SizeRatioV > 99 ) { bUseLocBlob = false; }
		else if ( SizeRatioV < 1 ) { bUseLocBlob = false; }
		else
		{	bUseLocBlob = true; }		

		bSearchRect = true;
		SearchSizeW=RoiSizeW;
		SearchSizeH=RoiSizeH;
		//SearchSizeW=RoiSizeW-(WndSizeW/2);
		//SearchSizeH=RoiSizeH-(WndSizeH/2);
		if ( SearchRatioV > 99 ) { bSearchRect = false; }
		else if ( SearchRatioV < 1 ) { bSearchRect = false; }
		else
		{	
			bSearchRect = true;			
			SearchSizeW=(SearchRatioV*SearchSizeW/100);
			SearchSizeH=(SearchRatioV*SearchSizeH/100);
			SearchSizeW=MAX(2, SearchSizeW);
			SearchSizeH=MAX(2, SearchSizeH);
		}	
		switch ( WndToward )
		{
		case BOX_TOWARD_UP:
		case BOX_TOWARD_DOWN:
			MinW = (int)(MinRatioU*WndRectW/100.0);
			MaxW = (int)(MaxRatioU*WndRectW/100.0);
			MinH = (int)(MinRangeV*ImageScale.y);
			MaxH = (int)(MaxRangeV*ImageScale.y);			
			break;
		case BOX_TOWARD_LEFT:
		case BOX_TOWARD_RIGHT:
			MinH = (int)(MinRatioU*WndRectH/100.0);
			MaxH = (int)(MaxRatioU*WndRectH/100.0);
			MinW = (int)(MinRangeV*ImageScale.x);
			MaxW = (int)(MaxRangeV*ImageScale.x);
			break;		
		default:			
			break;
		}
		AlignToward = JetAPI::RotateToward(180, WndToward);
		bAscMode = CAlgParam::CheckSortAscMode(WndToward, SarchDirection);				
		SerchToward = CAlgParam::ChangeTowardByDirection(WndToward, SarchDirection);//20260325
		if ( bInvertAlign )
		{	AlignToward = WndToward;	}
		if ( BOX_TOWARD_NULL != SerchToward )
		{	
			Limit.left   = MAX(1, MinW);
			Limit.top    = MAX(1, MinH);
			Limit.right  = MAX(1, MaxW);
			Limit.bottom = MAX(1, MaxH);
			SearchRect=RoiRect;
			BlobDetector.SortBlobListByToward(SerchToward, bAscMode); 
			if ( true == bSearchRect )
			{
				switch ( WndToward )//20260326
				{
				case BOX_TOWARD_UP:		SearchRect.top    = SearchRect.bottom-SearchSizeH;	break;
				case BOX_TOWARD_LEFT:	SearchRect.left   = SearchRect.right-SearchSizeW;	break;
				case BOX_TOWARD_DOWN:	SearchRect.bottom = SearchRect.top+SearchSizeH;	break;				
				case BOX_TOWARD_RIGHT:	SearchRect.right  = SearchRect.left+SearchSizeW;	break;
				}
			}
			for ( i=0; i<BlobCount; i++ )
			{
				BlobPtr = BlobDetector.GetBlobPtr(i, false);
				if ( NULL == BlobPtr ) { continue; }
				BlobW = BlobPtr->GetBlobRectW();
				BlobH = BlobPtr->GetBlobRectH();
				if ( BlobW < MinW ) { continue; }
				if ( BlobH < MinH ) { continue; }
				if ( true == bSearchRect )
				{
					//BlobRect = BlobPtr->m_BlobRectRaw;
					BlobRect = BlobPtr->m_BlobRect;//20230628-Blob
					switch ( WndToward )//20260326
					{
					case BOX_TOWARD_UP:
						if ( BlobRect.bottom < (SearchRect.top) )
						{	continue; }
						break;
					case BOX_TOWARD_LEFT:
						if ( BlobRect.right < (SearchRect.left) )
						{	continue; }
						break;
					case BOX_TOWARD_DOWN:
						if ( BlobRect.top > (SearchRect.bottom) )
						{	continue; }
						break;
					case BOX_TOWARD_RIGHT:
						if ( BlobRect.left > (SearchRect.right) )
						{	continue; }
						break;
					}
				}
				if ( BlobDetector.GetBlobPixelList(BlobPtr, PixelList) == false )
				{	continue; }				
				if ( ImageAPI.CreatePointListImage(PixelList, BlobImgW, BlobImgH, BlobImgStep, BlobBitCnt, BlobImgPtr) == false ) 
				{	continue; }
			#ifdef _DEBUG
				if ( true == bSaveBlob )
				{	
					str.Format(_T("%s\\%s_ModelWndAlgEdgeSearchBlobB#%d#%d.PNG"), DebugFolder, ComponentName, WndIndex+1, i+1);
					ImageAPI.SaveImage(str, BlobImgW, BlobImgH, BlobImgStep, BlobBitCnt, BlobImgPtr, true);
				}
			#endif//_DEBUG
				//BlobRect = BlobPtr->m_BlobRectRaw;
				BlobRect = BlobPtr->m_BlobRect;//20230628-Blob
				JetAPI::SizeToRect(BlobImgW, BlobImgH, EdgeRect);
				if ( true == bSearchRect )				
				{
					LocPosX = BlobRect.left;
					LocPosY = BlobRect.top;
					switch ( AlignToward )
					{
					case BOX_TOWARD_UP:
						if ( (EdgeRect.top+LocPosY) < SearchRect.top )
						{	EdgeRect.top = (SearchRect.top)-LocPosY;	}
						break;
					case BOX_TOWARD_LEFT:
						if ( (EdgeRect.left+LocPosX) < SearchRect.left )
						{	EdgeRect.left = (SearchRect.left)-LocPosX;	}
						break;
					case BOX_TOWARD_DOWN:
						if ( EdgeRect.bottom+LocPosY > (SearchRect.bottom) )
						{	EdgeRect.bottom = (SearchRect.bottom)-LocPosY; }
						break;
					case BOX_TOWARD_RIGHT:
						if ( (EdgeRect.right+LocPosX) > (SearchRect.right) )
						{	EdgeRect.right = (SearchRect.right)-LocPosX;	}
						break;
					}
				}								
				if ( ImageAPI.EdgeSearchImageRoi(BlobImgW, BlobImgH, BlobImgStep, BlobImgPtr, SerchToward, EdgeRect, Limit, Threshold, Pos, Range) == false ) //20260325				
				{
					JetMemory.free_func(BlobImgPtr);
					continue; 
				}
				if ( -1 == Pos )
				{ 
					JetMemory.free_func(BlobImgPtr);
					continue; 
				}
				LocPosX = BlobRect.left+LocOffsetX;
				LocPosY = BlobRect.top+LocOffsetY;
				//JetAPI::SizeToRect(BlobImgW, BlobImgH, BlobRect_Calc);
				GetSearchResultRect(BlobImgW, BlobImgH, AlignToward, SerchToward, Pos, Range, BlobRect_Calc);//20260325
				if ( false == bUseSubBlob )
				{	BlobRect_Sub = BlobRect_Calc;	}
				else
				{
					CJetBlob SubBlobDetector;
					SubBlobDetector.InitialBlob();
					SubBlobDetector.SetBlobSortMode(BLOB_SORT_PIXELS);
					SubBlobDetector.SetBlobConnectivity(BLOB_CONNECTIVITY_4);					
					if ( SubBlobDetector.GrayImageRoiBlobDetect(BlobImgW, BlobImgH, BlobImgStep, BlobImgPtr, BlobRect_Calc, Threshold, 255) == false )
					{
						JetMemory.free_func(BlobImgPtr);
						continue; 
					}
					TBlobResult *SubBlobPtr=SubBlobDetector.GetBlobPtr(0, true);
					if ( NULL == SubBlobPtr ) 
					{	
						JetMemory.free_func(BlobImgPtr);
						continue; 
					}				
					//BlobRect_Sub = SubBlobPtr->m_BlobRectRaw;
					BlobRect_Sub = SubBlobPtr->m_BlobRect;//20230628-Blob
				}
				BlobRect.left   = LocPosX + BlobRect_Sub.left;
				BlobRect.top    = LocPosY + BlobRect_Sub.top;
				BlobRect.right  = LocPosX + BlobRect_Sub.right;
				BlobRect.bottom = LocPosY + BlobRect_Sub.bottom;

				BlobW = BlobRect.right-BlobRect.left+1;
				BlobH = BlobRect.bottom-BlobRect.top+1;
				if ( ALG_OBJECT_SIZE_CALC_AVERAGE == SizeCalcMode )
				{	
					ImageAPI.CalcGrayImageAveSizeByRect(BlobImgW, BlobImgH, BlobImgStep, BlobImgPtr, BlobRect_Sub, Threshold, dSizeW, dSizeH);
					BlobW = (int)(dSizeW+0.5);
					BlobH = (int)(dSizeH+0.5);
				}
				JetMemory.free_func(BlobImgPtr);
				if ( BlobW < MinW ) { continue; }
				if ( BlobW > MaxW ) { continue; }
				if ( BlobH < MinH ) { continue; }
				if ( BlobH > MaxH ) { continue; }

				BlobRect_Calc = BlobRect;
				BlobW = BlobPtr->GetBlobRectW();
				BlobH = BlobPtr->GetBlobRectH();
				AlignT = AlignL = AlignR = AlignB = -1;
				switch ( AlignToward )
				{
				case BOX_TOWARD_UP:
					AlignT = AlignB = BlobRect.top;
					if ( false == bUseLocBlob )
					{
						AlignL = BlobRect.left;
						AlignR = BlobRect.right;
					}
					else					
					{
						BlobRect_Calc.bottom = (int)(BlobRect.top+(BlobH*SizeRatioV/100.0));
						BlobDetector.CalcBlobSubRect(BlobPtr, BlobRect_Calc, BlobRect_Loc);
						AlignL = BlobRect_Loc.left;
						AlignR = BlobRect_Loc.right;
					}
					break;
				case BOX_TOWARD_LEFT:
					AlignL = AlignR = BlobRect.left;					
					if ( false == bUseLocBlob )
					{
						AlignT = BlobRect.top;
						AlignB = BlobRect.bottom;
					}
					else
					{
						BlobRect_Calc.right = (int)(BlobRect.left+(BlobW*SizeRatioV/100.0));
						BlobDetector.CalcBlobSubRect(BlobPtr, BlobRect_Calc, BlobRect_Loc);
						AlignT = BlobRect_Loc.top;
						AlignB = BlobRect_Loc.bottom;
					}
					break;
				case BOX_TOWARD_DOWN:
					AlignB = AlignT = BlobRect.bottom;
					if ( false == bUseLocBlob )
					{	
						AlignL = BlobRect.left;
						AlignR = BlobRect.right;
					}
					else
					{
						BlobRect_Calc.top = (int)(BlobRect.bottom-(BlobH*SizeRatioV/100.0));
						BlobDetector.CalcBlobSubRect(BlobPtr, BlobRect_Calc, BlobRect_Loc);
						AlignL = BlobRect_Loc.left;
						AlignR = BlobRect_Loc.right;
					}
					break;
				case BOX_TOWARD_RIGHT:
					AlignR = AlignL = BlobRect.right;
					if ( false == bUseLocBlob )
					{
						AlignT = BlobRect.top;
						AlignB = BlobRect.bottom;
					}
					else
					{
						BlobRect_Calc.left = (int)(BlobRect.right-(BlobW*SizeRatioV/100.0));
						BlobDetector.CalcBlobSubRect(BlobPtr, BlobRect_Calc, BlobRect_Loc);
						AlignT = BlobRect_Loc.top;
						AlignB = BlobRect_Loc.bottom;
					}
					break;
				}

				switch ( WndToward )
				{
				case BOX_TOWARD_UP:
					nPosB = AlignB;
					nPosBL = AlignL;
					nPosBR = AlignR;					
					break;
				case BOX_TOWARD_LEFT:
					nPosR = AlignR;
					nPosRT = AlignT;
					nPosRB = AlignB;					
					break;
				case BOX_TOWARD_DOWN:					
					nPosT = AlignT;
					nPosTL = AlignL;
					nPosTR = AlignR;	
					break;
				case BOX_TOWARD_RIGHT:					
					nPosL = AlignL;
					nPosLT = AlignT;
					nPosLB = AlignB;
					break;
				}
				break;
			}
		}		
	}

	double OffsetX=0;
	double OffsetY=0;	
	double CadSizeW = 0;
	double CadSizeH = 0;
	double CadSizeW2 = 0;
	double CadSizeH2 = 0;
	double CadSkew = 0;
	double CadOffsetX = 0;
	double CadOffsetY = 0;
	double CadSkew_Self=0.0;	
	double CadOffsetX_Self=0.0;
	double CadOffsetY_Self=0.0;
	double CadSkew_Others=0.0;
	double CadOffsetX_Others=0.0;
	double CadOffsetY_Others=0.0;	
	const bool AlgSkewEnabled = GetAlgSkewEnabled();
	const double WndCpX = (WndRect.left+WndRect.right)*0.5;
	const double WndCpY = (WndRect.top+WndRect.bottom)*0.5;	
	const double dSkew = GetAlgSkewCalcRange();
	const bool bChkDefect = true;
	const bool ApplySkewAngle = true;	

	TPOINT4D BlobPos;
	std::vector<TPOINT4D> BlobPosList;

	WndPtr->GetWndBox().GetBoxSizeRes(CadSizeW, CadSizeH);
	WndPtr->GetWndBox().GetBoxSizeRes(CadSizeW2, CadSizeH2);	
	//Up-Down	
	if ( -1!=nPosT && -1!=nPosB )
	{
		if ( nPosT < nPosB )
		{
			OffsetY = (nPosT+nPosB)*0.5;
			OffsetY = OffsetY-WndCpY;
			CadSizeH = nPosB-nPosT;
			CadSizeH /= ImageScale.y;

			OffsetX = (nPosTL+nPosTR+nPosBL+nPosBR)*0.25;
			OffsetX = OffsetX-WndCpX;

			CadSizeW = (nPosTR+nPosBR-nPosTL-nPosBL)*0.5;
			CadSizeW /= ImageScale.x;

			if ( true == AlgSkewEnabled )
			{
				BlobPos.x = WndCpX;
				BlobPos.y = WndRect.top;
				BlobPos.u = (nPosTL+nPosTR)*0.5;
				BlobPos.v = nPosT;
				BlobPosList.push_back(BlobPos);

				BlobPos.x = WndCpX;
				BlobPos.y = WndRect.bottom;
				BlobPos.u = (nPosBL+nPosBR)*0.5;
				BlobPos.v = nPosB;
				BlobPosList.push_back(BlobPos);
				CadSkew = -1*CalcRotateAngle(BlobPosList, dSkew);					
			}
		}
		else
		{	nPosT = nPosB = -1;	}
	}
	else if ( -1 != nPosT )
	{
		OffsetY = nPosT-WndRect.top;		
		if ( (OffsetY+WndRect.bottom) > RoiRect.bottom )
		{	OffsetY = RoiRect.bottom-WndRect.bottom; }

		OffsetX = (nPosTL+nPosTR)*0.5;
		OffsetX = OffsetX-WndCpX;
		CadSizeW = nPosTR-nPosTL;
		CadSizeW /= ImageScale.x;
	}
	else if ( -1 != nPosB )
	{	
		OffsetY = nPosB-WndRect.bottom;	
		if ( (OffsetY+WndRect.top) < RoiRect.top )
		{	OffsetY = RoiRect.top-WndRect.top; }

		OffsetX = (nPosBL+nPosBR)*0.5;
		OffsetX = OffsetX-WndCpX;
		CadSizeW = nPosBR-nPosBL;
		CadSizeW /= ImageScale.x;
	}

	//Left-Right
	if ( -1!=nPosL && -1!=nPosR )
	{
		if ( nPosL < nPosR )
		{
			OffsetX = (nPosL+nPosR)*0.5;
			OffsetX = OffsetX-WndCpX;
			CadSizeW = nPosR-nPosL;
			CadSizeW /= ImageScale.x;

			OffsetY = (nPosLT+nPosLB+nPosRT+nPosRB)*0.25;
			OffsetY = OffsetY-WndCpY;

			CadSizeH = (nPosLB+nPosRB-nPosLT-nPosRT)*0.5;
			CadSizeH /= ImageScale.y;

			if ( true == AlgSkewEnabled )
			{
				BlobPos.x = WndRect.right;
				BlobPos.y = WndCpY;
				BlobPos.u = nPosR;
				BlobPos.v = (nPosRT+nPosRB)*0.5;
				BlobPosList.push_back(BlobPos);

				BlobPos.x = WndRect.left;
				BlobPos.y = WndCpY;
				BlobPos.u = nPosL;
				BlobPos.v = (nPosLT+nPosLB)*0.5;
				BlobPosList.push_back(BlobPos);
				CadSkew = -1*CalcRotateAngle(BlobPosList, dSkew);					
			}
		}
		else
		{	nPosL = nPosR = -1;	}
	}
	else if ( -1 != nPosL )
	{
		OffsetX = nPosL-WndRect.left;
		if ( (OffsetX+WndRect.right) > RoiRect.right )
		{	OffsetX = RoiRect.right-WndRect.right; }

		OffsetY = (nPosLT+nPosLB)*0.5;
		OffsetY = OffsetY-WndCpY;
		CadSizeH = nPosLB-nPosLT;
		CadSizeH /= ImageScale.y;
	}
	else if ( -1 != nPosR )
	{	
		OffsetX = nPosR-WndRect.right;	
		if ( (OffsetX+WndRect.left) < RoiRect.left )
		{	OffsetX = RoiRect.left-WndRect.left; }

		OffsetY = (nPosRT+nPosRB)*0.5;
		OffsetY = OffsetY-WndCpY;
		CadSizeH = nPosRB-nPosRT;
		CadSizeH /= ImageScale.y;
	}

	double ResOffsetX=OffsetX;
	double ResOffsetY=OffsetY;
	OffsetX /= ImageScale.x;
	OffsetY /= ImageScale.y;
	CadOffsetX =  OffsetX;
	CadOffsetY = -OffsetY;	

	//Judge OK/NG
	if ( true == esParam.esEnabled_F )
	{
		switch ( WndToward )
		{
		case BOX_TOWARD_UP:
			if ( -1 != nPosT ) { esParam.esResultID_F = RESULT_ID_OK; }
			else { esParam.esResultID_F = RESULT_ID_NG; }
			break;
		case BOX_TOWARD_LEFT:
			if ( -1 != nPosL ) { esParam.esResultID_F = RESULT_ID_OK; }
			else { esParam.esResultID_F = RESULT_ID_NG; }
			break;
		case BOX_TOWARD_DOWN:
			if ( -1 != nPosB ) { esParam.esResultID_F = RESULT_ID_OK; }
			else { esParam.esResultID_F = RESULT_ID_NG; }
			break;
		case BOX_TOWARD_RIGHT:
			if ( -1 != nPosR ) { esParam.esResultID_F = RESULT_ID_OK; }
			else { esParam.esResultID_F = RESULT_ID_NG; }
			break;
		}		
	}
	if ( true == esParam.esEnabled_B )
	{
		switch ( WndToward )
		{
		case BOX_TOWARD_UP:
			if ( -1 != nPosB ) { esParam.esResultID_B = RESULT_ID_OK; }
			else { esParam.esResultID_B = RESULT_ID_NG; }
			break;
		case BOX_TOWARD_LEFT:
			if ( -1 != nPosR ) { esParam.esResultID_B = RESULT_ID_OK; }
			else { esParam.esResultID_B = RESULT_ID_NG; }
			break;
		case BOX_TOWARD_DOWN:
			if ( -1 != nPosT ) { esParam.esResultID_B = RESULT_ID_OK; }
			else { esParam.esResultID_B = RESULT_ID_NG; }
			break;
		case BOX_TOWARD_RIGHT:
			if ( -1 != nPosL ) { esParam.esResultID_B = RESULT_ID_OK; }
			else { esParam.esResultID_B = RESULT_ID_NG; }
			break;
		}
	}

	CString strResult;		
	RESULT_ID ResultID = RESULT_ID_NONE;
	if ( RESULT_ID_OK==esParam.esResultID_F || RESULT_ID_OK==esParam.esResultID_B )
	{	ResultID = RESULT_ID_OK; }
	else
	{
		if ( RESULT_ID_NONE==esParam.esResultID_F && RESULT_ID_NONE==esParam.esResultID_B )
		{	ResultID = RESULT_ID_BYPASS; }
		else
		{	ResultID = RESULT_ID_NG; }
	}
	switch ( ResultID )
	{
	case RESULT_ID_OK:	strResult = _T("OK"); break;
	case RESULT_ID_NONE: strResult = _T("Bypass"); break;
	default:
		strResult = _T("NG");
		break;
	}		
	SetAlgResultReading1(0);	
	SetAlgResultID(ResultID);
	SetAlgResultText(strResult);

	SetAlgSkewReading(CadSkew);	
	SetAlgImageOffsetX(ResOffsetX);
	SetAlgImageOffsetY(-ResOffsetY);
	SetAlgOffsetXReading(CadOffsetX);
	SetAlgOffsetYReading(CadOffsetY);
	CalcAlgOffsetL();
	CheckAlgOffset(CadOffsetX_Self, CadOffsetY_Self, CadSkew_Self, CadOffsetX_Others, CadOffsetY_Others, CadSkew_Others, bChkDefect);

	if ( GetAlgOffsetXEnabled() == false ) 
	{	CadSizeW = CadSizeW2;	}
	if ( GetAlgOffsetYEnabled() == false ) 
	{	CadSizeH = CadSizeH2;	}	

	WndPtr->SetWndResultID(m_AlgResultID);
	WndPtr->SetWndResultText(m_AlgResultText);	
	SaveAlgDefectImage(MaskW, MaskH, MaskStep, MaskBitCount, GrayPtr, MaskPtr, WndRect);
	JetMemory.free_func(MaskPtr);
	JetMemory.free_func(GrayPtr);

	bool  ApplySkew = false;
	bool  ApplyOffsetX = false;
	bool  ApplyOffsetY = false;
	if ( fabs(CadSkew_Others) > 0.001 ) { ApplySkew = true; }
	if ( fabs(CadOffsetX_Others) > 0.001 ) { ApplyOffsetX = true; }
	if ( fabs(CadOffsetY_Others) > 0.001 ) { ApplyOffsetY = true; }
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
	CAOIBox *BoxPtr = WndPtr->GetWndBoxPtr();
	BoxPtr->SkewBoxAngle(CadSkew_Self);	
	BoxPtr->SetBoxSizeRes(CadSizeW, CadSizeH, true);
	BoxPtr->MoveBoxRes(CadOffsetX_Self, CadOffsetY_Self);		
	return true;
}
//-------------------------------------------------------------------------------------//