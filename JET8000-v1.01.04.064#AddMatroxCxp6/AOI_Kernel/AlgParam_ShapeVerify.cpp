// AlgParam_ShapeVerify.cpp: implementation of the CAlgParam class.
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
bool CAlgParam::CheckOK_ShapeVerifyOuterTol(const TALG_PARAM_SHAPE_VERIFY &Param)
{
	double Diff=::fabs(Param.svReadingROuter-Param.svCircleR);	
	if ( Diff > Param.svOuterTol ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_ShapeVerifyInnerTol(const TALG_PARAM_SHAPE_VERIFY &Param)
{
	double Diff=::fabs(Param.svReadingRInner-Param.svCircleR);	
	if ( Diff > Param.svInnerTol ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_ShapeVerifyRangeTol(const TALG_PARAM_SHAPE_VERIFY &Param)
{
	double Diff=::fabs(Param.svReadingROuter-Param.svReadingRInner);	
	if ( Diff > Param.svRangeTol ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_ShapeVerifyErrorTol(const TALG_PARAM_SHAPE_VERIFY &Param)
{
	double Err=::fabs(Param.svReadingRAverage-Param.svCircleR);	
	if ( Err > Param.svErrorTol ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::WriteAlgParamFile_ShapeVerify(const TALG_PARAM_SHAPE_VERIFY &svParam, CAOIFileIO &FileIO)//儲存外形驗證參數
{
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_SHAPE_VERIFY_START, 0) == false ) { return false; }			
	
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_SHAPE_VERIFY_SKIP_RATIO_INN, svParam.svSkipRatioInner) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_SHAPE_VERIFY_SKIP_RATIO_OUT, svParam.svSkipRatioOuter) == false ) { return false; }
	
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_SHAPE_VERIFY_CIRCLE_OUTER_TOL, svParam.svOuterTol) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_SHAPE_VERIFY_CIRCLE_INNER_TOL, svParam.svInnerTol) == false ) { return false; }	
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_SHAPE_VERIFY_CIRCLE_RANGE_TOL, svParam.svRangeTol) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_SHAPE_VERIFY_CIRCLE_ERROR_TOL, svParam.svErrorTol) == false ) { return false; }

	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_SHAPE_VERIFY_END, 0) == false ) { return false; }	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ReadAlgParamFile_ShapeVerify(TALG_PARAM_SHAPE_VERIFY &svParam, CAOIFileIO &FileIO)//載入外形驗證參數
{
	int       index = 0;
	while ( FileIO.CheckFileEnd()==false )
	{ 		
		if ( FileIO.LoadChunk(index) == false )		
		{	continue; }
		switch ( index )
		{		
		case FILE_IO_ALG_PARAM_SHAPE_VERIFY_SKIP_RATIO_INN://演算法參數-外形驗證參數-跳過比例-內部
			svParam.svSkipRatioInner = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_SHAPE_VERIFY_SKIP_RATIO_OUT://演算法參數-外形驗證參數-跳過比例-外部
			svParam.svSkipRatioOuter = FileIO.GetData_DBL();
			break;
		
		case FILE_IO_ALG_PARAM_SHAPE_VERIFY_CIRCLE_OUTER_TOL://外形驗證參數-外圓半徑公差
			svParam.svOuterTol = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_SHAPE_VERIFY_CIRCLE_INNER_TOL://外形驗證參數-內圓半徑公差
			svParam.svInnerTol = FileIO.GetData_DBL();
			break;			
		case FILE_IO_ALG_PARAM_SHAPE_VERIFY_CIRCLE_RANGE_TOL://外形驗證參數-外內誤差公差
			svParam.svRangeTol = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_SHAPE_VERIFY_CIRCLE_ERROR_TOL://外形驗證參數-平均誤差公差
			svParam.svErrorTol = FileIO.GetData_DBL();
			break;

		case FILE_IO_ALG_PARAM_SHAPE_VERIFY_END://外形驗證參數-終點
			return true;
		default:
		#ifdef _DEBUG
			index = index;
		#endif//_DEBUG
			break;		
		}
	}
	FileIO.SetErrorString(_T("Error, ReadAlgParamFile_ShapeVerify Fault"));
	return false;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ExecAlgInspection_ShapeVerify(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList)
{
	if ( NULL == ModelPtr ) { return false; }
	CAOIComponent *ComponentPtr = ModelPtr->GetModelComponentPtr();
	const size_t FrameCount = UniFrameList.size();		
	const size_t ImageFrameIndex = m_AlgImageBinParam.GetBinaryFrameIndex();
	if ( ImageFrameIndex >= FrameCount ) 
	{	return false; }
	
	CAOIWnd         *WndPtr = CAlgParam::GetAlgWndPtr();
	if ( NULL == WndPtr ) { return false; }	

	BOX_TOWARD      WndToward = WndPtr->GetWndToward();
	unsigned int    WndIndex = WndPtr->GetWndIndex();
	BOX_SHAPE_MODE  WndShapeMode=WndPtr->GetWndShapeMode();
	TUNI_FRAME      *UniFramePtr  = &(UniFrameList[ImageFrameIndex]);
	const IMAGE_SIZE FrameImageW    = UniFramePtr->ImageW;
	const IMAGE_SIZE FrameImageH    = UniFramePtr->ImageH;
	const IMAGE_SIZE FrameImageStep = UniFramePtr->ImageStep;
	const IMAGE_SIZE FrameBitCount  = UniFramePtr->BitCount;		
	IMAGE_PTR        FrameImagePtr  = UniFramePtr->ImagePtr;
	MASK_PTR         FrameMaskPtr   = UniFramePtr->MaskPtr;
	SPACE_PTR        FrameSpacePtr  = UniFramePtr->SpacePtr;	
	TALG_PARAM_SHAPE_VERIFY &svParam = GetAlgParamShapeVerify();			
	const int RoiRectSize = (int)((RoiRect.right-RoiRect.left)*(RoiRect.bottom-RoiRect.top));	
	IMAGE_SIZE    MaskW=0;
	IMAGE_SIZE    MaskH=0;	
	IMAGE_SIZE    MaskStep=0;
	IMAGE_SIZE    MaskBitCount=8;
	MASK_PTR      MaskPtr  = NULL;
	IMAGE_PTR     GrayPtr = NULL;	
	const bool    bTestWnd = false;	
	const TPOINT2D   ImageScale = ModelPtr->GetModelImageScale();	
	const double ScaleX=1.0/ImageScale.x;
	const double ScaleY=1.0/ImageScale.y;		
	if ( ExecAlgUniFrameBinary(m_AlgImageBinParam, WndRect, RoiRect, UniFrameList, MaskW, MaskH, MaskStep, MaskBitCount, MaskPtr, GrayPtr, bTestWnd) == false )
	{	
		JetMemory.free_func(MaskPtr);
		JetMemory.free_func(GrayPtr);
		return false; 
	}	
	if ( ExecAlgUniFrameShapeMask(WndPtr, RoiRect, MaskW, MaskH, MaskStep, MaskBitCount, MaskPtr) == false )
	{
		JetMemory.free_func(MaskPtr);
		JetMemory.free_func(GrayPtr);		
		return false; 
	}	
	if ( ExecAlgUniFrameMaskBinary(ModelPtr, m_AlgMaskBinParam, WndRect, RoiRect, UniFrameList, MaskW, MaskH, MaskStep, MaskBitCount, MaskPtr, bTestWnd) == false )
	{
		JetMemory.free_func(MaskPtr);
		JetMemory.free_func(GrayPtr);		
		return false; 
	}	
	
#ifdef _DEBUG
	CString str;
	bool    bSaveImage=false;//true, false
	bool    bOnlineSave=false;	
	CString Folder;
	CString ImageBase;
	CString ComponentName;	
	CString DebugFolder=GetAlgDebugFolder();	
	if ( NULL != ComponentPtr )
	{	ComponentName = ComponentPtr->GetComponentFullName(); }		
	else
	{	bSaveImage = false; }
	if ( true == bSaveImage )
	{	
		if ( true == bOnlineSave )
		{	ImageBase.Format(_T("%s\\%s_ModelWndAlgShapeVerify#%d"), DebugFolder, ComponentName, WndIndex+1);	}		
		else
		{
			Folder.Format(_T("%s\\%s"), DebugFolder, _T("AlgDebug"));
			::CreateDirectory(Folder, NULL);
			JetAPI::ClearFolder(Folder);
			ImageBase.Format(_T("%s\\ModelWndAlgShapeVerify"), Folder);
		}
	}
	if ( true == bSaveImage )
	{
		str.Format(_T("%s_00_Gray.PNG"), ImageBase);
		ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, 8, GrayPtr, true);
		str.Format(_T("%s_01_Mask.PNG"), ImageBase);
		ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, 8, MaskPtr, true);
	}
#endif //_DEBUG
	
	int          ShapeMode = 1;   
	const int    ShapeModeRect = 1;
	const int    ShapeModeCircle = 2;	
	const size_t BufferSize=ImageAPI.CalcBufferSize(MaskStep, MaskH);
	const size_t RectW=RoiRect.right-RoiRect.left;
	const int    BlobTh_H=255;
	const int    BlobTh_L=129;
	if ( BOX_SHAPE_ELLIPSE == WndShapeMode )
	{	ShapeMode = ShapeModeCircle;	}
	else
	{	ShapeMode = ShapeModeRect;	}	

	//先進行空洞填補
	CJetBlob BlobDetectorObj;	
	std::vector<TBlobResult> BlobListObj;	
	const BLOB_SORT_MODE BlobSortMode=BLOB_SORT_RECT_SIZE;//BLOB_SORT_PIXELS;
	const BLOB_CONNECTIVITY_MODE BlobConMode=BLOB_CONNECTIVITY_8;//BLOB_CONNECTIVITY_4, BLOB_CONNECTIVITY_8
	BlobDetectorObj.InitialBlob();	
	BlobDetectorObj.SetBlobSortMode(BlobSortMode);	
	BlobDetectorObj.SetBlobConnectivity(BlobConMode);
	
	const float SkipRatioInner=svParam.svSkipRatioInner;
	const float SkipRatioOuter=svParam.svSkipRatioOuter;
	const float WndRectCpX=(WndRect.right+WndRect.left)/2.0f;
	const float WndRectCpY=(WndRect.top+WndRect.bottom)/2.0f;
	const int WndRectW=WndRect.right-WndRect.left;
	const int WndRectH=WndRect.bottom-WndRect.top;
	const int WndRectW2=WndRectW/2;
	const int WndRectH2=WndRectH/2;
	const int WndRectR=(WndRectW2+WndRectH2)/2;
	const int WndSize=WndRectW*WndRectH;	
	const int FilterRIn=(int)(WndRectR*SkipRatioInner/100);		
	const int FilterROut=(int)(WndRectR*SkipRatioOuter/100);
	const int FilterRMin=MIN(FilterRIn, FilterROut);
	const int FilterRMax=MAX(FilterRIn, FilterROut);

	if ( FilterRMin>0 || FilterRMax>0 )//有使用前幾個區塊回填模式
	{	
		if ( BlobDetectorObj.GrayImageRoiBlobDetect(MaskW, MaskH, MaskStep, MaskPtr, RoiRect, BlobTh_L, BlobTh_H) == false )
		{	
			JetMemory.free_func(MaskPtr);
			JetMemory.free_func(GrayPtr);
			return false;
		}	
		BlobDetectorObj.CloneBlobList(BlobListObj);	
		const size_t BlobCountObj = BlobListObj.size();
		if ( BlobCountObj > 0 )
		{	
			::memset(MaskPtr, 0x00, sizeof(MASK_DATA)*BufferSize);
			for ( size_t j=0; j<BlobCountObj; j++ )
			{
				TBlobResult &ObjRef=BlobListObj[j];
				const RECT &ObjRect=ObjRef.m_BlobRect;
				if ( FilterRMin>0 || FilterRMax>0 )
				{
					if ( ShapeModeCircle == ShapeMode )
					{		
						if ( BlobDetectorObj.CheckBlobInDistRange(&ObjRef, WndRectCpX, WndRectCpY, FilterRMin, FilterRMax) == false )
						{	continue; }
					}
					if ( ShapeModeRect == ShapeMode )
					{
						if ( ObjRect.bottom < WndRect.top )
						{	continue;	}
						if ( ObjRect.top > WndRect.bottom )
						{	continue;	}
						if ( ObjRect.right < WndRect.left )
						{	continue;	}
						if ( ObjRect.left > WndRect.right )
						{	continue;	}
					}					
				}				
				BlobDetectorObj.GetBlobImage(&ObjRef, MaskW, MaskH, MaskStep, 8, MaskPtr, false);
			#ifdef _DEBUG
				if ( true == bSaveImage )
				{
					str.Format(_T("%s_02_MaskObj%02d.PNG"), ImageBase, j+1);			
					ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, 8, MaskPtr, true); 
				}
			#endif//_DEBUG
			}
		}
	}
#ifdef _DEBUG
	if ( true == bSaveImage )
	{
		str.Format(_T("%s_02_MaskObjEnd.PNG"), ImageBase);			
		ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, 8, MaskPtr, true); 
	}
#endif//_DEBUG

	if ( BlobDetectorObj.GrayImageRoiBlobDetect(MaskW, MaskH, MaskStep, MaskPtr, RoiRect, BlobTh_L, BlobTh_H) == false )
	{	
		JetMemory.free_func(MaskPtr);
		JetMemory.free_func(GrayPtr);
		return false;
	}	

	CJetBlob BlobDetector;
	std::vector<TBlobResult> BlobList;
	BlobDetector.InitialBlob();	
	BlobDetector.SetBlobSortMode(BlobSortMode);	
	BlobDetector.SetBlobConnectivity(BLOB_CONNECTIVITY_8);	
	if ( BlobDetector.GrayImageRoiBlobDetect(MaskW, MaskH, MaskStep, MaskPtr, RoiRect, BlobTh_L, BlobTh_H) == false )
	{	
		JetMemory.free_func(MaskPtr);
		JetMemory.free_func(GrayPtr);
		return false;
	}	
	BlobDetector.CloneBlobList(BlobList);

	size_t i=0;	
	TPOINT2D     BoxPos;
	TSIZE2D      BoxSize;	
	RECT         BlobRect={0,0,0,0};
	CAOIBox      ResBox, *BoxPtr=NULL;	
	TBlobResult *BlobPtr=NULL;	
	const size_t BlobCount = BlobList.size();

	TPOINT2D        RgnCp;
	TPOINT2D        ImageCp;
	TREGION4D       BoxRgn;
	TREGION4D       WndRgn;

	ImageCp.x = MaskW;
	ImageCp.y = MaskH;
	ImageCp.x = ImageCp.x*0.5;
	ImageCp.y = ImageCp.y*0.5;
	RgnCp.x = ModelRgn.GetCpX();
	RgnCp.y = ModelRgn.GetCpY();		

	TPOINT2D WndRectCalValue;		
	WndPtr->GetWndRectCalValue(WndRectCalValue);
	WndPtr->GetWndRegion(WndRgn);
	//WndRectCalValue.x = -WndRectCalValue.x;
	//WndRectCalValue.y = -WndRectCalValue.y;	
	
	double RadiusMax=0.0, RadiusMin=0.0;
	double RadiusSum=0.0, RadiusAve=0.0;
	const double SizeW = WndRgn.GetWidth();
	const double SizeH = WndRgn.GetHeight();	
	const double RX = SizeW*0.5;
	const double RY = SizeH*0.5;
	const double RadiusStd=(RX+RY)*0.5;
	svParam.svCircleR = RadiusStd;

	if ( BlobCount > 0 )
	{	
		RECT TotalBlobRect;
		std::vector<POINT> TotalBlobPixelList;
		BoxPtr = WndPtr->GetWndBoxPtr();
		for ( size_t j=0; j<BlobCount; j++ )
		{
			BlobPtr = &BlobList[j];
			//BlobRect = BlobPtr->m_BlobRectRaw;
			BlobRect = BlobPtr->m_BlobRect;//20230628-Blob
			if ( 0 == j )
			{	TotalBlobRect = BlobRect; }
			else
			{
				if ( TotalBlobRect.left > BlobRect.left ) { TotalBlobRect.left = BlobRect.left; }
				if ( TotalBlobRect.top > BlobRect.top ) { TotalBlobRect.top = BlobRect.top; }
				if ( TotalBlobRect.right < BlobRect.right ) { TotalBlobRect.right = BlobRect.right; }
				if ( TotalBlobRect.bottom < BlobRect.bottom ) { TotalBlobRect.bottom = BlobRect.bottom; }
			}
			if ( ShapeModeCircle == ShapeMode )
			{	
				if ( TotalBlobPixelList.empty() == true )
				{	BlobDetector.GetBlobPixelList(BlobPtr, TotalBlobPixelList);	}
				else
				{
					std::vector<POINT> BlobPixelList;
					BlobDetector.GetBlobPixelList(BlobPtr, BlobPixelList);					
					TotalBlobPixelList.reserve(TotalBlobPixelList.size()+BlobPixelList.size());
					TotalBlobPixelList.insert(TotalBlobPixelList.end(), BlobPixelList.begin(), BlobPixelList.end());					
				}
				
			}
		}
		BlobRect = TotalBlobRect;
		double BlobCpX=(BlobRect.left+BlobRect.right+1.0)*0.5;
		double BlobCpY=(BlobRect.top+BlobRect.bottom+1.0)*0.5;
		if ( ShapeModeCircle == ShapeMode )
		{				
			const bool bUseRadial=true;
			const int  RadialCount=360;//以每隔0.1個角度來找出平均長度
			const size_t BlobPixelCnt=TotalBlobPixelList.size();			
			std::vector<double> RadiusList(BlobPixelCnt);
			std::vector<int> EachAngleCount(RadialCount, 0);
			std::vector<int> EachAngleSum(RadialCount, 0);			
			std::vector<int> EachAngleMax(RadialCount, 0);
			std::vector<int> EachAngleMin(RadialCount, 0);
			std::vector<int> PixelAngleList(BlobPixelCnt, 0);//每個點對應的角度
			std::vector<float> PixelRadiusList(BlobPixelCnt, 0);//每個點對應的半徑			

			RadiusList.clear();
			if ( true == bUseRadial )
			{	
				PixelAngleList.clear();
				PixelRadiusList.clear();
				for ( i=0; i<BlobPixelCnt; i++ )
				{
					const POINT &Pt=TotalBlobPixelList[i];					
					double dX=(Pt.x-BlobCpX);
					double dY=(Pt.y-BlobCpY);					
					double Angle=::atan2(dY, dX)*RAD_TO_DEG_DBL;
					if ( Angle < 0 ) { Angle += 360.0; }

					dX *= ScaleX;
					dY *= ScaleY;
					double Radius=sqrt((dX*dX)+(dY*dY));
					const int AngleIndex=(int)(Angle+0.5);
					PixelRadiusList.push_back(Radius);
					PixelAngleList.push_back(AngleIndex);					
					if ( AngleIndex < RadialCount )
					{
						if ( 0 == EachAngleCount[AngleIndex] )
						{
							EachAngleCount[AngleIndex]  = 1; 							
							EachAngleSum[AngleIndex] = Radius;
							EachAngleMax[AngleIndex] = Radius;
							EachAngleMin[AngleIndex] = Radius;
						}
						else
						{							
							EachAngleCount[AngleIndex] ++;
							EachAngleSum[AngleIndex] += Radius;
							if ( EachAngleMax[AngleIndex] < Radius ) { EachAngleMax[AngleIndex] = Radius; }
							if ( EachAngleMin[AngleIndex] > Radius ) { EachAngleMin[AngleIndex] = Radius; }							
						}
					}
				}
				
				RadiusList.clear();				
				bool bFirstRadius=true;
				const bool  bUseAveMode=true;
				const int    PxlCntLSL=4;//至少幾個像素
				const float  RadiusRatioMinLSL=75.0f;//最短的半徑比例最小值
				const float  ResXY=(ScaleX+ScaleY)*0.5;//平均解析度
				const float  RangeUSL=ResXY*4;//超過4個像素時
				const size_t PixelAngleCnt=PixelAngleList.size();
				for ( int k=0; k<RadialCount; k++  )
				{					
					const int PxlCnt=EachAngleCount[k];
					if ( 0 == PxlCnt ) { continue; }					
					const double PxlMaxR=EachAngleMax[k];
					const double PxlMinR=EachAngleMin[k];
					const double PxlSumR=EachAngleSum[k];
					const double Angle=(k*DEG_TO_RAD_DBL);
					double Radius = PxlMaxR;
					//Radius = PxlSumR/PxlCnt;
					const double Range=PxlMaxR-PxlMinR;
					const double RangeRatioMin=PxlMinR*100.0f/RadiusStd;					
					//開關, 最短半徑過小(內部填滿), 數量少, 半徑誤差低
					if ( false==bUseAveMode || PxlCnt<PxlCntLSL || Range<RangeUSL )
					{	Radius = Radius;	}
					else
					{						
						std::vector<float> AngleRadiusList;
						for ( size_t s=0; s<PixelAngleCnt; s++ )
						{
							if ( k != PixelAngleList[s] ) { continue; }
							AngleRadiusList.push_back(PixelRadiusList[s]);
						}
						std::sort(AngleRadiusList.begin(), AngleRadiusList.end());//排序						
						double Sum=0.0;
						int    StartI=0, EndI=0;
						const int Total=AngleRadiusList.size();						
						if ( RangeRatioMin < RadiusRatioMinLSL )
						{	//內部填滿可考慮取較長的10%來做濾除
							StartI=Total*9/10;
							EndI = Total;
						}
						else
						{	//取中間50%
							StartI=Total/4;
							EndI = Total*3/4;
						}						
						const int Cnt=EndI-StartI;
						for ( int s=StartI; s<EndI; s++ )
						{	Sum += AngleRadiusList[s];	}
						if ( Cnt > 0 )
						{	Radius = (float)(Sum/Cnt);	}										
					}

					double PosX=(Radius*cos(Angle)/ScaleX)+BlobCpX;
					double PosY=(Radius*sin(Angle)/ScaleY)+BlobCpY;
					const int nPosX=(int)(PosX+0.5);
					const int nPosY=(int)(PosY+0.5);
					if ( true == bFirstRadius )//重新修正區域
					{
						bFirstRadius = false;
						TotalBlobRect.left = TotalBlobRect.right = nPosX;
						TotalBlobRect.top = TotalBlobRect.bottom = nPosY;
					}
					else
					{
						if ( TotalBlobRect.left > nPosX ) { TotalBlobRect.left = nPosX; }
						if ( TotalBlobRect.top  > nPosY ) { TotalBlobRect.top = nPosY; }
						if ( TotalBlobRect.right < nPosX ) { TotalBlobRect.right = nPosX; }
						if ( TotalBlobRect.bottom  < nPosY ) { TotalBlobRect.bottom = nPosY; }
					}					
					RadiusList.push_back(Radius);
				}
				BlobRect = TotalBlobRect;
				BlobCpX=(BlobRect.left+BlobRect.right+1.0)*0.5;
				BlobCpY=(BlobRect.top+BlobRect.bottom+1.0)*0.5;				
			}
			else
			{
				for ( i=0; i<BlobPixelCnt; i++ )
				{
					const POINT &Pt=TotalBlobPixelList[i];
					double dX=(Pt.x-BlobCpX)*ScaleX;
					double dY=(Pt.y-BlobCpY)*ScaleY;
					double Radius=sqrt((dX*dX)+(dY*dY));
					RadiusList.push_back(Radius);					
				}
			}
			
			RadiusMax = -DBL_MAX;
			RadiusMin =  DBL_MAX;
			std::sort(RadiusList.begin(), RadiusList.end());//排序
			const size_t RadiusCount=RadiusList.size();
			for ( i=0; i<RadiusCount; i++ )
			{	
				double Radius=RadiusList[i];
				RadiusSum += Radius;
				if ( RadiusMax < Radius ) { RadiusMax=Radius; }
				if ( RadiusMin > Radius ) { RadiusMin=Radius; }
			}			
			RadiusAve = RadiusSum/RadiusCount;
			//取前20%為外部半徑			
			double RadiusOuter=0.0;
			const int RatioOuter=20;
			const size_t CountOuter=RadiusCount*RatioOuter/100;			
			for ( i=0; i<CountOuter; i++ )
			{	RadiusOuter += RadiusList[RadiusCount-i-1];	}
			if ( CountOuter > 0 )
			{	RadiusOuter /= CountOuter; }

			//取後20%為內部半徑			
			double RadiusInner=0.0;
			const int RatioInner=20;
			const size_t CountInner=RadiusCount*RatioInner/100;			
			for ( i=0; i<CountInner; i++ )
			{	RadiusInner += RadiusList[i];	}
			if ( CountInner > 0 )
			{	RadiusInner /= CountInner; }

			svParam.svReadingRInner=RadiusInner;
			svParam.svReadingROuter=RadiusOuter;
			svParam.svReadingRAverage=RadiusAve;			
		}			
		CAOIModel::CalcModelBoxRectRegion(BlobRect, MaskW, MaskH, RgnCp, ImageScale, ImageCp, BoxRgn);
		//JetAPI::MoveRegion(BoxRgn, WndRectCalValue, BoxRgn);		
		WndPtr->GetWndBox().SetBoxRegionRes(BoxRgn);
	}

	//Judge OK/NG
	CString strResult;	
	RESULT_ID ResultID=RESULT_ID_OK;
	SetAlgResultID(ResultID);
	
	SetAlgResultReading1(svParam.svReadingRAverage);
	SetAlgResultText(_T("OK"));

	double Gap = 0;	
	double Value = 0;		
	double ValueAbs=0;
	if ( ShapeModeCircle == ShapeMode )
	{	
		Gap = svParam.svOuterTol;
		Value = (svParam.svReadingROuter-svParam.svCircleR);
		ValueAbs = ::fabs(Value);
		if ( ValueAbs > Gap )
		{	
			ResultID = RESULT_ID_NG;
			CString Key = _T("Outer");
			strResult.Format(_T("%s:%.0f (%.0f)"), Key, Value, svParam.svReadingROuter);
		}
		Gap = svParam.svInnerTol;
		Value = (svParam.svReadingRInner-svParam.svCircleR);
		ValueAbs = ::fabs(Value);
		if ( ValueAbs > Gap )
		{	
			ResultID = RESULT_ID_NG;
			CString Key = _T("Inner");
			strResult.Format(_T("%s:%.0f (%.0f)"), Key, Value, svParam.svReadingRInner);
		}

		Gap = svParam.svRangeTol;
		Value = (svParam.svReadingROuter-svParam.svReadingRInner);
		ValueAbs = ::fabs(Value);
		if ( ValueAbs > Gap )
		{	
			ResultID = RESULT_ID_NG;
			CString Key = _T("Range");
			strResult.Format(_T("%s:%.0f (%.0f)"), Key, Value, Value);
		}

		// = 100.0;
		Gap = svParam.svErrorTol;
		Value = (svParam.svReadingRAverage-svParam.svCircleR);
		ValueAbs = ::fabs(Value);
		if ( ValueAbs > Gap )
		{	
			ResultID = RESULT_ID_NG;
			CString Key = _T("Error");			
			strResult.Format(_T("%s:%.0f (%.0f)"), Key, Value, svParam.svReadingRAverage);
		}
	}	
	else
	{
		
	}
	SetAlgResultID(ResultID);
	SetAlgResultText(strResult);
	
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